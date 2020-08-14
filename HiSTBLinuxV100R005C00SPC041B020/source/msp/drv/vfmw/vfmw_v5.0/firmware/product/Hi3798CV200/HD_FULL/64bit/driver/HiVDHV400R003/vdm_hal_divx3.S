#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_divx3.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	DIVX3HAL_V400R003_InitHal
	.type	DIVX3HAL_V400R003_InitHal, %function
DIVX3HAL_V400R003_InitHal:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	DIVX3HAL_V400R003_InitHal, .-DIVX3HAL_V400R003_InitHal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3HAL_V400R003_WriteReg
	.type	DIVX3HAL_V400R003_WriteReg, %function
DIVX3HAL_V400R003_WriteReg:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	w19, w2
	mov	x21, x1
	mov	x20, x3
	bl	_mcount
	ldr	x1, .LC1
	mov	w0, 2
	bl	dprint_vfmw
	cmp	w19, wzr
	ble	.L3
	ldr	x2, .LC2
	mov	w3, w19
	ldr	x1, .LC4
	mov	w4, 0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3:
	ldp	w1, w4, [x21, 8]
	mov	w0, 0
	mov	x3, x20
	mov	w2, w19
	mul	w1, w1, w4
	sub	w1, w1, #1
	bfi	w0, w1, 0, 20
	str	w0, [x29, 76]
	strb	wzr, [x29, 79]
	lsr	w0, w0, 16
	strb	w0, [x29, 78]
	mov	w0, 8
	ldr	w23, [x29, 76]
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC6
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	strh	wzr, [x29, 78]
	ldp	w1, w2, [x21, 164]
	mov	w0, 0
	mov	x3, x20
	bfi	w0, w2, 4, 1
	ubfiz	w1, w1, 6, 1
	orr	w0, w0, -64
	strb	w1, [x29, 79]
	strb	w0, [x29, 77]
	mov	w0, 5
	strb	w0, [x29, 76]
	mov	w2, w19
	mov	w0, 12
	ldr	w23, [x29, 76]
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC8
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x22, 72]
	mov	x3, x20
	mov	w2, w19
	and	w23, w23, -16
	mov	w0, 16
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC10
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x22, 56]
	mov	x3, x20
	mov	w2, w19
	and	w23, w23, -16
	mov	w0, 20
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC12
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x21, 96]
	mov	x3, x20
	mov	w2, w19
	mov	w1, w23
	mov	w0, 24
	str	w23, [x29, 76]
	bl	MFDE_ConfigReg
	ldr	x1, .LC14
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w1, [x21, 8]
	mov	x3, x20
	mov	w2, w19
	mov	w0, 4
	cmp	w1, 256
	cset	w1, ls
	bl	SCD_ConfigReg
	mov	w1, 3075
	mov	x3, x20
	mov	w2, w19
	mov	w0, 60
	movk	w1, 0x30, lsl 16
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x20
	mov	w2, w19
	mov	w0, 64
	movk	w1, 0x30, lsl 16
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x20
	mov	w2, w19
	mov	w0, 68
	movk	w1, 0x30, lsl 16
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x20
	mov	w2, w19
	mov	w0, 72
	movk	w1, 0x30, lsl 16
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x20
	mov	w2, w19
	mov	w0, 76
	movk	w1, 0x30, lsl 16
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x20
	mov	w2, w19
	mov	w0, 80
	movk	w1, 0x30, lsl 16
	bl	MFDE_ConfigReg
	mov	w1, 3075
	mov	x3, x20
	mov	w2, w19
	mov	w0, 84
	movk	w1, 0x30, lsl 16
	bl	MFDE_ConfigReg
	ldr	x1, .LC16
	mov	w2, 3075
	movk	w2, 0x30, lsl 16
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x21, 128]
	mov	x3, x20
	mov	w2, w19
	and	w23, w23, -16
	mov	w0, 96
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC18
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x21, 136]
	mov	x3, x20
	mov	w2, w19
	mov	w1, w23
	mov	w0, 100
	bl	MFDE_ConfigReg
	ldr	x1, .LC20
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x21, 140]
	mov	x3, x20
	mov	w2, w19
	mov	w1, w23
	mov	w0, 104
	bl	MFDE_ConfigReg
	ldr	x1, .LC22
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x21, 148]
	mov	x3, x20
	mov	w2, w19
	mov	w1, w21
	mov	w0, 108
	bl	MFDE_ConfigReg
	ldr	x1, .LC24
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x22, 1220]
	mov	x3, x20
	mov	w2, w19
	mov	w1, w21
	mov	w0, 144
	bl	MFDE_ConfigReg
	ldr	x1, .LC26
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	str	wzr, [x29, 76]
	mov	x3, x20
	mov	w2, w19
	mov	w1, 0
	mov	w0, 148
	bl	MFDE_ConfigReg
	ldr	x1, .LC28
	mov	w2, 0
	mov	w0, 3
	bl	dprint_vfmw
	ldrb	w1, [x29, 76]
	mov	x3, x20
	mov	w2, w19
	mov	w0, 152
	and	w1, w1, -2
	strb	w1, [x29, 76]
	ldr	w21, [x29, 76]
	mov	w1, w21
	bl	MFDE_ConfigReg
	ldr	x1, .LC30
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x20
	mov	w2, w19
	mov	w1, -1
	mov	w0, 32
	bl	MFDE_ConfigReg
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.size	DIVX3HAL_V400R003_WriteReg, .-DIVX3HAL_V400R003_WriteReg
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
	.global	DIVX3HAL_V400R003_WritePicMsg
	.type	DIVX3HAL_V400R003_WritePicMsg, %function
