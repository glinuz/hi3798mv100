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
	mov	x2, 54040
	mov	w1, 0
	mov	x0, x20
	ldr	w22, [x19, 776]
	ldr	x3, [x3, 96]
	blr	x3
	str	w22, [x19, 776]
	add	x0, x19, 176
	bl	ResetVoQueue
	add	x4, x20, 12288
	ldr	w1, [x21, 32]
	str	x21, [x20]
	mov	w5, 1
	add	w1, w1, 255
	mov	x0, x20
	strb	w5, [x4, 2513]
	and	w1, w1, -256
	add	w3, w1, 4096
	add	w2, w1, 8192
	ldr	w6, [x21, 8]
	str	w6, [x19, 124]
	add	w6, w1, 2048
	str	w1, [x19, 96]
	add	w1, w1, 12288
	str	w3, [x19, 104]
	add	w3, w3, 2048
	str	w2, [x19, 112]
	add	w2, w2, 2048
	str	w6, [x19, 100]
	str	w3, [x19, 108]
	stp	w2, w1, [x19, 116]
	strb	w5, [x4, 2514]
	stp	wzr, wzr, [x19, 56]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x19, 784]
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
	add	x22, x20, 2504
	add	x1, x22, 20480
	add	x0, x21, 53248
	ldr	w1, [x1, 3620]
	ldr	w0, [x0, 784]
	bl	FSP_GetFsImagePtr
	mov	x19, x0
	cbz	x0, .L50
	mov	w0, 1
	str	w0, [x19, 376]
	ldrsb	w0, [x20, 2507]
	cmp	w0, 3
	beq	.L61
	strb	wzr, [x19, 364]
	mov	w2, 7168
.L54:
	str	w2, [x19, 108]
	mov	w3, 262144
	add	x20, x20, 2504
	mov	w1, 0
	ldr	x4, [x21]
	add	x20, x20, 20480
	mov	x0, x19
	ldr	w4, [x4, 592]
	cmp	w4, 1
	csel	w3, wzr, w3, ne
	orr	w2, w3, w2
	str	w2, [x19, 108]
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
.L61:
	mov	w0, 4
	strb	w0, [x19, 364]
	ldr	w0, [x19, 108]
	mov	w2, 4352
	ldrb	w3, [x21, 24]
	ubfx	x1, x0, 10, 2
	add	w0, w3, 1
	orr	w0, w0, w1
	cmp	w0, 3
	bhi	.L54
	lsl	w0, w0, 10
	and	w0, w0, -131073
	orr	w2, w0, w2
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
	cbz	x19, .L65
	add	x19, x19, 53248
	mov	w1, w20
	mov	w2, 0
	ldr	w0, [x19, 784]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 784]
	mov	w1, w20
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L66
	ldr	w0, [x19, 784]
	ldr	x1, [x20, 560]
	bl	FreeUsdByDec
	str	xzr, [x20, 560]
	ldr	x1, [x20, 568]
	ldr	w0, [x19, 784]
	bl	FreeUsdByDec
	str	xzr, [x20, 568]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L65:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L66:
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
	add	x19, x19, 2504
	bl	_mcount
	ldr	x0, [x19, 24000]
	cbz	x0, .L74
	ldr	x0, [x20]
	add	x21, x20, 53248
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L69
	add	x23, x19, 20480
	add	x21, x20, 53248
	ldr	w1, [x23, 3616]
	ldr	w0, [x21, 784]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L69
	ldr	w0, [x21, 784]
	ldr	w1, [x23, 3616]
	bl	FSP_GetDisplay
	cmp	w0, 2
	beq	.L83
.L69:
	add	x19, x21, 176
.L73:
	mov	x0, x19
	bl	GetVoLastImageID
.L80:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L83:
	mov	x0, x20
	bl	VC1_SetImgFormat
	str	xzr, [x22, 560]
	mov	w2, 1
	str	xzr, [x22, 568]
	str	w2, [x22, 188]
	ldr	w1, [x22, 156]
	ldr	w0, [x21, 784]
	bl	FSP_SetDisplay
	ldr	x0, [x19, 24000]
	ldrb	w1, [x0]
	cmp	w1, 1
	beq	.L84
.L70:
	add	x19, x21, 176
	ldr	w0, [x21, 784]
	mov	x4, x22
	mov	x3, x19
	mov	x2, x20
	mov	w1, 1
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L71
	ldr	w1, [x22, 156]
	mov	x0, x20
	bl	VC1DEC_RecycleImage
	ldr	w0, [x21, 784]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	str	xzr, [x22, 560]
	ldr	x1, [x22, 568]
	ldr	w0, [x21, 784]
	bl	FreeUsdByDec
	str	xzr, [x22, 568]
	b	.L73
	.p2align 3
.L71:
	ldr	w0, [x22, 160]
	cmp	w0, 100
	bhi	.L69
	ldr	w0, [x22, 112]
	cmp	w0, 31
	bls	.L69
	ldr	w1, [x22, 116]
	mov	w0, 0
	cmp	w1, 31
	bhi	.L80
	b	.L73
	.p2align 3
.L84:
	strb	w1, [x0, 1]
	mov	w0, 3
	ldr	x1, [x19, 24000]
	strb	wzr, [x1]
	strb	w0, [x19, 1]
	b	.L70
.L74:
	mov	w0, -1
	b	.L80
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
	bhi	.L86
	sub	w1, w23, #32
	cmp	w1, w0
	bhi	.L86
	add	x0, x22, 28672
	mov	w1, 0
	add	x0, x0, 128
	.p2align 2
.L89:
	str	w1, [x0, 1196]
	add	w1, w1, 1
	strb	wzr, [x0, 1]
	cmp	w1, 5
	strb	wzr, [x0]
	add	x0, x0, 1664
	bne	.L89
	ldr	x0, [x22]
	ldr	w19, [x0, 708]
	ldr	w2, [x0, 28]
	cmp	w2, 25
	cbnz	w19, .L90
	beq	.L116
.L110:
	mov	w19, w1
	mov	w24, 3
	mov	w20, 2
.L91:
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
	beq	.L101
	strb	w19, [x29, 85]
	sxtb	w19, w20
	strb	w24, [x29, 83]
.L106:
	add	x20, x22, 53248
	add	x1, x29, 80
	strb	w19, [x29, 86]
	strb	wzr, [x29, 82]
	ldr	w0, [x20, 784]
	bl	FSP_ConfigInstance
	mov	w19, w0
	cbnz	w0, .L102
	ldr	x1, [x22]
	add	x3, x29, 76
	ldr	w0, [x20, 784]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	beq	.L117
	cbnz	w0, .L104
	mov	w19, 1
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L86:
	ldr	x1, .LC31
	mov	w3, w23
	mov	w2, w21
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
.L112:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L90:
	bne	.L110
	ldr	w4, [x0, 668]
	ldr	w5, [x0, 672]
	cmp	w21, w4
	bhi	.L98
	cmp	w23, w5
	bhi	.L98
	ldr	w2, [x0, 696]
	mov	w20, 1
	ldr	w1, [x0, 692]
	add	w24, w1, 2
	cbnz	w2, .L118
	ldrsb	w19, [x0, 716]
	b	.L91
	.p2align 3
.L102:
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
.L116:
	ldr	w4, [x0, 668]
	cmp	w21, w4
	bhi	.L119
	ldr	w1, [x0, 672]
	cmp	w23, w1
	mov	w5, w1
	bhi	.L93
	add	x2, x22, 12288
	mov	w19, 1
	ldrb	w2, [x2, 2504]
	tbnz	x2, 3, .L112
	ldr	w3, [x0, 696]
	mov	w20, w19
	ldr	w2, [x0, 692]
	add	w24, w2, 2
	cbz	w3, .L96
	add	w2, w2, w19
	mov	w19, 17
	cmp	w2, w19
	csel	w20, w2, w19, ls
.L96:
	ldrsb	w19, [x0, 716]
	mov	w23, w1
	mov	w21, w4
	b	.L91
	.p2align 3
.L101:
	mov	w0, 1
	strb	w19, [x29, 85]
	strb	w0, [x29, 83]
	mov	w19, 1
	b	.L106
	.p2align 3
.L98:
	ldr	x1, .LC33
	mov	w3, w23
	mov	w2, w21
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	b	.L112
	.p2align 3
.L117:
	add	x0, x20, 176
	bl	ResetVoQueue
.L104:
	ldr	x1, .LC36
	mov	w0, 0
	bl	dprint_vfmw
	b	.L112
	.p2align 3
.L118:
	mov	w19, 17
	add	w1, w1, w20
	cmp	w1, w19
	csel	w20, w1, w19, ls
	ldrsb	w19, [x0, 716]
	b	.L91
.L119:
	ldr	w5, [x0, 672]
.L93:
	ldr	x1, .LC33
	mov	w3, w23
	mov	w2, w21
	mov	w0, 0
	bl	dprint_vfmw
	b	.L112
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
	add	x21, x20, 2504
	bl	_mcount
	add	x22, x22, 53248
	add	x1, x21, 20480
	ldr	w0, [x22, 784]
	ldr	w1, [x1, 3620]
	bl	FSP_GetLogicFs
	cbz	x0, .L145
	ldrsb	w1, [x21, 3104]
	cbz	w1, .L123
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
	beq	.L146
	sub	w1, w1, #1
	strh	w1, [x19, 84]
.L126:
	add	x3, x20, 2504
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
	ldrb	w2, [x20, 2512]
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
	ldr	w2, [x20, 13380]
	strb	w2, [x19, 16]
	ldrb	w2, [x1, 2406]
	cbnz	w2, .L127
	ldrb	w1, [x1, 2578]
	cmp	w1, 8
	cset	w1, ls
	strb	w1, [x19, 20]
.L128:
	add	x2, x20, 2504
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
	ldr	w3, [x20, 13436]
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
	ldr	x5, [x20, 26504]
	ldrb	w3, [x1, 2609]
	add	x3, x5, x3
	ldrb	w3, [x3, 1029]
	strb	w3, [x19, 30]
	ldrb	w3, [x1, 2609]
	ldr	x5, [x20, 26496]
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
	ldr	w3, [x20, 13412]
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
	ldr	w2, [x20, 13308]
	cmp	w2, 3
	beq	.L147
	ldrb	w2, [x1, 2409]
	strb	w2, [x0, 373]
	ldrb	w2, [x1, 2410]
	strb	w2, [x0, 374]
	ldrb	w2, [x1, 2411]
	strb	w2, [x0, 375]
	ldrb	w1, [x1, 2412]
	strb	w1, [x0, 376]
.L133:
	add	x1, x20, 2504
	add	x2, x1, 8192
	ldrb	w2, [x2, 2224]
	strb	w2, [x0, 378]
	ldrb	w0, [x20, 2515]
	strb	w0, [x19, 63]
	b	.L134
	.p2align 3
.L123:
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
	cbnz	w2, .L136
	ldrb	w1, [x1, 2462]
	cmp	w1, 8
	cset	w1, ls
	strb	w1, [x19, 20]
.L137:
	add	x2, x20, 2504
	add	x1, x2, 8192
	ldrb	w3, [x1, 2482]
	strb	w3, [x19, 24]
	ldrb	w3, [x1, 2481]
	strb	w3, [x19, 23]
	ldrb	w3, [x1, 2216]
	strb	w3, [x19, 22]
	ldrb	w3, [x1, 2480]
	strb	w3, [x19, 25]
	ldr	w3, [x20, 13188]
	strb	w3, [x19, 26]
	ldrb	w3, [x1, 2483]
	strb	w3, [x19, 21]
	ldrb	w3, [x1, 2467]
	strb	w3, [x19, 27]
	ldrb	w3, [x1, 2468]
	strb	w3, [x19, 28]
	ldr	x3, [x20, 26504]
	ldrb	w3, [x3, 1029]
	strb	w3, [x19, 30]
	ldr	x3, [x20, 26496]
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
	ldr	w3, [x20, 13248]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 42]
	ldr	w3, [x20, 13244]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 43]
	ldr	w3, [x20, 13252]
	cmp	w3, wzr
	cset	w3, eq
	strb	w3, [x19, 45]
	ldrb	w3, [x1, 2460]
	strb	w3, [x19, 50]
	ldrb	w3, [x1, 2472]
	strb	w3, [x19, 49]
	ldrb	w3, [x1, 2471]
	strb	w3, [x19, 48]
	ldr	w3, [x20, 13260]
	str	w3, [x19, 14424]
	ldrb	w3, [x1, 2220]
	cmp	w3, 1
	beq	.L148
	mov	w5, 0
	mov	w2, 0
	mov	w4, 0
	mov	w3, 0
.L140:
	add	x1, x20, 2504
	strb	w3, [x19, 59]
	strb	w4, [x19, 60]
	add	x1, x1, 8192
	strb	w2, [x19, 61]
	str	w5, [x0, 400]
	ldrb	w1, [x1, 2208]
	strb	w1, [x0, 378]
.L134:
	add	x2, x20, 2504
	add	x20, x20, 2517
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
	ldrsw	x0, [x22, 784]
	ldr	x0, [x5, x0, lsl 3]
	str	w4, [x0, 1556]
	ldrsw	x1, [x22, 784]
	ldrh	w0, [x19, 84]
	ldr	x1, [x5, x1, lsl 3]
	add	w0, w0, 1
	str	w0, [x1, 1560]
.L120:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L136:
	cmp	w2, 1
	beq	.L149
	cmp	w2, 2
	beq	.L150
	mov	w1, 1
	strb	w1, [x19, 20]
	b	.L137
	.p2align 3
.L127:
	cmp	w2, 1
	beq	.L151
	cmp	w2, 2
	beq	.L152
	mov	w1, 1
	strb	w1, [x19, 20]
	b	.L128
	.p2align 3
.L151:
	ldrb	w1, [x1, 2579]
	cmp	w1, wzr
	cset	w1, ne
	strb	w1, [x19, 20]
	b	.L128
	.p2align 3
.L149:
	ldrb	w1, [x1, 2464]
	cmp	w1, wzr
	cset	w1, ne
	strb	w1, [x19, 20]
	b	.L137
	.p2align 3
.L147:
	ldrb	w2, [x1, 2609]
	cbnz	w2, .L132
	ldrb	w2, [x1, 2409]
	strb	w2, [x0, 373]
	ldrb	w2, [x1, 2410]
	strb	w2, [x0, 374]
	ldrb	w2, [x1, 2411]
	strb	w2, [x0, 375]
	ldrb	w2, [x1, 2412]
	strb	w2, [x0, 376]
	ldrb	w2, [x1, 2609]
.L132:
	cmp	w2, 1
	bne	.L133
	add	x1, x20, 2504
	add	x1, x1, 8192
	ldrb	w2, [x1, 2409]
	strb	w2, [x0, 368]
	ldrb	w2, [x1, 2410]
	strb	w2, [x0, 369]
	ldrb	w2, [x1, 2411]
	strb	w2, [x0, 370]
	ldrb	w1, [x1, 2412]
	strb	w1, [x0, 371]
	b	.L133
	.p2align 3
.L146:
	add	w1, w1, 1
	add	w1, w1, w1, lsr 31
	asr	w1, w1, 1
	sub	w1, w1, #1
	strh	w1, [x19, 84]
	b	.L126
	.p2align 3
