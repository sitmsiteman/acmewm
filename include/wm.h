#pragma once

#include <wayland-server-core.h>
#include <wlr/backend.h>

struct wm_state {
    struct wl_display *display;
    struct wlr_backend *backend;
    struct wlr_session *session;
};

bool wm_init(struct wm_state *wm);
void wm_run(struct wm_state *wm);
void wm_fini(struct wm_state *wm);

