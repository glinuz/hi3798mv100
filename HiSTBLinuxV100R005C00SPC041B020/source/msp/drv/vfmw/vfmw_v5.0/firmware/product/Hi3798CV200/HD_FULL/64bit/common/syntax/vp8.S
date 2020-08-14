#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vp8.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	VP8_init_frame.part.0, %function
VP8_init_frame.part.0:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC0
	ldr	x21, .LC1
	mov	x2, 38
	add	x0, x19, 2374
	ldr	x3, [x20, 104]
	mov	x1, x21
	blr	x3
	ldr	x3, [x20, 104]
	add	x1, x21, 40
	mov	x2, 1056
	add	x0, x19, 1318
	blr	x3
	ldr	x21, .LC2
	ldr	x3, [x20, 104]
	mov	x2, 4
	mov	x1, x21
	add	x0, x19, 1310
	blr	x3
	ldr	x3, [x20, 104]
	add	x1, x21, 8
	mov	x2, 3
	add	x0, x19, 1314
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 8
	mov	w1, 0
	add	x0, x19, 22
	blr	x3
	strb	wzr, [x19, 17]
	mov	x2, 4
	mov	w1, 0
	ldr	x3, [x20, 96]
	add	x0, x19, 32
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 4
	mov	w1, 0
	add	x0, x19, 36
	blr	x3
	strb	wzr, [x19, 43]
	strb	wzr, [x19, 44]
	mov	w0, 1
	strb	wzr, [x19, 48]
	strb	w0, [x19, 41]
	strb	w0, [x19, 42]
	strb	wzr, [x19, 49]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VP8_init_frame.part.0, .-VP8_init_frame.part.0
	.align	3
.LC0:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC1:
	.xword	.LANCHOR0
	.align	3
.LC2:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vp8_Vfmw_StartBoolDecode
	.type	Vp8_Vfmw_StartBoolDecode, %function
Vp8_Vfmw_StartBoolDecode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	mov	w1, 255
	mov	w0, 16
	str	w1, [x19]
	mov	w1, 24
	str	w0, [x19, 8]
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 4]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	Vp8_Vfmw_StartBoolDecode, .-Vp8_Vfmw_StartBoolDecode
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vp8_Vfmw_DecodeBool
	.type	Vp8_Vfmw_DecodeBool, %function
Vp8_Vfmw_DecodeBool:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x22, x0
	mov	x0, x30
	uxtb	w19, w2
	mov	x23, x1
	bl	_mcount
	mov	w24, 0
	ldp	w3, w0, [x22]
	ldr	w25, [x22, 8]
	sub	w1, w3, #1
	mul	w1, w1, w19
	lsr	w1, w1, 8
	add	w1, w1, 1
	lsl	w2, w1, 16
	cmp	w0, w2
	bcc	.L4
	sub	w1, w3, w1
	sub	w0, w0, w2
	mov	w24, 1
.L4:
	ldr	x2, .LC3
	add	x2, x2, 1096
	ldrb	w20, [x2, w1, uxtw]
	subs	w25, w25, w20
	lsl	w21, w1, w20
	lsl	w20, w0, w20
	bmi	.L10
.L5:
	stp	w21, w20, [x22]
	str	w25, [x22, 8]
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L10:
	mov	w0, 8
	subs	w26, w0, w25
	bmi	.L5
	mov	w19, w26
	.p2align 2
.L6:
	mov	w1, 8
	mov	x0, x23
	bl	BsGet
	lsl	w0, w0, w19
	subs	w19, w19, #8
	orr	w20, w20, w0
	bpl	.L6
	add	w25, w25, 8
	and	w0, w26, -8
	add	w25, w0, w25
	b	.L5
	.size	Vp8_Vfmw_DecodeBool, .-Vp8_Vfmw_DecodeBool
	.align	3
.LC3:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vp8_Vfmw_DecodeValue
	.type	Vp8_Vfmw_DecodeValue, %function
Vp8_Vfmw_DecodeValue:
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
	bmi	.L12
	.p2align 2
.L13:
	mov	w2, -128
	mov	x1, x22
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w20, w0, w20
	cmn	w19, #1
	bne	.L13
.L12:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	Vp8_Vfmw_DecodeValue, .-Vp8_Vfmw_DecodeValue
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_SetImgFormat
	.type	VP8_SetImgFormat, %function
VP8_SetImgFormat:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x20, 8192]
	ldr	w0, [x20, 8816]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L16
	ldr	w3, [x20, 8200]
	mov	w2, 7200
	mov	w5, 262144
	mov	w6, 1
	and	w3, w3, 3
	mov	w4, 8
	orr	w2, w3, w2
	str	w2, [x0, 108]
	mov	w8, 13
	mov	w1, 0
	ldr	x7, [x20]
	ldr	w7, [x7, 592]
	strb	wzr, [x0, 364]
	cmp	w7, 1
	strb	w6, [x0, 371]
	csel	w5, wzr, w5, ne
	strb	w6, [x0, 369]
	orr	w2, w5, w2
	str	w2, [x0, 108]
	add	x2, x0, 256
	stp	w3, w3, [x0, 100]
	stp	w8, w4, [x2, 120]
	stp	w4, w4, [x2, 128]
	bl	SetAspectRatio
	ldr	w2, [x20, 5804]
	str	w2, [x19, 112]
	ldr	w1, [x20, 5808]
	str	w1, [x19, 116]
	ldr	x0, [x20]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L24
.L19:
	lsr	w3, w2, 1
	lsr	w0, w1, 1
	stp	w2, w1, [x19, 120]
	stp	w3, w0, [x19, 128]
.L16:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L24:
	ldr	w3, [x19, 140]
	add	w1, w1, 15
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
	b	.L19
	.size	VP8_SetImgFormat, .-VP8_SetImgFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8DEC_VDMPostProc
	.type	VP8DEC_VDMPostProc, %function
VP8DEC_VDMPostProc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w21, w1
	add	x19, x20, 3240
	bl	_mcount
	mov	x1, x20
	mov	w0, 13
	bl	ReleasePacket
	ldr	w1, [x19, 4952]
	ldr	w0, [x20, 8816]
	bl	FSP_GetFsImagePtr
	cbz	x0, .L94
	tbnz	w21, #31, .L28
	cmp	w21, 100
	mov	w2, 100
	csel	w2, w21, w2, le
	str	w2, [x0, 160]
	ldr	x0, [x20]
	ldr	w3, [x0]
	cmp	w2, w3
	bhi	.L95
	mov	x0, x20
	bl	VP8_SetImgFormat
	ldr	w0, [x19, 4916]
	cmp	w0, 2
	beq	.L96
.L30:
	ldr	w0, [x20, 3308]
	cbnz	w0, .L32
.L31:
	mov	w0, -1
	str	w0, [x19, 4956]
	ldr	w1, [x19, 4952]
	mov	w2, 0
	ldr	w0, [x20, 8816]
	bl	FSP_SetDisplay
	ldr	w1, [x19, 4956]
.L33:
	cmn	w1, #1
	beq	.L34
	ldr	w0, [x20, 8816]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L97
	ldr	w1, [x19, 4956]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_SetDisplay
	add	x3, x20, 8192
	ldr	w0, [x20, 8816]
	mov	x4, x22
	add	x3, x3, 16
	mov	x2, x20
	mov	w1, 13
	bl	InsertImgToVoQueue
	cmp	w0, 1
	mov	w21, w0
	bne	.L98
.L34:
	ldr	w0, [x19, 4920]
	add	x21, x20, 3248
	ldr	w1, [x19, 4960]
	add	w0, w0, 1
	str	w0, [x19, 4920]
	ldr	w22, [x19, 4948]
	cbnz	w1, .L99
	ldr	w0, [x19, 4940]
	cmp	w22, w0
	beq	.L40
.L38:
	ldr	w0, [x19, 4944]
	cmp	w22, w0
	beq	.L40
	ldr	w1, [x19, 4952]
	cmp	w22, w1
	cset	w23, ne
.L39:
	str	w1, [x19, 4948]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_SetRef
.L37:
	ldrb	w0, [x21, 44]
	cmp	w0, 1
	beq	.L100
	cmp	w0, 2
	beq	.L101
.L43:
	ldrb	w0, [x21, 43]
	cmp	w0, 1
	beq	.L102
	cmp	w0, 2
	beq	.L103
.L48:
	ldr	w0, [x19, 4960]
	cbz	w0, .L51
	ldrb	w0, [x21, 41]
	cbz	w0, .L52
.L51:
	ldr	w3, [x19, 4940]
	ldr	w0, [x19, 4948]
	cmp	w3, w0
	beq	.L92
	ldr	w0, [x19, 4944]
	cmp	w3, w0
	beq	.L92
	ldr	w1, [x19, 4952]
	cmp	w3, w1
	beq	.L54
	ldr	w0, [x20, 8816]
	mov	w2, 0
	mov	w1, w3
	bl	FSP_SetRef
.L92:
	ldr	w1, [x19, 4952]
.L54:
	str	w1, [x19, 4940]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_SetRef
.L52:
	ldr	w0, [x21, 56]
	cbz	w0, .L56
	ldrb	w0, [x21, 42]
	cbz	w0, .L57
.L56:
	ldr	w3, [x19, 4944]
	ldr	w0, [x19, 4948]
	cmp	w3, w0
	beq	.L93
	ldr	w0, [x19, 4940]
	cmp	w3, w0
	beq	.L93
	ldr	w1, [x19, 4952]
	cmp	w3, w1
	beq	.L59
	ldr	w0, [x20, 8816]
	mov	w2, 0
	mov	w1, w3
	bl	FSP_SetRef
.L93:
	ldr	w1, [x19, 4952]
.L59:
	str	w1, [x19, 4944]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_SetRef
.L57:
	mov	w21, 1
	cbnz	w23, .L104
.L81:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L32:
	ldr	w1, [x19, 4952]
	str	w1, [x19, 4956]
	b	.L33
	.p2align 3
.L99:
	ldrb	w0, [x21, 40]
	mov	w23, 0
	cbz	w0, .L37
	ldr	w0, [x19, 4940]
	cmp	w22, w0
	bne	.L38
	.p2align 2
.L40:
	mov	w23, 0
	ldr	w1, [x19, 4952]
	b	.L39
	.p2align 3
.L28:
	str	wzr, [x0, 160]
	mov	x0, x20
	bl	VP8_SetImgFormat
	ldr	w0, [x19, 4916]
	cmp	w0, 2
	bne	.L30
.L96:
	ldr	w0, [x19, 4960]
	cbnz	w0, .L31
	b	.L30
	.p2align 3
.L102:
	ldr	w1, [x19, 4940]
	ldr	w0, [x19, 4948]
	cmp	w1, w0
	beq	.L47
	ldr	w0, [x19, 4944]
	cmp	w0, w1
	ccmp	w22, w1, 4, ne
	beq	.L47
	ldr	w0, [x20, 8816]
	mov	w2, 0
	bl	FSP_SetRef
