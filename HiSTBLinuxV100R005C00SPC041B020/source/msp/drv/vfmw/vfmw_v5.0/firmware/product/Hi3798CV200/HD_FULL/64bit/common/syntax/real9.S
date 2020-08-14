#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"real9.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	Real9_CB_GetBits
	.type	Real9_CB_GetBits, %function
Real9_CB_GetBits:
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
	.size	Real9_CB_GetBits, .-Real9_CB_GetBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_Get1Bit
	.type	Real9_CB_Get1Bit, %function
Real9_CB_Get1Bit:
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
	.size	Real9_CB_Get1Bit, .-Real9_CB_Get1Bit
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_GetVLCBits
	.type	Real9_CB_GetVLCBits, %function
Real9_CB_GetVLCBits:
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
	ldr	x22, .LC2
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
	.size	Real9_CB_GetVLCBits, .-Real9_CB_GetVLCBits
	.align	3
.LC2:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_GetImageBuffer
	.type	Real9_GetImageBuffer, %function
Real9_GetImageBuffer:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 53248
	add	x19, x19, 8192
	bl	_mcount
	add	x19, x19, 3184
	add	x20, x19, 40960
	mov	w1, 1
	ldr	w0, [x21, 1272]
	bl	FSP_NewLogicFs
	str	w0, [x20, 1520]
	mov	w1, w0
	tbnz	w0, #31, .L15
	ldr	w0, [x21, 1272]
	bl	FSP_GetLogicFs
	mov	x19, x0
	cbz	x0, .L16
	ldr	x1, .LC8
	mov	w0, 18
	ldr	w2, [x20, 1520]
	bl	dprint_vfmw
	ldr	x0, [x19, 640]
	cbz	x0, .L14
	ldr	x1, [x19, 648]
	cbz	x1, .L14
	ldr	x2, [x19, 656]
	cbz	x2, .L14
	ldr	w3, [x1, 16]
	ldr	w4, [x2, 16]
	ldr	x1, .LC10
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
	ldr	x1, .LC4
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 1516]
	mov	w2, 1
	ldr	w0, [x21, 1272]
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 1512]
	mov	w2, 1
	ldr	w0, [x21, 1272]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 1512]
	str	wzr, [x20, 1516]
	ldr	w0, [x21, 1272]
	bl	FSP_GetNullLogicFs
	str	w0, [x20, 1512]
	str	w0, [x20, 1516]
	add	x1, x21, 664
	strb	wzr, [x19, 25]
	ldr	w0, [x21, 1272]
	bl	FSP_ClearNotInVoQueue
	ldr	x21, [sp, 32]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L16:
	ldr	x1, .LC6
	mov	w2, 516
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L10
	.size	Real9_GetImageBuffer, .-Real9_GetImageBuffer
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_ArrangeVHBMem
	.type	Real9_ArrangeVHBMem, %function
Real9_ArrangeVHBMem:
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
	ldp	w3, w2, [x19, 20]
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
	ldp	w22, w25, [x19, 12]
	mov	w24, 5
	mov	w21, 2
	mov	w23, 4
.L28:
	ldr	x0, .LC15
	mov	w1, 0
	mov	x2, 32
	ldr	x3, [x0, 96]
	add	x0, x29, 96
	blr	x3
	strb	wzr, [x29, 97]
	ldr	w1, [x19, 12]
	ldr	x0, [x20]
	str	w1, [x29, 124]
	ldr	w1, [x19, 16]
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
	ldr	w0, [x19, 1272]
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
	ldp	w3, w2, [x19, 12]
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
	ldr	x1, .LC12
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
	ldr	w0, [x19, 1272]
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
	ldp	w3, w2, [x19, 12]
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
	ldr	x1, .LC14
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L43
	.p2align 3
.L49:
	add	x0, x19, 664
	bl	ResetVoQueue
.L36:
	ldr	x1, .LC17
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
	ldr	x1, .LC14
	mov	w4, w25
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L43
	.p2align 3
.L50:
	ldr	w5, [x1, 672]
	b	.L25
	.size	Real9_ArrangeVHBMem, .-Real9_ArrangeVHBMem
	.align	3
.LC12:
	.xword	.LC11
	.align	3
.LC14:
	.xword	.LC13
	.align	3
.LC15:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC17:
	.xword	.LC16
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_GetCPFMT
	.type	Real9_CB_GetCPFMT, %function
Real9_CB_GetCPFMT:
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
	str	w20, [x21, 16]
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
	str	w22, [x21, 12]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Real9_CB_GetCPFMT, .-Real9_CB_GetCPFMT
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_SetDimensions
	.type	Real9_CB_SetDimensions, %function
Real9_CB_SetDimensions:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldp	w2, w0, [x19, 12]
	cmp	w0, 128
	beq	.L73
	cmp	w0, 176
	beq	.L74
	cmp	w0, 352
	bne	.L62
	cmp	w2, 288
	beq	.L75
	.p2align 2
.L58:
	add	w1, w0, 15
	add	w2, w2, 15
	add	x19, x19, 64
	lsr	w1, w1, 4
	lsr	w2, w2, 4
	mov	w0, 6
	stp	w2, w1, [x19, 8]
	mul	w1, w1, w2
	sub	w1, w1, #1
	cmp	w1, 47
	bls	.L59
	cmp	w1, 98
	bls	.L68
	cmp	w1, 395
	bls	.L69
	cmp	w1, 1583
	bls	.L70
.L72:
	mov	w2, 6335
	mov	w0, 14
	cmp	w1, w2
	bls	.L76
.L59:
	str	w0, [x19, 16]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L74:
	cmp	w2, 144
	bne	.L58
	add	x19, x19, 64
	mov	w2, 11
	mov	w0, 9
	mov	w1, 1
	stp	w0, w2, [x19, 8]
.L61:
	ldr	x0, .LC18
	add	x0, x0, 256
	ldr	w0, [x0, w1, uxtw 2]
	str	w0, [x19, 16]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L73:
	cmp	w2, 96
	bne	.L58
	add	x19, x19, 64
	mov	w1, 6
	mov	w2, 8
	mov	w0, w1
	stp	w1, w2, [x19, 8]
	b	.L59
	.p2align 3
.L62:
	cmp	w0, 704
	bne	.L64
	cmp	w2, 576
	bne	.L58
	add	x19, x19, 64
	mov	w1, 44
	mov	w0, 36
	stp	w0, w1, [x19, 8]
.L70:
	mov	w1, 3
	b	.L61
	.p2align 3
.L75:
	add	x19, x19, 64
	mov	w1, 22
	mov	w0, 18
	stp	w0, w1, [x19, 8]
.L69:
	mov	w1, 2
	b	.L61
	.p2align 3
.L64:
	cmp	w0, 1408
	bne	.L58
	cmp	w2, 1152
	bne	.L58
	add	x19, x19, 64
	mov	w2, 88
	mov	w0, 72
	mov	w1, 6335
	stp	w0, w2, [x19, 8]
	b	.L72
	.p2align 3
.L76:
	mov	w1, 4
	b	.L61
.L68:
	mov	w1, 1
	b	.L61
	.size	Real9_CB_SetDimensions, .-Real9_CB_SetDimensions
	.align	3
.LC18:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_GetPicSize
	.type	Real9_CB_GetPicSize, %function
Real9_CB_GetPicSize:
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
	ldr	x0, .LC20
	mov	w3, 180
	ldr	w1, [x20, 20]
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
	ldr	w0, [x20, 52]
	str	w0, [x24]
	ldr	w0, [x20, 56]
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
	.size	Real9_CB_GetPicSize, .-Real9_CB_GetPicSize
	.align	3
.LC20:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_FindNextSliceStartCode
	.type	Real9_CB_FindNextSliceStartCode, %function
Real9_CB_FindNextSliceStartCode:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w22, 7184
	mov	w19, 0
	bl	_mcount
	movk	w22, 0x1d, lsl 16
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
	bgt	.L99
	b	.L97
	.p2align 3
.L101:
	bl	BsSkip
	cmp	w19, w21
	beq	.L97
.L99:
	mov	w1, 24
	mov	x0, x20
	add	w19, w19, 1
	bl	BsShow
	cmp	w0, w22
	mov	w1, 8
	mov	x0, x20
	bne	.L101
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L97:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Real9_CB_FindNextSliceStartCode, .-Real9_CB_FindNextSliceStartCode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_SearchBits_IgnoreOnly0
	.type	Real9_CB_SearchBits_IgnoreOnly0, %function
Real9_CB_SearchBits_IgnoreOnly0:
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
	beq	.L103
	tst	w22, w19
	bne	.L103
	ldr	x0, .LC23
	mov	w20, 0
	add	x0, x0, 344
	ldr	w25, [x0, w25, uxtw 2]
	b	.L104
	.p2align 3
.L105:
	tst	w22, w19
	bne	.L103
