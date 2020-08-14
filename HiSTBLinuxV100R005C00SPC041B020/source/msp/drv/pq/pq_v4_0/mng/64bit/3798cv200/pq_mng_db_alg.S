#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"pq_mng_db_alg.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	CheckStartEndBorder, %function
CheckStartEndBorder:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	sxtb	w21, w1
	mov	x20, x2
	mov	x22, x3
	bl	_mcount
	cmp	w19, wzr
	ble	.L2
	add	w1, w21, w21, lsr 31
	mov	x4, x20
	asr	w1, w1, 1
	mov	w0, 0
	b	.L4
	.p2align 3
.L3:
	add	w0, w0, 1
	add	x4, x4, 1
	cmp	w19, w0
	beq	.L2
.L4:
	ldrb	w2, [x4]
	cmp	w2, 1
	bne	.L3
	cmp	w1, w0
	blt	.L3
	strb	wzr, [x4]
	ldr	w0, [x22]
	sub	w0, w0, #1
	str	w0, [x22]
.L2:
	cmp	w19, 1
	ble	.L1
	add	w21, w21, w21, lsr 31
	mov	w4, 1
	asr	w2, w21, 1
	b	.L7
	.p2align 3
.L6:
	add	w4, w4, 1
	cmp	w19, w4
	beq	.L1
.L7:
	sub	w0, w19, w4
	sxtw	x0, w0
	add	x1, x20, x0
	ldrb	w0, [x20, x0]
	cmp	w0, 1
	bne	.L6
	cmp	w2, w4
	blt	.L6
	strb	wzr, [x1]
	ldr	w0, [x22]
	sub	w0, w0, #1
	str	w0, [x22]
.L1:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	CheckStartEndBorder, .-CheckStartEndBorder
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	FillBorder_Size8, %function
FillBorder_Size8:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x3
	mov	w22, w2
	sxtb	w23, w1
	bl	_mcount
	sub	w2, w22, #3
	stp	xzr, xzr, [x29, 64]
	stp	xzr, xzr, [x29, 80]
	cmp	w2, 4
	str	wzr, [x20]
	bls	.L22
	mov	w0, 4
	b	.L14
	.p2align 3
.L13:
	add	w0, w0, 1
	cmp	w0, w2
	beq	.L29
.L14:
	ldrb	w1, [x21, w0, sxtw]
	cmp	w1, 1
	bne	.L13
	ubfiz	x1, x0, 2, 3
	add	x3, x29, 96
	add	x1, x3, x1
	add	w0, w0, 1
	cmp	w0, w2
	ldr	w3, [x1, -32]
	add	w3, w3, 1
	str	w3, [x1, -32]
	bne	.L14
.L29:
	ldr	w1, [x29, 64]
.L12:
	mov	x0, 0
	mov	w19, 0
	mov	w2, 0
	b	.L21
	.p2align 3
.L16:
	add	x1, x29, 64
	ldr	w1, [x1, x0, lsl 2]
.L21:
	cmp	w2, w1
	bcs	.L15
	mov	w19, w0
	mov	w2, w1
.L15:
	add	x0, x0, 1
	cmp	x0, 8
	bne	.L16
	uxtw	x2, w22
	mov	w1, 0
	mov	x0, x21
	bl	memset
	cbz	w19, .L17
	sub	w0, w19, #1
	mov	w1, 1
	strb	w1, [x21, x0]
	ldr	w0, [x20]
	add	w0, w0, 1
	str	w0, [x20]
.L17:
	sub	w0, w23, #1
	mov	w2, 1
	add	w0, w0, w19
	cmp	w22, w0
	bls	.L11
	.p2align 2
.L25:
	strb	w2, [x21, w0, sxtw]
	add	w0, w0, w23
	cmp	w22, w0
	ldr	w1, [x20]
	add	w1, w1, 1
	str	w1, [x20]
	bhi	.L25
.L11:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
.L22:
	mov	w1, 0
	b	.L12
	.size	FillBorder_Size8, .-FillBorder_Size8
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	FillBorderHchrVy.constprop.15, %function
FillBorderHchrVy.constprop.15:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	uxtb	w19, w2
	mov	w23, w1
	bl	_mcount
	ldr	x0, .LC4
	cmp	w19, 1
	ldr	x3, [x20, 4120]
	add	x1, x0, 16384
	ldr	x21, [x20, 4128]
	ldr	x19, [x0, 20504]
	ldr	x2, [x0, 20512]
	add	x0, x20, 8192
	ldrsb	w22, [x1, 13]
	ldr	x5, [x20, 8648]
	ldr	x4, [x20, 8656]
	add	x20, x0, 488
	ldrsb	w1, [x1, 14]
	beq	.L85
	cmp	w22, 8
	beq	.L86
.L33:
	sub	w4, w23, #3
	ldrb	w6, [x0, 476]
	cmp	w4, 4
	str	wzr, [x20]
	bls	.L63
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L63
	mov	w2, w4
	mov	w0, 4
	b	.L37
	.p2align 3
.L38:
	ldrb	w1, [x19, w0, sxtw]
	cmp	w1, 1
	beq	.L87
.L37:
	add	w0, w0, 1
	cmp	w0, w4
	bne	.L38
	mov	w10, w4
