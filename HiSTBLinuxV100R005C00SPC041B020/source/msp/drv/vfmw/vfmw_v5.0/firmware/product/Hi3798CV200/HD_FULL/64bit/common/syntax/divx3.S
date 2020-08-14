#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"divx3.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	Divx3_GetImageBuffer
	.type	Divx3_GetImageBuffer, %function
Divx3_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 40960
	add	x19, x19, 192
	bl	_mcount
	add	x19, x19, 40960
	mov	w1, 1
	ldr	w0, [x20, 1248]
	bl	FSP_NewLogicFs
	str	w0, [x19, 396]
	mov	w1, w0
	tbnz	w0, #31, .L9
	ldr	w0, [x20, 1248]
	bl	FSP_GetLogicFs
	mov	x20, x0
	cbz	x0, .L10
	ldr	x1, .LC5
	mov	w0, 18
	ldr	w2, [x19, 396]
	bl	dprint_vfmw
	ldr	x0, [x20, 640]
	cbz	x0, .L7
	ldr	x1, [x20, 648]
	cbz	x1, .L7
	ldr	x2, [x20, 656]
	cbz	x2, .L7
	ldr	w3, [x1, 16]
	ldr	w4, [x2, 16]
	ldr	x1, .LC7
	ldr	w2, [x0, 16]
	mov	w0, 18
	bl	dprint_vfmw
	mov	w0, 1
.L3:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L7:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L9:
	ldr	x1, .LC1
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x20, 1248]
	add	x1, x20, 640
	bl	FSP_ClearNotInVoQueue
	ldr	w1, [x19, 392]
	mov	w2, 1
	ldr	w0, [x20, 1248]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 392]
	ldr	w0, [x20, 1248]
	bl	FSP_GetNullLogicFs
	str	wzr, [x19, 308]
	str	w0, [x19, 392]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L10:
	ldr	x1, .LC3
	mov	w2, 79
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L3
	.size	Divx3_GetImageBuffer, .-Divx3_GetImageBuffer
	.align	3
.LC1:
	.xword	.LC0
	.align	3
.LC3:
	.xword	.LC2
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC7:
	.xword	.LC6
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Divx3_ArrangeVHBMem
	.type	Divx3_ArrangeVHBMem, %function
Divx3_ArrangeVHBMem:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x2
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldp	w2, w3, [x19, 48]
	cmp	w2, 1
	bls	.L13
	cmp	w3, 1
	bls	.L13
	ldr	x1, [x20]
	ldr	w0, [x1, 708]
	cbnz	w0, .L16
	ldr	w0, [x1, 28]
	cmp	w0, 25
	beq	.L41
.L17:
	ldp	w22, w21, [x19, 56]
	mov	w24, 5
	mov	w23, 4
	mov	w19, 2
.L22:
	ldr	x0, .LC12
	mov	x2, 32
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x29, 80
	blr	x3
	strb	wzr, [x29, 81]
	ldr	x0, [x20]
	stp	w22, w21, [x29, 88]
	ldr	w0, [x0, 28]
	cmp	w0, 24
	beq	.L28
	strb	w23, [x29, 83]
	sxtb	w0, w19
	strb	w24, [x29, 85]
.L32:
	add	x19, x20, 40960
	strb	w0, [x29, 86]
	add	x1, x29, 80
	strb	wzr, [x29, 82]
	ldr	w0, [x19, 1248]
	bl	FSP_ConfigInstance
	mov	w1, 0
	cbz	w0, .L42
.L37:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, w1
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L16:
	ldr	w0, [x1, 28]
	cmp	w0, 25
	bne	.L17
	ldr	w4, [x1, 668]
	ldp	w2, w3, [x19, 56]
	ldr	w5, [x1, 672]
	cmp	w2, w4
	bhi	.L25
	cmp	w3, w5
	bhi	.L25
	ldr	w4, [x1, 696]
	mov	w22, w2
	ldr	w0, [x1, 692]
	mov	w21, w3
	mov	w19, 1
	add	w23, w0, 2
	cbnz	w4, .L40
.L27:
	ldrsb	w24, [x1, 716]
	b	.L22
	.p2align 3
.L13:
	ldr	x1, .LC9
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, w1
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L42:
	ldr	x1, [x20]
	add	x3, x29, 76
	ldr	w0, [x19, 1248]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L43
	cbnz	w0, .L30
	mov	w0, 1
	strb	w0, [x20, 194]
	mov	w1, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, w1
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L41:
	ldr	w22, [x1, 668]
	ldp	w2, w3, [x19, 56]
	cmp	w2, w22
	bhi	.L44
	ldr	w21, [x1, 672]
	cmp	w3, w21
	bhi	.L45
	ldrb	w0, [x20, 194]
	cmp	w0, 1
	beq	.L33
	ldr	w2, [x1, 696]
	mov	w19, 1
	ldr	w0, [x1, 692]
	add	w23, w0, 2
	cbz	w2, .L27
.L40:
	add	w0, w0, w19
	mov	w19, 17
	cmp	w0, w19
	ldrsb	w24, [x1, 716]
	csel	w19, w0, w19, le
	b	.L22
	.p2align 3
.L28:
	mov	w0, 1
	strb	w24, [x29, 85]
	strb	w0, [x29, 83]
	mov	w0, 1
	b	.L32
	.p2align 3
.L25:
	ldr	x1, .LC11
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L37
	.p2align 3
.L43:
	add	x0, x19, 640
	bl	ResetVoQueue
.L30:
	ldr	x1, .LC14
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L37
.L33:
	mov	w1, w0
	b	.L37
	.p2align 3
.L45:
	mov	w5, w21
.L19:
	ldr	x1, .LC11
	mov	w4, w22
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L37
	.p2align 3