.L104:
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
	bhi	.L105
.L103:
	cmp	w23, w19
	ldr	x25, [sp, 64]
	cset	w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.size	Real9_CB_SearchBits_IgnoreOnly0, .-Real9_CB_SearchBits_IgnoreOnly0
	.align	3
.LC23:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_GetSSC
	.type	Real9_CB_GetSSC, %function
Real9_CB_GetSSC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 4]
	cmp	w0, 1
	beq	.L111
	mov	x0, x20
	bl	Real9_CB_FindNextSliceStartCode
	cmp	w0, wzr
	cset	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L111:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real9_CB_GetSSC, .-Real9_CB_GetSSC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_GetSliceHeader
	.type	Real9_CB_GetSliceHeader, %function
Real9_CB_GetSliceHeader:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x21, x5
	mov	x19, x0
	mov	x0, x30
	mov	w22, w1
	mov	x23, x2
	mov	x25, x3
	mov	x24, x4
	bl	_mcount
	ldr	w20, [x21, 4]
	cmp	w20, 1
	beq	.L150
	cbz	w22, .L151
	str	wzr, [x23]
	mov	w0, -1
	mov	w20, 1
	str	w0, [x24]
.L136:
	mov	w0, w20
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L150:
	add	x26, x21, 64
	cbnz	w22, .L114
	ldr	w0, [x26, 24]
	cmn	w0, #1
	beq	.L152
	mov	w1, w20
	mov	x0, x19
	bl	BsShow
	mov	w27, w0
	mov	w1, w20
	mov	x0, x19
	bl	BsSkip
	cbnz	w27, .L119
	mov	w1, 2
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 2
	mov	x0, x19
	bl	BsSkip
	ldr	w0, [x26, 20]
	cmp	w0, w20
	beq	.L117
.L119:
	mov	w20, 0
	ldr	x27, [sp, 80]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L151:
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	cmp	w20, 1
	bne	.L119
	ldr	w26, [x21, 80]
	mov	x0, x19
	mov	w1, w26
	bl	BsShow
	mov	w27, w0
	mov	w1, w26
	mov	x0, x19
	bl	BsSkip
	str	w27, [x23]
	ldr	w0, [x21, 80]
	cmp	w0, 11
	bls	.L132
	mov	w1, w20
	mov	x0, x19
	bl	BsShow
	mov	w22, w0
	mov	w1, w20
	mov	x0, x19
	bl	BsSkip
	cmp	w22, 1
	bne	.L119
.L132:
	mov	w1, 5
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 5
	mov	x0, x19
	bl	BsSkip
	str	w20, [x24]
	cmp	w20, 31
	bgt	.L119
	mov	w1, 2
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 2
	mov	x0, x19
	bl	BsSkip
	str	w20, [x21, 52]
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	cmp	w20, 1
	bne	.L119
	mov	w1, 2
	mov	x0, x19
	bl	BsShow
	mov	w21, w0
	mov	w1, 2
	mov	x0, x19
	bl	BsSkip
	str	w21, [x25]
	b	.L136
	.p2align 3
.L152:
	mov	w1, w20
	mov	x0, x19
	bl	BsShow
	mov	w22, w0
	mov	w1, w20
	mov	x0, x19
	bl	BsSkip
	cbnz	w22, .L119
	mov	w1, 2
	mov	x0, x19
	mov	w22, w20
	bl	BsShow
	mov	w20, w0
	mov	w1, 2
	mov	x0, x19
	bl	BsSkip
	str	w20, [x26, 20]
.L117:
	mov	w1, 5
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 5
	mov	x0, x19
	bl	BsSkip
	uxtb	w0, w20
	strb	w0, [x26, 2]
	cmp	w0, 31
	bhi	.L119
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	cbnz	w20, .L119
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	mov	w20, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	cbnz	w20, .L119
	mov	w1, 2
	mov	x0, x19
	bl	BsShow
	mov	w27, w0
	mov	w1, 2
	mov	x0, x19
	bl	BsSkip
	str	w27, [x21, 52]
	mov	w1, 1
	mov	x0, x19
	bl	BsShow
	mov	w27, w0
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	cmp	w27, wzr
	mov	w1, 13
	cset	w0, ne
	strb	w0, [x26, 1]
	mov	x0, x19
	cbz	w22, .L120
	bl	BsShow
	mov	w22, w0
	mov	w1, 13
	mov	x0, x19
	bl	BsSkip
	str	w22, [x26, 28]
	ldr	w0, [x26, 20]
	cmp	w0, 1
	bls	.L122
	cmp	w0, 2
	bne	.L140
	ldp	w1, w0, [x26, 44]
	stp	w1, w0, [x26, 52]
.L140:
	add	x2, x29, 104
	mov	x1, x26
	mov	x0, x19
	add	x3, x29, 108
	bl	Real9_CB_GetPicSize
	ldp	w1, w0, [x29, 104]
	stp	w1, w0, [x26, 44]
	sub	w2, w1, #48
	stp	w0, w1, [x21, 12]
	mov	w1, 8144
	cmp	w2, w1
	bhi	.L136
	sub	w0, w0, #48
	cmp	w0, w1
	bhi	.L136
	mov	x0, x21
	bl	Real9_CB_SetDimensions
.L128:
	ldr	w20, [x26, 16]
	mov	x0, x19
	mov	w1, w20
	bl	BsShow
	mov	w21, w0
	mov	w1, w20
	mov	x0, x19
	bl	BsSkip
	str	w21, [x26, 24]
.L114:
	ldr	w0, [x26, 24]
	mov	w20, 1
	str	w0, [x23]
	ldrb	w0, [x26, 2]
	str	w0, [x24]
	mov	w0, w20
	str	wzr, [x25]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L120:
	bl	BsShow
	mov	w21, w0
	mov	w1, 13
	mov	x0, x19
	bl	BsSkip
	ldr	w0, [x26, 28]
	cmp	w0, w21
	bne	.L119
	mov	x1, x26
	mov	x0, x19
	add	x3, x29, 108
	add	x2, x29, 104
	bl	Real9_CB_GetPicSize
	ldr	w1, [x26, 44]
	ldr	w0, [x29, 104]
	cmp	w1, w0
	bne	.L136
	ldr	w1, [x26, 48]
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bne	.L136
	b	.L128
.L122:
	stp	wzr, wzr, [x26, 52]
	b	.L140
	.size	Real9_CB_GetSliceHeader, .-Real9_CB_GetSliceHeader
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_CB_GetPictureHeader
	.type	Real9_CB_GetPictureHeader, %function
Real9_CB_GetPictureHeader:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 4]
	cmp	w0, 1
	beq	.L178
	ldr	x3, .LC26
	mov	x2, 128
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	mov	w1, 16
	mov	x0, x20
	bl	BsShow
	mov	w21, w0
	mov	w1, 16
	mov	x0, x20
	bl	BsSkip
	mov	w0, 21845
	cmp	w21, w0
	bne	.L173
	mov	w1, 16
	mov	x0, x20
	bl	BsShow
	mov	w22, w0
	mov	w1, 16
	mov	x0, x20
	bl	BsSkip
	cmp	w22, w21
	bne	.L173
	mov	x0, x20
	add	x1, x29, 60
	bl	Real9_CB_GetVLCBits
	cmp	w0, 30
	bls	.L173
	ldr	w0, [x29, 60]
	tbnz	x0, 0, .L173
	tbnz	x0, 1, .L164
	mov	w2, 176
	mov	w1, 144
	stp	w1, w2, [x19, 12]
.L165:
	ubfx	x1, x0, 2, 5
	ubfx	x0, x0, 7, 8
	str	w1, [x19, 48]
	add	x1, x29, 60
	str	w0, [x19, 36]
	mov	x0, x20
	bl	Real9_CB_GetVLCBits
	cmp	w0, 1
	beq	.L177
	cmp	w0, 3
	beq	.L179
	cmp	w0, 5
	bne	.L173
	ldr	w0, [x29, 60]
	cbnz	w0, .L173
	mov	w0, 2
.L177:
	str	w0, [x19, 8]
.L167:
	ldr	w0, [x19, 16]
	cbz	w0, .L180
.L169:
	sub	w0, w0, #48
	mov	w1, 8144
	cmp	w0, w1
	bhi	.L173
	ldr	w0, [x19, 12]
	sub	w0, w0, #48
	cmp	w0, w1
	bhi	.L173
	mov	x0, x19
	mov	w21, 1
	bl	Real9_CB_SetDimensions
	mov	w1, 2
	mov	x0, x20
	bl	BsShow
	mov	w22, w0
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
	str	w22, [x19, 52]
