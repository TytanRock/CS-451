#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "cmockery/cmockery.h"
#include "../include/processes.h"

/* Extern'd function to test and ensure works */
extern void append_string(char ** str, int * offset, int * sz, char * format, ...);

void test_processes_headers() {
	process_info tmpInfo;
	/* Always returns 0 because ID 1 is always valid */
	assert_int_equal(get_process_info(1, &tmpInfo), 0);
}

void test_run_process() {
	/* Call the actual binary */
	assert_int_equal(system("./bin/5ps -p 1"), 0);
}

void test_append_works() {
	/* Initialize string */
	char ** str;
	char * tmp;
	str = &tmp;

	int sz = 1;
	int offset = 0;
	*str = malloc(sz);
	
	append_string(str, &offset, &sz, "This is a test string %s!\n%d is %010d\n", "to test the function", 10, 10);

	assert_int_not_equal(sz, 1);
	assert_int_not_equal(offset, 0);
	assert_string_equal(*str, "This is a test string to test the function!\n10 is 0000000010\n");
}

void test_invalid_pid() {
	assert_int_not_equal(system("./bin/5ps -p 0"), 0);
}

void test_all_parameters() {
	assert_int_equal(system("./bin/5ps -stvcp 1"), 0);
}

int main() {
	const UnitTest tests[] = {
		unit_test(test_processes_headers),
		unit_test(test_run_process),
		unit_test(test_append_works),
		unit_test(test_invalid_pid),
		unit_test(test_all_parameters),
	};
	return run_tests(tests, "run");
}

