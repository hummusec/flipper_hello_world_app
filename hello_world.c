
#include "hello_world.h"

#define TAG "hello_world_app"

static bool app_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool app_back_event_callback(void* context) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void app_tick_event_callback(void* context) {
    furi_assert(context);
    App* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

static App* app_alloc() {
    // Allocate enough memory for the app
    App* app = malloc(sizeof(App));

    // Opening the records to shared services of the Flipper Zero.
    app->storage = furi_record_open(RECORD_STORAGE);
    app->dialogs = furi_record_open(RECORD_DIALOGS);

    // Initialize view dispatcher and scene manager
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&app_scene_handlers, app);
    view_dispatcher_enable_queue(app->view_dispatcher);

    // Set event callback application context
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    // Set event callback for custom events
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, app_custom_event_callback);

    // Set event callback for pressing the back button
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, app_back_event_callback);

    // Set event callback for tick events, the tick event is called every 500 ms
    view_dispatcher_set_tick_event_callback(app->view_dispatcher, app_tick_event_callback, 500);

    // Open GUI record
    app->gui = furi_record_open(RECORD_GUI);

    // Open Notification record
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Submenu
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, AppViewSubmenu, submenu_get_view(app->submenu));

    // Dialog
    app->dialog_ex = dialog_ex_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, AppViewDialogEx, dialog_ex_get_view(app->dialog_ex));

    // Popup
    app->popup = popup_alloc();
    view_dispatcher_add_view(app->view_dispatcher, AppViewPopup, popup_get_view(app->popup));

    // Loading
    app->loading = loading_alloc();
    view_dispatcher_add_view(app->view_dispatcher, AppViewLoading, loading_get_view(app->loading));

    // Widget
    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, AppViewWidget, widget_get_view(app->widget));

    // Variable Item List
    app->variable_item_list = variable_item_list_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        AppViewVariableItemList,
        variable_item_list_get_view(app->variable_item_list));

    // Text Input
    app->text_input = text_input_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, AppViewTextInput, text_input_get_view(app->text_input));

    return app;
}

static void app_free(App* app) {
    furi_assert(app);

    // Submenu
    view_dispatcher_remove_view(app->view_dispatcher, AppViewSubmenu);
    submenu_free(app->submenu);

    // DialogEx
    view_dispatcher_remove_view(app->view_dispatcher, AppViewDialogEx);
    dialog_ex_free(app->dialog_ex);

    // Popup
    view_dispatcher_remove_view(app->view_dispatcher, AppViewPopup);
    popup_free(app->popup);

    // Loading
    view_dispatcher_remove_view(app->view_dispatcher, AppViewLoading);
    loading_free(app->loading);

    // Widget
    view_dispatcher_remove_view(app->view_dispatcher, AppViewWidget);
    widget_free(app->widget);

    // Variable Item List
    view_dispatcher_remove_view(app->view_dispatcher, AppViewVariableItemList);
    variable_item_list_free(app->variable_item_list);

    // TextInput
    view_dispatcher_remove_view(app->view_dispatcher, AppViewTextInput);
    text_input_free(app->text_input);

    // View Dispatcher
    view_dispatcher_free(app->view_dispatcher);

    // Scene Manager
    scene_manager_free(app->scene_manager);

    // GUI
    furi_record_close(RECORD_GUI);
    app->gui = NULL;

    // Notifications
    furi_record_close(RECORD_NOTIFICATION);
    app->notifications = NULL;

    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_DIALOGS);

    free(app);
}

// Application Entry Point
int32_t hello_world_app(void* p) {
    // Initialize application context
    App* app = app_alloc();

    // For an unused parameter in our function, we can use UNUSED macro.
    UNUSED(p);

    // Attach our view dispatcher to GUI
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Set the first scene --> Menu
    scene_manager_next_scene(app->scene_manager, AppSceneMenu);

    // Run the view dispatcher
    view_dispatcher_run(app->view_dispatcher);

    // Once we are done, we can free the application context
    app_free(app);

    return 0;
}