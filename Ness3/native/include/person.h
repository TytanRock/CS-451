#ifndef __PERSON_H_
#define __PERSON_H_

typedef struct {
	struct {
		int floor;
		int wait;
	} *pairs;
	int pair_count;

	int id;
} person_time;

void *start_person_thread(void *arg);
void parse_person_line(char *str, person_time *ref);

#endif //__PERSON_H_
