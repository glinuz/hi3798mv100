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
	.file	"mpeg4.c"
	.text
	.align	2
	.global	log2bin
	.type	log2bin, %function
log2bin:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	mov	r3, #0
.L3:
	add	r3, r3, #1
	movs	r0, r0, lsr #1
	uxth	r3, r3
	bne	.L3
	sxth	r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	log2bin, .-log2bin
	.align	2
	.global	bs_get_spritetrajectory
	.type	bs_get_spritetrajectory, %function
bs_get_spritetrajectory:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L13
	mov	r7, r0
	mov	r4, #0
	mov	r5, r6
.L10:
	ldrb	r1, [r5, #4]	@ zero_extendqisi2
	mov	r0, r7
	bl	BsShow
	ldr	r3, [r6, r4, asl #3]
	ldr	r2, .L13
	mov	r1, r4, asl #3
	add	r5, r5, #8
	cmp	r0, r3
	beq	.L12
	add	r4, r4, #1
	cmp	r4, #12
	bne	.L10
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L12:
	add	r2, r2, r1
	mov	r0, r7
	ldrb	r1, [r2, #4]	@ zero_extendqisi2
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	bs_get_spritetrajectory, .-bs_get_spritetrajectory
	.global	__aeabi_idiv
	.align	2
	.global	MP4_GetGMCparam
	.type	MP4_GetGMCparam, %function
MP4_GetGMCparam:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldrb	r5, [r0, #65]	@ zero_extendqisi2
	mov	r3, #2
	mov	r10, r0
	mov	r3, r3, asl r5
	mov	r9, r1
	mov	r0, #16
	rsb	r2, r5, #3
	mov	r1, r3
	str	r3, [fp, #-56]
	str	r2, [fp, #-100]
	bl	__aeabi_idiv
	ldr	r8, [r10, #708]
	ldr	r3, [r10, #704]
	cmp	r8, #1
	mov	r2, r8, asl #4
	str	r2, [r9, #8]
	str	r2, [fp, #-64]
	ldr	r2, [r10, #704]
	mov	r2, r2, asl #4
	str	r2, [r9, #12]
	ldrb	r2, [r10, #65]	@ zero_extendqisi2
	str	r2, [r9, #4]
	ldrb	r2, [r10, #64]	@ zero_extendqisi2
	str	r2, [r9]
	str	r2, [fp, #-68]
	ldr	r2, [r10, #696]
	ldr	ip, [r10, #680]
	str	r2, [fp, #-52]
	ldr	r2, [r10, #700]
	str	r2, [fp, #-72]
	str	r0, [fp, #-48]
	add	r0, r10, #684
	ldmia	r0, {r0, r1, lr}
	ble	.L40
	mov	r5, #0
	mov	r2, #1
.L17:
	add	r5, r5, #1
	mov	r6, r2, asl r5
	cmp	r8, r6
	bgt	.L17
	mov	r2, r6, asl #4
	str	r2, [fp, #-96]
.L16:
	cmp	r3, #1
	ble	.L41
	mov	r4, #0
	mov	r2, #1
.L19:
	add	r4, r4, #1
	mov	r7, r2, asl r4
	cmp	r3, r7
	bgt	.L19
	mov	r2, r7, asl #4
	str	r2, [fp, #-104]
.L18:
	ldrb	r2, [r10, #12]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L45
	add	r2, ip, r8, lsl #1
	add	lr, r0, lr
	add	r1, r1, r2
	ldr	r2, [fp, #-56]
	str	lr, [fp, #-76]
	add	r10, r0, r3, lsl #1
	ldr	lr, [fp, #-52]
	mov	r2, r2, asr #1
	str	r2, [fp, #-52]
	ldr	r2, [fp, #-72]
	add	lr, ip, lr
	add	r10, r2, r10
	ldr	r2, [fp, #-52]
	mul	ip, r2, ip
	mul	r1, r1, r2
	str	ip, [fp, #-60]
	ldr	ip, [fp, #-76]
	mul	ip, ip, r2
	str	ip, [fp, #-84]
	mul	ip, lr, r2
	str	ip, [fp, #-88]
	mul	ip, r10, r2
	mul	r2, r2, r0
	str	ip, [fp, #-92]
	str	r2, [fp, #-52]
.L21:
	ldr	r2, [fp, #-64]
	ldr	ip, [fp, #-48]
	ldr	lr, [fp, #-60]
	rsb	r0, r2, #0
	rsb	r2, r6, r8
	str	r3, [fp, #-72]
	mla	r0, r1, ip, r0
	mov	r1, r8
	mul	r10, lr, ip
	mov	ip, r8, asr #1
	str	r2, [fp, #-76]
	str	ip, [fp, #-80]
	mul	r0, r6, r0
	mla	r0, r10, r2, r0
	cmp	r0, #0
	addgt	r0, r0, ip
	rsble	r0, ip, r0
	bl	__aeabi_idiv
	ldr	r2, [fp, #-84]
	ldr	r3, [fp, #-48]
	ldr	ip, [fp, #-80]
	mul	r1, r2, r3
	ldr	r2, [fp, #-52]
	mul	r3, r2, r3
	ldr	r2, [fp, #-76]
	mul	r1, r6, r1
	mla	r2, r3, r2, r1
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-96]
	mov	r1, r8
	cmp	r2, #0
	add	r3, r0, r3
	str	r3, [fp, #-76]
	ldr	r3, [fp, #-72]
	addgt	r0, r2, ip
	rsble	r0, ip, r2
	str	r3, [fp, #-72]
	bl	__aeabi_idiv
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-48]
	ldr	r8, [fp, #-92]
	mul	lr, r3, r2
	ldr	r3, [fp, #-72]
	rsb	r2, r7, r3
	mov	r1, r3, asr #1
	mov	ip, r0
	mul	r0, r7, lr
	ldr	lr, [fp, #-48]
	mla	r10, r2, r10, r0
	mov	r0, r3, asl #4
	rsb	r0, r0, #0
	mla	r0, r8, lr, r0
	ldr	lr, [fp, #-64]
	cmp	r10, #0
	addgt	r10, r10, r1
	rsble	r10, r1, r10
	mul	r0, r7, r0
	mla	r0, r2, lr, r0
	cmp	r0, #0
	addgt	r2, r0, r1
	rsble	r2, r1, r0
	str	r2, [fp, #-72]
	ldr	r2, [fp, #-68]
	cmp	r2, #3
	ldrls	pc, [pc, r2, asl #2]
	b	.L30
.L32:
	.word	.L42
	.word	.L33
	.word	.L34
	.word	.L35
.L42:
	mov	r0, #0
	mov	r3, r0
	mov	r8, r0
	mov	r10, r0
.L31:
	mov	r5, #0
	mov	ip, r5
.L39:
	ldr	r1, [fp, #-56]
	mov	r10, r10, asr r5
	mov	r3, r3, asr ip
	mov	r5, r8, asr r5
	mov	r0, r0, asr ip
	str	r3, [r9, #40]
	mov	r2, #1
	mov	r3, #0
	str	r10, [r9, #32]
	str	r5, [r9, #36]
	str	r0, [r9, #44]
	str	r1, [r9, #16]
	str	r1, [r9, #28]
	str	r3, [r9, #20]
	str	r3, [r9, #24]
	str	r2, [r9]
	b	.L15
.L35:
	ldr	r2, [fp, #-48]
	cmp	r4, r5
	ldr	r1, [fp, #-60]
	rsb	r2, r2, #0
	ldr	lr, [fp, #-100]
	str	ip, [fp, #-84]
	mov	r0, r2
	add	r2, r5, r4
	add	r2, r2, lr
	movge	r4, r5
	mul	ip, r1, r0
	rsb	r5, r4, r2
	mov	lr, #1
	sub	r8, r5, #1
	mov	r8, lr, asl r8
	ldr	lr, [fp, #-76]
	mov	r7, r7, asr r4
	str	r0, [fp, #-68]
	add	r2, lr, ip
	ldr	lr, [fp, #-52]
	mov	r0, r10
	add	r10, r8, r1, asl r5
	mov	r1, r3
	str	r10, [r9, #32]
	add	r8, r8, lr, asl r5
	str	r8, [r9, #36]
	mov	r4, r6, asr r4
	str	ip, [fp, #-80]
	str	r3, [fp, #-76]
	str	r4, [fp, #-64]
	mul	r4, r7, r2
	bl	__aeabi_idiv
	ldr	r3, [fp, #-76]
	mov	r2, r6, asl #1
	ldr	lr, [fp, #-52]
	mov	r6, r6, asl #4
	ldr	ip, [fp, #-84]
	mov	r1, r3
	ldr	r3, [fp, #-68]
	rsb	r6, r6, #0
	mul	r2, r7, r2
	mul	r6, r7, r6
	mul	r3, lr, r3
	str	r6, [fp, #-68]
	mov	r6, #1
	mov	lr, r3
	add	r3, r5, #1
	mov	r3, r6, asl r3
	ldr	r6, [fp, #-48]
	add	ip, ip, lr
	str	r3, [fp, #-76]
	ldr	r3, [fp, #-64]
	mul	r2, r6, r2
	str	lr, [fp, #-48]
	mul	r6, r7, ip
	ldr	r7, [fp, #-80]
	ldr	ip, [fp, #-60]
	add	r7, r0, r7
	ldr	r0, [fp, #-72]
	str	r2, [fp, #-72]
	mul	r7, r3, r7
	add	r3, r4, r7
	mla	r3, ip, r2, r3
	ldr	ip, [fp, #-68]
	ldr	r2, [fp, #-76]
	add	r3, r3, ip
	add	r3, r3, r2
	str	r3, [r9, #40]
	str	r3, [fp, #-60]
	bl	__aeabi_idiv
	ldr	r1, [fp, #-104]
	add	ip, r5, #2
	ldr	r2, [fp, #-64]
	ldr	r3, [fp, #-52]
	str	r7, [r9, #20]
	str	r4, [r9, #16]
	str	r6, [r9, #24]
	add	r0, r0, r1
	ldr	r1, [fp, #-48]
	add	lr, r0, r1
	mul	lr, r2, lr
	ldr	r2, [fp, #-72]
	add	r0, r6, lr
	mla	r2, r3, r2, r0
	ldr	r3, [fp, #-68]
	str	lr, [r9, #28]
	add	r0, r3, r2
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-76]
	add	r0, r2, r0
	str	r0, [r9, #44]
.L36:
	ldr	r2, [fp, #-56]
	cmp	r4, r2, asl r5
	beq	.L46
.L37:
	rsb	r5, r5, #16
	rsb	r1, ip, #16
	mov	r2, r4, asl r5
	mov	r10, r10, asl r5
	mov	r4, r7, asl r5
	mov	r8, r8, asl r5
	mov	r7, r6, asl r5
	mov	r3, r3, asl r1
	mov	r5, lr, asl r5
	mov	r0, r0, asl r1
	str	r10, [r9, #32]
	str	r8, [r9, #36]
	str	r2, [r9, #16]
	str	r4, [r9, #20]
	str	r7, [r9, #24]
	str	r5, [r9, #28]
	str	r3, [r9, #40]
	str	r0, [r9, #44]
.L15:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L34:
	ldr	lr, [fp, #-48]
	mov	r1, r6, asl #1
	ldr	r10, [fp, #-60]
	ldr	r0, [fp, #-76]
	rsb	r3, lr, #0
	ldr	r2, [fp, #-64]
	mul	r1, lr, r1
	rsb	r7, ip, r2
	mla	r4, r10, r3, r0
	ldr	r0, [fp, #-52]
	ldr	r2, [fp, #-100]
	ldr	r8, [fp, #-52]
	add	r5, r5, r2
	mov	r2, r6, asl #4
	mla	r6, r0, r3, ip
	add	r3, r7, r4
	mov	ip, #1
	rsb	r2, r2, #0
	mla	r3, r10, r1, r3
	add	lr, r4, r6
	add	r0, r5, #1
	str	r7, [r9, #20]
	mla	r1, r8, r1, lr
	mov	r0, ip, asl r0
	add	r3, r3, r2
	sub	r8, r5, #1
	add	r3, r3, r0
	mov	r8, ip, asl r8
	add	r2, r2, r1
	add	r10, r8, r10, asl r5
	add	r0, r0, r2
	ldr	r2, [fp, #-52]
	str	r4, [r9, #16]
	mov	lr, r4
	str	r6, [r9, #24]
	add	ip, r5, #2
	add	r8, r8, r2, asl r5
	ldr	r2, [fp, #-56]
	str	r4, [r9, #28]
	str	r3, [r9, #40]
	cmp	r4, r2, asl r5
	str	r10, [r9, #32]
	str	r0, [r9, #44]
	str	r8, [r9, #36]
	bne	.L37
.L46:
	cmp	r7, #0
	bne	.L37
	cmp	r6, #0
	bne	.L37
	cmp	r4, lr
	bne	.L37
	b	.L39
.L33:
	ldr	r2, [fp, #-60]
	ldr	r1, [fp, #-52]
	and	r3, r2, #1
	and	r0, r1, #1
	orr	r3, r3, r2, asr #1
	mov	r10, r2
	orr	r0, r0, r1, asr #1
	mov	r8, r1
	b	.L31
.L45:
	ldr	r2, [fp, #-56]
	add	lr, r0, lr
	str	lr, [fp, #-84]
	ldr	lr, [fp, #-52]
	mla	r10, r8, r2, ip
	str	r0, [fp, #-52]
	mla	r2, r3, r2, r0
	ldr	r0, [fp, #-72]
	add	lr, ip, lr
	add	r1, r1, r10
	str	lr, [fp, #-88]
	add	r2, r0, r2
	str	ip, [fp, #-60]
	str	r2, [fp, #-92]
	b	.L21
.L30:
	mov	ip, #0
	ldr	r4, [r9, #16]
	mov	r5, ip
	ldr	r10, [r9, #32]
	ldr	r8, [r9, #36]
	ldr	r3, [r9, #40]
	ldr	r0, [r9, #44]
	ldr	r7, [r9, #20]
	ldr	r6, [r9, #24]
	ldr	lr, [r9, #28]
	b	.L36
.L40:
	mov	r2, #16
	mov	r6, #1
	str	r2, [fp, #-96]
	mov	r5, #0
	b	.L16
.L41:
	mov	r2, #16
	mov	r7, #1
	str	r2, [fp, #-104]
	mov	r4, #0
	b	.L18
	UNWIND(.fnend)
	.size	MP4_GetGMCparam, .-MP4_GetGMCparam
	.align	2
	.global	Mp4_GetUserDataBuf
	.type	Mp4_GetUserDataBuf, %function
Mp4_GetUserDataBuf:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r3, [r1, r2, asl #2]
	cmp	r3, #0
	beq	.L52
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L52:
	str	r2, [fp, #-20]
	str	r1, [fp, #-16]
	bl	GetUsd
	ldr	r2, [fp, #-20]
	ldr	r1, [fp, #-16]
	cmp	r0, #0
	mov	r3, r0
	str	r0, [r1, r2, asl #2]
	addne	r0, r2, #4
	strne	r0, [r3, #1036]
	ldrne	r3, [r1, r2, asl #2]
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Mp4_GetUserDataBuf, .-Mp4_GetUserDataBuf
	.align	2
	.global	Mp4_ClearUsrData
	.type	Mp4_ClearUsrData, %function
Mp4_ClearUsrData:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	sub	r4, r1, #4
	add	r5, r1, #12
	mov	r7, r0
	mov	r6, #0
.L54:
	ldr	r1, [r4, #4]!
	mov	r0, r7
	bl	FreeUsdByDec
	cmp	r4, r5
	str	r6, [r4]
	bne	.L54
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Mp4_ClearUsrData, .-Mp4_ClearUsrData
	.align	2
	.global	Mp4_Syntax_Init
	.type	Mp4_Syntax_Init, %function
Mp4_Syntax_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L61
	mov	r4, r0
	add	r0, r0, #19968
	mov	r2, #24
	mov	r1, #0
	add	r0, r0, #8
	ldr	r3, [r5, #48]
	blx	r3
	add	r0, r4, #19968
	ldr	r3, [r5, #48]
	mov	r2, #604
	mov	r1, #0
	add	r0, r0, #32
	blx	r3
	add	r0, r4, #20992
	ldr	r3, [r5, #48]
	mov	r2, #60
	mov	r1, #0
	add	r0, r0, #216
	blx	r3
	add	r0, r4, #21248
	ldr	r3, [r5, #48]
	mov	r2, #768
	mov	r1, #0
	add	r0, r0, #20
	blx	r3
	add	r0, r4, #22016
	ldr	r3, [r5, #48]
	mov	r2, #7
	mov	r1, #0
	add	r0, r0, #20
	blx	r3
	add	r2, r4, #19968
	add	r3, r4, #20224
	ldr	r1, .L61+4
	add	r0, r4, #20480
	add	r2, r2, #116
	add	r3, r3, #116
	mov	ip, #1
	strb	ip, [r0, #805]
	str	ip, [r0, #2120]
.L57:
	ldr	ip, [r1, #4]!
	str	ip, [r2, #4]!
	cmp	r2, r3
	bne	.L57
	add	r0, r4, #20480
	ldr	r2, .L61+8
	add	r0, r0, #116
.L58:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L58
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L62:
	.align	2
.L61:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1-4
	.word	.LANCHOR1+252
	UNWIND(.fnend)
	.size	Mp4_Syntax_Init, .-Mp4_Syntax_Init
	.align	2
	.global	Global_Data_Init
	.type	Global_Data_Init, %function
Global_Data_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L64
	mov	r4, r0
	add	r0, r0, #22016
	mov	r2, #24
	mov	r1, #0
	add	r0, r0, #48
	ldr	r3, [r5, #48]
	blx	r3
	add	r0, r4, #22016
	ldr	r3, [r5, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r0, #28
	blx	r3
	mov	r0, r4
	bl	Mp4_Syntax_Init
	add	r3, r4, #20480
	mov	r2, #0
	mov	r0, r2
	str	r2, [r3, #2100]
	str	r2, [r3, #2104]
	str	r2, [r3, #2108]
	str	r2, [r3, #2112]
	str	r2, [r3, #2072]
	str	r2, [r3, #1612]
	str	r2, [r3, #2056]
	str	r2, [r3, #2060]
	str	r2, [r3, #2064]
	str	r2, [r3, #2116]
	str	r2, [r3, #2076]
	str	r2, [r3, #2080]
	str	r2, [r3, #2084]
	str	r2, [r3, #2088]
	str	r2, [r3, #2092]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L65:
	.align	2
.L64:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	Global_Data_Init, .-Global_Data_Init
	.align	2
	.global	Bs_Get_Matrix
	.type	Bs_Get_Matrix, %function
Bs_Get_Matrix:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r5, #0
	mov	r8, r0
	mov	r6, r1
	mov	r4, r5
	ldr	r7, .L76
	b	.L67
.L71:
	mov	r5, r0
.L67:
	mov	r1, #8
	mov	r0, r8
	bl	BsGet
	add	ip, r4, #1
	sxth	r9, r4
	uxth	r3, ip
	add	r1, r7, r9, lsl #2
	sxth	r2, r3
	uxth	r3, r3
	ldr	r1, [r1, #512]
	mov	r4, r3
	adds	lr, r0, #0
	str	r0, [r6, r1, asl #2]
	movne	lr, #1
	cmp	r2, #63
	movgt	r2, #0
	andle	r2, lr, #1
	cmp	r2, #0
	bne	.L71
	cmp	lr, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r9, #63
	ldmgtfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L70:
	uxth	r4, ip
	str	r5, [r6, r1, asl #2]
	add	ip, r4, #1
	sxth	r3, r4
	cmp	r3, #64
	add	r3, r7, r3, lsl #2
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r1, [r3, #512]
	b	.L70
.L77:
	.align	2
.L76:
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	Bs_Get_Matrix, .-Bs_Get_Matrix
	.align	2
	.global	NextStartCode
	.type	NextStartCode, %function
NextStartCode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #32)
	sub	sp, sp, #32
	mov	r6, r0
	sub	r0, fp, #60
	mov	r7, r3
	mov	r4, r2, asl #3
	movw	r5, #438
	bl	BsInit
	mov	r0, #0
	b	.L79
.L83:
	cmp	lr, #256
	beq	.L92
.L87:
	bl	BsSkip
	sub	r0, fp, #60
	bl	BsPos
.L79:
	rsb	ip, r0, r4
	mov	r1, #32
	cmp	ip, #31
	sub	r0, fp, #60
	ble	.L93
	bl	BsShow
	mov	r1, #8
	mov	ip, r0
	cmp	ip, r5
	bic	r3, r0, #1020
	bic	r2, r0, #15
	bic	lr, r0, #255
	bic	r3, r3, #3
	sub	r0, fp, #60
	beq	.L82
	cmp	r2, #288
	beq	.L82
	cmp	r3, #32768
	bne	.L83
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	mov	r1, #8
	sub	r0, fp, #60
	cmp	r3, #2
	bne	.L94
	ldr	r3, [r6, #744]
	add	r3, r3, #1
	str	r3, [r6, #744]
	cmp	r3, #18
	bls	.L87
	mov	r2, #196
	mov	r3, #1
	str	r2, [r7]
	sub	r0, fp, #60
	strb	r3, [r6, #9]
	bl	BsPos
	b	.L81
.L82:
	mov	r2, #2
	mov	r3, #0
	strb	r2, [r6, #9]
	sub	r0, fp, #60
	str	r3, [r6, #744]
	str	ip, [r7]
	bl	BsPos
.L81:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L93:
	mov	r3, #197
	str	r3, [r7]
	bl	BsPos
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L92:
	str	ip, [r7]
	sub	r0, fp, #60
	bl	BsPos
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L94:
	mov	r2, #1
	mov	r3, #196
	strb	r2, [r6, #9]
	str	r3, [r7]
	bl	BsPos
	b	.L81
	UNWIND(.fnend)
	.size	NextStartCode, .-NextStartCode
	.align	2
	.global	Mp4SearchStartCode
	.type	Mp4SearchStartCode, %function
Mp4SearchStartCode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #4
	ble	.L107
	add	lr, r0, r1
	add	r3, r0, #2
.L112:
	cmp	lr, r3
	bls	.L97
.L113:
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	bls	.L98
	and	ip, r2, #252
	cmp	ip, #128
	beq	.L98
.L105:
	add	r3, r3, #3
	cmp	lr, r3
	bhi	.L113
.L97:
	sub	r0, r1, #2
	ldmfd	sp, {fp, sp, pc}
.L98:
	cmp	r2, #0
	bne	.L100
	ldrb	r2, [r3, #-1]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #1
	beq	.L112
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #2
	addne	r3, r3, #4
	b	.L112
.L100:
	cmp	r2, #1
	ldrb	r2, [r3, #-2]	@ zero_extendqisi2
	beq	.L114
	cmp	r2, #0
	bne	.L105
	ldrb	r2, [r3, #-1]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L105
	rsb	r0, r0, r3
	sub	r0, r0, #2
	ldmfd	sp, {fp, sp, pc}
.L114:
	cmp	r2, #0
	bne	.L105
	ldrb	r2, [r3, #-1]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L105
	rsb	r3, r0, r3
	sub	r0, r3, #2
	ldmfd	sp, {fp, sp, pc}
.L107:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Mp4SearchStartCode, .-Mp4SearchStartCode
	.align	2
	.global	Mp4_Syntax_Gop
	.type	Mp4_Syntax_Gop, %function
Mp4_Syntax_Gop:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	mov	r4, r0
	sub	r0, fp, #60
	bl	BsInit
	mov	r1, #32
	sub	r0, fp, #60
	bl	BsSkip
	mov	r1, #5
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #6
	sub	r0, fp, #60
	bl	BsGet
	sub	r0, fp, #60
	mov	r1, #1
	bl	BsGet
	subs	r6, r0, #0
	bne	.L116
	ldr	r3, .L119
	add	r4, r4, #20480
	ldr	r1, .L119+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	strb	r6, [r4, #1558]
	sub	r0, fp, #60
	bl	BsPos
	ldr	r3, .L119+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L117
	ldr	r0, [r4, #2480]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #103
	blx	ip
	mov	r0, r5
.L117:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L116:
	mov	r1, #6
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	sub	r0, fp, #60
	add	r4, r4, #20480
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #790]
	sub	r0, fp, #60
	bl	BsGet
	mov	r3, #1
	strb	r3, [r4, #1558]
	strb	r0, [r4, #791]
	sub	r0, fp, #60
	bl	BsPos
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L120:
	.align	2
.L119:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	g_event_report
	UNWIND(.fnend)
	.size	Mp4_Syntax_Gop, .-Mp4_Syntax_Gop
	.align	2
	.global	Mp4_Syntax_UserData
	.type	Mp4_Syntax_UserData, %function
Mp4_Syntax_UserData:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	cmp	r2, #6
	mov	r3, #0
	mov	r4, r0
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-52]
	str	r3, [fp, #-48]
	strb	r3, [fp, #-61]
	ble	.L122
	cmp	r2, #4
	ble	.L123
	mov	r5, r0
	ldrb	r0, [r1, #4]!	@ zero_extendqisi2
	mov	ip, #4
	movw	r7, #1030
	mvn	r3, r0, asl #24
	strb	r0, [r5, #3]!
	mvn	r3, r3, lsr #24
	mov	r0, r5
	mov	r3, r3, asl #8
	b	.L124
.L129:
	ldrb	lr, [r1, #1]!	@ zero_extendqisi2
	orr	r3, r3, lr
	mov	r3, r3, asl #8
	cmp	r3, #256
	beq	.L183
	cmp	ip, r7
	strb	lr, [r0, #1]!
	beq	.L184
.L124:
	sub	r6, ip, #3
	add	ip, ip, #1
	cmp	ip, r2
	bne	.L129
.L180:
	add	r8, r4, #20480
	cmp	r6, #2
	mov	r3, #1
	mov	r10, r6
	mov	r7, r2, asl #3
	strb	r3, [r8, #1562]
	bhi	.L185
.L161:
	mov	r0, r7
.L179:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L122:
	add	r4, r0, #20480
	add	r7, r2, #1
	strb	r3, [r4, #1562]
	mov	r0, r7, asl #3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L183:
	sub	ip, ip, #2
	cmp	r2, ip
	bge	.L186
.L128:
	add	r7, r2, #1
	mov	r0, r7, asl #3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L185:
	add	r1, r4, #22528
	ldr	r2, [r8, #2092]
	add	r1, r1, #52
	ldr	r0, [r8, #2480]
	bl	Mp4_GetUserDataBuf
	ldr	r9, .L190
	cmp	r0, #0
	str	r0, [r8, #2116]
	beq	.L132
	cmn	r6, #-2147483648
	ldr	r3, [r9, #52]
	sub	r6, r6, #2
	str	r6, [r0, #1048]
	movpl	r2, #0
	ldr	r0, [r8, #2116]
	bmi	.L187
.L133:
	mov	r1, r5
	blx	r3
	ldr	r1, .L190+4
	ldr	r2, [r8, #1580]
	ldr	r3, [r8, #2116]
	ldr	ip, [r1]
	ldrd	r0, [r2, #48]
	add	r3, r3, #1056
	cmp	ip, #0
	strd	r0, [r3]
	beq	.L132
	mov	r3, #1072
	ldr	r2, [r8, #2116]
	mov	r1, #6
	ldr	r0, [r8, #2480]
	blx	ip
.L132:
	cmn	r6, #-2147483646
	movpl	r6, #0
	bmi	.L188
.L134:
	add	r6, r4, r6
	mov	r2, #0
	sub	r0, fp, #61
	sub	r3, fp, #56
	strb	r2, [r6, #3]
	sub	r2, fp, #60
	str	r0, [sp]
	mov	r0, r5
	ldr	ip, [r9, #76]
	ldr	r1, .L190+8
	blx	ip
	ldr	r2, .L190
	cmp	r0, #1
	ble	.L189
.L135:
	ldr	r3, [fp, #-56]
	cmp	r0, #3
	ldr	r2, [fp, #-60]
	str	r3, [r8, #1540]
	movne	r3, #0
	ldreqsb	r3, [fp, #-61]
	str	r2, [r8, #1536]
	subeq	r3, r3, #112
	clzeq	r3, r3
	moveq	r3, r3, lsr #5
	str	r3, [r8, #1544]
.L136:
	ldr	r3, [r9, #76]
	sub	r2, fp, #56
	ldr	r1, .L190+12
	mov	r0, r5
	blx	r3
	ldr	r4, .L190
	cmp	r0, #1
	beq	.L138
	sub	r3, fp, #56
	sub	r6, fp, #48
	str	r3, [sp, #4]
	sub	r2, fp, #60
	ldr	ip, [r4, #76]
	sub	r3, fp, #52
	str	r6, [sp]
	mov	r0, r5
	ldr	r1, .L190+16
	blx	ip
	cmp	r0, #4
	beq	.L138
	str	r6, [sp]
	sub	r3, fp, #52
	ldr	ip, [r4, #76]
	sub	r2, fp, #60
	ldr	r1, .L190+20
	mov	r0, r5
	blx	ip
	add	r0, r0, #1
	cmp	r0, #1
	ble	.L139
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-60]
	ldr	r1, [fp, #-48]
	mov	r2, r2, asl #8
	add	r3, r2, r3, lsl #16
	add	r3, r3, r1
	str	r3, [fp, #-56]
.L139:
	cmp	r0, #4
	beq	.L138
	ldr	r3, [r9, #80]
	mov	r2, #7
	ldr	r1, .L190+24
	mov	r0, r5
	blx	r3
	cmp	r0, #0
	movweq	r3, #4600
	streq	r3, [r8, #1552]
.L150:
	sub	r2, fp, #56
	ldr	r3, [r9, #76]
	ldr	r1, .L190+28
	mov	r0, r5
	blx	r3
	mov	r3, #0
	ldr	r1, [r8, #1536]
	strb	r3, [r8, #801]
	strb	r3, [r8, #802]
	strb	r3, [r8, #799]
	strb	r3, [r8, #798]
	strb	r3, [r8, #800]
	cmp	r0, #1
	ldreq	r2, [fp, #-56]
	ldrne	r2, [r8, #1548]
	streq	r2, [r8, #1548]
	cmp	r2, #0
	beq	.L143
	cmp	r1, r3
	strne	r3, [r8, #1536]
	strne	r3, [r8, #1540]
	cmp	r2, #1
	mvnne	r0, #0
	movne	r1, #0
	beq	.L182
.L153:
	movw	r3, #502
	cmp	r1, r3
	bls	.L145
.L152:
	mov	r3, #1
	strb	r3, [r8, #802]
.L145:
	sub	r2, r2, #1
	cmp	r2, #11
	bhi	.L146
.L154:
	mov	r3, #1
	strb	r3, [r8, #799]
.L146:
	ldr	r3, [r8, #1552]
	movw	r2, #4668
	sub	r3, r3, #1
	cmp	r3, r2
	movls	r3, #1
	strlsb	r3, [r8, #799]
	movw	r3, #498
	cmp	r0, r3
	movls	r3, #1
	strlsb	r3, [r8, #799]
	cmp	r1, #500
	bne	.L161
	ldr	r2, [r8, #1540]
	movw	r3, #413
	mov	r0, r7
	cmp	r2, r3
	moveq	r3, #1
	streqb	r3, [r8, #800]
	b	.L179
.L184:
	add	r4, r4, #20480
	mov	r3, #0
	movw	r0, #8224
	strb	r3, [r4, #1562]
	b	.L179
.L138:
	ldr	r3, [fp, #-56]
	str	r3, [r8, #1552]
	b	.L150
.L187:
	cmp	r10, #1024
	movlt	r2, r10
	movge	r2, #1024
	b	.L133
.L143:
	cmp	r1, #500
	subcs	r0, r1, #1
	movcs	r3, #1
	strcsb	r3, [r8, #801]
	bcs	.L153
	movw	r3, #502
	cmp	r1, r3
	sub	r0, r1, #1
	bhi	.L152
	b	.L146
.L188:
	add	r6, r6, #2
	cmp	r6, #1024
	movge	r6, #1024
	b	.L134
.L123:
	cmp	r2, #3
	ble	.L128
	add	r4, r4, #20480
	mov	r3, #1
	mov	r0, #32
	strb	r3, [r4, #1562]
	b	.L179
.L186:
	mov	r2, ip
	b	.L180
.L189:
	sub	r3, fp, #61
	ldr	r1, .L190+32
	str	r3, [sp]
	mov	r0, r5
	ldr	ip, [r2, #76]
	sub	r3, fp, #56
	sub	r2, fp, #60
	blx	ip
	cmp	r0, #1
	ble	.L136
	b	.L135
.L182:
	strb	r2, [r8, #801]
	mvn	r0, #0
	mov	r1, #0
	b	.L154
.L191:
	.align	2
.L190:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC1
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC2
	UNWIND(.fnend)
	.size	Mp4_Syntax_UserData, .-Mp4_Syntax_UserData
	.align	2
	.global	Mp4_Syntax_Vo
	.type	Mp4_Syntax_Vo, %function
Mp4_Syntax_Vo:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #32)
	sub	sp, sp, #32
	add	r4, r0, #20480
	mov	r5, r0
	sub	r0, fp, #60
	mov	r7, r2
	mov	r6, #1
	bl	BsInit
	mov	r3, #0
	mov	r1, #32
	strb	r3, [r4, #804]
	strb	r3, [r4, #806]
	sub	r0, fp, #60
	strb	r3, [r4, #807]
	add	r5, r5, #16384
	strb	r3, [r4, #808]
	strb	r3, [r4, #809]
	strb	r3, [r4, #810]
	strb	r3, [r4, #811]
	str	r3, [r4, #816]
	str	r3, [r4, #820]
	str	r3, [r4, #824]
	strb	r6, [r4, #805]
	bl	BsSkip
	mov	r1, r6
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #3592]
	cmp	r0, #0
	streqb	r6, [r5, #3593]
	bne	.L207
.L194:
	mov	r1, #4
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r5, #3595]
	cmp	r6, #1
	beq	.L208
	mov	r6, #0
	sub	r0, fp, #60
	strb	r6, [r4, #1557]
	bl	BsPos
	ldr	r3, .L210
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r5, r0
	beq	.L198
	ldr	r0, [r4, #2480]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
	mov	r0, r5
.L198:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L207:
	mov	r1, #4
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #3
	strb	r0, [r5, #3593]
	sub	r0, fp, #60
	bl	BsGet
	strb	r0, [r5, #3594]
	b	.L194
.L208:
	mov	r1, r6
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #3596]
	cmp	r0, #0
	bne	.L209
.L197:
	mov	r3, #1
	sub	r0, fp, #60
	strb	r3, [r4, #1557]
	bl	BsPos
	cmp	r0, r7, asl #3
	bgt	.L199
	ldrb	r3, [r5, #3592]	@ zero_extendqisi2
	strb	r3, [r4, #804]
	ldrb	r3, [r5, #3593]	@ zero_extendqisi2
	strb	r3, [r4, #805]
	ldrb	r3, [r5, #3594]	@ zero_extendqisi2
	strb	r3, [r4, #806]
	ldrb	r3, [r5, #3595]	@ zero_extendqisi2
	strb	r3, [r4, #807]
	ldrb	r3, [r5, #3596]	@ zero_extendqisi2
	strb	r3, [r4, #808]
	ldrb	r3, [r5, #3597]	@ zero_extendqisi2
	strb	r3, [r4, #809]
	ldrb	r3, [r5, #3598]	@ zero_extendqisi2
	strb	r3, [r4, #810]
	ldrb	r3, [r5, #3599]	@ zero_extendqisi2
	strb	r3, [r4, #811]
	ldr	r3, [r5, #3604]
	str	r3, [r4, #816]
	ldr	r3, [r5, #3608]
	str	r3, [r4, #820]
	ldr	r3, [r5, #3612]
	str	r3, [r4, #824]
.L199:
	sub	r0, fp, #60
	bl	BsPos
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L209:
	mov	r1, #3
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, r6
	strb	r0, [r5, #3597]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, r6
	strb	r0, [r5, #3598]
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #3599]
	cmp	r0, #0
	beq	.L197
	mov	r1, #8
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	str	r0, [r5, #3604]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	str	r0, [r5, #3608]
	sub	r0, fp, #60
	bl	BsGet
	str	r0, [r5, #3612]
	b	.L197
.L211:
	.align	2
.L210:
	.word	g_event_report
	UNWIND(.fnend)
	.size	Mp4_Syntax_Vo, .-Mp4_Syntax_Vo
	.align	2
	.global	Mp4_Syntax_Vol
	.type	Mp4_Syntax_Vol, %function
Mp4_Syntax_Vol:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	add	r5, r0, #20480
	mov	r6, r0
	ldr	r3, .L374
	ldr	r0, [r5, #2480]
	add	r7, r6, #19968
	ldr	r8, .L374+4
	add	r7, r7, #32
	add	r4, r6, #16384
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #1272]
	moveq	r9, #8192
	ldrne	r9, [r3, #1276]
	streq	r9, [fp, #-80]
	strne	r0, [fp, #-80]
	sub	r0, fp, #76
	bl	BsInit
	ldr	r3, [r8, #48]
	mov	r2, #604
	mov	r1, #0
	mov	r0, r7
	strb	r1, [r5, #789]
	blx	r3
	mov	r1, #32
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #8
	strb	r0, [r4, #3620]
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #3644]
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3616]
	cmp	r0, #0
	ldreqb	r3, [r5, #805]	@ zero_extendqisi2
	streqb	r3, [r4, #3617]
	bne	.L363
.L215:
	mov	r1, #4
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3619]
	cmp	r0, #15
	beq	.L364
.L216:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3621]
	cmp	r0, #0
	moveq	r3, #1
	streqb	r3, [r4, #3622]
	bne	.L365
.L221:
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r10, r0
	strb	r10, [r4, #3624]
	cmp	r10, #0
	subne	r0, fp, #76
	movne	r6, #0
	beq	.L366
.L357:
	strb	r6, [r5, #1559]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L292
	ldr	r0, [r5, #2480]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
	mov	r0, r4
.L327:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L366:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	mov	r1, #16
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4, #3688]
	beq	.L229
	subs	r0, r0, #1
	beq	.L229
	mov	r2, r10
	mov	r3, r0
.L231:
	add	r2, r2, #1
	movs	r3, r3, lsr #1
	uxth	r2, r2
	bne	.L231
	sxth	r2, r2
	cmp	r2, #0
	ble	.L229
.L233:
	add	r3, r3, #1
	movs	r0, r0, lsr #1
	uxth	r3, r3
	bne	.L233
	sxth	r3, r3
.L329:
	str	r3, [r4, #3684]
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3625]
	cmp	r0, #0
	bne	.L367
.L236:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	mov	r1, #13
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #3696]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	mov	r1, #13
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #3700]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	ldr	r3, [r4, #3696]
	cmp	r3, #31
	bls	.L240
	ldr	r2, [r4, #3700]
	cmp	r2, #31
	bls	.L240
	mul	r3, r3, r2
	ldr	r2, [fp, #-80]
	mul	r9, r9, r2
	cmp	r3, r9
	bls	.L241
.L240:
	mov	r6, #0
	sub	r0, fp, #76
	strb	r6, [r5, #1559]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L292
.L336:
	ldr	r0, [r5, #2480]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #103
	blx	ip
	mov	r0, r4
	b	.L327
.L342:
	str	r3, [fp, #-80]
	mov	r0, #1
	ldr	r2, [r8, #68]
	ldr	r1, .L374+12
	blx	r2
.L344:
	ldr	r3, [fp, #-80]
	sub	r0, fp, #76
	strb	r3, [r5, #1559]
	bl	BsPos
	ldr	r2, .L374+8
	ldr	ip, [r2]
	cmp	ip, #0
	mov	r4, r0
	beq	.L292
	ldr	r3, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r5, #2480]
	mov	r2, r3
	blx	ip
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L365:
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r10, r0
	strb	r10, [r4, #3622]
	cmp	r10, #1
	beq	.L218
	mov	r3, #0
	sub	r0, fp, #76
	strb	r3, [r5, #1559]
	bl	BsPos
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L363:
	mov	r1, #4
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #3617]
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #3618]
	b	.L215
.L218:
	mov	r1, r10
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	strb	r0, [r4, #3623]
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4, #3656]
	beq	.L221
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3660]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3664]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3668]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #11
	str	r0, [r4, #3672]
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3676]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3680]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	str	r3, [fp, #-84]
	bne	.L221
	ldr	r3, [r8, #68]
	mov	r0, r10
	ldr	r1, .L374+12
	blx	r3
	ldr	r6, [fp, #-84]
	sub	r0, fp, #76
	strb	r6, [r5, #1559]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r4, r0
	bne	.L336
.L292:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L364:
	mov	r1, #8
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #8
	str	r0, [r4, #3648]
	sub	r0, fp, #76
	bl	BsGet
	str	r0, [r4, #3652]
	b	.L216
.L229:
	mov	r3, #1
	b	.L329
.L359:
	str	r3, [fp, #-80]
	mov	r0, r10
	ldr	ip, [r8, #68]
	ldr	r1, .L374+12
	blx	ip
	b	.L344
.L367:
	ldr	r1, [r4, #3684]
	sub	r0, fp, #76
	bl	BsGet
	str	r0, [r4, #3692]
	b	.L236
.L241:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #3626]
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r3, r0
	strb	r3, [r4, #3627]
	cmp	r3, #0
	bne	.L243
	ldr	r2, .L374+8
	strb	r3, [r5, #1559]
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L243
	mov	r2, r3
	mov	r1, #102
	ldr	r0, [r5, #2480]
	blx	ip
.L243:
	ldrb	r3, [r4, #3617]	@ zero_extendqisi2
	sub	r0, fp, #76
	cmp	r3, #1
	moveq	r1, #1
	movne	r1, #2
	bl	BsGet
	uxtb	r9, r0
	strb	r9, [r4, #3628]
	cmp	r9, #0
	beq	.L246
	ldr	r3, .L374+16
	ldr	r10, [r3]
	ands	r10, r10, #8
	beq	.L368
	sub	r3, r9, #1
	cmp	r3, #1
	bls	.L369
	mov	r6, #0
	sub	r0, fp, #76
	strb	r6, [r5, #1559]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L253
	ldr	r0, [r5, #2480]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
.L253:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+20
	blx	r3
	mov	r0, r4
	b	.L327
.L370:
	mov	r1, #6
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r9
	strb	r0, [r4, #3640]
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #3641]
	sub	r0, fp, #76
	bl	BsGet
	ldrb	r3, [r4, #3628]	@ zero_extendqisi2
	cmp	r3, #2
	strb	r0, [r4, #3642]
	bne	.L361
.L246:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3629]
	cmp	r0, #0
	beq	.L254
	mov	r6, #0
	sub	r0, fp, #76
	strb	r6, [r5, #1559]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L255
	ldr	r0, [r5, #2480]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
.L255:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+24
	blx	r3
	mov	r0, r4
	b	.L327
.L369:
	cmp	r9, #2
	beq	.L370
.L361:
	mov	r6, #0
	sub	r0, fp, #76
	strb	r6, [r5, #1559]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L252
	ldr	r0, [r5, #2480]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
.L252:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+28
	blx	r3
	mov	r0, r4
	b	.L327
.L254:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3630]
	cmp	r0, #0
	bne	.L371
.L257:
	ldrb	r3, [r4, #3617]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r3, #0
	streqb	r3, [r4, #3633]
	beq	.L264
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #3633]
.L264:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r9, r0
	strb	r9, [r4, #3634]
	cmp	r9, #0
	bne	.L265
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+32
.L353:
	blx	r3
	strb	r9, [r5, #1559]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r4, r0
	beq	.L292
	ldr	r0, [r5, #2480]
	mov	r3, r9
	mov	r2, r9
	mov	r1, #102
	blx	ip
	mov	r0, r4
	b	.L327
.L368:
	strb	r10, [r5, #1559]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r4, r0
	beq	.L248
	ldr	r0, [r5, #2480]
	mov	r3, r10
	mov	r2, r10
	mov	r1, #102
	blx	ip
.L248:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+36
	blx	r3
	mov	r0, r4
	b	.L327
.L371:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3631]
	cmp	r0, #0
	bne	.L372
	add	r3, r6, #19968
	add	r1, r6, #20224
	ldr	r2, .L374+40
	add	r3, r3, #116
	add	r1, r1, #116
.L260:
	ldr	r0, [r2, #4]!
	str	r0, [r3, #4]!
	cmp	r3, r1
	bne	.L260
.L259:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3632]
	cmp	r0, #0
	bne	.L272
	add	r3, r6, #20224
	add	r1, r6, #20480
	ldr	r2, .L374+44
	add	r3, r3, #116
	add	r1, r1, #116
.L261:
	ldr	r0, [r2, #4]!
	str	r0, [r3, #4]!
	cmp	r3, r1
	bne	.L261
	b	.L257
.L265:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #3635]
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r10, r0
	strb	r10, [r4, #3636]
	cmp	r10, #0
	bne	.L373
	ldrb	r3, [r4, #3617]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L267
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r9, r0
	strb	r9, [r4, #3637]
	cmp	r9, #0
	beq	.L268
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+48
	blx	r3
	strb	r10, [r5, #1559]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r4, r0
	beq	.L292
	ldr	r0, [r5, #2480]
	mov	r3, r10
	mov	r2, r10
	mov	r1, #102
	blx	ip
	mov	r0, r4
	b	.L327
.L372:
	add	r1, r6, #19968
	sub	r0, fp, #76
	add	r1, r1, #120
	bl	Bs_Get_Matrix
	b	.L259
.L267:
	strb	r10, [r4, #3637]
	strb	r10, [r4, #3638]
.L269:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3639]
	cmp	r0, #0
	beq	.L270
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+52
.L355:
	blx	r3
	mov	r6, #0
	sub	r0, fp, #76
	b	.L357
.L373:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+56
	b	.L355
.L270:
	add	r0, r6, #20480
	mov	r3, #1
	ldr	ip, [r8, #52]
	mov	r1, r7
	mov	r2, #604
	strb	r3, [r5, #1559]
	strb	r3, [r5, #789]
	add	r0, r0, #124
	strb	r3, [r5, #1560]
	blx	ip
	sub	r0, fp, #76
	bl	BsPos
	b	.L327
.L268:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3638]
	cmp	r0, #0
	beq	.L269
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L374+60
	b	.L353
.L272:
	add	r1, r6, #20224
	sub	r0, fp, #76
	add	r1, r1, #120
	bl	Bs_Get_Matrix
	b	.L257
.L375:
	.align	2
.L374:
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC8
	.word	g_VdmCharacter
	.word	.LC11
	.word	.LC12
	.word	.LC10
	.word	.LC13
	.word	.LC9
	.word	.LANCHOR1-4
	.word	.LANCHOR1+252
	.word	.LC15
	.word	.LC17
	.word	.LC14
	.word	.LC16
	UNWIND(.fnend)
	.size	Mp4_Syntax_Vol, .-Mp4_Syntax_Vol
	.align	2
	.global	Mp4_Syntax_Vos
	.type	Mp4_Syntax_Vos, %function
Mp4_Syntax_Vos:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #32)
	sub	sp, sp, #32
	add	r5, r0, #20480
	add	r4, r0, #16384
	sub	r0, fp, #52
	bl	BsInit
	mov	r3, #0
	mov	r1, #32
	str	r3, [r5, #812]
	sub	r0, fp, #52
	bl	BsSkip
	sub	r0, fp, #52
	mov	r1, #8
	bl	BsGet
	sub	r3, r0, #1
	sub	r2, r0, #240
	cmp	r2, #5
	cmphi	r3, #2
	str	r0, [r4, #3600]
	movls	r3, #1
	movhi	r3, #0
	cmp	r0, #8
	orreq	r3, r3, #1
	cmp	r3, #0
	strne	r0, [r5, #812]
	movne	r3, #1
	strneb	r3, [r5, #1556]
	bne	.L378
	cmp	r0, #0
	beq	.L383
	ldr	r2, .L384
	strb	r3, [r5, #1556]
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L378
	ldr	r0, [r5, #2480]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L378:
	sub	r0, fp, #52
	bl	BsPos
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L383:
	ldr	r3, .L384+4
	mov	r2, r0
	ldr	r1, .L384+8
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r3, #1
	strb	r3, [r5, #1556]
	ldr	r3, [r4, #3600]
	str	r3, [r5, #812]
	b	.L378
.L385:
	.align	2
.L384:
	.word	g_event_report
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC18
	UNWIND(.fnend)
	.size	Mp4_Syntax_Vos, .-Mp4_Syntax_Vos
	.align	2
	.global	Frame_Init
	.type	Frame_Init, %function
Frame_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	add	r4, r0, #20480
	mov	r6, r1
	mov	r7, r2
	ldr	lr, [r4, #1580]
	ldr	r5, [lr, #776]
	ldr	r3, [lr, #28]
	cmp	r5, #0
	bne	.L387
	cmp	r3, #25
	beq	.L417
.L411:
	mov	r5, #2
	mov	r9, #5
	mov	r10, #3
.L388:
	ldr	r3, [lr, #852]
	cmp	r3, #1
	beq	.L399
	ldr	r3, .L420
	ldr	ip, [r3, #48]
	cmp	ip, #0
	beq	.L401
	ldr	r8, .L420+4
	mov	r2, #0
	ldr	r1, [lr, #24]
	ldr	r0, [lr, #20]
	mov	lr, #2
	str	r8, [sp, #16]
	ldr	r3, [r4, #2480]
	str	r2, [sp, #8]
	mov	r2, r6
	str	lr, [sp]
	str	r3, [sp, #12]
	mov	r3, #40
	str	r3, [sp, #4]
	mov	r3, r7
	blx	ip
	cmp	r0, #0
	bne	.L401
	ldr	r2, [r8, #552]
	ldr	r3, [r8, #292]
	cmp	r2, #1
	ble	.L403
	cmp	r3, #2
	ble	.L403
	ldr	r0, [r8, #8]
	str	r0, [r4, #1600]
	bl	MEM_Phy2Vir
	str	r0, [r4, #1588]
.L399:
	ldr	r8, .L420+8
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r2, [r4, #1580]
	str	r7, [fp, #-48]
	mov	r3, #0
	str	r6, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r7, [fp, #-64]
	cmp	r2, #24
	str	r6, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L405
	uxtb	r5, r5
	strb	r10, [fp, #-72]
	strb	r9, [fp, #-70]
.L408:
	sub	r1, fp, #76
	ldr	r0, [r4, #2480]
	strb	r5, [fp, #-69]
	mov	r6, #0
	strb	r6, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r5, r0, #0
	bne	.L406
	ldr	r1, [r4, #1580]
	sub	r3, fp, #80
	ldr	r0, [r4, #2480]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_PartitionFsMemory
	cmp	r0, r6
	moveq	r3, #1
	streq	r3, [r4, #2484]
	bne	.L418
.L414:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L387:
	cmp	r3, #25
	bne	.L411
	ldr	r1, [lr, #736]
	ldr	r2, [lr, #740]
	cmp	r6, r1
	bhi	.L395
	cmp	r7, r2
	bhi	.L395
	ldr	r3, [lr, #764]
	ldr	r5, [lr, #760]
	cmp	r3, #0
	add	r10, r5, #2
	moveq	r5, #1
	bne	.L419
	ldr	r9, [lr, #784]
	b	.L388
.L405:
	strb	r3, [fp, #-72]
	mov	r5, #1
	strb	r9, [fp, #-70]
	b	.L408
.L417:
	ldr	r1, [lr, #736]
	cmp	r1, r6
	ldrcc	r2, [lr, #740]
	bcc	.L395
	ldr	r3, [lr, #740]
	cmp	r3, r7
	mov	r2, r3
	bcc	.L395
	ldr	r2, [r4, #2484]
	cmp	r2, #1
	beq	.L414
	ldr	r2, [lr, #764]
	ldr	r5, [lr, #760]
	cmp	r2, #0
	add	r10, r5, #2
	moveq	r5, #1
	beq	.L393
	add	r5, r5, #1
	cmp	r5, #17
	movge	r5, #17
.L393:
	mov	r7, r3
	mov	r6, r1
	ldr	r9, [lr, #784]
	b	.L388
.L419:
	add	r5, r5, #1
	ldr	r9, [lr, #784]
	cmp	r5, #17
	movge	r5, #17
	b	.L388
.L403:
	ldr	ip, .L420+8
	mov	r0, #0
	ldr	r1, .L420+12
	mvn	r5, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L414
.L401:
	ldr	r3, .L420+8
	mov	r0, #0
	ldr	r1, .L420+16
	mvn	r5, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L414
.L406:
	ldr	r3, [r8, #68]
	mov	r0, r6
	ldr	r1, .L420+20
	mvn	r5, #0
	blx	r3
	b	.L414
.L395:
	ldr	ip, .L420+8
	mov	r3, r7
	str	r2, [sp, #4]
	mov	r0, #0
	str	r1, [sp]
	mov	r2, r6
	ldr	ip, [ip, #68]
	mvn	r5, #0
	ldr	r1, .L420+24
	blx	ip
	b	.L414
.L418:
	mov	r0, r5
	ldr	r3, [r8, #68]
	ldr	r1, .L420+28
	blx	r3
	str	r5, [r4, #2484]
	mvn	r5, #0
	b	.L414
.L421:
	.align	2
.L420:
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR2
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC21
	.word	.LC20
	.word	.LC23
	.word	.LC19
	.word	.LC22
	UNWIND(.fnend)
	.size	Frame_Init, .-Frame_Init
	.align	2
	.global	Mp4_RecordVFMWState
	.type	Mp4_RecordVFMWState, %function
Mp4_RecordVFMWState:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Mp4_RecordVFMWState, .-Mp4_RecordVFMWState
	.global	__aeabi_uidiv
	.align	2
	.global	Mp4_RecordFrameAtr
	.type	Mp4_RecordFrameAtr, %function
Mp4_RecordFrameAtr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r8, r0, #20480
	mov	r4, r1
	ldrb	r3, [r8, #831]	@ zero_extendqisi2
	cmp	r3, #15
	beq	.L441
	sub	r2, r3, #2
	cmp	r2, #1
	movls	r9, #1
	bls	.L425
	sub	r2, r3, #4
	cmp	r2, #1
	movls	r9, #2
	bls	.L425
	cmp	r3, #1
	moveq	r9, #5
	movne	r9, #0
.L425:
	mov	r1, r4
	ldr	r0, [r8, #2480]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L423
	ldr	r3, [r8, #1496]
	mov	r1, r9
	str	r3, [r4, #164]
	ldr	r3, [r8, #1492]
	str	r3, [r4, #168]
	bl	SetAspectRatio
	ldr	r3, [r8, #1580]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L442
.L431:
	ldr	r3, [r8, #1496]
	mov	r5, #0
	mvn	r6, #0
	mvn	r7, #0
	ldr	ip, .L443
	mov	r0, #29
	str	r3, [r4, #172]
	ldr	r3, [r8, #1492]
	ldr	r1, .L443+4
	str	r3, [r4, #176]
	ldr	r3, [r8, #1496]
	mov	r3, r3, lsr #1
	str	r3, [r4, #180]
	ldr	r3, [r8, #1492]
	mov	r3, r3, lsr #1
	str	r3, [r4, #184]
	ldr	r3, [r8, #2060]
	str	r3, [r4, #208]
	ldr	r3, [r8, #2056]
	str	r3, [r4, #204]
	ldrb	r3, [r8, #809]	@ zero_extendqisi2
	ldrb	r2, [r8, #1437]	@ zero_extendqisi2
	ldrb	lr, [r8, #1432]	@ zero_extendqisi2
	ldrb	r10, [r8, #838]	@ zero_extendqisi2
	mov	r3, r3, asl #5
	and	lr, lr, #3
	orr	lr, lr, r2, asl #12
	and	r10, r10, #1
	uxtb	r2, r3
	orr	lr, lr, #3072
	orr	r3, lr, r2
	orr	r3, r3, r10, asl #8
	orr	r3, r3, r9, asl #14
	str	r3, [r4, #160]
	ldrb	r3, [r8, #1432]	@ zero_extendqisi2
	and	r3, r3, #3
	str	r3, [r4, #152]
	ldrb	r3, [r8, #1432]	@ zero_extendqisi2
	str	r5, [r4, #148]
	and	r3, r3, #3
	str	r3, [r4, #156]
	ldr	r3, [r8, #2100]
	str	r3, [r4, #212]
	ldr	r3, [r8, #2104]
	str	r3, [r4, #216]
	ldr	r3, [r8, #2108]
	str	r3, [r4, #220]
	ldr	r3, [r8, #2112]
	str	r3, [r4, #224]
	ldr	r3, [r8, #1580]
	ldrd	r2, [r3, #48]
	strd	r2, [r4, #8]
	ldr	r3, [r8, #1580]
	ldrd	r2, [r3, #64]
	strd	r2, [r4, #16]
	ldr	r3, [r8, #1580]
	ldrd	r2, [r3, #80]
	strd	r2, [r4, #24]
	ldr	r3, [r8, #1580]
	ldr	r3, [r3, #96]
	str	r3, [r4, #68]
	ldr	r3, [r8, #1580]
	ldr	r3, [r3, #100]
	str	r3, [r4, #72]
	ldr	r3, [r8, #1580]
	ldr	r3, [r3, #104]
	str	r3, [r4, #76]
	ldr	r3, [r8, #1580]
	ldr	r3, [r3, #120]
	str	r3, [r4, #80]
	ldr	r3, [r8, #1580]
	strd	r6, [r3, #48]
	ldrd	r6, [r4, #16]
	ldrd	r2, [r4, #8]
	ldr	ip, [ip, #68]
	strd	r6, [sp]
	blx	ip
	mov	r3, #8
	mov	r2, #2
	strb	r5, [r4, #372]
	str	r2, [r4, #384]
	mov	r2, #1
	strb	r5, [r4, #379]
	str	r3, [r4, #388]
	str	r3, [r4, #396]
	str	r3, [r4, #392]
	strb	r2, [r4, #377]
.L423:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L442:
	ldr	r3, [r4, #168]
	ldr	r1, [r4, #192]
	add	r3, r3, #15
	ldr	r0, [r4, #104]
	ldr	ip, [r4, #32]
	mov	r2, r3, lsr #4
	mov	r3, r1, asl #4
	add	lr, r0, r1
	str	r0, [r4, #88]
	mul	r3, r2, r3
	str	r0, [r4, #120]
	str	lr, [r4, #96]
	str	ip, [r4, #48]
	add	r0, r3, r0
	add	r3, ip, r3
	add	r1, r0, r1
	str	r3, [r4, #36]
	str	r0, [r4, #108]
	str	r0, [r4, #92]
	str	r1, [r4, #100]
	bl	MEM_Phy2Vir
	ldr	r3, [r4, #92]
	str	r3, [r4, #124]
	str	r0, [r4, #52]
	b	.L431
.L441:
	ldr	r2, [r8, #860]
	ldr	r1, [r8, #864]
	cmp	r2, #0
	cmpne	r1, #0
	moveq	r9, #0
	beq	.L425
	ldr	r3, [r8, #912]
	mul	r1, r3, r1
	cmp	r1, #0
	beq	.L433
	ldr	r0, [r8, #908]
	mov	r2, r2, asl #10
	mul	r0, r0, r2
	bl	__aeabi_uidiv
	movw	r3, #1364
	movw	r2, #1819
	cmp	r0, r3
	rsble	r3, r0, #1360
	subgt	r3, r0, #1360
	addle	r3, r3, #5
	subgt	r3, r3, #5
	cmp	r0, r2
	rsble	r0, r0, #1808
	subgt	r0, r0, #1808
	addle	r0, r0, #12
	subgt	r0, r0, #12
	cmp	r3, r0
	movlt	r9, #1
	movge	r9, #2
	b	.L425
.L433:
	mov	r9, r1
	b	.L425
.L444:
	.align	2
.L443:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC24
	UNWIND(.fnend)
	.size	Mp4_RecordFrameAtr, .-Mp4_RecordFrameAtr
	.align	2
	.global	Mp4_Frame_Atr
	.type	Mp4_Frame_Atr, %function
Mp4_Frame_Atr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #20480
	mov	r5, r0
	mov	r1, #1
	add	r6, r5, #53248
	ldrb	r3, [r4, #1432]	@ zero_extendqisi2
	cmp	r3, #4
	strb	r3, [r0, #2]
	ldreq	r2, .L455
	moveq	r3, #300
	ldr	r0, [r4, #2480]
	mlaeq	r3, r3, r0, r2
	ldreq	r2, [r3, #12]
	addeq	r2, r2, #1
	streq	r2, [r3, #12]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r6, #3552]
	blt	.L448
	ldr	r3, [r4, #2484]
	cmp	r3, #0
	beq	.L448
	mov	r1, r0
	mov	r0, r5
	bl	Mp4_RecordFrameAtr
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #2064]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L448:
	ldr	r3, .L455+4
	mov	r0, #0
	ldr	r1, .L455+8
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r6, #3544]
	ldr	r0, [r4, #2480]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L450
	ldr	r1, [r7, #212]
	mov	r8, #0
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r7, #216]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r7, #220]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r7, #224]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r8, [r7, #212]
	str	r8, [r7, #216]
	mov	r2, #1
	str	r8, [r7, #220]
	str	r8, [r7, #224]
	ldr	r1, [r6, #3544]
	ldr	r0, [r4, #2480]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r6, #3536]
	ldr	r0, [r4, #2480]
	bl	FSP_ClearLogicFs
	str	r8, [r6, #3536]
	str	r8, [r6, #3544]
	ldr	r0, [r4, #2480]
	bl	FSP_GetNullLogicFs
	str	r0, [r6, #3544]
	str	r0, [r6, #3536]
.L450:
	add	r1, r5, #22528
	mov	r3, #0
	add	r1, r1, #80
	str	r3, [r4, #2488]
	ldr	r0, [r4, #2480]
	bl	FSP_ClearNotInVoQueue
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L456:
	.align	2
.L455:
	.word	g_VfmwChanStat
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC25
	UNWIND(.fnend)
	.size	Mp4_Frame_Atr, .-Mp4_Frame_Atr
	.align	2
	.global	Mp4_Set_DecPar
	.type	Mp4_Set_DecPar, %function
Mp4_Set_DecPar:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #76)
	sub	sp, sp, #76
	ldr	r8, .L485
	mov	r6, r2
	mov	r4, r0
	mov	r2, #48
	sub	r0, fp, #92
	ldr	r3, [r8, #48]
	mov	r1, #0
	blx	r3
	ldrb	r3, [r6, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L482
	ldr	r3, [r6, #88]
	add	r5, r4, #20480
	mov	r1, #0
	bic	r3, r3, #3
	str	r3, [r4, #1208]
	ldr	r3, [r5, #1568]
	and	r2, r3, #3
	str	r3, [r4, #1220]
	ldr	r3, [r5, #1564]
	str	r3, [r4, #1056]
	ldr	r3, [r5, #1576]
	add	r2, r3, r2, lsl #3
	str	r2, [r4, #1228]
	ldr	r2, [r5, #1572]
	rsb	r3, r3, r2, lsl #3
	str	r3, [r4, #1236]
	ldr	r3, [r6, #88]
	str	r3, [r4, #1224]
	ldr	r3, [r6, #80]
	str	r3, [r4, #1060]
	ldr	r3, [r6, #88]
	and	r3, r3, #3
	mov	r3, r3, asl #3
	str	r3, [r4, #1232]
	ldr	r3, [r6, #92]
	str	r1, [r4, #1216]
	mov	r3, r3, asl #3
	str	r3, [r4, #1240]
.L459:
	ldr	r2, [r5, #1496]
	add	r7, r4, #1040
	add	r6, r4, #12288
	add	r3, r2, #15
	mov	r3, r3, lsr #4
	strh	r3, [r7, #164]	@ movhi
	ldr	r1, [r5, #1492]
	add	r3, r1, #15
	mov	r3, r3, lsr #4
	strh	r3, [r7, #166]	@ movhi
	ldrb	r3, [r5, #1432]	@ zero_extendqisi2
	strb	r3, [r4, #1040]
	ldrb	r0, [r5, #842]	@ zero_extendqisi2
	strb	r0, [r4, #1041]
	ldrb	r0, [r5, #797]	@ zero_extendqisi2
	strb	r0, [r4, #1042]
	ldrb	r0, [r5, #1439]	@ zero_extendqisi2
	strb	r0, [r4, #1043]
	ldrb	r0, [r5, #1444]	@ zero_extendqisi2
	strb	r0, [r4, #1044]
	ldrb	r0, [r5, #1443]	@ zero_extendqisi2
	strb	r0, [r4, #1045]
	ldr	r0, [r5, #896]
	strb	r0, [r4, #1046]
	ldrb	r0, [r5, #1440]	@ zero_extendqisi2
	strb	r0, [r4, #1047]
	ldrb	r0, [r5, #1441]	@ zero_extendqisi2
	strb	r0, [r4, #1048]
	ldrb	r0, [r5, #1436]	@ zero_extendqisi2
	strb	r0, [r4, #1049]
	ldrb	r0, [r5, #847]	@ zero_extendqisi2
	strb	r0, [r4, #1050]
	ldrb	r0, [r5, #845]	@ zero_extendqisi2
	strb	r0, [r4, #1051]
	ldrb	r0, [r5, #1434]	@ zero_extendqisi2
	strb	r0, [r4, #1052]
	ldrb	r0, [r5, #1438]	@ zero_extendqisi2
	strb	r0, [r4, #1053]
	ldrb	r0, [r5, #1437]	@ zero_extendqisi2
	strb	r0, [r4, #1054]
	ldrb	r0, [r5, #838]	@ zero_extendqisi2
	strb	r0, [r4, #1055]
	ldr	r0, [r5, #1528]
	str	r0, [r4, #1244]
	ldr	r0, [r5, #1524]
	str	r0, [r4, #1248]
	ldrb	r0, [r5, #800]	@ zero_extendqisi2
	strb	r0, [r4, #1194]
	ldrb	r0, [r5, #799]	@ zero_extendqisi2
	strb	r0, [r4, #1193]
	ldrb	r0, [r5, #798]	@ zero_extendqisi2
	strb	r0, [r4, #1192]
	ldrb	r0, [r5, #801]	@ zero_extendqisi2
	strb	r0, [r4, #1195]
	ldrb	r0, [r5, #802]	@ zero_extendqisi2
	strb	r0, [r4, #1196]
	ldrb	r0, [r5, #803]	@ zero_extendqisi2
	strb	r0, [r4, #1197]
	str	r1, [r6, #3360]
	str	r2, [r6, #3364]
	ldrb	r2, [r5, #840]	@ zero_extendqisi2
	cmp	r2, #2
	cmpeq	r3, #3
	str	r2, [r6, #3368]
	moveq	r3, #1
	movne	r3, #0
	beq	.L483
	ldrb	r2, [r5, #853]	@ zero_extendqisi2
	str	r3, [r6, #3372]
	str	r3, [r6, #3380]
	str	r2, [r6, #3376]
	str	r3, [r6, #3384]
	str	r3, [r6, #3388]
	str	r3, [r6, #3392]
	str	r3, [r6, #3396]
	str	r3, [r6, #3400]
	str	r3, [r6, #3404]
	str	r3, [r6, #3408]
.L461:
	add	r10, r4, #53248
	ldr	r0, [r5, #2480]
	ldr	r1, [r10, #3552]
	bl	FSP_GetLogicFs
	ldr	r1, [r10, #3536]
	mov	r9, r0
	ldr	r0, [r5, #2480]
	bl	FSP_GetLogicFs
	ldr	r1, [r10, #3544]
	str	r0, [fp, #-96]
	ldr	r0, [r5, #2480]
	bl	FSP_GetLogicFs
	ldr	r2, [fp, #-96]
	cmp	r9, #0
	cmpne	r2, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L484
	ldr	r1, [r9, #520]
	ldr	ip, [r9, #528]
	cmp	r1, #0
	ldr	lr, [r9, #524]
	beq	.L465
	cmp	ip, #0
	beq	.L465
	cmp	lr, #0
	beq	.L465
	ldr	r3, [r9, #532]
	add	r1, r4, #1056
	add	r1, r1, #7
	str	r3, [r4, #1268]
	ldr	r3, [r0, #532]
	str	r3, [r4, #1272]
	ldr	r3, [r0, #520]
	cmp	r3, #0
	ldreq	r3, [r9, #520]
	ldr	r3, [r3, #12]
	str	r3, [r4, #1264]
	ldr	r3, [r2, #520]
	cmp	r3, #0
	ldreq	r3, [r9, #520]
	ldr	r3, [r3, #12]
	str	r3, [r4, #1260]
	ldr	r3, [r9, #520]
	ldr	r3, [r3, #12]
	str	r3, [r4, #1252]
	ldr	r3, [r0, #528]
	add	r0, r4, #21248
	add	r0, r0, #144
	cmp	r3, #0
	ldreq	r3, [r9, #528]
	ldr	r3, [r3, #12]
	str	r3, [r6, #3332]
	ldr	r3, [r2, #528]
	add	r2, r4, #21504
	add	r4, r4, #1120
	add	r2, r2, #144
	cmp	r3, #0
	add	r4, r4, #7
	ldreq	r3, [r9, #528]
	ldr	r3, [r3, #12]
	str	r3, [r6, #3336]
	ldr	r3, [r9, #528]
	ldr	ip, [r3, #12]
	mov	r3, #0
	str	ip, [r6, #3340]
	ldr	ip, [r9, #524]
	ldr	ip, [ip, #12]
	str	ip, [r6, #3344]
	ldr	ip, [r9, #524]
	ldr	ip, [ip, #20]
	str	ip, [r6, #3356]
.L476:
	ldr	ip, [r0, #4]!
	add	r3, r3, #1
	cmp	r3, #64
	strb	ip, [r1, #1]!
	ldr	ip, [r2, #4]!
	strb	ip, [r4, #1]!
	bne	.L476
	ldrh	r1, [r7, #164]
	ldrh	r3, [r7, #166]
	ldr	r2, [r5, #1580]
	mul	r3, r3, r1
	str	r3, [r5, #1464]
	ldr	r3, [r2, #676]
	str	r3, [r6, #3412]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L484:
	ldr	ip, [r8, #68]
	movw	r3, #3496
	ldr	r2, .L485+4
	mov	r0, #0
	ldr	r1, .L485+8
	blx	ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L482:
	add	r5, r4, #20480
	mov	r0, #0
	ldr	r2, [r5, #1568]
	bic	r1, r2, #3
	str	r1, [r4, #1220]
	str	r2, [r4, #1208]
	and	r2, r2, #3
	ldr	r1, [r5, #1564]
	str	r1, [r4, #1056]
	ldr	r1, [r5, #1576]
	add	r2, r1, r2, lsl #3
	str	r2, [r4, #1228]
	ldr	r2, [r5, #1572]
	str	r3, [r4, #1216]
	str	r0, [r4, #1224]
	rsb	r1, r1, r2, lsl #3
	str	r0, [r4, #1232]
	str	r1, [r4, #1236]
	str	r0, [r4, #1240]
	b	.L459
.L465:
	ldr	r4, [r10, #3544]
	mov	r0, #0
	ldr	r3, [r10, #3552]
	ldr	r2, [r10, #3536]
	stmib	sp, {r1, ip, lr}
	str	r4, [sp]
	ldr	ip, [r8, #68]
	ldr	r1, .L485+12
	blx	ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L483:
	add	r0, r4, #21248
	sub	r1, fp, #92
	add	r0, r0, #20
	bl	MP4_GetGMCparam
	sub	r0, fp, #68
	ldrb	r10, [r5, #853]	@ zero_extendqisi2
	ldmia	r0, {r0, r1, r2, r3}
	ldr	r9, [fp, #-92]
	ldr	lr, [fp, #-76]
	ldr	ip, [fp, #-72]
	str	r3, [r6, #3400]
	ldr	r3, [fp, #-52]
	str	r10, [r6, #3376]
	str	r9, [r6, #3372]
	str	r3, [r6, #3404]
	ldr	r3, [fp, #-48]
	str	lr, [r6, #3380]
	str	ip, [r6, #3384]
	str	r0, [r6, #3388]
	str	r1, [r6, #3392]
	str	r2, [r6, #3396]
	str	r3, [r6, #3408]
	b	.L461
.L486:
	.align	2
.L485:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+1280
	.word	.LC26
	.word	.LC27
	UNWIND(.fnend)
	.size	Mp4_Set_DecPar, .-Mp4_Set_DecPar
	.align	2
	.global	Mp4_Get_DecMode
	.type	Mp4_Get_DecMode, %function
Mp4_Get_DecMode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r0, r0, #20480
	ldr	r3, [r0, #1612]
	cmp	r3, #2
	ldr	r3, [r0, #1580]
	beq	.L499
.L489:
	ldr	r3, [r3, #8]
	cmp	r3, #2
	beq	.L492
	cmp	r3, #1
	beq	.L498
	cmp	r3, #4
	movne	r3, #0
.L498:
	str	r3, [r0, #1612]
	ldmfd	sp, {fp, sp, pc}
.L499:
	ldr	r2, [r3, #8]
	cmp	r2, #2
	movne	r2, #1
	strne	r2, [r0, #2120]
	bne	.L489
.L492:
	mov	r3, #2
	str	r3, [r0, #1612]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Mp4_Get_DecMode, .-Mp4_Get_DecMode
	.align	2
	.global	GetTrailingOneLen
	.type	GetTrailingOneLen, %function
GetTrailingOneLen:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r1, r0, r1
	ldrb	r3, [r1, #-1]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L503
	mov	r0, #0
.L502:
	mov	r3, r3, lsr #1
	add	r0, r0, #1
	tst	r3, #1
	bne	.L502
	cmp	r0, #0
	addne	r0, r0, #1
	ldmfd	sp, {fp, sp, pc}
.L503:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	GetTrailingOneLen, .-GetTrailingOneLen
	.align	2
	.global	MPEG4DEC_Init
	.type	MPEG4DEC_Init, %function
MPEG4DEC_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 776
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #780)
	sub	sp, sp, #780
	ldr	r7, .L517
	mov	r4, r0
	mov	r6, r1
	mov	r0, #2
	ldr	r1, .L517+4
	ldr	r3, [r7, #68]
	blx	r3
	cmp	r6, #0
	cmpne	r4, #0
	beq	.L513
	ldr	r3, [r6, #688]
	cmp	r3, #1
	beq	.L514
.L511:
	add	r5, r4, #20480
	ldr	r3, [r7, #48]
	movw	r2, #56824
	mov	r1, #0
	ldr	r8, [r5, #2472]
	mov	r0, r4
	blx	r3
	add	r0, r4, #22528
	add	r0, r0, #80
	mov	r7, #0
	str	r8, [r5, #2472]
	bl	ResetVoQueue
	str	r6, [r5, #1580]
	mov	r0, r4
	str	r7, [r6, #892]
	bl	VCTRL_GetChanIDByCtx
	ldr	r8, .L517
	cmn	r0, #1
	str	r0, [r5, #2480]
	beq	.L515
	mov	r0, r4
	bl	Global_Data_Init
	subs	r7, r0, #0
	bne	.L513
	str	r7, [r5, #2096]
	mov	r2, #1
	ldr	r3, [r8, #68]
	mov	r0, #2
	str	r2, [r5, #2068]
	ldr	r1, .L517+8
	blx	r3
	ldr	r5, [r6, #688]
	cmp	r5, #1
	beq	.L516
.L510:
	mov	r0, r7
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L516:
	add	r0, r4, #21248
	ldr	r3, [r8, #52]
	sub	r1, fp, #804
	mov	r2, #768
	add	r0, r0, #20
	blx	r3
	add	r0, r4, #19968
	ldr	r3, [r8, #52]
	sub	r1, fp, #788
	mov	r2, #24
	add	r0, r0, #8
	blx	r3
	add	r0, r4, #19968
	ldr	r3, [r8, #52]
	sub	r1, fp, #764
	mov	r2, #604
	add	r0, r0, #32
	blx	r3
	add	r0, r4, #22016
	ldr	r3, [r8, #52]
	mov	r2, #7
	sub	r1, fp, #812
	add	r0, r0, #20
	blx	r3
	add	r4, r4, #53248
	mov	r0, r7
	strb	r5, [r4, #3561]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L514:
	add	r1, r4, #21248
	ldr	r3, [r7, #52]
	add	r1, r1, #20
	mov	r2, #768
	sub	r0, fp, #804
	blx	r3
	add	r1, r4, #19968
	ldr	r3, [r7, #52]
	add	r1, r1, #8
	mov	r2, #24
	sub	r0, fp, #788
	blx	r3
	add	r1, r4, #19968
	ldr	r3, [r7, #52]
	add	r1, r1, #32
	mov	r2, #604
	sub	r0, fp, #764
	blx	r3
	add	r1, r4, #22016
	add	r1, r1, #20
	ldr	r3, [r7, #52]
	mov	r2, #7
	sub	r0, fp, #812
	blx	r3
	b	.L511
.L513:
	mvn	r7, #0
	mov	r0, r7
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L515:
	ldr	r3, [r8, #68]
	mov	r0, r7
	ldr	r1, .L517+12
	mvn	r7, #19
	blx	r3
	b	.L510
.L518:
	.align	2
.L517:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC30
	.word	.LC29
	UNWIND(.fnend)
	.size	MPEG4DEC_Init, .-MPEG4DEC_Init
	.align	2
	.global	MPEG4DEC_Destroy
	.type	MPEG4DEC_Destroy, %function
MPEG4DEC_Destroy:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L520
	mov	r4, r0
	add	r4, r4, #20480
	ldr	r1, .L520+4
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	mov	r3, #0
	str	r3, [r4, #2068]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L521:
	.align	2
.L520:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC31
	UNWIND(.fnend)
	.size	MPEG4DEC_Destroy, .-MPEG4DEC_Destroy
	.align	2
	.global	MPEG4DEC_RecycleImage
	.type	MPEG4DEC_RecycleImage, %function
MPEG4DEC_RecycleImage:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r5, r1
	beq	.L524
	add	r4, r0, #20480
	mov	r2, #0
	ldr	r0, [r4, #2480]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #2480]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L525
	ldr	r1, [r5, #212]
	mov	r6, #0
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r6, [r5, #212]
	ldr	r1, [r5, #216]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r6, [r5, #216]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L525:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L524:
	mvn	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG4DEC_RecycleImage, .-MPEG4DEC_RecycleImage
	.align	2
	.global	MP4_SetImgFormat
	.type	MP4_SetImgFormat, %function
MP4_SetImgFormat:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #53248
	add	r4, r0, #20480
	ldr	r1, [r3, #3552]
	ldr	r0, [r4, #2480]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, [r4, #1496]
	cmp	r3, #0
	beq	.L528
	ldr	r2, [r4, #1492]
	cmp	r2, #0
	beq	.L528
	ldr	r1, [r0, #164]
	ldr	r2, [r0, #168]
	cmp	r1, r3
	movcs	r1, r3
	str	r1, [r0, #172]
	ldr	r3, [r4, #1492]
	cmp	r2, r3
	movcs	r2, r3
	str	r2, [r0, #176]
	b	.L529
.L528:
	ldr	r2, [r0, #164]
	ldr	r3, [r0, #168]
	str	r2, [r0, #172]
	str	r3, [r0, #176]
.L529:
	mov	r3, #2
	mov	r2, #0
	str	r3, [r0, #384]
	mov	r1, #1
	mov	r3, #8
	strb	r2, [r0, #372]
	strb	r2, [r0, #379]
	str	r3, [r0, #388]
	str	r3, [r0, #396]
	str	r3, [r0, #392]
	strb	r1, [r0, #377]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP4_SetImgFormat, .-MP4_SetImgFormat
	.align	2
	.global	MPEG4DEC_GetRemainImg
	.type	MPEG4DEC_GetRemainImg, %function
MPEG4DEC_GetRemainImg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r6, r0, #20480
	mov	r4, r0
	ldr	r3, [r6, #1580]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L554
	add	r5, r0, #53248
	ldr	r0, [r6, #2480]
	ldr	r1, [r5, #3544]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L554
	ldr	r1, [r5, #3544]
	ldr	r0, [r6, #2480]
	bl	FSP_GetDisplay
	cmp	r0, #2
	mov	r8, r0
	beq	.L553
.L554:
	add	r5, r4, #22528
	add	r5, r5, #80
.L544:
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	GetVoLastImageID
.L553:
	mov	r0, r4
	add	r5, r4, #22528
	bl	MP4_SetImgFormat
	add	r5, r5, #80
	mov	r3, #1
	ldr	r1, [r7, #196]
	mov	r2, r3
	str	r3, [r7, #244]
	ldr	r0, [r6, #2480]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #2480]
	mov	r1, r8
	str	r7, [sp]
	mov	r3, r5
	mov	r2, r4
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L541
	mov	r0, r4
	ldr	r1, [r7, #196]
	bl	MPEG4DEC_RecycleImage
	ldr	r1, [r7, #212]
	ldr	r0, [r6, #2480]
	mov	r4, #0
	bl	FreeUsdByDec
	str	r4, [r7, #212]
	ldr	r1, [r7, #216]
	ldr	r0, [r6, #2480]
	bl	FreeUsdByDec
	str	r4, [r7, #216]
	b	.L544
.L541:
	ldr	r3, [r7, #200]
	cmp	r3, #100
	bhi	.L544
	ldr	r3, [r7, #164]
	cmp	r3, #31
	bls	.L544
	ldr	r3, [r7, #168]
	cmp	r3, #31
	bls	.L544
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG4DEC_GetRemainImg, .-MPEG4DEC_GetRemainImg
	.align	2
	.global	MPEG4DEC_GetImageBuffer
	.type	MPEG4DEC_GetImageBuffer, %function
MPEG4DEC_GetImageBuffer:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #20480
	mov	r5, r0
	ldr	r0, [r4, #2480]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #2480]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L561
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L561:
	add	r1, r5, #22528
	ldr	r0, [r4, #2480]
	add	r1, r1, #80
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG4DEC_GetImageBuffer, .-MPEG4DEC_GetImageBuffer
	.align	2
	.global	MP4_GetPacketState
	.type	MP4_GetPacketState, %function
MP4_GetPacketState:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r0, r0, #16384
	ldr	r3, [r0, #3588]
	cmp	r3, #1
	mvnne	r0, #0
	moveq	r0, #3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP4_GetPacketState, .-MP4_GetPacketState
	.align	2
	.global	MP4_ClearCurPacket
	.type	MP4_ClearCurPacket, %function
MP4_ClearCurPacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r1
	ldr	r1, [r1, #16]
	mov	r5, r0
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L566
	ldrb	r3, [r4, #85]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L567
.L566:
	ldr	r3, .L568
	mov	r0, r4
	mov	r2, #160
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L567:
	mov	r0, r5
	ldr	r1, [r4, #96]
	bl	SM_ReleaseStreamSeg
	b	.L566
.L569:
	.align	2
.L568:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MP4_ClearCurPacket, .-MP4_ClearCurPacket
	.align	2
	.global	MP4_ClearCurPic
	.type	MP4_ClearCurPic, %function
MP4_ClearCurPic:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r8, r0, #16384
	add	r3, r0, #20480
	ldr	r6, [r8, #3580]
	ldr	r7, [r3, #2480]
	cmp	r6, #0
	addgt	r5, r0, #15808
	movgt	r4, #0
	addgt	r5, r5, #56
	ble	.L573
.L572:
	add	r4, r4, #1
	ldr	r1, [r5, #4]!
	mov	r0, r7
	bl	SM_ReleaseStreamSeg
	cmp	r6, r4
	bne	.L572
.L573:
	mov	r3, #0
	str	r3, [r8, #3580]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP4_ClearCurPic, .-MP4_ClearCurPic
	.align	2
	.global	Vdm_Mem_Move
	.type	Vdm_Mem_Move, %function
Vdm_Mem_Move:
	UNWIND(.fnstart)
	@ args = 12, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	ip, .L587
	mov	r4, r0
	mov	r7, r1
	mov	r0, #2
	ldr	r1, .L587+4
	ldr	ip, [ip, #68]
	mov	r10, r3
	str	r2, [fp, #-48]
	add	r8, r4, #20480
	ldr	r9, [fp, #4]
	ldr	r6, [fp, #8]
	ldr	r5, [fp, #12]
	blx	ip
	ldr	r2, [fp, #-48]
	add	r1, r4, #12288
	add	r6, r6, #15
	add	r5, r5, #15
	movw	r3, #1206
	add	r0, r4, #1040
	str	r2, [r4, #1252]
	movw	r2, #1204
	str	r10, [r1, #3344]
	mov	r6, r6, lsr #4
	ldr	ip, [r7, #532]
	mov	r5, r5, lsr #4
	str	r9, [r4, #1260]
	str	ip, [r4, #1268]
	strh	r6, [r4, r2]	@ movhi
	strh	r5, [r4, r3]	@ movhi
	ldr	r3, [r7, #524]
	ldr	r3, [r3, #20]
	str	r3, [r1, #3356]
	ldr	r3, [r8, #1580]
	ldr	r2, [r3, #676]
	str	r2, [r1, #3412]
	str	r0, [r4, #1036]
	ldr	r2, [r3, #884]
	cmp	r2, #1
	beq	.L585
	cmp	r2, #2
	beq	.L586
.L578:
	mov	r3, #1
	strb	r3, [r4, #1198]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L585:
	cmp	r0, #0
	beq	.L578
	add	r1, r4, #53248
	mov	r3, #0
	mov	r2, r3
	str	r3, [r4, #1036]
	ldr	r0, [r8, #2480]
	ldr	r1, [r1, #3552]
	bl	FSP_SetDisplay
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	MP4_ClearCurPic
.L586:
	ldr	r3, [r3, #888]
	cmp	r0, #0
	cmpne	r3, #0
	ble	.L578
	add	r1, r4, #53248
	mov	r3, #0
	mov	r2, r3
	str	r3, [r4, #1036]
	ldr	r0, [r8, #2480]
	ldr	r1, [r1, #3552]
	bl	FSP_SetDisplay
	ldr	r2, [r8, #1580]
	mov	r0, r4
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	MP4_ClearCurPic
.L588:
	.align	2
.L587:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC32
	UNWIND(.fnend)
	.size	Vdm_Mem_Move, .-Vdm_Mem_Move
	.align	2
	.global	MP4_GetScdUpMsg
	.type	MP4_GetScdUpMsg, %function
MP4_GetScdUpMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #20480
	add	r3, r0, #15808
	add	r4, r0, #56576
	add	r3, r3, #56
	ldr	r2, [r2, #1580]
	add	r4, r4, #240
	ldr	r0, [r2, #852]
	cmp	r0, #1
	beq	.L591
.L590:
	ldr	r6, .L592
	add	r5, r1, #72
	mov	r0, r3
	mov	r2, #4
	mov	r1, r5
	ldr	r3, [r6, #52]
	blx	r3
	ldr	r3, [r6, #52]
	mov	r1, r5
	mov	r0, r4
	mov	r2, #4
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L591:
	ldr	r2, [r2, #892]
	cmp	r2, #1
	bne	.L590
	ldr	ip, .L592
	mov	r0, r3
	mov	r1, r4
	mov	r2, #4
	ldr	r3, [ip, #52]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L593:
	.align	2
.L592:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MP4_GetScdUpMsg, .-MP4_GetScdUpMsg
	.align	2
	.global	Mp4_Check_Slice_NotSH
	.type	Mp4_Check_Slice_NotSH, %function
Mp4_Check_Slice_NotSH:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #16384
	add	r3, r0, #20480
	ldrb	r1, [r2, #3635]	@ zero_extendqisi2
	ldrb	r2, [r3, #728]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L604
	add	r0, r0, #12288
	ldrb	r1, [r0, #3576]	@ zero_extendqisi2
	tst	r1, #120
	bne	.L604
	movw	r1, #3576
	ldrh	r0, [r0, r1]
	ubfx	r0, r0, #7, #4
	add	r0, r0, #17
	cmp	r2, #3
	ldrls	pc, [pc, r2, asl #2]
	b	.L596
.L598:
	.word	.L605
	.word	.L599
	.word	.L600
	.word	.L599
.L604:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L605:
	mov	r3, #17
.L597:
	subs	r0, r3, r0
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L600:
	ldrb	r2, [r3, #736]	@ zero_extendqisi2
	ldrb	r3, [r3, #737]	@ zero_extendqisi2
	cmp	r3, r2
	movcc	r3, r2
	cmp	r3, #1
	addgt	r3, r3, #16
	movle	r3, #18
	b	.L597
.L599:
	ldrb	r3, [r3, #736]	@ zero_extendqisi2
	add	r3, r3, #16
	b	.L597
.L596:
	ldr	r3, .L608
	mov	r0, #1
	ldr	r1, .L608+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L609:
	.align	2
.L608:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC33
	UNWIND(.fnend)
	.size	Mp4_Check_Slice_NotSH, .-Mp4_Check_Slice_NotSH
	.align	2
	.global	Mp4_Combine_SliceData
	.type	Mp4_Combine_SliceData, %function
Mp4_Combine_SliceData:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #16384
	add	r3, r0, #12288
	add	r0, r0, #1280
	ldr	ip, [r2, #3584]
	ldr	r2, [r3, #3328]
	tst	ip, #2
	beq	.L611
	ldrb	lr, [r3, #3421]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L627
	cmp	lr, #0
	bne	.L619
	ldrb	ip, [r3, #3501]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L619
	mov	ip, r2, asl #5
	sub	r2, ip, r2, asl #2
	sub	r2, r2, #28
	add	r2, r0, r2
	ldr	r0, [r2, #12]
	cmp	r0, #0
	bne	.L626
	ldr	r0, [r2, #20]
	cmp	r0, #0
	bne	.L626
	ldr	ip, [r2, #8]
	add	r1, ip, r1, lsl #3
	str	r1, [r2, #8]
	ldr	r1, [r3, #3504]
	str	r1, [r2, #20]
	ldr	r3, [r3, #3508]
	mov	r3, r3, asl #3
	str	r3, [r2, #12]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L627:
	sub	r4, r2, #1
	movw	ip, #510
	cmp	r4, ip
	bhi	.L628
	mov	ip, r2, asl #5
	sub	r2, ip, r2, asl #2
	sub	r2, r2, #28
	add	r2, r0, r2
	ldr	r0, [r2, #12]
	cmp	r0, #0
	beq	.L615
	ldr	ip, [r2, #20]
	cmp	ip, #0
	beq	.L615
	ldr	r3, [r3, #3424]
	cmp	ip, r3
	bhi	.L629
	add	r1, r0, r1, lsl #3
	mov	r0, #0
	str	r1, [r2, #12]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L615:
	ldr	ip, [r3, #3424]
	ldr	r0, [r2, #16]
	cmp	ip, r0
	movls	r1, r1, asl #3
	strls	r1, [r2, #12]
	ldrls	r3, [r3, #3424]
	movhi	r0, #0
	ldrhi	r3, [r2, #8]
	movls	r0, #0
	addhi	r1, r3, r1, lsl #3
	strls	r3, [r2, #20]
	strhi	r1, [r2, #8]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L619:
	ldr	r3, .L630
	movw	r2, #4859
.L625:
	ldr	r1, .L630+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
.L626:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L611:
	ldr	r3, .L630
	movw	r2, #4865
	b	.L625
.L628:
	ldr	r3, .L630
	mov	r0, lr
	ldr	r1, .L630+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L629:
	ldr	r3, .L630
	mov	r0, lr
	movw	r2, #4835
	ldr	r1, .L630+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L631:
	.align	2
.L630:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC35
	.word	.LC34
	UNWIND(.fnend)
	.size	Mp4_Combine_SliceData, .-Mp4_Combine_SliceData
	.align	2
	.global	MP4_Check_StreamType
	.type	MP4_Check_StreamType, %function
MP4_Check_StreamType:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #12288
	add	r0, r0, #20480
	ldrb	r3, [r2, #3576]	@ zero_extendqisi2
	tst	r3, #1
	bne	.L633
	mov	r3, #2
	strb	r3, [r0, #797]
	ldr	r3, [r1]
	mov	r3, r3, lsr #8
	sub	r3, r3, #128
	cmp	r3, #3
	ldrlsb	r3, [r2, #3576]	@ zero_extendqisi2
	orrls	r3, r3, #2
	strlsb	r3, [r2, #3576]
.L634:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L633:
	mov	r2, #196
	mov	r3, #1
	str	r2, [r1]
	strb	r3, [r0, #797]
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP4_Check_StreamType, .-MP4_Check_StreamType
	.align	2
	.global	MP4_Clean_HeadPacket
	.type	MP4_Clean_HeadPacket, %function
MP4_Clean_HeadPacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	mov	r8, r0
	ldr	r6, [r3, #2036]
	ldr	r7, [r3, #2480]
	cmp	r6, #64
	movge	r6, #64
	cmp	r6, #0
	addgt	r5, r0, #22016
	movgt	r4, #0
	addgt	r5, r5, #240
	ble	.L638
.L637:
	add	r4, r4, #1
	ldr	r1, [r5, #4]!
	mov	r0, r7
	bl	SM_ReleaseStreamSeg
	cmp	r4, r6
	bne	.L637
.L638:
	ldr	r3, .L640
	add	r0, r8, #22016
	add	r0, r0, #80
	mov	r2, #440
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L641:
	.align	2
.L640:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MP4_Clean_HeadPacket, .-MP4_Clean_HeadPacket
	.align	2
	.global	MPEG4DEC_VDMPostProc
	.type	MPEG4DEC_VDMPostProc, %function
MPEG4DEC_VDMPostProc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r7, .L693
	mov	r5, r0
	movw	r2, #14664
	mov	r8, r1
	add	r0, r0, #1040
	ldr	r3, [r7, #48]
	mov	r1, #0
	blx	r3
	add	r4, r5, #20480
	mov	r0, r5
	add	r6, r5, #53248
	bl	MP4_ClearCurPic
	mov	r0, r5
	bl	MP4_Clean_HeadPacket
	str	r8, [r4, #2064]
	ldr	r0, [r4, #2480]
	ldr	r1, [r6, #3552]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r6, #3544]
	mov	r8, r0
	ldr	r0, [r4, #2480]
	bl	FSP_GetFsImagePtr
	cmp	r8, #0
	mov	r9, r0
	beq	.L688
	ldr	r3, [r7, #68]
	mov	r0, #11
	ldr	r2, [r4, #2072]
	ldr	r1, .L693+4
	blx	r3
	ldr	r3, [r7, #68]
	ldrb	r2, [r4, #1432]	@ zero_extendqisi2
	mov	r0, #11
	ldr	r1, .L693+8
	blx	r3
	ldr	r3, [r4, #2064]
	add	r1, r5, #22528
	add	r1, r1, #52
	str	r3, [r8, #200]
	ldr	r0, [r4, #2480]
	bl	Mp4_ClearUsrData
	ldrb	r3, [r4, #1432]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L645
	ldr	r2, [r4, #1580]
	ldr	r3, [r4, #2064]
	ldr	r1, [r2]
	cmp	r3, r1
	bhi	.L689
.L645:
	mov	r3, #0
	mov	r0, r5
	str	r3, [r4, #2120]
	bl	MP4_SetImgFormat
	ldr	r3, [r4, #1580]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L648
	ldrb	lr, [r4, #1432]	@ zero_extendqisi2
	cmp	lr, #2
	beq	.L649
	ldr	r3, [r4, #2488]
	cmp	r3, #1
	ble	.L650
	ldr	ip, [r6, #3536]
	ldr	r3, [r6, #3540]
	str	ip, [r6, #3556]
	ldrb	r8, [r5]	@ zero_extendqisi2
	str	r3, [fp, #-56]
.L651:
	cmn	ip, #1
	beq	.L653
	mov	r1, ip
	ldr	r0, [r4, #2480]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L690
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	cmpeq	r8, #4
	beq	.L691
	ldr	r2, [r9, #200]
	cmp	r2, #0
	beq	.L656
	ldr	r3, [r4, #1580]
	ldr	r1, [r3, #4]
	cmp	r2, r1
	bhi	.L657
.L656:
	ldr	r1, [r6, #3556]
	ldr	r0, [r4, #2480]
	bl	FSP_GetLogicFs
	subs	r10, r0, #0
	beq	.L692
	cmp	r8, #2
	beq	.L659
	ldrsb	r3, [r10, #1]
	cmp	r3, #2
	beq	.L659
	ldr	r3, [r7, #68]
	mov	r0, #11
	ldr	r2, [r6, #3556]
	ldr	r1, .L693+12
	blx	r3
.L653:
	ldr	r3, [r4, #2072]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #2072]
.L686:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L648:
	ldr	ip, [r6, #3552]
	str	ip, [r6, #3556]
	ldr	r3, [r4, #1524]
	ldrb	r8, [r4, #1432]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	b	.L651
.L649:
	ldr	ip, [r6, #3552]
	cmp	r9, #0
	str	ip, [r6, #3556]
	ldr	r3, [r4, #1524]
	str	r3, [fp, #-56]
	beq	.L664
	ldrd	r2, [r9, #8]
	ldrd	r0, [r8, #8]
	cmp	r2, r0
	sbcs	r10, r3, r1
	bge	.L664
	strd	r2, [r8, #8]
	mov	r8, lr
	strd	r0, [r9, #8]
	ldr	ip, [r6, #3556]
	b	.L651
.L657:
	ldr	r3, .L693+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L662
	str	r2, [fp, #-52]
	mov	r3, #8
	str	r1, [fp, #-48]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r4, #2480]
	blx	ip
	ldr	r3, [r4, #1580]
	ldr	r2, [r9, #200]
	ldr	r1, [r3, #4]
.L662:
	mov	r3, r1
	ldr	ip, [r7, #68]
	ldr	r1, .L693+20
	mov	r0, #1
	blx	ip
	ldr	r1, [r9, #212]
	ldr	r0, [r4, #2480]
	mov	r5, #0
	bl	FreeUsdByDec
	str	r5, [r9, #212]
	ldr	r1, [r9, #216]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r5, [r9, #216]
	mov	r2, r5
	ldr	r1, [r6, #3556]
	ldr	r0, [r4, #2480]
	bl	FSP_SetDisplay
	b	.L653
.L659:
	mov	r2, #1
	ldr	r1, [r6, #3556]
	ldr	r0, [r4, #2480]
	bl	FSP_SetDisplay
	add	r3, r5, #22528
	ldr	r0, [r4, #2480]
	add	r3, r3, #80
	str	r9, [sp]
	mov	r2, r5
	mov	r1, #2
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L653
	ldr	r1, [r9, #212]
	mov	r5, #0
	ldr	r0, [r4, #2480]
.L687:
	bl	FreeUsdByDec
	str	r5, [r9, #212]
	ldr	r1, [r9, #216]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r5, [r9, #216]
	mov	r2, r5
	ldr	r0, [r4, #2480]
	ldr	r1, [r6, #3556]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L686
.L650:
	mvn	r3, #0
	str	r3, [r6, #3556]
	b	.L653
.L664:
	mov	r8, lr
	b	.L651
.L689:
	ldr	r1, .L693+16
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L646
	str	r3, [fp, #-52]
	mov	r1, #104
	ldr	lr, [r2]
	mov	r3, #8
	sub	r2, fp, #52
	ldr	r0, [r4, #2480]
	str	lr, [fp, #-48]
	blx	ip
.L646:
	mov	r8, #1
	ldr	r0, [r4, #2480]
	str	r8, [r4, #2120]
	ldr	r1, [r6, #3544]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L647
	ldr	r1, [r5, #212]
	mov	r7, #0
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r5, #216]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r5, #220]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r5, #224]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r7, [r5, #212]
	str	r7, [r5, #216]
	mov	r2, r8
	str	r7, [r5, #220]
	str	r7, [r5, #224]
	ldr	r1, [r6, #3544]
	ldr	r0, [r4, #2480]
	bl	FSP_ClearLogicFs
	mov	r2, r8
	ldr	r1, [r6, #3536]
	ldr	r0, [r4, #2480]
	bl	FSP_ClearLogicFs
	str	r7, [r6, #3536]
	str	r7, [r6, #3544]
	ldr	r0, [r4, #2480]
	bl	FSP_GetNullLogicFs
	str	r0, [r6, #3544]
	str	r0, [r6, #3536]
.L647:
	mov	r3, #0
	mvn	r0, #0
	str	r3, [r4, #2488]
	b	.L686
.L691:
	ldr	r1, [r4, #2480]
	mov	r2, #300
	ldr	r3, .L693+24
	mov	r5, #0
	mov	r0, r1
	mla	r3, r2, r1, r3
	ldr	r2, [r3, #16]
	add	r2, r2, #1
	str	r2, [r3, #16]
	ldr	r1, [r9, #212]
	b	.L687
.L688:
	mov	r3, r0
	ldr	ip, [r7, #68]
	mov	r2, r8
	mov	r0, r8
	ldr	r1, .L693+28
	blx	ip
	mvn	r0, #0
	b	.L686
.L690:
	ldr	r3, [r7, #68]
	movw	r2, #4566
	ldr	r1, .L693+32
	blx	r3
	mvn	r0, #0
	b	.L686
.L692:
	ldr	r1, [r9, #212]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r10, [r9, #212]
	ldr	r1, [r9, #216]
	ldr	r0, [r4, #2480]
	bl	FreeUsdByDec
	str	r10, [r9, #216]
	mov	r2, r10
	ldr	r0, [r4, #2480]
	ldr	r1, [r6, #3556]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L686
.L694:
	.align	2
.L693:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC37
	.word	.LC38
	.word	.LC40
	.word	g_event_report
	.word	.LC41
	.word	g_VfmwChanStat
	.word	.LC36
	.word	.LC39
	UNWIND(.fnend)
	.size	MPEG4DEC_VDMPostProc, .-MPEG4DEC_VDMPostProc
	.align	2
	.global	MP4_Record_SlcSegID
	.type	MP4_Record_SlcSegID, %function
MP4_Record_SlcSegID:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #16384
	movw	r6, #1022
	mov	r4, r0
	ldr	r2, [r5, #3580]
	cmp	r2, r6
	bhi	.L698
	add	r1, r0, #12288
	add	r4, r0, r2, lsl #2
	add	r4, r4, #12288
	add	r3, r2, #1
	ldr	r0, [r1, #3432]
	str	r0, [r4, #3580]
	str	r3, [r5, #3580]
	ldrb	r3, [r1, #3421]	@ zero_extendqisi2
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldrb	r3, [r1, #3501]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r1, #3512]
	addeq	r2, r2, #2
	streq	r3, [r4, #3584]
	streq	r2, [r5, #3580]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L698:
	bl	MP4_Clean_HeadPacket
	mov	r0, r4
	bl	MP4_ClearCurPic
	add	r3, r4, #20480
	add	r1, r4, #15680
	ldr	r0, [r3, #2480]
	add	r1, r1, #24
	bl	MP4_ClearCurPacket
	ldr	ip, .L699
	mov	r3, r6
	ldr	r2, [r5, #3580]
	ldr	r1, .L699+4
	mov	r0, #0
	ldr	ip, [ip, #68]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	ip
.L700:
	.align	2
.L699:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC42
	UNWIND(.fnend)
	.size	MP4_Record_SlcSegID, .-MP4_Record_SlcSegID
	.align	2
	.global	Mp4_Syntax_ShortHeader
	.type	Mp4_Syntax_ShortHeader, %function
Mp4_Syntax_ShortHeader:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r7, r0, #12288
	mov	r4, r2
	mov	r6, r0
	ldr	r2, [r7, #3328]
	mov	r5, r1
	ldr	r8, .L742
	cmp	r2, #0
	bne	.L737
.L702:
	mov	r1, r5
	mov	r2, r4
	sub	r0, fp, #76
	add	r5, r6, #20480
	bl	BsInit
	add	r0, r6, #20992
	ldr	r3, [r8, #48]
	mov	r2, #60
	mov	r1, #0
	add	r0, r0, #216
	blx	r3
	mov	r1, #22
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #13
	sub	r0, fp, #76
	bl	BsSkip
	sub	r0, fp, #76
	mov	r1, #3
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #738]
	sub	r0, r0, #1
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L703
.L705:
	.word	.L704
	.word	.L706
	.word	.L707
	.word	.L708
.L708:
	mov	r2, #18
	mov	r3, #88
	strb	r2, [r5, #739]
	mov	r2, #704
	strb	r3, [r5, #740]
	mov	r3, #576
	str	r2, [r5, #1496]
	str	r3, [r5, #1492]
.L709:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	ldrb	r3, [r5, #795]	@ zero_extendqisi2
	cmp	r3, #2
	uxtb	r0, r0
	strb	r0, [r5, #728]
	beq	.L711
	adds	r0, r0, #0
	ldr	r3, [r5, #2120]
	movne	r0, #1
	cmp	r3, #1
	movne	r0, #0
	cmp	r0, #0
	bne	.L711
	mov	r1, #4
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #5
	sub	r0, fp, #76
	mov	r10, r4, asl #3
	bl	BsGet
	mov	r1, #1
	mov	r4, #0
	uxtb	r0, r0
	cmp	r0, #0
	strneb	r0, [r5, #735]
	sub	r0, fp, #76
	moveq	r3, #1
	streqb	r3, [r5, #735]
	bl	BsSkip
.L717:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #1
	beq	.L738
	add	r4, r4, #1
	cmp	r4, #15
	bls	.L718
.L736:
	mov	r4, #0
	sub	r0, fp, #76
	strb	r4, [r5, #1561]
	bl	BsPos
	ldr	r3, .L742+4
	ldr	ip, [r3]
	cmp	ip, r4
	mov	r9, r0
	beq	.L724
	ldr	r0, [r5, #2480]
	mov	r3, r4
	mov	r2, r4
	mov	r1, #103
	blx	ip
	mov	r0, r9
.L735:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L707:
	mov	r2, #18
	mov	r3, #22
	strb	r2, [r5, #739]
	mov	r2, #352
	strb	r3, [r5, #740]
	mov	r3, #288
	str	r2, [r5, #1496]
	str	r3, [r5, #1492]
	b	.L709
.L706:
	mov	r2, #9
	mov	r3, #11
	strb	r2, [r5, #739]
	mov	r2, #176
	strb	r3, [r5, #740]
	mov	r3, #144
	str	r2, [r5, #1496]
	str	r3, [r5, #1492]
	b	.L709
.L704:
	mov	r2, #6
	mov	r3, #8
	strb	r2, [r5, #739]
	mov	r2, #128
	strb	r3, [r5, #740]
	mov	r3, #96
	str	r2, [r5, #1496]
	str	r3, [r5, #1492]
	b	.L709
.L738:
	sub	r0, fp, #76
	add	r4, r4, #1
	bl	BsPos
	mov	r1, #8
	cmp	r0, r10
	sub	r0, fp, #76
	bgt	.L739
	bl	BsSkip
	cmp	r4, #15
	bls	.L717
	b	.L736
.L737:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L742+8
	blx	r3
	mov	r3, #0
	str	r3, [r7, #3328]
	b	.L702
.L718:
	add	r4, r6, #16384
	mov	r2, #1
	strb	r2, [r5, #1561]
	sub	r0, fp, #76
	ldr	r3, [r4, #3584]
	orr	r3, r3, #2
	str	r3, [r4, #3584]
	ldr	r3, [r7, #3328]
	add	r3, r3, r2
	str	r3, [r7, #3328]
	strb	r2, [r5, #736]
	bl	BsPos
	mov	r3, r0, asr #31
	mov	r9, r0
	add	r1, r0, #7
	rsb	r10, r0, r10
	mov	r3, r3, lsr #29
	str	r10, [r6, #1288]
	add	r2, r9, r3
	ldr	r0, [r5, #1576]
	and	r2, r2, #7
	cmp	r9, #0
	rsb	r3, r3, r2
	add	r3, r3, r0
	strb	r3, [r6, #1286]
	ldr	r3, [r5, #1568]
	movge	r1, r9
	add	r3, r3, r1, asr #3
	str	r3, [r6, #1296]
	ldrb	r3, [r7, #3421]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L740
.L719:
	mov	r3, #0
	str	r3, [r6, #1292]
	strb	r3, [r6, #1287]
	str	r3, [r6, #1300]
.L720:
	ldrb	r3, [r5, #728]	@ zero_extendqisi2
	strb	r3, [r6, #1284]
	ldrb	r3, [r5, #737]	@ zero_extendqisi2
	strb	r3, [r6, #1282]
	ldrb	r3, [r5, #736]	@ zero_extendqisi2
	strb	r3, [r6, #1281]
	ldrb	r3, [r5, #735]	@ zero_extendqisi2
	strb	r3, [r6, #1285]
	ldrb	r3, [r5, #732]	@ zero_extendqisi2
	strb	r3, [r6, #1283]
	ldr	r2, [r4, #3580]
	cmp	r2, #0
	bne	.L741
.L721:
	mov	r0, r6
	bl	MP4_Record_SlcSegID
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L711:
	sub	r0, fp, #76
	bl	BsPos
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L703:
	mov	r4, #0
	sub	r0, fp, #76
	strb	r4, [r5, #1561]
	bl	BsPos
	ldr	r3, .L742+4
	ldr	ip, [r3]
	cmp	ip, r4
	mov	r9, r0
	beq	.L724
	ldr	r0, [r5, #2480]
	mov	r3, r4
	mov	r2, r4
	mov	r1, #102
	blx	ip
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L740:
	ldrb	r2, [r7, #3501]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L719
	ldr	r2, [r7, #3508]
	strb	r3, [r6, #1287]
	mov	r3, r2, asl #3
	str	r3, [r6, #1292]
	ldr	r3, [r7, #3504]
	str	r3, [r6, #1300]
	b	.L720
.L741:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L742+12
	blx	r3
	mov	r0, r6
	bl	MP4_ClearCurPic
	b	.L721
.L724:
	mov	r0, r9
	b	.L735
.L739:
	bl	BsPos
	b	.L735
.L743:
	.align	2
.L742:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC43
	.word	.LC44
	UNWIND(.fnend)
	.size	Mp4_Syntax_ShortHeader, .-Mp4_Syntax_ShortHeader
	.align	2
	.global	Mp4_Syntax_Vop
	.type	Mp4_Syntax_Vop, %function
Mp4_Syntax_Vop:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #60)
	sub	sp, sp, #60
	add	r7, r0, #12288
	mov	r6, r2
	mov	r5, r0
	ldr	r2, [r7, #3328]
	mov	r10, r1
	cmp	r2, #0
	bne	.L822
	add	r4, r5, #20480
	ldrb	r3, [r4, #789]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L746
.L823:
	mov	r0, #32
	strb	r3, [r4, #1561]
.L747:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L822:
	ldr	ip, .L838
	add	r4, r5, #20480
	ldr	r1, .L838+4
	mov	r0, #1
	ldr	r3, [ip, #68]
	blx	r3
	mov	r3, #0
	str	r3, [r7, #3328]
	ldrb	r3, [r4, #789]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L823
.L746:
	ldr	r9, .L838
	add	r8, r5, #20992
	add	r8, r8, #216
	mov	r1, r10
	sub	r0, fp, #76
	mov	r2, r6
	bl	BsInit
	mov	r2, #60
	ldr	r3, [r9, #48]
	mov	r1, #0
	mov	r0, r8
	blx	r3
	mov	r3, #0
	mov	r1, #32
	strb	r3, [r4, #1561]
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsGet
	ldr	r3, [r9, #68]
	ldr	r1, .L838+8
	uxtb	r2, r0
	strb	r0, [r4, #728]
	mov	r0, #18
	blx	r3
	ldrb	r10, [r4, #728]	@ zero_extendqisi2
	cmp	r10, #3
	beq	.L824
.L748:
	adds	r10, r10, #0
	ldrb	r3, [r4, #795]	@ zero_extendqisi2
	movne	r10, #1
	cmp	r3, #1
	movne	r2, #0
	andeq	r2, r10, #1
	cmp	r2, #0
	bne	.L751
	cmp	r3, #2
	beq	.L751
	ldr	r3, [r4, #2120]
	cmp	r3, #1
	movne	r10, #0
	andeq	r10, r10, #1
	cmp	r10, #0
	bne	.L751
	mov	r6, r6, asl #3
	b	.L754
.L826:
	bl	BsPos
	add	r10, r10, #1
	cmp	r0, r6
	bgt	.L825
	cmp	r10, #6400
	beq	.L755
.L754:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	sub	r0, fp, #76
	bne	.L826
	cmp	r10, #6400
	bcs	.L755
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L827
	ldr	r1, [r4, #896]
	cmp	r1, #0
	moveq	r0, r1
	bne	.L828
.L758:
	str	r10, [r4, #1500]
	mov	r1, #1
	str	r0, [r4, #1504]
	sub	r0, fp, #76
	bl	BsGet
	subs	r10, r0, #0
	bne	.L759
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L838+12
	blx	r3
	strb	r10, [r4, #1561]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L838+16
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L785
	ldr	r0, [r4, #2480]
	mov	r3, r10
	mov	r2, r10
	mov	r1, #103
	blx	ip
	mov	r0, r5
	b	.L747
.L755:
	mov	r3, #0
	sub	r0, fp, #76
	strb	r3, [r4, #1561]
	bl	BsPos
	ldr	r3, [r9, #68]
	mov	r2, r10
	ldr	r1, .L838+20
	mov	r4, r0
	mov	r0, #1
	blx	r3
	mov	r0, r4
	b	.L747
.L825:
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, [r9, #68]
	ldr	r1, .L838+24
	mov	r4, r0
	mov	r0, #1
	blx	r3
	mov	r0, r4
	b	.L747
.L824:
	ldr	r3, .L838+28
	ldr	r3, [r3]
	ands	r3, r3, #8
	bne	.L748
	strb	r3, [r4, #1561]
	sub	r0, fp, #76
	str	r3, [fp, #-80]
	bl	BsPos
	ldr	r2, .L838+16
	ldr	ip, [r2]
	cmp	ip, #0
	mov	r5, r0
	beq	.L749
	ldr	r3, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2480]
	mov	r2, r3
	blx	ip
.L749:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L838+32
	blx	r3
	mov	r0, r5
	b	.L747
.L751:
	sub	r0, fp, #76
	bl	BsPos
	b	.L747
.L759:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #729]
	cmp	r0, #0
	beq	.L829
	ldrb	r3, [r4, #728]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L761
	cmp	r3, #3
	beq	.L830
.L762:
	mov	r3, #0
	strb	r3, [r4, #730]
.L763:
	ldrb	r3, [r4, #836]	@ zero_extendqisi2
	mov	r2, #0
	strb	r2, [r4, #731]
	cmp	r3, #2
	addeq	r10, r5, #16384
	beq	.L764
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	add	r10, r5, #16384
	strb	r0, [r4, #732]
	ldrb	r3, [r10, #3626]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r3, [r4, #734]
	moveq	r3, #2
	streqb	r3, [r4, #733]
	bne	.L831
.L766:
	ldrb	r3, [r4, #840]	@ zero_extendqisi2
	sub	r3, r3, #1
	cmp	r3, #1
	bls	.L767
.L770:
	mov	r1, #5
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	cmp	r0, #0
	moveq	r3, #1
	streqb	r3, [r4, #735]
	strneb	r0, [r4, #735]
.L776:
	ldrb	r3, [r4, #728]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L832
.L764:
	mov	r3, #1
	strb	r3, [r4, #1561]
	ldr	r3, [r10, #3584]
	add	r0, r5, #21760
	mov	r1, r8
	mov	r2, #60
	orr	r3, r3, #2
	str	r3, [r10, #3584]
	ldr	r3, [r7, #3328]
	add	r0, r0, #152
	ldr	ip, [r9, #52]
	add	r3, r3, #1
	str	r3, [r7, #3328]
	blx	ip
	sub	r0, fp, #76
	bl	BsPos
	mov	r3, r0, asr #31
	mov	r8, r0
	add	r1, r0, #7
	rsb	r6, r0, r6
	mov	r3, r3, lsr #29
	str	r6, [r5, #1288]
	add	r2, r8, r3
	ldr	r0, [r4, #1576]
	and	r2, r2, #7
	cmp	r8, #0
	rsb	r3, r3, r2
	add	r3, r3, r0
	strb	r3, [r5, #1286]
	ldr	r3, [r4, #1568]
	movge	r1, r8
	add	r3, r3, r1, asr #3
	str	r3, [r5, #1296]
	ldrb	r3, [r7, #3421]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L780
	ldrb	r2, [r7, #3501]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L833
.L780:
	mov	r3, #0
	str	r3, [r5, #1292]
	strb	r3, [r5, #1287]
	str	r3, [r5, #1300]
.L781:
	ldrb	r3, [r4, #728]	@ zero_extendqisi2
	strb	r3, [r5, #1284]
	ldrb	r3, [r4, #737]	@ zero_extendqisi2
	strb	r3, [r5, #1282]
	ldrb	r3, [r4, #736]	@ zero_extendqisi2
	strb	r3, [r5, #1281]
	ldrb	r3, [r4, #735]	@ zero_extendqisi2
	strb	r3, [r5, #1285]
	ldrb	r3, [r4, #732]	@ zero_extendqisi2
	strb	r3, [r5, #1283]
	ldr	r2, [r10, #3580]
	cmp	r2, #0
	bne	.L834
.L782:
	mov	r0, r5
	bl	MP4_Record_SlcSegID
	mov	r0, r8
	b	.L747
.L827:
	ldr	r2, [r9, #68]
	mov	r0, #1
	ldr	r1, .L838+12
	str	r3, [fp, #-80]
	blx	r2
	ldr	r3, [fp, #-80]
	sub	r0, fp, #76
	strb	r3, [r4, #1561]
	bl	BsPos
	ldr	r2, .L838+16
	ldr	ip, [r2]
	cmp	ip, #0
	mov	r5, r0
	beq	.L785
	ldr	r3, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2480]
	mov	r2, r3
	blx	ip
	mov	r0, r5
	b	.L747
.L828:
	sub	r0, fp, #76
	bl	BsGet
	b	.L758
.L785:
	mov	r0, r5
	b	.L747
.L834:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L838+36
	blx	r3
	mov	r0, r5
	bl	MP4_ClearCurPic
	b	.L782
.L829:
	add	r5, r5, #16384
	mov	r3, #4
	mov	r2, #1
	strb	r3, [r4, #728]
	strb	r3, [r4, #1432]
	sub	r0, fp, #76
	strb	r2, [r4, #1561]
	ldr	r3, [r5, #3584]
	orr	r3, r3, #128
	str	r3, [r5, #3584]
	bl	BsPos
	b	.L747
.L830:
	ldrb	r3, [r4, #840]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L762
.L761:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #730]
	b	.L763
.L832:
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r2, r0
	strb	r2, [r4, #736]
	cmp	r2, #0
	beq	.L835
	ldrb	r3, [r4, #728]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L764
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r2, r0
	strb	r2, [r4, #737]
	cmp	r2, #0
	bne	.L764
	strb	r2, [r4, #1561]
	sub	r0, fp, #76
	str	r2, [fp, #-80]
	bl	BsPos
	ldr	r3, .L838+16
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L779
	ldr	r2, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2480]
	mov	r3, r2
	blx	ip
.L779:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L838+40
	blx	r3
	mov	r0, r5
	b	.L747
.L831:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #733]
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #734]
	b	.L766
.L767:
	ldrb	r3, [r4, #728]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L770
	ldrb	r3, [r4, #852]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L770
	add	r3, r5, #20992
	mov	r2, #0
	add	r3, r3, #252
	str	r5, [fp, #-84]
	str	r7, [fp, #-88]
	mov	r7, r2
	str	r8, [fp, #-80]
	mov	r5, r3
	str	r6, [fp, #-92]
	b	.L775
.L771:
	ldr	r3, [r4, #1536]
	cmp	r3, #500
	bne	.L772
	ldr	r2, [r4, #1540]
	movw	r3, #413
	cmp	r2, r3
	beq	.L773
.L772:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsSkip
.L773:
	sub	r0, fp, #76
	bl	bs_get_spritetrajectory
	subs	r3, r0, #0
	moveq	r6, r3
	bne	.L836
.L774:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsSkip
	str	r8, [r5]
	str	r6, [r5, #4]
	add	r7, r7, #1
	ldrb	r3, [r4, #852]	@ zero_extendqisi2
	add	r5, r5, #8
	cmp	r3, r7
	bls	.L837
.L775:
	sub	r0, fp, #76
	bl	bs_get_spritetrajectory
	subs	r6, r0, #0
	moveq	r8, r6
	beq	.L771
	mov	r1, r6
	sub	r0, fp, #76
	bl	BsGet
	sub	r2, r6, #1
	movs	r3, r0, asr r2
	mov	r8, r0
	moveq	r3, #1
	moveq	r3, r3, asl r6
	subeq	r3, r3, #1
	eoreq	r8, r8, r3
	rsbeq	r8, r8, #0
	b	.L771
.L836:
	mov	r1, r3
	sub	r0, fp, #76
	str	r3, [fp, #-96]
	bl	BsGet
	ldr	r3, [fp, #-96]
	sub	r2, r3, #1
	movs	r2, r0, asr r2
	mov	r6, r0
	moveq	r2, #1
	moveq	r3, r2, asl r3
	subeq	r2, r3, #1
	eoreq	r3, r0, r2
	rsbeq	r6, r3, #0
	b	.L774
.L833:
	ldr	r2, [r7, #3508]
	strb	r3, [r5, #1287]
	mov	r3, r2, asl #3
	str	r3, [r5, #1292]
	ldr	r3, [r7, #3504]
	str	r3, [r5, #1300]
	b	.L781
.L835:
	strb	r2, [r4, #1561]
	sub	r0, fp, #76
	str	r2, [fp, #-80]
	bl	BsPos
	ldr	r3, .L838+16
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L778
	ldr	r2, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2480]
	mov	r3, r2
	blx	ip
.L778:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldrb	r2, [r4, #728]	@ zero_extendqisi2
	ldr	r1, .L838+44
	blx	r3
	mov	r0, r5
	b	.L747
.L837:
	ldr	r8, [fp, #-80]
	ldr	r5, [fp, #-84]
	ldr	r7, [fp, #-88]
	ldr	r6, [fp, #-92]
	b	.L770
.L839:
	.align	2
.L838:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC43
	.word	.LC45
	.word	.LC8
	.word	g_event_report
	.word	.LC48
	.word	.LC47
	.word	g_VdmCharacter
	.word	.LC46
	.word	.LC44
	.word	.LC50
	.word	.LC49
	UNWIND(.fnend)
	.size	Mp4_Syntax_Vop, .-Mp4_Syntax_Vop
	.align	2
	.global	Mp4_Syntax_Slice_NotSH
	.type	Mp4_Syntax_Slice_NotSH, %function
Mp4_Syntax_Slice_NotSH:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	add	r6, r0, #12288
	mov	r4, r0
	movw	r3, #510
	ldr	r8, [r6, #3328]
	mov	r7, r2
	sub	r0, r8, #1
	cmp	r0, r3
	bhi	.L861
	movw	r3, #3576
	sub	r0, fp, #76
	ldrh	r9, [r6, r3]
	add	r5, r4, #20480
	add	r3, r8, #1
	str	r3, [r6, #3328]
	ubfx	r9, r9, #7, #4
	bl	BsInit
	add	r9, r9, #17
	sub	r0, fp, #76
	mov	r1, r9
	bl	BsSkip
	ldr	r2, [r5, #1492]
	ldr	r9, [r5, #1496]
	add	r2, r2, #15
	add	r9, r9, #15
	mov	r2, r2, lsr #4
	mov	r9, r9, lsr #4
	mul	r9, r9, r2
	cmp	r9, #0
	beq	.L843
	mov	lr, r9
	mov	r1, #0
.L844:
	add	ip, r1, #1
	movs	lr, lr, lsr #1
	uxth	r1, ip
	bne	.L844
	sxth	r1, r1
	sub	r0, fp, #76
	bl	BsGet
	mov	r10, r8, asl #2
	mov	r8, r8, asl #5
	sub	r1, r9, #1
	rsb	r3, r10, r8
	add	r3, r4, r3
	cmp	r0, r1
	str	r0, [r3, #1304]
	bhi	.L862
.L845:
	mov	r1, #5
	sub	r0, fp, #76
	bl	BsGet
	rsb	r9, r10, r8
	add	r9, r4, r9
	mov	r1, #1
	strb	r0, [r9, #1285]
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	beq	.L846
.L856:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	bne	.L856
	mov	r1, #1
	sub	r0, fp, #76
	add	ip, r4, #16384
	str	ip, [fp, #-80]
	bl	BsSkip
	ldr	ip, [fp, #-80]
	ldr	r1, [ip, #3684]
	cmp	r1, #0
	bne	.L863
.L848:
	mov	r1, #1
	sub	r0, fp, #76
	str	ip, [fp, #-80]
	bl	BsSkip
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsGet
	rsb	r9, r10, r8
	add	r9, r4, r9
	mov	r1, #3
	strb	r0, [r9, #1284]
	sub	r0, fp, #76
	bl	BsGet
	ldrb	r3, [r9, #1284]	@ zero_extendqisi2
	ldr	ip, [fp, #-80]
	cmp	r3, #0
	strb	r0, [r9, #1283]
	bne	.L864
.L850:
	ldrb	r2, [r5, #736]	@ zero_extendqisi2
	rsb	r3, r10, r8
	add	r3, r4, r3
	sub	r0, fp, #76
	str	r3, [fp, #-80]
	strb	r2, [r3, #1281]
	ldrb	r2, [r5, #737]	@ zero_extendqisi2
	strb	r2, [r3, #1282]
	ldr	r2, [ip, #3584]
	orr	r2, r2, #16
	str	r2, [ip, #3584]
	bl	BsPos
	ldr	r3, [fp, #-80]
	mov	r2, r0, asr #31
	mov	r9, r0
	rsb	r7, r0, r7, lsl #3
	str	r7, [r3, #1288]
	mov	r2, r2, lsr #29
	ldr	ip, [r5, #1576]
	add	r1, r9, r2
	cmp	r9, #0
	and	r1, r1, #7
	add	r0, r0, #7
	rsb	r2, r2, r1
	movge	r0, r9
	add	r2, r2, ip
	strb	r2, [r3, #1286]
	ldr	r2, [r5, #1568]
	add	r2, r2, r0, asr #3
	str	r2, [r3, #1296]
	ldrb	r2, [r6, #3421]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L852
	ldrb	r1, [r6, #3501]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L865
.L852:
	rsb	r8, r10, r8
	mov	r3, #0
	add	r8, r4, r8
	str	r3, [r8, #1292]
	strb	r3, [r8, #1287]
	str	r3, [r8, #1300]
.L853:
	mov	r0, r4
	bl	MP4_Record_SlcSegID
	mov	r0, r9
.L860:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L846:
	ldrb	r3, [r5, #728]	@ zero_extendqisi2
	add	ip, r4, #16384
	strb	r3, [r9, #1284]
	ldrb	r3, [r5, #732]	@ zero_extendqisi2
	strb	r3, [r9, #1283]
	b	.L850
.L861:
	add	r3, r4, #20480
	add	r1, r4, #15680
	add	r1, r1, #24
	ldr	r0, [r3, #2480]
	bl	MP4_ClearCurPacket
	mov	r0, r4
	add	r4, r4, #16384
	bl	MP4_ClearCurPic
	ldr	r1, .L866
	mov	r0, #1
	ldr	r3, [r4, #3584]
	ldr	r2, .L866+4
	bic	r3, r3, #432
	ldr	ip, [r1, #68]
	bic	r3, r3, #2
	ldr	r1, .L866+8
	str	r3, [r4, #3584]
	ldr	r3, [r6, #3328]
	blx	ip
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L863:
	sub	r0, fp, #76
	bl	BsSkip
	ldr	ip, [fp, #-80]
	b	.L848
.L864:
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsSkip
	ldrb	r3, [r9, #1284]	@ zero_extendqisi2
	ldr	ip, [fp, #-80]
	cmp	r3, #2
	bne	.L850
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsSkip
	ldr	ip, [fp, #-80]
	b	.L850
.L865:
	ldr	r1, [r6, #3508]
	strb	r2, [r3, #1287]
	mov	r2, r1, asl #3
	str	r2, [r3, #1292]
	ldr	r2, [r6, #3504]
	str	r2, [r3, #1300]
	b	.L853
.L843:
	mov	r1, r9
	sub	r0, fp, #76
	bl	BsGet
	mov	r10, r8, asl #2
	mov	r8, r8, asl #5
	rsb	r3, r10, r8
	add	r3, r4, r3
	str	r0, [r3, #1304]
	b	.L845
.L862:
	ldr	ip, .L866
	mov	r2, r0
	mov	r3, r9
	ldr	r1, .L866+12
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L860
.L867:
	.align	2
.L866:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+1296
	.word	.LC51
	.word	.LC52
	UNWIND(.fnend)
	.size	Mp4_Syntax_Slice_NotSH, .-Mp4_Syntax_Slice_NotSH
	.align	2
	.global	Mp4_Syntax_Slice_SH
	.type	Mp4_Syntax_Slice_SH, %function
Mp4_Syntax_Slice_SH:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	add	r8, r0, #12288
	movw	r3, #510
	mov	r4, r0
	ldr	r5, [r8, #3328]
	mov	r9, r2
	sub	r6, r5, #1
	cmp	r6, r3
	bhi	.L882
	ldrb	r7, [r8, #3576]	@ zero_extendqisi2
	add	r3, r5, #1
	sub	r0, fp, #76
	str	r3, [r8, #3328]
	ubfx	r7, r7, #3, #4
	bl	BsInit
	add	r7, r7, #17
	sub	r0, fp, #76
	add	r10, r4, #20480
	mov	r1, r7
	bl	BsSkip
	mov	r3, r5, asl #2
	mov	r1, #5
	sub	r0, fp, #76
	mov	r7, r3
	str	r3, [fp, #-80]
	bl	BsGet
	ldrb	r2, [r10, #740]	@ zero_extendqisi2
	mov	r3, r5, asl #5
	movw	lr, #1206
	str	r3, [fp, #-84]
	mov	r1, r3
	rsb	ip, r7, r1
	add	r5, r4, ip
	movw	r1, #1204
	mul	r0, r2, r0
	str	r0, [r5, #1304]
	ldrh	r2, [r4, lr]
	ldrh	r1, [r4, r1]
	mul	r1, r1, r2
	sub	r3, r1, #1
	cmp	r0, r3
	bhi	.L883
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #5
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r5, #1285]
	sub	r0, fp, #76
	ldrb	r2, [r10, #728]	@ zero_extendqisi2
	strb	r2, [r5, #1284]
	ldrb	r2, [r10, #732]	@ zero_extendqisi2
	strb	r2, [r5, #1283]
	ldrb	r2, [r10, #736]	@ zero_extendqisi2
	strb	r2, [r5, #1281]
	ldrb	r2, [r10, #737]	@ zero_extendqisi2
	strb	r2, [r5, #1282]
	bl	BsPos
	add	lr, r4, #16384
	mov	r2, r0, asr #31
	mov	r7, r0
	rsb	r9, r0, r9, lsl #3
	str	r9, [r5, #1288]
	mov	r2, r2, lsr #29
	ldr	r9, [r10, #1576]
	add	r1, r7, r2
	cmp	r7, #0
	and	r1, r1, #7
	add	r0, r0, #7
	rsb	r2, r2, r1
	movge	r0, r7
	add	r2, r2, r9
	strb	r2, [r5, #1286]
	ldr	r3, [r10, #1568]
	add	r0, r3, r0, asr #3
	str	r0, [r5, #1296]
	ldr	r3, [lr, #3584]
	orr	r3, r3, #16
	str	r3, [lr, #3584]
	ldrb	r3, [r8, #3421]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L884
.L872:
	ldr	r3, [fp, #-84]
	ldr	r2, [fp, #-80]
	rsb	r2, r2, r3
	mov	r3, #0
	add	r2, r4, r2
	str	r3, [r2, #1292]
	strb	r3, [r2, #1287]
	str	r3, [r2, #1300]
.L873:
	mov	r1, r6, asl #2
	mov	r3, r6, asl #5
	rsb	r2, r1, r3
	add	r2, r4, r2
	ldr	r0, [r2, #1300]
	cmp	r0, #0
	bne	.L885
.L874:
	rsb	r3, r1, r3
	ldrb	r2, [r8, #3576]	@ zero_extendqisi2
	add	r3, r4, r3
	ubfx	r2, r2, #3, #4
	ldr	r1, [r3, #1288]
	add	r2, r2, r1
	str	r2, [r3, #1288]
.L875:
	mov	r0, r4
	bl	MP4_Record_SlcSegID
	mov	r0, r7
.L870:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L885:
	ldr	r0, [r2, #1292]
	cmp	r0, #0
	beq	.L874
	ldrb	r3, [r8, #3576]	@ zero_extendqisi2
	ubfx	r3, r3, #3, #4
	add	r3, r3, r0
	str	r3, [r2, #1292]
	b	.L875
.L884:
	ldrb	r2, [r8, #3501]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L872
	ldr	r2, [r8, #3508]
	strb	r3, [r5, #1287]
	mov	r3, r2, asl #3
	str	r3, [r5, #1292]
	ldr	r3, [r8, #3504]
	str	r3, [r5, #1300]
	b	.L873
.L882:
	add	r3, r0, #20480
	add	r1, r0, #15680
	add	r1, r1, #24
	ldr	r0, [r3, #2480]
	bl	MP4_ClearCurPacket
	mov	r0, r4
	add	r4, r4, #16384
	bl	MP4_ClearCurPic
	ldr	r1, .L886
	mov	r0, #1
	ldr	r3, [r4, #3584]
	ldr	r2, .L886+4
	bic	r3, r3, #432
	ldr	ip, [r1, #68]
	bic	r3, r3, #2
	ldr	r1, .L886+8
	str	r3, [r4, #3584]
	ldr	r3, [r8, #3328]
	blx	ip
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L883:
	ldr	ip, .L886
	mov	r2, r0
	ldr	r1, .L886+12
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L870
.L887:
	.align	2
.L886:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+1320
	.word	.LC51
	.word	.LC53
	UNWIND(.fnend)
	.size	Mp4_Syntax_Slice_SH, .-Mp4_Syntax_Slice_SH
	.align	2
	.global	MP4_Dec_Slice
	.type	MP4_Dec_Slice, %function
MP4_Dec_Slice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	ip, r0, #12288
	mov	r4, r0
	ldrb	r3, [ip, #3576]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L898
	tst	r3, #4
	beq	.L899
	mov	r0, r4
	bl	Mp4_Syntax_Slice_SH
	cmn	r0, #1
	beq	.L896
.L892:
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L899:
	movw	r0, #3576
	ldrh	r0, [ip, r0]
	ubfx	r0, r0, #7, #4
	sub	lr, r0, #1
	cmp	lr, #6
	bfils	r3, r0, #3, #4
	mov	r0, r4
	strlsb	r3, [ip, #3576]
	bl	Mp4_Syntax_Slice_SH
	cmn	r0, #1
	bne	.L892
.L896:
	add	r3, r4, #20480
	add	r1, r4, #15680
	add	r1, r1, #24
	ldr	r0, [r3, #2480]
	bl	MP4_ClearCurPacket
	ldr	r3, .L901
	ldr	r2, .L901+4
	mov	r0, #1
	ldr	r1, .L901+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L898:
	str	r2, [fp, #-28]
	str	r1, [fp, #-24]
	bl	Mp4_Check_Slice_NotSH
	ldr	r1, [fp, #-24]
	ldr	r2, [fp, #-28]
	cmp	r0, #0
	beq	.L900
	mov	r1, r2
	mov	r0, r4
	bl	Mp4_Combine_SliceData
	cmp	r0, #0
	bne	.L896
	mov	r0, r4
	bl	MP4_Record_SlcSegID
	b	.L892
.L900:
	mov	r0, r4
	bl	Mp4_Syntax_Slice_NotSH
	cmn	r0, #1
	beq	.L896
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L902:
	.align	2
.L901:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+1340
	.word	.LC54
	UNWIND(.fnend)
	.size	MP4_Dec_Slice, .-MP4_Dec_Slice
	.align	2
	.global	MP4_Record_PacketID
	.type	MP4_Record_PacketID, %function
MP4_Record_PacketID:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #20480
	mov	r4, r0
	ldr	r3, [r5, #2036]
	cmp	r3, #62
	bgt	.L906
	add	r1, r0, #12288
	add	r4, r0, r3, lsl #2
	add	r4, r4, #20480
	add	r2, r3, #1
	ldr	r0, [r1, #3432]
	str	r0, [r4, #1780]
	str	r2, [r5, #2036]
	ldrb	r2, [r1, #3421]	@ zero_extendqisi2
	cmp	r2, #0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldrb	r2, [r1, #3501]	@ zero_extendqisi2
	cmp	r2, #1
	ldreq	r2, [r1, #3512]
	addeq	r3, r3, #2
	streq	r2, [r4, #1784]
	streq	r3, [r5, #2036]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L906:
	bl	MP4_Clean_HeadPacket
	add	r1, r4, #15680
	ldr	r0, [r5, #2480]
	add	r1, r1, #24
	bl	MP4_ClearCurPacket
	ldr	ip, .L907
	ldr	r2, [r5, #2036]
	mov	r3, #62
	ldr	r1, .L907+4
	mov	r0, #0
	ldr	ip, [ip, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	ip
.L908:
	.align	2
.L907:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC42
	UNWIND(.fnend)
	.size	MP4_Record_PacketID, .-MP4_Record_PacketID
	.align	2
	.global	MP4_Record_HeadPacketInfo
	.type	MP4_Record_HeadPacketInfo, %function
MP4_Record_HeadPacketInfo:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	mov	r7, r1
	beq	.L912
	ldr	r6, .L914
	add	r1, r4, #15680
	add	r0, r4, #22016
	add	r1, r1, #24
	mov	r2, #160
	add	r0, r0, #80
	ldr	r3, [r6, #52]
	add	r5, r4, #20480
	blx	r3
	add	r1, r4, #15808
	add	r0, r4, #22016
	ldr	r3, [r6, #52]
	add	r1, r1, #56
	add	r0, r0, #240
	mov	r2, #4
	blx	r3
	ldr	r3, [r5, #2044]
	str	r7, [r5, #2040]
	cmp	r3, #0
	beq	.L913
.L911:
	ldr	r3, [r5, #2048]
	add	r3, r3, #1
	str	r3, [r5, #2048]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L913:
	mov	r0, r4
	bl	MP4_Record_PacketID
	mov	r3, #1
	str	r3, [r5, #2044]
	b	.L911
.L912:
	ldr	r3, .L914
	mov	r0, #1
	ldr	r1, .L914+4
	ldr	r3, [r3, #68]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L915:
	.align	2
.L914:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC55
	UNWIND(.fnend)
	.size	MP4_Record_HeadPacketInfo, .-MP4_Record_HeadPacketInfo
	.align	2
	.global	Mp4_Syntax_Parse
	.type	Mp4_Syntax_Parse, %function
Mp4_Syntax_Parse:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #92)
	sub	sp, sp, #92
	ldr	r9, .L1067
	mov	r5, r2
	mov	r4, r0
	mov	r7, r1
	str	r2, [fp, #-120]
	ldr	r3, [r9, #48]
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #108
	blx	r3
	mov	r2, r5
	mov	r1, r7
	sub	r0, fp, #108
	bl	BsInit
	sub	r0, fp, #108
	mov	r1, #32
	add	r8, r4, #16384
	bl	BsShow
	movw	r3, #510
	mov	r10, #0
	str	r10, [r8, #3588]
	cmp	r0, r3
	mov	r5, r0
	beq	.L1046
	add	r1, r4, #12288
	ldrb	r3, [r1, #3576]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L1047
.L922:
	add	r6, r4, #20480
	mov	r5, #196
	mov	r2, #1
	strb	r2, [r6, #797]
.L923:
	and	r3, r3, #2
	ands	r3, r3, #255
	bne	.L924
	ldr	r2, [r8, #3584]
	ands	r10, r2, #130
	bne	.L1048
	cmp	r5, #197
	beq	.L1049
	cmp	r5, #432
	beq	.L1050
	movw	r3, #437
	cmp	r5, r3
	beq	.L1051
	bic	r3, r5, #15
	cmp	r3, #288
	beq	.L1052
	movw	r3, #435
	cmp	r5, r3
	beq	.L1053
	movw	r3, #438
	cmp	r5, r3
	beq	.L1054
	cmp	r5, #196
	beq	.L1055
	movw	r3, #434
	cmp	r5, r3
	beq	.L1056
	movw	r3, #286
	cmp	r5, r3
	bne	.L939
	ldr	r3, .L1067+4
	ldr	r10, [r3]
	cmp	r10, #0
	beq	.L940
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r6, #2480]
	blx	r10
.L940:
	ldr	r3, [fp, #-120]
	add	r1, r4, #15680
	ldr	r0, [r6, #2480]
	add	r1, r1, #24
	sub	r3, r3, #4
	str	r3, [fp, #-120]
	bl	MP4_ClearCurPacket
	ldr	r3, [r8, #3584]
	add	r2, r7, #4
	str	r2, [fp, #-124]
	tst	r3, #2
	orrne	r3, r3, #256
	strne	r3, [r8, #3584]
.L941:
	add	ip, r4, #53248
	mov	r10, #0
	ldrb	r3, [ip, #3561]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L978
.L947:
	ldrb	r3, [r6, #1559]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1041
	ldr	r3, [r6, #2056]
	add	r1, r4, #19968
	add	r0, r4, #21248
	add	r1, r1, #32
	add	r3, r3, #1
	mov	lr, #0
	str	r3, [r6, #2056]
	mov	r2, #604
	add	r0, r0, #60
	ldr	r3, [r9, #52]
	str	lr, [r6, #2060]
	str	ip, [fp, #-120]
	blx	r3
	ldr	r2, [r6, #1492]
	ldr	r0, [r8, #3700]
	ldr	r1, [r8, #3696]
	cmp	r2, r0
	ldr	ip, [fp, #-120]
	beq	.L1057
.L966:
	ldr	r3, .L1067+4
	ldr	r5, [r3]
	cmp	r5, #0
	beq	.L968
	ldr	lr, [r6, #1496]
	mov	r3, #8
	strh	r2, [fp, #-114]	@ movhi
	sub	r2, fp, #116
	strh	r1, [fp, #-112]	@ movhi
	mov	r1, #2
	strh	r0, [fp, #-110]	@ movhi
	strh	lr, [fp, #-116]	@ movhi
	ldr	r0, [r6, #2480]
	str	ip, [fp, #-120]
	blx	r5
	ldr	r0, [r8, #3700]
	ldr	r1, [r8, #3696]
	ldr	ip, [fp, #-120]
.L968:
	mov	r5, #1
	str	r0, [r6, #1492]
	str	r1, [r6, #1496]
.L967:
	sxtb	r5, r5
	cmp	r5, #1
	beq	.L1058
.L970:
	ldr	r3, [r6, #1496]
	mov	r2, #0
	ldr	r1, [r6, #1492]
	str	r3, [r6, #2080]
	str	r1, [r6, #2076]
	ldr	r3, [r8, #3700]
	str	r3, [r6, #1492]
	ldr	r3, [r8, #3696]
	strb	r2, [r6, #1559]
	str	r3, [r6, #1496]
	b	.L1041
.L1048:
	orr	r2, r2, #32
	mov	r10, r3
	str	r2, [r8, #3584]
	mov	r3, #1
	str	r3, [r8, #3588]
.L1041:
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L924:
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r4
	bl	MP4_Dec_Slice
	mov	r10, r0
.L928:
	add	ip, r4, #53248
	str	r7, [fp, #-124]
	ldrb	r3, [ip, #3561]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L978
	ldr	r3, [fp, #-120]
	cmp	r10, r3, asl #3
	bhi	.L946
.L1063:
	movw	r3, #438
	cmp	r5, r3
	cmpne	r5, #196
	bne	.L947
	ldr	r3, [r8, #3584]
	tst	r3, #130
	beq	.L947
	ldrb	r3, [r6, #797]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1059
	ldr	r2, [r6, #1496]
	ldr	r1, [r6, #1492]
	cmp	r2, #0
	str	r2, [r6, #2084]
	str	r1, [r6, #2088]
	beq	.L951
	ldr	r3, [r6, #2076]
	cmp	r2, r3
	beq	.L951
.L986:
	mov	r5, #1
.L952:
	ldr	r3, .L1067+4
	ldr	r8, [r3]
	cmp	r8, #0
	beq	.L956
	ldr	r0, [r6, #2080]
	mov	r3, #8
	ldr	lr, [r6, #2076]
	strh	r2, [fp, #-112]	@ movhi
	sub	r2, fp, #116
	strh	r1, [fp, #-110]	@ movhi
	mov	r1, #2
	strh	r0, [fp, #-114]	@ movhi
	strh	lr, [fp, #-116]	@ movhi
	ldr	r0, [r6, #2480]
	str	ip, [fp, #-120]
	blx	r8
	ldr	ip, [fp, #-120]
.L956:
	sxtb	r5, r5
	cmp	r5, #1
	beq	.L1060
.L958:
	ldr	r2, [r6, #2084]
	ldr	r3, [r6, #2088]
	str	r2, [r6, #2076]
	str	r3, [r6, #2080]
.L950:
	add	r1, r4, #20992
	add	r0, r4, #21760
	ldr	r3, [r9, #52]
	mov	r2, #60
	add	r1, r1, #216
	add	r0, r0, #152
	blx	r3
	ldr	r3, [r9, #68]
	ldr	r1, .L1067+8
	mov	r0, #2
	blx	r3
	ldr	r3, [fp, #-124]
	rsb	r7, r7, r3
	ldr	r3, [r6, #2060]
	add	r10, r10, r7, lsl #3
	add	r3, r3, #1
	str	r3, [r6, #2060]
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1047:
	mov	r2, r0, lsr #8
	add	r6, r4, #20480
	sub	r2, r2, #128
	mov	r0, #2
	cmp	r2, #3
	strb	r0, [r6, #797]
	ldrlsb	r3, [r1, #3576]	@ zero_extendqisi2
	orrls	r3, r3, r0
	strlsb	r3, [r1, #3576]
	b	.L923
.L951:
	cmp	r1, #0
	beq	.L953
	ldr	r3, [r6, #2080]
	cmp	r1, r3
	beq	.L1061
	cmp	r2, #0
	beq	.L986
	ldr	r3, [r6, #2076]
	cmp	r2, r3
	bne	.L986
	mov	r5, #1
	b	.L979
.L978:
	ldr	r1, [r8, #3696]
	cmp	r1, #0
	beq	.L944
	ldr	r2, [r8, #3700]
	cmp	r2, #0
	bne	.L1062
.L944:
	mov	r3, #0
	strb	r3, [ip, #3561]
	ldr	r3, [fp, #-120]
	cmp	r10, r3, asl #3
	bls	.L1063
.L946:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+12
	blx	r3
	ldr	r2, [fp, #-124]
	ldr	r3, [r8, #3584]
	rsb	r10, r7, r2
	bic	r3, r3, #432
	mov	r10, r10, asl #3
	bic	r3, r3, #2
	str	r3, [r8, #3584]
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1046:
	ldr	r3, [r9, #52]
	mov	r2, #32
	sub	r1, fp, #108
	sub	r0, fp, #76
	blx	r3
	mov	r1, #32
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #32
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #32
	mov	r6, r0
	sub	r0, fp, #76
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r6, r3
	bne	.L918
	ldr	r2, .L1067+4
	add	r3, r4, #53248
	mov	r1, #1
	add	r6, r4, #20480
	strb	r1, [r3, #3560]
	ldr	r5, [r2]
	cmp	r5, r10
	beq	.L919
	mov	r3, r10
	mov	r2, r10
	mov	r1, #117
	ldr	r0, [r6, #2480]
	blx	r5
.L919:
	ldr	r3, [r8, #3584]
	add	r1, r4, #15680
	add	r1, r1, #24
	mov	r10, #0
	tst	r3, #130
	orrne	r3, r3, #32
	strne	r3, [r8, #3584]
	movne	r2, #1
	strne	r2, [r8, #3588]
	ldr	r0, [r6, #2480]
	bl	MP4_ClearCurPacket
	b	.L1041
.L918:
	add	r3, r4, #12288
	ldrb	r3, [r3, #3576]	@ zero_extendqisi2
	tst	r3, #1
	bne	.L922
	add	r6, r4, #20480
	mov	r2, #2
	strb	r2, [r6, #797]
	b	.L923
.L939:
	ldr	r3, [fp, #-120]
	add	r1, r4, #15680
	ldr	r0, [r6, #2480]
	add	r1, r1, #24
	sub	r3, r3, #4
	str	r3, [fp, #-120]
	add	r3, r7, #4
	str	r3, [fp, #-124]
	bl	MP4_ClearCurPacket
	b	.L941
.L1050:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+16
	blx	r3
	str	r10, [r6, #2092]
	mov	r1, r7
	ldr	r2, [fp, #-120]
	mov	r0, r4
	bl	Mp4_Syntax_Vos
	mov	r1, r5
	mov	r10, r0
	mov	r0, r4
	bl	MP4_Record_HeadPacketInfo
	b	.L928
.L953:
	ldr	r5, [r6, #2484]
	subs	r5, r5, #1
	movne	r5, #1
	cmp	r2, #0
	beq	.L956
.L980:
	ldr	r3, [r6, #2076]
	cmp	r2, r3
	bne	.L952
.L955:
	cmp	r1, #0
	beq	.L956
.L979:
	ldr	r3, [r6, #2080]
	cmp	r3, r1
	beq	.L956
	b	.L952
.L1054:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+20
	blx	r3
	ldr	r3, [fp, #-120]
	cmp	r3, #4
	beq	.L1064
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r4
	bl	Mp4_Syntax_Vop
	ldr	r3, [r8, #3584]
	tst	r3, #2
	mov	r10, r0
	bne	.L935
	add	r1, r4, #15680
	ldr	r0, [r6, #2480]
	add	r1, r1, #24
	bl	MP4_ClearCurPacket
.L934:
	ldr	r3, [r9, #68]
	mov	r2, r10
	ldr	r1, .L1067+24
	mov	r0, #7
	blx	r3
	b	.L928
.L1057:
	ldr	r3, [r6, #1496]
	cmp	r3, r1
	bne	.L966
	ldr	r5, [r6, #2484]
	subs	r5, r5, #1
	movne	r5, #1
	b	.L967
.L1051:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+28
	blx	r3
	mov	r3, #1
	mov	r1, r7
	ldr	r2, [fp, #-120]
	str	r3, [r6, #2092]
	mov	r0, r4
	bl	Mp4_Syntax_Vo
	mov	r1, r5
	mov	r10, r0
	mov	r0, r4
	bl	MP4_Record_HeadPacketInfo
	b	.L928
.L1052:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+32
	blx	r3
	mov	r3, #2
	ldr	r2, [fp, #-120]
	mov	r1, r7
	str	r3, [r6, #2092]
	mov	r0, r4
	bl	Mp4_Syntax_Vol
	mov	r1, r5
	mov	r10, r0
	mov	r0, r4
	bl	MP4_Record_HeadPacketInfo
	ldrb	r3, [r6, #1559]	@ zero_extendqisi2
	ldr	r2, .L1067
	cmp	r3, #1
	beq	.L928
	ldrb	ip, [r6, #1560]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L928
	add	r1, r4, #20480
	add	r0, r4, #19968
	ldr	r3, [r2, #52]
	add	r1, r1, #124
	add	r0, r0, #32
	mov	r2, #604
	str	ip, [fp, #-124]
	blx	r3
	ldr	ip, [fp, #-124]
	strb	ip, [r6, #1559]
	strb	ip, [r6, #789]
	b	.L928
.L1049:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+36
	blx	r3
	add	r1, r4, #15680
	add	r1, r1, #24
	ldr	r0, [r6, #2480]
	bl	MP4_ClearCurPacket
	b	.L1041
.L1061:
	ldr	r5, [r6, #2484]
	subs	r5, r5, #1
	movne	r5, #1
	cmp	r2, #0
	beq	.L955
	b	.L980
.L1059:
	ldrb	r3, [r6, #728]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L949
	ldr	r2, [r6, #1508]
	ldr	r3, [r6, #1500]
	ldr	r0, [r6, #1504]
	str	r2, [r6, #1512]
	add	r2, r2, r3
	str	r2, [r6, #1508]
	ldr	r3, [r8, #3688]
	ldr	r1, [r6, #1520]
	mla	r3, r3, r2, r0
	rsb	r2, r1, r3
	str	r2, [r6, #1524]
	str	r3, [r6, #1516]
	str	r3, [r6, #1520]
	b	.L950
.L1053:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+40
	blx	r3
	mov	r3, #3
	mov	r1, r7
	ldr	r2, [fp, #-120]
	str	r3, [r6, #2092]
	mov	r0, r4
	bl	Mp4_Syntax_Gop
	mov	r1, r5
	mov	r10, r0
	mov	r0, r4
	bl	MP4_Record_HeadPacketInfo
	b	.L928
.L1062:
	mov	r0, r4
	str	ip, [fp, #-128]
	bl	Frame_Init
	ldr	ip, [fp, #-128]
	cmp	r0, #0
	beq	.L944
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldr	r1, .L1067+44
	blx	r3
	ldr	ip, [fp, #-128]
	b	.L944
.L1055:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+48
	blx	r3
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r4
	bl	Mp4_Syntax_ShortHeader
	ldr	r3, [r8, #3584]
	tst	r3, #2
	mov	r10, r0
	beq	.L1042
	mov	r1, #196
	mov	r0, r4
	bl	MP4_Record_HeadPacketInfo
	b	.L928
.L1056:
	ldr	r3, [r9, #68]
	mov	r0, #2
	ldr	r1, .L1067+52
	blx	r3
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r4
	bl	Mp4_Syntax_UserData
	mov	r10, r0
.L1042:
	add	r1, r4, #15680
	ldr	r0, [r6, #2480]
	add	r1, r1, #24
	bl	MP4_ClearCurPacket
	b	.L928
.L1060:
	ldr	r1, [ip, #3544]
	ldr	r0, [r6, #2480]
	str	ip, [fp, #-120]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L959
	ldr	r1, [r8, #212]
	mov	r3, #0
	ldr	r0, [r6, #2480]
	str	r3, [fp, #-128]
	bl	FreeUsdByDec
	ldr	r1, [r8, #216]
	ldr	r0, [r6, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r8, #220]
	ldr	r0, [r6, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r8, #224]
	ldr	r0, [r6, #2480]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-128]
	ldr	ip, [fp, #-120]
	mov	r2, r5
	str	r3, [r8, #212]
	str	r3, [r8, #216]
	str	r3, [r8, #220]
	str	r3, [r8, #224]
	ldr	r1, [ip, #3544]
	ldr	r0, [r6, #2480]
	bl	FSP_ClearLogicFs
	ldr	ip, [fp, #-120]
	mov	r2, r5
	ldr	r0, [r6, #2480]
	ldr	r1, [ip, #3536]
	bl	FSP_ClearLogicFs
	ldr	ip, [fp, #-120]
	ldr	r3, [fp, #-128]
	str	r3, [ip, #3536]
	str	r3, [ip, #3544]
	ldr	r0, [r6, #2480]
	bl	FSP_GetNullLogicFs
	ldr	ip, [fp, #-120]
	str	r0, [ip, #3544]
	str	r0, [ip, #3536]
.L959:
	add	r5, r4, #22528
	mov	r3, #0
	add	r5, r5, #80
	str	r3, [r6, #2488]
	ldr	r0, [r6, #2480]
	mov	r1, r5
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r6, #1580]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L960
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L1065
.L960:
	ldr	r0, [r6, #2480]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1043
.L962:
	ldr	r2, [r6, #2088]
	mov	r0, r4
	ldr	r1, [r6, #2084]
	bl	Frame_Init
	cmp	r0, #0
	beq	.L958
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldr	r1, .L1067+56
	blx	r3
	b	.L958
.L935:
	movw	r1, #438
	mov	r0, r4
	bl	MP4_Record_HeadPacketInfo
	b	.L934
.L949:
	ldr	r1, [r6, #1512]
	ldr	r2, [r6, #1500]
	ldr	r3, [r8, #3688]
	ldr	ip, [r6, #1504]
	add	r2, r2, r1
	ldr	r0, [r6, #1520]
	ldr	r1, [r6, #1524]
	mla	r3, r3, r2, ip
	rsb	r2, r0, r1
	add	r2, r2, r3
	str	r2, [r6, #1528]
	str	r3, [r6, #1516]
	b	.L950
.L1064:
	mov	r1, r5
	mov	r0, r4
	bl	MP4_Record_HeadPacketInfo
	b	.L934
.L1058:
	ldr	r1, [ip, #3544]
	ldr	r0, [r6, #2480]
	str	ip, [fp, #-120]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L971
	ldr	r1, [r7, #212]
	mov	r3, #0
	ldr	r0, [r6, #2480]
	str	r3, [fp, #-124]
	bl	FreeUsdByDec
	ldr	r1, [r7, #216]
	ldr	r0, [r6, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r7, #220]
	ldr	r0, [r6, #2480]
	bl	FreeUsdByDec
	ldr	r1, [r7, #224]
	ldr	r0, [r6, #2480]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-124]
	ldr	ip, [fp, #-120]
	mov	r2, r5
	str	r3, [r7, #212]
	str	r3, [r7, #216]
	str	r3, [r7, #220]
	str	r3, [r7, #224]
	ldr	r1, [ip, #3544]
	ldr	r0, [r6, #2480]
	bl	FSP_ClearLogicFs
	ldr	ip, [fp, #-120]
	mov	r2, r5
	ldr	r0, [r6, #2480]
	ldr	r1, [ip, #3536]
	bl	FSP_ClearLogicFs
	ldr	ip, [fp, #-120]
	ldr	r3, [fp, #-124]
	str	r3, [ip, #3536]
	str	r3, [ip, #3544]
	ldr	r0, [r6, #2480]
	bl	FSP_GetNullLogicFs
	ldr	ip, [fp, #-120]
	str	r0, [ip, #3544]
	str	r0, [ip, #3536]
.L971:
	add	r5, r4, #22528
	mov	r3, #0
	add	r5, r5, #80
	str	r3, [r6, #2488]
	ldr	r0, [r6, #2480]
	mov	r1, r5
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r6, #1580]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L972
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L1066
.L972:
	ldr	r0, [r6, #2480]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1044
.L974:
	mov	r0, r4
	ldr	r2, [r6, #1492]
	ldr	r1, [r6, #1496]
	bl	Frame_Init
	cmp	r0, #0
	beq	.L970
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldr	r1, .L1067+60
	blx	r3
	b	.L970
.L1065:
	ldr	r3, [r6, #2484]
	cmp	r3, #0
	bne	.L962
.L1043:
	mov	r0, r5
	bl	ResetVoQueue
	b	.L962
.L1066:
	ldr	r3, [r6, #2484]
	cmp	r3, #0
	bne	.L974
.L1044:
	mov	r0, r5
	bl	ResetVoQueue
	b	.L974
.L1068:
	.align	2
.L1067:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC67
	.word	.LC69
	.word	.LC57
	.word	.LC61
	.word	.LC62
	.word	.LC58
	.word	.LC59
	.word	.LC56
	.word	.LC60
	.word	.LC65
	.word	.LC63
	.word	.LC64
	.word	.LC66
	.word	.LC68
	UNWIND(.fnend)
	.size	Mp4_Syntax_Parse, .-Mp4_Syntax_Parse
	.align	2
	.global	MP4_Combine_HdPacket
	.type	MP4_Combine_HdPacket, %function
MP4_Combine_HdPacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #20480
	ldr	r3, [r5, #1616]
	cmp	r3, #0
	beq	.L1082
	add	r3, r0, #12288
	ldrb	r4, [r3, #3421]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L1083
	cmp	r4, #0
	bne	.L1077
	ldrb	r2, [r3, #3501]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1077
	ldr	ip, .L1086
	add	r1, r0, #15744
	ldr	r2, [r3, #3428]
	add	r0, r0, #22016
	ldr	r3, [r5, #1628]
	add	r1, r1, #40
	ldr	ip, [ip, #52]
	add	r0, r0, #160
	add	r3, r3, r2
	mov	r2, #80
	str	r3, [r5, #1628]
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1083:
	ldrb	r4, [r5, #1621]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L1084
	cmp	r4, #0
	bne	.L1076
	ldrb	r2, [r5, #1701]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1076
	ldr	r2, [r3, #3428]
	mov	r0, r4
	ldr	r3, [r5, #1708]
	add	r3, r3, r2
	str	r3, [r5, #1708]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1077:
	ldr	r3, .L1086
	movw	r2, #5404
.L1081:
	ldr	r1, .L1086+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1084:
	ldr	r1, [r3, #3424]
	ldr	r2, [r5, #1624]
	cmp	r1, r2
	bcc	.L1085
	ldr	r2, [r3, #3428]
	mov	r0, #0
	ldr	r3, [r5, #1628]
	add	r3, r3, r2
	str	r3, [r5, #1628]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1076:
	ldr	r3, .L1086
	movw	r2, #5390
	b	.L1081
.L1085:
	ldr	r3, .L1086
	add	r1, r0, #15680
	add	r0, r0, #22016
	add	r1, r1, #24
	mov	r2, #80
	add	r0, r0, #160
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #0
	strb	r4, [r5, #1701]
	mov	r0, r3
	strb	r3, [r5, #1621]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1082:
	ldr	r2, .L1086+8
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L1071
	ldr	r0, [r5, #2480]
	mov	r2, r3
	mov	r1, #103
	blx	ip
.L1071:
	ldr	r3, .L1086
	mov	r0, #1
	ldr	r1, .L1086+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1087:
	.align	2
.L1086:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC35
	.word	g_event_report
	.word	.LC70
	UNWIND(.fnend)
	.size	MP4_Combine_HdPacket, .-MP4_Combine_HdPacket
	.align	2
	.global	MP4_Renew_CurPacket
	.type	MP4_Renew_CurPacket, %function
MP4_Renew_CurPacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	bl	MP4_Combine_HdPacket
	cmn	r0, #1
	mov	r5, r0
	beq	.L1091
	ldr	r5, .L1092
	mov	r0, r4
	bl	MP4_Record_PacketID
	add	r1, r4, #22016
	add	r0, r4, #15680
	add	r1, r1, #80
	ldr	r3, [r5, #52]
	mov	r2, #160
	add	r0, r0, #24
	blx	r3
	add	r1, r4, #22016
	add	r0, r4, #15744
	ldr	r3, [r5, #52]
	add	r1, r1, #240
	mov	r2, #4
	add	r0, r0, #32
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1091:
	add	r3, r4, #20480
	add	r1, r4, #15680
	add	r1, r1, #24
	ldr	r0, [r3, #2480]
	bl	MP4_ClearCurPacket
	ldr	r3, .L1092
	movw	r2, #5420
	ldr	r1, .L1092+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1093:
	.align	2
.L1092:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC71
	UNWIND(.fnend)
	.size	MP4_Renew_CurPacket, .-MP4_Renew_CurPacket
	.align	2
	.global	MP4_Check_CurPacket
	.type	MP4_Check_CurPacket, %function
MP4_Check_CurPacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r4, r0, #12288
	add	r5, r0, #20480
	mov	r3, #0
	ldr	r2, [r4, #3424]
	mov	r6, r0
	sub	r0, fp, #60
	str	r2, [r5, #1568]
	ldr	r1, [r4, #3416]
	str	r1, [r5, #1564]
	ldr	r2, [r4, #3428]
	str	r3, [r5, #1576]
	str	r2, [r5, #1572]
	bl	BsInit
	sub	r0, fp, #60
	mov	r1, #32
	bl	BsShow
	ldrb	r2, [r4, #3576]	@ zero_extendqisi2
	tst	r2, #1
	mov	r3, r0
	beq	.L1120
	mov	r3, #1
	strb	r3, [r5, #797]
	ldrb	r3, [r4, #3576]	@ zero_extendqisi2
	tst	r3, #2
	bne	.L1097
	ldr	r2, [r5, #2040]
	mov	r0, #196
.L1101:
	movw	r3, #438
	cmp	r0, r3
	cmpeq	r2, r3
	beq	.L1108
	cmp	r0, #196
	cmpeq	r2, #196
	bne	.L1118
.L1108:
	ldr	r3, [r5, #2048]
	cmp	r3, #1
	bgt	.L1100
	b	.L1118
.L1120:
	mov	r2, r0, lsr #8
	mov	r1, #2
	sub	r2, r2, #128
	strb	r1, [r5, #797]
	cmp	r2, #3
	bls	.L1121
.L1096:
	ldrb	r2, [r4, #3576]	@ zero_extendqisi2
	tst	r2, #2
	ldr	r2, [r5, #2040]
	bne	.L1099
	movw	r1, #438
	cmp	r2, r1
	cmpne	r3, #196
	beq	.L1101
.L1100:
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
.L1118:
	mov	r0, #0
.L1105:
	ldr	r3, [r4, #3416]
	cmp	r3, #0
	beq	.L1111
	ldr	r3, [r4, #3424]
	cmp	r3, #0
	mvneq	r0, #0
.L1117:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1099:
	movw	r1, #438
	sub	r3, r3, #196
	rsb	r2, r1, r2
	clz	r3, r3
	clz	r2, r2
	mov	r3, r3, lsr #5
	mov	r2, r2, lsr #5
	orrs	r1, r3, r2
	beq	.L1102
	ldrb	r0, [r5, #1561]	@ zero_extendqisi2
	cmp	r0, #1
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L1110
.L1102:
	mov	r0, r6
	bl	MP4_Renew_CurPacket
	b	.L1105
.L1097:
	ldr	r3, [r5, #2040]
	movw	r2, #438
	rsb	r2, r2, r3
	clz	r2, r2
	mov	r2, r2, lsr #5
.L1110:
	ldr	r3, [r5, #2044]
	cmp	r3, #1
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r2, #0
	beq	.L1118
	ldr	r3, [r5, #1628]
	cmp	r3, #4
	bne	.L1118
	b	.L1102
.L1121:
	ldrb	r2, [r4, #3576]	@ zero_extendqisi2
	orr	r2, r2, r1
	strb	r2, [r4, #3576]
	b	.L1096
.L1111:
	mvn	r0, #0
	b	.L1117
	UNWIND(.fnend)
	.size	MP4_Check_CurPacket, .-MP4_Check_CurPacket
	.align	2
	.global	MPEG4DEC_Decode
	.type	MPEG4DEC_Decode, %function
MPEG4DEC_Decode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r4, r0, #20480
	mov	r6, r0
	mov	r5, r1
	ldr	r3, [r4, #2068]
	cmp	r3, #0
	beq	.L1123
	ldr	r3, [r1, #12]
	cmp	r3, #0
	ble	.L1123
	ldr	r3, [r1, #8]
	cmp	r3, #0
	beq	.L1123
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L1123
	ldr	r7, .L1225
	mov	r0, #2
	ldr	r1, .L1225+4
	add	r8, r6, #15680
	add	r8, r8, #24
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r2, [r5, #8]
	mov	r0, #7
	ldr	r1, .L1225+8
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r2, [r5, #12]
	mov	r0, #7
	ldr	r1, .L1225+12
	blx	r3
	ldr	r3, [r7, #68]
	ldrb	r2, [r5, #5]	@ zero_extendqisi2
	mov	r0, #7
	ldr	r1, .L1225+16
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r2, [r5, #88]
	mov	r0, #7
	ldr	r1, .L1225+20
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r2, [r5, #92]
	mov	r0, #7
	ldr	r1, .L1225+24
	blx	r3
	ldr	r3, [r7, #68]
	ldrb	r2, [r5, #85]	@ zero_extendqisi2
	mov	r0, #7
	ldr	r1, .L1225+28
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r2, [r5, #12]
	mov	r0, #7
	ldr	r1, .L1225+32
	blx	r3
	ldr	r2, [r5]
	ldr	r3, [r7, #68]
	mov	r0, #7
	ldr	r1, .L1225+36
	ldrb	r2, [r2]	@ zero_extendqisi2
	blx	r3
	ldr	r2, [r5]
	ldr	r3, [r7, #68]
	mov	r0, #7
	ldr	r1, .L1225+40
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	blx	r3
	ldr	r2, [r5]
	ldr	r3, [r7, #68]
	mov	r0, #7
	ldr	r1, .L1225+44
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	blx	r3
	ldr	r2, [r5]
	ldr	r3, [r7, #68]
	mov	r0, #7
	ldr	r1, .L1225+48
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	blx	r3
	ldr	r3, [r7, #52]
	mov	r2, #160
	mov	r1, r5
	mov	r0, r8
	blx	r3
	mov	r1, r8
	mov	r0, r6
	bl	MP4_GetScdUpMsg
	mov	r0, r6
	bl	MP4_Check_CurPacket
	cmn	r0, #1
	mov	r9, r0
	beq	.L1211
	mov	r1, r8
	mov	r0, r6
	bl	MP4_GetScdUpMsg
	add	r3, r6, #12288
	mov	r2, #0
	mov	r0, r6
	ldr	r1, [r3, #3424]
	str	r1, [r4, #1568]
	ldr	r1, [r3, #3416]
	str	r1, [r4, #1564]
	ldr	r3, [r3, #3428]
	str	r2, [r4, #1576]
	str	r3, [r4, #1572]
	bl	Mp4_Get_DecMode
	ldr	r3, [r4, #1580]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L1212
.L1127:
	add	r9, r6, #16384
	add	r8, r6, #53248
	ldr	r3, [r9, #3584]
	str	r3, [r8, #3564]
.L1128:
	ldr	r2, [r4, #1572]
	mov	r0, r6
	ldr	r1, [r4, #1564]
	bl	Mp4_Syntax_Parse
	ldr	r1, [r4, #1580]
	ldr	lr, [r4, #1564]
	ldr	r2, [r4, #1568]
	ldr	r3, [r4, #1572]
	mov	ip, r0, lsr #3
	and	r0, r0, #7
	rsb	r3, ip, r3
	add	lr, lr, ip
	str	r0, [r4, #1576]
	add	r2, ip, r2
	str	lr, [r4, #1564]
	str	r2, [r4, #1568]
	str	r3, [r4, #1572]
	ldr	r3, [r1, #852]
	cmp	r3, #1
	beq	.L1213
.L1129:
	ldr	r3, [r9, #3584]
	tst	r3, #288
	beq	.L1130
	bic	r3, r3, #432
	bic	r3, r3, #2
	str	r3, [r9, #3584]
	ldr	r3, [r4, #1612]
	cmp	r3, #2
	beq	.L1214
	cmp	r3, #1
	beq	.L1215
	cmp	r3, #4
	bne	.L1133
	ldrb	r3, [r4, #1432]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1135
	ldr	r3, [r4, #2096]
	cmp	r3, #0
	moveq	r3, #2
	movne	r3, #1
	str	r3, [r4, #2096]
.L1133:
	mov	r0, r6
	bl	Mp4_Frame_Atr
	cmn	r0, #1
	mov	r9, r0
	beq	.L1216
	ldr	r1, [r8, #3536]
	ldr	r0, [r4, #2480]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r8, #3544]
	mov	r10, r0
	ldr	r0, [r4, #2480]
	bl	FSP_GetFsImagePtr
	clz	r2, r10
	mov	r2, r2, lsr #5
	clz	r1, r0
	mov	ip, r0
	mov	r1, r1, lsr #5
	orrs	r9, r1, r2
	bne	.L1217
	ldrb	r0, [r4, #795]	@ zero_extendqisi2
	and	r3, r0, #253
	cmp	r3, #1
	beq	.L1218
	cmp	r0, #2
	beq	.L1219
.L1144:
	ldrb	r3, [r4, #1432]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1154
	ldr	r3, [r4, #2488]
	cmp	r3, #0
	movgt	r3, #2
	movle	r3, #1
	str	r3, [r4, #2488]
	ldr	r1, [r8, #3536]
	ldr	r3, [r8, #3544]
	cmp	r1, r3
	beq	.L1152
	mov	r2, #0
	ldr	r0, [r4, #2480]
	bl	FSP_SetRef
	ldr	r1, [r8, #3536]
	ldr	r0, [r4, #2480]
	bl	FSP_GetDisplay
	cmp	r0, #3
	beq	.L1209
	mov	r2, #0
	ldr	r1, [r8, #3536]
	ldr	r0, [r4, #2480]
	bl	FSP_SetDisplay
.L1209:
	ldr	r3, [r8, #3544]
.L1152:
	str	r3, [r8, #3536]
	mov	r2, #1
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	strb	r3, [r6]
	ldr	r3, [r8, #3548]
	ldr	r1, [r8, #3552]
	str	r3, [r8, #3540]
	str	r1, [r8, #3544]
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	strb	r3, [r6, #1]
	ldr	r3, [r4, #1524]
	str	r3, [r8, #3548]
	ldr	r0, [r4, #2480]
	bl	FSP_SetRef
	ldr	r1, [r8, #3536]
	ldr	r0, [r4, #2480]
	bl	FSP_RemovePmv
	ldrb	r3, [r4, #1432]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L1154
	ldrb	r10, [r4, #792]	@ zero_extendqisi2
	cmp	r10, #0
	beq	.L1220
.L1164:
	mov	r9, #0
.L1204:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1218:
	cmp	r2, #0
	bne	.L1142
	ldr	r2, [r4, #1580]
	ldr	r1, [r10, #200]
	ldr	r2, [r2]
	cmp	r1, r2
	bls	.L1144
	ldr	r1, [r8, #3552]
	mov	r2, r3
	ldr	r0, [r4, #2480]
	mvn	r9, #0
	str	r3, [fp, #-56]
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	r1, [r4, #1580]
	ldr	r3, [fp, #-56]
	ldr	ip, [r7, #68]
	ldr	r2, [r10, #200]
	mov	r0, r3
	ldr	r3, [r1]
	ldr	r1, .L1225+52
	blx	ip
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1135:
	sub	r2, r3, #2
	tst	r2, #253
	bne	.L1138
	ldr	r2, [r4, #2096]
	cmp	r2, #2
	beq	.L1171
.L1138:
	cmp	r3, #1
	streq	r3, [r4, #2096]
	b	.L1133
.L1213:
	ldr	r3, [r1, #892]
	cmp	r3, #0
	beq	.L1129
	b	.L1164
.L1212:
	ldr	r3, [r3, #892]
	cmp	r3, #1
	bne	.L1127
	add	r8, r6, #53248
	add	r9, r6, #16384
	ldr	r3, [r8, #3564]
	str	r3, [r9, #3584]
	b	.L1128
.L1154:
	mov	r2, r5
	mov	r1, #0
	mov	r0, r6
	bl	Mp4_Set_DecPar
	ldrb	ip, [r4, #1432]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L1221
	ldr	r3, [r4, #2096]
	cmp	r3, #0
	moveq	r3, #2
	movne	r3, #1
	str	r3, [r4, #2096]
	add	r3, r6, #1040
	str	r3, [r6, #1036]
	ldr	r1, [r4, #1580]
	ldr	r2, [r1, #884]
	cmp	r2, #1
	beq	.L1163
.L1162:
	cmp	r2, #2
	beq	.L1169
.L1163:
	cmp	ip, #0
	mov	r3, #0
	strb	r3, [r6, #1198]
	bne	.L1164
	ldr	r3, .L1225+56
	ldr	r5, [r3]
	cmp	r5, #0
	beq	.L1164
	sub	r2, fp, #44
	ldr	r0, [r4, #2480]
	mov	r3, #4
	mov	r1, #20
	str	ip, [r2, #-4]!
	mov	r9, ip
	blx	r5
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1221:
	sub	r3, ip, #2
	sub	r9, ip, #2
	clz	r9, r9
	tst	r3, #253
	mov	r9, r9, lsr #5
	bne	.L1158
	ldr	r3, [r4, #2096]
	cmp	r3, #2
	beq	.L1222
.L1158:
	cmp	ip, #1
	beq	.L1223
	add	r3, r6, #1040
	str	r3, [r6, #1036]
	ldr	r1, [r4, #1580]
	ldr	r2, [r1, #884]
	cmp	r2, #1
	bne	.L1162
	cmp	r3, #0
	moveq	r9, #0
	andne	r9, r9, #1
	cmp	r9, #0
	beq	.L1163
	mov	r3, #0
	str	r3, [r6, #1036]
	ldr	r1, [r8, #3552]
	mvn	r9, #0
	ldr	r0, [r4, #2480]
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1204
.L1214:
	ldrb	r3, [r4, #1432]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1133
.L1132:
	mov	r0, r6
	mvn	r9, #0
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1204
.L1215:
	ldrb	r3, [r4, #1432]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L1133
	b	.L1132
.L1220:
	ldr	r3, [r7, #68]
	mov	r0, #11
	ldr	r1, .L1225+60
	blx	r3
	ldr	r1, [r8, #3552]
	ldr	r0, [r4, #2480]
	bl	FSP_GetLogicFs
	ldr	r1, [r8, #3536]
	mov	r5, r0
	ldr	r0, [r4, #2480]
	bl	FSP_GetLogicFs
	ldr	ip, .L1225
	cmp	r5, #0
	cmpne	r0, #0
	beq	.L1224
	ldr	r2, [r5, #520]
	cmp	r2, #0
	beq	.L1166
	ldr	r1, [r5, #528]
	cmp	r1, #0
	beq	.L1166
	ldr	r1, [r5, #524]
	cmp	r1, #0
	beq	.L1166
	ldr	r0, [r0, #520]
	mov	r9, #0
	ldr	r3, [r1, #12]
	cmp	r0, #0
	ldr	r2, [r2, #12]
	ldr	lr, [r4, #1492]
	ldrne	r1, [r0, #12]
	mov	r0, r6
	ldr	ip, [r4, #1496]
	moveq	r1, r2
	str	r1, [sp]
	mov	r1, r5
	stmib	sp, {ip, lr}
	bl	Vdm_Mem_Move
	b	.L1204
.L1223:
	str	ip, [r4, #2096]
	add	r3, r6, #1040
	str	r3, [r6, #1036]
	ldr	r1, [r4, #1580]
	ldr	r2, [r1, #884]
	cmp	r2, #1
	beq	.L1160
	cmp	r2, #2
	beq	.L1169
.L1160:
	mov	r3, #0
	strb	r3, [r6, #1198]
	b	.L1164
.L1219:
	cmp	r2, #0
	bne	.L1145
	ldr	r3, [r4, #1580]
	ldr	r2, [r10, #200]
	ldr	r3, [r3]
	cmp	r2, r3
	bhi	.L1146
	cmp	r1, #0
	bne	.L1147
	ldr	r2, [ip, #200]
	cmp	r3, r2
	bcs	.L1144
.L1147:
	ldr	r1, [r8, #3552]
	mov	r2, #1
	ldr	r0, [r4, #2480]
	mvn	r9, #0
	str	ip, [fp, #-56]
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	r3, [r4, #1580]
	ldr	ip, [fp, #-56]
	mov	r0, #1
	ldr	r4, [r7, #68]
	ldr	r3, [r3]
	ldr	r2, [ip, #200]
	ldr	r1, .L1225+64
	blx	r4
	b	.L1204
.L1123:
	ldr	r3, .L1225
	mov	r0, #1
	ldr	r1, .L1225+68
	mvn	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1204
.L1146:
	ldr	r1, [r8, #3552]
	mov	r2, #1
	ldr	r0, [r4, #2480]
	mvn	r9, #0
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	r3, [r4, #1580]
	ldr	ip, [r7, #68]
	mov	r0, #1
	ldr	r2, [r10, #200]
	ldr	r3, [r3]
	ldr	r1, .L1225+72
	blx	ip
	b	.L1204
.L1171:
	mvn	r9, #0
	b	.L1204
.L1222:
	ldr	r1, [r8, #3552]
	mov	r2, #1
	ldr	r0, [r4, #2480]
	mvn	r9, #0
	bl	FSP_ClearLogicFs
	b	.L1204
.L1169:
	ldr	r2, [r1, #888]
	cmp	r2, #0
	andgt	r9, r9, #1
	movle	r9, #0
	cmp	r3, #0
	moveq	r3, #0
	andne	r3, r9, #1
	cmp	r3, #0
	beq	.L1163
	mov	r3, #0
	mov	r2, #1
	str	r3, [r6, #1036]
	mvn	r9, #0
	ldr	r1, [r8, #3552]
	ldr	r0, [r4, #2480]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #1580]
	mov	r0, r6
	ldr	r3, [r2, #888]
	add	r3, r3, r9
	str	r3, [r2, #888]
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1204
.L1142:
	ldr	r1, [r8, #3552]
	mov	r2, r3
	ldr	r0, [r4, #2480]
	mvn	r9, #0
	str	r3, [fp, #-56]
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	r3, [fp, #-56]
	ldr	ip, [r7, #68]
	ldr	r2, .L1225+76
	mov	r0, r3
	ldr	r1, .L1225+80
	movw	r3, #3969
	blx	ip
	b	.L1204
.L1211:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L1225+84
	blx	r3
	b	.L1204
.L1130:
	ldr	r3, [r7, #68]
	mov	r0, #2
	ldr	r1, .L1225+88
	mvn	r9, #0
	blx	r3
	b	.L1204
.L1217:
	ldr	ip, [r7, #68]
	movw	r3, #3955
	ldr	r2, .L1225+76
	mov	r0, #1
	ldr	r1, .L1225+92
	mvn	r9, #0
	blx	ip
	b	.L1204
.L1216:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L1225+96
	blx	r3
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1204
.L1145:
	ldr	r1, [r8, #3552]
	mov	r2, #1
	ldr	r0, [r4, #2480]
	mvn	r9, #0
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	ip, [r7, #68]
	movw	r3, #3989
	ldr	r2, .L1225+76
	ldr	r1, .L1225+80
	mov	r0, #1
	blx	ip
	b	.L1204
.L1166:
	ldr	r0, [r4, #2480]
	mov	r2, #1
	ldr	r1, [r8, #3552]
	mvn	r9, #0
	bl	FSP_ClearLogicFs
	ldr	r1, [r5, #524]
	ldr	r3, [r8, #3552]
	mov	r0, #0
	ldr	r2, [r8, #3536]
	str	r1, [sp, #12]
	ldr	r1, [r5, #528]
	ldr	r4, [r8, #3544]
	ldr	ip, [r7, #68]
	str	r1, [sp, #8]
	ldr	lr, [r5, #520]
	ldr	r1, .L1225+100
	stmia	sp, {r4, lr}
	blx	ip
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1204
.L1224:
	str	r0, [fp, #-56]
	mov	r2, #1
	ldr	r1, [r8, #3552]
	mvn	r9, #0
	ldr	r0, [r4, #2480]
	str	ip, [fp, #-60]
	bl	FSP_ClearLogicFs
	ldr	r3, [fp, #-56]
	movw	r2, #4194
	ldr	ip, [fp, #-60]
	mov	r0, r10
	ldr	r1, .L1225+104
	str	r3, [sp]
	mov	r3, r5
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1204
.L1226:
	.align	2
.L1225:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC73
	.word	.LC74
	.word	.LC75
	.word	.LC76
	.word	.LC77
	.word	.LC78
	.word	.LC79
	.word	.LC80
	.word	.LC81
	.word	.LC82
	.word	.LC83
	.word	.LC84
	.word	.LC93
	.word	g_event_report
	.word	.LC88
	.word	.LC95
	.word	.LC72
	.word	.LC94
	.word	.LANCHOR1+1356
	.word	.LC92
	.word	.LC85
	.word	.LC91
	.word	.LC87
	.word	.LC86
	.word	.LC90
	.word	.LC89
	UNWIND(.fnend)
	.size	MPEG4DEC_Decode, .-MPEG4DEC_Decode
	.align	2
	.global	MPEG4DEC_GetImageWidth
	.type	MPEG4DEC_GetImageWidth, %function
MPEG4DEC_GetImageWidth:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r0, r0, #20480
	ldr	r0, [r0, #1496]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG4DEC_GetImageWidth, .-MPEG4DEC_GetImageWidth
	.align	2
	.global	MPEG4DEC_GetImageHeight
	.type	MPEG4DEC_GetImageHeight, %function
MPEG4DEC_GetImageHeight:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r0, r0, #20480
	ldr	r0, [r0, #1492]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG4DEC_GetImageHeight, .-MPEG4DEC_GetImageHeight
	.global	sprite_trajectory_len
	.global	default_inter_matrix
	.global	default_intra_matrix
	.global	scan_tables
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	default_intra_matrix, %object
	.size	default_intra_matrix, 256
default_intra_matrix:
	.word	8
	.word	17
	.word	18
	.word	19
	.word	21
	.word	23
	.word	25
	.word	27
	.word	17
	.word	18
	.word	19
	.word	21
	.word	23
	.word	25
	.word	27
	.word	28
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	32
	.word	22
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	32
	.word	35
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	32
	.word	35
	.word	38
	.word	25
	.word	26
	.word	28
	.word	30
	.word	32
	.word	35
	.word	38
	.word	41
	.word	27
	.word	28
	.word	30
	.word	32
	.word	35
	.word	38
	.word	41
	.word	45
	.type	default_inter_matrix, %object
	.size	default_inter_matrix, 256
default_inter_matrix:
	.word	16
	.word	17
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	17
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	25
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	27
	.word	20
	.word	21
	.word	22
	.word	23
	.word	25
	.word	26
	.word	27
	.word	28
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	27
	.word	28
	.word	30
	.word	22
	.word	23
	.word	24
	.word	26
	.word	27
	.word	28
	.word	30
	.word	31
	.word	23
	.word	24
	.word	25
	.word	27
	.word	28
	.word	30
	.word	31
	.word	33
	.type	scan_tables, %object
	.size	scan_tables, 768
scan_tables:
	.word	0
	.word	1
	.word	8
	.word	16
	.word	9
	.word	2
	.word	3
	.word	10
	.word	17
	.word	24
	.word	32
	.word	25
	.word	18
	.word	11
	.word	4
	.word	5
	.word	12
	.word	19
	.word	26
	.word	33
	.word	40
	.word	48
	.word	41
	.word	34
	.word	27
	.word	20
	.word	13
	.word	6
	.word	7
	.word	14
	.word	21
	.word	28
	.word	35
	.word	42
	.word	49
	.word	56
	.word	57
	.word	50
	.word	43
	.word	36
	.word	29
	.word	22
	.word	15
	.word	23
	.word	30
	.word	37
	.word	44
	.word	51
	.word	58
	.word	59
	.word	52
	.word	45
	.word	38
	.word	31
	.word	39
	.word	46
	.word	53
	.word	60
	.word	61
	.word	54
	.word	47
	.word	55
	.word	62
	.word	63
	.word	0
	.word	1
	.word	2
	.word	3
	.word	8
	.word	9
	.word	16
	.word	17
	.word	10
	.word	11
	.word	4
	.word	5
	.word	6
	.word	7
	.word	15
	.word	14
	.word	13
	.word	12
	.word	19
	.word	18
	.word	24
	.word	25
	.word	32
	.word	33
	.word	26
	.word	27
	.word	20
	.word	21
	.word	22
	.word	23
	.word	28
	.word	29
	.word	30
	.word	31
	.word	34
	.word	35
	.word	40
	.word	41
	.word	48
	.word	49
	.word	42
	.word	43
	.word	36
	.word	37
	.word	38
	.word	39
	.word	44
	.word	45
	.word	46
	.word	47
	.word	50
	.word	51
	.word	56
	.word	57
	.word	58
	.word	59
	.word	52
	.word	53
	.word	54
	.word	55
	.word	60
	.word	61
	.word	62
	.word	63
	.word	0
	.word	8
	.word	16
	.word	24
	.word	1
	.word	9
	.word	2
	.word	10
	.word	17
	.word	25
	.word	32
	.word	40
	.word	48
	.word	56
	.word	57
	.word	49
	.word	41
	.word	33
	.word	26
	.word	18
	.word	3
	.word	11
	.word	4
	.word	12
	.word	19
	.word	27
	.word	34
	.word	42
	.word	50
	.word	58
	.word	35
	.word	43
	.word	51
	.word	59
	.word	20
	.word	28
	.word	5
	.word	13
	.word	6
	.word	14
	.word	21
	.word	29
	.word	36
	.word	44
	.word	52
	.word	60
	.word	37
	.word	45
	.word	53
	.word	61
	.word	22
	.word	30
	.word	7
	.word	15
	.word	23
	.word	31
	.word	38
	.word	46
	.word	54
	.word	62
	.word	39
	.word	47
	.word	55
	.word	63
	.type	__FUNCTION__.13899, %object
	.size	__FUNCTION__.13899, 15
__FUNCTION__.13899:
	.ascii	"Mp4_Set_DecPar\000"
	.space	1
	.type	__FUNCTION__.14033, %object
	.size	__FUNCTION__.14033, 23
__FUNCTION__.14033:
	.ascii	"Mp4_Syntax_Slice_NotSH\000"
	.space	1
	.type	__FUNCTION__.14047, %object
	.size	__FUNCTION__.14047, 20
__FUNCTION__.14047:
	.ascii	"Mp4_Syntax_Slice_SH\000"
	.type	__FUNCTION__.14062, %object
	.size	__FUNCTION__.14062, 14
__FUNCTION__.14062:
	.ascii	"MP4_Dec_Slice\000"
	.space	2
	.type	__FUNCTION__.13936, %object
	.size	__FUNCTION__.13936, 16
__FUNCTION__.13936:
	.ascii	"MPEG4DEC_Decode\000"
	.data
	.align	2
.LANCHOR0 = . + 0
	.type	sprite_trajectory_len, %object
	.size	sprite_trajectory_len, 120
sprite_trajectory_len:
	.word	0
	.byte	2
	.space	3
	.word	2
	.byte	3
	.space	3
	.word	3
	.byte	3
	.space	3
	.word	4
	.byte	3
	.space	3
	.word	5
	.byte	3
	.space	3
	.word	6
	.byte	3
	.space	3
	.word	14
	.byte	4
	.space	3
	.word	30
	.byte	5
	.space	3
	.word	62
	.byte	6
	.space	3
	.word	126
	.byte	7
	.space	3
	.word	254
	.byte	8
	.space	3
	.word	510
	.byte	9
	.space	3
	.word	1022
	.byte	10
	.space	3
	.word	2046
	.byte	11
	.space	3
	.word	4094
	.byte	12
	.space	3
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"fatal stream error --- GOP marker_bit = 0\012\000" )
	.space	1
.LC1:
	ASCII(.ascii	"DivX%4dBuild%4d%c\000" )
	.space	2
.LC2:
	ASCII(.ascii	"DivX%4db%4d%c\000" )
	.space	2
.LC3:
	ASCII(.ascii	"FFmpe%*[^b]b%4d\000" )
.LC4:
	ASCII(.ascii	"FFmpeg v%4d.%4d.%4d / libavcodec build: %d\000" )
	.space	1
.LC5:
	ASCII(.ascii	"Lavc%d.%d.%d\000" )
	.space	3
.LC6:
	ASCII(.ascii	"ffmpeg\000" )
	.space	1
.LC7:
	ASCII(.ascii	"XviD%4d\000" )
.LC8:
	ASCII(.ascii	"fatal stream error --- VOP marker_bit = 0\012\000" )
	.space	1
.LC9:
	ASCII(.ascii	"VDH doesn't support SPRITE_STATIC & SPRITE_GMC\012\000" )
.LC10:
	ASCII(.ascii	"VDH doesn't support SPRITE_STATIC\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"Unknown sprite_enable\012\000" )
	.space	1
.LC12:
	ASCII(.ascii	"ERROR: not_8_bit !=0\012\000" )
	.space	2
.LC13:
	ASCII(.ascii	"not support no complexity_estimation_disable\012\000" )
	.space	2
.LC14:
	ASCII(.ascii	"not support data_partitioned\012\000" )
	.space	2
.LC15:
	ASCII(.ascii	"not support newpred_enable\012\000" )
.LC16:
	ASCII(.ascii	"not support reduced_resolution_vop_eanble\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"not support scalability\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"warning! profile_and_level_indication=%#x, maybe HD" )
	ASCII(.ascii	" pic...\012\000" )
.LC19:
	ASCII(.ascii	"actual frame size(%dx%d) exeed max config(%dx%d)\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC20:
	ASCII(.ascii	"MPEG4 Frame_Init return err!\012\000" )
	.space	2
.LC21:
	ASCII(.ascii	"MPEG4 Frame_Init PmvNum=%d, FrameNum=%d, error!\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC22:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"ERROR: FSP_ConfigInstance fail!\012\000" )
	.space	3
.LC24:
	ASCII(.ascii	"dec_pts: %lld Usertag: %lld\012\000" )
	.space	3
.LC25:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"%s %d  abnormal!\012\000" )
	.space	2
.LC27:
	ASCII(.ascii	"CurFsID(fwd%d,%d,bwd%d) abnormal!(%p,%p,%p)\012\000" )
	.space	3
.LC28:
	ASCII(.ascii	"MPEG4DEC Init  -----\012\000" )
	.space	2
.LC29:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC30:
	ASCII(.ascii	"MPEG4DEC Init success -----\012\000" )
	.space	3
.LC31:
	ASCII(.ascii	"MPEG4 destroy\012\000" )
	.space	1
.LC32:
	ASCII(.ascii	"NVOP VDM copy process................\012\000" )
	.space	1
.LC33:
	ASCII(.ascii	"vop_coding_type = %d is not expected!\012\000" )
	.space	1
.LC34:
	ASCII(.ascii	" slice num %d is not extpected!\012\000" )
	.space	3
.LC35:
	ASCII(.ascii	" line %d packet is wrong!\012\000" )
	.space	1
.LC36:
	ASCII(.ascii	"pImg is null, pCurImg %#x pBwdImg %#x\012\000" )
	.space	1
.LC37:
	ASCII(.ascii	"FrameNum=%d\012\000" )
	.space	3
.LC38:
	ASCII(.ascii	"vop_coding_type=%d\012\000" )
.LC39:
	ASCII(.ascii	"line: %d pToQueImg is null!\012\000" )
	.space	3
.LC40:
	ASCII(.ascii	"CurImage %d is already out put\012\000" )
.LC41:
	ASCII(.ascii	"err(%d) > out_thr(%d)\012\000" )
	.space	1
.LC42:
	ASCII(.ascii	"StreamIDNum = %d > (%d)\012\000" )
	.space	3
.LC43:
	ASCII(.ascii	"Warning : vop slice num %d != 0\012\000" )
	.space	3
.LC44:
	ASCII(.ascii	"warning streamIDNum %d != 0 \012\000" )
	.space	2
.LC45:
	ASCII(.ascii	"vop ---%d\012\000" )
	.space	1
.LC46:
	ASCII(.ascii	"VDH doesn't support S_VOP\012\000" )
	.space	1
.LC47:
	ASCII(.ascii	"parse time_module_base stream not enough\012\000" )
	.space	2
.LC48:
	ASCII(.ascii	"module time base parse error, more than %d '1' was " )
	ASCII(.ascii	"found!\012\000" )
	.space	1
.LC49:
	ASCII(.ascii	"error: vop_coding_type = %d, but vop_fcode_forward " )
	ASCII(.ascii	"= 0\012\000" )
.LC50:
	ASCII(.ascii	"error: B vop, but vop_fcode_backward = 0\012\000" )
	.space	2
.LC51:
	ASCII(.ascii	"FUN: %s Invalid SlcNum = %d\012\000" )
	.space	3
.LC52:
	ASCII(.ascii	" pSlcPara->mb_start_num = %d, MbCnt = %d\012\000" )
	.space	2
.LC53:
	ASCII(.ascii	"pSlcPara->mb_start_num = %d, totalMbMin1 = %d\012\000" )
	.space	1
.LC54:
	ASCII(.ascii	"FUN: %s MP4_ClearCurPacket\012\000" )
.LC55:
	ASCII(.ascii	"pCtx is NUL!!!\012\000" )
.LC56:
	ASCII(.ascii	"Not find start code \012\000" )
	.space	2
.LC57:
	ASCII(.ascii	"VOS beginning ~ !\012\000" )
	.space	1
.LC58:
	ASCII(.ascii	"VO beginning ~ !\012\000" )
	.space	2
.LC59:
	ASCII(.ascii	"VOL beginning ~ !\012\000" )
	.space	1
.LC60:
	ASCII(.ascii	"GOP beginning ~ !\012\000" )
	.space	1
.LC61:
	ASCII(.ascii	"VOP beginning ~ !\012\000" )
	.space	1
.LC62:
	ASCII(.ascii	"BitUsed=%d\012\000" )
.LC63:
	ASCII(.ascii	"SHVOP beginning ~ !\012\000" )
	.space	3
.LC64:
	ASCII(.ascii	"USERDATA beginning ~ !\012\000" )
.LC65:
	ASCII(.ascii	"MPEG4 Frame_Init return err 3!\012\000" )
.LC66:
	ASCII(.ascii	"MPEG4 Frame_Init return err 1!\012\000" )
.LC67:
	ASCII(.ascii	"vop right\012\000" )
	.space	1
.LC68:
	ASCII(.ascii	"MPEG4 Frame_Init return err 2!\012\000" )
.LC69:
	ASCII(.ascii	"bits over boundary\012\000" )
.LC70:
	ASCII(.ascii	"pLastPacket->StreamPack[0].VirAddr is NULL!!\012\000" )
	.space	2
.LC71:
	ASCII(.ascii	"line %d MP4_Combine_HdPacket failed!!\012\000" )
	.space	1
.LC72:
	ASCII(.ascii	"bitsream para error -----\012\000" )
	.space	1
.LC73:
	ASCII(.ascii	"MPEG4DEC Decode -----\012\000" )
	.space	1
.LC74:
	ASCII(.ascii	"ptr_strm_para->PhyAddr0=0x%x\012\000" )
	.space	2
.LC75:
	ASCII(.ascii	"ptr_strm_para->Length0=%d\012\000" )
	.space	1
.LC76:
	ASCII(.ascii	"ptr_strm_para->IsLastSeg0=%d\012\000" )
	.space	2
.LC77:
	ASCII(.ascii	"ptr_strm_para->PhyAddr1=0x%x\012\000" )
	.space	2
.LC78:
	ASCII(.ascii	"ptr_strm_para->Length1=%d\012\000" )
	.space	1
.LC79:
	ASCII(.ascii	"ptr_strm_para->IsLastSeg1=%d\012\000" )
	.space	2
.LC80:
	ASCII(.ascii	"ptr_strm_para->Length0 = %d\012\000" )
	.space	3
.LC81:
	ASCII(.ascii	"byte0: = 0x%x\012\000" )
	.space	1
.LC82:
	ASCII(.ascii	"byte1: = 0x%x\012\000" )
	.space	1
.LC83:
	ASCII(.ascii	"byte2: = 0x%x\012\000" )
	.space	1
.LC84:
	ASCII(.ascii	"byte3: = 0x%x\012\000" )
	.space	1
.LC85:
	ASCII(.ascii	"MP4_Check_CurPacket failed!\012\000" )
	.space	3
.LC86:
	ASCII(.ascii	"Mp4_Frame_Atr return fail\012\000" )
	.space	1
.LC87:
	ASCII(.ascii	"fuction return value is null, %s %d!!\012\000" )
	.space	1
.LC88:
	ASCII(.ascii	"NVOP mem move--\012\000" )
	.space	3
.LC89:
	ASCII(.ascii	"line: %d pCur = %p, pFwd = %p\012\000" )
	.space	1
.LC90:
	ASCII(.ascii	"CurFsID(f%d,%d,b%d) abnormal!(%p,%p,%p)\012\000" )
	.space	3
.LC91:
	ASCII(.ascii	"bits not enought\012\000" )
	.space	2
.LC92:
	ASCII(.ascii	"fuction return value is null,%s %d!!\012\000" )
	.space	2
.LC93:
	ASCII(.ascii	"dec P/S frame, fwd ref err(%d) > ref_thr(%d)\012\000" )
	.space	2
.LC94:
	ASCII(.ascii	"dec B frame, bwd ref err(%d) > ref_thr(%d)\012\000" )
.LC95:
	ASCII(.ascii	"dec B frame, fwd ref err(%d) > ref_thr(%d)\012\000" )
	.bss
	.align	2
.LANCHOR2 = . + 0
	.type	MemArrange.13837, %object
	.size	MemArrange.13837, 564
MemArrange.13837:
	.space	564
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
