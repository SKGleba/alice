#ifndef __CORING_H__
#define __CORING_H__

#include "types.h"

// mark as volatile (!!)
struct _core_task_s {
    int task_id;
    int args[4];
    int status;
    int ret;
    void* next;
};
typedef struct _core_task_s core_task_s;

#define CORE_TASK_STATUS_ACCEPTED 0b1
#define CORE_TASK_STATUS_RUNNING 0b10
#define CORE_TASK_STATUS_DONE 0b100
#define CORE_TASK_STATUS_ISPTR 0b1000
#define CORE_TASK_STATUS_FAILED 0x80000000

extern volatile core_task_s* volatile g_core_tasks[4];

#define CORE_STATUS_RUNNING 0b1 // core is running
#define CORE_STATUS_WAITING 0b10 // core is waiting
#define CORE_STATUS_TASKING 0b100 // core is running a task (chain)

extern volatile int g_core_status[4];

void core_task_handler(void* (*get_task_by_id)(int id));
int core_schedule_task(int target_core, core_task_s* task, bool wait_core_done, bool wait_task_done);

#endif