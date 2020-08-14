#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_vp8.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VP8HAL_V400R003_InitHal
	.type	VP8HAL_V400R003_InitHal, %function
VP8HAL_V400R003_InitHal:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	VP8HAL_V400R003_InitHal, .-VP8HAL_V400R003_InitHal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8HAL_V400R003_CfgReg
	.type	VP8HAL_V400R003_CfgReg, %function
VP8HAL_V400R003_CfgReg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	mov	w22, w2
	mov	x19, x3
	bl	_mcount
	str	wzr, [x29, 60]
	cbz	w22, .L4
	cmp	w22, 1
	bne	.L12
	ldr	x2, .LC2
	mov	w4, 0
	ldr	x1, .LC4
	mov	w3, w22
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L6
	.p2align 3
.L4:
	ldr	x0, [x21]
	cbz	x0, .L13
.L7:
	ldr	w5, [x20, 2788]
	mov	x3, x19
	ldr	w4, [x20, 2784]
	mov	w0, 8
	ldr	w1, [x29, 60]
	mov	w2, 0
	mul	w4, w4, w5
	sub	w4, w4, #1
	bfi	w1, w4, 0, 20
	str	w1, [x29, 60]
	ubfx	x1, x1, 24, 6
	strb	w1, [x29, 63]
	ldr	w22, [x29, 60]
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC8
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w0, [x20, 3144]
	mov	w1, 0
	ldr	w2, [x20, 3140]
	mov	x3, x19
	bfi	w1, w0, 4, 1
	mov	w0, 8192
	orr	w1, w1, -64
	strh	w0, [x29, 62]
	strb	w1, [x29, 61]
	mov	w0, 12
	ubfiz	w1, w2, 6, 1
	strb	w0, [x29, 60]
	strb	w1, [x29, 63]
	mov	w2, 0
	ldr	w22, [x29, 60]
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC10
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w22, [x21, 72]
	mov	x3, x19
	mov	w0, 16
	and	w22, w22, -16
	mov	w2, 0
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC12
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w22, [x21, 56]
	mov	x3, x19
	mov	w0, 20
	and	w22, w22, -16
	mov	w2, 0
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC14
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w22, [x20, 2812]
	mov	x3, x19
	mov	w0, 24
	mov	w1, w22
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC16
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w22, [x21, 1180]
	mov	x3, x19
	mov	w0, 128
	and	w22, w22, -16
	mov	w2, 0
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC18
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x21, 1184]
	mov	x3, x19
	mov	w0, 132
	mov	w1, w21
	mov	w2, 0
	str	w21, [x29, 60]
	bl	MFDE_ConfigReg
	ldr	x1, .LC20
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w0, [x20, 2784]
	mov	x3, x19
	mov	w2, 0
	cmp	w0, 256
	mov	w0, 4
	cset	w1, ls
	bl	SCD_ConfigReg
	ldr	w21, [x20, 3116]
	mov	x3, x19
	mov	w0, 96
	and	w21, w21, -16
	mov	w2, 0
	mov	w1, w21
	bl	MFDE_ConfigReg
	ldr	x1, .LC22
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x20, 2792]
	mov	x3, x19
	mov	w0, 100
	mov	w1, w21
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC24
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x20, 3148]
	mov	x3, x19
	mov	w0, 104
	mov	w2, 0
	mov	w1, w21
	bl	MFDE_ConfigReg
	ldr	x1, .LC26
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w1, [x20, 3156]
	mov	x3, x19
	mov	w2, 0
	mov	w0, 108
	bl	MFDE_ConfigReg
	mov	w0, 32
	mov	x3, x19
	mov	w2, 0
	mov	w1, -1
	bl	MFDE_ConfigReg
	mov	w0, 0
.L6:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L12:
	ldr	x1, .LC1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L13:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L8
	str	x0, [x21]
	b	.L7
.L8:
	ldr	x1, .LC6
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L6
	.size	VP8HAL_V400R003_CfgReg, .-VP8HAL_V400R003_CfgReg
	.align	3
.LC1:
	.xword	.LC0
	.align	3
.LC2:
	.xword	.LANCHOR0
	.align	3
.LC4:
	.xword	.LC3
	.align	3
.LC6:
	.xword	.LC5
	.align	3
.LC8:
	.xword	.LC7
	.align	3