.L148:
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
	cbnz	w3, .L141
	mov	w5, w2
	mov	w3, w2
	b	.L140
	.p2align 3
.L141:
	ldrb	w3, [x1, 2458]
	mov	w5, w3
	b	.L140
	.p2align 3
.L150:
	strb	wzr, [x19, 20]
	b	.L137
	.p2align 3
.L152:
	strb	wzr, [x19, 20]
	b	.L128
	.p2align 3
.L145:
	ldr	x1, .LC40
	mov	w2, 1126
	mov	w0, 22
	bl	dprint_vfmw
	b	.L120
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
	cbz	w0, .L155
	ldr	w0, [x19, 10804]
	cmp	w0, 3
	beq	.L160
.L155:
	add	x0, x19, 36864
	ldr	w0, [x0, 1672]
	sub	w0, w0, #1
	str	w0, [x20, 14504]
.L156:
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
.L160:
	add	x0, x19, 36864
	ldr	w1, [x0, 1668]
	ldr	w2, [x0, 1664]
	add	w0, w1, 1
	add	w0, w0, w0, lsr 31
	asr	w0, w0, 1
	mul	w0, w0, w2
	sub	w0, w0, #1
	str	w0, [x20, 14504]
	b	.L156
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
	add	x0, x0, 2504
	ldrsb	w1, [x0, 3]
	cmp	w1, 3
	beq	.L173
.L163:
	add	x19, x19, 53248
	ldr	w0, [x19, 784]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L165
	ldr	w0, [x19, 784]
	mov	w20, 0
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	bne	.L164
	ldr	w0, [x19, 784]
	add	x1, x19, 176
	bl	FSP_ClearNotInVoQueue
	b	.L164
	.p2align 3
.L173:
	ldrb	w0, [x0, 9]
	cmp	w0, 1
	beq	.L163
.L165:
	mov	w20, 1
.L164:
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
	ldr	w0, [x22, 784]
	bl	FSP_NewLogicFs
	mov	w1, w0
	add	x0, x19, 2504
	add	x20, x0, 20480
	str	w1, [x20, 3620]
	tbnz	w1, #31, .L194
	add	x20, x21, 28672
	add	x0, x0, 12288
	add	x0, x0, 1720
	ldrh	w2, [x20, 128]
	cbz	w2, .L184
	add	x21, x21, 36864
	add	x0, x20, 1792
	add	x4, x21, 256
.L181:
	ldrh	w3, [x0]
	mov	x2, x0
	add	x0, x0, 1664
	cmp	x0, x4
	cbz	w3, .L178
	bne	.L181
	ldr	w0, [x22, 784]
	mov	w2, 1
	add	x21, x21, 257
	bl	FSP_ClearLogicFs
	ldr	x1, .LC48
	mov	w0, 1
	bl	dprint_vfmw
	add	x0, x20, 129
.L183:
	strb	wzr, [x0]
	strb	wzr, [x0, -1]
	add	x0, x0, 1664
	cmp	x0, x21
	bne	.L183
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L184:
	mov	x2, x0
.L178:
	str	x2, [x19, 26496]
	mov	w0, 1
	mov	w1, 0
	strb	w0, [x2, 1]
	.p2align 2
.L180:
	ldr	x4, [x19, 26496]
	sxtw	x0, w1
	uxtb	w2, w1
	add	w1, w1, 1
	add	x4, x4, x0
	cmp	w1, 256
	strb	w2, [x4, 3]
	ldr	x4, [x19, 26496]
	add	x4, x4, x0
	strb	w2, [x4, 259]
	ldr	x4, [x19, 26496]
	add	x4, x4, x0
	strb	w2, [x4, 515]
	ldr	x3, [x19, 26496]
	add	x0, x3, x0
	strb	w2, [x0, 771]
	bne	.L180
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L194:
	ldr	x1, .LC44
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 3616]
	ldr	w0, [x22, 784]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L176
	ldr	w0, [x22, 784]
	ldr	x1, [x21, 560]
	bl	FreeUsdByDec
	ldr	w0, [x22, 784]
	ldr	x1, [x21, 568]
	bl	FreeUsdByDec
	ldr	w0, [x22, 784]
	ldr	x1, [x21, 576]
	bl	FreeUsdByDec
	ldr	w0, [x22, 784]
	ldr	x1, [x21, 584]
	bl	FreeUsdByDec
	add	x0, x21, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w1, [x20, 3616]
	ldr	w0, [x22, 784]
	bl	FSP_ClearLogicFs
	ldr	w1, [x20, 3612]
	mov	w2, 1
	ldr	w0, [x22, 784]
	bl	FSP_ClearLogicFs
	str	wzr, [x20, 3612]
	str	wzr, [x20, 3616]
	ldr	w0, [x22, 784]
	bl	FSP_GetNullLogicFs
	str	w0, [x20, 3612]
	str	w0, [x20, 3616]
.L176:
	add	x0, x19, 2504
	add	x1, x22, 176
	add	x0, x0, 20480
	str	wzr, [x0, 3608]
	ldr	w0, [x22, 784]
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
	bne	.L197
	cmp	w19, 1
	beq	.L260
	cmp	w19, 2
	beq	.L261
.L206:
	str	wzr, [x22, 3580]
	str	wzr, [x22, 3596]
.L258:
	mov	w0, 1
.L243:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	add	sp, sp, 512
	ret
	.p2align 3
.L197:
	add	x26, x21, 8192
	ldrb	w0, [x26, 2572]
	cmp	w0, 1
	ldrb	w0, [x26, 2609]
	beq	.L262
	cbz	w0, .L263
.L217:
	ldr	x0, [x21, 24008]
	ldr	w27, [x0, 1196]
	mov	w23, w27
.L216:
	cmp	w19, 1
	beq	.L264
.L218:
	cmp	w19, 2
	bne	.L206
	mov	w0, 1
	strb	wzr, [x22, 3576]
	strb	w0, [x22, 3577]
	str	w19, [x22, 3580]
	str	w19, [x22, 3596]
	cbz	w24, .L232
	cbz	w25, .L233
	ldr	x0, [x21, 24008]
	ldr	x1, [x21, 23992]
	ldr	w7, [x0, 1196]
	str	w7, [x22, 3584]
	b	.L235
	.p2align 3
.L263:
	ldr	x0, [x21, 24000]
	cmp	w19, 1
	ldr	x1, [x21, 24008]
	ldr	w23, [x0, 1196]
	ldr	w27, [x1, 1196]
	bne	.L218
.L264:
	ldrb	w0, [x26, 2600]
	strb	w19, [x22, 3577]
	add	w0, w0, 1
	strb	wzr, [x22, 3576]
	str	w0, [x22, 3580]
	str	wzr, [x22, 3596]
	cbz	w24, .L219
	cbz	w25, .L220
	ldr	x0, [x21, 24008]
	ldr	x1, [x21, 23992]
	ldr	w19, [x0, 1196]
	str	w19, [x22, 3584]
	b	.L222
	.p2align 3
.L260:
	strb	wzr, [x22, 3576]
	str	w19, [x22, 3580]
	str	wzr, [x22, 3596]
	ldr	x0, [x21, 24008]
	ldr	w19, [x0, 1196]
	str	w19, [x22, 3584]
	ldrsb	w0, [x21, 3104]
	cbz	w0, .L199
	ldr	w1, [x21, 10804]
	add	x0, x21, 8192
	cmp	w1, 2
	ldrb	w20, [x0, 2584]
	ldrb	w22, [x0, 2585]
	ldrb	w0, [x0, 2582]
	beq	.L265
.L200:
	cmp	w0, 4
	beq	.L201
.L204:
	mov	w2, 1664
	add	x1, x21, 13
	smull	x2, w19, w2
	add	x2, x2, 12288
	add	x0, x2, 1723
	add	x2, x2, 1979
	add	x0, x21, x0
	add	x2, x21, x2
	.p2align 2
.L202:
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
	bne	.L202
	b	.L258
	.p2align 3
.L262:
	cbz	w0, .L217
	ldr	x1, [x21, 24000]
	ldr	x0, [x21, 24008]
	ldr	w27, [x1, 1196]
	ldr	w23, [x0, 1196]
	b	.L216
.L261:
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
	cbz	w0, .L207
	add	x0, x21, 8192
	ldrb	w22, [x0, 2584]
	ldrb	w23, [x0, 2585]
	ldrb	w0, [x0, 2582]
.L208:
	cmp	w0, 4
	beq	.L209
.L212:
	mov	w1, 1664
	mov	x3, 14011
	add	x0, x21, 13
	add	x4, x21, 269
	smaddl	x2, w19, w1, x3
	smaddl	x1, w20, w1, x3
	add	x2, x21, x2
	add	x1, x21, x1
	.p2align 2
.L210:
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
	bne	.L210
	b	.L258
.L199:
	add	x0, x21, 8192
	ldrb	w20, [x0, 2469]
	ldrb	w22, [x0, 2470]
	ldrb	w0, [x0, 2467]
	b	.L200
.L219:
	ldr	x0, [x21, 24008]
	ldr	w19, [x0, 1196]
	str	w19, [x22, 3584]
	ldr	x1, [x21, 23992]
.L222:
	ldr	w0, [x0, 1196]
	str	w0, [x22, 3588]
	ldr	w0, [x21, 10888]
	ldr	w20, [x1, 1196]
	cmp	w0, 1
	bls	.L266
.L223:
	tst	w0, -3
	bne	.L224
	ldrb	w1, [x26, 2582]
	cmp	w1, 4
	beq	.L267
.L224:
	mov	w3, 1664
	mov	x2, 14011
	add	x0, x21, 13
	add	x4, x21, 269
	smaddl	x1, w19, w3, x2
	smaddl	x2, w20, w3, x2
	add	x1, x21, x1
	add	x2, x21, x2
	.p2align 2
.L231:
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
	bne	.L231
	b	.L258
.L207:
	add	x0, x21, 8192
	ldrb	w22, [x0, 2469]
	ldrb	w23, [x0, 2470]
	ldrb	w0, [x0, 2467]
	b	.L208
.L232:
	ldr	x0, [x21, 24008]
	ldr	w7, [x0, 1196]
	str	w7, [x22, 3584]
	ldr	x1, [x21, 23992]
.L235:
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
	bls	.L268
	ands	w0, w0, -3
	beq	.L269
.L237:
	mov	w8, 1664
	mov	x0, x21
	mov	x4, 0
	smull	x7, w7, w8
	smull	x6, w6, w8
	smull	x8, w1, w8
	.p2align 2
.L238:
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
	bne	.L238
	b	.L258
.L269:
	ldrb	w2, [x26, 2582]
	cmp	w2, 4
	bne	.L237
	b	.L243
.L220:
	ldr	x1, [x21, 23992]
	ldr	x0, [x21, 24008]
	ldr	w19, [x1, 1196]
	str	w19, [x22, 3584]
	b	.L222
.L233:
	ldr	x1, [x21, 23992]
	ldr	w7, [x1, 1196]
	str	w7, [x22, 3584]
	b	.L235
.L267:
	cbnz	w0, .L228
	ldr	w22, [x21, 10892]
	ldr	w24, [x21, 10896]
	uxtb	w22, w22
	uxtb	w24, w24
.L229:
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
.L230:
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
	bne	.L230
	b	.L224
.L266:
	ldrb	w0, [x26, 2582]
	cmp	w0, 4
	bne	.L224
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
.L225:
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
	bne	.L225
	ldr	w0, [x21, 10888]
	b	.L223
.L268:
	ldrb	w0, [x26, 2582]
	cmp	w0, 4
	bne	.L237
	mov	w0, 0
	b	.L243
.L265:
	ldr	w1, [x21, 10880]
	cmp	w1, 1
	bne	.L200
.L201:
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
.L203:
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
	bne	.L203
	b	.L204
.L209:
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
.L211:
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
	bne	.L211
	b	.L212
.L228:
	ldrb	w22, [x26, 2584]
	ldrb	w24, [x26, 2585]
	b	.L229
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
	add	x0, x19, 2504
	add	x1, x0, 20480
	ldrsb	w2, [x0, 3104]
	ldr	w22, [x1, 3544]
	cbz	w2, .L271
	ldr	w0, [x19, 13308]
	ldr	w20, [x1, 3548]
	cmp	w0, 3
	beq	.L356
.L353:
	ldrh	w0, [x19, 5862]
	ldrh	w1, [x19, 5860]
	add	w0, w0, 15
	add	w1, w1, 15
	asr	w0, w0, 4
	mul	w20, w20, w22
	asr	w1, w1, 4
.L323:
	lsl	w2, w1, 4
	mov	w4, 8160
	sub	w3, w2, #32
	cmp	w3, w4
	lsl	w3, w0, 4
	bhi	.L274
	sub	w5, w3, #32
	cmp	w5, w4
	bhi	.L274
	add	x4, x19, 2504
	add	x2, x4, 20480
	ldr	w3, [x2, 3556]
	cmp	w0, w3
	beq	.L357
.L277:
	add	x1, x19, 2504
	lsl	w0, w22, 4
	add	x2, x1, 20480
	add	w0, w0, 63
	and	w0, w0, -64
	mov	w3, 1
	ldr	w4, [x2, 3548]
	str	w22, [x2, 3552]
	str	w4, [x2, 3556]
	str	w0, [x2, 3568]
	ldrb	w0, [x19, 2512]
	cbz	w0, .L285
.L279:
	ldrsb	w1, [x19, 2507]
	cmp	w1, 3
	beq	.L358
.L282:
	ldrb	w1, [x19, 2504]
	eor	x1, x1, 8
	ubfx	x1, x1, 3, 1
	orr	w0, w3, w1
	cbnz	w0, .L285
.L289:
	mov	x0, x21
	bl	VC1_Vfmw_GetImageBuffer
	mov	w23, w0
	cbz	w0, .L359
	add	x1, x19, 2504
	add	x23, x21, 53248
	add	x1, x1, 20480
	ldr	w0, [x23, 784]
	ldr	w1, [x1, 3620]
	bl	FSP_GetFsImagePtr
	mov	x2, x0
	cbz	x0, .L291
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
.L291:
	add	x0, x19, 2504
	ldrsb	w1, [x0, 3104]
	cbz	w1, .L292
	ldr	w1, [x19, 13308]
	cmp	w1, 3
	beq	.L360
.L292:
	ldr	x0, [x19, 26496]
	str	wzr, [x0, 1136]
.L293:
	ldr	x1, [x19, 26496]
	add	x0, x19, 2504
	strb	wzr, [x1, 2]
	ldrsb	w1, [x0, 3104]
	cbnz	w1, .L294
	add	x1, x0, 8192
	ldrb	w2, [x1, 2220]
	cmp	w2, 1
	beq	.L361
.L294:
	add	x24, x19, 2504
	ldrb	w0, [x24, 2]
	cmp	w0, 1
	ccmp	w0, 5, 4, hi
	bne	.L295
	add	x24, x24, 20480
	mov	w0, 2
	ldr	w1, [x24, 3608]
	ldr	w2, [x24, 3612]
	ldr	w3, [x24, 3616]
	cmp	w1, wzr
	csinc	w0, w0, wzr, ne
	str	w0, [x24, 3608]
	cmp	w2, w3
	beq	.L297
	ldr	x1, .LC60
	mov	w0, 13
	bl	dprint_vfmw
	ldr	w0, [x23, 784]
	mov	w2, 0
	ldr	w1, [x24, 3612]
	bl	FSP_SetRef
	ldr	w3, [x24, 3616]