.L47:
	str	w22, [x19, 4940]
	mov	w2, 1
	mov	w1, w22
	mov	w23, 0
	ldr	w0, [x20, 8816]
	bl	FSP_SetRef
	b	.L48
	.p2align 3
.L100:
	ldr	w1, [x19, 4944]
	ldr	w0, [x19, 4948]
	cmp	w1, w0
	beq	.L42
	ldr	w0, [x19, 4940]
	cmp	w0, w1
	ccmp	w22, w1, 4, ne
	beq	.L42
	ldr	w0, [x20, 8816]
	mov	w2, 0
	bl	FSP_SetRef
.L42:
	str	w22, [x19, 4944]
	mov	w2, 1
	mov	w1, w22
	mov	w23, 0
	ldr	w0, [x20, 8816]
	bl	FSP_SetRef
	b	.L43
	.p2align 3
.L98:
	ldr	w0, [x20, 8816]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	ldr	x1, [x22, 568]
	ldr	w0, [x20, 8816]
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	mov	w2, 0
	ldr	w1, [x19, 4956]
	ldr	w0, [x20, 8816]
	bl	FSP_SetDisplay
	ldr	x1, .LC12
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L103:
	ldr	w3, [x19, 4940]
	ldr	w0, [x19, 4948]
	ldr	w1, [x19, 4944]
	cmp	w3, w0
	beq	.L50
	cmp	w3, w1
	beq	.L50
	ldr	w0, [x20, 8816]
	mov	w1, w3
	mov	w2, 0
	bl	FSP_SetRef
	ldr	w1, [x19, 4944]
.L50:
	str	w1, [x19, 4940]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_SetRef
	b	.L48
	.p2align 3
.L101:
	ldr	w3, [x19, 4944]
	ldr	w0, [x19, 4948]
	ldr	w1, [x19, 4940]
	cmp	w3, w0
	beq	.L45
	cmp	w3, w1
	beq	.L45
	ldr	w0, [x20, 8816]
	mov	w1, w3
	mov	w2, 0
	bl	FSP_SetRef
	ldr	w1, [x19, 4940]
.L45:
	str	w1, [x19, 4944]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_SetRef
	b	.L43
	.p2align 3
.L104:
	ldr	w0, [x20, 8816]
	mov	w1, w22
	mov	w2, 0
	bl	FSP_SetRef
	ldr	x23, [sp, 48]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L95:
	ldr	x1, .LC8
	mov	w0, 1
	mov	w21, -1
	bl	dprint_vfmw
	ldr	w1, [x19, 4940]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_ClearLogicFs
	ldr	w1, [x19, 4944]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_ClearLogicFs
	ldr	w1, [x19, 4948]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 4940]
	str	wzr, [x19, 4944]
	str	wzr, [x19, 4948]
	ldr	w0, [x20, 8816]
	bl	FSP_GetNullLogicFs
	str	wzr, [x19, 4920]
	str	w0, [x19, 4940]
	str	w0, [x19, 4944]
	str	w0, [x19, 4948]
	b	.L81
.L97:
	ldr	x1, .LC10
	mov	w2, 636
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L81
.L94:
	ldr	x1, .LC6
	mov	w21, -1
	bl	dprint_vfmw
	b	.L81
	.size	VP8DEC_VDMPostProc, .-VP8DEC_VDMPostProc
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8DEC_Init
	.type	VP8DEC_Init, %function
VP8DEC_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x3, .LC13
	mov	x2, 8832
	mov	w1, 0
	mov	x0, x19
	ldr	w21, [x19, 8808]
	ldr	x3, [x3, 96]
	blr	x3
	str	w21, [x19, 8808]
	add	x0, x19, 8192
	add	x0, x0, 16
	bl	ResetVoQueue
	str	x20, [x19]
	mov	x0, x19
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x19, 8816]
	beq	.L108
	mov	w2, 1024
	str	w2, [x19, 8152]
	ldr	x2, [x19]
	mov	w3, 3
	str	wzr, [x19, 8164]
	mov	w0, 1
	ldr	w4, [x2, 8]
	str	w4, [x19, 8156]
	str	w3, [x19, 8168]
	str	wzr, [x19, 8172]
	str	w3, [x19, 8204]
	ldr	w2, [x2, 40]
	str	w2, [x19, 8176]
.L107:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L108:
	ldr	x1, .LC15
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L107
	.size	VP8DEC_Init, .-VP8DEC_Init
	.align	3
.LC13:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC15:
	.xword	.LC14
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8DEC_Destroy
	.type	VP8DEC_Destroy, %function
VP8DEC_Destroy:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VP8DEC_Destroy, .-VP8DEC_Destroy
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_setup_version
	.type	VP8_setup_version, %function
VP8_setup_version:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 64]
	cmp	w0, 3
	bls	.L120
.L111:
	stp	wzr, wzr, [x19, 76]
	stp	wzr, wzr, [x19, 84]
	str	wzr, [x19, 92]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L120:
	ldr	x1, .LC16
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx113
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx113:
	.section	.rodata
	.align	0
	.align	2
.L113:
	.byte	(.L111 - .Lrtx113) / 4
	.byte	(.L114 - .Lrtx113) / 4
	.byte	(.L115 - .Lrtx113) / 4
	.byte	(.L116 - .Lrtx113) / 4
	.text
	.p2align 3
.L115:
	mov	w0, 1
	stp	w0, wzr, [x19, 76]
	stp	w0, wzr, [x19, 84]
.L118:
	mov	w0, 1
	str	w0, [x19, 92]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L114:
	mov	w0, 1
	stp	wzr, w0, [x19, 76]
	stp	w0, wzr, [x19, 84]
	b	.L118
	.p2align 3
.L116:
	mov	w0, 1
	stp	w0, w0, [x19, 76]
	stp	w0, w0, [x19, 84]
	b	.L118
	.size	VP8_setup_version, .-VP8_setup_version
	.align	3
.LC16:
	.xword	.L113
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	read_mvcontexts
	.type	read_mvcontexts, %function
read_mvcontexts:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x23, x0
	mov	x0, x30
	mov	x22, x1
	mov	x28, x2
	bl	_mcount
	ldr	x27, .LC19
	mov	x26, 0
.L126:
	add	x24, x27, x26
	add	x25, x28, x26
	mov	x21, 0
	.p2align 2
.L125:
	ldrb	w2, [x24, x21]
	mov	x1, x22
	mov	x0, x23
	bl	Vp8_Vfmw_DecodeBool
	cbz	w0, .L122
	mov	w20, 0
	mov	w19, 6
	.p2align 2
.L123:
	mov	w2, -128
	mov	x1, x22
	mov	x0, x23
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w20, w0, w20
	cmn	w19, #1
	bne	.L123
	uxtb	w20, w20
	mov	w0, 1
	ubfiz	w1, w20, 1, 7
	cmp	w20, wzr
	csel	w0, w1, w0, ne
	strb	w0, [x25, x21]
.L122:
	add	x21, x21, 1
	cmp	x21, 19
	bne	.L125
	add	x26, x26, 19
	cmp	x26, 38
	bne	.L126
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.size	read_mvcontexts, .-read_mvcontexts
	.align	3
.LC19:
	.xword	.LANCHOR2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_mb_mode_mv_init
	.type	VP8_mb_mode_mv_init, %function
VP8_mb_mode_mv_init:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x23, x0
	mov	x0, x30
	mov	x19, x1
	mov	x20, x2
	bl	_mcount
	ldr	w0, [x23, 56]
	cbz	w0, .L135
	mov	w22, 0
	mov	w21, 7
	.p2align 2
.L137:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w22, w0, w22
	cmn	w21, #1
	bne	.L137
	strb	w22, [x23, 50]
	mov	w21, 7
	mov	w22, 0
	.p2align 2
.L138:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w22, w0, w22
	cmn	w21, #1
	bne	.L138
	strb	w22, [x23, 51]
	mov	w21, 7
	mov	w22, 0
	.p2align 2
.L139:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w22, w0, w22
	cmn	w21, #1
	bne	.L139
	strb	w22, [x23, 52]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	cmp	w0, 1
	beq	.L140
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	cbz	w0, .L142
.L141:
	add	x24, x23, 1314
	add	x25, x23, 1317
.L147:
	mov	w22, 0
	mov	w21, 7
	.p2align 2
.L146:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w22, w0, w22
	cmn	w21, #1
	bne	.L146
	strb	w22, [x24], 1
	cmp	x25, x24
	bne	.L147
.L142:
	add	x2, x23, 2374
	mov	x1, x20
	mov	x0, x19
	bl	read_mvcontexts
.L135:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L140:
	add	x24, x23, 1310
	add	x25, x23, 1314
.L144:
	mov	w22, 0
	mov	w21, 7
	.p2align 2
.L143:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w21
	sub	w21, w21, #1
	orr	w22, w0, w22
	cmn	w21, #1
	bne	.L143
	strb	w22, [x24], 1
	cmp	x24, x25
	bne	.L144
	mov	w2, -128
	mov	x1, x20
	mov	x0, x19
	bl	Vp8_Vfmw_DecodeBool
	cbnz	w0, .L141
	b	.L142
	.size	VP8_mb_mode_mv_init, .-VP8_mb_mode_mv_init
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	get_delta_q
	.type	get_delta_q, %function
get_delta_q:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x1
	mov	w23, w2
	mov	x24, x3
	bl	_mcount
	mov	w2, -128
	mov	x1, x22
	mov	x0, x21
	mov	w19, 0
	bl	Vp8_Vfmw_DecodeBool
	cbz	w0, .L159
	mov	w20, 3
.L160:
	mov	w2, -128
	mov	x1, x22
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w20
	sub	w20, w20, #1
	orr	w19, w0, w19
	cmn	w20, #1
	bne	.L160
	mov	w2, -128
	mov	x1, x22
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	cmp	w0, wzr
	csneg	w19, w19, w19, eq
.L159:
	cmp	w19, w23
	beq	.L161
	mov	w0, 1
	str	w0, [x24]
.L161:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	get_delta_q, .-get_delta_q
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_init_frame
	.type	VP8_init_frame, %function
VP8_init_frame:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 56]
	cbnz	w0, .L167
	mov	x0, x19
	bl	VP8_init_frame.part.0
.L167:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP8_init_frame, .-VP8_init_frame
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_GetPictureHeader
	.type	VP8_GetPictureHeader, %function
