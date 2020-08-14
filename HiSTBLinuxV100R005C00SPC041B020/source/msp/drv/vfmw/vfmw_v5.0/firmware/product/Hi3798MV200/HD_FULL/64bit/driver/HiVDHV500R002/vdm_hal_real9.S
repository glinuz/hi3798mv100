#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_real9.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	RV9HAL_V400R003_InitHal
	.type	RV9HAL_V400R003_InitHal, %function
RV9HAL_V400R003_InitHal:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	RV9HAL_V400R003_InitHal, .-RV9HAL_V400R003_InitHal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	RV9HAL_V400R003_WriteReg
	.type	RV9HAL_V400R003_WriteReg, %function
RV9HAL_V400R003_WriteReg:
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
	ldp	w1, w5, [x21, 4]
	mov	w0, 0
	mov	w4, 1
	mov	x3, x20
	mov	w2, w19
	mul	w1, w1, w5
	sub	w1, w1, #1
	bfi	w0, w1, 0, 20
	str	w0, [x29, 76]
	strb	w4, [x29, 79]
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
	str	wzr, [x29, 76]
	ldr	w2, [x21, 80]
	mov	w0, 0
	mov	w1, 9
	strb	w1, [x29, 76]
	bfi	w0, w2, 4, 1
	strb	w0, [x29, 77]
	ldr	w0, [x21, 60]
	ldrh	w2, [x29, 76]
	lsr	w0, w0, 6
	ldr	w3, [x21, 4]
	bfi	w2, w0, 4, 7
	ldr	w4, [x21, 36]
	ldrh	w1, [x29, 78]
	lsr	x0, x2, 8
	strh	w2, [x29, 76]
	bfi	w0, w4, 6, 1
	and	w1, w1, -4096
	lsl	w3, w3, 4
	orr	w2, w0, -128
	strh	w1, [x29, 78]
	and	w0, w1, 61440
	cmp	w3, 1920
	strb	w2, [x29, 77]
	lsr	w0, w0, 8
	and	w1, w0, -17
	bls	.L10
	ldr	w4, [x21, 76]
	and	w0, w0, -49
	strb	w0, [x29, 79]
	and	w4, w4, 1
.L6:
	ldrb	w1, [x29, 79]
	mov	x3, x20
	mov	w2, w19
	mov	w0, 12
	bfi	w1, w4, 6, 1
	and	w1, w1, 127
	strb	w1, [x29, 79]
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
	ldr	w23, [x21, 40]
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
	ldr	w1, [x21, 4]
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
	ldr	w23, [x21, 52]
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
	ldr	w23, [x21, 60]
	mov	x3, x20
	mov	w2, w19
	mov	w1, w23
	mov	w0, 100
	bl	MFDE_ConfigReg
	ldr	x1, .LC20
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x21, 84]
	mov	x3, x20
	mov	w2, w19
	mov	w1, w23
	mov	w0, 104
	bl	MFDE_ConfigReg
	ldr	x1, .LC22
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x21, 92]
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
	.p2align 3
.L10:
	str	wzr, [x21, 76]
	orr	w1, w1, 32
	mov	w4, 0
	strb	w1, [x29, 79]
	b	.L6
	.size	RV9HAL_V400R003_WriteReg, .-RV9HAL_V400R003_WriteReg
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
	.global	RV9HAL_V400R003_WritePicMsg
	.type	RV9HAL_V400R003_WritePicMsg, %function
