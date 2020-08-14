#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_vp6.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VP6HAL_V400R003_InitHal
	.type	VP6HAL_V400R003_InitHal, %function
VP6HAL_V400R003_InitHal:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	VP6HAL_V400R003_InitHal, .-VP6HAL_V400R003_InitHal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6HAL_V400R003_CfgReg
	.type	VP6HAL_V400R003_CfgReg, %function
VP6HAL_V400R003_CfgReg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	w22, w2
	mov	x0, x30
	mov	x21, x1
	mov	x19, x3
	bl	_mcount
	cbz	w22, .L4
	cmp	w22, 1
	beq	.L5
	ldr	x1, .LC1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L6:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4:
	ldr	x0, [x21]
	cbz	x0, .L14
.L7:
	ldrh	w5, [x20, 108]
	mov	w4, 0
	ldrh	w1, [x20, 106]
	mov	x3, x19
	mov	w0, 8
	mov	w2, 0
	mul	w1, w1, w5
	sub	w1, w1, #1
	bfi	w4, w1, 0, 20
	str	w4, [x29, 60]
	strb	wzr, [x29, 63]
	ldr	w22, [x29, 60]
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC8
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w2, [x20, 264]
	mov	w0, 0
	ldrh	w1, [x20, 106]
	mov	w3, 10
	bfi	w0, w2, 4, 1
	strb	w3, [x29, 60]
	orr	w0, w0, -64
	mov	w2, 8192
	cmp	w1, 120
	strb	w0, [x29, 61]
	strh	w2, [x29, 62]
	ble	.L15
	ldr	w4, [x20, 260]
	mov	w5, 0
	strb	wzr, [x20, 104]
	and	w4, w4, 1
.L10:
	ldrb	w1, [x29, 63]
	mov	x3, x19
	mov	w0, 12
	mov	w2, 0
	bfi	w1, w5, 5, 1
	bfi	w1, w4, 6, 1
	and	w1, w1, 127
	strb	w1, [x29, 63]
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
	ldr	w22, [x20, 112]
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
	str	wzr, [x29, 60]
	ldr	w0, [x21, 1184]
	mov	x3, x19
	strh	w0, [x29, 60]
	mov	w2, 0
	mov	w0, 132
	ldr	w21, [x29, 60]
	mov	w1, w21
	bl	MFDE_ConfigReg
	ldr	x1, .LC20
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldrh	w1, [x20, 106]
	mov	x3, x19
	mov	w2, 0
	mov	w0, 4
	cmp	w1, 256
	cset	w1, ls
	bl	SCD_ConfigReg
	mov	w1, 3075
	mov	x3, x19
	mov	w2, 0
	movk	w1, 0x30, lsl 16
	mov	w0, 60
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x19
	mov	w2, 0
	movk	w1, 0x30, lsl 16
	mov	w0, 64
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x19
	mov	w2, 0
	movk	w1, 0x30, lsl 16
	mov	w0, 68
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x19
	mov	w2, 0
	movk	w1, 0x30, lsl 16
	mov	w0, 72
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x19
	mov	w2, 0
	movk	w1, 0x30, lsl 16
	mov	w0, 76
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x19
	mov	w2, 0
	movk	w1, 0x30, lsl 16
	mov	w0, 80
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x19
	mov	w2, 0
	movk	w1, 0x30, lsl 16
	mov	w0, 84
	bl	MFDE_ConfigReg
	ldr	w21, [x20, 228]
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
	ldr	w21, [x20, 244]
	mov	x3, x19
	mov	w0, 100
	mov	w1, w21
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC24
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x20, 248]
	mov	x3, x19
	mov	w0, 104
	mov	w1, w21
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC26
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w20, [x20, 256]
	mov	x3, x19
	mov	w0, 108
	mov	w1, w20
	mov	w2, 0
	bl	MFDE_ConfigReg
	ldr	x1, .LC28
	mov	w2, w20
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x19
	mov	w2, 0
	mov	w1, 0
	mov	w0, 152
	bl	MFDE_ConfigReg
	ldr	x1, .LC30
	mov	w2, 0
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x19
	mov	w2, 0
	mov	w1, -1
	mov	w0, 32
	bl	MFDE_ConfigReg
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L15:
	mov	w0, 1
	mov	w4, 0
	strb	w0, [x20, 104]
	mov	w5, 1
	str	wzr, [x20, 260]
	b	.L10
	.p2align 3
