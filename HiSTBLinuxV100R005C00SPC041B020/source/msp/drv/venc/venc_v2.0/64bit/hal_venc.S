#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"hal_venc.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_ClrAllInt
	.type	VENC_HAL_ClrAllInt, %function
VENC_HAL_ClrAllInt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w0, -1
	str	w0, [x19, 12]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VENC_HAL_ClrAllInt, .-VENC_HAL_ClrAllInt
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_DisableAllInt
	.type	VENC_HAL_DisableAllInt, %function
VENC_HAL_DisableAllInt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	str	wzr, [x19, 4]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VENC_HAL_DisableAllInt, .-VENC_HAL_DisableAllInt
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_ReadReg_Smmu
	.type	VENC_HAL_ReadReg_Smmu, %function
VENC_HAL_ReadReg_Smmu:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 24]
	mov	w2, -1
	add	x0, x0, 61440
	ldr	w1, [x0, 40]
	and	w1, w1, 1
	str	w1, [x19, 2684]
	ldr	w1, [x0, 40]
	ubfx	x1, x1, 1, 1
	str	w1, [x19, 2688]
	ldr	w1, [x0, 40]
	ubfx	x1, x1, 2, 1
	str	w1, [x19, 2692]
	ldr	w1, [x0, 40]
	ubfx	x1, x1, 3, 1
	str	w1, [x19, 2696]
	ldr	w1, [x0, 24]
	and	w1, w1, 1
	str	w1, [x19, 2668]
	ldr	w1, [x0, 24]
	ubfx	x1, x1, 1, 1
	str	w1, [x19, 2672]
	ldr	w1, [x0, 24]
	ubfx	x1, x1, 2, 1
	str	w1, [x19, 2676]
	ldr	w1, [x0, 24]
	ubfx	x1, x1, 3, 1
	str	w1, [x19, 2680]
	str	w2, [x0, 28]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VENC_HAL_ReadReg_Smmu, .-VENC_HAL_ReadReg_Smmu
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_CfgSmmuReg
	.type	VENC_HAL_CfgSmmuReg, %function
VENC_HAL_CfgSmmuReg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 24]
	mov	w0, 0
	ldr	w2, [x19, 2640]
	add	x1, x1, 61440
	str	w2, [x1, 520]
	ldr	w2, [x19, 2640]
	str	w2, [x1, 524]
	ldr	w2, [x19, 2648]
	str	w2, [x1, 752]
	ldr	w2, [x19, 2644]
	str	w2, [x1, 756]
	ldr	w2, [x19, 2648]
	str	w2, [x1, 772]
	ldr	w2, [x19, 2644]
	str	w2, [x1, 776]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VENC_HAL_CfgSmmuReg, .-VENC_HAL_CfgSmmuReg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_ReadReg
	.type	VENC_HAL_ReadReg, %function
VENC_HAL_ReadReg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	x21, x2
	bl	_mcount
	ldr	x0, [x19, 24]
	ldr	w1, [x0]
	and	w1, w1, 1
	str	w1, [x19, 2652]
	ldr	w1, [x0]
	ubfx	x1, x1, 1, 1
	str	w1, [x19, 2656]
	ldr	w1, [x0]
	ubfx	x1, x1, 3, 1
	str	w1, [x19, 2660]
	ldr	x1, [x19, 2656]
	cbnz	x1, .L5
	ldr	w1, [x0, 5740]
	ldr	w2, [x19, 32]
	ubfx	x1, x1, 0, 26
	str	w1, [x19, 2664]
	cbz	w2, .L13
	ldr	w1, [x0, 7808]
	lsl	w1, w1, 3
	str	w1, [x20, 24]
	ldr	w2, [x0, 5636]
	ldr	w3, [x0, 5668]
	ubfx	x2, x2, 0, 19
	ubfx	x0, x3, 0, 17
	add	w0, w2, w0
	lsr	w0, w0, 2
	str	w0, [x20, 108]
	ldr	w0, [x21, 104]
	add	w0, w1, w0
	str	w0, [x21, 104]
	ldr	w1, [x20, 24]
	cmp	w0, w1
	bhi	.L9
.L15:
	mov	w0, 1
	stp	w1, w0, [x21, 104]
	ldr	w0, [x19, 32]
	ldr	w1, [x20, 12]
	cbz	w0, .L14
.L11:
	ldr	w0, [x20, 16]
	add	w1, w1, 7
	lsr	w1, w1, 3
	ldr	w2, [x19, 2664]
	add	w0, w0, 7
	lsr	w0, w0, 3
	mul	w0, w1, w0
	udiv	w0, w2, w0
	str	w0, [x19, 2664]
.L5:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L13:
	ldr	w1, [x0, 8324]
	lsl	w1, w1, 3
	str	w1, [x20, 24]
	ldr	w4, [x0, 5640]
	ldr	w2, [x0, 5644]
	ldr	w3, [x0, 5672]
	ubfx	x4, x4, 0, 19
	ubfx	x0, x2, 0, 19
	ubfx	x3, x3, 0, 19
	add	w0, w4, w0
	add	w0, w0, w3
	lsr	w0, w0, 2
	str	w0, [x20, 108]
	ldr	w0, [x21, 104]
	add	w0, w1, w0
	str	w0, [x21, 104]
	ldr	w1, [x20, 24]
	cmp	w0, w1
	bls	.L15
.L9:
	ldr	w0, [x21, 108]
	add	w0, w0, 1
	str	w0, [x21, 108]
	ldr	w0, [x19, 32]
	ldr	w1, [x20, 12]
	cbnz	w0, .L11
.L14:
	ldr	w0, [x20, 16]
	add	w1, w1, 15
	lsr	w1, w1, 4
	ldr	w2, [x19, 2664]
	add	w0, w0, 15
	lsr	w0, w0, 4
	mul	w0, w1, w0
	udiv	w0, w2, w0
	str	w0, [x19, 2664]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VENC_HAL_ReadReg, .-VENC_HAL_ReadReg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_VEDU_Cfg_CABAC_VLC
	.type	VENC_HAL_VEDU_Cfg_CABAC_VLC, %function
VENC_HAL_VEDU_Cfg_CABAC_VLC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x20, 44]
	mov	w4, 9729
	ldr	w3, [x19, 2356]
	mov	w2, 513
	ldr	x0, [x19, 24]
	cmp	w1, wzr
	mov	w1, 0
	csel	w2, w2, w4, eq
	bfi	w1, w3, 8, 3
	mov	w3, 0
	bfi	w1, w2, 16, 16
	str	w1, [x0, 7680]
	mov	w2, 0
	mov	w1, 16
	ldr	w4, [x19, 260]
	mov	w5, 0
	ldr	w6, [x19, 268]
	bfi	w3, w4, 0, 5
	mov	w4, 0
	bfi	w3, w6, 16, 16
	str	w3, [x0, 7684]
	ldr	w6, [x19, 264]
	ldr	w3, [x19, 272]
	bfi	w2, w6, 0, 5
	bfi	w2, w3, 16, 16
	str	w2, [x0, 7724]
	ldr	w2, [x19, 260]
	ldr	w3, [x19, 252]
	sub	w2, w1, w2
	lsl	w2, w3, w2
	bfi	w5, w2, 0, 16
	str	w5, [x0, 7688]
	ldr	w3, [x19, 264]
	ldr	w2, [x19, 256]
	sub	w1, w1, w3
	lsl	w1, w2, w1
	bfi	w4, w1, 0, 16
	str	w4, [x0, 7728]
	ldr	w1, [x19, 32]
	cmp	w1, 4
	beq	.L33
	ldr	w1, [x19, 300]
	str	w1, [x0, 8196]
	ldr	w1, [x19, 304]
	str	w1, [x0, 8200]
	ldr	w1, [x19, 308]
	str	w1, [x0, 8204]
	ldr	w1, [x19, 312]
	str	w1, [x0, 8208]
	ldr	w1, [x19, 316]
	str	w1, [x0, 8212]
	ldr	w1, [x19, 320]
	str	w1, [x0, 8216]
	ldr	w1, [x19, 324]
	str	w1, [x0, 8220]
	ldr	w1, [x19, 328]
	str	w1, [x0, 8224]
	ldr	w1, [x19, 148]
	ldr	w2, [x0, 8228]
	sub	w1, w1, #1
	bfi	w2, w1, 16, 7
	str	w2, [x0, 8228]
	ldr	w1, [x19, 148]
	ldr	w2, [x0, 8228]
	lsr	w1, w1, 8
	sub	w1, w1, #1
	bfi	w2, w1, 8, 6
	str	w2, [x0, 8228]
	ldrh	w1, [x19, 150]
	ldr	w2, [x0, 8228]
	sub	w1, w1, #1
	bfi	w2, w1, 0, 6
	str	w2, [x0, 8228]
	ldr	w1, [x19, 32]
	cmp	w1, 4
	beq	.L34
.L26:
	ldr	w1, [x19, 156]
	str	w1, [x0, 8236]
	ldr	w1, [x19, 160]
	str	w1, [x0, 8240]
	ldr	w1, [x19, 164]
	str	w1, [x0, 8244]
	ldr	w1, [x19, 168]
	str	w1, [x0, 8248]
	ldr	w1, [x19, 172]
	str	w1, [x0, 8252]
	ldr	w1, [x19, 176]
	str	w1, [x0, 8256]
	ldr	w1, [x19, 180]
	str	w1, [x0, 8260]
	ldr	w1, [x19, 184]
	str	w1, [x0, 8264]
	ldr	w1, [x19, 152]
	ldr	w2, [x0, 8268]
	sub	w1, w1, #1
	bfi	w2, w1, 16, 7
	str	w2, [x0, 8268]
	ldr	w1, [x19, 152]
	ldr	w2, [x0, 8268]
	lsr	w1, w1, 8
	sub	w1, w1, #1
	bfi	w2, w1, 8, 6
	str	w2, [x0, 8268]
	ldrh	w1, [x19, 154]
	ldr	w2, [x0, 8268]
	sub	w1, w1, #1
	bfi	w2, w1, 0, 6
	str	w2, [x0, 8268]
.L32:
	ldr	w1, [x0, 8232]
	mov	w2, 0
	and	w1, w1, -33554431
	str	w1, [x0, 8232]
	ldr	w1, [x0, 8232]
	and	w1, w1, -2
	str	w1, [x0, 8232]
	str	w2, [x0, 8232]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L33:
	ldr	w1, [x19, 268]
	cmp	w1, 31
	bls	.L35
	ldr	w1, [x19, 188]
	str	w1, [x0, 7692]
	ldr	w1, [x19, 268]
	cmp	w1, 63
	bhi	.L22
.L38:
	ldr	w2, [x19, 192]
	mov	w3, 64
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 7696]
.L23:
	ldr	w1, [x19, 268]
	cmp	w1, 64
	bls	.L24
	ldr	w2, [x19, 196]
	mov	w3, 96
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 7700]
.L24:
	ldr	w1, [x19, 200]
	str	w1, [x0, 7704]
	ldr	w1, [x19, 204]
	str	w1, [x0, 7708]
	ldr	w1, [x19, 208]
	str	w1, [x0, 7712]
	ldr	w1, [x19, 212]
	str	w1, [x0, 7716]
	ldr	w1, [x19, 216]
	str	w1, [x0, 7720]
	ldr	w1, [x19, 32]
	cmp	w1, 4
	bne	.L26
.L34:
	ldr	w1, [x19, 272]
	cmp	w1, 31
	bls	.L36
	ldr	w1, [x19, 220]
	str	w1, [x0, 7732]
	ldr	w1, [x19, 272]
	cmp	w1, 63
	bhi	.L29
.L37:
	ldr	w2, [x19, 224]
	mov	w3, 64
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 7736]
.L30:
	ldr	w1, [x19, 272]
	cmp	w1, 64
	bls	.L31
	ldr	w2, [x19, 228]
	mov	w3, 96
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 7740]
.L31:
	ldr	w1, [x19, 232]
	str	w1, [x0, 7744]
	ldr	w1, [x19, 236]
	str	w1, [x0, 7748]
	ldr	w1, [x19, 240]
	str	w1, [x0, 7752]
	ldr	w1, [x19, 244]
	str	w1, [x0, 7756]
	ldr	w1, [x19, 248]
	str	w1, [x0, 7760]
	b	.L32
	.p2align 3
.L36:
	ldr	w2, [x19, 220]
	mov	w3, 32
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 7732]
	ldr	w1, [x19, 272]
	cmp	w1, 63
	bls	.L37
.L29:
	ldr	w1, [x19, 224]
	str	w1, [x0, 7736]
	b	.L30
	.p2align 3
.L35:
	ldr	w2, [x19, 188]
	mov	w3, 32
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 7692]
	ldr	w1, [x19, 268]
	cmp	w1, 63
	bls	.L38
.L22:
	ldr	w1, [x19, 192]
	str	w1, [x0, 7696]
	b	.L23
	.size	VENC_HAL_VEDU_Cfg_CABAC_VLC, .-VENC_HAL_VEDU_Cfg_CABAC_VLC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_CfgReg
	.type	VENC_HAL_CfgReg, %function
VENC_HAL_CfgReg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x21, x1
	mov	x0, x30
	bl	_mcount
	ldr	x20, [x19, 24]
	ldr	w2, [x21, 68]
	mov	w1, 11
	mov	w0, 2
	mov	w6, 0
	bfi	w1, w2, 10, 1
	mov	w4, w0
	orr	w1, w1, 134217728
	str	w1, [x20, 4]
	mov	w3, 0
	mov	w2, 0
	ldr	w1, [x19, 32]
	ldr	w7, [x19, 2344]
	cmp	w1, 4
	cset	w5, ne
	bfi	w0, w5, 3, 2
	ldr	w5, [x21, 68]
	bfi	w0, w5, 5, 1
	ldr	w5, [x19, 2352]
	bfi	w0, w5, 6, 1
	ldr	w5, [x19, 2348]
	bfi	w0, w5, 7, 1
	ldr	w5, [x19, 712]
	and	w0, w0, -16777473
	bfi	w0, w5, 9, 1
	ldr	w5, [x21, 44]
	cmp	w5, wzr
	cset	w8, eq
	cmp	w1, 4
	bfi	w0, w8, 10, 2
	csel	w1, wzr, w4, ne
	bfi	w0, w7, 12, 1
	ldr	w4, [x19, 2340]
	bfi	w0, w5, 16, 1
	mov	w5, 0
	orr	w0, w0, 131072
	bfi	w0, w4, 18, 2
	ldr	w4, [x19, 432]
	and	w0, w0, -6291457
	and	w0, w0, -33554433
	bfi	w0, w1, 26, 2
	mov	w1, 2
	bfi	w0, w4, 28, 2
	movk	w1, 0x7, lsl 16
	and	w0, w0, 1073741823
	str	w0, [x20, 20]
	str	w6, [x20, 24]
	ldrh	w6, [x19, 36]
	ldrb	w4, [x19]
	ldr	w0, [x19, 40]
	bfi	w3, w6, 17, 1
	bfi	w3, w4, 16, 1
	mov	w4, 0
	bfi	w3, w0, 0, 16
	str	w3, [x20, 28]
	ldr	w6, [x21, 172]
	ldr	w3, [x19, 2220]
	ldr	w0, [x19, 2200]
	bfi	w2, w6, 10, 6
	bfi	w2, w3, 18, 1
	mov	w3, 0
	bfi	w2, w0, 19, 1
	str	w2, [x20, 32]
	str	wzr, [x20, 36]
	mov	w0, 0
	str	wzr, [x20, 40]
	str	w5, [x20, 44]
	ldr	w5, [x19, 424]
	ldr	w2, [x19, 428]
	bfi	w1, w5, 19, 2
	bfi	w1, w2, 21, 1
	and	w1, w1, -8388609
	str	w1, [x20, 48]
	ldp	w1, w2, [x21, 12]
	sub	w2, w2, #1
	sub	w1, w1, #1
	bfi	w4, w2, 16, 13
	bfi	w4, w1, 0, 13
	str	w4, [x20, 52]
	str	w3, [x20, 56]
	ldr	w1, [x19, 440]
	str	w1, [x20, 60]
	ldr	w2, [x19, 448]
	ldr	w1, [x19, 452]
	bfi	w0, w2, 4, 4
	bfi	w0, w1, 0, 4
	str	w0, [x20, 64]
	ldr	w0, [x19, 32]
	cmp	w0, 4
	beq	.L64
	cbz	w0, .L65
