/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: person.h
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */

#ifndef __PERSON_H_
#define __PERSON_H_

/* Struct that holds everything a person needs in its thread */
typedef struct {
	struct {
		int floor;
		int wait;
	} *pairs;
	int pair_count;

	int id;
} person_time;

/**
 * start_person_thread
 * Purpose: Start the thread for a person
 * Parameters:
 *  arg - person_time struct with everything the person needs
 * Returns: void* that's always NULL
 */
void *start_person_thread(void *arg);

/**
 * parse_person_line
 * Purpose: Parse string into a set of person_time's
 * Parameters:
 *  str - String to parse
 *  ref - Struct to fill with data
 */
void parse_person_line(char *str, person_time *ref);

#endif //__PERSON_H_