.L14:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L8
	str	x0, [x21]
	b	.L7
	.p2align 3
.L5:
	ldr	x2, .LC2
	mov	w4, 0
	ldr	x1, .LC4
	mov	w3, w22
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L6
.L8:
	ldr	x1, .LC6
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L6
	.size	VP6HAL_V400R003_CfgReg, .-VP6HAL_V400R003_CfgReg
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
	.align	3
.LC28:
	.xword	.LC27
	.align	3
.LC30:
	.xword	.LC29
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6HAL_V400R003_CfgDnMsg
	.type	VP6HAL_V400R003_CfgDnMsg, %function
VP6HAL_V400R003_CfgDnMsg:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x1
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x22, 72]
	bl	MEM_Phy2Vir
	mov	x20, x0
	cbz	x0, .L29
	ldr	w2, [x19, 116]
	mov	w0, 0
	str	wzr, [x29, 76]
	bfi	w0, w2, 0, 1
	strb	w0, [x29, 76]
	ldr	x1, .LC37
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x20]
	bl	dprint_vfmw
	ldrh	w3, [x19, 106]
	mov	w0, 0
	ldrh	w2, [x19, 108]
	mov	w1, 0
	sub	w3, w3, #1
	sub	w2, w2, #1
	bfi	w0, w3, 0, 9
	bfi	w1, w2, 0, 9
	strh	w0, [x29, 76]
	strh	w1, [x29, 78]
	ubfx	x0, x0, 8, 1
	ubfx	x1, x1, 8, 1
	strb	w0, [x29, 77]
	strb	w1, [x29, 79]
	mov	w0, 4
	ldr	x1, .LC39
	ldr	w2, [x29, 76]
	str	w2, [x20, 4]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldp	w1, w5, [x19, 156]
	ldp	w3, w4, [x19, 124]
	ldr	w2, [x19, 164]
	mov	w0, 0
	bfi	w0, w1, 0, 5
	mov	w1, 0
	bfi	w0, w5, 5, 2
	bfi	w1, w4, 0, 1
	lsr	w2, w2, 1
	bfi	w0, w3, 7, 1
	bfi	w1, w2, 1, 1
	strb	w0, [x29, 76]
	strb	w1, [x29, 77]
	mov	w0, 4
	ldr	x1, .LC41
	ldr	w3, [x29, 76]
	and	w3, w3, 1023
	str	w3, [x29, 76]
	mov	w2, w3
	str	w3, [x20, 8]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldp	w1, w2, [x19, 148]
	mov	w0, 0
	bfi	w0, w2, 0, 5
	bfi	w0, w1, 5, 3
	strb	w0, [x29, 76]
	ldp	w3, w4, [x19, 140]
	ldr	w2, [x29, 76]
	mov	w0, 4
	ldr	x1, .LC43
	bfi	w2, w4, 8, 10
	str	w2, [x29, 76]
	lsr	w2, w2, 16
	bfi	w2, w3, 2, 2
	strb	w2, [x29, 78]
	ldrh	w2, [x29, 78]
	and	w2, w2, 15
	strh	w2, [x29, 78]
	ldr	w2, [x29, 76]
	str	w2, [x20, 12]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldrb	w1, [x19]
	mov	w0, 0
	ldp	w4, w3, [x19, 132]
	bfi	w0, w1, 0, 4
	strb	w0, [x29, 76]
	ldr	x1, .LC45
	mov	w0, 4
	ldrh	w2, [x29, 76]
	bfi	w2, w4, 4, 8
	strh	w2, [x29, 76]
	ldr	w2, [x29, 76]
	bfi	w2, w3, 12, 9
	str	w2, [x29, 76]
	ubfx	x2, x2, 16, 5
	strh	w2, [x29, 78]
	ldr	w2, [x29, 76]
	str	w2, [x20, 16]
	bl	dprint_vfmw
	ldr	w2, [x19, 216]
	mov	w0, 4
	ldr	x1, .LC47
	str	w2, [x20, 32]
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	ldr	w1, [x19, 224]
	mov	w0, 0
	bfi	w0, w1, 0, 4
	strb	w0, [x29, 78]
	ldr	w0, [x19, 220]
	ldrh	w2, [x29, 78]
	strb	w0, [x29, 76]
	mov	w0, 4
	and	w2, w2, 15
	strh	w2, [x29, 78]
	ldr	x1, .LC49
	ldr	w2, [x29, 76]
	str	w2, [x20, 36]
	bl	dprint_vfmw
	ldr	w21, [x19, 176]
	ldr	w0, [x19, 224]
	ldp	w3, w2, [x19, 168]
	add	w0, w0, 8
	cmp	w0, w2
	add	w3, w0, w3
	bls	.L30
	add	w2, w2, 128
	sub	w21, w21, #16
	sub	w2, w2, w0
