#include <hardware/maika.h>
#include <hardware/xbar.h>
#include "include/debug.h"
#include "include/clib.h"
#include "include/zero.h"
#include "include/utils.h"

#include "include/test.h"

void dfl_test(int cpu_id) {
    if (cpu_id < 0)
        return;
    zero_enable_rpc(false, 0, false);
    return;
}