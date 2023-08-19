#include "include/types.h"

#include "include/defs.h"
#include "include/uart.h"
#include "include/maika.h"
#include "include/debug.h"
#include "include/utils.h"
#include "include/clib.h"
#include "include/gpio.h"
#include "include/paddr.h"
#include "include/compat.h"

#include "include/glitch.h"

volatile bool l_glitch_success;

void glitch_watchdog(uint32_t addr) {
    vp addr = 0;
    l_glitch_success = false;
    while (1) {
        if (vp addr) {
            l_glitch_success = true;
            for (int i = 0; i < 0x100; i++)
                print("ping pong ding dong ");
            break;
        }
    }
}

void glitch_loopSKR(uint32_t sk_addr, bool second) {
    while (!l_glitch_success) {
        print("GLSTART\n");
        compat_loadSK(sk_addr, second);
        print("GLSTOP\n");
    }
}