.LC10:
	.xword	.LC9
	.align	3
.LC12:
	.xword	.LC11
	.align	3
.LC14:
	.xword	.LC13
	.align	3
.LC16:
	.xword	.LC15
	.align	3
.LC18:
	.xword	.LC17
	.align	3
.LC20:
	.xword	.LC19
	.align	3
.LC22:
	.xword	.LC21
	.align	3
.LC24:
	.xword	.LC23
	.align	3
.LC26:
	.xword	.LC25
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8HAL_V400R003_CfgDnMsg
	.type	VP8HAL_V400R003_CfgDnMsg, %function
VP8HAL_V400R003_CfgDnMsg:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x23, x1
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x23, 72]
	bl	MEM_Phy2Vir
	mov	x22, x0
	cbz	x0, .L22
	ldr	w0, [x19, 2796]
	mov	w2, 0
	ldr	w3, [x19, 2800]
	str	wzr, [x29, 76]
	bfi	w2, w0, 0, 1
	bfi	w2, w3, 1, 2
	strb	w2, [x29, 76]
	ldr	x1, .LC35
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x22]
	bl	dprint_vfmw
	ldr	w2, [x19, 2808]
	mov	w0, 4
	ldr	w3, [x19, 2804]
	lsl	w2, w2, 1
	ldr	x1, .LC37
	sub	w2, w2, w3
	add	w2, w2, 1
	str	w2, [x22, 4]
	bl	dprint_vfmw
	ldr	w1, [x19, 2784]
	mov	w3, 0
	ldr	w0, [x19, 2788]
	mov	w2, 0
	sub	w1, w1, #1
	sub	w0, w0, #1
	bfi	w3, w1, 0, 9
	bfi	w2, w0, 0, 9
	strh	w3, [x29, 76]
	strh	w2, [x29, 78]
	mov	w0, 4
	ldr	x1, .LC39
	ldr	w2, [x29, 76]
	str	w2, [x22, 8]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w2, [x19, 2753]
	mov	w0, 0
	ldrb	w1, [x19, 2754]
	bfi	w0, w2, 0, 1
	ldrb	w2, [x19, 2752]
	bfi	w0, w1, 1, 2
	strb	w2, [x29, 76]
	strb	w0, [x29, 77]
	mov	w0, 4
	ldr	x1, .LC41
	ldr	w2, [x29, 76]
	str	w2, [x22, 12]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w0, [x19, 2755]
	mov	w2, 0
	ldrb	w1, [x19, 2756]
	bfi	w2, w0, 0, 1
	ldrb	w0, [x19, 2757]
	ldrb	w3, [x19, 2758]
	bfi	w2, w1, 1, 1
	bfi	w2, w0, 2, 1
	ldr	x1, .LC43
	bfi	w2, w3, 3, 1
	strb	w2, [x29, 76]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x22, 16]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w2, [x19, 2760]
	mov	w0, 0
	ldrb	w1, [x19, 2761]
	bfi	w0, w2, 0, 1
	ldrb	w5, [x19, 2762]
	bfi	w0, w1, 1, 2
	strb	w0, [x29, 76]
	ldrb	w4, [x19, 2763]
	mov	w0, 4
	ldrh	w2, [x29, 76]
	ldrb	w3, [x19, 2764]
	bfi	w2, w5, 3, 6
	strh	w2, [x29, 76]
	lsr	x2, x2, 8
	ldr	x1, .LC45
	bfi	w2, w4, 1, 3
	bfi	w2, w3, 4, 3
	strb	w2, [x29, 77]
	ldr	w2, [x29, 76]
	str	w2, [x22, 20]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w2, [x19, 2765]
	mov	w0, 0
	ldrb	w1, [x19, 2766]
	bfi	w0, w2, 0, 1
	ldrb	w2, [x19, 2767]
	bfi	w0, w1, 1, 4
	ldrb	w1, [x29, 78]
	bfi	w0, w2, 5, 1
	strb	w0, [x29, 76]
	ldrb	w0, [x19, 2772]
	ldrb	w6, [x19, 2768]
	ldrb	w4, [x19, 2773]
	bfi	w1, w0, 0, 4
	ldrh	w2, [x29, 76]
	ldrb	w3, [x19, 2769]
	bfi	w1, w4, 4, 1
	bfi	w2, w6, 6, 4
	strb	w1, [x29, 78]
	ldrb	w5, [x19, 2770]
	lsr	x0, x2, 8
	ldrb	w4, [x19, 2774]
	bfi	w0, w3, 2, 1
	ldrh	w1, [x29, 78]
	bfi	w0, w5, 3, 4
	ldrb	w3, [x19, 2771]
	strh	w2, [x29, 76]
	bfi	w1, w4, 5, 4
	bfi	w0, w3, 7, 1
	strh	w1, [x29, 78]
	strb	w0, [x29, 77]
	mov	w0, 4
	ldr	x1, .LC47
	ldr	w2, [x29, 76]
	str	w2, [x22, 24]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w1, [x19, 2759]
	mov	w0, 0
	bfi	w0, w1, 0, 7
	strb	w0, [x29, 76]
	ldr	x1, .LC49
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x22, 28]
	bl	dprint_vfmw
	ldr	w2, [x19, 2780]
	mov	w0, 4
	ldr	x1, .LC51
	str	w2, [x22, 32]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w1, [x19, 2779]
	mov	w0, 0
	ldrb	w2, [x19, 2778]
	bfi	w0, w1, 0, 6
	strb	w2, [x29, 76]
	strb	w0, [x29, 78]
	mov	w0, 4
	ldr	x1, .LC53
	ldr	w2, [x29, 76]
	str	w2, [x22, 36]
	bl	dprint_vfmw
	ldr	w20, [x19, 2824]
	ldrb	w0, [x19, 2779]
	ldr	w3, [x19, 2820]
	ldr	w2, [x19, 2816]
	cmp	w0, w3
	add	w2, w0, w2
	bls	.L23
	add	w3, w3, 128
	sub	w20, w20, #16
	sub	w3, w3, w0