DIVX3HAL_V400R003_WritePicMsg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x1, .LC32
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w0, [x21, 72]
	and	w0, w0, -16
	bl	MEM_Phy2Vir
	mov	x20, x0
	cbz	x0, .L11
	ldr	w2, [x19, 4]
	mov	w0, 0
	str	wzr, [x29, 60]
	bfi	w0, w2, 0, 1
	strb	w0, [x29, 60]
	ldr	x1, .LC39
	mov	w0, 4
	ldr	w2, [x29, 60]
	str	w2, [x20]
	bl	dprint_vfmw
	ldp	w1, w0, [x19, 8]
	mov	w3, 0
	mov	w2, 0
	sub	w1, w1, #1
	sub	w0, w0, #1
	bfi	w3, w1, 0, 9
	bfi	w2, w0, 0, 9
	strh	w3, [x29, 60]
	mov	w0, 4
	strh	w2, [x29, 62]
	ldr	x1, .LC41
	ldr	w2, [x29, 60]
	str	w2, [x20, 4]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	ldp	w1, w0, [x19, 16]
	mov	w2, 0
	ldr	w3, [x19, 24]
	bfi	w2, w1, 0, 5
	ldr	x1, .LC43
	bfi	w2, w0, 5, 1
	mov	w0, 4
	bfi	w2, w3, 6, 1
	strb	w2, [x29, 60]
	ldr	w2, [x29, 60]
	str	w2, [x20, 8]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	ldp	w1, w2, [x19, 28]
	mov	w0, 0
	ldr	w4, [x19, 40]
	bfi	w0, w1, 0, 2
	ldr	w1, [x19, 36]
	bfi	w0, w2, 2, 2
	ldr	w3, [x19, 48]
	ldr	w2, [x19, 44]
	bfi	w0, w1, 4, 1
	bfi	w0, w4, 5, 2
	mov	w1, 0
	bfi	w1, w3, 0, 1
	bfi	w0, w2, 7, 1
	strb	w1, [x29, 61]
	strb	w0, [x29, 60]
	mov	w0, 4
	ldr	x1, .LC45
	ldr	w2, [x29, 60]
	str	w2, [x20, 12]
	bl	dprint_vfmw
	ldr	w2, [x19, 100]
	mov	w0, 4
	ldr	x1, .LC47
	str	w2, [x20, 64]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	ldr	w1, [x19, 108]
	mov	w0, 0
	bfi	w0, w1, 0, 7
	strb	w0, [x29, 60]
	ldr	x1, .LC49
	mov	w0, 4
	ldr	w2, [x29, 60]
	str	w2, [x20, 68]
	bl	dprint_vfmw
	ldr	w2, [x19, 116]
	mov	w0, 4
	ldr	x1, .LC51
	str	w2, [x20, 72]
	bl	dprint_vfmw
	ldr	w2, [x19, 104]
	mov	w0, 4
	ldr	x1, .LC53
	str	w2, [x20, 76]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	ldr	w1, [x19, 112]
	mov	w0, 0
	bfi	w0, w1, 0, 7
	strb	w0, [x29, 60]
	ldr	x1, .LC55
	mov	w0, 4
	ldr	w2, [x29, 60]
	str	w2, [x20, 80]
	bl	dprint_vfmw
	ldr	w2, [x19, 120]
	mov	w0, 4
	ldr	x1, .LC57
	str	w2, [x20, 84]
	bl	dprint_vfmw
	ldr	w2, [x19, 132]
	mov	w0, 4
	ldr	x1, .LC59
	and	w2, w2, -16
	str	w2, [x20, 128]
	bl	dprint_vfmw
	ldr	w2, [x19, 124]
	mov	w0, 4
	ldr	x1, .LC61
	and	w2, w2, -16
	str	w2, [x20, 132]
	bl	dprint_vfmw
	ldr	w2, [x21, 1160]
	mov	w0, 4
	ldr	x1, .LC63
	and	w2, w2, -16
	str	w2, [x20, 136]
	bl	dprint_vfmw
	ldr	w2, [x21, 1164]
	mov	w0, 4
	ldr	x1, .LC65
	and	w2, w2, -16
	str	w2, [x20, 140]
	bl	dprint_vfmw
	ldr	w2, [x21, 1172]
	mov	w0, 4
	ldr	x1, .LC67
	and	w2, w2, -16
	str	w2, [x20, 144]
	bl	dprint_vfmw
	ldr	w2, [x19, 156]
	mov	w0, 4
	ldr	x1, .LC69
	and	w2, w2, -16
	str	w2, [x20, 148]
	bl	dprint_vfmw
	ldr	w2, [x21, 76]
	mov	w0, 4
	ldr	x1, .LC71
	and	w2, w2, -16
	str	w2, [x20, 252]
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L11:
	ldr	x2, .LC35
	mov	w0, 0
	ldr	x3, .LC34
	ldr	x1, .LC37
	add	x2, x2, 32
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
	.size	DIVX3HAL_V400R003_WritePicMsg, .-DIVX3HAL_V400R003_WritePicMsg
	.align	3