.L44:
	mov	w2, 3
	mov	w0, 0
	str	w22, [x20, 88]
	mov	w3, 1
	bfi	w0, w2, 0, 3
	str	w2, [x20, 92]
	bfi	w0, w3, 3, 2
	str	w3, [x20, 96]
	bfi	w0, w2, 5, 3
	mov	w1, 0
	bfi	w0, w2, 8, 3
	str	w0, [x20, 100]
	mov	w0, 0
	mov	w6, 0
	ldr	w3, [x19, 420]
	mov	w4, 0
	str	w3, [x20, 184]
	ldr	w3, [x19, 76]
	str	w3, [x20, 188]
	ldr	w3, [x19, 80]
	str	w3, [x20, 192]
	ldr	w3, [x19, 84]
	str	w3, [x20, 196]
	ldp	w5, w3, [x19, 60]
	bfi	w0, w5, 16, 16
	bfi	w0, w3, 0, 16
	str	w0, [x20, 208]
	mov	w3, 0
	mov	w0, 0
	ldr	w5, [x21, 52]
	add	x5, x19, x5, lsl 2
	ldr	w5, [x5, 136]
	str	w5, [x20, 212]
	ldr	w5, [x21, 48]
	add	x5, x19, x5, lsl 2
	ldr	w5, [x5, 88]
	str	w5, [x20, 216]
	ldr	w5, [x21, 48]
	add	x5, x5, 24
	ldr	w5, [x19, x5, lsl 2]
	str	w5, [x20, 220]
	ldr	w5, [x19, 284]
	bfi	w1, w5, 0, 16
	ldr	w5, [x19, 288]
	bfi	w1, w5, 16, 16
	str	w1, [x20, 224]
	ldr	w1, [x21, 48]
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 104]
	str	w1, [x20, 228]
	ldr	w1, [x21, 48]
	add	x1, x1, 28
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 232]
	ldr	w1, [x19, 292]
	bfi	w6, w1, 0, 16
	str	w6, [x20, 236]
	ldr	w1, [x21, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 20
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 8]
	str	w1, [x20, 240]
	ldr	w1, [x21, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 24
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 244]
	ldr	w5, [x19, 288]
	ldr	w1, [x19, 284]
	bfi	w4, w5, 0, 16
	bfi	w4, w1, 16, 16
	str	w4, [x20, 248]
	ldr	w1, [x21, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 24
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 8]
	str	w1, [x20, 252]
	ldr	w1, [x21, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 28
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 256]
	ldr	w4, [x19, 292]
	ldr	w1, [x19, 296]
	bfi	w3, w4, 16, 16
	bfi	w3, w1, 0, 16
	str	w3, [x20, 260]
	ldr	w1, [x21, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 32
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 264]
	ldr	w1, [x21, 48]
	add	x1, x1, 32
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 268]
	ldr	w1, [x21, 48]
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 120]
	str	w1, [x20, 276]
	ldr	w1, [x21, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 28
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 8]
	str	w1, [x20, 280]
	ldr	w1, [x19, 44]
	str	w1, [x20, 284]
	ldr	w1, [x19, 52]
	str	w1, [x20, 288]
	ldr	w1, [x19, 48]
	str	w1, [x20, 292]
	ldr	w1, [x19, 68]
	cmp	w1, 2
	beq	.L45
	cmp	w1, 1
	beq	.L59
	and	w0, w0, -961
	and	w0, w0, -16385
	cbnz	w1, .L46
	ldr	w1, [x19, 72]
	cmp	w1, wzr
	cset	w1, eq
