#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vp6.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	Vp6_Vfmw_DecodeBool
	.type	Vp6_Vfmw_DecodeBool, %function
Vp6_Vfmw_DecodeBool:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x1
	mov	x22, x0
	mov	x0, x30
	uxtb	w19, w2
	mov	w23, 0
	bl	_mcount
	ldp	w1, w20, [x21, 4]
	sub	w0, w1, #1
	mul	w19, w0, w19
	lsr	w19, w19, 8
	add	w19, w19, 1
	lsl	w0, w19, 16
	cmp	w20, w0
	bcc	.L2
	sub	w19, w1, w19
	sub	w20, w20, w0
	mov	w23, 1
.L2:
	cmp	w19, 127
	bhi	.L9
	ldr	w1, [x21, 12]
	b	.L6
	.p2align 3
.L5:
	cmp	w19, 127
	bhi	.L11
.L6:
	subs	w1, w1, #1
	lsl	w19, w19, 1
	lsl	w20, w20, 1
	bne	.L5
	mov	w1, 8
	mov	x0, x22
	bl	BsGet
	orr	w20, w20, w0
	cmp	w19, 127
	mov	w1, 8
	bls	.L6
.L11:
	str	w1, [x21, 12]
.L9:
	stp	w19, w20, [x21, 4]
	mov	w0, w23
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	Vp6_Vfmw_DecodeBool, .-Vp6_Vfmw_DecodeBool
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vp6_Vfmw_DecodeBool128
	.type	Vp6_Vfmw_DecodeBool128, %function
Vp6_Vfmw_DecodeBool128:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x1
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	ldp	w20, w1, [x19, 4]
	ldr	w2, [x19, 12]
	add	w0, w20, 1
	lsr	w0, w0, 1
	lsl	w21, w0, 16
	cmp	w1, w21
	bcc	.L13
	sub	w0, w20, w0
	sub	w1, w1, w21
	lsl	w20, w0, 1
	lsl	w21, w1, 1
	mov	w23, 1
.L14:
	subs	w2, w2, #1
	bne	.L15
	mov	w1, 8
	mov	x0, x22
	bl	BsGet
	orr	w21, w21, w0
	mov	w2, 8
.L15:
	stp	w20, w21, [x19, 4]
	str	w2, [x19, 12]
	mov	w0, w23
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L13:
	lsl	w20, w0, 1
	lsl	w21, w1, 1
	mov	w23, 0
	b	.L14
	.size	Vp6_Vfmw_DecodeBool128, .-Vp6_Vfmw_DecodeBool128
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vp6_Vfmw_BoolRead
	.type	Vp6_Vfmw_BoolRead, %function
Vp6_Vfmw_BoolRead:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w2
	mov	x22, x1
	bl	_mcount
	mov	w20, 0
	subs	w19, w19, #1
	bmi	.L17
	.p2align 2
.L18:
	mov	x1, x22
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w20, w0, w20
	cmn	w19, #1
	bne	.L18
.L17:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Vp6_Vfmw_BoolRead, .-Vp6_Vfmw_BoolRead
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vp6_Vfmw_StartDecode
	.type	Vp6_Vfmw_StartDecode, %function
Vp6_Vfmw_StartDecode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	mov	w1, 255
	mov	w0, 8
	stp	wzr, w1, [x19]
	str	w0, [x19, 12]
	mov	w1, 24
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 8]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Vp6_Vfmw_StartDecode, .-Vp6_Vfmw_StartDecode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_ArrangeVHBMem
	.type	VP6_ArrangeVHBMem, %function
VP6_ArrangeVHBMem:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 12288
	bl	_mcount
	add	x0, x20, 276
	ldp	w3, w2, [x0, 24]
	cmp	w3, 2
	bls	.L24
	sub	w1, w2, #3
	mov	w0, 512
	cmp	w1, 509
	ccmp	w3, w0, 2, ls
	bhi	.L24
	ldr	x1, [x19]
	ldr	w0, [x1, 748]
	cbnz	w0, .L27
	ldr	w0, [x1, 28]
	cmp	w0, 25
	beq	.L55
.L28:
	mov	w21, 3
	lsl	w23, w2, 4
	lsl	w22, w3, 4
	mov	w25, 5
	mov	w24, w21
.L33:
	ldr	x0, .LC4
	mov	x2, 32
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x29, 96
	blr	x3
	strb	wzr, [x29, 97]
	ldr	x0, [x19]
	stp	w23, w22, [x29, 120]
	stp	w23, w22, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 24
	beq	.L39
	strb	w24, [x29, 99]
	sxtb	w21, w21
	strb	w25, [x29, 101]
.L44:
	ldr	w0, [x19, 13360]
	add	x1, x29, 96
	strb	w21, [x29, 102]
	strb	wzr, [x29, 98]
	bl	FSP_ConfigInstance
	cbnz	w0, .L40
	ldr	x1, [x19]
	add	x3, x29, 92
	ldr	w0, [x19, 13360]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L56
	cbnz	w0, .L42
	mov	w0, 1
	str	w0, [x19, 12388]
	b	.L48
	.p2align 3
.L24:
	ldr	x1, .LC1
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
.L48:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L27:
	ldr	w0, [x1, 28]
	cmp	w0, 25
	bne	.L28
	ldr	w4, [x1, 708]
	lsl	w2, w2, 4
	cmp	w2, w4
	bhi	.L57
	ldr	w5, [x1, 712]
	lsl	w3, w3, 4
	cmp	w3, w5
	bhi	.L36
	ldr	w4, [x1, 736]
	mov	w23, w2
	ldr	w0, [x1, 732]
	mov	w22, w3
	mov	w21, 1
	add	w24, w0, 2
	cbz	w4, .L38
.L53:
	add	w21, w0, w21
	mov	w0, 17
	cmp	w21, w0
	csel	w21, w21, w0, le
.L38:
	ldrsb	w25, [x1, 756]
	b	.L33
	.p2align 3
.L40:
	ldr	x1, .LC8
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L48
	.p2align 3
.L39:
	mov	w0, 1
	strb	w25, [x29, 101]
	strb	w0, [x29, 99]
	mov	w21, 1
	b	.L44
	.p2align 3
.L55:
	ldr	w23, [x1, 708]
	lsl	w2, w2, 4
	cmp	w2, w23
	bhi	.L58
	ldr	w22, [x1, 712]
	lsl	w3, w3, 4
	cmp	w3, w22
	bhi	.L59
	ldr	w0, [x19, 12388]
	cmp	w0, 1
	beq	.L48
	ldr	w2, [x1, 736]
	mov	w21, 1
	ldr	w0, [x1, 732]
	add	w24, w0, 2
	cbnz	w2, .L53
	ldrsb	w25, [x1, 756]
	b	.L33
.L57:
	ldr	w5, [x1, 712]
	lsl	w3, w3, 4
.L36:
	ldr	x1, .LC3
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L48
.L56:
	add	x0, x20, 464
	bl	ResetVoQueue
.L42:
	ldr	x1, .LC6
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L48
.L59:
	mov	w5, w22
.L30:
	ldr	x1, .LC3
	mov	w4, w23
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L48
.L58:
	lsl	w3, w3, 4
	ldr	w5, [x1, 712]
	b	.L30
	.size	VP6_ArrangeVHBMem, .-VP6_ArrangeVHBMem
	.align	3
.LC1:
	.xword	.LC0
	.align	3
.LC3:
	.xword	.LC2
	.align	3
.LC4:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC6:
	.xword	.LC5
	.align	3
.LC8:
	.xword	.LC7
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_GetImageBuffer
	.type	VP6_GetImageBuffer, %function
VP6_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 312
	bl	_mcount
	ldr	w0, [x19, 13360]
	mov	w1, 1
	bl	FSP_NewLogicFs
	str	w0, [x20, 12088]
	mov	w1, w0
	tbnz	w0, #31, .L67
	ldr	w0, [x19, 13360]
	bl	FSP_GetLogicFs
	mov	x19, x0
	cbz	x0, .L68
	ldr	x1, .LC14
	mov	w0, 18
	ldr	w2, [x20, 12088]
	bl	dprint_vfmw
	ldr	x0, [x19, 640]
	cbz	x0, .L66
	ldr	x0, [x19, 648]
	cbz	x0, .L66
	ldr	x1, [x19, 656]
	cbz	x1, .L66
	ldr	w0, [x0, 32]
	str	w0, [x20, 12104]
	mov	w0, 18
	ldr	x1, .LC16
	ldr	x2, [x19, 640]
	ldr	x3, [x19, 648]
	ldr	x4, [x19, 656]
	ldr	w2, [x2, 16]
	ldr	w3, [x3, 16]
	ldr	w4, [x4, 16]
	bl	dprint_vfmw
	mov	w0, 1
.L62:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L66:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L67:
	ldr	x1, .LC10
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 13360]
	mov	w2, 1
	ldr	w1, [x20, 12092]
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 12096]
	mov	w2, 1
	ldr	w0, [x19, 13360]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 12092]
	str	wzr, [x20, 12096]
	ldr	w0, [x19, 13360]
	bl	FSP_GetNullLogicFs
	str	wzr, [x20, 12072]
	str	w0, [x20, 12092]
	add	x1, x19, 12288
	str	w0, [x20, 12096]
	add	x1, x1, 464
	ldr	w0, [x19, 13360]
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L68:
	ldr	x1, .LC12
	mov	w2, 640
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L62
	.size	VP6_GetImageBuffer, .-VP6_GetImageBuffer
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6DEC_GetImageBuffer
	.type	VP6DEC_GetImageBuffer, %function
VP6DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x20, 13360]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	mov	w19, w0
	beq	.L70
	ldr	w0, [x20, 13360]
	mov	w19, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L75
.L70:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L75:
	add	x1, x20, 12288
	ldr	w0, [x20, 13360]
	add	x1, x1, 464
	bl	FSP_ClearNotInVoQueue
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6DEC_GetImageBuffer, .-VP6DEC_GetImageBuffer
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_BuildScanOrder
	.type	VP6_BuildScanOrder, %function
VP6_BuildScanOrder:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	mov	w5, 1
	strb	wzr, [x19, 960]
	mov	w4, 0
	add	x6, x19, 1089
	.p2align 2
.L77:
	mov	x2, x6
	mov	w0, 1
	b	.L79
	.p2align 3
.L78:
	add	w0, w0, 1
	cmp	w0, 64
	beq	.L87
.L79:
	ldrb	w3, [x2]
	add	x2, x2, 1
	cmp	w3, w4
	bne	.L78
	add	x3, x19, x5, uxtw
	add	w5, w5, 1
	strb	w0, [x3, 960]
	add	w0, w0, 1
	cmp	w0, 64
	bne	.L79
.L87:
	add	w4, w4, 1
	cmp	w4, 16
	bne	.L77
	add	x8, x19, 64
	mov	x7, x19
	.p2align 2
.L80:
	mov	w6, w19
	sub	w5, w7, w19
	mov	x0, x19
	mov	w2, 0
	.p2align 2
.L81:
	ldrb	w3, [x0, 960]
	add	x0, x0, 1
	sub	w4, w0, w6
	cmp	w2, w3
	csel	w2, w2, w3, cs
	cmp	w4, w5
	bls	.L81
	uxtb	w2, w2
	strb	w2, [x7, 1024]
	ldr	w0, [x20, 12]
	cmp	w0, 6
	bls	.L82
	add	w2, w2, 1
	strb	w2, [x7, 1024]
.L82:
	add	x7, x7, 1
	cmp	x8, x7
	bne	.L80
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6_BuildScanOrder, .-VP6_BuildScanOrder
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_BuildModeTree
	.type	VP6_BuildModeTree, %function
VP6_BuildModeTree:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x15, x19, 44
	add	x16, x19, 10
	mov	x13, x19
	add	x8, x29, 104
	add	x14, x19, 60
	.p2align 2
.L89:
	sub	w7, w13, w19
	mov	x5, x19
	mov	x6, x15
	mov	x12, x13
	mov	x11, x13
.L95:
	mov	x1, 0
	mov	w4, 0
	.p2align 2
.L92:
	cmp	w7, w1
	add	x0, x5, x1
	beq	.L97
	ldrb	w0, [x0, 404]
	lsl	w2, w0, 2
	sub	w2, w2, w0
	lsl	w3, w2, 5
	add	w2, w2, w3
	add	w0, w2, w0
	str	w0, [x8, x1, lsl 2]
.L91:
	add	x1, x1, 1
	add	w4, w4, w0
	cmp	x1, 10
	bne	.L92
	ldrb	w23, [x11, 414]
	add	w4, w4, 1
	ldp	w22, w2, [x29, 104]
	ldp	w1, w21, [x29, 112]
	ldp	w10, w20, [x29, 120]
	ldrb	w24, [x11, 404]
	add	w1, w22, w1
	add	w10, w21, w10
	lsl	w26, w23, 8
	add	w3, w23, 1
	add	w0, w1, w10
	sub	w26, w26, w23
	add	w3, w3, w24
	ldp	w17, w18, [x29, 128]
	lsl	w23, w0, 8
	lsl	w27, w1, 8
	ldp	w30, w9, [x29, 136]
	sub	w23, w23, w0
	sub	w27, w27, w1
	sdiv	w3, w26, w3
	add	w0, w0, 1
	lsl	w26, w22, 8
	add	w17, w20, w17
	add	w18, w2, w18
	sub	w22, w26, w22
	lsl	w26, w2, 8
	add	w9, w30, w9
	lsl	w28, w18, 8
	sub	w26, w26, w2
	lsl	w2, w17, 8
	add	w25, w18, 1
	add	w24, w17, 1
	sub	w18, w28, w18
	lsl	w28, w21, 8
	add	w1, w1, 1
	sub	w21, w28, w21
	add	w10, w10, 1
	udiv	w27, w27, w0
	lsl	w0, w20, 8
	sub	w20, w0, w20
	add	w28, w9, w24
	mvn	w3, w3
	add	x5, x5, 20
	strb	w3, [x12, 4]
	add	w27, w27, 1
	cmp	x14, x5
	strb	w27, [x6, 1]
	add	x11, x11, 20
	add	x12, x12, 10
	add	x6, x6, 90
	udiv	w4, w23, w4
	add	w23, w9, w17
	sub	w17, w2, w17
	lsl	w2, w30, 8
	add	w23, w23, w25
	add	w9, w9, 1
	sub	w2, w2, w30
	add	w4, w4, 1
	strb	w4, [x6, -90]
	udiv	w24, w20, w24
	add	w0, w24, 1
	strb	w0, [x6, -83]
	udiv	w18, w18, w23
	add	w18, w18, 1
	strb	w18, [x6, -88]
	udiv	w1, w22, w1
	add	w1, w1, 1
	strb	w1, [x6, -87]
	udiv	w10, w21, w10
	add	w10, w10, 1
	strb	w10, [x6, -86]
	udiv	w25, w26, w25
	add	w25, w25, 1
	strb	w25, [x6, -85]
	udiv	w17, w17, w28
	add	w17, w17, 1
	strb	w17, [x6, -84]
	udiv	w2, w2, w9
	add	w0, w2, 1
	strb	w0, [x6, -82]
	bne	.L95
	add	x13, x13, 1
	add	x15, x15, 9
	cmp	x16, x13
	bne	.L89
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
.L97:
	str	wzr, [x8, x1, lsl 2]
	mov	w0, 0
	b	.L91
	.size	VP6_BuildModeTree, .-VP6_BuildModeTree
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_ConfigureContexts
	.type	VP6_ConfigureContexts, %function
VP6_ConfigureContexts:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x16, .LC21
	add	x4, x19, 1152
	mov	w11, 0
	mov	w9, 255
.L99:
	lsl	w12, w11, 2
	mov	x10, x16
	sub	w12, w12, w11
	lsl	w13, w11, 1
	add	w14, w12, 5
	neg	w15, w11, lsl 1
	mov	w7, 0
	mov	w5, 133
	mov	w0, 122
	add	w12, w12, 11
	add	w8, w11, 5
.L105:
	add	x3, x10, 28
	add	w17, w7, w13
	mov	w2, w11
	b	.L103
	.p2align 3
.L100:
	ldr	w0, [x3, -4]
	ldr	w5, [x3], 24
.L103:
	add	x1, x19, x2, uxtw
	add	w6, w17, w2
	add	w2, w2, 1
	ldrb	w1, [x1, 514]
	mul	w0, w1, w0
	add	w0, w0, 128
	add	w0, w5, w0, asr 8
	cmp	w0, 255
	csel	w0, w0, w9, le
	cmp	w0, wzr
	csinc	w0, w0, wzr, gt
	cmp	w8, w2
	strb	w0, [x4, x6]
	bne	.L100
	add	w0, w14, w7
	add	w3, w12, w7
	sub	w2, w15, w7
	.p2align 2
