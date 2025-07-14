#include <gui/view_dispatcher.h>
#include <gui/modules/variable_item_list.h>

#include "menu.h"
#include "_global.h"
#include "../app.h"
#include "../led_driver.h"

#define MENU_MAX_LED_COUNT 32

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

typedef struct {
    uint8_t led_count;
    uint8_t led_index;
    uint8_t led_color_index[MENU_MAX_LED_COUNT];
} LedState;

VariableItemList* item_list;
VariableItem* count_item;
VariableItem* index_item;
VariableItem* color_item;

static uint32_t back_view_id = VIEW_NONE;
static LedState state;

/** Convert menu state to led */
static void update_leds(bool leds) {
    if(leds) {
        // -- Update led driver --
        LedDriver* led_driver = led_driver_alloc(state.led_count, &gpio_swclk);
        for(uint32_t i = 0; i < state.led_count; i++) {
            led_driver_set_led(led_driver, i, COLORS[state.led_color_index[i]].rgb);
        }
        led_driver_transmit(led_driver, true);
        led_driver_free(led_driver);

        // -- Check menu consistency --
        if(state.led_index >= state.led_count) {
            state.led_index = 0;
        }
    }

    // -- Update menu --
    FuriString* str = furi_string_alloc();
    // Led count
    furi_string_reset(str);
    furi_string_printf(str, "%d", state.led_count);
    variable_item_set_current_value_index(count_item, state.led_count);
    variable_item_set_current_value_text(count_item, furi_string_get_cstr(str));

    // Led index
    furi_string_reset(str);
    furi_string_printf(str, "%d", state.led_index);
    variable_item_set_values_count(index_item, state.led_count);
    variable_item_set_current_value_index(index_item, state.led_index);
    variable_item_set_current_value_text(index_item, furi_string_get_cstr(str));

    // Led color
    // DO NOT UPDATE, keep current state
    // const char* name = COLORS[state.led_color_index[state.led_index]].name;
    // variable_item_set_current_value_text(color_item, name);

    furi_string_free(str);
}

static uint32_t menu_previous_callback(void* context) {
    UNUSED(context);
    return back_view_id;
}

static void count_item_changed_callback(VariableItem* item) {
    uint8_t value = variable_item_get_current_value_index(item);
    state.led_count = value;
    update_leds(true);
}

static void index_item_changed_callback(VariableItem* item) {
    uint8_t value = variable_item_get_current_value_index(item);
    state.led_index = value;
    update_leds(false);
}

static void color_item_changed_callback(VariableItem* item) {
    uint8_t value = variable_item_get_current_value_index(item);
    state.led_color_index[state.led_index] = value;
    const char* name = COLORS[value].name;
    variable_item_set_current_value_text(color_item, name);

    update_leds(true);
}

static void menu_enter_callback(void* context, uint32_t index) {
    App* app = context;
    switch(index) {
    case 0: { // LED count
        uint8_t value = variable_item_get_current_value_index(count_item);
        state.led_count = value;
        update_leds(true);
        break;
    }
    case 1: { // LED index
        uint8_t value = variable_item_get_current_value_index(index_item);
        state.led_index = value;
        break;
    }
    case 2: { // LED color
        uint8_t value = variable_item_get_current_value_index(color_item);
        state.led_color_index[state.led_index] = value;
        state.led_index = (state.led_index + 1) % state.led_count;
        update_leds(true);
        break;
    }
    case 3: { // About
        view_dispatcher_switch_to_view(app->view_dispatcher, APP_VIEW_ABOUT);
        break;
    }
    }
}

/** Initialize main view */
void menu_alloc(App* app, uint32_t my_back_view_id) {
    back_view_id = my_back_view_id;
    state.led_count = 8;
    for(uint8_t i = 0; i < MENU_MAX_LED_COUNT; i++) {
        state.led_color_index[i] = 7;
    }

    item_list = variable_item_list_alloc();
    View* view = variable_item_list_get_view(item_list);

    view_dispatcher_add_view(app->view_dispatcher, APP_VIEW_MAIN, view);
    view_set_previous_callback(view, &menu_previous_callback);

    count_item = variable_item_list_add(
        item_list, //
        "LED count", //
        MENU_MAX_LED_COUNT, //
        &count_item_changed_callback, //
        NULL);
    index_item = variable_item_list_add(
        item_list, //
        "LED index", //
        MENU_MAX_LED_COUNT, //
        &index_item_changed_callback, //
        NULL);
    color_item = variable_item_list_add(
        item_list, //
        "Color", //
        COUNT_OF(COLORS), //
        &color_item_changed_callback, //
        NULL);
    variable_item_list_add(
        item_list, //
        "About", //
        0, //
        NULL, //
        app);

    variable_item_list_set_enter_callback(item_list, &menu_enter_callback, app);

    // -- Update leds & menu --
    update_leds(true);
}

/** Free main view */
void menu_free(App* app) {
    view_dispatcher_remove_view(app->view_dispatcher, APP_VIEW_MAIN);
    variable_item_list_free(item_list);
}
