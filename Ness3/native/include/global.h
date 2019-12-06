/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: global.h
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */

#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <semaphore.h>

/* Struct that holds data that's used everywhere */
struct {
	unsigned int max_floor;
	unsigned int total_people;
	unsigned int max_wander_time;

	unsigned int uninitialized_people;
	sem_t start_semaphore;
} _global;

#endif //__GLOBAL_H_