.L44:
	ldr	w5, [x1, 672]
	b	.L19
	.size	Divx3_ArrangeVHBMem, .-Divx3_ArrangeVHBMem
	.align	3
.LC9:
	.xword	.LC8
	.align	3
.LC11:
	.xword	.LC10
	.align	3
.LC12:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC14:
	.xword	.LC13
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Divx3_SetImgFormat
	.type	Divx3_SetImgFormat, %function
Divx3_SetImgFormat:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 40960
	bl	_mcount
	add	x1, x21, 192
	add	x1, x1, 40960
	ldr	w0, [x20, 1248]
	ldr	w1, [x1, 396]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L46
	ldr	w3, [x20, 376]
	mov	w2, 7200
	mov	w4, 262144
	mov	w1, 0
	and	w3, w3, 3
	orr	w2, w3, w2
	str	w2, [x0, 108]
	ldr	x5, [x21]
	ldr	w5, [x5, 592]
	stp	wzr, w3, [x0, 96]
	cmp	w5, 1
	str	w3, [x0, 104]
	csel	w3, w4, wzr, eq
	orr	w2, w3, w2
	str	w2, [x0, 108]
	ldr	w2, [x20, 424]
	lsl	w2, w2, 4
	str	w2, [x0, 112]
	ldr	w2, [x20, 428]
	lsl	w2, w2, 4
	str	w2, [x0, 116]
	bl	SetAspectRatio
	ldr	x0, [x21]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L54
.L49:
	mov	w1, 1
	strb	w1, [x19, 371]
	strb	w1, [x19, 369]
	add	x1, x19, 256
	strb	wzr, [x19, 364]
	mov	w0, 8
	mov	w2, 5
	stp	w0, w0, [x1, 128]
	stp	w2, w0, [x1, 120]
.L46:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L54:
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
	b	.L49
	.size	Divx3_SetImgFormat, .-Divx3_SetImgFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Divx3_RecordVFMWState
	.type	Divx3_RecordVFMWState, %function
Divx3_RecordVFMWState:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	Divx3_RecordVFMWState, .-Divx3_RecordVFMWState
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Divx3_WritePicMsg
	.type	Divx3_WritePicMsg, %function
Divx3_WritePicMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 40960
	add	x1, x2, 376
	ldr	w2, [x2, 376]
	str	w2, [x19, 20]
	ldr	w2, [x1, 48]
	str	w2, [x19, 24]
	ldr	w2, [x1, 52]
	str	w2, [x19, 28]
	ldr	w2, [x1, 4]
	str	w2, [x19, 32]
	ldr	w2, [x1, 8]
	str	w2, [x19, 36]
	ldr	w2, [x1, 16]
	str	w2, [x19, 40]
	ldr	w2, [x1, 24]
	str	w2, [x19, 44]
	ldr	w2, [x1, 28]
	str	w2, [x19, 48]
	ldr	w2, [x1, 32]
	str	w2, [x19, 52]
	ldr	w2, [x1, 36]
	str	w2, [x19, 56]
	ldr	w2, [x1, 40]
	str	w2, [x19, 60]
	ldr	w1, [x1, 44]
	str	w1, [x19, 64]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Divx3_WritePicMsg, .-Divx3_WritePicMsg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Divx3_WriteReg
	.type	Divx3_WriteReg, %function
Divx3_WriteReg:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	Divx3_WriteReg, .-Divx3_WriteReg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Divx3_DecodeDecide
	.type	Divx3_DecodeDecide, %function
Divx3_DecodeDecide:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 192
	bl	_mcount
	ldrb	w0, [x19, 2]
	cbz	w0, .L87
	ldrb	w0, [x19, 3]
	add	x21, x19, 40960
	sub	w0, w0, #2
	ldr	w22, [x21, 184]
	uxtb	w0, w0
	cmp	w0, 1
	ldr	x0, [x20]
	ldr	w2, [x0, 8]
	bls	.L88
.L62:
	uxtb	w2, w2
	strb	w2, [x19, 3]
	cmp	w2, 4
	bls	.L89
	ldr	x1, .LC26
	mov	w0, 1
	bl	dprint_vfmw
.L65:
	mov	w0, 0
.L83:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L89:
	ldr	x0, .LC32
	ldrb	w0, [x0,w2,uxtw]
	adr	x1, .Lrtx70
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx70:
	.section	.rodata
	.align	0
	.align	2
.L70:
	.byte	(.L66 - .Lrtx70) / 4
	.byte	(.L66 - .Lrtx70) / 4
	.byte	(.L63 - .Lrtx70) / 4
	.byte	(.L65 - .Lrtx70) / 4
	.byte	(.L66 - .Lrtx70) / 4
	.text
	.p2align 3
.L88:
	cmp	w2, 1
	ccmp	w2, 4, 4, hi
	bne	.L62
	ldr	x1, .LC18
	add	x23, x20, 40960
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w1, [x21, 392]
	mov	w2, 1
	ldr	w0, [x23, 1248]
	bl	FSP_ClearLogicFs
	str	wzr, [x21, 392]
	ldr	w0, [x23, 1248]
	bl	FSP_GetNullLogicFs
	str	wzr, [x21, 308]
	str	w0, [x21, 392]
	ldr	x0, [x20]
	ldr	w2, [x0, 8]
	b	.L62
	.p2align 3
.L87:
	ldr	x1, .LC16
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L63:
	cbnz	w22, .L90
.L64:
	add	x19, x19, 40960
	add	x20, x20, 40960
	ldr	w1, [x19, 392]
	ldr	w0, [x20, 1248]
	bl	FSP_GetFsImagePtr
