#ifndef __ZERO_H__
#define __ZERO_H__

#include "types.h"

enum ZERO_TASKS {
    ZERO_TASKS_NOP = 0,
    ZERO_TASKS_ENABLE_RPC = 1
};

void zero_main(void);
void zero_init(void);
void* zero_get_task_by_id(int task_id);
int zero_nop(int a, int b, int c, int d);
int zero_enable_rpc(bool block_bob, int delegate_core);

#endif