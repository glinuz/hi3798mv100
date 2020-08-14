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
	.type	HEVC_SetFrmRepeatCount.part.4, %function
HEVC_SetFrmRepeatCount.part.4:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	w3, [x19, 308]
	ldr	w2, [x19, 2712]
	ldr	w0, [x19, 304]
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
	ldr	w0, [x19, 308]
	add	w0, w0, 2
	str	w0, [x19, 308]
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
	ldr	w0, [x19, 308]
	add	w0, w0, 1
	str	w0, [x19, 308]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_SetFrmRepeatCount.part.4, .-HEVC_SetFrmRepeatCount.part.4
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	HEVC_Sei_Mastering_Display_Colour_Volume.part.12, %function
HEVC_Sei_Mastering_Display_Colour_Volume.part.12:
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
	ldr	x0, [x20, 2344]
	mov	w1, 16
	bl	BsGet
	str	w0, [x19, -4]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 8]
	cmp	x19, x22
	bne	.L10
	ldr	x0, [x20, 2344]
	mov	w1, 16
	bl	BsGet
	str	w0, [x21, 28]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x21, 32]
	mov	w1, 32
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x21, 36]
	mov	w1, 32
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x21, 40]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVC_Sei_Mastering_Display_Colour_Volume.part.12, .-HEVC_Sei_Mastering_Display_Colour_Volume.part.12
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
	add	x13, x19, 356
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
	add	x8, x19, 356
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
	ldr	x4, [x22, 2888]
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
	ldr	w2, [x22, 1952]
	bl	dprint_vfmw
	ldr	w0, [x22, 1952]
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
	ldr	x3, [x20, 2400]
	bl	dprint_vfmw
	ldr	x2, [x20, 2400]
	mov	x1, x24
	mov	w0, 1
	cbz	x2, .L96
	ldr	x2, [x2, 32]
	bl	dprint_vfmw
	ldr	x0, [x20, 2400]
	ldr	x0, [x0, 32]
	cbz	x0, .L96
	ldr	w1, [x0, 32]
	ldr	w0, [x21, 276]
	bl	FSP_GetLogicFs
	mov	x2, x0
	mov	x1, x26
	mov	w0, 1
	bl	dprint_vfmw
.L96:
	mov	x1, x23
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x22, 1952]
	cmp	w0, w19
	bhi	.L98
.L99:
	ldr	x1, .LC25
	mov	w0, 1
	ldr	w2, [x22, 1956]
	bl	dprint_vfmw
	ldr	w0, [x22, 1956]
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
	ldr	x3, [x20, 2536]
	bl	dprint_vfmw
	ldr	x2, [x20, 2536]
	mov	x1, x24
	mov	w0, 1
	cbz	x2, .L101
	ldr	x2, [x2, 32]
	bl	dprint_vfmw
	ldr	x0, [x20, 2536]
	ldr	x0, [x0, 32]
	cbz	x0, .L101
	ldr	w1, [x0, 32]
	ldr	w0, [x21, 276]
	bl	FSP_GetLogicFs
	mov	x2, x0
	mov	x1, x26
	mov	w0, 1
	bl	dprint_vfmw
.L101:
	mov	x1, x23
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x22, 1956]
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
	mov	x23, 4344
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
	add	x3, x3, 3704
	ldrb	w6, [x4, 3706]
	add	x27, x21, x27
	ldrb	w5, [x4, 3705]
	ldr	w4, [x4, 3708]
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
	add	x20, x21, 3288
	mov	w19, 0
	bl	_mcount
	ldr	x1, .LC49
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w3, [x21, 3280]
	mov	w0, 1
	ldr	w2, [x21, 3284]
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
	ldr	w2, [x19, 2712]
	ldr	w3, [x19, 2716]
	ldr	w6, [x19, 2724]
	ldr	w4, [x19, 2728]
	ldr	w5, [x19, 2732]
	bl	dprint_vfmw
	ldr	w0, [x19, 2716]
	cbz	w0, .L129
	.p2align 2
.L128:
	add	x21, x19, x20, uxtw 3
	ldr	w0, [x19, 276]
	ldr	x2, [x21, 2736]
	ldrb	w7, [x2, 2]
	ldr	w1, [x2, 32]
	ldrb	w27, [x2]
	ldrb	w25, [x2, 1]
	ldr	w28, [x2, 4]
	ldr	w22, [x2, 12]
	ldr	w23, [x2, 56]
	str	w7, [x29, 108]
	bl	FSP_GetLogicFs
	ldr	x1, [x21, 2736]
	mov	x24, x0
	ldr	w0, [x19, 276]
	ldr	w1, [x1, 32]
	bl	FSP_GetRef
	mov	w26, w0
	ldr	x1, [x21, 2736]
	ldr	w0, [x19, 276]
	ldr	w1, [x1, 32]
	bl	FSP_GetDisplay
	ldr	x8, [x21, 2736]
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
	ldr	w0, [x19, 2716]
	cmp	w0, w20
	bhi	.L128
.L129:
	ldr	x24, .LC60
	mov	w0, 1
	mov	x1, x24
	bl	dprint_vfmw
	ldr	w0, [x19, 2716]
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
	ldr	w0, [x19, 2716]
	cmp	w0, w21
	bls	.L127
.L136:
	add	x22, x19, x21, uxtw 3
	ldr	w0, [x19, 276]
	ldr	x1, [x22, 2736]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	mov	x20, x0
	ldr	x1, [x22, 2736]
	ldr	w0, [x19, 276]
	ldr	w1, [x1, 32]
	bl	FSP_GetDecPhyFs
	mov	x23, x0
	ldr	x1, [x22, 2736]
	ldr	w0, [x19, 276]
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
	ldr	w0, [x19, 2716]
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
	ldr	w0, [x0, 1116]
	cbnz	w0, .L153
	ldr	x1, [x19, 2376]
	ldr	w2, [x1, 680]
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
	ldr	w2, [x1, 676]
	cbnz	w2, .L152
	ldr	w2, [x1, 684]
	cbnz	w2, .L152
	ldr	w0, [x1, 644]
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
	ldr	x0, [x19, 2344]
	mov	w1, 32
	bl	BsGet
	mov	w20, w0
	ldr	x0, [x19, 2344]
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
	.global	HEVC_IsFrameEndNal
	.type	HEVC_IsFrameEndNal, %function
HEVC_IsFrameEndNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	sub	w0, w19, #32
	sub	w19, w19, #48
	cmp	w0, 2
	ccmp	w19, 1, 0, hi
	ldr	x19, [sp, 16]
	cset	w0, ls
	ldp	x29, x30, [sp], 32
	ret
	.size	HEVC_IsFrameEndNal, .-HEVC_IsFrameEndNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_IsNeedSedOnly
	.type	HEVCDEC_IsNeedSedOnly, %function
HEVCDEC_IsNeedSedOnly:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 208]
	cbz	w0, .L163
	ldr	x1, [x19, 2384]
	cbz	x1, .L168
.L163:
	mov	w0, 0
.L162:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L168:
	add	x1, x19, 61440
	ldr	w1, [x1, 1700]
	cmp	w1, 1
	bls	.L163
	ldr	w1, [x19, 236]
	cmp	w0, w1
	beq	.L163
	ldr	w0, [x19, 220]
	cmp	w0, 1
	beq	.L169
	cbnz	w0, .L163
	ldr	w0, [x19, 276]
	bl	VCTRL_CheckFreshRaw
	cbz	w0, .L163
	ldr	w0, [x19, 276]
	bl	VCTRL_CheckFreshSeg
	cmp	w0, wzr
	cset	w0, ne
	b	.L162
	.p2align 3
.L169:
	ldr	x0, [x19, 2392]
	cmp	x0, xzr
	cset	w0, ne
	b	.L162
	.size	HEVCDEC_IsNeedSedOnly, .-HEVCDEC_IsNeedSedOnly
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
	ldr	x0, [x22, 28176]
	ldr	x1, [x22, 28168]
	ldp	w23, w26, [x0, 68]
	ldr	w25, [x1, 148]
	sub	w0, w23, #1
	cmp	w0, 9
	bhi	.L198
	sub	w0, w26, #1
	cmp	w0, 10
	bhi	.L199
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
	ble	.L179
	sub	w7, w25, #4
	mov	x2, x20
	mov	w1, 0
	mov	w6, 0
.L178:
	str	w1, [x2]
	lsl	w3, w1, w7
	ldr	x0, [x22, 28176]
	add	x0, x0, x6, sxtw 1
	ldrsh	w5, [x0, 176]
	add	w5, w5, w1
	lsl	w1, w5, w7
	sub	w0, w5, #1
	cmp	w3, w1
	str	w0, [x2, 4]
	bge	.L184
	cmp	w3, 255
	bhi	.L180
	uxtb	w4, w6
	b	.L182
	.p2align 3
.L183:
	cmp	w3, 255
	bhi	.L180
.L182:
	strb	w4, [x19, w3, sxtw]
	add	w3, w3, 1
	cmp	w3, w1
	blt	.L183
.L184:
	add	w6, w6, 1
	add	x2, x2, 8
	cmp	w23, w6
	mov	w1, w5
	bne	.L178
.L179:
	cmp	w26, wzr
	ble	.L176
	sub	w25, w25, #4
	mov	w0, 0
	mov	w5, 0
.L186:
	str	w0, [x21]
	lsl	w3, w0, w25
	ldr	x1, [x22, 28176]
	add	x1, x1, x5, sxtw 1
	ldrsh	w4, [x1, 216]
	add	w4, w4, w0
	lsl	w0, w4, w25
	sub	w1, w4, #1
	cmp	w3, w0
	str	w1, [x21, 4]
	bge	.L191
	cmp	w3, 255
	bhi	.L187
	uxtb	w2, w5
	add	x1, x19, 256
	b	.L189
	.p2align 3
.L190:
	cmp	w3, 255
	bhi	.L187
.L189:
	strb	w2, [x1, w3, sxtw]
	add	w3, w3, 1
	cmp	w3, w0
	blt	.L190
.L191:
	add	w5, w5, 1
	add	x21, x21, 8
	cmp	w26, w5
	mov	w0, w4
	bne	.L186
.L176:
	mov	w0, 0
	b	.L172
	.p2align 3
.L180:
	ldr	x2, .LC89
	mov	w0, 1
	ldr	x1, .LC96
	add	x2, x2, 136
	bl	dprint_vfmw
	mov	w0, -1
.L172:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L187:
	ldr	x2, .LC89
	mov	w0, 1
	ldr	x1, .LC98
	add	x2, x2, 136
	bl	dprint_vfmw
	mov	w0, -1
	b	.L172
.L199:
	ldr	x2, .LC89
	mov	w4, 11
	ldr	x1, .LC93
	mov	w3, w26
	add	x2, x2, 136
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L172
.L198:
	ldr	x2, .LC89
	mov	w4, 10
	ldr	x1, .LC91
	mov	w3, w23
	add	x2, x2, 136
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L172
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
	cbz	x19, .L223
	cbz	x20, .L224
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
.L204:
	ldrb	w4, [x1], 1
	ldrb	w3, [x2], 1
	cmp	w4, w3
	cset	w3, eq
	cmp	x1, x5
	and	w0, w0, w3
	bne	.L204
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
.L205:
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
	bls	.L205
	ldp	w5, w7, [x19, 28]
	ldp	w2, w1, [x20, 28]
	cmp	w2, w5
	ccmp	w1, w7, 0, eq
	cset	w3, eq
	and	w3, w3, w0
	cbz	w7, .L206
	mov	w6, 1
	.p2align 2
.L207:
	mov	w2, 0
	uxtw	x1, w6
	.p2align 2
.L208:
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
	bls	.L208
	add	w6, w6, 1
	cmp	w6, w7
	bls	.L207
.L206:
	ldrb	w1, [x20, 6]
	ldrb	w0, [x19, 6]
	cmp	w1, w0
	cset	w1, eq
	and	w3, w1, w3
	cbz	w0, .L209
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
	cbnz	w2, .L225
.L210:
	ldr	w0, [x20, 48]
	ldr	w1, [x19, 48]
	cmp	w1, w0
	cset	w0, eq
	and	w3, w0, w3
.L209:
	eor	w3, w3, 1
	neg	w0, w3
.L202:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L225:
	ldr	w0, [x20, 44]
	ldr	w1, [x19, 44]
	cmp	w1, w0
	cset	w0, eq
	and	w3, w3, w0
	b	.L210
.L224:
	ldr	x2, .LC100
	mov	w3, 1884
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L202
.L223:
	ldr	x2, .LC100
	mov	w3, 1883
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L202
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
	ldr	x1, [x19, 2344]
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
	ldr	x1, [x19, 2344]
	add	x0, x29, x2
	ldr	x3, [x3, 104]
	blr	x3
	ldp	w0, w1, [x29, 72]
	sub	w0, w0, w1
	mov	w1, 0
	cmp	w0, w20
	bcc	.L234
.L228:
	ldp	x19, x20, [sp, 16]
	mov	w0, w1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L234:
	mov	w1, 1
	add	x0, x29, 40
	bl	BsShow
	mov	w1, 1
	cbz	w0, .L228
	add	x0, x29, 40
	bl	BsSkip
	ldp	w0, w19, [x29, 72]
	sub	w19, w0, w19
	b	.L229
	.p2align 3
.L230:
	bl	BsSkip
	ldp	w0, w19, [x29, 72]
	sub	w19, w0, w19
.L229:
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
	bne	.L230
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
	beq	.L243
	ldr	x0, [x21, 2344]
	mov	w1, 8
	mov	x20, x22
	add	x23, x22, 48
	bl	BsGet
	mov	w1, w0
	stp	w0, w0, [x22, 48]
	cbnz	w1, .L237
.L246:
	mov	x19, 0
	.p2align 2
.L238:
	ldr	x0, [x21, 2344]
	mov	w1, 8
	bl	BsGet
	strb	w0, [x20, x19]
	add	x19, x19, 1
	cmp	x19, 16
	bne	.L238
.L239:
	add	x20, x20, 16
	cmp	x20, x23
	beq	.L244
.L248:
	ldr	w1, [x22, 52]
	cbz	w1, .L246
.L237:
	cmp	w1, 1
	beq	.L247
	cmp	w1, 2
	bne	.L239
	ldr	x0, [x21, 2344]
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
	bne	.L248
	.p2align 2
.L244:
	mov	w0, 0
.L236:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L247:
	ldr	x0, [x21, 2344]
	mov	w1, 16
	bl	BsGet
	lsr	w1, w0, 8
	strb	w0, [x20, 1]
	strb	w1, [x20]
	b	.L239
.L243:
	mov	w0, -1
	b	.L236
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
	beq	.L251
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19]
	mov	w1, 0
.L250:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L251:
	mov	w1, -1
	b	.L250
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
	beq	.L254
	mov	w0, 1
	mov	x1, x19
	strb	w0, [x19]
	mov	x0, x20
	bl	HEVC_Sei_Mastering_Display_Colour_Volume.part.12
.L253:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L254:
	mov	w0, -1
	b	.L253
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
	beq	.L257
	mov	w0, 1
	strb	w0, [x19]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	strh	w0, [x19, 2]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	strh	w0, [x19, 4]
	mov	w1, 0
.L256:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L257:
	mov	w1, -1
	b	.L256
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
	beq	.L260
	ldr	x0, [x20, 2344]
	mov	w1, 8
	bl	BsGet
	strb	w0, [x19]
	mov	w1, 0
.L259:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L260:
	mov	w1, -1
	b	.L259
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
	beq	.L263
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x20]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x20, 4]
	ldr	x0, [x19, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L262:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L263:
	mov	w0, -1
	b	.L262
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
	beq	.L267
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 2]
	cbz	w0, .L268
.L266:
	ldr	x0, [x19, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L265:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L268:
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 1]
	mov	w1, 16
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 3]
	b	.L266
	.p2align 3
.L267:
	mov	w0, -1
	b	.L265
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
	add	x0, x0, 3504
	ldr	x3, [x3, 96]
	blr	x3
	str	wzr, [x19, 5260]
	str	wzr, [x19, 5264]
	str	wzr, [x19, 5268]
	str	wzr, [x19, 5272]
	str	xzr, [x19, 7360]
	str	xzr, [x19, 7368]
	str	xzr, [x19, 7376]
	str	xzr, [x19, 7384]
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
	ldr	x2, [x19, 2368]
	add	x5, x3, 3504
	mov	w7, 2024
	ldr	w6, [x21, 12]
	ldr	w4, [x0, 1868]
	mov	w1, 26164
	ldr	x0, [x19, 2360]
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
	beq	.L278
	ldrb	w0, [x5, 73]
	add	x19, x19, 4096
	cbnz	w0, .L279
	ldr	w0, [x19, 1196]
	ldr	x1, [x19, 3264]
	add	x0, x0, x1
	str	x0, [x19, 3288]
.L272:
	add	x19, x19, 1184
	add	x3, x3, 3504
	str	x0, [x19, 2096]
	strb	wzr, [x3, 73]
	ldrb	w1, [x20, 9]
	cbnz	w1, .L280
.L276:
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
.L279:
	ldrb	w0, [x19, 121]
	cbnz	w0, .L281
	ldr	x1, [x19, 3264]
	ldr	x6, [x19, 3272]
.L275:
	ldr	w7, [x19, 1196]
	add	x0, x7, x1
	str	x0, [x19, 3264]
	cmp	x0, x6
	str	x0, [x19, 3288]
	bge	.L272
	add	x0, x7, x6
	add	x19, x19, 1184
	str	x0, [x19, 2080]
	add	x3, x3, 3504
	str	x0, [x19, 2104]
	str	x0, [x19, 2096]
	strb	wzr, [x3, 73]
	ldrb	w1, [x20, 9]
	cbz	w1, .L276
.L280:
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
.L278:
	add	x19, x19, 4096
	ldrsw	x0, [x19, 1164]
	str	x0, [x19, 3264]
	str	x0, [x19, 3288]
	b	.L272
	.p2align 3
.L281:
	ldr	x6, [x19, 3272]
	mov	x1, x6
	b	.L275
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
	bls	.L289
.L283:
	sub	w20, w20, #6
	cmp	w20, 3
	bls	.L282
	ldr	x0, [x19, 7384]
	str	x0, [x19, 7368]
.L282:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L289:
	ldr	x0, [x19, 2376]
	ldr	x0, [x0, 56]
	cmn	x0, #1
	beq	.L284
	ldr	x1, [x19, 28120]
	str	x0, [x19, 28136]
	str	x1, [x19, 28144]
.L284:
	add	x0, x19, 24576
	mov	w1, 1
	strb	w1, [x0, 3576]
	b	.L283
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
	ldr	x3, [x19, 28120]
	ldr	x6, [x19, 28136]
	ldr	x7, [x19, 28144]
	ldr	w4, [x1, 3552]
	ldr	w5, [x1, 3556]
	cbz	w4, .L293
	mov	w1, w4
	mov	w2, w5
	b	.L292
	.p2align 3
.L294:
	mov	w1, w0
.L292:
	udiv	w0, w2, w1
	msub	w0, w0, w1, w2
	mov	w2, w1
	cbnz	w0, .L294
.L291:
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
	str	x1, [x19, 1768]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L293:
	mov	w1, w5
	b	.L291
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
	bls	.L314
	add	x19, x22, 8
	add	x21, x21, 225
	sub	w20, w26, #1
	add	x25, x22, 24
	mov	w23, 241
	b	.L299
	.p2align 3
.L321:
	ldrb	w23, [x21], 1
.L299:
	ldr	x0, [x24, 2344]
	mov	w1, 8
	add	x19, x19, 1
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, w23
	strb	w0, [x19, -1]
	beq	.L297
	strb	wzr, [x22, 28]
	str	w20, [x22, 24]
.L297:
	cmp	x19, x25
	sub	w20, w20, #1
	bne	.L321
	ldrb	w19, [x22, 28]
	cmp	w19, 1
	beq	.L322
	sub	w26, w26, #16
	str	w26, [x22, 24]
	cbz	w26, .L311
	mov	w19, 0
	.p2align 2
.L312:
	ldr	x0, [x24, 2344]
	mov	w1, 8
	add	w19, w19, 1
	bl	BsGet
	ldr	w0, [x22, 24]
	cmp	w0, w19
	bhi	.L312
.L313:
	mov	w0, 0
.L296:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L311:
	str	xzr, [x22]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 144
	ret
.L322:
	ldr	x0, [x24, 2344]
	mov	w1, 5
	bl	BsGet
	uxtb	w20, w0
	ldr	x0, [x24, 2344]
	mov	w1, 3
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 3
	ccmp	w20, 6, 0, eq
	beq	.L323
	cmp	w20, 2
	cset	w1, eq
	cmp	w0, wzr
	cset	w2, eq
	tst	w1, w2
	bne	.L324
	cmp	w0, 1
	cset	w0, eq
	tst	w1, w0
	bne	.L325
	sub	w26, w26, #18
	str	w26, [x22, 24]
.L304:
	mov	w19, 0
	cbz	w26, .L311
	.p2align 2
.L317:
	ldr	x0, [x24, 2344]
	mov	w1, 8
	add	w19, w19, 1
	bl	BsGet
	ldr	w0, [x22, 24]
	cmp	w0, w19
	bhi	.L317
	b	.L313
.L325:
	ldr	x0, [x24, 2344]
	mov	w1, 8
	sub	w26, w26, #19
	bl	BsGet
	mov	w19, w0
	ldr	x0, [x24, 2344]
	mov	w1, 8
	bl	BsGet
	ubfiz	w0, w0, 8, 8
	add	w19, w0, w19, uxtb
	str	w26, [x22, 24]
	str	w19, [x22, 68]
	b	.L304
.L314:
	mov	w0, -1
	b	.L296
.L323:
	ldr	x0, [x24, 2344]
	mov	w1, 2
	bl	BsGet
	strb	w0, [x22, 35]
	mov	w1, 2
	ldr	x0, [x24, 2344]
	bl	BsGet
	strb	w0, [x22, 34]
	mov	w1, w19
	ldr	x0, [x24, 2344]
	bl	BsGet
	strb	w0, [x22, 33]
	mov	w1, w19
	ldr	x0, [x24, 2344]
	bl	BsGet
	strb	w0, [x22, 32]
	mov	w1, 2
	ldr	x0, [x24, 2344]
	bl	BsGet
	ldrb	w0, [x22, 33]
	mov	x2, 28
	ldr	x3, .LC108
	cbnz	w0, .L302
	ldr	x3, [x3, 104]
	add	x1, x29, 80
	add	x0, x22, 40
	blr	x3
.L303:
	ldr	x0, [x24, 2344]
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
	ldr	x0, [x24, 2344]
	bl	BsGet
	mov	w19, w0
	ldr	x0, [x24, 2344]
	mov	w1, 8
	bl	BsGet
	ubfiz	w0, w0, 8, 8
	add	w19, w0, w19, uxtb
	str	w26, [x22, 24]
	str	w19, [x22, 68]
	b	.L304
.L324:
	ldr	x0, [x24, 2344]
	mov	w1, 2
	bl	BsGet
	strb	w0, [x22, 35]
	mov	w1, 2
	ldr	x0, [x24, 2344]
	bl	BsGet
	strb	w0, [x22, 34]
	mov	w1, w19
	ldr	x0, [x24, 2344]
	bl	BsGet
	strb	w0, [x22, 33]
	mov	w1, w19
	ldr	x0, [x24, 2344]
	bl	BsGet
	strb	w0, [x22, 32]
	mov	w1, 2
	ldr	x0, [x24, 2344]
	bl	BsGet
	ldrb	w0, [x22, 33]
	mov	x2, 28
	ldr	x3, .LC108
	cbnz	w0, .L306
	ldr	x3, [x3, 104]
	add	x1, x29, 80
	add	x0, x22, 40
	blr	x3
.L307:
	ldr	x0, [x24, 2344]
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
	b	.L304
.L302:
	ldr	x3, [x3, 104]
	add	x1, x29, 112
	add	x0, x22, 40
	blr	x3
	b	.L303
.L306:
	ldr	x3, [x3, 104]
	add	x1, x29, 112
	add	x0, x22, 40
	blr	x3
	b	.L307
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
	ldr	w3, [x21, 1952]
	cbnz	w3, .L367
.L327:
	mov	w0, 0
.L365:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L367:
	ldr	x0, [x20, 2400]
	cbz	x0, .L349
	ldr	x0, [x0, 32]
	mov	w19, 0
	cbnz	x0, .L333
	b	.L329
	.p2align 3
.L369:
	ldr	x1, [x0, 640]
	cbz	x1, .L334
	ldr	w3, [x21, 1952]
	add	w19, w19, 1
	cmp	w3, w19
	add	x0, x20, x19, sxtw 3
	bls	.L368
	ldr	x0, [x0, 2400]
	cbz	x0, .L328
	ldr	x0, [x0, 32]
	cbz	x0, .L329
.L333:
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_GetLogicFs
	cbnz	x0, .L369
.L334:
	add	x20, x20, x19, sxtw 3
	ldr	x1, .LC116
	mov	x4, x0
	mov	w2, w19
	mov	w0, 1
	ldr	x3, [x20, 2400]
	ldr	x3, [x3, 32]
	bl	dprint_vfmw
	mov	w0, -1
.L373:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L368:
	cbz	w3, .L327
	ldr	w0, [x21, 1956]
	cbz	w0, .L327
	ldr	x0, [x20, 2536]
	cbz	x0, .L351
	ldr	x0, [x0, 32]
	mov	w19, 0
	cbnz	x0, .L344
	b	.L341
	.p2align 3
.L372:
	ldr	x1, [x0, 640]
	cbz	x1, .L345
	ldr	w0, [x21, 1956]
	add	w19, w19, 1
	cmp	w0, w19
	add	x1, x20, x19, sxtw 3
	bls	.L327
	ldr	x0, [x1, 2536]
	cbz	x0, .L370
	ldr	x0, [x0, 32]
	cbz	x0, .L371
.L344:
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_GetLogicFs
	cbnz	x0, .L372
.L345:
	ldr	x1, .LC122
	mov	x3, x0
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L365
.L349:
	mov	w19, 0
	.p2align 2
.L328:
	ldr	x1, .LC112
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L373
	.p2align 3
.L329:
	ldr	x1, .LC114
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L373
.L371:
	ldr	w3, [x21, 1952]
.L341:
	ldr	x1, .LC120
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L365
.L370:
	ldr	w3, [x21, 1952]
.L340:
	ldr	x1, .LC118
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L365
.L351:
	mov	w19, 0
	b	.L340
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
	ldr	w3, [x19, 2716]
	cmp	w3, 16
	bhi	.L375
	mov	w4, 2147483647
	mov	w0, 0
	cbz	w3, .L380
	.p2align 2
.L383:
	add	x2, x19, x0, uxtw 3
	ldr	x2, [x2, 2736]
	ldr	w5, [x2, 12]
	cmp	w5, w4
	bge	.L378
	ldr	w2, [x2]
	tst	w2, 16776960
	bne	.L378
	str	w0, [x20]
	mov	w4, w5
	ldr	w3, [x19, 2716]
.L378:
	add	w0, w0, 1
	cmp	w3, w0
	bhi	.L383
	ldr	w3, [x20]
	cmn	w3, #1
	beq	.L380
	ldr	x2, .LC123
	mov	w0, 1
	ldr	x1, .LC129
	add	x2, x2, 240
	bl	dprint_vfmw
	mov	w0, 0
.L377:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L380:
	ldr	x2, .LC123
	mov	w0, 1
	ldr	x1, .LC125
	add	x2, x2, 240
	bl	dprint_vfmw
	mov	w0, -1
	b	.L377
.L375:
	ldr	x2, .LC123
	mov	w0, 1
	ldr	x1, .LC127
	add	x2, x2, 240
	bl	dprint_vfmw
	mov	w0, w21
	b	.L377
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
	ldr	w3, [x19, 2712]
	cmp	w3, 16
	bhi	.L393
	ldr	w0, [x19, 276]
	bl	FSP_GetTotalValidFsNum
	add	x1, x19, 53248
	mov	w3, 1
	ldr	w2, [x1, 1120]
	cmp	w2, 1920
	bls	.L394
.L389:
	subs	w0, w0, w3
	ldr	x2, .LC130
	csel	w0, w0, wzr, pl
	cmp	w0, wzr
	ble	.L390
	ldr	w3, [x19, 2712]
	add	w3, w3, 1
	cmp	w3, w0
	bhi	.L390
	ldr	x1, .LC134
	add	x2, x2, 272
	mov	w0, 13
	str	w3, [x19, 2712]
	bl	dprint_vfmw
	mov	w0, 0
.L388:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L394:
	ldr	w1, [x1, 1124]
	mov	w3, 4
	cmp	w1, 1088
	csinc	w3, w3, wzr, ls
	b	.L389
	.p2align 3
.L390:
	ldr	x1, .LC136
	mov	w3, w0
	add	x2, x2, 272
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L388
	.p2align 3
.L393:
	ldr	x2, .LC130
	mov	w4, 17
	ldr	x1, .LC132
	mov	w0, 1
	add	x2, x2, 272
	bl	dprint_vfmw
	mov	w1, 17
	mov	w0, -1
	str	w1, [x19, 2712]
	b	.L388
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
	ldr	w1, [x0, 1864]
	sub	w2, w1, #16
	cmp	w2, 5
	bhi	.L396
	str	w1, [x0, 1108]
	str	wzr, [x0, 1116]
.L395:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L396:
	ldr	w1, [x0, 1108]
	sub	w1, w1, #16
	cmp	w1, 5
	bls	.L395
	ldr	w1, [x0, 1852]
	cmp	w1, 1
	beq	.L398
	cmp	w1, 2
	beq	.L399
	cbnz	w1, .L395
	mov	w1, 2
	str	w1, [x0, 1116]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L398:
	ldr	w2, [x0, 1116]
	cmp	w2, 2
	beq	.L395
	str	w1, [x0, 1116]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L399:
	ldr	w1, [x0, 1116]
	sub	w1, w1, #1
	cmp	w1, 1
	bls	.L395
	str	wzr, [x0, 1116]
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
	ldr	w1, [x19, 1864]
	cmp	w1, 15
	bhi	.L405
	mov	w0, 1
	tbnz	x1, 0, .L406
.L405:
	sub	w1, w1, #16
	cmp	w1, 7
	cset	w0, ls
.L406:
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
	ldrb	w4, [x1, 616]
	ldr	w3, [x1, 624]
	ldr	w2, [x1, 620]
	ldrb	w0, [x1, 612]
	ubfiz	w2, w2, 8, 8
	orr	w2, w2, w4, lsl 16
	orr	w0, w0, w3, lsl 24
	orr	w0, w2, w0
	str	w0, [x19]
	ldrb	w4, [x1, 625]
	ldrb	w2, [x1, 617]
	ldr	w3, [x1, 620]
	ldrb	w0, [x1, 613]
	lsl	w2, w2, 16
	and	w3, w3, 65280
	orr	w2, w2, w4, lsl 24
	orr	w0, w0, w3
	orr	w0, w2, w0
	str	w0, [x19, 4]
	ldrb	w4, [x1, 626]
	ldrb	w2, [x1, 622]
	ldr	w3, [x1, 616]
	ldrb	w0, [x1, 614]
	lsl	w2, w2, 8
	and	w3, w3, 16711680
	orr	w2, w2, w4, lsl 24
	orr	w0, w0, w3
	orr	w0, w2, w0
	str	w0, [x19, 8]
	ldrb	w3, [x1, 619]
	ldrb	w2, [x1, 623]
	ldr	w0, [x1, 624]
	ldrb	w1, [x1, 615]
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
	bl	_mcount
	uxtw	x0, w20
	add	x1, x0, 18
	add	x0, x21, x0, lsl 6
	add	x1, x21, x1, lsl 6
	ldrb	w5, [x0, 1780]
	ldr	w3, [x0, 1788]
	ldr	w4, [x0, 1772]
	ldrb	w2, [x1, 612]
	ubfiz	w4, w4, 8, 8
	orr	w4, w4, w5, lsl 16
	orr	w2, w2, w3, lsl 24
	orr	w2, w4, w2
	str	w2, [x19]
	ldrb	w5, [x0, 1812]
	ldr	w3, [x0, 1820]
	ldr	w4, [x0, 1804]
	ldrb	w2, [x0, 1796]
	ubfiz	w4, w4, 8, 8
	orr	w4, w4, w5, lsl 16
	orr	w2, w2, w3, lsl 24
	orr	w2, w4, w2
	str	w2, [x19, 4]
	ldrb	w5, [x0, 1789]
	ldrb	w4, [x0, 1781]
	ldr	w3, [x0, 1772]
	ldrb	w2, [x1, 613]
	lsl	w4, w4, 16
	and	w3, w3, 65280
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 8]
	ldrb	w5, [x0, 1821]
	ldrb	w4, [x0, 1813]
	ldr	w3, [x0, 1804]
	ldrb	w2, [x0, 1797]
	lsl	w4, w4, 16
	and	w3, w3, 65280
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 12]
	ldrb	w5, [x0, 1790]
	ldrb	w4, [x0, 1774]
	ldr	w3, [x0, 1780]
	ldrb	w2, [x1, 614]
	lsl	w4, w4, 8
	and	w3, w3, 16711680
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 16]
	ldrb	w5, [x0, 1822]
	ldrb	w4, [x0, 1806]
	ldr	w3, [x0, 1812]
	ldrb	w2, [x0, 1798]
	lsl	w4, w4, 8
	and	w3, w3, 16711680
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 20]
	ldrb	w4, [x0, 1783]
	ldrb	w3, [x0, 1775]
	ldr	w5, [x0, 1788]
	ldrb	w2, [x1, 615]
	lsl	w3, w3, 8
	and	w1, w5, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 24]
	ldrb	w4, [x0, 1815]
	ldrb	w3, [x0, 1807]
	ldr	w1, [x0, 1820]
	ldrb	w2, [x0, 1799]
	lsl	w3, w3, 8
	and	w1, w1, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 28]
	ldrb	w4, [x0, 1784]
	ldr	w2, [x0, 1792]
	ldr	w3, [x0, 1776]
	ldrb	w1, [x0, 1768]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2, lsl 24
	orr	w1, w3, w1
	str	w1, [x19, 32]
	ldrb	w4, [x0, 1816]
	ldr	w2, [x0, 1824]
	ldr	w3, [x0, 1808]
	ldrb	w1, [x0, 1800]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2, lsl 24
	orr	w1, w3, w1
	str	w1, [x19, 36]
	ldrb	w4, [x0, 1793]
	ldrb	w3, [x0, 1785]
	ldr	w2, [x0, 1776]
	ldrb	w1, [x0, 1769]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 40]
	ldrb	w4, [x0, 1825]
	ldrb	w3, [x0, 1817]
	ldr	w2, [x0, 1808]
	ldrb	w1, [x0, 1801]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 44]
	ldrb	w4, [x0, 1794]
	ldrb	w3, [x0, 1778]
	ldr	w2, [x0, 1784]
	ldrb	w1, [x0, 1770]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 48]
	ldrb	w4, [x0, 1826]
	ldrb	w3, [x0, 1810]
	ldr	w2, [x0, 1816]
	ldrb	w1, [x0, 1802]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 52]
	ldrb	w4, [x0, 1787]
	ldrb	w3, [x0, 1779]
	ldr	w1, [x0, 1792]
	ldrb	w2, [x0, 1771]
	lsl	w3, w3, 8
	and	w1, w1, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 56]
	ldrb	w3, [x0, 1819]
	ldrb	w2, [x0, 1811]
	ldr	w4, [x0, 1824]
	ldrb	w1, [x0, 1803]
	lsl	w2, w2, 8
	and	w0, w4, -16777216
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
	bl	_mcount
	uxtw	x0, w20
	add	x1, x0, 6
	add	x0, x21, x0, lsl 6
	add	x1, x21, x1, lsl 6
	ldrb	w5, [x0, 1028]
	ldr	w3, [x0, 1044]
	ldr	w4, [x0, 1012]
	ldrb	w2, [x1, 612]
	ubfiz	w4, w4, 8, 8
	orr	w4, w4, w5, lsl 16
	orr	w2, w2, w3, lsl 24
	orr	w2, w4, w2
	str	w2, [x19]
	ldrb	w5, [x0, 1036]
	ldr	w3, [x0, 1052]
	ldr	w4, [x0, 1020]
	ldrb	w2, [x0, 1004]
	ubfiz	w4, w4, 8, 8
	orr	w4, w4, w5, lsl 16
	orr	w2, w2, w3, lsl 24
	orr	w2, w4, w2
	str	w2, [x19, 4]
	ldrb	w5, [x0, 1045]
	ldrb	w4, [x0, 1029]
	ldr	w3, [x0, 1012]
	ldrb	w2, [x1, 613]
	lsl	w4, w4, 16
	and	w3, w3, 65280
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 8]
	ldrb	w5, [x0, 1053]
	ldrb	w4, [x0, 1037]
	ldr	w3, [x0, 1020]
	ldrb	w2, [x0, 1005]
	lsl	w4, w4, 16
	and	w3, w3, 65280
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 12]
	ldrb	w5, [x0, 1046]
	ldrb	w4, [x0, 1014]
	ldr	w3, [x0, 1028]
	ldrb	w2, [x1, 614]
	lsl	w4, w4, 8
	and	w3, w3, 16711680
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 16]
	ldrb	w5, [x0, 1054]
	ldrb	w4, [x0, 1022]
	ldr	w3, [x0, 1036]
	ldrb	w2, [x0, 1006]
	lsl	w4, w4, 8
	and	w3, w3, 16711680
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 20]
	ldrb	w4, [x0, 1031]
	ldrb	w3, [x0, 1015]
	ldr	w5, [x0, 1044]
	ldrb	w2, [x1, 615]
	lsl	w3, w3, 8
	and	w1, w5, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 24]
	ldrb	w4, [x0, 1039]
	ldrb	w3, [x0, 1023]
	ldr	w1, [x0, 1052]
	ldrb	w2, [x0, 1007]
	lsl	w3, w3, 8
	and	w1, w1, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 28]
	ldrb	w4, [x0, 1032]
	ldr	w2, [x0, 1048]
	ldr	w3, [x0, 1016]
	ldrb	w1, [x0, 1000]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2, lsl 24
	orr	w1, w3, w1
	str	w1, [x19, 32]
	ldrb	w4, [x0, 1040]
	ldr	w2, [x0, 1056]
	ldr	w3, [x0, 1024]
	ldrb	w1, [x0, 1008]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2, lsl 24
	orr	w1, w3, w1
	str	w1, [x19, 36]
	ldrb	w4, [x0, 1049]
	ldrb	w3, [x0, 1033]
	ldr	w2, [x0, 1016]
	ldrb	w1, [x0, 1001]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 40]
	ldrb	w4, [x0, 1057]
	ldrb	w3, [x0, 1041]
	ldr	w2, [x0, 1024]
	ldrb	w1, [x0, 1009]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 44]
	ldrb	w4, [x0, 1050]
	ldrb	w3, [x0, 1018]
	ldr	w2, [x0, 1032]
	ldrb	w1, [x0, 1002]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 48]
	ldrb	w4, [x0, 1058]
	ldrb	w3, [x0, 1026]
	ldr	w2, [x0, 1040]
	ldrb	w1, [x0, 1010]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 52]
	ldrb	w4, [x0, 1035]
	ldrb	w3, [x0, 1019]
	ldr	w1, [x0, 1048]
	ldrb	w2, [x0, 1003]
	lsl	w3, w3, 8
	and	w1, w1, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 56]
	ldrb	w3, [x0, 1043]
	ldrb	w2, [x0, 1027]
	ldr	w4, [x0, 1056]
	ldrb	w1, [x0, 1011]
	lsl	w2, w2, 8
	and	w0, w4, -16777216
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
	bl	_mcount
	uxtw	x0, w20
	add	x1, x0, 12
	add	x0, x21, x0, lsl 6
	add	x1, x21, x1, lsl 6
	ldrb	w5, [x0, 1396]
	ldr	w3, [x0, 1404]
	ldr	w4, [x0, 1388]
	ldrb	w2, [x1, 612]
	ubfiz	w4, w4, 8, 8
	orr	w4, w4, w5, lsl 16
	orr	w2, w2, w3, lsl 24
	orr	w2, w4, w2
	str	w2, [x19]
	ldrb	w5, [x0, 1428]
	ldr	w3, [x0, 1436]
	ldr	w4, [x0, 1420]
	ldrb	w2, [x0, 1412]
	ubfiz	w4, w4, 8, 8
	orr	w4, w4, w5, lsl 16
	orr	w2, w2, w3, lsl 24
	orr	w2, w4, w2
	str	w2, [x19, 4]
	ldrb	w5, [x0, 1405]
	ldrb	w4, [x0, 1397]
	ldr	w3, [x0, 1388]
	ldrb	w2, [x1, 613]
	lsl	w4, w4, 16
	and	w3, w3, 65280
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 8]
	ldrb	w5, [x0, 1437]
	ldrb	w4, [x0, 1429]
	ldr	w3, [x0, 1420]
	ldrb	w2, [x0, 1413]
	lsl	w4, w4, 16
	and	w3, w3, 65280
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 12]
	ldrb	w5, [x0, 1406]
	ldrb	w4, [x0, 1390]
	ldr	w3, [x0, 1396]
	ldrb	w2, [x1, 614]
	lsl	w4, w4, 8
	and	w3, w3, 16711680
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 16]
	ldrb	w5, [x0, 1438]
	ldrb	w4, [x0, 1422]
	ldr	w3, [x0, 1428]
	ldrb	w2, [x0, 1414]
	lsl	w4, w4, 8
	and	w3, w3, 16711680
	orr	w4, w4, w5, lsl 24
	orr	w2, w2, w3
	orr	w2, w4, w2
	str	w2, [x19, 20]
	ldrb	w4, [x0, 1399]
	ldrb	w3, [x0, 1391]
	ldr	w5, [x0, 1404]
	ldrb	w2, [x1, 615]
	lsl	w3, w3, 8
	and	w1, w5, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 24]
	ldrb	w4, [x0, 1431]
	ldrb	w3, [x0, 1423]
	ldr	w1, [x0, 1436]
	ldrb	w2, [x0, 1415]
	lsl	w3, w3, 8
	and	w1, w1, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 28]
	ldrb	w4, [x0, 1400]
	ldr	w2, [x0, 1408]
	ldr	w3, [x0, 1392]
	ldrb	w1, [x0, 1384]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2, lsl 24
	orr	w1, w3, w1
	str	w1, [x19, 32]
	ldrb	w4, [x0, 1432]
	ldr	w2, [x0, 1440]
	ldr	w3, [x0, 1424]
	ldrb	w1, [x0, 1416]
	ubfiz	w3, w3, 8, 8
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2, lsl 24
	orr	w1, w3, w1
	str	w1, [x19, 36]
	ldrb	w4, [x0, 1409]
	ldrb	w3, [x0, 1401]
	ldr	w2, [x0, 1392]
	ldrb	w1, [x0, 1385]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 40]
	ldrb	w4, [x0, 1441]
	ldrb	w3, [x0, 1433]
	ldr	w2, [x0, 1424]
	ldrb	w1, [x0, 1417]
	lsl	w3, w3, 16
	and	w2, w2, 65280
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 44]
	ldrb	w4, [x0, 1410]
	ldrb	w3, [x0, 1394]
	ldr	w2, [x0, 1400]
	ldrb	w1, [x0, 1386]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 48]
	ldrb	w4, [x0, 1442]
	ldrb	w3, [x0, 1426]
	ldr	w2, [x0, 1432]
	ldrb	w1, [x0, 1418]
	lsl	w3, w3, 8
	and	w2, w2, 16711680
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 52]
	ldrb	w4, [x0, 1403]
	ldrb	w3, [x0, 1395]
	ldr	w1, [x0, 1408]
	ldrb	w2, [x0, 1387]
	lsl	w3, w3, 8
	and	w1, w1, -16777216
	orr	w3, w3, w4, lsl 16
	orr	w1, w1, w2
	orr	w1, w3, w1
	str	w1, [x19, 56]
	ldrb	w3, [x0, 1435]
	ldrb	w2, [x0, 1427]
	ldr	w4, [x0, 1440]
	ldrb	w1, [x0, 1419]
	lsl	w2, w2, 8
	and	w0, w4, -16777216
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
	ldr	x3, [x20, 28176]
	ldrb	w0, [x3, 13]
	cbz	w0, .L446
	ldp	w6, w11, [x3, 68]
	ldr	x0, [x20, 28168]
	mov	w1, 0
	subs	w8, w11, #1
	ldrb	w7, [x3, 15]
	sub	w12, w6, #1
	ldr	w4, [x0, 72]
	ldr	w5, [x0, 148]
	bmi	.L418
	.p2align 2
.L449:
	sxtw	x0, w1
	add	x9, x29, 224
	add	w1, w1, 1
	add	x2, x3, x0, lsl 1
	add	x0, x9, x0, lsl 2
	cmp	w11, w1
	ldrsh	w2, [x2, 216]
	sub	w2, w2, #1
	str	w2, [x0, -172]
	bne	.L449
.L418:
	mov	w1, 0
	tbnz	w12, #31, .L416
	.p2align 2
.L448:
	sxtw	x0, w1
	add	x9, x29, 224
	add	x2, x0, 88
	add	w1, w1, 1
	add	x0, x9, x0, lsl 2
	cmp	w6, w1
	ldrsh	w2, [x3, x2, lsl 1]
	sub	w2, w2, #1
	str	w2, [x0, -84]
	bne	.L448
.L416:
	mov	w1, 1
	lsl	w0, w1, w5
	sdiv	w5, w4, w0
	msub	w0, w5, w0, w4
	cmp	w0, wzr
	csinc	w5, w5, w5, eq
	sdiv	w9, w19, w5
	msub	w5, w9, w5, w19
	cbz	w7, .L461
	tbnz	w8, #31, .L458
	mov	w14, 0
	mov	w13, 0
	mov	w4, 0
	mov	w7, 0
	mov	w8, 0
	mov	w0, 0
	.p2align 2
.L430:
	tbnz	w12, #31, .L441
	add	x0, x29, 224
	mov	w7, w4
	add	x0, x0, x13, sxtw 2
	mov	w1, 0
	mov	w2, 0
	ldr	w8, [x0, -172]
	add	w8, w8, 1
	add	w10, w14, w8
	b	.L427
	.p2align 3
.L439:
	mov	w7, w4
.L427:
	add	x0, x29, 224
	add	x0, x0, x2, sxtw 2
	add	w2, w2, 1
	ldr	w0, [x0, -84]
	add	w0, w0, 1
	add	w3, w0, w1
	cmp	w5, w3
	madd	w4, w0, w8, w4
	bge	.L425
	cmp	w9, w10
	blt	.L433
.L425:
	cmp	w6, w2
	mov	w1, w3
	bne	.L439
.L431:
	add	w1, w0, w3
	cmp	w5, w1
	bge	.L440
	cmp	w9, w10
	blt	.L456
.L440:
	add	w13, w13, 1
	mov	w14, w10
	cmp	w11, w13
	bne	.L430
.L457:
	sub	w9, w9, w10
	mov	w1, w3
	mul	w0, w9, w0
	.p2align 2
.L423:
	sub	w19, w5, w1
	add	w0, w19, w0
	add	w19, w0, w7
.L446:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 224
	ret
.L461:
	tbnz	w8, #31, .L458
	mov	w14, 0
	mov	w13, 0
	mov	w4, 0
	mov	w7, 0
	mov	w8, 0
	mov	w0, 0
	tbnz	w12, #31, .L444
	.p2align 2
.L462:
	add	x0, x29, 224
	mov	w7, w4
	add	x3, x0, x13, sxtw 2
	mov	w1, 0
	mov	w2, 0
	ldr	w8, [x3, -172]
	add	w8, w8, 1
	add	w10, w14, w8
	b	.L434
	.p2align 3
.L442:
	mov	w7, w4
.L434:
	add	x0, x29, 224
	add	x0, x0, x2, sxtw 2
	add	w2, w2, 1
	ldr	w0, [x0, -84]
	add	w0, w0, 1
	add	w3, w0, w1
	cmp	w5, w3
	madd	w4, w0, w8, w4
	bge	.L432
	cmp	w9, w10
	blt	.L433
.L432:
	cmp	w6, w2
	mov	w1, w3
	bne	.L442
.L438:
	add	w1, w0, w3
	cmp	w5, w1
	bge	.L443
	cmp	w9, w10
	blt	.L456
.L443:
	add	w13, w13, 1
	mov	w14, w10
	cmp	w11, w13
	beq	.L457
	tbz	w12, #31, .L462
.L444:
	mov	w3, 0
	add	w10, w14, w8
	b	.L438
	.p2align 3
.L433:
	sub	w9, w9, w14
	mul	w0, w9, w0
	b	.L423
.L441:
	mov	w3, 0
	add	w10, w14, w8
	b	.L431
.L458:
	mov	w0, 0
	mov	w1, 0
	mov	w7, 0
	b	.L423
.L456:
	sub	w9, w9, w14
	mov	w1, w3
	mul	w0, w9, w0
	b	.L423
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
	ldr	x3, [x20, 28176]
	ldrb	w0, [x3, 13]
	cbz	w0, .L481
	ldp	w8, w4, [x3, 68]
	ldr	x0, [x20, 28168]
	subs	w13, w4, #1
	sub	w12, w8, #1
	ldr	w5, [x0, 72]
	ldr	w6, [x0, 148]
	bmi	.L465
	mov	w1, 0
	.p2align 2
.L466:
	sxtw	x0, w1
	add	x7, x29, 224
	add	w1, w1, 1
	add	x2, x3, x0, lsl 1
	add	x0, x7, x0, lsl 2
	cmp	w4, w1
	ldrsh	w2, [x2, 216]
	sub	w2, w2, #1
	str	w2, [x0, -172]
	bne	.L466
	tbnz	w12, #31, .L498
.L468:
	mov	w1, 0
	.p2align 2
.L471:
	sxtw	x0, w1
	add	x4, x29, 224
	add	x2, x0, 88
	add	w1, w1, 1
	add	x0, x4, x0, lsl 2
	cmp	w12, w1
	ldrsh	w2, [x3, x2, lsl 1]
	sub	w2, w2, #1
	str	w2, [x0, -84]
	bge	.L471
	mov	w1, 1
	lsl	w0, w1, w6
	sdiv	w1, w5, w0
	msub	w0, w1, w0, w5
	cmp	w0, wzr
	csinc	w0, w1, w1, eq
	tbnz	w13, #31, .L469
.L470:
	ldr	w11, [x29, 140]
	mov	w10, 0
	mov	w9, 0
	mov	w4, 0
	mov	w1, 0
	mov	w7, 0
	mov	w2, 0
	add	w11, w11, 1
	.p2align 2
.L477:
	tbnz	w12, #31, .L483
	add	x1, x29, 224
	add	x1, x1, x9, sxtw 2
	ldr	w7, [x1, -172]
	add	w7, w7, 1
	madd	w5, w11, w7, w4
	cmp	w19, w5
	blt	.L484
	mov	w1, w4
	mov	w2, w11
	mov	w6, 0
	mov	w3, 0
	mov	w4, w5
	b	.L474
	.p2align 3
.L475:
	ldr	w2, [x5, -84]
	mov	w1, w4
	add	w2, w2, 1
	madd	w5, w2, w7, w4
	cmp	w5, w19
	bgt	.L476
	mov	w4, w5
.L474:
	add	w3, w3, 1
	add	x5, x29, 224
	cmp	w8, w3
	add	w6, w6, w2
	add	x5, x5, x3, sxtw 2
	bne	.L475
.L478:
	cmp	w4, w19
	bgt	.L482
	add	w9, w9, 1
	add	w10, w10, w7
	cmp	w13, w9
	bge	.L477
.L482:
	mov	w4, w1
	.p2align 2
.L476:
	cbz	w2, .L469
	sub	w4, w19, w4
	sdiv	w1, w4, w2
	add	w10, w1, w10
	msub	w4, w1, w2, w4
	add	w6, w4, w6
	madd	w0, w10, w0, w6
.L493:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 224
	ret
.L481:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 224
	ret
.L483:
	mov	w6, 0
	b	.L478
.L484:
	mov	w2, w11
	mov	w6, 0
	b	.L476
.L498:
	mov	w0, 1
	lsl	w0, w0, w6
	sdiv	w1, w5, w0
	msub	w0, w1, w0, w5
	cmp	w0, wzr
	csinc	w0, w1, w1, eq
	b	.L470
.L465:
	tbz	w12, #31, .L468
.L469:
	ldr	x1, .LC138
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L493
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
	add	x0, x21, 388
	blr	x3
	str	wzr, [x21, 384]
	ldr	x2, [x19, 28176]
	ldr	x1, [x19, 28168]
	ldr	w0, [x2, 72]
	ldr	w10, [x1, 240]
	cbz	w0, .L499
	mov	w14, 0
	mov	w16, 0
	mov	w4, 0
.L508:
	ldr	w0, [x2, 68]
	uxtw	x15, w14
	cbz	w0, .L509
	add	x9, x15, 104
	ldr	w1, [x21, 384]
	mul	w13, w16, w10
	mov	w11, 0
	mov	w12, 0
	lsl	x9, x9, 1
.L507:
	uxtw	x0, w1
	add	x3, x2, x9
	add	x5, x0, 8192
	add	x5, x19, x5, lsl 3
	str	w4, [x5, 388]
	uxtw	x5, w11
	ldrsh	w3, [x3, 8]
	cbz	w3, .L503
	add	w8, w12, w13
	mov	w7, 0
	uxtw	x5, w11
	.p2align 2
.L505:
	add	x1, x5, 88
	mov	w0, w4
	sub	w6, w8, w4
	ldrsh	w1, [x2, x1, lsl 1]
	cbz	w1, .L506
	.p2align 2
.L513:
	add	x2, x20, 1792
	add	w3, w6, w0
	add	x1, x5, 88
	str	w3, [x2, w0, sxtw 2]
	add	w0, w0, 1
	sub	w3, w0, w4
	ldr	x2, [x19, 28176]
	ldrsh	w1, [x2, x1, lsl 1]
	cmp	w1, w3
	bhi	.L513
	mov	w4, w0
.L506:
	add	x0, x2, x9
	add	w7, w7, 1
	add	w8, w8, w10
	ldrsh	w0, [x0, 8]
	cmp	w0, w7
	bhi	.L505
	ldr	w1, [x21, 384]
	uxtw	x0, w1
.L503:
	add	x0, x19, x0, lsl 3
	sub	w3, w4, #1
	add	x0, x0, 65536
	add	w1, w1, 1
	add	x5, x5, 88
	add	w11, w11, 1
	str	w3, [x0, 392]
	str	w1, [x21, 384]
	ldr	w3, [x2, 68]
	ldrsh	w0, [x2, x5, lsl 1]
	cmp	w3, w11
	add	w12, w12, w0
	bhi	.L507
.L509:
	add	x15, x2, x15, lsl 1
	ldr	w0, [x2, 72]
	add	w14, w14, 1
	cmp	w0, w14
	ldrsh	w0, [x15, 216]
	add	w16, w16, w0
	bhi	.L508
.L499:
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
	add	x21, x20, 53248
	add	x19, x20, 61440
	bl	_mcount
	add	x22, x20, 65536
	ldr	x1, .LC144
	mov	w0, 2
	ldr	w3, [x21, 1140]
	ldr	w2, [x21, 1324]
	bl	dprint_vfmw
	ldr	x1, .LC146
	mov	w0, 4
	bl	dprint_vfmw
	ldr	w0, [x21, 1308]
	mov	x2, 64
	str	w0, [x19, 2968]
	add	x1, x20, 3416
	add	x0, x19, 1800
	ldr	w3, [x21, 1212]
	ldr	w4, [x21, 1216]
	sub	w4, w4, w3
	str	w4, [x19, 2972]
	ldr	w4, [x21, 1324]
	str	w4, [x19, 2960]
	ldr	w4, [x21, 1140]
	str	w4, [x19, 2964]
	ldr	w4, [x21, 1312]
	str	w4, [x19, 2976]
	ldr	w4, [x21, 1316]
	str	w4, [x19, 2980]
	ldr	w4, [x21, 1320]
	str	w4, [x19, 2984]
	ldr	w4, [x21, 1236]
	sub	w3, w4, w3
	str	w3, [x19, 2988]
	ldr	w3, [x21, 1240]
	sub	w3, w3, w4
	str	w3, [x19, 2992]
	str	wzr, [x20, 208]
	ldr	x3, [x22, 2880]
	str	x3, [x22, 152]
	ldr	x3, [x20, 28168]
	ldrb	w4, [x3, 12]
	strb	w4, [x19, 1680]
	ldrb	w4, [x3, 5]
	strb	w4, [x19, 1681]
	ldrb	w4, [x3, 9]
	strb	w4, [x19, 1682]
	ldrb	w4, [x3, 8]
	strb	w4, [x19, 1683]
	ldrb	w4, [x3, 7]
	strb	w4, [x19, 1684]
	ldr	w4, [x3, 64]
	str	w4, [x19, 1688]
	ldr	w4, [x3, 116]
	strb	w4, [x19, 1692]
	ldr	w4, [x3, 244]
	sub	w4, w4, #1
	str	w4, [x19, 1696]
	ldr	w4, [x3, 240]
	sub	w4, w4, #1
	str	w4, [x19, 1700]
	ldr	w4, [x21, 1112]
	str	w4, [x19, 1704]
	ldr	x4, [x20, 28176]
	ldr	w5, [x4, 84]
	sub	w5, w5, #2
	str	w5, [x19, 1708]
	ldrb	w5, [x4, 11]
	strb	w5, [x19, 1712]
	ldrb	w4, [x4, 10]
	strb	w4, [x19, 1713]
	ldr	w4, [x3, 200]
	str	w4, [x19, 1716]
	ldr	w4, [x3, 208]
	str	w4, [x19, 1720]
	ldr	w4, [x3, 188]
	str	w4, [x19, 1724]
	ldr	w4, [x3, 196]
	str	w4, [x19, 1728]
	ldr	w4, [x3, 172]
	str	w4, [x19, 1732]
	ldr	w4, [x3, 180]
	str	w4, [x19, 1736]
	ldr	w4, [x3, 216]
	str	w4, [x19, 1740]
	ldr	w4, [x3, 140]
	str	w4, [x19, 1744]
	ldr	w4, [x3, 148]
	str	w4, [x19, 1748]
	ldr	x4, .LC147
	ldr	w5, [x3, 112]
	str	w5, [x19, 1752]
	ldr	x4, [x4, 104]
	ldr	w5, [x3, 104]
	str	w5, [x19, 1756]
	ldr	w5, [x3, 124]
	str	w5, [x19, 1760]
	ldr	w5, [x3, 120]
	str	w5, [x19, 1764]
	ldr	w5, [x3, 108]
	str	w5, [x19, 1768]
	ldr	w3, [x3, 100]
	str	w3, [x19, 1772]
	blr	x4
	ldr	x4, [x20, 28168]
	add	x3, x19, 4088
	add	x2, x19, 4008
	add	x1, x19, 3496
	mov	x0, x20
	ldr	w5, [x4, 76]
	str	w5, [x19, 1864]
	ldr	w4, [x4, 72]
	str	w4, [x19, 1868]
	bl	HEVC_CalcTileInfo
	cbnz	w0, .L539
	ldr	x1, [x20, 28176]
	mov	w0, 0
	ldrb	w2, [x1, 16]
	strb	w2, [x19, 1892]
	ldrb	w2, [x1, 7]
	strb	w2, [x19, 1893]
	ldrb	w2, [x1, 3]
	strb	w2, [x19, 1894]
	ldrb	w2, [x1, 14]
	strb	w2, [x19, 1895]
	ldrb	w2, [x1, 15]
	strb	w2, [x19, 1896]
	ldrb	w2, [x1, 13]
	strb	w2, [x19, 1897]
	ldr	x2, [x20, 28168]
	ldr	w3, [x1, 56]
	ldr	w2, [x2, 148]
	sub	w2, w2, w3
	str	w2, [x1, 88]
	ldr	x1, [x20, 28176]
	ldr	w2, [x1, 88]
	str	w2, [x19, 1900]
	ldr	w2, [x1, 52]
	str	w2, [x19, 1904]
	ldrb	w2, [x1, 8]
	strb	w2, [x19, 1908]
	ldrb	w2, [x1, 12]
	strb	w2, [x19, 1909]
	ldrb	w2, [x1, 6]
	strb	w2, [x19, 1910]
	ldr	w2, [x1, 60]
	str	w2, [x19, 1912]
	ldr	w2, [x1, 64]
	str	w2, [x19, 1916]
	ldr	w2, [x1, 72]
	sub	w2, w2, #1
	str	w2, [x19, 1920]
	ldr	w2, [x1, 68]
	sub	w2, w2, #1
	str	w2, [x19, 1924]
	ldrb	w2, [x1, 10]
	strb	w2, [x19, 1713]
	ldrb	w1, [x1, 11]
	strb	w1, [x19, 1712]
	str	wzr, [x20, 236]
	ldr	w1, [x20, 220]
	cmp	w1, 1
	beq	.L521
	ldr	x1, [x21, 1160]
	ldr	w0, [x20, 276]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	mov	x23, x0
	cbz	x0, .L540
	ldr	x2, [x0, 640]
	ldr	x3, [x0, 648]
	cbz	x2, .L524
	cbz	x3, .L524
	ldr	w0, [x20, 276]
	add	x3, x19, 3236
	add	x2, x19, 3016
	add	x1, x19, 3144
	bl	FSP_GetDecFsAddrTab
	ldr	x0, [x23, 640]
	add	x1, x19, 3228
	add	x2, x19, 3148
	ldr	w0, [x0, 8]
	str	w0, [x19, 2960]
	ldr	w0, [x20, 276]
	bl	FSP_GetPmvAddrTab
	ldr	w0, [x21, 1140]
	str	w0, [x19, 2964]
	mov	w0, 0
	ldr	w1, [x23, 668]
	str	w1, [x19, 3232]
	ldr	x1, [x23, 648]
	ldr	w1, [x1, 16]
	str	w1, [x22, 376]
	ldr	x1, [x23, 648]
	ldr	w1, [x1, 76]
	str	w1, [x22, 380]
	ldr	w3, [x20, 3280]
	str	w3, [x22, 144]
	cbz	w3, .L529
	.p2align 2
.L533:
	add	x1, x20, x0, sxtw 2
	add	w0, w0, 1
	add	x2, x1, 65536
	cmp	w0, w3
	ldr	w1, [x1, 3352]
	str	w1, [x2, 80]
	bne	.L533
.L529:
	add	x23, x19, 1932
	mov	w22, 0
.L527:
	mov	x2, x23
	mov	w0, w22
	mov	x1, x20
	add	w22, w22, 1
	add	x23, x23, 64
	bl	HEVC_WriteQmatrix_8x8
	cmp	w22, 4
	bne	.L527
	mov	w0, w22
	add	x2, x19, 2188
	mov	x1, x20
	add	x23, x19, 2444
	mov	w22, 2
	bl	HEVC_WriteQmatrix_8x8
	add	x2, x19, 2252
	mov	x1, x20
	mov	w0, 5
	bl	HEVC_WriteQmatrix_8x8
	add	x2, x19, 2316
	mov	x1, x20
	mov	w0, 0
	bl	HEVC_WriteQmatrix_16x16
	add	x2, x19, 2380
	mov	x1, x20
	mov	w0, 1
	bl	HEVC_WriteQmatrix_16x16
.L530:
	mov	x2, x23
	mov	w0, w22
	mov	x1, x20
	add	w22, w22, 1
	add	x23, x23, 64
	bl	HEVC_WriteQmatrix_16x16
	cmp	w22, 6
	bne	.L530
	add	x2, x19, 2700
	mov	x1, x20
	mov	w0, 0
	add	x23, x19, 2828
	mov	w22, 0
	bl	HEVC_WriteQmatrix_32x32
	add	x2, x19, 2764
	mov	x1, x20
	mov	w0, 1
	bl	HEVC_WriteQmatrix_32x32
	.p2align 2
.L531:
	mov	x2, x23
	mov	w0, w22
	mov	x1, x20
	add	w22, w22, 1
	add	x23, x23, 16
	bl	HEVC_WriteQmatrix_4x4
	cmp	w22, 6
	bne	.L531
	add	x5, x20, 2048
	add	x6, x20, 2304
	mov	w0, 0
	ldp	w3, w4, [x5, 252]
	ldp	w1, w2, [x5, 244]
	lsl	w3, w3, 16
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2, lsl 8
	orr	w1, w3, w1
	str	w1, [x19, 2924]
	ldp	w3, w4, [x6, 12]
	ldp	w1, w2, [x6, 4]
	lsl	w3, w3, 16
	orr	w3, w3, w4, lsl 24
	orr	w1, w1, w2, lsl 8
	orr	w1, w3, w1
	str	w1, [x19, 2928]
	ldr	w1, [x21, 1140]
	str	w1, [x19, 2956]
.L521:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L539:
	ldr	x2, .LC148
	mov	w0, 1
	ldr	x1, .LC150
	add	x2, x2, 296
	bl	dprint_vfmw
	mov	w0, -1
	b	.L521
.L524:
	ldr	x1, .LC154
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L521
.L540:
	ldr	x2, [x21, 1160]
	mov	w0, 1
	ldr	x1, .LC152
	ldr	w2, [x2, 32]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L521
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
	add	x0, x0, 1792
	ldrb	w1, [x0, 3]
	cbz	w1, .L543
	ldrsb	w1, [x19, 9]
	add	x0, x19, 57344
	add	x0, x0, 1428
	cbz	w1, .L575
.L543:
	ldp	w4, w5, [x0, 160]
	cmp	w4, 16
	bhi	.L576
	cmp	w5, 16
	bhi	.L547
	cbz	w4, .L548
	ldr	x0, [x19, 2400]
	cbz	x0, .L561
	ldr	x0, [x0, 32]
	cbz	x0, .L577
	add	x0, x19, 2408
	mov	w2, 0
	b	.L553
	.p2align 3
.L554:
	ldr	x1, [x0]
	add	x0, x0, 8
	cbz	x1, .L549
	ldr	x1, [x1, 32]
	cbz	x1, .L550
.L553:
	add	w2, w2, 1
	cmp	w4, w2
	bne	.L554
.L548:
	cbz	w5, .L563
	ldr	x0, [x19, 2536]
	cbz	x0, .L564
	ldr	x0, [x0, 32]
	cbz	x0, .L578
	add	x0, x19, 2544
	mov	w2, 0
	b	.L559
	.p2align 3
.L560:
	ldr	x1, [x0]
	add	x0, x0, 8
	cbz	x1, .L555
	ldr	x1, [x1, 32]
	cbz	x1, .L556
.L559:
	add	w2, w2, 1
	cmp	w5, w2
	bne	.L560
.L563:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L561:
	mov	w2, 0
	.p2align 2
.L549:
	ldr	x1, .LC163
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L545:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L577:
	mov	w2, 0
.L550:
	ldr	x1, .LC165
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L564:
	mov	w2, 0
.L555:
	ldr	x1, .LC167
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L578:
	mov	w2, 0
.L556:
	ldr	x1, .LC169
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L575:
	ldr	x2, .LC155
	mov	w0, 0
	ldr	x1, .LC157
	add	x2, x2, 320
	bl	dprint_vfmw
	mov	w0, -1
	b	.L545
.L547:
	ldr	x1, .LC161
	mov	w3, 17
	mov	w2, w5
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L545
.L576:
	ldr	x1, .LC159
	mov	w3, 17
	mov	w2, w4
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L545
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
	cbnz	w0, .L620
	ldr	w3, [x19, 144]
	ldr	w20, [x19, 208]
	cmp	w20, w3
	bcs	.L660
	add	x25, x19, 65536
	mov	w21, 1368
	ldr	x22, .LC172
	mov	x2, 1368
	umull	x20, w20, w21
	ldr	x0, [x25, 2880]
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
	bcc	.L661
	str	xzr, [x20, 1360]
.L583:
	ldr	w0, [x19, 248]
	add	x24, x19, 53248
	add	w1, w0, 1
	uxtw	x0, w0
	ldr	w2, [x24, 1868]
	add	x0, x0, 16384
	add	x0, x0, 724
	str	w2, [x19, x0, lsl 2]
	str	w1, [x19, 248]
	ldr	w21, [x19, 172]
	add	w0, w21, 1
	str	w0, [x19, 172]
	lsl	x21, x0, 5
	ldr	x1, [x25, 3696]
	add	x21, x21, x0, lsl 3
	ldr	w2, [x24, 1852]
	add	x0, x1, x21
	str	w2, [x0, 8]
	ldr	x3, [x19, 2392]
	ldrb	w1, [x3, 1]
	ldr	w0, [x3, 8]
	cmp	w1, 1
	ccmp	w1, w0, 2, ls
	bcs	.L584
	.p2align 2
.L585:
	uxtw	x2, w1
	add	x0, x2, 2
	lsl	x0, x0, 5
	add	x2, x20, x2, lsl 2
	add	x3, x3, x0
	ldr	w3, [x3, 4]
	str	w3, [x2, 8]
	ldr	x3, [x19, 2392]
	ldr	w3, [x3, x0]
	str	w3, [x20, w1, uxtw 2]
	add	w1, w1, 1
	ldr	x3, [x19, 2392]
	add	x0, x3, x0
	ldr	w0, [x0, 8]
	str	w0, [x2, 16]
	ldr	x3, [x19, 2392]
	ldr	w0, [x3, 8]
	cmp	w0, w1
	ccmp	w1, 1, 2, hi
	bls	.L585
.L584:
	cmp	w0, 1
	bhi	.L590
	.p2align 2
.L644:
	add	x1, x20, x0, uxtw 2
	str	wzr, [x1, 8]
	str	wzr, [x20, w0, uxtw 2]
	add	w0, w0, 1
	cmp	w0, 2
	str	wzr, [x1, 16]
	bne	.L644
.L590:
	ldrb	w0, [x24, 1795]
	add	x26, x19, 57344
	cbz	w0, .L662
.L588:
	ldr	w0, [x26, 1588]
	cbz	w0, .L591
	mov	w2, 0
	.p2align 2
.L592:
	uxtw	x0, w2
	add	x6, x29, 160
	add	w2, w2, 1
	add	x4, x19, x0, lsl 3
	add	x0, x20, x0, lsl 2
	ldr	x5, [x4, 2400]
	ldr	x3, [x5, 32]
	ldp	w1, w3, [x3, 24]
	add	x1, x1, 12288
	add	x3, x6, x3, lsl 2
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 15436]
	str	w1, [x3, -64]
	ldr	w1, [x5, 16]
	str	w1, [x0, 116]
	ldr	x1, [x4, 2400]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 28]
	str	w1, [x0, 372]
	ldr	w0, [x26, 1588]
	cmp	w0, w2
	bhi	.L592
	ldr	w3, [x26, 1592]
	cbz	w3, .L593
.L619:
	mov	w2, 0
	.p2align 2
.L594:
	uxtw	x0, w2
	add	x6, x29, 160
	add	w2, w2, 1
	add	x4, x19, x0, lsl 3
	add	x0, x20, x0, lsl 2
	ldr	x5, [x4, 2536]
	ldr	x3, [x5, 32]
	ldp	w1, w3, [x3, 24]
	add	x1, x1, 12288
	add	x3, x6, x3, lsl 2
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 15436]
	str	w1, [x3, -64]
	ldr	w1, [x5, 16]
	str	w1, [x0, 180]
	ldr	x1, [x4, 2536]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 28]
	str	w1, [x0, 436]
	ldr	w3, [x26, 1592]
	cmp	w3, w2
	bhi	.L594
	ldr	w0, [x26, 1588]
.L593:
	cbz	w0, .L595
	ldr	w1, [x20, 372]
	ldr	w4, [x24, 1112]
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 3416]
	cmp	w4, w1
	blt	.L626
	sub	w0, w0, #1
	add	x2, x20, x0, lsl 2
	mov	x0, x20
	b	.L598
	.p2align 3
.L599:
	ldr	w1, [x0, 376]
	add	x0, x0, 4
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 3416]
	cmp	w1, w4
	bgt	.L626
.L598:
	cmp	x2, x0
	bne	.L599
.L595:
	ldr	w0, [x26, 1488]
	cbnz	w0, .L659
	mov	w0, 1
	cbz	w3, .L596
	ldr	w0, [x20, 436]
	ldr	w2, [x24, 1112]
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 3416]
	cmp	w2, w0
	blt	.L626
	sub	w3, w3, #1
	mov	x0, x20
	add	x3, x20, x3, lsl 2
	b	.L601
	.p2align 3
.L602:
	ldr	w1, [x0, 440]
	add	x0, x0, 4
	add	x1, x19, x1, lsl 2
	ldr	w1, [x1, 3416]
	cmp	w1, w2
	bgt	.L626
.L601:
	cmp	x3, x0
	bne	.L602
.L659:
	mov	w0, 1
.L596:
	strb	w0, [x20, 24]
	ldrb	w0, [x24, 1795]
	strb	w0, [x20, 25]
	ldr	w0, [x26, 1484]
	str	w0, [x20, 28]
	ldrb	w0, [x26, 1438]
	strb	w0, [x20, 32]
	ldr	w0, [x26, 1508]
	str	w0, [x20, 36]
	ldr	w0, [x26, 1488]
	cbz	w0, .L663
	cmp	w0, 1
	mov	w27, 0
	beq	.L604
.L605:
	str	w27, [x20, 40]
	ldr	w0, [x26, 1540]
	str	w0, [x20, 48]
	ldrb	w0, [x26, 1439]
	strb	w0, [x20, 52]
	ldr	w0, [x26, 1544]
	str	w0, [x20, 44]
	ldr	w0, [x26, 1596]
	cbz	w0, .L606
	sub	w0, w0, #1
	str	w0, [x20, 60]
.L607:
	ldr	w0, [x26, 1600]
	cbz	w0, .L608
	sub	w0, w0, #1
	str	w0, [x20, 56]
.L609:
	ldrb	w0, [x26, 1435]
	strb	w0, [x20, 64]
	ldrb	w0, [x26, 1437]
	strb	w0, [x20, 65]
	ldr	w0, [x26, 1588]
	str	w0, [x20, 68]
	ldr	w0, [x26, 1592]
	str	w0, [x20, 72]
	ldr	w0, [x26, 1488]
	str	w0, [x20, 76]
	ldr	w0, [x24, 1872]
	str	w0, [x20, 80]
	ldr	x0, [x19, 28168]
	ldr	w3, [x24, 1872]
	ldr	w1, [x0, 240]
	mov	x0, x19
	udiv	w2, w3, w1
	msub	w1, w2, w1, w3
	str	w1, [x20, 84]
	ldr	x1, [x19, 28168]
	ldr	w2, [x1, 240]
	ldr	w1, [x24, 1872]
	udiv	w1, w1, w2
	str	w1, [x20, 88]
	ldr	w1, [x24, 1872]
	bl	HEVC_CtbRsAddrToTsAddr
	str	w0, [x20, 92]
	ldr	w0, [x26, 1552]
	str	w0, [x20, 96]
	ldr	w0, [x26, 1556]
	str	w0, [x20, 100]
	ldr	w0, [x26, 1564]
	str	w0, [x20, 104]
	ldr	w0, [x26, 1560]
	str	w0, [x20, 108]
	ldrb	w0, [x26, 1434]
	strb	w0, [x20, 112]
	ldrb	w0, [x26, 1433]
	strb	w0, [x20, 113]
	ldrb	w0, [x26, 1441]
	strb	w0, [x20, 114]
	ldrb	w0, [x26, 1442]
	strb	w0, [x20, 115]
	ldr	x0, [x19, 28176]
	ldrb	w1, [x0, 10]
	cbz	w1, .L610
	ldr	w1, [x26, 1488]
	cmp	w1, 1
	beq	.L611
.L610:
	ldrb	w0, [x0, 11]
	cmp	w0, 1
	beq	.L612
	add	x24, x19, 61440
.L613:
	ldrb	w0, [x26, 1435]
	cbz	w0, .L615
	add	x0, x29, 160
	add	x27, x0, x27, sxtw 2
	ldr	w0, [x27, -64]
	str	w0, [x20, 564]
.L615:
	ldr	w0, [x19, 220]
	cbnz	w0, .L616
	ldr	x0, [x25, 3696]
	ldr	x1, [x19, 2392]
	add	x21, x0, x21
	str	x1, [x21, 32]
	str	xzr, [x19, 2392]
.L616:
	ldrb	w0, [x24, 1897]
	cbz	w0, .L635
	ldr	w0, [x19, 248]
	cmp	w0, 1
	beq	.L664
	sub	w1, w0, #2
	sub	w0, w0, #1
	ldr	w3, [x20, 92]
	add	x2, x0, 8192
	add	x1, x19, x1, lsl 3
	add	x1, x1, 65536
	add	x2, x19, x2, lsl 3
	sub	w3, w3, #1
	str	w3, [x1, 1272]
	ldr	w1, [x20, 92]
	str	w1, [x2, 1268]
.L618:
	ldr	w1, [x24, 1696]
	add	x0, x19, x0, lsl 3
	ldr	w2, [x24, 1700]
	add	x0, x0, 65536
	mov	w3, 36863
	madd	w2, w1, w2, w2
	add	w1, w2, w1
	str	w1, [x0, 1272]
	ldr	w1, [x20, 92]
	sub	w1, w1, #1
	cmp	w1, w3
	bhi	.L635
	mov	x0, x19
	bl	HEVC_CtbTsAddrToRsAddr
	str	w0, [x20, 568]
.L635:
	mov	w0, w23
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L661:
	ldr	x1, [x25, 2880]
	umull	x0, w0, w21
	add	x0, x1, x0
	str	x0, [x20, 1360]
	b	.L583
	.p2align 3
.L626:
	mov	w0, 0
	b	.L596
	.p2align 3
.L608:
	str	wzr, [x20, 56]
	b	.L609
	.p2align 3
.L606:
	str	wzr, [x20, 60]
	b	.L607
	.p2align 3
.L663:
	ldrb	w0, [x26, 1439]
	cbnz	w0, .L604
	ldrsw	x0, [x26, 1540]
	add	x0, x20, x0, lsl 2
	ldr	w27, [x0, 436]
	b	.L605
	.p2align 3
.L604:
	ldrsw	x0, [x26, 1540]
	add	x0, x20, x0, lsl 2
	ldr	w27, [x0, 372]
	b	.L605
	.p2align 3
.L662:
	ldr	x3, [x22, 104]
	mov	x2, 3732
	add	x1, x24, 1792
	add	x0, x26, 1428
	blr	x3
	mov	w0, 1
	strb	w0, [x19, 9]
	b	.L588
	.p2align 3
.L612:
	ldr	w0, [x26, 1488]
	add	x24, x19, 61440
	cbnz	w0, .L613
.L611:
	ldr	w0, [x26, 3020]
	mov	x2, 64
	str	w0, [x20, 580]
	add	x1, x26, 4056
	ldr	x3, [x22, 104]
	add	x24, x19, 61440
	ldr	w0, [x26, 4052]
	str	w0, [x20, 584]
	add	x0, x20, 588
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 64
	add	x1, x26, 3220
	add	x0, x20, 716
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 128
	add	x1, x24, 24
	add	x0, x20, 844
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 128
	add	x1, x24, 152
	add	x0, x20, 1100
	blr	x3
	ldr	w0, [x26, 1488]
	cbnz	w0, .L613
	ldr	x3, [x22, 104]
	add	x1, x24, 280
	mov	x2, 64
	add	x0, x20, 652
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x26, 3732
	mov	x2, 64
	add	x0, x20, 780
	blr	x3
	ldr	x3, [x22, 104]
	add	x1, x24, 344
	mov	x2, 128
	add	x0, x20, 972
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 128
	add	x1, x24, 472
	add	x0, x20, 1228
	blr	x3
	b	.L613
	.p2align 3
.L664:
	ldr	w1, [x20, 92]
	mov	x0, 0
	str	w1, [x25, 1268]
	b	.L618
.L591:
	ldr	w0, [x26, 1592]
	cbnz	w0, .L619
	mov	w0, 1
	b	.L596
.L620:
	mov	w23, -1
	b	.L635
.L660:
	ldr	x1, .LC171
	mov	w2, w20
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L635
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
	ldr	w4, [x2, 1132]
	cmp	w4, wzr
	cset	w4, eq
	str	w4, [x1, 3012]
	ldr	w2, [x2, 1136]
	str	w2, [x1, 3004]
	ldr	w1, [x19, 208]
	str	w1, [x3, 148]
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
	ldr	w3, [x21, 1868]
	tbnz	w3, #31, .L670
	cmp	w3, w4
	bge	.L670
	ldr	x0, [x19, 2368]
	mov	w1, 2024
	ldr	w4, [x19, 152]
	smaddl	x3, w3, w1, x0
	ldr	w3, [x3, 32]
	tbnz	w3, #31, .L674
	cmp	w3, w4
	bge	.L674
	add	x20, x19, 57344
	ldr	x2, [x19, 2360]
	mov	w4, 26164
	mov	w0, 1
	smaddl	x3, w3, w4, x2
	ldr	w5, [x20, 980]
	ldr	w6, [x20, 984]
	ldr	w9, [x20, 988]
	add	w8, w5, w6
	ldr	w2, [x3, 128]
	cmp	w8, w9
	mov	w3, w8
	lsl	w2, w0, w2
	bcs	.L686
	cmp	w8, 15
	bgt	.L677
	ldr	w4, [x21, 1112]
	b	.L679
	.p2align 3
.L697:
	cmp	w9, w3
	bls	.L686
	cmp	w3, 16
	beq	.L677
.L679:
	add	x0, x19, x3, sxtw 2
	mov	w1, w4
	add	x7, x0, 53248
	add	x0, x0, 57344
	add	w3, w3, 1
	ldr	w7, [x7, 3316]
	ldr	w0, [x0, 1104]
	cbnz	w7, .L680
	sdiv	w1, w4, w2
	sdiv	w7, w0, w2
	msub	w1, w1, w2, w4
	msub	w0, w7, w2, w0
.L680:
	cmp	w1, w0
	bne	.L697
	mov	w19, 1
.L676:
	cbz	w8, .L672
	ldr	w0, [x20, 1036]
	add	x1, x20, 1040
	ldr	w3, [x21, 1840]
	mov	w4, 0
	ldr	w2, [x21, 1112]
	add	w0, w3, w0
	cmp	w2, w0
	bne	.L684
	b	.L690
	.p2align 3
.L683:
	ldr	w0, [x1], 4
	add	w0, w3, w0
	cmp	w0, w2
	beq	.L690
.L684:
	add	w4, w4, 1
	cmp	w4, w8
	beq	.L672
	cmp	w4, 17
	bne	.L683
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
.L674:
	ldr	x1, .LC176
	mov	w2, 6934
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
.L672:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L670:
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
.L686:
	mov	w19, 0
	b	.L676
	.p2align 3
.L677:
	ldr	x1, .LC178
	mov	w5, w9
	mov	w4, 16
	mov	w2, 6954
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	ldr	w5, [x20, 980]
	ldr	w6, [x20, 984]
	add	w8, w5, w6
	b	.L676
	.p2align 3
.L690:
	mov	w19, 1
	b	.L672
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
	ldr	w3, [x23, 1852]
	cmp	w3, 1
	beq	.L700
	cmp	w3, 2
	beq	.L716
	cbz	w3, .L715
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
.L703:
	ldr	x0, [x25, 2376]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L711
	mov	w0, 1
	str	w0, [x27]
	ldr	x0, [x23, 1160]
	add	x0, x0, 40
	str	x0, [x28, -8]
	.p2align 2
.L711:
	add	w26, w26, 1
	add	x27, x27, 4
	cmp	w26, w22
	add	x28, x28, 136
	bcc	.L713
.L716:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 112
	ret
.L715:
	mov	w22, 2
.L702:
	ldr	x21, .LC181
	add	x27, x23, 1952
	add	x28, x25, 2408
	mov	w26, 0
	add	x0, x21, 336
	str	x0, [x29, 96]
	.p2align 2
.L713:
	ldr	w1, [x27]
	cbz	w1, .L703
	ldr	x2, [x28, -8]
	cbnz	x2, .L717
	mov	x3, x28
	mov	w0, 0
	b	.L705
	.p2align 3
.L707:
	ldr	x24, [x3], 8
	cbnz	x24, .L722
.L705:
	add	w0, w0, 1
	cmp	w1, w0
	bne	.L707
	mov	w20, -1
.L706:
	ldr	x0, [x25, 2376]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L708
	ldr	x24, [x23, 1160]
	add	x24, x24, 40
.L704:
	mov	x0, x2
	uxtw	x8, w26
	mov	w19, 0
	lsl	x21, x8, 4
	cbz	x0, .L723
	.p2align 2
.L709:
	add	w19, w19, 1
	add	x0, x21, x8
	cmp	w1, w19
	add	x0, x0, x19, uxtw
	add	x0, x0, 300
	bls	.L711
	ldr	x0, [x25, x0, lsl 3]
	cbnz	x0, .L709
.L723:
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
	add	x0, x0, 300
	str	x24, [x25, x0, lsl 3]
	ldr	w1, [x27]
	b	.L709
	.p2align 3
.L722:
	sxtb	w20, w0
	cmn	w20, #1
	ccmp	x24, xzr, 4, ne
	bne	.L704
	b	.L706
	.p2align 3
.L717:
	mov	x24, x2
	mov	w20, 0
	b	.L704
.L700:
	mov	w22, w3
	b	.L702
.L708:
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
	cbz	x19, .L760
	ldr	x20, .LC189
	add	x21, x29, 48
	mov	x2, 272
	mov	w1, 0
	mov	x0, x21
	ldr	x3, [x20, 96]
	blr	x3
	ldr	w0, [x19, 2728]
	cbz	w0, .L761
	sub	w3, w0, #1
	mov	w5, 0
	add	x3, x3, 360
	add	x0, x19, 2872
	mov	x4, 4294967296
	add	x3, x19, x3, lsl 3
	b	.L728
	.p2align 3
.L727:
	cmp	x0, x3
	beq	.L726
.L728:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 1]
	cmp	w2, 1
	bne	.L727
	ldr	x2, [x1, 56]
	cmp	x2, x4
	bne	.L727
	add	x2, x29, 320
	add	x1, x1, 40
	add	x2, x2, x5, uxtw 3
	cmp	x0, x3
	sub	x2, x2, #4096
	add	w5, w5, 1
	str	x1, [x2, 3824]
	bne	.L728
	.p2align 2
.L726:
	ldr	w0, [x19, 2732]
	cbz	w0, .L762
	sub	w4, w0, #1
	mov	w3, w5
	add	x4, x4, 377
	add	x0, x19, 3008
	mov	x6, 4294967296
	add	x4, x19, x4, lsl 3
	b	.L731
	.p2align 3
.L730:
	cmp	x0, x4
	beq	.L789
.L731:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 1]
	cmp	w2, 1
	bne	.L730
	ldr	x2, [x1, 56]
	cmp	x2, x6
	bne	.L730
	add	x2, x29, 320
	add	x1, x1, 40
	add	x2, x2, x3, uxtw 3
	cmp	x0, x4
	sub	x2, x2, #4096
	add	w3, w3, 1
	str	x1, [x2, 3824]
	bne	.L731
	.p2align 2
.L789:
	sub	w6, w3, w5
.L729:
	ldr	w0, [x19, 2724]
	sub	w7, w3, #1
	cbz	w0, .L732
	sub	w2, w0, #1
	add	x0, x19, 3144
	add	x2, x2, 394
	add	x2, x19, x2, lsl 3
	b	.L734
	.p2align 3
.L733:
	cmp	x2, x0
	beq	.L790
.L734:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w4, [x1, 1]
	cmp	w4, 1
	bne	.L733
	ldr	x4, [x1, 56]
	cmp	x4, 1
	bne	.L733
	add	x4, x29, 320
	add	x1, x1, 40
	add	x4, x4, x3, uxtw 3
	cmp	x2, x0
	sub	x4, x4, #4096
	add	w3, w3, 1
	str	x1, [x4, 3824]
	bne	.L734
	.p2align 2
.L790:
	sub	w0, w3, w5
	sub	w0, w0, w6
.L732:
	cbz	w3, .L791
	add	x22, x19, 53248
	ldr	w8, [x22, 1852]
	cbnz	w8, .L792
	cbz	w6, .L740
	add	x4, x21, 136
	add	w9, w6, w5
	mov	w1, w5
	.p2align 2
.L741:
	add	x2, x29, 320
	add	x2, x2, x1, uxtw 3
	add	w1, w1, 1
	sub	x2, x2, #4096
	cmp	w9, w1
	ldr	x2, [x2, 3824]
	str	x2, [x4], 8
	bne	.L741
.L740:
	cbz	w5, .L742
	mov	x1, 0
	.p2align 2
.L743:
	add	w2, w6, w1
	add	x9, x29, 320
	ldr	x4, [x21, x1, lsl 3]
	add	x1, x1, 1
	add	x2, x9, x2, lsl 3
	cmp	w5, w1
	str	x4, [x2, -136]
	bhi	.L743
	add	w6, w6, w5
.L742:
	cbz	w0, .L744
	add	w1, w0, w6
	mov	w2, w6
	.p2align 2
.L745:
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
	bne	.L745
.L744:
	ldr	w5, [x22, 1960]
	ldr	w7, [x22, 1964]
	str	w5, [x22, 1952]
	str	w7, [x22, 1956]
	cbnz	w5, .L738
	.p2align 2
.L758:
	cbz	w7, .L750
	add	x0, x19, 57344
	add	x4, x19, 2536
	add	x2, x0, 1300
	mov	w1, 0
	ldrb	w6, [x0, 1169]
	.p2align 2
.L753:
	cbz	w6, .L751
	ldr	w0, [x2]
.L752:
	add	x8, x29, 320
	add	w1, w1, 1
	add	x0, x8, x0, uxtw 3
	cmp	w1, w7
	add	x2, x2, 4
	ldr	x0, [x0, -136]
	str	x0, [x4], 8
	bne	.L753
	mov	w0, w5
.L749:
	cmp	w0, 16
	bhi	.L763
	.p2align 2
.L755:
	add	x1, x19, x0, uxtw 3
	ldr	x2, [x19, 2400]
	add	w0, w0, 1
	cmp	w0, 16
	str	x2, [x1, 2400]
	bls	.L755
.L763:
	cmp	w7, 16
	mov	w0, w7
	bhi	.L756
	.p2align 2
.L757:
	add	x1, x19, x0, uxtw 3
	ldr	x2, [x19, 2536]
	add	w0, w0, 1
	cmp	w0, 17
	str	x2, [x1, 2536]
	bne	.L757
.L756:
	mov	w0, 0
.L786:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 320
	ret
	.p2align 3
.L792:
	ldr	w5, [x22, 1960]
	ldr	w7, [x22, 1964]
	str	w5, [x22, 1952]
	str	w7, [x22, 1956]
	cbz	w5, .L793
.L738:
	add	x0, x19, 57344
	add	x4, x19, 2400
	add	x2, x0, 1172
	ldrb	w6, [x0, 1168]
	mov	w0, 0
	.p2align 2
.L748:
	cbz	w6, .L746
	ldr	w1, [x2]
.L747:
	add	x9, x29, 320
	add	w0, w0, 1
	add	x1, x9, x1, uxtw 3
	cmp	w0, w5
	sub	x1, x1, #4096
	add	x2, x2, 4
	ldr	x1, [x1, 3824]
	str	x1, [x4], 8
	bne	.L748
	cbnz	w8, .L749
	b	.L758
	.p2align 3
.L746:
	udiv	w1, w0, w3
	msub	w1, w1, w3, w0
	b	.L747
	.p2align 3
.L751:
	udiv	w0, w1, w3
	msub	w0, w0, w3, w1
	b	.L752
	.p2align 3
.L791:
	add	x21, x19, 53248
	ldr	x3, [x20, 96]
	mov	x2, 272
	mov	w1, 0
	add	x0, x19, 2400
	str	wzr, [x21, 1952]
	str	wzr, [x21, 1956]
	blr	x3
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L756
	ldr	x1, [x21, 1160]
	mov	w0, 1
	str	w0, [x21, 1952]
	mov	w0, 0
	add	x1, x1, 40
	str	x1, [x19, 2400]
	b	.L786
.L750:
	ldr	x3, [x20, 104]
	mov	x2, 136
	add	x1, x19, 2400
	add	x0, x19, 2536
	blr	x3
	ldr	w7, [x22, 1952]
	str	w7, [x22, 1956]
	mov	w0, w7
	b	.L749
.L761:
	mov	w5, 0
	b	.L726
.L762:
	mov	w3, w5
	mov	w6, 0
	b	.L729
.L793:
	mov	w0, 0
	b	.L755
.L760:
	mov	w0, -1
	b	.L786
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
	ldr	w1, [x0, 1852]
	cmp	w1, 2
	beq	.L800
	mov	x0, x19
	bl	HEVC_InitListX
	cbnz	w0, .L801
	mov	x0, x19
	bl	HEVC_RepairList
	mov	w1, 0
	cbnz	w0, .L802
.L796:
	mov	w0, w1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L800:
	str	wzr, [x0, 1952]
	mov	w1, 0
	str	wzr, [x0, 1956]
	mov	w0, w1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L801:
	ldr	x1, .LC191
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L796
.L802:
	ldr	x1, .LC193
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L796
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
	ldr	x2, [x20, 2736]
	ldr	w0, [x2, 28]
	cmp	w0, 16
	beq	.L803
	add	x3, x19, x0, uxtw 2
	ldr	x1, .LC195
	mov	w0, 13
	str	wzr, [x3, 3288]
	ldr	w3, [x2, 28]
	add	x3, x19, x3, lsl 2
	str	wzr, [x3, 3352]
	ldr	w3, [x2, 28]
	add	x3, x19, x3, lsl 2
	str	wzr, [x3, 3416]
	ldr	w3, [x2, 24]
	add	x3, x19, x3, lsl 2
	str	wzr, [x3, 2148]
	ldp	w3, w2, [x2, 24]
	bl	dprint_vfmw
	ldr	x0, [x20, 2736]
	mov	w1, 16
	str	w1, [x0, 28]
	ldr	w0, [x19, 3284]
	sub	w0, w0, #1
	str	w0, [x19, 3284]
.L803:
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
	ldr	w1, [x20, 2716]
	cbnz	w1, .L813
	b	.L808
	.p2align 3
.L810:
	add	w19, w19, 1
	cmp	w1, w19
	bls	.L808
.L813:
	add	x0, x20, x19, sxtw 3
	ldr	x0, [x0, 2736]
	ldrb	w0, [x0, 1]
	cbnz	w0, .L810
	mov	w1, w19
	mov	x0, x20
	add	w19, w19, 1
	bl	HEVC_RemoveApcStore
	ldr	w1, [x20, 2716]
	cmp	w1, w19
	bhi	.L813
.L808:
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
	ldr	x1, [x26, 2368]
	mov	w5, 2024
	ldr	w2, [x23, 1868]
	mov	w4, 26164
	ldr	x0, [x26, 2360]
	add	x19, x26, 57344
	ldr	w3, [x26, 2716]
	smaddl	x1, w2, w5, x1
	ldr	w1, [x1, 32]
	smaddl	x0, w1, w4, x0
	ldr	w25, [x0, 128]
	lsl	w25, w22, w25
	cbz	w3, .L815
	add	x19, x19, 964
	mov	w24, w22
	.p2align 2
.L844:
	ldp	w2, w0, [x19, 16]
	ldr	w1, [x19, 24]
	sxtw	x21, w27
	add	w0, w2, w0
	cmp	w0, w1
	mov	w20, w0
	bcs	.L853
	add	x28, x26, x21, lsl 3
	mov	w4, 0
	ldr	x2, [x28, 2736]
	ldr	w1, [x2, 12]
	.p2align 2
.L824:
	add	x0, x26, x20, sxtw 2
	add	x3, x0, 53248
	ldr	w3, [x3, 3316]
	cbz	w3, .L819
	ldr	w3, [x2, 56]
	cbz	w3, .L820
	add	x3, x0, 57344
	ldr	w3, [x3, 1104]
	cmp	w3, w1
	beq	.L852
.L820:
	cbnz	w4, .L851
	add	x0, x0, 57344
	ldrb	w4, [x2, 1]
	mov	w3, w1
	cmp	w4, wzr
	ldr	w0, [x0, 1104]
	ccmp	w3, w0, 0, ne
	beq	.L854
.L837:
	mov	w3, w1
	mov	w4, 0
.L823:
	ldr	w0, [x19, 24]
	add	w20, w20, 1
	mov	w1, w3
	cmp	w0, w20
	bhi	.L824
	ldp	w1, w0, [x19, 16]
	add	w0, w1, w0
.L818:
	ldr	w5, [x23, 1840]
	cbz	w0, .L826
	mov	w20, 0
	lsl	x6, x21, 3
	.p2align 2
.L828:
	ldr	w0, [x2, 56]
	add	x1, x19, x20, sxtw 2
	add	w20, w20, 1
	cbnz	w0, .L827
	ldr	w0, [x1, 72]
	add	w0, w5, w0
	cmp	w0, w3
	beq	.L855
.L827:
	ldp	w1, w0, [x19, 16]
	add	w0, w1, w0
	cmp	w0, w20
	bhi	.L828
.L826:
	cmp	w4, wzr
	ccmp	w3, w5, 4, eq
	beq	.L830
	add	x21, x26, x21, lsl 3
	strb	wzr, [x2, 1]
	ldr	x0, [x21, 2736]
	str	wzr, [x0, 56]
	ldr	x0, [x21, 2736]
	ldr	w1, [x0, 8]
	cmp	w1, 1
	beq	.L830
	ldr	w1, [x0, 32]
	mov	w2, 0
	ldr	w0, [x26, 276]
	bl	FSP_SetRef
.L830:
	ldr	w0, [x26, 2716]
	add	w27, w27, 1
	cmp	w0, w27
	bhi	.L844
.L815:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L819:
	ldr	w3, [x2, 56]
	cbz	w3, .L822
	sdiv	w3, w1, w25
	add	x5, x0, 57344
	ldr	w6, [x5, 1104]
	sdiv	w5, w6, w25
	msub	w3, w3, w25, w1
	msub	w5, w5, w25, w6
	cmp	w3, w5
	bne	.L822
.L852:
	strb	w24, [x2, 1]
	mov	w2, 1
	ldr	x0, [x28, 2736]
	str	w22, [x0, 4]
	ldr	x1, [x28, 2736]
	ldr	w0, [x26, 276]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x2, [x28, 2736]
	mov	w4, 1
	ldr	w3, [x2, 12]
	b	.L823
	.p2align 3
.L822:
	cbnz	w4, .L851
	sdiv	w3, w1, w25
	add	x0, x0, 57344
	ldr	w4, [x0, 1104]
	sdiv	w0, w4, w25
	msub	w3, w3, w25, w1
	msub	w0, w0, w25, w4
	ldrb	w4, [x2, 1]
	cmp	w4, wzr
	ccmp	w3, w0, 0, ne
	bne	.L837
.L854:
	strb	w24, [x2, 1]
	mov	w2, 1
	ldr	x0, [x28, 2736]
	str	w22, [x0, 56]
	ldr	x0, [x28, 2736]
	str	w22, [x0, 4]
	ldr	x1, [x28, 2736]
	ldr	w0, [x26, 276]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x2, [x28, 2736]
	mov	w4, 1
	ldr	w3, [x2, 12]
	b	.L823
	.p2align 3
.L851:
	mov	w3, w1
	b	.L823
	.p2align 3
.L855:
	add	x28, x26, x6
	strb	w24, [x2, 1]
	mov	w2, 1
	str	x6, [x29, 104]
	ldr	x0, [x28, 2736]
	str	w22, [x0, 4]
	ldr	x1, [x28, 2736]
	ldr	w0, [x26, 276]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x2, [x28, 2736]
	mov	w4, 1
	ldr	w5, [x23, 1840]
	ldr	x6, [x29, 104]
	ldr	w3, [x2, 12]
	b	.L827
.L853:
	add	x1, x26, x21, lsl 3
	mov	w4, 0
	ldr	x2, [x1, 2736]
	ldr	w3, [x2, 12]
	b	.L818
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
	ldr	x0, [x19, 2368]
	mov	w4, 2024
	add	x17, x19, 57344
	add	x18, x17, 964
	ldr	x1, [x19, 2360]
	ldr	w2, [x7, 1868]
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
	bcs	.L857
	ldr	w3, [x19, 2716]
	mov	w10, 0
	mov	w9, 0
	.p2align 2
.L858:
	sxtw	x16, w15
	cbz	w3, .L864
	add	x11, x19, x16, lsl 2
	mov	w14, 0
	add	x1, x11, 53248
	mov	w0, 0
	add	x11, x11, 57344
	ldr	w4, [x1, 3316]
	b	.L861
	.p2align 3
.L898:
	ldr	x1, [x2, 2736]
	ldr	w2, [x1, 56]
	cbz	w2, .L860
	ldr	w6, [x1, 12]
	ldr	w2, [x11, 1104]
	cmp	w6, w2
	beq	.L896
.L860:
	add	w0, w0, 1
	cmp	w0, w3
	beq	.L897
.L861:
	add	x2, x19, x0, sxtw 3
	cbnz	w4, .L898
	ldr	x1, [x2, 2736]
	ldr	w2, [x1, 56]
	cbz	w2, .L860
	ldr	w13, [x1, 12]
	ldr	w12, [x11, 1104]
	sdiv	w6, w13, w5
	sdiv	w2, w12, w5
	msub	w6, w6, w5, w13
	msub	w2, w2, w5, w12
	cmp	w6, w2
	bne	.L860
.L896:
	ldrb	w1, [x1, 1]
	add	w0, w0, 1
	cmp	w1, wzr
	csinc	w14, w14, wzr, eq
	cmp	w0, w3
	bne	.L861
	.p2align 2
.L897:
	cbz	w14, .L864
.L862:
	add	w15, w15, 1
	cmp	w30, w15
	bne	.L858
	cbz	w8, .L866
.L865:
	mov	w12, 0
	add	x17, x17, 964
	.p2align 2
.L870:
	mov	w11, 0
	mov	w0, 0
	cbz	w3, .L872
	sxtw	x6, w12
	add	x6, x6, 16
	add	x6, x17, x6, lsl 2
	b	.L888
	.p2align 3
.L867:
	cmp	w0, w3
	beq	.L899
.L888:
	add	x2, x19, x0, sxtw 3
	add	w0, w0, 1
	ldr	x2, [x2, 2736]
	ldr	w4, [x2, 56]
	cbnz	w4, .L867
	ldr	w4, [x6, 8]
	ldr	w5, [x7, 1840]
	ldr	w1, [x2, 12]
	add	w4, w5, w4
	cmp	w1, w4
	bne	.L867
	ldrb	w1, [x2, 1]
	cmp	w1, wzr
	csinc	w11, w11, wzr, eq
	cmp	w0, w3
	bne	.L888
	.p2align 2
.L899:
	cbz	w11, .L872
.L869:
	add	w12, w12, 1
	cmp	w8, w12
	bhi	.L870
.L866:
	mov	w0, 1
	cbz	w9, .L887
	mov	w0, 0
	str	w10, [x20]
.L887:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L872:
	sxtw	x1, w12
	ldr	w4, [x7, 1840]
	ldr	w2, [x19, 200]
	add	x0, x17, x1, lsl 2
	ldr	w0, [x0, 72]
	add	w0, w4, w0
	cmp	w0, w2
	blt	.L869
	add	x1, x17, x1
	ldrb	w1, [x1, 53]
	cmp	w1, wzr
	csel	w10, w10, w0, eq
	csinc	w9, w9, wzr, eq
	b	.L869
	.p2align 3
.L864:
	add	x1, x18, x16, lsl 2
	ldr	w4, [x7, 1840]
	ldr	w2, [x19, 200]
	ldr	w0, [x1, 72]
	add	w0, w4, w0
	cmp	w0, w2
	blt	.L862
	add	x16, x18, x16
	ldrb	w1, [x16, 53]
	cmp	w1, wzr
	csel	w10, w10, w0, eq
	csinc	w9, w9, wzr, eq
	b	.L862
.L857:
	cbz	w8, .L887
	mov	w10, 0
	mov	w9, 0
	ldr	w3, [x19, 2716]
	b	.L865
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
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bls	.L901
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.4
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L901:
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
	add	x2, x2, 3704
	add	x5, x5, 1560
	mov	w0, 0
	.p2align 2
.L908:
	ldr	w1, [x2, 4]
	cbnz	w1, .L904
	cbz	x2, .L905
	ldr	w4, [x19, 2716]
	cbz	w4, .L905
	ldr	x1, [x19, 2736]
	cmp	x1, x2
	beq	.L904
	sub	w4, w4, #1
	mov	x1, x19
	add	x4, x19, x4, lsl 3
	b	.L906
	.p2align 3
.L907:
	ldr	x3, [x1, 2736]
	cmp	x3, x2
	beq	.L904
.L906:
	cmp	x4, x1
	add	x1, x1, 8
	bne	.L907
.L905:
	add	w0, w0, 1
.L904:
	add	x2, x2, 712
	cmp	x2, x5
	bne	.L908
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
	add	x24, x22, 964
	bl	_mcount
	mov	w20, 0
	ldr	w1, [x24, 16]
	cbz	w1, .L919
	add	x25, x21, 53248
	mov	w27, 1
	mov	w26, 1
	.p2align 2
.L926:
	uxtw	x0, w20
	add	x1, x24, x0
	ldrb	w1, [x1, 53]
	cbz	w1, .L920
	add	x0, x24, x0, lsl 2
	ldr	w5, [x21, 2716]
	ldr	w4, [x25, 1840]
	ldr	w0, [x0, 72]
	add	w4, w4, w0
	cbz	w5, .L920
	ldr	x1, [x21, 2736]
	ldr	w0, [x1, 12]
	cmp	w4, w0
	beq	.L976
	add	x2, x21, 2744
	mov	w0, 0
	b	.L923
	.p2align 3
.L925:
	ldr	x1, [x2], 8
	ldr	w3, [x1, 12]
	cmp	w4, w3
	beq	.L921
.L923:
	add	w0, w0, 1
	cmp	w0, w5
	bne	.L925
.L920:
	ldr	w1, [x24, 16]
	add	w20, w20, 1
	cmp	w1, w20
	bhi	.L926
.L919:
	ldr	w0, [x21, 2712]
	str	w23, [x21, 2728]
	cmp	w0, w23
	bls	.L931
	.p2align 2
.L959:
	add	x2, x21, x23, uxtw 3
	add	w23, w23, 1
	cmp	w23, w0
	str	xzr, [x2, 2872]
	bne	.L959
.L931:
	add	x23, x22, 964
	ldr	w25, [x23, 20]
	mov	w22, 0
	add	w25, w1, w25
	cmp	w25, w20
	bls	.L929
	add	x24, x21, 53248
	mov	w27, 1
	mov	w26, 1
	.p2align 2
.L958:
	uxtw	x0, w20
	add	x1, x23, x0
	ldrb	w1, [x1, 53]
	cbz	w1, .L932
	add	x0, x23, x0, lsl 2
	ldr	w5, [x21, 2716]
	ldr	w4, [x24, 1840]
	ldr	w0, [x0, 72]
	add	w4, w4, w0
	cbz	w5, .L932
	ldr	x1, [x21, 2736]
	ldr	w0, [x1, 12]
	cmp	w4, w0
	beq	.L977
	add	x2, x21, 2744
	mov	w0, 0
	b	.L935
	.p2align 3
.L937:
	ldr	x1, [x2], 8
	ldr	w3, [x1, 12]
	cmp	w4, w3
	beq	.L933
.L935:
	add	w0, w0, 1
	cmp	w0, w5
	bne	.L937
.L932:
	add	w20, w20, 1
	cmp	w25, w20
	bhi	.L958
.L980:
	ldr	w0, [x21, 2712]
.L929:
	cmp	w0, w22
	str	w22, [x21, 2732]
	bls	.L918
	.p2align 2
.L957:
	add	x1, x21, x22, uxtw 3
	add	w22, w22, 1
	cmp	w0, w22
	str	xzr, [x1, 3008]
	bne	.L957
.L918:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L976:
	mov	w0, 0
	.p2align 2
.L921:
	add	x19, x21, x0, uxtw 3
	strb	w27, [x1, 1]
	ldr	x0, [x19, 2736]
	ldrb	w1, [x0, 1]
	cbnz	w1, .L978
.L924:
	str	w26, [x0, 60]
	add	x0, x21, x23, uxtw 3
	add	w20, w20, 1
	add	w23, w23, 1
	ldr	x1, [x19, 2736]
	str	wzr, [x1, 56]
	ldr	x1, [x19, 2736]
	str	x1, [x0, 2872]
	ldr	w1, [x24, 16]
	cmp	w1, w20
	bhi	.L926
	b	.L919
.L977:
	mov	w0, 0
	.p2align 2
.L933:
	add	x19, x21, x0, uxtw 3
	strb	w27, [x1, 1]
	ldr	x0, [x19, 2736]
	ldrb	w1, [x0, 1]
	cbnz	w1, .L979
.L936:
	str	w26, [x0, 60]
	add	x0, x21, x22, uxtw 3
	add	w20, w20, 1
	add	w22, w22, 1
	ldr	x1, [x19, 2736]
	cmp	w25, w20
	str	wzr, [x1, 56]
	ldr	x1, [x19, 2736]
	str	x1, [x0, 3008]
	bhi	.L958
	b	.L980
	.p2align 3
.L978:
	ldr	w1, [x0, 32]
	mov	w2, 1
	ldr	w0, [x21, 276]
	bl	FSP_SetRef
	ldr	x0, [x19, 2736]
	b	.L924
	.p2align 3
.L979:
	ldr	w1, [x0, 32]
	mov	w2, 1
	ldr	w0, [x21, 276]
	bl	FSP_SetRef
	ldr	x0, [x19, 2736]
	b	.L936
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
	ldr	x0, [x19, 2368]
	add	x11, x19, 57344
	mov	w1, 2024
	add	x11, x11, 964
	ldr	x3, [x19, 2360]
	ldr	w2, [x2, 1868]
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
	ble	.L991
	mov	w9, 0
	sub	w7, w0, #1
	mov	w13, w12
	.p2align 2
.L988:
	sxtw	x0, w10
	add	x1, x11, x0
	ldrb	w1, [x1, 53]
	cbz	w1, .L983
	add	x0, x19, x0, lsl 2
	ldr	w8, [x19, 2716]
	add	x1, x0, 57344
	add	x0, x0, 53248
	ldr	w5, [x1, 1104]
	ldrb	w6, [x0, 3316]
	and	w0, w5, w7
	cmp	w6, wzr
	csel	w5, w0, w5, eq
	cmp	w8, wzr
	ble	.L983
	add	x2, x19, 2736
	mov	w1, 0
	b	.L987
	.p2align 3
.L986:
	add	w1, w1, 1
	cmp	w1, w8
	beq	.L983
.L987:
	ldr	x3, [x2]
	cmp	w6, wzr
	add	x2, x2, 8
	ldr	w0, [x3, 12]
	and	w4, w0, w7
	csel	w0, w4, w0, eq
	cmp	w5, w0
	bne	.L986
	add	x1, x19, x1, sxtw 3
	strb	w13, [x3, 1]
	add	x0, x19, x9, sxtw 3
	add	w9, w9, 1
	ldr	x2, [x1, 2736]
	str	wzr, [x2, 60]
	ldr	x2, [x1, 2736]
	str	w12, [x2, 56]
	ldr	x1, [x1, 2736]
	str	x1, [x0, 3144]
.L983:
	ldp	w1, w0, [x11, 16]
	sub	w10, w10, #1
	add	w0, w1, w0
	sub	w0, w0, #1
	cmp	w0, w10
	blt	.L988
	mov	w0, w9
.L982:
	ldr	w1, [x19, 2712]
	str	w0, [x19, 2724]
	cmp	w1, w9
	ble	.L981
	.p2align 2
.L994:
	add	x0, x19, x9, sxtw 3
	add	w9, w9, 1
	cmp	w9, w1
	str	xzr, [x0, 3144]
	bne	.L994
.L981:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L991:
	mov	w0, 0
	mov	w9, 0
	b	.L982
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
	cbz	x21, .L1000
	ldr	w2, [x20, 3280]
	cbz	w2, .L1009
	ldr	w0, [x20, 3288]
	cbz	w0, .L1015
	add	x0, x20, 3292
	mov	w19, 0
	b	.L1004
	.p2align 3
.L1007:
	ldr	w1, [x0], 4
	cbz	w1, .L1002
.L1004:
	add	w19, w19, 1
	cmp	w19, w2
	bne	.L1007
.L1009:
	mov	w0, -7
.L1013:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1015:
	mov	w19, 0
	.p2align 2
.L1002:
	ldr	w0, [x20, 276]
	ldr	w1, [x21, 32]
	bl	FSP_GetLogicFs
	mov	x5, x0
	cbz	x0, .L1005
	ldr	x0, [x0, 640]
	cbz	x0, .L1005
	add	x2, x20, x19, uxtw 2
	mov	w4, 1
	ldr	x1, .LC199
	mov	w3, w19
	mov	w0, 13
	str	w4, [x2, 3288]
	ldr	x4, [x5, 640]
	ldr	w4, [x4, 8]
	str	w4, [x2, 3352]
	ldr	w4, [x21, 12]
	str	w4, [x2, 3416]
	ldr	w2, [x20, 3284]
	add	w2, w2, 1
	str	w2, [x20, 3284]
	ldr	w2, [x21, 236]
	bl	dprint_vfmw
	str	w19, [x21, 28]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1005:
	ldrb	w4, [x21, 1]
	mov	x6, 0
	ldr	x1, .LC201
	mov	w0, 0
	ldr	w3, [x21, 12]
	ldr	w2, [x21, 32]
	bl	dprint_vfmw
	mov	w0, -7
	b	.L1013
.L1000:
	ldr	x2, .LC196
	mov	w3, 9276
	ldr	x1, .LC197
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -7
	b	.L1013
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
	ldr	w0, [x20, 276]
	bl	FSP_GetLogicFs
	cbz	x0, .L1017
	ldrsb	w4, [x0, 1]
	cmp	w4, 3
	ccmp	w4, wzr, 4, ne
	beq	.L1037
	ldr	w0, [x19, 8]
	cmp	w0, 1
	beq	.L1023
	ldr	w4, [x19, 4]
	ldrb	w5, [x19]
	cbz	w4, .L1022
	cmp	w5, 1
	mov	w0, 0
	beq	.L1022
.L1031:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1037:
	ldr	x2, .LC202
	mov	w0, 13
	ldr	x1, .LC204
	ldr	w3, [x19, 32]
	add	x2, x2, 352
	bl	dprint_vfmw
.L1019:
	ldr	w0, [x19, 8]
	cmp	w0, 1
	bne	.L1038
.L1023:
	mov	w1, 1
	mov	w0, 2
	strb	w1, [x19, 2]
	str	w0, [x19, 8]
	mov	w0, -3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1017:
	ldr	x1, .LC206
	mov	x2, x19
	ldp	w4, w3, [x19, 8]
	ldr	w5, [x19, 32]
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1019
.L1038:
	ldr	x2, .LC202
	mov	w0, 1
	ldr	x1, .LC208
	ldr	w3, [x19, 12]
	add	x2, x2, 352
	bl	dprint_vfmw
	mov	w0, -3
	b	.L1031
	.p2align 3
.L1022:
	ldr	x2, .LC202
	mov	x3, x19
	ldr	x1, .LC210
	mov	w0, 1
	add	x2, x2, 352
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1031
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
	ldr	x0, [x20, 28160]
	ldr	w2, [x0, 12468]
	and	w2, w2, 16776960
	cmp	w2, 256
	beq	.L1049
	mov	w0, 256
	movk	w0, 0x1, lsl 16
	cmp	w2, w0
	bne	.L1050
	ldr	w0, [x20, 5288]
	cmp	w0, 1
	cset	w2, ne
.L1040:
	ldr	x4, [x20, 28168]
	mov	w0, 3072
	ldrb	w4, [x4, 2633]
	cbz	w4, .L1041
	ldr	w2, [x19, 704]
	mov	w0, 1024
	cmp	w2, 1
	ccmp	w2, 11, 4, ne
	cset	w4, eq
	cbz	w4, .L1064
.L1042:
	ldr	w2, [x19, 708]
	cmp	w2, wzr
	cset	w2, eq
.L1041:
	and	w3, w3, 3
	orr	w0, w0, w2, lsl 8
	orr	w1, w3, w1
	add	x2, x20, 53248
	orr	w0, w0, w1
	orr	w1, w0, 8192
	str	w1, [x19, 188]
	ldr	w1, [x2, 1120]
	cmp	w1, 1920
	bhi	.L1043
	mov	w1, 8192
	movk	w1, 0x2, lsl 16
	orr	w0, w0, w1
	str	w0, [x19, 188]
	mov	w1, 0
	ldr	x2, [x20, 2376]
	ldr	w2, [x2, 632]
	cmp	w2, 1
	beq	.L1065
.L1045:
	ldr	w2, [x19, 4]
	orr	w0, w0, w1
	str	w0, [x19, 188]
	cbz	w2, .L1046
	ldr	w0, [x19, 20]
	str	w0, [x19, 240]
.L1046:
	ldr	w0, [x20, 276]
	ldr	w1, [x19, 32]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L1039
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
.L1039:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1043:
	and	w0, w0, -131073
	mov	w1, 0
	orr	w0, w0, 8192
	str	w0, [x19, 188]
	ldr	x2, [x20, 2376]
	ldr	w2, [x2, 632]
	cmp	w2, 1
	bne	.L1045
.L1065:
	and	w2, w0, 131072
	mov	w1, 262144
	cmp	w2, wzr
	csel	w1, wzr, w1, ne
	b	.L1045
	.p2align 3
.L1064:
	and	w2, w2, -9
	mov	w0, 3072
	mov	w4, 2048
	cmp	w2, 2
	csel	w0, w0, w4, ne
	b	.L1042
	.p2align 3
.L1049:
	mov	w2, 0
	b	.L1040
	.p2align 3
.L1050:
	mov	w2, 1
	b	.L1040
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
	add	x23, x22, 1104
	bl	_mcount
	mov	x2, 624
	add	x1, x22, 1168
	ldr	x20, [x22, 1160]
	ldr	w3, [x23, 40]
	add	x0, x20, 80
	str	w3, [x20, 20]
	bl	memcpy
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_GetImagePara
	ldr	w0, [x19, 276]
	ldr	w1, [x20, 32]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L1088
	ldr	w0, [x20, 20]
	str	w0, [x21, 160]
	cbnz	w0, .L1069
.L1074:
	ldrb	w0, [x22, 1104]
	cbnz	w0, .L1089
.L1071:
	ldr	w0, [x19, 276]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	add	x3, x19, 61440
	ldr	w0, [x19, 276]
	mov	x4, x21
	add	x3, x3, 1064
	mov	x2, x19
	mov	w1, 16
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L1090
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L1091
	str	wzr, [x21, 24]
.L1078:
	ldr	w1, [x19, 308]
	mov	w0, 0
	ldr	w2, [x19, 316]
	add	w1, w1, 1
	str	w1, [x19, 308]
	add	w2, w2, 1
	str	w2, [x19, 316]
.L1068:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1069:
	ldr	w1, [x23, 12]
	cbz	w1, .L1092
.L1072:
	ldr	x1, [x19, 2376]
	ldr	w1, [x1, 4]
	cmp	w1, w0
	bcs	.L1074
	ldr	w0, [x19, 276]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	ldr	x4, [x19, 2376]
	mov	w0, 1
	ldr	x2, .LC212
	ldr	x1, .LC221
	ldr	w3, [x21, 160]
	add	x2, x2, 376
	ldr	w4, [x4, 4]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1068
	.p2align 3
.L1089:
	ldr	w0, [x19, 276]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetRef
	b	.L1071
	.p2align 3
.L1091:
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.4
	b	.L1078
	.p2align 3
.L1092:
	ldr	x2, .LC212
	mov	w0, 1
	ldr	x1, .LC216
	add	x2, x2, 376
	bl	dprint_vfmw
	ldr	x0, .LC217
	ldr	x4, [x0]
	cbz	x4, .L1073
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 111
	blr	x4
.L1073:
	ldr	w0, [x21, 160]
	cbnz	w0, .L1072
	b	.L1074
	.p2align 3
.L1090:
	ldr	x2, .LC212
	mov	w0, 1
	ldr	x1, .LC219
	add	x2, x2, 376
	bl	dprint_vfmw
	ldr	w0, [x19, 276]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	mov	w0, -1
	b	.L1068
.L1088:
	ldr	x2, .LC212
	mov	w0, 0
	ldr	x1, .LC214
	add	x2, x2, 376
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1068
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
	cbnz	w0, .L1096
	add	x0, x19, 53248
	add	x2, x19, 61440
	mov	w4, 2
	mov	w1, 1
	ldr	x3, [x0, 1160]
	str	w4, [x3, 8]
	str	w1, [x2, 1672]
	ldr	w2, [x0, 1112]
	str	w2, [x19, 244]
	ldr	x0, [x0, 1160]
	strb	w1, [x0, 2]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1096:
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
	bhi	.L1099
	uxtw	x1, w19
	lsl	x0, x1, 2
	sub	x0, x0, x1
	lsl	x2, x0, 4
	sub	x0, x2, x0
	lsl	x0, x0, 1
	sub	x0, x0, x1
	add	x0, x20, x0, lsl 3
	add	x0, x0, 65536
	ldrb	w0, [x0, 3705]
	cmp	w0, wzr
	cset	w0, ne
.L1098:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1099:
	mov	w0, -1
	b	.L1098
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
	ldr	w0, [x21, 276]
	mov	w1, w22
	bl	FSP_GetLogicFs
	cbz	x0, .L1105
	ldrsb	w1, [x0, 1]
	add	x19, x0, 568
	add	x20, x0, 600
	cbz	w1, .L1111
.L1108:
	ldr	x1, [x19]
	cbnz	x1, .L1112
.L1103:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L1108
	ldr	w0, [x21, 276]
	mov	w2, 0
	mov	w1, w22
	bl	FSP_SetDisplay
	mov	w0, 0
.L1101:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1112:
	ldr	w0, [x21, 276]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L1103
.L1111:
	ldr	x1, .LC226
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1101
.L1105:
	mov	w0, -1
	b	.L1101
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
	ldr	x0, [x19, 2376]
	stp	w20, w21, [x29, 88]
	strb	wzr, [x29, 65]
	ldr	w0, [x0, 28]
	stp	w20, w21, [x29, 72]
	stp	w20, w21, [x29, 80]
	cmp	w0, 24
	beq	.L1114
	ldr	w1, [x19, 2712]
	mov	w0, 5
	strb	wzr, [x29, 68]
	sxtb	w6, w22
	strb	w0, [x29, 69]
	strb	w1, [x29, 67]
.L1119:
	ldr	w0, [x19, 276]
	add	x1, x29, 64
	strb	w6, [x29, 70]
	strb	wzr, [x29, 66]
	bl	FSP_ConfigInstance
	cbnz	w0, .L1115
	ldr	x1, [x19, 2376]
	add	x3, x29, 60
	ldr	w0, [x19, 276]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	mov	w1, w0
	beq	.L1123
	mov	w0, 1
	cbnz	w1, .L1117
.L1121:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1114:
	mov	w1, 1
	mov	w0, 5
	strb	w1, [x29, 67]
	mov	w6, 1
	strb	wzr, [x29, 68]
	strb	w0, [x29, 69]
	b	.L1119
	.p2align 3
.L1123:
	add	x0, x19, 61440
	add	x0, x0, 1064
	bl	ResetVoQueue
.L1117:
	ldr	x1, .LC229
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -20
	b	.L1121
	.p2align 3
.L1115:
	ldr	x1, .LC231
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -20
	b	.L1121
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
.L1125:
	ldr	w1, [x0]
	add	x0, x0, 4
	strh	w1, [x2]
	add	x2, x2, 2
	cmp	x0, x3
	ldr	w1, [x0, 8]
	strh	w1, [x2, 4]
	bne	.L1125
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
	beq	.L1128
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1128:
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
	add	x19, x19, 3704
	ldr	x0, [x23, 2368]
	mov	w5, 2024
	ldr	w1, [x22, 1868]
	mov	w4, 26164
	ldr	x2, [x23, 2360]
	mov	x3, 0
	smaddl	x0, w1, w5, x0
	ldr	w21, [x0, 32]
	strb	wzr, [x22, 1105]
	smaddl	x21, w21, w4, x2
	.p2align 2
.L1135:
	ldr	w0, [x19, 4]
	cbnz	w0, .L1130
	cbz	x19, .L1131
	ldr	w2, [x23, 2716]
	cbz	w2, .L1131
	ldr	x0, [x23, 2736]
	cmp	x0, x19
	beq	.L1130
	sub	w2, w2, #1
	mov	x0, x23
	add	x2, x23, x2, lsl 3
	b	.L1133
	.p2align 3
.L1134:
	ldr	x1, [x0, 2736]
	cmp	x1, x19
	beq	.L1130
.L1133:
	cmp	x0, x2
	add	x0, x0, 8
	bne	.L1134
.L1131:
	lsl	x20, x3, 2
	mov	w0, 101
	sub	x20, x20, x3
	str	w0, [x22, 1144]
	lsl	x0, x20, 4
	mov	w1, 1
	sub	x20, x0, x20
	strb	w1, [x22, 1105]
	lsl	x20, x20, x1
	mov	x2, 624
	sub	x3, x20, x3
	add	x0, x22, 1168
	add	x20, x23, x3, lsl 3
	add	x20, x20, 65536
	add	x1, x20, 3784
	bl	memcpy
	str	x19, [x22, 1160]
	str	wzr, [x22, 1328]
	strb	wzr, [x20, 3706]
	str	wzr, [x20, 3724]
	strb	wzr, [x19]
	ldr	x0, [x22, 1160]
	str	wzr, [x0, 56]
	ldr	x0, [x22, 1160]
	str	wzr, [x0, 60]
	ldr	x0, [x22, 1160]
	str	wzr, [x0, 8]
	ldrb	w1, [x22, 1105]
	cmp	w1, 1
	bne	.L1139
	ldr	w0, [x23, 276]
	bl	FSP_NewLogicFs
	mov	w24, w0
	mov	w1, w0
	ldr	w0, [x23, 276]
	bl	FSP_GetLogicFs
	mov	x20, x0
	cmp	w24, wzr
	ccmp	x0, xzr, 4, ge
	beq	.L1137
	add	x2, x23, 4096
	add	x1, x21, 2624
	add	x2, x2, 96
	add	x19, x0, 8
	bl	HEVC_ConvertHDRInfo
	ldr	x0, [x23, 2376]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L1152
.L1138:
	ldr	x21, .LC237
	mov	x2, 624
	ldr	x0, [x22, 1160]
	mov	x1, x19
	ldr	x3, [x21, 104]
	add	x0, x0, 80
	blr	x3
	ldr	x1, [x22, 1160]
	mov	x2, 624
	ldr	x3, [x21, 104]
	add	x0, x22, 1168
	add	x1, x1, 80
	blr	x3
	ldr	x1, [x22, 1160]
	mov	w0, 0
	str	w24, [x1, 32]
	ldrsb	w1, [x20, 4]
	str	w1, [x22, 1140]
.L1150:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1130:
	add	w3, w3, 1
	add	x19, x19, 712
	cmp	w3, 20
	bne	.L1135
.L1139:
	ldr	x1, .LC236
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -4
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L1152:
	ldr	x2, [x23, 28168]
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
	b	.L1138
.L1137:
	ldr	x1, .LC239
	mov	w0, 0
	strb	wzr, [x22, 1105]
	bl	dprint_vfmw
	mov	w0, -4
	b	.L1150
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
	ldr	w0, [x20, 276]
	bl	FSP_GetFspType
	cmn	w0, #1
	mov	w21, w0
	bne	.L1159
	b	.L1161
	.p2align 3
.L1156:
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	beq	.L1158
	cmp	w19, 32
	beq	.L1162
.L1159:
	ldr	w0, [x20, 276]
	mov	w1, w19
	add	w19, w19, 1
	bl	FSP_GetLogicFs
	cbnz	x0, .L1156
	ldr	x1, .LC243
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1161:
	ldr	x1, .LC241
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w21
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1158:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1162:
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
	ldr	w0, [x22, 276]
	bl	FSP_GetFspType
	cbz	w0, .L1177
.L1164:
	add	x20, x22, 65536
	add	x23, x22, 81920
	add	x20, x20, 3705
	add	x23, x23, 1561
	.p2align 2
.L1167:
	strb	wzr, [x20]
	add	x19, x20, 639
	add	x21, x20, 671
	str	wzr, [x20, 3]
.L1166:
	ldr	x1, [x19]
	cbnz	x1, .L1178
.L1165:
	add	x19, x19, 8
	cmp	x19, x21
	bne	.L1166
	add	x20, x20, 712
	cmp	x20, x23
	bne	.L1167
	ldr	w0, [x22, 180]
	cbz	w0, .L1163
	sub	w1, w0, #1
	add	x0, x22, 2148
	add	x1, x1, 538
	add	x22, x22, x1, lsl 2
	.p2align 2
.L1169:
	str	wzr, [x0], 4
	cmp	x0, x22
	bne	.L1169
.L1163:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1178:
	ldr	w0, [x22, 276]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L1165
.L1177:
	add	x0, x22, 61440
	add	x0, x0, 1064
	bl	ResetVoQueue
	b	.L1164
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
	ldr	x0, [x22, 2736]
	str	wzr, [x0, 4]
	ldr	x1, [x22, 2736]
	ldr	w0, [x20, 276]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	cbz	x0, .L1180
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	beq	.L1181
	ldr	x1, [x22, 2736]
	mov	w2, 0
	ldr	w0, [x20, 276]
	ldr	w1, [x1, 32]
	bl	FSP_SetDisplay
.L1181:
	ldr	x1, [x22, 2736]
	mov	w2, 0
	ldr	w0, [x20, 276]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
.L1180:
	ldr	w2, [x20, 2716]
	sub	w2, w2, #1
	cmp	w19, w2
	bhi	.L1184
	.p2align 2
.L1190:
	add	w0, w19, 1
	add	x1, x20, x21, lsl 3
	mov	w19, w0
	cmp	w0, w2
	mov	x21, x0
	add	x0, x20, x0, lsl 3
	ldr	x0, [x0, 2736]
	str	x0, [x1, 2736]
	bls	.L1190
.L1184:
	add	x0, x20, x2, uxtw 3
	str	xzr, [x0, 2736]
	str	w2, [x20, 2716]
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
	ldr	w2, [x20, 2716]
	cbnz	w2, .L1201
	b	.L1194
	.p2align 3
.L1196:
	add	w19, w19, 1
	cmp	w2, w19
	mov	w21, w19
	bls	.L1194
.L1201:
	add	x0, x20, x19, sxtw 3
	ldr	x0, [x0, 2736]
	ldrb	w1, [x0]
	cbz	w1, .L1196
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
	ldr	w2, [x20, 2716]
	cmp	w2, w19
	bhi	.L1201
.L1194:
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
	ldr	w22, [x21, 2716]
	cbz	w22, .L1205
	mov	w26, 0
	mov	w20, 0
	mov	w24, -1
	mov	w23, 1
	.p2align 2
.L1212:
	add	x19, x21, x26, uxtw 3
	ldr	w0, [x21, 276]
	ldr	x1, [x19, 2736]
	ldr	w1, [x1, 32]
	bl	FSP_GetLogicFs
	cbz	x0, .L1206
	ldrsb	w1, [x0]
	cbnz	w1, .L1221
	ldrsb	w0, [x0, 1]
	cmp	w0, wzr
	ccmp	w0, 3, 4, ne
	ldr	x0, [x19, 2736]
	bne	.L1208
.L1224:
	ldr	w1, [x0, 8]
	cmp	w1, 1
	beq	.L1208
	str	w24, [x0, 32]
	ldr	x0, [x19, 2736]
	strb	wzr, [x0, 1]
	ldr	x0, [x19, 2736]
	strb	w23, [x0, 2]
.L1221:
	ldr	x0, [x19, 2736]
.L1208:
	ldr	w0, [x0]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L1225
	add	w26, w26, 1
.L1211:
	add	w20, w20, 1
	cmp	w22, w20
	bne	.L1212
.L1205:
	mov	w0, w25
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1206:
	ldr	x0, [x19, 2736]
	b	.L1224
	.p2align 3
.L1225:
	mov	w1, w26
	mov	x0, x21
	mov	w25, 1
	bl	HEVC_RemoveFrameStoreOutDPB
	b	.L1211
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
	ldr	w0, [x19, 2712]
	ldr	w1, [x19, 2716]
	cmp	w1, w0
	bcc	.L1226
	cmp	w0, 15
	bls	.L1229
.L1230:
	ldr	x0, [x19, 2736]
	mov	w2, 0
	strb	wzr, [x0, 1]
	ldr	w1, [x0, 32]
	ldr	w0, [x19, 276]
	bl	FSP_SetRef
	mov	x0, x19
	mov	w1, 0
	bl	HEVC_RemoveApcStore
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
.L1226:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1229:
	mov	x0, x19
	bl	HEVC_IncreaseDPBSize
	cbnz	w0, .L1230
	b	.L1226
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
	ldr	w3, [x19, 2712]
	ldr	w2, [x19, 2716]
	cmp	w2, w3
	bhi	.L1240
.L1233:
	mov	w0, 0
	cbz	w2, .L1232
	.p2align 2
.L1238:
	add	x1, x19, x0, uxtw 3
	ldr	w2, [x20]
	ldr	x1, [x1, 2736]
	ldr	w1, [x1, 12]
	cmp	w2, w1
	ble	.L1235
	str	w1, [x20]
	str	w0, [x21]
.L1235:
	ldr	w1, [x19, 2716]
	add	w0, w0, 1
	cmp	w1, w0
	bhi	.L1238
.L1232:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1240:
	ldr	x1, .LC245
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w2, [x19, 2712]
	str	w2, [x19, 2716]
	b	.L1233
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
	ldr	x2, [x19, 2368]
	mov	w3, 2024
	ldr	x0, [x19, 2360]
	mov	w1, 26164
	ldr	w5, [x4, 1868]
	smaddl	x3, w5, w3, x2
	ldr	w3, [x3, 32]
	smaddl	x0, w3, w1, x0
	ldr	w1, [x0, 72]
	cmp	w1, w21
	ccmp	w20, wzr, 0, eq
	bne	.L1243
	ldr	w1, [x0, 76]
	mov	w0, 1
	cmp	w1, w22
	beq	.L1247
.L1242:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1243:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1247:
	ldr	w1, [x19, 168]
	cbz	w1, .L1242
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 928]
	cmp	w0, wzr
	cset	w0, ne
	b	.L1242
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
	ldr	x5, [x19, 2392]
	ldr	x0, [x19, 2344]
	ldrb	w4, [x5, 1]
	ldr	w6, [x0, 32]
	cbz	w4, .L1259
	sub	w2, w4, #1
	mov	w7, 32
	mov	x3, 88
	add	x0, x5, 56
	mov	w1, 0
	umaddl	x3, w2, w7, x3
	add	x3, x5, x3
	.p2align 2
.L1250:
	ldr	w2, [x0], 32
	cmp	x0, x3
	add	w1, w1, w2
	bne	.L1250
.L1249:
	add	x4, x5, x4, lsl 5
	and	w0, w6, 7
	str	w0, [x4, 68]
	ldr	x3, [x19, 2392]
	ldr	x0, [x19, 2344]
	ldrb	w2, [x3, 1]
	ldr	w0, [x0, 32]
	add	x4, x3, x2, lsl 5
	ldr	w3, [x4, 60]
	add	w0, w3, w0, lsr 3
	sub	w0, w0, w1
	str	w0, [x4, 72]
	ldr	x3, [x19, 2392]
	ldr	x0, [x19, 2344]
	ldrb	w2, [x3, 1]
	ldr	w0, [x0, 32]
	add	x4, x3, x2, lsl 5
	ldr	w2, [x4, 56]
	add	w1, w1, w2
	lsl	w1, w1, 3
	sub	w0, w1, w0
	str	w0, [x4, 64]
	ldr	x1, [x19, 2392]
	ldrb	w0, [x1, 1]
	add	x0, x1, x0, lsl 5
	ldp	w2, w1, [x0, 68]
	ubfiz	w1, w1, 3, 2
	add	w1, w2, w1
	str	w1, [x0, 68]
	ldr	x1, [x19, 2392]
	ldrb	w0, [x1, 1]
	add	x0, x1, x0, lsl 5
	ldr	w1, [x0, 72]
	and	w1, w1, -4
	str	w1, [x0, 72]
	ldr	x3, [x19, 2392]
	ldr	w2, [x3, 8]
	sub	w0, w2, #1
	cmp	w0, 1
	bhi	.L1267
	ldrb	w4, [x3, 1]
	add	w1, w4, 1
	cmp	w1, 1
	ccmp	w2, w1, 0, ls
	bls	.L1253
	add	w4, w4, 2
	.p2align 2
.L1254:
	sub	w0, w4, #1
	cmp	w4, 1
	add	x1, x0, 2
	lsl	x0, x0, 5
	lsl	x1, x1, 5
	cset	w5, ls
	add	x3, x3, x1
	str	wzr, [x3, 4]
	ldr	x2, [x19, 2392]
	add	x3, x2, x0
	add	x2, x2, x1
	ldr	w3, [x3, 60]
	str	w3, [x2, 8]
	ldr	x3, [x19, 2392]
	add	x2, x3, x0
	ldr	w2, [x2, 56]
	lsl	w2, w2, 3
	str	w2, [x3, x1]
	ldr	x1, [x19, 2392]
	add	x1, x1, x0
	ldp	w3, w2, [x1, 68]
	ubfiz	w2, w2, 3, 4
	add	w2, w3, w2
	str	w2, [x1, 68]
	ldr	x1, [x19, 2392]
	add	x0, x1, x0
	ldr	w1, [x0, 72]
	and	w1, w1, -16
	str	w1, [x0, 72]
	ldr	x3, [x19, 2392]
	ldr	w0, [x3, 8]
	cmp	w0, w4
	add	w4, w4, 1
	cset	w1, hi
	tst	w5, w1
	bne	.L1254
	sub	w0, w0, #1
.L1253:
	ubfiz	x0, x0, 5, 32
	ldr	w1, [x3, 12]
	add	x3, x3, x0
	add	x5, x19, 53248
	ldr	w0, [x3, 64]
	sub	w0, w0, w1
	str	w0, [x3, 64]
	ldr	x3, [x19, 2392]
	ldrb	w1, [x3, 1]
	ldr	w4, [x3, 8]
	cmp	w1, w4
	bcs	.L1248
	.p2align 2
.L1262:
	uxtw	x0, w1
	ldr	w2, [x5, 1136]
	add	w1, w1, 1
	add	x0, x3, x0, lsl 5
	ldr	w0, [x0, 72]
	cmp	w0, w2
	bcs	.L1256
	str	w0, [x5, 1136]
	ldr	w4, [x3, 8]
.L1256:
	cmp	w4, w1
	bhi	.L1262
.L1248:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1267:
	ldr	x1, .LC247
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1259:
	mov	w1, 0
	b	.L1249
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
	ldr	w2, [x1, 1868]
	cmp	w2, w0
	bge	.L1276
	mov	w0, 2024
	ldr	x1, [x19, 2368]
	smull	x0, w2, w0
	add	x3, x1, x0
	ldrb	w0, [x1, x0]
	cbz	w0, .L1277
	ldr	w2, [x3, 32]
	ldr	w0, [x19, 152]
	cmp	w2, w0
	bge	.L1278
	ldr	x1, [x19, 2360]
	mov	w0, 26164
	smaddl	x0, w2, w0, x1
	ldrb	w1, [x0, 1]
	cbz	w1, .L1279
	ldr	w2, [x0, 52]
	ldr	w0, [x19, 148]
	cmp	w2, w0
	bcs	.L1280
	mov	w1, 23196
	ldr	x3, [x19, 2352]
	mov	w0, 0
	umull	x1, w2, w1
	ldrb	w1, [x3, x1]
	cbz	w1, .L1281
.L1270:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1276:
	ldr	x1, .LC249
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1270
	.p2align 3
.L1277:
	ldr	x1, .LC251
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1270
	.p2align 3
.L1278:
	ldr	x1, .LC253
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1270
	.p2align 3
.L1279:
	ldr	x1, .LC255
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1270
	.p2align 3
.L1280:
	ldr	x1, .LC257
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1270
	.p2align 3
.L1281:
	ldr	x1, .LC259
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1270
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
	ldr	x1, [x19, 2392]
	mov	w0, 2147483647
	ldr	w5, [x19, 200]
	cmp	w5, w0
	ldr	w0, [x1, 16]
	sub	w2, w0, #16
	beq	.L1309
	cmp	w2, 2
	ccmp	w0, 21, 4, hi
	beq	.L1287
.L1295:
	sub	w0, w0, #19
	cmp	w0, 1
	bls	.L1310
.L1287:
	add	x0, x19, 53248
	ldr	w4, [x0, 1840]
	cmp	w4, w5
	bge	.L1286
	ldr	w6, [x1, 16]
	sub	w2, w6, #8
	cmp	w2, 1
	bls	.L1311
.L1288:
	ldrb	w3, [x19, 8]
	cbz	w3, .L1289
	ldr	w6, [x19, 204]
	cmp	w6, w4
	bgt	.L1312
.L1289:
	ldrb	w2, [x0, 1795]
	cbnz	w2, .L1290
	ldr	w1, [x1, 16]
	sub	w2, w1, #19
	cmp	w2, 1
	bls	.L1307
	cmp	w1, 21
	beq	.L1307
	sub	w1, w1, #16
	cmp	w1, 2
	bhi	.L1290
	mov	w1, 1
	str	w4, [x19, 204]
	strb	w1, [x19, 8]
	.p2align 2
.L1290:
	ldr	w1, [x19, 248]
	ldrb	w3, [x0, 1793]
	cbnz	w1, .L1293
	ldr	w2, [x0, 1868]
	mov	w0, 1
	ldr	w1, [x19, 320]
	cmp	w1, w2
	beq	.L1313
.L1304:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1309:
	cmp	w2, 5
	bhi	.L1308
	cmp	w2, 2
	ccmp	w0, 21, 4, hi
	bne	.L1295
	add	x0, x19, 53248
	ldr	w4, [x0, 1840]
	str	w4, [x19, 200]
.L1286:
	ldrb	w5, [x19, 3]
	cbz	w5, .L1288
	ldr	w6, [x1, 16]
	sub	w2, w6, #8
	cmp	w2, 1
	bhi	.L1288
	ldr	x2, .LC260
	mov	w3, 10448
	ldr	x1, .LC264
	mov	w0, 1
	add	x2, x2, 424
	bl	dprint_vfmw
	mov	w0, 2
	b	.L1304
	.p2align 3
.L1293:
	sub	w1, w1, #1
	ldr	w2, [x0, 1868]
	add	x1, x1, 16384
	add	x1, x1, 724
	ldr	w0, [x19, x1, lsl 2]
	ldr	w1, [x19, 320]
	cmp	w2, w0
	mov	w0, 1
	csinc	w3, w3, wzr, eq
	cmp	w1, w2
	bne	.L1304
.L1313:
	ldr	w0, [x19, 284]
	ldr	x19, [sp, 16]
	cmp	w0, w4
	csinc	w0, w3, wzr, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1312:
	ldr	w7, [x1, 16]
	sub	w2, w7, #8
	cmp	w2, 1
	bhi	.L1289
	ldr	x2, .LC260
	mov	w5, w4
	ldr	x1, .LC266
	mov	w4, w3
	add	x2, x2, 424
	mov	w3, 10458
	mov	w0, 1
	bl	dprint_vfmw
.L1308:
	mov	w0, 2
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1307:
	strb	wzr, [x19, 8]
	str	w4, [x19, 204]
	b	.L1290
	.p2align 3
.L1310:
	mov	w5, -2147483647
	str	w5, [x19, 200]
	b	.L1287
	.p2align 3
.L1311:
	ldr	x2, .LC260
	mov	w3, 10440
	ldr	x1, .LC262
	mov	w0, 1
	add	x2, x2, 424
	bl	dprint_vfmw
	mov	w0, 2
	b	.L1304
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
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	cbz	w0, .L1319
	ldr	x0, [x19, 2344]
	mov	w20, 0
	bl	BsBitsToNextByte
	mov	w1, w0
	cbz	w0, .L1316
	ldr	x0, [x19, 2344]
	bl	BsGet
	mov	w2, w0
	ldr	x1, .LC270
	mov	w0, 19
	bl	dprint_vfmw
.L1316:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1319:
	ldr	x1, .LC268
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	b	.L1316
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
	ldr	w0, [x19, 1864]
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
	ldr	w0, [x19, 1864]
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
	ldr	x0, [x20, 2344]
	cbnz	w23, .L1360
	mov	w1, 8
	bl	BsGet
	str	w0, [x19, 284]
	cmp	w24, wzr
	ble	.L1325
.L1338:
	sub	w25, w24, #1
	add	x22, x19, 43
	add	x25, x25, 44
	mov	x21, x22
	add	x25, x19, x25
	.p2align 2
.L1327:
	cbnz	w23, .L1361
.L1326:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21], 1
	cmp	x21, x25
	bne	.L1327
	cmp	w24, 7
	mov	w21, w24
	ble	.L1347
	b	.L1337
	.p2align 3
.L1330:
	cmp	w21, 8
	beq	.L1337
.L1347:
	ldr	x0, [x20, 2344]
	mov	w1, 2
	add	w21, w21, 1
	bl	BsGet
	cbz	w0, .L1330
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
.L1361:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21, -6]
	b	.L1326
.L1360:
	mov	w1, 2
	mov	x21, 0
	bl	BsGet
	str	w0, [x19, 276]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19]
	mov	w1, 5
	ldr	x0, [x20, 2344]
	bl	BsGet
	ldr	x25, .LC271
	ldr	x22, .LC273
	str	w0, [x19, 272]
	.p2align 2
.L1324:
	ldr	x4, [x25, 136]
	mov	w3, w21
	mov	x2, x22
	mov	x1, 100
	add	x0, x29, 88
	blr	x4
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	add	x1, x19, x21
	add	x21, x21, 1
	cmp	x21, 32
	strb	w0, [x1, 1]
	bne	.L1324
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 33]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 34]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 35]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 36]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	ldr	x0, [x20, 2344]
	mov	w1, 16
	bl	BsGet
	ldr	x0, [x20, 2344]
	mov	w1, 12
	bl	BsGet
	add	x0, x20, 86016
	ldr	w2, [x19, 272]
	mov	w1, 8
	str	w2, [x0, 936]
	ldrb	w2, [x19, 2]
	str	w2, [x0, 940]
	ldrb	w2, [x19, 3]
	str	w2, [x0, 944]
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 284]
	cmp	w24, wzr
	bgt	.L1338
.L1325:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	mov	w0, 0
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L1337:
	add	x26, x19, 360
	mov	w25, 0
	add	x19, x19, 49
	b	.L1335
	.p2align 3
.L1334:
	add	w25, w25, 1
	add	x22, x22, 1
	cmp	w24, w25
	add	x26, x26, 4
	mov	x19, x21
	ble	.L1325
.L1335:
	add	x21, x19, 32
	cbz	w23, .L1332
	ldrb	w0, [x22, -6]
	cbnz	w0, .L1362
.L1332:
	ldrb	w0, [x22]
	cbz	w0, .L1334
	ldr	x0, [x20, 2344]
	mov	w1, 8
	bl	BsGet
	str	w0, [x26]
	b	.L1334
	.p2align 3
.L1362:
	ldr	x0, [x20, 2344]
	mov	w1, 2
	bl	BsGet
	str	w0, [x26, -72]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x22, 198]
	mov	w1, 5
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x26, -48]
	.p2align 2
.L1333:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19], 1
	cmp	x19, x21
	bne	.L1333
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x22, 204]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x22, 210]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x22, 216]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x22, 222]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	ldr	x0, [x20, 2344]
	mov	w1, 16
	bl	BsGet
	ldr	x0, [x20, 2344]
	mov	w1, 12
	bl	BsGet
	b	.L1332
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
	add	x26, x20, 2228
	mov	w25, 0
	bl	_mcount
	ldr	x21, .LC277
	ldr	x22, .LC276
	mov	w23, 16
	ldr	x24, .LC278
.L1364:
	ldr	w0, [x26]
	cbz	w0, .L1373
	uxtw	x0, w25
	mov	w27, 0
	lsl	x28, x0, 2
	sub	x28, x28, x0
	lsl	x28, x28, 1
	.p2align 2
.L1370:
	uxtw	x19, w27
	add	x1, x21, 440
	add	x0, x19, x28
	cmp	w25, 3
	mov	x2, 64
	cbz	w25, .L1377
	add	x0, x20, x0, lsl 6
	beq	.L1378
	cmp	w27, 3
	ldr	x4, [x24, 104]
	add	x0, x0, 612
	csel	x1, x22, x1, cc
	blr	x4
.L1366:
	add	x19, x28, x19
	add	w27, w27, 1
	add	x19, x20, x19, lsl 2
	str	w23, [x19, 2244]
	ldr	w0, [x26]
	cmp	w0, w27
	bhi	.L1370
.L1373:
	add	w25, w25, 1
	add	x26, x26, 4
	cmp	w25, 4
	bne	.L1364
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1377:
	add	x0, x20, x19, lsl 6
	ldr	x1, .LC279
	ldr	x4, [x24, 104]
	mov	x2, 16
	add	x0, x0, 612
	blr	x4
	b	.L1366
.L1378:
	ldr	x0, .LC277
	uxtw	x19, w27
	cmp	w27, wzr
	ldr	x4, [x24, 104]
	add	x1, x0, 440
	ldr	x0, .LC276
	mov	x2, 64
	csel	x1, x0, x1, eq
	add	x0, x20, x19, lsl 6
	add	x0, x0, 1764
	blr	x4
	b	.L1366
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
	ldr	x0, [x19, 28168]
	ldrb	w1, [x0, 5]
	cbz	w1, .L1409
	ldrb	w0, [x0, 6]
	cbnz	w0, .L1381
	ldr	x0, [x19, 28176]
	ldrb	w0, [x0, 21]
	cbz	w0, .L1418
.L1382:
	ldr	x20, .LC280
	mov	x2, 1536
	mov	w1, 0
	add	x0, x19, 612
	ldr	x3, [x20, 96]
	blr	x3
	add	x16, x19, 324
	mov	x15, 310
	mov	w12, 0
	mov	w17, 64
	mov	w14, 6
.L1384:
	ldr	w13, [x16, 1904]
	cbz	w13, .L1393
	ldr	w8, [x16]
	mov	w11, 0
	ldr	x9, [x19, 28176]
	cmp	w8, 64
	csel	w8, w8, w17, ls
	add	x9, x9, x15
	lsr	w10, w8, 2
.L1391:
	uxtw	x7, w11
	mov	w4, 0
	umaddl	x7, w12, w14, x7
	lsl	x7, x7, 4
	.p2align 2
.L1385:
	cmp	w4, w10
	bcs	.L1388
	cbz	w8, .L1385
	add	x0, x7, x4, uxtw
	ldrb	w1, [x9, -1]
	mov	x6, x9
	mov	w3, 0
	add	x0, x19, x0, lsl 2
	ldr	w2, [x0, 612]
	orr	w1, w2, w1
	str	w1, [x0, 612]
	.p2align 2
.L1404:
	add	w3, w3, 1
	add	x0, x7, x4, uxtw
	cmp	w3, w8
	and	w5, w3, 3
	add	x0, x19, x0, lsl 2
	lsl	w1, w5, 3
	beq	.L1385
	ldrb	w2, [x6], 1
	cmp	w5, 3
	ldr	w5, [x0, 612]
	csinc	w4, w4, w4, ne
	lsl	w1, w2, w1
	orr	w1, w5, w1
	str	w1, [x0, 612]
	b	.L1404
.L1388:
	add	w11, w11, 1
	add	x9, x9, 64
	cmp	w11, w13
	bne	.L1391
.L1393:
	add	x15, x15, 384
	add	w12, w12, 1
	cmp	x15, 1846
	add	x16, x16, 4
	bne	.L1384
	ldr	x1, [x19, 28176]
	mov	x2, 96
	ldr	x3, [x20, 104]
	add	x0, x19, 2244
	add	x1, x1, 1928
	blr	x3
.L1409:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1381:
	ldr	x0, [x19, 28176]
	ldrb	w0, [x0, 21]
	cbnz	w0, .L1382
	ldr	x20, .LC280
	mov	w1, 0
	mov	x2, 1536
	add	x0, x19, 612
	ldr	x3, [x20, 96]
	blr	x3
	ldr	x1, [x19, 28168]
	add	x16, x19, 324
	mov	w13, 0
	mov	w18, 64
	mov	w15, 6
	add	x17, x1, 910
.L1394:
	ldr	w14, [x16, 1904]
	cbz	w14, .L1403
	ldr	w9, [x16]
	mov	x10, x17
	mov	w12, 0
	cmp	w9, 64
	csel	w9, w9, w18, ls
	lsr	w11, w9, 2
.L1401:
	uxtw	x8, w12
	mov	w5, 0
	umaddl	x8, w13, w15, x8
	lsl	x8, x8, 4
	.p2align 2
.L1395:
	cmp	w5, w11
	bcs	.L1398
	cbz	w9, .L1395
	add	x0, x8, x5, uxtw
	ldrb	w2, [x10, -1]
	mov	x7, x10
	mov	w4, 0
	add	x0, x19, x0, lsl 2
	ldr	w3, [x0, 612]
	orr	w2, w3, w2
	str	w2, [x0, 612]
	.p2align 2
.L1405:
	add	w4, w4, 1
	add	x0, x8, x5, uxtw
	cmp	w4, w9
	and	w6, w4, 3
	add	x0, x19, x0, lsl 2
	lsl	w2, w6, 3
	beq	.L1395
	ldrb	w3, [x7], 1
	cmp	w6, 3
	ldr	w6, [x0, 612]
	csinc	w5, w5, w5, ne
	lsl	w2, w3, w2
	orr	w2, w6, w2
	str	w2, [x0, 612]
	b	.L1405
.L1398:
	add	w12, w12, 1
	add	x10, x10, 64
	cmp	w12, w14
	bne	.L1401
.L1403:
	add	w13, w13, 1
	add	x17, x17, 384
	cmp	w13, 4
	add	x16, x16, 4
	bne	.L1394
	ldr	x3, [x20, 104]
	mov	x2, 96
	add	x1, x1, 2528
	add	x0, x19, 2244
	blr	x3
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1418:
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
	ldr	w0, [x0, 324]
	lsl	x9, x9, 4
	cmp	w0, w8
	csel	w8, w0, w8, ls
	lsr	w10, w8, 2
	.p2align 2
.L1420:
	cmp	w5, w10
	bcs	.L1419
	cbz	w8, .L1420
	add	x0, x9, x5, uxtw
	mov	x7, x21
	mov	w4, 0
	add	x0, x20, x0, lsl 2
	ldrb	w2, [x7], 1
	ldr	w3, [x0, 612]
	orr	w2, w3, w2
	str	w2, [x0, 612]
	.p2align 2
.L1426:
	add	w4, w4, 1
	add	x0, x9, x5, uxtw
	cmp	w8, w4
	and	w6, w4, 3
	add	x0, x20, x0, lsl 2
	lsl	w2, w6, 3
	beq	.L1420
	ldrb	w3, [x7], 1
	cmp	w6, 3
	ldr	w6, [x0, 612]
	csinc	w5, w5, w5, ne
	lsl	w2, w3, w2
	orr	w2, w6, w2
	str	w2, [x0, 612]
	b	.L1426
.L1419:
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
	bls	.L1440
	ldr	x1, .LC284
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1440:
	ldr	x0, .LC282
	ldrb	w0, [x0,w19,uxtw]
	adr	x1, .Lrtx1430
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1430:
	.section	.rodata
	.align	0
	.align	2
.L1430:
	.byte	(.L1434 - .Lrtx1430) / 4
	.byte	(.L1432 - .Lrtx1430) / 4
	.byte	(.L1432 - .Lrtx1430) / 4
	.byte	(.L1433 - .Lrtx1430) / 4
	.text
	.p2align 3
.L1432:
	cmp	w20, 2
	ldr	x0, .LC281
	bhi	.L1437
.L1438:
	add	x0, x0, 144
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1433:
	ldr	x0, .LC281
	cbz	w20, .L1438
.L1437:
	add	x0, x0, 80
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1434:
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
	.xword	.L1430
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
	beq	.L1451
	sub	w0, w20, #1
	mov	w19, 1
	asr	w0, w0, 1
	cbz	w0, .L1443
	.p2align 2
.L1444:
	asr	w0, w0, 1
	add	w19, w19, 1
	cbnz	w0, .L1444
.L1443:
	ldr	x0, [x22, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	cmp	w20, 1
	strb	w0, [x23]
	cset	w24, hi
	cmp	w0, wzr
	cset	w0, ne
	tst	w24, w0
	beq	.L1448
	ldr	w0, [x21, 168]
	cbz	w0, .L1448
	mov	w20, 0
	.p2align 2
.L1449:
	ldr	x0, [x22, 2344]
	mov	w1, w19
	bl	BsGet
	add	x1, x23, x20, uxtw 2
	add	w20, w20, 1
	str	w0, [x1, 4]
	ldr	w0, [x21, 168]
	cmp	w0, w20
	bhi	.L1449
.L1448:
	ldr	w0, [x21, 60]
	cbz	w0, .L1466
.L1447:
	mov	w0, 0
.L1442:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1466:
	ldr	x0, [x22, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, wzr
	strb	w0, [x23, 1]
	cset	w0, ne
	tst	w24, w0
	beq	.L1447
	ldr	w0, [x21, 172]
	cbz	w0, .L1447
	mov	w20, 0
	.p2align 2
.L1450:
	ldr	x0, [x22, 2344]
	mov	w1, w19
	bl	BsGet
	add	x1, x23, x20, uxtw 2
	add	w20, w20, 1
	str	w0, [x1, 132]
	ldr	w0, [x21, 172]
	cmp	w0, w20
	bhi	.L1450
	b	.L1447
.L1451:
	mov	w0, -1
	b	.L1442
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
	ldr	x0, [x19, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L1469
	ldr	x1, [x19, 2392]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w1, [x1, 12]
	add	w1, w1, 3
	cmp	w1, w0
	cset	w0, cc
	ret
	.p2align 3
.L1469:
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
	ldr	x0, [x19, 2344]
	ldr	w2, [x0, 24]
	ldr	w3, [x0, 32]
	lsl	w2, w2, 3
	cmp	w2, w3
	bls	.L1473
	mov	w1, 8
	bl	BsShow
	cmp	w0, 128
	cset	w0, ne
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1473:
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
.L1487:
	ldr	x1, [x20, 2392]
	mov	w3, 0
	ldr	w5, [x1, 8]
	sub	w5, w5, #1
	add	x2, x1, x5, lsl 5
	ldr	w0, [x2, 56]
	ldr	x2, [x2, 48]
	sub	x0, x0, #1
	add	x0, x2, x0
	.p2align 2
.L1475:
	ldrb	w2, [x0]
	lsl	x19, x5, 5
	add	x4, x1, x19
	cbnz	w2, .L1488
	ldr	x2, [x1, 48]
	add	w3, w3, 1
	cmp	x0, x2
	beq	.L1489
	ldr	x2, [x4, 48]
	cmp	x0, x2
	sub	x0, x0, #1
	bne	.L1475
	add	x1, x1, x5, lsl 5
	ldr	w0, [x20, 276]
	ldr	w1, [x1, 76]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x20, 2392]
	add	x19, x0, x19
	str	xzr, [x19, 48]
	ldr	x1, [x20, 2392]
	ldr	w0, [x1, 8]
	sub	w0, w0, #1
	str	w0, [x1, 8]
	b	.L1487
	.p2align 3
.L1488:
	mov	w0, 1
	mov	w5, w0
	tbnz	x2, 0, .L1482
	.p2align 2
.L1485:
	lsl	w4, w5, w0
	add	w0, w0, 1
	tst	w4, w2
	bne	.L1482
	cmp	w0, 8
	bne	.L1485
	ldr	x1, .LC290
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1489:
	ldr	x1, .LC288
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1482:
	str	wzr, [x1, 4]
	add	w3, w0, w3, lsl 3
	mov	w0, 0
	ldr	x1, [x20, 2392]
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
	ble	.L1491
	udiv	w1, w0, w3
	add	x2, x19, 65536
	ldr	x5, [x2, 2888]
	mov	w2, 0
	msub	w4, w1, w3, w0
	lsl	x1, x4, 3
	mov	x0, x4
	sub	x1, x1, x4
	add	x1, x5, x1, lsl 4
	ldr	w1, [x1, 36]
	cbnz	w1, .L1494
	b	.L1492
	.p2align 3
.L1495:
	udiv	w0, w1, w3
	msub	w4, w0, w3, w1
	lsl	x1, x4, 3
	mov	x0, x4
	sub	x1, x1, x4
	add	x1, x5, x1, lsl 4
	ldr	w1, [x1, 36]
	cbz	w1, .L1492
.L1494:
	add	w2, w2, 1
	cmp	w3, w2
	add	w1, w2, w0
	bne	.L1495
.L1491:
	str	w0, [x19, 140]
.L1506:
	mov	w0, -1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1492:
	cmp	w0, w3
	str	w0, [x19, 140]
	bcs	.L1506
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
	cbz	x20, .L1516
	ldr	x24, .LC293
	add	x19, x20, 48
	ldr	x23, .LC295
	add	x22, x20, 112
	stp	wzr, wzr, [x20, 36]
.L1511:
	ldr	x1, [x19]
	mov	w0, w21
	cbz	x1, .L1510
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
.L1510:
	add	x19, x19, 32
	cmp	x19, x22
	bne	.L1511
	ldr	x3, .LC296
	mov	x2, 112
	mov	w1, 0
	mov	x0, x20
	ldr	x3, [x3, 96]
	blr	x3
.L1507:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1516:
	ldr	x2, .LC291
	mov	w3, 11740
	ldr	x1, .LC292
	mov	w0, 0
	bl	dprint_vfmw
	b	.L1507
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
	ldr	x0, [x21, 28168]
	ldr	w0, [x0, 236]
	add	w0, w0, 1
	cmp	w0, 17
	bls	.L1549
	mov	w0, 17
.L1549:
	add	x2, x21, 2560
	add	x1, x21, 3008
	stp	w0, wzr, [x2, 152]
	stp	wzr, wzr, [x2, 160]
	str	wzr, [x21, 2728]
	add	x0, x21, 2872
	str	wzr, [x21, 2732]
	.p2align 2
.L1520:
	str	xzr, [x0, -136]
	add	x0, x0, 8
	str	xzr, [x0, -8]
	str	xzr, [x0, 128]
	str	xzr, [x0, 264]
	cmp	x0, x1
	bne	.L1520
	add	x22, x21, 65536
	ldr	x23, .LC297
	add	x19, x22, 3704
	mov	w20, 0
	mov	w26, 20
	mov	w25, 16
	.p2align 2
.L1521:
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
	bne	.L1521
	ldr	x3, [x23, 96]
	mov	w1, 0
	mov	x2, 200
	add	x0, x21, 3280
	blr	x3
	str	w24, [x21, 3280]
	add	x1, x21, 2400
	add	x0, x21, 2536
	str	wzr, [x21, 3284]
	.p2align 2
.L1522:
	str	xzr, [x1], 8
	cmp	x0, x1
	bne	.L1522
	add	x2, x21, 53248
	add	x1, x21, 2672
	str	wzr, [x2, 1952]
	.p2align 2
.L1523:
	str	xzr, [x0], 8
	cmp	x0, x1
	bne	.L1523
	str	wzr, [x2, 1956]
	ldr	w0, [x21, 180]
	cbz	w0, .L1528
	sub	w1, w0, #1
	add	x0, x21, 2148
	add	x1, x1, 538
	add	x1, x21, x1, lsl 2
	.p2align 2
.L1527:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L1527
.L1528:
	ldr	w3, [x21, 160]
	mov	w20, 0
	cbz	w3, .L1517
	.p2align 2
.L1541:
	uxtw	x0, w20
	ldr	x1, [x22, 3696]
	lsl	x19, x0, 5
	add	w20, w20, 1
	add	x19, x19, x0, lsl 3
	add	x0, x1, x19
	ldr	x0, [x0, 32]
	mov	x1, x0
	cbz	x0, .L1529
	ldr	x2, [x21, 2392]
	cmp	x0, x2
	beq	.L1529
	ldr	w0, [x21, 276]
	bl	HEVC_ReleaseNAL
	ldr	x0, [x22, 3696]
	mov	w2, 2
	add	x0, x0, x19
	str	xzr, [x0, 32]
	ldr	w1, [x21, 208]
	ldr	w0, [x21, 248]
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
	str	w0, [x21, 248]
.L1529:
	cmp	w3, w20
	bhi	.L1541
.L1517:
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
	cbz	x20, .L1561
	ldr	w3, [x20, 2716]
	mov	w0, 0
	mov	w4, 1
	cbz	w3, .L1556
	.p2align 2
.L1570:
	add	x1, x20, x0, uxtw 3
	add	w0, w0, 1
	ldr	x1, [x1, 2736]
	ldrb	w2, [x1, 2]
	cbnz	w2, .L1554
	strb	w4, [x1, 2]
	ldr	w3, [x20, 2716]
.L1554:
	cmp	w3, w0
	bhi	.L1570
.L1556:
	mov	w0, -2147483647
	add	x19, x20, 2736
	add	x21, x20, 2872
	str	w0, [x20, 244]
	.p2align 2
.L1553:
	ldr	x0, [x19]
	mov	w2, 1
	cbz	x0, .L1557
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L1557:
	ldr	x0, [x19, 136]
	mov	w2, 1
	cbz	x0, .L1558
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L1558:
	ldr	x0, [x19, 272]
	mov	w2, 1
	cbz	x0, .L1559
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L1559:
	ldr	x0, [x19, 408]
	mov	w2, 1
	cbz	x0, .L1560
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L1560:
	str	xzr, [x19, 136]
	str	xzr, [x19, 272]
	str	xzr, [x19, 408]
	str	xzr, [x19], 8
	cmp	x19, x21
	bne	.L1553
	mov	x0, x20
	bl	Hevc_InitDecBuffers
	mov	w0, 0
.L1551:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1561:
	mov	w0, -1
	b	.L1551
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
	ldr	x0, [x19, 2392]
	cbz	x0, .L1580
	ldr	w0, [x0, 56]
	cmp	w0, 4095
	bhi	.L1577
	ldr	x1, .LC301
	mov	w0, 7
	bl	dprint_vfmw
	ldr	x1, [x19, 2392]
	ldr	x3, .LC302
	ldr	w2, [x1, 56]
	ldr	x0, [x1, 80]
	ldr	x3, [x3, 104]
	ldr	x1, [x1, 48]
	sub	x0, x0, x2
	blr	x3
	ldr	x0, [x19, 2392]
	mov	w3, 1
	ldr	w2, [x0, 108]
	ldr	w1, [x0, 76]
	str	w2, [x0, 76]
	ldr	x0, [x19, 2392]
	str	wzr, [x0, 68]
	ldr	x0, [x19, 2392]
	ldr	w4, [x0, 56]
	ldr	x2, [x0, 80]
	sub	x2, x2, x4
	str	x2, [x0, 48]
	ldr	x0, [x19, 2392]
	ldr	w4, [x0, 56]
	ldr	w2, [x0, 92]
	sub	w2, w2, w4
	str	w2, [x0, 60]
	ldr	x0, [x19, 2392]
	ldr	w4, [x0, 56]
	ldr	w2, [x0, 88]
	add	w2, w4, w2
	str	w2, [x0, 56]
	ldr	x0, [x19, 2392]
	str	w3, [x0, 8]
	ldr	w0, [x19, 276]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 2392]
	str	xzr, [x0, 80]
	ldr	x0, [x19, 2392]
	str	wzr, [x0, 100]
	ldr	x0, [x19, 2392]
	str	wzr, [x0, 88]
.L1577:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1580:
	ldr	x1, .LC299
	bl	dprint_vfmw
	b	.L1577
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
	cbz	x20, .L1586
	ldr	w3, [x19, 2716]
	cbz	w3, .L1586
	ldr	x0, [x19, 2736]
	cmp	x20, x0
	beq	.L1588
	sub	w3, w3, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L1583
	.p2align 3
.L1584:
	ldr	x2, [x0, 2736]
	cmp	x20, x2
	beq	.L1588
.L1583:
	cmp	x0, x3
	add	x0, x0, 8
	bne	.L1584
.L1586:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1588:
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
	bls	.L1595
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
.L1595:
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
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 4]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19]
	cbz	w0, .L1597
	ldr	w2, [x19, 16]
.L1598:
	cmp	w2, 4
	mov	w1, 0
	bls	.L1619
.L1600:
	mov	w0, w1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1619:
	ldr	x0, .LC309
	ldrb	w0, [x0,w2,uxtw]
	adr	x1, .Lrtx1602
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1602:
	.section	.rodata
	.align	0
	.align	2
.L1602:
	.byte	(.L1601 - .Lrtx1602) / 4
	.byte	(.L1603 - .Lrtx1602) / 4
	.byte	(.L1604 - .Lrtx1602) / 4
	.byte	(.L1605 - .Lrtx1602) / 4
	.byte	(.L1606 - .Lrtx1602) / 4
	.text
	.p2align 3
.L1597:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 1]
	mov	w1, 8
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 8]
	mov	w1, 8
	ldr	x0, [x20, 2344]
	bl	BsGet
	cmp	w0, 8
	str	w0, [x19, 12]
	bhi	.L1620
	ldr	x0, [x20, 2344]
	mov	w1, 8
	bl	BsGet
	mov	w2, w0
	str	w0, [x19, 16]
	b	.L1598
	.p2align 3
.L1605:
	ldr	x0, [x20, 2344]
	mov	w1, 16
	bl	BsGet
	cmp	w0, 256
	str	w0, [x19, 80]
	bhi	.L1610
	mov	w21, 0
	cbz	w0, .L1613
	.p2align 2
.L1616:
	ldr	w1, [x19, 8]
	add	x22, x19, x21, uxtw 2
	ldr	x0, [x20, 2344]
	add	w21, w21, 1
	add	w1, w1, 7
	and	w1, w1, -8
	bl	BsGet
	str	w0, [x22, 84]
	ldr	w1, [x19, 12]
	ldr	x0, [x20, 2344]
	add	w1, w1, 7
	and	w1, w1, -8
	bl	BsGet
	str	w0, [x22, 1108]
	ldr	w0, [x19, 80]
	cmp	w0, w21
	bhi	.L1616
.L1613:
	mov	w1, 0
	ldr	x23, [sp, 48]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1604:
	ldr	w23, [x19, 12]
	mov	w0, 1
	lsl	w23, w0, w23
	cbz	w23, .L1609
	sub	w22, w23, #1
	add	x21, x19, 2132
	add	x22, x22, 534
	add	x22, x19, x22, lsl 2
	.p2align 2
.L1608:
	ldr	w1, [x19, 8]
	ldr	x0, [x20, 2344]
	add	w1, w1, 7
	and	w1, w1, -8
	bl	BsGet
	str	w0, [x21], 4
	cmp	x22, x21
	bne	.L1608
.L1609:
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
.L1603:
	ldr	x0, [x20, 2344]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 28]
	mov	w1, 32
	ldr	x0, [x20, 2344]
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
.L1601:
	ldr	x0, [x20, 2344]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 20]
	mov	w1, 32
	ldr	x0, [x20, 2344]
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
.L1606:
	ldr	x0, [x20, 2344]
	mov	w1, 8
	bl	BsGet
	cmp	w0, 255
	str	w0, [x19, 36]
	beq	.L1621
.L1614:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 2]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 52]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 56]
	mov	w1, 32
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 60]
	mov	w1, 32
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 64]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 68]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 72]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 52]
	mov	w1, 16
	ldr	x0, [x20, 2344]
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
.L1621:
	ldr	x0, [x20, 2344]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 40]
	b	.L1614
.L1620:
	ldr	x1, .LC308
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1600
.L1610:
	ldr	x1, .LC311
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1600
	.size	HEVC_Sei_Tone_Mapping_Info, .-HEVC_Sei_Tone_Mapping_Info
	.align	3
.LC306:
	.xword	.LC305
	.align	3
.LC308:
	.xword	.LC307
	.align	3
.LC309:
	.xword	.L1602
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
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	str	w0, [x20, 12]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20]
	cbnz	w0, .L1623
	ldr	x0, [x19, 2344]
	mov	w1, 7
	bl	BsGet
	sub	w1, w0, #3
	cmp	w1, 2
	str	w0, [x20, 16]
	bhi	.L1630
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20, 1]
	mov	w1, 6
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x20, 20]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 2]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 3]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 5]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 6]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 7]
	ldrb	w1, [x20, 1]
	cbnz	w1, .L1626
	ldr	w0, [x20, 16]
	cmp	w0, 5
	beq	.L1626
	ldr	x0, [x19, 2344]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 24]
	mov	w1, 4
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x20, 28]
	mov	w1, 4
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x20, 32]
	mov	w1, 4
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x20, 36]
.L1626:
	ldr	x0, [x19, 2344]
	mov	w1, 8
	bl	BsGet
	str	w0, [x20, 40]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 8]
.L1623:
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20, 9]
	ldr	x0, [x19, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L1625:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1630:
	ldr	x1, .LC315
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1625
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
	beq	.L1636
	ldr	x1, .LC317
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	str	w0, [x20, 4]
	ldrb	w0, [x21, 3]
	cbnz	w0, .L1640
	str	wzr, [x20, 8]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20]
	cbnz	w0, .L1641
.L1635:
	ldr	x0, [x19, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L1632:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1640:
	ldr	w1, [x21, 8]
	ldr	x0, [x19, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x20, 8]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20]
	cbz	w0, .L1635
.L1641:
	ldr	w1, [x21, 12]
	ldr	x0, [x19, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x20, 12]
	b	.L1635
	.p2align 3
.L1636:
	mov	w0, -1
	b	.L1632
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
	ldr	x0, [x21, 2344]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x20, 1]
	ldr	x1, .LC319
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	cmp	w0, 15
	str	w0, [x20, 8]
	bgt	.L1643
	tbnz	w0, #31, .L1647
	ldr	x22, .LC323
	mov	w19, 0
	.p2align 2
.L1646:
	ldr	x0, [x21, 2344]
	mov	x1, x22
	bl	HEVC_ue_v
	add	x1, x20, x19, sxtw 2
	add	w19, w19, 1
	str	w0, [x1, 12]
	ldr	w0, [x20, 8]
	cmp	w0, w19
	bge	.L1646
.L1647:
	ldr	x0, [x21, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L1645:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1643:
	ldr	x1, .LC321
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1645
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
	ldr	w0, [x19, 3504]
	ldr	w2, [x19, 3512]
	add	w0, w0, 1
	str	wzr, [x19, 3512]
	str	w0, [x19, 3504]
	str	w2, [x19, 3524]
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 15
	str	w0, [x22, 4]
	bhi	.L1677
	mov	w0, 1
	strb	w0, [x19, 3577]
	ldrb	w0, [x23, 2]
	cbz	w0, .L1652
	ldrb	w0, [x22]
	cbnz	w0, .L1678
.L1654:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x22, 1]
	ldr	w1, [x23, 32]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x22, 16]
	ldr	w2, [x23, 48]
	cmp	w2, 31
	bhi	.L1679
.L1655:
	mov	w21, 1
	mov	w19, 0
.L1656:
	cbz	w19, .L1680
	cmp	w19, 1
	beq	.L1681
.L1661:
	cmp	w21, 1
	bhi	.L1660
.L1659:
	add	w21, w21, 1
	add	w19, w19, 1
	b	.L1656
	.p2align 3
.L1660:
	ldr	x0, [x20, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L1676:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1652:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x22]
	cbz	w0, .L1654
.L1678:
	ldr	w1, [x23, 32]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x22, 8]
	ldr	w1, [x23, 36]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x22, 12]
	b	.L1654
	.p2align 3
.L1679:
	ldr	x1, .LC329
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x23, 48]
	b	.L1655
.L1677:
	ldr	x1, .LC327
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1676
.L1680:
	ldrb	w0, [x23]
	cbz	w0, .L1661
	ldr	w0, [x23, 48]
	mov	w25, 0
	cmn	w0, #1
	beq	.L1659
.L1666:
	uxtw	x26, w19
.L1665:
	add	x24, x26, x25, uxtw 1
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2344]
	add	w25, w25, 1
	add	x24, x22, x24, lsl 4
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 20]
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 24]
	ldrb	w0, [x23, 2]
	cbnz	w0, .L1663
	ldrb	w0, [x22]
	cbz	w0, .L1664
.L1663:
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 28]
	ldr	w1, [x23, 28]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x24, 32]
.L1664:
	ldr	w0, [x23, 48]
	add	w0, w0, 1
	cmp	w0, w25
	bhi	.L1665
	b	.L1661
.L1681:
	ldrb	w0, [x23, 1]
	cbz	w0, .L1660
	ldr	w0, [x23, 48]
	cmn	w0, #1
	beq	.L1660
	mov	w25, 0
	b	.L1666
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
	ldr	w2, [x0, 3508]
	ldr	w1, [x0, 3512]
	add	w2, w2, 1
	str	w2, [x0, 3508]
	add	w1, w1, 1
	str	w1, [x0, 3512]
	ldrb	w0, [x19, 9]
	cbnz	w0, .L1707
.L1683:
	ldrh	w0, [x19, 108]
	add	x19, x19, 108
	cbz	w0, .L1686
	ldr	w1, [x19, 32]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x21, 12]
	ldr	w1, [x19, 36]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x21, 16]
	ldrb	w0, [x19, 2]
	cbnz	w0, .L1708
.L1686:
	ldr	x0, [x20, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L1685:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1707:
	ldr	x0, [x20, 2344]
	mov	w1, 4
	bl	BsGet
	str	w0, [x21, 4]
	mov	w1, 2
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x21, 8]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x21]
	b	.L1683
	.p2align 3
.L1708:
	ldr	w1, [x19, 12]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x21, 20]
	ldrb	w0, [x19, 2]
	cbz	w0, .L1686
	ldrb	w0, [x19, 3]
	cbz	w0, .L1686
	ldr	x1, .LC331
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x21, 24]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 1]
	cbnz	w0, .L1709
.L1687:
	ldr	w0, [x21, 24]
	cmp	w0, 255
	bhi	.L1710
	ldr	x24, .LC335
	mov	w22, 0
	b	.L1690
	.p2align 3
.L1689:
	ldr	w0, [x21, 24]
	cmp	w0, w22
	bcc	.L1686
.L1690:
	add	x23, x21, x22, uxtw 2
	ldr	x0, [x20, 2344]
	mov	x1, x24
	add	w22, w22, 1
	bl	HEVC_ue_v
	str	w0, [x23, 32]
	ldrb	w0, [x21, 1]
	cbnz	w0, .L1689
	ldr	w1, [x19, 8]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x23, 1056]
	b	.L1689
	.p2align 3
.L1709:
	ldr	w1, [x19, 8]
	ldr	x0, [x20, 2344]
	add	w1, w1, 1
	bl	BsGet
	str	w0, [x21, 28]
	b	.L1687
.L1710:
	ldr	x1, .LC333
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1685
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
	cbnz	w20, .L1757
	str	wzr, [x19]
.L1714:
	ldr	x1, .LC349
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	cmp	w0, 16
	str	w0, [x19, 16]
	bhi	.L1758
	ldr	x1, .LC353
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	cmp	w0, 16
	str	w0, [x19, 20]
	bhi	.L1759
	ldr	w1, [x19, 16]
	add	w2, w0, w1
	str	w2, [x19, 24]
	cmp	w2, 16
	bhi	.L1740
	mov	w20, 0
	cbz	w1, .L1742
	ldr	x25, .LC359
	mov	w22, 0
	mov	w23, 0
	mov	w24, 32767
	b	.L1744
	.p2align 3
.L1743:
	str	w23, [x2, 72]
	add	x20, x19, x20
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x20, 53]
	ldr	w20, [x19, 16]
	cmp	w20, w22
	bls	.L1760
.L1744:
	ldr	x0, [x21, 2344]
	mov	x1, x25
	uxtw	x20, w22
	add	w22, w22, 1
	bl	HEVC_ue_v
	cmp	w0, w24
	mvn	w1, w0
	add	x2, x19, x20, lsl 2
	add	w23, w23, w1
	mov	w1, 1
	bls	.L1743
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
.L1757:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19]
	cbz	w0, .L1714
	ldr	w0, [x23, 228]
	cmp	w20, w0
	beq	.L1761
	sub	w20, w20, #1
	str	wzr, [x19, 4]
.L1718:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19, 8]
	ldr	x1, .LC343
	ldr	x0, [x21, 2344]
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
	bhi	.L1762
	mov	w28, 0
	mov	w27, 0
	mov	w20, 0
	mov	w22, 0
	b	.L1726
	.p2align 3
.L1721:
	sub	w2, w0, #1
	uxtw	x1, w28
	cmp	w2, 1
	bls	.L1763
.L1722:
	add	x1, x19, x1
	add	x2, x23, x24
	add	w28, w28, 1
	strb	w0, [x1, 36]
	ldr	w0, [x2, 13132]
	cmp	w0, w28
	bcc	.L1764
.L1726:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	cbnz	w0, .L1721
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	lsl	w0, w0, 1
	sub	w2, w0, #1
	uxtw	x1, w28
	cmp	w2, 1
	bhi	.L1722
.L1763:
	add	x5, x23, x24
	uxtw	x2, w22
	add	x3, x25, x1
	add	x6, x19, x2
	add	x2, x19, x2, lsl 2
	ldr	w5, [x5, 13132]
	add	x4, x23, x3, lsl 2
	mov	w3, 0
	cmp	w5, w28
	bls	.L1723
	ldr	w3, [x4, 13180]
.L1723:
	cmp	w0, 1
	add	w3, w3, w26
	str	w3, [x2, 72]
	cset	w2, eq
	strb	w2, [x6, 53]
	tbnz	w3, #31, .L1765
	add	w27, w27, 1
	add	w22, w22, 1
.L1766:
	add	x1, x19, x1
	add	x2, x23, x24
	add	w28, w28, 1
	strb	w0, [x1, 36]
	ldr	w0, [x2, 13132]
	cmp	w0, w28
	bcs	.L1726
.L1764:
	cmp	w22, 16
	add	w0, w0, 1
	ccmp	w20, 16, 2, ls
	ccmp	w27, 16, 2, ls
	stp	w20, w27, [x19, 16]
	stp	w22, w0, [x19, 24]
	bhi	.L1727
	cmp	w22, 1
	bls	.L1733
	sub	w10, w22, #2
	add	x9, x19, 76
	add	x10, x10, 1
	mov	x7, 0
	.p2align 2
.L1732:
	add	x0, x19, x7
	mov	w3, w7
	ldr	w5, [x9]
	ldrb	w8, [x0, 54]
	tbnz	w7, #31, .L1736
	.p2align 2
.L1748:
	sxtw	x1, w3
	add	w0, w3, 1
	add	x4, x19, x1
	add	x1, x19, x1, lsl 2
	sxtw	x0, w0
	add	x6, x19, x0
	add	x0, x19, x0, lsl 2
	ldr	w2, [x1, 72]
	cmp	w5, w2
	bge	.L1734
	str	w2, [x0, 72]
	ldrb	w0, [x4, 53]
	strb	w0, [x6, 53]
	str	w5, [x1, 72]
	strb	w8, [x4, 53]
.L1734:
	subs	w3, w3, #1
	bpl	.L1748
.L1736:
	add	x7, x7, 1
	add	x9, x9, 4
	cmp	x7, x10
	bne	.L1732
.L1733:
	lsr	w7, w20, 1
	sub	w20, w20, #1
	cbz	w7, .L1730
	add	x2, x19, 72
	add	x1, x19, 53
	sub	w7, w20, w7
	.p2align 2
.L1737:
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
	bne	.L1737
.L1730:
	mov	w0, 0
.L1719:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1765:
	add	w20, w20, 1
	add	w22, w22, 1
	b	.L1766
.L1760:
	ldr	w2, [x19, 24]
.L1742:
	cmp	w2, w20
	bls	.L1730
	ldr	x24, .LC363
	mov	w23, 0
	.p2align 2
.L1745:
	ldr	x0, [x21, 2344]
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
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x22, 53]
	ldr	w0, [x19, 24]
	cmp	w0, w20
	bhi	.L1745
	b	.L1730
.L1761:
	ldr	x1, .LC337
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w20, w2
	str	w2, [x19, 4]
	bcc	.L1767
	sub	w3, w20, #1
	sub	w20, w3, w0
	cmp	w20, w3
	bls	.L1718
	ldr	x1, .LC341
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1719
.L1727:
	ldr	x1, .LC347
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1719
.L1758:
	ldr	x1, .LC351
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1719
.L1767:
	ldr	x1, .LC339
	mov	w3, w20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1719
.L1762:
	ldr	x1, .LC345
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1719
.L1740:
	ldr	x1, .LC357
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1719
.L1759:
	ldr	x1, .LC355
	mov	w0, 1
	ldr	w2, [x19, 16]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1719
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
	beq	.L1792
	cbnz	w19, .L1813
.L1771:
	ldr	x23, .LC365
	mov	w19, 0
	ldr	x25, .LC367
	ldr	x24, .LC369
	ldr	x27, .LC371
	.p2align 2
.L1789:
	ldr	x0, [x21, 2344]
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
	cbz	w0, .L1814
	mov	w0, 1
	strb	w0, [x26, 41]
.L1776:
	add	x2, x28, x28, lsl 2
	lsl	x2, x2, 2
	sub	x2, x2, x28
	add	x2, x2, x2, lsl 4
	add	x26, x20, x2, lsl 2
	ldrb	w0, [x26, 41]
	strb	wzr, [x26, 42]
	str	wzr, [x26, 48]
	cbz	w0, .L1777
	ldr	x0, [x21, 2344]
	mov	x1, x23
	bl	HEVC_ue_v
	str	w0, [x26, 44]
	ldrb	w1, [x26, 42]
	cbz	w1, .L1779
.L1817:
	add	x0, x28, x28, lsl 2
	lsl	x0, x0, 2
	sub	x0, x0, x28
	add	x0, x0, x0, lsl 4
	add	x0, x20, x0, lsl 2
	ldr	w0, [x0, 48]
	cmp	w0, 31
	bhi	.L1815
.L1781:
	add	x0, x28, x28, lsl 2
	mov	w2, 0
	lsl	x0, x0, 2
	sub	x3, x0, x28
	lsl	x4, x3, 4
.L1782:
	cbz	w2, .L1816
	cmp	w2, 1
	beq	.L1811
.L1788:
	add	w2, w2, 1
	cmp	w2, 2
	bne	.L1782
.L1787:
	add	w19, w19, 1
	cmp	w22, w19
	bcs	.L1789
	mov	w0, 0
.L1810:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1777:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x26, 42]
	cbnz	w1, .L1817
.L1779:
	ldr	x0, [x21, 2344]
	mov	x1, x25
	bl	HEVC_ue_v
	cmp	w0, 31
	add	x1, x28, x28, lsl 2
	lsl	x1, x1, 2
	sub	x1, x1, x28
	add	x1, x1, x1, lsl 4
	add	x1, x20, x1, lsl 2
	str	w0, [x1, 48]
	bls	.L1781
.L1815:
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
	b	.L1781
	.p2align 3
.L1814:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x26, 41]
	b	.L1776
	.p2align 3
.L1813:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x20, 1]
	ldrh	w0, [x20]
	cbz	w0, .L1771
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 2]
	cbnz	w0, .L1818
.L1773:
	ldr	x0, [x21, 2344]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 16]
	mov	w1, 4
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 20]
	ldrb	w1, [x20, 2]
	cbnz	w1, .L1819
.L1774:
	ldr	x0, [x21, 2344]
	mov	w1, 5
	bl	BsGet
	str	w0, [x20, 28]
	mov	w1, 5
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 32]
	mov	w1, 5
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 36]
	b	.L1771
.L1792:
	mov	w0, -1
	b	.L1810
.L1819:
	ldr	x0, [x21, 2344]
	mov	w1, 4
	bl	BsGet
	str	w0, [x20, 24]
	b	.L1774
.L1818:
	ldr	x0, [x21, 2344]
	mov	w1, 8
	bl	BsGet
	str	w0, [x20, 4]
	mov	w1, 5
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 8]
	mov	w1, 1
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x20, 3]
	mov	w1, 5
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 36]
	b	.L1773
.L1816:
	ldrb	w0, [x20]
	cbnz	w0, .L1794
	mov	w2, 1
.L1811:
	ldrb	w0, [x20, 1]
	cbz	w0, .L1787
.L1794:
	add	x0, x3, x4
	uxtw	x1, w2
	mov	w26, 0
	stp	x0, x1, [x29, 104]
	add	x0, x20, x0, lsl 2
	str	x0, [x29, 96]
	b	.L1791
.L1790:
	ldr	x0, [x21, 2344]
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
	bcc	.L1788
.L1791:
	ldr	x0, [x21, 2344]
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
	ldr	x0, [x21, 2344]
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
	cbz	w0, .L1790
	ldr	x1, .LC375
	ldr	x0, [x21, 2344]
	str	w2, [x29, 140]
	bl	HEVC_ue_v
	str	w0, [x28, 564]
	ldr	x1, .LC377
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	str	w0, [x28, 820]
	ldr	w2, [x29, 140]
	ldp	x4, x3, [x29, 120]
	b	.L1790
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
	ldr	x0, [x21, 2344]
	mov	w1, 2
	bl	BsGet
	cmp	w0, 3
	str	w0, [x20, 12]
	beq	.L1821
	ldr	x1, .LC379
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
.L1821:
	ldr	x0, [x21, 2344]
	mov	w1, 6
	bl	BsGet
	cmp	w0, 63
	str	w0, [x20, 16]
	bhi	.L1868
	ldr	x0, [x21, 2344]
	mov	w1, 3
	bl	BsGet
	cmp	w0, 7
	mov	w25, w0
	bhi	.L1869
	add	w23, w0, 1
	str	w23, [x20, 20]
	mov	w1, 1
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x20, 1]
	mov	w1, 16
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 24]
	mov	w1, 65535
	cmp	w0, w1
	bne	.L1870
	add	x26, x20, 12288
	mov	w3, w25
	mov	w2, 1
	add	x1, x26, 148
	mov	x0, x21
	bl	HEVC_DecPTL
	cbnz	w0, .L1871
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x20, 7]
	tbnz	w25, #31, .L1831
	ldr	x28, .LC391
	mov	w19, 0
	ldr	x27, .LC393
	ldr	x24, .LC395
	b	.L1833
	.p2align 3
.L1830:
	cmp	w23, w19
	beq	.L1831
.L1833:
	add	x22, x20, x19, sxtw 2
	ldr	x0, [x21, 2344]
	mov	x1, x28
	add	w19, w19, 1
	bl	HEVC_ue_v
	add	w0, w0, 1
	str	w0, [x22, 52]
	mov	x1, x27
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	str	w0, [x22, 84]
	mov	x1, x24
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	str	w0, [x22, 116]
	ldrb	w0, [x20, 7]
	cbnz	w0, .L1830
	cmp	w25, w19
	blt	.L1831
	.p2align 2
.L1856:
	add	x0, x20, x19, sxtw 2
	ldr	w1, [x20, 52]
	add	w19, w19, 1
	cmp	w23, w19
	str	w1, [x0, 52]
	ldr	w1, [x20, 84]
	str	w1, [x0, 84]
	ldr	w1, [x20, 116]
	str	w1, [x0, 116]
	bne	.L1856
.L1831:
	ldr	x0, [x21, 2344]
	mov	w1, 6
	bl	BsGet
	str	w0, [x20, 28]
	ldr	x1, .LC389
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	cmp	w0, 1024
	str	w0, [x20, 32]
	bhi	.L1872
	cmp	w0, 1
	ble	.L1835
	ldr	x27, .LC398
	mov	w22, 1
	ldr	x24, .LC400
	ldr	w0, [x20, 28]
	.p2align 2
.L1837:
	cmp	w0, wzr
	mov	w19, 0
	sxtw	x23, w22
	ble	.L1839
	.p2align 2
.L1836:
	ldr	x5, [x27, 136]
	mov	w4, w19
	mov	w3, w22
	mov	x2, x24
	mov	x1, 100
	add	x0, x29, 104
	blr	x5
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	add	x1, x23, x19, sxtw
	add	w19, w19, 1
	add	x1, x20, x1, lsl 2
	str	w0, [x1, 148]
	ldr	w0, [x20, 28]
	cmp	w0, w19
	bgt	.L1836
.L1839:
	ldr	w1, [x20, 32]
	add	w22, w22, 1
	cmp	w1, w22
	bgt	.L1837
.L1835:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 6]
	cbz	w0, .L1845
	ldr	x0, [x21, 2344]
	mov	w1, 32
	bl	BsGet
	str	w0, [x20, 36]
	mov	w1, 32
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 40]
	mov	w1, 1
	ldr	x0, [x21, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 5]
	cbnz	w0, .L1873
.L1843:
	ldr	x1, .LC404
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	cmp	w0, 1023
	str	w0, [x20, 48]
	bhi	.L1874
	cmp	w0, wzr
	ble	.L1845
	mov	w0, 1
	str	w0, [x20, 8340]
	ldr	x23, .LC408
	add	x26, x26, 532
	ldr	x0, [x21, 2344]
	mov	w19, 0
	mov	x1, x23
	bl	HEVC_ue_v
	str	w0, [x20, 4244]
	ldr	w2, [x20, 8340]
	.p2align 2
.L1846:
	mov	w3, w25
	mov	x1, x26
	mov	x0, x21
	add	x22, x20, x19, sxtw 2
	add	w19, w19, 1
	bl	HEVC_DecHrdParam
	cbnz	w0, .L1875
.L1849:
	ldr	w0, [x20, 48]
	mov	x1, x23
	cmp	w0, w19
	ble	.L1845
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	str	w0, [x22, 4248]
	cmp	w19, wzr
	mov	w1, 1
	ble	.L1876
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x22, 8344]
	mov	w2, w0
	mov	w3, w25
	mov	x1, x26
	mov	x0, x21
	add	x22, x20, x19, sxtw 2
	add	w19, w19, 1
	bl	HEVC_DecHrdParam
	cbz	w0, .L1849
.L1875:
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
.L1845:
	ldr	x0, [x21, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x20, 3]
	cbz	w0, .L1842
	.p2align 2
.L1863:
	ldr	x0, [x21, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L1842
	ldr	x2, [x21, 2392]
	mov	w1, 1
	ldr	w2, [x2, 12]
	add	w2, w2, 3
	cmp	w2, w0
	bcc	.L1877
.L1842:
	mov	w0, 0
.L1864:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L1877:
	ldr	x0, [x21, 2344]
	bl	BsGet
	strb	w0, [x20, 4]
	b	.L1863
	.p2align 3
.L1876:
	ldr	w2, [x22, 8344]
	b	.L1846
.L1873:
	ldr	x1, .LC402
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	str	w0, [x20, 44]
	b	.L1843
.L1870:
	ldr	x1, .LC385
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1864
.L1868:
	ldr	x1, .LC381
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1864
.L1871:
	ldr	x1, .LC387
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1864
.L1869:
	ldr	x1, .LC383
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1864
.L1872:
	ldr	x1, .LC397
.L1866:
	mov	w2, w0
	mov	w3, 1024
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1864
.L1874:
	ldr	x1, .LC406
	b	.L1866
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
	ldr	x0, [x20, 2344]
	mov	w1, 4
	bl	BsGet
	cmp	w0, 15
	mov	w21, w0
	bhi	.L1884
	mov	w19, 23196
	umull	x19, w0, w19
	ldr	x0, [x20, 2352]
	add	x1, x0, x19
	ldrb	w0, [x0, x19]
	cbnz	w0, .L1885
	str	w21, [x1, 8]
	mov	x0, x20
	ldr	x1, [x20, 2352]
	add	x1, x1, x19
	bl	HEVC_ProcessVPS
	mov	w22, w0
	cbnz	w0, .L1886
	ldr	x0, [x20, 2352]
	mov	w1, 1
	add	x0, x0, x19
	strb	w1, [x0, 2]
	ldr	x0, [x20, 2352]
	strb	w1, [x0, x19]
.L1880:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1885:
	ldr	x24, .LC413
	mov	x0, x20
	add	x23, x24, 3328
	mov	x1, x23
	str	w21, [x23, 8]
	bl	HEVC_ProcessVPS
	mov	w22, w0
	cbnz	w0, .L1887
	ldr	x3, .LC416
	mov	w1, 1
	ldr	x0, [x20, 2352]
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
.L1887:
	ldr	x1, .LC415
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1880
	.p2align 3
.L1884:
	ldr	x1, .LC412
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1880
.L1886:
	ldr	x1, .LC418
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x0, [x20, 2352]
	mov	w1, 1
	add	x0, x0, x19
	strb	w1, [x0, 2]
	ldr	x0, [x20, 2352]
	strb	wzr, [x0, x19]
	b	.L1880
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
	ldr	x0, [x20, 2344]
	mov	w3, 2
	mov	w1, w2
	stp	w2, w3, [x29, 48]
	stp	w3, w2, [x29, 56]
	stp	w2, w3, [x29, 64]
	stp	w2, w2, [x29, 72]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19]
	cbnz	w0, .L1929
.L1890:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 1]
	cbnz	w0, .L1930
.L1892:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 3]
	cbnz	w0, .L1931
.L1894:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 6]
	cbnz	w0, .L1932
.L1896:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 8]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 9]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 10]
	cbz	w0, .L1897
	ldr	x1, .LC425
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 60]
	ldr	w1, [x21, 64]
	cmp	w1, 3
	bhi	.L1933
	add	x2, x29, 80
	add	x2, x2, x1, uxtw 2
	ldr	x1, .LC429
	ldr	w2, [x2, -32]
	mul	w0, w2, w0
	str	w0, [x19, 60]
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 64]
	add	x3, x29, 80
	ldr	x1, .LC431
	ldr	w2, [x21, 64]
	add	x2, x3, x2, lsl 2
	ldr	w2, [x2, -32]
	mul	w0, w2, w0
	str	w0, [x19, 64]
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 68]
	add	x3, x29, 80
	ldr	x1, .LC433
	ldr	w2, [x21, 64]
	add	x2, x3, x2, lsl 2
	ldr	w2, [x2, -16]
	mul	w0, w2, w0
	str	w0, [x19, 68]
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 72]
	add	x2, x29, 80
	ldr	w1, [x21, 64]
	add	x1, x2, x1, lsl 2
	ldr	w1, [x1, -16]
	mul	w0, w1, w0
	str	w0, [x19, 72]
.L1897:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 11]
	cbz	w0, .L1900
	ldr	x0, [x20, 2344]
	mov	w1, 32
	bl	BsGet
	str	w0, [x19, 76]
	mov	w1, 32
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 80]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 12]
	cbnz	w0, .L1934
.L1901:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 13]
	cbnz	w0, .L1935
.L1900:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 14]
	mov	w1, 0
	cbnz	w0, .L1936
.L1899:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, w1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1936:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 15]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 16]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 17]
	ldr	x1, .LC439
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 4095
	str	w0, [x19, 88]
	bhi	.L1937
.L1902:
	ldr	x1, .LC443
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 92]
	ldr	x1, .LC445
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 96]
	ldr	x1, .LC447
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 100]
	ldr	x1, .LC449
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 104]
	mov	w1, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, w1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1932:
	ldr	x1, .LC421
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 52]
	ldr	x1, .LC423
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 56]
	b	.L1896
	.p2align 3
.L1929:
	ldr	x0, [x20, 2344]
	mov	w1, 8
	bl	BsGet
	cmp	w0, 255
	str	w0, [x19, 24]
	bne	.L1890
	ldr	x0, [x20, 2344]
	mov	w1, 16
	bl	BsGet
	str	w0, [x19, 28]
	mov	w1, 16
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 32]
	b	.L1890
	.p2align 3
.L1931:
	ldr	x0, [x20, 2344]
	mov	w1, 3
	bl	BsGet
	str	w0, [x19, 36]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	cbz	w0, .L1894
	ldr	x0, [x20, 2344]
	mov	w1, 8
	bl	BsGet
	str	w0, [x19, 40]
	mov	w1, 8
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 44]
	mov	w1, 8
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 48]
	b	.L1894
	.p2align 3
.L1930:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 2]
	b	.L1892
	.p2align 3
.L1935:
	ldr	w3, [x21, 56]
	mov	w2, 1
	add	x1, x19, 108
	mov	x0, x20
	bl	HEVC_DecHrdParam
	cbz	w0, .L1900
	ldr	x1, .LC437
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1899
	.p2align 3
.L1934:
	ldr	x1, .LC435
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 84]
	b	.L1901
	.p2align 3
.L1937:
	ldr	x1, .LC441
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1902
	.p2align 3
.L1933:
	ldr	x1, .LC427
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L1899
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
	bls	.L1943
	mov	w0, 2147483647
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1943:
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
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	str	w0, [x22]
	ldr	x1, .LC453
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	cmp	w0, 1024
	str	w0, [x22, 4]
	bhi	.L1945
	mov	w19, 0
	cbz	w0, .L1951
	ldr	x23, .LC457
	ldr	x24, .LC459
	.p2align 2
.L1954:
	add	x20, x22, x19, uxtw 2
	ldr	x0, [x21, 2344]
	mov	w1, 6
	bl	BsGet
	str	w0, [x20, 8]
	mov	w1, 3
	ldr	x0, [x21, 2344]
	bl	BsGet
	str	w0, [x20, 4104]
	ldr	w2, [x20, 8]
	mov	x1, x23
	sub	w2, w2, #19
	cmp	w2, 1
	bls	.L1948
	ldr	x0, [x21, 2344]
	bl	HEVC_ue_v
	str	w0, [x20, 8200]
.L1948:
	cbnz	w19, .L1957
.L1949:
	ldr	w0, [x22, 4]
	add	w19, w19, 1
	cmp	w0, w19
	bhi	.L1954
.L1951:
	ldr	x0, [x21, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L1947:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1957:
	ldr	x0, [x21, 2344]
	mov	x1, x24
	bl	HEVC_se_v
	str	w0, [x20, 12296]
	b	.L1949
.L1945:
	ldr	x1, .LC455
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1947
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
	beq	.L1960
	ldr	x1, .LC461
	ldr	x0, [x19, 2344]
	bl	HEVC_se_v
	str	w0, [x20, 4]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	strb	w0, [x20, 1]
	ldr	x0, [x19, 2344]
	bl	BsToNextByte
	mov	w0, 0
.L1959:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1960:
	mov	w0, -1
	b	.L1959
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
	ldr	x0, [x19, 2392]
	ldr	w0, [x0, 16]
	cmp	w0, 39
	beq	.L2008
	cmp	w20, 5
	beq	.L1988
	cmp	w20, 132
	beq	.L1989
	mov	w20, 0
	cbz	w22, .L1982
	.p2align 2
.L2002:
	ldr	x0, [x19, 2344]
	mov	w1, 8
	add	w20, w20, 1
	bl	BsGet
	cmp	w22, w20
	bne	.L2002
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_Sei_More_Data_In_Payload
	cbnz	w0, .L1992
	.p2align 2
.L1996:
	mov	w0, 0
.L1984:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1988:
	mov	w2, w22
	add	x1, x21, 3200
	mov	x0, x19
	bl	HEVC_Sei_User_Data_Unregistered
	cbnz	w0, .L1985
.L1982:
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_Sei_More_Data_In_Payload
	cbz	w0, .L1996
.L1992:
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_Sei_Payload_Extension_Present
	cbnz	w0, .L2009
.L1993:
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x21, 12]
	b	.L1994
	.p2align 3
.L1995:
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x21, 16]
.L1994:
	ldr	x0, [x19, 2344]
	bl	BsIsByteAligned
	cbz	w0, .L1995
	b	.L1996
	.p2align 3
.L2008:
	cmp	w20, 147
	bls	.L2010
.L1963:
	mov	w20, 0
	cbz	w22, .L1982
	.p2align 2
.L2001:
	ldr	x0, [x19, 2344]
	mov	w1, 8
	add	w20, w20, 1
	bl	BsGet
	cmp	w22, w20
	bne	.L2001
	b	.L1982
	.p2align 3
.L1989:
	add	x1, x21, 20480
	mov	x0, x19
	add	x1, x1, 3348
	bl	HEVC_Sei_Decoded_Picture_Hash
	cbz	w0, .L1982
	.p2align 2
.L1985:
	mov	w0, -1
	b	.L1984
	.p2align 3
.L2010:
	ldr	x0, .LC462
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx1965
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1965:
	.section	.rodata
	.align	0
	.align	2
.L1965:
	.byte	(.L1964 - .Lrtx1965) / 4
	.byte	(.L1966 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1988 - .Lrtx1965) / 4
	.byte	(.L1968 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1969 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1970 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1971 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1972 - .Lrtx1965) / 4
	.byte	(.L1973 - .Lrtx1965) / 4
	.byte	(.L1974 - .Lrtx1965) / 4
	.byte	(.L1975 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1976 - .Lrtx1965) / 4
	.byte	(.L1977 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1978 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1979 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1963 - .Lrtx1965) / 4
	.byte	(.L1980 - .Lrtx1965) / 4
	.text
	.p2align 3
.L2009:
	ldr	x1, .LC464
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	str	w0, [x21, 8]
	b	.L1993
.L1980:
	add	x0, x21, 3486
	cmp	x19, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L1985
	ldr	x0, [x19, 2344]
	mov	w1, 8
	bl	BsGet
	strb	w0, [x21, 3486]
	b	.L1982
.L1979:
	add	x1, x21, 3480
	mov	x0, x19
	bl	HEVC_Sei_Content_Light_Level_Information
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1978:
	add	x1, x21, 3436
	cmp	x19, xzr
	ccmp	x1, xzr, 4, ne
	beq	.L1985
	mov	w0, 1
	strb	w0, [x21, 3436]
	mov	x0, x19
	bl	HEVC_Sei_Mastering_Display_Colour_Volume.part.12
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1977:
	add	x0, x21, 3432
	cmp	x19, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L1985
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21, 3432]
	b	.L1982
.L1976:
	mov	w3, w22
	mov	x2, x23
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_Sei_Scalable_Nesting
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1975:
	add	x1, x21, 3424
	mov	x0, x19
	bl	HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1974:
	add	x2, x23, 2732
	add	x1, x21, 3348
	mov	x0, x19
	bl	HEVC_Sei_Dec_Unit_Info
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1973:
	add	x1, x21, 3272
	mov	x0, x19
	bl	HEVC_Sei_Active_Param_sets
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1972:
	add	x1, x21, 4096
	mov	w2, w22
	add	x1, x1, 2552
	mov	x0, x19
	bl	HEVC_Sei_Structure_Of_Pictures_Info
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1971:
	add	x1, x21, 3416
	mov	x0, x19
	bl	HEVC_Sei_Display_Orientation
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1970:
	add	x1, x21, 3372
	mov	x0, x19
	bl	HEVC_Sei_Frame_Packing_Arrangement
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1969:
	add	x1, x21, 3488
	mov	x0, x19
	bl	HEVC_Sei_Tone_Mapping_Info
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1968:
	add	x1, x21, 3364
	mov	x0, x19
	bl	HEVC_Sei_Recovery_Point
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1966:
	add	x2, x23, 2624
	add	x1, x21, 1088
	mov	x0, x19
	bl	HEVC_Sei_Pic_Timming
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.p2align 3
.L1964:
	add	x2, x23, 2732
	add	x1, x21, 24
	mov	x0, x19
	bl	HEVC_Sei_Buf_Period
	cbz	w0, .L1982
	mov	w0, -1
	b	.L1984
	.size	HEVC_Sei_Payload, .-HEVC_Sei_Payload
	.align	3
.LC462:
	.xword	.L1965
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
	ldr	x0, [x19, 2344]
	b	.L2012
	.p2align 3
.L2014:
	bl	BsSkip
	add	w20, w20, 255
	ldr	x0, [x19, 2344]
	ldr	w3, [x0, 32]
	ldr	w4, [x0, 24]
	add	w1, w3, 8
	cmp	w1, w4
	bhi	.L2021
.L2012:
	mov	w1, 8
	bl	BsShow
	cmp	w0, 255
	mov	w1, 8
	ldr	x0, [x19, 2344]
	beq	.L2014
	bl	BsGet
	str	w0, [x22]
	mov	w24, w0
	mov	w21, 0
	ldr	x0, [x19, 2344]
	b	.L2015
	.p2align 3
.L2016:
	bl	BsSkip
	add	w21, w21, 255
	ldr	x0, [x19, 2344]
	ldr	w3, [x0, 32]
	ldr	w4, [x0, 24]
	add	w1, w3, 8
	cmp	w1, w4
	bhi	.L2022
.L2015:
	mov	w1, 8
	bl	BsShow
	cmp	w0, 255
	mov	w1, 8
	ldr	x0, [x19, 2344]
	beq	.L2016
	bl	BsGet
	add	w4, w21, w0
	mov	x1, x22
	str	w0, [x22, 4]
	add	w3, w20, w24
	mov	x2, x23
	mov	x0, x19
	bl	HEVC_Sei_Payload
	mov	w1, 0
	cbnz	w0, .L2023
.L2013:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2021:
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
.L2022:
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
.L2023:
	ldr	x1, .LC468
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	b	.L2013
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
	add	x22, x22, 96
	add	x21, x21, 3608
	b	.L2028
	.p2align 3
.L2025:
	ldr	x0, [x19, 2344]
	bl	BsIsByteAligned
	cbz	w0, .L2033
	mov	x0, x19
	bl	HEVC_More_Rbsp_Data
	mov	w20, w0
	cbz	w0, .L2034
.L2028:
	mov	x2, x21
	mov	x1, x22
	mov	x0, x19
	bl	HEVC_SeiMessage
	cbz	w0, .L2025
	ldr	x1, .LC470
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
.L2026:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2033:
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
.L2034:
	ldr	x0, [x19, 2344]
	mov	w1, 8
	bl	BsGet
	cmp	w0, 128
	beq	.L2026
	ldr	x1, .LC474
	mov	w2, w0
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	b	.L2026
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
	ldr	x0, [x19, 2344]
	mov	w1, 1
	add	x21, x24, 2560
	bl	BsGet
	strb	w0, [x24, 2560]
	mov	w1, 1
	ldr	x0, [x19, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 1]
	cbz	w0, .L2036
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x21, 2]
	ldr	x1, .LC476
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	cmp	w0, 63
	str	w0, [x21, 8]
	bhi	.L2051
	ldrb	w21, [x21, 2]
	cmp	w0, w21
	bcc	.L2042
	ldr	x25, .LC480
	.p2align 2
.L2041:
	add	x20, x22, x21, uxtw 2
	ldr	x0, [x19, 2344]
	add	x20, x20, 20480
	mov	w1, 3
	add	w21, w21, 1
	bl	BsGet
	str	w0, [x20, 2572]
	mov	x1, x25
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	str	w0, [x20, 2828]
	ldr	w0, [x24, 2568]
	cmp	w0, w21
	bcs	.L2041
.L2042:
	ldr	x0, [x19, 2344]
	bl	BsToNextByte
	strb	wzr, [x24, 2564]
.L2040:
	mov	x2, x23
	mov	x1, x22
	mov	x0, x19
	bl	HEVC_SeiMessage
	cbnz	w0, .L2052
	ldr	x0, [x19, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L2046
	ldr	x1, [x19, 2392]
	ldr	w1, [x1, 12]
	add	w1, w1, 3
	cmp	w1, w0
	bcc	.L2040
.L2046:
	mov	w0, 0
.L2038:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L2036:
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x21, 3]
	cbnz	w0, .L2042
	ldr	x0, [x19, 2344]
	mov	w1, 3
	mov	w20, 0
	mov	x25, x21
	bl	BsGet
	str	w0, [x21, 524]
	ldr	x1, .LC482
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	cmp	w0, 63
	str	w0, [x21, 528]
	bhi	.L2053
	.p2align 2
.L2047:
	ldr	x0, [x19, 2344]
	mov	w1, 6
	bl	BsGet
	add	x1, x25, x20, uxtw 2
	add	w20, w20, 1
	str	w0, [x1, 532]
	ldr	w0, [x25, 528]
	cmp	w0, w20
	bcs	.L2047
	b	.L2042
	.p2align 3
.L2052:
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
.L2051:
	ldr	x1, .LC478
	mov	w2, 64
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2038
.L2053:
	ldr	x1, .LC484
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2038
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
	add	x28, x20, 324
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
.L2055:
	ldr	w0, [x28, 1904]
	mov	w23, 0
	uxtw	x24, w26
	cbz	w0, .L2075
	lsl	x0, x24, 2
	sub	x24, x0, x24
	lsl	x0, x24, 1
	str	x0, [x29, 136]
	add	x0, x19, x0
	str	x0, [x29, 128]
.L2083:
	ldp	x1, x2, [x29, 128]
	uxtw	x24, w23
	ldr	x0, [x20, 2344]
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
	cbz	w26, .L2056
	add	x3, x20, 356
	cbz	w0, .L2088
	cmp	w26, 1
	bls	.L2081
	ldr	x1, .LC495
	ldr	x0, [x20, 2344]
	str	w2, [x29, 116]
	str	x3, [x29, 120]
	bl	HEVC_se_v
	add	w1, w0, 7
	cmp	w1, 254
	str	w0, [x19, 1660]
	ldr	w2, [x29, 116]
	ldr	x3, [x29, 120]
	bhi	.L2089
	add	x25, x19, x25, lsl 2
	add	w27, w0, 8
	str	w27, [x25, 1668]
.L2067:
	cmp	w2, wzr
	ble	.L2066
	sub	w25, w2, #1
	mov	x24, x3
	add	x25, x25, 1
	add	x25, x3, x25, lsl 2
	b	.L2071
	.p2align 3
.L2070:
	ldr	w0, [x24], 4
	and	w1, w1, 255
	sub	w27, w1, w27
	cmp	x24, x25
	strb	w27, [x21, x0]
	beq	.L2066
.L2071:
	ldr	x0, [x20, 2344]
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
	bls	.L2070
	ldr	x1, .LC501
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L2086:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2088:
	ldr	x1, .LC489
	ldr	x0, [x20, 2344]
	str	w2, [x29, 120]
	bl	HEVC_ue_v
	cmp	w0, w23
	ldr	w2, [x29, 120]
	bhi	.L2078
	uxtb	w0, w0
	strb	w0, [x27, 1585]
	sub	w0, w23, w0
	cmp	w26, 1
	uxtb	w0, w0
	strb	w0, [x27, 1609]
	bls	.L2061
	cmp	w0, w23
	beq	.L2080
	ldr	x1, [x29, 136]
	add	x1, x1, x0, uxtb
	add	x1, x19, x1, lsl 2
	ldr	w3, [x1, 1668]
.L2062:
	ldr	x1, [x29, 136]
	add	x1, x1, x24
	add	x1, x19, x1, lsl 2
	str	w3, [x1, 1668]
.L2061:
	cmp	w0, w23
	beq	.L2090
	ldr	x1, [x29, 136]
	add	x1, x1, x0, uxtb
	add	x1, x19, x1, lsl 6
	add	x1, x1, 49
.L2064:
	cbz	x1, .L2091
	ldr	x3, .LC487
	sxtw	x2, w2
	mov	x0, x21
	ldr	x3, [x3, 104]
	blr	x3
.L2066:
	ldr	w0, [x28, 1904]
	add	w23, w23, 1
	cmp	w0, w23
	bhi	.L2083
.L2075:
	add	w26, w26, 1
	add	x28, x28, 4
	cmp	w26, 4
	bne	.L2055
	mov	w0, 0
	b	.L2086
	.p2align 3
.L2081:
	mov	w27, 8
	b	.L2067
.L2080:
	mov	w3, 16
	b	.L2062
.L2090:
	mov	w1, w23
	mov	w0, w26
	str	w2, [x29, 120]
	bl	HEVC_GetScalingListDefaultAddress
	mov	x1, x0
	ldr	w2, [x29, 120]
	b	.L2064
.L2089:
	ldr	x1, .LC497
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2086
.L2091:
	ldr	x1, .LC493
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2086
.L2078:
	ldr	x1, .LC491
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2086
.L2056:
	cbz	w0, .L2076
	mov	w27, 8
	ldr	x3, [x29, 104]
	b	.L2067
.L2076:
	ldr	x1, .LC489
	ldr	x0, [x20, 2344]
	str	w2, [x29, 120]
	bl	HEVC_ue_v
	cmp	w23, w0
	ldr	w2, [x29, 120]
	bcc	.L2078
	add	x4, x19, x24
	uxtb	w0, w0
	strb	w0, [x4, 1585]
	sub	w0, w23, w0
	uxtb	w0, w0
	strb	w0, [x4, 1609]
	b	.L2061
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
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 1
	str	w0, [x19, 64]
	mov	w23, w0
	beq	.L2093
	cmp	w0, 3
	bhi	.L2227
	ldr	x1, .LC508
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L2223:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2227:
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
.L2093:
	ldr	x1, .LC510
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 8192
	str	w0, [x19, 72]
	bhi	.L2228
	ldr	x1, .LC514
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 4096
	str	w0, [x19, 76]
	bhi	.L2229
	ldr	w2, [x19, 760]
	ldr	w1, [x19, 72]
	cmp	w2, 120
	mul	w0, w0, w1
	beq	.L2099
	bgt	.L2100
	cmp	w2, 60
	beq	.L2163
	ble	.L2230
	cmp	w2, 90
	beq	.L2104
	cmp	w2, 93
	beq	.L2105
	cmp	w2, 63
	bne	.L2098
	mov	w1, 61440
	mov	w2, 245760
	b	.L2101
	.p2align 3
.L2100:
	cmp	w2, 156
	beq	.L2107
	bgt	.L2108
	cmp	w2, 150
	beq	.L2107
	cmp	w2, 153
	beq	.L2107
	cmp	w2, 123
	beq	.L2099
.L2098:
	ldr	x1, .LC518
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
	.p2align 3
.L2108:
	cmp	w2, 183
	beq	.L2109
	cmp	w2, 186
	beq	.L2109
	cmp	w2, 180
	bne	.L2098
.L2109:
	mov	w1, 8912896
	mov	w2, 35651584
	.p2align 2
.L2101:
	cmp	w0, w1
	bhi	.L2110
	mov	w0, 16
	str	w0, [x19, 236]
.L2111:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19, 80]
	cbz	w0, .L2114
	ldr	x1, .LC520
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	ldr	x21, .LC521
	ldr	w2, [x19, 64]
	ldr	x1, .LC523
	add	x2, x21, x2, lsl 2
	ldr	w2, [x2, 16]
	mul	w0, w2, w0
	str	w0, [x19, 84]
	ldr	x0, [x20, 2344]
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
	tbnz	w1, #31, .L2231
	ldr	x1, .LC527
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	ldr	w2, [x19, 64]
	ldr	x1, .LC529
	add	x2, x21, x2, lsl 2
	ldr	w2, [x2, 32]
	mul	w0, w2, w0
	str	w0, [x19, 92]
	ldr	x0, [x20, 2344]
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
	tbnz	w1, #31, .L2232
.L2114:
	ldr	x1, .LC533
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 8
	cmp	w0, 6
	str	w2, [x19, 100]
	bhi	.L2233
	ldr	x1, .LC537
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 8
	cmp	w0, 6
	str	w2, [x19, 108]
	bhi	.L2234
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
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 12
	bhi	.L2235
	add	w0, w0, 4
	mov	w1, 1
	stp	w0, w0, [x19, 128]
	mov	w21, 0
	ldr	x0, [x20, 2344]
	bl	BsGet
	ldr	x28, .LC545
	ldr	x27, .LC547
	ldr	x26, .LC549
	strb	w0, [x19, 3]
	b	.L2125
	.p2align 3
.L2119:
	ldr	w0, [x19, 56]
	add	w21, w21, 1
	cmp	w21, w0
	bhi	.L2124
.L2125:
	add	x22, x19, x21, uxtw 2
	ldr	x0, [x20, 2344]
	mov	x1, x28
	bl	HEVC_ue_v
	add	w0, w0, 1
	str	w0, [x22, 380]
	mov	x1, x27
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x22, 412]
	mov	x1, x26
	ldr	w24, [x22, 380]
	ldr	x0, [x20, 2344]
	ldr	w25, [x19, 380]
	bl	HEVC_ue_v
	str	w0, [x22, 444]
	ldrb	w0, [x19, 3]
	cbnz	w0, .L2119
	ldr	w2, [x19, 56]
	add	w21, w21, 1
	cmp	w21, w2
	bhi	.L2124
	.p2align 2
.L2201:
	add	x0, x19, x21, uxtw 2
	ldr	w1, [x19, 380]
	add	w21, w21, 1
	cmp	w21, w2
	str	w1, [x0, 380]
	ldr	w1, [x19, 412]
	str	w1, [x0, 412]
	ldr	w1, [x19, 444]
	str	w1, [x0, 444]
	bls	.L2201
.L2124:
	ldr	w0, [x19, 236]
	cmp	w24, w25
	csel	w24, w24, w25, cs
	ldr	x1, .LC551
	cmp	w24, w0
	csel	w24, w24, w0, ls
	str	w24, [x19, 236]
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 136]
	bhi	.L2236
	ldr	x1, .LC555
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 144]
	bhi	.L2237
	ldr	w1, [x19, 136]
	add	w1, w1, 3
	str	w1, [x19, 140]
	add	w0, w0, w1
	str	w0, [x19, 148]
	sub	w2, w0, #4
	cmp	w2, 2
	bhi	.L2238
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
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 168]
	bhi	.L2239
	ldr	x1, .LC565
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 176]
	bhi	.L2240
	ldr	w2, [x19, 168]
	ldr	w3, [x19, 140]
	add	w1, w2, 2
	str	w1, [x19, 172]
	cmp	w1, w3
	bcs	.L2241
	add	w1, w0, w1
	str	w1, [x19, 180]
	cmp	w1, 5
	bhi	.L2135
	ldr	w0, [x19, 148]
	cmp	w1, w0
	bhi	.L2135
	ldr	x1, .LC573
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 200]
	ldr	w1, [x19, 148]
	ldr	w2, [x19, 172]
	sub	w1, w1, w2
	cmp	w0, w1
	bhi	.L2242
	ldr	x1, .LC577
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 208]
	ldr	w1, [x19, 148]
	ldr	w2, [x19, 172]
	sub	w1, w1, w2
	cmp	w0, w1
	bhi	.L2243
	ldr	w1, [x19, 200]
	add	w0, w0, 1
	mov	w4, 1
	str	w0, [x19, 212]
	add	w0, w1, w4
	str	w0, [x19, 204]
	str	wzr, [x20, 260]
	ldr	w2, [x19, 144]
	ldr	w1, [x19, 160]
	ldr	w0, [x19, 172]
	lsr	w1, w1, w2
	lsl	w0, w4, w0
	cmp	w1, w0
	bhi	.L2140
	b	.L2244
	.p2align 3
.L2165:
	mov	w23, w3
.L2140:
	str	w23, [x20, 260]
	add	w3, w23, 1
	ldr	w0, [x19, 172]
	ldr	w2, [x19, 144]
	ldr	w1, [x19, 160]
	add	w0, w23, w0
	lsl	w0, w4, w0
	lsr	w1, w1, w2
	cmp	w1, w0
	bhi	.L2165
.L2139:
	add	w23, w23, w2
	mov	w21, 1
	stp	w23, wzr, [x19, 216]
	str	w21, [x19, 224]
	mov	w1, w21
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	cbz	w0, .L2141
	ldr	x0, [x20, 2344]
	mov	w1, w21
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 6]
	cbnz	w0, .L2245
.L2141:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 8]
	cbnz	w0, .L2142
	ldr	w2, [x19, 76]
	cmp	w2, 64
	bls	.L2246
.L2142:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x19, 116]
	cbz	w0, .L2143
	ldr	x0, [x20, 2344]
	mov	w1, 4
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x19, 120]
	mov	w1, 4
	ldr	x0, [x20, 2344]
	bl	BsGet
	add	w0, w0, 1
	str	w0, [x19, 124]
	ldr	x1, .LC586
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 2
	str	w0, [x19, 184]
	bhi	.L2247
.L2144:
	ldr	x1, .LC590
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 2
	str	w0, [x19, 192]
	bhi	.L2248
.L2145:
	ldr	w1, [x19, 184]
	add	w1, w1, 3
	str	w1, [x19, 188]
	add	w0, w1, w0
	str	w0, [x19, 196]
	cmp	w0, 5
	bhi	.L2146
	ldr	w1, [x19, 148]
	cmp	w0, w1
	bhi	.L2146
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 9]
.L2143:
	ldr	x1, .LC596
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 64
	str	w0, [x19, 228]
	bhi	.L2148
	mov	w21, 0
	cbnz	w0, .L2200
	b	.L2154
	.p2align 3
.L2152:
	ldr	w0, [x19, 228]
	cmp	w21, w0
	bcs	.L2154
.L2200:
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
	cbz	w0, .L2152
	ldr	x1, .LC600
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
	.p2align 3
.L2105:
	mov	w1, 245760
	mov	w2, 983040
	b	.L2101
	.p2align 3
.L2110:
	cmp	w0, w2, lsr 1
	bhi	.L2112
	mov	w0, 12
	str	w0, [x19, 236]
	b	.L2111
	.p2align 3
.L2154:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 10]
	cbz	w0, .L2151
	ldr	x1, .LC602
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 32
	str	w0, [x19, 232]
	bhi	.L2157
	mov	w22, 0
	cbz	w0, .L2151
	.p2align 2
.L2199:
	ldr	w1, [x19, 128]
	uxtw	x21, w22
	ldr	x0, [x20, 2344]
	add	w22, w22, 1
	bl	BsGet
	add	x2, x19, x21, lsl 2
	add	x21, x19, x21
	mov	w1, 1
	str	w0, [x2, 248]
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x21, 16]
	ldr	w0, [x19, 232]
	cmp	w22, w0
	bcc	.L2199
.L2151:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 11]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 12]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 13]
	cbz	w0, .L2156
	mov	x2, x19
	add	x1, x19, 2624
	mov	x0, x20
	bl	HEVC_DecVuiParam
	cbnz	w0, .L2249
.L2156:
	ldp	w3, w4, [x19, 72]
	ldr	w1, [x19, 2652]
	ldr	w2, [x19, 2656]
	ldr	w0, [x19, 2648]
	bl	HEVCGetDar
	str	w0, [x19, 2644]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 14]
	cbnz	w0, .L2222
.L2161:
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
.L2162:
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 15]
.L2222:
	ldr	x0, [x20, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L2161
	ldr	x2, [x20, 2392]
	mov	w1, 1
	ldr	w2, [x2, 12]
	add	w2, w2, 3
	cmp	w2, w0
	bcc	.L2162
	b	.L2161
	.p2align 3
.L2112:
	lsl	w1, w2, 2
	sub	w2, w1, w2
	cmp	w0, w2, lsr 2
	bhi	.L2113
	mov	w0, 8
	str	w0, [x19, 236]
	b	.L2111
	.p2align 3
.L2230:
	cbz	w2, .L2103
	cmp	w2, 30
	bne	.L2098
.L2103:
	mov	w1, 9216
	mov	w2, 36864
	b	.L2101
	.p2align 3
.L2246:
	ldr	x1, .LC583
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC584
	ldr	x4, [x0]
	cbz	x4, .L2166
	ldr	w0, [x20, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, -1
	b	.L2223
	.p2align 3
.L2163:
	mov	w1, 30720
	mov	w2, 122880
	b	.L2101
	.p2align 3
.L2099:
	mov	w1, 32768
	mov	w2, 2228224
	movk	w1, 0x8, lsl 16
	b	.L2101
	.p2align 3
.L2104:
	mov	w1, 7168
	mov	w2, 28672
	movk	w1, 0x2, lsl 16
	movk	w2, 0x8, lsl 16
	b	.L2101
	.p2align 3
.L2113:
	mov	w0, 6
	str	w0, [x19, 236]
	b	.L2111
	.p2align 3
.L2107:
	mov	w1, 2228224
	mov	w2, 8912896
	b	.L2101
.L2245:
	add	x1, x19, 860
	mov	x0, x20
	bl	HEVC_DecScalingListData
	cbz	w0, .L2141
	ldr	x1, .LC581
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
	.p2align 3
.L2135:
	ldr	x1, .LC571
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2248:
	ldr	x1, .LC592
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 192]
	b	.L2145
.L2247:
	ldr	x1, .LC588
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2144
.L2244:
	mov	w23, 0
	b	.L2139
.L2228:
	ldr	x1, .LC512
	mov	w0, w23
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2166:
	mov	w0, -1
	b	.L2223
.L2235:
	ldr	x1, .LC543
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2236:
	ldr	x1, .LC553
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2239:
	ldr	x1, .LC563
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2146:
	ldr	x1, .LC594
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2242:
	ldr	x1, .LC575
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2229:
	ldr	x1, .LC516
	mov	w0, w23
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2233:
	ldr	x1, .LC535
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2238:
	ldr	x1, .LC559
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2237:
	ldr	x1, .LC557
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2234:
	ldr	x1, .LC539
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2240:
	ldr	x1, .LC567
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2241:
	ldr	x1, .LC569
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2243:
	ldr	x1, .LC579
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2148:
	ldr	x1, .LC598
	mov	w2, 64
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2231:
	ldr	x1, .LC525
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2232:
	ldr	x1, .LC531
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2157:
	ldr	x1, .LC604
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
.L2249:
	ldr	x1, .LC606
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2223
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
	ldr	x0, [x19, 2344]
	mov	w1, 4
	bl	BsGet
	mov	w25, w0
	ldr	w2, [x19, 148]
	cmp	w2, w0
	bls	.L2261
	ldr	x0, [x19, 2344]
	mov	w1, 3
	bl	BsGet
	cmp	w0, 8
	mov	w24, w0
	bhi	.L2262
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	mov	w27, w0
	mov	w3, w24
	mov	w2, 1
	add	x1, x29, 112
	mov	x0, x19
	bl	HEVC_DecPTL
	cbnz	w0, .L2263
	ldr	x1, .LC616
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	mov	w23, w0
	ldr	w2, [x19, 152]
	cmp	w0, w2
	bcs	.L2264
	mov	w20, 26164
	ldr	x1, [x19, 2360]
	uxtb	w27, w27
	umull	x20, w0, w20
	add	x1, x1, x20
	ldrb	w0, [x1, 1]
	cbnz	w0, .L2265
	str	w25, [x1, 52]
	mov	x2, 384
	add	x1, x29, 112
	ldr	x0, [x19, 2360]
	add	x0, x0, x20
	str	w24, [x0, 56]
	ldr	x0, [x19, 2360]
	add	x0, x0, x20
	strb	w27, [x0, 2]
	ldr	x0, [x19, 2360]
	ldr	x3, [x21, 104]
	add	x0, x0, x20
	add	x0, x0, 476
	blr	x3
	ldr	x1, [x19, 2360]
	mov	x0, x19
	add	x1, x1, x20
	str	w23, [x1, 60]
	ldr	x1, [x19, 2360]
	add	x1, x1, x20
	bl	HEVC_ProcessSPS
	mov	w24, w0
	cbnz	w0, .L2266
	ldr	x1, [x19, 2360]
	mov	w3, 1
	add	x0, x19, 24576
	mov	x2, 26164
	add	x0, x0, 3608
	strb	w3, [x1, x20]
	ldr	x1, [x19, 2360]
	add	x1, x1, x20
	strb	w3, [x1, 1]
	ldr	x1, [x19, 2360]
	ldr	x3, [x21, 104]
	add	x1, x1, x20
	blr	x3
.L2252:
	mov	w0, w24
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 496
	ret
	.p2align 3
.L2265:
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
	cbnz	w0, .L2267
	ldr	x0, [x19, 2360]
	mov	w2, 1
	ldr	x3, [x21, 112]
	mov	x1, x22
	strb	w2, [x26, 3328]
	add	x0, x0, x20
	strb	w2, [x22, 1]
	mov	x2, 26164
	blr	x3
	ldr	x1, [x19, 2360]
	add	x0, x19, 24576
	ldr	x3, [x21, 104]
	mov	x2, 26164
	add	x1, x1, x20
	add	x0, x0, 3608
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
.L2264:
	ldr	x1, .LC618
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 152]
	cmp	w23, w1
	bcc	.L2260
	ldr	x0, .LC619
	ldr	x4, [x0]
	cbz	x4, .L2260
	ldr	w0, [x19, 276]
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
.L2260:
	mov	w24, -1
	b	.L2252
	.p2align 3
.L2261:
	ldr	x1, .LC610
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L2252
	.p2align 3
.L2262:
	ldr	x1, .LC612
	mov	w2, 8
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L2252
	.p2align 3
.L2263:
	ldr	x1, .LC614
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L2252
	.p2align 3
.L2266:
	ldr	x1, .LC623
	mov	w2, w23
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	ldr	x0, [x19, 2360]
	mov	w1, 1
	strb	w1, [x0, x20]
	ldr	x0, [x19, 2360]
	add	x20, x0, x20
	strb	wzr, [x20, 1]
	b	.L2252
	.p2align 3
.L2267:
	ldr	x1, [x19, 2360]
	add	x0, x19, 24576
	ldr	x3, [x21, 104]
	mov	x2, 26164
	add	x1, x1, x20
	add	x0, x0, 3608
	mov	w24, -1
	blr	x3
	ldr	x1, .LC621
	mov	w2, w23
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2252
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
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	str	w0, [x19, 32]
	tbnz	w0, #31, .L2269
	ldr	w1, [x20, 152]
	cmp	w0, w1
	bge	.L2269
	ldr	x1, [x20, 2360]
	mov	w21, 26164
	smaddl	x21, w0, w21, x1
	ldrb	w1, [x21, 1]
	cbz	w1, .L2377
	ldr	x0, [x20, 2344]
	mov	w1, w22
	bl	BsGet
	strb	w0, [x19, 2]
	mov	w1, w22
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 3
	ldr	x0, [x20, 2344]
	bl	BsGet
	str	w0, [x19, 36]
	mov	w1, w22
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 3]
	mov	w1, w22
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 5]
	ldr	x1, .LC632
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	add	w0, w0, 1
	cmp	w0, 15
	str	w0, [x19, 40]
	bhi	.L2378
	ldr	x1, .LC636
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	add	w0, w0, 1
	cmp	w0, 15
	str	w0, [x19, 44]
	bhi	.L2379
	ldr	x1, .LC640
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w2, w0, 26
	str	w2, [x19, 48]
	mov	w0, 51
	ldr	w3, [x21, 104]
	neg	w1, w3
	cmp	w2, w1
	ccmp	w2, w0, 0, ge
	bgt	.L2380
	ldr	x0, [x20, 2344]
	mov	w1, w22
	bl	BsGet
	strb	w0, [x19, 6]
	mov	w1, w22
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, w22
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 8]
	cbnz	w0, .L2381
	mov	w0, 0
	str	wzr, [x19, 52]
.L2278:
	str	w0, [x19, 56]
	ldr	x1, .LC648
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x19, 60]
	bhi	.L2382
	ldr	x1, .LC652
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x19, 64]
	bhi	.L2383
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 9]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 10]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 11]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 12]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 13]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 14]
	ldrb	w1, [x19, 13]
	cbnz	w1, .L2384
	ldr	w0, [x21, 240]
	strh	w0, [x19, 176]
	ldr	w0, [x21, 244]
	strh	w0, [x19, 216]
.L2315:
	ldr	w3, [x19, 68]
	cmp	w3, wzr
	ble	.L2320
.L2314:
	ldr	w4, [x21, 160]
	add	x0, x19, 176
	mov	w1, 0
	.p2align 2
.L2319:
	ldrsh	w2, [x0]
	add	w1, w1, 1
	add	x0, x0, 2
	mul	w2, w2, w4
	cmp	w2, 64
	bhi	.L2318
	ldr	w5, [x21, 72]
	cmp	w5, 64
	bhi	.L2385
.L2318:
	cmp	w1, w3
	blt	.L2319
.L2320:
	ldr	w1, [x19, 72]
	cmp	w1, wzr
	ble	.L2317
	ldrsh	w2, [x19, 216]
	ldr	w3, [x21, 160]
	mul	w2, w2, w3
	cmp	w2, 63
	bls	.L2321
	sub	w1, w1, #1
	mov	x0, x19
	add	x1, x19, x1, lsl 1
	b	.L2323
	.p2align 3
.L2324:
	ldrsh	w2, [x0, 218]
	add	x0, x0, 2
	mul	w2, w2, w3
	cmp	w2, 63
	bls	.L2321
.L2323:
	cmp	x1, x0
	bne	.L2324
.L2317:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 17]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 18]
	cbnz	w0, .L2386
.L2326:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 21]
	cbz	w0, .L2329
	add	x1, x19, 260
	mov	x0, x20
	bl	HEVC_DecScalingListData
	cbnz	w0, .L2387
.L2329:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 22]
	ldr	x1, .LC695
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 2
	cmp	w2, 1
	str	w2, [x19, 84]
	ble	.L2330
	ldr	w0, [x21, 148]
	cmp	w2, w0
	bhi	.L2330
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 23]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 24]
	cbz	w0, .L2333
	ldr	x0, [x20, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	ble	.L2333
	.p2align 2
.L2388:
	ldr	x2, [x20, 2392]
	mov	w1, 1
	ldr	w2, [x2, 12]
	add	w2, w2, 3
	cmp	w2, w0
	bcs	.L2333
	ldr	x0, [x20, 2344]
	bl	BsGet
	strb	w0, [x19, 25]
	ldr	x0, [x20, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	bgt	.L2388
.L2333:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2381:
	ldr	x1, .LC644
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	cmp	w0, 3
	str	w0, [x19, 52]
	bls	.L2278
	ldr	x1, .LC646
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
	.p2align 3
.L2384:
	ldr	x1, .LC656
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w0, 19
	str	w2, [x19, 68]
	bhi	.L2389
	ldr	x1, .LC660
	ldr	x0, [x20, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w0, 21
	str	w2, [x19, 72]
	bhi	.L2390
	ldr	w3, [x19, 68]
	cmp	w3, 10
	bgt	.L2391
	cmp	w2, 11
	ble	.L2286
	ldr	x1, .LC667
	mov	w3, w2
	mov	w2, 11
.L2374:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC665
	ldr	x4, [x0]
	cbz	x4, .L2369
.L2371:
	ldr	w0, [x20, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, -1
.L2365:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2321:
	ldr	x1, .LC683
.L2373:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC665
	ldr	x4, [x0]
	cbnz	x4, .L2371
.L2369:
	mov	w0, -1
.L2396:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2385:
	ldr	x1, .LC681
	b	.L2373
	.p2align 3
.L2380:
	ldr	x1, .LC642
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2286:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 15]
	cbnz	w0, .L2392
	ldr	w0, [x19, 68]
	sub	w1, w0, #1
	cmp	w1, wzr
	ble	.L2291
	ldr	x23, .LC669
	mov	w22, 0
	b	.L2295
	.p2align 3
.L2394:
	cmp	w3, w4
	bhi	.L2293
	ldr	w0, [x19, 68]
	add	w22, w22, 1
	sub	w1, w0, #1
	cmp	w1, w22
	ble	.L2393
.L2295:
	ldr	x0, [x20, 2344]
	mov	x1, x23
	bl	HEVC_ue_v
	add	w0, w0, 1
	add	x1, x19, x22, sxtw 1
	sxth	w3, w0
	strh	w3, [x1, 92]
	ldr	w4, [x21, 240]
	tbz	w3, #31, .L2394
.L2293:
	ldr	x1, .LC671
	mov	w2, w22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2391:
	ldr	x1, .LC664
	mov	w2, 10
	b	.L2374
.L2386:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 19]
	mov	w1, 1
	ldr	x0, [x20, 2344]
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 20]
	cbnz	w0, .L2326
	ldr	x1, .LC685
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 76]
	bhi	.L2395
.L2328:
	ldr	x1, .LC689
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 80]
	bls	.L2326
	ldr	x1, .LC691
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x19, 80]
	b	.L2326
.L2392:
	ldr	w0, [x19, 68]
.L2297:
	cmp	w0, wzr
	add	x5, x19, 176
	mov	w3, 0
	ble	.L2290
	.p2align 2
.L2349:
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
	bne	.L2349
.L2290:
	ldr	w4, [x19, 72]
	add	x6, x19, 216
	mov	w3, 0
	cmp	w4, wzr
	ble	.L2305
	.p2align 2
.L2348:
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
	bne	.L2348
.L2305:
	cmp	w0, 1
	bne	.L2313
	ldr	w3, [x19, 72]
	cmp	w3, 1
	beq	.L2314
.L2313:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x19, 16]
	b	.L2315
.L2269:
	ldr	x1, .LC628
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2377:
	ldr	x1, .LC630
	mov	w2, w0
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2378:
	ldr	x1, .LC634
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2379:
	ldr	x1, .LC638
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2382:
	ldr	x1, .LC650
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2383:
	ldr	x1, .LC654
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2330:
	ldr	x1, .LC697
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2396
.L2393:
	ldr	w1, [x19, 72]
	sub	w1, w1, #1
	cmp	w1, wzr
	ble	.L2303
.L2296:
	ldr	x23, .LC673
	mov	w22, 0
	b	.L2302
	.p2align 3
.L2398:
	cmp	w3, w4
	bhi	.L2300
	ldr	w0, [x19, 72]
	add	w22, w22, 1
	sub	w0, w0, #1
	cmp	w0, w22
	ble	.L2397
.L2302:
	ldr	x0, [x20, 2344]
	mov	x1, x23
	bl	HEVC_ue_v
	add	w0, w0, 1
	add	x1, x19, x22, sxtw 1
	sxth	w3, w0
	strh	w3, [x1, 132]
	ldr	w4, [x21, 244]
	tbz	w3, #31, .L2398
.L2300:
	ldr	x1, .LC675
	mov	w2, w22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2389:
	ldr	x1, .LC658
	mov	w3, 20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2395:
	ldr	x1, .LC687
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	str	wzr, [x19, 76]
	b	.L2328
.L2397:
	ldr	w0, [x19, 68]
.L2303:
	ldrb	w1, [x19, 15]
	cbnz	w1, .L2297
	add	x5, x19, x0, sxtw 1
	sub	w2, w0, #1
	ldrsh	w3, [x21, 240]
	cmp	w2, wzr
	strh	w3, [x5, 174]
	ble	.L2308
	sub	w6, w0, #2
	add	x4, x19, 92
	add	x6, x6, 47
	add	x6, x19, x6, lsl 1
	.p2align 2
.L2309:
	ldrsh	w3, [x4]
	add	x4, x4, 2
	strh	w3, [x4, 82]
	cmp	x4, x6
	ldrh	w1, [x5, 174]
	sub	w3, w1, w3
	sxth	w3, w3
	strh	w3, [x5, 174]
	bne	.L2309
.L2308:
	cmp	w3, wzr
	ble	.L2399
	ldr	w1, [x19, 72]
	ldrsh	w3, [x21, 244]
	sub	w2, w1, #1
	add	x5, x19, x1, sxtw 1
	cmp	w2, wzr
	strh	w3, [x5, 214]
	ble	.L2311
	sub	w6, w1, #2
	add	x4, x19, 132
	add	x6, x6, 67
	add	x6, x19, x6, lsl 1
	.p2align 2
.L2312:
	ldrsh	w3, [x4]
	add	x4, x4, 2
	strh	w3, [x4, 82]
	cmp	x6, x4
	ldrh	w1, [x5, 214]
	sub	w3, w1, w3
	sxth	w3, w3
	strh	w3, [x5, 214]
	bne	.L2312
.L2311:
	cmp	w3, wzr
	bgt	.L2305
	ldr	x1, .LC679
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
	.p2align 3
.L2390:
	ldr	x1, .LC662
	mov	w3, 22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2387:
	ldr	x1, .LC693
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
.L2291:
	ldr	w1, [x19, 72]
	sub	w1, w1, #1
	cmp	w1, wzr
	bgt	.L2296
	add	x1, x19, x0, sxtw 1
	ldrsh	w3, [x21, 240]
	sub	w2, w0, #1
	strh	w3, [x1, 174]
	b	.L2308
.L2399:
	ldr	x1, .LC677
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2365
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
	ldr	x0, [x19, 2344]
	bl	HEVC_ue_v
	mov	w21, w0
	ldr	w3, [x19, 156]
	cmp	w0, w3
	bcs	.L2406
	mov	w20, 2024
	umull	x20, w0, w20
	ldr	x0, [x19, 2368]
	add	x1, x0, x20
	ldrb	w0, [x0, x20]
	cbnz	w0, .L2407
	str	w21, [x1, 28]
	mov	x0, x19
	ldr	x1, [x19, 2368]
	add	x1, x1, x20
	bl	HEVC_ProcessPPS
	mov	w22, w0
	cbnz	w0, .L2408
	ldr	x0, [x19, 2368]
	mov	w1, 1
	add	x0, x0, x20
	strb	w1, [x0, 1]
	ldr	x0, [x19, 2368]
	strb	w1, [x0, x20]
.L2402:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2407:
	ldr	x24, .LC702
	mov	x0, x19
	add	x23, x24, 3328
	mov	x1, x23
	str	w21, [x23, 28]
	bl	HEVC_ProcessPPS
	mov	w22, w0
	cbnz	w0, .L2409
	ldr	x3, .LC705
	mov	w1, 1
	ldr	x0, [x19, 2368]
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
.L2409:
	ldr	x1, .LC704
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2402
	.p2align 3
.L2406:
	ldr	x1, .LC701
	mov	w2, w0
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2402
.L2408:
	ldr	x1, .LC704
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x0, [x19, 2368]
	mov	w1, 1
	add	x0, x0, x20
	strb	w1, [x0, 1]
	ldr	x0, [x19, 2368]
	strb	wzr, [x0, x20]
	b	.L2402
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
	ldr	x2, [x20, 2368]
	mov	w4, 2024
	ldr	w1, [x26, 1852]
	ldr	w0, [x26, 1868]
	cmp	w1, 1
	smaddl	x2, w0, w4, x2
	beq	.L2483
	cbnz	w1, .L2414
	ldrb	w0, [x2, 11]
	cbz	w0, .L2414
.L2412:
	ldr	x1, .LC708
	add	x23, x26, 3384
	ldr	x0, [x20, 2344]
	ldr	w21, [x2, 32]
	ldr	x19, [x20, 2360]
	bl	HEVC_ue_v
	cmp	w0, 7
	str	w0, [x26, 3384]
	bhi	.L2484
	mov	w22, 26164
	smaddl	x22, w21, w22, x19
	ldr	w1, [x22, 64]
	cbnz	w1, .L2485
	ldr	w1, [x23, 4]
.L2418:
	add	w0, w0, w1
	str	w0, [x26, 4416]
	cmp	w0, 7
	bhi	.L2419
	ldr	w0, [x26, 1960]
	mov	w19, 0
	add	x21, x26, 3384
	cbz	w0, .L2427
	.p2align 2
.L2469:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 8]
	ldr	w0, [x26, 1960]
	cmp	w0, w19
	bhi	.L2469
	ldr	w1, [x22, 64]
	cbz	w1, .L2423
	mov	w19, 0
	add	x21, x26, 3384
	cbz	w0, .L2427
	.p2align 2
.L2468:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 72]
	ldr	w0, [x26, 1960]
	cmp	w0, w19
	bhi	.L2468
.L2423:
	mov	w27, 0
	mov	w21, 1
	cbnz	w0, .L2467
	b	.L2427
	.p2align 3
.L2429:
	add	x23, x23, 57344
	ldr	w0, [x26, 3384]
	lsl	w0, w21, w0
	str	w0, [x23, 324]
	str	wzr, [x24, 3584]
.L2431:
	ldr	w0, [x22, 64]
	cbz	w0, .L2433
	ldr	w0, [x24, 3456]
	cbnz	w0, .L2434
	lsl	x0, x19, 3
	add	x19, x26, 3384
	add	x2, x20, x0
	ldr	w1, [x26, 4416]
	add	x2, x2, 57344
	add	x19, x19, x0
	lsl	w0, w21, w1
	str	wzr, [x2, 516]
	str	w0, [x2, 388]
	str	w0, [x19, 1104]
	str	wzr, [x19, 1232]
.L2433:
	ldr	w0, [x26, 1960]
	add	w27, w27, 1
	cmp	w0, w27
	bls	.L2427
.L2467:
	uxtw	x19, w27
	add	x23, x20, x19, lsl 2
	add	x24, x23, 53248
	ldr	w0, [x24, 3392]
	cbz	w0, .L2429
	ldr	x1, .LC718
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 128
	cmp	w1, 255
	str	w0, [x24, 3520]
	bhi	.L2486
	ldr	w2, [x26, 3384]
	add	x23, x23, 57344
	ldr	x1, .LC722
	lsl	w2, w21, w2
	add	w0, w2, w0
	str	w0, [x23, 324]
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 128
	cmp	w1, 255
	str	w0, [x24, 3584]
	bls	.L2431
	ldr	x1, .LC724
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
	.p2align 3
.L2483:
	ldrb	w0, [x2, 10]
	cbnz	w0, .L2412
.L2414:
	mov	w0, 0
.L2413:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2434:
	add	x19, x19, 4096
	ldr	x1, .LC726
	add	x5, x19, 3016
	ldr	x0, [x20, 2344]
	add	x19, x19, 3017
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
	bhi	.L2487
.L2435:
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 512
	cmp	w1, 1023
	str	w0, [x28, 128]
	bhi	.L2488
	ldr	w6, [x26, 4416]
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
	beq	.L2433
	ldr	x1, .LC726
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w2, w0, 128
	cmp	w2, 255
	str	w0, [x28]
	ldr	x1, .LC730
	bls	.L2435
.L2487:
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
.L2427:
	ldr	w0, [x26, 1852]
	cbnz	w0, .L2414
	ldr	w0, [x26, 1964]
	cbz	w0, .L2414
	mov	w19, 0
	add	x21, x26, 3384
	.p2align 2
.L2441:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 520]
	ldr	w0, [x26, 1964]
	cmp	w0, w19
	bhi	.L2441
	ldr	w1, [x22, 64]
	cbnz	w1, .L2489
.L2442:
	mov	w28, 0
	mov	w21, 1
	cbnz	w0, .L2465
	b	.L2414
	.p2align 3
.L2446:
	add	x19, x19, 57344
	ldr	w0, [x26, 3384]
	lsl	w0, w21, w0
	str	wzr, [x19]
	str	w0, [x19, 644]
.L2448:
	ldr	w0, [x22, 64]
	cbz	w0, .L2450
	ldr	w0, [x24, 3968]
	cbnz	w0, .L2451
	lsl	x23, x23, 3
	add	x1, x26, 3384
	add	x1, x1, x23
	add	x23, x20, x23
	add	x0, x23, 57344
	ldr	w2, [x26, 4416]
	lsl	w2, w21, w2
	str	wzr, [x0, 836]
	str	w2, [x0, 708]
	str	w2, [x1, 1424]
	str	wzr, [x1, 1552]
.L2450:
	ldr	w0, [x26, 1964]
	add	w28, w28, 1
	cmp	w0, w28
	bls	.L2414
.L2465:
	uxtw	x23, w28
	add	x19, x20, x23, lsl 2
	add	x24, x19, 53248
	ldr	w0, [x24, 3904]
	cbz	w0, .L2446
	ldr	x1, .LC734
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 128
	cmp	w1, 255
	str	w0, [x24, 4032]
	bhi	.L2490
	ldr	w1, [x26, 3384]
	add	x19, x19, 57344
	lsl	w1, w21, w1
	add	w0, w1, w0
	str	w0, [x19, 644]
	ldr	x1, .LC738
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	str	w0, [x19]
	add	w0, w0, 128
	cmp	w0, 255
	bls	.L2448
	ldr	x1, .LC740
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
	.p2align 3
.L2451:
	add	x23, x23, 4096
	mov	w25, 128
	add	x5, x23, 3080
	add	x23, x23, 3081
	mov	w24, 127
	add	x19, x20, x23, lsl 3
	add	x27, x20, x5, lsl 3
	mov	w23, -128
.L2454:
	ldr	x1, .LC742
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	str	w0, [x27]
	add	w0, w0, 128
	ldr	x1, .LC746
	cmp	w0, 255
	bhi	.L2491
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	add	w1, w0, 512
	cmp	w1, 1023
	str	w0, [x27, 128]
	bhi	.L2492
	ldr	w6, [x26, 4416]
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
	bne	.L2454
	b	.L2450
	.p2align 3
.L2485:
	ldr	x1, .LC712
	ldr	x0, [x20, 2344]
	bl	HEVC_se_v
	mov	w1, w0
	add	w0, w0, 7
	str	w1, [x23, 4]
	cmp	w0, 14
	bhi	.L2417
	ldr	w0, [x26, 3384]
	b	.L2418
	.p2align 3
.L2488:
	ldr	x1, .LC732
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
.L2489:
	mov	w19, 0
	add	x21, x26, 3384
	cbz	w0, .L2414
	.p2align 2
.L2466:
	ldr	x0, [x20, 2344]
	mov	w1, 1
	bl	BsGet
	add	x1, x21, x19, uxtw 2
	add	w19, w19, 1
	str	w0, [x1, 584]
	ldr	w0, [x26, 1964]
	cmp	w0, w19
	bhi	.L2466
	b	.L2442
.L2486:
	ldr	x1, .LC720
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
.L2491:
	ldr	x1, .LC744
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
.L2492:
	ldr	x1, .LC748
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
.L2490:
	ldr	x1, .LC736
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
.L2419:
	ldr	x1, .LC716
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
.L2484:
	ldr	x1, .LC710
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
.L2417:
	ldr	x1, .LC714
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2413
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
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	ldr	w23, [x27, 1864]
	strb	w0, [x27, 1793]
	sub	w0, w23, #16
	str	w0, [x29, 132]
	cmp	w0, 5
	bls	.L2759
.L2494:
	ldr	x1, .LC750
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	str	w0, [x27, 1868]
	ldr	w4, [x28, 156]
	tbnz	w0, #31, .L2496
	cmp	w0, w4
	bge	.L2496
	mov	x0, x28
	bl	HEVC_VpsSpsPpsCheck
	sxtw	x22, w0
	cbnz	w22, .L2760
	ldr	x0, [x28, 2368]
	mov	w3, 2024
	ldr	w20, [x27, 1868]
	mov	w2, 26164
	ldr	x1, [x28, 2360]
	smaddl	x20, w20, w3, x0
	ldr	w21, [x20, 32]
	ldrb	w0, [x20, 2]
	smaddl	x21, w21, w2, x1
	cbnz	w0, .L2761
.L2502:
	strb	wzr, [x27, 1795]
.L2503:
	ldp	w2, w3, [x21, 160]
	cbz	w2, .L2505
	cbz	w3, .L2505
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
	ble	.L2507
	.p2align 2
.L2508:
	add	w1, w1, 1
	lsl	w0, w2, w1
	cmp	w19, w0
	bgt	.L2508
.L2507:
	ldrb	w2, [x27, 1793]
	mov	w0, 0
	cbz	w2, .L2762
.L2509:
	mul	w19, w19, w24
	str	w0, [x27, 1944]
	str	w19, [x27, 1948]
	ldrb	w0, [x27, 1795]
	ldr	w1, [x28, 188]
	str	w1, [x27, 1840]
	cbz	w0, .L2763
.L2512:
	ldr	w0, [x20, 12]
	tst	w0, 16776960
	bne	.L2764
	str	wzr, [x27, 1932]
.L2620:
	ldrb	w0, [x20, 23]
	cbnz	w0, .L2624
.L2630:
	ldr	w0, [x27, 1944]
	cbnz	w0, .L2765
	ldrb	w0, [x28, 1]
	cbz	w0, .L2632
	ldr	w0, [x29, 132]
	cmp	w0, 5
	bhi	.L2633
	mov	w0, 1
	strb	w0, [x28, 3]
	ldr	w0, [x27, 1840]
	cmp	w0, wzr
	ldr	w0, [x29, 132]
	ccmp	w0, 5, 2, ne
	bhi	.L2633
.L2776:
	ldr	w0, [x27, 1864]
	cmp	w0, 21
	beq	.L2766
	.p2align 2
.L2633:
	strb	wzr, [x28, 2]
.L2636:
	mov	x0, x28
	bl	HEVC_IsNewPic
	str	w0, [x27, 3380]
	mov	x0, x28
	bl	HEVC_ReadByteAlignment
	cmp	w0, wzr
	csetm	w0, ne
.L2742:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2761:
	ldrb	w0, [x27, 1793]
	cbnz	w0, .L2502
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1795]
	b	.L2503
	.p2align 3
.L2763:
	ldr	w0, [x20, 36]
	cbz	w0, .L2517
	ldr	x24, .LC763
	mov	w19, 0
	.p2align 2
.L2516:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	add	w19, w19, w1
	bl	BsGet
	mov	w2, w0
	mov	x1, x24
	mov	w0, 19
	bl	dprint_vfmw
	ldr	w0, [x20, 36]
	cmp	w0, w19
	bhi	.L2516
.L2517:
	ldr	x1, .LC761
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	cmp	w0, 2
	str	w0, [x27, 1852]
	bhi	.L2767
	ldrb	w0, [x20, 4]
	cbnz	w0, .L2768
	mov	w0, 1
	strb	w0, [x27, 1792]
.L2519:
	sub	w23, w23, #19
	cmp	w23, 1
	bls	.L2520
	ldr	w1, [x21, 132]
	ldr	x0, [x28, 2344]
	bl	BsGet
	str	w0, [x27, 1880]
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
	bhi	.L2769
.L2521:
	ldr	w2, [x29, 116]
	cmp	w1, w2
	bcs	.L2522
	sub	w1, w2, w1
	ldr	w2, [x29, 112]
	cmp	w1, w2, lsr 1
	sub	w1, w0, w2
	csel	w0, w1, w0, hi
.L2522:
	ldr	w1, [x27, 1864]
	sub	w1, w1, #16
	cmp	w1, 2
	ldr	w1, [x29, 116]
	csel	w0, w0, wzr, hi
	add	w3, w0, w1
	str	w3, [x27, 1840]
	ldr	w2, [x28, 244]
	cmp	w3, w2
	bge	.L2524
	ldrb	w0, [x27, 1793]
	cmp	w0, 1
	beq	.L2770
.L2524:
	ldr	w0, [x28, 220]
	cbnz	w0, .L2525
	ldr	w0, [x27, 1840]
	str	w0, [x28, 188]
.L2525:
	ldr	w0, [x27, 1860]
	cbnz	w0, .L2526
	ldr	w0, [x27, 1864]
	cmp	w0, 15
	bhi	.L2527
	mov	w1, 1
	tbz	x0, 0, .L2527
.L2528:
	sub	w0, w0, #7
	tst	w0, -3
	cset	w0, ne
	tst	w1, w0
	beq	.L2526
	ldr	w0, [x27, 1840]
	str	w0, [x27, 1844]
	str	w0, [x28, 184]
	.p2align 2
.L2526:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1796]
	cbz	w0, .L2529
	ldr	w2, [x21, 228]
	mov	w1, 0
	mov	w3, 1
	cmp	w2, 1
	bls	.L2531
	.p2align 2
.L2707:
	add	w1, w1, 1
	lsl	w0, w3, w1
	cmp	w0, w2
	bcc	.L2707
	cbnz	w1, .L2771
.L2531:
	str	wzr, [x27, 1884]
.L2536:
	add	x0, x28, 57344
	str	x0, [x29, 104]
	add	x3, x0, 964
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
.L2532:
	ldrb	w0, [x21, 10]
	cbz	w0, .L2537
	ldr	x0, [x29, 104]
	ldr	w1, [x21, 232]
	add	x0, x0, 964
	ldr	w2, [x0, 16]
	ldr	w0, [x0, 20]
	add	w0, w2, w0
	str	w0, [x29, 100]
	cbnz	w1, .L2538
.L2541:
	mov	w23, 0
.L2539:
	ldr	x1, .LC777
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	str	w0, [x27, 1892]
	ldr	x1, [x29, 104]
	add	x24, x1, 964
	ldr	w1, [x27, 1888]
	add	w4, w0, w1
	ldp	w2, w3, [x24, 16]
	str	w4, [x24, 32]
	add	w0, w2, w3
	add	w0, w0, w4
	cmp	w0, 16
	bhi	.L2772
	ldr	w0, [x29, 100]
	sub	w0, w0, #1
	str	w0, [x29, 96]
	add	w0, w4, w0
	str	w0, [x29, 136]
	cbz	w4, .L2544
	mov	w26, 0
	str	wzr, [x29, 140]
	b	.L2545
	.p2align 3
.L2775:
	ldr	w0, [x27, 1888]
	cmp	w0, w26
	ldr	w0, [x29, 140]
	csel	w0, w0, wzr, ne
	str	w0, [x29, 140]
.L2555:
	add	x19, x19, 12288
	add	x19, x28, x19, lsl 2
	str	wzr, [x19, 7412]
.L2554:
	ldr	w1, [x24, 32]
	add	w26, w26, 1
	cmp	w1, w26
	bls	.L2556
	ldr	w1, [x27, 1888]
.L2545:
	ldr	w0, [x29, 136]
	cmp	w26, w1
	sub	w19, w0, w26
	bcs	.L2547
	cbnz	w23, .L2773
	mov	w0, 0
	uxtw	x25, w26
.L2548:
	sxtw	x0, w0
	add	x1, x21, x0
	add	x0, x21, x0, lsl 2
	ldrb	w1, [x1, 16]
	ldr	w3, [x0, 248]
	add	x0, x24, x19
	strb	w1, [x0, 53]
.L2551:
	add	x22, x28, x25, lsl 2
	mov	w1, 1
	add	x22, x22, 53248
	str	w3, [x22, 3188]
	ldr	x0, [x28, 2344]
	bl	BsGet
	uxtb	w0, w0
	add	x7, x28, x25
	add	x7, x7, 53248
	strb	w0, [x7, 1823]
	cbnz	w0, .L2774
	cbnz	w26, .L2775
	str	wzr, [x29, 140]
	b	.L2555
	.p2align 3
.L2765:
	ldrb	w0, [x28, 3]
	cbz	w0, .L2633
	ldr	w0, [x27, 1840]
	cmp	w0, wzr
	ldr	w0, [x29, 132]
	ccmp	w0, 5, 2, ne
	bls	.L2776
	b	.L2633
	.p2align 3
.L2759:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1794]
	b	.L2494
	.p2align 3
.L2762:
	ldr	x0, [x28, 2344]
	bl	BsGet
	str	w0, [x27, 1872]
	ldp	w1, w3, [x21, 240]
	mul	w3, w1, w3
	tbnz	w0, #31, .L2511
	cmp	w3, w0
	bls	.L2511
	mul	w0, w24, w0
	b	.L2509
	.p2align 3
.L2764:
	ldr	x1, .LC835
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	str	w0, [x27, 1932]
	ldr	w1, [x20, 12]
	and	w1, w1, 16776960
	cmp	w1, 65536
	beq	.L2777
	cmp	w1, 256
	ldr	w1, [x20, 68]
	beq	.L2778
	ldr	w2, [x21, 244]
	mul	w1, w1, w2
	sub	w1, w1, #1
.L2616:
	cmp	w1, 255
	mov	w3, 255
	csel	w3, w1, w3, ls
	cmp	w0, w3
	bhi	.L2779
	cbz	w0, .L2620
	ldr	x1, .LC839
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w0, 31
	str	w2, [x27, 1856]
	bhi	.L2780
	ldr	w0, [x27, 1932]
	cbz	w0, .L2620
	mov	w19, 0
	b	.L2622
	.p2align 3
.L2781:
	ldr	w2, [x27, 1856]
.L2622:
	ldr	x0, [x28, 2344]
	mov	w1, w2
	bl	BsGet
	uxtw	x1, w19
	add	w19, w19, 1
	add	x1, x1, 12288
	add	x1, x28, x1, lsl 2
	str	w0, [x1, 6068]
	ldr	w0, [x27, 1932]
	cmp	w0, w19
	bhi	.L2781
	ldrb	w0, [x20, 23]
	cbz	w0, .L2630
.L2624:
	ldr	x1, .LC843
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	cmp	w0, 256
	str	w0, [x27, 1936]
	bhi	.L2782
	ldr	x0, [x28, 2344]
	bl	BsResidBits
	ldr	w3, [x27, 1936]
	cmp	w3, w0
	bhi	.L2628
	mov	w19, 0
	cbz	w3, .L2630
	.p2align 2
.L2706:
	ldr	x0, [x28, 2344]
	mov	w1, 8
	add	w19, w19, 1
	bl	BsGet
	str	w0, [x27, 1940]
	ldr	w1, [x27, 1936]
	cmp	w1, w19
	bhi	.L2706
	b	.L2630
	.p2align 3
.L2632:
	ldr	w0, [x29, 132]
	cmp	w0, 5
	bhi	.L2633
	strb	wzr, [x28, 3]
	b	.L2633
	.p2align 3
.L2769:
	sub	w2, w1, w4
	cmp	w2, w3, lsr 1
	bcc	.L2521
	add	w0, w0, w3
	b	.L2522
.L2777:
	ldr	w1, [x21, 244]
	sub	w1, w1, #1
	b	.L2616
.L2527:
	sub	w1, w0, #16
	cmp	w1, 7
	cset	w1, ls
	b	.L2528
	.p2align 3
.L2547:
	ldr	w1, [x21, 132]
	uxtw	x25, w26
	ldr	x0, [x28, 2344]
	bl	BsGet
	mov	w3, w0
	add	x0, x25, 12288
	mov	w1, 1
	str	w3, [x29, 128]
	add	x0, x28, x0, lsl 2
	str	w3, [x0, 7156]
	ldr	x0, [x28, 2344]
	bl	BsGet
	uxtb	w0, w0
	add	x1, x28, x25
	ldr	w3, [x29, 128]
	add	x1, x1, 53248
	strb	w0, [x1, 1807]
	add	x1, x24, x19
	strb	w0, [x1, 53]
	b	.L2551
	.p2align 3
.L2774:
	ldr	x1, .LC783
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	str	w0, [x22, 3124]
	cbz	w26, .L2553
	ldr	w1, [x29, 140]
	add	w25, w0, w1
	ldr	w1, [x27, 1888]
	cmp	w1, w26
	csel	w0, w25, w0, ne
.L2553:
	add	x19, x28, x19, lsl 2
	mov	w1, 1
	add	x19, x19, 53248
	str	w0, [x29, 140]
	str	w0, [x19, 3252]
	str	w1, [x19, 3316]
	b	.L2554
	.p2align 3
.L2773:
	ldr	x0, [x28, 2344]
	mov	w1, w23
	uxtw	x25, w26
	bl	BsGet
	add	x1, x25, 12288
	add	x1, x28, x1, lsl 2
	str	w0, [x1, 7092]
	ldr	w4, [x21, 232]
	sub	w4, w4, #1
	tbnz	w0, #31, .L2550
	cmp	w4, w0
	bcs	.L2548
.L2550:
	ldr	x1, .LC781
	mov	w3, w0
	mov	w2, w26
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
	.p2align 3
.L2520:
	str	wzr, [x27, 1840]
	ldr	w0, [x28, 220]
	cbnz	w0, .L2562
	str	wzr, [x28, 188]
.L2562:
	ldr	w0, [x27, 1860]
	cbnz	w0, .L2563
	str	wzr, [x27, 1844]
	str	wzr, [x28, 184]
.L2563:
	ldrb	w0, [x21, 8]
	cbz	w0, .L2638
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1797]
	mov	w1, 1
	ldr	x0, [x28, 2344]
	bl	BsGet
	strb	w0, [x27, 1798]
.L2638:
	strb	wzr, [x27, 1799]
	str	xzr, [x29, 120]
.L2640:
	ldr	w0, [x27, 1852]
	cmp	w0, 2
	bne	.L2783
.L2564:
	ldrb	w0, [x27, 1799]
	cbnz	w0, .L2784
.L2585:
	mov	w1, 2
.L2595:
	ldrb	w0, [x20, 11]
	cbz	w0, .L2584
	cbnz	w1, .L2584
.L2598:
	mov	x0, x28
	bl	HEVC_DecPredWeightTable
	cbnz	w0, .L2785
	ldr	w1, [x27, 1852]
.L2584:
	cmp	w1, 2
	beq	.L2599
	ldr	x1, .LC807
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	mov	w3, 4
	mov	w1, 5
	sub	w3, w3, w0
	sub	w2, w1, w0
	cmp	w3, 4
	str	w2, [x27, 1908]
	bhi	.L2786
.L2599:
	ldr	x1, .LC811
	ldr	x0, [x28, 2344]
	bl	HEVC_se_v
	str	w0, [x27, 1912]
	mov	w4, 51
	ldr	w1, [x20, 48]
	add	w2, w0, w1
	str	w2, [x27, 1848]
	ldr	w3, [x21, 104]
	neg	w0, w3
	cmp	w2, w0
	ccmp	w2, w4, 0, ge
	bgt	.L2787
	ldrb	w0, [x20, 9]
	cbnz	w0, .L2788
.L2601:
	ldrb	w0, [x20, 18]
	cbz	w0, .L2749
	ldrb	w0, [x20, 19]
	cbnz	w0, .L2789
	strb	wzr, [x27, 1804]
.L2609:
	ldrb	w0, [x20, 20]
	strb	w0, [x27, 1805]
	ldr	w1, [x20, 76]
	str	w1, [x27, 1924]
	ldr	w1, [x20, 80]
	str	w1, [x27, 1928]
.L2606:
	ldrb	w2, [x21, 8]
	mov	w1, 0
	cbz	w2, .L2612
	ldr	w1, [x27, 1796]
	tst	w1, 16776960
	cset	w1, ne
.L2612:
	ldrb	w2, [x20, 17]
	cbz	w2, .L2613
	cmp	w0, wzr
	cset	w0, eq
	orr	w1, w0, w1
	cbnz	w1, .L2790
.L2613:
	strb	w2, [x27, 1806]
	b	.L2512
	.p2align 3
.L2783:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1800]
	cbnz	w0, .L2791
	ldr	w0, [x20, 40]
	ldr	w1, [x27, 1852]
	str	w0, [x27, 1960]
	cbnz	w1, .L2572
	ldr	w0, [x20, 44]
	str	w0, [x27, 1964]
.L2570:
	ldr	x0, [x29, 120]
	cbz	x0, .L2792
	ldr	x0, [x29, 120]
	ldr	w2, [x0, 24]
	cmp	w2, 16
	bhi	.L2574
	cbz	w2, .L2576
	ldr	x1, [x29, 120]
	sub	w2, w2, #1
	add	x2, x2, 54
	mov	w3, 0
	add	x0, x1, 53
	add	x2, x1, x2
	.p2align 2
.L2578:
	ldrb	w1, [x0], 1
	cmp	w1, wzr
	csinc	w3, w3, w3, eq
	cmp	x0, x2
	bne	.L2578
	ldrb	w0, [x20, 22]
	cmp	w0, wzr
	ccmp	w3, 1, 4, ne
	ble	.L2576
	add	x2, x28, 57344
	add	x1, x27, 1792
	add	x2, x2, 1168
	mov	x0, x28
	bl	HEVC_DecRefPicListsModification
	cbnz	w0, .L2793
.L2569:
	ldr	w1, [x27, 1852]
	cbnz	w1, .L2579
.L2641:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1801]
	ldr	w1, [x27, 1852]
.L2579:
	cmp	w1, 2
	beq	.L2564
.L2642:
	ldrb	w0, [x20, 5]
	cbnz	w0, .L2794
.L2580:
	ldrb	w0, [x27, 1799]
	cbz	w0, .L2581
	ldr	w0, [x27, 1852]
	cbz	w0, .L2795
	mov	w1, 1
	cmp	w0, 2
	strb	w1, [x27, 1803]
	beq	.L2585
.L2588:
	ldr	w0, [x27, 1960]
	cmp	w0, 1
	bls	.L2581
.L2586:
	ldr	x1, .LC801
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	str	w0, [x27, 1904]
	ldrb	w2, [x27, 1803]
	cbnz	w2, .L2591
	ldr	w4, [x27, 1900]
	sub	w4, w4, #1
.L2593:
	cmp	w4, w0
	ccmp	w0, wzr, 1, cs
	bge	.L2581
	ldr	x1, .LC803
	mov	w3, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
	.p2align 3
.L2581:
	ldr	w1, [x27, 1852]
.L2590:
	ldrb	w0, [x20, 10]
	cbz	w0, .L2595
	cmp	w1, 1
	bne	.L2595
	b	.L2598
.L2789:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1804]
	cbz	w0, .L2609
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x27, 1805]
	cbnz	w0, .L2606
	ldr	x1, .LC827
	ldr	x0, [x28, 2344]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x27, 1924]
	bhi	.L2796
.L2610:
	ldr	x1, .LC831
	ldr	x0, [x28, 2344]
	bl	HEVC_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x27, 1928]
	bls	.L2749
	ldr	x1, .LC833
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 80]
	str	w0, [x27, 1928]
	.p2align 2
.L2749:
	ldrb	w0, [x27, 1805]
	b	.L2606
.L2544:
	mov	w1, 0
.L2546:
	ldr	w0, [x29, 100]
	add	w2, w0, w1
	ldr	x0, [x29, 104]
	cmp	w2, 16
	str	w2, [x0, 988]
	bhi	.L2797
.L2537:
	ldrb	w0, [x21, 11]
	cbnz	w0, .L2798
	strb	wzr, [x27, 1799]
.L2561:
	ldrb	w0, [x21, 8]
	cbz	w0, .L2640
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1797]
	mov	w1, 1
	ldr	x0, [x28, 2344]
	bl	BsGet
	strb	w0, [x27, 1798]
	b	.L2640
	.p2align 3
.L2576:
	ldr	w0, [x27, 1852]
	cbnz	w0, .L2642
	b	.L2641
	.p2align 3
.L2768:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1792]
	b	.L2519
	.p2align 3
.L2496:
	ldr	x2, .LC751
	mov	w3, w0
	ldr	x1, .LC753
	mov	w0, 1
	add	x2, x2, 48
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
	.p2align 3
.L2788:
	ldr	x1, .LC815
	ldr	x0, [x28, 2344]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x27, 1916]
	bhi	.L2799
	ldr	w3, [x20, 60]
	add	w1, w0, w3
	add	w1, w1, 12
	cmp	w1, 24
	bhi	.L2800
	ldr	x1, .LC821
	ldr	x0, [x28, 2344]
	bl	HEVC_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x27, 1920]
	bhi	.L2801
	ldr	w1, [x20, 64]
	add	w2, w0, w1
	add	w0, w2, 12
	cmp	w0, 24
	bls	.L2601
	ldr	x1, .LC825
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
	.p2align 3
.L2794:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1802]
	b	.L2580
.L2556:
	ldr	w0, [x29, 96]
	add	w9, w1, w0
	cbz	w1, .L2544
	add	x7, x27, 3188
	mov	x3, 0
	b	.L2559
	.p2align 3
.L2802:
	ldr	w0, [x7]
	ldr	w2, [x8, 3252]
	ldr	w5, [x29, 112]
	ldr	w10, [x27, 1840]
	msub	w8, w5, w2, w0
	ldr	w0, [x29, 116]
	sub	w2, w10, w0
	add	w0, w8, w2
	str	w0, [x4, 1104]
	sub	w0, w0, w10
	str	w0, [x4, 1036]
.L2558:
	add	x3, x3, 1
	add	x7, x7, 4
	cmp	w1, w3
	bls	.L2546
.L2559:
	add	x2, x3, 53248
	sub	w0, w9, w3
	add	x2, x2, 1823
	add	x4, x28, x0, uxtw 2
	add	x0, x4, 57344
	add	x8, x4, 53248
	ldrb	w2, [x28, x2]
	mov	x4, x0
	cbnz	w2, .L2802
	ldr	w2, [x7]
	str	w2, [x0, 1104]
	ldr	w4, [x27, 1840]
	sub	w2, w2, w4
	str	w2, [x0, 1036]
	b	.L2558
.L2572:
	str	wzr, [x27, 1964]
.L2571:
	cmp	w1, 2
	bne	.L2570
	ldr	w1, [x27, 1852]
	b	.L2579
	.p2align 3
.L2529:
	add	x0, x28, 57344
	ldr	w3, [x21, 228]
	add	x2, x0, 964
	str	x0, [x29, 104]
	mov	x1, x21
	mov	x0, x28
	str	x2, [x29, 120]
	bl	HEVC_DecShortTermRefPicSet
	cbz	w0, .L2532
	ldr	x1, .LC769
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
	.p2align 3
.L2791:
	ldr	x1, .LC787
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w2, 16
	str	w2, [x27, 1896]
	bhi	.L2803
	ldr	w1, [x27, 1852]
	str	w2, [x27, 1960]
	cbz	w1, .L2804
	str	wzr, [x27, 1900]
	str	wzr, [x27, 1964]
	b	.L2571
.L2795:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	uxtb	w0, w0
	ldr	w1, [x27, 1852]
	strb	w0, [x27, 1803]
	cmp	w1, 2
	beq	.L2599
	cbnz	w0, .L2588
	ldr	w0, [x27, 1964]
	cmp	w0, 1
	bhi	.L2586
	b	.L2590
.L2784:
	mov	w0, 1
	mov	w1, 2
	strb	w0, [x27, 1803]
	b	.L2590
.L2790:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1806]
	b	.L2512
.L2771:
	ldr	x0, [x28, 2344]
	bl	BsGet
	sxtw	x22, w0
	str	w22, [x27, 1884]
	ldr	w3, [x21, 228]
	sub	w3, w3, #1
	tbnz	w22, #31, .L2535
	cmp	w3, w22
	bcs	.L2536
.L2535:
	ldr	x1, .LC771
	mov	w2, w22
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2591:
	ldr	w4, [x27, 1896]
	sub	w4, w4, #1
	b	.L2593
.L2538:
	ldr	x1, .LC773
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	str	w0, [x27, 1888]
	ldr	w3, [x21, 232]
	cmp	w0, w3
	bhi	.L2805
	cmp	w3, 1
	bls	.L2541
	mov	w23, 0
	mov	w1, 1
	.p2align 2
.L2542:
	add	w23, w23, 1
	lsl	w0, w1, w23
	cmp	w3, w0
	bhi	.L2542
	b	.L2539
.L2766:
	mov	w0, 1
	strb	w0, [x28, 2]
	b	.L2636
.L2804:
	ldr	x1, .LC791
	ldr	x0, [x28, 2344]
	bl	HEVC_ue_v
	add	w2, w0, 1
	cmp	w2, 16
	str	w2, [x27, 1900]
	bhi	.L2806
	ldr	w0, [x27, 1852]
	str	w2, [x27, 1964]
	cmp	w0, 2
	bne	.L2570
	b	.L2569
	.p2align 3
.L2782:
	ldr	x1, .LC845
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2797:
	ldr	x1, .LC785
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
	.p2align 3
.L2798:
	ldr	x0, [x28, 2344]
	mov	w1, 1
	bl	BsGet
	strb	w0, [x27, 1799]
	b	.L2561
.L2505:
	ldr	x1, .LC757
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2778:
	ldr	w2, [x20, 72]
	mul	w1, w1, w2
	sub	w1, w1, #1
	b	.L2616
.L2760:
	ldr	w0, [x28, 248]
	cbz	w0, .L2500
	ldr	w0, [x28, 188]
	str	w0, [x27, 1840]
.L2501:
	mov	x0, x28
	bl	HEVC_IsNewPic
	str	w0, [x27, 3380]
	ldr	x1, .LC755
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2787:
	ldr	x1, .LC813
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2500:
	str	wzr, [x27, 1840]
	b	.L2501
.L2770:
	ldr	x1, .LC767
	bl	dprint_vfmw
	b	.L2524
.L2511:
	ldr	x1, .LC759
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2767:
	ldr	x1, .LC765
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2574:
	ldr	x1, .LC797
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2786:
	ldr	x1, .LC809
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2779:
	ldr	x1, .LC837
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2796:
	ldr	x1, .LC829
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 76]
	str	w0, [x27, 1924]
	b	.L2610
.L2628:
	ldr	x1, .LC847
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2800:
	ldr	x1, .LC819
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2785:
	ldr	x1, .LC805
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2799:
	ldr	x1, .LC817
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2772:
	ldr	x1, .LC779
	mov	w5, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2801:
	ldr	x1, .LC823
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2780:
	ldr	x1, .LC841
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2803:
	ldr	x1, .LC789
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2792:
	ldr	x1, .LC795
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2793:
	ldr	x1, .LC799
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2805:
	ldr	x1, .LC775
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
.L2806:
	ldr	x1, .LC793
	mov	w3, 16
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2742
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
	ldr	x1, [x19, 2392]
	cbz	x1, .L2809
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L2809:
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
	cbz	w2, .L2818
	.p2align 2
.L2821:
	uxtw	x0, w20
	ldr	x1, [x22, 3696]
	lsl	x19, x0, 5
	add	w20, w20, 1
	add	x19, x19, x0, lsl 3
	add	x0, x1, x19
	ldr	x0, [x0, 32]
	mov	x1, x0
	cbz	x0, .L2816
	ldr	w0, [x21, 276]
	bl	HEVC_ReleaseNAL
	ldr	x0, [x22, 3696]
	add	x0, x0, x19
	str	xzr, [x0, 32]
	ldr	w2, [x21, 160]
.L2816:
	cmp	w2, w20
	bhi	.L2821
.L2818:
	strb	wzr, [x21, 9]
	mov	w0, 2
	str	wzr, [x21, 208]
	str	w0, [x21, 172]
	mov	w0, 0
	str	wzr, [x21, 248]
	str	wzr, [x21, 256]
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
	add	x19, x20, 2736
	bl	_mcount
	add	x21, x20, 2872
	mov	x0, x20
	bl	HEVC_ClearAllSlice
	.p2align 2
.L2830:
	ldr	x0, [x19]
	mov	w2, 1
	cbz	x0, .L2826
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L2826:
	ldr	x0, [x19, 136]
	mov	w2, 1
	cbz	x0, .L2827
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L2827:
	ldr	x0, [x19, 272]
	mov	w2, 1
	cbz	x0, .L2828
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L2828:
	ldr	x0, [x19, 408]
	mov	w2, 1
	cbz	x0, .L2829
	ldr	w1, [x0, 32]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
.L2829:
	str	xzr, [x19, 136]
	str	xzr, [x19, 272]
	str	xzr, [x19, 408]
	str	xzr, [x19], 8
	cmp	x19, x21
	bne	.L2830
	ldr	x0, [x20, 28168]
	ldr	w0, [x0, 236]
	add	w0, w0, 1
	cmp	w0, 17
	bls	.L2870
	mov	w0, 17
.L2870:
	add	x3, x20, 2560
	mov	w1, 0
	mov	x2, 200
	stp	w0, wzr, [x3, 152]
	ldr	x0, .LC848
	stp	wzr, wzr, [x3, 160]
	str	wzr, [x20, 2728]
	ldr	x3, [x0, 96]
	add	x0, x20, 3280
	str	wzr, [x20, 2732]
	blr	x3
	str	wzr, [x20, 3284]
	mov	w0, 16
	add	x1, x20, 2400
	str	w0, [x20, 3280]
	add	x0, x20, 2536
	.p2align 2
.L2833:
	str	xzr, [x1], 8
	cmp	x1, x0
	bne	.L2833
	add	x2, x20, 53248
	add	x1, x20, 2672
	str	wzr, [x2, 1952]
	.p2align 2
.L2834:
	str	xzr, [x0], 8
	cmp	x1, x0
	bne	.L2834
	str	wzr, [x2, 1956]
	cbnz	w22, .L2835
	ldr	x0, [x20, 2376]
	ldr	w0, [x0, 912]
	cmp	w0, 1
	beq	.L2835
.L2836:
	ldr	w0, [x20, 180]
	cbz	w0, .L2844
	sub	w1, w0, #1
	add	x0, x20, 2148
	add	x1, x1, 538
	add	x1, x20, x1, lsl 2
	.p2align 2
.L2843:
	str	wzr, [x0], 4
	cmp	x1, x0
	bne	.L2843
.L2844:
	ldr	w2, [x20, 160]
	mov	w21, 0
	add	x22, x20, 65536
	cbz	w2, .L2842
	.p2align 2
.L2859:
	uxtw	x0, w21
	ldr	x1, [x22, 3696]
	lsl	x19, x0, 5
	add	w21, w21, 1
	add	x19, x19, x0, lsl 3
	add	x0, x1, x19
	ldr	x0, [x0, 32]
	mov	x1, x0
	cbz	x0, .L2845
	ldr	x3, [x20, 2392]
	cmp	x0, x3
	beq	.L2845
	ldr	w0, [x20, 276]
	bl	HEVC_ReleaseNAL
	ldr	x0, [x22, 3696]
	add	x0, x0, x19
	str	xzr, [x0, 32]
	ldr	w2, [x20, 160]
.L2845:
	cmp	w2, w21
	bhi	.L2859
.L2842:
	mov	w0, 0
.L2837:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2835:
	ldr	x0, [x20, 28168]
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
	ldr	x1, [x20, 2376]
	ldr	w1, [x1, 912]
	cmp	w1, 1
	beq	.L2847
	cmp	w0, 1
	bne	.L2871
	ldr	w0, [x20, 276]
	bl	FSP_GetTotalValidFsNum
	cmp	w0, 20
	mov	w0, w19
	ble	.L2872
	str	w0, [x20, 176]
	b	.L2836
.L2872:
	ldr	w0, [x20, 276]
	bl	FSP_GetTotalValidFsNum
	str	w0, [x20, 176]
	b	.L2836
.L2871:
	ldr	x2, .LC849
	mov	w0, 0
	ldr	x1, .LC851
	add	x2, x2, 80
	bl	dprint_vfmw
	mov	w0, -5
	b	.L2837
.L2847:
	mov	w0, -2
	b	.L2837
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
	bne	.L2878
	b	.L2877
	.p2align 3
.L2875:
	add	w0, w3, 1
	cmp	w0, w19
	bls	.L2877
.L2878:
	uxtw	x1, w19
	ldr	x2, [x20, 2888]
	lsl	x0, x1, 3
	add	w19, w19, 1
	sub	x0, x0, x1
	add	x1, x2, x0, lsl 4
	ldr	w0, [x1, 36]
	cmp	w0, 1
	bne	.L2875
	ldr	w0, [x21, 276]
	bl	HEVC_ReleaseNAL
	ldr	w3, [x21, 160]
	add	w0, w3, 1
	cmp	w0, w19
	bhi	.L2878
.L2877:
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
	add	x19, x22, 3704
	ldr	x1, .LC853
	mov	w0, 22
	mov	w25, 20
	mov	w24, 16
	bl	dprint_vfmw
	mov	w1, -2147483647
	mov	x0, x21
	str	w1, [x21, 244]
	bl	HEVC_ClearAllNal
	add	x0, x21, 61440
	add	x0, x0, 1064
	bl	ResetVoQueue
	ldr	x23, .LC854
	.p2align 2
.L2882:
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
	bne	.L2882
	add	x0, x21, 2148
	add	x1, x21, 2228
	.p2align 2
.L2883:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L2883
	ldr	w0, [x21, 160]
	mov	w1, 0
	cbz	w0, .L2887
	.p2align 2
.L2889:
	uxtw	x0, w1
	ldr	x2, [x22, 3696]
	lsl	x3, x0, 5
	add	w1, w1, 1
	add	x0, x3, x0, lsl 3
	str	wzr, [x2, x0]
	ldr	x2, [x22, 3696]
	add	x2, x2, x0
	str	xzr, [x2, 24]
	ldr	x2, [x22, 3696]
	add	x0, x2, x0
	str	xzr, [x0, 32]
	ldr	w0, [x21, 160]
	cmp	w0, w1
	bhi	.L2889
.L2887:
	ldr	x3, [x23, 96]
	mov	w1, 0
	mov	x2, 200
	add	x0, x21, 3280
	blr	x3
	mov	w1, 16
	add	x0, x21, 2872
	str	w1, [x21, 3280]
	add	x1, x21, 3008
	.p2align 2
.L2885:
	str	xzr, [x0, -136]
	add	x0, x0, 8
	str	xzr, [x0, -8]
	str	xzr, [x0, 128]
	str	xzr, [x0, 264]
	cmp	x1, x0
	bne	.L2885
	add	x2, x21, 2560
	mov	w0, 17
	add	x19, x21, 2536
	stp	w0, wzr, [x2, 152]
	stp	wzr, wzr, [x2, 160]
	str	wzr, [x21, 2728]
	add	x0, x21, 2672
	str	wzr, [x21, 2732]
	.p2align 2
.L2888:
	str	xzr, [x19, -136]
	add	x19, x19, 8
	str	xzr, [x19, -8]
	cmp	x19, x0
	bne	.L2888
	add	x22, x21, 53248
	ldr	x3, [x23, 96]
	mov	w20, 2
	mov	x2, 3732
	str	w20, [x21, 172]
	mov	w1, 0
	add	x0, x22, 1792
	str	wzr, [x21, 208]
	str	wzr, [x21, 248]
	str	wzr, [x21, 256]
	str	xzr, [x21, 2392]
	blr	x3
	str	w20, [x22, 1852]
	mov	w0, 1
	mov	w1, 8
	str	w0, [x22, 3380]
	mov	w2, 16
	stp	w1, w1, [x21, 192]
	str	w1, [x21, 344]
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
	str	w2, [x21, 324]
	mov	w3, 2147483647
	str	w5, [x21, 328]
	mov	w5, 1025
	str	w2, [x21, 348]
	mov	x2, 40
	str	wzr, [x21, 168]
	ldr	x4, [x23, 96]
	stp	w0, w20, [x1, 188]
	stp	w0, w0, [x1, 180]
	str	w3, [x21, 200]
	mov	w1, 32
	mov	w3, 256
	str	w5, [x21, 336]
	str	w1, [x21, 352]
	mov	x0, x19
	str	w3, [x21, 332]
	mov	w1, 0
	mov	w3, 4
	str	w3, [x21, 340]
	blr	x4
	str	x19, [x21, 2344]
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
	mov	x2, 21440
	mov	w1, 0
	movk	x2, 0x1, lsl 16
	mov	x0, x19
	ldr	x3, [x20, 96]
	ldr	w23, [x21, 1664]
	blr	x3
	str	w23, [x21, 1664]
	str	x22, [x19, 2376]
	str	xzr, [x19, 2384]
	ldr	w0, [x22, 28]
	cmp	w0, 25
	beq	.L2903
	mov	w0, 17
	mov	w3, 65
	mov	w2, 200
	mov	w1, 210
	stp	w2, w0, [x19, 144]
	stp	w0, w3, [x19, 152]
	str	w1, [x19, 160]
.L2897:
	ldr	w0, [x22, 624]
	bl	MEM_Phy2Vir
	mov	x5, x0
	cbz	x0, .L2904
	ldr	x7, [x19, 2376]
	mov	w2, 23196
	ldr	w9, [x19, 148]
	mov	w23, 26164
	ldpsw	x4, x0, [x19, 152]
	ldr	w3, [x7, 620]
	add	x24, x19, 65536
	lsl	w1, w0, 7
	sub	w1, w1, w0
	add	x8, x5, x3, uxtw
	lsl	w1, w1, 1
	madd	w3, w9, w2, w3
	sub	w1, w1, w0
	str	x8, [x19, 2352]
	madd	w4, w23, w4, w3
	add	x3, x5, x3, uxtw
	str	x3, [x19, 2360]
	add	w1, w4, w1, lsl 3
	mov	w3, 1368
	add	x4, x5, x4, uxtw
	str	x4, [x19, 2368]
	add	x0, x5, x1, uxtw
	str	x0, [x24, 2880]
	ldrsw	x6, [x19, 144]
	madd	w1, w3, w6, w1
	add	x3, x5, x1, uxtw
	str	x3, [x24, 2888]
	ldr	w3, [x19, 160]
	add	w10, w3, 1
	lsl	w4, w3, 5
	add	w3, w4, w3, lsl 3
	lsl	w4, w10, 3
	sub	w4, w4, w10
	add	w1, w1, w4, lsl 4
	add	w3, w3, w1
	add	x1, x5, x1, uxtw
	str	x1, [x24, 3696]
	ldr	w4, [x7, 628]
	cmp	w3, w4
	bhi	.L2905
	ldr	w1, [x22, 648]
	cbz	w1, .L2906
.L2901:
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
	ldr	x0, [x24, 2888]
	sxtw	x2, w2
	lsl	x3, x2, 3
	sub	x2, x3, x2
	lsl	x2, x2, 4
	blr	x4
	ldrsw	x2, [x19, 160]
	mov	w1, 0
	ldr	x3, [x20, 96]
	lsl	x4, x2, 5
	ldr	x0, [x24, 3696]
	add	x2, x4, x2, lsl 3
	blr	x3
	ldp	w5, w2, [x19, 148]
	mov	w1, 23196
	mov	x0, -23196
	ldr	w4, [x19, 156]
	mov	w3, 2024
	smaddl	x0, w5, w1, x0
	mov	x1, -2024
	ldr	x6, [x19, 2352]
	smaddl	x3, w4, w3, x1
	mov	w4, 26164
	mov	x1, -26164
	add	x0, x6, x0
	ldr	x6, [x19, 2368]
	smaddl	x1, w2, w4, x1
	ldr	x4, [x19, 2360]
	add	x2, x6, x3
	str	x0, [x19, 28160]
	str	x2, [x19, 28176]
	add	x1, x4, x1
	str	x1, [x19, 28168]
	str	w5, [x0, 8]
	mov	x0, x19
	ldr	x1, [x19, 28176]
	ldr	w2, [x19, 152]
	str	w2, [x1, 28]
	ldr	x1, [x19, 28168]
	ldr	w2, [x19, 156]
	str	w2, [x1, 60]
	bl	HEVC_InitScalingOrderTable
	add	x1, x19, 86016
	mov	x0, x19
	str	w22, [x1, 948]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x19, 276]
	beq	.L2907
	mov	w1, -2147483647
	mov	x0, x19
	str	w1, [x19, 244]
	bl	HEVC_InitDecPara
	str	wzr, [x21, 1672]
	mov	w0, 9999
	strb	w22, [x19]
	ldr	w2, [x19, 156]
	ldr	x1, [x19, 2376]
	str	w0, [x19, 284]
	mov	w0, 0
	str	wzr, [x19, 240]
	str	w2, [x19, 320]
	ldr	w1, [x1, 664]
	str	w1, [x19, 212]
.L2899:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2906:
	smull	x2, w9, w2
	ldr	x3, [x20, 96]
	mov	x0, x8
	blr	x3
	ldr	w2, [x19, 152]
	mov	w1, 0
	ldr	x3, [x20, 96]
	ldr	x0, [x19, 2360]
	smull	x2, w2, w23
	blr	x3
	ldr	w4, [x19, 156]
	mov	w2, 2024
	ldr	x0, [x19, 2368]
	mov	w1, 0
	ldr	x3, [x20, 96]
	smull	x2, w4, w2
	blr	x3
	ldrsw	x6, [x19, 144]
	ldr	x0, [x24, 2880]
	b	.L2901
	.p2align 3
.L2903:
	ldr	w0, [x22, 720]
	str	w0, [x19, 148]
	ldr	w0, [x22, 724]
	str	w0, [x19, 152]
	ldr	w0, [x22, 728]
	str	w0, [x19, 156]
	ldr	w0, [x22, 716]
	str	w0, [x19, 144]
	add	w0, w0, 5
	str	w0, [x19, 160]
	b	.L2897
	.p2align 3
.L2905:
	ldr	x2, .LC856
	mov	w0, 0
	ldr	x1, .LC860
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -20
	b	.L2899
	.p2align 3
.L2907:
	ldr	x2, .LC856
	mov	w0, 0
	ldr	x1, .LC862
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -20
	b	.L2899
.L2904:
	ldr	x2, .LC856
	mov	w0, 0
	ldr	x1, .LC858
	add	x2, x2, 96
	bl	dprint_vfmw
	mov	w0, -20
	b	.L2899
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
	ldr	x0, [x19, 1160]
	cbz	x0, .L2909
	ldr	w1, [x0, 32]
	mov	w2, 1
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
	ldr	x0, [x19, 1160]
	str	wzr, [x0, 4]
	str	xzr, [x19, 1160]
.L2909:
	strb	wzr, [x19, 1105]
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
	cbnz	w20, .L2916
	add	x0, x19, 61440
	add	x0, x0, 1064
	bl	ResetVoQueue
	ldr	w0, [x19, 276]
	bl	FSP_EmptyInstance
	mov	x0, x19
	bl	HEVC_InitDecPara
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2916:
	add	x1, x19, 61440
	ldr	w0, [x19, 276]
	add	x1, x1, 1064
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
	stp	x29, x30, [sp, -112]!
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
	ldr	w1, [x20, 1868]
	tbnz	w1, #31, .L2920
	ldr	w0, [x19, 156]
	cmp	w1, w0
	bge	.L2920
	ldr	x4, [x19, 2368]
	mov	w28, 2024
	mov	w2, 26164
	ldr	x0, [x19, 28168]
	ldr	x25, [x19, 2360]
	smaddl	x28, w1, w28, x4
	ldr	w3, [x19, 288]
	mov	w1, 23196
	ldr	x5, [x19, 2352]
	ldr	w21, [x28, 32]
	ldr	x4, [x19, 28160]
	ldr	w22, [x19, 292]
	smull	x21, w21, w2
	ldr	w2, [x19, 300]
	add	x26, x25, x21
	ldr	w4, [x4, 8]
	stp	w3, w2, [x29, 104]
	ldr	w2, [x0, 100]
	str	w2, [x29, 100]
	ldr	w2, [x0, 108]
	str	w2, [x29, 96]
	ldr	w2, [x26, 52]
	ldr	w23, [x19, 296]
	umull	x1, w2, w1
	add	x1, x5, x1
	ldr	w2, [x1, 8]
	cmp	w4, w2
	bne	.L2921
	ldrb	w2, [x1, 2]
	cbz	w2, .L2980
.L2921:
	ldr	x0, .LC863
	mov	w24, 23196
	strb	wzr, [x1, 2]
	mov	x27, -23196
	ldr	x7, [x0, 104]
	ldr	w0, [x19, 148]
	ldr	x2, [x19, 2352]
	smaddl	x0, w0, w24, x27
	add	x0, x2, x0
	mov	x2, 23196
	blr	x7
	ldr	w0, [x19, 148]
	ldr	x1, [x19, 2352]
	smaddl	x0, w0, w24, x27
	add	x0, x1, x0
	str	x0, [x19, 28160]
	ldr	x0, [x19, 28168]
.L2922:
	ldr	w2, [x0, 60]
	ldr	w1, [x26, 60]
	cmp	w2, w1
	beq	.L2981
.L2923:
	ldr	x0, .LC863
	mov	x27, -26164
	strb	wzr, [x25, x21]
	mov	w25, 26164
	ldr	x5, [x0, 104]
	ldr	x0, [x19, 28168]
	ldr	w1, [x26, 236]
	ldr	w2, [x0, 236]
	ldr	w0, [x19, 152]
	cmp	w2, w1
	ldr	x1, [x19, 2360]
	mov	x2, 26164
	cset	w21, ne
	smaddl	x0, w0, w25, x27
	add	x0, x1, x0
	mov	x1, x26
	blr	x5
	ldr	w0, [x19, 152]
	ldr	x1, [x19, 2360]
	smaddl	x0, w0, w25, x27
	add	x0, x1, x0
	str	x0, [x19, 28168]
	ldr	w1, [x0, 60]
	str	w1, [x19, 288]
	ldr	w1, [x0, 72]
	str	w1, [x19, 292]
	ldr	w1, [x0, 76]
	str	w1, [x19, 296]
	ldr	w1, [x0, 148]
	str	w1, [x19, 300]
.L2924:
	ldr	w1, [x0, 108]
	ldr	w0, [x0, 100]
	cmp	w0, w1
	csel	w0, w0, w1, cs
	ldr	x1, [x19, 2376]
	str	w0, [x1, 924]
	ldr	x0, [x19, 28176]
	ldr	w1, [x0, 28]
	ldr	w0, [x28, 28]
	cmp	w1, w0
	beq	.L2982
.L2925:
	ldrb	w0, [x28, 13]
	strb	wzr, [x28, 1]
	cbz	w0, .L2927
	ldrb	w0, [x28, 15]
	cbnz	w0, .L2983
	ldr	w0, [x28, 68]
	ldr	w1, [x26, 240]
	sub	w2, w0, #1
	add	x3, x28, x0, sxtw 1
	cmp	w2, wzr
	strh	w1, [x3, 174]
	ble	.L2939
	sub	w4, w0, #2
	add	x0, x28, 92
	add	x4, x4, 47
	add	x4, x28, x4, lsl 1
	.p2align 2
.L2938:
	ldrsh	w2, [x0]
	add	x0, x0, 2
	strh	w2, [x0, 82]
	cmp	x0, x4
	ldrh	w1, [x3, 174]
	sub	w1, w1, w2
	strh	w1, [x3, 174]
	bne	.L2938
.L2939:
	ldr	w0, [x28, 72]
	ldr	w1, [x26, 244]
	sub	w2, w0, #1
	add	x3, x28, x0, sxtw 1
	cmp	w2, wzr
	strh	w1, [x3, 214]
	ble	.L2934
	sub	w4, w0, #2
	add	x0, x28, 132
	add	x4, x4, 67
	add	x4, x28, x4, lsl 1
	.p2align 2
.L2940:
	ldrsh	w2, [x0]
	add	x0, x0, 2
	strh	w2, [x0, 82]
	cmp	x4, x0
	ldrh	w1, [x3, 214]
	sub	w1, w1, w2
	strh	w1, [x3, 214]
	bne	.L2940
	b	.L2934
	.p2align 3
.L2920:
	mov	w0, 0
.L2919:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L2927:
	ldr	w0, [x26, 240]
	strh	w0, [x28, 176]
	ldr	w0, [x26, 244]
	strh	w0, [x28, 216]
.L2934:
	ldr	x2, .LC863
	mov	x1, x28
	ldr	w0, [x19, 156]
	mov	w27, 2024
	mov	x28, -2024
	ldr	x4, [x2, 104]
	smaddl	x0, w0, w27, x28
	ldr	x2, [x19, 2368]
	add	x0, x2, x0
	mov	x2, 2024
	blr	x4
	ldr	w0, [x19, 156]
	ldr	x1, [x19, 2368]
	smaddl	x0, w0, w27, x28
	add	x0, x1, x0
	str	x0, [x19, 28176]
.L2926:
	ldr	w0, [x19, 220]
	cbnz	w0, .L2941
	ldr	w0, [x26, 60]
	ldr	w1, [x29, 104]
	cmp	w0, w1
	ccmp	w21, wzr, 0, eq
	bne	.L2942
	ldr	w0, [x19, 168]
	cbz	w0, .L2942
	ldr	w0, [x26, 72]
	cmp	w22, w0
	bne	.L2942
	ldr	w0, [x26, 76]
	cmp	w23, w0
	bne	.L2942
	ldr	x0, [x19, 2376]
	ldr	w1, [x0, 912]
	cmp	w1, 1
	bne	.L2944
	b	.L2943
	.p2align 3
.L2942:
	ldr	w3, [x29, 108]
	mov	w4, w21
	mov	w2, w23
	mov	w1, w22
	mov	x0, x19
	bl	HEVC_GetReRangeFlag
	cbz	w0, .L2945
	ldr	x0, [x19, 2376]
.L2943:
	ldr	w0, [x0, 928]
	cmp	w0, 1
	beq	.L2984
.L2947:
	add	x0, x19, 86016
	ldr	w0, [x0, 948]
	cmp	w0, 1
	beq	.L2985
.L2948:
	mov	w1, 1
	mov	x0, x19
	bl	HEVC_InitDPB
	cmn	w0, #2
	beq	.L2919
	cbnz	w0, .L2949
	ldr	x0, [x19, 2376]
.L2946:
	str	wzr, [x0, 928]
.L2944:
	mov	x0, x19
	bl	HEVC_AllocFrameStore
	mov	w21, w0
	cbz	w0, .L2941
	ldr	x1, .LC869
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w21
	b	.L2919
	.p2align 3
.L2941:
	ldr	w0, [x20, 1840]
	mov	x3, -1
	ldr	w1, [x20, 1864]
	str	w1, [x20, 1108]
	mov	w1, 1
	str	w0, [x20, 1112]
	mov	w0, -1
	strb	w1, [x20, 1105]
	str	w0, [x20, 1136]
	add	x0, x20, 1168
	ldr	x1, [x19, 28168]
	ldr	w2, [x1, 72]
	str	w2, [x20, 1120]
	ldr	w2, [x1, 76]
	str	w2, [x20, 1124]
	ldr	w2, [x1, 244]
	ldr	w1, [x1, 240]
	str	wzr, [x20, 1132]
	mul	w1, w2, w1
	str	w1, [x20, 1128]
	ldr	x1, [x19, 2376]
	ldr	x2, [x1, 56]
	str	x2, [x20, 1768]
	ldr	x2, [x1, 72]
	str	x2, [x20, 1776]
	ldr	x2, [x1, 88]
	str	x2, [x20, 1784]
	ldr	w2, [x1, 104]
	str	w2, [x20, 1176]
	ldr	w2, [x1, 108]
	str	w2, [x20, 1180]
	ldr	w2, [x1, 112]
	str	w2, [x20, 1184]
	ldr	w2, [x1, 128]
	str	w2, [x20, 1188]
	str	x3, [x1, 56]
	ldr	x1, [x19, 28168]
	ldr	w2, [x1, 72]
	str	w2, [x20, 1280]
	ldr	w2, [x1, 76]
	str	w2, [x20, 1284]
	ldr	w3, [x1, 64]
	ldr	w2, [x1, 2660]
	cmp	w3, 1
	ubfiz	w3, w2, 5, 3
	cset	w2, ne
	orr	w2, w3, w2, lsl 2
	str	w2, [x20, 1276]
	ldr	w1, [x1, 2644]
	and	w1, w1, 7
	bl	SetAspectRatio
	str	wzr, [x20, 1264]
	ldr	w0, [x19, 220]
	cmp	w0, 1
	beq	.L2920
	ldr	x0, [x20, 1160]
	ldr	w1, [x20, 1840]
	str	w1, [x0, 12]
	ldr	x0, [x20, 1160]
	ldr	w1, [x20, 1276]
	str	w1, [x0, 188]
	ldr	x0, [x20, 1160]
	ldr	w1, [x20, 1264]
	str	w1, [x0, 176]
	ldr	x0, [x20, 1160]
	ldr	w1, [x20, 1172]
	str	w1, [x0, 84]
	ldr	x0, [x20, 1160]
	ldr	w1, [x20, 1168]
	str	w1, [x0, 80]
	ldr	x0, [x20, 1160]
	ldr	w1, [x20, 1280]
	str	w1, [x0, 192]
	ldr	x0, [x20, 1160]
	ldr	w1, [x20, 1284]
	str	w1, [x0, 196]
	ldr	x1, [x20, 1160]
	ldr	w0, [x19, 276]
	ldr	w1, [x1, 32]
	bl	FSP_GetDispPhyFs
	cbz	x0, .L2950
	ldr	w3, [x20, 1264]
	mov	w1, 0
	ldr	w2, [x0, 8]
	cmp	w3, wzr
	ldr	w0, [x19, 276]
	cset	w3, ne
	bl	FSP_SetStoreType
.L2950:
	ldr	x1, [x19, 28168]
	mov	w0, 0
	ldr	x5, [x20, 1160]
	ldr	w2, [x1, 84]
	str	w2, [x20, 1500]
	ldr	w4, [x1, 88]
	str	w4, [x20, 1504]
	ldr	w7, [x1, 92]
	str	w7, [x20, 1508]
	ldr	w6, [x1, 96]
	str	w6, [x20, 1512]
	ldr	w3, [x1, 72]
	sub	w2, w3, w2
	sub	w2, w2, w4
	str	w2, [x20, 1288]
	ldr	w4, [x1, 76]
	sub	w3, w4, w7
	sub	w3, w3, w6
	str	w3, [x20, 1292]
	ldr	w3, [x1, 64]
	str	w3, [x20, 1340]
	ldr	w3, [x1, 100]
	str	w3, [x20, 1344]
	ldr	w1, [x1, 108]
	str	w1, [x20, 1348]
	ldr	w1, [x5, 208]
	str	w1, [x20, 1296]
	ldr	w1, [x5, 212]
	str	w1, [x20, 1300]
	str	w2, [x5, 200]
	ldr	x1, [x20, 1160]
	ldr	w2, [x20, 1292]
	str	w2, [x1, 204]
	ldr	x1, [x20, 1160]
	ldr	w2, [x20, 1296]
	str	w2, [x1, 208]
	ldr	x1, [x20, 1160]
	ldr	w2, [x20, 1300]
	str	w2, [x1, 212]
	ldr	x1, [x20, 1160]
	ldr	w2, [x20, 1344]
	str	w2, [x1, 256]
	ldr	x1, [x20, 1160]
	ldr	w2, [x20, 1348]
	str	w2, [x1, 260]
	str	wzr, [x20, 1116]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L2945:
	ldr	x0, [x19, 2376]
	ldr	w1, [x0, 912]
	cmp	w1, 1
	bne	.L2946
	b	.L2943
	.p2align 3
.L2983:
	ldr	w3, [x28, 68]
	add	x4, x28, 176
	mov	w2, 0
	cmp	w3, wzr
	ble	.L2933
	.p2align 2
.L2968:
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
	bne	.L2968
.L2933:
	ldr	w3, [x28, 72]
	add	x4, x28, 216
	mov	w2, 0
	cmp	w3, wzr
	ble	.L2934
	.p2align 2
.L2967:
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
	bne	.L2967
	b	.L2934
	.p2align 3
.L2980:
	ldr	w2, [x19, 168]
	cbnz	w2, .L2922
	b	.L2921
	.p2align 3
.L2982:
	ldrb	w0, [x28, 1]
	cbnz	w0, .L2925
	ldr	w0, [x19, 168]
	cbz	w0, .L2925
	ldr	w0, [x26, 72]
	cmp	w22, w0
	bne	.L2925
	ldr	w0, [x26, 76]
	cmp	w23, w0
	bne	.L2925
	ldr	w0, [x26, 148]
	ldr	w1, [x29, 108]
	cmp	w1, w0
	bne	.L2925
	b	.L2926
	.p2align 3
.L2981:
	ldrb	w1, [x25, x21]
	cbnz	w1, .L2923
	ldr	w1, [x19, 168]
	cbz	w1, .L2923
	ldr	w1, [x26, 72]
	cmp	w22, w1
	bne	.L2923
	ldr	w1, [x26, 76]
	cmp	w23, w1
	bne	.L2923
	ldr	w1, [x26, 148]
	ldr	w2, [x29, 108]
	cmp	w2, w1
	bne	.L2923
	mov	w21, 0
	b	.L2924
	.p2align 3
.L2985:
	mov	x0, x19
	bl	HEVC_GetBackPicFromVOQueue
	b	.L2948
	.p2align 3
.L2984:
	ldr	x1, .LC865
	mov	w0, 31
	ldr	w3, [x26, 100]
	ldr	w5, [x26, 108]
	ldp	w4, w2, [x29, 96]
	bl	dprint_vfmw
	b	.L2947
.L2949:
	ldr	x1, .LC867
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_ClearAll
	mov	w0, -1
	b	.L2919
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
	cbz	w0, .L2995
	cmn	w0, #3
	mov	w0, 0
	bne	.L2996
.L2989:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2995:
	ldr	w0, [x19, 276]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 276]
	ldr	w1, [x20, 32]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L2997
	mov	x4, x0
	add	x3, x19, 61440
	ldr	w0, [x19, 276]
	add	x3, x3, 1064
	mov	x2, x19
	mov	w1, 16
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L2998
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L2999
	str	wzr, [x21, 24]
.L2992:
	ldr	w1, [x19, 308]
	mov	w0, 0
	ldr	w2, [x19, 316]
	add	w1, w1, 1
	str	w1, [x19, 308]
	add	w2, w2, 1
	str	w2, [x19, 316]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2999:
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.4
	b	.L2992
	.p2align 3
.L2996:
	ldr	x1, .LC876
	mov	w2, 9561
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x20, 236]
	mov	x0, x19
	bl	HEVC_DECRecycleImage
	mov	w0, -1
	b	.L2989
	.p2align 3
.L2998:
	ldr	x1, .LC874
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 276]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	mov	w1, 1
	mov	x0, x19
	bl	HEVC_ClearAll
	mov	w0, -1
	b	.L2989
.L2997:
	ldr	x2, .LC870
	mov	w0, 0
	ldr	x1, .LC872
	add	x2, x2, 112
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2989
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
	ldr	x19, [x20, 1160]
	add	x1, x20, 1168
	mov	x2, 624
	add	x0, x19, 80
	str	w4, [x19, 4]
	strb	wzr, [x19, 1]
	strb	w3, [x19, 2]
	ldr	w3, [x20, 1116]
	str	w3, [x19, 16]
	ldr	w3, [x20, 1144]
	str	w3, [x19, 20]
	bl	memcpy
	str	wzr, [x19, 52]
	ldr	w1, [x19, 32]
	mov	w2, 0
	ldr	w0, [x20, 1144]
	str	w0, [x19, 44]
	ldr	w0, [x21, 276]
	bl	FSP_SetRef
	mov	x1, x19
	mov	x0, x21
	bl	HEVC_OutputFrmToVO
	str	wzr, [x19, 4]
	ldr	x3, .LC877
	mov	w21, w0
	mov	x2, 688
	mov	w1, 0
	add	x0, x20, 1104
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
	ldr	w1, [x2, 1140]
	cmp	w1, w3
	beq	.L3002
	add	x19, x19, x1, uxtw 2
	str	wzr, [x19, 2148]
	str	w3, [x2, 1140]
.L3002:
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
	ldr	w4, [x19, 2716]
	cbz	w4, .L3004
	sub	w3, w4, #1
	add	x0, x19, 2736
	add	x3, x3, 343
	mov	w28, 0
	add	x3, x19, x3, lsl 3
	.p2align 2
.L3006:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cbnz	w2, .L3005
	ldr	w2, [x1, 12]
	ldr	w1, [x19, 244]
	cmp	w2, w1
	csinc	w28, w28, w28, le
.L3005:
	cmp	x3, x0
	bne	.L3006
	ldr	w0, [x19, 228]
	cmp	w0, 1
	beq	.L3017
.L3007:
	add	x23, x19, 53248
	ldr	x1, [x19, 28168]
	ldr	w0, [x23, 1860]
	add	x0, x1, x0, lsl 2
	ldr	w22, [x0, 412]
.L3009:
	cbz	w4, .L3018
	ldr	x25, .LC880
	mov	w20, 0
	ldr	x24, .LC879
	mov	w27, 0
	ldr	x26, .LC882
	add	x23, x19, 53248
	add	x25, x25, 136
	.p2align 2
.L3016:
	add	x21, x19, x20, uxtw 3
	ldrb	w9, [x19, 2]
	ldr	w5, [x19, 244]
	mov	x1, x24
	ldr	w8, [x23, 1840]
	mov	w7, w22
	ldr	x3, [x21, 2736]
	mov	w6, w28
	mov	w2, w20
	mov	w0, 22
	ldr	w4, [x3, 12]
	ldrb	w3, [x3, 2]
	str	w8, [sp]
	str	w9, [sp, 8]
	bl	dprint_vfmw
	ldr	x0, [x21, 2736]
	ldrb	w1, [x0, 2]
	cbnz	w1, .L3011
	ldr	w2, [x0, 12]
	cmp	w22, w28
	ldr	w3, [x19, 244]
	ccmp	w2, w3, 4, cc
	ble	.L3012
	ldr	w4, [x23, 1840]
	cmp	w2, w4
	blt	.L3013
	ldrb	w2, [x19, 2]
	cbnz	w2, .L3013
	.p2align 2
.L3012:
	cbnz	w3, .L3014
	ldr	w4, [x0, 12]
	tbz	w4, #31, .L3014
	cbz	w1, .L3030
.L3015:
	mov	x2, x25
	mov	x1, x26
	mov	w0, 31
	bl	dprint_vfmw
.L3014:
	ldr	w0, [x19, 2716]
	add	w20, w20, 1
	cmp	w0, w20
	bhi	.L3016
.L3029:
	add	sp, x29, 0
	mov	w0, w27
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L3013:
	mov	w1, 1
	strb	w1, [x0, 2]
	mov	x0, x19
	sub	w28, w28, #1
	ldr	x1, [x21, 2736]
	bl	HEVC_OutputFrmToVO
	mov	w27, w0
	ldr	x0, [x21, 2736]
	ldr	w0, [x0, 12]
	str	w0, [x19, 244]
	b	.L3014
	.p2align 3
.L3011:
	ldr	w3, [x19, 244]
	b	.L3012
	.p2align 3
.L3030:
	ldr	w1, [x0, 32]
	mov	w2, 0
	ldr	w0, [x19, 276]
	bl	FSP_SetDisplay
	ldr	x0, [x21, 2736]
	ldr	w3, [x19, 244]
	ldr	w4, [x0, 12]
	b	.L3015
.L3004:
	ldr	w0, [x19, 228]
	mov	w28, 0
	cmp	w0, 1
	bne	.L3007
	.p2align 2
.L3017:
	ldr	x2, [x19, 28168]
	mov	w22, 0
	add	x0, x2, 412
	add	x2, x2, 444
	.p2align 2
.L3008:
	ldr	w1, [x0], 4
	cmp	w22, w1
	csel	w22, w22, w1, cs
	cmp	x0, x2
	bne	.L3008
	b	.L3009
.L3018:
	mov	w27, 0
	b	.L3029
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
	ldr	x1, [x20, 2736]
	strb	w2, [x1, 2]
	ldr	x1, [x20, 2736]
	bl	HEVC_OutputFrmToVO
	mov	w22, w0
	ldr	x0, [x20, 2736]
	ldr	w1, [x0, 12]
	str	w1, [x19, 244]
	ldrb	w0, [x0, 1]
	cbnz	w0, .L3032
	mov	w1, w21
	mov	x0, x19
	bl	HEVC_RemoveApcStore
	mov	w1, w21
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
.L3032:
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
	cbnz	w0, .L3038
	ldr	w1, [x29, 44]
	mov	x0, x19
	bl	HEVC_OutputOneFrmFromDPB
	mov	w1, 0
	cbnz	w0, .L3039
.L3035:
	ldr	x19, [sp, 16]
	mov	w0, w1
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3038:
	ldr	x2, .LC884
	mov	w0, 1
	ldr	x1, .LC886
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w1, -1
	b	.L3035
	.p2align 3
.L3039:
	ldr	x2, .LC884
	mov	w0, 1
	ldr	x1, .LC888
	add	x2, x2, 160
	bl	dprint_vfmw
	mov	w1, -1
	b	.L3035
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
	cbz	x19, .L3049
	ldr	w0, [x19, 2716]
	mov	w20, 0
	mov	w22, 1
	cbnz	w0, .L3052
	b	.L3048
	.p2align 3
.L3056:
	strb	w22, [x1, 2]
	ldr	x1, [x21, 2736]
	bl	HEVC_OutputFrmToVO
	cmn	w0, #1
	mov	w21, w0
	beq	.L3055
.L3045:
	ldr	w0, [x19, 2716]
	cmp	w0, w20
	bls	.L3048
.L3052:
	add	x21, x19, x20, uxtw 3
	ldr	w0, [x19, 276]
	mov	w2, 0
	add	w20, w20, 1
	ldr	x1, [x21, 2736]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldr	x1, [x21, 2736]
	mov	x0, x19
	ldrb	w2, [x1, 2]
	cbz	w2, .L3056
	ldr	w1, [x1, 32]
	ldr	w0, [x19, 276]
	bl	FSP_GetDisplay
	cmp	w0, 3
	mov	w2, 0
	beq	.L3045
	ldr	x1, [x21, 2736]
	ldr	w0, [x19, 276]
	ldr	w1, [x1, 32]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 2716]
	cmp	w0, w20
	bhi	.L3052
.L3048:
	mov	w1, -2147483647
	mov	x0, x19
	str	w1, [x19, 244]
	mov	w21, 0
	bl	Hevc_InitDecBuffers
.L3041:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3055:
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
.L3049:
	mov	w21, -1
	b	.L3041
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
	str	wzr, [x21, 920]
	ldr	x0, [x19, 2392]
	cbz	x0, .L3079
	ldrb	w1, [x20]
	str	w1, [x0, 40]
	ldr	x0, [x19, 2376]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L3080
.L3063:
	ldrb	w0, [x20]
	cmp	w0, 1
	bne	.L3064
	ldr	x1, [x19, 2392]
	ldr	w0, [x1, 8]
	cmp	w0, 1
	bls	.L3065
	ldr	w2, [x1, 40]
	cmp	w2, 1
	beq	.L3064
.L3065:
	ldr	w2, [x20, 20]
	cmp	w2, wzr
	ble	.L3066
	ldr	x3, [x20, 8]
	cbz	x3, .L3066
	ldr	w2, [x20, 16]
	cbnz	w2, .L3081
.L3066:
	mov	w0, 1
	str	w0, [x1, 40]
	ldr	w0, [x19, 276]
	ldr	w1, [x20, 24]
	bl	SM_ReleaseStreamSeg
.L3062:
	ldr	x0, [x19, 2392]
	ldr	w0, [x0, 40]
	cmp	w0, wzr
	csetm	w0, eq
.L3061:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3080:
	ldr	w0, [x0, 912]
	cbnz	w0, .L3062
	b	.L3063
	.p2align 3
.L3081:
	ubfiz	x2, x0, 5, 32
	mov	x0, x19
	add	x1, x1, x2
	str	x3, [x1, 48]
	ldr	x2, [x19, 2392]
	ldr	w3, [x20, 20]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	w3, [x1, 56]
	ldr	x2, [x19, 2392]
	ldr	w3, [x20, 16]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	w3, [x1, 60]
	ldr	x2, [x19, 2392]
	ldr	w3, [x20, 24]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	w3, [x1, 76]
	ldr	x2, [x19, 2392]
	ldr	w1, [x2, 8]
	add	x1, x2, x1, lsl 5
	str	wzr, [x1, 68]
	ldr	x2, [x19, 2392]
	ldr	w1, [x2, 8]
	add	w1, w1, 1
	str	w1, [x2, 8]
	bl	HEVC_CombinePacket
	mov	w0, 1
	str	w0, [x21, 920]
	b	.L3062
	.p2align 3
.L3079:
	ldr	w0, [x20, 20]
	cmp	w0, wzr
	ble	.L3059
	ldr	x0, [x20, 8]
	cbz	x0, .L3059
	ldr	w0, [x20, 16]
	cbz	w0, .L3059
	mov	x0, x19
	bl	HEVC_FindNaluArraySlot
	cmn	w0, #1
	mov	w21, w0
	beq	.L3082
	add	x1, x19, 65536
	sxtw	x0, w0
	lsl	x21, x0, 3
	mov	w2, 1
	sub	x0, x21, x0
	ldr	x21, [x1, 2888]
	add	x0, x21, x0, lsl 4
	str	x0, [x19, 2392]
	ldr	x1, [x20, 8]
	str	x1, [x0, 48]
	ldr	x0, [x19, 2392]
	ldr	w1, [x20, 20]
	str	w1, [x0, 56]
	ldr	x0, [x19, 2392]
	ldr	w1, [x20, 24]
	str	w1, [x0, 76]
	ldr	x0, [x19, 2392]
	ldr	w1, [x20, 16]
	str	w1, [x0, 60]
	ldr	x0, [x19, 2392]
	str	wzr, [x0, 68]
	ldr	x0, [x19, 2392]
	str	w2, [x0, 8]
	ldr	x0, [x19, 2392]
	strb	wzr, [x0, 1]
	ldr	x0, [x19, 2392]
	ldrb	w1, [x20]
	str	w1, [x0, 40]
	ldr	w0, [x20, 28]
	str	w0, [x19, 216]
	b	.L3062
	.p2align 3
.L3064:
	ldr	x1, .LC897
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3061
	.p2align 3
.L3059:
	ldr	x1, .LC895
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3061
.L3082:
	ldr	x1, .LC893
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	mov	x0, x19
	bl	HEVC_ClearAll
	mov	w0, w21
	b	.L3061
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
	cbz	w0, .L3086
	add	x0, x19, 53248
	ldrb	w0, [x0, 1105]
	cmp	w0, 1
	beq	.L3086
	add	x1, x19, 65536
	add	x4, x19, 81920
	add	x1, x1, 3704
	add	x4, x4, 1560
	.p2align 2
.L3092:
	ldr	w0, [x1, 4]
	cbnz	w0, .L3087
	cbz	x1, .L3088
	ldr	w3, [x19, 2716]
	cbz	w3, .L3088
	ldr	x0, [x19, 2736]
	cmp	x0, x1
	beq	.L3087
	sub	w3, w3, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L3089
	.p2align 3
.L3090:
	ldr	x2, [x0, 2736]
	cmp	x2, x1
	beq	.L3087
.L3089:
	cmp	x3, x0
	add	x0, x0, 8
	bne	.L3090
.L3088:
	ldr	w0, [x19, 276]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L3086
	cmn	w0, #1
	beq	.L3111
.L3110:
	mov	w0, 0
.L3112:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3086:
	ldr	x19, [sp, 16]
	mov	w0, 1
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3087:
	add	x1, x1, 712
	cmp	x1, x4
	bne	.L3092
	add	x0, x19, 61440
	add	x2, x29, 44
	add	x1, x29, 40
	add	x0, x0, 1064
	bl	GetQueueImgNum
	mov	x0, x19
	bl	HEVC_PrintFrameStoreState
	ldr	x1, .LC899
	mov	w0, 0
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_PrintDPBState
	ldr	w0, [x19, 176]
	ldr	w2, [x19, 2716]
	ldp	w3, w4, [x29, 40]
	sub	w0, w0, #2
	sub	w0, w0, w2
	add	w1, w3, w4
	cmp	w1, w0
	bge	.L3110
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
.L3111:
	add	x1, x19, 61440
	ldr	w0, [x19, 276]
	add	x1, x1, 1064
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	b	.L3112
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
	ldr	w2, [x19, 2716]
	cbnz	w2, .L3125
	b	.L3115
	.p2align 3
.L3116:
	cmp	w2, w20
	bls	.L3115
.L3125:
	add	x0, x19, x20, uxtw 3
	add	w20, w20, 1
	ldr	x0, [x0, 2736]
	ldrb	w1, [x0, 1]
	cbz	w1, .L3116
	strb	wzr, [x0, 1]
	mov	w2, 0
	ldr	w1, [x0, 32]
	ldr	w0, [x19, 276]
	bl	FSP_SetRef
	ldr	w2, [x19, 2716]
	cmp	w2, w20
	bhi	.L3125
	.p2align 2
.L3115:
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cbz	w0, .L3119
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cbnz	w0, .L3115
	b	.L3119
	.p2align 3
.L3121:
	bl	HEVC_GetMinPOC
	ldr	w1, [x29, 40]
	mov	x0, x19
	bl	HEVC_OutputOneFrmFromDPB
	cmn	w0, #1
	beq	.L3120
.L3119:
	ldr	w3, [x19, 2716]
	add	x2, x29, 40
	add	x1, x29, 44
	mov	x0, x19
	cbnz	w3, .L3121
	mov	w0, 0
.L3120:
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
	cbz	w0, .L3131
	ldr	x2, .LC903
	mov	w3, w0
	ldr	x1, .LC905
	mov	w0, 0
	add	x2, x2, 248
	bl	dprint_vfmw
.L3131:
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
	b	.L3136
	.p2align 3
.L3138:
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_IncreaseDPBSize
	cbnz	w0, .L3146
.L3136:
	ldr	w5, [x19, 2712]
	mov	x2, x21
	ldr	w6, [x19, 2716]
	mov	x1, x22
	mov	w0, 13
	mov	w4, w5
	cmp	w6, w5
	mov	w3, w6
	bcs	.L3138
.L3135:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3146:
	ldr	x1, .LC910
	mov	x2, x21
	mov	w0, 1
	add	x20, x20, 264
	bl	dprint_vfmw
	ldr	x21, .LC912
	.p2align 2
.L3137:
	ldr	w5, [x19, 2712]
	mov	x2, x20
	ldr	w6, [x19, 2716]
	mov	x1, x21
	mov	w0, 1
	mov	w4, w5
	cmp	w6, w5
	mov	w3, w6
	bcc	.L3135
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_OutputUnRefPicWithMinPoc
	cbz	w0, .L3137
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
	bhi	.L3151
	add	x20, x21, x22, uxtw 3
	ldr	x0, [x19, 56]
	mov	w1, 1
	mov	w2, w1
	str	x0, [x20, 2736]
	str	w1, [x0, 4]
	ldr	x1, [x20, 2736]
	ldr	w0, [x21, 276]
	ldr	w1, [x1, 32]
	bl	FSP_SetRef
	ldrb	w1, [x19]
	mov	x2, 624
	ldr	x0, [x20, 2736]
	cmp	w1, wzr
	add	x1, x19, 64
	cset	w3, ne
	strb	w3, [x0, 1]
	ldr	x0, [x20, 2736]
	ldr	w3, [x19, 36]
	str	w3, [x0, 24]
	ldr	x0, [x20, 2736]
	add	x0, x0, 80
	bl	memcpy
	ldr	x0, [x20, 2736]
	add	w1, w22, 1
	ldr	w2, [x19, 40]
	ldr	x3, .LC918
	str	w2, [x0, 20]
	str	w2, [x0, 44]
	mov	w2, 8
	ldr	x0, [x20, 2736]
	ldr	x4, [x19, 56]
	str	x4, [x0, 72]
	ldr	x0, [x20, 2736]
	ldr	w4, [x19, 12]
	str	w4, [x0, 16]
	str	w4, [x0, 48]
	add	x0, x21, 2736
	ldr	x4, [x20, 2736]
	ldr	w5, [x19, 8]
	str	w5, [x4, 12]
	ldr	x4, [x20, 2736]
	ldr	w5, [x19, 8]
	str	w5, [x4, 40]
	bl	qsort
	ldr	w1, [x21, 2716]
	mov	x0, x21
	add	w1, w1, 1
	str	w1, [x21, 2716]
	ldr	x1, [x19, 56]
	bl	HEVC_GetAPC
	cbnz	w0, .L3152
	strb	wzr, [x19, 1]
	mov	w0, 0
.L3149:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3151:
	ldr	x2, .LC915
	mov	w3, w22
	ldr	x1, .LC917
	mov	w0, 0
	add	x2, x2, 288
	bl	dprint_vfmw
	mov	x0, x21
	bl	HEVC_ClearDPB
	mov	w0, -1
	b	.L3149
	.p2align 3
.L3152:
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
	b	.L3149
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
	ldrb	w0, [x19, 1104]
	cbnz	w0, .L3166
	ldr	x0, [x19, 1160]
	mov	w1, 2
	str	w1, [x0, 4]
.L3155:
	ldr	x1, [x19, 1160]
	mov	w2, 1
	mov	x0, x20
	strb	w2, [x1, 2]
	ldr	x1, [x19, 1160]
	bl	HEVC_CheckFrameStore
	cmn	w0, #1
	mov	w19, w0
	beq	.L3167
	cmn	w0, #3
	beq	.L3159
	mov	x0, x20
	bl	HEVC_OutputCurrPic
	cbnz	w0, .L3168
.L3159:
	mov	w0, 0
.L3156:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3166:
	mov	x0, x20
	bl	HEVC_SimpleSlideDPB
	ldr	w1, [x20, 2716]
	add	x2, x19, 1104
	mov	x0, x20
	bl	HEVC_InsertFrmInDPB
	cbz	w0, .L3155
	ldr	x2, .LC921
	mov	w4, w0
	ldr	x1, .LC923
	mov	w3, 8674
	add	x2, x2, 312
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -7
	b	.L3156
	.p2align 3
.L3168:
	ldr	x2, .LC921
	mov	w0, 0
	ldr	x1, .LC926
	add	x2, x2, 312
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3156
.L3167:
	ldr	x2, .LC921
	mov	w0, 0
	ldr	x1, .LC925
	add	x2, x2, 312
	bl	dprint_vfmw
	mov	w0, w19
	b	.L3156
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
	ldr	w0, [x19, 2712]
	ldr	w1, [x19, 2716]
	cmp	w1, w0
	bcs	.L3181
.L3170:
	add	x20, x19, 53248
	mov	x0, x19
	add	x2, x20, 1104
	bl	HEVC_InsertFrmInDPB
	cbnz	w0, .L3182
	ldr	x1, [x20, 1160]
	mov	x0, x19
	bl	HEVC_CheckFrameStore
	cmn	w0, #1
	mov	w20, w0
	beq	.L3183
	cmn	w0, #3
	beq	.L3176
	mov	x0, x19
	bl	HEVC_OutputCurrPic
	cbnz	w0, .L3184
.L3176:
	mov	w0, 0
.L3173:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3181:
	mov	x0, x19
	bl	HEVC_RemoveUnusedApcStore
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	w0, 1
	beq	.L3180
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC929
	add	x2, x2, 336
	bl	dprint_vfmw
.L3180:
	ldr	w1, [x19, 2716]
	b	.L3170
.L3184:
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC933
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3173
	.p2align 3
.L3182:
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC931
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3173
.L3183:
	ldr	x2, .LC927
	mov	w0, 0
	ldr	x1, .LC932
	add	x2, x2, 336
	bl	dprint_vfmw
	mov	w0, w20
	b	.L3173
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
	ldr	w0, [x0, 1672]
	cmp	w0, 1
	beq	.L3186
	mov	x0, x19
	bl	HEVC_FirstFrameFastOut
	cbnz	w0, .L3203
.L3186:
	ldr	w0, [x19, 2712]
	ldr	w1, [x19, 2716]
	cmp	w1, w0
	bcs	.L3204
.L3188:
	add	x2, x19, 53248
	mov	x0, x19
	add	x2, x2, 1104
	bl	HEVC_InsertFrmInDPB
	cbnz	w0, .L3205
	ldr	w0, [x19, 2716]
	cbnz	w0, .L3191
.L3192:
	mov	w0, 0
.L3187:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3191:
	mov	x0, x19
	bl	HEVC_OutputFrmFromDPB
	cbz	w0, .L3192
	ldr	x2, .LC934
	mov	w0, 1
	ldr	x1, .LC942
	add	x2, x2, 360
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3187
	.p2align 3
.L3204:
	mov	x0, x19
	bl	HEVC_RemoveUnusedApcStore
	mov	x0, x19
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	w0, 1
	beq	.L3202
	ldr	x1, .LC938
	mov	w0, 0
	bl	dprint_vfmw
.L3202:
	ldr	w1, [x19, 2716]
	b	.L3188
	.p2align 3
.L3205:
	ldr	x1, .LC940
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3187
.L3203:
	ldr	x2, .LC934
	mov	w0, 0
	ldr	x1, .LC936
	add	x2, x2, 360
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3187
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
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 636]
	cmp	w0, 1
	beq	.L3240
	add	x20, x19, 53248
	mov	x0, x19
	str	wzr, [x20, 1248]
	str	wzr, [x20, 1252]
	bl	HEVC_RemoveDummyFrame
	ldrb	w0, [x20, 1105]
	cbz	w0, .L3241
.L3210:
	ldr	x0, [x19, 2376]
	ldr	w1, [x20, 1144]
	ldr	w0, [x0, 4]
	cmp	w1, w0
	bhi	.L3242
	ldr	x1, [x19, 2384]
	ldr	x0, [x20, 1160]
	ldr	w1, [x1, 5288]
	str	w1, [x0, 704]
	ldr	x1, [x19, 2384]
	ldr	x0, [x20, 1160]
	ldr	w1, [x1, 5292]
	str	w1, [x0, 708]
	ldr	w0, [x19, 280]
	cmp	w0, 2
	beq	.L3243
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	beq	.L3244
	cmp	w0, 1
	beq	.L3245
	cbnz	w0, .L3224
.L3223:
	mov	x0, x19
	bl	HEVC_DispOrderProcess
	mov	w21, w0
	cbnz	w0, .L3221
.L3225:
	mov	w21, 0
.L3222:
	strb	wzr, [x20, 1105]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3240:
	ldr	x1, .LC943
	add	x20, x19, 53248
	ldr	x1, [x1]
	ldr	w2, [x1, 4180]
	ldr	w1, [x1, 4184]
	str	w0, [x20, 1248]
	mov	x0, x19
	cmp	w1, wzr
	csinc	w1, w1, wzr, ne
	udiv	w1, w2, w1
	str	w1, [x20, 1252]
	bl	HEVC_RemoveDummyFrame
	ldrb	w0, [x20, 1105]
	cbnz	w0, .L3210
.L3241:
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
.L3244:
	mov	x0, x19
	bl	HEVC_SimpleDPBProcess
	mov	w21, w0
	cbnz	w0, .L3246
.L3218:
	ldr	x1, .LC952
	mov	w3, 9003
	mov	w2, w21
	mov	w0, 14
	bl	dprint_vfmw
.L3216:
	cmn	w21, #1
	bne	.L3222
	.p2align 2
.L3224:
	mov	x0, x19
	mov	w21, -1
	bl	HEVC_ClearCurrPic
	ldr	x2, .LC944
	mov	w0, 1
	ldr	x1, .LC960
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3222
	.p2align 3
.L3242:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	ldr	x3, [x19, 2376]
	mov	w0, 1
	ldr	w2, [x20, 1144]
	ldr	x1, .LC948
	ldr	w3, [x3, 4]
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3243:
	mov	x0, x19
	bl	HEVC_DirectOutput
	mov	w21, w0
	ldr	w1, [x19, 180]
	ldr	w0, [x20, 1140]
	cmp	w0, w1
	beq	.L3214
	add	x0, x19, x0, uxtw 2
	str	wzr, [x0, 2148]
	str	w1, [x20, 1140]
.L3214:
	cbnz	w21, .L3247
.L3215:
	ldr	x1, .LC952
	mov	w3, 8988
	mov	w2, w21
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3216
	.p2align 3
.L3245:
	mov	x0, x19
	bl	HEVC_DecOrderProcess
	mov	w21, w0
	cbnz	w0, .L3248
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 12]
	cbnz	w0, .L3225
	b	.L3223
	.p2align 3
.L3221:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC958
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3216
	.p2align 3
.L3248:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC956
	add	x2, x2, 384
	bl	dprint_vfmw
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 12]
	cbnz	w0, .L3216
	b	.L3223
	.p2align 3
.L3247:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC950
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3215
	.p2align 3
.L3246:
	ldr	x2, .LC944
	mov	w0, 0
	ldr	x1, .LC954
	add	x2, x2, 384
	bl	dprint_vfmw
	b	.L3218
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
	add	x19, x19, 3704
	mov	w22, w1
	bl	_mcount
	.p2align 2
.L3255:
	ldr	w0, [x19, 4]
	cbnz	w0, .L3250
	cbz	x19, .L3251
	ldr	w2, [x20, 2716]
	cbz	w2, .L3251
	ldr	x0, [x20, 2736]
	cmp	x19, x0
	beq	.L3250
	sub	w2, w2, #1
	mov	x0, x20
	add	x2, x20, x2, lsl 3
	b	.L3253
	.p2align 3
.L3254:
	ldr	x1, [x0, 2736]
	cmp	x19, x1
	beq	.L3250
.L3253:
	cmp	x0, x2
	add	x0, x0, 8
	bne	.L3254
.L3251:
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
	add	x1, x21, 3784
	bl	memcpy
	strb	w24, [x21, 3706]
	strb	w24, [x21, 3704]
	cmp	w23, 19
	str	wzr, [x21, 3712]
	str	x19, [x29, 120]
	str	wzr, [x29, 288]
	bgt	.L3256
	ldr	w5, [x20, 2716]
	cbz	w5, .L3258
	mov	w6, 0
	mov	w3, 2147483647
	mov	w1, 0
	.p2align 2
.L3261:
	add	x0, x20, x1, sxtw 3
	add	w1, w1, 1
	ldr	x4, [x0, 2736]
	ldr	w0, [x4, 12]
	sub	w0, w0, w22
	sxtw	x0, w0
	eor	x2, x0, x0, asr 63
	sub	x2, x2, x0, asr 63
	cmp	w2, w3
	bge	.L3260
	ldrb	w0, [x4, 1]
	cmp	w0, wzr
	csel	w3, w3, w2, eq
	csinc	w6, w6, wzr, eq
.L3260:
	cmp	w1, w5
	bne	.L3261
	cbz	w6, .L3258
	mov	w1, 0
	b	.L3262
	.p2align 3
.L3263:
	cmp	w5, w1
	beq	.L3264
.L3262:
	sxtw	x19, w1
	add	w1, w1, 1
	add	x19, x19, 340
	add	x19, x20, x19, lsl 3
	ldr	x2, [x19, 16]
	ldr	w0, [x2, 12]
	sub	w0, w0, w22
	sxtw	x0, w0
	eor	x4, x0, x0, asr 63
	sub	x4, x4, x0, asr 63
	cmp	w3, w4
	bne	.L3263
	ldrb	w0, [x2, 1]
	cbz	w0, .L3263
	ldr	w1, [x2, 32]
	ldr	w0, [x20, 276]
	bl	FSP_GetLogicFs
	mov	x21, x0
	ldr	x2, [x19, 16]
	mov	w3, w22
	ldr	x1, .LC968
	mov	w0, 1
	ldr	w2, [x2, 12]
	bl	dprint_vfmw
	cbz	x21, .L3264
	ldr	x0, [x21, 640]
	cbz	x0, .L3297
	ldr	w0, [x20, 276]
	mov	w1, 0
	bl	FSP_NewLogicFs
	mov	w1, w0
	mov	w19, w0
	ldr	w0, [x20, 276]
	bl	FSP_GetLogicFs
	mov	x1, x0
	cmp	w19, wzr
	ccmp	x0, xzr, 4, ge
	beq	.L3267
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
	ldr	w1, [x20, 2716]
	str	w3, [x29, 100]
	bl	HEVC_InsertFrmInDPB
	cbnz	w0, .L3298
	ldr	w0, [x20, 276]
	mov	w2, 0
	mov	w1, w19
	bl	FSP_SetDisplay
	ldr	w2, [x20, 200]
	mov	w1, 2147483647
	mov	w0, 0
	cmp	w2, w1
	bne	.L3283
	str	w22, [x20, 200]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 688
	ret
	.p2align 3
.L3250:
	add	w23, w23, 1
	add	x19, x19, 712
	cmp	w23, 20
	bne	.L3255
.L3256:
	ldr	x1, .LC962
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L3283:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	add	sp, sp, 688
	ret
	.p2align 3
.L3264:
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
.L3267:
	ldr	x2, [x29, 120]
	mov	w0, 0
	ldr	x1, .LC972
	strb	wzr, [x2]
	bl	dprint_vfmw
	mov	w0, -4
	b	.L3283
.L3298:
	ldr	x0, [x29, 120]
	mov	w2, 1
	mov	w1, w19
	strb	wzr, [x0]
	ldr	w0, [x20, 276]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC974
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3283
.L3258:
	ldr	x1, .LC966
	mov	w0, 0
	strb	wzr, [x19]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3283
.L3297:
	ldr	x1, .LC970
	bl	dprint_vfmw
	mov	x0, x20
	bl	HEVC_ClearDPB
	mov	w0, -4
	b	.L3283
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
	b	.L3300
	.p2align 3
.L3303:
	ldr	w2, [x29, 44]
	mov	x1, x20
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 2376]
	mov	x0, x19
	ldr	w1, [x1, 4]
	cbz	w1, .L3301
	ldr	w1, [x29, 44]
	bl	HEVC_CreateLostPicture
	cbnz	w0, .L3306
.L3300:
	add	x1, x29, 44
	mov	x0, x19
	bl	HEVC_CheckThatAllRefPicsAreAvailable
	cbz	w0, .L3303
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
.L3301:
	add	x19, x19, 53248
	ldr	w4, [x29, 44]
	ldr	x1, .LC978
	mov	w2, 0
	mov	w0, 1
	ldr	w3, [x19, 1840]
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3306:
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
	ldr	x0, [x19, 2392]
	ldr	w1, [x0, 16]
	str	w1, [x20, 1864]
	ldr	w0, [x0, 32]
	str	w0, [x20, 1860]
	strb	wzr, [x19, 1]
	ldr	w0, [x20, 3380]
	cmp	w0, 1
	beq	.L3354
.L3308:
	ldr	w0, [x19, 144]
	ldr	w2, [x19, 208]
	cmp	w2, w0
	bcs	.L3355
	ldrb	w0, [x20, 1105]
	cbz	w0, .L3356
	mov	x0, x19
	bl	HEVC_PicTypeStatistic
	mov	x0, x19
	bl	HEVC_CalcStreamBits
	ldr	w0, [x20, 3380]
	cmp	w0, 2
	beq	.L3321
	ldr	w0, [x19, 220]
	cbnz	w0, .L3322
	ldrb	w0, [x20, 1795]
	cbz	w0, .L3357
.L3322:
	mov	x0, x19
	bl	HEVC_WriteSliceMsg
	cbnz	w0, .L3358
.L3325:
	ldr	w0, [x19, 220]
	cbz	w0, .L3359
.L3327:
	ldr	w1, [x19, 5284]
	mov	w0, 0
	str	w1, [x20, 1148]
	ldr	w1, [x19, 5288]
	str	w1, [x20, 1152]
.L3313:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3359:
	ldr	w0, [x20, 1868]
	mov	w1, 1
	str	w0, [x19, 320]
	ldr	w0, [x20, 1840]
	str	w0, [x19, 284]
	str	w1, [x19, 168]
	b	.L3327
	.p2align 3
.L3354:
	ldr	w0, [x19, 220]
	cbnz	w0, .L3310
	sub	w1, w1, #16
	cmp	w1, 4
	bls	.L3311
	ldr	w0, [x20, 1116]
	cbnz	w0, .L3312
	ldr	x0, [x19, 2376]
	ldr	w1, [x0, 680]
	cmp	w1, 1
	beq	.L3360
	.p2align 2
.L3312:
	ldrb	w21, [x19, 2]
	cmp	w21, 1
	beq	.L3361
.L3314:
	ldr	x0, [x19, 2376]
	ldr	w1, [x0, 8]
	cmp	w1, 2
	beq	.L3310
	ldr	w0, [x0, 12]
	cmp	w0, 2
	mov	x0, x19
	beq	.L3362
	bl	HEVC_RefPicProcess
	cbnz	w0, .L3328
.L3310:
	mov	x0, x19
	bl	HEVC_InitPic
	cmn	w0, #2
	beq	.L3313
	cbnz	w0, .L3363
	ldr	w1, [x19, 252]
	mov	x0, x19
	add	w1, w1, 1
	str	w1, [x19, 252]
	bl	HEVC_SetScalingList
	mov	x0, x19
	bl	HEVC_WritePicMsg
	cbz	w0, .L3308
	ldr	x1, .LC987
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3321:
	ldr	x1, [x19, 2392]
	cbz	x1, .L3325
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
	b	.L3325
	.p2align 3
.L3357:
	mov	x0, x19
	bl	HEVC_DecList
	cbnz	w0, .L3364
	mov	x0, x19
	bl	HEVC_IsRefListWrong
	cbz	w0, .L3322
	mov	x0, x19
	bl	HEVC_PrintDPBState
	ldr	x1, .LC998
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3360:
	ldr	w1, [x0, 676]
	cbnz	w1, .L3312
	ldr	w1, [x0, 684]
	cbnz	w1, .L3312
	ldr	w0, [x0, 644]
	add	w0, w0, 1024
	cmp	w0, 2048
	bls	.L3312
	.p2align 2
.L3311:
	mov	x0, x19
	bl	HEVC_FlushOutputFrmFromDPB
	cbz	w0, .L3312
	ldr	x2, .LC979
	mov	w0, 1
	ldr	x1, .LC981
	add	x2, x2, 408
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3361:
	ldr	w0, [x19, 2716]
	cbz	w0, .L3314
	mov	x0, x19
	bl	HEVC_EmptyFrmFromDPB
	cbz	w0, .L3314
	ldr	x2, .LC979
	mov	w0, w21
	ldr	x1, .LC983
	add	x2, x2, 408
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3328:
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3362:
	bl	HEVC_UpdateReflist
	mov	x0, x19
	bl	HEVC_UpdateLTReflist
	b	.L3310
	.p2align 3
.L3356:
	ldr	x1, .LC994
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3355:
	ldr	x1, .LC989
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x19, 144]
	ldr	w2, [x19, 208]
	cmp	w2, w1
	bcc	.L3319
	ldr	x0, .LC990
	ldr	x4, [x0]
	cbz	x4, .L3319
	ldr	w0, [x19, 276]
	mov	w3, 8
	stp	w2, w1, [x29, 56]
	add	x2, x29, 56
	mov	w1, 108
	blr	x4
.L3319:
	ldr	x1, .LC992
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3358:
	ldr	x1, .LC1000
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
	.p2align 3
.L3363:
	ldr	x1, .LC985
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
.L3364:
	ldr	x1, .LC996
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3313
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
	mov	w23, w1
	mov	x21, x3
	mov	w22, w4
	mov	w20, w5
	bl	_mcount
	add	x2, x19, 53248
	str	w23, [x2, 1144]
	ldr	w0, [x19, 240]
	cmp	w0, 1
	bls	.L3366
.L3369:
	ldr	w0, [x2, 1120]
	cmp	w0, 1920
	bhi	.L3383
.L3367:
	ldr	x0, [x19, 2376]
.L3370:
	ldr	w0, [x0, 664]
	str	w0, [x19, 212]
	cbz	w22, .L3373
.L3386:
	ldr	x5, .LC1001
	sbfiz	x20, x20, 6, 32
	add	x20, x5, x20
	ldr	w0, [x20, 44]
	cmp	w0, 3
	beq	.L3384
	cmp	w0, 1
	beq	.L3373
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_StorePicInDPB
	cbnz	w0, .L3385
.L3377:
	mov	w1, 1
	mov	w0, 0
	str	w1, [x19, 168]
.L3375:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3383:
	ldr	x0, [x19, 2376]
	ldr	w1, [x0, 8]
	cmp	w1, 2
	beq	.L3370
	str	wzr, [x19, 212]
	cbnz	w22, .L3386
.L3373:
	mov	x0, x19
	bl	HEVC_ClearAllSlice
	mov	x1, x21
	mov	x0, x19
	bl	HEVC_StorePicInDPB
	cbz	w0, .L3377
.L3385:
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
.L3366:
	add	w0, w0, 1
	str	w0, [x19, 240]
	cmp	w0, 1
	bls	.L3367
	b	.L3369
	.p2align 3
.L3384:
	mov	x0, x19
	bl	HEVC_ClearAllSlice
	mov	w0, 0
	b	.L3375
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
	bhi	.L3388
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	ldr	x0, .LC1004
	ldr	x4, [x0]
	cbz	x4, .L3389
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3389:
	ldr	w0, [x19, 256]
	cmp	w0, 524288
	bhi	.L3449
	ldr	x1, .LC1008
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
.L3442:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3388:
	ldr	w0, [x19, 312]
	add	x21, x19, 53248
	ldr	w2, [x19, 316]
	add	w1, w0, 1
	ldr	x3, [x19, 2376]
	cmp	w1, w2
	mov	x0, x19
	csel	w1, w1, w2, ge
	str	w1, [x19, 312]
	sub	w1, w1, w2
	str	w1, [x3, 616]
	bl	HEVC_IsRefForCurrPic
	strb	w0, [x21, 1104]
	uxtb	w3, w0
	ldr	w20, [x19, 264]
	ldr	w2, [x19, 280]
	ubfx	x20, x20, 16, 3
	cmp	w20, w2
	beq	.L3392
	sub	w1, w2, #2
	cmp	w1, 1
	ccmp	w20, 2, 4, ls
	beq	.L3392
	cmp	w20, 1
	ccmp	w20, 4, 4, hi
	beq	.L3450
.L3392:
	cmp	w20, 2
	str	w20, [x19, 280]
	beq	.L3394
	cmp	w20, 3
	beq	.L3395
	cmp	w20, 1
	beq	.L3451
	ldr	w1, [x19, 248]
	cbz	w1, .L3446
	cmp	w20, 4
	bne	.L3401
	ldr	w1, [x21, 1116]
	cbnz	w1, .L3402
	mov	w0, 2
	strb	w0, [x19, 4]
	.p2align 2
.L3403:
	ldr	w2, [x19, 252]
	mov	w0, 2
	ldr	x1, .LC1015
	sub	w2, w2, #1
	bl	dprint_vfmw
	ldr	w2, [x19, 208]
	cbz	w2, .L3452
	ldr	w1, [x21, 1132]
	add	x0, x19, 61440
	add	x3, x19, 65536
	cmp	w1, wzr
	cset	w4, eq
	str	w4, [x0, 3012]
	ldr	w4, [x21, 1136]
	str	w4, [x0, 3004]
	str	w2, [x3, 148]
	ldr	x2, [x19, 2376]
	ldr	w4, [x2, 632]
	str	w4, [x0, 2996]
	ldr	w2, [x2, 672]
	str	w2, [x0, 3000]
	ldr	w2, [x19, 252]
	cbz	w2, .L3447
	add	x0, x0, 1680
	str	x0, [x19, 2384]
	add	w1, w1, 1
	ldr	w2, [x21, 1148]
	str	w2, [x0, 5288]
	ldr	w2, [x21, 1152]
	str	w2, [x0, 5292]
	str	w1, [x21, 1132]
	str	wzr, [x0, 5284]
	ldr	w0, [x21, 1116]
	cbnz	w0, .L3447
	ldr	x1, [x3, 152]
	mov	w4, 0
	cbz	x1, .L3410
	.p2align 2
.L3411:
	ldp	w2, w0, [x1]
	ldr	x1, [x1, 1360]
	add	w0, w2, w0
	add	w0, w0, 7
	add	w4, w4, w0, lsr 3
	cbnz	x1, .L3411
.L3410:
	ldr	x0, .LC1004
	ldr	x5, [x0]
	cbz	x5, .L3447
	add	x2, x29, 64
	ldr	w0, [x19, 276]
	mov	w3, 4
	mov	w1, 20
	str	w4, [x2, -8]!
	blr	x5
.L3447:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3449:
	ldr	x1, .LC1006
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	mov	w0, -8
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3451:
	ldr	w1, [x21, 1116]
	cmp	w1, 2
	ccmp	w3, wzr, 0, eq
	beq	.L3398
.L3397:
	ldr	w1, [x19, 248]
	cbz	w1, .L3446
.L3401:
	ldr	w1, [x21, 1116]
.L3402:
	cmp	w1, 2
	beq	.L3453
	cmp	w1, 1
	bne	.L3403
	strb	w1, [x19, 4]
	b	.L3403
.L3450:
	ldr	x1, .LC1010
	mov	w3, w20
	mov	w0, 1
	bl	dprint_vfmw
	str	w20, [x19, 280]
.L3395:
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
.L3394:
	ldr	w1, [x21, 1116]
	cbz	w1, .L3397
	b	.L3398
	.p2align 3
.L3453:
	ldrb	w1, [x19, 4]
	cmp	w1, 2
	bne	.L3403
	ldr	x1, [x19, 2376]
	ldr	w1, [x1, 12]
	cbnz	w1, .L3403
	ldr	x1, .LC1011
	ldrb	w1, [x1]
	cmp	w1, 1
	beq	.L3403
	ldr	x1, .LC1013
	mov	w0, 1
	ldr	w2, [x21, 1112]
	bl	dprint_vfmw
	.p2align 2
.L3446:
	ldr	x0, .LC1004
	ldr	x4, [x0]
	cbz	x4, .L3398
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3398:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	mov	w0, -1
	b	.L3442
	.p2align 3
.L3452:
	ldr	x0, .LC1004
	ldr	x4, [x0]
	cbz	x4, .L3407
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3407:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	ldr	x2, .LC1016
	mov	w0, 0
	ldr	x1, .LC1018
	add	x2, x2, 424
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3442
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
	.global	HEVCDEC_CheckSedOnly
	.type	HEVCDEC_CheckSedOnly, %function
HEVCDEC_CheckSedOnly:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 216]
	str	wzr, [x19, 220]
	cmp	w0, 2
	str	wzr, [x19, 228]
	beq	.L3484
	cmp	w0, 1
	mov	w0, 0
	beq	.L3485
.L3476:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3485:
	ldr	w1, [x19, 224]
	cmp	w1, 1
	bne	.L3476
	str	wzr, [x19, 224]
	.p2align 2
.L3482:
	mov	w0, -1
	b	.L3476
	.p2align 3
.L3484:
	ldr	w0, [x19, 212]
	cbz	w0, .L3482
	ldr	w1, [x19, 224]
	mov	w0, 0
	cmp	w1, 1
	beq	.L3476
	ldr	w0, [x19, 276]
	bl	VCTRL_CheckFreshSeg
	cbz	w0, .L3482
	ldr	w0, [x19, 276]
	bl	VCTRL_CheckFreshRaw
	cbz	w0, .L3482
	ldr	x1, [x19, 2392]
	ldr	x0, [x19, 2344]
	ldr	w2, [x1, 56]
	ldr	x1, [x1, 48]
	bl	BsInit
	ldr	x0, [x19, 2344]
	mov	w1, 24
	bl	BsSkip
	ldr	x0, [x19, 2344]
	bl	BsResidBits
	cmp	w0, 15
	ble	.L3482
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsSkip
	ldr	x0, [x19, 2344]
	mov	w1, 6
	bl	BsGet
	mov	w20, w0
	ldr	x0, [x19, 2344]
	mov	w1, 9
	bl	BsSkip
	mov	w0, w20
	bl	HEVC_IsSliceUnit
	sxtb	w21, w0
	cmp	w21, 1
	beq	.L3486
	ldr	w0, [x19, 208]
	cbz	w0, .L3482
	sub	w0, w20, #32
	sub	w20, w20, #48
	cmp	w0, 2
	ccmp	w20, 1, 0, hi
	bhi	.L3482
.L3463:
	mov	x0, x19
	bl	HEVC_DecVDM
	cbnz	w0, .L3482
	add	x2, x19, 53248
	mov	w1, 1
	str	w1, [x19, 228]
	mov	w0, -1
	strb	w1, [x2, 1104]
	b	.L3476
	.p2align 3
.L3486:
	ldr	x0, [x19, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	mov	w0, 0
	ble	.L3460
	ldr	x0, [x19, 2344]
	mov	w1, w21
	bl	BsGet
.L3460:
	ldr	w1, [x19, 208]
	cbnz	w1, .L3487
.L3461:
	add	x1, x19, 53248
	mov	w0, 1
	stp	w0, wzr, [x19, 220]
	mov	w0, 0
	ldrb	w1, [x1, 1105]
	str	w1, [x19, 232]
	b	.L3476
.L3487:
	sub	w1, w20, #32
	sub	w20, w20, #48
	cmp	w1, 2
	ccmp	w20, 1, 0, hi
	ccmp	w0, wzr, 0, hi
	beq	.L3461
	b	.L3463
	.size	HEVCDEC_CheckSedOnly, .-HEVCDEC_CheckSedOnly
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_SetDecParam
	.type	HEVCDEC_SetDecParam, %function
HEVCDEC_SetDecParam:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w20, [x19, 224]
	cmp	w20, 1
	beq	.L3497
.L3489:
	mov	x0, x19
	bl	HEVCDEC_IsNeedSedOnly
	cmp	w0, 1
	bne	.L3488
	add	x2, x19, 65536
	add	x1, x19, 61440
	add	x3, x1, 1680
	str	w0, [x2, 2868]
	ldr	w0, [x19, 208]
	str	w0, [x19, 236]
	str	w0, [x2, 148]
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 672]
	str	w0, [x1, 3000]
	str	wzr, [x19, 224]
	str	x3, [x19, 2384]
.L3488:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3497:
	ldr	w0, [x19, 208]
	cbz	w0, .L3489
	ldr	x0, [x19, 2384]
	cbnz	x0, .L3489
	mov	x0, x19
	bl	HEVC_DecVDM
	cbz	w0, .L3498
	ldr	x1, [x19, 2392]
	cbz	x1, .L3488
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
	b	.L3488
.L3498:
	add	x0, x19, 53248
	str	w20, [x19, 228]
	strb	w20, [x0, 1104]
	b	.L3488
	.size	HEVCDEC_SetDecParam, .-HEVCDEC_SetDecParam
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
	ldr	x3, [x19, 2392]
	ldr	w0, [x3, 8]
	cbz	w0, .L3504
	ldr	w2, [x19, 256]
	mov	w0, 0
	.p2align 2
.L3503:
	ubfiz	x1, x0, 5, 32
	add	w0, w0, 1
	add	x1, x3, x1
	ldr	w1, [x1, 56]
	add	w2, w2, w1
	str	w2, [x19, 256]
	ldr	w1, [x3, 8]
	cmp	w1, w0
	bhi	.L3503
.L3504:
	strb	wzr, [x3, 1]
	ldr	x0, [x19, 2392]
	ldr	w1, [x0, 16]
	sub	w1, w1, #36
	cmp	w1, 1
	bls	.L3642
.L3502:
	ldr	w1, [x0, 4]
	add	w1, w1, 16
	str	w1, [x0, 4]
	ldr	x1, [x19, 2392]
	ldr	w2, [x1, 16]
	cmp	w2, 49
	bls	.L3643
.L3505:
	ldr	x1, .LC1030
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x19, 2392]
	ldr	w0, [x1, 16]
	cmp	w0, 63
	bhi	.L3532
	cbnz	x1, .L3628
	.p2align 2
.L3626:
	mov	w0, 0
.L3646:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3643:
	ldr	x0, .LC1019
	ldrb	w0, [x0,w2,uxtw]
	adr	x3, .Lrtx3507
	add	x0, x3, w0, sxtb #2
	br	x0
.Lrtx3507:
	.section	.rodata
	.align	0
	.align	2
.L3507:
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3506 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3508 - .Lrtx3507) / 4
	.byte	(.L3509 - .Lrtx3507) / 4
	.byte	(.L3510 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3511 - .Lrtx3507) / 4
	.byte	(.L3511 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3505 - .Lrtx3507) / 4
	.byte	(.L3512 - .Lrtx3507) / 4
	.byte	(.L3513 - .Lrtx3507) / 4
	.text
	.p2align 3
.L3642:
	mov	w1, 1
	strb	w1, [x19, 1]
	b	.L3502
.L3513:
	mov	x0, x19
	bl	HEVC_IsEosNal
	uxtb	w1, w0
	cmp	w1, 1
	beq	.L3644
.L3529:
	ldr	x1, [x19, 2392]
	cbz	x1, .L3626
.L3628:
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
	mov	w0, 0
.L3625:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L3506:
	mov	x0, x19
	bl	HEVC_DecSlice
	cmn	w0, #2
	beq	.L3625
	cbz	w0, .L3626
	ldr	x1, [x19, 2392]
	cbz	x1, .L3640
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3640:
	ldr	x0, .LC1022
	ldr	x4, [x0]
	cbz	x4, .L3627
.L3629:
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3627:
	mov	w0, -1
.L3645:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L3509:
	mov	x0, x19
	bl	HEVC_DecSPS
	mov	w20, w0
	ldr	x1, [x19, 2392]
	cbz	x1, .L3519
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3519:
	cbz	w20, .L3626
	ldr	x1, .LC1024
	b	.L3636
.L3510:
	mov	x0, x19
	bl	HEVC_DecPPS
	mov	w20, w0
	ldr	x1, [x19, 2392]
	cbz	x1, .L3521
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3521:
	cbz	w20, .L3626
	ldr	x1, .LC1026
	b	.L3636
.L3508:
	mov	x0, x19
	bl	HEVC_DecVPS
	mov	w20, w0
	ldr	x1, [x19, 2392]
	cbz	x1, .L3514
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3514:
	cbz	w20, .L3626
	ldr	x1, .LC1021
.L3636:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, .LC1022
	ldr	x4, [x21]
	cbz	x4, .L3627
	add	x2, x29, 64
	ldr	w0, [x19, 276]
	mov	w20, -1
	mov	w3, 4
	mov	w1, 100
	str	w20, [x2, -8]!
	blr	x4
	ldr	x4, [x21]
	cbz	x4, .L3627
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
.L3511:
	mov	x0, x19
	bl	HEVC_DecSEI
	mov	w20, w0
	ldr	x1, [x19, 2392]
	cbz	x1, .L3522
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3522:
	cbz	w20, .L3626
	ldr	x1, .LC1028
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3640
.L3512:
	ldrb	w0, [x19, 6]
	cmp	w0, 1
	beq	.L3527
	cbz	x1, .L3526
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3526:
	add	x0, x19, 53248
	ldrb	w0, [x0, 1105]
	cbz	w0, .L3626
.L3527:
	add	x20, x19, 2672
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
	bne	.L3626
	ldr	x20, .LC1022
	ldr	x4, [x20]
	cbz	x4, .L3528
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3528:
	mov	x0, x19
	bl	HEVC_DecVDM
	cbz	w0, .L3626
.L3638:
	ldr	x4, [x20]
	cbnz	x4, .L3629
	mov	w0, -1
	b	.L3645
.L3532:
	ldr	x20, .LC1022
	ldr	x4, [x20]
	cbz	x4, .L3533
	ldr	w0, [x19, 276]
	mov	w1, 102
	mov	w3, 0
	mov	x2, 0
	blr	x4
	ldr	x1, [x19, 2392]
	cbz	x1, .L3534
.L3535:
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3534:
	ldr	x4, [x20]
	cbz	x4, .L3626
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, 0
	b	.L3646
.L3533:
	cbnz	x1, .L3535
	mov	w0, 0
	b	.L3646
.L3644:
	ldr	x20, .LC1022
	strb	w0, [x19, 7]
	ldr	x4, [x20]
	cbz	x4, .L3530
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3530:
	ldr	w0, [x19, 212]
	cmp	w0, 1
	bne	.L3531
	add	x0, x19, 53248
	ldrb	w0, [x0, 1105]
	cbz	w0, .L3626
.L3531:
	mov	x0, x19
	bl	HEVC_DecVDM
	cbz	w0, .L3529
	b	.L3638
	.size	HEVC_DecOneNal, .-HEVC_DecOneNal
	.align	3
.LC1019:
	.xword	.L3507
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
	.global	HEVCDEC_VDMProcSedOnly
	.type	HEVCDEC_VDMProcSedOnly, %function
HEVCDEC_VDMProcSedOnly:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	sxtw	x3, w20
	ldr	x1, [x19, 2384]
	lsl	x0, x3, 2
	ldr	x20, .LC1032
	sub	x3, x0, x3
	ldr	x4, .LC1031
	lsl	x0, x3, 4
	ldrsw	x5, [x19, 276]
	ldp	w21, w2, [x1, 16]
	sub	x3, x0, x3
	ldr	x1, .LC1034
	mov	w0, 22
	ldr	x22, [x4, x5, lsl 3]
	add	x20, x20, x3, lsl 2
	madd	w21, w2, w21, w21
	ldr	w3, [x20, 20]
	add	w21, w21, w2
	mov	w2, w21
	bl	dprint_vfmw
	ldr	w0, [x20, 20]
	cmp	w21, w0
	ldr	w0, [x19, 220]
	beq	.L3657
	cmp	w0, 1
	beq	.L3658
.L3652:
	str	xzr, [x19, 2384]
.L3650:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3658:
	ldr	w2, [x19, 172]
	add	x0, x19, 53248
	ldr	w3, [x19, 208]
	ldr	w1, [x19, 248]
	sub	w2, w2, #1
	sub	w3, w3, #1
	str	w2, [x19, 172]
	sub	w1, w1, #1
	str	w3, [x19, 208]
	str	w1, [x19, 248]
	str	wzr, [x19, 224]
	ldr	w2, [x19, 232]
	strb	w2, [x0, 1105]
	ldr	x1, [x19, 2392]
	cbz	x1, .L3652
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
	b	.L3652
	.p2align 3
.L3657:
	cmp	w0, 1
	beq	.L3649
	mov	x0, x19
	bl	HEVC_DecVDM
	cbnz	w0, .L3650
	add	x1, x19, 53248
	mov	w0, 1
	str	w0, [x19, 228]
	strb	w0, [x1, 1104]
	b	.L3650
	.p2align 3
.L3649:
	ldr	w2, [x19, 172]
	add	x4, x19, 53248
	ldr	w3, [x19, 208]
	ldr	w1, [x19, 248]
	sub	w2, w2, #1
	str	w0, [x19, 224]
	sub	w3, w3, #1
	str	w2, [x19, 172]
	sub	w1, w1, #1
	str	w3, [x19, 208]
	str	w1, [x19, 248]
	ldr	w5, [x19, 232]
	strb	w5, [x4, 1105]
	strb	w0, [x22, 817]
	strb	w0, [x19, 5]
	mov	w0, 0
	str	xzr, [x19, 2384]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	HEVCDEC_VDMProcSedOnly, .-HEVCDEC_VDMProcSedOnly
	.align	3
.LC1031:
	.xword	s_pstVfmwChan
	.align	3
.LC1032:
	.xword	g_BackUp
	.align	3
.LC1034:
	.xword	.LC1033
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
	ldr	w0, [x20, 276]
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
	ldr	x3, .LC1035
	mov	x2, 3732
	str	wzr, [x22]
	mov	w1, 0
	add	x0, x20, 1792
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x1, [x19, 2392]
	mov	x0, x19
	ldr	w2, [x1, 16]
	str	w2, [x20, 1864]
	ldr	w1, [x1, 32]
	str	w1, [x20, 1860]
	bl	HEVC_DecSliceSegmentHeader
	cbnz	w0, .L3677
	ldr	w0, [x20, 3380]
	mov	w4, 2024
	mov	w3, 26164
	cmp	w0, 1
	cset	w0, eq
	str	w0, [x22]
	ldr	w1, [x20, 1868]
	ldr	x0, [x19, 2368]
	ldr	w22, [x19, 220]
	ldr	x2, [x19, 2360]
	smaddl	x0, w1, w4, x0
	cmp	w22, 1
	ldr	w0, [x0, 32]
	smaddl	x20, w0, w3, x2
	beq	.L3678
	ldr	x0, [x19, 28168]
	ldr	x1, [x19, 2376]
	ldr	w2, [x20, 236]
	ldp	w5, w3, [x0, 72]
	ldr	w0, [x0, 236]
	ldp	w6, w4, [x20, 72]
	cmp	w2, w0
	ldr	w0, [x1, 872]
	cset	w2, ne
	cmp	w0, 1
	beq	.L3679
.L3666:
	cmp	w6, w5
	cset	w0, ne
	cmp	w4, w3
	orr	w0, w2, w0
	cset	w1, ne
	orr	w0, w1, w0
	cbz	w0, .L3669
	mov	w1, 1
	mov	w0, 0
	str	w1, [x21]
.L3662:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3678:
	ldr	x0, [x19, 2344]
	bl	BsResidBits
	cmp	w0, wzr
	ldp	w2, w1, [x20, 240]
	ldr	w3, [x19, 208]
	mul	w1, w1, w2
	add	w3, w3, 1
	udiv	w3, w1, w3
	ccmp	w0, w3, 1, ge
	blt	.L3664
.L3669:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3677:
	ldr	x1, .LC1037
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC1038
	ldr	x4, [x0]
	cbz	x4, .L3670
	add	x2, x29, 64
	ldr	w0, [x19, 276]
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
.L3679:
	ldr	w7, [x20, 108]
	ldr	w0, [x20, 100]
	cmp	w0, w7
	csel	w0, w0, w7, cs
	str	w0, [x1, 924]
	ldr	x0, [x19, 28168]
	ldr	x7, [x19, 2376]
	ldr	w1, [x0, 108]
	ldr	w0, [x0, 100]
	cmp	w0, w1
	csel	w0, w0, w1, cs
	ldr	w1, [x7, 924]
	cmp	w0, 8
	ccmp	w1, 8, 0, hi
	bne	.L3680
.L3667:
	mov	w0, 1
	str	w0, [x7, 928]
	str	w0, [x21]
	b	.L3666
	.p2align 3
.L3670:
	mov	w0, -1
	b	.L3662
	.p2align 3
.L3680:
	cmp	w0, 8
	ccmp	w1, 8, 4, eq
	ble	.L3666
	b	.L3667
.L3664:
	ldr	x1, .LC1040
	mov	w2, w0
	mov	w0, w22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3662
	.size	HEVC_InquireSliceProperty, .-HEVC_InquireSliceProperty
	.align	3
.LC1035:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC1037:
	.xword	.LC1036
	.align	3
.LC1038:
	.xword	g_event_report
	.align	3
.LC1040:
	.xword	.LC1039
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HEVCDEC_DecodePacket
	.type	HEVCDEC_DecodePacket, %function
HEVCDEC_DecodePacket:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	str	wzr, [x29, 68]
	stp	wzr, wzr, [x29, 60]
	cbz	x20, .L3750
	ldrb	w0, [x19]
	cmp	w0, 1
	bne	.L3751
	ldr	x0, [x19, 2376]
	ldrb	w1, [x19, 5]
	ldr	w2, [x0, 8]
	cmp	w1, 1
	ubfiz	w2, w2, 16, 3
	str	w2, [x19, 264]
	ldr	w1, [x0]
	str	w1, [x19, 268]
	ldr	w1, [x0, 4]
	str	w1, [x19, 272]
	beq	.L3685
	ldr	w1, [x0, 872]
	add	x21, x19, 86016
	cmp	w1, 1
	beq	.L3752
.L3686:
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_ReceivePacket
	mov	w20, w0
	ldr	x0, [x19, 2392]
	str	x0, [x21, 928]
.L3687:
	strb	wzr, [x19, 5]
	cbz	w20, .L3688
.L3710:
	mov	w20, 0
.L3748:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3688:
	mov	x0, x19
	bl	HEVCDEC_CheckSedOnly
	mov	w20, w0
	cbz	w0, .L3689
	ldr	x1, [x19, 2392]
	cbz	x1, .L3707
	ldr	w0, [x19, 276]
	mov	w20, -1
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3752:
	ldr	w0, [x0, 912]
	cmp	w0, 1
	bne	.L3686
	ldr	w0, [x21, 920]
	cmp	w0, 1
	bne	.L3686
	ldr	x0, [x21, 928]
	mov	w20, 0
	str	x0, [x19, 2392]
	str	wzr, [x21, 920]
	strb	wzr, [x19, 5]
	b	.L3748
	.p2align 3
.L3685:
	ldr	x0, [x19, 2392]
	cmp	x0, xzr
	csetm	w20, eq
	cbz	w20, .L3687
	ldr	x1, .LC1046
	mov	w0, 0
	bl	dprint_vfmw
	b	.L3748
	.p2align 3
.L3707:
	mov	w20, -1
	b	.L3748
	.p2align 3
.L3689:
	ldr	x0, [x19, 2392]
	str	wzr, [x0, 4]
	ldr	x1, [x19, 2392]
	ldr	x0, [x19, 2344]
	ldr	w2, [x1, 56]
	ldr	x1, [x1, 48]
	bl	BsInit
	ldr	x0, [x19, 2344]
	mov	w1, 24
	bl	BsSkip
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x21, 24]
	mov	w1, 6
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x21, 16]
	mov	w1, 6
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x21, 28]
	mov	w1, 3
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x21, 32]
	ldr	x21, [x19, 2392]
	ldr	w0, [x21, 32]
	cbz	w0, .L3691
	sub	w0, w0, #1
	str	w0, [x21, 32]
	ldr	x21, [x19, 2392]
.L3691:
	ldr	w0, [x21, 16]
	bl	HEVC_IsSliceUnit
	sxtb	w22, w0
	cmp	w22, 1
	beq	.L3753
.L3692:
	str	wzr, [x21, 4]
	add	x0, x19, 2672
	ldr	x1, [x19, 2392]
	ldr	w2, [x1, 56]
	ldr	x1, [x1, 48]
	bl	BsInit
	ldr	x0, [x19, 2344]
	mov	w1, 24
	bl	BsSkip
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	mov	w1, 1
	bl	BsGet
	str	w0, [x21, 24]
	mov	w1, 6
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x21, 16]
	mov	w1, 6
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	bl	BsGet
	str	w0, [x21, 28]
	mov	w1, 3
	ldr	x21, [x19, 2392]
	ldr	x0, [x19, 2344]
	bl	BsGet
	sub	w0, w0, #1
	str	w0, [x21, 32]
.L3708:
	mov	x0, x19
	bl	HEVC_DecOneNal
	cmn	w0, #2
	beq	.L3712
	cbnz	w0, .L3754
	ldr	w0, [x19, 212]
	cmp	w0, 1
	bne	.L3710
	mov	x0, x19
	bl	HEVCDEC_SetDecParam
	b	.L3748
	.p2align 3
.L3751:
	ldr	x1, .LC1044
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L3748
	.p2align 3
.L3753:
	add	x2, x29, 76
	add	x1, x29, 72
	mov	x0, x19
	bl	HEVC_InquireSliceProperty
	cbnz	w0, .L3693
	ldr	w21, [x29, 72]
	cmp	w21, 1
	beq	.L3755
.L3694:
	ldr	w21, [x29, 76]
	cmp	w21, 1
	beq	.L3756
.L3705:
	ldr	x21, [x19, 2392]
	ldr	w0, [x21, 16]
	bl	HEVC_IsSliceUnit
	sxtb	w0, w0
	cmp	w0, 1
	beq	.L3708
	b	.L3692
	.p2align 3
.L3693:
	ldr	x1, .LC1056
	mov	w0, w22
	bl	dprint_vfmw
	ldr	x1, [x19, 2392]
	cbz	x1, .L3706
	ldr	w0, [x19, 276]
	bl	HEVC_ReleaseNAL
	str	xzr, [x19, 2392]
.L3706:
	ldr	x0, .LC1054
	ldr	x4, [x0]
	cbz	x4, .L3707
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	mov	w20, -1
	blr	x4
	b	.L3748
.L3750:
	ldr	x1, .LC1042
	mov	w0, 0
	mov	w20, -1
	bl	dprint_vfmw
	b	.L3748
.L3756:
	add	x0, x19, 53248
	ldr	w0, [x0, 1864]
	sub	w0, w0, #16
	cmp	w0, 4
	bls	.L3757
.L3697:
	ldrb	w21, [x19, 2]
	cmp	w21, 1
	beq	.L3758
.L3700:
	ldr	w0, [x19, 276]
	bl	FSP_GetFspType
	cbnz	w0, .L3705
	ldr	w0, [x19, 276]
	add	x3, x29, 68
	add	x2, x29, 64
	add	x1, x29, 60
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 748]
	cmp	w0, 1
	bne	.L3705
	ldr	w1, [x29, 68]
	cmp	w1, wzr
	bgt	.L3703
	ldr	w2, [x29, 64]
	cmp	w2, 32
	bgt	.L3703
	ldr	x2, [x19, 2384]
	cbnz	x2, .L3703
	ldr	w2, [x29, 76]
	cbz	w2, .L3705
	ldr	w3, [x19, 252]
	cbz	w3, .L3705
	add	x21, x19, 86016
	ldr	x3, [x19, 28168]
	ldr	w4, [x21, 952]
	ldr	w6, [x3, 76]
	cmp	w6, w4
	beq	.L3705
	ldr	w5, [x3, 72]
	ldr	w3, [x21, 956]
	cmp	w5, w3
	ccmp	w1, wzr, 0, ne
	bne	.L3705
	ldr	x1, .LC1053
	bl	dprint_vfmw
	str	wzr, [x21, 948]
	ldr	x1, .LC1054
	ldr	x0, [x19, 28168]
	ldr	x4, [x1]
	ldr	w1, [x0, 76]
	str	w1, [x21, 952]
	ldr	w0, [x0, 72]
	str	w0, [x21, 956]
	cbz	x4, .L3705
	ldr	w0, [x19, 276]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L3705
	.p2align 3
.L3755:
	ldr	w1, [x19, 208]
	cbnz	w1, .L3695
	add	x1, x19, 53248
	ldrb	w1, [x1, 1105]
	cbz	w1, .L3694
	ldr	x2, .LC1047
	ldr	x1, .LC1049
	add	x2, x2, 440
	bl	dprint_vfmw
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	b	.L3694
.L3754:
	ldr	x1, .LC1058
	mov	w0, 22
	mov	w20, -1
	bl	dprint_vfmw
	b	.L3748
.L3712:
	mov	w20, w0
	b	.L3748
.L3695:
	ldr	x3, [x19, 2392]
	mov	w2, 13
	ldr	x1, .LC1047
	add	x0, x19, 2672
	mov	w20, -2
	str	wzr, [x3, 4]
	add	x1, x1, 488
	bl	BsInit
	ldr	x0, [x19, 2344]
	mov	w1, 40
	bl	BsSkip
	ldr	x1, [x19, 2392]
	mov	w2, 48
	mov	x0, x19
	str	w2, [x1, 16]
	strb	w21, [x19, 6]
	bl	HEVC_DecOneNal
	strb	w21, [x19, 5]
	ldr	w0, [x19, 304]
	add	w0, w0, 1
	str	w0, [x19, 304]
	b	.L3748
.L3757:
	mov	x0, x19
	bl	HEVC_FlushOutputFrmFromDPB
	cbz	w0, .L3697
	ldr	x2, .LC1047
	mov	w0, w21
	ldr	x1, .LC1050
	add	x2, x2, 464
	bl	dprint_vfmw
	b	.L3697
.L3758:
	mov	x0, x19
	bl	HEVC_EmptyFrmFromDPB
	cbz	w0, .L3700
	ldr	x2, .LC1047
	mov	w0, w21
	ldr	x1, .LC1051
	add	x2, x2, 464
	bl	dprint_vfmw
	b	.L3700
.L3703:
	add	x1, x19, 86016
	mov	w0, 1
	mov	w20, -2
	str	wzr, [x1, 948]
	strb	w0, [x19, 5]
	b	.L3748
	.size	HEVCDEC_DecodePacket, .-HEVCDEC_DecodePacket
	.align	3
.LC1042:
	.xword	.LC1041
	.align	3
.LC1044:
	.xword	.LC1043
	.align	3
.LC1046:
	.xword	.LC1045
	.align	3
.LC1047:
	.xword	.LANCHOR5
	.align	3
.LC1049:
	.xword	.LC1048
	.align	3
.LC1050:
	.xword	.LC980
	.align	3
.LC1051:
	.xword	.LC982
	.align	3
.LC1053:
	.xword	.LC1052
	.align	3
.LC1054:
	.xword	g_event_report
	.align	3
.LC1056:
	.xword	.LC1055
	.align	3
.LC1058:
	.xword	.LC1057
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
	cbnz	w0, .L3760
	add	x0, x19, 53248
	mov	w20, -1
	ldrb	w0, [x0, 1105]
	cbnz	w0, .L3764
.L3760:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3764:
	ldr	x2, .LC1059
	mov	w0, 0
	ldr	x1, .LC1060
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
.LC1059:
	.xword	.LANCHOR5
	.align	3
.LC1060:
	.xword	.LC1048
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
	ldr	x2, [x19, 152]
	cbz	x2, .L3766
	.p2align 2
.L3767:
	ldp	w3, w1, [x2]
	ldr	x2, [x2, 1360]
	add	w1, w3, w1
	add	w1, w1, 7
	add	w0, w0, w1, lsr 3
	cbnz	x2, .L3767
.L3766:
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
	ldr	w0, [x19, 2716]
	cbz	w0, .L3782
	ldr	x23, .LC1061
	add	x22, x19, 61440
	ldr	x24, .LC1065
	mov	w21, -1
	add	x23, x23, 504
	ldr	x26, .LC1063
	add	x22, x22, 1064
	mov	x25, x23
	b	.L3781
	.p2align 3
.L3773:
	mov	x0, x19
	bl	HEVC_RemoveApcStore
	ldr	w1, [x29, 104]
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
	ldr	w0, [x19, 2716]
	cbz	w0, .L3791
.L3781:
	add	x1, x29, 108
	mov	x0, x19
	add	x2, x29, 104
	bl	HEVC_GetMinPOC
	ldr	w1, [x29, 104]
	add	x0, x19, x1, uxtw 3
	ldr	x20, [x0, 2736]
	cbz	x20, .L3773
	ldrb	w0, [x20, 2]
	cbnz	w0, .L3773
	mov	w0, 1
	mov	x1, x20
	strb	w0, [x20, 2]
	mov	x0, x19
	bl	HEVC_GetImagePara
	mov	x1, x20
	mov	x0, x19
	bl	HEVC_CheckFrameStore
	mov	w21, w0
	cbz	w0, .L3792
	ldr	w1, [x29, 104]
.L3796:
	mov	x0, x19
	bl	HEVC_RemoveApcStore
	ldr	w1, [x29, 104]
	mov	x0, x19
	bl	HEVC_RemoveFrameStoreOutDPB
	ldr	w0, [x19, 2716]
	cbnz	w0, .L3781
	.p2align 2
.L3791:
	cbnz	w21, .L3782
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3792:
	ldr	w0, [x19, 276]
	mov	w2, 1
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 276]
	ldr	w1, [x20, 32]
	bl	FSP_GetFsImagePtr
	mov	x27, x0
	cbz	x0, .L3793
	ldr	w0, [x19, 2716]
	cmp	w0, 1
	beq	.L3794
.L3776:
	ldr	w0, [x19, 276]
	mov	x4, x27
	mov	x3, x22
	mov	x2, x19
	mov	w1, 16
	bl	InsertImgToVoQueue
	cmp	w0, 1
	mov	w28, w0
	beq	.L3777
	mov	w3, w0
	mov	x2, x23
	mov	x1, x24
	mov	w0, 0
	mov	w28, -1
	bl	dprint_vfmw
	ldr	w0, [x19, 276]
	mov	w2, 0
	ldr	w1, [x20, 32]
	bl	FSP_SetDisplay
	mov	w1, 1
	mov	x0, x19
	bl	HEVC_ClearAll
.L3777:
	ldr	x0, [x19, 2376]
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L3795
	str	wzr, [x27, 24]
.L3779:
	ldr	w0, [x19, 316]
	ldr	w1, [x29, 104]
	add	w0, w0, 1
	str	w0, [x19, 316]
	ldr	w0, [x19, 308]
	add	w0, w0, 1
	str	w0, [x19, 308]
	ldr	w0, [x19, 2716]
	cmp	w0, 1
	beq	.L3773
	mov	w21, w28
	b	.L3773
	.p2align 3
.L3782:
	add	x0, x19, 61440
	add	x0, x0, 1064
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
.L3795:
	mov	x1, x27
	mov	x0, x19
	bl	HEVC_SetFrmRepeatCount.part.4
	b	.L3779
	.p2align 3
.L3794:
	str	w0, [x27, 188]
	b	.L3776
.L3793:
	mov	x1, x26
	mov	x2, x25
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	ldr	w1, [x29, 104]
	b	.L3796
	.size	HEVCDEC_GetRemainImg, .-HEVCDEC_GetRemainImg
	.align	3
.LC1061:
	.xword	.LANCHOR5
	.align	3
.LC1063:
	.xword	.LC1062
	.align	3
.LC1065:
	.xword	.LC1064
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
	ldr	x0, [x19, 28168]
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
	ldr	x0, [x19, 28168]
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
	.type	SarTable.11646, %object
	.size	SarTable.11646, 136
SarTable.11646:
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
	.type	__func__.11815, %object
	.size	__func__.11815, 18
__func__.11815:
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
	.type	__func__.12267, %object
	.size	__func__.12267, 27
__func__.12267:
	.string	"HEVC_GetUnRefPicWithMinPoc"
	.zero	5
	.type	__func__.12276, %object
	.size	__func__.12276, 21
__func__.12276:
	.string	"HEVC_IncreaseDPBSize"
	.zero	3
	.type	__func__.12507, %object
	.size	__func__.12507, 17
__func__.12507:
	.string	"HEVC_WritePicMsg"
	.zero	7
	.type	__func__.12536, %object
	.size	__func__.12536, 16
__func__.12536:
	.string	"HEVC_CheckListX"
	.type	__func__.12660, %object
	.size	__func__.12660, 16
__func__.12660:
	.string	"HEVC_RepairList"
	.type	__func__.13033, %object
	.size	__func__.13033, 21
__func__.13033:
	.string	"HEVC_CheckFrameStore"
	.zero	3
	.type	__func__.12876, %object
	.size	__func__.12876, 19
__func__.12876:
	.string	"HEVC_OutputCurrPic"
	.zero	5
	.type	__func__.12881, %object
	.size	__func__.12881, 23
__func__.12881:
	.string	"HEVC_FirstFrameFastOut"
	.zero	1
	.type	__func__.13179, %object
	.size	__func__.13179, 14
__func__.13179:
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
	.type	__func__.13460, %object
	.size	__func__.13460, 16
__func__.13460:
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
	.type	__func__.12358, %object
	.size	__func__.12358, 27
__func__.12358:
	.string	"HEVC_DecSliceSegmentHeader"
	.zero	5
	.type	__func__.12739, %object
	.size	__func__.12739, 13
__func__.12739:
	.string	"HEVC_InitDPB"
	.zero	3
	.type	__func__.13520, %object
	.size	__func__.13520, 13
__func__.13520:
	.string	"HEVCDEC_Init"
	.zero	3
	.type	__func__.13025, %object
	.size	__func__.13025, 19
__func__.13025:
	.string	"HEVC_OutputFrmToVO"
	.zero	5
	.type	__func__.12992, %object
	.size	__func__.12992, 22
__func__.12992:
	.string	"HEVC_OutputFrmFromDPB"
	.zero	2
	.type	__func__.12282, %object
	.size	__func__.12282, 30
__func__.12282:
	.string	"HEVC_OutputUnRefPicWithMinPoc"
	.zero	2
	.type	__func__.13005, %object
	.size	__func__.13005, 27
__func__.13005:
	.string	"HEVC_FlushOutputFrmFromDPB"
	.zero	5
	.type	__func__.13508, %object
	.size	__func__.13508, 23
__func__.13508:
	.string	"HEVCDEC_GetImageBuffer"
	.zero	1
	.type	__func__.13559, %object
	.size	__func__.13559, 14
__func__.13559:
	.string	"HEVC_ClearDPB"
	.zero	2
	.type	__func__.12287, %object
	.size	__func__.12287, 23
__func__.12287:
	.string	"HEVC_RecoverDPBProcess"
	.zero	1
	.type	__func__.12969, %object
	.size	__func__.12969, 20
__func__.12969:
	.string	"HEVC_InsertFrmInDPB"
	.zero	4
	.type	__func__.12886, %object
	.size	__func__.12886, 22
__func__.12886:
	.string	"HEVC_SimpleDPBProcess"
	.zero	2
	.type	__func__.12891, %object
	.size	__func__.12891, 21
__func__.12891:
	.string	"HEVC_DecOrderProcess"
	.zero	3
	.type	__func__.12896, %object
	.size	__func__.12896, 22
__func__.12896:
	.string	"HEVC_DispOrderProcess"
	.zero	2
	.type	__func__.12912, %object
	.size	__func__.12912, 19
__func__.12912:
	.string	"HEVC_StorePicInDPB"
	.zero	5
	.type	__func__.12314, %object
	.size	__func__.12314, 14
__func__.12314:
	.string	"HEVC_DecSlice"
	.zero	2
	.type	__func__.12613, %object
	.size	__func__.12613, 12
__func__.12613:
	.string	"HEVC_DecVDM"
	.zero	4
	.type	__func__.13623, %object
	.size	__func__.13623, 20
__func__.13623:
	.string	"HEVC_HaveSliceToDec"
	.zero	4
	.type	__func__.11762, %object
	.size	__func__.11762, 21
__func__.11762:
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
	.type	__func__.13640, %object
	.size	__func__.13640, 21
__func__.13640:
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
.LC1033:
	ASCII(.string	"CtbNumOneFrame=%d, SedEnd0=%d\n" )
	.zero	1
.LC1036:
	ASCII(.string	"HEVC_DecSliceSegmentHeader dec err\n" )
	.zero	4
.LC1039:
	ASCII(.string	"CurBitsLen = %d, MinBitsLen = %d\n" )
	.zero	6
.LC1041:
	ASCII(.string	"HEVCDEC_DecodePacket NULL == pPacket!\n" )
	.zero	1
.LC1043:
	ASCII(.string	"HevcCtx not init!\n" )
	.zero	5
.LC1045:
	ASCII(.string	"HEVCDEC_DecodePacket NULL == pHevcCtx->pCurrNal!\n" )
	.zero	6
.LC1048:
	ASCII(.string	"%s last frame fs unrelease, fix it!\n" )
	.zero	3
.LC1052:
	ASCII(.string	"IsSizeChanged = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	4
.LC1055:
	ASCII(.string	"HEVC_InquireSliceProperty error.\n" )
	.zero	6
.LC1057:
	ASCII(.string	"HEVC_DecOneNal ERR\n" )
	.zero	4
.LC1062:
	ASCII(.string	"%s pstLogicFsImage is NULL!\n" )
	.zero	3
.LC1064:
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
