#include "hi_asm_define.h"
	.arch armv7-a
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"system_timer.c"
	.text
	.align	2
	.type	TimerSwFunc, %function
TimerSwFunc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L8
	ldr	r2, [r3]
	cmp	r2, #2
	beq	.L7
.L2:
	ldr	r3, .L8+4
	mov	r1, #5
	mov	r0, #0
	ldr	r3, [r3, #96]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L7:
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L2
	blx	r3
	b	.L2
.L9:
	.align	2
.L8:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	TimerSwFunc, .-TimerSwFunc
	.align	2
	.global	TIMER_RegIsrFunc
	.type	TIMER_RegIsrFunc, %function
TIMER_RegIsrFunc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L11
	str	r0, [r3, #4]
	ldmfd	sp, {fp, sp, pc}
.L12:
	.align	2
.L11:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	TIMER_RegIsrFunc, .-TIMER_RegIsrFunc
	.align	2
	.global	TIMER_DelIsrFunc
	.type	TIMER_DelIsrFunc, %function
TIMER_DelIsrFunc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L14
	mov	r2, #0
	str	r2, [r3, #4]
	ldmfd	sp, {fp, sp, pc}
.L15:
	.align	2
.L14:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	TIMER_DelIsrFunc, .-TIMER_DelIsrFunc
	.align	2
	.global	TIMER_SetEnable
	.type	TIMER_SetEnable, %function
TIMER_SetEnable:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L17
	mov	r2, #2
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L18:
	.align	2
.L17:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	TIMER_SetEnable, .-TIMER_SetEnable
	.align	2
	.global	TIMER_SetDisable
	.type	TIMER_SetDisable, %function
TIMER_SetDisable:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L20
	mov	r2, #3
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L21:
	.align	2
.L20:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	TIMER_SetDisable, .-TIMER_SetDisable
	.align	2
	.global	TIMER_Init
	.type	TIMER_Init, %function
TIMER_Init:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, .L23
	mov	ip, #0
	ldr	r3, .L23+4
	mov	r4, #1
	mov	r0, ip
	mov	r2, #5
	ldr	lr, [r1, #92]
	stmia	r3, {r4, ip}
	ldr	r1, .L23+8
	mov	r3, lr
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	bx	r3
.L24:
	.align	2
.L23:
	.word	g_AdspOsalFunc
	.word	.LANCHOR0
	.word	TimerSwFunc
	UNWIND(.fnend)
	.size	TIMER_Init, .-TIMER_Init
	.align	2
	.global	TIMER_DeInit
	.type	TIMER_DeInit, %function
TIMER_DeInit:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, .L26
	mov	r3, #0
	ldr	r1, .L26+4
	mov	r0, r3
	str	r3, [r2, #4]
	ldr	r1, [r1, #104]
	str	r3, [r2]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r1
.L27:
	.align	2
.L26:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	TIMER_DeInit, .-TIMER_DeInit
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_Timer_Status, %object
	.size	g_Timer_Status, 4
g_Timer_Status:
	.space	4
	.type	g_TIMERRm, %object
	.size	g_TIMERRm, 4
g_TIMERRm:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
