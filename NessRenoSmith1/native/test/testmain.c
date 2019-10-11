#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <assert.h>
#include "cmockery/cmockery.h"

void compare_test() {
	assert(1);
}

int main() {
	const UnitTest tests[] = {
		unit_test(compare_test),
	};
	return run_tests(tests, "run");
}

