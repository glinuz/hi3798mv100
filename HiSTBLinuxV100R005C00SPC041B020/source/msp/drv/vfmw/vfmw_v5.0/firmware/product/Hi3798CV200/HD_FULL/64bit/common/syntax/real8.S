#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"real8.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	Real8_CB_GetBits
	.type	Real8_CB_GetBits, %function
Real8_CB_GetBits:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	mov	w1, w20
	mov	x0, x19
	bl	BsShow
	mov	w21, w0
	mov	w1, w20
	mov	x0, x19
	bl	BsSkip
	mov	w0, w21
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	Real8_CB_GetBits, .-Real8_CB_GetBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_Get1Bit
	.type	Real8_CB_Get1Bit, %function
Real8_CB_Get1Bit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real8_CB_Get1Bit, .-Real8_CB_Get1Bit
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_GetVLCBits
	.type	Real8_CB_GetVLCBits, %function
Real8_CB_GetVLCBits:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x23, x1
	mov	w20, 0
	bl	_mcount
	ldr	x22, .LC3
	mov	w19, 0
	.p2align 2
.L4:
	mov	w1, 8
	mov	x0, x21
	bl	BsShow
	ldrb	w2, [x22, w0, uxtw]
	mov	x0, x21
	lsr	w1, w2, 4
	lsr	w3, w2, 5
	add	w20, w20, w1
	and	w2, w2, 15
	lsl	w19, w19, w3
	orr	w19, w2, w19
	bl	BsSkip
	tbz	x20, 0, .L4
	str	w19, [x23]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	Real8_CB_GetVLCBits, .-Real8_CB_GetVLCBits
	.align	3
.LC3:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_GetImageBuffer
	.type	Real8_GetImageBuffer, %function
Real8_GetImageBuffer:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 53248
	add	x19, x19, 8192
	bl	_mcount
	add	x19, x19, 3192
	add	x20, x19, 40960
	mov	w1, 1
	ldr	w0, [x21, 1304]
	bl	FSP_NewLogicFs
	str	w0, [x20, 1544]
	mov	w1, w0
	tbnz	w0, #31, .L15
	ldr	w0, [x21, 1304]
	bl	FSP_GetLogicFs
	mov	x19, x0
	cbz	x0, .L16
	ldr	x1, .LC9
	mov	w0, 18
	ldr	w2, [x20, 1544]
	bl	dprint_vfmw
	ldr	x0, [x19, 640]
	cbz	x0, .L14
	ldr	x1, [x19, 648]
	cbz	x1, .L14
	ldr	x2, [x19, 656]
	cbz	x2, .L14
	ldr	w3, [x1, 16]
	ldr	w4, [x2, 16]
	ldr	x1, .LC11
	ldr	w2, [x0, 16]
	mov	w0, 18
	bl	dprint_vfmw
	mov	w0, 1
.L10:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L14:
	mov	w0, 1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L15:
	ldr	x1, .LC5
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 1540]
	mov	w2, 1
	ldr	w0, [x21, 1304]
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 1536]
	mov	w2, 1
	ldr	w0, [x21, 1304]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 1536]
	str	wzr, [x20, 1540]
	ldr	w0, [x21, 1304]
	bl	FSP_GetNullLogicFs
	str	w0, [x20, 1536]
	str	w0, [x20, 1540]
	add	x1, x21, 696
	strb	wzr, [x19, 23]
	ldr	w0, [x21, 1304]
	bl	FSP_ClearNotInVoQueue
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L16:
	ldr	x1, .LC7
	mov	w2, 517
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L10
	.size	Real8_GetImageBuffer, .-Real8_GetImageBuffer
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC7:
	.xword	.LC6
	.align	3
.LC9:
	.xword	.LC8
	.align	3
.LC11:
	.xword	.LC10
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_ArrangeVHBMem
	.type	Real8_ArrangeVHBMem, %function
Real8_ArrangeVHBMem:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x2
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldp	w3, w2, [x19, 104]
	cmp	w2, 1
	bls	.L19
	cmp	w3, 1
	bls	.L19
	ldr	x1, [x20]
	ldr	w0, [x1, 708]
	cbnz	w0, .L22
	ldr	w0, [x1, 28]
	cmp	w0, 25
	beq	.L47
.L23:
	ldp	w22, w25, [x19, 96]
	mov	w24, 5
	mov	w21, 2
	mov	w23, 4
.L28:
	ldr	x0, .LC16
	mov	w1, 0
	mov	x2, 32
	ldr	x3, [x0, 96]
	add	x0, x29, 96
	blr	x3
	strb	wzr, [x29, 97]
	ldr	w1, [x19, 96]
	ldr	x0, [x20]
	str	w1, [x29, 124]
	ldr	w1, [x19, 100]
	ldr	w0, [x0, 28]
	stp	w25, w22, [x29, 104]
	cmp	w0, 24
	str	w1, [x29, 120]
	beq	.L34
	strb	w23, [x29, 99]
	sxtb	w0, w21
	strb	w24, [x29, 101]
.L38:
	add	x19, x20, 53248
	strb	w0, [x29, 102]
	add	x1, x29, 96
	strb	wzr, [x29, 98]
	ldr	w0, [x19, 1304]
	bl	FSP_ConfigInstance
	mov	w1, 0
	cbz	w0, .L48
.L43:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	mov	w0, w1
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L22:
	ldr	w0, [x1, 28]
	cmp	w0, 25
	bne	.L23
	ldr	w4, [x1, 668]
	ldp	w3, w2, [x19, 96]
	ldr	w5, [x1, 672]
	cmp	w2, w4
	bhi	.L31
	cmp	w3, w5
	bhi	.L31
	ldr	w5, [x1, 696]
	mov	w25, w2
	ldr	w0, [x1, 692]
	mov	w22, w3
	mov	w21, 1
	add	w23, w0, 2
	cbnz	w5, .L46
.L33:
	ldrsb	w24, [x1, 716]
	b	.L28
	.p2align 3
.L19:
	ldr	x1, .LC13
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	mov	w1, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, w1
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L48:
	ldr	x1, [x20]
	add	x3, x29, 92
	ldr	w0, [x19, 1304]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L49
	cbnz	w0, .L36
	add	x20, x20, 8192
	mov	w0, 1
	mov	w1, 1
	strb	w0, [x20, 3212]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L47:
	ldr	w25, [x1, 668]
	ldp	w3, w2, [x19, 96]
	cmp	w2, w25
	bhi	.L50
	ldr	w22, [x1, 672]
	cmp	w3, w22
	bhi	.L51
	add	x0, x20, 8192
	ldrb	w0, [x0, 3212]
	cmp	w0, 1
	beq	.L39
	ldr	w2, [x1, 696]
	mov	w21, 1
	ldr	w0, [x1, 692]
	add	w23, w0, 2
	cbz	w2, .L33
.L46:
	add	w0, w0, w21
	mov	w21, 17
	cmp	w0, w21
	ldrsb	w24, [x1, 716]
	csel	w21, w0, w21, le
	b	.L28
	.p2align 3
.L34:
	mov	w0, 1
	strb	w24, [x29, 101]
	strb	w0, [x29, 99]
	mov	w0, 1
	b	.L38
	.p2align 3
.L31:
	ldr	x1, .LC15
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L43
	.p2align 3
.L49:
	add	x0, x19, 696
	bl	ResetVoQueue
.L36:
	ldr	x1, .LC18
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L43
.L39:
	mov	w1, w0
	b	.L43
	.p2align 3
.L51:
	mov	w5, w22
.L25:
	ldr	x1, .LC15
	mov	w4, w25
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L43
	.p2align 3
.L50:
	ldr	w5, [x1, 672]
	b	.L25
	.size	Real8_ArrangeVHBMem, .-Real8_ArrangeVHBMem
	.align	3
.LC13:
	.xword	.LC12
	.align	3
.LC15:
	.xword	.LC14
	.align	3
.LC16:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC18:
	.xword	.LC17
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_GetCPFMT
	.type	Real8_CB_GetCPFMT, %function
Real8_CB_GetCPFMT:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	mov	w1, 4
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 4
	mov	x0, x19
	bl	BsSkip
	strb	w20, [x21]
	mov	w1, 9
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 9
	mov	x0, x19
	add	w20, w20, 1
	bl	BsSkip
	lsl	w20, w20, 2
	mov	w1, 1
	str	w20, [x21, 100]
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	cmp	w20, 1
	beq	.L55
	mov	w20, 0
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L55:
	mov	w1, 9
	mov	x0, x19
	bl	BsShow
	mov	w22, w0
	mov	w1, 9
	mov	x0, x19
	lsl	w22, w22, 2
	bl	BsSkip
	str	w22, [x21, 96]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Real8_CB_GetCPFMT, .-Real8_CB_GetCPFMT
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_SetDimensions
	.type	Real8_CB_SetDimensions, %function
Real8_CB_SetDimensions:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x1, 96
	ldr	x0, [x19, 96]
	movk	x1, 0x80, lsl 32
	cmp	x0, x1
	beq	.L72
	mov	x1, 144
	movk	x1, 0xb0, lsl 32
	cmp	x0, x1
	beq	.L73
	mov	x1, 288
	movk	x1, 0x160, lsl 32
	cmp	x0, x1
	beq	.L74
	mov	x1, 576
	movk	x1, 0x2c0, lsl 32
	cmp	x0, x1
	beq	.L75
	mov	x1, 1152
	movk	x1, 0x580, lsl 32
	cmp	x0, x1
	beq	.L76
	ldr	w0, [x19, 100]
	add	x1, x19, 8
	mov	w2, 6
	add	w0, w0, 15
	lsr	w0, w0, 4
	str	w0, [x1, 28]
	ldr	w3, [x19, 96]
	add	w3, w3, 15
	lsr	w3, w3, 4
	str	w3, [x1, 24]
	mul	w0, w0, w3
	sub	w0, w0, #1
	cmp	w0, 47
	bls	.L58
	cmp	w0, 98
	bls	.L67
	cmp	w0, 395
	bls	.L68
	cmp	w0, 1583
	bls	.L69
.L71:
	mov	w3, 6335
	mov	w2, 14
	cmp	w0, w3
	bhi	.L58
	mov	w2, 4
	b	.L60
	.p2align 3
.L72:
	add	x1, x19, 8
	mov	w0, 6
	mov	w3, 8
	mov	w2, w0
	stp	w0, w3, [x1, 24]
.L58:
	str	w2, [x1, 32]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L73:
	add	x1, x19, 8
	mov	w3, 11
	mov	w0, 9
	mov	w2, 1
	stp	w0, w3, [x1, 24]
.L60:
	ldr	x0, .LC19
	add	x0, x0, 256
	ldr	w2, [x0, w2, uxtw 2]
	str	w2, [x1, 32]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L74:
	add	x1, x19, 8
	mov	w2, 22
	mov	w0, 18
	stp	w0, w2, [x1, 24]