.L85:
	mov	w0, 1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L66:
	cbz	w22, .L64
	add	x19, x19, 40960
	ldr	w0, [x19, 308]
	cbz	w0, .L91
	add	x0, x20, 40960
	ldr	w1, [x19, 392]
	ldr	w0, [x0, 1248]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L92
	ldr	x1, [x20]
	ldr	w2, [x0, 160]
	ldr	w3, [x1]
	cmp	w2, w3
	bls	.L85
	ldr	x1, .LC31
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L90:
	ldr	x1, .LC20
	mov	w2, w22
	mov	w0, 18
	bl	dprint_vfmw
	b	.L65
	.p2align 3
.L91:
	ldr	x2, .LC22
	mov	w3, 0
	ldr	x1, .LC24
	mov	w0, 18
	bl	dprint_vfmw
	b	.L65
	.p2align 3
.L92:
	ldr	x2, .LC27
	mov	w3, 445
	ldr	x1, .LC29
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L83
	.size	Divx3_DecodeDecide, .-Divx3_DecodeDecide
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
.LC27:
	.xword	.LANCHOR0
	.align	3
.LC29:
	.xword	.LC28
	.align	3
.LC31:
	.xword	.LC30
	.align	3
.LC32:
	.xword	.L70
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Divx3_GetPictureHeader
	.type	Divx3_GetPictureHeader, %function
Divx3_GetPictureHeader:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	mov	w21, w2
	bl	_mcount
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x19]
	bls	.L94
.L97:
	mov	w1, 0
.L95:
	mov	w0, w1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L94:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 4]
	ldr	w1, [x19]
	cbz	w1, .L115
	str	w21, [x19, 20]
	cbz	w21, .L108
	ldr	w1, [x19, 8]
	mov	w0, 1
	sub	w0, w0, w1
	str	w0, [x19, 8]
.L109:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 16]
	mov	w1, 2
	mov	x0, x20
	bl	BsShow
	cmp	w0, 3
	str	w0, [x19, 36]
	bhi	.L110
	ldr	x1, .LC35
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx112
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx112:
	.section	.rodata
	.align	0
	.align	2
.L112:
	.byte	(.L111 - .Lrtx112) / 4
	.byte	(.L111 - .Lrtx112) / 4
	.byte	(.L114 - .Lrtx112) / 4
	.byte	(.L114 - .Lrtx112) / 4
	.text
	.p2align 3
.L114:
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
.L110:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 40]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 44]
	mov	w1, 1
	mov	w0, w1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L108:
	str	wzr, [x19, 8]
	b	.L109
	.p2align 3
.L115:
	mov	w0, 1
	mov	w1, 5
	str	w0, [x19, 8]
	mov	x0, x20
	bl	BsGet
	sub	w1, w0, #23
	cmp	w1, 1
	str	w0, [x19, 12]
	ccmp	w0, 26, 4, hi
	bne	.L97
	mov	w1, 2
	mov	x0, x20
	bl	BsShow
	cmp	w0, 3
	str	w0, [x19, 24]
	bhi	.L98
	ldr	x1, .LC33
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx100
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx100:
	.section	.rodata
	.align	0
	.align	2
.L100:
	.byte	(.L99 - .Lrtx100) / 4
	.byte	(.L99 - .Lrtx100) / 4
	.byte	(.L102 - .Lrtx100) / 4
	.byte	(.L102 - .Lrtx100) / 4
	.text
	.p2align 3
.L111:
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	b	.L110
.L102:
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
.L98:
	mov	w1, 2
	mov	x0, x20
	bl	BsShow
	cmp	w0, 3
	str	w0, [x19, 28]
	bhi	.L103
	ldr	x1, .LC34
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx105
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx105:
	.section	.rodata
	.align	0
	.align	2
.L105:
	.byte	(.L104 - .Lrtx105) / 4
	.byte	(.L104 - .Lrtx105) / 4
	.byte	(.L107 - .Lrtx105) / 4
	.byte	(.L107 - .Lrtx105) / 4
	.text
.L107:
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
.L103:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 32]
	mov	w1, 1
	b	.L95
.L99:
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	b	.L98
.L104:
	mov	w1, 1
	mov	x0, x20
	bl	BsSkip
	b	.L103
	.size	Divx3_GetPictureHeader, .-Divx3_GetPictureHeader
	.align	3
.LC33:
	.xword	.L100
	.align	3
.LC34:
	.xword	.L105
	.align	3
.LC35:
	.xword	.L112
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3DEC_Init
	.type	DIVX3DEC_Init, %function
DIVX3DEC_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	add	x20, x19, 40960
	bl	_mcount
	ldr	x3, .LC36
	mov	x2, 42224
	mov	w1, 0
	mov	x0, x19
	ldr	w22, [x20, 1240]
	ldr	x3, [x3, 96]
	blr	x3
	str	w22, [x20, 1240]
	add	x0, x20, 640
	bl	ResetVoQueue
	mov	x1, x19
	mov	w3, 1
	mov	x0, x19
	str	x21, [x1], 192
	add	x2, x1, 40960
	str	w3, [x2, 280]
	ldr	w2, [x21, 8]
	strb	w2, [x1, 3]
	strb	wzr, [x1, 2]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x20, 1248]
	beq	.L119
	mov	w0, 0
.L117:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L119:
	ldr	x1, .LC38
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L117
	.size	DIVX3DEC_Init, .-DIVX3DEC_Init
	.align	3
.LC36:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC38:
	.xword	.LC37
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3DEC_Destroy
	.type	DIVX3DEC_Destroy, %function
DIVX3DEC_Destroy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 192
	add	x19, x19, 40960
	bl	_mcount
	ldr	x1, .LC40
	mov	w0, 2
	ldr	w2, [x19, 304]
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	DIVX3DEC_Destroy, .-DIVX3DEC_Destroy
	.align	3
