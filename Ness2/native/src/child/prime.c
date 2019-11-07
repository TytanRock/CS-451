/**
 * Author: Cory
 * Assignment Number: 2
 * Date of Submission: 11/6/2019
 * Name of this file: prime.c
 * Description of the program:
 *  This program runs a scheduler on a set of prime-finders
 *  in order to show how a priority scheduler works
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define MAX_NUM 1000000000

/**
 * Struct that holds module-specific variables
 */
struct {
	unsigned long int current_num; //!< Current num we're checking
	unsigned long int last_prime; //!< Last prime we found

	volatile unsigned active : 1; //!< Flag that tells us if we're running
	volatile unsigned closed : 1; //!< Flag that tells us to exit
} _module;

/**
 * handle_sigterm
 * Purpose: Close application if sigterm is caught
 * Parameters:
 *  signal - signal that triggered this function
 */
void handle_sigterm(int signal) {
	/* Set flag to close */
	_module.closed = 1;
}
/**
 * handle_sigtstp
 * Purpose: Pause execution when sigtstp is caught
 * Parameters:
 *  signal - signal that triggered this function
 */
void handle_sigtstp(int signal) {
	/* Print the last prime we found so user sees it */
	printf("%lu", _module.last_prime);
	fflush(stdout);
	/* Set active to 0 so we pause */
	_module.active = 0;
}
/**
 * handle_sigcont
 * Purpose: Continue execution when sigcont is caught
 * Parameters:
 *  signal - signal that triggered this function
 */
void handle_sigcont(int signal) {
	/* Resume execution */
	_module.active = 1;
	/* Print current number we're checking */
	printf("%lu", _module.current_num);
	fflush(stdout);
}
/**
 * check_next_num
 * Purpose: Check current number to see if it's prime
 * Returns: 1 if prime, 0 if composite
 */
int check_next_num() {
	/* Look at every number between 2 and num/2 */
	for(int i = 2; i < _module.current_num / 2; ++i) {
		/* If number is divisible by i, it's composite */
		if(_module.current_num % i == 0) return 0;
		/* If we need to stop, we'll stop */
		if(_module.closed) return 0;
	}
	/* No number between 2 and num/2 was divisible, this is prime */
	return 1;
}
/**
 * find_next_prime
 * Purpose: Find the next prime after current_num
 */
void find_next_prime() {
	/* We'll check every number until we find a prime */
	while(!check_next_num()) {
		/* This number isn't good, let's check the next one */
		++_module.current_num;
		/* We need to close */
		if(_module.closed) return;
	}
	/* Found the prime */
	_module.last_prime = _module.current_num;
	/* Let's check the next one, too */
	++_module.current_num;
}
/**
 * initialize
 * Purpose: Initialize everything for module
 */
void initialize() {
	/* Seed random with time */
	srand(time(0));
	/* Make current number from random */
	_module.current_num = rand();
	/* Make sure our start isn't too high */
	if(_module.current_num > MAX_NUM) 
		_module.current_num = MAX_NUM;
	/* Start with module being active */
	_module.active = 1;
	/* Assign all signals to handlers */
	signal(SIGTERM, handle_sigterm);
	signal(SIGTSTP, handle_sigtstp);
	signal(SIGCONT, handle_sigcont);

	/* Print number we're starting off with */
	printf("%lu", _module.current_num);
	fflush(stdout);
}

int main(int argc, char ** argv) {
	/* Initilize module */
	initialize();	

	/* We will be exited by sigterm */
	while(!_module.closed) {
		/* If we're active, find the next prime */
		if(_module.active) {
			find_next_prime();
		/* Otherwise we'll pause execution */
		} else {
			pause();
		}
	}
	
	/* We've exited, print the last prime we found */
	printf("%lu", _module.last_prime);
	fflush(stdout);
	
	/* Return 0 cause we exited cleanly */
	return 0;
}