.L68:
	mov	w2, 2
	b	.L60
	.p2align 3
.L76:
	add	x1, x19, 8
	mov	w3, 88
	mov	w2, 72
	mov	w0, 6335
	stp	w2, w3, [x1, 24]
	b	.L71
	.p2align 3
.L75:
	add	x1, x19, 8
	mov	w2, 44
	mov	w0, 36
	stp	w0, w2, [x1, 24]
.L69:
	mov	w2, 3
	b	.L60
.L67:
	mov	w2, 1
	b	.L60
	.size	Real8_CB_SetDimensions, .-Real8_CB_SetDimensions
	.align	3
.LC19:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_GetPicSize
	.type	Real8_CB_GetPicSize, %function
Real8_CB_GetPicSize:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	x24, x2
	mov	x23, x3
	bl	_mcount
	ldr	x0, .LC21
	mov	w3, 180
	ldr	w1, [x20, 36]
	add	x2, x0, 280
	add	x0, x0, 312
	sub	w1, w1, #2
	str	w3, [x29, 64]
	cmp	w1, 1
	ldp	x4, x5, [x2]
	stp	x4, x5, [x29, 80]
	ldp	x4, x5, [x0]
	ldp	x2, x3, [x2, 16]
	stp	x4, x5, [x29, 112]
	ldp	x4, x5, [x0, 16]
	stp	x2, x3, [x29, 96]
	mov	w0, 576
	mov	w2, 360
	stp	x4, x5, [x29, 128]
	stp	w0, wzr, [x29, 72]
	str	w2, [x29, 68]
	bls	.L91
.L78:
	mov	w1, 3
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 3
	mov	x0, x19
	bl	BsSkip
	add	x0, x29, 144
	add	x20, x0, x20, uxtw 2
	ldr	w21, [x20, -64]
	cbz	w21, .L81
.L80:
	mov	w1, 3
	mov	x0, x19
	bl	BsShow
	mov	w22, w0
	mov	w1, 3
	mov	x0, x19
	bl	BsSkip
	add	x0, x29, 144
	add	x0, x0, x22, uxtw 2
	ldr	w20, [x0, -32]
	cbz	w20, .L92
.L82:
	str	w21, [x24]
	str	w20, [x23]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L81:
	mov	w1, 8
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 8
	mov	x0, x19
	add	w21, w21, w20, lsl 2
	bl	BsSkip
	cmp	w20, 255
	beq	.L81
	b	.L80
	.p2align 3
.L91:
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	mov	w21, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	cbz	w21, .L78
	ldr	w0, [x20, 68]
	str	w0, [x24]
	ldr	w0, [x20, 72]
	str	w0, [x23]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L92:
	mov	w1, 1
	mov	x0, x19
	lsl	w22, w22, 1
	bl	BsShow
	mov	w20, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	orr	w0, w20, w22
	add	x1, x29, 144
	ubfiz	x0, x0, 2, 2
	add	x0, x1, x0
	ldr	w20, [x0, -80]
	cbnz	w20, .L82
	.p2align 2
.L83:
	mov	w1, 8
	mov	x0, x19
	bl	BsShow
	mov	w22, w0
	mov	w1, 8
	mov	x0, x19
	add	w20, w20, w22, lsl 2
	bl	BsSkip
	cmp	w22, 255
	beq	.L83
	b	.L82
	.size	Real8_CB_GetPicSize, .-Real8_CB_GetPicSize
	.align	3
.LC21:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_FindNextSliceStartCode
	.type	Real8_CB_FindNextSliceStartCode, %function
Real8_CB_FindNextSliceStartCode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, 0
	bl	_mcount
	mov	x0, x20
	bl	BsToNextByte
	mov	x0, x20
	bl	BsResidBits
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w21, w1, w0, lt
	asr	w21, w21, 3
	sub	w21, w21, #2
	cmp	w21, wzr
	bgt	.L100
	b	.L97
	.p2align 3
.L103:
	bl	BsSkip
	cmp	w19, w21
	beq	.L97
.L100:
	mov	w1, 17
	mov	x0, x20
	add	w19, w19, 1
	bl	BsShow
	mov	w2, w0
	cmp	w2, 1
	mov	w1, 8
	mov	x0, x20
	bne	.L103
	mov	w0, w2
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L97:
	mov	w2, 0
	ldr	x21, [sp, 32]
	mov	w0, w2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	Real8_CB_FindNextSliceStartCode, .-Real8_CB_FindNextSliceStartCode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_SearchBits_IgnoreOnly0
	.type	Real8_CB_SearchBits_IgnoreOnly0, %function
Real8_CB_SearchBits_IgnoreOnly0:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	mov	w25, w1
	mov	w24, w3
	mov	w23, w2
	bl	_mcount
	sub	w2, w25, #1
	mov	w1, w25
	mov	x0, x21
	mov	w22, 1
	lsl	w22, w22, w2
	bl	BsShow
	mov	w19, w0
	mov	w1, w25
	mov	x0, x21
	bl	BsSkip
	cmp	w23, w19
	ccmp	w24, wzr, 4, ne
	beq	.L105
	tst	w22, w19
	bne	.L105
	ldr	x0, .LC24
	mov	w20, 0
	add	x0, x0, 344
	ldr	w25, [x0, w25, uxtw 2]
	b	.L106
	.p2align 3
.L107:
	tst	w22, w19
	bne	.L105
.L106:
	mov	w1, 1
	mov	x0, x21
	add	w20, w20, w1
	and	w19, w25, w19, lsl 1
	bl	BsShow
	mov	w2, w0
	mov	w1, 1
	mov	x0, x21
	orr	w19, w2, w19
	bl	BsSkip
	cmp	w23, w19
	ccmp	w24, w20, 0, ne
	bhi	.L107
.L105:
	cmp	w23, w19
	ldr	x25, [sp, 64]
	cset	w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.size	Real8_CB_SearchBits_IgnoreOnly0, .-Real8_CB_SearchBits_IgnoreOnly0
	.align	3
.LC24:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_SetRPRSizes
	.type	Real8_CB_SetRPRSizes, %function
Real8_CB_SetRPRSizes:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	x21, x2
	bl	_mcount
	ldr	x0, .LC26
	add	x6, x29, 56
	add	x1, x29, 96
	str	x21, [x19, 16]
	add	x0, x0, 448
	add	x1, x1, x20, uxtw 2
	str	w20, [x19, 24]
	ldp	x4, x5, [x0]
	stp	x4, x5, [x6]
	add	x4, x29, 72
	ldp	x2, x3, [x0, 16]
	ldr	w0, [x0, 32]
	stp	x2, x3, [x4]
	str	w0, [x29, 88]
	ldr	w0, [x1, -40]
	str	w0, [x19, 12]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.size	Real8_CB_SetRPRSizes, .-Real8_CB_SetRPRSizes
	.align	3
.LC26:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_GetSSC
	.type	Real8_CB_GetSSC, %function
Real8_CB_GetSSC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 88]
	cmp	w0, 1
	beq	.L114
	mov	x0, x20
	bl	Real8_CB_FindNextSliceStartCode
	cmp	w0, wzr
	cset	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L114:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real8_CB_GetSSC, .-Real8_CB_GetSSC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_GetSliceHeader
	.type	Real8_CB_GetSliceHeader, %function
Real8_CB_GetSliceHeader:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x19, x5
	mov	x20, x0
	mov	x0, x30
	mov	w21, w1
	mov	x22, x2
	mov	x24, x3
	mov	x23, x4
	bl	_mcount
	ldr	w0, [x19, 88]
	cmp	w0, 1
	beq	.L150
	cbz	w21, .L151
	str	wzr, [x22]
	mov	w0, -1
	mov	w19, 1
	str	w0, [x23]
.L141:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L150:
	cbnz	w21, .L152
	add	x21, x19, 8
	mov	w1, 3
	mov	x0, x20
	ldr	w26, [x21, 40]
	bl	BsShow
	mov	w25, w0
	mov	w1, 3
	mov	x0, x20
	bl	BsSkip
	cmp	w25, 7
	bhi	.L121
	ldr	x0, .LC28
	add	x0, x0, 488
	ldr	w0, [x0, w25, uxtw 2]
	cmp	w0, 2
	bhi	.L121
	cmn	w26, #1
	mov	w1, 2
	mov	x0, x20
	beq	.L153
	bl	BsShow
	mov	w25, w0
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
	ldr	w0, [x21, 36]
	cmp	w0, w25
	bne	.L121
.L123:
	mov	w1, 1
	mov	x0, x20
	bl	BsShow
	mov	w25, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	cbnz	w25, .L121
	mov	w1, 5
	mov	x0, x20
	bl	BsShow
	mov	w25, w0
	mov	w1, 5
	mov	x0, x20
	bl	BsSkip
	uxtb	w0, w25
	strb	w0, [x21, 2]
	cmp	w0, 31
	bhi	.L121
	mov	w1, 1
	mov	x0, x20
	bl	BsShow
	mov	w25, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	cmp	w25, wzr
	mov	w1, 13
	cset	w0, ne
	cmn	w26, #1
	strb	w0, [x21, 1]
	mov	x0, x20
	bne	.L124
	bl	BsShow
	mov	w26, w0
	mov	w1, 13
	mov	x0, x20
	bl	BsSkip
	ldr	w25, [x21, 4]
	str	w26, [x21, 44]
	cbz	w25, .L142
	mov	w1, w25
	mov	x0, x20
	bl	BsShow
	mov	w26, w0
	mov	w1, w25
	mov	x0, x20
	bl	BsSkip
	str	w26, [x21, 56]
	ldr	x1, [x21, 8]
	cbz	x1, .L121
	lsl	w26, w26, 1
	mov	x0, x19
	add	w3, w26, 1
	ldr	w2, [x1, w26, uxtw 2]
	ldr	w1, [x1, w3, uxtw 2]
	stp	w2, w1, [x21, 60]
	stp	w1, w2, [x19, 96]
	bl	Real8_CB_SetDimensions
.L142:
	ldr	w25, [x21, 32]
	mov	x0, x20
	mov	w1, w25
	bl	BsShow
	mov	w26, w0
	mov	w1, w25
	mov	x0, x20
	bl	BsSkip
	str	w26, [x21, 40]
	mov	w1, 1
	mov	x0, x20
	bl	BsShow
	mov	w25, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	strb	w25, [x19, 8]
	b	.L118
	.p2align 3
.L151:
	mov	w1, 1
	mov	x0, x20
	bl	BsShow
	mov	w21, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	cmp	w21, 1
	beq	.L154
.L121:
	mov	w19, 0
.L155:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L152:
	add	x21, x19, 8
