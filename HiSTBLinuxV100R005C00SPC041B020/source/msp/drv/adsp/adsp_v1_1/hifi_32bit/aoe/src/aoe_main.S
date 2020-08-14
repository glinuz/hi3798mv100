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
	.file	"aoe_main.c"
	.text
	.align	2
	.global	AOE_Reset
	.type	AOE_Reset, %function
AOE_Reset:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L2
	mov	r2, #0
	str	r2, [r3]
	ldmfd	sp, {fp, sp, pc}
.L3:
	.align	2
.L2:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_Reset, .-AOE_Reset
	.align	2
	.global	AOE_InitWKBuf
	.type	AOE_InitWKBuf, %function
AOE_InitWKBuf:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L12
	mov	r5, r1
	ldr	r4, .L12+4
	mov	r6, r0
	mov	r7, r2
	mov	r1, #0
	ldr	r3, [r3, #20]
	add	r0, r4, #4
	mov	r2, #16
	blx	r3
	sub	r3, r5, #1
	cmp	r3, #1
	ldmhifd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r2, r7
	mov	r0, #26
	mul	r1, r6, r5
	bl	HI_ADSP_VMALLOC_Aligned
	cmp	r0, #0
	strne	r0, [r4, #16]
	strne	r5, [r4, #12]
	strne	r6, [r4, #8]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L13:
	.align	2
.L12:
	.word	g_AdspOsalFunc
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_InitWKBuf, .-AOE_InitWKBuf
	.align	2
	.global	AOE_Init
	.type	AOE_Init, %function
AOE_Init:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L16
	ldr	r5, [r4]
	cmp	r5, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r6, .L16+4
	mov	r0, #20480
	mov	r1, #2816
	movt	r0, 65535
	ldr	r3, [r6, #56]
	blx	r3
	mov	r1, r5
	mov	r2, #2816
	ldr	r3, [r6, #20]
	str	r0, [r4, #20]
	blx	r3
	ldr	r3, [r4, #20]
	add	r0, r3, #832
	str	r3, [r4, #24]
	bl	AIP_Init
	ldr	r0, [r4, #20]
	add	r0, r0, #1600
	bl	AOP_Init
	ldr	r0, [r4, #20]
	add	r0, r0, #64
	bl	ENGINE_Init
	mov	r2, #32
	mov	r1, #2
	mov	r0, #61440
	bl	AOE_InitWKBuf
	mov	r3, #1
	str	r3, [r4]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L17:
	.align	2
.L16:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	AOE_Init, .-AOE_Init
	.align	2
	.global	AOE_DeInitWKBuf
	.type	AOE_DeInitWKBuf, %function
AOE_DeInitWKBuf:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L19
	mov	r0, #26
	ldr	r1, [r4, #16]
	bl	HI_ADSP_VFREE_Aligned
	ldr	r3, .L19+4
	add	r0, r4, #4
	mov	r2, #16
	mov	r1, #0
	ldr	r3, [r3, #20]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L20:
	.align	2
.L19:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	AOE_DeInitWKBuf, .-AOE_DeInitWKBuf
	.align	2
	.global	AOE_DeInit
	.type	AOE_DeInit, %function
AOE_DeInit:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L28
	ldr	r3, [r4]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	bl	AIP_DeInit
	mov	r5, #0
	bl	AOP_DeInit
	bl	ENGINE_DeInit
	bl	AOE_DeInitWKBuf
	ldr	r0, [r4, #20]
	str	r5, [r4, #24]
	cmp	r0, r5
	str	r5, [r4]
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L28+4
	ldr	r3, [r3, #60]
	blx	r3
	str	r5, [r4, #20]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L29:
	.align	2
.L28:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	AOE_DeInit, .-AOE_DeInit
	.align	2
	.global	AOE_FlushWKBuf
	.type	AOE_FlushWKBuf, %function
AOE_FlushWKBuf:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L31
	mov	r2, #0
	str	r2, [r3, #4]
	ldmfd	sp, {fp, sp, pc}
.L32:
	.align	2
.L31:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_FlushWKBuf, .-AOE_FlushWKBuf
	.align	2
	.global	AOE_GetWKBuf
	.type	AOE_GetWKBuf, %function
AOE_GetWKBuf:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L36
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	cmp	r2, r1
	ldr	r1, [r3, #16]
	addlt	ip, r2, #1
	movge	r2, #0
	movge	ip, #1
	mla	r0, r0, r2, r1
	str	ip, [r3, #4]
	ldmfd	sp, {fp, sp, pc}
.L37:
	.align	2
.L36:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_GetWKBuf, .-AOE_GetWKBuf
	.align	2
	.global	AOE_ReleaseWKBuf
	.type	AOE_ReleaseWKBuf, %function
AOE_ReleaseWKBuf:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L42
	ldr	r2, [r3, #4]
	cmp	r2, #0
	ldreq	r2, [r3, #12]
	sub	r2, r2, #1
	str	r2, [r3, #4]
	ldmfd	sp, {fp, sp, pc}
.L43:
	.align	2
.L42:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_ReleaseWKBuf, .-AOE_ReleaseWKBuf
	.align	2
	.global	AOE_GetWKBufSize
	.type	AOE_GetWKBufSize, %function
AOE_GetWKBufSize:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L45
	ldr	r0, [r3, #8]
	ldmfd	sp, {fp, sp, pc}
.L46:
	.align	2
.L45:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_GetWKBufSize, .-AOE_GetWKBufSize
	.align	2
	.global	AOE_SchedStatisticsInit
	.type	AOE_SchedStatisticsInit, %function
AOE_SchedStatisticsInit:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L52
	ldr	r2, [r3]
	cmp	r2, #0
	ldrne	r3, [r3, #24]
	movne	r2, #0
	strne	r2, [r3, #12]
	ldmfd	sp, {fp, sp, pc}
.L53:
	.align	2
.L52:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_SchedStatisticsInit, .-AOE_SchedStatisticsInit
	.align	2
	.global	AOE_SchedStatisticsShow
	.type	AOE_SchedStatisticsShow, %function
AOE_SchedStatisticsShow:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L61
	ldr	r2, [r3]
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r0, #0
	beq	.L60
	cmp	r0, #1
	ldreq	r3, [r3, #24]
	ldreq	r2, [r3, #12]
	bfieq	r2, r1, #16, #16
	streq	r2, [r3, #12]
	ldmfd	sp, {fp, sp, pc}
.L60:
	ldr	r3, [r3, #24]
	ldr	r2, [r3, #12]
	bfi	r2, r1, #0, #16
	str	r2, [r3, #12]
	ldmfd	sp, {fp, sp, pc}
.L62:
	.align	2
.L61:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_SchedStatisticsShow, .-AOE_SchedStatisticsShow
	.align	2
	.global	AOE_ProcThread_Sw
	.type	AOE_ProcThread_Sw, %function
AOE_ProcThread_Sw:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L67
	mov	r2, #0
	ldr	r3, [r4]
	str	r2, [r4, #28]
	cmp	r3, r2
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	bl	AIP_Process
	bl	AOP_Process
	bl	ENGINE_Process
	ldr	r3, [r4, #32]
	mov	r2, #1
	str	r2, [r4, #28]
	add	r3, r3, r2
	str	r3, [r4, #32]
	ands	r3, r3, #3
	streq	r3, [r4, #28]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L68:
	.align	2
.L67:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_ProcThread_Sw, .-AOE_ProcThread_Sw
	.align	2
	.global	AOE_ProcThread
	.type	AOE_ProcThread, %function
AOE_ProcThread:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L73
	mov	r2, #0
	ldr	r3, [r4]
	str	r2, [r4, #28]
	cmp	r3, r2
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	bl	AIP_Process
	bl	AOP_Process
	bl	ENGINE_Process
	ldr	r3, [r4, #32]
	mov	r2, #1
	str	r2, [r4, #28]
	add	r3, r3, r2
	str	r3, [r4, #32]
	ands	r3, r3, r2
	streq	r3, [r4, #28]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L74:
	.align	2
.L73:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_ProcThread, .-AOE_ProcThread
	.align	2
	.global	AOE_GetThreadStatus
	.type	AOE_GetThreadStatus, %function
AOE_GetThreadStatus:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L76
	ldr	r0, [r3, #28]
	ldmfd	sp, {fp, sp, pc}
.L77:
	.align	2
.L76:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AOE_GetThreadStatus, .-AOE_GetThreadStatus
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stAoeInitFlag, %object
	.size	g_stAoeInitFlag, 4
g_stAoeInitFlag:
	.space	4
	.type	g_stWkbuf, %object
	.size	g_stWkbuf, 16
g_stWkbuf:
	.space	16
	.type	g_pRegMapAddr, %object
	.size	g_pRegMapAddr, 4
g_pRegMapAddr:
	.space	4
	.type	g_pstAOEComReg, %object
	.size	g_pstAOEComReg, 4
g_pstAOEComReg:
	.space	4
	.type	g_enAoeTaskStatus, %object
	.size	g_enAoeTaskStatus, 4
g_enAoeTaskStatus:
	.space	4
	.type	g_u32AoeTaskTicks, %object
	.size	g_u32AoeTaskTicks, 4
g_u32AoeTaskTicks:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
