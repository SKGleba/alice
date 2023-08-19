#ifndef __EX_H__
#define __EX_H__

void c_RESET(int cpu_id);
void c_EXC(int cpu_id, uint32_t exc_pc, uint32_t phlr);

extern volatile bool g_bss_cleared;
extern void* exc_prehandlers;

#endif