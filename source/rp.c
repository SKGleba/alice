#include "include/defs.h"
#include "include/clib.h"
#include "include/utils.h"
#include "include/debug.h"
#include "include/jig.h"
#include "include/compat.h"
#include "include/coring.h"
#include "include/glitch.h"
#include "include/bob.h"

#include "include/rpc.h"

int g_rpc_status;

static volatile core_task_s l_rpc_glitch_watchdog_task;
static volatile core_task_s l_rpc_sk_reset_loop_task;
volatile bool l_completed_acquire; // bss clear on reset will set to 0x0 / false

void rpc_loop(void) {
    uint8_t xsize = 0; // rpc cmd extra return sz
    uint8_t chash = 0; // calculated cmd checksum
    uint32_t cret = 0; // rpc cmd exit return val
    rpc_buf_s rpc_buf; // full rpc cmd & data buf
    bool push_reply = false; // push reply to jig
    uint32_t delay_cval = RPC_READ_DELAY; // recv
    uint32_t delay_rval = RPC_WRITE_DELAY; //send
    uint32_t(*ccode)() = NULL;
    if (g_rpc_status < 0) {
        printf("RPC mode disabled, status: %X\n", g_rpc_status);
        return;
    }
    printf("entering RPC mode, delay %X\n", delay_cval);
    g_rpc_status |= RPC_STATUS_RUNNING;
    while (true) {
        delay(delay_cval);

        if (g_rpc_status & RPC_STATUS_REQUEST_BLOCK) {
            g_rpc_status |= RPC_STATUS_BLOCKED;
            while (g_rpc_status & RPC_STATUS_REQUEST_BLOCK) {
                delay(RPC_BLOCKED_DELAY);
            }
            g_rpc_status &= ~RPC_STATUS_BLOCKED;
        }

        memset(&rpc_buf, 0, sizeof(rpc_buf_s));
        jig_read_shared_buffer((uint8_t*)&rpc_buf.cmd, 0, sizeof(rpc_cmd_s));

        if (rpc_buf.cmd.magic != RPC_MAGIC || (rpc_buf.cmd.cmd_id & RPC_FLAG_REPLY))
            continue;
        chash = 0;
        for (uint8_t i = 3; i < sizeof(rpc_cmd_s); i++) // start from after hash
            chash += *(uint8_t*)((uint8_t*)&rpc_buf.cmd + i);
        if (chash != rpc_buf.cmd.hash)
            continue;

        if (rpc_buf.cmd.cmd_id & RPC_FLAG_EXTRA)
            jig_read_shared_buffer(rpc_buf.extra_data, sizeof(rpc_cmd_s), sizeof(rpc_buf.extra_data));

        printf("RPC CMD %X\n", rpc_buf.cmd.cmd_id);

        g_rpc_status |= RPC_STATUS_INCMD;

        cret = -1;
        xsize = 0;
        switch (rpc_buf.cmd.cmd_id) {
        case RPC_CMD_NOP:
            cret = get_build_timestamp();
            break;
        case RPC_CMD_READ32:
            cret = vp rpc_buf.cmd.args[0];
            break;
        case RPC_CMD_WRITE32:
            vp(rpc_buf.cmd.args[0]) = rpc_buf.cmd.args[1];
            cret = 0;
            break;
        case RPC_CMD_MEMSET:
            cret = (uint32_t)memset((void*)rpc_buf.cmd.args[0], rpc_buf.cmd.args[1] & 0xFF, rpc_buf.cmd.args[2]);
            break;
        case RPC_CMD_MEMCPY:
            cret = (uint32_t)memcpy((void*)rpc_buf.cmd.args[0], (void*)rpc_buf.cmd.args[1], rpc_buf.cmd.args[2]);
            break;
        case RPC_CMD_SET_DELAY:
            cret = delay_cval;
            delay_cval = rpc_buf.cmd.args[0];
            delay_rval = rpc_buf.cmd.args[1];
            break;
        case RPC_CMD_STOP_RPC:
            cret = 0;
            break;
        case RPC_CMD_SET_PUSH:
            cret = push_reply;
            push_reply = (int)rpc_buf.cmd.args[0];
            break;
        case RPC_CMD_HEXDUMP:
            hexdump(rpc_buf.cmd.args[0], rpc_buf.cmd.args[1], rpc_buf.cmd.args[2]);
            cret = 0;
            break;
        case RPC_CMD_MEMSET32:
            cret = (uint32_t)memset32((void*)rpc_buf.cmd.args[0], rpc_buf.cmd.args[1], rpc_buf.cmd.args[2]);
            break;
        case RPC_CMD_LOAD_SK:
            cret = compat_loadSK(rpc_buf.cmd.args[0], rpc_buf.cmd.args[1]);
            break;
        case RPC_CMD_ENABLE_GLITCH_WATCHDOG:
            l_rpc_glitch_watchdog_task.args[0] = rpc_buf.cmd.args[1];
            l_rpc_glitch_watchdog_task.task_id = (int)glitch_watchdog | CORE_TASK_TYPE_ISPTR;
            cret = core_schedule_task(rpc_buf.cmd.args[0], (core_task_s*)&l_rpc_glitch_watchdog_task, false, false);
            break;
        case RPC_CMD_SK_RESET_LOOP:
            l_rpc_sk_reset_loop_task.args[0] = rpc_buf.cmd.args[1];
            l_rpc_sk_reset_loop_task.args[1] = rpc_buf.cmd.args[2];
            l_rpc_sk_reset_loop_task.task_id = (int)glitch_loopSKR | CORE_TASK_TYPE_ISPTR;
            cret = core_schedule_task(rpc_buf.cmd.args[0], (core_task_s*)&l_rpc_sk_reset_loop_task, false, false);
            break;
        case RPC_CMD_BOB_READ32:
            cret = bob_sendSimpleCmd(BOB_A2B_READ32, rpc_buf.cmd.args[0], rpc_buf.cmd.args[1], rpc_buf.cmd.args[2]);
            break;
        case RPC_CMD_BOB_WRITE32:
            cret = bob_sendSimpleCmd(BOB_A2B_WRITE32, rpc_buf.cmd.args[0], rpc_buf.cmd.args[1], rpc_buf.cmd.args[2]);
            break;

        case RPC_CMD_COPYTO:
            cret = (uint32_t)memcpy((void*)rpc_buf.cmd.args[0], rpc_buf.extra_data, rpc_buf.cmd.args[1]);
            break;
        case RPC_CMD_COPYFROM:
            cret = (uint32_t)memcpy(rpc_buf.extra_data, (void*)rpc_buf.cmd.args[0], rpc_buf.cmd.args[1]);
            xsize = rpc_buf.cmd.args[1];
            break;
        case RPC_CMD_EXEC:
            ccode = (void*)rpc_buf.cmd.args[0];
            printf("RPC EXEC %X\n", ccode);
            cret = ccode(rpc_buf.cmd.args[1], rpc_buf.cmd.args[2], (uint32_t)rpc_buf.extra_data);
            xsize = sizeof(rpc_buf.extra_data);
            break;
        case RPC_CMD_EXEC_EXTENDED:
            ccode = (void*)rpc_buf.cmd.args[0];
            printf("RPC EXECE %X\n", ccode);
            cret = ccode(rpc_buf.cmd.args[1], rpc_buf.cmd.args[2], *(uint32_t*)((uint32_t*)&rpc_buf.extra_data), *(uint32_t*)((uint32_t*)&rpc_buf.extra_data + 1), *(uint32_t*)((uint32_t*)&rpc_buf.extra_data + 2), *(uint32_t*)((uint32_t*)&rpc_buf.extra_data + 3), *(uint32_t*)((uint32_t*)&rpc_buf.extra_data + 4), *(uint32_t*)((uint32_t*)&rpc_buf.extra_data + 5));
            break;
        default:
            break;
        }

        g_rpc_status &= ~RPC_STATUS_INCMD;

        delay(delay_rval);

        printf("RPC RET %X\n", cret);
        
        rpc_buf.cmd.args[0] = cret;
        rpc_buf.cmd.cmd_id |= RPC_FLAG_REPLY;
        rpc_buf.cmd.hash = 0;
        for (uint8_t i = 3; i < sizeof(rpc_cmd_s); i++) // start from after hash
            rpc_buf.cmd.hash += *(uint8_t*)((uint8_t*)&rpc_buf.cmd + i);
        
        if (push_reply)
            jig_update_shared_buffer((uint8_t*)&rpc_buf, 0, sizeof(rpc_cmd_s) + xsize, true);
        else {
            jig_update_shared_buffer((uint8_t*)&rpc_buf + 4, 4, sizeof(rpc_cmd_s) + xsize - 4, false);
            jig_update_shared_buffer((uint8_t*)&rpc_buf, 0, 4, false);
        }

        if (rpc_buf.cmd.cmd_id == (RPC_CMD_STOP_RPC | RPC_FLAG_REPLY))
            break;

        if (g_rpc_status & RPC_STATUS_REQUEST_STOP)
            break;
    }

    g_rpc_status &= 0xFFFF0000; // clear status, keep requests

    printf("exiting RPC mode\n");
}