.L35:
	sub	w0, w10, w22
	mov	w3, 1
	cmp	w0, 4
	ble	.L43
	.p2align 2
.L74:
	strb	w3, [x19, w0, sxtw]
	sub	w0, w0, w22
	cmp	w0, 4
	ldr	w1, [x20]
	add	w1, w1, 1
	str	w1, [x20]
	bgt	.L74
.L43:
	cmp	w4, wzr
	mov	w0, w4
	ble	.L41
	ldrb	w1, [x19, w4, sxtw]
	cmp	w1, 1
	bne	.L45
	b	.L41
	.p2align 3
.L46:
	ldrb	w1, [x19, w0, sxtw]
	cmp	w1, 1
	beq	.L41
.L45:
	subs	w0, w0, #1
	bne	.L46
.L41:
	add	w1, w22, w0
	mov	w7, 1
	cmp	w1, w4
	bcs	.L51
	.p2align 2
.L73:
	strb	w7, [x19, w1, sxtw]
	add	w1, w1, w22
	cmp	w1, w4
	ldr	w3, [x20]
	add	w3, w3, 1
	str	w3, [x20]
	bcc	.L73
.L51:
	cmp	w0, w10
	mov	w7, w10
	mov	w4, 0
	mov	w9, 1
	bcs	.L72
	b	.L49
	.p2align 3
.L54:
	add	w2, w2, 1
	cmp	w0, w2
	mov	w7, w2
	bcc	.L49
.L72:
	ldrb	w1, [x19, w2, sxtw]
	cmp	w1, 1
	bne	.L54
	ldr	w3, [x20]
	cmp	w22, wzr
	ccmp	w10, w7, 4, ne
	add	w3, w3, 1
	str	w3, [x20]
	beq	.L66
	sub	w8, w7, w4
	udiv	w8, w8, w22
	cmp	w8, 1
	bls	.L66
	.p2align 2
.L55:
	add	w4, w4, w22
	add	w1, w1, 1
	cmp	w8, w1
	strb	w9, [x19, w4, uxtw]
	ldr	w3, [x20]
	add	w3, w3, 1
	str	w3, [x20]
	bne	.L55
.L66:
	add	w2, w2, 1
	mov	w4, w7
	cmp	w0, w2
	mov	w7, w2
	bcs	.L72
.L49:
	cmp	w23, 4
	mov	w0, 4
	bls	.L34
	.p2align 2
.L71:
	sxtw	x1, w0
	sub	x2, x1, #1
	ldrb	w3, [x19, x1]
	cmp	w3, 1
	beq	.L88
	ldrsb	w1, [x5, x2]
	cmp	w1, wzr
	sub	w1, w1, #1
	ble	.L60
.L83:
	strb	w1, [x5, x2]
.L59:
	add	w0, w0, 1
	cmp	w0, w23
	bne	.L71
.L34:
	mov	x3, x20
	mov	x2, x19
	mov	w1, w22
	mov	w0, w23
	bl	CheckStartEndBorder
	uxtw	x2, w23
	mov	x1, x19
	mov	x0, x21
	bl	memcpy
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L60:
	strb	wzr, [x5, x2]
	b	.L59
	.p2align 3
.L88:
	strb	wzr, [x19, x1]
	strb	w3, [x19, x2]
	ldrsb	w1, [x5, x2]
	cmp	w1, w6
	add	w1, w1, 1
	blt	.L83
	strb	w6, [x5, x2]
	b	.L59
	.p2align 3
.L87:
	mov	w2, w0
	mov	w10, w0
	b	.L35
	.p2align 3
.L85:
	mov	w22, w1
	mov	x5, x4
	cmp	w22, 8
	add	x20, x0, 484
	mov	x21, x3
	mov	x19, x2
	bne	.L33
.L86:
	mov	x3, x20
	mov	w2, w23
	mov	w1, w22
	mov	x0, x19
	bl	FillBorder_Size8
	b	.L34
.L63:
	mov	w10, 4
	mov	w2, w10
	b	.L35
	.size	FillBorderHchrVy.constprop.15, .-FillBorderHchrVy.constprop.15
	.align	3
.LC4:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_GetDbDetInfo
	.type	PQ_MNG_ALG_GetDbDetInfo, %function
PQ_MNG_ALG_GetDbDetInfo:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x2
	mov	x23, x1
	bl	_mcount
	ldr	x20, .LC6
	mov	x2, 20520
	mov	w1, 0
	mov	x0, x20
	bl	memset
	mov	x2, 8808
	mov	x1, x22
	mov	x0, x19
	bl	memcpy
	add	x0, x20, 16384
	ldr	w1, [x22, 48]
	strb	w1, [x0, 12]
	add	w1, w1, 1
	ldr	w2, [x22, 64]
	uxtb	w1, w1
	strb	w2, [x0, 13]
	cmp	w1, 64
	ldr	w1, [x22, 56]
	strb	w1, [x0, 14]
	ldr	w3, [x22, 44]
	str	w3, [x0]
	ldr	w3, [x22, 60]
	str	w3, [x0, 4]
	ldr	w3, [x22, 52]
	str	w3, [x0, 8]
	bls	.L90
	mov	w3, -1
	strb	w3, [x0, 12]
.L90:
	add	w0, w2, 1
	uxtb	w0, w0
	cmp	w0, 64
	bls	.L91
	add	x0, x20, 16384
	mov	w2, -1
	strb	w2, [x0, 13]
