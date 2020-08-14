#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"resample.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	cvtShortShort, %function
cvtShortShort:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w23, w3
	mov	w22, w4
	mov	x21, x1
	mov	w19, w2
	bl	_mcount
	cmp	w23, wzr
	ccmp	w22, 2, 0, eq
	bne	.L2
	add	x2, x21, x19, sxtw 1
	cmp	x21, x2
	beq	.L8
	mov	x4, x2
	mov	x3, x21
	mov	x0, x20
	.p2align 2
.L4:
	add	x0, x0, 4
	ldrsh	w5, [x3], 2
	cmp	x2, x3
	strh	w5, [x0, -4]
	ldrsh	w5, [x4], 2
	strh	w5, [x0, -2]
	bne	.L4
	add	x0, x21, 2
	ldr	x23, [sp, 48]
	sub	x0, x2, x0
	lsr	x0, x0, 1
	add	x0, x0, 1
	ubfiz	x0, x0, 1, 62
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2:
	cmp	w22, 2
	lsl	w0, w19, 1
	csel	w19, w0, w19, eq
	add	x2, x21, x19, sxtw 1
	cmp	x21, x2
	beq	.L8
	mov	x3, x21
	mov	x0, x20
	.p2align 2
.L6:
	ldrsh	w4, [x3], 2
	strh	w4, [x0], 2
	cmp	x2, x3
	bne	.L6
	add	x0, x21, 2
	sub	x0, x2, x0
	and	x0, x0, -2
	add	x0, x0, 2
	lsr	x0, x0, 1
.L3:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L8:
	mov	w0, 0
	b	.L3
	.size	cvtShortShort, .-cvtShortShort
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	cvt2int, %function
cvt2int:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	w24, w4
	mov	w22, w5
	mov	x20, x1
	mov	w19, w2
	mov	w23, w3
	bl	_mcount
	cmp	w24, wzr
	ccmp	w22, 2, 0, eq
	bne	.L13
	cmp	w23, 16
	beq	.L30
	add	x2, x20, x19, sxtw 2
	cmp	x20, x2
	beq	.L25
	mov	x4, x2
	mov	x3, x20
	mov	x0, x21
	.p2align 2
.L17:
	add	x0, x0, 8
	ldr	w5, [x3], 4
	cmp	x2, x3
	str	w5, [x0, -8]
	ldr	w5, [x4], 4
	str	w5, [x0, -4]
	bne	.L17
	add	x0, x20, 4
	sub	x0, x2, x0
	lsr	x0, x0, 2
	add	x0, x0, 1
	ubfiz	x0, x0, 1, 61
.L15:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L13:
	cmp	w22, 2
	lsl	w0, w19, 1
	csel	w19, w0, w19, eq
	cmp	w23, 16
	beq	.L31
	add	x2, x20, x19, sxtw 2
	cmp	x20, x2
	beq	.L25
	mov	x3, x20
	mov	x0, x21
	.p2align 2
.L21:
	ldr	w4, [x3], 4
	str	w4, [x0], 4
	cmp	x2, x3
	bne	.L21
	add	x0, x20, 4
	sub	x0, x2, x0
	and	x0, x0, -4
	add	x0, x0, 4
	lsr	x0, x0, 2
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L31:
	add	x2, x20, x19, sxtw 1
	cmp	x20, x2
	beq	.L25
	mov	x3, x20
	mov	x0, x21
	.p2align 2
.L20:
	ldrsh	w1, [x3], 2
	lsl	w1, w1, 16
	cmp	x2, x3
	str	w1, [x0], 4
	bne	.L20
	add	x0, x20, 2
	sub	x0, x2, x0
	lsr	x0, x0, 1
	add	w0, w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L30:
	add	x2, x20, x19, sxtw 1
	cmp	x20, x2
	beq	.L25
	mov	x4, x2
	mov	x3, x20
	mov	x0, x21
	.p2align 2
.L16:
	add	x0, x0, 8
	ldrsh	w1, [x3], 2
	lsl	w1, w1, 16
	cmp	x2, x3
	str	w1, [x0, -8]
	ldrsh	w1, [x4], 2
	lsl	w1, w1, 16
	str	w1, [x0, -4]
	bne	.L16
	add	x0, x20, 2
	sub	x0, x2, x0
	lsr	x0, x0, 1
	add	x0, x0, 1
	ubfiz	x0, x0, 1, 61
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L25:
	mov	w0, 0
	b	.L15
	.size	cvt2int, .-cvt2int
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ResamplerMonoProcess24, %function
ResamplerMonoProcess24:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	mov	w22, w2
	mov	x23, x3
	bl	_mcount
	mov	w5, 1
	ldr	w3, [x21, 40]
	ldr	x0, [x21, 72]
	mov	w2, w22
	mov	x1, x20
	mov	w4, w5
	ldr	w19, [x21, 20]
	ldr	w20, [x21, 28]
	ldr	x22, [x21, 104]
	bl	cvt2int
	sxtw	x3, w19
	ldrsw	x4, [x21, 32]
	ldp	x2, x14, [x21, 72]
	neg	x3, x3, lsl 2
	sbfiz	x1, x0, 2, 32
	add	x3, x3, 4
	ldr	x15, [x21, 88]
	add	x2, x2, x3
	add	x0, x2, x1
	add	x16, x2, x4, lsl 2
	cmp	x0, x16
	bls	.L50
	add	x17, x23, 4
	.p2align 2
.L48:
	cmp	w19, wzr
	sub	x11, x16, #4
	ble	.L34
	mov	x13, x16
	mov	x10, 0
	mov	w12, 0
	mov	w4, 0
	mov	w6, 0
	.p2align 2
.L35:
	ldr	w7, [x11], -8
	add	w12, w12, 2
	ldr	w5, [x13], 8
	cmp	w19, w12
	ldr	w2, [x14, x10]
	ldr	w8, [x15, x10]
	add	x10, x10, 4
	lsl	w30, w2, 16
	ldr	w9, [x11, 4]
	lsl	w24, w8, 16
	ldr	w18, [x13, -4]
	and	w3, w2, -65536
	and	w8, w8, -65536
	smull	x7, w7, w24
	smull	x5, w5, w30
	smull	x8, w9, w8
	smull	x3, w3, w18
	asr	x5, x5, 32
	asr	x7, x7, 32
	add	w4, w4, w5
	add	w6, w6, w7
	asr	x5, x8, 32
	asr	x2, x3, 32
	add	w6, w6, w5
	add	w4, w4, w2
	bgt	.L35
	ldr	w2, [x21, 40]
	add	w6, w4, w6
	cmp	w2, 16
	beq	.L56
	cmp	w2, 18
	beq	.L57