.L20:
	mov	w0, 0
	ldr	x1, .LC51
	bfi	w0, w3, 0, 25
	str	w0, [x29, 76]
	lsr	w3, w0, 24
	mov	w0, 4
	bfi	w3, w2, 1, 7
	strb	w3, [x29, 79]
	ldr	w2, [x29, 76]
	str	w2, [x20, 64]
	bl	dprint_vfmw
	mov	w0, 0
	ldr	x1, .LC53
	bfi	w0, w21, 0, 24
	str	w0, [x29, 76]
	strb	wzr, [x29, 79]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x20, 68]
	bl	dprint_vfmw
	ldr	w1, [x19, 112]
	ldr	w0, [x19, 176]
	add	w0, w1, w0
	bl	MEM_Phy2Vir
	cbz	x0, .L31
.L21:
	ldp	w2, w3, [x19, 180]
	mov	w0, 0
	ldr	x1, .LC57
	bfi	w0, w2, 0, 25
	str	w0, [x29, 76]
	lsr	w2, w0, 24
	mov	w0, 4
	bfi	w2, w3, 1, 7
	strb	w2, [x29, 79]
	ldr	w2, [x29, 76]
	str	w2, [x20, 72]
	bl	dprint_vfmw
	ldr	w2, [x19, 188]
	mov	w0, 0
	ldr	x1, .LC59
	bfi	w0, w2, 0, 24
	str	w0, [x29, 76]
	strb	wzr, [x29, 79]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x20, 76]
	bl	dprint_vfmw
	ldp	w1, w2, [x19, 192]
	mov	w0, 0
	bfi	w0, w1, 0, 25
	str	w0, [x29, 76]
	lsr	w0, w0, 24
	ldr	x1, .LC61
	bfi	w0, w2, 1, 7
	strb	w0, [x29, 79]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x20, 80]
	bl	dprint_vfmw
	ldr	w2, [x19, 200]
	mov	w0, 0
	ldr	x1, .LC63
	bfi	w0, w2, 0, 24
	str	w0, [x29, 76]
	strb	wzr, [x29, 79]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x20, 84]
	bl	dprint_vfmw
	ldp	w1, w2, [x19, 204]
	mov	w0, 0
	bfi	w0, w1, 0, 25
	str	w0, [x29, 76]
	lsr	w0, w0, 24
	ldr	x1, .LC65
	bfi	w0, w2, 1, 7
	strb	w0, [x29, 79]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x20, 88]
	bl	dprint_vfmw
	ldr	w2, [x19, 212]
	mov	w0, 0
	ldr	x1, .LC67
	bfi	w0, w2, 0, 24
	str	w0, [x29, 76]
	strb	wzr, [x29, 79]
	mov	w0, 4
	ldr	w2, [x29, 76]
	str	w2, [x20, 92]
	bl	dprint_vfmw
	ldr	w2, [x19, 228]
	mov	w0, 4
	ldr	x1, .LC69
	and	w2, w2, -16
	str	w2, [x20, 128]
	bl	dprint_vfmw
	ldr	w2, [x19, 232]
	mov	w0, 4
	ldr	x1, .LC71
	and	w2, w2, -16
	str	w2, [x20, 136]
	bl	dprint_vfmw
	ldr	w2, [x19, 236]
	mov	w0, 4
	ldr	x1, .LC73
	and	w2, w2, -16
	str	w2, [x20, 140]
	bl	dprint_vfmw
	ldr	w2, [x22, 1160]
	mov	w0, 4
	ldr	x1, .LC75
	and	w2, w2, -16
	str	w2, [x20, 144]
	bl	dprint_vfmw
	ldr	w2, [x22, 1164]
	mov	w0, 4
	ldr	x1, .LC77
	and	w2, w2, -16
	str	w2, [x20, 148]
	bl	dprint_vfmw
	ldr	w2, [x22, 1168]
	mov	w0, 4
	ldr	x1, .LC79
	and	w2, w2, -16
	str	w2, [x20, 152]
	bl	dprint_vfmw
	ldr	w21, [x22, 1200]
	ldr	x1, .LC81
	mov	w0, 4
	and	w21, w21, -16
	str	w21, [x20, 156]
	mov	w2, w21
	str	w21, [x29, 76]
	bl	dprint_vfmw
	mov	w0, w21
	bl	MEM_Phy2Vir
	mov	x23, x0
	ldr	w0, [x22, 1200]
	bl	MEM_Phy2Vir
	cmp	x0, xzr
	ccmp	x23, xzr, 4, ne
	beq	.L32
	ldr	x20, .LC84
	mov	x2, 4096
	mov	w1, 0
	mov	x0, x23
	ldr	x3, [x20, 96]
	blr	x3
	ldr	x1, [x19, 8]
	mov	x2, 64
	ldr	x3, [x20, 104]
	mov	x0, x23
	blr	x3
	ldr	x1, [x19, 16]
	mov	x2, 640
	ldr	x3, [x20, 104]
	add	x0, x23, 64
	blr	x3
	ldr	x1, [x19, 24]
	mov	x2, 64
	ldr	x3, [x20, 104]
	add	x0, x23, 704
	blr	x3
	ldr	w0, [x19, 128]
	cbnz	w0, .L23
	mov	w0, w21
	bl	MEM_Phy2Vir
	mov	x22, x0
	adds	x0, x0, 768
	beq	.L33
	ldr	x1, [x19, 56]
	mov	x2, 96
	ldr	x3, [x20, 104]
	blr	x3
	ldr	x1, [x19, 72]
	mov	x2, 32
	ldr	x3, [x20, 104]
	add	x0, x22, 864
	blr	x3
	ldr	x1, [x19, 64]
	mov	x2, 576
	ldr	x3, [x20, 104]
	add	x0, x22, 896
	blr	x3