.L91:
	add	w0, w1, 1
	uxtb	w0, w0
	cmp	w0, 64
	bls	.L92
	add	x0, x20, 16384
	mov	w1, -1
	strb	w1, [x0, 14]
.L92:
	mov	x2, 16384
	mov	w1, 0
	mov	x0, x20
	add	x24, x22, 580
	bl	memset
	mov	x2, x24
	mov	x0, x20
	add	x4, x20, 16384
	.p2align 2
.L93:
	add	x0, x0, 8
	ldr	w1, [x2], 4
	cmp	x4, x0
	and	w3, w1, 2047
	ubfx	x1, x1, 16, 11
	stp	w3, w1, [x0, -8]
	bne	.L93
	add	x0, x20, 16384
	mov	w1, 0
	mov	x2, 4096
	add	x0, x0, 15
	add	x21, x22, 68
	bl	memset
	ldr	w1, [x22, 68]
	add	x6, x22, 576
	mov	x4, x21
	mov	x5, 0
	.p2align 2
.L94:
	add	x0, x5, 16384
	and	w1, w1, 1
	add	x0, x0, 15
	add	w2, w5, 1
	mov	w3, 1
	strb	w1, [x20, x0]
	b	.L97
	.p2align 3
.L95:
	lsr	w1, w1, w3
	add	w3, w3, 1
	and	w1, w1, 1
	cmp	w3, 32
	strb	w1, [x0, 15]
	add	w2, w2, 1
	beq	.L282
.L97:
	cmp	w2, 4095
	add	x0, x20, x2, uxtw
	add	x0, x0, 16384
	ldr	w1, [x4]
	bls	.L95
.L213:
	add	x0, x20, 16384
	add	x2, x0, 15
	add	x1, x0, 1935
	str	x2, [x20, 20496]
	add	x0, x0, 2895
	str	x1, [x20, 20504]
	str	x0, [x20, 20512]
	ldp	w21, w1, [x22]
	ldr	w2, [x22, 8]
	sub	w0, w21, #4
	cmp	w0, 3836
	bhi	.L214
	sub	w0, w1, #1
	cmp	w0, 2159
	bhi	.L214
.L99:
	mov	w0, 1080
	cmp	w21, 1920
	ccmp	w1, w0, 0, le
	ble	.L100
	stp	wzr, wzr, [x22, 36]
.L100:
	cmp	w2, 960
	ble	.L101
	str	wzr, [x22, 36]
.L101:
	ldr	w0, [x22, 32]
	cmp	w0, 1
	beq	.L283
.L102:
	mov	w0, -1
	strb	w0, [x23]
.L116:
	ldr	w0, [x22, 36]
	cmp	w0, 1
	beq	.L284
.L162:
	mov	w0, -1
	strb	w0, [x23, 1]
.L176:
	ldr	w0, [x22, 40]
	cmp	w0, 1
	beq	.L285
.L187:
	mov	w0, -1
	strb	w0, [x23, 2]
.L201:
	ldr	w0, [x23, 4]
	str	w0, [x19, 44]
	ldrsb	w0, [x23]
	str	w0, [x19, 48]
	ldr	w0, [x23, 8]
	str	w0, [x19, 52]
	ldrsb	w0, [x23, 2]
	str	w0, [x19, 56]
	ldr	w0, [x23, 12]
	str	w0, [x19, 60]
	ldrsb	w0, [x23, 1]
	str	w0, [x19, 64]
	ldr	w0, [x23, 8672]
	str	w0, [x19, 16]
	ldr	w0, [x23, 8676]
	str	w0, [x19, 20]
	ldr	w0, [x23, 8680]
	str	w0, [x19, 24]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L214:
	str	wzr, [x22, 28]
	b	.L99
	.p2align 3
.L282:
	cmp	x4, x6
	beq	.L213
	ldr	w1, [x4, 4]!
	add	x5, x5, 32
	cmp	x4, x24
	bne	.L94
	b	.L213
.L283:
	ldr	w0, [x22, 28]
	cmp	w0, 1
	bne	.L102
	add	x1, x20, 16384
	add	x0, x23, 4096
	ldr	w3, [x1]
	ldrb	w2, [x0, 42]
	cmp	w3, w2
	bcc	.L286
	ldrb	w1, [x1, 12]
	add	w1, w1, 1
	uxtb	w1, w1
	cmp	w1, 1
	bls	.L104
.L105:
	ldrb	w4, [x0, 40]
	add	x2, x0, 64
	sub	w1, w4, #1
	sxtb	w1, w1
	cmp	w1, wzr
	ble	.L106
.L107:
	sbfiz	x3, x1, 3, 32
	sub	w1, w1, #1
	sub	x4, x3, #8
	add	x5, x2, x3
	add	x6, x2, x4
	sxtb	w1, w1
	ldrsb	w4, [x2, x4]
	strb	w4, [x2, x3]
	ldr	w3, [x6, 4]
	str	w3, [x5, 4]
	cbnz	w1, .L107
	ldrb	w4, [x0, 40]
.L106:
	add	x3, x20, 16384
	ldrsb	w1, [x3, 12]
	strb	w1, [x0, 64]
	ldr	w1, [x23, 4144]
	ldr	w3, [x3]
	cmp	w1, w4
	str	w3, [x23, 4164]
	bcs	.L108
	add	w1, w1, 1
	str	w1, [x23, 4144]
