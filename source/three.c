#include <inttypes.h>

#include "include/uart.h"
#include "include/debug.h"
#include "include/defs.h"
#include "include/utils.h"
#include "include/main.h"
#include "include/coring.h"

#include "include/three.h"

int three_nop(int a, int b, int c, int d) {
    return 3;
}

void* three_get_task_by_id(int task_id) {
    switch (task_id) {
    case THREE_TASKS_NOP:
        return three_nop;
    default:
        return NULL;
    }
}

void three_init(void) {
    printf("ready\n");
    g_core_status[3] |= CORE_STATUS_RUNNING;
}

void three_main(void) {
    while (1)
        core_task_handler(three_get_task_by_id);
}