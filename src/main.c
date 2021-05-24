#include "common.h"
#include "uci_conifd.h"
#include "ping.h"
#include "script.h"

void detect(struct intf *f) {
    if (f->conf_before_check == 'y')
        task_error_check_run(f);
    else
        f->state = UNKNOWN;

    if (f->state == UP || f->state == UNKNOWN) {
        if (ping(f->conf_dest, f->conf_timeout) == EXIT_SUCCESS)
            f->state = ONLINE;
        else
            f->state = OFFLINE;

        task_scripts_run(f);
    }
}

int main() {
    struct intf frame[1] = {0};
    int idx              = 0;

    if (load_config(frame) == EXIT_FAILURE) {
        debug("/etc/config/conifd read error.\n");
        exit(EXIT_FAILURE);
    }
    debug("src = %s, dest = %s, interval = %d, timeout = %d, before_check = %c\n",
          frame[idx].conf_src, frame[idx].conf_dest, frame[idx].conf_interval, frame[idx].conf_timeout, frame[idx].conf_before_check);

    while (1) {
        detect(frame);
        sleep(frame[0].conf_interval);
    }

    return 0;
}