.L108:
	cmp	w4, w1
	beq	.L109
	add	x0, x20, 16384
	ldrsb	w25, [x23]
	ldrsb	w1, [x0, 12]
.L110:
	cmp	w25, w1
	bne	.L287
	add	w0, w25, 1
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L288
	add	x24, x23, 8192
	ldr	x26, [x20, 20496]
	cmp	w25, 8
	add	x27, x24, 480
	ldrb	w8, [x24, 476]
	str	wzr, [x23, 8672]
	beq	.L289
	sub	w4, w21, #3
	ldr	x7, [x23, 8640]
	cmp	w4, 4
	bls	.L218
	ldrb	w0, [x26, 4]
	cmp	w0, 1
	beq	.L218
	mov	w0, 4
	b	.L122
.L123:
	ldrb	w1, [x26, w0, sxtw]
	cmp	w1, 1
	beq	.L219
.L122:
	add	w0, w0, 1
	cmp	w0, w4
	bne	.L123
.L219:
	mov	w3, w0
.L120:
	sub	w5, w3, w25
	mov	x12, -4
	cmp	w5, 4
	mov	w11, 1
	ble	.L130
.L249:
	add	x1, x12, x5, sxtw 2
	mov	w9, 0
	add	x1, x20, x1
	mov	w10, 0
	mov	w2, 0
.L128:
	ldr	w6, [x1]
	add	x1, x1, 4
	cmp	w6, w10
	bls	.L127
	mov	w9, w2
	mov	w10, w6
.L127:
	add	w2, w2, 1
	cmp	w2, 3
	bne	.L128
	sub	w5, w5, #1
	add	w5, w5, w9
	strb	w11, [x26, w5, sxtw]
	sub	w5, w5, w25
	cmp	w5, 4
	ldr	w1, [x23, 8672]
	add	w1, w1, 1
	str	w1, [x23, 8672]
	bgt	.L249
.L130:
	cmp	w4, wzr
	mov	w5, w4
	ble	.L126
	ldrb	w1, [x26, w4, sxtw]
	cmp	w1, 1
	bne	.L132
	b	.L126
	.p2align 3
.L133:
	ldrb	w1, [x26, w5, sxtw]
	cmp	w1, 1
	beq	.L126
.L132:
	subs	w5, w5, #1
	bne	.L133
.L126:
	add	w6, w5, w25
	mov	x13, -4
	cmp	w6, w4
	mov	w12, 1
	bcs	.L140
.L248:
	add	x1, x13, x6, sxtw 2
	mov	w10, 0
	add	x1, x20, x1
	mov	w11, 0
	mov	w2, 0
.L138:
	ldr	w9, [x1]
	add	x1, x1, 4
	cmp	w9, w11
	bls	.L137
	mov	w10, w2
	mov	w11, w9
.L137:
	add	w2, w2, 1
	cmp	w2, 3
	bne	.L138
	sub	w6, w6, #1
	add	w6, w6, w10
	strb	w12, [x26, w6, sxtw]
	add	w6, w6, w25
	cmp	w6, w4
	ldr	w1, [x23, 8672]
	add	w1, w1, 1
	str	w1, [x23, 8672]
	bcc	.L248
.L140:
	cmp	w5, w3
	mov	w9, w3
	mov	w1, 0
	mov	x14, -4
	mov	w13, 1
	bcs	.L247
	b	.L136
	.p2align 3
.L143:
	add	w0, w0, 1
	cmp	w0, w5
	mov	w9, w0
	bhi	.L136
.L247:
	ldrb	w10, [x26, w0, sxtw]
	cmp	w10, 1
	bne	.L143
	ldr	w2, [x23, 8672]
	cmp	w9, w3
	ccmp	w25, wzr, 4, ne
	add	w2, w2, 1
	str	w2, [x23, 8672]
	beq	.L221
	sub	w11, w9, w1
	udiv	w11, w11, w25
	cmp	w11, 1
	bls	.L221
.L146:
	add	w1, w25, w1
	mov	w2, 0
	mov	w15, 0
	mov	w6, 0
	add	x4, x14, x1, uxtw 2
	add	x4, x20, x4
.L145:
	ldr	w12, [x4]
	add	x4, x4, 4
	cmp	w12, w15
	bls	.L144
	mov	w2, w6
	mov	w15, w12
.L144:
	add	w6, w6, 1
	cmp	w6, 3
	bne	.L145
	sub	w1, w1, #1
	add	w10, w10, 1
	add	w1, w1, w2
	cmp	w11, w10
	strb	w13, [x26, w1, uxtw]
	ldr	w2, [x23, 8672]
	add	w2, w2, 1
	str	w2, [x23, 8672]
	bne	.L146
.L221:
	mov	w1, w9
	b	.L143
.L285:
	ldr	w0, [x22, 28]
	cmp	w0, 1
	bne	.L187
	add	x1, x20, 16384
	add	x0, x23, 4096
	ldr	w21, [x22, 4]
	ldrb	w2, [x0, 44]
	ldr	w3, [x1, 8]
	cmp	w3, w2
	bcs	.L188
	mov	w2, -1
	strb	w2, [x1, 14]
