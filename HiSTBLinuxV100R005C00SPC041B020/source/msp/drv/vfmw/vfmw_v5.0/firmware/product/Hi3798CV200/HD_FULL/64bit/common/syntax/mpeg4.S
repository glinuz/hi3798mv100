#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"mpeg4.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	log2bin
	.type	log2bin, %function
log2bin:
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
	.size	log2bin, .-log2bin
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	bs_get_spritetrajectory
	.type	bs_get_spritetrajectory, %function
bs_get_spritetrajectory:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w20, 0
	bl	_mcount
	ldr	x22, .LC3
	add	x19, x22, 4
	.p2align 2
.L10:
	ldrb	w1, [x19]
	mov	x0, x21
	add	x19, x19, 8
	bl	BsShow
	ldr	w1, [x19, -12]
	cmp	w0, w1
	beq	.L12
	add	w20, w20, 1
	cmp	w20, 12
	bne	.L10
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L12:
	add	x22, x22, x20, uxtw 3
	mov	x0, x21
	ldrb	w1, [x22, 4]
	bl	BsSkip
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	bs_get_spritetrajectory, .-bs_get_spritetrajectory
	.align	3
.LC3:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_GetGMCparam
	.type	MP4_GetGMCparam, %function
MP4_GetGMCparam:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	w8, [x20, 708]
	mov	w11, 2
	ldrb	w0, [x20, 65]
	mov	w9, 3
	ldr	w1, [x20, 704]
	lsl	w2, w8, 4
	str	w2, [x19, 8]
	lsl	w11, w11, w0
	sub	w9, w9, w0
	mov	w5, 16
	ldr	w0, [x20, 704]
	cmp	w8, 1
	sdiv	w12, w5, w11
	lsl	w0, w0, 4
	str	w0, [x19, 12]
	ldrb	w0, [x20, 65]
	str	w0, [x19, 4]
	add	x0, x20, 512
	ldrb	w14, [x20, 64]
	str	w14, [x19]
	ldp	w13, w10, [x0, 168]
	ldp	w6, w7, [x0, 176]
	ldr	w17, [x20, 696]
	ldr	w16, [x20, 700]
	ble	.L38
	mov	w2, 0
	mov	w0, 1
	.p2align 2
.L15:
	add	w2, w2, 1
	lsl	w3, w0, w2
	cmp	w8, w3
	bgt	.L15
	lsl	w30, w3, 4
.L14:
	cmp	w1, 1
	ble	.L39
	mov	w0, 0
	mov	w15, 1
	.p2align 2
.L17:
	add	w0, w0, 1
	lsl	w4, w15, w0
	cmp	w1, w4
	bgt	.L17
	lsl	w15, w4, 4
.L16:
	ldrb	w5, [x20, 12]
	add	w7, w10, w7
	add	w17, w13, w17
	cmp	w5, 1
	beq	.L44
	add	w5, w13, w8, lsl 1
	add	w6, w5, w6
	add	w5, w10, w1, lsl 1
	add	w16, w5, w16
	asr	w5, w11, 1
	mul	w13, w13, w5
	mul	w10, w10, w5
	mul	w6, w6, w5
	mul	w7, w7, w5
	mul	w17, w17, w5
	mul	w16, w16, w5
.L19:
	neg	w5, w8, lsl 4
	sub	w23, w8, w3
	mul	w21, w12, w13
	madd	w6, w12, w6, w5
	asr	w22, w8, 1
	mul	w5, w23, w21
	madd	w5, w6, w3, w5
	cmp	w5, wzr
	ble	.L20
	add	w5, w5, w22
.L21:
	sdiv	w5, w5, w8
	mul	w20, w12, w10
	mul	w7, w12, w7
	mul	w18, w12, w17
	add	w5, w5, w30
	sub	w30, w1, w4
	mul	w6, w23, w20
	madd	w6, w7, w3, w6
	mul	w17, w21, w30
	madd	w17, w18, w4, w17
	cmp	w6, wzr
	add	w7, w6, w22
	sub	w6, w6, w22
	asr	w18, w1, 1
	csel	w6, w6, w7, le
	cmp	w17, wzr
	sdiv	w6, w6, w8
	ble	.L24
	add	w17, w17, w18
.L25:
	neg	w7, w1, lsl 4
	mul	w30, w20, w30
	madd	w8, w12, w16, w7
	madd	w7, w8, w4, w30
	cmp	w7, wzr
	add	w8, w7, w18
	sub	w7, w7, w18
	csel	w7, w7, w8, le
	cmp	w14, 3
	bls	.L45
	ldp	w8, w7, [x19, 16]
	ldp	w5, w9, [x19, 24]
	ldp	w13, w10, [x19, 32]
	ldp	w1, w3, [x19, 40]
	mov	w4, 0
	mov	w0, 0
.L34:
	lsl	w2, w11, w0
	cmp	w2, w8
	beq	.L46
.L35:
	mov	w2, 16
	sub	w0, w2, w0
	sub	w2, w2, w4
	lsl	w13, w13, w0
	lsl	w10, w10, w0
	lsl	w8, w8, w0
	lsl	w7, w7, w0
	lsl	w5, w5, w0
	lsl	w1, w1, w2
	lsl	w3, w3, w2
	lsl	w0, w9, w0
	stp	w8, w7, [x19, 16]
	stp	w5, w0, [x19, 24]
	stp	w13, w10, [x19, 32]
	stp	w1, w3, [x19, 40]
.L13:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L45:
	ldr	x8, .LC4
	ldrb	w8, [x8,w14,uxtw]
	adr	x14, .Lrtx30
	add	x8, x14, w8, sxtb #2
	br	x8
.Lrtx30:
	.section	.rodata
	.align	0
	.align	2
.L30:
	.byte	(.L40 - .Lrtx30) / 4
	.byte	(.L31 - .Lrtx30) / 4
	.byte	(.L32 - .Lrtx30) / 4
	.byte	(.L33 - .Lrtx30) / 4
	.text
	.p2align 3
.L40:
	mov	w3, 0
	mov	w1, 0
	mov	w10, 0
	mov	w13, 0
.L29:
	mov	w0, 0
	mov	w4, 0
.L37:
	asr	w13, w13, w0
	asr	w10, w10, w0
	asr	w1, w1, w4
	asr	w3, w3, w4
	mov	w0, 1
	str	w0, [x19]
	stp	w11, wzr, [x19, 16]
	stp	wzr, w11, [x19, 24]
	stp	w13, w10, [x19, 32]
	stp	w1, w3, [x19, 40]
	b	.L13
	.p2align 3
.L33:
	sdiv	w7, w7, w1
	cmp	w2, w0
	csel	w8, w2, w0, le
	add	w0, w2, w0
	add	w0, w0, w9
	asr	w4, w4, w8
	sub	w0, w0, w8
	asr	w9, w3, w8
	add	w7, w7, w15
	sdiv	w17, w17, w1
	neg	w1, w12
	mul	w8, w1, w13
	mul	w2, w1, w10
	lsl	w1, w3, 1
	add	w5, w5, w8
	add	w17, w17, w8
	add	w6, w6, w2
	mul	w1, w1, w4
	add	w2, w7, w2
	neg	w3, w3, lsl 4
	mul	w8, w5, w4
	mul	w7, w17, w9
	mul	w5, w6, w4
	mul	w9, w2, w9
	mul	w12, w1, w12
	add	w2, w5, w9
	add	w1, w8, w7
	mul	w3, w3, w4
	madd	w1, w12, w13, w1
	sub	w4, w0, #1
	lsl	w13, w13, w0
	madd	w12, w12, w10, w2
	mov	w2, 1
	lsl	w10, w10, w0
	lsl	w4, w2, w4
	add	w1, w1, w3
	add	w13, w13, w4
	add	w10, w10, w4
	add	w4, w0, w2
	add	w3, w12, w3
	lsl	w2, w2, w4
	add	w4, w0, 2
	add	w1, w1, w2
	add	w3, w3, w2
	stp	w8, w7, [x19, 16]
	stp	w5, w9, [x19, 24]
	stp	w13, w10, [x19, 32]
	stp	w1, w3, [x19, 40]
	b	.L34
	.p2align 3
.L32:
	neg	w1, w12
	sub	w7, w20, w6
	add	w0, w9, w2
	mov	w4, 1
	madd	w8, w1, w13, w5
	madd	w5, w1, w10, w6
	lsl	w1, w3, 1
	neg	w3, w3, lsl 4
	mov	w9, w8
	mul	w12, w1, w12
	add	w1, w7, w8
	stp	w8, w7, [x19, 16]
	madd	w1, w12, w13, w1
	lsl	w13, w13, w0
	add	w2, w1, w3
	add	w1, w8, w5
	stp	w5, w8, [x19, 24]
	madd	w12, w12, w10, w1
	sub	w1, w0, #1
	lsl	w10, w10, w0
	add	w12, w12, w3
	add	w3, w0, w4
	lsl	w1, w4, w1
	lsl	w3, w4, w3
	add	w13, w13, w1
	add	w10, w10, w1
	add	w4, w0, 2
	add	w1, w2, w3
	stp	w13, w10, [x19, 32]
	add	w3, w12, w3
	stp	w1, w3, [x19, 40]
	b	.L34
	.p2align 3
.L31:
	and	w1, w13, 1
	and	w3, w10, 1
	orr	w1, w1, w13, asr 1
	orr	w3, w3, w10, asr 1
	b	.L29
	.p2align 3
.L44:
	madd	w5, w11, w8, w13
	add	w6, w5, w6
	madd	w5, w11, w1, w10
	add	w16, w5, w16
	b	.L19
	.p2align 3
.L20:
	sub	w5, w5, w22
	b	.L21
	.p2align 3
.L24:
	sub	w17, w17, w18
	b	.L25
.L39:
	mov	w15, 16
	mov	w4, 1
	mov	w0, 0
	b	.L16
.L38:
	mov	w30, w5
	mov	w3, 1
	mov	w2, 0
	b	.L14
.L46:
	cbnz	w7, .L35
	cbnz	w5, .L35
	cmp	w8, w9
	mov	w7, w5
	bne	.L35
	b	.L37
	.size	MP4_GetGMCparam, .-MP4_GetGMCparam
	.align	3
.LC4:
	.xword	.L30
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_GetUserDataBuf
	.type	Mp4_GetUserDataBuf, %function
Mp4_GetUserDataBuf:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x1
	mov	w19, w2
	mov	w21, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x20, w19, uxtw 3]
	cbz	x0, .L52
.L48:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L52:
	mov	w0, w21
	bl	GetUsd
	str	x0, [x20, w19, uxtw 3]
	cbz	x0, .L48
	add	w1, w19, 4
	str	w1, [x0, 1040]
	ldr	x0, [x20, w19, uxtw 3]
	b	.L48
	.size	Mp4_GetUserDataBuf, .-Mp4_GetUserDataBuf
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_ClearUsrData
	.type	Mp4_ClearUsrData, %function
Mp4_ClearUsrData:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	x19, 0
	mov	x20, x1
	bl	_mcount
.L54:
	ldr	x1, [x20, x19]
	mov	w0, w21
	bl	FreeUsdByDec
	str	xzr, [x20, x19]
	add	x19, x19, 8
	cmp	x19, 32
	bne	.L54
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Mp4_ClearUsrData, .-Mp4_ClearUsrData
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Init
	.type	Mp4_Syntax_Init, %function
Mp4_Syntax_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 16384
	add	x20, x20, 20480
	bl	_mcount
	ldr	x21, .LC7
	mov	x2, 24
	mov	w1, 0
	add	x0, x19, 3632
	ldr	x3, [x21, 96]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 604
	mov	w1, 0
	add	x0, x19, 3656
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 60
	mov	w1, 0
	add	x0, x20, 768
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 768
	mov	w1, 0
	add	x0, x20, 828
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 7
	mov	w1, 0
	add	x0, x20, 1596
	blr	x3
	ldr	x3, .LC8
	mov	w1, 1
	mov	w0, 1
	strb	w1, [x20, 845]
	str	w0, [x20, 2208]
	add	x1, x19, 3744
	add	x4, x3, 256
	mov	w2, 8
	add	x0, x3, 4
	b	.L59
	.p2align 3
.L57:
	ldr	w2, [x0], 4
.L59:
	str	w2, [x1], 4
	cmp	x0, x4
	bne	.L57
	add	x0, x3, 260
	add	x1, x19, 4000
	add	x3, x3, 512
	mov	w2, 16
	b	.L58
	.p2align 3
.L61:
	ldr	w2, [x0], 4
.L58:
	str	w2, [x1], 4
	cmp	x0, x3
	bne	.L61
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Mp4_Syntax_Init, .-Mp4_Syntax_Init
	.align	3
.LC7:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC8:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Global_Data_Init
	.type	Global_Data_Init, %function
Global_Data_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 20480
	bl	_mcount
	ldr	x21, .LC11
	mov	x2, 32
	mov	w1, 0
	add	x0, x19, 1640
	ldr	x3, [x21, 96]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x19, 1608
	blr	x3
	mov	x0, x20
	bl	Mp4_Syntax_Init
	str	xzr, [x20, 22648]
	str	xzr, [x20, 22656]
	add	x2, x19, 2048
	str	xzr, [x20, 22664]
	mov	w0, 0
	str	xzr, [x20, 22672]
	str	wzr, [x19, 1676]
	str	wzr, [x19, 2120]
	str	wzr, [x19, 2124]
	str	wzr, [x19, 2128]
	str	wzr, [x19, 2136]
	str	xzr, [x20, 22680]
	stp	wzr, wzr, [x2, 92]
	stp	wzr, wzr, [x2, 100]
	str	wzr, [x19, 2156]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Global_Data_Init, .-Global_Data_Init
	.align	3
.LC11:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Bs_Get_Matrix
	.type	Bs_Get_Matrix, %function
Bs_Get_Matrix:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x25, x0
	mov	x0, x30
	mov	x21, x1
	mov	w19, 0
	bl	_mcount
	mov	w22, 0
	ldr	x24, .LC12
	mov	w20, 0
	mov	w23, 63
	b	.L65
	.p2align 3
.L72:
	ldr	w19, [x3, w1, sxtw 2]
	mov	w20, w1
	mov	w22, w0
.L65:
	mov	w1, 8
	mov	x0, x25
	bl	BsGet
	cmp	w0, wzr
	add	w2, w20, 1
	str	w0, [x21, w19, sxtw 2]
	add	x3, x24, 512
	sxtw	x19, w19
	sxth	w1, w2
	ccmp	w1, w23, 0, ne
	ble	.L72
	cbnz	w0, .L63
	cmp	w20, 63
	bgt	.L63
	ldr	x1, .LC12
	b	.L68
	.p2align 3
.L73:
	ldrsw	x19, [x0, w20, sxtw 2]
.L68:
	sxth	w20, w2
	str	w22, [x21, x19, lsl 2]
	cmp	w20, 64
	add	x0, x1, 512
	add	w2, w20, 1
	bne	.L73
.L63:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	Bs_Get_Matrix, .-Bs_Get_Matrix
	.align	3
.LC12:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	NextStartCode
	.type	NextStartCode, %function
NextStartCode:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w22, w2
	mov	x23, x1
	mov	x21, x3
	bl	_mcount
	mov	w2, w22
	mov	x1, x23
	add	x0, x29, 72
	lsl	w19, w22, 3
	bl	BsInit
	mov	w2, 0
	b	.L75
	.p2align 3
.L79:
	cmp	w5, 256
	beq	.L88
.L83:
	bl	BsSkip
	add	x0, x29, 72
	bl	BsPos
	mov	w2, w0
.L75:
	sub	w2, w19, w2
	mov	w1, 32
	cmp	w2, 31
	add	x0, x29, 72
	ble	.L89
	bl	BsShow
	mov	w2, w0
	cmp	w2, 438
	mov	w1, 8
	add	x0, x29, 72
	and	w3, w2, -16
	and	w4, w2, -1024
	and	w5, w2, -256
	beq	.L76
	cmp	w3, 288
	beq	.L76
	cmp	w4, 32768
	bne	.L79
	ldrb	w2, [x20, 9]
	mov	w1, 8
	add	x0, x29, 72
	cmp	w2, 2
	bne	.L90
	ldr	w2, [x20, 744]
	add	w2, w2, 1
	str	w2, [x20, 744]
	cmp	w2, 18
	bls	.L83
	mov	w0, 196
	str	w0, [x21]
	mov	w1, 1
	add	x0, x29, 72
	strb	w1, [x20, 9]
	bl	BsPos
	b	.L78
	.p2align 3
.L76:
	mov	w0, 2
	str	wzr, [x20, 744]
	strb	w0, [x20, 9]
	add	x0, x29, 72
	str	w2, [x21]
	bl	BsPos
.L78:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L89:
	mov	w1, 197
	str	w1, [x21]
	bl	BsPos
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L88:
	str	w2, [x21]
	add	x0, x29, 72
	bl	BsPos
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 112
	ret
.L90:
	mov	w0, 1
	strb	w0, [x20, 9]
	mov	w1, 196
	add	x0, x29, 72
	str	w1, [x21]
	bl	BsPos
	b	.L78
	.size	NextStartCode, .-NextStartCode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4SearchStartCode
	.type	Mp4SearchStartCode, %function
Mp4SearchStartCode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 4
	mov	w0, 0
	ble	.L92
	add	x4, x20, x19, sxtw
	add	x0, x20, 2
.L108:
	cmp	x4, x0
	bls	.L93
.L113:
	ldrb	w2, [x0]
	cmp	w2, 1
	bls	.L94
	and	w3, w2, 252
	cmp	w3, 128
	beq	.L94
.L101:
	add	x0, x0, 3
	cmp	x4, x0
	bhi	.L113
.L93:
	sub	w0, w19, #2
.L92:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L94:
	cbnz	w2, .L96
	ldrb	w2, [x0, -1]
	cbnz	w2, .L97
	add	x0, x0, 1
	b	.L108
	.p2align 3
.L96:
	ldrb	w1, [x0, -2]
	cbnz	w1, .L101
	ldrb	w1, [x0, -1]
	cbnz	w1, .L101
	sub	x0, x0, x20
	sub	w0, w0, #2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L97:
	ldrb	w2, [x0, 1]
	add	x1, x0, 2
	add	x0, x0, 4
	cmp	w2, wzr
	csel	x0, x0, x1, ne
	b	.L108
	.size	Mp4SearchStartCode, .-Mp4SearchStartCode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Gop
	.type	Mp4_Syntax_Gop, %function
Mp4_Syntax_Gop:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	w21, w2
	bl	_mcount
	mov	w2, w21
	mov	x1, x20
	add	x0, x29, 56
	bl	BsInit
	mov	w1, 32
	add	x0, x29, 56
	bl	BsSkip
	mov	w1, 5
	add	x0, x29, 56
	bl	BsGet
	mov	w1, 6
	add	x0, x29, 56
	bl	BsGet
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	cbnz	w0, .L115
	ldr	x1, .LC15
	mov	w0, 1
	add	x19, x19, 20480
	bl	dprint_vfmw
	add	x0, x29, 56
	strb	wzr, [x19, 1598]
	bl	BsPos
	mov	w20, w0
	ldr	x0, .LC16
	ldr	x4, [x0]
	cbz	x4, .L117
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L115:
	mov	w1, 6
	add	x0, x29, 56
	add	x19, x19, 20480
	bl	BsGet
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 830]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 831]
	mov	w1, 1
	add	x0, x29, 56
	strb	w1, [x19, 1598]
	bl	BsPos
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L117:
	mov	w0, w20
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.size	Mp4_Syntax_Gop, .-Mp4_Syntax_Gop
	.align	3
.LC15:
	.xword	.LC14
	.align	3
.LC16:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_UserData
	.type	Mp4_Syntax_UserData, %function
Mp4_Syntax_UserData:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x22, x0
	mov	x0, x30
	mov	w20, w2
	mov	x19, x1
	bl	_mcount
	strb	wzr, [x29, 95]
	cmp	w20, 6
	stp	wzr, wzr, [x29, 96]
	stp	wzr, wzr, [x29, 104]
	ble	.L119
	cmp	w20, 4
	ble	.L120
	ldrb	w0, [x19, 4]
	add	x1, x19, 5
	strb	w0, [x22, 3]
	add	x5, x22, 4
	orr	w0, w0, -256
	mov	w19, 4
	lsl	w0, w0, 8
	b	.L121
	.p2align 3
.L126:
	ldrb	w4, [x1]
	add	x1, x1, 1
	orr	w0, w4, w0
	lsl	w0, w0, 8
	cmp	w0, 256
	beq	.L177
	strb	w4, [x5], 1
	cmp	w3, 1030
	beq	.L178
	mov	w19, w3
.L121:
	add	w3, w19, 1
	sub	w21, w19, #3
	cmp	w20, w3
	bne	.L126
.L124:
	add	x23, x22, 20480
	mov	w1, 1
	cmp	w21, 2
	lsl	w25, w20, 3
	mov	w0, w25
	strb	w1, [x23, 1602]
	bhi	.L179
.L169:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L119:
	add	x22, x22, 20480
	add	w19, w20, 1
	lsl	w0, w19, 3
	strb	wzr, [x22, 1602]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L177:
	sub	w0, w19, #1
	cmp	w20, w0
	bge	.L180
.L125:
	add	w0, w20, 1
	ldp	x21, x22, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	lsl	w0, w0, 3
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L179:
	ldr	w2, [x23, 2156]
	add	x1, x23, 2168
	ldr	w0, [x23, 2824]
	bl	Mp4_GetUserDataBuf
	str	x0, [x22, 22680]
	cbz	x0, .L181
	sub	w26, w19, #5
	str	w26, [x0, 1052]
	mov	w0, 65533
	ldr	x24, .LC17
	movk	w0, 0x7fff, lsl 16
	add	x20, x22, 3
	cmn	w19, w0
	mov	x2, 0
	ldr	x3, [x24, 104]
	ldr	x0, [x22, 22680]
	bmi	.L182
.L129:
	mov	x1, x20
	blr	x3
	ldr	x1, [x22, 22112]
	ldr	x2, .LC18
	ldr	x0, [x22, 22680]
	ldr	x1, [x1, 56]
	ldr	x4, [x2]
	str	x1, [x0, 1056]
	cbz	x4, .L128
	ldr	w0, [x23, 2824]
	mov	w3, 1064
	ldr	x2, [x22, 22680]
	mov	w1, 6
	blr	x4
.L128:
	mov	w1, 2
	mov	w0, 0
	movk	w1, 0x8000, lsl 16
	cmn	w26, w1
	bmi	.L183
.L130:
	add	x0, x22, x0, sxtw
	ldr	x1, .LC20
	add	x4, x29, 95
	add	x3, x29, 100
	add	x2, x29, 96
	strb	wzr, [x0, 3]
	mov	x0, x20
	ldr	x5, [x24, 144]
	blr	x5
	cmp	w0, 1
	ble	.L184
.L131:
	ldp	w2, w1, [x29, 96]
	cmp	w0, 3
	str	w2, [x23, 1576]
	str	w1, [x23, 1580]
	mov	w0, 0
	beq	.L185
.L133:
	str	w0, [x23, 1584]
.L132:
	ldr	x1, .LC24
	add	x2, x29, 100
	ldr	x3, [x24, 144]
	mov	x0, x20
	blr	x3
	cmp	w0, 1
	beq	.L134
	ldr	x1, .LC26
	add	x5, x29, 100
	ldr	x6, [x24, 144]
	add	x4, x29, 108
	add	x3, x29, 104
	add	x2, x29, 96
	mov	x0, x20
	blr	x6
	cmp	w0, 4
	beq	.L134
	ldr	x1, .LC28
	add	x4, x29, 108
	ldr	x5, [x24, 144]
	add	x3, x29, 104
	add	x2, x29, 96
	mov	x0, x20
	blr	x5
	add	w0, w0, 1
	cmp	w0, 1
	ble	.L135
	ldp	w3, w1, [x29, 104]
	ldr	w2, [x29, 96]
	lsl	w3, w3, 8
	add	w2, w3, w2, lsl 16
	add	w1, w2, w1
	str	w1, [x29, 100]
.L135:
	cmp	w0, 4
	beq	.L134
	ldr	x1, .LC30
	mov	w2, 7
	ldr	x3, [x24, 152]
	mov	x0, x20
	blr	x3
	cbz	w0, .L186
.L146:
	ldr	x1, .LC32
	add	x2, x29, 100
	ldr	x3, [x24, 144]
	mov	x0, x20
	blr	x3
	cmp	w0, 1
	beq	.L137
	ldr	w0, [x23, 1588]
