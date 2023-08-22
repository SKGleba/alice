#include <inttypes.h>
#include "include/uart.h"
#include "include/debug.h"
#include "include/compile_time.h"
#include "include/defs.h"
#include "include/utils.h"
#include "include/ex.h"
#include "include/coring.h"
#include "include/rpc.h"
#include "include/bob.h"
#include "include/perv.h"

#include "include/zero.h"
#include "include/one.h"
#include "include/two.h"
#include "include/three.h"

#include "include/main.h"

// init each core's workers
// TODO: properly handle multiple cores initwait when i get smart enough
void init(int cpu_id) {
    switch (cpu_id) {
    case 0:
#ifndef SILENT
        if (xcfg.uart_bus < 0) {
            xcfg.uart_bus = UART_BUS;
#ifdef UART_FIND_BUS
            for (int i = 0; i <= 6; i++) {
                if (vp(PERV_GET_REG(PERV_CTRL_RESET, (PERV_CTRL_RESET_DEV_UART0 + i))) == 0) {
                    xcfg.uart_bus = i;
                    break;
                }
            }
#endif
        }
        if (xcfg.uart_rate > 0)
            uart_init(xcfg.uart_bus, xcfg.uart_rate);
#endif
        printf("init alice [%X], me @ %X\n", get_build_timestamp(), init);
        zero_init();
        break;
    case 1:
        while (!(g_core_status[cpu_id - 1] & CORE_STATUS_RUNNING))
            ;
        one_init();
        break;
    case 2:
        while (!(g_core_status[cpu_id - 1] & CORE_STATUS_RUNNING))
            ;
        two_init();
        break;
    case 3:
        while (!(g_core_status[cpu_id - 1] & CORE_STATUS_RUNNING))
            ;
        three_init();
        break;
    default:
        break;
    }

    while (!(g_core_status[3] & CORE_STATUS_RUNNING))
        ;
}

void main(int cpu_id) {
    while (1) {
        switch (cpu_id) {
        case 0:
            zero_main();
            break;
        case 1:
            one_main();
            break;
        case 2:
            two_main();
            break;
        case 3:
            three_main();
            break;
        default:
            break;
        }
    }
}