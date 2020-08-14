#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"vc1.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	VC1_Vfmw_GenerateIntensityMap, %function
VC1_Vfmw_GenerateIntensityMap:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w2
	mov	w21, w1
	uxtb	w22, w3
	bl	_mcount
	cmp	w20, 32
	sub	w0, w20, #64
	csel	w20, w0, w20, ge
	cbnz	w21, .L3
	lsl	w2, w20, 25
	mov	w4, -64
	sub	w2, w2, w20
	lsl	w2, w2, 7
	add	w2, w2, 12288
	add	w2, w2, 4032
.L4:
	cbnz	w22, .L5
	mov	x0, x19
	add	w2, w2, 32
	add	x5, x19, 256
	mov	w3, 255
	.p2align 2
.L6:
	asr	w1, w2, 6
	add	w2, w2, w4
	cmp	w1, wzr
	csel	w1, w1, wzr, ge
	cmp	w1, 255
	csel	w1, w1, w3, le
	strb	w1, [x0], 1
	cmp	x5, x0
	bne	.L6
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L5:
	lsl	w2, w4, 25
	mov	x0, x19
	sub	w2, w2, w4
	add	x5, x19, 256
	lsl	w2, w2, 7
	mov	w3, 255
	add	w2, w2, 8192
	add	w2, w2, 32
	.p2align 2
.L7:
	asr	w1, w2, 6
	add	w2, w2, w4
	cmp	w1, wzr
	csel	w1, w1, wzr, ge
	cmp	w1, 255
	csel	w1, w1, w3, le
	strb	w1, [x0], 1
	cmp	x5, x0
	bne	.L7
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3:
	add	w4, w21, 32
	lsl	w2, w20, 6
	b	.L4
	.size	VC1_Vfmw_GenerateIntensityMap, .-VC1_Vfmw_GenerateIntensityMap
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5, %function
VC1_Vfmw_UpdataLastPicRNDCTRL.part.5:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 8192
	ldrb	w0, [x1, 2459]
	and	w2, w0, -5
	cbz	w2, .L15
	cmp	w0, 1
	ldrsb	w0, [x19, 10]
	uxtb	w2, w0
	beq	.L16
	strb	w0, [x19, 10]
	strb	w2, [x1, 2478]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L15:
	mov	w2, 1
	mov	w0, w2
	strb	w0, [x19, 10]
	strb	w2, [x1, 2478]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L16:
	cmp	w0, wzr
	cset	w0, eq
	mov	w2, w0
	sxtb	w0, w0
	strb	w0, [x19, 10]
	strb	w2, [x1, 2478]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5, .-VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	emp_print
	.type	emp_print, %function
emp_print:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	mov	x0, x30
	stp	x2, x3, [x29, 16]
	stp	x4, x5, [x29, 32]
	stp	x6, x7, [x29, 48]
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.size	emp_print, .-emp_print
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AbandonBitsLenthNoUse
	.type	AbandonBitsLenthNoUse, %function
AbandonBitsLenthNoUse:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w1, [x19]
	mov	w0, 0
	cbz	w1, .L27
	b	.L31
	.p2align 3
.L25:
	mov	w0, w2
.L27:
	ldrb	w1, [x19, -1]!
	add	w2, w0, 8
	cbz	w1, .L25
	add	w0, w0, 9
.L23:
	tbz	x1, 0, .L28
	b	.L21
	.p2align 3
.L26:
	mov	w2, w0
.L28:
	lsr	w1, w1, 1
	add	w0, w2, 1
	tbz	x1, 0, .L26
	add	w0, w2, 2
.L21:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L31:
	mov	w0, 1
	mov	w2, 0
	b	.L23
	.size	AbandonBitsLenthNoUse, .-AbandonBitsLenthNoUse
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1DEC_Init
	.type	VC1DEC_Init, %function
VC1DEC_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	add	x19, x20, 53248
	bl	_mcount
	ldr	x3, .LC12
	mov	x2, 54056
	mov	w1, 0
	mov	x0, x20
	ldr	w22, [x19, 792]
	ldr	x3, [x3, 96]
	blr	x3
	str	w22, [x19, 792]
	add	x0, x19, 192
	bl	ResetVoQueue
	add	x4, x20, 12288
	ldr	w1, [x21, 32]
	str	x21, [x20]
	mov	w5, 1
	add	w1, w1, 255
	mov	x0, x20
	strb	w5, [x4, 2529]
	and	w1, w1, -256
	add	w3, w1, 4096
	add	w2, w1, 8192
	ldr	w6, [x21, 8]
	str	w6, [x19, 140]
	add	w6, w1, 2048
	str	w1, [x19, 112]
	add	w1, w1, 12288
	str	w3, [x19, 120]
	add	w3, w3, 2048
	str	w2, [x19, 128]
	add	w2, w2, 2048
	str	w6, [x19, 116]
	str	w3, [x19, 124]
	stp	w2, w1, [x19, 132]
	strb	w5, [x4, 2530]
	stp	wzr, wzr, [x19, 72]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x19, 800]
	beq	.L35
	ldr	x1, .LC16
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 1
.L34:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L35:
	ldr	x1, .LC14
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L34
	.size	VC1DEC_Init, .-VC1DEC_Init
	.align	3
.LC12:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC14:
	.xword	.LC13
	.align	3
.LC16:
	.xword	.LC15
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Vc1SMPSeqHdr
	.type	Vc1SMPSeqHdr, %function
Vc1SMPSeqHdr:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x2
	mov	w21, w1
	bl	_mcount
	lsl	w2, w21, 3
	mov	x1, x20
	add	x0, x29, 56
	bl	BsInit
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	mov	w20, w0
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	add	w20, w20, w0, lsl 8
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	add	w20, w20, w0, lsl 16
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	add	w0, w20, w0, lsl 24
	mov	w1, 8
	str	w0, [x19, 8]
	add	x0, x29, 56
	bl	BsGet
	mov	w20, w0
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	add	w20, w20, w0, lsl 8
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	add	w20, w20, w0, lsl 16
	mov	w1, 8
	add	x0, x29, 56
	bl	BsGet
	add	w0, w20, w0, lsl 24
	ldr	w1, [x19, 8]
	str	w0, [x19, 12]
	cmp	w1, 8192
	bgt	.L40
	cmp	w0, 8192
	bgt	.L41
	cmp	w1, 31
	ble	.L40
	cmp	w0, 31
	ble	.L41
	mov	w1, 2
	add	x0, x29, 56
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 16]
	bhi	.L43
	mov	w1, 2
	add	x0, x29, 56
	bl	BsGet
	cbnz	w0, .L43
	mov	w1, 3
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 17]
	mov	w1, 5
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 18]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 19]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	cbnz	w0, .L43
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 20]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 21]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 22]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 23]
	mov	w1, 2
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 24]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 25]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	mov	w20, w0
	cbnz	w0, .L43
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 26]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 27]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 28]
	ldrb	w1, [x19, 16]
	cbnz	w1, .L44
	strb	wzr, [x19, 28]
.L44:
	mov	w1, 3
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 29]
	mov	w1, 2
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 30]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	strb	w0, [x19, 31]
	mov	w1, 1
	add	x0, x29, 56
	bl	BsGet
	mov	w1, 32
	add	x0, x29, 56
	bl	BsGet
	cbnz	w0, .L49
	mov	w0, w20
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L49:
	ldr	x1, .LC27
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L43:
	ldr	x3, .LC25
.L48:
	ldr	x2, .LC19
	mov	w0, 0
	ldr	x1, .LC21
	mov	w20, -1
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L41:
	ldr	x3, .LC23
	b	.L48
	.p2align 3
.L40:
	ldr	x3, .LC18
	b	.L48
	.size	Vc1SMPSeqHdr, .-Vc1SMPSeqHdr
	.align	3
.LC18:
	.xword	.LC17
	.align	3
.LC19:
	.xword	.LANCHOR0
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
	.global	VC1_SetImgFormat
	.type	VC1_SetImgFormat, %function
VC1_SetImgFormat:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 12288
	bl	_mcount
	add	x22, x20, 2520
	add	x1, x22, 20480
	add	x0, x21, 53248
	ldr	w1, [x1, 3620]
	ldr	w0, [x0, 800]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L50
	mov	w0, 1
	str	w0, [x19, 376]
	ldrsb	w0, [x20, 2523]
	cmp	w0, 3
	beq	.L64
	mov	w0, 7168
	strb	wzr, [x19, 364]
	str	w0, [x19, 108]
.L54:
	add	x1, x20, 2520
	add	x1, x1, 20480
	ldr	w1, [x1, 3560]
	cmp	w1, 1920
	bls	.L65
	and	w0, w0, -131073
	str	w0, [x19, 108]
	mov	w1, 0
	ldr	x2, [x21]
	ldr	w2, [x2, 632]
	cmp	w2, 1
	beq	.L66
.L57:
	add	x20, x20, 2520
	orr	w0, w0, w1
	add	x20, x20, 20480
	str	w0, [x19, 108]
	mov	w1, 0
	mov	x0, x19
	ldr	w2, [x20, 3564]
	str	w2, [x19, 116]
	ldr	w2, [x20, 3560]
	str	w2, [x19, 112]
	bl	SetAspectRatio
	ldr	w3, [x21, 14588]
	ldr	w1, [x19, 44]
	ldr	w2, [x19, 140]
	ldr	x4, [x19, 504]
	add	w0, w3, w1
	add	w5, w2, w1
	str	w1, [x19, 60]
	add	w2, w2, w0
	add	x3, x4, x3, uxtw
	stp	w1, w0, [x19, 28]
	stp	w5, w2, [x19, 36]
	str	x3, [x19, 512]
	str	w0, [x19, 48]
	str	x4, [x19, 520]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	ldr	w1, [x19, 32]
	mov	w2, 1
	str	w1, [x19, 64]
	mov	w1, 8
	ldr	w0, [x19, 112]
	ldr	w3, [x20, 3564]
	str	w3, [x19, 124]
	add	w0, w0, 511
	and	w0, w0, -512
	ldr	w3, [x20, 3560]
	lsr	w0, w0, 5
	strb	w2, [x19, 371]
	strb	w2, [x19, 369]
	str	w3, [x19, 120]
	str	w0, [x19, 144]
	str	w1, [x19, 380]
	str	w1, [x19, 384]
	str	w1, [x19, 388]
.L50:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L65:
	orr	w0, w0, 131072
	str	w0, [x19, 108]
	mov	w1, 0
	ldr	x2, [x21]
	ldr	w2, [x2, 632]
	cmp	w2, 1
	bne	.L57
.L66:
	and	w2, w0, 131072
	mov	w1, 262144
	cmp	w2, wzr
	csel	w1, wzr, w1, ne
	b	.L57
	.p2align 3
.L64:
	mov	w0, 4
	strb	w0, [x19, 364]
	ldr	w1, [x19, 108]
	mov	w0, 4352
	ldrb	w3, [x21, 24]
	ubfx	x2, x1, 10, 2
	add	w1, w3, 1
	orr	w1, w1, w2
	cmp	w1, 3
	orr	w1, w0, w1, lsl 10
	csel	w0, w1, w0, ls
	str	w0, [x19, 108]
	b	.L54
	.size	VC1_SetImgFormat, .-VC1_SetImgFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1DEC_Destroy
	.type	VC1DEC_Destroy, %function
VC1DEC_Destroy:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VC1DEC_Destroy, .-VC1DEC_Destroy
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1DEC_RecycleImage
	.type	VC1DEC_RecycleImage, %function
VC1DEC_RecycleImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cbz	x19, .L70
	add	x19, x19, 53248
	mov	w1, w20
	mov	w2, 0
	ldr	w0, [x19, 800]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 800]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L71
	ldr	w0, [x19, 800]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 800]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L70:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L71:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1DEC_RecycleImage, .-VC1DEC_RecycleImage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1DEC_GetRemainImg
	.type	VC1DEC_GetRemainImg, %function
VC1DEC_GetRemainImg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 12288
	add	x19, x19, 2520
	bl	_mcount
	ldr	x0, [x19, 24000]
	cbz	x0, .L79
	ldr	x0, [x20]
	add	x21, x20, 53248
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L74
	add	x23, x19, 20480
	add	x21, x20, 53248
	ldr	w1, [x23, 3616]
	ldr	w0, [x21, 800]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L74
	ldr	w0, [x21, 800]
	ldr	w1, [x23, 3616]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L88
.L74:
	add	x19, x21, 192
.L78:
	mov	x0, x19
	bl	GetVoLastImageID
.L85:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L88:
	mov	x0, x20
	bl	VC1_SetImgFormat
	str	xzr, [x22, 560]
	mov	w2, 1
	str	xzr, [x22, 568]
	str	w2, [x22, 188]
	ldr	w1, [x22, 156]
	ldr	w0, [x21, 800]
	bl	FSP_SetDisplay
	ldr	x0, [x19, 24000]
	ldrb	w1, [x0]
	cmp	w1, 1
	beq	.L89
.L75:
	add	x19, x21, 192
	ldr	w0, [x21, 800]
	mov	x4, x22
	mov	x3, x19
	mov	x2, x20
	mov	w1, 1
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L76
	ldr	w1, [x22, 156]
	mov	x0, x20
	bl	VC1DEC_RecycleImage
	ldr	w0, [x21, 800]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	ldr	x1, [x22, 568]
	ldr	w0, [x21, 800]
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	b	.L78
	.p2align 3
.L76:
	ldr	w0, [x22, 160]
	cmp	w0, 100
	bhi	.L74
	ldr	w0, [x22, 112]
	cmp	w0, 31
	bls	.L74
	ldr	w1, [x22, 116]
	mov	w0, 0
	cmp	w1, 31
	bhi	.L85
	b	.L78
	.p2align 3
.L89:
	strb	w1, [x0, 1]
	mov	w0, 3
	ldr	x1, [x19, 24000]
	strb	wzr, [x1]
	strb	w0, [x19, 1]
	b	.L75
.L79:
	mov	w0, -1
	b	.L85
	.size	VC1DEC_GetRemainImg, .-VC1DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_ArrangeVahbMem
	.type	VC1_ArrangeVahbMem, %function
VC1_ArrangeVahbMem:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	w21, w1
	mov	w23, w2
	bl	_mcount
	sub	w1, w21, #32
	mov	w0, 8160
	cmp	w1, w0
	bhi	.L91
	sub	w1, w23, #32
	cmp	w1, w0
	bhi	.L91
	add	x0, x22, 28672
	mov	w1, 0
	add	x0, x0, 144
	.p2align 2
.L94:
	str	w1, [x0, 1196]
	add	w1, w1, 1
	strb	wzr, [x0, 1]
	cmp	w1, 5
	strb	wzr, [x0]
	add	x0, x0, 1664
	bne	.L94
	ldr	x0, [x22]
	ldr	w19, [x0, 748]
	ldr	w2, [x0, 28]
	cmp	w2, 25
	cbnz	w19, .L95
	beq	.L121
.L115:
	mov	w19, w1
	mov	w24, 3
	mov	w20, 2
.L96:
	ldr	x0, .LC34
	mov	x2, 32
	mov	w1, 0
	ldr	x3, [x0, 96]
	add	x0, x29, 80
	blr	x3
	strb	wzr, [x29, 80]
	ldr	x0, [x22]
	stp	w21, w23, [x29, 104]
	strb	wzr, [x29, 81]
	ldr	w0, [x0, 28]
	stp	w21, w23, [x29, 88]
	cmp	w0, 24
	beq	.L106
	strb	w19, [x29, 85]
	sxtb	w19, w20
	strb	w24, [x29, 83]
.L111:
	add	x20, x22, 53248
	add	x1, x29, 80
	strb	w19, [x29, 86]
	strb	wzr, [x29, 82]
	ldr	w0, [x20, 800]
	bl	FSP_ConfigInstance
	mov	w19, w0
	cbnz	w0, .L107
	ldr	x1, [x22]
	add	x3, x29, 76
	ldr	w0, [x20, 800]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L122
	cbnz	w0, .L109
	mov	w19, 1
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L91:
	ldr	x1, .LC31
	mov	w3, w23
	mov	w2, w21
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
.L117:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L95:
	bne	.L115
	ldr	w4, [x0, 708]
	ldr	w5, [x0, 712]
	cmp	w21, w4
	bhi	.L103
	cmp	w23, w5
	bhi	.L103
	ldr	w2, [x0, 736]
	mov	w20, 1
	ldr	w1, [x0, 732]
	add	w24, w1, 2
	cbnz	w2, .L123
	ldrsb	w19, [x0, 756]
	b	.L96
	.p2align 3
.L107:
	ldr	x1, .LC38
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L121:
	ldr	w4, [x0, 708]
	cmp	w21, w4
	bhi	.L124
	ldr	w1, [x0, 712]
	cmp	w23, w1
	mov	w5, w1
	bhi	.L98
	add	x2, x22, 12288
	mov	w19, 1
	ldrb	w2, [x2, 2520]
	tbnz	x2, 3, .L117
	ldr	w3, [x0, 736]
	mov	w20, w19
	ldr	w2, [x0, 732]
	add	w24, w2, 2
	cbz	w3, .L101
	add	w2, w2, w19
	mov	w19, 17
	cmp	w2, w19
	csel	w20, w2, w19, ls
.L101:
	ldrsb	w19, [x0, 756]
	mov	w23, w1
	mov	w21, w4
	b	.L96
	.p2align 3
.L106:
	mov	w0, 1
	strb	w19, [x29, 85]
	strb	w0, [x29, 83]
	mov	w19, 1
	b	.L111
	.p2align 3
.L103:
	ldr	x1, .LC33
	mov	w3, w23
	mov	w2, w21
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	b	.L117
	.p2align 3
.L122:
	add	x0, x20, 192
	bl	ResetVoQueue
.L109:
	ldr	x1, .LC36
	mov	w0, 0
	bl	dprint_vfmw
	b	.L117
	.p2align 3
.L123:
	mov	w19, 17
	add	w1, w1, w20
	cmp	w1, w19
	csel	w20, w1, w19, ls
	ldrsb	w19, [x0, 756]
	b	.L96
.L124:
	ldr	w5, [x0, 712]
.L98:
	ldr	x1, .LC33
	mov	w3, w23
	mov	w2, w21
	mov	w0, 0
	bl	dprint_vfmw
	b	.L117
	.size	VC1_ArrangeVahbMem, .-VC1_ArrangeVahbMem
	.align	3
.LC31:
	.xword	.LC30
	.align	3
.LC33:
	.xword	.LC32
	.align	3
.LC34:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC36:
	.xword	.LC35
	.align	3
.LC38:
	.xword	.LC37
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_WritePicMsg
	.type	VC1_WritePicMsg, %function
VC1_WritePicMsg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	add	x20, x22, 12288
	mov	x19, x1
	add	x21, x20, 2520
	bl	_mcount
	add	x22, x22, 53248
	add	x1, x21, 20480
	ldr	w0, [x22, 800]
	ldr	w1, [x1, 3620]
	bl	FSP_GetLogicFs
	cbz	x0, .L150
	ldrsb	w1, [x21, 3104]
	cbz	w1, .L128
	ldr	w2, [x21, 10804]
	add	x1, x21, 8192
	strb	w2, [x19, 5]
	mov	w2, 3
	ldrb	w3, [x1, 2224]
	strb	w3, [x19, 4]
	ldrb	w1, [x1, 2569]
	cmp	w1, 4
	csel	w1, w1, w2, ne
	strb	w1, [x19, 3]
	ldr	w1, [x21, 10804]
	add	x21, x21, 36864
	cmp	w1, w2
	ldr	w1, [x21, 1668]
	beq	.L151
	sub	w1, w1, #1
	strh	w1, [x19, 84]
.L131:
	add	x3, x20, 2520
	add	x2, x3, 36864
	add	x1, x3, 8192
	add	x5, x3, 20480
	ldr	w2, [x2, 1664]
	sub	w2, w2, #1
	strh	w2, [x19, 82]
	ldrb	w2, [x1, 2609]
	strb	w2, [x19, 8]
	ldrb	w2, [x1, 2400]
	strb	w2, [x19, 6]
	ldrb	w2, [x1, 2600]
	strb	w2, [x19, 9]
	ldrb	w2, [x20, 2528]
	strb	w2, [x19, 7]
	ldrsw	x4, [x5, 3584]
	lsl	x2, x4, 3
	sub	x2, x2, x4
	lsl	x2, x2, 1
	sub	x2, x2, x4
	add	x2, x3, x2, lsl 7
	add	x2, x2, 12288
	ldrb	w2, [x2, 2747]
	strb	w2, [x19, 10]
	ldrsw	x4, [x5, 3600]
	lsl	x2, x4, 3
	sub	x2, x2, x4
	lsl	x2, x2, 1
	sub	x2, x2, x4
	add	x2, x3, x2, lsl 7
	add	x2, x2, 12288
	ldrb	w2, [x2, 2747]
	strb	w2, [x19, 11]
	ldrb	w2, [x1, 2598]
	strb	w2, [x19, 15]
	ldrb	w2, [x1, 2401]
	strb	w2, [x19, 13]
	ldrb	w2, [x1, 2405]
	strb	w2, [x19, 14]
	ldrb	w2, [x1, 2568]
	strb	w2, [x19, 12]
	ldrb	w2, [x1, 2736]
	strb	w2, [x19, 18]
	ldrb	w2, [x1, 2571]
	strb	w2, [x19, 19]
	ldrb	w2, [x1, 2578]
	strb	w2, [x19, 17]
	ldr	w2, [x20, 13396]
	strb	w2, [x19, 16]
	ldrb	w2, [x1, 2406]
	cbnz	w2, .L132
	ldrb	w1, [x1, 2578]
	cmp	w1, 8
	cset	w1, ls
	strb	w1, [x19, 20]
.L133:
	add	x2, x20, 2520
	add	x1, x2, 8192
	add	x4, x2, 36864
	ldrb	w3, [x1, 2730]
	strb	w3, [x19, 24]
	ldrb	w3, [x1, 2729]
	strb	w3, [x19, 23]
	ldrb	w3, [x1, 2403]
	strb	w3, [x19, 22]
	ldrb	w3, [x1, 2728]
	strb	w3, [x19, 25]
	ldr	w3, [x20, 13452]
	strb	w3, [x19, 26]
	ldrb	w3, [x1, 2731]
	strb	w3, [x19, 21]
	ldrb	w3, [x1, 2582]
	strb	w3, [x19, 27]
	ldrb	w3, [x1, 2583]
	strb	w3, [x19, 28]
	ldrb	w3, [x1, 2601]
	strb	w3, [x19, 34]
	ldrb	w3, [x1, 2593]
	strb	w3, [x19, 33]
	ldr	x5, [x20, 26520]
	ldrb	w3, [x1, 2609]
	add	x3, x5, x3
	ldrb	w3, [x3, 1029]
	strb	w3, [x19, 30]
	ldrb	w3, [x1, 2609]
	ldr	x5, [x20, 26512]
	add	x3, x5, x3
	ldrb	w3, [x3, 1029]
	strb	w3, [x19, 29]
	ldrb	w3, [x1, 2581]
	strb	w3, [x19, 31]
	ldrb	w3, [x1, 2599]
	strb	w3, [x19, 32]
	ldrb	w3, [x1, 2592]
	strb	w3, [x19, 35]
	ldrb	w3, [x1, 2404]
	strb	w3, [x19, 36]
	ldrb	w3, [x1, 2588]
	strb	w3, [x19, 37]
	ldrb	w3, [x1, 2590]
	strb	w3, [x19, 38]
	ldrb	w3, [x1, 2591]
	strb	w3, [x19, 39]
	ldrb	w3, [x1, 2589]
	strb	w3, [x19, 40]
	ldrb	w3, [x1, 2605]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 46]
	ldrb	w3, [x1, 2603]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 42]
	ldrb	w3, [x1, 2607]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 44]
	ldrb	w3, [x1, 2608]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 41]
	ldrb	w3, [x1, 2602]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 43]
	ldrb	w3, [x1, 2606]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 47]
	ldrb	w3, [x1, 2604]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 45]
	ldrb	w3, [x1, 2570]
	strb	w3, [x19, 50]
	ldrb	w3, [x1, 2587]
	strb	w3, [x19, 49]
	ldrb	w3, [x1, 2586]
	strb	w3, [x19, 48]
	ldrb	w3, [x1, 2597]
	strb	w3, [x19, 53]
	ldrb	w3, [x1, 2594]
	strb	w3, [x19, 54]
	ldrb	w3, [x1, 2595]
	strb	w3, [x19, 51]
	ldrb	w3, [x1, 2596]
	strb	w3, [x19, 52]
	ldr	w3, [x20, 13428]
	str	w3, [x19, 14424]
	ldr	w3, [x4, 1640]
	str	w3, [x19, 14428]
	ldr	w3, [x4, 1644]
	str	w3, [x19, 14432]
	ldrb	w3, [x1, 2409]
	strb	w3, [x19, 55]
	ldrb	w3, [x1, 2410]
	strb	w3, [x19, 56]
	ldrb	w3, [x1, 2411]
	strb	w3, [x19, 57]
	ldrb	w3, [x1, 2412]
	strb	w3, [x19, 58]
	ldr	w2, [x20, 13324]
	cmp	w2, 3
	beq	.L152
	ldrb	w2, [x1, 2409]
	strb	w2, [x0, 373]
	ldrb	w2, [x1, 2410]
	strb	w2, [x0, 374]
	ldrb	w2, [x1, 2411]
	strb	w2, [x0, 375]
	ldrb	w1, [x1, 2412]
	strb	w1, [x0, 376]
.L138:
	add	x1, x20, 2520
	add	x2, x1, 8192
	ldrb	w2, [x2, 2224]
	strb	w2, [x0, 378]
	ldrb	w0, [x20, 2531]
	strb	w0, [x19, 63]
	b	.L139
	.p2align 3
