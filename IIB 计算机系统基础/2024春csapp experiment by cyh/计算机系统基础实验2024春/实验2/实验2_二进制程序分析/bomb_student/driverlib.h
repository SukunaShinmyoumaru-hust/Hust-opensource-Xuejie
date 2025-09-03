/*********************************************************************** 
 *
 * driverlib.h - Include file for the driverlib package
 *
 * Copyright (c) 2004-2011, D. O'Hallaron, All rights reserved.  May not be
 * used, modified, or copied without permission.
 *
 * $Id: driverlib.h,v 1.6 2005/11/21 23:03:59 autolab Exp $
 ***********************************************************************/

#ifndef __DRIVERLIB_H__
#define __DRIVERLIB_H__

#define SUBMITR_MAXBUF 8192
#define COURSE_NAME "csapp"
#define AUTOGRADE_TIMEOUT 0

/* 
 * Persistent state for the robust I/O (Rio) package 
 */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

/* 
 * Package interface
 */

/* init_timeout - Start a timout timer */
void init_timeout(int timeout);

/* init_driver - Initialize the driverlib package */
int init_driver(char *status_msg);

/* driver_post - Transmit an autoresult string to server */
int driver_post(char *userid,      /* Userid */
		char *result,      /* Result string to submit */
		int autograded,    /* True if called by an autograder */
		char *status_msg); /* Return status msg */

#endif /* __DRIVERLIB_H__ */