.L101:
	add	w1, w2, w0
	add	x1, x19, x1
	ldrb	w1, [x1, 514]
	strb	w1, [x4, w0, uxtw]
	add	w0, w0, 1
	cmp	w3, w0
	bne	.L101
	add	w7, w7, 11
	add	x10, x10, 8
	cmp	w7, 33
	beq	.L102
	ldp	w0, w5, [x10]
	b	.L105
	.p2align 3
.L102:
	add	w11, w11, 11
	cmp	w11, 22
	bne	.L99
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6_ConfigureContexts, .-VP6_ConfigureContexts
	.align	3
.LC21:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_ConfigureEntropyDecoder
	.type	VP6_ConfigureEntropyDecoder, %function
VP6_ConfigureEntropyDecoder:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x23, x0
	mov	x0, x30
	mov	x19, x2
	mov	x21, x1
	mov	w26, w3
	str	x2, [x29, 96]
	bl	_mcount
	add	x25, x29, 144
	ldr	x3, .LC28
	mov	x2, 11
	mov	w1, 128
	mov	x0, x25
	mov	x22, 0
	add	x20, x19, 72
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x0, .LC29
	mov	w2, 146
	add	x24, x0, 120
.L108:
	mov	x1, x20
	mov	x0, x21
	mov	x19, 0
	str	w22, [x29, 136]
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L109
.L152:
	mov	w28, 0
	mov	w27, 6
	.p2align 2
.L110:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w27
	sub	w27, w27, #1
	orr	w28, w0, w28
	cmn	w27, #1
	bne	.L110
	ldr	w0, [x29, 136]
	ubfiz	w3, w28, 1, 7
	cmp	w3, wzr
	add	w1, w0, w19
	csinc	w3, w3, w3, ne
	add	x1, x23, x1
	uxtb	w0, w3
	strb	w0, [x25, x19]
	strb	w0, [x1, 514]
.L111:
	add	x19, x19, 1
	cmp	x19, 11
	beq	.L151
.L112:
	add	x0, x24, x22
	mov	x1, x20
	ldrb	w2, [x0, x19]
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbnz	w0, .L152
.L109:
	cbnz	w26, .L111
	ldr	w0, [x29, 136]
	ldrb	w1, [x25, x19]
	add	w0, w0, w19
	add	x19, x19, 1
	add	x0, x23, x0
	cmp	x19, 11
	strb	w1, [x0, 514]
	bne	.L112
.L151:
	add	x22, x22, 11
	cmp	x22, 22
	beq	.L113
	ldrb	w2, [x22, x24]
	b	.L108
.L113:
	cbnz	w26, .L115
	ldr	x0, .LC28
	mov	x2, 28
	ldr	x3, [x0, 104]
	ldr	x0, .LC29
	add	x1, x0, 144
	add	x0, x23, 536
	blr	x3
.L115:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	cbz	w0, .L116
	ldr	x0, .LC29
	add	x22, x23, 1089
	mov	w2, 132
	add	x19, x0, 178
	add	x24, x0, 240
	.p2align 2
.L120:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L117
	mov	w28, 0
	mov	w27, 3
.L118:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w27
	sub	w27, w27, #1
	orr	w28, w0, w28
	cmn	w27, #1
	bne	.L118
	strb	w28, [x22]
.L117:
	cmp	x24, x19
	add	x22, x22, 1
	beq	.L119
	ldrb	w2, [x19], 1
	b	.L120
.L119:
	ldr	x1, [x29, 96]
	mov	x0, x23
	bl	VP6_BuildScanOrder
.L116:
	ldr	x0, .LC29
	mov	x22, 0
	mov	w2, 219
	add	x24, x0, 240
.L121:
	mov	x19, 0
	.p2align 2
.L127:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L122
	mov	w28, 0
	mov	w27, 6
	.p2align 2
.L123:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w27
	sub	w27, w27, #1
	orr	w28, w0, w28
	cmn	w27, #1
	bne	.L123
	add	x0, x19, x22
	ubfiz	w2, w28, 1, 7
	add	x0, x23, x0
	cmp	w2, wzr
	csinc	w2, w2, w2, ne
	strb	w2, [x0, 536]
.L122:
	add	x19, x19, 1
	cmp	x19, 14
	beq	.L153
	add	x0, x24, x22
	ldrb	w2, [x0, x19]
	b	.L127
.L153:
	add	x22, x22, 14
	cmp	x22, 28
	beq	.L154
	ldrb	w2, [x22, x24]
	b	.L121
.L154:
	ldr	x0, .LC29
	mov	w2, 227
	str	wzr, [x29, 128]
	add	x0, x0, 272
	str	x0, [x29, 112]
.L126:
	ldr	w0, [x29, 128]
	str	w0, [x29, 132]
	add	w1, w0, 198
	ldr	x0, [x29, 112]
	str	w1, [x29, 108]
	str	x0, [x29, 120]
.L137:
	ldr	x22, [x29, 120]
	str	xzr, [x29, 136]
	.p2align 2
.L135:
	ldp	w0, w1, [x29, 132]
	mov	x19, 0
	add	w24, w0, w1
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L128
	.p2align 2
.L156:
	mov	w28, 0
	mov	w27, 6
	.p2align 2
.L129:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w27
	sub	w27, w27, #1
	orr	w28, w0, w28
	cmn	w27, #1
	bne	.L129
	ubfiz	w3, w28, 1, 7
	add	w1, w24, w19
	cmp	w3, wzr
	add	x1, x23, x1
	csinc	w3, w3, w3, ne
	uxtb	w0, w3
	strb	w0, [x25, x19]
	strb	w0, [x1, 564]
.L130:
	add	x19, x19, 1
	cmp	x19, 11
	beq	.L155
.L131:
	ldrb	w2, [x22, x19]
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbnz	w0, .L156
.L128:
	cbnz	w26, .L130
	add	w0, w24, w19
	ldrb	w1, [x25, x19]
	add	x0, x23, x0
	add	x19, x19, 1
	cmp	x19, 11
	strb	w1, [x0, 564]
	bne	.L131
.L155:
	ldr	x0, [x29, 136]
	add	x22, x22, 11
	add	x0, x0, 11
	str	x0, [x29, 136]
	cmp	x0, 66
	beq	.L132
	ldrb	w2, [x22]
	b	.L135
.L132:
	ldr	x0, [x29, 120]
	ldr	w2, [x29, 108]
	ldr	w1, [x29, 132]
	add	x0, x0, 66
	str	x0, [x29, 120]
	cmp	w1, w2
	beq	.L134
	ldrb	w2, [x0]
	add	w0, w1, 198
	str	w0, [x29, 132]
	b	.L137
.L134:
	ldr	w0, [x29, 128]
	ldr	x1, [x29, 112]
	add	w0, w0, 66
	str	w0, [x29, 128]
	add	x1, x1, 132
	cmp	w0, 198
	str	x1, [x29, 112]
	beq	.L136
	ldrb	w2, [x1]
	b	.L126
.L136:
	ldr	x1, [x29, 96]
	mov	x0, x23
	bl	VP6_ConfigureContexts
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 160
	ret
	.size	VP6_ConfigureEntropyDecoder, .-VP6_ConfigureEntropyDecoder
	.align	3
.LC28:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC29:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_decodeModeDiff
	.type	VP6_decodeModeDiff, %function
VP6_decodeModeDiff:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x19, x1, 72
	bl	_mcount
	mov	w2, -51
	mov	x1, x19
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbnz	w0, .L167
	mov	w0, 0
.L158:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L167:
	mov	x1, x19
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	neg	w22, w0, lsl 1
	mov	w2, -85
	mov	x1, x19
	mov	x0, x21
	add	w22, w22, 1
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L168
	mov	w2, -57
	mov	x1, x19
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L169
	mov	w23, 0
	mov	w20, 6
	.p2align 2
.L164:
	mov	x1, x19
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w20
	sub	w20, w20, #1
	orr	w23, w0, w23
	cmn	w20, #1
	bne	.L164
	mul	w0, w22, w23
	ldp	x19, x20, [sp, 16]
	lsl	w0, w0, 2
	ldr	x23, [sp, 48]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L169:
	mov	w2, -116
	mov	x1, x19
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L161
	lsl	w0, w22, 2
	sub	w0, w0, w22
	lsl	w0, w0, 2
	b	.L158
	.p2align 3
.L168:
	mov	x1, x19
	mov	w2, 83
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	ldr	x23, [sp, 48]
	mov	w1, 3
	sub	w0, w1, w0
	lsl	w0, w22, w0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L161:
	mov	w2, 125
	mov	x1, x19
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L162
	lsl	w0, w22, 4
	b	.L158
.L162:
	mov	w2, 104
	mov	x1, x19
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L163
	lsl	w0, w22, 4
	add	w0, w0, w22, lsl 2
	b	.L158
.L163:
	lsl	w0, w22, 2
	sub	w0, w0, w22
	lsl	w0, w0, 3
	b	.L158
	.size	VP6_decodeModeDiff, .-VP6_decodeModeDiff
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_DecodeModeProbs
	.type	VP6_DecodeModeProbs, %function
VP6_DecodeModeProbs:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x27, x0
	mov	x0, x30
	mov	x21, x2
	mov	x19, x1
	bl	_mcount
	ldr	x26, .LC32
	add	x23, x21, 72
	add	x20, x27, 424
	add	x0, x26, 672
	add	x25, x27, 414
	mov	x24, 0
	mov	w28, 20
	str	x0, [x29, 104]
.L177:
	mov	w2, -82
	mov	x1, x23
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L176
	mov	w22, 0
	mov	w26, 3
.L174:
	mov	x1, x23
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w26
	sub	w26, w26, #1
	orr	w22, w0, w22
	cmn	w26, #1
	bne	.L174
	smaddl	x0, w22, w28, x24
	ldr	x2, [x29, 104]
	mov	x1, x25
	add	x0, x2, x0
	.p2align 2
.L175:
	add	x1, x1, 1
	ldrb	w2, [x0]
	add	x0, x0, 2
	cmp	x20, x1
	strb	w2, [x1, -1]
	ldrb	w2, [x0, -1]
	strb	w2, [x1, -11]
	bne	.L175
.L176:
	mov	w2, -2
	mov	x1, x23
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L173
	mov	x26, x25
	mov	w22, 255
	b	.L172
	.p2align 3
.L190:
	cmp	w0, 255
	mov	x1, x21
	csel	w0, w0, w22, le
	uxtb	w0, w0
	strb	w0, [x26]
	mov	x0, x19
	bl	VP6_decodeModeDiff
	ldrb	w1, [x26, -10]
	adds	w0, w1, w0
	bmi	.L183
.L191:
	cmp	w0, 255
	add	x26, x26, 1
	csel	w0, w0, w22, le
	uxtb	w0, w0
	strb	w0, [x26, -11]
	cmp	x26, x20
	beq	.L173
.L172:
	mov	x1, x21
	mov	x0, x19
	bl	VP6_decodeModeDiff
	ldrb	w1, [x26]
	adds	w0, w1, w0
	bpl	.L190
	mov	w0, 0
	mov	x1, x21
	strb	w0, [x26]
	mov	x0, x19
	bl	VP6_decodeModeDiff
	ldrb	w1, [x26, -10]
	adds	w0, w1, w0
	bpl	.L191
.L183:
	mov	w0, 0
	add	x26, x26, 1
	strb	w0, [x26, -11]
	cmp	x26, x20
	bne	.L172
	.p2align 2
.L173:
	add	x24, x24, 320
	add	x20, x20, 20
	cmp	x24, 960
	add	x25, x25, 20
	bne	.L177
	mov	x1, x21
	mov	x0, x27
	bl	VP6_BuildModeTree
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.size	VP6_DecodeModeProbs, .-VP6_DecodeModeProbs
	.align	3
.LC32:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_ConfigureMvEntropyDecoder
	.type	VP6_ConfigureMvEntropyDecoder, %function
VP6_ConfigureMvEntropyDecoder:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x25, x0
	mov	x0, x30
	mov	x20, x2
	mov	x21, x1
	bl	_mcount
	add	x20, x20, 72
	ldr	x0, .LC35
	mov	x22, x25
	add	x26, x25, 2
	mov	w28, 246
	add	x23, x0, 1649
	mov	w2, 237
	mov	w27, 1
.L205:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L194
	mov	w24, 0
	mov	w19, 6
	.p2align 2
.L195:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w24, w0, w24
	cmn	w19, #1
	bne	.L195
	ubfiz	w2, w24, 1, 7
	tst	w2, 254
	beq	.L196
	strb	w2, [x22, 2]
.L194:
	mov	w2, w28
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L199
	mov	w28, 0
	mov	w19, 6
	.p2align 2
.L200:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w28, w0, w28
	cmn	w19, #1
	bne	.L200
	ubfiz	w28, w28, 1, 7
	tst	w28, 254
	beq	.L201
	strb	w28, [x22]
.L199:
	add	x22, x22, 1
	cmp	x22, x26
	beq	.L227
	ldrb	w2, [x23]
	add	x23, x23, 17
	ldrb	w28, [x23, -16]
	b	.L205
	.p2align 3
.L227:
	ldr	x0, .LC35
	add	x23, x25, 7
	mov	w2, 253
	mov	w27, 1
	add	x26, x0, 1635
	add	x0, x25, 21
	str	x0, [x29, 104]
.L204:
	sub	x19, x23, #7
	mov	x22, x26
	.p2align 2
.L214:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L207
	mov	w28, 0
	mov	w24, 6
	.p2align 2
.L208:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w24
	sub	w24, w24, #1
	orr	w28, w0, w28
	cmn	w24, #1
	bne	.L208
	ubfiz	w3, w28, 1, 7
	tst	w3, 254
	beq	.L209
	strb	w3, [x19, 484]
.L207:
	add	x19, x19, 1
	cmp	x19, x23
	beq	.L228
.L211:
	ldrb	w2, [x22], 1
	b	.L214
	.p2align 3
.L209:
	strb	w27, [x19, 484]
	add	x19, x19, 1
	cmp	x19, x23
	bne	.L211
.L228:
	ldr	x0, [x29, 104]
	add	x23, x19, 7
	add	x26, x26, 17
	cmp	x23, x0
	beq	.L229
	ldrb	w2, [x26, -1]
	b	.L204
	.p2align 3
.L229:
	ldr	x0, .LC35
	add	x27, x25, 8
	mov	w2, 254
	add	x25, x25, 24
	add	x24, x0, 1642
	mov	w28, 1
.L213:
	sub	x23, x27, #8
	mov	x26, x24
	.p2align 2
.L222:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool
	cbz	w0, .L216
	mov	w22, 0
	mov	w19, 6
	.p2align 2
.L217:
	mov	x1, x20
	mov	x0, x21
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w22, w0, w22
	cmn	w19, #1
	bne	.L217
	ubfiz	w22, w22, 1, 7
	tst	w22, 254
	beq	.L218
	strb	w22, [x23, 498]
.L216:
	add	x23, x23, 1
	cmp	x23, x27
	beq	.L230
.L220:
	ldrb	w2, [x26], 1
	b	.L222
	.p2align 3
.L218:
	strb	w28, [x23, 498]
	add	x23, x23, 1
	cmp	x23, x27
	bne	.L220
.L230:
	add	x27, x27, 8
	add	x24, x24, 17
	cmp	x25, x27
	beq	.L192
	ldrb	w2, [x24, -1]
	b	.L213
	.p2align 3
.L192:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L196:
	strb	w27, [x22, 2]
	b	.L194
.L201:
	strb	w27, [x22]
	b	.L199
	.size	VP6_ConfigureMvEntropyDecoder, .-VP6_ConfigureMvEntropyDecoder
	.align	3
.LC35:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_BoolTreeToHuffCodes
	.type	VP6_BoolTreeToHuffCodes, %function