.L128:
	add	x1, x21, 8192
	strb	wzr, [x19, 5]
	add	x3, x21, 36864
	mov	w4, 3
	ldrb	w2, [x1, 2208]
	strb	w2, [x19, 4]
	ldrb	w2, [x1, 2459]
	cmp	w2, 4
	csel	w2, w2, w4, ne
	strb	w2, [x19, 3]
	ldr	w2, [x3, 1668]
	sub	w2, w2, #1
	strh	w2, [x19, 84]
	ldr	w2, [x3, 1664]
	strb	wzr, [x19, 8]
	sub	w2, w2, #1
	strh	w2, [x19, 82]
	ldrb	w2, [x1, 2211]
	strb	w2, [x19, 6]
	strb	wzr, [x19, 9]
	ldrb	w2, [x21, 8]
	strb	w2, [x19, 7]
	strb	wzr, [x19, 10]
	strb	wzr, [x19, 11]
	ldrb	w2, [x1, 2214]
	strb	w2, [x19, 13]
	ldrb	w2, [x1, 2218]
	strb	w2, [x19, 14]
	ldrb	w2, [x1, 2478]
	strb	w2, [x19, 12]
	ldrb	w2, [x1, 2488]
	strb	w2, [x19, 18]
	ldrb	w2, [x1, 2463]
	strb	w2, [x19, 19]
	ldrb	w2, [x1, 2462]
	strb	w2, [x19, 17]
	ldr	w2, [x21, 10752]
	strb	w2, [x19, 16]
	ldrb	w2, [x1, 2222]
	cbnz	w2, .L141
	ldrb	w1, [x1, 2462]
	cmp	w1, 8
	cset	w1, ls
	strb	w1, [x19, 20]
.L142:
	add	x2, x20, 2520
	add	x1, x2, 8192
	ldrb	w3, [x1, 2482]
	strb	w3, [x19, 24]
	ldrb	w3, [x1, 2481]
	strb	w3, [x19, 23]
	ldrb	w3, [x1, 2216]
	strb	w3, [x19, 22]
	ldrb	w3, [x1, 2480]
	strb	w3, [x19, 25]
	ldr	w3, [x20, 13204]
	strb	w3, [x19, 26]
	ldrb	w3, [x1, 2483]
	strb	w3, [x19, 21]
	ldrb	w3, [x1, 2467]
	strb	w3, [x19, 27]
	ldrb	w3, [x1, 2468]
	strb	w3, [x19, 28]
	ldr	x3, [x20, 26520]
	ldrb	w3, [x3, 1029]
	strb	w3, [x19, 30]
	ldr	x3, [x20, 26512]
	ldrb	w3, [x3, 1029]
	strb	w3, [x19, 29]
	ldrb	w3, [x1, 2465]
	strb	w3, [x19, 31]
	ldrb	w3, [x1, 2599]
	strb	w3, [x19, 32]
	ldrb	w3, [x1, 2477]
	strb	w3, [x19, 35]
	ldrb	w3, [x1, 2217]
	strb	w3, [x19, 36]
	ldrb	w3, [x1, 2473]
	strb	w3, [x19, 37]
	ldrb	w3, [x1, 2475]
	strb	w3, [x19, 38]
	ldrb	w3, [x1, 2476]
	strb	w3, [x19, 39]
	ldrb	w3, [x1, 2474]
	strb	w3, [x19, 40]
	ldr	w3, [x20, 13264]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 42]
	ldr	w3, [x20, 13260]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 43]
	ldr	w3, [x20, 13268]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 45]
	ldrb	w3, [x1, 2460]
	strb	w3, [x19, 50]
	ldrb	w3, [x1, 2472]
	strb	w3, [x19, 49]
	ldrb	w3, [x1, 2471]
	strb	w3, [x19, 48]
	ldr	w3, [x20, 13276]
	str	w3, [x19, 14424]
	ldrb	w3, [x1, 2220]
	cmp	w3, 1
	beq	.L153
	mov	w5, 0
	mov	w2, 0
	mov	w4, 0
	mov	w3, 0
.L145:
	add	x1, x20, 2520
	strb	w3, [x19, 59]
	strb	w4, [x19, 60]
	add	x1, x1, 8192
	strb	w2, [x19, 61]
	str	w5, [x0, 400]
	ldrb	w1, [x1, 2208]
	strb	w1, [x0, 378]
.L139:
	add	x2, x20, 2520
	add	x20, x20, 2533
	add	x0, x2, 20480
	add	x1, x2, 36864
	ldr	x5, .LC41
	ldrh	w4, [x19, 82]
	ldr	w3, [x0, 3628]
	strh	w3, [x19, 76]
	add	w4, w4, 1
	ldr	w3, [x0, 3560]
	strh	w3, [x19, 78]
	ldr	w3, [x0, 3564]
	strh	w3, [x19, 80]
	ldrb	w3, [x2, 11]
	strb	w3, [x19, 63]
	ldrb	w2, [x2, 12]
	strb	w2, [x19, 62]
	ldr	w2, [x0, 3544]
	add	w3, w2, 254
	adds	w2, w2, 127
	csel	w2, w3, w2, mi
	asr	w2, w2, 7
	lsl	w2, w2, 4
	str	w2, [x19, 14456]
	ldr	w0, [x1, 1688]
	str	w0, [x19, 14460]
	ldr	w0, [x1, 1692]
	str	w0, [x19, 14464]
	ldr	w0, [x1, 1696]
	str	w0, [x19, 14468]
	ldr	w0, [x1, 1700]
	str	w0, [x19, 14472]
	ldr	w0, [x1, 1704]
	str	w0, [x19, 14476]
	ldr	w0, [x1, 1708]
	str	w0, [x19, 14480]
	ldr	w0, [x1, 1712]
	str	w0, [x19, 14484]
	str	x20, [x19, 64]
	ldrsw	x0, [x22, 800]
	ldr	x0, [x5, x0, lsl 3]
	str	w4, [x0, 1596]
	ldrsw	x1, [x22, 800]
	ldrh	w0, [x19, 84]
	ldr	x1, [x5, x1, lsl 3]
	add	w0, w0, 1
	str	w0, [x1, 1600]
.L125:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L141:
	cmp	w2, 1
	beq	.L154
	cmp	w2, 2
	beq	.L155
	mov	w1, 1
	strb	w1, [x19, 20]
	b	.L142
	.p2align 3
.L132:
	cmp	w2, 1
	beq	.L156
	cmp	w2, 2
	beq	.L157
	mov	w1, 1
	strb	w1, [x19, 20]
	b	.L133
	.p2align 3
.L156:
	ldrb	w1, [x1, 2579]
	cmp	w1, wzr
	cset	w1, ne
	strb	w1, [x19, 20]
	b	.L133
	.p2align 3
.L154:
	ldrb	w1, [x1, 2464]
	cmp	w1, wzr
	cset	w1, ne
	strb	w1, [x19, 20]
	b	.L142
	.p2align 3
.L152:
	ldrb	w2, [x1, 2609]
	cbnz	w2, .L137
	ldrb	w2, [x1, 2409]
	strb	w2, [x0, 373]
	ldrb	w2, [x1, 2410]
	strb	w2, [x0, 374]
	ldrb	w2, [x1, 2411]
	strb	w2, [x0, 375]
	ldrb	w2, [x1, 2412]
	strb	w2, [x0, 376]
	ldrb	w2, [x1, 2609]
.L137:
	cmp	w2, 1
	bne	.L138
	add	x1, x20, 2520
	add	x1, x1, 8192
	ldrb	w2, [x1, 2409]
	strb	w2, [x0, 368]
	ldrb	w2, [x1, 2410]
	strb	w2, [x0, 369]
	ldrb	w2, [x1, 2411]
	strb	w2, [x0, 370]
	ldrb	w1, [x1, 2412]
	strb	w1, [x0, 371]
	b	.L138
	.p2align 3
.L151:
	add	w1, w1, 1
	add	w1, w1, w1, lsr 31
	asr	w1, w1, 1
	sub	w1, w1, #1
	strh	w1, [x19, 84]
	b	.L131
	.p2align 3
.L153:
	add	x4, x2, 20480
	ldrb	w5, [x1, 2459]
	sub	w5, w5, #2
	ldr	w3, [x4, 3584]
	and	w5, w5, -3
	ldr	w6, [x4, 3600]
	lsl	x4, x3, 3
	sub	x4, x4, x3
	lsl	x4, x4, 1
	sub	x4, x4, x3
	lsl	x3, x6, 3
	sub	x3, x3, x6
	lsl	x3, x3, 1
	add	x4, x2, x4, lsl 7
	sub	x3, x3, x6
	add	x4, x4, 12288
	add	x2, x2, x3, lsl 7
	uxtb	w3, w5
	add	x2, x2, 12288
	ldrb	w4, [x4, 1722]
	ldrb	w2, [x2, 1722]
	cbnz	w3, .L146
	mov	w5, w2
	mov	w3, w2
	b	.L145
	.p2align 3
.L146:
	ldrb	w3, [x1, 2458]
	mov	w5, w3
	b	.L145
	.p2align 3
.L155:
	strb	wzr, [x19, 20]
	b	.L142
	.p2align 3
.L157:
	strb	wzr, [x19, 20]
	b	.L133
	.p2align 3
.L150:
	ldr	x1, .LC40
	mov	w2, 1126
	mov	w0, 22
	bl	dprint_vfmw
	b	.L125
	.size	VC1_WritePicMsg, .-VC1_WritePicMsg
	.align	3
.LC40:
	.xword	.LC39
	.align	3
.LC41:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_WriteSliceMsg
	.type	VC1_WriteSliceMsg, %function
VC1_WriteSliceMsg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	add	x2, x19, 20480
	ldr	x4, .LC42
	add	x1, x2, 3632
	add	x0, x20, 88
	ldr	w3, [x2, 3628]
	strb	w3, [x20, 74]
	ldr	w3, [x2, 3628]
	ldr	x4, [x4, 104]
	lsl	x2, x3, 3
	sub	x2, x2, x3
	lsl	x2, x2, 3
	blr	x4
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_WriteSliceMsg, .-VC1_WriteSliceMsg
	.align	3
.LC42:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_CfgVDH
	.type	VC1_CfgVDH, %function
VC1_CfgVDH:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldrsb	w0, [x19, 3104]
	cbz	w0, .L160
	ldr	w0, [x19, 10804]
	cmp	w0, 3
	beq	.L165
.L160:
	add	x0, x19, 36864
	ldr	w0, [x0, 1672]
	sub	w0, w0, #1
	str	w0, [x20, 14504]
.L161:
	add	x4, x19, 20480
	ldr	w1, [x20, 14596]
	mov	w0, 16385
	movk	w0, 0x2001, lsl 16
	ubfiz	w1, w1, 30, 1
	ldr	w3, [x4, 3568]
	cmp	w3, wzr
	add	w2, w3, 63
	csel	w2, w2, w3, lt
	asr	w2, w2, 6
	add	w0, w0, w2, lsl 4
	orr	w0, w0, w1
	str	w0, [x20, 14508]
	ldrsw	x3, [x4, 3600]
	ldrsw	x1, [x4, 3584]
	lsl	x2, x3, 3
	ldr	x4, [x19, 23992]
	sub	x2, x2, x3
	lsl	x0, x1, 3
	lsl	x2, x2, 1
	sub	x0, x0, x1
	sub	x2, x2, x3
	lsl	x0, x0, 1
	sub	x0, x0, x1
	ldr	w4, [x4, 1136]
	add	x2, x19, x2, lsl 7
	add	x0, x19, x0, lsl 7
	and	w4, w4, 3
	ubfiz	w4, w4, 4, 4
	ldr	w3, [x2, 15144]
	ldr	w2, [x0, 15144]
	and	w0, w3, 3
	strb	wzr, [x20, 73]
	and	w2, w2, 3
	orr	w0, w4, w0, lsl 2
	orr	w0, w0, w2
	strb	w0, [x20, 72]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L165:
	add	x0, x19, 36864
	ldr	w1, [x0, 1668]
	ldr	w2, [x0, 1664]
	add	w0, w1, 1
	add	w0, w0, w0, lsr 31
	asr	w0, w0, 1
	mul	w0, w0, w2
	sub	w0, w0, #1
	str	w0, [x20, 14504]
	b	.L161
	.size	VC1_CfgVDH, .-VC1_CfgVDH
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_GenerateDecPara
	.type	VC1_Vfmw_GenerateDecPara, %function
VC1_Vfmw_GenerateDecPara:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VC1_Vfmw_GenerateDecPara, .-VC1_Vfmw_GenerateDecPara
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1DEC_GetImageBuffer
	.type	VC1DEC_GetImageBuffer, %function
VC1DEC_GetImageBuffer:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 12288
	add	x0, x0, 2520
	ldrsb	w1, [x0, 3]
	cmp	w1, 3
	beq	.L178
.L168:
	add	x19, x19, 53248
	ldr	w0, [x19, 800]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L170
	ldr	w0, [x19, 800]
	mov	w20, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	bne	.L169
	ldr	w0, [x19, 800]
	add	x1, x19, 192
	bl	FSP_ClearNotInVoQueue
	b	.L169
	.p2align 3
.L178:
	ldrb	w0, [x0, 9]
	cmp	w0, 1
	beq	.L168
.L170:
	mov	w20, 1
.L169:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1DEC_GetImageBuffer, .-VC1DEC_GetImageBuffer
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_GetImageBuffer
	.type	VC1_Vfmw_GetImageBuffer, %function
VC1_Vfmw_GetImageBuffer:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x22, x21, 53248
	add	x19, x21, 12288
	bl	_mcount
	mov	w1, 1
	ldr	w0, [x22, 800]
	bl	FSP_NewLogicFs
	mov	w1, w0
	add	x0, x19, 2520
	add	x20, x0, 20480
	str	w1, [x20, 3620]
	tbnz	w1, #31, .L199
	add	x20, x21, 28672
	add	x0, x0, 12288
	add	x0, x0, 1720
	ldrh	w2, [x20, 144]
	cbz	w2, .L189
	add	x21, x21, 36864
	add	x0, x20, 1808
	add	x4, x21, 272
.L186:
	ldrh	w3, [x0]
	mov	x2, x0
	add	x0, x0, 1664
	cmp	x0, x4
	cbz	w3, .L183
	bne	.L186
	ldr	w0, [x22, 800]
	mov	w2, 1
	add	x21, x21, 273
	bl	FSP_ClearLogicFs
	ldr	x1, .LC48
	mov	w0, 1
	bl	dprint_vfmw
	add	x0, x20, 145
.L188:
	strb	wzr, [x0]
	strb	wzr, [x0, -1]
	add	x0, x0, 1664
	cmp	x0, x21
	bne	.L188
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L189:
	mov	x2, x0
.L183:
	str	x2, [x19, 26512]
	mov	w0, 1
	mov	w1, 0
	strb	w0, [x2, 1]
	.p2align 2
.L185:
	ldr	x4, [x19, 26512]
	sxtw	x0, w1
	uxtb	w2, w1
	add	w1, w1, 1
	add	x4, x4, x0
	cmp	w1, 256
	strb	w2, [x4, 3]
	ldr	x4, [x19, 26512]
	add	x4, x4, x0
	strb	w2, [x4, 259]
	ldr	x4, [x19, 26512]
	add	x4, x4, x0
	strb	w2, [x4, 515]
	ldr	x3, [x19, 26512]
	add	x0, x3, x0
	strb	w2, [x0, 771]
	bne	.L185
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L199:
	ldr	x1, .LC44
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 3616]
	ldr	w0, [x22, 800]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L181
	ldr	w0, [x22, 800]
	ldr	x1, [x21, 560]
	bl	FreeUsdByDec
	ldr	w0, [x22, 800]
	ldr	x1, [x21, 568]
	bl	FreeUsdByDec
	ldr	w0, [x22, 800]
	ldr	x1, [x21, 576]
	bl	FreeUsdByDec
	ldr	w0, [x22, 800]
	ldr	x1, [x21, 584]
	bl	FreeUsdByDec
	add	x0, x21, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w1, [x20, 3616]
	ldr	w0, [x22, 800]
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 3612]
	mov	w2, 1
	ldr	w0, [x22, 800]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 3612]
	str	wzr, [x20, 3616]
	ldr	w0, [x22, 800]
	bl	FSP_GetNullLogicFs
	str	w0, [x20, 3612]
	str	w0, [x20, 3616]
.L181:
	add	x0, x19, 2520
	add	x1, x22, 192
	add	x0, x0, 20480
	str	wzr, [x0, 3608]
	ldr	w0, [x22, 800]
	bl	FSP_ClearNotInVoQueue
	ldr	x1, .LC46
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VC1_Vfmw_GetImageBuffer, .-VC1_Vfmw_GetImageBuffer
	.align	3
.LC44:
	.xword	.LC43
	.align	3
.LC46:
	.xword	.LC45
	.align	3
.LC48:
	.xword	.LC47
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_MemMove
	.type	VC1_Vfmw_MemMove, %function
VC1_Vfmw_MemMove:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 20480
	ldr	x5, [x19, 23992]
	ldr	x1, [x19, 24000]
	ldr	x4, .LC49
	ldr	w3, [x0, 3548]
	ldr	w2, [x0, 3568]
	add	w3, w3, 1
	ldr	x4, [x4, 104]
	ldr	x1, [x1, 1544]
	add	w3, w3, w3, lsr 31
	ldr	x0, [x5, 1544]
	asr	w3, w3, 1
	lsl	w3, w3, 5
	mul	w2, w3, w2
	lsl	w3, w2, 2
	sub	w2, w3, w2
	asr	w2, w2, 1
	sxtw	x2, w2
	blr	x4
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_Vfmw_MemMove, .-VC1_Vfmw_MemMove
	.align	3
.LC49:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_BuildRefList
	.type	VC1_Vfmw_BuildRefList, %function
VC1_Vfmw_BuildRefList:
	sub	sp, sp, #512
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	mov	w25, w3
	mov	w19, w1
	mov	w24, w2
	bl	_mcount
	mov	x2, 256
	mov	w1, 0
	add	x0, x29, 96
	add	x22, x21, 20480
	bl	memset
	mov	x2, 256
	mov	w1, 0
	add	x0, x29, 352
	bl	memset
	ldr	x20, .LC50
	mov	x2, 16
	mov	w1, 0
	add	x0, x22, 3576
	ldr	x3, [x20, 96]
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 16
	mov	w1, 0
	add	x0, x22, 3592
	blr	x3
	ldrsb	w0, [x21, 4]
	tst	w0, -3
	bne	.L202
	cmp	w19, 1
	beq	.L265
	cmp	w19, 2
	beq	.L266
.L211:
	str	wzr, [x22, 3580]
	str	wzr, [x22, 3596]
.L263:
	mov	w0, 1
.L248:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	add	sp, sp, 512
	ret
	.p2align 3
.L202:
	add	x26, x21, 8192
	ldrb	w0, [x26, 2572]
	cmp	w0, 1
	ldrb	w0, [x26, 2609]
	beq	.L267
	cbz	w0, .L268
.L222:
	ldr	x0, [x21, 24008]
	ldr	w27, [x0, 1196]
	mov	w23, w27
.L221:
	cmp	w19, 1
	beq	.L269
.L223:
	cmp	w19, 2
	bne	.L211
	mov	w0, 1
	strb	wzr, [x22, 3576]
	strb	w0, [x22, 3577]
	str	w19, [x22, 3580]
	str	w19, [x22, 3596]
	cbz	w24, .L237
	cbz	w25, .L238
	ldr	x0, [x21, 24008]
	ldr	x1, [x21, 23992]
	ldr	w7, [x0, 1196]
	str	w7, [x22, 3584]
	b	.L240
	.p2align 3
.L268:
	ldr	x0, [x21, 24000]
	cmp	w19, 1
	ldr	x1, [x21, 24008]
	ldr	w23, [x0, 1196]
	ldr	w27, [x1, 1196]
	bne	.L223
.L269:
	ldrb	w0, [x26, 2600]
	strb	w19, [x22, 3577]
	add	w0, w0, 1
	strb	wzr, [x22, 3576]
	str	w0, [x22, 3580]
	str	wzr, [x22, 3596]
	cbz	w24, .L224
	cbz	w25, .L225
	ldr	x0, [x21, 24008]
	ldr	x1, [x21, 23992]
	ldr	w19, [x0, 1196]
	str	w19, [x22, 3584]
	b	.L227
	.p2align 3
.L265:
	strb	wzr, [x22, 3576]
	str	w19, [x22, 3580]
	str	wzr, [x22, 3596]
	ldr	x0, [x21, 24008]
	ldr	w19, [x0, 1196]
	str	w19, [x22, 3584]
	ldrsb	w0, [x21, 3104]
	cbz	w0, .L204
	ldr	w1, [x21, 10804]
	add	x0, x21, 8192
	cmp	w1, 2
	ldrb	w20, [x0, 2584]
	ldrb	w22, [x0, 2585]
	ldrb	w0, [x0, 2582]
	beq	.L270
.L205:
	cmp	w0, 4
	beq	.L206
.L209:
	mov	w2, 1664
	add	x1, x21, 13
	smull	x2, w19, w2
	add	x2, x2, 12288
	add	x0, x2, 1723
	add	x2, x2, 1979
	add	x0, x21, x0
	add	x2, x21, x2
	.p2align 2
.L207:
	ldrb	w3, [x0]
	add	x0, x0, 1
	strb	w3, [x1]
	add	x1, x1, 1
	cmp	x0, x2
	ldrb	w3, [x0, 255]
	strb	w3, [x1, 255]
	ldrb	w3, [x0, 511]
	strb	w3, [x1, 511]
	ldrb	w3, [x0, 767]
	strb	w3, [x1, 767]
	bne	.L207
	b	.L263
	.p2align 3
.L267:
	cbz	w0, .L222
	ldr	x1, [x21, 24000]
	ldr	x0, [x21, 24008]
	ldr	w27, [x1, 1196]
	ldr	w23, [x0, 1196]
	b	.L221
.L266:
	strb	wzr, [x22, 3576]
	mov	w0, 1
	str	w0, [x22, 3580]
	str	w0, [x22, 3596]
	ldr	x0, [x21, 24008]
	ldr	w19, [x0, 1196]
	str	w19, [x22, 3584]
	strb	wzr, [x22, 3592]
	ldr	x0, [x21, 24000]
	ldr	w20, [x0, 1196]
	str	w20, [x22, 3600]
	ldrsb	w0, [x21, 3104]
	cbz	w0, .L212
	add	x0, x21, 8192
	ldrb	w22, [x0, 2584]
	ldrb	w23, [x0, 2585]
	ldrb	w0, [x0, 2582]
.L213:
	cmp	w0, 4
	beq	.L214
.L217:
	mov	w1, 1664
	mov	x3, 14011
	add	x0, x21, 13
	add	x4, x21, 269
	smaddl	x2, w19, w1, x3
	smaddl	x1, w20, w1, x3
	add	x2, x21, x2
	add	x1, x21, x1
	.p2align 2
.L215:
	add	x0, x0, 1
	ldrb	w3, [x2]
	add	x2, x2, 1
	add	x1, x1, 1
	cmp	x0, x4
	strb	w3, [x0, -1]
	ldrb	w3, [x2, 255]
	strb	w3, [x0, 255]
	ldrb	w3, [x2, 511]
	strb	w3, [x0, 511]
	ldrb	w3, [x2, 767]
	strb	w3, [x0, 767]
	ldrb	w3, [x1, -1]
	strb	w3, [x0, 1023]
	ldrb	w3, [x1, 255]
	strb	w3, [x0, 1279]
	ldrb	w3, [x1, 511]
	strb	w3, [x0, 1535]
	ldrb	w3, [x1, 767]
	strb	w3, [x0, 1791]
	bne	.L215
	b	.L263
.L204:
	add	x0, x21, 8192
	ldrb	w20, [x0, 2469]
	ldrb	w22, [x0, 2470]
	ldrb	w0, [x0, 2467]
	b	.L205
.L224:
	ldr	x0, [x21, 24008]
	ldr	w19, [x0, 1196]
	str	w19, [x22, 3584]
	ldr	x1, [x21, 23992]
.L227:
	ldr	w0, [x0, 1196]
	str	w0, [x22, 3588]
	ldr	w0, [x21, 10888]
	ldr	w20, [x1, 1196]
	cmp	w0, 1
	bls	.L271
.L228:
	tst	w0, -3
	bne	.L229
	ldrb	w1, [x26, 2582]
	cmp	w1, 4
	beq	.L272
.L229:
	mov	w3, 1664
	mov	x2, 14011
	add	x0, x21, 13
	add	x4, x21, 269
	smaddl	x1, w19, w3, x2
	smaddl	x2, w20, w3, x2
	add	x1, x21, x1
	add	x2, x21, x2
	.p2align 2
.L236:
	add	x0, x0, 1
	ldrb	w3, [x1]
	add	x1, x1, 1
	add	x2, x2, 1
	cmp	x0, x4
	strb	w3, [x0, -1]
	ldrb	w3, [x1, 255]
	strb	w3, [x0, 255]
	ldrb	w3, [x1, 511]
	strb	w3, [x0, 511]
	ldrb	w3, [x1, 767]
	strb	w3, [x0, 767]
	ldrb	w3, [x2, -1]
	strb	w3, [x0, 2047]
	ldrb	w3, [x2, 255]
	strb	w3, [x0, 2303]
	ldrb	w3, [x2, 511]
	strb	w3, [x0, 2559]
	ldrb	w3, [x2, 767]
	strb	w3, [x0, 2815]
	bne	.L236
	b	.L263
.L212:
	add	x0, x21, 8192
	ldrb	w22, [x0, 2469]
	ldrb	w23, [x0, 2470]
	ldrb	w0, [x0, 2467]
	b	.L213
.L237:
	ldr	x0, [x21, 24008]
	ldr	w7, [x0, 1196]
	str	w7, [x22, 3584]
	ldr	x1, [x21, 23992]
.L240:
	strb	wzr, [x22, 3592]
	mov	w0, 1
	strb	w0, [x22, 3593]
	ldr	x0, [x21, 24000]
	ldr	w6, [x0, 1196]
	str	w6, [x22, 3600]
	ldr	w0, [x0, 1196]
	str	w0, [x22, 3604]
	ldr	w0, [x21, 10888]
	ldr	w1, [x1, 1196]
	cmp	w0, 1
	bls	.L273
	ands	w0, w0, -3
	beq	.L274
