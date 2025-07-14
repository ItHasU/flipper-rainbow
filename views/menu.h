#include <gui/view_dispatcher.h>
#include "../led_driver.h"
#include "../app.h"

/** Initialize main view */
void menu_alloc(App* app, uint32_t my_back_view_id);
/** Free main view */
void menu_free(App* app);