.L18:
	mov	w0, 0
	ldr	x1, .LC55
	bfi	w0, w2, 0, 25
	str	w0, [x29, 76]
	lsr	w2, w0, 24
	mov	w0, 4
	bfi	w2, w3, 1, 7
	strb	w2, [x29, 79]
	ldr	w2, [x29, 76]
	str	w2, [x22, 64]
	bl	dprint_vfmw
	mov	w0, 0
	ldr	x1, .LC57
	bfi	w0, w20, 0, 24
	str	w0, [x29, 76]
	mov	w2, w0
	str	w0, [x22, 68]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w1, [x19, 2828]
	mov	w0, 0
	ldr	w2, [x19, 2832]
	bfi	w0, w1, 0, 25
	str	w0, [x29, 76]
	lsr	w0, w0, 24
	ldr	x1, .LC59
	bfi	w0, w2, 1, 7
	strb	w0, [x29, 79]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x22, 72]
	bl	dprint_vfmw
	ldr	w2, [x19, 2836]
	mov	w0, 0
	ldr	x1, .LC61
	bfi	w0, w2, 0, 24
	str	w0, [x29, 76]
	mov	w2, w0
	str	w0, [x22, 76]
	mov	w0, 4
	bl	dprint_vfmw
	add	x4, x19, 2816
	ldp	w3, w2, [x4, 220]
	ldp	w1, w0, [x4, 228]
	strb	w1, [x29, 78]
	strb	w3, [x29, 76]
	strb	w0, [x29, 79]
	mov	w0, 4
	strb	w2, [x29, 77]
	ldr	x1, .LC63
	ldr	w2, [x29, 76]
	str	w2, [x22, 80]
	bl	dprint_vfmw
	add	x5, x19, 2816
	ldp	w3, w2, [x5, 236]
	ldp	w1, w0, [x5, 244]
	strb	w1, [x29, 78]
	strb	w3, [x29, 76]
	strb	w0, [x29, 79]
	mov	w0, 4
	strb	w2, [x29, 77]
	ldr	x1, .LC65
	ldr	w2, [x29, 76]
	str	w2, [x22, 84]
	bl	dprint_vfmw
	add	x6, x19, 3072
	ldp	w3, w2, [x6, -4]
	ldp	w1, w0, [x6, 4]
	strb	w1, [x29, 78]
	strb	w3, [x29, 76]
	strb	w0, [x29, 79]
	mov	w0, 4
	strb	w2, [x29, 77]
	ldr	x1, .LC67
	ldr	w2, [x29, 76]
	str	w2, [x22, 88]
	bl	dprint_vfmw
	add	x7, x19, 3072
	ldp	w3, w2, [x7, 12]
	ldp	w1, w0, [x7, 20]
	strb	w1, [x29, 78]
	strb	w3, [x29, 76]
	strb	w0, [x29, 79]
	mov	w0, 4
	strb	w2, [x29, 77]
	ldr	x1, .LC69
	ldr	w2, [x29, 76]
	str	w2, [x22, 92]
	bl	dprint_vfmw
	add	x8, x19, 3072
	ldp	w1, w0, [x8, 36]
	ldp	w3, w2, [x8, 28]
	strb	w3, [x29, 76]
	strb	w1, [x29, 78]
	strb	w0, [x29, 79]
	mov	w0, 4
	strb	w2, [x29, 77]
	ldr	x1, .LC71
	ldr	w2, [x29, 76]
	str	w2, [x22, 96]
	bl	dprint_vfmw
	ldr	w2, [x19, 3136]
	mov	w0, 4
	ldr	x1, .LC73
	str	w2, [x22, 100]
	bl	dprint_vfmw
	ldr	w2, [x19, 3116]
	mov	w0, 4
	ldr	x1, .LC75
	str	w2, [x22, 128]
	bl	dprint_vfmw
	ldr	w2, [x19, 3120]
	mov	w0, 4
	ldr	x1, .LC77
	str	w2, [x22, 132]
	bl	dprint_vfmw
	ldr	w2, [x19, 3124]
	mov	w0, 4
	ldr	x1, .LC79
	str	w2, [x22, 136]
	bl	dprint_vfmw
	ldr	w2, [x19, 3128]
	mov	w0, 4
	ldr	x1, .LC81
	str	w2, [x22, 140]
	bl	dprint_vfmw
	ldr	w2, [x23, 1160]
	mov	w0, 4
	ldr	x1, .LC83
	and	w2, w2, -16
	str	w2, [x22, 144]
	bl	dprint_vfmw
	ldr	w2, [x23, 1164]
	mov	w0, 4
	ldr	x1, .LC85
	and	w2, w2, -16
	str	w2, [x22, 148]
	bl	dprint_vfmw
	ldr	w2, [x23, 1168]
	mov	w0, 4
	ldr	x1, .LC87
	and	w2, w2, -16
	str	w2, [x22, 152]
	bl	dprint_vfmw
	ldr	w20, [x23, 1204]
	ldr	x1, .LC89
	mov	w0, 4
	and	w20, w20, -16
	str	w20, [x22, 156]
	mov	w2, w20
	str	w20, [x29, 76]
	bl	dprint_vfmw
	mov	w0, w20
	bl	MEM_Phy2Vir
	cbz	x0, .L24
	ldr	x3, .LC92
	mov	x1, x19
	mov	x2, 2752
	add	x21, x22, 252
	add	x19, x19, 2844
	mov	w20, 64
	ldr	x3, [x3, 104]
	blr	x3
	ldr	w2, [x23, 1176]
	mov	w0, 4
	ldr	x1, .LC94
	and	w2, w2, -16
	str	w2, [x22, 160]
	bl	dprint_vfmw
	mov	x0, x22
	bl	MEM_Vir2Phy
	add	w2, w0, 256
	ldr	x1, .LC96
	mov	w0, 4
	str	w2, [x22, 252]
	bl	dprint_vfmw
	ldr	x22, .LC98
	.p2align 2