VP8_GetPictureHeader:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	x27, x1
	mov	w23, w2
	bl	_mcount
	add	x25, x27, 2460
	mov	w2, 7
	mov	w1, 8
	str	w2, [x29, 136]
	mov	x0, x20
	mov	w2, 6
	str	w2, [x29, 140]
	bl	BsGet
	mov	w19, w0
	mov	w1, 8
	mov	x0, x20
	uxtb	w21, w19
	bl	BsGet
	mov	w22, w0
	mov	w1, 8
	mov	x0, x20
	ubfiz	w22, w22, 8, 8
	bl	BsGet
	ubfiz	w0, w0, 16, 8
	and	w1, w19, 1
	orr	w0, w22, w0
	and	w19, w19, 255
	str	w1, [x25, 56]
	orr	w0, w0, w19
	str	w1, [x27, 4960]
	asr	w0, w0, 5
	ubfx	x1, x21, 1, 3
	cmp	w0, w23
	ubfx	x21, x21, 4, 1
	str	w0, [x25, 68]
	stp	w21, w1, [x25, 60]
	bhi	.L269
	mov	x0, x25
	bl	VP8_setup_version
	ldr	w0, [x25, 56]
	cbz	w0, .L270
.L173:
	add	x21, x27, 2600
	mov	w0, 255
	str	w0, [x27, 2600]
	mov	w2, 16
	mov	w1, 24
	mov	x0, x20
	str	w2, [x21, 8]
	bl	BsGet
	str	w0, [x21, 4]
	ldr	w0, [x25, 56]
	cbz	w0, .L271
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	strb	w0, [x25, 14]
	cbnz	w0, .L179
.L187:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	mov	w22, 0
	mov	w19, 5
	bl	Vp8_Vfmw_DecodeBool
	str	w0, [x25, 136]
.L180:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w22, w0, w22
	cmn	w19, #1
	bne	.L180
	strb	w22, [x25, 10]
	mov	w19, 2
	mov	w22, 0
.L191:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w22, w0, w22
	cmn	w19, #1
	bne	.L191
	strb	w22, [x25, 11]
	mov	w2, -128
	strb	wzr, [x25, 1]
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	strb	w0, [x25, 2]
	cbnz	w0, .L192
.L194:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	mov	w22, 0
	mov	w19, 6
	bl	Vp8_Vfmw_DecodeBool
	mov	w23, w0
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	orr	w0, w0, w23, lsl 1
	strb	w0, [x25, 13]
.L193:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w22, w0, w22
	cmn	w19, #1
	bne	.L193
	add	x19, x29, 144
	ldrsb	w2, [x25, 5]
	mov	x1, x20
	strb	w22, [x25, 3]
	mov	x0, x21
	str	wzr, [x19, -12]!
	mov	x3, x19
	bl	get_delta_q
	strb	w0, [x25, 5]
	ldrsb	w2, [x25, 6]
	mov	x3, x19
	mov	x1, x20
	mov	x0, x21
	bl	get_delta_q
	strb	w0, [x25, 6]
	ldrsb	w2, [x25, 7]
	mov	x3, x19
	mov	x1, x20
	mov	x0, x21
	bl	get_delta_q
	strb	w0, [x25, 7]
	ldrsb	w2, [x25, 8]
	mov	x3, x19
	mov	x1, x20
	mov	x0, x21
	bl	get_delta_q
	strb	w0, [x25, 8]
	ldrsb	w2, [x25, 9]
	mov	x1, x20
	mov	x3, x19
	mov	x0, x21
	bl	get_delta_q
	strb	w0, [x25, 9]
	ldr	w1, [x25, 56]
	cbnz	w1, .L272
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	strb	w0, [x25, 45]
	cbz	w0, .L273
.L208:
	ldr	w0, [x25, 56]
	cbnz	w0, .L209
.L275:
	mov	w0, 1
	strb	w0, [x25, 40]
.L210:
	ldr	x28, .LC34
	add	x0, x27, 1056
	mov	w2, 255
	str	x0, [x29, 104]
	str	x27, [x29, 112]
	add	x0, x28, 304
	str	x0, [x29, 120]
.L211:
	ldp	x26, x0, [x29, 112]
	sub	x24, x0, #231
	.p2align 2
.L220:
	sub	x22, x24, #33
	mov	x23, x26
.L218:
	mov	x19, 0
	b	.L216
	.p2align 3
.L212:
	add	x19, x19, 1
	cmp	x19, 11
	beq	.L274
.L214:
	ldrb	w2, [x22, x19]
.L216:
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	cmp	w0, 1
	bne	.L212
	mov	w27, 0
	mov	w28, 7
	.p2align 2
.L213:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w28
	sub	w28, w28, #1
	orr	w27, w0, w27
	cmn	w28, #1
	bne	.L213
	add	x0, x23, x19
	add	x19, x19, 1
	cmp	x19, 11
	strb	w27, [x0, 3778]
	bne	.L214
.L274:
	add	x22, x22, 11
	add	x23, x23, 11
	cmp	x24, x22
	beq	.L215
	ldrb	w2, [x22]
	b	.L218
.L273:
	ldr	x3, .LC33
	mov	x2, 1149
	add	x1, x27, 3761
	add	x0, x27, 2612
	ldr	x3, [x3, 104]
	blr	x3
	ldr	w0, [x25, 56]
	cbz	w0, .L275
	b	.L209
	.p2align 3
.L272:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	strb	w0, [x25, 41]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	strb	wzr, [x25, 43]
	ldrb	w2, [x25, 41]
	uxtb	w1, w0
	strb	w1, [x25, 42]
	cbnz	w2, .L206
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	mov	w19, w0
	mov	x1, x20
	mov	w2, -128
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	orr	w0, w0, w19, lsl 1
	ldrb	w1, [x25, 42]
	strb	w0, [x25, 43]
.L206:
	strb	wzr, [x25, 44]
	cbnz	w1, .L207
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	mov	w19, w0
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	orr	w0, w0, w19, lsl 1
	strb	w0, [x25, 44]
.L207:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	strb	w0, [x25, 48]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	strb	w0, [x25, 49]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	strb	w0, [x25, 45]
	cbnz	w0, .L208
	b	.L273
	.p2align 3
.L192:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	strb	w0, [x25, 1]
	cbz	w0, .L194
	add	x22, x27, 2492
	add	x19, x27, 2496
	b	.L199
.L196:
	add	x22, x22, 1
	cmp	x19, x22
	beq	.L276
.L199:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x25, 30]
	bne	.L196
	mov	w24, 0
	mov	w23, 5
.L197:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w23
	sub	w23, w23, #1
	orr	w24, w0, w24
	cmn	w23, #1
	bne	.L197
	strb	w24, [x22]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x25, 12]
	bne	.L196
	ldrb	w0, [x22]
	add	x22, x22, 1
	neg	w0, w0
	strb	w0, [x22, -1]
	cmp	x19, x22
	bne	.L199
.L276:
	add	x24, x27, 2500
	b	.L204
	.p2align 3
.L201:
	add	x19, x19, 1
	cmp	x24, x19
	beq	.L194
.L204:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x25, 31]
	bne	.L201
	mov	w23, 0
	mov	w22, 5
.L202:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w22
	sub	w22, w22, #1
	orr	w23, w0, w23
	cmn	w22, #1
	bne	.L202
	strb	w23, [x19]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	cbz	w0, .L201
	ldrb	w0, [x19]
	neg	w0, w0
	strb	w0, [x19]
	b	.L201
	.p2align 3
.L179:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	strb	w0, [x25, 15]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	strb	w0, [x25, 16]
	cbnz	w0, .L277
.L181:
	ldrb	w0, [x25, 15]
	cbz	w0, .L187
	ldr	x3, .LC33
	add	x0, x27, 2478
	mov	x2, 3
	mov	w1, 255
	mov	x19, x0
	add	x24, x27, 2481
	ldr	x3, [x3, 96]
	blr	x3
	b	.L190
	.p2align 3
.L188:
	add	x19, x19, 1
	cmp	x24, x19
	beq	.L187
.L190:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x25, 21]
	bne	.L188
	mov	w23, 0
	mov	w22, 7
.L189:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w22
	sub	w22, w22, #1
	orr	w23, w0, w23
	cmn	w22, #1
	bne	.L189
	strb	w23, [x19]
	b	.L188
	.p2align 3
.L271:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	str	w0, [x25, 128]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	str	w0, [x25, 132]
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	strb	w0, [x25, 14]
	cbz	w0, .L187
	b	.L179
	.p2align 3
.L209:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	strb	w0, [x25, 40]
	b	.L210
	.p2align 3
.L215:
	ldr	x0, [x29, 120]
	add	x26, x26, 33
	cmp	x24, x0
	beq	.L217
	ldrb	w2, [x24], 33
	b	.L220
.L217:
	ldp	x1, x0, [x29, 104]
	add	x0, x0, 264
	str	x0, [x29, 112]
	cmp	x1, x0
	beq	.L219
	ldr	x0, [x29, 120]
	ldrb	w2, [x0]
	add	x0, x0, 264
	str	x0, [x29, 120]
	b	.L211
.L219:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	str	w0, [x25, 72]
	cbz	w0, .L221
	mov	w22, 0
	mov	w19, 7
.L222:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w19
	sub	w19, w19, #1
	orr	w22, w0, w22
	cmn	w19, #1
	bne	.L222
	strb	w22, [x25, 53]
.L223:
	mov	x0, x25
	mov	x2, x20
	mov	x1, x21
	bl	VP8_mb_mode_mv_init
	ldr	w0, [x25, 56]
	cbz	w0, .L224
	ldr	w1, [x25, 40]
	mov	w0, 1
	tst	w1, 16776960
	beq	.L171
.L224:
	ldrb	w1, [x25, 3]
	mov	w0, 1
	strb	w1, [x25, 4]
.L171:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
.L269:
	ldr	x1, .LC24
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
.L221:
	strb	wzr, [x25, 53]
	b	.L223
.L270:
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	mov	w19, w0
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	mov	w21, w0
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	uxtb	w2, w0
	uxtb	w1, w19
	uxtb	w3, w21
	cmp	w1, 157
	mov	w1, 42
	ccmp	w3, 1, 0, eq
	ccmp	w2, w1, 0, eq
	beq	.L174
	ldr	x1, .LC26
	mov	w4, w2
	mov	w0, 1
	uxtb	w2, w19
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
.L174:
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	mov	w21, w0
	mov	w1, 8
	mov	x0, x20
	and	w21, w21, 255
	bl	BsGet
	mov	w22, w0
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	mov	w19, w0
	mov	w1, 8
	mov	x0, x20
	and	w19, w19, 255
	bl	BsGet
	ubfiz	w3, w0, 8, 6
	ubfiz	w2, w22, 8, 6
	orr	w3, w3, w19
	orr	w2, w2, w21
	mov	w1, 32
	cmp	w3, w1
	add	w4, w2, 15
	lsr	w4, w4, 4
	ccmp	w2, w1, 0, hi
	ubfx	x0, x0, 6, 2
	add	w1, w3, 15
	lsr	w1, w1, 4
	cset	w5, ls
	ubfx	x22, x22, 6, 2
	cmp	w4, 512
	stp	w22, w0, [x25, 112]
	mov	w0, 512
	ccmp	w1, w0, 2, ls
	mul	w6, w4, w1
	cset	w0, hi
	str	w6, [x25, 120]
	stp	w4, w1, [x25, 96]
	stp	w2, w3, [x25, 104]
	orr	w0, w5, w0
	cbnz	w0, .L278
	ldp	w19, w21, [x27, 112]
	cmp	w2, w19
	ccmp	w3, w21, 0, eq
	beq	.L176
	ldr	x1, .LC30
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, .LC32
	mov	w3, w21
	ldp	w4, w5, [x25, 96]
	mov	w2, w19
	mov	w0, 22
	bl	dprint_vfmw
