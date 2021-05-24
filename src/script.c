#include "common.h"

#include <sys/types.h>
#include <sys/wait.h>

#define CMD_LEN 512

void task_scripts_run(struct intf *f) {
    char cmd[CMD_LEN] = {0};
    int len           = 0;
    int child_state   = 0;
    char *state_str   = f->state == ONLINE ? "online" : "offline";
    pid_t pid         = fork();

    if (pid < 0) {
        debug("Run scripts fork failed!\n");
        return;
    } else if (pid > 0) {
        /* parent process: monitor until child has finished */
        wait(&child_state);
        return;
    }

    /* child process */
    len = snprintf(
        cmd, sizeof(cmd),
        "export INTERFACE=\"%s\"; export GLOBAL=\"Orbit\"; "
        "for hook in /etc/conifd/%s.d/*; do [ -r \"$hook\" ] && sh $hook; "
        "done",
        f->conf_src, state_str);

    if (len <= 0 || (unsigned int)len >= sizeof(cmd)) { // error or truncated
        debug("Run scripts commands truncated!");
        _exit(EXIT_FAILURE);
    }

    execlp("/bin/sh", "/bin/sh", "-c", cmd, NULL);
    debug("Run scripts exec error!");
    _exit(EXIT_FAILURE);
}

void task_error_check_run(struct intf *f) {
    char cmd[CMD_LEN] = {0};
    int len           = 0;
    int child_state   = 0;
    pid_t pid         = fork();

    if (pid < 0) {
        debug("Run scripts fork failed!\n");
        return;
    } else if (pid > 0) {
        /* parent process: monitor until child has finished */
        wait(&child_state);

        //printf("WIFEXITED = %d\n", WIFEXITED(child_state));
        //printf("exit value:%d\n", WEXITSTATUS(child_state));
        if (WIFEXITED(child_state)) {
            if (WEXITSTATUS(child_state) == SUCCESS)
                f->state = UP;
            else if (WEXITSTATUS(child_state) == FAIL)
                f->state = DOWN;
            else if (WEXITSTATUS(child_state) == RESTART)
                exit(EXIT_SUCCESS);
        } else {
            f->state = DOWN;
        }
        return;
    }

    /* child process */
    len = snprintf(
        cmd, sizeof(cmd),
        "export INTERFACE=\"%s\"; export FAIL=\"%d\"; export SUCCESS=\"%d\"; export RESTART=\"%d\";"
        "/etc/conifd/CONIFD_CHECK.sh",
        f->conf_src, FAIL, SUCCESS, RESTART);

    if (len <= 0 || (unsigned int)len >= sizeof(cmd)) { // error or truncated
        debug("Run scripts commands truncated!");
        _exit(EXIT_FAILURE);
    }

    execlp("/bin/sh", "/bin/sh", "-c", cmd, NULL);
    debug("Run scripts exec error!");
    _exit(EXIT_FAILURE);
}