.L47:
	bfi	w0, w1, 16, 8
	str	w0, [x20, 296]
	mov	w0, 0
	mov	w2, 5
	ldr	w5, [x19, 2336]
	mov	w1, 0
	ldr	w3, [x19, 2332]
	mov	w4, 3
	bfi	w0, w5, 0, 3
	bfi	w1, w2, 24, 8
	bfi	w0, w3, 4, 1
	mov	w6, 15
	and	w0, w0, -33
	bfi	w1, w2, 16, 8
	orr	w0, w0, 1792
	mov	w3, 27
	orr	w0, w0, 28672
	str	w0, [x20, 316]
	str	w4, [x20, 320]
	mov	w0, 500
	str	w3, [x20, 324]
	movk	w0, 0x64, lsl 16
	bfi	w1, w6, 8, 8
	str	w0, [x20, 328]
	bfi	w1, w6, 0, 8
	str	w1, [x20, 332]
	mov	w1, 0
	mov	w0, 0
	ldr	w5, [x19, 724]
	mov	w2, -128
	ldr	w3, [x19, 720]
	ldr	w4, [x19, 716]
	bfi	w1, w5, 0, 1
	bfi	w1, w3, 1, 1
	mov	w3, 2
	bfi	w1, w4, 2, 1
	mov	w4, 0
	and	w1, w1, -25
	str	w1, [x20, 352]
	bfi	w4, w3, 16, 8
	ldr	w1, [x19, 16]
	bfi	w4, w3, 24, 8
	str	w1, [x20, 392]
	mov	w1, 0
	bfi	w1, w2, 0, 8
	mov	w3, 0
	ldr	w5, [x19, 2192]
	bfi	w3, w2, 0, 8
	bfi	w1, w2, 8, 8
	bfi	w3, w2, 8, 8
	bfi	w0, w5, 0, 1
	ldr	w5, [x19, 2196]
	bfi	w1, w2, 16, 8
	mov	w2, 128
	bfi	w0, w5, 1, 1
	ldr	w5, [x19, 2208]
	bfi	w0, w5, 2, 1
	ldr	w5, [x19, 2216]
	bfi	w0, w5, 4, 1
	ldr	w5, [x19, 2188]
	bfi	w0, w5, 8, 1
	ldr	w5, [x19, 2184]
	bfi	w0, w5, 9, 1
	ldr	w5, [x19, 2264]
	bfi	w0, w5, 10, 1
	ldr	w5, [x19, 2268]
	bfi	w0, w5, 11, 1
	ldr	w5, [x19, 2272]
	bfi	w0, w5, 12, 1
	ldr	w5, [x19, 2180]
	bfi	w0, w5, 13, 1
	ldr	w5, [x19, 2176]
	bfi	w0, w5, 14, 1
	ldr	w5, [x19, 2276]
	bfi	w0, w5, 15, 1
	str	w0, [x20, 440]
	str	w4, [x20, 444]
	mov	w0, 0
	str	w1, [x20, 464]
	bfi	w0, w2, 0, 10
	str	w3, [x20, 468]
	bfi	w0, w2, 10, 10
	str	w0, [x20, 472]
	mov	w0, 0
	str	w0, [x20, 476]
	mov	w0, 43691
	movk	w0, 0xaaaa, lsl 16
	mov	w4, 1
	ldr	w1, [x21, 52]
	add	w1, w1, 2
	umull	x2, w1, w0
	lsr	x2, x2, 32
	lsr	w2, w2, 1
	lsl	w3, w2, 2
	sub	w2, w3, w2
	sub	w1, w1, w2
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 136]
	str	w1, [x20, 480]
	ldr	w1, [x21, 52]
	add	w1, w1, w4
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 1
	lsl	w2, w0, 2
	sub	w0, w2, w0
	sub	w0, w1, w0
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 136]
	str	w0, [x20, 484]
	str	w4, [x20, 492]
	ldrb	w9, [x19, 480]
	ldrb	w8, [x19, 476]
	ldrb	w2, [x19, 472]
	ldrb	w10, [x19, 484]
	ldrb	w7, [x19, 468]
	orr	w1, w8, w2
	ldrb	w5, [x19, 464]
	orr	w0, w10, w9
	ldrb	w4, [x19, 460]
	orr	w1, w1, w0
	ldrb	w3, [x19, 456]
	orr	w0, w7, w5
	orr	w1, w1, w0
	ldr	w11, [x19, 708]
	orr	w0, w4, w3
	orr	w1, w1, w0
	mov	w0, 0
	bfi	w0, w1, 24, 1
	ldr	w1, [x19, 704]
	bfi	w0, w11, 23, 1
	bfi	w0, w1, 22, 1
	ldr	w1, [x19, 700]
	bfi	w0, w1, 21, 1
	ldr	w1, [x19, 696]
	bfi	w0, w1, 20, 1
	ldr	w1, [x19, 692]
	bfi	w0, w1, 19, 1
	ldr	w1, [x19, 688]
	bfi	w0, w1, 18, 1
	ldr	w1, [x19, 684]
	bfi	w0, w1, 17, 1
	ldr	w1, [x19, 680]
	bfi	w0, w1, 16, 1
	ldr	w1, [x19, 516]
	bfi	w0, w1, 15, 1
	ldr	w1, [x19, 512]
	bfi	w0, w1, 14, 1
	ldr	w1, [x19, 508]
	bfi	w0, w1, 13, 1
	ldr	w1, [x19, 504]
	bfi	w0, w1, 12, 1
	ldr	w1, [x19, 500]
	bfi	w0, w1, 11, 1
	ldr	w1, [x19, 496]
	bfi	w0, w1, 10, 1
	ldr	w1, [x19, 492]
	bfi	w0, w1, 9, 1
	ldr	w1, [x19, 488]
	bfi	w0, w1, 8, 1
	mov	w1, 0
	bfi	w0, w10, 7, 1
	bfi	w0, w9, 6, 1
	mov	w9, -16711936
	bfi	w0, w8, 5, 1
	mov	w8, 0
	bfi	w0, w2, 4, 1
	mov	w2, 0
	bfi	w0, w7, 3, 1
	bfi	w0, w5, 2, 1
	mov	w5, 0
	bfi	w0, w4, 1, 1
	mov	w4, 0
	bfi	w0, w3, 0, 1
	str	w0, [x20, 512]
	ldr	w3, [x19, 532]
	ldr	w0, [x19, 528]
	bfi	w2, w3, 24, 7
	ldr	w3, [x19, 524]
	bfi	w2, w0, 16, 7
	ldr	w0, [x19, 520]
	bfi	w2, w3, 8, 7
	mov	w3, 0
	bfi	w2, w0, 0, 7
	str	w2, [x20, 516]
	mov	w2, 0
	ldr	w0, [x19, 548]
	bfi	w1, w0, 24, 7
	ldr	w0, [x19, 544]
	bfi	w1, w0, 16, 7
	ldr	w0, [x19, 540]
	bfi	w1, w0, 8, 7
	ldr	w0, [x19, 536]
	bfi	w1, w0, 0, 7
	str	w1, [x20, 520]
	mov	w1, 0
	mov	w0, 0
	ldr	w7, [x19, 584]
	bfi	w5, w7, 16, 10
	ldr	w7, [x19, 552]
	bfi	w5, w7, 0, 10
	str	w5, [x20, 524]
	ldr	w5, [x19, 588]
	bfi	w4, w5, 16, 10
	ldr	w5, [x19, 556]
	bfi	w4, w5, 0, 10
	str	w4, [x20, 528]
	mov	w5, 0
	mov	w4, 0
	ldr	w7, [x19, 592]
	bfi	w3, w7, 16, 10
	ldr	w7, [x19, 560]
	bfi	w3, w7, 0, 10
	str	w3, [x20, 532]
	ldr	w3, [x19, 596]
	bfi	w2, w3, 16, 10
	ldr	w3, [x19, 564]
	bfi	w2, w3, 0, 10
	str	w2, [x20, 536]
	mov	w3, 0
	mov	w2, 0
	ldr	w7, [x19, 600]
	bfi	w1, w7, 16, 10
	ldr	w7, [x19, 568]
	bfi	w1, w7, 0, 10
	str	w1, [x20, 540]
	ldr	w1, [x19, 604]
	bfi	w0, w1, 16, 10
	ldr	w1, [x19, 572]
	bfi	w0, w1, 0, 10
	str	w0, [x20, 544]
	mov	w1, 0
	mov	w0, 0
	ldr	w7, [x19, 608]
	bfi	w5, w7, 16, 10
	ldr	w7, [x19, 576]
	bfi	w5, w7, 0, 10
	str	w5, [x20, 548]
	mov	w7, 0
	ldr	w5, [x19, 612]
	bfi	w4, w5, 16, 10
	ldr	w5, [x19, 580]
	bfi	w4, w5, 0, 10
	str	w4, [x20, 552]
	mov	w4, 0
	ldr	w5, [x19, 648]
	bfi	w3, w5, 16, 9
	ldr	w5, [x19, 616]
	bfi	w3, w5, 0, 9
	str	w3, [x20, 556]
	mov	w5, 0
	ldr	w3, [x19, 652]
	bfi	w2, w3, 16, 9
	ldr	w3, [x19, 620]
	bfi	w2, w3, 0, 9
	str	w2, [x20, 560]
	mov	w3, 0
	ldr	w2, [x19, 656]
	bfi	w1, w2, 16, 9
	ldr	w2, [x19, 624]
	bfi	w1, w2, 0, 9
	str	w1, [x20, 564]
	ldr	w1, [x19, 660]
	bfi	w0, w1, 16, 9
	ldr	w1, [x19, 628]
	bfi	w0, w1, 0, 9
	str	w0, [x20, 568]
	mov	w1, 16
	mov	w0, 64
	ldr	w2, [x19, 664]
	bfi	w7, w2, 16, 9
	ldr	w2, [x19, 632]
	bfi	w7, w2, 0, 9
	str	w7, [x20, 572]
	mov	w7, 0
	ldr	w2, [x19, 668]
	bfi	w4, w2, 16, 9
	ldr	w2, [x19, 636]
	bfi	w4, w2, 0, 9
	str	w4, [x20, 576]
	mov	w4, 0
	ldr	w2, [x19, 672]
	bfi	w5, w2, 16, 9
	ldr	w2, [x19, 640]
	bfi	w5, w2, 0, 9
	str	w5, [x20, 580]
	mov	w5, 0
	ldr	w2, [x19, 676]
	bfi	w3, w2, 16, 9
	ldr	w2, [x19, 644]
	bfi	w3, w2, 0, 9
	str	w3, [x20, 584]
	mov	w3, 0
	ldr	w2, [x19, 32]
	cmp	w2, wzr
	csel	w2, w0, w1, ne
	ldp	w0, w1, [x21, 12]
	sub	w0, w0, #1
	sub	w1, w1, #1
	add	w0, w0, w2
	add	w1, w1, w2
	udiv	w0, w0, w2
	udiv	w1, w1, w2
	ldr	w2, [x21, 168]
	mul	w0, w0, w1
	mov	w1, 0
	udiv	w0, w2, w0
	bfi	w8, w0, 0, 20
	str	w8, [x20, 588]
	mov	w8, 0
	ldr	w2, [x19, 728]
	ldr	w0, [x19, 732]
	bfi	w4, w2, 16, 5
	bfi	w4, w0, 0, 10
	str	w4, [x20, 640]
	mov	w0, 0
	mov	w4, 0
	ldr	w2, [x19, 736]
	bfi	w3, w2, 16, 5
	ldr	w2, [x19, 740]
	bfi	w3, w2, 0, 8
	str	w3, [x20, 644]
	mov	w3, 0
	ldr	w2, [x19, 744]
	bfi	w7, w2, 16, 5
	ldr	w2, [x19, 748]
	bfi	w7, w2, 0, 10
	str	w7, [x20, 648]
	ldr	w2, [x19, 752]
	bfi	w1, w2, 16, 5
	ldr	w2, [x19, 756]
	bfi	w1, w2, 0, 8
	str	w1, [x20, 652]
	mov	w2, 0
	mov	w1, 0
	ldr	w7, [x19, 760]
	bfi	w5, w7, 16, 5
	ldr	w7, [x19, 764]
	bfi	w5, w7, 0, 10
	str	w5, [x20, 656]
	ldr	w5, [x19, 768]
	bfi	w0, w5, 16, 5
	ldr	w5, [x19, 772]
	bfi	w0, w5, 0, 8
	str	w0, [x20, 660]
	mov	w5, 0
	mov	w0, 0
	ldr	w7, [x19, 776]
	bfi	w4, w7, 16, 5
	ldr	w7, [x19, 780]
	bfi	w4, w7, 0, 10
	str	w4, [x20, 664]
	ldr	w4, [x19, 784]
	bfi	w3, w4, 16, 5
	ldr	w4, [x19, 788]
	bfi	w3, w4, 0, 8
	str	w3, [x20, 668]
	mov	w4, 0
	mov	w3, 0
	ldr	w7, [x19, 792]
	bfi	w2, w7, 12, 4
	ldr	w7, [x19, 796]
	bfi	w2, w7, 8, 4
	ldr	w7, [x19, 800]
	bfi	w2, w7, 4, 4
	ldr	w7, [x19, 804]
	bfi	w2, w7, 0, 4
	ldr	w7, [x19, 808]
	bfi	w2, w7, 28, 4
	ldr	w7, [x19, 812]
	bfi	w2, w7, 24, 4
	ldr	w7, [x19, 816]
	bfi	w2, w7, 20, 4
	ldr	w7, [x19, 820]
	bfi	w2, w7, 16, 4
	str	w2, [x20, 672]
	ldr	w2, [x19, 824]
	bfi	w1, w2, 12, 4
	ldr	w2, [x19, 828]
	bfi	w1, w2, 8, 4
	ldr	w2, [x19, 832]
	bfi	w1, w2, 4, 4
	ldr	w2, [x19, 836]
	bfi	w1, w2, 0, 4
	ldr	w2, [x19, 840]
	bfi	w1, w2, 28, 4
	ldr	w2, [x19, 844]
	bfi	w1, w2, 24, 4
	ldr	w2, [x19, 848]
	bfi	w1, w2, 20, 4
	ldr	w2, [x19, 852]
	bfi	w1, w2, 16, 4
	str	w1, [x20, 676]
	mov	w2, 0
	mov	w1, 0
	ldr	w7, [x19, 856]
	bfi	w5, w7, 12, 4
	ldr	w7, [x19, 860]
	bfi	w5, w7, 8, 4
	ldr	w7, [x19, 864]
	bfi	w5, w7, 4, 4
	ldr	w7, [x19, 868]
	bfi	w5, w7, 0, 4
	ldr	w7, [x19, 872]
	bfi	w5, w7, 28, 4
	ldr	w7, [x19, 876]
	bfi	w5, w7, 24, 4
	ldr	w7, [x19, 880]
	bfi	w5, w7, 20, 4
	ldr	w7, [x19, 884]
	bfi	w5, w7, 16, 4
	str	w5, [x20, 680]
	mov	w7, 0
	ldr	w5, [x19, 888]
	bfi	w0, w5, 12, 4
	ldr	w5, [x19, 892]
	bfi	w0, w5, 8, 4
	ldr	w5, [x19, 896]
	bfi	w0, w5, 4, 4
	ldr	w5, [x19, 900]
	bfi	w0, w5, 0, 4
	ldr	w5, [x19, 904]
	bfi	w0, w5, 28, 4
	ldr	w5, [x19, 908]
	bfi	w0, w5, 24, 4
	ldr	w5, [x19, 912]
	bfi	w0, w5, 20, 4
	ldr	w5, [x19, 916]
	bfi	w0, w5, 16, 4
	str	w0, [x20, 684]
	mov	w0, 0
	ldr	w5, [x19, 920]
	bfi	w4, w5, 12, 4
	ldr	w5, [x19, 924]
	bfi	w4, w5, 8, 4
	ldr	w5, [x19, 928]
	bfi	w4, w5, 4, 4
	ldr	w5, [x19, 932]
	bfi	w4, w5, 0, 4
	ldr	w5, [x19, 936]
	bfi	w4, w5, 28, 4
	ldr	w5, [x19, 940]
	bfi	w4, w5, 24, 4
	ldr	w5, [x19, 944]
	bfi	w4, w5, 20, 4
	ldr	w5, [x19, 948]
	bfi	w4, w5, 16, 4
	str	w4, [x20, 688]
	mov	w5, 0
	ldr	w4, [x19, 952]
	bfi	w3, w4, 12, 4
	ldr	w4, [x19, 956]
	bfi	w3, w4, 8, 4
	ldr	w4, [x19, 960]
	bfi	w3, w4, 4, 4
	ldr	w4, [x19, 964]
	bfi	w3, w4, 0, 4
	ldr	w4, [x19, 968]
	bfi	w3, w4, 28, 4
	ldr	w4, [x19, 972]
	bfi	w3, w4, 24, 4
	ldr	w4, [x19, 976]
	bfi	w3, w4, 20, 4
	ldr	w4, [x19, 980]
	bfi	w3, w4, 16, 4
	str	w3, [x20, 692]
	mov	w4, 0
	ldr	w3, [x19, 984]
	bfi	w2, w3, 12, 4
	ldr	w3, [x19, 988]
	bfi	w2, w3, 8, 4
	ldr	w3, [x19, 992]
	bfi	w2, w3, 4, 4
	ldr	w3, [x19, 996]
	bfi	w2, w3, 0, 4
	ldr	w3, [x19, 1000]
	bfi	w2, w3, 28, 4
	ldr	w3, [x19, 1004]
	bfi	w2, w3, 24, 4
	ldr	w3, [x19, 1008]
	bfi	w2, w3, 20, 4
	ldr	w3, [x19, 1012]
	bfi	w2, w3, 16, 4
	str	w2, [x20, 696]
	mov	w3, 0
	ldr	w2, [x19, 1016]
	bfi	w1, w2, 12, 4
	ldr	w2, [x19, 1020]
	bfi	w1, w2, 8, 4
	ldr	w2, [x19, 1024]
	bfi	w1, w2, 4, 4
	ldr	w2, [x19, 1028]
	bfi	w1, w2, 0, 4
	ldr	w2, [x19, 1032]
	bfi	w1, w2, 28, 4
	ldr	w2, [x19, 1036]
	bfi	w1, w2, 24, 4
	ldr	w2, [x19, 1040]
	bfi	w1, w2, 20, 4
	ldr	w2, [x19, 1044]
	bfi	w1, w2, 16, 4
	str	w1, [x20, 700]
	mov	w2, 0
	ldr	w1, [x19, 1048]
	bfi	w0, w1, 12, 4
	ldr	w1, [x19, 1052]
	bfi	w0, w1, 8, 4
	ldr	w1, [x19, 1056]
	bfi	w0, w1, 4, 4
	ldr	w1, [x19, 1060]
	bfi	w0, w1, 0, 4
	ldr	w1, [x19, 1064]
	bfi	w0, w1, 28, 4
	ldr	w1, [x19, 1068]
	bfi	w0, w1, 24, 4
	ldr	w1, [x19, 1072]
	bfi	w0, w1, 20, 4
	ldr	w1, [x19, 1076]
	bfi	w0, w1, 16, 4
	str	w0, [x20, 704]
	mov	w1, 0
	ldr	w0, [x19, 1080]
	bfi	w8, w0, 0, 3
	ldr	w0, [x19, 1084]
	bfi	w8, w0, 3, 7
	ldr	w0, [x19, 1088]
	bfi	w8, w0, 10, 3
	ldr	w0, [x19, 1092]
	bfi	w8, w0, 13, 5
	ldr	w0, [x19, 1096]
	bfi	w8, w0, 18, 4
	ldr	w0, [x19, 1100]
	bfi	w8, w0, 22, 4
	ldr	w0, [x19, 1104]
	bfi	w8, w0, 26, 4
	str	w8, [x20, 708]
	mov	w8, 0
	ldr	w0, [x19, 1108]
	bfi	w7, w0, 0, 3
	ldr	w0, [x19, 1112]
	bfi	w7, w0, 3, 7
	ldr	w0, [x19, 1116]
	bfi	w7, w0, 10, 3
	ldr	w0, [x19, 1120]
	bfi	w7, w0, 13, 5
	ldr	w0, [x19, 1124]
	bfi	w7, w0, 18, 4
	ldr	w0, [x19, 1128]
	bfi	w7, w0, 22, 4
	ldr	w0, [x19, 1132]
	bfi	w7, w0, 26, 4
	str	w7, [x20, 712]
	ldr	w0, [x19, 1136]
	bfi	w5, w0, 0, 3
	ldr	w0, [x19, 1140]
	bfi	w5, w0, 3, 7
	ldr	w0, [x19, 1144]
	bfi	w5, w0, 10, 3
	ldr	w0, [x19, 1148]
	bfi	w5, w0, 13, 5
	ldr	w0, [x19, 1152]
	bfi	w5, w0, 18, 4
	ldr	w0, [x19, 1156]
	bfi	w5, w0, 22, 4
	ldr	w0, [x19, 1160]
	bfi	w5, w0, 26, 4
	str	w5, [x20, 716]
	mov	w0, 0
	mov	w5, 0
	ldr	w7, [x19, 1436]
	bfi	w4, w7, 0, 3
	ldr	w7, [x19, 1440]
	bfi	w4, w7, 3, 7
	ldr	w7, [x19, 1444]
	bfi	w4, w7, 10, 3
	ldr	w7, [x19, 1448]
	bfi	w4, w7, 13, 5
	ldr	w7, [x19, 1452]
	bfi	w4, w7, 18, 4
	ldr	w7, [x19, 1456]
	bfi	w4, w7, 22, 4
	ldr	w7, [x19, 1460]
	bfi	w4, w7, 26, 4
	str	w4, [x20, 720]
	mov	w7, 0
	ldr	w4, [x19, 1464]
	bfi	w3, w4, 0, 3
	ldr	w4, [x19, 1468]
	bfi	w3, w4, 3, 7
	ldr	w4, [x19, 1472]
	bfi	w3, w4, 10, 3
	ldr	w4, [x19, 1476]
	bfi	w3, w4, 13, 5
	ldr	w4, [x19, 1480]
	bfi	w3, w4, 18, 4
	ldr	w4, [x19, 1484]
	bfi	w3, w4, 22, 4
	ldr	w4, [x19, 1488]
	bfi	w3, w4, 26, 4
	str	w3, [x20, 724]
	mov	w4, 0
	ldr	w3, [x19, 1492]
	bfi	w2, w3, 0, 3
	ldr	w3, [x19, 1496]
	bfi	w2, w3, 3, 7
	ldr	w3, [x19, 1500]
	bfi	w2, w3, 10, 3
	ldr	w3, [x19, 1504]
	bfi	w2, w3, 13, 5
	ldr	w3, [x19, 1508]
	bfi	w2, w3, 18, 4
	ldr	w3, [x19, 1512]
	bfi	w2, w3, 22, 4
	ldr	w3, [x19, 1516]
	bfi	w2, w3, 26, 4
	str	w2, [x20, 728]
	mov	w3, 0
	ldr	w2, [x19, 1520]
	bfi	w1, w2, 0, 3
	ldr	w2, [x19, 1524]
	bfi	w1, w2, 3, 7
	ldr	w2, [x19, 1528]
	bfi	w1, w2, 10, 3
	ldr	w2, [x19, 1532]
	bfi	w1, w2, 13, 5
	ldr	w2, [x19, 1536]
	bfi	w1, w2, 18, 4
	ldr	w2, [x19, 1540]
	bfi	w1, w2, 22, 4
	ldr	w2, [x19, 1544]
	bfi	w1, w2, 26, 4
	str	w1, [x20, 732]
	mov	w2, 0
	ldr	w1, [x19, 1164]
	bfi	w0, w1, 0, 4
	ldr	w1, [x19, 1168]
	bfi	w0, w1, 4, 4
	ldr	w1, [x19, 1172]
	bfi	w0, w1, 8, 4
	ldr	w1, [x19, 1176]
	bfi	w0, w1, 12, 4
	ldr	w1, [x19, 1180]
	bfi	w0, w1, 16, 4
	ldr	w1, [x19, 1184]
	bfi	w0, w1, 20, 4
	ldr	w1, [x19, 1188]
	bfi	w0, w1, 24, 4
	ldr	w1, [x19, 1192]
	bfi	w0, w1, 28, 4
	str	w0, [x20, 736]
	ldr	w0, [x19, 1196]
	bfi	w5, w0, 0, 4
	ldr	w0, [x19, 1200]
	bfi	w5, w0, 4, 4
	ldr	w0, [x19, 1204]
	bfi	w5, w0, 8, 4
	ldr	w0, [x19, 1208]
	bfi	w5, w0, 12, 4
	ldr	w0, [x19, 1212]
	bfi	w5, w0, 16, 4
	ldr	w0, [x19, 1216]
	bfi	w5, w0, 20, 4
	str	w5, [x20, 740]
	mov	w5, 0
	mov	w0, 0
	ldr	w1, [x19, 1220]
	bfi	w7, w1, 0, 10
	ldr	w1, [x19, 1224]
	bfi	w7, w1, 16, 5
	str	w7, [x20, 744]
	ldr	w1, [x19, 1228]
	bfi	w4, w1, 0, 8
	ldr	w1, [x19, 1232]
	bfi	w4, w1, 16, 5
	str	w4, [x20, 748]
	mov	w1, 0
	mov	w4, 0
	ldr	w7, [x19, 1236]
	bfi	w3, w7, 0, 10
	ldr	w7, [x19, 1240]
	bfi	w3, w7, 16, 5
	str	w3, [x20, 752]
	ldr	w3, [x19, 1244]
	bfi	w2, w3, 0, 8
	ldr	w3, [x19, 1248]
	bfi	w2, w3, 16, 5
	str	w2, [x20, 756]
	mov	w3, 0
	mov	w2, 0
	ldr	w7, [x19, 1252]
	bfi	w5, w7, 0, 10
	ldr	w7, [x19, 1256]
	bfi	w5, w7, 16, 5
	str	w5, [x20, 760]
	mov	w7, 0
	ldr	w5, [x19, 1260]
	bfi	w0, w5, 0, 8
	ldr	w5, [x19, 1264]
	bfi	w0, w5, 16, 5
	str	w0, [x20, 764]
	mov	w0, 0
	ldr	w5, [x19, 1548]
	bfi	w1, w5, 0, 4
	ldr	w5, [x19, 1552]
	bfi	w1, w5, 4, 4
	ldr	w5, [x19, 1556]
	bfi	w1, w5, 8, 4
	ldr	w5, [x19, 1560]
	bfi	w1, w5, 12, 4
	ldr	w5, [x19, 1564]
	bfi	w1, w5, 16, 4
	ldr	w5, [x19, 1568]
	bfi	w1, w5, 20, 4
	str	w1, [x20, 768]
	mov	w5, 0
	ldr	w1, [x19, 1572]
	bfi	w4, w1, 0, 12
	ldr	w1, [x19, 1576]
	bfi	w4, w1, 12, 12
	ldr	w1, [x19, 1580]
	bfi	w4, w1, 24, 8
	str	w4, [x20, 772]
	mov	w4, 0
	ldr	w1, [x19, 1584]
	bfi	w3, w1, 0, 12
	ldr	w1, [x19, 1588]
	bfi	w3, w1, 12, 12
	ldr	w1, [x19, 1592]
	bfi	w3, w1, 24, 8
	str	w3, [x20, 776]
	mov	w3, 0
	ldr	w1, [x19, 1596]
	bfi	w2, w1, 0, 12
	ldr	w1, [x19, 1600]
	bfi	w2, w1, 12, 12
	ldr	w1, [x19, 1604]
	bfi	w2, w1, 24, 8
	str	w2, [x20, 780]
	mov	w2, 0
	ldr	w1, [x19, 1608]
	bfi	w7, w1, 0, 8
	ldr	w1, [x19, 1612]
	bfi	w7, w1, 8, 8
	ldr	w1, [x19, 1616]
	bfi	w7, w1, 16, 8
	str	w7, [x20, 784]
	ldr	w1, [x19, 1268]
	bfi	w0, w1, 0, 1
	ldr	w1, [x19, 1272]
	bfi	w0, w1, 1, 3
	ldr	w1, [x19, 1276]
	bfi	w0, w1, 4, 6
	ldr	w1, [x19, 1280]
	bfi	w0, w1, 10, 5
	ldr	w1, [x19, 1284]
	bfi	w0, w1, 15, 6
	ldr	w1, [x19, 1288]
	bfi	w0, w1, 21, 5
	str	w0, [x20, 788]
	mov	w1, 0
	mov	w0, 0
	ldr	w7, [x19, 1292]
	bfi	w5, w7, 0, 1
	ldr	w7, [x19, 1296]
	bfi	w5, w7, 1, 3
	ldr	w7, [x19, 1300]
	bfi	w5, w7, 4, 6
	ldr	w7, [x19, 1304]
	bfi	w5, w7, 10, 5
	ldr	w7, [x19, 1308]
	bfi	w5, w7, 15, 6
	ldr	w7, [x19, 1312]
	bfi	w5, w7, 21, 5
	str	w5, [x20, 792]
	mov	w7, 0
	ldr	w5, [x19, 1316]
	bfi	w4, w5, 0, 1
	ldr	w5, [x19, 1320]
	bfi	w4, w5, 1, 3
	ldr	w5, [x19, 1324]
	bfi	w4, w5, 4, 6
	ldr	w5, [x19, 1328]
	bfi	w4, w5, 10, 5
	ldr	w5, [x19, 1332]
	bfi	w4, w5, 15, 6
	ldr	w5, [x19, 1336]
	bfi	w4, w5, 21, 5
	str	w4, [x20, 796]
	mov	w4, 0
	ldr	w5, [x19, 1340]
	bfi	w3, w5, 0, 1
	ldr	w5, [x19, 1344]
	bfi	w3, w5, 1, 3
	ldr	w5, [x19, 1348]
	bfi	w3, w5, 4, 6
	ldr	w5, [x19, 1352]
	bfi	w3, w5, 10, 5
	ldr	w5, [x19, 1356]
	bfi	w3, w5, 15, 6
	ldr	w5, [x19, 1360]
	bfi	w3, w5, 21, 5
	str	w3, [x20, 800]
	mov	w5, 0
	ldr	w3, [x19, 1364]
	bfi	w2, w3, 0, 1
	ldr	w3, [x19, 1368]
	bfi	w2, w3, 1, 3
	ldr	w3, [x19, 1372]
	bfi	w2, w3, 4, 6
	ldr	w3, [x19, 1376]
	bfi	w2, w3, 10, 5
	ldr	w3, [x19, 1380]
	bfi	w2, w3, 15, 6
	ldr	w3, [x19, 1384]
	bfi	w2, w3, 21, 5
	str	w2, [x20, 804]
	mov	w3, 0
	ldr	w2, [x19, 1388]
	bfi	w1, w2, 0, 1
	ldr	w2, [x19, 1392]
	bfi	w1, w2, 1, 3
	ldr	w2, [x19, 1396]
	bfi	w1, w2, 4, 6
	ldr	w2, [x19, 1400]
	bfi	w1, w2, 10, 5
	ldr	w2, [x19, 1404]
	bfi	w1, w2, 15, 6
	ldr	w2, [x19, 1408]
	bfi	w1, w2, 21, 5
	str	w1, [x20, 808]
	mov	w2, 0
	ldr	w1, [x19, 1412]
	bfi	w0, w1, 0, 1
	ldr	w1, [x19, 1416]
	bfi	w0, w1, 1, 3
	ldr	w1, [x19, 1420]
	bfi	w0, w1, 4, 6
	ldr	w1, [x19, 1424]
	bfi	w0, w1, 10, 5
	ldr	w1, [x19, 1428]
	bfi	w0, w1, 15, 6
	ldr	w1, [x19, 1432]
	bfi	w0, w1, 21, 5
	str	w0, [x20, 812]
	mov	w1, 0
	ldr	w0, [x19, 416]
	bfi	w4, w0, 20, 8
	ldr	w0, [x21, 68]
	bfi	w4, w0, 28, 1
	str	w4, [x20, 1024]
	str	w9, [x20, 1120]
	mov	w4, 3
	ldr	w0, [x19, 2388]
	bfi	w8, w0, 0, 1
	str	w8, [x20, 1536]
	ldr	w0, [x19, 2392]
	bfi	w7, w0, 0, 16
	str	w7, [x20, 1540]
	mov	w0, 0
	ldr	w7, [x19, 2396]
	bfi	w5, w7, 0, 16
	str	w5, [x20, 1544]
	ldr	w5, [x19, 2400]
	bfi	w1, w5, 0, 16
	str	w1, [x20, 1548]
	mov	w5, 20
	mov	w1, 0
	ldr	w7, [x19, 2404]
	bfi	w3, w7, 18, 9
	ldr	w7, [x19, 2408]
	bfi	w3, w7, 9, 9
	ldr	w7, [x19, 2412]
	bfi	w3, w7, 0, 9
	str	w3, [x20, 1556]
	ldr	w3, [x19, 2416]
	bfi	w2, w3, 16, 8
	ldr	w3, [x19, 2420]
	bfi	w2, w3, 8, 8
	ldr	w3, [x19, 2424]
	bfi	w2, w3, 0, 8
	str	w2, [x20, 1560]
	mov	w3, 0
	mov	w2, 0
	ldr	w7, [x19, 2428]
	bfi	w0, w7, 8, 4
	ldr	w7, [x19, 2432]
	bfi	w0, w7, 4, 4
	ldr	w7, [x19, 2436]
	bfi	w0, w7, 0, 4
	mov	w7, 0
	bfi	w0, w4, 20, 8
	mov	w4, 0
	bfi	w0, w5, 12, 8
	str	w0, [x20, 1564]
	ldrh	w0, [x19, 2568]
	str	w0, [x20, 1568]
	ldr	w5, [x19, 2584]
	ldr	w0, [x19, 2580]
	bfi	w1, w5, 0, 8
	ldr	w5, [x19, 2576]
	bfi	w1, w0, 8, 8
	ldr	w0, [x19, 2572]
	bfi	w1, w5, 16, 8
	bfi	w1, w0, 24, 8
	str	w1, [x20, 1572]
	mov	w1, 0
	ldr	w0, [x19, 2444]
	bfi	w3, w0, 24, 8
	ldr	w0, [x19, 2448]
	bfi	w3, w0, 16, 8
	ldr	w0, [x19, 2452]
	bfi	w3, w0, 8, 8
	ldr	w0, [x19, 2456]
	bfi	w3, w0, 0, 8
	str	w3, [x20, 1584]
	mov	w3, 0
	mov	w0, 0
	ldr	w5, [x19, 2460]
	bfi	w2, w5, 0, 8
	ldr	w5, [x19, 2464]
	bfi	w2, w5, 8, 8
	ldr	w5, [x19, 2468]
	bfi	w2, w5, 16, 1
	ldr	w5, [x19, 2612]
	bfi	w2, w5, 17, 1
	str	w2, [x20, 1588]
	mov	w5, 0
	ldr	w2, [x19, 2472]
	bfi	w4, w2, 0, 16
	ldr	w2, [x19, 2476]
	bfi	w4, w2, 16, 16
	str	w4, [x20, 1592]
	mov	w4, 0
	ldr	w2, [x19, 2480]
	bfi	w1, w2, 22, 9
	ldr	w2, [x19, 2484]
	bfi	w1, w2, 17, 5
	ldr	w2, [x19, 2488]
	bfi	w1, w2, 9, 8
	ldr	w2, [x19, 2440]
	bfi	w1, w2, 0, 9
	str	w1, [x20, 1596]
	mov	w2, 0
	ldr	w1, [x19, 2492]
	bfi	w7, w1, 8, 14
	ldr	w1, [x19, 2496]
	bfi	w7, w1, 0, 8
	str	w7, [x20, 1732]
	mov	w1, 0
	ldr	w7, [x19, 2500]
	bfi	w3, w7, 16, 8
	ldr	w7, [x19, 2504]
	bfi	w3, w7, 8, 8
	ldr	w7, [x19, 2508]
	bfi	w3, w7, 0, 8
	str	w3, [x20, 1736]
	mov	w7, 0
	ldr	w3, [x19, 2516]
	bfi	w0, w3, 17, 1
	ldr	w3, [x19, 2520]
	bfi	w0, w3, 12, 2
	ldr	w3, [x19, 2524]
	bfi	w0, w3, 4, 4
	ldr	w3, [x19, 2528]
	bfi	w0, w3, 0, 4
	str	w0, [x20, 1740]
	mov	w0, 0
	ldr	w3, [x19, 2532]
	bfi	w5, w3, 0, 10
	ldr	w3, [x19, 2536]
	bfi	w5, w3, 16, 9
	str	w5, [x20, 1744]
	ldr	w3, [x19, 2540]
	bfi	w4, w3, 0, 10
	ldr	w3, [x19, 2544]
	bfi	w4, w3, 16, 9
	str	w4, [x20, 1748]
	mov	w4, 0
	mov	w3, 0
	ldr	w5, [x19, 2548]
	bfi	w2, w5, 0, 10
	ldr	w5, [x19, 2552]
	bfi	w2, w5, 16, 9
	str	w2, [x20, 1752]
	ldr	w2, [x19, 2556]
	bfi	w1, w2, 0, 10
	ldr	w2, [x19, 2560]
	bfi	w1, w2, 16, 9
	str	w1, [x20, 1756]
	mov	w2, 0
	mov	w1, 0
	ldr	w5, [x19, 2564]
	bfi	w7, w5, 0, 16
	str	w7, [x20, 1776]
	ldr	w5, [x19, 2588]
	bfi	w0, w5, 0, 13
	ldr	w5, [x19, 2592]
	bfi	w0, w5, 13, 1
	str	w0, [x20, 1780]
	mov	w0, 0
	mov	w5, 257
	ldr	w7, [x19, 2596]
	movk	w5, 0xc8, lsl 16
	bfi	w4, w7, 0, 18
	str	w4, [x20, 1784]
	ldr	w4, [x19, 2600]
	bfi	w3, w4, 0, 18
	str	w3, [x20, 1788]
	ldr	w4, [x19, 2624]
	ldr	w3, [x19, 2628]
	bfi	w2, w4, 16, 7
	bfi	w2, w3, 23, 7
	str	w2, [x20, 1792]
	ldr	w4, [x19, 2616]
	ldr	w3, [x19, 2620]
	bfi	w1, w4, 0, 8
	ldr	w2, [x19, 2604]
	bfi	w1, w3, 8, 8
	bfi	w1, w2, 16, 8
	str	w1, [x20, 1796]
	ldr	w3, [x19, 2608]
	ldr	w2, [x19, 2632]
	ldr	w1, [x19, 2636]
	bfi	w0, w3, 12, 8
	bfi	w0, w2, 8, 4
	bfi	w0, w1, 0, 8
	str	w0, [x20, 1800]
	str	w5, [x20, 1804]
	ldr	w0, [x19, 32]
	cbnz	w0, .L49
	mov	w1, 23
	bfi	w0, w1, 0, 5
	bfi	w0, w6, 5, 5
	bfi	w0, w1, 10, 5
	bfi	w0, w6, 15, 5
	and	w0, w0, -1048577
	str	w0, [x20, 2052]
