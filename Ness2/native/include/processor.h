#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "global.h"
#include "fileparser.h"

ERR_CODE start_schedule(process_table *table, int entry_count);
ERR_CODE stop_processor();
ERR_CODE free_processor();

#endif //_PROCESSOR_H_

