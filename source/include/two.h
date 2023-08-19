#ifndef __TWO_H__
#define __TWO_H__

#include "types.h"

enum TWO_TASKS {
    TWO_TASKS_NOP = 0,
};

void two_main(void);
void two_init(void);
void* two_get_task_by_id(int task_id);
int two_nop(int a, int b, int c, int d);

#endif