.L297:
	add	x24, x19, 2504
	mov	w2, 1
	add	x25, x24, 20480
	ldr	w1, [x25, 3620]
	str	w3, [x25, 3612]
	str	w1, [x25, 3616]
	ldr	w0, [x23, 784]
	bl	FSP_SetRef
	ldr	w0, [x23, 784]
	ldr	w1, [x25, 3612]
	bl	FSP_RemovePmv
	ldrb	w0, [x24, 2]
	cmp	w0, 1
	ccmp	w0, 5, 4, hi
	bne	.L299
	ldr	x0, [x24, 24008]
	mov	w1, 1
	strb	wzr, [x0]
	ldr	x0, [x24, 23992]
	strb	w1, [x0]
	ldr	x1, [x24, 24000]
	ldr	x0, [x24, 23992]
	str	x0, [x24, 24000]
	str	x1, [x24, 24008]
.L299:
	add	x25, x19, 2504
	ldr	w0, [x23, 784]
	add	x26, x25, 20480
	ldr	w1, [x26, 3620]
	bl	FSP_GetLogicFs
	mov	x24, x0
	ldr	w1, [x26, 3612]
	ldr	w0, [x23, 784]
	bl	FSP_GetLogicFs
	mov	x28, x0
	ldr	w1, [x26, 3616]
	ldr	w0, [x23, 784]
	bl	FSP_GetLogicFs
	mov	x27, x0
	cmp	x24, xzr
	ccmp	x28, xzr, 4, ne
	ccmp	x0, xzr, 4, ne
	beq	.L362
	ldrsb	w0, [x25, 3104]
	cbnz	w0, .L363
.L305:
	str	wzr, [x24, 104]
.L306:
	ldrb	w1, [x19, 2506]
	add	x0, x19, 2504
	cmp	w1, 5
	beq	.L364
.L307:
	ldr	x0, [x24, 640]
	cbz	x0, .L308
	ldr	x1, [x24, 656]
	cbz	x1, .L308
	ldr	x1, [x24, 648]
	cbz	x1, .L308
	ldr	x1, [x27, 640]
	cbz	x1, .L310
	ldr	w0, [x1, 16]
.L311:
	str	w0, [x21, 14460]
	ldr	x0, [x28, 640]
	cbz	x0, .L365
.L354:
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
	ldr	x24, [x19, 26496]
	ldr	w0, [x0, 16]
	bl	MEM_Phy2Vir
	str	x0, [x24, 1544]
	ldr	x24, [x19, 26512]
	ldr	w0, [x21, 14456]
	bl	MEM_Phy2Vir
	str	x0, [x24, 1544]
	ldr	x23, [x19, 26504]
	ldr	w0, [x21, 14460]
	bl	MEM_Phy2Vir
	str	x0, [x23, 1544]
.L284:
	add	x0, x19, 2504
	ldr	x2, [x0, 23992]
	ldrsb	w1, [x0, 3104]
	cbnz	w1, .L314
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
	bls	.L328
	cmp	w3, 4
	mov	w0, 0
	beq	.L366
.L315:
	add	x24, x19, 2504
	strb	w0, [x2, 1030]
	add	x20, x24, 20480
	strb	w0, [x2, 1029]
	mov	w0, 1
	add	x22, x24, 36864
	strb	wzr, [x20, 3648]
	str	w0, [x20, 3628]
	ldr	x0, [x19, 5640]
	ldr	x23, [x22, 1592]
	bl	BsPos
	cmp	w0, wzr
	add	w1, w0, 7
	csel	w0, w1, w0, lt
	asr	w0, w0, 3
	add	x0, x23, x0, sxtw
	str	x0, [x19, 26616]
	ldr	x0, [x19, 5640]
	ldr	w23, [x22, 1612]
	bl	BsPos
	add	w1, w0, 7
	cmp	w0, wzr
	csel	w0, w1, w0, lt
	add	w0, w23, w0, asr 3
	str	w0, [x20, 3652]
	ldr	x0, [x19, 5640]
	bl	BsPos
	and	w0, w0, 7
	str	w0, [x20, 3656]
	ldr	w23, [x22, 1620]
	ldr	x0, [x19, 5640]
	lsl	w23, w23, 3
	bl	BsPos
	sub	w0, w23, w0
	str	w0, [x20, 3660]
	ldr	w1, [x22, 1668]
	ldr	w0, [x22, 1664]
	mul	w0, w0, w1
	sub	w0, w0, #1
	str	w0, [x20, 3664]
	ldrb	w1, [x19, 2506]
.L316:
	add	x19, x19, 2504
	mov	w3, 1
	mov	w2, 0
	mov	x0, x19
	bl	VC1_Vfmw_BuildRefList
	mov	w23, w0
	cbz	w0, .L367
	mov	w23, 1
	b	.L352
	.p2align 3
.L274:
	ldr	x1, .LC51
	mov	w0, 0
	mov	w23, 0
	bl	dprint_vfmw
.L352:
	mov	w0, w23
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L271:
	ldr	w0, [x1, 3548]
	mov	w1, w22
	mul	w20, w22, w0
	b	.L323
	.p2align 3
.L356:
	add	w20, w20, 1
	add	w20, w20, w20, lsr 31
	asr	w20, w20, 1
	b	.L353
	.p2align 3
.L285:
	add	x0, x19, 2504
	add	x23, x21, 53248
	add	x25, x0, 20480
	add	x1, x25, 1848
	str	x1, [x19, 26496]
	str	x1, [x19, 26504]
	str	x1, [x19, 26512]
	str	x1, [x19, 26520]
	ldr	w0, [x23, 784]
	ldr	w1, [x25, 3616]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	cbz	x0, .L281
	ldr	w0, [x23, 784]
	ldr	x1, [x24, 560]
	bl	FreeUsdByDec
	ldr	w0, [x23, 784]
	ldr	x1, [x24, 568]
	bl	FreeUsdByDec
	ldr	w0, [x23, 784]
	ldr	x1, [x24, 576]
	bl	FreeUsdByDec
	ldr	w0, [x23, 784]
	ldr	x1, [x24, 584]
	bl	FreeUsdByDec
	add	x0, x24, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w0, [x23, 784]
	ldr	w1, [x25, 3616]
	bl	FSP_ClearLogicFs
	ldr	w0, [x23, 784]
	mov	w2, 1
	ldr	w1, [x25, 3612]
	bl	FSP_ClearLogicFs
	str	wzr, [x25, 3612]
	str	wzr, [x25, 3616]
	ldr	w0, [x23, 784]
	bl	FSP_GetNullLogicFs
	str	w0, [x25, 3612]
	str	w0, [x25, 3616]
.L281:
	add	x2, x19, 2504
	add	x0, x21, 28672
	add	x2, x2, 20480
	add	x1, x21, 36864
	add	x0, x0, 129
	add	x1, x1, 257
	str	wzr, [x2, 3608]
	.p2align 2
.L287:
	strb	wzr, [x0]
	strb	wzr, [x0, -1]
	add	x0, x0, 1664
	cmp	x1, x0
	bne	.L287
	add	x24, x23, 176
	ldr	w0, [x23, 784]
	mov	x1, x24
	bl	FSP_ClearNotInVoQueue
	ldrb	w0, [x19, 2504]
	tbnz	x0, 3, .L289
	mov	x0, x24
	bl	ResetVoQueue
	ldr	x1, [x21]
	ldr	w0, [x1, 668]
	ldr	w3, [x1, 672]
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
	bne	.L288
	ldrb	w0, [x19, 2504]
	orr	w0, w0, 8
	strb	w0, [x19, 2504]
	b	.L289
	.p2align 3
.L359:
	ldr	x1, .LC55
	mov	w0, 1
	bl	dprint_vfmw
	b	.L352
	.p2align 3
.L357:
	ldr	w0, [x2, 3552]
	cmp	w1, w0
	bne	.L277
	lsl	w0, w22, 4
	mov	w3, 0
	add	w0, w0, 63
	and	w0, w0, -64
	str	w0, [x2, 3568]
	ldrb	w0, [x4, 8]
	cbz	w0, .L282
	b	.L279
	.p2align 3
.L295:
	add	x0, x24, 20480
	ldr	w1, [x0, 3608]
	cmp	w1, 1
	bhi	.L299
	ldr	w1, [x0, 3620]
	mov	w2, 1
	ldr	w0, [x23, 784]
	mov	w23, 0
	bl	FSP_ClearLogicFs
	ldr	x2, [x24, 23992]
	mov	w0, 1
	ldr	x1, .LC62
	strb	wzr, [x2]
	ldr	x2, [x24, 23992]
	strb	wzr, [x2, 1]
	bl	dprint_vfmw
	b	.L352
	.p2align 3
.L362:
	ldr	w0, [x23, 784]
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
	beq	.L368
.L303:
	ldr	x1, .LC66
	mov	w0, 1
	mov	w23, 0
	bl	dprint_vfmw
	b	.L352
	.p2align 3
.L314:
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
	bls	.L330
	cmp	w3, 4
	mov	w2, 0
	beq	.L369
.L317:
	add	x0, x4, x0, sxtw
	add	x3, x19, 2504
	add	x1, x3, 20480
	strb	w2, [x0, 1029]
	ldr	w5, [x1, 3628]
	cbz	w5, .L318
	sub	w5, w5, #1
	mov	w1, 0
	mov	w4, w5
	cbz	w5, .L355
	.p2align 2
.L343:
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
	bne	.L343
	uxtb	w4, w4
.L320:
	lsl	x0, x5, 3
	add	x2, x19, 2504
	sub	x0, x0, x5
	sub	w20, w20, #1
	add	x0, x2, x0, lsl 3
	add	x0, x0, 20480
	strb	w4, [x0, 3648]
	str	w20, [x0, 3664]
	ldrb	w1, [x19, 2506]
	cmp	w1, 2
	bne	.L316
	add	x0, x2, 8192
	ldr	w3, [x19, 13412]
	add	x2, x2, 36864
	ldrb	w0, [x0, 2599]
	mul	w3, w0, w3
	asr	w3, w3, 8
	str	w3, [x2, 1640]
	sub	w0, w0, w3
	subs	w0, w0, #1
	csel	w0, w0, wzr, pl
	str	w0, [x2, 1644]
	b	.L316
	.p2align 3
.L363:
	ldr	w0, [x25, 10804]
	cmp	w0, 3
	bne	.L305
	ldr	x0, .LC58
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L305
	str	w0, [x24, 104]
	b	.L306
	.p2align 3
.L368:
	add	x25, x25, 36864
	ldr	w0, [x25, 1684]
	cmp	w0, wzr
	ble	.L303
	sub	w0, w0, #1
	str	w0, [x25, 1684]
	b	.L303
	.p2align 3
.L358:
	ldrb	w0, [x19, 2513]
	cmp	w0, 1
	bne	.L284
	b	.L282
	.p2align 3
.L367:
	add	x0, x19, 20480
	add	x21, x21, 53248
	mov	w2, 1
	ldr	w1, [x0, 3620]
	ldr	w0, [x21, 784]
	bl	FSP_ClearLogicFs
	ldr	x2, [x19, 23992]
	mov	w0, 1
	ldr	x1, .LC70
	strb	wzr, [x2]
	ldr	x2, [x19, 23992]
	strb	wzr, [x2, 1]
	bl	dprint_vfmw
	b	.L352
	.p2align 3
.L308:
	add	x19, x19, 2504
	ldr	w0, [x23, 784]
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
	bne	.L303
	add	x19, x19, 36864
	ldr	w0, [x19, 1684]
	cmp	w0, wzr
	ble	.L303
	sub	w0, w0, #1
	str	w0, [x19, 1684]
	b	.L303
	.p2align 3
.L364:
	ldr	w1, [x28, 104]
	add	x0, x0, 36864
	str	w1, [x24, 104]
	ldr	x2, [x19, 26512]
	ldr	x1, [x19, 26496]
	ldr	w2, [x2, 1136]
	str	w2, [x1, 1136]
	ldr	w1, [x0, 1684]
	add	w1, w1, 1
	str	w1, [x0, 1684]
	b	.L307
	.p2align 3
.L318:
	mov	w0, 1
	mov	x5, 0
	str	w0, [x1, 3628]
.L355:
	mov	w4, 0
	b	.L320
	.p2align 3
.L361:
	ldr	x0, [x19, 26496]
	ldrb	w1, [x1, 2458]
	strb	w1, [x0, 2]
	b	.L294
	.p2align 3
.L360:
	ldr	x1, .LC58
	ldr	w1, [x1]
	cmp	w1, 1
	bne	.L292
	ldr	x0, [x19, 26496]
	str	w1, [x0, 1136]
	b	.L293
	.p2align 3
.L288:
	ldr	x1, .LC53
	mov	w0, 1
	mov	w23, 0
	bl	dprint_vfmw
	ldrb	w0, [x19, 2504]
	and	w0, w0, -9
	strb	w0, [x19, 2504]
	b	.L352
	.p2align 3
.L330:
	mov	w2, 1
	b	.L317
	.p2align 3
.L328:
	mov	w0, 1
	b	.L315
.L369:
	ldrb	w1, [x1, 2583]
	sub	w1, w1, #2
	uxtb	w1, w1
	cmp	w1, 1
	cset	w2, ls
	b	.L317
.L366:
	ldrb	w0, [x1, 2468]
	sub	w0, w0, #2
	uxtb	w0, w0
	cmp	w0, 1
	cset	w0, ls
	b	.L315
.L310:
	ldr	w0, [x0, 16]
	b	.L311
.L365:
	ldr	x0, [x24, 640]
	b	.L354
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
	cbz	w0, .L373
	mov	w19, 1
	mov	w0, w19
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L373:
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
	beq	.L378
	cmp	w20, 2
	beq	.L384
	cbz	w20, .L383
	add	w2, w2, 1
	add	w2, w2, w2, lsr 31
	asr	w2, w2, 1
.L384:
	str	w2, [x0, 1664]
	ldr	w1, [x1, 3548]
	add	w1, w1, 1
	add	w1, w1, w1, lsr 31
	asr	w1, w1, 1
	str	w1, [x0, 1668]
	b	.L381
	.p2align 3
.L378:
	add	w2, w2, 1
	add	w2, w2, w2, lsr 31
	asr	w2, w2, 1
.L383:
	str	w2, [x0, 1664]
	ldr	w1, [x1, 3548]
	str	w1, [x0, 1668]
.L381:
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
	ble	.L393
	cmp	w0, 3
	ble	.L391
	cmp	w0, 8
	mov	w1, 2
	csinc	w1, w1, wzr, lt
	cset	w19, lt
.L387:
	mov	x0, x20
	sbfiz	x19, x19, 2, 32
	bl	BsSkip
	cmp	w21, 12
	add	x0, x29, 96
	add	x19, x0, x19
	bhi	.L394
.L388:
	ldr	w0, [x19, -24]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L391:
	mov	w1, 3
	mov	x0, x20
	mov	w19, 2
	bl	BsSkip
	sbfiz	x19, x19, 2, 32
	cmp	w21, 12
	add	x0, x29, 96
	add	x19, x0, x19
	bls	.L388