.L25:
	mov	w0, w21
	bl	MEM_Phy2Vir
	mov	x22, x0
	adds	x0, x0, 2560
	beq	.L34
	ldr	x1, [x19, 32]
	mov	x2, 64
	ldr	x3, [x20, 104]
	blr	x3
	ldr	x1, [x19, 40]
	mov	x2, 64
	ldr	x3, [x20, 104]
	add	x0, x22, 2624
	blr	x3
	ldr	x1, [x19, 48]
	mov	x2, 960
	ldr	x3, [x20, 104]
	add	x0, x22, 2688
	blr	x3
	mov	w0, w21
	bl	MEM_Phy2Vir
	adds	x0, x0, 2816
	beq	.L35
	ldr	x1, [x19, 48]
	mov	x2, 960
	ldr	x3, [x20, 104]
	blr	x3
	mov	w0, 0
.L18:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L30:
	sub	w2, w2, w0
	b	.L20
	.p2align 3
.L23:
	mov	w0, w21
	bl	MEM_Phy2Vir
	mov	x22, x0
	adds	x0, x0, 768
	beq	.L36
	ldr	x1, [x19, 88]
	mov	x2, 1536
	ldr	x3, [x20, 104]
	blr	x3
	ldr	x1, [x19, 80]
	mov	x2, 128
	ldr	x3, [x20, 104]
	add	x0, x22, 2304
	blr	x3
	ldr	x1, [x19, 96]
	mov	x2, 128
	ldr	x3, [x20, 104]
	add	x0, x22, 2432
	blr	x3
	b	.L25
	.p2align 3
