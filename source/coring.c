#include <inttypes.h>
#include "include/types.h"
#include "include/utils.h"

#include "include/debug.h"

#include "include/coring.h"

volatile int g_core_status[4];
volatile core_task_s* volatile g_core_tasks[4];

void core_task_handler(void* (*get_task_by_id)(int id)) {
    int core_n = get_cpu_id();
    do {
        ;
    } while (g_core_tasks[core_n] == NULL);

    volatile core_task_s* task = g_core_tasks[core_n];

    //g_core_tasks[core_n] = NULL; // clear queue since we copied ptr here

    g_core_status[core_n] |= CORE_STATUS_TASKING;

    while (task) {
        if (task->status & CORE_TASK_STATUS_DONE || task->status & CORE_TASK_STATUS_FAILED) {
            task = (volatile core_task_s*)task->next;
            continue;
        }

        task->status |= CORE_TASK_STATUS_ACCEPTED;

        int (*core_task)(int a0, int a1, int a2, int a3) = get_task_by_id(task->task_id);
        if (!core_task) { // invalid cmd, break
            if (task->task_id >= 0) { // might be a ptr
                task->status |= CORE_TASK_STATUS_FAILED;
                break;
            }
            task->status |= CORE_TASK_STATUS_ISPTR;
            core_task = (void*)(task->task_id & ~CORE_TASK_TYPE_ISPTR);
        }

        task->status |= CORE_TASK_STATUS_RUNNING;
        task->ret = core_task(task->args[0], task->args[1], task->args[2], task->args[3]);

        task->status |= CORE_TASK_STATUS_DONE;
        
        task = (volatile core_task_s*)task->next;
    }

    g_core_status[core_n] &= ~CORE_STATUS_TASKING;

    g_core_tasks[core_n] = NULL; // TEMPORARY TASK RESTORE FIX UNTIL EXCRET

    return;
}

int core_schedule_task(int target_core, core_task_s* task, bool wait_core_done, bool wait_task_done) {
    if (g_core_status[target_core] & CORE_STATUS_TASKING) {
        if (!wait_core_done)
            return -1;
        while (g_core_status[target_core] & CORE_STATUS_TASKING)
            ;
    }

    task->status = 0;
    g_core_tasks[target_core] = task;

    sev();
    
    if (!wait_task_done)
        return 0;
    
    do {
        if (task->status & CORE_TASK_STATUS_FAILED)
            break;
    } while (!(task->status & CORE_TASK_STATUS_DONE));
    
    return task->ret;
}