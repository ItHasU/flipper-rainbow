#include <gui/view_dispatcher.h>
#include <gui/modules/variable_item_list.h>

#include "menu.h"
#include "_global.h"
#include "../app.h"

VariableItemList* item_list;
VariableItem* color_item;

PushColorCallback color_callback;
static uint32_t back_view_id = VIEW_NONE;

typedef struct {
    uint32_t rgb;
    const char* name;
} ColorDef;

static ColorDef COLORS[] = {
    {.rgb = 0x00000000, .name = "black"},
    {.rgb = 0x00ff0000, .name = "red"},
    {.rgb = 0x0000ff00, .name = "green"},
    {.rgb = 0x000000ff, .name = "blue"},
    {.rgb = 0x0000ffff, .name = "cyan"},
    {.rgb = 0x00ffff00, .name = "yellow"},
    {.rgb = 0x00ff00ff, .name = "pink"},
    {.rgb = 0x00ffffff, .name = "white"},
};

static uint32_t menu_previous_callback(void* context) {
    UNUSED(context);
    return back_view_id;
}

static void item_color_changed_callback(VariableItem* item) {
    uint8_t value = variable_item_get_current_value_index(item);
    const char* name = COLORS[value].name;
    variable_item_set_current_value_text(item, name);
}

static void menu_enter_callback(void* context, uint32_t index) {
    App* app = context;
    switch(index) {
    case 0: { // Color
        uint8_t value = variable_item_get_current_value_index(color_item);
        uint32_t color = COLORS[value].rgb;
        (*color_callback)(color);
        break;
    }
    case 1: { // About
        view_dispatcher_switch_to_view(app->view_dispatcher, APP_VIEW_ABOUT);
        break;
    }
    }
}

/** Initialize main view */
void menu_alloc(App* app, PushColorCallback my_color_callback, uint32_t my_back_view_id) {
    back_view_id = my_back_view_id;
    color_callback = my_color_callback;

    item_list = variable_item_list_alloc();
    View* view = variable_item_list_get_view(item_list);

    view_dispatcher_add_view(app->view_dispatcher, APP_VIEW_MAIN, view);
    view_set_previous_callback(view, &menu_previous_callback);

    color_item = variable_item_list_add(item_list, "Color", 8, &item_color_changed_callback, app);
    variable_item_list_add(item_list, "About", 0, NULL, app);
    item_color_changed_callback(color_item);

    variable_item_list_set_enter_callback(item_list, &menu_enter_callback, app);
}

/** Free main view */
void menu_free(App* app) {
    view_dispatcher_remove_view(app->view_dispatcher, APP_VIEW_MAIN);
    variable_item_list_free(item_list);
}
