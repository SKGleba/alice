#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"
//#include <intrinsics.h>

// atrocious, but i love it
#define p *(uint32_t*)
#define vp *(volatile uint32_t*)

// wait for ~n * 200 cycles
void delay(int n);

// get compile timestamp
__attribute__((noinline)) uint32_t get_build_timestamp(void);

unsigned int get_cpu_id(void);

#define dmb() asm volatile("dmb\n\t" ::: "memory")
#define dsb() asm volatile("dsb\n\t" ::: "memory")
#define wfe() asm volatile("wfe\n\t")
#define sev() asm volatile("sev\n\t")

unsigned int get_dfsr(bool clear);
unsigned int get_ifsr(bool clear);
unsigned int get_adfsr(bool clear);
unsigned int get_aifsr(bool clear);
unsigned int get_dfar(bool clear);
unsigned int get_ifar(bool clear);

#endif