void rpc_loop_exclusive(void) {
    if (!l_completed_acquire) {
        printf("acquiring bob arm interface..\n");
        bob_sendSimpleCmd(BOB_ACQUIRE_ARM_CMD, BOB_ACQUIRE_ARM_CMD, BOB_ACQUIRE_ARM_CMD, BOB_ACQUIRE_ARM_CMD);
        printf("acquiring jig rpc..\n");
        bob_sendSimpleCmd(BOB_A2B_MASK_RPC_STATUS, RPC_STATUS_REQUEST_BLOCK, true, 0);
        do {
            delay(0x6000);
        } while (!(bob_sendSimpleCmd(BOB_A2B_GET_RPC_STATUS, 0, 0, 0) & RPC_STATUS_BLOCKED));

        printf("acquired jig rpc\n");
        l_completed_acquire = true;
    } else
        printf("already acquired jig rpc\n");

    rpc_loop();

    printf("rpc loop exited, giving rpc back to bob\n");
    bob_sendSimpleCmd(BOB_A2B_MASK_RPC_STATUS, RPC_STATUS_REQUEST_BLOCK, false, 0);
    delay(0x6000);
    bob_sendSimpleCmd(BOB_RELINQUISH_ARM_CMD, BOB_RELINQUISH_ARM_CMD, BOB_RELINQUISH_ARM_CMD, BOB_RELINQUISH_ARM_CMD);

    l_completed_acquire = false;
}