.L20:
	ldr	w1, [x19]
	mov	w0, 0
	ldr	w3, [x19, 32]
	mov	w2, w20
	bfi	w0, w1, 0, 25
	str	w0, [x29, 76]
	lsr	w0, w0, 24
	add	x19, x19, 4
	bfi	w0, w3, 1, 7
	strb	w0, [x29, 79]
	mov	x1, x22
	mov	w0, 4
	ldr	w3, [x29, 76]
	add	x23, x21, 16
	str	w3, [x21, 4]
	bl	dprint_vfmw
	ldr	w0, [x19, 60]
	mov	w3, 0
	add	w2, w20, 1
	mov	x1, x22
	bfi	w3, w0, 0, 24
	str	w3, [x29, 76]
	str	w3, [x21, 8]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w2, [x19, 92]
	mov	w0, 0
	ldr	w1, [x19, 124]
	bfi	w0, w2, 0, 25
	str	w0, [x29, 76]
	lsr	w0, w0, 24
	add	w2, w20, 2
	bfi	w0, w1, 1, 7
	strb	w0, [x29, 79]
	mov	x1, x22
	mov	w0, 4
	ldr	w3, [x29, 76]
	str	w3, [x21, 12]
	bl	dprint_vfmw
	ldr	w0, [x19, 156]
	mov	w3, 0
	add	w2, w20, 3
	mov	x1, x22
	bfi	w3, w0, 0, 24
	str	w3, [x29, 76]
	mov	w0, 4
	str	w3, [x21, 16]
	add	w20, w20, w0
	mov	x21, x23
	bl	dprint_vfmw
	cmp	w20, 96
	bne	.L20
	mov	w0, 0
