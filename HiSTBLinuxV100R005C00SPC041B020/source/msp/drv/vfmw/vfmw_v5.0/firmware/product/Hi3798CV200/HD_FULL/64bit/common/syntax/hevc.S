#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"hevc.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	compare_pic_by_poc_asc, %function
compare_pic_by_poc_asc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, -1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 12]
	ldr	w2, [x2, 12]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_pic_by_poc_asc, .-compare_pic_by_poc_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	HEVC_SetFrmRepeatCount.part.3, %function
HEVC_SetFrmRepeatCount.part.3:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	w3, [x19, 276]
	ldr	w2, [x19, 2680]
	ldr	w0, [x19, 272]
	add	w2, w3, w2
	add	w3, w2, 1
	cmp	w0, w3
	bcc	.L5
	sub	w0, w0, #1
	sub	w0, w0, w2
	cmp	w0, 1
	bls	.L6
	mov	w0, 2
	str	w0, [x20, 24]
	ldr	w0, [x19, 276]
	add	w0, w0, 2
	str	w0, [x19, 276]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L5:
	str	wzr, [x20, 24]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L6:
	mov	w0, 1
	str	w0, [x20, 24]
	ldr	w0, [x19, 276]
	add	w0, w0, 1
	str	w0, [x19, 276]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_SetFrmRepeatCount.part.3, .-HEVC_SetFrmRepeatCount.part.3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	HEVC_Sei_Mastering_Display_Colour_Volume.part.11, %function
HEVC_Sei_Mastering_Display_Colour_Volume.part.11:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	add	x19, x1, 4
	add	x22, x1, 16
	bl	_mcount
.L10:
	add	x19, x19, 4
	ldr	x0, [x20, 2312]
	mov	w1, 16
	bl	BsGet
	str	w0, [x19, -4]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 8]
	cmp	x19, x22
	bne	.L10
	ldr	x0, [x20, 2312]
	mov	w1, 16
	bl	BsGet
	str	w0, [x21, 28]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x21, 32]
	mov	w1, 32
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x21, 36]
	mov	w1, 32
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x21, 40]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_Sei_Mastering_Display_Colour_Volume.part.11, .-HEVC_Sei_Mastering_Display_Colour_Volume.part.11
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InitSigLastScan
	.type	HEVC_InitSigLastScan, %function
HEVC_InitSigLastScan:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w5
	mov	x20, x4
	bl	_mcount
	cmp	w21, 15
	bgt	.L13
	cmp	w21, 8
	mul	w10, w21, w21
	beq	.L48
	mov	x8, x20
	cbz	w10, .L13
.L19:
	mov	w7, 1
	mov	w1, 0
	mov	w9, 0
	sub	w7, w7, w21
.L23:
	cmp	w21, w9
	mov	w0, w9
	mov	w2, 0
	bgt	.L25
	.p2align 2
.L39:
	sub	w0, w0, #1
	add	w2, w2, 1
	cmp	w21, w0
	ble	.L39
.L25:
	cmp	w0, wzr
	ccmp	w21, w2, 4, ge
	ble	.L21
	madd	w3, w21, w0, w2
	sub	w2, w2, w1
	.p2align 2
.L22:
	str	w3, [x8, w1, uxtw 2]
	sub	w0, w0, #1
	add	w1, w1, 1
	cmp	w0, wzr
	add	w6, w2, w1
	add	w3, w3, w7
	ccmp	w21, w6, 4, ge
	bgt	.L22
.L21:
	cmp	w10, w1
	add	w9, w9, 1
	bhi	.L23
.L13:
	cmp	w21, 4
	ble	.L12
	asr	w14, w21, 2
	add	x0, x19, x14, uxtw
	mul	w16, w14, w14
	ldrsb	w0, [x0, 10]
	add	w0, w0, 1
	cbz	w16, .L12
.L17:
	ldr	x15, .LC5
	sbfiz	x0, x0, 16, 32
	mov	w9, 1
	add	x13, x19, 324
	add	x15, x15, x0
	mov	x12, 0
	sub	w9, w9, w21
.L34:
	cmp	w21, 32
	ldr	w3, [x15, x12, lsl 2]
	beq	.L49
.L27:
	udiv	w2, w3, w14
	lsl	w8, w12, 4
	mov	w10, 0
	mov	w0, 0
	mov	w7, 0
	mov	w1, 0
	madd	w3, w2, w21, w3
	msub	w2, w2, w14, w3
	lsl	w11, w2, 2
	.p2align 2
.L28:
	cmp	w1, wzr
	ccmp	w7, 3, 0, ge
	bgt	.L29
	madd	w2, w21, w1, w7
	sub	w7, w7, w0
	add	w2, w2, w11
	.p2align 2
.L30:
	add	w6, w0, w8
	sub	w1, w1, #1
	add	w0, w0, 1
	cmp	w1, wzr
	add	w3, w7, w0
	str	w2, [x20, w6, uxtw 2]
	ccmp	w3, 3, 0, ge
	add	w2, w2, w9
	ble	.L30
.L29:
	cmp	w0, 15
	add	w10, w10, 1
	bhi	.L31
	cmp	w10, 3
	ble	.L50
	sub	w7, w10, #3
	mov	w1, 3
	b	.L28
.L31:
	add	x12, x12, 1
	add	x13, x13, 4
	cmp	w16, w12
	bhi	.L34
.L12:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L49:
	ldr	w3, [x13]
	b	.L27
.L48:
	add	x8, x19, 324
	cbnz	w10, .L19
	ldrsb	w0, [x19, 12]
	mov	w14, 2
	mov	w16, 4
	add	w0, w0, 1
	b	.L17
.L50:
	mov	w1, w10
	mov	w7, 0
	b	.L28
	.size	HEVC_InitSigLastScan, .-HEVC_InitSigLastScan
	.align	3
.LC5:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InitScalingOrderTable
	.type	HEVC_InitScalingOrderTable, %function
HEVC_InitScalingOrderTable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x22, 10
	add	x2, x22, 139
	mov	w1, -1
	.p2align 2
.L52:
	strb	w1, [x0], 1
	cmp	x2, x0
	bne	.L52
	ldr	x21, .LC8
	mov	w3, 1
	mov	w2, 2
	mov	w1, 3
	mov	w0, 4
	strb	wzr, [x22, 14]
	strb	w3, [x22, 18]
	mov	w20, 2
	strb	w2, [x22, 26]
	mov	w19, 0
	strb	w1, [x22, 42]
	strb	w0, [x22, 74]
	.p2align 2
.L53:
	mov	w7, w19
	mov	w6, w20
	mov	w5, w20
	mov	x4, x21
	mov	x3, 0
	mov	x2, 0
	mov	x1, 0
	mov	x0, x22
	add	w19, w19, 1
	lsl	w20, w20, 1
	bl	HEVC_InitSigLastScan
	add	x21, x21, 65536
	cmp	w19, 7
	bne	.L53
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_InitScalingOrderTable, .-HEVC_InitScalingOrderTable
	.align	3
.LC8:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCGetDar
	.type	HEVCGetDar, %function
HEVCGetDar:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	w21, w1
	mov	w23, w2
	mov	w22, w3
	mov	w24, w4
	mov	w20, 0
	bl	_mcount
	cbz	w19, .L57
	mov	w0, 255
	cmp	w19, 16
	ccmp	w19, w0, 4, gt
	bne	.L57
	cmp	w21, wzr
	ccmp	w23, wzr, 4, ne
	ccmp	w19, w0, 0, eq
	beq	.L57
	cmp	w19, 1
	mov	w20, 5
	beq	.L57
	cmp	w19, w0
	beq	.L58
	ldr	x0, .LC9
	sbfiz	x19, x19, 3, 32
	add	x1, x0, x19
	ldr	w21, [x0, x19]
	ldr	w23, [x1, 4]
.L58:
	lsl	w4, w21, 10
	lsl	w22, w22, 10
	sdiv	w2, w4, w23
	sdiv	w4, w22, w24
	mul	w4, w2, w4
	cmp	w4, wzr
	add	w0, w4, 1023
	csel	w4, w0, w4, lt
	asr	w4, w4, 10
	subs	w1, w4, #2406
	bmi	.L80
.L59:
	subs	w0, w4, #2263
	bmi	.L81
	cmp	w1, w0
	mov	w20, 4
	bge	.L63
.L57:
	ldr	x1, .LC11
	mov	w2, w20
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L81:
	mov	w0, 2263
	mov	w20, 4
	sub	w0, w0, w4
	cmp	w1, w0
	blt	.L57
.L63:
	subs	w1, w4, #1820
	bmi	.L82
	cmp	w0, w1
	mov	w20, 3
	blt	.L57
.L64:
	subs	w0, w4, #1365
	bmi	.L83
.L62:
	mov	w20, 2
	cmp	w1, w0
	csinc	w20, w20, wzr, lt
	b	.L57
	.p2align 3
.L80:
	mov	w1, 2406
	sub	w1, w1, w4
	b	.L59
	.p2align 3
.L82:
	mov	w1, 1820
	mov	w20, 3
	sub	w1, w1, w4
	cmp	w0, w1
	blt	.L57
	b	.L64
.L83:
	mov	w0, 1365
	sub	w0, w0, w4
	b	.L62
	.size	HEVCGetDar, .-HEVCGetDar
	.align	3
.LC9:
	.xword	.LANCHOR1
	.align	3
.LC11:
	.xword	.LC10
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_PrintNaluArraySlot
	.type	HEVC_PrintNaluArraySlot, %function
HEVC_PrintNaluArraySlot:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC13
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x24, .LC15
	ldr	w0, [x20, 160]
	cmn	w0, #1
	beq	.L88
	mov	w21, 52429
	ldr	x23, .LC19
	mov	w19, 0
	add	x22, x20, 65536
	movk	w21, 0xcccc, lsl 16
	b	.L87
	.p2align 3
.L86:
	ldr	w0, [x20, 160]
	add	w0, w0, 1
	cmp	w0, w19
	bls	.L88
.L87:
	uxtw	x3, w19
	ldr	x4, [x22, 2856]
	lsl	x0, x3, 3
	mov	w2, w19
	sub	x3, x0, x3
	mov	x1, x23
	mov	w0, 1
	add	x4, x4, x3, lsl 4
	mov	x3, x4
	ldr	w4, [x4, 36]
	bl	dprint_vfmw
	umull	x0, w19, w21
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	lsl	w1, w0, 3
	add	w0, w1, w0, lsl 1
	cmp	w19, w0
	add	w19, w19, 1
	bne	.L86
	mov	x1, x24
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 160]
	add	w0, w0, 1
	cmp	w0, w19
	bhi	.L87
.L88:
	mov	x1, x24
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, .LC17
	mov	w0, 1
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	HEVC_PrintNaluArraySlot, .-HEVC_PrintNaluArraySlot
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_PrintCurrSlicelistX
	.type	HEVC_PrintCurrSlicelistX, %function
HEVC_PrintCurrSlicelistX:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	add	x22, x21, 53248
	bl	_mcount
	ldr	x1, .LC21
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, .LC23
	mov	w0, 1
	ldr	w2, [x22, 1920]
	bl	dprint_vfmw
	ldr	w0, [x22, 1920]
	cbz	w0, .L99
	ldr	x25, .LC27
	mov	w19, 0
	ldr	x23, .LC32
	ldr	x24, .LC29
	ldr	x26, .LC31
	.p2align 2
.L98:
	add	x20, x21, x19, uxtw 3
	mov	w2, w19
	mov	x1, x25
	mov	w0, 1
	add	w19, w19, w0
	ldr	x3, [x20, 2368]
	bl	dprint_vfmw
	ldr	x2, [x20, 2368]
	mov	x1, x24
	mov	w0, 1
	cbz	x2, .L96
	ldr	x2, [x2, 32]
	bl	dprint_vfmw
	ldr	x0, [x20, 2368]
	ldr	x0, [x0, 32]
	cbz	x0, .L96
	ldr	w1, [x0, 32]
	ldr	w0, [x21, 244]
	bl	FSP_GetLogicFs
	mov	x2, x0
	mov	x1, x26
	mov	w0, 1
	bl	dprint_vfmw
.L96:
	mov	x1, x23
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x22, 1920]
	cmp	w0, w19
	bhi	.L98
.L99:
	ldr	x1, .LC25
	mov	w0, 1
	ldr	w2, [x22, 1924]
	bl	dprint_vfmw
	ldr	w0, [x22, 1924]
	cbz	w0, .L94
	ldr	x23, .LC32
	mov	w19, 0
	ldr	x25, .LC36
	ldr	x24, .LC29
	ldr	x26, .LC31
	.p2align 2
.L103:
	add	x20, x21, x19, uxtw 3
	mov	w2, w19
	mov	x1, x25
	mov	w0, 1
	add	w19, w19, w0
	ldr	x3, [x20, 2504]
	bl	dprint_vfmw
	ldr	x2, [x20, 2504]
	mov	x1, x24
	mov	w0, 1
	cbz	x2, .L101
	ldr	x2, [x2, 32]
	bl	dprint_vfmw
	ldr	x0, [x20, 2504]
	ldr	x0, [x0, 32]
	cbz	x0, .L101
	ldr	w1, [x0, 32]
	ldr	w0, [x21, 244]
	bl	FSP_GetLogicFs
	mov	x2, x0
	mov	x1, x26
	mov	w0, 1
	bl	dprint_vfmw
.L101:
	mov	x1, x23
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x22, 1924]
	cmp	w0, w19
	bhi	.L103
.L94:
	ldr	x1, .LC34
	mov	w0, 1
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	HEVC_PrintCurrSlicelistX, .-HEVC_PrintCurrSlicelistX
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
.LC32:
	.xword	.LC14
	.align	3
.LC34:
	.xword	.LC33
	.align	3
.LC36:
	.xword	.LC35
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_PrintFrameStoreState
	.type	HEVC_PrintFrameStoreState, %function
HEVC_PrintFrameStoreState:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC40
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x21, 176]
	cbz	w0, .L117
	mov	x23, 4312
	ldr	x26, .LC44
	ldr	x22, .LC46
	mov	w20, 0
	ldr	x25, .LC47
	mov	w24, 712
	movk	x23, 0x1, lsl 16
	.p2align 2
.L116:
	uxtw	x5, w20
	mov	w2, w20
	lsl	x3, x5, 2
	mov	x1, x26
	sub	x3, x3, x5
	mov	w0, 1
	lsl	x4, x3, 4
	mov	w19, 0
	sub	x3, x4, x3
	lsl	x3, x3, 1
	sub	x3, x3, x5
	lsl	x3, x3, 3
	add	x4, x21, x3
	add	x3, x3, 65536
	add	x4, x4, 65536
	add	x3, x21, x3
	umaddl	x27, w20, w24, x23
	add	x3, x3, 3672
	ldrb	w6, [x4, 3674]
	add	x27, x21, x27
	ldrb	w5, [x4, 3673]
	ldr	w4, [x4, 3676]
	bl	dprint_vfmw
.L115:
	ldr	x3, [x27], 8
	mov	w2, w19
	mov	w0, 1
	mov	x1, x22
	add	w19, w19, w0
	bl	dprint_vfmw
	cmp	w19, 4
	bne	.L115
	mov	w0, 1
	mov	x1, x25
	add	w20, w20, w0
	bl	dprint_vfmw
	ldr	w0, [x21, 176]
	cmp	w0, w20
	bhi	.L116
.L117:
	ldr	x1, .LC42
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.size	HEVC_PrintFrameStoreState, .-HEVC_PrintFrameStoreState
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
.LC47:
	.xword	.LC14
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_PrintAPCState
	.type	HEVC_PrintAPCState, %function
HEVC_PrintAPCState:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 3256
	mov	w19, 0
	bl	_mcount
	ldr	x1, .LC49
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w3, [x21, 3248]
	mov	w0, 1
	ldr	w2, [x21, 3252]
	ldr	x1, .LC51
	bl	dprint_vfmw
	ldr	x21, .LC53
	.p2align 2
.L122:
	ldr	w4, [x20, 64]
	mov	w2, w19
	ldr	w5, [x20, 128]
	mov	w0, 1
	ldr	w3, [x20], 4
	mov	x1, x21
	add	w19, w19, w0
	bl	dprint_vfmw
	cmp	w19, 16
	bne	.L122
	ldr	x1, .LC55
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_PrintAPCState, .-HEVC_PrintAPCState
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_PrintDPBState
	.type	HEVC_PrintDPBState, %function
HEVC_PrintDPBState:
	sub	sp, sp, #176
	stp	x29, x30, [sp, 64]
	add	x29, sp, 64
	stp	x19, x20, [sp, 80]
	stp	x21, x22, [sp, 96]
	stp	x23, x24, [sp, 112]
	stp	x25, x26, [sp, 128]
	stp	x27, x28, [sp, 144]
	mov	x19, x0
	mov	x0, x30
	mov	w20, 0
	bl	_mcount
	ldr	x1, .LC57
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, .LC59
	mov	w0, 1
	ldr	w2, [x19, 2680]
	ldr	w3, [x19, 2684]
	ldr	w6, [x19, 2692]
	ldr	w4, [x19, 2696]
	ldr	w5, [x19, 2700]
	bl	dprint_vfmw
	ldr	w0, [x19, 2684]
	cbz	w0, .L129
	.p2align 2
.L128:
	add	x21, x19, x20, uxtw 3
	ldr	w0, [x19, 244]
	ldr	x2, [x21, 2704]
	ldrb	w7, [x2, 2]
	ldr	w1, [x2, 32]
	ldrb	w27, [x2]
	ldrb	w25, [x2, 1]
	ldr	w28, [x2, 4]
	ldr	w22, [x2, 12]
	ldr	w23, [x2, 56]
	str	w7, [x29, 108]
	bl	FSP_GetLogicFs
	ldr	x1, [x21, 2704]
	mov	x24, x0
	ldr	w0, [x19, 244]
	ldr	w1, [x1, 32]
	bl	FSP_GetRef
	mov	w26, w0
	ldr	x1, [x21, 2704]
	ldr	w0, [x19, 244]
	ldr	w1, [x1, 32]
	bl	FSP_GetDisplay
	ldr	x8, [x21, 2704]
	mov	w2, w20
	ldr	w7, [x29, 108]
	mov	w6, w25
	mov	x3, x8
	mov	w5, w28
	ldr	w1, [x8, 236]
	mov	w4, w27
	str	w1, [sp, 48]
	add	w20, w20, 1
	ldr	x1, .LC62
	ldr	w8, [x8, 32]
	str	w8, [sp, 40]
	str	w0, [sp, 32]
	mov	w0, 1
	str	w22, [sp]
	str	w23, [sp, 8]
	str	x24, [sp, 16]
	str	w26, [sp, 24]
	bl	dprint_vfmw
	ldr	w0, [x19, 2684]
	cmp	w0, w20
	bhi	.L128
.L129:
	ldr	x24, .LC60
	mov	w0, 1
	mov	x1, x24
	bl	dprint_vfmw
	ldr	w0, [x19, 2684]
	cbz	w0, .L127
	ldr	x26, .LC66
	mov	w21, 0
	ldr	x28, .LC68
	ldr	x27, .LC70
	ldr	x25, .LC78
	b	.L136
	.p2align 3
.L140:
	mov	x2, x20
	mov	x1, x28
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w2, [x20, 632]
	mov	x1, x27
	mov	w0, 1
	bl	dprint_vfmw
	ldrsb	w2, [x20]
	mov	w0, 1
	ldr	x1, .LC72
	bl	dprint_vfmw
	ldrsb	w2, [x20, 1]
	mov	w0, 1
	ldr	x1, .LC74
	bl	dprint_vfmw
	mov	x2, x23
	mov	x1, x25
	mov	w0, 1
	cbz	x23, .L132
.L141:
	bl	dprint_vfmw
	ldr	w2, [x23, 88]
	mov	w0, 1
	ldr	x1, .LC80
	bl	dprint_vfmw
	mov	x2, x22
	mov	w0, 1
	cbz	x22, .L134
.L142:
	ldr	x1, .LC84
	bl	dprint_vfmw
	ldr	x1, .LC86
	mov	w0, 1
	ldr	w2, [x22, 88]
	bl	dprint_vfmw
	mov	w0, 1
	mov	x1, x24
	add	w21, w21, w0
	bl	dprint_vfmw
	ldr	w0, [x19, 2684]
	cmp	w0, w21
	bls	.L127
.L136:
	add	x22, x19, x21, uxtw 3
	ldr	w0, [x19, 244]
	ldr	x1, [x22, 2704]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	mov	x20, x0
	ldr	x1, [x22, 2704]
	ldr	w0, [x19, 244]
	ldr	w1, [x1, 32]
	bl	FSP_GetDecPhyFs
	mov	x23, x0
	ldr	x1, [x22, 2704]
	ldr	w0, [x19, 244]
	ldr	w1, [x1, 32]
	bl	FSP_GetDispPhyFs
	mov	x22, x0
	mov	w2, w21
	mov	x1, x26
	mov	w0, 1
	bl	dprint_vfmw
	cbnz	x20, .L140
	ldr	x1, .LC76
	mov	w0, 1
	bl	dprint_vfmw
	mov	x2, x23
	mov	x1, x25
	mov	w0, 1
	cbnz	x23, .L141
	.p2align 2
.L132:
	ldr	x1, .LC82
	mov	w0, 1
	bl	dprint_vfmw
	mov	x2, x22
	mov	w0, 1
	cbnz	x22, .L142
	.p2align 2
.L134:
	ldr	x1, .LC88
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 1
	mov	x1, x24
	add	w21, w21, w0
	bl	dprint_vfmw
	ldr	w0, [x19, 2684]
	cmp	w0, w21
	bhi	.L136
.L127:
	ldr	x1, .LC64
	mov	w0, 1
	bl	dprint_vfmw
	add	sp, x29, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.size	HEVC_PrintDPBState, .-HEVC_PrintDPBState
	.align	3
.LC57:
	.xword	.LC56
	.align	3
.LC59:
	.xword	.LC58
	.align	3
.LC60:
	.xword	.LC14
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsSliceUnit
	.type	HEVC_IsSliceUnit, %function
HEVC_IsSliceUnit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 9
	mov	w0, 1
	bls	.L144
	sub	w19, w19, #16
	cmp	w19, 5
	cset	w0, ls
.L144:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsSliceUnit, .-HEVC_IsSliceUnit
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsIUnit
	.type	HEVC_IsIUnit, %function
HEVC_IsIUnit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	sub	w19, w19, #16
	bl	_mcount
	cmp	w19, 5
	ldr	x19, [sp, 16]
	cset	w0, ls
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsIUnit, .-HEVC_IsIUnit
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsNonRefPic
	.type	HEVC_IsNonRefPic, %function
HEVC_IsNonRefPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ands	w19, w19, -7
	mov	w0, 1
	beq	.L148
	cmp	w19, 8
	cset	w0, eq
.L148:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsNonRefPic, .-HEVC_IsNonRefPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsFlushUnit
	.type	HEVC_IsFlushUnit, %function
HEVC_IsFlushUnit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	sub	w19, w19, #16
	bl	_mcount
	cmp	w19, 4
	ldr	x19, [sp, 16]
	cset	w0, ls
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsFlushUnit, .-HEVC_IsFlushUnit
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsPVRFlushPic
	.type	HEVC_IsPVRFlushPic, %function
HEVC_IsPVRFlushPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 53248
	ldr	w0, [x0, 1084]
	cbnz	w0, .L153
	ldr	x1, [x19, 2344]
	ldr	w2, [x1, 640]
	cmp	w2, 1
	beq	.L157
.L152:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L153:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L157:
	ldr	w2, [x1, 636]
	cbnz	w2, .L152
	ldr	w2, [x1, 644]
	cbnz	w2, .L152
	ldr	w0, [x1, 604]
	add	w0, w0, 1024
	cmp	w0, 2048
	cset	w0, hi
	b	.L152
	.size	HEVC_IsPVRFlushPic, .-HEVC_IsPVRFlushPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsEosNal
	.type	HEVC_IsEosNal, %function
HEVC_IsEosNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 2312]
	mov	w1, 32
	bl	BsGet
	mov	w20, w0
	ldr	x0, [x19, 2312]
	mov	w1, 32
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w20, w2
	ccmp	w0, w1, 0, eq
	cset	w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsEosNal, .-HEVC_IsEosNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CalcTileInfo
	.type	HEVC_CalcTileInfo, %function
HEVC_CalcTileInfo:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x22, x0
	mov	x0, x30
	mov	x19, x1
	mov	x20, x2
	mov	x21, x3
	bl	_mcount
	ldr	x0, [x22, 28144]
	ldr	x1, [x22, 28136]
	ldp	w23, w26, [x0, 68]
	ldr	w25, [x1, 148]
	sub	w0, w23, #1
	cmp	w0, 9
	bhi	.L187
	sub	w0, w26, #1
	cmp	w0, 10
	bhi	.L188
	ldr	x24, .LC94
	mov	x2, 512
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x24, 96]
	blr	x3
	ldr	x3, [x24, 96]
	mov	x2, 80
	mov	w1, 0
	mov	x0, x20
	blr	x3
	ldr	x3, [x24, 96]
	mov	x2, 88
	mov	w1, 0
	mov	x0, x21
	blr	x3
	cmp	w23, wzr
	ble	.L168
	sub	w7, w25, #4
	mov	x2, x20
	mov	w1, 0
	mov	w6, 0
.L167:
	str	w1, [x2]
	lsl	w3, w1, w7
	ldr	x0, [x22, 28144]
	add	x0, x0, x6, sxtw 1
	ldrsh	w5, [x0, 176]
	add	w5, w5, w1
	lsl	w1, w5, w7
	sub	w0, w5, #1
	cmp	w3, w1
	str	w0, [x2, 4]
	bge	.L173
	cmp	w3, 255
	bhi	.L169
	uxtb	w4, w6
	b	.L171
	.p2align 3
.L172:
	cmp	w3, 255
	bhi	.L169
.L171:
	strb	w4, [x19, w3, sxtw]
	add	w3, w3, 1
	cmp	w3, w1
	blt	.L172
.L173:
	add	w6, w6, 1
	add	x2, x2, 8
	cmp	w23, w6
	mov	w1, w5
	bne	.L167
.L168:
	cmp	w26, wzr
	ble	.L165
	sub	w25, w25, #4
	mov	w0, 0
	mov	w5, 0
.L175:
	str	w0, [x21]
	lsl	w3, w0, w25
	ldr	x1, [x22, 28144]
	add	x1, x1, x5, sxtw 1
	ldrsh	w4, [x1, 216]
	add	w4, w4, w0
	lsl	w0, w4, w25
	sub	w1, w4, #1
	cmp	w3, w0
	str	w1, [x21, 4]
	bge	.L180
	cmp	w3, 255
	bhi	.L176
	uxtb	w2, w5
	add	x1, x19, 256
	b	.L178
	.p2align 3
.L179:
	cmp	w3, 255
	bhi	.L176
.L178:
	strb	w2, [x1, w3, sxtw]
	add	w3, w3, 1
	cmp	w3, w0
	blt	.L179
.L180:
	add	w5, w5, 1
	add	x21, x21, 8
	cmp	w26, w5
	mov	w0, w4
	bne	.L175
.L165:
	mov	w0, 0
	b	.L161
	.p2align 3
.L169:
	ldr	x2, .LC89
	mov	w0, 1
	ldr	x1, .LC96
	add	x2, x2, 136
	bl	dprint_vfmw
	mov	w0, -1
.L161:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L176:
	ldr	x2, .LC89
	mov	w0, 1
	ldr	x1, .LC98
	add	x2, x2, 136
	bl	dprint_vfmw
	mov	w0, -1
	b	.L161
.L188:
	ldr	x2, .LC89
	mov	w4, 11
	ldr	x1, .LC93
	mov	w3, w26
	add	x2, x2, 136
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L161
.L187:
	ldr	x2, .LC89
	mov	w4, 10
	ldr	x1, .LC91
	mov	w3, w23
	add	x2, x2, 136
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L161
	.size	HEVC_CalcTileInfo, .-HEVC_CalcTileInfo
	.align	3
.LC89:
	.xword	.LANCHOR1
	.align	3
.LC91:
	.xword	.LC90
	.align	3
.LC93:
	.xword	.LC92
	.align	3
.LC94:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC96:
	.xword	.LC95
	.align	3
.LC98:
	.xword	.LC97
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_VPSEqual
	.type	HEVC_VPSEqual, %function
HEVC_VPSEqual:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cbz	x19, .L212
	cbz	x20, .L213
	ldp	w3, w1, [x19, 12]
	ldp	w2, w0, [x20, 12]
	ldrb	w5, [x19, 1]
	add	x6, x19, 12288
	cmp	w3, w2
	ldrb	w4, [x20, 1]
	ccmp	w1, w0, 0, eq
	ldr	w8, [x19, 20]
	ldr	w0, [x20, 20]
	cset	w2, eq
	cmp	w5, w4
	ldr	w4, [x19, 24]
	ccmp	w0, w8, 0, eq
	add	x7, x20, 12288
	cset	w0, eq
	ldr	w3, [x6, 424]
	and	w0, w2, w0
	ldr	w2, [x20, 24]
	ldrb	w5, [x6, 148]
	add	x1, x6, 149
	cmp	w4, w2
	ldr	w4, [x19, 8]
	ldr	w2, [x20, 8]
	ldr	w9, [x6, 420]
	ccmp	w4, w2, 0, eq
	ldr	w4, [x7, 420]
	cset	w2, eq
	and	w0, w2, w0
	ldr	w2, [x7, 424]
	cmp	w3, w2
	ldrb	w3, [x7, 148]
	add	x2, x7, 149
	ccmp	w5, w3, 0, eq
	add	x5, x6, 181
	cset	w3, eq
	cmp	w9, w4
	and	w0, w3, w0
	cset	w3, eq
	and	w0, w3, w0
	.p2align 2
.L193:
	ldrb	w4, [x1], 1
	ldrb	w3, [x2], 1
	cmp	w4, w3
	cset	w3, eq
	cmp	x1, x5
	and	w0, w0, w3
	bne	.L193
	ldrb	w4, [x6, 181]
	sub	w8, w8, #1
	ldrb	w2, [x7, 181]
	ldrb	w5, [x6, 182]
	ldrb	w3, [x7, 182]
	cmp	w4, w2
	ldrb	w4, [x6, 183]
	ldrb	w2, [x7, 183]
	ccmp	w5, w3, 0, eq
	ldrb	w1, [x7, 184]
	cset	w3, eq
	ldrb	w5, [x6, 184]
	cmp	w4, w2
	ldrb	w4, [x19, 7]
	ldrb	w2, [x20, 7]
	ccmp	w5, w1, 0, eq
	cset	w1, eq
	cmp	w4, w2
	and	w1, w3, w1
	cset	w3, eq
	mov	w4, 0
	and	w3, w3, w1
	and	w0, w3, w0
	.p2align 2
.L194:
	ubfiz	x1, x4, 2, 32
	add	w4, w4, 1
	add	x2, x19, x1
	add	x1, x20, x1
	ldr	w5, [x2, 52]
	ldr	w3, [x1, 52]
	ldr	w6, [x1, 84]
	ldr	w7, [x2, 84]
	cmp	w5, w3
	ldr	w3, [x1, 116]
	cset	w1, eq
	ldr	w5, [x2, 116]
	cmp	w7, w6
	cset	w2, eq
	cmp	w5, w3
	and	w1, w1, w2
	cset	w3, eq
	cmp	w4, w8
	and	w1, w3, w1
	and	w0, w0, w1
	bls	.L194
	ldp	w5, w7, [x19, 28]
	ldp	w2, w1, [x20, 28]
	cmp	w2, w5
	ccmp	w1, w7, 0, eq
	cset	w3, eq
	and	w3, w3, w0
	cbz	w7, .L195
	mov	w6, 1
	.p2align 2
.L196:
	mov	w2, 0
	uxtw	x1, w6
	.p2align 2
.L197:
	add	x0, x1, x2, uxtw
	add	w2, w2, 1
	add	x0, x0, 36
	lsl	x0, x0, 2
	add	x4, x19, x0
	add	x0, x20, x0
	ldr	w4, [x4, 4]
	ldr	w0, [x0, 4]
	cmp	w4, w0
	cset	w0, eq
	cmp	w2, w5
	and	w3, w3, w0
	bls	.L197
	add	w6, w6, 1
	cmp	w6, w7
	bls	.L196
.L195:
	ldrb	w1, [x20, 6]
	ldrb	w0, [x19, 6]
	cmp	w1, w0
	cset	w1, eq
	and	w3, w1, w3
	cbz	w0, .L198
	ldp	w1, w0, [x20, 36]
	ldp	w2, w5, [x19, 36]
	ldrb	w4, [x20, 5]
	cmp	w2, w1
	ldrb	w2, [x19, 5]
	cset	w1, eq
	cmp	w5, w0
	cset	w0, eq
	cmp	w4, w2
	and	w1, w1, w0
	cset	w0, eq
	and	w0, w1, w0
	and	w3, w0, w3
	cbnz	w2, .L214
.L199:
	ldr	w0, [x20, 48]
	ldr	w1, [x19, 48]
	cmp	w1, w0
	cset	w0, eq
	and	w3, w0, w3
.L198:
	eor	w3, w3, 1
	neg	w0, w3
.L191:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L214:
	ldr	w0, [x20, 44]
	ldr	w1, [x19, 44]
	cmp	w1, w0
	cset	w0, eq
	and	w3, w3, w0
	b	.L199
.L213:
	ldr	x2, .LC100
	mov	w3, 1884
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L191
.L212:
	ldr	x2, .LC100
	mov	w3, 1883
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L191
	.size	HEVC_VPSEqual, .-HEVC_VPSEqual
	.align	3
.LC100:
	.xword	.LC99
	.align	3
.LC102:
	.xword	.LC101
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_More_Data_In_Payload
	.type	HEVC_Sei_More_Data_In_Payload, %function
HEVC_Sei_More_Data_In_Payload:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	lsl	w20, w20, 3
	ldr	x1, [x19, 2312]
	mov	x0, x1
	ldp	w2, w19, [x1, 32]
	sub	w19, w2, w19
	bl	BsIsByteAligned
	cmp	w20, w19
	ccmp	w0, wzr, 4, ne
	cset	w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_Sei_More_Data_In_Payload, .-HEVC_Sei_More_Data_In_Payload
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Payload_Extension_Present
	.type	HEVC_Sei_Payload_Extension_Present, %function
HEVC_Sei_Payload_Extension_Present:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	lsl	w20, w20, 3
	ldr	x3, .LC103
	mov	x2, 40
	ldr	x1, [x19, 2312]
	add	x0, x29, x2
	ldr	x3, [x3, 104]
	blr	x3
	ldp	w0, w1, [x29, 72]
	sub	w0, w0, w1
	mov	w1, 0
	cmp	w0, w20
	bcc	.L223
.L217:
	ldp	x19, x20, [sp, 16]
	mov	w0, w1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L223:
	mov	w1, 1
	add	x0, x29, 40
	bl	BsShow
	mov	w1, 1
	cbz	w0, .L217
	add	x0, x29, 40
	bl	BsSkip
	ldp	w0, w19, [x29, 72]
	sub	w19, w0, w19
	b	.L218
	.p2align 3
.L219:
	bl	BsSkip
	ldp	w0, w19, [x29, 72]
	sub	w19, w0, w19
.L218:
	mov	w1, 1
	add	x0, x29, 40
	bl	BsShow
	cmp	w19, w20
	mov	w1, 1
	cset	w2, cc
	cmp	w0, wzr
	cset	w3, eq
	add	x0, x29, 40
	tst	w2, w3
	bne	.L219
	mov	w1, w2
	ldp	x19, x20, [sp, 16]
	mov	w0, w1
	ldp	x29, x30, [sp], 80
	ret
	.size	HEVC_Sei_Payload_Extension_Present, .-HEVC_Sei_Payload_Extension_Present
	.align	3
.LC103:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Decoded_Picture_Hash
	.type	HEVC_Sei_Decoded_Picture_Hash, %function
HEVC_Sei_Decoded_Picture_Hash:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	cmp	x21, xzr
	ccmp	x22, xzr, 4, ne
	beq	.L232
	ldr	x0, [x21, 2312]
	mov	w1, 8
	mov	x20, x22
	add	x23, x22, 48
	bl	BsGet
	mov	w1, w0
	stp	w0, w0, [x22, 48]
	cbnz	w1, .L226
.L235:
	mov	x19, 0
	.p2align 2
.L227:
	ldr	x0, [x21, 2312]
	mov	w1, 8
	bl	BsGet
	strb	w0, [x20, x19]
	add	x19, x19, 1
	cmp	x19, 16
	bne	.L227
.L228:
	add	x20, x20, 16
	cmp	x20, x23
	beq	.L233
.L237:
	ldr	w1, [x22, 52]
	cbz	w1, .L235
.L226:
	cmp	w1, 1
	beq	.L236
	cmp	w1, 2
	bne	.L228
	ldr	x0, [x21, 2312]
	mov	w1, 32
	add	x20, x20, 16
	bl	BsGet
	strb	w0, [x20, -13]
	lsr	w3, w0, 24
	lsr	w2, w0, 16
	strb	w3, [x20, -16]
	lsr	w1, w0, 8
	strb	w2, [x20, -15]
	strb	w1, [x20, -14]
	cmp	x20, x23
	bne	.L237
	.p2align 2
.L233:
	mov	w0, 0
.L225:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L236:
	ldr	x0, [x21, 2312]
	mov	w1, 16
	bl	BsGet
	lsr	w1, w0, 8
	strb	w0, [x20, 1]
	strb	w1, [x20]
	b	.L228
.L232:
	mov	w0, -1
	b	.L225
	.size	HEVC_Sei_Decoded_Picture_Hash, .-HEVC_Sei_Decoded_Picture_Hash
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Region_Refresh_Info
	.type	HEVC_Sei_Region_Refresh_Info, %function
HEVC_Sei_Region_Refresh_Info:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	x20, xzr
	ccmp	x19, xzr, 4, ne
	beq	.L240
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19]
	mov	w1, 0
.L239:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L240:
	mov	w1, -1
	b	.L239
	.size	HEVC_Sei_Region_Refresh_Info, .-HEVC_Sei_Region_Refresh_Info
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Mastering_Display_Colour_Volume
	.type	HEVC_Sei_Mastering_Display_Colour_Volume, %function
HEVC_Sei_Mastering_Display_Colour_Volume:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	x20, xzr
	ccmp	x19, xzr, 4, ne
	beq	.L243
	mov	w0, 1
	mov	x1, x19
	strb	w0, [x19]
	mov	x0, x20
	bl	HEVC_Sei_Mastering_Display_Colour_Volume.part.11
.L242:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L243:
	mov	w0, -1
	b	.L242
	.size	HEVC_Sei_Mastering_Display_Colour_Volume, .-HEVC_Sei_Mastering_Display_Colour_Volume
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Content_Light_Level_Information
	.type	HEVC_Sei_Content_Light_Level_Information, %function
HEVC_Sei_Content_Light_Level_Information:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	x20, xzr
	ccmp	x19, xzr, 4, ne
	beq	.L246
	mov	w0, 1
	strb	w0, [x19]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	strh	w0, [x19, 2]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	strh	w0, [x19, 4]
	mov	w1, 0
.L245:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L246:
	mov	w1, -1
	b	.L245
	.size	HEVC_Sei_Content_Light_Level_Information, .-HEVC_Sei_Content_Light_Level_Information
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_HLGHDR_Comapitibility_Info
	.type	HEVC_Sei_HLGHDR_Comapitibility_Info, %function
HEVC_Sei_HLGHDR_Comapitibility_Info:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	x20, xzr
	ccmp	x19, xzr, 4, ne
	beq	.L249
	ldr	x0, [x20, 2312]
	mov	w1, 8
	bl	BsGet
	strb	w0, [x19]
	mov	w1, 0
.L248:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L249:
	mov	w1, -1
	b	.L248
	.size	HEVC_Sei_HLGHDR_Comapitibility_Info, .-HEVC_Sei_HLGHDR_Comapitibility_Info
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	.type	HEVC_Sei_Temporal_Sub_Layer_Zero_Index, %function
HEVC_Sei_Temporal_Sub_Layer_Zero_Index:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	x19, xzr
	ccmp	x20, xzr, 4, ne
	beq	.L252
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x20]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 4]
	ldr	x0, [x19, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L251:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L252:
	mov	w0, -1
	b	.L251
	.size	HEVC_Sei_Temporal_Sub_Layer_Zero_Index, .-HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Display_Orientation
	.type	HEVC_Sei_Display_Orientation, %function
HEVC_Sei_Display_Orientation:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	x19, xzr
	ccmp	x20, xzr, 4, ne
	beq	.L256
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 2]
	cbz	w0, .L257
.L255:
	ldr	x0, [x19, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L254:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L257:
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 1]
	mov	w1, 16
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 3]
	b	.L255
	.p2align 3
.L256:
	mov	w0, -1
	b	.L254
	.size	HEVC_Sei_Display_Orientation, .-HEVC_Sei_Display_Orientation
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_ParsePts_Init
	.type	HEVC_Sei_ParsePts_Init, %function
HEVC_Sei_ParsePts_Init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x3, .LC104
	add	x0, x19, 24576
	mov	x2, 80
	mov	w1, 0
	add	x0, x0, 3472
	ldr	x3, [x3, 96]
	blr	x3
	str	wzr, [x19, 5228]
	str	wzr, [x19, 5232]
	str	wzr, [x19, 5236]
	str	wzr, [x19, 5240]
	str	xzr, [x19, 7328]
	str	xzr, [x19, 7336]
	str	xzr, [x19, 7344]
	str	xzr, [x19, 7352]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_Sei_ParsePts_Init, .-HEVC_Sei_ParsePts_Init
	.align	3
.LC104:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Get_Dpb_Output_Time
	.type	HEVC_Sei_Get_Dpb_Output_Time, %function
HEVC_Sei_Get_Dpb_Output_Time:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	mov	x20, x2
	bl	_mcount
	add	x0, x19, 53248
	add	x3, x19, 24576
	ldr	x2, [x19, 2336]
	add	x5, x3, 3472
	mov	w7, 2024
	ldr	w6, [x21, 12]
	ldr	w4, [x0, 1836]
	mov	w1, 26164
	ldr	x0, [x19, 2328]
	smaddl	x2, w4, w7, x2
	ldr	w4, [x5, 4]
	cmp	w4, 1
	ldr	w4, [x2, 32]
	str	w6, [x5, 24]
	ldr	w2, [x21, 16]
	str	w2, [x5, 28]
	smaddl	x0, w4, w1, x0
	ldr	w4, [x20, 76]
	str	w4, [x5, 48]
	ldr	w1, [x20, 80]
	str	w1, [x5, 52]
	ldr	w1, [x0, 56]
	add	x0, x0, x1, lsl 2
	ldrb	w2, [x0, 412]
	beq	.L267
	ldrb	w0, [x5, 73]
	add	x19, x19, 4096
	cbnz	w0, .L268
	ldr	w0, [x19, 1164]
	ldr	x1, [x19, 3232]
	add	x0, x0, x1
	str	x0, [x19, 3256]
.L261:
	add	x19, x19, 1152
	add	x3, x3, 3472
	str	x0, [x19, 2096]
	strb	wzr, [x3, 73]
	ldrb	w1, [x20, 9]
	cbnz	w1, .L269
.L265:
	ldr	w1, [x19, 16]
	uxtw	x4, w4
	add	x0, x1, x0
	sub	x0, x0, x2, uxtb
	str	x0, [x3, 40]
	mul	x0, x4, x0
	str	x0, [x3, 32]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L268:
	ldrb	w0, [x19, 89]
	cbnz	w0, .L270
	ldr	x1, [x19, 3232]
	ldr	x6, [x19, 3240]
.L264:
	ldr	w7, [x19, 1164]
	add	x0, x7, x1
	str	x0, [x19, 3232]
	cmp	x0, x6
	str	x0, [x19, 3256]
	bge	.L261
	add	x0, x7, x6
	add	x19, x19, 1152
	str	x0, [x19, 2080]
	add	x3, x3, 3472
	str	x0, [x19, 2104]
	str	x0, [x19, 2096]
	strb	wzr, [x3, 73]
	ldrb	w1, [x20, 9]
	cbz	w1, .L265
.L269:
	ldr	w1, [x19, 16]
	uxtw	x4, w4
	add	x1, x1, x0
	asr	x1, x1, 1
	sub	x0, x1, x2, uxtb
	str	x0, [x3, 40]
	mul	x0, x4, x0
	str	x0, [x3, 32]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L267:
	add	x19, x19, 4096
	ldrsw	x0, [x19, 1132]
	str	x0, [x19, 3232]
	str	x0, [x19, 3256]
	b	.L261
	.p2align 3
.L270:
	ldr	x6, [x19, 3240]
	mov	x1, x6
	b	.L264
	.size	HEVC_Sei_Get_Dpb_Output_Time, .-HEVC_Sei_Get_Dpb_Output_Time
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Get_Last_IDR_Frame_Info
	.type	HEVC_Get_Last_IDR_Frame_Info, %function
HEVC_Get_Last_IDR_Frame_Info:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	sub	w0, w20, #19
	cmp	w0, 1
	bls	.L278
.L272:
	sub	w20, w20, #6
	cmp	w20, 3
	bls	.L271
	ldr	x0, [x19, 7352]
	str	x0, [x19, 7336]
.L271:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L278:
	ldr	x0, [x19, 2344]
	ldr	x0, [x0, 56]
	cmn	x0, #1
	beq	.L273
	ldr	x1, [x19, 28088]
	str	x0, [x19, 28104]
	str	x1, [x19, 28112]
.L273:
	add	x0, x19, 24576
	mov	w1, 1
	strb	w1, [x0, 3544]
	b	.L272
	.size	HEVC_Get_Last_IDR_Frame_Info, .-HEVC_Get_Last_IDR_Frame_Info
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Get_Frame_Pts
	.type	HEVC_Sei_Get_Frame_Pts, %function
HEVC_Sei_Get_Frame_Pts:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 24576
	ldr	x3, [x19, 28088]
	ldr	x6, [x19, 28104]
	ldr	x7, [x19, 28112]
	ldr	w4, [x1, 3520]
	ldr	w5, [x1, 3524]
	cbz	w4, .L282
	mov	w1, w4
	mov	w2, w5
	b	.L281
	.p2align 3
.L283:
	mov	w1, w0
.L281:
	udiv	w0, w2, w1
	msub	w0, w0, w1, w2
	mov	w2, w1
	cbnz	w0, .L283
.L280:
	udiv	w4, w4, w1
	sub	x0, x3, x7
	lsl	w2, w0, 6
	add	x19, x19, 53248
	sub	w2, w2, w0
	lsl	w2, w2, 1
	sub	w0, w2, w0
	lsl	w0, w0, 3
	udiv	w1, w5, w1
	mul	w0, w0, w4
	udiv	w1, w0, w1
	add	x1, x1, x6
	str	x1, [x19, 1736]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L282:
	mov	w1, w5
	b	.L280
	.size	HEVC_Sei_Get_Frame_Pts, .-HEVC_Sei_Get_Frame_Pts
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_User_Data_Unregistered
	.type	HEVC_Sei_User_Data_Unregistered, %function
HEVC_Sei_User_Data_Unregistered:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x24, x0
	mov	x0, x30
	mov	x22, x1
	mov	w26, w2
	bl	_mcount
	ldr	x21, .LC107
	mov	w2, 1
	cmp	w26, 15
	strb	w2, [x22, 28]
	ldp	x4, x5, [x21, 160]
	stp	x4, x5, [x29, 80]
	ldr	w3, [x21, 184]
	ldp	x4, x5, [x21, 192]
	ldr	w0, [x21, 216]
	ldr	x6, [x21, 176]
	ldr	x1, [x21, 208]
	stp	x4, x5, [x29, 112]
	str	x6, [x29, 96]
	str	w3, [x29, 104]
	str	x1, [x29, 128]
	str	w0, [x29, 136]
	bls	.L303
	add	x19, x22, 8
	add	x21, x21, 225
	sub	w20, w26, #1
	add	x25, x22, 24
	mov	w23, 241
	b	.L288
	.p2align 3
.L310:
	ldrb	w23, [x21], 1
.L288:
	ldr	x0, [x24, 2312]
	mov	w1, 8
	add	x19, x19, 1
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, w23
	strb	w0, [x19, -1]
	beq	.L286
	strb	wzr, [x22, 28]
	str	w20, [x22, 24]
.L286:
	cmp	x19, x25
	sub	w20, w20, #1
	bne	.L310
	ldrb	w19, [x22, 28]
	cmp	w19, 1
	beq	.L311
	sub	w26, w26, #16
	str	w26, [x22, 24]
	cbz	w26, .L300
	mov	w19, 0
	.p2align 2
.L301:
	ldr	x0, [x24, 2312]
	mov	w1, 8
	add	w19, w19, 1
	bl	BsGet
	ldr	w0, [x22, 24]
	cmp	w0, w19
	bhi	.L301
.L302:
	mov	w0, 0
.L285:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L300:
	str	xzr, [x22]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 144
	ret
.L311:
	ldr	x0, [x24, 2312]
	mov	w1, 5
	bl	BsGet
	uxtb	w20, w0
	ldr	x0, [x24, 2312]
	mov	w1, 3
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 3
	ccmp	w20, 6, 0, eq
	beq	.L312
	cmp	w20, 2
	cset	w1, eq
	cmp	w0, wzr
	cset	w2, eq
	tst	w1, w2
	bne	.L313
	cmp	w0, 1
	cset	w0, eq
	tst	w1, w0
	bne	.L314
	sub	w26, w26, #18
	str	w26, [x22, 24]
.L293:
	mov	w19, 0
	cbz	w26, .L300
	.p2align 2
.L306:
	ldr	x0, [x24, 2312]
	mov	w1, 8
	add	w19, w19, 1
	bl	BsGet
	ldr	w0, [x22, 24]
	cmp	w0, w19
	bhi	.L306
	b	.L302
.L314:
	ldr	x0, [x24, 2312]
	mov	w1, 8
	sub	w26, w26, #19
	bl	BsGet
	mov	w19, w0
	ldr	x0, [x24, 2312]
	mov	w1, 8
	bl	BsGet
	ubfiz	w0, w0, 8, 8
	add	w19, w0, w19, uxtb
	str	w26, [x22, 24]
	str	w19, [x22, 68]
	b	.L293
.L303:
	mov	w0, -1
	b	.L285
.L312:
	ldr	x0, [x24, 2312]
	mov	w1, 2
	bl	BsGet
	strb	w0, [x22, 35]
	mov	w1, 2
	ldr	x0, [x24, 2312]
	bl	BsGet
	strb	w0, [x22, 34]
	mov	w1, w19
	ldr	x0, [x24, 2312]
	bl	BsGet
	strb	w0, [x22, 33]
	mov	w1, w19
	ldr	x0, [x24, 2312]
	bl	BsGet
	strb	w0, [x22, 32]
	mov	w1, 2
	ldr	x0, [x24, 2312]
	bl	BsGet
	ldrb	w0, [x22, 33]
	mov	x2, 28
	ldr	x3, .LC108
	cbnz	w0, .L291
	ldr	x3, [x3, 104]
	add	x1, x29, 80
	add	x0, x22, 40
	blr	x3
.L292:
	ldr	x0, [x24, 2312]
	mov	w1, 8
	sub	w26, w26, #21
	bl	BsGet
	uxtb	w0, w0
	lsl	w2, w0, 3
	mov	w1, 8
	sub	w2, w2, w0
	add	w0, w0, w2, lsl 3
	sub	w0, w0, w2
	str	w0, [x22, 36]
	ldr	x0, [x24, 2312]
	bl	BsGet
	mov	w19, w0
	ldr	x0, [x24, 2312]
	mov	w1, 8
	bl	BsGet
	ubfiz	w0, w0, 8, 8
	add	w19, w0, w19, uxtb
	str	w26, [x22, 24]
	str	w19, [x22, 68]
	b	.L293
.L313:
	ldr	x0, [x24, 2312]
	mov	w1, 2
	bl	BsGet
	strb	w0, [x22, 35]
	mov	w1, 2
	ldr	x0, [x24, 2312]
	bl	BsGet
	strb	w0, [x22, 34]
	mov	w1, w19
	ldr	x0, [x24, 2312]
	bl	BsGet
	strb	w0, [x22, 33]
	mov	w1, w19
	ldr	x0, [x24, 2312]
	bl	BsGet
	strb	w0, [x22, 32]
	mov	w1, 2
	ldr	x0, [x24, 2312]
	bl	BsGet
	ldrb	w0, [x22, 33]
	mov	x2, 28
	ldr	x3, .LC108
	cbnz	w0, .L295
	ldr	x3, [x3, 104]
	add	x1, x29, 80
	add	x0, x22, 40
	blr	x3
.L296:
	ldr	x0, [x24, 2312]
	mov	w1, 8
	sub	w26, w26, #19
	bl	BsGet
	uxtb	w0, w0
	lsl	w1, w0, 3
	str	w26, [x22, 24]
	sub	w1, w1, w0
	add	w0, w0, w1, lsl 3
	sub	w0, w0, w1
	str	w0, [x22, 36]
	b	.L293
.L291:
	ldr	x3, [x3, 104]
	add	x1, x29, 112
	add	x0, x22, 40
	blr	x3
	b	.L292
.L295:
	ldr	x3, [x3, 104]
	add	x1, x29, 112
	add	x0, x22, 40
	blr	x3
	b	.L296
	.size	HEVC_Sei_User_Data_Unregistered, .-HEVC_Sei_User_Data_Unregistered
	.align	3
.LC107:
	.xword	.LANCHOR1
	.align	3
.LC108:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsRefListWrong
	.type	HEVC_IsRefListWrong, %function
HEVC_IsRefListWrong:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 53248
	bl	_mcount
	ldr	w3, [x21, 1920]
	cbnz	w3, .L356
.L316:
	mov	w0, 0
.L354:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L356:
	ldr	x0, [x20, 2368]
	cbz	x0, .L338
	ldr	x0, [x0, 32]
	mov	w19, 0
	cbnz	x0, .L322
	b	.L318
	.p2align 3
.L358:
	ldr	x1, [x0, 640]
	cbz	x1, .L323
	ldr	w3, [x21, 1920]
	add	w19, w19, 1
	cmp	w3, w19
	add	x0, x20, x19, sxtw 3
	bls	.L357
	ldr	x0, [x0, 2368]
	cbz	x0, .L317
	ldr	x0, [x0, 32]
	cbz	x0, .L318
.L322:
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_GetLogicFs
	cbnz	x0, .L358
.L323:
	add	x20, x20, x19, sxtw 3
	ldr	x1, .LC116
	mov	x4, x0
	mov	w2, w19
	mov	w0, 1
	ldr	x3, [x20, 2368]
	ldr	x3, [x3, 32]
	bl	dprint_vfmw
	mov	w0, -1
.L362:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L357:
	cbz	w3, .L316
	ldr	w0, [x21, 1924]
	cbz	w0, .L316
	ldr	x0, [x20, 2504]
	cbz	x0, .L340
	ldr	x0, [x0, 32]
	mov	w19, 0
	cbnz	x0, .L333
	b	.L330
	.p2align 3
.L361:
	ldr	x1, [x0, 640]
	cbz	x1, .L334
	ldr	w0, [x21, 1924]
	add	w19, w19, 1
	cmp	w0, w19
	add	x1, x20, x19, sxtw 3
	bls	.L316
	ldr	x0, [x1, 2504]
	cbz	x0, .L359
	ldr	x0, [x0, 32]
	cbz	x0, .L360
.L333:
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_GetLogicFs
	cbnz	x0, .L361
.L334:
	ldr	x1, .LC122
	mov	x3, x0
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L354
.L338:
	mov	w19, 0
	.p2align 2
.L317:
	ldr	x1, .LC112
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L362
	.p2align 3
.L318:
	ldr	x1, .LC114
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L362
.L360:
	ldr	w3, [x21, 1920]
.L330:
	ldr	x1, .LC120
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L354
.L359:
	ldr	w3, [x21, 1920]
.L329:
	ldr	x1, .LC118
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L354
.L340:
	mov	w19, 0
	b	.L329
	.size	HEVC_IsRefListWrong, .-HEVC_IsRefListWrong
	.align	3
.LC112:
	.xword	.LC111
	.align	3
.LC114:
	.xword	.LC113
	.align	3
.LC116:
	.xword	.LC115
	.align	3
.LC118:
	.xword	.LC117
	.align	3
.LC120:
	.xword	.LC119
	.align	3
.LC122:
	.xword	.LC121
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetUnRefPicWithMinPoc
	.type	HEVC_GetUnRefPicWithMinPoc, %function
HEVC_GetUnRefPicWithMinPoc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	mov	w21, -1
	bl	_mcount
	str	w21, [x20]
	ldr	w3, [x19, 2684]
	cmp	w3, 16
	bhi	.L364
	mov	w4, 2147483647
	mov	w0, 0
	cbz	w3, .L369
	.p2align 2
.L372:
	add	x2, x19, x0, uxtw 3
	ldr	x2, [x2, 2704]
	ldr	w5, [x2, 12]
	cmp	w5, w4
	bge	.L367
	ldr	w2, [x2]
	tst	w2, 16776960
	bne	.L367
	str	w0, [x20]
	mov	w4, w5
	ldr	w3, [x19, 2684]
.L367:
	add	w0, w0, 1
	cmp	w3, w0
	bhi	.L372
	ldr	w3, [x20]
	cmn	w3, #1
	beq	.L369
	ldr	x2, .LC123
	mov	w0, 1
	ldr	x1, .LC129
	add	x2, x2, 240
	bl	dprint_vfmw
	mov	w0, 0
.L366:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L369:
	ldr	x2, .LC123
	mov	w0, 1
	ldr	x1, .LC125
	add	x2, x2, 240
	bl	dprint_vfmw
	mov	w0, -1
	b	.L366
.L364:
	ldr	x2, .LC123
	mov	w0, 1
	ldr	x1, .LC127
	add	x2, x2, 240
	bl	dprint_vfmw
	mov	w0, w21
	b	.L366
	.size	HEVC_GetUnRefPicWithMinPoc, .-HEVC_GetUnRefPicWithMinPoc
	.align	3
.LC123:
	.xword	.LANCHOR1
	.align	3
.LC125:
	.xword	.LC124
	.align	3
.LC127:
	.xword	.LC126
	.align	3
.LC129:
	.xword	.LC128
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IncreaseDPBSize
	.type	HEVC_IncreaseDPBSize, %function
HEVC_IncreaseDPBSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w3, [x19, 2680]
	cmp	w3, 16
	bhi	.L382
	ldr	w0, [x19, 244]
	bl	FSP_GetTotalValidFsNum
	add	x1, x19, 53248
	mov	w3, 1
	ldr	w2, [x1, 1088]
	cmp	w2, 1920
	bls	.L383
.L378:
	subs	w0, w0, w3
	ldr	x2, .LC130
	csel	w0, w0, wzr, pl
	cmp	w0, wzr
	ble	.L379
	ldr	w3, [x19, 2680]
	add	w3, w3, 1
	cmp	w3, w0
	bhi	.L379
	ldr	x1, .LC134
	add	x2, x2, 272
	mov	w0, 13
	str	w3, [x19, 2680]
	bl	dprint_vfmw
	mov	w0, 0
.L377:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L383:
	ldr	w1, [x1, 1092]
	mov	w3, 4
	cmp	w1, 1088
	csinc	w3, w3, wzr, ls
	b	.L378
	.p2align 3
.L379:
	ldr	x1, .LC136
	mov	w3, w0
	add	x2, x2, 272
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L377
	.p2align 3
.L382:
	ldr	x2, .LC130
	mov	w4, 17
	ldr	x1, .LC132
	mov	w0, 1
	add	x2, x2, 272
	bl	dprint_vfmw
	mov	w1, 17
	mov	w0, -1
	str	w1, [x19, 2680]
	b	.L377
	.size	HEVC_IncreaseDPBSize, .-HEVC_IncreaseDPBSize
	.align	3
.LC130:
	.xword	.LANCHOR1
	.align	3
.LC132:
	.xword	.LC131
	.align	3
.LC134:
	.xword	.LC133
	.align	3
.LC136:
	.xword	.LC135
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_PicTypeStatistic
	.type	HEVC_PicTypeStatistic, %function
HEVC_PicTypeStatistic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 53248
	ldr	w1, [x0, 1832]
	sub	w2, w1, #16
	cmp	w2, 5
	bhi	.L385
	str	w1, [x0, 1076]
	str	wzr, [x0, 1084]
.L384:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L385:
	ldr	w1, [x0, 1076]
	sub	w1, w1, #16
	cmp	w1, 5
	bls	.L384
	ldr	w1, [x0, 1820]
	cmp	w1, 1
	beq	.L387
	cmp	w1, 2
	beq	.L388
	cbnz	w1, .L384
	mov	w1, 2
	str	w1, [x0, 1084]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L387:
	ldr	w2, [x0, 1084]
	cmp	w2, 2
	beq	.L384
	str	w1, [x0, 1084]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L388:
	ldr	w1, [x0, 1084]
	sub	w1, w1, #1
	cmp	w1, 1
	bls	.L384
	str	wzr, [x0, 1084]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_PicTypeStatistic, .-HEVC_PicTypeStatistic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsReferenceNalu
	.type	HEVC_IsReferenceNalu, %function
HEVC_IsReferenceNalu:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 53248
	bl	_mcount
	ldr	w1, [x19, 1832]
	cmp	w1, 15
	bhi	.L394
	mov	w0, 1
	tbnz	x1, 0, .L395
.L394:
	sub	w1, w1, #16
	cmp	w1, 7
	cset	w0, ls
.L395:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsReferenceNalu, .-HEVC_IsReferenceNalu
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WriteQmatrix_4x4
	.type	HEVC_WriteQmatrix_4x4, %function
HEVC_WriteQmatrix_4x4:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x2
	mov	x21, x1
	ubfiz	x20, x20, 6, 32
	bl	_mcount
	add	x1, x21, x20
	ldrb	w4, [x1, 584]
	ldr	w3, [x1, 592]
	ldr	w2, [x1, 588]
	ldrb	w0, [x1, 580]
	ubfiz	w2, w2, 8, 8
	orr	w2, w2, w4, lsl 16
	orr	w0, w0, w3, lsl 24
	orr	w0, w2, w0
	str	w0, [x19]
	ldrb	w4, [x1, 593]
	ldrb	w2, [x1, 585]
	ldr	w3, [x1, 588]
	ldrb	w0, [x1, 581]
	lsl	w2, w2, 16
	and	w3, w3, 65280
	orr	w2, w2, w4, lsl 24
	orr	w0, w0, w3
	orr	w0, w2, w0
	str	w0, [x19, 4]
	ldrb	w4, [x1, 594]
	ldrb	w2, [x1, 590]
	ldr	w3, [x1, 584]
	ldrb	w0, [x1, 582]
	lsl	w2, w2, 8
	and	w3, w3, 16711680
	orr	w2, w2, w4, lsl 24
	orr	w0, w0, w3
	orr	w0, w2, w0
	str	w0, [x19, 8]
	ldrb	w3, [x1, 587]
	ldrb	w2, [x1, 591]
	ldr	w0, [x1, 592]
	ldrb	w1, [x1, 583]
	lsl	w2, w2, 8
	and	w0, w0, -16777216
	orr	w2, w2, w3, lsl 16
	orr	w0, w0, w1
	orr	w0, w2, w0
	str	w0, [x19, 12]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_WriteQmatrix_4x4, .-HEVC_WriteQmatrix_4x4
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WriteQmatrix_32x32
	.type	HEVC_WriteQmatrix_32x32, %function
HEVC_WriteQmatrix_32x32:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x2
	mov	x21, x1
	ubfiz	x20, x20, 6, 32
	bl	_mcount
	add	x1, x21, x20
	ldrb	w4, [x1, 1748]
	ldr	w2, [x1, 1756]
	ldr	w3, [x1, 1740]
	ldrb	w0, [x1, 1732]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19]
	ldrb	w4, [x1, 1780]
	ldr	w2, [x1, 1788]
	ldr	w3, [x1, 1772]
	ldrb	w0, [x1, 1764]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 4]
	ldrb	w4, [x1, 1757]
	ldrb	w3, [x1, 1749]
	ldr	w2, [x1, 1740]
	ldrb	w0, [x1, 1733]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 8]
	ldrb	w4, [x1, 1789]
	ldrb	w3, [x1, 1781]
	ldr	w2, [x1, 1772]
	ldrb	w0, [x1, 1765]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 12]
	ldrb	w4, [x1, 1758]
	ldrb	w3, [x1, 1742]
	ldr	w2, [x1, 1748]
	ldrb	w0, [x1, 1734]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 16]
	ldrb	w4, [x1, 1790]
	ldrb	w3, [x1, 1774]
	ldr	w2, [x1, 1780]
	ldrb	w0, [x1, 1766]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 20]
	ldrb	w4, [x1, 1751]
	ldrb	w3, [x1, 1743]
	ldr	w0, [x1, 1756]
	ldrb	w2, [x1, 1735]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 24]
	ldrb	w4, [x1, 1783]
	ldrb	w3, [x1, 1775]
	ldr	w0, [x1, 1788]
	ldrb	w2, [x1, 1767]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 28]
	ldrb	w4, [x1, 1752]
	ldr	w2, [x1, 1760]
	ldr	w3, [x1, 1744]
	ldrb	w0, [x1, 1736]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 32]
	ldrb	w4, [x1, 1784]
	ldr	w2, [x1, 1792]
	ldr	w3, [x1, 1776]
	ldrb	w0, [x1, 1768]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 36]
	ldrb	w4, [x1, 1761]
	ldrb	w3, [x1, 1753]
	ldr	w2, [x1, 1744]
	ldrb	w0, [x1, 1737]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 40]
	ldrb	w4, [x1, 1793]
	ldrb	w3, [x1, 1785]
	ldr	w2, [x1, 1776]
	ldrb	w0, [x1, 1769]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 44]
	ldrb	w4, [x1, 1762]
	ldrb	w3, [x1, 1746]
	ldr	w2, [x1, 1752]
	ldrb	w0, [x1, 1738]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 48]
	ldrb	w4, [x1, 1794]
	ldrb	w3, [x1, 1778]
	ldr	w2, [x1, 1784]
	ldrb	w0, [x1, 1770]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 52]
	ldrb	w4, [x1, 1755]
	ldrb	w3, [x1, 1747]
	ldr	w0, [x1, 1760]
	ldrb	w2, [x1, 1739]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 56]
	ldrb	w3, [x1, 1787]
	ldrb	w2, [x1, 1779]
	ldr	w0, [x1, 1792]
	ldrb	w1, [x1, 1771]
	lsl	w2, w2, 8
	and	w0, w0, -16777216
	orr	w2, w2, w3, lsl 16
	orr	w0, w0, w1
	orr	w0, w2, w0
	str	w0, [x19, 60]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_WriteQmatrix_32x32, .-HEVC_WriteQmatrix_32x32
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WriteQmatrix_8x8
	.type	HEVC_WriteQmatrix_8x8, %function
HEVC_WriteQmatrix_8x8:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x2
	mov	x21, x1
	ubfiz	x20, x20, 6, 32
	bl	_mcount
	add	x1, x21, x20
	ldrb	w4, [x1, 996]
	ldr	w2, [x1, 1012]
	ldr	w3, [x1, 980]
	ldrb	w0, [x1, 964]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19]
	ldrb	w4, [x1, 1004]
	ldr	w2, [x1, 1020]
	ldr	w3, [x1, 988]
	ldrb	w0, [x1, 972]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 4]
	ldrb	w4, [x1, 1013]
	ldrb	w3, [x1, 997]
	ldr	w2, [x1, 980]
	ldrb	w0, [x1, 965]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 8]
	ldrb	w4, [x1, 1021]
	ldrb	w3, [x1, 1005]
	ldr	w2, [x1, 988]
	ldrb	w0, [x1, 973]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 12]
	ldrb	w4, [x1, 1014]
	ldrb	w3, [x1, 982]
	ldr	w2, [x1, 996]
	ldrb	w0, [x1, 966]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 16]
	ldrb	w4, [x1, 1022]
	ldrb	w3, [x1, 990]
	ldr	w2, [x1, 1004]
	ldrb	w0, [x1, 974]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 20]
	ldrb	w4, [x1, 999]
	ldrb	w3, [x1, 983]
	ldr	w0, [x1, 1012]
	ldrb	w2, [x1, 967]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 24]
	ldrb	w4, [x1, 1007]
	ldrb	w3, [x1, 991]
	ldr	w0, [x1, 1020]
	ldrb	w2, [x1, 975]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 28]
	ldrb	w4, [x1, 1000]
	ldr	w2, [x1, 1016]
	ldr	w3, [x1, 984]
	ldrb	w0, [x1, 968]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 32]
	ldrb	w4, [x1, 1008]
	ldr	w2, [x1, 1024]
	ldr	w3, [x1, 992]
	ldrb	w0, [x1, 976]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 36]
	ldrb	w4, [x1, 1017]
	ldrb	w3, [x1, 1001]
	ldr	w2, [x1, 984]
	ldrb	w0, [x1, 969]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 40]
	ldrb	w4, [x1, 1025]
	ldrb	w3, [x1, 1009]
	ldr	w2, [x1, 992]
	ldrb	w0, [x1, 977]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 44]
	ldrb	w4, [x1, 1018]
	ldrb	w3, [x1, 986]
	ldr	w2, [x1, 1000]
	ldrb	w0, [x1, 970]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 48]
	ldrb	w4, [x1, 1026]
	ldrb	w3, [x1, 994]
	ldr	w2, [x1, 1008]
	ldrb	w0, [x1, 978]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 52]
	ldrb	w4, [x1, 1003]
	ldrb	w3, [x1, 987]
	ldr	w0, [x1, 1016]
	ldrb	w2, [x1, 971]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 56]
	ldrb	w3, [x1, 1011]
	ldrb	w2, [x1, 995]
	ldr	w0, [x1, 1024]
	ldrb	w1, [x1, 979]
	lsl	w2, w2, 8
	and	w0, w0, -16777216
	orr	w2, w2, w3, lsl 16
	orr	w0, w0, w1
	orr	w0, w2, w0
	str	w0, [x19, 60]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_WriteQmatrix_8x8, .-HEVC_WriteQmatrix_8x8
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WriteQmatrix_16x16
	.type	HEVC_WriteQmatrix_16x16, %function
HEVC_WriteQmatrix_16x16:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x2
	mov	x21, x1
	ubfiz	x20, x20, 6, 32
	bl	_mcount
	add	x1, x21, x20
	ldrb	w4, [x1, 1364]
	ldr	w2, [x1, 1372]
	ldr	w3, [x1, 1356]
	ldrb	w0, [x1, 1348]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19]
	ldrb	w4, [x1, 1396]
	ldr	w2, [x1, 1404]
	ldr	w3, [x1, 1388]
	ldrb	w0, [x1, 1380]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 4]
	ldrb	w4, [x1, 1373]
	ldrb	w3, [x1, 1365]
	ldr	w2, [x1, 1356]
	ldrb	w0, [x1, 1349]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 8]
	ldrb	w4, [x1, 1405]
	ldrb	w3, [x1, 1397]
	ldr	w2, [x1, 1388]
	ldrb	w0, [x1, 1381]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 12]
	ldrb	w4, [x1, 1374]
	ldrb	w3, [x1, 1358]
	ldr	w2, [x1, 1364]
	ldrb	w0, [x1, 1350]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 16]
	ldrb	w4, [x1, 1406]
	ldrb	w3, [x1, 1390]
	ldr	w2, [x1, 1396]
	ldrb	w0, [x1, 1382]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 20]
	ldrb	w4, [x1, 1367]
	ldrb	w3, [x1, 1359]
	ldr	w0, [x1, 1372]
	ldrb	w2, [x1, 1351]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 24]
	ldrb	w4, [x1, 1399]
	ldrb	w3, [x1, 1391]
	ldr	w0, [x1, 1404]
	ldrb	w2, [x1, 1383]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 28]
	ldrb	w4, [x1, 1368]
	ldr	w2, [x1, 1376]
	ldr	w3, [x1, 1360]
	ldrb	w0, [x1, 1352]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 32]
	ldrb	w4, [x1, 1400]
	ldr	w2, [x1, 1408]
	ldr	w3, [x1, 1392]
	ldrb	w0, [x1, 1384]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2, lsl 24
	orr	w0, w3, w0
	str	w0, [x19, 36]
	ldrb	w4, [x1, 1377]
	ldrb	w3, [x1, 1369]
	ldr	w2, [x1, 1360]
	ldrb	w0, [x1, 1353]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 40]
	ldrb	w4, [x1, 1409]
	ldrb	w3, [x1, 1401]
	ldr	w2, [x1, 1392]
	ldrb	w0, [x1, 1385]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 44]
	ldrb	w4, [x1, 1378]
	ldrb	w3, [x1, 1362]
	ldr	w2, [x1, 1368]
	ldrb	w0, [x1, 1354]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 48]
	ldrb	w4, [x1, 1410]
	ldrb	w3, [x1, 1394]
	ldr	w2, [x1, 1400]
	ldrb	w0, [x1, 1386]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 52]
	ldrb	w4, [x1, 1371]
	ldrb	w3, [x1, 1363]
	ldr	w0, [x1, 1376]
	ldrb	w2, [x1, 1355]
	lsl	w3, w3, 8
	and	w0, w0, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w0, w0, w2
	orr	w0, w3, w0
	str	w0, [x19, 56]
	ldrb	w3, [x1, 1403]
	ldrb	w2, [x1, 1395]
	ldr	w0, [x1, 1408]
	ldrb	w1, [x1, 1387]
	lsl	w2, w2, 8
	and	w0, w0, -16777216
	orr	w2, w2, w3, lsl 16
	orr	w0, w0, w1
	orr	w0, w2, w0
	str	w0, [x19, 60]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_WriteQmatrix_16x16, .-HEVC_WriteQmatrix_16x16
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CtbRsAddrToTsAddr
	.type	HEVC_CtbRsAddrToTsAddr, %function
HEVC_CtbRsAddrToTsAddr:
	stp	x29, x30, [sp, -224]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x3, [x20, 28144]
	ldrb	w0, [x3, 13]
	cbz	w0, .L435
	ldp	w6, w11, [x3, 68]
	ldr	x0, [x20, 28136]
	mov	w1, 0
	subs	w8, w11, #1
	ldrb	w7, [x3, 15]
	sub	w12, w6, #1
	ldr	w4, [x0, 72]
	ldr	w5, [x0, 148]
	bmi	.L407
	.p2align 2
.L438:
	sxtw	x0, w1
	add	x9, x29, 224
	add	w1, w1, 1
	add	x2, x3, x0, lsl 1
	add	x0, x9, x0, lsl 2
	cmp	w11, w1
	ldrsh	w2, [x2, 216]
	sub	w2, w2, #1
	str	w2, [x0, -172]
	bne	.L438
.L407:
	mov	w1, 0
	tbnz	w12, #31, .L405
	.p2align 2
.L437:
	sxtw	x0, w1
	add	x9, x29, 224
	add	x2, x0, 88
	add	w1, w1, 1
	add	x0, x9, x0, lsl 2
	cmp	w6, w1
	ldrsh	w2, [x3, x2, lsl 1]
	sub	w2, w2, #1
	str	w2, [x0, -84]
	bne	.L437
.L405:
	mov	w1, 1
	lsl	w0, w1, w5
	sdiv	w5, w4, w0
	msub	w0, w5, w0, w4
	cmp	w0, wzr
	csinc	w5, w5, w5, eq
	sdiv	w9, w19, w5
	msub	w5, w9, w5, w19
	cbz	w7, .L450
	tbnz	w8, #31, .L447
	mov	w14, 0
	mov	w13, 0
	mov	w4, 0
	mov	w7, 0
	mov	w8, 0
	mov	w0, 0
	.p2align 2
.L419:
	tbnz	w12, #31, .L430
	add	x0, x29, 224
	mov	w7, w4
	add	x0, x0, x13, sxtw 2
	mov	w1, 0
	mov	w2, 0
	ldr	w8, [x0, -172]
	add	w8, w8, 1
	add	w10, w14, w8
	b	.L416
	.p2align 3
.L428:
	mov	w7, w4
.L416:
	add	x0, x29, 224
	add	x0, x0, x2, sxtw 2
	add	w2, w2, 1
	ldr	w0, [x0, -84]
	add	w0, w0, 1
	add	w3, w0, w1
	cmp	w5, w3
	madd	w4, w0, w8, w4
	bge	.L414
	cmp	w9, w10
	blt	.L422
.L414:
	cmp	w6, w2
	mov	w1, w3
	bne	.L428
.L420:
	add	w1, w0, w3
	cmp	w5, w1
	bge	.L429
	cmp	w9, w10
	blt	.L445
.L429:
	add	w13, w13, 1
	mov	w14, w10
	cmp	w11, w13
	bne	.L419
.L446:
	sub	w9, w9, w10
	mov	w1, w3
	mul	w0, w9, w0
	.p2align 2
.L412:
	sub	w19, w5, w1
	add	w0, w19, w0
	add	w19, w0, w7
.L435:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 224
	ret
.L450:
	tbnz	w8, #31, .L447
	mov	w14, 0
	mov	w13, 0
	mov	w4, 0
	mov	w7, 0
	mov	w8, 0
	mov	w0, 0
	tbnz	w12, #31, .L433
	.p2align 2
.L451:
	add	x0, x29, 224
	mov	w7, w4
	add	x3, x0, x13, sxtw 2
	mov	w1, 0
	mov	w2, 0
	ldr	w8, [x3, -172]
	add	w8, w8, 1
	add	w10, w14, w8
	b	.L423
	.p2align 3
.L431:
	mov	w7, w4
.L423:
	add	x0, x29, 224
	add	x0, x0, x2, sxtw 2
	add	w2, w2, 1
	ldr	w0, [x0, -84]
	add	w0, w0, 1
	add	w3, w0, w1
	cmp	w5, w3
	madd	w4, w0, w8, w4
	bge	.L421
	cmp	w9, w10
	blt	.L422
.L421:
	cmp	w6, w2
	mov	w1, w3
	bne	.L431
.L427:
	add	w1, w0, w3
	cmp	w5, w1
	bge	.L432
	cmp	w9, w10
	blt	.L445
.L432:
	add	w13, w13, 1
	mov	w14, w10
	cmp	w11, w13
	beq	.L446
	tbz	w12, #31, .L451
.L433:
	mov	w3, 0
	add	w10, w14, w8
	b	.L427
	.p2align 3
.L422:
	sub	w9, w9, w14
	mul	w0, w9, w0
	b	.L412
.L430:
	mov	w3, 0
	add	w10, w14, w8
	b	.L420
.L447:
	mov	w0, 0
	mov	w1, 0
	mov	w7, 0
	b	.L412
.L445:
	sub	w9, w9, w14
	mov	w1, w3
	mul	w0, w9, w0
	b	.L412
	.size	HEVC_CtbRsAddrToTsAddr, .-HEVC_CtbRsAddrToTsAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CtbTsAddrToRsAddr
	.type	HEVC_CtbTsAddrToRsAddr, %function
HEVC_CtbTsAddrToRsAddr:
	stp	x29, x30, [sp, -224]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x3, [x20, 28144]
	ldrb	w0, [x3, 13]
	cbz	w0, .L470
	ldp	w8, w4, [x3, 68]
	ldr	x0, [x20, 28136]
	subs	w13, w4, #1
	sub	w12, w8, #1
	ldr	w5, [x0, 72]
	ldr	w6, [x0, 148]
	bmi	.L454
	mov	w1, 0
	.p2align 2
.L455:
	sxtw	x0, w1
	add	x7, x29, 224
	add	w1, w1, 1
	add	x2, x3, x0, lsl 1
	add	x0, x7, x0, lsl 2
	cmp	w4, w1
	ldrsh	w2, [x2, 216]
	sub	w2, w2, #1
	str	w2, [x0, -172]
	bne	.L455
	tbnz	w12, #31, .L487
.L457:
	mov	w1, 0
	.p2align 2
.L460:
	sxtw	x0, w1
	add	x4, x29, 224
	add	x2, x0, 88
	add	w1, w1, 1
	add	x0, x4, x0, lsl 2
	cmp	w12, w1
	ldrsh	w2, [x3, x2, lsl 1]
	sub	w2, w2, #1
	str	w2, [x0, -84]
	bge	.L460
	mov	w1, 1
	lsl	w0, w1, w6
	sdiv	w1, w5, w0
	msub	w0, w1, w0, w5
	cmp	w0, wzr
	csinc	w0, w1, w1, eq
	tbnz	w13, #31, .L458
.L459:
	ldr	w11, [x29, 140]
	mov	w10, 0
	mov	w9, 0
	mov	w4, 0
	mov	w1, 0
	mov	w7, 0
	mov	w2, 0
	add	w11, w11, 1
	.p2align 2
.L466:
	tbnz	w12, #31, .L472
	add	x1, x29, 224
	add	x1, x1, x9, sxtw 2
	ldr	w7, [x1, -172]
	add	w7, w7, 1
	madd	w5, w11, w7, w4
	cmp	w19, w5
	blt	.L473
	mov	w1, w4
	mov	w2, w11
	mov	w6, 0
	mov	w3, 0
	mov	w4, w5
	b	.L463
	.p2align 3
.L464:
	ldr	w2, [x5, -84]
	mov	w1, w4
	add	w2, w2, 1
	madd	w5, w2, w7, w4
	cmp	w5, w19
	bgt	.L465
	mov	w4, w5
.L463:
	add	w3, w3, 1
	add	x5, x29, 224
	cmp	w8, w3
	add	w6, w6, w2
	add	x5, x5, x3, sxtw 2
	bne	.L464
.L467:
	cmp	w4, w19
	bgt	.L471
	add	w9, w9, 1
	add	w10, w10, w7
	cmp	w13, w9
	bge	.L466
.L471:
	mov	w4, w1
	.p2align 2
.L465:
	cbz	w2, .L458
	sub	w4, w19, w4
	sdiv	w1, w4, w2
	add	w10, w1, w10
	msub	w4, w1, w2, w4
	add	w6, w4, w6
	madd	w0, w10, w0, w6
.L482:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 224
	ret
.L470:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 224
	ret
.L472:
	mov	w6, 0
	b	.L467
.L473:
	mov	w2, w11
	mov	w6, 0
	b	.L465
.L487:
	mov	w0, 1
	lsl	w0, w0, w6
	sdiv	w1, w5, w0
	msub	w0, w1, w0, w5
	cmp	w0, wzr
	csinc	w0, w1, w1, eq
	b	.L459
.L454:
	tbz	w12, #31, .L457
.L458:
	ldr	x1, .LC138
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L482
	.size	HEVC_CtbTsAddrToRsAddr, .-HEVC_CtbTsAddrToRsAddr
	.align	3
.LC138:
	.xword	.LC137
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CalcTileMap
	.type	HEVC_CalcTileMap, %function
HEVC_CalcTileMap:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 65536
	bl	_mcount
	ldr	x22, .LC139
	ldr	x20, .LC140
	mov	x2, 16384
	movk	x2, 0x2, lsl 16
	mov	w1, 0
	ldr	x3, [x22, 96]
	add	x0, x20, 1792
	blr	x3
	ldr	x0, .LC141
	mov	x2, 36864
	ldr	x3, [x22, 96]
	mov	w1, 0
	add	x0, x0, 1280
	blr	x3
	ldr	x3, [x22, 96]
	mov	x2, 880
	mov	w1, 0
	add	x0, x21, 356
	blr	x3
	str	wzr, [x21, 352]
	ldr	x2, [x19, 28144]
	ldr	x1, [x19, 28136]
	ldr	w0, [x2, 72]
	ldr	w10, [x1, 240]
	cbz	w0, .L488
	mov	w14, 0
	mov	w16, 0
	mov	w4, 0
.L497:
	ldr	w0, [x2, 68]
	uxtw	x15, w14
	cbz	w0, .L498
	add	x9, x15, 104
	ldr	w1, [x21, 352]
	mul	w13, w16, w10
	mov	w11, 0
	mov	w12, 0
	lsl	x9, x9, 1
.L496:
	uxtw	x0, w1
	add	x3, x2, x9
	add	x5, x0, 8192
	add	x5, x19, x5, lsl 3
	str	w4, [x5, 356]
	uxtw	x5, w11
	ldrsh	w3, [x3, 8]
	cbz	w3, .L492
	add	w8, w12, w13
	mov	w7, 0
	uxtw	x5, w11
	.p2align 2
.L494:
	add	x1, x5, 88
	mov	w0, w4
	sub	w6, w8, w4
	ldrsh	w1, [x2, x1, lsl 1]
	cbz	w1, .L495
	.p2align 2
.L502:
	add	x2, x20, 1792
	add	w3, w6, w0
	add	x1, x5, 88
	str	w3, [x2, w0, sxtw 2]
	add	w0, w0, 1
	sub	w3, w0, w4
	ldr	x2, [x19, 28144]
	ldrsh	w1, [x2, x1, lsl 1]
	cmp	w1, w3
	bhi	.L502
	mov	w4, w0
.L495:
	add	x0, x2, x9
	add	w7, w7, 1
	add	w8, w8, w10
	ldrsh	w0, [x0, 8]
	cmp	w0, w7
	bhi	.L494
	ldr	w1, [x21, 352]
	uxtw	x0, w1
.L492:
	add	x0, x19, x0, lsl 3
	sub	w3, w4, #1
	add	x0, x0, 65536
	add	w1, w1, 1
	add	x5, x5, 88
	add	w11, w11, 1
	str	w3, [x0, 360]
	str	w1, [x21, 352]
	ldr	w3, [x2, 68]
	ldrsh	w0, [x2, x5, lsl 1]
	cmp	w3, w11
	add	w12, w12, w0
	bhi	.L496
.L498:
	add	x15, x2, x15, lsl 1
	ldr	w0, [x2, 72]
	add	w14, w14, 1
	cmp	w0, w14
	ldrsh	w0, [x15, 216]
	add	w16, w16, w0
	bhi	.L497
.L488:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_CalcTileMap, .-HEVC_CalcTileMap
	.align	3
.LC139:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC140:
	.xword	.LANCHOR2
	.align	3
.LC141:
	.xword	.LANCHOR3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WritePicMsg
	.type	HEVC_WritePicMsg, %function
HEVC_WritePicMsg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x23, x20, 53248
	add	x19, x20, 61440
	bl	_mcount
	add	x22, x20, 65536
	ldr	x1, .LC144
	mov	w0, 2
	ldr	w3, [x23, 1108]
	ldr	w2, [x23, 1292]
	bl	dprint_vfmw
	ldr	x1, .LC146
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w0, [x23, 1276]
	mov	x2, 64
	str	w0, [x19, 2936]
	add	x1, x20, 3384
	add	x0, x19, 1768
	ldr	w3, [x23, 1180]
	ldr	w4, [x23, 1184]
	sub	w4, w4, w3
	str	w4, [x19, 2940]
	ldr	w4, [x23, 1292]
	str	w4, [x19, 2928]
	ldr	w4, [x23, 1108]
	str	w4, [x19, 2932]
	ldr	w4, [x23, 1280]
	str	w4, [x19, 2944]
	ldr	w4, [x23, 1284]
	str	w4, [x19, 2948]
	ldr	w4, [x23, 1288]
	str	w4, [x19, 2952]
	ldr	w4, [x23, 1204]
	sub	w3, w4, w3
	str	w3, [x19, 2956]
	ldr	w3, [x23, 1208]
	sub	w3, w3, w4
	str	w3, [x19, 2960]
	str	wzr, [x20, 208]
	ldr	x3, [x22, 2848]
	str	x3, [x22, 120]
	ldr	x3, [x20, 28136]
	ldrb	w4, [x3, 12]
	strb	w4, [x19, 1648]
	ldrb	w4, [x3, 5]
	strb	w4, [x19, 1649]
	ldrb	w4, [x3, 9]
	strb	w4, [x19, 1650]
	ldrb	w4, [x3, 8]
	strb	w4, [x19, 1651]
	ldrb	w4, [x3, 7]
	strb	w4, [x19, 1652]
	ldr	w4, [x3, 64]
	str	w4, [x19, 1656]
	ldr	w4, [x3, 116]
	strb	w4, [x19, 1660]
	ldr	w4, [x3, 244]
	sub	w4, w4, #1
	str	w4, [x19, 1664]
	ldr	w4, [x3, 240]
	sub	w4, w4, #1
	str	w4, [x19, 1668]
	ldr	w4, [x23, 1080]
	str	w4, [x19, 1672]
	ldr	x4, [x20, 28144]
	ldr	w5, [x4, 84]
	sub	w5, w5, #2
	str	w5, [x19, 1676]
	ldrb	w5, [x4, 11]
	strb	w5, [x19, 1680]
	ldrb	w4, [x4, 10]
	strb	w4, [x19, 1681]
	ldr	w4, [x3, 200]
	str	w4, [x19, 1684]
	ldr	w4, [x3, 208]
	str	w4, [x19, 1688]
	ldr	w4, [x3, 188]
	str	w4, [x19, 1692]
	ldr	w4, [x3, 196]
	str	w4, [x19, 1696]
	ldr	w4, [x3, 172]
	str	w4, [x19, 1700]
	ldr	w4, [x3, 180]
	str	w4, [x19, 1704]
	ldr	w4, [x3, 216]
	str	w4, [x19, 1708]
	ldr	w4, [x3, 140]
	str	w4, [x19, 1712]
	ldr	w4, [x3, 148]
	str	w4, [x19, 1716]
	ldr	x4, .LC147
	ldr	w5, [x3, 112]
	str	w5, [x19, 1720]
	ldr	x4, [x4, 104]
	ldr	w5, [x3, 104]
	str	w5, [x19, 1724]
	ldr	w5, [x3, 124]
	str	w5, [x19, 1728]
	ldr	w5, [x3, 120]
	str	w5, [x19, 1732]
	ldr	w5, [x3, 108]
	str	w5, [x19, 1736]
	ldr	w3, [x3, 100]
	str	w3, [x19, 1740]
	blr	x4
	ldr	x4, [x20, 28136]
	add	x3, x19, 4056
	add	x2, x19, 3976
	add	x1, x19, 3464
	mov	x0, x20
	ldr	w5, [x4, 76]
	str	w5, [x19, 1832]
	ldr	w4, [x4, 72]
	str	w4, [x19, 1836]
	bl	HEVC_CalcTileInfo
	cbnz	w0, .L526
	ldr	x0, [x20, 28144]
	ldrb	w1, [x0, 16]
	strb	w1, [x19, 1860]
	ldrb	w1, [x0, 7]
	strb	w1, [x19, 1861]
	ldrb	w1, [x0, 3]
	strb	w1, [x19, 1862]
	ldrb	w1, [x0, 14]
	strb	w1, [x19, 1863]
	ldrb	w1, [x0, 15]
	strb	w1, [x19, 1864]
	ldrb	w1, [x0, 13]
	strb	w1, [x19, 1865]
	ldr	x1, [x20, 28136]
	ldr	w2, [x0, 56]
	ldr	w1, [x1, 148]
	sub	w1, w1, w2
	str	w1, [x0, 88]
	ldr	x0, [x20, 28144]
	ldr	w1, [x0, 88]
	str	w1, [x19, 1868]
	ldr	w1, [x0, 52]
	str	w1, [x19, 1872]
	ldrb	w1, [x0, 8]
	strb	w1, [x19, 1876]
	ldrb	w1, [x0, 12]
	strb	w1, [x19, 1877]
	ldrb	w1, [x0, 6]
	strb	w1, [x19, 1878]
	ldr	w1, [x0, 60]
	str	w1, [x19, 1880]
	ldr	w1, [x0, 64]
	str	w1, [x19, 1884]
	ldr	w1, [x0, 72]
	sub	w1, w1, #1
	str	w1, [x19, 1888]
	ldr	w1, [x0, 68]
	sub	w1, w1, #1
	str	w1, [x19, 1892]
	ldrb	w1, [x0, 10]
	strb	w1, [x19, 1681]
	ldrb	w0, [x0, 11]
	strb	w0, [x19, 1680]
	ldr	x1, [x23, 1128]
	ldr	w0, [x20, 244]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	mov	x21, x0
	cbz	x0, .L527
	ldr	x2, [x0, 640]
	ldr	x3, [x0, 648]
	cbz	x2, .L513
	cbz	x3, .L513
	ldr	w0, [x20, 244]
	add	x3, x19, 3204
	add	x2, x19, 2984
	add	x1, x19, 3112
	bl	FSP_GetDecFsAddrTab
	ldr	x0, [x21, 640]
	add	x1, x19, 3196
	add	x2, x19, 3116
	ldr	w0, [x0, 8]
	str	w0, [x19, 2928]
	ldr	w0, [x20, 244]
	bl	FSP_GetPmvAddrTab
	ldr	w0, [x23, 1108]
	str	w0, [x19, 2932]
	mov	w0, 0
	ldr	w1, [x21, 668]
	str	w1, [x19, 3200]
	ldr	x1, [x21, 648]
	ldr	w1, [x1, 16]
	str	w1, [x22, 344]
	ldr	x1, [x21, 648]
	ldr	w1, [x1, 76]
	str	w1, [x22, 348]
	ldr	w3, [x20, 3248]
	str	w3, [x22, 112]
	cbz	w3, .L518
	.p2align 2
.L521:
	add	x1, x20, x0, sxtw 2
	add	w0, w0, 1
	add	x2, x1, 65536
	cmp	w0, w3
	ldr	w1, [x1, 3320]
	str	w1, [x2, 48]
	bne	.L521
.L518:
	add	x22, x19, 1900
	mov	w21, 0
.L516:
	mov	x2, x22
	mov	w0, w21
	mov	x1, x20
	add	w21, w21, 1
	add	x22, x22, 64
	bl	HEVC_WriteQmatrix_8x8
	cmp	w21, 4
	bne	.L516
	mov	w0, w21
	add	x2, x19, 2156
	mov	x1, x20
	add	x22, x19, 2412
	mov	w21, 2
	bl	HEVC_WriteQmatrix_8x8
	add	x2, x19, 2220
	mov	x1, x20
	mov	w0, 5
	bl	HEVC_WriteQmatrix_8x8
	add	x2, x19, 2284
	mov	x1, x20
	mov	w0, 0
	bl	HEVC_WriteQmatrix_16x16
	add	x2, x19, 2348
	mov	x1, x20
	mov	w0, 1
	bl	HEVC_WriteQmatrix_16x16
.L519:
	mov	x2, x22
	mov	w0, w21
	mov	x1, x20
	add	w21, w21, 1
	add	x22, x22, 64
	bl	HEVC_WriteQmatrix_16x16
	cmp	w21, 6
	bne	.L519
	add	x2, x19, 2668
	mov	x1, x20
	mov	w0, 0
	add	x22, x19, 2796
	mov	w21, 0
	bl	HEVC_WriteQmatrix_32x32
	add	x2, x19, 2732
	mov	x1, x20
	mov	w0, 1
	bl	HEVC_WriteQmatrix_32x32
	.p2align 2
.L520:
	mov	x2, x22
	mov	w0, w21
	mov	x1, x20
	add	w21, w21, 1
	add	x22, x22, 16
	bl	HEVC_WriteQmatrix_4x4
	cmp	w21, 6
	bne	.L520
	add	x5, x20, 2048
	add	x6, x20, 2048
	mov	w0, 0
	ldp	w3, w4, [x5, 220]
	ldp	w1, w2, [x5, 212]
	lsl	w3, w3, 16
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2, lsl 8
	orr	w1, w3, w1
	str	w1, [x19, 2892]
	ldp	w3, w4, [x6, 236]
	ldp	w1, w2, [x6, 228]
	lsl	w3, w3, 16
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2, lsl 8
	orr	w1, w3, w1
	str	w1, [x19, 2896]
	ldr	w1, [x23, 1108]
	str	w1, [x19, 2924]
.L510:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L526:
	ldr	x2, .LC148
	mov	w0, 1
	ldr	x1, .LC150
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, -1
	b	.L510
.L513:
	ldr	x1, .LC154
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L510
.L527:
	ldr	x2, [x23, 1128]
	mov	w0, 1
	ldr	x1, .LC152
	ldr	w2, [x2, 32]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L510
	.size	HEVC_WritePicMsg, .-HEVC_WritePicMsg
	.align	3
.LC144:
	.xword	.LC143
	.align	3
.LC146:
	.xword	.LC145
	.align	3
.LC147:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC148:
	.xword	.LANCHOR1
	.align	3
.LC150:
	.xword	.LC149
	.align	3
.LC152:
	.xword	.LC151
	.align	3
.LC154:
	.xword	.LC153
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CheckListX
	.type	HEVC_CheckListX, %function
HEVC_CheckListX:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 53248
	add	x0, x0, 1760
	ldrb	w1, [x0, 3]
	cbz	w1, .L530
	ldrsb	w1, [x19, 9]
	add	x0, x19, 57344
	add	x0, x0, 1396
	cbz	w1, .L562
.L530:
	ldp	w4, w5, [x0, 160]
	cmp	w4, 16
	bhi	.L563
	cmp	w5, 16
	bhi	.L534
	cbz	w4, .L535
	ldr	x0, [x19, 2368]
	cbz	x0, .L548
	ldr	x0, [x0, 32]
	cbz	x0, .L564
	add	x0, x19, 2376
	mov	w2, 0
	b	.L540
	.p2align 3
.L541:
	ldr	x1, [x0]
	add	x0, x0, 8
	cbz	x1, .L536
	ldr	x1, [x1, 32]
	cbz	x1, .L537
.L540:
	add	w2, w2, 1
	cmp	w4, w2
	bne	.L541
.L535:
	cbz	w5, .L550
	ldr	x0, [x19, 2504]
	cbz	x0, .L551
	ldr	x0, [x0, 32]
	cbz	x0, .L565
	add	x0, x19, 2512
	mov	w2, 0
	b	.L546
	.p2align 3
.L547:
	ldr	x1, [x0]
	add	x0, x0, 8
	cbz	x1, .L542
	ldr	x1, [x1, 32]
	cbz	x1, .L543
.L546:
	add	w2, w2, 1
	cmp	w5, w2
	bne	.L547
.L550:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L548:
	mov	w2, 0
	.p2align 2
.L536:
	ldr	x1, .LC163
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L532:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L564:
	mov	w2, 0
.L537:
	ldr	x1, .LC165
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L551:
	mov	w2, 0
.L542:
	ldr	x1, .LC167
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L565:
	mov	w2, 0
.L543:
	ldr	x1, .LC169
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L562:
	ldr	x2, .LC155
	mov	w0, 0
	ldr	x1, .LC157
	add	x2, x2, 320
	bl	dprint_vfmw
	mov	w0, -1
	b	.L532
.L534:
	ldr	x1, .LC161
	mov	w3, 17
	mov	w2, w5
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L532
.L563:
	ldr	x1, .LC159
	mov	w3, 17
	mov	w2, w4
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L532
	.size	HEVC_CheckListX, .-HEVC_CheckListX
	.align	3
.LC155:
	.xword	.LANCHOR1
	.align	3
.LC157:
	.xword	.LC156
	.align	3
.LC159:
	.xword	.LC158
	.align	3
.LC161:
	.xword	.LC160
	.align	3
.LC163:
	.xword	.LC162
	.align	3
.LC165:
	.xword	.LC164
	.align	3
.LC167:
	.xword	.LC166
	.align	3
.LC169:
	.xword	.LC168
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_WriteSliceMsg
	.type	HEVC_WriteSliceMsg, %function
HEVC_WriteSliceMsg:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	HEVC_CheckListX
	mov	w23, w0
	cbnz	w0, .L606
	ldr	w3, [x19, 144]
	ldr	w20, [x19, 208]
	cmp	w20, w3
	bcs	.L646
	add	x25, x19, 65536
	mov	w21, 1368
	ldr	x22, .LC172
	mov	x2, 1368
	umull	x20, w20, w21
	ldr	x0, [x25, 2848]
	mov	w1, 0
	ldr	x3, [x22, 96]
	add	x20, x0, x20
	mov	x0, x20
	blr	x3
	ldr	x3, [x22, 96]
	mov	w1, 0
	mov	x2, 64
	add	x0, x29, 96
	blr	x3
	ldr	w0, [x19, 208]
	ldr	w1, [x19, 144]
	add	w0, w0, 1
	str	w0, [x19, 208]
	cmp	w0, w1
	bcc	.L647
	str	xzr, [x20, 1360]
.L570:
	ldr	w0, [x19, 216]
	add	x24, x19, 53248
	add	w1, w0, 1
	uxtw	x0, w0
	ldr	w2, [x24, 1836]
	add	x0, x0, 16384
	add	x0, x0, 716
	str	w2, [x19, x0, lsl 2]
	str	w1, [x19, 216]
	ldr	w21, [x19, 172]
	add	w0, w21, 1
	str	w0, [x19, 172]
	lsl	x21, x0, 5
	ldr	x1, [x25, 3664]
	add	x21, x21, x0, lsl 3
	ldr	w2, [x24, 1820]
	add	x0, x1, x21
	str	w2, [x0, 8]
	ldr	x3, [x19, 2360]
	ldrb	w1, [x3, 1]
	ldr	w0, [x3, 8]
	cmp	w1, 1
	ccmp	w1, w0, 2, ls
	bcs	.L571
	.p2align 2
.L572:
	uxtw	x2, w1
	add	x0, x2, 2
	lsl	x0, x0, 5
	add	x2, x20, x2, lsl 2
	add	x3, x3, x0
	ldr	w3, [x3, 4]
	str	w3, [x2, 8]
	ldr	x3, [x19, 2360]
	ldr	w3, [x3, x0]
	str	w3, [x20, w1, uxtw 2]
	add	w1, w1, 1
	ldr	x3, [x19, 2360]
	add	x0, x3, x0
	ldr	w0, [x0, 8]
	str	w0, [x2, 16]
	ldr	x3, [x19, 2360]
	ldr	w0, [x3, 8]
	cmp	w0, w1
	ccmp	w1, 1, 2, hi
	bls	.L572
.L571:
	cmp	w0, 1
	bhi	.L577
	.p2align 2
.L630:
	add	x1, x20, x0, uxtw 2
	str	wzr, [x1, 8]
	str	wzr, [x20, w0, uxtw 2]
	add	w0, w0, 1
	cmp	w0, 2
	str	wzr, [x1, 16]
	bne	.L630
.L577:
	ldrb	w0, [x24, 1763]
	add	x26, x19, 57344
	cbz	w0, .L648
.L575:
	ldr	w0, [x26, 1556]
	cbz	w0, .L578
	mov	w2, 0
	.p2align 2
.L579:
	uxtw	x0, w2
	add	x6, x29, 160
	add	w2, w2, 1
	add	x4, x19, x0, lsl 3
	add	x0, x20, x0, lsl 2
	ldr	x5, [x4, 2368]
	ldr	x3, [x5, 32]
	ldp	w1, w3, [x3, 24]
	add	x1, x1, 12288
	add	x3, x6, x3, lsl 2
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 15404]
	str	w1, [x3, -64]
	ldr	w1, [x5, 16]
	str	w1, [x0, 116]
	ldr	x1, [x4, 2368]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 28]
	str	w1, [x0, 372]
	ldr	w0, [x26, 1556]
	cmp	w0, w2
	bhi	.L579
	ldr	w3, [x26, 1560]
	cbz	w3, .L580
.L605:
	mov	w2, 0
	.p2align 2
.L581:
	uxtw	x0, w2
	add	x6, x29, 160
	add	w2, w2, 1
	add	x4, x19, x0, lsl 3
	add	x0, x20, x0, lsl 2
	ldr	x5, [x4, 2504]
	ldr	x3, [x5, 32]
	ldp	w1, w3, [x3, 24]
	add	x1, x1, 12288
	add	x3, x6, x3, lsl 2
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 15404]
	str	w1, [x3, -64]
	ldr	w1, [x5, 16]
	str	w1, [x0, 180]
	ldr	x1, [x4, 2504]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 28]
	str	w1, [x0, 436]
	ldr	w3, [x26, 1560]
	cmp	w3, w2
	bhi	.L581
	ldr	w0, [x26, 1556]
.L580:
	cbz	w0, .L582
	ldr	w1, [x20, 372]
	ldr	w4, [x24, 1080]
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 3384]
	cmp	w4, w1
	blt	.L612
	sub	w0, w0, #1
	add	x2, x20, x0, lsl 2
	mov	x0, x20
	b	.L585
	.p2align 3
.L586:
	ldr	w1, [x0, 376]
	add	x0, x0, 4
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 3384]
	cmp	w1, w4
	bgt	.L612
.L585:
	cmp	x2, x0
	bne	.L586
.L582:
	ldr	w0, [x26, 1456]
	cbnz	w0, .L645
	mov	w0, 1
	cbz	w3, .L583
	ldr	w0, [x20, 436]
	ldr	w2, [x24, 1080]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 3384]
	cmp	w2, w0
	blt	.L612
	sub	w3, w3, #1
	mov	x0, x20
	add	x3, x20, x3, lsl 2
	b	.L588
	.p2align 3
.L589:
	ldr	w1, [x0, 440]
	add	x0, x0, 4
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 3384]
	cmp	w1, w2
	bgt	.L612
.L588:
	cmp	x3, x0
	bne	.L589
.L645:
	mov	w0, 1
.L583:
	strb	w0, [x20, 24]
	ldrb	w0, [x24, 1763]
	strb	w0, [x20, 25]
	ldr	w0, [x26, 1452]
	str	w0, [x20, 28]
	ldrb	w0, [x26, 1406]
	strb	w0, [x20, 32]
	ldr	w0, [x26, 1476]
	str	w0, [x20, 36]
	ldr	w0, [x26, 1456]
	cbz	w0, .L649
	cmp	w0, 1
	mov	w27, 0
	beq	.L591
.L592:
	str	w27, [x20, 40]
	ldr	w0, [x26, 1508]
	str	w0, [x20, 48]
	ldrb	w0, [x26, 1407]
	strb	w0, [x20, 52]
	ldr	w0, [x26, 1512]
	str	w0, [x20, 44]
	ldr	w0, [x26, 1564]
	cbz	w0, .L593
	sub	w0, w0, #1
	str	w0, [x20, 60]
.L594:
	ldr	w0, [x26, 1568]
	cbz	w0, .L595
	sub	w0, w0, #1
	str	w0, [x20, 56]
.L596:
	ldrb	w0, [x26, 1403]
	strb	w0, [x20, 64]
	ldrb	w0, [x26, 1405]
	strb	w0, [x20, 65]
	ldr	w0, [x26, 1556]
	str	w0, [x20, 68]
	ldr	w0, [x26, 1560]
	str	w0, [x20, 72]
	ldr	w0, [x26, 1456]
	str	w0, [x20, 76]
	ldr	w0, [x24, 1840]
	str	w0, [x20, 80]
	ldr	x0, [x19, 28136]
	ldr	w3, [x24, 1840]
	ldr	w1, [x0, 240]
	mov	x0, x19
	udiv	w2, w3, w1
	msub	w1, w2, w1, w3
	str	w1, [x20, 84]
	ldr	x1, [x19, 28136]
	ldr	w2, [x1, 240]
	ldr	w1, [x24, 1840]
	udiv	w1, w1, w2
	str	w1, [x20, 88]
	ldr	w1, [x24, 1840]
	bl	HEVC_CtbRsAddrToTsAddr
	str	w0, [x20, 92]
	ldr	w0, [x26, 1520]
	str	w0, [x20, 96]
	ldr	w0, [x26, 1524]
	str	w0, [x20, 100]
	ldr	w0, [x26, 1532]
	str	w0, [x20, 104]
	ldr	w0, [x26, 1528]
	str	w0, [x20, 108]
	ldrb	w0, [x26, 1402]
	strb	w0, [x20, 112]
	ldrb	w0, [x26, 1401]
	strb	w0, [x20, 113]
	ldrb	w0, [x26, 1409]
	strb	w0, [x20, 114]
	ldrb	w0, [x26, 1410]
	strb	w0, [x20, 115]
	ldr	x0, [x19, 28144]
	ldrb	w1, [x0, 10]
	cbz	w1, .L597
	ldr	w1, [x26, 1456]
	cmp	w1, 1
	beq	.L598
.L597:
	ldrb	w0, [x0, 11]
	cmp	w0, 1
	beq	.L599
	add	x24, x19, 61440
.L600:
	ldrb	w0, [x26, 1403]
	cbz	w0, .L602
	add	x0, x29, 160
	add	x27, x0, x27, sxtw 2
	ldr	w0, [x27, -64]
	str	w0, [x20, 564]
.L602:
	ldr	x0, [x25, 3664]
	ldr	x1, [x19, 2360]
	add	x21, x0, x21
	str	x1, [x21, 32]
	str	xzr, [x19, 2360]
	ldrb	w0, [x24, 1865]
	cbz	w0, .L621
	ldr	w0, [x19, 216]
	cmp	w0, 1
	beq	.L650
	sub	w1, w0, #2
	sub	w0, w0, #1
	ldr	w3, [x20, 92]
	add	x2, x0, 8192
	add	x1, x19, x1, lsl 3
	add	x1, x1, 65536
	add	x2, x19, x2, lsl 3
	sub	w3, w3, #1
	str	w3, [x1, 1240]
	ldr	w1, [x20, 92]
	str	w1, [x2, 1236]
.L604:
	ldr	w1, [x24, 1664]
	add	x0, x19, x0, lsl 3
	ldr	w2, [x24, 1668]
	add	x0, x0, 65536
	mov	w3, 36863
	madd	w2, w1, w2, w2
	add	w1, w2, w1
	str	w1, [x0, 1240]
	ldr	w1, [x20, 92]
	sub	w1, w1, #1
	cmp	w1, w3
	bhi	.L621
	mov	x0, x19
	bl	HEVC_CtbTsAddrToRsAddr
	str	w0, [x20, 568]
.L621:
	mov	w0, w23
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L647:
	ldr	x1, [x25, 2848]
	umull	x0, w0, w21
	add	x0, x1, x0
	str	x0, [x20, 1360]
	b	.L570
	.p2align 3
.L612:
	mov	w0, 0
	b	.L583
	.p2align 3
.L595:
	str	wzr, [x20, 56]
	b	.L596
	.p2align 3
.L593:
	str	wzr, [x20, 60]
	b	.L594
	.p2align 3
.L649:
	ldrb	w0, [x26, 1407]
	cbnz	w0, .L591
	ldrsw	x0, [x26, 1508]
	add	x0, x20, x0, lsl 2
	ldr	w27, [x0, 436]
	b	.L592
	.p2align 3
.L591:
	ldrsw	x0, [x26, 1508]
	add	x0, x20, x0, lsl 2
	ldr	w27, [x0, 372]
	b	.L592
	.p2align 3
.L648:
	ldr	x3, [x22, 104]
	mov	x2, 3732
	add	x1, x24, 1760
	add	x0, x26, 1396
	blr	x3
	mov	w0, 1
	strb	w0, [x19, 9]
	b	.L575
	.p2align 3
.L599:
	ldr	w0, [x26, 1456]
	add	x24, x19, 61440
	cbnz	w0, .L600
.L598:
	ldr	w0, [x26, 2988]
	mov	x2, 64
	str	w0, [x20, 580]
	add	x1, x26, 4024
	ldr	x3, [x22, 104]
	add	x24, x19, 61440
	ldr	w0, [x26, 4020]
	str	w0, [x20, 584]
	add	x0, x20, 588
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 64
	add	x1, x26, 3188
	add	x0, x20, 716
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 128
	add	x1, x26, 4088
	add	x0, x20, 844
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 128
	add	x1, x24, 120
	add	x0, x20, 1100
	blr	x3
	ldr	w0, [x26, 1456]
	cbnz	w0, .L600
	ldr	x3, [x22, 104]
	add	x1, x24, 248
	mov	x2, 64
	add	x0, x20, 652
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x26, 3700
	mov	x2, 64
	add	x0, x20, 780
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x24, 312
	mov	x2, 128
	add	x0, x20, 972
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 128
	add	x1, x24, 440
	add	x0, x20, 1228
	blr	x3
	b	.L600
	.p2align 3
.L650:
	ldr	w1, [x20, 92]
	mov	x0, 0
	str	w1, [x25, 1236]
	b	.L604
.L578:
	ldr	w0, [x26, 1560]
	cbnz	w0, .L605
	mov	w0, 1
	b	.L583
.L606:
	mov	w23, -1
	b	.L621
.L646:
	ldr	x1, .LC171
	mov	w2, w20
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L621
	.size	HEVC_WriteSliceMsg, .-HEVC_WriteSliceMsg
	.align	3
.LC171:
	.xword	.LC170
	.align	3
.LC172:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetVirAddr
	.type	HEVC_GetVirAddr, %function
HEVC_GetVirAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	add	x0, x19, x20, uxtw 2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_GetVirAddr, .-HEVC_GetVirAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetPhyAddr
	.type	HEVC_GetPhyAddr, %function
HEVC_GetPhyAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	w0, w19, w20, lsl 2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_GetPhyAddr, .-HEVC_GetPhyAddr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CfgVDMReg
	.type	HEVC_CfgVDMReg, %function
HEVC_CfgVDMReg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 53248
	add	x1, x19, 61440
	add	x3, x19, 65536
	mov	w0, 0
	ldr	w4, [x2, 1100]
	cmp	w4, wzr
	cset	w4, eq
	str	w4, [x1, 2980]
	ldr	w2, [x2, 1104]
	str	w2, [x1, 2972]
	ldr	w1, [x19, 208]
	str	w1, [x3, 116]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_CfgVDMReg, .-HEVC_CfgVDMReg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsRefForCurrPic
	.type	HEVC_IsRefForCurrPic, %function
HEVC_IsRefForCurrPic:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 53248
	bl	_mcount
	ldr	w4, [x19, 156]
	ldr	w3, [x21, 1836]
	tbnz	w3, #31, .L656
	cmp	w3, w4
	bge	.L656
	ldr	x0, [x19, 2336]
	mov	w1, 2024
	ldr	w4, [x19, 152]
	smaddl	x3, w3, w1, x0
	ldr	w3, [x3, 32]
	tbnz	w3, #31, .L660
	cmp	w3, w4
	bge	.L660
	add	x20, x19, 57344
	ldr	x2, [x19, 2328]
	mov	w4, 26164
	mov	w0, 1
	smaddl	x3, w3, w4, x2
	ldr	w5, [x20, 948]
	ldr	w6, [x20, 952]
	ldr	w9, [x20, 956]
	add	w8, w5, w6
	ldr	w2, [x3, 128]
	cmp	w8, w9
	mov	w3, w8
	lsl	w2, w0, w2
	bcs	.L672
	cmp	w8, 15
	bgt	.L663
	ldr	w4, [x21, 1080]
	b	.L665
	.p2align 3
.L683:
	cmp	w9, w3
	bls	.L672
	cmp	w3, 16
	beq	.L663
.L665:
	add	x0, x19, x3, sxtw 2
	mov	w1, w4
	add	x7, x0, 53248
	add	x0, x0, 57344
	add	w3, w3, 1
	ldr	w7, [x7, 3284]
	ldr	w0, [x0, 1072]
	cbnz	w7, .L666
	sdiv	w1, w4, w2
	sdiv	w7, w0, w2
	msub	w1, w1, w2, w4
	msub	w0, w7, w2, w0
.L666:
	cmp	w1, w0
	bne	.L683
	mov	w19, 1
.L662:
	cbz	w8, .L658
	ldr	w0, [x20, 1004]
	add	x1, x20, 1008
	ldr	w3, [x21, 1808]
	mov	w4, 0
	ldr	w2, [x21, 1080]
	add	w0, w3, w0
	cmp	w2, w0
	bne	.L670
	b	.L676
	.p2align 3
.L669:
	ldr	w0, [x1], 4
	add	w0, w3, w0
	cmp	w0, w2
	beq	.L676
.L670:
	add	w4, w4, 1
	cmp	w4, w8
	beq	.L658
	cmp	w4, 17
	bne	.L669
	ldr	x1, .LC180
	mov	w3, w4
	mov	w2, 6983
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L660:
	ldr	x1, .LC176
	mov	w2, 6934
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
.L658:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L656:
	ldr	x1, .LC174
	mov	w2, 6922
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L672:
	mov	w19, 0
	b	.L662
	.p2align 3
.L663:
	ldr	x1, .LC178
	mov	w5, w9
	mov	w4, 16
	mov	w2, 6954
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	ldr	w5, [x20, 948]
	ldr	w6, [x20, 952]
	add	w8, w5, w6
	b	.L662
	.p2align 3
.L676:
	mov	w19, 1
	b	.L658
	.size	HEVC_IsRefForCurrPic, .-HEVC_IsRefForCurrPic
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RepairList
	.type	HEVC_RepairList, %function
HEVC_RepairList:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x25, x0
	mov	x0, x30
	add	x23, x25, 53248
	bl	_mcount
	ldr	w3, [x23, 1820]
	cmp	w3, 1
	beq	.L686
	cmp	w3, 2
	beq	.L702
	cbz	w3, .L701
	ldr	x2, .LC181
	mov	w0, 1
	ldr	x1, .LC183
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L689:
	ldr	x0, [x25, 2344]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L697
	mov	w0, 1
	str	w0, [x27]
	ldr	x0, [x23, 1128]
	add	x0, x0, 40
	str	x0, [x28, -8]
	.p2align 2
.L697:
	add	w26, w26, 1
	add	x27, x27, 4
	cmp	w26, w22
	add	x28, x28, 136
	bcc	.L699
.L702:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 112
	ret
.L701:
	mov	w22, 2
.L688:
	ldr	x21, .LC181
	add	x27, x23, 1920
	add	x28, x25, 2376
	mov	w26, 0
	add	x0, x21, 336
	str	x0, [x29, 96]
	.p2align 2
.L699:
	ldr	w1, [x27]
	cbz	w1, .L689
	ldr	x2, [x28, -8]
	cbnz	x2, .L703
	mov	x3, x28
	mov	w0, 0
	b	.L691
	.p2align 3
.L693:
	ldr	x24, [x3], 8
	cbnz	x24, .L708
.L691:
	add	w0, w0, 1
	cmp	w1, w0
	bne	.L693
	mov	w20, -1
.L692:
	ldr	x0, [x25, 2344]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L694
	ldr	x24, [x23, 1128]
	add	x24, x24, 40
.L690:
	mov	x0, x2
	uxtw	x8, w26
	mov	w19, 0
	lsl	x21, x8, 4
	cbz	x0, .L709
	.p2align 2
.L695:
	add	w19, w19, 1
	add	x0, x21, x8
	cmp	w1, w19
	add	x0, x0, x19, uxtw
	add	x0, x0, 296
	bls	.L697
	ldr	x0, [x25, x0, lsl 3]
	cbnz	x0, .L695
.L709:
	ldr	x1, .LC187
	mov	w4, w19
	ldr	x2, [x29, 96]
	mov	w5, w20
	mov	w3, w26
	mov	w0, 1
	str	x8, [x29, 104]
	bl	dprint_vfmw
	ldr	x8, [x29, 104]
	add	x0, x21, x8
	add	x0, x0, x19, uxtw
	add	x0, x0, 296
	str	x24, [x25, x0, lsl 3]
	ldr	w1, [x27]
	b	.L695
	.p2align 3
.L708:
	sxtb	w20, w0
	cmn	w20, #1
	ccmp	x24, xzr, 4, ne
	bne	.L690
	b	.L692
	.p2align 3
.L703:
	mov	x24, x2
	mov	w20, 0
	b	.L690
.L686:
	mov	w22, w3
	b	.L688
.L694:
	ldr	x2, .LC181
	mov	w3, w26
	ldr	x1, .LC185
	mov	w0, 1
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.size	HEVC_RepairList, .-HEVC_RepairList
	.align	3
.LC181:
	.xword	.LANCHOR1
	.align	3
.LC183:
	.xword	.LC182
	.align	3
.LC185:
	.xword	.LC184
	.align	3
.LC187:
	.xword	.LC186
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InitListX
	.type	HEVC_InitListX, %function
HEVC_InitListX:
	stp	x29, x30, [sp, -320]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L746
	ldr	x20, .LC189
	add	x21, x29, 48
	mov	x2, 272
	mov	w1, 0
	mov	x0, x21
	ldr	x3, [x20, 96]
	blr	x3
	ldr	w0, [x19, 2696]
	cbz	w0, .L747
	sub	w3, w0, #1
	mov	w5, 0
	add	x3, x3, 356
	add	x0, x19, 2840
	mov	x4, 4294967296
	add	x3, x19, x3, lsl 3
	b	.L714
	.p2align 3
.L713:
	cmp	x0, x3
	beq	.L712
.L714:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 1]
	cmp	w2, 1
	bne	.L713
	ldr	x2, [x1, 56]
	cmp	x2, x4
	bne	.L713
	add	x2, x29, 320
	add	x1, x1, 40
	add	x2, x2, x5, uxtw 3
	cmp	x0, x3
	sub	x2, x2, #4096
	add	w5, w5, 1
	str	x1, [x2, 3824]
	bne	.L714
	.p2align 2
.L712:
	ldr	w0, [x19, 2700]
	cbz	w0, .L748
	sub	w4, w0, #1
	mov	w3, w5
	add	x4, x4, 373
	add	x0, x19, 2976
	mov	x6, 4294967296
	add	x4, x19, x4, lsl 3
	b	.L717
	.p2align 3
.L716:
	cmp	x0, x4
	beq	.L775
.L717:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 1]
	cmp	w2, 1
	bne	.L716
	ldr	x2, [x1, 56]
	cmp	x2, x6
	bne	.L716
	add	x2, x29, 320
	add	x1, x1, 40
	add	x2, x2, x3, uxtw 3
	cmp	x0, x4
	sub	x2, x2, #4096
	add	w3, w3, 1
	str	x1, [x2, 3824]
	bne	.L717
	.p2align 2
.L775:
	sub	w6, w3, w5
.L715:
	ldr	w0, [x19, 2692]
	sub	w7, w3, #1
	cbz	w0, .L718
	sub	w2, w0, #1
	add	x0, x19, 3112
	add	x2, x2, 390
	add	x2, x19, x2, lsl 3
	b	.L720
	.p2align 3
.L719:
	cmp	x2, x0
	beq	.L776
.L720:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w4, [x1, 1]
	cmp	w4, 1
	bne	.L719
	ldr	x4, [x1, 56]
	cmp	x4, 1
	bne	.L719
	add	x4, x29, 320
	add	x1, x1, 40
	add	x4, x4, x3, uxtw 3
	cmp	x2, x0
	sub	x4, x4, #4096
	add	w3, w3, 1
	str	x1, [x4, 3824]
	bne	.L720
	.p2align 2
.L776:
	sub	w0, w3, w5
	sub	w0, w0, w6
.L718:
	cbz	w3, .L777
	add	x22, x19, 53248
	ldr	w8, [x22, 1820]
	cbnz	w8, .L778
	cbz	w6, .L726
	add	x4, x21, 136
	add	w9, w6, w5
	mov	w1, w5
	.p2align 2
.L727:
	add	x2, x29, 320
	add	x2, x2, x1, uxtw 3
	add	w1, w1, 1
	sub	x2, x2, #4096
	cmp	w9, w1
	ldr	x2, [x2, 3824]
	str	x2, [x4], 8
	bne	.L727
.L726:
	cbz	w5, .L728
	mov	x1, 0
	.p2align 2
.L729:
	add	w2, w6, w1
	add	x9, x29, 320
	ldr	x4, [x21, x1, lsl 3]
	add	x1, x1, 1
	add	x2, x9, x2, lsl 3
	cmp	w5, w1
	str	x4, [x2, -136]
	bhi	.L729
	add	w6, w6, w5
.L728:
	cbz	w0, .L730
	add	w1, w0, w6
	mov	w2, w6
	.p2align 2
.L731:
	add	w4, w2, 1
	add	x0, x29, 320
	add	x5, x0, x2, uxtw 3
	sub	w0, w4, w6
	add	w0, w0, w7
	mov	w2, w4
	cmp	w1, w4
	add	x4, x29, 320
	add	x0, x4, x0, lsl 3
	sub	x0, x0, #4096
	ldr	x0, [x0, 3824]
	str	x0, [x5, -136]
	bne	.L731
.L730:
	ldr	w5, [x22, 1928]
	ldr	w7, [x22, 1932]
	str	w5, [x22, 1920]
	str	w7, [x22, 1924]
	cbnz	w5, .L724
	.p2align 2
.L744:
	cbz	w7, .L736
	add	x0, x19, 57344
	add	x4, x19, 2504
	add	x2, x0, 1268
	mov	w1, 0
	ldrb	w6, [x0, 1137]
	.p2align 2
.L739:
	cbz	w6, .L737
	ldr	w0, [x2]
.L738:
	add	x8, x29, 320
	add	w1, w1, 1
	add	x0, x8, x0, uxtw 3
	cmp	w1, w7
	add	x2, x2, 4
	ldr	x0, [x0, -136]
	str	x0, [x4], 8
	bne	.L739
	mov	w0, w5
.L735:
	cmp	w0, 16
	bhi	.L749
	.p2align 2
.L741:
	add	x1, x19, x0, uxtw 3
	ldr	x2, [x19, 2368]
	add	w0, w0, 1
	cmp	w0, 16
	str	x2, [x1, 2368]
	bls	.L741
.L749:
	cmp	w7, 16
	mov	w0, w7
	bhi	.L742
	.p2align 2
.L743:
	add	x1, x19, x0, uxtw 3
	ldr	x2, [x19, 2504]
	add	w0, w0, 1
	cmp	w0, 17
	str	x2, [x1, 2504]
	bne	.L743
.L742:
	mov	w0, 0
.L772:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 320
	ret
	.p2align 3
.L778:
	ldr	w5, [x22, 1928]
	ldr	w7, [x22, 1932]
	str	w5, [x22, 1920]
	str	w7, [x22, 1924]
	cbz	w5, .L779
.L724:
	add	x0, x19, 57344
	add	x4, x19, 2368
	add	x2, x0, 1140
	ldrb	w6, [x0, 1136]
	mov	w0, 0
	.p2align 2
.L734:
	cbz	w6, .L732
	ldr	w1, [x2]
.L733:
	add	x9, x29, 320
	add	w0, w0, 1
	add	x1, x9, x1, uxtw 3
	cmp	w0, w5
	sub	x1, x1, #4096
	add	x2, x2, 4
	ldr	x1, [x1, 3824]
	str	x1, [x4], 8
	bne	.L734
	cbnz	w8, .L735
	b	.L744
	.p2align 3
.L732:
	udiv	w1, w0, w3
	msub	w1, w1, w3, w0
	b	.L733
	.p2align 3
.L737:
	udiv	w0, w1, w3
	msub	w0, w0, w3, w1
	b	.L738
	.p2align 3
.L777:
	add	x21, x19, 53248
	ldr	x3, [x20, 96]
	mov	x2, 272
	mov	w1, 0
	add	x0, x19, 2368
	str	wzr, [x21, 1920]
	str	wzr, [x21, 1924]
	blr	x3
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L742
	ldr	x1, [x21, 1128]
	mov	w0, 1
	str	w0, [x21, 1920]
	mov	w0, 0
	add	x1, x1, 40
	str	x1, [x19, 2368]
	b	.L772
.L736:
	ldr	x3, [x20, 104]
	mov	x2, 136
	add	x1, x19, 2368
	add	x0, x19, 2504
	blr	x3
	ldr	w7, [x22, 1920]
	str	w7, [x22, 1924]
	mov	w0, w7
	b	.L735
.L747:
	mov	w5, 0
	b	.L712
.L748:
	mov	w3, w5
	mov	w6, 0
	b	.L715
.L779:
	mov	w0, 0
	b	.L741
.L746:
	mov	w0, -1
	b	.L772
	.size	HEVC_InitListX, .-HEVC_InitListX
	.align	3
.LC189:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecList
	.type	HEVC_DecList, %function
HEVC_DecList:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 53248
	ldr	w1, [x0, 1820]
	cmp	w1, 2
	beq	.L786
	mov	x0, x19
	bl	HEVC_InitListX
	cbnz	w0, .L787
	mov	x0, x19
	bl	HEVC_RepairList
	mov	w1, 0
	cbnz	w0, .L788
.L782:
	mov	w0, w1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L786:
	str	wzr, [x0, 1920]
	mov	w1, 0
	str	wzr, [x0, 1924]
	mov	w0, w1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L787:
	ldr	x1, .LC191
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L782
.L788:
	ldr	x1, .LC193
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L782
	.size	HEVC_DecList, .-HEVC_DecList
	.align	3
.LC191:
	.xword	.LC190
	.align	3
.LC193:
	.xword	.LC192
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RemoveApcStore
	.type	HEVC_RemoveApcStore, %function
HEVC_RemoveApcStore:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, x1, uxtw 3
	bl	_mcount
	ldr	x2, [x20, 2704]
	ldr	w0, [x2, 28]
	cmp	w0, 16
	beq	.L789
	add	x3, x19, x0, uxtw 2
	ldr	x1, .LC195
	mov	w0, 13
	str	wzr, [x3, 3256]
	ldr	w3, [x2, 28]
	add	x3, x19, x3, lsl 2
	str	wzr, [x3, 3320]
	ldr	w3, [x2, 28]
	add	x3, x19, x3, lsl 2
	str	wzr, [x3, 3384]
	ldr	w3, [x2, 24]
	add	x3, x19, x3, lsl 2
	str	wzr, [x3, 2116]
	ldp	w3, w2, [x2, 24]
	bl	dprint_vfmw
	ldr	x0, [x20, 2704]
	mov	w1, 16
	str	w1, [x0, 28]
	ldr	w0, [x19, 3252]
	sub	w0, w0, #1
	str	w0, [x19, 3252]
.L789:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_RemoveApcStore, .-HEVC_RemoveApcStore
	.align	3
.LC195:
	.xword	.LC194
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RemoveUnusedApcStore
	.type	HEVC_RemoveUnusedApcStore, %function
HEVC_RemoveUnusedApcStore:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, 0
	bl	_mcount
	ldr	w1, [x20, 2684]
	cbnz	w1, .L799
	b	.L794
	.p2align 3
.L796:
	add	w19, w19, 1
	cmp	w1, w19
	bls	.L794
.L799:
	add	x0, x20, x19, sxtw 3
	ldr	x0, [x0, 2704]
	ldrb	w0, [x0, 1]
	cbnz	w0, .L796
	mov	w1, w19
	mov	x0, x20
	add	w19, w19, 1
	bl	HEVC_RemoveApcStore
	ldr	w1, [x20, 2684]
	cmp	w1, w19
	bhi	.L799
.L794:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_RemoveUnusedApcStore, .-HEVC_RemoveUnusedApcStore
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ApplyReferencePictureSet
	.type	HEVC_ApplyReferencePictureSet, %function
HEVC_ApplyReferencePictureSet:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x26, x0
	mov	x0, x30
	add	x23, x26, 53248
	mov	w22, 1
	bl	_mcount
	mov	w27, 0
	ldr	x1, [x26, 2336]
	mov	w5, 2024
	ldr	w2, [x23, 1836]
	mov	w4, 26164
	ldr	x0, [x26, 2328]
	add	x19, x26, 57344
	ldr	w3, [x26, 2684]
	smaddl	x1, w2, w5, x1
	ldr	w1, [x1, 32]
	smaddl	x0, w1, w4, x0
	ldr	w25, [x0, 128]
	lsl	w25, w22, w25
	cbz	w3, .L801
	add	x19, x19, 932
	mov	w24, w22
	.p2align 2
.L830:
	ldp	w2, w0, [x19, 16]
	ldr	w1, [x19, 24]
	sxtw	x21, w27
	add	w0, w2, w0
	cmp	w0, w1
	mov	w20, w0
	bcs	.L839
	add	x28, x26, x21, lsl 3
	mov	w4, 0
	ldr	x2, [x28, 2704]
	ldr	w1, [x2, 12]
	.p2align 2
.L810:
	add	x0, x26, x20, sxtw 2
	add	x3, x0, 53248
	ldr	w3, [x3, 3284]
	cbz	w3, .L805
	ldr	w3, [x2, 56]
	cbz	w3, .L806
	add	x3, x0, 57344
	ldr	w3, [x3, 1072]
	cmp	w3, w1
	beq	.L838
.L806:
	cbnz	w4, .L837
	add	x0, x0, 57344
	ldrb	w4, [x2, 1]
	mov	w3, w1
	cmp	w4, wzr
	ldr	w0, [x0, 1072]
	ccmp	w3, w0, 0, ne
	beq	.L840
.L823:
	mov	w3, w1
	mov	w4, 0
.L809:
	ldr	w0, [x19, 24]
	add	w20, w20, 1
	mov	w1, w3
	cmp	w0, w20
	bhi	.L810
	ldp	w1, w0, [x19, 16]
	add	w0, w1, w0
.L804:
	ldr	w5, [x23, 1808]
	cbz	w0, .L812
	mov	w20, 0
	lsl	x6, x21, 3
	.p2align 2
.L814:
	ldr	w0, [x2, 56]
	add	x1, x19, x20, sxtw 2
	add	w20, w20, 1
	cbnz	w0, .L813
	ldr	w0, [x1, 72]
	add	w0, w5, w0
	cmp	w0, w3
	beq	.L841
.L813:
	ldp	w1, w0, [x19, 16]
	add	w0, w1, w0
	cmp	w0, w20
	bhi	.L814
.L812:
	cmp	w4, wzr
	ccmp	w3, w5, 4, eq
	beq	.L816
	add	x21, x26, x21, lsl 3
	strb	wzr, [x2, 1]
	ldr	x0, [x21, 2704]
	str	wzr, [x0, 56]
	ldr	x0, [x21, 2704]
	ldr	w1, [x0, 8]
	cmp	w1, 1
	beq	.L816
	ldr	w1, [x0, 32]
	mov	w2, 0
	ldr	w0, [x26, 244]
	bl	FSP_SetRef
.L816:
	ldr	w0, [x26, 2684]
	add	w27, w27, 1
	cmp	w0, w27
	bhi	.L830
.L801:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L805:
	ldr	w3, [x2, 56]
	cbz	w3, .L808
	sdiv	w3, w1, w25
	add	x5, x0, 57344
	ldr	w6, [x5, 1072]
	sdiv	w5, w6, w25
	msub	w3, w3, w25, w1
	msub	w5, w5, w25, w6
	cmp	w3, w5
	bne	.L808
.L838:
	strb	w24, [x2, 1]
	mov	w2, 1
	ldr	x0, [x28, 2704]
	str	w22, [x0, 4]
	ldr	x1, [x28, 2704]
	ldr	w0, [x26, 244]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x2, [x28, 2704]
	mov	w4, 1
	ldr	w3, [x2, 12]
	b	.L809
	.p2align 3
.L808:
	cbnz	w4, .L837
	sdiv	w3, w1, w25
	add	x0, x0, 57344
	ldr	w4, [x0, 1072]
	sdiv	w0, w4, w25
	msub	w3, w3, w25, w1
	msub	w0, w0, w25, w4
	ldrb	w4, [x2, 1]
	cmp	w4, wzr
	ccmp	w3, w0, 0, ne
	bne	.L823
.L840:
	strb	w24, [x2, 1]
	mov	w2, 1
	ldr	x0, [x28, 2704]
	str	w22, [x0, 56]
	ldr	x0, [x28, 2704]
	str	w22, [x0, 4]
	ldr	x1, [x28, 2704]
	ldr	w0, [x26, 244]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x2, [x28, 2704]
	mov	w4, 1
	ldr	w3, [x2, 12]
	b	.L809
	.p2align 3
.L837:
	mov	w3, w1
	b	.L809
	.p2align 3
.L841:
	add	x28, x26, x6
	strb	w24, [x2, 1]
	mov	w2, 1
	str	x6, [x29, 104]
	ldr	x0, [x28, 2704]
	str	w22, [x0, 4]
	ldr	x1, [x28, 2704]
	ldr	w0, [x26, 244]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x2, [x28, 2704]
	mov	w4, 1
	ldr	w5, [x23, 1808]
	ldr	x6, [x29, 104]
	ldr	w3, [x2, 12]
	b	.L813
.L839:
	add	x1, x26, x21, lsl 3
	mov	w4, 0
	ldr	x2, [x1, 2704]
	ldr	w3, [x2, 12]
	b	.L804
	.size	HEVC_ApplyReferencePictureSet, .-HEVC_ApplyReferencePictureSet
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CheckThatAllRefPicsAreAvailable
	.type	HEVC_CheckThatAllRefPicsAreAvailable, %function
HEVC_CheckThatAllRefPicsAreAvailable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	add	x7, x19, 53248
	ldr	x0, [x19, 2336]
	mov	w4, 2024
	add	x17, x19, 57344
	add	x18, x17, 932
	ldr	x1, [x19, 2328]
	ldr	w2, [x7, 1836]
	mov	w3, 26164
	ldr	w8, [x18, 16]
	ldr	w30, [x18, 24]
	smaddl	x0, w2, w4, x0
	ldr	w2, [x18, 20]
	ldr	w0, [x0, 32]
	add	w8, w8, w2
	cmp	w8, w30
	mov	w15, w8
	smaddl	x1, w0, w3, x1
	mov	w0, 1
	ldr	w5, [x1, 128]
	lsl	w5, w0, w5
	bcs	.L843
	ldr	w3, [x19, 2684]
	mov	w10, 0
	mov	w9, 0
	.p2align 2
.L844:
	sxtw	x16, w15
	cbz	w3, .L850
	add	x11, x19, x16, lsl 2
	mov	w14, 0
	add	x1, x11, 53248
	mov	w0, 0
	add	x11, x11, 57344
	ldr	w4, [x1, 3284]
	b	.L847
	.p2align 3
.L884:
	ldr	x1, [x2, 2704]
	ldr	w2, [x1, 56]
	cbz	w2, .L846
	ldr	w6, [x1, 12]
	ldr	w2, [x11, 1072]
	cmp	w6, w2
	beq	.L882
.L846:
	add	w0, w0, 1
	cmp	w0, w3
	beq	.L883
.L847:
	add	x2, x19, x0, sxtw 3
	cbnz	w4, .L884
	ldr	x1, [x2, 2704]
	ldr	w2, [x1, 56]
	cbz	w2, .L846
	ldr	w13, [x1, 12]
	ldr	w12, [x11, 1072]
	sdiv	w6, w13, w5
	sdiv	w2, w12, w5
	msub	w6, w6, w5, w13
	msub	w2, w2, w5, w12
	cmp	w6, w2
	bne	.L846
.L882:
	ldrb	w1, [x1, 1]
	add	w0, w0, 1
	cmp	w1, wzr
	csinc	w14, w14, wzr, eq
	cmp	w0, w3
	bne	.L847
	.p2align 2
.L883:
	cbz	w14, .L850
.L848:
	add	w15, w15, 1
	cmp	w30, w15
	bne	.L844
	cbz	w8, .L852
.L851:
	mov	w12, 0
	add	x17, x17, 932
	.p2align 2
.L856:
	mov	w11, 0
	mov	w0, 0
	cbz	w3, .L858
	sxtw	x6, w12
	add	x6, x6, 16
	add	x6, x17, x6, lsl 2
	b	.L874
	.p2align 3
.L853:
	cmp	w0, w3
	beq	.L885
.L874:
	add	x2, x19, x0, sxtw 3
	add	w0, w0, 1
	ldr	x2, [x2, 2704]
	ldr	w4, [x2, 56]
	cbnz	w4, .L853
	ldr	w4, [x6, 8]
	ldr	w5, [x7, 1808]
	ldr	w1, [x2, 12]
	add	w4, w5, w4
	cmp	w1, w4
	bne	.L853
	ldrb	w1, [x2, 1]
	cmp	w1, wzr
	csinc	w11, w11, wzr, eq
	cmp	w0, w3
	bne	.L874
	.p2align 2
.L885:
	cbz	w11, .L858
.L855:
	add	w12, w12, 1
	cmp	w8, w12
	bhi	.L856
.L852:
	mov	w0, 1
	cbz	w9, .L873
	mov	w0, 0
	str	w10, [x20]
.L873:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L858:
	sxtw	x1, w12
	ldr	w4, [x7, 1808]
	ldr	w2, [x19, 200]
	add	x0, x17, x1, lsl 2
	ldr	w0, [x0, 72]
	add	w0, w4, w0
	cmp	w0, w2
	blt	.L855
	add	x1, x17, x1
	ldrb	w1, [x1, 53]
	cmp	w1, wzr
	csel	w10, w10, w0, eq
	csinc	w9, w9, wzr, eq
	b	.L855
	.p2align 3
.L850:
	add	x1, x18, x16, lsl 2
	ldr	w4, [x7, 1808]
	ldr	w2, [x19, 200]
	ldr	w0, [x1, 72]
	add	w0, w4, w0
	cmp	w0, w2
	blt	.L848
	add	x16, x18, x16
	ldrb	w1, [x16, 53]
	cmp	w1, wzr
	csel	w10, w10, w0, eq
	csinc	w9, w9, wzr, eq
	b	.L848
.L843:
	cbz	w8, .L873
	mov	w10, 0
	mov	w9, 0
	ldr	w3, [x19, 2684]
	b	.L851
	.size	HEVC_CheckThatAllRefPicsAreAvailable, .-HEVC_CheckThatAllRefPicsAreAvailable
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_SetFrmRepeatCount
	.type	HEVC_SetFrmRepeatCount, %function
HEVC_SetFrmRepeatCount:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bls	.L887
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L887:
	str	wzr, [x20, 24]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_SetFrmRepeatCount, .-HEVC_SetFrmRepeatCount
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetFreeFrameStoreNum
	.type	HEVC_GetFreeFrameStoreNum, %function
HEVC_GetFreeFrameStoreNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 65536
	add	x5, x19, 81920
	add	x2, x2, 3672
	add	x5, x5, 1528
	mov	w0, 0
	.p2align 2
.L894:
	ldr	w1, [x2, 4]
	cbnz	w1, .L890
	cbz	x2, .L891
	ldr	w4, [x19, 2684]
	cbz	w4, .L891
	ldr	x1, [x19, 2704]
	cmp	x1, x2
	beq	.L890
	sub	w4, w4, #1
	mov	x1, x19
	add	x4, x19, x4, lsl 3
	b	.L892
	.p2align 3
.L893:
	ldr	x3, [x1, 2704]
	cmp	x3, x2
	beq	.L890
.L892:
	cmp	x4, x1
	add	x1, x1, 8
	bne	.L893
.L891:
	add	w0, w0, 1
.L890:
	add	x2, x2, 712
	cmp	x2, x5
	bne	.L894
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_GetFreeFrameStoreNum, .-HEVC_GetFreeFrameStoreNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_UpdateReflist
	.type	HEVC_UpdateReflist, %function
HEVC_UpdateReflist:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	add	x22, x21, 57344
	mov	w23, 0
	add	x24, x22, 932
	bl	_mcount
	mov	w20, 0
	ldr	w1, [x24, 16]
	cbz	w1, .L905
	add	x25, x21, 53248
	mov	w27, 1
	mov	w26, 1
	.p2align 2
.L912:
	uxtw	x0, w20
	add	x1, x24, x0
	ldrb	w1, [x1, 53]
	cbz	w1, .L906
	add	x0, x24, x0, lsl 2
	ldr	w5, [x21, 2684]
	ldr	w4, [x25, 1808]
	ldr	w0, [x0, 72]
	add	w4, w4, w0
	cbz	w5, .L906
	ldr	x1, [x21, 2704]
	ldr	w0, [x1, 12]
	cmp	w4, w0
	beq	.L962
	add	x2, x21, 2712
	mov	w0, 0
	b	.L909
	.p2align 3
.L911:
	ldr	x1, [x2], 8
	ldr	w3, [x1, 12]
	cmp	w4, w3
	beq	.L907
.L909:
	add	w0, w0, 1
	cmp	w0, w5
	bne	.L911
.L906:
	ldr	w1, [x24, 16]
	add	w20, w20, 1
	cmp	w1, w20
	bhi	.L912
.L905:
	ldr	w0, [x21, 2680]
	str	w23, [x21, 2696]
	cmp	w0, w23
	bls	.L917
	.p2align 2
.L945:
	add	x2, x21, x23, uxtw 3
	add	w23, w23, 1
	cmp	w23, w0
	str	xzr, [x2, 2840]
	bne	.L945
.L917:
	add	x23, x22, 932
	ldr	w25, [x23, 20]
	mov	w22, 0
	add	w25, w1, w25
	cmp	w25, w20
	bls	.L915
	add	x24, x21, 53248
	mov	w27, 1
	mov	w26, 1
	.p2align 2
.L944:
	uxtw	x0, w20
	add	x1, x23, x0
	ldrb	w1, [x1, 53]
	cbz	w1, .L918
	add	x0, x23, x0, lsl 2
	ldr	w5, [x21, 2684]
	ldr	w4, [x24, 1808]
	ldr	w0, [x0, 72]
	add	w4, w4, w0
	cbz	w5, .L918
	ldr	x1, [x21, 2704]
	ldr	w0, [x1, 12]
	cmp	w4, w0
	beq	.L963
	add	x2, x21, 2712
	mov	w0, 0
	b	.L921
	.p2align 3
.L923:
	ldr	x1, [x2], 8
	ldr	w3, [x1, 12]
	cmp	w4, w3
	beq	.L919
.L921:
	add	w0, w0, 1
	cmp	w0, w5
	bne	.L923
.L918:
	add	w20, w20, 1
	cmp	w25, w20
	bhi	.L944
.L966:
	ldr	w0, [x21, 2680]
.L915:
	cmp	w0, w22
	str	w22, [x21, 2700]
	bls	.L904
	.p2align 2
.L943:
	add	x1, x21, x22, uxtw 3
	add	w22, w22, 1
	cmp	w0, w22
	str	xzr, [x1, 2976]
	bne	.L943
.L904:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L962:
	mov	w0, 0
	.p2align 2
.L907:
	add	x19, x21, x0, uxtw 3
	strb	w27, [x1, 1]
	ldr	x0, [x19, 2704]
	ldrb	w1, [x0, 1]
	cbnz	w1, .L964
.L910:
	str	w26, [x0, 60]
	add	x0, x21, x23, uxtw 3
	add	w20, w20, 1
	add	w23, w23, 1
	ldr	x1, [x19, 2704]
	str	wzr, [x1, 56]
	ldr	x1, [x19, 2704]
	str	x1, [x0, 2840]
	ldr	w1, [x24, 16]
	cmp	w1, w20
	bhi	.L912
	b	.L905
.L963:
	mov	w0, 0
	.p2align 2
.L919:
	add	x19, x21, x0, uxtw 3
	strb	w27, [x1, 1]
	ldr	x0, [x19, 2704]
	ldrb	w1, [x0, 1]
	cbnz	w1, .L965
.L922:
	str	w26, [x0, 60]
	add	x0, x21, x22, uxtw 3
	add	w20, w20, 1
	add	w22, w22, 1
	ldr	x1, [x19, 2704]
	cmp	w25, w20
	str	wzr, [x1, 56]
	ldr	x1, [x19, 2704]
	str	x1, [x0, 2976]
	bhi	.L944
	b	.L966
	.p2align 3
.L964:
	ldr	w1, [x0, 32]
	mov	w2, 1
	ldr	w0, [x21, 244]
	bl	FSP_SetRef
	ldr	x0, [x19, 2704]
	b	.L910
	.p2align 3
.L965:
	ldr	w1, [x0, 32]
	mov	w2, 1
	ldr	w0, [x21, 244]
	bl	FSP_SetRef
	ldr	x0, [x19, 2704]
	b	.L922
	.size	HEVC_UpdateReflist, .-HEVC_UpdateReflist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_UpdateLTReflist
	.type	HEVC_UpdateLTReflist, %function
HEVC_UpdateLTReflist:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 53248
	ldr	x0, [x19, 2336]
	add	x11, x19, 57344
	mov	w1, 2024
	add	x11, x11, 932
	ldr	x3, [x19, 2328]
	ldr	w2, [x2, 1836]
	mov	w4, 26164
	ldr	w10, [x11, 24]
	mov	w12, 1
	smaddl	x1, w2, w1, x0
	ldp	w2, w0, [x11, 16]
	sub	w10, w10, #1
	ldr	w1, [x1, 32]
	add	w0, w2, w0
	sub	w0, w0, #1
	cmp	w10, w0
	smaddl	x0, w1, w4, x3
	ldr	w0, [x0, 128]
	lsl	w0, w12, w0
	ble	.L977
	mov	w9, 0
	sub	w7, w0, #1
	mov	w13, w12
	.p2align 2
.L974:
	sxtw	x0, w10
	add	x1, x11, x0
	ldrb	w1, [x1, 53]
	cbz	w1, .L969
	add	x0, x19, x0, lsl 2
	ldr	w8, [x19, 2684]
	add	x1, x0, 57344
	add	x0, x0, 53248
	ldr	w5, [x1, 1072]
	ldrb	w6, [x0, 3284]
	and	w0, w5, w7
	cmp	w6, wzr
	csel	w5, w0, w5, eq
	cmp	w8, wzr
	ble	.L969
	add	x2, x19, 2704
	mov	w1, 0
	b	.L973
	.p2align 3
.L972:
	add	w1, w1, 1
	cmp	w1, w8
	beq	.L969
.L973:
	ldr	x3, [x2]
	cmp	w6, wzr
	add	x2, x2, 8
	ldr	w0, [x3, 12]
	and	w4, w0, w7
	csel	w0, w4, w0, eq
	cmp	w5, w0
	bne	.L972
	add	x1, x19, x1, sxtw 3
	strb	w13, [x3, 1]
	add	x0, x19, x9, sxtw 3
	add	w9, w9, 1
	ldr	x2, [x1, 2704]
	str	wzr, [x2, 60]
	ldr	x2, [x1, 2704]
	str	w12, [x2, 56]
	ldr	x1, [x1, 2704]
	str	x1, [x0, 3112]
.L969:
	ldp	w1, w0, [x11, 16]
	sub	w10, w10, #1
	add	w0, w1, w0
	sub	w0, w0, #1
	cmp	w0, w10
	blt	.L974
	mov	w0, w9
.L968:
	ldr	w1, [x19, 2680]
	str	w0, [x19, 2692]
	cmp	w1, w9
	ble	.L967
	.p2align 2
.L980:
	add	x0, x19, x9, sxtw 3
	add	w9, w9, 1
	cmp	w9, w1
	str	xzr, [x0, 3112]
	bne	.L980
.L967:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L977:
	mov	w0, 0
	mov	w9, 0
	b	.L968
	.size	HEVC_UpdateLTReflist, .-HEVC_UpdateLTReflist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetAPC
	.type	HEVC_GetAPC, %function
HEVC_GetAPC:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x21, x1
	mov	x0, x30
	bl	_mcount
	cbz	x21, .L986
	ldr	w2, [x20, 3248]
	cbz	w2, .L995
	ldr	w0, [x20, 3256]
	cbz	w0, .L1001
	add	x0, x20, 3260
	mov	w19, 0
	b	.L990
	.p2align 3
.L993:
	ldr	w1, [x0], 4
	cbz	w1, .L988
.L990:
	add	w19, w19, 1
	cmp	w19, w2
	bne	.L993
.L995:
	mov	w0, -7
.L999:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1001:
	mov	w19, 0
	.p2align 2
.L988:
	ldr	w0, [x20, 244]
	ldr	w1, [x21, 32]
	bl	FSP_GetLogicFs
	mov	x5, x0
	cbz	x0, .L991
	ldr	x0, [x0, 640]
	cbz	x0, .L991
	add	x2, x20, x19, uxtw 2
	mov	w4, 1
	ldr	x1, .LC199
	mov	w3, w19
	mov	w0, 13
	str	w4, [x2, 3256]
	ldr	x4, [x5, 640]
	ldr	w4, [x4, 8]
	str	w4, [x2, 3320]
	ldr	w4, [x21, 12]
	str	w4, [x2, 3384]
	ldr	w2, [x20, 3252]
	add	w2, w2, 1
	str	w2, [x20, 3252]
	ldr	w2, [x21, 236]
	bl	dprint_vfmw
	str	w19, [x21, 28]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L991:
	ldrb	w4, [x21, 1]
	mov	x6, 0
	ldr	x1, .LC201
	mov	w0, 0
	ldr	w3, [x21, 12]
	ldr	w2, [x21, 32]
	bl	dprint_vfmw
	mov	w0, -7
	b	.L999
.L986:
	ldr	x2, .LC196
	mov	w3, 9276
	ldr	x1, .LC197
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -7
	b	.L999
	.size	HEVC_GetAPC, .-HEVC_GetAPC
	.align	3
.LC196:
	.xword	.LC99
	.align	3
.LC197:
	.xword	.LC101
	.align	3
.LC199:
	.xword	.LC198
	.align	3
.LC201:
	.xword	.LC200
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CheckFrameStore
	.type	HEVC_CheckFrameStore, %function
HEVC_CheckFrameStore:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 244]
	bl	FSP_GetLogicFs
	cbz	x0, .L1003
	ldrsb	w4, [x0, 1]
	cmp	w4, 3
	ccmp	w4, wzr, 4, ne
	beq	.L1023
	ldr	w0, [x19, 8]
	cmp	w0, 1
	beq	.L1009
	ldr	w4, [x19, 4]
	ldrb	w5, [x19]
	cbz	w4, .L1008
	cmp	w5, 1
	mov	w0, 0
	beq	.L1008
.L1017:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1023:
	ldr	x2, .LC202
	mov	w0, 13
	ldr	x1, .LC204
	ldr	w3, [x19, 32]
	add	x2, x2, 352
	bl	dprint_vfmw
.L1005:
	ldr	w0, [x19, 8]
	cmp	w0, 1
	bne	.L1024
.L1009:
	mov	w1, 1
	mov	w0, 2
	strb	w1, [x19, 2]
	str	w0, [x19, 8]
	mov	w0, -3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1003:
	ldr	x1, .LC206
	mov	x2, x19
	ldp	w4, w3, [x19, 8]
	ldr	w5, [x19, 32]
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1005
.L1024:
	ldr	x2, .LC202
	mov	w0, 1
	ldr	x1, .LC208
	ldr	w3, [x19, 12]
	add	x2, x2, 352
	bl	dprint_vfmw
	mov	w0, -3
	b	.L1017
	.p2align 3
.L1008:
	ldr	x2, .LC202
	mov	x3, x19
	ldr	x1, .LC210
	mov	w0, 1
	add	x2, x2, 352
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1017
	.size	HEVC_CheckFrameStore, .-HEVC_CheckFrameStore
	.align	3
.LC202:
	.xword	.LANCHOR1
	.align	3
.LC204:
	.xword	.LC203
	.align	3
.LC206:
	.xword	.LC205
	.align	3
.LC208:
	.xword	.LC207
	.align	3
.LC210:
	.xword	.LC209
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetImagePara
	.type	HEVC_GetImagePara, %function
HEVC_GetImagePara:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 188]
	mov	w0, -16132
	mov	w2, 25600
	str	w2, [x19, 216]
	and	w1, w1, w0
	str	w1, [x19, 188]
	ldr	w3, [x19, 16]
	ldr	x0, [x20, 28128]
	ldr	w2, [x0, 12468]
	and	w2, w2, 16776960
	cmp	w2, 256
	beq	.L1033
	mov	w0, 256
	movk	w0, 0x1, lsl 16
	cmp	w2, w0
	bne	.L1034
	ldr	w0, [x20, 5256]
	cmp	w0, 1
	cset	w2, ne
.L1026:
	ldr	x4, [x20, 28136]
	mov	w0, 3072
	ldrb	w4, [x4, 2633]
	cbz	w4, .L1027
	ldr	w2, [x19, 704]
	mov	w0, 1024
	cmp	w2, 1
	ccmp	w2, 11, 4, ne
	cset	w4, eq
	cbz	w4, .L1047
.L1028:
	ldr	w2, [x19, 708]
	cmp	w2, wzr
	cset	w2, eq
.L1027:
	and	w3, w3, 3
	orr	w2, w0, w2, lsl 8
	orr	w1, w3, w1
	mov	w3, 262144
	orr	w0, w2, w1
	ldr	w2, [x19, 4]
	and	w0, w0, -131073
	orr	w0, w0, 8192
	str	w0, [x19, 188]
	ldr	x1, [x20, 2344]
	ldr	w1, [x1, 592]
	cmp	w1, 1
	csel	w1, wzr, w3, ne
	orr	w0, w1, w0
	str	w0, [x19, 188]
	cbz	w2, .L1030
	ldr	w0, [x19, 20]
	str	w0, [x19, 240]
.L1030:
	ldr	w0, [x20, 244]
	ldr	w1, [x19, 32]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L1025
	ldr	x3, .LC211
	mov	x2, 624
	add	x1, x19, 80
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 16
	str	w0, [x20, 376]
	mov	w0, 4
	mov	w1, 1
	ldr	w2, [x19, 52]
	strb	w1, [x20, 371]
	cmp	w2, wzr
	strb	w1, [x20, 369]
	csel	w0, w0, wzr, ne
	strb	w0, [x20, 364]
.L1025:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1047:
	and	w4, w2, -9
	mov	w0, 3072
	mov	w2, 2048
	cmp	w4, 2
	csel	w0, w0, w2, ne
	b	.L1028
	.p2align 3
.L1033:
	mov	w2, 0
	b	.L1026
	.p2align 3
.L1034:
	mov	w2, 1
	b	.L1026
	.size	HEVC_GetImagePara, .-HEVC_GetImagePara
	.align	3
.LC211:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_OutputCurrPic
	.type	HEVC_OutputCurrPic, %function
HEVC_OutputCurrPic:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x22, x19, 53248
	add	x23, x22, 1072
	bl	_mcount
	mov	x2, 624
	add	x1, x22, 1136
	ldr	x20, [x22, 1128]
	ldr	w3, [x23, 40]
	add	x0, x20, 80
	str	w3, [x20, 20]
	bl	memcpy
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_GetImagePara
	ldr	w0, [x19, 244]
	ldr	w1, [x20, 32]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L1070
	ldr	w0, [x20, 20]
	str	w0, [x21, 160]
	cbnz	w0, .L1051
.L1056:
	ldrb	w0, [x22, 1072]
	cbnz	w0, .L1071
.L1053:
	ldr	w0, [x19, 244]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	add	x3, x19, 61440
	ldr	w0, [x19, 244]
	mov	x4, x21
	add	x3, x3, 1032
	mov	x2, x19
	mov	w1, 16
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L1072
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L1073
	str	wzr, [x21, 24]
.L1060:
	ldr	w1, [x19, 276]
	mov	w0, 0
	ldr	w2, [x19, 284]
	add	w1, w1, 1
	str	w1, [x19, 276]
	add	w2, w2, 1
	str	w2, [x19, 284]
.L1050:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1051:
	ldr	w1, [x23, 12]
	cbz	w1, .L1074
.L1054:
	ldr	x1, [x19, 2344]
	ldr	w1, [x1, 4]
	cmp	w1, w0
	bcs	.L1056
	ldr	w0, [x19, 244]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	ldr	x4, [x19, 2344]
	mov	w0, 1
	ldr	x2, .LC212
	ldr	x1, .LC221
	ldr	w3, [x21, 160]
	add	x2, x2, 376
	ldr	w4, [x4, 4]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1050
	.p2align 3
.L1071:
	ldr	w0, [x19, 244]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetRef
	b	.L1053
	.p2align 3
.L1073:
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.3
	b	.L1060
	.p2align 3
.L1074:
	ldr	x2, .LC212
	mov	w0, 1
	ldr	x1, .LC216
	add	x2, x2, 376
	bl	dprint_vfmw
	ldr	x0, .LC217
	ldr	x4, [x0]
	cbz	x4, .L1055
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 111
	blr	x4
.L1055:
	ldr	w0, [x21, 160]
	cbnz	w0, .L1054
	b	.L1056
	.p2align 3
.L1072:
	ldr	x2, .LC212
	mov	w0, 1
	ldr	x1, .LC219
	add	x2, x2, 376
	bl	dprint_vfmw
	ldr	w0, [x19, 244]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	mov	w0, -1
	b	.L1050
.L1070:
	ldr	x2, .LC212
	mov	w0, 0
	ldr	x1, .LC214
	add	x2, x2, 376
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1050
	.size	HEVC_OutputCurrPic, .-HEVC_OutputCurrPic
	.align	3
.LC212:
	.xword	.LANCHOR1
	.align	3
.LC214:
	.xword	.LC213
	.align	3
.LC216:
	.xword	.LC215
	.align	3
.LC217:
	.xword	g_event_report
	.align	3
.LC219:
	.xword	.LC218
	.align	3
.LC221:
	.xword	.LC220
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_FirstFrameFastOut
	.type	HEVC_FirstFrameFastOut, %function
HEVC_FirstFrameFastOut:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	HEVC_OutputCurrPic
	mov	w20, w0
	cbnz	w0, .L1078
	add	x0, x19, 53248
	add	x2, x19, 61440
	mov	w4, 2
	mov	w1, 1
	ldr	x3, [x0, 1128]
	str	w4, [x3, 8]
	str	w1, [x2, 1640]
	ldr	w2, [x0, 1080]
	str	w2, [x19, 212]
	ldr	x0, [x0, 1128]
	strb	w1, [x0, 2]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1078:
	ldr	x2, .LC222
	mov	w0, 1
	ldr	x1, .LC224
	add	x2, x2, 400
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_FirstFrameFastOut, .-HEVC_FirstFrameFastOut
	.align	3
.LC222:
	.xword	.LANCHOR1
	.align	3
.LC224:
	.xword	.LC223
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CheckOutputImage
	.type	HEVC_CheckOutputImage, %function
HEVC_CheckOutputImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 19
	bhi	.L1081
	uxtw	x1, w19
	lsl	x0, x1, 2
	sub	x0, x0, x1
	lsl	x2, x0, 4
	sub	x0, x2, x0
	lsl	x0, x0, 1
	sub	x0, x0, x1
	add	x0, x20, x0, lsl 3
	add	x0, x0, 65536
	ldrb	w0, [x0, 3673]
	cmp	w0, wzr
	cset	w0, ne
.L1080:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1081:
	mov	w0, -1
	b	.L1080
	.size	HEVC_CheckOutputImage, .-HEVC_CheckOutputImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DECRecycleImage
	.type	HEVC_DECRecycleImage, %function
HEVC_DECRecycleImage:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w22, w1
	bl	_mcount
	ldr	w0, [x21, 244]
	mov	w1, w22
	bl	FSP_GetLogicFs
	cbz	x0, .L1087
	ldrsb	w1, [x0, 1]
	add	x19, x0, 568
	add	x20, x0, 600
	cbz	w1, .L1093
.L1090:
	ldr	x1, [x19]
	cbnz	x1, .L1094
.L1085:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L1090
	ldr	w0, [x21, 244]
	mov	w2, 0
	mov	w1, w22
	bl	FSP_SetDisplay
	mov	w0, 0
.L1083:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1094:
	ldr	w0, [x21, 244]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L1085
.L1093:
	ldr	x1, .LC226
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1083
.L1087:
	mov	w0, -1
	b	.L1083
	.size	HEVC_DECRecycleImage, .-HEVC_DECRecycleImage
	.align	3
.LC226:
	.xword	.LC225
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ArrangeVahbMem
	.type	HEVC_ArrangeVahbMem, %function
HEVC_ArrangeVahbMem:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	w22, w6
	bl	_mcount
	ldr	x3, .LC227
	mov	x2, 32
	mov	w1, 0
	add	x0, x29, 64
	ldr	x3, [x3, 96]
	blr	x3
	strb	wzr, [x29, 64]
	ldr	x0, [x19, 2344]
	stp	w20, w21, [x29, 88]
	strb	wzr, [x29, 65]
	ldr	w0, [x0, 28]
	stp	w20, w21, [x29, 72]
	stp	w20, w21, [x29, 80]
	cmp	w0, 24
	beq	.L1096
	ldr	w1, [x19, 2680]
	mov	w0, 5
	strb	wzr, [x29, 68]
	sxtb	w6, w22
	strb	w0, [x29, 69]
	strb	w1, [x29, 67]
.L1101:
	ldr	w0, [x19, 244]
	add	x1, x29, 64
	strb	w6, [x29, 70]
	strb	wzr, [x29, 66]
	bl	FSP_ConfigInstance
	cbnz	w0, .L1097
	ldr	x1, [x19, 2344]
	add	x3, x29, 60
	ldr	w0, [x19, 244]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	mov	w1, w0
	beq	.L1105
	mov	w0, 1
	cbnz	w1, .L1099
.L1103:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1096:
	mov	w1, 1
	mov	w0, 5
	strb	w1, [x29, 67]
	mov	w6, 1
	strb	wzr, [x29, 68]
	strb	w0, [x29, 69]
	b	.L1101
	.p2align 3
.L1105:
	add	x0, x19, 61440
	add	x0, x0, 1032
	bl	ResetVoQueue
.L1099:
	ldr	x1, .LC229
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -20
	b	.L1103
	.p2align 3
.L1097:
	ldr	x1, .LC231
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -20
	b	.L1103
	.size	HEVC_ArrangeVahbMem, .-HEVC_ArrangeVahbMem
	.align	3
.LC227:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC229:
	.xword	.LC228
	.align	3
.LC231:
	.xword	.LC230
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ConvertHDRInfo
	.type	HEVC_ConvertHDRInfo, %function
HEVC_ConvertHDRInfo:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x21, x1
	mov	x0, x30
	mov	x20, x2
	bl	_mcount
	ldrb	w0, [x21, 4]
	add	x2, x19, 260
	strb	w0, [x19, 256]
	add	x3, x20, 3452
	add	x0, x20, 3440
	ldr	w1, [x21, 40]
	strb	w1, [x19, 257]
	ldr	w1, [x21, 48]
	strb	w1, [x19, 258]
	ldr	w1, [x21, 44]
	str	w1, [x19, 252]
	ldrb	w1, [x20, 3436]
	strb	w1, [x19, 248]
.L1107:
	ldr	w1, [x0]
	add	x0, x0, 4
	strh	w1, [x2]
	add	x2, x2, 2
	cmp	x0, x3
	ldr	w1, [x0, 8]
	strh	w1, [x2, 4]
	bne	.L1107
	ldr	w0, [x20, 3464]
	strh	w0, [x19, 272]
	ldr	w0, [x20, 3468]
	strh	w0, [x19, 274]
	ldr	w0, [x20, 3472]
	str	w0, [x19, 276]
	ldr	w0, [x20, 3476]
	str	w0, [x19, 280]
	ldrb	w0, [x20, 3480]
	strb	w0, [x19, 249]
	ldrh	w0, [x20, 3482]
	str	w0, [x19, 284]
	ldrh	w0, [x20, 3484]
	str	w0, [x19, 288]
	ldrb	w0, [x20, 3486]
	strb	w0, [x19, 292]
	ldrb	w0, [x20, 3228]
	cmp	w0, 1
	beq	.L1110
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1110:
	ldr	x3, .LC234
	add	x1, x20, 3232
	strb	w0, [x19, 296]
	mov	x2, 36
	add	x0, x19, 296
	ldr	w4, [x20, 3268]
	ldr	x3, [x3, 104]
	str	w4, [x19, 336]
	blr	x3
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_ConvertHDRInfo, .-HEVC_ConvertHDRInfo
	.align	3
.LC234:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_AllocFrameStore
	.type	HEVC_AllocFrameStore, %function
HEVC_AllocFrameStore:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	add	x22, x23, 53248
	add	x19, x23, 65536
	bl	_mcount
	add	x19, x19, 3672
	ldr	x0, [x23, 2336]
	mov	w5, 2024
	ldr	w1, [x22, 1836]
	mov	w4, 26164
	ldr	x2, [x23, 2328]
	mov	x3, 0
	smaddl	x0, w1, w5, x0
	ldr	w21, [x0, 32]
	strb	wzr, [x22, 1073]
	smaddl	x21, w21, w4, x2
	.p2align 2
.L1117:
	ldr	w0, [x19, 4]
	cbnz	w0, .L1112
	cbz	x19, .L1113
	ldr	w2, [x23, 2684]
	cbz	w2, .L1113
	ldr	x0, [x23, 2704]
	cmp	x0, x19
	beq	.L1112
	sub	w2, w2, #1
	mov	x0, x23
	add	x2, x23, x2, lsl 3
	b	.L1115
	.p2align 3
.L1116:
	ldr	x1, [x0, 2704]
	cmp	x1, x19
	beq	.L1112
.L1115:
	cmp	x0, x2
	add	x0, x0, 8
	bne	.L1116
.L1113:
	lsl	x20, x3, 2
	mov	w0, 101
	sub	x20, x20, x3
	str	w0, [x22, 1112]
	lsl	x0, x20, 4
	mov	w1, 1
	sub	x20, x0, x20
	strb	w1, [x22, 1073]
	lsl	x20, x20, x1
	mov	x2, 624
	sub	x3, x20, x3
	add	x0, x22, 1136
	add	x20, x23, x3, lsl 3
	add	x20, x20, 65536
	add	x1, x20, 3752
	bl	memcpy
	str	x19, [x22, 1128]
	str	wzr, [x22, 1296]
	strb	wzr, [x20, 3674]
	str	wzr, [x20, 3692]
	strb	wzr, [x19]
	ldr	x0, [x22, 1128]
	str	wzr, [x0, 56]
	ldr	x0, [x22, 1128]
	str	wzr, [x0, 60]
	ldr	x0, [x22, 1128]
	str	wzr, [x0, 8]
	ldrb	w1, [x22, 1073]
	cmp	w1, 1
	bne	.L1121
	ldr	w0, [x23, 244]
	bl	FSP_NewLogicFs
	mov	w24, w0
	mov	w1, w0
	ldr	w0, [x23, 244]
	bl	FSP_GetLogicFs
	mov	x20, x0
	cmp	w24, wzr
	ccmp	x0, xzr, 4, ge
	beq	.L1119
	add	x2, x23, 4096
	add	x1, x21, 2624
	add	x2, x2, 64
	add	x19, x0, 8
	bl	HEVC_ConvertHDRInfo
	ldr	x0, [x23, 2344]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L1134
.L1120:
	ldr	x21, .LC237
	mov	x2, 624
	ldr	x0, [x22, 1128]
	mov	x1, x19
	ldr	x3, [x21, 104]
	add	x0, x0, 80
	blr	x3
	ldr	x1, [x22, 1128]
	mov	x2, 624
	ldr	x3, [x21, 104]
	add	x0, x22, 1136
	add	x1, x1, 80
	blr	x3
	ldr	x1, [x22, 1128]
	mov	w0, 0
	str	w24, [x1, 32]
	ldrsb	w1, [x20, 4]
	str	w1, [x22, 1108]
.L1132:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1112:
	add	w3, w3, 1
	add	x19, x19, 712
	cmp	w3, 20
	bne	.L1117
.L1121:
	ldr	x1, .LC236
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -4
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L1134:
	ldr	x2, [x23, 28136]
	ldr	w21, [x19, 140]
	ldr	w1, [x19, 44]
	ldr	w0, [x2, 76]
	ldr	w2, [x2, 72]
	stp	w2, w0, [x19, 112]
	mul	w21, w0, w21
	add	w0, w21, w1
	str	w0, [x19, 48]
	bl	MEM_Phy2Vir
	str	x0, [x19, 512]
	ldr	w2, [x19, 28]
	ldr	w1, [x19, 60]
	ldr	w3, [x19, 140]
	add	w2, w21, w2
	add	w0, w21, w1
	str	w2, [x19, 32]
	add	w3, w3, w2
	str	w0, [x19, 64]
	str	w3, [x19, 40]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	b	.L1120
.L1119:
	ldr	x1, .LC239
	mov	w0, 0
	strb	wzr, [x22, 1073]
	bl	dprint_vfmw
	mov	w0, -4
	b	.L1132
	.size	HEVC_AllocFrameStore, .-HEVC_AllocFrameStore
	.align	3
.LC236:
	.xword	.LC235
	.align	3
.LC237:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC239:
	.xword	.LC238
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsVoFinish
	.type	HEVC_IsVoFinish, %function
HEVC_IsVoFinish:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, 0
	bl	_mcount
	ldr	w0, [x20, 244]
	bl	FSP_GetFspType
	cmn	w0, #1
	mov	w21, w0
	bne	.L1141
	b	.L1143
	.p2align 3
.L1138:
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	beq	.L1140
	cmp	w19, 32
	beq	.L1144
.L1141:
	ldr	w0, [x20, 244]
	mov	w1, w19
	add	w19, w19, 1
	bl	FSP_GetLogicFs
	cbnz	x0, .L1138
	ldr	x1, .LC243
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1143:
	ldr	x1, .LC241
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w21
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1140:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1144:
	mov	w0, 1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_IsVoFinish, .-HEVC_IsVoFinish
	.align	3
.LC241:
	.xword	.LC240
	.align	3
.LC243:
	.xword	.LC242
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetBackPicFromVOQueue
	.type	HEVC_GetBackPicFromVOQueue, %function
HEVC_GetBackPicFromVOQueue:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x22, 244]
	bl	FSP_GetFspType
	cbz	w0, .L1159
.L1146:
	add	x20, x22, 65536
	add	x23, x22, 81920
	add	x20, x20, 3673
	add	x23, x23, 1529
	.p2align 2
.L1149:
	strb	wzr, [x20]
	add	x19, x20, 639
	add	x21, x20, 671
	str	wzr, [x20, 3]
.L1148:
	ldr	x1, [x19]
	cbnz	x1, .L1160
.L1147:
	add	x19, x19, 8
	cmp	x19, x21
	bne	.L1148
	add	x20, x20, 712
	cmp	x20, x23
	bne	.L1149
	ldr	w0, [x22, 180]
	cbz	w0, .L1145
	sub	w1, w0, #1
	add	x0, x22, 2116
	add	x1, x1, 530
	add	x22, x22, x1, lsl 2
	.p2align 2
.L1151:
	str	wzr, [x0], 4
	cmp	x0, x22
	bne	.L1151
.L1145:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1160:
	ldr	w0, [x22, 244]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L1147
.L1159:
	add	x0, x22, 61440
	add	x0, x0, 1032
	bl	ResetVoQueue
	b	.L1146
	.size	HEVC_GetBackPicFromVOQueue, .-HEVC_GetBackPicFromVOQueue
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RemoveFrameStoreOutDPB
	.type	HEVC_RemoveFrameStoreOutDPB, %function
HEVC_RemoveFrameStoreOutDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	uxtw	x21, w1
	mov	x20, x0
	mov	x0, x30
	mov	x19, x21
	add	x22, x20, x21, lsl 3
	bl	_mcount
	ldr	x0, [x22, 2704]
	str	wzr, [x0, 4]
	ldr	x1, [x22, 2704]
	ldr	w0, [x20, 244]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	cbz	x0, .L1162
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	beq	.L1163
	ldr	x1, [x22, 2704]
	mov	w2, 0
	ldr	w0, [x20, 244]
	ldr	w1, [x1, 32]
	bl	FSP_SetDisplay
.L1163:
	ldr	x1, [x22, 2704]
	mov	w2, 0
	ldr	w0, [x20, 244]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
.L1162:
	ldr	w2, [x20, 2684]
	sub	w2, w2, #1
	cmp	w19, w2
	bhi	.L1166
	.p2align 2
.L1172:
	add	w0, w19, 1
	add	x1, x20, x21, lsl 3
	mov	w19, w0
	cmp	w0, w2
	mov	x21, x0
	add	x0, x20, x0, lsl 3
	ldr	x0, [x0, 2704]
	str	x0, [x1, 2704]
	bls	.L1172
.L1166:
	add	x0, x20, x2, uxtw 3
	str	xzr, [x0, 2704]
	str	w2, [x20, 2684]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_RemoveFrameStoreOutDPB, .-HEVC_RemoveFrameStoreOutDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RemoveDummyFrame
	.type	HEVC_RemoveDummyFrame, %function
HEVC_RemoveDummyFrame:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w21, 0
	mov	w19, 0
	bl	_mcount
	ldr	w2, [x20, 2684]
	cbnz	w2, .L1183
	b	.L1176
	.p2align 3
.L1178:
	add	w19, w19, 1
	cmp	w2, w19
	mov	w21, w19
	bls	.L1176
.L1183:
	add	x0, x20, x19, sxtw 3
	ldr	x0, [x0, 2704]
	ldrb	w1, [x0]
	cbz	w1, .L1178
	mov	w1, w21
	strb	wzr, [x0]
	mov	x0, x20
	sub	w19, w19, #1
	add	w19, w19, 1
	bl	HEVC_RemoveApcStore
	mov	w1, w21
	mov	x0, x20
	mov	w21, w19
	bl	HEVC_RemoveFrameStoreOutDPB
	ldr	w2, [x20, 2684]
	cmp	w2, w19
	bhi	.L1183
.L1176:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_RemoveDummyFrame, .-HEVC_RemoveDummyFrame
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RemoveUnUsedFrameStore
	.type	HEVC_RemoveUnUsedFrameStore, %function
HEVC_RemoveUnUsedFrameStore:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	mov	w25, 0
	bl	_mcount
	ldr	w22, [x21, 2684]
	cbz	w22, .L1187
	mov	w26, 0
	mov	w20, 0
	mov	w24, -1
	mov	w23, 1
	.p2align 2
.L1194:
	add	x19, x21, x26, uxtw 3
	ldr	w0, [x21, 244]
	ldr	x1, [x19, 2704]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	cbz	x0, .L1188
	ldrsb	w1, [x0]
	cbnz	w1, .L1203
	ldrsb	w0, [x0, 1]
	cmp	w0, wzr
	ccmp	w0, 3, 4, ne
	ldr	x0, [x19, 2704]
	bne	.L1190
.L1206:
	ldr	w1, [x0, 8]
	cmp	w1, 1
	beq	.L1190
	str	w24, [x0, 32]
	ldr	x0, [x19, 2704]
	strb	wzr, [x0, 1]
	ldr	x0, [x19, 2704]
	strb	w23, [x0, 2]
.L1203:
	ldr	x0, [x19, 2704]
.L1190:
	ldr	w0, [x0]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L1207
	add	w26, w26, 1
.L1193:
	add	w20, w20, 1
	cmp	w22, w20
	bne	.L1194
.L1187:
	mov	w0, w25
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1188:
	ldr	x0, [x19, 2704]
	b	.L1206
	.p2align 3
.L1207:
	mov	w1, w26
	mov	x0, x21
	mov	w25, 1
	bl	HEVC_RemoveFrameStoreOutDPB
	b	.L1193
	.size	HEVC_RemoveUnUsedFrameStore, .-HEVC_RemoveUnUsedFrameStore
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_SimpleSlideDPB
	.type	HEVC_SimpleSlideDPB, %function
HEVC_SimpleSlideDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 2680]
	ldr	w1, [x19, 2684]
	cmp	w1, w0
	bcc	.L1208
	cmp	w0, 15
	bls	.L1211
.L1212:
	ldr	x0, [x19, 2704]
	mov	w2, 0
	strb	wzr, [x0, 1]
	ldr	w1, [x0, 32]
	ldr	w0, [x19, 244]
	bl	FSP_SetRef
	mov	x0, x19
	mov	w1, 0
	bl	HEVC_RemoveApcStore
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
.L1208:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1211:
	mov	x0, x19
	bl	HEVC_IncreaseDPBSize
	cbnz	w0, .L1212
	b	.L1208
	.size	HEVC_SimpleSlideDPB, .-HEVC_SimpleSlideDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetMinPOC
	.type	HEVC_GetMinPOC, %function
HEVC_GetMinPOC:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x2
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	str	wzr, [x21]
	mov	w0, 2147483647
	str	w0, [x20]
	ldr	w3, [x19, 2680]
	ldr	w2, [x19, 2684]
	cmp	w2, w3
	bhi	.L1222
.L1215:
	mov	w0, 0
	cbz	w2, .L1214
	.p2align 2
.L1220:
	add	x1, x19, x0, uxtw 3
	ldr	w2, [x20]
	ldr	x1, [x1, 2704]
	ldr	w1, [x1, 12]
	cmp	w2, w1
	ble	.L1217
	str	w1, [x20]
	str	w0, [x21]
.L1217:
	ldr	w1, [x19, 2684]
	add	w0, w0, 1
	cmp	w1, w0
	bhi	.L1220
.L1214:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1222:
	ldr	x1, .LC245
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w2, [x19, 2680]
	str	w2, [x19, 2684]
	b	.L1215
	.size	HEVC_GetMinPOC, .-HEVC_GetMinPOC
	.align	3
.LC245:
	.xword	.LC244
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetReRangeFlag
	.type	HEVC_GetReRangeFlag, %function
HEVC_GetReRangeFlag:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w1
	mov	w20, w4
	mov	w22, w2
	bl	_mcount
	add	x4, x19, 53248
	ldr	x2, [x19, 2336]
	mov	w3, 2024
	ldr	x0, [x19, 2328]
	mov	w1, 26164
	ldr	w5, [x4, 1836]
	smaddl	x3, w5, w3, x2
	ldr	w3, [x3, 32]
	smaddl	x0, w3, w1, x0
	ldr	w1, [x0, 72]
	cmp	w1, w21
	ccmp	w20, wzr, 0, eq
	bne	.L1225
	ldr	w1, [x0, 76]
	mov	w0, 1
	cmp	w1, w22
	beq	.L1229
.L1224:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1225:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1229:
	ldr	w1, [x19, 168]
	cbz	w1, .L1224
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 888]
	cmp	w0, wzr
	cset	w0, ne
	b	.L1224
	.size	HEVC_GetReRangeFlag, .-HEVC_GetReRangeFlag
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CalcStreamBits
	.type	HEVC_CalcStreamBits, %function
HEVC_CalcStreamBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x5, [x19, 2360]
	ldr	x0, [x19, 2312]
	ldrb	w4, [x5, 1]
	ldr	w6, [x0, 32]
	cbz	w4, .L1241
	sub	w2, w4, #1
	mov	w7, 32
	mov	x3, 88
	add	x0, x5, 56
	mov	w1, 0
	umaddl	x3, w2, w7, x3
	add	x3, x5, x3
	.p2align 2
.L1232:
	ldr	w2, [x0], 32
	cmp	x0, x3
	add	w1, w1, w2
	bne	.L1232
.L1231:
	add	x4, x5, x4, lsl 5
	and	w0, w6, 7
	str	w0, [x4, 68]
	ldr	x3, [x19, 2360]
	ldr	x0, [x19, 2312]
	ldrb	w2, [x3, 1]
	ldr	w0, [x0, 32]
	add	x4, x3, x2, lsl 5
	ldr	w3, [x4, 60]
	add	w0, w3, w0, lsr 3
	sub	w0, w0, w1
	str	w0, [x4, 72]
	ldr	x3, [x19, 2360]
	ldr	x0, [x19, 2312]
	ldrb	w2, [x3, 1]
	ldr	w0, [x0, 32]
	add	x4, x3, x2, lsl 5
	ldr	w2, [x4, 56]
	add	w1, w1, w2
	lsl	w1, w1, 3
	sub	w0, w1, w0
	str	w0, [x4, 64]
	ldr	x1, [x19, 2360]
	ldrb	w0, [x1, 1]
	add	x0, x1, x0, lsl 5
	ldp	w2, w1, [x0, 68]
	ubfiz	w1, w1, 3, 2
	add	w1, w2, w1
	str	w1, [x0, 68]
	ldr	x1, [x19, 2360]
	ldrb	w0, [x1, 1]
	add	x0, x1, x0, lsl 5
	ldr	w1, [x0, 72]
	and	w1, w1, -4
	str	w1, [x0, 72]
	ldr	x3, [x19, 2360]
	ldr	w2, [x3, 8]
	sub	w0, w2, #1
	cmp	w0, 1
	bhi	.L1249
	ldrb	w4, [x3, 1]
	add	w1, w4, 1
	cmp	w1, 1
	ccmp	w2, w1, 0, ls
	bls	.L1235
	add	w4, w4, 2
	.p2align 2
.L1236:
	sub	w0, w4, #1
	cmp	w4, 1
	add	x1, x0, 2
	lsl	x0, x0, 5
	lsl	x1, x1, 5
	cset	w5, ls
	add	x3, x3, x1
	str	wzr, [x3, 4]
	ldr	x2, [x19, 2360]
	add	x3, x2, x0
	add	x2, x2, x1
	ldr	w3, [x3, 60]
	str	w3, [x2, 8]
	ldr	x3, [x19, 2360]
	add	x2, x3, x0
	ldr	w2, [x2, 56]
	lsl	w2, w2, 3
	str	w2, [x3, x1]
	ldr	x1, [x19, 2360]
	add	x1, x1, x0
	ldp	w3, w2, [x1, 68]
	ubfiz	w2, w2, 3, 4
	add	w2, w3, w2
	str	w2, [x1, 68]
	ldr	x1, [x19, 2360]
	add	x0, x1, x0
	ldr	w1, [x0, 72]
	and	w1, w1, -16
	str	w1, [x0, 72]
	ldr	x3, [x19, 2360]
	ldr	w0, [x3, 8]
	cmp	w0, w4
	add	w4, w4, 1
	cset	w1, hi
	tst	w5, w1
	bne	.L1236
	sub	w0, w0, #1
.L1235:
	ubfiz	x0, x0, 5, 32
	ldr	w1, [x3, 12]
	add	x3, x3, x0
	add	x5, x19, 53248
	ldr	w0, [x3, 64]
	sub	w0, w0, w1
	str	w0, [x3, 64]
	ldr	x3, [x19, 2360]
	ldrb	w1, [x3, 1]
	ldr	w4, [x3, 8]
	cmp	w1, w4
	bcs	.L1230
	.p2align 2
.L1244:
	uxtw	x0, w1
	ldr	w2, [x5, 1104]
	add	w1, w1, 1
	add	x0, x3, x0, lsl 5
	ldr	w0, [x0, 72]
	cmp	w0, w2
	bcs	.L1238
	str	w0, [x5, 1104]
	ldr	w4, [x3, 8]
.L1238:
	cmp	w4, w1
	bhi	.L1244
.L1230:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1249:
	ldr	x1, .LC247
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1241:
	mov	w1, 0
	b	.L1231
	.size	HEVC_CalcStreamBits, .-HEVC_CalcStreamBits
	.align	3
.LC247:
	.xword	.LC246
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_VpsSpsPpsCheck
	.type	HEVC_VpsSpsPpsCheck, %function
HEVC_VpsSpsPpsCheck:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 53248
	ldr	w0, [x19, 156]
	ldr	w2, [x1, 1836]
	cmp	w2, w0
	bge	.L1258
	mov	w0, 2024
	ldr	x1, [x19, 2336]
	smull	x0, w2, w0
	add	x3, x1, x0
	ldrb	w0, [x1, x0]
	cbz	w0, .L1259
	ldr	w2, [x3, 32]
	ldr	w0, [x19, 152]
	cmp	w2, w0
	bge	.L1260
	ldr	x1, [x19, 2328]
	mov	w0, 26164
	smaddl	x0, w2, w0, x1
	ldrb	w1, [x0, 1]
	cbz	w1, .L1261
	ldr	w2, [x0, 52]
	ldr	w0, [x19, 148]
	cmp	w2, w0
	bcs	.L1262
	mov	w1, 23196
	ldr	x3, [x19, 2320]
	mov	w0, 0
	umull	x1, w2, w1
	ldrb	w1, [x3, x1]
	cbz	w1, .L1263
.L1252:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1258:
	ldr	x1, .LC249
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1252
	.p2align 3
.L1259:
	ldr	x1, .LC251
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1252
	.p2align 3
.L1260:
	ldr	x1, .LC253
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1252
	.p2align 3
.L1261:
	ldr	x1, .LC255
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1252
	.p2align 3
.L1262:
	ldr	x1, .LC257
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1252
	.p2align 3
.L1263:
	ldr	x1, .LC259
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1252
	.size	HEVC_VpsSpsPpsCheck, .-HEVC_VpsSpsPpsCheck
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
	.align	3
.LC257:
	.xword	.LC256
	.align	3
.LC259:
	.xword	.LC258
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsNewPic
	.type	HEVC_IsNewPic, %function
HEVC_IsNewPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 2360]
	mov	w0, 2147483647
	ldr	w5, [x19, 200]
	cmp	w5, w0
	ldr	w0, [x1, 16]
	sub	w2, w0, #16
	beq	.L1291
	cmp	w2, 2
	ccmp	w0, 21, 4, hi
	beq	.L1269
.L1277:
	sub	w0, w0, #19
	cmp	w0, 1
	bls	.L1292
.L1269:
	add	x0, x19, 53248
	ldr	w4, [x0, 1808]
	cmp	w4, w5
	bge	.L1268
	ldr	w6, [x1, 16]
	sub	w2, w6, #8
	cmp	w2, 1
	bls	.L1293
.L1270:
	ldrb	w3, [x19, 8]
	cbz	w3, .L1271
	ldr	w6, [x19, 204]
	cmp	w6, w4
	bgt	.L1294
.L1271:
	ldrb	w2, [x0, 1763]
	cbnz	w2, .L1272
	ldr	w1, [x1, 16]
	sub	w2, w1, #19
	cmp	w2, 1
	bls	.L1289
	cmp	w1, 21
	beq	.L1289
	sub	w1, w1, #16
	cmp	w1, 2
	bhi	.L1272
	mov	w1, 1
	str	w4, [x19, 204]
	strb	w1, [x19, 8]
	.p2align 2
.L1272:
	ldr	w1, [x19, 216]
	ldrb	w3, [x0, 1761]
	cbnz	w1, .L1275
	ldr	w2, [x0, 1836]
	mov	w0, 1
	ldr	w1, [x19, 288]
	cmp	w1, w2
	beq	.L1295
.L1286:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1291:
	cmp	w2, 5
	bhi	.L1290
	cmp	w2, 2
	ccmp	w0, 21, 4, hi
	bne	.L1277
	add	x0, x19, 53248
	ldr	w4, [x0, 1808]
	str	w4, [x19, 200]
.L1268:
	ldrb	w5, [x19, 3]
	cbz	w5, .L1270
	ldr	w6, [x1, 16]
	sub	w2, w6, #8
	cmp	w2, 1
	bhi	.L1270
	ldr	x2, .LC260
	mov	w3, 10448
	ldr	x1, .LC264
	mov	w0, 1
	add	x2, x2, 424
	bl	dprint_vfmw
	mov	w0, 2
	b	.L1286
	.p2align 3
.L1275:
	sub	w1, w1, #1
	ldr	w2, [x0, 1836]
	add	x1, x1, 16384
	add	x1, x1, 716
	ldr	w0, [x19, x1, lsl 2]
	ldr	w1, [x19, 288]
	cmp	w2, w0
	mov	w0, 1
	csinc	w3, w3, wzr, eq
	cmp	w1, w2
	bne	.L1286
.L1295:
	ldr	w0, [x19, 252]
	ldr	x19, [sp, 16]
	cmp	w0, w4
	csinc	w0, w3, wzr, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1294:
	ldr	w7, [x1, 16]
	sub	w2, w7, #8
	cmp	w2, 1
	bhi	.L1271
	ldr	x2, .LC260
	mov	w5, w4
	ldr	x1, .LC266
	mov	w4, w3
	add	x2, x2, 424
	mov	w3, 10458
	mov	w0, 1
	bl	dprint_vfmw
.L1290:
	mov	w0, 2
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1289:
	strb	wzr, [x19, 8]
	str	w4, [x19, 204]
	b	.L1272
	.p2align 3
.L1292:
	mov	w5, -2147483647
	str	w5, [x19, 200]
	b	.L1269
	.p2align 3
.L1293:
	ldr	x2, .LC260
	mov	w3, 10440
	ldr	x1, .LC262
	mov	w0, 1
	add	x2, x2, 424
	bl	dprint_vfmw
	mov	w0, 2
	b	.L1286
	.size	HEVC_IsNewPic, .-HEVC_IsNewPic
	.align	3
.LC260:
	.xword	.LANCHOR1
	.align	3
.LC262:
	.xword	.LC261
	.align	3
.LC264:
	.xword	.LC263
	.align	3
.LC266:
	.xword	.LC265
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ReadByteAlignment
	.type	HEVC_ReadByteAlignment, %function
HEVC_ReadByteAlignment:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	cbz	w0, .L1301
	ldr	x0, [x19, 2312]
	mov	w20, 0
	bl	BsBitsToNextByte
	mov	w1, w0
	cbz	w0, .L1298
	ldr	x0, [x19, 2312]
	bl	BsGet
	mov	w2, w0
	ldr	x1, .LC270
	mov	w0, 19
	bl	dprint_vfmw
.L1298:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1301:
	ldr	x1, .LC268
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	b	.L1298
	.size	HEVC_ReadByteAlignment, .-HEVC_ReadByteAlignment
	.align	3
.LC268:
	.xword	.LC267
	.align	3
.LC270:
	.xword	.LC269
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetRapPicFlag
	.type	HEVC_GetRapPicFlag, %function
HEVC_GetRapPicFlag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 53248
	bl	_mcount
	ldr	w0, [x19, 1832]
	ldr	x19, [sp, 16]
	sub	w0, w0, #16
	cmp	w0, 5
	cset	w0, ls
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_GetRapPicFlag, .-HEVC_GetRapPicFlag
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetIdrPicFlag
	.type	HEVC_GetIdrPicFlag, %function
HEVC_GetIdrPicFlag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 53248
	bl	_mcount
	ldr	w0, [x19, 1832]
	ldr	x19, [sp, 16]
	sub	w0, w0, #19
	cmp	w0, 1
	cset	w0, ls
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_GetIdrPicFlag, .-HEVC_GetIdrPicFlag
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecPTL
	.type	HEVC_DecPTL, %function
HEVC_DecPTL:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	w23, w2
	mov	x19, x1
	mov	w24, w3
	bl	_mcount
	ldr	x0, [x20, 2312]
	cbnz	w23, .L1342
	mov	w1, 8
	bl	BsGet
	str	w0, [x19, 284]
	cmp	w24, wzr
	ble	.L1307
.L1320:
	sub	w25, w24, #1
	add	x22, x19, 43
	add	x25, x25, 44
	mov	x21, x22
	add	x25, x19, x25
	.p2align 2
.L1309:
	cbnz	w23, .L1343
.L1308:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21], 1
	cmp	x21, x25
	bne	.L1309
	cmp	w24, 7
	mov	w21, w24
	ble	.L1329
	b	.L1319
	.p2align 3
.L1312:
	cmp	w21, 8
	beq	.L1319
.L1329:
	ldr	x0, [x20, 2312]
	mov	w1, 2
	add	w21, w21, 1
	bl	BsGet
	cbz	w0, .L1312
	ldr	x1, .LC275
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L1343:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21, -6]
	b	.L1308
.L1342:
	mov	w1, 2
	mov	x21, 0
	bl	BsGet
	str	w0, [x19, 276]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19]
	mov	w1, 5
	ldr	x0, [x20, 2312]
	bl	BsGet
	ldr	x25, .LC271
	ldr	x22, .LC273
	str	w0, [x19, 272]
	.p2align 2
.L1306:
	ldr	x4, [x25, 136]
	mov	w3, w21
	mov	x2, x22
	mov	x1, 100
	add	x0, x29, 88
	blr	x4
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	add	x1, x19, x21
	add	x21, x21, 1
	cmp	x21, 32
	strb	w0, [x1, 1]
	bne	.L1306
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 33]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 34]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 35]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 36]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	ldr	x0, [x20, 2312]
	mov	w1, 16
	bl	BsGet
	ldr	x0, [x20, 2312]
	mov	w1, 12
	bl	BsGet
	add	x0, x20, 86016
	ldr	w2, [x19, 272]
	mov	w1, 8
	str	w2, [x0, 904]
	ldrb	w2, [x19, 2]
	str	w2, [x0, 908]
	ldrb	w2, [x19, 3]
	str	w2, [x0, 912]
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 284]
	cmp	w24, wzr
	bgt	.L1320
.L1307:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	mov	w0, 0
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L1319:
	add	x26, x19, 360
	mov	w25, 0
	add	x19, x19, 49
	b	.L1317
	.p2align 3
.L1316:
	add	w25, w25, 1
	add	x22, x22, 1
	cmp	w24, w25
	add	x26, x26, 4
	mov	x19, x21
	ble	.L1307
.L1317:
	add	x21, x19, 32
	cbz	w23, .L1314
	ldrb	w0, [x22, -6]
	cbnz	w0, .L1344
.L1314:
	ldrb	w0, [x22]
	cbz	w0, .L1316
	ldr	x0, [x20, 2312]
	mov	w1, 8
	bl	BsGet
	str	w0, [x26]
	b	.L1316
	.p2align 3
.L1344:
	ldr	x0, [x20, 2312]
	mov	w1, 2
	bl	BsGet
	str	w0, [x26, -72]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x22, 198]
	mov	w1, 5
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x26, -48]
	.p2align 2
.L1315:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19], 1
	cmp	x19, x21
	bne	.L1315
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x22, 204]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x22, 210]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x22, 216]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x22, 222]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	ldr	x0, [x20, 2312]
	mov	w1, 16
	bl	BsGet
	ldr	x0, [x20, 2312]
	mov	w1, 12
	bl	BsGet
	b	.L1314
	.size	HEVC_DecPTL, .-HEVC_DecPTL
	.align	3
.LC271:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC273:
	.xword	.LC272
	.align	3
.LC275:
	.xword	.LC274
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_SetDefaultScalingList
	.type	HEVC_SetDefaultScalingList, %function
HEVC_SetDefaultScalingList:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	add	x26, x20, 2196
	mov	w25, 0
	bl	_mcount
	ldr	x21, .LC277
	ldr	x22, .LC276
	mov	w23, 16
	ldr	x24, .LC278
.L1346:
	ldr	w0, [x26]
	cbz	w0, .L1355
	uxtw	x0, w25
	mov	w27, 0
	lsl	x28, x0, 2
	sub	x28, x28, x0
	lsl	x28, x28, 1
	.p2align 2
.L1352:
	uxtw	x19, w27
	add	x1, x21, 440
	add	x0, x19, x28
	cmp	w25, 3
	mov	x2, 64
	cbz	w25, .L1359
	add	x0, x20, x0, lsl 6
	beq	.L1360
	cmp	w27, 3
	ldr	x4, [x24, 104]
	add	x0, x0, 580
	csel	x1, x22, x1, cc
	blr	x4
.L1348:
	add	x19, x28, x19
	add	w27, w27, 1
	add	x19, x20, x19, lsl 2
	str	w23, [x19, 2212]
	ldr	w0, [x26]
	cmp	w0, w27
	bhi	.L1352
.L1355:
	add	w25, w25, 1
	add	x26, x26, 4
	cmp	w25, 4
	bne	.L1346
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1359:
	add	x0, x20, x19, lsl 6
	ldr	x1, .LC279
	ldr	x4, [x24, 104]
	mov	x2, 16
	add	x0, x0, 580
	blr	x4
	b	.L1348
.L1360:
	ldr	x0, .LC277
	uxtw	x19, w27
	cmp	w27, wzr
	ldr	x4, [x24, 104]
	add	x1, x0, 440
	ldr	x0, .LC276
	mov	x2, 64
	csel	x1, x0, x1, eq
	add	x0, x20, x19, lsl 6
	add	x0, x0, 1732
	blr	x4
	b	.L1348
	.size	HEVC_SetDefaultScalingList, .-HEVC_SetDefaultScalingList
	.align	3
.LC276:
	.xword	.LANCHOR4
	.align	3
.LC277:
	.xword	.LANCHOR1
	.align	3
.LC278:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC279:
	.xword	.LANCHOR1+504
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_SetScalingList
	.type	HEVC_SetScalingList, %function
HEVC_SetScalingList:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 28136]
	ldrb	w1, [x0, 5]
	cbz	w1, .L1391
	ldrb	w0, [x0, 6]
	cbnz	w0, .L1363
	ldr	x0, [x19, 28144]
	ldrb	w0, [x0, 21]
	cbz	w0, .L1400
.L1364:
	ldr	x20, .LC280
	mov	x2, 1536
	mov	w1, 0
	add	x0, x19, 580
	ldr	x3, [x20, 96]
	blr	x3
	add	x16, x19, 292
	mov	x15, 310
	mov	w12, 0
	mov	w17, 64
	mov	w14, 6
.L1366:
	ldr	w13, [x16, 1904]
	cbz	w13, .L1375
	ldr	w8, [x16]
	mov	w11, 0
	ldr	x9, [x19, 28144]
	cmp	w8, 64
	csel	w8, w8, w17, ls
	add	x9, x9, x15
	lsr	w10, w8, 2
.L1373:
	uxtw	x7, w11
	mov	w4, 0
	umaddl	x7, w12, w14, x7
	lsl	x7, x7, 4
	.p2align 2
.L1367:
	cmp	w4, w10
	bcs	.L1370
	cbz	w8, .L1367
	add	x0, x7, x4, uxtw
	ldrb	w1, [x9, -1]
	mov	x6, x9
	mov	w3, 0
	add	x0, x19, x0, lsl 2
	ldr	w2, [x0, 580]
	orr	w1, w2, w1
	str	w1, [x0, 580]
	.p2align 2
.L1386:
	add	w3, w3, 1
	add	x0, x7, x4, uxtw
	cmp	w3, w8
	and	w5, w3, 3
	add	x0, x19, x0, lsl 2
	lsl	w1, w5, 3
	beq	.L1367
	ldrb	w2, [x6], 1
	cmp	w5, 3
	ldr	w5, [x0, 580]
	csinc	w4, w4, w4, ne
	lsl	w1, w2, w1
	orr	w1, w5, w1
	str	w1, [x0, 580]
	b	.L1386
.L1370:
	add	w11, w11, 1
	add	x9, x9, 64
	cmp	w11, w13
	bne	.L1373
.L1375:
	add	x15, x15, 384
	add	w12, w12, 1
	cmp	x15, 1846
	add	x16, x16, 4
	bne	.L1366
	ldr	x1, [x19, 28144]
	mov	x2, 96
	ldr	x3, [x20, 104]
	add	x0, x19, 2212
	add	x1, x1, 1928
	blr	x3
.L1391:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1363:
	ldr	x0, [x19, 28144]
	ldrb	w0, [x0, 21]
	cbnz	w0, .L1364
	ldr	x20, .LC280
	mov	w1, 0
	mov	x2, 1536
	add	x0, x19, 580
	ldr	x3, [x20, 96]
	blr	x3
	ldr	x1, [x19, 28136]
	add	x16, x19, 292
	mov	w13, 0
	mov	w18, 64
	mov	w15, 6
	add	x17, x1, 910
.L1376:
	ldr	w14, [x16, 1904]
	cbz	w14, .L1385
	ldr	w9, [x16]
	mov	x10, x17
	mov	w12, 0
	cmp	w9, 64
	csel	w9, w9, w18, ls
	lsr	w11, w9, 2
.L1383:
	uxtw	x8, w12
	mov	w5, 0
	umaddl	x8, w13, w15, x8
	lsl	x8, x8, 4
	.p2align 2
.L1377:
	cmp	w5, w11
	bcs	.L1380
	cbz	w9, .L1377
	add	x0, x8, x5, uxtw
	ldrb	w2, [x10, -1]
	mov	x7, x10
	mov	w4, 0
	add	x0, x19, x0, lsl 2
	ldr	w3, [x0, 580]
	orr	w2, w3, w2
	str	w2, [x0, 580]
	.p2align 2
.L1387:
	add	w4, w4, 1
	add	x0, x8, x5, uxtw
	cmp	w4, w9
	and	w6, w4, 3
	add	x0, x19, x0, lsl 2
	lsl	w2, w6, 3
	beq	.L1377
	ldrb	w3, [x7], 1
	cmp	w6, 3
	ldr	w6, [x0, 580]
	csinc	w5, w5, w5, ne
	lsl	w2, w3, w2
	orr	w2, w6, w2
	str	w2, [x0, 580]
	b	.L1387
.L1380:
	add	w12, w12, 1
	add	x10, x10, 64
	cmp	w12, w14
	bne	.L1383
.L1385:
	add	w13, w13, 1
	add	x17, x17, 384
	cmp	w13, 4
	add	x16, x16, 4
	bne	.L1376
	ldr	x3, [x20, 104]
	mov	x2, 96
	add	x1, x1, 2528
	add	x0, x19, 2212
	blr	x3
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1400:
	mov	x0, x19
	bl	HEVC_SetDefaultScalingList
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_SetScalingList, .-HEVC_SetScalingList
	.align	3
.LC280:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CalScalingList
	.type	HEVC_CalScalingList, %function
HEVC_CalScalingList:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w2
	uxtw	x22, w3
	mov	x21, x1
	bl	_mcount
	add	x0, x20, x19, uxtw 2
	mov	w9, 6
	mov	w8, 64
	mov	w5, 0
	umaddl	x9, w19, w9, x22
	ldr	w0, [x0, 292]
	lsl	x9, x9, 4
	cmp	w0, w8
	csel	w8, w0, w8, ls
	lsr	w10, w8, 2
	.p2align 2
.L1402:
	cmp	w5, w10
	bcs	.L1401
	cbz	w8, .L1402
	add	x0, x9, x5, uxtw
	mov	x7, x21
	mov	w4, 0
	add	x0, x20, x0, lsl 2
	ldrb	w2, [x7], 1
	ldr	w3, [x0, 580]
	orr	w2, w3, w2
	str	w2, [x0, 580]
	.p2align 2
.L1408:
	add	w4, w4, 1
	add	x0, x9, x5, uxtw
	cmp	w8, w4
	and	w6, w4, 3
	add	x0, x20, x0, lsl 2
	lsl	w2, w6, 3
	beq	.L1402
	ldrb	w3, [x7], 1
	cmp	w6, 3
	ldr	w6, [x0, 580]
	csinc	w5, w5, w5, ne
	lsl	w2, w3, w2
	orr	w2, w6, w2
	str	w2, [x0, 580]
	b	.L1408
.L1401:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_CalScalingList, .-HEVC_CalScalingList
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetScalingListDefaultAddress
	.type	HEVC_GetScalingListDefaultAddress, %function
HEVC_GetScalingListDefaultAddress:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w19, 3
	bls	.L1422
	ldr	x1, .LC284
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1422:
	ldr	x0, .LC282
	ldrb	w0, [x0,w19,uxtw]
	adr	x1, .Lrtx1412
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1412:
	.section	.rodata
	.align	0
	.align	2
.L1412:
	.byte	(.L1416 - .Lrtx1412) / 4
	.byte	(.L1414 - .Lrtx1412) / 4
	.byte	(.L1414 - .Lrtx1412) / 4
	.byte	(.L1415 - .Lrtx1412) / 4
	.text
	.p2align 3
.L1414:
	cmp	w20, 2
	ldr	x0, .LC281
	bhi	.L1419
.L1420:
	add	x0, x0, 144
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1415:
	ldr	x0, .LC281
	cbz	w20, .L1420
.L1419:
	add	x0, x0, 80
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1416:
	ldr	x0, .LC281
	ldp	x19, x20, [sp, 16]
	add	x0, x0, 64
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_GetScalingListDefaultAddress, .-HEVC_GetScalingListDefaultAddress
	.align	3
.LC281:
	.xword	.LANCHOR4
	.align	3
.LC282:
	.xword	.L1412
	.align	3
.LC284:
	.xword	.LC283
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecRefPicListsModification
	.type	HEVC_DecRefPicListsModification, %function
HEVC_DecRefPicListsModification:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	x21, x1
	mov	x23, x2
	mov	w20, w3
	bl	_mcount
	cmp	x21, xzr
	ccmp	x23, xzr, 4, ne
	ccmp	x22, xzr, 4, ne
	beq	.L1433
	sub	w0, w20, #1
	mov	w19, 1
	asr	w0, w0, 1
	cbz	w0, .L1425
	.p2align 2
.L1426:
	asr	w0, w0, 1
	add	w19, w19, 1
	cbnz	w0, .L1426
.L1425:
	ldr	x0, [x22, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	cmp	w20, 1
	strb	w0, [x23]
	cset	w24, hi
	cmp	w0, wzr
	cset	w0, ne
	tst	w24, w0
	beq	.L1430
	ldr	w0, [x21, 168]
	cbz	w0, .L1430
	mov	w20, 0
	.p2align 2
.L1431:
	ldr	x0, [x22, 2312]
	mov	w1, w19
	bl	BsGet
	add	x1, x23, x20, uxtw 2
	add	w20, w20, 1
	str	w0, [x1, 4]
	ldr	w0, [x21, 168]
	cmp	w0, w20
	bhi	.L1431
.L1430:
	ldr	w0, [x21, 60]
	cbz	w0, .L1448
.L1429:
	mov	w0, 0
.L1424:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1448:
	ldr	x0, [x22, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, wzr
	strb	w0, [x23, 1]
	cset	w0, ne
	tst	w24, w0
	beq	.L1429
	ldr	w0, [x21, 172]
	cbz	w0, .L1429
	mov	w20, 0
	.p2align 2
.L1432:
	ldr	x0, [x22, 2312]
	mov	w1, w19
	bl	BsGet
	add	x1, x23, x20, uxtw 2
	add	w20, w20, 1
	str	w0, [x1, 132]
	ldr	w0, [x21, 172]
	cmp	w0, w20
	bhi	.L1432
	b	.L1429
.L1433:
	mov	w0, -1
	b	.L1424
	.size	HEVC_DecRefPicListsModification, .-HEVC_DecRefPicListsModification
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_MoreRbspData
	.type	HEVC_MoreRbspData, %function
HEVC_MoreRbspData:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 2312]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L1451
	ldr	x1, [x19, 2360]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w1, [x1, 12]
	add	w1, w1, 3
	cmp	w1, w0
	cset	w0, cc
	ret
	.p2align 3
.L1451:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_MoreRbspData, .-HEVC_MoreRbspData
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_More_Rbsp_Data
	.type	HEVC_More_Rbsp_Data, %function
HEVC_More_Rbsp_Data:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 2312]
	ldr	w2, [x0, 24]
	ldr	w3, [x0, 32]
	lsl	w2, w2, 3
	cmp	w2, w3
	bls	.L1455
	mov	w1, 8
	bl	BsShow
	cmp	w0, 128
	cset	w0, ne
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1455:
	ldr	x1, .LC286
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_More_Rbsp_Data, .-HEVC_More_Rbsp_Data
	.align	3
.LC286:
	.xword	.LC285
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_FindTrailZeros
	.type	HEVC_FindTrailZeros, %function
HEVC_FindTrailZeros:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
.L1469:
	ldr	x1, [x20, 2360]
	mov	w3, 0
	ldr	w5, [x1, 8]
	sub	w5, w5, #1
	add	x2, x1, x5, lsl 5
	ldr	w0, [x2, 56]
	ldr	x2, [x2, 48]
	sub	x0, x0, #1
	add	x0, x2, x0
	.p2align 2
.L1457:
	ldrb	w2, [x0]
	lsl	x19, x5, 5
	add	x4, x1, x19
	cbnz	w2, .L1470
	ldr	x2, [x1, 48]
	add	w3, w3, 1
	cmp	x0, x2
	beq	.L1471
	ldr	x2, [x4, 48]
	cmp	x0, x2
	sub	x0, x0, #1
	bne	.L1457
	add	x1, x1, x5, lsl 5
	ldr	w0, [x20, 244]
	ldr	w1, [x1, 76]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x20, 2360]
	add	x19, x0, x19
	str	xzr, [x19, 48]
	ldr	x1, [x20, 2360]
	ldr	w0, [x1, 8]
	sub	w0, w0, #1
	str	w0, [x1, 8]
	b	.L1469
	.p2align 3
.L1470:
	mov	w0, 1
	mov	w5, w0
	tbnz	x2, 0, .L1464
	.p2align 2
.L1467:
	lsl	w4, w5, w0
	add	w0, w0, 1
	tst	w4, w2
	bne	.L1464
	cmp	w0, 8
	bne	.L1467
	ldr	x1, .LC290
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1471:
	ldr	x1, .LC288
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1464:
	str	wzr, [x1, 4]
	add	w3, w0, w3, lsl 3
	mov	w0, 0
	ldr	x1, [x20, 2360]
	str	w3, [x1, 12]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_FindTrailZeros, .-HEVC_FindTrailZeros
	.align	3
.LC288:
	.xword	.LC287
	.align	3
.LC290:
	.xword	.LC289
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_FindNaluArraySlot
	.type	HEVC_FindNaluArraySlot, %function
HEVC_FindNaluArraySlot:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w3, [x19, 160]
	ldr	w0, [x19, 140]
	add	w3, w3, 1
	cmp	w3, wzr
	ble	.L1473
	udiv	w1, w0, w3
	add	x2, x19, 65536
	ldr	x5, [x2, 2856]
	mov	w2, 0
	msub	w4, w1, w3, w0
	lsl	x1, x4, 3
	mov	x0, x4
	sub	x1, x1, x4
	add	x1, x5, x1, lsl 4
	ldr	w1, [x1, 36]
	cbnz	w1, .L1476
	b	.L1474
	.p2align 3
.L1477:
	udiv	w0, w1, w3
	msub	w4, w0, w3, w1
	lsl	x1, x4, 3
	mov	x0, x4
	sub	x1, x1, x4
	add	x1, x5, x1, lsl 4
	ldr	w1, [x1, 36]
	cbz	w1, .L1474
.L1476:
	add	w2, w2, 1
	cmp	w3, w2
	add	w1, w2, w0
	bne	.L1477
.L1473:
	str	w0, [x19, 140]
.L1488:
	mov	w0, -1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1474:
	cmp	w0, w3
	str	w0, [x19, 140]
	bcs	.L1488
	lsl	x1, x4, 3
	mov	w2, 1
	sub	x4, x1, x4
	add	x5, x5, x4, lsl 4
	str	w2, [x5, 36]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_FindNaluArraySlot, .-HEVC_FindNaluArraySlot
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ReleaseNAL
	.type	HEVC_ReleaseNAL, %function
HEVC_ReleaseNAL:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w21, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L1498
	ldr	x24, .LC293
	add	x19, x20, 48
	ldr	x23, .LC295
	add	x22, x20, 112
	stp	wzr, wzr, [x20, 36]
.L1493:
	ldr	x1, [x19]
	mov	w0, w21
	cbz	x1, .L1492
	ldr	w1, [x19, 28]
	bl	SM_ReleaseStreamSeg
	ldr	w4, [x19, 8]
	mov	x2, x24
	ldr	x3, [x19]
	mov	x1, x23
	mov	w0, 7
	bl	dprint_vfmw
	str	xzr, [x19]
	str	wzr, [x19, 8]
	str	wzr, [x19, 20]
.L1492:
	add	x19, x19, 32
	cmp	x19, x22
	bne	.L1493
	ldr	x3, .LC296
	mov	x2, 112
	mov	w1, 0
	mov	x0, x20
	ldr	x3, [x3, 96]
	blr	x3
.L1489:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1498:
	ldr	x2, .LC291
	mov	w3, 11740
	ldr	x1, .LC292
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1489
	.size	HEVC_ReleaseNAL, .-HEVC_ReleaseNAL
	.align	3
.LC291:
	.xword	.LC99
	.align	3
.LC292:
	.xword	.LC101
	.align	3
.LC293:
	.xword	.LANCHOR5
	.align	3
.LC295:
	.xword	.LC294
	.align	3
.LC296:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Hevc_InitDecBuffers
	.type	Hevc_InitDecBuffers, %function
Hevc_InitDecBuffers:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x21, 28136]
	ldr	w0, [x0, 236]
	add	w0, w0, 1
	cmp	w0, 17
	bls	.L1531
	mov	w0, 17
.L1531:
	add	x2, x21, 2560
	add	x1, x21, 2976
	stp	w0, wzr, [x2, 120]
	stp	wzr, wzr, [x2, 128]
	str	wzr, [x21, 2696]
	add	x0, x21, 2840
	str	wzr, [x21, 2700]
	.p2align 2
.L1502:
	str	xzr, [x0, -136]
	add	x0, x0, 8
	str	xzr, [x0, -8]
	str	xzr, [x0, 128]
	str	xzr, [x0, 264]
	cmp	x0, x1
	bne	.L1502
	add	x22, x21, 65536
	ldr	x23, .LC297
	add	x19, x22, 3672
	mov	w20, 0
	mov	w26, 20
	mov	w25, 16
	.p2align 2
.L1503:
	ldr	x3, [x23, 96]
	add	x0, x19, 80
	mov	x2, 624
	mov	w1, 0
	mov	w24, 16
	blr	x3
	str	w20, [x19, 236]
	add	w20, w20, 1
	strb	wzr, [x19]
	strb	wzr, [x19, 1]
	cmp	w20, 20
	strb	wzr, [x19, 2]
	str	wzr, [x19, 4]
	str	wzr, [x19, 12]
	stp	w26, w25, [x19, 24]
	str	wzr, [x19, 52]
	str	x19, [x19, 72]
	add	x19, x19, 712
	bne	.L1503
	ldr	x3, [x23, 96]
	mov	w1, 0
	mov	x2, 200
	add	x0, x21, 3248
	blr	x3
	str	w24, [x21, 3248]
	add	x1, x21, 2368
	add	x0, x21, 2504
	str	wzr, [x21, 3252]
	.p2align 2
.L1504:
	str	xzr, [x1], 8
	cmp	x0, x1
	bne	.L1504
	add	x2, x21, 53248
	add	x1, x21, 2640
	str	wzr, [x2, 1920]
	.p2align 2
.L1505:
	str	xzr, [x0], 8
	cmp	x0, x1
	bne	.L1505
	str	wzr, [x2, 1924]
	ldr	w0, [x21, 180]
	cbz	w0, .L1510
	sub	w1, w0, #1
	add	x0, x21, 2116
	add	x1, x1, 530
	add	x1, x21, x1, lsl 2
	.p2align 2
.L1509:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L1509
.L1510:
	ldr	w3, [x21, 160]
	mov	w20, 0
	cbz	w3, .L1499
	.p2align 2
.L1523:
	uxtw	x0, w20
	ldr	x1, [x22, 3664]
	lsl	x19, x0, 5
	add	w20, w20, 1
	add	x19, x19, x0, lsl 3
	add	x0, x1, x19
	ldr	x0, [x0, 32]
	mov	x1, x0
	cbz	x0, .L1511
	ldr	x2, [x21, 2360]
	cmp	x0, x2
	beq	.L1511
	ldr	w0, [x21, 244]
	bl	HEVC_ReleaseNAL
	ldr	x0, [x22, 3664]
	mov	w2, 2
	add	x0, x0, x19
	str	xzr, [x0, 32]
	ldr	w1, [x21, 208]
	ldr	w0, [x21, 216]
	ldr	w4, [x21, 172]
	cmp	w1, wzr
	cset	w7, ne
	cmp	w0, wzr
	cset	w5, ne
	sub	w6, w4, #1
	cmp	w4, 3
	ldr	w3, [x21, 160]
	sub	w1, w1, w7
	csel	w2, w6, w2, cs
	sub	w0, w0, w5
	str	w2, [x21, 172]
	str	w1, [x21, 208]
	str	w0, [x21, 216]
.L1511:
	cmp	w3, w20
	bhi	.L1523
.L1499:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	Hevc_InitDecBuffers, .-Hevc_InitDecBuffers
	.align	3
.LC297:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_EmptyFrmFromDPB
	.type	HEVC_EmptyFrmFromDPB, %function
HEVC_EmptyFrmFromDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L1543
	ldr	w3, [x20, 2684]
	mov	w0, 0
	mov	w4, 1
	cbz	w3, .L1538
	.p2align 2
.L1552:
	add	x1, x20, x0, uxtw 3
	add	w0, w0, 1
	ldr	x1, [x1, 2704]
	ldrb	w2, [x1, 2]
	cbnz	w2, .L1536
	strb	w4, [x1, 2]
	ldr	w3, [x20, 2684]
.L1536:
	cmp	w3, w0
	bhi	.L1552
.L1538:
	mov	w0, -2147483647
	add	x19, x20, 2704
	add	x21, x20, 2840
	str	w0, [x20, 212]
	.p2align 2
.L1535:
	ldr	x0, [x19]
	mov	w2, 1
	cbz	x0, .L1539
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L1539:
	ldr	x0, [x19, 136]
	mov	w2, 1
	cbz	x0, .L1540
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L1540:
	ldr	x0, [x19, 272]
	mov	w2, 1
	cbz	x0, .L1541
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L1541:
	ldr	x0, [x19, 408]
	mov	w2, 1
	cbz	x0, .L1542
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L1542:
	str	xzr, [x19, 136]
	str	xzr, [x19, 272]
	str	xzr, [x19, 408]
	str	xzr, [x19], 8
	cmp	x19, x21
	bne	.L1535
	mov	x0, x20
	bl	Hevc_InitDecBuffers
	mov	w0, 0
.L1533:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1543:
	mov	w0, -1
	b	.L1533
	.size	HEVC_EmptyFrmFromDPB, .-HEVC_EmptyFrmFromDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CombinePacket
	.type	HEVC_CombinePacket, %function
HEVC_CombinePacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 2360]
	cbz	x0, .L1562
	ldr	w0, [x0, 56]
	cmp	w0, 4095
	bhi	.L1559
	ldr	x1, .LC301
	mov	w0, 7
	bl	dprint_vfmw
	ldr	x1, [x19, 2360]
	ldr	x3, .LC302
	ldr	w2, [x1, 56]
	ldr	x0, [x1, 80]
	ldr	x3, [x3, 104]
	ldr	x1, [x1, 48]
	sub	x0, x0, x2
	blr	x3
	ldr	x0, [x19, 2360]
	mov	w3, 1
	ldr	w2, [x0, 108]
	ldr	w1, [x0, 76]
	str	w2, [x0, 76]
	ldr	x0, [x19, 2360]
	str	wzr, [x0, 68]
	ldr	x0, [x19, 2360]
	ldr	w4, [x0, 56]
	ldr	x2, [x0, 80]
	sub	x2, x2, x4
	str	x2, [x0, 48]
	ldr	x0, [x19, 2360]
	ldr	w4, [x0, 56]
	ldr	w2, [x0, 92]
	sub	w2, w2, w4
	str	w2, [x0, 60]
	ldr	x0, [x19, 2360]
	ldr	w4, [x0, 56]
	ldr	w2, [x0, 88]
	add	w2, w4, w2
	str	w2, [x0, 56]
	ldr	x0, [x19, 2360]
	str	w3, [x0, 8]
	ldr	w0, [x19, 244]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 2360]
	str	xzr, [x0, 80]
	ldr	x0, [x19, 2360]
	str	wzr, [x0, 100]
	ldr	x0, [x19, 2360]
	str	wzr, [x0, 88]
.L1559:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1562:
	ldr	x1, .LC299
	bl	dprint_vfmw
	b	.L1559
	.size	HEVC_CombinePacket, .-HEVC_CombinePacket
	.align	3
.LC299:
	.xword	.LC298
	.align	3
.LC301:
	.xword	.LC300
	.align	3
.LC302:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsOutDPB
	.type	HEVC_IsOutDPB, %function
HEVC_IsOutDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L1568
	ldr	w3, [x19, 2684]
	cbz	w3, .L1568
	ldr	x0, [x19, 2704]
	cmp	x20, x0
	beq	.L1570
	sub	w3, w3, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L1565
	.p2align 3
.L1566:
	ldr	x2, [x0, 2704]
	cmp	x20, x2
	beq	.L1570
.L1565:
	cmp	x0, x3
	add	x0, x0, 8
	bne	.L1566
.L1568:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1570:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsOutDPB, .-HEVC_IsOutDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IsNewPicNal
	.type	HEVC_IsNewPicNal, %function
HEVC_IsNewPicNal:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	HEVC_IsNewPicNal, .-HEVC_IsNewPicNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_u_v
	.type	HEVC_u_v, %function
HEVC_u_v:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	mov	w1, w20
	mov	x0, x19
	bl	BsGet
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_u_v, .-HEVC_u_v
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_f_v
	.type	HEVC_f_v, %function
HEVC_f_v:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	mov	w1, w20
	mov	x0, x19
	bl	BsGet
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_f_v, .-HEVC_f_v
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ue_v
	.type	HEVC_ue_v, %function
HEVC_ue_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 32
	mov	x0, x19
	bl	BsShow
	bl	ZerosMS_32
	cmp	w0, 31
	bls	.L1577
	ldr	x1, .LC304
	mov	w2, w0
	mov	w3, -4370
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -4370
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1577:
	mov	w1, w0
	mov	x0, x19
	add	w21, w1, 1
	bl	BsSkip
	mov	w1, w21
	mov	x0, x19
	bl	BsShow
	sub	w20, w0, #1
	mov	w1, w21
	mov	x0, x19
	bl	BsSkip
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_ue_v, .-HEVC_ue_v
	.align	3
.LC304:
	.xword	.LC303
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Tone_Mapping_Info
	.type	HEVC_Sei_Tone_Mapping_Info, %function
HEVC_Sei_Tone_Mapping_Info:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x1, .LC306
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 4]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19]
	cbz	w0, .L1579
	ldr	w2, [x19, 16]
.L1580:
	cmp	w2, 4
	mov	w1, 0
	bls	.L1601
.L1582:
	mov	w0, w1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1601:
	ldr	x0, .LC309
	ldrb	w0, [x0,w2,uxtw]
	adr	x1, .Lrtx1584
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1584:
	.section	.rodata
	.align	0
	.align	2
.L1584:
	.byte	(.L1583 - .Lrtx1584) / 4
	.byte	(.L1585 - .Lrtx1584) / 4
	.byte	(.L1586 - .Lrtx1584) / 4
	.byte	(.L1587 - .Lrtx1584) / 4
	.byte	(.L1588 - .Lrtx1584) / 4
	.text
	.p2align 3
.L1579:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 1]
	mov	w1, 8
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 8]
	mov	w1, 8
	ldr	x0, [x20, 2312]
	bl	BsGet
	cmp	w0, 8
	str	w0, [x19, 12]
	bhi	.L1602
	ldr	x0, [x20, 2312]
	mov	w1, 8
	bl	BsGet
	mov	w2, w0
	str	w0, [x19, 16]
	b	.L1580
	.p2align 3
.L1587:
	ldr	x0, [x20, 2312]
	mov	w1, 16
	bl	BsGet
	cmp	w0, 256
	str	w0, [x19, 80]
	bhi	.L1592
	mov	w21, 0
	cbz	w0, .L1595
	.p2align 2
.L1598:
	ldr	w1, [x19, 8]
	add	x22, x19, x21, uxtw 2
	ldr	x0, [x20, 2312]
	add	w21, w21, 1
	add	w1, w1, 7
	and	w1, w1, -8
	bl	BsGet
	str	w0, [x22, 84]
	ldr	w1, [x19, 12]
	ldr	x0, [x20, 2312]
	add	w1, w1, 7
	and	w1, w1, -8
	bl	BsGet
	str	w0, [x22, 1108]
	ldr	w0, [x19, 80]
	cmp	w0, w21
	bhi	.L1598
.L1595:
	mov	w1, 0
	ldr	x23, [sp, 48]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1586:
	ldr	w23, [x19, 12]
	mov	w0, 1
	lsl	w23, w0, w23
	cbz	w23, .L1591
	sub	w22, w23, #1
	add	x21, x19, 2132
	add	x22, x22, 534
	add	x22, x19, x22, lsl 2
	.p2align 2
.L1590:
	ldr	w1, [x19, 8]
	ldr	x0, [x20, 2312]
	add	w1, w1, 7
	and	w1, w1, -8
	bl	BsGet
	str	w0, [x21], 4
	cmp	x22, x21
	bne	.L1590
.L1591:
	add	x23, x19, x23, uxtw 2
	ldr	w2, [x19, 8]
	mov	w0, 1
	mov	w1, 0
	lsl	w0, w0, w2
	str	w0, [x23, 2132]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1585:
	ldr	x0, [x20, 2312]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 28]
	mov	w1, 32
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 32]
	mov	w1, 0
	mov	w0, w1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1583:
	ldr	x0, [x20, 2312]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 20]
	mov	w1, 32
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 24]
	mov	w1, 0
	mov	w0, w1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1588:
	ldr	x0, [x20, 2312]
	mov	w1, 8
	bl	BsGet
	cmp	w0, 255
	str	w0, [x19, 36]
	beq	.L1603
.L1596:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 2]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 52]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 56]
	mov	w1, 32
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 60]
	mov	w1, 32
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 64]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 68]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 72]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 52]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 76]
	mov	w1, 0
	mov	w0, w1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1603:
	ldr	x0, [x20, 2312]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 40]
	b	.L1596
.L1602:
	ldr	x1, .LC308
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1582
.L1592:
	ldr	x1, .LC311
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1582
	.size	HEVC_Sei_Tone_Mapping_Info, .-HEVC_Sei_Tone_Mapping_Info
	.align	3
.LC306:
	.xword	.LC305
	.align	3
.LC308:
	.xword	.LC307
	.align	3
.LC309:
	.xword	.L1584
	.align	3
.LC311:
	.xword	.LC310
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Frame_Packing_Arrangement
	.type	HEVC_Sei_Frame_Packing_Arrangement, %function
HEVC_Sei_Frame_Packing_Arrangement:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x1, .LC313
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	str	w0, [x20, 12]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20]
	cbnz	w0, .L1605
	ldr	x0, [x19, 2312]
	mov	w1, 7
	bl	BsGet
	sub	w1, w0, #3
	cmp	w1, 2
	str	w0, [x20, 16]
	bhi	.L1612
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20, 1]
	mov	w1, 6
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 20]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 2]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 3]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 5]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 6]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 7]
	ldrb	w1, [x20, 1]
	cbnz	w1, .L1608
	ldr	w0, [x20, 16]
	cmp	w0, 5
	beq	.L1608
	ldr	x0, [x19, 2312]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 24]
	mov	w1, 4
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 28]
	mov	w1, 4
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 32]
	mov	w1, 4
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 36]
.L1608:
	ldr	x0, [x19, 2312]
	mov	w1, 8
	bl	BsGet
	str	w0, [x20, 40]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 8]
.L1605:
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20, 9]
	ldr	x0, [x19, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L1607:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1612:
	ldr	x1, .LC315
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1607
	.size	HEVC_Sei_Frame_Packing_Arrangement, .-HEVC_Sei_Frame_Packing_Arrangement
	.align	3
.LC313:
	.xword	.LC312
	.align	3
.LC315:
	.xword	.LC314
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Dec_Unit_Info
	.type	HEVC_Sei_Dec_Unit_Info, %function
HEVC_Sei_Dec_Unit_Info:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	x21, x2
	bl	_mcount
	cmp	x20, xzr
	ccmp	x21, xzr, 4, ne
	ccmp	x19, xzr, 4, ne
	beq	.L1618
	ldr	x1, .LC317
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	str	w0, [x20, 4]
	ldrb	w0, [x21, 3]
	cbnz	w0, .L1622
	str	wzr, [x20, 8]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20]
	cbnz	w0, .L1623
.L1617:
	ldr	x0, [x19, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L1614:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1622:
	ldr	w1, [x21, 8]
	ldr	x0, [x19, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x20, 8]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20]
	cbz	w0, .L1617
.L1623:
	ldr	w1, [x21, 12]
	ldr	x0, [x19, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x20, 12]
	b	.L1617
	.p2align 3
.L1618:
	mov	w0, -1
	b	.L1614
	.size	HEVC_Sei_Dec_Unit_Info, .-HEVC_Sei_Dec_Unit_Info
	.align	3
.LC317:
	.xword	.LC316
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Active_Param_sets
	.type	HEVC_Sei_Active_Param_sets, %function
HEVC_Sei_Active_Param_sets:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, [x21, 2312]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x20, 1]
	ldr	x1, .LC319
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	cmp	w0, 15
	str	w0, [x20, 8]
	bgt	.L1625
	tbnz	w0, #31, .L1629
	ldr	x22, .LC323
	mov	w19, 0
	.p2align 2
.L1628:
	ldr	x0, [x21, 2312]
	mov	x1, x22
	bl	HEVC_ue_v
	add	x1, x20, x19, sxtw 2
	add	w19, w19, 1
	str	w0, [x1, 12]
	ldr	w0, [x20, 8]
	cmp	w0, w19
	bge	.L1628
.L1629:
	ldr	x0, [x21, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L1627:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1625:
	ldr	x1, .LC321
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1627
	.size	HEVC_Sei_Active_Param_sets, .-HEVC_Sei_Active_Param_sets
	.align	3
.LC319:
	.xword	.LC318
	.align	3
.LC321:
	.xword	.LC320
	.align	3
.LC323:
	.xword	.LC322
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Buf_Period
	.type	HEVC_Sei_Buf_Period, %function
HEVC_Sei_Buf_Period:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 24576
	mov	x22, x1
	mov	x23, x2
	bl	_mcount
	ldr	x1, .LC325
	ldr	w0, [x19, 3472]
	ldr	w2, [x19, 3480]
	add	w0, w0, 1
	str	wzr, [x19, 3480]
	str	w0, [x19, 3472]
	str	w2, [x19, 3492]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 15
	str	w0, [x22, 4]
	bhi	.L1659
	mov	w0, 1
	strb	w0, [x19, 3545]
	ldrb	w0, [x23, 2]
	cbz	w0, .L1634
	ldrb	w0, [x22]
	cbnz	w0, .L1660
.L1636:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x22, 1]
	ldr	w1, [x23, 32]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x22, 16]
	ldr	w2, [x23, 48]
	cmp	w2, 31
	bhi	.L1661
.L1637:
	mov	w21, 1
	mov	w19, 0
.L1638:
	cbz	w19, .L1662
	cmp	w19, 1
	beq	.L1663
.L1643:
	cmp	w21, 1
	bhi	.L1642
.L1641:
	add	w21, w21, 1
	add	w19, w19, 1
	b	.L1638
	.p2align 3
.L1642:
	ldr	x0, [x20, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L1658:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1634:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x22]
	cbz	w0, .L1636
.L1660:
	ldr	w1, [x23, 32]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x22, 8]
	ldr	w1, [x23, 36]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x22, 12]
	b	.L1636
	.p2align 3
.L1661:
	ldr	x1, .LC329
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x23, 48]
	b	.L1637
.L1659:
	ldr	x1, .LC327
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1658
.L1662:
	ldrb	w0, [x23]
	cbz	w0, .L1643
	ldr	w0, [x23, 48]
	mov	w25, 0
	cmn	w0, #1
	beq	.L1641
.L1648:
	uxtw	x26, w19
.L1647:
	add	x24, x26, x25, uxtw 1
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2312]
	add	w25, w25, 1
	add	x24, x22, x24, lsl 4
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 20]
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 24]
	ldrb	w0, [x23, 2]
	cbnz	w0, .L1645
	ldrb	w0, [x22]
	cbz	w0, .L1646
.L1645:
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 28]
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 32]
.L1646:
	ldr	w0, [x23, 48]
	add	w0, w0, 1
	cmp	w0, w25
	bhi	.L1647
	b	.L1643
.L1663:
	ldrb	w0, [x23, 1]
	cbz	w0, .L1642
	ldr	w0, [x23, 48]
	cmn	w0, #1
	beq	.L1642
	mov	w25, 0
	b	.L1648
	.size	HEVC_Sei_Buf_Period, .-HEVC_Sei_Buf_Period
	.align	3
.LC325:
	.xword	.LC324
	.align	3
.LC327:
	.xword	.LC326
	.align	3
.LC329:
	.xword	.LC328
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Pic_Timming
	.type	HEVC_Sei_Pic_Timming, %function
HEVC_Sei_Pic_Timming:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x2
	mov	x21, x1
	bl	_mcount
	add	x0, x20, 24576
	ldr	w2, [x0, 3476]
	ldr	w1, [x0, 3480]
	add	w2, w2, 1
	str	w2, [x0, 3476]
	add	w1, w1, 1
	str	w1, [x0, 3480]
	ldrb	w0, [x19, 9]
	cbnz	w0, .L1689
.L1665:
	ldrh	w0, [x19, 108]
	add	x19, x19, 108
	cbz	w0, .L1668
	ldr	w1, [x19, 32]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x21, 12]
	ldr	w1, [x19, 36]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x21, 16]
	ldrb	w0, [x19, 2]
	cbnz	w0, .L1690
.L1668:
	ldr	x0, [x20, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L1667:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1689:
	ldr	x0, [x20, 2312]
	mov	w1, 4
	bl	BsGet
	str	w0, [x21, 4]
	mov	w1, 2
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x21, 8]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x21]
	b	.L1665
	.p2align 3
.L1690:
	ldr	w1, [x19, 12]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x21, 20]
	ldrb	w0, [x19, 2]
	cbz	w0, .L1668
	ldrb	w0, [x19, 3]
	cbz	w0, .L1668
	ldr	x1, .LC331
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x21, 24]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 1]
	cbnz	w0, .L1691
.L1669:
	ldr	w0, [x21, 24]
	cmp	w0, 255
	bhi	.L1692
	ldr	x24, .LC335
	mov	w22, 0
	b	.L1672
	.p2align 3
.L1671:
	ldr	w0, [x21, 24]
	cmp	w0, w22
	bcc	.L1668
.L1672:
	add	x23, x21, x22, uxtw 2
	ldr	x0, [x20, 2312]
	mov	x1, x24
	add	w22, w22, 1
	bl	HEVC_ue_v
	str	w0, [x23, 32]
	ldrb	w0, [x21, 1]
	cbnz	w0, .L1671
	ldr	w1, [x19, 8]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x23, 1056]
	b	.L1671
	.p2align 3
.L1691:
	ldr	w1, [x19, 8]
	ldr	x0, [x20, 2312]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x21, 28]
	b	.L1669
.L1692:
	ldr	x1, .LC333
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1667
	.size	HEVC_Sei_Pic_Timming, .-HEVC_Sei_Pic_Timming
	.align	3
.LC331:
	.xword	.LC330
	.align	3
.LC333:
	.xword	.LC332
	.align	3
.LC335:
	.xword	.LC334
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecShortTermRefPicSet
	.type	HEVC_DecShortTermRefPicSet, %function
HEVC_DecShortTermRefPicSet:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	w20, w3
	mov	x0, x30
	mov	x23, x1
	mov	x19, x2
	bl	_mcount
	cbnz	w20, .L1739
	str	wzr, [x19]
.L1696:
	ldr	x1, .LC349
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	cmp	w0, 16
	str	w0, [x19, 16]
	bhi	.L1740
	ldr	x1, .LC353
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	cmp	w0, 16
	str	w0, [x19, 20]
	bhi	.L1741
	ldr	w1, [x19, 16]
	add	w2, w0, w1
	str	w2, [x19, 24]
	cmp	w2, 16
	bhi	.L1722
	mov	w20, 0
	cbz	w1, .L1724
	ldr	x25, .LC359
	mov	w22, 0
	mov	w23, 0
	mov	w24, 32767
	b	.L1726
	.p2align 3
.L1725:
	str	w23, [x2, 72]
	add	x20, x19, x20
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x20, 53]
	ldr	w20, [x19, 16]
	cmp	w20, w22
	bls	.L1742
.L1726:
	ldr	x0, [x21, 2312]
	mov	x1, x25
	uxtw	x20, w22
	add	w22, w22, 1
	bl	HEVC_ue_v
	cmp	w0, w24
	mvn	w1, w0
	add	x2, x19, x20, lsl 2
	add	w23, w23, w1
	mov	w1, 1
	bls	.L1725
	ldr	x1, .LC361
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1739:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19]
	cbz	w0, .L1696
	ldr	w0, [x23, 228]
	cmp	w20, w0
	beq	.L1743
	sub	w20, w20, #1
	str	wzr, [x19, 4]
.L1700:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19, 8]
	ldr	x1, .LC343
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	add	w26, w0, 1
	uxtw	x2, w20
	str	w26, [x19, 12]
	lsl	x1, x2, 2
	mov	w0, 1
	sub	x1, x1, x2
	ldr	w2, [x19, 8]
	add	x25, x1, x1, lsl 4
	lsl	x24, x25, 2
	sub	w1, w0, w2, lsl 1
	add	x3, x23, x24
	mul	w26, w1, w26
	ldr	w2, [x3, 13132]
	cmp	w2, 16
	bhi	.L1744
	mov	w28, 0
	mov	w27, 0
	mov	w20, 0
	mov	w22, 0
	b	.L1708
	.p2align 3
.L1703:
	sub	w2, w0, #1
	uxtw	x1, w28
	cmp	w2, 1
	bls	.L1745
.L1704:
	add	x1, x19, x1
	add	x2, x23, x24
	add	w28, w28, 1
	strb	w0, [x1, 36]
	ldr	w0, [x2, 13132]
	cmp	w0, w28
	bcc	.L1746
.L1708:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	cbnz	w0, .L1703
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	lsl	w0, w0, 1
	sub	w2, w0, #1
	uxtw	x1, w28
	cmp	w2, 1
	bhi	.L1704
.L1745:
	add	x5, x23, x24
	uxtw	x2, w22
	add	x3, x25, x1
	add	x6, x19, x2
	add	x2, x19, x2, lsl 2
	ldr	w5, [x5, 13132]
	add	x4, x23, x3, lsl 2
	mov	w3, 0
	cmp	w5, w28
	bls	.L1705
	ldr	w3, [x4, 13180]
.L1705:
	cmp	w0, 1
	add	w3, w3, w26
	str	w3, [x2, 72]
	cset	w2, eq
	strb	w2, [x6, 53]
	tbnz	w3, #31, .L1747
	add	w27, w27, 1
	add	w22, w22, 1
.L1748:
	add	x1, x19, x1
	add	x2, x23, x24
	add	w28, w28, 1
	strb	w0, [x1, 36]
	ldr	w0, [x2, 13132]
	cmp	w0, w28
	bcs	.L1708
.L1746:
	cmp	w22, 16
	add	w0, w0, 1
	ccmp	w20, 16, 2, ls
	ccmp	w27, 16, 2, ls
	stp	w20, w27, [x19, 16]
	stp	w22, w0, [x19, 24]
	bhi	.L1709
	cmp	w22, 1
	bls	.L1715
	sub	w10, w22, #2
	add	x9, x19, 76
	add	x10, x10, 1
	mov	x7, 0
	.p2align 2
.L1714:
	add	x0, x19, x7
	mov	w3, w7
	ldr	w5, [x9]
	ldrb	w8, [x0, 54]
	tbnz	w7, #31, .L1718
	.p2align 2
.L1730:
	sxtw	x1, w3
	add	w0, w3, 1
	add	x4, x19, x1
	add	x1, x19, x1, lsl 2
	sxtw	x0, w0
	add	x6, x19, x0
	add	x0, x19, x0, lsl 2
	ldr	w2, [x1, 72]
	cmp	w5, w2
	bge	.L1716
	str	w2, [x0, 72]
	ldrb	w0, [x4, 53]
	strb	w0, [x6, 53]
	str	w5, [x1, 72]
	strb	w8, [x4, 53]
.L1716:
	subs	w3, w3, #1
	bpl	.L1730
.L1718:
	add	x7, x7, 1
	add	x9, x9, 4
	cmp	x7, x10
	bne	.L1714
.L1715:
	lsr	w7, w20, 1
	sub	w20, w20, #1
	cbz	w7, .L1712
	add	x2, x19, 72
	add	x1, x19, 53
	sub	w7, w20, w7
	.p2align 2
.L1719:
	sxtw	x0, w20
	ldrb	w4, [x1]
	add	x3, x19, x0
	ldr	w5, [x2]
	add	x0, x19, x0, lsl 2
	sub	w20, w20, #1
	cmp	w20, w7
	ldr	w6, [x0, 72]
	str	w6, [x2], 4
	ldrb	w6, [x3, 53]
	strb	w6, [x1], 1
	str	w5, [x0, 72]
	strb	w4, [x3, 53]
	bne	.L1719
.L1712:
	mov	w0, 0
.L1701:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1747:
	add	w20, w20, 1
	add	w22, w22, 1
	b	.L1748
.L1742:
	ldr	w2, [x19, 24]
.L1724:
	cmp	w2, w20
	bls	.L1712
	ldr	x24, .LC363
	mov	w23, 0
	.p2align 2
.L1727:
	ldr	x0, [x21, 2312]
	mov	x1, x24
	uxtw	x22, w20
	add	w20, w20, 1
	bl	HEVC_ue_v
	add	w0, w0, 1
	add	x2, x19, x22, lsl 2
	add	w23, w23, w0
	add	x22, x19, x22
	mov	w1, 1
	str	w23, [x2, 72]
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x22, 53]
	ldr	w0, [x19, 24]
	cmp	w0, w20
	bhi	.L1727
	b	.L1712
.L1743:
	ldr	x1, .LC337
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w20, w2
	str	w2, [x19, 4]
	bcc	.L1749
	sub	w3, w20, #1
	sub	w20, w3, w0
	cmp	w20, w3
	bls	.L1700
	ldr	x1, .LC341
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1701
.L1709:
	ldr	x1, .LC347
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1701
.L1740:
	ldr	x1, .LC351
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1701
.L1749:
	ldr	x1, .LC339
	mov	w3, w20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1701
.L1744:
	ldr	x1, .LC345
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1701
.L1722:
	ldr	x1, .LC357
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1701
.L1741:
	ldr	x1, .LC355
	mov	w0, 1
	ldr	w2, [x19, 16]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1701
	.size	HEVC_DecShortTermRefPicSet, .-HEVC_DecShortTermRefPicSet
	.align	3
.LC337:
	.xword	.LC336
	.align	3
.LC339:
	.xword	.LC338
	.align	3
.LC341:
	.xword	.LC340
	.align	3
.LC343:
	.xword	.LC342
	.align	3
.LC345:
	.xword	.LC344
	.align	3
.LC347:
	.xword	.LC346
	.align	3
.LC349:
	.xword	.LC348
	.align	3
.LC351:
	.xword	.LC350
	.align	3
.LC353:
	.xword	.LC352
	.align	3
.LC355:
	.xword	.LC354
	.align	3
.LC357:
	.xword	.LC356
	.align	3
.LC359:
	.xword	.LC358
	.align	3
.LC361:
	.xword	.LC360
	.align	3
.LC363:
	.xword	.LC362
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecHrdParam
	.type	HEVC_DecHrdParam, %function
HEVC_DecHrdParam:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	uxtb	w19, w2
	mov	w22, w3
	bl	_mcount
	cmp	x21, xzr
	ccmp	x20, xzr, 4, ne
	beq	.L1774
	cbnz	w19, .L1795
.L1753:
	ldr	x23, .LC365
	mov	w19, 0
	ldr	x25, .LC367
	ldr	x24, .LC369
	ldr	x27, .LC371
	.p2align 2
.L1771:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	uxtw	x28, w19
	bl	BsGet
	uxtb	w0, w0
	add	x1, x28, x28, lsl 2
	lsl	x1, x1, 2
	sub	x1, x1, x28
	add	x1, x1, x1, lsl 4
	add	x26, x20, x1, lsl 2
	strb	w0, [x26, 40]
	cbz	w0, .L1796
	mov	w0, 1
	strb	w0, [x26, 41]
.L1758:
	add	x2, x28, x28, lsl 2
	lsl	x2, x2, 2
	sub	x2, x2, x28
	add	x2, x2, x2, lsl 4
	add	x26, x20, x2, lsl 2
	ldrb	w0, [x26, 41]
	strb	wzr, [x26, 42]
	str	wzr, [x26, 48]
	cbz	w0, .L1759
	ldr	x0, [x21, 2312]
	mov	x1, x23
	bl	HEVC_ue_v
	str	w0, [x26, 44]
	ldrb	w1, [x26, 42]
	cbz	w1, .L1761
.L1799:
	add	x0, x28, x28, lsl 2
	lsl	x0, x0, 2
	sub	x0, x0, x28
	add	x0, x0, x0, lsl 4
	add	x0, x20, x0, lsl 2
	ldr	w0, [x0, 48]
	cmp	w0, 31
	bhi	.L1797
.L1763:
	add	x0, x28, x28, lsl 2
	mov	w2, 0
	lsl	x0, x0, 2
	sub	x3, x0, x28
	lsl	x4, x3, 4
.L1764:
	cbz	w2, .L1798
	cmp	w2, 1
	beq	.L1793
.L1770:
	add	w2, w2, 1
	cmp	w2, 2
	bne	.L1764
.L1769:
	add	w19, w19, 1
	cmp	w22, w19
	bcs	.L1771
	mov	w0, 0
.L1792:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1759:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x26, 42]
	cbnz	w1, .L1799
.L1761:
	ldr	x0, [x21, 2312]
	mov	x1, x25
	bl	HEVC_ue_v
	cmp	w0, 31
	add	x1, x28, x28, lsl 2
	lsl	x1, x1, 2
	sub	x1, x1, x28
	add	x1, x1, x1, lsl 4
	add	x1, x20, x1, lsl 2
	str	w0, [x1, 48]
	bls	.L1763
.L1797:
	mov	w2, w0
	mov	x1, x24
	mov	w0, 1
	bl	dprint_vfmw
	add	x0, x28, x28, lsl 2
	lsl	x0, x0, 2
	sub	x0, x0, x28
	add	x0, x0, x0, lsl 4
	add	x0, x20, x0, lsl 2
	str	wzr, [x0, 48]
	b	.L1763
	.p2align 3
.L1796:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x26, 41]
	b	.L1758
	.p2align 3
.L1795:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x20, 1]
	ldrh	w0, [x20]
	cbz	w0, .L1753
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 2]
	cbnz	w0, .L1800
.L1755:
	ldr	x0, [x21, 2312]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 16]
	mov	w1, 4
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 20]
	ldrb	w1, [x20, 2]
	cbnz	w1, .L1801
.L1756:
	ldr	x0, [x21, 2312]
	mov	w1, 5
	bl	BsGet
	str	w0, [x20, 28]
	mov	w1, 5
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 32]
	mov	w1, 5
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 36]
	b	.L1753
.L1774:
	mov	w0, -1
	b	.L1792
.L1801:
	ldr	x0, [x21, 2312]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 24]
	b	.L1756
.L1800:
	ldr	x0, [x21, 2312]
	mov	w1, 8
	bl	BsGet
	str	w0, [x20, 4]
	mov	w1, 5
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 8]
	mov	w1, 1
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x20, 3]
	mov	w1, 5
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 36]
	b	.L1755
.L1798:
	ldrb	w0, [x20]
	cbnz	w0, .L1776
	mov	w2, 1
.L1793:
	ldrb	w0, [x20, 1]
	cbz	w0, .L1769
.L1776:
	add	x0, x3, x4
	uxtw	x1, w2
	mov	w26, 0
	stp	x0, x1, [x29, 104]
	add	x0, x20, x0, lsl 2
	str	x0, [x29, 96]
	b	.L1773
.L1772:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	stp	x4, x3, [x29, 120]
	str	w2, [x29, 140]
	bl	BsGet
	str	w0, [x28, 1076]
	ldr	x0, [x29, 96]
	ldr	w2, [x29, 140]
	ldp	x4, x3, [x29, 120]
	ldr	w0, [x0, 48]
	cmp	w0, w26
	bcc	.L1770
.L1773:
	ldr	x0, [x21, 2312]
	mov	x1, x27
	stp	x4, x3, [x29, 120]
	str	w2, [x29, 140]
	bl	HEVC_ue_v
	ldp	x1, x28, [x29, 104]
	add	x1, x1, x26, uxtw 1
	add	x1, x1, x28
	add	x1, x20, x1, lsl 2
	str	w0, [x1, 52]
	ldr	x1, .LC373
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	add	x5, x28, x26, uxtw 1
	mov	w1, 323
	add	w26, w26, 1
	ldr	w2, [x29, 140]
	umaddl	x5, w19, w1, x5
	ldp	x4, x3, [x29, 120]
	add	x28, x20, x5, lsl 2
	str	w0, [x28, 308]
	ldrb	w0, [x20, 2]
	cbz	w0, .L1772
	ldr	x1, .LC375
	ldr	x0, [x21, 2312]
	str	w2, [x29, 140]
	bl	HEVC_ue_v
	str	w0, [x28, 564]
	ldr	x1, .LC377
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	str	w0, [x28, 820]
	ldr	w2, [x29, 140]
	ldp	x4, x3, [x29, 120]
	b	.L1772
	.size	HEVC_DecHrdParam, .-HEVC_DecHrdParam
	.align	3
.LC365:
	.xword	.LC364
	.align	3
.LC367:
	.xword	.LC366
	.align	3
.LC369:
	.xword	.LC368
	.align	3
.LC371:
	.xword	.LC370
	.align	3
.LC373:
	.xword	.LC372
	.align	3
.LC375:
	.xword	.LC374
	.align	3
.LC377:
	.xword	.LC376
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ProcessVPS
	.type	HEVC_ProcessVPS, %function
HEVC_ProcessVPS:
	stp	x29, x30, [sp, -208]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, [x21, 2312]
	mov	w1, 2
	bl	BsGet
	cmp	w0, 3
	str	w0, [x20, 12]
	beq	.L1803
	ldr	x1, .LC379
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
.L1803:
	ldr	x0, [x21, 2312]
	mov	w1, 6
	bl	BsGet
	cmp	w0, 63
	str	w0, [x20, 16]
	bhi	.L1850
	ldr	x0, [x21, 2312]
	mov	w1, 3
	bl	BsGet
	cmp	w0, 7
	mov	w25, w0
	bhi	.L1851
	add	w23, w0, 1
	str	w23, [x20, 20]
	mov	w1, 1
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x20, 1]
	mov	w1, 16
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 24]
	mov	w1, 65535
	cmp	w0, w1
	bne	.L1852
	add	x26, x20, 12288
	mov	w3, w25
	mov	w2, 1
	add	x1, x26, 148
	mov	x0, x21
	bl	HEVC_DecPTL
	cbnz	w0, .L1853
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20, 7]
	tbnz	w25, #31, .L1813
	ldr	x28, .LC391
	mov	w19, 0
	ldr	x27, .LC393
	ldr	x24, .LC395
	b	.L1815
	.p2align 3
.L1812:
	cmp	w23, w19
	beq	.L1813
.L1815:
	add	x22, x20, x19, sxtw 2
	ldr	x0, [x21, 2312]
	mov	x1, x28
	add	w19, w19, 1
	bl	HEVC_ue_v
	add	w0, w0, 1
	str	w0, [x22, 52]
	mov	x1, x27
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	str	w0, [x22, 84]
	mov	x1, x24
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	str	w0, [x22, 116]
	ldrb	w0, [x20, 7]
	cbnz	w0, .L1812
	cmp	w25, w19
	blt	.L1813
	.p2align 2
.L1838:
	add	x0, x20, x19, sxtw 2
	ldr	w1, [x20, 52]
	add	w19, w19, 1
	cmp	w23, w19
	str	w1, [x0, 52]
	ldr	w1, [x20, 84]
	str	w1, [x0, 84]
	ldr	w1, [x20, 116]
	str	w1, [x0, 116]
	bne	.L1838
.L1813:
	ldr	x0, [x21, 2312]
	mov	w1, 6
	bl	BsGet
	str	w0, [x20, 28]
	ldr	x1, .LC389
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	cmp	w0, 1024
	str	w0, [x20, 32]
	bhi	.L1854
	cmp	w0, 1
	ble	.L1817
	ldr	x27, .LC398
	mov	w22, 1
	ldr	x24, .LC400
	ldr	w0, [x20, 28]
	.p2align 2
.L1819:
	cmp	w0, wzr
	mov	w19, 0
	sxtw	x23, w22
	ble	.L1821
	.p2align 2
.L1818:
	ldr	x5, [x27, 136]
	mov	w4, w19
	mov	w3, w22
	mov	x2, x24
	mov	x1, 100
	add	x0, x29, 104
	blr	x5
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	add	x1, x23, x19, sxtw
	add	w19, w19, 1
	add	x1, x20, x1, lsl 2
	str	w0, [x1, 148]
	ldr	w0, [x20, 28]
	cmp	w0, w19
	bgt	.L1818
.L1821:
	ldr	w1, [x20, 32]
	add	w22, w22, 1
	cmp	w1, w22
	bgt	.L1819
.L1817:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 6]
	cbz	w0, .L1827
	ldr	x0, [x21, 2312]
	mov	w1, 32
	bl	BsGet
	str	w0, [x20, 36]
	mov	w1, 32
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 40]
	mov	w1, 1
	ldr	x0, [x21, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 5]
	cbnz	w0, .L1855
.L1825:
	ldr	x1, .LC404
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	cmp	w0, 1023
	str	w0, [x20, 48]
	bhi	.L1856
	cmp	w0, wzr
	ble	.L1827
	mov	w0, 1
	str	w0, [x20, 8340]
	ldr	x23, .LC408
	add	x26, x26, 532
	ldr	x0, [x21, 2312]
	mov	w19, 0
	mov	x1, x23
	bl	HEVC_ue_v
	str	w0, [x20, 4244]
	ldr	w2, [x20, 8340]
	.p2align 2
.L1828:
	mov	w3, w25
	mov	x1, x26
	mov	x0, x21
	add	x22, x20, x19, sxtw 2
	add	w19, w19, 1
	bl	HEVC_DecHrdParam
	cbnz	w0, .L1857
.L1831:
	ldr	w0, [x20, 48]
	mov	x1, x23
	cmp	w0, w19
	ble	.L1827
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	str	w0, [x22, 4248]
	cmp	w19, wzr
	mov	w1, 1
	ble	.L1858
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x22, 8344]
	mov	w2, w0
	mov	w3, w25
	mov	x1, x26
	mov	x0, x21
	add	x22, x20, x19, sxtw 2
	add	w19, w19, 1
	bl	HEVC_DecHrdParam
	cbz	w0, .L1831
.L1857:
	ldr	x1, .LC410
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
.L1827:
	ldr	x0, [x21, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 3]
	cbz	w0, .L1824
	.p2align 2
.L1845:
	ldr	x0, [x21, 2312]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L1824
	ldr	x2, [x21, 2360]
	mov	w1, 1
	ldr	w2, [x2, 12]
	add	w2, w2, 3
	cmp	w2, w0
	bcc	.L1859
.L1824:
	mov	w0, 0
.L1846:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L1859:
	ldr	x0, [x21, 2312]
	bl	BsGet
	strb	w0, [x20, 4]
	b	.L1845
	.p2align 3
.L1858:
	ldr	w2, [x22, 8344]
	b	.L1828
.L1855:
	ldr	x1, .LC402
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	str	w0, [x20, 44]
	b	.L1825
.L1852:
	ldr	x1, .LC385
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1846
.L1850:
	ldr	x1, .LC381
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1846
.L1853:
	ldr	x1, .LC387
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1846
.L1851:
	ldr	x1, .LC383
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1846
.L1854:
	ldr	x1, .LC397
.L1848:
	mov	w2, w0
	mov	w3, 1024
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1846
.L1856:
	ldr	x1, .LC406
	b	.L1848
	.size	HEVC_ProcessVPS, .-HEVC_ProcessVPS
	.align	3
.LC379:
	.xword	.LC378
	.align	3
.LC381:
	.xword	.LC380
	.align	3
.LC383:
	.xword	.LC382
	.align	3
.LC385:
	.xword	.LC384
	.align	3
.LC387:
	.xword	.LC386
	.align	3
.LC389:
	.xword	.LC388
	.align	3
.LC391:
	.xword	.LC390
	.align	3
.LC393:
	.xword	.LC392
	.align	3
.LC395:
	.xword	.LC394
	.align	3
.LC397:
	.xword	.LC396
	.align	3
.LC398:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC400:
	.xword	.LC399
	.align	3
.LC402:
	.xword	.LC401
	.align	3
.LC404:
	.xword	.LC403
	.align	3
.LC406:
	.xword	.LC405
	.align	3
.LC408:
	.xword	.LC407
	.align	3
.LC410:
	.xword	.LC409
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecVPS
	.type	HEVC_DecVPS, %function
HEVC_DecVPS:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x20, 2312]
	mov	w1, 4
	bl	BsGet
	cmp	w0, 15
	mov	w21, w0
	bhi	.L1866
	mov	w19, 23196
	umull	x19, w0, w19
	ldr	x0, [x20, 2320]
	add	x1, x0, x19
	ldrb	w0, [x0, x19]
	cbnz	w0, .L1867
	str	w21, [x1, 8]
	mov	x0, x20
	ldr	x1, [x20, 2320]
	add	x1, x1, x19
	bl	HEVC_ProcessVPS
	mov	w22, w0
	cbnz	w0, .L1868
	ldr	x0, [x20, 2320]
	mov	w1, 1
	add	x0, x0, x19
	strb	w1, [x0, 2]
	ldr	x0, [x20, 2320]
	strb	w1, [x0, x19]
.L1862:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1867:
	ldr	x24, .LC413
	mov	x0, x20
	add	x23, x24, 3328
	mov	x1, x23
	str	w21, [x23, 8]
	bl	HEVC_ProcessVPS
	mov	w22, w0
	cbnz	w0, .L1869
	ldr	x3, .LC416
	mov	w1, 1
	ldr	x0, [x20, 2320]
	mov	x2, 23196
	strb	w1, [x23, 2]
	ldr	x3, [x3, 112]
	add	x0, x0, x19
	strb	w1, [x24, 3328]
	mov	x1, x23
	blr	x3
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1869:
	ldr	x1, .LC415
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1862
	.p2align 3
.L1866:
	ldr	x1, .LC412
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1862
.L1868:
	ldr	x1, .LC418
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x0, [x20, 2320]
	mov	w1, 1
	add	x0, x0, x19
	strb	w1, [x0, 2]
	ldr	x0, [x20, 2320]
	strb	wzr, [x0, x19]
	b	.L1862
	.size	HEVC_DecVPS, .-HEVC_DecVPS
	.align	3
.LC412:
	.xword	.LC411
	.align	3
.LC413:
	.xword	.LANCHOR6
	.align	3
.LC415:
	.xword	.LC414
	.align	3
.LC416:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC418:
	.xword	.LC417
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecVuiParam
	.type	HEVC_DecVuiParam, %function
HEVC_DecVuiParam:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	mov	x21, x2
	bl	_mcount
	mov	w2, 1
	ldr	x0, [x20, 2312]
	mov	w3, 2
	mov	w1, w2
	stp	w2, w3, [x29, 48]
	stp	w3, w2, [x29, 56]
	stp	w2, w3, [x29, 64]
	stp	w2, w2, [x29, 72]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19]
	cbnz	w0, .L1911
.L1872:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 1]
	cbnz	w0, .L1912
.L1874:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3]
	cbnz	w0, .L1913
.L1876:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 6]
	cbnz	w0, .L1914
.L1878:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 8]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 9]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 10]
	cbz	w0, .L1879
	ldr	x1, .LC425
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 60]
	ldr	w1, [x21, 64]
	cmp	w1, 3
	bhi	.L1915
	add	x2, x29, 80
	add	x2, x2, x1, uxtw 2
	ldr	x1, .LC429
	ldr	w2, [x2, -32]
	mul	w0, w2, w0
	str	w0, [x19, 60]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 64]
	add	x3, x29, 80
	ldr	x1, .LC431
	ldr	w2, [x21, 64]
	add	x2, x3, x2, lsl 2
	ldr	w2, [x2, -32]
	mul	w0, w2, w0
	str	w0, [x19, 64]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 68]
	add	x3, x29, 80
	ldr	x1, .LC433
	ldr	w2, [x21, 64]
	add	x2, x3, x2, lsl 2
	ldr	w2, [x2, -16]
	mul	w0, w2, w0
	str	w0, [x19, 68]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 72]
	add	x2, x29, 80
	ldr	w1, [x21, 64]
	add	x1, x2, x1, lsl 2
	ldr	w1, [x1, -16]
	mul	w0, w1, w0
	str	w0, [x19, 72]
.L1879:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 11]
	cbz	w0, .L1882
	ldr	x0, [x20, 2312]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 76]
	mov	w1, 32
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 80]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 12]
	cbnz	w0, .L1916
.L1883:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 13]
	cbnz	w0, .L1917
.L1882:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 14]
	mov	w1, 0
	cbnz	w0, .L1918
.L1881:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, w1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1918:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 15]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 16]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 17]
	ldr	x1, .LC439
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 4095
	str	w0, [x19, 88]
	bhi	.L1919
.L1884:
	ldr	x1, .LC443
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 92]
	ldr	x1, .LC445
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 96]
	ldr	x1, .LC447
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 100]
	ldr	x1, .LC449
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 104]
	mov	w1, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, w1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1914:
	ldr	x1, .LC421
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 52]
	ldr	x1, .LC423
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 56]
	b	.L1878
	.p2align 3
.L1911:
	ldr	x0, [x20, 2312]
	mov	w1, 8
	bl	BsGet
	cmp	w0, 255
	str	w0, [x19, 24]
	bne	.L1872
	ldr	x0, [x20, 2312]
	mov	w1, 16
	bl	BsGet
	str	w0, [x19, 28]
	mov	w1, 16
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 32]
	b	.L1872
	.p2align 3
.L1913:
	ldr	x0, [x20, 2312]
	mov	w1, 3
	bl	BsGet
	str	w0, [x19, 36]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	cbz	w0, .L1876
	ldr	x0, [x20, 2312]
	mov	w1, 8
	bl	BsGet
	str	w0, [x19, 40]
	mov	w1, 8
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 44]
	mov	w1, 8
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 48]
	b	.L1876
	.p2align 3
.L1912:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 2]
	b	.L1874
	.p2align 3
.L1917:
	ldr	w3, [x21, 56]
	mov	w2, 1
	add	x1, x19, 108
	mov	x0, x20
	bl	HEVC_DecHrdParam
	cbz	w0, .L1882
	ldr	x1, .LC437
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1881
	.p2align 3
.L1916:
	ldr	x1, .LC435
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 84]
	b	.L1883
	.p2align 3
.L1919:
	ldr	x1, .LC441
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1884
	.p2align 3
.L1915:
	ldr	x1, .LC427
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1881
	.size	HEVC_DecVuiParam, .-HEVC_DecVuiParam
	.align	3
.LC421:
	.xword	.LC420
	.align	3
.LC423:
	.xword	.LC422
	.align	3
.LC425:
	.xword	.LC424
	.align	3
.LC427:
	.xword	.LC426
	.align	3
.LC429:
	.xword	.LC428
	.align	3
.LC431:
	.xword	.LC430
	.align	3
.LC433:
	.xword	.LC432
	.align	3
.LC435:
	.xword	.LC434
	.align	3
.LC437:
	.xword	.LC436
	.align	3
.LC439:
	.xword	.LC438
	.align	3
.LC441:
	.xword	.LC440
	.align	3
.LC443:
	.xword	.LC442
	.align	3
.LC445:
	.xword	.LC444
	.align	3
.LC447:
	.xword	.LC446
	.align	3
.LC449:
	.xword	.LC448
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_se_v
	.type	HEVC_se_v, %function
HEVC_se_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 32
	mov	x0, x20
	bl	BsShow
	bl	ZerosMS_32
	cmp	w0, 31
	bls	.L1925
	mov	w0, 2147483647
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1925:
	mov	w1, w0
	mov	x0, x20
	add	w21, w1, 1
	bl	BsSkip
	mov	w1, w21
	mov	x0, x20
	bl	BsShow
	sub	w0, w0, #1
	asr	w2, w0, 1
	and	w0, w0, 1
	cmp	w0, wzr
	mov	w1, w21
	mov	x0, x20
	neg	w19, w2
	csinc	w19, w19, w2, eq
	bl	BsSkip
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_se_v, .-HEVC_se_v
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Structure_Of_Pictures_Info
	.type	HEVC_Sei_Structure_Of_Pictures_Info, %function
HEVC_Sei_Structure_Of_Pictures_Info:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	ldr	x1, .LC451
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	str	w0, [x22]
	ldr	x1, .LC453
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	cmp	w0, 1024
	str	w0, [x22, 4]
	bhi	.L1927
	mov	w19, 0
	cbz	w0, .L1933
	ldr	x23, .LC457
	ldr	x24, .LC459
	.p2align 2
.L1936:
	add	x20, x22, x19, uxtw 2
	ldr	x0, [x21, 2312]
	mov	w1, 6
	bl	BsGet
	str	w0, [x20, 8]
	mov	w1, 3
	ldr	x0, [x21, 2312]
	bl	BsGet
	str	w0, [x20, 4104]
	ldr	w2, [x20, 8]
	mov	x1, x23
	sub	w2, w2, #19
	cmp	w2, 1
	bls	.L1930
	ldr	x0, [x21, 2312]
	bl	HEVC_ue_v
	str	w0, [x20, 8200]
.L1930:
	cbnz	w19, .L1939
.L1931:
	ldr	w0, [x22, 4]
	add	w19, w19, 1
	cmp	w0, w19
	bhi	.L1936
.L1933:
	ldr	x0, [x21, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L1929:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1939:
	ldr	x0, [x21, 2312]
	mov	x1, x24
	bl	HEVC_se_v
	str	w0, [x20, 12296]
	b	.L1931
.L1927:
	ldr	x1, .LC455
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1929
	.size	HEVC_Sei_Structure_Of_Pictures_Info, .-HEVC_Sei_Structure_Of_Pictures_Info
	.align	3
.LC451:
	.xword	.LC450
	.align	3
.LC453:
	.xword	.LC452
	.align	3
.LC455:
	.xword	.LC454
	.align	3
.LC457:
	.xword	.LC456
	.align	3
.LC459:
	.xword	.LC458
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Recovery_Point
	.type	HEVC_Sei_Recovery_Point, %function
HEVC_Sei_Recovery_Point:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	x19, xzr
	ccmp	x20, xzr, 4, ne
	beq	.L1942
	ldr	x1, .LC461
	ldr	x0, [x19, 2312]
	bl	HEVC_se_v
	str	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	strb	w0, [x20, 1]
	ldr	x0, [x19, 2312]
	bl	BsToNextByte
	mov	w0, 0
.L1941:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1942:
	mov	w0, -1
	b	.L1941
	.size	HEVC_Sei_Recovery_Point, .-HEVC_Sei_Recovery_Point
	.align	3
.LC461:
	.xword	.LC460
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Payload
	.type	HEVC_Sei_Payload, %function
HEVC_Sei_Payload:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	mov	x23, x2
	mov	w20, w3
	mov	w22, w4
	bl	_mcount
	ldr	x0, [x19, 2360]
	ldr	w0, [x0, 16]
	cmp	w0, 39
	beq	.L1990
	cmp	w20, 5
	beq	.L1970
	cmp	w20, 132
	beq	.L1971
	mov	w20, 0
	cbz	w22, .L1964
	.p2align 2
.L1984:
	ldr	x0, [x19, 2312]
	mov	w1, 8
	add	w20, w20, 1
	bl	BsGet
	cmp	w22, w20
	bne	.L1984
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_Sei_More_Data_In_Payload
	cbnz	w0, .L1974
	.p2align 2
.L1978:
	mov	w0, 0
.L1966:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1970:
	mov	w2, w22
	add	x1, x21, 3200
	mov	x0, x19
	bl	HEVC_Sei_User_Data_Unregistered
	cbnz	w0, .L1967
.L1964:
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_Sei_More_Data_In_Payload
	cbz	w0, .L1978
.L1974:
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_Sei_Payload_Extension_Present
	cbnz	w0, .L1991
.L1975:
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x21, 12]
	b	.L1976
	.p2align 3
.L1977:
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x21, 16]
.L1976:
	ldr	x0, [x19, 2312]
	bl	BsIsByteAligned
	cbz	w0, .L1977
	b	.L1978
	.p2align 3
.L1990:
	cmp	w20, 147
	bls	.L1992
.L1945:
	mov	w20, 0
	cbz	w22, .L1964
	.p2align 2
.L1983:
	ldr	x0, [x19, 2312]
	mov	w1, 8
	add	w20, w20, 1
	bl	BsGet
	cmp	w22, w20
	bne	.L1983
	b	.L1964
	.p2align 3
.L1971:
	add	x1, x21, 20480
	mov	x0, x19
	add	x1, x1, 3348
	bl	HEVC_Sei_Decoded_Picture_Hash
	cbz	w0, .L1964
	.p2align 2
.L1967:
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1992:
	ldr	x0, .LC462
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx1947
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1947:
	.section	.rodata
	.align	0
	.align	2
.L1947:
	.byte	(.L1946 - .Lrtx1947) / 4
	.byte	(.L1948 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1970 - .Lrtx1947) / 4
	.byte	(.L1950 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1951 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1952 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1953 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1954 - .Lrtx1947) / 4
	.byte	(.L1955 - .Lrtx1947) / 4
	.byte	(.L1956 - .Lrtx1947) / 4
	.byte	(.L1957 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1958 - .Lrtx1947) / 4
	.byte	(.L1959 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1960 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1961 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1945 - .Lrtx1947) / 4
	.byte	(.L1962 - .Lrtx1947) / 4
	.text
	.p2align 3
.L1991:
	ldr	x1, .LC464
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	str	w0, [x21, 8]
	b	.L1975
.L1962:
	add	x0, x21, 3486
	cmp	x19, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L1967
	ldr	x0, [x19, 2312]
	mov	w1, 8
	bl	BsGet
	strb	w0, [x21, 3486]
	b	.L1964
.L1961:
	add	x1, x21, 3480
	mov	x0, x19
	bl	HEVC_Sei_Content_Light_Level_Information
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1960:
	add	x1, x21, 3436
	cmp	x19, xzr
	ccmp	x1, xzr, 4, ne
	beq	.L1967
	mov	w0, 1
	strb	w0, [x21, 3436]
	mov	x0, x19
	bl	HEVC_Sei_Mastering_Display_Colour_Volume.part.11
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1959:
	add	x0, x21, 3432
	cmp	x19, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L1967
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21, 3432]
	b	.L1964
.L1958:
	mov	w3, w22
	mov	x2, x23
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_Sei_Scalable_Nesting
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1957:
	add	x1, x21, 3424
	mov	x0, x19
	bl	HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1956:
	add	x2, x23, 2732
	add	x1, x21, 3348
	mov	x0, x19
	bl	HEVC_Sei_Dec_Unit_Info
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1955:
	add	x1, x21, 3272
	mov	x0, x19
	bl	HEVC_Sei_Active_Param_sets
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1954:
	add	x1, x21, 4096
	mov	w2, w22
	add	x1, x1, 2552
	mov	x0, x19
	bl	HEVC_Sei_Structure_Of_Pictures_Info
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1953:
	add	x1, x21, 3416
	mov	x0, x19
	bl	HEVC_Sei_Display_Orientation
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1952:
	add	x1, x21, 3372
	mov	x0, x19
	bl	HEVC_Sei_Frame_Packing_Arrangement
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1951:
	add	x1, x21, 3488
	mov	x0, x19
	bl	HEVC_Sei_Tone_Mapping_Info
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1950:
	add	x1, x21, 3364
	mov	x0, x19
	bl	HEVC_Sei_Recovery_Point
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1948:
	add	x2, x23, 2624
	add	x1, x21, 1088
	mov	x0, x19
	bl	HEVC_Sei_Pic_Timming
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.p2align 3
.L1946:
	add	x2, x23, 2732
	add	x1, x21, 24
	mov	x0, x19
	bl	HEVC_Sei_Buf_Period
	cbz	w0, .L1964
	mov	w0, -1
	b	.L1966
	.size	HEVC_Sei_Payload, .-HEVC_Sei_Payload
	.align	3
.LC462:
	.xword	.L1947
	.align	3
.LC464:
	.xword	.LC463
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_SeiMessage
	.type	HEVC_SeiMessage, %function
HEVC_SeiMessage:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x1
	mov	x23, x2
	bl	_mcount
	mov	w20, 0
	ldr	x0, [x19, 2312]
	b	.L1994
	.p2align 3
.L1996:
	bl	BsSkip
	add	w20, w20, 255
	ldr	x0, [x19, 2312]
	ldr	w3, [x0, 32]
	ldr	w4, [x0, 24]
	add	w1, w3, 8
	cmp	w1, w4
	bhi	.L2003
.L1994:
	mov	w1, 8
	bl	BsShow
	cmp	w0, 255
	mov	w1, 8
	ldr	x0, [x19, 2312]
	beq	.L1996
	bl	BsGet
	str	w0, [x22]
	mov	w24, w0
	mov	w21, 0
	ldr	x0, [x19, 2312]
	b	.L1997
	.p2align 3
.L1998:
	bl	BsSkip
	add	w21, w21, 255
	ldr	x0, [x19, 2312]
	ldr	w3, [x0, 32]
	ldr	w4, [x0, 24]
	add	w1, w3, 8
	cmp	w1, w4
	bhi	.L2004
.L1997:
	mov	w1, 8
	bl	BsShow
	cmp	w0, 255
	mov	w1, 8
	ldr	x0, [x19, 2312]
	beq	.L1998
	bl	BsGet
	add	w4, w21, w0
	mov	x1, x22
	str	w0, [x22, 4]
	add	w3, w20, w24
	mov	x2, x23
	mov	x0, x19
	bl	HEVC_Sei_Payload
	mov	w1, 0
	cbnz	w0, .L2005
.L1995:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2003:
	ldr	x1, .LC466
	mov	w2, 3041
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2004:
	ldr	x1, .LC466
	mov	w2, 3063
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L2005:
	ldr	x1, .LC468
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1995
	.size	HEVC_SeiMessage, .-HEVC_SeiMessage
	.align	3
.LC466:
	.xword	.LC465
	.align	3
.LC468:
	.xword	.LC467
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecSEI
	.type	HEVC_DecSEI, %function
HEVC_DecSEI:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x22, x19, 4096
	add	x21, x19, 24576
	bl	_mcount
	add	x22, x22, 64
	add	x21, x21, 3576
	b	.L2010
	.p2align 3
.L2007:
	ldr	x0, [x19, 2312]
	bl	BsIsByteAligned
	cbz	w0, .L2015
	mov	x0, x19
	bl	HEVC_More_Rbsp_Data
	mov	w20, w0
	cbz	w0, .L2016
.L2010:
	mov	x2, x21
	mov	x1, x22
	mov	x0, x19
	bl	HEVC_SeiMessage
	cbz	w0, .L2007
	ldr	x1, .LC470
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
.L2008:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2015:
	ldr	x1, .LC472
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2016:
	ldr	x0, [x19, 2312]
	mov	w1, 8
	bl	BsGet
	cmp	w0, 128
	beq	.L2008
	ldr	x1, .LC474
	mov	w2, w0
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	b	.L2008
	.size	HEVC_DecSEI, .-HEVC_DecSEI
	.align	3
.LC470:
	.xword	.LC469
	.align	3
.LC472:
	.xword	.LC471
	.align	3
.LC474:
	.xword	.LC473
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_Sei_Scalable_Nesting
	.type	HEVC_Sei_Scalable_Nesting, %function
HEVC_Sei_Scalable_Nesting:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x1
	mov	x23, x2
	bl	_mcount
	add	x24, x22, 20480
	ldr	x0, [x19, 2312]
	mov	w1, 1
	add	x21, x24, 2560
	bl	BsGet
	strb	w0, [x24, 2560]
	mov	w1, 1
	ldr	x0, [x19, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 1]
	cbz	w0, .L2018
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21, 2]
	ldr	x1, .LC476
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	cmp	w0, 63
	str	w0, [x21, 8]
	bhi	.L2033
	ldrb	w21, [x21, 2]
	cmp	w0, w21
	bcc	.L2024
	ldr	x25, .LC480
	.p2align 2
.L2023:
	add	x20, x22, x21, uxtw 2
	ldr	x0, [x19, 2312]
	add	x20, x20, 20480
	mov	w1, 3
	add	w21, w21, 1
	bl	BsGet
	str	w0, [x20, 2572]
	mov	x1, x25
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	str	w0, [x20, 2828]
	ldr	w0, [x24, 2568]
	cmp	w0, w21
	bcs	.L2023
.L2024:
	ldr	x0, [x19, 2312]
	bl	BsToNextByte
	strb	wzr, [x24, 2564]
.L2022:
	mov	x2, x23
	mov	x1, x22
	mov	x0, x19
	bl	HEVC_SeiMessage
	cbnz	w0, .L2034
	ldr	x0, [x19, 2312]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L2028
	ldr	x1, [x19, 2360]
	ldr	w1, [x1, 12]
	add	w1, w1, 3
	cmp	w1, w0
	bcc	.L2022
.L2028:
	mov	w0, 0
.L2020:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L2018:
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 3]
	cbnz	w0, .L2024
	ldr	x0, [x19, 2312]
	mov	w1, 3
	mov	w20, 0
	mov	x25, x21
	bl	BsGet
	str	w0, [x21, 524]
	ldr	x1, .LC482
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	cmp	w0, 63
	str	w0, [x21, 528]
	bhi	.L2035
	.p2align 2
.L2029:
	ldr	x0, [x19, 2312]
	mov	w1, 6
	bl	BsGet
	add	x1, x25, x20, uxtw 2
	add	w20, w20, 1
	str	w0, [x1, 532]
	ldr	w0, [x25, 528]
	cmp	w0, w20
	bcs	.L2029
	b	.L2024
	.p2align 3
.L2034:
	ldr	x1, .LC485
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L2033:
	ldr	x1, .LC478
	mov	w2, 64
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2020
.L2035:
	ldr	x1, .LC484
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2020
	.size	HEVC_Sei_Scalable_Nesting, .-HEVC_Sei_Scalable_Nesting
	.align	3
.LC476:
	.xword	.LC475
	.align	3
.LC478:
	.xword	.LC477
	.align	3
.LC480:
	.xword	.LC479
	.align	3
.LC482:
	.xword	.LC481
	.align	3
.LC484:
	.xword	.LC483
	.align	3
.LC485:
	.xword	.LC469
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecScalingListData
	.type	HEVC_DecScalingListData, %function
HEVC_DecScalingListData:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	add	x28, x20, 292
	bl	_mcount
	mov	w26, 0
	ldr	x3, .LC487
	mov	x2, 64
	add	x1, x19, 1265
	add	x0, x19, 1393
	ldr	x3, [x3, 104]
	blr	x3
	ldr	x22, .LC499
	ldr	x0, .LC486
	add	x0, x0, 65536
	str	x0, [x29, 104]
.L2037:
	ldr	w0, [x28, 1904]
	mov	w23, 0
	uxtw	x24, w26
	cbz	w0, .L2057
	lsl	x0, x24, 2
	sub	x24, x0, x24
	lsl	x0, x24, 1
	str	x0, [x29, 136]
	add	x0, x19, x0
	str	x0, [x29, 128]
.L2065:
	ldp	x1, x2, [x29, 128]
	uxtw	x24, w23
	ldr	x0, [x20, 2312]
	add	x27, x1, x24
	mov	w1, 1
	add	x25, x24, x2
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 25]
	add	x21, x19, x25, lsl 6
	mov	w1, 64
	add	x21, x21, 49
	ldr	w2, [x28]
	cmp	w2, 64
	csel	w2, w2, w1, le
	cbz	w26, .L2038
	add	x3, x20, 324
	cbz	w0, .L2070
	cmp	w26, 1
	bls	.L2063
	ldr	x1, .LC495
	ldr	x0, [x20, 2312]
	str	w2, [x29, 116]
	str	x3, [x29, 120]
	bl	HEVC_se_v
	add	w1, w0, 7
	cmp	w1, 254
	str	w0, [x19, 1660]
	ldr	w2, [x29, 116]
	ldr	x3, [x29, 120]
	bhi	.L2071
	add	x25, x19, x25, lsl 2
	add	w27, w0, 8
	str	w27, [x25, 1668]
.L2049:
	cmp	w2, wzr
	ble	.L2048
	sub	w25, w2, #1
	mov	x24, x3
	add	x25, x25, 1
	add	x25, x3, x25, lsl 2
	b	.L2053
	.p2align 3
.L2052:
	ldr	w0, [x24], 4
	and	w1, w1, 255
	sub	w27, w1, w27
	cmp	x24, x25
	strb	w27, [x21, x0]
	beq	.L2048
.L2053:
	ldr	x0, [x20, 2312]
	mov	x1, x22
	bl	HEVC_se_v
	add	w27, w0, w27
	add	w1, w27, 256
	add	w2, w0, 128
	asr	w27, w1, 31
	cmp	w2, 255
	str	w0, [x19, 1664]
	lsr	w27, w27, 24
	add	w1, w1, w27
	bls	.L2052
	ldr	x1, .LC501
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L2068:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2070:
	ldr	x1, .LC489
	ldr	x0, [x20, 2312]
	str	w2, [x29, 120]
	bl	HEVC_ue_v
	cmp	w0, w23
	ldr	w2, [x29, 120]
	bhi	.L2060
	uxtb	w0, w0
	strb	w0, [x27, 1585]
	sub	w0, w23, w0
	cmp	w26, 1
	uxtb	w0, w0
	strb	w0, [x27, 1609]
	bls	.L2043
	cmp	w0, w23
	beq	.L2062
	ldr	x1, [x29, 136]
	add	x1, x1, x0, uxtb
	add	x1, x19, x1, lsl 2
	ldr	w3, [x1, 1668]
.L2044:
	ldr	x1, [x29, 136]
	add	x1, x1, x24
	add	x1, x19, x1, lsl 2
	str	w3, [x1, 1668]
.L2043:
	cmp	w0, w23
	beq	.L2072
	ldr	x1, [x29, 136]
	add	x1, x1, x0, uxtb
	add	x1, x19, x1, lsl 6
	add	x1, x1, 49
.L2046:
	cbz	x1, .L2073
	ldr	x3, .LC487
	sxtw	x2, w2
	mov	x0, x21
	ldr	x3, [x3, 104]
	blr	x3
.L2048:
	ldr	w0, [x28, 1904]
	add	w23, w23, 1
	cmp	w0, w23
	bhi	.L2065
.L2057:
	add	w26, w26, 1
	add	x28, x28, 4
	cmp	w26, 4
	bne	.L2037
	mov	w0, 0
	b	.L2068
	.p2align 3
.L2063:
	mov	w27, 8
	b	.L2049
.L2062:
	mov	w3, 16
	b	.L2044
.L2072:
	mov	w1, w23
	mov	w0, w26
	str	w2, [x29, 120]
	bl	HEVC_GetScalingListDefaultAddress
	mov	x1, x0
	ldr	w2, [x29, 120]
	b	.L2046
.L2071:
	ldr	x1, .LC497
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2068
.L2073:
	ldr	x1, .LC493
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2068
.L2060:
	ldr	x1, .LC491
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2068
.L2038:
	cbz	w0, .L2058
	mov	w27, 8
	ldr	x3, [x29, 104]
	b	.L2049
.L2058:
	ldr	x1, .LC489
	ldr	x0, [x20, 2312]
	str	w2, [x29, 120]
	bl	HEVC_ue_v
	cmp	w23, w0
	ldr	w2, [x29, 120]
	bcc	.L2060
	add	x4, x19, x24
	uxtb	w0, w0
	strb	w0, [x4, 1585]
	sub	w0, w23, w0
	uxtb	w0, w0
	strb	w0, [x4, 1609]
	b	.L2043
	.size	HEVC_DecScalingListData, .-HEVC_DecScalingListData
	.align	3
.LC486:
	.xword	.LANCHOR0
	.align	3
.LC487:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC489:
	.xword	.LC488
	.align	3
.LC491:
	.xword	.LC490
	.align	3
.LC493:
	.xword	.LC492
	.align	3
.LC495:
	.xword	.LC494
	.align	3
.LC497:
	.xword	.LC496
	.align	3
.LC499:
	.xword	.LC498
	.align	3
.LC501:
	.xword	.LC500
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ProcessSPS
	.type	HEVC_ProcessSPS, %function
HEVC_ProcessSPS:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x1, .LC504
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 1
	str	w0, [x19, 64]
	mov	w23, w0
	beq	.L2075
	cmp	w0, 3
	bhi	.L2209
	ldr	x1, .LC508
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L2205:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2209:
	ldr	x1, .LC506
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2075:
	ldr	x1, .LC510
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 8192
	str	w0, [x19, 72]
	bhi	.L2210
	ldr	x1, .LC514
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 4096
	str	w0, [x19, 76]
	bhi	.L2211
	ldr	w2, [x19, 760]
	ldr	w1, [x19, 72]
	cmp	w2, 120
	mul	w0, w0, w1
	beq	.L2081
	bgt	.L2082
	cmp	w2, 60
	beq	.L2145
	ble	.L2212
	cmp	w2, 90
	beq	.L2086
	cmp	w2, 93
	beq	.L2087
	cmp	w2, 63
	bne	.L2080
	mov	w1, 61440
	mov	w2, 245760
	b	.L2083
	.p2align 3
.L2082:
	cmp	w2, 156
	beq	.L2089
	bgt	.L2090
	cmp	w2, 150
	beq	.L2089
	cmp	w2, 153
	beq	.L2089
	cmp	w2, 123
	beq	.L2081
.L2080:
	ldr	x1, .LC518
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
	.p2align 3
.L2090:
	cmp	w2, 183
	beq	.L2091
	cmp	w2, 186
	beq	.L2091
	cmp	w2, 180
	bne	.L2080
.L2091:
	mov	w1, 8912896
	mov	w2, 35651584
	.p2align 2
.L2083:
	cmp	w0, w1
	bhi	.L2092
	mov	w0, 16
	str	w0, [x19, 236]
.L2093:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19, 80]
	cbz	w0, .L2096
	ldr	x1, .LC520
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	ldr	x21, .LC521
	ldr	w2, [x19, 64]
	ldr	x1, .LC523
	add	x2, x21, x2, lsl 2
	ldr	w2, [x2, 16]
	mul	w0, w2, w0
	str	w0, [x19, 84]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	ldr	w2, [x19, 64]
	ldr	w1, [x19, 72]
	ldr	w3, [x19, 84]
	add	x2, x21, x2, lsl 2
	sub	w1, w1, w3
	ldr	w2, [x2, 16]
	mul	w0, w0, w2
	str	w0, [x19, 88]
	sub	w1, w1, w0
	tbnz	w1, #31, .L2213
	ldr	x1, .LC527
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	ldr	w2, [x19, 64]
	ldr	x1, .LC529
	add	x2, x21, x2, lsl 2
	ldr	w2, [x2, 32]
	mul	w0, w2, w0
	str	w0, [x19, 92]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	ldr	w2, [x19, 64]
	ldr	w1, [x19, 76]
	ldr	w3, [x19, 92]
	add	x21, x21, x2, lsl 2
	sub	w1, w1, w3
	ldr	w2, [x21, 32]
	mul	w0, w0, w2
	str	w0, [x19, 96]
	sub	w1, w1, w0
	tbnz	w1, #31, .L2214
.L2096:
	ldr	x1, .LC533
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 8
	cmp	w0, 6
	str	w2, [x19, 100]
	bhi	.L2215
	ldr	x1, .LC537
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 8
	cmp	w0, 6
	str	w2, [x19, 108]
	bhi	.L2216
	ldr	w0, [x19, 100]
	str	w0, [x20, 192]
	ldr	x1, .LC541
	ldr	w0, [x19, 108]
	str	w0, [x20, 196]
	ldr	w4, [x19, 100]
	ldr	w3, [x19, 108]
	lsl	w2, w4, 2
	lsl	w0, w3, 2
	sub	w2, w2, w4
	sub	w0, w0, w3
	lsl	w2, w2, 1
	lsl	w0, w0, 1
	sub	w2, w2, #48
	sub	w0, w0, #48
	str	w2, [x19, 104]
	str	w0, [x19, 112]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 12
	bhi	.L2217
	add	w0, w0, 4
	mov	w1, 1
	stp	w0, w0, [x19, 128]
	mov	w21, 0
	ldr	x0, [x20, 2312]
	bl	BsGet
	ldr	x28, .LC545
	ldr	x27, .LC547
	ldr	x26, .LC549
	strb	w0, [x19, 3]
	b	.L2107
	.p2align 3
.L2101:
	ldr	w0, [x19, 56]
	add	w21, w21, 1
	cmp	w21, w0
	bhi	.L2106
.L2107:
	add	x22, x19, x21, uxtw 2
	ldr	x0, [x20, 2312]
	mov	x1, x28
	bl	HEVC_ue_v
	add	w0, w0, 1
	str	w0, [x22, 380]
	mov	x1, x27
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x22, 412]
	mov	x1, x26
	ldr	w24, [x22, 380]
	ldr	x0, [x20, 2312]
	ldr	w25, [x19, 380]
	bl	HEVC_ue_v
	str	w0, [x22, 444]
	ldrb	w0, [x19, 3]
	cbnz	w0, .L2101
	ldr	w2, [x19, 56]
	add	w21, w21, 1
	cmp	w21, w2
	bhi	.L2106
	.p2align 2
.L2183:
	add	x0, x19, x21, uxtw 2
	ldr	w1, [x19, 380]
	add	w21, w21, 1
	cmp	w21, w2
	str	w1, [x0, 380]
	ldr	w1, [x19, 412]
	str	w1, [x0, 412]
	ldr	w1, [x19, 444]
	str	w1, [x0, 444]
	bls	.L2183
.L2106:
	ldr	w0, [x19, 236]
	cmp	w24, w25
	csel	w24, w24, w25, cs
	ldr	x1, .LC551
	cmp	w24, w0
	csel	w24, w24, w0, ls
	str	w24, [x19, 236]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 136]
	bhi	.L2218
	ldr	x1, .LC555
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 144]
	bhi	.L2219
	ldr	w1, [x19, 136]
	add	w1, w1, 3
	str	w1, [x19, 140]
	add	w0, w0, w1
	str	w0, [x19, 148]
	sub	w2, w0, #4
	cmp	w2, 2
	bhi	.L2220
	ldp	w4, w5, [x19, 72]
	mov	w21, 1
	lsl	w0, w21, w0
	lsl	w1, w21, w1
	stp	w0, w0, [x19, 160]
	udiv	w3, w4, w0
	stp	w1, w0, [x19, 152]
	ldr	x1, .LC561
	udiv	w2, w5, w0
	msub	w4, w3, w0, w4
	cmp	w4, wzr
	csinc	w3, w3, w3, eq
	str	w3, [x19, 240]
	msub	w0, w2, w0, w5
	cmp	w0, wzr
	csinc	w2, w2, w2, eq
	str	w2, [x19, 244]
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 168]
	bhi	.L2221
	ldr	x1, .LC565
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 176]
	bhi	.L2222
	ldr	w2, [x19, 168]
	ldr	w3, [x19, 140]
	add	w1, w2, 2
	str	w1, [x19, 172]
	cmp	w1, w3
	bcs	.L2223
	add	w1, w0, w1
	str	w1, [x19, 180]
	cmp	w1, 5
	bhi	.L2117
	ldr	w0, [x19, 148]
	cmp	w1, w0
	bhi	.L2117
	ldr	x1, .LC573
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 200]
	ldr	w1, [x19, 148]
	ldr	w2, [x19, 172]
	sub	w1, w1, w2
	cmp	w0, w1
	bhi	.L2224
	ldr	x1, .LC577
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 208]
	ldr	w1, [x19, 148]
	ldr	w2, [x19, 172]
	sub	w1, w1, w2
	cmp	w0, w1
	bhi	.L2225
	ldr	w1, [x19, 200]
	add	w0, w0, 1
	mov	w4, 1
	str	w0, [x19, 212]
	add	w0, w1, w4
	str	w0, [x19, 204]
	str	wzr, [x20, 228]
	ldr	w2, [x19, 144]
	ldr	w1, [x19, 160]
	ldr	w0, [x19, 172]
	lsr	w1, w1, w2
	lsl	w0, w4, w0
	cmp	w1, w0
	bhi	.L2122
	b	.L2226
	.p2align 3
.L2147:
	mov	w23, w3
.L2122:
	str	w23, [x20, 228]
	add	w3, w23, 1
	ldr	w0, [x19, 172]
	ldr	w2, [x19, 144]
	ldr	w1, [x19, 160]
	add	w0, w23, w0
	lsl	w0, w4, w0
	lsr	w1, w1, w2
	cmp	w1, w0
	bhi	.L2147
.L2121:
	add	w23, w23, w2
	mov	w21, 1
	stp	w23, wzr, [x19, 216]
	str	w21, [x19, 224]
	mov	w1, w21
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	cbz	w0, .L2123
	ldr	x0, [x20, 2312]
	mov	w1, w21
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 6]
	cbnz	w0, .L2227
.L2123:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 8]
	cbnz	w0, .L2124
	ldr	w2, [x19, 76]
	cmp	w2, 64
	bls	.L2228
.L2124:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19, 116]
	cbz	w0, .L2125
	ldr	x0, [x20, 2312]
	mov	w1, 4
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x19, 120]
	mov	w1, 4
	ldr	x0, [x20, 2312]
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x19, 124]
	ldr	x1, .LC586
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 2
	str	w0, [x19, 184]
	bhi	.L2229
.L2126:
	ldr	x1, .LC590
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 2
	str	w0, [x19, 192]
	bhi	.L2230
.L2127:
	ldr	w1, [x19, 184]
	add	w1, w1, 3
	str	w1, [x19, 188]
	add	w0, w1, w0
	str	w0, [x19, 196]
	cmp	w0, 5
	bhi	.L2128
	ldr	w1, [x19, 148]
	cmp	w0, w1
	bhi	.L2128
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 9]
.L2125:
	ldr	x1, .LC596
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 64
	str	w0, [x19, 228]
	bhi	.L2130
	mov	w21, 0
	cbnz	w0, .L2182
	b	.L2136
	.p2align 3
.L2134:
	ldr	w0, [x19, 228]
	cmp	w21, w0
	bcs	.L2136
.L2182:
	uxtw	x2, w21
	mov	w3, w21
	lsl	x0, x2, 2
	mov	x1, x19
	sub	x2, x0, x2
	mov	x0, x20
	add	w21, w21, 1
	add	x2, x2, x2, lsl 4
	add	x2, x19, x2, lsl 2
	add	x2, x2, 12288
	add	x2, x2, 820
	bl	HEVC_DecShortTermRefPicSet
	cbz	w0, .L2134
	ldr	x1, .LC600
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
	.p2align 3
.L2087:
	mov	w1, 245760
	mov	w2, 983040
	b	.L2083
	.p2align 3
.L2092:
	cmp	w0, w2, lsr 1
	bhi	.L2094
	mov	w0, 12
	str	w0, [x19, 236]
	b	.L2093
	.p2align 3
.L2136:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 10]
	cbz	w0, .L2133
	ldr	x1, .LC602
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 32
	str	w0, [x19, 232]
	bhi	.L2139
	mov	w22, 0
	cbz	w0, .L2133
	.p2align 2
.L2181:
	ldr	w1, [x19, 128]
	uxtw	x21, w22
	ldr	x0, [x20, 2312]
	add	w22, w22, 1
	bl	BsGet
	add	x2, x19, x21, lsl 2
	add	x21, x19, x21
	mov	w1, 1
	str	w0, [x2, 248]
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x21, 16]
	ldr	w0, [x19, 232]
	cmp	w22, w0
	bcc	.L2181
.L2133:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 11]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 12]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 13]
	cbz	w0, .L2138
	mov	x2, x19
	add	x1, x19, 2624
	mov	x0, x20
	bl	HEVC_DecVuiParam
	cbnz	w0, .L2231
.L2138:
	ldp	w3, w4, [x19, 72]
	ldr	w1, [x19, 2652]
	ldr	w2, [x19, 2656]
	ldr	w0, [x19, 2648]
	bl	HEVCGetDar
	str	w0, [x19, 2644]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 14]
	cbnz	w0, .L2204
.L2143:
	mov	w1, 1
	strb	w1, [x19]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2144:
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 15]
.L2204:
	ldr	x0, [x20, 2312]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L2143
	ldr	x2, [x20, 2360]
	mov	w1, 1
	ldr	w2, [x2, 12]
	add	w2, w2, 3
	cmp	w2, w0
	bcc	.L2144
	b	.L2143
	.p2align 3
.L2094:
	lsl	w1, w2, 2
	sub	w2, w1, w2
	cmp	w0, w2, lsr 2
	bhi	.L2095
	mov	w0, 8
	str	w0, [x19, 236]
	b	.L2093
	.p2align 3
.L2212:
	cbz	w2, .L2085
	cmp	w2, 30
	bne	.L2080
.L2085:
	mov	w1, 9216
	mov	w2, 36864
	b	.L2083
	.p2align 3
.L2228:
	ldr	x1, .LC583
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC584
	ldr	x4, [x0]
	cbz	x4, .L2148
	ldr	w0, [x20, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, -1
	b	.L2205
	.p2align 3
.L2145:
	mov	w1, 30720
	mov	w2, 122880
	b	.L2083
	.p2align 3
.L2081:
	mov	w1, 32768
	mov	w2, 2228224
	movk	w1, 0x8, lsl 16
	b	.L2083
	.p2align 3
.L2086:
	mov	w1, 7168
	mov	w2, 28672
	movk	w1, 0x2, lsl 16
	movk	w2, 0x8, lsl 16
	b	.L2083
	.p2align 3
.L2095:
	mov	w0, 6
	str	w0, [x19, 236]
	b	.L2093
	.p2align 3
.L2089:
	mov	w1, 2228224
	mov	w2, 8912896
	b	.L2083
.L2227:
	add	x1, x19, 860
	mov	x0, x20
	bl	HEVC_DecScalingListData
	cbz	w0, .L2123
	ldr	x1, .LC581
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
	.p2align 3
.L2117:
	ldr	x1, .LC571
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2230:
	ldr	x1, .LC592
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 192]
	b	.L2127
.L2229:
	ldr	x1, .LC588
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2126
.L2226:
	mov	w23, 0
	b	.L2121
.L2210:
	ldr	x1, .LC512
	mov	w0, w23
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2148:
	mov	w0, -1
	b	.L2205
.L2217:
	ldr	x1, .LC543
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2218:
	ldr	x1, .LC553
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2221:
	ldr	x1, .LC563
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2128:
	ldr	x1, .LC594
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2224:
	ldr	x1, .LC575
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2211:
	ldr	x1, .LC516
	mov	w0, w23
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2215:
	ldr	x1, .LC535
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2220:
	ldr	x1, .LC559
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2219:
	ldr	x1, .LC557
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2216:
	ldr	x1, .LC539
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2222:
	ldr	x1, .LC567
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2223:
	ldr	x1, .LC569
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2225:
	ldr	x1, .LC579
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2130:
	ldr	x1, .LC598
	mov	w2, 64
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2213:
	ldr	x1, .LC525
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2214:
	ldr	x1, .LC531
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2139:
	ldr	x1, .LC604
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
.L2231:
	ldr	x1, .LC606
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2205
	.size	HEVC_ProcessSPS, .-HEVC_ProcessSPS
	.align	3
.LC504:
	.xword	.LC503
	.align	3
.LC506:
	.xword	.LC505
	.align	3
.LC508:
	.xword	.LC507
	.align	3
.LC510:
	.xword	.LC509
	.align	3
.LC512:
	.xword	.LC511
	.align	3
.LC514:
	.xword	.LC513
	.align	3
.LC516:
	.xword	.LC515
	.align	3
.LC518:
	.xword	.LC517
	.align	3
.LC520:
	.xword	.LC519
	.align	3
.LC521:
	.xword	.LANCHOR5
	.align	3
.LC523:
	.xword	.LC522
	.align	3
.LC525:
	.xword	.LC524
	.align	3
.LC527:
	.xword	.LC526
	.align	3
.LC529:
	.xword	.LC528
	.align	3
.LC531:
	.xword	.LC530
	.align	3
.LC533:
	.xword	.LC532
	.align	3
.LC535:
	.xword	.LC534
	.align	3
.LC537:
	.xword	.LC536
	.align	3
.LC539:
	.xword	.LC538
	.align	3
.LC541:
	.xword	.LC540
	.align	3
.LC543:
	.xword	.LC542
	.align	3
.LC545:
	.xword	.LC544
	.align	3
.LC547:
	.xword	.LC546
	.align	3
.LC549:
	.xword	.LC548
	.align	3
.LC551:
	.xword	.LC550
	.align	3
.LC553:
	.xword	.LC552
	.align	3
.LC555:
	.xword	.LC554
	.align	3
.LC557:
	.xword	.LC556
	.align	3
.LC559:
	.xword	.LC558
	.align	3
.LC561:
	.xword	.LC560
	.align	3
.LC563:
	.xword	.LC562
	.align	3
.LC565:
	.xword	.LC564
	.align	3
.LC567:
	.xword	.LC566
	.align	3
.LC569:
	.xword	.LC568
	.align	3
.LC571:
	.xword	.LC570
	.align	3
.LC573:
	.xword	.LC572
	.align	3
.LC575:
	.xword	.LC574
	.align	3
.LC577:
	.xword	.LC576
	.align	3
.LC579:
	.xword	.LC578
	.align	3
.LC581:
	.xword	.LC580
	.align	3
.LC583:
	.xword	.LC582
	.align	3
.LC584:
	.xword	g_event_report
	.align	3
.LC586:
	.xword	.LC585
	.align	3
.LC588:
	.xword	.LC587
	.align	3
.LC590:
	.xword	.LC589
	.align	3
.LC592:
	.xword	.LC591
	.align	3
.LC594:
	.xword	.LC593
	.align	3
.LC596:
	.xword	.LC595
	.align	3
.LC598:
	.xword	.LC597
	.align	3
.LC600:
	.xword	.LC599
	.align	3
.LC602:
	.xword	.LC601
	.align	3
.LC604:
	.xword	.LC603
	.align	3
.LC606:
	.xword	.LC605
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecSPS
	.type	HEVC_DecSPS, %function
HEVC_DecSPS:
	stp	x29, x30, [sp, -496]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x21, .LC607
	ldr	x26, .LC608
	mov	x2, 26164
	mov	w1, 0
	ldr	x3, [x21, 96]
	add	x22, x26, 3328
	mov	x0, x22
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 384
	mov	w1, 0
	add	x0, x29, 112
	blr	x3
	ldr	x0, [x19, 2312]
	mov	w1, 4
	bl	BsGet
	mov	w25, w0
	ldr	w2, [x19, 148]
	cmp	w2, w0
	bls	.L2243
	ldr	x0, [x19, 2312]
	mov	w1, 3
	bl	BsGet
	cmp	w0, 8
	mov	w24, w0
	bhi	.L2244
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	mov	w27, w0
	mov	w3, w24
	mov	w2, 1
	add	x1, x29, 112
	mov	x0, x19
	bl	HEVC_DecPTL
	cbnz	w0, .L2245
	ldr	x1, .LC616
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	mov	w23, w0
	ldr	w2, [x19, 152]
	cmp	w0, w2
	bcs	.L2246
	mov	w20, 26164
	ldr	x1, [x19, 2328]
	uxtb	w27, w27
	umull	x20, w0, w20
	add	x1, x1, x20
	ldrb	w0, [x1, 1]
	cbnz	w0, .L2247
	str	w25, [x1, 52]
	mov	x2, 384
	add	x1, x29, 112
	ldr	x0, [x19, 2328]
	add	x0, x0, x20
	str	w24, [x0, 56]
	ldr	x0, [x19, 2328]
	add	x0, x0, x20
	strb	w27, [x0, 2]
	ldr	x0, [x19, 2328]
	ldr	x3, [x21, 104]
	add	x0, x0, x20
	add	x0, x0, 476
	blr	x3
	ldr	x1, [x19, 2328]
	mov	x0, x19
	add	x1, x1, x20
	str	w23, [x1, 60]
	ldr	x1, [x19, 2328]
	add	x1, x1, x20
	bl	HEVC_ProcessSPS
	mov	w24, w0
	cbnz	w0, .L2248
	ldr	x1, [x19, 2328]
	mov	w3, 1
	add	x0, x19, 24576
	mov	x2, 26164
	add	x0, x0, 3576
	strb	w3, [x1, x20]
	ldr	x1, [x19, 2328]
	add	x1, x1, x20
	strb	w3, [x1, 1]
	ldr	x1, [x19, 2328]
	ldr	x3, [x21, 104]
	add	x1, x1, x20
	blr	x3
.L2234:
	mov	w0, w24
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 496
	ret
	.p2align 3
.L2247:
	ldr	x3, [x21, 104]
	add	x1, x29, 112
	mov	x2, 384
	strb	w27, [x22, 2]
	stp	w25, w24, [x22, 52]
	add	x0, x26, 3804
	blr	x3
	str	w23, [x22, 60]
	mov	x1, x22
	mov	x0, x19
	bl	HEVC_ProcessSPS
	mov	w24, w0
	cbnz	w0, .L2249
	ldr	x0, [x19, 2328]
	mov	w2, 1
	ldr	x3, [x21, 112]
	mov	x1, x22
	strb	w2, [x26, 3328]
	add	x0, x0, x20
	strb	w2, [x22, 1]
	mov	x2, 26164
	blr	x3
	ldr	x1, [x19, 2328]
	add	x0, x19, 24576
	ldr	x3, [x21, 104]
	mov	x2, 26164
	add	x1, x1, x20
	add	x0, x0, 3576
	blr	x3
	ldr	x27, [sp, 80]
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 496
	ret
	.p2align 3
.L2246:
	ldr	x1, .LC618
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 152]
	cmp	w23, w1
	bcc	.L2242
	ldr	x0, .LC619
	ldr	x4, [x0]
	cbz	x4, .L2242
	ldr	w0, [x19, 244]
	add	x2, x29, 104
	stp	w23, w1, [x29, 104]
	mov	w3, 8
	mov	w1, 109
	mov	w24, -1
	blr	x4
	ldr	x27, [sp, 80]
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 496
	ret
	.p2align 3
.L2242:
	mov	w24, -1
	b	.L2234
	.p2align 3
.L2243:
	ldr	x1, .LC610
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L2234
	.p2align 3
.L2244:
	ldr	x1, .LC612
	mov	w2, 8
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L2234
	.p2align 3
.L2245:
	ldr	x1, .LC614
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L2234
	.p2align 3
.L2248:
	ldr	x1, .LC623
	mov	w2, w23
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	ldr	x0, [x19, 2328]
	mov	w1, 1
	strb	w1, [x0, x20]
	ldr	x0, [x19, 2328]
	add	x20, x0, x20
	strb	wzr, [x20, 1]
	b	.L2234
	.p2align 3
.L2249:
	ldr	x1, [x19, 2328]
	add	x0, x19, 24576
	ldr	x3, [x21, 104]
	mov	x2, 26164
	add	x1, x1, x20
	add	x0, x0, 3576
	mov	w24, -1
	blr	x3
	ldr	x1, .LC621
	mov	w2, w23
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2234
	.size	HEVC_DecSPS, .-HEVC_DecSPS
	.align	3
.LC607:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC608:
	.xword	.LANCHOR6
	.align	3
.LC610:
	.xword	.LC609
	.align	3
.LC612:
	.xword	.LC611
	.align	3
.LC614:
	.xword	.LC613
	.align	3
.LC616:
	.xword	.LC615
	.align	3
.LC618:
	.xword	.LC617
	.align	3
.LC619:
	.xword	g_event_report
	.align	3
.LC621:
	.xword	.LC620
	.align	3
.LC623:
	.xword	.LC622
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ProcessPPS
	.type	HEVC_ProcessPPS, %function
HEVC_ProcessPPS:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	mov	w22, 1
	bl	_mcount
	mov	w0, 1
	strb	w0, [x19, 16]
	stp	w22, w22, [x19, 68]
	ldr	x1, .LC626
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	str	w0, [x19, 32]
	tbnz	w0, #31, .L2251
	ldr	w1, [x20, 152]
	cmp	w0, w1
	bge	.L2251
	ldr	x1, [x20, 2328]
	mov	w21, 26164
	smaddl	x21, w0, w21, x1
	ldrb	w1, [x21, 1]
	cbz	w1, .L2359
	ldr	x0, [x20, 2312]
	mov	w1, w22
	bl	BsGet
	strb	w0, [x19, 2]
	mov	w1, w22
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 3
	ldr	x0, [x20, 2312]
	bl	BsGet
	str	w0, [x19, 36]
	mov	w1, w22
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 3]
	mov	w1, w22
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 5]
	ldr	x1, .LC632
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	add	w0, w0, 1
	cmp	w0, 15
	str	w0, [x19, 40]
	bhi	.L2360
	ldr	x1, .LC636
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	add	w0, w0, 1
	cmp	w0, 15
	str	w0, [x19, 44]
	bhi	.L2361
	ldr	x1, .LC640
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w2, w0, 26
	str	w2, [x19, 48]
	mov	w0, 51
	ldr	w3, [x21, 104]
	neg	w1, w3
	cmp	w2, w1
	ccmp	w2, w0, 0, ge
	bgt	.L2362
	ldr	x0, [x20, 2312]
	mov	w1, w22
	bl	BsGet
	strb	w0, [x19, 6]
	mov	w1, w22
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, w22
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 8]
	cbnz	w0, .L2363
	mov	w0, 0
	str	wzr, [x19, 52]
.L2260:
	str	w0, [x19, 56]
	ldr	x1, .LC648
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x19, 60]
	bhi	.L2364
	ldr	x1, .LC652
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x19, 64]
	bhi	.L2365
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 9]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 10]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 11]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 12]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 13]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 14]
	ldrb	w1, [x19, 13]
	cbnz	w1, .L2366
	ldr	w0, [x21, 240]
	strh	w0, [x19, 176]
	ldr	w0, [x21, 244]
	strh	w0, [x19, 216]
.L2297:
	ldr	w3, [x19, 68]
	cmp	w3, wzr
	ble	.L2302
.L2296:
	ldr	w4, [x21, 160]
	add	x0, x19, 176
	mov	w1, 0
	.p2align 2
.L2301:
	ldrsh	w2, [x0]
	add	w1, w1, 1
	add	x0, x0, 2
	mul	w2, w2, w4
	cmp	w2, 64
	bhi	.L2300
	ldr	w5, [x21, 72]
	cmp	w5, 64
	bhi	.L2367
.L2300:
	cmp	w1, w3
	blt	.L2301
.L2302:
	ldr	w1, [x19, 72]
	cmp	w1, wzr
	ble	.L2299
	ldrsh	w2, [x19, 216]
	ldr	w3, [x21, 160]
	mul	w2, w2, w3
	cmp	w2, 63
	bls	.L2303
	sub	w1, w1, #1
	mov	x0, x19
	add	x1, x19, x1, lsl 1
	b	.L2305
	.p2align 3
.L2306:
	ldrsh	w2, [x0, 218]
	add	x0, x0, 2
	mul	w2, w2, w3
	cmp	w2, 63
	bls	.L2303
.L2305:
	cmp	x1, x0
	bne	.L2306
.L2299:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 17]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 18]
	cbnz	w0, .L2368
.L2308:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 21]
	cbz	w0, .L2311
	add	x1, x19, 260
	mov	x0, x20
	bl	HEVC_DecScalingListData
	cbnz	w0, .L2369
.L2311:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 22]
	ldr	x1, .LC695
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 2
	cmp	w2, 1
	str	w2, [x19, 84]
	ble	.L2312
	ldr	w0, [x21, 148]
	cmp	w2, w0
	bhi	.L2312
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 23]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 24]
	cbz	w0, .L2315
	ldr	x0, [x20, 2312]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L2315
	.p2align 2
.L2370:
	ldr	x2, [x20, 2360]
	mov	w1, 1
	ldr	w2, [x2, 12]
	add	w2, w2, 3
	cmp	w2, w0
	bcs	.L2315
	ldr	x0, [x20, 2312]
	bl	BsGet
	strb	w0, [x19, 25]
	ldr	x0, [x20, 2312]
	bl	BsResidBits
	cmp	w0, wzr
	bgt	.L2370
.L2315:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2363:
	ldr	x1, .LC644
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 52]
	bls	.L2260
	ldr	x1, .LC646
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
	.p2align 3
.L2366:
	ldr	x1, .LC656
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w0, 19
	str	w2, [x19, 68]
	bhi	.L2371
	ldr	x1, .LC660
	ldr	x0, [x20, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w0, 21
	str	w2, [x19, 72]
	bhi	.L2372
	ldr	w3, [x19, 68]
	cmp	w3, 10
	bgt	.L2373
	cmp	w2, 11
	ble	.L2268
	ldr	x1, .LC667
	mov	w3, w2
	mov	w2, 11
.L2356:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC665
	ldr	x4, [x0]
	cbz	x4, .L2351
.L2353:
	ldr	w0, [x20, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, -1
.L2347:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2303:
	ldr	x1, .LC683
.L2355:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC665
	ldr	x4, [x0]
	cbnz	x4, .L2353
.L2351:
	mov	w0, -1
.L2378:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2367:
	ldr	x1, .LC681
	b	.L2355
	.p2align 3
.L2362:
	ldr	x1, .LC642
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2268:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 15]
	cbnz	w0, .L2374
	ldr	w0, [x19, 68]
	sub	w1, w0, #1
	cmp	w1, wzr
	ble	.L2273
	ldr	x23, .LC669
	mov	w22, 0
	b	.L2277
	.p2align 3
.L2376:
	cmp	w3, w4
	bhi	.L2275
	ldr	w0, [x19, 68]
	add	w22, w22, 1
	sub	w1, w0, #1
	cmp	w1, w22
	ble	.L2375
.L2277:
	ldr	x0, [x20, 2312]
	mov	x1, x23
	bl	HEVC_ue_v
	add	w0, w0, 1
	add	x1, x19, x22, sxtw 1
	sxth	w3, w0
	strh	w3, [x1, 92]
	ldr	w4, [x21, 240]
	tbz	w3, #31, .L2376
.L2275:
	ldr	x1, .LC671
	mov	w2, w22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2373:
	ldr	x1, .LC664
	mov	w2, 10
	b	.L2356
.L2368:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 19]
	mov	w1, 1
	ldr	x0, [x20, 2312]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 20]
	cbnz	w0, .L2308
	ldr	x1, .LC685
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 76]
	bhi	.L2377
.L2310:
	ldr	x1, .LC689
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 80]
	bls	.L2308
	ldr	x1, .LC691
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x19, 80]
	b	.L2308
.L2374:
	ldr	w0, [x19, 68]
.L2279:
	cmp	w0, wzr
	add	x5, x19, 176
	mov	w3, 0
	ble	.L2272
	.p2align 2
.L2331:
	add	w3, w3, 1
	ldr	w4, [x21, 240]
	sub	w2, w3, #1
	cmp	w3, w0
	mul	w1, w3, w4
	mul	w2, w2, w4
	udiv	w1, w1, w0
	udiv	w2, w2, w0
	sub	w1, w1, w2
	strh	w1, [x5], 2
	bne	.L2331
.L2272:
	ldr	w4, [x19, 72]
	add	x6, x19, 216
	mov	w3, 0
	cmp	w4, wzr
	ble	.L2287
	.p2align 2
.L2330:
	add	w3, w3, 1
	ldr	w5, [x21, 244]
	sub	w2, w3, #1
	cmp	w3, w4
	mul	w1, w3, w5
	mul	w2, w2, w5
	udiv	w1, w1, w4
	udiv	w2, w2, w4
	sub	w1, w1, w2
	strh	w1, [x6], 2
	bne	.L2330
.L2287:
	cmp	w0, 1
	bne	.L2295
	ldr	w3, [x19, 72]
	cmp	w3, 1
	beq	.L2296
.L2295:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 16]
	b	.L2297
.L2251:
	ldr	x1, .LC628
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2359:
	ldr	x1, .LC630
	mov	w2, w0
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2360:
	ldr	x1, .LC634
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2361:
	ldr	x1, .LC638
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2364:
	ldr	x1, .LC650
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2365:
	ldr	x1, .LC654
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2312:
	ldr	x1, .LC697
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2378
.L2375:
	ldr	w1, [x19, 72]
	sub	w1, w1, #1
	cmp	w1, wzr
	ble	.L2285
.L2278:
	ldr	x23, .LC673
	mov	w22, 0
	b	.L2284
	.p2align 3
.L2380:
	cmp	w3, w4
	bhi	.L2282
	ldr	w0, [x19, 72]
	add	w22, w22, 1
	sub	w0, w0, #1
	cmp	w0, w22
	ble	.L2379
.L2284:
	ldr	x0, [x20, 2312]
	mov	x1, x23
	bl	HEVC_ue_v
	add	w0, w0, 1
	add	x1, x19, x22, sxtw 1
	sxth	w3, w0
	strh	w3, [x1, 132]
	ldr	w4, [x21, 244]
	tbz	w3, #31, .L2380
.L2282:
	ldr	x1, .LC675
	mov	w2, w22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2371:
	ldr	x1, .LC658
	mov	w3, 20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2377:
	ldr	x1, .LC687
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x19, 76]
	b	.L2310
.L2379:
	ldr	w0, [x19, 68]
.L2285:
	ldrb	w1, [x19, 15]
	cbnz	w1, .L2279
	add	x5, x19, x0, sxtw 1
	sub	w2, w0, #1
	ldrsh	w3, [x21, 240]
	cmp	w2, wzr
	strh	w3, [x5, 174]
	ble	.L2290
	sub	w6, w0, #2
	add	x4, x19, 92
	add	x6, x6, 47
	add	x6, x19, x6, lsl 1
	.p2align 2
.L2291:
	ldrsh	w3, [x4]
	add	x4, x4, 2
	strh	w3, [x4, 82]
	cmp	x4, x6
	ldrh	w1, [x5, 174]
	sub	w3, w1, w3
	sxth	w3, w3
	strh	w3, [x5, 174]
	bne	.L2291
.L2290:
	cmp	w3, wzr
	ble	.L2381
	ldr	w1, [x19, 72]
	ldrsh	w3, [x21, 244]
	sub	w2, w1, #1
	add	x5, x19, x1, sxtw 1
	cmp	w2, wzr
	strh	w3, [x5, 214]
	ble	.L2293
	sub	w6, w1, #2
	add	x4, x19, 132
	add	x6, x6, 67
	add	x6, x19, x6, lsl 1
	.p2align 2
.L2294:
	ldrsh	w3, [x4]
	add	x4, x4, 2
	strh	w3, [x4, 82]
	cmp	x6, x4
	ldrh	w1, [x5, 214]
	sub	w3, w1, w3
	sxth	w3, w3
	strh	w3, [x5, 214]
	bne	.L2294
.L2293:
	cmp	w3, wzr
	bgt	.L2287
	ldr	x1, .LC679
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
	.p2align 3
.L2372:
	ldr	x1, .LC662
	mov	w3, 22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2369:
	ldr	x1, .LC693
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
.L2273:
	ldr	w1, [x19, 72]
	sub	w1, w1, #1
	cmp	w1, wzr
	bgt	.L2278
	add	x1, x19, x0, sxtw 1
	ldrsh	w3, [x21, 240]
	sub	w2, w0, #1
	strh	w3, [x1, 174]
	b	.L2290
.L2381:
	ldr	x1, .LC677
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2347
	.size	HEVC_ProcessPPS, .-HEVC_ProcessPPS
	.align	3
.LC626:
	.xword	.LC625
	.align	3
.LC628:
	.xword	.LC627
	.align	3
.LC630:
	.xword	.LC629
	.align	3
.LC632:
	.xword	.LC631
	.align	3
.LC634:
	.xword	.LC633
	.align	3
.LC636:
	.xword	.LC635
	.align	3
.LC638:
	.xword	.LC637
	.align	3
.LC640:
	.xword	.LC639
	.align	3
.LC642:
	.xword	.LC641
	.align	3
.LC644:
	.xword	.LC643
	.align	3
.LC646:
	.xword	.LC645
	.align	3
.LC648:
	.xword	.LC647
	.align	3
.LC650:
	.xword	.LC649
	.align	3
.LC652:
	.xword	.LC651
	.align	3
.LC654:
	.xword	.LC653
	.align	3
.LC656:
	.xword	.LC655
	.align	3
.LC658:
	.xword	.LC657
	.align	3
.LC660:
	.xword	.LC659
	.align	3
.LC662:
	.xword	.LC661
	.align	3
.LC664:
	.xword	.LC663
	.align	3
.LC665:
	.xword	g_event_report
	.align	3
.LC667:
	.xword	.LC666
	.align	3
.LC669:
	.xword	.LC668
	.align	3
.LC671:
	.xword	.LC670
	.align	3
.LC673:
	.xword	.LC672
	.align	3
.LC675:
	.xword	.LC674
	.align	3
.LC677:
	.xword	.LC676
	.align	3
.LC679:
	.xword	.LC678
	.align	3
.LC681:
	.xword	.LC680
	.align	3
.LC683:
	.xword	.LC682
	.align	3
.LC685:
	.xword	.LC684
	.align	3
.LC687:
	.xword	.LC686
	.align	3
.LC689:
	.xword	.LC688
	.align	3
.LC691:
	.xword	.LC690
	.align	3
.LC693:
	.xword	.LC692
	.align	3
.LC695:
	.xword	.LC694
	.align	3
.LC697:
	.xword	.LC696
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecPPS
	.type	HEVC_DecPPS, %function
HEVC_DecPPS:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC699
	ldr	x0, [x19, 2312]
	bl	HEVC_ue_v
	mov	w21, w0
	ldr	w3, [x19, 156]
	cmp	w0, w3
	bcs	.L2388
	mov	w20, 2024
	umull	x20, w0, w20
	ldr	x0, [x19, 2336]
	add	x1, x0, x20
	ldrb	w0, [x0, x20]
	cbnz	w0, .L2389
	str	w21, [x1, 28]
	mov	x0, x19
	ldr	x1, [x19, 2336]
	add	x1, x1, x20
	bl	HEVC_ProcessPPS
	mov	w22, w0
	cbnz	w0, .L2390
	ldr	x0, [x19, 2336]
	mov	w1, 1
	add	x0, x0, x20
	strb	w1, [x0, 1]
	ldr	x0, [x19, 2336]
	strb	w1, [x0, x20]
.L2384:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2389:
	ldr	x24, .LC702
	mov	x0, x19
	add	x23, x24, 3328
	mov	x1, x23
	str	w21, [x23, 28]
	bl	HEVC_ProcessPPS
	mov	w22, w0
	cbnz	w0, .L2391
	ldr	x3, .LC705
	mov	w1, 1
	ldr	x0, [x19, 2336]
	mov	x2, 2024
	strb	w1, [x23, 1]
	ldr	x3, [x3, 112]
	add	x0, x0, x20
	strb	w1, [x24, 3328]
	mov	x1, x23
	blr	x3
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2391:
	ldr	x1, .LC704
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2384
	.p2align 3
.L2388:
	ldr	x1, .LC701
	mov	w2, w0
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2384
.L2390:
	ldr	x1, .LC704
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x0, [x19, 2336]
	mov	w1, 1
	add	x0, x0, x20
	strb	w1, [x0, 1]
	ldr	x0, [x19, 2336]
	strb	wzr, [x0, x20]
	b	.L2384
	.size	HEVC_DecPPS, .-HEVC_DecPPS
	.align	3
.LC699:
	.xword	.LC698
	.align	3
.LC701:
	.xword	.LC700
	.align	3
.LC702:
	.xword	.LANCHOR6
	.align	3
.LC704:
	.xword	.LC703
	.align	3
.LC705:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecPredWeightTable
	.type	HEVC_DecPredWeightTable, %function
HEVC_DecPredWeightTable:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	add	x26, x20, 53248
	bl	_mcount
	ldr	x2, [x20, 2336]
	mov	w4, 2024
	ldr	w1, [x26, 1820]
	ldr	w0, [x26, 1836]
	cmp	w1, 1
	smaddl	x2, w0, w4, x2
	beq	.L2465
	cbnz	w1, .L2396
	ldrb	w0, [x2, 11]
	cbz	w0, .L2396
.L2394:
	ldr	x1, .LC708
	add	x23, x26, 3352
	ldr	x0, [x20, 2312]
	ldr	w21, [x2, 32]
	ldr	x19, [x20, 2328]
	bl	HEVC_ue_v
	cmp	w0, 7
	str	w0, [x26, 3352]
	bhi	.L2466
	mov	w22, 26164
	smaddl	x22, w21, w22, x19
	ldr	w1, [x22, 64]
	cbnz	w1, .L2467
	ldr	w1, [x23, 4]
.L2400:
	add	w0, w0, w1
	str	w0, [x26, 4384]
	cmp	w0, 7
	bhi	.L2401
	ldr	w0, [x26, 1928]
	mov	w19, 0
	add	x21, x26, 3352
	cbz	w0, .L2409
	.p2align 2
.L2451:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 8]
	ldr	w0, [x26, 1928]
	cmp	w0, w19
	bhi	.L2451
	ldr	w1, [x22, 64]
	cbz	w1, .L2405
	mov	w19, 0
	add	x21, x26, 3352
	cbz	w0, .L2409
	.p2align 2
.L2450:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 72]
	ldr	w0, [x26, 1928]
	cmp	w0, w19
	bhi	.L2450
.L2405:
	mov	w27, 0
	mov	w21, 1
	cbnz	w0, .L2449
	b	.L2409
	.p2align 3
.L2411:
	add	x23, x23, 57344
	ldr	w0, [x26, 3352]
	lsl	w0, w21, w0
	str	w0, [x23, 292]
	str	wzr, [x24, 3552]
.L2413:
	ldr	w0, [x22, 64]
	cbz	w0, .L2415
	ldr	w0, [x24, 3424]
	cbnz	w0, .L2416
	lsl	x0, x19, 3
	add	x19, x26, 3352
	add	x2, x20, x0
	ldr	w1, [x26, 4384]
	add	x2, x2, 57344
	add	x19, x19, x0
	lsl	w0, w21, w1
	str	wzr, [x2, 484]
	str	w0, [x2, 356]
	str	w0, [x19, 1104]
	str	wzr, [x19, 1232]
.L2415:
	ldr	w0, [x26, 1928]
	add	w27, w27, 1
	cmp	w0, w27
	bls	.L2409
.L2449:
	uxtw	x19, w27
	add	x23, x20, x19, lsl 2
	add	x24, x23, 53248
	ldr	w0, [x24, 3360]
	cbz	w0, .L2411
	ldr	x1, .LC718
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 128
	cmp	w1, 255
	str	w0, [x24, 3488]
	bhi	.L2468
	ldr	w2, [x26, 3352]
	add	x23, x23, 57344
	ldr	x1, .LC722
	lsl	w2, w21, w2
	add	w0, w2, w0
	str	w0, [x23, 292]
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 128
	cmp	w1, 255
	str	w0, [x24, 3552]
	bls	.L2413
	ldr	x1, .LC724
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
	.p2align 3
.L2465:
	ldrb	w0, [x2, 10]
	cbnz	w0, .L2394
.L2396:
	mov	w0, 0
.L2395:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2416:
	add	x19, x19, 4096
	ldr	x1, .LC726
	add	x5, x19, 3012
	ldr	x0, [x20, 2312]
	add	x19, x19, 3013
	mov	w25, 128
	add	x28, x20, x5, lsl 3
	mov	w24, 127
	bl	HEVC_se_v
	add	w2, w0, 128
	cmp	w2, 255
	mov	w23, -128
	str	w0, [x28]
	add	x19, x20, x19, lsl 3
	ldr	x1, .LC730
	bhi	.L2469
.L2417:
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 512
	cmp	w1, 1023
	str	w0, [x28, 128]
	bhi	.L2470
	ldr	w6, [x26, 4384]
	add	x28, x28, 4
	ldr	w2, [x28, -4]
	lsl	w1, w21, w6
	add	w1, w1, w2
	str	w1, [x28, 832]
	lsl	w1, w1, 7
	asr	w1, w1, w6
	sub	w1, w25, w1
	add	w0, w1, w0
	cmp	w0, 127
	csel	w0, w0, w24, le
	cmn	w0, #128
	csel	w0, w0, w23, ge
	cmp	x28, x19
	str	w0, [x28, 960]
	beq	.L2415
	ldr	x1, .LC726
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w2, w0, 128
	cmp	w2, 255
	str	w0, [x28]
	ldr	x1, .LC730
	bls	.L2417
.L2469:
	ldr	x1, .LC728
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2409:
	ldr	w0, [x26, 1820]
	cbnz	w0, .L2396
	ldr	w0, [x26, 1932]
	cbz	w0, .L2396
	mov	w19, 0
	add	x21, x26, 3352
	.p2align 2
.L2423:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 520]
	ldr	w0, [x26, 1932]
	cmp	w0, w19
	bhi	.L2423
	ldr	w1, [x22, 64]
	cbnz	w1, .L2471
.L2424:
	mov	w28, 0
	mov	w21, 1
	cbnz	w0, .L2447
	b	.L2396
	.p2align 3
.L2428:
	add	x23, x23, 57344
	ldr	w0, [x26, 3352]
	lsl	w0, w21, w0
	str	w0, [x23, 612]
	str	wzr, [x24, 4064]
.L2430:
	ldr	w0, [x22, 64]
	cbz	w0, .L2432
	ldr	w0, [x24, 3936]
	cbnz	w0, .L2433
	lsl	x19, x19, 3
	add	x1, x26, 3352
	add	x1, x1, x19
	add	x19, x20, x19
	add	x0, x19, 57344
	ldr	w2, [x26, 4384]
	lsl	w2, w21, w2
	str	wzr, [x0, 804]
	str	w2, [x0, 676]
	str	w2, [x1, 1424]
	str	wzr, [x1, 1552]
.L2432:
	ldr	w0, [x26, 1932]
	add	w28, w28, 1
	cmp	w0, w28
	bls	.L2396
.L2447:
	uxtw	x19, w28
	add	x23, x20, x19, lsl 2
	add	x24, x23, 53248
	ldr	w0, [x24, 3872]
	cbz	w0, .L2428
	ldr	x1, .LC734
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 128
	cmp	w1, 255
	str	w0, [x24, 4000]
	bhi	.L2472
	ldr	w1, [x26, 3352]
	add	x23, x23, 57344
	lsl	w1, w21, w1
	add	w0, w1, w0
	str	w0, [x23, 612]
	ldr	x1, .LC738
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	str	w0, [x24, 4064]
	add	w0, w0, 128
	cmp	w0, 255
	bls	.L2430
	ldr	x1, .LC740
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
	.p2align 3
.L2433:
	add	x19, x19, 4096
	mov	w25, 128
	add	x5, x19, 3076
	add	x19, x19, 3077
	mov	w24, 127
	mov	w23, -128
	add	x27, x20, x5, lsl 3
	add	x19, x20, x19, lsl 3
.L2436:
	ldr	x1, .LC742
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	str	w0, [x27]
	add	w0, w0, 128
	ldr	x1, .LC746
	cmp	w0, 255
	bhi	.L2473
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	add	w1, w0, 512
	cmp	w1, 1023
	str	w0, [x27, 128]
	bhi	.L2474
	ldr	w6, [x26, 4384]
	add	x27, x27, 4
	ldr	w2, [x27, -4]
	lsl	w1, w21, w6
	add	w1, w1, w2
	str	w1, [x27, 640]
	lsl	w1, w1, 7
	asr	w1, w1, w6
	sub	w1, w25, w1
	add	w0, w1, w0
	cmp	w0, 127
	csel	w0, w0, w24, le
	cmn	w0, #128
	csel	w0, w0, w23, ge
	cmp	x19, x27
	str	w0, [x27, 768]
	bne	.L2436
	b	.L2432
	.p2align 3
.L2467:
	ldr	x1, .LC712
	ldr	x0, [x20, 2312]
	bl	HEVC_se_v
	mov	w1, w0
	add	w0, w0, 7
	str	w1, [x23, 4]
	cmp	w0, 14
	bhi	.L2399
	ldr	w0, [x26, 3352]
	b	.L2400
	.p2align 3
.L2470:
	ldr	x1, .LC732
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
.L2471:
	mov	w19, 0
	add	x21, x26, 3352
	cbz	w0, .L2396
	.p2align 2
.L2448:
	ldr	x0, [x20, 2312]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 584]
	ldr	w0, [x26, 1932]
	cmp	w0, w19
	bhi	.L2448
	b	.L2424
.L2468:
	ldr	x1, .LC720
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
.L2473:
	ldr	x1, .LC744
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
.L2474:
	ldr	x1, .LC748
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
.L2472:
	ldr	x1, .LC736
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
.L2401:
	ldr	x1, .LC716
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
.L2466:
	ldr	x1, .LC710
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
.L2399:
	ldr	x1, .LC714
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2395
	.size	HEVC_DecPredWeightTable, .-HEVC_DecPredWeightTable
	.align	3
.LC708:
	.xword	.LC707
	.align	3
.LC710:
	.xword	.LC709
	.align	3
.LC712:
	.xword	.LC711
	.align	3
.LC714:
	.xword	.LC713
	.align	3
.LC716:
	.xword	.LC715
	.align	3
.LC718:
	.xword	.LC717
	.align	3
.LC720:
	.xword	.LC719
	.align	3
.LC722:
	.xword	.LC721
	.align	3
.LC724:
	.xword	.LC723
	.align	3
.LC726:
	.xword	.LC725
	.align	3
.LC728:
	.xword	.LC727
	.align	3
.LC730:
	.xword	.LC729
	.align	3
.LC732:
	.xword	.LC731
	.align	3
.LC734:
	.xword	.LC733
	.align	3
.LC736:
	.xword	.LC735
	.align	3
.LC738:
	.xword	.LC737
	.align	3
.LC740:
	.xword	.LC739
	.align	3
.LC742:
	.xword	.LC741
	.align	3
.LC744:
	.xword	.LC743
	.align	3
.LC746:
	.xword	.LC745
	.align	3
.LC748:
	.xword	.LC747
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecSliceSegmentHeader
	.type	HEVC_DecSliceSegmentHeader, %function
HEVC_DecSliceSegmentHeader:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x28, x0
	mov	x0, x30
	add	x27, x28, 53248
	bl	_mcount
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	ldr	w23, [x27, 1832]
	strb	w0, [x27, 1761]
	sub	w0, w23, #16
	str	w0, [x29, 132]
	cmp	w0, 5
	bls	.L2740
.L2476:
	ldr	x1, .LC750
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	str	w0, [x27, 1836]
	ldr	w4, [x28, 156]
	tbnz	w0, #31, .L2478
	cmp	w0, w4
	bge	.L2478
	mov	x0, x28
	bl	HEVC_VpsSpsPpsCheck
	sxtw	x22, w0
	cbnz	w22, .L2741
	ldr	x0, [x28, 2336]
	mov	w3, 2024
	ldr	w20, [x27, 1836]
	mov	w2, 26164
	ldr	x1, [x28, 2328]
	smaddl	x20, w20, w3, x0
	ldr	w21, [x20, 32]
	ldrb	w0, [x20, 2]
	smaddl	x21, w21, w2, x1
	cbnz	w0, .L2742
.L2484:
	strb	wzr, [x27, 1763]
.L2485:
	ldp	w2, w3, [x21, 160]
	cbz	w2, .L2487
	cbz	w3, .L2487
	ldr	w0, [x21, 72]
	mov	w1, 0
	ldr	w24, [x21, 216]
	add	w0, w2, w0
	sub	w0, w0, #1
	lsl	w24, w24, 1
	udiv	w2, w0, w2
	ldr	w0, [x21, 76]
	add	w0, w3, w0
	sub	w0, w0, #1
	udiv	w19, w0, w3
	mul	w19, w2, w19
	mov	w2, 1
	cmp	w19, 1
	lsl	w24, w2, w24
	ble	.L2489
	.p2align 2
.L2490:
	add	w1, w1, 1
	lsl	w0, w2, w1
	cmp	w19, w0
	bgt	.L2490
.L2489:
	ldrb	w2, [x27, 1761]
	mov	w0, 0
	cbz	w2, .L2743
.L2491:
	mul	w19, w19, w24
	str	w0, [x27, 1912]
	str	w19, [x27, 1916]
	ldrb	w0, [x27, 1763]
	ldr	w1, [x28, 188]
	str	w1, [x27, 1808]
	cbz	w0, .L2744
.L2494:
	ldr	w0, [x20, 12]
	tst	w0, 16776960
	bne	.L2745
	str	wzr, [x27, 1900]
.L2600:
	ldrb	w0, [x20, 23]
	cbnz	w0, .L2604
.L2610:
	ldr	w0, [x27, 1912]
	cbnz	w0, .L2746
	ldrb	w0, [x28, 1]
	cbz	w0, .L2612
	ldr	w0, [x29, 132]
	cmp	w0, 5
	bhi	.L2613
	mov	w0, 1
	strb	w0, [x28, 3]
	ldr	w0, [x27, 1808]
	cmp	w0, wzr
	ldr	w0, [x29, 132]
	ccmp	w0, 5, 2, ne
	bhi	.L2613
.L2757:
	ldr	w0, [x27, 1832]
	cmp	w0, 21
	beq	.L2747
	.p2align 2
.L2613:
	strb	wzr, [x28, 2]
.L2616:
	mov	x0, x28
	bl	HEVC_IsNewPic
	str	w0, [x27, 3348]
	mov	x0, x28
	bl	HEVC_ReadByteAlignment
	cmp	w0, wzr
	csetm	w0, ne
.L2722:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2742:
	ldrb	w0, [x27, 1761]
	cbnz	w0, .L2484
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1763]
	b	.L2485
	.p2align 3
.L2744:
	ldr	w0, [x20, 36]
	cbz	w0, .L2499
	ldr	x24, .LC763
	mov	w19, 0
	.p2align 2
.L2498:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	add	w19, w19, w1
	bl	BsGet
	mov	w2, w0
	mov	x1, x24
	mov	w0, 19
	bl	dprint_vfmw
	ldr	w0, [x20, 36]
	cmp	w0, w19
	bhi	.L2498
.L2499:
	ldr	x1, .LC761
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	cmp	w0, 2
	str	w0, [x27, 1820]
	bhi	.L2748
	ldrb	w0, [x20, 4]
	cbnz	w0, .L2749
	mov	w0, 1
	strb	w0, [x27, 1760]
.L2501:
	sub	w23, w23, #19
	cmp	w23, 1
	bls	.L2502
	ldr	w1, [x21, 132]
	ldr	x0, [x28, 2312]
	bl	BsGet
	str	w0, [x27, 1848]
	mov	w4, w0
	mov	w2, 1
	ldr	w1, [x21, 128]
	ldr	w0, [x28, 184]
	lsl	w1, w2, w1
	mov	w3, w1
	udiv	w1, w0, w1
	stp	w3, w4, [x29, 112]
	msub	w1, w1, w3, w0
	cmp	w1, w4
	sub	w0, w0, w1
	bhi	.L2750
.L2503:
	ldr	w2, [x29, 116]
	cmp	w1, w2
	bcs	.L2504
	sub	w1, w2, w1
	ldr	w2, [x29, 112]
	cmp	w1, w2, lsr 1
	sub	w1, w0, w2
	csel	w0, w1, w0, hi
.L2504:
	ldr	w1, [x27, 1832]
	sub	w1, w1, #16
	cmp	w1, 2
	ldr	w1, [x29, 116]
	csel	w0, w0, wzr, hi
	add	w3, w0, w1
	str	w3, [x27, 1808]
	ldr	w2, [x28, 212]
	cmp	w3, w2
	bge	.L2506
	ldrb	w0, [x27, 1761]
	cmp	w0, 1
	beq	.L2751
.L2506:
	str	w3, [x28, 188]
	ldr	w0, [x27, 1828]
	cbnz	w0, .L2507
	ldr	w0, [x27, 1832]
	cmp	w0, 15
	bhi	.L2508
	mov	w1, 1
	tbz	x0, 0, .L2508
.L2509:
	sub	w0, w0, #7
	tst	w0, -3
	cset	w0, ne
	tst	w1, w0
	beq	.L2507
	str	w3, [x27, 1812]
	str	w3, [x28, 184]
	.p2align 2
.L2507:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1764]
	cbz	w0, .L2510
	ldr	w2, [x21, 228]
	mov	w1, 0
	mov	w3, 1
	cmp	w2, 1
	bls	.L2512
	.p2align 2
.L2687:
	add	w1, w1, 1
	lsl	w0, w3, w1
	cmp	w0, w2
	bcc	.L2687
	cbnz	w1, .L2752
.L2512:
	str	wzr, [x27, 1852]
.L2517:
	add	x0, x28, 57344
	str	x0, [x29, 104]
	add	x3, x0, 932
	lsl	x0, x22, 2
	sub	x22, x0, x22
	mov	x2, 204
	mov	x0, x3
	add	x22, x22, x22, lsl 4
	add	x1, x21, x22, lsl 2
	add	x1, x1, 12288
	add	x1, x1, 820
	bl	memcpy
	str	x0, [x29, 120]
.L2513:
	ldrb	w0, [x21, 10]
	cbz	w0, .L2518
	ldr	x0, [x29, 104]
	ldr	w1, [x21, 232]
	add	x0, x0, 932
	ldr	w2, [x0, 16]
	ldr	w0, [x0, 20]
	add	w0, w2, w0
	str	w0, [x29, 100]
	cbnz	w1, .L2519
.L2522:
	mov	w23, 0
.L2520:
	ldr	x1, .LC777
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	str	w0, [x27, 1860]
	ldr	x1, [x29, 104]
	add	x24, x1, 932
	ldr	w1, [x27, 1856]
	add	w4, w0, w1
	ldp	w2, w3, [x24, 16]
	str	w4, [x24, 32]
	add	w0, w2, w3
	add	w0, w0, w4
	cmp	w0, 16
	bhi	.L2753
	ldr	w0, [x29, 100]
	sub	w0, w0, #1
	str	w0, [x29, 96]
	add	w0, w4, w0
	str	w0, [x29, 136]
	cbz	w4, .L2525
	mov	w26, 0
	str	wzr, [x29, 140]
	b	.L2526
	.p2align 3
.L2756:
	ldr	w0, [x27, 1856]
	cmp	w0, w26
	ldr	w0, [x29, 140]
	csel	w0, w0, wzr, ne
	str	w0, [x29, 140]
.L2536:
	add	x19, x19, 12288
	add	x19, x28, x19, lsl 2
	str	wzr, [x19, 7380]
.L2535:
	ldr	w1, [x24, 32]
	add	w26, w26, 1
	cmp	w1, w26
	bls	.L2537
	ldr	w1, [x27, 1856]
.L2526:
	ldr	w0, [x29, 136]
	cmp	w1, w26
	sub	w19, w0, w26
	bls	.L2528
	cbnz	w23, .L2754
	mov	w0, 0
	uxtw	x25, w26
.L2529:
	sxtw	x0, w0
	add	x1, x21, x0
	add	x0, x21, x0, lsl 2
	ldrb	w1, [x1, 16]
	ldr	w3, [x0, 248]
	add	x0, x24, x19
	strb	w1, [x0, 53]
.L2532:
	add	x22, x28, x25, lsl 2
	mov	w1, 1
	add	x22, x22, 53248
	str	w3, [x22, 3156]
	ldr	x0, [x28, 2312]
	bl	BsGet
	uxtb	w0, w0
	add	x7, x28, x25
	add	x7, x7, 53248
	strb	w0, [x7, 1791]
	cbnz	w0, .L2755
	cbnz	w26, .L2756
	str	wzr, [x29, 140]
	b	.L2536
	.p2align 3
.L2746:
	ldrb	w0, [x28, 3]
	cbz	w0, .L2613
	ldr	w0, [x27, 1808]
	cmp	w0, wzr
	ldr	w0, [x29, 132]
	ccmp	w0, 5, 2, ne
	bls	.L2757
	b	.L2613
	.p2align 3
.L2740:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1762]
	b	.L2476
	.p2align 3
.L2743:
	ldr	x0, [x28, 2312]
	bl	BsGet
	str	w0, [x27, 1840]
	ldp	w1, w3, [x21, 240]
	mul	w3, w1, w3
	tbnz	w0, #31, .L2493
	cmp	w3, w0
	bls	.L2493
	mul	w0, w24, w0
	b	.L2491
	.p2align 3
.L2745:
	ldr	x1, .LC835
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	str	w0, [x27, 1900]
	ldr	w1, [x20, 12]
	and	w1, w1, 16776960
	cmp	w1, 65536
	beq	.L2758
	cmp	w1, 256
	ldr	w1, [x20, 68]
	beq	.L2759
	ldr	w2, [x21, 244]
	mul	w1, w1, w2
	sub	w1, w1, #1
.L2596:
	cmp	w1, 255
	mov	w3, 255
	csel	w3, w1, w3, ls
	cmp	w0, w3
	bhi	.L2760
	cbz	w0, .L2600
	ldr	x1, .LC839
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w0, 31
	str	w2, [x27, 1824]
	bhi	.L2761
	ldr	w0, [x27, 1900]
	cbz	w0, .L2600
	mov	w19, 0
	b	.L2602
	.p2align 3
.L2762:
	ldr	w2, [x27, 1824]
.L2602:
	ldr	x0, [x28, 2312]
	mov	w1, w2
	bl	BsGet
	uxtw	x1, w19
	add	w19, w19, 1
	add	x1, x1, 12288
	add	x1, x28, x1, lsl 2
	str	w0, [x1, 6036]
	ldr	w0, [x27, 1900]
	cmp	w0, w19
	bhi	.L2762
	ldrb	w0, [x20, 23]
	cbz	w0, .L2610
.L2604:
	ldr	x1, .LC843
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	cmp	w0, 256
	str	w0, [x27, 1904]
	bhi	.L2763
	ldr	x0, [x28, 2312]
	bl	BsResidBits
	ldr	w3, [x27, 1904]
	cmp	w3, w0
	bhi	.L2608
	mov	w19, 0
	cbz	w3, .L2610
	.p2align 2
.L2686:
	ldr	x0, [x28, 2312]
	mov	w1, 8
	add	w19, w19, 1
	bl	BsGet
	str	w0, [x27, 1908]
	ldr	w1, [x27, 1904]
	cmp	w1, w19
	bhi	.L2686
	b	.L2610
	.p2align 3
.L2612:
	ldr	w0, [x29, 132]
	cmp	w0, 5
	bhi	.L2613
	strb	wzr, [x28, 3]
	b	.L2613
	.p2align 3
.L2750:
	sub	w2, w1, w4
	cmp	w2, w3, lsr 1
	bcc	.L2503
	add	w0, w0, w3
	b	.L2504
.L2758:
	ldr	w1, [x21, 244]
	sub	w1, w1, #1
	b	.L2596
.L2508:
	sub	w1, w0, #16
	cmp	w1, 7
	cset	w1, ls
	b	.L2509
	.p2align 3
.L2528:
	ldr	w1, [x21, 132]
	uxtw	x25, w26
	ldr	x0, [x28, 2312]
	bl	BsGet
	mov	w3, w0
	add	x0, x25, 12288
	mov	w1, 1
	str	w3, [x29, 128]
	add	x0, x28, x0, lsl 2
	str	w3, [x0, 7124]
	ldr	x0, [x28, 2312]
	bl	BsGet
	uxtb	w0, w0
	add	x1, x28, x25
	ldr	w3, [x29, 128]
	add	x1, x1, 53248
	strb	w0, [x1, 1775]
	add	x1, x24, x19
	strb	w0, [x1, 53]
	b	.L2532
	.p2align 3
.L2755:
	ldr	x1, .LC783
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	str	w0, [x22, 3092]
	cbz	w26, .L2534
	ldr	w1, [x29, 140]
	add	w25, w0, w1
	ldr	w1, [x27, 1856]
	cmp	w1, w26
	csel	w0, w25, w0, ne
.L2534:
	add	x19, x28, x19, lsl 2
	mov	w1, 1
	add	x19, x19, 53248
	str	w0, [x29, 140]
	str	w0, [x19, 3220]
	str	w1, [x19, 3284]
	b	.L2535
	.p2align 3
.L2754:
	ldr	x0, [x28, 2312]
	mov	w1, w23
	uxtw	x25, w26
	bl	BsGet
	add	x1, x25, 12288
	add	x1, x28, x1, lsl 2
	str	w0, [x1, 7060]
	ldr	w4, [x21, 232]
	sub	w4, w4, #1
	tbnz	w0, #31, .L2531
	cmp	w4, w0
	bcs	.L2529
.L2531:
	ldr	x1, .LC781
	mov	w3, w0
	mov	w2, w26
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
	.p2align 3
.L2502:
	str	wzr, [x27, 1808]
	str	wzr, [x28, 188]
	ldr	w0, [x27, 1828]
	cbnz	w0, .L2543
	str	wzr, [x27, 1812]
	str	wzr, [x28, 184]
.L2543:
	ldrb	w0, [x21, 8]
	cbz	w0, .L2620
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1765]
	mov	w1, 1
	ldr	x0, [x28, 2312]
	bl	BsGet
	strb	w0, [x27, 1766]
.L2620:
	strb	wzr, [x27, 1767]
	str	xzr, [x29, 120]
.L2618:
	ldr	w0, [x27, 1820]
	cmp	w0, 2
	bne	.L2764
.L2544:
	ldrb	w0, [x27, 1767]
	cbnz	w0, .L2765
.L2565:
	mov	w1, 2
.L2575:
	ldrb	w0, [x20, 11]
	cbz	w0, .L2564
	cbnz	w1, .L2564
.L2578:
	mov	x0, x28
	bl	HEVC_DecPredWeightTable
	cbnz	w0, .L2766
	ldr	w1, [x27, 1820]
.L2564:
	cmp	w1, 2
	beq	.L2579
	ldr	x1, .LC807
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	mov	w3, 4
	mov	w1, 5
	sub	w3, w3, w0
	sub	w2, w1, w0
	cmp	w3, 4
	str	w2, [x27, 1876]
	bhi	.L2767
.L2579:
	ldr	x1, .LC811
	ldr	x0, [x28, 2312]
	bl	HEVC_se_v
	str	w0, [x27, 1880]
	mov	w4, 51
	ldr	w1, [x20, 48]
	add	w2, w0, w1
	str	w2, [x27, 1816]
	ldr	w3, [x21, 104]
	neg	w0, w3
	cmp	w2, w0
	ccmp	w2, w4, 0, ge
	bgt	.L2768
	ldrb	w0, [x20, 9]
	cbnz	w0, .L2769
.L2581:
	ldrb	w0, [x20, 18]
	cbz	w0, .L2729
	ldrb	w0, [x20, 19]
	cbnz	w0, .L2770
	strb	wzr, [x27, 1772]
.L2589:
	ldrb	w0, [x20, 20]
	strb	w0, [x27, 1773]
	ldr	w1, [x20, 76]
	str	w1, [x27, 1892]
	ldr	w1, [x20, 80]
	str	w1, [x27, 1896]
.L2586:
	ldrb	w2, [x21, 8]
	mov	w1, 0
	cbz	w2, .L2592
	ldr	w1, [x27, 1764]
	tst	w1, 16776960
	cset	w1, ne
.L2592:
	ldrb	w2, [x20, 17]
	cbz	w2, .L2593
	cmp	w0, wzr
	cset	w0, eq
	orr	w1, w0, w1
	cbnz	w1, .L2771
.L2593:
	strb	w2, [x27, 1774]
	b	.L2494
	.p2align 3
.L2764:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1768]
	cbnz	w0, .L2772
	ldr	w0, [x20, 40]
	ldr	w1, [x27, 1820]
	str	w0, [x27, 1928]
	cbnz	w1, .L2552
	ldr	w0, [x20, 44]
	str	w0, [x27, 1932]
.L2550:
	ldr	x0, [x29, 120]
	cbz	x0, .L2773
	ldr	x0, [x29, 120]
	ldr	w2, [x0, 24]
	cmp	w2, 16
	bhi	.L2554
	cbz	w2, .L2556
	ldr	x1, [x29, 120]
	sub	w2, w2, #1
	add	x2, x2, 54
	mov	w3, 0
	add	x0, x1, 53
	add	x2, x1, x2
	.p2align 2
.L2558:
	ldrb	w1, [x0], 1
	cmp	w1, wzr
	csinc	w3, w3, w3, eq
	cmp	x0, x2
	bne	.L2558
	ldrb	w0, [x20, 22]
	cmp	w0, wzr
	ccmp	w3, 1, 4, ne
	ble	.L2556
	add	x2, x28, 57344
	add	x1, x27, 1760
	add	x2, x2, 1136
	mov	x0, x28
	bl	HEVC_DecRefPicListsModification
	cbnz	w0, .L2774
.L2549:
	ldr	w1, [x27, 1820]
	cbnz	w1, .L2559
.L2621:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1769]
	ldr	w1, [x27, 1820]
.L2559:
	cmp	w1, 2
	beq	.L2544
.L2622:
	ldrb	w0, [x20, 5]
	cbnz	w0, .L2775
.L2560:
	ldrb	w0, [x27, 1767]
	cbz	w0, .L2561
	ldr	w0, [x27, 1820]
	cbz	w0, .L2776
	mov	w1, 1
	cmp	w0, 2
	strb	w1, [x27, 1771]
	beq	.L2565
.L2568:
	ldr	w0, [x27, 1928]
	cmp	w0, 1
	bls	.L2561
.L2566:
	ldr	x1, .LC801
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	str	w0, [x27, 1872]
	ldrb	w2, [x27, 1771]
	cbnz	w2, .L2571
	ldr	w4, [x27, 1868]
	sub	w4, w4, #1
.L2573:
	cmp	w4, w0
	ccmp	w0, wzr, 1, cs
	bge	.L2561
	ldr	x1, .LC803
	mov	w3, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
	.p2align 3
.L2561:
	ldr	w1, [x27, 1820]
.L2570:
	ldrb	w0, [x20, 10]
	cbz	w0, .L2575
	cmp	w1, 1
	bne	.L2575
	b	.L2578
.L2770:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1772]
	cbz	w0, .L2589
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1773]
	cbnz	w0, .L2586
	ldr	x1, .LC827
	ldr	x0, [x28, 2312]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x27, 1892]
	bhi	.L2777
.L2590:
	ldr	x1, .LC831
	ldr	x0, [x28, 2312]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x27, 1896]
	bls	.L2729
	ldr	x1, .LC833
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 80]
	str	w0, [x27, 1896]
	.p2align 2
.L2729:
	ldrb	w0, [x27, 1773]
	b	.L2586
.L2525:
	mov	w1, 0
.L2527:
	ldr	w0, [x29, 100]
	add	w2, w0, w1
	ldr	x0, [x29, 104]
	cmp	w2, 16
	str	w2, [x0, 956]
	bhi	.L2778
.L2518:
	ldrb	w0, [x21, 11]
	cbnz	w0, .L2779
	strb	wzr, [x27, 1767]
.L2542:
	ldrb	w0, [x21, 8]
	cbz	w0, .L2618
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1765]
	mov	w1, 1
	ldr	x0, [x28, 2312]
	bl	BsGet
	strb	w0, [x27, 1766]
	b	.L2618
	.p2align 3
.L2556:
	ldr	w0, [x27, 1820]
	cbnz	w0, .L2622
	b	.L2621
	.p2align 3
.L2749:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1760]
	b	.L2501
	.p2align 3
.L2478:
	ldr	x2, .LC751
	mov	w3, w0
	ldr	x1, .LC753
	mov	w0, 1
	add	x2, x2, 48
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
	.p2align 3
.L2769:
	ldr	x1, .LC815
	ldr	x0, [x28, 2312]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x27, 1884]
	bhi	.L2780
	ldr	w3, [x20, 60]
	add	w1, w0, w3
	add	w1, w1, 12
	cmp	w1, 24
	bhi	.L2781
	ldr	x1, .LC821
	ldr	x0, [x28, 2312]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x27, 1888]
	bhi	.L2782
	ldr	w1, [x20, 64]
	add	w2, w0, w1
	add	w0, w2, 12
	cmp	w0, 24
	bls	.L2581
	ldr	x1, .LC825
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
	.p2align 3
.L2775:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1770]
	b	.L2560
.L2537:
	ldr	w0, [x29, 96]
	add	w9, w1, w0
	cbz	w1, .L2525
	add	x7, x27, 3156
	mov	x3, 0
	b	.L2540
	.p2align 3
.L2783:
	ldr	w0, [x7]
	ldr	w2, [x8, 3220]
	ldr	w5, [x29, 112]
	ldr	w10, [x27, 1808]
	msub	w8, w5, w2, w0
	ldr	w0, [x29, 116]
	sub	w2, w10, w0
	add	w0, w8, w2
	str	w0, [x4, 1072]
	sub	w0, w0, w10
	str	w0, [x4, 1004]
.L2539:
	add	x3, x3, 1
	add	x7, x7, 4
	cmp	w1, w3
	bls	.L2527
.L2540:
	add	x2, x3, 53248
	sub	w0, w9, w3
	add	x2, x2, 1791
	add	x4, x28, x0, uxtw 2
	add	x0, x4, 57344
	add	x8, x4, 53248
	ldrb	w2, [x28, x2]
	mov	x4, x0
	cbnz	w2, .L2783
	ldr	w2, [x7]
	str	w2, [x0, 1072]
	ldr	w4, [x27, 1808]
	sub	w2, w2, w4
	str	w2, [x0, 1004]
	b	.L2539
.L2552:
	str	wzr, [x27, 1932]
.L2551:
	cmp	w1, 2
	bne	.L2550
	ldr	w1, [x27, 1820]
	b	.L2559
	.p2align 3
.L2510:
	add	x0, x28, 57344
	ldr	w3, [x21, 228]
	add	x2, x0, 932
	str	x0, [x29, 104]
	mov	x1, x21
	mov	x0, x28
	str	x2, [x29, 120]
	bl	HEVC_DecShortTermRefPicSet
	cbz	w0, .L2513
	ldr	x1, .LC769
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
	.p2align 3
.L2772:
	ldr	x1, .LC787
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w2, 16
	str	w2, [x27, 1864]
	bhi	.L2784
	ldr	w1, [x27, 1820]
	str	w2, [x27, 1928]
	cbz	w1, .L2785
	str	wzr, [x27, 1868]
	str	wzr, [x27, 1932]
	b	.L2551
.L2776:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	ldr	w1, [x27, 1820]
	strb	w0, [x27, 1771]
	cmp	w1, 2
	beq	.L2579
	cbnz	w0, .L2568
	ldr	w0, [x27, 1932]
	cmp	w0, 1
	bhi	.L2566
	b	.L2570
.L2765:
	mov	w0, 1
	mov	w1, 2
	strb	w0, [x27, 1771]
	b	.L2570
.L2771:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1774]
	b	.L2494
.L2752:
	ldr	x0, [x28, 2312]
	bl	BsGet
	sxtw	x22, w0
	str	w22, [x27, 1852]
	ldr	w3, [x21, 228]
	sub	w3, w3, #1
	tbnz	w22, #31, .L2516
	cmp	w3, w22
	bcs	.L2517
.L2516:
	ldr	x1, .LC771
	mov	w2, w22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2571:
	ldr	w4, [x27, 1864]
	sub	w4, w4, #1
	b	.L2573
.L2519:
	ldr	x1, .LC773
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	str	w0, [x27, 1856]
	ldr	w3, [x21, 232]
	cmp	w0, w3
	bhi	.L2786
	cmp	w3, 1
	bls	.L2522
	mov	w23, 0
	mov	w1, 1
	.p2align 2
.L2523:
	add	w23, w23, 1
	lsl	w0, w1, w23
	cmp	w3, w0
	bhi	.L2523
	b	.L2520
.L2747:
	mov	w0, 1
	strb	w0, [x28, 2]
	b	.L2616
.L2785:
	ldr	x1, .LC791
	ldr	x0, [x28, 2312]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w2, 16
	str	w2, [x27, 1868]
	bhi	.L2787
	ldr	w0, [x27, 1820]
	str	w2, [x27, 1932]
	cmp	w0, 2
	bne	.L2550
	b	.L2549
	.p2align 3
.L2763:
	ldr	x1, .LC845
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2778:
	ldr	x1, .LC785
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
	.p2align 3
.L2779:
	ldr	x0, [x28, 2312]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1767]
	b	.L2542
.L2487:
	ldr	x1, .LC757
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2759:
	ldr	w2, [x20, 72]
	mul	w1, w1, w2
	sub	w1, w1, #1
	b	.L2596
.L2741:
	ldr	w0, [x28, 216]
	cbz	w0, .L2482
	ldr	w0, [x28, 188]
	str	w0, [x27, 1808]
.L2483:
	mov	x0, x28
	bl	HEVC_IsNewPic
	str	w0, [x27, 3348]
	ldr	x1, .LC755
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2768:
	ldr	x1, .LC813
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2482:
	str	wzr, [x27, 1808]
	b	.L2483
.L2751:
	ldr	x1, .LC767
	bl	dprint_vfmw
	ldr	w3, [x27, 1808]
	b	.L2506
.L2493:
	ldr	x1, .LC759
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2748:
	ldr	x1, .LC765
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2554:
	ldr	x1, .LC797
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2767:
	ldr	x1, .LC809
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2781:
	ldr	x1, .LC819
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2608:
	ldr	x1, .LC847
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2760:
	ldr	x1, .LC837
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2777:
	ldr	x1, .LC829
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 76]
	str	w0, [x27, 1892]
	b	.L2590
.L2766:
	ldr	x1, .LC805
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2780:
	ldr	x1, .LC817
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2782:
	ldr	x1, .LC823
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2753:
	ldr	x1, .LC779
	mov	w5, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2774:
	ldr	x1, .LC799
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2784:
	ldr	x1, .LC789
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2786:
	ldr	x1, .LC775
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2773:
	ldr	x1, .LC795
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2761:
	ldr	x1, .LC841
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
.L2787:
	ldr	x1, .LC793
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2722
	.size	HEVC_DecSliceSegmentHeader, .-HEVC_DecSliceSegmentHeader
	.align	3
.LC750:
	.xword	.LC749
	.align	3
.LC751:
	.xword	.LANCHOR5
	.align	3
.LC753:
	.xword	.LC752
	.align	3
.LC755:
	.xword	.LC754
	.align	3
.LC757:
	.xword	.LC756
	.align	3
.LC759:
	.xword	.LC758
	.align	3
.LC761:
	.xword	.LC760
	.align	3
.LC763:
	.xword	.LC762
	.align	3
.LC765:
	.xword	.LC764
	.align	3
.LC767:
	.xword	.LC766
	.align	3
.LC769:
	.xword	.LC768
	.align	3
.LC771:
	.xword	.LC770
	.align	3
.LC773:
	.xword	.LC772
	.align	3
.LC775:
	.xword	.LC774
	.align	3
.LC777:
	.xword	.LC776
	.align	3
.LC779:
	.xword	.LC778
	.align	3
.LC781:
	.xword	.LC780
	.align	3
.LC783:
	.xword	.LC782
	.align	3
.LC785:
	.xword	.LC784
	.align	3
.LC787:
	.xword	.LC786
	.align	3
.LC789:
	.xword	.LC788
	.align	3
.LC791:
	.xword	.LC790
	.align	3
.LC793:
	.xword	.LC792
	.align	3
.LC795:
	.xword	.LC794
	.align	3
.LC797:
	.xword	.LC796
	.align	3
.LC799:
	.xword	.LC798
	.align	3
.LC801:
	.xword	.LC800
	.align	3
.LC803:
	.xword	.LC802
	.align	3
.LC805:
	.xword	.LC804
	.align	3
.LC807:
	.xword	.LC806
	.align	3
.LC809:
	.xword	.LC808
	.align	3
.LC811:
	.xword	.LC810
	.align	3
.LC813:
	.xword	.LC812
	.align	3
.LC815:
	.xword	.LC814
	.align	3
.LC817:
	.xword	.LC816
	.align	3
.LC819:
	.xword	.LC818
	.align	3
.LC821:
	.xword	.LC820
	.align	3
.LC823:
	.xword	.LC822
	.align	3
.LC825:
	.xword	.LC824
	.align	3
.LC827:
	.xword	.LC826
	.align	3
.LC829:
	.xword	.LC828
	.align	3
.LC831:
	.xword	.LC830
	.align	3
.LC833:
	.xword	.LC832
	.align	3
.LC835:
	.xword	.LC834
	.align	3
.LC837:
	.xword	.LC836
	.align	3
.LC839:
	.xword	.LC838
	.align	3
.LC841:
	.xword	.LC840
	.align	3
.LC843:
	.xword	.LC842
	.align	3
.LC845:
	.xword	.LC844
	.align	3
.LC847:
	.xword	.LC846
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_FrameStoreRefFlg
	.type	HEVC_FrameStoreRefFlg, %function
HEVC_FrameStoreRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 1]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_FrameStoreRefFlg, .-HEVC_FrameStoreRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ClearCurrSlice
	.type	HEVC_ClearCurrSlice, %function
HEVC_ClearCurrSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 2360]
	cbz	x1, .L2790
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L2790:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_ClearCurrSlice, .-HEVC_ClearCurrSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ClearCurrNal
	.type	HEVC_ClearCurrNal, %function
HEVC_ClearCurrNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	HEVC_ClearCurrSlice
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_ClearCurrNal, .-HEVC_ClearCurrNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ClearAllSlice
	.type	HEVC_ClearAllSlice, %function
HEVC_ClearAllSlice:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w20, 0
	add	x22, x21, 65536
	bl	_mcount
	ldr	w2, [x21, 160]
	cbz	w2, .L2799
	.p2align 2
.L2802:
	uxtw	x0, w20
	ldr	x1, [x22, 3664]
	lsl	x19, x0, 5
	add	w20, w20, 1
	add	x19, x19, x0, lsl 3
	add	x0, x1, x19
	ldr	x0, [x0, 32]
	mov	x1, x0
	cbz	x0, .L2797
	ldr	w0, [x21, 244]
	bl	HEVC_ReleaseNAL
	ldr	x0, [x22, 3664]
	add	x0, x0, x19
	str	xzr, [x0, 32]
	ldr	w2, [x21, 160]
.L2797:
	cmp	w2, w20
	bhi	.L2802
.L2799:
	strb	wzr, [x21, 9]
	mov	w0, 2
	str	wzr, [x21, 208]
	str	w0, [x21, 172]
	mov	w0, 0
	str	wzr, [x21, 216]
	str	wzr, [x21, 224]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_ClearAllSlice, .-HEVC_ClearAllSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InitDPB
	.type	HEVC_InitDPB, %function
HEVC_InitDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w22, w1
	add	x19, x20, 2704
	bl	_mcount
	add	x21, x20, 2840
	mov	x0, x20
	bl	HEVC_ClearAllSlice
	.p2align 2
.L2811:
	ldr	x0, [x19]
	mov	w2, 1
	cbz	x0, .L2807
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L2807:
	ldr	x0, [x19, 136]
	mov	w2, 1
	cbz	x0, .L2808
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L2808:
	ldr	x0, [x19, 272]
	mov	w2, 1
	cbz	x0, .L2809
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L2809:
	ldr	x0, [x19, 408]
	mov	w2, 1
	cbz	x0, .L2810
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
.L2810:
	str	xzr, [x19, 136]
	str	xzr, [x19, 272]
	str	xzr, [x19, 408]
	str	xzr, [x19], 8
	cmp	x19, x21
	bne	.L2811
	ldr	x0, [x20, 28136]
	ldr	w0, [x0, 236]
	add	w0, w0, 1
	cmp	w0, 17
	bls	.L2851
	mov	w0, 17
.L2851:
	add	x3, x20, 2560
	mov	w1, 0
	mov	x2, 200
	stp	w0, wzr, [x3, 120]
	ldr	x0, .LC848
	stp	wzr, wzr, [x3, 128]
	str	wzr, [x20, 2696]
	ldr	x3, [x0, 96]
	add	x0, x20, 3248
	str	wzr, [x20, 2700]
	blr	x3
	str	wzr, [x20, 3252]
	mov	w0, 16
	add	x1, x20, 2368
	str	w0, [x20, 3248]
	add	x0, x20, 2504
	.p2align 2
.L2814:
	str	xzr, [x1], 8
	cmp	x1, x0
	bne	.L2814
	add	x2, x20, 53248
	add	x1, x20, 2640
	str	wzr, [x2, 1920]
	.p2align 2
.L2815:
	str	xzr, [x0], 8
	cmp	x1, x0
	bne	.L2815
	str	wzr, [x2, 1924]
	cbnz	w22, .L2816
	ldr	x0, [x20, 2344]
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L2816
.L2817:
	ldr	w0, [x20, 180]
	cbz	w0, .L2825
	sub	w1, w0, #1
	add	x0, x20, 2116
	add	x1, x1, 530
	add	x1, x20, x1, lsl 2
	.p2align 2
.L2824:
	str	wzr, [x0], 4
	cmp	x1, x0
	bne	.L2824
.L2825:
	ldr	w2, [x20, 160]
	mov	w21, 0
	add	x22, x20, 65536
	cbz	w2, .L2823
	.p2align 2
.L2840:
	uxtw	x0, w21
	ldr	x1, [x22, 3664]
	lsl	x19, x0, 5
	add	w21, w21, 1
	add	x19, x19, x0, lsl 3
	add	x0, x1, x19
	ldr	x0, [x0, 32]
	mov	x1, x0
	cbz	x0, .L2826
	ldr	x3, [x20, 2360]
	cmp	x0, x3
	beq	.L2826
	ldr	w0, [x20, 244]
	bl	HEVC_ReleaseNAL
	ldr	x0, [x22, 3664]
	add	x0, x0, x19
	str	xzr, [x0, 32]
	ldr	w2, [x20, 160]
.L2826:
	cmp	w2, w21
	bhi	.L2840
.L2823:
	mov	w0, 0
.L2818:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2816:
	ldr	x0, [x20, 28136]
	mov	w19, 20
	stp	w19, w19, [x20, 176]
	mov	w6, w19
	mov	w5, w19
	ldr	w1, [x0, 148]
	ldp	w3, w4, [x0, 240]
	mov	x0, x20
	lsl	w2, w4, w1
	lsl	w1, w3, w1
	bl	HEVC_ArrangeVahbMem
	ldr	x1, [x20, 2344]
	ldr	w1, [x1, 872]
	cmp	w1, 1
	beq	.L2828
	cmp	w0, 1
	bne	.L2852
	ldr	w0, [x20, 244]
	bl	FSP_GetTotalValidFsNum
	cmp	w0, 20
	mov	w0, w19
	ble	.L2853
	str	w0, [x20, 176]
	b	.L2817
.L2853:
	ldr	w0, [x20, 244]
	bl	FSP_GetTotalValidFsNum
	str	w0, [x20, 176]
	b	.L2817
.L2852:
	ldr	x2, .LC849
	mov	w0, 0
	ldr	x1, .LC851
	add	x2, x2, 80
	bl	dprint_vfmw
	mov	w0, -5
	b	.L2818
.L2828:
	mov	w0, -2
	b	.L2818
	.size	HEVC_InitDPB, .-HEVC_InitDPB
	.align	3
.LC848:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC849:
	.xword	.LANCHOR5
	.align	3
.LC851:
	.xword	.LC850
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ClearAllNal
	.type	HEVC_ClearAllNal, %function
HEVC_ClearAllNal:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w19, 0
	add	x20, x21, 65536
	bl	_mcount
	ldr	w3, [x21, 160]
	cmn	w3, #1
	bne	.L2859
	b	.L2858
	.p2align 3
.L2856:
	add	w0, w3, 1
	cmp	w0, w19
	bls	.L2858
.L2859:
	uxtw	x1, w19
	ldr	x2, [x20, 2856]
	lsl	x0, x1, 3
	add	w19, w19, 1
	sub	x0, x0, x1
	add	x1, x2, x0, lsl 4
	ldr	w0, [x1, 36]
	cmp	w0, 1
	bne	.L2856
	ldr	w0, [x21, 244]
	bl	HEVC_ReleaseNAL
	ldr	w3, [x21, 160]
	add	w0, w3, 1
	cmp	w0, w19
	bhi	.L2859
.L2858:
	strb	wzr, [x21, 5]
	mov	w0, 0
	str	wzr, [x21, 140]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_ClearAllNal, .-HEVC_ClearAllNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InitDecPara
	.type	HEVC_InitDecPara, %function
HEVC_InitDecPara:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	add	x22, x21, 65536
	mov	w20, 0
	bl	_mcount
	add	x19, x22, 3672
	ldr	x1, .LC853
	mov	w0, 22
	mov	w25, 20
	mov	w24, 16
	bl	dprint_vfmw
	mov	w1, -2147483647
	mov	x0, x21
	str	w1, [x21, 212]
	bl	HEVC_ClearAllNal
	add	x0, x21, 61440
	add	x0, x0, 1032
	bl	ResetVoQueue
	ldr	x23, .LC854
	.p2align 2
.L2863:
	strb	wzr, [x19]
	add	x0, x19, 80
	strb	wzr, [x19, 1]
	mov	x2, 624
	strb	wzr, [x19, 2]
	mov	w1, 0
	str	wzr, [x19, 4]
	str	wzr, [x19, 12]
	stp	w25, w24, [x19, 24]
	ldr	x3, [x23, 96]
	blr	x3
	str	w20, [x19, 236]
	add	w20, w20, 1
	str	wzr, [x19, 52]
	str	x19, [x19, 72]
	cmp	w20, 20
	add	x19, x19, 712
	bne	.L2863
	add	x0, x21, 2116
	add	x1, x21, 2196
	.p2align 2
.L2864:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L2864
	ldr	w0, [x21, 160]
	mov	w1, 0
	cbz	w0, .L2868
	.p2align 2
.L2870:
	uxtw	x0, w1
	ldr	x2, [x22, 3664]
	lsl	x3, x0, 5
	add	w1, w1, 1
	add	x0, x3, x0, lsl 3
	str	wzr, [x2, x0]
	ldr	x2, [x22, 3664]
	add	x2, x2, x0
	str	xzr, [x2, 24]
	ldr	x2, [x22, 3664]
	add	x0, x2, x0
	str	xzr, [x0, 32]
	ldr	w0, [x21, 160]
	cmp	w0, w1
	bhi	.L2870
.L2868:
	ldr	x3, [x23, 96]
	mov	w1, 0
	mov	x2, 200
	add	x0, x21, 3248
	blr	x3
	mov	w1, 16
	add	x0, x21, 2840
	str	w1, [x21, 3248]
	add	x1, x21, 2976
	.p2align 2
.L2866:
	str	xzr, [x0, -136]
	add	x0, x0, 8
	str	xzr, [x0, -8]
	str	xzr, [x0, 128]
	str	xzr, [x0, 264]
	cmp	x1, x0
	bne	.L2866
	add	x2, x21, 2560
	mov	w0, 17
	add	x19, x21, 2504
	stp	w0, wzr, [x2, 120]
	stp	wzr, wzr, [x2, 128]
	str	wzr, [x21, 2696]
	add	x0, x21, 2640
	str	wzr, [x21, 2700]
	.p2align 2
.L2869:
	str	xzr, [x19, -136]
	add	x19, x19, 8
	str	xzr, [x19, -8]
	cmp	x19, x0
	bne	.L2869
	add	x22, x21, 53248
	ldr	x3, [x23, 96]
	mov	w20, 2
	mov	x2, 3732
	str	w20, [x21, 172]
	mov	w1, 0
	add	x0, x22, 1760
	str	wzr, [x21, 208]
	str	wzr, [x21, 216]
	str	wzr, [x21, 224]
	str	xzr, [x21, 2360]
	blr	x3
	str	w20, [x22, 1820]
	mov	w0, 1
	mov	w1, 8
	str	w0, [x22, 3348]
	mov	w2, 16
	stp	w1, w1, [x21, 192]
	str	w1, [x21, 312]
	add	x1, x21, 2048
	strb	w0, [x21, 1]
	mov	w3, 20
	strb	w0, [x21, 3]
	mov	w5, 64
	strb	wzr, [x21, 4]
	mov	w0, 6
	strb	wzr, [x21, 2]
	strb	wzr, [x21, 8]
	stp	w3, w3, [x21, 176]
	str	w2, [x21, 292]
	mov	w3, 2147483647
	str	w5, [x21, 296]
	mov	w5, 1025
	str	w2, [x21, 316]
	mov	x2, 40
	str	wzr, [x21, 168]
	ldr	x4, [x23, 96]
	stp	w0, w20, [x1, 156]
	stp	w0, w0, [x1, 148]
	str	w3, [x21, 200]
	mov	w1, 32
	mov	w3, 256
	str	w5, [x21, 304]
	str	w1, [x21, 320]
	mov	x0, x19
	str	w3, [x21, 300]
	mov	w1, 0
	mov	w3, 4
	str	w3, [x21, 308]
	blr	x4
	str	x19, [x21, 2312]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	HEVC_InitDecPara, .-HEVC_InitDecPara
	.align	3
.LC853:
	.xword	.LC852
	.align	3
.LC854:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_Init
	.type	HEVCDEC_Init, %function
HEVCDEC_Init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x1
	add	x21, x19, 61440
	bl	_mcount
	ldr	x20, .LC855
	mov	x2, 21408
	mov	w1, 0
	movk	x2, 0x1, lsl 16
	mov	x0, x19
	ldr	x3, [x20, 96]
	ldr	w23, [x21, 1632]
	blr	x3
	str	w23, [x21, 1632]
	str	x22, [x19, 2344]
	str	xzr, [x19, 2352]
	ldr	w0, [x22, 28]
	cmp	w0, 25
	beq	.L2884
	mov	w0, 17
	mov	w3, 65
	mov	w2, 200
	mov	w1, 210
	stp	w2, w0, [x19, 144]
	stp	w0, w3, [x19, 152]
	str	w1, [x19, 160]
.L2878:
	ldr	w0, [x22, 584]
	bl	MEM_Phy2Vir
	mov	x5, x0
	cbz	x0, .L2885
	ldr	x7, [x19, 2344]
	mov	w2, 23196
	ldr	w9, [x19, 148]
	mov	w23, 26164
	ldpsw	x4, x0, [x19, 152]
	ldr	w3, [x7, 580]
	add	x24, x19, 65536
	lsl	w1, w0, 7
	sub	w1, w1, w0
	add	x8, x5, x3, uxtw
	lsl	w1, w1, 1
	madd	w3, w9, w2, w3
	sub	w1, w1, w0
	str	x8, [x19, 2320]
	madd	w4, w23, w4, w3
	add	x3, x5, x3, uxtw
	str	x3, [x19, 2328]
	add	w1, w4, w1, lsl 3
	mov	w3, 1368
	add	x4, x5, x4, uxtw
	str	x4, [x19, 2336]
	add	x0, x5, x1, uxtw
	str	x0, [x24, 2848]
	ldrsw	x6, [x19, 144]
	madd	w1, w3, w6, w1
	add	x3, x5, x1, uxtw
	str	x3, [x24, 2856]
	ldr	w3, [x19, 160]
	add	w10, w3, 1
	lsl	w4, w3, 5
	add	w3, w4, w3, lsl 3
	lsl	w4, w10, 3
	sub	w4, w4, w10
	add	w1, w1, w4, lsl 4
	add	w3, w3, w1
	add	x1, x5, x1, uxtw
	str	x1, [x24, 3664]
	ldr	w4, [x7, 588]
	cmp	w3, w4
	bhi	.L2886
	ldr	w1, [x22, 608]
	cbz	w1, .L2887
.L2882:
	lsl	x2, x6, 2
	ldr	x3, [x20, 96]
	sub	x2, x2, x6
	mov	w1, 0
	lsl	x2, x2, 2
	mov	w22, 1
	sub	x2, x2, x6
	lsl	x2, x2, 2
	sub	x2, x2, x6
	lsl	x2, x2, 2
	sub	x2, x2, x6
	lsl	x2, x2, 3
	blr	x3
	ldr	w2, [x19, 160]
	mov	w1, 0
	ldr	x4, [x20, 96]
	add	w2, w2, 1
	ldr	x0, [x24, 2856]
	sxtw	x2, w2
	lsl	x3, x2, 3
	sub	x2, x3, x2
	lsl	x2, x2, 4
	blr	x4
	ldrsw	x2, [x19, 160]
	mov	w1, 0
	ldr	x3, [x20, 96]
	lsl	x4, x2, 5
	ldr	x0, [x24, 3664]
	add	x2, x4, x2, lsl 3
	blr	x3
	ldp	w5, w2, [x19, 148]
	mov	w1, 23196
	mov	x0, -23196
	ldr	w4, [x19, 156]
	mov	w3, 2024
	smaddl	x0, w5, w1, x0
	mov	x1, -2024
	ldr	x6, [x19, 2320]
	smaddl	x3, w4, w3, x1
	mov	w4, 26164
	mov	x1, -26164
	add	x0, x6, x0
	ldr	x6, [x19, 2336]
	smaddl	x1, w2, w4, x1
	ldr	x4, [x19, 2328]
	add	x2, x6, x3
	str	x0, [x19, 28128]
	str	x2, [x19, 28144]
	add	x1, x4, x1
	str	x1, [x19, 28136]
	str	w5, [x0, 8]
	mov	x0, x19
	ldr	x1, [x19, 28144]
	ldr	w2, [x19, 152]
	str	w2, [x1, 28]
	ldr	x1, [x19, 28136]
	ldr	w2, [x19, 156]
	str	w2, [x1, 60]
	bl	HEVC_InitScalingOrderTable
	add	x1, x19, 86016
	mov	x0, x19
	str	w22, [x1, 916]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x19, 244]
	beq	.L2888
	mov	w1, -2147483647
	mov	x0, x19
	str	w1, [x19, 212]
	bl	HEVC_InitDecPara
	str	wzr, [x21, 1640]
	mov	w1, 9999
	strb	w22, [x19]
	ldr	w0, [x19, 156]
	str	w0, [x19, 288]
	mov	w0, 0
	str	w1, [x19, 252]
.L2880:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2887:
	smull	x2, w9, w2
	ldr	x3, [x20, 96]
	mov	x0, x8
	blr	x3
	ldr	w2, [x19, 152]
	mov	w1, 0
	ldr	x3, [x20, 96]
	ldr	x0, [x19, 2328]
	smull	x2, w2, w23
	blr	x3
	ldr	w4, [x19, 156]
	mov	w2, 2024
	ldr	x0, [x19, 2336]
	mov	w1, 0
	ldr	x3, [x20, 96]
	smull	x2, w4, w2
	blr	x3
	ldrsw	x6, [x19, 144]
	ldr	x0, [x24, 2848]
	b	.L2882
	.p2align 3
.L2884:
	ldr	w0, [x22, 680]
	str	w0, [x19, 148]
	ldr	w0, [x22, 684]
	str	w0, [x19, 152]
	ldr	w0, [x22, 688]
	str	w0, [x19, 156]
	ldr	w0, [x22, 676]
	str	w0, [x19, 144]
	add	w0, w0, 5
	str	w0, [x19, 160]
	b	.L2878
	.p2align 3
.L2886:
	ldr	x2, .LC856
	mov	w0, 0
	ldr	x1, .LC860
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -20
	b	.L2880
	.p2align 3
.L2888:
	ldr	x2, .LC856
	mov	w0, 0
	ldr	x1, .LC862
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -20
	b	.L2880
.L2885:
	ldr	x2, .LC856
	mov	w0, 0
	ldr	x1, .LC858
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -20
	b	.L2880
	.size	HEVCDEC_Init, .-HEVCDEC_Init
	.align	3
.LC855:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC856:
	.xword	.LANCHOR5
	.align	3
.LC858:
	.xword	.LC857
	.align	3
.LC860:
	.xword	.LC859
	.align	3
.LC862:
	.xword	.LC861
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ClearCurrPic
	.type	HEVC_ClearCurrPic, %function
HEVC_ClearCurrPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 53248
	bl	_mcount
	mov	x0, x20
	bl	HEVC_ClearAllSlice
	ldr	x0, [x19, 1128]
	cbz	x0, .L2890
	ldr	w1, [x0, 32]
	mov	w2, 1
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
	ldr	x0, [x19, 1128]
	str	wzr, [x0, 4]
	str	xzr, [x19, 1128]
.L2890:
	strb	wzr, [x19, 1073]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_ClearCurrPic, .-HEVC_ClearCurrPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ClearAll
	.type	HEVC_ClearAll, %function
HEVC_ClearAll:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_InitDPB
	cbnz	w20, .L2897
	add	x0, x19, 61440
	add	x0, x0, 1032
	bl	ResetVoQueue
	ldr	w0, [x19, 244]
	bl	FSP_EmptyInstance
	mov	x0, x19
	bl	HEVC_InitDecPara
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2897:
	add	x1, x19, 61440
	ldr	w0, [x19, 244]
	add	x1, x1, 1032
	bl	FSP_ClearNotInVoQueue
	mov	x0, x19
	bl	HEVC_InitDecPara
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_ClearAll, .-HEVC_ClearAll
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InitPic
	.type	HEVC_InitPic, %function
HEVC_InitPic:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 53248
	bl	_mcount
	ldr	w1, [x20, 1836]
	tbnz	w1, #31, .L2931
	ldr	w2, [x19, 156]
	mov	w0, 0
	cmp	w1, w2
	bge	.L2945
	ldr	x3, [x19, 2336]
	mov	w28, 2024
	add	x0, x19, 256
	mov	w2, 26164
	ldr	x5, [x19, 2328]
	smaddl	x28, w1, w28, x3
	ldp	w25, w22, [x0]
	ldp	w23, w24, [x0, 8]
	ldr	w4, [x28, 32]
	mov	w1, 23196
	ldr	x0, [x19, 28136]
	ldr	x7, [x19, 2320]
	smull	x27, w4, w2
	ldr	w2, [x0, 100]
	add	x26, x5, x27
	str	w2, [x29, 120]
	ldr	w2, [x0, 108]
	str	w2, [x29, 108]
	ldr	w2, [x26, 52]
	ldr	x3, [x19, 28128]
	umull	x1, w2, w1
	ldr	w3, [x3, 8]
	add	x1, x7, x1
	ldr	w2, [x1, 8]
	cmp	w3, w2
	bne	.L2900
	ldrb	w2, [x1, 2]
	cbz	w2, .L2957
.L2900:
	ldr	x0, .LC863
	mov	w3, 23196
	strb	wzr, [x1, 2]
	mov	x21, -23196
	str	x5, [x29, 112]
	ldr	x7, [x0, 104]
	ldr	w0, [x19, 148]
	ldr	x2, [x19, 2320]
	str	w3, [x29, 124]
	smaddl	x0, w0, w3, x21
	add	x0, x2, x0
	mov	x2, 23196
	blr	x7
	ldr	w0, [x19, 148]
	ldr	w3, [x29, 124]
	ldr	x1, [x19, 2320]
	ldr	x5, [x29, 112]
	smaddl	x0, w0, w3, x21
	add	x0, x1, x0
	str	x0, [x19, 28128]
	ldr	x0, [x19, 28136]
.L2901:
	ldr	w2, [x0, 60]
	ldr	w1, [x26, 60]
	cmp	w2, w1
	beq	.L2958
.L2902:
	ldr	x0, .LC863
	mov	w4, 26164
	strb	wzr, [x5, x27]
	mov	x27, -26164
	str	w4, [x29, 124]
	ldr	x5, [x0, 104]
	ldr	x0, [x19, 28136]
	ldr	w1, [x26, 236]
	ldr	w2, [x0, 236]
	ldr	w0, [x19, 152]
	cmp	w2, w1
	ldr	x1, [x19, 2328]
	mov	x2, 26164
	cset	w21, ne
	smaddl	x0, w0, w4, x27
	add	x0, x1, x0
	mov	x1, x26
	blr	x5
	ldr	w0, [x19, 152]
	ldr	w4, [x29, 124]
	ldr	x1, [x19, 2328]
	smaddl	x0, w0, w4, x27
	add	x0, x1, x0
	str	x0, [x19, 28136]
	ldr	w1, [x0, 60]
	str	w1, [x19, 256]
	ldr	w1, [x0, 72]
	str	w1, [x19, 260]
	ldr	w1, [x0, 76]
	str	w1, [x19, 264]
	ldr	w1, [x0, 148]
	str	w1, [x19, 268]
.L2903:
	ldr	w1, [x0, 108]
	ldr	w0, [x0, 100]
	cmp	w0, w1
	csel	w0, w0, w1, cs
	ldr	x1, [x19, 2344]
	str	w0, [x1, 884]
	ldr	x0, [x19, 28144]
	ldr	w1, [x0, 28]
	ldr	w0, [x28, 28]
	cmp	w1, w0
	beq	.L2959
.L2904:
	ldrb	w0, [x28, 13]
	strb	wzr, [x28, 1]
	cbz	w0, .L2906
	ldrb	w0, [x28, 15]
	cbnz	w0, .L2960
	ldr	w0, [x28, 68]
	ldr	w1, [x26, 240]
	sub	w2, w0, #1
	add	x3, x28, x0, sxtw 1
	cmp	w2, wzr
	strh	w1, [x3, 174]
	ble	.L2918
	sub	w4, w0, #2
	add	x0, x28, 92
	add	x4, x4, 47
	add	x4, x28, x4, lsl 1
	.p2align 2
.L2917:
	ldrsh	w2, [x0]
	add	x0, x0, 2
	strh	w2, [x0, 82]
	cmp	x0, x4
	ldrh	w1, [x3, 174]
	sub	w1, w1, w2
	strh	w1, [x3, 174]
	bne	.L2917
.L2918:
	ldr	w0, [x28, 72]
	ldr	w1, [x26, 244]
	sub	w2, w0, #1
	add	x3, x28, x0, sxtw 1
	cmp	w2, wzr
	strh	w1, [x3, 214]
	ble	.L2913
	sub	w4, w0, #2
	add	x0, x28, 132
	add	x4, x4, 67
	add	x4, x28, x4, lsl 1
	.p2align 2
.L2919:
	ldrsh	w2, [x0]
	add	x0, x0, 2
	strh	w2, [x0, 82]
	cmp	x4, x0
	ldrh	w1, [x3, 214]
	sub	w1, w1, w2
	strh	w1, [x3, 214]
	bne	.L2919
	b	.L2913
	.p2align 3
.L2906:
	ldr	w0, [x26, 240]
	strh	w0, [x28, 176]
	ldr	w0, [x26, 244]
	strh	w0, [x28, 216]
.L2913:
	ldr	x2, .LC863
	mov	x1, x28
	ldr	w0, [x19, 156]
	mov	w27, 2024
	mov	x28, -2024
	ldr	x4, [x2, 104]
	smaddl	x0, w0, w27, x28
	ldr	x2, [x19, 2336]
	add	x0, x2, x0
	mov	x2, 2024
	blr	x4
	ldr	w0, [x19, 156]
	ldr	x1, [x19, 2336]
	smaddl	x0, w0, w27, x28
	add	x0, x1, x0
	str	x0, [x19, 28144]
	ldr	w0, [x26, 60]
	cmp	w0, w25
	ccmp	w21, wzr, 0, eq
	bne	.L2920
	ldr	w0, [x19, 168]
	cbnz	w0, .L2961
	.p2align 2
.L2920:
	mov	w4, w21
	mov	w3, w24
	mov	w2, w23
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_GetReRangeFlag
	cbz	w0, .L2923
	ldr	x0, [x19, 2344]
.L2921:
	ldr	w0, [x0, 888]
	cmp	w0, 1
	beq	.L2962
.L2925:
	add	x0, x19, 86016
	ldr	w0, [x0, 916]
	cmp	w0, 1
	beq	.L2963
.L2926:
	mov	w1, 1
	mov	x0, x19
	bl	HEVC_InitDPB
	cmn	w0, #2
	beq	.L2945
	cbnz	w0, .L2927
	ldr	x0, [x19, 2344]
.L2924:
	str	wzr, [x0, 888]
.L2922:
	mov	x0, x19
	bl	HEVC_AllocFrameStore
	mov	w21, w0
	cbnz	w0, .L2964
	ldr	w0, [x20, 1808]
	mov	x3, -1
	ldr	w1, [x20, 1832]
	str	w1, [x20, 1076]
	mov	w1, 1
	str	w0, [x20, 1080]
	mov	w0, -1
	strb	w1, [x20, 1073]
	str	w0, [x20, 1104]
	add	x0, x20, 1136
	ldr	x1, [x19, 28136]
	ldr	w2, [x1, 72]
	str	w2, [x20, 1088]
	ldr	w2, [x1, 76]
	str	w2, [x20, 1092]
	ldr	w2, [x1, 244]
	ldr	w1, [x1, 240]
	str	wzr, [x20, 1100]
	mul	w1, w2, w1
	str	w1, [x20, 1096]
	ldr	x1, [x19, 2344]
	ldr	x2, [x1, 56]
	str	x2, [x20, 1736]
	ldr	x2, [x1, 72]
	str	x2, [x20, 1744]
	ldr	x2, [x1, 88]
	str	x2, [x20, 1752]
	ldr	w2, [x1, 104]
	str	w2, [x20, 1144]
	ldr	w2, [x1, 108]
	str	w2, [x20, 1148]
	ldr	w2, [x1, 112]
	str	w2, [x20, 1152]
	ldr	w2, [x1, 128]
	str	w2, [x20, 1156]
	str	x3, [x1, 56]
	ldr	x1, [x19, 28136]
	ldr	w2, [x1, 72]
	str	w2, [x20, 1248]
	ldr	w2, [x1, 76]
	str	w2, [x20, 1252]
	ldr	w3, [x1, 64]
	ldr	w2, [x1, 2660]
	cmp	w3, 1
	ubfiz	w3, w2, 5, 3
	cset	w2, ne
	orr	w2, w3, w2, lsl 2
	str	w2, [x20, 1244]
	ldr	w1, [x1, 2644]
	and	w1, w1, 7
	bl	SetAspectRatio
	str	wzr, [x20, 1232]
	ldr	x0, [x20, 1128]
	ldr	w1, [x20, 1808]
	str	w1, [x0, 12]
	ldr	x0, [x20, 1128]
	ldr	w1, [x20, 1244]
	str	w1, [x0, 188]
	ldr	x0, [x20, 1128]
	ldr	w1, [x20, 1232]
	str	w1, [x0, 176]
	ldr	x0, [x20, 1128]
	ldr	w1, [x20, 1140]
	str	w1, [x0, 84]
	ldr	x0, [x20, 1128]
	ldr	w1, [x20, 1136]
	str	w1, [x0, 80]
	ldr	x0, [x20, 1128]
	ldr	w1, [x20, 1248]
	str	w1, [x0, 192]
	ldr	x0, [x20, 1128]
	ldr	w1, [x20, 1252]
	str	w1, [x0, 196]
	ldr	x1, [x20, 1128]
	ldr	w0, [x19, 244]
	ldr	w1, [x1, 32]
	bl	FSP_GetDispPhyFs
	cbz	x0, .L2929
	ldr	w3, [x20, 1232]
	mov	w1, 0
	ldr	w2, [x0, 8]
	cmp	w3, wzr
	ldr	w0, [x19, 244]
	cset	w3, ne
	bl	FSP_SetStoreType
.L2929:
	ldr	x1, [x19, 28136]
	mov	w0, 0
	ldr	x5, [x20, 1128]
	ldr	w2, [x1, 84]
	str	w2, [x20, 1468]
	ldr	w4, [x1, 88]
	str	w4, [x20, 1472]
	ldr	w7, [x1, 92]
	str	w7, [x20, 1476]
	ldr	w6, [x1, 96]
	str	w6, [x20, 1480]
	ldr	w3, [x1, 72]
	sub	w2, w3, w2
	sub	w2, w2, w4
	str	w2, [x20, 1256]
	ldr	w4, [x1, 76]
	sub	w3, w4, w7
	sub	w3, w3, w6
	str	w3, [x20, 1260]
	ldr	w3, [x1, 64]
	str	w3, [x20, 1308]
	ldr	w3, [x1, 100]
	str	w3, [x20, 1312]
	ldr	w1, [x1, 108]
	str	w1, [x20, 1316]
	ldr	w1, [x5, 208]
	str	w1, [x20, 1264]
	ldr	w1, [x5, 212]
	str	w1, [x20, 1268]
	str	w2, [x5, 200]
	ldr	x1, [x20, 1128]
	ldr	w2, [x20, 1260]
	str	w2, [x1, 204]
	ldr	x1, [x20, 1128]
	ldr	w2, [x20, 1264]
	str	w2, [x1, 208]
	ldr	x1, [x20, 1128]
	ldr	w2, [x20, 1268]
	str	w2, [x1, 212]
	ldr	x1, [x20, 1128]
	ldr	w2, [x20, 1312]
	str	w2, [x1, 256]
	ldr	x1, [x20, 1128]
	ldr	w2, [x20, 1316]
	str	w2, [x1, 260]
	str	wzr, [x20, 1084]
.L2945:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2923:
	ldr	x0, [x19, 2344]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	bne	.L2924
	b	.L2921
	.p2align 3
.L2960:
	ldr	w3, [x28, 68]
	add	x4, x28, 176
	mov	w2, 0
	cmp	w3, wzr
	ble	.L2912
	.p2align 2
.L2947:
	add	w2, w2, 1
	ldr	w0, [x26, 240]
	sub	w1, w2, #1
	cmp	w2, w3
	mul	w1, w1, w0
	mul	w0, w2, w0
	udiv	w1, w1, w3
	udiv	w0, w0, w3
	sub	w0, w0, w1
	strh	w0, [x4], 2
	bne	.L2947
.L2912:
	ldr	w3, [x28, 72]
	add	x4, x28, 216
	mov	w2, 0
	cmp	w3, wzr
	ble	.L2913
	.p2align 2
.L2946:
	add	w2, w2, 1
	ldr	w0, [x26, 244]
	sub	w1, w2, #1
	cmp	w2, w3
	mul	w1, w1, w0
	mul	w0, w2, w0
	udiv	w1, w1, w3
	udiv	w0, w0, w3
	sub	w0, w0, w1
	strh	w0, [x4], 2
	bne	.L2946
	b	.L2913
	.p2align 3
.L2957:
	ldr	w2, [x19, 168]
	cbnz	w2, .L2901
	b	.L2900
	.p2align 3
.L2931:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2964:
	ldr	x1, .LC869
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2959:
	ldrb	w0, [x28, 1]
	cbnz	w0, .L2904
	ldr	w0, [x19, 168]
	cbz	w0, .L2904
	ldr	w0, [x26, 72]
	cmp	w22, w0
	bne	.L2904
	ldr	w0, [x26, 76]
	cmp	w23, w0
	bne	.L2904
	ldr	w1, [x26, 148]
	cmp	w24, w1
	bne	.L2904
	ldr	w1, [x26, 60]
	cmp	w1, w25
	ccmp	w21, wzr, 0, eq
	bne	.L2920
.L2930:
	cmp	w23, w0
	bne	.L2920
	ldr	x0, [x19, 2344]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	bne	.L2922
	b	.L2921
	.p2align 3
.L2958:
	ldrb	w1, [x5, x27]
	cbnz	w1, .L2902
	ldr	w1, [x19, 168]
	cbz	w1, .L2902
	ldr	w1, [x26, 72]
	cmp	w22, w1
	bne	.L2902
	ldr	w1, [x26, 76]
	cmp	w23, w1
	bne	.L2902
	ldr	w1, [x26, 148]
	cmp	w24, w1
	bne	.L2902
	mov	w21, 0
	b	.L2903
	.p2align 3
.L2961:
	ldr	w0, [x26, 72]
	cmp	w22, w0
	bne	.L2920
	ldr	w0, [x26, 76]
	b	.L2930
	.p2align 3
.L2963:
	mov	x0, x19
	bl	HEVC_GetBackPicFromVOQueue
	b	.L2926
	.p2align 3
.L2962:
	ldr	x1, .LC865
	mov	w0, 31
	ldr	w3, [x26, 100]
	ldr	w5, [x26, 108]
	ldr	w4, [x29, 108]
	ldr	w2, [x29, 120]
	bl	dprint_vfmw
	b	.L2925
.L2927:
	ldr	x1, .LC867
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_ClearAll
	mov	w0, -1
	b	.L2945
	.size	HEVC_InitPic, .-HEVC_InitPic
	.align	3
.LC863:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC865:
	.xword	.LC864
	.align	3
.LC867:
	.xword	.LC866
	.align	3
.LC869:
	.xword	.LC868
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_OutputFrmToVO
	.type	HEVC_OutputFrmToVO, %function
HEVC_OutputFrmToVO:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_GetImagePara
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_CheckFrameStore
	mov	w3, w0
	cbz	w0, .L2974
	cmn	w0, #3
	mov	w0, 0
	bne	.L2975
.L2968:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2974:
	ldr	w0, [x19, 244]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 244]
	ldr	w1, [x20, 32]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L2976
	mov	x4, x0
	add	x3, x19, 61440
	ldr	w0, [x19, 244]
	add	x3, x3, 1032
	mov	x2, x19
	mov	w1, 16
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L2977
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L2978
	str	wzr, [x21, 24]
.L2971:
	ldr	w1, [x19, 276]
	mov	w0, 0
	ldr	w2, [x19, 284]
	add	w1, w1, 1
	str	w1, [x19, 276]
	add	w2, w2, 1
	str	w2, [x19, 284]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2978:
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.3
	b	.L2971
	.p2align 3
.L2975:
	ldr	x1, .LC876
	mov	w2, 9561
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x20, 236]
	mov	x0, x19
	bl	HEVC_DECRecycleImage
	mov	w0, -1
	b	.L2968
	.p2align 3
.L2977:
	ldr	x1, .LC874
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 244]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	mov	w1, 1
	mov	x0, x19
	bl	HEVC_ClearAll
	mov	w0, -1
	b	.L2968
.L2976:
	ldr	x2, .LC870
	mov	w0, 0
	ldr	x1, .LC872
	add	x2, x2, 112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2968
	.size	HEVC_OutputFrmToVO, .-HEVC_OutputFrmToVO
	.align	3
.LC870:
	.xword	.LANCHOR5
	.align	3
.LC872:
	.xword	.LC871
	.align	3
.LC874:
	.xword	.LC873
	.align	3
.LC876:
	.xword	.LC875
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DirectOutput
	.type	HEVC_DirectOutput, %function
HEVC_DirectOutput:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 53248
	bl	_mcount
	mov	w4, 2
	mov	w3, 1
	ldr	x19, [x20, 1128]
	add	x1, x20, 1136
	mov	x2, 624
	add	x0, x19, 80
	str	w4, [x19, 4]
	strb	wzr, [x19, 1]
	strb	w3, [x19, 2]
	ldr	w3, [x20, 1084]
	str	w3, [x19, 16]
	ldr	w3, [x20, 1112]
	str	w3, [x19, 20]
	bl	memcpy
	str	wzr, [x19, 52]
	ldr	w1, [x19, 32]
	mov	w2, 0
	ldr	w0, [x20, 1112]
	str	w0, [x19, 44]
	ldr	w0, [x21, 244]
	bl	FSP_SetRef
	mov	x1, x19
	mov	x0, x21
	bl	HEVC_OutputFrmToVO
	str	wzr, [x19, 4]
	ldr	x3, .LC877
	mov	w21, w0
	mov	x2, 688
	mov	w1, 0
	add	x0, x20, 1072
	ldr	x3, [x3, 96]
	blr	x3
	mov	w0, w21
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_DirectOutput, .-HEVC_DirectOutput
	.align	3
.LC877:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_IModeProcess
	.type	HEVC_IModeProcess, %function
HEVC_IModeProcess:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	HEVC_DirectOutput
	add	x2, x19, 53248
	ldr	w3, [x19, 180]
	ldr	w1, [x2, 1108]
	cmp	w1, w3
	beq	.L2981
	add	x19, x19, x1, uxtw 2
	str	wzr, [x19, 2116]
	str	w3, [x2, 1108]
.L2981:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IModeProcess, .-HEVC_IModeProcess
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_OutputFrmFromDPB
	.type	HEVC_OutputFrmFromDPB, %function
HEVC_OutputFrmFromDPB:
	sub	sp, sp, #112
	stp	x29, x30, [sp, 16]
	add	x29, sp, 16
	stp	x19, x20, [sp, 32]
	stp	x21, x22, [sp, 48]
	stp	x23, x24, [sp, 64]
	stp	x25, x26, [sp, 80]
	stp	x27, x28, [sp, 96]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 2684]
	cbz	w0, .L2994
	sub	w3, w0, #1
	ldr	w5, [x19, 212]
	add	x3, x3, 339
	add	x0, x19, 2704
	mov	w22, 0
	add	x3, x19, x3, lsl 3
	.p2align 2
.L2985:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cbnz	w2, .L2984
	ldr	w1, [x1, 12]
	cmp	w1, w5
	csinc	w22, w22, w22, le
.L2984:
	cmp	x3, x0
	bne	.L2985
	add	x24, x19, 53248
	ldr	x1, [x19, 28136]
	ldr	x26, .LC880
	mov	w20, 0
	ldr	x25, .LC879
	mov	w28, 0
	ldr	w0, [x24, 1828]
	add	x26, x26, 136
	ldr	x27, .LC882
	add	x0, x1, x0, lsl 2
	ldr	w23, [x0, 412]
	.p2align 2
.L2993:
	add	x21, x19, x20, uxtw 3
	ldrb	w9, [x19, 2]
	ldr	w8, [x24, 1808]
	mov	x1, x25
	mov	w7, w23
	mov	w6, w22
	ldr	x3, [x21, 2704]
	mov	w2, w20
	mov	w0, 22
	ldr	w4, [x3, 12]
	ldrb	w3, [x3, 2]
	str	w8, [sp]
	str	w9, [sp, 8]
	bl	dprint_vfmw
	ldr	x0, [x21, 2704]
	ldrb	w1, [x0, 2]
	cbnz	w1, .L2987
	ldr	w2, [x0, 12]
	cmp	w23, w22
	ldr	w3, [x19, 212]
	ccmp	w2, w3, 4, cc
	ble	.L2988
	ldr	w4, [x24, 1808]
	cmp	w2, w4
	blt	.L2989
	ldrb	w2, [x19, 2]
	cbnz	w2, .L2989
	.p2align 2
.L2988:
	cbnz	w3, .L2990
	ldr	w4, [x0, 12]
	tbz	w4, #31, .L2990
	cbz	w1, .L3005
.L2991:
	mov	x2, x26
	mov	x1, x27
	mov	w0, 31
	bl	dprint_vfmw
.L2990:
	ldr	w0, [x19, 2684]
	add	w20, w20, 1
	cmp	w0, w20
	bls	.L3003
.L2992:
	ldr	w5, [x19, 212]
	b	.L2993
	.p2align 3
.L2989:
	mov	w1, 1
	strb	w1, [x0, 2]
	mov	x0, x19
	add	w20, w20, 1
	ldr	x1, [x21, 2704]
	sub	w22, w22, #1
	bl	HEVC_OutputFrmToVO
	mov	w28, w0
	ldr	x0, [x21, 2704]
	ldr	w0, [x0, 12]
	str	w0, [x19, 212]
	ldr	w0, [x19, 2684]
	cmp	w0, w20
	bhi	.L2992
.L3003:
	add	sp, x29, 0
	mov	w0, w28
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2987:
	ldr	w3, [x19, 212]
	b	.L2988
	.p2align 3
.L3005:
	ldr	w1, [x0, 32]
	mov	w2, 0
	ldr	w0, [x19, 244]
	bl	FSP_SetDisplay
	ldr	x0, [x21, 2704]
	ldr	w3, [x19, 212]
	ldr	w4, [x0, 12]
	b	.L2991
.L2994:
	mov	w28, 0
	b	.L3003
	.size	HEVC_OutputFrmFromDPB, .-HEVC_OutputFrmFromDPB
	.align	3
.LC879:
	.xword	.LC878
	.align	3
.LC880:
	.xword	.LANCHOR5
	.align	3
.LC882:
	.xword	.LC881
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_OutputOneFrmFromDPB
	.type	HEVC_OutputOneFrmFromDPB, %function
HEVC_OutputOneFrmFromDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w1
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, x21, uxtw 3
	bl	_mcount
	mov	w2, 1
	mov	x0, x19
	ldr	x1, [x20, 2704]
	strb	w2, [x1, 2]
	ldr	x1, [x20, 2704]
	bl	HEVC_OutputFrmToVO
	mov	w22, w0
	ldr	x0, [x20, 2704]
	ldr	w1, [x0, 12]
	str	w1, [x19, 212]
	ldrb	w0, [x0, 1]
	cbnz	w0, .L3007
	mov	w1, w21
	mov	x0, x19
	bl	HEVC_RemoveApcStore
	mov	w1, w21
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
.L3007:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_OutputOneFrmFromDPB, .-HEVC_OutputOneFrmFromDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_OutputUnRefPicWithMinPoc
	.type	HEVC_OutputUnRefPicWithMinPoc, %function
HEVC_OutputUnRefPicWithMinPoc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x29, 44
	mov	x0, x19
	bl	HEVC_GetUnRefPicWithMinPoc
	cbnz	w0, .L3013
	ldr	w1, [x29, 44]
	mov	x0, x19
	bl	HEVC_OutputOneFrmFromDPB
	mov	w1, 0
	cbnz	w0, .L3014
.L3010:
	ldr	x19, [sp, 16]
	mov	w0, w1
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3013:
	ldr	x2, .LC884
	mov	w0, 1
	ldr	x1, .LC886
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w1, -1
	b	.L3010
	.p2align 3
.L3014:
	ldr	x2, .LC884
	mov	w0, 1
	ldr	x1, .LC888
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w1, -1
	b	.L3010
	.size	HEVC_OutputUnRefPicWithMinPoc, .-HEVC_OutputUnRefPicWithMinPoc
	.align	3
.LC884:
	.xword	.LANCHOR5
	.align	3
.LC886:
	.xword	.LC885
	.align	3
.LC888:
	.xword	.LC887
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_FlushOutputFrmFromDPB
	.type	HEVC_FlushOutputFrmFromDPB, %function
HEVC_FlushOutputFrmFromDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L3024
	ldr	w0, [x19, 2684]
	mov	w20, 0
	mov	w22, 1
	cbnz	w0, .L3027
	b	.L3023
	.p2align 3
.L3031:
	strb	w22, [x1, 2]
	ldr	x1, [x21, 2704]
	bl	HEVC_OutputFrmToVO
	cmn	w0, #1
	mov	w21, w0
	beq	.L3030
.L3020:
	ldr	w0, [x19, 2684]
	cmp	w0, w20
	bls	.L3023
.L3027:
	add	x21, x19, x20, uxtw 3
	ldr	w0, [x19, 244]
	mov	w2, 0
	add	w20, w20, 1
	ldr	x1, [x21, 2704]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x1, [x21, 2704]
	mov	x0, x19
	ldrb	w2, [x1, 2]
	cbz	w2, .L3031
	ldr	w1, [x1, 32]
	ldr	w0, [x19, 244]
	bl	FSP_GetDisplay
	cmp	w0, 3
	mov	w2, 0
	beq	.L3020
	ldr	x1, [x21, 2704]
	ldr	w0, [x19, 244]
	ldr	w1, [x1, 32]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 2684]
	cmp	w0, w20
	bhi	.L3027
.L3023:
	mov	w1, -2147483647
	mov	x0, x19
	str	w1, [x19, 212]
	mov	w21, 0
	bl	Hevc_InitDecBuffers
.L3016:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3030:
	ldr	x2, .LC889
	mov	w0, 0
	ldr	x1, .LC891
	add	x2, x2, 192
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L3024:
	mov	w21, -1
	b	.L3016
	.size	HEVC_FlushOutputFrmFromDPB, .-HEVC_FlushOutputFrmFromDPB
	.align	3
.LC889:
	.xword	.LANCHOR5
	.align	3
.LC891:
	.xword	.LC890
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ReceivePacket
	.type	HEVC_ReceivePacket, %function
HEVC_ReceivePacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 86016
	mov	x20, x1
	bl	_mcount
	str	wzr, [x21, 888]
	ldr	x0, [x19, 2360]
	cbz	x0, .L3054
	ldrb	w1, [x20]
	str	w1, [x0, 40]
	ldr	x0, [x19, 2344]
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L3055
.L3038:
	ldrb	w0, [x20]
	cmp	w0, 1
	bne	.L3039
	ldr	x1, [x19, 2360]
	ldr	w0, [x1, 8]
	cmp	w0, 1
	bls	.L3040
	ldr	w2, [x1, 40]
	cmp	w2, 1
	beq	.L3039
.L3040:
	ldr	w2, [x20, 20]
	cmp	w2, wzr
	ble	.L3041
	ldr	x3, [x20, 8]
	cbz	x3, .L3041
	ldr	w2, [x20, 16]
	cbnz	w2, .L3056
.L3041:
	mov	w0, 1
	str	w0, [x1, 40]
	ldr	w0, [x19, 244]
	ldr	w1, [x20, 24]
	bl	SM_ReleaseStreamSeg
.L3037:
	ldr	x0, [x19, 2360]
	ldr	w0, [x0, 40]
	cmp	w0, wzr
	csetm	w0, eq
.L3036:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3055:
	ldr	w0, [x0, 872]
	cbnz	w0, .L3037
	b	.L3038
	.p2align 3
.L3056:
	ubfiz	x2, x0, 5, 32
	mov	x0, x19
	add	x1, x1, x2
	str	x3, [x1, 48]
	ldr	x2, [x19, 2360]
	ldr	w3, [x20, 20]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	w3, [x1, 56]
	ldr	x2, [x19, 2360]
	ldr	w3, [x20, 16]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	w3, [x1, 60]
	ldr	x2, [x19, 2360]
	ldr	w3, [x20, 24]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	w3, [x1, 76]
	ldr	x2, [x19, 2360]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	wzr, [x1, 68]
	ldr	x2, [x19, 2360]
	ldr	w1, [x2, 8]
	add	w1, w1, 1
	str	w1, [x2, 8]
	bl	HEVC_CombinePacket
	mov	w0, 1
	str	w0, [x21, 888]
	b	.L3037
	.p2align 3
.L3054:
	ldr	w0, [x20, 20]
	cmp	w0, wzr
	ble	.L3034
	ldr	x0, [x20, 8]
	cbz	x0, .L3034
	ldr	w0, [x20, 16]
	cbz	w0, .L3034
	mov	x0, x19
	bl	HEVC_FindNaluArraySlot
	cmn	w0, #1
	mov	w21, w0
	beq	.L3057
	add	x1, x19, 65536
	sxtw	x0, w0
	lsl	x21, x0, 3
	mov	w2, 1
	sub	x0, x21, x0
	ldr	x21, [x1, 2856]
	add	x0, x21, x0, lsl 4
	str	x0, [x19, 2360]
	ldr	x1, [x20, 8]
	str	x1, [x0, 48]
	ldr	x0, [x19, 2360]
	ldr	w1, [x20, 20]
	str	w1, [x0, 56]
	ldr	x0, [x19, 2360]
	ldr	w1, [x20, 24]
	str	w1, [x0, 76]
	ldr	x0, [x19, 2360]
	ldr	w1, [x20, 16]
	str	w1, [x0, 60]
	ldr	x0, [x19, 2360]
	str	wzr, [x0, 68]
	ldr	x0, [x19, 2360]
	str	w2, [x0, 8]
	ldr	x0, [x19, 2360]
	strb	wzr, [x0, 1]
	ldr	x0, [x19, 2360]
	ldrb	w1, [x20]
	str	w1, [x0, 40]
	b	.L3037
	.p2align 3
.L3039:
	ldr	x1, .LC897
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3036
	.p2align 3
.L3034:
	ldr	x1, .LC895
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3036
.L3057:
	ldr	x1, .LC893
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_ClearAll
	mov	w0, w21
	b	.L3036
	.size	HEVC_ReceivePacket, .-HEVC_ReceivePacket
	.align	3
.LC893:
	.xword	.LC892
	.align	3
.LC895:
	.xword	.LC894
	.align	3
.LC897:
	.xword	.LC896
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_GetImageBuffer
	.type	HEVCDEC_GetImageBuffer, %function
HEVCDEC_GetImageBuffer:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 176]
	cbz	w0, .L3061
	add	x0, x19, 53248
	ldrb	w0, [x0, 1073]
	cmp	w0, 1
	beq	.L3061
	add	x1, x19, 65536
	add	x4, x19, 81920
	add	x1, x1, 3672
	add	x4, x4, 1528
	.p2align 2
.L3067:
	ldr	w0, [x1, 4]
	cbnz	w0, .L3062
	cbz	x1, .L3063
	ldr	w3, [x19, 2684]
	cbz	w3, .L3063
	ldr	x0, [x19, 2704]
	cmp	x0, x1
	beq	.L3062
	sub	w3, w3, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L3064
	.p2align 3
.L3065:
	ldr	x2, [x0, 2704]
	cmp	x2, x1
	beq	.L3062
.L3064:
	cmp	x3, x0
	add	x0, x0, 8
	bne	.L3065
.L3063:
	ldr	w0, [x19, 244]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L3061
	cmn	w0, #1
	beq	.L3086
.L3085:
	mov	w0, 0
.L3087:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3061:
	ldr	x19, [sp, 16]
	mov	w0, 1
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3062:
	add	x1, x1, 712
	cmp	x1, x4
	bne	.L3067
	add	x0, x19, 61440
	add	x2, x29, 44
	add	x1, x29, 40
	add	x0, x0, 1032
	bl	GetQueueImgNum
	mov	x0, x19
	bl	HEVC_PrintFrameStoreState
	ldr	x1, .LC899
	mov	w0, 0
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_PrintDPBState
	ldr	w0, [x19, 176]
	ldr	w2, [x19, 2684]
	ldp	w3, w4, [x29, 40]
	sub	w0, w0, #2
	sub	w0, w0, w2
	add	w1, w3, w4
	cmp	w1, w0
	bge	.L3085
	ldr	x2, .LC900
	mov	w0, 0
	ldr	x1, .LC902
	add	x2, x2, 224
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x19
	bl	HEVC_ClearAll
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
.L3086:
	add	x1, x19, 61440
	ldr	w0, [x19, 244]
	add	x1, x1, 1032
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	b	.L3087
	.size	HEVCDEC_GetImageBuffer, .-HEVCDEC_GetImageBuffer
	.align	3
.LC899:
	.xword	.LC898
	.align	3
.LC900:
	.xword	.LANCHOR5
	.align	3
.LC902:
	.xword	.LC901
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_FlushDPB
	.type	HEVC_FlushDPB, %function
HEVC_FlushDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, 0
	bl	_mcount
	ldr	w2, [x19, 2684]
	cbnz	w2, .L3100
	b	.L3090
	.p2align 3
.L3091:
	cmp	w2, w20
	bls	.L3090
.L3100:
	add	x0, x19, x20, uxtw 3
	add	w20, w20, 1
	ldr	x0, [x0, 2704]
	ldrb	w1, [x0, 1]
	cbz	w1, .L3091
	strb	wzr, [x0, 1]
	mov	w2, 0
	ldr	w1, [x0, 32]
	ldr	w0, [x19, 244]
	bl	FSP_SetRef
	ldr	w2, [x19, 2684]
	cmp	w2, w20
	bhi	.L3100
	.p2align 2
.L3090:
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cbz	w0, .L3094
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cbnz	w0, .L3090
	b	.L3094
	.p2align 3
.L3096:
	bl	HEVC_GetMinPOC
	ldr	w1, [x29, 40]
	mov	x0, x19
	bl	HEVC_OutputOneFrmFromDPB
	cmn	w0, #1
	beq	.L3095
.L3094:
	ldr	w3, [x19, 2684]
	add	x2, x29, 40
	add	x1, x29, 44
	mov	x0, x19
	cbnz	w3, .L3096
	mov	w0, 0
.L3095:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_FlushDPB, .-HEVC_FlushDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_ClearDPB
	.type	HEVC_ClearDPB, %function
HEVC_ClearDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	HEVC_FlushDPB
	cbz	w0, .L3106
	ldr	x2, .LC903
	mov	w3, w0
	ldr	x1, .LC905
	mov	w0, 0
	add	x2, x2, 248
	bl	dprint_vfmw
.L3106:
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_InitDPB
	ldr	x19, [sp, 16]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_ClearDPB, .-HEVC_ClearDPB
	.align	3
.LC903:
	.xword	.LANCHOR5
	.align	3
.LC905:
	.xword	.LC904
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RecoverDPBProcess
	.type	HEVC_RecoverDPBProcess, %function
HEVC_RecoverDPBProcess:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC906
	ldr	x22, .LC908
	add	x21, x20, 264
	b	.L3111
	.p2align 3
.L3113:
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_IncreaseDPBSize
	cbnz	w0, .L3121
.L3111:
	ldr	w5, [x19, 2680]
	mov	x2, x21
	ldr	w6, [x19, 2684]
	mov	x1, x22
	mov	w0, 13
	mov	w4, w5
	cmp	w6, w5
	mov	w3, w6
	bcs	.L3113
.L3110:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3121:
	ldr	x1, .LC910
	mov	x2, x21
	mov	w0, 1
	add	x20, x20, 264
	bl	dprint_vfmw
	ldr	x21, .LC912
	.p2align 2
.L3112:
	ldr	w5, [x19, 2680]
	mov	x2, x20
	ldr	w6, [x19, 2684]
	mov	x1, x21
	mov	w0, 1
	mov	w4, w5
	cmp	w6, w5
	mov	w3, w6
	bcc	.L3110
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_OutputUnRefPicWithMinPoc
	cbz	w0, .L3112
	ldr	x1, .LC913
	mov	x2, x20
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_PrintDPBState
	mov	x0, x19
	bl	HEVC_ClearDPB
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_RecoverDPBProcess, .-HEVC_RecoverDPBProcess
	.align	3
.LC906:
	.xword	.LANCHOR5
	.align	3
.LC908:
	.xword	.LC907
	.align	3
.LC910:
	.xword	.LC909
	.align	3
.LC912:
	.xword	.LC911
	.align	3
.LC913:
	.xword	.LC887
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InsertFrmInDPB
	.type	HEVC_InsertFrmInDPB, %function
HEVC_InsertFrmInDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w22, w1
	mov	x19, x2
	bl	_mcount
	cmp	w22, 16
	bhi	.L3126
	add	x20, x21, x22, uxtw 3
	ldr	x0, [x19, 56]
	mov	w1, 1
	mov	w2, w1
	str	x0, [x20, 2704]
	str	w1, [x0, 4]
	ldr	x1, [x20, 2704]
	ldr	w0, [x21, 244]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldrb	w1, [x19]
	mov	x2, 624
	ldr	x0, [x20, 2704]
	cmp	w1, wzr
	add	x1, x19, 64
	cset	w3, ne
	strb	w3, [x0, 1]
	ldr	x0, [x20, 2704]
	ldr	w3, [x19, 36]
	str	w3, [x0, 24]
	ldr	x0, [x20, 2704]
	add	x0, x0, 80
	bl	memcpy
	ldr	x0, [x20, 2704]
	add	w1, w22, 1
	ldr	w2, [x19, 40]
	ldr	x3, .LC918
	str	w2, [x0, 20]
	str	w2, [x0, 44]
	mov	w2, 8
	ldr	x0, [x20, 2704]
	ldr	x4, [x19, 56]
	str	x4, [x0, 72]
	ldr	x0, [x20, 2704]
	ldr	w4, [x19, 12]
	str	w4, [x0, 16]
	str	w4, [x0, 48]
	add	x0, x21, 2704
	ldr	x4, [x20, 2704]
	ldr	w5, [x19, 8]
	str	w5, [x4, 12]
	ldr	x4, [x20, 2704]
	ldr	w5, [x19, 8]
	str	w5, [x4, 40]
	bl	qsort
	ldr	w1, [x21, 2684]
	mov	x0, x21
	add	w1, w1, 1
	str	w1, [x21, 2684]
	ldr	x1, [x19, 56]
	bl	HEVC_GetAPC
	cbnz	w0, .L3127
	strb	wzr, [x19, 1]
	mov	w0, 0
.L3124:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3126:
	ldr	x2, .LC915
	mov	w3, w22
	ldr	x1, .LC917
	mov	w0, 0
	add	x2, x2, 288
	bl	dprint_vfmw
	mov	x0, x21
	bl	HEVC_ClearDPB
	mov	w0, -1
	b	.L3124
	.p2align 3
.L3127:
	ldr	x2, .LC915
	mov	w3, w0
	ldr	x1, .LC920
	mov	w0, 0
	add	x2, x2, 288
	bl	dprint_vfmw
	mov	x0, x21
	bl	HEVC_PrintDPBState
	mov	x0, x21
	bl	HEVC_ClearDPB
	mov	w0, -7
	b	.L3124
	.size	HEVC_InsertFrmInDPB, .-HEVC_InsertFrmInDPB
	.align	3
.LC915:
	.xword	.LANCHOR5
	.align	3
.LC917:
	.xword	.LC916
	.align	3
.LC918:
	.xword	compare_pic_by_poc_asc
	.align	3
.LC920:
	.xword	.LC919
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_SimpleDPBProcess
	.type	HEVC_SimpleDPBProcess, %function
HEVC_SimpleDPBProcess:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 53248
	bl	_mcount
	ldrb	w0, [x19, 1072]
	cbnz	w0, .L3141
	ldr	x0, [x19, 1128]
	mov	w1, 2
	str	w1, [x0, 4]
.L3130:
	ldr	x1, [x19, 1128]
	mov	w2, 1
	mov	x0, x20
	strb	w2, [x1, 2]
	ldr	x1, [x19, 1128]
	bl	HEVC_CheckFrameStore
	cmn	w0, #1
	mov	w19, w0
	beq	.L3142
	cmn	w0, #3
	beq	.L3134
	mov	x0, x20
	bl	HEVC_OutputCurrPic
	cbnz	w0, .L3143
.L3134:
	mov	w0, 0
.L3131:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3141:
	mov	x0, x20
	bl	HEVC_SimpleSlideDPB
	ldr	w1, [x20, 2684]
	add	x2, x19, 1072
	mov	x0, x20
	bl	HEVC_InsertFrmInDPB
	cbz	w0, .L3130
	ldr	x2, .LC921
	mov	w4, w0
	ldr	x1, .LC923
	mov	w3, 8674
	add	x2, x2, 312
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -7
	b	.L3131
	.p2align 3
.L3143:
	ldr	x2, .LC921
	mov	w0, 0
	ldr	x1, .LC926
	add	x2, x2, 312
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3131
.L3142:
	ldr	x2, .LC921
	mov	w0, 0
	ldr	x1, .LC925
	add	x2, x2, 312
	bl	dprint_vfmw
	mov	w0, w19
	b	.L3131
	.size	HEVC_SimpleDPBProcess, .-HEVC_SimpleDPBProcess
	.align	3
.LC921:
	.xword	.LANCHOR5
	.align	3
.LC923:
	.xword	.LC922
	.align	3
.LC925:
	.xword	.LC924
	.align	3
.LC926:
	.xword	.LC223
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecOrderProcess
	.type	HEVC_DecOrderProcess, %function
HEVC_DecOrderProcess:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 2680]
	ldr	w1, [x19, 2684]
	cmp	w1, w0
	bcs	.L3156
.L3145:
	add	x20, x19, 53248
	mov	x0, x19
	add	x2, x20, 1072
	bl	HEVC_InsertFrmInDPB
	cbnz	w0, .L3157
	ldr	x1, [x20, 1128]
	mov	x0, x19
	bl	HEVC_CheckFrameStore
	cmn	w0, #1
	mov	w20, w0
	beq	.L3158
	cmn	w0, #3
	beq	.L3151
	mov	x0, x19
	bl	HEVC_OutputCurrPic
	cbnz	w0, .L3159
.L3151:
	mov	w0, 0
.L3148:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3156:
	mov	x0, x19
	bl	HEVC_RemoveUnusedApcStore
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	w0, 1
	beq	.L3155
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC929
	add	x2, x2, 336
	bl	dprint_vfmw
.L3155:
	ldr	w1, [x19, 2684]
	b	.L3145
.L3159:
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC933
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3148
	.p2align 3
.L3157:
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC931
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3148
.L3158:
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC932
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, w20
	b	.L3148
	.size	HEVC_DecOrderProcess, .-HEVC_DecOrderProcess
	.align	3
.LC927:
	.xword	.LANCHOR5
	.align	3
.LC929:
	.xword	.LC928
	.align	3
.LC931:
	.xword	.LC930
	.align	3
.LC932:
	.xword	.LC924
	.align	3
.LC933:
	.xword	.LC223
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DispOrderProcess
	.type	HEVC_DispOrderProcess, %function
HEVC_DispOrderProcess:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 61440
	ldr	w0, [x0, 1640]
	cmp	w0, 1
	beq	.L3161
	mov	x0, x19
	bl	HEVC_FirstFrameFastOut
	cbnz	w0, .L3178
.L3161:
	ldr	w0, [x19, 2680]
	ldr	w1, [x19, 2684]
	cmp	w1, w0
	bcs	.L3179
.L3163:
	add	x2, x19, 53248
	mov	x0, x19
	add	x2, x2, 1072
	bl	HEVC_InsertFrmInDPB
	cbnz	w0, .L3180
	ldr	w0, [x19, 2684]
	cbnz	w0, .L3166
.L3167:
	mov	w0, 0
.L3162:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3166:
	mov	x0, x19
	bl	HEVC_OutputFrmFromDPB
	cbz	w0, .L3167
	ldr	x2, .LC934
	mov	w0, 1
	ldr	x1, .LC942
	add	x2, x2, 360
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3162
	.p2align 3
.L3179:
	mov	x0, x19
	bl	HEVC_RemoveUnusedApcStore
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	w0, 1
	beq	.L3177
	ldr	x1, .LC938
	mov	w0, 0
	bl	dprint_vfmw
.L3177:
	ldr	w1, [x19, 2684]
	b	.L3163
	.p2align 3
.L3180:
	ldr	x1, .LC940
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3162
.L3178:
	ldr	x2, .LC934
	mov	w0, 0
	ldr	x1, .LC936
	add	x2, x2, 360
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3162
	.size	HEVC_DispOrderProcess, .-HEVC_DispOrderProcess
	.align	3
.LC934:
	.xword	.LANCHOR5
	.align	3
.LC936:
	.xword	.LC935
	.align	3
.LC938:
	.xword	.LC937
	.align	3
.LC940:
	.xword	.LC939
	.align	3
.LC942:
	.xword	.LC941
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_StorePicInDPB
	.type	HEVC_StorePicInDPB, %function
HEVC_StorePicInDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 596]
	cmp	w0, 1
	beq	.L3215
	add	x20, x19, 53248
	mov	x0, x19
	str	wzr, [x20, 1216]
	str	wzr, [x20, 1220]
	bl	HEVC_RemoveDummyFrame
	ldrb	w0, [x20, 1073]
	cbz	w0, .L3216
.L3185:
	ldr	x0, [x19, 2344]
	ldr	w1, [x20, 1112]
	ldr	w0, [x0, 4]
	cmp	w1, w0
	bhi	.L3217
	ldr	x1, [x19, 2352]
	ldr	x0, [x20, 1128]
	ldr	w1, [x1, 5284]
	str	w1, [x0, 704]
	ldr	x1, [x19, 2352]
	ldr	x0, [x20, 1128]
	ldr	w1, [x1, 5288]
	str	w1, [x0, 708]
	ldr	w0, [x19, 248]
	cmp	w0, 2
	beq	.L3218
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	beq	.L3219
	cmp	w0, 1
	beq	.L3220
	cbnz	w0, .L3199
.L3198:
	mov	x0, x19
	bl	HEVC_DispOrderProcess
	mov	w21, w0
	cbnz	w0, .L3196
.L3200:
	mov	w21, 0
.L3197:
	strb	wzr, [x20, 1073]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3215:
	ldr	x1, .LC943
	add	x20, x19, 53248
	ldr	x1, [x1]
	ldr	w2, [x1, 4180]
	ldr	w1, [x1, 4184]
	str	w0, [x20, 1216]
	mov	x0, x19
	cmp	w1, wzr
	csinc	w1, w1, wzr, ne
	udiv	w1, w2, w1
	str	w1, [x20, 1220]
	bl	HEVC_RemoveDummyFrame
	ldrb	w0, [x20, 1073]
	cbnz	w0, .L3185
.L3216:
	ldr	x2, .LC944
	mov	w0, 1
	ldr	x1, .LC946
	add	x2, x2, 384
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3219:
	mov	x0, x19
	bl	HEVC_SimpleDPBProcess
	mov	w21, w0
	cbnz	w0, .L3221
.L3193:
	ldr	x1, .LC952
	mov	w3, 9003
	mov	w2, w21
	mov	w0, 14
	bl	dprint_vfmw
.L3191:
	cmn	w21, #1
	bne	.L3197
	.p2align 2
.L3199:
	mov	x0, x19
	mov	w21, -1
	bl	HEVC_ClearCurrPic
	ldr	x2, .LC944
	mov	w0, 1
	ldr	x1, .LC960
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3197
	.p2align 3
.L3217:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	ldr	x3, [x19, 2344]
	mov	w0, 1
	ldr	w2, [x20, 1112]
	ldr	x1, .LC948
	ldr	w3, [x3, 4]
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3218:
	mov	x0, x19
	bl	HEVC_DirectOutput
	mov	w21, w0
	ldr	w1, [x19, 180]
	ldr	w0, [x20, 1108]
	cmp	w0, w1
	beq	.L3189
	add	x0, x19, x0, uxtw 2
	str	wzr, [x0, 2116]
	str	w1, [x20, 1108]
.L3189:
	cbnz	w21, .L3222
.L3190:
	ldr	x1, .LC952
	mov	w3, 8988
	mov	w2, w21
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3191
	.p2align 3
.L3220:
	mov	x0, x19
	bl	HEVC_DecOrderProcess
	mov	w21, w0
	cbnz	w0, .L3223
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 12]
	cbnz	w0, .L3200
	b	.L3198
	.p2align 3
.L3196:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC958
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3191
	.p2align 3
.L3223:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC956
	add	x2, x2, 384
	bl	dprint_vfmw
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 12]
	cbnz	w0, .L3191
	b	.L3198
	.p2align 3
.L3222:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC950
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3190
	.p2align 3
.L3221:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC954
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3193
	.size	HEVC_StorePicInDPB, .-HEVC_StorePicInDPB
	.align	3
.LC943:
	.xword	g_HwMem
	.align	3
.LC944:
	.xword	.LANCHOR5
	.align	3
.LC946:
	.xword	.LC945
	.align	3
.LC948:
	.xword	.LC947
	.align	3
.LC950:
	.xword	.LC949
	.align	3
.LC952:
	.xword	.LC951
	.align	3
.LC954:
	.xword	.LC953
	.align	3
.LC956:
	.xword	.LC955
	.align	3
.LC958:
	.xword	.LC957
	.align	3
.LC960:
	.xword	.LC959
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_CreateLostPicture
	.type	HEVC_CreateLostPicture, %function
HEVC_CreateLostPicture:
	sub	sp, sp, #688
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	w23, 0
	add	x19, x20, 65536
	mov	x0, x30
	add	x19, x19, 3672
	mov	w22, w1
	bl	_mcount
	.p2align 2
.L3230:
	ldr	w0, [x19, 4]
	cbnz	w0, .L3225
	cbz	x19, .L3226
	ldr	w2, [x20, 2684]
	cbz	w2, .L3226
	ldr	x0, [x20, 2704]
	cmp	x19, x0
	beq	.L3225
	sub	w2, w2, #1
	mov	x0, x20
	add	x2, x20, x2, lsl 3
	b	.L3228
	.p2align 3
.L3229:
	ldr	x1, [x0, 2704]
	cmp	x19, x1
	beq	.L3225
.L3228:
	cmp	x0, x2
	add	x0, x0, 8
	bne	.L3229
.L3226:
	sxtw	x21, w23
	mov	w24, 1
	lsl	x1, x21, 2
	mov	w4, 101
	sub	x3, x1, x21
	mov	x2, 624
	lsl	x1, x3, 4
	add	x0, x29, 128
	sub	x1, x1, x3
	strb	w24, [x29, 65]
	lsl	x1, x1, x24
	strb	w24, [x29, 64]
	sub	x21, x1, x21
	str	w22, [x29, 72]
	str	w4, [x29, 104]
	add	x21, x20, x21, lsl 3
	add	x21, x21, 65536
	add	x1, x21, 3752
	bl	memcpy
	strb	w24, [x21, 3674]
	strb	w24, [x21, 3672]
	cmp	w23, 19
	str	wzr, [x21, 3680]
	str	x19, [x29, 120]
	str	wzr, [x29, 288]
	bgt	.L3231
	ldr	w5, [x20, 2684]
	cbz	w5, .L3233
	mov	w6, 0
	mov	w3, 2147483647
	mov	w1, 0
	.p2align 2
.L3236:
	add	x0, x20, x1, sxtw 3
	add	w1, w1, 1
	ldr	x4, [x0, 2704]
	ldr	w0, [x4, 12]
	sub	w0, w0, w22
	sxtw	x0, w0
	eor	x2, x0, x0, asr 63
	sub	x2, x2, x0, asr 63
	cmp	w2, w3
	bge	.L3235
	ldrb	w0, [x4, 1]
	cmp	w0, wzr
	csel	w3, w3, w2, eq
	csinc	w6, w6, wzr, eq
.L3235:
	cmp	w1, w5
	bne	.L3236
	cbz	w6, .L3233
	mov	w1, 0
	b	.L3237
	.p2align 3
.L3238:
	cmp	w5, w1
	beq	.L3239
.L3237:
	sxtw	x19, w1
	add	w1, w1, 1
	add	x19, x19, 336
	add	x19, x20, x19, lsl 3
	ldr	x2, [x19, 16]
	ldr	w0, [x2, 12]
	sub	w0, w0, w22
	sxtw	x0, w0
	eor	x4, x0, x0, asr 63
	sub	x4, x4, x0, asr 63
	cmp	w3, w4
	bne	.L3238
	ldrb	w0, [x2, 1]
	cbz	w0, .L3238
	ldr	w1, [x2, 32]
	ldr	w0, [x20, 244]
	bl	FSP_GetLogicFs
	mov	x21, x0
	ldr	x2, [x19, 16]
	mov	w3, w22
	ldr	x1, .LC968
	mov	w0, 1
	ldr	w2, [x2, 12]
	bl	dprint_vfmw
	cbz	x21, .L3239
	ldr	x0, [x21, 640]
	cbz	x0, .L3272
	ldr	w0, [x20, 244]
	mov	w1, 0
	bl	FSP_NewLogicFs
	mov	w1, w0
	mov	w19, w0
	ldr	w0, [x20, 244]
	bl	FSP_GetLogicFs
	mov	x1, x0
	cmp	w19, wzr
	ccmp	x0, xzr, 4, ge
	beq	.L3242
	ldr	x3, [x29, 120]
	add	x2, x29, 64
	mov	x0, x20
	str	w19, [x3, 32]
	ldr	x3, [x21, 640]
	str	x3, [x1, 640]
	ldr	x3, [x21, 648]
	str	x3, [x1, 648]
	ldr	x3, [x21, 656]
	str	x3, [x1, 656]
	ldrsb	w3, [x21, 4]
	ldr	w1, [x20, 2684]
	str	w3, [x29, 100]
	bl	HEVC_InsertFrmInDPB
	cbnz	w0, .L3273
	ldr	w0, [x20, 244]
	mov	w2, 0
	mov	w1, w19
	bl	FSP_SetDisplay
	ldr	w2, [x20, 200]
	mov	w1, 2147483647
	mov	w0, 0
	cmp	w2, w1
	bne	.L3258
	str	w22, [x20, 200]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 688
	ret
	.p2align 3
.L3225:
	add	w23, w23, 1
	add	x19, x19, 712
	cmp	w23, 20
	bne	.L3230
.L3231:
	ldr	x1, .LC962
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L3258:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 688
	ret
	.p2align 3
.L3239:
	ldr	x1, .LC964
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	mov	w0, -4
	add	sp, sp, 688
	ret
.L3242:
	ldr	x2, [x29, 120]
	mov	w0, 0
	ldr	x1, .LC972
	strb	wzr, [x2]
	bl	dprint_vfmw
	mov	w0, -4
	b	.L3258
.L3273:
	ldr	x0, [x29, 120]
	mov	w2, 1
	mov	w1, w19
	strb	wzr, [x0]
	ldr	w0, [x20, 244]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC974
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3258
.L3233:
	ldr	x1, .LC966
	mov	w0, 0
	strb	wzr, [x19]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3258
.L3272:
	ldr	x1, .LC970
	bl	dprint_vfmw
	mov	x0, x20
	bl	HEVC_ClearDPB
	mov	w0, -4
	b	.L3258
	.size	HEVC_CreateLostPicture, .-HEVC_CreateLostPicture
	.align	3
.LC962:
	.xword	.LC961
	.align	3
.LC964:
	.xword	.LC963
	.align	3
.LC966:
	.xword	.LC965
	.align	3
.LC968:
	.xword	.LC967
	.align	3
.LC970:
	.xword	.LC969
	.align	3
.LC972:
	.xword	.LC971
	.align	3
.LC974:
	.xword	.LC973
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_RefPicProcess
	.type	HEVC_RefPicProcess, %function
HEVC_RefPicProcess:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	HEVC_RemoveDummyFrame
	mov	x0, x19
	bl	HEVC_ApplyReferencePictureSet
	mov	x0, x19
	bl	HEVC_RemoveUnusedApcStore
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	ldr	x20, .LC976
	b	.L3275
	.p2align 3
.L3278:
	ldr	w2, [x29, 44]
	mov	x1, x20
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 2344]
	mov	x0, x19
	ldr	w1, [x1, 4]
	cbz	w1, .L3276
	ldr	w1, [x29, 44]
	bl	HEVC_CreateLostPicture
	cbnz	w0, .L3281
.L3275:
	add	x1, x29, 44
	mov	x0, x19
	bl	HEVC_CheckThatAllRefPicsAreAvailable
	cbz	w0, .L3278
	mov	x0, x19
	bl	HEVC_UpdateReflist
	mov	x0, x19
	bl	HEVC_UpdateLTReflist
	mov	x0, x19
	bl	HEVC_RecoverDPBProcess
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3276:
	add	x19, x19, 53248
	ldr	w4, [x29, 44]
	ldr	x1, .LC978
	mov	w2, 0
	mov	w0, 1
	ldr	w3, [x19, 1808]
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3281:
	mov	x0, x19
	bl	HEVC_RemoveDummyFrame
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_RefPicProcess, .-HEVC_RefPicProcess
	.align	3
.LC976:
	.xword	.LC975
	.align	3
.LC978:
	.xword	.LC977
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecSlice
	.type	HEVC_DecSlice, %function
HEVC_DecSlice:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 53248
	bl	_mcount
	ldr	x0, [x19, 2360]
	ldr	w1, [x0, 16]
	str	w1, [x20, 1832]
	ldr	w0, [x0, 32]
	str	w0, [x20, 1828]
	strb	wzr, [x19, 1]
	ldr	w0, [x20, 3348]
	cmp	w0, 1
	beq	.L3325
.L3283:
	ldr	w0, [x19, 144]
	ldr	w2, [x19, 208]
	cmp	w2, w0
	bcs	.L3326
	ldrb	w0, [x20, 1073]
	cbz	w0, .L3327
	mov	x0, x19
	bl	HEVC_PicTypeStatistic
	mov	x0, x19
	bl	HEVC_CalcStreamBits
	ldr	w0, [x20, 3348]
	cmp	w0, 2
	beq	.L3296
	ldrb	w0, [x20, 1763]
	cbz	w0, .L3328
.L3297:
	mov	x0, x19
	bl	HEVC_WriteSliceMsg
	cbnz	w0, .L3329
.L3300:
	ldr	w0, [x20, 1836]
	mov	w3, 1
	str	w0, [x19, 288]
	mov	w0, 0
	ldr	w1, [x19, 5252]
	ldr	w2, [x20, 1808]
	str	w2, [x19, 252]
	str	w3, [x19, 168]
	str	w1, [x20, 1116]
	ldr	w1, [x19, 5256]
	str	w1, [x20, 1120]
.L3286:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3328:
	mov	x0, x19
	bl	HEVC_DecList
	cbnz	w0, .L3330
	mov	x0, x19
	bl	HEVC_IsRefListWrong
	cbz	w0, .L3297
	mov	x0, x19
	bl	HEVC_PrintDPBState
	ldr	x1, .LC998
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3325:
	sub	w1, w1, #16
	cmp	w1, 4
	bls	.L3284
	ldr	w0, [x20, 1084]
	cbnz	w0, .L3285
	ldr	x0, [x19, 2344]
	ldr	w1, [x0, 640]
	cmp	w1, 1
	beq	.L3331
	.p2align 2
.L3285:
	ldrb	w21, [x19, 2]
	cmp	w21, 1
	beq	.L3332
.L3287:
	ldr	x0, [x19, 2344]
	ldr	w1, [x0, 8]
	cmp	w1, 2
	beq	.L3289
	ldr	w0, [x0, 12]
	cmp	w0, 2
	mov	x0, x19
	beq	.L3333
	bl	HEVC_RefPicProcess
	cbnz	w0, .L3302
.L3289:
	mov	x0, x19
	bl	HEVC_InitPic
	cmn	w0, #2
	beq	.L3286
	cbnz	w0, .L3334
	ldr	w1, [x19, 220]
	mov	x0, x19
	add	w1, w1, 1
	str	w1, [x19, 220]
	bl	HEVC_SetScalingList
	mov	x0, x19
	bl	HEVC_WritePicMsg
	cbz	w0, .L3283
	ldr	x1, .LC987
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3296:
	ldr	x1, [x19, 2360]
	cbz	x1, .L3300
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
	b	.L3300
	.p2align 3
.L3331:
	ldr	w1, [x0, 636]
	cbnz	w1, .L3285
	ldr	w1, [x0, 644]
	cbnz	w1, .L3285
	ldr	w0, [x0, 604]
	add	w0, w0, 1024
	cmp	w0, 2048
	bls	.L3285
	.p2align 2
.L3284:
	mov	x0, x19
	bl	HEVC_FlushOutputFrmFromDPB
	cbz	w0, .L3285
	ldr	x2, .LC979
	mov	w0, 1
	ldr	x1, .LC981
	add	x2, x2, 408
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3332:
	ldr	w0, [x19, 2684]
	cbz	w0, .L3287
	mov	x0, x19
	bl	HEVC_EmptyFrmFromDPB
	cbz	w0, .L3287
	ldr	x2, .LC979
	mov	w0, w21
	ldr	x1, .LC983
	add	x2, x2, 408
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3333:
	bl	HEVC_UpdateReflist
	mov	x0, x19
	bl	HEVC_UpdateLTReflist
	b	.L3289
	.p2align 3
.L3327:
	ldr	x1, .LC994
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3326:
	ldr	x1, .LC989
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x19, 144]
	ldr	w2, [x19, 208]
	cmp	w2, w1
	bcc	.L3294
	ldr	x0, .LC990
	ldr	x4, [x0]
	cbz	x4, .L3294
	ldr	w0, [x19, 244]
	mov	w3, 8
	stp	w2, w1, [x29, 56]
	add	x2, x29, 56
	mov	w1, 108
	blr	x4
.L3294:
	ldr	x1, .LC992
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3330:
	ldr	x1, .LC996
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3329:
	ldr	x1, .LC1000
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
	.p2align 3
.L3334:
	ldr	x1, .LC985
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3286
.L3302:
	mov	w0, -1
	b	.L3286
	.size	HEVC_DecSlice, .-HEVC_DecSlice
	.align	3
.LC979:
	.xword	.LANCHOR5
	.align	3
.LC981:
	.xword	.LC980
	.align	3
.LC983:
	.xword	.LC982
	.align	3
.LC985:
	.xword	.LC984
	.align	3
.LC987:
	.xword	.LC986
	.align	3
.LC989:
	.xword	.LC988
	.align	3
.LC990:
	.xword	g_event_report
	.align	3
.LC992:
	.xword	.LC991
	.align	3
.LC994:
	.xword	.LC993
	.align	3
.LC996:
	.xword	.LC995
	.align	3
.LC998:
	.xword	.LC997
	.align	3
.LC1000:
	.xword	.LC999
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_VDMPostProc
	.type	HEVCDEC_VDMPostProc, %function
HEVCDEC_VDMPostProc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w22, w1
	mov	x23, x3
	mov	w21, w4
	mov	w20, w5
	bl	_mcount
	add	x0, x19, 53248
	str	w22, [x0, 1112]
	cbz	w21, .L3336
	ldr	x5, .LC1001
	sbfiz	x20, x20, 6, 32
	add	x20, x5, x20
	ldr	w0, [x20, 44]
	cmp	w0, 3
	beq	.L3344
	cmp	w0, 1
	beq	.L3336
.L3339:
	mov	x1, x23
	mov	x0, x19
	bl	HEVC_StorePicInDPB
	cbnz	w0, .L3345
	mov	w1, 1
	mov	w0, 0
	str	w1, [x19, 168]
.L3338:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3345:
	ldr	x1, .LC1003
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	x0, x19
	bl	HEVC_ClearDPB
	ldr	x23, [sp, 48]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3336:
	mov	x0, x19
	bl	HEVC_ClearAllSlice
	b	.L3339
	.p2align 3
.L3344:
	mov	x0, x19
	bl	HEVC_ClearAllSlice
	mov	w0, 0
	b	.L3338
	.size	HEVCDEC_VDMPostProc, .-HEVCDEC_VDMPostProc
	.align	3
.LC1001:
	.xword	g_VdmDrvParam
	.align	3
.LC1003:
	.xword	.LC1002
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecVDM
	.type	HEVC_DecVDM, %function
HEVC_DecVDM:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 172]
	cmp	w0, 2
	bhi	.L3347
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	ldr	x0, .LC1004
	ldr	x4, [x0]
	cbz	x4, .L3348
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3348:
	ldr	w0, [x19, 224]
	cmp	w0, 524288
	bhi	.L3408
	ldr	x1, .LC1008
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L3401:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3347:
	ldr	w0, [x19, 280]
	add	x21, x19, 53248
	ldr	w2, [x19, 284]
	add	w1, w0, 1
	ldr	x3, [x19, 2344]
	cmp	w1, w2
	mov	x0, x19
	csel	w1, w1, w2, ge
	str	w1, [x19, 280]
	sub	w1, w1, w2
	str	w1, [x3, 576]
	bl	HEVC_IsRefForCurrPic
	strb	w0, [x21, 1072]
	uxtb	w3, w0
	ldr	w20, [x19, 232]
	ldr	w2, [x19, 248]
	ubfx	x20, x20, 16, 3
	cmp	w20, w2
	beq	.L3351
	sub	w1, w2, #2
	cmp	w1, 1
	ccmp	w20, 2, 4, ls
	beq	.L3351
	cmp	w20, 1
	ccmp	w20, 4, 4, hi
	beq	.L3409
.L3351:
	cmp	w20, 2
	str	w20, [x19, 248]
	beq	.L3353
	cmp	w20, 3
	beq	.L3354
	cmp	w20, 1
	beq	.L3410
	ldr	w1, [x19, 216]
	cbz	w1, .L3405
	cmp	w20, 4
	bne	.L3360
	ldr	w1, [x21, 1084]
	cbnz	w1, .L3361
	mov	w0, 2
	strb	w0, [x19, 4]
	.p2align 2
.L3362:
	ldr	w2, [x19, 220]
	mov	w0, 2
	ldr	x1, .LC1015
	sub	w2, w2, #1
	bl	dprint_vfmw
	ldr	w2, [x19, 208]
	cbz	w2, .L3411
	ldr	w1, [x21, 1100]
	add	x0, x19, 61440
	add	x3, x19, 65536
	cmp	w1, wzr
	cset	w4, eq
	str	w4, [x0, 2980]
	ldr	w4, [x21, 1104]
	str	w4, [x0, 2972]
	str	w2, [x3, 116]
	ldr	x2, [x19, 2344]
	ldr	w4, [x2, 592]
	str	w4, [x0, 2964]
	ldr	w2, [x2, 632]
	str	w2, [x0, 2968]
	ldr	w2, [x19, 220]
	cbz	w2, .L3406
	add	x0, x0, 1648
	str	x0, [x19, 2352]
	add	w1, w1, 1
	ldr	w2, [x21, 1116]
	str	w2, [x0, 5284]
	ldr	w2, [x21, 1120]
	str	w2, [x0, 5288]
	str	w1, [x21, 1100]
	ldr	w0, [x21, 1084]
	cbnz	w0, .L3406
	ldr	x1, [x3, 120]
	mov	w4, 0
	cbz	x1, .L3369
	.p2align 2
.L3370:
	ldp	w2, w0, [x1]
	ldr	x1, [x1, 1360]
	add	w0, w2, w0
	add	w0, w0, 7
	add	w4, w4, w0, lsr 3
	cbnz	x1, .L3370
.L3369:
	ldr	x0, .LC1004
	ldr	x5, [x0]
	cbz	x5, .L3406
	add	x2, x29, 64
	ldr	w0, [x19, 244]
	mov	w3, 4
	mov	w1, 20
	str	w4, [x2, -8]!
	blr	x5
.L3406:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3408:
	ldr	x1, .LC1006
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	mov	w0, -8
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3410:
	ldr	w1, [x21, 1084]
	cmp	w1, 2
	ccmp	w3, wzr, 0, eq
	beq	.L3357
.L3356:
	ldr	w1, [x19, 216]
	cbz	w1, .L3405
.L3360:
	ldr	w1, [x21, 1084]
.L3361:
	cmp	w1, 2
	beq	.L3412
	cmp	w1, 1
	bne	.L3362
	strb	w1, [x19, 4]
	b	.L3362
.L3409:
	ldr	x1, .LC1010
	mov	w3, w20
	mov	w0, 1
	bl	dprint_vfmw
	str	w20, [x19, 248]
.L3354:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	x0, x19
	bl	HEVC_ClearDPB
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3353:
	ldr	w1, [x21, 1084]
	cbz	w1, .L3356
	b	.L3357
	.p2align 3
.L3412:
	ldrb	w1, [x19, 4]
	cmp	w1, 2
	bne	.L3362
	ldr	x1, [x19, 2344]
	ldr	w1, [x1, 12]
	cbnz	w1, .L3362
	ldr	x1, .LC1011
	ldrb	w1, [x1]
	cmp	w1, 1
	beq	.L3362
	ldr	x1, .LC1013
	mov	w0, 1
	ldr	w2, [x21, 1080]
	bl	dprint_vfmw
	.p2align 2
.L3405:
	ldr	x0, .LC1004
	ldr	x4, [x0]
	cbz	x4, .L3357
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3357:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	w0, -1
	b	.L3401
	.p2align 3
.L3411:
	ldr	x0, .LC1004
	ldr	x4, [x0]
	cbz	x4, .L3366
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3366:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	ldr	x2, .LC1016
	mov	w0, 0
	ldr	x1, .LC1018
	add	x2, x2, 424
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3401
	.size	HEVC_DecVDM, .-HEVC_DecVDM
	.align	3
.LC1004:
	.xword	g_event_report
	.align	3
.LC1006:
	.xword	.LC1005
	.align	3
.LC1008:
	.xword	.LC1007
	.align	3
.LC1010:
	.xword	.LC1009
	.align	3
.LC1011:
	.xword	g_allow_B_before_P
	.align	3
.LC1013:
	.xword	.LC1012
	.align	3
.LC1015:
	.xword	.LC1014
	.align	3
.LC1016:
	.xword	.LANCHOR5
	.align	3
.LC1018:
	.xword	.LC1017
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_DecOneNal
	.type	HEVC_DecOneNal, %function
HEVC_DecOneNal:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x3, [x19, 2360]
	ldr	w0, [x3, 8]
	cbz	w0, .L3418
	ldr	w2, [x19, 224]
	mov	w0, 0
	.p2align 2
.L3417:
	ubfiz	x1, x0, 5, 32
	add	w0, w0, 1
	add	x1, x3, x1
	ldr	w1, [x1, 56]
	add	w2, w2, w1
	str	w2, [x19, 224]
	ldr	w1, [x3, 8]
	cmp	w1, w0
	bhi	.L3417
.L3418:
	strb	wzr, [x3, 1]
	ldr	x0, [x19, 2360]
	ldr	w1, [x0, 16]
	sub	w1, w1, #36
	cmp	w1, 1
	bls	.L3552
.L3416:
	ldr	w1, [x0, 4]
	add	w1, w1, 16
	str	w1, [x0, 4]
	ldr	x1, [x19, 2360]
	ldr	w2, [x1, 16]
	cmp	w2, 49
	bls	.L3553
.L3419:
	ldr	x1, .LC1030
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x19, 2360]
	ldr	w0, [x1, 16]
	cmp	w0, 63
	bhi	.L3445
	cbnz	x1, .L3538
	.p2align 2
.L3536:
	mov	w0, 0
.L3556:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3553:
	ldr	x0, .LC1019
	ldrb	w0, [x0,w2,uxtw]
	adr	x3, .Lrtx3421
	add	x0, x3, w0, sxtb #2
	br	x0
.Lrtx3421:
	.section	.rodata
	.align	0
	.align	2
.L3421:
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3420 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3422 - .Lrtx3421) / 4
	.byte	(.L3423 - .Lrtx3421) / 4
	.byte	(.L3424 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3425 - .Lrtx3421) / 4
	.byte	(.L3425 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3419 - .Lrtx3421) / 4
	.byte	(.L3426 - .Lrtx3421) / 4
	.byte	(.L3427 - .Lrtx3421) / 4
	.text
	.p2align 3
.L3552:
	mov	w1, 1
	strb	w1, [x19, 1]
	b	.L3416
.L3427:
	mov	x0, x19
	bl	HEVC_IsEosNal
	uxtb	w1, w0
	cmp	w1, 1
	beq	.L3554
.L3443:
	ldr	x1, [x19, 2360]
	cbz	x1, .L3536
.L3538:
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
	mov	w0, 0
.L3535:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L3420:
	mov	x0, x19
	bl	HEVC_DecSlice
	cmn	w0, #2
	beq	.L3535
	cbz	w0, .L3536
	ldr	x1, [x19, 2360]
	cbz	x1, .L3550
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3550:
	ldr	x0, .LC1022
	ldr	x4, [x0]
	cbz	x4, .L3537
.L3539:
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3537:
	mov	w0, -1
.L3555:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L3423:
	mov	x0, x19
	bl	HEVC_DecSPS
	mov	w20, w0
	ldr	x1, [x19, 2360]
	cbz	x1, .L3433
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3433:
	cbz	w20, .L3536
	ldr	x1, .LC1024
	b	.L3546
.L3424:
	mov	x0, x19
	bl	HEVC_DecPPS
	mov	w20, w0
	ldr	x1, [x19, 2360]
	cbz	x1, .L3435
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3435:
	cbz	w20, .L3536
	ldr	x1, .LC1026
	b	.L3546
.L3422:
	mov	x0, x19
	bl	HEVC_DecVPS
	mov	w20, w0
	ldr	x1, [x19, 2360]
	cbz	x1, .L3428
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3428:
	cbz	w20, .L3536
	ldr	x1, .LC1021
.L3546:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, .LC1022
	ldr	x4, [x21]
	cbz	x4, .L3537
	add	x2, x29, 64
	ldr	w0, [x19, 244]
	mov	w20, -1
	mov	w3, 4
	mov	w1, 100
	str	w20, [x2, -8]!
	blr	x4
	ldr	x4, [x21]
	cbz	x4, .L3537
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
.L3425:
	mov	x0, x19
	bl	HEVC_DecSEI
	mov	w20, w0
	ldr	x1, [x19, 2360]
	cbz	x1, .L3436
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3436:
	cbz	w20, .L3536
	ldr	x1, .LC1028
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3550
.L3426:
	ldrb	w0, [x19, 6]
	cmp	w0, 1
	beq	.L3441
	cbz	x1, .L3440
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3440:
	add	x0, x19, 53248
	ldrb	w0, [x0, 1073]
	cbz	w0, .L3536
.L3441:
	add	x20, x19, 2640
	mov	w1, 32
	strb	wzr, [x19, 6]
	mov	x0, x20
	bl	BsGet
	mov	w21, w0
	mov	w1, 32
	mov	x0, x20
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w21, w2
	ccmp	w0, w1, 0, eq
	bne	.L3536
	ldr	x20, .LC1022
	ldr	x4, [x20]
	cbz	x4, .L3442
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3442:
	mov	x0, x19
	bl	HEVC_DecVDM
	cbz	w0, .L3536
.L3548:
	ldr	x4, [x20]
	cbnz	x4, .L3539
	mov	w0, -1
	b	.L3555
.L3445:
	ldr	x20, .LC1022
	ldr	x4, [x20]
	cbz	x4, .L3446
	ldr	w0, [x19, 244]
	mov	w1, 102
	mov	w3, 0
	mov	x2, 0
	blr	x4
	ldr	x1, [x19, 2360]
	cbz	x1, .L3447
.L3448:
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3447:
	ldr	x4, [x20]
	cbz	x4, .L3536
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, 0
	b	.L3556
.L3446:
	cbnz	x1, .L3448
	mov	w0, 0
	b	.L3556
.L3554:
	ldr	x20, .LC1022
	strb	w0, [x19, 7]
	ldr	x4, [x20]
	cbz	x4, .L3444
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3444:
	mov	x0, x19
	bl	HEVC_DecVDM
	cbz	w0, .L3443
	b	.L3548
	.size	HEVC_DecOneNal, .-HEVC_DecOneNal
	.align	3
.LC1019:
	.xword	.L3421
	.align	3
.LC1021:
	.xword	.LC1020
	.align	3
.LC1022:
	.xword	g_event_report
	.align	3
.LC1024:
	.xword	.LC1023
	.align	3
.LC1026:
	.xword	.LC1025
	.align	3
.LC1028:
	.xword	.LC1027
	.align	3
.LC1030:
	.xword	.LC1029
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_UnMarkFrameStoreRef
	.type	HEVC_UnMarkFrameStoreRef, %function
HEVC_UnMarkFrameStoreRef:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	strb	wzr, [x19, 1]
	ldr	w1, [x19, 32]
	mov	w2, 0
	ldr	w0, [x20, 244]
	bl	FSP_SetRef
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_UnMarkFrameStoreRef, .-HEVC_UnMarkFrameStoreRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_InquireSliceProperty
	.type	HEVC_InquireSliceProperty, %function
HEVC_InquireSliceProperty:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x21, x2
	mov	x0, x30
	mov	x22, x1
	add	x20, x19, 53248
	bl	_mcount
	str	wzr, [x21]
	ldr	x3, .LC1031
	mov	x2, 3732
	str	wzr, [x22]
	mov	w1, 0
	add	x0, x20, 1760
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x1, [x19, 2360]
	mov	x0, x19
	ldr	w2, [x1, 16]
	str	w2, [x20, 1832]
	ldr	w1, [x1, 32]
	str	w1, [x20, 1828]
	bl	HEVC_DecSliceSegmentHeader
	cbnz	w0, .L3570
	ldr	w0, [x20, 3348]
	mov	w5, 2024
	mov	w4, 26164
	cmp	w0, 1
	cset	w0, eq
	str	w0, [x22]
	ldr	w2, [x20, 1836]
	ldr	x1, [x19, 2336]
	ldr	x0, [x19, 2328]
	ldr	x3, [x19, 28136]
	smaddl	x1, w2, w5, x1
	ldr	x2, [x19, 2344]
	ldr	w8, [x3, 236]
	ldr	w1, [x1, 32]
	ldr	w7, [x2, 832]
	ldr	w5, [x3, 72]
	smaddl	x0, w1, w4, x0
	ldr	w3, [x3, 76]
	ldr	w1, [x0, 236]
	ldp	w6, w4, [x0, 72]
	cmp	w1, w8
	cset	w1, ne
	cmp	w7, 1
	beq	.L3571
.L3562:
	cmp	w6, w5
	cset	w0, ne
	cmp	w4, w3
	orr	w0, w1, w0
	cset	w1, ne
	orr	w0, w1, w0
	cbz	w0, .L3566
	mov	w1, 1
	mov	w0, 0
	str	w1, [x21]
.L3560:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3570:
	ldr	x1, .LC1033
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC1034
	ldr	x4, [x0]
	cbz	x4, .L3565
	add	x2, x29, 64
	ldr	w0, [x19, 244]
	mov	w20, -1
	mov	w3, 4
	mov	w1, 100
	str	w20, [x2, -8]!
	blr	x4
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3571:
	ldr	w7, [x0, 108]
	ldr	w0, [x0, 100]
	cmp	w0, w7
	csel	w0, w0, w7, cs
	str	w0, [x2, 884]
	ldr	x0, [x19, 28136]
	ldr	x7, [x19, 2344]
	ldr	w2, [x0, 108]
	ldr	w0, [x0, 100]
	cmp	w0, w2
	csel	w0, w0, w2, cs
	ldr	w2, [x7, 884]
	cmp	w0, 8
	ccmp	w2, 8, 0, hi
	bne	.L3572
.L3563:
	mov	w0, 1
	str	w0, [x7, 888]
	str	w0, [x21]
	b	.L3562
	.p2align 3
.L3566:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3565:
	mov	w0, -1
	b	.L3560
	.p2align 3
.L3572:
	cmp	w0, 8
	ccmp	w2, 8, 4, eq
	ble	.L3562
	b	.L3563
	.size	HEVC_InquireSliceProperty, .-HEVC_InquireSliceProperty
	.align	3
.LC1031:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC1033:
	.xword	.LC1032
	.align	3
.LC1034:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_DecodePacket
	.type	HEVCDEC_DecodePacket, %function
HEVCDEC_DecodePacket:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	str	wzr, [x29, 68]
	stp	wzr, wzr, [x29, 60]
	cbz	x20, .L3639
	ldrb	w0, [x19]
	cmp	w0, 1
	bne	.L3640
	ldr	x0, [x19, 2344]
	ldrb	w1, [x19, 5]
	ldr	w2, [x0, 8]
	cmp	w1, 1
	ubfiz	w2, w2, 16, 3
	str	w2, [x19, 232]
	ldr	w1, [x0]
	str	w1, [x19, 236]
	ldr	w1, [x0, 4]
	str	w1, [x19, 240]
	beq	.L3577
	ldr	w1, [x0, 832]
	add	x21, x19, 86016
	cmp	w1, 1
	beq	.L3641
.L3578:
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_ReceivePacket
	mov	w20, w0
	ldr	x0, [x19, 2360]
	str	x0, [x21, 896]
.L3579:
	strb	wzr, [x19, 5]
	cbz	w20, .L3580
.L3598:
	mov	w0, 0
.L3637:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3580:
	ldr	x0, [x19, 2360]
	str	wzr, [x0, 4]
	ldr	x1, [x19, 2360]
	ldr	x0, [x19, 2312]
	ldr	w2, [x1, 56]
	ldr	x1, [x1, 48]
	bl	BsInit
	ldr	x0, [x19, 2312]
	mov	w1, 24
	bl	BsSkip
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x20, 24]
	mov	w1, 6
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 16]
	mov	w1, 6
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 28]
	mov	w1, 3
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 32]
	ldr	x20, [x19, 2360]
	ldr	w0, [x20, 32]
	cbnz	w0, .L3642
.L3581:
	ldr	w0, [x20, 16]
	bl	HEVC_IsSliceUnit
	sxtb	w21, w0
	cmp	w21, 1
	beq	.L3643
.L3582:
	str	wzr, [x20, 4]
	add	x0, x19, 2640
	ldr	x1, [x19, 2360]
	ldr	w2, [x1, 56]
	ldr	x1, [x1, 48]
	bl	BsInit
	ldr	x0, [x19, 2312]
	mov	w1, 24
	bl	BsSkip
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	mov	w1, 1
	bl	BsGet
	str	w0, [x20, 24]
	mov	w1, 6
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 16]
	mov	w1, 6
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	bl	BsGet
	str	w0, [x20, 28]
	mov	w1, 3
	ldr	x20, [x19, 2360]
	ldr	x0, [x19, 2312]
	bl	BsGet
	sub	w0, w0, #1
	str	w0, [x20, 32]
.L3597:
	mov	x0, x19
	bl	HEVC_DecOneNal
	cmn	w0, #2
	beq	.L3637
	cbz	w0, .L3598
	ldr	x1, .LC1052
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3637
	.p2align 3
.L3641:
	ldr	w0, [x0, 872]
	cmp	w0, 1
	bne	.L3578
	ldr	w0, [x21, 888]
	cmp	w0, 1
	bne	.L3578
	ldr	x1, [x21, 896]
	mov	w0, 0
	str	x1, [x19, 2360]
	str	wzr, [x21, 888]
	strb	wzr, [x19, 5]
	b	.L3637
	.p2align 3
.L3577:
	ldr	x0, [x19, 2360]
	cmp	x0, xzr
	csetm	w20, eq
	cbz	w20, .L3579
	ldr	x1, .LC1040
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, w20
	b	.L3637
	.p2align 3
.L3642:
	sub	w0, w0, #1
	str	w0, [x20, 32]
	ldr	x20, [x19, 2360]
	b	.L3581
	.p2align 3
.L3643:
	add	x2, x29, 76
	add	x1, x29, 72
	mov	x0, x19
	bl	HEVC_InquireSliceProperty
	cbnz	w0, .L3583
	ldr	w20, [x29, 72]
	cmp	w20, 1
	beq	.L3644
.L3584:
	ldr	w20, [x29, 76]
	cmp	w20, 1
	beq	.L3645
.L3595:
	ldr	x20, [x19, 2360]
	ldr	w0, [x20, 16]
	bl	HEVC_IsSliceUnit
	sxtb	w0, w0
	cmp	w0, 1
	beq	.L3597
	b	.L3582
	.p2align 3
.L3583:
	ldr	x1, .LC1050
	mov	w0, w21
	bl	dprint_vfmw
	ldr	x1, [x19, 2360]
	cbz	x1, .L3596
	ldr	w0, [x19, 244]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2360]
.L3596:
	ldr	x0, .LC1048
	ldr	x4, [x0]
	cbz	x4, .L3600
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, -1
	b	.L3637
	.p2align 3
.L3640:
	ldr	x1, .LC1038
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3637
	.p2align 3
.L3645:
	add	x0, x19, 53248
	ldr	w0, [x0, 1832]
	sub	w0, w0, #16
	cmp	w0, 4
	bls	.L3646
.L3587:
	ldrb	w20, [x19, 2]
	cmp	w20, 1
	beq	.L3647
.L3590:
	ldr	w0, [x19, 244]
	bl	FSP_GetFspType
	cbnz	w0, .L3595
	ldr	w0, [x19, 244]
	add	x3, x29, 68
	add	x2, x29, 64
	add	x1, x29, 60
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 708]
	cmp	w0, 1
	bne	.L3595
	ldr	w1, [x29, 68]
	cmp	w1, wzr
	bgt	.L3593
	ldr	w2, [x29, 64]
	cmp	w2, 32
	bgt	.L3593
	ldr	x2, [x19, 2352]
	cbnz	x2, .L3593
	ldr	w2, [x29, 76]
	cbz	w2, .L3595
	ldr	w3, [x19, 220]
	cbz	w3, .L3595
	add	x20, x19, 86016
	ldr	x3, [x19, 28136]
	ldr	w4, [x20, 920]
	ldr	w6, [x3, 76]
	cmp	w6, w4
	beq	.L3595
	ldr	w5, [x3, 72]
	ldr	w3, [x20, 924]
	cmp	w5, w3
	ccmp	w1, wzr, 0, ne
	bne	.L3595
	ldr	x1, .LC1047
	bl	dprint_vfmw
	str	wzr, [x20, 916]
	ldr	x1, .LC1048
	ldr	x0, [x19, 28136]
	ldr	x4, [x1]
	ldr	w1, [x0, 76]
	str	w1, [x20, 920]
	ldr	w0, [x0, 72]
	str	w0, [x20, 924]
	cbz	x4, .L3595
	ldr	w0, [x19, 244]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L3595
	.p2align 3
.L3644:
	ldr	w1, [x19, 208]
	cbnz	w1, .L3585
	add	x1, x19, 53248
	ldrb	w1, [x1, 1073]
	cbz	w1, .L3584
	ldr	x2, .LC1041
	ldr	x1, .LC1043
	add	x2, x2, 440
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	b	.L3584
.L3585:
	ldr	x3, [x19, 2360]
	mov	w2, 13
	ldr	x1, .LC1041
	add	x0, x19, 2640
	str	wzr, [x3, 4]
	add	x1, x1, 488
	bl	BsInit
	ldr	x0, [x19, 2312]
	mov	w1, 40
	bl	BsSkip
	ldr	x1, [x19, 2360]
	mov	w2, 48
	mov	x0, x19
	str	w2, [x1, 16]
	strb	w20, [x19, 6]
	bl	HEVC_DecOneNal
	strb	w20, [x19, 5]
	ldr	w1, [x19, 272]
	mov	w0, -2
	add	w1, w1, 1
	str	w1, [x19, 272]
	b	.L3637
.L3600:
	mov	w0, -1
	b	.L3637
.L3639:
	ldr	x1, .LC1036
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3637
.L3646:
	mov	x0, x19
	bl	HEVC_FlushOutputFrmFromDPB
	cbz	w0, .L3587
	ldr	x2, .LC1041
	mov	w0, w20
	ldr	x1, .LC1044
	add	x2, x2, 464
	bl	dprint_vfmw
	b	.L3587
.L3647:
	mov	x0, x19
	bl	HEVC_EmptyFrmFromDPB
	cbz	w0, .L3590
	ldr	x2, .LC1041
	mov	w0, w20
	ldr	x1, .LC1045
	add	x2, x2, 464
	bl	dprint_vfmw
	b	.L3590
.L3593:
	add	x2, x19, 86016
	mov	w1, 1
	mov	w0, -2
	str	wzr, [x2, 916]
	strb	w1, [x19, 5]
	b	.L3637
	.size	HEVCDEC_DecodePacket, .-HEVCDEC_DecodePacket
	.align	3
.LC1036:
	.xword	.LC1035
	.align	3
.LC1038:
	.xword	.LC1037
	.align	3
.LC1040:
	.xword	.LC1039
	.align	3
.LC1041:
	.xword	.LANCHOR5
	.align	3
.LC1043:
	.xword	.LC1042
	.align	3
.LC1044:
	.xword	.LC980
	.align	3
.LC1045:
	.xword	.LC982
	.align	3
.LC1047:
	.xword	.LC1046
	.align	3
.LC1048:
	.xword	g_event_report
	.align	3
.LC1050:
	.xword	.LC1049
	.align	3
.LC1052:
	.xword	.LC1051
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_HaveSliceToDec
	.type	HEVC_HaveSliceToDec, %function
HEVC_HaveSliceToDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, 0
	bl	_mcount
	ldr	w0, [x19, 208]
	cbnz	w0, .L3649
	add	x0, x19, 53248
	mov	w20, -1
	ldrb	w0, [x0, 1073]
	cbnz	w0, .L3653
.L3649:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3653:
	ldr	x2, .LC1053
	mov	w0, 0
	ldr	x1, .LC1054
	add	x2, x2, 440
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_HaveSliceToDec, .-HEVC_HaveSliceToDec
	.align	3
.LC1053:
	.xword	.LANCHOR5
	.align	3
.LC1054:
	.xword	.LC1042
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVC_GetPicStreamSize
	.type	HEVC_GetPicStreamSize, %function
HEVC_GetPicStreamSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 65536
	bl	_mcount
	mov	w0, 0
	ldr	x2, [x19, 120]
	cbz	x2, .L3655
	.p2align 2
.L3656:
	ldp	w3, w1, [x2]
	ldr	x2, [x2, 1360]
	add	w1, w3, w1
	add	w1, w1, 7
	add	w0, w0, w1, lsr 3
	cbnz	x2, .L3656
.L3655:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_GetPicStreamSize, .-HEVC_GetPicStreamSize
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_GetRemainImg
	.type	HEVCDEC_GetRemainImg, %function
HEVCDEC_GetRemainImg:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 2684]
	cbz	w0, .L3671
	ldr	x23, .LC1055
	add	x22, x19, 61440
	ldr	x24, .LC1059
	mov	w21, -1
	add	x23, x23, 504
	ldr	x26, .LC1057
	add	x22, x22, 1032
	mov	x25, x23
	b	.L3670
	.p2align 3
.L3662:
	mov	x0, x19
	bl	HEVC_RemoveApcStore
	ldr	w1, [x29, 104]
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
	ldr	w0, [x19, 2684]
	cbz	w0, .L3680
.L3670:
	add	x1, x29, 108
	mov	x0, x19
	add	x2, x29, 104
	bl	HEVC_GetMinPOC
	ldr	w1, [x29, 104]
	add	x0, x19, x1, uxtw 3
	ldr	x20, [x0, 2704]
	cbz	x20, .L3662
	ldrb	w0, [x20, 2]
	cbnz	w0, .L3662
	mov	w0, 1
	mov	x1, x20
	strb	w0, [x20, 2]
	mov	x0, x19
	bl	HEVC_GetImagePara
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_CheckFrameStore
	mov	w21, w0
	cbz	w0, .L3681
	ldr	w1, [x29, 104]
.L3685:
	mov	x0, x19
	bl	HEVC_RemoveApcStore
	ldr	w1, [x29, 104]
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
	ldr	w0, [x19, 2684]
	cbnz	w0, .L3670
	.p2align 2
.L3680:
	cbnz	w21, .L3671
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3681:
	ldr	w0, [x19, 244]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 244]
	ldr	w1, [x20, 32]
	bl	FSP_GetFsImagePtr
	mov	x27, x0
	cbz	x0, .L3682
	ldr	w0, [x19, 2684]
	cmp	w0, 1
	beq	.L3683
.L3665:
	ldr	w0, [x19, 244]
	mov	x4, x27
	mov	x3, x22
	mov	x2, x19
	mov	w1, 16
	bl	InsertImgToVoQueue
	cmp	w0, 1
	mov	w28, w0
	beq	.L3666
	mov	w3, w0
	mov	x2, x23
	mov	x1, x24
	mov	w0, 0
	mov	w28, -1
	bl	dprint_vfmw
	ldr	w0, [x19, 244]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	mov	w1, 1
	mov	x0, x19
	bl	HEVC_ClearAll
.L3666:
	ldr	x0, [x19, 2344]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L3684
	str	wzr, [x27, 24]
.L3668:
	ldr	w0, [x19, 284]
	ldr	w1, [x29, 104]
	add	w0, w0, 1
	str	w0, [x19, 284]
	ldr	w0, [x19, 276]
	add	w0, w0, 1
	str	w0, [x19, 276]
	ldr	w0, [x19, 2684]
	cmp	w0, 1
	beq	.L3662
	mov	w21, w28
	b	.L3662
	.p2align 3
.L3671:
	add	x0, x19, 61440
	add	x0, x0, 1032
	bl	GetVoLastImageID
	mov	w21, w0
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3684:
	mov	x1, x27
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.3
	b	.L3668
	.p2align 3
.L3683:
	str	w0, [x27, 188]
	b	.L3665
.L3682:
	mov	x1, x26
	mov	x2, x25
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	ldr	w1, [x29, 104]
	b	.L3685
	.size	HEVCDEC_GetRemainImg, .-HEVCDEC_GetRemainImg
	.align	3
.LC1055:
	.xword	.LANCHOR5
	.align	3
.LC1057:
	.xword	.LC1056
	.align	3
.LC1059:
	.xword	.LC1058
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_GetImageWidth
	.type	HEVCDEC_GetImageWidth, %function
HEVCDEC_GetImageWidth:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 28136]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w2, [x0, 152]
	ldr	w0, [x0, 72]
	add	w1, w2, w0
	neg	w2, w2
	sub	w0, w1, #1
	and	w0, w0, w2
	ret
	.size	HEVCDEC_GetImageWidth, .-HEVCDEC_GetImageWidth
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_GetImageHeight
	.type	HEVCDEC_GetImageHeight, %function
HEVCDEC_GetImageHeight:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 28136]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w2, [x0, 152]
	ldr	w0, [x0, 76]
	add	w1, w2, w0
	neg	w2, w2
	sub	w0, w1, #1
	and	w0, w0, w2
	ret
	.size	HEVCDEC_GetImageHeight, .-HEVCDEC_GetImageHeight
	.global	UUID_ISO_IEC_11578
	.global	g_IsDeced
	.global	g_TsToRsMap
	.global	s_TmpParam
	.section	.rodata
	.align	3
.LANCHOR1 = . + 0
	.type	SarTable.11598, %object
	.size	SarTable.11598, 136
SarTable.11598:
	.word	1
	.word	1
	.word	1
	.word	1
	.word	12
	.word	11
	.word	10
	.word	11
	.word	16
	.word	11
	.word	40
	.word	33
	.word	24
	.word	11
	.word	20
	.word	11
	.word	32
	.word	11
	.word	80
	.word	33
	.word	18
	.word	11
	.word	15
	.word	11
	.word	64
	.word	33
	.word	160
	.word	99
	.word	4
	.word	3
	.word	3
	.word	2
	.word	2
	.word	1
	.type	__func__.11751, %object
	.size	__func__.11751, 18
__func__.11751:
	.string	"HEVC_CalcTileInfo"
	.zero	6
.LC0:
	.word	32
	.word	70
	.word	175
	.word	403
	.word	-48
	.word	-120
	.word	475
	.zero	4
.LC1:
	.word	33
	.word	58
	.word	203
	.word	377
	.word	-42
	.word	-146
	.word	482
	.zero	4
	.type	UUID_ISO_IEC_11578, %object
	.size	UUID_ISO_IEC_11578, 16
UUID_ISO_IEC_11578:
	.byte	-15
	.byte	76
	.byte	60
	.byte	-64
	.byte	85
	.byte	-29
	.byte	17
	.byte	-28
	.byte	-119
	.byte	38
	.byte	0
	.byte	2
	.byte	-91
	.byte	-43
	.byte	-59
	.byte	27
	.type	__func__.12203, %object
	.size	__func__.12203, 27
__func__.12203:
	.string	"HEVC_GetUnRefPicWithMinPoc"
	.zero	5
	.type	__func__.12212, %object
	.size	__func__.12212, 21
__func__.12212:
	.string	"HEVC_IncreaseDPBSize"
	.zero	3
	.type	__func__.12443, %object
	.size	__func__.12443, 17
__func__.12443:
	.string	"HEVC_WritePicMsg"
	.zero	7
	.type	__func__.12472, %object
	.size	__func__.12472, 16
__func__.12472:
	.string	"HEVC_CheckListX"
	.type	__func__.12596, %object
	.size	__func__.12596, 16
__func__.12596:
	.string	"HEVC_RepairList"
	.type	__func__.12966, %object
	.size	__func__.12966, 21
__func__.12966:
	.string	"HEVC_CheckFrameStore"
	.zero	3
	.type	__func__.12812, %object
	.size	__func__.12812, 19
__func__.12812:
	.string	"HEVC_OutputCurrPic"
	.zero	5
	.type	__func__.12817, %object
	.size	__func__.12817, 23
__func__.12817:
	.string	"HEVC_FirstFrameFastOut"
	.zero	1
	.type	__func__.13112, %object
	.size	__func__.13112, 14
__func__.13112:
	.string	"HEVC_IsNewPic"
	.zero	2
	.type	quant8_inter_default, %object
	.size	quant8_inter_default, 64
quant8_inter_default:
	.word	269488144
	.word	403968529
	.word	286265360
	.word	421008402
	.word	303108112
	.word	471406612
	.word	336728336
	.word	555489560
	.word	403968529
	.word	690035737
	.word	421008402
	.word	908665116
	.word	471406612
	.word	1194731809
	.word	555489560
	.word	1531393577
	.type	quant_ts_default, %object
	.size	quant_ts_default, 16
quant_ts_default:
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.align	3
.LANCHOR5 = . + 0
	.type	__func__.13393, %object
	.size	__func__.13393, 16
__func__.13393:
	.string	"HEVC_ReleaseNAL"
	.type	g_CropUnitX, %object
	.size	g_CropUnitX, 16
g_CropUnitX:
	.word	1
	.word	2
	.word	2
	.word	1
	.type	g_CropUnitY, %object
	.size	g_CropUnitY, 16
g_CropUnitY:
	.word	1
	.word	2
	.word	1
	.word	1
	.type	__func__.12294, %object
	.size	__func__.12294, 27
__func__.12294:
	.string	"HEVC_DecSliceSegmentHeader"
	.zero	5
	.type	__func__.12675, %object
	.size	__func__.12675, 13
__func__.12675:
	.string	"HEVC_InitDPB"
	.zero	3
	.type	__func__.13445, %object
	.size	__func__.13445, 13
__func__.13445:
	.string	"HEVCDEC_Init"
	.zero	3
	.type	__func__.12958, %object
	.size	__func__.12958, 19
__func__.12958:
	.string	"HEVC_OutputFrmToVO"
	.zero	5
	.type	__func__.12925, %object
	.size	__func__.12925, 22
__func__.12925:
	.string	"HEVC_OutputFrmFromDPB"
	.zero	2
	.type	__func__.12218, %object
	.size	__func__.12218, 30
__func__.12218:
	.string	"HEVC_OutputUnRefPicWithMinPoc"
	.zero	2
	.type	__func__.12938, %object
	.size	__func__.12938, 27
__func__.12938:
	.string	"HEVC_FlushOutputFrmFromDPB"
	.zero	5
	.type	__func__.13433, %object
	.size	__func__.13433, 23
__func__.13433:
	.string	"HEVCDEC_GetImageBuffer"
	.zero	1
	.type	__func__.13484, %object
	.size	__func__.13484, 14
__func__.13484:
	.string	"HEVC_ClearDPB"
	.zero	2
	.type	__func__.12223, %object
	.size	__func__.12223, 23
__func__.12223:
	.string	"HEVC_RecoverDPBProcess"
	.zero	1
	.type	__func__.12905, %object
	.size	__func__.12905, 20
__func__.12905:
	.string	"HEVC_InsertFrmInDPB"
	.zero	4
	.type	__func__.12822, %object
	.size	__func__.12822, 22
__func__.12822:
	.string	"HEVC_SimpleDPBProcess"
	.zero	2
	.type	__func__.12827, %object
	.size	__func__.12827, 21
__func__.12827:
	.string	"HEVC_DecOrderProcess"
	.zero	3
	.type	__func__.12832, %object
	.size	__func__.12832, 22
__func__.12832:
	.string	"HEVC_DispOrderProcess"
	.zero	2
	.type	__func__.12848, %object
	.size	__func__.12848, 19
__func__.12848:
	.string	"HEVC_StorePicInDPB"
	.zero	5
	.type	__func__.12250, %object
	.size	__func__.12250, 14
__func__.12250:
	.string	"HEVC_DecSlice"
	.zero	2
	.type	__func__.12549, %object
	.size	__func__.12549, 12
__func__.12549:
	.string	"HEVC_DecVDM"
	.zero	4
	.type	__func__.13546, %object
	.size	__func__.13546, 20
__func__.13546:
	.string	"HEVC_HaveSliceToDec"
	.zero	4
	.type	__func__.11698, %object
	.size	__func__.11698, 21
__func__.11698:
	.string	"HEVCDEC_DecodePacket"
	.zero	3
	.type	g_HEVCNalTypeEOPIC, %object
	.size	g_HEVCNalTypeEOPIC, 13
g_HEVCNalTypeEOPIC:
	.byte	0
	.byte	0
	.byte	1
	.byte	96
	.byte	0
	.byte	72
	.byte	83
	.byte	80
	.byte	73
	.byte	67
	.byte	69
	.byte	78
	.byte	68
	.zero	3
	.type	__func__.13563, %object
	.size	__func__.13563, 21
__func__.13563:
	.string	"HEVCDEC_GetRemainImg"
	.data
	.align	3
.LANCHOR4 = . + 0
	.type	quant8_intra_default, %object
	.size	quant8_intra_default, 64
quant8_intra_default:
	.word	269488144
	.word	404034065
	.word	269488144
	.word	420877073
	.word	303108112
	.word	488183316
	.word	353505296
	.word	606018328
	.word	403968273
	.word	791225118
	.word	454431506
	.word	1094069283
	.word	521737749
	.word	1480996393
	.word	605886744
	.word	1935163695
	.type	g_quantTSDefault4x4, %object
	.size	g_quantTSDefault4x4, 16
g_quantTSDefault4x4:
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.type	g_quantInterDefault8x8, %object
	.size	g_quantInterDefault8x8, 64
g_quantInterDefault8x8:
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	54
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	54
	.byte	71
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	54
	.byte	71
	.byte	91
	.type	g_quantIntraDefault8x8, %object
	.size	g_quantIntraDefault8x8, 64
g_quantIntraDefault8x8:
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	21
	.byte	24
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	19
	.byte	22
	.byte	25
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	22
	.byte	25
	.byte	29
	.byte	16
	.byte	16
	.byte	18
	.byte	21
	.byte	24
	.byte	27
	.byte	31
	.byte	36
	.byte	17
	.byte	17
	.byte	20
	.byte	24
	.byte	30
	.byte	35
	.byte	41
	.byte	47
	.byte	18
	.byte	19
	.byte	22
	.byte	27
	.byte	35
	.byte	44
	.byte	54
	.byte	65
	.byte	21
	.byte	22
	.byte	25
	.byte	31
	.byte	41
	.byte	54
	.byte	70
	.byte	88
	.byte	24
	.byte	25
	.byte	29
	.byte	36
	.byte	47
	.byte	65
	.byte	88
	.byte	115
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC10:
	ASCII(.string	"dar=%d\n" )
.LC12:
	ASCII(.string	"--------------- PrintNaluArraySlot START ---------------\n" )
	.zero	6
.LC14:
	ASCII(.string	"\n" )
	.zero	6
.LC16:
	ASCII(.string	"--------------- PrintNaluArraySlot END ---------------\n" )
.LC18:
	ASCII(.string	"NalArray[%d]=%p,is_valid=%d   " )
	.zero	1
.LC20:
	ASCII(.string	"--------------- PrintCurrSlicelistX START ---------------\n" )
	.zero	5
.LC22:
	ASCII(.string	"List0, size: %d\n" )
	.zero	7
.LC24:
	ASCII(.string	"List1, size: %d\n" )
	.zero	7
.LC26:
	ASCII(.string	"pListX[0][%d]:%p " )
	.zero	6
.LC28:
	ASCII(.string	"pFrameStore:%p " )
.LC30:
	ASCII(.string	"pLogicFs:%p " )
	.zero	3
.LC33:
	ASCII(.string	"--------------- PrintCurrSlicelistX END ---------------\n" )
	.zero	7
.LC35:
	ASCII(.string	"pListX[1][%d]: %p" )
	.zero	6
.LC39:
	ASCII(.string	"--------------- PrintFrameStoreState START ---------------\n" )
	.zero	4
.LC41:
	ASCII(.string	"--------------- PrintFrameStoreState END ---------------\n" )
	.zero	6
.LC43:
	ASCII(.string	"Fs=%d, pFs=%p, state=%d, is_reference=%d, is_displayed=%d " )
	.zero	5
.LC45:
	ASCII(.string	"p_usrdat[%d]=%p " )
	.zero	7
.LC48:
	ASCII(.string	"--------------- PrintAPCState START ---------------\n" )
	.zero	3
.LC50:
	ASCII(.string	"APC.used_size:%d, APC.size:%d\n" )
	.zero	1
.LC52:
	ASCII(.string	"%d: is_used=%d, idc=%d, poc=%d\n" )
.LC54:
	ASCII(.string	"--------------- PrintAPCState END ---------------\n" )
	.zero	5
.LC56:
	ASCII(.string	"--------------- PrintDPBState START ---------------\n" )
	.zero	3
.LC58:
	ASCII(.string	"DPB: size:%d, used_size:%d, negative:%d, positive:%d, longterm:%d\n" )
	.zero	5
.LC61:
	ASCII(.string	"fs[%d]:%p non_existing:%d state:%d is_ref:%d displayed:%d poc:%d long_term:%d LogicFs:%p, fsp_ref:%d fsp_disp:%d logic_id:%d img_id:%d\n" )
.LC63:
	ASCII(.string	"--------------- PrintDPBState END ---------------\n" )
	.zero	5
.LC65:
	ASCII(.string	"fs[%d]\t" )
.LC67:
	ASCII(.string	"LogicFs:%p\t" )
	.zero	4
.LC69:
	ASCII(.string	"LogicTick:%d\t" )
	.zero	2
.LC71:
	ASCII(.string	"IsRef:%d\t" )
	.zero	6
.LC73:
	ASCII(.string	"s32DispState:%d\t" )
	.zero	7
.LC75:
	ASCII(.string	"LogicFs:Null\t" )
	.zero	2
.LC77:
	ASCII(.string	"DecFs:%p\t" )
	.zero	6
.LC79:
	ASCII(.string	"DecTick:%d\t" )
	.zero	4
.LC81:
	ASCII(.string	"DecFs:Null\t" )
	.zero	4
.LC83:
	ASCII(.string	"DispFs:%p\t" )
	.zero	5
.LC85:
	ASCII(.string	"DispTick:%d\t" )
	.zero	3
.LC87:
	ASCII(.string	"DispFs:Null\t" )
	.zero	3
.LC90:
	ASCII(.string	"%s num_tile_columns(%d) out of range(0, %d].\n" )
	.zero	2
.LC92:
	ASCII(.string	"%s num_tile_rows(%d) out of range(0, %d].\n" )
	.zero	5
.LC95:
	ASCII(.string	"%s 0 idx(%d) out of range(0, 512).\n" )
	.zero	4
.LC97:
	ASCII(.string	"%s 1 idx(%d) out of range(0, 512).\n" )
	.zero	4
.LC99:
	ASCII(.string	"firmware/common/syntax/hevc.c" )
	.zero	2
.LC101:
	ASCII(.string	"NULL pointer: %s, L%d\n" )
	.zero	1
.LC111:
	ASCII(.string	"pListX[0][%d] = NULL, listXsize[0] = %d\n" )
	.zero	7
.LC113:
	ASCII(.string	"pListX[0][%d]->frame_store = NULL, listXsize[0] = %d\n" )
	.zero	2
.LC115:
	ASCII(.string	"pListX[0][%d] frame_store(%p), pstLogicFs(%p) invalid\n" )
	.zero	1
.LC117:
	ASCII(.string	"pListX[1][%d] = NULL, listXsize[1] = %d\n" )
	.zero	7
.LC119:
	ASCII(.string	"pListX[1][%d]->frame_store = NULL, listXsize[1] = %d\n" )
	.zero	2
.LC121:
	ASCII(.string	"pListX[i][%d] pstLogicFs(%p) invalid\n" )
	.zero	2
.LC124:
	ASCII(.string	"%s can't find such pic.\n" )
	.zero	7
.LC126:
	ASCII(.string	"%s DPB.used_size(%d) >= 17.\n" )
	.zero	3
.LC128:
	ASCII(.string	"%s find pos=%d, poc=%d.\n" )
	.zero	7
.LC131:
	ASCII(.string	"%s DPB.size(%d) already reached max(%d).\n" )
	.zero	6
.LC133:
	ASCII(.string	"%s increase DPB size to %d\n" )
	.zero	4
.LC135:
	ASCII(.string	"%s no enough fs(total_fs_num=%d) to increase DPB size.\n" )
.LC137:
	ASCII(.string	"TileWidthInCtb is 0\n" )
	.zero	3
.LC143:
	ASCII(.string	"image_id=%d, pmv_idc=%d\n" )
	.zero	7
.LC145:
	ASCII(.string	"pic msg burst0:\n" )
	.zero	7
.LC149:
	ASCII(.string	"%s call HEVC_CalcTileInfo failed.\n" )
	.zero	5
.LC151:
	ASCII(.string	"logic frame id(=%d): get LogicFs error!\n" )
	.zero	7
.LC153:
	ASCII(.string	"phy fs is null: pstDecodeFs = %p, pstDispOutFs = %p\n" )
	.zero	3
.LC156:
	ASCII(.string	"%s ERROR: bHaveIndependentSlice = 0\n\n" )
	.zero	2
.LC158:
	ASCII(.string	"ListX ERROR: list0size >= %d\n" )
	.zero	2
.LC160:
	ASCII(.string	"ListX ERROR: list1size >= %d\n" )
	.zero	2
.LC162:
	ASCII(.string	"pListX[0][%d] = NULL\n" )
	.zero	2
.LC164:
	ASCII(.string	"pListX[0][%d]->frame_store = NULL\n" )
	.zero	5
.LC166:
	ASCII(.string	"pListX[1][%d] = NULL\n" )
	.zero	2
.LC168:
	ASCII(.string	"pListX[1][%d]->frame_store = NULL\n" )
	.zero	5
.LC170:
	ASCII(.string	"NumSlicePara(%d) >= MaxSliceNum(%d)\n" )
	.zero	3
.LC173:
	ASCII(.string	"line:%d pic_parameter_set_id(%d) out of range(0,%d).\n" )
	.zero	2
.LC175:
	ASCII(.string	"line:%d seq_parameter_set_id(%d) out of range[0,%d].\n" )
	.zero	2
.LC177:
	ASCII(.string	"line:%d, i(%d) > Max(%d) num_of_pics %d\n" )
	.zero	7
.LC179:
	ASCII(.string	"line:%d, i(%d) > Max(%d)(neg%d + pos%d)\n" )
	.zero	7
.LC182:
	ASCII(.string	"%s unkown slice type(%d).\n" )
	.zero	5
.LC184:
	ASCII(.string	"%s pListX[%d] no available pic to repair.\n" )
	.zero	5
.LC186:
	ASCII(.string	"%s: pListX[%d][%d] = NULL, repair it with pos(%d).\n" )
	.zero	4
.LC190:
	ASCII(.string	"HEVC_InitListX error.\n" )
	.zero	1
.LC192:
	ASCII(.string	"HEVC_RepairList error.\n" )
.LC194:
	ASCII(.string	"release(apc=%d, pmvIdc=%d)\t" )
	.zero	4
.LC198:
	ASCII(.string	"framestore %d allocate apc %d\n" )
	.zero	1
.LC200:
	ASCII(.string	"find APC, but logic_fs_id %d abnormal(poc=%d, ref=%d, logic_fs=%p, dec_fs=%p)\n" )
	.zero	1
.LC203:
	ASCII(.string	"%s frame logic_fs_id=%d, state=%d\n" )
	.zero	5
.LC205:
	ASCII(.string	"LogicFs(fs=%p, poc=%d, FirstFrameState=%d) is null, logic_fs_id = %d\n" )
	.zero	2
.LC207:
	ASCII(.string	"%s frame poc=%d, is already output\n" )
	.zero	4
.LC209:
	ASCII(.string	"%s frame(%p) invalid, state:%d, non_existing:%d\n" )
	.zero	7
.LC213:
	ASCII(.string	"%s: pImg is NULL!\n" )
	.zero	5
.LC215:
	ASCII(.string	"%s, REPORT_IFRAME_ERR\n" )
	.zero	1
.LC218:
	ASCII(.string	"%s: InsertImgToVoQueue failed!\n" )
.LC220:
	ASCII(.string	"%s: ImgErrorLevel = %d, OutErrThr:%d, discarded!\n" )
	.zero	6
.LC223:
	ASCII(.string	"%s output current picture failed\n" )
	.zero	6
.LC225:
	ASCII(.string	"HEVC_DECRecycleImage ERROR, DispState = FS_DISP_STATE_NOT_USED\n" )
.LC228:
	ASCII(.string	"hevc partition fs memory fail!\n" )
.LC230:
	ASCII(.string	"ERROR: HEVC FSP_ConfigInstance fail!\n" )
	.zero	2
.LC235:
	ASCII(.string	"FS_ALLOC_ERR, ClearAll\n" )
.LC238:
	ASCII(.string	"Can not new logic fs! ClearAll\n" )
.LC240:
	ASCII(.string	"FSP_GetFspType -1\n" )
	.zero	5
.LC242:
	ASCII(.string	"FSP_GetLogicFs return NULL\n" )
	.zero	4
.LC244:
	ASCII(.string	"%s: pHevcCtx->DPB.used_size(%d) > DPB.size(%d), error resilence.\n" )
	.zero	6
.LC246:
	ASCII(.string	"nal_segment = %d is not expected value\n" )
.LC248:
	ASCII(.string	"HEVC_VpsSpsPpsCheck pic_parameter_set_id(%d) out of range\n" )
	.zero	5
.LC250:
	ASCII(.string	"pps with this pic_parameter_set_id = %d havn't be decoded\n" )
	.zero	5
.LC252:
	ASCII(.string	"HEVC_VpsSpsPpsCheck seq_parameter_set_id(%d) out of range\n" )
	.zero	5
.LC254:
	ASCII(.string	"sps with this seq_parameter_set_id = %d havn't be decoded\n" )
	.zero	5
.LC256:
	ASCII(.string	"HEVC_VpsSpsPpsCheck video_parameter_set_id out of range\n" )
	.zero	7
.LC258:
	ASCII(.string	"vps with this video_parameter_set_id = %d havn't be decoded\n" )
	.zero	3
.LC261:
	ASCII(.string	"%s, %d, CurrSlice.poc = %d, PocRandomAccess = %d, nal_unit_type = %d, should skip this frame\n" )
	.zero	2
.LC263:
	ASCII(.string	"%s, %d, CurrSlice.poc = %d, bNoRaslOutputFlag = %d, nal_unit_type = %d, should skip this frame\n" )
.LC265:
	ASCII(.string	"%s, %d, PrevRAPIsBLA = %d, CurrSlice.poc = %d, PocCRA = %d, nal_unit_type = %d, should skip this frame\n" )
.LC267:
	ASCII(.string	"HEVC_ReadByteAlignment: code should not be zero.\n" )
	.zero	6
.LC269:
	ASCII(.string	"HEVC_ReadByteAlignment: code:%d\n" )
	.zero	7
.LC272:
	ASCII(.string	"general_profile_compatibility_flag[][%d]" )
	.zero	7
.LC274:
	ASCII(.string	"reserved_zero_2bits not equal 0.\n" )
	.zero	6
.LC283:
	ASCII(.string	"HEVC_GetScalingListDefaultAddress NULL.\n" )
	.zero	7
.LC285:
	ASCII(.string	"Error, BsLen: 0x%x, TotalPos: 0x%x\n" )
	.zero	4
.LC287:
	ASCII(.string	"Panic: All zero data sequence in RBSP \n" )
.LC289:
	ASCII(.string	"nal_trail_zero_bit_num err\n" )
	.zero	4
.LC294:
	ASCII(.string	"%s release streambuff=%p, bitstream_length=%d\n" )
	.zero	1
.LC298:
	ASCII(.string	"HEVC_CombinePacket FATAL: pCurrNal=NULL!\n" )
	.zero	6
.LC300:
	ASCII(.string	"END of the bit buffer, copy the first packet!\n" )
	.zero	1
.LC303:
	ASCII(.string	"HEVC_ue_v ERROR: LeadingZeros (%d) >= 32, return 0x%x\n" )
	.zero	1
.LC305:
	ASCII(.string	"tone_map_id" )
	.zero	4
.LC307:
	ASCII(.string	"Unsupport: target_bit_depth(%d) > 8\n" )
	.zero	3
.LC310:
	ASCII(.string	"pTon->num_pivots > 256\n" )
.LC312:
	ASCII(.string	"frame_packing_arrangement_id" )
	.zero	3
.LC314:
	ASCII(.string	"frame_packing_arrangement_type(%d) out of range\n" )
	.zero	7
.LC316:
	ASCII(.string	"decoding_unit_idx" )
	.zero	6
.LC318:
	ASCII(.string	"num_sps_ids_minus1" )
	.zero	5
.LC320:
	ASCII(.string	"pAct->num_sps_ids_minus1 > 15\n" )
	.zero	1
.LC322:
	ASCII(.string	"active_seq_parameter_set_id" )
	.zero	4
.LC324:
	ASCII(.string	"bp_seq_parameter_set_id" )
.LC326:
	ASCII(.string	"Wrong bp_seq_parameter_set_id = %d, should be 0 to 15!\n" )
.LC328:
	ASCII(.string	"HEVC_Sei_Buf_Period cpb_cnt_minus1(%d) out of range(0,31).\n" )
	.zero	4
.LC330:
	ASCII(.string	"num_decoding_units_minus1" )
	.zero	6
.LC332:
	ASCII(.string	"pPic->num_decoding_units_minus1 > 255\n" )
	.zero	1
.LC334:
	ASCII(.string	"num_nalus_in_du_minus1" )
	.zero	1
.LC336:
	ASCII(.string	"delta_idx_minus1" )
	.zero	7
.LC338:
	ASCII(.string	"delta_idx(%d) > idx(%d).\n" )
	.zero	6
.LC340:
	ASCII(.string	"rIdx(%d) > (idx-1)(%d) or < 0).\n" )
	.zero	7
.LC342:
	ASCII(.string	"abs_delta_rps_minus1" )
	.zero	3
.LC344:
	ASCII(.string	"pTempRPset->num_of_pics(%d) out of range(0,15).\n" )
	.zero	7
.LC346:
	ASCII(.string	"num_of_pics out of range(0,15).\n" )
	.zero	7
.LC348:
	ASCII(.string	"num_negative_pics" )
	.zero	6
.LC350:
	ASCII(.string	"pTempRPset->num_negative_pics(%d) out of range(0,15).\n" )
	.zero	1
.LC352:
	ASCII(.string	"num_positive_pics" )
	.zero	6
.LC354:
	ASCII(.string	"pTempRPset->num_positive_pics(%d) out of range(0,15).\n" )
	.zero	1
.LC356:
	ASCII(.string	"pShortTermRpset->num_of_pics(%d) out of range[0,%d].\n" )
	.zero	2
.LC358:
	ASCII(.string	"delta_poc_s0_minus1" )
	.zero	4
.LC360:
	ASCII(.string	"delta_poc_s0_minus1(%d) out of range.\n" )
	.zero	1
.LC362:
	ASCII(.string	"delta_poc_s1_minus1" )
	.zero	4
.LC364:
	ASCII(.string	"elemental_duration_in_tc_minus1" )
.LC366:
	ASCII(.string	"cpb_cnt_minus1" )
	.zero	1
.LC368:
	ASCII(.string	"cpb_cnt_minus1(%d) out of range(0,31).\n" )
.LC370:
	ASCII(.string	"bit_rate_value_minus1" )
	.zero	2
.LC372:
	ASCII(.string	"cpb_size_value_minus1" )
	.zero	2
.LC374:
	ASCII(.string	"cpb_size_du_value_minus1" )
	.zero	7
.LC376:
	ASCII(.string	"bit_rate_du_value_minus1" )
	.zero	7
.LC378:
	ASCII(.string	"vps_reserved_three_2bits(%d) not equal 0x3.\n" )
	.zero	3
.LC380:
	ASCII(.string	"vps_max_layers_minus1 out of range(0,63).\n" )
	.zero	5
.LC382:
	ASCII(.string	"vps_max_sub_layers_minus1=%d\n" )
	.zero	2
.LC384:
	ASCII(.string	"vps_reserved_0xffff_16bits not equal 0xffff.\n" )
	.zero	2
.LC386:
	ASCII(.string	"VPS HEVC_DecPTL error.\n" )
.LC388:
	ASCII(.string	"vps_num_layer_sets_minus1" )
	.zero	6
.LC390:
	ASCII(.string	"vps_max_dec_pic_buffering_minus1[i]" )
	.zero	4
.LC392:
	ASCII(.string	"vps_num_reorder_pics[i]" )
.LC394:
	ASCII(.string	"vps_max_latency_increase_plus1[i]" )
	.zero	6
.LC396:
	ASCII(.string	"vps_num_layer_sets_minus1(%d) out of range(0,%d).\n" )
	.zero	5
.LC399:
	ASCII(.string	"layer_id_included_flag[%d][%d]" )
	.zero	1
.LC401:
	ASCII(.string	"vps_num_ticks_poc_diff_one_minus1" )
	.zero	6
.LC403:
	ASCII(.string	"vps_num_hrd_parameters" )
	.zero	1
.LC405:
	ASCII(.string	"vps_num_hrd_parameters(%d) out of range(0,%d).\n" )
.LC407:
	ASCII(.string	"hrd_layer_set_idx" )
	.zero	6
.LC409:
	ASCII(.string	"VPS HEVC_DecHrdParam error.\n" )
	.zero	3
.LC411:
	ASCII(.string	"pVPS->vps_video_parameter_set_id out of range(0,15).\n" )
	.zero	2
.LC414:
	ASCII(.string	"VPS[%d] decode error0.\n" )
.LC417:
	ASCII(.string	"VPS[%d] decode error1.\n" )
.LC420:
	ASCII(.string	"chroma_sample_loc_type_top_field" )
	.zero	7
.LC422:
	ASCII(.string	"chroma_sample_loc_type_bottom_field" )
	.zero	4
.LC424:
	ASCII(.string	"def_disp_win_left_offset" )
	.zero	7
.LC426:
	ASCII(.string	"VUI pSps->chroma_format_idc out of range(0,3).\n" )
.LC428:
	ASCII(.string	"def_disp_win_right_offset" )
	.zero	6
.LC430:
	ASCII(.string	"def_disp_win_top_offset" )
.LC432:
	ASCII(.string	"def_disp_win_bottom_offset" )
	.zero	5
.LC434:
	ASCII(.string	"vui_num_ticks_poc_diff_one_minus1" )
	.zero	6
.LC436:
	ASCII(.string	"VUI HEVC_DecHrdParam error.\n" )
	.zero	3
.LC438:
	ASCII(.string	"min_spatial_segmentation_idc" )
	.zero	3
.LC440:
	ASCII(.string	"VUI min_spatial_segmentation_idc out of range(0,4095).\n" )
.LC442:
	ASCII(.string	"max_bytes_per_pic_denom" )
.LC444:
	ASCII(.string	"max_bits_per_min_cu_denom" )
	.zero	6
.LC446:
	ASCII(.string	"log2_max_mv_length_horizontal" )
	.zero	2
.LC448:
	ASCII(.string	"log2_max_mv_length_vertical" )
	.zero	4
.LC450:
	ASCII(.string	"sop_seq_parameter_set_id" )
	.zero	7
.LC452:
	ASCII(.string	"num_entries_in_sop_minus1" )
	.zero	6
.LC454:
	ASCII(.string	"num_entries_in_sop_minus1 out of range(0,1024)\n" )
.LC456:
	ASCII(.string	"sop_short_term_rps_idx" )
	.zero	1
.LC458:
	ASCII(.string	"sop_poc_delta" )
	.zero	2
.LC460:
	ASCII(.string	"recovery_poc_cnt" )
	.zero	7
.LC463:
	ASCII(.string	"reserved_payload_extension_data" )
.LC465:
	ASCII(.string	"line:%d Cosume TotalPos(%d > %d)!!!\n" )
	.zero	3
.LC467:
	ASCII(.string	"HEVC_SeiMessage HEVC_Sei_Payload error.\n" )
	.zero	7
.LC469:
	ASCII(.string	"SEI HEVC_SeiMessage error.\n" )
	.zero	4
.LC471:
	ASCII(.string	"SEI BsIsByteAligned error.\n" )
	.zero	4
.LC473:
	ASCII(.string	"SEI rbsp_trailing_bits(%d) not equal 0x80.\n" )
	.zero	4
.LC475:
	ASCII(.string	"nesting_num_ops_minus1" )
	.zero	1
.LC477:
	ASCII(.string	"nesting_num_ops_minus1 out of range(0,%d)\n" )
	.zero	5
.LC479:
	ASCII(.string	"nesting_op_idx" )
	.zero	1
.LC481:
	ASCII(.string	"nesting_num_layers_minus1" )
	.zero	6
.LC483:
	ASCII(.string	"nesting_num_layers_minus1 out of range(0,63)\n" )
	.zero	2
.LC488:
	ASCII(.string	"scaling_list_pred_matrix_id_delta" )
	.zero	6
.LC490:
	ASCII(.string	"scaling_list_pred_matrix_id_delta out of range(0,matrixId).\n" )
	.zero	3
.LC492:
	ASCII(.string	"HEVC_DecScalingListData pMatrixAddress NULL.\n" )
	.zero	2
.LC494:
	ASCII(.string	"scaling_list_dc_coef_minus8" )
	.zero	4
.LC496:
	ASCII(.string	"scaling_list_dc_coef_minus8 out of range(-7,247).\n" )
	.zero	5
.LC498:
	ASCII(.string	"scaling_list_delta_coef" )
.LC500:
	ASCII(.string	"scaling_list_dc_coef_minus8 out of range(-128,127).\n" )
	.zero	3
.LC503:
	ASCII(.string	"chroma_format_idc" )
	.zero	6
.LC505:
	ASCII(.string	"pSPS->chroma_format_idc out of range(0,3).\n" )
	.zero	4
.LC507:
	ASCII(.string	"pSPS->chroma_format_idc not equal(%d) 1.\n" )
	.zero	6
.LC509:
	ASCII(.string	"pic_width_in_luma_samples" )
	.zero	6
.LC511:
	ASCII(.string	"pic_width_in_luma_samples out of range(0,8192).\n" )
	.zero	7
.LC513:
	ASCII(.string	"pic_height_in_luma_samples" )
	.zero	5
.LC515:
	ASCII(.string	"pic_height_in_luma_samples out of range(0,4096).\n" )
	.zero	6
.LC517:
	ASCII(.string	"ERROR Unkown level: %d\n" )
.LC519:
	ASCII(.string	"conf_win_left_offset" )
	.zero	3
.LC522:
	ASCII(.string	"conf_win_right_offset" )
	.zero	2
.LC524:
	ASCII(.string	"conf_win_left_offset+conf_win_right_offset out of range.\n" )
	.zero	6
.LC526:
	ASCII(.string	"conf_win_top_offset" )
	.zero	4
.LC528:
	ASCII(.string	"conf_win_bottom_offset" )
	.zero	1
.LC530:
	ASCII(.string	"conf_win_top_offset+conf_win_bottom_offset out of range.\n" )
	.zero	6
.LC532:
	ASCII(.string	"bit_depth_luma_minus8" )
	.zero	2
.LC534:
	ASCII(.string	"bit_depth_luma(%d) out of range(8,14).\n" )
.LC536:
	ASCII(.string	"bit_depth_chroma_minus8" )
.LC538:
	ASCII(.string	"bit_depth_chroma(%d) out of range[8,14].\n" )
	.zero	6
.LC540:
	ASCII(.string	"log2_max_pic_order_cnt_lsb_minus4" )
	.zero	6
.LC542:
	ASCII(.string	"log2_max_pic_order_cnt_lsb_minus4 out of range[0,12].\n" )
	.zero	1
.LC544:
	ASCII(.string	"sps_max_dec_pic_buffering_minus1" )
	.zero	7
.LC546:
	ASCII(.string	"sps_num_reorder_pics" )
	.zero	3
.LC548:
	ASCII(.string	"sps_max_latency_increase_plus1" )
	.zero	1
.LC550:
	ASCII(.string	"log2_min_coding_block_size_minus3" )
	.zero	6
.LC552:
	ASCII(.string	"log2_min_luma_coding_block_size_minus3 out of range(0,3).\n" )
	.zero	5
.LC554:
	ASCII(.string	"log2_diff_max_min_coding_block_size" )
	.zero	4
.LC556:
	ASCII(.string	"log2_diff_max_min_luma_coding_block_size out of range(0,3).\n" )
	.zero	3
.LC558:
	ASCII(.string	"log2_ctb_sizeY out of range(4,6).\n" )
	.zero	5
.LC560:
	ASCII(.string	"log2_min_transform_block_size_minus2" )
	.zero	3
.LC562:
	ASCII(.string	"log2_min_transform_block_size_minus2 out of range(0,3).\n" )
	.zero	7
.LC564:
	ASCII(.string	"log2_diff_max_min_transform_block_size" )
	.zero	1
.LC566:
	ASCII(.string	"log2_diff_max_min_transform_block_size out of range(0,3).\n" )
	.zero	5
.LC568:
	ASCII(.string	"quadtree_tu_log2_min_size not less than log2_min_cb_sizeY.\n" )
	.zero	4
.LC570:
	ASCII(.string	"quadtree_tu_log2_max_size greater than Min( CtbLog2SizeY, 5 ).\n" )
.LC572:
	ASCII(.string	"max_transform_hierarchy_depth_inter" )
	.zero	4
.LC574:
	ASCII(.string	"max_transform_hierarchy_depth_inter out of range(0,CtbLog2SizeY-Log2MinTrafoSize).\n" )
	.zero	4
.LC576:
	ASCII(.string	"max_transform_hierarchy_depth_intra" )
	.zero	4
.LC578:
	ASCII(.string	"max_transform_hierarchy_depth_intra out of range(0,CtbLog2SizeY-Log2MinTrafoSize).\n" )
	.zero	4
.LC580:
	ASCII(.string	"SPS HEVC_DecScalingListData error.\n" )
	.zero	4
.LC582:
	ASCII(.string	"SPS sao is off and pic height(%d) is too small.(Logic Unsupported).\n" )
	.zero	3
.LC585:
	ASCII(.string	"log2_min_pcm_luma_coding_block_size_minus3" )
	.zero	5
.LC587:
	ASCII(.string	"log2_min_pcm_coding_block_size_minus3(%d) out of range[0,2].\n" )
	.zero	2
.LC589:
	ASCII(.string	"log2_diff_max_min_pcm_luma_coding_block_size" )
	.zero	3
.LC591:
	ASCII(.string	"log2_diff_max_min_pcm_coding_block_size(%d) out of range[0,2].\n" )
.LC593:
	ASCII(.string	"pcm_log2_max_size greater than Min( CtbLog2SizeY, 5 ).\n" )
.LC595:
	ASCII(.string	"num_short_term_ref_pic_sets" )
	.zero	4
.LC597:
	ASCII(.string	"num_short_term_ref_pic_sets out of range[0,%d].\n" )
	.zero	7
.LC599:
	ASCII(.string	"SPS HEVC_DecShortTermRefPicSet error.\n" )
	.zero	1
.LC601:
	ASCII(.string	"num_long_term_ref_pic_sps" )
	.zero	6
.LC603:
	ASCII(.string	"num_long_term_ref_pic_sps out of range[0,32].\n" )
	.zero	1
.LC605:
	ASCII(.string	"SPS HEVC_DecVuiParam error.\n" )
	.zero	3
.LC609:
	ASCII(.string	"sps_video_parameter_set_id out of range(0,%d).\n" )
.LC611:
	ASCII(.string	"sps_max_sub_layers_minus1 out of range(0,%d).\n" )
	.zero	1
.LC613:
	ASCII(.string	"SPS HEVC_DecPTL error.\n" )
.LC615:
	ASCII(.string	"sps_seq_parameter_set_id" )
	.zero	7
.LC617:
	ASCII(.string	"sps_seq_parameter_set_id out of range(0,%d).\n" )
	.zero	2
.LC620:
	ASCII(.string	"SPS[%d] decode error0.\n" )
.LC622:
	ASCII(.string	"SPS[%d] decode error.\n" )
	.zero	1
.LC625:
	ASCII(.string	"pps_seq_parameter_set_id" )
	.zero	7
.LC627:
	ASCII(.string	"pic_parameter_set_id(%d) out of range[0,15].\n" )
	.zero	2
.LC629:
	ASCII(.string	"SPS(%d) haven't decode.\n" )
	.zero	7
.LC631:
	ASCII(.string	"num_ref_idx_l0_default_active_minus1" )
	.zero	3
.LC633:
	ASCII(.string	"num_ref_idx_l0_default_active out of range[0,15].\n" )
	.zero	5
.LC635:
	ASCII(.string	"num_ref_idx_l1_default_active_minus1" )
	.zero	3
.LC637:
	ASCII(.string	"num_ref_idx_l1_default_active out of range[0,15].\n" )
	.zero	5
.LC639:
	ASCII(.string	"init_qp_minus26" )
.LC641:
	ASCII(.string	"pic_init_qp(%d) out of range[-%d,51].\n" )
	.zero	1
.LC643:
	ASCII(.string	"diff_cu_qp_delta_depth" )
	.zero	1
.LC645:
	ASCII(.string	"diff_cu_qp_delta_depth out of range[0,3].\n" )
	.zero	5
.LC647:
	ASCII(.string	"pps_cb_qp_offset" )
	.zero	7
.LC649:
	ASCII(.string	"pic_cb_qp_offset out of range[-12,12].\n" )
.LC651:
	ASCII(.string	"pps_cr_qp_offset" )
	.zero	7
.LC653:
	ASCII(.string	"pic_cr_qp_offset out of range[-12,12].\n" )
.LC655:
	ASCII(.string	"num_tile_columns_minus1" )
.LC657:
	ASCII(.string	"num_tile_columns(%d) out of range(0,%d).\n" )
	.zero	6
.LC659:
	ASCII(.string	"num_tile_rows_minus1" )
	.zero	3
.LC661:
	ASCII(.string	"num_tile_rows out(%d) of range(0,%d).\n" )
	.zero	1
.LC663:
	ASCII(.string	"Logic limit(%d), num_tile_columns(%d) exceed.\n" )
	.zero	1
.LC666:
	ASCII(.string	"Logic limit(%d), num_tile_rows(%d) exceed.\n" )
	.zero	4
.LC668:
	ASCII(.string	"column_width_minus1" )
	.zero	4
.LC670:
	ASCII(.string	"column_width[%d](%d) out of range (0 %d)\n" )
	.zero	6
.LC672:
	ASCII(.string	"row_height_minus1" )
	.zero	6
.LC674:
	ASCII(.string	"row_height[%d](%d) out of range (0 %d)\n" )
.LC676:
	ASCII(.string	"column_bd[%d](%d) <= 0, invalid!\n" )
	.zero	6
.LC678:
	ASCII(.string	"row_bd[%d](%d) <= 0, invalid!\n" )
	.zero	1
.LC680:
	ASCII(.string	"PPS tile width(%d) is too small.(Logic Unsupport)\n" )
	.zero	5
.LC682:
	ASCII(.string	"PPS tile height(%d) is too small.(Logic Unsupport)\n" )
	.zero	4
.LC684:
	ASCII(.string	"pps_beta_offset_div2" )
	.zero	3
.LC686:
	ASCII(.string	"pps_beta_offset_div2(%d) out of range(-6,6).\n" )
	.zero	2
.LC688:
	ASCII(.string	"pps_tc_offset_div2" )
	.zero	5
.LC690:
	ASCII(.string	"pps_tc_offset_div2(%d) out of range(-6,6).\n" )
	.zero	4
.LC692:
	ASCII(.string	"PPS HEVC_DecScalingListData error.\n" )
	.zero	4
.LC694:
	ASCII(.string	"log2_parallel_merge_level_minus2" )
	.zero	7
.LC696:
	ASCII(.string	"log2_parallel_merge_level(%d) out of range(2,log2_ctb_sizeY).\n" )
	.zero	1
.LC698:
	ASCII(.string	"pps_pic_parameter_set_id" )
	.zero	7
.LC700:
	ASCII(.string	"pic_parameter_set_id(%d) out of range(0,%d).\n" )
	.zero	2
.LC703:
	ASCII(.string	"PPS[%d] decode error.\n" )
	.zero	1
.LC707:
	ASCII(.string	"luma_log2_weight_denom" )
	.zero	1
.LC709:
	ASCII(.string	"luma_log2_weight_denom out of range(0,7).\n" )
	.zero	5
.LC711:
	ASCII(.string	"delta_chroma_log2_weight_denom" )
	.zero	1
.LC713:
	ASCII(.string	"delta_chroma_log2_weight_denom out of range(-7,7).\n" )
	.zero	4
.LC715:
	ASCII(.string	"chroma_log2_weight_denom out of range(0,7).\n" )
	.zero	3
.LC717:
	ASCII(.string	"delta_luma_weight_l0" )
	.zero	3
.LC719:
	ASCII(.string	"delta_luma_weight_l0(%d) out of range(-128,127).\n" )
	.zero	6
.LC721:
	ASCII(.string	"luma_offset_l0" )
	.zero	1
.LC723:
	ASCII(.string	"luma_offset_l0(%d) out of range(-128,127).\n" )
	.zero	4
.LC725:
	ASCII(.string	"delta_chroma_weight_l0" )
	.zero	1
.LC727:
	ASCII(.string	"delta_chroma_weight_l0 out of range(-128,127).\n" )
.LC729:
	ASCII(.string	"delta_chroma_offset_l0" )
	.zero	1
.LC731:
	ASCII(.string	"delta_chroma_offset_l0 out of range(-512,511).\n" )
.LC733:
	ASCII(.string	"delta_luma_weight_l1" )
	.zero	3
.LC735:
	ASCII(.string	"delta_luma_weight_l1 out of range(-128,127).\n" )
	.zero	2
.LC737:
	ASCII(.string	"luma_offset_l1" )
	.zero	1
.LC739:
	ASCII(.string	"luma_offset_l1 out of range(-128,127).\n" )
.LC741:
	ASCII(.string	"delta_chroma_weight_l1" )
	.zero	1
.LC743:
	ASCII(.string	"delta_chroma_weight_l1 out of range(-128,127).\n" )
.LC745:
	ASCII(.string	"delta_chroma_offset_l1" )
	.zero	1
.LC747:
	ASCII(.string	"delta_chroma_offset_l1 out of range(-512,511).\n" )
.LC749:
	ASCII(.string	"slice_pic_parameter_set_id" )
	.zero	5
.LC752:
	ASCII(.string	"%s: pic_parameter_set_id(%d) out of range(0 %d)!\n" )
	.zero	6
.LC754:
	ASCII(.string	"HEVC_VpsSpsPpsCheck != HEVC_DEC_NORMAL\n" )
.LC756:
	ASCII(.string	"max_cu_width(%d)/max_cu_height(%d) = 0\n" )
.LC758:
	ASCII(.string	"slice_segment_address(%d) out of range(0,%d).\n" )
	.zero	1
.LC760:
	ASCII(.string	"slice_type" )
	.zero	5
.LC762:
	ASCII(.string	"slice_reserved_flag: %d\n" )
	.zero	7
.LC764:
	ASCII(.string	"slice_type(%d) out of range(0,2].\n" )
	.zero	5
.LC766:
	ASCII(.string	"HEVC_ClearDPB,LastDisplayPoc:%d poc:%d \n" )
	.zero	7
.LC768:
	ASCII(.string	"SH HEVC_DecShortTermRefPicSet error.\n" )
	.zero	2
.LC770:
	ASCII(.string	"short_term_ref_pic_set_idx(%d) out of range[0,%d].\n" )
	.zero	4
.LC772:
	ASCII(.string	"num_long_term_sps" )
	.zero	6
.LC774:
	ASCII(.string	"num_long_term_sps(%d) out of range[0,%d].\n" )
	.zero	5
.LC776:
	ASCII(.string	"num_long_term_pics" )
	.zero	5
.LC778:
	ASCII(.string	"SH ERROR: (num_negative_pics(%d) + num_positive_pics(%d) + num_of_longterm_pics(%d)) > %d\n" )
	.zero	5
.LC780:
	ASCII(.string	"lt_idx_sps[%d] = %d, but out of range[0,%d].\n" )
	.zero	2
.LC782:
	ASCII(.string	"delta_poc_msb_cycle_lt" )
	.zero	1
.LC784:
	ASCII(.string	"ERROR: pRPS->num_of_pics(%d) > %d.\n" )
	.zero	4
.LC786:
	ASCII(.string	"num_ref_idx_l0_active_minus1" )
	.zero	3
.LC788:
	ASCII(.string	"num_ref_idx_l0_active(%d) out of range(0,%d).\n" )
	.zero	1
.LC790:
	ASCII(.string	"num_ref_idx_l1_active_minus1" )
	.zero	3
.LC792:
	ASCII(.string	"num_ref_idx_l1_active(%d) out of range(0,%d).\n" )
	.zero	1
.LC794:
	ASCII(.string	"pRPS is null.\n" )
	.zero	1
.LC796:
	ASCII(.string	"pRPS->num_of_pics out(%d) of range(0,%d).\n" )
	.zero	5
.LC798:
	ASCII(.string	"SH HEVC_DecRefPicListsModification error.\n" )
	.zero	5
.LC800:
	ASCII(.string	"collocated_ref_idx" )
	.zero	5
.LC802:
	ASCII(.string	"collocated_from_l0_flag=%d, collocated_ref_idx(%d) out of range(0,%d).\n" )
.LC804:
	ASCII(.string	"SH HEVC_DecPredWeightTable error.\n" )
	.zero	5
.LC806:
	ASCII(.string	"five_minus_max_num_merge_cand" )
	.zero	2
.LC808:
	ASCII(.string	"SH max_num_merge_cand(%d) out of range(1,5).\n" )
	.zero	2
.LC810:
	ASCII(.string	"slice_qp_delta" )
	.zero	1
.LC812:
	ASCII(.string	"SH slice_qp(%d) out of range [-%d,51].\n" )
.LC814:
	ASCII(.string	"slice_cb_qp_offset" )
	.zero	5
.LC816:
	ASCII(.string	"SH slice_cb_qp_offset(%d) out of range[-12,12].\n" )
	.zero	7
.LC818:
	ASCII(.string	"SH slice_cb_qp_offset(%d)+pic_cb_qp_offset(%d) out of range[-12,12].\n" )
	.zero	2
.LC820:
	ASCII(.string	"slice_cr_qp_offset" )
	.zero	5
.LC822:
	ASCII(.string	"SH slice_cr_qp_offset(%d) out of range[-12,12].\n" )
	.zero	7
.LC824:
	ASCII(.string	"SH slice_cr_qp_offset+pPPS->pic_cr_qp_offset(%d) out of range[-12,12].\n" )
.LC826:
	ASCII(.string	"slice_beta_offset_div2" )
	.zero	1
.LC828:
	ASCII(.string	"SH slice_beta_offset_div2(%d) out of range(-6,6).\n" )
	.zero	5
.LC830:
	ASCII(.string	"slice_tc_offset_div2" )
	.zero	3
.LC832:
	ASCII(.string	"SH slice_tc_offset_div2(%d) out of range(-6,6).\n" )
	.zero	7
.LC834:
	ASCII(.string	"num_entry_point_offsets" )
.LC836:
	ASCII(.string	"CurrSlice.num_entry_point_offsets(%d) out of range[0,%d].\n" )
	.zero	5
.LC838:
	ASCII(.string	"offset_len_minus1" )
	.zero	6
.LC840:
	ASCII(.string	"CurrSlice.offset_len(%d) out of range[1,32].\n" )
	.zero	2
.LC842:
	ASCII(.string	"slice_segment_header_extension_length" )
	.zero	2
.LC844:
	ASCII(.string	"slice_segment_header_extension_length(%d) out of range [0,256]\n" )
.LC846:
	ASCII(.string	"bitsLeft(%d) < slice_segment_header_extension_length(%d)\n" )
	.zero	6
.LC850:
	ASCII(.string	"%s arrange mem failed.\n" )
.LC852:
	ASCII(.string	"Init dec para\n" )
	.zero	1
.LC857:
	ASCII(.string	"%s: Get context addr failed!\n" )
	.zero	2
.LC859:
	ASCII(.string	"%s FATAL: MemOffset %d > ContextMemSize %d.\n" )
	.zero	3
.LC861:
	ASCII(.string	"%s: VCTRL_GetChanIDByCtx() return -1!\n" )
	.zero	1
.LC864:
	ASCII(.string	"BitDepthChange ReRange FS Luma(%d-->%d),Chroma(%d -->%d)\n" )
	.zero	6
.LC866:
	ASCII(.string	"HEVC_InitDPB error, return %d\n" )
	.zero	1
.LC868:
	ASCII(.string	"HEVC_AllocFrameStore error\n" )
	.zero	4
.LC871:
	ASCII(.string	"%s: pstLogicFsImage is NULL!\n" )
	.zero	2
.LC873:
	ASCII(.string	"HEVC InsertVO Failed return %d, ClearAll\n" )
	.zero	6
.LC875:
	ASCII(.string	"HEVC.c line %d: frame para err(ret=%d), recycle image self\n" )
	.zero	4
.LC878:
	ASCII(.string	"[%d]disp=%d,poc=%d,lpoc=%d,no=%d,pics=%d,cpoc=%d,flag=%d\n" )
	.zero	6
.LC881:
	ASCII(.string	"%s: FirstFrameFastOut LastPoc: %d, CurPos: %d!\n" )
.LC885:
	ASCII(.string	"%s call HEVC_GetUnRefPicWithMinPoc failed\n" )
	.zero	5
.LC887:
	ASCII(.string	"%s call HEVC_OutputOneFrmFromDPB failed\n" )
	.zero	7
.LC890:
	ASCII(.string	"%s: HEVC_OutputFrmToVO err!\n" )
	.zero	3
.LC892:
	ASCII(.string	"cann't find slot for current nal\n" )
	.zero	6
.LC894:
	ASCII(.string	"receive a zero packet\n" )
	.zero	1
.LC896:
	ASCII(.string	"nal_release_err\n" )
	.zero	7
.LC898:
	ASCII(.string	"No frame store available!\n" )
	.zero	5
.LC901:
	ASCII(.string	"%s ReadImgNum = %d, NewImgNum = %d, FrameStore leak, ClearAll!\n" )
.LC904:
	ASCII(.string	"%s: flush dpb failed, return %d\n" )
	.zero	7
.LC907:
	ASCII(.string	"%s: DPB.used_size(%d) >= DPB.size(%d), need to increase DPB size.\n" )
	.zero	5
.LC909:
	ASCII(.string	"%s call HEVC_IncreaseDPBSize failed\n" )
	.zero	3
.LC911:
	ASCII(.string	"%s: DPB.used_size(%d) >= DPB.size(%d), output minpoc frame.\n" )
	.zero	3
.LC916:
	ASCII(.string	"%s with invalid pos(%d)\n" )
	.zero	7
.LC919:
	ASCII(.string	"%s get APC error, ret %d\n" )
	.zero	6
.LC922:
	ASCII(.string	"%s %d: InsertFrmInDPB failed, return %d\n" )
	.zero	7
.LC924:
	ASCII(.string	"%s call HEVC_CheckFrameStore failed!\n" )
	.zero	2
.LC928:
	ASCII(.string	"%s call HEVC_RemoveUnUsedFrameStore Failed.\n" )
	.zero	3
.LC930:
	ASCII(.string	"%s call HEVC_InsertFrmInDPB Failed.\n" )
	.zero	3
.LC935:
	ASCII(.string	"%s call HEVC_FirstFrameFastOut Failed\n" )
	.zero	1
.LC937:
	ASCII(.string	"DPB.used_size may bigger than DPB.size\n" )
.LC939:
	ASCII(.string	"HEVC_InsertFrmInDPB Failed, ret = %d\n" )
	.zero	2
.LC941:
	ASCII(.string	"%s call HEVC_OutputFrmFromDPB failed\n" )
	.zero	2
.LC945:
	ASCII(.string	"%s CurrPic.state = HEVC_PIC_EMPTY, return HEVC_DEC_NORMAL\n" )
	.zero	5
.LC947:
	ASCII(.string	"Discard cur pic, ErrLevel(%d) > OutErrThr(%d)\n" )
	.zero	1
.LC949:
	ASCII(.string	"%s call HEVC_IModeProcess Failed\n" )
	.zero	6
.LC951:
	ASCII(.string	"HEVC_StorePicInDpb return(%d) from L%d\n" )
.LC953:
	ASCII(.string	"%s call HEVC_SimpleDPBProcess Failed\n" )
	.zero	2
.LC955:
	ASCII(.string	"%s call HEVC_DecOrderProcess Failed\n" )
	.zero	3
.LC957:
	ASCII(.string	"%s call HEVC_DispOrderProcess Failed\n" )
	.zero	2
.LC959:
	ASCII(.string	"%s return HEVC_DEC_ERR\n" )
.LC961:
	ASCII(.string	"No frame store for lost pic.\n" )
	.zero	2
.LC963:
	ASCII(.string	"DPB ERROR: Can not get ref_fs!\n" )
.LC965:
	ASCII(.string	"DPB no suited fs for lost pic.\n" )
.LC967:
	ASCII(.string	"Take poc(%d) to creat lost poc(%d).\n" )
	.zero	3
.LC969:
	ASCII(.string	"DPB ERROR: Ref DecodeFs = NULL, ClearDPB!\n" )
	.zero	5
.LC971:
	ASCII(.string	"ERROR: Can not new logic fs!\n" )
	.zero	2
.LC973:
	ASCII(.string	"Lost pic insert in DPB Failed!\n" )
.LC975:
	ASCII(.string	"Ref frame(poc %d) lost.\n" )
	.zero	7
.LC977:
	ASCII(.string	"s32OutErrThr = %d,drop poc %d because lost ref frame(pod %d)\n" )
	.zero	2
.LC980:
	ASCII(.string	"%s call HEVC_FlushOutputFrmFromDPB failed\n" )
	.zero	5
.LC982:
	ASCII(.string	"%s call HEVC_EmptyFrmFromDPB failed\n" )
	.zero	3
.LC984:
	ASCII(.string	"Init pic err, exit\n" )
	.zero	4
.LC986:
	ASCII(.string	"HEVC_WritePicMsg failed!\n" )
	.zero	6
.LC988:
	ASCII(.string	"Start Reason: SliceParaNum= %d\n" )
.LC991:
	ASCII(.string	"Too many slice , err!\n" )
	.zero	1
.LC993:
	ASCII(.string	"Cur pic not init yet, discard this slice!\n" )
	.zero	5
.LC995:
	ASCII(.string	"Dec List error, ret=%d\n" )
.LC997:
	ASCII(.string	"Ref List is Wrong!\n" )
	.zero	4
.LC999:
	ASCII(.string	"HEVC_WriteSliceMsg err.\n" )
	.zero	7
.LC1002:
	ASCII(.string	"HEVC_StorePicInDPB failed, ret = %d\n" )
	.zero	3
.LC1005:
	ASCII(.string	"DecVDM no slice and needn't start VDM, but MaxBytesReceived exceed threshold!\n" )
	.zero	1
.LC1007:
	ASCII(.string	"DecVDM no slice and needn't start VDM\n" )
	.zero	1
.LC1009:
	ASCII(.string	"IPBDecMode Changed (%d -> %d)\n" )
	.zero	1
.LC1012:
	ASCII(.string	"Discard this B(poc=%d) before P, is_ref_idc=%d.\n" )
	.zero	7
.LC1014:
	ASCII(.string	"VDM start, TotalPicNum=%d.\n" )
	.zero	4
.LC1017:
	ASCII(.string	"%s no correct slice in the pic\n" )
.LC1020:
	ASCII(.string	"VPS decode error.\n" )
	.zero	5
.LC1023:
	ASCII(.string	"SPS decode error.\n" )
	.zero	5
.LC1025:
	ASCII(.string	"PPS decode error.\n" )
	.zero	5
.LC1027:
	ASCII(.string	"SEI decode error.\n" )
	.zero	5
.LC1029:
	ASCII(.string	"***** NAL: UNSUPPORT, nal_unit_type=%d\n" )
.LC1032:
	ASCII(.string	"HEVC_DecSliceSegmentHeader dec err\n" )
	.zero	4
.LC1035:
	ASCII(.string	"HEVCDEC_DecodePacket NULL == pPacket!\n" )
	.zero	1
.LC1037:
	ASCII(.string	"HevcCtx not init!\n" )
	.zero	5
.LC1039:
	ASCII(.string	"HEVCDEC_DecodePacket NULL == pHevcCtx->pCurrNal!\n" )
	.zero	6
.LC1042:
	ASCII(.string	"%s last frame fs unrelease, fix it!\n" )
	.zero	3
.LC1046:
	ASCII(.string	"IsSizeChanged = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	4
.LC1049:
	ASCII(.string	"HEVC_InquireSliceProperty error.\n" )
	.zero	6
.LC1051:
	ASCII(.string	"HEVC_DecOneNal ERR\n" )
	.zero	4
.LC1056:
	ASCII(.string	"%s pstLogicFsImage is NULL!\n" )
	.zero	3
.LC1058:
	ASCII(.string	"%s InsertVO err:%d, ClearAll\n" )
	.bss
	.align	3
.LANCHOR0 = . + 0
.LANCHOR2 = . + 456960
.LANCHOR3 = . + 604928
.LANCHOR6 = . + 639744
	.type	s_auiSigLastScan, %object
	.size	s_auiSigLastScan, 458752
s_auiSigLastScan:
	.zero	458752
	.type	g_TsToRsMap, %object
	.size	g_TsToRsMap, 147456
g_TsToRsMap:
	.zero	147456
	.type	g_IsDeced, %object
	.size	g_IsDeced, 36864
g_IsDeced:
	.zero	36864
	.type	s_TmpParam, %object
	.size	s_TmpParam, 26164
s_TmpParam:
	.zero	26164
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
