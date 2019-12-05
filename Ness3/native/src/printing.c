/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: printing.c
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */

#include "../include/printing.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <semaphore.h>

/* Struct to keep track of every variable used in this module */
static struct {
	int current_time; //!< Current time of program
	sem_t stdout_semaphore; //!< Semaphore used to keep printing safe
} _module;

/**
 * timer_handler
 * Purpose: Gets called every timer interrupt
 * Parameters:
 *  signal - signal that called this function
 */
void timer_handler(int signal) {
	/* This does not need a semaphore because this is the only thing that writes current_time */
	_module.current_time ++; //!< Increment current_time
}

/**
 * initialize_printing
 * Purpose: Initialize this module
 */
void initialize_printing() {
	/* Initialize module variables */
	_module.current_time = 0;

	/* Configure timer interrupt for 1 second */
	signal(SIGALRM, timer_handler);
	struct itimerval timer;
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &timer, NULL);

	/* Initialize semaphore */
	sem_init(&_module.stdout_semaphore, 0, 1);
}

/**
 * timed_print
 * Purpose: printf with a prepend-ed timestamp
 * Parameters
 *  format - String format to print
 *  ... - Parameters based on string format
 */
void timed_print(const char *format, ...) {
	/* Lock semaphore to make sure we don't write on top of each other */
	sem_wait(&_module.stdout_semaphore);
	/* Get extra parameters passed down */
	va_list args;
	va_start(args, format);

	/* Printf the current time */
	printf("%2d: ", _module.current_time);
	/* Printf the user string */
	vprintf(format, args);
	
	/* Close the extra parameters */
	va_end(args);
	/* Release lock so others can print */
	sem_post(&_module.stdout_semaphore);
}
