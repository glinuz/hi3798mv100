#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_avs.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	AVSHAL_V400R003_InitHal
	.type	AVSHAL_V400R003_InitHal, %function
AVSHAL_V400R003_InitHal:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	AVSHAL_V400R003_InitHal, .-AVSHAL_V400R003_InitHal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSHAL_V400R003_WirteSlicMsg
	.type	AVSHAL_V400R003_WirteSlicMsg, %function
AVSHAL_V400R003_WirteSlicMsg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w22, w2
	mov	w21, w1
	bl	_mcount
	add	x19, x20, 104
	sxtw	x0, w22
	ldr	x2, .LC0
	lsl	x1, x0, 6
	sub	x1, x1, x0
	lsl	x1, x1, 2
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	add	x0, x2, x0
	ldr	w23, [x0, 72]
	mov	w0, w23
	add	w22, w23, 320
	bl	MEM_Phy2Vir
	add	x1, x0, 320
	ldr	w2, [x19, 16]
	mov	w8, 0
	cbz	w2, .L3
	ldr	w2, [x20, 104]
	add	w2, w2, 4
	sub	w2, w2, w21
	ubfiz	w3, w2, 3, 4
	and	w2, w2, -16
	cmp	w3, 7
	bls	.L31
	sub	w3, w3, #8
.L5:
	lsl	w3, w3, 25
	and	w2, w2, 16777215
	orr	w3, w3, 8
	str	w3, [x0, 320]
	str	w2, [x0, 324]
	add	w23, w23, 576
	str	wzr, [x0, 328]
	mov	w8, 1
	str	wzr, [x0, 332]
	ldr	w2, [x19, 16]
	sub	w2, w2, #1
	lsl	w2, w2, 16
	str	w2, [x0, 336]
	str	w23, [x0, 572]
.L3:
	ldr	w5, [x20, 14440]
	cmp	w5, wzr
	ble	.L2
	add	w8, w22, w8, lsl 8
	mov	w2, 0
	mov	w7, 0
	mov	w0, 0
	.p2align 2
.L21:
	cmp	w7, 511
	bgt	.L7
	sxtw	x4, w0
	cmp	w0, wzr
	lsl	x3, x4, 3
	sub	x3, x3, x4
	add	x3, x19, x3, lsl 2
	ble	.L9
	ldr	w6, [x3, -12]
	ldr	w9, [x3, 16]
	cmp	w9, w6
	bls	.L10
.L9:
	ldp	w2, w6, [x3]
	ldr	w5, [x3, 8]
	add	w2, w2, 4
	sub	w2, w2, w21
	sub	w5, w5, #4
	and	w9, w2, -16
	lsl	w5, w5, 3
	ubfiz	w2, w2, 3, 4
	cbz	w6, .L29
	lsl	x10, x4, 3
	sub	x10, x10, x4
	add	x10, x20, x10, lsl 2
	ldr	w10, [x10, 116]
	cmp	w10, wzr
	ble	.L29
	ldr	w11, [x3, 12]
	sub	w6, w6, w21
	and	w12, w6, -16
	ubfiz	w10, w6, 3, 4
	lsl	w11, w11, 3
	cbz	w5, .L26
	cmp	w2, 7
	and	w11, w11, 33554431
	orr	w10, w11, w10, lsl 25
	and	w6, w6, 16777200
	bhi	.L12
	.p2align 2
.L34:
	sub	w9, w9, #4
	add	w2, w2, 120
	add	w5, w5, 8
.L13:
	lsl	x4, x4, 8
	and	w5, w5, 33554431
	add	x12, x4, 4
	add	x11, x4, 8
	orr	w2, w5, w2, lsl 25
	add	x5, x4, 12
	str	w2, [x1, x4]
	and	w9, w9, 16777215
	str	w9, [x1, x12]
	add	w2, w0, 1
	str	w10, [x1, x11]
	str	w6, [x1, x5]
	ldr	w6, [x20, 14440]
	cmp	w2, w6
	bge	.L32
	sxtw	x9, w2
	ldr	w5, [x3, 16]
	lsl	x0, x9, 3
	sub	x0, x0, x9
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 16]
	cmp	w5, w0
	bcs	.L17
	b	.L15
	.p2align 3
