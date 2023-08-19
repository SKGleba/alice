#ifndef __THREE_H__
#define __THREE_H__

#include "types.h"

enum THREE_TASKS {
    THREE_TASKS_NOP = 0,
};

void three_main(void);
void three_init(void);
void* three_get_task_by_id(int task_id);
int three_nop(int a, int b, int c, int d);

#endif