#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vdm_hal_mpeg4.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	MP4HAL_V400R003_Log2bin
	.type	MP4HAL_V400R003_Log2bin, %function
MP4HAL_V400R003_Log2bin:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	cbz	w19, .L2
	.p2align 2
.L3:
	add	w0, w0, 1
	lsr	w19, w19, 1
	sxth	w0, w0
	cbnz	w19, .L3
.L2:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP4HAL_V400R003_Log2bin, .-MP4HAL_V400R003_Log2bin
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4HAL_V400R003_InitHal
	.type	MP4HAL_V400R003_InitHal, %function
MP4HAL_V400R003_InitHal:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	MP4HAL_V400R003_InitHal, .-MP4HAL_V400R003_InitHal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4HAL_V400R003_CfgReg
	.type	MP4HAL_V400R003_CfgReg, %function
MP4HAL_V400R003_CfgReg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	w19, w1
	mov	x21, x2
	mov	x20, x3
	bl	_mcount
	cmp	w19, wzr
	bgt	.L9
	ldr	w5, [x22, 14588]
	mov	w4, 0
	mov	w23, -1
	cbz	w5, .L11
	.p2align 2
.L27:
	sxtw	x3, w4
	lsl	x1, x3, 3
	sub	x1, x1, x3
	add	x1, x22, x1, lsl 2
	mov	x0, x1
	ldr	w2, [x1, 268]
	cbz	w2, .L16
	ldr	w1, [x1, 260]
	and	w2, w2, -16
	cmp	w23, w2
	cbz	w1, .L16
	csel	w23, w23, w2, ls
.L16:
	ldr	w1, [x0, 272]
	cbz	w1, .L13
	ldr	w0, [x0, 264]
	and	w1, w1, -16
	cmp	w23, w1
	cbz	w0, .L13
	csel	w23, w23, w1, ls
.L13:
	add	w4, w4, 1
	cmp	w4, w5
	bne	.L27
	cmn	w23, #1
	beq	.L11
	str	w23, [x21]
	mov	w21, 1111490560
	mov	x3, x20
	mov	w0, 8
	ldrh	w4, [x22, 176]
	mov	w2, w19
	ldrh	w1, [x22, 178]
	mul	w1, w4, w1
	sub	w1, w1, #1
	and	w1, w1, 1048575
	orr	w21, w1, w21
	mov	w1, w21
	bl	MFDE_ConfigReg
	ldr	x1, .LC6
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	ldrh	w0, [x22, 176]
	cmp	w0, 120
	ble	.L33
	ldr	w2, [x22, 14684]
	mov	w0, 49154
	movk	w0, 0x1, lsl 16
	mov	w1, 0
	ubfiz	w2, w2, 30, 1
	str	wzr, [x22, 14692]
	orr	w0, w2, w0
.L20:
	ldr	w21, [x22, 14688]
	orr	w1, w1, w0
	mov	x3, x20
	mov	w0, 12
	ubfiz	w21, w21, 12, 1
	mov	w2, w19
	orr	w21, w21, w1
	mov	w1, w21
	bl	MFDE_ConfigReg
	ldr	x1, .LC8
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	sxtw	x0, w19
	ldr	x1, .LC9
	lsl	x21, x0, 6
	mov	x3, x20
	sub	x21, x21, x0
	mov	w2, w19
	lsl	x21, x21, 2
	sub	x0, x21, x0
	add	x0, x21, x0, lsl 2
	add	x21, x1, x0
	mov	w0, 16
	ldr	w24, [x21, 72]
	and	w24, w24, -16
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC11
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w24, [x21, 56]
	mov	x3, x20
	mov	w0, 20
	and	w24, w24, -16
	mov	w2, w19
	mov	w1, w24
	bl	MFDE_ConfigReg
	ldr	x1, .LC13
	mov	w2, w24
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x20
	mov	w1, w23
	mov	w2, w19
	mov	w0, 24
	bl	MFDE_ConfigReg
	ldr	x1, .LC15
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldrh	w1, [x22, 176]
	mov	w0, 4
	mov	x3, x20
	mov	w2, w19
	cmp	w1, 120
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
	ldr	w23, [x22, 224]
	mov	x3, x20
	mov	w0, 96
	and	w23, w23, -16
	mov	w2, w19
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC17
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x22, 14616]
	mov	x3, x20
	mov	w0, 100
	mov	w1, w23
	mov	w2, w19
	bl	MFDE_ConfigReg
	ldr	x1, .LC19
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w23, [x22, 14620]
	mov	x3, x20
	mov	w0, 104
	mov	w2, w19
	mov	w1, w23
	bl	MFDE_ConfigReg
	ldr	x1, .LC21
	mov	w2, w23
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w22, [x22, 14628]
	mov	x3, x20
	mov	w0, 108
	mov	w2, w19
	mov	w1, w22
	bl	MFDE_ConfigReg
	ldr	x1, .LC23
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	mov	x3, x20
	mov	w2, w19
	mov	w1, w22
	mov	w0, 152
	bl	MFDE_ConfigReg
	ldr	x1, .LC25
	mov	w2, w22
	mov	w0, 3
	bl	dprint_vfmw
	ldr	w21, [x21, 1220]
	mov	x3, x20
	mov	w0, 144
	mov	w2, w19
	mov	w1, w21
	bl	MFDE_ConfigReg
	ldr	x1, .LC27
	mov	w2, w21
	mov	w0, 3
	bl	dprint_vfmw
	mov	w0, 32
	mov	x3, x20
	mov	w2, w19
	mov	w1, -1
	bl	MFDE_ConfigReg
	mov	w0, 0
