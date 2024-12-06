#ifndef __DEFS_H__
#define __DEFS_H__

// hard definitions

#include "types.h"

//#define SILENT // suppress prints and uart init

#ifndef SILENT
#define UART_BUS 1 // default uart bus to print to
#define UART_FIND_BUS // if defined, will use the first enabled uart bus
#endif

#define RPC_READ_DELAY 0x2000 // delay between RPC checks
#define RPC_WRITE_DELAY 0x100 // delay before replying to a RPC
#define RPC_BLOCKED_DELAY 0x100 // delay between g_status check for unblock

#define SDIF_UNUSE // dont include the storage/sdif code (very large, around 6kb)
#define SDIF_NOINITS // dont include the storage/sdif init functions (saves around 2kb, requires ctx import)

// from linker.x
extern unsigned int prog_bss_addr;
extern unsigned int prog_bss_end;
extern unsigned int cfg_phys_load_off;

#endif