.L138:
	strb	wzr, [x23, 841]
	strb	wzr, [x23, 842]
	strb	wzr, [x23, 839]
	strb	wzr, [x23, 838]
	strb	wzr, [x23, 840]
	ldr	w1, [x23, 1576]
	cbz	w0, .L139
	cbz	w1, .L140
	str	wzr, [x23, 1576]
	str	wzr, [x23, 1580]
.L140:
	cmp	w0, 1
	beq	.L147
	mov	w1, -1
.L141:
	sub	w0, w0, #1
	cmp	w0, 11
	bls	.L151
.L142:
	ldr	w0, [x23, 1592]
	mov	w2, 4668
	sub	w0, w0, #1
	cmp	w0, w2
	bhi	.L143
	mov	w0, 1
	strb	w0, [x23, 839]
.L143:
	cmp	w1, 498
	bhi	.L144
	mov	w0, 1
	strb	w0, [x23, 839]
.L144:
	mov	x1, 500
	ldr	x2, [x22, 22056]
	movk	x1, 0x19d, lsl 32
	mov	w0, w25
	cmp	x2, x1
	bne	.L169
	mov	w1, 1
	strb	w1, [x23, 840]
	b	.L169
.L178:
	add	x22, x22, 20480
	sub	w19, w19, #1
	lsl	w0, w19, 3
	strb	wzr, [x22, 1602]
	b	.L169
.L139:
	cmp	w1, 499
	bls	.L187
	mov	w2, 1
	cmp	w1, 502
	strb	w2, [x23, 841]
	sub	w1, w1, #1
	bls	.L141
.L150:
	mov	w2, 1
	strb	w2, [x23, 842]
	b	.L141
.L147:
	strb	w0, [x23, 841]
	mov	w1, -1
.L151:
	mov	w0, 1
	strb	w0, [x23, 839]
	b	.L142
.L134:
	ldr	w0, [x29, 100]
	str	w0, [x23, 1592]
	b	.L146
.L186:
	mov	w0, 4600
	str	w0, [x23, 1592]
	b	.L146
.L185:
	ldrsb	w0, [x29, 95]
	cmp	w0, 112
	cset	w0, eq
	b	.L133
.L137:
	ldr	w0, [x29, 100]
	str	w0, [x23, 1588]
	b	.L138
.L183:
	add	w0, w26, 2
	mov	w1, 1024
	cmp	w0, w1
	csel	w0, w0, w1, le
	b	.L130
.L182:
	mov	w1, 1024
	cmp	w21, w1
	csel	w21, w21, w1, le
	sxtw	x2, w21
	b	.L129
.L120:
	cmp	w20, 3
	ble	.L125
	add	x22, x22, 20480
	mov	w1, 1
	mov	w0, 32
	strb	w1, [x22, 1602]
	b	.L169
.L181:
	add	x20, x22, 3
	mov	w26, w21
	ldr	x24, .LC17
	b	.L128
.L184:
	ldr	x1, .LC22
	add	x4, x29, 95
	ldr	x5, [x24, 144]
	add	x3, x29, 100
	add	x2, x29, 96
	mov	x0, x20
	blr	x5
	cmp	w0, 1
	ble	.L132
	b	.L131
	.p2align 3
.L180:
	mov	w20, w0
	b	.L124
.L187:
	cmp	w1, 502
	sub	w1, w1, #1
	bhi	.L150
	b	.L142
	.size	Mp4_Syntax_UserData, .-Mp4_Syntax_UserData
	.align	3
.LC17:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC18:
	.xword	g_event_report
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
	.align	3
.LC32:
	.xword	.LC31
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Vo
	.type	Mp4_Syntax_Vo, %function
Mp4_Syntax_Vo:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	mov	w22, w2
	bl	_mcount
	add	x19, x20, 20480
	mov	w2, w22
	mov	x1, x21
	add	x0, x29, 56
	mov	w21, 1
	add	x20, x20, 16384
	bl	BsInit
	strb	wzr, [x19, 844]
	mov	w1, 32
	strb	w21, [x19, 845]
	strb	wzr, [x19, 846]
	add	x0, x29, 56
	strb	wzr, [x19, 847]
	strb	wzr, [x19, 848]
	strb	wzr, [x19, 849]
	strb	wzr, [x19, 850]
	strb	wzr, [x19, 851]
	str	wzr, [x19, 856]
	str	wzr, [x19, 860]
	str	wzr, [x19, 864]
	bl	BsSkip
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 3632]
	cbnz	w0, .L203
	strb	w21, [x20, 3633]
	mov	w1, 4
	add	x0, x29, 56
	bl	BsGet
	uxtb	w21, w0
	cmp	w21, 1
	strb	w21, [x20, 3635]
	beq	.L204
.L191:
	strb	wzr, [x19, 1597]
	add	x0, x29, 56
	bl	BsPos
	mov	w20, w0
	ldr	x0, .LC33
	ldr	x4, [x0]
	cbz	x4, .L196
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L203:
	mov	w1, 4
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x20, 3633]
	mov	w1, 3
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x20, 3634]
	mov	w1, 4
	add	x0, x29, 56
	bl	BsGet
	uxtb	w21, w0
	cmp	w21, 1
	strb	w21, [x20, 3635]
	bne	.L191
.L204:
	mov	w1, w21
	add	x0, x29, 56
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 3636]
	cbnz	w0, .L205
.L193:
	mov	w1, 1
	add	x0, x29, 56
	strb	w1, [x19, 1597]
	bl	BsPos
	cmp	w0, w22, lsl 3
	bgt	.L195
	ldrb	w0, [x20, 3632]
	strb	w0, [x19, 844]
	ldrb	w0, [x20, 3633]
	strb	w0, [x19, 845]
	ldrb	w0, [x20, 3634]
	strb	w0, [x19, 846]
	ldrb	w0, [x20, 3635]
	strb	w0, [x19, 847]
	ldrb	w0, [x20, 3636]
	strb	w0, [x19, 848]
	ldrb	w0, [x20, 3637]
	strb	w0, [x19, 849]
	ldrb	w0, [x20, 3638]
	strb	w0, [x19, 850]
	ldrb	w0, [x20, 3639]
	strb	w0, [x19, 851]
	ldr	w0, [x20, 3644]
	str	w0, [x19, 856]
	ldr	w0, [x20, 3648]
	str	w0, [x19, 860]
	ldr	w0, [x20, 3652]
	str	w0, [x19, 864]
.L195:
	add	x0, x29, 56
	bl	BsPos
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L196:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L205:
	mov	w1, 3
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x20, 3637]
	mov	w1, w21
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x20, 3638]
	mov	w1, w21
	add	x0, x29, 56
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 3639]
	cbz	w0, .L193
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	str	w0, [x20, 3644]
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	str	w0, [x20, 3648]
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	str	w0, [x20, 3652]
	b	.L193
	.size	Mp4_Syntax_Vo, .-Mp4_Syntax_Vo
	.align	3
.LC33:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Vol
	.type	Mp4_Syntax_Vol, %function
Mp4_Syntax_Vol:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 20480
	mov	x21, x1
	mov	w22, w2
	bl	_mcount
	ldr	x0, .LC34
	ldrsw	x1, [x20, 2824]
	ldr	x0, [x0, x1, lsl 3]
	cbz	x0, .L267
	ldr	w23, [x0, 1348]
	ldr	w24, [x0, 1352]
.L207:
	mov	w2, w22
	mov	x1, x21
	add	x0, x29, 88
	add	x19, x19, 16384
	add	x21, x19, 3656
	bl	BsInit
	ldr	x22, .LC35
	mov	x2, 604
	mov	w1, 0
	strb	wzr, [x20, 829]
	mov	x0, x21
	ldr	x3, [x22, 96]
	blr	x3
	mov	w1, 32
	add	x0, x29, 88
	bl	BsSkip
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3660]
	mov	w1, 8
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3684]
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3656]
	cbnz	w0, .L354
	ldrb	w0, [x20, 845]
	mov	w1, 4
	strb	w0, [x19, 3657]
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 15
	strb	w0, [x19, 3659]
	beq	.L355
.L210:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3661]
	cbnz	w0, .L356
	mov	w0, 1
	strb	w0, [x19, 3662]
.L215:
	mov	w1, 2
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3664]
	cbz	w0, .L357
.L348:
	strb	wzr, [x20, 1599]
	add	x0, x29, 88
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC38
	ldr	x4, [x0]
	cbz	x4, .L286
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, w19
.L322:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L357:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L349
	mov	w1, 16
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3728]
	cbz	w0, .L223
	subs	w0, w0, #1
	beq	.L275
	mov	w2, w0
	mov	w1, 0
	.p2align 2
.L225:
	add	w1, w1, 1
	lsr	w2, w2, 1
	sxth	w1, w1
	cbnz	w2, .L225
	cmp	w1, wzr
	mov	w1, 0
	ble	.L275
	.p2align 2
.L307:
	add	w1, w1, 1
	lsr	w0, w0, 1
	sxth	w1, w1
	cbnz	w0, .L307
.L224:
	str	w1, [x19, 3724]
.L228:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L349
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3665]
	cbnz	w0, .L358
.L230:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L349
	mov	w1, 13
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3736]
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L349
	mov	w1, 13
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3740]
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L349
	ldr	w0, [x19, 3736]
	cmp	w0, 31
	bls	.L234
	ldr	w1, [x19, 3740]
	cmp	w1, 31
	bls	.L234
	mul	w0, w0, w1
	mul	w23, w23, w24
	cmp	w0, w23
	bhi	.L234
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3666]
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3667]
	cbnz	w0, .L237
	ldr	x0, .LC38
	strb	wzr, [x20, 1599]
	ldr	x4, [x0]
	cbz	x4, .L237
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L237:
	ldrb	w2, [x19, 3657]
	mov	w1, 2
	add	x0, x29, 88
	cmp	w2, 1
	csinc	w1, w1, wzr, ne
	bl	BsGet
	uxtb	w23, w0
	strb	w23, [x19, 3668]
	cbz	w23, .L240
	ldr	x0, .LC39
	ldr	w0, [x0]
	tbz	x0, 3, .L359
	sub	w0, w23, #1
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L360
	strb	wzr, [x20, 1599]
	add	x0, x29, 88
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC38
	ldr	x4, [x0]
	cbz	x4, .L247
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L247:
	ldr	x1, .LC45
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L322
	.p2align 3
.L349:
	ldr	x1, .LC37
	mov	w0, 1
	bl	dprint_vfmw
.L234:
	strb	wzr, [x20, 1599]
	add	x0, x29, 88
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC38
	ldr	x4, [x0]
	cbz	x4, .L286
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
	ldr	x25, [sp, 64]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L356:
	mov	w1, 2
	add	x0, x29, 88
	bl	BsGet
	uxtb	w25, w0
	cmp	w25, 1
	strb	w25, [x19, 3662]
	beq	.L212
	strb	wzr, [x20, 1599]
	add	x0, x29, 88
	bl	BsPos
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L354:
	mov	w1, 4
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3657]
	mov	w1, 3
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3658]
	mov	w1, 4
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 15
	strb	w0, [x19, 3659]
	bne	.L210
.L355:
	mov	w1, 8
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3688]
	mov	w1, 8
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3692]
	b	.L210
	.p2align 3
.L286:
	mov	w0, w19
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L212:
	mov	w1, w25
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3663]
	mov	w1, w25
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3696]
	cbz	w0, .L215
	mov	w1, 15
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3700]
	mov	w1, w25
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L350
	mov	w1, 15
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3704]
	mov	w1, w25
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L350
	mov	w1, 15
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3708]
	mov	w1, w25
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L350
	mov	w1, 3
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3712]
	mov	w1, 11
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3716]
	mov	w1, w25
	add	x0, x29, 88
	bl	BsGet
	cbz	w0, .L350
	mov	w1, 15
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3720]
	mov	w1, w25
	add	x0, x29, 88
	bl	BsGet
	cbnz	w0, .L215
	.p2align 2
.L350:
	ldr	x1, .LC37
	mov	w0, w25
	bl	dprint_vfmw
	b	.L234
	.p2align 3
.L267:
	mov	w24, 8192
	mov	w23, w24
	b	.L207
	.p2align 3
.L223:
	mov	w0, 1
	str	w0, [x19, 3724]
	b	.L228
	.p2align 3
.L275:
	mov	w1, 1
	b	.L224
	.p2align 3
.L358:
	ldr	w1, [x19, 3724]
	add	x0, x29, 88
	bl	BsGet
	str	w0, [x19, 3732]
	b	.L230
.L361:
	mov	w1, 6
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3680]
	mov	w1, w23
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3681]
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3682]
	ldrb	w1, [x19, 3668]
	cmp	w1, 2
	bne	.L352
.L240:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3669]
	cbz	w0, .L248
	strb	wzr, [x20, 1599]
	add	x0, x29, 88
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC38
	ldr	x4, [x0]
	cbz	x4, .L249
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L249:
	ldr	x1, .LC47
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L322
.L360:
	cmp	w23, 2
	beq	.L361
.L352:
	strb	wzr, [x20, 1599]
	add	x0, x29, 88
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC38
	ldr	x4, [x0]
	cbz	x4, .L246
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L246:
	ldr	x1, .LC43
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L322
.L248:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3670]
	cbnz	w0, .L362
.L251:
	ldrb	w0, [x19, 3657]
	cmp	w0, 1
	beq	.L258
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3673]
.L259:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3674]
	ldr	x1, .LC50
	cbz	w0, .L343
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 3675]
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3676]
	ldr	x1, .LC52
	cbnz	w0, .L343
	ldrb	w0, [x19, 3657]
	cmp	w0, 1
	beq	.L262
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3677]
	ldr	x1, .LC54
	cbnz	w0, .L343
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3678]
	ldr	x1, .LC56
	cbnz	w0, .L343
.L264:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3679]
	cbz	w0, .L265
	ldr	x1, .LC58
.L343:
	mov	w0, 1
	bl	dprint_vfmw
	b	.L348
.L359:
	strb	wzr, [x20, 1599]
	add	x0, x29, 88
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC38
	ldr	x4, [x0]
	cbz	x4, .L242
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L242:
	ldr	x1, .LC41
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L322
.L258:
	strb	wzr, [x19, 3673]
	b	.L259
.L362:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3671]
	cbnz	w0, .L363
	ldr	x0, .LC48
	add	x1, x19, 3744
	add	x3, x0, 256
.L254:
	ldr	w2, [x0], 4
	str	w2, [x1], 4
	cmp	x0, x3
	bne	.L254
.L253:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3672]
	cbnz	w0, .L364
	ldr	x0, .LC48
	add	x1, x19, 4000
	add	x0, x0, 256
	add	x3, x0, 256
.L256:
	ldr	w2, [x0], 4
	str	w2, [x1], 4
	cmp	x0, x3
	bne	.L256
	b	.L251
.L363:
	add	x1, x19, 3744
	add	x0, x29, 88
	bl	Bs_Get_Matrix
	b	.L253
.L364:
	add	x1, x19, 4000
	add	x0, x29, 88
	bl	Bs_Get_Matrix
	b	.L251
.L265:
	mov	w0, 1
	ldr	x3, [x22, 104]
	strb	w0, [x20, 1599]
	mov	x2, 604
	strb	w0, [x20, 829]
	mov	x1, x21
	strb	w0, [x20, 1600]
	add	x0, x20, 164
	blr	x3
	add	x0, x29, 88
	bl	BsPos
	b	.L322
.L262:
	strb	wzr, [x19, 3677]
	strb	wzr, [x19, 3678]
	b	.L264
	.size	Mp4_Syntax_Vol, .-Mp4_Syntax_Vol
	.align	3
.LC34:
	.xword	s_pstVfmwChan
	.align	3
.LC35:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC37:
	.xword	.LC36
	.align	3
.LC38:
	.xword	g_event_report
	.align	3
.LC39:
	.xword	g_VdmCharacter
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
.LC48:
	.xword	.LANCHOR1
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Vos
	.type	Mp4_Syntax_Vos, %function
Mp4_Syntax_Vos:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	w21, w2
	bl	_mcount
	mov	w2, w21
	mov	x1, x20
	add	x0, x29, 56
	add	x20, x19, 20480
	add	x19, x19, 16384
	bl	BsInit
	mov	w1, 32
	add	x0, x29, 56
	str	wzr, [x20, 852]
	bl	BsSkip
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	sub	w2, w0, #1
	sub	w1, w0, #240
	cmp	w2, 2
	ccmp	w1, 5, 0, hi
	str	w0, [x19, 3640]
	ccmp	w0, 8, 4, hi
	beq	.L372
	cbz	w0, .L373
	ldr	x0, .LC63
	strb	wzr, [x20, 1596]
	ldr	x4, [x0]
	cbz	x4, .L367
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	add	x0, x29, 56
	bl	BsPos
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L372:
	mov	w1, 1
	str	w0, [x20, 852]
	strb	w1, [x20, 1596]
.L367:
	add	x0, x29, 56
	bl	BsPos
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L373:
	ldr	x1, .LC62
	mov	w2, 0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 1
	strb	w0, [x20, 1596]
	ldr	w0, [x19, 3640]
	str	w0, [x20, 852]
	b	.L367
	.size	Mp4_Syntax_Vos, .-Mp4_Syntax_Vos
	.align	3
.LC62:
	.xword	.LC61
	.align	3
.LC63:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Frame_Init
	.type	Frame_Init, %function
Frame_Init:
	sub	sp, sp, #144
	stp	x29, x30, [sp, 16]
	add	x29, sp, 16
	stp	x19, x20, [sp, 32]
	stp	x21, x22, [sp, 48]
	stp	x23, x24, [sp, 64]
	stp	x25, x26, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	mov	w22, w1
	mov	w23, w2
	bl	_mcount
	ldr	x0, [x19, 22112]
	ldr	w21, [x0, 708]
	ldr	w1, [x0, 28]
	cmp	w1, 25
	cbnz	w21, .L375
	beq	.L409
.L401:
	mov	w24, 2
	mov	w25, 5
	mov	w26, 3
.L376:
	ldr	w1, [x0, 832]
	add	x20, x19, 20480
	cmp	w1, 1
	beq	.L387
	ldr	x1, .LC66
	ldr	x8, [x1, 128]
	cbz	x8, .L389
	add	x20, x19, 20480
	ldr	w1, [x0, 24]
	ldr	x21, .LC69
	mov	w6, 0
	ldr	w0, [x0, 20]
	mov	w5, 40
	ldr	w7, [x20, 2824]
	mov	w4, 2
	mov	w3, w23
	mov	w2, w22
	str	x21, [sp]
	blr	x8
	cbnz	w0, .L389
	ldr	w2, [x21, 1248]
	ldr	w3, [x21, 1080]
	cmp	w2, 1
	ble	.L391
	cmp	w3, 2
	ble	.L391
	ldr	w0, [x21, 8]
	str	w0, [x20, 1664]
	bl	MEM_Phy2Vir
	str	x0, [x19, 22128]
.L387:
	ldr	x0, .LC72
	mov	x2, 32
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x29, 96
	blr	x3
	strb	wzr, [x29, 97]
	ldr	x0, [x19, 22112]
	stp	w22, w23, [x29, 120]
	stp	w22, w23, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 24
	beq	.L393
	strb	w26, [x29, 99]
	sxtb	w24, w24
	strb	w25, [x29, 101]
.L398:
	ldr	w0, [x20, 2824]
	add	x1, x29, 96
	strb	w24, [x29, 102]
	strb	wzr, [x29, 98]
	bl	FSP_ConfigInstance
	mov	w21, w0
	cbnz	w0, .L394
	ldr	x1, [x19, 22112]
	add	x3, x29, 92
	ldr	w0, [x20, 2824]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L410
	cbnz	w0, .L396
	mov	w0, 1
	str	w0, [x20, 2828]
.L404:
	add	sp, x29, 0
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L375:
	bne	.L401
	ldr	w4, [x0, 668]
	ldr	w5, [x0, 672]
	cmp	w22, w4
	bhi	.L383
	cmp	w23, w5
	bhi	.L383
	ldr	w2, [x0, 696]
	mov	w24, 1
	ldr	w1, [x0, 692]
	add	w26, w1, 2
	cbnz	w2, .L411
	ldr	w25, [x0, 716]
	b	.L376
	.p2align 3
.L393:
	mov	w0, 1
	strb	w25, [x29, 101]
	strb	w0, [x29, 99]
	mov	w24, 1
	b	.L398
	.p2align 3
.L409:
	ldr	w4, [x0, 668]
	cmp	w4, w22
	bcc	.L412
	ldr	w1, [x0, 672]
	cmp	w1, w23
	mov	w5, w1
	bcc	.L383
	add	x20, x19, 20480
	ldr	w2, [x20, 2828]
	cmp	w2, 1
	beq	.L404
	ldr	w3, [x0, 696]
	mov	w24, 1
	ldr	w2, [x0, 692]
	add	w26, w2, 2
	cbz	w3, .L381
	add	w2, w2, w24
	mov	w24, 17
	cmp	w2, w24
	csel	w24, w2, w24, le
.L381:
	mov	w23, w1
	mov	w22, w4
	ldr	w25, [x0, 716]
	b	.L376
	.p2align 3
.L411:
	add	w1, w1, w24
	mov	w24, 17
	cmp	w1, w24
	ldr	w25, [x0, 716]
	csel	w24, w1, w24, le
	b	.L376
	.p2align 3
.L410:
	add	x0, x20, 2216
	bl	ResetVoQueue
.L396:
	ldr	x1, .LC74
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	str	wzr, [x20, 2828]
	b	.L404
	.p2align 3
.L391:
	ldr	x1, .LC71
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L404
	.p2align 3
.L389:
	ldr	x1, .LC68
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L404
	.p2align 3
.L394:
	ldr	x1, .LC76
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L404
.L412:
	ldr	w5, [x0, 672]
	.p2align 2
.L383:
	ldr	x1, .LC65
	mov	w3, w23
	mov	w2, w22
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L404
	.size	Frame_Init, .-Frame_Init
	.align	3
.LC65:
	.xword	.LC64
	.align	3
.LC66:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC68:
	.xword	.LC67
	.align	3
.LC69:
	.xword	.LANCHOR2
	.align	3
.LC71:
	.xword	.LC70
	.align	3
.LC72:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC74:
	.xword	.LC73
	.align	3
.LC76:
	.xword	.LC75
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vdm_Mem_Move
	.type	Vdm_Mem_Move, %function
Vdm_Mem_Move:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	w25, w2
	mov	w24, w3
	mov	w23, w4
	mov	w22, w5
	mov	w21, w6
	bl	_mcount
	add	w22, w22, 15
	ldr	x1, .LC78
	mov	w0, 2
	add	w21, w21, 15
	lsr	w22, w22, 4
	lsr	w21, w21, 4
	bl	dprint_vfmw
	str	w25, [x19, 1272]
	str	w24, [x19, 15652]
	add	x2, x19, 1048
	ldr	w0, [x20, 664]
	str	w0, [x19, 1288]
	strh	w22, [x19, 1224]
	strh	w21, [x19, 1226]
	str	w23, [x19, 1280]
	ldr	x0, [x19, 22112]
	ldr	x1, [x20, 648]
	ldr	w1, [x1, 32]
	str	w1, [x19, 15664]
	ldr	x1, [x20, 648]
	ldr	w1, [x1, 76]
	str	w1, [x19, 15668]
	ldr	x1, [x20, 648]
	ldr	w1, [x1, 60]
	str	w1, [x19, 15672]
	ldr	x1, [x20, 648]
	ldr	w1, [x1, 64]
	str	w1, [x19, 15676]
	ldr	w1, [x0, 592]
	str	w1, [x19, 15732]
	ldr	w1, [x0, 632]
	str	w1, [x19, 15736]
	str	x2, [x19, 1040]
	ldr	w1, [x0, 864]
	cmp	w1, 1
	beq	.L429
	cmp	w1, 2
	beq	.L430
.L415:
	mov	w0, 1
	strb	w0, [x19, 1214]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L429:
	cbz	x2, .L415
	add	x20, x19, 20480
	str	xzr, [x19, 1040]
	add	x21, x19, 16384
	mov	w2, 0
	ldr	w0, [x20, 2824]
	ldr	w1, [x20, 2852]
	bl	FSP_SetDisplay
	ldr	w22, [x20, 2824]
	ldr	w0, [x21, 3620]
	cmp	w0, wzr
	ble	.L419
	sub	w0, w0, #1
	add	x20, x19, 12288
	add	x0, x0, 3978
	add	x20, x20, 3620
	add	x19, x19, x0, lsl 2
	.p2align 2
.L418:
	ldr	w1, [x20], 4
	mov	w0, w22
	bl	SM_ReleaseStreamSeg
	cmp	x20, x19
	bne	.L418