.LC40:
	.xword	.LC39
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3DEC_Decode
	.type	DIVX3DEC_Decode, %function
DIVX3DEC_Decode:
	stp	x29, x30, [sp, -272]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	str	wzr, [x29, 144]
	add	x0, x29, 256
	str	wzr, [x29, 168]
	stp	xzr, xzr, [x29, 176]
	stp	xzr, xzr, [x29, 192]
	stp	xzr, xzr, [x0]
	add	x0, x29, 152
	stp	xzr, xzr, [x29, 208]
	stp	xzr, xzr, [x29, 224]
	stp	xzr, xzr, [x29, 240]
	stp	xzr, xzr, [x29, 128]
	stp	xzr, xzr, [x0]
	cbz	x19, .L213
	ldr	w0, [x19, 12]
	cbz	w0, .L214
	ldr	x0, [x19]
	cbz	x0, .L215
	ldr	w0, [x19, 16]
	cmp	w0, 9
	ble	.L216
	ldr	x24, .LC51
	add	x23, x21, 16
	add	x20, x21, 192
	mov	x2, 172
	mov	w1, 0
	mov	x0, x23
	ldr	x3, [x24, 96]
	blr	x3
	str	wzr, [x20, 152]
	add	x0, x20, 40960
	ldr	w1, [x19, 16]
	str	w1, [x0, 288]
	ldr	w0, [x19, 12]
	str	w0, [x20, 156]
	ldr	x0, [x19]
	str	x0, [x20, 56]
	ldr	w2, [x19, 16]
	str	w2, [x20, 160]
	ldrb	w0, [x19, 9]
	str	w0, [x20, 164]
	ldrb	w0, [x19, 9]
	cmp	w0, 1
	beq	.L127
	ldr	w0, [x19, 92]
	str	w0, [x20, 168]
	ldr	x0, [x19, 80]
	str	x0, [x20, 64]
	ldr	w0, [x19, 96]
	str	w0, [x20, 172]
	ldrb	w0, [x19, 89]
	str	w0, [x20, 176]
	ldrb	w0, [x19, 89]
	cmp	w0, 1
	bne	.L217
	mov	w0, 65525
	sub	w1, w2, #10
	movk	w0, 0x2f, lsl 16
	mov	w22, 0
	cmp	w1, w0
	bhi	.L123
.L224:
	ldr	x1, [x20, 56]
	cbz	x1, .L123
	add	x26, x21, 40960
	add	x25, x26, 600
	mov	x0, x25
	bl	BsInit
	mov	x0, x25
	bl	BsResidBits
	mov	w28, w0
	mov	w1, 32
	mov	x0, x25
	bl	BsGet
	rev	w27, w0
	add	w2, w27, 15
	mov	w1, 32
	lsr	w2, w2, 4
	mov	x0, x25
	str	w2, [x29, 224]
	str	w27, [x29, 232]
	bl	BsGet
	rev	w3, w0
	sub	w0, w27, #32
	cmp	w0, 1888
	add	w0, w3, 15
	lsr	w0, w0, 4
	sub	w1, w3, #32
	str	w0, [x29, 228]
	mov	w0, 1056
	str	w3, [x29, 236]
	ccmp	w1, w0, 2, ls
	bhi	.L129
	add	x27, x20, 40960
	ldr	w0, [x26, 384]
	str	w0, [x29, 184]
	add	x1, x29, 176
	mov	x0, x25
	ldr	w2, [x27, 280]
	bl	Divx3_GetPictureHeader
	cmp	w0, 1
	mov	w22, w0
	bne	.L218
	ldr	w1, [x27, 308]
	cbz	w1, .L219
.L132:
	ldp	w0, w1, [x29, 224]
	ldrb	w2, [x20, 2]
	mul	w3, w0, w1
	str	w3, [x29, 240]
	cbz	w2, .L133
	ldr	w3, [x26, 424]
	cmp	w0, w3
	beq	.L220
.L133:
	ldr	x27, .LC58
	ldr	x4, [x27]
	cbz	x4, .L135
	ldr	w2, [x26, 424]
	mov	w3, 8
	strh	w0, [x29, 124]
	strh	w1, [x29, 126]
	ldr	w0, [x26, 1248]
	ldr	w1, [x26, 428]
	strh	w2, [x29, 120]
	add	x2, x29, 120
	strh	w1, [x29, 122]
	mov	w1, 2
	blr	x4
.L135:
	ldr	w0, [x26, 1248]
	bl	FSP_GetFspType
	cbnz	w0, .L136
	add	x3, x29, 272
	ldr	w0, [x26, 1248]
	add	x2, x29, 116
	add	x1, x29, 112
	stp	wzr, wzr, [x29, 112]
	str	wzr, [x3, -152]!
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x21]
	ldr	w2, [x0, 708]
	cmp	w2, 1
	beq	.L221
.L136:
	add	x3, x20, 40960
	add	x1, x26, 640
	mov	x27, x3
	ldr	w0, [x26, 1248]
	str	x1, [x29, 104]
	mov	w2, 1
	ldr	w1, [x3, 392]
	bl	FSP_ClearLogicFs
	str	wzr, [x27, 392]
	ldr	w0, [x26, 1248]
	bl	FSP_GetNullLogicFs
	str	wzr, [x27, 308]
	str	w0, [x27, 392]
	add	x1, x26, 640
	ldr	w0, [x26, 1248]
	bl	FSP_ClearNotInVoQueue
	ldp	w2, w3, [x29, 224]
	cmp	w2, 512
	bhi	.L143
	cmp	w3, 512
	bhi	.L143
	ldr	x0, [x21]
	ldr	w1, [x0, 708]
	cbnz	w1, .L145
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L222
.L145:
	ldr	w0, [x26, 1248]
	bl	FSP_GetFspType
	cbz	w0, .L147