.L242:
	mov	w8, 1664
	mov	x0, x21
	mov	x4, 0
	smull	x7, w7, w8
	smull	x6, w6, w8
	smull	x8, w1, w8
	.p2align 2
.L243:
	add	x3, x4, x7
	add	x0, x0, 1
	add	x3, x21, x3
	add	x2, x4, x6
	add	x3, x3, 12288
	add	x2, x21, x2
	add	x2, x2, 12288
	add	x1, x4, x8
	add	x1, x21, x1
	add	x4, x4, 1
	ldrb	w5, [x3, 1723]
	add	x1, x1, 12288
	strb	w5, [x0, 12]
	cmp	x4, 256
	ldrb	w5, [x3, 1979]
	strb	w5, [x0, 268]
	ldrb	w5, [x3, 2235]
	strb	w5, [x0, 524]
	ldrb	w3, [x3, 2491]
	strb	w3, [x0, 780]
	ldrb	w3, [x2, 1723]
	strb	w3, [x0, 1036]
	ldrb	w3, [x2, 1979]
	strb	w3, [x0, 1292]
	ldrb	w3, [x2, 2235]
	strb	w3, [x0, 1548]
	ldrb	w2, [x2, 2491]
	strb	w2, [x0, 1804]
	ldrb	w2, [x1, 1723]
	strb	w2, [x0, 2060]
	ldrb	w2, [x1, 1979]
	strb	w2, [x0, 2316]
	ldrb	w2, [x1, 2235]
	strb	w2, [x0, 2572]
	ldrb	w1, [x1, 2491]
	strb	w1, [x0, 2828]
	bne	.L243
	b	.L263
.L274:
	ldrb	w2, [x26, 2582]
	cmp	w2, 4
	bne	.L242
	b	.L248
.L225:
	ldr	x1, [x21, 23992]
	ldr	x0, [x21, 24008]
	ldr	w19, [x1, 1196]
	str	w19, [x22, 3584]
	b	.L227
.L238:
	ldr	x1, [x21, 23992]
	ldr	w7, [x1, 1196]
	str	w7, [x22, 3584]
	b	.L240
.L272:
	cbnz	w0, .L233
	ldr	w22, [x21, 10892]
	ldr	w24, [x21, 10896]
	uxtb	w22, w22
	uxtb	w24, w24
.L234:
	mov	w3, 0
	mov	w2, w24
	add	x0, x29, 96
	mov	w1, w22
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w2, w24
	mov	w1, w22
	add	x0, x29, 352
	mov	w3, 1
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w2, 1664
	sub	x0, x29, #3488
	smull	x2, w23, w2
	add	x2, x2, 12288
	add	x1, x2, 2235
	add	x2, x2, 2491
	add	x1, x21, x1
	add	x4, x21, x2
	.p2align 2
.L235:
	ldrb	w3, [x1]
	add	x1, x1, 1
	ldrb	w2, [x1, 255]
	cmp	x1, x4
	add	x3, x0, x3
	add	x2, x0, x2
	ldrb	w3, [x3, 3584]
	ldrb	w2, [x2, 3840]
	strb	w3, [x1, -1]
	strb	w2, [x1, 255]
	bne	.L235
	b	.L229
.L271:
	ldrb	w0, [x26, 2582]
	cmp	w0, 4
	bne	.L229
	ldrb	w22, [x26, 2584]
	mov	w3, 0
	ldrb	w24, [x26, 2585]
	add	x0, x29, 96
	mov	w1, w22
	mov	w2, w24
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w1, w22
	mov	w3, 1
	add	x0, x29, 352
	mov	w2, w24
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w0, 1664
	smull	x0, w27, w0
	add	x0, x0, 12288
	add	x1, x0, 1723
	add	x0, x0, 1979
	add	x3, x21, x0
	add	x1, x21, x1
	sub	x0, x29, #3488
	.p2align 2
.L230:
	ldrb	w2, [x1]
	add	x1, x1, 1
	add	x2, x0, x2
	ldrb	w2, [x2, 3584]
	strb	w2, [x1, -1]
	ldrb	w2, [x1, 255]
	add	x2, x0, x2
	ldrb	w2, [x2, 3840]
	strb	w2, [x1, 255]
	cmp	x1, x3
	bne	.L230
	ldr	w0, [x21, 10888]
	b	.L228
.L273:
	ldrb	w0, [x26, 2582]
	cmp	w0, 4
	bne	.L242
	mov	w0, 0
	b	.L248
.L270:
	ldr	w1, [x21, 10880]
	cmp	w1, 1
	bne	.L205
.L206:
	mov	w3, 0
	mov	w2, w22
	mov	w1, w20
	add	x0, x29, 96
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w2, w22
	mov	w1, w20
	add	x0, x29, 352
	mov	w3, 1
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w2, 1664
	sub	x0, x29, #3488
	smull	x2, w19, w2
	add	x2, x2, 12288
	add	x1, x2, 1723
	add	x2, x2, 1979
	add	x1, x21, x1
	add	x6, x21, x2
	.p2align 2
.L208:
	ldrb	w5, [x1]
	add	x1, x1, 1
	ldrb	w4, [x1, 255]
	ldrb	w3, [x1, 511]
	add	x5, x0, x5
	ldrb	w2, [x1, 767]
	add	x4, x0, x4
	add	x3, x0, x3
	cmp	x1, x6
	add	x2, x0, x2
	ldrb	w5, [x5, 3584]
	ldrb	w4, [x4, 3840]
	ldrb	w3, [x3, 3584]
	ldrb	w2, [x2, 3840]
	strb	w5, [x1, -1]
	strb	w4, [x1, 255]
	strb	w3, [x1, 511]
	strb	w2, [x1, 767]
	bne	.L208
	b	.L209
.L214:
	mov	w3, 0
	mov	w2, w23
	mov	w1, w22
	add	x0, x29, 96
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w3, 1
	mov	w2, w23
	mov	w1, w22
	add	x0, x29, 352
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	w1, 1664
	mov	x2, 14011
	sub	x0, x29, #3488
	smull	x3, w19, w1
	smaddl	x2, w20, w1, x2
	add	x3, x3, 12288
	add	x1, x3, 1723
	add	x3, x3, 1979
	add	x1, x21, x1
	add	x2, x21, x2
	add	x7, x21, x3
	.p2align 2
.L216:
	ldrb	w6, [x1]
	add	x1, x1, 1
	ldrb	w5, [x1, 255]
	add	x2, x2, 1
	ldrb	w4, [x1, 511]
	add	x6, x0, x6
	ldrb	w3, [x1, 767]
	add	x5, x0, x5
	add	x4, x0, x4
	cmp	x1, x7
	add	x3, x0, x3
	ldrb	w6, [x6, 3584]
	ldrb	w5, [x5, 3840]
	ldrb	w4, [x4, 3584]
	ldrb	w3, [x3, 3840]
	strb	w6, [x1, -1]
	strb	w5, [x1, 255]
	strb	w4, [x1, 511]
	strb	w3, [x1, 767]
	ldrb	w6, [x2, -1]
	ldrb	w5, [x2, 255]
	ldrb	w4, [x2, 511]
	add	x6, x0, x6
	ldrb	w3, [x2, 767]
	add	x5, x0, x5
	add	x4, x0, x4
	add	x3, x0, x3
	ldrb	w6, [x6, 3584]
	ldrb	w5, [x5, 3840]
	ldrb	w4, [x4, 3584]
	ldrb	w3, [x3, 3840]
	strb	w6, [x2, -1]
	strb	w5, [x2, 255]
	strb	w4, [x2, 511]
	strb	w3, [x2, 767]
	bne	.L216
	b	.L217
.L233:
	ldrb	w22, [x26, 2584]
	ldrb	w24, [x26, 2585]
	b	.L234
	.size	VC1_Vfmw_BuildRefList, .-VC1_Vfmw_BuildRefList
	.align	3
.LC50:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_GetStreamInfo
	.type	VC1_Vfmw_GetStreamInfo, %function
VC1_Vfmw_GetStreamInfo:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	add	x19, x21, 12288
	bl	_mcount
	add	x0, x19, 2520
	add	x1, x0, 20480
	ldrsb	w2, [x0, 3104]
	ldr	w22, [x1, 3544]
	cbz	w2, .L276
	ldr	w0, [x19, 13324]
	ldr	w20, [x1, 3548]
	cmp	w0, 3
	beq	.L361
.L358:
	ldrh	w0, [x19, 5878]
	ldrh	w1, [x19, 5876]
	add	w0, w0, 15
	add	w1, w1, 15
	asr	w0, w0, 4
	mul	w20, w20, w22
	asr	w1, w1, 4
.L328:
	lsl	w2, w1, 4
	mov	w4, 8160
	sub	w3, w2, #32
	cmp	w3, w4
	lsl	w3, w0, 4
	bhi	.L279
	sub	w5, w3, #32
	cmp	w5, w4
	bhi	.L279
	add	x4, x19, 2520
	add	x2, x4, 20480
	ldr	w3, [x2, 3556]
	cmp	w0, w3
	beq	.L362
.L282:
	add	x1, x19, 2520
	lsl	w0, w22, 4
	add	x2, x1, 20480
	add	w0, w0, 63
	and	w0, w0, -64
	mov	w3, 1
	ldr	w4, [x2, 3548]
	str	w22, [x2, 3552]
	str	w4, [x2, 3556]
	str	w0, [x2, 3568]
	ldrb	w0, [x19, 2528]
	cbz	w0, .L290
.L284:
	ldrsb	w1, [x19, 2523]
	cmp	w1, 3
	beq	.L363
.L287:
	ldrb	w1, [x19, 2520]
	eor	x1, x1, 8
	ubfx	x1, x1, 3, 1
	orr	w0, w3, w1
	cbnz	w0, .L290
.L294:
	mov	x0, x21
	bl	VC1_Vfmw_GetImageBuffer
	mov	w23, w0
	cbz	w0, .L364
	add	x1, x19, 2520
	add	x23, x21, 53248
	add	x1, x1, 20480
	ldr	w0, [x23, 800]
	ldr	w1, [x1, 3620]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L296
	ldr	x3, [x21]
	mov	x4, -1
	ldr	x1, .LC57
	mov	w0, 29
	ldr	x3, [x3, 56]
	str	x3, [x2, 600]
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
.L296:
	add	x0, x19, 2520
	ldrsb	w1, [x0, 3104]
	cbz	w1, .L297
	ldr	w1, [x19, 13324]
	cmp	w1, 3
	beq	.L365
.L297:
	ldr	x0, [x19, 26512]
	str	wzr, [x0, 1136]
.L298:
	ldr	x1, [x19, 26512]
	add	x0, x19, 2520
	strb	wzr, [x1, 2]
	ldrsb	w1, [x0, 3104]
	cbnz	w1, .L299
	add	x1, x0, 8192
	ldrb	w2, [x1, 2220]
	cmp	w2, 1
	beq	.L366
.L299:
	add	x24, x19, 2520
	ldrb	w0, [x24, 2]
	cmp	w0, 1
	ccmp	w0, 5, 4, hi
	bne	.L300
	add	x24, x24, 20480
	mov	w0, 2
	ldr	w1, [x24, 3608]
	ldr	w2, [x24, 3612]
	ldr	w3, [x24, 3616]
	cmp	w1, wzr
	csinc	w0, w0, wzr, ne
	str	w0, [x24, 3608]
	cmp	w2, w3
	beq	.L302
	ldr	x1, .LC60
	mov	w0, 13
	bl	dprint_vfmw
	ldr	w0, [x23, 800]
	mov	w2, 0
	ldr	w1, [x24, 3612]
	bl	FSP_SetRef
	ldr	w3, [x24, 3616]
.L302:
	add	x24, x19, 2520
	mov	w2, 1
	add	x25, x24, 20480
	ldr	w1, [x25, 3620]
	str	w3, [x25, 3612]
	str	w1, [x25, 3616]
	ldr	w0, [x23, 800]
	bl	FSP_SetRef
	ldr	w0, [x23, 800]
	ldr	w1, [x25, 3612]
	bl	FSP_RemovePmv
	ldrb	w0, [x24, 2]
	cmp	w0, 1
	ccmp	w0, 5, 4, hi
	bne	.L304
	ldr	x0, [x24, 24008]
	mov	w1, 1
	strb	wzr, [x0]
	ldr	x0, [x24, 23992]
	strb	w1, [x0]
	ldr	x1, [x24, 24000]
	ldr	x0, [x24, 23992]
	str	x0, [x24, 24000]
	str	x1, [x24, 24008]
.L304:
	add	x25, x19, 2520
	ldr	w0, [x23, 800]
	add	x26, x25, 20480
	ldr	w1, [x26, 3620]
	bl	FSP_GetLogicFs
	mov	x24, x0
	ldr	w1, [x26, 3612]
	ldr	w0, [x23, 800]
	bl	FSP_GetLogicFs
	mov	x28, x0
	ldr	w1, [x26, 3616]
	ldr	w0, [x23, 800]
	bl	FSP_GetLogicFs
	mov	x27, x0
	cmp	x24, xzr
	ccmp	x28, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L367
	ldrsb	w0, [x25, 3104]
	cbnz	w0, .L368
.L310:
	str	wzr, [x24, 104]
.L311:
	ldrb	w1, [x19, 2522]
	add	x0, x19, 2520
	cmp	w1, 5
	beq	.L369
.L312:
	ldr	x0, [x24, 640]
	cbz	x0, .L313
	ldr	x1, [x24, 656]
	cbz	x1, .L313
	ldr	x1, [x24, 648]
	cbz	x1, .L313
	ldr	x1, [x27, 640]
	cbz	x1, .L315
	ldr	w0, [x1, 16]
.L316:
	str	w0, [x21, 14460]
	ldr	x0, [x28, 640]
	cbz	x0, .L370
.L359:
	ldr	w0, [x0, 16]
	str	w0, [x21, 14456]
	ldr	x0, [x24, 640]
	ldr	w0, [x0, 16]
	str	w0, [x21, 14580]
	ldr	w0, [x24, 664]
	str	w0, [x21, 14464]
	ldr	w0, [x27, 664]
	str	w0, [x21, 14468]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 16]
	str	w0, [x21, 14452]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 32]
	str	w0, [x21, 14532]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 76]
	str	w0, [x21, 14536]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 60]
	str	w0, [x21, 14540]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 64]
	str	w0, [x21, 14544]
	ldr	x0, [x24, 640]
	ldr	x24, [x19, 26512]
	ldr	w0, [x0, 16]
	bl	MEM_Phy2Vir
	str	x0, [x24, 1544]
	ldr	x24, [x19, 26528]
	ldr	w0, [x21, 14456]
	bl	MEM_Phy2Vir
	str	x0, [x24, 1544]
	ldr	x23, [x19, 26520]
	ldr	w0, [x21, 14460]
	bl	MEM_Phy2Vir
	str	x0, [x23, 1544]
.L289:
	add	x0, x19, 2520
	ldr	x2, [x0, 23992]
	ldrsb	w1, [x0, 3104]
	cbnz	w1, .L319
	strb	wzr, [x2, 1027]
	add	x1, x0, 8192
	ldr	x2, [x0, 23992]
	ldrb	w3, [x1, 2459]
	strb	w3, [x2, 1028]
	ldrb	w3, [x1, 2467]
	ldr	x2, [x0, 23992]
	sub	w0, w3, #2
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L333
	cmp	w3, 4
	mov	w0, 0
	beq	.L371
.L320:
	add	x24, x19, 2520
	strb	w0, [x2, 1030]
	add	x20, x24, 20480
	strb	w0, [x2, 1029]
	mov	w0, 1
	add	x22, x24, 36864
	strb	wzr, [x20, 3648]
	str	w0, [x20, 3628]
	ldr	x0, [x19, 5656]
	ldr	x23, [x22, 1592]
	bl	BsPos
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w0, w1, w0, lt
	asr	w0, w0, 3
	add	x0, x23, x0, sxtw
	str	x0, [x19, 26632]
	ldr	x0, [x19, 5656]
	ldr	w23, [x22, 1612]
	bl	BsPos
	add	w1, w0, 7
	cmp	w0, wzr
	csel	w0, w1, w0, lt
	add	w0, w23, w0, asr 3
	str	w0, [x20, 3652]
	ldr	x0, [x19, 5656]
	bl	BsPos
	and	w0, w0, 7
	str	w0, [x20, 3656]
	ldr	w23, [x22, 1620]
	ldr	x0, [x19, 5656]
	lsl	w23, w23, 3
	bl	BsPos
	sub	w0, w23, w0
	str	w0, [x20, 3660]
	ldr	w1, [x22, 1668]
	ldr	w0, [x22, 1664]
	mul	w0, w0, w1
	sub	w0, w0, #1
	str	w0, [x20, 3664]
	ldrb	w1, [x19, 2522]
.L321:
	add	x19, x19, 2520
	mov	w3, 1
	mov	w2, 0
	mov	x0, x19
	bl	VC1_Vfmw_BuildRefList
	mov	w23, w0
	cbz	w0, .L372
	mov	w23, 1
	b	.L357
	.p2align 3
.L279:
	ldr	x1, .LC51
	mov	w0, 0
	mov	w23, 0
	bl	dprint_vfmw
.L357:
	mov	w0, w23
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L276:
	ldr	w0, [x1, 3548]
	mov	w1, w22
	mul	w20, w22, w0
	b	.L328
	.p2align 3
.L361:
	add	w20, w20, 1
	add	w20, w20, w20, lsr 31
	asr	w20, w20, 1
	b	.L358
	.p2align 3
.L290:
	add	x0, x19, 2520
	add	x23, x21, 53248
	add	x25, x0, 20480
	add	x1, x25, 1848
	str	x1, [x19, 26512]
	str	x1, [x19, 26520]
	str	x1, [x19, 26528]
	str	x1, [x19, 26536]
	ldr	w0, [x23, 800]
	ldr	w1, [x25, 3616]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	cbz	x0, .L286
	ldr	w0, [x23, 800]
	ldr	x1, [x24, 560]
	bl	FreeUsdByDec
	ldr	w0, [x23, 800]
	ldr	x1, [x24, 568]
	bl	FreeUsdByDec
	ldr	w0, [x23, 800]
	ldr	x1, [x24, 576]
	bl	FreeUsdByDec
	ldr	w0, [x23, 800]
	ldr	x1, [x24, 584]
	bl	FreeUsdByDec
	add	x0, x24, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x23, 800]
	ldr	w1, [x25, 3616]
	bl	FSP_ClearLogicFs
	ldr	w0, [x23, 800]
	mov	w2, 1
	ldr	w1, [x25, 3612]
	bl	FSP_ClearLogicFs
	str	wzr, [x25, 3612]
	str	wzr, [x25, 3616]
	ldr	w0, [x23, 800]
	bl	FSP_GetNullLogicFs
	str	w0, [x25, 3612]
	str	w0, [x25, 3616]
.L286:
	add	x2, x19, 2520
	add	x0, x21, 28672
	add	x2, x2, 20480
	add	x1, x21, 36864
	add	x0, x0, 145
	add	x1, x1, 273
	str	wzr, [x2, 3608]
	.p2align 2
.L292:
	strb	wzr, [x0]
	strb	wzr, [x0, -1]
	add	x0, x0, 1664
	cmp	x1, x0
	bne	.L292
	add	x24, x23, 192
	ldr	w0, [x23, 800]
	mov	x1, x24
	bl	FSP_ClearNotInVoQueue
	ldrb	w0, [x19, 2520]
	tbnz	x0, 3, .L294
	mov	x0, x24
	bl	ResetVoQueue
	ldr	x1, [x21]
	ldr	w0, [x1, 708]
	ldr	w3, [x1, 712]
	adds	w1, w0, 15
	add	w0, w0, 30
	add	w2, w3, 30
	csel	w1, w0, w1, mi
	adds	w0, w3, 15
	asr	w1, w1, 4
	csel	w0, w2, w0, mi
	str	w1, [x21, 14620]
	asr	w0, w0, 4
	mov	w3, 2
	add	w2, w0, 1
	str	w0, [x21, 14624]
	lsl	w1, w1, 4
	mov	x0, x21
	add	w2, w2, w2, lsr 31
	asr	w2, w2, 1
	lsl	w2, w2, 5
	bl	VC1_ArrangeVahbMem
	cmp	w0, 1
	bne	.L293
	ldrb	w0, [x19, 2520]
	orr	w0, w0, 8
	strb	w0, [x19, 2520]
	b	.L294
	.p2align 3
.L364:
	ldr	x1, .LC55
	mov	w0, 1
	bl	dprint_vfmw
	b	.L357
	.p2align 3
.L362:
	ldr	w0, [x2, 3552]
	cmp	w1, w0
	bne	.L282
	lsl	w0, w22, 4
	mov	w3, 0
	add	w0, w0, 63
	and	w0, w0, -64
	str	w0, [x2, 3568]
	ldrb	w0, [x4, 8]
	cbz	w0, .L287
	b	.L284
	.p2align 3
.L300:
	add	x0, x24, 20480
	ldr	w1, [x0, 3608]
	cmp	w1, 1
	bhi	.L304
	ldr	w1, [x0, 3620]
	mov	w2, 1
	ldr	w0, [x23, 800]
	mov	w23, 0
	bl	FSP_ClearLogicFs
	ldr	x2, [x24, 23992]
	mov	w0, 1
	ldr	x1, .LC62
	strb	wzr, [x2]
	ldr	x2, [x24, 23992]
	strb	wzr, [x2, 1]
	bl	dprint_vfmw
	b	.L357
	.p2align 3
.L367:
	ldr	w0, [x23, 800]
	mov	w2, 1
	ldr	w1, [x26, 3620]
	bl	FSP_ClearLogicFs
	ldr	x0, [x25, 23992]
	mov	x5, x27
	ldr	x1, .LC64
	mov	x4, x28
	mov	x3, x24
	mov	w2, 2406
	strb	wzr, [x0]
	mov	w0, 0
	ldr	x6, [x25, 23992]
	strb	wzr, [x6, 1]
	bl	dprint_vfmw
	ldrb	w0, [x25, 2]
	cmp	w0, 5
	beq	.L373
.L308:
	ldr	x1, .LC66
	mov	w0, 1
	mov	w23, 0
	bl	dprint_vfmw
	b	.L357
	.p2align 3
.L319:
	ldr	w3, [x0, 10804]
	add	x1, x0, 8192
	strb	w3, [x2, 1027]
	ldr	x2, [x0, 23992]
	ldrb	w3, [x1, 2569]
	strb	w3, [x2, 1028]
	ldrb	w3, [x1, 2582]
	ldr	x4, [x0, 23992]
	sub	w2, w3, #2
	ldrb	w0, [x1, 2609]
	uxtb	w2, w2
	cmp	w2, 1
	bls	.L335
	cmp	w3, 4
	mov	w2, 0
	beq	.L374
.L322:
	add	x0, x4, x0, sxtw
	add	x3, x19, 2520
	add	x1, x3, 20480
	strb	w2, [x0, 1029]
	ldr	w5, [x1, 3628]
	cbz	w5, .L323
	sub	w5, w5, #1
	mov	w1, 0
	mov	w4, w5
	cbz	w5, .L360
	.p2align 2
.L348:
	sxtw	x2, w1
	lsl	x0, x2, 3
	sub	x0, x0, x2
	lsl	x0, x0, 3
	add	x2, x3, x0
	add	x0, x0, 20480
	add	x2, x2, 20480
	add	x0, x3, x0
	strb	w1, [x2, 3648]
	add	w1, w1, 1
	cmp	w4, w1
	ldr	w0, [x0, 3720]
	mul	w0, w22, w0
	sub	w0, w0, #1
	str	w0, [x2, 3664]
	bne	.L348
	uxtb	w4, w4
.L325:
	lsl	x0, x5, 3
	add	x2, x19, 2520
	sub	x0, x0, x5
	sub	w20, w20, #1
	add	x0, x2, x0, lsl 3
	add	x0, x0, 20480
	strb	w4, [x0, 3648]
	str	w20, [x0, 3664]
	ldrb	w1, [x19, 2522]
	cmp	w1, 2
	bne	.L321
	add	x0, x2, 8192
	ldr	w3, [x19, 13428]
	add	x2, x2, 36864
	ldrb	w0, [x0, 2599]
	mul	w3, w0, w3
	asr	w3, w3, 8
	str	w3, [x2, 1640]
	sub	w0, w0, w3
	subs	w0, w0, #1
	csel	w0, w0, wzr, pl
	str	w0, [x2, 1644]
	b	.L321
	.p2align 3
.L368:
	ldr	w0, [x25, 10804]
	cmp	w0, 3
	bne	.L310
	ldr	x0, .LC58
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L310
	str	w0, [x24, 104]
	b	.L311
	.p2align 3
.L373:
	add	x25, x25, 36864
	ldr	w0, [x25, 1684]
	cmp	w0, wzr
	ble	.L308
	sub	w0, w0, #1
	str	w0, [x25, 1684]
	b	.L308
	.p2align 3
.L363:
	ldrb	w0, [x19, 2529]
	cmp	w0, 1
	bne	.L289
	b	.L287
	.p2align 3
.L372:
	add	x0, x19, 20480
	add	x21, x21, 53248
	mov	w2, 1
	ldr	w1, [x0, 3620]
	ldr	w0, [x21, 800]
	bl	FSP_ClearLogicFs
	ldr	x2, [x19, 23992]
	mov	w0, 1
	ldr	x1, .LC70
	strb	wzr, [x2]
	ldr	x2, [x19, 23992]
	strb	wzr, [x2, 1]
	bl	dprint_vfmw
	b	.L357
	.p2align 3
