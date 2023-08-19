#include <inttypes.h>

#include "include/uart.h"
#include "include/debug.h"
#include "include/defs.h"
#include "include/utils.h"
#include "include/main.h"
#include "include/coring.h"

#include "include/two.h"

int two_nop(int a, int b, int c, int d) {
    return 2;
}

void* two_get_task_by_id(int task_id) {
    switch (task_id) {
    case TWO_TASKS_NOP:
        return two_nop;
    default:
        return NULL;
    }
}

void two_init(void) {
    printf("ready\n");
    g_core_status[2] |= CORE_STATUS_RUNNING;
}

void two_main(void) {
    while (1)
        core_task_handler(two_get_task_by_id);
}