#ifndef __UART_H__
#define __UART_H__

#include <hardware/uart.h>

#include "types.h"
#include "defs.h"

void uart_init(int bus, unsigned int clk);
void uart_write(int bus, unsigned int data);
void uart_print(int bus, char* str);
void uart_printn(int bus, char* str, int n);

#endif