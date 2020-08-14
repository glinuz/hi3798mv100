/**
 * cmd_entry.S
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 */

.section .text
.type _start, %function
.globl _start
_start:
	push	{r0 - r10}
	b	flying

. = 0x10
_CMD_TEXT_BASE:
	.word CMD_TEXT_BASE

_arm_start:
	.word _start
_flying:
	.word flying
_STACK_POINT:
	.word STACK_POINT

.globl _bss_start
_bss_start: .word __bss_start
.globl _bss_end
_bss_end:   .word _end

_real_start:
	.word real_start
flying:
	mov	r2, pc
	sub	r2, r2, #8
	ldr	r1, _arm_start
	ldr	r0, _flying
	sub	r1, r0, r1
	sub	r0, r2, r1
	ldr	r1, _CMD_TEXT_BASE
	cmp	r0, r1
	beq	real_start

	/* need relocation */
	ldr	r2, _arm_start
	ldr	r3, _bss_start
	sub	r2, r3, r2
	add	r2, r0, r2
self_move:
	ldmia	r0!, {r3 - r10}
	stmia	r1!, {r3 - r10}
	cmp	r0,  r2
	ble	self_move
	ldr	pc, _real_start

real_start:
	ldr	r0, _bss_start
	ldr	r1, _bss_end
	mov	r2, #0x00000000

clear_bss_loop:
	str	r2, [r0]
	cmp	r0, r1
	add	r0, r0, #4
	bne	clear_bss_loop

	ldr	r4, =_lr
	str	lr, [r4]
	pop	{r0 - r10}

	ldr	lr, =_sp
	str	sp, [lr]
	ldr	sp, _STACK_POINT

	bl	ddr_training_cmd_entry

	ldr	lr, =_sp
	ldr	sp, [lr]

	ldr	lr, =_lr
	ldr	pc, [lr]

.section .data
_lr:	.word 0
_sp:	.word 0
