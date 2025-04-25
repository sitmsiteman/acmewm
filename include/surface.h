#include <wayland-server-core.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_xdg_shell.h>

struct wm_window {
    struct wm_state *wm;
    struct wlr_xdg_surface *xdg_surface;
    struct wl_list link;
    
    struct wl_listener map;
    struct wl_listener unmap;
    struct wl_listener destroy;
};
void surface_init(struct wm_state *wm);
void surface_finish(struct wm_state *wm);