.LC32:
	.xword	.LC31
	.align	3
.LC34:
	.xword	.LC33
	.align	3
.LC35:
	.xword	.LANCHOR0
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3HAL_V400R003_WriteSliceMsg
	.type	DIVX3HAL_V400R003_WriteSliceMsg, %function
DIVX3HAL_V400R003_WriteSliceMsg:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	mov	x27, x1
	bl	_mcount
	ldr	w19, [x19, 76]
	and	w19, w19, -16
	mov	w0, w19
	bl	MEM_Phy2Vir
	mov	x21, x0
	cbz	x0, .L31
	ldr	w0, [x27, 56]
	mov	w22, 0
	cbnz	w0, .L32
.L15:
	ldr	w2, [x27, 52]
	cbz	w2, .L12
	lsl	w22, w22, 6
	ldr	x26, .LC75
	ldr	x25, .LC76
	sxtw	x0, w22
	ldr	x24, .LC77
	str	x0, [x29, 104]
	add	w0, w22, w19
	mov	w22, 0
	str	w0, [x29, 100]
	.p2align 2
.L26:
	cbz	w22, .L33
	sub	w0, w22, #1
	add	x1, x27, x22, uxtw 2
	add	x0, x27, x0, lsl 2
	ldr	w1, [x1, 56]
	ldr	w0, [x0, 56]
	cmp	w1, w0
	bls	.L19
.L18:
	ldr	x0, [x29, 104]
	ubfiz	x20, x22, 6, 26
	mov	w4, 0
	add	w19, w22, 1
	add	x20, x20, x0
	bfi	w4, w1, 0, 20
	str	w4, [x29, 124]
	mov	x1, x26
	mov	w2, w4
	mov	w0, 4
	str	w4, [x21, x20, lsl 2]
	lsl	x20, x20, 2
	bl	dprint_vfmw
	ldr	w1, [x27, 52]
	cmp	w19, w1
	bcs	.L20
	add	x3, x27, x22, uxtw 2
	add	x0, x27, x19, uxtw 2
	ldr	w2, [x3, 56]
	ldr	w0, [x0, 56]
	cmp	w2, w0
	bcs	.L22
	b	.L20
	.p2align 3
.L23:
	ldr	w0, [x0, 56]
	cmp	w0, w2
	bhi	.L20
.L22:
	add	w19, w19, 1
	cmp	w19, w1
	add	x0, x27, x19, uxtw 2
	bcc	.L23
