/*
 * support.c - Helper functions for the bomb
 *
 * Copyright (c) 2004-2011, R. Bryant and D. O'Hallaron, All rights reserved.
 * May not be used, modified, or copied without permission.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "support.h"
#include "config.h"
#include "driverlib.h"

/* Globals defined by phases.c */
extern int bomb_id;

#ifdef NOTIFY
extern char userid[];
#endif

/* Global input stream defined by bomb.c */
extern FILE *infile;

/* Global that keeps track of the user's input strings */
char input_strings[MAX_STRINGS][MAX_LINE];
int num_input_strings = 0;

/* Global scratch buffer */
char scratch[MAX_LINE];

/******************************************************
 * Amusing signal handler called when user types ctrl-c
 ******************************************************/

static void sig_handler(int sig)
{
    printf("So you think you can stop the bomb with ctrl-c, do you?\n");
    sleep(3);
    printf("Well...");
    fflush(stdout);
    sleep(1);
    printf("OK. :-)\n");
    exit(16);
}

/************************************************** 
 * Helper routines called by the phases in phases.c
 **************************************************/

/* Invoked by improperly built phases */
void invalid_phase(char *s) 
{
    printf("Invalid phase%s\n", s);
    exit(8);
}

/* Extract numbers from an input string */
 void read_six_numbers(char *input, int *numbers)
{
  int numScanned = sscanf(input, "%d %d %d %d %d %d",
			  numbers, numbers + 1, numbers + 2,
			  numbers + 3, numbers + 4, numbers + 5);
  if (numScanned < 6)
    explode_bomb();
}

/* A more user-friendly version of strlen */
int string_length(char *aString)
{
    int length;
    char *ptr;

    ptr = aString;
    length = 0;

    while (*ptr != 0) {
	ptr++;
	length = length + 1;
    }
    return length;
}

/* A more user-friendly version of strcmp */
int strings_not_equal(char *string1, char *string2)
{
    char *p, *q;

    if (string_length(string1) != string_length(string2))
	return 1;

    p = string1;
    q = string2;

    while (*p != 0) {
	if (*p != *q)
	    return 1;
	p++;
	q++;
    }
    return 0;
}


/***********************************
 * Helper functions called by bomb.c  			 
 ***********************************/

/* 
 * initialize_bomb - initialize the bomb 
 */
void initialize_bomb(void)
{
#ifdef NOTIFY
    int i;
    char hostname[MAXHOSTNAMELEN];
    char status_msg[SUBMITR_MAXBUF];
    int valid_host = 0;
#endif

    /* Just for fun, trap Ctrl-C: */
    signal(SIGINT, sig_handler);
    
#ifdef NOTIFY
    /* Get the host name of the machine */
    if (gethostname(hostname, MAXHOSTNAMELEN) != 0) {
	printf("Initialization error: Running on an illegal host [1]\n");
	exit(8);
    }

    /* Make sure it's in the list of legal machines */
    for (i = 0; host_table[i]; i++) {
	if (strcasecmp(host_table[i], hostname) == 0) {
	    valid_host = 1;
	    break;
	}
    }
    if (!valid_host) {
	printf("Initialization error: Running on an illegal host [2]\n");
	exit(8);
    }

    /* Initialize the submitr package */
    if (init_driver(status_msg) < 0) {
	printf("Initialization error:\n%s\n", status_msg);
	exit(8);
    }
#endif
}

/*
 * Initialize solution version of bomb
 */
void initialize_bomb_solve(void)
{
}

/* Return true if str is a blank line */
int blank_line(char *str)
{
    while (*str)
	if (!isspace(*str++))
	    return 0;
    return 1;
}

/* Read input lines until first non-blank or EOF encountered */
char *skip() 
{
    char *p;

    while (1) {
	p = fgets(input_strings[num_input_strings], MAX_LINE, infile);
	if ((p == NULL) || (!blank_line(p)))
	    return p;
    }
}

/* 
 * Read a line of input from stream "infile". There are a couple
 * of tricky aspects to this. First, we cut the students a little slack
 * by skipping over blank lines. Second, we allow partial solutions
 * to be read from a file before switching over to stdin.
 */
char *read_line(void)
{
    int len;
    char *str;

    /* switch over to stdin if we hit EOF on an input disk file */
    str = skip();
    if (str == NULL) { /* EOF */
	if (infile == stdin) { /* premature EOF on stdin */
	    printf("Error: Premature EOF on stdin\n");
	    exit(8);
	}
	else {
	    /* exit with OK status on EOF if we are grading the bomb */
	    /* this lets us evaluate partial solutions */
	    if (getenv("GRADE_BOMB")) {
		exit(0); 
	    }

	    /* otherwise switch over to stdin */
	    else {
		infile = stdin; 
		str = skip();
		if (str == NULL) { /* premature EOF on stdin */
		    printf("Error: Premature EOF on stdin\n");
		    exit(0);
		}
	    }
	}
    }

    len = strlen(input_strings[num_input_strings]);
    if(len >= MAX_LINE-1) {
	printf("Error: Input line too long\n");
	strcpy(input_strings[num_input_strings++], "***truncated***");
	explode_bomb();
    }

    /* Strip off trailing newline: */
    input_strings[num_input_strings][len-1] = '\0';
    return input_strings[num_input_strings++];
}

#ifdef NOTIFY
void send_msg(int defused)
{
    char autoresult[SUBMITR_MAXBUF];
    char status_msg[SUBMITR_MAXBUF];

    int status;

    if (strlen(input_strings[num_input_strings - 1]) + 100 > SUBMITR_MAXBUF) {
	printf("ERROR: Input string is too large.");
	exit(8);
    }
    
    sprintf(autoresult, "%d:%s:%d:%s", 
	    bomb_id,
	    defused ? "defused" : "exploded",  
	    num_input_strings, 
	    input_strings[num_input_strings - 1]);

    status = driver_post(userid, autoresult, 0, status_msg);
    if (status < 0) {
	printf("%s\n", status_msg);
	exit(0);
    }
}
#endif

void explode_bomb(void)
{
    printf("\nBOOM!!!\n");
    printf("The bomb has blown up.\n");
#ifdef NOTIFY
    send_msg(0);
    printf("Your instructor has been notified.\n");
#endif

    exit(8);
}

void phase_defused(void)
{
    char passphrase[MAX_LINE];
    int tmp1, tmp2, numScanned;

#ifdef NOTIFY
    send_msg(1);
#endif

    if (num_input_strings == 6) { /* user has just defused phase 6 */
	numScanned = sscanf(input_strings[3], "%d %d %s", 
			    &tmp1, &tmp2, passphrase);
	if ((numScanned == 3) && 
	    (strings_not_equal(passphrase, SECRET_PHRASE) == 0)) {
	    printf("Curses, you've found the secret phase!\n");
	    printf("But finding it and solving it are quite different...\n");
	    secret_phase();
	}
	printf ("Congratulations! You've defused the bomb!\n");
#ifdef NOTIFY
	printf("Your instructor has been notified and will verify your solution.\n");
#endif	
    }
}