RV9HAL_V400R003_WritePicMsg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x1, .LC32
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w0, [x21, 72]
	and	w0, w0, -16
	bl	MEM_Phy2Vir
	mov	x19, x0
	cbz	x0, .L16
	ldrb	w2, [x20, 1]
	mov	w0, 0
	str	wzr, [x29, 60]
	bfi	w0, w2, 0, 2
	strb	w0, [x29, 60]
	ldr	x1, .LC39
	mov	w0, 4
	ldr	w2, [x29, 60]
	str	w2, [x19]
	bl	dprint_vfmw
	ldp	w1, w0, [x20, 4]
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
	str	w2, [x19, 4]
	bl	dprint_vfmw
	ldp	w1, w0, [x20, 12]
	strh	w1, [x29, 60]
	strh	w0, [x29, 62]
	mov	w0, 4
	ldr	x1, .LC43
	ldr	w2, [x29, 60]
	str	w2, [x19, 8]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	strb	wzr, [x29, 60]
	mov	w0, 4
	ldr	x1, .LC45
	ldr	w2, [x29, 60]
	str	w2, [x19, 12]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	ldr	w1, [x20, 20]
	mov	w0, 0
	bfi	w0, w1, 0, 5
	strb	w0, [x29, 60]
	ldr	x1, .LC47
	mov	w0, 4
	ldr	w2, [x29, 60]
	str	w2, [x19, 16]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	ldr	w1, [x20, 24]
	mov	w0, 0
	bfi	w0, w1, 0, 5
	strb	w0, [x29, 60]
	ldr	x1, .LC49
	mov	w0, 4
	ldr	w2, [x29, 60]
	str	w2, [x19, 20]
	bl	dprint_vfmw
	str	wzr, [x29, 60]
	ldp	w0, w1, [x20, 4]
	mul	w0, w0, w1
	cmp	w0, 99
	ldrh	w0, [x29, 60]
	bls	.L17
	and	w0, w0, -2017
	strh	w0, [x29, 60]
.L15:
	ldr	w3, [x20, 28]
	mov	w0, 4
	ldrb	w2, [x29, 60]
	ldr	x1, .LC51
	bfi	w2, w3, 0, 5
	strb	w2, [x29, 60]
	ldr	w2, [x29, 60]
	str	w2, [x19, 24]
	bl	dprint_vfmw
	ldr	w2, [x20, 56]
	mov	w0, 4
	ldr	x1, .LC53
	and	w2, w2, -16
	str	w2, [x19, 64]
	bl	dprint_vfmw
	ldr	w2, [x20, 48]
	mov	w0, 4
	ldr	x1, .LC55
	and	w2, w2, -16
	str	w2, [x19, 68]
	bl	dprint_vfmw
	ldr	w2, [x20, 44]
	mov	w0, 4
	ldr	x1, .LC57
	and	w2, w2, -16
	str	w2, [x19, 72]
	bl	dprint_vfmw
	ldr	w2, [x20, 68]
	mov	w0, 4
	ldr	x1, .LC59
	str	w2, [x19, 76]
	bl	dprint_vfmw
	ldr	w2, [x20, 72]
	mov	w0, 4
	ldr	x1, .LC61
	str	w2, [x19, 80]
	bl	dprint_vfmw
	ldr	w2, [x21, 1160]
	mov	w0, 4
	ldr	x1, .LC63
	and	w2, w2, -16
	str	w2, [x19, 84]
	bl	dprint_vfmw
	ldr	w2, [x21, 1164]
	mov	w0, 4
	ldr	x1, .LC65
	and	w2, w2, -16
	str	w2, [x19, 88]
	bl	dprint_vfmw
	ldr	w2, [x21, 1168]
	mov	w0, 4
	ldr	x1, .LC67
	and	w2, w2, -16
	str	w2, [x19, 92]
	bl	dprint_vfmw
	ldr	w2, [x21, 1176]
	mov	w0, 4
	ldr	x1, .LC69
	str	w2, [x19, 96]
	bl	dprint_vfmw
	ldr	w2, [x21, 72]
	mov	w0, 4
	ldr	x1, .LC71
	and	w2, w2, -16
	add	w2, w2, 256
	str	w2, [x19, 252]
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L17:
	mov	w1, 1
	bfi	w0, w1, 5, 6
	strh	w0, [x29, 60]
	b	.L15
	.p2align 3
.L16:
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
	.size	RV9HAL_V400R003_WritePicMsg, .-RV9HAL_V400R003_WritePicMsg
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
	.global	RV9HAL_V400R003_WriteSliceMsg
	.type	RV9HAL_V400R003_WriteSliceMsg, %function