.L155:
	ldp	w2, w1, [x19, 12]
	mov	w0, w21
	add	w1, w1, 15
	add	w2, w2, 15
	lsr	w2, w2, 4
	lsr	w1, w1, 4
	stp	w2, w1, [x19, 20]
	mul	w1, w1, w2
	str	w1, [x19, 28]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L179:
	ldr	w0, [x29, 60]
	cmp	w0, 1
	beq	.L181
.L173:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, -2
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L178:
	add	x22, x19, 64
	mov	w0, -1
	mov	x5, x19
	add	x4, x29, 60
	add	x3, x29, 52
	add	x2, x29, 56
	str	w0, [x22, 24]
	mov	w1, 0
	mov	x0, x20
	mov	w21, -2
	bl	Real9_CB_GetSliceHeader
	cmp	w0, 1
	bne	.L155
	ldr	w0, [x22, 20]
	cmp	w0, 3
	bls	.L182
.L156:
	ldrb	w1, [x22, 1]
	str	w1, [x19, 44]
	mov	w1, 8144
	ldr	w0, [x19, 16]
	ldrb	w2, [x22, 2]
	str	w2, [x19, 48]
	sub	w0, w0, #48
	cmp	w0, w1
	ldr	w0, [x22, 28]
	str	w0, [x19, 36]
	ldr	w0, [x22, 32]
	str	w0, [x19, 56]
	ldr	w0, [x22, 36]
	str	w0, [x19, 60]
	bhi	.L173
	ldr	w0, [x19, 12]
	sub	w0, w0, #48
	cmp	w0, w1
	bhi	.L173
	mov	x0, x19
	bl	Real9_CB_SetDimensions
	b	.L155
	.p2align 3
.L164:
	stp	wzr, wzr, [x19, 12]
	b	.L165
	.p2align 3
.L180:
	mov	x1, x19
	mov	x0, x20
	bl	Real9_CB_GetCPFMT
	cbz	w0, .L173
	ldr	w0, [x19, 16]
	b	.L169
	.p2align 3
.L182:
	ldr	x1, .LC25
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx158
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx158:
	.section	.rodata
	.align	0
	.align	2
.L158:
	.byte	(.L157 - .Lrtx158) / 4
	.byte	(.L159 - .Lrtx158) / 4
	.byte	(.L160 - .Lrtx158) / 4
	.byte	(.L161 - .Lrtx158) / 4
	.text
	.p2align 3
.L181:
	str	wzr, [x19, 8]
	b	.L167
.L160:
	mov	w0, 1
	str	w0, [x19, 8]
	mov	w21, w0
	b	.L156
.L159:
	str	wzr, [x19, 8]
	mov	w21, 1
	str	wzr, [x22, 4]
	b	.L156
.L157:
	mov	w0, 1
	str	wzr, [x19, 8]
	mov	w21, w0
	str	w0, [x22, 4]
	b	.L156
.L161:
	mov	w0, 2
	mov	w21, 1
	str	w0, [x19, 8]
	b	.L156
	.size	Real9_CB_GetPictureHeader, .-Real9_CB_GetPictureHeader
	.align	3
.LC25:
	.xword	.L158
	.align	3
.LC26:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_SetImgFormat
	.type	Real9_SetImgFormat, %function
Real9_SetImgFormat:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, 52336
	add	x2, x20, 53248
	add	x0, x20, x0
	ldr	w1, [x0, 1520]
	ldr	w0, [x2, 1272]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L183
	add	x3, x20, 49152
	mov	w2, 7200
	mov	w5, 262144
	mov	w1, 0
	ldr	w4, [x3, 3360]
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
	ldr	w2, [x3, 3368]
	str	w2, [x0, 112]
	ldr	w2, [x3, 3364]
	str	w2, [x0, 116]
	bl	SetAspectRatio
	ldr	x0, [x20]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L191
.L186:
	mov	w1, 1
	strb	w1, [x19, 371]
	strb	w1, [x19, 369]
	add	x1, x19, 256
	strb	wzr, [x19, 364]
	mov	w0, 8
	mov	w2, 9
	stp	w0, w0, [x1, 128]
	stp	w2, w0, [x1, 120]
.L183:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L191:
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
	b	.L186
	.size	Real9_SetImgFormat, .-Real9_SetImgFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	RV9_RecordVFMWState
	.type	RV9_RecordVFMWState, %function
RV9_RecordVFMWState:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	RV9_RecordVFMWState, .-RV9_RecordVFMWState
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_WriteSliceMsg
	.type	Real9_WriteSliceMsg, %function
Real9_WriteSliceMsg:
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
	ldr	w0, [x1, 3508]
	add	x2, x19, x2, lsl 2
	str	w0, [x2, 96]
	ldr	w0, [x1, 3500]
	str	w0, [x2, 104]
	ldr	w0, [x1, 3504]
	str	w0, [x2, 112]
	ldr	w0, [x1, 3520]
	str	w0, [x2, 100]
	ldr	w0, [x1, 3512]
	str	w0, [x2, 108]
	ldr	w0, [x1, 3516]
	str	w0, [x2, 116]
	ldr	w0, [x1, 3488]
	str	w0, [x2, 120]
	ldr	w0, [x1, 3484]
	str	w0, [x2, 124]
	ldr	w0, [x1, 3480]
	str	w0, [x2, 128]
	ldr	w0, [x1, 3492]
	str	w0, [x2, 132]
	str	w20, [x19, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real9_WriteSliceMsg, .-Real9_WriteSliceMsg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_WritePicMsg
	.type	Real9_WritePicMsg, %function
Real9_WritePicMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 49152
	mov	x0, 52336
	add	x0, x19, x0
	add	x2, x2, 3352
	ldr	w3, [x2, 8]
	strb	w3, [x19, 17]
	cmp	w3, 2
	ldr	w3, [x2, 24]
	str	w3, [x19, 20]
	ldr	w3, [x2, 20]
	str	w3, [x19, 24]
	ldr	w3, [x0, 1436]
	str	w3, [x19, 28]
	ldr	w3, [x0, 1440]
	str	w3, [x19, 32]
	ldr	w2, [x2, 48]
	str	w2, [x19, 36]
	ldr	w3, [x0, 340]
	str	w3, [x19, 40]
	ldr	w3, [x0, 344]
	str	w3, [x19, 44]
	beq	.L194
	str	w2, [x0, 340]
.L194:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real9_WritePicMsg, .-Real9_WritePicMsg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_WriteReg
	.type	Real9_WriteReg, %function
Real9_WriteReg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	str	w20, [x19, 52]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Real9_WriteReg, .-Real9_WriteReg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Real9_ModifySliceMsg
	.type	Real9_ModifySliceMsg, %function
Real9_ModifySliceMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 16
	ldr	w1, [x0, 132]
	cbnz	w1, .L204
	ldp	w1, w4, [x0, 4]
	ldr	w5, [x0, 32]
	mul	w4, w1, w4
	cmp	w5, 2
	sub	w4, w4, #1
	bhi	.L226
.L203:
	cmp	w5, 1
	bls	.L208
	sub	w7, w5, #1
	mov	w3, 0
	b	.L209
	.p2align 3
.L210:
	add	w3, w3, 1
	lsl	x1, x1, 2
	sub	x1, x1, x2
	sxtw	x6, w3
	lsl	x2, x6, 2
	add	x1, x0, x1, lsl 2
	sub	x2, x2, x6
	lsl	x2, x2, 2
	sub	x2, x2, x6
	ldr	w6, [x1, 132]
	add	x1, x0, x2, lsl 2
	ldr	w1, [x1, 132]
	cmp	w6, w1
	bge	.L204
.L209:
	sxtw	x2, w3
	cmp	w3, w7
	lsl	x1, x2, 2
	sub	x1, x1, x2
	bne	.L210
	uxtw	x3, w3
	lsl	x1, x3, 2
	sub	x1, x1, x3
	lsl	x1, x1, 2
	sub	x3, x1, x3
	add	x3, x0, x3, lsl 2
	ldr	w1, [x3, 132]
	cmp	w4, w1
	ccmp	w1, wzr, 4, ge
	ble	.L204
.L208:
	cbz	w5, .L211
	sub	w9, w5, #1
	mov	w8, 0
	mov	x10, 44
	b	.L214
	.p2align 3
.L229:
	beq	.L227
.L214:
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
	beq	.L228
	sub	x3, x3, x6
	add	x2, x0, x2, lsl 2
	add	x3, x0, x3, lsl 2
	ldr	w3, [x3, 132]
	sub	w3, w3, #1
	str	w3, [x2, 136]
.L213:
	lsl	x1, x7, 2
	add	w8, w8, 1
	sub	x1, x1, x7
	lsl	x1, x1, 2
	sub	x1, x1, x7
	add	x1, x0, x1, lsl 2
	ldr	w2, [x1, 132]
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
	cbz	w1, .L229
	.p2align 2
.L204:
	mov	w0, 0
.L224:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L226:
	ldr	w1, [x0, 176]
	sub	w6, w5, #1
	mov	w2, 1
	cmp	w4, w1
	ccmp	w1, wzr, 4, gt
	bgt	.L206
	b	.L204
	.p2align 3
.L207:
	sub	x1, x1, x3
	lsl	x1, x1, 2
	sub	x1, x1, x3
	add	x1, x0, x1, lsl 2
	ldr	w1, [x1, 132]
	cmp	w1, wzr
	ccmp	w4, w1, 4, gt
	ble	.L204
.L206:
	add	w2, w2, 1
	cmp	w2, w6
	sxtw	x3, w2
	lsl	x1, x3, 2
	bne	.L207
	b	.L203
.L227:
	ldr	w3, [x0, 96]
	mov	w6, 0
	mov	x4, 0
	mov	w2, w3
	b	.L217
	.p2align 3
.L230:
	ldr	w2, [x2, 96]
.L217:
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
	ldr	w7, [x1, 100]
	add	x2, x0, x2, lsl 2
	cmp	w7, w3
	bcs	.L215
	ldr	w1, [x1, 116]
	cmp	w1, wzr
	csel	w3, w3, w7, eq
.L215:
	cmp	w6, w5
	bne	.L230
	and	w3, w3, -16
	mov	w6, 0
	str	w3, [x0, 40]
	b	.L221
	.p2align 3
.L231:
	ldr	w2, [x1, 104]
	ldr	w7, [x1, 96]
	mov	x4, x2
	add	w2, w7, w2, lsr 3
	and	w4, w4, 7
	sub	w3, w2, w3
	ubfiz	w2, w3, 3, 4
	and	w3, w3, -16
	add	w2, w2, w4
	str	w3, [x1, 96]
	str	w2, [x1, 104]
.L219:
	add	w6, w6, 1
	cmp	w5, w6
	bls	.L220
	ldr	w3, [x0, 40]
.L221:
	sxtw	x2, w6
	lsl	x1, x2, 2
	sub	x1, x1, x2
	lsl	x1, x1, 2
	sub	x1, x1, x2
	add	x1, x0, x1, lsl 2
	ldr	w2, [x1, 100]
	cbz	w2, .L231
	ldp	w4, w2, [x1, 104]
	ldp	w10, w9, [x1, 96]
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
	stp	w4, w3, [x1, 96]
	stp	w7, w2, [x1, 104]
	b	.L219
.L211:
	ldr	w1, [x0, 96]
	and	w1, w1, -16
	str	w1, [x0, 40]
.L220:
	mov	w0, 1
	b	.L224
.L228:
	madd	x1, x7, x10, x0
	mov	w3, w4
	str	w4, [x1, 136]
	b	.L213
	.size	Real9_ModifySliceMsg, .-Real9_ModifySliceMsg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_Init
	.type	REAL9DEC_Init, %function
REAL9DEC_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	add	x20, x19, 53248
	bl	_mcount
	ldr	x3, .LC27
	mov	x2, 54536
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	add	x0, x20, 664
	bl	ResetVoQueue
	str	x21, [x19]
	add	x1, x19, 8192
	mov	w5, 15
	add	x1, x1, 3184
	add	x2, x19, 49152
	add	x0, x1, 40960
	mov	w3, 1
	mov	w6, 8192
	str	w5, [x0, 340]
	str	w5, [x0, 344]
	mov	x0, x19
	ldr	w5, [x21, 8]
	strb	w5, [x1, 24]
	strb	wzr, [x1, 28]
	strb	w3, [x2, 3352]
	str	w3, [x2, 3356]
	str	w6, [x2, 3384]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x20, 1272]
	beq	.L235
	mov	w0, 0
