#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_DEST_LEN 256 // for ipv4 and hostname
#define MAX_SRC_LEN 256  // interface

#define RESTART 2
#define FAIL 1
#define SUCCESS 0

#ifdef DEBUG
#define debug(fmt, args...)                                              \
    do {                                                                 \
        FILE *fp = fopen("/dev/console", "w");                           \
        if (fp) {                                                        \
            fprintf(fp, "%s(%d): " fmt, __FUNCTION__, __LINE__, ##args); \
            fclose(fp);                                                  \
        }                                                                \
    } while (0)
#else
#define debug(fmt, args...)
#endif

enum online_state {
    UNKNOWN,

    /* CONIFD_CHECK.sh Return fail. 
     * Will not perform ping operation (will not call the scripts in online.d and offline.d).
	 * */
    DOWN,

    /* CONIFD_CHECK.sh Return success.'
	 * Will execute the ping operation and trigger the script in the online and offline folders.
	 * */
    UP,

    /* ping fail */
    OFFLINE,

    /* ping success */
    ONLINE
};

struct intf {
    char conf_src[MAX_SRC_LEN];
    char conf_dest[MAX_DEST_LEN];
    unsigned int conf_interval; //sec
    unsigned int conf_timeout; // sec
    char conf_before_check; // y or n

    enum online_state state;
};

int test();
