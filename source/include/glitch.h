#ifndef __GLITCH_H__
#define __GLITCH_H__

#include "types.h"

void glitch_watchdog(uint32_t addr);
void glitch_loopSKR(uint32_t sk_addr, bool second);

#endif