.L233:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L235:
	ldr	x1, .LC29
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L233
	.size	REAL9DEC_Init, .-REAL9DEC_Init
	.align	3
.LC27:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC29:
	.xword	.LC28
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_Destroy
	.type	REAL9DEC_Destroy, %function
REAL9DEC_Destroy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, 52336
	ldr	x1, .LC31
	add	x2, x19, x0
	mov	w0, 2
	ldr	w2, [x2, 1412]
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	REAL9DEC_Destroy, .-REAL9DEC_Destroy
	.align	3
.LC31:
	.xword	.LC30
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_Decode
	.type	REAL9DEC_Decode, %function
REAL9DEC_Decode:
	stp	x29, x30, [sp, -304]!
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
	add	x0, x29, 288
	add	x5, x29, 256
	add	x6, x29, 256
	ldr	w3, [x23, 3460]
	stp	xzr, xzr, [x29, 176]
	stp	xzr, xzr, [x0]
	add	x0, x29, 272
	ldr	w2, [x23, 3464]
	stp	xzr, xzr, [x29, 208]
	mov	w4, 8192
	str	w3, [x23, 3468]
	stp	xzr, xzr, [x0]
	stp	xzr, xzr, [x29, 192]
	mov	w0, 1
	str	w4, [x23, 3384]
	stp	xzr, xzr, [x29, 224]
	stp	xzr, xzr, [x29, 240]
	stp	xzr, xzr, [x5]
	str	w0, [x23, 3356]
	add	x1, x20, 16
	str	w2, [x23, 3472]
	str	x1, [x29, 152]
	str	w0, [x29, 180]
	str	w4, [x29, 208]
	stp	w3, w2, [x6, 28]
	stp	w3, w2, [x6, 36]
	cbz	x19, .L382
	ldr	x25, .LC36
	mov	w1, 0
	mov	x2, 11360
	add	x0, x20, 16
	add	x22, x20, 8192
	ldr	x3, [x25, 96]
	blr	x3
	add	x1, x22, 3184
	add	x0, x1, 40960
	str	wzr, [x0, 1376]
	ldr	w2, [x19, 16]
	str	w2, [x0, 1396]
	ldr	w2, [x19, 12]
	str	w2, [x0, 1380]
	ldr	x2, [x19]
	str	x2, [x20, 11376]
	ldr	w2, [x19, 16]
	str	w2, [x0, 1384]
	ldrb	w3, [x19, 9]
	strb	w3, [x1, 8]
	ldrb	w3, [x19, 9]
	cmp	w3, 1
	beq	.L240
	ldr	w3, [x19, 92]
	str	w3, [x0, 1388]
	ldr	x3, [x19, 80]
	str	x3, [x1, 16]
	ldr	w3, [x19, 96]
	str	w3, [x0, 1392]
	ldrb	w0, [x19, 89]
	strb	w0, [x1, 9]
	ldrb	w0, [x19, 89]
	cmp	w0, 1
	bne	.L383
	mov	w0, 65525
	sub	w1, w2, #10
	movk	w0, 0x2f, lsl 16
	cmp	w1, w0
	bhi	.L243
.L242:
	ldr	x1, [x20, 11376]
	add	x21, x22, 3184
	cbz	x1, .L243
	add	x0, x20, 53248
	str	x0, [x29, 144]
	add	x19, x0, 624
	mov	x0, x19
	bl	BsInit
	mov	w1, 8
	mov	x0, x19
	bl	BsShow
	mov	w27, w0
	mov	w1, 8
	mov	x0, x19
	bl	BsSkip
	add	w0, w27, 1
	cmp	w27, 255
	add	x27, x21, 40960
	mov	w2, w0
	str	w0, [x27, 348]
	bhi	.L243
	cbz	w0, .L244
	mov	w26, 0
	mov	w28, 3145727
	b	.L245
	.p2align 3
.L384:
	ldr	w2, [x27, 348]
	cmp	w2, w26
	bls	.L244
.L245:
	mov	w1, 32
	mov	x0, x19
	bl	BsSkip
	mov	w1, 32
	mov	x0, x19
	bl	BsShow
	mov	w24, w0
	mov	w1, 32
	mov	x0, x19
	bl	BsSkip
	uxtw	x1, w26
	rev	w0, w24
	add	x1, x1, 8192
	cmp	w0, w28
	add	x1, x1, 2136
	add	w26, w26, 1
	str	w0, [x21, x1, lsl 2]
	ble	.L384
	.p2align 2
.L243:
	mov	w21, 0
.L369:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 304
	ret
	.p2align 3
.L383:
	add	x20, x20, 53248
	ldr	w1, [x19, 20]
	mov	w21, 0
	ldr	w0, [x20, 1272]
	bl	SM_ReleaseStreamSeg
	ldr	w1, [x19, 100]
	ldr	w0, [x20, 1272]
	bl	SM_ReleaseStreamSeg
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 304
	ret
	.p2align 3
.L240:
	str	wzr, [x0, 1388]
	str	xzr, [x1, 16]
	str	wzr, [x0, 1392]
	mov	w0, 65525
	strb	wzr, [x1, 9]
	movk	w0, 0x2f, lsl 16
	sub	w1, w2, #10
	cmp	w1, w0
	bls	.L242
	b	.L243
	.p2align 3
