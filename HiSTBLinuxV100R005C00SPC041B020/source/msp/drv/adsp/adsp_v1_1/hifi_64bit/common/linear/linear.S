#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"linear.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	LSRC_Create
	.type	LSRC_Create, %function
LSRC_Create:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	w20, w1
	mov	w19, w2
	mov	w23, w3
	mov	w22, w4
	bl	_mcount
	str	xzr, [x21]
	mov	w0, 60927
	sub	w1, w20, #1
	movk	w0, 0x2, lsl 16
	cmp	w1, w0
	bhi	.L1
	sub	w1, w19, #1
	cmp	w1, w0
	bhi	.L1
	sdiv	w0, w19, w20
	cmp	w0, 6
	ble	.L21
.L1:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L21:
	mov	w1, 76
	mov	w0, 26
	bl	HI_ADSP_MALLOC
	mov	x24, x0
	cbz	x0, .L1
	ldr	x3, .LC2
	mov	x2, 76
	mov	w1, 0
	ldr	x3, [x3, 40]
	blr	x3
	mov	w0, 8000
	cmp	w19, w0
	beq	.L12
	ldr	x3, .LC3
	mov	w1, 11025
	mov	w0, 1
	add	x2, x3, 8
	b	.L8
	.p2align 3
.L23:
	ldr	w1, [x2], 4
.L8:
	cmp	w19, w1
	beq	.L22
	add	w0, w0, 1
	cmp	w0, 13
	bne	.L23
.L7:
	cbz	w22, .L11
	sub	w1, w22, #1
	add	x0, x24, 36
	add	x1, x1, 10
	add	x1, x24, x1, lsl 2
	.p2align 2
.L10:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L10
.L11:
	stp	w23, w22, [x24]
	stp	w20, w20, [x24, 8]
	str	w19, [x24, 16]
	str	x24, [x21]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L22:
	add	x3, x3, 56
	ldr	w0, [x3, w0, uxtw 2]
	str	w0, [x24, 32]
	b	.L7
.L12:
	mov	w0, 3145
	movk	w0, 0x2, lsl 16
	str	w0, [x24, 32]
	b	.L7
	.size	LSRC_Create, .-LSRC_Create
	.align	3
.LC2:
	.xword	g_AdspOsalFunc
	.align	3
.LC3:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_Destroy
	.type	LSRC_Destroy, %function
LSRC_Destroy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L24
	mov	x1, x19
	mov	w0, 26
	bl	HI_ADSP_FREE
.L24:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	LSRC_Destroy, .-LSRC_Destroy
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_GetMaxOutputNum
	.type	LSRC_GetMaxOutputNum, %function
LSRC_GetMaxOutputNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cbz	x20, .L31
	ldr	w1, [x20, 16]
	ldr	w0, [x20, 8]
	mul	w19, w19, w1
	sdiv	w0, w19, w0
	add	w0, w0, 1
.L30:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L31:
	mov	w0, -1
	b	.L30
	.size	LSRC_GetMaxOutputNum, .-LSRC_GetMaxOutputNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_GetMinInputNum
	.type	LSRC_GetMinInputNum, %function
LSRC_GetMinInputNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cbz	x20, .L34
	ldr	w1, [x20, 8]
	ldr	w2, [x20, 16]
	mul	w1, w19, w1
	sdiv	w0, w1, w2
	msub	w1, w0, w2, w1
	cmp	w1, wzr
	csinc	w0, w0, w0, eq
.L33:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L34:
	mov	w0, -1
	b	.L33
	.size	LSRC_GetMinInputNum, .-LSRC_GetMinInputNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_CheckUpdate
	.type	LSRC_CheckUpdate, %function
LSRC_CheckUpdate:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w1
	mov	w20, w2
	mov	w23, w3
	mov	w22, w4
	bl	_mcount
	ldp	w0, w4, [x19]
	ldp	w1, w2, [x19, 12]
	cmp	w0, w23
	ldr	x23, [sp, 48]
	cset	w0, ne
	cmp	w4, w22
	csinc	w0, w0, w0, eq
	cmp	w1, w21
	csinc	w0, w0, w0, eq
	cmp	w2, w20
	csinc	w0, w0, w0, eq
	cmp	w0, wzr
	cset	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	LSRC_CheckUpdate, .-LSRC_CheckUpdate
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_UpdateChange
	.type	LSRC_UpdateChange, %function
