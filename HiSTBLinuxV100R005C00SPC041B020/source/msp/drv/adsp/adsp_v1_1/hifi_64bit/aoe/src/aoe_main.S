#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"aoe_main.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	AOE_Reset
	.type	AOE_Reset, %function
AOE_Reset:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC0
	str	wzr, [x0]
	ldp	x29, x30, [sp], 16
	ret
	.size	AOE_Reset, .-AOE_Reset
	.align	3
.LC0:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_Init
	.type	AOE_Init, %function
AOE_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC1
	ldr	w0, [x19]
	cbz	w0, .L9
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L9:
	ldr	x21, .LC2
	mov	w1, 2816
	mov	w0, -45056
	add	x20, x19, 24
	ldr	x2, [x21, 96]
	blr	x2
	str	x0, [x19, 8]
	ldr	x3, [x21, 40]
	mov	x2, 2816
	mov	w1, 0
	blr	x3
	ldr	x0, [x19, 8]
	str	x0, [x19, 16]
	add	x0, x0, 832
	bl	AIP_Init
	ldr	x0, [x19, 8]
	add	x0, x0, 1600
	bl	AOP_Init
	ldr	x0, [x19, 8]
	add	x0, x0, 64
	bl	ENGINE_Init
	ldr	x3, [x21, 40]
	mov	x2, 24
	mov	w1, 0
	mov	x0, x20
	blr	x3
	mov	w2, 32
	mov	w1, 122880
	mov	w0, 26
	bl	HI_ADSP_VMALLOC_Aligned
	cbz	x0, .L4
	mov	w2, 2
	mov	w1, 61440
	str	x0, [x19, 40]
	stp	w1, w2, [x19, 28]
.L4:
	mov	w0, 1
	str	w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	AOE_Init, .-AOE_Init
	.align	3
.LC1:
	.xword	.LANCHOR0
	.align	3
.LC2:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_InitWKBuf
	.type	AOE_InitWKBuf, %function
AOE_InitWKBuf:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	w20, w1
	mov	w22, w2
	bl	_mcount
	ldr	x19, .LC5
	ldr	x0, .LC4
	mov	x2, 24
	add	x19, x19, x2
	mov	w1, 0
	ldr	x3, [x0, 40]
	mov	x0, x19
	blr	x3
	sub	w0, w20, #1
	cmp	w0, 1
	bls	.L17
.L10:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L17:
	mul	w1, w20, w21
	mov	w2, w22
	mov	w0, 26
	bl	HI_ADSP_VMALLOC_Aligned
	cbz	x0, .L10
	stp	w21, w20, [x19, 4]
	str	x0, [x19, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	AOE_InitWKBuf, .-AOE_InitWKBuf
	.align	3
.LC4:
	.xword	g_AdspOsalFunc
	.align	3
.LC5:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_DeInitWKBuf
	.type	AOE_DeInitWKBuf, %function
AOE_DeInitWKBuf:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC7
	mov	w0, 26
	add	x19, x19, 24
	ldr	x1, [x19, 16]
	bl	HI_ADSP_VFREE_Aligned
	ldr	x3, .LC8
	mov	x2, 24
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 40]
	blr	x3
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_DeInitWKBuf, .-AOE_DeInitWKBuf
	.align	3
.LC7:
	.xword	.LANCHOR0
	.align	3
.LC8:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_DeInit
	.type	AOE_DeInit, %function
AOE_DeInit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC10
	ldr	w0, [x19]
	cbnz	w0, .L26
.L19:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L26:
	bl	AIP_DeInit
	bl	AOP_DeInit
	bl	ENGINE_DeInit
	bl	AOE_DeInitWKBuf
	str	wzr, [x19]
	ldr	x0, [x19, 8]
	str	xzr, [x19, 16]
	cbz	x0, .L19
	ldr	x1, .LC11
	ldr	x1, [x1, 104]
	blr	x1
	str	xzr, [x19, 8]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_DeInit, .-AOE_DeInit
	.align	3
.LC10:
	.xword	.LANCHOR0
	.align	3
.LC11:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_FlushWKBuf
	.type	AOE_FlushWKBuf, %function
AOE_FlushWKBuf:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC12
	str	wzr, [x0, 24]
	ldp	x29, x30, [sp], 16
	ret
	.size	AOE_FlushWKBuf, .-AOE_FlushWKBuf
	.align	3
.LC12:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_GetWKBuf
	.type	AOE_GetWKBuf, %function
AOE_GetWKBuf:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC13
	ldr	w1, [x2, 24]
	ldr	w0, [x2, 32]
	add	w5, w1, 1
	cmp	w1, w0
	blt	.L29
	mov	w5, 1
	mov	w1, 0