.L313:
	add	x19, x19, 2520
	ldr	w0, [x23, 800]
	add	x20, x19, 20480
	mov	w2, 1
	ldr	w1, [x20, 3620]
	bl	FSP_ClearLogicFs
	ldr	x2, [x19, 23992]
	mov	w0, 0
	ldr	x1, .LC68
	strb	wzr, [x2]
	ldr	x2, [x19, 23992]
	strb	wzr, [x2, 1]
	ldr	w2, [x20, 3612]
	ldr	w4, [x20, 3616]
	ldr	w3, [x20, 3620]
	ldr	x5, [x24, 640]
	ldr	x7, [x24, 648]
	ldr	x6, [x24, 656]
	bl	dprint_vfmw
	ldrb	w0, [x19, 2]
	cmp	w0, 5
	bne	.L308
	add	x19, x19, 36864
	ldr	w0, [x19, 1684]
	cmp	w0, wzr
	ble	.L308
	sub	w0, w0, #1
	str	w0, [x19, 1684]
	b	.L308
	.p2align 3
.L369:
	ldr	w1, [x28, 104]
	add	x0, x0, 36864
	str	w1, [x24, 104]
	ldr	x2, [x19, 26528]
	ldr	x1, [x19, 26512]
	ldr	w2, [x2, 1136]
	str	w2, [x1, 1136]
	ldr	w1, [x0, 1684]
	add	w1, w1, 1
	str	w1, [x0, 1684]
	b	.L312
	.p2align 3
.L323:
	mov	w0, 1
	mov	x5, 0
	str	w0, [x1, 3628]
.L360:
	mov	w4, 0
	b	.L325
	.p2align 3
.L366:
	ldr	x0, [x19, 26512]
	ldrb	w1, [x1, 2458]
	strb	w1, [x0, 2]
	b	.L299
	.p2align 3
.L365:
	ldr	x1, .LC58
	ldr	w1, [x1]
	cmp	w1, 1
	bne	.L297
	ldr	x0, [x19, 26512]
	str	w1, [x0, 1136]
	b	.L298
	.p2align 3
.L293:
	ldr	x1, .LC53
	mov	w0, 1
	mov	w23, 0
	bl	dprint_vfmw
	ldrb	w0, [x19, 2520]
	and	w0, w0, -9
	strb	w0, [x19, 2520]
	b	.L357
	.p2align 3
.L335:
	mov	w2, 1
	b	.L322
	.p2align 3
.L333:
	mov	w0, 1
	b	.L320
.L374:
	ldrb	w1, [x1, 2583]
	sub	w1, w1, #2
	uxtb	w1, w1
	cmp	w1, 1
	cset	w2, ls
	b	.L322
.L371:
	ldrb	w0, [x1, 2468]
	sub	w0, w0, #2
	uxtb	w0, w0
	cmp	w0, 1
	cset	w0, ls
	b	.L320
.L315:
	ldr	w0, [x0, 16]
	b	.L316
.L370:
	ldr	x0, [x24, 640]
	b	.L359
	.size	VC1_Vfmw_GetStreamInfo, .-VC1_Vfmw_GetStreamInfo
	.align	3
.LC51:
	.xword	.LC30
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
	.xword	USE_FF_APT_EN
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_PrepareDecode
	.type	VC1_Vfmw_PrepareDecode, %function
VC1_Vfmw_PrepareDecode:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	VC1_Vfmw_GetStreamInfo
	mov	w19, w0
	cbz	w0, .L378
	mov	w19, 1
	mov	w0, w19
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L378:
	ldr	x1, .LC72
	bl	dprint_vfmw
	mov	w0, w19
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_Vfmw_PrepareDecode, .-VC1_Vfmw_PrepareDecode
	.align	3
.LC72:
	.xword	.LC71
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecMvrange
	.type	VC1_DecMvrange, %function
VC1_DecMvrange:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x3, .LC73
	mov	w1, 3
	mov	x0, x19
	ldp	x8, x9, [x3, 16]
	ldp	x6, x7, [x3, 32]
	ldp	x4, x5, [x3, 48]
	ldp	x2, x3, [x3, 64]
	stp	x8, x9, [x29, 32]
	stp	x6, x7, [x29, 48]
	stp	x4, x5, [x29, 64]
	stp	x2, x3, [x29, 80]
	bl	BsShow
	add	x1, x29, 96
	add	x1, x1, x0, sxtw 3
	mov	x0, x19
	ldp	w19, w1, [x1, -64]
	bl	BsSkip
	mov	w0, w19
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.size	VC1_DecMvrange, .-VC1_DecMvrange
	.align	3
.LC73:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecTransacfrm
	.type	VC1_DecTransacfrm, %function
VC1_DecTransacfrm:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w2, 2
	mov	w3, 1
	mov	w1, w2
	mov	x0, x19
	stp	xzr, xzr, [x29, 32]
	str	w3, [x29, 36]
	stp	w3, w3, [x29, 44]
	stp	w2, w2, [x29, 52]
	str	w2, [x29, 60]
	stp	xzr, xzr, [x29, 64]
	stp	xzr, xzr, [x29, 80]
	bl	BsShow
	add	x1, x29, 96
	add	x1, x1, x0, sxtw 3
	mov	x0, x19
	ldp	w19, w1, [x1, -64]
	bl	BsSkip
	mov	w0, w19
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 96
	ret
	.size	VC1_DecTransacfrm, .-VC1_DecTransacfrm
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_SetCurPicResolutionInfo
	.type	VC1_SetCurPicResolutionInfo, %function
VC1_SetCurPicResolutionInfo:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	add	x1, x19, 20480
	cmp	w20, 1
	add	x0, x19, 36864
	ldr	w2, [x1, 3544]
	beq	.L383
	cmp	w20, 2
	beq	.L389
	cbz	w20, .L388
	add	w2, w2, 1
	add	w2, w2, w2, lsr 31
	asr	w2, w2, 1
.L389:
	str	w2, [x0, 1664]
	ldr	w1, [x1, 3548]
	add	w1, w1, 1
	add	w1, w1, w1, lsr 31
	asr	w1, w1, 1
	str	w1, [x0, 1668]
	b	.L386
	.p2align 3
.L383:
	add	w2, w2, 1
	add	w2, w2, w2, lsr 31
	asr	w2, w2, 1
.L388:
	str	w2, [x0, 1664]
	ldr	w1, [x1, 3548]
	str	w1, [x0, 1668]
.L386:
	mul	w3, w2, w1
	str	w3, [x0, 1672]
	lsl	w2, w2, 4
	lsl	w1, w1, 4
	str	w2, [x0, 1656]
	str	w1, [x0, 1660]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_SetCurPicResolutionInfo, .-VC1_SetCurPicResolutionInfo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecMvmodeProgrP
	.type	VC1_DecMvmodeProgrP, %function
VC1_DecMvmodeProgrP:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	uxtb	w21, w1
	bl	_mcount
	mov	w1, 4
	mov	x0, x20
	bl	BsShow
	cmp	w0, 1
	ldr	x1, .LC75
	add	x2, x1, 104
	ldp	x4, x5, [x1, 80]
	ldr	w3, [x1, 96]
	ldp	x6, x7, [x2]
	add	x2, x29, 72
	ldr	w1, [x1, 120]
	stp	x4, x5, [x29, 48]
	str	w3, [x29, 64]
	stp	x6, x7, [x2]
	str	w1, [x29, 88]
	ble	.L398
	cmp	w0, 3
	ble	.L396
	cmp	w0, 8
	mov	w1, 2
	csinc	w1, w1, wzr, lt
	cset	w19, lt
.L392:
	mov	x0, x20
	sbfiz	x19, x19, 2, 32
	bl	BsSkip
	cmp	w21, 12
	add	x0, x29, 96
	add	x19, x0, x19
	bhi	.L399
.L393:
	ldr	w0, [x19, -24]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L396:
	mov	w1, 3
	mov	x0, x20
	mov	w19, 2
	bl	BsSkip
	sbfiz	x19, x19, 2, 32
	cmp	w21, 12
	add	x0, x29, 96
	add	x19, x0, x19
	bls	.L393
.L399:
	ldr	w0, [x19, -48]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L398:
	mov	w1, 4
	mov	w19, 3
	cbz	w0, .L392
	mov	w19, w1
	b	.L392
	.size	VC1_DecMvmodeProgrP, .-VC1_DecMvmodeProgrP
	.align	3
.LC75:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecMvmodeProgrB
	.type	VC1_DecMvmodeProgrB, %function
VC1_DecMvmodeProgrB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 1
	mov	x0, x19
	bl	BsSkip
	ldr	x19, [sp, 16]
	add	x1, x29, 48
	mov	w2, 3
	add	x0, x1, x0, sxtw 2
	mov	w1, 1
	stp	w2, w1, [x29, 40]
	ldr	w0, [x0, -8]
	ldp	x29, x30, [sp], 48
	ret
	.size	VC1_DecMvmodeProgrB, .-VC1_DecMvmodeProgrB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecMvmodeInterB
	.type	VC1_DecMvmodeInterB, %function
VC1_DecMvmodeInterB:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	uxtb	w21, w1
	bl	_mcount
	mov	w1, 3
	mov	x0, x20
	bl	BsShow
	str	wzr, [x29, 88]
	add	x1, x29, 72
	mov	w3, 1
	mov	w2, 2
	cmp	w0, w3
	stp	w2, wzr, [x29, 64]
	stp	xzr, xzr, [x1]
	str	w3, [x29, 72]
	mov	w1, 3
	stp	w1, w3, [x29, 56]
	stp	w2, w1, [x29, 80]
	ble	.L408
	cmp	w0, 4
	csinc	w1, w2, wzr, lt
	cset	w19, lt
.L403:
	mov	x0, x20
	bl	BsSkip
	cmp	w21, 12
	sbfiz	x2, x19, 2, 32
	add	x0, x29, 96
	add	x2, x0, x2
	bhi	.L409
	ldr	w0, [x2, -24]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L409:
	ldr	w0, [x2, -40]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L408:
	mov	w19, w1
	cbz	w0, .L403
	mov	w19, w2
	b	.L403
	.size	VC1_DecMvmodeInterB, .-VC1_DecMvmodeInterB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecMvmode2
	.type	VC1_DecMvmode2, %function
VC1_DecMvmode2:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	uxtb	w21, w1
	bl	_mcount
	mov	w1, 3
	mov	x0, x20
	bl	BsShow
	mov	w3, 1
	mov	w1, 3
	mov	w2, 2
	cmp	w0, w3
	stp	w1, w3, [x29, 48]
	stp	w2, wzr, [x29, 56]
	stp	w3, wzr, [x29, 64]
	stp	w2, w1, [x29, 72]
	ble	.L417
	cmp	w0, 4
	csinc	w1, w2, wzr, lt
	cset	w19, lt
.L412:
	mov	x0, x20
	bl	BsSkip
	cmp	w21, 12
	sbfiz	x2, x19, 2, 32
	add	x0, x29, 80
	add	x2, x0, x2
	bhi	.L418
	ldr	w0, [x2, -16]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L418:
	ldr	w0, [x2, -32]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L417:
	mov	w19, w1
	cbz	w0, .L412
	mov	w19, w2
	b	.L412
	.size	VC1_DecMvmode2, .-VC1_DecMvmode2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecBfractionSMP
	.type	VC1_DecBfractionSMP, %function
VC1_DecBfractionSMP:
	stp	x29, x30, [sp, -256]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	mov	x22, x2
	bl	_mcount
	ldr	x0, .LC77
	add	x1, x0, 248
	add	x8, x0, 304
	ldp	x4, x5, [x0, 128]
	ldr	w3, [x0, 184]
	ldr	x6, [x0, 144]
	stp	x4, x5, [x29, 48]
	ldr	x4, [x0, 176]
	ldr	w7, [x0, 152]
	str	x6, [x29, 64]
	str	x4, [x29, 96]
	ldr	x6, [x0, 240]
	ldp	x4, x5, [x0, 192]
	str	w3, [x29, 104]
	ldr	x3, [x0, 296]
	str	w7, [x29, 72]
	stp	x4, x5, [x29, 144]
	str	x6, [x29, 192]
	ldp	x4, x5, [x0, 208]
	ldp	x6, x7, [x0, 160]
	str	x3, [x29, 248]
	ldp	x2, x3, [x0, 224]
	add	x0, x29, 200
	stp	x6, x7, [x29, 80]
	ldp	x6, x7, [x1]
	stp	x6, x7, [x0]
	ldp	x6, x7, [x8]
	stp	x6, x7, [x29, 112]
	add	x6, x29, 216
	mov	x0, x21
	stp	x4, x5, [x29, 160]
	ldp	x4, x5, [x1, 16]
	stp	x4, x5, [x6]
	add	x4, x29, 232
	stp	x2, x3, [x29, 176]
	ldp	x2, x3, [x1, 32]
	stp	x2, x3, [x4]
	ldp	x2, x3, [x8, 16]
	mov	w1, 3
	stp	x2, x3, [x29, 128]
	bl	BsShow
	cmp	w0, 6
	mov	w19, w0
	bgt	.L420
	mov	w1, 3
	mov	x0, x21
	bl	BsSkip
	add	x0, x29, 256
	add	x0, x0, x19, sxtw 2
	ldr	w2, [x0, -208]
	ldr	w1, [x0, -176]
.L421:
	sub	w1, w1, #1
	add	x3, x29, 256
	mov	w0, w19
	add	x1, x3, x1, sxtw 2
	ldr	w1, [x1, -144]
	mul	w1, w1, w2
	str	w1, [x20]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 256
	ret
	.p2align 3
.L420:
	mov	w1, 7
	mov	x0, x21
	bl	BsGet
	and	w19, w0, 15
	cmp	w19, 15
	beq	.L424
	sub	w0, w0, #112
	add	x1, x29, 256
	add	w19, w19, 7
	add	x0, x1, x0, sxtw 2
	ldr	w2, [x0, -112]
	ldr	w1, [x0, -56]
	b	.L421
	.p2align 3
.L424:
	mov	w1, 4
	strb	w1, [x22]
	mov	w0, 22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 256
	ret
	.size	VC1_DecBfractionSMP, .-VC1_DecBfractionSMP
	.align	3
.LC77:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecBfraction
	.type	VC1_DecBfraction, %function
VC1_DecBfraction:
	stp	x29, x30, [sp, -256]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	ldr	x19, .LC83
	add	x0, x19, 248
	ldr	w6, [x19, 152]
	ldr	x8, [x19, 176]
	str	w6, [x29, 72]
	add	x6, x29, 200
	ldr	w5, [x19, 184]
	ldr	x4, [x19, 240]
	str	x8, [x29, 96]
	add	x8, x29, 216
	ldp	x2, x3, [x19, 128]
	str	w5, [x29, 104]
	str	x4, [x29, 192]
	ldp	x4, x5, [x0]
	stp	x4, x5, [x6]
	stp	x2, x3, [x29, 48]
	ldp	x4, x5, [x0, 16]
	ldp	x2, x3, [x19, 160]
	add	x6, x19, 304
	ldr	x7, [x19, 144]
	stp	x4, x5, [x8]
	stp	x2, x3, [x29, 80]
	add	x4, x29, 232
	str	x7, [x29, 64]
	ldp	x2, x3, [x19, 192]
	stp	x2, x3, [x29, 144]
	ldp	x2, x3, [x19, 208]
	stp	x2, x3, [x29, 160]
	ldp	x2, x3, [x0, 32]
	stp	x2, x3, [x4]
	ldp	x2, x3, [x6]
	ldp	x0, x1, [x19, 224]
	ldr	x7, [x19, 296]
	stp	x2, x3, [x29, 112]
	ldp	x2, x3, [x6, 16]
	stp	x0, x1, [x29, 176]
	mov	w1, 3
	mov	x0, x22
	str	x7, [x29, 248]
	stp	x2, x3, [x29, 128]
	bl	BsShow
	cmp	w0, 6
	mov	w20, w0
	bgt	.L426
	mov	w1, 3
	mov	x0, x22
	bl	BsSkip
	add	x0, x29, 256
	add	x0, x0, x20, sxtw 2
	ldr	w4, [x0, -176]
	ldr	w1, [x0, -208]
	sub	w0, w4, #1
	cmp	w0, 7
	bhi	.L430
.L432:
	add	x2, x29, 256
	add	x0, x2, x0, sxtw 2
	ldr	w0, [x0, -144]
	mul	w0, w1, w0
.L431:
	str	w0, [x21]
.L428:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 256
	ret
	.p2align 3
.L426:
	mov	w1, 7
	mov	x0, x22
	bl	BsGet
	and	w20, w0, 15
	sub	w1, w20, #14
	add	w20, w20, 7
	cmp	w1, 1
	bls	.L428
	sub	w1, w0, #112
	cmp	w1, 13
	bhi	.L429
	add	x0, x29, 256
	add	x0, x0, x1, sxtw 2
	ldr	w4, [x0, -56]
	ldr	w1, [x0, -112]
	sub	w0, w4, #1
	cmp	w0, 7
	bls	.L432
.L430:
	ldr	x1, .LC87
	mov	w3, 3591
	add	x2, x19, 336
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L431
	.p2align 3
.L429:
	ldr	x1, .LC85
	mov	w4, w0
	mov	w3, 3581
	add	x2, x19, 336
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, .LC87
	mov	w4, 0
	mov	w3, 3591
	add	x2, x19, 336
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L431
	.size	VC1_DecBfraction, .-VC1_DecBfraction
	.align	3
.LC83:
	.xword	.LANCHOR0
	.align	3
.LC85:
	.xword	.LC84
	.align	3
.LC87:
	.xword	.LC86
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_VopDQuant
	.type	VC1_VopDQuant, %function
VC1_VopDQuant:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	uxtb	w21, w1
	uxtb	w20, w2
	mov	x19, x3
	bl	_mcount
	cmp	w21, 2
	beq	.L451
	mov	w1, 1
	mov	x0, x22
	bl	BsGet
	uxtb	w21, w0
	cmp	w21, 1
	strb	w21, [x19]
	beq	.L452
.L437:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L451:
	mov	w0, 1
	mov	w1, 3
	strb	w0, [x19, 3]
	strb	w0, [x19]
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 7
	strb	w0, [x19, 4]
	beq	.L453
	add	w20, w20, 1
	add	w0, w0, w20
	strb	w0, [x19, 8]
.L436:
	mov	w0, 1
	str	w0, [x19, 12]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L452:
	mov	w1, 2
	mov	x0, x22
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x19, 1]
	cbnz	w1, .L439
	str	w21, [x19, 12]
.L440:
	ldr	w0, [x19]
	and	w0, w0, 16776960
	cmp	w0, 768
	beq	.L437
	mov	w0, 1
	mov	w1, 3
	strb	w0, [x19, 3]
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 7
	strb	w0, [x19, 4]
	beq	.L454
	add	w20, w20, 1
	add	w0, w0, w20
	strb	w0, [x19, 8]
	b	.L437
	.p2align 3
.L439:
	cmp	w1, 2
	beq	.L455
	cmp	w1, 1
	beq	.L456
	cmp	w1, 3
	beq	.L457
	str	wzr, [x19, 12]
	b	.L440
.L457:
	mov	w1, w21
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	mov	w1, 10
	mov	w2, 11
	cmp	w0, wzr
	strb	w0, [x19, 2]
	csel	w0, w1, w2, ne
	str	w0, [x19, 12]
	b	.L440
	.p2align 3
.L453:
	mov	w1, 5
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	strb	w0, [x19, 8]
	b	.L436
.L455:
	mov	x0, x22
	bl	BsGet
	and	w1, w0, 255
	strb	w0, [x19, 6]
	add	w0, w1, 6
	str	w0, [x19, 12]
	b	.L440
.L456:
	mov	w1, 2
	mov	x0, x22
	bl	BsGet
	and	w1, w0, 255
	strb	w0, [x19, 7]
	add	w0, w1, 2
	str	w0, [x19, 12]
	b	.L440
.L454:
	mov	w1, 5
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	strb	w0, [x19, 8]
	b	.L437
	.size	VC1_VopDQuant, .-VC1_VopDQuant
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecNormDiff6Tile
	.type	VC1_DecNormDiff6Tile, %function
VC1_DecNormDiff6Tile:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x23, x1
	mov	x20, x0
	mov	x0, x30
	mov	w22, 0
	bl	_mcount
	ldr	w24, [x23]
	b	.L459
	.p2align 3
.L469:
	mov	w22, w19
.L459:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	mov	w21, w0
	mov	w1, 3
	mov	x0, x20
	cbnz	w21, .L468
	bl	BsGet
	cmp	w0, 1
	mov	w19, w0
	mov	w1, w0
	mov	x0, x20
	bgt	.L478
	bne	.L462
	bl	BsGet
	cbz	w0, .L463
	cbz	w22, .L469
.L475:
	str	w24, [x23]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L468:
	mov	w0, 0
.L460:
	eor	w1, w0, 63
	cmp	w22, wzr
	csel	w0, w1, w0, ne
	mov	w21, 1
	str	w0, [x23]
.L480:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L478:
	sub	w19, w19, #2
	mov	w0, 1
	lsl	w0, w0, w19
	b	.L460
	.p2align 3
.L462:
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	cmp	w0, 255
	beq	.L475
	ldr	x1, .LC93
	add	x1, x1, 488
	ldr	w0, [x1, w0, sxtw 2]
	b	.L460
	.p2align 3
.L463:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	ldr	x1, .LC93
	add	x1, x1, 360
	ldr	w1, [x1, w0, sxtw 2]
	cmp	w1, 2
	beq	.L479
	cmp	w1, 3
	beq	.L460
	str	w0, [x23]
	b	.L480
.L479:
	orr	w0, w0, 32
	b	.L460
	.size	VC1_DecNormDiff6Tile, .-VC1_DecNormDiff6Tile
	.align	3
.LC93:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvSeqHdr
	.type	VC1_Vfmw_ParseAdvSeqHdr, %function
VC1_Vfmw_ParseAdvSeqHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x22, x2
	bl	_mcount
	add	x19, x22, 16
	ldr	x3, .LC96
	mov	x2, 172
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	mov	x0, x20
	bl	BsPos
	str	w0, [x19, 168]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	mov	w0, 2
	mov	w1, 3
	strb	w0, [x22, 16]
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 1]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 2]
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 5]
	mov	w1, 12
	mov	x0, x20
	bl	BsGet
	uxth	w0, w0
	add	w2, w0, 1
	mov	w1, 12
	ubfiz	w2, w2, 1, 15
	mov	x0, x20
	strh	w2, [x19, 156]
	bl	BsGet
	uxth	w0, w0
	add	w2, w0, 1
	mov	w1, 1
	ubfiz	w2, w2, 1, 15
	mov	x0, x20
	strh	w2, [x19, 158]
	bl	BsGet
	strb	w0, [x19, 6]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w21, w0
	cmp	w21, 1
	strb	w21, [x19, 10]
	beq	.L495
	ldrh	w1, [x19, 156]
	ldrh	w0, [x19, 158]
	strh	w1, [x19, 160]
	strh	w0, [x19, 162]
.L488:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 23]
	beq	.L489
.L491:
	mov	x0, x20
	bl	BsPos
	ldr	w1, [x19, 168]
	mov	w2, 1
	sub	w0, w0, w1
	str	w0, [x19, 168]
.L490:
	mov	w0, w2
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L489:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 31
	strb	w0, [x19, 24]
	mov	w2, 0
	bhi	.L490
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 25]
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 26]
	ldrb	w1, [x19, 24]
	cbz	w1, .L491
	mov	w21, 0
	.p2align 2
.L492:
	add	x23, x22, x21, sxtw 1
	mov	w1, 16
	mov	x0, x20
	add	w21, w21, 1
	bl	BsGet
	strh	w0, [x23, 44]
	mov	w1, 16
	mov	x0, x20
	bl	BsGet
	strh	w0, [x23, 108]
	ldrb	w0, [x19, 24]
	cmp	w0, w21
	bgt	.L492
	b	.L491
	.p2align 3
.L495:
	mov	w1, 14
	mov	x0, x20
	bl	BsGet
	add	w0, w0, 1
	mov	w1, 14
	strh	w0, [x19, 160]
	mov	x0, x20
	bl	BsGet
	add	w0, w0, 1
	mov	w1, w21
	strh	w0, [x19, 162]
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 11]
	beq	.L496
.L484:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w1, w0
	cmp	w1, 1
	strb	w1, [x19, 15]
	beq	.L497
.L486:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 19]
	bne	.L488
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 20]
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 22]
	b	.L488
	.p2align 3
.L497:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 16]
	cbnz	w0, .L487
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 17]
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	b	.L486
	.p2align 3
.L496:
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 15
	strb	w0, [x19, 12]
	bne	.L484
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 13]
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 14]
	b	.L484
.L487:
	mov	w1, 16
	mov	x0, x20
	bl	BsGet
	strh	w0, [x19, 164]
	b	.L486
	.size	VC1_Vfmw_ParseAdvSeqHdr, .-VC1_Vfmw_ParseAdvSeqHdr
	.align	3
.LC96:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvEntptHdr
	.type	VC1_Vfmw_ParseAdvEntptHdr, %function
VC1_Vfmw_ParseAdvEntptHdr:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	bl	_mcount
	add	x19, x21, 188
	ldr	x3, .LC97
	mov	x2, 60
	mov	w1, 0
	mov	x0, x19
	add	x22, x21, 16
	ldr	x3, [x3, 96]
	blr	x3
	mov	x0, x20
	bl	BsPos
	str	w0, [x19, 56]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x21, 188]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 1]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 2]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 5]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 6]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 10]
	ldrb	w0, [x22, 23]
	cmp	w0, 1
	beq	.L499
.L502:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 11]
	beq	.L519
.L517:
	ldrh	w0, [x19, 52]
.L504:
	ldrh	w1, [x19, 50]
	ldrh	w2, [x22, 156]
	cmp	w1, 15
	ccmp	w1, w2, 2, hi
	bls	.L505
	strh	w2, [x19, 50]
.L505:
	ldrh	w1, [x22, 158]
	cmp	w0, 15
	ccmp	w1, w0, 0, hi
	bcs	.L506
	strh	w1, [x19, 52]
.L506:
	ldrb	w1, [x19, 6]
	cmp	w1, 1
	beq	.L520
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 13]
	beq	.L521
.L508:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 15]
	beq	.L522
