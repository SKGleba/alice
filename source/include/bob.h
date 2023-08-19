#ifndef __BOB_H__
#define __BOB_H__

#include "types.h"

enum ALICE2BOB_COMMANDS {
    BOB_A2B_GET_RPC_STATUS = 0xA20,
    BOB_A2B_SET_RPC_STATUS,
    BOB_A2B_MASK_RPC_STATUS,
    BOB_A2B_REBOOT,
    BOB_A2B_MEMCPY,
    BOB_A2B_MEMSET,
    BOB_A2B_MEMSET32,
    BOB_A2B_READ32,
    BOB_A2B_WRITE32,
    BOB_A2B_EXEC = 0x80000000 // OR it with paddr for bob to exec
};

#define BOB_ACQUIRE_ARM_CMD 0x5E7A21CE
#define BOB_RELINQUISH_ARM_CMD 0xA21CEDED

int bob_sendSimpleCmd(int cmd, uint32_t a1, uint32_t a2, uint32_t a3);

#endif