.L176:
	ldr	w0, [x25, 56]
	cbnz	w0, .L173
	mov	x0, x25
	bl	VP8_init_frame.part.0
	b	.L173
.L278:
	ldr	x1, .LC28
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
.L277:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	mov	x23, x27
	mov	x24, 0
	bl	Vp8_Vfmw_DecodeBool
	strb	w0, [x25, 17]
	ldr	x3, .LC33
	mov	x2, 8
	mov	w1, 0
	add	x0, x27, 2482
	ldr	x3, [x3, 96]
	blr	x3
.L182:
	add	x26, x23, 4
.L186:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	mov	w28, 0
	bl	Vp8_Vfmw_DecodeBool
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x25]
	beq	.L279
.L183:
	strb	w28, [x23, 2482]
	add	x23, x23, 1
	cmp	x23, x26
	bne	.L186
	add	x24, x24, 4
	cmp	x24, 8
	bne	.L182
	b	.L181
	.p2align 3
.L279:
	add	x0, x29, 136
	mov	w19, 0
	ldr	w22, [x24, x0]
	subs	w22, w22, #1
	bmi	.L184
.L185:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	lsl	w0, w0, w22
	sub	w22, w22, #1
	orr	w19, w0, w19
	cmn	w22, #1
	bne	.L185
	sxtb	w28, w19
.L184:
	mov	w2, -128
	mov	x1, x20
	mov	x0, x21
	bl	Vp8_Vfmw_DecodeBool
	cbz	w0, .L183
	neg	w19, w19
	sxtb	w28, w19
	b	.L183
	.size	VP8_GetPictureHeader, .-VP8_GetPictureHeader
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
	.align	3
.LC33:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC34:
	.xword	.LANCHOR2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_ArrangeVHBMem
	.type	VP8_ArrangeVHBMem, %function
VP8_ArrangeVHBMem:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 4096
	add	x0, x0, 1604
	ldp	w2, w3, [x0, 104]
	cmp	w3, 31
	bls	.L282
	cmp	w2, 31
	bls	.L282
	ldr	x1, [x19]
	ldr	w0, [x1, 708]
	cbnz	w0, .L285
	ldr	w0, [x1, 28]
	cmp	w0, 25
	beq	.L312
.L286:
	mov	w20, 4
	mov	w22, w2
	mov	w21, w3
	mov	w24, 5
	mov	w23, w20
.L291:
	ldr	x0, .LC39
	mov	x2, 32
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x29, 80
	blr	x3
	strb	wzr, [x29, 81]
	ldr	x0, [x19]
	stp	w22, w21, [x29, 104]
	stp	w22, w21, [x29, 88]
	ldr	w0, [x0, 28]
	cmp	w0, 24
	beq	.L297
	strb	w23, [x29, 83]
	sxtb	w20, w20
	strb	w24, [x29, 85]
.L302:
	ldr	w0, [x19, 8816]
	add	x1, x29, 80
	strb	w20, [x29, 86]
	strb	wzr, [x29, 82]
	bl	FSP_ConfigInstance
	cbnz	w0, .L298
	ldr	x1, [x19]
	add	x3, x29, 76
	ldr	w0, [x19, 8816]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L313
	cbnz	w0, .L300
	mov	w0, 1
	str	w0, [x19, 8164]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L282:
	ldr	x1, .LC36
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
.L306:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L285:
	ldr	w0, [x1, 28]
	cmp	w0, 25
	bne	.L286
	ldr	w4, [x1, 668]
	ldr	w5, [x1, 672]
	cmp	w2, w4
	bhi	.L294
	cmp	w3, w5
	bhi	.L294
	ldr	w4, [x1, 696]
	mov	w22, w2
	ldr	w0, [x1, 692]
	mov	w21, w3
	mov	w20, 1
	add	w23, w0, 2
	cbnz	w4, .L311
.L296:
	ldrsb	w24, [x1, 716]
	b	.L291
	.p2align 3
.L298:
	ldr	x1, .LC43
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L312:
	ldr	w22, [x1, 668]
	cmp	w2, w22
	bhi	.L314
	ldr	w21, [x1, 672]
	cmp	w3, w21
	bhi	.L315
	ldr	w0, [x19, 8164]
	cmp	w0, 1
	beq	.L306
	ldr	w2, [x1, 696]
	mov	w20, 1
	ldr	w0, [x1, 692]
	add	w23, w0, 2
	cbz	w2, .L296
.L311:
	add	w20, w0, w20
	mov	w0, 17
	cmp	w20, w0
	ldrsb	w24, [x1, 716]
	csel	w20, w20, w0, le
	b	.L291
	.p2align 3
.L297:
	mov	w0, 1
	strb	w24, [x29, 85]
	strb	w0, [x29, 83]
	mov	w20, 1
	b	.L302
	.p2align 3
.L294:
	ldr	x1, .LC38
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L306
	.p2align 3
.L313:
	add	x0, x19, 8192
	add	x0, x0, 16
	bl	ResetVoQueue
.L300:
	ldr	x1, .LC41
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L306
	.p2align 3
.L315:
	mov	w5, w21
.L288:
	ldr	x1, .LC38
	mov	w4, w22
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L306
	.p2align 3
.L314:
	ldr	w5, [x1, 672]
	b	.L288
	.size	VP8_ArrangeVHBMem, .-VP8_ArrangeVHBMem
	.align	3
.LC36:
	.xword	.LC35
	.align	3
.LC38:
	.xword	.LC37
	.align	3
.LC39:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC41:
	.xword	.LC40
	.align	3
.LC43:
	.xword	.LC42
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_DecodeDecide
	.type	VP8_DecodeDecide, %function
VP8_DecodeDecide:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 3240
	bl	_mcount
	ldr	w0, [x19, 4924]
	cbz	w0, .L344
	ldr	w0, [x19, 4916]
	sub	w0, w0, #2
	cmp	w0, 1
	ldr	x0, [x20]
	ldr	w2, [x0, 8]
	bls	.L345
.L320:
	cmp	w2, 4
	str	w2, [x19, 4916]
	bls	.L346
	ldr	x1, .LC55
	mov	w0, 1
	bl	dprint_vfmw
.L323:
	mov	w0, 0
.L337:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L346:
	ldr	x0, .LC61
	ldrb	w0, [x0,w2,uxtw]
	adr	x1, .Lrtx326
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx326:
	.section	.rodata
	.align	0
	.align	2
.L326:
	.byte	(.L324 - .Lrtx326) / 4
	.byte	(.L324 - .Lrtx326) / 4
	.byte	(.L321 - .Lrtx326) / 4
	.byte	(.L323 - .Lrtx326) / 4
	.byte	(.L324 - .Lrtx326) / 4
	.text
	.p2align 3
.L345:
	cmp	w2, 1
	ccmp	w2, 4, 4, hi
	bne	.L320
	ldr	x1, .LC47
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w1, [x19, 4940]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_ClearLogicFs
	ldr	w1, [x19, 4944]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_ClearLogicFs
	ldr	w1, [x19, 4948]
	mov	w2, 1
	ldr	w0, [x20, 8816]
	bl	FSP_ClearLogicFs
	str	wzr, [x19, 4940]
	str	wzr, [x19, 4944]
	str	wzr, [x19, 4948]
	ldr	w0, [x20, 8816]
	bl	FSP_GetNullLogicFs
	str	wzr, [x19, 4920]
	str	w0, [x19, 4940]
	str	w0, [x19, 4944]
	str	w0, [x19, 4948]
	ldr	x0, [x20]
	ldr	w2, [x0, 8]
	b	.L320
	.p2align 3
.L344:
	ldr	x1, .LC45
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L321:
	ldr	w2, [x19, 4960]
	cbnz	w2, .L347
.L322:
	ldr	w1, [x19, 4948]
	ldr	w0, [x20, 8816]
	bl	FSP_GetFsImagePtr
	mov	x1, x0
	cbz	x0, .L348
	ldr	w2, [x19, 4960]
	mov	w0, 1
	cbz	w2, .L337
	ldr	x3, [x20]
	ldr	w2, [x1, 160]
	ldr	w3, [x3]
	cmp	w2, w3
	bls	.L337
	ldr	x1, .LC60
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L324:
	ldr	w0, [x19, 4960]
	cbz	w0, .L322
	ldr	w0, [x19, 4920]
	cbnz	w0, .L322
	ldr	x2, .LC51
	mov	w3, 0
	ldr	x1, .LC53
	mov	w0, 18
	bl	dprint_vfmw
	b	.L323
	.p2align 3
.L347:
	ldr	x1, .LC49
	mov	w0, 18
	bl	dprint_vfmw
	b	.L323
	.p2align 3
.L348:
	ldr	x2, .LC56
	mov	w3, 1504
	ldr	x1, .LC58
	mov	w0, 22
	add	x2, x2, 1096
	bl	dprint_vfmw
	mov	w0, 0
	b	.L337
	.size	VP8_DecodeDecide, .-VP8_DecodeDecide
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
	.xword	.LANCHOR2
	.align	3
.LC58:
	.xword	.LC57
	.align	3
.LC60:
	.xword	.LC59
	.align	3
.LC61:
	.xword	.L326
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_GetImageBuffer
	.type	VP8_GetImageBuffer, %function
VP8_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 3240
	bl	_mcount
	ldr	w0, [x19, 8816]
	mov	w1, 1
	bl	FSP_NewLogicFs
	str	w0, [x20, 4952]
	mov	w1, w0
	tbnz	w0, #31, .L356
	ldr	w0, [x19, 8816]
	bl	FSP_GetLogicFs
	mov	x19, x0
	cbz	x0, .L357
	ldr	x1, .LC67
	mov	w0, 18
	ldr	w2, [x20, 4952]
	bl	dprint_vfmw
	ldr	x0, [x19, 640]
	cbz	x0, .L355
	ldr	x1, [x19, 648]
	cbz	x1, .L355
	ldr	x2, [x19, 656]
	cbz	x2, .L355
	ldr	w3, [x1, 16]
	ldr	w4, [x2, 16]
	ldr	x1, .LC69
	ldr	w2, [x0, 16]
	mov	w0, 18
	bl	dprint_vfmw
	mov	w0, 1
