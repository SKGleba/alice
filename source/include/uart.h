#ifndef __UART_H__
#define __UART_H__

#include "types.h"
#include "defs.h"
#include "paddr.h"

#define UART_REGS(i)			((void *)(UART0_OFFSET + (i) * UART0_SIZE))

enum UART_BAUD_TO_CLKGEN {
    UART_BAUD_115200 = 0x1001A,
    UART_BAUD_38400 = 0x1004E,
};

void uart_init(int bus, unsigned int clk);
void uart_write(int bus, unsigned int data);
void uart_print(int bus, char* str);
void uart_printn(int bus, char* str, int n);

#endif