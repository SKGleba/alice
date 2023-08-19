.cpu cortex-a9
.align 4
.code 32

.text

.section .text.exports

.macro STATIC_EXPORT func
.global x_\func
x_\func:
	ldr pc, xa_\func
xa_\func:
	.word \func
.endm

STATIC_EXPORT main
STATIC_EXPORT init
STATIC_EXPORT debug_printFormat
STATIC_EXPORT core_schedule_task