.L40:
	cmp	w2, 20
	beq	.L58
	add	w6, w6, 64
	mov	x4, x17
	asr	w3, w6, 31
	ubfx	x2, x6, 7, 24
	eor	w5, w3, 8388607
	cmp	w3, w6, asr 30
	csel	w2, w2, w5, eq
.L55:
	str	w2, [x17, -4]
.L39:
	ldr	w20, [x22, w20, sxtw 2]
	add	x17, x17, 4
	ldr	x15, [x21, 96]
	ubfx	x3, x20, 1, 7
	ubfx	x14, x20, 8, 12
	lsr	w2, w20, 20
	asr	w20, w20, 20
	add	x16, x16, x3, lsl 2
	mul	w14, w14, w19
	mul	w2, w2, w19
	cmp	x0, x16
	add	x14, x15, x14, sxtw 1
	add	x15, x15, x2, sxtw 1
	bhi	.L48
	sub	x4, x4, x23
	lsr	x19, x4, 2
.L33:
	sub	x0, x16, x0
	ldr	x3, .LC4
	asr	x0, x0, 2
	ldrsw	x2, [x21, 24]
	stp	w20, w0, [x21, 28]
	ldr	x0, [x21, 64]
	lsl	x2, x2, 2
	ldr	x3, [x3, 56]
	stp	x14, x15, [x21, 80]
	add	x1, x0, x1
	blr	x3
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L56:
	add	w6, w6, 16384
	mov	x4, x17
	asr	w2, w6, 31
	asr	w3, w6, 15
	cmp	w2, w6, asr 30
	beq	.L59
	eor	w2, w2, 32767
	sxth	w2, w2
	b	.L55
	.p2align 3
.L57:
	add	w6, w6, 4096
	asr	w3, w6, 31
	asr	w2, w6, 13
	cmp	w3, w6, asr 30
	beq	.L41
	eor	w2, w3, 131071
.L42:
	mov	x4, x17
	str	w2, [x17, -4]
	b	.L39
	.p2align 3
.L58:
	add	w6, w6, 1024
	mov	x4, x17
	asr	w3, w6, 31
	ubfx	x2, x6, 11, 20
	eor	w5, w3, 524287
	cmp	w3, w6, asr 30
	csel	w2, w2, w5, eq
	str	w2, [x17, -4]
	b	.L39
.L34:
	ldr	w2, [x21, 40]
	cmp	w2, 16
	beq	.L49
	cmp	w2, 18
	beq	.L51
	mov	w6, 0
	b	.L40
.L51:
	mov	w2, 0
.L41:
	and	w2, w2, 262143
	b	.L42
.L59:
	sxth	w2, w3
	b	.L55
.L49:
	mov	x4, x17
	mov	w2, 0
	b	.L55
.L50:
	mov	w19, 0
	b	.L33
	.size	ResamplerMonoProcess24, .-ResamplerMonoProcess24
	.align	3
.LC4:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ResamplerStereoProcess24, %function
ResamplerStereoProcess24:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	mov	w22, w2
	mov	x23, x3
	mov	w24, w4
	bl	_mcount
	ldr	w19, [x21, 20]
	ldr	w3, [x21, 40]
	mov	w2, w22
	ldr	x0, [x21, 72]
	mov	x1, x20
	mov	w5, 2
	mov	w4, w24
	ldr	w20, [x21, 28]
	ldr	x22, [x21, 104]
	bl	cvt2int
	ldp	x15, x13, [x21, 72]
	sub	w2, w19, #1
	ldrsw	x3, [x21, 32]
	sbfiz	x1, x0, 2, 32
	sub	x2, x15, x2, sxtw 3
	add	x0, x2, x1
	add	x15, x2, x3, lsl 2
	cmp	x0, x15
	ldp	x14, x17, [x21, 88]
	bls	.L69
	mov	x16, x23
	.p2align 2
.L68:
	cmp	w19, wzr
	sub	x9, x15, #4
	ble	.L62
	mov	x10, x15
	mov	x11, 0
	mov	w12, 0
	mov	w26, 0
	mov	w2, 0
	.p2align 2
.L63:
	ldr	w30, [x14, x11]
	add	w12, w12, 2
	ldr	w24, [x13, x11]
	cmp	w19, w12
	ldr	w18, [x9]
	lsl	w7, w30, 16
	ldp	w4, w28, [x10]
	ldp	w3, w27, [x9, -8]
	lsl	w8, w24, 16
	ldr	w5, [x10, 8]
	sub	x9, x9, #16
	add	x10, x10, 16
	and	w25, w30, -65536
	and	w24, w24, -65536
	smull	x6, w18, w7
	smull	x4, w4, w8
	smull	x7, w7, w27
	ldr	w30, [x9, 4]
	asr	x18, x6, 32
	smull	x27, w3, w25
	ldr	w3, [x10, -4]
	asr	x6, x4, 32
	smull	x28, w8, w28
	smull	x4, w5, w24
	add	w2, w2, w18
	asr	x7, x7, 32
	mov	x5, x4
	asr	x8, x28, 32
	add	w4, w26, w6
	add	w2, w2, w8
	smull	x18, w24, w3
	smull	x25, w25, w30
	add	w6, w4, w7
	asr	x5, x5, 32
	asr	x3, x27, 32
	asr	x30, x25, 32
	add	w3, w2, w3
	add	w4, w6, w5
	asr	x2, x18, 32
	add	x11, x11, 4
	add	w26, w4, w30
	add	w2, w3, w2
	bgt	.L63
	add	w4, w26, 64
	add	w2, w2, 64
	asr	w3, w4, 31
	asr	w5, w4, 7
	cmp	w3, w4, asr 30
	beq	.L73
	eor	w3, w3, 8388607
	asr	w5, w2, 7
	lsl	w4, w3, 8
	asr	w6, w2, 30
	asr	w3, w2, 31