.L118:
	ldr	w0, [x21, 40]
	mov	w19, 1
	str	w0, [x22]
	ldrb	w0, [x21, 2]
	str	w0, [x23]
	mov	w0, w19
	str	wzr, [x24]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L154:
	ldr	w25, [x19, 40]
	mov	x0, x20
	mov	w1, w25
	bl	BsShow
	mov	w26, w0
	mov	w1, w25
	mov	x0, x20
	bl	BsSkip
	str	w26, [x22]
	ldr	w0, [x19, 40]
	cmp	w0, 11
	bls	.L134
	mov	w1, w21
	mov	x0, x20
	bl	BsShow
	mov	w19, w0
	mov	w1, w21
	mov	x0, x20
	bl	BsSkip
	cmp	w19, 1
	bne	.L121
.L134:
	mov	w1, 5
	mov	x0, x20
	bl	BsShow
	mov	w19, w0
	mov	w1, 5
	mov	x0, x20
	bl	BsSkip
	str	w19, [x23]
	cmp	w19, 31
	bgt	.L121
	mov	w1, 1
	mov	x0, x20
	bl	BsShow
	mov	w19, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	cmp	w19, 1
	bne	.L121
	mov	w1, 2
	mov	x0, x20
	bl	BsShow
	mov	w21, w0
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
	str	w21, [x24]
	b	.L141
	.p2align 3
.L153:
	bl	BsShow
	mov	w25, w0
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
	str	w25, [x21, 36]
	b	.L123
.L124:
	bl	BsShow
	mov	w25, w0
	mov	w1, 13
	mov	x0, x20
	bl	BsSkip
	ldr	w0, [x21, 44]
	cmp	w0, w25
	bne	.L121
	ldr	w25, [x21, 4]
	cbz	w25, .L146
	mov	w1, w25
	mov	x0, x20
	bl	BsShow
	mov	w26, w0
	mov	w1, w25
	mov	x0, x20
	bl	BsSkip
	ldr	w0, [x21, 56]
	cmp	w0, w26
	bne	.L121
.L146:
	ldr	w25, [x21, 32]
	mov	x0, x20
	mov	w1, w25
	bl	BsShow
	mov	w26, w0
	mov	w1, w25
	mov	x0, x20
	bl	BsSkip
	str	w26, [x21, 40]
	mov	w1, 1
	mov	x0, x20
	bl	BsShow
	mov	w25, w0
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	ldrb	w0, [x19, 8]
	cmp	w0, w25
	beq	.L118
	mov	w19, 0
	b	.L155
	.size	Real8_CB_GetSliceHeader, .-Real8_CB_GetSliceHeader
	.align	3
.LC28:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_CB_GetPictureHeader
	.type	Real8_CB_GetPictureHeader, %function
Real8_CB_GetPictureHeader:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 88]
	cmp	w0, 1
	beq	.L183
	ldr	x3, .LC31
	mov	x2, 152
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	mov	w1, 24
	mov	x0, x20
	bl	BsShow
	mov	w21, w0
	mov	w1, 24
	mov	x0, x20
	bl	BsSkip
	cmp	w21, 1
	bne	.L175
	mov	x0, x20
	add	x1, x29, 60
	bl	Real8_CB_GetVLCBits
	cmp	w0, 30
	bls	.L175
	ldr	w0, [x29, 60]
	tbnz	x0, 0, .L175
	tbz	x0, 1, .L184
	stp	wzr, wzr, [x19, 96]
.L168:
	ubfx	x1, x0, 2, 5
	ubfx	x0, x0, 7, 8
	str	w1, [x19, 132]
	add	x1, x29, 60
	str	w0, [x19, 120]
	mov	x0, x20
	bl	Real8_CB_GetVLCBits
	cmp	w0, 1
	beq	.L179
	cmp	w0, 3
	beq	.L185
	cmp	w0, 5
	bne	.L175
	ldr	w0, [x29, 60]
	cbnz	w0, .L175
	mov	w0, 2
.L179:
	str	w0, [x19, 92]
.L170:
	ldr	w0, [x19, 100]
	cbz	w0, .L186
.L172:
	sub	w0, w0, #48
	mov	w1, 8144
	cmp	w0, w1
	bhi	.L175
.L182:
	ldr	w0, [x19, 96]
	sub	w0, w0, #48
	cmp	w0, w1
	bhi	.L175
	mov	x0, x19
	bl	Real8_CB_SetDimensions
	mov	w0, 1
.L158:
	ldp	w2, w1, [x19, 96]
	add	w1, w1, 15
	add	w2, w2, 15
	lsr	w2, w2, 4
	lsr	w1, w1, 4
	stp	w2, w1, [x19, 104]
	mul	w1, w1, w2
	str	w1, [x19, 112]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L184:
	mov	w2, 176
	mov	w1, 144
	stp	w1, w2, [x19, 96]
	b	.L168
	.p2align 3
.L185:
	ldr	w0, [x29, 60]
	cmp	w0, 1
	beq	.L187
.L175:
	mov	w0, -2
.L189:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L183:
	add	x21, x19, 8
	mov	w0, -1
	mov	x5, x19
	add	x4, x29, 60
	add	x3, x29, 52
	add	x2, x29, 56
	str	w0, [x21, 40]
	mov	w1, 0
	mov	x0, x20
	bl	Real8_CB_GetSliceHeader
	cmp	w0, 1
	mov	w0, -2
	bne	.L158
	ldr	w0, [x21, 36]
	cmp	w0, 3
	bls	.L188
.L159:
	ldrb	w1, [x19, 8]
	str	w1, [x19, 124]
	mov	w1, 8144
	ldr	w0, [x19, 100]
	ldrb	w2, [x21, 1]
	str	w2, [x19, 128]
	sub	w0, w0, #48
	cmp	w0, w1
	ldrb	w0, [x21, 2]
	str	w0, [x19, 132]
	ldr	w0, [x21, 44]
	str	w0, [x19, 120]
	ldr	w0, [x21, 48]
	str	w0, [x19, 140]
	ldr	w0, [x21, 52]
	str	w0, [x19, 144]
	bls	.L182
	mov	w0, -2
	b	.L189
	.p2align 3
.L186:
	mov	x1, x19
	mov	x0, x20
	bl	Real8_CB_GetCPFMT
	cbz	w0, .L175
	ldr	w0, [x19, 100]
	b	.L172
	.p2align 3
.L188:
	ldr	x1, .LC30
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx161
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx161:
	.section	.rodata
	.align	0
	.align	2
.L161:
	.byte	(.L160 - .Lrtx161) / 4
	.byte	(.L162 - .Lrtx161) / 4
	.byte	(.L163 - .Lrtx161) / 4
	.byte	(.L164 - .Lrtx161) / 4
	.text
	.p2align 3
.L187:
	str	wzr, [x19, 92]
	b	.L170
.L163:
	mov	w0, 1
	str	w0, [x19, 92]
	b	.L159
.L162:
	str	wzr, [x19, 92]
	str	wzr, [x21, 20]
	b	.L159
.L160:
	str	wzr, [x19, 92]
	mov	w0, 1
	str	w0, [x21, 20]
	b	.L159
.L164:
	mov	w0, 2
	str	w0, [x19, 92]
	b	.L159
	.size	Real8_CB_GetPictureHeader, .-Real8_CB_GetPictureHeader
	.align	3
.LC30:
	.xword	.L161
	.align	3
.LC31:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_SetImgFormat
	.type	Real8_SetImgFormat, %function
Real8_SetImgFormat:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, 52344
	add	x2, x20, 53248
	add	x0, x20, x0
	ldr	w1, [x0, 1544]
	ldr	w0, [x2, 1304]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L190
	add	x3, x20, 49152
	mov	w2, 7200
	mov	w5, 262144
	mov	w1, 0
	ldr	w4, [x3, 3452]
	and	w4, w4, 3
	orr	w2, w4, w2
	str	w2, [x0, 108]
	ldr	x6, [x20]
	ldr	w6, [x6, 592]
	stp	wzr, w4, [x0, 96]
	cmp	w6, 1
	str	w4, [x0, 104]
	csel	w4, w5, wzr, eq
	orr	w2, w4, w2
	str	w2, [x0, 108]
	ldr	w2, [x3, 3460]
	str	w2, [x0, 112]
	ldr	w2, [x3, 3456]
	str	w2, [x0, 116]
	bl	SetAspectRatio
	ldr	x0, [x20]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L198
.L193:
	mov	w1, 1
	strb	w1, [x19, 371]
	strb	w1, [x19, 369]
	add	x1, x19, 256
	strb	wzr, [x19, 364]
	mov	w0, 8
	stp	w0, w0, [x1, 120]
	stp	w0, w0, [x1, 128]
.L190:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L198:
	ldr	w2, [x19, 116]
	ldr	w3, [x19, 140]
	add	w2, w2, 15
	ldr	w0, [x19, 44]
	lsl	w1, w3, 4
	lsr	w2, w2, 4
	ldr	x4, [x19, 504]
	add	w5, w3, w0
	mul	w1, w2, w1
	str	w0, [x19, 28]
	str	w0, [x19, 60]
	add	w0, w1, w0
	add	w3, w3, w0
	add	x1, x4, x1, sxtw
	str	w5, [x19, 36]
	str	x1, [x19, 512]
	str	x4, [x19, 520]
	str	w0, [x19, 32]
	str	w3, [x19, 40]
	str	w0, [x19, 48]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	ldr	w1, [x19, 32]
	str	w1, [x19, 64]
	b	.L193
	.size	Real8_SetImgFormat, .-Real8_SetImgFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	RV8_RecordVFMWState
	.type	RV8_RecordVFMWState, %function
RV8_RecordVFMWState:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	RV8_RecordVFMWState, .-RV8_RecordVFMWState
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_WriteSliceMsg
	.type	Real8_WriteSliceMsg, %function
Real8_WriteSliceMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	sxtw	x0, w20
	add	x1, x19, 49152
	lsl	x2, x0, 2
	add	x19, x19, 16
	sub	x2, x2, x0
	add	w20, w20, 1
	lsl	x2, x2, 2
	sub	x2, x2, x0
	ldr	w0, [x1, 3540]
	add	x2, x19, x2, lsl 2
	str	w0, [x2, 100]
	ldr	w0, [x1, 3532]
	str	w0, [x2, 108]
	ldr	w0, [x1, 3536]
	str	w0, [x2, 116]
	ldr	w0, [x1, 3552]
	str	w0, [x2, 104]
	ldr	w0, [x1, 3544]
	str	w0, [x2, 112]
	ldr	w0, [x1, 3548]
	str	w0, [x2, 120]
	ldr	w0, [x1, 3520]
	str	w0, [x2, 124]
	ldr	w0, [x1, 3516]
	str	w0, [x2, 128]
	ldr	w0, [x1, 3512]
	str	w0, [x2, 132]
	ldr	w0, [x1, 3524]
	str	w0, [x2, 136]
	str	w20, [x19, 36]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real8_WriteSliceMsg, .-Real8_WriteSliceMsg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_WritePicMsg
	.type	Real8_WritePicMsg, %function
