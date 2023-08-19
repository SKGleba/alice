#ifndef __COMPAT_H__
#define __COMPAT_H__

#include "types.h"

typedef struct compat_paddr_list {
    uint32_t paddr;
    uint32_t size;
} compat_paddr_list;

int compat_loadSK(uint32_t addr, bool second);

#endif