.L394:
	ldr	w0, [x19, -48]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L393:
	mov	w1, 4
	mov	w19, 3
	cbz	w0, .L387
	mov	w19, w1
	b	.L387
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
	ble	.L403
	cmp	w0, 4
	csinc	w1, w2, wzr, lt
	cset	w19, lt
.L398:
	mov	x0, x20
	bl	BsSkip
	cmp	w21, 12
	sbfiz	x2, x19, 2, 32
	add	x0, x29, 96
	add	x2, x0, x2
	bhi	.L404
	ldr	w0, [x2, -24]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L404:
	ldr	w0, [x2, -40]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L403:
	mov	w19, w1
	cbz	w0, .L398
	mov	w19, w2
	b	.L398
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
	ble	.L412
	cmp	w0, 4
	csinc	w1, w2, wzr, lt
	cset	w19, lt
.L407:
	mov	x0, x20
	bl	BsSkip
	cmp	w21, 12
	sbfiz	x2, x19, 2, 32
	add	x0, x29, 80
	add	x2, x0, x2
	bhi	.L413
	ldr	w0, [x2, -16]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L413:
	ldr	w0, [x2, -32]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L412:
	mov	w19, w1
	cbz	w0, .L407
	mov	w19, w2
	b	.L407
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
	bgt	.L415
	mov	w1, 3
	mov	x0, x21
	bl	BsSkip
	add	x0, x29, 256
	add	x0, x0, x19, sxtw 2
	ldr	w2, [x0, -208]
	ldr	w1, [x0, -176]
.L416:
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
.L415:
	mov	w1, 7
	mov	x0, x21
	bl	BsGet
	and	w19, w0, 15
	cmp	w19, 15
	beq	.L419
	sub	w0, w0, #112
	add	x1, x29, 256
	add	w19, w19, 7
	add	x0, x1, x0, sxtw 2
	ldr	w2, [x0, -112]
	ldr	w1, [x0, -56]
	b	.L416
	.p2align 3
.L419:
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
	bgt	.L421
	mov	w1, 3
	mov	x0, x22
	bl	BsSkip
	add	x0, x29, 256
	add	x0, x0, x20, sxtw 2
	ldr	w4, [x0, -176]
	ldr	w1, [x0, -208]
	sub	w0, w4, #1
	cmp	w0, 7
	bhi	.L425
.L427:
	add	x2, x29, 256
	add	x0, x2, x0, sxtw 2
	ldr	w0, [x0, -144]
	mul	w0, w1, w0
.L426:
	str	w0, [x21]
.L423:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 256
	ret
	.p2align 3
.L421:
	mov	w1, 7
	mov	x0, x22
	bl	BsGet
	and	w20, w0, 15
	sub	w1, w20, #14
	add	w20, w20, 7
	cmp	w1, 1
	bls	.L423
	sub	w1, w0, #112
	cmp	w1, 13
	bhi	.L424
	add	x0, x29, 256
	add	x0, x0, x1, sxtw 2
	ldr	w4, [x0, -56]
	ldr	w1, [x0, -112]
	sub	w0, w4, #1
	cmp	w0, 7
	bls	.L427
.L425:
	ldr	x1, .LC87
	mov	w3, 3591
	add	x2, x19, 336
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L426
	.p2align 3
.L424:
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
	b	.L426
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
	beq	.L446
	mov	w1, 1
	mov	x0, x22
	bl	BsGet
	uxtb	w21, w0
	cmp	w21, 1
	strb	w21, [x19]
	beq	.L447
.L432:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L446:
	mov	w0, 1
	mov	w1, 3
	strb	w0, [x19, 3]
	strb	w0, [x19]
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 7
	strb	w0, [x19, 4]
	beq	.L448
	add	w20, w20, 1
	add	w0, w0, w20
	strb	w0, [x19, 8]
.L431:
	mov	w0, 1
	str	w0, [x19, 12]
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L447:
	mov	w1, 2
	mov	x0, x22
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x19, 1]
	cbnz	w1, .L434
	str	w21, [x19, 12]
.L435:
	ldr	w0, [x19]
	and	w0, w0, 16776960
	cmp	w0, 768
	beq	.L432
	mov	w0, 1
	mov	w1, 3
	strb	w0, [x19, 3]
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 7
	strb	w0, [x19, 4]
	beq	.L449
	add	w20, w20, 1
	add	w0, w0, w20
	strb	w0, [x19, 8]
	b	.L432
	.p2align 3
.L434:
	cmp	w1, 2
	beq	.L450
	cmp	w1, 1
	beq	.L451
	cmp	w1, 3
	beq	.L452
	str	wzr, [x19, 12]
	b	.L435
.L452:
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
	b	.L435
	.p2align 3
.L448:
	mov	w1, 5
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	strb	w0, [x19, 8]
	b	.L431
.L450:
	mov	x0, x22
	bl	BsGet
	and	w1, w0, 255
	strb	w0, [x19, 6]
	add	w0, w1, 6
	str	w0, [x19, 12]
	b	.L435
.L451:
	mov	w1, 2
	mov	x0, x22
	bl	BsGet
	and	w1, w0, 255
	strb	w0, [x19, 7]
	add	w0, w1, 2
	str	w0, [x19, 12]
	b	.L435
.L449:
	mov	w1, 5
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 5]
	strb	w0, [x19, 8]
	b	.L432
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
	b	.L454
	.p2align 3
.L464:
	mov	w22, w19
.L454:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	mov	w21, w0
	mov	w1, 3
	mov	x0, x20
	cbnz	w21, .L463
	bl	BsGet
	cmp	w0, 1
	mov	w19, w0
	mov	w1, w0
	mov	x0, x20
	bgt	.L473
	bne	.L457
	bl	BsGet
	cbz	w0, .L458
	cbz	w22, .L464
.L470:
	str	w24, [x23]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L463:
	mov	w0, 0
.L455:
	eor	w1, w0, 63
	cmp	w22, wzr
	csel	w0, w1, w0, ne
	mov	w21, 1
	str	w0, [x23]
.L475:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L473:
	sub	w19, w19, #2
	mov	w0, 1
	lsl	w0, w0, w19
	b	.L455
	.p2align 3
.L457:
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	cmp	w0, 255
	beq	.L470
	ldr	x1, .LC93
	add	x1, x1, 488
	ldr	w0, [x1, w0, sxtw 2]
	b	.L455
	.p2align 3
.L458:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	ldr	x1, .LC93
	add	x1, x1, 360
	ldr	w1, [x1, w0, sxtw 2]
	cmp	w1, 2
	beq	.L474
	cmp	w1, 3
	beq	.L455
	str	w0, [x23]
	b	.L475
.L474:
	orr	w0, w0, 32
	b	.L455
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
	beq	.L490
	ldrh	w1, [x19, 156]
	ldrh	w0, [x19, 158]
	strh	w1, [x19, 160]
	strh	w0, [x19, 162]
.L483:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 23]
	beq	.L484
.L486:
	mov	x0, x20
	bl	BsPos
	ldr	w1, [x19, 168]
	mov	w2, 1
	sub	w0, w0, w1
	str	w0, [x19, 168]
.L485:
	mov	w0, w2
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L484:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 31
	strb	w0, [x19, 24]
	mov	w2, 0
	bhi	.L485
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 25]
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 26]
	ldrb	w1, [x19, 24]
	cbz	w1, .L486
	mov	w21, 0
	.p2align 2
.L487:
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
	bgt	.L487
	b	.L486
	.p2align 3
.L490:
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
	beq	.L491
.L479:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w1, w0
	cmp	w1, 1
	strb	w1, [x19, 15]
	beq	.L492
.L481:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 19]
	bne	.L483
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
	b	.L483
	.p2align 3
.L492:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 16]
	cbnz	w0, .L482
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 17]
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	b	.L481
	.p2align 3
.L491:
	mov	w1, 4
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 15
	strb	w0, [x19, 12]
	bne	.L479
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 13]
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 14]
	b	.L479
.L482:
	mov	w1, 16
	mov	x0, x20
	bl	BsGet
	strh	w0, [x19, 164]
	b	.L481
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
	beq	.L494
.L497:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 11]
	beq	.L514
.L512:
	ldrh	w0, [x19, 52]
.L499:
	ldrh	w1, [x19, 50]
	ldrh	w2, [x22, 156]
	cmp	w1, 15
	ccmp	w1, w2, 2, hi
	bls	.L500
	strh	w2, [x19, 50]
.L500:
	ldrh	w1, [x22, 158]
	cmp	w0, 15
	ccmp	w1, w0, 0, hi
	bcs	.L501
	strh	w1, [x19, 52]
.L501:
	ldrb	w1, [x19, 6]
	cmp	w1, 1
	beq	.L515
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 13]
	beq	.L516
.L503:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 15]
	beq	.L517
.L504:
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
.L494:
	ldrb	w0, [x22, 24]
	cbz	w0, .L497
	mov	w21, 0
	.p2align 2
.L498:
	mov	w1, 8
	mov	x0, x20
	bl	BsGet
	add	x1, x19, x21, sxtw
	add	w21, w21, 1
	strb	w0, [x1, 17]
	ldrb	w0, [x22, 24]
	cmp	w0, w21
	bgt	.L498
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 11]
	bne	.L512
.L514:
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
	b	.L499
	.p2align 3
.L517:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	b	.L504
	.p2align 3
.L515:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 13]
	bne	.L503
	.p2align 2
.L516:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 14]
	b	.L503
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
	ble	.L521
	mov	w1, 2
	mov	x0, x20
	bl	BsSkip
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L521:
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
	ble	.L524
	cmp	w0, 11
	bgt	.L528
	mov	w1, 2
	mov	w20, w1
.L523:
	mov	x0, x19
	bl	BsSkip
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L528:
	cmp	w0, 13
	ble	.L526
	cmp	w0, 14
	mov	w1, 4
	beq	.L527
	mov	w20, 5
	b	.L523
	.p2align 3
.L524:
	mov	w1, 1
	mov	x0, x19
	mov	w20, w1
	bl	BsSkip
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L526:
	mov	w1, 3
	mov	w20, 0
	b	.L523
	.p2align 3
.L527:
	mov	w20, w1
	b	.L523
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
	bgt	.L530
	mov	w1, 2
	mov	x0, x20
	asr	w19, w2, 3
	bl	BsSkip
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L530:
	cmp	w0, 27
	mov	w19, 3
	bgt	.L536
	mov	w1, w19
	mov	x0, x20
	bl	BsSkip
	mov	w0, w19
.L537:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L536:
	mov	w0, 5
	cmp	w2, 29
	mov	w19, 4
	csel	w19, w0, w19, gt
	mov	x0, x20
	mov	w1, w19
	bl	BsSkip
	mov	w0, w19
	b	.L537
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
	bgt	.L546
	b	.L543
	.p2align 3
.L549:
	bl	BsSkip
	cmp	w19, w21
	beq	.L543
.L546:
	mov	w1, 24
	mov	x0, x20
	add	w19, w19, 1
	bl	BsShow
	mov	w2, w0
	cmp	w2, 1
	mov	w1, 8
	mov	x0, x20
	bne	.L549
	mov	w0, w2
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L543:
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
	cbz	x1, .L552
	ldr	w2, [x2, 1616]
	cbnz	w2, .L571
.L552:
	mov	x1, 0
	str	xzr, [x19, 8]
	stp	wzr, wzr, [x19, 36]
	str	wzr, [x19, 44]
	str	wzr, [x19, 52]
.L553:
	ldr	w4, [x20, 3544]
	ldr	w0, [x20, 3548]
	ldrb	w2, [x22, 8]
	mul	w4, w0, w4
	sub	w4, w4, #1
	cbz	w2, .L554
	add	w0, w0, 1
	ldr	w3, [x21, 4]
	add	w0, w0, w0, lsr 31
	sub	w3, w3, w0, asr 1
	str	w3, [x19, 32]
	cmp	w4, w3
	ccmp	w3, wzr, 4, cc
	bgt	.L572
.L555:
	ldr	w0, [x19, 28]
	cmp	w0, wzr
	ble	.L557
	cbz	x1, .L558
	ldr	w0, [x19, 44]
	cmp	w0, wzr
	ble	.L557
.L558:
	ldr	w1, [x20, 3628]
	mov	w0, 1
	add	w1, w1, w0
	str	w1, [x20, 3628]
.L556:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L554:
	ldr	w3, [x21, 4]
	str	w3, [x19, 32]
	cmp	w4, w3
	bcs	.L555
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
.L571:
	str	x1, [x19, 8]
	str	wzr, [x19, 40]
	ldr	w2, [x0, 32]
	lsl	w2, w2, 3
	str	w2, [x19, 44]
	ldr	w2, [x0, 24]
	str	w2, [x19, 36]
	ldr	w0, [x0, 40]
	str	w0, [x19, 52]
	b	.L553
	.p2align 3
.L572:
	ldr	x1, .LC101
	mov	w0, 1
	ldr	w4, [x20, 3548]
	ldr	w2, [x21, 4]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L556
	.p2align 3
.L557:
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
	beq	.L574
	add	x0, x20, 20480
	add	w1, w1, 15
	asr	w1, w1, 4
	str	w1, [x0, 3544]
	ldrh	w1, [x19, 238]
	str	w1, [x0, 3560]
	ldrh	w1, [x19, 240]
	ldrh	w2, [x21, 2448]
	cmp	w2, w1
	beq	.L575
.L576:
	add	w1, w1, 15
	asr	w1, w1, 4
	str	w1, [x0, 3548]
	ldrh	w1, [x19, 240]
	str	w1, [x0, 3564]
.L575:
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
.L574:
	ldrh	w1, [x19, 240]
	ldrh	w0, [x21, 2448]
	cmp	w0, w1
	bne	.L581
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L581:
	add	x0, x20, 20480
	b	.L576
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
	cbnz	w0, .L583
	mov	x0, x19
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
.L583:
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
	cbnz	w20, .L585
	add	x0, x29, 160
	add	x19, x0, x19, sxtw 2
	ldr	w0, [x19, -128]
.L585:
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
	cbz	w22, .L588
	ldrb	w24, [x21, 14]
	mov	w1, w22
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 104]
	cmp	w22, 8
	bls	.L599
	strb	wzr, [x19, 7]
.L590:
	cmp	w24, 1
	beq	.L600
	strb	wzr, [x19, 8]
.L592:
	ldrb	w0, [x21, 7]
	cmp	w0, 1
	beq	.L601
	strb	wzr, [x19, 9]
.L594:
	ldrb	w0, [x19, 3]
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cbz	w0, .L597
	ldrb	w0, [x21, 4]
	cmp	w0, 1
	beq	.L602
.L597:
	ldrb	w1, [x19, 10]
.L596:
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
.L588:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L599:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	ldrb	w24, [x21, 14]
	b	.L590
	.p2align 3
.L601:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 9]
	b	.L594
	.p2align 3
.L600:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L592
	.p2align 3
.L602:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x19, 10]
	b	.L596
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
	cbz	w22, .L615
	ldrb	w24, [x21, 14]
	uxtb	w1, w0
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 104]
	cmp	w22, 8
	bls	.L626
	strb	wzr, [x19, 7]
.L607:
	cmp	w24, 1
	beq	.L627
	strb	wzr, [x19, 8]
.L609:
	ldrb	w0, [x21, 7]
	cmp	w0, 1
	beq	.L628
	strb	wzr, [x19, 9]