.L12:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L33:
	mov	w0, 1
	str	w0, [x22, 14692]
	mov	w0, 49154
	mov	w1, 536870912
	str	wzr, [x22, 14684]
	movk	w0, 0x1, lsl 16
	b	.L20
.L9:
	ldr	x2, .LC0
	mov	w4, 0
	ldr	x1, .LC2
	mov	w3, w19
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L12
.L11:
	ldr	x1, .LC4
	mov	w2, -1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L12
	.size	MP4HAL_V400R003_CfgReg, .-MP4HAL_V400R003_CfgReg
	.align	3
.LC0:
	.xword	.LANCHOR0
	.align	3
.LC2:
	.xword	.LC1
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
.LC9:
	.xword	g_HwMem
	.align	3
.LC11:
	.xword	.LC10
	.align	3
.LC13:
	.xword	.LC12
	.align	3
.LC15:
	.xword	.LC14
	.align	3
.LC17:
	.xword	.LC16
	.align	3
.LC19:
	.xword	.LC18
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4HAL_V400R003_CfgDnMsg
	.type	MP4HAL_V400R003_CfgDnMsg, %function
MP4HAL_V400R003_CfgDnMsg:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	sxtw	x22, w1
	mov	w20, 0
	bl	_mcount
	ldrh	w0, [x19, 176]
	ldrh	w1, [x19, 178]
	mul	w0, w0, w1
	cbz	w0, .L35
	.p2align 2
.L36:
	add	w20, w20, 1
	lsr	w0, w0, 1
	sxth	w20, w20
	cbnz	w0, .L36
.L35:
	lsl	x0, x22, 6
	ldr	x23, .LC28
	sub	x0, x0, x22
	lsl	x0, x0, 2
	sub	x1, x0, x22
	add	x0, x0, x1, lsl 2
	add	x0, x23, x0
	ldr	w0, [x0, 72]
	bl	MEM_Phy2Vir
	mov	x21, x0
	cbz	x0, .L66
	ldrb	w0, [x19, 2]
	cmp	w0, 1
	beq	.L67
	ldr	x1, .LC32
	mov	w2, 0
	mov	w0, 4
	str	wzr, [x21]
	bl	dprint_vfmw
	ldrb	w2, [x19, 3]
	mov	w0, 4
	ldrb	w3, [x19, 1]
	ldr	x1, .LC34
	ubfiz	w2, w2, 26, 5
	orr	w2, w2, w3, lsl 31
	str	w2, [x21, 4]
	bl	dprint_vfmw
	ldrb	w0, [x19]
	cmp	w0, 2
	beq	.L41
	ldrb	w2, [x19, 12]
	ubfiz	w2, w2, 3, 1
.L42:
	ldrb	w3, [x19, 15]
	ubfiz	w1, w0, 5, 2
	ldr	w4, [x19, 14640]
	ubfiz	w20, w20, 22, 4
	and	w3, w3, 1
	ldr	w5, [x19, 14648]
	ubfiz	w4, w4, 26, 2
	orr	w4, w4, w3
	ldr	w3, [x19, 14644]
	ubfiz	w0, w5, 28, 2
	ldrb	w5, [x19, 11]
	orr	w0, w0, w3, lsl 30
	ldrb	w3, [x19, 14]
	orr	w0, w4, w0
	ubfiz	w4, w3, 1, 1
	ldrb	w3, [x19, 13]
	ubfiz	w3, w3, 2, 1
	orr	w3, w4, w3
	orr	w4, w0, w3
	ubfiz	w0, w5, 4, 1
	ldrb	w3, [x19, 10]
	orr	w0, w0, w1
	ldrb	w1, [x19, 9]
	orr	w0, w4, w0
	ubfiz	w3, w3, 7, 1
	ldrb	w4, [x19, 7]
	ubfiz	w1, w1, 8, 3
	orr	w3, w3, w1
	ldrb	w1, [x19, 8]
	orr	w0, w0, w3
	ubfiz	w3, w4, 11, 3
	ldrb	w4, [x19, 6]
	ubfiz	w1, w1, 14, 3
	orr	w1, w3, w1
	ubfiz	w4, w4, 17, 5
	orr	w3, w0, w1
	orr	w20, w4, w20
	ldr	x1, .LC36
	orr	w3, w3, w20
	mov	w0, 4
	orr	w2, w3, w2
	str	w2, [x21, 8]
	bl	dprint_vfmw
	ldrb	w0, [x19]
	cmp	w0, 2
	beq	.L68
.L40:
	ldrh	w2, [x19, 178]
	mov	w0, 4
	ldrh	w4, [x19, 176]
	sub	w3, w2, #1
	ldr	x1, .LC40
	sub	w4, w4, #1
	and	w2, w4, 65535
	orr	w2, w2, w3, lsl 16
	str	w2, [x21, 16]
	bl	dprint_vfmw
	ldr	w2, [x19, 14632]
	mov	w0, 4
	ldr	w3, [x19, 14636]
	ldr	x1, .LC42
	orr	w2, w3, w2, lsl 16
	str	w2, [x21, 20]
	bl	dprint_vfmw
	ldrb	w0, [x19]
	cmp	w0, 3
	beq	.L69
