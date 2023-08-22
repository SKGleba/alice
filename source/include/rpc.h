#ifndef __RPC_H__
#define __RPC_H__

#include "jig.h"

#define RPC_MAGIC 0xA21C
#define RPC_FLAG_REPLY 0b10000000 // data is my reply
#define RPC_FLAG_EXTRA 0b01000000 // use extra_data

#define RPC_STATUS_RUNNING 0b1
#define RPC_STATUS_BLOCKED 0b10
#define RPC_STATUS_INCMD 0b100

#define RPC_STATUS_REQUEST_STOP 0x8000
#define RPC_STATUS_REQUEST_BLOCK 0x800000
#define RPC_STATUS_DISABLED 0x80000000

enum RPC_COMMANDS {
    RPC_CMD_NOP,
    RPC_CMD_READ32,
    RPC_CMD_WRITE32,
    RPC_CMD_MEMSET,
    RPC_CMD_MEMCPY,
    RPC_CMD_SET_DELAY,
    RPC_CMD_STOP_RPC,
    RPC_CMD_SET_PUSH,
    RPC_CMD_HEXDUMP,
    RPC_CMD_MEMSET32,
    RPC_CMD_LOAD_SK,
    RPC_CMD_ENABLE_GLITCH_WATCHDOG,
    RPC_CMD_SK_RESET_LOOP,
    RPC_CMD_BOB_READ32,
    RPC_CMD_BOB_WRITE32,
    RPC_CMD_COPYTO = RPC_FLAG_EXTRA,
    RPC_CMD_COPYFROM,
    RPC_CMD_EXEC, // exec arg0(arg1, arg2, &extra) | ret to arg0
    RPC_CMD_EXEC_EXTENDED // exec argX(extra32[X], extra32[X+1], extra32[X+2], extra32[X+3]) | rets to argX
};

struct _rpc_cmd_s { // size is 0x10
    uint16_t magic;
    uint8_t hash;
    uint8_t cmd_id;
    uint32_t args[3];
} __attribute__((packed));
typedef struct _rpc_cmd_s rpc_cmd_s;

struct _rpc_buf_s { // size is 0x40
    rpc_cmd_s cmd;
    uint8_t extra_data[JIG_KERMIT_SHBUF_SIZE - sizeof(rpc_cmd_s)];
} __attribute__((packed));
typedef struct _rpc_buf_s rpc_buf_s;

extern int g_rpc_status;

void rpc_loop(void);
void rpc_loop_exclusive(void);

#endif