.L419:
	str	wzr, [x21, 3620]
.L431:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L430:
	ldr	w0, [x0, 868]
	cmp	w0, wzr
	ccmp	x2, xzr, 4, gt
	beq	.L415
	add	x20, x19, 20480
	str	xzr, [x19, 1040]
	mov	w2, 0
	add	x21, x19, 16384
	ldr	w0, [x20, 2824]
	ldr	w1, [x20, 2852]
	bl	FSP_SetDisplay
	ldr	x1, [x19, 22112]
	ldr	w0, [x1, 868]
	sub	w0, w0, #1
	str	w0, [x1, 868]
	ldr	w0, [x21, 3620]
	ldr	w22, [x20, 2824]
	cmp	w0, wzr
	ble	.L419
	sub	w0, w0, #1
	add	x20, x19, 12288
	add	x0, x0, 3978
	add	x20, x20, 3620
	add	x19, x19, x0, lsl 2
	.p2align 2
.L420:
	ldr	w1, [x20], 4
	mov	w0, w22
	bl	SM_ReleaseStreamSeg
	cmp	x20, x19
	bne	.L420
	str	wzr, [x21, 3620]
	b	.L431
	.size	Vdm_Mem_Move, .-Vdm_Mem_Move
	.align	3
.LC78:
	.xword	.LC77
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_RecordVFMWState
	.type	Mp4_RecordVFMWState, %function
Mp4_RecordVFMWState:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	Mp4_RecordVFMWState, .-Mp4_RecordVFMWState
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_RecordFrameAtr
	.type	Mp4_RecordFrameAtr, %function
Mp4_RecordFrameAtr:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 20480
	mov	w19, w1
	bl	_mcount
	ldrb	w0, [x20, 871]
	cmp	w0, 15
	beq	.L452
	sub	w2, w0, #2
	uxtb	w2, w2
	cmp	w2, 1
	bls	.L444
	sub	w2, w0, #4
	uxtb	w2, w2
	cmp	w2, 1
	bls	.L445
	mov	w22, 5
	cmp	w0, 1
	csel	w22, wzr, w22, ne
.L435:
	ldr	w0, [x20, 2824]
	mov	w1, w19
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L433
	ldr	w2, [x20, 1536]
	mov	w1, w22
	str	w2, [x0, 112]
	ldr	w2, [x20, 1532]
	str	w2, [x0, 116]
	bl	SetAspectRatio
	ldr	x0, [x21, 22112]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L453
.L439:
	ldr	w0, [x20, 1536]
	mov	w4, 262144
	str	w0, [x19, 120]
	mov	x5, -1
	mov	w0, 29
	ldr	w1, [x20, 1532]
	str	w1, [x19, 124]
	ldr	w1, [x20, 1536]
	lsr	w1, w1, 1
	str	w1, [x19, 128]
	ldr	w1, [x20, 1532]
	lsr	w1, w1, 1
	str	w1, [x19, 132]
	ldr	w1, [x20, 2124]
	str	w1, [x19, 168]
	ldr	w1, [x20, 2120]
	str	w1, [x19, 164]
	ldrb	w3, [x20, 849]
	ldrb	w2, [x20, 878]
	ubfiz	w3, w3, 5, 3
	ldrb	w1, [x20, 1472]
	ubfiz	w2, w2, 8, 1
	orr	w3, w3, w2
	ldrb	w2, [x20, 1477]
	and	w1, w1, 3
	orr	w1, w1, w2, lsl 12
	orr	w1, w3, w1
	orr	w1, w1, 3072
	orr	w22, w1, w22, lsl 14
	and	w22, w22, -131073
	str	w22, [x19, 108]
	ldr	x1, [x21, 22112]
	ldr	w1, [x1, 592]
	cmp	w1, 1
	csel	w1, w4, wzr, eq
	orr	w22, w1, w22
	str	w22, [x19, 108]
	ldrb	w1, [x20, 1472]
	and	w1, w1, 3
	str	w1, [x19, 100]
	ldrb	w1, [x20, 1472]
	str	wzr, [x19, 96]
	and	w1, w1, 3
	str	w1, [x19, 104]
	ldr	x1, [x21, 22648]
	str	x1, [x19, 560]
	ldr	x1, [x21, 22656]
	str	x1, [x19, 568]
	ldr	x1, [x21, 22664]
	str	x1, [x19, 576]
	ldr	x1, [x21, 22672]
	str	x1, [x19, 584]
	ldr	x2, [x21, 22112]
	ldr	x1, .LC80
	ldr	x2, [x2, 56]
	str	x2, [x19, 600]
	ldr	x2, [x21, 22112]
	ldr	x2, [x2, 72]
	str	x2, [x19, 608]
	ldr	x2, [x21, 22112]
	ldr	x2, [x2, 88]
	str	x2, [x19, 616]
	ldr	x2, [x21, 22112]
	ldr	w2, [x2, 104]
	str	w2, [x19, 8]
	ldr	x2, [x21, 22112]
	ldr	w2, [x2, 108]
	str	w2, [x19, 12]
	ldr	x2, [x21, 22112]
	ldr	w2, [x2, 112]
	str	w2, [x19, 16]
	ldr	x2, [x21, 22112]
	ldr	w2, [x2, 128]
	str	w2, [x19, 20]
	ldr	x2, [x21, 22112]
	str	x5, [x2, 56]
	ldr	x2, [x19, 600]
	ldr	x3, [x19, 608]
	bl	dprint_vfmw
	strb	wzr, [x19, 364]
	mov	w1, 1
	strb	w1, [x19, 369]
	add	x1, x19, 256
	strb	wzr, [x19, 371]
	mov	w0, 8
	mov	w2, 2
	stp	w0, w0, [x1, 128]
	stp	w2, w0, [x1, 120]
.L433:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L445:
	mov	w22, 2
	b	.L435
	.p2align 3
.L444:
	mov	w22, 1
	b	.L435
	.p2align 3
.L453:
	ldr	w0, [x19, 116]
	ldr	w3, [x19, 140]
	add	w0, w0, 15
	ldr	w2, [x19, 44]
	lsl	w1, w3, 4
	lsr	w0, w0, 4
	ldr	x4, [x19, 504]
	mul	w1, w0, w1
	add	w0, w3, w2
	str	w0, [x19, 36]
	add	w0, w1, w2
	str	w2, [x19, 28]
	add	x1, x4, x1, sxtw
	add	w3, w3, w0
	str	x1, [x19, 512]
	str	w2, [x19, 60]
	str	x4, [x19, 520]
	str	w0, [x19, 32]
	str	w3, [x19, 40]
	str	w0, [x19, 48]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	ldr	w1, [x19, 32]
	str	w1, [x19, 64]
	b	.L439
	.p2align 3
.L452:
	ldr	w1, [x20, 900]
	mov	w22, 0
	ldr	w0, [x20, 904]
	cmp	w1, wzr
	ccmp	w0, wzr, 4, ne
	cset	w3, eq
	cbnz	w3, .L435
	ldr	w2, [x20, 952]
	mov	w22, w3
	mul	w2, w0, w2
	cbz	w2, .L435
	ldr	w0, [x20, 948]
	lsl	w1, w1, 10
	mul	w0, w1, w0
	udiv	w0, w0, w2
	subs	w2, w0, #1365
	bmi	.L454
.L436:
	subs	w1, w0, #1820
	bmi	.L455
.L437:
	mov	w22, 2
	cmp	w2, w1
	csinc	w22, w22, wzr, ge
	b	.L435
	.p2align 3
.L454:
	mov	w2, 1365
	sub	w2, w2, w0
	b	.L436
	.p2align 3
.L455:
	mov	w1, 1820
	sub	w1, w1, w0
	b	.L437
	.size	Mp4_RecordFrameAtr, .-Mp4_RecordFrameAtr
	.align	3
.LC80:
	.xword	.LC79
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Frame_Atr
	.type	Mp4_Frame_Atr, %function
Mp4_Frame_Atr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 20480
	bl	_mcount
	ldrb	w0, [x19, 1472]
	strb	w0, [x20, 2]
	cmp	w0, 4
	ldr	w0, [x19, 2824]
	beq	.L466
.L458:
	mov	w1, 1
	bl	FSP_NewLogicFs
	str	w0, [x19, 2852]
	tbnz	w0, #31, .L459
	ldr	w1, [x19, 2828]
	cbz	w1, .L459
	mov	w1, w0
	mov	x0, x20
	bl	Mp4_RecordFrameAtr
	str	wzr, [x19, 2128]
	mov	w0, 0
.L462:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L466:
	sxtw	x2, w0
	ldr	x1, .LC81
	lsl	x3, x2, 4
	add	x2, x3, x2, lsl 2
	add	x1, x1, x2, lsl 4
	sub	x1, x1, x2
	ldr	w2, [x1, 12]
	add	w2, w2, 1
	str	w2, [x1, 12]
	b	.L458
	.p2align 3
.L459:
	ldr	x1, .LC83
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L461
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 568]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 576]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 584]
	bl	FreeUsdByDec
	add	x0, x20, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2836]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 2836]
	ldr	w0, [x19, 2824]
	str	wzr, [x19, 2844]
	bl	FSP_GetNullLogicFs
	str	w0, [x19, 2836]
	str	w0, [x19, 2844]
.L461:
	ldr	w0, [x19, 2824]
	add	x1, x19, 2216
	str	wzr, [x19, 2832]
	bl	FSP_ClearNotInVoQueue
	mov	w0, -1
	b	.L462
	.size	Mp4_Frame_Atr, .-Mp4_Frame_Atr
	.align	3
.LC81:
	.xword	g_VfmwChanStat
	.align	3
.LC83:
	.xword	.LC82
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Set_DecPar
	.type	Mp4_Set_DecPar, %function
Mp4_Set_DecPar:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	x20, x2
	bl	_mcount
	ldr	x3, .LC84
	mov	x2, 48
	mov	w1, 0
	add	x0, x29, 64
	ldr	x3, [x3, 96]
	blr	x3
	ldrb	w0, [x20, 9]
	cmp	w0, 1
	beq	.L492
	add	x19, x22, 1048
	ldr	w0, [x20, 92]
	add	x21, x22, 20480
	and	w0, w0, -4
	str	w0, [x19, 180]
	ldr	w0, [x21, 1616]
	str	w0, [x19, 192]
	ubfiz	w0, w0, 3, 2
	ldr	x2, [x22, 22088]
	str	x2, [x19, 16]
	ldr	w2, [x21, 1624]
	add	w0, w0, w2
	str	w0, [x19, 200]
	ldr	w0, [x21, 1620]
	lsl	w0, w0, 3
	sub	w0, w0, w2
	str	w0, [x19, 208]
	ldr	w0, [x20, 92]
	str	w0, [x19, 196]
	ldr	x0, [x20, 80]
	str	x0, [x19, 24]
	ldr	w0, [x20, 92]
	ubfiz	w0, w0, 3, 2
	str	w0, [x19, 204]
	ldr	w0, [x20, 96]
	str	wzr, [x19, 188]
	lsl	w0, w0, 3
	str	w0, [x19, 212]
.L469:
	add	x20, x21, 828
	ldr	w2, [x20, 708]
	add	w0, w2, 15
	lsr	w0, w0, 4
	strh	w0, [x19, 176]
	ldr	w1, [x20, 704]
	add	w0, w1, 15
	lsr	w0, w0, 4
	strh	w0, [x19, 178]
	ldrb	w0, [x20, 644]
	strb	w0, [x22, 1048]
	cmp	w0, 3
	ldrb	w0, [x20, 54]
	strb	w0, [x19, 1]
	ldrb	w0, [x20, 9]
	strb	w0, [x19, 2]
	ldrb	w0, [x20, 651]
	strb	w0, [x19, 3]
	ldrb	w0, [x20, 656]
	strb	w0, [x19, 4]
	ldrb	w0, [x20, 655]
	strb	w0, [x19, 5]
	ldr	w0, [x20, 108]
	strb	w0, [x19, 6]
	ldrb	w0, [x20, 652]
	strb	w0, [x19, 7]
	ldrb	w0, [x20, 653]
	strb	w0, [x19, 8]
	ldrb	w0, [x20, 648]
	strb	w0, [x19, 9]
	ldrb	w0, [x20, 59]
	strb	w0, [x19, 10]
	ldrb	w0, [x20, 57]
	strb	w0, [x19, 11]
	ldrb	w0, [x20, 646]
	strb	w0, [x19, 12]
	ldrb	w0, [x20, 650]
	strb	w0, [x19, 13]
	ldrb	w0, [x20, 649]
	strb	w0, [x19, 14]
	ldrb	w0, [x20, 50]
	strb	w0, [x19, 15]
	ldr	w0, [x20, 740]
	str	w0, [x19, 216]
	ldr	w0, [x20, 736]
	str	w0, [x19, 220]
	ldrb	w0, [x20, 12]
	strb	w0, [x19, 162]
	ldrb	w0, [x20, 11]
	strb	w0, [x19, 161]
	ldrb	w0, [x20, 10]
	strb	w0, [x19, 160]
	ldrb	w0, [x20, 13]
	strb	w0, [x19, 163]
	ldrb	w0, [x20, 14]
	strb	w0, [x19, 164]
	ldrb	w0, [x20, 15]
	strb	w0, [x19, 165]
	str	w1, [x19, 14632]
	str	w2, [x19, 14636]
	ldrb	w0, [x20, 52]
	str	w0, [x19, 14640]
	ccmp	w0, 2, 0, eq
	beq	.L493
	ldrb	w0, [x20, 65]
	str	w0, [x19, 14648]
	str	wzr, [x19, 14644]
	str	wzr, [x19, 14652]
	str	wzr, [x19, 14656]
	str	wzr, [x19, 14660]
	str	wzr, [x19, 14664]
	str	wzr, [x19, 14668]
	str	wzr, [x19, 14672]
	str	wzr, [x19, 14676]
	str	wzr, [x19, 14680]
.L471:
	ldr	w1, [x21, 2852]
	ldr	w0, [x21, 2824]
	bl	FSP_GetLogicFs
	mov	x20, x0
	ldr	w1, [x21, 2836]
	ldr	w0, [x21, 2824]
	bl	FSP_GetLogicFs
	mov	x23, x0
	ldr	w1, [x21, 2844]
	ldr	w0, [x21, 2824]
	bl	FSP_GetLogicFs
	cmp	x20, xzr
	ccmp	x23, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L494
	ldr	x5, [x20, 640]
	ldr	x7, [x20, 648]
	ldr	x6, [x20, 656]
	cbz	x5, .L475
	cbz	x6, .L475
	cbz	x7, .L475
	ldr	w1, [x20, 664]
	str	w1, [x19, 240]
	ldr	w1, [x0, 664]
	str	w1, [x19, 244]
	ldr	x1, [x0, 640]
	cbz	x1, .L495
.L488:
	ldr	w1, [x1, 16]
	str	w1, [x19, 236]
	ldr	x1, [x23, 640]
	cbz	x1, .L496
.L489:
	ldr	w1, [x1, 16]
	str	w1, [x19, 232]
	ldr	x1, [x20, 640]
	ldr	w1, [x1, 16]
	str	w1, [x19, 224]
	ldr	x0, [x0, 656]
	cbz	x0, .L497
.L490:
	ldr	w0, [x0, 16]
	str	w0, [x19, 14592]
	ldr	x0, [x23, 656]
	cbz	x0, .L498
.L491:
	ldr	w0, [x0, 16]
	add	x1, x22, 1080
	str	w0, [x19, 14596]
	add	x3, x21, 1212
	add	x0, x21, 956
	ldr	x2, [x20, 656]
	ldr	w2, [x2, 16]
	str	w2, [x19, 14600]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 16]
	str	w2, [x19, 14604]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 32]
	str	w2, [x19, 14616]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 76]
	str	w2, [x19, 14620]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 60]
	str	w2, [x19, 14624]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 64]
	str	w2, [x19, 14628]
	.p2align 2
.L486:
	ldr	w2, [x0]
	add	x0, x0, 4
	strb	w2, [x1]
	add	x1, x1, 1
	cmp	x3, x0
	ldr	w2, [x0, 252]
	strb	w2, [x1, 63]
	bne	.L486
	ldrh	w1, [x19, 178]
	ldrh	w0, [x19, 176]
	mul	w0, w0, w1
	str	w0, [x21, 1504]
	ldr	x0, [x22, 22112]
	ldr	w1, [x0, 592]
	str	w1, [x19, 14684]
	ldr	w0, [x0, 632]
	str	w0, [x19, 14688]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
.L494:
	ldr	x2, .LC85
	mov	w3, 3468
	ldr	x1, .LC87
	mov	w0, 0
	add	x2, x2, 1280
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 112
	ret
.L492:
	add	x21, x22, 20480
	add	x19, x22, 1048
	ldr	w1, [x21, 1616]
	str	w1, [x19, 180]
	and	w3, w1, -4
	str	w3, [x19, 192]
	ubfiz	w1, w1, 3, 2
	ldr	x3, [x22, 22088]
	str	x3, [x19, 16]
	ldr	w3, [x21, 1624]
	add	w1, w1, w3
	str	w1, [x19, 200]
	ldr	w1, [x21, 1620]
	str	w0, [x19, 188]
	lsl	w0, w1, 3
	str	wzr, [x19, 196]
	sub	w0, w0, w3
	str	wzr, [x19, 204]
	str	wzr, [x19, 212]
	str	w0, [x19, 208]
	b	.L469
.L498:
	ldr	x0, [x20, 656]
	b	.L491
.L495:
	ldr	x1, [x20, 640]
	b	.L488
.L496:
	ldr	x1, [x20, 640]
	b	.L489
.L497:
	ldr	x0, [x20, 656]
	b	.L490
.L475:
	ldr	w2, [x21, 2836]
	mov	w0, 0
	ldr	w4, [x21, 2844]
	ldr	w3, [x21, 2852]
	ldr	x1, .LC89
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 112
	ret
.L493:
	add	x1, x29, 64
	mov	x0, x20
	bl	MP4_GetGMCparam
	ldrb	w2, [x20, 65]
	ldp	w6, w0, [x29, 80]
	ldr	w1, [x29, 64]
	ldp	w5, w4, [x29, 88]
	ldr	w3, [x29, 96]
	str	w1, [x19, 14644]
	str	w2, [x19, 14648]
	str	w0, [x19, 14656]
	ldp	w2, w1, [x29, 100]
	ldr	w0, [x29, 108]
	str	w6, [x19, 14652]
	str	w5, [x19, 14660]
	str	w4, [x19, 14664]
	str	w3, [x19, 14668]
	str	w2, [x19, 14672]
	str	w1, [x19, 14676]
	str	w0, [x19, 14680]
	b	.L471
	.size	Mp4_Set_DecPar, .-Mp4_Set_DecPar
	.align	3
.LC84:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC85:
	.xword	.LANCHOR1
	.align	3
.LC87:
	.xword	.LC86
	.align	3
.LC89:
	.xword	.LC88
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Get_DecMode
	.type	Mp4_Get_DecMode, %function
Mp4_Get_DecMode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 20480
	ldr	w0, [x1, 1676]
	cmp	w0, 2
	ldr	x0, [x19, 22112]
	beq	.L511
	ldr	w0, [x0, 8]
	cmp	w0, 2
	beq	.L504
.L512:
	cmp	w0, 1
	beq	.L510
	cmp	w0, 4
	csel	w0, w0, wzr, eq
.L510:
	str	w0, [x1, 1676]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L511:
	ldr	w2, [x0, 8]
	cmp	w2, 2
	beq	.L504
	mov	w2, 1
	str	w2, [x1, 2208]
	ldr	w0, [x0, 8]
	cmp	w0, 2
	bne	.L512
.L504:
	mov	w0, 2
	str	w0, [x1, 1676]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Mp4_Get_DecMode, .-Mp4_Get_DecMode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetTrailingOneLen
	.type	GetTrailingOneLen, %function
GetTrailingOneLen:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w1
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, x19, sxtw
	bl	_mcount
	mov	w2, 0
	ldrb	w1, [x19, -1]
	tbnz	x1, 0, .L515
	b	.L520
	.p2align 3
.L517:
	mov	w2, w0
.L515:
	lsr	w1, w1, 1
	add	w0, w2, 1
	tbnz	x1, 0, .L517
	add	w2, w2, 2
	cmp	w0, wzr
	csel	w0, w2, w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L520:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	GetTrailingOneLen, .-GetTrailingOneLen
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_Init
	.type	MPEG4DEC_Init, %function
MPEG4DEC_Init:
	sub	sp, sp, #784
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	ldr	x1, .LC91
	mov	w0, 2
	bl	dprint_vfmw
	cmp	x19, xzr
	ccmp	x21, xzr, 4, ne
	beq	.L526
	ldr	w0, [x21, 608]
	add	x20, x19, 20480
	ldr	x22, .LC92
	cmp	w0, 1
	beq	.L529
.L524:
	ldr	x3, [x22, 96]
	mov	x2, 23352
	mov	w1, 0
	mov	x0, x19
	ldr	w23, [x20, 2816]
	blr	x3
	str	w23, [x20, 2816]
	add	x0, x20, 2216
	bl	ResetVoQueue
	str	x21, [x19, 22112]
	mov	x0, x19
	str	wzr, [x21, 872]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x20, 2824]
	beq	.L530
	mov	x0, x19
	bl	Global_Data_Init
	mov	w23, w0
	cbnz	w0, .L526
	mov	w0, 1
	ldr	x1, .LC96
	str	w0, [x20, 2132]
	mov	w0, 2
	str	wzr, [x20, 2160]
	bl	dprint_vfmw
	ldr	w21, [x21, 608]
	cmp	w21, 1
	beq	.L531
.L523:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, w23
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 784
	ret
	.p2align 3
.L531:
	ldr	x3, [x22, 104]
	add	x1, x29, 80
	mov	x2, 768
	add	x0, x20, 828
	add	x19, x19, 16384
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x29, 96
	mov	x2, 24
	add	x0, x19, 3632
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x29, 120
	mov	x2, 604
	add	x0, x19, 3656
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x29, 72
	mov	x2, 7
	add	x0, x20, 1596
	blr	x3
	strb	w21, [x20, 2861]
	mov	w0, w23
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 784
	ret
	.p2align 3
.L529:
	ldr	x3, [x22, 104]
	add	x1, x20, 828
	mov	x2, 768
	add	x0, x29, 80
	add	x23, x19, 16384
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x23, 3632
	mov	x2, 24
	add	x0, x29, 96
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x23, 3656
	mov	x2, 604
	add	x0, x29, 120
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 7
	add	x1, x20, 1596
	add	x0, x29, 72
	blr	x3
	b	.L524
	.p2align 3
.L526:
	mov	w23, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, w23
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 784
	ret
	.p2align 3
.L530:
	ldr	x1, .LC94
	mov	w0, 0
	mov	w23, -20
	bl	dprint_vfmw
	b	.L523
	.size	MPEG4DEC_Init, .-MPEG4DEC_Init
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_Destroy
	.type	MPEG4DEC_Destroy, %function
MPEG4DEC_Destroy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 20480
	bl	_mcount
	ldr	x1, .LC98
	mov	w0, 2
	bl	dprint_vfmw
	str	wzr, [x19, 2132]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MPEG4DEC_Destroy, .-MPEG4DEC_Destroy
	.align	3
.LC98:
	.xword	.LC97
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_RecycleImage
	.type	MPEG4DEC_RecycleImage, %function
MPEG4DEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L535
	add	x19, x19, 20480
	mov	w1, w20
	mov	w2, 0
	ldr	w0, [x19, 2824]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 2824]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L536
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 2824]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
.L534:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L536:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L535:
	mov	w0, -2
	b	.L534
	.size	MPEG4DEC_RecycleImage, .-MPEG4DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_SetImgFormat
	.type	MP4_SetImgFormat, %function
MP4_SetImgFormat:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 20480
	bl	_mcount
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2852]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L537
	ldr	w1, [x19, 1536]
	add	x19, x19, 828
	cbnz	w1, .L550
.L539:
	ldp	w2, w1, [x0, 112]
	stp	w2, w1, [x0, 120]
.L540:
	mov	w2, 1
	strb	w2, [x0, 369]
	add	x2, x0, 256
	strb	wzr, [x0, 364]
	strb	wzr, [x0, 371]
	mov	w1, 8
	mov	w3, 2
	stp	w1, w1, [x2, 128]
	stp	w3, w1, [x2, 120]