.L509:
	mov	x0, x20
	bl	BsPos
	ldr	w1, [x19, 56]
	sub	w1, w0, w1
	str	w1, [x19, 56]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L499:
	ldrb	w0, [x22, 24]
	cbz	w0, .L502
	mov	w21, 0
	.p2align 2
.L503:
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	add	x1, x19, x21, sxtw
	add	w21, w21, 1
	strb	w0, [x1, 17]
	ldrb	w0, [x22, 24]
	cmp	w0, w21
	bgt	.L503
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 11]
	bne	.L517
.L519:
	mov	w1, 12
	mov	x0, x20
	bl	BsGet
	add	w0, w0, 1
	ubfiz	w0, w0, 1, 15
	mov	w1, 12
	strh	w0, [x19, 50]
	mov	x0, x20
	bl	BsGet
	add	w0, w0, 1
	ubfiz	w0, w0, 1, 15
	strh	w0, [x19, 52]
	b	.L504
	.p2align 3
.L522:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	b	.L509
	.p2align 3
.L520:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 13]
	bne	.L508
	.p2align 2
.L521:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 14]
	b	.L508
	.size	VC1_Vfmw_ParseAdvEntptHdr, .-VC1_Vfmw_ParseAdvEntptHdr
	.align	3
.LC97:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecFcm
	.type	VC1_DecFcm, %function
VC1_DecFcm:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 2
	mov	x0, x20
	bl	BsShow
	cmp	w0, 1
	mov	w19, w0
	ble	.L526
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L526:
	mov	w1, 1
	mov	x0, x20
	mov	w19, 0
	bl	BsSkip
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_DecFcm, .-VC1_DecFcm
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecAdvPtype
	.type	VC1_DecAdvPtype, %function
VC1_DecAdvPtype:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 4
	mov	x0, x19
	bl	BsShow
	cmp	w0, 7
	ble	.L529
	cmp	w0, 11
	bgt	.L533
	mov	w1, 2
	mov	w20, w1
.L528:
	mov	x0, x19
	bl	BsSkip
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L533:
	cmp	w0, 13
	ble	.L531
	cmp	w0, 14
	mov	w1, 4
	beq	.L532
	mov	w20, 5
	b	.L528
	.p2align 3
.L529:
	mov	w1, 1
	mov	x0, x19
	mov	w20, w1
	bl	BsSkip
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L531:
	mov	w1, 3
	mov	w20, 0
	b	.L528
	.p2align 3
.L532:
	mov	w20, w1
	b	.L528
	.size	VC1_DecAdvPtype, .-VC1_DecAdvPtype
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecRefdist
	.type	VC1_DecRefdist, %function
VC1_DecRefdist:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	mov	w1, 5
	mov	x0, x20
	bl	BsShow
	cmp	w0, 23
	mov	w2, w0
	bgt	.L535
	mov	w1, 2
	mov	x0, x20
	asr	w19, w2, 3
	bl	BsSkip
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L535:
	cmp	w0, 27
	mov	w19, 3
	bgt	.L541
	mov	w1, w19
	mov	x0, x20
	bl	BsSkip
	mov	w0, w19
.L542:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L541:
	mov	w0, 5
	cmp	w2, 29
	mov	w19, 4
	csel	w19, w0, w19, gt
	mov	x0, x20
	mov	w1, w19
	bl	BsSkip
	mov	w0, w19
	b	.L542
	.size	VC1_DecRefdist, .-VC1_DecRefdist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_DecIntCompField
	.type	VC1_DecIntCompField, %function
VC1_DecIntCompField:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC98
	mov	w1, 2
	mov	x0, x19
	add	x2, x2, 552
	ldp	x4, x5, [x2]
	ldp	x2, x3, [x2, 16]
	stp	x4, x5, [x29, 32]
	stp	x2, x3, [x29, 48]
	bl	BsShow
	add	x1, x29, 64
	add	x1, x1, x0, sxtw 3
	mov	x0, x19
	ldp	w19, w1, [x1, -32]
	bl	BsSkip
	mov	w0, w19
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 64
	ret
	.size	VC1_DecIntCompField, .-VC1_DecIntCompField
	.align	3
.LC98:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_next_start_code
	.type	VC1_Vfmw_next_start_code, %function
VC1_Vfmw_next_start_code:
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
	sub	w21, w21, #3
	cmp	w21, wzr
	bgt	.L551
	b	.L548
	.p2align 3
.L554:
	bl	BsSkip
	cmp	w19, w21
	beq	.L548
.L551:
	mov	w1, 24
	mov	x0, x20
	add	w19, w19, 1
	bl	BsShow
	mov	w2, w0
	cmp	w2, 1
	mov	w1, 8
	mov	x0, x20
	bne	.L554
	mov	w0, w2
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L548:
	mov	w2, 0
	ldr	x21, [sp, 32]
	mov	w0, w2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	VC1_Vfmw_next_start_code, .-VC1_Vfmw_next_start_code
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_UserData
	.type	VC1_Vfmw_UserData, %function
VC1_Vfmw_UserData:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 1
	ldp	x29, x30, [sp], 16
	ret
	.size	VC1_Vfmw_UserData, .-VC1_Vfmw_UserData
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_RecordSlcStream
	.type	VC1_RecordSlcStream, %function
VC1_RecordSlcStream:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x20, x22, 20480
	mov	x21, x1
	mov	x23, x2
	bl	_mcount
	add	x1, x21, 536
	add	x5, x20, 3632
	ldr	w0, [x20, 3628]
	ldr	x4, [x22, 3136]
	uxtw	x21, w0
	lsl	x3, x21, 3
	sub	x3, x3, x21
	lsl	x3, x3, 3
	add	x19, x5, x3
	strb	w0, [x19, 16]
	ldr	w0, [x4, 32]
	ubfx	x0, x0, 3, 29
	add	x0, x23, x0
	str	x0, [x5, x3]
	ldr	w2, [x4, 32]
	and	w2, w2, 7
	str	w2, [x19, 24]
	ldr	w3, [x4, 24]
	ldr	w2, [x4, 32]
	lsl	w3, w3, 3
	sub	w3, w3, w2
	lsl	x2, x21, 2
	sub	x2, x2, x21
	str	w3, [x19, 28]
	add	x21, x1, x2, lsl 2
	bl	MEM_Vir2Phy
	add	x2, x22, 36864
	str	w0, [x19, 20]
	add	x0, x2, 1592
	ldr	w1, [x2, 1628]
	str	w1, [x19, 48]
	ldr	x1, [x2, 1600]
	cbz	x1, .L557
	ldr	w2, [x2, 1616]
	cbnz	w2, .L576
.L557:
	mov	x1, 0
	str	xzr, [x19, 8]
	stp	wzr, wzr, [x19, 36]
	str	wzr, [x19, 44]
	str	wzr, [x19, 52]
.L558:
	ldr	w4, [x20, 3544]
	ldr	w0, [x20, 3548]
	ldrb	w2, [x22, 8]
	mul	w4, w0, w4
	sub	w4, w4, #1
	cbz	w2, .L559
	add	w0, w0, 1
	ldr	w3, [x21, 4]
	add	w0, w0, w0, lsr 31
	sub	w3, w3, w0, asr 1
	str	w3, [x19, 32]
	cmp	w4, w3
	ccmp	w3, wzr, 4, cc
	bgt	.L577
.L560:
	ldr	w0, [x19, 28]
	cmp	w0, wzr
	ble	.L562
	cbz	x1, .L563
	ldr	w0, [x19, 44]
	cmp	w0, wzr
	ble	.L562
.L563:
	ldr	w1, [x20, 3628]
	mov	w0, 1
	add	w1, w1, w0
	str	w1, [x20, 3628]
.L561:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L559:
	ldr	w3, [x21, 4]
	str	w3, [x19, 32]
	cmp	w4, w3
	bcs	.L560
	ldr	w2, [x21, 4]
	mov	w0, 1
	ldr	x1, .LC103
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L576:
	str	x1, [x19, 8]
	str	wzr, [x19, 40]
	ldr	w2, [x0, 32]
	lsl	w2, w2, 3
	str	w2, [x19, 44]
	ldr	w2, [x0, 24]
	str	w2, [x19, 36]
	ldr	w0, [x0, 40]
	str	w0, [x19, 52]
	b	.L558
	.p2align 3
.L577:
	ldr	x1, .LC101
	mov	w0, 1
	ldr	w4, [x20, 3548]
	ldr	w2, [x21, 4]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L561
	.p2align 3
.L562:
	ldr	x1, .LC105
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	VC1_RecordSlcStream, .-VC1_RecordSlcStream
	.align	3
.LC101:
	.xword	.LC100
	.align	3
.LC103:
	.xword	.LC102
	.align	3
.LC105:
	.xword	.LC104
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_UpdateAdvSPS
	.type	VC1_Vfmw_UpdateAdvSPS, %function
VC1_Vfmw_UpdateAdvSPS:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x1
	add	x21, x0, 8192
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldrh	w1, [x19, 238]
	ldrh	w0, [x21, 2446]
	cmp	w0, w1
	beq	.L579
	add	x0, x20, 20480
	add	w1, w1, 15
	asr	w1, w1, 4
	str	w1, [x0, 3544]
	ldrh	w1, [x19, 238]
	str	w1, [x0, 3560]
	ldrh	w1, [x19, 240]
	ldrh	w2, [x21, 2448]
	cmp	w2, w1
	beq	.L580
.L581:
	add	w1, w1, 15
	asr	w1, w1, 4
	str	w1, [x0, 3548]
	ldrh	w1, [x19, 240]
	str	w1, [x0, 3564]
.L580:
	add	x1, x19, 16
	add	x0, x21, 2224
	add	x19, x19, 188
	mov	x2, 172
	add	x21, x21, 2396
	bl	memcpy
	ldp	x0, x1, [x19]
	stp	x0, x1, [x21]
	mov	w0, 1
	ldp	x2, x3, [x19, 16]
	stp	x2, x3, [x21, 16]
	ldp	x2, x3, [x19, 32]
	stp	x2, x3, [x21, 32]
	ldr	x1, [x19, 48]
	str	x1, [x21, 48]
	ldr	w1, [x19, 56]
	str	w1, [x21, 56]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L579:
	ldrh	w1, [x19, 240]
	ldrh	w0, [x21, 2448]
	cmp	w0, w1
	bne	.L586
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L586:
	add	x0, x20, 20480
	b	.L581
	.size	VC1_Vfmw_UpdateAdvSPS, .-VC1_Vfmw_UpdateAdvSPS
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_UpdataLastPicRNDCTRL
	.type	VC1_Vfmw_UpdataLastPicRNDCTRL, %function
VC1_Vfmw_UpdataLastPicRNDCTRL:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrsb	w0, [x19, 3104]
	cbnz	w0, .L588
	mov	x0, x19
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
.L588:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_Vfmw_UpdataLastPicRNDCTRL, .-VC1_Vfmw_UpdataLastPicRNDCTRL
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_DecPQUANT
	.type	VC1_Vfmw_DecPQUANT, %function
VC1_Vfmw_DecPQUANT:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x1, .LC106
	mov	x2, 128
	add	x0, x29, 32
	add	x1, x1, 584
	bl	memcpy
	mov	w0, w19
	cbnz	w20, .L590
	add	x0, x29, 160
	add	x19, x0, x19, sxtw 2
	ldr	w0, [x19, -128]
.L590:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 160
	ret
	.size	VC1_Vfmw_DecPQUANT, .-VC1_Vfmw_DecPQUANT
	.align	3
.LC106:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseSMPIPicHdr
	.type	VC1_Vfmw_ParseSMPIPicHdr, %function
VC1_Vfmw_ParseSMPIPicHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	x23, x1
	bl	_mcount
	add	x19, x21, 248
	mov	w1, 7
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 5]
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 6]
	mov	w1, 0
	cbz	w22, .L593
	ldrb	w24, [x21, 14]
	mov	w1, w22
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 104]
	cmp	w22, 8
	bls	.L604
	strb	wzr, [x19, 7]
.L595:
	cmp	w24, 1
	beq	.L605
	strb	wzr, [x19, 8]
.L597:
	ldrb	w0, [x21, 7]
	cmp	w0, 1
	beq	.L606
	strb	wzr, [x19, 9]
.L599:
	ldrb	w0, [x19, 3]
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cbz	w0, .L602
	ldrb	w0, [x21, 4]
	cmp	w0, 1
	beq	.L607
.L602:
	ldrb	w1, [x19, 10]
.L601:
	mov	x0, x23
	bl	VC1_SetCurPicResolutionInfo
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 19]
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 20]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	mov	w1, 1
.L593:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L604:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	ldrb	w24, [x21, 14]
	b	.L595
	.p2align 3
.L606:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 9]
	b	.L599
	.p2align 3
.L605:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L597
	.p2align 3
.L607:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x19, 10]
	b	.L601
	.size	VC1_Vfmw_ParseSMPIPicHdr, .-VC1_Vfmw_ParseSMPIPicHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseSMPPPicHdr
	.type	VC1_Vfmw_ParseSMPPPicHdr, %function
VC1_Vfmw_ParseSMPPPicHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	x23, x1
	add	x19, x21, 248
	bl	_mcount
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 6]
	cbz	w22, .L620
	ldrb	w24, [x21, 14]
	uxtb	w1, w0
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 104]
	cmp	w22, 8
	bls	.L631
	strb	wzr, [x19, 7]
.L612:
	cmp	w24, 1
	beq	.L632
	strb	wzr, [x19, 8]
.L614:
	ldrb	w0, [x21, 7]
	cmp	w0, 1
	beq	.L633
	strb	wzr, [x19, 9]
.L616:
	ldrb	w0, [x21, 4]
	cmp	w0, 1
	beq	.L617
	ldrb	w1, [x19, 10]
.L618:
	mov	x0, x23
	bl	VC1_SetCurPicResolutionInfo
	mov	x0, x20
	bl	BsPos
	mov	w2, w0
	ldr	x1, .LC108
	mov	w0, 18
	bl	dprint_vfmw
	ldrb	w1, [x19, 104]
	mov	x0, x20
	bl	VC1_DecMvmodeProgrP
	uxtb	w0, w0
	cmp	w0, 4
	strb	w0, [x19, 11]
	beq	.L634
.L619:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x23
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L620
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 15]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	ldrb	w1, [x21, 8]
	cbnz	w1, .L635
.L621:
	mov	x0, x20
	bl	BsPos
	mov	w2, w0
	ldr	x1, .LC110
	mov	w0, 18
	bl	dprint_vfmw
	ldrb	w1, [x21, 9]
	cmp	w1, 1
	beq	.L636
.L623:
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 19]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	mov	w1, 1
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L620:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L631:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	ldrb	w24, [x21, 14]
	b	.L612
	.p2align 3
.L635:
	ldrb	w2, [x19, 104]
	add	x3, x21, 272
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L621
	.p2align 3
.L634:
	ldrb	w1, [x19, 104]
	mov	x0, x20
	bl	VC1_DecMvmode2
	strb	w0, [x19, 12]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 13]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 14]
	b	.L619
	.p2align 3
.L617:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x19, 10]
	b	.L618
	.p2align 3
.L633:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 9]
	b	.L616
	.p2align 3
.L632:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L614
	.p2align 3
.L636:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 17]
	bne	.L623
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	b	.L623
	.size	VC1_Vfmw_ParseSMPPPicHdr, .-VC1_Vfmw_ParseSMPPPicHdr
	.align	3
.LC108:
	.xword	.LC107
	.align	3
.LC110:
	.xword	.LC109
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseMPBPicHdr
	.type	VC1_Vfmw_ParseMPBPicHdr, %function
VC1_Vfmw_ParseMPBPicHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	x24, x1
	add	x19, x21, 248
	bl	_mcount
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 6]
	cbz	w22, .L647
	ldrb	w23, [x21, 14]
	uxtb	w1, w0
	mov	w0, w23
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 104]
	cmp	w22, 8
	bls	.L659
	strb	wzr, [x19, 7]
.L641:
	cmp	w23, 1
	beq	.L660
	strb	wzr, [x19, 8]
.L643:
	ldrb	w0, [x21, 7]
	cmp	w0, 1
	beq	.L661
	strb	wzr, [x19, 9]
.L645:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, wzr
	mov	w1, 3
	mov	w3, 0
	csinc	w0, w1, wzr, eq
	mov	x2, x21
	strb	w0, [x19, 11]
	mov	x1, x24
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L647
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 15]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	ldrb	w1, [x21, 8]
	cbnz	w1, .L662
.L648:
	ldrb	w1, [x21, 9]
	cmp	w1, 1
	beq	.L663
.L650:
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 19]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	mov	w1, 1
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L647:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L659:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	ldrb	w23, [x21, 14]
	b	.L641
	.p2align 3
.L662:
	ldrb	w2, [x19, 104]
	add	x3, x21, 272
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L648
	.p2align 3
.L661:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 9]
	b	.L645
	.p2align 3
.L660:
	mov	w1, w23
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L643
	.p2align 3
.L663:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 17]
	bne	.L650
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	b	.L650
	.size	VC1_Vfmw_ParseMPBPicHdr, .-VC1_Vfmw_ParseMPBPicHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseSMPPicHdr
	.type	VC1_Vfmw_ParseSMPPicHdr, %function
VC1_Vfmw_ParseSMPPicHdr:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	add	x19, x2, 248
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	mov	x22, x2
	bl	_mcount
	ldrb	w23, [x19, 10]
	mov	x0, x21
	mov	w1, w23
	bl	VC1_SetCurPicResolutionInfo
	ldr	w0, [x20, 24]
	cmp	w0, 1
	ble	.L702
	ldr	x25, .LC111
	mov	x2, 112
	mov	w1, 0
	mov	x0, x19
	add	x24, x21, 36864
	ldr	x3, [x25, 96]
	blr	x3
	strb	w23, [x19, 10]
	ldr	x3, [x25, 96]
	add	x0, x21, 20480
	mov	x2, 14336
	mov	w1, 0
	add	x0, x0, 3632
	blr	x3
	ldr	x1, .LC113
	mov	w0, 18
	ldr	w2, [x24, 1584]
	bl	dprint_vfmw
	mov	x0, x20
	bl	BsPos
	str	w0, [x19, 88]
	ldrb	w0, [x22, 15]
	cbnz	w0, .L667
	ldrb	w2, [x22, 248]
.L668:
	ldr	x1, .LC115
	mov	w0, 18
	bl	dprint_vfmw
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 1]
	ldrb	w0, [x22, 12]
	cbnz	w0, .L703
.L669:
	ldrb	w0, [x22, 13]
	mov	w1, 1
	cbnz	w0, .L670
	mov	x0, x20
	bl	BsGet
	cmp	w0, wzr
	cset	w0, ne
	strb	w0, [x19, 3]
.L671:
	strb	w0, [x21, 2]
	strb	wzr, [x21, 4]
	ldrb	w0, [x19, 3]
	cmp	w0, 4
	bhi	.L700
	ldr	x1, .LC116
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx676
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx676:
	.section	.rodata
	.align	0
	.align	2
.L676:
	.byte	(.L675 - .Lrtx676) / 4
	.byte	(.L677 - .Lrtx676) / 4
	.byte	(.L678 - .Lrtx676) / 4
	.byte	(.L700 - .Lrtx676) / 4
	.byte	(.L679 - .Lrtx676) / 4
	.text
	.p2align 3
.L677:
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseSMPPPicHdr
	mov	w21, w0
.L680:
	mov	x0, x20
	bl	BsPos
	ldr	w1, [x19, 88]
	sub	w0, w0, w1
	str	w0, [x19, 88]
	mov	x0, x20
	ldr	w19, [x24, 1620]
	bl	BsPos
	lsl	w19, w19, 3
	sub	w19, w19, w0
	mov	w0, w21
	cmp	w19, wzr
	bgt	.L693
	ldr	x1, .LC122
	mov	w0, 1
	bl	dprint_vfmw
	.p2align 2
.L700:
	mov	w0, 0
.L693:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L702:
	mov	w0, 5
	strb	w0, [x19, 3]
	strb	w0, [x21, 2]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L670:
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	beq	.L704
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	beq	.L705
	mov	w0, 2
	add	x2, x22, 251
	strb	w0, [x19, 3]
	add	x1, x22, 356
	mov	x0, x20
	bl	VC1_DecBfractionSMP
	uxtb	w0, w0
	cmp	w0, 21
	strb	w0, [x19, 4]
	beq	.L700
	ldrb	w0, [x19, 3]
	b	.L671
	.p2align 3
.L667:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w2, w0
	strb	w2, [x22, 248]
	b	.L668
	.p2align 3
.L704:
	strb	w0, [x19, 3]
	b	.L671
	.p2align 3
.L679:
	ldrb	w23, [x21, 3120]
	cmp	w23, 1
	bne	.L700
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseSMPIPicHdr
	cmp	w0, 1
	mov	w21, w0
	beq	.L680
	ldr	x1, .LC120
	mov	w0, w23
	bl	dprint_vfmw
	b	.L680
	.p2align 3
.L678:
	ldrb	w23, [x21, 3120]
	cmp	w23, 1
	bne	.L700
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseMPBPicHdr
	cmp	w0, 1
	mov	w21, w0
	beq	.L680
	ldr	x1, .LC118
	mov	w0, w23
	bl	dprint_vfmw
	b	.L680
	.p2align 3
.L675:
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseSMPIPicHdr
	mov	w21, w0
	b	.L680
	.p2align 3
.L703:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 2]
	b	.L669
	.p2align 3
.L705:
	strb	wzr, [x19, 3]
	mov	w0, 0
	b	.L671
	.size	VC1_Vfmw_ParseSMPPicHdr, .-VC1_Vfmw_ParseSMPPicHdr
	.align	3
.LC111:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC113:
	.xword	.LC112
	.align	3
.LC115:
	.xword	.LC114
	.align	3
.LC116:
	.xword	.L676
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
	.global	VC1_Vfmw_ParseAdvIPicHdr
	.type	VC1_Vfmw_ParseAdvIPicHdr, %function
VC1_Vfmw_ParseAdvIPicHdr:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	x25, x1
	bl	_mcount
	add	x23, x21, 16
	mov	w1, 1
	mov	x0, x20
	add	x19, x21, 360
	bl	BsGet
	strb	w0, [x21, 360]
	ldrb	w1, [x23, 7]
	cmp	w1, 1
	beq	.L722
.L707:
	ldr	w0, [x19, 44]
	cbnz	w0, .L708
	ldrb	w1, [x23, 9]
	cmp	w1, 1
	beq	.L723
.L708:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 10]
	cbz	w22, .L716
	add	x26, x21, 188
	ldrb	w24, [x26, 10]
	uxtb	w1, w0
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L724
	strb	wzr, [x19, 3]
.L712:
	cmp	w24, 1
	beq	.L725
	strb	wzr, [x19, 11]
.L714:
	ldrb	w0, [x23, 5]
	cmp	w0, 1
	beq	.L726
.L715:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x25
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L716
	mov	x0, x20
	mov	w22, 1
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 22]
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 23]
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 24]
	ldrb	w1, [x26, 7]
	cbz	w1, .L710
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L716:
	mov	w22, 0
.L710:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L724:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w24, [x26, 10]
	b	.L712
	.p2align 3
.L723:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	b	.L708
	.p2align 3
.L722:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L707
	.p2align 3
.L726:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L715
	.p2align 3
.L725:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L714
	.size	VC1_Vfmw_ParseAdvIPicHdr, .-VC1_Vfmw_ParseAdvIPicHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvPPicHdr
	.type	VC1_Vfmw_ParseAdvPPicHdr, %function
VC1_Vfmw_ParseAdvPPicHdr:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	x26, x1
	bl	_mcount
	add	x22, x21, 16
	mov	w1, 1
	mov	x0, x20
	add	x19, x21, 360
	bl	BsGet
	strb	w0, [x21, 360]
	ldrb	w0, [x22, 7]
	cmp	w0, 1
	beq	.L728
	ldr	w0, [x19, 44]
	cmp	w0, 2
	beq	.L728
	ldrb	w1, [x22, 9]
	cmp	w1, 1
	beq	.L764
.L730:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w23, w0
	strb	w23, [x19, 10]
	cbz	w23, .L744
	add	x24, x21, 188
	ldrb	w25, [x24, 10]
	uxtb	w1, w0
	mov	w0, w25
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w23, 8
	bls	.L765
	strb	wzr, [x19, 3]
.L734:
	cmp	w25, 1
	beq	.L766
	strb	wzr, [x19, 11]
.L736:
	ldrb	w0, [x22, 5]
	cmp	w0, 1
	beq	.L767
.L737:
	ldrb	w0, [x24, 6]
	cmp	w0, 1
	beq	.L768
.L738:
	ldr	w0, [x19, 44]
	cbz	w0, .L769
	ldrb	w0, [x24, 12]
	cmp	w0, 1
	beq	.L770
	strb	wzr, [x19, 25]
.L742:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 26]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x19, 120]
	beq	.L771
.L743:
	strb	wzr, [x19, 34]
.L740:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x26
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L744
	ldr	w1, [x19, 44]
	cmp	w1, 2
	beq	.L772
.L745:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	ldr	w1, [x19, 44]
	cbnz	w1, .L746
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
.L747:
	ldr	w22, [x19, 44]
	strb	w0, [x19, 19]
	cmp	w22, 2
	beq	.L773
.L748:
	ldrb	w1, [x24, 7]
	cbnz	w1, .L774
.L750:
	ldrb	w1, [x24, 8]
	cmp	w1, 1
	beq	.L775
.L752:
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 22]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 24]
	mov	w1, 1
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L744:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L728:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	ldrb	w1, [x22, 9]
	cmp	w1, 1
	bne	.L730
.L764:
	ldr	w0, [x19, 44]
	cbnz	w0, .L730
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	b	.L730
	.p2align 3
.L746:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	b	.L747
	.p2align 3
.L765:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w25, [x24, 10]
	b	.L734
	.p2align 3
.L771:
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 17]
	b	.L743
	.p2align 3