.L50:
	mov	w3, 16
	mov	w1, 2
	mov	w0, 0
	str	w1, [x20, 2560]
	str	wzr, [x20, 2592]
	bfi	w0, w3, 6, 6
	mov	w1, 10
	mov	w2, 64
	bfi	w0, w1, 0, 6
	str	w0, [x20, 2596]
	mov	w4, 51
	mov	w1, 4
	ldp	w6, w0, [x21, 4]
	mov	w5, 0
	mov	w7, 0
	ubfiz	w0, w0, 8, 6
	orr	w0, w0, w2
	bfi	w0, w6, 0, 6
	bfi	w0, w4, 16, 6
	mov	w4, 0
	bfi	w0, w1, 24, 4
	str	w0, [x20, 3076]
	ldr	w6, [x19, 32]
	ldp	w0, w1, [x21, 12]
	cmp	w6, wzr
	ldr	w6, [x19, 1800]
	csel	w2, w2, w3, ne
	sub	w0, w0, #1
	sub	w1, w1, #1
	add	w0, w0, w2
	add	w1, w1, w2
	mov	w3, 0
	udiv	w0, w0, w2
	udiv	w1, w1, w2
	ldr	w2, [x21, 168]
	mul	w0, w0, w1
	mov	w1, 0
	udiv	w0, w2, w0
	mov	w2, 0
	bfi	w5, w0, 0, 16
	bfi	w5, w6, 16, 6
	str	w5, [x20, 3080]
	ldr	w6, [x19, 1832]
	ldr	w5, [x19, 1792]
	mov	w0, 0
	bfi	w4, w6, 0, 6
	ldr	w6, [x19, 1796]
	bfi	w4, w5, 24, 6
	ldr	w5, [x19, 1804]
	bfi	w4, w6, 16, 6
	mov	w6, 0
	bfi	w4, w5, 8, 6
	str	w4, [x20, 3084]
	mov	w4, 0
	ldr	w5, [x19, 1652]
	bfi	w3, w5, 24, 8
	ldr	w5, [x19, 1648]
	bfi	w3, w5, 16, 8
	ldr	w5, [x19, 1644]
	bfi	w3, w5, 8, 8
	ldr	w5, [x19, 1640]
	bfi	w3, w5, 0, 8
	str	w3, [x20, 3088]
	mov	w3, 0
	ldr	w5, [x19, 1668]
	bfi	w2, w5, 24, 8
	ldr	w5, [x19, 1664]
	bfi	w2, w5, 16, 8
	ldr	w5, [x19, 1660]
	bfi	w2, w5, 8, 8
	ldr	w5, [x19, 1656]
	bfi	w2, w5, 0, 8
	str	w2, [x20, 3092]
	mov	w5, 0
	ldr	w2, [x19, 1684]
	bfi	w1, w2, 24, 8
	ldr	w2, [x19, 1680]
	bfi	w1, w2, 16, 8
	ldr	w2, [x19, 1676]
	bfi	w1, w2, 8, 8
	ldr	w2, [x19, 1672]
	bfi	w1, w2, 0, 8
	str	w1, [x20, 3096]
	mov	w2, 0
	ldr	w1, [x19, 1700]
	bfi	w0, w1, 24, 8
	ldr	w1, [x19, 1696]
	bfi	w0, w1, 16, 8
	ldr	w1, [x19, 1692]
	bfi	w0, w1, 8, 8
	ldr	w1, [x19, 1688]
	bfi	w0, w1, 0, 8
	str	w0, [x20, 3072]
	add	x1, x20, 3152
	ldr	w0, [x19, 1824]
	bfi	w4, w0, 0, 4
	ldr	w0, [x19, 1828]
	bfi	w4, w0, 4, 4
	str	w4, [x20, 3100]
	mov	w4, 0
	ldr	w0, [x19, 1776]
	bfi	w7, w0, 0, 4
	str	w7, [x20, 3104]
	ldr	w0, [x19, 1772]
	bfi	w3, w0, 0, 4
	ldr	w0, [x19, 1780]
	bfi	w3, w0, 4, 4
	str	w3, [x20, 3108]
	add	x0, x19, 1836
	add	x3, x19, 2156
	ldr	w7, [x19, 1784]
	bfi	w6, w7, 0, 4
	str	w6, [x20, 3112]
	ldr	w6, [x19, 1704]
	bfi	w2, w6, 0, 2
	ldr	w6, [x19, 1708]
	bfi	w2, w6, 2, 2
	ldr	w6, [x19, 1712]
	bfi	w2, w6, 4, 2
	ldr	w6, [x19, 1716]
	bfi	w2, w6, 6, 2
	ldr	w6, [x19, 1720]
	bfi	w2, w6, 8, 2
	ldr	w6, [x19, 1724]
	bfi	w2, w6, 10, 2
	ldr	w6, [x19, 1728]
	bfi	w2, w6, 12, 2
	ldr	w6, [x19, 1732]
	bfi	w2, w6, 14, 2
	ldr	w6, [x19, 1736]
	bfi	w2, w6, 16, 2
	ldr	w6, [x19, 1740]
	bfi	w2, w6, 18, 2
	ldr	w6, [x19, 1744]
	bfi	w2, w6, 20, 2
	ldr	w6, [x19, 1748]
	bfi	w2, w6, 22, 2
	ldr	w6, [x19, 1752]
	bfi	w2, w6, 24, 2
	ldr	w6, [x19, 1756]
	bfi	w2, w6, 26, 2
	ldr	w6, [x19, 1760]
	bfi	w2, w6, 28, 2
	ldr	w6, [x19, 1764]
	bfi	w2, w6, 30, 2
	str	w2, [x20, 3116]
	ldr	w2, [x19, 1768]
	bfi	w5, w2, 0, 5
	str	w5, [x20, 3120]
	ldr	w5, [x19, 1812]
	ldr	w2, [x19, 1816]
	ldr	w6, [x19, 1820]
	bfi	w4, w5, 24, 4
	bfi	w4, w2, 28, 4
	ldr	w5, [x19, 1788]
	ldr	w2, [x19, 1808]
	bfi	w4, w6, 16, 6
	bfi	w4, w5, 8, 4
	bfi	w4, w2, 0, 6
	str	w4, [x20, 3124]
	.p2align 2
