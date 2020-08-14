#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"bitstream.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	ZerosMS_32
	.type	ZerosMS_32, %function
ZerosMS_32:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x4, .LC0
	ubfx	x0, x19, 24, 8
	lsl	w1, w19, 8
	mov	w2, 3
	ldrb	w0, [x4, x0]
	cmp	w0, 8
	bne	.L5
.L4:
	ubfx	x3, x1, 24, 8
	lsl	w1, w1, 8
	ldrb	w3, [x4, x3]
	cmp	w3, 8
	add	w0, w0, w3
	bne	.L5
	subs	w2, w2, #1
	bne	.L4
.L5:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ZerosMS_32, .-ZerosMS_32
	.align	3
.LC0:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsInit
	.type	BsInit, %function
BsInit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	w21, w2
	bl	_mcount
	and	x0, x20, -4
	str	w21, [x19, 24]
	add	x1, x0, 8
	ubfiz	x20, x20, 3, 2
	stp	x0, x1, [x19]
	ldr	w1, [x0]
	rev	w1, w1
	str	w1, [x19, 16]
	ldr	w0, [x0, 4]
	stp	w20, wzr, [x19, 28]
	rev	w0, w0
	str	w0, [x19, 20]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	BsInit, .-BsInit
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsPos
	.type	BsPos, %function
BsPos:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 32]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	BsPos, .-BsPos
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsGet
	.type	BsGet, %function
BsGet:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	w0, [x19, 28]
	add	w3, w20, w0
	cmp	w3, 32
	bgt	.L16
	ldr	w2, [x19, 16]
	mov	w1, 32
	sub	w1, w1, w20
	lsl	w0, w2, w0
	lsr	w0, w0, w1
.L13:
	ldr	w2, [x19, 32]
	ldr	w4, [x19, 24]
	add	w1, w20, w2
	str	w1, [x19, 32]
	cmp	w1, w4, lsl 3
	bcs	.L14
	cmp	w3, 31
	bgt	.L17
	str	w3, [x19, 28]
.L14:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L17:
	ldr	w1, [x19, 20]
	sub	w3, w3, #32
	ldr	x2, [x19, 8]
	str	w1, [x19, 16]
	str	w3, [x19, 28]
	ldr	w1, [x2], 4
	str	x2, [x19, 8]
	rev	w1, w1
	str	w1, [x19, 20]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L16:
	mov	w1, 32
	ldp	w2, w4, [x19, 16]
	sub	w5, w1, w0
	sub	w1, w1, w20
	lsl	w2, w2, w0
	lsr	w0, w4, w5
	orr	w0, w0, w2
	lsr	w0, w0, w1
	b	.L13
	.size	BsGet, .-BsGet
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsShow
	.type	BsShow, %function
BsShow:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	w0, [x19, 28]
	add	w1, w0, w20
	cmp	w1, 32
	bgt	.L21
	ldr	w2, [x19, 16]
	mov	w1, 32
	sub	w20, w1, w20
	lsl	w2, w2, w0
	lsr	w0, w2, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L21:
	ldp	w3, w4, [x19, 16]
	mov	w1, 32
	sub	w5, w1, w0
	lsl	w2, w3, w0
	lsr	w0, w4, w5
	sub	w1, w1, w20
	orr	w0, w0, w2
	lsr	w0, w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	BsShow, .-BsShow
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsSkip
	.type	BsSkip, %function
BsSkip:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	w0, [x19, 32]
	ldp	w1, w2, [x19, 24]
	add	w0, w20, w0
	str	w0, [x19, 32]
	cmp	w0, w1, lsl 3
	bcs	.L23
	add	w0, w2, w20
	cmp	w0, 31
	bgt	.L25
	str	w0, [x19, 28]
.L23:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L25:
	ldr	w2, [x19, 20]
	sub	w0, w0, #32
	ldr	x1, [x19, 8]
	str	w2, [x19, 16]
	str	w0, [x19, 28]
	ldr	w0, [x1], 4
	str	x1, [x19, 8]
	rev	w0, w0
	str	w0, [x19, 20]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	BsSkip, .-BsSkip
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsBack
	.type	BsBack, %function
BsBack:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	w0, [x19, 32]
	mov	w2, w20
	cmp	w0, w20
	bcs	.L27
	mov	w20, w0
	mov	w2, w0
