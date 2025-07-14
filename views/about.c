#include "_global.h"
#include "about.h"
#include <gui/view_stack.h>
#include <gui/modules/text_box.h>

static TextBox* text_box;
static uint32_t back_view_id = VIEW_NONE;

static uint32_t about_previous_callback(void* context) {
    UNUSED(context);
    return back_view_id;
}

void about_alloc(ViewDispatcher* view_dispatcher, uint32_t my_back_view_id) {
    // -- Register back view --
    back_view_id = my_back_view_id;

    // -- Create the text box --
    text_box = text_box_alloc();
    text_box_set_font(text_box, TextBoxFontHex);
    text_box_set_focus(text_box, TextBoxFocusStart);
    text_box_set_text(
        text_box,
        "A very long text describing the goal of the application...\nThis is natural for it to be very long.\n\n\n\n\n\n\n\nThis is secret, only visible if you scroll long enough.");

    View* view = text_box_get_view(text_box);
    view_set_previous_callback(view, &about_previous_callback);

    // -- Register the view --
    view_dispatcher_add_view(view_dispatcher, APP_VIEW_ABOUT, view);
}

void about_free(ViewDispatcher* view_dispatcher) {
    UNUSED(view_dispatcher);
    // Free the text box
    view_dispatcher_remove_view(view_dispatcher, APP_VIEW_ABOUT);
    text_box_free(text_box);
}
