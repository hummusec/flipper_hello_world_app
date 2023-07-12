
#include "../hello_world.h"
// #include <assets_icons.h>

#define TAG "app_scene_hello"

void app_scene_hello_widget_callback(GuiButtonType result, InputType type, void* context) {
    // Callback for the widget buttons
    App* app = context;
    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

void app_scene_hello_update_widget(App* app) {
    // Clear the widget
    widget_reset(app->widget);

    widget_add_text_scroll_element(
        app->widget, // Widget
        0, // X position
        0, // Y position
        128, // Width
        52, // Height
        "Hello!\nThis is a scrollable & formatable text box. you can add:\n\e#Bold text\n\e*Monospaced text\n\ecCentered Text\n\erRight-aligned text");

    widget_add_button_element(
        app->widget, GuiButtonTypeRight, "Action", app_scene_hello_widget_callback, app);
    // widget_add_button_element(
    //     app->widget, GuiButtonTypeCenter, "OK", app_scene_hello_widget_callback, app);
    widget_add_button_element(
        app->widget, GuiButtonTypeLeft, "Back", app_scene_hello_widget_callback, app);
}

void app_scene_hello_on_enter(void* context) {
    App* app = context;

    // Initialize the widget and add the elements
    app_scene_hello_update_widget(app);

    // Switch to the Widget view
    view_dispatcher_switch_to_view(app->view_dispatcher, AppViewWidget);
}

bool app_scene_hello_on_event(void* context, SceneManagerEvent event) {
    App* app = context;
    SceneManager* scene_manager = app->scene_manager;
    bool consumed = false;

    // There are several ways that an event could be generated, but we only care about custom events
    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case EventGuiButtonTypeRight: // "Action" button
            notification_message(app->notifications, &sequence_success);
            consumed = true;
            break;

            // case EventGuiButtonTypeCenter:
            //     // Your custom code here!
            //     consumed = true;
            //     break;

        case EventGuiButtonTypeLeft: // "Back" button
            // Tell the scene manager to go back to the previous scene
            scene_manager_previous_scene(scene_manager);
            consumed = true;
            break;

        default:
            break;
        }
    }

    return consumed;
}

void app_scene_hello_on_exit(void* context) {
    App* app = context;

    // Clear the widget before leaving the scene
    widget_reset(app->widget);
}