.L537:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L550:
	ldr	w2, [x19, 704]
	cbz	w2, .L539
	ldp	w2, w3, [x0, 112]
	cmp	w2, w1
	csel	w2, w2, w1, ls
	str	w2, [x0, 120]
	ldr	w1, [x19, 704]
	cmp	w1, w3
	csel	w1, w1, w3, ls
	str	w1, [x0, 124]
	b	.L540
	.size	MP4_SetImgFormat, .-MP4_SetImgFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_GetRemainImg
	.type	MPEG4DEC_GetRemainImg, %function
MPEG4DEC_GetRemainImg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 20480
	bl	_mcount
	ldr	x0, [x20, 22112]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L552
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L552
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_GetDisplay
	cmp	w0, 2
	mov	w23, w0
	beq	.L565
.L552:
	add	x21, x19, 2216
.L556:
	mov	x0, x21
	bl	GetVoLastImageID
.L562:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L565:
	mov	x0, x20
	add	x21, x19, 2216
	bl	MP4_SetImgFormat
	mov	w2, 1
	str	w2, [x22, 188]
	ldr	w1, [x22, 156]
	ldr	w0, [x19, 2824]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 2824]
	mov	x4, x22
	mov	x3, x21
	mov	x2, x20
	mov	w1, w23
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L553
	ldr	w1, [x22, 156]
	mov	x0, x20
	bl	MPEG4DEC_RecycleImage
	ldr	w0, [x19, 2824]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	ldr	x1, [x22, 568]
	ldr	w0, [x19, 2824]
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	b	.L556
	.p2align 3
.L553:
	ldr	w0, [x22, 160]
	cmp	w0, 100
	bhi	.L552
	ldr	w0, [x22, 112]
	cmp	w0, 31
	bls	.L552
	ldr	w1, [x22, 116]
	mov	w0, 0
	cmp	w1, 31
	bhi	.L562
	b	.L556
	.size	MPEG4DEC_GetRemainImg, .-MPEG4DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_GetImageBuffer
	.type	MPEG4DEC_GetImageBuffer, %function
MPEG4DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 20480
	bl	_mcount
	ldr	w0, [x19, 2824]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	mov	w20, w0
	beq	.L567
	ldr	w0, [x19, 2824]
	mov	w20, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L572
.L567:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L572:
	ldr	w0, [x19, 2824]
	add	x1, x19, 2216
	bl	FSP_ClearNotInVoQueue
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MPEG4DEC_GetImageBuffer, .-MPEG4DEC_GetImageBuffer
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_GetPacketState
	.type	MP4_GetPacketState, %function
MP4_GetPacketState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 16384
	bl	_mcount
	mov	w0, 3
	ldr	w1, [x19, 3628]
	ldr	x19, [sp, 16]
	cmp	w1, 1
	csinv	w0, w0, wzr, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MP4_GetPacketState, .-MP4_GetPacketState
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_ClearCurPacket
	.type	MP4_ClearCurPacket, %function
MP4_ClearCurPacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 20]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x19, 9]
	cbnz	w0, .L577
	ldrb	w0, [x19, 89]
	cmp	w0, 1
	beq	.L578
.L577:
	ldr	x3, .LC99
	mov	x2, 160
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L578:
	ldr	w1, [x19, 100]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
	b	.L577
	.size	MP4_ClearCurPacket, .-MP4_ClearCurPacket
	.align	3
.LC99:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_ClearCurPic
	.type	MP4_ClearCurPic, %function
MP4_ClearCurPic:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x22, x20, 16384
	bl	_mcount
	add	x1, x20, 20480
	ldr	w0, [x22, 3620]
	cmp	w0, wzr
	ldr	w21, [x1, 2824]
	ble	.L582
	sub	w0, w0, #1
	add	x19, x20, 12288
	add	x0, x0, 3978
	add	x19, x19, 3620
	add	x20, x20, x0, lsl 2
	.p2align 2
.L581:
	ldr	w1, [x19], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x19, x20
	bne	.L581
.L582:
	str	wzr, [x22, 3620]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP4_ClearCurPic, .-MP4_ClearCurPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_GetScdUpMsg
	.type	MP4_GetScdUpMsg, %function
MP4_GetScdUpMsg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x1, [x19, 22112]
	add	x0, x19, 12288
	add	x19, x19, 20480
	add	x0, x0, 3616
	add	x19, x19, 2868
	ldr	w2, [x1, 832]
	cmp	w2, 1
	beq	.L588
.L586:
	ldr	x21, .LC100
	add	x20, x20, 72
	mov	x2, 4
	mov	x1, x20
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 104]
	mov	x2, 4
	mov	x1, x20
	mov	x0, x19
	blr	x3
.L585:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L588:
	ldr	w1, [x1, 872]
	cmp	w1, 1
	bne	.L586
	ldr	x3, .LC100
	mov	x2, 4
	mov	x1, x19
	ldr	x3, [x3, 104]
	blr	x3
	b	.L585
	.size	MP4_GetScdUpMsg, .-MP4_GetScdUpMsg
	.align	3
.LC100:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Check_Slice_NotSH
	.type	Mp4_Check_Slice_NotSH, %function
Mp4_Check_Slice_NotSH:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 16384
	add	x3, x19, 20480
	ldrb	w0, [x0, 3675]
	ldrb	w2, [x3, 768]
	cbnz	w0, .L597
	add	x19, x19, 12288
	ldrb	w0, [x19, 3616]
	tst	w0, 120
	bne	.L597
	ldrh	w0, [x19, 3616]
	cmp	w2, 3
	ubfx	x0, x0, 7, 4
	add	w0, w0, 17
	bls	.L599
	ldr	x1, .LC103
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L590:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L599:
	ldr	x1, .LC101
	ldrb	w1, [x1,w2,uxtw]
	adr	x2, .Lrtx593
	add	x1, x2, w1, sxtb #2
	br	x1
.Lrtx593:
	.section	.rodata
	.align	0
	.align	2
.L593:
	.byte	(.L598 - .Lrtx593) / 4
	.byte	(.L594 - .Lrtx593) / 4
	.byte	(.L595 - .Lrtx593) / 4
	.byte	(.L594 - .Lrtx593) / 4
	.text
	.p2align 3
.L598:
	mov	w1, 17
.L592:
	cmp	w1, w0
	ldr	x19, [sp, 16]
	csetm	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L595:
	ldrb	w2, [x3, 777]
	ldrb	w1, [x3, 776]
	mov	w3, 18
	cmp	w1, w2, uxtb
	csel	w1, w1, w2, cs
	uxtb	w1, w1
	add	w1, w1, 16
	cmp	w1, w3
	csel	w1, w1, w3, ge
	b	.L592
	.p2align 3
.L594:
	ldrb	w1, [x3, 776]
	add	w1, w1, 16
	b	.L592
	.p2align 3
.L597:
	mov	w0, -1
	b	.L590
	.size	Mp4_Check_Slice_NotSH, .-Mp4_Check_Slice_NotSH
	.align	3
.LC101:
	.xword	.L593
	.align	3
.LC103:
	.xword	.LC102
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Combine_SliceData
	.type	Mp4_Combine_SliceData, %function
Mp4_Combine_SliceData:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	add	x0, x19, 16384
	add	x4, x19, 1300
	ldr	w3, [x19, 15636]
	ldr	w0, [x0, 3624]
	tbz	x0, 1, .L601
	add	x1, x19, 12288
	ldrb	w0, [x1, 3465]
	cmp	w0, 1
	beq	.L615
	cbnz	w0, .L609
	ldrb	w0, [x1, 3545]
	cmp	w0, 1
	bne	.L609
	sub	w2, w3, #1
	lsl	x0, x2, 3
	sub	x2, x0, x2
	add	x2, x4, x2, lsl 2
	ldr	w0, [x2, 12]
	cbnz	w0, .L611
	ldr	w0, [x2, 20]
	cbnz	w0, .L611
	ldr	w1, [x2, 8]
	add	w20, w1, w20, lsl 3
	str	w20, [x2, 8]
	ldr	w1, [x19, 15836]
	str	w1, [x2, 20]
	ldr	w1, [x19, 15840]
	lsl	w1, w1, 3
	str	w1, [x2, 12]
.L604:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L615:
	sub	w2, w3, #1
	cmp	w2, 510
	bhi	.L616
	lsl	x1, x2, 3
	sub	x1, x1, x2
	add	x2, x4, x1, lsl 2
	ldr	w1, [x2, 12]
	cbz	w1, .L605
	ldr	w3, [x2, 20]
	cbz	w3, .L605
	ldr	w4, [x19, 15756]
	cmp	w3, w4
	bhi	.L617
	add	w1, w1, w20, lsl 3
	mov	w0, 0
	str	w1, [x2, 12]
	b	.L604
	.p2align 3
.L605:
	ldr	w0, [x2, 16]
	ldr	w1, [x19, 15756]
	cmp	w1, w0
	bls	.L607
	ldr	w1, [x2, 8]
	mov	w0, 0
	add	w20, w1, w20, lsl 3
	str	w20, [x2, 8]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L607:
	lsl	w20, w20, 3
	str	w20, [x2, 12]
	mov	w0, 0
	ldr	w1, [x19, 15756]
	str	w1, [x2, 20]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L609:
	ldr	x1, .LC107
	mov	w2, 4833
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L604
	.p2align 3
.L611:
	mov	w0, -1
	b	.L604
	.p2align 3
.L601:
	ldr	x1, .LC107
	mov	w2, 4839
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L604
.L616:
	ldr	x1, .LC105
	mov	w2, w3
	bl	dprint_vfmw
	mov	w0, -1
	b	.L604
.L617:
	ldr	x1, .LC107
	mov	w2, 4809
	bl	dprint_vfmw
	mov	w0, -1
	b	.L604
	.size	Mp4_Combine_SliceData, .-Mp4_Combine_SliceData
	.align	3
.LC105:
	.xword	.LC104
	.align	3
.LC107:
	.xword	.LC106
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Check_StreamType
	.type	MP4_Check_StreamType, %function
MP4_Check_StreamType:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	add	x2, x19, 12288
	add	x19, x19, 20480
	ldrb	w0, [x2, 3616]
	tbnz	x0, 0, .L619
	mov	w0, 2
	strb	w0, [x19, 837]
	ldr	w0, [x20]
	lsr	w0, w0, 8
	sub	w0, w0, #128
	cmp	w0, 3
	bhi	.L620
	ldrb	w0, [x2, 3616]
	orr	w0, w0, 2
	strb	w0, [x2, 3616]
.L620:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L619:
	mov	w0, 196
	str	w0, [x20]
	mov	w0, 1
	strb	w0, [x19, 837]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP4_Check_StreamType, .-MP4_Check_StreamType
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Clean_HeadPacket
	.type	MP4_Clean_HeadPacket, %function
MP4_Clean_HeadPacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x22, x20, 20480
	bl	_mcount
	mov	w1, 64
	ldr	w0, [x22, 2100]
	ldr	w21, [x22, 2824]
	cmp	w0, w1
	csel	w0, w0, w1, le
	cmp	w0, wzr
	ble	.L624
	sub	w0, w0, #1
	add	x19, x22, 1844
	add	x0, x0, 4096
	add	x0, x0, 1486
	add	x20, x20, x0, lsl 2
	.p2align 2
.L623:
	ldr	w1, [x19], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x19, x20
	bne	.L623
.L624:
	ldr	x3, .LC108
	mov	x2, 440
	mov	w1, 0
	add	x0, x22, 1680
	ldr	x3, [x3, 96]
	blr	x3
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP4_Clean_HeadPacket, .-MP4_Clean_HeadPacket
	.align	3
.LC108:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_VDMPostProc
	.type	MPEG4DEC_VDMPostProc, %function
MPEG4DEC_VDMPostProc:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x23, x0
	mov	x0, x30
	mov	w25, w1
	add	x24, x23, 16384
	bl	_mcount
	add	x20, x23, 20480
	ldr	x3, .LC109
	mov	x2, 14696
	mov	w1, 0
	add	x0, x23, 1048
	ldr	x3, [x3, 96]
	blr	x3
	ldr	w22, [x20, 2824]
	ldr	w0, [x24, 3620]
	cmp	w0, wzr
	ble	.L632
	sub	w21, w0, #1
	add	x19, x23, 12288
	add	x21, x21, 3978
	add	x19, x19, 3620
	add	x21, x23, x21, lsl 2
	.p2align 2
.L631:
	ldr	w1, [x19], 4
	mov	w0, w22
	bl	SM_ReleaseStreamSeg
	cmp	x21, x19
	bne	.L631
.L632:
	mov	x0, x23
	str	wzr, [x24, 3620]
	bl	MP4_Clean_HeadPacket
	str	w25, [x20, 2128]
	ldr	w1, [x20, 2852]
	ldr	w0, [x20, 2824]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	ldr	w1, [x20, 2844]
	ldr	w0, [x20, 2824]
	bl	FSP_GetFsImagePtr
	mov	x25, x0
	cbz	x24, .L683
	ldr	x1, .LC113
	mov	w0, 11
	ldr	w2, [x20, 2136]
	add	x21, x20, 2168
	mov	x19, 0
	bl	dprint_vfmw
	ldrb	w2, [x20, 1472]
	mov	w0, 11
	ldr	x1, .LC115
	bl	dprint_vfmw
	ldr	w0, [x20, 2128]
	str	w0, [x24, 160]
	ldr	w22, [x20, 2824]
.L634:
	ldr	x1, [x21, x19]
	mov	w0, w22
	bl	FreeUsdByDec
	str	xzr, [x21, x19]
	add	x19, x19, 8
	cmp	x19, 32
	bne	.L634
	ldrb	w0, [x20, 1472]
	cmp	w0, 2
	beq	.L635
	ldr	x1, [x23, 22112]
	ldr	w0, [x20, 2128]
	ldr	w2, [x1]
	cmp	w0, w2
	bhi	.L684
.L635:
	mov	x0, x23
	str	wzr, [x20, 2208]
	bl	MP4_SetImgFormat
	ldr	x0, [x23, 22112]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L638
	ldrb	w19, [x20, 1472]
	cmp	w19, 2
	beq	.L639
	ldr	w0, [x20, 2832]
	cmp	w0, 1
	ble	.L640
	ldr	w1, [x20, 2836]
	str	w1, [x20, 2856]
	ldr	w22, [x20, 2840]
	ldrb	w19, [x23]
.L641:
	cmn	w1, #1
	beq	.L643
	ldr	w0, [x20, 2824]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L685
	cmp	w19, 4
	ccmp	w22, wzr, 0, eq
	beq	.L686
	ldr	w2, [x0, 160]
	cbz	w2, .L646
	ldr	x0, [x23, 22112]
	ldr	w3, [x0, 4]
	cmp	w2, w3
	bhi	.L647
.L646:
	ldr	w0, [x20, 2824]
	ldr	w1, [x20, 2856]
	bl	FSP_GetLogicFs
	cbz	x0, .L680
	cmp	w19, 2
	beq	.L649
	ldrsb	w0, [x0, 1]
	cmp	w0, 2
	beq	.L649
	ldr	x1, .LC121
	mov	w0, 11
	ldr	w2, [x20, 2856]
	bl	dprint_vfmw
	.p2align 2
.L643:
	ldr	w1, [x20, 2136]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x20, 2136]
.L679:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L638:
	ldr	w1, [x20, 2852]
	ldrb	w19, [x20, 1472]
	ldr	w22, [x20, 1564]
	str	w1, [x20, 2856]
	b	.L641
	.p2align 3
.L639:
	ldr	w1, [x20, 2852]
	str	w1, [x20, 2856]
	ldr	w22, [x20, 1564]
	cbz	x25, .L641
	ldr	x0, [x24, 600]
	ldr	x2, [x25, 600]
	cmp	x2, x0
	bge	.L641
	str	x2, [x24, 600]
	str	x0, [x25, 600]
	ldr	w1, [x20, 2856]
	b	.L641
	.p2align 3
.L647:
	ldr	x0, .LC116
	ldr	x4, [x0]
	cbz	x4, .L652
	ldr	w0, [x20, 2824]
	mov	w1, 104
	stp	w2, w3, [x29, 88]
	mov	w3, 8
	add	x2, x29, 88
	blr	x4
	ldr	x0, [x23, 22112]
	ldr	w2, [x21, 160]
	ldr	w3, [x0, 4]
.L652:
	ldr	x1, .LC123
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 2824]
	ldr	x1, [x21, 560]
	bl	FreeUsdByDec
	str	xzr, [x21, 560]
	ldr	x1, [x21, 568]
	ldr	w0, [x20, 2824]
	bl	FreeUsdByDec
	str	xzr, [x21, 568]
	mov	w2, 0
	ldr	w0, [x20, 2824]
	ldr	w1, [x20, 2856]
	bl	FSP_SetDisplay
	b	.L643
	.p2align 3
.L649:
	ldr	w0, [x20, 2824]
	mov	w2, 1
	ldr	w1, [x20, 2856]
	bl	FSP_SetDisplay
	ldr	w0, [x20, 2824]
	mov	x4, x21
	add	x3, x20, 2216
	mov	x2, x23
	mov	w1, 2
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L643
.L680:
	ldr	w0, [x20, 2824]
	ldr	x1, [x21, 560]
.L681:
	bl	FreeUsdByDec
	str	xzr, [x21, 560]
	ldr	x1, [x21, 568]
	ldr	w0, [x20, 2824]
	bl	FreeUsdByDec
	str	xzr, [x21, 568]
	mov	w2, 0
	ldr	w0, [x20, 2824]
	ldr	w1, [x20, 2856]
	bl	FSP_SetDisplay
	mov	w0, -1
	b	.L679
	.p2align 3
.L640:
	mov	w0, -1
	str	w0, [x20, 2856]
	b	.L643
.L684:
	ldr	x2, .LC116
	ldr	x4, [x2]
	cbz	x4, .L636
	str	w0, [x29, 88]
	mov	w3, 8
	ldr	w0, [x20, 2824]
	add	x2, x29, 88
	ldr	w5, [x1]
	mov	w1, 104
	str	w5, [x29, 92]
	blr	x4
.L636:
	ldr	w0, [x20, 2824]
	mov	w21, 1
	ldr	w1, [x20, 2844]
	str	w21, [x20, 2208]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L637
	ldr	w0, [x20, 2824]
	ldr	x1, [x19, 560]
	bl	FreeUsdByDec
	ldr	w0, [x20, 2824]
	ldr	x1, [x19, 568]
	bl	FreeUsdByDec
	ldr	w0, [x20, 2824]
	ldr	x1, [x19, 576]
	bl	FreeUsdByDec
	ldr	w0, [x20, 2824]
	ldr	x1, [x19, 584]
	bl	FreeUsdByDec
	add	x0, x19, 512
	mov	w2, w21
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x20, 2824]
	ldr	w1, [x20, 2844]
	bl	FSP_ClearLogicFs
	ldr	w0, [x20, 2824]
	mov	w2, w21
	ldr	w1, [x20, 2836]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 2836]
	ldr	w0, [x20, 2824]
	str	wzr, [x20, 2844]
	bl	FSP_GetNullLogicFs
	str	w0, [x20, 2836]
	str	w0, [x20, 2844]
.L637:
	mov	w0, -1
	str	wzr, [x20, 2832]
	b	.L679
.L683:
	ldr	x1, .LC111
	mov	x3, x0
	mov	x2, 0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L679
.L686:
	ldr	w1, [x20, 2824]
	ldr	x2, .LC119
	mov	w0, w1
	sxtw	x1, w1
	lsl	x3, x1, 4
	add	x1, x3, x1, lsl 2
	add	x2, x2, x1, lsl 4
	sub	x1, x2, x1
	ldr	w2, [x1, 16]
	add	w2, w2, 1
	str	w2, [x1, 16]
	ldr	x1, [x21, 560]
	b	.L681
.L685:
	ldr	x1, .LC118
	mov	w2, 4541
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L679
	.size	MPEG4DEC_VDMPostProc, .-MPEG4DEC_VDMPostProc
	.align	3
.LC109:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC111:
	.xword	.LC110
	.align	3
.LC113:
	.xword	.LC112
	.align	3
.LC115:
	.xword	.LC114
	.align	3
.LC116:
	.xword	g_event_report
	.align	3
.LC118:
	.xword	.LC117
	.align	3
.LC119:
	.xword	g_VfmwChanStat
	.align	3
.LC121:
	.xword	.LC120
	.align	3
.LC123:
	.xword	.LC122
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Record_SlcSegID
	.type	MP4_Record_SlcSegID, %function
MP4_Record_SlcSegID:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x22, x20, 16384
	bl	_mcount
	ldr	w0, [x22, 3620]
	cmp	w0, 1022
	bhi	.L694
	add	x1, x20, x0, uxtw 2
	ldr	w4, [x20, 15764]
	add	x3, x20, 12288
	add	w2, w0, 1
	str	w4, [x1, 15908]
	str	w2, [x22, 3620]
	ldrb	w1, [x3, 3465]
	cbnz	w1, .L687
	ldrb	w1, [x3, 3545]
	cmp	w1, 1
	beq	.L695
.L687:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L694:
	mov	x0, x20
	add	x23, x20, 20480
	bl	MP4_Clean_HeadPacket
	ldr	w1, [x22, 3620]
	ldr	w21, [x23, 2824]
	cmp	w1, wzr
	ble	.L692
	sub	w1, w1, #1
	add	x24, x20, 12288
	add	x1, x1, 3978
	add	x19, x24, 3620
	add	x20, x20, x1, lsl 2
	.p2align 2
.L690:
	ldr	w1, [x19], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x19, x20
	bne	.L690
	ldr	w0, [x23, 2824]
.L689:
	add	x1, x24, 3456
	str	wzr, [x22, 3620]
	bl	MP4_ClearCurPacket
	ldr	w2, [x22, 3620]
	mov	w3, 1022
	ldr	x1, .LC125
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L695:
	add	x2, x20, x2, uxtw 2
	ldr	w1, [x20, 15844]
	add	w0, w0, 2
	str	w1, [x2, 15908]
	str	w0, [x22, 3620]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L692:
	mov	w0, w21
	add	x24, x20, 12288
	b	.L689
	.size	MP4_Record_SlcSegID, .-MP4_Record_SlcSegID
	.align	3
.LC125:
	.xword	.LC124
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_ShortHeader
	.type	Mp4_Syntax_ShortHeader, %function
Mp4_Syntax_ShortHeader:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w2
	mov	x20, x1
	bl	_mcount
	ldr	w2, [x21, 15636]
	cbnz	w2, .L737
.L697:
	mov	x1, x20
	mov	w2, w19
	add	x0, x29, 72
	add	x20, x21, 20480
	bl	BsInit
	ldr	x3, .LC128
	mov	x2, 60
	mov	w1, 0
	add	x0, x20, 768
	ldr	x3, [x3, 96]
	blr	x3
	mov	w1, 22
	add	x0, x29, 72
	bl	BsSkip
	mov	w1, 13
	add	x0, x29, 72
	bl	BsSkip
	mov	w1, 3
	add	x0, x29, 72
	bl	BsGet
	uxtb	w0, w0
	sub	w1, w0, #1
	strb	w0, [x20, 778]
	cmp	w1, 3
	bls	.L738
	strb	wzr, [x20, 1601]
	add	x0, x29, 72
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC130
	ldr	x4, [x0]
	cbz	x4, .L722
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L738:
	ldr	x0, .LC129
	ldrb	w0, [x0,w1,uxtw]
	adr	x1, .Lrtx700
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx700:
	.section	.rodata
	.align	0
	.align	2
.L700:
	.byte	(.L699 - .Lrtx700) / 4
	.byte	(.L701 - .Lrtx700) / 4
	.byte	(.L702 - .Lrtx700) / 4
	.byte	(.L703 - .Lrtx700) / 4
	.text
	.p2align 3
.L702:
	mov	w3, 18
	mov	w2, 22
	mov	w1, 352
	mov	w0, 288
	strb	w3, [x20, 779]
	strb	w2, [x20, 780]
	str	w0, [x20, 1532]
	str	w1, [x20, 1536]
.L704:
	mov	w1, 1
	add	x0, x29, 72
	bl	BsGet
	uxtb	w0, w0
	ldrb	w1, [x20, 835]
	strb	w0, [x20, 768]
	cmp	w1, 2
	beq	.L706
	ldr	w1, [x20, 2208]
	cmp	w1, 1
	ccmp	w0, wzr, 4, eq
	bne	.L706
	mov	w1, 4
	add	x0, x29, 72
	bl	BsSkip
	mov	w1, 5
	add	x0, x29, 72
	bl	BsGet
	uxtb	w0, w0
	cbnz	w0, .L736
	mov	w0, 1
.L736:
	strb	w0, [x20, 775]
	mov	w1, 1
	add	x0, x29, 72
	lsl	w22, w19, 3
	mov	w19, 0
	bl	BsSkip