RV9HAL_V400R003_WriteSliceMsg:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	add	x24, x21, 96
	ldr	x1, .LC73
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w19, [x19, 72]
	and	w19, w19, -16
	add	w22, w19, 256
	mov	w0, w22
	bl	MEM_Phy2Vir
	mov	x20, x0
	cbz	x0, .L38
	ldr	w1, [x21, 132]
	mov	w0, 0
	cbnz	w1, .L39
.L21:
	ldr	w2, [x21, 32]
	cbz	w2, .L18
	lsl	w0, w0, 6
	ldr	x28, .LC77
	ldr	x27, .LC78
	mov	w23, 0
	sxtw	x1, w0
	add	w0, w0, w22
	str	x1, [x29, 104]
	str	w0, [x29, 100]
	.p2align 2
.L31:
	cbz	w23, .L34
	mov	w1, 44
	sub	w0, w23, #1
	umull	x5, w23, w1
	umull	x0, w0, w1
	add	x26, x24, x5
	add	x0, x24, x0
	ldr	w1, [x26, 36]
	ldr	w0, [x0, 36]
	cmp	w1, w0
	ble	.L24
.L23:
	ldr	w2, [x26, 8]
	mov	w1, 0
	ldr	w4, [x26, 16]
	mov	w0, 0
	bfi	w0, w2, 0, 7
	bfi	w1, w4, 0, 24
	str	w1, [x29, 124]
	strb	w0, [x29, 127]
	ubfiz	x4, x23, 6, 26
	ldr	x0, [x29, 104]
	mov	x1, x28
	ldr	w2, [x29, 124]
	add	w23, w23, 1
	add	x4, x4, x0
	mov	w0, 4
	lsl	x19, x4, 2
	str	w2, [x20, x4, lsl 2]
	bl	dprint_vfmw
	ldr	w1, [x26]
	add	x4, x19, 4
	mov	w0, 0
	bfi	w0, w1, 0, 24
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, x4]
	mov	x1, x27
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w0, [x26, 20]
	mov	w1, 0
	ldr	w2, [x26, 12]
	add	x4, x19, 8
	bfi	w1, w0, 0, 24
	mov	w0, 0
	str	w1, [x29, 124]
	bfi	w0, w2, 0, 7
	strb	w0, [x29, 127]
	mov	w0, 4
	ldr	x1, .LC79
	ldr	w2, [x29, 124]
	str	w2, [x20, x4]
	bl	dprint_vfmw
	ldr	w1, [x26, 4]
	add	x4, x19, 12
	mov	w0, 0
	bfi	w0, w1, 0, 24
	ldr	x1, .LC80
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, x4]
	mov	w0, 4
	bl	dprint_vfmw
	str	wzr, [x29, 124]
	ldp	w2, w1, [x26, 24]
	mov	w0, 0
	add	x4, x19, 16
	bfi	w0, w2, 0, 1
	ldr	w2, [x26, 32]
	bfi	w0, w1, 1, 2
	ldr	x1, .LC81
	bfi	w0, w2, 3, 5
	strb	w0, [x29, 126]
	mov	w0, 4
	ldr	w2, [x29, 124]
	str	w2, [x20, x4]
	bl	dprint_vfmw
	ldr	w2, [x21, 32]
	ldr	w4, [x26, 36]
	cmp	w23, w2
	strh	w4, [x29, 124]
	bcs	.L25
	mov	w1, 44
	umull	x0, w23, w1
	add	x0, x24, x0
	ldr	w0, [x0, 36]
	cmp	w4, w0
	bge	.L27
	b	.L25
	.p2align 3
.L28:
	add	x0, x24, x0
	ldr	w0, [x0, 36]
	cmp	w4, w0
	blt	.L25
.L27:
	add	w23, w23, 1
	cmp	w23, w2
	umull	x0, w23, w1
	bcc	.L28
.L25:
	cmp	w2, w23
	beq	.L40
	mov	w0, 44
	ldr	w1, [x29, 100]
	umull	x0, w23, w0
	add	w25, w1, w23, lsl 8
	add	x0, x24, x0
	mov	w22, w25
	ldr	w0, [x0, 36]
	sub	w0, w0, #1
	str	w0, [x26, 40]