LSRC_UpdateChange:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w22, w1
	mov	w21, w2
	mov	w23, w3
	mov	w19, w4
	bl	_mcount
	ldp	w0, w3, [x20]
	ldp	w2, w1, [x20, 12]
	cmp	w0, w23
	cset	w0, ne
	cmp	w3, w19
	csinc	w0, w0, w0, eq
	cmp	w2, w22
	csinc	w0, w0, w0, eq
	cmp	w1, w21
	csinc	w0, w0, w0, eq
	cbz	w0, .L37
	mov	w0, 8000
	cmp	w21, w0
	stp	w23, w19, [x20]
	stp	w22, w22, [x20, 8]
	stp	w21, wzr, [x20, 16]
	stp	wzr, wzr, [x20, 24]
	beq	.L45
	ldr	x2, .LC5
	mov	w1, 11025
	mov	w0, 1
	add	x3, x2, 8
	b	.L42
	.p2align 3
.L54:
	ldr	w1, [x3], 4
.L42:
	cmp	w21, w1
	beq	.L53
	add	w0, w0, 1
	cmp	w0, 13
	bne	.L54
.L41:
	cbz	w19, .L37
	sub	w1, w19, #1
	add	x0, x20, 36
	add	x1, x1, 10
	add	x1, x20, x1, lsl 2
	.p2align 2
.L44:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L44
.L37:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L53:
	add	x2, x2, 56
	ldr	w0, [x2, w0, uxtw 2]
	str	w0, [x20, 32]
	b	.L41
.L45:
	mov	w0, 3145
	movk	w0, 0x2, lsl 16
	str	w0, [x20, 32]
	b	.L41
	.size	LSRC_UpdateChange, .-LSRC_UpdateChange
	.align	3
.LC5:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_Flush
	.type	LSRC_Flush, %function
LSRC_Flush:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w4, [x19, 16]
	mov	w0, 8000
	stp	wzr, wzr, [x19, 20]
	cmp	w4, w0
	beq	.L63
	ldr	x1, .LC7
	mov	w2, 11025
	mov	w0, 1
	add	x3, x1, 8
	b	.L59
	.p2align 3
.L69:
	ldr	w2, [x3], 4
.L59:
	cmp	w4, w2
	beq	.L68
	add	w0, w0, 1
	cmp	w0, 13
	bne	.L69
.L58:
	ldr	w0, [x19, 4]
	cbz	w0, .L55
	sub	w1, w0, #1
	add	x0, x19, 36
	add	x1, x1, 10
	add	x1, x19, x1, lsl 2
	.p2align 2
.L61:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L61
.L55:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L68:
	add	x1, x1, 56
	ldr	w0, [x1, w0, uxtw 2]
	str	w0, [x19, 32]
	b	.L58
.L63:
	mov	w0, 3145
	movk	w0, 0x2, lsl 16
	str	w0, [x19, 32]
	b	.L58
	.size	LSRC_Flush, .-LSRC_Flush
	.align	3
.LC7:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_CheckDoSRC
	.type	LSRC_CheckDoSRC, %function
LSRC_CheckDoSRC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 8]
	ldr	w0, [x19, 16]
	cmp	w1, w0
	beq	.L73
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L73:
	ldr	w0, [x19, 24]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	LSRC_CheckDoSRC, .-LSRC_CheckDoSRC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_SetSpeedAdjust
	.type	LSRC_SetSpeedAdjust, %function
LSRC_SetSpeedAdjust:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	w1, w19, 100
	ldr	w0, [x20, 28]
	cmp	w1, 200
	ccmp	w0, w19, 4, ls
	beq	.L74
	cmp	w19, wzr
	ccmp	w0, wzr, 0, ne
	bne	.L76
	mov	w1, 1
	str	w1, [x20, 24]
.L76:
	cmp	w19, wzr
	ccmp	w0, wzr, 4, eq
	beq	.L77
	mov	w0, 1
	str	w0, [x20, 24]
.L77:
	ldr	w0, [x20, 12]
	add	w1, w19, 1000
	mov	w2, 19923
	str	wzr, [x20, 20]
	movk	w2, 0x1062, lsl 16
	str	w19, [x20, 28]
	mul	w0, w1, w0
	smull	x1, w0, w2
	asr	x1, x1, 38
	sub	w0, w1, w0, asr 31
	str	w0, [x20, 8]
.L74:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	LSRC_SetSpeedAdjust, .-LSRC_SetSpeedAdjust
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LSRC_ProcessFrame
	.type	LSRC_ProcessFrame, %function