.L27:
	ldr	w1, [x19, 28]
	sub	w0, w0, w2
	str	w0, [x19, 32]
	subs	w1, w1, w20
	bmi	.L28
	str	w1, [x19, 28]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L28:
	ldr	x0, [x19, 8]
	add	w1, w1, 32
	ldr	w2, [x19, 16]
	str	w2, [x19, 20]
	sub	x2, x0, #4
	str	x2, [x19, 8]
	ldr	w0, [x0, -12]
	str	w1, [x19, 28]
	rev	w0, w0
	str	w0, [x19, 16]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	BsBack, .-BsBack
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsBitsToNextByte
	.type	BsBitsToNextByte, %function
BsBitsToNextByte:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 32]
	ldr	x19, [sp, 16]
	add	w0, w1, 7
	and	w0, w0, -8
	sub	w0, w0, w1
	ldp	x29, x30, [sp], 32
	ret
	.size	BsBitsToNextByte, .-BsBitsToNextByte
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsToNextByte
	.type	BsToNextByte, %function
BsToNextByte:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w3, [x19, 32]
	ldp	w4, w2, [x19, 24]
	add	w1, w3, 7
	and	w0, w1, -8
	str	w0, [x19, 32]
	cmp	w0, w4, lsl 3
	sub	w0, w0, w3
	bcs	.L32
	add	w1, w0, w2
	cmp	w1, 31
	bgt	.L34
	str	w1, [x19, 28]
.L32:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L34:
	ldr	w3, [x19, 20]
	sub	w1, w1, #32
	ldr	x2, [x19, 8]
	str	w3, [x19, 16]
	str	w1, [x19, 28]
	ldr	w1, [x2], 4
	str	x2, [x19, 8]
	rev	w1, w1
	str	w1, [x19, 20]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	BsToNextByte, .-BsToNextByte
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsResidBits
	.type	BsResidBits, %function
BsResidBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 24]
	ldr	w1, [x19, 32]
	lsl	w0, w0, 3
	ldr	x19, [sp, 16]
	sub	w0, w0, w1
	ldp	x29, x30, [sp], 32
	ret
	.size	BsResidBits, .-BsResidBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsIsByteAligned
	.type	BsIsByteAligned, %function
BsIsByteAligned:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 32]
	ldr	x19, [sp, 16]
	and	w0, w0, 7
	cmp	w0, wzr
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	BsIsByteAligned, .-BsIsByteAligned
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsNextBitsByteAligned
	.type	BsNextBitsByteAligned, %function
BsNextBitsByteAligned:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w1
	bl	_mcount
	ldr	w3, [x19, 32]
	mov	w4, 8
	ldp	w2, w0, [x19, 24]
	and	w1, w3, 7
	sub	w4, w4, w1
	add	w3, w3, w4
	str	w3, [x19, 32]
	cmp	w3, w2, lsl 3
	bcs	.L43
	add	w0, w4, w0
	cmp	w0, 31
	bgt	.L44
	ldr	w2, [x19, 16]
	str	w0, [x19, 28]
.L39:
	add	w1, w21, w0
	cmp	w1, 32
	bgt	.L45
.L41:
	mov	w20, 32
	lsl	w0, w2, w0
	sub	w20, w20, w21
	mov	w1, w4
	lsr	w20, w0, w20
	mov	x0, x19
	bl	BsBack
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L44:
	ldr	w2, [x19, 20]
	sub	w0, w0, #32
	ldr	x3, [x19, 8]
	str	w2, [x19, 16]
	str	w0, [x19, 28]
	ldr	w1, [x3], 4
	str	x3, [x19, 8]
	rev	w1, w1
	str	w1, [x19, 20]
	add	w1, w21, w0
	cmp	w1, 32
	ble	.L41
.L45:
	ldr	w3, [x19, 20]
	mov	w1, 32
	sub	w20, w1, w0
	lsl	w0, w2, w0
	lsr	w20, w3, w20
	sub	w1, w1, w21
	orr	w0, w20, w0
	lsr	w20, w0, w1
	mov	w1, w4
	mov	x0, x19
	bl	BsBack
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L43:
	ldr	w2, [x19, 16]
	b	.L39
	.size	BsNextBitsByteAligned, .-BsNextBitsByteAligned
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsLongSkip
	.type	BsLongSkip, %function