.L30:
	add	x5, x19, 20
	strh	w0, [x29, 126]
	ldr	x1, .LC82
	mov	w0, 4
	ldr	w2, [x29, 124]
	add	x19, x19, 252
	str	w2, [x20, x5]
	sub	w23, w23, #1
	bl	dprint_vfmw
	str	w22, [x20, x19]
	ldr	x1, .LC83
	mov	w2, w25
	mov	w0, 4
	str	w25, [x29, 124]
	bl	dprint_vfmw
	ldr	w2, [x21, 32]
.L24:
	add	w23, w23, 1
	cmp	w2, w23
	bhi	.L31
.L18:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L34:
	mov	x26, x24
	b	.L23
	.p2align 3
.L40:
	ldp	w0, w1, [x21, 4]
	mov	w22, 0
	mov	w25, 0
	mul	w0, w0, w1
	sub	w0, w0, #1
	str	w0, [x26, 40]
	b	.L30
.L39:
	ldr	w2, [x24, 8]
	add	w19, w19, 512
	str	wzr, [x29, 124]
	bfi	w0, w2, 0, 7
	strb	w0, [x29, 127]
	ldr	x1, .LC77
	mov	w0, 4
	ldr	w2, [x29, 124]
	str	w2, [x20]
	bl	dprint_vfmw
	ldr	w2, [x21, 96]
	mov	w0, 0
	ldr	x1, .LC78
	bfi	w0, w2, 0, 24
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, 4]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w1, [x24, 12]
	mov	w0, 0
	str	wzr, [x29, 124]
	bfi	w0, w1, 0, 7
	strb	w0, [x29, 127]
	ldr	x1, .LC79
	mov	w0, 4
	ldr	w2, [x29, 124]
	str	w2, [x20, 8]
	bl	dprint_vfmw
	ldr	w2, [x24, 4]
	mov	w0, 0
	ldr	x1, .LC80
	bfi	w0, w2, 0, 24
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, 12]
	mov	w0, 4
	bl	dprint_vfmw
	str	wzr, [x29, 124]
	ldp	w1, w0, [x24, 24]
	mov	w2, 0
	ldr	w3, [x24, 32]
	bfi	w2, w1, 0, 1
	ldr	x1, .LC81
	bfi	w2, w0, 1, 2
	mov	w0, 4
	bfi	w2, w3, 3, 5
	strb	w2, [x29, 126]
	ldr	w2, [x29, 124]
	str	w2, [x20, 16]
	bl	dprint_vfmw
	strh	wzr, [x29, 124]
	ldr	w0, [x24, 36]
	ldr	x1, .LC82
	sub	w0, w0, #1
	strh	w0, [x29, 126]
	mov	w0, 4
	ldr	w2, [x29, 124]
	str	w2, [x20, 20]
	bl	dprint_vfmw
	str	w19, [x20, 252]
	ldr	x1, .LC83
	mov	w2, w19
	mov	w0, 4
	str	w19, [x29, 124]
	bl	dprint_vfmw
	mov	w0, 1
	b	.L21
.L38:
	ldr	x2, .LC75
	mov	w0, 0
	ldr	x3, .LC74
	ldr	x1, .LC76
	add	x2, x2, 64
	bl	dprint_vfmw
	b	.L18
	.size	RV9HAL_V400R003_WriteSliceMsg, .-RV9HAL_V400R003_WriteSliceMsg
	.align	3
.LC73:
	.xword	.LC72
	.align	3
.LC74:
	.xword	.LC33
	.align	3
.LC75:
	.xword	.LANCHOR0
	.align	3
.LC76:
	.xword	.LC36
	.align	3
.LC77:
	.xword	.LC38
	.align	3
.LC78:
	.xword	.LC40
	.align	3
.LC79:
	.xword	.LC42
	.align	3
.LC80:
	.xword	.LC44
	.align	3
.LC81:
	.xword	.LC46
	.align	3
.LC82:
	.xword	.LC48
	.align	3
.LC83:
	.xword	.LC70
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	RV9HAL_V400R003_StartDec
	.type	RV9HAL_V400R003_StartDec, %function
