
#include "../hello_world.h"

#define TAG "app_scene_menu"

// The menu items are defined in an enum
enum SubmenuIndex {
    SubmenuIndexHello,
    SubmenuIndexWorld,
    SubmenuIndexExit,
};

void app_scene_menu_submenu_callback(void* context, uint32_t index) {
    App* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void app_scene_menu_on_enter(void* context) {
    App* app = context;
    Submenu* submenu = app->submenu;

    // Set menu header title
    submenu_set_header(submenu, "Hello World App");

    // Add menu items
    submenu_add_item(
        submenu, "Hello Scene", SubmenuIndexHello, app_scene_menu_submenu_callback, app);
    submenu_add_item(
        submenu, "World Scene", SubmenuIndexWorld, app_scene_menu_submenu_callback, app);

    // Set selected menu item, the scene state is used to remember the last selected item
    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, AppSceneMenu));

    // After we set up our menu, we can switch to it's view
    view_dispatcher_switch_to_view(app->view_dispatcher, AppViewSubmenu);
}

bool app_scene_menu_on_event(void* context, SceneManagerEvent event) {
    App* app = context;
    bool consumed = false;

    // There are several ways that an event could be generated, but we only care about custom events
    if(event.type == SceneManagerEventTypeCustom) {
        // Setting the scene state to the event value will cause the menu to select the correct item
        scene_manager_set_scene_state(app->scene_manager, AppSceneMenu, event.event);

        // If a menu item is selected, we can switch to the next scene
        switch(event.event) {
        case SubmenuIndexHello:
            scene_manager_next_scene(app->scene_manager, AppSceneHello);
            consumed = true;
            break;

        case SubmenuIndexWorld:
            scene_manager_next_scene(app->scene_manager, AppSceneWorld);
            consumed = true;
            break;

        case SubmenuIndexExit:
            // If there are no previous scenes, this will cause the application to exit
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;

        default:
            break;
        }
    }

    return consumed;
}

void app_scene_menu_on_exit(void* context) {
    App* app = context;
    submenu_reset(app->submenu);
}