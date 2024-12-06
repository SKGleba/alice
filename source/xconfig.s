.cpu cortex-a9
.align 4
.code 32

.data

.section .data.xcfg

.global xcfg
.type xcfg, "object"

.macro ADD_XCFG_ENTRY name value:vararg
.global xcfg_\name
xcfg_\name:
	.word \value
.endm

xcfg:
ADD_XCFG_ENTRY sp_addr_per_core cfg_sp_core0_addr,cfg_sp_core1_addr,cfg_sp_core2_addr,cfg_sp_core3_addr
ADD_XCFG_ENTRY uart_bus 0xFFFFFFFF
ADD_XCFG_ENTRY uart_rate 0xFFFFFFFF