.L20:
	cmp	w1, w19
	beq	.L34
	add	x0, x27, x19, uxtw 2
	ldr	w1, [x29, 100]
	add	w23, w1, w19, lsl 8
	ldr	w1, [x0, 56]
	mov	w28, w23
	sub	w1, w1, #1
.L25:
	add	x3, x20, 4
	mov	w0, 0
	bfi	w0, w1, 0, 20
	str	w0, [x29, 124]
	mov	w2, w0
	mov	x1, x25
	str	w0, [x21, x3]
	add	x20, x20, 252
	mov	w0, 4
	sub	w22, w19, #1
	bl	dprint_vfmw
	str	w28, [x21, x20]
	mov	w2, w23
	mov	x1, x24
	mov	w0, 4
	str	w23, [x29, 124]
	bl	dprint_vfmw
	ldr	w2, [x27, 52]
.L19:
	add	w22, w22, 1
	cmp	w2, w22
	bhi	.L26
.L12:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L33:
	ldr	w1, [x27, 56]
	b	.L18
	.p2align 3
.L34:
	ldp	w0, w1, [x27, 8]
	mov	w28, 0
	mov	w23, 0
	mul	w1, w0, w1
	sub	w1, w1, #1
	b	.L25
.L32:
	str	wzr, [x29, 124]
	mov	w2, 0
	ldr	x1, .LC75
	mov	w0, 4
	str	wzr, [x21]
	mov	w22, 1
	bl	dprint_vfmw
	ldr	w2, [x27, 56]
	mov	w3, 0
	ldr	x1, .LC76
	mov	w0, 4
	sub	w2, w2, #1
	bfi	w3, w2, 0, 20
	str	w3, [x29, 124]
	mov	w2, w3
	str	w3, [x21, 4]
	bl	dprint_vfmw
	add	w3, w19, 256
	ldr	x1, .LC77
	mov	w2, w3
	str	w3, [x21, 252]
	mov	w0, 4
	str	w3, [x29, 124]
	bl	dprint_vfmw
	b	.L15
.L31:
	ldr	x2, .LC73
	mov	w0, 0
	ldr	x3, .LC72
	ldr	x1, .LC74
	add	x2, x2, 64
	bl	dprint_vfmw
	b	.L12
	.size	DIVX3HAL_V400R003_WriteSliceMsg, .-DIVX3HAL_V400R003_WriteSliceMsg
	.align	3
.LC72:
	.xword	.LC33
	.align	3
.LC73:
	.xword	.LANCHOR0
	.align	3
.LC74:
	.xword	.LC36
	.align	3
.LC75:
	.xword	.LC38
	.align	3
.LC76:
	.xword	.LC40
	.align	3
.LC77:
	.xword	.LC70
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3HAL_V400R003_StartDec
	.type	DIVX3HAL_V400R003_StartDec, %function
DIVX3HAL_V400R003_StartDec:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	w19, w1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	cbz	w19, .L37
	cmp	w19, 1
	bne	.L42
	ldr	x2, .LC80
	mov	w4, 0
	ldr	x1, .LC81
	mov	w3, w19
	add	x2, x2, 96
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L39
	.p2align 3
.L37:
	ldr	x19, .LC82
	ldr	x0, [x19]
	cbz	x0, .L43
.L40:
	ldr	x5, .LC85
	mov	x3, x21
	mov	w2, 0
	mov	x1, x20
	mov	x0, x19
	ldr	w4, [x5]
	add	w4, w4, 1
	str	w4, [x5]
	bl	DIVX3HAL_V400R003_WriteReg
	mov	x1, x20
	mov	x0, x19
	bl	DIVX3HAL_V400R003_WritePicMsg
	mov	x0, x19
	mov	x1, x20
	bl	DIVX3HAL_V400R003_WriteSliceMsg
	mov	w0, 0
.L39:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L42:
	ldr	x1, .LC79
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L43:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L41
	str	x0, [x19]
	b	.L40
.L41:
	ldr	x1, .LC84
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L39
	.size	DIVX3HAL_V400R003_StartDec, .-DIVX3HAL_V400R003_StartDec
	.align	3
.LC79:
	.xword	.LC78
	.align	3
.LC80:
	.xword	.LANCHOR0
	.align	3
.LC81:
	.xword	.LC3
	.align	3
.LC82:
	.xword	g_HwMem
	.align	3
.LC84:
	.xword	.LC83
	.align	3
