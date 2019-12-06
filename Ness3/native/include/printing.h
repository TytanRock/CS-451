/**
 * Author: Cory Ness
 * Assignment Number: 3
 * Date of Submission: 12/5/2019
 * Name of this file: printing.h
 * Description of program:
 *  This program runs an elevator and n number of people
 *  The people get on and off the elevator and move to specified floors
 *  They wait for some number of seconds at each floor, and eventually leave the building
 * This project uses 1 coupon
 */

#ifndef __PRINTING_H_
#define __PRINTING_H_

/**
 * initialize_printing
 * Purpose: Initialize this module
 */
void initialize_printing();

/**
 * timed_print
 * Purpose: printf with a prepend-ed timestamp
 * Parameters
 *  format - String format to print
 *  ... - Parameters based on string format
 */
void timed_print(const char *format, ...);

#endif //__PRINTING_H_