.L52:
	add	x1, x1, 4
	ldr	w2, [x0], 4
	cmp	x3, x0
	str	w2, [x1, -4]
	bne	.L52
	str	wzr, [x20, 3660]
	mov	w0, 0
	str	w0, [x20, 4608]
	mov	w1, 13
	str	w0, [x20, 4612]
	bfi	w0, w1, 0, 4
	str	wzr, [x20, 4616]
	mov	w2, -1
	str	w0, [x20, 4620]
	mov	w0, 7
	str	wzr, [x20, 4624]
	mov	w6, 0
	str	wzr, [x20, 4628]
	mov	w4, 0
	str	wzr, [x20, 4632]
	mov	w5, 0
	str	wzr, [x20, 4636]
	mov	w3, 0
	str	w2, [x20, 4640]
	mov	x1, x21
	str	w0, [x20, 4644]
	str	w2, [x20, 4648]
	str	w0, [x20, 4652]
	str	w2, [x20, 4656]
	str	w0, [x20, 4660]
	str	w2, [x20, 4664]
	str	w0, [x20, 4668]
	ldr	w0, [x19, 2360]
	bfi	w6, w0, 0, 1
	str	w6, [x20, 5308]
	mov	x0, x19
	ldr	w2, [x19, 2364]
	str	w2, [x20, 5284]
	ldr	w2, [x19, 2368]
	str	w2, [x20, 5288]
	ldr	w2, [x19, 2372]
	str	w2, [x20, 5292]
	ldr	w2, [x19, 2376]
	str	w2, [x20, 5296]
	ldr	w2, [x19, 2380]
	str	w2, [x20, 5300]
	ldr	w2, [x19, 2384]
	str	w2, [x20, 5304]
	ldr	w6, [x19, 1620]
	ldr	w2, [x19, 1624]
	bfi	w4, w6, 0, 16
	bfi	w4, w2, 16, 16
	str	w4, [x20, 5764]
	ldr	w2, [x19, 1636]
	bfi	w5, w2, 16, 16
	str	w5, [x20, 5768]
	ldr	w4, [x19, 1628]
	ldr	w2, [x19, 1632]
	bfi	w3, w4, 0, 16
	bfi	w3, w2, 16, 16
	str	w3, [x20, 5772]
	bl	VENC_HAL_VEDU_Cfg_CABAC_VLC
	ldr	w0, [x19, 56]
	mov	w2, -85
	str	w0, [x20, 9220]
	mov	w0, 0
	mov	w1, 0
	mov	w4, 0
	ldr	w3, [x19, 436]
	bfi	w1, w2, 0, 8
	mov	w2, 85
	mov	w5, 0
	bfi	w0, w3, 0, 1
	str	w0, [x20, 9232]
	bfi	w1, w2, 8, 8
	mov	w2, 0
	ldr	w0, [x19, 444]
	mov	w3, 0
	str	w0, [x20, 9236]
	mov	w8, 0
	mov	w0, 0
	add	x7, x20, 61440
	ldr	w6, [x21, 44]
	cmp	w6, wzr
	cset	w6, ne
	str	w6, [x20, 9240]
	ldr	w6, [x19, 276]
	str	w6, [x20, 9244]
	mov	w6, 0
	str	wzr, [x20, 9248]
	str	wzr, [x20, 9252]
	str	wzr, [x20, 9256]
	str	w1, [x20, 9728]
	mov	w1, 0
	ldr	w9, [x19, 2156]
	bfi	w4, w9, 0, 6
	ldr	w9, [x19, 2160]
	bfi	w4, w9, 8, 2
	ldr	w9, [x19, 2164]
	bfi	w4, w9, 10, 1
	ldr	w9, [x19, 2168]
	bfi	w4, w9, 12, 4
	ldr	w9, [x19, 2172]
	bfi	w4, w9, 16, 5
	str	w4, [x20, 12240]
	mov	w9, 3
	mov	w4, 1
	ldr	w10, [x19, 2224]
	str	w10, [x20, 12244]
	ldr	w10, [x19, 2228]
	str	w10, [x20, 12248]
	ldr	w10, [x19, 2232]
	bfi	w2, w10, 0, 16
	ldr	w10, [x19, 2236]
	bfi	w2, w10, 16, 2
	ldr	w10, [x19, 2240]
	bfi	w2, w10, 18, 2
	ldr	w10, [x19, 2244]
	bfi	w2, w10, 20, 4
	ldr	w10, [x19, 2248]
	bfi	w2, w10, 24, 1
	ldr	w10, [x19, 2256]
	bfi	w2, w10, 25, 2
	ldr	w10, [x19, 2260]
	bfi	w2, w10, 27, 1
	ldr	w10, [x19, 2252]
	bfi	w2, w10, 28, 1
	str	w2, [x20, 12252]
	ldr	w2, [x19, 2292]
	bfi	w3, w2, 0, 1
	ldr	w2, [x19, 2288]
	bfi	w3, w2, 2, 2
	ldr	w2, [x19, 2284]
	bfi	w3, w2, 4, 2
	ldr	w2, [x19, 2280]
	bfi	w3, w2, 6, 1
	ldr	w2, [x19, 2300]
	bfi	w3, w2, 8, 6
	ldr	w2, [x19, 2304]
	bfi	w3, w2, 24, 6
	str	w3, [x20, 12256]
	ldr	w2, [x19, 2308]
	bfi	w5, w2, 0, 6
	ldr	w2, [x19, 2312]
	bfi	w5, w2, 8, 6
	ldr	w2, [x19, 2316]
	bfi	w5, w2, 16, 6
	ldr	w2, [x19, 2320]
	bfi	w5, w2, 24, 6
	str	w5, [x20, 12260]
	ldr	w2, [x19, 2324]
	bfi	w8, w2, 0, 6
	ldr	w2, [x19, 2328]
	bfi	w8, w2, 6, 25
	str	w8, [x20, 12264]
	ldr	w3, [x19, 336]
	ldr	w2, [x19, 352]
	bfi	w0, w3, 10, 1
	ldr	w3, [x19, 332]
	bfi	w0, w2, 9, 1
	ldr	w2, [x19, 356]
	bfi	w0, w3, 8, 1
	ldr	w3, [x19, 360]
	bfi	w0, w2, 7, 1
	ldr	w2, [x19, 344]
	bfi	w0, w3, 6, 1
	ldr	w3, [x19, 340]
	bfi	w0, w2, 5, 1
	ldr	w2, [x19, 364]
	bfi	w0, w3, 4, 1
	ldr	w3, [x19, 368]
	bfi	w0, w2, 3, 1
	ldr	w2, [x19, 372]
	bfi	w0, w3, 2, 1
	bfi	w0, w2, 1, 1
	ldr	w2, [x19, 376]
	bfi	w0, w2, 0, 1
	str	w0, [x20, 504]
	ldr	w2, [x19, 348]
	ldr	w0, [x19, 380]
	ldr	w3, [x19, 384]
	bfi	w1, w2, 7, 1
	bfi	w1, w0, 6, 1
	ldr	w2, [x19, 388]
	ldr	w0, [x19, 392]
	bfi	w1, w3, 5, 1
	bfi	w1, w2, 4, 1
	ldr	w3, [x19, 396]
	bfi	w1, w0, 3, 1
	ldr	w2, [x19, 400]
	ldr	w0, [x19, 404]
	bfi	w1, w3, 2, 1
	bfi	w1, w2, 1, 1
	bfi	w1, w0, 0, 1
	str	w1, [x20, 508]
	ldr	w1, [x19, 408]
	ldr	w0, [x19, 412]
	bfi	w6, w1, 0, 1
	bfi	w6, w0, 3, 1
	bfi	w6, w9, 16, 4
	str	w6, [x7]
	str	wzr, [x7, 32]
	ldr	x0, [x19, 24]
	ldr	w1, [x19, 2640]
	add	x0, x0, 61440
	str	w1, [x0, 520]
	ldr	w1, [x19, 2640]
	str	w1, [x0, 524]
	ldr	w1, [x19, 2648]
	str	w1, [x0, 752]
	ldr	w1, [x19, 2644]
	str	w1, [x0, 756]
	ldr	w1, [x19, 2648]
	str	w1, [x0, 772]
	ldr	w1, [x19, 2644]
	str	w1, [x0, 776]
	str	wzr, [x7, 8]
	str	wzr, [x20, 16]
	str	w4, [x20, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L49:
	mov	w0, 0
	bfi	w0, w6, 0, 5
	bfi	w0, w6, 5, 5
	bfi	w0, w6, 10, 5
	bfi	w0, w6, 15, 5
	and	w0, w0, -1048577
	str	w0, [x20, 2052]
	b	.L50
.L59:
	mov	w2, 6
.L45:
	bfi	w0, w2, 6, 4
	and	w0, w0, -16385
.L46:
	ldrb	w1, [x19, 72]
	b	.L47
.L65:
	mov	w0, 15
	mov	w22, 0
	bfi	w22, w0, 16, 6
	mov	w0, 31
	bfi	w22, w0, 0, 7
	b	.L44
.L64:
	ldp	w0, w1, [x21, 12]
	mov	w2, 32
	mov	w4, 72
	lsr	w1, w1, 2
	lsr	w0, w0, 2
	sub	w1, w1, #16
	sub	w0, w0, #16
	lsr	w1, w1, 1
	lsr	w0, w0, 1
	cmp	w1, w2
	mov	w3, 7
	csel	w1, w1, w2, ls
	cmp	w0, w4
	and	w1, w1, 2147483644
	csel	w0, w0, w4, ls
	cmp	w1, w3
	and	w0, w0, 2147483644
	bls	.L42
	cmp	w1, w2
	csel	w1, w1, w2, ls
	sub	w1, w1, #1
	and	w3, w1, 63
.L42:
	cmp	w0, 7
	mov	w2, 7
	bls	.L43
	mov	w1, 72
	cmp	w0, w1
	csel	w0, w0, w1, ls
	sub	w0, w0, #1
	and	w2, w0, 127
.L43:
	mov	w22, 0
	bfi	w22, w3, 16, 6
	bfi	w22, w2, 0, 7
	b	.L44
	.size	VENC_HAL_CfgReg, .-VENC_HAL_CfgReg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_VEDU_DDRCfg_CABAC_VLC
	.type	VENC_HAL_VEDU_DDRCfg_CABAC_VLC, %function
VENC_HAL_VEDU_DDRCfg_CABAC_VLC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	w1, [x19, 32]
	ldr	x0, [x19, 8]
	cmp	w1, 4
	beq	.L83
	ldr	w1, [x19, 300]
	mov	w2, 0
	str	w1, [x0, 256]
	ldr	w1, [x19, 304]
	str	w1, [x0, 260]
	ldr	w1, [x19, 308]
	str	w1, [x0, 264]
	ldr	w1, [x19, 312]
	str	w1, [x0, 268]
	ldr	w1, [x19, 316]
	str	w1, [x0, 272]
	ldr	w1, [x19, 320]
	str	w1, [x0, 276]
	ldr	w1, [x19, 324]
	str	w1, [x0, 280]
	ldr	w1, [x19, 328]
	str	w1, [x0, 284]
	ldr	w1, [x19, 148]
	str	wzr, [x0, 292]
	sub	w4, w1, #1
	lsr	w3, w1, 8
	bfi	w2, w4, 16, 7
	sub	w3, w3, #1
	lsr	w1, w1, 16
	bfi	w2, w3, 8, 6
	sub	w1, w1, #1
	bfi	w2, w1, 0, 6
	str	w2, [x0, 288]
	ldr	w1, [x19, 32]
	cmp	w1, 4
	beq	.L84
.L76:
	ldr	w1, [x19, 156]
	mov	w2, 0
	str	w1, [x0, 296]
	ldr	w1, [x19, 160]
	str	w1, [x0, 300]
	ldr	w1, [x19, 164]
	str	w1, [x0, 304]
	ldr	w1, [x19, 168]
	str	w1, [x0, 308]
	ldr	w1, [x19, 172]
	str	w1, [x0, 312]
	ldr	w1, [x19, 176]
	str	w1, [x0, 316]
	ldr	w1, [x19, 180]
	str	w1, [x0, 320]
	ldr	w1, [x19, 184]
	str	w1, [x0, 324]
	ldr	w1, [x19, 152]
	sub	w4, w1, #1
	lsr	w3, w1, 8
	bfi	w2, w4, 16, 7
	sub	w3, w3, #1
	lsr	w1, w1, 16
	bfi	w2, w3, 8, 6
	sub	w1, w1, #1
	bfi	w2, w1, 0, 6
	str	w2, [x0, 328]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L83:
	ldr	w4, [x20, 44]
	mov	w1, 0
	ldr	w3, [x19, 2356]
	mov	w2, 513
	cmp	w4, wzr
	mov	w4, 9729
	csel	w2, w2, w4, eq
	bfi	w1, w3, 8, 3
	bfi	w1, w2, 16, 16
	str	w1, [x0, 256]
	mov	w3, 0
	mov	w2, 0
	ldr	w4, [x19, 260]
	mov	w1, 16
	ldr	w6, [x19, 268]
	mov	w5, 0
	bfi	w3, w4, 0, 5
	mov	w4, 0
	bfi	w3, w6, 16, 16
	str	w3, [x0, 260]
	ldr	w6, [x19, 264]
	ldr	w3, [x19, 272]
	bfi	w2, w6, 0, 5
	bfi	w2, w3, 16, 16
	str	w2, [x0, 300]
	ldr	w2, [x19, 260]
	ldr	w3, [x19, 252]
	sub	w2, w1, w2
	lsl	w2, w3, w2
	bfi	w5, w2, 0, 16
	str	w5, [x0, 264]
	ldr	w3, [x19, 264]
	ldr	w2, [x19, 256]
	sub	w1, w1, w3
	lsl	w1, w2, w1
	bfi	w4, w1, 0, 16
	str	w4, [x0, 304]
	ldr	w1, [x19, 268]
	cmp	w1, 31
	bls	.L85
	ldr	w1, [x19, 188]
	str	w1, [x0, 268]
	ldr	w1, [x19, 268]
	cmp	w1, 63
	bhi	.L72
.L88:
	ldr	w2, [x19, 192]
	mov	w3, 64
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 272]
.L73:
	ldr	w1, [x19, 268]
	cmp	w1, 64
	bls	.L74
	ldr	w2, [x19, 196]
	mov	w3, 96
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 276]
.L74:
	ldr	w1, [x19, 200]
	str	w1, [x0, 280]
	ldr	w1, [x19, 204]
	str	w1, [x0, 284]
	ldr	w1, [x19, 208]
	str	w1, [x0, 288]
	ldr	w1, [x19, 212]
	str	w1, [x0, 292]
	ldr	w1, [x19, 216]
	str	w1, [x0, 296]
	ldr	w1, [x19, 32]
	cmp	w1, 4
	bne	.L76
.L84:
	ldr	w1, [x19, 272]
	cmp	w1, 31
	bls	.L86
	ldr	w1, [x19, 220]
	str	w1, [x0, 308]
	ldr	w1, [x19, 272]
	cmp	w1, 63
	bhi	.L79
.L87:
	ldr	w2, [x19, 224]
	mov	w3, 64
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 312]
.L80:
	ldr	w1, [x19, 272]
	cmp	w1, 64
	bls	.L81
	ldr	w2, [x19, 228]
	mov	w3, 96
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 316]
.L81:
	ldr	w1, [x19, 232]
	str	w1, [x0, 320]
	ldr	w1, [x19, 236]
	str	w1, [x0, 324]
	ldr	w1, [x19, 240]
	str	w1, [x0, 328]
	ldr	w1, [x19, 244]
	str	w1, [x0, 332]
	ldr	w1, [x19, 248]
	str	w1, [x0, 336]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L86:
	ldr	w2, [x19, 220]
	mov	w3, 32
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 308]
	ldr	w1, [x19, 272]
	cmp	w1, 63
	bls	.L87
.L79:
	ldr	w1, [x19, 224]
	str	w1, [x0, 312]
	b	.L80
	.p2align 3
.L85:
	ldr	w2, [x19, 188]
	mov	w3, 32
	sub	w1, w3, w1
	lsl	w1, w2, w1
	str	w1, [x0, 268]
	ldr	w1, [x19, 268]
	cmp	w1, 63
	bls	.L88
.L72:
	ldr	w1, [x19, 192]
	str	w1, [x0, 272]
	b	.L73
	.size	VENC_HAL_VEDU_DDRCfg_CABAC_VLC, .-VENC_HAL_VEDU_DDRCfg_CABAC_VLC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_HAL_DDRCfgReg
	.type	VENC_HAL_DDRCfgReg, %function
VENC_HAL_DDRCfgReg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x23, x1
	mov	x0, x30
	bl	_mcount
	ldr	x22, [x19, 24]
	ldr	w2, [x23, 68]
	mov	w1, 11
	ldr	x20, [x19, 8]
	mov	w0, 6
	bfi	w1, w2, 10, 1
	mov	w4, 2
	orr	w1, w1, 134217728
	str	w1, [x22, 4]
	mov	w3, 0
	ldr	w2, [x19, 32]
	ldr	w1, [x23, 68]
	cmp	w2, 4
	ldr	w5, [x19, 2352]
	cset	w6, ne
	bfi	w0, w6, 3, 2
	ldr	w6, [x19, 2344]
	bfi	w0, w1, 5, 1
	ldr	w1, [x19, 2348]
	bfi	w0, w5, 6, 1
	ldr	w5, [x19, 712]
	bfi	w0, w1, 7, 1
	ldr	w1, [x23, 44]
	and	w0, w0, -16777473
	cmp	w1, wzr
	bfi	w0, w5, 9, 1
	cset	w5, eq
	cmp	w2, 4
	bfi	w0, w5, 10, 2
	ldr	w2, [x19, 2340]
	bfi	w0, w6, 12, 1
	ldr	w5, [x19, 432]
	bfi	w0, w1, 16, 1
	csel	w4, wzr, w4, ne
	orr	w0, w0, 131072
	mov	w1, 2
	bfi	w0, w2, 18, 2
	mov	w2, 0
	and	w0, w0, -6291457
	movk	w1, 0x7, lsl 16
	and	w0, w0, -33554433
	bfi	w0, w4, 26, 2
	mov	w4, 0
	bfi	w0, w5, 28, 2
	and	w0, w0, 1073741823
	str	w0, [x22, 20]
	str	wzr, [x20, 512]
	ldrh	w5, [x19, 36]
	ldrb	w0, [x19]
	bfi	w3, w5, 17, 1
	mov	w5, 0
	bfi	w3, w0, 16, 1
	ldr	w0, [x19, 40]
	bfi	w3, w0, 0, 16
	str	w3, [x20, 516]
	ldr	w3, [x23, 172]
	ldr	w0, [x19, 2220]
	bfi	w2, w3, 10, 6
	ldr	w3, [x19, 2200]
	bfi	w2, w0, 18, 1
	mov	w0, 0
	bfi	w2, w3, 19, 1
	str	w2, [x22, 32]
	str	wzr, [x22, 36]
	str	wzr, [x22, 40]
	str	wzr, [x20, 520]
	ldr	w3, [x19, 424]
	ldr	w2, [x19, 428]
	bfi	w1, w3, 19, 2
	bfi	w1, w2, 21, 1
	and	w1, w1, -8388609
	str	w1, [x22, 48]
	ldp	w1, w2, [x23, 12]
	sub	w2, w2, #1
	sub	w1, w1, #1
	bfi	w4, w2, 16, 13
	bfi	w4, w1, 0, 13
	str	w4, [x20]
	str	w5, [x22, 56]
	ldr	w1, [x19, 440]
	str	w1, [x22, 60]
	ldr	w2, [x19, 448]
	ldr	w1, [x19, 452]
	bfi	w0, w2, 4, 4
	bfi	w0, w1, 0, 4
	str	w0, [x22, 64]
	ldr	w0, [x19, 32]
	cmp	w0, 4
	beq	.L116
	cbz	w0, .L117
.L94:
	mov	w2, 3
	mov	w0, 0
	mov	w1, 1
	bfi	w0, w2, 0, 3
	bfi	w0, w1, 3, 2
	str	w1, [x20, 524]
	stp	w24, w2, [x20, 16]
	bfi	w0, w2, 5, 3
	mov	w3, 0
	bfi	w0, w2, 8, 3
	str	w0, [x22, 100]
	mov	w1, 0
	mov	w6, 0
	ldr	w0, [x19, 420]
	mov	w4, 0
	str	w0, [x20, 128]
	ldr	w0, [x19, 76]
	str	w0, [x20, 132]
	ldr	w0, [x19, 80]
	str	w0, [x20, 136]
	ldr	w0, [x19, 84]
	str	w0, [x20, 140]
	ldr	w0, [x19, 60]
	bfi	w3, w0, 16, 16
	ldr	w0, [x19, 64]
	bfi	w3, w0, 0, 16
	str	w3, [x20, 152]
	mov	w3, 0
	mov	w0, 0
	ldr	w5, [x23, 52]
	add	x5, x19, x5, lsl 2
	ldr	w5, [x5, 136]
	str	w5, [x20, 156]
	ldr	w5, [x23, 48]
	add	x5, x19, x5, lsl 2
	ldr	w5, [x5, 88]
	str	w5, [x20, 160]
	ldr	w5, [x23, 48]
	add	x5, x5, 24
	ldr	w5, [x19, x5, lsl 2]
	str	w5, [x20, 164]
	ldr	w5, [x19, 284]
	bfi	w1, w5, 0, 16
	ldr	w5, [x19, 288]
	bfi	w1, w5, 16, 16
	str	w1, [x20, 168]
	ldr	w1, [x23, 48]
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 104]
	str	w1, [x20, 172]
	ldr	w1, [x23, 48]
	add	x1, x1, 28
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 176]
	ldr	w1, [x19, 292]
	bfi	w6, w1, 0, 16
	str	w6, [x20, 180]
	ldr	w1, [x23, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 20
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 8]
	str	w1, [x20, 184]
	ldr	w1, [x23, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 24
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 188]
	ldr	w5, [x19, 288]
	ldr	w1, [x19, 284]
	bfi	w4, w5, 0, 16
	bfi	w4, w1, 16, 16
	str	w4, [x20, 192]
	ldr	w1, [x23, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 24
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 8]
	str	w1, [x20, 196]
	ldr	w1, [x23, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 28
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 200]
	ldr	w4, [x19, 292]
	ldr	w1, [x19, 296]
	bfi	w3, w4, 16, 16
	bfi	w3, w1, 0, 16
	str	w3, [x20, 204]
	ldr	w1, [x23, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 32
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 208]
	ldr	w1, [x23, 48]
	add	x1, x1, 32
	ldr	w1, [x19, x1, lsl 2]
	str	w1, [x20, 212]
	ldr	w1, [x23, 48]
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 120]
	str	w1, [x20, 48]
	ldr	w1, [x23, 48]
	cmp	w1, wzr
	cset	x1, eq
	add	x1, x1, 28
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 8]
	str	w1, [x20, 52]
	ldr	w1, [x19, 44]
	str	w1, [x20, 56]
	ldr	w1, [x19, 52]
	str	w1, [x20, 60]
	ldr	w1, [x19, 48]
	str	w1, [x20, 64]
	ldr	w1, [x19, 68]
	cmp	w1, 2
	beq	.L95
	cmp	w1, 1
	beq	.L110
	and	w0, w0, -961
	and	w0, w0, -16385
	cbnz	w1, .L96
	ldr	w1, [x19, 72]
	cmp	w1, wzr
	cset	w1, eq