.L18:
	sub	x0, x0, x3
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 16]
	cmp	w0, w5
	bhi	.L15
.L17:
	add	w2, w2, 1
	cmp	w2, w6
	sxtw	x3, w2
	lsl	x0, x3, 3
	blt	.L18
.L15:
	cmp	w6, w2
	beq	.L33
	sxtw	x3, w2
	add	w6, w8, w2, lsl 8
	lsl	x0, x3, 3
	sub	x0, x0, x3
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 16]
	sub	w0, w0, #1
.L20:
	add	x3, x4, 16
	add	x4, x4, 252
	and	w5, w5, 65535
	add	w7, w7, 1
	orr	w5, w5, w0, lsl 16
	str	w5, [x1, x3]
	str	w6, [x1, x4]
	ldr	w5, [x20, 14440]
.L7:
	sub	w0, w2, #1
.L10:
	add	w0, w0, 1
	cmp	w5, w0
	bgt	.L21
.L2:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L26:
	mov	w9, w12
	mov	w5, w11
	mov	w2, w10
.L29:
	cmp	w2, 7
	mov	w6, 0
	mov	w10, 0
	bls	.L34
.L12:
	sub	w2, w2, #8
	add	w5, w5, 8
	b	.L13
	.p2align 3
.L33:
	ldrh	w0, [x20, 64]
	mov	w6, 0
	ldrh	w3, [x20, 66]
	mul	w0, w0, w3
	sub	w0, w0, #1
	b	.L20
.L31:
	sub	w2, w2, #4
	add	w3, w3, 120
	b	.L5
.L32:
	ldr	w5, [x3, 16]
	b	.L15
	.size	AVSHAL_V400R003_WirteSlicMsg, .-AVSHAL_V400R003_WirteSlicMsg
	.align	3
.LC0:
	.xword	g_HwMem
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AVSHAL_V400R003_StartDec
	.type	AVSHAL_V400R003_StartDec, %function
AVSHAL_V400R003_StartDec:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	x21, x2
	bl	_mcount
	ldr	x22, .LC4
	ldr	x2, .LC3
	sbfiz	x0, x20, 6, 32
	cmp	x21, xzr
	ldrb	w1, [x22]
	add	x0, x2, x0
	ccmp	w1, 1, 4, eq
	ldr	w23, [x0, 8]
	bne	.L82
	cmp	w20, wzr
	bgt	.L83
	ldrh	w0, [x19, 64]
	cmp	w0, 512
	bhi	.L40
	ldrh	w0, [x19, 66]
	cmp	w0, 512
	bhi	.L40
	cbnz	w20, .L76
	ldr	x20, .LC14
	ldr	x0, [x20]
	cbz	x0, .L84
.L44:
	ldr	x0, .LC19
	ldr	x0, [x0, w23, sxtw 3]
	cbz	x0, .L65
	ldr	w23, [x0, 1296]
.L46:
	cmp	w1, 1
	beq	.L47
	strb	w23, [x21, 1]
.L47:
	ldr	w0, [x19, 14440]
	cmp	w0, wzr
	ble	.L49
	sub	w1, w0, #1
	mov	w3, 28
	mov	x2, 132
	add	x0, x19, 104
	mov	w22, -1
	umaddl	x2, w1, w3, x2
	add	x2, x19, x2
	.p2align 2
.L51:
	ldr	w1, [x0]
	cbz	w1, .L53
	ldr	w3, [x0, 8]
	and	w1, w1, -16
	cmp	w3, wzr
	ble	.L53
	cmp	w22, w1
	csel	w22, w22, w1, ls