.L211:
	ldp	w2, w3, [x29, 224]
.L146:
	ldr	x1, .LC64
	mov	w0, 1
	bl	dprint_vfmw
	add	x2, x29, 176
	mov	x1, x21
	mov	w0, 6
	bl	Divx3_ArrangeVHBMem
	cmp	w0, 1
	bne	.L223
.L134:
	ldr	w0, [x29, 176]
	mov	w27, 23
	cbz	w0, .L148
	ldr	w27, [x26, 388]
.L148:
	ldr	x3, [x24, 104]
	add	x24, x26, 376
	add	x1, x29, 176
	mov	x2, 96
	mov	x0, x24
	blr	x3
	ldr	w0, [x29, 176]
	cbz	w0, .L149
	str	w27, [x26, 388]
.L149:
	mov	x0, x21
	bl	Divx3_DecodeDecide
	cmp	w0, 1
	mov	w24, w0
	beq	.L150
	ldr	x1, .LC68
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
	.p2align 3
.L216:
	ldr	x1, .LC50
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
.L123:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 272
	ret
	.p2align 3
.L217:
	add	x21, x21, 40960
	ldr	w1, [x19, 20]
	mov	w22, 0
	ldr	w0, [x21, 1248]
	bl	SM_ReleaseStreamSeg
	ldr	w1, [x19, 100]
	ldr	w0, [x21, 1248]
	bl	SM_ReleaseStreamSeg
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 272
	ret
	.p2align 3
.L214:
	ldr	x1, .LC46
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 272
	ret
	.p2align 3
.L127:
	mov	w0, 65525
	sub	w1, w2, #10
	movk	w0, 0x2f, lsl 16
	str	xzr, [x20, 64]
	stp	wzr, wzr, [x20, 168]
	str	wzr, [x20, 176]
	cmp	w1, w0
	mov	w22, 0
	bls	.L224
	b	.L123
	.p2align 3
.L129:
	ldr	x1, .LC53
	mov	w2, w27
	mov	w0, 1
	bl	dprint_vfmw
	b	.L123
	.p2align 3
.L219:
	ldr	w1, [x29, 176]
	cbz	w1, .L132
	ldr	x1, .LC57
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
.L222:
	ldrb	w0, [x20, 2]
	cbnz	w0, .L146
.L147:
	add	x0, x26, 640
	bl	ResetVoQueue
	b	.L211
	.p2align 3
.L213:
	add	x21, x21, 192
	ldr	x1, .LC44
	add	x21, x21, 40960
	mov	w0, 1
	mov	w22, 0
	ldr	w2, [x21, 304]
	bl	dprint_vfmw
	b	.L123
	.p2align 3
.L215:
	ldr	x1, .LC48
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
	.p2align 3
.L218:
	ldr	x1, .LC55
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
	.p2align 3
.L143:
	ldr	x1, .LC62
	mov	w0, 18
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
.L223:
	ldr	x1, .LC66
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
.L220:
	ldr	w2, [x26, 428]
	cmp	w1, w2
	bne	.L133
	b	.L134
.L221:
	ldr	w0, [x29, 120]
	cmp	w0, wzr
	bgt	.L139
	ldr	w0, [x29, 116]
	cmp	w0, 32
	bgt	.L139
	ldr	x0, [x21, 8]
	cbnz	x0, .L139
	ldr	w3, [x26, 492]
	cbz	w3, .L136
	ldr	w0, [x26, 424]
	cbz	w0, .L136
	ldr	w5, [x26, 1252]
	ldr	w7, [x29, 228]
	cmp	w7, w5
	beq	.L136
	ldr	w4, [x26, 1256]
	ldr	w6, [x29, 224]
	cmp	w6, w4
	beq	.L136
	ldr	x1, .LC60
	mov	w0, w2
	bl	dprint_vfmw
	ldr	w0, [x29, 228]
	str	w0, [x26, 1252]
	ldr	x4, [x27]
	ldr	w0, [x29, 224]
	str	w0, [x26, 1256]
	cbz	x4, .L136
	ldr	w0, [x26, 1248]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L136
	.p2align 3
.L150:
	mov	x0, x21
	bl	Divx3_GetImageBuffer
	cbz	w0, .L225
	add	x27, x20, 40960
	ldr	w0, [x26, 1248]
	ldr	w1, [x27, 396]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L152
	ldr	x0, [x21]
	mov	x4, -1
	ldr	x1, .LC72
	ldr	x0, [x0, 56]
	str	x0, [x2, 600]
	mov	w0, 29
	ldr	x3, [x21]
	ldr	x3, [x3, 72]
	str	x3, [x2, 608]
	ldr	x3, [x21]
	ldr	x3, [x3, 88]
	str	x3, [x2, 616]
	ldr	x3, [x21]
	ldr	w3, [x3, 104]
	str	w3, [x2, 8]
	ldr	x3, [x21]
	ldr	w3, [x3, 108]
	str	w3, [x2, 12]
	ldr	x3, [x21]
	ldr	w3, [x3, 112]
	str	w3, [x2, 16]
	ldr	x3, [x21]
	ldr	w3, [x3, 128]
	str	w3, [x2, 20]
	ldr	x3, [x21]
	str	x4, [x3, 56]
	ldr	x3, [x2, 608]
	ldr	x2, [x2, 600]
	bl	dprint_vfmw
	ldr	w1, [x27, 396]
	ldr	w0, [x26, 1248]
	bl	FSP_GetLogicFs
	mov	x24, x0
	ldr	w1, [x27, 392]
	ldr	w0, [x26, 1248]
	bl	FSP_GetLogicFs
	cmp	x24, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L153
	ldr	x1, [x24, 640]
	cbz	x1, .L153
	ldr	x2, [x24, 656]
	cbz	x2, .L153
	ldr	x2, [x24, 648]
	cbz	x2, .L153
	ldr	x0, [x0, 640]
	cbz	x0, .L156
	ldr	w4, [x0, 16]