VP6_BoolTreeToHuffCodes:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldrb	w2, [x19, 1]
	mov	w0, 255
	ldrb	w1, [x19]
	mul	w1, w1, w2
	lsr	w1, w1, 8
	str	w1, [x20, 44]
	ldrb	w1, [x19, 1]
	ldrb	w2, [x19]
	sub	w1, w0, w1
	mul	w1, w1, w2
	lsr	w1, w1, 8
	str	w1, [x20]
	ldrb	w2, [x19]
	ldrb	w3, [x19, 2]
	sub	w2, w0, w2
	mul	w3, w3, w2
	lsr	w3, w3, 8
	str	w3, [x20, 4]
	ldrb	w1, [x19, 2]
	ldrb	w3, [x19, 3]
	sub	w1, w0, w1
	ldrb	w4, [x19, 4]
	mul	w2, w1, w2
	lsr	w2, w2, 8
	mul	w1, w3, w2
	lsr	w3, w1, 8
	mul	w1, w4, w3
	lsr	w1, w1, 8
	str	w1, [x20, 8]
	ldrb	w1, [x19, 4]
	ldrb	w4, [x19, 5]
	sub	w1, w0, w1
	mul	w1, w1, w3
	lsr	w1, w1, 8
	mul	w3, w4, w1
	lsr	w3, w3, 8
	str	w3, [x20, 12]
	ldrb	w3, [x19, 5]
	sub	w3, w0, w3
	mul	w1, w3, w1
	lsr	w1, w1, 8
	str	w1, [x20, 16]
	ldrb	w1, [x19, 3]
	ldrb	w3, [x19, 6]
	sub	w1, w0, w1
	ldrb	w4, [x19, 7]
	mul	w2, w1, w2
	lsr	w2, w2, 8
	mul	w1, w3, w2
	lsr	w3, w1, 8
	mul	w1, w4, w3
	lsr	w1, w1, 8
	str	w1, [x20, 20]
	ldrb	w1, [x19, 7]
	sub	w1, w0, w1
	mul	w1, w1, w3
	lsr	w1, w1, 8
	str	w1, [x20, 24]
	ldrb	w1, [x19, 6]
	ldrb	w3, [x19, 8]
	sub	w1, w0, w1
	ldrb	w4, [x19, 9]
	mul	w1, w1, w2
	lsr	w2, w1, 8
	mul	w1, w3, w2
	lsr	w3, w1, 8
	mul	w1, w4, w3
	lsr	w1, w1, 8
	str	w1, [x20, 28]
	ldrb	w1, [x19, 9]
	sub	w1, w0, w1
	mul	w1, w1, w3
	lsr	w1, w1, 8
	str	w1, [x20, 32]
	ldrb	w1, [x19, 8]
	ldrb	w3, [x19, 10]
	sub	w1, w0, w1
	mul	w1, w1, w2
	lsr	w1, w1, 8
	mul	w2, w3, w1
	lsr	w2, w2, 8
	str	w2, [x20, 36]
	ldrb	w2, [x19, 10]
	sub	w0, w0, w2
	mul	w0, w0, w1
	lsr	w0, w0, 8
	str	w0, [x20, 40]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6_BoolTreeToHuffCodes, .-VP6_BoolTreeToHuffCodes
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_ZerosBoolTreeToHuffCodes
	.type	VP6_ZerosBoolTreeToHuffCodes, %function