.L611:
	ldrb	w0, [x21, 4]
	cmp	w0, 1
	beq	.L612
	ldrb	w1, [x19, 10]
.L613:
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
	beq	.L629
.L614:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x23
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L615
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 15]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	ldrb	w1, [x21, 8]
	cbnz	w1, .L630
.L616:
	mov	x0, x20
	bl	BsPos
	mov	w2, w0
	ldr	x1, .LC110
	mov	w0, 18
	bl	dprint_vfmw
	ldrb	w1, [x21, 9]
	cmp	w1, 1
	beq	.L631
.L618:
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
.L615:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L626:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	ldrb	w24, [x21, 14]
	b	.L607
	.p2align 3
.L630:
	ldrb	w2, [x19, 104]
	add	x3, x21, 272
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L616
	.p2align 3
.L629:
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
	b	.L614
	.p2align 3
.L612:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w1, w0
	strb	w1, [x19, 10]
	b	.L613
	.p2align 3
.L628:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 9]
	b	.L611
	.p2align 3
.L627:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L609
	.p2align 3
.L631:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 17]
	bne	.L618
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	b	.L618
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
	cbz	w22, .L642
	ldrb	w23, [x21, 14]
	uxtb	w1, w0
	mov	w0, w23
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 104]
	cmp	w22, 8
	bls	.L654
	strb	wzr, [x19, 7]
.L636:
	cmp	w23, 1
	beq	.L655
	strb	wzr, [x19, 8]
.L638:
	ldrb	w0, [x21, 7]
	cmp	w0, 1
	beq	.L656
	strb	wzr, [x19, 9]
.L640:
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
	beq	.L642
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 15]
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	ldrb	w1, [x21, 8]
	cbnz	w1, .L657
.L643:
	ldrb	w1, [x21, 9]
	cmp	w1, 1
	beq	.L658
.L645:
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
.L642:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L654:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 7]
	ldrb	w23, [x21, 14]
	b	.L636
	.p2align 3
.L657:
	ldrb	w2, [x19, 104]
	add	x3, x21, 272
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L643
	.p2align 3
.L656:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 9]
	b	.L640
	.p2align 3
.L655:
	mov	w1, w23
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L638
	.p2align 3
.L658:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 17]
	bne	.L645
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	b	.L645
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
	ble	.L697
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
	cbnz	w0, .L662
	ldrb	w2, [x22, 248]
.L663:
	ldr	x1, .LC115
	mov	w0, 18
	bl	dprint_vfmw
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 1]
	ldrb	w0, [x22, 12]
	cbnz	w0, .L698
.L664:
	ldrb	w0, [x22, 13]
	mov	w1, 1
	cbnz	w0, .L665
	mov	x0, x20
	bl	BsGet
	cmp	w0, wzr
	cset	w0, ne
	strb	w0, [x19, 3]
.L666:
	strb	w0, [x21, 2]
	strb	wzr, [x21, 4]
	ldrb	w0, [x19, 3]
	cmp	w0, 4
	bhi	.L695
	ldr	x1, .LC116
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx671
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx671:
	.section	.rodata
	.align	0
	.align	2
.L671:
	.byte	(.L670 - .Lrtx671) / 4
	.byte	(.L672 - .Lrtx671) / 4
	.byte	(.L673 - .Lrtx671) / 4
	.byte	(.L695 - .Lrtx671) / 4
	.byte	(.L674 - .Lrtx671) / 4
	.text
	.p2align 3
.L672:
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseSMPPPicHdr
	mov	w21, w0
.L675:
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
	bgt	.L688
	ldr	x1, .LC122
	mov	w0, 1
	bl	dprint_vfmw
	.p2align 2
.L695:
	mov	w0, 0
.L688:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L697:
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
.L665:
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	beq	.L699
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	beq	.L700
	mov	w0, 2
	add	x2, x22, 251
	strb	w0, [x19, 3]
	add	x1, x22, 356
	mov	x0, x20
	bl	VC1_DecBfractionSMP
	uxtb	w0, w0
	cmp	w0, 21
	strb	w0, [x19, 4]
	beq	.L695
	ldrb	w0, [x19, 3]
	b	.L666
	.p2align 3
.L662:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w2, w0
	strb	w2, [x22, 248]
	b	.L663
	.p2align 3
.L699:
	strb	w0, [x19, 3]
	b	.L666
	.p2align 3
.L674:
	ldrb	w23, [x21, 3120]
	cmp	w23, 1
	bne	.L695
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseSMPIPicHdr
	cmp	w0, 1
	mov	w21, w0
	beq	.L675
	ldr	x1, .LC120
	mov	w0, w23
	bl	dprint_vfmw
	b	.L675
	.p2align 3
.L673:
	ldrb	w23, [x21, 3120]
	cmp	w23, 1
	bne	.L695
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseMPBPicHdr
	cmp	w0, 1
	mov	w21, w0
	beq	.L675
	ldr	x1, .LC118
	mov	w0, w23
	bl	dprint_vfmw
	b	.L675
	.p2align 3
.L670:
	mov	x1, x21
	mov	x2, x22
	mov	x0, x20
	bl	VC1_Vfmw_ParseSMPIPicHdr
	mov	w21, w0
	b	.L675
	.p2align 3
.L698:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 2]
	b	.L664
	.p2align 3
.L700:
	strb	wzr, [x19, 3]
	mov	w0, 0
	b	.L666
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
	.xword	.L671
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
	beq	.L717
.L702:
	ldr	w0, [x19, 44]
	cbnz	w0, .L703
	ldrb	w1, [x23, 9]
	cmp	w1, 1
	beq	.L718
.L703:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w22, w0
	strb	w22, [x19, 10]
	cbz	w22, .L711
	add	x26, x21, 188
	ldrb	w24, [x26, 10]
	uxtb	w1, w0
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L719
	strb	wzr, [x19, 3]
.L707:
	cmp	w24, 1
	beq	.L720
	strb	wzr, [x19, 11]
.L709:
	ldrb	w0, [x23, 5]
	cmp	w0, 1
	beq	.L721
.L710:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x25
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L711
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
	cbz	w1, .L705
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
.L711:
	mov	w22, 0
.L705:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L719:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w24, [x26, 10]
	b	.L707
	.p2align 3
.L718:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	b	.L703
	.p2align 3
.L717:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	b	.L702
	.p2align 3
.L721:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L710
	.p2align 3
.L720:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L709
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
	beq	.L723
	ldr	w0, [x19, 44]
	cmp	w0, 2
	beq	.L723
	ldrb	w1, [x22, 9]
	cmp	w1, 1
	beq	.L759
.L725:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w23, w0
	strb	w23, [x19, 10]
	cbz	w23, .L739
	add	x24, x21, 188
	ldrb	w25, [x24, 10]
	uxtb	w1, w0
	mov	w0, w25
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w23, 8
	bls	.L760
	strb	wzr, [x19, 3]
.L729:
	cmp	w25, 1
	beq	.L761
	strb	wzr, [x19, 11]
.L731:
	ldrb	w0, [x22, 5]
	cmp	w0, 1
	beq	.L762
.L732:
	ldrb	w0, [x24, 6]
	cmp	w0, 1
	beq	.L763
.L733:
	ldr	w0, [x19, 44]
	cbz	w0, .L764
	ldrb	w0, [x24, 12]
	cmp	w0, 1
	beq	.L765
	strb	wzr, [x19, 25]
.L737:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 26]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, 1
	str	w0, [x19, 120]
	beq	.L766
.L738:
	strb	wzr, [x19, 34]
.L735:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x26
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L739
	ldr	w1, [x19, 44]
	cmp	w1, 2
	beq	.L767
.L740:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	ldr	w1, [x19, 44]
	cbnz	w1, .L741
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
.L742:
	ldr	w22, [x19, 44]
	strb	w0, [x19, 19]
	cmp	w22, 2
	beq	.L768
.L743:
	ldrb	w1, [x24, 7]
	cbnz	w1, .L769
.L745:
	ldrb	w1, [x24, 8]
	cmp	w1, 1
	beq	.L770
.L747:
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
.L739:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L723:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	ldrb	w1, [x22, 9]
	cmp	w1, 1
	bne	.L725
.L759:
	ldr	w0, [x19, 44]
	cbnz	w0, .L725
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	b	.L725
	.p2align 3
.L741:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	b	.L742
	.p2align 3
.L760:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w25, [x24, 10]
	b	.L729
	.p2align 3
.L766:
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 16]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 17]
	b	.L738
	.p2align 3
.L764:
	ldrb	w1, [x19, 116]
	mov	x0, x20
	bl	VC1_DecMvmodeProgrP
	uxtb	w0, w0
	cmp	w0, 4
	strb	w0, [x19, 14]
	bne	.L735
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
	b	.L735
	.p2align 3
.L769:
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L745
	.p2align 3
.L761:
	mov	w1, w25
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L731
	.p2align 3
.L763:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L733
	.p2align 3
.L762:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L732
	.p2align 3
.L767:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	b	.L740
	.p2align 3
.L768:
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 28]
	ldrb	w1, [x19, 26]
	mov	w0, 0
	cmp	w1, 1
	beq	.L771
	strb	w0, [x19, 29]
	b	.L743
	.p2align 3
.L770:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L747
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L747
	.p2align 3
.L765:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L737
.L771:
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 29]
	b	.L743
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
	beq	.L773
	ldr	w2, [x19, 44]
	cmp	w2, 2
	beq	.L773
.L774:
	ldrb	w1, [x22, 9]
	cmp	w1, 1
	beq	.L814
.L775:
	cbnz	w2, .L779
	add	x1, x21, 508
	mov	x0, x20
	bl	VC1_DecBfraction
	uxtb	w0, w0
	cmp	w0, 21
	strb	w0, [x19, 2]
	beq	.L777
.L779:
	mov	w1, 5
	mov	x0, x20
	bl	BsGet
	uxtb	w23, w0
	strb	w23, [x19, 10]
	cbz	w23, .L777
	add	x24, x21, 188
	ldrb	w26, [x24, 10]
	uxtb	w1, w0
	mov	w0, w26
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w23, 8
	bls	.L815
	strb	wzr, [x19, 3]
.L782:
	cmp	w26, 1
	beq	.L816
	strb	wzr, [x19, 11]
.L784:
	ldrb	w0, [x22, 5]
	cmp	w0, 1
	beq	.L817
.L785:
	ldr	w0, [x19, 44]
	cmp	w0, 2
	beq	.L818
.L786:
	ldrb	w0, [x24, 6]
	cmp	w0, 1
	beq	.L819
.L787:
	ldr	w0, [x19, 44]
	cbz	w0, .L820
	ldrb	w0, [x24, 12]
	cmp	w0, 1
	beq	.L821
.L791:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 120]
.L790:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x25
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L777
	ldr	w1, [x19, 44]
	cmp	w1, 2
	beq	.L822
.L792:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 18]
	ldr	w1, [x19, 44]
	cbnz	w1, .L793
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
.L794:
	ldr	w22, [x19, 44]
	strb	w0, [x19, 19]
	cmp	w22, 2
	beq	.L823
.L795:
	ldrb	w1, [x24, 7]
	cbnz	w1, .L824
.L796:
	ldrb	w1, [x24, 8]
	cmp	w1, 1
	beq	.L825
.L798:
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
.L777:
	mov	w1, 0
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L814:
	cbnz	w2, .L779
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 9]
	ldr	w2, [x19, 44]
	b	.L775
	.p2align 3
.L773:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 8]
	ldr	w2, [x19, 44]
	b	.L774
	.p2align 3
.L793:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	b	.L794
	.p2align 3
.L815:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w26, [x24, 10]
	b	.L782
	.p2align 3
.L820:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, wzr
	mov	w1, 3
	csinc	w1, w1, wzr, eq
	strb	w1, [x19, 14]
	b	.L790
	.p2align 3
.L824:
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L796
	.p2align 3
.L819:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L787
	.p2align 3
.L818:
	add	x1, x21, 508
	mov	x0, x20
	bl	VC1_DecBfraction
	strb	w0, [x19, 2]
	b	.L786
	.p2align 3
.L817:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L785
	.p2align 3
.L816:
	mov	w1, w26
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L784
	.p2align 3
.L825:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L798
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L798
	.p2align 3
.L823:
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 28]
	mov	w1, w22
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 29]
	b	.L795
	.p2align 3
.L822:
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	b	.L792
	.p2align 3
.L821:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L791
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
	ble	.L847
	cmp	w0, 11
	bgt	.L859
	mov	w24, 2
	mov	w1, w24
.L827:
	add	x20, x21, 360
	mov	x0, x19
	add	x22, x21, 16
	bl	BsSkip
	strb	w24, [x20, 1]
	ldrb	w0, [x23, 3545]
	strb	w0, [x23, 2]
	ldrb	w0, [x22, 8]
	cbnz	w0, .L860
.L828:
	ldrb	w0, [x22, 6]
	cbz	w0, .L829
	ldrb	w0, [x22, 7]
	cbnz	w0, .L830
	mov	w1, 2
	mov	x0, x19
	bl	BsGet
	strb	w0, [x20, 6]
	ldrb	w0, [x22, 7]
	cmp	w0, 1
	ldrb	w0, [x22, 6]
	beq	.L834
.L863:
	cmp	w0, 1
	bne	.L833
	ldrb	w0, [x20, 6]
	add	w0, w0, 1
	str	w0, [x20, 144]
.L836:
	ldrb	w1, [x21, 190]
	cmp	w1, 1
	beq	.L837
.L845:
	ldrb	w0, [x20, 1]
	cmp	w0, 5
	bls	.L861
.L851:
	mov	w0, 0
	b	.L838
	.p2align 3
.L861:
	ldr	x1, .LC123
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx840
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx840:
	.section	.rodata
	.align	0
	.align	2
.L840:
	.byte	(.L843 - .Lrtx840) / 4
	.byte	(.L841 - .Lrtx840) / 4
	.byte	(.L842 - .Lrtx840) / 4
	.byte	(.L851 - .Lrtx840) / 4
	.byte	(.L843 - .Lrtx840) / 4
	.byte	(.L844 - .Lrtx840) / 4
	.text
	.p2align 3
.L859:
	cmp	w0, 13
	ble	.L849
	cmp	w0, 14
	beq	.L862
	mov	w24, 5
	mov	w1, 4
	b	.L827
	.p2align 3
.L843:
	mov	x2, x21
	mov	x1, x23
	mov	x0, x19
	bl	VC1_Vfmw_ParseAdvIPicHdr
.L838:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L829:
	mov	w0, 1
	strb	w0, [x20, 4]
	ldrb	w0, [x22, 7]
	cmp	w0, 1
	beq	.L832
.L833:
	mov	w0, 1
	str	w0, [x20, 144]
	ldrb	w1, [x21, 190]
	cmp	w1, 1
	bne	.L845
.L837:
	mov	x0, x19
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 7]
	bne	.L845
	ldr	w0, [x20, 144]
	cmp	w0, wzr
	ble	.L845
	mov	w24, 0
	.p2align 2
.L846:
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
	bgt	.L846
	b	.L845
	.p2align 3
.L832:
	mov	w0, 2
	str	w0, [x20, 144]
	b	.L836
	.p2align 3
.L847:
	mov	w24, 1
	mov	w1, w24
	b	.L827
	.p2align 3
.L860:
	mov	w1, 8
	mov	x0, x19
	bl	BsGet
	str	w0, [x20, 48]
	b	.L828
	.p2align 3