Real8_WritePicMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 49152
	mov	x0, 52344
	add	x0, x19, x0
	add	x2, x2, 3360
	ldr	w3, [x2, 92]
	strb	w3, [x19, 28]
	cmp	w3, 2
	ldr	w3, [x2, 108]
	str	w3, [x19, 20]
	ldr	w3, [x2, 104]
	str	w3, [x19, 24]
	ldr	w3, [x0, 1460]
	str	w3, [x19, 32]
	ldr	w3, [x0, 1464]
	str	w3, [x19, 36]
	ldr	w2, [x2, 132]
	str	w2, [x19, 40]
	ldr	w3, [x0, 364]
	str	w3, [x19, 44]
	ldr	w3, [x0, 368]
	str	w3, [x19, 48]
	beq	.L201
	str	w2, [x0, 364]
.L201:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real8_WritePicMsg, .-Real8_WritePicMsg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_WriteReg
	.type	Real8_WriteReg, %function
Real8_WriteReg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	str	w20, [x19, 56]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real8_WriteReg, .-Real8_WriteReg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real8_ModifySliceMsg
	.type	Real8_ModifySliceMsg, %function
Real8_ModifySliceMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 16
	ldr	w1, [x0, 136]
	cbnz	w1, .L211
	ldp	w1, w4, [x0, 4]
	ldr	w5, [x0, 36]
	mul	w4, w1, w4
	cmp	w5, 2
	sub	w4, w4, #1
	bhi	.L233
.L210:
	cmp	w5, 1
	bls	.L215
	sub	w7, w5, #1
	mov	w3, 0
	b	.L216
	.p2align 3
.L217:
	add	w3, w3, 1
	lsl	x1, x1, 2
	sub	x1, x1, x2
	sxtw	x6, w3
	lsl	x2, x6, 2
	add	x1, x0, x1, lsl 2
	sub	x2, x2, x6
	lsl	x2, x2, 2
	sub	x2, x2, x6
	ldr	w6, [x1, 136]
	add	x1, x0, x2, lsl 2
	ldr	w1, [x1, 136]
	cmp	w6, w1
	bge	.L211
.L216:
	sxtw	x2, w3
	cmp	w3, w7
	lsl	x1, x2, 2
	sub	x1, x1, x2
	bne	.L217
	uxtw	x3, w3
	lsl	x1, x3, 2
	sub	x1, x1, x3
	lsl	x1, x1, 2
	sub	x3, x1, x3
	add	x3, x0, x3, lsl 2
	ldr	w1, [x3, 136]
	cmp	w4, w1
	ccmp	w1, wzr, 4, ge
	ble	.L211
.L215:
	cbz	w5, .L218
	sub	w9, w5, #1
	mov	w8, 0
	mov	x10, 44
	b	.L221
	.p2align 3
.L236:
	beq	.L234
.L221:
	add	w3, w8, 1
	sxtw	x7, w8
	lsl	x2, x7, 2
	cmp	w8, w9
	sxtw	x6, w3
	sub	x2, x2, x7
	lsl	x3, x6, 2
	lsl	x2, x2, 2
	sub	x3, x3, x6
	sub	x2, x2, x7
	lsl	x3, x3, 2
	beq	.L235
	sub	x3, x3, x6
	add	x2, x0, x2, lsl 2
	add	x3, x0, x3, lsl 2
	ldr	w3, [x3, 136]
	sub	w3, w3, #1
	str	w3, [x2, 140]
.L220:
	lsl	x1, x7, 2
	add	w8, w8, 1
	sub	x1, x1, x7
	lsl	x1, x1, 2
	sub	x1, x1, x7
	add	x1, x0, x1, lsl 2
	ldr	w2, [x1, 136]
	cmp	w2, w3
	ccmp	w3, wzr, 1, le
	cset	w1, lt
	cmp	w2, wzr
	ccmp	w4, w2, 1, ge
	cset	w2, lt
	cmp	w4, w3
	cset	w3, lt
	orr	w1, w1, w2
	orr	w1, w3, w1
	cmp	w8, w5
	cbz	w1, .L236
	.p2align 2
.L211:
	mov	w0, 0
.L231:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L233:
	ldr	w1, [x0, 180]
	sub	w6, w5, #1
	mov	w2, 1
	cmp	w4, w1
	ccmp	w1, wzr, 4, gt
	bgt	.L213
	b	.L211
	.p2align 3
.L214:
	sub	x1, x1, x3
	lsl	x1, x1, 2
	sub	x1, x1, x3
	add	x1, x0, x1, lsl 2
	ldr	w1, [x1, 136]
	cmp	w1, wzr
	ccmp	w4, w1, 4, gt
	ble	.L211
.L213:
	add	w2, w2, 1
	cmp	w2, w6
	sxtw	x3, w2
	lsl	x1, x3, 2
	bne	.L214
	b	.L210
.L234:
	ldr	w3, [x0, 100]
	mov	w6, 0
	mov	x4, 0
	mov	w2, w3
	b	.L224
	.p2align 3
.L237:
	ldr	w2, [x2, 100]
.L224:
	lsl	x1, x4, 2
	add	w6, w6, 1
	sub	x1, x1, x4
	cmp	w3, w2
	lsl	x1, x1, 2
	csel	w3, w3, w2, ls
	sub	x1, x1, x4
	sxtw	x4, w6
	lsl	x2, x4, 2
	add	x1, x0, x1, lsl 2
	sub	x2, x2, x4
	lsl	x2, x2, 2
	sub	x2, x2, x4
	ldr	w7, [x1, 104]
	add	x2, x0, x2, lsl 2
	cmp	w7, w3
	bcs	.L222
	ldr	w1, [x1, 120]
	cmp	w1, wzr
	csel	w3, w3, w7, eq
.L222:
	cmp	w6, w5
	bne	.L237
	and	w3, w3, -16
	mov	w6, 0
	str	w3, [x0, 44]
	b	.L228
	.p2align 3
.L238:
	ldr	w2, [x1, 108]
	ldr	w7, [x1, 100]
	mov	x4, x2
	add	w2, w7, w2, lsr 3
	and	w4, w4, 7
	sub	w3, w2, w3
	ubfiz	w2, w3, 3, 4
	and	w3, w3, -16
	add	w2, w2, w4
	str	w3, [x1, 100]
	str	w2, [x1, 108]
.L226:
	add	w6, w6, 1
	cmp	w5, w6
	bls	.L227
	ldr	w3, [x0, 44]
.L228:
	sxtw	x2, w6
	lsl	x1, x2, 2
	sub	x1, x1, x2
	lsl	x1, x1, 2
	sub	x1, x1, x2
	add	x1, x0, x1, lsl 2
	ldr	w2, [x1, 104]
	cbz	w2, .L238
	ldp	w4, w2, [x1, 108]
	ldp	w10, w9, [x1, 100]
	mov	x7, x2
	mov	x8, x4
	add	w2, w9, w2, lsr 3
	add	w4, w10, w4, lsr 3
	sub	w4, w4, w3
	sub	w3, w2, w3
	ubfiz	w10, w4, 3, 4
	ubfiz	w9, w3, 3, 4
	and	w2, w7, 7
	and	w8, w8, 7
	and	w4, w4, -16
	add	w7, w10, w8
	and	w3, w3, -16
	add	w2, w9, w2
	stp	w4, w3, [x1, 100]
	stp	w7, w2, [x1, 108]
	b	.L226
.L218:
	ldr	w1, [x0, 100]
	and	w1, w1, -16
	str	w1, [x0, 44]
.L227:
	mov	w0, 1
	b	.L231
.L235:
	madd	x1, x7, x10, x0
	mov	w3, w4
	str	w4, [x1, 140]
	b	.L220
	.size	Real8_ModifySliceMsg, .-Real8_ModifySliceMsg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_Init
	.type	REAL8DEC_Init, %function
REAL8DEC_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	add	x20, x19, 53248
	bl	_mcount
	ldr	x3, .LC32
	mov	x2, 54568
	mov	w1, 0
	mov	x0, x19
	ldr	w22, [x20, 1296]
	ldr	x3, [x3, 96]
	blr	x3
	str	w22, [x20, 1296]
	add	x0, x20, 696
	bl	ResetVoQueue
	str	x21, [x19]
	add	x1, x19, 8192
	mov	w5, 15
	add	x1, x1, 3192
	add	x2, x19, 49152
	add	x0, x1, 40960
	mov	w3, 1
	mov	w6, 8192
	str	w5, [x0, 364]
	str	w5, [x0, 368]
	mov	x0, x19
	ldr	w5, [x21, 8]
	strb	w5, [x1, 18]
	strb	wzr, [x1, 20]
	strb	w3, [x2, 3360]
	str	w3, [x2, 3448]
	str	w6, [x2, 3476]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x20, 1304]
	beq	.L242
	mov	w0, 0
.L240:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L242:
	ldr	x1, .LC34
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L240
	.size	REAL8DEC_Init, .-REAL8DEC_Init
	.align	3
.LC32:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC34:
	.xword	.LC33
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_Destroy
	.type	REAL8DEC_Destroy, %function
REAL8DEC_Destroy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, 52344
	ldr	x1, .LC36
	add	x2, x19, x0
	mov	w0, 2
	ldr	w2, [x2, 1436]
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	REAL8DEC_Destroy, .-REAL8DEC_Destroy
	.align	3
.LC36:
	.xword	.LC35
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_Decode
	.type	REAL8DEC_Decode, %function