.LC85:
	.xword	.LANCHOR1
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.12167, %object
	.size	__func__.12167, 27
__func__.12167:
	.string	"DIVX3HAL_V400R003_WriteReg"
	.zero	5
	.type	__func__.12175, %object
	.size	__func__.12175, 30
__func__.12175:
	.string	"DIVX3HAL_V400R003_WritePicMsg"
	.zero	2
	.type	__func__.12189, %object
	.size	__func__.12189, 32
__func__.12189:
	.string	"DIVX3HAL_V400R003_WriteSliceMsg"
	.type	__func__.12159, %object
	.size	__func__.12159, 27
__func__.12159:
	.string	"DIVX3HAL_V400R003_StartDec"
	.data
	.align	2
.LANCHOR1 = . + 0
	.type	FrameNum, %object
	.size	FrameNum, 4
FrameNum:
	.word	-1
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"configuring VDM registers...\n" )
	.zero	2
.LC3:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC5:
	ASCII(.string	"BASIC_V200R003_CFG0 = 0x%x\n" )
	.zero	4
.LC7:
	ASCII(.string	"BASIC_V400R003_CFG1=0x%x\n" )
	.zero	6
.LC9:
	ASCII(.string	"AVM_V400R003_ADDR=0x%x\n" )
.LC11:
	ASCII(.string	"VAM_V400R003_ADDR=0x%x\n" )
.LC13:
	ASCII(.string	"STREAM_V200R003_BASE_ADDR=0x%x\n" )
.LC15:
	ASCII(.string	"TIME_OUT = 0x%x\n" )
	.zero	7
.LC17:
	ASCII(.string	"YSTADDR_V400R003_1D = 0x%x\n" )
	.zero	4
.LC19:
	ASCII(.string	"YSTRIDE_V400R003_1D = 0x%x\n" )
	.zero	4
.LC21:
	ASCII(.string	"UVOFFSET_V400R003_1D = 0x%x\n" )
	.zero	3
.LC23:
	ASCII(.string	"HEAD_INF_OFFSET = 0x%x\n" )
.LC25:
	ASCII(.string	"DNR_MBINFO_STADDR=0x%x\n" )
.LC27:
	ASCII(.string	"REF_V400R003_PIC_TYPE=0x%x\n" )
	.zero	4
.LC29:
	ASCII(.string	"FF_V400R003_APT_EN=0x%x\n" )
	.zero	7
.LC31:
	ASCII(.string	"configuring Pic Msg...\n" )
.LC33:
	ASCII(.string	"can not map dn msg virtual address!\n" )
	.zero	3
.LC36:
	ASCII(.string	"%s: %s\n" )
.LC38:
	ASCII(.string	"D0 = 0x%x\n" )
	.zero	5
.LC40:
	ASCII(.string	"D1 = 0x%x\n" )
	.zero	5
.LC42:
	ASCII(.string	"D2 = 0x%x\n" )
	.zero	5
.LC44:
	ASCII(.string	"D3 = 0x%x\n" )
	.zero	5
.LC46:
	ASCII(.string	"D16 = 0x%x\n" )
	.zero	4
.LC48:
	ASCII(.string	"D17 = 0x%x\n" )
	.zero	4
.LC50:
	ASCII(.string	"D18 = 0x%x\n" )
	.zero	4
.LC52:
	ASCII(.string	"D19 = 0x%x\n" )
	.zero	4
.LC54:
	ASCII(.string	"D20 = 0x%x\n" )
	.zero	4
.LC56:
	ASCII(.string	"D21 = 0x%x\n" )
	.zero	4
.LC58:
	ASCII(.string	"D32 = 0x%x\n" )
	.zero	4
.LC60:
	ASCII(.string	"D33 = 0x%x\n" )
	.zero	4
.LC62:
	ASCII(.string	"D34 = 0x%x\n" )
	.zero	4
.LC64:
	ASCII(.string	"D35 = 0x%x\n" )
	.zero	4
.LC66:
	ASCII(.string	"D36 = 0x%x\n" )
	.zero	4
.LC68:
	ASCII(.string	"D37 = 0x%x\n" )
	.zero	4
.LC70:
	ASCII(.string	"D63 = 0x%x\n" )
	.zero	4
.LC78:
	ASCII(.string	"VdhId is wrong! DIVX3HAL_V400R003_StartDec\n" )
	.zero	4
.LC83:
	ASCII(.string	"vdm register virtual address not mapped, reset failed!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
