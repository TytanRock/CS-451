#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

struct {
	unsigned long int current_num;
	unsigned long int last_prime;

	volatile unsigned active : 1;
	volatile unsigned closed : 1;
} _module;

void handle_sigint(int signal) {
	/* Set flag to close */
	_module.closed = 1;
}
void handle_sigtstp(int signal) {
	/* Pause execution for a bit */
	printf("%lu", _module.last_prime);
	fflush(stdout);
	_module.active = 0;
}
void handle_sigcont(int signal) {
	/* Resume execution */
	_module.active = 1;
}

int check_next_num() {
	for(int i = 2; i < _module.current_num; ++i) {
		if(_module.current_num % i == 0) return 0;
		if(_module.closed) return 0;
	}
	return 1;
}

void find_next_prime() {
	while(!check_next_num()) {
		++_module.current_num;
		if(_module.closed) return;
	}
	/* Found the prime */
	_module.last_prime = _module.current_num;
	++_module.current_num;
}

void initialize() {
	_module.current_num = 1234567890;
	_module.active = 1;
	signal(SIGINT, handle_sigint);
	signal(SIGTSTP, handle_sigtstp);
	signal(SIGCONT, handle_sigcont);
}

int main(int argc, char ** argv) {
	initialize();	

	/* We will be exited by sigint */
	while(!_module.closed) {
		if(_module.active) {
			find_next_prime();
		}
		//printf("Prime is: %lu\n", _module.last_prime);
	}

	printf("%lu", _module.last_prime);
	fflush(stdout);

	return 0;
}
