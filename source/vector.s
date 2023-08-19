.cpu cortex-a9
.align 4
.code 32

.text

.section .text.vectors

.global vectors_exceptions
.global vectors_handler_addrs
.global sp_addr_per_core

vectors_exceptions:
	.rept 8
	ldr pc, [pc, #0x18]
	.endr

vectors_handler_addrs:
	.word exc_RESET
	.word exc_UNDEF
	.word exc_SWI
	.word exc_PABT
	.word exc_DABT
	.word exc_RESERVED
	.word exc_IRQ
	.word exc_FIQ

sp_addr_per_core:
	.word cfg_sp_core0_addr
	.word cfg_sp_core1_addr
	.word cfg_sp_core2_addr
	.word cfg_sp_core3_addr
