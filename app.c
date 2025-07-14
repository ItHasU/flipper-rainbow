#include "app.h"

App* app_alloc() {
    App* app = (App*)malloc(sizeof(App));

    // -- Register GUI --
    Gui* gui = furi_record_open(RECORD_GUI);

    // -- Create the view dispatcher --
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, &app);

    return app;
}

void app_free(App* app) {
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close(RECORD_GUI);

    free(app);
}
