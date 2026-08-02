#ifndef __MAIN_H__
#define __MAIN_H__

#include "types.h"

// mark as volatile (!!)
struct _xcfg_s {
    void* sp_per_core[4];
    int uart_bus;
    int uart_rate;
};
typedef struct _xcfg_s xcfg_s;

extern volatile xcfg_s xcfg;

bool init(int cpu_id);
bool main(int cpu_id);

#endif