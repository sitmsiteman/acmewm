#include "../include/wm.h"
#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_management_v1.h>
#include <wlr/util/log.h>

void
handle_new_output(struct wl_listener *listener, void *data)
{
	struct wm_state *wm = wl_container_of(listener, wm, new_output);
	struct wlr_output *output = data;
	struct wlr_output_configuration_v1 *output_config;
	struct wlr_output_configuration_head_v1 *output_config_head;


	wlr_log(WLR_INFO, "New output detected: %s", output->name);

	wm->allocator = wlr_allocator_autocreate(wm->backend, wm->renderer);
	if (!wm->allocator) {
		wlr_log(WLR_ERROR, "Failed to create allocator");
		return;
	}

	output->phys_width = 480;  // mm
	output->phys_height = 270; // mm

	output->make = "ACME";
	output->model = "WM-1000";

	if (!wlr_output_init_render(output, wm->allocator, wm->renderer)) {
		wlr_log(WLR_ERROR, "Failed to init output rendering");
		return;
	}

	struct wlr_output_state state;
	wlr_output_state_init(&state);
	wlr_output_state_set_enabled(&state, true);

	struct wlr_output_mode *mode = wlr_output_preferred_mode(output);

	if (!mode) {
		wlr_log(WLR_INFO, "No preferred mode for output %s", output->name);
		wlr_log(WLR_INFO, "Setting custom mode for %s: 1920x1080 @ 0Hz", output->name);
		wlr_output_state_set_custom_mode(&state, 1920, 1080, 0);
	} else {
		wlr_log(WLR_INFO, "Preferred mode for %s: %dx%d @ %.3fHz",
			output->name, mode->width, mode->height, (double)mode->refresh / 1000.0);
		wlr_output_state_set_mode(&state, mode);
	}

	wlr_log(WLR_INFO, "Committing state for output %s", output->name);

	if (!wlr_output_commit_state(output, &state)) {
		wlr_log(WLR_ERROR, "Failed to commit output state for %s",
			output->name);
		return;
	} else {
		wlr_log(WLR_DEBUG, "Output state committed for %s",
			output->name);
	}


	wlr_output_state_finish(&state);

	wm->output_manager = wlr_output_manager_v1_create(wm->display);
	if (!wm->output_manager) {
		wlr_log(WLR_ERROR, "Failed to create output manager");
		return;
	}

	output_config = wlr_output_configuration_v1_create();
	if (!output_config) {
		wlr_log(WLR_ERROR, "Failed to create config");
		return;
	}

	output_config_head =
		wlr_output_configuration_head_v1_create(output_config,output);

	if (!output_config_head) {
		wlr_log(WLR_ERROR, "Failed to create config head");
		return;
	}

	wlr_output_manager_v1_set_configuration(wm->output_manager,
						output_config);

	wlr_output_set_description(output, "output");
	wlr_output_create_global(output, wm->display);
	wlr_log(WLR_INFO, "Output configured: %s (%dx%dmm %dx%d@%dHz)",
		output->name, output->phys_width, output->phys_height,
		output->width, output->height, output->refresh/1000);
}

