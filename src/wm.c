#include <stdlib.h>
#include <stdbool.h>
#include <wlr/backend.h>
#include <wlr/util/log.h>
#include "../include/wm.h"

bool wm_init(struct wm_state *wm) {
    wm->display = wl_display_create();
    if (!wm->display) {
        wlr_log(WLR_ERROR, "Could not create Wayland display");
        return false;
    }

    struct wl_event_loop *loop = wl_display_get_event_loop(wm->display);
    wm->backend = wlr_backend_autocreate(loop, &wm->session);
    if (!wm->backend) {
        wlr_log(WLR_ERROR, "Failed to create backend");
        return false;
    }

    if (!wlr_backend_start(wm->backend)) {
        wlr_log(WLR_ERROR, "Failed to start backend");
        return false;
    }

    const char *socket = wl_display_add_socket_auto(wm->display);
    if (!socket) {
        wlr_log(WLR_ERROR, "Failed to add Wayland socket");
        return false;
    }

    setenv("WAYLAND_DISPLAY", socket, 1);
    wlr_log(WLR_INFO, "Running Wayland compositor on WAYLAND_DISPLAY=%s", socket);

    return true;
}

void wm_run(struct wm_state *wm) {
    wl_display_run(wm->display);
}

void wm_fini(struct wm_state *wm) {
    if (wm->backend) {
        wlr_backend_destroy(wm->backend);
    }
    if (wm->display) {
        wl_display_destroy(wm->display);
    }
}

