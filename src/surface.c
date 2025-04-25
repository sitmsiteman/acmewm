// surface.c
#include "wm.h"      // Your main header file
#include "surface.h" // Include the header for this file
#include <stdlib.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>
#include <wayland-server.h>
#include <wlr/types/wlr_output.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/render/allocator.h>

// Function to handle the map event of an XDG surface
static void handle_xdg_surface_map(struct wl_listener *listener, void *data) {
    struct window *window = wl_container_of(listener, window, map);
    wlr_log(WLR_INFO, "Surface mapped (title: %s)", window->xdg_surface->toplevel->title);

    // Add the window to the list of managed windows
    wl_list_insert(&window->wm->windows, &window->link);

    // Set initial position (you can implement your layout logic here)
    window->x = 100; // Example: initial x position
    window->y = 100; // Example: initial y position

    // Set window capabilities (Corrected)
 //   wlr_xdg_toplevel_set_max_size(window->xdg_surface, 0, 0);
//    wlr_xdg_toplevel_set_min_size(window->xdg_surface, 0, 0);

    // Bring the window to the top (set focus) -  You'll need to implement focus management
    // You'll likely want to set keyboard focus here.  This is a placeholder.
    // The following is still a placeholder, as proper focus management is more involved.
    // You'll need to track focused surfaces and handle keyboard input.
    // This is a simplified example and might not be fully correct for all cases.
    if (window->wm->seat) {
   //     wlr_seat_set_keyboard_focus(window->wm->seat, window->xdg_surface->surface, NULL);
        // Notify the pointer that it entered the surface.  You might need to adjust the coordinates.
        wlr_seat_pointer_notify_enter(window->wm->seat, window->xdg_surface->surface, window->x, window->y);
    }
}

// Function to handle the unmap event of an XDG surface
static void handle_xdg_surface_unmap(struct wl_listener *listener, void *data) {
    struct window *window = wl_container_of(listener, window, unmap);
    wlr_log(WLR_INFO, "Surface unmapped (title: %s)", window->xdg_surface->toplevel->title);

    // Remove the window from the list of managed windows
    wl_list_remove(&window->link);

    // Free the memory associated with the window
    free(window); //  Make sure all memory allocated for the window is freed.
}

// Function to handle the destroy event of an XDG surface
static void handle_xdg_surface_destroy(struct wl_listener *listener, void *data) {
    struct window *window = wl_container_of(listener, window, destroy);
    wlr_log(WLR_INFO, "Surface destroyed (title: %s)", window->xdg_surface->toplevel->title);
    free(window);
}

// Function to handle the new XDG surface creation
void handle_new_xdg_surface(struct wl_listener *listener, void *data) {
    struct wm_state *wm = wl_container_of(listener, wm, new_xdg_surface);
    struct wlr_xdg_surface *xdg_surface = data;

    if (xdg_surface->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL) {
        return; // We only manage top-level windows in this example
    }

    wlr_log(WLR_INFO, "New XDG surface created (role: %d)", xdg_surface->role);

    // Allocate memory for our custom window structure
    struct window *window = malloc(sizeof(struct window));
    if (!window) {
        wlr_log(WLR_ERROR, "Failed to allocate memory for window");
        return; // Handle allocation failure
    }
    window->wm = wm;
    window->xdg_surface = xdg_surface;
    window->x = 0;
    window->y = 0;

    // Set up listeners for map and unmap events
    window->map.notify = handle_xdg_surface_map;
    wl_signal_add(&xdg_surface->events.map, &window->map);
    window->unmap.notify = handle_xdg_surface_unmap;
    wl_signal_add(&xdg_surface->events.unmap, &window->unmap);
    window->destroy.notify = handle_xdg_surface_destroy;
    wl_signal_add(&xdg_surface->events.destroy, &window->destroy);

    xdg_surface->data = window; // Store a pointer to our window data within the wlr_xdg_surface

}