.L31:
	ldr	x1, .LC55
	mov	w2, 413
	mov	w0, 1
	bl	dprint_vfmw
	b	.L21
	.p2align 3
.L32:
	ldr	x2, .LC33
	mov	x3, x0
	ldr	x1, .LC83
	mov	x4, x23
	add	x2, x2, 24
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L18
.L29:
	ldr	x2, .LC33
	mov	w0, 0
	ldr	x3, .LC32
	ldr	x1, .LC35
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
	b	.L18
.L35:
	ldr	x1, .LC86
	mov	w2, 623
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L18
.L34:
	ldr	x1, .LC86
	mov	w2, 602
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L18
.L36:
	ldr	x1, .LC86
	mov	w2, 578
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L18
.L33:
	ldr	x1, .LC86
	mov	w2, 552
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L18
	.size	VP6HAL_V400R003_CfgDnMsg, .-VP6HAL_V400R003_CfgDnMsg
	.align	3
.LC32:
	.xword	.LC31
	.align	3
.LC33:
	.xword	.LANCHOR0
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
.LC84:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC86:
	.xword	.LC85
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6HAL_V400R003_StartDec
	.type	VP6HAL_V400R003_StartDec, %function
VP6HAL_V400R003_StartDec:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	w20, w1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	cbz	w20, .L39
	cmp	w20, 1
	bne	.L47
	ldr	x2, .LC89
	mov	w4, 0
	ldr	x1, .LC90
	mov	w3, w20
	add	x2, x2, 56
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L41
	.p2align 3
.L39:
	ldrh	w0, [x19, 106]
	sub	w0, w0, #1
	uxth	w0, w0
	cmp	w0, 511
	bhi	.L48
	ldrh	w0, [x19, 108]
	sub	w0, w0, #1
	uxth	w0, w0
	cmp	w0, 511
	bhi	.L49
	ldr	x20, .LC96
	ldr	x0, [x20]
	cbz	x0, .L50
.L44:
	mov	x3, x21
	mov	w2, 0
	mov	x1, x20
	mov	x0, x19
	bl	VP6HAL_V400R003_CfgReg
	mov	w2, 0
	mov	x1, x20
	mov	x0, x19
	bl	VP6HAL_V400R003_CfgDnMsg
	mov	w0, 0
.L41:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L47:
	ldr	x1, .LC88
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L50:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L45
	str	x0, [x20]
	b	.L44
	.p2align 3
.L48:
	ldr	x2, .LC89
	ldr	x3, .LC92
.L46:
	ldr	x1, .LC93
	mov	w0, 0
	add	x2, x2, 56
	bl	dprint_vfmw
	mov	w0, -1
	b	.L41
	.p2align 3
.L49:
	ldr	x2, .LC89
	ldr	x3, .LC95
	b	.L46
.L45:
	ldr	x1, .LC97
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L41
	.size	VP6HAL_V400R003_StartDec, .-VP6HAL_V400R003_StartDec
	.align	3
.LC88:
	.xword	.LC87
	.align	3
.LC89:
	.xword	.LANCHOR0
	.align	3
.LC90:
	.xword	.LC3
	.align	3
.LC92:
	.xword	.LC91
	.align	3
.LC93:
	.xword	.LC34
	.align	3
.LC95:
	.xword	.LC94
	.align	3
.LC96:
	.xword	g_HwMem
	.align	3
.LC97:
	.xword	.LC5
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.12147, %object
	.size	__func__.12147, 23