.L351:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L355:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L356:
	ldr	x1, .LC63
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 8816]
	mov	w2, 1
	ldr	w1, [x20, 4940]
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 8816]
	mov	w2, 1
	ldr	w1, [x20, 4944]
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 4948]
	mov	w2, 1
	ldr	w0, [x19, 8816]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 4940]
	str	wzr, [x20, 4944]
	str	wzr, [x20, 4948]
	ldr	w0, [x19, 8816]
	bl	FSP_GetNullLogicFs
	str	wzr, [x20, 4920]
	str	w0, [x20, 4940]
	add	x1, x19, 8192
	str	w0, [x20, 4944]
	add	x1, x1, 16
	str	w0, [x20, 4948]
	ldr	w0, [x19, 8816]
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L357:
	ldr	x1, .LC65
	mov	w2, 1560
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L351
	.size	VP8_GetImageBuffer, .-VP8_GetImageBuffer
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_Set_DecParam
	.type	VP8_Set_DecParam, %function
VP8_Set_DecParam:
	stp	x29, x30, [sp, -176]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x24, x0
	mov	x0, x30
	add	x1, x24, 4096
	str	x1, [x29, 104]
	mov	x20, x1
	add	x21, x24, 16
	bl	_mcount
	ldr	x28, .LC70
	ldr	x3, [x24, 3192]
	mov	x2, 3160
	str	x3, [x29, 96]
	mov	w1, 0
	ldr	x3, [x28, 96]
	mov	x0, x21
	add	x19, x24, 3240
	blr	x3
	ldr	w1, [x20, 1660]
	add	x0, x20, 1604
	str	w1, [x21, 2796]
	ldr	w1, [x19, 4964]
	str	w1, [x21, 2800]
	ldr	w1, [x0, 96]
	str	w1, [x21, 2784]
	ldr	w1, [x0, 100]
	str	w1, [x21, 2788]
	ldr	w1, [x0, 84]
	str	w1, [x21, 2804]
	ldr	w1, [x0, 88]
	str	w1, [x21, 2808]
	ldrb	w1, [x0, 53]
	strb	w1, [x21, 2752]
	ldr	w1, [x0, 72]
	strb	w1, [x21, 2753]
	ldrb	w1, [x0, 13]
	strb	w1, [x21, 2754]
	ldrb	w1, [x0, 14]
	strb	w1, [x21, 2755]
	ldrb	w1, [x0, 15]
	strb	w1, [x21, 2756]
	ldrb	w1, [x0, 17]
	strb	w1, [x21, 2757]
	ldrb	w1, [x0, 2]
	strb	w1, [x21, 2758]
	ldr	w1, [x0, 136]
	strb	w1, [x21, 2760]
	ldrb	w1, [x0, 10]
	strb	w1, [x21, 2762]
	ldrb	w1, [x0, 11]
	strb	w1, [x21, 2763]
	ldr	w1, [x19, 4928]
	strb	w1, [x21, 2761]
	ldr	w1, [x19, 4932]
	strb	w1, [x21, 2764]
	ldrb	w1, [x0, 3]
	strb	w1, [x21, 2759]
	ldrsb	x1, [x0, 5]
	lsr	w2, w1, 31
	strb	w2, [x21, 2765]
	eor	x2, x1, x1, asr 63
	sub	x2, x2, x1, asr 63
	strb	w2, [x21, 2766]
	ldrsb	x1, [x0, 6]
	lsr	w2, w1, 31
	strb	w2, [x21, 2767]
	eor	x2, x1, x1, asr 63
	sub	x2, x2, x1, asr 63
	strb	w2, [x21, 2768]
	ldrsb	x1, [x0, 7]
	lsr	w2, w1, 31
	strb	w2, [x21, 2769]
	eor	x2, x1, x1, asr 63
	sub	x2, x2, x1, asr 63
	strb	w2, [x21, 2770]
	ldrsb	x1, [x0, 8]
	lsr	w2, w1, 31
	strb	w2, [x21, 2771]
	eor	x2, x1, x1, asr 63
	sub	x2, x2, x1, asr 63
	strb	w2, [x21, 2772]
	ldrsb	x1, [x0, 9]
	lsr	w2, w1, 31
	strb	w2, [x21, 2773]
	eor	x2, x1, x1, asr 63
	sub	x2, x2, x1, asr 63
	strb	w2, [x21, 2774]
	ldrb	w1, [x0, 50]
	strb	w1, [x21, 2775]
	ldrb	w1, [x0, 51]
	strb	w1, [x21, 2776]
	ldrb	w1, [x0, 52]
	strb	w1, [x21, 2777]
	ldr	w1, [x0, 144]
	str	w1, [x21, 2780]
	ldr	w1, [x0, 140]
	strb	w1, [x21, 2778]
	ldr	w0, [x0, 148]
	strb	w0, [x21, 2779]
	ldr	w0, [x19, 4936]
	str	w0, [x21, 3136]
	ldr	w1, [x19, 4952]
	ldr	w0, [x24, 8816]
	bl	FSP_GetLogicFs
	mov	x20, x0
	ldr	w1, [x19, 4940]
	ldr	w0, [x24, 8816]
	bl	FSP_GetLogicFs
	mov	x22, x0
	ldr	w1, [x19, 4944]
	ldr	w0, [x24, 8816]
	bl	FSP_GetLogicFs
	mov	x23, x0
	ldr	w1, [x19, 4948]
	ldr	w0, [x24, 8816]
	bl	FSP_GetLogicFs
	cmp	x20, xzr
	ccmp	x22, xzr, 4, ne
	beq	.L359
	cmp	x23, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L359
	ldr	x1, [x20, 640]
	cbz	x1, .L398
	ldr	w1, [x1, 16]
	str	w1, [x21, 3116]
	ldr	w1, [x20, 664]
	str	w1, [x21, 3132]
	ldr	x1, [x22, 640]
	cbz	x1, .L399
.L394:
	ldr	w1, [x1, 16]
	str	w1, [x21, 3124]
	ldr	x1, [x23, 640]
	cbz	x1, .L400
.L395:
	ldr	w1, [x1, 16]
	str	w1, [x21, 3120]
	ldr	x0, [x0, 640]
	cbz	x0, .L401
.L396:
	ldr	w0, [x0, 16]
	add	x3, x24, 3068
	str	w0, [x21, 3128]
	ldr	x0, [x29, 104]
	ldr	x2, [x20, 648]
	add	x1, x0, 1626
	add	x0, x24, 3052
	ldr	w2, [x2, 32]
	str	w2, [x19, 4912]
	str	w2, [x21, 2792]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 76]
	str	w2, [x21, 3148]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 60]
	str	w2, [x21, 3152]
	ldr	x2, [x20, 648]
	ldr	w2, [x2, 64]
	str	w2, [x21, 3156]
