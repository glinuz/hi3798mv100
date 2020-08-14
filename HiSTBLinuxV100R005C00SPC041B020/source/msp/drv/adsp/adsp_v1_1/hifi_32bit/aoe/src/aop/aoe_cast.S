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
	.file	"aoe_cast.c"
	.text
	.align	2
	.type	CASTCalcSize, %function
CASTCalcSize:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r1
	mov	r5, r0
	ldr	r1, [r1]
	ldr	r0, [r4, #4]
	bl	UTIL_CalcFrameSize
	ldr	r3, [r4, #8]
	movw	r1, #19923
	movt	r1, 4194
	mul	r3, r3, r5
	umull	r2, r3, r3, r1
	mov	r3, r3, lsr #6
	tst	r3, #3
	bicne	r3, r3, #3
	addne	r3, r3, #4
	mul	r0, r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	CASTCalcSize, .-CASTCalcSize
	.align	2
	.type	CAST_TimerHandle, %function
CAST_TimerHandle:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L24
	mov	r5, #0
	ldr	r9, .L24+4
.L15:
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	beq	.L7
	ldr	r3, [r9, #112]
	mov	r0, r5
	blx	r3
	ldr	r0, [r4, #36]
	ldr	r7, [r4, #32]
	bl	RBUF_QueryBusy
	mov	r8, r0
	ldr	r0, [r4, #40]
	bl	RBUF_QueryFree
	cmp	r7, r8
	bhi	.L8
	cmp	r7, r0
	mov	r2, r7
	bhi	.L14
.L20:
	mov	r3, #1
	ldr	r1, [r4, #40]
	ldr	r0, [r4, #36]
	bl	RBUF_CopyData
	ldr	r2, [r4, #44]
	add	r2, r2, #1
	str	r2, [r4, #44]
.L10:
	ldr	r3, [r4, #56]
	add	r3, r3, #1
	str	r3, [r4, #56]
	ldr	r3, [r6, #-16]
	str	r2, [r3, #28]
	ldr	r2, [r4, #48]
	str	r2, [r3, #32]
	ldr	r2, [r4, #52]
	str	r2, [r3, #36]
	ldr	r2, [r4, #56]
	str	r2, [r3, #40]
	ldr	r2, [r4, #60]
	str	r2, [r3, #24]
	ldr	r2, [r4, #64]
	str	r2, [r3, #16]
	ldr	r2, [r4, #68]
	str	r2, [r3, #20]
.L11:
	ldr	r3, [r9, #116]
	mov	r0, r5
	blx	r3
.L7:
	add	r5, r5, #1
	cmp	r5, #4
	bne	.L15
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L8:
	cmp	r8, #0
	bne	.L12
	cmp	r7, r0
	bls	.L23
	ldr	r3, [r4, #52]
	add	r3, r3, #1
	str	r3, [r4, #52]
	b	.L11
.L12:
	cmp	r8, r0
	mov	r2, r8
	bls	.L20
.L14:
	mov	r1, #0
	ldr	r0, [r4, #36]
	bl	RBUF_ReadData
	ldr	r3, [r4, #52]
	add	r3, r3, #1
	str	r3, [r4, #52]
	b	.L11
.L23:
	mov	r2, r7
	mov	r1, r8
	ldr	r0, [r4, #40]
	bl	RBUF_WriteDataDma
	ldr	r3, [r4, #48]
	ldr	r2, [r4, #44]
	add	r3, r3, #1
	str	r3, [r4, #48]
	b	.L10
.L25:
	.align	2
.L24:
	.word	.LANCHOR0+32
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	CAST_TimerHandle, .-CAST_TimerHandle
	.align	2
	.global	CAST_SetAopBuf
	.type	CAST_SetAopBuf, %function
CAST_SetAopBuf:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L37
	ldr	r2, [r3, #4]
	cmp	r0, r2
	beq	.L30
	ldr	r2, [r3, #8]
	cmp	r0, r2
	beq	.L31
	ldr	r2, [r3, #12]
	cmp	r0, r2
	beq	.L32
	ldr	r2, [r3, #16]
	cmp	r0, r2
	moveq	r2, #3
	ldmnefd	sp, {fp, sp, pc}
.L28:
	add	r3, r3, r2, lsl #2
	ldr	r3, [r3, #36]
	str	r1, [r3, #40]
	ldmfd	sp, {fp, sp, pc}
.L30:
	mov	r2, #0
	b	.L28
.L31:
	mov	r2, #1
	b	.L28
.L32:
	mov	r2, #2
	b	.L28
.L38:
	.align	2
.L37:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	CAST_SetAopBuf, .-CAST_SetAopBuf
	.align	2
	.global	CAST_Destroy
	.type	CAST_Destroy, %function
CAST_Destroy:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L50
	ldr	r2, [r3, #4]
	cmp	r0, r2
	beq	.L43
	ldr	r2, [r3, #8]
	cmp	r0, r2
	beq	.L44
	ldr	r2, [r3, #12]
	cmp	r0, r2
	beq	.L45
	ldr	r2, [r3, #16]
	cmp	r0, r2
	moveq	r6, #3
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L40:
	add	r4, r6, #8
	ldr	r7, .L50
	add	r4, r3, r4, lsl #2
	ldr	r5, [r4, #4]
	cmp	r5, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r8, .L50+4
	mov	r0, r6
	ldr	r9, [r5, #40]
	ldr	r3, [r8, #112]
	blx	r3
	mov	r1, r5
	mov	r0, #26
	bl	HI_ADSP_FREE
	ldr	r3, [r8, #116]
	mov	r2, #0
	mov	r0, r6
	str	r2, [r4, #4]
	blx	r3
	mov	r0, r9
	bl	RBUF_Destroy
	ldr	r3, [r7]
	sub	r3, r3, #1
	str	r3, [r7]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L43:
	mov	r6, #0
	b	.L40
.L44:
	mov	r6, #1
	b	.L40
.L45:
	mov	r6, #2
	b	.L40
.L51:
	.align	2
.L50:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	CAST_Destroy, .-CAST_Destroy
	.align	2
	.global	CAST_Create
	.type	CAST_Create, %function
CAST_Create:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r5, .L64
	str	r3, [fp, #-60]
	mov	r3, #0
	mov	r8, r0
	mov	r6, r1
	ldr	r7, [r5, #36]
	mov	r9, r2
	str	r3, [fp, #-48]
	cmp	r7, r3
	beq	.L53
	ldr	r3, [r5, #40]
	cmp	r3, #0
	beq	.L59
	ldr	r3, [r5, #44]
	cmp	r3, #0
	beq	.L60
	ldr	r3, [r5, #48]
	cmp	r3, #0
	moveq	r7, #3
	beq	.L53
.L55:
	mov	r5, #0
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L59:
	mov	r7, #1
.L53:
	mov	r1, #72
	mov	r0, #26
	bl	HI_ADSP_MALLOC
	subs	r4, r0, #0
	beq	.L55
	ldr	r10, .L64+4
	mov	r2, #72
	mov	r1, #0
	add	r3, r4, #12
	str	r3, [fp, #-56]
	add	r5, r5, r7, lsl #2
	ldr	ip, [r10, #20]
	blx	ip
	ldr	r2, [fp, #-60]
	str	r9, [r4, #12]
	mov	r0, #65
	ldr	r1, [fp, #-56]
	str	r2, [r4, #16]
	ldr	r2, [fp, #4]
	ldr	r9, .L64
	str	r2, [r4, #20]
	mov	r2, #0
	ldr	r5, [r5, #20]
	str	r2, [r6]
	bl	CASTCalcSize
	add	r2, r5, #12
	add	r1, r5, #8
	str	r2, [r6, #16]
	str	r1, [r6, #12]
	str	r0, [r6, #8]
	ldr	r1, [r4, #12]
	ldr	r0, [r4, #16]
	bl	UTIL_CalcFrameSize
	ldr	r2, [r6, #8]
	mov	r1, r6
	str	r0, [r6, #24]
	sub	r0, fp, #48
	str	r2, [r5, #4]
	str	r8, [r5]
	bl	RBUF_Create
	ldr	r5, [fp, #-48]
	cmp	r5, #0
	beq	.L57
	add	lr, r9, r7, lsl #2
	ldr	r2, [r9]
	ldr	r1, [fp, #-56]
	mov	r0, #5
	str	r5, [r4, #36]
	add	r2, r2, #1
	str	r8, [lr, #4]
	str	r2, [r9]
	bl	CASTCalcSize
	str	r0, [r4, #32]
	mov	r0, r7
	ldr	r2, [r10, #108]
	add	r7, r9, r7, lsl #2
	blx	r2
	ldr	r3, [r10, #20]
	mov	r1, #0
	mov	r2, #28
	add	r0, r4, #44
	blx	r3
	ldr	r5, [fp, #-48]
	str	r4, [r7, #36]
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L57:
	mov	r1, r4
	mov	r0, #26
	bl	HI_ADSP_FREE
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L60:
	mov	r7, #2
	b	.L53
.L65:
	.align	2
.L64:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	CAST_Create, .-CAST_Create
	.align	2
	.global	CAST_FiFoTryCnt
	.type	CAST_FiFoTryCnt, %function
CAST_FiFoTryCnt:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L77
	ldr	r2, [r3, #4]
	cmp	r0, r2
	beq	.L70
	ldr	r2, [r3, #8]
	cmp	r0, r2
	beq	.L71
	ldr	r2, [r3, #12]
	cmp	r0, r2
	beq	.L72
	ldr	r2, [r3, #16]
	cmp	r0, r2
	moveq	r2, #3
	ldmnefd	sp, {fp, sp, pc}
.L67:
	add	r3, r3, r2, lsl #2
	ldr	r3, [r3, #36]
	cmp	r3, #0
	ldrne	r2, [r3, #64]
	addne	r2, r2, #1
	strne	r2, [r3, #64]
	ldmfd	sp, {fp, sp, pc}
.L70:
	mov	r2, #0
	b	.L67
.L71:
	mov	r2, #1
	b	.L67
.L72:
	mov	r2, #2
	b	.L67
.L78:
	.align	2
.L77:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	CAST_FiFoTryCnt, .-CAST_FiFoTryCnt
	.align	2
	.global	CAST_FiFoFreeCnt
	.type	CAST_FiFoFreeCnt, %function
CAST_FiFoFreeCnt:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L90
	ldr	r2, [r3, #4]
	cmp	r0, r2
	beq	.L83
	ldr	r2, [r3, #8]
	cmp	r0, r2
	beq	.L84
	ldr	r2, [r3, #12]
	cmp	r0, r2
	beq	.L85
	ldr	r2, [r3, #16]
	cmp	r0, r2
	moveq	r2, #3
	ldmnefd	sp, {fp, sp, pc}
.L80:
	add	r3, r3, r2, lsl #2
	ldr	r3, [r3, #36]
	cmp	r3, #0
	ldrne	r2, [r3, #68]
	addne	r2, r2, #1
	strne	r2, [r3, #68]
	ldmfd	sp, {fp, sp, pc}
.L83:
	mov	r2, #0
	b	.L80
.L84:
	mov	r2, #1
	b	.L80
.L85:
	mov	r2, #2
	b	.L80
.L91:
	.align	2
.L90:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	CAST_FiFoFreeCnt, .-CAST_FiFoFreeCnt
	.align	2
	.global	CAST_Init
	.type	CAST_Init, %function
CAST_Init:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r7, .L95
	mov	r4, r0
	mov	r1, #0
	mov	r2, #64
	ldr	r0, .L95+4
	mov	r6, r1
	ldr	r3, [r7, #20]
	add	r4, r4, #1024
	blx	r3
	ldr	r5, .L95+4
	mov	r8, r6
.L93:
	str	r4, [r5, #20]
	mov	r0, r4
	ldr	r3, [r7, #20]
	add	r6, r6, #1
	mov	r2, #48
	mov	r1, #0
	blx	r3
	cmp	r6, #4
	str	r8, [r5, #36]
	mov	r3, #8
	add	r4, r4, #48
	str	r3, [r5, #4]!
	bne	.L93
	ldr	r0, .L95+8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	TIMER_RegIsrFunc
.L96:
	.align	2
.L95:
	.word	g_AdspOsalFunc
	.word	.LANCHOR0
	.word	CAST_TimerHandle
	UNWIND(.fnend)
	.size	CAST_Init, .-CAST_Init
	.align	2
	.global	CAST_DeInit
	.type	CAST_DeInit, %function
CAST_DeInit:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	bl	TIMER_DelIsrFunc
	ldr	r3, .L98
	mov	r2, #0
	str	r2, [r3, #36]
	str	r2, [r3, #40]
	str	r2, [r3, #44]
	str	r2, [r3, #48]
	ldmfd	sp, {fp, sp, pc}
.L99:
	.align	2
.L98:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	CAST_DeInit, .-CAST_DeInit
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stCastRm, %object
	.size	g_stCastRm, 64
g_stCastRm:
	.space	64
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