RV9HAL_V400R003_StartDec:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	w19, w1
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	cbz	w19, .L43
	cmp	w19, 1
	bne	.L48
	ldr	x2, .LC86
	mov	w4, 0
	ldr	x1, .LC87
	mov	w3, w19
	add	x2, x2, 96
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L45
	.p2align 3
.L43:
	ldr	x19, .LC88
	ldr	x0, [x19]
	cbz	x0, .L49
.L46:
	ldr	x5, .LC91
	mov	x3, x21
	mov	w2, 0
	mov	x1, x20
	mov	x0, x19
	ldr	w4, [x5]
	add	w4, w4, 1
	str	w4, [x5]
	bl	RV9HAL_V400R003_WriteReg
	mov	x1, x20
	mov	x0, x19
	bl	RV9HAL_V400R003_WritePicMsg
	mov	x0, x19
	mov	x1, x20
	bl	RV9HAL_V400R003_WriteSliceMsg
	mov	w0, 0
.L45:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L48:
	ldr	x1, .LC85
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L49:
	mov	w0, -121438208
	bl	MEM_Phy2Vir
	cbz	x0, .L47
	str	x0, [x19]
	b	.L46
.L47:
	ldr	x1, .LC90
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L45
	.size	RV9HAL_V400R003_StartDec, .-RV9HAL_V400R003_StartDec
	.align	3
.LC85:
	.xword	.LC84
	.align	3
.LC86:
	.xword	.LANCHOR0
	.align	3
.LC87:
	.xword	.LC3
	.align	3
.LC88:
	.xword	g_HwMem
	.align	3
.LC90:
	.xword	.LC89
	.align	3
.LC91:
	.xword	.LANCHOR1
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.12281, %object
	.size	__func__.12281, 25
__func__.12281:
	.string	"RV9HAL_V400R003_WriteReg"
	.zero	7
	.type	__func__.12289, %object
	.size	__func__.12289, 28
__func__.12289:
	.string	"RV9HAL_V400R003_WritePicMsg"
	.zero	4
	.type	__func__.12302, %object
	.size	__func__.12302, 30
__func__.12302:
	.string	"RV9HAL_V400R003_WriteSliceMsg"
	.zero	2
	.type	__func__.12273, %object
	.size	__func__.12273, 25
__func__.12273:
	.string	"RV9HAL_V400R003_StartDec"
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
	ASCII(.string	"BASIC_V400R003_CFG0 = 0x%x\n" )
	.zero	4
.LC7:
	ASCII(.string	"BASIC_V400R003_CFG1=0x%x\n" )
	.zero	6
.LC9:
	ASCII(.string	"AVM_V200R003_ADDR=0x%x\n" )
.LC11:
	ASCII(.string	"VAM_V200R003_ADDR=0x%x\n" )
.LC13:
	ASCII(.string	"STREAM_V400R003_BASE_ADDR=0x%x\n" )
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
	ASCII(.string	"REF_V200R003_PIC_TYPE=0x%x\n" )
	.zero	4
.LC29:
	ASCII(.string	"FF_V200R003_APT_EN=0x%x\n" )
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
	ASCII(.string	"D4 = 0x%x\n" )
	.zero	5
.LC48:
	ASCII(.string	"D5 = 0x%x\n" )
	.zero	5
.LC50:
	ASCII(.string	"D6 = 0x%x\n" )
	.zero	5
.LC52:
	ASCII(.string	"D16 = 0x%x\n" )
	.zero	4
.LC54:
	ASCII(.string	"D17 = 0x%x\n" )
	.zero	4
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
	ASCII(.string	"D024 = 0x%x\n" )
	.zero	3
.LC70:
	ASCII(.string	"D63 = 0x%x\n" )
	.zero	4
.LC72:
	ASCII(.string	"configuring Slice Msg...\n" )
	.zero	6
.LC84:
	ASCII(.string	"VdhId is wrong! RV9HAL_V400R003_StartDec\n" )
	.zero	6
.LC89:
	ASCII(.string	"vdm register virtual address not mapped, reset failed!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