.L65:
	cmp	w3, w6
	str	w4, [x16]
	beq	.L66
	eor	w3, w3, 8388607
.L67:
	add	x16, x16, 8
	lsl	w3, w3, 8
	str	w3, [x16, -4]
	ldr	w2, [x22, w20, sxtw 2]
	ubfx	x13, x2, 8, 12
	lsr	w14, w2, 20
	add	x15, x15, x2, uxtb 2
	asr	w20, w2, 20
	mul	w13, w13, w19
	mul	w14, w14, w19
	cmp	x0, x15
	add	x13, x17, x13, sxtw 1
	add	x14, x17, x14, sxtw 1
	bhi	.L68
	sub	x2, x16, x23
	asr	x0, x2, 2
	add	x0, x0, x0, lsr 63
	lsr	x19, x0, 1
.L61:
	ldr	x3, .LC5
	stp	w20, wzr, [x21, 28]
	ldrsw	x2, [x21, 24]
	ldr	x0, [x21, 64]
	ldr	x3, [x3, 56]
	lsl	x2, x2, 2
	stp	x13, x14, [x21, 80]
	add	x1, x0, x1
	blr	x3
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L62:
	mov	w5, 0
	str	wzr, [x16]
.L66:
	and	w3, w5, 16777215
	b	.L67
	.p2align 3
.L73:
	lsl	w4, w5, 8
	asr	w3, w2, 31
	asr	w5, w2, 7
	asr	w6, w2, 30
	b	.L65
.L69:
	mov	w19, 0
	b	.L61
	.size	ResamplerStereoProcess24, .-ResamplerStereoProcess24
	.align	3
.LC5:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResampleConfig
	.type	ResampleConfig, %function
ResampleConfig:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	mov	w21, w2
	bl	_mcount
	cmp	w19, w21
	beq	.L87
	mov	w3, w21
	mov	w0, w19
	b	.L78
	.p2align 3
.L98:
	sub	w0, w0, w3
	cmp	w0, w3
	beq	.L75
.L78:
	cmp	w0, w3
	bgt	.L98
	sub	w3, w3, w0
	cmp	w0, w3
	bne	.L78
.L75:
	sdiv	w9, w21, w0
	cmp	w9, 1280
	sdiv	w5, w19, w0
	bgt	.L89
	add	w0, w9, w5
	ldr	w10, [x20, 16]
	sub	w0, w0, #1
	sdiv	w0, w0, w9
	mul	w0, w0, w10
	cmp	w0, 255
	bgt	.L89
	cmp	w19, wzr
	add	w6, w19, 3
	csel	w6, w6, w19, lt
	add	w2, w21, 3
	cmp	w21, wzr
	add	w1, w19, w19, lsr 31
	add	w8, w21, w21, lsr 31
	csel	w2, w2, w21, lt
	asr	w1, w1, 1
	asr	w8, w8, 1
	asr	w6, w6, 2
	asr	w2, w2, 2
	ldr	x3, .LC6
	mov	w4, 0
	b	.L84
	.p2align 3
.L80:
	cmp	w0, w1
	beq	.L99
	cmp	w0, w6
	beq	.L100
.L83:
	add	w4, w4, 1
	add	x3, x3, 32
	cmp	w4, 40
	beq	.L89
.L84:
	ldr	w0, [x3]
	cmp	w19, w0
	bne	.L80
	ldr	w7, [x3, 4]
	cmp	w21, w7
	bne	.L80
.L81:
	cmp	w4, 40
	mov	w0, -1
	beq	.L79
	ldr	x2, [x3, 24]
	ldr	w4, [x3, 12]
	str	x2, [x20, 96]
	lsl	w1, w4, 1
	ldr	x0, [x3, 16]
	str	x0, [x20, 104]
	ldr	w0, [x20, 40]
	str	wzr, [x20, 36]
	cmp	w0, 16
	mul	w1, w1, w10
	beq	.L97
	sub	w3, w0, #18
	and	w3, w3, -3
	cmp	w3, wzr
	ccmp	w0, 24, 4, ne
	beq	.L97
.L86:
	sub	w0, w9, #1
	stp	w9, w5, [x20]
	mul	w0, w0, w4
	stp	w19, w21, [x20, 8]
	stp	w4, w1, [x20, 20]
	stp	wzr, wzr, [x20, 28]
	add	x0, x2, x0, sxtw 1
	stp	x0, x2, [x20, 80]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L99:
	ldr	w7, [x3, 4]
	cmp	w7, w8
	beq	.L81
	cmp	w0, w6
	bne	.L83
.L100:
	ldr	w0, [x3, 4]
	cmp	w0, w2
	beq	.L81
	add	w4, w4, 1
	add	x3, x3, 32
	cmp	w4, 40
	bne	.L84
.L89:
	mov	w0, -1
.L79:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L97:
	ldr	x0, [x20, 64]
	add	x0, x0, x1, sxtw 2
	str	x0, [x20, 72]
	b	.L86
.L87:
	mov	w0, w19
	b	.L75
	.size	ResampleConfig, .-ResampleConfig
	.align	3
.LC6:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResamplerCreate
	.type	ResamplerCreate, %function
ResamplerCreate:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w20, w0
	mov	x0, x30
	mov	w24, w1
	mov	w22, w2
	mov	w26, w3
	mov	w23, w4
	bl	_mcount
	mov	w0, 32000
	cmp	w20, w0
	beq	.L103
	ble	.L158
	mov	w0, 22664
	movk	w0, 0x1, lsl 16
	cmp	w20, w0
	beq	.L103
	ble	.L159
	mov	w0, 45328
	movk	w0, 0x2, lsl 16
	cmp	w20, w0
	beq	.L103
	mov	w0, 60928
	movk	w0, 0x2, lsl 16
	cmp	w20, w0
	beq	.L103
	mov	w0, 30464
	movk	w0, 0x1, lsl 16
	cmp	w20, w0
	beq	.L103
	.p2align 2
.L102:
	add	x0, x29, 120
	mov	w4, w22
	mov	w3, w26
	mov	w2, w24
	mov	w1, w20
	bl	LSRC_Create
	ldr	x0, [x29, 120]
	cbz	x0, .L128
	mov	w1, 128
	mov	w0, 26
	bl	HI_ADSP_MALLOC
	cbz	x0, .L160
	ldr	x2, [x29, 120]
	mov	w1, 1
	str	x2, [x0, 120]
	str	w1, [x0, 116]
