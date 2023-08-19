.cpu cortex-a9
.align 4
.code 32

.text

.section .text.exh

.global exc_prehandlers
.global exc_RESET
.global exc_EXC


.macro PREHANDLE type
.global exc_\type
exc_\type:
	mcr p15, 0, lr, c6, c0, 2
	bl exc_EXC
1:
	wfi
	b 1b
.endm

exc_prehandlers:
PREHANDLE UNDEF
PREHANDLE SWI
PREHANDLE PABT
PREHANDLE DABT
PREHANDLE RESERVED
PREHANDLE IRQ
PREHANDLE FIQ


exc_RESET:
	# disable irq, fiq, abt | enter system mode
	cpsid	aif, #0x1F

	# clear core local record and global of a *ex caller
	clrex

	# get current core
	mrc	p15, 0, r1, c0, c0, 5
	and	r1, r1, #0xF

	# set sp per core based on config from vectors
	ldr r0, =sp_addr_per_core
	add r0, r0, r1, lsl #2
	ldr sp, [r0], #0

	# c reset (can be thumb)
	mov r0, r1
	blx	c_RESET

	# ded
1:
	wfi
	b 1b


# temporary handler for all non-reset exceptions, acts as soft reset
# TODO: add ctx save & restore, exc ret
exc_EXC:
	# for c exc handler to id the exc
	mov r2, lr

	# disable irq, fiq, abt | enter system mode
	cpsid	aif, #0x1F

	# clear core local record and global of a *ex caller
	clrex

	# get current core
	mrc	p15, 0, r1, c0, c0, 5
	and	r1, r1, #0xF

	# set sp per core based on config from vectors
	ldr r0, =sp_addr_per_core
	add r0, r0, r1, lsl #2
	ldr sp, [r0], #0

	# c exc (can be thumb)
	mov r0, r1
	mrc p15, 0, r1, c6, c0, 2
	sub r1, r1, #0x8
	blx	c_EXC

	# ded
1:
	wfi
	b 1b