.L16:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L23:
	sub	w3, w3, w0
	b	.L18
.L24:
	ldr	x2, .LC31
	ldr	x1, .LC91
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
	b	.L16
.L22:
	ldr	x2, .LC31
	mov	w0, 0
	ldr	x3, .LC30
	ldr	x1, .LC33
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
	b	.L16
	.size	VP8HAL_V400R003_CfgDnMsg, .-VP8HAL_V400R003_CfgDnMsg
	.align	3
.LC30:
	.xword	.LC29
	.align	3
.LC31:
	.xword	.LANCHOR0
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
	.align	3
.LC92:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC94:
	.xword	.LC93
	.align	3
.LC96:
	.xword	.LC95
	.align	3
.LC98:
	.xword	.LC97
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8HAL_V400R003_StartDec
	.type	VP8HAL_V400R003_StartDec, %function
VP8HAL_V400R003_StartDec:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	w20, w1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	cbz	w20, .L27
	cmp	w20, 1
	bne	.L34
	ldr	x2, .LC101
	mov	w4, 0
	ldr	x1, .LC102
	mov	w3, w20
	add	x2, x2, 56
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L29
	.p2align 3
.L27:
	ldr	w0, [x19, 2784]
	cmp	w0, 512
	bhi	.L31
	ldr	w0, [x19, 2788]
	cmp	w0, 512
	bhi	.L31
	ldr	x20, .LC106
	ldr	x0, [x20]
	cbz	x0, .L35
.L32:
	mov	x3, x21
	mov	w2, 0
	mov	x1, x20
	mov	x0, x19
	bl	VP8HAL_V400R003_CfgReg
	mov	w2, 0
	mov	x1, x20
	mov	x0, x19
	bl	VP8HAL_V400R003_CfgDnMsg
	mov	w0, 0
.L29:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L34:
	ldr	x1, .LC100
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L35:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L33
	str	x0, [x20]
	b	.L32
	.p2align 3
.L31:
	ldr	x2, .LC101
	mov	w0, 0
	ldr	x3, .LC104
	ldr	x1, .LC105
	add	x2, x2, 56
	bl	dprint_vfmw
	mov	w0, -1
	b	.L29
.L33:
	ldr	x1, .LC107
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L29
	.size	VP8HAL_V400R003_StartDec, .-VP8HAL_V400R003_StartDec
	.align	3
.LC100:
	.xword	.LC99
	.align	3
.LC101:
	.xword	.LANCHOR0
	.align	3
.LC102:
	.xword	.LC3
	.align	3
.LC104:
	.xword	.LC103
	.align	3
.LC105:
	.xword	.LC32
	.align	3
.LC106:
	.xword	g_HwMem
	.align	3
.LC107:
	.xword	.LC5
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.12095, %object
	.size	__func__.12095, 23
__func__.12095:
	.string	"VP8HAL_V400R003_CfgReg"
	.zero	1
	.type	__func__.12122, %object
	.size	__func__.12122, 25
__func__.12122:
	.string	"VP8HAL_V400R003_CfgDnMsg"
	.zero	7
	.type	__func__.12108, %object
	.size	__func__.12108, 25
__func__.12108:
	.string	"VP8HAL_V400R003_StartDec"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"VdhId is wrong! MP2HAL_V200R003_CfgReg\n" )
.LC3:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC5:
	ASCII(.string	"vdm register virtual address not mapped, reset failed!\n" )
.LC7:
	ASCII(.string	"BASIC_CFG0 = 0x%x\n" )
	.zero	5