.L43:
	ldr	x1, .LC48
	mov	w2, 0
	mov	w0, 4
	str	wzr, [x21, 32]
	lsl	x20, x22, 6
	bl	dprint_vfmw
	str	wzr, [x21, 36]
	ldr	x1, .LC50
	mov	w2, 0
	mov	w0, 4
	sub	x20, x20, x22
	lsl	x20, x20, 2
	bl	dprint_vfmw
	str	wzr, [x21, 40]
	ldr	x1, .LC52
	mov	w2, 0
	mov	w0, 4
	bl	dprint_vfmw
	str	wzr, [x21, 44]
	ldr	x1, .LC54
	mov	w2, 0
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w2, [x19, 14604]
	mov	w0, 4
	ldr	x1, .LC56
	and	w2, w2, -16
	str	w2, [x21, 48]
	bl	dprint_vfmw
	ldr	w2, [x19, 232]
	mov	w0, 4
	ldr	x1, .LC58
	and	w2, w2, -16
	str	w2, [x21, 52]
	bl	dprint_vfmw
	ldr	w2, [x19, 236]
	mov	w0, 4
	ldr	x1, .LC60
	and	w2, w2, -16
	str	w2, [x21, 56]
	bl	dprint_vfmw
	ldr	w2, [x19, 240]
	mov	w0, 4
	ldr	x1, .LC62
	and	w2, w2, -16
	str	w2, [x21, 60]
	bl	dprint_vfmw
	ldr	w2, [x19, 244]
	mov	w0, 4
	ldr	x1, .LC64
	and	w2, w2, -16
	str	w2, [x21, 64]
	bl	dprint_vfmw
	sub	x2, x20, x22
	ldr	x1, .LC66
	mov	w0, 4
	add	x20, x20, x2, lsl 2
	add	x20, x23, x20
	ldr	w2, [x20, 1172]
	and	w2, w2, -16
	str	w2, [x21, 68]
	bl	dprint_vfmw
	ldr	w2, [x20, 1164]
	mov	w0, 4
	ldr	x1, .LC68
	and	w2, w2, -16
	str	w2, [x21, 72]
	bl	dprint_vfmw
	strb	wzr, [x19, 165]
	ldrb	w0, [x19]
	cmp	w0, 3
	beq	.L70
.L49:
	ldrb	w0, [x19, 161]
	strb	w0, [x19, 160]
	and	w0, w0, 1
	lsl	w3, w0, 3
.L51:
	ldrb	w4, [x19, 163]
	orr	w3, w3, w0, lsl 2
	ldrb	w2, [x19, 164]
	and	w0, w4, 1
	ldr	x1, .LC70
	ubfiz	w2, w2, 1, 1
	orr	w2, w2, w0
	mov	w0, 4
	orr	w2, w2, w3
	str	w2, [x21, 76]
	bl	dprint_vfmw
	ldr	w2, [x19, 14652]
	mov	w0, 4
	ldr	x1, .LC72
	str	w2, [x21, 80]
	bl	dprint_vfmw
	ldr	w2, [x19, 14656]
	mov	w0, 4
	ldr	x1, .LC74
	str	w2, [x21, 84]
	bl	dprint_vfmw
	ldr	w2, [x19, 14660]
	mov	w0, 4
	ldr	x1, .LC76
	str	w2, [x21, 88]
	bl	dprint_vfmw
	ldr	w2, [x19, 14664]
	mov	w0, 4
	ldr	x1, .LC78
	str	w2, [x21, 92]
	bl	dprint_vfmw
	ldr	w2, [x19, 14668]
	mov	w0, 4
	ldr	x1, .LC80
	str	w2, [x21, 96]
	bl	dprint_vfmw
	ldr	w2, [x19, 14672]
	mov	w0, 4
	ldr	x1, .LC82
	str	w2, [x21, 100]
	bl	dprint_vfmw
	ldr	w2, [x19, 14676]
	mov	w0, 4
	ldr	x1, .LC84
	str	w2, [x21, 104]
	bl	dprint_vfmw
	ldr	w2, [x19, 14680]
	mov	w0, 4
	ldr	x1, .LC86
	str	w2, [x21, 108]
	bl	dprint_vfmw
	ldrb	w0, [x19, 2]
	cmp	w0, 2
	beq	.L71
.L52:
	lsl	x2, x22, 6
	ldr	x1, .LC90
	sub	x2, x2, x22
	mov	w0, 4
	lsl	x2, x2, 2
	sub	x22, x2, x22
	add	x2, x2, x22, lsl 2
	add	x2, x23, x2
	ldr	w3, [x2, 1160]
	and	w3, w3, -16
	str	w3, [x21, 240]
	ldr	w2, [x2, 72]
	add	w2, w2, 256
	str	w2, [x21, 252]
	bl	dprint_vfmw
	mov	w0, 0
.L38:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L67:
	mov	w24, 4
	ldr	x1, .LC32
	mov	w2, w24
	mov	w0, w24
	str	w24, [x21]
	ubfiz	w20, w20, 22, 4
	bl	dprint_vfmw
	ldrb	w2, [x19, 4]
	ldrb	w1, [x19, 3]
	ldrb	w0, [x19, 5]
	ubfiz	w2, w2, 5, 7
	ubfiz	w1, w1, 26, 5
	sub	w0, w0, #1
	orr	w1, w2, w1
	and	w0, w0, 31
	orr	w0, w1, w0
	ldr	x1, .LC34
	mov	w2, w0
	str	w0, [x21, 4]
	mov	w0, w24
	bl	dprint_vfmw
	ldrb	w2, [x19]
	mov	w0, w24
	ldr	x1, .LC36
	ubfiz	w2, w2, 5, 2
	orr	w2, w2, w20
	str	w2, [x21, 8]
	bl	dprint_vfmw
	b	.L40
	.p2align 3
