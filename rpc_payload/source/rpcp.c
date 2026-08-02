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
#include "../../source/include/bob.h"
#include "../../source/include/rpc.h"

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
        bob_sendSimpleCmd(BOB_A2B_MASK_RPC_STATUS, RPC_STATUS_REQUEST_BLOCK, false, 0);
        delay(0x6000);
        uint32_t* buffer = 0x1f840000;
        memset(buffer, 0, 0x00200000);
        uint32_t cur = 0, pre = 0, z = 0;
        while (vp(0xE0000000) != 2) {
            cur = vp(0xE0010004);
            if (cur != pre)
                buffer[++z] = cur;
        }
        delay(0x8000);
        for (uint32_t a = 0; a < (z + 4); a++)
            printx(buffer[a]);
    }

    if (testno < test_count)
        goto do_tests;

    print("[RPCP] bye\n\n");
    return ret;
}