.L189:
	add	x1, x20, 16384
	mov	w2, 100
	str	w2, [x1, 8]
.L190:
	ldrb	w4, [x0, 40]
	add	x2, x0, 192
	sub	w1, w4, #1
	sxtb	w1, w1
	cmp	w1, wzr
	ble	.L191
.L192:
	sbfiz	x3, x1, 3, 32
	sub	w1, w1, #1
	sub	x4, x3, #8
	add	x5, x2, x3
	add	x6, x2, x4
	sxtb	w1, w1
	ldrsb	w4, [x2, x4]
	strb	w4, [x2, x3]
	ldr	w3, [x6, 4]
	str	w3, [x5, 4]
	cbnz	w1, .L192
	ldrb	w4, [x0, 40]
.L191:
	add	x3, x20, 16384
	ldrsb	w1, [x3, 14]
	strb	w1, [x0, 192]
	ldr	w1, [x23, 4152]
	ldr	w3, [x3, 8]
	cmp	w1, w4
	str	w3, [x23, 4292]
	bcs	.L193
	add	w1, w1, 1
	str	w1, [x23, 4152]
.L193:
	cmp	w4, w1
	add	x1, x20, 16384
	beq	.L194
	ldrsb	w0, [x23, 2]
	ldrsb	w1, [x1, 14]
.L195:
	cmp	w0, w1
	bne	.L290
	add	w0, w0, 1
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L291
	mov	x0, x23
	mov	w2, 1
	mov	w1, w21
	bl	FillBorderHchrVy.constprop.15
	ldrsb	w0, [x23, 2]
	cmp	w0, 8
	beq	.L204
	add	x24, x23, 8192
	ldrb	w0, [x24, 474]
	ldrb	w1, [x24, 475]
	cmp	w1, w0
	bhi	.L205
	cmp	w21, 4
	mov	w0, 5
	mov	x1, 4
	mov	w10, 1
	bhi	.L243
	b	.L204
	.p2align 3
.L208:
	cmp	w21, w2
	add	x1, x1, 1
	add	w0, w0, 1
	bls	.L204
.L243:
	ldr	x5, [x23, 4120]
	mov	w2, w0
	ldrb	w9, [x5, x1]
	cmp	w9, 1
	bne	.L208
	ldr	x4, [x23, 8656]
	sub	x8, x1, #1
	ldrb	w6, [x24, 475]
	ldrsb	w3, [x4, x1]
	cmp	w3, w6
	bge	.L208
	ldrsb	w7, [x4, x8]
	uxtw	x3, w0
	cmp	w6, w7
	ldrsb	w4, [x4, x3]
	bgt	.L209
	cmp	w7, w4
	blt	.L210
	strb	w9, [x5, x8]
	ldr	x3, [x23, 4120]
	strb	wzr, [x3, x1]
	b	.L208
	.p2align 3
.L284:
	ldr	w0, [x22, 28]
	cmp	w0, 1
	bne	.L162
	add	x1, x20, 16384
	add	x0, x23, 4096
	ldr	w21, [x22, 8]
	ldrb	w2, [x0, 43]
	ldr	w3, [x1, 4]
	cmp	w3, w2
	bcc	.L292
	ldrb	w1, [x1, 13]
	add	w1, w1, 1
	uxtb	w1, w1
	cmp	w1, 1
	bls	.L164
.L165:
	ldrb	w4, [x0, 40]
	add	x2, x0, 320
	sub	w1, w4, #1
	sxtb	w1, w1
	cmp	w1, wzr
	ble	.L166
.L167:
	sbfiz	x3, x1, 3, 32
	sub	w1, w1, #1
	sub	x4, x3, #8
	add	x5, x2, x3
	add	x6, x2, x4
	sxtb	w1, w1
	ldrsb	w4, [x2, x4]
	strb	w4, [x2, x3]
	ldr	w3, [x6, 4]
	str	w3, [x5, 4]
	cbnz	w1, .L167
	ldrb	w4, [x0, 40]
.L166:
	add	x3, x20, 16384
	ldrsb	w1, [x3, 13]
	strb	w1, [x0, 320]
	ldr	w1, [x23, 4148]
	ldr	w3, [x3, 4]
	cmp	w1, w4
	str	w3, [x23, 4420]
	bcs	.L168
	add	w1, w1, 1
	str	w1, [x23, 4148]
.L168:
	cmp	w4, w1
	add	x1, x20, 16384
	beq	.L169
	ldrsb	w0, [x23, 1]
	ldrsb	w1, [x1, 13]
.L170:
	cmp	w0, w1
	bne	.L293
	add	w0, w0, 1
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L294
	mov	x0, x23
	mov	w2, 2
	mov	w1, w21
	bl	FillBorderHchrVy.constprop.15
	ldrsb	w0, [x23, 1]
	cmp	w0, 8
	beq	.L179
	add	x24, x23, 8192
	ldrb	w0, [x24, 473]
	ldrb	w1, [x24, 475]
	cmp	w1, w0
	bhi	.L180
	cmp	w21, 4
	mov	w0, 5
	mov	x1, 4
	mov	w10, 1
	bhi	.L244
	b	.L179
	.p2align 3
.L183:
	cmp	w21, w2
	add	x1, x1, 1
	add	w0, w0, 1
	bls	.L179
