#include "../include/stringwork.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
void append_string(char ** str, int * offset, int * sz,	const char * format, ...) {
	/* Initialize variable argument list */
	va_list args;
	va_start(args, format);

	int appended_chars = 0;
	
	int everything_fits;
	do {

		everything_fits = 1;
		/* Make a copy of the variable arguments to ensure we don't screw up if we need to reallocate and call this function again */
		va_list copy;
		va_copy(copy, args);
			
		/* sprintf the destination string, and get how many characters successfully got written */
		appended_chars = vsnprintf((*str) + *offset, *sz, format, copy);
		/* If we've reached the end, we need to allocate more room */
		if(appended_chars + *offset >= *sz) {
			/* We've run out of room, let's increase the size */
			*sz *= 2;
			*str = realloc(*str, *sz);
			everything_fits = 0;	
		}
	} while(!everything_fits) ;
	/* Otherwise we've appended successfully, so move the offset */
	*offset += appended_chars;


	/* Free the variable argument list */
	va_end(args);
}

