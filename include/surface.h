// surface.h
#ifndef SURFACE_H
#define SURFACE_H

#include <wayland-server.h>
#include <wlr/types/wlr_xdg_shell.h>

struct wm_state; // Forward declaration of your main state structure

// Declare the window structure.  This contains the wlr_xdg_surface and any
// other data you want to associate with a window.
struct window {
    struct wl_list link;            // For adding to the list of managed windows
    struct wm_state *wm;         // Pointer to the main compositor state
    struct wlr_xdg_surface *xdg_surface; // The WLR XDG surface
    int x, y;                     // Position of the window
    // Add width and height if you want to track the size.
    struct wl_listener map;       // Listener for map events
    struct wl_listener unmap;     // Listener for unmap events
    struct wl_listener destroy;   // Listener for destroy events
    // Add other window-specific data here (e.g., size, is_fullscreen, etc.)
};

// Declare the function to handle new XDG surface creation.
extern void handle_new_xdg_surface(struct wl_listener *listener, void *data);

#endif // SURFACE_H