.L369:
	add	x0, x0, 4
	ldrsb	w2, [x1]
	add	x1, x1, 1
	cmp	x3, x0
	str	w2, [x0, -4]
	ldrsb	w2, [x1, 3]
	str	w2, [x0, 12]
	ldrsb	w2, [x1, 9]
	str	w2, [x0, 28]
	ldrsb	w2, [x1, 13]
	str	w2, [x0, 44]
	ldrb	w2, [x1, 23]
	str	w2, [x0, 60]
	bne	.L369
	ldr	x20, [x29, 104]
	mov	x2, 16
	ldr	x3, [x28, 96]
	mov	w1, 0
	add	x19, x20, 1604
	mov	x0, x21
	mov	w25, 1152
	add	x26, x20, 2922
	blr	x3
	ldrb	w0, [x19, 53]
	mov	x2, 16
	strb	w0, [x24, 16]
	mov	w1, 0
	add	x0, x24, 32
	ldrb	w3, [x19, 18]
	strb	w3, [x24, 17]
	ldrb	w3, [x19, 19]
	strb	w3, [x24, 18]
	ldrb	w3, [x19, 20]
	strb	w3, [x24, 19]
	ldrb	w3, [x19, 50]
	strb	w3, [x24, 20]
	ldrb	w3, [x19, 51]
	strb	w3, [x24, 21]
	ldrb	w3, [x19, 52]
	strb	w3, [x24, 22]
	ldr	x3, [x28, 96]
	blr	x3
	ldrb	w0, [x19, 1310]
	mov	x2, 16
	strb	w0, [x24, 32]
	mov	w1, 0
	add	x0, x24, 48
	ldrb	w3, [x19, 1311]
	strb	w3, [x24, 33]
	ldrb	w3, [x19, 1312]
	strb	w3, [x24, 34]
	ldrb	w3, [x19, 1313]
	strb	w3, [x24, 35]
	ldrb	w3, [x19, 1314]
	strb	w3, [x24, 36]
	ldrb	w3, [x19, 1315]
	strb	w3, [x24, 37]
	ldrb	w3, [x19, 1316]
	strb	w3, [x24, 38]
	ldr	x3, [x28, 96]
	blr	x3
	ldr	x3, [x28, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x24, 64
	blr	x3
	ldr	x3, [x28, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x24, 720
	blr	x3
	ldrb	w0, [x19, 2375]
	mov	x2, 16
	strb	w0, [x24, 720]
	mov	w1, 0
	ldr	x3, [x28, 96]
	ldrb	w0, [x19, 2374]
	strb	w0, [x24, 721]
	ldrb	w0, [x19, 2376]
	strb	w0, [x24, 722]
	ldrb	w0, [x19, 2377]
	strb	w0, [x24, 723]
	ldrb	w0, [x19, 2378]
	strb	w0, [x24, 724]
	ldrb	w0, [x19, 2379]
	strb	w0, [x24, 725]
	ldrb	w0, [x19, 2380]
	strb	w0, [x24, 726]
	ldrb	w0, [x19, 2381]
	strb	w0, [x24, 727]
	ldrb	w0, [x19, 2382]
	strb	w0, [x24, 728]
	add	x0, x24, 736
	blr	x3
	ldrb	w0, [x19, 2375]
	mov	x2, 16
	strb	w0, [x24, 736]
	mov	w1, 0
	ldr	x3, [x28, 96]
	ldrb	w0, [x19, 2383]
	strb	w0, [x24, 737]
	ldrb	w0, [x19, 2384]
	strb	w0, [x24, 738]
	ldrb	w0, [x19, 2385]
	strb	w0, [x24, 739]
	ldrb	w0, [x19, 2386]
	strb	w0, [x24, 740]
	ldrb	w0, [x19, 2387]
	strb	w0, [x24, 741]
	ldrb	w0, [x19, 2388]
	strb	w0, [x24, 742]
	ldrb	w0, [x19, 2389]
	strb	w0, [x24, 743]
	ldrb	w0, [x19, 2390]
	strb	w0, [x24, 744]
	ldrb	w0, [x19, 2391]
	strb	w0, [x24, 745]
	ldrb	w0, [x19, 2392]
	strb	w0, [x24, 746]
	add	x0, x24, 752
	blr	x3
	ldrb	w0, [x19, 2394]
	mov	x2, 16
	strb	w0, [x24, 752]
	mov	w1, 0
	ldr	x3, [x28, 96]
	ldrb	w0, [x19, 2393]
	strb	w0, [x24, 753]
	ldrb	w0, [x19, 2395]
	strb	w0, [x24, 754]
	ldrb	w0, [x19, 2396]
	strb	w0, [x24, 755]
	ldrb	w0, [x19, 2397]
	strb	w0, [x24, 756]
	ldrb	w0, [x19, 2398]
	strb	w0, [x24, 757]
	ldrb	w0, [x19, 2399]
	strb	w0, [x24, 758]
	ldrb	w0, [x19, 2400]
	strb	w0, [x24, 759]
	ldrb	w0, [x19, 2401]
	strb	w0, [x24, 760]
	add	x0, x24, 768
	blr	x3
	ldrb	w0, [x19, 2394]
	strb	w0, [x24, 768]
	ldrb	w0, [x19, 2402]
	strb	w0, [x24, 769]
	ldrb	w0, [x19, 2403]
	strb	w0, [x24, 770]
	ldrb	w0, [x19, 2404]
	strb	w0, [x24, 771]
	ldrb	w0, [x19, 2405]
	strb	w0, [x24, 772]
	ldrb	w0, [x19, 2406]
	strb	w0, [x24, 773]
	ldrb	w0, [x19, 2407]
	strb	w0, [x24, 774]
	ldrb	w0, [x19, 2408]
	strb	w0, [x24, 775]
	ldrb	w0, [x19, 2409]
	strb	w0, [x24, 776]
	ldrb	w0, [x19, 2410]
	strb	w0, [x24, 777]
	ldrb	w0, [x19, 2411]
	strb	w0, [x24, 778]
.L370:
	sub	w19, w25, #384
	mov	x23, x26
	.p2align 2
.L374:
	add	w22, w19, 48
	mov	x20, x23
.L371:
	add	x27, x21, x19, uxtw
	ldr	x5, [x28, 96]
	mov	x2, 16
	mov	w1, 0
	mov	x0, x27
	add	w19, w19, 16
	blr	x5
	ldr	x5, [x28, 104]
	mov	x1, x20
	mov	x2, 11
	mov	x0, x27
	add	x20, x20, x2
	blr	x5
	cmp	w19, w22
	bne	.L371
	cmp	w25, w19
	add	x23, x23, 33
	bne	.L374
	add	w25, w25, 384
	add	x26, x26, 264
	cmp	w25, 2688
	bne	.L370
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 1660]
	cbnz	w0, .L386
	mov	x19, 10
	mov	w20, w19
.L375:
	ldr	x26, [x29, 96]
	add	x25, x24, 3200
	ldr	x0, [x29, 104]
	ldr	w1, [x26, 8]
	add	x27, x0, 1604
	mov	x0, x25
	and	w1, w1, -16
	sub	w1, w1, #16
	str	w1, [x21, 2812]
	ldr	w22, [x27, 68]
	bl	BsPos
	add	w22, w20, w22
	lsl	w22, w22, 3
	sub	w0, w22, w0
	str	w0, [x21, 2816]
	mov	x0, x25
	ldr	w23, [x26, 8]
	bl	BsPos
	mov	w22, w0
	cmp	w22, wzr
	add	w1, w22, 7
	csel	w1, w1, w22, lt
	and	w23, w23, 15
	mov	x0, x25
	add	w22, w23, 16
	add	w22, w22, w1, asr 3
	bl	BsPos
	asr	w2, w0, 31
	lsr	w2, w2, 29
	ubfiz	w3, w22, 3, 4
	add	w1, w0, w2
	and	w22, w22, -16
	and	w1, w1, 7
	sub	w0, w1, w2
	add	x2, x21, 2816
	add	w0, w3, w0
	mov	w1, 1
	stp	w0, w22, [x2, 4]
	stp	wzr, wzr, [x2, 12]
	str	wzr, [x21, 2836]
	ldrb	w2, [x27, 13]
	ldr	w9, [x27, 68]
	lsl	w1, w1, w2
	ldr	w3, [x26, 12]
	and	w7, w1, 255
	ldr	x0, [x26]
	uxtb	w1, w1
	str	w7, [x21, 2840]
	cmp	w1, 1
	add	x19, x19, x9, uxtw
	sub	w3, w3, w20
	add	x0, x0, x19
	bls	.L387
	sub	w5, w7, #1
	lsl	w1, w5, 2
	sub	w5, w1, w5
	add	x5, x0, x5, sxtw
.L376:
	cbz	w7, .L385
	mov	w2, 1
	sub	w8, w7, #1
	sub	w4, w2, w7
	mov	x1, 0
	lsl	w2, w4, 2
	sub	w2, w2, w4
	add	x4, x29, 144
	sub	w2, w2, w9
	add	w10, w2, w3
	.p2align 2
.L380:
	cmp	w8, w1
	ble	.L378
	ldrb	w2, [x0, 1]
	ldrb	w6, [x0]
	ldrb	w3, [x0, 2]
	add	w2, w6, w2, lsl 8
	add	w2, w2, w3, lsl 16
.L379:
	str	w2, [x4, x1, lsl 2]
	add	x1, x1, 1
	cmp	w7, w1
	add	x0, x0, 3
	add	x5, x5, x2, uxtw
	bgt	.L380
	ldr	x0, [x29, 96]
	add	w20, w20, w8, lsl 2
	sub	w8, w20, w8
	mov	x2, x4
	add	x3, x29, 112
	mov	w4, 0
	ldr	w1, [x0, 8]
	add	x0, x24, 2860
	and	w1, w1, 15
	add	w9, w1, w9
	add	w8, w9, w8
	.p2align 2
.L384:
	cbz	w4, .L402
	ldr	w1, [x2, -4]
	ldr	w5, [x3, -4]
	add	w1, w5, w1
	str	w1, [x3]
.L383:
	ldr	w6, [x2], 4
	and	w5, w1, -16
	add	w4, w4, 1
	ubfiz	w1, w1, 3, 4
	lsl	w6, w6, 3
	str	w1, [x0, 32]
	str	w6, [x0]
	add	w1, w5, 16
	str	wzr, [x0, 96]
	cmp	w7, w4
	str	wzr, [x0, 128]
	add	x3, x3, 4
	str	wzr, [x0, 160]
	add	x0, x0, 4
	str	w1, [x0, 60]
	bne	.L384
.L385:
	ldr	x1, [x24]
	mov	w0, 1
	str	x21, [x24, 8]
	ldr	w2, [x1, 592]
	str	w2, [x24, 3156]
	ldr	w1, [x1, 632]
	str	w1, [x24, 3160]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 176
	ret
	.p2align 3
.L359:
	ldr	x1, .LC72
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
.L392:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 176
	ret
.L386:
	mov	x19, 3
	mov	w20, w19
	b	.L375
.L387:
	mov	x5, x0
	b	.L376
.L401:
	ldr	x0, [x20, 640]
	b	.L396
.L400:
	ldr	x1, [x20, 640]
	b	.L395
.L399:
	ldr	x1, [x20, 640]
	b	.L394
.L398:
	ldr	x1, .LC74
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L392
.L378:
	sub	x2, x0, x5
	add	w2, w10, w2
	b	.L379
.L402:
	str	w8, [x29, 112]
	ldr	w1, [x3]
	b	.L383
	.size	VP8_Set_DecParam, .-VP8_Set_DecParam
	.align	3
.LC70:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC72:
	.xword	.LC71
	.align	3
.LC74:
	.xword	.LC73
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8_Update_PictHeader
	.type	VP8_Update_PictHeader, %function
VP8_Update_PictHeader:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 2460
	add	x22, x19, 8
	bl	_mcount
	ldr	x21, .LC75
	ldrb	w0, [x20, 45]
	cbnz	w0, .L404
	ldr	x3, [x21, 104]
	mov	x2, 1149
	add	x1, x19, 2612
	add	x0, x19, 3761
	blr	x3
.L404:
	ldr	x3, [x21, 104]
	mov	x1, x20
	mov	x2, 2452
	mov	x0, x22
	blr	x3
	ldr	w0, [x20, 56]
	str	w0, [x19, 4964]
	ldr	w0, [x20, 136]
	str	w0, [x19, 4928]
	ldrb	w0, [x20, 11]
	str	w0, [x19, 4932]
	ldrb	w0, [x20, 40]
	strb	w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VP8_Update_PictHeader, .-VP8_Update_PictHeader
	.align	3
.LC75:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8DEC_Decode
	.type	VP8DEC_Decode, %function
VP8DEC_Decode:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x20]
	str	x1, [x19, 3176]
	ldr	w0, [x20, 12]
	str	w0, [x19, 3184]
	ldr	w2, [x20, 16]
	str	w2, [x19, 3188]
	cmp	w2, 2
	ble	.L449
	add	x21, x19, 3200
	add	x20, x19, 3240
	mov	x0, x21
	bl	BsInit
	add	x0, x19, 3176
	ldr	w2, [x19, 3188]
	str	x0, [x19, 3192]
	mov	x1, x20
	mov	x0, x21
	bl	VP8_GetPictureHeader
	cmp	w0, 1
	bne	.L450
	ldr	w0, [x20, 4924]
	cbnz	w0, .L451
.L411:
	ldr	x22, .LC80
	ldr	x4, [x22]
	cbz	x4, .L413
	ldr	w3, [x19, 3348]
	ldr	w2, [x19, 5796]
	ldr	w1, [x19, 5800]
	ldr	w5, [x19, 3344]
	ldr	w0, [x19, 8816]
	strh	w3, [x29, 74]
	mov	w3, 8
	strh	w2, [x29, 76]
	add	x2, x29, 72
	strh	w1, [x29, 78]
	mov	w1, 2
	strh	w5, [x29, 72]
	blr	x4
.L413:
	ldr	w0, [x19, 8816]
	bl	FSP_GetFspType
	cbz	w0, .L452
.L448:
	add	x21, x19, 4096
.L414:
	ldr	w0, [x19, 8816]
	mov	w2, 1
	ldr	w1, [x20, 4940]
	add	x22, x19, 8192
	add	x22, x22, 16
	add	x21, x21, 1604
	bl	FSP_ClearLogicFs
	ldr	w0, [x19, 8816]
	mov	w2, 1
	ldr	w1, [x20, 4944]
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 4948]
	mov	w2, 1
	ldr	w0, [x19, 8816]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 4940]
	str	wzr, [x20, 4944]
	str	wzr, [x20, 4948]
	ldr	w0, [x19, 8816]
	bl	FSP_GetNullLogicFs
	str	wzr, [x20, 4920]
	str	w0, [x20, 4940]
	mov	x1, x22
	str	w0, [x20, 4944]
	str	w0, [x20, 4948]
	ldr	w0, [x19, 8816]
	bl	FSP_ClearNotInVoQueue
	ldp	w2, w3, [x21, 96]
	cmp	w2, 512
	bhi	.L421
	cmp	w3, 512
	bhi	.L421
	ldr	x0, [x19]
	ldr	w1, [x0, 708]
	cbnz	w1, .L423
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L453
.L423:
	mov	x0, x19
	bl	VP8_ArrangeVHBMem
	cmp	w0, 1
	bne	.L454