.L769:
	ldrb	w1, [x19, 116]
	mov	x0, x20
	bl	VC1_DecMvmodeProgrP
	uxtb	w0, w0
	cmp	w0, 4
	strb	w0, [x19, 14]
	bne	.L740
	ldrb	w1, [x19, 116]
	mov	x0, x20
	bl	VC1_DecMvmode2
	strb	w0, [x19, 15]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 17]
	b	.L740
	.p2align 3
.L774:
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L750
	.p2align 3
.L766:
	mov	w1, w25
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L736
	.p2align 3
.L768:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L738
	.p2align 3
.L767:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L737
	.p2align 3
.L772:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	b	.L745
	.p2align 3
.L773:
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 28]
	ldrb	w1, [x19, 26]
	mov	w0, 0
	cmp	w1, 1
	beq	.L776
	strb	w0, [x19, 29]
	b	.L748
	.p2align 3
.L775:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L752
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L752
	.p2align 3
.L770:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L742
.L776:
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 29]
	b	.L748
	.size	VC1_Vfmw_ParseAdvPPicHdr, .-VC1_Vfmw_ParseAdvPPicHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvBPicHdr
	.type	VC1_Vfmw_ParseAdvBPicHdr, %function
VC1_Vfmw_ParseAdvBPicHdr:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	x25, x1
	bl	_mcount
	add	x22, x21, 16
	mov	w1, 1
	mov	x0, x20
	add	x19, x21, 360
	bl	BsGet
	strb	w0, [x21, 360]
	ldrb	w0, [x22, 7]
	cmp	w0, 1
	beq	.L778
	ldr	w2, [x19, 44]
	cmp	w2, 2
	beq	.L778
.L779:
	ldrb	w1, [x22, 9]
	cmp	w1, 1
	beq	.L819
.L780:
	cbnz	w2, .L784
	add	x1, x21, 508
	mov	x0, x20
	bl	VC1_DecBfraction
	uxtb	w0, w0
	cmp	w0, 21
	strb	w0, [x19, 2]
	beq	.L782
.L784:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w23, w0
	strb	w23, [x19, 10]
	cbz	w23, .L782
	add	x24, x21, 188
	ldrb	w26, [x24, 10]
	uxtb	w1, w0
	mov	w0, w26
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w23, 8
	bls	.L820
	strb	wzr, [x19, 3]
.L787:
	cmp	w26, 1
	beq	.L821
	strb	wzr, [x19, 11]
.L789:
	ldrb	w0, [x22, 5]
	cmp	w0, 1
	beq	.L822
.L790:
	ldr	w0, [x19, 44]
	cmp	w0, 2
	beq	.L823
.L791:
	ldrb	w0, [x24, 6]
	cmp	w0, 1
	beq	.L824
.L792:
	ldr	w0, [x19, 44]
	cbz	w0, .L825
	ldrb	w0, [x24, 12]
	cmp	w0, 1
	beq	.L826
.L796:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 120]
.L795:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x25
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L782
	ldr	w1, [x19, 44]
	cmp	w1, 2
	beq	.L827
.L797:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	ldr	w1, [x19, 44]
	cbnz	w1, .L798
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
.L799:
	ldr	w22, [x19, 44]
	strb	w0, [x19, 19]
	cmp	w22, 2
	beq	.L828
.L800:
	ldrb	w1, [x24, 7]
	cbnz	w1, .L829
.L801:
	ldrb	w1, [x24, 8]
	cmp	w1, 1
	beq	.L830
.L803:
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 22]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 24]
	mov	w1, 1
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L782:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L819:
	cbnz	w2, .L784
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	ldr	w2, [x19, 44]
	b	.L780
	.p2align 3
.L778:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	ldr	w2, [x19, 44]
	b	.L779
	.p2align 3
.L798:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	b	.L799
	.p2align 3
.L820:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w26, [x24, 10]
	b	.L787
	.p2align 3
.L825:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, wzr
	mov	w1, 3
	csinc	w1, w1, wzr, eq
	strb	w1, [x19, 14]
	b	.L795
	.p2align 3
.L829:
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L801
	.p2align 3
.L824:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L792
	.p2align 3
.L823:
	add	x1, x21, 508
	mov	x0, x20
	bl	VC1_DecBfraction
	strb	w0, [x19, 2]
	b	.L791
	.p2align 3
.L822:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L790
	.p2align 3
.L821:
	mov	w1, w26
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L789
	.p2align 3
.L830:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L803
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L803
	.p2align 3
.L828:
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 28]
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 29]
	b	.L800
	.p2align 3
.L827:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	b	.L797
	.p2align 3
.L826:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L796
	.size	VC1_Vfmw_ParseAdvBPicHdr, .-VC1_Vfmw_ParseAdvBPicHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvFramePicHdr
	.type	VC1_Vfmw_ParseAdvFramePicHdr, %function
VC1_Vfmw_ParseAdvFramePicHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x23, x1
	mov	x21, x2
	bl	_mcount
	mov	w1, 4
	mov	x0, x19
	bl	BsShow
	cmp	w0, 7
	ble	.L852
	cmp	w0, 11
	bgt	.L864
	mov	w24, 2
	mov	w1, w24
.L832:
	add	x20, x21, 360
	mov	x0, x19
	add	x22, x21, 16
	bl	BsSkip
	strb	w24, [x20, 1]
	ldrb	w0, [x23, 3545]
	strb	w0, [x23, 2]
	ldrb	w0, [x22, 8]
	cbnz	w0, .L865
.L833:
	ldrb	w0, [x22, 6]
	cbz	w0, .L834
	ldrb	w0, [x22, 7]
	cbnz	w0, .L835
	mov	w1, 2
	mov	x0, x19
	bl	BsGet
	strb	w0, [x20, 6]
	ldrb	w0, [x22, 7]
	cmp	w0, 1
	ldrb	w0, [x22, 6]
	beq	.L839
.L868:
	cmp	w0, 1
	bne	.L838
	ldrb	w0, [x20, 6]
	add	w0, w0, 1
	str	w0, [x20, 144]
.L841:
	ldrb	w1, [x21, 190]
	cmp	w1, 1
	beq	.L842
.L850:
	ldrb	w0, [x20, 1]
	cmp	w0, 5
	bls	.L866
.L856:
	mov	w0, 0
	b	.L843
	.p2align 3
.L866:
	ldr	x1, .LC123
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx845
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx845:
	.section	.rodata
	.align	0
	.align	2
.L845:
	.byte	(.L848 - .Lrtx845) / 4
	.byte	(.L846 - .Lrtx845) / 4
	.byte	(.L847 - .Lrtx845) / 4
	.byte	(.L856 - .Lrtx845) / 4
	.byte	(.L848 - .Lrtx845) / 4
	.byte	(.L849 - .Lrtx845) / 4
	.text
	.p2align 3
.L864:
	cmp	w0, 13
	ble	.L854
	cmp	w0, 14
	beq	.L867
	mov	w24, 5
	mov	w1, 4
	b	.L832
	.p2align 3
.L848:
	mov	x2, x21
	mov	x1, x23
	mov	x0, x19
	bl	VC1_Vfmw_ParseAdvIPicHdr
.L843:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L834:
	mov	w0, 1
	strb	w0, [x20, 4]
	ldrb	w0, [x22, 7]
	cmp	w0, 1
	beq	.L837
.L838:
	mov	w0, 1
	str	w0, [x20, 144]
	ldrb	w1, [x21, 190]
	cmp	w1, 1
	bne	.L850
.L842:
	mov	x0, x19
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 7]
	bne	.L850
	ldr	w0, [x20, 144]
	cmp	w0, wzr
	ble	.L850
	mov	w24, 0
	.p2align 2
.L851:
	add	x22, x21, x24, sxtw 2
	mov	w1, 18
	mov	x0, x19
	add	w24, w24, 1
	bl	BsGet
	str	w0, [x22, 412]
	mov	w1, 18
	mov	x0, x19
	bl	BsGet
	str	w0, [x22, 428]
	mov	w1, 14
	mov	x0, x19
	bl	BsGet
	str	w0, [x22, 444]
	mov	w1, 14
	mov	x0, x19
	bl	BsGet
	str	w0, [x22, 460]
	ldr	w0, [x20, 144]
	cmp	w0, w24
	bgt	.L851
	b	.L850
	.p2align 3
.L837:
	mov	w0, 2
	str	w0, [x20, 144]
	b	.L841
	.p2align 3
.L852:
	mov	w24, 1
	mov	w1, w24
	b	.L832
	.p2align 3
.L865:
	mov	w1, 8
	mov	x0, x19
	bl	BsGet
	str	w0, [x20, 48]
	b	.L833
	.p2align 3
.L835:
	mov	w1, 1
	mov	x0, x19
	bl	BsGet
	strb	w0, [x20, 4]
	mov	w1, 1
	mov	x0, x19
	bl	BsGet
	strb	w0, [x20, 5]
	ldrb	w0, [x22, 7]
	cmp	w0, 1
	ldrb	w0, [x22, 6]
	bne	.L868
.L839:
	cmp	w0, 1
	bne	.L837
	ldrb	w0, [x20, 5]
	add	w0, w0, 2
	str	w0, [x20, 144]
	b	.L841
	.p2align 3
.L846:
	mov	x2, x21
	mov	x1, x23
	mov	x0, x19
	bl	VC1_Vfmw_ParseAdvPPicHdr
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L849:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L847:
	mov	x2, x21
	mov	x1, x23
	mov	x0, x19
	bl	VC1_Vfmw_ParseAdvBPicHdr
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L854:
	mov	w24, 0
	mov	w1, 3
	b	.L832
	.p2align 3
.L867:
	mov	w24, 4
	mov	w1, w24
	b	.L832
	.size	VC1_Vfmw_ParseAdvFramePicHdr, .-VC1_Vfmw_ParseAdvFramePicHdr
	.align	3
.LC123:
	.xword	.L845
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvIFieldHdr
	.type	VC1_Vfmw_ParseAdvIFieldHdr, %function
VC1_Vfmw_ParseAdvIFieldHdr:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x2
	mov	x25, x1
	add	x19, x21, 360
	bl	_mcount
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 10]
	cbz	w22, .L877
	add	x24, x21, 188
	ldrb	w23, [x24, 10]
	uxtb	w1, w0
	mov	w0, w23
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L883
	strb	wzr, [x19, 3]
.L873:
	cmp	w23, 1
	beq	.L884
	strb	wzr, [x19, 11]
.L875:
	ldrb	w0, [x21, 21]
	cmp	w0, 1
	beq	.L885
.L876:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x25
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L877
	mov	x0, x20
	mov	w22, 1
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 22]
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 23]
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 24]
	ldrb	w1, [x24, 7]
	cbz	w1, .L871
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	ldr	x25, [sp, 64]
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L877:
	mov	w22, 0
.L871:
	mov	w0, w22
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L883:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w23, [x24, 10]
	b	.L873
	.p2align 3
.L885:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L876
	.p2align 3
.L884:
	mov	w1, w23
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L875
	.size	VC1_Vfmw_ParseAdvIFieldHdr, .-VC1_Vfmw_ParseAdvIFieldHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvPFieldHdr
	.type	VC1_Vfmw_ParseAdvPFieldHdr, %function
VC1_Vfmw_ParseAdvPFieldHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x23, x2
	add	x19, x23, 360
	bl	_mcount
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 10]
	mov	w1, 0
	cbz	w22, .L887
	add	x21, x23, 188
	ldrb	w24, [x21, 10]
	mov	w1, w22
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L914
	strb	wzr, [x19, 3]
.L889:
	cmp	w24, 1
	beq	.L915
	strb	wzr, [x19, 11]
.L891:
	ldrb	w0, [x23, 21]
	cmp	w0, 1
	beq	.L916
.L892:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 32]
	cbz	w0, .L917
.L893:
	ldrb	w0, [x21, 6]
	cmp	w0, 1
	beq	.L918
.L894:
	ldrb	w0, [x21, 12]
	cmp	w0, 1
	beq	.L919
.L895:
	ldrb	w1, [x19, 116]
	mov	x0, x20
	bl	VC1_DecMvmodeProgrP
	uxtb	w0, w0
	cmp	w0, 4
	strb	w0, [x19, 14]
	beq	.L920
.L897:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	ldrb	w1, [x19, 32]
	cbnz	w1, .L899
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	mov	w1, 3
	strb	w0, [x19, 18]
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 19]
	ldrb	w1, [x19, 14]
	cbz	w1, .L901
.L923:
	ldrh	w0, [x19, 14]
	cmp	w0, 4
	beq	.L901
	ldrb	w1, [x21, 7]
	cbnz	w1, .L921
.L903:
	ldrb	w1, [x21, 8]
	cmp	w1, 1
	beq	.L922
.L905:
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 22]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 24]
	mov	w1, 1
.L887:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L899:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	mov	w1, 3
	strb	w0, [x19, 18]
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 19]
	ldrb	w1, [x19, 14]
	cbnz	w1, .L923
.L901:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 29]
	ldrb	w1, [x21, 7]
	cbz	w1, .L903
.L921:
	ldrb	w2, [x19, 116]
	add	x3, x23, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L903
	.p2align 3
.L914:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w24, [x21, 10]
	b	.L889
	.p2align 3
.L917:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, wzr
	cset	w0, eq
	strb	w0, [x19, 33]
	b	.L893
	.p2align 3
.L920:
	ldrb	w1, [x19, 116]
	mov	x0, x20
	bl	VC1_DecMvmode2
	strb	w0, [x19, 15]
	mov	x0, x20
	bl	VC1_DecIntCompField
	str	w0, [x19, 128]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 17]
	ldr	w1, [x19, 128]
	cbnz	w1, .L897
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 132]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 136]
	b	.L897
	.p2align 3
.L919:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L895
	.p2align 3
.L922:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L905
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L905
	.p2align 3
.L918:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L894
	.p2align 3
.L916:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L892
	.p2align 3
.L915:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L891
	.size	VC1_Vfmw_ParseAdvPFieldHdr, .-VC1_Vfmw_ParseAdvPFieldHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvBFieldHdr
	.type	VC1_Vfmw_ParseAdvBFieldHdr, %function
VC1_Vfmw_ParseAdvBFieldHdr:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x19, x2, 360
	mov	x21, x2
	mov	x25, x1
	bl	_mcount
	mov	w0, 1
	mov	w1, 5
	strb	w0, [x19, 32]
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 10]
	cbz	w22, .L934
	add	x23, x21, 188
	ldrb	w24, [x23, 10]
	uxtb	w1, w0
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L946
	strb	wzr, [x19, 3]
.L928:
	cmp	w24, 1
	beq	.L947
	strb	wzr, [x19, 11]
.L930:
	ldrb	w0, [x21, 21]
	cmp	w0, 1
	beq	.L948
.L931:
	ldrb	w0, [x23, 6]
	cmp	w0, 1
	beq	.L949
.L932:
	ldrb	w0, [x23, 12]
	cmp	w0, 1
	beq	.L950
.L933:
	ldrb	w1, [x19, 116]
	mov	x0, x20
	bl	VC1_DecMvmodeInterB
	strb	w0, [x19, 14]
	mov	w3, 0
	mov	x2, x21
	mov	x1, x25
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L934
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 19]
	ldrb	w1, [x19, 14]
	cbz	w1, .L951
.L935:
	ldrb	w1, [x23, 7]
	cbnz	w1, .L952
.L936:
	ldrb	w1, [x23, 8]
	cmp	w1, 1
	beq	.L953
.L938:
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 22]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 24]
	mov	w1, 1
	mov	w0, w1
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L934:
	mov	w1, 0
	ldr	x25, [sp, 64]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L946:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w24, [x23, 10]
	b	.L928
	.p2align 3
.L952:
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L936
	.p2align 3
.L951:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 29]
	b	.L935
	.p2align 3
.L950:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L933
	.p2align 3
.L949:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L932
	.p2align 3
.L948:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L931
	.p2align 3
.L947:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L930
	.p2align 3
.L953:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L938
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L938
	.size	VC1_Vfmw_ParseAdvBFieldHdr, .-VC1_Vfmw_ParseAdvBFieldHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvFieldPicHdr
	.type	VC1_Vfmw_ParseAdvFieldPicHdr, %function
VC1_Vfmw_ParseAdvFieldPicHdr:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x1
	mov	x21, x0
	mov	x0, x30
	mov	x22, x2
	bl	_mcount
	ldrb	w0, [x20, 8]
	cbz	w0, .L955
	add	x0, x20, 20480
	ldr	w0, [x0, 3628]
	cbnz	w0, .L955
	add	x19, x22, 360
	ldr	w0, [x19, 156]
	strb	w0, [x19, 1]
	ldrb	w0, [x20, 3545]
	strb	w0, [x20, 2]
.L974:
	ldrb	w0, [x19, 1]
	cmp	w0, 4
	bls	.L995
.L983:
	ldrb	w1, [x19, 4]
	mov	w0, 0
	cbz	w1, .L981
.L1000:
	ldrb	w1, [x20, 8]
	cmp	w1, wzr
	cset	w1, ne
	strb	w1, [x19, 41]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L955:
	mov	w1, 3
	mov	x0, x21
	add	x19, x22, 360
	add	x23, x22, 16
	bl	BsGet
	sbfiz	x1, x0, 3, 32
	ldr	x2, .LC124
	str	w0, [x19, 124]
	add	x0, x2, x1
	ldr	w1, [x2, x1]
	ldr	w0, [x0, 4]
	stp	w1, w0, [x19, 152]
	uxtb	w1, w1
	ldrb	w2, [x20, 8]
	uxtb	w0, w0
	cmp	w2, wzr
	csel	w0, w0, w1, ne
	strb	w0, [x19, 1]
	ldrb	w0, [x20, 3545]
	strb	w0, [x20, 2]
	ldrb	w0, [x23, 8]
	cbnz	w0, .L996
	ldrb	w0, [x23, 6]
	cbnz	w0, .L997
.L960:
	mov	w0, 1
	strb	w0, [x19, 4]
	ldrb	w0, [x23, 7]
	cmp	w0, 1
	beq	.L963
.L964:
	mov	w0, 1
	str	w0, [x19, 144]
.L967:
	add	x23, x22, 188
	ldrb	w1, [x23, 2]
	cmp	w1, 1
	beq	.L968
.L971:
	mov	w1, 1
	mov	x0, x21
	bl	BsGet
	strb	w0, [x22, 360]
	mov	w1, 1
	mov	x0, x21
	bl	BsGet
	strb	w0, [x19, 8]
	ldrb	w0, [x23, 3]
	cmp	w0, 1
	beq	.L998
.L970:
	ldr	w0, [x19, 124]
	cmp	w0, 3
	ble	.L974
.L975:
	add	x1, x22, 508
	mov	x0, x21
	bl	VC1_DecBfraction
	strb	w0, [x19, 2]
	ldrb	w0, [x19, 1]
	cmp	w0, 4
	bhi	.L983
	.p2align 2
.L995:
	ldr	x1, .LC125
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx978
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx978:
	.section	.rodata
	.align	0
	.align	2
.L978:
	.byte	(.L977 - .Lrtx978) / 4
	.byte	(.L979 - .Lrtx978) / 4
	.byte	(.L980 - .Lrtx978) / 4
	.byte	(.L983 - .Lrtx978) / 4
	.byte	(.L977 - .Lrtx978) / 4
	.text
	.p2align 3
.L997:
	ldrb	w0, [x23, 7]
	cbz	w0, .L999
	mov	w1, 1
	mov	x0, x21
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 1
	mov	x0, x21
	bl	BsGet
	strb	w0, [x19, 5]
.L962:
	ldrb	w0, [x23, 7]
	cmp	w0, 1
	ldrb	w0, [x23, 6]
	beq	.L965
	cmp	w0, 1
	bne	.L964
	ldrb	w0, [x19, 6]
	add	w0, w0, 1
	str	w0, [x19, 144]
	b	.L967
	.p2align 3
.L980:
	mov	x2, x22
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvBFieldHdr
.L976:
	ldrb	w1, [x19, 4]
	cbnz	w1, .L1000
.L981:
	ldrb	w1, [x20, 8]
	cmp	w1, 1
	cset	w1, ne
	strb	w1, [x19, 41]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L979:
	mov	x2, x22
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvPFieldHdr
	b	.L976
	.p2align 3
.L977:
	mov	x2, x22
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvIFieldHdr
	b	.L976
	.p2align 3
.L996:
	mov	w1, 8
	mov	x0, x21
	bl	BsGet
	str	w0, [x19, 48]
	ldrb	w0, [x23, 6]
	cbz	w0, .L960
	b	.L997
	.p2align 3
.L999:
	mov	w1, 2
	mov	x0, x21
	bl	BsGet
	strb	w0, [x19, 6]
	b	.L962
	.p2align 3
.L998:
	ldr	w0, [x19, 124]
	cmp	w0, 3
	bgt	.L975
	mov	x0, x21
	bl	VC1_DecRefdist
	strb	w0, [x19, 31]
	b	.L970
	.p2align 3
.L963:
	mov	w0, 2
	str	w0, [x19, 144]
	b	.L967
	.p2align 3
.L965:
	cmp	w0, 1
	bne	.L963
	ldrb	w0, [x19, 5]
	add	w0, w0, 2
	str	w0, [x19, 144]
	b	.L967
	.p2align 3
.L968:
	mov	x0, x21
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 7]
	bne	.L971
	ldr	w0, [x19, 144]
	cmp	w0, wzr
	ble	.L971
	mov	w25, 0
	.p2align 2
.L972:
	add	x24, x22, x25, sxtw 2
	mov	w1, 18
	mov	x0, x21
	add	w25, w25, 1
	bl	BsGet
	str	w0, [x24, 412]
	mov	w1, 18
	mov	x0, x21
	bl	BsGet
	str	w0, [x24, 428]
	mov	w1, 14
	mov	x0, x21
	bl	BsGet
	str	w0, [x24, 444]
	mov	w1, 14
	mov	x0, x21
	bl	BsGet
	str	w0, [x24, 460]
	ldr	w0, [x19, 144]
	cmp	w0, w25
	bgt	.L972
	b	.L971
	.size	VC1_Vfmw_ParseAdvFieldPicHdr, .-VC1_Vfmw_ParseAdvFieldPicHdr
	.align	3
.LC124:
	.xword	.LANCHOR1
	.align	3
.LC125:
	.xword	.L978
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvPicHdr
	.type	VC1_Vfmw_ParseAdvPicHdr, %function
VC1_Vfmw_ParseAdvPicHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x19, x1
	mov	x20, x2
	bl	_mcount
	add	x2, x19, 20480
	add	x0, x19, 36864
	ldr	w1, [x2, 3544]
	str	w1, [x0, 1664]
	lsl	w4, w1, 4
	ldr	w3, [x2, 3548]
	str	w3, [x0, 1668]
	str	w4, [x0, 1656]
	lsl	w4, w3, 4
	mul	w1, w1, w3
	str	w1, [x0, 1672]
	str	w4, [x0, 1660]
	ldrb	w0, [x19, 8]
	cbz	w0, .L1002
	ldr	w0, [x2, 3628]
	cbz	w0, .L1012
.L1002:
	ldrb	w23, [x20, 23]
	cmp	w23, 1
	beq	.L1013
	mov	w1, 0
	mov	w0, 0
.L1004:
	str	w0, [x20, 404]
	mov	x2, x20
	cbz	w1, .L1014
.L1006:
	mov	x1, x19
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvFieldPicHdr
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1012:
	ldr	w1, [x20, 404]
	mov	x2, x20
	and	w1, w1, -3
	cbnz	w1, .L1006
.L1014:
	mov	x1, x19
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvFramePicHdr
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1013:
	mov	w1, 2
	mov	x0, x21
	bl	BsShow
	cmp	w0, 1
	mov	w22, w0
	ble	.L1015
	mov	w1, 2
	mov	x0, x21
	bl	BsSkip
	and	w1, w22, -3
	mov	w0, w22
	b	.L1004
	.p2align 3
.L1015:
	mov	w1, w23
	mov	x0, x21
	bl	BsSkip
	mov	w1, 0
	mov	w0, 0
	b	.L1004
	.size	VC1_Vfmw_ParseAdvPicHdr, .-VC1_Vfmw_ParseAdvPicHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvSlcHdr
	.type	VC1_Vfmw_ParseAdvSlcHdr, %function
VC1_Vfmw_ParseAdvSlcHdr:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x22, x1
	mov	x21, x2
	bl	_mcount
	add	x2, x22, 20480
	mov	w1, 9
	mov	x0, x20
	ldr	w19, [x2, 3628]
	bl	BsGet
	mov	w1, 1
	uxtw	x19, w19
	lsl	x2, x19, 2
	sub	x19, x2, x19
	add	x19, x21, x19, lsl 2
	str	w0, [x19, 540]
	mov	x0, x20
	bl	BsGet
	sxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 536]
	beq	.L1019
	mov	w0, 1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1019:
	add	x19, x22, 4096
	add	x4, x21, 188
	add	x3, x19, 2884
	mov	x2, 172
	add	x23, x19, 2696
	ldp	x0, x1, [x4]
	stp	x0, x1, [x3]
	add	x1, x21, 16
	add	x0, x19, 2712
	ldp	x6, x7, [x4, 16]
	stp	x6, x7, [x3, 16]
	ldp	x6, x7, [x4, 32]
	stp	x6, x7, [x3, 32]
	ldr	x5, [x21, 236]
	str	x5, [x3, 48]
	ldr	w4, [x21, 244]
	str	w4, [x22, 7036]
	bl	memcpy
	add	x1, x21, 360
	mov	x2, 176
	add	x0, x19, 3056
	bl	memcpy
	mov	x2, x23
	mov	x1, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseAdvPicHdr
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	VC1_Vfmw_ParseAdvSlcHdr, .-VC1_Vfmw_ParseAdvSlcHdr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_ClearCurPacket
	.type	VC1_ClearCurPacket, %function
VC1_ClearCurPacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 36]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
	ldrsb	w0, [x19, 16]
	cmp	w0, 1
	beq	.L1021
	ldr	w1, [x19, 40]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
.L1021:
	ldr	x3, .LC126
	mov	x2, 48
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_ClearCurPacket, .-VC1_ClearCurPacket
	.align	3