.L128:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L168:
	mov	w0, 8000
	cmp	w20, w0
	beq	.L103
	mov	w0, 11025
	cmp	w20, w0
	bne	.L102
	.p2align 2
.L103:
	mov	w0, 32000
	cmp	w24, w0
	beq	.L107
	ble	.L161
	mov	w0, 22664
	movk	w0, 0x1, lsl 16
	cmp	w24, w0
	beq	.L107
	ble	.L162
	mov	w0, 45328
	movk	w0, 0x2, lsl 16
	cmp	w24, w0
	beq	.L107
	mov	w0, 60928
	movk	w0, 0x2, lsl 16
	cmp	w24, w0
	beq	.L107
	mov	w0, 30464
	movk	w0, 0x1, lsl 16
	cmp	w24, w0
	bne	.L102
	.p2align 2
.L107:
	cmp	w20, w24
	bge	.L102
	sub	w0, w22, #1
	cmp	w0, 1
	bhi	.L102
	and	w0, w26, -3
	cmp	w0, 16
	beq	.L132
	sub	w0, w26, #20
	tst	w0, -5
	bne	.L102
.L132:
	mov	w0, w24
	mov	w1, w20
	b	.L115
	.p2align 3
.L164:
	sub	w1, w1, w0
	cmp	w0, w1
	beq	.L163
.L115:
	cmp	w1, w0
	bgt	.L164
	sub	w0, w0, w1
	cmp	w0, w1
	bne	.L115
.L163:
	sdiv	w21, w24, w0
	cmp	w21, 1280
	sdiv	w0, w20, w0
	str	w0, [x29, 108]
	bgt	.L102
	add	w0, w21, w0
	sub	w0, w0, #1
	sdiv	w0, w0, w21
	mul	w0, w0, w22
	cmp	w0, 255
	bgt	.L102
	cmp	w20, wzr
	add	w4, w20, 3
	csel	w4, w4, w20, lt
	add	w6, w24, 3
	cmp	w24, wzr
	add	w2, w20, w20, lsr 31
	csel	w6, w6, w24, lt
	add	w7, w24, w24, lsr 31
	asr	w2, w2, 1
	asr	w4, w4, 2
	asr	w6, w6, 2
	asr	w7, w7, 1
	ldr	x19, .LC7
	mov	w1, 0
	b	.L120
	.p2align 3
.L116:
	cmp	w0, w2
	beq	.L165
.L118:
	cmp	w0, w4
	beq	.L166
.L119:
	add	w1, w1, 1
	add	x19, x19, 32
	cmp	w1, 40
	beq	.L102
.L120:
	ldr	w0, [x19]
	cmp	w20, w0
	bne	.L116
	ldr	w5, [x19, 4]
	cmp	w24, w5
	bne	.L116
	ldr	w5, [x19, 8]
	cmp	w5, 2
	bne	.L116
.L117:
	cmp	w1, 40
	beq	.L102
	ldr	w28, [x19, 12]
	mov	w1, 128
	mov	w0, 26
	lsl	w27, w28, 1
	bl	HI_ADSP_MALLOC
	mov	x25, x0
	mul	w27, w27, w22
	cbz	x0, .L102
	ldr	x0, [x19, 24]
	cmp	w26, 16
	str	x0, [x25, 96]
	ldr	x1, [x19, 16]
	str	x1, [x25, 104]
	stp	wzr, w26, [x25, 36]
	beq	.L167
	sub	w1, w26, #18
	and	w1, w1, -3
	cmp	w1, wzr
	ccmp	w26, 24, 4, ne
	cset	w1, eq
	cbnz	w1, .L157
.L125:
	sub	w1, w21, #1
	ldr	w2, [x29, 108]
	stp	w21, w2, [x25]
	mul	w1, w1, w28
	stp	w20, w24, [x25, 8]
	stp	w22, w28, [x25, 16]
	stp	w27, wzr, [x25, 24]
	str	wzr, [x25, 32]
	add	x1, x0, x1, sxtw 1
	stp	w23, wzr, [x25, 112]
	stp	x1, x0, [x25, 80]
	str	xzr, [x25, 120]
	mov	x0, x25
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L158:
	mov	w0, 12000
	cmp	w20, w0
	beq	.L103
	ble	.L168
	mov	w0, 22050
	cmp	w20, w0
	beq	.L103
	mov	w0, 24000
	cmp	w20, w0
	beq	.L103
	mov	w0, 16000
	cmp	w20, w0
	bne	.L102
	b	.L103
	.p2align 3
.L161:
	mov	w0, 12000
	cmp	w24, w0
	beq	.L107
	ble	.L169
	mov	w0, 22050
	cmp	w24, w0
	beq	.L107
	mov	w0, 24000
	cmp	w24, w0
	beq	.L107
	mov	w0, 16000
	cmp	w24, w0
	bne	.L102
	b	.L107
	.p2align 3
.L165:
	ldr	w5, [x19, 4]
	cmp	w5, w7
	bne	.L118
	ldr	w5, [x19, 8]
	cmp	w5, 2
	beq	.L117
	cmp	w0, w4
	bne	.L119
	.p2align 2
.L166:
	ldr	w0, [x19, 4]
	cmp	w0, w6
	bne	.L119
	ldr	w0, [x19, 8]
	cmp	w0, 2
	bne	.L119
	b	.L117
	.p2align 3
.L159:
	mov	w0, 44100
	cmp	w20, w0
	beq	.L103
	mov	w0, 48000
	cmp	w20, w0
	beq	.L103
	b	.L102
	.p2align 3
.L169:
	mov	w0, 8000
	cmp	w24, w0
	beq	.L107
	mov	w0, 11025
	cmp	w24, w0
	beq	.L107
	b	.L102
	.p2align 3
.L162:
	mov	w0, 44100
	cmp	w24, w0
	beq	.L107
	mov	w0, 48000
	cmp	w24, w0
	beq	.L107
	b	.L102
.L167:
	cbz	w23, .L170