.L382:
	mov	x2, 52336
	ldr	x1, .LC35
	add	x20, x20, x2
	mov	w21, 0
	ldr	w2, [x20, 1412]
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 304
	ret
	.p2align 3
.L244:
	add	x3, x23, 3536
	sub	w5, w2, #1
	mov	w1, 0
	b	.L246
	.p2align 3
.L247:
	add	x0, x0, 8192
	ldr	w4, [x3], 4
	add	x0, x22, x0, lsl 2
	ldr	w0, [x0, 11728]
	cmp	w0, w4
	ble	.L243
.L246:
	cmp	w1, w5
	add	w0, w1, 1
	mov	x1, x0
	bne	.L247
	add	x28, x22, 3184
	lsl	w2, w2, 3
	add	x0, x28, 40960
	add	w1, w2, 1
	mov	w4, w1
	mov	w2, 65534
	movk	w2, 0x2f, lsl 16
	mov	w1, 3145727
	ldr	w27, [x0, 352]
	str	w4, [x29, 140]
	add	w27, w4, w27
	sub	w3, w27, #1
	cmp	w3, w2
	ccmp	w4, w1, 0, ls
	bgt	.L243
	ldr	w2, [x0, 1384]
	sub	w2, w2, w27
	cmp	w2, wzr
	ble	.L243
	ldr	x1, [x20, 11376]
	mov	x0, x19
	add	x1, x1, x27, sxtw
	bl	BsInit
	add	x1, x29, 176
	mov	x0, x19
	bl	Real9_CB_GetPictureHeader
	cmp	w0, 1
	mov	w21, w0
	bne	.L385
	ldrb	w0, [x28, 28]
	cbz	w0, .L249
	ldr	w1, [x23, 3376]
	ldr	w2, [x29, 200]
	cmp	w2, w1
	beq	.L386
.L249:
	ldr	x0, .LC39
	ldr	x4, [x0]
	cbz	x4, .L251
	ldr	w0, [x23, 3372]
	mov	w3, 8
	strh	w0, [x29, 170]
	add	x2, x29, 168
	ldp	w0, w1, [x29, 196]
	strh	w0, [x29, 174]
	ldr	x0, [x29, 144]
	strh	w1, [x29, 172]
	ldr	w1, [x23, 3376]
	ldr	w0, [x0, 1272]
	strh	w1, [x29, 168]
	mov	w1, 2
	blr	x4
.L251:
	ldr	x24, [x29, 144]
	ldr	w0, [x24, 1272]
	bl	FSP_GetFspType
	cbnz	w0, .L252
	add	x28, x29, 304
	ldr	w0, [x24, 1272]
	add	x2, x29, 164
	add	x1, x29, 160
	stp	wzr, wzr, [x29, 160]
	str	wzr, [x28, -136]!
	mov	x3, x28
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x20]
	ldr	w2, [x0, 708]
	cmp	w2, 1
	beq	.L387
.L252:
	add	x24, x22, 3184
	ldr	x26, [x29, 144]
	add	x28, x24, 40960
	mov	w2, 1
	add	x1, x26, 664
	str	x1, [x29, 128]
	ldr	w0, [x26, 1272]
	ldr	w1, [x28, 1516]
	bl	FSP_ClearLogicFs
	ldr	w1, [x28, 1512]
	mov	w2, 1
	ldr	w0, [x26, 1272]
	bl	FSP_ClearLogicFs
	str	wzr, [x28, 1512]
	str	wzr, [x28, 1516]
	ldr	w0, [x26, 1272]
	bl	FSP_GetNullLogicFs
	str	w0, [x28, 1512]
	str	w0, [x28, 1516]
	strb	wzr, [x24, 25]
	ldr	x1, [x29, 128]
	ldr	w0, [x26, 1272]
	bl	FSP_ClearNotInVoQueue
	ldr	w2, [x29, 200]
	cmp	w2, 512
	bhi	.L388
	ldr	w0, [x29, 196]
	cmp	w0, 512
	mov	w3, w0
	bhi	.L259
	ldr	x1, [x20]
	ldr	w3, [x1, 708]
	cbnz	w3, .L261
	ldr	w1, [x1, 28]
	cmp	w1, 25
	beq	.L389
.L261:
	ldr	x0, [x29, 144]
	ldr	w0, [x0, 1272]
	bl	FSP_GetFspType
	cbz	w0, .L370
.L373:
	ldp	w0, w2, [x29, 196]
.L263:
	ldr	x1, .LC45
	mov	w3, w0
	mov	w0, 1
	bl	dprint_vfmw
	add	x2, x29, 176
	mov	x1, x20
	mov	w0, 6
	bl	Real9_ArrangeVHBMem
	cbz	w0, .L390
.L250:
	ldr	x3, [x25, 104]
	add	x0, x23, 3352
	add	x1, x29, 176
	mov	x2, 128
	str	x0, [x29, 120]
	add	x25, x22, 3184
	blr	x3
	ldr	x0, [x20]
	ldr	w0, [x0, 8]
	strb	w0, [x22, 3208]
	mov	x0, x20
	bl	Real9_GetImageBuffer
	cbz	w0, .L391
	ldr	x0, [x29, 144]
	add	x28, x25, 40960
	ldr	w0, [x0, 1272]
	ldr	w1, [x28, 1520]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L377
	ldr	x0, [x20]
	mov	x4, -1
	ldr	x1, .LC51
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
	ldr	x0, [x29, 120]
	ldr	w0, [x0, 8]
	cmp	w0, 2
	beq	.L379
	cmp	w0, 1
	ldrb	w0, [x25, 25]
	beq	.L392
	cmp	w0, wzr
	mov	w0, 2
	csinc	w0, w0, wzr, ne
.L270:
	add	x25, x22, 3184
	strb	w0, [x22, 3209]
	add	x25, x25, 40960
	ldr	w2, [x25, 1512]
	ldr	w0, [x25, 1516]
	cmp	w2, w0
	beq	.L271
	ldr	x1, .LC53
	mov	w0, 13
	bl	dprint_vfmw
	ldr	x0, [x29, 144]
	mov	w2, 0
	ldr	w1, [x25, 1512]
	ldr	w0, [x0, 1272]
	bl	FSP_SetRef
.L271:
	add	x28, x22, 3184
	ldr	x24, [x29, 144]
	add	x25, x28, 40960
	mov	w2, 1
	ldr	w0, [x25, 1516]
	ldr	w1, [x25, 1520]
	str	w1, [x25, 1516]
	str	w0, [x25, 1512]
	ldr	w0, [x24, 1272]
	bl	FSP_SetRef
	ldr	w0, [x24, 1272]
	ldr	w1, [x25, 1512]
	bl	FSP_RemovePmv
	ldr	x1, .LC55
	mov	w0, 13
	ldr	w2, [x25, 1520]
	bl	dprint_vfmw
	ldrb	w0, [x22, 3208]
	cmp	w0, 4
	bne	.L273
	ldr	w0, [x23, 3360]
	cbnz	w0, .L275
	str	wzr, [x25, 1528]
.L273:
	add	x28, x22, 3184
	ldr	x24, [x29, 144]
	add	x28, x28, 40960
	ldr	w0, [x24, 1272]
	ldr	w1, [x28, 1520]
	bl	FSP_GetLogicFs
	mov	x25, x0
	ldr	w1, [x28, 1512]
	ldr	w0, [x24, 1272]
	bl	FSP_GetLogicFs
	mov	x9, x0
	ldr	w1, [x28, 1516]
	ldr	w0, [x24, 1272]
	str	x9, [x29, 128]
	bl	FSP_GetLogicFs
	mov	x28, x0
	ldr	x9, [x29, 128]
	cmp	x25, xzr
	ccmp	x9, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L393
	ldr	x0, [x25, 640]
	cbz	x0, .L279
	ldr	x1, [x25, 656]
	cbz	x1, .L279
	ldr	x1, [x25, 648]
	cbz	x1, .L279
	ldr	x1, [x28, 640]
	cbz	x1, .L281
	ldr	w5, [x1, 16]
.L282:
	ldr	x0, [x29, 152]
	str	w5, [x0, 44]
	ldr	x0, [x9, 640]
	cbz	x0, .L394
