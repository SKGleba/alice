#include "../../source/include/types.h"

#include "../../source/include/defs.h"
#include "../../source/include/uart.h"
#include "../../source/include/debug.h"
#include "../../source/include/utils.h"
#include "../../source/include/clib.h"
#include "../../source/include/maika.h"
#include "../../source/include/spi.h"
#include "../../source/include/perv.h"
#include "../../source/include/gpio.h"
#include "../../source/include/i2c.h"
#include "../../source/include/paddr.h"
#include "../../source/include/ernie.h"
#include "../../source/include/compat.h"

#define TEST_COUNT 1

__attribute__((section(".text.rpcp")))
int rpcp(uint32_t arg0, uint32_t arg1, void* extra_data) {
    printf("[RPCP] hello world (%X, %X, %X)\n", arg0, arg1, (uint32_t)extra_data);

    int ret = 0;
    int testno = 0;
    int test_count = TEST_COUNT;
    if (arg1)
        test_count = arg1;
do_tests:
    testno++;
    printf("[RPCP] test number %X\n", testno);

    {
        g_uart_bus = 0;
        uart_init(g_uart_bus, UART_BAUD_38400);
        ret = ernie_exec_cmd_short(0xb2, 1, 1);
        if (ret < 0)
            return ret;
        delay(0x800);
        compat_loadSK(arg0, arg1);
        printf("[RPCP] GLSTART \n");
        while (1) {};
    }

    if (testno < test_count)
        goto do_tests;

    print("[RPCP] bye\n\n");
    return ret;
}