.L712:
	mov	w1, 1
	add	x0, x29, 72
	bl	BsGet
	cmp	w0, 1
	beq	.L739
	add	w19, w19, 1
	cmp	w19, 15
	bls	.L713
.L735:
	strb	wzr, [x20, 1601]
	add	x0, x29, 72
	bl	BsPos
	mov	w19, w0
	ldr	x0, .LC130
	ldr	x4, [x0]
	cbz	x4, .L722
	ldr	w0, [x20, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L701:
	mov	w3, 9
	mov	w2, 11
	mov	w1, 176
	mov	w0, 144
	strb	w3, [x20, 779]
	strb	w2, [x20, 780]
	str	w0, [x20, 1532]
	str	w1, [x20, 1536]
	b	.L704
	.p2align 3
.L699:
	mov	w3, 6
	mov	w2, 8
	mov	w1, 128
	mov	w0, 96
	strb	w3, [x20, 779]
	strb	w2, [x20, 780]
	str	w0, [x20, 1532]
	str	w1, [x20, 1536]
	b	.L704
	.p2align 3
.L703:
	mov	w3, 18
	mov	w2, 88
	mov	w1, 704
	mov	w0, 576
	strb	w3, [x20, 779]
	strb	w2, [x20, 780]
	str	w0, [x20, 1532]
	str	w1, [x20, 1536]
	b	.L704
	.p2align 3
.L739:
	add	x0, x29, 72
	add	w19, w19, 1
	bl	BsPos
	cmp	w0, w22
	mov	w1, 8
	add	x0, x29, 72
	bgt	.L740
	bl	BsSkip
	cmp	w19, 15
	bls	.L712
	b	.L735
	.p2align 3
.L737:
	ldr	x1, .LC127
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x21, 15636]
	b	.L697
	.p2align 3
.L713:
	add	x24, x21, 16384
	mov	w2, 1
	strb	w2, [x20, 1601]
	add	x0, x29, 72
	add	x19, x21, 12288
	ldr	w1, [x24, 3624]
	orr	w1, w1, 2
	str	w1, [x24, 3624]
	ldr	w1, [x21, 15636]
	add	w1, w1, 1
	str	w1, [x21, 15636]
	strb	w2, [x20, 776]
	bl	BsPos
	mov	w23, w0
	asr	w0, w0, 31
	sub	w22, w22, w23
	lsr	w0, w0, 29
	str	w22, [x21, 1308]
	add	w1, w23, w0
	add	w2, w23, 7
	ldr	w3, [x20, 1624]
	and	w1, w1, 7
	sub	w0, w1, w0
	cmp	w23, wzr
	add	w0, w3, w0
	strb	w0, [x21, 1306]
	csel	w0, w2, w23, lt
	ldr	w1, [x20, 1616]
	add	w0, w1, w0, asr 3
	str	w0, [x21, 1316]
	ldrb	w0, [x19, 3465]
	cbz	w0, .L741
.L714:
	strb	wzr, [x21, 1307]
	str	wzr, [x21, 1312]
	str	wzr, [x21, 1320]
.L715:
	ldrb	w0, [x20, 768]
	strb	w0, [x21, 1304]
	ldrb	w0, [x20, 777]
	strb	w0, [x21, 1302]
	ldrb	w0, [x20, 776]
	strb	w0, [x21, 1301]
	ldrb	w0, [x20, 775]
	strb	w0, [x21, 1305]
	ldrb	w0, [x20, 772]
	strb	w0, [x21, 1303]
	ldr	w2, [x24, 3620]
	cbnz	w2, .L742
.L716:
	mov	x0, x21
	bl	MP4_Record_SlcSegID
	mov	w0, w23
.L734:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L706:
	add	x0, x29, 72
	bl	BsPos
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L741:
	ldrb	w0, [x19, 3545]
	cmp	w0, 1
	bne	.L714
	ldr	w0, [x21, 15840]
	ldr	w1, [x21, 15836]
	lsl	w0, w0, 3
	strb	wzr, [x21, 1307]
	str	w0, [x21, 1312]
	str	w1, [x21, 1320]
	b	.L715
	.p2align 3
.L722:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
.L742:
	ldr	x1, .LC132
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w22, [x20, 2824]
	ldr	w0, [x24, 3620]
	cmp	w0, wzr
	ble	.L719
	sub	w20, w0, #1
	add	x19, x19, 3620
	add	x20, x20, 3978
	add	x20, x21, x20, lsl 2
	.p2align 2
.L718:
	ldr	w1, [x19], 4
	mov	w0, w22
	bl	SM_ReleaseStreamSeg
	cmp	x19, x20
	bne	.L718
.L719:
	str	wzr, [x24, 3620]
	b	.L716
.L740:
	bl	BsPos
	b	.L734
	.size	Mp4_Syntax_ShortHeader, .-Mp4_Syntax_ShortHeader
	.align	3
.LC127:
	.xword	.LC126
	.align	3
.LC128:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC129:
	.xword	.L700
	.align	3
.LC130:
	.xword	g_event_report
	.align	3
.LC132:
	.xword	.LC131
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Vop
	.type	Mp4_Syntax_Vop, %function
Mp4_Syntax_Vop:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	w21, w2
	mov	x22, x1
	bl	_mcount
	ldr	w2, [x20, 15636]
	cbnz	w2, .L828
	add	x19, x20, 20480
	ldrb	w0, [x19, 829]
	cbnz	w0, .L745
.L829:
	strb	wzr, [x19, 1601]
	mov	w0, 32
.L746:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L828:
	ldr	x1, .LC133
	mov	w0, 1
	add	x19, x20, 20480
	bl	dprint_vfmw
	str	wzr, [x20, 15636]
	ldrb	w0, [x19, 829]
	cbz	w0, .L829
.L745:
	mov	x1, x22
	mov	w2, w21
	add	x0, x29, 120
	add	x22, x19, 768
	bl	BsInit
	ldr	x23, .LC134
	mov	x2, 60
	mov	w1, 0
	mov	x0, x22
	ldr	x3, [x23, 96]
	blr	x3
	strb	wzr, [x19, 1601]
	mov	w1, 32
	add	x0, x29, 120
	bl	BsSkip
	mov	w1, 2
	add	x0, x29, 120
	bl	BsGet
	strb	w0, [x19, 768]
	ldr	x1, .LC136
	uxtb	w2, w0
	mov	w0, 18
	bl	dprint_vfmw
	ldrb	w0, [x19, 768]
	cmp	w0, 3
	beq	.L830
.L747:
	ldrb	w1, [x19, 835]
	cmp	w0, wzr
	cset	w0, ne
	cmp	w1, 1
	cset	w2, eq
	tst	w0, w2
	bne	.L750
	cmp	w1, 2
	beq	.L750
	ldr	w1, [x19, 2208]
	cmp	w1, 1
	cset	w1, eq
	tst	w0, w1
	bne	.L750
	lsl	w21, w21, 3
	mov	w24, 0
	mov	w25, 6400
	b	.L753
	.p2align 3
.L832:
	add	x0, x29, 120
	add	w24, w24, 1
	bl	BsPos
	cmp	w0, w21
	bgt	.L831
	cmp	w24, w25
	beq	.L754
.L753:
	mov	w1, 1
	add	x0, x29, 120
	bl	BsGet
	cbnz	w0, .L832
	mov	w0, 6399
	cmp	w24, w0
	bhi	.L754
	mov	w1, 1
	add	x0, x29, 120
	bl	BsGet
	cbz	w0, .L827
	ldr	w1, [x19, 936]
	mov	w0, 0
	cbnz	w1, .L833
.L757:
	str	w0, [x19, 1544]
	mov	w1, 1
	str	w24, [x19, 1540]
	add	x0, x29, 120
	bl	BsGet
	cbz	w0, .L827
	mov	w1, 1
	add	x0, x29, 120
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 769]
	cbz	w0, .L834
	ldrb	w0, [x19, 768]
	cmp	w0, 1
	beq	.L760
	cmp	w0, 3
	beq	.L835
.L761:
	strb	wzr, [x19, 770]
.L762:
	ldrb	w0, [x19, 876]
	add	x24, x20, 16384
	strb	wzr, [x19, 771]
	cmp	w0, 2
	beq	.L763
	mov	w1, 3
	add	x0, x29, 120
	add	x24, x20, 16384
	bl	BsGet
	strb	w0, [x19, 772]
	ldrb	w0, [x24, 3666]
	cbnz	w0, .L836
	mov	w0, 2
	strb	wzr, [x19, 774]
	strb	w0, [x19, 773]
.L765:
	ldrb	w0, [x19, 880]
	sub	w0, w0, #1
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L766
.L769:
	mov	w1, 5
	add	x0, x29, 120
	bl	BsGet
	uxtb	w0, w0
	cbnz	w0, .L824
	mov	w0, 1
.L824:
	strb	w0, [x19, 775]
	ldrb	w0, [x19, 768]
	cbnz	w0, .L837
.L763:
	mov	w0, 1
	strb	w0, [x19, 1601]
	ldr	x4, [x23, 104]
	mov	x1, x22
	ldr	w3, [x24, 3624]
	mov	x2, 60
	add	x0, x19, 1472
	add	x23, x20, 12288
	orr	w3, w3, 2
	str	w3, [x24, 3624]
	ldr	w3, [x20, 15636]
	add	w3, w3, 1
	str	w3, [x20, 15636]
	blr	x4
	add	x0, x29, 120
	bl	BsPos
	mov	w22, w0
	asr	w0, w0, 31
	sub	w21, w21, w22
	lsr	w0, w0, 29
	str	w21, [x20, 1308]
	add	w1, w22, w0
	add	w2, w22, 7
	ldr	w3, [x19, 1624]
	and	w1, w1, 7
	sub	w0, w1, w0
	cmp	w22, wzr
	add	w0, w3, w0
	strb	w0, [x20, 1306]
	csel	w0, w2, w22, lt
	ldr	w1, [x19, 1616]
	add	w0, w1, w0, asr 3
	str	w0, [x20, 1316]
	ldrb	w0, [x23, 3465]
	cbnz	w0, .L777
	ldrb	w0, [x23, 3545]
	cmp	w0, 1
	beq	.L838
.L777:
	strb	wzr, [x20, 1307]
	str	wzr, [x20, 1312]
	str	wzr, [x20, 1320]
.L778:
	ldrb	w0, [x19, 768]
	strb	w0, [x20, 1304]
	ldrb	w0, [x19, 777]
	strb	w0, [x20, 1302]
	ldrb	w0, [x19, 776]
	strb	w0, [x20, 1301]
	ldrb	w0, [x19, 775]
	strb	w0, [x20, 1305]
	ldrb	w0, [x19, 772]
	strb	w0, [x20, 1303]
	ldr	w2, [x24, 3620]
	cbnz	w2, .L839
.L779:
	mov	x0, x20
	bl	MP4_Record_SlcSegID
	mov	w0, w22
	b	.L746
	.p2align 3
.L754:
	strb	wzr, [x19, 1601]
	add	x0, x29, 120
	bl	BsPos
	mov	w19, w0
	ldr	x1, .LC144
	mov	w2, w24
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L746
.L835:
	ldrb	w0, [x19, 880]
	cmp	w0, 2
	bne	.L761
.L760:
	mov	w1, 1
	add	x0, x29, 120
	bl	BsGet
	strb	w0, [x19, 770]
	b	.L762
	.p2align 3
.L831:
	add	x0, x29, 120
	bl	BsPos
	mov	w19, w0
	ldr	x1, .LC142
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L746
	.p2align 3
.L830:
	ldr	x1, .LC137
	ldr	w1, [x1]
	tbnz	x1, 3, .L747
	strb	wzr, [x19, 1601]
	add	x0, x29, 120
	bl	BsPos
	mov	w20, w0
	ldr	x0, .LC138
	ldr	x4, [x0]
	cbz	x4, .L748
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
.L748:
	ldr	x1, .LC140
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w20
	b	.L746
	.p2align 3
.L750:
	add	x0, x29, 120
	bl	BsPos
	b	.L746
	.p2align 3
.L827:
	ldr	x1, .LC145
	mov	w0, 1
	bl	dprint_vfmw
	strb	wzr, [x19, 1601]
	add	x0, x29, 120
	bl	BsPos
	mov	w20, w0
	ldr	x0, .LC138
	ldr	x4, [x0]
	cbz	x4, .L785
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
	mov	w0, w20
	b	.L746
.L833:
	add	x0, x29, 120
	bl	BsGet
	b	.L757
.L785:
	mov	w0, w20
	b	.L746
.L839:
	ldr	x1, .LC150
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w25, [x19, 2824]
	ldr	w0, [x24, 3620]
	cmp	w0, wzr
	ble	.L782
	sub	w21, w0, #1
	add	x19, x23, 3620
	add	x21, x21, 3978
	add	x21, x20, x21, lsl 2
	.p2align 2
.L781:
	ldr	w1, [x19], 4
	mov	w0, w25
	bl	SM_ReleaseStreamSeg
	cmp	x21, x19
	bne	.L781
.L782:
	str	wzr, [x24, 3620]
	b	.L779
.L834:
	add	x20, x20, 16384
	mov	w0, 4
	mov	w1, 1
	strb	w0, [x19, 768]
	strb	w0, [x19, 1472]
	add	x0, x29, 120
	strb	w1, [x19, 1601]
	ldr	w1, [x20, 3624]
	orr	w1, w1, 128
	str	w1, [x20, 3624]
	bl	BsPos
	b	.L746
.L836:
	mov	w1, 1
	add	x0, x29, 120
	bl	BsGet
	strb	w0, [x19, 773]
	mov	w1, 1
	add	x0, x29, 120
	bl	BsGet
	strb	w0, [x19, 774]
	b	.L765
.L837:
	mov	w1, 3
	add	x0, x29, 120
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 776]
	cbz	w0, .L840
	ldrb	w0, [x19, 768]
	cmp	w0, 2
	bne	.L763
	mov	w1, 3
	add	x0, x29, 120
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 777]
	cbnz	w0, .L763
	strb	wzr, [x19, 1601]
	add	x0, x29, 120
	bl	BsPos
	mov	w20, w0
	ldr	x0, .LC138
	ldr	x4, [x0]
	cbz	x4, .L776
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
.L776:
	ldr	x1, .LC149
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w20
	b	.L746
.L766:
	ldrb	w0, [x19, 768]
	cmp	w0, 3
	bne	.L769
	ldrb	w0, [x19, 892]
	cbz	w0, .L769
	mov	x28, 500
	mov	w25, 0
	movk	x28, 0x19d, lsl 32
	b	.L773
.L770:
	ldr	x0, [x20, 22056]
	mov	w1, 1
	cmp	x0, x28
	beq	.L771
	add	x0, x29, 120
	bl	BsSkip
.L771:
	add	x0, x29, 120
	bl	bs_get_spritetrajectory
	mov	w2, w0
	cbnz	w0, .L841
.L772:
	mov	w1, 1
	add	x0, x29, 120
	bl	BsSkip
	add	x0, x20, x25, uxtw 3
	add	w25, w25, 1
	add	x0, x0, 20480
	str	w27, [x0, 804]
	str	w26, [x0, 808]
	ldrb	w0, [x19, 892]
	cmp	w0, w25
	bls	.L769
.L773:
	add	x0, x29, 120
	mov	w27, 0
	mov	w26, 0
	bl	bs_get_spritetrajectory
	mov	w2, w0
	cbz	w0, .L770
	mov	w1, w0
	add	x0, x29, 120
	str	w2, [x29, 108]
	bl	BsGet
	mov	w27, w0
	ldr	w2, [x29, 108]
	mov	w0, 1
	lsl	w1, w0, w2
	sub	w0, w2, #1
	asr	w0, w27, w0
	sub	w1, w1, #1
	eor	w1, w1, w27
	cbnz	w0, .L770
	neg	w27, w1
	b	.L770
.L841:
	mov	w1, w0
	add	x0, x29, 120
	str	w2, [x29, 108]
	bl	BsGet
	mov	w26, w0
	ldr	w2, [x29, 108]
	mov	w0, 1
	lsl	w1, w0, w2
	sub	w0, w2, #1
	asr	w0, w26, w0
	sub	w1, w1, #1
	eor	w1, w1, w26
	cbnz	w0, .L772
	neg	w26, w1
	b	.L772
.L838:
	ldr	w0, [x20, 15840]
	ldr	w1, [x20, 15836]
	lsl	w0, w0, 3
	strb	wzr, [x20, 1307]
	str	w0, [x20, 1312]
	str	w1, [x20, 1320]
	b	.L778
.L840:
	strb	wzr, [x19, 1601]
	add	x0, x29, 120
	bl	BsPos
	mov	w20, w0
	ldr	x0, .LC138
	ldr	x4, [x0]
	cbz	x4, .L775
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
.L775:
	ldrb	w2, [x19, 768]
	mov	w0, 1
	ldr	x1, .LC147
	bl	dprint_vfmw
	mov	w0, w20
	b	.L746
	.size	Mp4_Syntax_Vop, .-Mp4_Syntax_Vop
	.align	3
.LC133:
	.xword	.LC126
	.align	3
.LC134:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC136:
	.xword	.LC135
	.align	3
.LC137:
	.xword	g_VdmCharacter
	.align	3
.LC138:
	.xword	g_event_report
	.align	3
.LC140:
	.xword	.LC139
	.align	3
.LC142:
	.xword	.LC141
	.align	3
.LC144:
	.xword	.LC143
	.align	3
.LC145:
	.xword	.LC36
	.align	3
.LC147:
	.xword	.LC146
	.align	3
.LC149:
	.xword	.LC148
	.align	3
.LC150:
	.xword	.LC131
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Slice_NotSH
	.type	Mp4_Syntax_Slice_NotSH, %function
Mp4_Syntax_Slice_NotSH:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	mov	w23, w2
	bl	_mcount
	ldr	w19, [x20, 15636]
	sub	w0, w19, #1
	cmp	w0, 510
	bls	.L843
	add	x21, x20, 20480
	add	x19, x20, 12288
	add	x22, x20, 16384
	add	x1, x19, 3456
	ldr	w0, [x21, 2824]
	bl	MP4_ClearCurPacket
	ldr	w23, [x21, 2824]
	ldr	w0, [x22, 3620]
	cmp	w0, wzr
	ble	.L847
	sub	w21, w0, #1
	add	x19, x19, 3620
	add	x21, x21, 3978
	add	x21, x20, x21, lsl 2
	.p2align 2
.L846:
	ldr	w1, [x19], 4
	mov	w0, w23
	bl	SM_ReleaseStreamSeg
	cmp	x19, x21
	bne	.L846
.L847:
	ldr	w1, [x22, 3624]
	mov	w0, -435
	str	wzr, [x22, 3620]
	and	w0, w1, w0
	str	w0, [x22, 3624]
	ldr	x2, .LC151
	mov	w0, 1
	ldr	x1, .LC153
	ldr	w3, [x20, 15636]
	add	x2, x2, 1296
	bl	dprint_vfmw
	mov	w0, 0
.L866:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L843:
	add	x25, x20, 12288
	add	w0, w19, 1
	str	w0, [x20, 15636]
	mov	x1, x21
	mov	w2, w23
	add	x0, x29, 88
	ldrh	w21, [x25, 3616]
	add	x24, x20, 20480
	bl	BsInit
	ubfx	x21, x21, 7, 4
	add	x0, x29, 88
	add	w21, w21, 17
	mov	w1, w21
	bl	BsSkip
	ldr	w21, [x24, 1532]
	ldr	w0, [x24, 1536]
	add	w21, w21, 15
	add	w0, w0, 15
	lsr	w21, w21, 4
	lsr	w0, w0, 4
	mul	w21, w0, w21
	cbz	w21, .L848
	mov	w0, w21
	mov	w1, 0
	.p2align 2
.L849:
	add	w1, w1, 1
	lsr	w0, w0, 1
	sxth	w1, w1
	cbnz	w0, .L849
	add	x0, x29, 88
	uxtw	x19, w19
	bl	BsGet
	lsl	x1, x19, 3
	sub	w3, w21, #1
	sub	x1, x1, x19
	cmp	w0, w3
	add	x1, x20, x1, lsl 2
	str	w0, [x1, 1324]
	bhi	.L867
.L850:
	lsl	x21, x19, 3
	mov	w1, 5
	sub	x21, x21, x19
	add	x0, x29, 88
	add	x21, x20, x21, lsl 2
	bl	BsGet
	mov	w1, 1
	strb	w0, [x21, 1305]
	add	x0, x29, 88
	bl	BsGet
	cbnz	w0, .L861
	ldrb	w0, [x24, 768]
	add	x22, x20, 16384
	strb	w0, [x21, 1304]
	ldrb	w0, [x24, 772]
	strb	w0, [x21, 1303]
.L855:
	lsl	x21, x19, 3
	ldrb	w1, [x24, 776]
	sub	x21, x21, x19
	add	x0, x29, 88
	lsl	w23, w23, 3
	add	x21, x20, x21, lsl 2
	strb	w1, [x21, 1301]
	ldrb	w1, [x24, 777]
	strb	w1, [x21, 1302]
	ldr	w1, [x22, 3624]
	orr	w1, w1, 16
	str	w1, [x22, 3624]
	bl	BsPos
	mov	w22, w0
	asr	w0, w0, 31
	sub	w23, w23, w22
	lsr	w0, w0, 29
	str	w23, [x21, 1308]
	add	w1, w22, w0
	add	w2, w22, 7
	ldr	w3, [x24, 1624]
	and	w1, w1, 7
	sub	w0, w1, w0
	cmp	w22, wzr
	add	w0, w3, w0
	strb	w0, [x21, 1306]
	csel	w0, w2, w22, lt
	ldr	w1, [x24, 1616]
	add	w0, w1, w0, asr 3
	str	w0, [x21, 1316]
	ldrb	w0, [x25, 3465]
	cbnz	w0, .L857
	ldrb	w0, [x25, 3545]
	cmp	w0, 1
	beq	.L868
.L857:
	lsl	x0, x19, 3
	sub	x19, x0, x19
	add	x19, x20, x19, lsl 2
	strb	wzr, [x19, 1307]
	str	wzr, [x19, 1312]
	str	wzr, [x19, 1320]
.L858:
	mov	x0, x20
	bl	MP4_Record_SlcSegID
	ldr	x25, [sp, 64]
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L861:
	mov	w1, 1
	add	x0, x29, 88
	bl	BsGet
	cbnz	w0, .L861
	mov	w1, 1
	add	x22, x20, 16384
	add	x0, x29, 88
	bl	BsSkip
	ldr	w1, [x22, 3724]
	cbnz	w1, .L869
.L853:
	mov	w1, 1
	add	x0, x29, 88
	lsl	x21, x19, 3
	sub	x21, x21, x19
	bl	BsSkip
	mov	w1, 2
	add	x0, x29, 88
	add	x21, x20, x21, lsl 2
	bl	BsGet
	mov	w1, 3
	strb	w0, [x21, 1304]
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x21, 1303]
	ldrb	w1, [x21, 1304]
	cbz	w1, .L855
	mov	w1, 3
	add	x0, x29, 88
	bl	BsSkip
	ldrb	w0, [x21, 1304]
	cmp	w0, 2
	bne	.L855
	mov	w1, 3
	add	x0, x29, 88
	bl	BsSkip
	b	.L855
	.p2align 3
.L869:
	add	x0, x29, 88
	bl	BsSkip
	b	.L853
	.p2align 3
.L868:
	ldr	w0, [x20, 15840]
	strb	wzr, [x21, 1307]
	lsl	w0, w0, 3
	str	w0, [x21, 1312]
	ldr	w0, [x20, 15836]
	str	w0, [x21, 1320]
	b	.L858
.L848:
	mov	w1, 0
	add	x0, x29, 88
	uxtw	x19, w19
	bl	BsGet
	lsl	x1, x19, 3
	sub	x1, x1, x19
	add	x1, x20, x1, lsl 2
	str	w0, [x1, 1324]
	b	.L850
.L867:
	ldr	x1, .LC155
	mov	w2, w0
	mov	w3, w21
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L866
	.size	Mp4_Syntax_Slice_NotSH, .-Mp4_Syntax_Slice_NotSH
	.align	3
.LC151:
	.xword	.LANCHOR1
	.align	3
.LC153:
	.xword	.LC152
	.align	3
.LC155:
	.xword	.LC154
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Slice_SH
	.type	Mp4_Syntax_Slice_SH, %function