.L71:
	ldrb	w0, [x19, 1]
	cbz	w0, .L52
	ldr	x20, .LC88
	mov	w27, 0
	mov	w28, 0
	.p2align 2
.L55:
	mov	w24, w28
	mov	w26, w27
	mov	w25, 0
.L54:
	add	w5, w24, 16
	add	w2, w24, 32
	add	w1, w24, 48
	add	x0, x19, x24, sxtw
	add	x5, x19, x5, sxtw
	add	x2, x19, x2, sxtw
	add	x1, x19, x1, sxtw
	sxtw	x4, w26
	ldrb	w3, [x0, 32]
	add	x4, x4, 112
	ldrb	w6, [x5, 32]
	add	w24, w24, 8
	ldrb	w5, [x2, 32]
	add	w2, w25, w25, lsr 31
	ldrb	w0, [x1, 32]
	mov	x1, x20
	lsl	w5, w5, 16
	add	w2, w27, w2, asr 1
	orr	w5, w5, w6, lsl 8
	add	w2, w2, 24
	orr	w0, w3, w0, lsl 24
	add	w25, w25, 1
	orr	w3, w5, w0
	mov	w0, 4
	str	w3, [x21, x4]
	add	w26, w26, 4
	bl	dprint_vfmw
	cmp	w25, 2
	bne	.L54
	add	w28, w28, 1
	add	w27, w27, 8
	cmp	w28, 8
	bne	.L55
	mov	w24, 64
	mov	w25, 0
	.p2align 2
.L58:
	lsl	w0, w25, 3
	mov	w26, w25
	mov	w28, w24
	mov	w27, 0
	str	w0, [x29, 108]
.L57:
	add	w5, w26, 16
	add	w2, w26, 32
	add	w1, w26, 48
	add	x0, x19, x26, sxtw
	add	x5, x19, x5, sxtw
	add	x2, x19, x2, sxtw
	add	x1, x19, x1, sxtw
	sxtw	x4, w28
	ldrb	w3, [x0, 96]
	add	x4, x4, 112
	ldrb	w6, [x5, 96]
	add	w26, w26, 8
	ldrb	w5, [x2, 96]
	add	w2, w27, w27, lsr 31
	ldrb	w0, [x1, 96]
	mov	x1, x20
	lsl	w5, w5, 16
	add	w27, w27, 1
	orr	w5, w5, w6, lsl 8
	ldr	w6, [x29, 108]
	orr	w0, w3, w0, lsl 24
	add	w28, w28, 4
	add	w2, w6, w2, asr 1
	orr	w3, w5, w0
	add	w2, w2, 24
	mov	w0, 4
	str	w3, [x21, x4]
	bl	dprint_vfmw
	cmp	w27, 2
	bne	.L57
	add	w25, w25, 1
	add	w24, w24, 8
	cmp	w25, 8
	bne	.L58
	b	.L52
	.p2align 3
.L70:
	ldr	w0, [x19, 14644]
	cmp	w0, 1
	beq	.L72
	bhi	.L49
	ldrb	w3, [x19, 160]
	ldrb	w1, [x19, 161]
	ubfiz	w3, w3, 3, 1
	and	w0, w1, 1
	b	.L51
	.p2align 3
.L69:
	ldr	w0, [x19, 14644]
	cmp	w0, 1
	bne	.L43
	ldrb	w20, [x19, 162]
	ldrb	w2, [x19, 11]
	cmp	w20, 1
	beq	.L73
	ldr	w1, [x19, 14668]
	lsl	w1, w1, w2
	ldr	w2, [x19, 14648]
	cmp	w1, wzr
	lsl	w0, w0, w2
	add	w0, w1, w0, asr 1
	ble	.L74
.L65:
	ldr	x1, .LC44
	asr	w2, w0, w2
	mov	w0, 4
	str	w2, [x21, 24]
	bl	dprint_vfmw
	ldrb	w3, [x19, 11]
	ldr	w0, [x19, 14672]
	ldr	w2, [x19, 14648]
	lsl	w3, w0, w3
	cmp	w3, wzr
	ble	.L47
	mov	w0, 1
	lsl	w0, w0, w2
	add	w0, w3, w0, asr 1
	asr	w2, w0, w2
.L48:
	ldr	x1, .LC46
	mov	w0, 4
	str	w2, [x21, 28]
	bl	dprint_vfmw
	b	.L43
	.p2align 3
.L41:
	strb	wzr, [x19, 12]
	mov	w2, 0
	b	.L42
.L68:
	ldp	w3, w2, [x19, 216]
	ldr	x1, .LC38
	mov	w0, 4
	ubfiz	w3, w3, 1, 15
	orr	w2, w3, w2, lsl 17
	str	w2, [x21, 12]
	bl	dprint_vfmw
	b	.L40
.L72:
	ldrb	w1, [x19, 161]
	mov	w3, 8
	strb	w0, [x19, 160]
	and	w0, w1, 1
	b	.L51
