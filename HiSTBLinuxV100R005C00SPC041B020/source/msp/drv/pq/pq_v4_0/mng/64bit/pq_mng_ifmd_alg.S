#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"pq_mng_ifmd_alg.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	SearchPhase, %function
SearchPhase:
	stp	x29, x30, [sp, -272]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	str	x0, [x29, 120]
	mov	x0, x30
	mov	x19, x3
	mov	x23, x1
	mov	x28, x4
	add	x26, x29, 168
	stp	x2, x3, [x29, 104]
	bl	_mcount
	mov	w24, 0
	add	x0, x19, 428
	ldr	x19, .LC4
	add	x27, x29, 136
	mov	x25, x0
	str	x0, [x29, 96]
	.p2align 2
.L13:
	ldp	x8, x9, [x28]
	ldp	x6, x7, [x28, 16]
	mov	x2, 100
	mov	x1, x19
	add	x0, x29, 168
	ldr	w22, [x28, 28]
	ldp	w21, w20, [x28, 20]
	stp	x8, x9, [x27]
	stp	x6, x7, [x27, 16]
	bl	memcpy
	cmp	w20, w21, lsl 1
	blt	.L7
	mov	x0, 0
	mov	w1, 0
	b	.L5
	.p2align 3
.L21:
	ldr	w2, [x27, x0]
	add	x0, x0, 4
	cmp	w2, w21
	csinc	w1, w1, w1, ne
	cmp	x0, 20
	beq	.L20
.L5:
	ldr	w2, [x26, x0]
	cbz	w2, .L21
	ldr	w2, [x27, x0]
	add	x0, x0, 4
	cmp	w22, w2, lsl 5
	csinc	w1, w1, w1, ge
	cmp	x0, 20
	bne	.L5
.L20:
	cmp	w1, 4
	ble	.L7
	ldr	x0, [x29, 120]
	str	w24, [x0]
	ldr	x0, [x29, 112]
	ldrsb	w0, [x0, 109]
	add	w1, w0, 1
	cmp	w1, w24
	beq	.L8
	add	w1, w24, 4
	cmp	w1, w0
	beq	.L8
.L7:
	ldr	w0, [x23]
	add	w0, w0, 1
	str	w0, [x23]
.L10:
	add	w24, w24, 1
	add	x26, x26, 20
	cmp	w24, 5
	add	x25, x25, 4
	bne	.L13
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 272
	ret
	.p2align 3
.L8:
	ldr	x0, [x29, 104]
	mov	w1, 5
	str	w24, [x0]
	ldr	w0, [x25]
	add	w0, w0, 1
	cmp	w0, 5
	csel	w0, w0, w1, le
	cmp	w0, wzr
	csel	w0, w0, wzr, ge
	str	w0, [x25]
	cmp	w0, 3
	ble	.L10
	ldr	x1, [x29, 96]
	mov	w0, 0
	.p2align 2
.L9:
	cmp	w0, w24
	add	w0, w0, 1
	beq	.L11
	mov	w2, 3
	str	w2, [x1]
.L11:
	cmp	w0, 5
	add	x1, x1, 4
	bne	.L9
	b	.L10
	.size	SearchPhase, .-SearchPhase
	.align	3
.LC4:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	DetectSceneChange.isra.4, %function
DetectSceneChange.isra.4:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w21, w2
	mov	w22, w1
	mov	w19, w3
	bl	_mcount
	add	w3, w22, w21, lsl 1
	ldr	w4, [x20]
	add	w3, w3, w19, lsl 2
	mov	x1, x20
	lsl	w3, w3, 1
	mov	w6, 0
	mov	w2, 0
	mov	w5, w4
	b	.L25
	.p2align 3
.L28:
	ldr	w0, [x1, 4]
	add	w2, w2, 1
	str	w0, [x1]
	cmp	w2, 6
	add	x1, x1, 4
	beq	.L27
.L25:
	ldr	w0, [x1]
	cmp	w4, w0
	add	w6, w6, w0
	csel	w4, w4, w0, le
	cmp	w5, w0
	csel	w5, w5, w0, ge
	cmp	w2, 4
	ble	.L28
	add	w2, w2, 1
	str	w3, [x20, 20]
	cmp	w2, 6
	add	x1, x1, 4
	bne	.L25
.L27:
	sub	w4, w6, w4
	mov	w0, 3392
	sub	w5, w4, w5
	movk	w0, 0x3, lsl 16
	cmp	w5, wzr
	add	w6, w5, 3
	csel	w5, w6, w5, lt
	asr	w5, w5, 2
	lsl	w5, w5, 1
	cmp	w3, w5
	ccmp	w3, w0, 4, gt
	cset	w0, gt
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	DetectSceneChange.isra.4, .-DetectSceneChange.isra.4
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	Calc22PLD_4FLD, %function
Calc22PLD_4FLD:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w22, w1
	mov	x19, x2
	mov	w21, w3
	bl	_mcount
	cmp	w22, wzr
	ldr	w11, [x20, 28]
	cset	w8, eq
	ldp	w7, w6, [x20, 4]
	ldp	w13, w14, [x20, 12]
	ldp	w0, w1, [x20, 40]
	ldp	w10, w4, [x20, 120]
	ldp	w2, w12, [x20, 128]
	cbnz	w22, .L30
	mov	w9, w0
	mov	w5, w11
	ldp	w0, w3, [x19, 40]
.L31:
	add	w4, w10, w4, lsl 1
	cmp	w0, w3
	add	w4, w4, w2, lsl 2
	csel	w2, w0, w3, le
	lsl	w10, w2, 1
	cmp	w3, 80
	mov	w2, 80
	add	w4, w4, w12, lsl 3
	ccmp	w5, w2, 0, le
	ble	.L32
	lsl	w14, w5, 2
	lsl	w13, w3, 1
	sub	w2, w14, w5
	cmp	w2, w13
	bgt	.L101
	add	w2, w3, w3, lsl 2
	add	w2, w2, w2, lsr 31
	cmp	w0, w2, asr 1
	bgt	.L35
	cmp	w0, 1000
	ble	.L37
	lsl	w2, w3, 3
	add	w15, w0, w0, lsl 2
	sub	w2, w2, w3
	cmp	w15, w2
	bge	.L35
.L37:
	cmp	w5, 99
	ble	.L38
	uxtw	x0, w8
	cmp	w5, 499
	add	x2, x19, x0
	ldrb	w10, [x2, 88]
	mov	w2, 5
	bgt	.L39
	mov	w2, 34079
	movk	w2, 0x51eb, lsl 16
	smull	x2, w5, w2
	asr	x2, x2, 37
	sub	w2, w2, w5, asr 31
	uxtb	w2, w2
.L39:
	add	x0, x19, x0
	cmp	w9, 400
	sub	w2, w10, w2
	ccmp	w1, 29, 0, gt
	strb	w2, [x0, 88]
	mov	w2, 1
	ble	.L63
	.p2align 2
