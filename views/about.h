#pragma once
#include <gui/view_dispatcher.h>

#include "_global.h"

/** Initialize */
void about_alloc(ViewDispatcher* view_dispatcher, uint32_t back_view_id);

/** Free at the end of the app */
void about_free(ViewDispatcher* view_dispatcher);
