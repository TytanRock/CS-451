#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "cmockery/cmockery.h"
#include "../include/processes.h"

void test_processes_headers() {
	process_info tmpInfo;
	/* Always returns 0 because ID 1 is always valid */
	assert(get_process_info(1, &tmpInfo) == 0);
}

void test_run_process() {
	/* Call the actual binary */
	assert(system("./5ps_test -p 1"));
}

int main() {
	const UnitTest tests[] = {
		unit_test(test_processes_headers),
		unit_test(test_run_process),
	};
	return run_tests(tests, "run");
}

