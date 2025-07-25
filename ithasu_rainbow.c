#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <gui/modules/variable_item_list.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include "app.h"
#include "views/_global.h"
#include "views/about.h"
#include "views/menu.h"

/**
 * @brief      Main function for skeleton application.
 * @details    This function is the entry point for the skeleton application.  It should be defined in
 *             application.fam as the entry_point setting.
 * @param      _p  Input parameter - unused
 * @return     0 - Success
 */
int32_t ithasu_rainbow_app(void* _p) {
    UNUSED(_p);

    App* app = app_alloc();
    about_alloc(app->view_dispatcher, APP_VIEW_MAIN);
    menu_alloc(app, VIEW_NONE);

    view_dispatcher_switch_to_view(app->view_dispatcher, APP_VIEW_MAIN);
    view_dispatcher_run(app->view_dispatcher);

    menu_free(app);
    about_free(app->view_dispatcher);
    app_free(app);

    return 0;
}
