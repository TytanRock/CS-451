#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include "cmockery/cmockery.h"

typedef struct _childs {
	int fd[2];
	int in[2];
	int pid;
	int OK;
} childs;

childs create_fork() {
	childs ret;
	if(pipe(ret.fd) < 0 || pipe(ret.in) < 0) {
		ret.OK = 0;
		return ret;
	}
	ret.OK = 1;
	ret.pid = fork();
	if(ret.pid == 0) {
		close(ret.fd[0]);
		close(ret.in[1]);
		dup2(ret.fd[1], STDOUT_FILENO);
		dup2(ret.fd[1], STDERR_FILENO);
		dup2(ret.in[0], STDIN_FILENO);
		close(ret.fd[1]);
	} else {
		close(ret.fd[1]);
		close(ret.in[0]);
	}
	return ret;
}

void test_longfile() {
	childs ch = create_fork();
	assert(ch.OK);

	if(ch.pid == 0) {
		char * args[] = {"./gcov/elevator", "-p", "5", NULL };
		FILE *test_file = fopen("longrun.txt", "r");
		dup2(fileno(test_file), STDIN_FILENO);
		fclose(test_file);
		execv(args[0], args);
		exit(1);
	}
	FILE *test_file = fopen("longrun.txt", "r");
	char c;
	while ((c = getc(test_file)) != EOF) {
		write(ch.in[1], &c, 1);
	}
	int ret;
	waitpid(ch.pid, &ret, 0);
	ret = WEXITSTATUS(ret);
	assert(ret == 0);

	close(ch.fd[0]);
}


void test_file() {
	childs ch = create_fork();
	assert(ch.OK);

	if(ch.pid == 0) {
		char * args[] = {"./gcov/elevator", "-p", "2", NULL };
		execv(args[0], args);
		exit(1);
	}
	FILE *test_file = fopen("test.txt", "r");
	char c;
	while ((c = getc(test_file)) != EOF) {
		write(ch.in[1], &c, 1);
	}

	int ret;
	waitpid(ch.pid, &ret, 0);
	ret = WEXITSTATUS(ret);
	assert(ret == 0);

	close(ch.fd[0]);
}

int main(int argc, char **argv) {
	const UnitTest tests[] = {
		unit_test(test_file),
		unit_test(test_longfile),
	};
	return run_tests(tests, "run");
}

