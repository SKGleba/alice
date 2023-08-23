#include <inttypes.h>

#include "include/clib.h"
#include "include/uart.h"
#include "include/debug.h"
#include "include/defs.h"
#include "include/utils.h"
#include "include/main.h"
#include "include/coring.h"
#include "include/rpc.h"
#include "include/bob.h"
#include "include/ex.h"

#include "include/zero.h"

static volatile core_task_s l_zero_rpc_delegate;

int zero_nop(int a, int b, int c, int d) {
    return 1;
}

int zero_enable_rpc(bool block_bob, int delegate_core) {
    if (delegate_core) {
        if (block_bob)
            l_zero_rpc_delegate.task_id = (int)rpc_loop_exclusive | CORE_TASK_TYPE_ISPTR;
        else
            l_zero_rpc_delegate.task_id = (int)rpc_loop | CORE_TASK_TYPE_ISPTR;
        return core_schedule_task(delegate_core, &l_zero_rpc_delegate, false, false);
    }
    if (block_bob)
        rpc_loop_exclusive();
    else
        rpc_loop();
    return 0;
}

void* zero_get_task_by_id(int task_id) {
    switch (task_id) {
    case ZERO_TASKS_NOP:
        return zero_nop;
    case ZERO_TASKS_ENABLE_RPC:
        return zero_enable_rpc;
    default:
        return NULL;
    }
}

void zero_init(void) {
    printf("ready\n");
    g_core_status[0] |= CORE_STATUS_RUNNING;
}

void test(void);

void zero_main(void) {
    test();
    while (1)
        core_task_handler(zero_get_task_by_id);
}

void test(void) {
    printf("test test test\n");
    {
        zero_enable_rpc(true, 0);
    }

    printf("all tests done\n");
}