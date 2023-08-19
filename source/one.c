#include <inttypes.h>

#include "include/uart.h"
#include "include/debug.h"
#include "include/defs.h"
#include "include/utils.h"
#include "include/main.h"
#include "include/coring.h"

#include "include/one.h"

int one_nop(int a, int b, int c, int d) {
    return 1;
}

void* one_get_task_by_id(int task_id) {
    switch (task_id) {
    case ONE_TASKS_NOP:
        return one_nop;
    default:
        return NULL;
    }
}

void one_init(void) {
    printf("ready\n");
    g_core_status[1] |= CORE_STATUS_RUNNING;
}

void one_main(void) {
    while (1)
        core_task_handler(one_get_task_by_id);
}