.L53:
	ldr	w1, [x0, 4]
	cbz	w1, .L50
	ldr	w3, [x0, 12]
	and	w1, w1, -16
	cmp	w3, wzr
	ble	.L50
	cmp	w22, w1
	csel	w22, w22, w1, ls
.L50:
	add	x0, x0, 28
	cmp	x2, x0
	bne	.L51
	cmn	w22, #1
	beq	.L49
	ldrh	w4, [x19, 66]
	mov	w0, 0
	ldrh	w1, [x19, 64]
	mov	w2, 65
	mov	x3, x21
	mul	w1, w1, w4
	sub	w1, w1, #1
	bfi	w0, w1, 0, 20
	str	w0, [x29, 76]
	lsr	w0, w0, 16
	strb	w2, [x29, 79]
	orr	w0, w0, 64
	strb	w0, [x29, 78]
	mov	w2, 0
	mov	w0, 8
	ldr	w24, [x29, 76]
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC23
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	mov	w0, 1
	mov	w2, 12288
	ldrb	w3, [x19, 16]
	bfi	w2, w0, 0, 12
	ldr	w4, [x19, 14896]
	mov	w1, 0
	cmp	w3, w0
	uxth	w0, w2
	lsr	w0, w0, 8
	ldr	w5, [x19, 14900]
	bfi	w0, w23, 4, 1
	ccmp	w3, 3, 4, hi
	and	w0, w0, -33
	bfi	w1, w5, 4, 1
	bfi	w0, w4, 6, 1
	cset	w3, eq
	strh	w2, [x29, 78]
	and	w0, w0, 127
	orr	w1, w1, 64
	strb	w0, [x29, 79]
	bfi	w1, w3, 7, 1
	mov	w0, 6
	strb	w1, [x29, 77]
	mov	x3, x21
	strb	w0, [x29, 76]
	mov	w2, 0
	mov	w0, 12
	ldr	w24, [x29, 76]
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC25
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w24, [x20, 72]
	mov	x3, x21
	mov	w0, 16
	and	w24, w24, -16
	mov	w2, 0
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC27
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w24, [x20, 56]
	mov	x3, x21
	mov	w0, 20
	and	w24, w24, -16
	mov	w2, 0
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC29
	mov	w2, w24
	mov	w0, 3
	mov	w24, 3075
	movk	w24, 0x30, lsl 16
	bl	dprint_vfmw
	str	w22, [x29, 76]
	mov	x3, x21
	mov	w1, w22
	mov	w0, 24
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC31
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldrh	w0, [x19, 64]
	mov	x3, x21
	mov	w2, 0
	cmp	w0, 120
	mov	w0, 4
	cset	w1, ls
	bl	SCD_ConfigReg
	str	w24, [x29, 76]
	mov	w2, 0
	mov	w1, w24
	mov	w0, 60
	mov	x3, x21
	bl	MFDE_ConfigReg
	mov	x3, x21
	mov	w2, 0
	mov	w1, w24
	mov	w0, 64
	bl	MFDE_ConfigReg
	mov	x3, x21
	mov	w2, 0
	mov	w1, w24
	mov	w0, 68
	bl	MFDE_ConfigReg
	mov	x3, x21
	mov	w2, 0
	mov	w1, w24
	mov	w0, 72
	bl	MFDE_ConfigReg
	mov	x3, x21
	mov	w2, 0
	mov	w1, w24
	mov	w0, 76
	bl	MFDE_ConfigReg
	mov	x3, x21
	mov	w2, 0
	mov	w1, w24
	mov	w0, 80
	bl	MFDE_ConfigReg
	mov	x3, x21
	mov	w2, 0
	mov	w1, w24
	mov	w0, 84
	bl	MFDE_ConfigReg
	cmp	w23, 1
	beq	.L85