__func__.12147:
	.string	"VP6HAL_V400R003_CfgReg"
	.zero	1
	.type	__func__.12162, %object
	.size	__func__.12162, 25
__func__.12162:
	.string	"VP6HAL_V400R003_CfgDnMsg"
	.zero	7
	.type	__func__.12133, %object
	.size	__func__.12133, 25
__func__.12133:
	.string	"VP6HAL_V400R003_StartDec"
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
	ASCII(.string	"PPFD_V400R003_BUF_ADDR = 0x%x\n" )
	.zero	1
.LC19:
	ASCII(.string	"PPFD_V200R003_BUF_LEN = 0x%x\n" )
	.zero	2
.LC21:
	ASCII(.string	"YSTADDR_1D = 0x%x\n" )
	.zero	5
.LC23:
	ASCII(.string	"YSTRIDE_1D = 0x%x\n" )
	.zero	5
.LC25:
	ASCII(.string	"UVOFFSET_1D = 0x%x\n" )
	.zero	4
.LC27:
	ASCII(.string	"HEAD_INF_OFFSET = 0x%x\n" )
.LC29:
	ASCII(.string	"FF_APT_EN = 0x%x\n" )
	.zero	6
.LC31:
	ASCII(.string	"can not map down msg virtual address!" )
	.zero	2
.LC34:
	ASCII(.string	"%s: %s\n" )
.LC36:
	ASCII(.string	"D0 = 0x%x\n" )
	.zero	5
.LC38:
	ASCII(.string	"D1 = 0x%x\n" )
	.zero	5
.LC40:
	ASCII(.string	"D2 = 0x%x\n" )
	.zero	5
.LC42:
	ASCII(.string	"D3 = 0x%x\n" )
	.zero	5
.LC44:
	ASCII(.string	"D4 = 0x%x\n" )
	.zero	5
.LC46:
	ASCII(.string	"D8 = 0x%x\n" )
	.zero	5
.LC48:
	ASCII(.string	"D9 = 0x%x\n" )
	.zero	5
.LC50:
	ASCII(.string	"D16 = 0x%x\n" )
	.zero	4
.LC52:
	ASCII(.string	"D17 = 0x%x\n" )
	.zero	4
.LC54:
	ASCII(.string	"LINE:%d NULL == tmpAddr.\n" )
	.zero	6
.LC56:
	ASCII(.string	"D18 = 0x%x\n" )
	.zero	4
.LC58:
	ASCII(.string	"D19 = 0x%x\n" )
	.zero	4
.LC60:
	ASCII(.string	"D20 = 0x%x\n" )
	.zero	4
.LC62:
	ASCII(.string	"D21 = 0x%x\n" )
	.zero	4
.LC64:
	ASCII(.string	"D22 = 0x%x\n" )
	.zero	4
.LC66:
	ASCII(.string	"D23 = 0x%x\n" )
	.zero	4
.LC68:
	ASCII(.string	"D32 = 0x%x\n" )
	.zero	4
.LC70:
	ASCII(.string	"D34 = 0x%x\n" )
	.zero	4
.LC72:
	ASCII(.string	"D35 = 0x%x\n" )
	.zero	4
.LC74:
	ASCII(.string	"D36 = 0x%x\n" )
	.zero	4
.LC76:
	ASCII(.string	"D37 = 0x%x\n" )
	.zero	4
.LC78:
	ASCII(.string	"D38 = 0x%x\n" )
	.zero	4
.LC80:
	ASCII(.string	"D39 = 0x%x\n" )
	.zero	4
.LC82:
	ASCII(.string	"%s: tmpAddr(%p) = NULL / TabBaseAddr(%p) = NULL\n" )
	.zero	7
.LC85:
	ASCII(.string	"line: %d  NULL == TabBaseAddr.\n" )
.LC87:
	ASCII(.string	"VdhId is wrong! MP2HAL_V200R003_StartDec\n" )
	.zero	6
.LC91:
	ASCII(.string	"picture width out of range" )
	.zero	5
.LC94:
	ASCII(.string	"picture height out of range" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