REAL8DEC_Decode:
	stp	x29, x30, [sp, -432]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	add	x23, x20, 49152
	bl	_mcount
	add	x24, x20, 16
	add	x5, x29, 280
	add	x0, x29, 344
	add	x8, x29, 256
	ldr	w3, [x23, 3428]
	ldr	w2, [x23, 3432]
	mov	w4, 8192
	stp	xzr, xzr, [x5]
	add	x5, x29, 296
	str	w3, [x23, 3436]
	stp	xzr, xzr, [x0]
	add	x0, x29, 360
	str	w2, [x23, 3440]
	stp	xzr, xzr, [x5]
	add	x5, x29, 312
	str	w4, [x23, 3476]
	stp	xzr, xzr, [x0]
	add	x0, x29, 392
	stp	xzr, xzr, [x5]
	add	x5, x29, 328
	stp	xzr, xzr, [x0]
	mov	w0, 1
	str	w0, [x23, 3448]
	stp	xzr, xzr, [x5]
	add	x5, x29, 376
	stp	xzr, xzr, [x5]
	add	x5, x29, 408
	stp	xzr, xzr, [x5]
	add	x5, x29, 216
	stp	xzr, xzr, [x5]
	add	x5, x29, 232
	stp	xzr, xzr, [x5]
	add	x5, x29, 248
	stp	xzr, xzr, [x5]
	add	x5, x29, 264
	stp	xzr, xzr, [x5]
	stp	w3, w2, [x8, 92]
	stp	w3, w2, [x8, 100]
	str	w0, [x29, 368]
	str	w4, [x29, 396]
	str	xzr, [x29, 424]
	cbz	x19, .L376
	ldr	x26, .LC41
	mov	w1, 0
	mov	x2, 11364
	mov	x0, x24
	add	x22, x20, 8192
	ldr	x3, [x26, 96]
	blr	x3
	add	x1, x22, 3192
	add	x0, x1, 40960
	str	wzr, [x0, 1400]
	ldr	w2, [x19, 16]
	str	w2, [x0, 1420]
	ldr	w2, [x19, 12]
	str	w2, [x0, 1404]
	ldr	x2, [x19]
	str	x2, [x20, 11384]
	ldr	w2, [x19, 16]
	str	w2, [x0, 1408]
	ldrb	w3, [x19, 9]
	strb	w3, [x1, 16]
	ldrb	w3, [x19, 9]
	cmp	w3, 1
	beq	.L247
	ldr	w3, [x19, 92]
	str	w3, [x0, 1412]
	ldr	x3, [x19, 80]
	str	x3, [x1, 8]
	ldr	w3, [x19, 96]
	str	w3, [x0, 1416]
	ldrb	w0, [x19, 89]
	strb	w0, [x1, 17]
	ldrb	w0, [x19, 89]
	cmp	w0, 1
	bne	.L377
	mov	w0, 65460
	sub	w1, w2, #75
	movk	w0, 0x2f, lsl 16
	cmp	w1, w0
	bhi	.L250
.L249:
	ldr	x1, [x20, 11384]
	cbz	x1, .L250
	add	x0, x20, 53248
	str	x0, [x29, 136]
	add	x19, x0, 656
	mov	x0, x19
	bl	BsInit
	mov	w1, 8
	mov	x0, x19
	bl	BsShow
	mov	w21, w0
	mov	w1, 8
	mov	x0, x19
	str	w21, [x29, 128]
	bl	BsSkip
	cmp	w21, 8
	bhi	.L250
	mov	x25, 0
	add	x21, x29, 216
	.p2align 2
.L251:
	mov	w1, 32
	mov	x0, x19
	bl	BsShow
	mov	w27, w0
	mov	w1, 32
	mov	x0, x19
	rev	w27, w27
	bl	BsSkip
	str	w27, [x21, x25]
	add	x25, x25, 4
	cmp	x25, 64
	bne	.L251
	mov	w1, 8
	mov	x0, x19
	bl	BsShow
	mov	w25, w0
	mov	w1, 8
	mov	x0, x19
	bl	BsSkip
	add	w0, w25, 1
	cmp	w25, 255
	add	x25, x22, 3192
	mov	w2, w0
	add	x27, x25, 40960
	str	w0, [x27, 372]
	bhi	.L250
	cbz	w0, .L252
	stp	x27, x25, [x29, 144]
	mov	w28, 3145727
	mov	w27, 0
	b	.L253
	.p2align 3
.L378:
	ldr	x0, [x29, 144]
	ldr	w2, [x0, 372]
	cmp	w2, w27
	bls	.L252
.L253:
	mov	w1, 32
	mov	x0, x19
	bl	BsSkip
	mov	w1, 32
	mov	x0, x19
	bl	BsShow
	mov	w25, w0
	mov	w1, 32
	mov	x0, x19
	bl	BsSkip
	uxtw	x1, w27
	ldr	x2, [x29, 152]
	add	x1, x1, 8192
	rev	w0, w25
	cmp	w0, w28
	add	w27, w27, 1
	add	x1, x2, x1, lsl 2
	str	w0, [x1, 8568]
	ble	.L378
	.p2align 2
.L250:
	mov	w28, 0
.L364:
	mov	w0, w28
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 432
	ret
	.p2align 3
.L377:
	add	x20, x20, 53248
	ldr	w1, [x19, 20]
	mov	w28, 0
	ldr	w0, [x20, 1304]
	bl	SM_ReleaseStreamSeg
	ldr	w1, [x19, 100]
	ldr	w0, [x20, 1304]
	bl	SM_ReleaseStreamSeg
	mov	w0, w28
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 432
	ret
	.p2align 3
.L247:
	str	wzr, [x0, 1412]
	str	xzr, [x1, 8]
	str	wzr, [x0, 1416]
	mov	w0, 65460
	strb	wzr, [x1, 17]
	movk	w0, 0x2f, lsl 16
	sub	w1, w2, #75
	cmp	w1, w0
	bls	.L249
	b	.L250
	.p2align 3
.L376:
	mov	x2, 52344
	ldr	x1, .LC40
	add	x20, x20, x2
	mov	w28, 0
	ldr	w2, [x20, 1436]
	bl	dprint_vfmw
	mov	w0, w28
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 432
	ret
	.p2align 3
.L252:
	add	x3, x23, 3568
	sub	w5, w2, #1
	mov	w1, 0
	add	x6, x22, 3192
	b	.L254
	.p2align 3
.L255:
	add	x0, x0, 8192
	ldr	w4, [x3], 4
	add	x0, x6, x0, lsl 2
	ldr	w0, [x0, 8568]
	cmp	w0, w4
	ble	.L250
.L254:
	cmp	w1, w5
	add	w0, w1, 1
	mov	x1, x0
	bne	.L255
	add	x4, x22, 3192
	lsl	w2, w2, 3
	add	x0, x4, 40960
	add	w25, w2, 66
	mov	w2, 65461
	mov	w1, 3145727
	movk	w2, 0x2f, lsl 16
	str	x4, [x29, 152]
	ldr	w27, [x0, 376]
	add	w27, w25, w27
	sub	w3, w27, #74
	cmp	w3, w2
	ccmp	w25, w1, 0, ls
	bgt	.L250
	ldr	w2, [x0, 1408]
	sub	w2, w2, w27
	cmp	w2, wzr
	ble	.L250
	ldr	x1, [x20, 11384]
	mov	x0, x19
	add	x1, x1, x27, sxtw
	bl	BsInit
	str	x21, [x29, 296]
	ldr	w0, [x29, 128]
	add	x1, x29, 432
	ldr	x3, .LC42
	str	w0, [x29, 304]
	add	x2, x1, x0, uxtw 2
	add	x3, x3, 448
	ldr	w0, [x3, 32]
	add	x1, x29, 280
	str	w0, [x29, 208]
	mov	x0, x19
	ldp	x6, x7, [x3]
	stp	x6, x7, [x29, 176]
	ldp	x6, x7, [x3, 16]
	stp	x6, x7, [x29, 192]
	ldr	w2, [x2, -256]
	str	w2, [x29, 292]
	bl	Real8_CB_GetPictureHeader
	cmp	w0, 1
	mov	w28, w0
	ldr	x4, [x29, 152]
	bne	.L379
	ldrb	w0, [x4, 20]
	cbnz	w0, .L257
	ldr	x21, .LC45
	ldr	x4, [x21]
	cbz	x4, .L258
	ldr	w0, [x23, 3464]
	mov	w3, 8
	strh	w0, [x29, 178]
	add	x2, x29, 176
	ldr	w0, [x29, 384]
	strh	w0, [x29, 182]
	ldr	x0, [x29, 136]
	ldr	w1, [x29, 388]
	strh	w1, [x29, 180]
	ldr	w0, [x0, 1304]
	ldr	w1, [x23, 3468]
	strh	w1, [x29, 176]
	mov	w1, 2
	blr	x4
.L258:
	ldr	x0, [x29, 136]
	ldr	w0, [x0, 1304]
	bl	FSP_GetFspType
	cbnz	w0, .L259
	ldr	x0, [x29, 136]
	add	x2, x29, 172
	add	x3, x29, 176
	add	x1, x29, 168
	stp	wzr, wzr, [x29, 168]
	ldr	w0, [x0, 1304]
	str	wzr, [x29, 176]
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x20]
	ldr	w2, [x0, 708]
	cmp	w2, 1
	beq	.L380
.L259:
	add	x4, x22, 3192
	ldr	x1, [x29, 136]
	add	x21, x4, 40960
	mov	w2, 1
	add	x3, x1, 696
	ldr	w0, [x1, 1304]
	ldr	w1, [x21, 1540]
	stp	x4, x3, [x29, 144]
	bl	FSP_ClearLogicFs
	ldr	x0, [x29, 136]
	mov	w2, 1
	ldr	w1, [x21, 1536]
	ldr	w0, [x0, 1304]
	bl	FSP_ClearLogicFs
	str	wzr, [x21, 1536]
	ldr	x0, [x29, 136]
	str	wzr, [x21, 1540]
	ldr	w0, [x0, 1304]
	bl	FSP_GetNullLogicFs
	str	w0, [x21, 1536]
	str	w0, [x21, 1540]
	ldp	x4, x1, [x29, 144]
	ldr	x0, [x29, 136]
	strb	wzr, [x4, 23]
	ldr	w0, [x0, 1304]
	bl	FSP_ClearNotInVoQueue
	ldr	w2, [x29, 388]
	ldr	w3, [x29, 384]
	cmp	w2, 512
	ldr	x4, [x29, 144]
	bhi	.L266
	ldr	w0, [x29, 384]
	cmp	w0, 512
	mov	w3, w0
	bhi	.L266
	ldr	x1, [x20]
	ldr	w3, [x1, 708]
	cbnz	w3, .L268
	ldr	w1, [x1, 28]
	cmp	w1, 25
	beq	.L381
.L268:
	ldr	x0, [x29, 136]
	ldr	w0, [x0, 1304]
	bl	FSP_GetFspType
	cbz	w0, .L365
.L368:
	ldr	w0, [x29, 384]
	ldr	w2, [x29, 388]
.L270:
	ldr	x1, .LC51
	mov	w3, w0
	mov	w0, 1
	bl	dprint_vfmw
	add	x2, x29, 280
	mov	x1, x20
	mov	w0, 6
	bl	Real8_ArrangeVHBMem
	cbz	w0, .L382