.L157:
	str	w4, [x23, 124]
	ldr	w6, [x23, 160]
	ldr	x0, [x24, 640]
	ldr	x1, .LC78
	ldr	w2, [x0, 16]
	str	w2, [x23, 128]
	ldr	w5, [x24, 664]
	str	w5, [x23, 156]
	ldr	x0, [x24, 648]
	ldr	w3, [x0, 16]
	mov	w0, 13
	str	w3, [x23, 132]
	bl	dprint_vfmw
	ldr	x1, [x24, 648]
	mov	x0, x25
	ldr	w1, [x1, 32]
	str	w1, [x23, 136]
	ldr	x1, [x24, 648]
	ldr	w1, [x1, 76]
	str	w1, [x23, 140]
	ldr	x1, [x24, 648]
	ldr	w1, [x1, 60]
	str	w1, [x23, 144]
	ldr	x1, [x24, 648]
	ldr	w1, [x1, 64]
	str	w1, [x23, 148]
	bl	BsResidBits
	ldr	w1, [x19, 12]
	and	w1, w1, -16
	str	w1, [x23, 96]
	ldr	w1, [x19, 12]
	ubfiz	w1, w1, 3, 4
	add	w1, w1, w28
	sub	w1, w1, w0
	mov	x0, x25
	lsr	w2, w1, 7
	and	w1, w1, 127
	lsl	w2, w2, 4
	str	w1, [x23, 108]
	str	w2, [x23, 100]
	bl	BsResidBits
	str	wzr, [x23, 104]
	add	x1, x26, 376
	str	wzr, [x23, 120]
	stp	wzr, w0, [x23, 112]
	ldr	w0, [x1, 12]
	cmp	w0, 24
	beq	.L159
	cmp	w0, 26
	beq	.L160
	cmp	w0, 23
	beq	.L226
	ldr	x1, .LC80
	mov	w0, 1
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
.L225:
	add	x20, x20, 40960
	ldr	x1, .LC70
	mov	w22, 0
	ldr	w2, [x20, 304]
	bl	dprint_vfmw
	b	.L123
.L139:
	mov	w22, -2
	b	.L123
.L153:
	add	x20, x20, 40960
	ldr	w0, [x26, 1248]
	mov	w2, 1
	mov	w22, 0
	ldr	w1, [x20, 396]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC76
	mov	w0, 0
	bl	dprint_vfmw
	b	.L123
.L152:
	ldr	x1, .LC74
	mov	w0, w24
	mov	w22, 0
	bl	dprint_vfmw
	b	.L123
.L226:
	ldr	w0, [x1, 64]
	mov	w2, 1
	str	wzr, [x29, 128]
	sub	w0, w0, #1
	str	w0, [x29, 152]
.L162:
	ldr	w0, [x26, 376]
	str	w0, [x23, 4]
	ldr	w0, [x26, 424]
	str	w0, [x23, 8]
	ldr	w0, [x26, 428]
	str	w0, [x23, 12]
	ldr	w0, [x26, 380]
	str	w0, [x23, 16]
	ldr	w0, [x26, 384]
	str	w0, [x23, 20]
	ldr	w0, [x26, 392]
	str	w0, [x23, 24]
	ldr	w0, [x26, 400]
	str	w0, [x23, 28]
	ldr	w0, [x26, 404]
	str	w0, [x23, 32]
	ldr	w0, [x26, 408]
	str	w0, [x23, 36]
	ldr	w0, [x26, 412]
	str	w0, [x23, 40]
	ldr	w0, [x26, 416]
	str	w0, [x23, 44]
	ldr	w0, [x26, 420]
	stp	w0, w2, [x23, 48]
	cbz	w2, .L165
	add	x1, x21, 72
	mov	x0, 0
	add	x6, x29, 128
	add	x5, x29, 152
.L164:
	ldr	w4, [x6, x0, lsl 2]
	add	x1, x1, 4
	ldr	w3, [x5, x0, lsl 2]
	add	x0, x0, 1
	str	w4, [x1, -4]
	cmp	w2, w0
	str	w3, [x1, 16]
	bhi	.L164
.L165:
	ldr	x0, [x21]
	str	x23, [x21, 8]
	ldr	w1, [x0, 592]
	str	w1, [x21, 180]
	ldr	w0, [x0, 632]
	str	w0, [x21, 184]
	b	.L123
.L160:
	ldp	w4, w2, [x1, 48]
	lsr	w0, w2, 2
	and	w3, w2, 3
	mul	w0, w0, w4
	stp	wzr, w0, [x29, 128]
	lsl	w2, w0, 1
	sub	w5, w0, #1
	add	w4, w2, w0
	sub	w6, w2, #1
	stp	w2, w4, [x29, 136]
	lsl	w0, w0, 2
	sub	w2, w4, #1
	stp	w5, w6, [x29, 152]
	str	w2, [x29, 160]
	sub	w2, w0, #1
	str	w2, [x29, 164]
	mov	w2, 4
	cbz	w3, .L162
	ldr	w1, [x1, 64]
	mov	w2, 5
	str	w0, [x29, 144]
	sub	w0, w1, #1
	str	w0, [x29, 168]
	b	.L162