.L157:
	mov	w1, 16640
	mov	w0, 26
	bl	HI_ADSP_MALLOC
	str	x0, [x25, 64]
	cbz	x0, .L155
	ldr	x1, .LC8
	sbfiz	x19, x27, 2, 32
	mov	x2, x19
	ldr	x4, [x1, 40]
	mov	w1, 0
	blr	x4
	ldr	x0, [x25, 64]
	add	x19, x0, x19
	ldr	x0, [x25, 96]
	str	x19, [x25, 72]
	b	.L125
.L170:
	add	w1, w27, 4096
	mov	w0, 26
	lsl	w1, w1, 1
	bl	HI_ADSP_MALLOC
	str	x0, [x25, 48]
	cbz	x0, .L155
	ldr	x1, .LC8
	sbfiz	x19, x27, 1, 32
	mov	x2, x19
	ldr	x4, [x1, 40]
	mov	w1, 0
	blr	x4
	ldr	x0, [x25, 48]
	add	x19, x0, x19
	ldr	x0, [x25, 96]
	str	x19, [x25, 56]
	b	.L125
.L160:
	ldr	x0, [x29, 120]
	bl	LSRC_Destroy
	mov	x0, 0
	b	.L128
.L155:
	mov	x1, x25
	mov	w0, 26
	bl	HI_ADSP_FREE
	b	.L102
	.size	ResamplerCreate, .-ResamplerCreate
	.align	3
.LC7:
	.xword	.LANCHOR0
	.align	3
.LC8:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResamplerFree
	.type	ResamplerFree, %function
ResamplerFree:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L171
	ldr	w0, [x19, 116]
	cbz	w0, .L196
	ldr	x0, [x19, 120]
	bl	LSRC_Destroy
.L193:
	mov	x1, x19
	mov	w0, 26
	bl	HI_ADSP_FREE
.L171:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L196:
	ldr	w0, [x19, 40]
	cmp	w0, 16
	beq	.L197
	sub	w1, w0, #18
	and	w1, w1, -3
	cmp	w1, wzr
	ccmp	w0, 24, 4, ne
	bne	.L193
.L195:
	ldr	x1, [x19, 64]
	cbz	x1, .L193
.L192:
	mov	w0, 26
	bl	HI_ADSP_FREE
	b	.L193
	.p2align 3
.L197:
	ldr	w0, [x19, 112]
	cbnz	w0, .L195
	ldr	x1, [x19, 48]
	cbnz	x1, .L192
	b	.L193
	.size	ResamplerFree, .-ResamplerFree
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResampleFlush
	.type	ResampleFlush, %function
ResampleFlush:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L198
	ldr	w0, [x19, 116]
	cbnz	w0, .L198
	ldr	w1, [x19]
	ldr	w0, [x19, 20]
	sub	w1, w1, #1
	ldr	x3, [x19, 96]
	ldr	w2, [x19, 40]
	mul	w0, w1, w0
	cmp	w2, 16
	str	wzr, [x19, 36]
	stp	wzr, wzr, [x19, 28]
	add	x0, x3, x0, sxtw 1
	stp	x0, x3, [x19, 80]
	beq	.L208
	sub	w0, w2, #18
	and	w0, w0, -3
	cmp	w0, wzr
	ccmp	w2, 24, 4, ne
	beq	.L209
.L198:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L208:
	ldr	w0, [x19, 112]
	ldr	x3, .LC9
	ldr	w2, [x19, 24]
	cbnz	w0, .L207
	add	w2, w2, 4096
	ldr	x3, [x3, 40]
	ldr	x0, [x19, 48]
	mov	w1, 0
	sbfiz	x2, x2, 1, 32
	blr	x3
	ldrsw	x0, [x19, 24]
	ldr	x1, [x19, 48]
	add	x0, x1, x0, lsl 1
	str	x0, [x19, 56]
	b	.L198
	.p2align 3
.L209:
	ldr	x3, .LC9
	ldr	w2, [x19, 24]
.L207:
	ldr	x3, [x3, 40]
	add	w2, w2, 4096
	ldr	x0, [x19, 64]
	mov	w1, 0
	sbfiz	x2, x2, 2, 32
	blr	x3
	ldrsw	x0, [x19, 24]
	ldr	x1, [x19, 64]
	add	x0, x1, x0, lsl 2
	str	x0, [x19, 72]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ResampleFlush, .-ResampleFlush
	.align	3
.LC9:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	NEW_ResamplerCheck
	.type	NEW_ResamplerCheck, %function
NEW_ResamplerCheck:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	w22, w3
	mov	w23, w4
	bl	_mcount
	cbz	x19, .L213
	ldr	w0, [x19, 116]
	cbnz	w0, .L212
	ldr	w1, [x19, 8]
	mov	w0, 1
	cmp	w20, w1
	beq	.L217
.L211:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L217:
	ldr	w1, [x19, 12]
	cmp	w21, w1
	bne	.L211
	ldr	w1, [x19, 16]
	cmp	w22, w1
	bne	.L211
	ldr	w0, [x19, 40]
	cmp	w0, w23
	cset	w0, ne
	b	.L211
	.p2align 3
.L212:
	ldr	x0, [x19, 120]
	mov	w4, w22
	mov	w3, w23
	mov	w2, w21
	mov	w1, w20
	bl	LSRC_CheckUpdate
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L213:
	mov	w0, -2147483647
	b	.L211
	.size	NEW_ResamplerCheck, .-NEW_ResamplerCheck
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResamplerGetMaxOutputNum
	.type	ResamplerGetMaxOutputNum, %function
ResamplerGetMaxOutputNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L221
	ldr	w0, [x19, 116]
	cbnz	w0, .L220
	cmp	w20, 2048
	mov	w0, -2147483645
	bgt	.L219
	tbnz	x20, 0, .L219
	ldr	w1, [x19, 16]
	ldp	w0, w2, [x19, 8]
	sub	w1, w1, #1
	add	w20, w1, w20
	mul	w20, w20, w2
	sdiv	w0, w20, w0
	add	w0, w0, 1
.L219:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L220:
	ldr	x0, [x19, 120]
	mov	w1, w20
	bl	LSRC_GetMaxOutputNum
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L221:
	mov	w0, -2147483647
	b	.L219
	.size	ResamplerGetMaxOutputNum, .-ResamplerGetMaxOutputNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResamplerGetMinInputNum
	.type	ResamplerGetMinInputNum, %function
ResamplerGetMinInputNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cbz	x20, .L229
	ldr	w0, [x20, 116]
	cbnz	w0, .L226
	ldp	w2, w5, [x20, 8]
	ldr	w4, [x20, 16]
	mov	w0, -2147483645
	mul	w2, w19, w2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	sdiv	w3, w2, w5
	msub	w2, w3, w5, w2
	cmp	w2, wzr
	csinc	w3, w3, w3, eq
	mul	w2, w3, w4
	and	w2, w2, 1
	add	w3, w3, w2
	cmp	w3, 2048
	csel	w0, w3, w0, le
	ret
	.p2align 3
.L226:
	ldr	x0, [x20, 120]
	mov	w1, w19
	bl	LSRC_GetMinInputNum
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L229:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ResamplerGetMinInputNum, .-ResamplerGetMinInputNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResamplerProcessFrame
	.type	ResamplerProcessFrame, %function
ResamplerProcessFrame:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	x23, x1
	mov	x19, x2
	mov	w22, w3
	mov	w21, w4
	mov	w24, w5
	mov	w25, w6
	bl	_mcount
	cbz	x20, .L244
	ldr	w0, [x20, 116]
	cbz	w0, .L275
	ldr	x0, [x20, 120]
	mov	w3, w22
	mov	x2, x19
	mov	x1, x23
	bl	LSRC_ProcessFrame
	mov	w24, w0
.L271:
	mov	w0, w24
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L275:
	cbz	x23, .L264
	cmp	w22, 2048
	bgt	.L240
	tbnz	x22, 0, .L240
	cbz	x19, .L264
	ldr	w0, [x20, 8]
	cmp	w21, w0
	beq	.L276
.L241:
	mov	w2, w24
	mov	w1, w21
	mov	x0, x20
	bl	ResampleConfig
.L242:
	mov	w1, w22
	mov	x0, x20
	bl	ResamplerGetMaxOutputNum
	tbnz	w0, #31, .L277
	ldr	w4, [x20, 16]
	cmp	w4, 2
	beq	.L278
	cmp	w4, 1
	bne	.L266
	ldr	w0, [x20, 40]
	cmp	w0, 16
	beq	.L279
	sub	w1, w0, #18
	and	w1, w1, -3
	cmp	w1, wzr
	ccmp	w0, 24, 4, ne
	bne	.L274
.L256:
	mov	x3, x19
	mov	w2, w22
	mov	x1, x23
	mov	x0, x20
	bl	ResamplerMonoProcess24
	mov	w24, w0
	mov	w0, w24
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L276:
	ldr	w0, [x20, 12]
	cmp	w24, w0
	bne	.L241
	b	.L242
	.p2align 3
.L278:
	ldr	w0, [x20, 40]
	cmp	w0, 16
	beq	.L280
	sub	w1, w0, #18
	and	w1, w1, -3
	cmp	w1, wzr
	ccmp	w0, 24, 4, ne
	bne	.L274
.L247:
	mov	w4, w25
	mov	x3, x19
	mov	w2, w22
	mov	x1, x23
	mov	x0, x20
	bl	ResamplerStereoProcess24
	mov	w24, w0
	b	.L271
	.p2align 3
.L277:
	mov	w1, -2147483647
	cmp	w0, w1
	beq	.L244
	mov	w1, -2147483645
	cmp	w0, w1
	beq	.L240
	mov	w24, 8
	movk	w24, 0x8000, lsl 16
	b	.L271
	.p2align 3
.L240:
	mov	w24, -2147483645
	b	.L271
.L274:
	mov	w24, 6
	movk	w24, 0x8000, lsl 16
	b	.L271
	.p2align 3
.L279:
	ldr	w24, [x20, 112]
	cbnz	w24, .L256
	ldr	x0, [x20, 56]
	mov	w2, w22
	mov	x1, x23
	mov	w3, w25
	ldr	w21, [x20, 20]
	ldr	w22, [x20, 28]
	ldr	x23, [x20, 104]
	bl	cvtShortShort
	sxtw	x3, w21
	ldrsw	x4, [x20, 32]
	ldr	x2, [x20, 56]
	sbfiz	x1, x0, 1, 32
	neg	x3, x3, lsl 1
	add	x3, x3, 2
	add	x2, x2, x3
	add	x0, x2, x1
	add	x13, x2, x4, lsl 1
	cmp	x0, x13
	ldp	x11, x12, [x20, 80]
	bls	.L257
	add	x14, x19, 2
	.p2align 2
.L262:
	cmp	w21, wzr
	sub	x8, x13, #2
	ble	.L258
	mov	x10, x13
	mov	x7, 0
	mov	w9, 0
	mov	w2, 0
	mov	w4, 0
	.p2align 2
.L259:
	ldr	w15, [x12, x7]
	add	w9, w9, 2
	ldrsh	w5, [x8], -4
	cmp	w21, w9
	lsl	w6, w15, 16
	ldr	w16, [x11, x7]
	lsl	w5, w5, 16
	ldrsh	w3, [x10], 4
	and	w15, w15, -65536
	lsl	w17, w16, 16
	smull	x5, w5, w6
	ldrsh	w6, [x8, 2]
	lsl	w3, w3, 16
	and	w16, w16, -65536
	asr	x5, x5, 32
	lsl	w6, w6, 16
	add	w4, w4, w5
	smull	x3, w3, w17
	smull	x15, w6, w15
	ldrsh	w6, [x10, -2]
	asr	x3, x3, 32
	add	w2, w2, w3
	asr	x5, x15, 32
	lsl	w6, w6, 16
	add	x7, x7, 4
	add	w4, w4, w5
	smull	x3, w6, w16
	asr	x3, x3, 32
	add	w2, w2, w3
	bgt	.L259
	add	w2, w4, w2
	mov	x4, x14
	add	w2, w2, 16384
	asr	w3, w2, 31
	cmp	w3, w2, asr 30
	asr	w2, w2, 15
	beq	.L273
	eor	w2, w3, 32767
.L273:
	sxth	w2, w2
.L261:
	strh	w2, [x14, -2]
	add	x14, x14, 2
	ldr	w22, [x23, w22, sxtw 2]
	ldr	x12, [x20, 96]
	ubfx	x11, x22, 8, 12
	lsr	w2, w22, 20
	mul	w11, w11, w21
	mul	w2, w2, w21
	add	x11, x12, x11, sxtw 1
	add	x12, x12, x2, sxtw 1
	and	x2, x22, 254
	add	x13, x13, x2
	asr	w22, w22, 20
	cmp	x0, x13
	bhi	.L262
	sub	x4, x4, x19
	lsr	x24, x4, 1
