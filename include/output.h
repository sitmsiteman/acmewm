#ifndef OUTPUT_H
#define OUTPUT_H

// Include necessary WLR headers (or your main wm.h if it includes them)
#include <wayland-util.h>
#include <wlr/types/wlr_output.h>

// Declare the handle_new_output function so other files can use it
extern void handle_new_output(struct wl_listener *listener, void *data);

#endif