.L257:
	ldr	x3, [x26, 104]
	add	x0, x23, 3360
	add	x1, x29, 280
	mov	x2, 152
	str	x0, [x29, 144]
	add	x21, x22, 3192
	blr	x3
	ldr	x0, [x20]
	ldr	w0, [x0, 8]
	strb	w0, [x22, 3210]
	mov	x0, x20
	bl	Real8_GetImageBuffer
	cbz	w0, .L383
	ldr	x0, [x29, 136]
	add	x26, x21, 40960
	ldr	w0, [x0, 1304]
	ldr	w1, [x26, 1544]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L273
	ldr	x0, [x20]
	mov	x4, -1
	ldr	x1, .LC57
	ldr	x0, [x0, 56]
	str	x0, [x2, 600]
	mov	w0, 29
	ldr	x3, [x20]
	ldr	x3, [x3, 72]
	str	x3, [x2, 608]
	ldr	x3, [x20]
	ldr	x3, [x3, 88]
	str	x3, [x2, 616]
	ldr	x3, [x20]
	ldr	w3, [x3, 104]
	str	w3, [x2, 8]
	ldr	x3, [x20]
	ldr	w3, [x3, 108]
	str	w3, [x2, 12]
	ldr	x3, [x20]
	ldr	w3, [x3, 112]
	str	w3, [x2, 16]
	ldr	x3, [x20]
	ldr	w3, [x3, 128]
	str	w3, [x2, 20]
	ldr	x3, [x20]
	str	x4, [x3, 56]
	ldr	x3, [x2, 608]
	ldr	x2, [x2, 600]
	bl	dprint_vfmw
	ldr	x0, [x29, 144]
	ldr	w0, [x0, 92]
	cmp	w0, 2
	beq	.L384
	cmp	w0, 1
	beq	.L385
	ldrb	w0, [x21, 23]
	cmp	w0, wzr
	mov	w0, 2
	csinc	w0, w0, wzr, ne
.L277:
	add	x21, x22, 3192
	strb	w0, [x22, 3215]
	add	x21, x21, 40960
	ldr	w2, [x21, 1536]
	ldr	w0, [x21, 1540]
	cmp	w2, w0
	beq	.L278
	ldr	x1, .LC59
	mov	w0, 13
	bl	dprint_vfmw
	ldr	x0, [x29, 136]
	mov	w2, 0
	ldr	w1, [x21, 1536]
	ldr	w0, [x0, 1304]
	bl	FSP_SetRef
.L278:
	add	x26, x22, 3192
	mov	w2, 1
	add	x21, x26, 40960
	ldr	w0, [x21, 1540]
	ldr	w1, [x21, 1544]
	str	w0, [x21, 1536]
	ldr	x0, [x29, 136]
	str	w1, [x21, 1540]
	ldr	w0, [x0, 1304]
	bl	FSP_SetRef
	ldr	x0, [x29, 136]
	ldr	w1, [x21, 1536]
	ldr	w0, [x0, 1304]
	bl	FSP_RemovePmv
	ldr	x1, .LC61
	mov	w0, 13
	ldr	w2, [x21, 1544]
	bl	dprint_vfmw
	ldrb	w0, [x22, 3210]
	cmp	w0, 4
	bne	.L280
	ldr	w0, [x23, 3452]
	cbnz	w0, .L283
	str	wzr, [x21, 1552]
.L280:
	add	x26, x22, 3192
	ldr	x0, [x29, 136]
	add	x26, x26, 40960
	ldr	w0, [x0, 1304]
	ldr	w1, [x26, 1544]
	bl	FSP_GetLogicFs
	mov	x21, x0
	ldr	x0, [x29, 136]
	ldr	w1, [x26, 1536]
	ldr	w0, [x0, 1304]
	bl	FSP_GetLogicFs
	mov	x9, x0
	ldr	x0, [x29, 136]
	ldr	w1, [x26, 1540]
	str	x9, [x29, 152]
	ldr	w0, [x0, 1304]
	bl	FSP_GetLogicFs
	mov	x26, x0
	ldr	x9, [x29, 152]
	cmp	x21, xzr
	ccmp	x9, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L386
	ldr	x0, [x21, 640]
	cbz	x0, .L287
	ldr	x1, [x21, 656]
	cbz	x1, .L287
	ldr	x1, [x21, 648]
	cbz	x1, .L287
	ldr	x1, [x26, 640]
	cbz	x1, .L289
	ldr	w5, [x1, 16]
.L290:
	str	w5, [x24, 48]
	ldr	x0, [x9, 640]
	cbz	x0, .L387
.L369:
	ldr	w4, [x0, 16]
	str	w4, [x24, 52]
	ldr	x1, .LC68
	ldr	x0, [x21, 640]
	str	x9, [x29, 152]
	ldr	w2, [x0, 16]
	mov	w0, 13
	str	w2, [x24, 56]
	ldr	w6, [x21, 664]
	str	w6, [x24, 72]
	ldr	w7, [x26, 664]
	str	w7, [x24, 76]
	ldr	x3, [x21, 648]
	ldr	w3, [x3, 16]
	str	w3, [x24, 60]
	bl	dprint_vfmw
	ldr	x0, [x21, 648]
	ldr	x9, [x29, 152]
	ldr	w0, [x0, 32]
	str	w0, [x24, 64]
	ldr	x0, [x21, 648]
	ldr	w0, [x0, 76]
	str	w0, [x24, 88]
	ldr	x0, [x21, 648]
	ldr	w0, [x0, 60]
	str	w0, [x24, 92]
	ldr	x0, [x21, 648]
	ldr	w0, [x0, 64]
	str	w0, [x24, 96]
	ldr	w0, [x23, 3452]
	cmp	w0, 2
	beq	.L388
.L293:
	ldr	w0, [x23, 3480]
	str	w0, [x21, 672]
	add	x0, x22, 3192
	add	x0, x0, 40960
	ldr	w2, [x23, 3452]
	ldr	w3, [x23, 3492]
	strb	w2, [x24, 12]
	cmp	w2, 2
	str	w3, [x29, 152]
	ldr	w2, [x23, 3468]
	str	w2, [x24, 4]
	ldr	w1, [x23, 3464]
	str	w1, [x24, 8]
	ldr	w1, [x0, 1460]
	str	w1, [x24, 16]
	ldr	w1, [x0, 1464]
	stp	w1, w3, [x24, 20]
	ldr	w1, [x0, 364]
	str	w1, [x24, 28]
	ldr	w1, [x0, 368]
	str	w1, [x24, 32]
	beq	.L298
	str	w3, [x0, 364]
.L298:
	mov	x0, x19
	bl	BsPos
	and	w1, w0, -2147483641
	add	x2, x23, 3512
	tbnz	w0, #31, .L389
.L299:
	str	w1, [x2, 20]
	add	x1, x22, 3192
	add	x1, x1, 40960
	ldr	w0, [x1, 372]
	cmp	w0, 1
	beq	.L390
	ldr	w21, [x1, 376]
	ldr	w0, [x1, 380]
.L370:
	sub	w21, w0, w21
	mov	x0, x19
	lsl	w21, w21, 3
	bl	BsPos
	sub	w0, w21, w0
	add	x21, x22, 3192
	str	w0, [x23, 3536]
	add	x9, x21, 40960
	mov	x0, x19
	str	x9, [x29, 128]
	ldr	w1, [x9, 1404]
	add	w27, w27, w1
	bl	BsPos
	add	x5, x23, 3328
	mov	w1, 8
	add	x6, x23, 3328
	ldr	w2, [x23, 3488]
	ldr	w3, [x23, 3496]
	mov	w7, 1
	sdiv	w0, w0, w1
	ldr	w4, [x29, 152]
	ldr	x9, [x29, 128]
	stp	w4, w3, [x5, 184]
	stp	w2, wzr, [x5, 192]
	stp	wzr, wzr, [x6, 220]
	add	w0, w0, w27
	stp	w0, wzr, [x6, 212]
	str	w0, [x24, 100]
	ldr	w0, [x23, 3532]
	str	w0, [x24, 108]
	ldr	w0, [x23, 3536]
	stp	wzr, w0, [x24, 112]
	str	w7, [x24, 36]
	str	wzr, [x24, 104]
	stp	wzr, w2, [x24, 120]
	stp	w3, w4, [x24, 128]
	str	wzr, [x24, 136]
	ldr	w0, [x9, 372]
	cmp	w0, w7
	bls	.L321
	ldr	w6, [x9, 380]
	mov	w0, 3145727
	add	w6, w25, w6
	cmp	w6, w0
	bgt	.L307
	cmp	w6, 73
	ble	.L307
	ldr	w2, [x9, 1408]
	sub	w2, w2, w6
	cmp	w2, wzr
	ble	.L307
	mov	w26, w7
	mov	x27, x9
	stp	w0, w7, [x29, 120]
	b	.L310
	.p2align 3
.L311:
	ldr	w0, [x27, 372]
	add	w26, w26, 1
	cmp	w0, w26
	bls	.L321
	uxtw	x0, w26
	add	x0, x0, 8192
	add	x0, x21, x0, lsl 2
	ldr	w6, [x0, 8568]
	ldr	w0, [x29, 120]
	add	w6, w25, w6
	cmp	w6, w0
	bgt	.L307
	cmp	w6, 73
	ble	.L307
	ldr	w2, [x27, 1408]
	sub	w2, w2, w6
	cmp	w2, wzr
	ble	.L307
.L310:
	ldr	x1, [x21]
	mov	x0, x19
	str	w6, [x29, 128]
	add	x1, x1, x6, sxtw
	bl	BsInit
	ldr	x5, [x29, 144]
	add	x2, x29, 168
	add	x4, x29, 176
	add	x3, x29, 172
	mov	w1, 0
	mov	x0, x19
	bl	Real8_CB_GetSliceHeader
	cmp	w0, 1
	mov	w2, w0
	ldr	w6, [x29, 128]
	bne	.L311
	ldr	w1, [x29, 168]
	ldr	w0, [x23, 3468]
	str	w1, [x29, 128]
	cbz	w0, .L391
	ldr	w0, [x29, 176]
	ldr	w1, [x29, 152]
	cmp	w0, wzr
	str	w6, [x29, 116]
	csel	w0, w1, w0, lt
	str	w0, [x29, 152]
	mov	x0, x19
	bl	BsPos
	asr	w1, w0, 31
	lsr	w1, w1, 29
	ldr	w6, [x29, 116]
	add	w0, w0, w1
	and	w0, w0, 7
	sub	w0, w0, w1
	str	w0, [x23, 3532]
	add	x0, x23, 3512
	str	x0, [x29, 104]
	ldr	w0, [x27, 372]
	sub	w0, w0, #1
	cmp	w0, w26
	beq	.L392
	cmp	w26, 254
	bhi	.L393
	add	w0, w26, 1
	uxtw	x1, w26
	add	x0, x0, 8192
	add	x1, x1, 8192
	str	w6, [x29, 104]
	add	x0, x21, x0, lsl 2
	add	x1, x21, x1, lsl 2
	ldr	w0, [x0, 8568]
	ldr	w1, [x1, 8568]