.L47:
	mov	w1, 1
	lsl	w1, w1, w2
	add	w0, w3, w1, asr 1
	sub	w0, w0, #1
	asr	w2, w0, w2
	b	.L48
.L74:
	sub	w0, w0, #1
	b	.L65
.L73:
	ldr	w0, [x19, 14648]
	ldr	w3, [x19, 14668]
	sub	w2, w0, w2
	ldr	x1, .LC44
	lsl	w2, w20, w2
	mov	w0, 4
	sdiv	w2, w3, w2
	str	w2, [x21, 24]
	bl	dprint_vfmw
	ldrb	w4, [x19, 11]
	mov	w0, 4
	ldr	w3, [x19, 14648]
	ldr	w2, [x19, 14672]
	sub	w3, w3, w4
	ldr	x1, .LC46
	lsl	w20, w20, w3
	sdiv	w2, w2, w20
	str	w2, [x21, 28]
	bl	dprint_vfmw
	b	.L43
.L66:
	ldr	x1, .LC30
	mov	w2, 333
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L38
	.size	MP4HAL_V400R003_CfgDnMsg, .-MP4HAL_V400R003_CfgDnMsg
	.align	3
.LC28:
	.xword	g_HwMem
	.align	3
.LC30:
	.xword	.LC29
	.align	3
.LC32:
	.xword	.LC31
	.align	3
.LC34:
	.xword	.LC33
	.align	3
.LC36:
	.xword	.LC35
	.align	3
.LC38:
	.xword	.LC37
	.align	3
.LC40:
	.xword	.LC39
	.align	3
.LC42:
	.xword	.LC41
	.align	3
.LC44:
	.xword	.LC43
	.align	3
.LC46:
	.xword	.LC45
	.align	3
.LC48:
	.xword	.LC47
	.align	3
.LC50:
	.xword	.LC49
	.align	3
.LC52:
	.xword	.LC51
	.align	3
.LC54:
	.xword	.LC53
	.align	3
.LC56:
	.xword	.LC55
	.align	3
.LC58:
	.xword	.LC57
	.align	3
.LC60:
	.xword	.LC59
	.align	3
.LC62:
	.xword	.LC61
	.align	3
.LC64:
	.xword	.LC63
	.align	3
.LC66:
	.xword	.LC65
	.align	3
.LC68:
	.xword	.LC67
	.align	3
.LC70:
	.xword	.LC69
	.align	3
.LC72:
	.xword	.LC71
	.align	3
.LC74:
	.xword	.LC73
	.align	3
.LC76:
	.xword	.LC75
	.align	3
.LC78:
	.xword	.LC77
	.align	3
.LC80:
	.xword	.LC79
	.align	3
.LC82:
	.xword	.LC81
	.align	3
.LC84:
	.xword	.LC83
	.align	3
.LC86:
	.xword	.LC85
	.align	3
.LC88:
	.xword	.LC87
	.align	3
.LC90:
	.xword	.LC89
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4HAL_V400R003_WriteSlicMsg
	.type	MP4HAL_V400R003_WriteSlicMsg, %function
MP4HAL_V400R003_WriteSlicMsg:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	mov	w22, w2
	str	x20, [x29, 96]
	bl	_mcount
	sxtw	x0, w19
	ldr	x2, .LC91
	lsl	x1, x0, 6
	ldr	w25, [x20, 14588]
	sub	x1, x1, x0
	add	x26, x20, 252
	lsl	x1, x1, 2
	str	wzr, [x29, 124]
	sub	x0, x1, x0
	add	x0, x1, x0, lsl 2
	add	x0, x2, x0
	ldr	w21, [x0, 72]
	add	w19, w21, 256
	mov	w0, w19
	bl	MEM_Phy2Vir
	mov	x20, x0
	cbz	x0, .L98
	ldr	w0, [x26, 24]
	mov	w23, 0
	cbnz	w0, .L99
.L78:
	cmp	w25, wzr
	ble	.L94
	ldr	x28, .LC96
	add	w0, w19, w23, lsl 5
	mov	w24, 0
	str	w0, [x29, 108]
	.p2align 2
.L93:
	sxtw	x19, w24
	cmp	w24, wzr
	lsl	x21, x19, 3
	sub	x21, x21, x19
	add	x21, x26, x21, lsl 2
	ble	.L83
	ldr	w0, [x21, -4]
	ldr	w1, [x21, 24]
	cmp	w1, w0
	bls	.L84
.L83:
	ldr	w27, [x21, 16]
	mov	w2, 0
	ldr	w7, [x21, 8]
	mov	w1, 0
	ldrb	w0, [x21, 6]
	ubfiz	w8, w27, 3, 4
	bfi	w2, w7, 0, 24
	str	w2, [x29, 124]
	add	w0, w8, w0
	add	x19, x19, x23, sxtw 2
	bfi	w1, w0, 0, 7
	strb	w1, [x29, 127]
	lsl	x19, x19, 5
	mov	x1, x28
	ldr	w7, [x29, 124]
	mov	w0, 4
	mov	w2, w7
	str	w7, [x20, x19]
	bl	dprint_vfmw
	add	x7, x19, 4
	and	w6, w27, -16
	sub	w6, w6, w22
	mov	w0, 0
	bfi	w0, w6, 0, 24
	ldr	x1, .LC98
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, x7]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w27, [x21, 20]
	ldr	w1, [x21, 12]
	mov	w2, 0
	ldrb	w0, [x21, 7]
	ubfiz	w7, w27, 3, 4
	bfi	w2, w1, 0, 24
	mov	w1, 0
	str	w2, [x29, 124]
	add	w0, w7, w0
	bfi	w1, w0, 0, 7
	add	x7, x19, 8
	strb	w1, [x29, 127]
	mov	w0, 4
	ldr	x1, .LC100
	ldr	w2, [x29, 124]
	str	w2, [x20, x7]
	bl	dprint_vfmw
	ldr	w0, [x21, 20]
	cbz	w0, .L85
	and	w6, w27, -16
	mov	w0, 0
	sub	w6, w6, w22
	bfi	w0, w6, 0, 24
	str	w0, [x29, 124]