.L79:
	lsl	w1, w1, 5
	uxtw	x0, w8
	cmp	w1, 127
	ble	.L63
.L84:
	add	x0, x19, x0
	cmp	w2, 1
	ldrb	w1, [x0, 88]
	sub	w1, w1, #5
	strb	w1, [x0, 88]
	beq	.L83
.L64:
	mov	w2, 0
.L59:
	ldrsb	w0, [x19, 88]
	mov	w8, 32
	str	w2, [x19, 36]
	cmp	w0, 32
	ldrb	w0, [x19, 88]
	ldrsb	w2, [x19, 89]
	csel	w0, w0, w8, le
	ldrb	w1, [x19, 89]
	sxtb	w9, w0
	cmp	w9, wzr
	csel	w0, w0, wzr, ge
	cmp	w2, 32
	csel	w1, w1, w8, le
	strb	w0, [x19, 88]
	sxtb	w0, w0
	sxtb	w2, w1
	cmp	w2, wzr
	csel	w1, w1, wzr, ge
	cmp	w0, 24
	strb	w1, [x19, 89]
	sxtb	w1, w1
	ccmp	w1, 24, 4, gt
	ble	.L65
	mov	w9, 1
	str	w9, [x19, 92]
.L66:
	ldp	w8, w0, [x19, 8]
	ldr	w2, [x20]
	add	w1, w0, w8
	stp	w8, w0, [x19, 12]
	ldr	w0, [x19, 4]
	str	w0, [x19, 8]
	add	w8, w1, w0
	ldr	w1, [x19]
	stp	w2, w1, [x19]
	mov	w0, 26215
	add	w1, w8, w1
	add	w1, w1, w2
	movk	w0, 0x6666, lsl 16
	ldr	w2, [x19, 28]
	smull	x0, w1, w0
	asr	x0, x0, 33
	sub	w0, w0, w1, asr 31
	add	w1, w0, w2
	ldr	w2, [x19, 20]
	sub	w1, w1, w2
	cmn	w1, #2999
	stp	w0, w1, [x19, 24]
	blt	.L68
	mov	w2, 3000
	str	w0, [x19, 20]
	cmp	w1, w2
	csel	w1, w1, w2, le
	str	w1, [x19, 28]
	cbz	w9, .L81
	cmp	w1, 999
	ble	.L70
	strb	wzr, [x19, 88]
	strb	wzr, [x19, 89]
	str	wzr, [x19, 92]
.L70:
	ldrsb	w1, [x19, 100]
	mov	w2, 100
	ldrb	w0, [x19, 100]
	sub	w6, w6, w7
	cmp	w1, 100
	mov	w1, 34464
	csel	w0, w0, w2, le
	movk	w1, 0x1, lsl 16
	sxtb	w2, w0
	cmp	w2, wzr
	csel	w0, w0, wzr, ge
	cmp	w6, w1
	strb	w0, [x19, 100]
	ble	.L72
	add	w0, w0, 10
	sxtb	w0, w0
	strb	w0, [x19, 100]
.L73:
	cmp	w0, 59
	bgt	.L102
.L75:
	str	w4, [x19, 32]
	mov	w0, 1
	str	w0, [x19, 96]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L32:
	mov	w11, 5499
	cmp	w7, w11
	bgt	.L48
	cmp	w6, wzr
	mov	w2, 20
	csel	w2, w6, w2, ne
	cmp	w7, w2
	bgt	.L48
	cmp	w13, w11
	bgt	.L49
	add	w2, w14, 150
	cmp	w13, w2
	ble	.L50
.L49:
	cbz	w21, .L48
.L50:
	cmp	w5, 39
	ble	.L51
	cmp	w5, w3, lsl 1
	bge	.L48
.L51:
	cmp	w0, 80
	bgt	.L103
.L53:
	cmp	w6, 1000
	ble	.L55
	lsl	w2, w6, 1
	lsl	w0, w7, 4
	cmp	w2, w0
	ble	.L56
	cbnz	w21, .L57
	cmp	w14, w13, lsl 1
	ble	.L56
.L57:
	mov	w0, 9000
	cmp	w6, w0
	ble	.L52
	uxtw	x0, w8
	cmp	w9, 400
	add	x8, x19, x0
	ccmp	w1, 29, 0, gt
	ldrb	w2, [x8, 88]
	add	w2, w2, 1
	strb	w2, [x8, 88]
	ble	.L64
	lsl	w1, w1, 5
	cmp	w1, 127
	ble	.L64
	mov	w2, 0
	b	.L84
	.p2align 3
.L30:
	mov	w9, w1
	mov	w1, w0
	ldr	w0, [x20, 32]
	mov	w3, w11
	ldr	w5, [x20, 24]
	stp	w0, w11, [x19, 40]
	b	.L31
	.p2align 3
.L101:
	uxtw	x0, w8
	mov	w2, 1
	add	x0, x19, x0
	strb	wzr, [x0, 88]
.L34:
	cmp	w9, 400
	ccmp	w1, 29, 0, gt
	bgt	.L79
.L63:
	cmp	w2, 1
	bne	.L64
	b	.L83
	.p2align 3
.L48:
	uxtw	x0, w8
	add	x0, x19, x0
	strb	wzr, [x0, 88]
.L52:
	cmp	w1, 29
	mov	w0, 400
	ccmp	w9, w0, 4, le
	mov	w2, 1
	ble	.L79
.L83:
	ldr	w2, [x19, 36]
	mov	w0, 50
	add	w2, w2, 1
	cmp	w2, w0
	csel	w2, w2, w0, le
	cmp	w2, wzr
	csel	w2, w2, wzr, ge
	b	.L59
	.p2align 3
.L72:
	sub	w3, w3, w5
	mov	w1, 20000
	cmp	w3, 1000
	ccmp	w6, w1, 4, gt
	ble	.L74
	add	w0, w0, 6
	sxtb	w0, w0
	strb	w0, [x19, 100]
	cmp	w0, 59
	ble	.L75
