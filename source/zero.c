#include <inttypes.h>

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

void test(void);

int zero_nop(int a, int b, int c, int d) {
    return 1;
}

void* zero_get_task_by_id(int task_id) {
    switch (task_id) {
    case ZERO_TASKS_NOP:
        return zero_nop;
    default:
        return NULL;
    }
}

void zero_init(void) {
    printf("ready\n");
    g_core_status[1] |= CORE_STATUS_RUNNING;
}

void zero_main(void) {
    test();
    while (1)
        core_task_handler(zero_get_task_by_id);
}

volatile bool l_completed_acquire; // bss clear on reset will set to 0x0 / false

void test(void) {
    printf("test test test\n");
    {
        int ret = 0;

        if (!l_completed_acquire) {
            printf("acquiring bob arm interface..\n");
            bob_sendSimpleCmd(BOB_ACQUIRE_ARM_CMD, BOB_ACQUIRE_ARM_CMD, BOB_ACQUIRE_ARM_CMD, BOB_ACQUIRE_ARM_CMD);
            printf("acquiring jig rpc..\n");
            bob_sendSimpleCmd(BOB_A2B_MASK_RPC_STATUS, RPC_STATUS_REQUEST_BLOCK, true, 0);
            do {
                delay(0x6000);
                ret = bob_sendSimpleCmd(BOB_A2B_GET_RPC_STATUS, 0, 0, 0);
            } while (!(ret & RPC_STATUS_BLOCKED));

            printf("acquired jig rpc\n");
            l_completed_acquire = true;
        } else
            printf("already acquired jig rpc\n");

        rpc_loop();

        printf("rpc loop exited, giving rpc back to bob\n");
        bob_sendSimpleCmd(BOB_A2B_MASK_RPC_STATUS, RPC_STATUS_REQUEST_BLOCK, false, 0);
        delay(0x6000);
        bob_sendSimpleCmd(BOB_RELINQUISH_ARM_CMD, BOB_RELINQUISH_ARM_CMD, BOB_RELINQUISH_ARM_CMD, BOB_RELINQUISH_ARM_CMD);

        printf("infiniloop\n");

        /*
            since we are the main "task giver", reset should give us a clean bss slate imo
            one-time flags such as device inits should be set in .data
            this is until we add proper interrupt handling
        */
        g_bss_cleared = false;

        while (1)
            wfe();
    }

    printf("all tests done\n");
}