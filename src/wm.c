#include "../include/wm.h"
#include <stdbool.h>                      // for false, true, bool
#include <stdlib.h>                       // for setenv
#include <wayland-util.h>                 // for wl_container_of
#include <wlr/backend.h>                  // for wlr_backend_autocreate, wlr...
#include <wlr/render/allocator.h>         // for wlr_allocator_autocreate
#include <wlr/render/wlr_renderer.h>      // for wlr_renderer_autocreate
#include <wlr/types/wlr_data_device.h>    // for wlr_data_device_manager_create
#include <wlr/types/wlr_output.h>         // for wlr_output, wlr_output_mode
#include <wlr/types/wlr_seat.h>           // for wlr_seat_create
#include <wlr/types/wlr_shm.h>            // for wlr_shm_create_with_renderer
#include <wlr/types/wlr_subcompositor.h>  // for wlr_subcompositor_create
#include <wlr/types/wlr_xdg_shell.h>      // for wlr_xdg_shell_create
#include <wlr/util/log.h>                 // for wlr_log, wlr_log_importance
#include "wlr/types/wlr_compositor.h"     // for wlr_compositor_create

static void
handle_new_output(struct wl_listener *listener, void *data)
{
	struct wm_state *wm = wl_container_of(listener, wm, new_output);
	struct wlr_output *output = data;

	wlr_log(WLR_INFO, "New output detected: %s", output->name);

	wm->allocator = wlr_allocator_autocreate(wm->backend, wm->renderer);
	if (!wm->allocator) {
		wlr_log(WLR_ERROR, "Failed to create allocator");
		return;
	}


	if (!wlr_output_init_render(output, wm->allocator,wm->renderer)) {
		wlr_log(WLR_ERROR, "Failed to init output rendering");
		return;
	}


	struct wlr_output_state state;
	wlr_output_state_init(&state);

	wlr_output_state_set_enabled(&state, true);

	struct wlr_output_mode *mode = wlr_output_preferred_mode(output);


	if (mode) {
		wlr_log(WLR_INFO, "Preferred mode for %s: %dx%d @ %.3fHz",
			output->name, mode->width, mode->height, (double)mode->refresh / 1000.0);
	} else {
		wlr_log(WLR_INFO, "No preferred mode for output %s", output->name);
	}

	if (!mode) {
		wlr_log(WLR_INFO, "Setting custom mode for %s: 1280x720 @ 60Hz",
			output->name);
		wlr_output_state_set_custom_mode(&state, 1280, 720, 0);
	} else {
		wlr_output_state_set_mode(&state, mode);
	}



	wlr_log(WLR_INFO, "Committing state for output %s", output->name);
	if (!wlr_output_commit_state(output, &state)) {
		wlr_log(WLR_ERROR, "Failed to commit output state for %s", output->name);
	} else {
		wlr_log(WLR_DEBUG, "Output state committed for %s", output->name);
	}

	wlr_output_state_finish(&state);
}

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

	wm->compositor = wlr_compositor_create(wm->display, 6,wm->renderer);
	if (!wm->compositor) {
		wlr_log(WLR_ERROR, "Failed to create compositor");
		return false;
	}

	wm->data_device_manager = wlr_data_device_manager_create(wm->display);
	if (!wm->data_device_manager) {
		wlr_log(WLR_ERROR, "Failed to create data device manager");
		return false;
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

