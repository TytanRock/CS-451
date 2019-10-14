#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <assert.h>
#include "cmockery/cmockery.h"
#include "../include/processes.h"

void test_processes_headers() {
	process_info tmpInfo;
	/* Always returns 0 because ID 1 is always valid */
	assert(get_process_info(1, &tmpInfo) == 0);
}

int main() {
	const UnitTest tests[] = {
		unit_test(test_processes_headers),
	};
	return run_tests(tests, "run");
}

