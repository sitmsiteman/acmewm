#include "../include/wm.h"
#include "../include/output.h"
#include "../include/surface.h"
#include <stdbool.h>
#include <stdlib.h>
#include <wayland-util.h>
#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_primary_selection_v1.h>
#include <wlr/types/wlr_cursor_shape_v1.h>
#include <wlr/types/wlr_text_input_v3.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_management_v1.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_shm.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

bool
wm_init(struct wm_state *wm)
{
	wm->display = wl_display_create();
	if (!wm->display) {
		wlr_log(WLR_ERROR, "Could not create Wayland display");
		return false;
	}

	wm->loop = wl_display_get_event_loop(wm->display);
	if (!wm->loop) {
		wlr_log(WLR_ERROR, "Failed to get event loop");
		return false;
	}


	wm->backend = wlr_backend_autocreate(wm->loop, &wm->session);
	if (!wm->backend) {
		wlr_log(WLR_ERROR, "Failed to create backend");
		return false;
	}

	wm->renderer = wlr_renderer_autocreate(wm->backend);
	if (!wm->renderer) {
		wlr_log(WLR_ERROR, "Failed to get renderer from backend");
		return false;
	}

	// Setup output listener
	wm->new_output.notify = handle_new_output;
	wl_signal_add(&wm->backend->events.new_output, &wm->new_output);

	if (!wlr_backend_start(wm->backend)) {
		wlr_log(WLR_ERROR, "Failed to start backend");
		return false;
	}

	wm->compositor = wlr_compositor_create(wm->display, 6, wm->renderer);
	if (!wm->compositor) {
		wlr_log(WLR_ERROR, "Failed to create compositor");
		return false;
	}

//to do.
	
	if (!wm->region) {
		wlr_log(WLR_ERROR, "Failed to create compositor region");
		return false;
	}

	wm->data_device_manager = wlr_data_device_manager_create(wm->display);
	if (!wm->data_device_manager) {
		wlr_log(WLR_ERROR, "Failed to create data device manager");
		return false;
	}

	wm->primary_selection = wlr_primary_selection_v1_device_manager_create (wm->display);
	if (!wm->primary_selection) {
		wlr_log(WLR_ERROR,
			"Failed to create primary selection manager");
	}

	wm->cursor_shape = wlr_cursor_shape_manager_v1_create(wm->display, 1);
	if (!wm->cursor_shape) {
		wlr_log(WLR_ERROR, "Failed to create cursor shape manager");
	}

	wm->text_input = wlr_text_input_manager_v3_create(wm->display);
	if (!wm->text_input) {
		wlr_log(WLR_ERROR, "Failed to create text input manager");
	}

	wm->seat = wlr_seat_create(wm->display, "seat0");
	if (!wm->seat) {
		wlr_log(WLR_ERROR, "Failed to create wlr_seat");
		return false;
	}

	wm->subcompositor = wlr_subcompositor_create (wm->display);
	if (!wm->subcompositor) {
		wlr_log(WLR_ERROR, "Failed to create subcompositor");
		return false;
	}

	wm->shm = wlr_shm_create_with_renderer(wm->display, 2, wm->renderer);
	if (!wm->shm) {
		wlr_log(WLR_ERROR, "Failed to create shm interface");
		return false;
	}

	wm->xdg_shell = wlr_xdg_shell_create(wm->display,1);
	if (!wm->xdg_shell) {
		wlr_log(WLR_ERROR, "Failed to create XDG shell");
		return false;
	}

	surface_init(wm);

	wm->socket = wl_display_add_socket_auto(wm->display);
	if (!wm->socket) {
		wlr_log(WLR_ERROR, "Failed to add Wayland socket");
		return false;
	}

	setenv("WAYLAND_DISPLAY", wm->socket, 1);
	wlr_log(WLR_INFO, "Running Wayland compositor on WAYLAND_DISPLAY=%s", wm->socket);

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

