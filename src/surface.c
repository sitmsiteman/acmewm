
#include "../include/wm.h"
#include "../include/surface.h"
#include <stdlib.h>
#include <wayland-util.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

#include "../include/wm.h"
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

static void
handle_map(struct wl_listener *listener, void *data)
{

    struct wm_window *window = wl_container_of(listener, window, map);
    wlr_log(WLR_INFO, "XDG surface mapped");
}

static void
handle_unmap(struct wl_listener *listener, void *data)
{

    struct wm_window *window = wl_container_of(listener, window, unmap);
    wlr_log(WLR_INFO, "XDG surface unmapped");
}

static void
handle_destroy(struct wl_listener *listener, void *data)
{

    struct wm_window *window = wl_container_of(listener, window, destroy);
    wlr_log(WLR_INFO, "XDG surface destroyed");
    
    wl_list_remove(&window->link);
    wl_list_remove(&window->map.link);
    wl_list_remove(&window->unmap.link);
    wl_list_remove(&window->destroy.link);
    free(window);
}

static void handle_new_xdg_surface(struct wl_listener *listener, void *data) {
    struct wm_state *wm = wl_container_of(listener, wm, new_xdg_surface);
    struct wlr_xdg_surface *xdg_surface = data;
    
    if (xdg_surface->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL) {
        return;
    }

    struct wm_window *window = calloc(1, sizeof(struct wm_window));
    window->wm = wm;
    window->xdg_surface = xdg_surface;
    
    // Correct signal names for wlroots 0.18
    window->map.notify = handle_map;
    wl_signal_add(&xdg_surface->surface->events.map, &window->map);
    
    window->unmap.notify = handle_unmap;
    wl_signal_add(&xdg_surface->surface->events.unmap, &window->unmap);
    
    window->destroy.notify = handle_destroy;
    wl_signal_add(&xdg_surface->events.destroy, &window->destroy);
    
    wl_list_insert(&wm->windows, &window->link);
}

void surface_init(struct wm_state *wm) {
    wl_list_init(&wm->windows);
    wm->new_xdg_surface.notify = handle_new_xdg_surface;
    wl_signal_add(&wm->xdg_shell->events.new_surface, &wm->new_xdg_surface);
}

void surface_finish(struct wm_state *wm) {
    struct wm_window *window, *tmp;
    wl_list_for_each_safe(window, tmp, &wm->windows, link) {
        wl_list_remove(&window->link);
        free(window);
    }
}
