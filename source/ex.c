#include <inttypes.h>

#include "include/uart.h"
#include "include/debug.h"
#include "include/defs.h"
#include "include/utils.h"

#include "include/zero.h"
#include "include/one.h"
#include "include/two.h"
#include "include/three.h"

#include "include/main.h"

#include "include/ex.h"

const char* exc_types[] = { "UNDEF", "SWI", "PABT", "DABT", "PABT", "RESERVED", "IRQ", "FIQ" };

volatile bool g_bss_cleared = false;

void c_RESET(int cpu_id) {

    if (cpu_id == 0 && !g_bss_cleared) {
        for (uint32_t i = (uint32_t)&prog_bss_addr; i < (uint32_t)&prog_bss_end; i -= -4)
            vp i = 0;
        printf("warning: did reset\n");
        g_bss_cleared = true;
    }

    while (!g_bss_cleared)
        ;

    init(cpu_id);

    while (1) {
        main(cpu_id);
        wfe();
    }
}

// temp
void c_EXC(int cpu_id, uint32_t exc_pc, uint32_t phlr) {
    int exc_id = (phlr - (uint32_t)&exc_prehandlers) >> 4;
    printf("|------>  !EXCEPTION!  <------|\n");
    printf(" pc %X\n", exc_pc);
    printf(" phlr %X\n", phlr);
    printf(" type %s\n", exc_types[exc_id]);
    printf(" dfsr %X\n", get_dfsr(true));
    printf(" ifsr %X\n", get_ifsr(true));
    printf(" adfsr %X\n", get_adfsr(true));
    printf(" aifsr %X\n", get_aifsr(true));
    printf(" dfar %X\n", get_dfar(true));
    printf(" ifar %X\n", get_ifar(true) - 4); // is this right?, bing ai says so and i cant bring myself to pabt at 4AM..
    printf("|------>  !EXCEPTION!  <------|\n");
    
    while (1) {
        main(cpu_id);
        wfe();
    }
}