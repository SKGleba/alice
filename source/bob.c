#include "include/defs.h"
#include "include/clib.h"
#include "include/utils.h"
#include "include/debug.h"
#include "include/rpc.h"

#include <hardware/maika.h>
#include <hardware/paddr.h>

#include "include/bob.h"

int bob_sendSimpleCmd(int cmd, uint32_t a1, uint32_t a2, uint32_t a3) {
    int ret = 0;
    maika_s* maika = (maika_s*)MAIKA_OFFSET;
    maika->mailbox.arm2cry[3] = a3;
    maika->mailbox.arm2cry[2] = a2;
    maika->mailbox.arm2cry[1] = a1;
    maika->mailbox.arm2cry[0] = cmd;
    do {
        ret = maika->mailbox.arm2cry[0];
    } while (ret == cmd);
    ret = maika->mailbox.cry2arm[1];
    maika->mailbox.cry2arm[1] = -1;
    return ret;
}