.L97:
	bfi	w0, w1, 16, 8
	str	w0, [x20, 4]
	mov	w0, 0
	mov	w2, 5
	ldr	w4, [x19, 2336]
	mov	w1, 0
	ldr	w3, [x19, 2332]
	bfi	w1, w2, 24, 8
	bfi	w0, w4, 0, 3
	mov	w5, 15
	bfi	w0, w3, 4, 1
	bfi	w1, w2, 16, 8
	and	w0, w0, -33
	mov	w2, 3
	orr	w0, w0, 1792
	bfi	w1, w5, 8, 8
	orr	w0, w0, 28672
	str	w0, [x20, 528]
	mov	w0, 500
	str	w2, [x20, 72]
	bfi	w1, w5, 0, 8
	mov	w2, 27
	str	w1, [x20, 544]
	movk	w0, 0x64, lsl 16
	str	w2, [x20, 532]
	mov	w1, 0
	str	w0, [x20, 536]
	mov	w0, 0
	mov	w2, -128
	mov	w7, 2
	ldr	w4, [x19, 724]
	mov	w6, 128
	ldr	w3, [x19, 720]
	bfi	w1, w4, 0, 1
	ldr	w4, [x19, 716]
	bfi	w1, w3, 1, 1
	mov	w3, 0
	bfi	w1, w4, 2, 1
	bfi	w3, w2, 0, 8
	and	w1, w1, -25
	str	w1, [x22, 352]
	mov	w4, 0
	bfi	w3, w2, 8, 8
	ldr	w1, [x19, 16]
	bfi	w4, w7, 16, 8
	str	w1, [x22, 392]
	bfi	w3, w2, 16, 8
	bfi	w4, w7, 24, 8
	ldr	w1, [x19, 2192]
	bfi	w0, w1, 0, 1
	ldr	w1, [x19, 2196]
	bfi	w0, w1, 1, 1
	ldr	w1, [x19, 2208]
	bfi	w0, w1, 2, 1
	ldr	w1, [x19, 2216]
	bfi	w0, w1, 4, 1
	ldr	w1, [x19, 2188]
	bfi	w0, w1, 8, 1
	ldr	w1, [x19, 2184]
	bfi	w0, w1, 9, 1
	ldr	w1, [x19, 2264]
	bfi	w0, w1, 10, 1
	ldr	w1, [x19, 2268]
	bfi	w0, w1, 11, 1
	ldr	w1, [x19, 2272]
	bfi	w0, w1, 12, 1
	ldr	w1, [x19, 2180]
	bfi	w0, w1, 13, 1
	ldr	w1, [x19, 2176]
	bfi	w0, w1, 14, 1
	ldr	w1, [x19, 2276]
	bfi	w0, w1, 15, 1
	mov	w1, 0
	str	w0, [x22, 440]
	mov	w0, 0
	bfi	w1, w2, 0, 8
	bfi	w0, w6, 0, 10
	bfi	w1, w2, 8, 8
	bfi	w0, w6, 10, 10
	str	w4, [x20, 540]
	str	w3, [x20, 1760]
	str	w0, [x20, 1768]
	mov	w0, 43691
	str	wzr, [x20, 32]
	movk	w0, 0xaaaa, lsl 16
	str	w1, [x20, 1764]
	ldr	w1, [x23, 52]
	add	w1, w1, w7
	umull	x2, w1, w0
	lsr	x2, x2, 32
	lsr	w2, w2, 1
	lsl	w3, w2, 2
	sub	w2, w3, w2
	mov	w3, 1
	sub	w1, w1, w2
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 136]
	str	w1, [x20, 216]
	ldr	w1, [x23, 52]
	add	w1, w1, w3
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 1
	lsl	w2, w0, 2
	sub	w0, w2, w0
	sub	w0, w1, w0
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 136]
	str	w0, [x20, 220]
	str	w3, [x22, 492]
	ldrb	w7, [x19, 472]
	ldrb	w10, [x19, 484]
	ldrb	w9, [x19, 480]
	ldrb	w8, [x19, 476]
	ldrb	w2, [x19, 468]
	orr	w1, w10, w9
	ldrb	w6, [x19, 464]
	orr	w0, w8, w7
	ldrb	w3, [x19, 456]
	orr	w1, w1, w0
	ldrb	w4, [x19, 460]
	orr	w0, w2, w6
	orr	w1, w1, w0
	ldr	w11, [x19, 708]
	orr	w0, w4, w3
	orr	w1, w1, w0
	mov	w0, 0
	bfi	w0, w1, 24, 1
	ldr	w1, [x19, 704]
	bfi	w0, w11, 23, 1
	bfi	w0, w1, 22, 1
	ldr	w1, [x19, 700]
	bfi	w0, w1, 21, 1
	ldr	w1, [x19, 696]
	bfi	w0, w1, 20, 1
	ldr	w1, [x19, 692]
	bfi	w0, w1, 19, 1
	ldr	w1, [x19, 688]
	bfi	w0, w1, 18, 1
	ldr	w1, [x19, 684]
	bfi	w0, w1, 17, 1
	ldr	w1, [x19, 680]
	bfi	w0, w1, 16, 1
	ldr	w1, [x19, 516]
	bfi	w0, w1, 15, 1
	ldr	w1, [x19, 512]
	bfi	w0, w1, 14, 1
	ldr	w1, [x19, 508]
	bfi	w0, w1, 13, 1
	ldr	w1, [x19, 504]
	bfi	w0, w1, 12, 1
	ldr	w1, [x19, 500]
	bfi	w0, w1, 11, 1
	ldr	w1, [x19, 496]
	bfi	w0, w1, 10, 1
	ldr	w1, [x19, 492]
	bfi	w0, w1, 9, 1
	ldr	w1, [x19, 488]
	bfi	w0, w1, 8, 1
	mov	w1, 0
	bfi	w0, w10, 7, 1
	bfi	w0, w9, 6, 1
	bfi	w0, w8, 5, 1
	bfi	w0, w7, 4, 1
	mov	w7, 0
	bfi	w0, w2, 3, 1
	mov	w2, 0
	bfi	w0, w6, 2, 1
	mov	w6, 0
	bfi	w0, w4, 1, 1
	bfi	w0, w3, 0, 1
	str	w0, [x20, 1024]
	ldr	w4, [x19, 532]
	ldr	w0, [x19, 528]
	ldr	w3, [x19, 524]
	bfi	w2, w4, 24, 7
	bfi	w2, w0, 16, 7
	ldr	w0, [x19, 520]
	bfi	w2, w3, 8, 7
	mov	w4, 0
	bfi	w2, w0, 0, 7
	str	w2, [x20, 1028]
	mov	w2, 0
	mov	w3, 0
	ldr	w0, [x19, 548]
	bfi	w1, w0, 24, 7
	ldr	w0, [x19, 544]
	bfi	w1, w0, 16, 7
	ldr	w0, [x19, 540]
	bfi	w1, w0, 8, 7
	ldr	w0, [x19, 536]
	bfi	w1, w0, 0, 7
	str	w1, [x20, 1032]
	mov	w0, 0
	ldr	w1, [x19, 584]
	bfi	w6, w1, 16, 10
	ldr	w1, [x19, 552]
	bfi	w6, w1, 0, 10
	str	w6, [x20, 1036]
	ldr	w1, [x19, 588]
	bfi	w2, w1, 16, 10
	ldr	w1, [x19, 556]
	bfi	w2, w1, 0, 10
	str	w2, [x20, 1040]
	mov	w1, 0
	mov	w2, 0
	ldr	w6, [x19, 592]
	bfi	w4, w6, 16, 10
	ldr	w6, [x19, 560]
	bfi	w4, w6, 0, 10
	str	w4, [x20, 1044]
	ldr	w4, [x19, 596]
	bfi	w0, w4, 16, 10
	ldr	w4, [x19, 564]
	bfi	w0, w4, 0, 10
	str	w0, [x20, 1048]
	mov	w0, 0
	mov	w4, 0
	ldr	w6, [x19, 600]
	bfi	w3, w6, 16, 10
	ldr	w6, [x19, 568]
	bfi	w3, w6, 0, 10
	str	w3, [x20, 1052]
	ldr	w3, [x19, 604]
	bfi	w1, w3, 16, 10
	ldr	w3, [x19, 572]
	bfi	w1, w3, 0, 10
	str	w1, [x20, 1056]
	mov	w1, 0
	mov	w3, 0
	ldr	w6, [x19, 608]
	bfi	w2, w6, 16, 10
	ldr	w6, [x19, 576]
	bfi	w2, w6, 0, 10
	str	w2, [x20, 1060]
	ldr	w2, [x19, 612]
	bfi	w0, w2, 16, 10
	ldr	w2, [x19, 580]
	bfi	w0, w2, 0, 10
	str	w0, [x20, 1064]
	mov	w0, 0
	mov	w2, 0
	ldr	w6, [x19, 648]
	bfi	w4, w6, 16, 9
	ldr	w6, [x19, 616]
	bfi	w4, w6, 0, 9
	str	w4, [x20, 1068]
	ldr	w4, [x19, 652]
	bfi	w1, w4, 16, 9
	ldr	w4, [x19, 620]
	bfi	w1, w4, 0, 9
	str	w1, [x20, 1072]
	mov	w1, 0
	mov	w4, 0
	ldr	w6, [x19, 656]
	bfi	w3, w6, 16, 9
	ldr	w6, [x19, 624]
	bfi	w3, w6, 0, 9
	str	w3, [x20, 1076]
	ldr	w3, [x19, 660]
	bfi	w0, w3, 16, 9
	ldr	w3, [x19, 628]
	bfi	w0, w3, 0, 9
	str	w0, [x20, 1080]
	mov	w3, 0
	mov	w0, 64
	ldr	w6, [x19, 664]
	bfi	w2, w6, 16, 9
	ldr	w6, [x19, 632]
	bfi	w2, w6, 0, 9
	str	w2, [x20, 1084]
	mov	w6, 0
	ldr	w2, [x19, 668]
	bfi	w1, w2, 16, 9
	ldr	w2, [x19, 636]
	bfi	w1, w2, 0, 9
	str	w1, [x20, 1088]
	mov	w1, 16
	ldr	w2, [x19, 672]
	bfi	w4, w2, 16, 9
	ldr	w2, [x19, 640]
	bfi	w4, w2, 0, 9
	str	w4, [x20, 1092]
	mov	w4, 0
	ldr	w2, [x19, 676]
	bfi	w3, w2, 16, 9
	ldr	w2, [x19, 644]
	bfi	w3, w2, 0, 9
	str	w3, [x20, 1096]
	mov	w3, 0
	ldr	w2, [x19, 32]
	cmp	w2, wzr
	csel	w2, w0, w1, ne
	ldp	w0, w1, [x23, 12]
	sub	w0, w0, #1
	sub	w1, w1, #1
	add	w0, w0, w2
	add	w1, w1, w2
	udiv	w0, w0, w2
	udiv	w1, w1, w2
	ldr	w2, [x23, 168]
	mul	w0, w0, w1
	udiv	w0, w2, w0
	mov	w2, 0
	bfi	w7, w0, 0, 20
	str	w7, [x22, 588]
	mov	w7, 0
	ldr	w1, [x19, 728]
	ldr	w0, [x19, 732]
	bfi	w6, w1, 16, 5
	bfi	w6, w0, 0, 10
	str	w6, [x20, 1536]
	mov	w0, 0
	ldr	w1, [x19, 736]
	bfi	w4, w1, 16, 5
	ldr	w1, [x19, 740]
	bfi	w4, w1, 0, 8
	str	w4, [x20, 1540]
	mov	w1, 0
	mov	w4, 0
	ldr	w6, [x19, 744]
	bfi	w3, w6, 16, 5
	ldr	w6, [x19, 748]
	bfi	w3, w6, 0, 10
	str	w3, [x20, 1544]
	ldr	w3, [x19, 752]
	bfi	w0, w3, 16, 5
	ldr	w3, [x19, 756]
	bfi	w0, w3, 0, 8
	str	w0, [x20, 1548]
	mov	w0, 0
	mov	w3, 0
	ldr	w6, [x19, 760]
	bfi	w2, w6, 16, 5
	ldr	w6, [x19, 764]
	bfi	w2, w6, 0, 10
	str	w2, [x20, 1552]
	ldr	w2, [x19, 768]
	bfi	w1, w2, 16, 5
	ldr	w2, [x19, 772]
	bfi	w1, w2, 0, 8
	str	w1, [x20, 1556]
	mov	w2, 0
	mov	w1, 0
	ldr	w6, [x19, 776]
	bfi	w4, w6, 16, 5
	ldr	w6, [x19, 780]
	bfi	w4, w6, 0, 10
	str	w4, [x20, 1560]
	ldr	w4, [x19, 784]
	bfi	w0, w4, 16, 5
	ldr	w4, [x19, 788]
	bfi	w0, w4, 0, 8
	str	w0, [x20, 1564]
	mov	w0, 0
	mov	w4, 0
	ldr	w6, [x19, 792]
	bfi	w3, w6, 12, 4
	ldr	w6, [x19, 796]
	bfi	w3, w6, 8, 4
	ldr	w6, [x19, 800]
	bfi	w3, w6, 4, 4
	ldr	w6, [x19, 804]
	bfi	w3, w6, 0, 4
	ldr	w6, [x19, 808]
	bfi	w3, w6, 28, 4
	ldr	w6, [x19, 812]
	bfi	w3, w6, 24, 4
	ldr	w6, [x19, 816]
	bfi	w3, w6, 20, 4
	ldr	w6, [x19, 820]
	bfi	w3, w6, 16, 4
	str	w3, [x20, 1568]
	ldr	w3, [x19, 824]
	bfi	w2, w3, 12, 4
	ldr	w3, [x19, 828]
	bfi	w2, w3, 8, 4
	ldr	w3, [x19, 832]
	bfi	w2, w3, 4, 4
	ldr	w3, [x19, 836]
	bfi	w2, w3, 0, 4
	ldr	w3, [x19, 840]
	bfi	w2, w3, 28, 4
	ldr	w3, [x19, 844]
	bfi	w2, w3, 24, 4
	ldr	w3, [x19, 848]
	bfi	w2, w3, 20, 4
	ldr	w3, [x19, 852]
	bfi	w2, w3, 16, 4
	str	w2, [x20, 1572]
	mov	w3, 0
	mov	w2, 0
	ldr	w6, [x19, 856]
	bfi	w1, w6, 12, 4
	ldr	w6, [x19, 860]
	bfi	w1, w6, 8, 4
	ldr	w6, [x19, 864]
	bfi	w1, w6, 4, 4
	ldr	w6, [x19, 868]
	bfi	w1, w6, 0, 4
	ldr	w6, [x19, 872]
	bfi	w1, w6, 28, 4
	ldr	w6, [x19, 876]
	bfi	w1, w6, 24, 4
	ldr	w6, [x19, 880]
	bfi	w1, w6, 20, 4
	ldr	w6, [x19, 884]
	bfi	w1, w6, 16, 4
	str	w1, [x20, 1576]
	ldr	w1, [x19, 888]
	bfi	w0, w1, 12, 4
	ldr	w1, [x19, 892]
	bfi	w0, w1, 8, 4
	ldr	w1, [x19, 896]
	bfi	w0, w1, 4, 4
	ldr	w1, [x19, 900]
	bfi	w0, w1, 0, 4
	ldr	w1, [x19, 904]
	bfi	w0, w1, 28, 4
	ldr	w1, [x19, 908]
	bfi	w0, w1, 24, 4
	ldr	w1, [x19, 912]
	bfi	w0, w1, 20, 4
	ldr	w1, [x19, 916]
	bfi	w0, w1, 16, 4
	str	w0, [x20, 1580]
	mov	w1, 0
	mov	w0, 0
	ldr	w6, [x19, 920]
	bfi	w4, w6, 12, 4
	ldr	w6, [x19, 924]
	bfi	w4, w6, 8, 4
	ldr	w6, [x19, 928]
	bfi	w4, w6, 4, 4
	ldr	w6, [x19, 932]
	bfi	w4, w6, 0, 4
	ldr	w6, [x19, 936]
	bfi	w4, w6, 28, 4
	ldr	w6, [x19, 940]
	bfi	w4, w6, 24, 4
	ldr	w6, [x19, 944]
	bfi	w4, w6, 20, 4
	ldr	w6, [x19, 948]
	bfi	w4, w6, 16, 4
	str	w4, [x20, 1584]
	mov	w6, 0
	ldr	w4, [x19, 952]
	bfi	w3, w4, 12, 4
	ldr	w4, [x19, 956]
	bfi	w3, w4, 8, 4
	ldr	w4, [x19, 960]
	bfi	w3, w4, 4, 4
	ldr	w4, [x19, 964]
	bfi	w3, w4, 0, 4
	ldr	w4, [x19, 968]
	bfi	w3, w4, 28, 4
	ldr	w4, [x19, 972]
	bfi	w3, w4, 24, 4
	ldr	w4, [x19, 976]
	bfi	w3, w4, 20, 4
	ldr	w4, [x19, 980]
	bfi	w3, w4, 16, 4
	str	w3, [x20, 1588]
	mov	w3, 0
	ldr	w4, [x19, 984]
	bfi	w2, w4, 12, 4
	ldr	w4, [x19, 988]
	bfi	w2, w4, 8, 4
	ldr	w4, [x19, 992]
	bfi	w2, w4, 4, 4
	ldr	w4, [x19, 996]
	bfi	w2, w4, 0, 4
	ldr	w4, [x19, 1000]
	bfi	w2, w4, 28, 4
	ldr	w4, [x19, 1004]
	bfi	w2, w4, 24, 4
	ldr	w4, [x19, 1008]
	bfi	w2, w4, 20, 4
	ldr	w4, [x19, 1012]
	bfi	w2, w4, 16, 4
	str	w2, [x20, 1592]
	mov	w4, 0
	ldr	w2, [x19, 1016]
	bfi	w1, w2, 12, 4
	ldr	w2, [x19, 1020]
	bfi	w1, w2, 8, 4
	ldr	w2, [x19, 1024]
	bfi	w1, w2, 4, 4
	ldr	w2, [x19, 1028]
	bfi	w1, w2, 0, 4
	ldr	w2, [x19, 1032]
	bfi	w1, w2, 28, 4
	ldr	w2, [x19, 1036]
	bfi	w1, w2, 24, 4
	ldr	w2, [x19, 1040]
	bfi	w1, w2, 20, 4
	ldr	w2, [x19, 1044]
	bfi	w1, w2, 16, 4
	str	w1, [x20, 1596]
	mov	w1, 0
	ldr	w2, [x19, 1048]
	bfi	w0, w2, 12, 4
	ldr	w2, [x19, 1052]
	bfi	w0, w2, 8, 4
	ldr	w2, [x19, 1056]
	bfi	w0, w2, 4, 4
	ldr	w2, [x19, 1060]
	bfi	w0, w2, 0, 4
	ldr	w2, [x19, 1064]
	bfi	w0, w2, 28, 4
	ldr	w2, [x19, 1068]
	bfi	w0, w2, 24, 4
	ldr	w2, [x19, 1072]
	bfi	w0, w2, 20, 4
	ldr	w2, [x19, 1076]
	bfi	w0, w2, 16, 4
	str	w0, [x20, 1600]
	mov	w2, 0
	ldr	w0, [x19, 1080]
	bfi	w3, w0, 0, 3
	ldr	w0, [x19, 1084]
	bfi	w3, w0, 3, 7
	ldr	w0, [x19, 1088]
	bfi	w3, w0, 10, 3
	ldr	w0, [x19, 1092]
	bfi	w3, w0, 13, 5
	ldr	w0, [x19, 1096]
	bfi	w3, w0, 18, 4
	ldr	w0, [x19, 1100]
	bfi	w3, w0, 22, 4
	ldr	w0, [x19, 1104]
	bfi	w3, w0, 26, 4
	str	w3, [x20, 1604]
	mov	w3, 0
	ldr	w0, [x19, 1108]
	bfi	w6, w0, 0, 3
	ldr	w0, [x19, 1112]
	bfi	w6, w0, 3, 7
	ldr	w0, [x19, 1116]
	bfi	w6, w0, 10, 3
	ldr	w0, [x19, 1120]
	bfi	w6, w0, 13, 5
	ldr	w0, [x19, 1124]
	bfi	w6, w0, 18, 4
	ldr	w0, [x19, 1128]
	bfi	w6, w0, 22, 4
	ldr	w0, [x19, 1132]
	bfi	w6, w0, 26, 4
	str	w6, [x20, 1608]
	ldr	w0, [x19, 1136]
	bfi	w1, w0, 0, 3
	ldr	w0, [x19, 1140]
	bfi	w1, w0, 3, 7
	ldr	w0, [x19, 1144]
	bfi	w1, w0, 10, 3
	ldr	w0, [x19, 1148]
	bfi	w1, w0, 13, 5
	ldr	w0, [x19, 1152]
	bfi	w1, w0, 18, 4
	ldr	w0, [x19, 1156]
	bfi	w1, w0, 22, 4
	ldr	w0, [x19, 1160]
	bfi	w1, w0, 26, 4
	str	w1, [x20, 1612]
	mov	w1, 0
	mov	w0, 0
	ldr	w6, [x19, 1436]
	bfi	w4, w6, 0, 3
	ldr	w6, [x19, 1440]
	bfi	w4, w6, 3, 7
	ldr	w6, [x19, 1444]
	bfi	w4, w6, 10, 3
	ldr	w6, [x19, 1448]
	bfi	w4, w6, 13, 5
	ldr	w6, [x19, 1452]
	bfi	w4, w6, 18, 4
	ldr	w6, [x19, 1456]
	bfi	w4, w6, 22, 4
	ldr	w6, [x19, 1460]
	bfi	w4, w6, 26, 4
	str	w4, [x20, 1616]
	mov	w6, 0
	ldr	w4, [x19, 1464]
	bfi	w3, w4, 0, 3
	ldr	w4, [x19, 1468]
	bfi	w3, w4, 3, 7
	ldr	w4, [x19, 1472]
	bfi	w3, w4, 10, 3
	ldr	w4, [x19, 1476]
	bfi	w3, w4, 13, 5
	ldr	w4, [x19, 1480]
	bfi	w3, w4, 18, 4
	ldr	w4, [x19, 1484]
	bfi	w3, w4, 22, 4
	ldr	w4, [x19, 1488]
	bfi	w3, w4, 26, 4
	str	w3, [x20, 1620]
	mov	w3, 0
	ldr	w4, [x19, 1492]
	bfi	w2, w4, 0, 3
	ldr	w4, [x19, 1496]
	bfi	w2, w4, 3, 7
	ldr	w4, [x19, 1500]
	bfi	w2, w4, 10, 3
	ldr	w4, [x19, 1504]
	bfi	w2, w4, 13, 5
	ldr	w4, [x19, 1508]
	bfi	w2, w4, 18, 4
	ldr	w4, [x19, 1512]
	bfi	w2, w4, 22, 4
	ldr	w4, [x19, 1516]
	bfi	w2, w4, 26, 4
	str	w2, [x20, 1624]
	mov	w4, 0
	ldr	w2, [x19, 1520]
	bfi	w1, w2, 0, 3
	ldr	w2, [x19, 1524]
	bfi	w1, w2, 3, 7
	ldr	w2, [x19, 1528]
	bfi	w1, w2, 10, 3
	ldr	w2, [x19, 1532]
	bfi	w1, w2, 13, 5
	ldr	w2, [x19, 1536]
	bfi	w1, w2, 18, 4
	ldr	w2, [x19, 1540]
	bfi	w1, w2, 22, 4
	ldr	w2, [x19, 1544]
	bfi	w1, w2, 26, 4
	str	w1, [x20, 1628]
	mov	w2, 0
	ldr	w1, [x19, 1164]
	bfi	w0, w1, 0, 4
	ldr	w1, [x19, 1168]
	bfi	w0, w1, 4, 4
	ldr	w1, [x19, 1172]
	bfi	w0, w1, 8, 4
	ldr	w1, [x19, 1176]
	bfi	w0, w1, 12, 4
	ldr	w1, [x19, 1180]
	bfi	w0, w1, 16, 4
	ldr	w1, [x19, 1184]
	bfi	w0, w1, 20, 4
	ldr	w1, [x19, 1188]
	bfi	w0, w1, 24, 4
	ldr	w1, [x19, 1192]
	bfi	w0, w1, 28, 4
	str	w0, [x20, 1632]
	mov	w1, 0
	ldr	w0, [x19, 1196]
	bfi	w3, w0, 0, 4
	ldr	w0, [x19, 1200]
	bfi	w3, w0, 4, 4
	ldr	w0, [x19, 1204]
	bfi	w3, w0, 8, 4
	ldr	w0, [x19, 1208]
	bfi	w3, w0, 12, 4
	ldr	w0, [x19, 1212]
	bfi	w3, w0, 16, 4
	ldr	w0, [x19, 1216]
	bfi	w3, w0, 20, 4
	str	w3, [x20, 1636]
	mov	w3, 0
	ldr	w0, [x19, 1220]
	bfi	w6, w0, 0, 10
	ldr	w0, [x19, 1224]
	bfi	w6, w0, 16, 5
	str	w6, [x20, 1640]
	ldr	w0, [x19, 1228]
	bfi	w2, w0, 0, 8
	ldr	w0, [x19, 1232]
	bfi	w2, w0, 16, 5
	str	w2, [x20, 1644]
	mov	w2, 0
	mov	w0, 0
	ldr	w6, [x19, 1236]
	bfi	w4, w6, 0, 10
	ldr	w6, [x19, 1240]
	bfi	w4, w6, 16, 5
	str	w4, [x20, 1648]
	mov	w6, 0
	ldr	w4, [x19, 1244]
	bfi	w1, w4, 0, 8
	ldr	w4, [x19, 1248]
	bfi	w1, w4, 16, 5
	str	w1, [x20, 1652]
	mov	w1, 0
	ldr	w4, [x19, 1252]
	bfi	w3, w4, 0, 10
	ldr	w4, [x19, 1256]
	bfi	w3, w4, 16, 5
	str	w3, [x20, 1656]
	ldr	w3, [x19, 1260]
	bfi	w2, w3, 0, 8
	ldr	w3, [x19, 1264]
	bfi	w2, w3, 16, 5
	str	w2, [x20, 1660]
	mov	w3, 0
	mov	w2, 0
	ldr	w4, [x19, 1548]
	bfi	w0, w4, 0, 4
	ldr	w4, [x19, 1552]
	bfi	w0, w4, 4, 4
	ldr	w4, [x19, 1556]
	bfi	w0, w4, 8, 4
	ldr	w4, [x19, 1560]
	bfi	w0, w4, 12, 4
	ldr	w4, [x19, 1564]
	bfi	w0, w4, 16, 4
	ldr	w4, [x19, 1568]
	bfi	w0, w4, 20, 4
	str	w0, [x20, 1664]
	mov	w4, 0
	ldr	w0, [x19, 1572]
	bfi	w1, w0, 0, 12
	ldr	w0, [x19, 1576]
	bfi	w1, w0, 12, 12
	ldr	w0, [x19, 1580]
	bfi	w1, w0, 24, 8
	str	w1, [x20, 1668]
	mov	w1, 0
	ldr	w0, [x19, 1584]
	bfi	w6, w0, 0, 12
	ldr	w0, [x19, 1588]
	bfi	w6, w0, 12, 12
	ldr	w0, [x19, 1592]
	bfi	w6, w0, 24, 8
	str	w6, [x20, 1672]
	ldr	w0, [x19, 1596]
	bfi	w3, w0, 0, 12
	ldr	w0, [x19, 1600]
	bfi	w3, w0, 12, 12
	ldr	w0, [x19, 1604]
	bfi	w3, w0, 24, 8
	str	w3, [x20, 1676]
	mov	w0, 0
	mov	w3, 0
	ldr	w6, [x19, 1608]
	bfi	w2, w6, 0, 8
	ldr	w6, [x19, 1612]
	bfi	w2, w6, 8, 8
	ldr	w6, [x19, 1616]
	bfi	w2, w6, 16, 8
	str	w2, [x20, 1680]
	ldr	w2, [x19, 1268]
	bfi	w1, w2, 0, 1
	ldr	w2, [x19, 1272]
	bfi	w1, w2, 1, 3
	ldr	w2, [x19, 1276]
	bfi	w1, w2, 4, 6
	ldr	w2, [x19, 1280]
	bfi	w1, w2, 10, 5
	ldr	w2, [x19, 1284]
	bfi	w1, w2, 15, 6
	ldr	w2, [x19, 1288]
	bfi	w1, w2, 21, 5
	str	w1, [x20, 1684]
	mov	w2, 0
	mov	w1, 0
	ldr	w6, [x19, 1292]
	bfi	w4, w6, 0, 1
	ldr	w6, [x19, 1296]
	bfi	w4, w6, 1, 3
	ldr	w6, [x19, 1300]
	bfi	w4, w6, 4, 6
	ldr	w6, [x19, 1304]
	bfi	w4, w6, 10, 5
	ldr	w6, [x19, 1308]
	bfi	w4, w6, 15, 6
	ldr	w6, [x19, 1312]
	bfi	w4, w6, 21, 5
	str	w4, [x20, 1688]
	ldr	w4, [x19, 1316]
	bfi	w0, w4, 0, 1
	ldr	w4, [x19, 1320]
	bfi	w0, w4, 1, 3
	ldr	w4, [x19, 1324]
	bfi	w0, w4, 4, 6
	ldr	w4, [x19, 1328]
	bfi	w0, w4, 10, 5
	ldr	w4, [x19, 1332]
	bfi	w0, w4, 15, 6
	ldr	w4, [x19, 1336]
	bfi	w0, w4, 21, 5
	str	w0, [x20, 1692]
	mov	w0, 0
	mov	w4, 0
	ldr	w6, [x19, 1340]
	bfi	w3, w6, 0, 1
	ldr	w6, [x19, 1344]
	bfi	w3, w6, 1, 3
	ldr	w6, [x19, 1348]
	bfi	w3, w6, 4, 6
	ldr	w6, [x19, 1352]
	bfi	w3, w6, 10, 5
	ldr	w6, [x19, 1356]
	bfi	w3, w6, 15, 6
	ldr	w6, [x19, 1360]
	bfi	w3, w6, 21, 5
	str	w3, [x20, 1696]
	mov	w6, 0
	ldr	w3, [x19, 1364]
	bfi	w2, w3, 0, 1
	ldr	w3, [x19, 1368]
	bfi	w2, w3, 1, 3
	ldr	w3, [x19, 1372]
	bfi	w2, w3, 4, 6
	ldr	w3, [x19, 1376]
	bfi	w2, w3, 10, 5
	ldr	w3, [x19, 1380]
	bfi	w2, w3, 15, 6
	ldr	w3, [x19, 1384]
	bfi	w2, w3, 21, 5
	str	w2, [x20, 1700]
	mov	w2, -16711936
	ldr	w3, [x19, 1388]
	bfi	w1, w3, 0, 1
	ldr	w3, [x19, 1392]
	bfi	w1, w3, 1, 3
	ldr	w3, [x19, 1396]
	bfi	w1, w3, 4, 6
	ldr	w3, [x19, 1400]
	bfi	w1, w3, 10, 5
	ldr	w3, [x19, 1404]
	bfi	w1, w3, 15, 6
	ldr	w3, [x19, 1408]
	bfi	w1, w3, 21, 5
	str	w1, [x20, 1704]
	mov	w3, 0
	ldr	w1, [x19, 1412]
	bfi	w0, w1, 0, 1
	ldr	w1, [x19, 1416]
	bfi	w0, w1, 1, 3
	ldr	w1, [x19, 1420]
	bfi	w0, w1, 4, 6
	ldr	w1, [x19, 1424]
	bfi	w0, w1, 10, 5
	ldr	w1, [x19, 1428]
	bfi	w0, w1, 15, 6
	ldr	w1, [x19, 1432]
	bfi	w0, w1, 21, 5
	str	w0, [x20, 1708]
	mov	w0, 0
	ldr	w1, [x19, 416]
	bfi	w4, w1, 20, 8
	ldr	w1, [x23, 68]
	bfi	w4, w1, 28, 1
	stp	w4, w2, [x20, 8]
	mov	w2, 0
	ldr	w1, [x19, 2388]
	bfi	w7, w1, 0, 1
	str	w7, [x20, 1824]
	mov	w7, 0
	ldr	w1, [x19, 2392]
	bfi	w0, w1, 0, 16
	str	w0, [x20, 1828]
	mov	w1, 0
	mov	w0, 0
	ldr	w4, [x19, 2396]
	bfi	w6, w4, 0, 16
	str	w6, [x20, 1832]
	mov	w6, 3
	ldr	w4, [x19, 2400]
	bfi	w3, w4, 0, 16
	str	w3, [x20, 1836]
	mov	w4, 20
	ldr	w3, [x19, 2404]
	bfi	w2, w3, 18, 9
	ldr	w3, [x19, 2408]
	bfi	w2, w3, 9, 9
	ldr	w3, [x19, 2412]
	bfi	w2, w3, 0, 9
	str	w2, [x20, 1844]
	mov	w3, 0
	ldr	w2, [x19, 2416]
	bfi	w1, w2, 16, 8
	ldr	w2, [x19, 2420]
	bfi	w1, w2, 8, 8
	ldr	w2, [x19, 2424]
	bfi	w1, w2, 0, 8
	str	w1, [x20, 1848]
	mov	w1, 0
	ldr	w2, [x19, 2428]
	bfi	w0, w2, 8, 4
	ldr	w2, [x19, 2432]
	bfi	w0, w2, 4, 4
	ldr	w2, [x19, 2436]
	bfi	w0, w2, 0, 4
	mov	w2, 0
	bfi	w0, w6, 20, 8
	bfi	w0, w4, 12, 8
	str	w0, [x20, 1852]
	ldrh	w0, [x19, 2568]
	str	w0, [x20, 1856]
	ldr	w4, [x19, 2584]
	ldr	w0, [x19, 2580]
	bfi	w1, w4, 0, 8
	ldr	w4, [x19, 2576]
	bfi	w1, w0, 8, 8
	ldr	w0, [x19, 2572]
	bfi	w1, w4, 16, 8
	bfi	w1, w0, 24, 8
	str	w1, [x20, 1860]
	mov	w0, 0
	mov	w1, 0
	ldr	w4, [x19, 2444]
	bfi	w3, w4, 24, 8
	ldr	w4, [x19, 2448]
	bfi	w3, w4, 16, 8
	ldr	w4, [x19, 2452]
	bfi	w3, w4, 8, 8
	ldr	w4, [x19, 2456]
	bfi	w3, w4, 0, 8
	str	w3, [x20, 1872]
	mov	w4, 0
	mov	w3, 0
	ldr	w6, [x19, 2460]
	bfi	w2, w6, 0, 8
	ldr	w6, [x19, 2464]
	bfi	w2, w6, 8, 8
	ldr	w6, [x19, 2468]
	bfi	w2, w6, 16, 1
	ldr	w6, [x19, 2612]
	bfi	w2, w6, 17, 1
	str	w2, [x20, 1876]
	mov	w6, 0
	ldr	w2, [x19, 2472]
	bfi	w0, w2, 0, 16
	ldr	w2, [x19, 2476]
	bfi	w0, w2, 16, 16
	str	w0, [x20, 1880]
	mov	w0, 0
	ldr	w2, [x19, 2480]
	bfi	w1, w2, 22, 9
	ldr	w2, [x19, 2484]
	bfi	w1, w2, 17, 5
	ldr	w2, [x19, 2488]
	bfi	w1, w2, 9, 8
	ldr	w2, [x19, 2440]
	bfi	w1, w2, 0, 9
	str	w1, [x20, 1884]
	mov	w2, 0
	ldr	w1, [x19, 2492]
	bfi	w4, w1, 8, 14
	ldr	w1, [x19, 2496]
	bfi	w4, w1, 0, 8
	str	w4, [x20, 1792]
	mov	w4, 0
	ldr	w1, [x19, 2500]
	bfi	w3, w1, 16, 8
	ldr	w1, [x19, 2504]
	bfi	w3, w1, 8, 8
	ldr	w1, [x19, 2508]
	bfi	w3, w1, 0, 8
	str	w3, [x20, 1796]
	ldr	w1, [x19, 2516]
	bfi	w0, w1, 17, 1
	ldr	w1, [x19, 2520]
	bfi	w0, w1, 12, 2
	ldr	w1, [x19, 2524]
	bfi	w0, w1, 4, 4
	ldr	w1, [x19, 2528]
	bfi	w0, w1, 0, 4
	str	w0, [x20, 1800]
	mov	w1, 0
	ldr	w0, [x19, 2532]
	bfi	w6, w0, 0, 10
	ldr	w0, [x19, 2536]
	bfi	w6, w0, 16, 9
	str	w6, [x20, 1808]
	ldr	w0, [x19, 2540]
	bfi	w4, w0, 0, 10
	ldr	w0, [x19, 2544]
	bfi	w4, w0, 16, 9
	str	w4, [x20, 1812]
	mov	w0, 0
	mov	w4, 0
	ldr	w3, [x19, 2548]
	bfi	w2, w3, 0, 10
	ldr	w3, [x19, 2552]
	bfi	w2, w3, 16, 9
	str	w2, [x20, 1816]
	mov	w3, 0
	ldr	w2, [x19, 2556]
	bfi	w1, w2, 0, 10
	ldr	w2, [x19, 2560]
	bfi	w1, w2, 16, 9
	str	w1, [x20, 1820]
	mov	w2, 0
	ldr	w1, [x19, 2564]
	bfi	w7, w1, 0, 16
	str	w7, [x20, 1804]
	ldr	w1, [x19, 2588]
	bfi	w0, w1, 0, 13
	ldr	w1, [x19, 2592]
	bfi	w0, w1, 13, 1
	str	w0, [x20, 36]
	mov	w1, 0
	mov	w0, 0
	ldr	w6, [x19, 2596]
	bfi	w4, w6, 0, 18
	str	w4, [x20, 40]
	ldr	w4, [x19, 2600]
	bfi	w3, w4, 0, 18
	str	w3, [x20, 44]
	mov	w3, 257
	ldr	w4, [x19, 2624]
	movk	w3, 0xc8, lsl 16
	bfi	w2, w4, 16, 7
	ldr	w4, [x19, 2628]
	bfi	w2, w4, 23, 7
	str	w2, [x22, 1792]
	ldr	w4, [x19, 2616]
	ldr	w2, [x19, 2620]
	bfi	w1, w4, 0, 8
	bfi	w1, w2, 8, 8
	ldr	w2, [x19, 2604]
	bfi	w1, w2, 16, 8
	str	w1, [x22, 1796]
	ldr	w4, [x19, 2608]
	ldr	w2, [x19, 2632]
	ldr	w1, [x19, 2636]
	bfi	w0, w4, 12, 8
	bfi	w0, w2, 8, 4
	bfi	w0, w1, 0, 8
	str	w0, [x22, 1800]
	str	w3, [x22, 1804]
	ldr	w0, [x19, 32]
	cbnz	w0, .L99
	mov	w1, 23
	bfi	w0, w1, 0, 5
	bfi	w0, w5, 5, 5
	bfi	w0, w1, 10, 5
	bfi	w0, w5, 15, 5
	and	w0, w0, -1048577
	str	w0, [x20, 24]
