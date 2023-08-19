#ifndef __ONE_H__
#define __ONE_H__

#include "types.h"

enum ONE_TASKS {
    ONE_TASKS_NOP = 0,
};

void one_main(void);
void one_init(void);
void* one_get_task_by_id(int task_id);
int one_nop(int a, int b, int c, int d);

#endif