.L371:
	sub	w1, w0, w1
	mov	x0, x19
	lsl	w1, w1, 3
	str	w1, [x29, 116]
	bl	BsPos
	ldr	w1, [x29, 116]
	ldr	w6, [x29, 104]
	sub	w0, w1, w0
	str	w0, [x23, 3536]
	mov	x0, x19
	ldr	w2, [x27, 1404]
	add	w6, w6, w2
	str	w6, [x29, 116]
	bl	BsPos
	cmp	w0, wzr
	add	w3, w0, 7
	ldr	w6, [x29, 116]
	csel	w0, w3, w0, lt
	ldr	w4, [x29, 128]
	str	w4, [x23, 3524]
	add	w2, w6, w0, asr 3
	ldr	w0, [x29, 152]
	str	w0, [x23, 3512]
	add	x0, x23, 3328
	stp	w2, wzr, [x0, 212]
	stp	wzr, wzr, [x0, 220]
	ldr	w0, [x29, 124]
	sxtw	x3, w0
	add	w0, w0, 1
	mov	w5, w0
	cmp	w0, 131
	lsl	x0, x3, 2
	str	w5, [x29, 124]
	sub	x0, x0, x3
	lsl	x0, x0, 2
	sub	x0, x0, x3
	ldr	w3, [x23, 3496]
	str	w3, [x23, 3516]
	add	x0, x24, x0, lsl 2
	ldr	w3, [x23, 3488]
	str	w3, [x23, 3520]
	str	w2, [x0, 100]
	ldr	w2, [x23, 3532]
	str	w2, [x0, 108]
	ldr	w2, [x23, 3536]
	str	w2, [x0, 116]
	ldr	w2, [x23, 3552]
	str	w2, [x0, 104]
	ldr	w2, [x23, 3544]
	str	w2, [x0, 112]
	ldr	w2, [x23, 3548]
	str	w2, [x0, 120]
	ldr	w2, [x23, 3520]
	str	w2, [x0, 124]
	ldr	w2, [x23, 3516]
	str	w2, [x0, 128]
	ldr	w2, [x23, 3512]
	str	w2, [x0, 132]
	ldr	w1, [x23, 3524]
	str	w1, [x0, 136]
	str	w5, [x24, 36]
	ble	.L311
	mov	x0, x20
	bl	Real8_ModifySliceMsg
	cmp	w0, 1
	bne	.L394
	str	x24, [x20, 8]
	cbz	x24, .L395
	ldr	x0, [x20]
	ldr	w1, [x0, 864]
	cmp	w1, 1
	beq	.L396
	cmp	w1, 2
	bne	.L311
	ldr	w0, [x0, 868]
	cmp	w0, wzr
	ble	.L311
	ldrb	w0, [x24, 12]
	cmp	w0, 2
	bne	.L311
	ldr	x0, [x29, 136]
	mov	w2, 0
	str	xzr, [x20, 8]
	mov	w28, 0
	ldr	w1, [x0, 640]
	ldr	w0, [x0, 1304]
	bl	FSP_SetDisplay
	ldr	x3, [x20]
	mov	x1, x20
	mov	w0, 8
	ldr	w2, [x3, 868]
	sub	w2, w2, #1
	str	w2, [x3, 868]
	bl	ReleasePacket
	b	.L364
	.p2align 3
.L379:
	ldr	x1, .LC44
	mov	w0, 1
	mov	w28, 0
	bl	dprint_vfmw
	b	.L364
.L266:
	ldr	x1, .LC49
	mov	w0, 18
	mov	w28, 0
	bl	dprint_vfmw
	b	.L364
.L383:
	add	x21, x21, 40960
	ldr	x1, .LC55
	mov	w28, 0
	ldr	w2, [x21, 1436]
	bl	dprint_vfmw
	b	.L364
.L382:
	ldr	x1, .LC53
	mov	w0, 1
	mov	w28, 0
	bl	dprint_vfmw
	b	.L364
.L384:
	ldrb	w0, [x21, 23]
	cmp	w0, 1
	bls	.L307
	ldrb	w0, [x21, 18]
	cmp	w0, 4
	bne	.L280
.L323:
	add	x0, x22, 3192
	add	x0, x0, 40960
	ldr	w1, [x0, 1552]
	cbnz	w1, .L280
	ldr	w1, [x0, 1544]
.L372:
	ldr	x0, [x29, 136]
	mov	w2, 1
	mov	w28, 0
	ldr	w0, [x0, 1304]
	bl	FSP_ClearLogicFs
	b	.L364
.L385:
	ldrb	w1, [x21, 23]
	mov	w0, 2
	cbnz	w1, .L277
.L307:
	add	x1, x22, 3192
	ldr	x0, [x29, 136]
	add	x1, x1, 40960
	mov	w2, 1
	mov	w28, 0
	ldr	w0, [x0, 1304]
	ldr	w1, [x1, 1544]
	bl	FSP_ClearLogicFs
	b	.L364
.L381:
	ldrb	w1, [x4, 20]
	cbnz	w1, .L270
.L365:
	ldr	x0, [x29, 152]
	bl	ResetVoQueue
	b	.L368
.L390:
	ldr	w0, [x1, 1408]
	ldr	w21, [x1, 376]
	sub	w0, w0, w25
	b	.L370
.L388:
	ldr	w3, [x9, 672]
	ldr	w1, [x23, 3480]
	ldr	w0, [x26, 672]
	subs	w1, w1, w3
	sub	w0, w0, w3
	bmi	.L397
.L294:
	tbnz	w0, #31, .L398
.L295:
	cmp	w1, w0
	csel	w1, w1, wzr, le
	cmp	w0, wzr
	ble	.L327
	sub	w3, w0, w1
	lsl	w4, w1, 14
	lsl	w3, w3, 14
	sdiv	w4, w4, w0
	sdiv	w3, w3, w0
.L297:
	add	x2, x22, 3192
	add	x2, x2, 40960
	add	x7, x2, 1280
	stp	w1, w0, [x7, 172]
	stp	w4, w3, [x7, 180]
	b	.L293
.L287:
	add	x1, x22, 3192
	ldr	x0, [x29, 136]
	add	x1, x1, 40960
	mov	w2, 1
	mov	w28, 0
	ldr	w0, [x0, 1304]
	ldr	w1, [x1, 1544]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC66
	mov	w0, 0
	bl	dprint_vfmw
	b	.L364
.L386:
	ldr	x2, .LC62
	mov	w3, 2023
	ldr	x1, .LC64
	mov	w0, 22
	mov	w28, 0
	bl	dprint_vfmw
	b	.L364
.L380:
	ldr	w0, [x29, 176]
	cmp	w0, wzr
	bgt	.L262
	ldr	w0, [x29, 172]
	cmp	w0, 32
	bgt	.L262
	ldr	x0, [x20, 8]
	cbnz	x0, .L262
	ldr	x5, [x29, 136]
	ldr	w3, [x5, 528]
	cbz	w3, .L259
	ldr	w4, [x23, 3468]
	cbz	w4, .L259
	ldr	w1, [x5, 1308]
	ldr	w7, [x29, 384]
	cmp	w7, w1
	beq	.L259
	ldr	w1, [x5, 1312]
	ldr	w6, [x29, 388]
	cmp	w6, w1
	beq	.L259
	ldr	x1, .LC47
	mov	w0, w2
	ldr	w5, [x23, 3464]
	bl	dprint_vfmw
	ldr	x1, [x29, 136]
	ldr	w0, [x29, 384]
	ldr	x4, [x21]
	str	w0, [x1, 1308]
	ldr	w0, [x29, 388]
	str	w0, [x1, 1312]
	cbz	x4, .L259
	ldr	w0, [x1, 1304]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L259
.L392:
	uxtw	x0, w26
	str	w6, [x29, 104]
	add	x0, x0, 8192
	add	x0, x21, x0, lsl 2
	ldr	w1, [x0, 8568]
	ldr	w0, [x27, 1408]
	sub	w0, w0, w25
	b	.L371
.L283:
	cmp	w0, 2
	beq	.L323
	cmp	w0, 1
	bne	.L280
	str	w0, [x21, 1552]
	b	.L280
.L327:
	mov	w3, 0
	mov	w4, 0
	b	.L297
.L398:
	ldr	w2, [x23, 3476]
	add	w0, w0, w2
	b	.L295
.L273:
	ldr	w1, [x26, 1544]
	b	.L372
.L321:
	mov	w19, 1
	str	w19, [x24, 40]
	ldr	x0, [x20, 8]
	cbz	x0, .L399
.L304:
	ldr	x0, [x20]
	ldr	w1, [x0, 592]
	str	w1, [x20, 96]
	ldr	w0, [x0, 632]
	str	w0, [x20, 100]
	b	.L364
.L387:
	ldr	x0, [x21, 640]
	b	.L369
.L289:
	ldr	w5, [x0, 16]
	b	.L290
.L399:
	mov	x0, x20
	bl	Real8_ModifySliceMsg
	cmp	w0, 1
	bne	.L400
	str	x24, [x20, 8]
	b	.L304
.L262:
	mov	w28, -2
	b	.L364
.L397:
	ldr	w2, [x23, 3476]
	add	w1, w1, w2
	b	.L294
.L389:
	sub	w1, w1, #1
	orr	w1, w1, -8
	add	w1, w1, 1
	b	.L299
.L396:
	ldrb	w0, [x24, 12]
	cmp	w0, 2
	bne	.L311
	ldr	x1, [x29, 136]
	mov	w2, 0
	str	xzr, [x20, 8]
	mov	w28, 0
	ldr	w0, [x1, 1304]
	ldr	w1, [x1, 640]
	bl	FSP_SetDisplay
	mov	x1, x20
	mov	w0, 8
	bl	ReleasePacket
	b	.L364
.L395:
	ldr	x2, .LC62
	mov	w3, 2223
	ldr	x1, .LC72
	mov	w0, 22
	mov	w28, 0
	bl	dprint_vfmw
	b	.L364
.L394:
	ldr	w1, [x27, 1544]
	b	.L372
.L393:
	ldr	x1, .LC70
	mov	w2, 2190
	mov	w0, 22
	mov	w28, 0
	bl	dprint_vfmw
	b	.L364
.L391:
	add	x21, x21, 40960
	ldr	x0, [x29, 136]
	mov	w28, 0
	ldr	w1, [x21, 1544]
	ldr	w0, [x0, 1304]
	bl	FSP_ClearLogicFs
	b	.L364
.L400:
	add	x1, x22, 3192
	ldr	x0, [x29, 136]
	add	x1, x1, 40960
	mov	w2, w19
	mov	w28, 0
	ldr	w0, [x0, 1304]
	ldr	w1, [x1, 1544]
	bl	FSP_ClearLogicFs
	b	.L364
	.size	REAL8DEC_Decode, .-REAL8DEC_Decode
	.align	3
.LC40:
	.xword	.LC39
	.align	3