VP6_ZerosBoolTreeToHuffCodes:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldrb	w2, [x19, 1]
	mov	w0, 255
	ldrb	w1, [x19]
	ldrb	w3, [x19, 2]
	mul	w1, w1, w2
	lsr	w2, w1, 8
	mul	w1, w3, w2
	lsr	w1, w1, 8
	str	w1, [x20]
	ldrb	w1, [x19, 2]
	sub	w1, w0, w1
	mul	w1, w1, w2
	lsr	w1, w1, 8
	str	w1, [x20, 4]
	ldrb	w1, [x19, 1]
	ldrb	w3, [x19]
	sub	w1, w0, w1
	ldrb	w2, [x19, 3]
	mul	w1, w1, w3
	lsr	w1, w1, 8
	mul	w2, w2, w1
	lsr	w2, w2, 8
	str	w2, [x20, 8]
	ldrb	w2, [x19, 3]
	sub	w2, w0, w2
	mul	w1, w2, w1
	lsr	w1, w1, 8
	str	w1, [x20, 12]
	ldrb	w3, [x19]
	ldrb	w2, [x19, 4]
	sub	w3, w0, w3
	ldrb	w4, [x19, 5]
	ldrb	w1, [x19, 6]
	mul	w2, w3, w2
	lsr	w2, w2, 8
	mul	w2, w2, w4
	lsr	w2, w2, 8
	mul	w1, w1, w2
	lsr	w1, w1, 8
	str	w1, [x20, 16]
	ldrb	w1, [x19, 6]
	sub	w1, w0, w1
	mul	w2, w1, w2
	lsr	w2, w2, 8
	str	w2, [x20, 20]
	ldrb	w3, [x19]
	ldrb	w2, [x19, 4]
	sub	w3, w0, w3
	ldrb	w4, [x19, 5]
	ldrb	w1, [x19, 7]
	mul	w2, w3, w2
	sub	w4, w0, w4
	lsr	w2, w2, 8
	mul	w2, w2, w4
	lsr	w2, w2, 8
	mul	w1, w1, w2
	lsr	w1, w1, 8
	str	w1, [x20, 24]
	ldrb	w1, [x19, 7]
	sub	w1, w0, w1
	mul	w2, w1, w2
	lsr	w2, w2, 8
	str	w2, [x20, 28]
	ldrb	w2, [x19, 4]
	ldrb	w1, [x19]
	sub	w1, w0, w1
	sub	w0, w0, w2
	mul	w0, w1, w0
	lsr	w0, w0, 8
	str	w0, [x20, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6_ZerosBoolTreeToHuffCodes, .-VP6_ZerosBoolTreeToHuffCodes
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_InsertSorted
	.type	VP6_InsertSorted, %function
VP6_InsertSorted:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x2
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	w8, [x21]
	uxtw	x7, w20
	cmn	w8, #1
	beq	.L234
	uxtw	x1, w8
	lsl	x3, x7, 2
	lsl	x0, x1, 2
	sub	x7, x3, x7
	sub	x0, x0, x1
	add	x7, x19, x7, lsl 2
	add	x0, x19, x0, lsl 2
	ldr	w6, [x7, 8]
	ldr	w1, [x0, 8]
	cmp	w6, w1
	bls	.L235
	uxtw	x5, w8
	b	.L237
	.p2align 3
.L245:
	sub	x0, x0, x4
	add	x0, x19, x0, lsl 2
	ldr	w4, [x0, 8]
	cmp	w4, w6
	bcs	.L244
	uxtw	x5, w3
.L237:
	ldr	w3, [x0, 4]
	cmn	w3, #1
	uxtw	x4, w3
	lsl	x0, x4, 2
	bne	.L245
.L236:
	lsl	x0, x5, 2
	sub	x5, x0, x5
	add	x19, x19, x5, lsl 2
	str	w20, [x19, 4]
	str	w3, [x7, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L244:
	cmp	w8, w3
	bne	.L236
.L239:
	str	w20, [x21]
	str	w3, [x7, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L234:
	lsl	x0, x7, 2
	sub	x7, x0, x7
	add	x7, x19, x7, lsl 2
.L235:
	mov	w3, w8
	b	.L239
	.size	VP6_InsertSorted, .-VP6_InsertSorted
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_BuildHuffTree
	.type	VP6_BuildHuffTree, %function
VP6_BuildHuffTree:
	stp	x29, x30, [sp, -448]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x22, x0
	mov	x0, x30
	mov	w24, w2
	mov	x21, x1
	bl	_mcount
	str	wzr, [x29, 84]
	sub	w19, w24, #1
	cbz	w24, .L251
	add	x20, x29, 88
	mov	x3, 0
	add	x4, x20, 8
	mov	w6, 1
	mov	w1, -1
	.p2align 2
.L250:
	uxtw	x2, w3
	ldr	w5, [x21, x3, lsl 2]
	lsl	x0, x2, 2
	sub	x0, x0, x2
	add	x2, x29, 448
	add	x0, x2, x0, lsl 2
	sub	x0, x0, #512
	ldrb	w2, [x0, 152]
	orr	w2, w2, 1
	bfi	w2, w3, 1, 4
	and	w2, w2, 31
	strb	w2, [x0, 152]
	cbnz	w5, .L249
	str	w6, [x21, x3, lsl 2]
	mov	w5, 1
.L249:
	add	x3, x3, 1
	stp	w1, w5, [x4, -4]
	cmp	w24, w3
	add	x4, x4, 12
	bhi	.L250
	cmp	w24, 1
	bls	.L251
	mov	w23, 1
	add	x21, x29, 84
	.p2align 2
.L252:
	mov	w1, w23
	mov	x2, x21
	mov	x0, x20
	add	w23, w23, 1
	bl	VP6_InsertSorted
	cmp	w24, w23
	bne	.L252
	ldr	w2, [x29, 84]
.L248:
	uxtw	x2, w2
	add	x1, x29, 448
	lsl	x0, x2, 2
	sub	x0, x0, x2
	add	x0, x1, x0, lsl 2
	sub	x0, x0, #4096
	ldr	w4, [x0, 3740]
	cmn	w4, #1
	beq	.L246
	sub	w25, w24, #2
	add	x20, x29, 88
	and	w25, w25, 15
	add	x21, x29, 84
	mov	w23, -1
	.p2align 2
.L254:
	uxtw	x3, w24
	lsl	x0, x2, 2
	sub	x2, x0, x2
	uxtw	x4, w4
	lsl	x0, x3, 2
	lsl	x1, x4, 2
	sub	x3, x0, x3
	sub	x4, x1, x4
	add	x0, x29, 448
	add	x1, x29, 448
	add	x3, x1, x3, lsl 2
	add	x0, x0, x2, lsl 2
	sub	x7, x3, #512
	sub	x0, x0, #4096
	sub	x3, x3, #4096
	add	x4, x1, x4, lsl 2
	sub	w19, w19, #1
	sub	x4, x4, #4096
	ubfiz	x1, x19, 1, 32
	ldrb	w6, [x0, 3736]
	add	x2, x22, x1
	ldrb	w5, [x7, 152]
	str	w23, [x3, 3740]
	ldrb	w8, [x4, 3736]
	and	w5, w5, 254
	strb	w6, [x22, x1]
	bfi	w5, w25, 1, 4
	ldr	w9, [x0, 3744]
	mov	w1, w24
	ldr	w6, [x4, 3744]
	mov	x0, x20
	strb	w8, [x2, 1]
	mov	x2, x21
	ldr	w8, [x4, 3740]
	add	w4, w9, w6
	strb	w5, [x7, 152]
	add	w25, w25, 15
	str	w4, [x3, 3744]
	add	w24, w24, 1
	str	w8, [x29, 84]
	and	w25, w25, 15
	bl	VP6_InsertSorted
	ldr	w2, [x29, 84]
	add	x0, x29, 448
	lsl	x1, x2, 2
	sub	x1, x1, x2
	add	x1, x0, x1, lsl 2
	sub	x1, x1, #4096
	ldr	w4, [x1, 3740]
	cmn	w4, #1
	bne	.L254
.L246:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 448
	ret
.L251:
	mov	w2, 0
	b	.L248
	.size	VP6_BuildHuffTree, .-VP6_BuildHuffTree
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_BuildHuffLookupTable
	.type	VP6_BuildHuffLookupTable, %function
VP6_BuildHuffLookupTable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	mov	x5, 0
	mov	w7, 6
	.p2align 2
.L263:
	mov	w6, w5
	mov	w0, 0
	mov	w2, 0
	b	.L262
	.p2align 3
.L267:
	ldrb	w3, [x0, 1]
	and	w4, w3, 1
	ubfx	x0, x3, 1, 4
	cmp	w4, wzr
	ccmp	w2, 5, 2, eq
	bhi	.L266
.L262:
	add	w2, w2, 1
	ubfiz	x4, x0, 1, 4
	sub	w3, w7, w2
	add	x0, x19, x4
	asr	w3, w6, w3
	tbnz	x3, 0, .L267
	ldrb	w3, [x19, x4]
	and	w4, w3, 1
	ubfx	x0, x3, 1, 4
	cmp	w4, wzr
	ccmp	w2, 5, 2, eq
	bls	.L262
.L266:
	ldrb	w3, [x20, x5]
	bfi	w3, w0, 4, 4
	mov	w0, w3
	bfi	w0, w2, 1, 3
	bfi	w0, w4, 0, 1
	strb	w0, [x20, x5]
	add	x5, x5, 1
	cmp	x5, 64
	bne	.L263
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6_BuildHuffLookupTable, .-VP6_BuildHuffLookupTable
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_BuildTreeTab
	.type	VP6_BuildTreeTab, %function
VP6_BuildTreeTab:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x8, x19, 8192
	mov	w0, 0
	add	x10, x8, 926
	add	x11, x8, 1790
.L269:
	sub	x9, x10, #432
.L278:
	sub	x7, x9, #96
.L276:
	add	x1, x19, x0, uxtw
	add	w2, w0, 1
	ldrb	w4, [x7, -2]
	mov	x3, x7
	add	x2, x19, x2, uxtw
	add	w0, w0, 2
	strb	w4, [x1, 1218]
	mov	w1, 0
	ldrb	w4, [x7, -1]
	strb	w4, [x2, 1218]
	b	.L274
	.p2align 3
.L272:
	ldrb	w0, [x3]
	cmp	w1, 5
	add	x3, x3, 2
	strb	w0, [x6, 1218]
	ccmp	w1, 11, 4, ne
	cset	w6, eq
	ldrb	w0, [x3, -1]
	cmp	w6, wzr
	strb	w0, [x2, 1218]
	csel	w0, w4, w5, ne
.L274:
	add	w1, w1, 1
	add	w2, w0, 1
	cmp	w1, 12
	add	x6, x19, x0, uxtw
	add	x2, x19, x2
	add	w5, w0, 2
	add	w4, w0, 6
	bne	.L272
	add	x7, x7, 24
	cmp	x7, x9
	bne	.L276
	add	x9, x7, 144
	cmp	x10, x9
	bne	.L278
	add	x10, x7, 576
	cmp	x10, x11
	bne	.L269
	add	x7, x8, 1262
	add	x9, x8, 1310
	mov	w0, 768
.L277:
	add	x1, x19, x0, uxtw
	add	w2, w0, 1
	ldrb	w4, [x7, -2]
	mov	x3, x7
	add	x2, x19, x2, uxtw
	add	w0, w0, 2
	strb	w4, [x1, 1218]
	mov	w1, 0
	ldrb	w4, [x7, -1]
	strb	w4, [x2, 1218]
	b	.L283
	.p2align 3
.L281:
	ldrb	w0, [x3]
	cmp	w1, 5
	add	x3, x3, 2
	strb	w0, [x6, 1218]
	ccmp	w1, 11, 4, ne
	cset	w6, eq
	ldrb	w0, [x3, -1]
	cmp	w6, wzr
	strb	w0, [x2, 1218]
	csel	w0, w4, w5, ne
.L283:
	add	w1, w1, 1
	add	w2, w0, 1
	cmp	w1, 12
	add	x6, x19, x0, uxtw
	add	x2, x19, x2
	add	w5, w0, 2
	add	w4, w0, 6
	bne	.L281
	add	x7, x7, 24
	cmp	x9, x7
	bne	.L277
	add	x6, x8, 340
	mov	w3, 832
	add	x8, x8, 396
.L282:
	mov	x2, x6
	mov	w1, 0
	b	.L288
	.p2align 3
.L293:
	add	w1, w1, 1
	add	w3, w3, 6
	cmp	w1, 14
	add	x2, x2, 2
	beq	.L287
.L288:
	add	x4, x19, x3, uxtw
	ldrb	w5, [x2]
	add	w0, w3, 1
	cmp	w1, 5
	add	x0, x19, x0
	ccmp	w1, 11, 4, ne
	strb	w5, [x4, 1218]
	ldrb	w4, [x2, 1]
	strb	w4, [x0, 1218]
	beq	.L293
	cmp	w1, 13
	beq	.L294
	add	w1, w1, 1
	add	w3, w3, 2
	cmp	w1, 14
	add	x2, x2, 2
	bne	.L288
.L287:
	add	x6, x6, 28
	cmp	x8, x6
	bne	.L282
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L294:
	add	w3, w3, 30
	b	.L287
	.size	VP6_BuildTreeTab, .-VP6_BuildTreeTab
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_ConvertBoolToHuff
	.type	VP6_ConvertBoolToHuff, %function
VP6_ConvertBoolToHuff:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	add	x27, x21, 4096
	add	x19, x21, 8192
	bl	_mcount
	add	x23, x27, 2500
	ldr	x1, .LC37
	mov	w0, 22
	add	x22, x19, 1260
	add	x20, x27, 2548
	add	x25, x19, 1284
	add	x24, x27, 2596
	bl	dprint_vfmw
	mov	w26, 198
	mov	x1, x23
	add	x0, x21, 514
	bl	VP6_BoolTreeToHuffCodes
	mov	w2, 12
	mov	x1, x23
	mov	x0, x22
	add	x23, x19, 340
	bl	VP6_BuildHuffTree
	mov	x0, x22
	add	x1, x19, 1308
	add	x22, x27, 2652
	bl	VP6_BuildHuffLookupTable
	mov	x1, x20
	add	x0, x21, 525
	bl	VP6_BoolTreeToHuffCodes
	add	x3, x27, 2708
	mov	w2, 12
	mov	x1, x20
	mov	x0, x25
	str	x3, [x29, 104]
	add	x20, x19, 368
	bl	VP6_BuildHuffTree
	add	x1, x19, 1372
	mov	x0, x25
	bl	VP6_BuildHuffLookupTable
	mov	x1, x24
	add	x0, x21, 536
	bl	VP6_ZerosBoolTreeToHuffCodes
	add	x0, x19, 396
	mov	w2, 9
	mov	x1, x24
	str	x0, [x29, 120]
	mov	x0, x23
	bl	VP6_BuildHuffTree
	add	x0, x19, 1436
	add	x1, x19, 3740
	str	x0, [x29, 112]
	mov	x0, x23
	bl	VP6_BuildHuffLookupTable
	mov	x1, x22
	add	x0, x21, 550
	bl	VP6_ZerosBoolTreeToHuffCodes
	mov	x1, x22
	mov	x0, x20
	mov	w2, 9
	bl	VP6_BuildHuffTree
	add	x1, x19, 3804
	mov	x0, x20
	bl	VP6_BuildHuffLookupTable
.L296:
	ldp	x23, x25, [x29, 104]
	ldr	x24, [x29, 120]
	sub	w27, w26, #198
.L300:
	add	w22, w27, 66
	mov	x20, x25
	mov	x19, x24
	mov	x28, x23
	.p2align 2
.L297:
	add	x0, x21, x27, uxtw
	mov	x1, x28
	add	x0, x0, 564
	add	w27, w27, 11
	bl	VP6_BoolTreeToHuffCodes
	mov	x1, x28
	mov	x0, x19
	mov	w2, 12
	add	x28, x28, 48
	bl	VP6_BuildHuffTree
	mov	x1, x20
	mov	x0, x19
	add	x20, x20, 64
	add	x19, x19, 24
	bl	VP6_BuildHuffLookupTable
	cmp	w27, w22
	bne	.L297
	cmp	w26, w27
	add	x23, x23, 288
	add	x24, x24, 144
	add	x25, x25, 384
	bne	.L300
	ldr	x0, [x29, 104]
	add	w26, w26, 198
	cmp	w26, 594
	add	x0, x0, 864
	str	x0, [x29, 104]
	ldr	x0, [x29, 120]
	add	x0, x0, 432
	str	x0, [x29, 120]
	ldr	x0, [x29, 112]
	add	x0, x0, 1152
	str	x0, [x29, 112]
	bne	.L296
	mov	x0, x21
	bl	VP6_BuildTreeTab
	ldr	x1, .LC39
	mov	w0, 22
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.size	VP6_ConvertBoolToHuff, .-VP6_ConvertBoolToHuff
	.align	3
.LC37:
	.xword	.LC36
	.align	3
.LC39:
	.xword	.LC38
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_IntraFrmHeader
	.type	VP6_IntraFrmHeader, %function
VP6_IntraFrmHeader:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x24, x0
	mov	x0, x30
	mov	x19, x1
	add	x22, x24, 12288
	bl	_mcount
	sub	x21, x22, #36
	ldr	x1, .LC41
	mov	w0, 22
	bl	dprint_vfmw
	mov	w1, 5
	mov	x0, x19
	bl	BsGet
	uxtb	w0, w0
	mov	w1, 2
	str	w0, [x22, -24]
	mov	x0, x19
	bl	BsGet
	uxtb	w0, w0
	ldr	w2, [x22, -24]
	str	w0, [x22, -20]
	cmp	w2, 8
	bhi	.L333
	mov	w1, 1
	mov	x0, x19
	bl	BsGet
	uxtb	w0, w0
	cbnz	w0, .L334
	ldrb	w0, [x22, -34]
	cbz	w0, .L306
	mov	w1, 16
	mov	x0, x19
	bl	BsGet
	str	w0, [x21, 52]
	str	wzr, [x24, 12324]
	mov	w1, 255
	mov	w0, 8
	str	w1, [x22, 40]
	str	w0, [x22, 48]
	mov	w1, 24
	mov	x0, x19
	bl	BsGet
	str	w0, [x22, 44]
.L307:
	mov	x20, 12324
	mov	w23, 0
	add	x20, x24, x20
	mov	w21, 7
	.p2align 2
.L308:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w23, w0, w23
	cmn	w21, #1
	bne	.L308
	and	w0, w23, 255
	mov	w21, 7
	mov	w23, 0
	str	w0, [x22, -12]
	.p2align 2
.L309:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w23, w0, w23
	cmn	w21, #1
	bne	.L309
	ldr	w26, [x22, -12]
	and	w23, w23, 255
	lsl	w25, w23, 1
	mov	w21, 7
	lsl	w26, w26, 1
	str	w25, [x22]
	stp	w23, w26, [x22, -8]
	mov	w23, 0
	.p2align 2
.L310:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w23, w0, w23
	cmn	w21, #1
	bne	.L310
	ubfiz	w0, w23, 1, 8
	mov	w21, 7
	mov	w23, 0
	str	w0, [x22, 4]
	.p2align 2
.L311:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w23, w0, w23
	cmn	w21, #1
	bne	.L311
	mov	x1, x20
	ubfiz	w23, w23, 1, 8
	mov	x0, x19
	str	w23, [x22, 8]
	bl	Vp6_Vfmw_DecodeBool128
	mov	w21, w0
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	orr	w0, w0, w21, lsl 1
	strb	w0, [x22, -33]
	ldr	w3, [x24, 12064]
	ldr	w2, [x24, 12060]
	cmp	w26, w3
	bne	.L313
	cmp	w25, w2
	beq	.L314
.L313:
	ldr	x1, .LC47
	mov	w5, w26
	mov	w4, w25
	mov	w0, 22
	bl	dprint_vfmw
.L314:
	str	w25, [x24, 12060]
	sub	x21, x22, #36
	str	w26, [x24, 12064]
	ldr	w0, [x22, -20]
	cbnz	w0, .L335
	mov	w0, 14
	strb	w0, [x22, -30]
	str	wzr, [x21, 56]
.L324:
	ldr	x1, .LC49
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L333:
	ldr	x1, .LC43
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L306:
	str	wzr, [x24, 12324]
	mov	w3, 255
	mov	w2, 8
	mov	w1, 24
	mov	x0, x19
	str	w3, [x22, 40]
	str	w2, [x22, 48]
	bl	BsGet
	str	w0, [x22, 44]
	b	.L307
	.p2align 3
.L334:
	ldr	x1, .LC45
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L335:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	cbz	w0, .L316
	mov	w0, 2
	mov	w23, 0
	str	w0, [x21, 56]
	mov	w21, 4
	.p2align 2
.L317:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w23, w0, w23
	cmn	w21, #1
	bne	.L317
	ldr	w1, [x22, -24]
	mov	w0, 5
	and	w23, w23, 255
	mov	w24, 0
	cmp	w1, 7
	mov	w21, 2
	csel	w0, wzr, w0, hi
	lsl	w0, w23, w0
	strb	w0, [x22, -32]
.L319:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w24, w0, w24
	cmn	w21, #1
	bne	.L319
	strb	w24, [x22, -31]
.L320:
	ldr	w0, [x22, -24]
	cmp	w0, 7
	bls	.L322
	mov	w23, 0
	mov	w21, 3
.L323:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w23, w0, w23
	cmn	w21, #1
	bne	.L323
	strb	w23, [x22, -30]
	b	.L324
	.p2align 3
.L322:
	mov	w0, 16
	strb	w0, [x22, -30]
	b	.L324
	.p2align 3
.L316:
	mov	x1, x20
	mov	x0, x19
	bl	Vp6_Vfmw_DecodeBool128
	cbz	w0, .L321
	mov	w0, 1
	str	w0, [x21, 56]
	b	.L320
.L321:
	str	wzr, [x21, 56]
	b	.L320
	.size	VP6_IntraFrmHeader, .-VP6_IntraFrmHeader
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_InterFrmHeader
	.type	VP6_InterFrmHeader, %function
VP6_InterFrmHeader:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	ldr	w0, [x19, 12120]
	cbz	w0, .L358
	add	x20, x19, 12288
	str	w0, [x20, -24]
	ldr	w1, [x19, 12124]
	ldrb	w0, [x20, -34]
	str	w1, [x20, -20]
	cbnz	w0, .L359
	str	wzr, [x19, 12324]
	mov	w3, 255
	mov	w2, 8
	mov	w1, 24
	mov	x0, x22
	str	w3, [x20, 40]
	str	w2, [x20, 48]
	bl	BsGet
	str	w0, [x20, 44]
.L340:
	mov	x21, 12324
	sub	x23, x20, #36
	add	x21, x19, x21
	mov	x0, x22
	mov	x1, x21
	bl	Vp6_Vfmw_DecodeBool128
	str	w0, [x23, 60]
	str	w0, [x19, 12084]
	ldr	w0, [x19, 12124]
	cbnz	w0, .L360
	mov	w0, 14
	strb	w0, [x20, -30]
	mov	w19, 1
	str	wzr, [x23, 56]
.L338:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L359:
	mov	w1, 16
	mov	x0, x22
	bl	BsGet
	str	w0, [x20, 16]
	str	wzr, [x19, 12324]
	mov	w1, 255
	mov	w0, 8
	str	w1, [x20, 40]
	str	w0, [x20, 48]
	mov	w1, 24
	mov	x0, x22
	bl	BsGet
	str	w0, [x20, 44]
	b	.L340
	.p2align 3
.L360:
	mov	x1, x21
	mov	x0, x22
	bl	Vp6_Vfmw_DecodeBool128
	str	w0, [x23, 64]
	mov	w24, w0
	cbnz	w0, .L361
.L342:
	ldr	w0, [x19, 12120]
	cmp	w0, 7
	bhi	.L362
	mov	w0, 16
	strb	w0, [x20, -30]
	mov	w19, 1
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L362:
	mov	x1, x21
	mov	x0, x22
	mov	w19, 1
	bl	Vp6_Vfmw_DecodeBool128
	cbz	w0, .L338
	mov	x1, x21
	mov	x0, x22
	bl	Vp6_Vfmw_DecodeBool128
	cbz	w0, .L344
	mov	w0, 2
	mov	w23, 0
	mov	w19, 4
	str	w0, [x20, 20]
	.p2align 2
.L345:
	mov	x1, x21
	mov	x0, x22
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w23, w0, w23
	cmn	w19, #1
	bne	.L345
	strb	w23, [x20, -32]
	mov	w19, 2
	mov	w23, 0
.L346:
	mov	x1, x21
	mov	x0, x22
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w23, w0, w23
	cmn	w19, #1
	bne	.L346
	strb	w23, [x20, -31]
.L349:
	mov	w23, 0
	mov	w19, 3
.L347:
	mov	x1, x21
	mov	x0, x22
	bl	Vp6_Vfmw_DecodeBool128
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w23, w0, w23
	cmn	w19, #1
	bne	.L347
	strb	w23, [x20, -30]
	mov	w19, 1
	b	.L338
	.p2align 3
.L361:
	mov	x1, x21
	mov	x0, x22
	bl	Vp6_Vfmw_DecodeBool128
	orr	w0, w0, w24, lsl 1
	str	w0, [x23, 64]
	b	.L342
	.p2align 3
.L358:
	ldr	w3, [x19, 12264]
	mov	w2, 0
	ldr	x1, .LC51
	mov	w0, 1
	mov	w19, -88
	bl	dprint_vfmw
	b	.L338
	.p2align 3
.L344:
	mov	x1, x21
	mov	x0, x22
	bl	Vp6_Vfmw_DecodeBool128
	cmp	w0, wzr
	mov	w1, 0
	csel	w19, w19, w1, ne
	str	w19, [x20, 20]
	b	.L349
	.size	VP6_InterFrmHeader, .-VP6_InterFrmHeader
	.align	3
.LC51:
	.xword	.LC50
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_GetPictureHeader
	.type	VP6_GetPictureHeader, %function
VP6_GetPictureHeader:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	x21, xzr
	ccmp	x20, xzr, 4, ne
	beq	.L369
	ldr	x1, .LC55
	mov	w0, 22
	add	x19, x21, 12288
	add	x22, x21, 8192
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	and	w1, w0, 255
	str	w1, [x19, -16]
	mov	w1, 6
	strb	w0, [x22, 3888]
	mov	x0, x20
	bl	BsGet
	and	w3, w0, 255
	ldr	x2, .LC56
	mov	w1, 1
	strb	w0, [x19, -36]
	mov	x0, x20
	add	x5, x2, 1672
	add	x4, x2, 1736
	add	x2, x2, 1864
	ldrb	w5, [x5, w3, sxtw]
	ldrh	w4, [x4, w3, sxtw 1]
	ldrh	w2, [x2, w3, sxtw 1]
	strb	w5, [x19, -35]
	strh	w4, [x19, -28]
	strh	w2, [x19, -26]
	bl	BsGet
	strb	w0, [x19, -34]
	ldr	w1, [x19, -16]
	cbz	w1, .L370
	mov	x0, x21
	mov	x1, x20
	bl	VP6_InterFrmHeader
	cmp	w0, 1
	mov	w21, w0
	beq	.L371
.L365:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L370:
	mov	x0, x21
	mov	x1, x20
	bl	VP6_IntraFrmHeader
	cmp	w0, 1
	mov	w21, w0
	bne	.L365
.L371:
	add	x1, x22, 4060
	mov	x0, x20
	add	x1, x1, 72
	bl	Vp6_Vfmw_DecodeBool128
	str	w0, [x19, 32]
	ldrb	w3, [x19, -36]
	mov	w0, 22
	ldr	x1, .LC58
	str	w3, [x19, 12]
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L369:
	ldr	x1, .LC53
	mov	w0, 22
	mov	w21, 0
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VP6_GetPictureHeader, .-VP6_GetPictureHeader
	.align	3
.LC53:
	.xword	.LC52
	.align	3
.LC55:
	.xword	.LC54
	.align	3
.LC56:
	.xword	.LANCHOR0
	.align	3
.LC58:
	.xword	.LC57
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6DEC_Init
	.type	VP6DEC_Init, %function
VP6DEC_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x21, .LC62
	mov	x2, 13376
	mov	w1, 0
	mov	x0, x19
	ldr	w22, [x19, 13352]
	ldr	x3, [x21, 96]
	blr	x3
	str	w22, [x19, 13352]
	add	x0, x19, 12288
	add	x0, x0, 464
	bl	ResetVoQueue
	str	x20, [x19]
	mov	x0, x19
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x19, 13360]
	beq	.L375
	ldr	x3, [x21, 96]
	add	x20, x19, 312
	mov	x2, 12396
	mov	w1, 0
	mov	x0, x20
	blr	x3
	str	wzr, [x19, 12388]
	ldr	x2, [x19]
	mov	w0, 1024
	str	w0, [x19, 12416]
	mov	w0, 22
	ldr	x1, .LC66
	ldr	w2, [x2, 8]
	str	w2, [x19, 12380]
	bl	dprint_vfmw
	mov	w0, 1
.L374:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L375:
	ldr	x1, .LC64
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L374
	.size	VP6DEC_Init, .-VP6DEC_Init
	.align	3
.LC62:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC64:
	.xword	.LC63
	.align	3
.LC66:
	.xword	.LC65
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6DEC_Destroy
	.type	VP6DEC_Destroy, %function
VP6DEC_Destroy:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VP6DEC_Destroy, .-VP6DEC_Destroy
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_DecodeDecide
	.type	VP6_DecodeDecide, %function
VP6_DecodeDecide:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 312
	bl	_mcount
	ldr	w0, [x19, 12076]
	cbz	w0, .L405
	ldr	w0, [x19, 12068]
	sub	w0, w0, #2
	cmp	w0, 1
	ldr	x0, [x20]
	ldr	w2, [x0, 8]
	bls	.L406
.L381:
	cmp	w2, 4
	str	w2, [x19, 12068]
	bls	.L407
	ldr	x1, .LC78
	mov	w0, 1
	bl	dprint_vfmw
.L384:
	mov	w0, 0
.L398:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L407:
	ldr	x0, .LC84
	ldrb	w0, [x0,w2,uxtw]
	adr	x1, .Lrtx387
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx387:
	.section	.rodata
	.align	0
	.align	2
.L387:
	.byte	(.L385 - .Lrtx387) / 4
	.byte	(.L385 - .Lrtx387) / 4
	.byte	(.L382 - .Lrtx387) / 4
	.byte	(.L384 - .Lrtx387) / 4
	.byte	(.L385 - .Lrtx387) / 4
	.text
	.p2align 3
.L406:
	cmp	w2, 1
	ccmp	w2, 4, 4, hi
	bne	.L381
	ldr	x1, .LC70
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w1, [x19, 12092]
	mov	w2, 1
	ldr	w0, [x20, 13360]
	bl	FSP_ClearLogicFs
	ldr	w1, [x19, 12096]
	mov	w2, 1
	ldr	w0, [x20, 13360]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 12092]
	str	wzr, [x19, 12096]
	ldr	w0, [x20, 13360]
	bl	FSP_GetNullLogicFs
	str	wzr, [x19, 12072]
	str	w0, [x19, 12092]
	str	w0, [x19, 12096]
	ldr	x0, [x20]
	ldr	w2, [x0, 8]
	b	.L381
	.p2align 3
.L405:
	ldr	x1, .LC68
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L382:
	add	x0, x19, 8192
	ldrb	w2, [x0, 3888]
	cbnz	w2, .L408
.L383:
	ldr	w1, [x19, 12096]
	ldr	w0, [x20, 13360]
	bl	FSP_GetFsImagePtr
	mov	x1, x0
	cbz	x0, .L409
	add	x19, x19, 8192
	mov	w0, 1
	ldrb	w2, [x19, 3888]
	cbz	w2, .L398
	ldr	x3, [x20]
	ldr	w2, [x1, 160]
	ldr	w3, [x3]
	cmp	w2, w3
	bls	.L398
	ldr	x1, .LC83
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L385:
	add	x0, x19, 8192
	ldrb	w0, [x0, 3888]
	cbz	w0, .L383
	ldr	w0, [x19, 12072]
	cbnz	w0, .L383
	ldr	x2, .LC74
	mov	w3, 0
	ldr	x1, .LC76
	mov	w0, 18
	bl	dprint_vfmw
	b	.L384
	.p2align 3
.L408:
	ldr	x1, .LC72
	mov	w0, 18
	bl	dprint_vfmw
	b	.L384
	.p2align 3
.L409:
	ldr	x2, .LC79
	mov	w3, 2145
	ldr	x1, .LC81
	mov	w0, 22
	add	x2, x2, 1992
	bl	dprint_vfmw
	mov	w0, 0
	b	.L398
	.size	VP6_DecodeDecide, .-VP6_DecodeDecide
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
.LC79:
	.xword	.LANCHOR0
	.align	3
.LC81:
	.xword	.LC80
	.align	3
.LC83:
	.xword	.LC82
	.align	3
.LC84:
	.xword	.L387
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_Set_DecParam
	.type	VP6_Set_DecParam, %function
VP6_Set_DecParam:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 312
	add	x19, x20, 16
	bl	_mcount
	add	x0, x21, 8192
	ldrb	w0, [x0, 3888]
	cmp	w0, 3
	beq	.L437
	ldr	x3, .LC89
	add	x23, x20, 12288
	mov	x2, 272
	mov	w1, 0
	mov	x0, x19
	ldr	x24, [x20, 288]
	ldr	x3, [x3, 96]
	blr	x3
	ldr	w1, [x23, 296]
	str	w1, [x19, 116]
	strb	wzr, [x19, 1]
	ldrb	w1, [x23, 276]
	str	w1, [x19, 120]
	ldrb	w1, [x23, 278]
	str	w1, [x19, 124]
	ldr	w1, [x23, 344]
	str	w1, [x19, 128]
	ldr	w1, [x23, 332]
	str	w1, [x19, 140]
	ldrb	w1, [x23, 280]
	str	w1, [x19, 144]
	ldrb	w1, [x23, 281]
	str	w1, [x19, 148]
	ldrb	w1, [x23, 282]
	str	w1, [x19, 152]
	ldr	w1, [x23, 340]
	str	w1, [x19, 164]
	ldrb	w1, [x23, 277]
	strb	w1, [x20, 16]
	ldrh	w1, [x23, 284]
	str	w1, [x19, 132]
	ldrh	w1, [x23, 286]
	str	w1, [x19, 136]
	ldr	w1, [x23, 292]
	str	w1, [x19, 160]
	ldr	w1, [x23, 288]
	str	w1, [x19, 156]
	ldr	w1, [x23, 304]
	strh	w1, [x19, 106]
	ldr	w0, [x23, 300]
	strh	w0, [x19, 108]
	ldr	w1, [x21, 12088]
	ldr	w0, [x20, 13360]
	bl	FSP_GetLogicFs
	mov	x22, x0
	ldr	w1, [x21, 12096]
	ldr	w0, [x20, 13360]
	bl	FSP_GetLogicFs
	mov	x25, x0
	ldr	w1, [x21, 12092]
	ldr	w0, [x20, 13360]
	bl	FSP_GetLogicFs
	cmp	x22, xzr
	ccmp	x25, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L419
	ldr	x1, [x22, 640]
	cbz	x1, .L420
	ldr	w1, [x1, 16]
	str	w1, [x19, 228]
	ldr	w1, [x22, 664]
	str	w1, [x19, 240]
	ldr	x0, [x0, 640]
	cbz	x0, .L438
.L435:
	ldr	w0, [x0, 16]
	str	w0, [x19, 232]
	ldr	x0, [x25, 640]
	cbz	x0, .L439
.L436:
	ldr	w0, [x0, 16]
	add	x26, x23, 424
	str	w0, [x19, 236]
	mov	x0, x26
	ldr	w1, [x21, 12104]
	str	w1, [x19, 244]
	ldr	x1, [x22, 640]
	ldr	w1, [x1, 76]
	str	w1, [x19, 248]
	ldr	x1, [x22, 640]
	ldr	w1, [x1, 60]
	str	w1, [x19, 252]
	ldr	x1, [x22, 640]
	ldr	w1, [x1, 64]
	str	w1, [x19, 256]
	ldr	w1, [x23, 356]
	str	w1, [x19, 216]
	ldr	w1, [x23, 352]
	str	w1, [x19, 220]
	ldr	w1, [x23, 360]
	str	w1, [x19, 224]
	ldr	x22, [x24]
	bl	BsPos
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w0, w1, w0, lt
	asr	w0, w0, 3
	add	x22, x22, x0, sxtw
	mov	x0, x22
	bl	MEM_Vir2Phy
	and	w0, w0, -16
	sub	w1, w0, #16
	mov	x0, x22
	str	w1, [x19, 112]
	bl	MEM_Vir2Phy
	and	w0, w0, 15
	ldrb	w1, [x23, 278]
	cbnz	w1, .L425
	ldr	w1, [x23, 292]
	cbnz	w1, .L426
.L425:
	mov	w1, 16
	lsl	w0, w0, 3
	stp	w0, w1, [x19, 172]
	mov	x0, x26
	ldr	w22, [x23, 328]
	bl	BsPos
	str	wzr, [x19, 188]
	lsl	w22, w22, 3
	sub	w0, w22, w0
	str	w0, [x19, 168]
	stp	wzr, wzr, [x19, 180]
	ldr	w0, [x23, 328]
	ldr	x22, [x24]
	add	x22, x22, x0
	mov	x0, x22
	bl	MEM_Vir2Phy
	mov	w26, w0
	mov	x0, x22
	ubfiz	w26, w26, 3, 4
	bl	MEM_Vir2Phy
	and	w0, w0, -16
	ldr	w1, [x19, 112]
	sub	w0, w0, w1
	stp	w26, w0, [x19, 196]
	ldr	w1, [x23, 328]
	ldr	w0, [x24, 12]
	stp	wzr, wzr, [x19, 204]
	lsl	w0, w0, 3
	str	wzr, [x19, 212]
	sub	w0, w0, w1, lsl 3
	str	w0, [x19, 192]
.L427:
	add	x21, x21, 4096
	add	x1, x20, 3386
	add	x0, x21, 834
	str	x0, [x19, 88]
	add	x0, x21, 706
	str	x1, [x19, 40]
	str	x0, [x19, 80]
	add	x1, x21, 98
	add	x0, x20, 3450
	str	x1, [x19, 64]
	add	x1, x21, 2
	stp	x0, x1, [x19, 48]
	add	x0, x20, 2618
	add	x1, x20, 3322
	str	x0, [x19, 16]
	add	x0, x20, 3258
	str	x1, [x19, 32]
	add	x1, x20, 2554
	str	x0, [x19, 24]
	add	x0, x21, 2370
	str	x1, [x19, 8]
	add	x21, x21, 674
	str	x0, [x19, 96]
	str	x21, [x19, 72]
	ldr	x0, [x20]
	ldr	w1, [x0, 632]
	str	w1, [x19, 260]
	ldr	w0, [x0, 672]
	str	w0, [x19, 264]
	ldr	w0, [x23, 304]
	lsl	w0, w0, 4
	cmp	w0, 1920
	bhi	.L428
	mov	w0, 1
	strb	w0, [x19, 104]
.L429:
	mov	w0, 1
	str	x19, [x20, 8]
.L413:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L419:
	ldr	x1, .LC86
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L437:
	str	w0, [x19, 116]
	mov	w0, 1
	strb	w0, [x19, 1]
	ldr	w1, [x21, 12088]
	ldr	w0, [x20, 13360]
	bl	FSP_GetLogicFs
	mov	x22, x0
	ldr	w1, [x21, 12096]
	ldr	w0, [x20, 13360]
	bl	FSP_GetLogicFs
	mov	x23, x0
	ldr	w1, [x21, 12092]
	ldr	w0, [x20, 13360]
	bl	FSP_GetLogicFs
	cmp	x22, xzr
	ccmp	x23, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L419
	ldr	x1, [x22, 640]
	cbz	x1, .L420
	ldr	w1, [x1, 16]
	str	w1, [x19, 228]
	ldr	w1, [x22, 664]
	str	w1, [x19, 240]
	ldr	x0, [x0, 640]
	cbz	x0, .L440
.L433:
	ldr	w0, [x0, 16]
	str	w0, [x19, 232]
	ldr	x0, [x23, 640]
	cbz	x0, .L441
.L434:
	ldr	w0, [x0, 16]
	str	w0, [x19, 236]
	mov	w0, 1
	str	x19, [x20, 8]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L428:
	strb	wzr, [x19, 104]
	b	.L429
	.p2align 3
.L426:
	lsl	w0, w0, 3
	mov	w1, 16
	stp	w0, w1, [x19, 172]
	mov	x0, x26
	ldr	w22, [x24, 12]
	bl	BsPos
	str	wzr, [x19, 212]
	lsl	w22, w22, 3
	sub	w0, w22, w0
	str	w0, [x19, 168]
	stp	wzr, wzr, [x19, 180]
	stp	wzr, wzr, [x19, 188]
	stp	wzr, wzr, [x19, 196]
	stp	wzr, wzr, [x19, 204]
	b	.L427
	.p2align 3
.L439:
	ldr	x0, [x22, 640]
	b	.L436
	.p2align 3
.L438:
	ldr	x0, [x22, 640]
	b	.L435
	.p2align 3
.L420:
	ldr	x1, .LC88
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L413
	.p2align 3
.L441:
	ldr	x0, [x22, 640]
	b	.L434
	.p2align 3
.L440:
	ldr	x0, [x22, 640]
	b	.L433
	.size	VP6_Set_DecParam, .-VP6_Set_DecParam
	.align	3
.LC86:
	.xword	.LC85
	.align	3
.LC88:
	.xword	.LC87
	.align	3
.LC89:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_CfgTable
	.type	VP6_CfgTable, %function
VP6_CfgTable:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x22, x0
	mov	x0, x30
	mov	x25, x1
	add	x20, x22, 2242
	bl	_mcount
	ldr	x21, .LC90
	stp	xzr, xzr, [x29, 96]
	add	x19, x22, 4
	add	x23, x22, 34
.L443:
	ldr	x3, [x21, 104]
	mov	x1, x19
	mov	x0, x20
	mov	x2, 10
	add	x19, x19, x2
	add	x20, x20, 16
	blr	x3
	cmp	x19, x23
	bne	.L443
	add	x24, x22, 2306
	add	x19, x22, 44
	add	x26, x22, 404
	.p2align 2
.L444:
	add	x23, x19, 90
	mov	x20, x24
	.p2align 2
.L445:
	ldr	x3, [x21, 104]
	mov	x1, x19
	mov	x0, x20
	mov	x2, 9
	add	x19, x19, x2
	add	x20, x20, 16
	blr	x3
	cmp	x23, x19
	bne	.L445
	cmp	x23, x26
	add	x24, x24, 160
	mov	x19, x23
	bne	.L444
	ldr	x3, [x21, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x29, 96
	blr	x3
	ldrb	w5, [x22]
	ldrb	w4, [x22, 2]
	ldrb	w3, [x22, 484]
	ldrb	w0, [x22, 485]
	ldrb	w2, [x22, 486]
	ldrb	w1, [x22, 487]
	strb	w5, [x29, 96]
	strb	w4, [x29, 97]
	ldrb	w5, [x22, 488]
	ldrb	w4, [x22, 489]
	strb	w3, [x29, 98]
	strb	w0, [x29, 99]
	ldr	x3, [x21, 104]
	ldrb	w0, [x22, 490]
	strb	w5, [x29, 102]
	strb	w4, [x29, 103]
	strb	w2, [x29, 100]
	mov	x2, 16
	strb	w1, [x29, 101]
	add	x1, x29, 96
	strb	w0, [x29, 104]
	add	x0, x22, 2946
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x29, 96
	blr	x3
	strb	wzr, [x29, 105]
	ldrb	w5, [x22]
	ldrb	w4, [x22, 498]
	ldrb	w3, [x22, 499]
	ldrb	w0, [x22, 500]
	ldrb	w2, [x22, 501]
	ldrb	w1, [x22, 502]
	strb	w5, [x29, 96]
	strb	w4, [x29, 97]
	ldrb	w5, [x22, 503]
	ldrb	w4, [x22, 504]
	strb	w3, [x29, 98]
	strb	w0, [x29, 99]
	ldr	x3, [x21, 104]
	ldrb	w0, [x22, 505]
	strb	w5, [x29, 102]
	strb	w4, [x29, 103]
	strb	w2, [x29, 100]
	mov	x2, 16
	strb	w1, [x29, 101]
	add	x1, x29, 96
	strb	w0, [x29, 104]
	add	x0, x22, 2962
	strb	wzr, [x29, 106]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x29, 96
	blr	x3
	ldrb	w5, [x22, 1]
	ldrb	w4, [x22, 3]
	ldrb	w3, [x22, 491]
	ldrb	w0, [x22, 492]
	ldrb	w2, [x22, 493]
	ldrb	w1, [x22, 494]
	strb	w5, [x29, 96]
	strb	w4, [x29, 97]
	ldrb	w5, [x22, 495]
	ldrb	w4, [x22, 496]
	strb	w3, [x29, 98]
	strb	w0, [x29, 99]
	ldr	x3, [x21, 104]
	ldrb	w0, [x22, 497]
	strb	w5, [x29, 102]
	strb	w4, [x29, 103]
	strb	w2, [x29, 100]
	mov	x2, 16
	strb	w1, [x29, 101]
	add	x1, x29, 96
	strb	w0, [x29, 104]
	add	x0, x22, 2978
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x29, 96
	blr	x3
	strb	wzr, [x29, 105]
	ldrb	w2, [x22, 1]
	ldrb	w1, [x22, 506]
	ldrb	w3, [x22, 507]
	ldrb	w0, [x22, 508]
	ldrb	w5, [x22, 509]
	ldrb	w4, [x22, 510]
	strb	w2, [x29, 96]
	strb	w1, [x29, 97]
	ldrb	w2, [x22, 511]
	ldrb	w1, [x22, 512]
	strb	w3, [x29, 98]
	strb	w0, [x29, 99]
	ldr	x3, [x21, 104]
	ldrb	w0, [x22, 513]
	strb	w2, [x29, 102]
	mov	x2, 16
	strb	w1, [x29, 103]
	add	x1, x29, 96
	strb	w0, [x29, 104]
	add	x0, x22, 2994
	strb	w5, [x29, 100]
	strb	w4, [x29, 101]
	strb	wzr, [x29, 106]
	blr	x3
	ldr	w0, [x25, 68]
	cbnz	w0, .L447
	add	x20, x22, 4096
	mov	w25, 0
	add	x19, x20, 2
.L448:
	sxtw	x1, w25
	ldr	x3, [x21, 104]
	add	x1, x1, 1152
	mov	x0, x19
	mov	x2, 11
	add	x1, x22, x1
	add	w25, w25, 11
	add	x19, x19, 16
	blr	x3
	cmp	w25, 33
	bne	.L448
	add	x19, x20, 50
.L449:
	sxtw	x1, w25
	ldr	x3, [x21, 104]
	add	x1, x1, 1152
	mov	x0, x19
	mov	x2, 11
	add	x1, x22, x1
	add	w25, w25, 11
	add	x19, x19, 16
	blr	x3
	cmp	w25, 66
	bne	.L449
	ldr	x3, [x21, 104]
	mov	x2, 14
	add	x1, x22, 536
	add	x0, x20, 674
	add	x27, x20, 98
	blr	x3
	ldr	x3, [x21, 104]
	mov	x2, 14
	add	x1, x22, 550
	add	x0, x20, 690
	blr	x3
.L456:
	sub	w23, w25, #66
	mov	x26, x27
	.p2align 2
.L459:
	add	w24, w23, 198
	mov	x20, x26
	mov	w19, w23
.L457:
	add	x1, x22, x19, sxtw
	ldr	x3, [x21, 104]
	mov	x0, x20
	mov	x2, 11
	add	x1, x1, 564
	add	w19, w19, 66
	add	x20, x20, 16
	blr	x3
	cmp	w24, w19
	bne	.L457
	add	w23, w23, 11
	add	x26, x26, 48
	cmp	w25, w23
	bne	.L459
	add	w25, w25, 198
	add	x27, x27, 288
	cmp	w25, 462
	bne	.L456
.L455:
	ldr	x3, [x21, 104]
	add	x1, x22, 960
	mov	x2, 64
	add	x0, x22, 3010
	blr	x3
	ldr	x3, [x21, 104]
	add	x1, x22, 1024
	mov	x2, 64
	add	x0, x22, 3074
	blr	x3
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	mov	w0, 0
	ldp	x29, x30, [sp], 112
	ret
.L447:
	add	x19, x22, 4096
	add	x20, x22, 8192
	ldr	x3, [x21, 104]
	mov	x2, 128
	add	x1, x20, 1308
	add	x0, x19, 706
	add	x25, x19, 1602
	add	x26, x20, 1436
	blr	x3
	add	x27, x19, 3138
	ldr	x3, [x21, 104]
	mov	x2, 128
	add	x1, x20, 3740
	add	x0, x19, 2370
	blr	x3
.L450:
	sub	x19, x25, #768
	mov	x24, x26
.L454:
	add	x23, x19, 256
	mov	x20, x24
.L451:
	ldr	x3, [x21, 104]
	mov	x2, 64
	mov	x1, x20
	mov	x0, x19
	add	x20, x20, x2
	add	x19, x19, x2
	blr	x3
	cmp	x19, x23
	bne	.L451
	cmp	x25, x19
	add	x24, x24, 384
	bne	.L454
	add	x25, x25, 768
	add	x26, x26, 1152
	cmp	x27, x25
	bne	.L450
	ldr	x3, [x21, 104]
	mov	x2, 960
	add	x1, x22, 1218
	add	x0, x22, 3138
	blr	x3
	b	.L455
	.size	VP6_CfgTable, .-VP6_CfgTable
	.align	3
.LC90:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	VP6_BuildTables.part.2, %function
VP6_BuildTables.part.2:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	mov	x23, x2
	bl	_mcount
	ldr	w0, [x20, 20]
	cbz	w0, .L470
	mov	x2, x20
	mov	x1, x23
	mov	x0, x19
	bl	VP6_DecodeModeProbs
	mov	x2, x20
	mov	x1, x23
	mov	x0, x19
	bl	VP6_ConfigureMvEntropyDecoder
	ldr	w3, [x20, 20]
	mov	x0, x19
	mov	x2, x20
	mov	x1, x23
	bl	VP6_ConfigureEntropyDecoder
	ldr	w0, [x20, 68]
	cbnz	w0, .L476
.L472:
	mov	x1, x20
	mov	x0, x19
	bl	VP6_CfgTable
	ldr	x23, [sp, 48]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L470:
	ldr	x22, .LC91
	mov	x2, 80
	ldr	x21, .LC92
	add	x0, x19, 404
	ldr	x3, [x22, 104]
	mov	x1, x21
	blr	x3
	mov	x2, 2
	ldr	x3, [x22, 104]
	add	x1, x21, 80
	add	x0, x19, x2
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 14
	add	x1, x21, 88
	add	x0, x19, 484
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 2
	add	x1, x21, 104
	mov	x0, x19
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 16
	add	x1, x21, 112
	add	x0, x19, 498
	blr	x3
	ldr	x3, [x22, 104]
	mov	x2, 64
	add	x1, x21, 128
	add	x0, x19, 1088
	blr	x3
	mov	x1, x20
	mov	x0, x19
	bl	VP6_BuildScanOrder
	ldr	w3, [x20, 20]
	mov	x0, x19
	mov	x2, x20
	mov	x1, x23
	bl	VP6_ConfigureEntropyDecoder
	ldr	w0, [x20, 68]
	cbz	w0, .L472
.L476:
	mov	x0, x19
	bl	VP6_ConvertBoolToHuff
	b	.L472
	.size	VP6_BuildTables.part.2, .-VP6_BuildTables.part.2
	.align	3
.LC91:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC92:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_BuildTables
	.type	VP6_BuildTables, %function
VP6_BuildTables:
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
	beq	.L479
	mov	x2, x21
	mov	x1, x20
	mov	x0, x19
	bl	VP6_BuildTables.part.2
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L479:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	VP6_BuildTables, .-VP6_BuildTables
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsFrameSizeChanged
	.type	IsFrameSizeChanged, %function
IsFrameSizeChanged:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 12132]
	ldr	w0, [x19, 12276]
	cmp	w1, w0
	beq	.L483
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L483:
	ldr	w0, [x19, 12280]
	ldr	w1, [x19, 12136]
	ldr	x19, [sp, 16]
	cmp	w1, w0
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	IsFrameSizeChanged, .-IsFrameSizeChanged
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6DEC_Decode
	.type	VP6DEC_Decode, %function
VP6DEC_Decode:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	add	x21, x19, 296
	bl	_mcount
	ldr	x1, [x20]
	str	x1, [x19, 296]
	ldr	w0, [x20, 16]
	str	w0, [x21, 8]
	ldr	w2, [x20, 20]
	str	w2, [x21, 12]
	cbnz	w2, .L485
	add	x20, x19, 312
	mov	w1, 3
	add	x0, x20, 8192
	strb	w1, [x0, 3888]
.L486:
	mov	x0, x19
	bl	VP6_DecodeDecide
	cmp	w0, 1
	mov	w21, w0
	beq	.L508
	ldr	x1, .LC107
	mov	w0, 1
	mov	w19, 0
	bl	dprint_vfmw
.L488:
	mov	w0, w19
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L485:
	cmp	w2, 2
	ble	.L530
	add	x23, x19, 12288
	add	x20, x19, 312
	add	x22, x23, 424
	mov	x0, x22
	bl	BsInit
	str	x21, [x19, 288]
	mov	x1, x22
	mov	x0, x20
	bl	VP6_GetPictureHeader
	cmp	w0, 1
	bne	.L531
	add	x1, x20, 8192
	cmp	x20, xzr
	add	x1, x1, 4060
	ccmp	x1, xzr, 4, ne
	ccmp	x22, xzr, 4, ne
	beq	.L491
	mov	x2, x22
	mov	x0, x20
	bl	VP6_BuildTables.part.2
	cmp	w0, 1
	bne	.L491
	ldr	w0, [x20, 12076]
	cbz	w0, .L492
	ldr	w1, [x20, 12132]
	ldr	w0, [x20, 12276]
	cmp	w1, w0
	beq	.L532
.L492:
	ldr	x21, .LC99
	ldr	x4, [x21]
	cbz	x4, .L494
	ldr	w0, [x20, 12132]
	ldr	w3, [x20, 12136]
	ldr	w1, [x20, 12276]
	ldr	w2, [x20, 12280]
	strh	w0, [x29, 74]
	ldr	w0, [x19, 13360]
	strh	w3, [x29, 72]
	mov	w3, 8
	strh	w2, [x29, 76]
	add	x2, x29, 72
	strh	w1, [x29, 78]
	mov	w1, 2
	blr	x4
.L494:
	ldr	w0, [x19, 13360]
	bl	FSP_GetFspType
	cbnz	w0, .L495
	add	x3, x29, 80
	ldr	w0, [x19, 13360]
	add	x2, x29, 68
	add	x1, x29, 64
	stp	wzr, wzr, [x29, 64]
	str	wzr, [x3, -8]!
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19]
	ldr	w2, [x0, 748]
	cmp	w2, 1
	bne	.L495
	ldr	w0, [x29, 72]
	cmp	w0, wzr
	bgt	.L498
	ldr	w0, [x29, 68]
	cmp	w0, 32
	bgt	.L498
	ldr	x0, [x19, 8]
	cbnz	x0, .L498
	ldr	w3, [x20, 12136]
	cbz	w3, .L495
	ldr	w0, [x19, 13364]
	ldr	w6, [x20, 12276]
	cmp	w6, w0
	beq	.L495
	ldr	w0, [x19, 13368]
	ldr	w5, [x20, 12280]
	cmp	w5, w0
	beq	.L495
	ldr	w4, [x20, 12132]
	mov	w0, w2
	ldr	x1, .LC101
	bl	dprint_vfmw
	ldr	w0, [x20, 12276]
	str	w0, [x19, 13364]
	ldr	x4, [x21]
	ldr	w0, [x20, 12280]
	str	w0, [x19, 13368]
	cbz	x4, .L495
	ldr	w0, [x19, 13360]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	.p2align 2