.L257:
	sub	x0, x13, x0
	ldr	x3, .LC10
	asr	x0, x0, 1
	ldrsw	x2, [x20, 24]
	stp	w22, w0, [x20, 28]
	ldr	x0, [x20, 48]
	lsl	x2, x2, 1
	ldr	x3, [x3, 56]
	stp	x11, x12, [x20, 80]
	add	x1, x0, x1
	blr	x3
	b	.L271
.L258:
	mov	x4, x14
	mov	w2, 0
	b	.L261
.L280:
	ldr	w24, [x20, 112]
	cbnz	w24, .L247
	ldr	x0, [x20, 56]
	mov	w2, w22
	mov	x1, x23
	mov	w3, w25
	ldr	w21, [x20, 20]
	ldr	w22, [x20, 28]
	ldr	x23, [x20, 104]
	bl	cvtShortShort
	ldr	x15, [x20, 56]
	sub	w2, w21, #1
	ldrsw	x3, [x20, 32]
	sbfiz	x1, x0, 1, 32
	sub	x2, x15, x2, sxtw 2
	add	x0, x2, x1
	add	x15, x2, x3, lsl 1
	cmp	x0, x15
	ldp	x13, x14, [x20, 80]
	bls	.L248
	mov	x12, x19
	.p2align 2
.L254:
	cmp	w21, wzr
	sub	x9, x15, #4
	ble	.L249
	mov	x11, x15
	mov	x8, 0
	mov	w10, 0
	mov	w3, 0
	mov	w5, 0
	.p2align 2
.L250:
	ldr	w16, [x9], -8
	add	w10, w10, 2
	ldr	w2, [x14, x8]
	cmp	w21, w10
	lsl	w7, w16, 16
	and	w16, w16, -65536
	lsl	w6, w2, 16
	ldr	w18, [x11], 8
	ldr	w4, [x13, x8]
	and	w2, w2, -65536
	smull	x7, w7, w6
	smull	x6, w6, w16
	asr	x7, x7, 32
	ldr	w17, [x9, 4]
	asr	x6, x6, 32
	add	w7, w5, w7
	add	w3, w3, w6
	lsl	w5, w4, 16
	lsl	w6, w18, 16
	and	w18, w18, -65536
	ldr	w16, [x11, -4]
	and	w4, w4, -65536
	smull	x6, w6, w5
	smull	x5, w5, w18
	asr	x6, x6, 32
	add	x8, x8, 4
	add	w7, w7, w6
	lsl	w6, w17, 16
	and	w17, w17, -65536
	asr	x5, x5, 32
	smull	x6, w6, w2
	smull	x2, w2, w17
	add	w3, w3, w5
	asr	x6, x6, 32
	asr	x2, x2, 32
	add	w5, w7, w6
	add	w2, w3, w2
	lsl	w7, w16, 16
	and	w3, w16, -65536
	smull	x7, w7, w4
	smull	x3, w4, w3
	asr	x7, x7, 32
	asr	x3, x3, 32
	add	w5, w5, w7
	add	w3, w2, w3
	bgt	.L250
	add	w5, w5, 16384
	asr	w2, w5, 31
	cmp	w2, w5, asr 30
	asr	w5, w5, 15
	beq	.L272
	eor	w5, w2, 32767
.L272:
	add	w3, w3, 16384
	sxth	w5, w5
	asr	w2, w3, 31
	asr	w4, w3, 30
	cmp	w2, w4
	strh	w5, [x12], 4
	asr	w3, w3, 15
	beq	.L253
	eor	w3, w2, 32767
.L253:
	strh	w3, [x12, -2]
	ldr	w22, [x23, w22, sxtw 2]
	ldr	x14, [x20, 96]
	ubfx	x13, x22, 8, 12
	lsr	w2, w22, 20
	add	x15, x15, x22, uxtb 1
	asr	w22, w22, 20
	mul	w13, w13, w21
	mul	w2, w2, w21
	cmp	x0, x15
	add	x13, x14, x13, sxtw 1
	add	x14, x14, x2, sxtw 1
	bhi	.L254
	sub	x2, x12, x19
	asr	x2, x2, 1
	add	x2, x2, x2, lsr 63
	lsr	x24, x2, 1
.L248:
	sub	x0, x15, x0
	ldr	x3, .LC10
	asr	x0, x0, 1
	ldrsw	x2, [x20, 24]
	stp	w22, w0, [x20, 28]
	ldr	x0, [x20, 48]
	lsl	x2, x2, 1
	ldr	x3, [x3, 56]
	stp	x13, x14, [x20, 80]
	add	x1, x0, x1
	blr	x3
	b	.L271
.L266:
	mov	w24, 5
	movk	w24, 0x8000, lsl 16
	b	.L271
.L249:
	strh	wzr, [x12], 4
	mov	w3, 0
	b	.L253
.L244:
	mov	w24, -2147483647
	b	.L271
.L264:
	mov	w24, 2
	movk	w24, 0x8000, lsl 16
	b	.L271
	.size	ResamplerProcessFrame, .-ResamplerProcessFrame
	.align	3
.LC10:
	.xword	g_AdspOsalFunc
	.data
	.align	3
.LANCHOR0 = . + 0
	.type	resampler_info, %object
	.size	resampler_info, 1280
