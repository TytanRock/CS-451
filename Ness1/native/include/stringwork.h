/**
 * Author: Cory
 * Assignment Number: 1
 * Date of Submission: ~
 * Name of this file: main.c
 * Description of the program:
 *  This program mimics the output of ps -ely for any specific pid
 *  It uses the /proc directory to find the process information that
 *  the user wants, specified by the 5 CLI parameters, p,s,t,v,c
 */


#ifndef _STRINGWORK_H_
#define _STRINGWORK_H_

/**
 * append_string
 * Purpose: Append characters to a string following the format specified.
 *  This will dynamically increase the size of the destination if needed.
 *
 * Parameters:
 *  str - destination string pointer
 *  offset - offset pointer
 *  sz - current size of string pointer
 *  format - character format
 *  ... - optional arguments
 */
void append_string(char ** str, int * offset, int * sz,	const char * format, ...);

#endif //_STRINGWORK_H_ 