.L100:
	mov	w2, 16
	mov	w0, 0
	bfi	w0, w2, 6, 6
	mov	w3, 10
	str	wzr, [x20, 552]
	bfi	w0, w3, 0, 6
	mov	w1, 2
	str	w0, [x20, 556]
	str	w1, [x20, 548]
	mov	w1, 64
	mov	w4, 51
	mov	w3, 4
	ldp	w5, w0, [x23, 4]
	mov	w7, 0
	mov	w6, 0
	ubfiz	w0, w0, 8, 6
	orr	w0, w0, w1
	bfi	w0, w5, 0, 6
	mov	w5, 0
	bfi	w0, w4, 16, 6
	mov	w4, 0
	bfi	w0, w3, 24, 4
	str	w0, [x22, 3076]
	mov	w3, 0
	ldr	w0, [x19, 32]
	cmp	w0, wzr
	csel	w2, w1, w2, ne
	ldp	w0, w1, [x23, 12]
	sub	w0, w0, #1
	sub	w1, w1, #1
	add	w0, w0, w2
	add	w1, w1, w2
	udiv	w0, w0, w2
	udiv	w1, w1, w2
	ldr	w2, [x23, 168]
	mul	w0, w0, w1
	ldr	w1, [x19, 1800]
	udiv	w0, w2, w0
	mov	w2, 0
	bfi	w5, w0, 0, 16
	bfi	w5, w1, 16, 6
	str	w5, [x22, 3080]
	mov	w1, 0
	ldr	w5, [x19, 1832]
	mov	w0, 0
	bfi	w4, w5, 0, 6
	ldr	w5, [x19, 1792]
	bfi	w4, w5, 24, 6
	ldr	w5, [x19, 1796]
	bfi	w4, w5, 16, 6
	ldr	w5, [x19, 1804]
	bfi	w4, w5, 8, 6
	str	w4, [x22, 3084]
	mov	w4, 0
	ldr	w5, [x19, 1652]
	bfi	w3, w5, 24, 8
	ldr	w5, [x19, 1648]
	bfi	w3, w5, 16, 8
	ldr	w5, [x19, 1644]
	bfi	w3, w5, 8, 8
	ldr	w5, [x19, 1640]
	bfi	w3, w5, 0, 8
	str	w3, [x22, 3088]
	mov	w5, 0
	ldr	w3, [x19, 1668]
	bfi	w2, w3, 24, 8
	ldr	w3, [x19, 1664]
	bfi	w2, w3, 16, 8
	ldr	w3, [x19, 1660]
	bfi	w2, w3, 8, 8
	ldr	w3, [x19, 1656]
	bfi	w2, w3, 0, 8
	str	w2, [x22, 3092]
	mov	w2, 0
	ldr	w3, [x19, 1684]
	bfi	w1, w3, 24, 8
	ldr	w3, [x19, 1680]
	bfi	w1, w3, 16, 8
	ldr	w3, [x19, 1676]
	bfi	w1, w3, 8, 8
	ldr	w3, [x19, 1672]
	bfi	w1, w3, 0, 8
	str	w1, [x22, 3096]
	mov	w3, 0
	ldr	w1, [x19, 1700]
	bfi	w0, w1, 24, 8
	ldr	w1, [x19, 1696]
	bfi	w0, w1, 16, 8
	ldr	w1, [x19, 1692]
	bfi	w0, w1, 8, 8
	ldr	w1, [x19, 1688]
	bfi	w0, w1, 0, 8
	str	w0, [x22, 3072]
	add	x1, x19, 1836
	ldr	w0, [x19, 1824]
	bfi	w4, w0, 0, 4
	ldr	w0, [x19, 1828]
	bfi	w4, w0, 4, 4
	str	w4, [x22, 3100]
	mov	w4, 0
	ldr	w0, [x19, 1776]
	bfi	w7, w0, 0, 4
	str	w7, [x22, 3104]
	ldr	w0, [x19, 1772]
	bfi	w2, w0, 0, 4
	ldr	w0, [x19, 1780]
	bfi	w2, w0, 4, 4
	str	w2, [x22, 3108]
	add	x2, x20, 1104
	ldr	w0, [x19, 1784]
	bfi	w6, w0, 0, 4
	str	w6, [x22, 3112]
	add	x0, x19, 1900
	ldr	w6, [x19, 1704]
	bfi	w3, w6, 0, 2
	ldr	w6, [x19, 1708]
	bfi	w3, w6, 2, 2
	ldr	w6, [x19, 1712]
	bfi	w3, w6, 4, 2
	ldr	w6, [x19, 1716]
	bfi	w3, w6, 6, 2
	ldr	w6, [x19, 1720]
	bfi	w3, w6, 8, 2
	ldr	w6, [x19, 1724]
	bfi	w3, w6, 10, 2
	ldr	w6, [x19, 1728]
	bfi	w3, w6, 12, 2
	ldr	w6, [x19, 1732]
	bfi	w3, w6, 14, 2
	ldr	w6, [x19, 1736]
	bfi	w3, w6, 16, 2
	ldr	w6, [x19, 1740]
	bfi	w3, w6, 18, 2
	ldr	w6, [x19, 1744]
	bfi	w3, w6, 20, 2
	ldr	w6, [x19, 1748]
	bfi	w3, w6, 22, 2
	ldr	w6, [x19, 1752]
	bfi	w3, w6, 24, 2
	ldr	w6, [x19, 1756]
	bfi	w3, w6, 26, 2
	ldr	w6, [x19, 1760]
	bfi	w3, w6, 28, 2
	ldr	w6, [x19, 1764]
	bfi	w3, w6, 30, 2
	str	w3, [x22, 3116]
	ldr	w3, [x19, 1768]
	bfi	w5, w3, 0, 5
	str	w5, [x22, 3120]
	ldr	w5, [x19, 1812]
	ldr	w3, [x19, 1816]
	bfi	w4, w5, 24, 4
	ldr	w5, [x19, 1820]
	bfi	w4, w3, 28, 4
	ldr	w3, [x19, 1788]
	bfi	w4, w5, 16, 6
	bfi	w4, w3, 8, 4
	ldr	w3, [x19, 1808]
	bfi	w4, w3, 0, 6
	str	w4, [x22, 3124]
	.p2align 2