.L86:
	add	x6, x19, 12
	ldr	w7, [x29, 124]
	ldr	x1, .LC102
	mov	w0, 4
	mov	w2, w7
	add	w24, w24, 1
	str	w7, [x20, x6]
	bl	dprint_vfmw
	str	wzr, [x29, 124]
	ldrb	w2, [x21, 5]
	mov	w0, 0
	ldrb	w1, [x21, 4]
	add	x6, x19, 16
	bfi	w0, w2, 0, 5
	ldrb	w9, [x21, 3]
	bfi	w0, w1, 5, 2
	strb	w0, [x29, 124]
	ldrb	w8, [x21, 1]
	mov	w0, 4
	ldrh	w2, [x29, 124]
	ldrb	w7, [x21, 2]
	bfi	w2, w9, 7, 3
	strh	w2, [x29, 124]
	lsr	x2, x2, 8
	ldr	x1, .LC104
	bfi	w2, w8, 2, 3
	bfi	w2, w7, 5, 3
	strb	w2, [x29, 125]
	ldr	w2, [x29, 124]
	str	w2, [x20, x6]
	bl	dprint_vfmw
	ldr	w1, [x21, 24]
	add	x6, x19, 20
	mov	w0, 0
	bfi	w0, w1, 0, 20
	ldr	x1, .LC106
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, x6]
	mov	w0, 4
	bl	dprint_vfmw
	cmp	w25, w24
	ble	.L87
	sxtw	x1, w24
	ldr	w2, [x21, 24]
	lsl	x0, x1, 3
	sub	x0, x0, x1
	add	x0, x26, x0, lsl 2
	ldr	w0, [x0, 24]
	cmp	w2, w0
	bcs	.L89
	b	.L87
	.p2align 3
.L90:
	sub	x0, x0, x1
	add	x0, x26, x0, lsl 2
	ldr	w0, [x0, 24]
	cmp	w0, w2
	bhi	.L87
.L89:
	add	w24, w24, 1
	cmp	w25, w24
	sxtw	x1, w24
	lsl	x0, x1, 3
	bgt	.L90
.L87:
	cmp	w25, w24
	beq	.L100
	ldr	w0, [x29, 108]
	sxtw	x1, w24
	add	w27, w0, w24, lsl 5
	lsl	x0, x1, 3
	sub	x0, x0, x1
	mov	w21, w27
	add	x0, x26, x0, lsl 2
	ldr	w1, [x0, 24]
	sub	w1, w1, #1
.L92:
	add	x7, x19, 24
	mov	w0, 0
	bfi	w0, w1, 0, 20
	ldr	x1, .LC108
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, x7]
	mov	w0, 4
	add	x19, x19, 28
	sub	w24, w24, #1
	bl	dprint_vfmw
	str	w27, [x29, 124]
	ldr	x1, .LC110
	mov	w2, w27
	mov	w0, 4
	str	w21, [x20, x19]
	bl	dprint_vfmw
.L84:
	add	w24, w24, 1
	cmp	w25, w24
	bgt	.L93
.L94:
	mov	w0, 0
.L77:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L85:
	str	wzr, [x29, 124]
	b	.L86
	.p2align 3
.L100:
	ldr	x1, [x29, 96]
	mov	w21, 0
	mov	w27, 0
	ldrh	w0, [x1, 176]
	ldrh	w1, [x1, 178]
	mul	w1, w0, w1
	sub	w1, w1, #1
	b	.L92
.L99:
	ldr	w23, [x26, 16]
	ldr	w0, [x29, 124]
	ldrb	w1, [x26, 6]
	ubfiz	w2, w23, 3, 4
	and	w0, w0, -16777216
	str	w0, [x29, 124]
	add	w1, w2, w1
	lsr	w0, w0, 24
	bfi	w0, w1, 0, 7
	strb	w0, [x29, 127]
	ldr	x1, .LC96
	mov	w0, 4
	ldr	w2, [x29, 124]
	str	w2, [x20]
	bl	dprint_vfmw
	and	w0, w23, -16
	mov	w3, 0
	sub	w0, w0, w22
	ldr	x1, .LC98
	bfi	w3, w0, 0, 24
	str	w3, [x29, 124]
	mov	w2, w3
	str	w3, [x20, 4]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w23, [x26, 20]
	ldrb	w0, [x26, 7]
	ubfiz	w1, w23, 3, 4
	str	wzr, [x29, 124]
	add	w0, w1, w0
	mov	w1, 0
	bfi	w1, w0, 0, 7
	strb	w1, [x29, 127]
	ldr	x1, .LC100
	mov	w0, 4
	ldr	w2, [x29, 124]
	str	w2, [x20, 8]
	bl	dprint_vfmw
	ldr	w0, [x26, 20]
	cbnz	w0, .L101
	str	wzr, [x29, 124]