.L412:
	mov	x0, x19
	bl	VP8_DecodeDecide
	cmp	w0, 1
	mov	w21, w0
	beq	.L424
	ldr	x1, .LC88
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L409
	.p2align 3
.L450:
	ldr	x1, .LC79
	mov	w0, 2
	bl	dprint_vfmw
	mov	w0, 0
.L409:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L449:
	ldr	x1, .LC77
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L451:
	ldr	w2, [x19, 3344]
	add	x21, x19, 4096
	ldr	w1, [x19, 5796]
	add	x21, x21, 1604
	cmp	w2, w1
	bne	.L411
	ldr	w1, [x19, 3348]
	ldr	w0, [x21, 100]
	cmp	w1, w0
	bne	.L411
	b	.L412
	.p2align 3
.L452:
	add	x3, x29, 80
	ldr	w0, [x19, 8816]
	add	x2, x29, 68
	add	x1, x29, 64
	stp	wzr, wzr, [x29, 64]
	str	wzr, [x3, -8]!
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19]
	ldr	w2, [x0, 708]
	cmp	w2, 1
	bne	.L448
	ldr	w0, [x29, 72]
	cmp	w0, wzr
	bgt	.L417
	ldr	w0, [x29, 68]
	cmp	w0, 32
	bgt	.L417
	ldr	x0, [x19, 8]
	cbnz	x0, .L417
	ldr	w3, [x19, 3344]
	add	x21, x19, 4096
	cbz	w3, .L414
	ldr	w1, [x19, 8820]
	ldr	w6, [x21, 1704]
	cmp	w6, w1
	beq	.L414
	ldr	w1, [x19, 8824]
	ldr	w5, [x21, 1700]
	cmp	w5, w1
	beq	.L414
	ldr	w4, [x19, 3348]
	mov	w0, w2
	ldr	x1, .LC82
	bl	dprint_vfmw
	ldr	w0, [x21, 1704]
	str	w0, [x19, 8820]
	ldr	x4, [x22]
	ldr	w0, [x21, 1700]
	str	w0, [x19, 8824]
	cbz	x4, .L414
	ldr	w0, [x19, 8816]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L414
	.p2align 3
.L421:
	ldr	x1, .LC84
	mov	w0, 18
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L454:
	ldr	x1, .LC86
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L409
	.p2align 3
.L424:
	mov	x0, x19
	bl	VP8_GetImageBuffer
	cmp	w0, 1
	beq	.L425
	ldr	x1, .LC90
	mov	w0, w21
	bl	dprint_vfmw
	mov	w0, 0
	b	.L409
	.p2align 3
.L453:
	ldr	w0, [x20, 4924]
	cbnz	w0, .L423
	mov	x0, x22
	bl	ResetVoQueue
	b	.L423
	.p2align 3
.L425:
	ldr	w0, [x19, 8816]
	ldr	w1, [x20, 4952]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L455
	ldr	x3, [x19]
	mov	x4, -1
	ldr	x1, .LC93
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
	ldr	w0, [x20, 4960]
	cbnz	w0, .L427
	ldr	w1, [x20, 4944]
	ldr	w0, [x20, 4952]
	cmp	w1, w0
	beq	.L428
	ldr	w0, [x19, 8816]
	mov	w2, 0
	bl	FSP_SetRef
	ldr	w0, [x20, 4952]
.L428:
	ldr	w1, [x20, 4948]
	cmp	w1, w0
	beq	.L429
	ldr	w0, [x19, 8816]
	mov	w2, 0
	bl	FSP_SetRef
	ldr	w0, [x20, 4952]
.L429:
	ldr	w1, [x20, 4940]
	cmp	w1, w0
	beq	.L430
	ldr	w0, [x19, 8816]
	mov	w2, 0
	bl	FSP_SetRef
.L430:
	str	wzr, [x20, 4920]
.L427:
	mov	x0, x19
	bl	VP8_Set_DecParam
	mov	x0, x20
	bl	VP8_Update_PictHeader
	mov	w0, 0
	b	.L409
	.p2align 3
.L417:
	mov	w0, -2
	b	.L409
.L455:
	ldr	x1, .LC91
	mov	w2, 2044
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L409
	.size	VP8DEC_Decode, .-VP8DEC_Decode
	.align	3
.LC77:
	.xword	.LC76
	.align	3
.LC79:
	.xword	.LC78
	.align	3
.LC80:
	.xword	g_event_report
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
	.align	3
.LC91:
	.xword	.LC64
	.align	3
.LC93:
	.xword	.LC92
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8DEC_RecycleImage
	.type	VP8DEC_RecycleImage, %function
VP8DEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L458
	ldr	w0, [x19, 8816]
	mov	w1, w20
	mov	w2, 0
	bl	FSP_SetDisplay
	ldr	w0, [x19, 8816]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L459
	ldr	w0, [x19, 8816]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 8816]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L458:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L459:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP8DEC_RecycleImage, .-VP8DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8DEC_GetRemainImg
	.type	VP8DEC_GetRemainImg, %function
VP8DEC_GetRemainImg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 8192
	add	x0, x0, 16
	bl	GetVoLastImageID
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP8DEC_GetRemainImg, .-VP8DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VP8DEC_GetImageBuffer
	.type	VP8DEC_GetImageBuffer, %function
VP8DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x20, 8816]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	mov	w19, w0
	beq	.L462
	ldr	w0, [x20, 8816]
	mov	w19, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L467
.L462:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L467:
	add	x1, x20, 8192
	ldr	w0, [x20, 8816]
	add	x1, x1, 16
	bl	FSP_ClearNotInVoQueue
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VP8DEC_GetImageBuffer, .-VP8DEC_GetImageBuffer
	.global	default_uv_mode_prob
	.global	default_ymode_prob
	.global	VP8_VFMWBITREADER_NORM
	.global	vp8_mv_update_probs
	.global	vp8_default_mv_context
	.global	vp8_coef_update_probs
	.global	default_coef_probs
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	vp8_default_mv_context, %object
	.size	vp8_default_mv_context, 38
vp8_default_mv_context:
	.byte	-94
	.byte	-128
	.byte	-31
	.byte	-110
	.byte	-84
	.byte	-109
	.byte	-42
	.byte	39
	.byte	-100
	.byte	-128
	.byte	-127
	.byte	-124
	.byte	75
	.byte	-111
	.byte	-78
	.byte	-50
	.byte	-17
	.byte	-2
	.byte	-2
	.byte	-92
	.byte	-128
	.byte	-52
	.byte	-86
	.byte	119
	.byte	-21
	.byte	-116
	.byte	-26
	.byte	-28
	.byte	-128
	.byte	-126
	.byte	-126
	.byte	74
	.byte	-108
	.byte	-76
	.byte	-53
	.byte	-20
	.byte	-2
	.byte	-2
	.zero	2
	.type	default_coef_probs, %object
	.size	default_coef_probs, 1056
