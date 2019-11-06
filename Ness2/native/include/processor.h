#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "global.h"
#include "fileparser.h"

/**
 * start_schedule
 * Purpose: initialize module for future
 * Parameters:
 *  table - Array of tables of processes to run
 *  entry_count - Number of tables in array
 * Returns: Error code based on startup state
 */
ERR_CODE start_schedule(process_table *table, int entry_count);
/**
 * run_schedule
 * Purpose: check if we've triggered our 1 second timer,
 *  if we have, let's run our scheduling algorithm,
 *  otherwise, let's immediately return
 * Returns: Error code based on success of function
 */
ERR_CODE run_schedule();
/**
 * stop_processor
 * Purpose: Stop all children processes
 * Completely side-effectful
 * Returns: Error code based on success of function
 */
ERR_CODE stop_processor();
/**
 * free_processor
 * Purpose: Clean up allocated memory in module
 * Completely side-effectful
 * Returns: Erorr code based on success of function
 */
ERR_CODE free_processor();

#endif //_PROCESSOR_H_