.LC9:
	ASCII(.string	"BASIC_CFG1 = 0x%x\n" )
	.zero	5
.LC11:
	ASCII(.string	"AVM_ADDR = 0x%x\n" )
	.zero	7
.LC13:
	ASCII(.string	"VAM_ADDR = 0x%x\n" )
	.zero	7
.LC15:
	ASCII(.string	"STREAM_BASE_ADDR = 0x%x\n" )
	.zero	7
.LC17:
	ASCII(.string	"PPFD_BUF_ADDR = 0x%x\n" )
	.zero	2
.LC19:
	ASCII(.string	"PPFD_BUF_LEN = 0x%x\n" )
	.zero	3
.LC21:
	ASCII(.string	"YSTADDR_1D = 0x%x\n" )
	.zero	5
.LC23:
	ASCII(.string	"YSTRIDE_1D = 0x%x\n" )
	.zero	5
.LC25:
	ASCII(.string	"UVOFFSET_1D = 0x%x\n" )
	.zero	4
.LC29:
	ASCII(.string	"can not map down msg virtual address!" )
	.zero	2
.LC32:
	ASCII(.string	"%s: %s\n" )
.LC34:
	ASCII(.string	"D0 = 0x%x\n" )
	.zero	5
.LC36:
	ASCII(.string	"D1 = 0x%x\n" )
	.zero	5
.LC38:
	ASCII(.string	"D2 = 0x%x\n" )
	.zero	5
.LC40:
	ASCII(.string	"D3 = 0x%x\n" )
	.zero	5
.LC42:
	ASCII(.string	"D4 = 0x%x\n" )
	.zero	5
.LC44:
	ASCII(.string	"D5 = 0x%x\n" )
	.zero	5
.LC46:
	ASCII(.string	"D6 = 0x%x\n" )
	.zero	5
.LC48:
	ASCII(.string	"D7 = 0x%x\n" )
	.zero	5
.LC50:
	ASCII(.string	"D8 = 0x%x\n" )
	.zero	5
.LC52:
	ASCII(.string	"D9 = 0x%x\n" )
	.zero	5
.LC54:
	ASCII(.string	"D16 = 0x%x\n" )
	.zero	4
.LC56:
	ASCII(.string	"D17 = 0x%x\n" )
	.zero	4
.LC58:
	ASCII(.string	"D18 = 0x%x\n" )
	.zero	4
.LC60:
	ASCII(.string	"D19 = 0x%x\n" )
	.zero	4
.LC62:
	ASCII(.string	"D20 = 0x%x\n" )
	.zero	4
.LC64:
	ASCII(.string	"D21 = 0x%x\n" )
	.zero	4
.LC66:
	ASCII(.string	"D22 = 0x%x\n" )
	.zero	4
.LC68:
	ASCII(.string	"D23 = 0x%x\n" )
	.zero	4
.LC70:
	ASCII(.string	"D24 = 0x%x\n" )
	.zero	4
.LC72:
	ASCII(.string	"D25 = 0x%x\n" )
	.zero	4
.LC74:
	ASCII(.string	"D32 = 0x%x\n" )
	.zero	4
.LC76:
	ASCII(.string	"D33 = 0x%x\n" )
	.zero	4
.LC78:
	ASCII(.string	"D34 = 0x%x\n" )
	.zero	4
.LC80:
	ASCII(.string	"D35 = 0x%x\n" )
	.zero	4
.LC82:
	ASCII(.string	"D36 = 0x%x\n" )
	.zero	4
.LC84:
	ASCII(.string	"D37 = 0x%x\n" )
	.zero	4
.LC86:
	ASCII(.string	"D38 = 0x%x\n" )
	.zero	4
.LC88:
	ASCII(.string	"D39 = 0x%x\n" )
	.zero	4
.LC90:
	ASCII(.string	"%s: u8Tmp = NULL\n" )
	.zero	6
.LC93:
	ASCII(.string	"D40 = 0x%x\n" )
	.zero	4
.LC95:
	ASCII(.string	"D63 = 0x%x\n" )
	.zero	4
.LC97:
	ASCII(.string	"D%d = 0x%x\n" )
	.zero	4
.LC99:
	ASCII(.string	"VdhId is wrong! VP8HAL_V200R003_StartDec\n" )
	.zero	6
.LC103:
	ASCII(.string	"picture width out of range" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