LSRC_ProcessFrame:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	mov	x23, x2
	mov	w22, w3
	bl	_mcount
	ldp	w0, w13, [x19]
	cmp	w0, 16
	ldr	w0, [x19, 24]
	beq	.L175
	cmp	w0, wzr
	ldr	w4, [x19, 8]
	ccmp	w22, wzr, 4, ne
	ldr	w5, [x19, 16]
	ble	.L116
	cmp	w4, w5
	beq	.L117
	cmp	w13, wzr
	add	x4, x19, 36
	mov	x0, 0
	ble	.L124
.L156:
	ldr	w6, [x21, x0, lsl 2]
	add	x0, x0, 1
	str	w6, [x4], 4
	cmp	w13, w0
	bgt	.L156
.L124:
	sub	w22, w22, #1
	mov	w0, 0
	mov	w16, 1
	str	wzr, [x19, 24]
.L119:
	cmp	w22, wzr
	ble	.L129
	ldr	w8, [x19, 20]
	cmp	w5, w8
	ble	.L130
	mul	w15, w13, w16
	mul	w12, w13, w0
	ldr	w5, [x19, 16]
	add	w14, w15, w13
	sub	w12, w12, w15
	mov	w6, w5
.L131:
	cmp	w13, wzr
	ble	.L134
	add	x11, x19, 36
	mov	w10, w15
	b	.L135
	.p2align 3
.L133:
	ldp	w5, w8, [x19, 16]
.L135:
	ldr	w4, [x19, 32]
	sub	w9, w5, w8
	ldr	w7, [x21, w10, sxtw 2]
	add	w17, w12, w10
	ldr	w5, [x11], 4
	add	w10, w10, 1
	mul	w6, w4, w8
	mul	w4, w9, w4
	smull	x6, w7, w6
	smull	x4, w5, w4
	asr	x5, x6, 32
	asr	x4, x4, 32
	add	w4, w5, w4
	asr	w6, w4, 31
	asr	w5, w4, 14
	cmp	w6, w4, asr 29
	eor	w6, w6, 32767
	csel	w5, w6, w5, ne
	cmp	w14, w10
	lsl	w5, w5, 16
	str	w5, [x23, w17, sxtw 2]
	bne	.L133
	ldp	w5, w8, [x19, 16]
	mov	w6, w5
.L134:
	ldr	w4, [x19, 8]
	add	w0, w0, 1
	add	w12, w12, w13
	add	w8, w8, w4
	str	w8, [x19, 20]
	cmp	w8, w6
	blt	.L131
.L130:
	sub	w4, w22, #1
	sub	w9, w8, w5
	cmp	w4, wzr
	str	w9, [x19, 20]
	ble	.L129
	add	w18, w16, 1
	mvn	w12, w16
	ldr	w6, [x19, 16]
	mov	w17, w18
	mul	w14, w13, w18
	add	w16, w22, w16
	mov	w7, w0
	sub	w4, w14, w13
	madd	w12, w12, w13, w4
.L142:
	cmp	w6, w9
	add	w11, w13, w14
	mov	w5, w6
	ble	.L139
	mul	w10, w13, w7
	sub	w15, w7, w0
	add	w11, w13, w14
	sub	w10, w10, w14
	.p2align 2
.L143:
	cmp	w13, wzr
	ble	.L140
	mov	w7, w14
	b	.L141
	.p2align 3
.L138:
	ldp	w5, w9, [x19, 16]
.L141:
	ldr	w4, [x19, 32]
	add	w20, w7, w12
	sub	w6, w5, w9
	ldr	w8, [x21, w7, sxtw 2]
	add	w30, w10, w7
	add	w7, w7, 1
	ldr	w5, [x21, w20, sxtw 2]
	mul	w9, w4, w9
	mul	w4, w6, w4
	smull	x6, w8, w9
	smull	x4, w5, w4
	asr	x5, x6, 32
	asr	x4, x4, 32
	add	w4, w5, w4
	asr	w6, w4, 31
	asr	w5, w4, 14
	cmp	w6, w4, asr 29
	eor	w6, w6, 32767
	csel	w5, w6, w5, ne
	cmp	w7, w11
	lsl	w5, w5, 16
	str	w5, [x23, w30, sxtw 2]
	bne	.L138
	ldp	w5, w9, [x19, 16]
	mov	w6, w5
.L140:
	ldr	w4, [x19, 8]
	add	w0, w0, 1
	add	w7, w15, w0
	add	w10, w10, w13
	add	w9, w9, w4
	str	w9, [x19, 20]
	cmp	w9, w6
	blt	.L143
.L139:
	add	w17, w17, 1
	sub	w9, w9, w6
	cmp	w16, w17
	str	w9, [x19, 20]
	mov	w14, w11
	bne	.L142
	sub	w16, w22, #2
	add	w16, w16, w18