.L495:
	ldr	w0, [x19, 13360]
	mov	w2, 1
	ldr	w1, [x20, 12092]
	add	x23, x23, 464
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 12096]
	mov	w2, 1
	ldr	w0, [x19, 13360]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 12092]
	str	wzr, [x20, 12096]
	ldr	w0, [x19, 13360]
	bl	FSP_GetNullLogicFs
	str	wzr, [x20, 12072]
	str	w0, [x20, 12092]
	mov	x1, x23
	str	w0, [x20, 12096]
	ldr	w0, [x19, 13360]
	bl	FSP_ClearNotInVoQueue
	ldr	w2, [x20, 12280]
	ldr	w3, [x20, 12276]
	cmp	w2, 512
	bhi	.L502
	cmp	w3, 512
	bhi	.L502
	ldr	x0, [x19]
	ldr	w1, [x0, 748]
	cbnz	w1, .L504
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L533
.L504:
	ldr	w0, [x19, 13360]
	bl	FSP_GetFspType
	cbz	w0, .L529
.L506:
	mov	x0, x19
	bl	VP6_ArrangeVHBMem
	cmp	w0, 1
	beq	.L486
	ldr	x1, .LC105
	mov	w0, 1
	mov	w19, 0
	bl	dprint_vfmw
	b	.L488
	.p2align 3
