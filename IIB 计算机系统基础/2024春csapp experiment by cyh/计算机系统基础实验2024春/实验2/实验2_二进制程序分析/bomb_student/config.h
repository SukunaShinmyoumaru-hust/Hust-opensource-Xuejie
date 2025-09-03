/*
 * config.h - Configuration info for the bomb's automatic notification feature
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * We don't want copies of bombs from all over the world contacting 
 * our server, so restrict bomb execution to one of the machines on 
 * the following NULL-terminated comma-separated list:
 */
char *host_table[128] = {

    /* Put your host names here */
    "greatwhite.ics.cs.cmu.edu",
    "angelshark.ics.cs.cmu.edu",
    "makoshark.ics.cs.cmu.edu",

    0 /* The zero terminates the list */
};


#endif /* __CONFIG_H__ */

