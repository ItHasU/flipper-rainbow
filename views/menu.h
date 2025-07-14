#include <gui/view_dispatcher.h>
#include "../app.h"

typedef void (*PushColorCallback)(uint32_t color);

/** Initialize main view */
void menu_alloc(App* app, PushColorCallback my_color_callback, uint32_t my_back_view_id);
/** Free main view */
void menu_free(App* app);