Mp4_Syntax_Slice_SH:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	mov	w22, w2
	bl	_mcount
	ldr	w3, [x20, 15636]
	sub	w26, w3, #1
	cmp	w26, 510
	bls	.L871
	add	x21, x20, 20480
	add	x19, x20, 12288
	add	x23, x20, 16384
	add	x1, x19, 3456
	ldr	w0, [x21, 2824]
	bl	MP4_ClearCurPacket
	ldr	w22, [x21, 2824]
	ldr	w0, [x23, 3620]
	cmp	w0, wzr
	ble	.L875
	sub	w21, w0, #1
	add	x19, x19, 3620
	add	x21, x21, 3978
	add	x21, x20, x21, lsl 2
	.p2align 2
.L874:
	ldr	w1, [x19], 4
	mov	w0, w22
	bl	SM_ReleaseStreamSeg
	cmp	x19, x21
	bne	.L874
.L875:
	ldr	w1, [x23, 3624]
	mov	w0, -435
	str	wzr, [x23, 3620]
	and	w0, w1, w0
	str	w0, [x23, 3624]
	ldr	x2, .LC156
	mov	w0, 1
	ldr	x1, .LC157
	ldr	w3, [x20, 15636]
	add	x2, x2, 1320
	bl	dprint_vfmw
	mov	w0, 0
.L873:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L871:
	add	x24, x20, 12288
	add	w0, w3, 1
	str	w0, [x20, 15636]
	mov	x1, x19
	mov	w2, w22
	add	x0, x29, 88
	ldrb	w19, [x24, 3616]
	uxtw	x25, w3
	bl	BsInit
	add	x23, x20, 20480
	ubfx	x19, x19, 3, 4
	add	x0, x29, 88
	add	w19, w19, 17
	mov	w1, w19
	lsl	x19, x25, 3
	sub	x19, x19, x25
	bl	BsSkip
	mov	w1, 5
	add	x0, x29, 88
	add	x19, x20, x19, lsl 2
	bl	BsGet
	ldrb	w2, [x23, 780]
	mul	w0, w2, w0
	str	w0, [x19, 1324]
	ldrh	w1, [x20, 1226]
	ldrh	w3, [x20, 1224]
	mul	w3, w1, w3
	sub	w3, w3, #1
	cmp	w0, w3
	bhi	.L888
	mov	w1, 2
	add	x0, x29, 88
	lsl	w22, w22, 3
	bl	BsSkip
	mov	w1, 5
	add	x0, x29, 88
	bl	BsGet
	strb	w0, [x19, 1305]
	add	x0, x29, 88
	ldrb	w1, [x23, 768]
	strb	w1, [x19, 1304]
	ldrb	w1, [x23, 772]
	strb	w1, [x19, 1303]
	ldrb	w1, [x23, 776]
	strb	w1, [x19, 1301]
	ldrb	w1, [x23, 777]
	strb	w1, [x19, 1302]
	bl	BsPos
	mov	w21, w0
	asr	w0, w0, 31
	sub	w22, w22, w21
	lsr	w0, w0, 29
	str	w22, [x19, 1308]
	add	w1, w21, w0
	add	w2, w21, 7
	ldr	w3, [x23, 1624]
	and	w1, w1, 7
	sub	w0, w1, w0
	cmp	w21, wzr
	add	w0, w3, w0
	strb	w0, [x19, 1306]
	csel	w0, w2, w21, lt
	add	x1, x20, 16384
	ldr	w2, [x23, 1616]
	add	w0, w2, w0, asr 3
	str	w0, [x19, 1316]
	ldr	w0, [x1, 3624]
	orr	w0, w0, 16
	str	w0, [x1, 3624]
	ldrb	w0, [x24, 3465]
	cbnz	w0, .L877
	ldrb	w0, [x24, 3545]
	cmp	w0, 1
	beq	.L889
.L877:
	lsl	x0, x25, 3
	sub	x0, x0, x25
	add	x0, x20, x0, lsl 2
	strb	wzr, [x0, 1307]
	str	wzr, [x0, 1312]
	str	wzr, [x0, 1320]
.L878:
	uxtw	x0, w26
	lsl	x1, x0, 3
	sub	x1, x1, x0
	add	x1, x20, x1, lsl 2
	ldr	w2, [x1, 1320]
	cbz	w2, .L879
	ldr	w2, [x1, 1312]
	cbnz	w2, .L890
.L879:
	lsl	x1, x0, 3
	ldrb	w2, [x24, 3616]
	sub	x0, x1, x0
	ubfx	x1, x2, 3, 4
	add	x0, x20, x0, lsl 2
	ldr	w2, [x0, 1308]
	add	w1, w2, w1
	str	w1, [x0, 1308]
.L880:
	mov	x0, x20
	bl	MP4_Record_SlcSegID
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L890:
	ldrb	w0, [x24, 3616]
	ubfx	x0, x0, 3, 4
	add	w2, w0, w2
	str	w2, [x1, 1312]
	b	.L880
	.p2align 3
.L889:
	ldr	w0, [x20, 15840]
	strb	wzr, [x19, 1307]
	lsl	w0, w0, 3
	str	w0, [x19, 1312]
	ldr	w0, [x20, 15836]
	str	w0, [x19, 1320]
	b	.L878
.L888:
	ldr	x1, .LC159
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L873
	.size	Mp4_Syntax_Slice_SH, .-Mp4_Syntax_Slice_SH
	.align	3
.LC156:
	.xword	.LANCHOR1
	.align	3
.LC157:
	.xword	.LC152
	.align	3
.LC159:
	.xword	.LC158
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Dec_Slice
	.type	MP4_Dec_Slice, %function
MP4_Dec_Slice:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 12288
	mov	x22, x1
	mov	w21, w2
	bl	_mcount
	ldrb	w0, [x20, 3616]
	tbz	x0, 0, .L901
	tbz	x0, 2, .L902
.L897:
	mov	w2, w21
	mov	x1, x22
	mov	x0, x19
	bl	Mp4_Syntax_Slice_SH
	cmn	w0, #1
	beq	.L899
.L895:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L902:
	ldrh	w1, [x20, 3616]
	ubfx	x1, x1, 7, 4
	sub	w2, w1, #1
	cmp	w2, 6
	bhi	.L897
	bfi	w0, w1, 3, 4
	mov	w2, w21
	strb	w0, [x20, 3616]
	mov	x1, x22
	mov	x0, x19
	bl	Mp4_Syntax_Slice_SH
	cmn	w0, #1
	bne	.L895
.L899:
	add	x19, x19, 20480
	add	x1, x20, 3456
	ldr	w0, [x19, 2824]
	bl	MP4_ClearCurPacket
	ldr	x2, .LC160
	mov	w0, 1
	ldr	x1, .LC162
	add	x2, x2, 1344
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L901:
	mov	x0, x19
	bl	Mp4_Check_Slice_NotSH
	cbz	w0, .L903
	mov	w1, w21
	mov	x0, x19
	bl	Mp4_Combine_SliceData
	cbnz	w0, .L899
	mov	x0, x19
	bl	MP4_Record_SlcSegID
	b	.L895
	.p2align 3
.L903:
	mov	w2, w21
	mov	x1, x22
	mov	x0, x19
	bl	Mp4_Syntax_Slice_NotSH
	cmn	w0, #1
	beq	.L899
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MP4_Dec_Slice, .-MP4_Dec_Slice
	.align	3
.LC160:
	.xword	.LANCHOR1
	.align	3
.LC162:
	.xword	.LC161
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Record_PacketID
	.type	MP4_Record_PacketID, %function
MP4_Record_PacketID:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 20480
	bl	_mcount
	ldr	w1, [x20, 2100]
	cmp	w1, 62
	bgt	.L907
	add	x0, x19, x1, sxtw 2
	ldr	w4, [x19, 15764]
	add	x0, x0, 20480
	add	x3, x19, 12288
	add	w2, w1, 1
	str	w4, [x0, 1844]
	str	w2, [x20, 2100]
	ldrb	w2, [x3, 3465]
	cbnz	w2, .L904
	ldrb	w2, [x3, 3545]
	cmp	w2, 1
	beq	.L908
.L904:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L907:
	mov	x0, x19
	bl	MP4_Clean_HeadPacket
	ldr	w0, [x20, 2824]
	add	x1, x19, 12288
	add	x1, x1, 3456
	bl	MP4_ClearCurPacket
	ldr	w2, [x20, 2100]
	mov	w3, 62
	ldr	x1, .LC163
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L908:
	ldr	w2, [x19, 15844]
	add	w1, w1, 2
	str	w2, [x0, 1848]
	str	w1, [x20, 2100]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MP4_Record_PacketID, .-MP4_Record_PacketID
	.align	3
.LC163:
	.xword	.LC124
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Record_HeadPacketInfo
	.type	MP4_Record_HeadPacketInfo, %function
MP4_Record_HeadPacketInfo:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	w23, w1
	bl	_mcount
	cbz	x21, .L913
	ldr	x22, .LC166
	add	x19, x21, 20480
	add	x20, x21, 12288
	mov	x2, 160
	add	x1, x20, 3456
	add	x0, x19, 1680
	ldr	x3, [x22, 104]
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 4
	add	x1, x20, 3616
	add	x0, x19, 1840
	blr	x3
	str	w23, [x19, 2104]
	ldr	w0, [x19, 2108]
	cbz	w0, .L914
.L912:
	ldr	w0, [x19, 2112]
	add	w0, w0, 1
	str	w0, [x19, 2112]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L914:
	mov	x0, x21
	bl	MP4_Record_PacketID
	mov	w0, 1
	str	w0, [x19, 2108]
	b	.L912
	.p2align 3
.L913:
	ldr	x1, .LC165
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	MP4_Record_HeadPacketInfo, .-MP4_Record_HeadPacketInfo
	.align	3
.LC165:
	.xword	.LC164
	.align	3
.LC166:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Mp4_Syntax_Parse
	.type	Mp4_Syntax_Parse, %function
Mp4_Syntax_Parse:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x22, x0
	mov	x0, x30
	mov	x24, x1
	mov	w25, w2
	bl	_mcount
	ldr	x26, .LC167
	mov	x2, 40
	mov	w1, 0
	add	x0, x29, 112
	add	x23, x22, 16384
	ldr	x3, [x26, 96]
	blr	x3
	mov	w2, w25
	mov	x1, x24
	add	x0, x29, 112
	bl	BsInit
	mov	w1, 32
	add	x0, x29, 112
	bl	BsShow
	str	wzr, [x23, 3628]
	cmp	w0, 510
	mov	w20, w0
	beq	.L1048
	add	x27, x22, 12288
	ldrb	w0, [x27, 3616]
	tbz	x0, 0, .L1049
.L921:
	add	x19, x22, 20480
	mov	w1, 1
	mov	w20, 196
	strb	w1, [x19, 837]
.L922:
	tbnz	x0, 1, .L923
	ldr	w0, [x23, 3624]
	mov	w21, 130
	ands	w21, w0, w21
	bne	.L1050
	cmp	w20, 197
	beq	.L1051
	cmp	w20, 432
	beq	.L1052
	cmp	w20, 437
	beq	.L1053
	and	w0, w20, -16
	cmp	w0, 288
	beq	.L1054
	cmp	w20, 435
	beq	.L1055
	cmp	w20, 438
	beq	.L1056
	cmp	w20, 196
	beq	.L1057
	cmp	w20, 434
	beq	.L1058
	cmp	w20, 286
	bne	.L938
	ldr	x28, .LC168
	ldr	x4, [x28]
	cbz	x4, .L939
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L939:
	ldr	w0, [x19, 2824]
	add	x1, x27, 3456
	add	x21, x24, 4
	sub	w25, w25, #4
	bl	MP4_ClearCurPacket
	ldr	w0, [x23, 3624]
	tbz	x0, 1, .L940
	orr	w0, w0, 256
	str	w0, [x23, 3624]
.L940:
	ldr	w1, [x23, 3624]
	mov	w0, 18
	and	w0, w1, w0
	cmp	w0, 18
	beq	.L941
	ldr	x4, [x28]
	cbz	x4, .L941
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L941:
	ldrb	w0, [x19, 2861]
	mov	w28, 0
	cmp	w0, 1
	beq	.L972
.L947:
	ldrb	w0, [x19, 1599]
	mov	w21, w28
	cmp	w0, 1
	bne	.L1022
	ldr	w0, [x19, 2120]
	mov	x2, 604
	ldr	x3, [x26, 104]
	add	x1, x23, 3656
	add	w0, w0, 1
	str	wzr, [x19, 2124]
	str	w0, [x19, 2120]
	add	x0, x19, 868
	blr	x3
	ldr	w3, [x19, 1532]
	ldr	w1, [x23, 3740]
	ldr	w2, [x23, 3736]
	cmp	w3, w1
	beq	.L1059
.L963:
	ldr	x0, .LC168
	ldr	x4, [x0]
	cbz	x4, .L965
	ldr	w5, [x19, 1536]
	ldr	w0, [x19, 2824]
	strh	w2, [x29, 108]
	add	x2, x29, 104
	strh	w1, [x29, 110]
	mov	w1, 2
	strh	w3, [x29, 106]
	mov	w3, 8
	strh	w5, [x29, 104]
	blr	x4
	ldr	w2, [x23, 3736]
	ldr	w1, [x23, 3740]
.L965:
	str	w1, [x19, 1532]
	str	w2, [x19, 1536]
.L966:
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L968
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 568]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 576]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x20, 584]
	bl	FreeUsdByDec
	add	x0, x20, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2836]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 2836]
	ldr	w0, [x19, 2824]
	str	wzr, [x19, 2844]
	bl	FSP_GetNullLogicFs
	str	w0, [x19, 2836]
	str	w0, [x19, 2844]
.L968:
	add	x20, x19, 2216
	ldr	w0, [x19, 2824]
	mov	x1, x20
	str	wzr, [x19, 2832]
	bl	FSP_ClearNotInVoQueue
	ldr	x0, [x22, 22112]
	ldr	w1, [x0, 708]
	cbnz	w1, .L969
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L1060
.L969:
	ldr	w2, [x19, 1532]
	mov	x0, x22
	ldr	w1, [x19, 1536]
	bl	Frame_Init
	cbz	w0, .L967
	ldr	x1, .LC194
	mov	w0, 0
	bl	dprint_vfmw
	b	.L967
	.p2align 3
.L1050:
	orr	w0, w0, 32
	mov	w1, 1
	mov	w21, 0
	str	w0, [x23, 3624]
	str	w1, [x23, 3628]
.L1022:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L923:
	mov	w2, w25
	mov	x1, x24
	mov	x0, x22
	bl	MP4_Dec_Slice
	mov	w28, w0
.L927:
	ldrb	w0, [x19, 2861]
	mov	x21, x24
	cmp	w0, 1
	beq	.L972
	cmp	w28, w25, lsl 3
	bhi	.L946
.L1065:
	mov	w0, 196
	cmp	w20, 438
	ccmp	w20, w0, 4, ne
	bne	.L947
	ldr	w1, [x23, 3624]
	mov	w0, 130
	tst	w1, w0
	beq	.L947
	ldrb	w0, [x19, 837]
	cmp	w0, 2
	beq	.L1061
	ldr	w1, [x19, 1532]
	ldr	w0, [x19, 1536]
	str	w0, [x19, 2148]
	str	w1, [x19, 2152]
	cbz	w0, .L951
	ldr	w2, [x19, 2140]
	mov	w20, 1
	cmp	w0, w2
	beq	.L951
.L952:
	ldr	x2, .LC168
	ldr	x4, [x2]
	cbz	x4, .L956
	strh	w0, [x29, 108]
	mov	w3, 8
	ldr	w0, [x19, 2140]
	add	x2, x29, 104
	strh	w0, [x29, 104]
	ldr	w0, [x19, 2144]
	strh	w0, [x29, 106]
	ldr	w0, [x19, 2824]
	strh	w1, [x29, 110]
	mov	w1, 2
	blr	x4
.L956:
	cbnz	w20, .L1062
.L958:
	ldr	w1, [x19, 2148]
	ldr	w0, [x19, 2152]
	str	w1, [x19, 2140]
	str	w0, [x19, 2144]
.L950:
	ldr	x3, [x26, 104]
	mov	x2, 60
	add	x1, x19, 768
	add	x0, x19, 1472
	sub	x21, x21, x24
	blr	x3
	ldr	x1, .LC192
	mov	w0, 2
	add	w21, w28, w21, lsl 3
	bl	dprint_vfmw
	ldr	w0, [x19, 2124]
	add	w0, w0, 1
	str	w0, [x19, 2124]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L1049:
	add	x19, x22, 20480
	mov	w1, 2
	strb	w1, [x19, 837]
	lsr	w1, w20, 8
	sub	w1, w1, #128
	cmp	w1, 3
	bhi	.L922
	ldrb	w0, [x27, 3616]
	orr	w0, w0, 2
	strb	w0, [x27, 3616]
	uxtb	w0, w0
	b	.L922
	.p2align 3
.L951:
	cbz	w1, .L953
	ldr	w2, [x19, 2144]
	cmp	w1, w2
	beq	.L1063
	mov	w20, 1
	cbz	w0, .L952
	ldr	w2, [x19, 2140]
	cmp	w0, w2
	bne	.L952
.L973:
	ldr	w2, [x19, 2144]
	cmp	w1, w2
	beq	.L956
	b	.L952
	.p2align 3
.L972:
	ldr	w1, [x23, 3736]
	cbz	w1, .L944
	ldr	w2, [x23, 3740]
	cbnz	w2, .L1064
.L944:
	strb	wzr, [x19, 2861]
.L1067:
	cmp	w28, w25, lsl 3
	bls	.L1065
.L946:
	ldr	x1, .LC196
	mov	w0, 2
	sub	x21, x21, x24
	lsl	w21, w21, 3
	bl	dprint_vfmw
	ldr	w1, [x23, 3624]
	mov	w0, -435
	and	w0, w1, w0
	str	w0, [x23, 3624]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L1048:
	ldr	x3, [x26, 104]
	mov	x2, 40
	add	x1, x29, 112
	add	x0, x29, 152
	blr	x3
	mov	w1, 32
	add	x0, x29, 152
	bl	BsSkip
	mov	w1, 32
	add	x0, x29, 152
	bl	BsGet
	mov	w19, w0
	mov	w1, 32
	add	x0, x29, 152
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w19, w2
	ccmp	w0, w1, 0, eq
	bne	.L917
	ldr	x1, .LC168
	add	x19, x22, 20480
	mov	w0, 1
	ldr	x4, [x1]
	strb	w0, [x19, 2860]
	cbz	x4, .L918
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 117
	blr	x4
.L918:
	ldr	w0, [x23, 3624]
	mov	w1, 130
	tst	w0, w1
	beq	.L919
	orr	w0, w0, 32
	mov	w1, 1
	str	w0, [x23, 3624]
	str	w1, [x23, 3628]
.L919:
	add	x1, x22, 12288
	ldr	w0, [x19, 2824]
	add	x1, x1, 3456
	mov	w21, 0
	bl	MP4_ClearCurPacket
	b	.L1022
	.p2align 3
.L917:
	add	x27, x22, 12288
	ldrb	w0, [x27, 3616]
	tbnz	x0, 0, .L921
	add	x19, x22, 20480
	mov	w1, 2
	strb	w1, [x19, 837]
	b	.L922
	.p2align 3
.L938:
	ldr	w0, [x19, 2824]
	add	x1, x27, 3456
	add	x21, x24, 4
	sub	w25, w25, #4
	bl	MP4_ClearCurPacket
	b	.L941
	.p2align 3
.L1052:
	ldr	x1, .LC172
	mov	w0, 2
	bl	dprint_vfmw
	str	wzr, [x19, 2156]
	mov	x1, x24
	mov	w2, w25
	mov	x0, x22
	bl	Mp4_Syntax_Vos
	mov	w28, w0
	mov	w1, w20
	mov	x0, x22
	bl	MP4_Record_HeadPacketInfo
	b	.L927
	.p2align 3
.L1056:
	ldr	x1, .LC180
	mov	w0, 2
	bl	dprint_vfmw
	cmp	w25, 4
	beq	.L1066
	mov	w2, w25
	mov	x1, x24
	mov	x0, x22
	bl	Mp4_Syntax_Vop
	mov	w28, w0
	ldr	w0, [x23, 3624]
	tbnz	x0, 1, .L934
	ldr	w0, [x19, 2824]
	add	x1, x27, 3456
	bl	MP4_ClearCurPacket
.L933:
	ldr	x1, .LC182
	mov	w2, w28
	mov	w0, 7
	bl	dprint_vfmw
	b	.L927
	.p2align 3
.L953:
	ldr	w2, [x19, 2828]
	cmp	w2, 1
	cset	w20, ne
	cbz	w0, .L956
.L974:
	ldr	w2, [x19, 2140]
	cmp	w0, w2
	bne	.L952
	cbnz	w1, .L973
	b	.L956
	.p2align 3
.L1064:
	mov	x0, x22
	bl	Frame_Init
	cbz	w0, .L944
	ldr	x1, .LC188
	mov	w0, 0
	bl	dprint_vfmw
	strb	wzr, [x19, 2861]
	b	.L1067
	.p2align 3
.L1059:
	ldr	w0, [x19, 1536]
	cmp	w0, w2
	bne	.L963
	ldr	w0, [x19, 2828]
	cmp	w0, 1
	bne	.L966
.L967:
	ldr	w0, [x19, 1532]
	mov	w21, w28
	ldr	w1, [x19, 1536]
	str	w1, [x19, 2140]
	str	w0, [x19, 2144]
	ldr	w0, [x23, 3740]
	str	w0, [x19, 1532]
	ldr	w0, [x23, 3736]
	str	w0, [x19, 1536]
	strb	wzr, [x19, 1599]
	b	.L1022
	.p2align 3
.L1053:
	ldr	x1, .LC174
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, 1
	mov	x1, x24
	str	w0, [x19, 2156]
	mov	w2, w25
	mov	x0, x22
	bl	Mp4_Syntax_Vo
	mov	w28, w0
	mov	w1, w20
	mov	x0, x22
	bl	MP4_Record_HeadPacketInfo
	b	.L927
	.p2align 3
.L1054:
	ldr	x1, .LC176
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, 2
	mov	w2, w25
	mov	x1, x24
	str	w0, [x19, 2156]
	mov	x0, x22
	bl	Mp4_Syntax_Vol
	mov	w28, w0
	mov	w1, w20
	mov	x0, x22
	bl	MP4_Record_HeadPacketInfo
	ldrb	w0, [x19, 1599]
	cmp	w0, 1
	beq	.L927
	ldrb	w21, [x19, 1600]
	cmp	w21, 1
	bne	.L927
	ldr	x3, [x26, 104]
	mov	x2, 604
	add	x1, x19, 164
	add	x0, x23, 3656
	blr	x3
	strb	w21, [x19, 1599]
	strb	w21, [x19, 829]
	b	.L927
	.p2align 3
.L1051:
	ldr	x1, .LC170
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w0, [x19, 2824]
	add	x1, x27, 3456
	bl	MP4_ClearCurPacket
	b	.L1022
	.p2align 3
.L1055:
	ldr	x1, .LC178
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, 3
	mov	x1, x24
	str	w0, [x19, 2156]
	mov	w2, w25
	mov	x0, x22
	bl	Mp4_Syntax_Gop
	mov	w28, w0
	mov	w1, w20
	mov	x0, x22
	bl	MP4_Record_HeadPacketInfo
	b	.L927
	.p2align 3
.L1061:
	ldrb	w0, [x19, 768]
	cmp	w0, 2
	beq	.L949
	ldr	w0, [x19, 1548]
	ldr	w1, [x19, 1540]
	str	w0, [x19, 1552]
	add	w1, w0, w1
	str	w1, [x19, 1548]
	ldr	w3, [x19, 1544]
	ldr	w0, [x23, 3728]
	ldr	w2, [x19, 1560]
	madd	w0, w1, w0, w3
	str	w0, [x19, 1556]
	str	w0, [x19, 1560]
	sub	w0, w0, w2
	str	w0, [x19, 1564]
	b	.L950
	.p2align 3
.L1057:
	ldr	x1, .LC184
	mov	w0, 2
	bl	dprint_vfmw
	mov	w2, w25
	mov	x1, x24
	mov	x0, x22
	bl	Mp4_Syntax_ShortHeader
	mov	w28, w0
	ldr	w0, [x23, 3624]
	tbz	x0, 1, .L1045
	mov	w1, 196
	mov	x0, x22
	bl	MP4_Record_HeadPacketInfo
	b	.L927
	.p2align 3
.L1058:
	ldr	x1, .LC186
	mov	w0, 2
	bl	dprint_vfmw
	mov	w2, w25
	mov	x1, x24
	mov	x0, x22
	bl	Mp4_Syntax_UserData
	mov	w28, w0
.L1045:
	ldr	w0, [x19, 2824]
	add	x1, x27, 3456
	bl	MP4_ClearCurPacket
	b	.L927
	.p2align 3