.LC41:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC42:
	.xword	.LANCHOR0
	.align	3
.LC44:
	.xword	.LC43
	.align	3
.LC45:
	.xword	g_event_report
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
.LC62:
	.xword	.LANCHOR1
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_RecycleImage
	.type	REAL8DEC_RecycleImage, %function
REAL8DEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L403
	add	x19, x19, 53248
	mov	w2, 0
	mov	w1, w20
	ldr	w0, [x19, 1304]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 1304]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L403:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	REAL8DEC_RecycleImage, .-REAL8DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_GetRemainImg
	.type	REAL8DEC_GetRemainImg, %function
REAL8DEC_GetRemainImg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 53248
	bl	_mcount
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L405
	mov	x21, 52344
	add	x20, x19, 53248
	add	x21, x19, x21
	ldr	w0, [x20, 1304]
	ldr	w1, [x21, 1540]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L405
	ldr	w0, [x20, 1304]
	ldr	w1, [x21, 1540]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L418
.L405:
	add	x22, x20, 696
.L409:
	mov	x0, x22
	bl	GetVoLastImageID
.L415:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L418:
	mov	x0, x19
	add	x22, x20, 696
	bl	Real8_SetImgFormat
	mov	w2, 1
	str	w2, [x23, 188]
	ldr	w0, [x20, 1304]
	ldr	w1, [x21, 1540]
	bl	FSP_SetDisplay
	ldr	w0, [x20, 1304]
	mov	x4, x23
	mov	x3, x22
	mov	x2, x19
	mov	w1, 8
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L406
	ldr	w0, [x20, 1304]
	mov	w2, 0
	ldr	w1, [x21, 1540]
	bl	FSP_SetDisplay
	b	.L409
	.p2align 3
.L406:
	ldr	w0, [x23, 160]
	cmp	w0, 100
	bhi	.L405
	ldr	w0, [x23, 112]
	cmp	w0, 31
	bls	.L405
	ldr	w1, [x23, 116]
	mov	w0, 0
	cmp	w1, 31
	bhi	.L415
	b	.L409
	.size	REAL8DEC_GetRemainImg, .-REAL8DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_VDMPostProc
	.type	REAL8DEC_VDMPostProc, %function
REAL8DEC_VDMPostProc:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	mov	w20, w1
	mov	w24, w2
	bl	_mcount
	add	x19, x21, 8192
	ldr	x1, .LC75
	mov	w2, w20
	mov	w0, 22
	add	x25, x19, 3192
	add	x22, x21, 53248
	add	x23, x25, 40960
	bl	dprint_vfmw
	mov	x1, x21
	mov	w0, 8
	bl	ReleasePacket
	ldr	w0, [x22, 1304]
	ldr	w1, [x23, 1544]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L428
	ldr	w1, [x23, 260]
	strb	w1, [x19, 3213]
	cmp	w1, 2
	beq	.L421
	and	w24, w24, 31
	str	w24, [x23, 368]
.L421:
	tbnz	w20, #31, .L429
	mov	w1, 100
	cmp	w20, w1
	csel	w20, w20, w1, le
.L422:
	ldr	x1, .LC77
	str	w20, [x0, 160]
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x21
	bl	Real8_SetImgFormat
	ldrb	w1, [x19, 3210]
	add	x0, x19, 3192
	cmp	w1, 2
	beq	.L425
	ldr	x1, [x21]
	ldr	w1, [x1, 12]
	cmp	w1, 1
	beq	.L425
	add	x1, x0, 40960
	ldr	w2, [x1, 260]
	cmp	w2, 2
	beq	.L425
	ldrb	w0, [x0, 23]
	cmp	w0, 1
	bls	.L430
	ldr	w1, [x1, 1536]
.L426:
	add	x0, x19, 3192
	cmn	w1, #1
	add	x0, x0, 40960
	str	w1, [x0, 1548]
	beq	.L427
.L447:
	add	x20, x19, 3192
	ldr	w0, [x22, 1304]
	add	x20, x20, 40960
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	ldr	w23, [x20, 1548]
	ldr	w0, [x22, 1304]
	mov	w1, w23
	bl	FSP_GetRef
	mov	w3, w0
	ldr	x1, .LC79
	mov	w2, w23
	mov	w0, 12
	bl	dprint_vfmw
	ldr	w1, [x20, 1548]
	mov	w2, 1
	ldr	w0, [x22, 1304]
	bl	FSP_SetDisplay
	ldr	w0, [x22, 1304]
	mov	x4, x24
	add	x3, x22, 696
	mov	x2, x21
	mov	w1, 8
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L446
.L427:
	add	x19, x19, 3192
	mov	w0, 1
	add	x19, x19, 40960
	ldr	w1, [x19, 1436]
	add	w1, w1, w0
	str	w1, [x19, 1436]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L425:
	add	x0, x19, 3192
	add	x0, x0, 40960
	ldr	w1, [x0, 1544]
	str	w1, [x0, 1548]
	cmn	w1, #1
	bne	.L447
	b	.L427
	.p2align 3
.L429:
	mov	w20, 0
	b	.L422
	.p2align 3
.L446:
	ldr	w1, [x20, 1548]
	mov	w2, 0
	ldr	w0, [x22, 1304]
	bl	FSP_SetDisplay
	ldr	x25, [sp, 64]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L428:
	mov	w0, 0
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L430:
	mov	w1, -1
	b	.L426
	.size	REAL8DEC_VDMPostProc, .-REAL8DEC_VDMPostProc
	.align	3
.LC75:
	.xword	.LC74
	.align	3
.LC77:
	.xword	.LC76
	.align	3
.LC79:
	.xword	.LC78
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_GetImageBuffer
	.type	REAL8DEC_GetImageBuffer, %function
REAL8DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 53248
	bl	_mcount
	ldr	w0, [x19, 1304]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	mov	w20, w0
	beq	.L449
	ldr	w0, [x19, 1304]
	mov	w20, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L454
.L449:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L454:
	ldr	w0, [x19, 1304]
	add	x1, x19, 696
	bl	FSP_ClearNotInVoQueue
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	REAL8DEC_GetImageBuffer, .-REAL8DEC_GetImageBuffer
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL8DEC_Support
	.type	REAL8DEC_Support, %function
REAL8DEC_Support:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	str	w0, [x19]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	REAL8DEC_Support, .-REAL8DEC_Support
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	gs_VLCDecodeTable, %object
	.size	gs_VLCDecodeTable, 256
gs_VLCDecodeTable:
	.byte	-128
	.byte	-127
	.byte	112
	.byte	112
	.byte	-126
	.byte	-125
	.byte	113
	.byte	113
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	-124
	.byte	-123
	.byte	114
	.byte	114
	.byte	-122
	.byte	-121
	.byte	115
	.byte	115
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	-120
	.byte	-119
	.byte	116
	.byte	116
	.byte	-118
	.byte	-117
	.byte	117
	.byte	117
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	-116
	.byte	-115
	.byte	118
	.byte	118
	.byte	-114
	.byte	-113
	.byte	119
	.byte	119
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
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
	.type	MBA_FieldWidth, %object
	.size	MBA_FieldWidth, 24
MBA_FieldWidth:
	.word	6
	.word	7
	.word	9
	.word	11
	.word	13
	.word	14
.LC0:
	.word	160
	.word	176
	.word	240
	.word	320
	.word	352
	.word	640
	.word	704
	.word	0
.LC1:
	.word	120
	.word	132
	.word	144
	.word	240
	.word	288
	.word	480
	.word	0
	.word	0
	.type	GetBitsMask, %object
	.size	GetBitsMask, 100
GetBitsMask:
	.word	0
	.word	1
	.word	3
	.word	7
	.word	15
	.word	31
	.word	63
	.word	127
	.word	255
	.word	511
	.word	1023
	.word	2047
	.word	4095
	.word	8191
	.word	16383
	.word	32767
	.word	65535
	.word	131071
	.word	262143
	.word	524287
	.word	1048575
	.word	2097151
	.word	4194303
	.word	8388607
	.word	16777215
	.zero	4
.LC2:
	.word	0
	.word	1
	.word	1
	.word	2
	.word	2
	.word	3
	.word	3
	.word	3
	.word	3
	.zero	4
	.type	s_RVVersionEncodingToMinorVersion, %object
	.size	s_RVVersionEncodingToMinorVersion, 32
s_RVVersionEncodingToMinorVersion:
	.word	2
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.align	3
.LANCHOR1 = . + 0
	.type	__FUNCTION__.11821, %object
	.size	__FUNCTION__.11821, 16
__FUNCTION__.11821:
	.string	"REAL8DEC_Decode"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC4:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC6:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC8:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC10:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC12:
	ASCII(.string	"image size abnormal (%dx%d) in MB\n" )
	.zero	5
.LC14:
	ASCII(.string	"real8 actual frame size(%dx%d) exeed max config(%dx%d)\n" )
.LC17:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC33:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC35:
	ASCII(.string	"Decoding quits at frm %d\n" )
	.zero	6
.LC39:
	ASCII(.string	"ERROR: frm %d, Invalid packet pointer!\n" )
.LC43:
	ASCII(.string	"Real8_CB_GetPictureHeader Error!\n" )
	.zero	6
.LC46:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC48:
	ASCII(.string	"pic width/height to large,MbWidth = %d, MbHeight = %d" )
	.zero	2
.LC50:
	ASCII(.string	"new seq size %d * %d\n" )
	.zero	2
.LC52:
	ASCII(.string	"Real8_ArrangeVHBMem err\n" )
	.zero	7
.LC54:
	ASCII(.string	"Fatal Error: frm%d, get image buffer failed\n" )
	.zero	3
.LC56:
	ASCII(.string	"dec_pts: %lld Usertag = %lld\n" )
	.zero	2
.LC58:
	ASCII(.string	"---------- unref %d\n" )
	.zero	3
.LC60:
	ASCII(.string	"++++++++++ ref %d\n" )
	.zero	5
.LC63:
	ASCII(.string	"%s %d unknow error!!\n" )
	.zero	2
.LC65:
	ASCII(.string	"CurFsID abnormal!\n" )
	.zero	5
.LC67:
	ASCII(.string	"dec addr: cur,cur2d,fwd,bwd,currpmv,colpmv = %p,%p,%p,%p,%p,%p\n" )
.LC69:
	ASCII(.string	"line:%d! i > (RV8_MAX_SLC_PARAM_NUM - 2)\n" )
	.zero	6
.LC71:
	ASCII(.string	"%s %d NULL == pCtx->pRv8DecParam!!\n" )
	.zero	4
.LC74:
	ASCII(.string	"ErrRatio = %d\n" )
	.zero	1
.LC76:
	ASCII(.string	"DEBUG: DEC over\n" )
	.zero	7
.LC78:
	ASCII(.string	"insert buf %d ref %d\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