.L159:
	ldp	w3, w4, [x1, 48]
	mov	w2, 2
	lsr	w0, w4, 1
	mul	w0, w0, w3
	stp	wzr, w0, [x29, 128]
	lsl	w3, w0, 1
	sub	w0, w0, #1
	sub	w5, w3, #1
	str	w0, [x29, 152]
	str	w5, [x29, 156]
	tbz	x4, 0, .L162
	ldr	w0, [x1, 64]
	mov	w2, 3
	str	w3, [x29, 136]
	sub	w0, w0, #1
	str	w0, [x29, 160]
	b	.L162
.L156:
	ldr	w4, [x1, 16]
	b	.L157
	.size	DIVX3DEC_Decode, .-DIVX3DEC_Decode
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
.LC51:
	.xword	vfmw_Osal_Func_Ptr_S
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
.LC58:
	.xword	g_event_report
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3DEC_RecycleImage
	.type	DIVX3DEC_RecycleImage, %function
DIVX3DEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L229
	add	x19, x19, 40960
	mov	w2, 0
	mov	w1, w20
	ldr	w0, [x19, 1248]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 1248]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L229:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	DIVX3DEC_RecycleImage, .-DIVX3DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3DEC_GetRemainImg
	.type	DIVX3DEC_GetRemainImg, %function
DIVX3DEC_GetRemainImg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 40960
	bl	_mcount
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L231
	add	x20, x19, 192
	add	x21, x19, 40960
	add	x20, x20, 40960
	ldr	w0, [x21, 1248]
	ldr	w1, [x20, 396]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L231
	ldr	w1, [x20, 396]
	ldr	w0, [x21, 1248]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L244
.L231:
	add	x22, x21, 640
.L235:
	mov	x0, x22
	bl	GetVoLastImageID
.L241:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L244:
	mov	x0, x19
	add	x22, x21, 640
	bl	Divx3_SetImgFormat
	mov	w2, 1
	str	w2, [x23, 188]
	ldr	w1, [x20, 396]
	ldr	w0, [x21, 1248]
	bl	FSP_SetDisplay
	ldr	w0, [x21, 1248]
	mov	x4, x23
	mov	x3, x22
	mov	x2, x19
	mov	w1, 5
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L232
	ldr	w1, [x20, 396]
	mov	w2, 0
	ldr	w0, [x21, 1248]
	bl	FSP_SetDisplay
	ldr	w0, [x21, 1248]
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	str	xzr, [x23, 560]
	ldr	x1, [x23, 568]
	ldr	w0, [x21, 1248]
	bl	FreeUsdByDec
	str	xzr, [x23, 568]
	b	.L235
	.p2align 3
.L232:
	ldr	w0, [x23, 160]
	cmp	w0, 100
	bhi	.L231
	ldr	w0, [x23, 112]
	cmp	w0, 31
	bls	.L231
	ldr	w1, [x23, 116]
	mov	w0, 0
	cmp	w1, 31
	bhi	.L241
	b	.L235
	.size	DIVX3DEC_GetRemainImg, .-DIVX3DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DIVX3DEC_VDMPostProc
	.type	DIVX3DEC_VDMPostProc, %function
DIVX3DEC_VDMPostProc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	w21, w1
	mov	w24, w2
	bl	_mcount
	add	x19, x22, 192
	ldr	x1, .LC82
	mov	w2, w21
	mov	w0, 22
	add	x20, x22, 40960
	add	x23, x19, 40960
	bl	dprint_vfmw
	mov	x1, x22
	mov	w0, 5
	bl	ReleasePacket
	ldr	w0, [x20, 1248]
	ldr	w1, [x23, 396]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L273
	ldr	w1, [x23, 184]
	cbz	w1, .L275
	tbnz	w21, #31, .L258
.L278:
	mov	w1, 100
	cmp	w21, w1
	csel	w21, w21, w1, le
.L249:
	ldr	x1, .LC84
	str	w21, [x0, 160]
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x22
	bl	Divx3_SetImgFormat
	ldrb	w0, [x19, 3]
	cmp	w0, 2
	beq	.L252
	ldr	x0, [x22]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L252
	add	x0, x19, 40960
	mov	w1, -1
	ldr	w2, [x0, 308]
	cbz	w2, .L253
	ldr	w1, [x0, 392]
.L253:
	add	x0, x19, 40960
	str	w1, [x0, 400]
.L251:
	cmn	w1, #1
	beq	.L254
	add	x21, x19, 40960
	ldr	w0, [x20, 1248]
	bl	FSP_GetFsImagePtr
	ldr	w23, [x21, 400]
	mov	x24, x0
	ldr	w0, [x20, 1248]
	mov	w1, w23
	bl	FSP_GetRef
	mov	w3, w0
	ldr	x1, .LC86
	mov	w2, w23
	mov	w0, 12
	bl	dprint_vfmw
	ldr	w0, [x20, 1248]
	mov	w2, 1
	ldr	w1, [x21, 400]
	bl	FSP_SetDisplay
	ldr	w0, [x20, 1248]
	mov	x4, x24
	add	x3, x20, 640
	mov	x2, x22
	mov	w1, 5
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L276
.L254:
	add	x1, x19, 40960
	ldr	w0, [x1, 304]
	add	w0, w0, 1
	str	w0, [x1, 304]
	ldr	w0, [x20, 376]
	cmp	w0, 1
	beq	.L277
.L255:
	add	x21, x19, 40960
	mov	w1, 1
	ldr	w2, [x21, 392]
	ldr	w0, [x21, 396]
	str	w1, [x21, 308]
	cmp	w2, w0
	beq	.L257
	ldr	x1, .LC88
	mov	w0, 13
	bl	dprint_vfmw
	ldr	w0, [x20, 1248]
	mov	w2, 0
	ldr	w1, [x21, 392]
	bl	FSP_SetRef
	ldr	w0, [x20, 1248]
	ldr	w1, [x21, 392]
	bl	FSP_RemovePmv
	ldr	w0, [x21, 396]