.L102:
	ldr	w3, [x1], 4
	bfi	w21, w3, 0, 20
	cmp	x1, x0
	str	w21, [x2], 4
	bne	.L102
	add	x1, x20, 1280
	add	x3, x19, 2156
	.p2align 2
.L103:
	ldr	w2, [x0], 4
	bfi	w21, w2, 0, 20
	cmp	x3, x0
	str	w21, [x1], 4
	bne	.L103
	add	x5, x20, 512
	add	x6, x20, 512
	str	wzr, [x22, 3660]
	mov	w1, 13
	mov	w0, -1
	mov	w4, 0
	stp	wzr, w1, [x5, 60]
	stp	wzr, wzr, [x5, 52]
	stp	wzr, wzr, [x6, 68]
	stp	wzr, wzr, [x6, 76]
	str	w0, [x20, 596]
	mov	w3, 0
	str	w0, [x20, 604]
	mov	w2, 0
	str	w0, [x20, 612]
	mov	x1, x23
	str	w0, [x20, 620]
	mov	w0, 7
	str	w0, [x20, 600]
	str	w0, [x20, 608]
	str	w0, [x20, 616]
	str	w0, [x20, 624]
	mov	w0, 0
	ldr	w5, [x19, 2360]
	bfi	w4, w5, 0, 1
	str	w4, [x20, 120]
	ldr	w4, [x19, 2364]
	str	w4, [x20, 96]
	ldr	w4, [x19, 2368]
	str	w4, [x20, 100]
	ldr	w4, [x19, 2372]
	str	w4, [x20, 104]
	ldr	w4, [x19, 2376]
	str	w4, [x20, 108]
	ldr	w4, [x19, 2380]
	str	w4, [x20, 112]
	ldr	w4, [x19, 2384]
	str	w4, [x20, 116]
	ldr	w5, [x19, 1620]
	ldr	w4, [x19, 1624]
	bfi	w0, w5, 0, 16
	bfi	w0, w4, 16, 16
	str	w0, [x20, 1728]
	mov	x0, x19
	ldr	w4, [x19, 1636]
	bfi	w3, w4, 16, 16
	str	w3, [x20, 1732]
	ldr	w4, [x19, 1628]
	ldr	w3, [x19, 1632]
	bfi	w2, w4, 0, 16
	bfi	w2, w3, 16, 16
	str	w2, [x20, 1736]
	bl	VENC_HAL_VEDU_DDRCfg_CABAC_VLC
	ldr	w0, [x19, 56]
	mov	w3, -85
	str	w0, [x20, 68]
	mov	w0, 0
	str	w0, [x22, 9232]
	mov	w1, 85
	bfi	w0, w3, 0, 8
	mov	w4, 0
	ldr	w2, [x19, 444]
	bfi	w0, w1, 8, 8
	str	w2, [x22, 9236]
	mov	w2, 0
	mov	w3, 0
	mov	w5, 0
	ldr	w1, [x23, 44]
	mov	w6, 0
	cmp	w1, wzr
	cset	w1, ne
	str	w1, [x20, 76]
	ldr	w1, [x19, 276]
	stp	w1, wzr, [x20, 80]
	str	w0, [x20, 28]
	mov	w0, 0
	stp	wzr, wzr, [x20, 88]
	ldr	w7, [x19, 2156]
	ldr	w1, [x19, 2160]
	bfi	w4, w7, 0, 6
	ldr	w7, [x19, 2164]
	bfi	w4, w1, 8, 2
	ldr	w1, [x19, 2168]
	bfi	w4, w7, 10, 1
	ldr	w7, [x19, 2172]
	bfi	w4, w1, 12, 4
	mov	w1, 0
	bfi	w4, w7, 16, 5
	str	w4, [x22, 12240]
	ldr	w4, [x19, 2224]
	str	w4, [x22, 12244]
	ldr	w4, [x19, 2228]
	str	w4, [x22, 12248]
	ldr	w7, [x19, 2232]
	ldr	w4, [x19, 2236]
	bfi	w2, w7, 0, 16
	ldr	w7, [x19, 2240]
	bfi	w2, w4, 16, 2
	ldr	w4, [x19, 2244]
	bfi	w2, w7, 18, 2
	ldr	w7, [x19, 2248]
	bfi	w2, w4, 20, 4
	ldr	w4, [x19, 2256]
	bfi	w2, w7, 24, 1
	ldr	w7, [x19, 2260]
	bfi	w2, w4, 25, 2
	ldr	w4, [x19, 2252]
	bfi	w2, w7, 27, 1
	bfi	w2, w4, 28, 1
	str	w2, [x22, 12252]
	add	x4, x22, 61440
	mov	w2, 0
	ldr	w7, [x19, 2292]
	bfi	w3, w7, 0, 1
	ldr	w7, [x19, 2288]
	bfi	w3, w7, 2, 2
	ldr	w7, [x19, 2284]
	bfi	w3, w7, 4, 2
	ldr	w7, [x19, 2280]
	bfi	w3, w7, 6, 1
	ldr	w7, [x19, 2300]
	bfi	w3, w7, 8, 6
	ldr	w7, [x19, 2304]
	bfi	w3, w7, 24, 6
	str	w3, [x22, 12256]
	mov	w7, 3
	mov	w3, 1
	ldr	w8, [x19, 2308]
	bfi	w5, w8, 0, 6
	ldr	w8, [x19, 2312]
	bfi	w5, w8, 8, 6
	ldr	w8, [x19, 2316]
	bfi	w5, w8, 16, 6
	ldr	w8, [x19, 2320]
	bfi	w5, w8, 24, 6
	str	w5, [x22, 12260]
	ldr	w5, [x19, 2324]
	bfi	w6, w5, 0, 6
	ldr	w5, [x19, 2328]
	bfi	w6, w5, 6, 25
	str	w6, [x22, 12264]
	ldr	w6, [x19, 336]
	ldr	w5, [x19, 352]
	bfi	w0, w6, 10, 1
	ldr	w6, [x19, 332]
	bfi	w0, w5, 9, 1
	ldr	w5, [x19, 356]
	bfi	w0, w6, 8, 1
	ldr	w6, [x19, 360]
	bfi	w0, w5, 7, 1
	ldr	w5, [x19, 344]
	bfi	w0, w6, 6, 1
	ldr	w6, [x19, 340]
	bfi	w0, w5, 5, 1
	ldr	w5, [x19, 364]
	bfi	w0, w6, 4, 1
	ldr	w6, [x19, 368]
	bfi	w0, w5, 3, 1
	ldr	w5, [x19, 372]
	bfi	w0, w6, 2, 1
	bfi	w0, w5, 1, 1
	ldr	w5, [x19, 376]
	bfi	w0, w5, 0, 1
	str	w0, [x22, 504]
	ldr	w5, [x19, 348]
	ldr	w0, [x19, 380]
	ldr	w6, [x19, 384]
	bfi	w1, w5, 7, 1
	bfi	w1, w0, 6, 1
	ldr	w5, [x19, 388]
	ldr	w0, [x19, 392]
	bfi	w1, w6, 5, 1
	bfi	w1, w5, 4, 1
	ldr	w6, [x19, 396]
	bfi	w1, w0, 3, 1
	ldr	w5, [x19, 400]
	ldr	w0, [x19, 404]
	bfi	w1, w6, 2, 1
	bfi	w1, w5, 1, 1
	bfi	w1, w0, 0, 1
	str	w1, [x22, 508]
	ldr	w1, [x19, 408]
	ldr	w0, [x19, 412]
	bfi	w2, w1, 0, 1
	bfi	w2, w0, 3, 1
	bfi	w2, w7, 16, 4
	str	w2, [x4]
	str	wzr, [x4, 32]
	ldr	x0, [x19, 24]
	ldr	w1, [x19, 2640]
	add	x0, x0, 61440
	str	w1, [x0, 520]
	ldr	w1, [x19, 2640]
	str	w1, [x0, 524]
	ldr	w1, [x19, 2648]
	str	w1, [x0, 752]
	ldr	w1, [x19, 2644]
	str	w1, [x0, 756]
	ldr	w1, [x19, 2648]
	str	w1, [x0, 772]
	ldr	w1, [x19, 2644]
	str	w1, [x0, 776]
	str	wzr, [x4, 8]
	str	wzr, [x22, 16]
	str	w3, [x22, 16]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L99:
	mov	w0, 0
	bfi	w0, w5, 0, 5
	bfi	w0, w5, 5, 5
	bfi	w0, w5, 10, 5
	bfi	w0, w5, 15, 5
	and	w0, w0, -1048577
	str	w0, [x20, 24]
	b	.L100
.L110:
	mov	w2, 6
.L95:
	bfi	w0, w2, 6, 4
	and	w0, w0, -16385
.L96:
	ldrb	w1, [x19, 72]
	b	.L97
.L117:
	mov	w0, 15
	mov	w24, 0
	bfi	w24, w0, 16, 6
	mov	w0, 31
	bfi	w24, w0, 0, 7
	b	.L94
.L116:
	ldp	w0, w1, [x23, 12]
	mov	w2, 32
	mov	w4, 72
	lsr	w1, w1, 2
	lsr	w0, w0, 2
	sub	w1, w1, #16
	sub	w0, w0, #16
	lsr	w1, w1, 1
	lsr	w0, w0, 1
	cmp	w1, w2
	mov	w3, 7
	csel	w1, w1, w2, ls
	cmp	w0, w4
	and	w1, w1, 2147483644
	csel	w0, w0, w4, ls
	cmp	w1, w3
	and	w0, w0, 2147483644
	bls	.L92
	cmp	w1, w2
	csel	w1, w1, w2, ls
	sub	w1, w1, #1
	and	w3, w1, 63
.L92:
	cmp	w0, 7
	mov	w2, 7
	bls	.L93
	mov	w1, 72
	cmp	w0, w1
	csel	w0, w0, w1, ls
	sub	w0, w0, #1
	and	w2, w0, 127
.L93:
	mov	w24, 0
	bfi	w24, w3, 16, 6
	bfi	w24, w2, 0, 7
	b	.L94
	.size	VENC_HAL_DDRCfgReg, .-VENC_HAL_DDRCfgReg
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