.L1062:
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L959
	ldr	w0, [x19, 2824]
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x23, 568]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x23, 576]
	bl	FreeUsdByDec
	ldr	w0, [x19, 2824]
	ldr	x1, [x23, 584]
	bl	FreeUsdByDec
	add	x1, x23, 512
	mov	w2, w20
	stp	xzr, xzr, [x1, 48]
	stp	xzr, xzr, [x1, 64]
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2844]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 2824]
	mov	w2, w20
	ldr	w1, [x19, 2836]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 2836]
	ldr	w0, [x19, 2824]
	str	wzr, [x19, 2844]
	bl	FSP_GetNullLogicFs
	str	w0, [x19, 2836]
	str	w0, [x19, 2844]
.L959:
	add	x20, x19, 2216
	ldr	w0, [x19, 2824]
	mov	x1, x20
	str	wzr, [x19, 2832]
	bl	FSP_ClearNotInVoQueue
	ldr	x0, [x22, 22112]
	ldr	w1, [x0, 708]
	cbnz	w1, .L960
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L1068
.L960:
	ldr	w1, [x19, 2148]
	mov	x0, x22
	ldr	w2, [x19, 2152]
	bl	Frame_Init
	cbz	w0, .L958
	ldr	x1, .LC190
	mov	w0, 0
	bl	dprint_vfmw
	b	.L958
.L934:
	mov	w1, 438
	mov	x0, x22
	bl	MP4_Record_HeadPacketInfo
	b	.L933
.L949:
	ldr	w2, [x19, 1552]
	ldr	w1, [x19, 1540]
	ldr	w0, [x23, 3728]
	ldr	w4, [x19, 1544]
	add	w1, w2, w1
	ldr	w3, [x19, 1560]
	ldr	w2, [x19, 1564]
	madd	w0, w1, w0, w4
	sub	w1, w2, w3
	str	w0, [x19, 1556]
	add	w0, w1, w0
	str	w0, [x19, 1568]
	b	.L950
.L1066:
	mov	w1, w20
	mov	x0, x22
	mov	w28, 0
	bl	MP4_Record_HeadPacketInfo
	b	.L933
.L1063:
	ldr	w2, [x19, 2828]
	cmp	w2, 1
	cset	w20, ne
	cbnz	w0, .L974
	cbnz	w1, .L973
	b	.L956
.L1060:
	ldr	w0, [x19, 2828]
	cbnz	w0, .L969
	mov	x0, x20
	bl	ResetVoQueue
	b	.L969
.L1068:
	ldr	w0, [x19, 2828]
	cbnz	w0, .L960
	mov	x0, x20
	bl	ResetVoQueue
	b	.L960
	.size	Mp4_Syntax_Parse, .-Mp4_Syntax_Parse
	.align	3
.LC167:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC168:
	.xword	g_event_report
	.align	3
.LC170:
	.xword	.LC169
	.align	3
.LC172:
	.xword	.LC171
	.align	3
.LC174:
	.xword	.LC173
	.align	3
.LC176:
	.xword	.LC175
	.align	3
.LC178:
	.xword	.LC177
	.align	3
.LC180:
	.xword	.LC179
	.align	3
.LC182:
	.xword	.LC181
	.align	3
.LC184:
	.xword	.LC183
	.align	3
.LC186:
	.xword	.LC185
	.align	3
.LC188:
	.xword	.LC187
	.align	3
.LC190:
	.xword	.LC189
	.align	3
.LC192:
	.xword	.LC191
	.align	3
.LC194:
	.xword	.LC193
	.align	3
.LC196:
	.xword	.LC195
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Combine_HdPacket
	.type	MP4_Combine_HdPacket, %function
MP4_Combine_HdPacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 22160]
	add	x0, x19, 20480
	add	x20, x0, 1680
	cbz	x1, .L1081
	add	x1, x19, 12288
	ldrb	w2, [x1, 3465]
	cmp	w2, 1
	beq	.L1082
	cbnz	w2, .L1077
	ldrb	w2, [x1, 3545]
	cmp	w2, 1
	bne	.L1077
	ldr	x4, .LC200
	mov	x2, 80
	ldr	w3, [x19, 15760]
	add	x1, x1, 3536
	ldr	w5, [x20, 16]
	add	x0, x0, 1760
	ldr	x4, [x4, 104]
	add	w3, w5, w3
	str	w3, [x20, 16]
	blr	x4
	mov	w0, 0
.L1072:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1082:
	ldrb	w21, [x20, 9]
	cmp	w21, 1
	beq	.L1083
	cbnz	w21, .L1076
	ldrb	w0, [x20, 89]
	cmp	w0, 1
	bne	.L1076
	ldr	w1, [x19, 15760]
	mov	w0, 0
	ldr	w2, [x20, 96]
	add	w1, w2, w1
	str	w1, [x20, 96]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1077:
	ldr	x1, .LC201
	mov	w2, 5377
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1083:
	ldr	w3, [x19, 15756]
	ldr	w2, [x20, 12]
	cmp	w3, w2
	bcc	.L1084
	ldr	w1, [x19, 15760]
	mov	w0, 0
	ldr	w2, [x20, 16]
	add	w1, w2, w1
	str	w1, [x20, 16]
	b	.L1072
	.p2align 3
.L1076:
	ldr	x1, .LC201
	mov	w2, 5363
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1072
	.p2align 3
.L1084:
	ldr	x3, .LC200
	mov	x2, 80
	add	x1, x1, 3456
	add	x0, x0, 1760
	ldr	x3, [x3, 104]
	blr	x3
	strb	wzr, [x20, 9]
	strb	w21, [x20, 89]
	mov	w0, 0
	b	.L1072
.L1081:
	ldr	x1, .LC197
	ldr	x4, [x1]
	cbz	x4, .L1071
	ldr	w0, [x0, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 103
	blr	x4
.L1071:
	ldr	x1, .LC199
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1072
	.size	MP4_Combine_HdPacket, .-MP4_Combine_HdPacket
	.align	3
.LC197:
	.xword	g_event_report
	.align	3
.LC199:
	.xword	.LC198
	.align	3
.LC200:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC201:
	.xword	.LC106
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Renew_CurPacket
	.type	MP4_Renew_CurPacket, %function
MP4_Renew_CurPacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x20
	bl	MP4_Combine_HdPacket
	cmn	w0, #1
	mov	w19, w0
	beq	.L1088
	mov	x0, x20
	add	x20, x20, 20480
	add	x19, x0, 12288
	bl	MP4_Record_PacketID
	ldr	x21, .LC204
	mov	x2, 160
	add	x1, x20, 1680
	add	x0, x19, 3456
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 104]
	mov	x2, 4
	add	x1, x20, 1840
	add	x0, x19, 3528
	blr	x3
	mov	w0, 0
.L1087:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1088:
	add	x0, x20, 20480
	add	x1, x20, 12288
	add	x1, x1, 3456
	ldr	w0, [x0, 2824]
	bl	MP4_ClearCurPacket
	ldr	x1, .LC203
	mov	w2, 5393
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L1087
	.size	MP4_Renew_CurPacket, .-MP4_Renew_CurPacket
	.align	3
.LC203:
	.xword	.LC202
	.align	3
.LC204:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MP4_Check_CurPacket
	.type	MP4_Check_CurPacket, %function
MP4_Check_CurPacket:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 12288
	add	x21, x19, 20480
	bl	_mcount
	add	x0, x29, 56
	ldr	w1, [x20, 3468]
	str	w1, [x21, 1616]
	ldr	x1, [x19, 15744]
	str	x1, [x19, 22088]
	ldr	w2, [x20, 3472]
	str	w2, [x21, 1620]
	str	wzr, [x21, 1624]
	bl	BsInit
	mov	w1, 32
	add	x0, x29, 56
	bl	BsShow
	ldrb	w1, [x20, 3616]
	tbz	x1, 0, .L1117
	mov	w0, 1
	strb	w0, [x21, 837]
	ldrb	w0, [x20, 3616]
	tbnz	x0, 1, .L1092
	ldr	w2, [x21, 2104]
	mov	w1, 196
.L1096:
	mov	w0, 438
	cmp	w1, w0
	ccmp	w2, w0, 0, eq
	beq	.L1115
	mov	w0, 196
	cmp	w1, w0
	ccmp	w2, w0, 0, eq
	beq	.L1115
.L1114:
	mov	w0, 0
	b	.L1100
	.p2align 3
.L1117:
	lsr	w1, w0, 8
	mov	w2, 2
	sub	w1, w1, #128
	strb	w2, [x21, 837]
	cmp	w1, 3
	mov	w1, w0
	bls	.L1118
.L1091:
	ldrb	w2, [x20, 3616]
	tbz	x2, 1, .L1119
	ldr	w2, [x21, 2104]
	cmp	w0, 196
	cset	w1, eq
	cmp	w2, 438
	cset	w0, eq
	orr	w0, w1, w0
	cbnz	w0, .L1120
.L1097:
	mov	x0, x19
	bl	MP4_Renew_CurPacket
.L1100:
	ldr	x1, [x19, 15744]
	cbz	x1, .L1104
	ldr	w1, [x19, 15756]
	cmp	w1, wzr
	csinv	w0, w0, wzr, ne
.L1113:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
.L1120:
	ldrb	w0, [x21, 1601]
	cmp	w0, 1
	cset	w0, eq
	orr	w1, w1, w0
	cbz	w1, .L1097
	.p2align 2
.L1092:
	mov	x0, 438
	ldr	x1, [x19, 22584]
	movk	x0, 0x1, lsl 32
	cmp	x1, x0
	bne	.L1114
	ldr	w0, [x21, 1696]
	cmp	w0, 4
	beq	.L1097
	mov	w0, 0
	b	.L1100
	.p2align 3
.L1118:
	ldrb	w2, [x20, 3616]
	orr	w2, w2, 2
	strb	w2, [x20, 3616]
	b	.L1091
	.p2align 3
.L1115:
	ldr	w0, [x21, 2112]
	cmp	w0, 1
	ble	.L1114
.L1095:
	mov	x0, x19
	bl	MP4_Clean_HeadPacket
	mov	w0, 0
	b	.L1100
	.p2align 3
.L1119:
	ldr	w2, [x21, 2104]
	mov	w3, 196
	cmp	w2, 438
	ccmp	w0, w3, 4, ne
	bne	.L1095
	b	.L1096
	.p2align 3
.L1104:
	mov	w0, -1
	b	.L1113
	.size	MP4_Check_CurPacket, .-MP4_Check_CurPacket
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_Decode
	.type	MPEG4DEC_Decode, %function
MPEG4DEC_Decode:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 20480
	mov	x21, x1
	bl	_mcount
	ldr	w0, [x19, 2132]
	cbz	w0, .L1122
	ldr	w0, [x21, 16]
	cmp	w0, wzr
	ble	.L1122
	ldr	w0, [x21, 12]
	cbz	w0, .L1122
	ldr	x0, [x21]
	cbz	x0, .L1122
	ldr	x1, .LC208
	mov	w0, 2
	add	x22, x20, 12288
	add	x23, x22, 3456
	bl	dprint_vfmw
	ldr	x1, .LC210
	mov	w0, 7
	ldr	w2, [x21, 12]
	bl	dprint_vfmw
	ldr	x1, .LC212
	mov	w0, 7
	ldr	w2, [x21, 16]
	bl	dprint_vfmw
	ldrb	w2, [x21, 9]
	mov	w0, 7
	ldr	x1, .LC214
	bl	dprint_vfmw
	ldr	x1, .LC216
	mov	w0, 7
	ldr	w2, [x21, 92]
	bl	dprint_vfmw
	ldr	x1, .LC218
	mov	w0, 7
	ldr	w2, [x21, 96]
	bl	dprint_vfmw
	ldrb	w2, [x21, 89]
	mov	w0, 7
	ldr	x1, .LC220
	bl	dprint_vfmw
	ldr	x1, .LC222
	mov	w0, 7
	ldr	w2, [x21, 16]
	bl	dprint_vfmw
	ldr	x2, [x21]
	mov	w0, 7
	ldr	x1, .LC224
	ldrb	w2, [x2]
	bl	dprint_vfmw
	ldr	x2, [x21]
	mov	w0, 7
	ldr	x1, .LC226
	ldrb	w2, [x2, 1]
	bl	dprint_vfmw
	ldr	x2, [x21]
	mov	w0, 7
	ldr	x1, .LC228
	ldrb	w2, [x2, 2]
	bl	dprint_vfmw
	ldr	x2, [x21]
	mov	w0, 7
	ldr	x1, .LC230
	ldrb	w2, [x2, 3]
	bl	dprint_vfmw
	ldr	x3, .LC231
	mov	x2, 160
	mov	x1, x21
	mov	x0, x23
	ldr	x3, [x3, 104]
	blr	x3
	mov	x1, x23
	mov	x0, x20
	bl	MP4_GetScdUpMsg
	mov	x0, x20
	bl	MP4_Check_CurPacket
	cmn	w0, #1
	mov	w24, w0
	beq	.L1267
	mov	x1, x23
	mov	x0, x20
	bl	MP4_GetScdUpMsg
	ldr	w1, [x23, 12]
	str	w1, [x19, 1616]
	ldr	x1, [x20, 15744]
	str	x1, [x20, 22088]
	ldr	w0, [x19, 1676]
	ldr	w2, [x23, 16]
	str	w2, [x19, 1620]
	cmp	w0, 2
	str	wzr, [x19, 1624]
	ldr	x3, [x20, 22112]
	beq	.L1268
.L1127:
	ldr	w0, [x3, 8]
	cmp	w0, 2
	beq	.L1128
	cmp	w0, 1
	beq	.L1260
	cmp	w0, 4
	csel	w0, w0, wzr, eq
.L1260:
	str	w0, [x19, 1676]
	add	x23, x20, 16384
	ldr	w0, [x3, 832]
	cmp	w0, 1
	beq	.L1269
.L1133:
	ldr	w0, [x23, 3624]
	str	w0, [x19, 2864]
.L1134:
	mov	x0, x20
	bl	Mp4_Syntax_Parse
	lsr	w2, w0, 3
	ldr	x4, [x20, 22088]
	mov	x3, x2
	and	w0, w0, 7
	add	x2, x4, x2
	str	x2, [x20, 22088]
	str	w0, [x19, 1624]
	ldr	w2, [x19, 1616]
	ldr	w0, [x19, 1620]
	add	w2, w2, w3
	str	w2, [x19, 1616]
	sub	w0, w0, w3
	str	w0, [x19, 1620]
	ldr	x0, [x20, 22112]
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L1270
.L1135:
	ldr	w1, [x23, 3624]
	mov	w0, 288
	tst	w1, w0
	beq	.L1136
	mov	w0, -435
	and	w0, w1, w0
	str	w0, [x23, 3624]
	ldr	w0, [x19, 1676]
	cmp	w0, 2
	beq	.L1271
	cmp	w0, 1
	beq	.L1272
	cmp	w0, 4
	bne	.L1190
	ldrb	w0, [x19, 1472]
	cbz	w0, .L1273
	sub	w1, w0, #2
	and	w1, w1, -3
	uxtb	w1, w1
	cbnz	w1, .L1146
	ldr	w1, [x19, 2160]
	cmp	w1, 2
	beq	.L1175
.L1146:
	cmp	w0, 1
	bne	.L1190
	str	w0, [x19, 2160]
	b	.L1190
	.p2align 3
.L1272:
	ldrb	w0, [x19, 1472]
	cmp	w0, 2
	beq	.L1138
.L1190:
	mov	x0, x20
	bl	Mp4_Frame_Atr
	cmn	w0, #1
	beq	.L1274
	ldr	w1, [x19, 2836]
	ldr	w0, [x19, 2824]
	bl	FSP_GetFsImagePtr
	mov	x25, x0
	ldr	w1, [x19, 2844]
	ldr	w0, [x19, 2824]
	bl	FSP_GetFsImagePtr
	mov	x26, x0
	cmp	x25, xzr
	cset	w24, eq
	cmp	x0, xzr
	cset	w0, eq
	orr	w24, w24, w0
	cbnz	w24, .L1275
	ldrb	w0, [x19, 835]
	and	w2, w0, -3
	uxtb	w2, w2
	cmp	w2, 1
	bne	.L1150
	cbz	x25, .L1151
	ldr	x0, [x20, 22112]
	ldr	w1, [x25, 160]
	ldr	w0, [x0]
	cmp	w1, w0
	bls	.L1153
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	w21, [x23, 3620]
	ldr	w24, [x19, 2824]
	cmp	w21, wzr
	ble	.L1276
.L1192:
	add	x22, x22, 3620
	mov	w19, 0
	.p2align 2
.L1154:
	ldr	w1, [x22], 4
	mov	w0, w24
	add	w19, w19, 1
	bl	SM_ReleaseStreamSeg
	cmp	w19, w21
	blt	.L1154
	mov	x0, x20
	str	wzr, [x23, 3620]
	bl	MP4_Clean_HeadPacket
	cbz	x25, .L1191
.L1155:
	ldr	x3, [x20, 22112]
	mov	w0, 1
	ldr	w2, [x25, 160]
	mov	w24, -1
	ldr	x1, .LC242
	ldr	w3, [x3]
	bl	dprint_vfmw
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1150:
	cmp	w0, 2
	beq	.L1277
.L1153:
	ldrb	w0, [x19, 1472]
	cmp	w0, 2
	beq	.L1170
	ldr	w3, [x19, 2832]
	mov	w0, 2
	ldr	w1, [x19, 2836]
	ldr	w2, [x19, 2844]
	cmp	w3, wzr
	csinc	w0, w0, wzr, gt
	str	w0, [x19, 2832]
	cmp	w1, w2
	beq	.L1168
	ldr	w0, [x19, 2824]
	mov	w2, 0
	bl	FSP_SetRef
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2836]
	bl	FSP_GetDisplay
	cmp	w0, 3
	beq	.L1261
	ldr	w0, [x19, 2824]
	mov	w2, 0
	ldr	w1, [x19, 2836]
	bl	FSP_SetDisplay
.L1261:
	ldr	w2, [x19, 2844]
.L1168:
	str	w2, [x19, 2836]
	mov	w2, 1
	ldrb	w0, [x20, 1]
	strb	w0, [x20]
	ldr	w0, [x19, 2848]
	ldr	w1, [x19, 2852]
	str	w1, [x19, 2844]
	str	w0, [x19, 2840]
	ldrb	w0, [x20, 2]
	strb	w0, [x20, 1]
	ldr	w3, [x19, 1564]
	ldr	w0, [x19, 2824]
	str	w3, [x19, 2848]
	bl	FSP_SetRef
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2836]
	bl	FSP_RemovePmv
	ldrb	w0, [x19, 1472]
	cmp	w0, 4
	bne	.L1170
	ldrb	w0, [x19, 832]
	cbz	w0, .L1278
.L1183:
	mov	w24, 0
.L1251:
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1268:
	ldr	w0, [x3, 8]
	cmp	w0, 2
	bne	.L1279
.L1128:
	mov	w0, 2
	str	w0, [x19, 1676]
	add	x23, x20, 16384
	ldr	w0, [x3, 832]
	cmp	w0, 1
	bne	.L1133
.L1269:
	ldr	w0, [x3, 872]
	cmp	w0, 1
	bne	.L1133
	ldr	w0, [x19, 2864]
	str	w0, [x23, 3624]
	b	.L1134
	.p2align 3
.L1270:
	ldr	w0, [x0, 872]
	cbz	w0, .L1135
	b	.L1183
	.p2align 3
.L1170:
	mov	x0, x20
	mov	x2, x21
	mov	w1, 0
	bl	Mp4_Set_DecPar
	ldrb	w0, [x19, 1472]
	cbnz	w0, .L1280
	ldr	w1, [x19, 2160]
	cbnz	w1, .L1172
	mov	w1, 2
	str	w1, [x19, 2160]
.L1173:
	ldr	x3, [x20, 22112]
	add	x1, x20, 1048
	str	x1, [x20, 1040]
	ldr	w2, [x3, 864]
	cmp	w2, 1
	beq	.L1180
.L1179:
	cmp	w2, 2
	beq	.L1199
.L1180:
	strb	wzr, [x20, 1214]
	cbnz	w0, .L1183
	ldr	x0, .LC245
	ldr	x4, [x0]
	cbz	x4, .L1183
	add	x2, x29, 96
	ldr	w0, [x19, 2824]
	mov	w3, 4
	mov	w1, 20
	mov	w24, 0
	str	wzr, [x2, -8]!
	blr	x4
	b	.L1251
	.p2align 3
.L1271:
	ldrb	w0, [x19, 1472]
	cbz	w0, .L1190
.L1138:
	ldr	w0, [x23, 3620]
	ldr	w21, [x19, 2824]
	cmp	w0, wzr
	ble	.L1143
	sub	w19, w0, #1
	add	x22, x22, 3620
	add	x19, x19, 3978
	add	x19, x20, x19, lsl 2
	.p2align 2
.L1142:
	ldr	w1, [x22], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x19, x22
	bne	.L1142
.L1143:
	mov	x0, x20
	str	wzr, [x23, 3620]
	mov	w24, -1
	bl	MP4_Clean_HeadPacket
	b	.L1251
	.p2align 3
.L1273:
	ldr	w0, [x19, 2160]
	cbnz	w0, .L1145
	mov	w0, 2
	str	w0, [x19, 2160]
	b	.L1190
	.p2align 3
.L1279:
	mov	w0, 1
	str	w0, [x19, 2208]
	b	.L1127
	.p2align 3
.L1280:
	sub	w1, w0, #2
	cmp	w0, 2
	and	w1, w1, -3
	cset	w24, eq
	uxtb	w1, w1
	cbnz	w1, .L1174
	ldr	w1, [x19, 2160]
	cmp	w1, 2
	beq	.L1281
.L1174:
	cmp	w0, 1
	bne	.L1176
	str	w0, [x19, 2160]
	add	x1, x20, 1048
	str	x1, [x20, 1040]
	ldr	x3, [x20, 22112]
	ldr	w2, [x3, 864]
	cmp	w2, 1
	beq	.L1177
	cmp	w2, 2
	beq	.L1199
.L1177:
	strb	wzr, [x20, 1214]
	b	.L1183
	.p2align 3
.L1277:
	cbz	x25, .L1156
	ldr	x0, [x20, 22112]
	ldr	w1, [x25, 160]
	ldr	w0, [x0]
	cmp	w1, w0
	bhi	.L1157
	cbz	x26, .L1161
	ldr	w1, [x26, 160]
	cmp	w0, w1
	bcs	.L1153
.L1161:
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	w21, [x19, 2824]
	ldr	w0, [x23, 3620]
	cmp	w0, wzr
	ble	.L1162
	sub	w19, w0, #1
	add	x22, x22, 3620
	add	x19, x19, 3978
	add	x19, x20, x19, lsl 2
	.p2align 2
.L1163:
	ldr	w1, [x22], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x22, x19
	bne	.L1163
.L1162:
	mov	x0, x20
	str	wzr, [x23, 3620]
	mov	w24, -1
	bl	MP4_Clean_HeadPacket
	ldr	x3, [x20, 22112]
	mov	w0, 1
	ldr	x1, .LC255
	ldr	w2, [x26, 160]
	ldr	w3, [x3]
	bl	dprint_vfmw
	b	.L1251
	.p2align 3
.L1278:
	ldr	x1, .LC247
	mov	w0, 11
	bl	dprint_vfmw
	ldr	w1, [x19, 2852]
	ldr	w0, [x19, 2824]
	bl	FSP_GetLogicFs
	mov	x21, x0
	ldr	w1, [x19, 2836]
	ldr	w0, [x19, 2824]
	bl	FSP_GetLogicFs
	mov	x24, x0
	cmp	x21, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L1282
	ldr	x0, [x21, 640]
	cbz	x0, .L1186
	ldr	x1, [x21, 656]
	cbz	x1, .L1186
	ldr	x1, [x21, 648]
	cbz	x1, .L1186
	ldr	x4, [x24, 640]
	ldr	w2, [x0, 16]
	ldr	w3, [x1, 16]
	cbz	x4, .L1202
	ldr	w4, [x4, 16]
.L1189:
	ldr	w6, [x19, 1532]
	mov	x1, x21
	ldr	w5, [x19, 1536]
	mov	x0, x20
	mov	w24, 0
	bl	Vdm_Mem_Move
	b	.L1251
	.p2align 3
.L1145:
	mov	w0, 1
	str	w0, [x19, 2160]
	b	.L1190
	.p2align 3
.L1122:
	ldr	x1, .LC206
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L1251
	.p2align 3
