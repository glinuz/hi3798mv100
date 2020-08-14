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
	.file	"bitstream.c"
	.text
	.align	2
	.global	ZerosMS_32
	.type	ZerosMS_32, %function
ZerosMS_32:
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
	ldr	ip, .L10
	mov	r2, r0
	ldrb	r0, [ip, r0, lsr #24]	@ zero_extendqisi2
	cmp	r0, #8
	moveq	r2, r2, asl #8
	moveq	r1, #3
	bne	.L9
.L4:
	ldrb	r3, [ip, r2, lsr #24]	@ zero_extendqisi2
	mov	r2, r2, asl #8
	cmp	r3, #8
	add	r0, r0, r3
	ldmnefd	sp, {fp, sp, pc}
	subs	r1, r1, #1
	bne	.L4
	ldmfd	sp, {fp, sp, pc}
.L9:
	ldmfd	sp, {fp, sp, pc}
.L11:
	.align	2
.L10:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	ZerosMS_32, .-ZerosMS_32
	.align	2
	.global	BsInit
	.type	BsInit, %function
BsInit:
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
	bic	r3, r1, #3
	str	r2, [r0, #16]
	add	lr, r3, #8
	str	r3, [r0]
	str	lr, [r0, #4]
	and	r1, r1, #3
	ldr	r2, [r3]
	mov	ip, #0
	mov	r1, r1, asl #3
	rev	r2, r2
	str	r2, [r0, #8]
	ldr	r3, [r3, #4]
	str	r1, [r0, #20]
	rev	r3, r3
	str	ip, [r0, #24]
	str	r3, [r0, #12]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsInit, .-BsInit
	.align	2
	.global	BsPos
	.type	BsPos, %function
BsPos:
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
	ldr	r0, [r0, #24]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsPos, .-BsPos
	.align	2
	.global	BsGet
	.type	BsGet, %function
BsGet:
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
	ldr	r4, [r0, #20]
	mov	r3, r0
	add	lr, r1, r4
	cmp	lr, #32
	rsbgt	r2, r4, #32
	rsble	r0, r1, #32
	ldrgt	ip, [r0, #12]
	ldrgt	r5, [r0, #8]
	rsbgt	r0, r1, #32
	ldrle	r2, [r3, #8]
	movgt	ip, ip, lsr r2
	orrgt	r2, ip, r5, asl r4
	movle	r2, r2, asl r4
	cmp	lr, #31
	subgt	lr, lr, #32
	strgt	lr, [r3, #20]
	mov	r0, r2, lsr r0
	ldr	r2, [r3, #24]
	strle	lr, [r3, #20]
	add	r1, r2, r1
	ldrgt	r2, [r3, #4]
	str	r1, [r3, #24]
	ldrgt	r1, [r3, #12]
	strgt	r1, [r3, #8]
	ldrgt	r1, [r2], #4
	revgt	r1, r1
	strgt	r2, [r3, #4]
	strgt	r1, [r3, #12]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	BsGet, .-BsGet
	.align	2
	.global	BsShow
	.type	BsShow, %function
BsShow:
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
	ldr	ip, [r0, #20]
	add	r3, ip, r1
	cmp	r3, #32
	rsbgt	r3, ip, #32
	rsbgt	r1, r1, #32
	ldrgt	r2, [r0, #12]
	rsble	r1, r1, #32
	ldrgt	r0, [r0, #8]
	ldrle	r3, [r0, #8]
	movgt	r2, r2, lsr r3
	orrgt	r3, r2, r0, asl ip
	movle	r3, r3, asl ip
	mov	r0, r3, lsr r1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsShow, .-BsShow
	.align	2
	.global	BsSkip
	.type	BsSkip, %function
BsSkip:
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
	ldr	r2, [r0, #20]
	mov	r3, r0
	ldr	r0, [r0, #24]
	add	r2, r1, r2
	cmp	r2, #31
	add	r0, r0, r1
	str	r0, [r3, #24]
	subgt	r2, r2, #32
	ldrgt	r0, [r3, #4]
	ldrgt	ip, [r3, #12]
	strgt	r2, [r3, #20]
	strle	r2, [r3, #20]
	strgt	ip, [r3, #8]
	ldrgt	r2, [r0], #4
	revgt	r2, r2
	strgt	r0, [r3, #4]
	mov	r0, r1
	strgt	r2, [r3, #12]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsSkip, .-BsSkip
	.align	2
	.global	BsBack
	.type	BsBack, %function
BsBack:
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
	ldr	r2, [r0, #24]
	mov	r3, r0
	ldr	ip, [r0, #20]
	cmp	r1, r2
	movlt	r0, r1
	movge	r0, r2
	subs	ip, ip, r0
	rsb	r2, r0, r2
	str	r2, [r3, #24]
	bmi	.L26
	str	ip, [r3, #20]
	ldmfd	sp, {fp, sp, pc}
.L26:
	ldr	r2, [r3, #4]
	add	ip, ip, #32
	ldr	r1, [r3, #8]
	sub	lr, r2, #4
	str	lr, [r3, #4]
	str	r1, [r3, #12]
	ldr	r2, [r2, #-12]
	str	ip, [r3, #20]
	rev	r2, r2
	str	r2, [r3, #8]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsBack, .-BsBack
	.align	2
	.global	BsBitsToNextByte
	.type	BsBitsToNextByte, %function
BsBitsToNextByte:
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
	ldr	r0, [r0, #24]
	add	r3, r0, #7
	bic	r3, r3, #7
	rsb	r0, r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsBitsToNextByte, .-BsBitsToNextByte
	.align	2
	.global	BsToNextByte
	.type	BsToNextByte, %function
BsToNextByte:
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
	ldr	r1, [r0, #24]
	ldr	r2, [r0, #20]
	add	r3, r1, #7
	bic	r3, r3, #7
	str	r3, [r0, #24]
	rsb	r3, r1, r3
	add	r2, r3, r2
	cmp	r2, #31
	strle	r2, [r0, #20]
	subgt	r2, r2, #32
	strgt	r2, [r0, #20]
	ldrgt	ip, [r0, #12]
	ldrgt	r1, [r0, #4]
	strgt	ip, [r0, #8]
	ldrgt	r2, [r1], #4
	revgt	r2, r2
	strgt	r1, [r0, #4]
	strgt	r2, [r0, #12]
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsToNextByte, .-BsToNextByte
	.align	2
	.global	BsResidBits
	.type	BsResidBits, %function
BsResidBits:
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
	ldr	r2, [r0, #16]
	ldr	r0, [r0, #24]
	rsb	r0, r0, r2, lsl #3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsResidBits, .-BsResidBits
	.align	2
	.global	BsIsByteAligned
	.type	BsIsByteAligned, %function
BsIsByteAligned:
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
	ldr	r3, [r0, #24]
	tst	r3, #7
	moveq	r0, #1
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsIsByteAligned, .-BsIsByteAligned
	.align	2
	.global	BsNextBitsByteAligned
	.type	BsNextBitsByteAligned, %function
BsNextBitsByteAligned:
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
	ldr	r2, [r0, #24]
	ldr	ip, [r0, #20]
	and	r3, r2, #7
	rsb	r3, r3, #8
	add	ip, r3, ip
	add	r2, r2, r3
	cmp	ip, #31
	str	r2, [r0, #24]
	bgt	.L39
	ldr	r2, [r0, #8]
	mov	r4, ip
	str	ip, [r0, #20]
.L36:
	add	ip, r1, r4
	cmp	ip, #32
	rsbgt	lr, r4, #32
	movle	r2, r2, asl r4
	ldrgt	ip, [r0, #12]
	rsble	r4, r1, #32
	rsbgt	r1, r1, #32
	movle	r4, r2, lsr r4
	movgt	ip, ip, lsr lr
	orrgt	r2, ip, r2, asl r4
	movgt	r4, r2, lsr r1
	mov	r1, r3
	bl	BsBack
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L39:
	ldr	r2, [r0, #12]
	sub	r4, ip, #32
	ldr	ip, [r0, #4]
	str	r4, [r0, #20]
	str	r2, [r0, #8]
	ldr	lr, [ip], #4
	rev	lr, lr
	str	lr, [r0, #12]
	str	ip, [r0, #4]
	b	.L36
	UNWIND(.fnend)
	.size	BsNextBitsByteAligned, .-BsNextBitsByteAligned
	.align	2
	.global	BsLongSkip
	.type	BsLongSkip, %function
BsLongSkip:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	beq	.L41
	cmp	r1, #64
	ldr	r3, [r0, #20]
	bgt	.L42
	cmp	r1, #32
	ldr	r2, [r0, #24]
	bgt	.L52
.L44:
	add	r3, r1, r3
	add	r2, r1, r2
	cmp	r3, #31
	str	r2, [r0, #24]
	subgt	r3, r3, #32
	strgt	r3, [r0, #20]
	ldrgt	ip, [r0, #12]
	ldrgt	r2, [r0, #4]
	strle	r3, [r0, #20]
	strgt	ip, [r0, #8]
	ldrgt	r3, [r2], #4
	revgt	r3, r3
	strgt	r2, [r0, #4]
	strgt	r3, [r0, #12]
.L41:
	mov	r0, r1
	ldmfd	sp, {r4, fp, sp, pc}
.L42:
	rsb	r2, r3, #32
	mov	ip, #0
	rsb	r1, r2, r1
	str	ip, [r0, #20]
	ldr	r3, [r0, #24]
	cmp	r1, #0
	add	ip, r1, #31
	ldr	r4, [r0, #4]
	add	r3, r2, r3
	movge	r2, r1
	movlt	r2, ip
	str	r3, [r0, #24]
	mov	r2, r2, asr #5
	ands	lr, r1, #31
	sub	ip, r2, #-1073741823
	add	r3, r3, r2, lsl #5
	add	r2, r4, ip, lsl #2
	str	r2, [r0, #4]
	ldr	ip, [r4, ip, asl #2]
	add	r4, r2, #4
	str	r4, [r0, #4]
	add	r4, r2, #8
	rev	ip, ip
	str	ip, [r0, #8]
	ldr	r2, [r2, #4]
	addne	r3, r3, lr
	str	r4, [r0, #4]
	rev	r2, r2
	strne	lr, [r0, #20]
	str	r2, [r0, #12]
	str	r3, [r0, #24]
	mov	r0, r1
	ldmfd	sp, {r4, fp, sp, pc}
.L52:
	add	ip, r3, #32
	add	r2, r2, #32
	cmp	ip, #31
	str	r2, [r0, #24]
	strle	ip, [r0, #20]
	sub	r1, r1, #32
	ldrgt	lr, [r0, #12]
	movle	r3, ip
	ldrgt	ip, [r0, #4]
	strgt	lr, [r0, #8]
	ldrgt	lr, [ip], #4
	revgt	lr, lr
	strgt	ip, [r0, #4]
	strgt	lr, [r0, #12]
	b	.L44
	UNWIND(.fnend)
	.size	BsLongSkip, .-BsLongSkip
	.align	2
	.global	BsSkipWithoutCount
	.type	BsSkipWithoutCount, %function
BsSkipWithoutCount:
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
	ldr	r3, [r0, #20]
	mov	r2, r0
	add	r3, r1, r3
	cmp	r3, #31
	strle	r3, [r2, #20]
	subgt	r3, r3, #32
	ldrgt	ip, [r0, #12]
	ldrgt	r0, [r0, #4]
	strgt	r3, [r2, #20]
	strgt	ip, [r2, #8]
	ldrgt	r3, [r0], #4
	revgt	r3, r3
	strgt	r0, [r2, #4]
	mov	r0, r1
	strgt	r3, [r2, #12]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsSkipWithoutCount, .-BsSkipWithoutCount
	.align	2
	.global	BsGetNextBytePtr
	.type	BsGetNextBytePtr, %function
BsGetNextBytePtr:
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
	ldr	r3, [r0, #20]
	ldr	r0, [r0, #4]
	rsb	r3, r3, #32
	add	r2, r3, #7
	cmp	r3, #0
	movlt	r3, r2
	mov	r3, r3, asr #3
	add	r3, r3, #4
	rsb	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	BsGetNextBytePtr, .-BsGetNextBytePtr
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	CalcZeroNum, %object
	.size	CalcZeroNum, 256
CalcZeroNum:
	.byte	8
	.byte	7
	.byte	6
	.byte	6
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