.L57:
	ldr	w23, [x19, 14456]
	mov	x3, x21
	mov	w0, 96
	mov	w2, 0
	and	w23, w23, -16
	mov	w24, -1
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC37
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x19, 14860]
	mov	x3, x21
	mov	w0, 100
	mov	w1, w23
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC39
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x19, 14868]
	mov	x3, x21
	mov	w0, 104
	mov	w1, w23
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC41
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x19, 14876]
	mov	x3, x21
	mov	w0, 108
	mov	w1, w23
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC43
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x20, 1220]
	mov	x3, x21
	mov	w0, 144
	mov	w1, w23
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC45
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x21
	mov	w2, 0
	mov	w1, 0
	mov	w0, 148
	bl	MFDE_ConfigReg
	ldr	x1, .LC47
	mov	w2, 0
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x21
	mov	w2, 0
	mov	w1, 0
	mov	w0, 152
	bl	MFDE_ConfigReg
	ldr	x1, .LC49
	mov	w2, 0
	mov	w0, 3
	bl	dprint_vfmw
	str	w24, [x29, 76]
	mov	x3, x21
	mov	w2, 0
	mov	w1, w24
	mov	w0, 32
	bl	MFDE_ConfigReg
	ldr	w0, [x20, 72]
	bl	MEM_Phy2Vir
	mov	x23, x0
	cbz	x0, .L86
	ldrh	w0, [x19, 66]
	add	x21, x19, 12288
	ldrh	w2, [x29, 78]
	sub	w0, w0, #1
	ldrb	w5, [x19, 40]
	ldrh	w3, [x19, 64]
	bfi	w2, w0, 0, 9
	ldrb	w4, [x19, 39]
	lsr	x0, x2, 8
	ldrh	w1, [x29, 76]
	sub	w3, w3, #1
	bfi	w0, w5, 1, 2
	strh	w2, [x29, 78]
	bfi	w1, w3, 0, 9
	bfi	w0, w4, 3, 2
	strh	w1, [x29, 76]
	and	w0, w0, 31
	ubfx	x1, x1, 8, 1
	strb	w0, [x29, 79]
	strb	w1, [x29, 77]
	mov	w0, 4
	ldr	x1, .LC53
	ldr	w2, [x29, 76]
	str	w2, [x23]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w1, [x19, 25]
	mov	w0, 0
	ldrb	w2, [x19, 17]
	ldrb	w3, [x19, 16]
	bfi	w0, w1, 0, 1
	ldrb	w4, [x19, 24]
	bfi	w0, w2, 1, 2
	ldrb	w1, [x19, 19]
	bfi	w0, w3, 3, 2
	ldrb	w5, [x19, 18]
	mov	w2, 0
	ldrb	w3, [x19, 23]
	bfi	w2, w1, 0, 1
	ldrb	w6, [x19, 21]
	bfi	w0, w4, 5, 1
	bfi	w2, w5, 1, 1
	bfi	w0, w3, 6, 1
	strb	w2, [x29, 78]
	mov	w1, 0
	ldrb	w3, [x19, 20]
	bfi	w1, w6, 0, 6
	ldrb	w4, [x19, 22]
	ldrh	w2, [x29, 78]
	bfi	w1, w3, 6, 1
	ldrb	w3, [x19, 31]
	bfi	w0, w4, 7, 1
	strb	w0, [x29, 76]
	and	w0, w2, 3
	bfi	w1, w3, 7, 1
	strh	w0, [x29, 78]
	strb	w1, [x29, 77]
	mov	w0, 4
	ldr	x1, .LC55
	ldr	w2, [x29, 76]
	str	w2, [x23, 4]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w2, [x19, 29]
	mov	w0, 0
	ldrb	w1, [x19, 28]
	bfi	w0, w2, 0, 1
	ldrb	w2, [x19, 27]
	bfi	w0, w1, 1, 1
	ldrb	w3, [x19, 26]
	bfi	w0, w2, 2, 5
	strb	w0, [x29, 76]
	ldr	x1, .LC57
	mov	w0, 4
	ldrh	w2, [x29, 76]
	bfi	w2, w3, 7, 5
	strh	w2, [x29, 76]
	ldr	w2, [x29, 76]
	str	w2, [x23, 8]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w0, [x19, 34]
	mov	w2, 0
	ldrb	w3, [x19, 35]
	ldrb	w1, [x19, 36]
	bfi	w2, w0, 0, 1
	bfi	w2, w3, 1, 2
	ldrb	w0, [x19, 37]
	ldrb	w4, [x19, 32]
	bfi	w2, w1, 3, 1
	bfi	w2, w0, 4, 2
	ldrb	w3, [x19, 33]
	bfi	w2, w4, 6, 1
	strb	w2, [x29, 76]
	ldr	x1, .LC59
	mov	w0, 4
	ldrh	w2, [x29, 76]
	bfi	w2, w3, 7, 2
	strh	w2, [x29, 76]
	ldr	w3, [x29, 76]
	and	w3, w3, 511
	str	w3, [x29, 76]
	mov	w2, w3
	str	w3, [x23, 12]
	bl	dprint_vfmw
	ldr	w2, [x19, 14444]
	mov	w0, 4
	ldr	x1, .LC61
	and	w2, w2, -16
	str	w2, [x23, 16]
	bl	dprint_vfmw
	ldr	w2, [x19, 14448]
	mov	w0, 4
	ldr	x1, .LC63
	and	w2, w2, -16
	str	w2, [x23, 20]
	bl	dprint_vfmw
	ldr	w2, [x19, 14452]
	mov	w0, 4
	ldr	x1, .LC65
	and	w2, w2, -16
	str	w2, [x23, 24]
	bl	dprint_vfmw
	ldr	w2, [x19, 14468]
	mov	w0, 4
	ldr	x1, .LC67
	and	w2, w2, -16
	str	w2, [x23, 28]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w3, [x19, 38]
	mov	w1, 0
	ldrb	w2, [x19, 30]
	mov	w0, 0
	bfi	w1, w3, 0, 1
	strb	w1, [x29, 76]
	bfi	w0, w2, 0, 2
	strb	w0, [x29, 79]
	ldr	x1, .LC69
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x23, 32]
	bl	dprint_vfmw
	ldr	w2, [x19, 80]
	mov	w0, 0
	ldr	x1, .LC71
	bfi	w0, w2, 0, 24
	str	w0, [x29, 76]
	mov	w2, w0
	str	w0, [x23, 36]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w0, [x19, 96]
	mov	w1, 0
	ldr	w2, [x19, 88]
	bfi	w1, w0, 0, 24
	mov	w0, 0
	str	w1, [x29, 76]
	bfi	w0, w2, 0, 7
	strb	w0, [x29, 79]
	mov	w0, 4
	ldr	x1, .LC73
	ldr	w2, [x29, 76]
	str	w2, [x23, 40]
	bl	dprint_vfmw
	ldr	w2, [x19, 84]
	mov	w0, 0
	ldr	x1, .LC75
	bfi	w0, w2, 0, 24
	str	w0, [x29, 76]
	mov	w2, w0
	str	w0, [x23, 44]
	mov	w0, 4
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w2, [x19, 41]
	mov	w1, 0
	ldrb	w4, [x19, 43]
	mov	w0, 0
	ldrb	w3, [x19, 42]
	bfi	w1, w2, 0, 1
	ldrb	w2, [x19, 45]
	bfi	w0, w4, 0, 6
	ldrb	w5, [x19, 44]
	bfi	w1, w3, 1, 1
	ldrb	w4, [x19, 46]
	bfi	w0, w2, 6, 1
	ldrb	w3, [x19, 47]
	mov	w2, 0
	bfi	w1, w5, 2, 6
	bfi	w0, w4, 7, 1
	bfi	w2, w3, 0, 1
	strb	w1, [x29, 76]
	strb	w0, [x29, 77]
	mov	w0, 4
	strb	w2, [x29, 78]
	ldr	x1, .LC77
	ldr	w2, [x29, 76]
	str	w2, [x23, 48]
	bl	dprint_vfmw
	ldr	w2, [x20, 1164]
	mov	w0, 4
	ldr	x1, .LC79
	and	w2, w2, -16
	str	w2, [x23, 52]
	bl	dprint_vfmw
	ldr	w2, [x20, 1168]
	mov	w0, 4
	ldr	x1, .LC81
	and	w2, w2, -16
	str	w2, [x23, 56]
	bl	dprint_vfmw
	ldr	w2, [x19, 14464]
	mov	w0, 4
	ldr	x1, .LC83
	and	w2, w2, -16
	str	w2, [x23, 64]
	bl	dprint_vfmw
	add	x0, x21, 2188
	add	x1, x23, 68
	add	x3, x21, 2316
	.p2align 2
