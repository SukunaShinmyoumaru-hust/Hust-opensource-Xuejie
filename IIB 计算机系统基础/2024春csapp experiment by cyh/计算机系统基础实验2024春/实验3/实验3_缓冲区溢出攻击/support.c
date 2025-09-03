/*
 * support.c - Helper functions for the buffer bomb autograding service
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

//#include "support.h"
//#include "gencookie.h"
//#include "config.h"
//#include "driverlib.h"


/* Globals defined in bufbomb.c  */
extern char gets_buf[]; /* the exploit string */
extern char *userid;    /* userid of person submitting the string */
extern unsigned cookie; /* cookie generated from userid */
extern int success;     /* indicates success of exploit back to bufbomb */
extern int notify;      /* should the bomb send each exploit string to a server */
extern int autograde;   /* should bomb run in autograde mode and set timeout */

/* Globals private to support.c */
#define NUM_LEVELS 5

static int level_counts[NUM_LEVELS] = {1,1,1,1,5}; /* counts for each level */


/*
 * validate - Do some simple sanity checks and optionally report results 
 * to the grading server. 
 *
 * Note: Some students might try to avoid doing the assignment by
 * constructing an exploit string that simply jumps to the validate()
 * routine with a legal level stored on the stack.  This is a
 * vulnerability in the current autograding scheme that we haven't
 * completely solved. To be thorough, you should manually check that
 * exploit strings submitted by your students don't contain the
 * address of the validate() routine.
 */
void validate(int level)
{
  //  char autoresult[SUBMITR_MAXBUF];
  //  char status_msg[SUBMITR_MAXBUF];
    int status;

    if (level < 0 || level >= NUM_LEVELS) {
		printf("Invalid level.  Results not validated\n");
		return;
    }
    
    /* Let the caller know that the exploit succeeded */
    success = 1;
    
    /* Recall that nitro mode produces 5 exploit strings */
    if (--level_counts[level] > 0) {
		printf("Keep going\n");
    } 
    else {

		printf("NICE JOB!\n");
    }
}

int gencookie(char *s)
{
        if (strlen(s) !=10)  {
          printf("length of userid  must be 10. \n");
          return 0;
     }
     if (s[0] !='U' && s[0] !='u')  {
          printf("student id  satrt with U. \n");
          return 0;
     }
     for(int i=1;i<10;i++) 
         if (s[i]<'0' || s[i]>'9') {
          printf("stuid must be digitals. \n");
          return 0;
     }
     return atoi(s+1);   
}

