#ifndef __BOB_H__
#define __BOB_H__

#include "types.h"
#include <hardware/paddr.h>

#define BOB_ABBA_ISHBUF_SIZE (0x40 * 4) // 64 bytes * 4 cores

struct _bob_a2b_cmd_s {
    union {
        int cmd;
        int ret;
    };
    uint32_t arg[3];
};

enum ALICE2BOB_COMMANDS {
    BOB_A2B_GET_RPC_STATUS,
    BOB_A2B_SET_RPC_STATUS,
    BOB_A2B_MASK_RPC_STATUS,
    BOB_A2B_REBOOT,
    BOB_A2B_MEMCPY,
    BOB_A2B_MEMSET,
    BOB_A2B_MEMSET32,
    BOB_A2B_READ32,
    BOB_A2B_WRITE32,
    BOB_A2B_STOP_RELOAD_ALICE,
    BOB_A2B_INIT_STORAGE,
    BOB_A2B_READ_SD,
    BOB_A2B_WRITE_SD,
    BOB_A2B_READ_EMMC,
    BOB_A2B_WRITE_EMMC,
    BOB_A2B_EXPORT_SDIF_CTX,
    BOB_A2B_IMPORT_SDIF_CTX,
    BOB_A2B_SET_B2A_SHBUF,
    
    BOB_A2B__MAX,
    BOB_A2B_EXEC = 0x80000000 // OR it with paddr for bob to exec
};

enum BOB_MAILREGS {
    BOB_MAILR_BOB = 0,
    BOB_MAILR_SM,
    BOB_MAILR_COUNT = 4
};

int bob_getCmdReply(uint32_t cmd, enum BOB_MAILREGS mbr, uint32_t *retx);
int bob_sendCmd(uint32_t cmd, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t *retx, enum BOB_MAILREGS mbr, bool waitPre, bool waitPost);
int bob_init(void);

#define bob_sendSimpleCmd(cmd, a1, a2, a3) bob_sendCmd((uint32_t)cmd, a1, a2, a3, NULL, BOB_MAILR_BOB, true, true)

#endif