.L374:
	ldr	x24, [x29, 152]
	ldr	w4, [x0, 16]
	ldr	x1, .LC62
	str	w4, [x24, 48]
	str	x9, [x29, 128]
	ldr	x0, [x25, 640]
	ldr	w2, [x0, 16]
	mov	w0, 13
	str	w2, [x24, 52]
	ldr	w6, [x25, 664]
	str	w6, [x24, 68]
	ldr	w7, [x28, 664]
	str	w7, [x24, 72]
	ldr	x3, [x25, 648]
	ldr	w3, [x3, 16]
	str	w3, [x24, 56]
	bl	dprint_vfmw
	ldr	x0, [x25, 648]
	ldr	x9, [x29, 128]
	ldr	w0, [x0, 32]
	str	w0, [x24, 60]
	add	x0, x22, 3184
	add	x0, x0, 40960
	ldr	x1, [x25, 648]
	ldr	w1, [x1, 32]
	str	w1, [x0, 1484]
	ldr	x0, [x25, 648]
	ldr	w0, [x0, 76]
	str	w0, [x24, 84]
	ldr	x0, [x25, 648]
	ldr	w0, [x0, 60]
	str	w0, [x24, 88]
	ldr	x0, [x25, 648]
	ldr	w0, [x0, 64]
	str	w0, [x24, 92]
	ldr	w0, [x23, 3360]
	cmp	w0, 2
	beq	.L395
.L285:
	ldr	w0, [x23, 3388]
	str	w0, [x25, 672]
	add	x0, x22, 3184
	add	x0, x0, 40960
	ldr	w3, [x23, 3400]
	ldr	w2, [x23, 3360]
	mov	w4, w3
	ldr	x3, [x29, 152]
	cmp	w2, 2
	str	w4, [x29, 116]
	strb	w2, [x3, 1]
	ldr	w2, [x23, 3376]
	str	w2, [x3, 4]
	ldr	w1, [x23, 3372]
	str	w1, [x3, 8]
	ldr	w1, [x0, 1436]
	str	w1, [x3, 12]
	ldr	w1, [x0, 1440]
	str	w1, [x3, 16]
	str	w4, [x3, 20]
	ldr	w1, [x0, 340]
	str	w1, [x3, 24]
	ldr	w1, [x0, 344]
	str	w1, [x3, 28]
	beq	.L290
	str	w4, [x0, 340]
.L290:
	mov	x0, x19
	add	x28, x23, 3480
	bl	BsPos
	asr	w1, w0, 31
	lsr	w1, w1, 29
	add	w0, w0, w1
	and	w0, w0, 7
	sub	w0, w0, w1
	add	x1, x22, 3184
	add	x1, x1, 40960
	str	w0, [x23, 3500]
	ldr	w0, [x1, 348]
	cmp	w0, 1
	beq	.L396
	ldr	w0, [x1, 356]
	ldr	w1, [x1, 352]
.L375:
	sub	w1, w0, w1
	mov	x0, x19
	lsl	w25, w1, 3
	bl	BsPos
	sub	w0, w25, w0
	add	x25, x22, 3184
	str	w0, [x28, 24]
	add	x28, x25, 40960
	mov	x0, x19
	ldr	w1, [x28, 1380]
	add	w27, w27, w1
	bl	BsPos
	cmp	w0, wzr
	add	w2, w0, 7
	add	x1, x23, 3328
	ldr	w4, [x23, 3404]
	csel	w0, w2, w0, lt
	ldr	w5, [x29, 116]
	mov	w7, 1
	str	w5, [x23, 3480]
	add	w0, w27, w0, asr 3
	str	w4, [x23, 3484]
	str	wzr, [x23, 3492]
	ldr	x3, [x29, 152]
	stp	w0, wzr, [x1, 180]
	stp	wzr, wzr, [x1, 188]
	ldr	w2, [x23, 3396]
	str	w2, [x23, 3488]
	str	w0, [x3, 96]
	ldr	w0, [x23, 3500]
	str	w0, [x3, 104]
	ldr	w0, [x23, 3504]
	stp	wzr, w0, [x3, 108]
	str	w7, [x3, 32]
	str	wzr, [x3, 100]
	stp	wzr, w2, [x3, 116]
	str	w4, [x3, 124]
	str	wzr, [x3, 132]
	str	w5, [x3, 128]
	ldr	w0, [x28, 348]
	cmp	w0, w7
	bls	.L310
	ldr	w0, [x29, 140]
	ldr	w6, [x28, 356]
	add	w6, w0, w6
	mov	w0, 3145727
	cmp	w6, w0
	bgt	.L298
	cmp	w6, wzr
	ble	.L298
	ldr	w2, [x28, 1384]
	sub	w2, w2, w6
	cmp	w2, wzr
	ble	.L298
	str	x25, [x29, 128]
	mov	w27, w7
	mov	x25, x28
	add	x28, x29, 168
	stp	w7, w0, [x29, 108]
	b	.L301
	.p2align 3
.L302:
	ldr	w0, [x25, 348]
	add	w27, w27, 1
	cmp	w0, w27
	bls	.L310
	uxtw	x0, w27
	ldr	x1, [x29, 128]
	add	x0, x0, 8192
	add	x0, x0, 2136
	ldr	w6, [x1, x0, lsl 2]
	ldr	w0, [x29, 140]
	add	w6, w0, w6
	ldr	w0, [x29, 112]
	cmp	w6, w0
	bgt	.L298
	cmp	w6, wzr
	ble	.L298
	ldr	w2, [x25, 1384]
	sub	w2, w2, w6
	cmp	w2, wzr
	ble	.L298
.L301:
	ldr	x0, [x29, 128]
	str	w6, [x29, 136]
	ldr	x1, [x0]
	mov	x0, x19
	add	x1, x1, x6, sxtw
	bl	BsInit
	ldr	x5, [x29, 120]
	add	x2, x29, 160
	mov	x4, x28
	add	x3, x29, 164
	mov	w1, 0
	mov	x0, x19
	bl	Real9_CB_GetSliceHeader
	cmp	w0, 1
	mov	w2, w0
	ldr	w6, [x29, 136]
	bne	.L302
	ldr	w0, [x23, 3376]
	ldr	w24, [x29, 160]
	cbz	w0, .L397
	ldr	w0, [x29, 168]
	add	x26, x23, 3480
	ldr	w1, [x29, 116]
	cmp	w0, wzr
	str	w6, [x29, 136]
	csel	w0, w1, w0, lt
	str	w0, [x29, 116]
	mov	x0, x19
	bl	BsPos
	asr	w1, w0, 31
	lsr	w1, w1, 29
	ldr	w6, [x29, 136]
	add	w0, w0, w1
	and	w0, w0, 7
	sub	w0, w0, w1
	str	w0, [x23, 3500]
	ldr	w0, [x25, 348]
	sub	w0, w0, #1
	cmp	w0, w27
	beq	.L398
	cmp	w27, 254
	bhi	.L399
	add	w0, w27, 1
	uxtw	x1, w27
	add	x0, x0, 8192
	add	x1, x1, 8192
	ldr	x2, [x29, 128]
	add	x0, x0, 2136
	add	x1, x1, 2136
	str	w6, [x29, 104]
	ldr	w0, [x2, x0, lsl 2]
	ldr	w1, [x2, x1, lsl 2]
.L376:
	sub	w1, w0, w1
	mov	x0, x19
	lsl	w1, w1, 3
	str	w1, [x29, 136]
	bl	BsPos
	ldr	w1, [x29, 136]
	ldr	w6, [x29, 104]
	sub	w0, w1, w0
	str	w0, [x26, 24]
	mov	x0, x19
	ldr	w2, [x25, 1380]
	add	w6, w6, w2
	str	w6, [x29, 136]
	bl	BsPos
	cmp	w0, wzr
	add	w3, w0, 7
	ldr	w6, [x29, 136]
	csel	w0, w3, w0, lt
	str	w24, [x23, 3492]
	add	w2, w6, w0, asr 3
	ldr	w0, [x29, 116]
	str	w0, [x23, 3480]
	add	x0, x23, 3328
	ldr	x6, [x29, 152]
	stp	w2, wzr, [x0, 180]
	stp	wzr, wzr, [x0, 188]
	ldr	w0, [x29, 108]
	sxtw	x3, w0
	add	w0, w0, 1
	mov	w5, w0
	cmp	w0, 131
	lsl	x0, x3, 2
	str	w5, [x29, 108]
	sub	x0, x0, x3
	lsl	x0, x0, 2
	sub	x0, x0, x3
	ldr	w3, [x23, 3404]
	str	w3, [x23, 3484]
	add	x0, x6, x0, lsl 2
	ldr	w3, [x23, 3396]
	str	w3, [x23, 3488]
	mov	x3, x6
	str	w2, [x0, 96]
	ldr	w2, [x23, 3500]
	str	w2, [x0, 104]
	ldr	w2, [x23, 3504]
	str	w2, [x0, 112]
	ldr	w2, [x23, 3520]
	str	w2, [x0, 100]
	ldr	w2, [x23, 3512]
	str	w2, [x0, 108]
	ldr	w2, [x23, 3516]
	str	w2, [x0, 116]
	ldr	w2, [x23, 3488]
	str	w2, [x0, 120]
	ldr	w2, [x23, 3484]
	str	w2, [x0, 124]
	ldr	w2, [x23, 3480]
	str	w2, [x0, 128]
	ldr	w1, [x23, 3492]
	str	w1, [x0, 132]
	str	w5, [x3, 32]
	ble	.L302
	mov	x0, x20
	bl	Real9_ModifySliceMsg
	cmp	w0, 1
	bne	.L400
	ldr	x0, [x29, 152]
	str	x0, [x20, 8]
	b	.L302
