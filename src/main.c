#include <stdlib.h>
#include <wlr/util/log.h>
#include "../include/wm.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    wlr_log_init(WLR_DEBUG, NULL);

    struct wm_state wm = {0};

    if (!wm_init(&wm)) {
        wlr_log(WLR_ERROR, "Failed to initialize WM");
        return 1;
    }

    wm_run(&wm);
    wm_fini(&wm);

    return 0;
}