.L830:
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
	bne	.L863
.L834:
	cmp	w0, 1
	bne	.L832
	ldrb	w0, [x20, 5]
	add	w0, w0, 2
	str	w0, [x20, 144]
	b	.L836
	.p2align 3
.L841:
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
.L844:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L842:
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
.L849:
	mov	w24, 0
	mov	w1, 3
	b	.L827
	.p2align 3
.L862:
	mov	w24, 4
	mov	w1, w24
	b	.L827
	.size	VC1_Vfmw_ParseAdvFramePicHdr, .-VC1_Vfmw_ParseAdvFramePicHdr
	.align	3
.LC123:
	.xword	.L840
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
	cbz	w22, .L872
	add	x24, x21, 188
	ldrb	w23, [x24, 10]
	uxtb	w1, w0
	mov	w0, w23
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L878
	strb	wzr, [x19, 3]
.L868:
	cmp	w23, 1
	beq	.L879
	strb	wzr, [x19, 11]
.L870:
	ldrb	w0, [x21, 21]
	cmp	w0, 1
	beq	.L880
.L871:
	mov	w3, 0
	mov	x2, x21
	mov	x1, x25
	mov	x0, x20
	bl	BPD_Drv
	cmp	w0, 1
	beq	.L872
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
	cbz	w1, .L866
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
.L872:
	mov	w22, 0
.L866:
	mov	w0, w22
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L878:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w23, [x24, 10]
	b	.L868
	.p2align 3
.L880:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L871
	.p2align 3
.L879:
	mov	w1, w23
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L870
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
	cbz	w22, .L882
	add	x21, x23, 188
	ldrb	w24, [x21, 10]
	mov	w1, w22
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L909
	strb	wzr, [x19, 3]
.L884:
	cmp	w24, 1
	beq	.L910
	strb	wzr, [x19, 11]
.L886:
	ldrb	w0, [x23, 21]
	cmp	w0, 1
	beq	.L911
.L887:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	strb	w0, [x19, 32]
	cbz	w0, .L912
.L888:
	ldrb	w0, [x21, 6]
	cmp	w0, 1
	beq	.L913
.L889:
	ldrb	w0, [x21, 12]
	cmp	w0, 1
	beq	.L914
.L890:
	ldrb	w1, [x19, 116]
	mov	x0, x20
	bl	VC1_DecMvmodeProgrP
	uxtb	w0, w0
	cmp	w0, 4
	strb	w0, [x19, 14]
	beq	.L915
.L892:
	mov	w1, 3
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 27]
	ldrb	w1, [x19, 32]
	cbnz	w1, .L894
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
	cbz	w1, .L896
.L918:
	ldrh	w0, [x19, 14]
	cmp	w0, 4
	beq	.L896
	ldrb	w1, [x21, 7]
	cbnz	w1, .L916
.L898:
	ldrb	w1, [x21, 8]
	cmp	w1, 1
	beq	.L917
.L900:
	mov	x0, x20
	bl	VC1_DecTransacfrm
	strb	w0, [x19, 22]
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 24]
	mov	w1, 1
.L882:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L894:
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
	cbnz	w1, .L918
.L896:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 29]
	ldrb	w1, [x21, 7]
	cbz	w1, .L898
.L916:
	ldrb	w2, [x19, 116]
	add	x3, x23, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L898
	.p2align 3
.L909:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w24, [x21, 10]
	b	.L884
	.p2align 3
.L912:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	cmp	w0, wzr
	cset	w0, eq
	strb	w0, [x19, 33]
	b	.L888
	.p2align 3
.L915:
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
	cbnz	w1, .L892
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 132]
	mov	w1, 6
	mov	x0, x20
	bl	BsGet
	str	w0, [x19, 136]
	b	.L892
	.p2align 3
.L914:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L890
	.p2align 3
.L917:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L900
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L900
	.p2align 3
.L913:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L889
	.p2align 3
.L911:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L887
	.p2align 3
.L910:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L886
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
	cbz	w22, .L929
	add	x23, x21, 188
	ldrb	w24, [x23, 10]
	uxtb	w1, w0
	mov	w0, w24
	bl	VC1_Vfmw_DecPQUANT
	str	w0, [x19, 116]
	cmp	w22, 8
	bls	.L941
	strb	wzr, [x19, 3]
.L923:
	cmp	w24, 1
	beq	.L942
	strb	wzr, [x19, 11]
.L925:
	ldrb	w0, [x21, 21]
	cmp	w0, 1
	beq	.L943
.L926:
	ldrb	w0, [x23, 6]
	cmp	w0, 1
	beq	.L944
.L927:
	ldrb	w0, [x23, 12]
	cmp	w0, 1
	beq	.L945
.L928:
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
	beq	.L929
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
	cbz	w1, .L946
.L930:
	ldrb	w1, [x23, 7]
	cbnz	w1, .L947
.L931:
	ldrb	w1, [x23, 8]
	cmp	w1, 1
	beq	.L948
.L933:
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
.L929:
	mov	w1, 0
	ldr	x25, [sp, 64]
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L941:
	mov	w1, 1
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 3]
	ldrb	w24, [x23, 10]
	b	.L923
	.p2align 3
.L947:
	ldrb	w2, [x19, 116]
	add	x3, x21, 520
	mov	x0, x20
	bl	VC1_VopDQuant
	b	.L931
	.p2align 3
.L946:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 29]
	b	.L930
	.p2align 3
.L945:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 25]
	b	.L928
	.p2align 3
.L944:
	mov	x0, x20
	bl	VC1_DecMvrange
	strb	w0, [x19, 13]
	b	.L927
	.p2align 3
.L943:
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 12]
	b	.L926
	.p2align 3
.L942:
	mov	w1, w24
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 11]
	b	.L925
	.p2align 3
.L948:
	mov	x0, x20
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 20]
	bne	.L933
	mov	w1, 2
	mov	x0, x20
	bl	BsGet
	strb	w0, [x19, 21]
	b	.L933
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
	cbz	w0, .L950
	add	x0, x20, 20480
	ldr	w0, [x0, 3628]
	cbnz	w0, .L950
	add	x19, x22, 360
	ldr	w0, [x19, 156]
	strb	w0, [x19, 1]
	ldrb	w0, [x20, 3545]
	strb	w0, [x20, 2]
.L969:
	ldrb	w0, [x19, 1]
	cmp	w0, 4
	bls	.L990
.L978:
	ldrb	w1, [x19, 4]
	mov	w0, 0
	cbz	w1, .L976
.L995:
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
.L950:
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
	cbnz	w0, .L991
	ldrb	w0, [x23, 6]
	cbnz	w0, .L992
.L955:
	mov	w0, 1
	strb	w0, [x19, 4]
	ldrb	w0, [x23, 7]
	cmp	w0, 1
	beq	.L958
.L959:
	mov	w0, 1
	str	w0, [x19, 144]
.L962:
	add	x23, x22, 188
	ldrb	w1, [x23, 2]
	cmp	w1, 1
	beq	.L963
.L966:
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
	beq	.L993
.L965:
	ldr	w0, [x19, 124]
	cmp	w0, 3
	ble	.L969
.L970:
	add	x1, x22, 508
	mov	x0, x21
	bl	VC1_DecBfraction
	strb	w0, [x19, 2]
	ldrb	w0, [x19, 1]
	cmp	w0, 4
	bhi	.L978
	.p2align 2
.L990:
	ldr	x1, .LC125
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx973
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx973:
	.section	.rodata
	.align	0
	.align	2
.L973:
	.byte	(.L972 - .Lrtx973) / 4
	.byte	(.L974 - .Lrtx973) / 4
	.byte	(.L975 - .Lrtx973) / 4
	.byte	(.L978 - .Lrtx973) / 4
	.byte	(.L972 - .Lrtx973) / 4
	.text
	.p2align 3
.L992:
	ldrb	w0, [x23, 7]
	cbz	w0, .L994
	mov	w1, 1
	mov	x0, x21
	bl	BsGet
	strb	w0, [x19, 4]
	mov	w1, 1
	mov	x0, x21
	bl	BsGet
	strb	w0, [x19, 5]
.L957:
	ldrb	w0, [x23, 7]
	cmp	w0, 1
	ldrb	w0, [x23, 6]
	beq	.L960
	cmp	w0, 1
	bne	.L959
	ldrb	w0, [x19, 6]
	add	w0, w0, 1
	str	w0, [x19, 144]
	b	.L962
	.p2align 3
.L975:
	mov	x2, x22
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvBFieldHdr
.L971:
	ldrb	w1, [x19, 4]
	cbnz	w1, .L995
.L976:
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
.L974:
	mov	x2, x22
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvPFieldHdr
	b	.L971
	.p2align 3
.L972:
	mov	x2, x22
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvIFieldHdr
	b	.L971
	.p2align 3
.L991:
	mov	w1, 8
	mov	x0, x21
	bl	BsGet
	str	w0, [x19, 48]
	ldrb	w0, [x23, 6]
	cbz	w0, .L955
	b	.L992
	.p2align 3
.L994:
	mov	w1, 2
	mov	x0, x21
	bl	BsGet
	strb	w0, [x19, 6]
	b	.L957
	.p2align 3
.L993:
	ldr	w0, [x19, 124]
	cmp	w0, 3
	bgt	.L970
	mov	x0, x21
	bl	VC1_DecRefdist
	strb	w0, [x19, 31]
	b	.L965
	.p2align 3
.L958:
	mov	w0, 2
	str	w0, [x19, 144]
	b	.L962
	.p2align 3
.L960:
	cmp	w0, 1
	bne	.L958
	ldrb	w0, [x19, 5]
	add	w0, w0, 2
	str	w0, [x19, 144]
	b	.L962
	.p2align 3
.L963:
	mov	x0, x21
	bl	BsGet
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 7]
	bne	.L966
	ldr	w0, [x19, 144]
	cmp	w0, wzr
	ble	.L966
	mov	w25, 0
	.p2align 2
.L967:
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
	bgt	.L967
	b	.L966
	.size	VC1_Vfmw_ParseAdvFieldPicHdr, .-VC1_Vfmw_ParseAdvFieldPicHdr
	.align	3
.LC124:
	.xword	.LANCHOR1
	.align	3
.LC125:
	.xword	.L973
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
	cbz	w0, .L997
	ldr	w0, [x2, 3628]
	cbz	w0, .L1007
.L997:
	ldrb	w23, [x20, 23]
	cmp	w23, 1
	beq	.L1008
	mov	w1, 0
	mov	w0, 0
.L999:
	str	w0, [x20, 404]
	mov	x2, x20
	cbz	w1, .L1009
.L1001:
	mov	x1, x19
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvFieldPicHdr
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1007:
	ldr	w1, [x20, 404]
	mov	x2, x20
	and	w1, w1, -3
	cbnz	w1, .L1001
.L1009:
	mov	x1, x19
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvFramePicHdr
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1008:
	mov	w1, 2
	mov	x0, x21
	bl	BsShow
	cmp	w0, 1
	mov	w22, w0
	ble	.L1010
	mov	w1, 2
	mov	x0, x21
	bl	BsSkip
	and	w1, w22, -3
	mov	w0, w22
	b	.L999
	.p2align 3
.L1010:
	mov	w1, w23
	mov	x0, x21
	bl	BsSkip
	mov	w1, 0
	mov	w0, 0
	b	.L999
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
	beq	.L1014
	mov	w0, 1
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1014:
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
	beq	.L1016
	ldr	w1, [x19, 40]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
.L1016:
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
	cbz	w0, .L1021
	add	x24, x22, 20480
	mov	w20, 0
	ldr	w0, [x24, 3628]
	cbnz	w0, .L1031
	b	.L1020
	.p2align 3
.L1024:
	ldr	w0, [x24, 3628]
	cmp	w0, w20
	bls	.L1020
.L1031:
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
	cbz	x0, .L1024
	ldr	w0, [x21, 3668]
	cbz	w0, .L1024
	ldr	w1, [x21, 3684]
	mov	w0, w23
	bl	SM_ReleaseStreamSeg
	ldr	w0, [x24, 3628]
	cmp	w0, w20
	bhi	.L1031
.L1020:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1021:
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
	add	x0, x20, 2504
	ldr	w1, [x19, 784]
	add	x25, x0, 20480
	bl	VC1_ClearCurPic
	ldr	w1, [x25, 3620]
	ldr	w0, [x19, 784]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	ldr	w1, [x25, 3616]
	ldr	w0, [x19, 784]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	ldr	w1, [x25, 3612]
	ldr	w0, [x19, 784]
	bl	FSP_GetFsImagePtr
	cbz	x23, .L1069
	tbnz	w22, #31, .L1055
	mov	w0, 100
	cmp	w22, w0
	csel	w22, w22, w0, le
.L1038:
	mov	x0, x21
	str	w22, [x23, 160]
	bl	VC1_SetImgFormat
	ldrsb	w1, [x20, 2507]
	cmp	w1, 3
	beq	.L1070
	str	xzr, [x23, 560]
	str	xzr, [x23, 568]
	ldr	x0, [x21]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	beq	.L1040
.L1075:
	add	x1, x20, 2504
	ldrb	w0, [x1, 2]
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cbz	w0, .L1041
	add	x1, x1, 20480
	ldr	w2, [x1, 3608]
	cmp	w2, 1
	bls	.L1056
	ldr	w1, [x1, 3612]
.L1042:
	add	x0, x20, 2504
	cmp	w2, 1
	add	x0, x0, 20480
	cset	w22, hi
	str	w1, [x0, 3624]
.L1043:
	add	x24, x20, 2504
	cmn	w1, #1
	add	x0, x24, 36864
	ldr	w2, [x0, 1584]
	add	w2, w2, 1
	str	w2, [x0, 1584]
	beq	.L1044
.L1074:
	ldr	w0, [x19, 784]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L1071
	ldr	w2, [x0, 160]
	cbz	w2, .L1046
	ldr	x0, [x21]
	ldr	w3, [x0, 4]
	cmp	w2, w3
	bhi	.L1047
.L1046:
	add	x24, x20, 2504
	ldr	w0, [x19, 784]
	add	x25, x24, 20480
	ldr	w26, [x25, 3624]
	mov	w1, w26
	bl	FSP_GetRef
	mov	w3, w0
	ldr	x1, .LC132
	mov	w2, w26
	mov	w0, 12
	bl	dprint_vfmw
	ldr	w0, [x19, 784]
	mov	w2, 1
	ldr	w1, [x25, 3624]
	bl	FSP_SetDisplay
	cbnz	w22, .L1072
	ldr	x1, [x24, 23992]
	mov	w2, 1
	mov	w0, 2
	strb	w2, [x1, 1]
	strb	w0, [x24, 1]
