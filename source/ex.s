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
	isb SY

	# clear core local record and global of a *ex caller
	clrex
	dsb SY

	# disable all features
	mov r0, #0x0
	mcr p15, 0, r0, c1, c0, 0
	dsb SY
	isb SY

	# invalidate bp entries
	mcr p15, 0, r0, c7, c1, 6
	mcr p15, 0, r0, c7, c5, 6
	dsb SY
	isb SY

	# invalidate instruction cache, flush bp
	mcr p15, 0, r0, c7, c1, 0
	mcr p15, 0, r0, c7, c5, 0
	dsb SY
	isb SY

	# invalidate cached TLB entries
	mcr p15, 0, r0, c8, c3, 0
	mcr p15, 0, r0, c8, c7, 0
	dsb SY

	# warm reset TTBCR
	mcr p15, 0, r0, c2, c0, 2
	dsb SY

	# clean & invalidate l1 dcache
	mov r1, #0x8000
1:
	mcr p15, 0, r0, c7, c14, 2
	add r0, r0, #0x20
	cmp r0, r1
	bne 1b
	dsb SY

	# get current core
	mrc	p15, 0, r1, c0, c0, 5
	and	r1, r1, #0xF

	# set sp per core based on config from vectors
	ldr r0, =xcfg_sp_addr_per_core
	ldr sp, [r0, r1, lsl #2]

	# hello world
	ldr r0, =g_core_status
	str sp, [r0, r1, lsl #2]

	# c reset (can be thumb)
	mov r0, r1
	blx	c_RESET

	# ded
3:
	wfi
	b 3b


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
	ldr r0, =xcfg_sp_addr_per_core
	ldr sp, [r0, r1, lsl #2]

	# c exc (can be thumb)
	mov r0, r1
	mrc p15, 0, r1, c6, c0, 2
	sub r1, r1, #0x8
	blx	c_EXC

	# ded
1:
	wfi
	b 1b