BsLongSkip:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	mov	w0, 0
	cbz	w19, .L47
	cmp	w19, 64
	bgt	.L48
	cmp	w19, 32
	bgt	.L49
	ldp	w1, w2, [x20, 24]
	ldr	w0, [x20, 32]
	lsl	w1, w1, 3
.L50:
	add	w0, w19, w0
	str	w0, [x20, 32]
	cmp	w0, w1
	mov	w0, w19
	bcs	.L47
	add	w2, w19, w2
	cmp	w2, 31
	bgt	.L59
	str	w2, [x20, 28]
.L47:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L48:
	ldp	w2, w1, [x20, 28]
	mov	w0, 32
	ldr	x3, [x20, 8]
	sub	w0, w0, w2
	sub	w19, w19, w0
	add	w2, w0, w1
	cmp	w19, wzr
	add	w0, w19, 31
	csel	w0, w0, w19, lt
	ands	w6, w19, 31
	asr	w0, w0, 5
	lsl	w1, w0, 2
	sub	w1, w1, #4
	add	w0, w2, w0, lsl 5
	stp	wzr, w2, [x20, 28]
	add	x2, x3, x1
	str	x2, [x20, 8]
	add	x5, x2, 4
	add	x4, x2, 8
	ldr	w1, [x3, x1]
	str	x5, [x20, 8]
	rev	w1, w1
	str	w1, [x20, 16]
	ldr	w1, [x2, 4]
	str	x4, [x20, 8]
	rev	w1, w1
	str	w1, [x20, 20]
	bne	.L54
	str	w0, [x20, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L49:
	ldr	w0, [x20, 32]
	ldp	w1, w2, [x20, 24]
	add	w0, w0, 32
	str	w0, [x20, 32]
	lsl	w1, w1, 3
	cmp	w0, w1
	bcs	.L51
	add	w3, w2, 32
	cmp	w3, 31
	bgt	.L60
	mov	w2, w3
	str	w3, [x20, 28]
.L51:
	sub	w19, w19, #32
	b	.L50
	.p2align 3
.L54:
	ldr	w2, [x20, 24]
	add	w1, w6, w0
	str	w1, [x20, 32]
	mov	w0, w19
	cmp	w1, w2, lsl 3
	bcs	.L47
	str	w6, [x20, 28]
	b	.L47
	.p2align 3
.L59:
	ldr	w1, [x20, 20]
	sub	w2, w2, #32
	ldr	x3, [x20, 8]
	str	w1, [x20, 16]
	str	w2, [x20, 28]
	ldr	w1, [x3], 4
	str	x3, [x20, 8]
	rev	w1, w1
	str	w1, [x20, 20]
	b	.L47
	.p2align 3
.L60:
	ldr	w3, [x20, 20]
	sub	w19, w19, #32
	ldr	x4, [x20, 8]
	str	w3, [x20, 16]
	ldr	w3, [x4], 4
	str	x4, [x20, 8]
	rev	w3, w3
	str	w3, [x20, 20]
	b	.L50
	.size	BsLongSkip, .-BsLongSkip
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsSkipWithoutCount
	.type	BsSkipWithoutCount, %function
BsSkipWithoutCount:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	w0, [x19, 28]
	add	w0, w20, w0
	cmp	w0, 31
	bgt	.L64
	str	w0, [x19, 28]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L64:
	ldr	w2, [x19, 20]
	sub	w0, w0, #32
	ldr	x1, [x19, 8]
	str	w2, [x19, 16]
	str	w0, [x19, 28]
	ldr	w0, [x1], 4
	str	x1, [x19, 8]
	rev	w0, w0
	str	w0, [x19, 20]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	BsSkipWithoutCount, .-BsSkipWithoutCount
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	BsGetNextBytePtr
	.type	BsGetNextBytePtr, %function
BsGetNextBytePtr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 28]
	mov	w1, 32
	ldr	x2, [x19, 8]
	sub	w0, w1, w0
	ldr	x19, [sp, 16]
	add	w1, w0, 7
	cmp	w0, wzr
	csel	w0, w1, w0, lt
	asr	w0, w0, 3
	add	w0, w0, 4
	ldp	x29, x30, [sp], 32
	sub	x0, x2, x0, sxtw
	ret
	.size	BsGetNextBytePtr, .-BsGetNextBytePtr
	.section	.rodata
	.align	3
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
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
