#pragma once

#include <stdbool.h>
#include <wayland-server-core.h>


struct wm_state {
    struct wl_display *display;
    struct wlr_backend *backend;
    struct wlr_session *session;
    struct wlr_renderer *renderer;
    struct wlr_compositor *compositor;
    struct wl_event_loop *loop;
    struct wlr_seat *seat;
    struct wlr_data_device_manager *data_device_manager;
    struct wlr_subcompositor *subcompositor;
    struct wlr_shm *shm;
    struct wlr_xdg_shell *xdg_shell;
    struct wlr_output_mode *mode;
    struct wlr_allocator *allocator;
    struct wl_listener new_output;
    struct wl_listener new_xdg_surface;
    struct wlr_output_manager_v1 *output_manager;
    struct wl_list windows;

    struct wlr_primary_selection_v1_device_manager *primary_selection;
    struct wlr_cursor_shape_manager_v1 *cursor_shape;
    struct wlr_text_input_manager_v3 *text_input;

    const char *socket;
};

bool wm_init(struct wm_state *wm);
void wm_run(struct wm_state *wm);
void wm_fini(struct wm_state *wm);

