#include "include/defs.h"
#include "include/clib.h"
#include "include/utils.h"
#include "include/debug.h"
#include "include/rpc.h"

#include <hardware/maika.h>
#include <hardware/paddr.h>

#include "include/bob.h"

uint8_t volatile x_bob_b2a_ishmem[BOB_ABBA_ISHBUF_SIZE] __attribute__((aligned(4)));
struct _bob_a2b_cmd_s volatile l_bob_a2b_cmds[BOB_MAILR_COUNT] __attribute__((aligned(4))); // for both normal & async

int bob_getCmdReply(uint32_t cmd, enum BOB_MAILREGS mbr, uint32_t *retx) {
    maika_s* maika = (maika_s*)MAIKA_OFFSET;
    uint32_t cmdep = 0x80000000 | (uint32_t)&cfg_phys_load_off | (uint32_t)&l_bob_a2b_cmds[mbr];
    int ret = 0;
    do {
        ret = maika->mailbox.cry2arm[mbr];
    } while (ret != cmdep);
    maika->mailbox.cry2arm[mbr] = -1;
    if (retx) {
        for (int i = 0; i < 3; i++) {
            retx[i] = l_bob_a2b_cmds[mbr].arg[i];
        }
    }
    ret = l_bob_a2b_cmds[mbr].ret;
    l_bob_a2b_cmds[mbr].cmd = -1; // reset
    return ret;
}

int bob_sendCmd(uint32_t cmd, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t *retx, enum BOB_MAILREGS mbr, bool waitPre, bool waitPost) {
    int waited = 0;
    if (l_bob_a2b_cmds[mbr].cmd != -1) {
        if (!waitPre)
            return -1; // busy
        do {
            waited++;
        } while (l_bob_a2b_cmds[mbr].cmd != -1);
    }
    l_bob_a2b_cmds[mbr].cmd = (int)cmd;
    l_bob_a2b_cmds[mbr].arg[0] = a1;
    l_bob_a2b_cmds[mbr].arg[1] = a2;
    l_bob_a2b_cmds[mbr].arg[2] = a3;

    maika_s* maika = (maika_s*)MAIKA_OFFSET;
    uint32_t cmdep = 0x80000000 | (uint32_t)&cfg_phys_load_off | (uint32_t)&l_bob_a2b_cmds[mbr];
    maika->mailbox.cry2arm[mbr] = -1; // just to be sure
    maika->mailbox.arm2cry[mbr] = cmdep;
    do {
        ;
    } while (maika->mailbox.arm2cry[mbr] == cmdep);
    if (!waitPost)
        return waited & 0x7fffffff;
    return bob_getCmdReply(cmd, mbr, retx);
}

int bob_init(void) {
    for (int i = 0; i < BOB_MAILR_COUNT; i++) {
        memset((void*)&l_bob_a2b_cmds[i], 0xff, sizeof(struct _bob_a2b_cmd_s));
    }
    memset((void*)x_bob_b2a_ishmem, 0, sizeof(x_bob_b2a_ishmem));
    return bob_sendSimpleCmd(BOB_A2B_SET_B2A_SHBUF, ((uint32_t)x_bob_b2a_ishmem | (uint32_t)&cfg_phys_load_off), sizeof(x_bob_b2a_ishmem), 0);
}