.L59:
	ldr	w2, [x0], 4
	str	w2, [x1], 4
	cmp	x3, x0
	bne	.L59
	ldr	w2, [x19, 14460]
	mov	w0, 4
	ldr	x1, .LC85
	and	w2, w2, -16
	str	w2, [x23, 196]
	bl	dprint_vfmw
	ldr	w2, [x20, 1176]
	mov	w0, 4
	ldr	x1, .LC87
	and	w2, w2, -16
	str	w2, [x23, 200]
	bl	dprint_vfmw
	ldr	w2, [x20, 1160]
	mov	w0, 4
	ldr	x1, .LC89
	and	w2, w2, -16
	str	w2, [x23, 204]
	bl	dprint_vfmw
	ldr	w3, [x20, 72]
	mov	w0, 4
	ldr	x1, .LC91
	add	w3, w3, 320
	str	w3, [x23, 252]
	mov	w2, w3
	str	w3, [x29, 76]
	bl	dprint_vfmw
	ldrb	w3, [x19, 41]
	cmp	w3, 1
	beq	.L60
.L64:
	mov	x0, x19
	mov	w2, 0
	mov	w1, w22
	bl	AVSHAL_V400R003_WirteSlicMsg
	mov	w0, 0
.L37:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L60:
	add	x4, x23, 256
	add	x0, x21, 2340
	add	x23, x23, 320
	tbz	x3, 0, .L61
	.p2align 2
