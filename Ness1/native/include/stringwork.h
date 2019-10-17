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