.L389:
	ldrb	w1, [x24, 28]
	cbnz	w1, .L263
.L370:
	ldr	x0, [x29, 128]
	bl	ResetVoQueue
	b	.L373
.L385:
	ldr	x1, .LC38
	mov	w0, 1
	mov	w21, 0
	bl	dprint_vfmw
	b	.L369
.L388:
	ldr	w3, [x29, 196]
.L259:
	ldr	x1, .LC43
	mov	w0, 18
	mov	w21, 0
	bl	dprint_vfmw
	b	.L369
.L391:
	add	x25, x25, 40960
	ldr	x1, .LC49
	mov	w21, 0
	ldr	w2, [x25, 1412]
	bl	dprint_vfmw
	b	.L369
.L386:
	ldr	w0, [x23, 3372]
	ldr	w1, [x29, 196]
	cmp	w1, w0
	bne	.L249
	b	.L250
.L387:
	ldr	w0, [x29, 168]
	cmp	w0, wzr
	bgt	.L255
	ldr	w0, [x29, 164]
	cmp	w0, 32
	bgt	.L255
	ldr	x0, [x20, 8]
	cbnz	x0, .L255
	ldr	x24, [x29, 144]
	ldr	w3, [x24, 496]
	cbz	w3, .L252
	ldr	w4, [x23, 3376]
	cbz	w4, .L252
	ldr	w1, [x24, 1276]
	ldr	w7, [x29, 196]
	cmp	w7, w1
	beq	.L252
	ldr	w1, [x24, 1280]
	ldr	w6, [x29, 200]
	cmp	w6, w1
	beq	.L252
	ldr	x1, .LC41
	mov	w0, w2
	ldr	w5, [x23, 3372]
	bl	dprint_vfmw
	ldr	x0, .LC39
	ldr	x4, [x0]
	ldr	w0, [x29, 196]
	str	w0, [x24, 1276]
	ldr	w0, [x29, 200]
	str	w0, [x24, 1280]
	cbz	x4, .L252
	ldr	w0, [x24, 1272]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L252
	.p2align 3
.L390:
	ldr	x1, .LC47
	mov	w0, 1
	mov	w21, 0
	bl	dprint_vfmw
	b	.L369
.L298:
	add	x1, x22, 3184
	ldr	x0, [x29, 144]
	add	x1, x1, 40960
	mov	w2, 1
	mov	w21, 0
	ldr	w0, [x0, 1272]
	ldr	w1, [x1, 1520]
	bl	FSP_ClearLogicFs
	b	.L369
.L379:
	ldrb	w0, [x25, 25]
	cmp	w0, 1
	bls	.L377
	ldrb	w0, [x25, 24]
	cmp	w0, 4
	bne	.L273
.L314:
	add	x0, x22, 3184
	add	x0, x0, 40960
	ldr	w1, [x0, 1528]
	cbnz	w1, .L273
	ldr	w1, [x0, 1520]
.L378:
	ldr	x0, [x29, 144]
	mov	w2, 1
	mov	w21, 0
	ldr	w0, [x0, 1272]
	bl	FSP_ClearLogicFs
	b	.L369
.L392:
	cbz	w0, .L243
	mov	w0, 2
	b	.L270
.L255:
	mov	w21, -2
	b	.L369
.L398:
	uxtw	x0, w27
	ldr	x1, [x29, 128]
	add	x0, x0, 8192
	ldr	w2, [x29, 140]
	add	x0, x0, 2136
	str	w6, [x29, 104]
	ldr	w1, [x1, x0, lsl 2]
	ldr	w0, [x25, 1384]
	sub	w0, w0, w2
	b	.L376
.L393:
	ldr	x2, .LC56
	mov	w3, 1997
	ldr	x1, .LC58
	mov	w0, 0
	mov	w21, 0
	bl	dprint_vfmw
	b	.L369
.L275:
	cmp	w0, 2
	beq	.L314
	cmp	w0, 1
	bne	.L273
	str	w0, [x25, 1528]
	b	.L273
.L281:
	ldr	w5, [x0, 16]
	b	.L282
.L279:
	add	x1, x22, 3184
	ldr	x0, [x29, 144]
	add	x1, x1, 40960
	mov	w2, 1
	mov	w21, 0
	ldr	w0, [x0, 1272]
	ldr	w1, [x1, 1520]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC60
	mov	w0, 0
	bl	dprint_vfmw
	b	.L369
.L310:
	ldr	x0, [x29, 152]
	mov	w19, 1
	str	w19, [x0, 36]
	ldr	x3, [x20, 8]
	cbz	x3, .L401
	ldr	x0, [x20]
	ldr	w1, [x0, 864]
	cmp	w1, 1
	beq	.L315
.L312:
	cmp	w1, 2
	beq	.L402
.L313:
	ldr	w1, [x0, 592]
	str	w1, [x20, 92]
	ldr	w0, [x0, 632]
	str	w0, [x20, 96]
	b	.L369
.L396:
	ldr	w0, [x1, 1384]
	ldr	w2, [x29, 140]
	ldr	w1, [x1, 352]
	sub	w0, w0, w2
	b	.L375
.L395:
	ldr	w3, [x9, 672]
	ldr	w1, [x23, 3388]
	ldr	w0, [x28, 672]
	subs	w1, w1, w3
	sub	w0, w0, w3
	bmi	.L403
.L286:
	tbnz	w0, #31, .L404
.L287:
	cmp	w1, w0
	csel	w1, w1, wzr, le
	cmp	w0, wzr
	ble	.L318
	sub	w3, w0, w1
	lsl	w4, w1, 14
	lsl	w3, w3, 14
	sdiv	w4, w4, w0
	sdiv	w3, w3, w0
.L289:
	add	x2, x22, 3184
	add	x2, x2, 40960
	add	x5, x2, 1280
	stp	w1, w0, [x5, 148]
	stp	w4, w3, [x5, 156]
	b	.L285
.L394:
	ldr	x0, [x25, 640]
	b	.L374
.L377:
	ldr	w1, [x28, 1520]
	b	.L378
.L397:
	ldr	x0, [x29, 128]
	mov	w21, 0
	add	x27, x0, 40960
	ldr	x0, [x29, 144]
	ldr	w1, [x27, 1520]
	ldr	w0, [x0, 1272]
	bl	FSP_ClearLogicFs
	b	.L369
.L403:
	ldr	w2, [x23, 3384]
	add	w1, w1, w2
	b	.L286
.L402:
	ldr	w1, [x0, 868]
	cmp	w1, wzr
	ble	.L313
	ldr	x1, [x20, 8]
	cbz	x1, .L313
	ldrb	w1, [x1, 1]
	cmp	w1, 2
	bne	.L313
	ldr	x0, [x29, 144]
	mov	w2, 0
	str	xzr, [x20, 8]
	mov	w21, 0
	ldr	w1, [x0, 608]
	ldr	w0, [x0, 1272]
	bl	FSP_SetDisplay
	ldr	x3, [x20]
	mov	x1, x20
	mov	w0, 9
	ldr	w2, [x3, 868]
	sub	w2, w2, #1
	str	w2, [x3, 868]
	bl	ReleasePacket
	b	.L369
.L400:
	ldr	w1, [x25, 1520]
	b	.L378
.L399:
	ldr	x1, .LC64
	mov	w2, 2229
	mov	w0, 22
	mov	w21, 0
	bl	dprint_vfmw
	b	.L369
.L401:
	mov	x0, x20
	bl	Real9_ModifySliceMsg
	cmp	w0, 1
	bne	.L405
	ldr	x0, [x20]
	ldr	x2, [x29, 152]
	str	x2, [x20, 8]
	ldr	w1, [x0, 864]
	cmp	w1, 1
	bne	.L312
	cbz	x2, .L313
	mov	x3, x2
.L315:
	ldrb	w1, [x3, 1]
	cmp	w1, 2
	bne	.L313
	ldr	x1, [x29, 144]
	mov	w2, 0
	str	xzr, [x20, 8]
	mov	w21, 0
	ldr	w0, [x1, 1272]
	ldr	w1, [x1, 608]
	bl	FSP_SetDisplay
	mov	x1, x20
	mov	w0, 9
	bl	ReleasePacket
	b	.L369
.L318:
	mov	w3, 0
	mov	w4, 0
	b	.L289
.L404:
	ldr	w2, [x23, 3384]
	add	w0, w0, w2
	b	.L287