.L257:
	add	x19, x19, 40960
	mov	w1, w0
	mov	w2, 1
	str	w0, [x19, 392]
	ldr	w0, [x20, 1248]
	bl	FSP_SetRef
	ldr	w2, [x19, 396]
	mov	w0, 13
	ldr	x1, .LC90
	bl	dprint_vfmw
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L275:
	and	w24, w24, 1
	str	w24, [x23, 280]
	tbz	w21, #31, .L278
.L258:
	mov	w21, 0
	b	.L249
	.p2align 3
.L252:
	add	x0, x19, 40960
	ldr	w1, [x0, 396]
	str	w1, [x0, 400]
	b	.L251
	.p2align 3
.L276:
	ldr	w0, [x20, 1248]
	mov	w2, 0
	ldr	w1, [x21, 400]
	bl	FSP_SetDisplay
.L273:
	mov	w0, 0
.L279:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L277:
	ldr	w0, [x1, 308]
	cbnz	w0, .L255
	mov	w0, 0
	b	.L279
	.size	DIVX3DEC_VDMPostProc, .-DIVX3DEC_VDMPostProc
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
	.global	DIVX3DEC_GetImageBuffer
	.type	DIVX3DEC_GetImageBuffer, %function
DIVX3DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 40960
	bl	_mcount
	ldr	w0, [x19, 1248]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	mov	w20, w0
	beq	.L281
	ldr	w0, [x19, 1248]
	mov	w20, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L286
.L281:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L286:
	ldr	w0, [x19, 1248]
	add	x1, x19, 640
	bl	FSP_ClearNotInVoQueue
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	DIVX3DEC_GetImageBuffer, .-DIVX3DEC_GetImageBuffer
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__FUNCTION__.11580, %object
	.size	__FUNCTION__.11580, 19
__FUNCTION__.11580:
	.string	"Divx3_DecodeDecide"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC2:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC4:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC6:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC8:
	ASCII(.string	"image size abnormal (%dx%d) in MB\n" )
	.zero	5
.LC10:
	ASCII(.string	"divx3 actual frame size(%dx%d) exeed max config(%dx%d)\n" )
.LC13:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC15:
	ASCII(.string	"ERROR: No Img buffer is allocated\n" )
	.zero	5
.LC17:
	ASCII(.string	"----------------- divx3 Imode -> %d ------------------\n" )
.LC19:
	ASCII(.string	"-----I_MODE, but FrameType is %d-----\n" )
	.zero	1
.LC21:
	ASCII(.string	"P-frm" )
	.zero	2
.LC23:
	ASCII(.string	"-----cur framtype is %s, but refnum=%d-----\n" )
	.zero	3
.LC25:
	ASCII(.string	"error: DecMode illegal\n" )
.LC28:
	ASCII(.string	"fuction return value is null,%s %d unknow error!!\n" )
	.zero	5
.LC30:
	ASCII(.string	"dec P frame, fwd ref err(%d) > ref_thr(%d)\n" )
	.zero	4
.LC37:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC39:
	ASCII(.string	"Decoding quits at frm %d\n" )
	.zero	6
.LC43:
	ASCII(.string	"ERROR: frm %d, Invalid packet pointer!\n" )
.LC45:
	ASCII(.string	"ERROR: frm %d, Invalid packet PhyAddr!\n" )
.LC47:
	ASCII(.string	"ERROR: frm %d, Invalid packet VirAddr!\n" )
.LC49:
	ASCII(.string	"ERROR: frm %d, Invalid packet len!\n" )
	.zero	4
.LC52:
	ASCII(.string	"ERROR: frm %d, Invalid WxH: %dx%d!\n" )
	.zero	4
.LC54:
	ASCII(.string	"Divx3_GetPictureHeader Error!\n" )
	.zero	1
.LC56:
	ASCII(.string	"Can't decode from P frame!\n" )
	.zero	4
.LC59:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC61:
	ASCII(.string	"pic width/height to large,MbWidth = %d, MbHeight = %d" )
	.zero	2
.LC63:
	ASCII(.string	"new seq size %d * %d\n" )
	.zero	2
.LC65:
	ASCII(.string	"Divx3_ArrangeVHBMem err\n" )
	.zero	7
.LC67:
	ASCII(.string	"Divx3_DecodeDecide err\n" )
.LC69:
	ASCII(.string	"Fatal Error: frm%d, get image buffer failed\n" )
	.zero	3
.LC71:
	ASCII(.string	"dec_pts: %lld Usertag = %lld\n" )
	.zero	2
.LC73:
	ASCII(.string	"FSP_GetFsImagePtr Error!\n" )
	.zero	6
.LC75:
	ASCII(.string	"CurFsID abnormal!\n" )
	.zero	5
.LC77:
	ASCII(.string	"dec addr: cur,cur2d,fwd,currpmv,colpmv = %p,%p,%p,%p,%p,%p\n" )
	.zero	4
.LC79:
	ASCII(.string	"vol_mode Error!\n" )
	.zero	7
.LC81:
	ASCII(.string	"ErrRatio = %d\n" )
	.zero	1
.LC83:
	ASCII(.string	"DEBUG: DEC over\n" )
	.zero	7
.LC85:
	ASCII(.string	"insert buf %d ref %d\n" )
	.zero	2
.LC87:
	ASCII(.string	"---------- unref %d\n" )
	.zero	3
.LC89:
	ASCII(.string	"++++++++++ ref %d\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
