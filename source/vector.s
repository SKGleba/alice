.cpu cortex-a9
.align 4
.code 32

.text

.section .text.vectors

.global vectors_exceptions
.global vectors_handler_addrs
.global info_addrs

vectors_exceptions:
	.rept 8
	ldr pc, [pc, #0x18]
	.endr

vectors_handler_addrs:
	.word exc_RESET, exc_UNDEF, exc_SWI, exc_PABT, exc_DABT, exc_RESERVED, exc_IRQ, exc_FIQ

info_addrs:
	.word xcfg, exports, g_core_tasks, g_core_status