.LC126:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_ClearCurPic
	.type	VC1_ClearCurPic, %function
VC1_ClearCurPic:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	w23, w1
	bl	_mcount
	ldrsb	w0, [x22, 3104]
	cbz	w0, .L1026
	add	x24, x22, 20480
	mov	w20, 0
	ldr	w0, [x24, 3628]
	cbnz	w0, .L1036
	b	.L1025
	.p2align 3
.L1029:
	ldr	w0, [x24, 3628]
	cmp	w0, w20
	bls	.L1025
.L1036:
	sxtw	x19, w20
	mov	w0, w23
	lsl	x1, x19, 3
	add	w20, w20, 1
	sub	x19, x1, x19
	add	x19, x22, x19, lsl 3
	add	x21, x19, 20480
	ldr	w1, [x21, 3680]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 24120]
	cbz	x0, .L1029
	ldr	w0, [x21, 3668]
	cbz	w0, .L1029
	ldr	w1, [x21, 3684]
	mov	w0, w23
	bl	SM_ReleaseStreamSeg
	ldr	w0, [x24, 3628]
	cmp	w0, w20
	bhi	.L1036
.L1025:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1026:
	add	x1, x22, 36864
	mov	w0, w23
	add	x1, x1, 1592
	bl	VC1_ClearCurPacket
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	VC1_ClearCurPic, .-VC1_ClearCurPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1DEC_VDMPostProc
	.type	VC1DEC_VDMPostProc, %function
VC1DEC_VDMPostProc:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	add	x19, x21, 53248
	mov	w22, w1
	add	x20, x21, 12288
	bl	_mcount
	add	x0, x20, 2520
	ldr	w1, [x19, 800]
	add	x25, x0, 20480
	bl	VC1_ClearCurPic
	ldr	w1, [x25, 3620]
	ldr	w0, [x19, 800]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	ldr	w1, [x25, 3616]
	ldr	w0, [x19, 800]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	ldr	w1, [x25, 3612]
	ldr	w0, [x19, 800]
	bl	FSP_GetFsImagePtr
	cbz	x23, .L1074
	tbnz	w22, #31, .L1060
	mov	w0, 100
	cmp	w22, w0
	csel	w22, w22, w0, le
.L1043:
	mov	x0, x21
	str	w22, [x23, 160]
	bl	VC1_SetImgFormat
	ldrsb	w1, [x20, 2523]
	cmp	w1, 3
	beq	.L1075
	str	xzr, [x23, 560]
	str	xzr, [x23, 568]
	ldr	x0, [x21]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L1045
.L1080:
	add	x1, x20, 2520
	ldrb	w0, [x1, 2]
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cbz	w0, .L1046
	add	x1, x1, 20480
	ldr	w2, [x1, 3608]
	cmp	w2, 1
	bls	.L1061
	ldr	w1, [x1, 3612]
.L1047:
	add	x0, x20, 2520
	cmp	w2, 1
	add	x0, x0, 20480
	cset	w22, hi
	str	w1, [x0, 3624]
.L1048:
	add	x24, x20, 2520
	cmn	w1, #1
	add	x0, x24, 36864
	ldr	w2, [x0, 1584]
	add	w2, w2, 1
	str	w2, [x0, 1584]
	beq	.L1049
.L1079:
	ldr	w0, [x19, 800]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L1076
	ldr	w2, [x0, 160]
	cbz	w2, .L1051
	ldr	x0, [x21]
	ldr	w3, [x0, 4]
	cmp	w2, w3
	bhi	.L1052
.L1051:
	add	x24, x20, 2520
	ldr	w0, [x19, 800]
	add	x25, x24, 20480
	ldr	w26, [x25, 3624]
	mov	w1, w26
	bl	FSP_GetRef
	mov	w3, w0
	ldr	x1, .LC132
	mov	w2, w26
	mov	w0, 12
	bl	dprint_vfmw
	ldr	w0, [x19, 800]
	mov	w2, 1
	ldr	w1, [x25, 3624]
	bl	FSP_SetDisplay
	cbnz	w22, .L1077
	ldr	x1, [x24, 23992]
	mov	w2, 1
	mov	w0, 2
	strb	w2, [x1, 1]
	strb	w0, [x24, 1]
.L1054:
	ldr	w0, [x19, 800]
	mov	x4, x23
	add	x3, x19, 192
	mov	x2, x21
	mov	w1, 1
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L1055
	ldr	w0, [x19, 800]
	add	x20, x20, 2520
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	str	xzr, [x23, 560]
	ldr	x1, [x23, 568]
	ldr	w0, [x19, 800]
	bl	FreeUsdByDec
	str	xzr, [x23, 568]
	add	x0, x20, 20480
	mov	w2, 0
	ldr	w1, [x0, 3624]
	ldr	w0, [x19, 800]
	bl	FSP_SetDisplay
	cbnz	w22, .L1078
	ldr	x1, [x20, 23992]
	mov	w0, 0
	strb	wzr, [x1, 1]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1046:
	add	x2, x1, 20480
	ldr	w1, [x2, 3620]
	str	w1, [x2, 3624]
	cbz	x24, .L1062
	ldr	x0, [x23, 600]
	mov	w22, 0
	ldr	x3, [x24, 600]
	cmp	x3, x0
	bge	.L1048
	str	x3, [x23, 600]
	str	x0, [x24, 600]
	add	x24, x20, 2520
	add	x0, x24, 36864
	ldr	w1, [x2, 3624]
	ldr	w2, [x0, 1584]
	cmn	w1, #1
	add	w2, w2, 1
	str	w2, [x0, 1584]
	bne	.L1079
	.p2align 2
.L1049:
	mov	w0, 1
.L1042:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1075:
	ldrb	w0, [x20, 2529]
	cmp	w0, 1
	bne	.L1049
	str	xzr, [x23, 560]
	str	xzr, [x23, 568]
	ldr	x0, [x21]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L1080
	.p2align 2
.L1045:
	add	x0, x20, 2520
	mov	w22, 0
	add	x0, x0, 20480
	ldr	w1, [x0, 3620]
	str	w1, [x0, 3624]
	b	.L1048
	.p2align 3
.L1060:
	mov	w22, 0
	b	.L1043
	.p2align 3
.L1055:
	add	x20, x20, 2520
	ldrsb	w22, [x20, 1]
	cmp	w22, 3
	beq	.L1081
	cmp	w22, 1
	beq	.L1082
	cmp	w22, 2
	bne	.L1049
	ldr	x1, [x20, 23992]
	strb	wzr, [x1, 1]
	strb	wzr, [x20, 1]
	b	.L1042
	.p2align 3
.L1077:
	ldr	x0, [x24, 24008]
	strb	w22, [x0, 1]
	strb	w22, [x24, 1]
	b	.L1054
	.p2align 3
.L1052:
	ldr	x1, .LC134
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 800]
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	str	xzr, [x23, 560]
	ldr	x1, [x23, 568]
	ldr	w0, [x19, 800]
	bl	FreeUsdByDec
	str	xzr, [x23, 568]
	add	x0, x24, 20480
	mov	w2, 0
	ldr	w1, [x0, 3624]
	ldr	w0, [x19, 800]
	bl	FSP_SetDisplay
	cbnz	w22, .L1083
	ldr	x1, [x24, 23992]
	mov	w0, 1
	strb	wzr, [x1, 1]
	b	.L1042
	.p2align 3
.L1061:
	mov	w1, -1
	b	.L1047
	.p2align 3
.L1078:
	ldr	x1, [x20, 24008]
	mov	w0, 0
	strb	wzr, [x1, 1]
	b	.L1042
	.p2align 3
.L1082:
	ldr	x1, [x20, 24008]
	mov	w0, w22
	strb	wzr, [x1, 1]
	strb	wzr, [x20, 1]
	b	.L1042
	.p2align 3
.L1081:
	ldr	x0, [x20, 24000]
	strb	wzr, [x0, 1]
	strb	wzr, [x20, 1]
	b	.L1049
	.p2align 3
.L1074:
	ldr	x1, .LC128
	mov	x4, x0
	mov	x3, x24
	mov	x2, 0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1042
	.p2align 3
.L1062:
	mov	w22, 0
	b	.L1048
	.p2align 3
.L1076:
	ldr	x1, .LC130
	mov	w2, 733
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1042
	.p2align 3
.L1083:
	ldr	x1, [x24, 24008]
	mov	w0, w22
	strb	wzr, [x1, 1]
	b	.L1042
	.size	VC1DEC_VDMPostProc, .-VC1DEC_VDMPostProc
	.align	3
.LC128:
	.xword	.LC127
	.align	3
.LC130:
	.xword	.LC129
	.align	3
.LC132:
	.xword	.LC131
	.align	3
.LC134:
	.xword	.LC133
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_DecodeArbiter
	.type	VC1_Vfmw_DecodeArbiter, %function
VC1_Vfmw_DecodeArbiter:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 12288
	bl	_mcount
	add	x3, x19, 2520
	ldrb	w0, [x19, 2520]
	tbz	x0, 4, .L1137
	mov	w2, -21
	add	x1, x3, 36864
	and	w0, w0, w2
	strb	w0, [x19, 2520]
	ldr	w0, [x1, 1716]
	sub	w0, w0, #2
	cmp	w0, 1
	ldr	x0, [x20]
	bls	.L1087
	add	x3, x3, 20480
	ldr	w2, [x0, 8]
	ldr	w3, [x3, 3608]
.L1088:
	add	x0, x19, 2520
	sub	w1, w2, #1
	add	x0, x0, 36864
	cmp	w1, 3
	str	w2, [x0, 1716]
	bls	.L1139
.L1115:
	mov	w4, 1
	.p2align 2
.L1094:
	cbz	w3, .L1105
.L1104:
	cmp	w3, 1
	bls	.L1140
.L1108:
	cbz	w4, .L1107
	add	x19, x19, 2520
	add	x20, x20, 16384
	ldrsb	w0, [x19, 3104]
	cbnz	w0, .L1110
	add	x1, x20, 1608
	add	x19, x19, 8192
	add	x0, x19, 2048
	add	x20, x20, 1856
	add	x19, x19, 2456
	ldp	x2, x3, [x1]
	stp	x2, x3, [x0, 160]
	mov	w0, 1
	ldp	x2, x3, [x20]
	stp	x2, x3, [x19]
	ldp	x2, x3, [x20, 16]
	stp	x2, x3, [x19, 16]
	ldp	x2, x3, [x20, 32]
	stp	x2, x3, [x19, 32]
	ldp	x2, x3, [x20, 48]
	stp	x2, x3, [x19, 48]
	ldp	x2, x3, [x20, 64]
	stp	x2, x3, [x19, 64]
	ldp	x2, x3, [x20, 80]
	stp	x2, x3, [x19, 80]
	ldp	x2, x3, [x20, 96]
	stp	x2, x3, [x19, 96]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1136:
	ldr	x0, [x20]
	ldr	w1, [x0, 8]
	add	x0, x19, 2520
	add	x0, x0, 36864
	sub	w2, w1, #1
	cmp	w2, 3
	str	w1, [x0, 1716]
	bls	.L1141
	mov	w4, 1
	.p2align 2
.L1105:
	ldrb	w0, [x19, 2522]
	sub	w2, w0, #1
	sub	w1, w0, #4
	uxtb	w2, w2
	uxtb	w1, w1
	cmp	w2, 1
	ccmp	w1, 1, 0, hi
	bls	.L1111
.L1106:
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cbnz	w0, .L1108
	ldr	x1, .LC141
	mov	w0, 1
	bl	dprint_vfmw
	.p2align 2
.L1107:
	add	x20, x20, 53248
	add	x0, x19, 2520
	ldr	w1, [x20, 800]
	bl	VC1_ClearCurPic
	ldr	x0, .LC143
	ldr	x4, [x0]
	cbz	x4, .L1137
	ldr	w0, [x20, 800]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L1137:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1139:
	ldr	x0, .LC137
	ldrb	w0, [x0,w1,uxtw]
	adr	x1, .Lrtx1096
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1096:
	.section	.rodata
	.align	0
	.align	2
.L1096:
	.byte	(.L1095 - .Lrtx1096) / 4
	.byte	(.L1097 - .Lrtx1096) / 4
	.byte	(.L1098 - .Lrtx1096) / 4
	.byte	(.L1099 - .Lrtx1096) / 4
	.text
	.p2align 3
.L1087:
	ldr	w2, [x0, 8]
	cmp	w2, 1
	ccmp	w2, 4, 4, hi
	beq	.L1089
	add	x3, x3, 20480
	ldr	w3, [x3, 3608]
	b	.L1088
	.p2align 3
.L1098:
	mov	w4, 0
	b	.L1094
	.p2align 3
.L1097:
	ldrb	w0, [x19, 2522]
	cmp	w0, wzr
	cset	w4, eq
	b	.L1094
	.p2align 3
.L1099:
	add	x0, x19, 2520
	ldrb	w1, [x0, 2]
	cbnz	w1, .L1100
	add	x0, x0, 36864
	mov	w4, 1
	str	wzr, [x0, 1648]
	b	.L1094
	.p2align 3
.L1095:
	ldrb	w0, [x19, 2522]
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cmp	w0, wzr
	cset	w4, ne
	b	.L1094
	.p2align 3
.L1100:
	cmp	w1, 2
	bne	.L1102
	add	x0, x0, 36864
	ldr	w0, [x0, 1648]
	cbnz	w0, .L1115
	mov	w4, 0
	cbnz	w3, .L1104
	.p2align 2
.L1111:
	ldr	x1, .LC139
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1107
	.p2align 3
.L1110:
	add	x21, x19, 8192
	add	x1, x20, 1624
	mov	x2, 172
	add	x0, x21, 2224
	bl	memcpy
	add	x4, x20, 1796
	add	x3, x21, 2396
	mov	x2, 176
	ldp	x0, x1, [x4]
	stp	x0, x1, [x3]
	add	x1, x20, 1968
	add	x0, x21, 2568
	ldp	x6, x7, [x4, 16]
	stp	x6, x7, [x3, 16]
	ldp	x6, x7, [x4, 32]
	stp	x6, x7, [x3, 32]
	ldr	x5, [x4, 48]
	str	x5, [x3, 48]
	ldr	w4, [x20, 1852]
	str	w4, [x19, 10644]
	bl	memcpy
	ldr	w0, [x20, 2012]
	add	x3, x19, 20480
	strb	w0, [x19, 4]
	add	x1, x20, 2144
	ldr	x2, .LC142
	add	x0, x21, 2744
	ldr	w3, [x3, 3628]
	ldr	x4, [x2, 104]
	lsl	x2, x3, 2
	sub	x2, x2, x3
	lsl	x2, x2, 2
	blr	x4
	ldr	x23, [sp, 48]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1102:
	and	w1, w1, -5
	uxtb	w1, w1
	cmp	w1, 1
	bne	.L1115
	add	x0, x0, 36864
	mov	w4, w1
	str	w1, [x0, 1648]
	cbz	w3, .L1111
	b	.L1104
	.p2align 3
.L1089:
	ldr	x1, .LC136
	add	x21, x3, 20480
	mov	w0, 2
	add	x23, x20, 53248
	bl	dprint_vfmw
	ldr	w1, [x21, 3616]
	ldr	w0, [x23, 800]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L1090
	ldr	w0, [x23, 800]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	ldr	w0, [x23, 800]
	ldr	x1, [x22, 568]
	bl	FreeUsdByDec
	ldr	w0, [x23, 800]
	ldr	x1, [x22, 576]
	bl	FreeUsdByDec
	ldr	w0, [x23, 800]
	ldr	x1, [x22, 584]
	bl	FreeUsdByDec
	add	x0, x22, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w1, [x21, 3616]
	ldr	w0, [x23, 800]
	bl	FSP_ClearLogicFs
	ldr	w1, [x21, 3612]
	mov	w2, 1
	ldr	w0, [x23, 800]
	bl	FSP_ClearLogicFs
	str	wzr, [x21, 3612]
	str	wzr, [x21, 3616]
	ldr	w0, [x23, 800]
	bl	FSP_GetNullLogicFs
	str	w0, [x21, 3612]
	str	w0, [x21, 3616]
.L1090:
	add	x0, x19, 2520
	add	x1, x0, 20480
	str	wzr, [x1, 3608]
	ldr	x2, [x0, 24000]
	cbz	x2, .L1136
	ldr	x3, [x0, 24008]
	cbz	x3, .L1136
	strb	wzr, [x2, 1]
	ldr	x2, [x0, 24008]
	strb	wzr, [x2, 1]
	ldr	x2, [x0, 24000]
	strb	wzr, [x2]
	ldr	x0, [x0, 24008]
	strb	wzr, [x0]
	ldr	x0, [x20]
	ldr	w3, [x1, 3608]
	ldr	w2, [x0, 8]
	b	.L1088