.L80:
	ldr	w2, [x29, 124]
	mov	w0, 4
	ldr	x1, .LC102
	add	w21, w21, 288
	str	w2, [x20, 12]
	mov	w23, 1
	bl	dprint_vfmw
	str	wzr, [x29, 124]
	ldrb	w2, [x26, 5]
	mov	w0, 0
	ldrb	w1, [x26, 4]
	bfi	w0, w2, 0, 5
	ldrb	w2, [x26, 3]
	bfi	w0, w1, 5, 2
	strb	w0, [x29, 124]
	ldrb	w1, [x26, 1]
	ldrh	w0, [x29, 124]
	bfi	w0, w2, 7, 3
	strh	w0, [x29, 124]
	lsr	x0, x0, 8
	ldrb	w2, [x26, 2]
	bfi	w0, w1, 2, 3
	ldr	x1, .LC104
	bfi	w0, w2, 5, 3
	strb	w0, [x29, 125]
	mov	w0, 4
	ldr	w2, [x29, 124]
	str	w2, [x20, 16]
	bl	dprint_vfmw
	str	wzr, [x29, 124]
	ldr	x1, .LC106
	mov	w2, 0
	str	wzr, [x20, 20]
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w2, [x26, 24]
	mov	w0, 0
	ldr	x1, .LC108
	sub	w2, w2, #1
	bfi	w0, w2, 0, 20
	str	w0, [x29, 124]
	mov	w2, w0
	str	w0, [x20, 24]
	mov	w0, 4
	bl	dprint_vfmw
	str	w21, [x20, 28]
	ldr	x1, .LC110
	mov	w2, w21
	mov	w0, 4
	str	w21, [x29, 124]
	bl	dprint_vfmw
	b	.L78
.L101:
	and	w0, w23, -16
	mov	w1, 0
	sub	w0, w0, w22
	bfi	w1, w0, 0, 24
	str	w1, [x29, 124]
	b	.L80
.L98:
	ldr	x2, .LC92
	mov	w0, 0
	ldr	x1, .LC94
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, -1
	b	.L77
	.size	MP4HAL_V400R003_WriteSlicMsg, .-MP4HAL_V400R003_WriteSlicMsg
	.align	3
.LC91:
	.xword	g_HwMem
	.align	3
.LC92:
	.xword	.LANCHOR0
	.align	3
.LC94:
	.xword	.LC93
	.align	3
.LC96:
	.xword	.LC95
	.align	3
.LC98:
	.xword	.LC97
	.align	3
.LC100:
	.xword	.LC99
	.align	3
.LC102:
	.xword	.LC101
	.align	3
.LC104:
	.xword	.LC103
	.align	3
.LC106:
	.xword	.LC105
	.align	3
.LC108:
	.xword	.LC107
	.align	3
.LC110:
	.xword	.LC109
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4HAL_V400R003_StartDec
	.type	MP4HAL_V400R003_StartDec, %function
MP4HAL_V400R003_StartDec:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	x21, x2
	bl	_mcount
	str	wzr, [x29, 60]
	ldrh	w0, [x19, 176]
	cmp	w0, 512
	bhi	.L105
	ldrh	w0, [x19, 178]
	cmp	w0, 512
	bhi	.L105
	mov	x3, x21
	add	x2, x29, 60
	mov	w1, w20
	mov	x0, x19
	bl	MP4HAL_V400R003_CfgReg
	cbnz	w0, .L110
	mov	w1, w20
	mov	x0, x19
	bl	MP4HAL_V400R003_CfgDnMsg
	cbnz	w0, .L111
	ldr	w2, [x29, 60]
	mov	w1, w20
	mov	x0, x19
	bl	MP4HAL_V400R003_WriteSlicMsg
	mov	w1, 0
	cbnz	w0, .L112
.L104:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, w1
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L105:
	ldr	x2, .LC113
	mov	w0, 0
	ldr	x1, .LC115
	ldr	x3, .LC112
	add	x2, x2, 56
	bl	dprint_vfmw
	mov	w1, -1
	b	.L104
	.p2align 3
.L110:
	ldr	x1, .LC117
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L104
	.p2align 3
.L111:
	ldr	x1, .LC119
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L104
	.p2align 3
.L112:
	ldr	x1, .LC121
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L104
	.size	MP4HAL_V400R003_StartDec, .-MP4HAL_V400R003_StartDec
	.align	3
.LC112:
	.xword	.LC111
	.align	3
.LC113:
	.xword	.LANCHOR0
	.align	3
.LC115:
	.xword	.LC114
	.align	3
.LC117:
	.xword	.LC116
	.align	3
.LC119:
	.xword	.LC118
	.align	3
.LC121:
	.xword	.LC120
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.12017, %object
	.size	__func__.12017, 23
__func__.12017:
	.string	"MP4HAL_V400R003_CfgReg"
	.zero	1
	.type	__func__.12068, %object
	.size	__func__.12068, 29
__func__.12068:
	.string	"MP4HAL_V400R003_WriteSlicMsg"
	.zero	3
	.type	__func__.12005, %object
	.size	__func__.12005, 25
__func__.12005:
	.string	"MP4HAL_V400R003_StartDec"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC1:
	ASCII(.string	"%s: VdhId(%d) > %d\n" )
	.zero	4
