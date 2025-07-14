#pragma once

#include <gui/gui.h>
#include <gui/view_dispatcher.h>

typedef struct {
    // -- GUI --
    ViewDispatcher* view_dispatcher; // Switches between our views
} App;

/** Initialize a new app */
App* app_alloc();

/** Release all memory used by the app */
void app_free(App* app);
