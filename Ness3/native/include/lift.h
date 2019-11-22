#ifndef __LIFT_H_
#define __LIFT_H_

void initialize_lift();
void *add_floor_stop(unsigned int floor);
void *start_lift_thread(void *arg);
int get_floor_stop();

#endif //__LIFT_H_