resampler_info:
	.word	32000
	.word	48000
	.word	1
	.word	8
	.xword	SRC_res_32_48_mid_nextphasetab
	.xword	SRC_res_32_48_mid_filttab
	.word	44100
	.word	48000
	.word	1
	.word	8
	.xword	SRC_res_44_48_mid_nextphasetab
	.xword	SRC_res_44_48_mid_filttab
	.word	48000
	.word	48000
	.word	1
	.word	8
	.xword	res_48_48_nextphasetab
	.xword	res_48_48_good_filttab
	.word	8000
	.word	11025
	.word	2
	.word	8
	.xword	SRC_res_8_11_good_nextphasetab
	.xword	SRC_res_8_11_mid_filttab
	.word	16000
	.word	22050
	.word	2
	.word	8
	.xword	SRC_res_8_11_good_nextphasetab
	.xword	SRC_res_8_11_mid_filttab
	.word	8000
	.word	22050
	.word	2
	.word	8
	.xword	SRC_res_8_22_good_nextphasetab
	.xword	SRC_res_8_22_mid_filttab
	.word	16000
	.word	44100
	.word	2
	.word	8
	.xword	SRC_res_8_22_good_nextphasetab
	.xword	SRC_res_8_22_mid_filttab
	.word	8000
	.word	44100
	.word	2
	.word	8
	.xword	SRC_res_8_44_good_nextphasetab
	.xword	SRC_res_8_44_mid_filttab
	.word	11025
	.word	12000
	.word	2
	.word	8
	.xword	SRC_res_11_12_good_nextphasetab
	.xword	SRC_res_11_12_mid_filttab
	.word	22050
	.word	24000
	.word	2
	.word	8
	.xword	SRC_res_11_12_good_nextphasetab
	.xword	SRC_res_11_12_mid_filttab
	.word	11025
	.word	16000
	.word	2
	.word	8
	.xword	SRC_res_11_16_good_nextphasetab
	.xword	SRC_res_11_16_mid_filttab
	.word	22050
	.word	32000
	.word	2
	.word	8
	.xword	SRC_res_11_16_good_nextphasetab
	.xword	SRC_res_11_16_mid_filttab
	.word	11025
	.word	24000
	.word	2
	.word	8
	.xword	SRC_res_11_24_good_nextphasetab
	.xword	SRC_res_11_24_mid_filttab
	.word	22050
	.word	48000
	.word	2
	.word	8
	.xword	SRC_res_11_24_good_nextphasetab
	.xword	SRC_res_11_24_mid_filttab
	.word	11025
	.word	32000
	.word	2
	.word	8
	.xword	SRC_res_11_32_good_nextphasetab
	.xword	SRC_res_11_32_mid_filttab
	.word	11025
	.word	48000
	.word	2
	.word	8
	.xword	SRC_res_11_48_good_nextphasetab
	.xword	SRC_res_11_48_mid_filttab
	.word	12000
	.word	22050
	.word	2
	.word	8
	.xword	SRC_res_12_22_good_nextphasetab
	.xword	SRC_res_12_22_mid_filttab
	.word	24000
	.word	44100
	.word	2
	.word	8
	.xword	SRC_res_12_22_good_nextphasetab
	.xword	SRC_res_12_22_mid_filttab
	.word	12000
	.word	44100
	.word	2
	.word	8
	.xword	SRC_res_12_44_good_nextphasetab
	.xword	SRC_res_12_44_mid_filttab
	.word	8000
	.word	12000
	.word	2
	.word	16
	.xword	SRC_res_8_12_good_nextphasetab
	.xword	SRC_res_8_12_good_filttab
	.word	8000
	.word	16000
	.word	2
	.word	16
	.xword	SRC_res_8_16_good_nextphasetab
	.xword	SRC_res_8_16_good_filttab
	.word	8000
	.word	24000
	.word	2
	.word	16
	.xword	SRC_res_8_24_good_nextphasetab
	.xword	SRC_res_8_24_good_filttab
	.word	8000
	.word	32000
	.word	2
	.word	16
	.xword	SRC_res_8_32_good_nextphasetab
	.xword	SRC_res_8_32_good_filttab
	.word	8000
	.word	48000
	.word	2
	.word	16
	.xword	SRC_res_8_48_good_nextphasetab
	.xword	SRC_res_8_48_good_filttab
	.word	11025
	.word	22050
	.word	2
	.word	16
	.xword	SRC_res_8_16_good_nextphasetab
	.xword	SRC_res_8_16_good_filttab
	.word	11025
	.word	44100
	.word	2
	.word	16
	.xword	SRC_res_8_32_good_nextphasetab
	.xword	SRC_res_8_32_good_filttab
	.word	12000
	.word	16000
	.word	2
	.word	16
	.xword	SRC_res_12_16_good_nextphasetab
	.xword	SRC_res_12_16_good_filttab
	.word	12000
	.word	24000
	.word	2
	.word	16
	.xword	SRC_res_8_16_good_nextphasetab
	.xword	SRC_res_8_16_good_filttab
	.word	12000
	.word	32000
	.word	2
	.word	16
	.xword	SRC_res_12_32_good_nextphasetab
	.xword	SRC_res_12_32_good_filttab
	.word	12000
	.word	48000
	.word	2
	.word	16
	.xword	SRC_res_8_32_good_nextphasetab
	.xword	SRC_res_8_32_good_filttab
	.word	16000
	.word	24000
	.word	2
	.word	16
	.xword	SRC_res_8_12_good_nextphasetab
	.xword	SRC_res_8_12_good_filttab
	.word	16000
	.word	32000
	.word	2
	.word	16
	.xword	SRC_res_8_16_good_nextphasetab
	.xword	SRC_res_8_16_good_filttab
	.word	16000
	.word	48000
	.word	2
	.word	16
	.xword	SRC_res_8_24_good_nextphasetab
	.xword	SRC_res_8_24_good_filttab
	.word	22050
	.word	44100
	.word	2
	.word	16
	.xword	SRC_res_8_16_good_nextphasetab
	.xword	SRC_res_8_16_good_filttab
	.word	24000
	.word	32000
	.word	2
	.word	16
	.xword	SRC_res_12_16_good_nextphasetab
	.xword	SRC_res_12_16_good_filttab
	.word	24000
	.word	48000
	.word	2
	.word	16
	.xword	SRC_res_8_16_good_nextphasetab
	.xword	SRC_res_8_16_good_filttab
	.word	32000
	.word	44100
	.word	2
	.word	16
	.xword	SRC_res_8_11_good_nextphasetab
	.xword	SRC_res_8_11_good_filttab
	.word	32000
	.word	48000
	.word	2
	.word	16
	.xword	SRC_res_8_12_good_nextphasetab
	.xword	SRC_res_8_12_good_filttab
	.word	44100
	.word	48000
	.word	2
	.word	36
	.xword	SRC_res_11_12_good_nextphasetab
	.xword	SRC_res_11_12_good_filttab
	.word	48000
	.word	48000
	.word	2
	.word	8
	.xword	res_48_48_nextphasetab
	.xword	res_48_48_good_filttab
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