.L129:
	cmp	w13, wzr
	ble	.L96
	mul	w16, w13, w16
	add	x19, x19, 36
	add	w13, w13, w16
.L144:
	ldr	w2, [x21, w16, sxtw 2]
	add	w16, w16, 1
	str	w2, [x19], 4
	cmp	w16, w13
	bne	.L144
.L96:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L116:
	mov	w0, 0
.L122:
	cmp	w4, w5
	mov	w16, 0
	bne	.L119
	cmp	w22, wzr
	ble	.L96
	mul	w6, w13, w0
	mov	w9, w22
	mov	w8, 0
.L127:
	cmp	w13, wzr
	ble	.L176
	mov	w4, w6
	add	w6, w6, w13
	sub	w7, w8, w4
	.p2align 2
.L126:
	add	w5, w7, w4
	ldr	w5, [x21, w5, sxtw 2]
	str	w5, [x23, w4, sxtw 2]
	add	w4, w4, 1
	cmp	w4, w6
	bne	.L126
	subs	w9, w9, #1
	add	w8, w8, w13
	bne	.L127
.L173:
	add	w0, w0, w22
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L176:
	subs	w9, w9, #1
	add	w6, w6, w13
	add	w8, w8, w13
	bne	.L127
	b	.L173
	.p2align 3
.L175:
	cmp	w0, wzr
	ldr	w5, [x19, 8]
	ccmp	w22, wzr, 4, ne
	ldr	w4, [x19, 16]
	ble	.L86
	cmp	w5, w4
	beq	.L87
	cmp	w13, wzr
	add	x5, x19, 36
	mov	x0, 0
	ble	.L94
.L155:
	ldrsh	w6, [x21, x0, lsl 1]
	add	x0, x0, 1
	str	w6, [x5], 4
	cmp	w13, w0
	bgt	.L155
.L94:
	sub	w22, w22, #1
	mov	w0, 0
	mov	w16, 1
	str	wzr, [x19, 24]
.L89:
	cmp	w22, wzr
	ble	.L100
	ldr	w7, [x19, 20]
	cmp	w4, w7
	ble	.L101
	mul	w15, w13, w16
	mul	w12, w13, w0
	ldr	w4, [x19, 16]
	add	w14, w15, w13
	sub	w12, w12, w15
	mov	w6, w4
.L102:
	cmp	w13, wzr
	ble	.L105
	add	x10, x19, 36
	mov	w9, w15
	b	.L106
	.p2align 3
.L104:
	ldp	w4, w7, [x19, 16]
.L106:
	ldr	w5, [x19, 32]
	sub	w8, w4, w7
	ldrsh	w6, [x21, w9, sxtw 1]
	add	w11, w12, w9
	ldr	w4, [x10], 4
	add	w9, w9, 1
	mul	w7, w5, w7
	mul	w5, w8, w5
	smull	x6, w6, w7
	smull	x4, w4, w5
	asr	x5, x6, 32
	asr	x4, x4, 32
	add	w4, w5, w4
	lsl	w4, w4, 2
	asr	w5, w4, 31
	cmp	w5, w4, asr 15
	eor	w5, w5, 32767
	csel	w4, w5, w4, ne
	cmp	w14, w9
	strh	w4, [x23, w11, sxtw 1]
	bne	.L104
	ldp	w4, w7, [x19, 16]
	mov	w6, w4
.L105:
	ldr	w5, [x19, 8]
	add	w0, w0, 1
	add	w12, w12, w13
	add	w7, w7, w5
	str	w7, [x19, 20]
	cmp	w7, w6
	blt	.L102
.L101:
	sub	w5, w22, #1
	sub	w8, w7, w4
	cmp	w5, wzr
	str	w8, [x19, 20]
	ble	.L100
	add	w18, w16, 1
	mvn	w11, w16
	ldr	w6, [x19, 16]
	mov	w17, w18
	mul	w14, w13, w18
	add	w16, w16, w22
	mov	w5, w0
	sub	w4, w14, w13
	madd	w11, w11, w13, w4
.L113:
	cmp	w6, w8
	add	w10, w13, w14
	mov	w4, w6
	ble	.L110
	mul	w9, w13, w5
	mov	w12, w0
	sub	w15, w5, w0
	sub	w9, w9, w14
	add	w10, w13, w14
	.p2align 2
.L114:
	cmp	w13, wzr
	ble	.L111
	mov	w6, w14
	b	.L112
	.p2align 3
.L109:
	ldp	w4, w8, [x19, 16]
