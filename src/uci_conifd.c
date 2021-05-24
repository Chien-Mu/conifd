#include "common.h"

#include <uci.h>

/* analogous to uci_lookup_option_string from uci.h, returns -1 when not found */
static int uci_lookup_option_int(struct uci_context *uci,
                                 struct uci_section *s,
                                 const char *name) {
    const char *str = uci_lookup_option_string(uci, s, name);
    return str == NULL ? -1 : atoi(str);
}

int load_config(struct intf *conf) {
    struct uci_context *uci = NULL;
    struct uci_package *p   = NULL;
    struct uci_element *e   = NULL;
    int idx                 = 0;
    int val;
    const char *str;

    if ((uci = uci_alloc_context()) == NULL) {
        return EXIT_FAILURE;
    }

    if (uci_load(uci, "conifd", &p)) {
        uci_free_context(uci);
        return EXIT_FAILURE;
    }

    uci_foreach_element(&p->sections, e) {
        struct uci_section *s = uci_to_section(e);
        if (strcmp(s->type, "interface") == 0) {
            if ((str = uci_lookup_option_string(uci, s, "src")) != NULL)
                strncpy(conf[idx].conf_src, str, sizeof(conf[idx].conf_src));
            else
                strncpy(conf[idx].conf_src, "", sizeof(conf[idx].conf_src));

            if ((str = uci_lookup_option_string(uci, s, "dest")) != NULL)
                strncpy(conf[idx].conf_dest, str, sizeof(conf[idx].conf_dest));
            else
                strncpy(conf[idx].conf_dest, "8.8.8.8", sizeof(conf[idx].conf_dest));

            val                     = uci_lookup_option_int(uci, s, "interval");
            conf[idx].conf_interval = val > 0 ? val : 10;

            val                    = uci_lookup_option_int(uci, s, "timeout");
            conf[idx].conf_timeout = val > 0 ? val : 3;

            if ((str = uci_lookup_option_string(uci, s, "before_check")) != NULL) {
                if (!strcmp(str, "y") || !strcmp(str, "n"))
                    strncpy(&conf[idx].conf_before_check, str, 1);
                else
                    strncpy(&conf[idx].conf_before_check, "y", 1);
            } else
                strncpy(&conf[idx].conf_before_check, "y", 1);
        }

        ++idx;
    }

    uci_unload(uci, p);
    uci_free_context(uci);
    return EXIT_SUCCESS;
}