.L508:
	mov	x0, x19
	bl	VP6_GetImageBuffer
	cmp	w0, 1
	mov	w22, w0
	beq	.L509
	ldr	x1, .LC109
	mov	w0, w21
	mov	w19, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L530:
	ldr	x1, .LC94
	mov	w0, 1
	mov	w19, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L531:
	ldr	x1, .LC96
	mov	w0, 1
	mov	w19, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L509:
	mov	w2, w0
	ldr	w1, [x20, 12088]
	ldr	w0, [x19, 13360]
	bl	FSP_SetRef
	ldr	w0, [x19, 13360]
	ldr	w1, [x20, 12088]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L534
	ldr	x3, [x19]
	mov	x4, -1
	ldr	x1, .LC112
	mov	w0, 29
	ldr	x3, [x3, 56]
	str	x3, [x2, 600]
	ldr	x3, [x19]
	ldr	x3, [x3, 72]
	str	x3, [x2, 608]
	ldr	x3, [x19]
	ldr	x3, [x3, 88]
	str	x3, [x2, 616]
	ldr	x3, [x19]
	ldr	w3, [x3, 104]
	str	w3, [x2, 8]
	ldr	x3, [x19]
	ldr	w3, [x3, 108]
	str	w3, [x2, 12]
	ldr	x3, [x19]
	ldr	w3, [x3, 112]
	str	w3, [x2, 16]
	ldr	x3, [x19]
	ldr	w3, [x3, 128]
	str	w3, [x2, 20]
	str	wzr, [x2, 96]
	ldr	x3, [x19]
	str	x4, [x3, 56]
	ldr	x3, [x2, 608]
	ldr	x2, [x2, 600]
	bl	dprint_vfmw
	mov	x0, x19
	bl	VP6_Set_DecParam
	cmp	w0, 1
	mov	w19, w0
	beq	.L511
	ldr	x1, .LC114
	mov	w0, w22
	mov	w19, 0
	bl	dprint_vfmw
	b	.L488
	.p2align 3
.L491:
	ldr	x1, .LC98
	mov	w0, 1
	mov	w19, 0
	bl	dprint_vfmw
	b	.L488
	.p2align 3
.L511:
	add	x0, x20, 8192
	ldr	x3, .LC115
	mov	x2, 144
	ldrb	w1, [x0, 3888]
	cbz	w1, .L535
	ldr	x3, [x3, 104]
	add	x1, x0, 4060
	add	x0, x0, 3916
	ldr	w22, [x20, 12120]
	ldr	w21, [x20, 12124]
	blr	x3
	str	w22, [x20, 12120]
	str	w21, [x20, 12124]
.L513:
	ldr	x1, .LC117
	mov	w0, 22
	bl	dprint_vfmw
	b	.L488
	.p2align 3
.L502:
	ldr	x1, .LC103
	mov	w0, 18
	mov	w19, 0
	bl	dprint_vfmw
	b	.L488
	.p2align 3
.L532:
	ldr	w1, [x20, 12136]
	ldr	w0, [x20, 12280]
	cmp	w1, w0
	beq	.L486
	b	.L492
	.p2align 3
.L533:
	ldr	w0, [x20, 12076]
	cbnz	w0, .L506
.L529:
	mov	x0, x23
	bl	ResetVoQueue
	b	.L506
	.p2align 3
.L535:
	ldr	x3, [x3, 104]
	add	x1, x0, 4060
	add	x0, x0, 3916
	blr	x3
	b	.L513
.L534:
	ldr	x1, .LC110
	mov	w2, 2779
	mov	w0, 22
	mov	w19, 0
	bl	dprint_vfmw
	b	.L488
.L498:
	mov	w19, -2
	b	.L488
	.size	VP6DEC_Decode, .-VP6DEC_Decode
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
.LC99:
	.xword	g_event_report
	.align	3
.LC101:
	.xword	.LC100
	.align	3
.LC103:
	.xword	.LC102
	.align	3
.LC105:
	.xword	.LC104
	.align	3
.LC107:
	.xword	.LC106
	.align	3
.LC109:
	.xword	.LC108
	.align	3
.LC110:
	.xword	.LC11
	.align	3
.LC112:
	.xword	.LC111
	.align	3
.LC114:
	.xword	.LC113
	.align	3
.LC115:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC117:
	.xword	.LC116
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6_SetImgFormat
	.type	VP6_SetImgFormat, %function
VP6_SetImgFormat:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 312
	bl	_mcount
	ldr	w0, [x21, 13360]
	ldr	w1, [x20, 12088]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L536
	add	x0, x20, 8192
	mov	w1, 7200
	ldrb	w0, [x0, 3888]
	and	w0, w0, 3
	orr	w2, w0, w1
	str	w2, [x19, 108]
	ldr	w2, [x20, 12280]
	lsl	w2, w2, 4
	cmp	w2, 1920
	bls	.L547
	orr	w1, w0, w1
	str	w1, [x19, 108]
	mov	w2, 0
	ldr	x3, [x21]
	ldr	w3, [x3, 632]
	cmp	w3, 1
	beq	.L548