.L244:
	ldr	x5, [x23, 4128]
	mov	w2, w0
	ldrb	w9, [x5, x1]
	cmp	w9, 1
	bne	.L183
	ldr	x4, [x23, 8648]
	sub	x8, x1, #1
	ldrb	w7, [x24, 475]
	ldrsb	w3, [x4, x1]
	cmp	w3, w7
	bge	.L183
	ldrsb	w6, [x4, x8]
	uxtw	x3, w0
	cmp	w7, w6
	ldrsb	w4, [x4, x3]
	bgt	.L184
	cmp	w6, w4
	blt	.L185
	strb	w9, [x5, x8]
	ldr	x3, [x23, 4128]
	strb	wzr, [x3, x1]
	b	.L183
	.p2align 3
.L292:
	mov	w2, -1
	strb	w2, [x1, 13]
.L164:
	add	x1, x20, 16384
	mov	w2, 100
	str	w2, [x1, 4]
	b	.L165
.L286:
	mov	w2, -1
	strb	w2, [x1, 12]
.L104:
	add	x1, x20, 16384
	mov	w2, 100
	str	w2, [x1]
	b	.L105
.L188:
	ldrb	w1, [x1, 14]
	add	w1, w1, 1
	uxtb	w1, w1
	cmp	w1, 1
	bhi	.L190
	b	.L189
	.p2align 3
.L209:
	mov	w2, w0
.L210:
	cmp	w7, w4
	ccmp	w6, w4, 0, lt
	bgt	.L208
	strb	w10, [x5, x3]
	ldr	x3, [x23, 4120]
	strb	wzr, [x3, x1]
	b	.L208
	.p2align 3
.L169:
	cmp	w4, 1
	ldrsb	w1, [x1, 13]
	ble	.L225
	mov	w3, 1
	mov	w5, w3
	b	.L173
.L172:
	cmp	w3, w4
	bge	.L171
.L173:
	ldrsb	w6, [x2, 8]!
	add	w3, w3, 1
	cmp	w6, w1
	sxtb	w3, w3
	bne	.L172
	ldrb	w7, [x0, 62]
	add	w6, w5, 1
	ldr	w8, [x2, 4]
	cmp	w8, w7
	bcc	.L172
	uxtb	w5, w6
	b	.L172
.L225:
	mov	w5, 1
.L171:
	ldrb	w0, [x0, 60]
	cmp	w0, w5
	bhi	.L295
	add	x2, x20, 16384
	strb	w1, [x23, 1]
	mov	w0, w1
	ldrsb	w1, [x2, 13]
	b	.L170
.L205:
	add	w0, w0, 1
	strb	w0, [x24, 474]
.L204:
	add	x20, x20, 16384
	ldr	w0, [x20, 8]
	str	w0, [x23, 8]
	b	.L201
.L287:
	ldr	x0, [x23, 8640]
	sxtw	x2, w21
	mov	w1, 0
	bl	memset
	add	x0, x23, 8192
	strb	wzr, [x0, 472]
	b	.L116
.L109:
	add	x1, x20, 16384
	cmp	w4, 1
	ldrsb	w1, [x1, 12]
	ble	.L216
	mov	w3, 1
	mov	w5, w3
	b	.L113
.L112:
	cmp	w3, w4
	bge	.L111
.L113:
	ldrsb	w6, [x2, 8]!
	add	w3, w3, 1
	cmp	w6, w1
	sxtb	w3, w3
	bne	.L112
	ldrb	w7, [x0, 61]
	add	w6, w5, 1
	ldr	w8, [x2, 4]
	cmp	w8, w7
	bcc	.L112
	uxtb	w5, w6
	b	.L112
.L216:
	mov	w5, 1
.L111:
	ldrb	w0, [x0, 60]
	cmp	w0, w5
	bhi	.L296
	add	x0, x20, 16384
	strb	w1, [x23]
	mov	w25, w1
	ldrsb	w1, [x0, 12]
	b	.L110
.L184:
	mov	w2, w0
.L185:
	cmp	w7, w4
	ccmp	w6, w4, 0, le
	bge	.L183
	strb	w10, [x5, x3]
	ldr	x3, [x23, 4128]
	strb	wzr, [x3, x1]
	b	.L183
.L180:
	add	w0, w0, 1
	strb	w0, [x24, 473]
.L179:
	add	x0, x20, 16384
	ldr	w0, [x0, 4]
	str	w0, [x23, 12]
	b	.L176
.L293:
	ldr	x0, [x23, 8648]
	sxtw	x2, w21
	mov	w1, 0
	bl	memset
	add	x0, x23, 8192
	strb	wzr, [x0, 473]
	b	.L176
.L290:
	ldr	x0, [x23, 8656]
	sxtw	x2, w21
	mov	w1, 0
	bl	memset
	add	x0, x23, 8192
	strb	wzr, [x0, 474]
	b	.L201
.L194:
	cmp	w4, 1
	ldrsb	w1, [x1, 14]
	ble	.L228
	mov	w3, 1
	mov	w5, w3
	b	.L198
.L197:
	cmp	w4, w3
	ble	.L196
.L198:
	ldrsb	w6, [x2, 8]!
	add	w3, w3, 1
	cmp	w6, w1
	sxtb	w3, w3
	bne	.L197
	ldrb	w7, [x0, 63]
	add	w6, w5, 1
	ldr	w8, [x2, 4]
	cmp	w8, w7
	bcc	.L197
	uxtb	w5, w6
	b	.L197
