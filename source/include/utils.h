#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"
//#include <intrinsics.h>

// atrocious, but i love it
#define p *(uint32_t*)
#define vp *(volatile uint32_t*)
#define v8p *(volatile uint8_t*)
#define v16p *(volatile uint16_t*)
#define v32p *(volatile uint32_t*)
#define v64p *(volatile uint64_t*)

void delay_nx(int n, int x);
extern void delay(int n);
#define delay_inline(n) __asm__ __volatile__("1: subs %0, %0, #1; bne 1b" : : "r"(n))

// get compile timestamp
__attribute__((noinline)) uint32_t get_build_timestamp(void);

// stub func for disabled features
int stub();

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