.L102:
	str	wzr, [x19, 96]
	str	w4, [x19, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L68:
	mov	w1, -3000
	str	w0, [x19, 20]
	str	w1, [x19, 28]
	cbnz	w9, .L70
.L81:
	str	wzr, [x19, 28]
	b	.L70
	.p2align 3
.L65:
	cmp	w1, 19
	ccmp	w0, 19, 4, gt
	bgt	.L104
	mov	w9, 0
	str	wzr, [x19, 92]
	b	.L66
	.p2align 3
.L74:
	sub	w0, w0, #5
	sxtb	w0, w0
	strb	w0, [x19, 100]
	b	.L73
	.p2align 3
.L35:
	uxtw	x0, w8
	mov	w2, 1
	add	x0, x19, x0
	ldrb	w10, [x0, 88]
	sub	w10, w10, #1
	strb	w10, [x0, 88]
	b	.L34
	.p2align 3
.L104:
	ldr	w9, [x19, 92]
	b	.L66
	.p2align 3
.L38:
	lsl	w15, w5, 3
	cmp	w3, w15
	blt	.L40
	cmp	w5, 10
	mov	w2, 10
	csel	w2, w5, w2, ge
	lsl	w16, w0, 1
	cmp	w16, w2, lsl 3
	blt	.L40
.L41:
	cmp	w15, w13
	bge	.L42
	cmp	w15, w0, lsl 1
	bge	.L105
.L43:
	add	w12, w12, 100
	uxtw	x0, w8
	cmp	w14, w12
	add	x0, x19, x0
	ble	.L44
	ldrb	w10, [x0, 88]
	mov	w2, 1
	sub	w10, w10, #15
	strb	w10, [x0, 88]
	b	.L34
	.p2align 3
.L40:
	sub	w2, w3, w0
	cmp	w10, 150
	sxtw	x2, w2
	eor	x16, x2, x2, asr 63
	sub	x16, x16, x2, asr 63
	mov	w2, 150
	csel	w2, w10, w2, ge
	add	w2, w2, 50
	cmp	w2, w16
	bge	.L41
	ldr	w2, [x19, 32]
	cmp	w2, 10
	ble	.L41
	uxtw	x0, w8
	mov	w2, 1
	add	x0, x19, x0
	ldrb	w10, [x0, 88]
	sub	w10, w10, #5
	strb	w10, [x0, 88]
	b	.L34
	.p2align 3
.L55:
	uxtw	x0, w8
	add	x0, x19, x0
	ldrb	w2, [x0, 88]
	sub	w2, w2, #1
	strb	w2, [x0, 88]
	b	.L52
	.p2align 3
.L105:
	ldr	w0, [x19, 32]
	cmp	w0, 9
	ble	.L43
.L42:
	add	w0, w3, 150
	cmp	w5, w0
	ble	.L106
.L45:
	ldr	w2, [x19, 92]
	uxtw	x0, w8
	add	x0, x19, x0
	cmp	w2, 1
	beq	.L107
	ldrb	w10, [x0, 88]
	mov	w2, 1
	sub	w10, w10, #3
	strb	w10, [x0, 88]
	b	.L34
	.p2align 3
.L103:
	sub	w0, w3, w0
	add	w2, w10, 50
	sxtw	x0, w0
	eor	x11, x0, x0, asr 63
	sub	x11, x11, x0, asr 63
	cmp	w2, w11
	bge	.L53
	ldr	w0, [x19, 32]
	cmp	w0, 10
	ble	.L53
	ldr	w2, [x19, 92]
	cmp	w2, 1
	beq	.L61
	mov	w2, 5000
	cmp	w0, w2
	ble	.L52
.L61:
	uxtw	x0, w8
	ldr	w2, [x19, 36]
	add	x0, x19, x0
	mvn	w10, w2
	ldrb	w2, [x0, 88]
	add	w2, w10, w2
	strb	w2, [x0, 88]
	b	.L52
	.p2align 3
.L106:
	add	w11, w11, 150
	mov	w2, 1
	cmp	w5, w11
	bgt	.L45
	b	.L34
	.p2align 3
.L107:
	ldrb	w10, [x0, 88]
	sub	w10, w10, #1
	strb	w10, [x0, 88]
	b	.L34
	.p2align 3
.L44:
	ldrb	w10, [x0, 88]
	mov	w2, 0
	add	w10, w10, 1
	strb	w10, [x0, 88]
	b	.L34
	.p2align 3
.L56:
	ldr	w10, [x19, 92]
	cmp	w10, 1
	bne	.L55
	cmp	w2, w0
	bge	.L61
	cbnz	w21, .L62
	lsl	w13, w13, 4
	cmp	w13, w14, lsl 1
	ble	.L61
.L62:
	uxtw	x0, w8
	ldr	w2, [x19, 36]
	add	x0, x19, x0
	mvn	w2, w2
	ldrb	w10, [x0, 88]
	add	w2, w10, w2, lsl 1
	strb	w2, [x0, 88]
	b	.L52
	.size	Calc22PLD_4FLD, .-Calc22PLD_4FLD
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	calc_MCorMA
	.type	calc_MCorMA, %function
calc_MCorMA:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x4
	mov	w22, w0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	w23, w3
	bl	_mcount
	ldrsb	w1, [x19, 100]
	mov	w0, 100
	ldrb	w5, [x19, 100]
	cmp	w1, 100
	sub	w1, w20, w22
	csel	w5, w5, w0, le
	mov	w0, 34464
	movk	w0, 0x1, lsl 16
	sxtb	w2, w5
	cmp	w2, wzr
	csel	w5, w5, wzr, ge
	cmp	w1, w0
	strb	w5, [x19, 100]
	ble	.L109
	add	w5, w5, 10
	sxtb	w5, w5
	strb	w5, [x19, 100]
.L110:
	cmp	w5, 59
	bgt	.L114
.L112:
	mov	w0, 1
	str	w0, [x19, 96]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L109:
	sub	w21, w21, w23
	mov	w0, 20000
	cmp	w21, 1000
	ccmp	w1, w0, 4, gt
	ble	.L111
	add	w5, w5, 6
	sxtb	w5, w5
	strb	w5, [x19, 100]
	cmp	w5, 59
	ble	.L112
.L114:
	str	wzr, [x19, 96]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L111:
	sub	w5, w5, #5
	sxtb	w5, w5
	strb	w5, [x19, 100]
	b	.L110
	.size	calc_MCorMA, .-calc_MCorMA
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_FmdSet
	.type	PQ_MNG_ALG_FmdSet, %function
PQ_MNG_ALG_FmdSet:
	stp	x29, x30, [sp, -208]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	mov	x25, x2
	bl	_mcount
	ldr	w0, [x20]
	cbz	w0, .L364
	stp	xzr, xzr, [x25]
	str	xzr, [x25, 16]
	str	wzr, [x25, 24]
	ldr	w0, [x19, 68]
	cmp	w0, 1
	beq	.L365
	ldrb	w0, [x20, 7]
	add	x27, x20, 7
	cmp	w0, 255
	beq	.L119
	ldrb	w0, [x27, 1]
	cmp	w0, 255
	beq	.L119
	ldrb	w0, [x27, 2]
	cmp	w0, 255
	beq	.L119
	ldrb	w0, [x20, 4]
	add	x26, x20, 4
	cmp	w0, 255
	beq	.L121
.L377:
	ldrb	w0, [x26, 1]
	cmp	w0, 255
	beq	.L121
	ldr	w0, [x19, 48]
	cbz	w0, .L129
.L379:
	add	x21, x20, 16
	ldrb	w0, [x27, 2]
	str	w0, [x21, 8]
	ldr	w0, [x19, 48]
	cbz	w0, .L131
.L380:
	ldrb	w0, [x26, 2]
	str	w0, [x21, 12]
	mov	w0, 2
	ldr	w1, [x19, 48]
	cmp	w1, wzr
	csinc	w0, w0, wzr, eq
	str	w0, [x21, 16]
	ldr	w0, [x19, 48]
	cbz	w0, .L134
.L381:
	ldp	w3, w2, [x19, 60]
.L135:
	ldr	x0, [x21, 8]
	mov	x1, 4294967297
	cmp	x0, xzr
	ccmp	x0, x1, 4, ne
	orr	w0, w3, w2
	cset	w4, ne
	mov	w1, 0
	cbz	w0, .L136
	cmp	w3, 1
	ccmp	w2, 1, 0, eq
	cset	w1, ne
.L136:
	str	w4, [x20, 16]
	add	x0, x20, 162
	str	w1, [x21, 4]
	add	x4, x20, 258
	ldr	w2, [x21, 452]
	ldr	w1, [x21, 456]
	str	w1, [x21, 452]
	str	w2, [x21, 448]
	ldr	w1, [x21, 468]
	ldr	w2, [x19, 52]
	ldr	w3, [x21, 464]
	str	w2, [x21, 456]
	str	w3, [x21, 460]
	str	w1, [x21, 464]
	ldr	w1, [x19, 56]
	str	w1, [x21, 468]
	ldp	w2, w1, [x19, 40]
	mul	w1, w1, w2
	asr	w1, w1, 11
	str	w1, [x20, 488]
	.p2align 2
.L137:
	ldrh	w3, [x0]
	ldrh	w2, [x0, -2]
	strh	w3, [x0, 2]
	strh	w2, [x0], 6
	cmp	x4, x0
	bne	.L137
	add	x2, x19, 236
	add	x0, x20, 160
	add	x4, x20, 256
	mov	w5, 0
	.p2align 2
.L139:
	ldrh	w3, [x0, 4]
	cmp	w1, w3
	ble	.L138
	ldrh	w3, [x0]
	cmp	w1, w3
	ble	.L138
	ldrh	w3, [x0, 2]
	cmp	w1, w3
	csinc	w5, w5, w5, le
.L138:
	ldr	w3, [x2], 4
	strh	w3, [x0], 6
	cmp	x0, x4
	bne	.L139
	ldr	w0, [x19, 20]
	str	w5, [x19, 232]
	cmp	w0, 1
	beq	.L366
	cmp	w0, 2
	beq	.L367
	cmp	w0, 3
	beq	.L368
.L141:
	ldr	w28, [x20, 16]
	mov	w22, 2
.L187:
	add	x5, x20, 136
	ldr	w3, [x19, 216]
	ldp	w1, w2, [x19, 208]
	mov	x0, x5
	str	x5, [x29, 104]
	uxtw	x24, w28
	bl	DetectSceneChange.isra.4
	add	x1, x21, x24
	ldr	x5, [x29, 104]
	strb	w0, [x1, 107]
	cbz	w28, .L283
	ldrsb	w0, [x21, 107]
	cbnz	w0, .L369
.L283:
	mov	w23, 0
	str	wzr, [x29, 104]
.L189:
	ldp	w1, w2, [x19, 208]
	ldr	w3, [x19, 216]
	mov	x0, x5
	bl	DetectSceneChange.isra.4
	add	x4, x21, x24
	strb	w0, [x4, 107]
	ldr	w0, [x19, 24]
	cbz	w0, .L284
	ldr	x0, .LC7
	ldr	w2, [x19, 76]
	ldr	w0, [x0, 16]
	cmp	w0, 1
	add	x0, x19, 84
	beq	.L370
	cmp	w2, 1
	beq	.L371
.L198:
	ldr	w1, [x19, 40]
	cmp	w1, 288
	bgt	.L199
	ldr	w1, [x19, 72]
.L360:
	cmp	w1, 1
	beq	.L288
.L199:
	ldr	w1, [x21, 448]
	cbz	w1, .L372
	ldr	w0, [x19, 28]
	ldr	w28, [x21, 248]
	cbnz	w0, .L373
	ldrsb	w0, [x20, 120]
	ldrsb	w1, [x20, 121]
	cmp	w0, 1
	beq	.L303
	cmp	w0, 2
	bne	.L374
	mov	w2, 0
	mov	w4, 0
.L231:
	ldr	w5, [x21, 16]
.L237:
	cmp	w28, 1
	beq	.L357
	cmp	w2, 1
	mov	w1, 2
	cset	w0, eq
.L234:
	ldr	x6, .LC9
	add	x9, x29, 168
	strb	w1, [x20, 121]
	cmp	w0, 1
	add	x1, x6, 104
	strb	w0, [x20, 120]
	ldr	x2, [x6, 136]
	str	x2, [x29, 200]
	ldp	x2, x3, [x1]
	stp	x2, x3, [x9]
	ldp	x2, x3, [x1, 16]
	add	x1, x29, 184
	ldr	w8, [x19, 8]
	ldr	w7, [x20, 20]
	stp	x2, x3, [x1]
	beq	.L243
	cmp	w0, 2
	beq	.L244
	cbz	w0, .L245
	stp	wzr, wzr, [x25, 4]
	tbz	x5, 0, .L254
	cmp	w7, wzr
	cset	w1, eq
	str	w1, [x25]
.L255:
	cmp	w0, 2
	str	w22, [x25, 20]
	stp	wzr, w23, [x25, 12]
	beq	.L298
	cmp	w0, 1
	csetm	w1, ne
	mov	w0, w1
.L260:
	cmp	w1, 1
	bls	.L375
.L257:
	str	w0, [x25, 24]
	ldrb	w0, [x27, 1]
	strb	w0, [x20, 7]
	ldrb	w0, [x27, 2]
	strb	w0, [x27, 1]
	ldr	w0, [x19, 20]
	cmp	w0, 1
	beq	.L376
.L258:
	add	x0, x20, 492
	ldp	x2, x3, [x25]
	stp	x2, x3, [x0]
	ldr	x1, [x25, 16]
	str	x1, [x0, 16]
	ldr	w1, [x25, 24]
	str	w1, [x20, 516]
	ldrb	w0, [x26, 1]
	strb	w0, [x20, 4]
	ldrb	w0, [x26, 2]
	strb	w0, [x26, 1]
	ldr	w0, [x19, 64]
	strb	w0, [x26, 2]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L119:
	ldr	w0, [x19, 60]
	add	x26, x20, 4
	strb	w0, [x20, 7]
	ldr	w0, [x19, 60]
	strb	w0, [x27, 1]
	ldr	w0, [x19, 60]
	strb	w0, [x27, 2]
	ldrb	w0, [x20, 4]
	cmp	w0, 255
	bne	.L377
.L121:
	ldr	w0, [x19, 48]
	cbnz	w0, .L378
	ldrb	w0, [x19, 60]
	strb	w0, [x20, 4]
	ldr	w0, [x19, 48]
	cbz	w0, .L125
.L383:
	ldrb	w0, [x19, 60]
	strb	w0, [x26, 1]
	ldr	w0, [x19, 48]
	cbz	w0, .L127
.L384:
	ldr	w0, [x19, 60]
	cmp	w0, wzr
	cset	w0, eq
	strb	w0, [x26, 2]
.L385:
	ldr	w0, [x19, 48]
	cbnz	w0, .L379
.L129:
	add	x21, x20, 16
	ldrb	w0, [x27, 1]
	str	w0, [x21, 8]
	ldr	w0, [x19, 48]
	cbnz	w0, .L380
.L131:
	ldrb	w0, [x26, 1]
	str	w0, [x21, 12]
	mov	w0, 2
	ldr	w1, [x19, 48]
	cmp	w1, wzr
	csinc	w0, w0, wzr, eq
	str	w0, [x21, 16]
	ldr	w0, [x19, 48]
	cbnz	w0, .L381
.L134:
	ldrb	w2, [x26, 2]
	ldrb	w3, [x27, 2]
	b	.L135
	.p2align 3
.L364:
	stp	wzr, wzr, [x25]
	stp	wzr, wzr, [x25, 8]
	add	x0, x20, 492
	ldr	w1, [x19, 60]
	str	w1, [x25, 20]
	ldr	w1, [x20]
	add	w1, w1, 1
	str	w1, [x20]
	ldp	x2, x3, [x25]
	stp	x2, x3, [x0]
	ldr	x1, [x25, 16]
	str	x1, [x0, 16]
	ldr	w1, [x25, 24]
	str	w1, [x20, 516]
.L347:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L284:
	mov	w28, 0
.L190:
	ldr	w0, [x19, 28]
	cbnz	w0, .L382
	ldr	w5, [x21, 16]
	mov	w4, 0
.L203:
	ldrsb	w0, [x20, 120]
	ldrsb	w1, [x20, 121]
	cmp	w0, 1
	beq	.L233
	cmp	w0, 2
	mov	w2, 0
	beq	.L237
.L241:
	cmp	w28, 1
	beq	.L297
.L357:
	sxtb	w1, w1
	b	.L234
	.p2align 3
.L378:
	ldr	w0, [x19, 60]
	cmp	w0, wzr
	cset	w0, eq
	strb	w0, [x20, 4]
	ldr	w0, [x19, 48]
	cbnz	w0, .L383
.L125:
	ldr	w0, [x19, 60]
	cmp	w0, wzr
	cset	w0, eq
	strb	w0, [x26, 1]
	ldr	w0, [x19, 48]
	cbnz	w0, .L384
.L127:
	ldrb	w0, [x19, 60]
	strb	w0, [x26, 2]
	b	.L385
	.p2align 3
.L369:
	ldrsb	w0, [x21, 108]
	cbz	w0, .L283
	mov	w23, 1
	str	w23, [x29, 104]
	b	.L189
.L371:
	ldr	x3, .LC8
	ldr	w1, [x19, 80]
	ldr	w4, [x3]
	cmp	w1, w4
	bcc	.L198
	ldr	w3, [x3, 4]
	cmp	w1, w3
	bcs	.L198
	mov	w28, w2
	b	.L190
	.p2align 3
.L376:
	ldr	w0, [x25, 20]
	cmp	w0, 2
	beq	.L258
	strb	w0, [x27, 2]
	b	.L258
	.p2align 3
.L366:
	ldr	w0, [x21, 448]
	cbnz	w0, .L141
	ldr	w22, [x19, 8]
	cmp	w22, 2
	beq	.L386
	cmp	w22, 1
	beq	.L387
.L144:
	ldr	w28, [x20, 16]
	cbz	w28, .L354
	ldr	w2, [x20, 24]
	add	x1, x19, 220
	ldr	w4, [x1, 4]
	cmp	w2, 1
	ldr	w2, [x19, 220]
	beq	.L388
	ldr	w6, [x1, 8]
.L149:
	ldp	w1, w3, [x19, 40]
	ldr	x8, .LC7
	mul	w1, w1, w3
	ldr	w7, [x8]
	asr	w3, w1, 9
	str	w7, [x8, 12]
	cmp	w3, wzr
	add	w1, w3, 31
	csel	w1, w1, w3, lt
	mov	w3, 16
	sub	w5, w3, w5
	asr	w1, w1, 5
	stp	w7, w7, [x8, 4]
	mul	w1, w1, w5
	mov	w5, 6
	cmp	w1, wzr
	add	w3, w1, 15
	csel	w1, w3, w1, lt
	asr	w3, w1, 4
	lsl	w1, w3, 2
	sub	w1, w1, w3
	add	w1, w1, w1, lsr 31
	asr	w1, w1, 1
	cmp	w1, 5
	csel	w1, w1, w5, gt
	sub	w5, w2, w4
	cmp	w4, w2
	sdiv	w5, w5, w1
	csel	w9, w4, w2, le
	csel	w10, w4, w2, ge
	cmp	w5, wzr
	ble	.L151
	cmp	w4, 254
	mov	w3, 50
	ble	.L152
	mov	w3, 26215
	movk	w3, 0x6666, lsl 16
	smull	x3, w4, w3
	asr	x3, x3, 33
	sub	w3, w3, w4, asr 31
.L152:
	add	w3, w3, w4
	cmp	w3, w2
	bge	.L153
	mov	w3, 1
	str	w3, [x8]
.L154:
	cmp	w10, 100
	ble	.L353
	sub	w8, w10, w9
	mov	w1, 0
	lsl	w11, w8, 2
	sub	w11, w11, w8
	lsl	w12, w11, 5
	add	w11, w11, w12
	add	w8, w11, w8
	sdiv	w8, w8, w10
	cmp	w8, 19
	ble	.L157
.L353:
	cmn	w5, #500
	mov	w1, -500
	csel	w1, w5, w1, ge
	mov	w5, 500
	cmp	w1, 500
	csel	w1, w1, w5, le
.L157:
	cmp	w7, w3
	add	x3, x29, 168
	csel	w1, w1, wzr, eq
	cmp	w6, w4
	csel	w5, w6, w4, ge
	str	xzr, [x29, 184]
	stp	xzr, xzr, [x3]
	cmp	w5, 254
	mov	w3, 50
	ble	.L160
	mov	w7, 26215
	movk	w7, 0x6666, lsl 16
	smull	x7, w5, w7
	asr	x7, x7, 33
	sub	w3, w7, w5, asr 31
.L160:
	add	w3, w5, w3
	cmp	w3, w2
	mov	w3, 0
	bge	.L161
	mov	w3, 1
	str	w3, [x29, 168]
.L161:
	cmp	w6, w2
	mov	w7, 50
	csel	w5, w6, w2, ge
	cmp	w5, 254
	ble	.L162
	mov	w8, 26215
	movk	w8, 0x6666, lsl 16
	smull	x8, w5, w8
	asr	x8, x8, 33
	sub	w7, w8, w5, asr 31
.L162:
	add	w7, w5, w7
	cmp	w7, w4
	bge	.L163
	mov	w5, 1
	str	w5, [x29, 172]
.L163:
	cmp	w10, 254
	mov	w5, 50
	ble	.L164
	mov	w7, 26215
	movk	w7, 0x6666, lsl 16
	smull	x7, w10, w7
	asr	x7, x7, 33
	sub	w5, w7, w10, asr 31
.L164:
	add	w5, w10, w5
	cmp	w5, w6
	bge	.L165
	mov	w5, 1
	str	w5, [x29, 176]
.L165:
	cmp	w6, w4
	mov	w5, 50
	csel	w7, w6, w4, le
	cmp	w7, 254
	ble	.L166
	mov	w5, 26215
	movk	w5, 0x6666, lsl 16
	smull	x5, w7, w5
	asr	x5, x5, 33
	sub	w5, w5, w7, asr 31
.L166:
	sub	w5, w7, w5
	cmp	w5, w2
	ble	.L167
	mov	w5, 1
	str	w5, [x29, 180]
.L167:
	cmp	w6, w2
	mov	w5, 50
	csel	w2, w6, w2, le
	cmp	w2, 254
	ble	.L168
	mov	w5, 26215
	movk	w5, 0x6666, lsl 16
	smull	x5, w2, w5
	asr	x5, x5, 33
	sub	w5, w5, w2, asr 31
.L168:
	sub	w2, w2, w5
	cmp	w2, w4
	ble	.L169
	mov	w2, 1
	str	w2, [x29, 184]
.L169:
	cmp	w9, 254
	mov	w2, 50
	ble	.L170
	mov	w2, 26215
	movk	w2, 0x6666, lsl 16
	smull	x2, w9, w2
	asr	x2, x2, 33
	sub	w2, w2, w9, asr 31
.L170:
	sub	w2, w9, w2
	cmp	w2, w6
	ble	.L171
	mov	w0, 1
	str	w0, [x29, 188]
.L171:
	cbz	w3, .L172
	ldr	w2, [x29, 184]
	cbnz	w2, .L172
	sxtw	x4, w1
	eor	x2, x4, x4, asr 63
	sub	x2, x2, x4, asr 63
	cmp	w2, 20
	ble	.L173
	cmp	w1, 50
	mov	w2, 50
	csel	w1, w1, w2, ge
.L173:
	cbnz	w0, .L174
	ldr	w0, [x29, 176]
	cbnz	w0, .L277
	ldr	w2, [x29, 184]
	cmp	w2, wzr
	ccmp	w3, wzr, 4, ne
	bne	.L174
	ldr	w0, [x29, 180]
	cbnz	w0, .L389
.L175:
	orr	w0, w2, w0
	cmp	w0, wzr
	csel	w1, w1, wzr, eq
.L174:
	mov	x0, 0
	mov	w2, 0
	b	.L177
.L390:
	add	x3, x29, 168
	ldr	w3, [x3, x0]
.L177:
	add	x0, x0, 4
	add	w2, w2, w3
	cmp	x0, 24
	bne	.L390
	cbnz	w2, .L178
	cmp	w1, 50
	mov	w0, 50
	csel	w1, w1, w0, le
	mov	w0, -50
	cmp	w1, w0
	csel	w1, w1, w0, ge
.L178:
	add	w0, w1, 49
	cmp	w0, 98
	ldr	w0, [x20, 128]
	csel	w1, w1, wzr, hi
	cmp	w6, 1
	csel	w1, w1, wzr, gt
	add	w1, w1, w0
	str	w1, [x20, 128]
	cmp	w1, 1000
	ble	.L181
	mov	w0, 1000
	str	w0, [x20, 128]
.L182:
	mov	w22, 0
	str	wzr, [x20, 132]
	b	.L187
.L303:
	mov	w2, 0
	mov	w4, 0
.L230:
	cmp	w2, 1
	ldr	w5, [x21, 16]
	beq	.L357
.L233:
	cmp	w28, 1
	beq	.L235
	ldr	w7, [x20, 20]
	mov	w0, 1
	strb	wzr, [x20, 120]
	strb	w0, [x20, 121]
.L245:
	stp	wzr, wzr, [x25, 4]
	tbz	x5, 0, .L246
	cmp	w7, wzr
	cset	w0, eq
	str	w0, [x25]
.L247:
	mov	w1, -1
	str	wzr, [x25, 12]
	mov	w0, w1
	stp	w23, w22, [x25, 16]
	b	.L260
.L297:
	mov	w28, 0
.L235:
	ldr	x6, .LC9
	mov	w0, 2
	ldr	w7, [x20, 20]
	strb	w28, [x20, 121]
	strb	w0, [x20, 120]
.L244:
	mov	w0, 1
	stp	w0, w0, [x25, 4]
	tbz	x5, 0, .L251
	cmp	w7, wzr
	cset	w0, eq
	str	w0, [x25]
.L252:
	mov	w0, 1
	str	w22, [x25, 20]
	stp	w0, w23, [x25, 12]
.L253:
	add	x0, x6, x0, sxtw 2
	ldr	w0, [x0, 144]
	b	.L257
.L382:
	ldr	w8, [x21, 448]
	ldr	w5, [x21, 16]
	cbnz	w8, .L201
	ldr	w0, [x21, 460]
	cmp	w0, 1
	bgt	.L391
	ldr	x2, [x21, 96]
	mov	w1, -1
	stp	wzr, w1, [x29, 156]
	str	w1, [x29, 164]
	ldr	w3, [x19, 208]
	ldr	w0, [x19, 216]
	ldr	w1, [x19, 212]
	cbz	x2, .L208
	add	w1, w3, w1, lsl 1
	add	x11, x20, 100
	add	w0, w1, w0, lsl 2
	add	x10, x20, 40
	lsl	w0, w0, 1
	str	w0, [x2]
	ldr	x0, [x21, 96]
	ldr	w13, [x21, 20]
	add	x0, x0, 4
	cmp	x0, x11
	add	w1, w13, 1
	csel	x0, x0, x10, ls
	str	w1, [x21, 20]
	str	x0, [x21, 96]
	cbz	x0, .L208
	cmp	w1, 4
	bgt	.L392
.L208:
	ldr	w0, [x21, 404]
	mov	w2, 0
	strb	wzr, [x21, 364]
	mov	w8, 0
	strb	wzr, [x21, 365]
	cmp	w0, 1
	str	wzr, [x21, 356]
	mov	w0, -1
	strb	w0, [x21, 109]
	mov	w0, 0
	ldrsb	w4, [x20, 382]
	ldr	w6, [x20, 376]
	beq	.L205
.L206:
	add	x3, x20, 396
	ldr	x9, [x20, 408]
	add	x1, x20, 420
	ldr	x10, [x3]
	str	x10, [x20, 384]
	ldr	w10, [x20, 404]
	str	w10, [x20, 392]
	str	x9, [x3]
	ldr	w9, [x20, 416]
	str	w9, [x20, 404]
	ldr	x3, [x1]
	str	x3, [x20, 408]
	ldr	w3, [x20, 428]
	str	w3, [x20, 416]
	ldr	x3, [x20, 432]
	str	x3, [x1]
	str	w8, [x20, 432]
	ldr	w3, [x20, 440]
	str	w3, [x20, 428]
	strb	w0, [x20, 440]
	strb	w2, [x20, 441]
	strb	w4, [x20, 442]
	str	w6, [x20, 436]
.L201:
	ldr	w0, [x21, 456]
	cbz	w0, .L225
	ldr	w0, [x21, 452]
	cbz	w0, .L226
	sub	w0, w5, #2
	sxtw	x1, w0
	lsl	x0, x1, 2
	sub	x0, x0, x1
.L356:
	lsl	x0, x0, 2
	add	x0, x0, 368
	add	x1, x21, x0
	ldr	w2, [x21, x0]
	ldrsb	w0, [x20, 120]
	ldrsb	w4, [x1, 10]
	cmp	w0, 1
	ldrsb	w1, [x20, 121]
	beq	.L230
	cmp	w0, 2
	beq	.L231
	cmp	w2, 1
	bne	.L240
	ldr	x6, .LC9
	strb	w2, [x20, 120]
	add	x0, x6, 104
	strb	wzr, [x20, 121]
	ldr	x1, [x6, 136]
	str	x1, [x29, 200]
	add	x1, x29, 168
	ldp	x2, x3, [x0]
	ldr	w8, [x19, 8]
	stp	x2, x3, [x1]
	add	x2, x29, 184
	ldp	x0, x1, [x0, 16]
	stp	x0, x1, [x2]
.L243:
	mov	w0, 1
	stp	w0, wzr, [x25, 4]
	cbnz	w8, .L249
	add	w4, w4, w0
.L358:
	mov	w0, 26215
	movk	w0, 0x6666, lsl 16
	smull	x0, w4, w0
	asr	x0, x0, 34
	sub	w0, w0, w4, asr 31
	lsl	w1, w0, 3
	add	w0, w1, w0, lsl 1
	add	x1, x29, 208
	sub	w0, w4, w0
	add	x0, x1, x0, sxtw 2
	ldr	w0, [x0, -40]
	str	w0, [x25]
.L250:
	mov	w1, 0
	mov	w0, 0
	str	wzr, [x25, 12]
	stp	w23, w22, [x25, 16]
	b	.L260
.L367:
	mov	w22, 0
	ldr	w28, [x20, 16]
	b	.L187
.L370:
	cmp	w2, 1
	ldr	w1, [x19, 72]
	bne	.L360
	cbnz	w1, .L360
	ldr	x3, .LC8
	ldr	w1, [x19, 80]
	ldr	w4, [x3]
	cmp	w1, w4
	bcc	.L199
	ldr	w3, [x3, 4]
	cmp	w1, w3
	bcs	.L199
	mov	w28, w2
	b	.L190
	.p2align 3
.L251:
	str	w7, [x25]
	b	.L252
.L249:
	cmp	w8, 1
	bne	.L250
	add	w4, w4, 2
	b	.L358
.L246:
	str	w7, [x25]
	b	.L247
.L254:
	str	w7, [x25]
	b	.L255
.L372:
	ldr	w2, [x20, 260]
	mov	w1, w28
	ldr	w3, [x19, 8]
	add	x28, x20, 268
	str	w2, [x20, 256]
	ldr	w2, [x20, 264]
	str	w2, [x20, 260]
	mov	x2, x28
	bl	Calc22PLD_4FLD
	ldr	w28, [x20, 360]
	str	w28, [x20, 264]
	b	.L190
.L365:
	add	x20, x20, 492
	ldp	x0, x1, [x20]
	stp	x0, x1, [x25]
	ldr	x0, [x20, 16]
	str	x0, [x25, 16]
	ldr	w0, [x20, 24]
	str	w0, [x25, 24]
	b	.L347
.L368:
	mov	w22, 1
	ldr	w28, [x20, 16]
	b	.L187
.L225:
	ldr	w0, [x21, 452]
	cbnz	w0, .L226
	sxtw	x5, w5
	lsl	x0, x5, 2
	sub	x0, x0, x5
	b	.L356
.L391:
	add	x0, x20, 384
	str	xzr, [x20, 432]
	stp	xzr, xzr, [x0]
	stp	xzr, xzr, [x0, 16]
	stp	xzr, xzr, [x0, 32]
	add	x0, x20, 444
	str	wzr, [x20, 440]
	str	wzr, [x20, 460]
	ldrsb	w4, [x20, 394]
	stp	xzr, xzr, [x0]
	b	.L203
.L375:
	ldr	x6, .LC9
	b	.L253
.L288:
	mov	w28, w1
	b	.L190
.L226:
	sub	w0, w5, #1
	sxtw	x1, w0
	lsl	x0, x1, 2
	sub	x0, x0, x1
	b	.L356
.L373:
	ldr	w5, [x21, 16]
	b	.L201
.L298:
	mov	w0, 22
	b	.L257
.L374:
	mov	w4, 0
.L240:
	ldr	w5, [x21, 16]
	b	.L241
.L172:
	ldr	w2, [x29, 172]
	cbz	w2, .L173
	ldr	w2, [x29, 180]
	cbnz	w2, .L173
	sxtw	x4, w1
	eor	x2, x4, x4, asr 63
	sub	x2, x2, x4, asr 63
	cmp	w2, 20
	ble	.L173
	cmn	w1, #50
	mov	w2, -50
	csel	w1, w1, w2, le
	b	.L173
.L153:
	mov	w3, 0
	str	wzr, [x8]
	b	.L154
.L181:
	cmn	w1, #1000
	bge	.L183
	mov	w0, -1000
	str	w0, [x20, 128]
.L184:
	mov	w22, 1
	str	w22, [x20, 132]
	b	.L187
.L392:
	ldr	x6, [x21, 88]
	add	x9, x29, 208
	mov	w3, 0
	mov	w12, 0
	mov	x0, x6
	ldr	w7, [x6]
	str	w7, [x9, -40]!
	mov	w2, w7
	mov	w1, w7
	add	x4, x9, 4
.L209:
	cmp	w2, w1
	add	x0, x0, 4
	csel	w2, w2, w1, ge
	add	w3, w3, 1
	cmp	x11, x0
	csel	x0, x0, x10, cs
	cmp	w3, 5
	beq	.L393
.L212:
	ldr	w1, [x0]
	add	x4, x4, 4
	str	w1, [x4, -4]
	cmp	w1, w7
	bge	.L209
	cmp	w2, w1
	add	x0, x0, 4
	mov	w12, w3
	csel	w2, w2, w1, ge
	add	w3, w3, 1
	cmp	x11, x0
	csel	x0, x0, x10, cs
	cmp	w3, 5
	mov	w7, w1
	bne	.L212
.L393:
	mov	w3, w2
	mov	x1, 0
.L214:
	ldr	w0, [x9, x1, lsl 2]
	cmp	w1, w12
	add	x1, x1, 1
	ccmp	w0, w3, 0, ne
	csel	w0, w0, w3, lt
	cmp	x1, 5
	mov	w3, w0
	bne	.L214
	stp	w5, w8, [x29, 96]
	add	x5, x29, 168
	str	w2, [x29, 196]
	stp	w7, w0, [x29, 188]
	add	x0, x6, 4
	add	x1, x20, 40
	cmp	x11, x0
	str	w13, [x21, 20]
	csel	x0, x0, x1, cs
	str	x0, [x21, 88]
	ldp	x6, x7, [x5]
	add	x5, x29, 184
	add	x2, x29, 160
	stp	x6, x7, [x29, 112]
	add	x1, x29, 156
	add	x4, x29, 112
	ldp	x6, x7, [x5]
	mov	x3, x21
	add	x0, x29, 164
	stp	x6, x7, [x29, 128]
	bl	SearchPhase
	ldp	w0, w1, [x19, 124]
	ldr	w2, [x29, 104]
	cmp	w0, w1
	ldr	w5, [x29, 96]
	csel	w0, w0, w1, le
	ldr	w1, [x29, 156]
	cmp	w0, 64
	cset	w6, gt
	cmp	w1, 4
	cset	w1, gt
	str	w6, [x21, 360]
	orr	w1, w6, w1
	orr	w24, w2, w1
	cbnz	w24, .L394
	ldr	w1, [x29, 160]
	cmp	w1, 4
	bhi	.L395
	add	x0, x21, x1, sxtw 2
	and	w2, w1, 1
	ldr	w0, [x0, 428]
	cmp	w0, 3
	cset	w3, gt
	sxtb	w0, w0
	str	w3, [x21, 356]
	strb	w0, [x21, 364]
	ldr	w4, [x20, 16]
	cmp	w4, w2
	beq	.L396
	add	w4, w1, 5
	sxtb	w4, w4
.L223:
	cmp	w1, wzr
	strb	w4, [x21, 366]
	cset	w2, eq
.L221:
	strb	w2, [x21, 365]
	tbnz	w1, #31, .L218
	sxtb	w1, w1
	mov	w8, w3
.L224:
	strb	w1, [x21, 109]
	ldr	w1, [x21, 404]
	cmp	w1, 1
	bne	.L206
	cbnz	w3, .L206
.L205:
	mov	w1, 2
	str	wzr, [x21, 380]
	strb	w1, [x21, 424]
	strb	w1, [x21, 412]
	strb	w1, [x21, 400]
	strb	w1, [x21, 388]
	str	wzr, [x21, 392]
	str	wzr, [x21, 404]
	str	wzr, [x21, 416]
	b	.L206
.L386:
	str	w22, [x21, 116]
	ldr	w28, [x20, 16]
	b	.L187
.L394:
	cmp	w0, 64
	mov	w0, -1
	str	w0, [x29, 160]
	ble	.L217
	ldr	w8, [x29, 100]
	mov	w0, 0
	strb	wzr, [x21, 364]
.L219:
	add	x1, x20, 256
	str	w8, [x21, 428]
	mov	w3, 0
	mov	w4, 0
	mov	w2, 0
	stp	w8, w8, [x1, 192]
	stp	w8, w8, [x1, 200]
	strb	wzr, [x21, 366]
	strb	wzr, [x21, 365]
	str	wzr, [x21, 356]
.L218:
	ldrsb	w1, [x29, 164]
	mov	w8, w3
	b	.L224
.L387:
	ldr	x1, [x21, 8]
	mov	x2, 4294967297
	cmp	x1, x2
	ccmp	x1, xzr, 4, ne
	bne	.L144
	ldr	w22, [x21, 116]
	ldr	w28, [x20, 16]
	b	.L187
.L183:
	cmp	w1, 200
	bgt	.L182
	cmn	w1, #200
	blt	.L184
.L354:
	ldr	w22, [x21, 116]
	b	.L187
.L151:
	bge	.L153
	cmp	w2, 254
	mov	w3, 50
	ble	.L155
	mov	w3, 26215
	movk	w3, 0x6666, lsl 16
	smull	x3, w2, w3
	asr	x3, x3, 33
	sub	w3, w3, w2, asr 31
.L155:
	add	w3, w3, w2
	cmp	w3, w4
	bge	.L153
	mov	w3, 2
	str	w3, [x8]
	b	.L154
.L388:
	mov	w3, w2
	ldr	w6, [x1, 8]
	mov	w2, w4
	mov	w4, w3
	b	.L149
.L277:
	mov	w1, 0
	b	.L174
.L217:
	ldrb	w0, [x21, 364]
	mov	w1, 4
	sub	w0, w0, #1
	sxtb	w2, w0
	cmp	w2, 4
	csel	w0, w0, w1, le
	sxtb	w1, w0
	cmp	w1, wzr
	csel	w0, w0, wzr, ge
	strb	w0, [x21, 364]
	sxtb	w0, w0
	mov	w8, w0
	b	.L219
.L389:
	ldr	w4, [x29, 172]
	cbnz	w4, .L174
	b	.L175
.L395:
	cmp	w1, wzr
	ldrsb	w0, [x21, 364]
	ldrsb	w4, [x20, 382]
	cset	w2, eq
	ldr	w3, [x20, 372]
	b	.L221
.L396:
	sxtb	w4, w1
	b	.L223
	.size	PQ_MNG_ALG_FmdSet, .-PQ_MNG_ALG_FmdSet
	.align	3
.LC7:
	.xword	.LANCHOR1
	.align	3
.LC8:
	.xword	.LANCHOR2
	.align	3
.LC9:
	.xword	.LANCHOR0
	.global	g_u32FrmRateThr_H
	.global	g_u32FrmRateThr_L
	.global	g_u32IPDetect
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
.LC0:
	.word	1
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	1
	.zero	4
.LC1:
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.type	g_u32PDTypeNum, %object
	.size	g_u32PDTypeNum, 8
g_u32PDTypeNum:
	.word	32
	.word	22
	.data
	.align	2
.LANCHOR2 = . + 0
	.type	g_u32FrmRateThr_L, %object
	.size	g_u32FrmRateThr_L, 4
g_u32FrmRateThr_L:
	.word	23800
	.type	g_u32FrmRateThr_H, %object
	.size	g_u32FrmRateThr_H, 4
g_u32FrmRateThr_H:
	.word	24200
	.bss
	.align	3
.LANCHOR1 = . + 0
	.type	stastlasi1432big.1819, %object
	.size	stastlasi1432big.1819, 16
stastlasi1432big.1819:
	.zero	16
	.type	g_u32IPDetect, %object
	.size	g_u32IPDetect, 4
g_u32IPDetect:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