.L1049:
	ldr	w0, [x19, 784]
	mov	x4, x23
	add	x3, x19, 176
	mov	x2, x21
	mov	w1, 1
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L1050
	ldr	w0, [x19, 784]
	add	x20, x20, 2504
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	str	xzr, [x23, 560]
	ldr	x1, [x23, 568]
	ldr	w0, [x19, 784]
	bl	FreeUsdByDec
	str	xzr, [x23, 568]
	add	x0, x20, 20480
	mov	w2, 0
	ldr	w1, [x0, 3624]
	ldr	w0, [x19, 784]
	bl	FSP_SetDisplay
	cbnz	w22, .L1073
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
.L1041:
	add	x2, x1, 20480
	ldr	w1, [x2, 3620]
	str	w1, [x2, 3624]
	cbz	x24, .L1057
	ldr	x0, [x23, 600]
	mov	w22, 0
	ldr	x3, [x24, 600]
	cmp	x3, x0
	bge	.L1043
	str	x3, [x23, 600]
	str	x0, [x24, 600]
	add	x24, x20, 2504
	add	x0, x24, 36864
	ldr	w1, [x2, 3624]
	ldr	w2, [x0, 1584]
	cmn	w1, #1
	add	w2, w2, 1
	str	w2, [x0, 1584]
	bne	.L1074
	.p2align 2
.L1044:
	mov	w0, 1
.L1037:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1070:
	ldrb	w0, [x20, 2513]
	cmp	w0, 1
	bne	.L1044
	str	xzr, [x23, 560]
	str	xzr, [x23, 568]
	ldr	x0, [x21]
	ldr	w0, [x0, 12]
	cmp	w0, 1
	bne	.L1075
	.p2align 2
.L1040:
	add	x0, x20, 2504
	mov	w22, 0
	add	x0, x0, 20480
	ldr	w1, [x0, 3620]
	str	w1, [x0, 3624]
	b	.L1043
	.p2align 3
.L1055:
	mov	w22, 0
	b	.L1038
	.p2align 3
.L1050:
	add	x20, x20, 2504
	ldrsb	w22, [x20, 1]
	cmp	w22, 3
	beq	.L1076
	cmp	w22, 1
	beq	.L1077
	cmp	w22, 2
	bne	.L1044
	ldr	x1, [x20, 23992]
	strb	wzr, [x1, 1]
	strb	wzr, [x20, 1]
	b	.L1037
	.p2align 3
.L1072:
	ldr	x0, [x24, 24008]
	strb	w22, [x0, 1]
	strb	w22, [x24, 1]
	b	.L1049
	.p2align 3
.L1047:
	ldr	x1, .LC134
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 784]
	ldr	x1, [x23, 560]
	bl	FreeUsdByDec
	str	xzr, [x23, 560]
	ldr	x1, [x23, 568]
	ldr	w0, [x19, 784]
	bl	FreeUsdByDec
	str	xzr, [x23, 568]
	add	x0, x24, 20480
	mov	w2, 0
	ldr	w1, [x0, 3624]
	ldr	w0, [x19, 784]
	bl	FSP_SetDisplay
	cbnz	w22, .L1078
	ldr	x1, [x24, 23992]
	mov	w0, 1
	strb	wzr, [x1, 1]
	b	.L1037
	.p2align 3
.L1056:
	mov	w1, -1
	b	.L1042
	.p2align 3
.L1073:
	ldr	x1, [x20, 24008]
	mov	w0, 0
	strb	wzr, [x1, 1]
	b	.L1037
	.p2align 3
.L1077:
	ldr	x1, [x20, 24008]
	mov	w0, w22
	strb	wzr, [x1, 1]
	strb	wzr, [x20, 1]
	b	.L1037
	.p2align 3
.L1076:
	ldr	x0, [x20, 24000]
	strb	wzr, [x0, 1]
	strb	wzr, [x20, 1]
	b	.L1044
	.p2align 3
.L1069:
	ldr	x1, .LC128
	mov	x4, x0
	mov	x3, x24
	mov	x2, 0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1037
	.p2align 3
.L1057:
	mov	w22, 0
	b	.L1043
	.p2align 3
.L1071:
	ldr	x1, .LC130
	mov	w2, 733
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1037
	.p2align 3
.L1078:
	ldr	x1, [x24, 24008]
	mov	w0, w22
	strb	wzr, [x1, 1]
	b	.L1037
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
	add	x3, x19, 2504
	ldrb	w0, [x19, 2504]
	tbz	x0, 4, .L1132
	mov	w2, -21
	add	x1, x3, 36864
	and	w0, w0, w2
	strb	w0, [x19, 2504]
	ldr	w0, [x1, 1716]
	sub	w0, w0, #2
	cmp	w0, 1
	ldr	x0, [x20]
	bls	.L1082
	add	x3, x3, 20480
	ldr	w2, [x0, 8]
	ldr	w3, [x3, 3608]
.L1083:
	add	x0, x19, 2504
	sub	w1, w2, #1
	add	x0, x0, 36864
	cmp	w1, 3
	str	w2, [x0, 1716]
	bls	.L1134
.L1110:
	mov	w4, 1
	.p2align 2
.L1089:
	cbz	w3, .L1100
.L1099:
	cmp	w3, 1
	bls	.L1135
.L1103:
	cbz	w4, .L1102
	add	x19, x19, 2504
	add	x20, x20, 16384
	ldrsb	w0, [x19, 3104]
	cbnz	w0, .L1105
	add	x1, x20, 1592
	add	x19, x19, 8192
	add	x0, x19, 2048
	add	x20, x20, 1840
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
.L1131:
	ldr	x0, [x20]
	ldr	w1, [x0, 8]
	add	x0, x19, 2504
	add	x0, x0, 36864
	sub	w2, w1, #1
	cmp	w2, 3
	str	w1, [x0, 1716]
	bls	.L1136
	mov	w4, 1
	.p2align 2
.L1100:
	ldrb	w0, [x19, 2506]
	sub	w2, w0, #1
	sub	w1, w0, #4
	uxtb	w2, w2
	uxtb	w1, w1
	cmp	w2, 1
	ccmp	w1, 1, 0, hi
	bls	.L1106
.L1101:
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cbnz	w0, .L1103
	ldr	x1, .LC141
	mov	w0, 1
	bl	dprint_vfmw
	.p2align 2
.L1102:
	add	x20, x20, 53248
	add	x0, x19, 2504
	ldr	w1, [x20, 784]
	bl	VC1_ClearCurPic
	ldr	x0, .LC143
	ldr	x4, [x0]
	cbz	x4, .L1132
	ldr	w0, [x20, 784]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L1132:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1134:
	ldr	x0, .LC137
	ldrb	w0, [x0,w1,uxtw]
	adr	x1, .Lrtx1091
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1091:
	.section	.rodata
	.align	0
	.align	2
.L1091:
	.byte	(.L1090 - .Lrtx1091) / 4
	.byte	(.L1092 - .Lrtx1091) / 4
	.byte	(.L1093 - .Lrtx1091) / 4
	.byte	(.L1094 - .Lrtx1091) / 4
	.text
	.p2align 3
.L1082:
	ldr	w2, [x0, 8]
	cmp	w2, 1
	ccmp	w2, 4, 4, hi
	beq	.L1084
	add	x3, x3, 20480
	ldr	w3, [x3, 3608]
	b	.L1083
	.p2align 3
.L1093:
	mov	w4, 0
	b	.L1089
	.p2align 3
.L1092:
	ldrb	w0, [x19, 2506]
	cmp	w0, wzr
	cset	w4, eq
	b	.L1089
	.p2align 3
.L1094:
	add	x0, x19, 2504
	ldrb	w1, [x0, 2]
	cbnz	w1, .L1095
	add	x0, x0, 36864
	mov	w4, 1
	str	wzr, [x0, 1648]
	b	.L1089
	.p2align 3
.L1090:
	ldrb	w0, [x19, 2506]
	sub	w0, w0, #2
	and	w0, w0, -3
	uxtb	w0, w0
	cmp	w0, wzr
	cset	w4, ne
	b	.L1089
	.p2align 3
.L1095:
	cmp	w1, 2
	bne	.L1097
	add	x0, x0, 36864
	ldr	w0, [x0, 1648]
	cbnz	w0, .L1110
	mov	w4, 0
	cbnz	w3, .L1099
	.p2align 2
.L1106:
	ldr	x1, .LC139
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1102
	.p2align 3
.L1105:
	add	x21, x19, 8192
	add	x1, x20, 1608
	mov	x2, 172
	add	x0, x21, 2224
	bl	memcpy
	add	x4, x20, 1780
	add	x3, x21, 2396
	mov	x2, 176
	ldp	x0, x1, [x4]
	stp	x0, x1, [x3]
	add	x1, x20, 1952
	add	x0, x21, 2568
	ldp	x6, x7, [x4, 16]
	stp	x6, x7, [x3, 16]
	ldp	x6, x7, [x4, 32]
	stp	x6, x7, [x3, 32]
	ldr	x5, [x4, 48]
	str	x5, [x3, 48]
	ldr	w4, [x20, 1836]
	str	w4, [x19, 10644]
	bl	memcpy
	ldr	w0, [x20, 1996]
	add	x3, x19, 20480
	strb	w0, [x19, 4]
	add	x1, x20, 2128
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
.L1097:
	and	w1, w1, -5
	uxtb	w1, w1
	cmp	w1, 1
	bne	.L1110
	add	x0, x0, 36864
	mov	w4, w1
	str	w1, [x0, 1648]
	cbz	w3, .L1106
	b	.L1099
	.p2align 3
.L1084:
	ldr	x1, .LC136
	add	x21, x3, 20480
	mov	w0, 2
	add	x23, x20, 53248
	bl	dprint_vfmw
	ldr	w1, [x21, 3616]
	ldr	w0, [x23, 784]
	bl	FSP_GetFsImagePtr
	mov	x22, x0
	cbz	x0, .L1085
	ldr	w0, [x23, 784]
	ldr	x1, [x22, 560]
	bl	FreeUsdByDec
	ldr	w0, [x23, 784]
	ldr	x1, [x22, 568]
	bl	FreeUsdByDec
	ldr	w0, [x23, 784]
	ldr	x1, [x22, 576]
	bl	FreeUsdByDec
	ldr	w0, [x23, 784]
	ldr	x1, [x22, 584]
	bl	FreeUsdByDec
	add	x0, x22, 512
	mov	w2, 1
	stp	xzr, xzr, [x0, 48]
	stp	xzr, xzr, [x0, 64]
	ldr	w1, [x21, 3616]
	ldr	w0, [x23, 784]
	bl	FSP_ClearLogicFs
	ldr	w1, [x21, 3612]
	mov	w2, 1
	ldr	w0, [x23, 784]
	bl	FSP_ClearLogicFs
	str	wzr, [x21, 3612]
	str	wzr, [x21, 3616]
	ldr	w0, [x23, 784]
	bl	FSP_GetNullLogicFs
	str	w0, [x21, 3612]
	str	w0, [x21, 3616]
.L1085:
	add	x0, x19, 2504
	add	x1, x0, 20480
	str	wzr, [x1, 3608]
	ldr	x2, [x0, 24000]
	cbz	x2, .L1131
	ldr	x3, [x0, 24008]
	cbz	x3, .L1131
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
	b	.L1083
