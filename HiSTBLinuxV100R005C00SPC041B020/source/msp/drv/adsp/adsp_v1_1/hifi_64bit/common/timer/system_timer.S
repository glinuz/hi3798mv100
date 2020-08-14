#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"system_timer.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	TimerSwFunc, %function
TimerSwFunc:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC0
	ldr	w1, [x0]
	cmp	w1, 2
	beq	.L7
.L2:
	ldr	x2, .LC1
	mov	w1, 5
	mov	w0, 0
	ldr	x2, [x2, 176]
	blr	x2
	ldp	x29, x30, [sp], 16
	ret
	.p2align 3
.L7:
	ldr	x0, [x0, 8]
	cbz	x0, .L2
	blr	x0
	ldr	x2, .LC1
	mov	w1, 5
	mov	w0, 0
	ldr	x2, [x2, 176]
	blr	x2
	ldp	x29, x30, [sp], 16
	ret
	.size	TimerSwFunc, .-TimerSwFunc
	.align	3
.LC0:
	.xword	.LANCHOR0
	.align	3
.LC1:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	TIMER_RegIsrFunc
	.type	TIMER_RegIsrFunc, %function
TIMER_RegIsrFunc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC2
	str	x19, [x0, 8]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	TIMER_RegIsrFunc, .-TIMER_RegIsrFunc
	.align	3
.LC2:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	TIMER_DelIsrFunc
	.type	TIMER_DelIsrFunc, %function
TIMER_DelIsrFunc:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC3
	str	xzr, [x0, 8]
	ldp	x29, x30, [sp], 16
	ret
	.size	TIMER_DelIsrFunc, .-TIMER_DelIsrFunc
	.align	3
.LC3:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	TIMER_SetEnable
	.type	TIMER_SetEnable, %function
TIMER_SetEnable:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC4
	mov	w1, 2
	str	w1, [x0]
	ldp	x29, x30, [sp], 16
	ret
	.size	TIMER_SetEnable, .-TIMER_SetEnable
	.align	3
.LC4:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	TIMER_SetDisable
	.type	TIMER_SetDisable, %function
TIMER_SetDisable:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC5
	mov	w1, 3
	str	w1, [x0]
	ldp	x29, x30, [sp], 16
	ret
	.size	TIMER_SetDisable, .-TIMER_SetDisable
	.align	3
.LC5:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	TIMER_Init
	.type	TIMER_Init, %function
TIMER_Init:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x4, .LC7
	mov	w5, 1
	ldr	x3, .LC6
	mov	w2, 5
	ldr	x1, .LC8
	mov	w0, 0
	ldr	x4, [x4, 168]
	str	w5, [x3]
	str	xzr, [x3, 8]
	blr	x4
	ldp	x29, x30, [sp], 16
	ret
	.size	TIMER_Init, .-TIMER_Init
	.align	3
.LC6:
	.xword	.LANCHOR0
	.align	3
.LC7:
	.xword	g_AdspOsalFunc
	.align	3
.LC8:
	.xword	TimerSwFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	TIMER_DeInit
	.type	TIMER_DeInit, %function
TIMER_DeInit:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC10
	mov	w0, 0
	ldr	x1, .LC9
	ldr	x2, [x2, 192]
	str	wzr, [x1]
	str	xzr, [x1, 8]
	blr	x2
	ldp	x29, x30, [sp], 16
	ret
	.size	TIMER_DeInit, .-TIMER_DeInit
	.align	3
.LC9:
	.xword	.LANCHOR0
	.align	3
.LC10:
	.xword	g_AdspOsalFunc
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	g_Timer_Status, %object
	.size	g_Timer_Status, 4
g_Timer_Status:
	.zero	4
	.zero	4
	.type	g_TIMERRm, %object
	.size	g_TIMERRm, 8
g_TIMERRm:
	.zero	8
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