.L1172:
	mov	w1, 1
	str	w1, [x19, 2160]
	b	.L1173
	.p2align 3
.L1157:
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	w21, [x23, 3620]
	ldr	w24, [x19, 2824]
	cmp	w21, wzr
	ble	.L1283
.L1195:
	add	x22, x22, 3620
	mov	w19, 0
	.p2align 2
.L1159:
	ldr	w1, [x22], 4
	mov	w0, w24
	add	w19, w19, 1
	bl	SM_ReleaseStreamSeg
	cmp	w19, w21
	blt	.L1159
	mov	x0, x20
	str	wzr, [x23, 3620]
	bl	MP4_Clean_HeadPacket
	cbz	x25, .L1197
.L1160:
	ldr	x3, [x20, 22112]
	mov	w0, 1
	ldr	x1, .LC244
	mov	w24, -1
	ldr	w2, [x25, 160]
	ldr	w3, [x3]
	bl	dprint_vfmw
	b	.L1251
	.p2align 3
.L1199:
	ldr	w2, [x3, 868]
	cmp	w2, wzr
	cset	w2, gt
	cmp	x1, xzr
	and	w24, w24, w2
	cset	w1, ne
	tst	w1, w24
	beq	.L1180
	str	xzr, [x20, 1040]
	mov	w2, 1
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	x1, [x20, 22112]
	ldr	w0, [x1, 868]
	sub	w0, w0, #1
	str	w0, [x1, 868]
	ldr	w0, [x23, 3620]
	ldr	w21, [x19, 2824]
	cmp	w0, wzr
	ble	.L1143
	sub	w19, w0, #1
	add	x22, x22, 3620
	add	x19, x19, 3978
	add	x19, x20, x19, lsl 2
.L1182:
	ldr	w1, [x22], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x22, x19
	bne	.L1182
	b	.L1143
	.p2align 3
.L1176:
	ldr	x3, [x20, 22112]
	add	x1, x20, 1048
	str	x1, [x20, 1040]
	ldr	w2, [x3, 864]
	cmp	w2, 1
	bne	.L1179
	cmp	x1, xzr
	cset	w1, ne
	tst	w24, w1
	beq	.L1180
	str	xzr, [x20, 1040]
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	w21, [x19, 2824]
	ldr	w0, [x23, 3620]
	cmp	w0, wzr
	ble	.L1143
	sub	w19, w0, #1
	add	x22, x22, 3620
	add	x19, x19, 3978
	add	x19, x20, x19, lsl 2
.L1181:
	ldr	w1, [x22], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x22, x19
	bne	.L1181
	b	.L1143
	.p2align 3
.L1151:
	ldr	w0, [x19, 2824]
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	w21, [x23, 3620]
	ldr	w24, [x19, 2824]
	cmp	w21, wzr
	bgt	.L1192
	mov	x0, x20
	str	wzr, [x23, 3620]
	bl	MP4_Clean_HeadPacket
	.p2align 2
.L1191:
	ldr	x2, .LC236
	mov	w3, 3945
	ldr	x1, .LC240
	mov	w0, 1
	add	x2, x2, 1360
	mov	w24, -1
	bl	dprint_vfmw
	b	.L1251
.L1276:
	mov	x0, x20
	str	wzr, [x23, 3620]
	bl	MP4_Clean_HeadPacket
	b	.L1155
.L1281:
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	x0, .LC245
	ldr	x4, [x0]
	cbz	x4, .L1175
	ldr	w0, [x19, 2824]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	mov	w24, -1
	blr	x4
	b	.L1251
.L1267:
	ldr	x1, .LC233
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1251
.L1136:
	ldr	x1, .LC253
	mov	w0, 2
	mov	w24, -1
	bl	dprint_vfmw
	b	.L1251
.L1275:
	ldr	x2, .LC236
	mov	w3, 3931
	ldr	x1, .LC238
	mov	w0, 1
	add	x2, x2, 1360
	mov	w24, -1
	bl	dprint_vfmw
	b	.L1251
.L1274:
	ldr	x1, .LC235
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w21, [x19, 2824]
	ldr	w0, [x23, 3620]
	cmp	w0, wzr
	ble	.L1143
	sub	w19, w0, #1
	add	x22, x22, 3620
	add	x19, x19, 3978
	add	x19, x20, x19, lsl 2
	.p2align 2
.L1148:
	ldr	w1, [x22], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x22, x19
	bne	.L1148
	b	.L1143
.L1202:
	mov	w4, w2
	b	.L1189
.L1156:
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	w21, [x23, 3620]
	cmp	w21, wzr
	bgt	.L1284
	mov	x0, x20
	str	wzr, [x23, 3620]
	bl	MP4_Clean_HeadPacket
.L1197:
	ldr	x2, .LC236
	mov	w3, 3965
	ldr	x1, .LC240
	mov	w0, 1
	add	x2, x2, 1360
	mov	w24, -1
	bl	dprint_vfmw
	b	.L1251
.L1175:
	mov	w24, -1
	b	.L1251
.L1284:
	ldr	w24, [x19, 2824]
	b	.L1195
.L1282:
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC249
	mov	x3, x21
	mov	x4, x24
	mov	w2, 4171
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w21, [x19, 2824]
	ldr	w0, [x23, 3620]
	cmp	w0, wzr
	ble	.L1143
	sub	w19, w0, #1
	add	x22, x22, 3620
	add	x19, x19, 3978
	add	x19, x20, x19, lsl 2
.L1185:
	ldr	w1, [x22], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x19, x22
	bne	.L1185
	b	.L1143
.L1186:
	ldr	w0, [x19, 2824]
	mov	w2, 1
	ldr	w1, [x19, 2852]
	bl	FSP_ClearLogicFs
	ldr	x5, [x21, 640]
	mov	w0, 0
	ldr	x7, [x21, 648]
	ldr	x6, [x21, 656]
	ldr	x1, .LC251
	ldr	w2, [x19, 2836]
	ldr	w4, [x19, 2844]
	ldr	w3, [x19, 2852]
	bl	dprint_vfmw
	ldr	w21, [x19, 2824]
	ldr	w0, [x23, 3620]
	cmp	w0, wzr
	ble	.L1143
	sub	w19, w0, #1
	add	x22, x22, 3620
	add	x19, x19, 3978
	add	x19, x20, x19, lsl 2
.L1188:
	ldr	w1, [x22], 4
	mov	w0, w21
	bl	SM_ReleaseStreamSeg
	cmp	x19, x22
	bne	.L1188
	b	.L1143
.L1283:
	mov	x0, x20
	str	wzr, [x23, 3620]
	bl	MP4_Clean_HeadPacket
	b	.L1160
	.size	MPEG4DEC_Decode, .-MPEG4DEC_Decode
	.align	3
.LC206:
	.xword	.LC205
	.align	3
.LC208:
	.xword	.LC207
	.align	3
.LC210:
	.xword	.LC209
	.align	3
.LC212:
	.xword	.LC211
	.align	3
.LC214:
	.xword	.LC213
	.align	3
.LC216:
	.xword	.LC215
	.align	3
.LC218:
	.xword	.LC217
	.align	3
.LC220:
	.xword	.LC219
	.align	3
.LC222:
	.xword	.LC221
	.align	3
.LC224:
	.xword	.LC223
	.align	3
.LC226:
	.xword	.LC225
	.align	3
.LC228:
	.xword	.LC227
	.align	3
.LC230:
	.xword	.LC229
	.align	3
.LC231:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC233:
	.xword	.LC232
	.align	3
.LC235:
	.xword	.LC234
	.align	3
.LC236:
	.xword	.LANCHOR1
	.align	3
.LC238:
	.xword	.LC237
	.align	3
.LC240:
	.xword	.LC239
	.align	3
.LC242:
	.xword	.LC241
	.align	3
.LC244:
	.xword	.LC243
	.align	3
.LC245:
	.xword	g_event_report
	.align	3
.LC247:
	.xword	.LC246
	.align	3
.LC249:
	.xword	.LC248
	.align	3
.LC251:
	.xword	.LC250
	.align	3
.LC253:
	.xword	.LC252
	.align	3
.LC255:
	.xword	.LC254
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_GetImageWidth
	.type	MPEG4DEC_GetImageWidth, %function
MPEG4DEC_GetImageWidth:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 20480
	bl	_mcount
	ldr	w0, [x19, 1536]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MPEG4DEC_GetImageWidth, .-MPEG4DEC_GetImageWidth
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MPEG4DEC_GetImageHeight
	.type	MPEG4DEC_GetImageHeight, %function
MPEG4DEC_GetImageHeight:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 20480
	bl	_mcount
	ldr	w0, [x19, 1532]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MPEG4DEC_GetImageHeight, .-MPEG4DEC_GetImageHeight
	.global	sprite_trajectory_len
	.global	default_inter_matrix
	.global	default_intra_matrix
	.global	scan_tables
	.section	.rodata
	.align	3
.LANCHOR1 = . + 0
	.type	default_intra_matrix, %object
	.size	default_intra_matrix, 256
default_intra_matrix:
	.word	8
	.word	17
	.word	18
	.word	19
	.word	21
	.word	23
	.word	25
	.word	27
	.word	17
	.word	18
	.word	19
	.word	21
	.word	23
	.word	25
	.word	27
	.word	28
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	32
	.word	22
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	32
	.word	35
	.word	23
	.word	24
	.word	26
	.word	28
	.word	30
	.word	32
	.word	35
	.word	38
	.word	25
	.word	26
	.word	28
	.word	30
	.word	32
	.word	35
	.word	38
	.word	41
	.word	27
	.word	28
	.word	30
	.word	32
	.word	35
	.word	38
	.word	41
	.word	45
	.type	default_inter_matrix, %object
	.size	default_inter_matrix, 256
default_inter_matrix:
	.word	16
	.word	17
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	17
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	25
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	27
	.word	20
	.word	21
	.word	22
	.word	23
	.word	25
	.word	26
	.word	27
	.word	28
	.word	21
	.word	22
	.word	23
	.word	24
	.word	26
	.word	27
	.word	28
	.word	30
	.word	22
	.word	23
	.word	24
	.word	26
	.word	27
	.word	28
	.word	30
	.word	31
	.word	23
	.word	24
	.word	25
	.word	27
	.word	28
	.word	30
	.word	31
	.word	33
	.type	scan_tables, %object
	.size	scan_tables, 768
scan_tables:
	.word	0
	.word	1
	.word	8
	.word	16
	.word	9
	.word	2
	.word	3
	.word	10
	.word	17
	.word	24
	.word	32
	.word	25
	.word	18
	.word	11
	.word	4
	.word	5
	.word	12
	.word	19
	.word	26
	.word	33
	.word	40
	.word	48
	.word	41
	.word	34
	.word	27
	.word	20
	.word	13
	.word	6
	.word	7
	.word	14
	.word	21
	.word	28
	.word	35
	.word	42
	.word	49
	.word	56
	.word	57
	.word	50
	.word	43
	.word	36
	.word	29
	.word	22
	.word	15
	.word	23
	.word	30
	.word	37
	.word	44
	.word	51
	.word	58
	.word	59
	.word	52
	.word	45
	.word	38
	.word	31
	.word	39
	.word	46
	.word	53
	.word	60
	.word	61
	.word	54
	.word	47
	.word	55
	.word	62
	.word	63
	.word	0
	.word	1
	.word	2
	.word	3
	.word	8
	.word	9
	.word	16
	.word	17
	.word	10
	.word	11
	.word	4
	.word	5
	.word	6
	.word	7
	.word	15
	.word	14
	.word	13
	.word	12
	.word	19
	.word	18
	.word	24
	.word	25
	.word	32
	.word	33
	.word	26
	.word	27
	.word	20
	.word	21
	.word	22
	.word	23
	.word	28
	.word	29
	.word	30
	.word	31
	.word	34
	.word	35
	.word	40
	.word	41
	.word	48
	.word	49
	.word	42
	.word	43
	.word	36
	.word	37
	.word	38
	.word	39
	.word	44
	.word	45
	.word	46
	.word	47
	.word	50
	.word	51
	.word	56
	.word	57
	.word	58
	.word	59
	.word	52
	.word	53
	.word	54
	.word	55
	.word	60
	.word	61
	.word	62
	.word	63
	.word	0
	.word	8
	.word	16
	.word	24
	.word	1
	.word	9
	.word	2
	.word	10
	.word	17
	.word	25
	.word	32
	.word	40
	.word	48
	.word	56
	.word	57
	.word	49
	.word	41
	.word	33
	.word	26
	.word	18
	.word	3
	.word	11
	.word	4
	.word	12
	.word	19
	.word	27
	.word	34
	.word	42
	.word	50
	.word	58
	.word	35
	.word	43
	.word	51
	.word	59
	.word	20
	.word	28
	.word	5
	.word	13
	.word	6
	.word	14
	.word	21
	.word	29
	.word	36
	.word	44
	.word	52
	.word	60
	.word	37
	.word	45
	.word	53
	.word	61
	.word	22
	.word	30
	.word	7
	.word	15
	.word	23
	.word	31
	.word	38
	.word	46
	.word	54
	.word	62
	.word	39
	.word	47
	.word	55
	.word	63
	.type	__FUNCTION__.11876, %object
	.size	__FUNCTION__.11876, 15
__FUNCTION__.11876:
	.string	"Mp4_Set_DecPar"
	.zero	1
	.type	__FUNCTION__.12010, %object
	.size	__FUNCTION__.12010, 23
__FUNCTION__.12010:
	.string	"Mp4_Syntax_Slice_NotSH"
	.zero	1
	.type	__FUNCTION__.12024, %object
	.size	__FUNCTION__.12024, 20
__FUNCTION__.12024:
	.string	"Mp4_Syntax_Slice_SH"
	.zero	4
	.type	__FUNCTION__.12039, %object
	.size	__FUNCTION__.12039, 14
__FUNCTION__.12039:
	.string	"MP4_Dec_Slice"
	.zero	2
	.type	__FUNCTION__.11913, %object
	.size	__FUNCTION__.11913, 16
__FUNCTION__.11913:
	.string	"MPEG4DEC_Decode"
	.data
	.align	3
.LANCHOR0 = . + 0
	.type	sprite_trajectory_len, %object
	.size	sprite_trajectory_len, 120
sprite_trajectory_len:
	.word	0
	.byte	2
	.zero	3
	.word	2
	.byte	3
	.zero	3
	.word	3
	.byte	3
	.zero	3
	.word	4
	.byte	3
	.zero	3
	.word	5
	.byte	3
	.zero	3
	.word	6
	.byte	3
	.zero	3
	.word	14
	.byte	4
	.zero	3
	.word	30
	.byte	5
	.zero	3
	.word	62
	.byte	6
	.zero	3
	.word	126
	.byte	7
	.zero	3
	.word	254
	.byte	8
	.zero	3
	.word	510
	.byte	9
	.zero	3
	.word	1022
	.byte	10
	.zero	3
	.word	2046
	.byte	11
	.zero	3
	.word	4094
	.byte	12
	.zero	3
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC14:
	ASCII(.string	"fatal stream error --- GOP marker_bit = 0\n" )
	.zero	5
.LC19:
	ASCII(.string	"DivX%4dBuild%4d%c" )
	.zero	6
.LC21:
	ASCII(.string	"DivX%4db%4d%c" )
	.zero	2
.LC23:
	ASCII(.string	"FFmpe%*[^b]b%4d" )
.LC25:
	ASCII(.string	"FFmpeg v%4d.%4d.%4d / libavcodec build: %d" )
	.zero	5
.LC27:
	ASCII(.string	"Lavc%d.%d.%d" )
	.zero	3
.LC29:
	ASCII(.string	"ffmpeg" )
	.zero	1
.LC31:
	ASCII(.string	"XviD%4d" )
.LC36:
	ASCII(.string	"fatal stream error --- VOP marker_bit = 0\n" )
	.zero	5
.LC40:
	ASCII(.string	"VDH doesn't support SPRITE_STATIC & SPRITE_GMC\n" )
.LC42:
	ASCII(.string	"VDH doesn't support SPRITE_STATIC\n" )
	.zero	5
.LC44:
	ASCII(.string	"Unknown sprite_enable\n" )
	.zero	1
.LC46:
	ASCII(.string	"ERROR: not_8_bit !=0\n" )
	.zero	2
.LC49:
	ASCII(.string	"not support no complexity_estimation_disable\n" )
	.zero	2
.LC51:
	ASCII(.string	"not support data_partitioned\n" )
	.zero	2
.LC53:
	ASCII(.string	"not support newpred_enable\n" )
	.zero	4
.LC55:
	ASCII(.string	"not support reduced_resolution_vop_eanble\n" )
	.zero	5
.LC57:
	ASCII(.string	"not support scalability\n" )
	.zero	7
.LC61:
	ASCII(.string	"warning! profile_and_level_indication=%#x, maybe HD pic...\n" )
	.zero	4
.LC64:
	ASCII(.string	"actual frame size(%dx%d) exeed max config(%dx%d)\n" )
	.zero	6
.LC67:
	ASCII(.string	"MPEG4 Frame_Init return err!\n" )
	.zero	2
.LC70:
	ASCII(.string	"MPEG4 Frame_Init PmvNum=%d, FrameNum=%d, error!\n" )
	.zero	7
.LC73:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC75:
	ASCII(.string	"ERROR: FSP_ConfigInstance fail!\n" )
	.zero	7
.LC77:
	ASCII(.string	"NVOP VDM copy process................\n" )
	.zero	1
.LC79:
	ASCII(.string	"dec_pts: %lld Usertag: %lld\n" )
	.zero	3
.LC82:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC86:
	ASCII(.string	"%s %d  abnormal!\n" )
	.zero	6
.LC88:
	ASCII(.string	"CurFsID(fwd%d,%d,bwd%d) abnormal!(%p,%p,%p)\n" )
	.zero	3
.LC90:
	ASCII(.string	"MPEG4DEC Init  -----\n" )
	.zero	2
.LC93:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC95:
	ASCII(.string	"MPEG4DEC Init success -----\n" )
	.zero	3
.LC97:
	ASCII(.string	"MPEG4 destroy\n" )
	.zero	1
.LC102:
	ASCII(.string	"vop_coding_type = %d is not expected!\n" )
	.zero	1
.LC104:
	ASCII(.string	" slice num %d is not extpected!\n" )
	.zero	7
.LC106:
	ASCII(.string	" line %d packet is wrong!\n" )
	.zero	5
.LC110:
	ASCII(.string	"pImg is null, pCurImg %#x pBwdImg %#x\n" )
	.zero	1
.LC112:
	ASCII(.string	"FrameNum=%d\n" )
	.zero	3
.LC114:
	ASCII(.string	"vop_coding_type=%d\n" )
	.zero	4
.LC117:
	ASCII(.string	"line: %d pToQueImg is null!\n" )
	.zero	3
.LC120:
	ASCII(.string	"CurImage %d is already out put\n" )
.LC122:
	ASCII(.string	"err(%d) > out_thr(%d)\n" )
	.zero	1
.LC124:
	ASCII(.string	"StreamIDNum = %d > (%d)\n" )
	.zero	7
.LC126:
	ASCII(.string	"Warning : vop slice num %d != 0\n" )
	.zero	7
.LC131:
	ASCII(.string	"warning streamIDNum %d != 0 \n" )
	.zero	2
.LC135:
	ASCII(.string	"vop ---%d\n" )
	.zero	5
.LC139:
	ASCII(.string	"VDH doesn't support S_VOP\n" )
	.zero	5
.LC141:
	ASCII(.string	"parse time_module_base stream not enough\n" )
	.zero	6
.LC143:
	ASCII(.string	"module time base parse error, more than %d '1' was found!\n" )
	.zero	5
.LC146:
	ASCII(.string	"error: vop_coding_type = %d, but vop_fcode_forward = 0\n" )
.LC148:
	ASCII(.string	"error: B vop, but vop_fcode_backward = 0\n" )
	.zero	6
.LC152:
	ASCII(.string	"FUN: %s Invalid SlcNum = %d\n" )
	.zero	3
.LC154:
	ASCII(.string	" pSlcPara->mb_start_num = %d, MbCnt = %d\n" )
	.zero	6
.LC158:
	ASCII(.string	"pSlcPara->mb_start_num = %d, totalMbMin1 = %d\n" )
	.zero	1
.LC161:
	ASCII(.string	"FUN: %s MP4_ClearCurPacket\n" )
	.zero	4
.LC164:
	ASCII(.string	"pCtx is NUL!!!\n" )
.LC169:
	ASCII(.string	"Not find start code \n" )
	.zero	2
.LC171:
	ASCII(.string	"VOS beginning ~ !\n" )
	.zero	5
.LC173:
	ASCII(.string	"VO beginning ~ !\n" )
	.zero	6
.LC175:
	ASCII(.string	"VOL beginning ~ !\n" )
	.zero	5
.LC177:
	ASCII(.string	"GOP beginning ~ !\n" )
	.zero	5
.LC179:
	ASCII(.string	"VOP beginning ~ !\n" )
	.zero	5
.LC181:
	ASCII(.string	"BitUsed=%d\n" )
	.zero	4
.LC183:
	ASCII(.string	"SHVOP beginning ~ !\n" )
	.zero	3
.LC185:
	ASCII(.string	"USERDATA beginning ~ !\n" )
.LC187:
	ASCII(.string	"MPEG4 Frame_Init return err 3!\n" )
.LC189:
	ASCII(.string	"MPEG4 Frame_Init return err 1!\n" )
.LC191:
	ASCII(.string	"vop right\n" )
	.zero	5
.LC193:
	ASCII(.string	"MPEG4 Frame_Init return err 2!\n" )
.LC195:
	ASCII(.string	"bits over boundary\n" )
	.zero	4
.LC198:
	ASCII(.string	"pLastPacket->StreamPack[0].VirAddr is NULL!!\n" )
	.zero	2
.LC202:
	ASCII(.string	"line %d MP4_Combine_HdPacket failed!!\n" )
	.zero	1
.LC205:
	ASCII(.string	"bitsream para error -----\n" )
	.zero	5
.LC207:
	ASCII(.string	"MPEG4DEC Decode -----\n" )
	.zero	1
.LC209:
	ASCII(.string	"ptr_strm_para->PhyAddr0=0x%x\n" )
	.zero	2
.LC211:
	ASCII(.string	"ptr_strm_para->Length0=%d\n" )
	.zero	5
.LC213:
	ASCII(.string	"ptr_strm_para->IsLastSeg0=%d\n" )
	.zero	2
.LC215:
	ASCII(.string	"ptr_strm_para->PhyAddr1=0x%x\n" )
	.zero	2
.LC217:
	ASCII(.string	"ptr_strm_para->Length1=%d\n" )
	.zero	5
.LC219:
	ASCII(.string	"ptr_strm_para->IsLastSeg1=%d\n" )
	.zero	2
.LC221:
	ASCII(.string	"ptr_strm_para->Length0 = %d\n" )
	.zero	3
.LC223:
	ASCII(.string	"byte0: = 0x%x\n" )
	.zero	1
.LC225:
	ASCII(.string	"byte1: = 0x%x\n" )
	.zero	1
.LC227:
	ASCII(.string	"byte2: = 0x%x\n" )
	.zero	1
.LC229:
	ASCII(.string	"byte3: = 0x%x\n" )
	.zero	1
.LC232:
	ASCII(.string	"MP4_Check_CurPacket failed!\n" )
	.zero	3
.LC234:
	ASCII(.string	"Mp4_Frame_Atr return fail\n" )
	.zero	5
.LC237:
	ASCII(.string	"fuction return value is null, %s %d!!\n" )
	.zero	1
.LC239:
	ASCII(.string	"fuction return value is null,%s %d!!\n" )
	.zero	2
.LC241:
	ASCII(.string	"dec P/S frame, fwd ref err(%d) > ref_thr(%d)\n" )
	.zero	2
.LC243:
	ASCII(.string	"dec B frame, bwd ref err(%d) > ref_thr(%d)\n" )
	.zero	4
.LC246:
	ASCII(.string	"NVOP mem move--\n" )
	.zero	7
.LC248:
	ASCII(.string	"line: %d pCur = %p, pFwd = %p\n" )
	.zero	1
.LC250:
	ASCII(.string	"CurFsID(f%d,%d,b%d) abnormal!(%p,%p,%p)\n" )
	.zero	7
.LC252:
	ASCII(.string	"bits not enought\n" )
	.zero	6
.LC254:
	ASCII(.string	"dec B frame, fwd ref err(%d) > ref_thr(%d)\n" )
	.bss
	.align	3
.LANCHOR2 = . + 0
	.type	MemArrange.11814, %object
	.size	MemArrange.11814, 1272
MemArrange.11814:
	.zero	1272
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