.L1136:
	ldr	x0, .LC144
	mov	w3, 0
	ldrh	w0, [x0,w2,uxtw #1]
	adr	x1, .Lrtx1109
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx1109:
	.section	.rodata
	.align	0
	.align	2
.L1109:
	.2byte	(.L1090 - .Lrtx1109) / 4
	.2byte	(.L1092 - .Lrtx1109) / 4
	.2byte	(.L1093 - .Lrtx1109) / 4
	.2byte	(.L1094 - .Lrtx1109) / 4
	.text
.L1135:
	ldrb	w0, [x19, 2506]
	b	.L1101
	.size	VC1_Vfmw_DecodeArbiter, .-VC1_Vfmw_DecodeArbiter
	.align	3
.LC136:
	.xword	.LC135
	.align	3
.LC137:
	.xword	.L1091
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
	.xword	.L1109
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
	add	x21, x22, 2504
	ldr	x0, [x20]
	ldr	w1, [x0, 592]
	str	w1, [x19, 14596]
	ldr	w0, [x0, 632]
	str	w0, [x19, 14600]
	ldrsb	w0, [x21, 3104]
	cbz	w0, .L1152
.L1138:
	mov	x1, x19
	mov	x0, x20
	add	x23, x22, 2504
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
	ldrb	w0, [x22, 2506]
	cmp	w0, 1
	bls	.L1153
.L1139:
	cmp	w0, 5
	beq	.L1154
	strb	wzr, [x19, 1]
	add	x23, x22, 2504
	ldrsb	w1, [x22, 2508]
	ldrb	w0, [x22, 2512]
	strb	w1, [x20, 16]
	strb	w1, [x23, 3]
	strb	w0, [x23, 9]
	ldrb	w1, [x19, 3]
	cmp	w1, 2
	beq	.L1155
.L1142:
	and	w0, w1, -5
	uxtb	w0, w0
	cmp	w0, 1
	bne	.L1143
	add	x23, x23, 36864
	str	w0, [x23, 1648]
.L1143:
	ldr	x2, [x20]
	str	x19, [x20, 8]
	ldr	w0, [x2, 864]
	cmp	w0, 1
	beq	.L1156
	cmp	w0, 2
	beq	.L1157
.L1147:
	mov	w0, 1
.L1144:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1157:
	ldr	w0, [x2, 868]
	cmp	w0, wzr
	ccmp	x19, xzr, 4, gt
	beq	.L1147
	sub	w1, w1, #2
	mov	w0, 1
	and	w1, w1, -3
	uxtb	w1, w1
	cbnz	w1, .L1144
	ldr	x1, [x22, 26496]
	add	x3, x20, 53248
	mov	x0, x21
	strb	wzr, [x1, 1]
	ldr	x2, [x20]
	ldr	w1, [x2, 868]
	sub	w1, w1, #1
	str	w1, [x2, 868]
	ldr	w1, [x3, 784]
	bl	VC1_ClearCurPic
	str	xzr, [x20, 8]
	mov	w0, 0
	b	.L1144
	.p2align 3
.L1154:
	mov	w3, 1
	strb	w3, [x19, 1]
	add	x2, x22, 2504
	strb	w3, [x22, 2512]
	ldrsb	w1, [x22, 2509]
	add	x4, x2, 36864
	strb	w1, [x22, 2508]
	add	x23, x22, 2504
	mov	w0, 1
	ldr	w2, [x4, 1672]
	sub	w2, w2, #1
	str	w2, [x19, 14504]
	strb	w1, [x20, 16]
	strb	w1, [x23, 3]
	strb	w0, [x23, 9]
	ldrb	w1, [x19, 3]
	cmp	w1, 2
	bne	.L1142
.L1155:
	add	x24, x23, 36864
	ldr	w0, [x24, 1648]
	cbnz	w0, .L1143
	add	x1, x20, 53248
	mov	x0, x21
	ldr	w1, [x1, 784]
	bl	VC1_ClearCurPic
	ldr	x1, [x23, 23992]
	mov	w0, 0
	strb	wzr, [x1]
	ldr	w1, [x24, 1652]
	add	w1, w1, 1
	str	w1, [x24, 1652]
	str	xzr, [x20, 8]
	b	.L1144
	.p2align 3
.L1153:
	ldrb	w1, [x22, 2508]
	strb	w1, [x22, 2509]
	b	.L1139
	.p2align 3
.L1156:
	cbz	x19, .L1144
	sub	w1, w1, #2
	and	w1, w1, -3
	uxtb	w1, w1
	cbnz	w1, .L1144
	ldr	x2, [x22, 26496]
	add	x1, x20, 53248
	mov	x0, x21
	strb	wzr, [x2, 1]
	ldr	w1, [x1, 784]
	bl	VC1_ClearCurPic
	str	xzr, [x20, 8]
	mov	w0, 0
	b	.L1144
	.p2align 3
.L1152:
	mov	x0, x21
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
	b	.L1138
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
	bhi	.L1159
	cmp	w26, 11
	beq	.L1226
	ldrb	w1, [x20]
	and	w2, w1, 7
	cmp	w2, 7
	beq	.L1227
	add	x27, x20, 20480
	strb	wzr, [x20, 8]
	ldr	x2, .LC153
	str	wzr, [x27, 3628]
	ldrh	w0, [x2,w0,uxtw #1]
	adr	x2, .Lrtx1175
	add	x0, x2, w0, sxth #2
	br	x0
.Lrtx1175:
	.section	.rodata
	.align	0
	.align	2
.L1175:
	.2byte	(.L1174 - .Lrtx1175) / 4
	.2byte	(.L1191 - .Lrtx1175) / 4
	.2byte	(.L1176 - .Lrtx1175) / 4
	.2byte	(.L1193 - .Lrtx1175) / 4
	.2byte	(.L1178 - .Lrtx1175) / 4
	.2byte	(.L1179 - .Lrtx1175) / 4
	.text
	.p2align 3
.L1159:
	sub	w0, w26, #27
	cmp	w0, 4
	bls	.L1192
	cmp	w26, 254
	beq	.L1228
.L1191:
	mov	w0, w24
	mov	x1, x23
	bl	VC1_ClearCurPacket
	mov	w0, 0
.L1164:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1226:
	ldrb	w0, [x20]
	and	w0, w0, 7
	cmp	w0, 7
	bne	.L1191
	mov	x0, x21
	mov	x2, x25
	mov	x1, x20
	bl	VC1_Vfmw_ParseAdvSlcHdr
	cmp	w0, 1
	mov	w21, w0
	bne	.L1162
	add	x27, x20, 20480
	ldr	w0, [x27, 3628]
	cmp	w0, 255
	bls	.L1163
	mov	w1, w24
	mov	x0, x20
	bl	VC1_ClearCurPic
	ldr	x1, .LC147
	mov	w0, w21
	ldr	w2, [x27, 3628]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1164
	.p2align 3
.L1228:
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
	bne	.L1191
	ldrb	w0, [x20]
	mov	w1, 1
	strb	w1, [x20, 7]
	and	w1, w0, 7
	cmp	w1, 7
	bne	.L1192
	orr	w0, w0, 16
	strb	w0, [x20]
.L1192:
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
.L1193:
	ldr	x26, .LC154
.L1221:
	ldr	x3, [x26, 96]
	mov	x2, 176
	mov	w1, 0
	add	x0, x25, 360
	ldrb	w28, [x25, 391]
	blr	x3
	strb	w28, [x25, 391]
.L1180:
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
	beq	.L1229
	mov	x1, x23
	mov	w0, w24
	bl	VC1_ClearCurPacket
	ldr	x1, .LC156
	mov	w2, 6128
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w26
	b	.L1164
	.p2align 3
.L1227:
	mov	w0, 1
	orr	w1, w1, 16
	strb	w0, [x20, 6]
	mov	w0, 1
	strb	w1, [x20]
	b	.L1164
	.p2align 3
.L1174:
	orr	w0, w1, 32
	mov	x1, x23
	strb	w0, [x20]
	mov	w0, w24
	bl	VC1_ClearCurPacket
	mov	w0, 1
	b	.L1164
	.p2align 3
.L1178:
	mov	x2, x25
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvEntptHdr
	cmp	w0, 1
	mov	w19, w0
	beq	.L1230
.L1189:
	mov	w0, w24
	mov	x1, x23
	bl	VC1_ClearCurPacket
	mov	w0, w19
	b	.L1164
	.p2align 3
.L1179:
	mov	x2, x25
	mov	x1, x20
	mov	x0, x21
	bl	VC1_Vfmw_ParseAdvSeqHdr
	cmp	w0, 1
	mov	w19, w0
	bne	.L1189
	ldrb	w0, [x20]
	orr	w0, w0, 1
	strb	w0, [x20]
	b	.L1189
	.p2align 3
.L1176:
	ldr	x26, .LC154
	mov	w0, 1
	strb	w0, [x20, 8]
	mov	x2, 16
	mov	w1, 0
	add	x0, x25, 520
	ldr	x3, [x26, 96]
	blr	x3
	ldrb	w0, [x20, 8]
	cbnz	w0, .L1180
	b	.L1221
	.p2align 3
.L1162:
	mov	x1, x23
	mov	w0, w24
	bl	VC1_ClearCurPacket
	ldr	x1, .LC152
	mov	w2, 6039
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w21
	b	.L1164
	.p2align 3
.L1229:
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
	bge	.L1182
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
	cbz	x1, .L1183
	ldr	w0, [x22, 24]
	cbz	w0, .L1183
	str	x1, [x21, 8]
	str	wzr, [x21, 40]
	ldr	w0, [x22, 32]
	lsl	w0, w0, 3
	str	w0, [x21, 44]
	ldr	w0, [x22, 24]
	str	w0, [x21, 36]
	ldr	w0, [x22, 40]
	str	w0, [x21, 52]
.L1184:
	ldr	w4, [x27, 3544]
	ldr	w0, [x27, 3548]
	ldrb	w2, [x20, 8]
	mul	w4, w0, w4
	sub	w4, w4, #1
	cbz	w2, .L1185
	add	w0, w0, 1
	ldr	w3, [x23, 4]
	add	w0, w0, w0, lsr 31
	sub	w3, w3, w0, asr 1
	str	w3, [x21, 32]
	cmp	w3, wzr
	ccmp	w4, w3, 2, gt
	bcc	.L1231
.L1168:
	ldr	w0, [x21, 28]
	cmp	w0, wzr
	ble	.L1171
	cbz	x1, .L1187
	ldr	w0, [x21, 44]
	cmp	w0, wzr
	ble	.L1171
.L1187:
	ldr	w1, [x27, 3628]
	mov	w0, 1
	add	w1, w1, w0
	str	w1, [x27, 3628]
	b	.L1164
	.p2align 3
.L1182:
	mov	x1, x23
	mov	w0, w24
	bl	VC1_ClearCurPacket
	ldr	x1, .LC156
	mov	w2, 6122
	mov	w0, w26
	bl	dprint_vfmw
	mov	w0, w26
	b	.L1164
	.p2align 3
.L1163:
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
	cbz	x1, .L1165
	ldr	w0, [x23, 24]
	cbz	w0, .L1165
	str	x1, [x21, 8]
	str	wzr, [x21, 40]
	ldr	w0, [x23, 32]
	lsl	w0, w0, 3
	str	w0, [x21, 44]
	ldr	w0, [x23, 24]
	str	w0, [x21, 36]
	ldr	w0, [x23, 40]
	str	w0, [x21, 52]
.L1166:
	ldr	w4, [x27, 3544]
	ldr	w0, [x27, 3548]
	ldrb	w2, [x20, 8]
	mul	w4, w0, w4
	sub	w4, w4, #1
	cbz	w2, .L1167
	add	w0, w0, 1
	ldr	w3, [x22, 4]
	add	w0, w0, w0, lsr 31
	sub	w3, w3, w0, asr 1
	str	w3, [x21, 32]
	cmp	w3, wzr
	ccmp	w4, w3, 2, gt
	bcs	.L1168
	ldr	x1, .LC148
	mov	w0, 1
	ldr	w2, [x22, 4]
	ldr	w4, [x27, 3548]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1164
	.p2align 3
.L1183:
	mov	x1, 0
	str	xzr, [x21, 8]
	stp	wzr, wzr, [x21, 36]
	str	wzr, [x21, 44]
	str	wzr, [x21, 52]
	b	.L1184
.L1230:
	mov	x1, x25
	mov	x0, x20
	bl	VC1_Vfmw_UpdateAdvSPS
	ldrb	w0, [x20]
	orr	w0, w0, 2
	strb	w0, [x20]
	b	.L1189
.L1185:
	ldr	w3, [x23, 4]
	str	w3, [x21, 32]
	cmp	w4, w3
	bcs	.L1168
	ldr	x1, .LC149
	mov	w0, 1
	ldr	w2, [x23, 4]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1164
.L1165:
	mov	x1, 0
	str	xzr, [x21, 8]
	stp	wzr, wzr, [x21, 36]
	str	wzr, [x21, 44]
	str	wzr, [x21, 52]
	b	.L1166
.L1167:
	ldr	w3, [x22, 4]
	str	w3, [x21, 32]
	cmp	w4, w3
	bcs	.L1168
	ldr	x1, .LC149
	mov	w0, 1
	ldr	w2, [x22, 4]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1164
.L1171:
	ldr	x1, .LC150
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1164
.L1231:
	ldr	x1, .LC148
	mov	w0, 1
	ldr	w2, [x23, 4]
	ldr	w4, [x27, 3548]
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1164
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
	.xword	.L1175
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
	cbnz	w0, .L1233
	ldrb	w3, [x19]
	mov	x2, x20
	ldr	x0, [x19, 3136]
	mov	x1, x19
	orr	w3, w3, 3
	strb	w3, [x19]
	bl	VC1_Vfmw_ParseSMPPicHdr
	cmp	w0, 1
	beq	.L1235
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1233:
	mov	w2, w21
	mov	x1, x20
	mov	x0, x19
	bl	VC1_Vfmw_ParseAdvHdr
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1235:
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
	add	x2, x19, 2504
	add	x0, x2, 3144
	str	x0, [x19, 5640]
	ldrb	w1, [x2, 3106]
	cmp	w1, 8
	beq	.L1241
	strb	wzr, [x2, 12]
	strb	w1, [x2, 11]
.L1238:
	add	x20, x19, 2504
	ldr	w2, [x21, 28]
	ldrsb	w1, [x20, 3104]
	cbz	w1, .L1242
	ldr	x1, [x21]
	bl	BsInit
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1242:
	ldr	x1, [x21]
	bl	BsInit
	ldr	w1, [x19, 5616]
	add	x2, x20, 20480
	add	x3, x20, 3120
	add	x20, x20, 3184
	adds	w4, w1, 15
	add	w0, w1, 30
	csel	w0, w0, w4, mi
	asr	w0, w0, 4
	str	w0, [x2, 3544]
	ldr	w0, [x19, 5620]
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
.L1241:
	mov	w3, 1
	mov	w1, 6
	strb	w3, [x2, 12]
	strb	w1, [x2, 11]
	b	.L1238
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
	add	x1, x20, 53248
	cbz	x19, .L1251
	ldr	w0, [x19, 12]
	str	w0, [x1, 20]
	ldr	x0, [x19]
	str	x0, [x1]
	ldr	w0, [x19, 16]
	str	w0, [x1, 28]
	ldrb	w0, [x19, 9]
	strb	w0, [x1, 16]
	ldr	w0, [x19, 20]
	str	w0, [x1, 36]
	ldrb	w0, [x19, 9]
	cmp	w0, 1
	beq	.L1246
	ldr	w0, [x19, 92]
	str	w0, [x1, 24]
	ldr	x0, [x19, 80]
	str	x0, [x1, 8]
	ldr	w0, [x19, 96]
	str	w0, [x1, 32]
	ldrb	w0, [x19, 89]
	strb	w0, [x1, 17]
	ldr	w0, [x19, 100]
	str	w0, [x1, 40]
	ldrb	w0, [x19, 89]
	cmp	w0, 1
	bne	.L1252
.L1247:
	mov	x0, x20
	bl	VC1_SetCodeInfo
	mov	w0, 1
.L1245:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1252:
	ldr	w0, [x1, 784]
	bl	VC1_ClearCurPacket
	ldrb	w2, [x19, 89]
	mov	w0, 1
	ldr	x1, .LC160
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1246:
	strb	wzr, [x1, 17]
	str	xzr, [x1, 8]
	str	wzr, [x1, 24]
	str	wzr, [x1, 32]
	str	wzr, [x1, 40]
	b	.L1247
	.p2align 3
.L1251:
	ldr	x1, .LC158
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 0
	b	.L1245
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
	mov	x2, 160
	mov	x1, x22
	mov	x0, x21
	ldr	x3, [x3, 104]
	blr	x3
	mov	x1, x21
	mov	x0, x19
	bl	VC1_ReceivePacket
	mov	w21, w0
	cbz	w0, .L1280
	ldr	x0, [x19]
	add	x22, x19, 53248
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L1281
.L1256:
	ldrb	w0, [x20, 2504]
	strb	w0, [x22, 128]
.L1257:
	add	x20, x20, 2504
	add	x1, x19, 16384
	ldr	w2, [x22, 784]
	add	x1, x1, 1592
	mov	x0, x20
	bl	VC1_Vfmw_ParseHeader
	mov	w23, w0
	cbnz	w0, .L1258
	ldr	w0, [x22, 784]
	mov	x1, x22
	bl	VC1_ClearCurPacket
	ldr	x0, .LC164
	ldr	x4, [x0]
	cbz	x4, .L1260
	ldr	w0, [x22, 784]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	mov	w21, 0
	blr	x4
.L1277:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1258:
	mov	x0, x19
	bl	VC1_Vfmw_DecodeArbiter
	cbnz	w0, .L1282
.L1260:
	mov	w21, 0
.L1284:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1281:
	ldr	w0, [x0, 872]
	cmp	w0, 1
	bne	.L1256
	ldrb	w0, [x22, 128]
	strb	w0, [x20, 2504]
	b	.L1257
	.p2align 3
.L1280:
	add	x19, x19, 49152
	ldr	x1, .LC163
	ldr	w2, [x19, 4088]
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1282:
	mov	x0, x19
	bl	VC1_Vfmw_GetStreamInfo
	mov	w21, w0
	cbz	w0, .L1283
	mov	x0, x19
	bl	VC1_Vfmw_StartDecode
	mov	w21, w0
	cbz	w0, .L1264
	mov	w21, w23
	b	.L1277
	.p2align 3
.L1283:
	ldr	x1, .LC165
	bl	dprint_vfmw
	ldr	x0, [x19]
	ldr	w1, [x0, 832]
	cmp	w1, 1
	bne	.L1262
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L1263
.L1262:
	ldr	w1, [x22, 784]
	mov	x0, x20
	bl	VC1_ClearCurPic
.L1263:
	ldr	x0, .LC164
	ldr	x4, [x0]
	cbz	x4, .L1260
.L1278:
	ldr	w0, [x22, 784]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L1277
	.p2align 3
.L1264:
	add	x19, x19, 36864
	ldr	w0, [x22, 784]
	mov	w2, 1
	ldr	w1, [x19, 2028]
	bl	FSP_ClearLogicFs
	ldr	x1, [x19, 1920]
	ldr	x0, .LC164
	strb	wzr, [x1]
	ldr	x1, [x19, 1920]
	strb	wzr, [x1, 1]
	ldr	x4, [x0]
	cbnz	x4, .L1278
	mov	w21, 0
	b	.L1284
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
	ldrsb	w1, [x19, 2510]
	ldr	x19, [sp, 16]
	cmp	w1, 1
	csel	w0, w0, wzr, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	VC1_GetPacketState, .-VC1_GetPacketState
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	__func__.11638, %object
	.size	__func__.11638, 13
__func__.11638:
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
	.type	__func__.11949, %object
	.size	__func__.11949, 17
__func__.11949:
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