default_coef_probs:
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-3
	.byte	-120
	.byte	-2
	.byte	-1
	.byte	-28
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-67
	.byte	-127
	.byte	-14
	.byte	-1
	.byte	-29
	.byte	-43
	.byte	-1
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	106
	.byte	126
	.byte	-29
	.byte	-4
	.byte	-42
	.byte	-47
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	98
	.byte	-8
	.byte	-1
	.byte	-20
	.byte	-30
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-75
	.byte	-123
	.byte	-18
	.byte	-2
	.byte	-35
	.byte	-22
	.byte	-1
	.byte	-102
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	78
	.byte	-122
	.byte	-54
	.byte	-9
	.byte	-58
	.byte	-76
	.byte	-1
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-71
	.byte	-7
	.byte	-1
	.byte	-13
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-72
	.byte	-106
	.byte	-9
	.byte	-1
	.byte	-20
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	77
	.byte	110
	.byte	-40
	.byte	-1
	.byte	-20
	.byte	-26
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	101
	.byte	-5
	.byte	-1
	.byte	-15
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-86
	.byte	-117
	.byte	-15
	.byte	-4
	.byte	-20
	.byte	-47
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	37
	.byte	116
	.byte	-60
	.byte	-13
	.byte	-28
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-52
	.byte	-2
	.byte	-1
	.byte	-11
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-49
	.byte	-96
	.byte	-6
	.byte	-1
	.byte	-18
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	102
	.byte	103
	.byte	-25
	.byte	-1
	.byte	-45
	.byte	-85
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-104
	.byte	-4
	.byte	-1
	.byte	-16
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-79
	.byte	-121
	.byte	-13
	.byte	-1
	.byte	-22
	.byte	-31
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	80
	.byte	-127
	.byte	-45
	.byte	-1
	.byte	-62
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-10
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-58
	.byte	35
	.byte	-19
	.byte	-33
	.byte	-63
	.byte	-69
	.byte	-94
	.byte	-96
	.byte	-111
	.byte	-101
	.byte	62
	.byte	-125
	.byte	45
	.byte	-58
	.byte	-35
	.byte	-84
	.byte	-80
	.byte	-36
	.byte	-99
	.byte	-4
	.byte	-35
	.byte	1
	.byte	68
	.byte	47
	.byte	-110
	.byte	-48
	.byte	-107
	.byte	-89
	.byte	-35
	.byte	-94
	.byte	-1
	.byte	-33
	.byte	-128
	.byte	1
	.byte	-107
	.byte	-15
	.byte	-1
	.byte	-35
	.byte	-32
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-72
	.byte	-115
	.byte	-22
	.byte	-3
	.byte	-34
	.byte	-36
	.byte	-1
	.byte	-57
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	81
	.byte	99
	.byte	-75
	.byte	-14
	.byte	-80
	.byte	-66
	.byte	-7
	.byte	-54
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	-127
	.byte	-24
	.byte	-3
	.byte	-42
	.byte	-59
	.byte	-14
	.byte	-60
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	99
	.byte	121
	.byte	-46
	.byte	-6
	.byte	-55
	.byte	-58
	.byte	-1
	.byte	-54
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	23
	.byte	91
	.byte	-93
	.byte	-14
	.byte	-86
	.byte	-69
	.byte	-9
	.byte	-46
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	-56
	.byte	-10
	.byte	-1
	.byte	-22
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	109
	.byte	-78
	.byte	-15
	.byte	-1
	.byte	-25
	.byte	-11
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	44
	.byte	-126
	.byte	-55
	.byte	-3
	.byte	-51
	.byte	-64
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-124
	.byte	-17
	.byte	-5
	.byte	-37
	.byte	-47
	.byte	-1
	.byte	-91
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	94
	.byte	-120
	.byte	-31
	.byte	-5
	.byte	-38
	.byte	-66
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	22
	.byte	100
	.byte	-82
	.byte	-11
	.byte	-70
	.byte	-95
	.byte	-1
	.byte	-57
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-74
	.byte	-7
	.byte	-1
	.byte	-24
	.byte	-21
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	124
	.byte	-113
	.byte	-15
	.byte	-1
	.byte	-29
	.byte	-22
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	35
	.byte	77
	.byte	-75
	.byte	-5
	.byte	-63
	.byte	-45
	.byte	-1
	.byte	-51
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-99
	.byte	-9
	.byte	-1
	.byte	-20
	.byte	-25
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	121
	.byte	-115
	.byte	-21
	.byte	-1
	.byte	-31
	.byte	-29
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	45
	.byte	99
	.byte	-68
	.byte	-5
	.byte	-61
	.byte	-39
	.byte	-1
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	1
	.byte	-5
	.byte	-1
	.byte	-43
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-53
	.byte	1
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-119
	.byte	1
	.byte	-79
	.byte	-1
	.byte	-32
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-3
	.byte	9
	.byte	-8
	.byte	-5
	.byte	-49
	.byte	-48
	.byte	-1
	.byte	-64
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-81
	.byte	13
	.byte	-32
	.byte	-13
	.byte	-63
	.byte	-71
	.byte	-7
	.byte	-58
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	73
	.byte	17
	.byte	-85
	.byte	-35
	.byte	-95
	.byte	-77
	.byte	-20
	.byte	-89
	.byte	-1
	.byte	-22
	.byte	-128
	.byte	1
	.byte	95
	.byte	-9
	.byte	-3
	.byte	-44
	.byte	-73
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-17
	.byte	90
	.byte	-12
	.byte	-6
	.byte	-45
	.byte	-47
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-101
	.byte	77
	.byte	-61
	.byte	-8
	.byte	-68
	.byte	-61
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	24
	.byte	-17
	.byte	-5
	.byte	-38
	.byte	-37
	.byte	-1
	.byte	-51
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-55
	.byte	51
	.byte	-37
	.byte	-1
	.byte	-60
	.byte	-70
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	69
	.byte	46
	.byte	-66
	.byte	-17
	.byte	-55
	.byte	-38
	.byte	-1
	.byte	-28
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-65
	.byte	-5
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-33
	.byte	-91
	.byte	-7
	.byte	-1
	.byte	-43
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-115
	.byte	124
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	16
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-66
	.byte	36
	.byte	-26
	.byte	-1
	.byte	-20
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-107
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-30
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-9
	.byte	-64
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-16
	.byte	-128
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-122
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-43
	.byte	62
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	55
	.byte	93
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-54
	.byte	24
	.byte	-43
	.byte	-21
	.byte	-70
	.byte	-65
	.byte	-36
	.byte	-96
	.byte	-16
	.byte	-81
	.byte	-1
	.byte	126
	.byte	38
	.byte	-74
	.byte	-24
	.byte	-87
	.byte	-72
	.byte	-28
	.byte	-82
	.byte	-1
	.byte	-69
	.byte	-128
	.byte	61
	.byte	46
	.byte	-118
	.byte	-37
	.byte	-105
	.byte	-78
	.byte	-16
	.byte	-86
	.byte	-1
	.byte	-40
	.byte	-128
	.byte	1
	.byte	112
	.byte	-26
	.byte	-6
	.byte	-57
	.byte	-65
	.byte	-9
	.byte	-97
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-90
	.byte	109
	.byte	-28
	.byte	-4
	.byte	-45
	.byte	-41
	.byte	-1
	.byte	-82
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	39
	.byte	77
	.byte	-94
	.byte	-24
	.byte	-84
	.byte	-76
	.byte	-11
	.byte	-78
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	52
	.byte	-36
	.byte	-10
	.byte	-58
	.byte	-57
	.byte	-7
	.byte	-36
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	124
	.byte	74
	.byte	-65
	.byte	-13
	.byte	-73
	.byte	-63
	.byte	-6
	.byte	-35
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	24
	.byte	71
	.byte	-126
	.byte	-37
	.byte	-102
	.byte	-86
	.byte	-13
	.byte	-74
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	-74
	.byte	-31
	.byte	-7
	.byte	-37
	.byte	-16
	.byte	-1
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-107
	.byte	-106
	.byte	-30
	.byte	-4
	.byte	-40
	.byte	-51
	.byte	-1
	.byte	-85
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	28
	.byte	108
	.byte	-86
	.byte	-14
	.byte	-73
	.byte	-62
	.byte	-2
	.byte	-33
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	81
	.byte	-26
	.byte	-4
	.byte	-52
	.byte	-53
	.byte	-1
	.byte	-64
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	123
	.byte	102
	.byte	-47
	.byte	-9
	.byte	-68
	.byte	-60
	.byte	-1
	.byte	-23
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	20
	.byte	95
	.byte	-103
	.byte	-13
	.byte	-92
	.byte	-83
	.byte	-1
	.byte	-53
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-34
	.byte	-8
	.byte	-1
	.byte	-40
	.byte	-43
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-88
	.byte	-81
	.byte	-10
	.byte	-4
	.byte	-21
	.byte	-51
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	47
	.byte	116
	.byte	-41
	.byte	-1
	.byte	-45
	.byte	-44
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	121
	.byte	-20
	.byte	-3
	.byte	-44
	.byte	-42
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-115
	.byte	84
	.byte	-43
	.byte	-4
	.byte	-55
	.byte	-54
	.byte	-1
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	42
	.byte	80
	.byte	-96
	.byte	-16
	.byte	-94
	.byte	-71
	.byte	-1
	.byte	-51
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-12
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-18
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.type	VP8_VFMWBITREADER_NORM, %object
	.size	VP8_VFMWBITREADER_NORM, 256
VP8_VFMWBITREADER_NORM:
	.byte	0
	.byte	7
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
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
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
	.byte	0
	.byte	0
	.align	3
.LANCHOR2 = . + 0
	.type	vp8_mv_update_probs, %object
	.size	vp8_mv_update_probs, 38
vp8_mv_update_probs:
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
	.byte	-2
	.byte	-2
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
	.byte	-2
	.byte	-2
	.zero	2
	.type	vp8_coef_update_probs, %object
	.size	vp8_coef_update_probs, 1056
vp8_coef_update_probs:
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
	.byte	-80
	.byte	-10
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-33
	.byte	-15
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-3
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
	.byte	-12
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-22
	.byte	-2
	.byte	-2
	.byte	-1
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
	.byte	-10
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-17
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-1
	.byte	-2
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
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-3
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-6
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-39
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
	.byte	-4
	.byte	-15
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-22
	.byte	-6
	.byte	-15
	.byte	-6
	.byte	-3
	.byte	-1
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-33
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-18
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-2
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
	.byte	-9
	.byte	-2
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
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-4
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
	.byte	-2
	.byte	-2
	.byte	-1
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
	.byte	-2
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-6
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
	.byte	-2
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
	.byte	-70
	.byte	-5
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-22
	.byte	-5
	.byte	-12
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-5
	.byte	-13
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-20
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-2
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
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-2
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
	.byte	-6
	.byte	-2
	.byte	-4
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-2
	.byte	-7
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-10
	.byte	-3
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-4
	.byte	-2
	.byte	-5
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-2
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-11
	.byte	-5
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-4
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-1
	.byte	-2
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
	.byte	-2
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
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-6
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
	.byte	-2
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
	.type	__FUNCTION__.11708, %object
	.size	__FUNCTION__.11708, 17
__FUNCTION__.11708:
	.string	"VP8_DecodeDecide"
	.data
	.align	3
.LANCHOR1 = . + 0
	.type	default_ymode_prob, %object
	.size	default_ymode_prob, 4
default_ymode_prob:
	.byte	112
	.byte	86
	.byte	-116
	.byte	37
	.zero	4
	.type	default_uv_mode_prob, %object
	.size	default_uv_mode_prob, 3
default_uv_mode_prob:
	.byte	-94
	.byte	101
	.byte	-52
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC5:
	ASCII(.string	"pCurImg is null\n" )
	.zero	7
.LC7:
	ASCII(.string	"err_level(%d) over ref_thr(%d)\n" )
.LC9:
	ASCII(.string	"line: %d pToQueImg is null\n" )
	.zero	4
.LC11:
	ASCII(.string	"insert img to Voqueue failed!\n" )
	.zero	1
.LC14:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC23:
	ASCII(.string	"Error!partition 0 length > streamlength!\n" )
	.zero	6
.LC25:
	ASCII(.string	"Invalid frame sync code,data[0-1]:%02x,%02x,%02x\n" )
	.zero	6
.LC27:
	ASCII(.string	"Invalid frame width %d and Height %d!\n" )
	.zero	1
.LC29:
	ASCII(.string	"We have a new size image!\n" )
	.zero	5
.LC31:
	ASCII(.string	"The old size is %d x %d,the new is %d x %d\n" )
	.zero	4
.LC35:
	ASCII(.string	"image size abnormal(%dx%d)\n" )
	.zero	4
.LC37:
	ASCII(.string	"vp8 actual frame size(%dx%d) exeed max config(%dx%d)\n" )
	.zero	2
.LC40:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC42:
	ASCII(.string	"FSP_ConfigInstance fail!\n" )
	.zero	6
.LC44:
	ASCII(.string	"ERROR: No Img buffer is allocated\n" )
	.zero	5
.LC46:
	ASCII(.string	"----------------- vp8 Imode -> %d ------------------\n" )
	.zero	2
.LC48:
	ASCII(.string	"-----I_MODE, but FrameType is %d-----\n" )
	.zero	1
.LC50:
	ASCII(.string	"P-frm" )
	.zero	2
.LC52:
	ASCII(.string	"-----cur framtype is %s, but refnum=%d-----\n" )
	.zero	3
.LC54:
	ASCII(.string	"error: DecMode illegal\n" )
.LC57:
	ASCII(.string	"%s %d NULL == pFwdImg!!\n" )
	.zero	7
.LC59:
	ASCII(.string	"dec P frame, fwd ref err(%d) > ref_thr(%d)\n" )
	.zero	4
.LC62:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC64:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC66:
	ASCII(.string	"get image buffer ok: LogicFsID = %d\n" )
	.zero	3
.LC68:
	ASCII(.string	"decode %p, disp %p, tf %p\n" )
	.zero	5
.LC71:
	ASCII(.string	"VP8_Set_DecParam but logic fs is null!\n" )
.LC73:
	ASCII(.string	"VP8_Set_DecParam but cur logic pstDecodeFs is null!\n" )
	.zero	3
.LC76:
	ASCII(.string	"Error!Truncated packet\n" )
.LC78:
	ASCII(.string	"VP8_CB_GetPictureHeader Error!\n" )
.LC81:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC83:
	ASCII(.string	"pic width/height to large,MbWidth = %d, MbHeight = %d" )
	.zero	2
.LC85:
	ASCII(.string	"VP8_ArrangeVHBMem error\n" )
	.zero	7
.LC87:
	ASCII(.string	"VP8_DecodeDecide err\n" )
	.zero	2
.LC89:
	ASCII(.string	"VP8_GetBufFromQueue err\n" )
	.zero	7
.LC92:
	ASCII(.string	"dec_pts: %lld Usertag = %lld\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
