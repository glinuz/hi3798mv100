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
	.file	"sw_main.c"
	.text
	.align	2
	.type	SchedStatisticsStop, %function
SchedStatisticsStop:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L9
	mov	r5, r0
	ldr	r3, [r4, #64]
	blx	r3
	ldr	r3, [r4, #128]
	movw	r2, #19923
	cmp	r5, #0
	movt	r2, 4194
	rsb	r3, r3, r0
	str	r0, [r4, #120]
	str	r1, [r4, #124]
	umull	r2, r3, r3, r2
	mov	r3, r3, lsr #6
	beq	.L8
	cmp	r3, #41
	movls	r3, #0
	movhi	r3, #1
	cmp	r5, #1
	movne	r3, #0
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldrh	r1, [r4, #138]
	mov	r0, #1
	add	r1, r1, r0
	uxth	r1, r1
	strh	r1, [r4, #138]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	AOE_SchedStatisticsShow
.L8:
	cmp	r3, #4
	ldmlsfd	sp, {r4, r5, fp, sp, pc}
	ldrh	r1, [r4, #136]
	mov	r0, r5
	add	r1, r1, #1
	uxth	r1, r1
	strh	r1, [r4, #136]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	AOE_SchedStatisticsShow
.L10:
	.align	2
.L9:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	SchedStatisticsStop, .-SchedStatisticsStop
	.align	2
	.type	AoEngineTask, %function
AoEngineTask:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L18
	ldr	r3, [r6, #72]
	mov	r5, r6
	mov	r7, r6
	blx	r3
	bl	AOE_Init
	mov	r3, #0
	mov	r0, #0
	mov	r1, #0
	strh	r3, [r6, #138]
	strh	r3, [r6, #136]
	str	r3, [r6, #140]
	strd	r0, [r6, #128]
	strd	r0, [r6, #120]
	bl	AOE_SchedStatisticsInit
	movw	r3, #42405
	bfi	r3, r3, #16, #16
	str	r3, [r6, #144]
.L13:
	ldr	r4, [r6, #148]
	ldr	r3, .L18
	cmp	r4, #1
	bne	.L17
.L14:
	ldr	r3, [r5, #64]
	blx	r3
	strd	r0, [r5, #128]
	bl	AOE_ProcThread_Sw
	mov	r0, #0
	bl	SchedStatisticsStop
	bl	AOE_GetThreadStatus
	cmp	r0, #1
	beq	.L13
	ldr	r3, [r7, #64]
	blx	r3
	ldr	r3, [r7, #68]
	strd	r0, [r7, #128]
	movw	r0, #19264
	movt	r0, 76
	blx	r3
	mov	r0, r4
	bl	SchedStatisticsStop
	ldr	r4, [r6, #148]
	ldr	r3, .L18
	cmp	r4, #1
	beq	.L14
.L17:
	ldr	r3, [r3, #76]
	blx	r3
	bl	AOE_DeInit
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L19:
	.align	2
.L18:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AoEngineTask, .-AoEngineTask
	.align	2
	.global	AOE_SwEngineCreate
	.type	AOE_SwEngineCreate, %function
AOE_SwEngineCreate:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L29
	mov	r3, #0
	mov	r4, #1
	mov	r0, r5
	str	r3, [r5, #144]
	str	r4, [r5, #148]
	bl	ASDP_OSAL_InterfaceInit
	ldr	r3, [r5, #80]
	add	r0, r5, #152
	ldr	r2, .L29+4
	ldr	r1, .L29+8
	blx	r3
	cmp	r0, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r5, #88]
	ldr	r0, [r5, #152]
	blx	r3
	bl	TIMER_Init
	bl	TIMER_SetEnable
	ldr	r2, [r5, #144]
	movw	r3, #42405
	movt	r3, 42405
	cmp	r2, r3
	beq	.L23
	mov	r0, r4
	ldr	r3, [r5, #16]
	blx	r3
	movw	r4, #511
	mov	r6, r5
	b	.L24
.L28:
	ldr	r3, [r6, #16]
	blx	r3
	subs	r4, r4, #1
	beq	.L23
.L24:
	ldr	r2, [r5, #144]
	movw	r3, #42405
	movt	r3, 42405
	mov	r0, #1
	cmp	r2, r3
	bne	.L28
.L23:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L30:
	.align	2
.L29:
	.word	.LANCHOR0
	.word	AoEngineTask
	.word	.LC0
	UNWIND(.fnend)
	.size	AOE_SwEngineCreate, .-AOE_SwEngineCreate
	.align	2
	.global	AOE_SwEngineDestory
	.type	AOE_SwEngineDestory, %function
AOE_SwEngineDestory:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L32
	mov	r5, #0
	str	r5, [r4, #148]
	bl	TIMER_SetDisable
	bl	TIMER_DeInit
	ldr	r0, [r4, #152]
	ldr	r3, [r4, #84]
	blx	r3
	str	r5, [r4, #152]
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L33:
	.align	2
.L32:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_SwEngineDestory, .-AOE_SwEngineDestory
	.global	g_AdspOsalFunc
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"AoEngineTask\000" )
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	g_AdspOsalFunc, %object
	.size	g_AdspOsalFunc, 120
g_AdspOsalFunc:
	.space	120
	.type	g_u64EndPts, %object
	.size	g_u64EndPts, 8
g_u64EndPts:
	.space	8
	.type	g_u64BeginPts, %object
	.size	g_u64BeginPts, 8
g_u64BeginPts:
	.space	8
	.type	g_u16ExcuteTimeoutCnt, %object
	.size	g_u16ExcuteTimeoutCnt, 2
g_u16ExcuteTimeoutCnt:
	.space	2
	.type	g_u16SchTimeoutCnt, %object
	.size	g_u16SchTimeoutCnt, 2
g_u16SchTimeoutCnt:
	.space	2
	.type	g_u32cnt, %object
	.size	g_u32cnt, 4
g_u32cnt:
	.space	4
	.type	g_AoeRunningFlag, %object
	.size	g_AoeRunningFlag, 4
g_AoeRunningFlag:
	.space	4
	.type	g_AoeTaskstate, %object
	.size	g_AoeTaskstate, 4
g_AoeTaskstate:
	.space	4
	.type	g_pAOEThread, %object
	.size	g_pAOEThread, 4
g_pAOEThread:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