.LC3:
	ASCII(.string	"stream_base_addr = %#x\n" )
.LC5:
	ASCII(.string	"BASIC_V400R003_CFG0=0x%x\n" )
	.zero	6
.LC7:
	ASCII(.string	"BASIC_V400R003_CFG1=0x%x\n" )
	.zero	6
.LC10:
	ASCII(.string	"AVM_V400R003_ADDR=0x%x\n" )
.LC12:
	ASCII(.string	"VAM_V400R003_ADDR=0x%x\n" )
.LC14:
	ASCII(.string	"STREAM_V400R003_BASE_ADDR=0x%x\n" )
.LC16:
	ASCII(.string	"YSTADDR_V200R003_1D=0x%x\n" )
	.zero	6
.LC18:
	ASCII(.string	"YSTRIDE_V200R003_1D=0x%x\n" )
	.zero	6
.LC20:
	ASCII(.string	"UVOFFSET_V200R003_1D=0x%x\n" )
	.zero	5
.LC22:
	ASCII(.string	"PRCNUM_1D_CNT=0x%x\n" )
	.zero	4
.LC24:
	ASCII(.string	"PRCMEM2_1D_CNT=0x%x\n" )
	.zero	3
.LC26:
	ASCII(.string	"DNR_MBINFO_STADDR=0x%x\n" )
.LC29:
	ASCII(.string	"line: %d ,pMsgBlock = NULL is not expected value!\n" )
	.zero	5
.LC31:
	ASCII(.string	"D0=0x%x\n" )
	.zero	7
.LC33:
	ASCII(.string	"D1=0x%x\n" )
	.zero	7
.LC35:
	ASCII(.string	"D2=0x%x\n" )
	.zero	7
.LC37:
	ASCII(.string	"D3=0x%x\n" )
	.zero	7
.LC39:
	ASCII(.string	"D4=0x%x\n" )
	.zero	7
.LC41:
	ASCII(.string	"D5=0x%x\n" )
	.zero	7
.LC43:
	ASCII(.string	"D6=0x%x\n" )
	.zero	7
.LC45:
	ASCII(.string	"D7=0x%x\n" )
	.zero	7
.LC47:
	ASCII(.string	"D8=0x%x\n" )
	.zero	7
.LC49:
	ASCII(.string	"D9=0x%x\n" )
	.zero	7
.LC51:
	ASCII(.string	"D10=0x%x\n" )
	.zero	6
.LC53:
	ASCII(.string	"D11=0x%x\n" )
	.zero	6
.LC55:
	ASCII(.string	"D12= 0x%x\n" )
	.zero	5
.LC57:
	ASCII(.string	"D13= 0x%x\n" )
	.zero	5
.LC59:
	ASCII(.string	"D14= 0x%x\n" )
	.zero	5
.LC61:
	ASCII(.string	"D15= 0x%x\n" )
	.zero	5
.LC63:
	ASCII(.string	"D16= 0x%x\n" )
	.zero	5
.LC65:
	ASCII(.string	"D17= 0x%x\n" )
	.zero	5
.LC67:
	ASCII(.string	"D18= 0x%x\n" )
	.zero	5
.LC69:
	ASCII(.string	"D19= 0x%x\n" )
	.zero	5
.LC71:
	ASCII(.string	"D20= 0x%x\n" )
	.zero	5
.LC73:
	ASCII(.string	"D21= 0x%x\n" )
	.zero	5
.LC75:
	ASCII(.string	"D22= 0x%x\n" )
	.zero	5
.LC77:
	ASCII(.string	"D23= 0x%x\n" )
	.zero	5
.LC79:
	ASCII(.string	"D24= 0x%x\n" )
	.zero	5
.LC81:
	ASCII(.string	"D25= 0x%x\n" )
	.zero	5
.LC83:
	ASCII(.string	"D26= 0x%x\n" )
	.zero	5
.LC85:
	ASCII(.string	"D27= 0x%x\n" )
	.zero	5
.LC87:
	ASCII(.string	"D%d= 0x%x\n" )
	.zero	5
.LC89:
	ASCII(.string	"D63= 0x%x\n" )
	.zero	5
.LC93:
	ASCII(.string	"%s: SlcDnMsgVirAddr = NULL\n" )
	.zero	4
.LC95:
	ASCII(.string	"D0 = %#x \n" )
	.zero	5
.LC97:
	ASCII(.string	"D1 = %#x \n" )
	.zero	5
.LC99:
	ASCII(.string	"D2 = %#x \n" )
	.zero	5
.LC101:
	ASCII(.string	"D3 = %#x \n" )
	.zero	5
.LC103:
	ASCII(.string	"D4 = %#x \n" )
	.zero	5
.LC105:
	ASCII(.string	"D5 = %#x \n" )
	.zero	5
.LC107:
	ASCII(.string	"D6 = %#x \n" )
	.zero	5
.LC109:
	ASCII(.string	"D7 = %#x \n" )
	.zero	5
.LC111:
	ASCII(.string	"picture width out of range" )
	.zero	5
.LC114:
	ASCII(.string	"%s: %s\n" )
.LC116:
	ASCII(.string	"MP4HAL_V200R003_CfgReg failed!\n" )
.LC118:
	ASCII(.string	"MP4HAL_V200R003_CfgDnMsg failed!\n" )
	.zero	6
.LC120:
	ASCII(.string	"MP4HAL_V200R003_WriteSlicMsg failed!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
