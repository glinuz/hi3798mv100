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
	strne	r0, [r3, #1040]
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
	add	r0, r0, #32
	ldr	r3, [r5, #48]
	blx	r3
	add	r0, r4, #19968
	ldr	r3, [r5, #48]
	mov	r2, #604
	mov	r1, #0
	add	r0, r0, #56
	blx	r3
	add	r0, r4, #20992
	ldr	r3, [r5, #48]
	mov	r2, #60
	mov	r1, #0
	add	r0, r0, #240
	blx	r3
	add	r0, r4, #21248
	ldr	r3, [r5, #48]
	mov	r2, #768
	mov	r1, #0
	add	r0, r0, #44
	blx	r3
	add	r0, r4, #22016
	ldr	r3, [r5, #48]
	mov	r2, #7
	mov	r1, #0
	add	r0, r0, #44
	blx	r3
	add	r2, r4, #19968
	add	r3, r4, #20224
	ldr	r1, .L61+4
	add	r0, r4, #20480
	add	r2, r2, #140
	add	r3, r3, #140
	mov	ip, #1
	strb	ip, [r0, #829]
	str	ip, [r0, #2144]
.L57:
	ldr	ip, [r1, #4]!
	str	ip, [r2, #4]!
	cmp	r2, r3
	bne	.L57
	add	r0, r4, #20480
	ldr	r2, .L61+8
	add	r0, r0, #140
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
	add	r0, r0, #72
	ldr	r3, [r5, #48]
	blx	r3
	add	r0, r4, #22016
	ldr	r3, [r5, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r0, #52
	blx	r3
	mov	r0, r4
	bl	Mp4_Syntax_Init
	add	r3, r4, #20480
	mov	r2, #0
	mov	r0, r2
	str	r2, [r3, #2124]
	str	r2, [r3, #2128]
	str	r2, [r3, #2132]
	str	r2, [r3, #2136]
	str	r2, [r3, #2096]
	str	r2, [r3, #1636]
	str	r2, [r3, #2080]
	str	r2, [r3, #2084]
	str	r2, [r3, #2088]
	str	r2, [r3, #2140]
	str	r2, [r3, #2100]
	str	r2, [r3, #2104]
	str	r2, [r3, #2108]
	str	r2, [r3, #2112]
	str	r2, [r3, #2116]
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
	add	r4, r4, #20480
	ldr	r1, .L119
	mov	r0, #1
	bl	dprint_vfmw
	strb	r6, [r4, #1582]
	sub	r0, fp, #60
	bl	BsPos
	ldr	r3, .L119+4
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L117
	ldr	r0, [r4, #2504]
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
	strb	r0, [r4, #814]
	sub	r0, fp, #60
	bl	BsGet
	mov	r3, #1
	strb	r3, [r4, #1582]
	strb	r0, [r4, #815]
	sub	r0, fp, #60
	bl	BsPos
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L120:
	.align	2
.L119:
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
	strb	r3, [r8, #1586]
	bhi	.L185
.L161:
	mov	r0, r7
.L179:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L122:
	add	r4, r0, #20480
	add	r7, r2, #1
	strb	r3, [r4, #1586]
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
	ldr	r2, [r8, #2116]
	add	r1, r1, #76
	ldr	r0, [r8, #2504]
	bl	Mp4_GetUserDataBuf
	ldr	r9, .L190
	cmp	r0, #0
	str	r0, [r8, #2140]
	beq	.L132
	cmn	r6, #-2147483648
	ldr	r3, [r9, #52]
	sub	r6, r6, #2
	str	r6, [r0, #1052]
	movpl	r2, #0
	ldr	r0, [r8, #2140]
	bmi	.L187
.L133:
	mov	r1, r5
	blx	r3
	ldr	r1, .L190+4
	ldr	r2, [r8, #1604]
	ldr	r3, [r8, #2140]
	ldr	ip, [r1]
	ldrd	r0, [r2, #56]
	add	r3, r3, #1056
	cmp	ip, #0
	strd	r0, [r3]
	beq	.L132
	movw	r3, #1064
	ldr	r2, [r8, #2140]
	mov	r1, #6
	ldr	r0, [r8, #2504]
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
	ldr	ip, [r9, #72]
	ldr	r1, .L190+8
	blx	ip
	ldr	r2, .L190
	cmp	r0, #1
	ble	.L189
.L135:
	ldr	r3, [fp, #-56]
	cmp	r0, #3
	ldr	r2, [fp, #-60]
	str	r3, [r8, #1564]
	movne	r3, #0
	ldreqsb	r3, [fp, #-61]
	str	r2, [r8, #1560]
	subeq	r3, r3, #112
	clzeq	r3, r3
	moveq	r3, r3, lsr #5
	str	r3, [r8, #1568]
.L136:
	ldr	r3, [r9, #72]
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
	ldr	ip, [r4, #72]
	sub	r3, fp, #52
	str	r6, [sp]
	mov	r0, r5
	ldr	r1, .L190+16
	blx	ip
	cmp	r0, #4
	beq	.L138
	str	r6, [sp]
	sub	r3, fp, #52
	ldr	ip, [r4, #72]
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
	ldr	r3, [r9, #76]
	mov	r2, #7
	ldr	r1, .L190+24
	mov	r0, r5
	blx	r3
	cmp	r0, #0
	movweq	r3, #4600
	streq	r3, [r8, #1576]
.L150:
	sub	r2, fp, #56
	ldr	r3, [r9, #72]
	ldr	r1, .L190+28
	mov	r0, r5
	blx	r3
	mov	r3, #0
	ldr	r1, [r8, #1560]
	strb	r3, [r8, #825]
	strb	r3, [r8, #826]
	strb	r3, [r8, #823]
	strb	r3, [r8, #822]
	strb	r3, [r8, #824]
	cmp	r0, #1
	ldreq	r2, [fp, #-56]
	ldrne	r2, [r8, #1572]
	streq	r2, [r8, #1572]
	cmp	r2, #0
	beq	.L143
	cmp	r1, r3
	strne	r3, [r8, #1560]
	strne	r3, [r8, #1564]
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
	strb	r3, [r8, #826]
.L145:
	sub	r2, r2, #1
	cmp	r2, #11
	bhi	.L146
.L154:
	mov	r3, #1
	strb	r3, [r8, #823]
.L146:
	ldr	r3, [r8, #1576]
	movw	r2, #4668
	sub	r3, r3, #1
	cmp	r3, r2
	movls	r3, #1
	strlsb	r3, [r8, #823]
	movw	r3, #498
	cmp	r0, r3
	movls	r3, #1
	strlsb	r3, [r8, #823]
	cmp	r1, #500
	bne	.L161
	ldr	r2, [r8, #1564]
	movw	r3, #413
	mov	r0, r7
	cmp	r2, r3
	moveq	r3, #1
	streqb	r3, [r8, #824]
	b	.L179
.L184:
	add	r4, r4, #20480
	mov	r3, #0
	movw	r0, #8224
	strb	r3, [r4, #1586]
	b	.L179
.L138:
	ldr	r3, [fp, #-56]
	str	r3, [r8, #1576]
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
	strcsb	r3, [r8, #825]
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
	strb	r3, [r4, #1586]
	b	.L179
.L186:
	mov	r2, ip
	b	.L180
.L189:
	sub	r3, fp, #61
	ldr	r1, .L190+32
	str	r3, [sp]
	mov	r0, r5
	ldr	ip, [r2, #72]
	sub	r3, fp, #56
	sub	r2, fp, #60
	blx	ip
	cmp	r0, #1
	ble	.L136
	b	.L135
.L182:
	strb	r2, [r8, #825]
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
	strb	r3, [r4, #828]
	strb	r3, [r4, #830]
	sub	r0, fp, #60
	strb	r3, [r4, #831]
	add	r5, r5, #16384
	strb	r3, [r4, #832]
	strb	r3, [r4, #833]
	strb	r3, [r4, #834]
	strb	r3, [r4, #835]
	str	r3, [r4, #840]
	str	r3, [r4, #844]
	str	r3, [r4, #848]
	strb	r6, [r4, #829]
	bl	BsSkip
	mov	r1, r6
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #3616]
	cmp	r0, #0
	streqb	r6, [r5, #3617]
	bne	.L207
.L194:
	mov	r1, #4
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r5, #3619]
	cmp	r6, #1
	beq	.L208
	mov	r6, #0
	sub	r0, fp, #60
	strb	r6, [r4, #1581]
	bl	BsPos
	ldr	r3, .L210
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r5, r0
	beq	.L198
	ldr	r0, [r4, #2504]
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
	strb	r0, [r5, #3617]
	sub	r0, fp, #60
	bl	BsGet
	strb	r0, [r5, #3618]
	b	.L194
.L208:
	mov	r1, r6
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #3620]
	cmp	r0, #0
	bne	.L209
.L197:
	mov	r3, #1
	sub	r0, fp, #60
	strb	r3, [r4, #1581]
	bl	BsPos
	cmp	r0, r7, asl #3
	bgt	.L199
	ldrb	r3, [r5, #3616]	@ zero_extendqisi2
	strb	r3, [r4, #828]
	ldrb	r3, [r5, #3617]	@ zero_extendqisi2
	strb	r3, [r4, #829]
	ldrb	r3, [r5, #3618]	@ zero_extendqisi2
	strb	r3, [r4, #830]
	ldrb	r3, [r5, #3619]	@ zero_extendqisi2
	strb	r3, [r4, #831]
	ldrb	r3, [r5, #3620]	@ zero_extendqisi2
	strb	r3, [r4, #832]
	ldrb	r3, [r5, #3621]	@ zero_extendqisi2
	strb	r3, [r4, #833]
	ldrb	r3, [r5, #3622]	@ zero_extendqisi2
	strb	r3, [r4, #834]
	ldrb	r3, [r5, #3623]	@ zero_extendqisi2
	strb	r3, [r4, #835]
	ldr	r3, [r5, #3628]
	str	r3, [r4, #840]
	ldr	r3, [r5, #3632]
	str	r3, [r4, #844]
	ldr	r3, [r5, #3636]
	str	r3, [r4, #848]
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
	strb	r0, [r5, #3621]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, r6
	strb	r0, [r5, #3622]
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #3623]
	cmp	r0, #0
	beq	.L197
	mov	r1, #8
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	str	r0, [r5, #3628]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	str	r0, [r5, #3632]
	sub	r0, fp, #60
	bl	BsGet
	str	r0, [r5, #3636]
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
	ldr	r0, [r5, #2504]
	add	r7, r6, #19968
	ldr	r8, .L374+4
	add	r7, r7, #56
	add	r4, r6, #16384
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #1276]
	moveq	r9, #8192
	ldrne	r9, [r3, #1280]
	streq	r9, [fp, #-80]
	strne	r0, [fp, #-80]
	sub	r0, fp, #76
	bl	BsInit
	ldr	r3, [r8, #48]
	mov	r2, #604
	mov	r1, #0
	mov	r0, r7
	strb	r1, [r5, #813]
	blx	r3
	mov	r1, #32
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #8
	strb	r0, [r4, #3644]
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #3668]
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3640]
	cmp	r0, #0
	ldreqb	r3, [r5, #829]	@ zero_extendqisi2
	streqb	r3, [r4, #3641]
	bne	.L363
.L215:
	mov	r1, #4
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3643]
	cmp	r0, #15
	beq	.L364
.L216:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3645]
	cmp	r0, #0
	moveq	r3, #1
	streqb	r3, [r4, #3646]
	bne	.L365
.L221:
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r10, r0
	strb	r10, [r4, #3648]
	cmp	r10, #0
	subne	r0, fp, #76
	movne	r6, #0
	beq	.L366
.L357:
	strb	r6, [r5, #1583]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L292
	ldr	r0, [r5, #2504]
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
	str	r0, [r4, #3712]
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
	str	r3, [r4, #3708]
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3649]
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
	str	r0, [r4, #3720]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	mov	r1, #13
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #3724]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L342
	ldr	r3, [r4, #3720]
	cmp	r3, #31
	bls	.L240
	ldr	r2, [r4, #3724]
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
	strb	r6, [r5, #1583]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L292
.L336:
	ldr	r0, [r5, #2504]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #103
	blx	ip
	mov	r0, r4
	b	.L327
.L342:
	ldr	r1, .L374+12
	mov	r0, #1
	str	r3, [fp, #-80]
.L344:
	bl	dprint_vfmw
	ldr	r3, [fp, #-80]
	sub	r0, fp, #76
	strb	r3, [r5, #1583]
	bl	BsPos
	ldr	r2, .L374+8
	ldr	ip, [r2]
	cmp	ip, #0
	mov	r4, r0
	beq	.L292
	ldr	r3, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r5, #2504]
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
	strb	r10, [r4, #3646]
	cmp	r10, #1
	beq	.L218
	mov	r3, #0
	sub	r0, fp, #76
	strb	r3, [r5, #1583]
	bl	BsPos
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L363:
	mov	r1, #4
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #3641]
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #3642]
	b	.L215
.L218:
	mov	r1, r10
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	strb	r0, [r4, #3647]
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4, #3680]
	beq	.L221
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3684]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3688]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3692]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #11
	str	r0, [r4, #3696]
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3700]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L359
	mov	r1, #15
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r10
	str	r0, [r4, #3704]
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	str	r3, [fp, #-84]
	bne	.L221
	ldr	r1, .L374+12
	mov	r0, r10
	bl	dprint_vfmw
	ldr	r6, [fp, #-84]
	sub	r0, fp, #76
	strb	r6, [r5, #1583]
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
	str	r0, [r4, #3672]
	sub	r0, fp, #76
	bl	BsGet
	str	r0, [r4, #3676]
	b	.L216
.L229:
	mov	r3, #1
	b	.L329
.L359:
	str	r3, [fp, #-80]
	mov	r0, r10
	ldr	r1, .L374+12
	b	.L344
.L367:
	ldr	r1, [r4, #3708]
	sub	r0, fp, #76
	bl	BsGet
	str	r0, [r4, #3716]
	b	.L236
.L241:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #3650]
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r3, r0
	strb	r3, [r4, #3651]
	cmp	r3, #0
	bne	.L243
	ldr	r2, .L374+8
	strb	r3, [r5, #1583]
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L243
	mov	r2, r3
	mov	r1, #102
	ldr	r0, [r5, #2504]
	blx	ip
.L243:
	ldrb	r3, [r4, #3641]	@ zero_extendqisi2
	sub	r0, fp, #76
	cmp	r3, #1
	moveq	r1, #1
	movne	r1, #2
	bl	BsGet
	uxtb	r9, r0
	strb	r9, [r4, #3652]
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
	strb	r6, [r5, #1583]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L253
	ldr	r0, [r5, #2504]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
.L253:
	ldr	r1, .L374+20
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	b	.L327
.L370:
	mov	r1, #6
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, r9
	strb	r0, [r4, #3664]
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #3665]
	sub	r0, fp, #76
	bl	BsGet
	ldrb	r3, [r4, #3652]	@ zero_extendqisi2
	cmp	r3, #2
	strb	r0, [r4, #3666]
	bne	.L361
.L246:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3653]
	cmp	r0, #0
	beq	.L254
	mov	r6, #0
	sub	r0, fp, #76
	strb	r6, [r5, #1583]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L255
	ldr	r0, [r5, #2504]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
.L255:
	ldr	r1, .L374+24
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	b	.L327
.L369:
	cmp	r9, #2
	beq	.L370
.L361:
	mov	r6, #0
	sub	r0, fp, #76
	strb	r6, [r5, #1583]
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, r6
	mov	r4, r0
	beq	.L252
	ldr	r0, [r5, #2504]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
.L252:
	ldr	r1, .L374+28
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	b	.L327
.L254:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3654]
	cmp	r0, #0
	bne	.L371
.L257:
	ldrb	r3, [r4, #3641]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r3, #0
	streqb	r3, [r4, #3657]
	beq	.L264
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #3657]
.L264:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r9, r0
	strb	r9, [r4, #3658]
	cmp	r9, #0
	ldreq	r1, .L374+32
	beq	.L353
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #3659]
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r10, r0
	strb	r10, [r4, #3660]
	cmp	r10, #0
	ldrne	r1, .L374+36
	bne	.L355
	ldrb	r3, [r4, #3641]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L267
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r9, r0
	strb	r9, [r4, #3661]
	cmp	r9, #0
	beq	.L268
	ldr	r1, .L374+40
	mov	r0, #1
	bl	dprint_vfmw
	strb	r10, [r5, #1583]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r4, r0
	beq	.L292
	ldr	r0, [r5, #2504]
	mov	r3, r10
	mov	r2, r10
	mov	r1, #102
	blx	ip
	mov	r0, r4
	b	.L327
.L368:
	strb	r10, [r5, #1583]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r4, r0
	beq	.L248
	ldr	r0, [r5, #2504]
	mov	r3, r10
	mov	r2, r10
	mov	r1, #102
	blx	ip
.L248:
	ldr	r1, .L374+44
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	b	.L327
.L371:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3655]
	cmp	r0, #0
	bne	.L372
	add	r3, r6, #19968
	add	r1, r6, #20224
	ldr	r2, .L374+48
	add	r3, r3, #140
	add	r1, r1, #140
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
	strb	r0, [r4, #3656]
	cmp	r0, #0
	bne	.L272
	add	r3, r6, #20224
	add	r1, r6, #20480
	ldr	r2, .L374+52
	add	r3, r3, #140
	add	r1, r1, #140
.L261:
	ldr	r0, [r2, #4]!
	str	r0, [r3, #4]!
	cmp	r3, r1
	bne	.L261
	b	.L257
.L373:
	ldr	r1, .L374+56
.L353:
	mov	r0, #1
	bl	dprint_vfmw
	strb	r9, [r5, #1583]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L374+8
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r4, r0
	beq	.L292
	ldr	r0, [r5, #2504]
	mov	r3, r9
	mov	r2, r9
	mov	r1, #102
	blx	ip
	mov	r0, r4
	b	.L327
.L268:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3662]
	cmp	r0, #0
	bne	.L373
.L269:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3663]
	cmp	r0, #0
	beq	.L270
	ldr	r1, .L374+60
.L355:
	mov	r0, #1
	mov	r6, #0
	bl	dprint_vfmw
	sub	r0, fp, #76
	b	.L357
.L372:
	add	r1, r6, #19968
	sub	r0, fp, #76
	add	r1, r1, #144
	bl	Bs_Get_Matrix
	b	.L259
.L272:
	add	r1, r6, #20224
	sub	r0, fp, #76
	add	r1, r1, #144
	bl	Bs_Get_Matrix
	b	.L257
.L267:
	strb	r10, [r4, #3661]
	strb	r10, [r4, #3662]
	b	.L269
.L270:
	add	r0, r6, #20480
	mov	r3, #1
	ldr	ip, [r8, #52]
	mov	r1, r7
	mov	r2, #604
	strb	r3, [r5, #1583]
	strb	r3, [r5, #813]
	add	r0, r0, #148
	strb	r3, [r5, #1584]
	blx	ip
	sub	r0, fp, #76
	bl	BsPos
	b	.L327
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
	.word	.LC14
	.word	.LC15
	.word	.LC9
	.word	.LANCHOR1-4
	.word	.LANCHOR1+252
	.word	.LC16
	.word	.LC17
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
	str	r3, [r5, #836]
	sub	r0, fp, #52
	bl	BsSkip
	sub	r0, fp, #52
	mov	r1, #8
	bl	BsGet
	sub	r3, r0, #1
	sub	r2, r0, #240
	cmp	r2, #5
	cmphi	r3, #2
	str	r0, [r4, #3624]
	movls	r3, #1
	movhi	r3, #0
	cmp	r0, #8
	orreq	r3, r3, #1
	cmp	r3, #0
	strne	r0, [r5, #836]
	movne	r3, #1
	strneb	r3, [r5, #1580]
	bne	.L378
	cmp	r0, #0
	beq	.L383
	ldr	r2, .L384
	strb	r3, [r5, #1580]
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L378
	ldr	r0, [r5, #2504]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L378:
	sub	r0, fp, #52
	bl	BsPos
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L383:
	mov	r2, r0
	ldr	r1, .L384+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #1
	strb	r3, [r5, #1580]
	ldr	r3, [r4, #3624]
	str	r3, [r5, #836]
	b	.L378
.L385:
	.align	2
.L384:
	.word	g_event_report
	.word	.LC18
	UNWIND(.fnend)
	.size	Mp4_Syntax_Vos, .-Mp4_Syntax_Vos
	.align	2
	.global	Frame_Init
	.type	Frame_Init, %function
Frame_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 48
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
	add	r4, r0, #20480
	mov	r6, r2
	mov	r8, r0
	ldr	r3, [r4, #1604]
	mov	r5, r1
	ldr	r7, [r3, #708]
	ldr	r2, [r3, #28]
	cmp	r7, #0
	bne	.L387
	cmp	r2, #25
	beq	.L420
.L413:
	mov	r9, #5
	mov	r10, #3
	mov	r2, #2
	str	r2, [fp, #-88]
.L388:
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L399
	ldr	r2, .L423
	ldr	ip, [r2, #64]
	cmp	ip, #0
	beq	.L401
	ldr	r1, [r3, #24]
	mov	lr, #0
	ldr	r0, [r3, #20]
	mov	r2, #40
	ldr	r3, [r4, #2504]
	ldr	r7, .L423+4
	str	lr, [sp, #8]
	mov	lr, #2
	str	r3, [sp, #12]
	mov	r3, r6
	str	r2, [sp, #4]
	mov	r2, r5
	str	r7, [sp, #16]
	str	lr, [sp]
	blx	ip
	cmp	r0, #0
	bne	.L401
	ldr	r2, [r7, #1248]
	ldr	r3, [r7, #1080]
	cmp	r2, #1
	ble	.L403
	cmp	r3, #2
	ble	.L403
	ldr	r0, [r7, #8]
	str	r0, [r4, #1624]
	bl	MEM_Phy2Vir
	str	r0, [r4, #1612]
.L399:
	ldr	r3, .L423+8
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r2, [r4, #1604]
	str	r6, [fp, #-48]
	mov	r3, #0
	str	r5, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r6, [fp, #-64]
	cmp	r2, #24
	str	r5, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L405
	ldrb	r3, [fp, #-88]	@ zero_extendqisi2
	strb	r10, [fp, #-73]
	strb	r9, [fp, #-71]
.L410:
	sub	r1, fp, #76
	ldr	r0, [r4, #2504]
	strb	r3, [fp, #-70]
	mov	r5, #0
	strb	r5, [fp, #-74]
	bl	FSP_ConfigInstance
	subs	r7, r0, #0
	bne	.L406
	ldr	r1, [r4, #1604]
	sub	r3, fp, #80
	ldr	r0, [r4, #2504]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L421
	cmp	r0, #0
	moveq	r3, #1
	streq	r3, [r4, #2508]
	bne	.L408
.L416:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L387:
	cmp	r2, #25
	bne	.L413
	ldr	r2, [r3, #668]
	ldr	r1, [r3, #672]
	cmp	r5, r2
	bhi	.L395
	cmp	r6, r1
	bhi	.L395
	ldr	r1, [r3, #696]
	ldr	r2, [r3, #692]
	cmp	r1, #0
	add	r10, r2, #2
	moveq	r2, #1
	streq	r2, [fp, #-88]
	bne	.L422
	ldr	r9, [r3, #716]
	b	.L388
.L405:
	mov	r3, #1
	strb	r9, [fp, #-71]
	strb	r3, [fp, #-73]
	b	.L410
.L420:
	ldr	r0, [r3, #668]
	cmp	r0, r1
	ldrcc	r1, [r3, #672]
	bcc	.L390
	ldr	r2, [r3, #672]
	cmp	r2, r6
	mov	r1, r2
	bcc	.L390
	ldr	r1, [r4, #2508]
	cmp	r1, #1
	beq	.L416
	ldr	ip, [r3, #696]
	ldr	r1, [r3, #692]
	cmp	ip, #0
	add	r10, r1, #2
	moveq	r1, #1
	streq	r1, [fp, #-88]
	beq	.L393
	add	r1, r1, #1
	cmp	r1, #17
	movge	r1, #17
	str	r1, [fp, #-88]
.L393:
	mov	r6, r2
	mov	r5, r0
	ldr	r9, [r3, #716]
	b	.L388
.L422:
	add	r2, r2, #1
	ldr	r9, [r3, #716]
	cmp	r2, #17
	movge	r2, #17
	str	r2, [fp, #-88]
	b	.L388
.L421:
	add	r0, r8, #22528
	add	r0, r0, #104
	bl	ResetVoQueue
.L408:
	ldr	r1, .L423+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r7, #0
	mov	r3, #0
	str	r3, [r4, #2508]
	b	.L416
.L403:
	ldr	r1, .L423+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L416
.L401:
	ldr	r1, .L423+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L416
.L406:
	mov	r0, r5
	ldr	r1, .L423+24
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L416
.L395:
	mov	r3, r6
	str	r1, [sp, #4]
	str	r2, [sp]
.L419:
	mov	r2, r5
	ldr	r1, .L423+28
	mov	r0, #0
	mvn	r7, #0
	bl	dprint_vfmw
	b	.L416
.L390:
	str	r1, [sp, #4]
	mov	r3, r6
	str	r0, [sp]
	b	.L419
.L424:
	.align	2
.L423:
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR2
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC22
	.word	.LC21
	.word	.LC20
	.word	.LC23
	.word	.LC19
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
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r5, r0, #20480
	str	r0, [fp, #-52]
	mov	r4, r1
	ldrb	r3, [r5, #855]	@ zero_extendqisi2
	cmp	r3, #15
	beq	.L451
	sub	r2, r3, #2
	cmp	r2, #1
	bls	.L442
	sub	r2, r3, #4
	cmp	r2, #1
	bls	.L443
	cmp	r3, #1
	moveq	r3, #5
	movne	r3, #0
	str	r3, [fp, #-48]
.L428:
	mov	r1, r4
	ldr	r0, [r5, #2504]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L426
	ldr	r3, [r5, #1520]
	ldr	r1, [fp, #-48]
	str	r3, [r4, #112]
	ldr	r3, [r5, #1516]
	str	r3, [r4, #116]
	bl	SetAspectRatio
	ldr	r3, [r5, #1604]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L452
.L434:
	ldr	r3, [r5, #1520]
	str	r3, [r4, #120]
	ldr	r3, [r5, #1516]
	str	r3, [r4, #124]
	ldr	r3, [r5, #1520]
	mov	r3, r3, lsr #1
	str	r3, [r4, #128]
	ldr	r3, [r5, #1516]
	mov	r3, r3, lsr #1
	str	r3, [r4, #132]
	ldr	r3, [r5, #2084]
	str	r3, [r4, #168]
	ldr	r3, [r5, #2080]
	str	r3, [r4, #164]
	ldrb	r2, [r5, #833]	@ zero_extendqisi2
	ldrb	r3, [r5, #1456]	@ zero_extendqisi2
	ldrb	r0, [r5, #1461]	@ zero_extendqisi2
	ldrb	r1, [r5, #862]	@ zero_extendqisi2
	and	r3, r3, #3
	mov	r2, r2, asl #5
	orr	r3, r3, r0, asl #12
	and	r1, r1, #1
	uxtb	r2, r2
	orr	r3, r3, #3072
	orr	r3, r3, r2
	orr	r1, r3, r1, asl #8
	ldr	r3, [fp, #-48]
	orr	r7, r1, r3, asl #14
	str	r7, [r4, #108]
	ldr	r3, [r5, #1520]
	cmp	r3, #1920
	orrls	r7, r7, #131072
	bichi	r7, r7, #131072
	str	r7, [r4, #108]
	ldr	r3, [r5, #1604]
	ldr	r3, [r3, #592]
	cmp	r3, #1
	movne	r3, #0
	beq	.L453
.L437:
	orr	r7, r3, r7
	ldr	r3, [fp, #-52]
	str	r7, [r4, #108]
	mov	ip, #0
	ldrb	r2, [r5, #1456]	@ zero_extendqisi2
	add	r6, r3, #22528
	add	r6, r6, #72
	add	r3, r4, #552
	and	r2, r2, #3
	add	r0, r4, #584
	str	r2, [r4, #100]
	mov	r1, ip
	ldrb	r2, [r5, #1456]	@ zero_extendqisi2
	str	ip, [r4, #96]
	and	r2, r2, #3
	str	r2, [r4, #104]
.L438:
	ldr	r2, [r6, #4]!
	mov	r8, #0
	str	r2, [r3, #8]!
	cmp	r3, r0
	str	r1, [r3, #4]
	bne	.L438
	ldr	r3, [r5, #1604]
	add	ip, r4, #608
	add	lr, r4, #624
	mvn	r6, #0
	mvn	r7, #0
	ldr	r1, .L454
	ldrd	r2, [r3, #56]
	mov	r0, #29
	strd	r2, [ip, #-8]
	ldr	r3, [r5, #1604]
	ldrd	r2, [r3, #72]
	strd	r2, [ip]
	ldr	r3, [r5, #1604]
	ldrd	r2, [r3, #88]
	strd	r2, [lr, #-8]
	ldr	r3, [r5, #1604]
	ldr	r3, [r3, #104]
	str	r3, [r4, #8]
	ldr	r3, [r5, #1604]
	ldr	r3, [r3, #108]
	str	r3, [r4, #12]
	ldr	r3, [r5, #1604]
	ldr	r3, [r3, #112]
	str	r3, [r4, #16]
	ldr	r3, [r5, #1604]
	ldr	r3, [r3, #128]
	str	r3, [r4, #20]
	ldr	r3, [r5, #1604]
	strd	r6, [r3, #56]
	ldrd	r6, [ip]
	ldrd	r2, [ip, #-8]
	strd	r6, [sp]
	bl	dprint_vfmw
	mov	r3, #8
	mov	r2, #2
	strb	r8, [r4, #364]
	str	r2, [r4, #376]
	mov	r2, #1
	strb	r8, [r4, #371]
	str	r3, [r4, #380]
	str	r3, [r4, #388]
	str	r3, [r4, #384]
	strb	r2, [r4, #369]
.L426:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L453:
	tst	r7, #131072
	movne	r3, #0
	moveq	r3, #262144
	b	.L437
.L443:
	mov	r3, #2
	str	r3, [fp, #-48]
	b	.L428
.L442:
	mov	r3, #1
	str	r3, [fp, #-48]
	b	.L428
.L452:
	ldr	r1, [r4, #116]
	add	r9, r4, #512
	ldr	ip, [r4, #140]
	add	r1, r1, #15
	ldr	lr, [r4, #44]
	mov	r3, r1, lsr #4
	mov	r1, ip, asl #4
	add	r10, lr, ip
	mul	r1, r3, r1
	add	r8, r1, lr
	str	r8, [r4, #48]
	ldrd	r2, [r9, #-8]
	add	ip, r8, ip
	mov	r0, r8
	adds	r6, r2, r1
	adc	r7, r3, r1, asr #31
	strd	r6, [r9]
	str	r8, [r4, #32]
	add	r8, r4, #528
	str	lr, [r4, #28]
	str	ip, [r4, #40]
	str	r10, [r4, #36]
	strd	r2, [r8, #-8]
	str	lr, [r4, #60]
	bl	MEM_Phy2Vir
	mov	r3, #0
	str	r3, [r4, #532]
	ldr	r3, [r4, #32]
	str	r3, [r4, #64]
	str	r0, [r4, #528]
	b	.L434
.L451:
	ldr	r2, [r5, #884]
	ldr	r1, [r5, #888]
	cmp	r2, #0
	cmpne	r1, #0
	moveq	r3, #0
	streq	r3, [fp, #-48]
	beq	.L428
	ldr	r3, [r5, #936]
	mul	r1, r3, r1
	cmp	r1, #0
	beq	.L440
	ldr	r0, [r5, #932]
	mov	r2, r2, asl #10
	mul	r0, r0, r2
	bl	__aeabi_uidiv
	movw	r3, #1364
	cmp	r0, r3
	movw	r3, #1819
	rsble	r7, r0, #1360
	subgt	r7, r0, #1360
	addle	r7, r7, #5
	subgt	r7, r7, #5
	cmp	r0, r3
	rsble	r0, r0, #1808
	subgt	r0, r0, #1808
	addle	r0, r0, #12
	subgt	r0, r0, #12
	cmp	r7, r0
	movlt	r3, #1
	movge	r3, #2
	str	r3, [fp, #-48]
	b	.L428
.L440:
	str	r1, [fp, #-48]
	b	.L428
.L455:
	.align	2
.L454:
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
	ldrb	r3, [r4, #1456]	@ zero_extendqisi2
	cmp	r3, #4
	strb	r3, [r0, #2]
	ldreq	r2, .L466
	moveq	r3, #300
	ldr	r0, [r4, #2504]
	mlaeq	r3, r3, r0, r2
	ldreq	r2, [r3, #12]
	addeq	r2, r2, #1
	streq	r2, [r3, #12]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #2532]
	blt	.L459
	ldr	r3, [r4, #2508]
	cmp	r3, #0
	beq	.L459
	mov	r1, r0
	mov	r0, r5
	bl	Mp4_RecordFrameAtr
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #2088]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L459:
	ldr	r1, .L466+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L461
	ldr	r1, [r6, #560]
	add	r7, r6, #576
	ldr	r0, [r4, #2504]
	add	r8, r6, #592
	bl	FreeUsdByDec
	ldr	r1, [r6, #568]
	ldr	r0, [r4, #2504]
	add	r9, r6, #560
	bl	FreeUsdByDec
	ldr	r1, [r6, #576]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	ldr	r1, [r6, #584]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r9]
	strd	r0, [r7, #-8]
	strd	r0, [r7]
	strd	r0, [r8, #-8]
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2504]
	str	r3, [r4, #2516]
	str	r3, [r4, #2524]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2524]
	str	r0, [r4, #2516]
.L461:
	add	r1, r5, #22528
	mov	r3, #0
	add	r1, r1, #104
	str	r3, [r4, #2512]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearNotInVoQueue
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L467:
	.align	2
.L466:
	.word	g_VfmwChanStat
	.word	.LC25
	UNWIND(.fnend)
	.size	Mp4_Frame_Atr, .-Mp4_Frame_Atr
	.align	2
	.global	Mp4_Set_DecPar
	.type	Mp4_Set_DecPar, %function
Mp4_Set_DecPar:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 48
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
	ldr	r3, .L496
	mov	r6, r2
	mov	r4, r0
	mov	r2, #48
	sub	r0, fp, #92
	ldr	r3, [r3, #48]
	mov	r1, #0
	blx	r3
	ldrb	r3, [r6, #6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L493
	ldr	r3, [r6, #92]
	add	r5, r4, #20480
	mov	r1, #0
	bic	r3, r3, #3
	str	r3, [r4, #1208]
	ldr	r3, [r5, #1592]
	and	r2, r3, #3
	str	r3, [r4, #1220]
	ldr	r3, [r5, #1588]
	str	r3, [r4, #1056]
	ldr	r3, [r5, #1600]
	add	r2, r3, r2, lsl #3
	str	r2, [r4, #1228]
	ldr	r2, [r5, #1596]
	rsb	r3, r3, r2, lsl #3
	str	r3, [r4, #1236]
	ldr	r3, [r6, #92]
	str	r3, [r4, #1224]
	ldr	r3, [r6, #80]
	str	r3, [r4, #1060]
	ldr	r3, [r6, #92]
	and	r3, r3, #3
	mov	r3, r3, asl #3
	str	r3, [r4, #1232]
	ldr	r3, [r6, #96]
	str	r1, [r4, #1216]
	mov	r3, r3, asl #3
	str	r3, [r4, #1240]
.L470:
	ldr	r2, [r5, #1520]
	add	r7, r4, #1040
	add	r6, r4, #12288
	add	r3, r2, #15
	mov	r3, r3, lsr #4
	strh	r3, [r7, #164]	@ movhi
	ldr	r1, [r5, #1516]
	add	r3, r1, #15
	mov	r3, r3, lsr #4
	strh	r3, [r7, #166]	@ movhi
	ldrb	r3, [r5, #1456]	@ zero_extendqisi2
	strb	r3, [r4, #1040]
	ldrb	r0, [r5, #866]	@ zero_extendqisi2
	strb	r0, [r4, #1041]
	ldrb	r0, [r5, #821]	@ zero_extendqisi2
	strb	r0, [r4, #1042]
	ldrb	r0, [r5, #1463]	@ zero_extendqisi2
	strb	r0, [r4, #1043]
	ldrb	r0, [r5, #1468]	@ zero_extendqisi2
	strb	r0, [r4, #1044]
	ldrb	r0, [r5, #1467]	@ zero_extendqisi2
	strb	r0, [r4, #1045]
	ldr	r0, [r5, #920]
	strb	r0, [r4, #1046]
	ldrb	r0, [r5, #1464]	@ zero_extendqisi2
	strb	r0, [r4, #1047]
	ldrb	r0, [r5, #1465]	@ zero_extendqisi2
	strb	r0, [r4, #1048]
	ldrb	r0, [r5, #1460]	@ zero_extendqisi2
	strb	r0, [r4, #1049]
	ldrb	r0, [r5, #871]	@ zero_extendqisi2
	strb	r0, [r4, #1050]
	ldrb	r0, [r5, #869]	@ zero_extendqisi2
	strb	r0, [r4, #1051]
	ldrb	r0, [r5, #1458]	@ zero_extendqisi2
	strb	r0, [r4, #1052]
	ldrb	r0, [r5, #1462]	@ zero_extendqisi2
	strb	r0, [r4, #1053]
	ldrb	r0, [r5, #1461]	@ zero_extendqisi2
	strb	r0, [r4, #1054]
	ldrb	r0, [r5, #862]	@ zero_extendqisi2
	strb	r0, [r4, #1055]
	ldr	r0, [r5, #1552]
	str	r0, [r4, #1244]
	ldr	r0, [r5, #1548]
	str	r0, [r4, #1248]
	ldrb	r0, [r5, #824]	@ zero_extendqisi2
	strb	r0, [r4, #1194]
	ldrb	r0, [r5, #823]	@ zero_extendqisi2
	strb	r0, [r4, #1193]
	ldrb	r0, [r5, #822]	@ zero_extendqisi2
	strb	r0, [r4, #1192]
	ldrb	r0, [r5, #825]	@ zero_extendqisi2
	strb	r0, [r4, #1195]
	ldrb	r0, [r5, #826]	@ zero_extendqisi2
	strb	r0, [r4, #1196]
	ldrb	r0, [r5, #827]	@ zero_extendqisi2
	strb	r0, [r4, #1197]
	str	r1, [r6, #3372]
	str	r2, [r6, #3376]
	ldrb	r2, [r5, #864]	@ zero_extendqisi2
	cmp	r2, #2
	cmpeq	r3, #3
	str	r2, [r6, #3380]
	moveq	r3, #1
	movne	r3, #0
	beq	.L494
	ldrb	r2, [r5, #877]	@ zero_extendqisi2
	str	r3, [r6, #3384]
	str	r3, [r6, #3392]
	str	r2, [r6, #3388]
	str	r3, [r6, #3396]
	str	r3, [r6, #3400]
	str	r3, [r6, #3404]
	str	r3, [r6, #3408]
	str	r3, [r6, #3412]
	str	r3, [r6, #3416]
	str	r3, [r6, #3420]
.L472:
	ldr	r1, [r5, #2532]
	ldr	r0, [r5, #2504]
	bl	FSP_GetLogicFs
	ldr	r1, [r5, #2516]
	mov	r8, r0
	ldr	r0, [r5, #2504]
	bl	FSP_GetLogicFs
	ldr	r1, [r5, #2524]
	mov	r9, r0
	ldr	r0, [r5, #2504]
	bl	FSP_GetLogicFs
	cmp	r8, #0
	cmpne	r9, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L495
	ldr	r1, [r8, #640]
	ldr	ip, [r8, #648]
	cmp	r1, #0
	ldr	lr, [r8, #644]
	beq	.L476
	cmp	ip, #0
	beq	.L476
	cmp	lr, #0
	beq	.L476
	ldr	r3, [r8, #652]
	add	ip, r4, #21248
	add	r1, r4, #1056
	add	r2, r4, #21504
	add	r4, r4, #1120
	add	ip, ip, #168
	str	r3, [r4, #148]
	add	r1, r1, #7
	ldr	r3, [r0, #652]
	add	r2, r2, #168
	str	r3, [r4, #152]
	ldr	r3, [r0, #640]
	cmp	r3, #0
	ldreq	r3, [r8, #640]
	ldr	r3, [r3, #16]
	str	r3, [r4, #144]
	ldr	r3, [r9, #640]
	cmp	r3, #0
	ldreq	r3, [r8, #640]
	ldr	r3, [r3, #16]
	str	r3, [r4, #140]
	ldr	r3, [r8, #640]
	ldr	r3, [r3, #16]
	str	r3, [r4, #132]
	add	r4, r4, #7
	ldr	r3, [r0, #648]
	cmp	r3, #0
	ldreq	r3, [r8, #648]
	ldr	r3, [r3, #16]
	str	r3, [r6, #3332]
	ldr	r3, [r9, #648]
	cmp	r3, #0
	ldreq	r3, [r8, #648]
	ldr	r3, [r3, #16]
	str	r3, [r6, #3336]
	ldr	r3, [r8, #648]
	ldr	r0, [r3, #16]
	mov	r3, #0
	str	r0, [r6, #3340]
	ldr	r0, [r8, #644]
	ldr	r0, [r0, #16]
	str	r0, [r6, #3344]
	ldr	r0, [r8, #644]
	ldr	r0, [r0, #32]
	str	r0, [r6, #3356]
	ldr	r0, [r8, #644]
	ldr	r0, [r0, #76]
	str	r0, [r6, #3360]
	ldr	r0, [r8, #644]
	ldr	r0, [r0, #60]
	str	r0, [r6, #3364]
	ldr	r0, [r8, #644]
	ldr	r0, [r0, #64]
	str	r0, [r6, #3368]
.L487:
	ldr	r0, [ip, #4]!
	add	r3, r3, #1
	cmp	r3, #64
	strb	r0, [r1, #1]!
	ldr	r0, [r2, #4]!
	strb	r0, [r4, #1]!
	bne	.L487
	ldrh	r1, [r7, #164]
	ldrh	r3, [r7, #166]
	ldr	r2, [r5, #1604]
	mul	r3, r3, r1
	str	r3, [r5, #1488]
	ldr	r3, [r2, #592]
	str	r3, [r6, #3424]
	ldr	r3, [r2, #632]
	str	r3, [r6, #3428]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L495:
	movw	r3, #3468
	ldr	r2, .L496+4
	ldr	r1, .L496+8
	mov	r0, #0
	bl	dprint_vfmw
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L493:
	add	r5, r4, #20480
	mov	r0, #0
	ldr	r2, [r5, #1592]
	bic	r1, r2, #3
	str	r1, [r4, #1220]
	str	r2, [r4, #1208]
	and	r2, r2, #3
	ldr	r1, [r5, #1588]
	str	r1, [r4, #1056]
	ldr	r1, [r5, #1600]
	add	r2, r1, r2, lsl #3
	str	r2, [r4, #1228]
	ldr	r2, [r5, #1596]
	str	r3, [r4, #1216]
	str	r0, [r4, #1224]
	rsb	r1, r1, r2, lsl #3
	str	r0, [r4, #1232]
	str	r1, [r4, #1236]
	str	r0, [r4, #1240]
	b	.L470
.L476:
	ldr	r4, [r5, #2524]
	mov	r0, #0
	ldr	r3, [r5, #2532]
	ldr	r2, [r5, #2516]
	stmib	sp, {r1, ip, lr}
	str	r4, [sp]
	ldr	r1, .L496+12
	bl	dprint_vfmw
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L494:
	add	r0, r4, #21248
	sub	r1, fp, #92
	add	r0, r0, #44
	bl	MP4_GetGMCparam
	sub	r0, fp, #64
	ldrb	r10, [r5, #877]	@ zero_extendqisi2
	ldmia	r0, {r0, r1, r2, r3}
	ldr	r9, [fp, #-92]
	ldr	r8, [fp, #-76]
	ldr	lr, [fp, #-72]
	ldr	ip, [fp, #-68]
	str	r3, [r6, #3416]
	ldr	r3, [fp, #-48]
	str	r10, [r6, #3388]
	str	r9, [r6, #3384]
	str	r8, [r6, #3392]
	str	lr, [r6, #3396]
	str	ip, [r6, #3400]
	str	r0, [r6, #3404]
	str	r1, [r6, #3408]
	str	r2, [r6, #3412]
	str	r3, [r6, #3420]
	b	.L472
.L497:
	.align	2
.L496:
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
	ldr	r3, [r0, #1636]
	cmp	r3, #2
	ldr	r3, [r0, #1604]
	beq	.L510
.L500:
	ldr	r3, [r3, #8]
	cmp	r3, #2
	beq	.L503
	cmp	r3, #1
	beq	.L509
	cmp	r3, #4
	movne	r3, #0
.L509:
	str	r3, [r0, #1636]
	ldmfd	sp, {fp, sp, pc}
.L510:
	ldr	r2, [r3, #8]
	cmp	r2, #2
	movne	r2, #1
	strne	r2, [r0, #2144]
	bne	.L500
.L503:
	mov	r3, #2
	str	r3, [r0, #1636]
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
	beq	.L514
	mov	r0, #0
.L513:
	mov	r3, r3, lsr #1
	add	r0, r0, #1
	tst	r3, #1
	bne	.L513
	cmp	r0, #0
	addne	r0, r0, #1
	ldmfd	sp, {fp, sp, pc}
.L514:
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
	mov	r4, r0
	mov	r6, r1
	mov	r0, #2
	ldr	r1, .L530
	bl	dprint_vfmw
	cmp	r6, #0
	cmpne	r4, #0
	beq	.L524
	ldr	r3, [r6, #608]
	ldr	r7, .L530+4
	cmp	r3, #1
	beq	.L527
.L522:
	add	r5, r4, #20480
	ldr	r3, [r7, #48]
	movw	r2, #23032
	mov	r1, #0
	ldr	r8, [r5, #2496]
	mov	r0, r4
	blx	r3
	add	r0, r4, #22528
	add	r0, r0, #104
	mov	r7, #0
	str	r8, [r5, #2496]
	bl	ResetVoQueue
	str	r6, [r5, #1604]
	mov	r0, r4
	str	r7, [r6, #872]
	bl	VCTRL_GetChanIDByCtx
	ldr	r8, .L530+4
	cmn	r0, #1
	str	r0, [r5, #2504]
	beq	.L528
	mov	r0, r4
	bl	Global_Data_Init
	subs	r7, r0, #0
	bne	.L524
	mov	r3, #1
	str	r7, [r5, #2120]
	ldr	r1, .L530+8
	mov	r0, #2
	str	r3, [r5, #2092]
	bl	dprint_vfmw
	ldr	r6, [r6, #608]
	cmp	r6, #1
	beq	.L529
.L521:
	mov	r0, r7
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L529:
	add	r0, r4, #21248
	ldr	r3, [r8, #52]
	sub	r1, fp, #804
	mov	r2, #768
	add	r0, r0, #44
	blx	r3
	add	r0, r4, #19968
	ldr	r3, [r8, #52]
	sub	r1, fp, #788
	mov	r2, #24
	add	r0, r0, #32
	blx	r3
	add	r0, r4, #19968
	ldr	r3, [r8, #52]
	sub	r1, fp, #764
	mov	r2, #604
	add	r0, r0, #56
	blx	r3
	add	r0, r4, #22016
	ldr	r3, [r8, #52]
	mov	r2, #7
	sub	r1, fp, #812
	add	r0, r0, #44
	blx	r3
	mov	r0, r7
	strb	r6, [r5, #2541]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L527:
	add	r1, r4, #21248
	ldr	r3, [r7, #52]
	add	r1, r1, #44
	mov	r2, #768
	sub	r0, fp, #804
	blx	r3
	add	r1, r4, #19968
	ldr	r3, [r7, #52]
	add	r1, r1, #32
	mov	r2, #24
	sub	r0, fp, #788
	blx	r3
	add	r1, r4, #19968
	ldr	r3, [r7, #52]
	add	r1, r1, #56
	mov	r2, #604
	sub	r0, fp, #764
	blx	r3
	add	r1, r4, #22016
	ldr	r3, [r7, #52]
	add	r1, r1, #44
	mov	r2, #7
	sub	r0, fp, #812
	blx	r3
	b	.L522
.L524:
	mvn	r7, #0
	mov	r0, r7
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L528:
	mov	r0, r7
	ldr	r1, .L530+12
	bl	dprint_vfmw
	mvn	r7, #19
	b	.L521
.L531:
	.align	2
.L530:
	.word	.LC28
	.word	vfmw_Osal_Func_Ptr_S
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
	mov	r4, r0
	add	r4, r4, #20480
	ldr	r1, .L533
	mov	r0, #2
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #2092]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L534:
	.align	2
.L533:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r5, r1
	beq	.L537
	add	r4, r0, #20480
	mov	r2, #0
	ldr	r0, [r4, #2504]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L538
	add	r5, r0, #576
	add	r8, r0, #560
	ldr	r1, [r0, #560]
	mov	r6, #0
	ldr	r0, [r4, #2504]
	mov	r7, #0
	bl	FreeUsdByDec
	strd	r6, [r8]
	ldr	r0, [r4, #2504]
	ldr	r1, [r5, #-8]
	bl	FreeUsdByDec
	strd	r6, [r5, #-8]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L538:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L537:
	mvn	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
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
	add	r4, r0, #20480
	ldr	r1, [r4, #2532]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r2, [r4, #1520]
	cmp	r2, #0
	beq	.L541
	ldr	r1, [r4, #1516]
	cmp	r1, #0
	beq	.L541
	ldr	r0, [r3, #112]
	ldr	r1, [r3, #116]
	cmp	r0, r2
	movcs	r0, r2
	str	r0, [r3, #120]
	ldr	r2, [r4, #1516]
	cmp	r1, r2
	movcs	r1, r2
	str	r1, [r3, #124]
	b	.L542
.L541:
	ldr	r1, [r3, #112]
	ldr	r2, [r3, #116]
	str	r1, [r3, #120]
	str	r2, [r3, #124]
.L542:
	mov	r2, #2
	mov	r1, #0
	str	r2, [r3, #376]
	mov	r0, #1
	mov	r2, #8
	strb	r1, [r3, #364]
	strb	r1, [r3, #371]
	str	r2, [r3, #380]
	str	r2, [r3, #388]
	str	r2, [r3, #384]
	strb	r0, [r3, #369]
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r4, r0, #20480
	mov	r6, r0
	ldr	r3, [r4, #1604]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L567
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L567
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_GetDisplay
	cmp	r0, #2
	mov	r8, r0
	beq	.L566
.L567:
	add	r5, r6, #22528
	add	r5, r5, #104
.L557:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	GetVoLastImageID
.L566:
	mov	r0, r6
	add	r5, r6, #22528
	bl	MP4_SetImgFormat
	add	r5, r5, #104
	mov	r3, #1
	ldr	r1, [r7, #156]
	mov	r2, r3
	str	r3, [r7, #188]
	ldr	r0, [r4, #2504]
	bl	FSP_SetDisplay
	ldr	r0, [r4, #2504]
	mov	r1, r8
	str	r7, [sp]
	mov	r3, r5
	mov	r2, r6
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L554
	mov	r0, r6
	ldr	r1, [r7, #156]
	add	r10, r7, #560
	bl	MPEG4DEC_RecycleImage
	add	r6, r7, #576
	ldr	r1, [r7, #560]
	ldr	r0, [r4, #2504]
	mov	r8, #0
	bl	FreeUsdByDec
	mov	r9, #0
	strd	r8, [r10]
	ldr	r0, [r4, #2504]
	ldr	r1, [r7, #568]
	bl	FreeUsdByDec
	strd	r8, [r6, #-8]
	b	.L557
.L554:
	ldr	r3, [r7, #160]
	cmp	r3, #100
	bhi	.L557
	ldr	r3, [r7, #112]
	cmp	r3, #31
	bls	.L557
	ldr	r3, [r7, #116]
	cmp	r3, #31
	bls	.L557
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
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
	ldr	r0, [r4, #2504]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #2504]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L574
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L574:
	add	r1, r5, #22528
	ldr	r0, [r4, #2504]
	add	r1, r1, #104
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
	ldr	r3, [r0, #3612]
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
	ldr	r1, [r1, #20]
	mov	r5, r0
	bl	SM_ReleaseStreamSeg
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L579
	ldrb	r3, [r4, #86]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L580
.L579:
	ldr	r3, .L581
	mov	r0, r4
	mov	r2, #160
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L580:
	mov	r0, r5
	ldr	r1, [r4, #100]
	bl	SM_ReleaseStreamSeg
	b	.L579
.L582:
	.align	2
.L581:
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
	ldr	r6, [r8, #3604]
	ldr	r7, [r3, #2504]
	cmp	r6, #0
	addgt	r5, r0, #15872
	movgt	r4, #0
	addgt	r5, r5, #16
	ble	.L586
.L585:
	add	r4, r4, #1
	ldr	r1, [r5, #4]!
	mov	r0, r7
	bl	SM_ReleaseStreamSeg
	cmp	r6, r4
	bne	.L585
.L586:
	mov	r3, #0
	str	r3, [r8, #3604]
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
	mov	r4, r0
	mov	r5, r1
	mov	r0, #2
	ldr	r1, .L602
	mov	r10, r3
	ldr	r8, [fp, #8]
	add	r7, r4, #20480
	str	r2, [fp, #-48]
	ldr	r9, [fp, #4]
	ldr	r6, [fp, #12]
	bl	dprint_vfmw
	ldr	r2, [fp, #-48]
	add	r0, r4, #12288
	add	r6, r6, #15
	movw	r1, #1206
	movw	ip, #1204
	cmp	r8, #1920
	str	r2, [r4, #1252]
	add	r2, r8, #15
	str	r10, [r0, #3344]
	mov	r6, r6, lsr #4
	ldr	lr, [r5, #652]
	mov	r3, r2, lsr #4
	str	r9, [r4, #1260]
	str	lr, [r4, #1268]
	strh	r3, [r4, ip]	@ movhi
	strh	r6, [r4, r1]	@ movhi
	add	r1, r4, #1040
	ldr	r3, [r5, #644]
	ldr	r3, [r3, #32]
	str	r3, [r0, #3356]
	ldr	r3, [r5, #644]
	ldr	r3, [r3, #76]
	str	r3, [r0, #3360]
	ldr	r3, [r5, #644]
	ldr	r3, [r3, #60]
	str	r3, [r0, #3364]
	ldr	r3, [r5, #644]
	ldr	r3, [r3, #64]
	str	r3, [r0, #3368]
	ldr	r3, [r7, #1604]
	ldr	r2, [r3, #592]
	str	r2, [r0, #3424]
	ldr	r2, [r3, #632]
	str	r2, [r0, #3428]
	movls	r2, #1
	movhi	r2, #0
	str	r2, [r0, #3432]
	str	r1, [r4, #1036]
	ldr	r2, [r3, #864]
	cmp	r2, #1
	beq	.L600
	cmp	r2, #2
	beq	.L601
.L593:
	mov	r3, #1
	strb	r3, [r4, #1198]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L600:
	cmp	r1, #0
	beq	.L593
	mov	r2, #0
	str	r2, [r4, #1036]
	ldr	r0, [r7, #2504]
	ldr	r1, [r7, #2532]
	bl	FSP_SetDisplay
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	MP4_ClearCurPic
.L601:
	ldr	r3, [r3, #868]
	cmp	r1, #0
	cmpne	r3, #0
	ble	.L593
	mov	r2, #0
	str	r2, [r4, #1036]
	ldr	r0, [r7, #2504]
	ldr	r1, [r7, #2532]
	bl	FSP_SetDisplay
	ldr	r2, [r7, #1604]
	mov	r0, r4
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	MP4_ClearCurPic
.L603:
	.align	2
.L602:
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
	add	r3, r0, #15872
	add	r4, r0, #22784
	add	r3, r3, #16
	ldr	r2, [r2, #1604]
	add	r4, r4, #244
	ldr	r0, [r2, #832]
	cmp	r0, #1
	beq	.L606
.L605:
	ldr	r6, .L607
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
.L606:
	ldr	r2, [r2, #872]
	cmp	r2, #1
	bne	.L605
	ldr	ip, .L607
	mov	r0, r3
	mov	r1, r4
	mov	r2, #4
	ldr	r3, [ip, #52]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L608:
	.align	2
.L607:
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
	add	r3, r0, #16384
	add	r1, r0, #20480
	ldrb	r3, [r3, #3659]	@ zero_extendqisi2
	ldrb	r2, [r1, #752]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L617
	add	r0, r0, #12288
	ldrb	r3, [r0, #3600]	@ zero_extendqisi2
	tst	r3, #120
	bne	.L617
	add	r0, r0, #3600
	ldrh	r0, [r0]
	ubfx	r0, r0, #7, #4
	add	r0, r0, #17
	cmp	r2, #3
	ldrls	pc, [pc, r2, asl #2]
	b	.L611
.L613:
	.word	.L618
	.word	.L614
	.word	.L615
	.word	.L614
.L617:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L618:
	mov	r3, #17
.L612:
	subs	r0, r3, r0
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L615:
	ldrb	r3, [r1, #761]	@ zero_extendqisi2
	ldrb	r2, [r1, #760]	@ zero_extendqisi2
	cmp	r3, r2
	movcc	r3, r2
	cmp	r3, #1
	addgt	r3, r3, #16
	movle	r3, #18
	b	.L612
.L614:
	ldrb	r3, [r1, #760]	@ zero_extendqisi2
	add	r3, r3, #16
	b	.L612
.L611:
	ldr	r1, .L621
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L622:
	.align	2
.L621:
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
	ldr	ip, [r2, #3608]
	ldr	r2, [r3, #3328]
	tst	ip, #2
	beq	.L624
	ldrb	lr, [r3, #3446]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L638
	cmp	lr, #0
	bne	.L632
	ldrb	ip, [r3, #3526]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L632
	mov	ip, r2, asl #5
	sub	r2, ip, r2, asl #2
	sub	r2, r2, #28
	add	r2, r0, r2
	ldr	r0, [r2, #12]
	cmp	r0, #0
	bne	.L634
	ldr	r0, [r2, #20]
	cmp	r0, #0
	bne	.L634
	ldr	ip, [r2, #8]
	add	r1, ip, r1, lsl #3
	str	r1, [r2, #8]
	ldr	r1, [r3, #3532]
	str	r1, [r2, #20]
	ldr	r3, [r3, #3536]
	mov	r3, r3, asl #3
	str	r3, [r2, #12]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L638:
	sub	r4, r2, #1
	movw	ip, #510
	cmp	r4, ip
	bhi	.L639
	mov	ip, r2, asl #5
	sub	r2, ip, r2, asl #2
	sub	r2, r2, #28
	add	r2, r0, r2
	ldr	r0, [r2, #12]
	cmp	r0, #0
	beq	.L628
	ldr	ip, [r2, #20]
	cmp	ip, #0
	beq	.L628
	ldr	r3, [r3, #3452]
	cmp	ip, r3
	bhi	.L640
	add	r1, r0, r1, lsl #3
	mov	r0, #0
	str	r1, [r2, #12]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L628:
	ldr	ip, [r3, #3452]
	ldr	r0, [r2, #16]
	cmp	ip, r0
	movls	r1, r1, asl #3
	strls	r1, [r2, #12]
	ldrls	r3, [r3, #3452]
	movhi	r0, #0
	ldrhi	r3, [r2, #8]
	movls	r0, #0
	addhi	r1, r3, r1, lsl #3
	strls	r3, [r2, #20]
	strhi	r1, [r2, #8]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L632:
	movw	r2, #4833
	ldr	r1, .L641
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L634:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L624:
	movw	r2, #4839
	ldr	r1, .L641
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L639:
	mov	r0, lr
	ldr	r1, .L641+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L640:
	mov	r0, lr
	movw	r2, #4809
	ldr	r1, .L641
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L642:
	.align	2
.L641:
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
	ldrb	r3, [r2, #3600]	@ zero_extendqisi2
	tst	r3, #1
	bne	.L644
	mov	r3, #2
	strb	r3, [r0, #821]
	ldr	r3, [r1]
	mov	r3, r3, lsr #8
	sub	r3, r3, #128
	cmp	r3, #3
	ldrlsb	r3, [r2, #3600]	@ zero_extendqisi2
	orrls	r3, r3, #2
	strlsb	r3, [r2, #3600]
.L645:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L644:
	mov	r2, #196
	mov	r3, #1
	str	r2, [r1]
	strb	r3, [r0, #821]
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
	ldr	r6, [r3, #2060]
	ldr	r7, [r3, #2504]
	cmp	r6, #64
	movge	r6, #64
	cmp	r6, #0
	addgt	r5, r0, #22272
	movgt	r4, #0
	addgt	r5, r5, #8
	ble	.L649
.L648:
	add	r4, r4, #1
	ldr	r1, [r5, #4]!
	mov	r0, r7
	bl	SM_ReleaseStreamSeg
	cmp	r4, r6
	bne	.L648
.L649:
	ldr	r3, .L651
	add	r0, r8, #22016
	add	r0, r0, #104
	mov	r2, #440
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L652:
	.align	2
.L651:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MP4_Clean_HeadPacket, .-MP4_Clean_HeadPacket
	.align	2
	.global	MPEG4DEC_VDMPostProc
	.type	MPEG4DEC_VDMPostProc, %function
MPEG4DEC_VDMPostProc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	ldr	r3, .L704
	movw	r2, #14684
	mov	r5, r0
	mov	r6, r1
	add	r0, r0, #1040
	ldr	r3, [r3, #48]
	mov	r1, #0
	blx	r3
	add	r4, r5, #20480
	mov	r0, r5
	bl	MP4_ClearCurPic
	mov	r0, r5
	bl	MP4_Clean_HeadPacket
	str	r6, [r4, #2088]
	ldr	r1, [r4, #2532]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #2524]
	mov	r6, r0
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	cmp	r6, #0
	mov	r8, r0
	beq	.L699
	ldr	r2, [r4, #2096]
	mov	r0, #11
	ldr	r1, .L704+4
	bl	dprint_vfmw
	ldrb	r2, [r4, #1456]	@ zero_extendqisi2
	ldr	r1, .L704+8
	mov	r0, #11
	bl	dprint_vfmw
	ldr	r3, [r4, #2088]
	add	r1, r5, #22528
	add	r1, r1, #76
	str	r3, [r6, #160]
	ldr	r0, [r4, #2504]
	bl	Mp4_ClearUsrData
	ldrb	r2, [r4, #1456]	@ zero_extendqisi2
	cmp	r2, #2
	beq	.L656
	ldr	r2, [r4, #1604]
	ldr	r3, [r4, #2088]
	ldr	r1, [r2]
	cmp	r3, r1
	bhi	.L700
.L656:
	mov	r3, #0
	mov	r0, r5
	str	r3, [r4, #2144]
	bl	MP4_SetImgFormat
	ldr	r3, [r4, #1604]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L659
	ldrb	ip, [r4, #1456]	@ zero_extendqisi2
	cmp	ip, #2
	beq	.L660
	ldr	r3, [r4, #2512]
	cmp	r3, #1
	ble	.L661
	ldr	r1, [r4, #2516]
	ldr	r7, [r4, #2520]
	str	r1, [r4, #2536]
	ldrb	r6, [r5]	@ zero_extendqisi2
.L662:
	cmn	r1, #1
	beq	.L664
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L701
	cmp	r7, #0
	cmpeq	r6, #4
	beq	.L702
	ldr	r2, [r8, #160]
	cmp	r2, #0
	beq	.L667
	ldr	r3, [r4, #1604]
	ldr	r1, [r3, #4]
	cmp	r2, r1
	bhi	.L668
.L667:
	ldr	r1, [r4, #2536]
	ldr	r0, [r4, #2504]
	bl	FSP_GetLogicFs
	subs	r7, r0, #0
	beq	.L703
	cmp	r6, #2
	beq	.L670
	ldrsb	r3, [r7, #1]
	cmp	r3, #2
	beq	.L670
	ldr	r2, [r4, #2536]
	mov	r0, #11
	ldr	r1, .L704+12
	bl	dprint_vfmw
.L664:
	ldr	r3, [r4, #2096]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #2096]
.L697:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L659:
	ldr	r1, [r4, #2532]
	ldrb	r6, [r4, #1456]	@ zero_extendqisi2
	ldr	r7, [r4, #1548]
	str	r1, [r4, #2536]
	b	.L662
.L660:
	ldr	r1, [r4, #2532]
	cmp	r8, #0
	ldr	r7, [r4, #1548]
	str	r1, [r4, #2536]
	beq	.L675
	add	r0, r8, #608
	add	r6, r6, #608
	ldrd	r8, [r0, #-8]
	ldrd	r2, [r6, #-8]
	cmp	r8, r2
	sbcs	lr, r9, r3
	bge	.L675
	strd	r8, [r6, #-8]
	mov	r6, ip
	strd	r2, [r0, #-8]
	ldr	r1, [r4, #2536]
	b	.L662
.L668:
	ldr	r3, .L704+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L673
	str	r2, [fp, #-44]
	mov	r3, #8
	str	r1, [fp, #-40]
	sub	r2, fp, #44
	mov	r1, #104
	ldr	r0, [r4, #2504]
	blx	ip
	ldr	r3, [r4, #1604]
	ldr	r2, [r8, #160]
	ldr	r1, [r3, #4]
.L673:
	mov	r3, r1
	add	r9, r8, #560
	ldr	r1, .L704+20
	mov	r0, #1
	bl	dprint_vfmw
	add	r5, r8, #576
	ldr	r1, [r8, #560]
	mov	r6, #0
	ldr	r0, [r4, #2504]
	mov	r7, #0
	bl	FreeUsdByDec
	strd	r6, [r9]
	ldr	r1, [r8, #568]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	strd	r6, [r5, #-8]
	mov	r2, #0
	ldr	r1, [r4, #2536]
	ldr	r0, [r4, #2504]
	bl	FSP_SetDisplay
	b	.L664
.L670:
	mov	r2, #1
	ldr	r1, [r4, #2536]
	ldr	r0, [r4, #2504]
	bl	FSP_SetDisplay
	add	r3, r5, #22528
	ldr	r0, [r4, #2504]
	add	r3, r3, #104
	str	r8, [sp]
	mov	r2, r5
	mov	r1, #2
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L664
	ldr	r1, [r8, #560]
	add	r9, r8, #560
	ldr	r0, [r4, #2504]
	add	r5, r8, #576
	mov	r6, #0
	mov	r7, #0
.L698:
	bl	FreeUsdByDec
	strd	r6, [r9]
	ldr	r1, [r8, #568]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	strd	r6, [r5, #-8]
	mov	r2, #0
	ldr	r0, [r4, #2504]
	ldr	r1, [r4, #2536]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L697
.L661:
	mvn	r3, #0
	str	r3, [r4, #2536]
	b	.L664
.L675:
	mov	r6, ip
	b	.L662
.L700:
	ldr	r1, .L704+16
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L657
	str	r3, [fp, #-44]
	mov	r1, #104
	ldr	lr, [r2]
	mov	r3, #8
	sub	r2, fp, #44
	ldr	r0, [r4, #2504]
	str	lr, [fp, #-40]
	blx	ip
.L657:
	mov	r6, #1
	ldr	r1, [r4, #2524]
	str	r6, [r4, #2144]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L658
	ldr	r1, [r5, #560]
	add	r7, r5, #576
	ldr	r0, [r4, #2504]
	add	r8, r5, #592
	bl	FreeUsdByDec
	ldr	r1, [r5, #568]
	ldr	r0, [r4, #2504]
	add	r9, r5, #560
	bl	FreeUsdByDec
	ldr	r1, [r5, #576]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	ldr	r1, [r5, #584]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	mov	r0, #0
	mov	r1, #0
	mov	r2, r6
	strd	r0, [r9]
	strd	r0, [r7, #-8]
	strd	r0, [r7]
	strd	r0, [r8, #-8]
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r2, r6
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2504]
	str	r3, [r4, #2516]
	str	r3, [r4, #2524]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2524]
	str	r0, [r4, #2516]
.L658:
	mov	r3, #0
	mvn	r0, #0
	str	r3, [r4, #2512]
	b	.L697
.L702:
	ldr	r2, [r4, #2504]
	mov	r1, #300
	ldr	r3, .L704+24
	add	r9, r8, #560
	add	r5, r8, #576
	mov	r6, #0
	mov	r0, r2
	mov	r7, #0
	mla	r3, r1, r2, r3
	ldr	r2, [r3, #16]
	add	r2, r2, #1
	str	r2, [r3, #16]
	ldr	r1, [r8, #560]
	b	.L698
.L699:
	mov	r3, r0
	mov	r2, r6
	mov	r0, r6
	ldr	r1, .L704+28
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L697
.L701:
	movw	r2, #4541
	ldr	r1, .L704+32
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L697
.L703:
	add	r5, r8, #576
	add	r6, r8, #560
	ldr	r1, [r8, #560]
	mov	r9, #0
	ldr	r0, [r4, #2504]
	mov	r8, #0
	bl	FreeUsdByDec
	strd	r8, [r6]
	ldr	r1, [r5, #-8]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	strd	r8, [r5, #-8]
	mov	r2, r7
	ldr	r0, [r4, #2504]
	ldr	r1, [r4, #2536]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L697
.L705:
	.align	2
.L704:
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
	ldr	r2, [r5, #3604]
	cmp	r2, r6
	bhi	.L709
	add	r1, r0, #12288
	add	r4, r0, r2, lsl #2
	add	r4, r4, #12288
	add	r3, r2, #1
	ldr	r0, [r1, #3460]
	str	r0, [r4, #3604]
	str	r3, [r5, #3604]
	ldrb	r3, [r1, #3446]	@ zero_extendqisi2
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldrb	r3, [r1, #3526]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r1, #3540]
	addeq	r2, r2, #2
	streq	r3, [r4, #3608]
	streq	r2, [r5, #3604]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L709:
	bl	MP4_Clean_HeadPacket
	mov	r0, r4
	bl	MP4_ClearCurPic
	add	r3, r4, #20480
	add	r1, r4, #15680
	ldr	r0, [r3, #2504]
	add	r1, r1, #48
	bl	MP4_ClearCurPacket
	mov	r3, r6
	ldr	r2, [r5, #3604]
	mov	r0, #0
	ldr	r1, .L710
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L711:
	.align	2
.L710:
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
	cmp	r2, #0
	bne	.L748
.L713:
	mov	r1, r5
	mov	r2, r4
	sub	r0, fp, #76
	add	r5, r6, #20480
	bl	BsInit
	ldr	r3, .L753
	add	r0, r6, #20992
	mov	r2, #60
	mov	r1, #0
	add	r0, r0, #240
	ldr	r3, [r3, #48]
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
	strb	r0, [r5, #762]
	sub	r0, r0, #1
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L714
.L716:
	.word	.L715
	.word	.L717
	.word	.L718
	.word	.L719
.L719:
	mov	r2, #18
	mov	r3, #88
	strb	r2, [r5, #763]
	mov	r2, #704
	strb	r3, [r5, #764]
	mov	r3, #576
	str	r2, [r5, #1520]
	str	r3, [r5, #1516]
.L720:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	ldrb	r3, [r5, #819]	@ zero_extendqisi2
	cmp	r3, #2
	uxtb	r0, r0
	strb	r0, [r5, #752]
	beq	.L722
	adds	r0, r0, #0
	ldr	r3, [r5, #2144]
	movne	r0, #1
	cmp	r3, #1
	movne	r0, #0
	cmp	r0, #0
	bne	.L722
	mov	r1, #4
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #5
	sub	r0, fp, #76
	mov	r9, r4, asl #3
	bl	BsGet
	mov	r1, #1
	mov	r4, #0
	uxtb	r0, r0
	cmp	r0, #0
	strneb	r0, [r5, #759]
	sub	r0, fp, #76
	moveq	r3, #1
	streqb	r3, [r5, #759]
	bl	BsSkip
.L728:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #1
	beq	.L749
	add	r4, r4, #1
	cmp	r4, #15
	bls	.L729
.L747:
	mov	r4, #0
	sub	r0, fp, #76
	strb	r4, [r5, #1585]
	bl	BsPos
	ldr	r3, .L753+4
	ldr	ip, [r3]
	cmp	ip, r4
	mov	r8, r0
	beq	.L735
	ldr	r0, [r5, #2504]
	mov	r3, r4
	mov	r2, r4
	mov	r1, #103
	blx	ip
	mov	r0, r8
.L746:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L718:
	mov	r2, #18
	mov	r3, #22
	strb	r2, [r5, #763]
	mov	r2, #352
	strb	r3, [r5, #764]
	mov	r3, #288
	str	r2, [r5, #1520]
	str	r3, [r5, #1516]
	b	.L720
.L717:
	mov	r2, #9
	mov	r3, #11
	strb	r2, [r5, #763]
	mov	r2, #176
	strb	r3, [r5, #764]
	mov	r3, #144
	str	r2, [r5, #1520]
	str	r3, [r5, #1516]
	b	.L720
.L715:
	mov	r2, #6
	mov	r3, #8
	strb	r2, [r5, #763]
	mov	r2, #128
	strb	r3, [r5, #764]
	mov	r3, #96
	str	r2, [r5, #1520]
	str	r3, [r5, #1516]
	b	.L720
.L749:
	sub	r0, fp, #76
	add	r4, r4, #1
	bl	BsPos
	mov	r1, #8
	cmp	r0, r9
	sub	r0, fp, #76
	bgt	.L750
	bl	BsSkip
	cmp	r4, #15
	bls	.L728
	b	.L747
.L748:
	ldr	r1, .L753+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r7, #3328]
	b	.L713
.L729:
	add	r10, r6, #16384
	mov	r2, #1
	strb	r2, [r5, #1585]
	sub	r0, fp, #76
	ldr	r3, [r10, #3608]
	orr	r3, r3, #2
	str	r3, [r10, #3608]
	ldr	r3, [r7, #3328]
	add	r3, r3, r2
	str	r3, [r7, #3328]
	strb	r2, [r5, #760]
	bl	BsPos
	mov	r3, r0, asr #31
	mov	r8, r0
	add	r1, r0, #7
	rsb	r4, r0, r9
	mov	r3, r3, lsr #29
	str	r4, [r6, #1288]
	add	r2, r8, r3
	ldr	r0, [r5, #1600]
	and	r2, r2, #7
	cmp	r8, #0
	rsb	r3, r3, r2
	add	r3, r3, r0
	strb	r3, [r6, #1286]
	ldr	r3, [r5, #1592]
	movge	r1, r8
	add	r3, r3, r1, asr #3
	str	r3, [r6, #1296]
	ldrb	r3, [r7, #3446]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L751
.L730:
	mov	r3, #0
	str	r3, [r6, #1292]
	strb	r3, [r6, #1287]
	str	r3, [r6, #1300]
.L731:
	ldrb	r3, [r5, #752]	@ zero_extendqisi2
	strb	r3, [r6, #1284]
	ldrb	r3, [r5, #761]	@ zero_extendqisi2
	strb	r3, [r6, #1282]
	ldrb	r3, [r5, #760]	@ zero_extendqisi2
	strb	r3, [r6, #1281]
	ldrb	r3, [r5, #759]	@ zero_extendqisi2
	strb	r3, [r6, #1285]
	ldrb	r3, [r5, #756]	@ zero_extendqisi2
	strb	r3, [r6, #1283]
	ldr	r2, [r10, #3604]
	cmp	r2, #0
	bne	.L752
.L732:
	mov	r0, r6
	bl	MP4_Record_SlcSegID
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L722:
	sub	r0, fp, #76
	bl	BsPos
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L714:
	mov	r4, #0
	sub	r0, fp, #76
	strb	r4, [r5, #1585]
	bl	BsPos
	ldr	r3, .L753+4
	ldr	ip, [r3]
	cmp	ip, r4
	mov	r8, r0
	beq	.L735
	ldr	r0, [r5, #2504]
	mov	r3, r4
	mov	r2, r4
	mov	r1, #102
	blx	ip
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L751:
	ldrb	r2, [r7, #3526]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L730
	ldr	r2, [r7, #3536]
	strb	r3, [r6, #1287]
	mov	r3, r2, asl #3
	str	r3, [r6, #1292]
	ldr	r3, [r7, #3532]
	str	r3, [r6, #1300]
	b	.L731
.L752:
	ldr	r1, .L753+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	bl	MP4_ClearCurPic
	b	.L732
.L735:
	mov	r0, r8
	b	.L746
.L750:
	bl	BsPos
	b	.L746
.L754:
	.align	2
.L753:
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
	bne	.L833
	add	r4, r5, #20480
	ldrb	r3, [r4, #813]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L757
.L834:
	mov	r0, #32
	strb	r3, [r4, #1585]
.L758:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L833:
	add	r4, r5, #20480
	ldr	r1, .L849
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r7, #3328]
	ldrb	r3, [r4, #813]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L834
.L757:
	add	r8, r5, #20992
	ldr	r9, .L849+4
	add	r8, r8, #240
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
	strb	r3, [r4, #1585]
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsGet
	ldr	r1, .L849+8
	uxtb	r2, r0
	strb	r0, [r4, #752]
	mov	r0, #18
	bl	dprint_vfmw
	ldrb	r10, [r4, #752]	@ zero_extendqisi2
	cmp	r10, #3
	beq	.L835
.L759:
	adds	r10, r10, #0
	ldrb	r3, [r4, #819]	@ zero_extendqisi2
	movne	r10, #1
	cmp	r3, #1
	movne	r2, #0
	andeq	r2, r10, #1
	cmp	r2, #0
	bne	.L762
	cmp	r3, #2
	beq	.L762
	ldr	r3, [r4, #2144]
	cmp	r3, #1
	movne	r10, #0
	andeq	r10, r10, #1
	cmp	r10, #0
	bne	.L762
	mov	r6, r6, asl #3
	b	.L765
.L837:
	bl	BsPos
	add	r10, r10, #1
	cmp	r0, r6
	bgt	.L836
	cmp	r10, #6400
	beq	.L766
.L765:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	sub	r0, fp, #76
	bne	.L837
	cmp	r10, #6400
	bcs	.L766
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	subs	r3, r0, #0
	beq	.L838
	ldr	r1, [r4, #920]
	cmp	r1, #0
	moveq	r0, r1
	bne	.L839
.L769:
	str	r10, [r4, #1524]
	mov	r1, #1
	str	r0, [r4, #1528]
	sub	r0, fp, #76
	bl	BsGet
	subs	r10, r0, #0
	bne	.L770
	ldr	r1, .L849+12
	mov	r0, #1
	bl	dprint_vfmw
	strb	r10, [r4, #1585]
	sub	r0, fp, #76
	bl	BsPos
	ldr	r3, .L849+16
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L796
	ldr	r0, [r4, #2504]
	mov	r3, r10
	mov	r2, r10
	mov	r1, #103
	blx	ip
	mov	r0, r5
	b	.L758
.L766:
	mov	r3, #0
	sub	r0, fp, #76
	strb	r3, [r4, #1585]
	bl	BsPos
	mov	r2, r10
	ldr	r1, .L849+20
	mov	r4, r0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	b	.L758
.L836:
	sub	r0, fp, #76
	bl	BsPos
	ldr	r1, .L849+24
	mov	r4, r0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	b	.L758
.L835:
	ldr	r3, .L849+28
	ldr	r3, [r3]
	ands	r3, r3, #8
	bne	.L759
	strb	r3, [r4, #1585]
	sub	r0, fp, #76
	str	r3, [fp, #-80]
	bl	BsPos
	ldr	r2, .L849+16
	ldr	ip, [r2]
	cmp	ip, #0
	mov	r5, r0
	beq	.L760
	ldr	r3, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2504]
	mov	r2, r3
	blx	ip
.L760:
	ldr	r1, .L849+32
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	b	.L758
.L762:
	sub	r0, fp, #76
	bl	BsPos
	b	.L758
.L770:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #753]
	cmp	r0, #0
	beq	.L840
	ldrb	r3, [r4, #752]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L772
	cmp	r3, #3
	beq	.L841
.L773:
	mov	r3, #0
	strb	r3, [r4, #754]
.L774:
	ldrb	r3, [r4, #860]	@ zero_extendqisi2
	mov	r2, #0
	strb	r2, [r4, #755]
	cmp	r3, #2
	addeq	r10, r5, #16384
	beq	.L775
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	add	r10, r5, #16384
	strb	r0, [r4, #756]
	ldrb	r3, [r10, #3650]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r3, [r4, #758]
	moveq	r3, #2
	streqb	r3, [r4, #757]
	bne	.L842
.L777:
	ldrb	r3, [r4, #864]	@ zero_extendqisi2
	sub	r3, r3, #1
	cmp	r3, #1
	bls	.L778
.L781:
	mov	r1, #5
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r0, r0
	cmp	r0, #0
	moveq	r3, #1
	streqb	r3, [r4, #759]
	strneb	r0, [r4, #759]
.L787:
	ldrb	r3, [r4, #752]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L843
.L775:
	mov	r3, #1
	strb	r3, [r4, #1585]
	ldr	r3, [r10, #3608]
	add	r0, r5, #21760
	mov	r1, r8
	mov	r2, #60
	orr	r3, r3, #2
	str	r3, [r10, #3608]
	ldr	r3, [r7, #3328]
	add	r0, r0, #176
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
	ldr	r0, [r4, #1600]
	and	r2, r2, #7
	cmp	r8, #0
	rsb	r3, r3, r2
	add	r3, r3, r0
	strb	r3, [r5, #1286]
	ldr	r3, [r4, #1592]
	movge	r1, r8
	add	r3, r3, r1, asr #3
	str	r3, [r5, #1296]
	ldrb	r3, [r7, #3446]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L791
	ldrb	r2, [r7, #3526]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L844
.L791:
	mov	r3, #0
	str	r3, [r5, #1292]
	strb	r3, [r5, #1287]
	str	r3, [r5, #1300]
.L792:
	ldrb	r3, [r4, #752]	@ zero_extendqisi2
	strb	r3, [r5, #1284]
	ldrb	r3, [r4, #761]	@ zero_extendqisi2
	strb	r3, [r5, #1282]
	ldrb	r3, [r4, #760]	@ zero_extendqisi2
	strb	r3, [r5, #1281]
	ldrb	r3, [r4, #759]	@ zero_extendqisi2
	strb	r3, [r5, #1285]
	ldrb	r3, [r4, #756]	@ zero_extendqisi2
	strb	r3, [r5, #1283]
	ldr	r2, [r10, #3604]
	cmp	r2, #0
	bne	.L845
.L793:
	mov	r0, r5
	bl	MP4_Record_SlcSegID
	mov	r0, r8
	b	.L758
.L838:
	ldr	r1, .L849+12
	mov	r0, #1
	str	r3, [fp, #-80]
	bl	dprint_vfmw
	ldr	r3, [fp, #-80]
	sub	r0, fp, #76
	strb	r3, [r4, #1585]
	bl	BsPos
	ldr	r2, .L849+16
	ldr	ip, [r2]
	cmp	ip, #0
	mov	r5, r0
	beq	.L796
	ldr	r3, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2504]
	mov	r2, r3
	blx	ip
	mov	r0, r5
	b	.L758
.L839:
	sub	r0, fp, #76
	bl	BsGet
	b	.L769
.L796:
	mov	r0, r5
	b	.L758
.L845:
	ldr	r1, .L849+36
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	bl	MP4_ClearCurPic
	b	.L793
.L840:
	add	r5, r5, #16384
	mov	r3, #4
	mov	r2, #1
	strb	r3, [r4, #752]
	strb	r3, [r4, #1456]
	sub	r0, fp, #76
	strb	r2, [r4, #1585]
	ldr	r3, [r5, #3608]
	orr	r3, r3, #128
	str	r3, [r5, #3608]
	bl	BsPos
	b	.L758
.L841:
	ldrb	r3, [r4, #864]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L773
.L772:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #754]
	b	.L774
.L843:
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r2, r0
	strb	r2, [r4, #760]
	cmp	r2, #0
	beq	.L846
	ldrb	r3, [r4, #752]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L775
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsGet
	uxtb	r2, r0
	strb	r2, [r4, #761]
	cmp	r2, #0
	bne	.L775
	strb	r2, [r4, #1585]
	sub	r0, fp, #76
	str	r2, [fp, #-80]
	bl	BsPos
	ldr	r3, .L849+16
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L790
	ldr	r2, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2504]
	mov	r3, r2
	blx	ip
.L790:
	ldr	r1, .L849+40
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	b	.L758
.L842:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #757]
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r4, #758]
	b	.L777
.L778:
	ldrb	r3, [r4, #752]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L781
	ldrb	r3, [r4, #876]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L781
	add	r3, r5, #21248
	mov	r2, #0
	add	r3, r3, #20
	str	r5, [fp, #-84]
	str	r6, [fp, #-88]
	mov	r6, r2
	str	r8, [fp, #-80]
	mov	r5, r3
	str	r7, [fp, #-92]
	b	.L786
.L782:
	ldr	r3, [r4, #1560]
	cmp	r3, #500
	bne	.L783
	ldr	r2, [r4, #1564]
	movw	r3, #413
	cmp	r2, r3
	beq	.L784
.L783:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsSkip
.L784:
	sub	r0, fp, #76
	bl	bs_get_spritetrajectory
	subs	r3, r0, #0
	moveq	r7, r3
	bne	.L847
.L785:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsSkip
	str	r8, [r5]
	str	r7, [r5, #4]
	add	r6, r6, #1
	ldrb	r3, [r4, #876]	@ zero_extendqisi2
	add	r5, r5, #8
	cmp	r3, r6
	bls	.L848
.L786:
	sub	r0, fp, #76
	bl	bs_get_spritetrajectory
	subs	r7, r0, #0
	moveq	r8, r7
	beq	.L782
	mov	r1, r7
	sub	r0, fp, #76
	bl	BsGet
	sub	r2, r7, #1
	movs	r3, r0, asr r2
	mov	r8, r0
	moveq	r3, #1
	moveq	r3, r3, asl r7
	subeq	r3, r3, #1
	eoreq	r8, r8, r3
	rsbeq	r8, r8, #0
	b	.L782
.L847:
	mov	r1, r3
	sub	r0, fp, #76
	str	r3, [fp, #-96]
	bl	BsGet
	ldr	r3, [fp, #-96]
	sub	r2, r3, #1
	movs	r2, r0, asr r2
	mov	r7, r0
	moveq	r2, #1
	moveq	r3, r2, asl r3
	subeq	r2, r3, #1
	eoreq	r3, r0, r2
	rsbeq	r7, r3, #0
	b	.L785
.L844:
	ldr	r2, [r7, #3536]
	strb	r3, [r5, #1287]
	mov	r3, r2, asl #3
	str	r3, [r5, #1292]
	ldr	r3, [r7, #3532]
	str	r3, [r5, #1300]
	b	.L792
.L846:
	strb	r2, [r4, #1585]
	sub	r0, fp, #76
	str	r2, [fp, #-80]
	bl	BsPos
	ldr	r3, .L849+16
	ldr	ip, [r3]
	cmp	ip, #0
	mov	r5, r0
	beq	.L789
	ldr	r2, [fp, #-80]
	mov	r1, #103
	ldr	r0, [r4, #2504]
	mov	r3, r2
	blx	ip
.L789:
	ldrb	r2, [r4, #752]	@ zero_extendqisi2
	mov	r0, #1
	ldr	r1, .L849+44
	bl	dprint_vfmw
	mov	r0, r5
	b	.L758
.L848:
	ldr	r8, [fp, #-80]
	ldr	r5, [fp, #-84]
	ldr	r6, [fp, #-88]
	ldr	r7, [fp, #-92]
	b	.L781
.L850:
	.align	2
.L849:
	.word	.LC43
	.word	vfmw_Osal_Func_Ptr_S
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
	bhi	.L872
	add	r3, r6, #3600
	add	r0, r8, #1
	str	r0, [r6, #3328]
	add	r5, r4, #20480
	ldrh	r9, [r3]
	sub	r0, fp, #76
	bl	BsInit
	sub	r0, fp, #76
	ubfx	r9, r9, #7, #4
	add	r9, r9, #17
	mov	r1, r9
	bl	BsSkip
	ldr	r2, [r5, #1516]
	ldr	r9, [r5, #1520]
	add	r2, r2, #15
	add	r9, r9, #15
	mov	r2, r2, lsr #4
	mov	r9, r9, lsr #4
	mul	r9, r9, r2
	cmp	r9, #0
	beq	.L854
	mov	lr, r9
	mov	r1, #0
.L855:
	add	ip, r1, #1
	movs	lr, lr, lsr #1
	uxth	r1, ip
	bne	.L855
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
	bhi	.L873
.L856:
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
	beq	.L857
.L867:
	mov	r1, #1
	sub	r0, fp, #76
	bl	BsGet
	cmp	r0, #0
	bne	.L867
	mov	r1, #1
	sub	r0, fp, #76
	add	ip, r4, #16384
	str	ip, [fp, #-80]
	bl	BsSkip
	ldr	ip, [fp, #-80]
	ldr	r1, [ip, #3708]
	cmp	r1, #0
	bne	.L874
.L859:
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
	bne	.L875
.L861:
	ldrb	r2, [r5, #760]	@ zero_extendqisi2
	rsb	r3, r10, r8
	add	r3, r4, r3
	sub	r0, fp, #76
	str	r3, [fp, #-80]
	strb	r2, [r3, #1281]
	ldrb	r2, [r5, #761]	@ zero_extendqisi2
	strb	r2, [r3, #1282]
	ldr	r2, [ip, #3608]
	orr	r2, r2, #16
	str	r2, [ip, #3608]
	bl	BsPos
	ldr	r3, [fp, #-80]
	mov	r2, r0, asr #31
	mov	r9, r0
	rsb	r7, r0, r7, lsl #3
	str	r7, [r3, #1288]
	mov	r2, r2, lsr #29
	ldr	ip, [r5, #1600]
	add	r1, r9, r2
	cmp	r9, #0
	and	r1, r1, #7
	add	r0, r0, #7
	rsb	r2, r2, r1
	movge	r0, r9
	add	r2, r2, ip
	strb	r2, [r3, #1286]
	ldr	r2, [r5, #1592]
	add	r2, r2, r0, asr #3
	str	r2, [r3, #1296]
	ldrb	r2, [r6, #3446]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L863
	ldrb	r1, [r6, #3526]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L876
.L863:
	rsb	r8, r10, r8
	mov	r3, #0
	add	r8, r4, r8
	str	r3, [r8, #1292]
	strb	r3, [r8, #1287]
	str	r3, [r8, #1300]
.L864:
	mov	r0, r4
	bl	MP4_Record_SlcSegID
	mov	r0, r9
.L871:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L857:
	ldrb	r3, [r5, #752]	@ zero_extendqisi2
	add	ip, r4, #16384
	strb	r3, [r9, #1284]
	ldrb	r3, [r5, #756]	@ zero_extendqisi2
	strb	r3, [r9, #1283]
	b	.L861
.L872:
	add	r3, r4, #20480
	add	r1, r4, #15680
	add	r1, r1, #48
	ldr	r0, [r3, #2504]
	bl	MP4_ClearCurPacket
	mov	r0, r4
	add	r4, r4, #16384
	bl	MP4_ClearCurPic
	ldr	r2, .L877
	mov	r0, #1
	ldr	r3, [r4, #3608]
	ldr	r1, .L877+4
	bic	r3, r3, #432
	bic	r3, r3, #2
	str	r3, [r4, #3608]
	ldr	r3, [r6, #3328]
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L874:
	sub	r0, fp, #76
	bl	BsSkip
	ldr	ip, [fp, #-80]
	b	.L859
.L875:
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsSkip
	ldrb	r3, [r9, #1284]	@ zero_extendqisi2
	ldr	ip, [fp, #-80]
	cmp	r3, #2
	bne	.L861
	mov	r1, #3
	sub	r0, fp, #76
	bl	BsSkip
	ldr	ip, [fp, #-80]
	b	.L861
.L876:
	ldr	r1, [r6, #3536]
	strb	r2, [r3, #1287]
	mov	r2, r1, asl #3
	str	r2, [r3, #1292]
	ldr	r2, [r6, #3532]
	str	r2, [r3, #1300]
	b	.L864
.L854:
	mov	r1, r9
	sub	r0, fp, #76
	bl	BsGet
	mov	r10, r8, asl #2
	mov	r8, r8, asl #5
	rsb	r3, r10, r8
	add	r3, r4, r3
	str	r0, [r3, #1304]
	b	.L856
.L873:
	mov	r2, r0
	mov	r3, r9
	ldr	r1, .L877+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L871
.L878:
	.align	2
.L877:
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
	bhi	.L893
	ldrb	r7, [r8, #3600]	@ zero_extendqisi2
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
	ldrb	r2, [r10, #764]	@ zero_extendqisi2
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
	bhi	.L894
	mov	r1, #2
	sub	r0, fp, #76
	bl	BsSkip
	mov	r1, #5
	sub	r0, fp, #76
	bl	BsGet
	strb	r0, [r5, #1285]
	sub	r0, fp, #76
	ldrb	r2, [r10, #752]	@ zero_extendqisi2
	strb	r2, [r5, #1284]
	ldrb	r2, [r10, #756]	@ zero_extendqisi2
	strb	r2, [r5, #1283]
	ldrb	r2, [r10, #760]	@ zero_extendqisi2
	strb	r2, [r5, #1281]
	ldrb	r2, [r10, #761]	@ zero_extendqisi2
	strb	r2, [r5, #1282]
	bl	BsPos
	add	lr, r4, #16384
	mov	r2, r0, asr #31
	mov	r7, r0
	rsb	r9, r0, r9, lsl #3
	str	r9, [r5, #1288]
	mov	r2, r2, lsr #29
	ldr	r9, [r10, #1600]
	add	r1, r7, r2
	cmp	r7, #0
	and	r1, r1, #7
	add	r0, r0, #7
	rsb	r2, r2, r1
	movge	r0, r7
	add	r2, r2, r9
	strb	r2, [r5, #1286]
	ldr	r3, [r10, #1592]
	add	r0, r3, r0, asr #3
	str	r0, [r5, #1296]
	ldr	r3, [lr, #3608]
	orr	r3, r3, #16
	str	r3, [lr, #3608]
	ldrb	r3, [r8, #3446]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L895
.L883:
	ldr	r3, [fp, #-84]
	ldr	r2, [fp, #-80]
	rsb	r2, r2, r3
	mov	r3, #0
	add	r2, r4, r2
	str	r3, [r2, #1292]
	strb	r3, [r2, #1287]
	str	r3, [r2, #1300]
.L884:
	mov	r1, r6, asl #2
	mov	r3, r6, asl #5
	rsb	r2, r1, r3
	add	r2, r4, r2
	ldr	r0, [r2, #1300]
	cmp	r0, #0
	bne	.L896
.L885:
	rsb	r3, r1, r3
	ldrb	r2, [r8, #3600]	@ zero_extendqisi2
	add	r3, r4, r3
	ubfx	r2, r2, #3, #4
	ldr	r1, [r3, #1288]
	add	r2, r2, r1
	str	r2, [r3, #1288]
.L886:
	mov	r0, r4
	bl	MP4_Record_SlcSegID
	mov	r0, r7
.L881:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L896:
	ldr	r0, [r2, #1292]
	cmp	r0, #0
	beq	.L885
	ldrb	r3, [r8, #3600]	@ zero_extendqisi2
	ubfx	r3, r3, #3, #4
	add	r3, r3, r0
	str	r3, [r2, #1292]
	b	.L886
.L895:
	ldrb	r2, [r8, #3526]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L883
	ldr	r2, [r8, #3536]
	strb	r3, [r5, #1287]
	mov	r3, r2, asl #3
	str	r3, [r5, #1292]
	ldr	r3, [r8, #3532]
	str	r3, [r5, #1300]
	b	.L884
.L893:
	add	r3, r0, #20480
	add	r1, r0, #15680
	add	r1, r1, #48
	ldr	r0, [r3, #2504]
	bl	MP4_ClearCurPacket
	mov	r0, r4
	add	r4, r4, #16384
	bl	MP4_ClearCurPic
	ldr	r2, .L897
	mov	r0, #1
	ldr	r3, [r4, #3608]
	ldr	r1, .L897+4
	bic	r3, r3, #432
	bic	r3, r3, #2
	str	r3, [r4, #3608]
	ldr	r3, [r8, #3328]
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L894:
	mov	r2, r0
	ldr	r1, .L897+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L881
.L898:
	.align	2
.L897:
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
	ldrb	r3, [ip, #3600]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L909
	tst	r3, #4
	beq	.L910
	mov	r0, r4
	bl	Mp4_Syntax_Slice_SH
	cmn	r0, #1
	beq	.L907
.L903:
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L910:
	add	r0, ip, #3600
	ldrh	r0, [r0]
	ubfx	r0, r0, #7, #4
	sub	lr, r0, #1
	cmp	lr, #6
	bfils	r3, r0, #3, #4
	mov	r0, r4
	strlsb	r3, [ip, #3600]
	bl	Mp4_Syntax_Slice_SH
	cmn	r0, #1
	bne	.L903
.L907:
	add	r3, r4, #20480
	add	r1, r4, #15680
	add	r1, r1, #48
	ldr	r0, [r3, #2504]
	bl	MP4_ClearCurPacket
	ldr	r2, .L912
	ldr	r1, .L912+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L909:
	str	r2, [fp, #-28]
	str	r1, [fp, #-24]
	bl	Mp4_Check_Slice_NotSH
	ldr	r1, [fp, #-24]
	ldr	r2, [fp, #-28]
	cmp	r0, #0
	beq	.L911
	mov	r1, r2
	mov	r0, r4
	bl	Mp4_Combine_SliceData
	cmp	r0, #0
	bne	.L907
	mov	r0, r4
	bl	MP4_Record_SlcSegID
	b	.L903
.L911:
	mov	r0, r4
	bl	Mp4_Syntax_Slice_NotSH
	cmn	r0, #1
	beq	.L907
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L913:
	.align	2
.L912:
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
	ldr	r3, [r5, #2060]
	cmp	r3, #62
	bgt	.L917
	add	r1, r0, #12288
	add	r4, r0, r3, lsl #2
	add	r4, r4, #20480
	add	r2, r3, #1
	ldr	r0, [r1, #3460]
	str	r0, [r4, #1804]
	str	r2, [r5, #2060]
	ldrb	r2, [r1, #3446]	@ zero_extendqisi2
	cmp	r2, #0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldrb	r2, [r1, #3526]	@ zero_extendqisi2
	cmp	r2, #1
	ldreq	r2, [r1, #3540]
	addeq	r3, r3, #2
	streq	r2, [r4, #1808]
	streq	r3, [r5, #2060]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L917:
	bl	MP4_Clean_HeadPacket
	add	r1, r4, #15680
	ldr	r0, [r5, #2504]
	add	r1, r1, #48
	bl	MP4_ClearCurPacket
	ldr	r2, [r5, #2060]
	mov	r3, #62
	ldr	r1, .L918
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L919:
	.align	2
.L918:
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
	beq	.L923
	ldr	r6, .L925
	add	r1, r4, #15680
	add	r0, r4, #22016
	add	r1, r1, #48
	mov	r2, #160
	add	r0, r0, #104
	ldr	r3, [r6, #52]
	add	r5, r4, #20480
	blx	r3
	add	r1, r4, #15872
	add	r0, r4, #22272
	ldr	r3, [r6, #52]
	add	r1, r1, #16
	add	r0, r0, #8
	mov	r2, #4
	blx	r3
	ldr	r3, [r5, #2068]
	str	r7, [r5, #2064]
	cmp	r3, #0
	beq	.L924
.L922:
	ldr	r3, [r5, #2072]
	add	r3, r3, #1
	str	r3, [r5, #2072]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L924:
	mov	r0, r4
	bl	MP4_Record_PacketID
	mov	r3, #1
	str	r3, [r5, #2068]
	b	.L922
.L923:
	ldr	r1, .L925+4
	mov	r0, #1
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L926:
	.align	2
.L925:
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
	ldr	r9, .L1077
	mov	r6, r0
	mov	r7, r1
	mov	r4, r2
	str	r2, [fp, #-120]
	ldr	r3, [r9, #48]
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #108
	blx	r3
	mov	r2, r4
	mov	r1, r7
	sub	r0, fp, #108
	bl	BsInit
	sub	r0, fp, #108
	mov	r1, #32
	add	r8, r6, #16384
	bl	BsShow
	movw	r3, #510
	mov	r10, #0
	str	r10, [r8, #3612]
	cmp	r0, r3
	mov	r5, r0
	beq	.L1056
	add	r1, r6, #12288
	ldrb	r3, [r1, #3600]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L1057
.L933:
	add	r4, r6, #20480
	mov	r5, #196
	mov	r2, #1
	strb	r2, [r4, #821]
.L934:
	and	r3, r3, #2
	ands	r3, r3, #255
	bne	.L935
	ldr	r2, [r8, #3608]
	ands	r10, r2, #130
	bne	.L1058
	cmp	r5, #197
	beq	.L1059
	cmp	r5, #432
	beq	.L1060
	movw	r3, #437
	cmp	r5, r3
	beq	.L1061
	bic	r3, r5, #15
	cmp	r3, #288
	beq	.L1062
	movw	r3, #435
	cmp	r5, r3
	beq	.L1063
	movw	r3, #438
	cmp	r5, r3
	beq	.L1064
	cmp	r5, #196
	beq	.L1065
	movw	r3, #434
	cmp	r5, r3
	beq	.L1066
	movw	r3, #286
	cmp	r5, r3
	bne	.L950
	ldr	r10, .L1077+4
	ldr	ip, [r10]
	cmp	ip, #0
	beq	.L951
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #2504]
	blx	ip
.L951:
	ldr	r3, [fp, #-120]
	add	r1, r6, #15680
	ldr	r0, [r4, #2504]
	add	r1, r1, #48
	sub	r3, r3, #4
	str	r3, [fp, #-120]
	bl	MP4_ClearCurPacket
	ldr	r3, [r8, #3608]
	add	r2, r7, #4
	str	r2, [fp, #-124]
	tst	r3, #2
	orrne	r3, r3, #256
	strne	r3, [r8, #3608]
	ldr	r3, [r8, #3608]
	and	r3, r3, #18
	cmp	r3, #18
	beq	.L953
	ldr	r10, [r10]
	cmp	r10, #0
	beq	.L953
	mov	r3, #0
	mov	r1, #127
	mov	r2, r3
	ldr	r0, [r4, #2504]
	blx	r10
.L953:
	ldrb	r3, [r4, #2541]	@ zero_extendqisi2
	mov	r10, #0
	cmp	r3, #1
	beq	.L984
.L959:
	ldrb	r3, [r4, #1583]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1053
	ldr	r3, [r4, #2080]
	add	r1, r6, #19968
	add	r0, r6, #21248
	add	r1, r1, #56
	add	r3, r3, #1
	mov	r2, #604
	str	r3, [r4, #2080]
	add	r0, r0, #84
	mov	r3, #0
	ldr	r5, [r9, #52]
	str	r3, [r4, #2084]
	blx	r5
	ldr	r2, [r4, #1516]
	ldr	r0, [r8, #3724]
	ldr	r1, [r8, #3720]
	cmp	r2, r0
	beq	.L1067
.L975:
	ldr	r3, .L1077+4
	ldr	r5, [r3]
	cmp	r5, #0
	beq	.L977
	ldr	lr, [r4, #1520]
	mov	r3, #8
	strh	r2, [fp, #-114]	@ movhi
	sub	r2, fp, #116
	strh	r1, [fp, #-112]	@ movhi
	mov	r1, #2
	strh	r0, [fp, #-110]	@ movhi
	strh	lr, [fp, #-116]	@ movhi
	ldr	r0, [r4, #2504]
	blx	r5
	ldr	r0, [r8, #3724]
	ldr	r1, [r8, #3720]
.L977:
	mov	r5, #1
	str	r0, [r4, #1516]
	str	r1, [r4, #1520]
.L976:
	sxtb	r5, r5
	cmp	r5, #1
	beq	.L1068
.L979:
	ldr	r3, [r4, #1516]
	mov	r2, #0
	ldr	r1, [r4, #1520]
	str	r3, [r4, #2104]
	str	r1, [r4, #2100]
	ldr	r3, [r8, #3724]
	str	r3, [r4, #1516]
	ldr	r3, [r8, #3720]
	strb	r2, [r4, #1583]
	str	r3, [r4, #1520]
	b	.L1053
.L1058:
	orr	r2, r2, #32
	mov	r10, r3
	str	r2, [r8, #3608]
	mov	r3, #1
	str	r3, [r8, #3612]
.L1053:
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L935:
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r6
	bl	MP4_Dec_Slice
	mov	r10, r0
.L939:
	ldrb	r3, [r4, #2541]	@ zero_extendqisi2
	str	r7, [fp, #-124]
	cmp	r3, #1
	beq	.L984
	ldr	r3, [fp, #-120]
	cmp	r10, r3, asl #3
	bhi	.L958
.L1073:
	movw	r3, #438
	cmp	r5, r3
	cmpne	r5, #196
	bne	.L959
	ldr	r3, [r8, #3608]
	tst	r3, #130
	beq	.L959
	ldrb	r3, [r4, #821]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1069
	ldr	r1, [r4, #1520]
	ldr	r0, [r4, #1516]
	cmp	r1, #0
	str	r1, [r4, #2108]
	str	r0, [r4, #2112]
	beq	.L963
	ldr	r3, [r4, #2100]
	cmp	r1, r3
	beq	.L963
.L992:
	mov	r8, #1
.L964:
	ldr	r3, .L1077+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L968
	ldr	lr, [r4, #2100]
	mov	r3, #8
	ldr	r5, [r4, #2104]
	sub	r2, fp, #116
	strh	r1, [fp, #-112]	@ movhi
	mov	r1, #2
	strh	r0, [fp, #-110]	@ movhi
	strh	lr, [fp, #-116]	@ movhi
	strh	r5, [fp, #-114]	@ movhi
	ldr	r0, [r4, #2504]
	blx	ip
.L968:
	sxtb	r8, r8
	cmp	r8, #1
	beq	.L1070
.L970:
	ldr	r2, [r4, #2108]
	ldr	r3, [r4, #2112]
	str	r2, [r4, #2100]
	str	r3, [r4, #2104]
.L962:
	add	r1, r6, #20992
	add	r0, r6, #21760
	ldr	r3, [r9, #52]
	mov	r2, #60
	add	r1, r1, #240
	add	r0, r0, #176
	blx	r3
	ldr	r1, .L1077+8
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r3, [fp, #-124]
	rsb	r7, r7, r3
	ldr	r3, [r4, #2084]
	add	r10, r10, r7, lsl #3
	add	r3, r3, #1
	str	r3, [r4, #2084]
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1057:
	mov	r2, r0, lsr #8
	add	r4, r6, #20480
	sub	r2, r2, #128
	mov	r0, #2
	cmp	r2, #3
	strb	r0, [r4, #821]
	ldrlsb	r3, [r1, #3600]	@ zero_extendqisi2
	orrls	r3, r3, r0
	strlsb	r3, [r1, #3600]
	b	.L934
.L963:
	cmp	r0, #0
	beq	.L965
	ldr	r3, [r4, #2104]
	cmp	r0, r3
	beq	.L1071
	cmp	r1, #0
	beq	.L992
	ldr	r3, [r4, #2100]
	cmp	r1, r3
	bne	.L992
	mov	r8, #1
	b	.L985
.L984:
	ldr	r1, [r8, #3720]
	cmp	r1, #0
	beq	.L956
	ldr	r2, [r8, #3724]
	cmp	r2, #0
	bne	.L1072
.L956:
	mov	r3, #0
	strb	r3, [r4, #2541]
	ldr	r3, [fp, #-120]
	cmp	r10, r3, asl #3
	bls	.L1073
.L958:
	ldr	r1, .L1077+12
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r2, [fp, #-124]
	ldr	r3, [r8, #3608]
	rsb	r7, r7, r2
	bic	r3, r3, #432
	mov	r10, r7, asl #3
	bic	r3, r3, #2
	str	r3, [r8, #3608]
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1056:
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
	mov	r4, r0
	sub	r0, fp, #76
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r4, r3
	bne	.L929
	ldr	r3, .L1077+4
	add	r4, r6, #20480
	mov	r2, #1
	strb	r2, [r4, #2540]
	ldr	r5, [r3]
	cmp	r5, r10
	beq	.L930
	mov	r3, r10
	mov	r2, r10
	mov	r1, #117
	ldr	r0, [r4, #2504]
	blx	r5
.L930:
	ldr	r3, [r8, #3608]
	add	r1, r6, #15680
	add	r1, r1, #48
	mov	r10, #0
	tst	r3, #130
	orrne	r3, r3, #32
	strne	r3, [r8, #3608]
	movne	r2, #1
	strne	r2, [r8, #3612]
	ldr	r0, [r4, #2504]
	bl	MP4_ClearCurPacket
	b	.L1053
.L929:
	add	r3, r6, #12288
	ldrb	r3, [r3, #3600]	@ zero_extendqisi2
	tst	r3, #1
	bne	.L933
	add	r4, r6, #20480
	mov	r2, #2
	strb	r2, [r4, #821]
	b	.L934
.L950:
	ldr	r3, [fp, #-120]
	add	r1, r6, #15680
	ldr	r0, [r4, #2504]
	add	r1, r1, #48
	sub	r3, r3, #4
	str	r3, [fp, #-120]
	add	r3, r7, #4
	str	r3, [fp, #-124]
	bl	MP4_ClearCurPacket
	b	.L953
.L1060:
	ldr	r1, .L1077+16
	mov	r0, #2
	bl	dprint_vfmw
	str	r10, [r4, #2116]
	mov	r1, r7
	ldr	r2, [fp, #-120]
	mov	r0, r6
	bl	Mp4_Syntax_Vos
	mov	r1, r5
	mov	r10, r0
	mov	r0, r6
	bl	MP4_Record_HeadPacketInfo
	b	.L939
.L965:
	ldr	r8, [r4, #2508]
	subs	r8, r8, #1
	movne	r8, #1
	cmp	r1, #0
	beq	.L968
.L986:
	ldr	r3, [r4, #2100]
	cmp	r1, r3
	bne	.L964
.L967:
	cmp	r0, #0
	beq	.L968
.L985:
	ldr	r3, [r4, #2104]
	cmp	r0, r3
	beq	.L968
	b	.L964
.L1064:
	ldr	r1, .L1077+20
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r3, [fp, #-120]
	cmp	r3, #4
	beq	.L1074
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r6
	bl	Mp4_Syntax_Vop
	ldr	r3, [r8, #3608]
	tst	r3, #2
	mov	r10, r0
	bne	.L946
	add	r1, r6, #15680
	ldr	r0, [r4, #2504]
	add	r1, r1, #48
	bl	MP4_ClearCurPacket
.L945:
	mov	r2, r10
	ldr	r1, .L1077+24
	mov	r0, #7
	bl	dprint_vfmw
	b	.L939
.L1067:
	ldr	r3, [r4, #1520]
	cmp	r3, r1
	bne	.L975
	ldr	r5, [r4, #2508]
	subs	r5, r5, #1
	movne	r5, #1
	b	.L976
.L1061:
	ldr	r1, .L1077+28
	mov	r0, #2
	bl	dprint_vfmw
	mov	r3, #1
	mov	r1, r7
	ldr	r2, [fp, #-120]
	str	r3, [r4, #2116]
	mov	r0, r6
	bl	Mp4_Syntax_Vo
	mov	r1, r5
	mov	r10, r0
	mov	r0, r6
	bl	MP4_Record_HeadPacketInfo
	b	.L939
.L1062:
	ldr	r1, .L1077+32
	mov	r0, #2
	bl	dprint_vfmw
	mov	r3, #2
	ldr	r2, [fp, #-120]
	mov	r1, r7
	str	r3, [r4, #2116]
	mov	r0, r6
	bl	Mp4_Syntax_Vol
	mov	r1, r5
	mov	r10, r0
	mov	r0, r6
	bl	MP4_Record_HeadPacketInfo
	ldrb	r3, [r4, #1583]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L939
	ldrb	r3, [r4, #1584]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L939
	add	r1, r6, #20480
	add	r0, r6, #19968
	add	r1, r1, #148
	add	r0, r0, #56
	ldr	ip, [r9, #52]
	mov	r2, #604
	str	r3, [fp, #-124]
	blx	ip
	ldr	r3, [fp, #-124]
	strb	r3, [r4, #1583]
	strb	r3, [r4, #813]
	b	.L939
.L1059:
	ldr	r1, .L1077+36
	mov	r0, #2
	bl	dprint_vfmw
	add	r1, r6, #15680
	add	r1, r1, #48
	ldr	r0, [r4, #2504]
	bl	MP4_ClearCurPacket
	b	.L1053
.L1071:
	ldr	r8, [r4, #2508]
	subs	r8, r8, #1
	movne	r8, #1
	cmp	r1, #0
	beq	.L967
	b	.L986
.L1069:
	ldrb	r3, [r4, #752]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L961
	ldr	r2, [r4, #1532]
	ldr	r3, [r4, #1524]
	ldr	r0, [r4, #1528]
	str	r2, [r4, #1536]
	add	r2, r2, r3
	str	r2, [r4, #1532]
	ldr	r3, [r8, #3712]
	ldr	r1, [r4, #1544]
	mla	r3, r3, r2, r0
	rsb	r2, r1, r3
	str	r2, [r4, #1548]
	str	r3, [r4, #1540]
	str	r3, [r4, #1544]
	b	.L962
.L1063:
	ldr	r1, .L1077+40
	mov	r0, #2
	bl	dprint_vfmw
	mov	r3, #3
	mov	r1, r7
	ldr	r2, [fp, #-120]
	str	r3, [r4, #2116]
	mov	r0, r6
	bl	Mp4_Syntax_Gop
	mov	r1, r5
	mov	r10, r0
	mov	r0, r6
	bl	MP4_Record_HeadPacketInfo
	b	.L939
.L1072:
	mov	r0, r6
	bl	Frame_Init
	cmp	r0, #0
	beq	.L956
	ldr	r1, .L1077+44
	mov	r0, #0
	bl	dprint_vfmw
	b	.L956
.L1065:
	ldr	r1, .L1077+48
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r6
	bl	Mp4_Syntax_ShortHeader
	ldr	r3, [r8, #3608]
	tst	r3, #2
	mov	r10, r0
	beq	.L1054
	mov	r1, #196
	mov	r0, r6
	bl	MP4_Record_HeadPacketInfo
	b	.L939
.L1066:
	ldr	r1, .L1077+52
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r2, [fp, #-120]
	mov	r1, r7
	mov	r0, r6
	bl	Mp4_Syntax_UserData
	mov	r10, r0
.L1054:
	add	r1, r6, #15680
	ldr	r0, [r4, #2504]
	add	r1, r1, #48
	bl	MP4_ClearCurPacket
	b	.L939
.L1070:
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	subs	r2, r0, #0
	beq	.L971
	ldr	r1, [r2, #560]
	add	r5, r2, #576
	ldr	r0, [r4, #2504]
	add	r3, r2, #592
	str	r2, [fp, #-128]
	str	r3, [fp, #-120]
	bl	FreeUsdByDec
	ldr	r2, [fp, #-128]
	ldr	r1, [r5, #-8]
	ldr	r0, [r4, #2504]
	add	r3, r2, #560
	str	r2, [fp, #-132]
	str	r3, [fp, #-128]
	bl	FreeUsdByDec
	ldr	r2, [fp, #-132]
	ldr	r0, [r4, #2504]
	ldr	r1, [r2, #576]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-120]
	ldr	r0, [r4, #2504]
	ldr	r1, [r3, #-8]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-128]
	mov	r0, #0
	mov	r1, #0
	mov	r2, r8
	strd	r0, [r3]
	ldr	r3, [fp, #-120]
	strd	r0, [r5, #-8]
	strd	r0, [r5]
	strd	r0, [r3, #-8]
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r2, r8
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2504]
	str	r3, [r4, #2516]
	str	r3, [r4, #2524]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2524]
	str	r0, [r4, #2516]
.L971:
	add	r5, r6, #22528
	mov	r3, #0
	add	r5, r5, #104
	str	r3, [r4, #2512]
	ldr	r0, [r4, #2504]
	mov	r1, r5
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r4, #1604]
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L972
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L1075
.L972:
	ldr	r2, [r4, #2112]
	mov	r0, r6
	ldr	r1, [r4, #2108]
	bl	Frame_Init
	cmp	r0, #0
	beq	.L970
	ldr	r1, .L1077+56
	mov	r0, #0
	bl	dprint_vfmw
	b	.L970
.L946:
	movw	r1, #438
	mov	r0, r6
	bl	MP4_Record_HeadPacketInfo
	b	.L945
.L961:
	ldr	r1, [r4, #1536]
	ldr	r2, [r4, #1524]
	ldr	r3, [r8, #3712]
	ldr	ip, [r4, #1528]
	add	r2, r2, r1
	ldr	r0, [r4, #1544]
	ldr	r1, [r4, #1548]
	mla	r3, r3, r2, ip
	rsb	r2, r0, r1
	add	r2, r2, r3
	str	r2, [r4, #1552]
	str	r3, [r4, #1540]
	b	.L962
.L1074:
	mov	r1, r5
	mov	r0, r6
	bl	MP4_Record_HeadPacketInfo
	b	.L945
.L1068:
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L980
	ldr	r1, [r7, #560]
	add	r3, r7, #592
	ldr	r0, [r4, #2504]
	add	r9, r7, #576
	str	r3, [fp, #-120]
	bl	FreeUsdByDec
	ldr	r1, [r7, #568]
	add	r3, r7, #560
	ldr	r0, [r4, #2504]
	str	r3, [fp, #-124]
	bl	FreeUsdByDec
	ldr	r1, [r7, #576]
	ldr	r0, [r4, #2504]
	bl	FreeUsdByDec
	ldr	r7, [fp, #-120]
	ldr	r0, [r4, #2504]
	ldr	r1, [r7, #-8]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-124]
	mov	r0, #0
	mov	r1, #0
	mov	r2, r5
	strd	r0, [r3]
	strd	r0, [r9, #-8]
	strd	r0, [r9]
	strd	r0, [r7, #-8]
	ldr	r1, [r4, #2524]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r2, r5
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2504]
	str	r3, [r4, #2516]
	str	r3, [r4, #2524]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #2524]
	str	r0, [r4, #2516]
.L980:
	add	r5, r6, #22528
	mov	r3, #0
	add	r5, r5, #104
	str	r3, [r4, #2512]
	ldr	r0, [r4, #2504]
	mov	r1, r5
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r4, #1604]
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L981
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L1076
.L981:
	mov	r0, r6
	ldr	r2, [r4, #1516]
	ldr	r1, [r4, #1520]
	bl	Frame_Init
	cmp	r0, #0
	beq	.L979
	ldr	r1, .L1077+60
	mov	r0, #0
	bl	dprint_vfmw
	b	.L979
.L1075:
	ldr	r3, [r4, #2508]
	cmp	r3, #0
	bne	.L972
	mov	r0, r5
	bl	ResetVoQueue
	b	.L972
.L1076:
	ldr	r3, [r4, #2508]
	cmp	r3, #0
	bne	.L981
	mov	r0, r5
	bl	ResetVoQueue
	b	.L981
.L1078:
	.align	2
.L1077:
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
	mov	r2, r0
	ldr	r3, [r5, #1640]
	cmp	r3, #0
	beq	.L1091
	add	r3, r0, #12288
	ldrb	r4, [r3, #3446]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L1092
	cmp	r4, #0
	bne	.L1087
	ldrb	r1, [r3, #3526]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L1087
	ldr	ip, .L1095
	add	r0, r0, #22016
	ldr	lr, [r5, #1656]
	add	r1, r2, #15808
	ldr	r3, [r3, #3456]
	mov	r2, #80
	ldr	ip, [ip, #52]
	add	r0, r0, #184
	add	r3, r3, lr
	str	r3, [r5, #1656]
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1092:
	ldrb	r4, [r5, #1646]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L1093
	cmp	r4, #0
	bne	.L1086
	ldrb	r2, [r5, #1726]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1086
	ldr	r3, [r3, #3456]
	mov	r0, r4
	ldr	r2, [r5, #1736]
	add	r3, r3, r2
	str	r3, [r5, #1736]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1087:
	movw	r2, #5377
	ldr	r1, .L1095+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1093:
	ldr	r0, [r3, #3452]
	ldr	r1, [r5, #1652]
	cmp	r0, r1
	bcc	.L1094
	ldr	r3, [r3, #3456]
	mov	r0, #0
	ldr	r2, [r5, #1656]
	add	r3, r3, r2
	str	r3, [r5, #1656]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1086:
	movw	r2, #5363
	ldr	r1, .L1095+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1094:
	ldr	r3, .L1095
	add	r1, r2, #15680
	add	r0, r2, #22016
	add	r1, r1, #48
	mov	r2, #80
	add	r0, r0, #184
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #0
	strb	r4, [r5, #1726]
	mov	r0, r3
	strb	r3, [r5, #1646]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1091:
	ldr	r2, .L1095+8
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L1081
	ldr	r0, [r5, #2504]
	mov	r2, r3
	mov	r1, #103
	blx	ip
.L1081:
	ldr	r1, .L1095+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1096:
	.align	2
.L1095:
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
	beq	.L1100
	ldr	r5, .L1101
	mov	r0, r4
	bl	MP4_Record_PacketID
	add	r1, r4, #22016
	add	r0, r4, #15680
	add	r1, r1, #104
	ldr	r3, [r5, #52]
	mov	r2, #160
	add	r0, r0, #48
	blx	r3
	add	r1, r4, #22272
	add	r0, r4, #15744
	ldr	r3, [r5, #52]
	add	r1, r1, #8
	mov	r2, #4
	add	r0, r0, #56
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1100:
	add	r3, r4, #20480
	add	r1, r4, #15680
	add	r1, r1, #48
	ldr	r0, [r3, #2504]
	bl	MP4_ClearCurPacket
	movw	r2, #5393
	ldr	r1, .L1101+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1102:
	.align	2
.L1101:
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
	ldr	r2, [r4, #3452]
	mov	r6, r0
	sub	r0, fp, #60
	str	r2, [r5, #1592]
	ldr	r1, [r4, #3440]
	str	r1, [r5, #1588]
	ldr	r2, [r4, #3456]
	str	r3, [r5, #1600]
	str	r2, [r5, #1596]
	bl	BsInit
	sub	r0, fp, #60
	mov	r1, #32
	bl	BsShow
	ldrb	r2, [r4, #3600]	@ zero_extendqisi2
	tst	r2, #1
	mov	r3, r0
	beq	.L1129
	mov	r3, #1
	strb	r3, [r5, #821]
	ldrb	r3, [r4, #3600]	@ zero_extendqisi2
	tst	r3, #2
	bne	.L1106
	ldr	r2, [r5, #2064]
	mov	r0, #196
.L1110:
	movw	r3, #438
	cmp	r0, r3
	cmpeq	r2, r3
	beq	.L1117
	cmp	r0, #196
	cmpeq	r2, #196
	bne	.L1127
.L1117:
	ldr	r3, [r5, #2072]
	cmp	r3, #1
	bgt	.L1109
	b	.L1127
.L1129:
	mov	r2, r0, lsr #8
	mov	r1, #2
	sub	r2, r2, #128
	strb	r1, [r5, #821]
	cmp	r2, #3
	bls	.L1130
.L1105:
	ldrb	r2, [r4, #3600]	@ zero_extendqisi2
	tst	r2, #2
	ldr	r2, [r5, #2064]
	bne	.L1108
	movw	r1, #438
	cmp	r2, r1
	cmpne	r3, #196
	beq	.L1110
.L1109:
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
.L1127:
	mov	r0, #0
.L1114:
	ldr	r3, [r4, #3440]
	cmp	r3, #0
	beq	.L1120
	ldr	r3, [r4, #3452]
	cmp	r3, #0
	mvneq	r0, #0
.L1126:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1108:
	movw	r1, #438
	sub	r3, r3, #196
	rsb	r2, r1, r2
	clz	r3, r3
	clz	r2, r2
	mov	r3, r3, lsr #5
	mov	r2, r2, lsr #5
	orrs	r1, r3, r2
	beq	.L1111
	ldrb	r0, [r5, #1585]	@ zero_extendqisi2
	cmp	r0, #1
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L1119
.L1111:
	mov	r0, r6
	bl	MP4_Renew_CurPacket
	b	.L1114
.L1106:
	ldr	r3, [r5, #2064]
	movw	r2, #438
	rsb	r2, r2, r3
	clz	r2, r2
	mov	r2, r2, lsr #5
.L1119:
	ldr	r3, [r5, #2068]
	cmp	r3, #1
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r2, #0
	beq	.L1127
	ldr	r3, [r5, #1656]
	cmp	r3, #4
	bne	.L1127
	b	.L1111
.L1130:
	ldrb	r2, [r4, #3600]	@ zero_extendqisi2
	orr	r2, r2, r1
	strb	r2, [r4, #3600]
	b	.L1105
.L1120:
	mvn	r0, #0
	b	.L1126
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
	ldr	r3, [r4, #2092]
	cmp	r3, #0
	beq	.L1132
	ldr	r3, [r1, #16]
	cmp	r3, #0
	beq	.L1132
	ldr	r3, [r1, #12]
	cmp	r3, #0
	beq	.L1132
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L1132
	ldr	r1, .L1241
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r2, [r5, #12]
	ldr	r1, .L1241+4
	mov	r0, #7
	bl	dprint_vfmw
	ldr	r2, [r5, #16]
	ldr	r1, .L1241+8
	mov	r0, #7
	bl	dprint_vfmw
	ldrb	r2, [r5, #6]	@ zero_extendqisi2
	ldr	r1, .L1241+12
	mov	r0, #7
	bl	dprint_vfmw
	ldr	r2, [r5, #92]
	ldr	r1, .L1241+16
	mov	r0, #7
	bl	dprint_vfmw
	ldr	r2, [r5, #96]
	ldr	r1, .L1241+20
	mov	r0, #7
	bl	dprint_vfmw
	ldrb	r2, [r5, #86]	@ zero_extendqisi2
	ldr	r1, .L1241+24
	mov	r0, #7
	bl	dprint_vfmw
	ldr	r2, [r5, #16]
	ldr	r1, .L1241+28
	mov	r0, #7
	bl	dprint_vfmw
	ldr	r3, [r5]
	ldr	r1, .L1241+32
	mov	r0, #7
	add	r8, r6, #15680
	ldrb	r2, [r3]	@ zero_extendqisi2
	add	r8, r8, #48
	bl	dprint_vfmw
	ldr	r3, [r5]
	ldr	r1, .L1241+36
	mov	r0, #7
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	bl	dprint_vfmw
	ldr	r3, [r5]
	ldr	r1, .L1241+40
	mov	r0, #7
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	bl	dprint_vfmw
	ldr	r3, [r5]
	ldr	r1, .L1241+44
	mov	r0, #7
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bl	dprint_vfmw
	ldr	r3, .L1241+48
	mov	r2, #160
	mov	r1, r5
	mov	r0, r8
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, r8
	mov	r0, r6
	bl	MP4_GetScdUpMsg
	mov	r0, r6
	bl	MP4_Check_CurPacket
	cmn	r0, #1
	mov	r7, r0
	beq	.L1227
	mov	r1, r8
	mov	r0, r6
	bl	MP4_GetScdUpMsg
	add	r3, r6, #12288
	mov	r2, #0
	mov	r0, r6
	ldr	r1, [r3, #3452]
	str	r1, [r4, #1592]
	ldr	r1, [r3, #3440]
	str	r1, [r4, #1588]
	ldr	r3, [r3, #3456]
	str	r2, [r4, #1600]
	str	r3, [r4, #1596]
	bl	Mp4_Get_DecMode
	ldr	r3, [r4, #1604]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L1228
.L1136:
	add	r7, r6, #16384
	ldr	r3, [r7, #3608]
	str	r3, [r4, #2544]
.L1137:
	ldr	r2, [r4, #1596]
	mov	r0, r6
	ldr	r1, [r4, #1588]
	bl	Mp4_Syntax_Parse
	ldr	lr, [r4, #1604]
	ldr	ip, [r4, #1588]
	ldr	r2, [r4, #1592]
	ldr	r3, [r4, #1596]
	mov	r1, r0, lsr #3
	and	r0, r0, #7
	rsb	r3, r1, r3
	add	ip, ip, r1
	str	r0, [r4, #1600]
	add	r2, r1, r2
	str	ip, [r4, #1588]
	str	r2, [r4, #1592]
	str	r3, [r4, #1596]
	ldr	r3, [lr, #832]
	cmp	r3, #1
	beq	.L1229
.L1138:
	ldr	r3, [r7, #3608]
	tst	r3, #288
	beq	.L1139
	bic	r3, r3, #432
	bic	r3, r3, #2
	str	r3, [r7, #3608]
	ldr	r3, [r4, #1636]
	cmp	r3, #2
	beq	.L1230
	cmp	r3, #1
	beq	.L1231
	cmp	r3, #4
	bne	.L1142
	ldrb	r3, [r4, #1456]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1144
	ldr	r3, [r4, #2120]
	cmp	r3, #0
	moveq	r3, #2
	movne	r3, #1
	str	r3, [r4, #2120]
.L1142:
	mov	r0, r6
	bl	Mp4_Frame_Atr
	cmn	r0, #1
	mov	r7, r0
	beq	.L1232
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #2524]
	mov	r8, r0
	ldr	r0, [r4, #2504]
	bl	FSP_GetFsImagePtr
	clz	r3, r8
	mov	r3, r3, lsr #5
	clz	r2, r0
	mov	ip, r0
	mov	r2, r2, lsr #5
	orrs	r9, r2, r3
	bne	.L1233
	ldrb	r1, [r4, #819]	@ zero_extendqisi2
	and	r10, r1, #253
	cmp	r10, #1
	beq	.L1234
	cmp	r1, #2
	beq	.L1235
.L1153:
	ldrb	r3, [r4, #1456]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1163
	ldr	r3, [r4, #2512]
	ldr	r1, [r4, #2516]
	cmp	r3, #0
	ldr	r3, [r4, #2524]
	movgt	r2, #2
	movle	r2, #1
	cmp	r1, r3
	str	r2, [r4, #2512]
	beq	.L1161
	mov	r2, #0
	ldr	r0, [r4, #2504]
	bl	FSP_SetRef
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_GetDisplay
	cmp	r0, #3
	beq	.L1224
	mov	r2, #0
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_SetDisplay
.L1224:
	ldr	r3, [r4, #2524]
.L1161:
	str	r3, [r4, #2516]
	mov	r2, #1
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	strb	r3, [r6]
	ldr	r3, [r4, #2528]
	ldr	r1, [r4, #2532]
	str	r3, [r4, #2520]
	str	r1, [r4, #2524]
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	strb	r3, [r6, #1]
	ldr	r3, [r4, #1548]
	ldr	r0, [r4, #2504]
	str	r3, [r4, #2528]
	bl	FSP_SetRef
	ldr	r1, [r4, #2516]
	ldr	r0, [r4, #2504]
	bl	FSP_RemovePmv
	ldrb	r3, [r4, #1456]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L1163
	ldrb	r8, [r4, #816]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L1236
.L1174:
	mov	r7, #0
.L1219:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1234:
	cmp	r3, #0
	bne	.L1151
	ldr	r3, [r4, #1604]
	ldr	r2, [r8, #160]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L1153
	mov	r2, r10
	ldr	r1, [r4, #2532]
	ldr	r0, [r4, #2504]
	mvn	r7, #0
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	r3, [r4, #1604]
	ldr	r2, [r8, #160]
	mov	r0, r10
	ldr	r1, .L1241+52
	ldr	r3, [r3]
	bl	dprint_vfmw
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1144:
	sub	r2, r3, #2
	tst	r2, #253
	bne	.L1147
	ldr	r2, [r4, #2120]
	cmp	r2, #2
	beq	.L1225
.L1147:
	cmp	r3, #1
	streq	r3, [r4, #2120]
	b	.L1142
.L1229:
	ldr	r3, [lr, #872]
	cmp	r3, #0
	beq	.L1138
	b	.L1174
.L1228:
	ldr	r3, [r3, #872]
	cmp	r3, #1
	bne	.L1136
	ldr	r3, [r4, #2544]
	add	r7, r6, #16384
	str	r3, [r7, #3608]
	b	.L1137
.L1163:
	mov	r2, r5
	mov	r1, #0
	mov	r0, r6
	bl	Mp4_Set_DecPar
	ldrb	r7, [r4, #1456]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L1237
	ldr	r3, [r4, #2120]
	cmp	r3, #0
	moveq	r3, #2
	movne	r3, #1
	str	r3, [r4, #2120]
	add	r3, r6, #1040
	str	r3, [r6, #1036]
	ldr	r1, [r4, #1604]
	ldr	r2, [r1, #864]
	cmp	r2, #1
	beq	.L1173
.L1172:
	cmp	r2, #2
	beq	.L1179
.L1173:
	cmp	r7, #0
	mov	r3, #0
	strb	r3, [r6, #1198]
	bne	.L1174
	ldr	r3, .L1241+56
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1174
	sub	r2, fp, #44
	ldr	r0, [r4, #2504]
	mov	r3, #4
	mov	r1, #20
	str	r7, [r2, #-4]!
	blx	ip
	b	.L1219
.L1237:
	sub	r3, r7, #2
	sub	r9, r7, #2
	clz	r9, r9
	ands	r5, r3, #253
	mov	r9, r9, lsr #5
	bne	.L1167
	ldr	r3, [r4, #2120]
	cmp	r3, #2
	beq	.L1238
.L1167:
	cmp	r7, #1
	beq	.L1239
	add	r3, r6, #1040
	str	r3, [r6, #1036]
	ldr	r1, [r4, #1604]
	ldr	r2, [r1, #864]
	cmp	r2, #1
	bne	.L1172
	cmp	r3, #0
	moveq	r9, #0
	andne	r9, r9, #1
	cmp	r9, #0
	beq	.L1173
	mov	r3, #0
	str	r3, [r6, #1036]
	ldr	r1, [r4, #2532]
	mvn	r7, #0
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1219
.L1230:
	ldrb	r3, [r4, #1456]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1142
.L1141:
	mov	r0, r6
	mvn	r7, #0
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1219
.L1231:
	ldrb	r3, [r4, #1456]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L1142
	b	.L1141
.L1236:
	ldr	r1, .L1241+60
	mov	r0, #11
	bl	dprint_vfmw
	ldr	r1, [r4, #2532]
	ldr	r0, [r4, #2504]
	bl	FSP_GetLogicFs
	ldr	r1, [r4, #2516]
	mov	r5, r0
	ldr	r0, [r4, #2504]
	bl	FSP_GetLogicFs
	cmp	r5, #0
	cmpne	r0, #0
	mov	r9, r0
	beq	.L1240
	ldr	r2, [r5, #640]
	cmp	r2, #0
	beq	.L1176
	ldr	r3, [r5, #648]
	cmp	r3, #0
	beq	.L1176
	ldr	r3, [r5, #644]
	cmp	r3, #0
	beq	.L1176
	ldr	r1, [r0, #640]
	mov	r0, r6
	ldr	r2, [r2, #16]
	mov	r7, #0
	cmp	r1, #0
	ldr	lr, [r4, #1516]
	ldr	ip, [r4, #1520]
	ldrne	r1, [r1, #16]
	moveq	r1, r2
	ldr	r3, [r3, #16]
	stmib	sp, {ip, lr}
	str	r1, [sp]
	mov	r1, r5
	bl	Vdm_Mem_Move
	b	.L1219
.L1239:
	str	r7, [r4, #2120]
	add	r3, r6, #1040
	str	r3, [r6, #1036]
	ldr	r1, [r4, #1604]
	ldr	r2, [r1, #864]
	cmp	r2, #1
	beq	.L1170
	cmp	r2, #2
	beq	.L1179
.L1170:
	mov	r3, #0
	strb	r3, [r6, #1198]
	b	.L1174
.L1132:
	ldr	r1, .L1241+64
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1235:
	cmp	r3, #0
	bne	.L1154
	ldr	r3, [r4, #1604]
	ldr	r1, [r8, #160]
	ldr	r3, [r3]
	cmp	r1, r3
	bhi	.L1155
	cmp	r2, #0
	bne	.L1156
	ldr	r2, [r0, #160]
	cmp	r3, r2
	bcs	.L1153
.L1156:
	ldr	r1, [r4, #2532]
	mov	r2, #1
	ldr	r0, [r4, #2504]
	mvn	r7, #0
	str	ip, [fp, #-56]
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	r3, [r4, #1604]
	ldr	ip, [fp, #-56]
	mov	r0, #1
	ldr	r1, .L1241+68
	ldr	r3, [r3]
	ldr	r2, [ip, #160]
	bl	dprint_vfmw
	b	.L1219
.L1155:
	ldr	r1, [r4, #2532]
	mov	r2, #1
	ldr	r0, [r4, #2504]
	mvn	r7, #0
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	ldr	r3, [r4, #1604]
	ldr	r2, [r8, #160]
	mov	r0, #1
	ldr	r1, .L1241+72
	ldr	r3, [r3]
	bl	dprint_vfmw
	b	.L1219
.L1238:
	mov	r2, #1
	ldr	r1, [r4, #2532]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	ldr	r3, .L1241+56
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1225
	ldr	r0, [r4, #2504]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #127
	blx	ip
	mvn	r7, #0
	b	.L1219
.L1233:
	movw	r3, #3931
	ldr	r2, .L1241+76
	ldr	r1, .L1241+80
	mov	r0, #1
	bl	dprint_vfmw
.L1225:
	mvn	r7, #0
	b	.L1219
.L1179:
	ldr	r2, [r1, #868]
	cmp	r2, #0
	andgt	r9, r9, #1
	movle	r9, #0
	cmp	r3, #0
	moveq	r3, #0
	andne	r3, r9, #1
	cmp	r3, #0
	beq	.L1173
	mov	r3, #0
	mov	r2, #1
	str	r3, [r6, #1036]
	mvn	r7, #0
	ldr	r1, [r4, #2532]
	ldr	r0, [r4, #2504]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #1604]
	mov	r0, r6
	ldr	r3, [r2, #868]
	add	r3, r3, r7
	str	r3, [r2, #868]
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1219
.L1151:
	mov	r2, r10
	ldr	r1, [r4, #2532]
	ldr	r0, [r4, #2504]
	mvn	r7, #0
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	mov	r0, r10
	movw	r3, #3945
	ldr	r2, .L1241+76
	ldr	r1, .L1241+84
	bl	dprint_vfmw
	b	.L1219
.L1227:
	ldr	r1, .L1241+88
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1219
.L1139:
	ldr	r1, .L1241+92
	mov	r0, #2
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L1219
.L1232:
	ldr	r1, .L1241+96
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1219
.L1154:
	ldr	r1, [r4, #2532]
	mov	r2, #1
	ldr	r0, [r4, #2504]
	mvn	r7, #0
	bl	FSP_ClearLogicFs
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	movw	r3, #3965
	ldr	r2, .L1241+76
	mov	r0, #1
	ldr	r1, .L1241+84
	bl	dprint_vfmw
	b	.L1219
.L1176:
	ldr	r1, [r4, #2532]
	mov	r2, #1
	ldr	r0, [r4, #2504]
	mvn	r7, #0
	bl	FSP_ClearLogicFs
	ldr	r1, [r5, #644]
	ldr	r3, [r4, #2532]
	mov	r0, #0
	ldr	r2, [r4, #2516]
	str	r1, [sp, #12]
	ldr	ip, [r5, #648]
	ldr	lr, [r4, #2524]
	ldr	r1, .L1241+100
	str	ip, [sp, #8]
	ldr	ip, [r5, #640]
	str	lr, [sp]
	str	ip, [sp, #4]
	bl	dprint_vfmw
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1219
.L1240:
	ldr	r1, [r4, #2532]
	mov	r2, #1
	ldr	r0, [r4, #2504]
	mvn	r7, #0
	bl	FSP_ClearLogicFs
	mov	r3, r5
	movw	r2, #4171
	ldr	r1, .L1241+104
	str	r9, [sp]
	mov	r0, r8
	bl	dprint_vfmw
	mov	r0, r6
	bl	MP4_ClearCurPic
	mov	r0, r6
	bl	MP4_Clean_HeadPacket
	b	.L1219
.L1242:
	.align	2
.L1241:
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
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC93
	.word	g_event_report
	.word	.LC88
	.word	.LC72
	.word	.LC95
	.word	.LC94
	.word	.LANCHOR1+1356
	.word	.LC87
	.word	.LC92
	.word	.LC85
	.word	.LC91
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
	ldr	r0, [r0, #1520]
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
	ldr	r0, [r0, #1516]
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
	.type	__FUNCTION__.14593, %object
	.size	__FUNCTION__.14593, 15
__FUNCTION__.14593:
	.ascii	"Mp4_Set_DecPar\000"
	.space	1
	.type	__FUNCTION__.14727, %object
	.size	__FUNCTION__.14727, 23
__FUNCTION__.14727:
	.ascii	"Mp4_Syntax_Slice_NotSH\000"
	.space	1
	.type	__FUNCTION__.14741, %object
	.size	__FUNCTION__.14741, 20
__FUNCTION__.14741:
	.ascii	"Mp4_Syntax_Slice_SH\000"
	.type	__FUNCTION__.14756, %object
	.size	__FUNCTION__.14756, 14
__FUNCTION__.14756:
	.ascii	"MP4_Dec_Slice\000"
	.space	2
	.type	__FUNCTION__.14630, %object
	.size	__FUNCTION__.14630, 16
__FUNCTION__.14630:
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
	.type	MemArrange.14531, %object
	.size	MemArrange.14531, 1272
MemArrange.14531:
	.space	1272
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
