#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "types.h"
#include "defs.h"
#include "uart.h"
#include "main.h"

#ifdef SILENT

#define print(str)
#define printf
#define printn(str, n)
#define printx(x)
#define printp(x)
#define hexdump(addr, length, show_addr)

#else

#define print(str) uart_print(xcfg.uart_bus, (char *)(str))
#define printf debug_printFormat
#define printn(str, n) uart_printn(xcfg.uart_bus, (char *)(str), n)
#define printx(x) debug_printU32((uint32_t)(x), true)
#define printp(x) printf("%X: %X\n", (uint32_t)(x), vp (x))
#define hexdump(addr, length, show_addr) debug_printRange(addr, length, (int)show_addr)

#endif

bool debug_coreID(bool set, bool enabled, char** id);
void debug_printU32(uint32_t value, int add_nl);
void debug_printFormat(char* base, ...);
void debug_printRange(uint32_t addr, uint32_t size, int show_addr);

#endif