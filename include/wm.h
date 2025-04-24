#pragma once

#include <wayland-server-core.h>
#include <wlr/backend.h>
#include <wlr/backend/session.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_shm.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_seat.h>


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

    const char *socket;
};

bool wm_init(struct wm_state *wm);
void wm_run(struct wm_state *wm);
void wm_fini(struct wm_state *wm);

