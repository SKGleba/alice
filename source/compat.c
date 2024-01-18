#include "include/compat.h"

#include <hardware/maika.h>
#include <hardware/paddr.h>

#include "include/clib.h"
#include "include/debug.h"
#include "include/defs.h"
#include "include/ex.h"
#include "include/perv.h"
#include "include/types.h"
#include "include/utils.h"

int compat_loadSK(uint32_t addr, bool second) {
    maika_s* maika = (maika_s*)MAIKA_OFFSET;
    printf("performing f00d reset\n");
    if (second) {
        maika->reset_ctrl.f00d_cycle_reset = 1;
        maika->reset_ctrl.f00d_cycle_reset = 0;
        while (maika->reset_ctrl.f00d_cycle_reset) {};
    } else {
        maika->reset_ctrl.f00d_reset = 1;
        maika->reset_ctrl.f00d_reset = 0;
        while (maika->reset_ctrl.f00d_reset) {};
    }
    
    int ret = 0;
    while (ret >= 0) {
        ret = (int)maika->reset_ctrl.crySboot;
    }

    printf("did reset to %X | loading sk from %X\n", ret, addr);

    maika->mailbox.arm2cry[0] = addr | 1;
    ret = 0;
    while (ret != 1) {
        ret = maika->mailbox.cry2arm[0];
        if (ret == 2)
            break;
    }
    maika->mailbox.cry2arm[0] = ret;

    printf("load_secure_kernel ret %X\n", ret);

    return ret;
}