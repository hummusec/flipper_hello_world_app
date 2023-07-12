
#include "../hello_world.h"
// #include <assets_icons.h>

#define TAG "app_scene_world"

void app_scene_world_widget_callback(GuiButtonType result, InputType type, void* context) {
    // Callback for the widget buttons
    App* app = context;
    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

void app_scene_world_update_popup(void* context) {
    App* app = context;

    // Update message in our app context with the current tick count
    snprintf(app->message, 32, "Tick count: %ld", app->tick_count);

    // Reset the popup and set our icon and text
    popup_reset(app->popup);
    popup_set_icon(app->popup, 59, 20, &I_Hi_10x10);
    popup_set_text(app->popup, app->message, 0, 0, AlignLeft, AlignTop);
}

void app_scene_world_tick(void* context) {
    App* app = context;

    // Increase tick count
    app->tick_count++;

    // Update our Popup's elements
    app_scene_world_update_popup(app);
}

void app_scene_world_on_enter(void* context) {
    App* app = context;

    // Initialize tick count
    app->tick_count = 0;

    // Initialize message
    app->message = malloc(32);

    // Prepare our 'Popup' elements once
    app_scene_world_update_popup(app);

    // Tell the view dispatcher to switch to the 'Popup' view
    view_dispatcher_switch_to_view(app->view_dispatcher, AppViewPopup);
}

bool app_scene_world_on_event(void* context, SceneManagerEvent event) {
    App* app = context;
    bool consumed = false;

    // This time we only care about the tick events
    if(event.type == SceneManagerEventTypeTick) {
        app_scene_world_tick(app);
        consumed = true;
    }

    return consumed;
}

void app_scene_world_on_exit(void* context) {
    App* app = context;
    free(app->message);
    widget_reset(app->widget);
}