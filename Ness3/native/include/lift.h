/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: lift.h
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */

#ifndef __LIFT_H_
#define __LIFT_H_

/**
 * initialize_lift
 * Purpose: Initialize lift module
 */
void initialize_lift();

/**
 * add_floor_stop
 * Purpose: "Push the button" so elevator stops at a floor
 * Parameters:
 *  floor - Floor for elevator to stop at
 */
void add_floor_stop(unsigned int floor);

/**
 * start_lift_thread
 * Purpose: Run the elevator thread
 * Parameters:
 *  arg - Pointer to struct (unused)
 * Returns: void pointer to NULL
 */
void *start_lift_thread(void *arg);

/**
 * get_floor_stop
 * Purpose: Get current floor for elevator
 * Returns: Floor elevator is at
 */
int get_floor_stop();

#endif //__LIFT_H_

