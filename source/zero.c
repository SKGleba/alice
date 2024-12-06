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

int zero_enable_rpc(bool block_bob, int delegate_core, bool direct) {
    if (delegate_core) {
        if (direct)
            l_zero_rpc_delegate.task_id = (int)rpc_loop | CORE_TASK_TYPE_ISPTR;
        else {
            l_zero_rpc_delegate.task_id = (int)rpc_loop_bobcompat | CORE_TASK_TYPE_ISPTR;
            l_zero_rpc_delegate.args[0] = block_bob;
        }
        return core_schedule_task(delegate_core, &l_zero_rpc_delegate, false, false);
    }
    if (direct)
        rpc_loop();
    else
        rpc_loop_bobcompat(block_bob);
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
        zero_enable_rpc(true, 0, false);
    }

    printf("all tests done\n");
}