.L112:
	ldr	w0, [x19, 32]
	add	w30, w6, w11
	sub	w5, w4, w8
	ldrsh	w7, [x21, w6, sxtw 1]
	add	w20, w9, w6
	add	w6, w6, 1
	ldrsh	w4, [x21, w30, sxtw 1]
	mul	w8, w0, w8
	mul	w0, w5, w0
	smull	x5, w7, w8
	smull	x0, w4, w0
	asr	x4, x5, 32
	asr	x0, x0, 32
	add	w0, w4, w0
	lsl	w0, w0, 2
	asr	w4, w0, 31
	cmp	w4, w0, asr 15
	eor	w4, w4, 32767
	csel	w0, w4, w0, ne
	cmp	w6, w10
	strh	w0, [x23, w20, sxtw 1]
	bne	.L109
	ldp	w4, w8, [x19, 16]
	mov	w6, w4
.L111:
	ldr	w0, [x19, 8]
	add	w12, w12, 1
	add	w5, w15, w12
	add	w9, w9, w13
	add	w8, w8, w0
	str	w8, [x19, 20]
	cmp	w8, w6
	blt	.L114
	mov	w0, w12
.L110:
	add	w17, w17, 1
	sub	w8, w8, w6
	cmp	w16, w17
	str	w8, [x19, 20]
	mov	w14, w10
	bne	.L113
	sub	w16, w22, #2
	add	w16, w16, w18
.L100:
	cmp	w13, wzr
	ble	.L96
	mul	w16, w13, w16
	add	x19, x19, 36
	add	w13, w13, w16
.L115:
	ldrsh	w2, [x21, w16, sxtw 1]
	add	w16, w16, 1
	str	w2, [x19], 4
	cmp	w16, w13
	bne	.L115
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L117:
	cmp	w13, wzr
	ble	.L148
	add	x4, x19, 36
	mov	x0, 0
.L121:
	ldr	w5, [x4], 4
	str	w5, [x23, x0, lsl 2]
	add	x0, x0, 1
	cmp	w13, w0
	bgt	.L121
	ldr	w4, [x19, 8]
	ldr	w5, [x19, 16]
.L120:
	mov	w0, 1
	str	wzr, [x19, 24]
	b	.L122
.L86:
	mov	w0, 0
.L92:
	cmp	w5, w4
	mov	w16, 0
	bne	.L89
	cmp	w22, wzr
	ble	.L96
	cmp	w13, wzr
	mov	w9, w22
	mul	w5, w13, w0
	mov	w8, 0
	ble	.L177
.L169:
	mov	w4, w5
	add	w5, w5, w13
	sub	w7, w8, w4
.L97:
	add	w6, w7, w4
	ldrsh	w6, [x21, w6, sxtw 1]
	strh	w6, [x23, w4, sxtw 1]
	add	w4, w4, 1
	cmp	w4, w5
	bne	.L97
	subs	w9, w9, #1
	add	w8, w8, w13
	beq	.L173
.L174:
	cmp	w13, wzr
	bgt	.L169
.L177:
	subs	w9, w9, #1
	add	w5, w5, w13
	add	w8, w8, w13
	bne	.L174
	b	.L173
.L148:
	mov	w5, w4
	b	.L120
.L87:
	cmp	w13, wzr
	ble	.L145
	add	x4, x19, 36
	mov	x0, 0
.L91:
	ldr	w5, [x4], 4
	strh	w5, [x23, x0, lsl 1]
	add	x0, x0, 1
	cmp	w13, w0
	bgt	.L91
	ldr	w5, [x19, 8]
	ldr	w4, [x19, 16]
	b	.L90
.L145:
	mov	w4, w5
.L90:
	mov	w0, 1
	str	wzr, [x19, 24]
	b	.L92
	.size	LSRC_ProcessFrame, .-LSRC_ProcessFrame
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	g_s32SampleRateTab, %object
	.size	g_s32SampleRateTab, 52
g_s32SampleRateTab:
	.word	8000
	.word	11025
	.word	12000
	.word	16000
	.word	22050
	.word	24000
	.word	32000
	.word	44100
	.word	48000
	.word	88200
	.word	96000
	.word	176400
	.word	192000
	.zero	4
	.type	g_s32IvtSfTab, %object
	.size	g_s32IvtSfTab, 52
g_s32IvtSfTab:
	.word	134217
	.word	97391
	.word	89478
	.word	67109
	.word	48696
	.word	44739
	.word	33554
	.word	24348
	.word	22369
	.word	12174
	.word	11185
	.word	6087
	.word	5592
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