.L540:
	orr	w1, w1, w2
	mov	w3, 8
	stp	w0, w0, [x19, 100]
	mov	w0, 10
	mov	w2, 1
	str	w1, [x19, 108]
	mov	w1, 0
	strb	w2, [x19, 371]
	strb	w2, [x19, 369]
	str	w0, [x19, 376]
	mov	x0, x19
	strb	wzr, [x19, 364]
	str	w3, [x19, 380]
	str	w3, [x19, 384]
	str	w3, [x19, 388]
	bl	SetAspectRatio
	ldr	w2, [x20, 12280]
	lsl	w2, w2, 4
	str	w2, [x19, 112]
	ldr	w1, [x20, 12276]
	lsl	w1, w1, 4
	str	w1, [x19, 116]
	ldr	x0, [x21]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L549
.L541:
	lsr	w3, w2, 1
	lsr	w0, w1, 1
	stp	w2, w1, [x19, 120]
	stp	w3, w0, [x19, 128]
.L536:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L547:
	add	w1, w1, 131072
	mov	w2, 0
	orr	w1, w0, w1
	str	w1, [x19, 108]
	ldr	x3, [x21]
	ldr	w3, [x3, 632]
	cmp	w3, 1
	bne	.L540
.L548:
	and	w3, w1, 131072
	mov	w2, 262144
	cmp	w3, wzr
	csel	w2, wzr, w2, ne
	b	.L540
	.p2align 3
.L549:
	ldr	w3, [x19, 140]
	lsr	w2, w1, 4
	ldr	w0, [x19, 44]
	lsl	w1, w3, 4
	ldr	x4, [x19, 504]
	add	w5, w3, w0
	str	w0, [x19, 28]
	mul	w1, w2, w1
	str	w0, [x19, 60]
	add	w0, w1, w0
	str	w5, [x19, 36]
	add	x1, x4, x1, sxtw
	add	w3, w3, w0
	str	x1, [x19, 512]
	str	w3, [x19, 40]
	str	x4, [x19, 520]
	str	w0, [x19, 32]
	str	w0, [x19, 48]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	ldr	w3, [x19, 32]
	ldp	w2, w1, [x19, 112]
	str	w3, [x19, 64]
	b	.L541
	.size	VP6_SetImgFormat, .-VP6_SetImgFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6DEC_VDMPostProc
	.type	VP6DEC_VDMPostProc, %function
VP6DEC_VDMPostProc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w21, w1
	add	x19, x20, 312
	bl	_mcount
	ldr	x1, .LC119
	mov	w2, w21
	mov	w0, 22
	bl	dprint_vfmw
	mov	x1, x20
	mov	w0, 10
	bl	ReleasePacket
	ldr	w1, [x19, 12088]
	ldr	w0, [x20, 13360]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L583
	tbnz	w21, #31, .L553
	cmp	w21, 100
	mov	w2, 100
	csel	w2, w21, w2, le
	str	w2, [x0, 160]
	ldr	x0, [x20]
	ldr	w3, [x0]
	cmp	w2, w3
	bhi	.L584
	mov	x0, x20
	bl	VP6_SetImgFormat
	ldr	w0, [x19, 12068]
	mov	w1, 1
	str	w1, [x19, 12072]
	cmp	w0, 2
	beq	.L585
.L555:
	ldr	w1, [x19, 12088]
	str	w1, [x19, 12100]
.L556:
	cmn	w1, #1
	beq	.L557
	ldr	w0, [x20, 13360]
	mov	w2, 1
	bl	FSP_SetDisplay
	ldr	w1, [x19, 12100]
	ldr	w0, [x20, 13360]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L586
	mov	x4, x0
	add	x3, x20, 12288
	ldr	w0, [x20, 13360]
	add	x3, x3, 464
	mov	x2, x20
	mov	w1, 10
	bl	InsertImgToVoQueue
	cmp	w0, 1
	mov	w21, w0
	bne	.L587
.L557:
	ldr	w0, [x19, 12092]
	ldr	w3, [x19, 12096]
	cmp	w3, w0
	beq	.L588
.L582:
	ldr	w1, [x19, 12088]
	cmp	w3, w1
	beq	.L560
	ldr	w0, [x20, 13360]
	mov	w2, 0
	mov	w1, w3
	bl	FSP_SetRef
.L580:
	ldr	w1, [x19, 12088]
.L560:
	str	w1, [x19, 12096]
	mov	w2, 1
	ldr	w0, [x20, 13360]
	bl	FSP_SetRef
	add	x0, x19, 8192
	ldrb	w0, [x0, 3888]
	cbnz	w0, .L589
	ldr	w1, [x19, 12088]
	ldr	w3, [x19, 12092]
	cmp	w3, w1
	beq	.L563
.L590:
	ldr	w0, [x20, 13360]
	mov	w1, w3
	mov	w2, 0
	bl	FSP_SetRef
	ldr	w1, [x19, 12088]
.L563:
	str	w1, [x19, 12092]
	mov	w2, 1
	ldr	w0, [x20, 13360]
	bl	FSP_SetRef
	mov	w0, 1
.L573:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L589:
	ldr	w1, [x19, 12084]
	mov	w0, 1
	cbz	w1, .L573
	ldr	w1, [x19, 12088]
	ldr	w3, [x19, 12092]
	cmp	w3, w1
	bne	.L590
	b	.L563
	.p2align 3
.L553:
	str	wzr, [x0, 160]
	mov	x0, x20
	bl	VP6_SetImgFormat
	ldr	w0, [x19, 12068]
	mov	w1, 1
	str	w1, [x19, 12072]
	cmp	w0, 2
	bne	.L555
.L585:
	add	x0, x19, 8192
	ldrb	w0, [x0, 3888]
	cbz	w0, .L555
	mov	w0, -1
	str	w0, [x19, 12100]
	ldr	w1, [x19, 12088]
	mov	w2, 0
	ldr	w0, [x20, 13360]
	bl	FSP_SetDisplay
	ldr	w1, [x19, 12100]
	b	.L556
	.p2align 3
.L587:
	ldr	w0, [x20, 13360]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	ldr	x1, [x22, 568]
	ldr	w0, [x20, 13360]
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	mov	w2, 0
	ldr	w1, [x19, 12100]
	ldr	w0, [x20, 13360]
	bl	FSP_SetDisplay
	ldr	x1, .LC127
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L588:
	ldr	w0, [x20, 13360]
	mov	w1, w3
	bl	FSP_GetRef
	cbnz	w0, .L580
	ldr	w3, [x19, 12096]
	b	.L582
	.p2align 3
.L584:
	ldr	x1, .LC123
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 12092]
	mov	w2, 1
	ldr	w0, [x20, 13360]
	bl	FSP_ClearLogicFs
	ldr	w1, [x19, 12096]
	mov	w2, 1
	ldr	w0, [x20, 13360]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 12092]
	str	wzr, [x19, 12096]
	ldr	w0, [x20, 13360]
	bl	FSP_GetNullLogicFs
	str	wzr, [x19, 12072]
	str	w0, [x19, 12092]
	str	w0, [x19, 12096]
	mov	w0, -1
	b	.L573
.L586:
	ldr	x1, .LC125
	mov	x2, 0
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L573
.L583:
	ldr	x1, .LC121
	bl	dprint_vfmw
	mov	w0, -1
	b	.L573
	.size	VP6DEC_VDMPostProc, .-VP6DEC_VDMPostProc
	.align	3
.LC119:
	.xword	.LC118
	.align	3
.LC121:
	.xword	.LC120
	.align	3
.LC123:
	.xword	.LC122
	.align	3
.LC125:
	.xword	.LC124
	.align	3
.LC127:
	.xword	.LC126
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6DEC_RecycleImage
	.type	VP6DEC_RecycleImage, %function
VP6DEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L593
	ldr	w0, [x19, 13360]
	mov	w1, w20
	mov	w2, 0
	bl	FSP_SetDisplay
	ldr	w0, [x19, 13360]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L594
	ldr	w0, [x19, 13360]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 13360]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L593:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L594:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6DEC_RecycleImage, .-VP6DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP6DEC_GetRemainImg
	.type	VP6DEC_GetRemainImg, %function
VP6DEC_GetRemainImg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 12288
	add	x0, x0, 464
	bl	GetVoLastImageID
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP6DEC_GetRemainImg, .-VP6DEC_GetRemainImg
	.global	DefaultScanBands
	.global	ZrlUpdateProbs
	.global	ScanBandUpdateProbs
	.global	ZeroRunProbDefaults
	.global	VP6_AcUpdateProbs
	.global	VP6_DcUpdateProbs
	.global	DefaultSignProbs
	.global	DefaultIsShortProbs
	.global	DefaultMvLongProbs
	.global	DefaultMvShortProbs
	.global	VP6_MvUpdateProbs
	.global	VP6_BaselineXmittedProbs
	.global	VP6_ModeVq
	.global	VP6_DcNodeEqs
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	VP6_DcNodeEqs, %object
	.size	VP6_DcNodeEqs, 120
VP6_DcNodeEqs:
	.word	122
	.word	133
	.word	133
	.word	51
	.word	142
	.word	-16
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	1
	.word	78
	.word	171
	.word	169
	.word	71
	.word	221
	.word	-30
	.word	139
	.word	117
	.word	214
	.word	44
	.word	246
	.word	-3
	.word	168
	.word	79
	.word	210
	.word	38
	.word	203
	.word	17
	.type	VP6_DcUpdateProbs, %object
	.size	VP6_DcUpdateProbs, 22
VP6_DcUpdateProbs:
	.byte	-110
	.byte	-1
	.byte	-75
	.byte	-49
	.byte	-24
	.byte	-13
	.byte	-18
	.byte	-5
	.byte	-12
	.byte	-6
	.byte	-7
	.byte	-77
	.byte	-1
	.byte	-42
	.byte	-16
	.byte	-6
	.byte	-1
	.byte	-12
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.zero	2
	.type	ZeroRunProbDefaults, %object
	.size	ZeroRunProbDefaults, 28
ZeroRunProbDefaults:
	.byte	-58
	.byte	-59
	.byte	-60
	.byte	-110
	.byte	-58
	.byte	-52
	.byte	-87
	.byte	-114
	.byte	-126
	.byte	-120
	.byte	-107
	.byte	-107
	.byte	-65
	.byte	-7
	.byte	-121
	.byte	-55
	.byte	-75
	.byte	-102
	.byte	98
	.byte	117
	.byte	-124
	.byte	126
	.byte	-110
	.byte	-87
	.byte	-72
	.byte	-16
	.byte	-10
	.byte	-2
	.zero	4
	.type	ScanBandUpdateProbs, %object
	.size	ScanBandUpdateProbs, 64
ScanBandUpdateProbs:
	.byte	-1
	.byte	-124
	.byte	-124
	.byte	-97
	.byte	-103
	.byte	-105
	.byte	-95
	.byte	-86
	.byte	-92
	.byte	-94
	.byte	-120
	.byte	110
	.byte	103
	.byte	114
	.byte	-127
	.byte	118
	.byte	124
	.byte	125
	.byte	-124
	.byte	-120
	.byte	114
	.byte	110
	.byte	-114
	.byte	-121
	.byte	-122
	.byte	123
	.byte	-113
	.byte	126
	.byte	-103
	.byte	-73
	.byte	-90
	.byte	-95
	.byte	-85
	.byte	-76
	.byte	-77
	.byte	-92
	.byte	-53
	.byte	-38
	.byte	-31
	.byte	-39
	.byte	-41
	.byte	-50
	.byte	-53
	.byte	-39
	.byte	-27
	.byte	-15
	.byte	-8
	.byte	-13
	.byte	-3
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.type	ZrlUpdateProbs, %object
	.size	ZrlUpdateProbs, 28
ZrlUpdateProbs:
	.byte	-37
	.byte	-10
	.byte	-18
	.byte	-7
	.byte	-24
	.byte	-17
	.byte	-7
	.byte	-1
	.byte	-8
	.byte	-3
	.byte	-17
	.byte	-12
	.byte	-15
	.byte	-8
	.byte	-58
	.byte	-24
	.byte	-5
	.byte	-3
	.byte	-37
	.byte	-15
	.byte	-3
	.byte	-1
	.byte	-8
	.byte	-7
	.byte	-12
	.byte	-18
	.byte	-5
	.byte	-1
	.zero	4
	.type	VP6_AcUpdateProbs, %object
	.size	VP6_AcUpdateProbs, 396
VP6_AcUpdateProbs:
	.byte	-29
	.byte	-10
	.byte	-26
	.byte	-9
	.byte	-12
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-47
	.byte	-25
	.byte	-25
	.byte	-7
	.byte	-7
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-31
	.byte	-14
	.byte	-15
	.byte	-5
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-15
	.byte	-3
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-16
	.byte	-1
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-16
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-50
	.byte	-53
	.byte	-29
	.byte	-17
	.byte	-9
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-49
	.byte	-57
	.byte	-36
	.byte	-20
	.byte	-13
	.byte	-4
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-44
	.byte	-37
	.byte	-26
	.byte	-13
	.byte	-12
	.byte	-3
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-20
	.byte	-19
	.byte	-9
	.byte	-4
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-16
	.byte	-16
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-26
	.byte	-23
	.byte	-7
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-18
	.byte	-18
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-5
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-31
	.byte	-17
	.byte	-29
	.byte	-25
	.byte	-12
	.byte	-3
	.byte	-13
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-24
	.byte	-22
	.byte	-32
	.byte	-28
	.byte	-14
	.byte	-7
	.byte	-14
	.byte	-4
	.byte	-5
	.byte	-5
	.byte	-1
	.byte	-21
	.byte	-7
	.byte	-18
	.byte	-16
	.byte	-5
	.byte	-1
	.byte	-7
	.byte	-1
	.byte	-3
	.byte	-3
	.byte	-1
	.byte	-7
	.byte	-3
	.byte	-5
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-6
	.byte	-7
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-13
	.byte	-12
	.byte	-6
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-8
	.byte	-6
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.zero	4
	.type	VP6_ModeVq, %object
	.size	VP6_ModeVq, 960