.L228:
	mov	w5, 1
.L196:
	ldrb	w0, [x0, 60]
	cmp	w0, w5
	bhi	.L297
	add	x2, x20, 16384
	strb	w1, [x23, 2]
	mov	w0, w1
	ldrsb	w1, [x2, 14]
	b	.L195
.L294:
	sxtw	x21, w21
	ldr	x0, [x23, 4128]
	mov	x2, x21
	mov	w1, 0
	bl	memset
	str	wzr, [x23, 12]
	mov	w1, -1
	ldr	x0, [x23, 8648]
	strb	w1, [x23, 1]
	mov	x2, x21
	mov	w1, 0
	bl	memset
	add	x0, x23, 8192
	strb	wzr, [x0, 473]
	str	wzr, [x23, 8680]
	b	.L176
.L291:
	sxtw	x21, w21
	ldr	x0, [x23, 4120]
	mov	x2, x21
	mov	w1, 0
	bl	memset
	str	wzr, [x23, 8]
	mov	w1, -1
	ldr	x0, [x23, 8656]
	strb	w1, [x23, 2]
	mov	x2, x21
	mov	w1, 0
	bl	memset
	add	x0, x23, 8192
	strb	wzr, [x0, 474]
	str	wzr, [x23, 8676]
	b	.L201
.L295:
	ldrsb	w0, [x23, 1]
	b	.L170
.L289:
	mov	x3, x27
	mov	w2, w21
	mov	w1, w25
	mov	x0, x26
	bl	FillBorder_Size8
.L119:
	mov	x3, x27
	mov	x2, x26
	mov	w1, w25
	mov	w0, w21
	bl	CheckStartEndBorder
	ldr	x1, [x20, 20496]
	uxtw	x2, w21
	ldr	x0, [x23, 4112]
	bl	memcpy
	ldrsb	w0, [x23]
	cmp	w0, 8
	beq	.L154
	ldrb	w0, [x24, 472]
	ldrb	w1, [x24, 475]
	cmp	w1, w0
	bhi	.L155
	cmp	w21, 4
	mov	w0, 5
	mov	x1, 4
	mov	w10, 1
	bhi	.L245
	b	.L154
	.p2align 3
.L158:
	cmp	w21, w2
	add	x1, x1, 1
	add	w0, w0, 1
	bls	.L154
.L245:
	ldr	x5, [x23, 4112]
	mov	w2, w0
	ldrb	w9, [x5, x1]
	cmp	w9, 1
	bne	.L158
	ldr	x4, [x23, 8640]
	sub	x8, x1, #1
	ldrb	w7, [x24, 475]
	ldrsb	w3, [x4, x1]
	cmp	w3, w7
	bge	.L158
	ldrsb	w6, [x4, x8]
	uxtw	x3, w0
	cmp	w7, w6
	ldrsb	w4, [x4, x3]
	bgt	.L159
	cmp	w6, w4
	blt	.L160
	strb	w9, [x5, x8]
	ldr	x3, [x23, 4112]
	strb	wzr, [x3, x1]
	b	.L158
.L155:
	add	w0, w0, 1
	strb	w0, [x24, 472]
.L154:
	add	x0, x20, 16384
	ldr	w0, [x0]
	str	w0, [x23, 4]
	b	.L116
.L159:
	mov	w2, w0
.L160:
	cmp	w7, w4
	ccmp	w6, w4, 0, le
	bge	.L158
	strb	w10, [x5, x3]
	ldr	x3, [x23, 4112]
	strb	wzr, [x3, x1]
	b	.L158
.L288:
	sxtw	x21, w21
	ldr	x0, [x23, 4112]
	mov	x2, x21
	mov	w1, 0
	bl	memset
	str	wzr, [x23, 4]
	mov	w1, -1
	ldr	x0, [x23, 8640]
	strb	w1, [x23]
	mov	x2, x21
	mov	w1, 0
	bl	memset
	add	x0, x23, 8192
	strb	wzr, [x0, 472]
	str	wzr, [x23, 8672]
	b	.L116
	.p2align 3
.L136:
	cmp	w21, 4
	mov	w0, 4
	bhi	.L246
	b	.L119
	.p2align 3
.L148:
	ldrsb	w1, [x7, x3]
	cmp	w1, wzr
	sub	w1, w1, #1
	ble	.L151
	strb	w1, [x7, x3]
.L150:
	add	w0, w0, 1
	cmp	w21, w0
	beq	.L119
.L246:
	sxtw	x2, w0
	sub	x3, x2, #1
	ldrb	w4, [x26, x2]
	cmp	w4, 1
	bne	.L148
	strb	wzr, [x26, x2]
	strb	w4, [x26, x3]
	ldrsb	w2, [x7, x3]
	cmp	w2, w8
	add	w2, w2, 1
	bge	.L149
	strb	w2, [x7, x3]
	b	.L150
.L151:
	strb	wzr, [x7, x3]
	b	.L150
.L149:
	strb	w8, [x7, x3]
	b	.L150
.L218:
	mov	w3, 4
	mov	w0, w3
	b	.L120
.L296:
	ldrsb	w25, [x23]
	b	.L110