.L1141:
	ldr	x0, .LC144
	mov	w3, 0
	ldrh	w0, [x0,w2,uxtw #1]
	adr	x1, .Lrtx1114
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx1114:
	.section	.rodata
	.align	0
	.align	2
.L1114:
	.2byte	(.L1095 - .Lrtx1114) / 4
	.2byte	(.L1097 - .Lrtx1114) / 4
	.2byte	(.L1098 - .Lrtx1114) / 4
	.2byte	(.L1099 - .Lrtx1114) / 4
	.text
.L1140:
	ldrb	w0, [x19, 2522]
	b	.L1106
	.size	VC1_Vfmw_DecodeArbiter, .-VC1_Vfmw_DecodeArbiter
	.align	3
.LC136:
	.xword	.LC135
	.align	3
.LC137:
	.xword	.L1096
	.align	3
.LC139:
	.xword	.LC138
	.align	3
.LC141:
	.xword	.LC140
	.align	3
.LC142:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC143:
	.xword	g_event_report
	.align	3
.LC144:
	.xword	.L1114
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_StartDecode
	.type	VC1_Vfmw_StartDecode, %function
VC1_Vfmw_StartDecode:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 16
	add	x22, x20, 12288
	bl	_mcount
	add	x21, x22, 2520
	ldr	x0, [x20]
	ldr	w1, [x0, 632]
	str	w1, [x19, 14596]
	ldr	w0, [x0, 672]
	str	w0, [x19, 14600]
	ldrsb	w0, [x21, 3104]
	cbz	w0, .L1159
.L1143:
	mov	x1, x19
	mov	x0, x20
	add	x23, x22, 2520
	bl	VC1_WritePicMsg
	add	x2, x23, 20480
	ldr	x3, .LC145
	add	x1, x2, 3632
	add	x0, x20, 104
	ldr	w2, [x2, 3628]
	strb	w2, [x19, 74]
	uxtw	x2, w2
	ldr	x4, [x3, 104]
	lsl	x3, x2, 3
	sub	x2, x3, x2
	lsl	x2, x2, 3
	blr	x4
	mov	x0, x21
	mov	x1, x19
	bl	VC1_CfgVDH
	ldrb	w0, [x22, 2522]
	cmp	w0, 1
	bls	.L1160
.L1144:
	cmp	w0, 5
	beq	.L1161
	strb	wzr, [x19, 1]
	ldrsb	w1, [x22, 2524]
	ldrb	w0, [x22, 2528]
.L1146:
	ldrh	w2, [x19, 82]
	cmp	w2, 120
	bgt	.L1147
	add	x23, x22, 2520
	mov	w2, 1
	strb	w2, [x19, 75]
	strb	w1, [x20, 16]
	strb	w1, [x23, 3]
	strb	w0, [x23, 9]
	ldrb	w1, [x19, 3]
	cmp	w1, 2
	beq	.L1162
.L1149:
	and	w0, w1, -5
	uxtb	w0, w0
	cmp	w0, 1
	bne	.L1150
	add	x23, x23, 36864
	str	w0, [x23, 1648]
.L1150:
	ldr	x2, [x20]
	str	x19, [x20, 8]
	ldr	w0, [x2, 904]
	cmp	w0, 1
	beq	.L1163
	cmp	w0, 2
	beq	.L1164
.L1154:
	mov	w0, 1
.L1151:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1147:
	add	x23, x22, 2520
	strb	wzr, [x19, 75]
	strb	w1, [x20, 16]
	strb	w1, [x23, 3]
	strb	w0, [x23, 9]
	ldrb	w1, [x19, 3]
	cmp	w1, 2
	bne	.L1149
.L1162:
	add	x24, x23, 36864
	ldr	w0, [x24, 1648]
	cbnz	w0, .L1150
	add	x1, x20, 53248
	mov	x0, x21
	ldr	w1, [x1, 800]
	bl	VC1_ClearCurPic
	ldr	x1, [x23, 23992]
	mov	w0, 0
	strb	wzr, [x1]
	ldr	w1, [x24, 1652]
	add	w1, w1, 1
	str	w1, [x24, 1652]
	str	xzr, [x20, 8]
	b	.L1151
	.p2align 3
.L1164:
	ldr	w0, [x2, 908]
	cmp	w0, wzr
	ccmp	x19, xzr, 4, gt
	beq	.L1154
	sub	w1, w1, #2
	mov	w0, 1
	and	w1, w1, -3
	uxtb	w1, w1
	cbnz	w1, .L1151
	ldr	x1, [x22, 26512]
	add	x3, x20, 53248
	mov	x0, x21
	strb	wzr, [x1, 1]
	ldr	x2, [x20]
	ldr	w1, [x2, 908]
	sub	w1, w1, #1
	str	w1, [x2, 908]
	ldr	w1, [x3, 800]
	bl	VC1_ClearCurPic
	str	xzr, [x20, 8]
	mov	w0, 0
	b	.L1151
	.p2align 3
.L1161:
	mov	w3, 1
	strb	w3, [x19, 1]
	add	x2, x22, 2520
	strb	w3, [x22, 2528]
	ldrsb	w1, [x22, 2525]
	add	x4, x2, 36864
	strb	w1, [x22, 2524]
	mov	w0, 1
	ldr	w2, [x4, 1672]
	sub	w2, w2, #1
	str	w2, [x19, 14504]
	b	.L1146
	.p2align 3
.L1160:
	ldrb	w1, [x22, 2524]
	strb	w1, [x22, 2525]
	b	.L1144
	.p2align 3
.L1163:
	cbz	x19, .L1151
	sub	w1, w1, #2
	and	w1, w1, -3
	uxtb	w1, w1
	cbnz	w1, .L1151
	ldr	x2, [x22, 26512]
	add	x1, x20, 53248
	mov	x0, x21
	strb	wzr, [x2, 1]
	ldr	w1, [x1, 800]
	bl	VC1_ClearCurPic
	str	xzr, [x20, 8]
	mov	w0, 0
	b	.L1151
	.p2align 3
.L1159:
	mov	x0, x21
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
	b	.L1143
	.size	VC1_Vfmw_StartDecode, .-VC1_Vfmw_StartDecode
	.align	3
.LC145:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseAdvHdr
	.type	VC1_Vfmw_ParseAdvHdr, %function
VC1_Vfmw_ParseAdvHdr:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	w24, w2
	mov	x25, x1
	bl	_mcount
	add	x22, x20, 36864
	ldr	x21, [x20, 3136]
	mov	w1, 32
	strb	wzr, [x20, 6]
	add	x23, x22, 1592
	mov	x0, x21
	bl	BsGet
	ldr	x19, [x22, 1592]
	ldr	w3, [x21, 32]
	uxtb	w26, w0
	ldr	w2, [x23, 28]
	mov	x0, x21
	lsr	w3, w3, 3
	add	x19, x19, x3
	sub	w2, w2, w3
	mov	x1, x19
	bl	BsInit
	sub	w0, w26, #10
	cmp	w0, 5
	bhi	.L1166
	cmp	w26, 11
	beq	.L1233
	ldrb	w1, [x20]
	and	w2, w1, 7
	cmp	w2, 7
	beq	.L1234
	add	x27, x20, 20480
	strb	wzr, [x20, 8]
	ldr	x2, .LC153
	str	wzr, [x27, 3628]
	ldrh	w0, [x2,w0,uxtw #1]
	adr	x2, .Lrtx1182
	add	x0, x2, w0, sxth #2
	br	x0
.Lrtx1182:
	.section	.rodata
	.align	0
	.align	2
.L1182:
	.2byte	(.L1181 - .Lrtx1182) / 4
	.2byte	(.L1198 - .Lrtx1182) / 4
	.2byte	(.L1183 - .Lrtx1182) / 4
	.2byte	(.L1200 - .Lrtx1182) / 4
	.2byte	(.L1185 - .Lrtx1182) / 4
	.2byte	(.L1186 - .Lrtx1182) / 4
	.text
	.p2align 3
.L1166:
	sub	w0, w26, #27
	cmp	w0, 4
	bls	.L1199
	cmp	w26, 254
	beq	.L1235
.L1198:
	mov	w0, w24
	mov	x1, x23
	bl	VC1_ClearCurPacket
	mov	w0, 0
.L1171:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1233:
	ldrb	w0, [x20]
	and	w0, w0, 7
	cmp	w0, 7
	bne	.L1198
	mov	x0, x21
	mov	x2, x25
	mov	x1, x20
	bl	VC1_Vfmw_ParseAdvSlcHdr
	cmp	w0, 1
	mov	w21, w0
	bne	.L1169
	add	x27, x20, 20480
	ldr	w0, [x27, 3628]
	cmp	w0, 255
	bls	.L1170
	mov	w1, w24
	mov	x0, x20
	bl	VC1_ClearCurPic
	ldr	x1, .LC147
	mov	w0, w21
	ldr	w2, [x27, 3628]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1171
	.p2align 3
.L1235:
	mov	w1, 32
	mov	x0, x21
	bl	BsGet
	mov	w19, w0
	mov	w1, 32
	mov	x0, x21
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w19, w2
	ccmp	w0, w1, 0, eq
	bne	.L1198
	ldrb	w0, [x20]
	mov	w1, 1
	strb	w1, [x20, 7]
	and	w1, w0, 7
	cmp	w1, 7
	bne	.L1199
	orr	w0, w0, 16
	strb	w0, [x20]
.L1199:
	mov	x1, x23
	mov	w0, w24
	bl	VC1_ClearCurPacket
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1200:
	ldr	x26, .LC154
.L1228:
	ldr	x3, [x26, 96]
	mov	x2, 176
	mov	w1, 0
	add	x0, x25, 360
	ldrb	w28, [x25, 391]
	blr	x3
	strb	w28, [x25, 391]
.L1187:
	ldr	x3, [x26, 96]
	add	x28, x27, 3632
	mov	x2, 14336
	mov	w1, 0
	mov	x0, x28
	blr	x3
	mov	x0, x21
	bl	BsPos
	str	w0, [x25, 500]
	mov	x2, x25
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvPicHdr
	cmp	w0, 1
	mov	w26, w0
	beq	.L1236
	mov	x1, x23
	mov	w0, w24
	bl	VC1_ClearCurPacket
	ldr	x1, .LC156
	mov	w2, 6128
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w26
	b	.L1171
	.p2align 3
.L1234:
	mov	w0, 1
	orr	w1, w1, 16
	strb	w0, [x20, 6]
	mov	w0, 1
	strb	w1, [x20]
	b	.L1171
	.p2align 3
.L1181:
	orr	w0, w1, 32
	mov	x1, x23
	strb	w0, [x20]
	mov	w0, w24
	bl	VC1_ClearCurPacket
	mov	w0, 1
	b	.L1171
	.p2align 3
.L1185:
	mov	x2, x25
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvEntptHdr
	cmp	w0, 1
	mov	w19, w0
	beq	.L1237
.L1196:
	mov	w0, w24
	mov	x1, x23
	bl	VC1_ClearCurPacket
	mov	w0, w19
	b	.L1171
	.p2align 3
.L1186:
	mov	x2, x25
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvSeqHdr
	cmp	w0, 1
	mov	w19, w0
	bne	.L1196
	ldrb	w0, [x20]
	orr	w0, w0, 1
	strb	w0, [x20]
	b	.L1196
	.p2align 3
.L1183:
	ldr	x26, .LC154
	mov	w0, 1
	strb	w0, [x20, 8]
	mov	x2, 16
	mov	w1, 0
	add	x0, x25, 520
	ldr	x3, [x26, 96]
	blr	x3
	ldrb	w0, [x20, 8]
	cbnz	w0, .L1187
	b	.L1228
	.p2align 3
.L1169:
	mov	x1, x23
	mov	w0, w24
	bl	VC1_ClearCurPacket
	ldr	x1, .LC152
	mov	w2, 6039
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w21
	b	.L1171
	.p2align 3
.L1236:
	mov	x0, x21
	add	x22, x22, 1592
	bl	BsPos
	ldr	w1, [x25, 500]
	sub	w0, w0, w1
	str	w0, [x25, 500]
	mov	x0, x21
	ldrb	w1, [x20]
	orr	w1, w1, 4
	strb	w1, [x20]
	bl	BsPos
	add	w0, w0, 32
	ldr	w1, [x22, 28]
	cmp	w0, w1, lsl 3
	bge	.L1189
	ldr	w3, [x27, 3628]
	add	x23, x25, 536
	ldr	x2, [x20, 3136]
	uxtw	x0, w3
	lsl	x1, x0, 3
	sub	x1, x1, x0
	lsl	x1, x1, 3
	add	x21, x28, x1
	strb	w3, [x21, 16]
	lsl	x3, x0, 2
	sub	x0, x3, x0
	add	x23, x23, x0, lsl 2
	ldr	w0, [x2, 32]
	ubfx	x0, x0, 3, 29
	add	x0, x19, x0
	str	x0, [x28, x1]
	ldr	w1, [x2, 32]
	and	w1, w1, 7
	str	w1, [x21, 24]
	ldr	w1, [x2, 24]
	ldr	w2, [x2, 32]
	lsl	w1, w1, 3
	sub	w1, w1, w2
	str	w1, [x21, 28]
	bl	MEM_Vir2Phy
	str	w0, [x21, 20]
	ldr	w0, [x22, 36]
	str	w0, [x21, 48]
	ldr	x1, [x22, 8]
	cbz	x1, .L1190
	ldr	w0, [x22, 24]
	cbz	w0, .L1190
	str	x1, [x21, 8]
	str	wzr, [x21, 40]
	ldr	w0, [x22, 32]
	lsl	w0, w0, 3
	str	w0, [x21, 44]
	ldr	w0, [x22, 24]
	str	w0, [x21, 36]
	ldr	w0, [x22, 40]
	str	w0, [x21, 52]
.L1191:
	ldr	w4, [x27, 3544]
	ldr	w0, [x27, 3548]
	ldrb	w2, [x20, 8]
	mul	w4, w0, w4
	sub	w4, w4, #1
	cbz	w2, .L1192
	add	w0, w0, 1
	ldr	w3, [x23, 4]
	add	w0, w0, w0, lsr 31
	sub	w3, w3, w0, asr 1
	str	w3, [x21, 32]
	cmp	w3, wzr
	ccmp	w4, w3, 2, gt
	bcc	.L1238
.L1175:
	ldr	w0, [x21, 28]
	cmp	w0, wzr
	ble	.L1178
	cbz	x1, .L1194
	ldr	w0, [x21, 44]
	cmp	w0, wzr
	ble	.L1178
.L1194:
	ldr	w1, [x27, 3628]
	mov	w0, 1
	add	w1, w1, w0
	str	w1, [x27, 3628]
	b	.L1171
	.p2align 3
.L1189:
	mov	x1, x23
	mov	w0, w24
	bl	VC1_ClearCurPacket
	ldr	x1, .LC156
	mov	w2, 6122
	mov	w0, w26
	bl	dprint_vfmw
	mov	w0, w26
	b	.L1171
	.p2align 3
.L1170:
	uxtw	x4, w0
	add	x3, x27, 3632
	lsl	x1, x4, 3
	ldr	x2, [x20, 3136]
	sub	x1, x1, x4
	add	x22, x25, 536
	lsl	x1, x1, 3
	add	x21, x3, x1
	strb	w0, [x21, 16]
	lsl	x0, x4, 2
	sub	x4, x0, x4
	ldr	w0, [x2, 32]
	add	x22, x22, x4, lsl 2
	ubfx	x0, x0, 3, 29
	add	x0, x19, x0
	str	x0, [x3, x1]
	ldr	w1, [x2, 32]
	and	w1, w1, 7
	str	w1, [x21, 24]
	ldr	w1, [x2, 24]
	ldr	w2, [x2, 32]
	lsl	w1, w1, 3
	sub	w1, w1, w2
	str	w1, [x21, 28]
	bl	MEM_Vir2Phy
	str	w0, [x21, 20]
	ldr	w0, [x23, 36]
	str	w0, [x21, 48]
	ldr	x1, [x23, 8]
	cbz	x1, .L1172
	ldr	w0, [x23, 24]
	cbz	w0, .L1172
	str	x1, [x21, 8]
	str	wzr, [x21, 40]
	ldr	w0, [x23, 32]
	lsl	w0, w0, 3
	str	w0, [x21, 44]
	ldr	w0, [x23, 24]
	str	w0, [x21, 36]
	ldr	w0, [x23, 40]
	str	w0, [x21, 52]
.L1173:
	ldr	w4, [x27, 3544]
	ldr	w0, [x27, 3548]
	ldrb	w2, [x20, 8]
	mul	w4, w0, w4
	sub	w4, w4, #1
	cbz	w2, .L1174
	add	w0, w0, 1
	ldr	w3, [x22, 4]
	add	w0, w0, w0, lsr 31
	sub	w3, w3, w0, asr 1
	str	w3, [x21, 32]
	cmp	w3, wzr
	ccmp	w4, w3, 2, gt
	bcs	.L1175
	ldr	x1, .LC148
	mov	w0, 1
	ldr	w2, [x22, 4]
	ldr	w4, [x27, 3548]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1171
	.p2align 3
.L1190:
	mov	x1, 0
	str	xzr, [x21, 8]
	stp	wzr, wzr, [x21, 36]
	str	wzr, [x21, 44]
	str	wzr, [x21, 52]
	b	.L1191
.L1237:
	mov	x1, x25
	mov	x0, x20
	bl	VC1_Vfmw_UpdateAdvSPS
	ldrb	w0, [x20]
	orr	w0, w0, 2
	strb	w0, [x20]
	b	.L1196
.L1192:
	ldr	w3, [x23, 4]
	str	w3, [x21, 32]
	cmp	w4, w3
	bcs	.L1175
	ldr	x1, .LC149
	mov	w0, 1
	ldr	w2, [x23, 4]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1171
.L1172:
	mov	x1, 0
	str	xzr, [x21, 8]
	stp	wzr, wzr, [x21, 36]
	str	wzr, [x21, 44]
	str	wzr, [x21, 52]
	b	.L1173
.L1174:
	ldr	w3, [x22, 4]
	str	w3, [x21, 32]
	cmp	w4, w3
	bcs	.L1175
	ldr	x1, .LC149
	mov	w0, 1
	ldr	w2, [x22, 4]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1171
.L1178:
	ldr	x1, .LC150
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1171
.L1238:
	ldr	x1, .LC148
	mov	w0, 1
	ldr	w2, [x23, 4]
	ldr	w4, [x27, 3548]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1171
	.size	VC1_Vfmw_ParseAdvHdr, .-VC1_Vfmw_ParseAdvHdr
	.align	3
.LC147:
	.xword	.LC146
	.align	3
.LC148:
	.xword	.LC100
	.align	3
.LC149:
	.xword	.LC102
	.align	3
.LC150:
	.xword	.LC104
	.align	3
.LC152:
	.xword	.LC151
	.align	3
.LC153:
	.xword	.L1182
	.align	3
.LC154:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC156:
	.xword	.LC155
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_Vfmw_ParseHeader
	.type	VC1_Vfmw_ParseHeader, %function
VC1_Vfmw_ParseHeader:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	w21, w2
	bl	_mcount
	ldrsb	w0, [x19, 3104]
	cbnz	w0, .L1240
	ldrb	w3, [x19]
	mov	x2, x20
	ldr	x0, [x19, 3136]
	mov	x1, x19
	orr	w3, w3, 3
	strb	w3, [x19]
	bl	VC1_Vfmw_ParseSMPPicHdr
	cmp	w0, 1
	beq	.L1242
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1240:
	mov	w2, w21
	mov	x1, x20
	mov	x0, x19
	bl	VC1_Vfmw_ParseAdvHdr
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1242:
	ldrb	w2, [x19]
	mov	w1, 20
	orr	w1, w2, w1
	strb	w1, [x19]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VC1_Vfmw_ParseHeader, .-VC1_Vfmw_ParseHeader
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_SetCodeInfo
	.type	VC1_SetCodeInfo, %function
VC1_SetCodeInfo:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 12288
	mov	x21, x1
	bl	_mcount
	add	x2, x19, 2520
	add	x0, x2, 3144
	str	x0, [x19, 5656]
	ldrb	w1, [x2, 3106]
	cmp	w1, 8
	beq	.L1248
	strb	wzr, [x2, 12]
	strb	w1, [x2, 11]
.L1245:
	add	x20, x19, 2520
	ldr	w2, [x21, 28]
	ldrsb	w1, [x20, 3104]
	cbz	w1, .L1249
	ldr	x1, [x21]
	bl	BsInit
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1249:
	ldr	x1, [x21]
	bl	BsInit
	ldr	w1, [x19, 5632]
	add	x2, x20, 20480
	add	x3, x20, 3120
	add	x20, x20, 3184
	adds	w4, w1, 15
	add	w0, w1, 30
	csel	w0, w0, w4, mi
	asr	w0, w0, 4
	str	w0, [x2, 3544]
	ldr	w0, [x19, 5636]
	str	w0, [x2, 3564]
	str	w1, [x2, 3560]
	add	w1, w0, 30
	adds	w0, w0, 15
	csel	w0, w1, w0, mi
	asr	w0, w0, 4
	str	w0, [x2, 3548]
	ldp	x0, x1, [x3]
	stp	x0, x1, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1248:
	mov	w3, 1
	mov	w1, 6
	strb	w3, [x2, 12]
	strb	w1, [x2, 11]
	b	.L1245
	.size	VC1_SetCodeInfo, .-VC1_SetCodeInfo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_ReceivePacket
	.type	VC1_ReceivePacket, %function
VC1_ReceivePacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	add	x0, x20, 53248
	add	x1, x0, 16
	cbz	x19, .L1258
	ldr	w2, [x19, 16]
	str	w2, [x1, 20]
	ldr	x2, [x19]
	str	x2, [x0, 16]
	ldr	w2, [x19, 20]
	str	w2, [x1, 28]
	ldrb	w2, [x19, 10]
	strb	w2, [x1, 16]
	ldr	w2, [x19, 24]
	str	w2, [x1, 36]
	ldrb	w2, [x19, 10]
	cmp	w2, 1
	beq	.L1253
	ldr	w2, [x19, 104]
	str	w2, [x1, 24]
	ldr	x2, [x19, 88]
	str	x2, [x1, 8]
	ldr	w2, [x19, 108]
	str	w2, [x1, 32]
	ldrb	w2, [x19, 98]
	strb	w2, [x1, 17]
	ldr	w2, [x19, 112]
	str	w2, [x1, 40]
	ldrb	w2, [x19, 98]
	cmp	w2, 1
	bne	.L1259
.L1254:
	mov	x0, x20
	bl	VC1_SetCodeInfo
	mov	w0, 1
.L1252:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1259:
	ldr	w0, [x0, 800]
	bl	VC1_ClearCurPacket
	ldrb	w2, [x19, 98]
	mov	w0, 1
	ldr	x1, .LC160
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1253:
	strb	wzr, [x1, 17]
	str	xzr, [x1, 8]
	str	wzr, [x1, 24]
	str	wzr, [x1, 32]
	str	wzr, [x1, 40]
	b	.L1254
	.p2align 3
.L1258:
	ldr	x1, .LC158
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1252
	.size	VC1_ReceivePacket, .-VC1_ReceivePacket
	.align	3
.LC158:
	.xword	.LC157
	.align	3
.LC160:
	.xword	.LC159
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1DEC_Decode
	.type	VC1DEC_Decode, %function
VC1DEC_Decode:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x1
	add	x20, x19, 12288
	bl	_mcount
	add	x21, x20, 2344
	ldr	x3, .LC161
	mov	x2, 176
	mov	x1, x22
	mov	x0, x21
	ldr	x3, [x3, 104]
	blr	x3
	mov	x1, x21
	mov	x0, x19
	bl	VC1_ReceivePacket
	mov	w21, w0
	cbz	w0, .L1287
	ldr	x0, [x19]
	add	x22, x19, 53248
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L1288
.L1263:
	ldrb	w0, [x20, 2520]
	strb	w0, [x22, 144]
.L1264:
	add	x20, x20, 2520
	add	x1, x19, 16384
	ldr	w2, [x22, 800]
	add	x1, x1, 1608
	mov	x0, x20
	bl	VC1_Vfmw_ParseHeader
	mov	w23, w0
	cbnz	w0, .L1265
	ldr	w0, [x22, 800]
	add	x1, x22, 16
	bl	VC1_ClearCurPacket
	ldr	x0, .LC164
	ldr	x4, [x0]
	cbz	x4, .L1267
	ldr	w0, [x22, 800]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	mov	w21, 0
	blr	x4
.L1284:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1265:
	mov	x0, x19
	bl	VC1_Vfmw_DecodeArbiter
	cbnz	w0, .L1289
.L1267:
	mov	w21, 0
.L1291:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1288:
	ldr	w0, [x0, 912]
	cmp	w0, 1
	bne	.L1263
	ldrb	w0, [x22, 144]
	strb	w0, [x20, 2520]
	b	.L1264
	.p2align 3
.L1287:
	add	x19, x19, 53248
	ldr	x1, .LC163
	ldr	w2, [x19, 8]
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1289:
	mov	x0, x19
	bl	VC1_Vfmw_GetStreamInfo
	mov	w21, w0
	cbz	w0, .L1290
	mov	x0, x19
	bl	VC1_Vfmw_StartDecode
	mov	w21, w0
	cbz	w0, .L1271
	mov	w21, w23
	b	.L1284
	.p2align 3
.L1290:
	ldr	x1, .LC165
	bl	dprint_vfmw
	ldr	x0, [x19]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	bne	.L1269
	ldr	w0, [x0, 912]
	cmp	w0, 1
	beq	.L1270
.L1269:
	ldr	w1, [x22, 800]
	mov	x0, x20
	bl	VC1_ClearCurPic
.L1270:
	ldr	x0, .LC164
	ldr	x4, [x0]
	cbz	x4, .L1267
.L1285:
	ldr	w0, [x22, 800]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L1284
	.p2align 3
.L1271:
	add	x19, x19, 36864
	ldr	w0, [x22, 800]
	mov	w2, 1
	ldr	w1, [x19, 2044]
	bl	FSP_ClearLogicFs
	ldr	x1, [x19, 1936]
	ldr	x0, .LC164
	strb	wzr, [x1]
	ldr	x1, [x19, 1936]
	strb	wzr, [x1, 1]
	ldr	x4, [x0]
	cbnz	x4, .L1285
	mov	w21, 0
	b	.L1291
	.size	VC1DEC_Decode, .-VC1DEC_Decode
	.align	3
.LC161:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC163:
	.xword	.LC162
	.align	3
.LC164:
	.xword	g_event_report
	.align	3
.LC165:
	.xword	.LC71
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VC1_GetPacketState
	.type	VC1_GetPacketState, %function
VC1_GetPacketState:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 12288
	bl	_mcount
	mov	w0, 2
	ldrsb	w1, [x19, 2526]
	ldr	x19, [sp, 16]
	cmp	w1, 1
	csel	w0, w0, wzr, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_GetPacketState, .-VC1_GetPacketState
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.11686, %object
	.size	__func__.11686, 13
__func__.11686:
	.string	"Vc1SMPSeqHdr"
	.zero	3
.LC0:
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	1
	.word	1
	.word	2
	.word	1
	.word	2
	.word	2
	.word	3
	.word	3
	.word	3
.LC1:
	.word	3
	.word	1
	.word	2
	.word	0
	.word	4
	.zero	4
.LC2:
	.word	1
	.word	0
	.word	2
	.word	3
	.word	4
	.zero	4
.LC3:
	.word	1
	.word	1
	.word	2
	.word	1
	.word	3
	.word	1
	.word	2
	.zero	4
.LC4:
	.word	2
	.word	3
	.word	3
	.word	4
	.word	4
	.word	5
	.word	5
	.zero	4
.LC5:
	.word	3
	.word	4
	.word	1
	.word	5
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	1
	.word	3
	.word	5
	.word	7
.LC6:
	.word	5
	.word	5
	.word	6
	.word	6
	.word	7
	.word	7
	.word	7
	.word	7
	.word	7
	.word	7
	.word	8
	.word	8
	.word	8
	.word	8
.LC7:
	.word	256
	.word	128
	.word	85
	.word	64
	.word	51
	.word	43
	.word	37
	.word	32
	.type	__func__.11997, %object
	.size	__func__.11997, 17
__func__.11997:
	.string	"VC1_DecBfraction"
	.zero	7
	.type	aCount, %object
	.size	aCount, 128
aCount:
	.word	0
	.word	1
	.word	1
	.word	2
	.word	1
	.word	2
	.word	2
	.word	3
	.word	1
	.word	2
	.word	2
	.word	3
	.word	2
	.word	3
	.word	3
	.word	4
	.word	1
	.word	2
	.word	2
	.word	3
	.word	2
	.word	3
	.word	3
	.word	4
	.word	2
	.word	3
	.word	3
	.word	4
	.word	3
	.word	4
	.word	4
	.word	5
	.type	a2On4Off, %object
	.size	a2On4Off, 60
a2On4Off:
	.word	3
	.word	5
	.word	6
	.word	9
	.word	10
	.word	12
	.word	17
	.word	18
	.word	20
	.word	24
	.word	33
	.word	34
	.word	36
	.word	40
	.word	48
	.zero	4
.LC8:
	.word	1
	.word	2
	.word	2
	.word	2
	.word	0
	.word	1
	.word	0
	.word	1
.LC9:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	7
	.word	8
	.word	6
	.word	7
	.word	8
	.word	9
	.word	10
	.word	11
	.word	12
	.word	13
	.word	14
	.word	15
	.word	16
	.word	17
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	25
	.word	27
	.word	29
	.word	31
	.align	3
.LANCHOR1 = . + 0
	.type	FldPTypeTab, %object
	.size	FldPTypeTab, 64
FldPTypeTab:
	.word	0
	.word	0
	.word	0
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	2
	.word	2
	.word	2
	.word	4
	.word	4
	.word	2
	.word	4
	.word	4
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC13:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC15:
	ASCII(.string	"VC1 INIT!!!\n" )
	.zero	3
.LC17:
	ASCII(.string	"picture width out of range" )
	.zero	5
.LC20:
	ASCII(.string	"%s: %s\n" )
.LC22:
	ASCII(.string	"picture height out of range" )
	.zero	4
.LC24:
	ASCII(.string	"Vc1SMPSeqHdr Err, BS is wrong" )
	.zero	2
.LC26:
	ASCII(.string	"Vc1SMPSeqHdr Err, BS is wrong\n" )
	.zero	1
.LC30:
	ASCII(.string	"image size abnormal(%dx%d)\n" )
	.zero	4
.LC32:
	ASCII(.string	"vc1 actual frame size(%dx%d) exeed max config(%dx%d)\n" )
	.zero	2
.LC35:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC37:
	ASCII(.string	"ERROR: FSP_ConfigInstance fail!\n" )
	.zero	7
.LC39:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC43:
	ASCII(.string	"get frame store fail!\n" )
	.zero	1
.LC45:
	ASCII(.string	"VC1_Vfmw_GetImageBuffer get imgbuf err, clear all ref!\n" )
.LC47:
	ASCII(.string	"VC1_Vfmw_GetImageBuffer get_flag!=1, FSP_ClearLogicFs.\n" )
.LC52:
	ASCII(.string	"VC1_Vfmw_GetStreamInfo VC1_ArrangeVahbMem err!\n" )
.LC54:
	ASCII(.string	"VC1_Vfmw_GetStreamInfo VC1_Vfmw_GetImageBuffer err!\n" )
	.zero	3
.LC56:
	ASCII(.string	"dec_pts: %lld Usertag = %lld\n" )
	.zero	2
.LC59:
	ASCII(.string	"---------- unref %d\n" )
	.zero	3
.LC61:
	ASCII(.string	"VC1_Vfmw_GetStreamInfo B pic pCodecInf->RefNum < 2 err!\n" )
	.zero	7
.LC63:
	ASCII(.string	"line: %d (pCur,pFwd,pBwd)(%p,%p,%p)\n" )
	.zero	3
.LC65:
	ASCII(.string	"VC1_Vfmw_GetStreamInfo imgbuf = NULL err!\n" )
	.zero	5
.LC67:
	ASCII(.string	"CurFsID(fwd%d,%d,bwd%d) abnormal!(%p,%p,%p)\n" )
	.zero	3
.LC69:
	ASCII(.string	"VC1_Vfmw_GetStreamInfo VC1_Vfmw_BuildRefList err!\n" )
	.zero	5
.LC71:
	ASCII(.string	"VC1_Vfmw_GetStreamInfo error! Release all packets!\n" )
	.zero	4
.LC84:
	ASCII(.string	"%s:%d Code(%d) is invalid\n" )
	.zero	5
.LC86:
	ASCII(.string	"%s:%d Denominator(%d) is invalid\n" )
	.zero	6
.LC100:
	ASCII(.string	"SLICE_ADDR = %d,SlcEndMbn = %d, MbHeight = %d\n" )
	.zero	1
.LC102:
	ASCII(.string	"SLICE_ADDR = %d,SlcEndMbn = %d, totalMbMin1 = %d\n" )
	.zero	6
.LC104:
	ASCII(.string	"VC1_RecordSlcStream stream len <= 0\n" )
	.zero	3
.LC107:
	ASCII(.string	"Check Pos 2: %d\n" )
	.zero	7
.LC109:
	ASCII(.string	"Check Pos 3: %d\n" )
	.zero	7
.LC112:
	ASCII(.string	"\n======PicNum  :%6d=========\n" )
	.zero	2
.LC114:
	ASCII(.string	"INTERPFRM  :%#x\n" )
	.zero	7
.LC117:
	ASCII(.string	"VC1_Vfmw_ParseMPBPicHdr err!\n" )
	.zero	2
.LC119:
	ASCII(.string	"VC1_Vfmw_ParseSMPIPicHdr err!\n" )
	.zero	1
.LC121:
	ASCII(.string	"After VC1_Vfmw_ParseSMPPicHdr BS Len <= 0 err!\n" )
.LC127:
	ASCII(.string	"pImg is null, pCurImg %p pBwdImg %p pFwdImg %p\n" )
.LC129:
	ASCII(.string	"line: %d, pToQueImg is Null!\n" )
	.zero	2
.LC131:
	ASCII(.string	"insert buf %d ref %d\n" )
	.zero	2
.LC133:
	ASCII(.string	"err(%d) > out_thr(%d)\n" )
	.zero	1
.LC135:
	ASCII(.string	"----------------- VC1 Imode -> %d ------------------\n" )
	.zero	2
.LC138:
	ASCII(.string	"NOTE: Drop non-I\n" )
	.zero	6
.LC140:
	ASCII(.string	"NOTE: Drop B\n" )
	.zero	2
.LC146:
	ASCII(.string	" pCodecInf->SlcNum = %d >= 256 \n" )
	.zero	7
.LC151:
	ASCII(.string	"line %d, Code = VC1_SCSLC failed!\n" )
	.zero	5
.LC155:
	ASCII(.string	"line %d, Code = VC1_SCFRM failed!\n" )
	.zero	5
.LC157:
	ASCII(.string	"bitsream para error -----\n" )
	.zero	5
.LC159:
	ASCII(.string	"line %d, pPacket->StreamPack[1].IsLastSeg = %d\n" )
.LC162:
	ASCII(.string	"ERROR: frm %d, Invalid packet pointer!\n" )
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