.L405:
	add	x1, x22, 3184
	ldr	x0, [x29, 144]
	add	x1, x1, 40960
	mov	w2, w19
	mov	w21, 0
	ldr	w0, [x0, 1272]
	ldr	w1, [x1, 1520]
	bl	FSP_ClearLogicFs
	b	.L369
	.size	REAL9DEC_Decode, .-REAL9DEC_Decode
	.align	3
.LC35:
	.xword	.LC34
	.align	3
.LC36:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC38:
	.xword	.LC37
	.align	3
.LC39:
	.xword	g_event_report
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
.LC56:
	.xword	.LANCHOR1
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_RecycleImage
	.type	REAL9DEC_RecycleImage, %function
REAL9DEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L408
	add	x19, x19, 53248
	mov	w2, 0
	mov	w1, w20
	ldr	w0, [x19, 1272]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 1272]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L408:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	REAL9DEC_RecycleImage, .-REAL9DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_GetRemainImg
	.type	REAL9DEC_GetRemainImg, %function
REAL9DEC_GetRemainImg:
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
	beq	.L410
	mov	x21, 52336
	add	x20, x19, 53248
	add	x21, x19, x21
	ldr	w0, [x20, 1272]
	ldr	w1, [x21, 1516]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L410
	ldr	w0, [x20, 1272]
	ldr	w1, [x21, 1516]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L423
.L410:
	add	x22, x20, 664
.L414:
	mov	x0, x22
	bl	GetVoLastImageID
.L420:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L423:
	mov	x0, x19
	add	x22, x20, 664
	bl	Real9_SetImgFormat
	mov	w2, 1
	str	w2, [x23, 188]
	ldr	w0, [x20, 1272]
	ldr	w1, [x21, 1516]
	bl	FSP_SetDisplay
	ldr	w0, [x20, 1272]
	mov	x4, x23
	mov	x3, x22
	mov	x2, x19
	mov	w1, 9
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L411
	ldr	w0, [x20, 1272]
	mov	w2, 0
	ldr	w1, [x21, 1516]
	bl	FSP_SetDisplay
	b	.L414
	.p2align 3
.L411:
	ldr	w0, [x23, 160]
	cmp	w0, 100
	bhi	.L410
	ldr	w0, [x23, 112]
	cmp	w0, 31
	bls	.L410
	ldr	w1, [x23, 116]
	mov	w0, 0
	cmp	w1, 31
	bhi	.L420
	b	.L414
	.size	REAL9DEC_GetRemainImg, .-REAL9DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_VDMPostProc
	.type	REAL9DEC_VDMPostProc, %function
REAL9DEC_VDMPostProc:
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
	ldr	x1, .LC66
	mov	w2, w20
	mov	w0, 22
	add	x25, x19, 3184
	add	x22, x21, 53248
	add	x23, x25, 40960
	bl	dprint_vfmw
	mov	x1, x21
	mov	w0, 9
	bl	ReleasePacket
	ldr	w0, [x22, 1272]
	ldr	w1, [x23, 1520]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L433
	ldr	w1, [x23, 176]
	strb	w1, [x19, 3210]
	cmp	w1, 2
	beq	.L426
	and	w24, w24, 31
	str	w24, [x23, 344]
.L426:
	tbnz	w20, #31, .L434
	mov	w1, 100
	cmp	w20, w1
	csel	w20, w20, w1, le
.L427:
	ldr	x1, .LC68
	str	w20, [x0, 160]
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x21
	bl	Real9_SetImgFormat
	ldrb	w1, [x19, 3208]
	add	x0, x19, 3184
	cmp	w1, 2
	beq	.L430
	ldr	x1, [x21]
	ldr	w1, [x1, 12]
	cmp	w1, 1
	beq	.L430
	add	x1, x0, 40960
	ldr	w2, [x1, 176]
	cmp	w2, 2
	beq	.L430
	ldrb	w0, [x0, 25]
	cmp	w0, 1
	bls	.L435
	ldr	w1, [x1, 1512]
.L431:
	add	x0, x19, 3184
	cmn	w1, #1
	add	x0, x0, 40960
	str	w1, [x0, 1524]
	beq	.L432
.L452:
	add	x20, x19, 3184
	ldr	w0, [x22, 1272]
	add	x20, x20, 40960
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	ldr	w23, [x20, 1524]
	ldr	w0, [x22, 1272]
	mov	w1, w23
	bl	FSP_GetRef
	mov	w3, w0
	ldr	x1, .LC70
	mov	w2, w23
	mov	w0, 12
	bl	dprint_vfmw
	ldr	w1, [x20, 1524]
	mov	w2, 1
	ldr	w0, [x22, 1272]
	bl	FSP_SetDisplay
	ldr	w0, [x22, 1272]
	mov	x4, x24
	add	x3, x22, 664
	mov	x2, x21
	mov	w1, 9
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L451
.L432:
	add	x19, x19, 3184
	mov	w0, 1
	add	x19, x19, 40960
	ldr	w1, [x19, 1412]
	add	w1, w1, w0
	str	w1, [x19, 1412]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L430:
	add	x0, x19, 3184
	add	x0, x0, 40960
	ldr	w1, [x0, 1520]
	str	w1, [x0, 1524]
	cmn	w1, #1
	bne	.L452
	b	.L432
	.p2align 3
.L434:
	mov	w20, 0
	b	.L427
	.p2align 3
.L451:
	ldr	w1, [x20, 1524]
	mov	w2, 0
	ldr	w0, [x22, 1272]
	bl	FSP_SetDisplay
	ldr	x25, [sp, 64]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L433:
	mov	w0, 0
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L435:
	mov	w1, -1
	b	.L431
	.size	REAL9DEC_VDMPostProc, .-REAL9DEC_VDMPostProc
	.align	3
.LC66:
	.xword	.LC65
	.align	3
.LC68:
	.xword	.LC67
	.align	3
.LC70:
	.xword	.LC69
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_GetImageBuffer
	.type	REAL9DEC_GetImageBuffer, %function
REAL9DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 53248
	bl	_mcount
	ldr	w0, [x19, 1272]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	mov	w20, w0
	beq	.L454
	ldr	w0, [x19, 1272]
	mov	w20, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L459
.L454:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L459:
	ldr	w0, [x19, 1272]
	add	x1, x19, 664
	bl	FSP_ClearNotInVoQueue
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	REAL9DEC_GetImageBuffer, .-REAL9DEC_GetImageBuffer
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	REAL9DEC_Support
	.type	REAL9DEC_Support, %function
REAL9DEC_Support:
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
	.size	REAL9DEC_Support, .-REAL9DEC_Support
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
	.align	3
.LANCHOR1 = . + 0
	.type	__FUNCTION__.11815, %object
	.size	__FUNCTION__.11815, 16
__FUNCTION__.11815:
	.string	"REAL9DEC_Decode"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC3:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC5:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC7:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC9:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC11:
	ASCII(.string	"image size abnormal (%dx%d) in MB\n" )
	.zero	5
.LC13:
	ASCII(.string	"real9 actual frame size(%dx%d) exeed max config(%dx%d)\n" )
.LC16:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC28:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC30:
	ASCII(.string	"Decoding quits at frm %d\n" )
	.zero	6
.LC34:
	ASCII(.string	"ERROR: frm %d, Invalid packet pointer!\n" )
.LC37:
	ASCII(.string	"Real9_CB_GetPictureHeader Error!\n" )
	.zero	6
.LC40:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC42:
	ASCII(.string	"pic width/height to large,MbWidth = %d, MbHeight = %d" )
	.zero	2
.LC44:
	ASCII(.string	"new seq size %d * %d\n" )
	.zero	2
.LC46:
	ASCII(.string	"Real9_ArrangeVHBMem err\n" )
	.zero	7
.LC48:
	ASCII(.string	"Fatal Error: frm%d, get image buffer failed\n" )
	.zero	3
.LC50:
	ASCII(.string	"dec_pts: %lld Usertag = %lld\n" )
	.zero	2
.LC52:
	ASCII(.string	"---------- unref %d\n" )
	.zero	3
.LC54:
	ASCII(.string	"++++++++++ ref %d\n" )
	.zero	5
.LC57:
	ASCII(.string	"CurFsID abnormal!,%s %d (NULL == pCur) || (NULL == pFwd) || (NULL == pBwd)!!\n" )
	.zero	2
.LC59:
	ASCII(.string	"CurFsID abnormal!\n" )
	.zero	5
.LC61:
	ASCII(.string	"dec addr: cur,cur2d,fwd,bwd,currpmv,colpmv = %p,%p,%p,%p,%p,%p\n" )
.LC63:
	ASCII(.string	"line:%d! i > (RV9_MAX_SLC_PARAM_NUM - 2)\n" )
	.zero	6
.LC65:
	ASCII(.string	"ErrRatio = %d\n" )
	.zero	1
.LC67:
	ASCII(.string	"DEBUG: DEC over\n" )
	.zero	7
.LC69:
	ASCII(.string	"insert buf %d ref %d\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