.L297:
	ldrsb	w0, [x23, 2]
	b	.L195
	.size	PQ_MNG_ALG_GetDbDetInfo, .-PQ_MNG_ALG_GetDbDetInfo
	.align	3
.LC6:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_DBCfgDetector
	.type	PQ_MNG_ALG_DBCfgDetector, %function
PQ_MNG_ALG_DBCfgDetector:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x1
	mov	x19, x0
	mov	x0, x30
	mov	x20, x2
	bl	_mcount
	ldp	w4, w5, [x21]
	str	w4, [x19, 668]
	ldr	w0, [x21, 4]
	str	w0, [x19, 672]
	ldr	w0, [x21, 48]
	ldp	w1, w2, [x21, 4]
	cmp	w0, 8
	beq	.L326
	mov	w0, 90
	strh	w0, [x19, 24]
.L300:
	ldr	w0, [x21, 64]
	cmp	w0, 8
	beq	.L327
	mov	w0, 60
	strh	w0, [x19, 26]
.L302:
	ldr	w0, [x21, 56]
	add	w2, w0, 1
	cmp	w2, 1
	bls	.L303
	cmp	w1, wzr
	add	w3, w1, 7
	sdiv	w0, w1, w0
	csel	w3, w3, w1, lt
	asr	w3, w3, 3
	add	w2, w1, 63
	and	w0, w0, 65535
	cmp	w0, w3
	csel	w0, w0, w3, le
	cmp	w1, wzr
	csel	w1, w2, w1, lt
	asr	w1, w1, 6
	cmp	w0, w1
	csel	w0, w0, w1, ge
	strh	w0, [x19, 28]
.L303:
	mov	w1, 1080
	cmp	w4, 1920
	mov	w0, 1
	strh	wzr, [x19, 20]
	strh	w0, [x19, 16]
	ccmp	w5, w1, 0, le
	strh	w0, [x19, 18]
	strh	w0, [x19, 22]
	bgt	.L304
	mov	w0, 6
	strh	w0, [x19, 30]
	ldp	w2, w1, [x21]
	ldr	w0, [x21, 8]
	cmp	w2, 720
	bgt	.L306
.L331:
	mov	w2, 16
	strh	w2, [x19, 32]
.L307:
	cmp	w1, 576
	bgt	.L309
.L332:
	mov	w1, 16
	strh	w1, [x19, 36]
.L310:
	cmp	w0, 360
	ble	.L328
.L312:
	cmp	w0, 960
	ble	.L329
	mov	w0, 63
	strh	w0, [x19, 34]
.L313:
	add	x19, x19, 40
	mov	x2, 512
	mov	w1, 0
	mov	x0, x19
	add	x20, x20, 16
	bl	memset
	mov	w0, 0
	mov	x4, x19
	mov	x5, 0
	.p2align 2
.L315:
	ldrb	w3, [x20, x5]
	mov	w2, 1
	add	w1, w5, 1
	and	w3, w3, w2
	orr	w3, w3, w0
	str	w3, [x4]
	b	.L318
	.p2align 3
.L316:
	ldrb	w0, [x20, w1, uxtw]
	add	w1, w1, 1
	and	w0, w0, 1
	lsl	w0, w0, w2
	add	w2, w2, 1
	orr	w3, w3, w0
	cmp	w2, 32
	str	w3, [x4]
	beq	.L330
.L318:
	cmp	w1, 4095
	bls	.L316
.L321:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L304:
	mov	w0, 8
	strh	wzr, [x19, 22]
	strh	w0, [x19, 30]
	ldp	w2, w1, [x21]
	ldr	w0, [x21, 8]
	cmp	w2, 720
	ble	.L331
.L306:
	cmp	w2, 1920
	bgt	.L308
	mov	w2, 42
	cmp	w1, 576
	strh	w2, [x19, 32]
	ble	.L332
.L309:
	cmp	w1, 1080
	bgt	.L311
	mov	w1, 36
	cmp	w0, 360
	strh	w1, [x19, 36]
	bgt	.L312
.L328:
	mov	w0, 16
	strh	w0, [x19, 34]
	b	.L313
.L308:
	mov	w2, 63
	strh	w2, [x19, 32]
	b	.L307
.L311:
	mov	w1, 63
	strh	w1, [x19, 36]
	b	.L310
.L329:
	mov	w0, 42
	strh	w0, [x19, 34]
	b	.L313
	.p2align 3
.L330:
	cmp	x5, 4064
	beq	.L321
	add	x5, x5, 32
	ldr	w0, [x4, 4]!
	cmp	x5, 4096
	bne	.L315
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L327:
	add	w0, w2, 7
	cmp	w2, wzr
	csel	w0, w0, w2, lt
	asr	w0, w0, 3
	strh	w0, [x19, 26]
	b	.L302
.L326:
	ldr	w3, [x21]
	add	w0, w3, 7
	cmp	w3, wzr
	csel	w0, w0, w3, lt
	asr	w0, w0, 3
	strh	w0, [x19, 24]
	b	.L300
	.size	PQ_MNG_ALG_DBCfgDetector, .-PQ_MNG_ALG_DBCfgDetector
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	stDetStaticInfo, %object
	.size	stDetStaticInfo, 20520
stDetStaticInfo:
	.zero	20520
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
