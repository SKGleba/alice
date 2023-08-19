#include "include/types.h"
#include "include/compile_time.h"
#include "include/utils.h"

void delay(int n) {
    volatile int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 200; j++)
            ;
}

__attribute__((noinline))
uint32_t get_build_timestamp(void) {
    return (uint32_t)UNIX_TIMESTAMP;
}

unsigned int get_cpu_id(void) {
    unsigned int mpidr;
    asm volatile("mrc p15, 0, %0, c0, c0, 5\n\t" : "=r"(mpidr));
    return mpidr & 0xF;
}

unsigned int get_dfsr(bool clear) {
    unsigned int ret;
    asm volatile("mrc p15, 0, %0, c5, c0, 0\n\t" : "=r"(ret));
    if (clear)
        asm volatile("mcr p15, 0, %0, c5, c0, 0\n\t" : : "r"(!clear));
    return ret;
}

unsigned int get_ifsr(bool clear) {
    unsigned int ret;
    asm volatile("mrc p15, 0, %0, c5, c0, 1\n\t" : "=r"(ret));
    if (clear)
        asm volatile("mcr p15, 0, %0, c5, c0, 1\n\t" : : "r"(!clear));
    return ret;
}

unsigned int get_adfsr(bool clear) {
    unsigned int ret;
    asm volatile("mrc p15, 0, %0, c5, c1, 0\n\t" : "=r"(ret));
    if (clear)
        asm volatile("mcr p15, 0, %0, c5, c1, 0\n\t" : : "r"(!clear));
    return ret;
}

unsigned int get_aifsr(bool clear) {
    unsigned int ret;
    asm volatile("mrc p15, 0, %0, c5, c1, 1\n\t" : "=r"(ret));
    if (clear)
        asm volatile("mcr p15, 0, %0, c5, c1, 1\n\t" : : "r"(!clear));
    return ret;
}

unsigned int get_dfar(bool clear) {
    unsigned int ret;
    asm volatile("mrc p15, 0, %0, c6, c0, 0\n\t" : "=r"(ret));
    if (clear)
        asm volatile("mcr p15, 0, %0, c6, c0, 0\n\t" : : "r"(!clear));
    return ret;
}

unsigned int get_ifar(bool clear) {
    unsigned int ret;
    asm volatile("mrc p15, 0, %0, c6, c0, 2\n\t" : "=r"(ret));
    if (clear)
        asm volatile("mcr p15, 0, %0, c6, c0, 2\n\t" : : "r"(!clear));
    return ret;
}