.L29:
	ldr	w4, [x2, 28]
	ldr	x0, [x2, 40]
	str	w5, [x2, 24]
	mul	w1, w1, w4
	ldp	x29, x30, [sp], 16
	add	x0, x0, x1
	ret
	.size	AOE_GetWKBuf, .-AOE_GetWKBuf
	.align	3
.LC13:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_ReleaseWKBuf
	.type	AOE_ReleaseWKBuf, %function
AOE_ReleaseWKBuf:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC15
	add	x2, x0, 24
	ldr	w1, [x0, 24]
	cbnz	w1, .L34
	ldr	w1, [x2, 8]
.L34:
	sub	w1, w1, #1
	str	w1, [x0, 24]
	ldp	x29, x30, [sp], 16
	ret
	.size	AOE_ReleaseWKBuf, .-AOE_ReleaseWKBuf
	.align	3
.LC15:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_GetWKBufSize
	.type	AOE_GetWKBufSize, %function
AOE_GetWKBufSize:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC17
	ldp	x29, x30, [sp], 16
	ldr	w0, [x0, 28]
	ret
	.size	AOE_GetWKBufSize, .-AOE_GetWKBufSize
	.align	3
.LC17:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_SchedStatisticsInit
	.type	AOE_SchedStatisticsInit, %function
AOE_SchedStatisticsInit:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC18
	ldr	w1, [x0]
	cbz	w1, .L36
	ldr	x0, [x0, 16]
	str	wzr, [x0, 12]
.L36:
	ldp	x29, x30, [sp], 16
	ret
	.size	AOE_SchedStatisticsInit, .-AOE_SchedStatisticsInit
	.align	3
.LC18:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_SchedStatisticsShow
	.type	AOE_SchedStatisticsShow, %function
AOE_SchedStatisticsShow:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	uxth	w20, w1
	bl	_mcount
	ldr	x0, .LC19
	ldr	w1, [x0]
	cbz	w1, .L41
	cbz	w19, .L47
	cmp	w19, 1
	beq	.L48
.L41:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L47:
	ldr	x0, [x0, 16]
	ldr	w1, [x0, 12]
	bfi	w1, w20, 0, 16
	str	w1, [x0, 12]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L48:
	ldr	x0, [x0, 16]
	ldr	w1, [x0, 12]
	bfi	w1, w20, 16, 16
	str	w1, [x0, 12]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_SchedStatisticsShow, .-AOE_SchedStatisticsShow
	.align	3
.LC19:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_ProcThread_Sw
	.type	AOE_ProcThread_Sw, %function
AOE_ProcThread_Sw:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC20
	ldr	w0, [x19]
	str	wzr, [x19, 48]
	cbnz	w0, .L53
.L49:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L53:
	bl	AIP_Process
	bl	AOP_Process
	bl	ENGINE_Process
	ldr	w0, [x19, 52]
	mov	w1, 1
	add	w0, w0, w1
	stp	w1, w0, [x19, 48]
	and	w0, w0, 3
	cbnz	w0, .L49
	str	wzr, [x19, 48]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_ProcThread_Sw, .-AOE_ProcThread_Sw
	.align	3
.LC20:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_ProcThread
	.type	AOE_ProcThread, %function
AOE_ProcThread:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC21
	ldr	w0, [x19]
	str	wzr, [x19, 48]
	cbnz	w0, .L58
.L54:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L58:
	bl	AIP_Process
	bl	AOP_Process
	bl	ENGINE_Process
	ldr	w0, [x19, 52]
	mov	w1, 1
	add	w0, w0, w1
	stp	w1, w0, [x19, 48]
	tbnz	x0, 0, .L54
	str	wzr, [x19, 48]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_ProcThread, .-AOE_ProcThread
	.align	3
.LC21:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_GetThreadStatus
	.type	AOE_GetThreadStatus, %function
AOE_GetThreadStatus:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC22
	ldp	x29, x30, [sp], 16
	ldr	w0, [x0, 48]
	ret
	.size	AOE_GetThreadStatus, .-AOE_GetThreadStatus
	.align	3
.LC22:
	.xword	.LANCHOR0
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	g_stAoeInitFlag, %object
	.size	g_stAoeInitFlag, 4
g_stAoeInitFlag:
	.zero	4
	.zero	4
	.type	g_pRegMapAddr, %object
	.size	g_pRegMapAddr, 8
g_pRegMapAddr:
	.zero	8
	.type	g_pstAOEComReg, %object
	.size	g_pstAOEComReg, 8
g_pstAOEComReg:
	.zero	8
	.type	g_stWkbuf, %object
	.size	g_stWkbuf, 24
g_stWkbuf:
	.zero	24
	.type	g_enAoeTaskStatus, %object
	.size	g_enAoeTaskStatus, 4
g_enAoeTaskStatus:
	.zero	4
	.type	g_u32AoeTaskTicks, %object
	.size	g_u32AoeTaskTicks, 4
g_u32AoeTaskTicks:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