VP6_ModeVq:
	.byte	9
	.byte	15
	.byte	32
	.byte	25
	.byte	7
	.byte	19
	.byte	9
	.byte	21
	.byte	1
	.byte	12
	.byte	14
	.byte	12
	.byte	3
	.byte	18
	.byte	14
	.byte	23
	.byte	3
	.byte	10
	.byte	0
	.byte	4
	.byte	48
	.byte	39
	.byte	1
	.byte	2
	.byte	11
	.byte	27
	.byte	29
	.byte	44
	.byte	7
	.byte	27
	.byte	1
	.byte	4
	.byte	0
	.byte	3
	.byte	1
	.byte	6
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	21
	.byte	32
	.byte	1
	.byte	2
	.byte	4
	.byte	10
	.byte	32
	.byte	43
	.byte	6
	.byte	23
	.byte	2
	.byte	3
	.byte	1
	.byte	19
	.byte	1
	.byte	6
	.byte	12
	.byte	21
	.byte	0
	.byte	7
	.byte	69
	.byte	83
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	10
	.byte	29
	.byte	3
	.byte	12
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	0
	.byte	3
	.byte	2
	.byte	2
	.byte	0
	.byte	0
	.byte	11
	.byte	20
	.byte	1
	.byte	4
	.byte	18
	.byte	36
	.byte	43
	.byte	48
	.byte	13
	.byte	35
	.byte	0
	.byte	2
	.byte	0
	.byte	5
	.byte	3
	.byte	12
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	70
	.byte	44
	.byte	0
	.byte	1
	.byte	2
	.byte	10
	.byte	37
	.byte	46
	.byte	8
	.byte	26
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	8
	.byte	15
	.byte	0
	.byte	1
	.byte	8
	.byte	21
	.byte	74
	.byte	53
	.byte	22
	.byte	42
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	3
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	-115
	.byte	42
	.byte	0
	.byte	0
	.byte	1
	.byte	4
	.byte	11
	.byte	24
	.byte	1
	.byte	11
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	8
	.byte	19
	.byte	4
	.byte	10
	.byte	24
	.byte	45
	.byte	21
	.byte	37
	.byte	9
	.byte	29
	.byte	0
	.byte	3
	.byte	1
	.byte	7
	.byte	11
	.byte	25
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	46
	.byte	42
	.byte	0
	.byte	1
	.byte	2
	.byte	10
	.byte	54
	.byte	51
	.byte	10
	.byte	30
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	28
	.byte	32
	.byte	0
	.byte	0
	.byte	3
	.byte	10
	.byte	75
	.byte	51
	.byte	14
	.byte	33
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	100
	.byte	46
	.byte	0
	.byte	1
	.byte	3
	.byte	9
	.byte	21
	.byte	37
	.byte	5
	.byte	20
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	1
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	27
	.byte	29
	.byte	0
	.byte	1
	.byte	9
	.byte	25
	.byte	53
	.byte	51
	.byte	12
	.byte	34
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	1
	.byte	5
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	80
	.byte	38
	.byte	0
	.byte	0
	.byte	1
	.byte	4
	.byte	69
	.byte	33
	.byte	5
	.byte	16
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	16
	.byte	20
	.byte	0
	.byte	0
	.byte	2
	.byte	8
	.byte	104
	.byte	49
	.byte	15
	.byte	33
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-62
	.byte	16
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	9
	.byte	1
	.byte	3
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	41
	.byte	22
	.byte	1
	.byte	0
	.byte	1
	.byte	31
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	7
	.byte	0
	.byte	1
	.byte	98
	.byte	25
	.byte	4
	.byte	10
	.byte	123
	.byte	37
	.byte	6
	.byte	4
	.byte	1
	.byte	27
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	5
	.byte	8
	.byte	1
	.byte	7
	.byte	0
	.byte	1
	.byte	12
	.byte	10
	.byte	0
	.byte	2
	.byte	26
	.byte	14
	.byte	14
	.byte	12
	.byte	0
	.byte	24
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	55
	.byte	17
	.byte	1
	.byte	9
	.byte	0
	.byte	36
	.byte	5
	.byte	7
	.byte	1
	.byte	3
	.byte	-47
	.byte	5
	.byte	0
	.byte	0
	.byte	0
	.byte	27
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	5
	.byte	4
	.byte	5
	.byte	0
	.byte	121
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	2
	.byte	4
	.byte	1
	.byte	4
	.byte	2
	.byte	2
	.byte	0
	.byte	1
	.byte	-81
	.byte	5
	.byte	0
	.byte	1
	.byte	0
	.byte	48
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	83
	.byte	5
	.byte	2
	.byte	3
	.byte	0
	.byte	102
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	3
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	-23
	.byte	6
	.byte	0
	.byte	0
	.byte	0
	.byte	8
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	34
	.byte	16
	.byte	112
	.byte	21
	.byte	1
	.byte	28
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	6
	.byte	8
	.byte	1
	.byte	7
	.byte	0
	.byte	3
	.byte	2
	.byte	5
	.byte	0
	.byte	2
	.byte	-97
	.byte	35
	.byte	2
	.byte	2
	.byte	0
	.byte	25
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	6
	.byte	0
	.byte	5
	.byte	0
	.byte	1
	.byte	4
	.byte	4
	.byte	0
	.byte	1
	.byte	75
	.byte	39
	.byte	5
	.byte	7
	.byte	2
	.byte	48
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	11
	.byte	2
	.byte	16
	.byte	1
	.byte	4
	.byte	7
	.byte	10
	.byte	0
	.byte	2
	.byte	-44
	.byte	21
	.byte	0
	.byte	1
	.byte	0
	.byte	9
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	2
	.byte	2
	.byte	0
	.byte	0
	.byte	4
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	-84
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	-69
	.byte	22
	.byte	1
	.byte	1
	.byte	0
	.byte	17
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	6
	.byte	0
	.byte	4
	.byte	0
	.byte	1
	.byte	4
	.byte	4
	.byte	0
	.byte	1
	.byte	-123
	.byte	6
	.byte	1
	.byte	2
	.byte	1
	.byte	70
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	4
	.byte	0
	.byte	3
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-5
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	3
	.byte	2
	.byte	3
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	11
	.byte	4
	.byte	1
	.byte	4
	.byte	0
	.byte	2
	.byte	3
	.byte	2
	.byte	0
	.byte	4
	.byte	49
	.byte	46
	.byte	3
	.byte	4
	.byte	7
	.byte	31
	.byte	42
	.byte	41
	.byte	0
	.byte	0
	.byte	2
	.byte	6
	.byte	1
	.byte	7
	.byte	1
	.byte	4
	.byte	2
	.byte	4
	.byte	0
	.byte	1
	.byte	26
	.byte	25
	.byte	1
	.byte	1
	.byte	2
	.byte	10
	.byte	67
	.byte	39
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	0
	.byte	14
	.byte	0
	.byte	2
	.byte	31
	.byte	26
	.byte	1
	.byte	6
	.byte	103
	.byte	46
	.byte	1
	.byte	2
	.byte	2
	.byte	10
	.byte	33
	.byte	42
	.byte	0
	.byte	0
	.byte	1
	.byte	4
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	1
	.byte	3
	.byte	0
	.byte	0
	.byte	14
	.byte	31
	.byte	9
	.byte	13
	.byte	14
	.byte	54
	.byte	22
	.byte	29
	.byte	0
	.byte	0
	.byte	2
	.byte	6
	.byte	4
	.byte	18
	.byte	6
	.byte	13
	.byte	1
	.byte	5
	.byte	0
	.byte	1
	.byte	85
	.byte	39
	.byte	0
	.byte	0
	.byte	1
	.byte	9
	.byte	69
	.byte	40
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	0
	.byte	0
	.byte	31
	.byte	28
	.byte	0
	.byte	0
	.byte	3
	.byte	14
	.byte	-126
	.byte	34
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	3
	.byte	3
	.byte	0
	.byte	1
	.byte	-85
	.byte	25
	.byte	0
	.byte	0
	.byte	1
	.byte	5
	.byte	25
	.byte	21
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	17
	.byte	21
	.byte	68
	.byte	29
	.byte	6
	.byte	15
	.byte	13
	.byte	22
	.byte	0
	.byte	0
	.byte	6
	.byte	12
	.byte	3
	.byte	14
	.byte	4
	.byte	10
	.byte	1
	.byte	7
	.byte	0
	.byte	3
	.byte	51
	.byte	39
	.byte	0
	.byte	1
	.byte	2
	.byte	12
	.byte	91
	.byte	44
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	0
	.byte	1
	.byte	81
	.byte	25
	.byte	0
	.byte	0
	.byte	2
	.byte	9
	.byte	106
	.byte	26
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-116
	.byte	37
	.byte	0
	.byte	1
	.byte	1
	.byte	8
	.byte	24
	.byte	33
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	14
	.byte	23
	.byte	1
	.byte	3
	.byte	11
	.byte	53
	.byte	90
	.byte	31
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	1
	.byte	5
	.byte	2
	.byte	6
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	123
	.byte	29
	.byte	0
	.byte	0
	.byte	1
	.byte	7
	.byte	57
	.byte	30
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	13
	.byte	14
	.byte	0
	.byte	0
	.byte	4
	.byte	20
	.byte	-81
	.byte	20
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-54
	.byte	23
	.byte	0
	.byte	0
	.byte	1
	.byte	3
	.byte	2
	.byte	9
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.type	VP6_MvUpdateProbs, %object
	.size	VP6_MvUpdateProbs, 34
VP6_MvUpdateProbs:
	.byte	-19
	.byte	-10
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-6
	.byte	-6
	.byte	-4
	.byte	-25
	.byte	-13
	.byte	-11
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-5
	.byte	-5
	.byte	-2
	.zero	6
	.type	VP6_LoopFilterLimitValues, %object
	.size	VP6_LoopFilterLimitValues, 64
VP6_LoopFilterLimitValues:
	.byte	14
	.byte	14
	.byte	13
	.byte	13
	.byte	12
	.byte	12
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.type	DcQuantTab, %object
	.size	DcQuantTab, 128
DcQuantTab:
	.hword	188
	.hword	188
	.hword	188
	.hword	188
	.hword	180
	.hword	172
	.hword	172
	.hword	172
	.hword	172
	.hword	172
	.hword	168
	.hword	164
	.hword	164
	.hword	160
	.hword	160
	.hword	160
	.hword	160
	.hword	140
	.hword	140
	.hword	140
	.hword	140
	.hword	132
	.hword	132
	.hword	132
	.hword	132
	.hword	128
	.hword	128
	.hword	128
	.hword	108
	.hword	108
	.hword	104
	.hword	104
	.hword	100
	.hword	100
	.hword	96
	.hword	96
	.hword	92
	.hword	92
	.hword	76
	.hword	76
	.hword	76
	.hword	76
	.hword	72
	.hword	72
	.hword	68
	.hword	64
	.hword	64
	.hword	64
	.hword	64
	.hword	64
	.hword	60
	.hword	44
	.hword	44
	.hword	44
	.hword	40
	.hword	40
	.hword	36
	.hword	32
	.hword	28
	.hword	20
	.hword	12
	.hword	12
	.hword	8
	.hword	8
	.type	AcQuantTab, %object
	.size	AcQuantTab, 128
AcQuantTab:
	.hword	376
	.hword	368
	.hword	360
	.hword	352
	.hword	344
	.hword	328
	.hword	312
	.hword	296
	.hword	280
	.hword	264
	.hword	248
	.hword	232
	.hword	216
	.hword	212
	.hword	208
	.hword	204
	.hword	200
	.hword	196
	.hword	192
	.hword	188
	.hword	184
	.hword	180
	.hword	176
	.hword	172
	.hword	168
	.hword	160
	.hword	156
	.hword	148
	.hword	144
	.hword	140
	.hword	136
	.hword	132
	.hword	128
	.hword	124
	.hword	120
	.hword	116
	.hword	112
	.hword	108
	.hword	104
	.hword	100
	.hword	96
	.hword	92
	.hword	88
	.hword	84
	.hword	80
	.hword	76
	.hword	72
	.hword	68
	.hword	64
	.hword	60
	.hword	56
	.hword	52
	.hword	48
	.hword	44
	.hword	40
	.hword	36
	.hword	32
	.hword	28
	.hword	24
	.hword	20
	.hword	16
	.hword	12
	.hword	8
	.hword	4
	.type	__FUNCTION__.11936, %object
	.size	__FUNCTION__.11936, 17
__FUNCTION__.11936:
	.string	"VP6_DecodeDecide"
	.align	3
.LANCHOR1 = . + 0
	.type	VP6_BaselineXmittedProbs, %object
	.size	VP6_BaselineXmittedProbs, 80
VP6_BaselineXmittedProbs:
	.byte	42
	.byte	2
	.byte	7
	.byte	42
	.byte	22
	.byte	3
	.byte	2
	.byte	5
	.byte	1
	.byte	0
	.byte	69
	.byte	1
	.byte	1
	.byte	44
	.byte	6
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	8
	.byte	1
	.byte	8
	.byte	0
	.byte	0
	.byte	2
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	-27
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	35
	.byte	1
	.byte	6
	.byte	34
	.byte	0
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	122
	.byte	1
	.byte	1
	.byte	46
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	64
	.byte	0
	.byte	64
	.byte	64
	.byte	64
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	64
	.byte	0
	.byte	64
	.byte	64
	.byte	64
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.type	DefaultIsShortProbs, %object
	.size	DefaultIsShortProbs, 2
DefaultIsShortProbs:
	.byte	-94
	.byte	-92
	.zero	6
	.type	DefaultMvShortProbs, %object
	.size	DefaultMvShortProbs, 14
DefaultMvShortProbs:
	.byte	-31
	.byte	-110
	.byte	-84
	.byte	-109
	.byte	-42
	.byte	39
	.byte	-100
	.byte	-52
	.byte	-86
	.byte	119
	.byte	-21
	.byte	-116
	.byte	-26
	.byte	-28
	.zero	2
	.type	DefaultSignProbs, %object
	.size	DefaultSignProbs, 2
DefaultSignProbs:
	.byte	-128
	.byte	-128
	.zero	6
	.type	DefaultMvLongProbs, %object
	.size	DefaultMvLongProbs, 16
DefaultMvLongProbs:
	.byte	-9
	.byte	-46
	.byte	-121
	.byte	68
	.byte	-118
	.byte	-36
	.byte	-17
	.byte	-10
	.byte	-12
	.byte	-72
	.byte	-55
	.byte	44
	.byte	-83
	.byte	-35
	.byte	-17
	.byte	-3
	.type	DefaultScanBands, %object
	.size	DefaultScanBands, 64
DefaultScanBands:
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	3
	.byte	3
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	6
	.byte	6
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	8
	.byte	8
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	10
	.byte	10
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	12
	.byte	12
	.byte	12
	.byte	12
	.byte	12
	.byte	12
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	14
	.byte	14
	.byte	14
	.byte	14
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"image_size_in_mb abnormal(%d x %d)\n" )
	.zero	4
.LC2:
	ASCII(.string	"vp6 actual frame size(%dx%d) exeed max config(%dx%d)\n" )
	.zero	2
.LC5:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC7:
	ASCII(.string	"FSP_ConfigInstance fail!\n" )
	.zero	6
.LC9:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC11:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC13:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC15:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC36:
	ASCII(.string	"enter VP6_ConvertBoolToHuff\n" )
	.zero	3
.LC38:
	ASCII(.string	"leave VP6_ConvertBoolToHuff\n" )
	.zero	3
.LC40:
	ASCII(.string	"enter VP6_IntraFrmHeader\n" )
	.zero	6
.LC42:
	ASCII(.string	"error: Vp3VersionNo %d > CURRENT_DECODE_VERSION(8)\n" )
	.zero	4
.LC44:
	ASCII(.string	"error: bitstream error!\n" )
	.zero	7
.LC46:
	ASCII(.string	"We have a new size image!\n The old size is %d x %d, and the new is %d x %d\n" )
	.zero	4
.LC48:
	ASCII(.string	"leave VP6_IntraFrmHeader\n" )
	.zero	6
.LC50:
	ASCII(.string	"VP6_InterFrmHeader VersionNo fault,old:%d,new:%d\n" )
	.zero	6
.LC52:
	ASCII(.string	"[ERR]VP6_GetPictureHeader invalid param!\n" )
	.zero	6
.LC54:
	ASCII(.string	"VP6_GetPictureHeader enter!\n" )
	.zero	3
.LC57:
	ASCII(.string	"VP6_GetPictureHeader leave!\n" )
	.zero	3
.LC63:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC65:
	ASCII(.string	"VP6DEC_Init leave\n" )
	.zero	5
.LC67:
	ASCII(.string	"ERROR: No Img buffer is allocated\n" )
	.zero	5
.LC69:
	ASCII(.string	"----------------- vp6 Imode -> %d ------------------\n" )
	.zero	2
.LC71:
	ASCII(.string	"-----I_MODE, but FrameType is %d-----\n" )
	.zero	1
.LC73:
	ASCII(.string	"P-frm" )
	.zero	2
.LC75:
	ASCII(.string	"-----cur framtype is %s, but refnum=%d-----\n" )
	.zero	3
.LC77:
	ASCII(.string	"error: DecMode illegal\n" )
.LC80:
	ASCII(.string	"%s %d NULL == pFwdImg!!\n" )
	.zero	7
.LC82:
	ASCII(.string	"dec P frame, fwd ref err(%d) > ref_thr(%d)\n" )
	.zero	4
.LC85:
	ASCII(.string	"VP6_Set_DecParam but logic fs is null!\n" )
.LC87:
	ASCII(.string	"Decord FS is NULL!\n" )
	.zero	4
.LC93:
	ASCII(.string	"Error! Truncated packet\n" )
	.zero	7
.LC95:
	ASCII(.string	"[ERR]VP6_GetPictureHeader Error!\n" )
	.zero	6
.LC97:
	ASCII(.string	"[ERR] VP6_DecodeDecide Error!\n" )
	.zero	1
.LC100:
	ASCII(.string	"IsSizeChanged = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	4
.LC102:
	ASCII(.string	"pic width/height to large,MbWidth = %d, MbHeight = %d" )
	.zero	2
.LC104:
	ASCII(.string	"VP6_ArrangeVHBMem err\n" )
	.zero	1
.LC106:
	ASCII(.string	"VP6_DecodeDecide err\n" )
	.zero	2
.LC108:
	ASCII(.string	"VP6DEC_GetImageBuffer err\n" )
	.zero	5
.LC111:
	ASCII(.string	"dec_pts: %lld Usertag = %lld\n" )
	.zero	2
.LC113:
	ASCII(.string	"VP6_PrePare_decode Error!\n" )
	.zero	5
.LC116:
	ASCII(.string	"leave VP6DEC_Decode\n" )
	.zero	3
.LC118:
	ASCII(.string	"ErrRatio = %d\n" )
	.zero	1
.LC120:
	ASCII(.string	"pCurImg is null\n" )
	.zero	7
.LC122:
	ASCII(.string	"err_level(%d) over ref_thr(%d)\n" )
.LC124:
	ASCII(.string	"line: %d pToQueImg is null!\n" )
	.zero	3
.LC126:
	ASCII(.string	"insert img to Voqueue failed!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