.L87:
	ldrb	w6, [x0]
	ldrb	w5, [x0, -8]
	ldrb	w2, [x0, -16]
	ldrb	w1, [x0, -24]
	lsl	w5, w5, 16
	orr	w5, w5, w6, lsl 24
	orr	w1, w1, w2, lsl 8
	orr	w1, w5, w1
.L62:
	str	w1, [x4], 4
	add	w3, w3, 1
	add	x0, x0, 16
	cmp	x4, x23
	beq	.L64
	tbnz	x3, 0, .L87
.L61:
	ldrb	w6, [x0, -12]
	ldrb	w5, [x0, -20]
	ldrb	w2, [x0, -28]
	ldrb	w1, [x0, -36]
	lsl	w5, w5, 16
	orr	w5, w5, w6, lsl 24
	orr	w1, w1, w2, lsl 8
	orr	w1, w5, w1
	b	.L62
.L65:
	mov	w23, 0
	b	.L46
.L85:
	mov	x3, x21
	mov	w2, 0
	mov	w1, 60
	mov	w0, 92
	bl	MFDE_ConfigReg
	ldr	x1, .LC33
	mov	w2, 60
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x19, 14864]
	mov	w0, 112
	mov	x3, x21
	mov	w1, w23
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC35
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	b	.L57
.L84:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L45
	ldrb	w1, [x22]
	str	x0, [x20]
	b	.L44
.L83:
	ldr	x2, .LC5
	mov	w4, 0
	ldr	x1, .LC9
	mov	w3, w20
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L37
.L40:
	ldr	x3, .LC11
.L78:
	ldr	x2, .LC5
	mov	w0, 0
	ldr	x1, .LC13
	bl	dprint_vfmw
	mov	w0, -1
	b	.L37
.L49:
	ldr	x1, .LC21
	mov	w2, -1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L37
.L76:
	ldr	x1, .LC16
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L37
.L86:
	ldr	x3, .LC51
	b	.L78
.L82:
	ldr	x2, .LC5
	mov	x3, x21
	ldr	x1, .LC7
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L37
.L45:
	ldr	x1, .LC18
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L37
	.size	AVSHAL_V400R003_StartDec, .-AVSHAL_V400R003_StartDec
	.align	3
.LC3:
	.xword	g_VdmDrvParam
	.align	3
.LC4:
	.xword	g_HalDisable
	.align	3
.LC5:
	.xword	.LANCHOR0
	.align	3
.LC7:
	.xword	.LC6
	.align	3
.LC9:
	.xword	.LC8
	.align	3
.LC11:
	.xword	.LC10
	.align	3
.LC13:
	.xword	.LC12
	.align	3
.LC14:
	.xword	g_HwMem
	.align	3
.LC16:
	.xword	.LC15
	.align	3
.LC18:
	.xword	.LC17
	.align	3
.LC19:
	.xword	s_pstVfmwChan
	.align	3
.LC21:
	.xword	.LC20
	.align	3
.LC23:
	.xword	.LC22
	.align	3
.LC25:
	.xword	.LC24
	.align	3
.LC27:
	.xword	.LC26
	.align	3
.LC29:
	.xword	.LC28
	.align	3
.LC31:
	.xword	.LC30
	.align	3
.LC33:
	.xword	.LC32
	.align	3
.LC35:
	.xword	.LC34
	.align	3
.LC37:
	.xword	.LC36
	.align	3
.LC39:
	.xword	.LC38
	.align	3
.LC41:
	.xword	.LC40
	.align	3
.LC43:
	.xword	.LC42
	.align	3
.LC45:
	.xword	.LC44
	.align	3
.LC47:
	.xword	.LC46
	.align	3
.LC49:
	.xword	.LC48
	.align	3
.LC51:
	.xword	.LC50
	.align	3
.LC53:
	.xword	.LC52
	.align	3
.LC55:
	.xword	.LC54
	.align	3
.LC57:
	.xword	.LC56
	.align	3
.LC59:
	.xword	.LC58
	.align	3
.LC61:
	.xword	.LC60
	.align	3
.LC63:
	.xword	.LC62
	.align	3
.LC65:
	.xword	.LC64
	.align	3
.LC67:
	.xword	.LC66
	.align	3
.LC69:
	.xword	.LC68
	.align	3
.LC71:
	.xword	.LC70
	.align	3
.LC73:
	.xword	.LC72
	.align	3
.LC75:
	.xword	.LC74
	.align	3
.LC77:
	.xword	.LC76
	.align	3
.LC79:
	.xword	.LC78
	.align	3
.LC81:
	.xword	.LC80
	.align	3
.LC83:
	.xword	.LC82
	.align	3
.LC85:
	.xword	.LC84
	.align	3
.LC87:
	.xword	.LC86
	.align	3
.LC89:
	.xword	.LC88
	.align	3
.LC91:
	.xword	.LC90
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.12039, %object
	.size	__func__.12039, 25
__func__.12039:
	.string	"AVSHAL_V400R003_StartDec"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC6:
	ASCII(.string	"%s: pMfdeTask(%p) = NULL\n" )
	.zero	6
.LC8:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC10:
	ASCII(.string	"picture width out of range" )
	.zero	5
.LC12:
	ASCII(.string	"%s: %s\n" )
.LC15:
	ASCII(.string	"VdhId is wrong! AVSHAL_V200R003_StartDec\n" )
	.zero	6
.LC17:
	ASCII(.string	"vdm register virtual address not mapped, reset failed!\n" )
.LC20:
	ASCII(.string	"stream_base_addr = %#x\n" )
.LC22:
	ASCII(.string	"BASIC_V400R003_CFG0 = 0x%x\n" )
	.zero	4
.LC24:
	ASCII(.string	"BASIC_V400R003_CFG1 = 0x%x\n" )
	.zero	4
.LC26:
	ASCII(.string	"AVM_V400R003_ADDR = 0x%x\n" )
	.zero	6
.LC28:
	ASCII(.string	"VAM_V400R003_ADDR = 0x%x\n" )
	.zero	6
.LC30:
	ASCII(.string	"STREAM_V400R003_BASE_ADDR = 0x%x\n" )
	.zero	6
.LC32:
	ASCII(.string	"VREG_V200R003_PART_DEC_OVER_INT_LEVEL=0x%x\n" )
	.zero	4
.LC34:
	ASCII(.string	"VREG_LINE_NUM_STADDR = 0x%x\n" )
	.zero	3
.LC36:
	ASCII(.string	"YSTADDR_V400R003_1D = 0x%x\n" )
	.zero	4
.LC38:
	ASCII(.string	"YSTRIDE_V400R003_1D = 0x%x\n" )
	.zero	4
.LC40:
	ASCII(.string	"UVOFFSET_V400R003_1D = 0x%x\n" )
	.zero	3
.LC42:
	ASCII(.string	"HEAD_INF_OFFSET = 0x%x\n" )
.LC44:
	ASCII(.string	"VREG_V400R003_DNR_MBINFO_STADDR = 0x%x\n" )
.LC46:
	ASCII(.string	"VREG_V200R003_REF_PIC_TYPE = 0x%x\n" )
	.zero	5
.LC48:
	ASCII(.string	"VREG_V400R003_FF_APT_EN = 0x%x\n" )
.LC50:
	ASCII(.string	"can not map down msg virtual address!" )
	.zero	2
.LC52:
	ASCII(.string	"D0 = 0x%x\n" )
	.zero	5
.LC54:
	ASCII(.string	"D1 = 0x%x\n" )
	.zero	5
.LC56:
	ASCII(.string	"D2 = 0x%x\n" )
	.zero	5
.LC58:
	ASCII(.string	"D3 = 0x%x\n" )
	.zero	5
.LC60:
	ASCII(.string	"D4 = 0x%x\n" )
	.zero	5
.LC62:
	ASCII(.string	"D5 = 0x%x\n" )
	.zero	5
.LC64:
	ASCII(.string	"D6 = 0x%x\n" )
	.zero	5
.LC66:
	ASCII(.string	"D7 = 0x%x\n" )
	.zero	5
.LC68:
	ASCII(.string	"D8 = 0x%x\n" )
	.zero	5
.LC70:
	ASCII(.string	"D9 = 0x%x\n" )
	.zero	5
.LC72:
	ASCII(.string	"D10 = 0x%x\n" )
	.zero	4
.LC74:
	ASCII(.string	"D11 = 0x%x\n" )
	.zero	4
.LC76:
	ASCII(.string	"D12 = 0x%x\n" )
	.zero	4
.LC78:
	ASCII(.string	"D13 = 0x%x\n" )
	.zero	4
.LC80:
	ASCII(.string	"D14 = 0x%x\n" )
	.zero	4
.LC82:
	ASCII(.string	"D16 = 0x%x\n" )
	.zero	4
.LC84:
	ASCII(.string	"D49 = 0x%x\n" )
	.zero	4
.LC86:
	ASCII(.string	"D50 = 0x%x\n" )
	.zero	4
.LC88:
	ASCII(.string	"D51 = 0x%x\n" )
	.zero	4
.LC90:
	ASCII(.string	"D63 = 0x%x\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
