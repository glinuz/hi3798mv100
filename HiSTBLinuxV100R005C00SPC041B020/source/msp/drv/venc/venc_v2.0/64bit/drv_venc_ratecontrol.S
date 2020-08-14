#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"drv_venc_ratecontrol.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcMinQPForVBR
	.type	VEDU_DRV_RCCalcMinQPForVBR, %function
VEDU_DRV_RCCalcMinQPForVBR:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 20]
	cbnz	w0, .L9
	ldr	w2, [x19, 84]
	mov	w3, 34079
	movk	w3, 0x51eb, lsl 16
	mov	w0, 200
	smull	x1, w2, w3
	asr	x1, x1, 38
	sub	w1, w1, w2, asr 31
	msub	w0, w1, w0, w2
	cbz	w0, .L4
	ldr	w1, [x19, 572]
.L5:
	cmp	w1, 19
	mov	w0, 20
	ble	.L6
	mov	w0, 30
	cmp	w1, w0
	csel	w0, w1, w0, le
.L6:
	str	w0, [x19, 572]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L4:
	ldr	w0, [x19, 560]
	ldr	w2, [x19, 572]
	add	w0, w0, 200
	str	wzr, [x19, 560]
	add	w1, w2, 1
	sub	w2, w2, #1
	umull	x0, w0, w3
	lsr	x0, x0, 32
	lsr	w0, w0, 6
	cmp	w0, w1
	csel	w1, w2, w1, cc
	b	.L5
	.p2align 3
.L9:
	ldr	w0, [x19, 176]
	add	w0, w0, 5
	str	w0, [x19, 572]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCCalcMinQPForVBR, .-VEDU_DRV_RCCalcMinQPForVBR
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCGetVBRMaxBitRate
	.type	VEDU_DRV_RCGetVBRMaxBitRate, %function
VEDU_DRV_RCGetVBRMaxBitRate:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 12]
	cmp	w0, 1280
	bhi	.L15
	cmp	w0, 720
	bhi	.L16
	cmp	w0, 352
	bhi	.L17
	mov	w0, 1536
	str	w0, [x19, 576]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L16:
	mov	w0, 5242880
	str	w0, [x19, 576]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L15:
	mov	w0, 8388608
	str	w0, [x19, 576]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L17:
	mov	w0, 3072
	str	w0, [x19, 576]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCGetVBRMaxBitRate, .-VEDU_DRV_RCGetVBRMaxBitRate
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCGetRecodingQP
	.type	VEDU_DRV_RCGetRecodingQP, %function
VEDU_DRV_RCGetRecodingQP:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 264]
	str	w0, [x19, 164]
	lsr	w1, w0, 6
	add	w0, w0, 128
	str	w1, [x19, 172]
	str	w0, [x19, 100]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCGetRecodingQP, .-VEDU_DRV_RCGetRecodingQP
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCInitInfo
	.type	VEDU_DRV_RCInitInfo, %function
VEDU_DRV_RCInitInfo:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldp	w2, w3, [x19, 36]
	ldr	w4, [x19, 28]
	mov	w5, 1
	lsl	w0, w2, 3
	ldr	w6, [x19, 76]
	add	w0, w0, w2, lsl 1
	mov	w7, 5
	udiv	w1, w2, w4
	lsr	w0, w0, 3
	str	w1, [x19, 92]
	stp	w7, w5, [x19, 200]
	stp	wzr, w0, [x19, 208]
	stp	w0, w5, [x19, 216]
	str	w4, [x19, 544]
	str	w3, [x19, 548]
	str	w2, [x19, 552]
	mul	w1, w1, w3
	str	w1, [x19, 88]
	cbz	w6, .L19
	str	wzr, [x19, 84]
.L19:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCInitInfo, .-VEDU_DRV_RCInitInfo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCInitInstantBits
	.type	VEDU_DRV_RCInitInstantBits, %function
VEDU_DRV_RCInitInstantBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC2
	add	x4, x19, 512
	ldr	w1, [x19, 28]
	mov	x2, 260
	ldr	x3, [x0]
	add	x0, x19, 268
	stp	w1, wzr, [x4, 16]
	stp	wzr, wzr, [x4, 24]
	mov	w1, 0
	blr	x3
	ldr	w1, [x19, 40]
	ldr	w3, [x19, 200]
	ldr	w0, [x19, 88]
	sub	w1, w1, #1
	add	w1, w1, w3
	ldr	w2, [x19, 532]
	ldr	w4, [x19, 528]
	udiv	w1, w0, w1
	cmp	w4, 1
	mul	w3, w3, w1
	str	w3, [x19, 268]
	add	w3, w3, w2
	str	w3, [x19, 532]
	bls	.L25
	sub	w2, w4, #2
	add	x0, x19, 272
	mov	x4, x2
	add	x2, x2, 69
	add	x2, x19, x2, lsl 2
	.p2align 2
.L26:
	str	w1, [x0], 4
	cmp	x0, x2
	bne	.L26
	madd	w1, w1, w4, w1
	add	w3, w3, w1
	str	w3, [x19, 532]
.L25:
	str	w3, [x19, 540]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCInitInstantBits, .-VEDU_DRV_RCInitInstantBits
	.align	3
.LC2:
	.xword	venc_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCInitFrameBits
	.type	VEDU_DRV_RCInitFrameBits, %function
VEDU_DRV_RCInitFrameBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 168]
	mov	w1, -1
	ldr	w2, [x19, 200]
	stp	w0, w0, [x19, 224]
	stp	w1, w1, [x19, 252]
	udiv	w0, w0, w2
	stp	w0, w0, [x19, 232]
	stp	w0, w0, [x19, 240]
	str	w0, [x19, 248]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCInitFrameBits, .-VEDU_DRV_RCInitFrameBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_EflRcInitQp
	.type	VEDU_DRV_EflRcInitQp, %function
VEDU_DRV_EflRcInitQp:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w2
	mov	w21, w1
	bl	_mcount
	lsl	w0, w20, 2
	lsl	w3, w19, 2
	sub	w2, w0, w20
	sub	w0, w3, w19
	lsl	w3, w0, 5
	mul	w1, w2, w21
	add	w0, w0, w3
	add	w19, w0, w19
	lsr	w1, w1, 1
	mov	w0, 7
	udiv	w19, w19, w1
	cmp	w19, 170
	bhi	.L30
	sub	w1, w19, #121
	mov	w0, 15
	cmp	w1, 49
	bls	.L30
	sub	w1, w19, #81
	mov	w0, 20
	cmp	w1, 39
	bls	.L30
	sub	w1, w19, #41
	mov	w0, 25
	cmp	w1, 39
	bls	.L30
	sub	w1, w19, #16
	mov	w0, 30
	cmp	w1, 24
	bls	.L30
	sub	w1, w19, #6
	mov	w0, 38
	cmp	w1, 9
	bls	.L30
	sub	w19, w19, #3
	mov	w0, 40
	mov	w1, 43
	cmp	w19, 3
	csel	w0, w0, w1, cc
.L30:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	VEDU_DRV_EflRcInitQp, .-VEDU_DRV_EflRcInitQp
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCModifyQPDeltaOfIP
	.type	VEDU_DRV_RCModifyQPDeltaOfIP, %function
VEDU_DRV_RCModifyQPDeltaOfIP:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w4, [x19, 88]
	ldr	w2, [x19, 40]
	lsl	w3, w4, 4
	ldr	w0, [x19, 24]
	add	w2, w2, 19
	add	w4, w3, w4, lsl 2
	ldr	w5, [x19, 208]
	udiv	w3, w4, w2
	cmp	w0, w3
	bhi	.L40
	cbz	w5, .L38
.L40:
	sub	w2, w0, w3
	mov	w1, -256
	lsl	w2, w2, 8
	ldr	w4, [x19, 96]
	mov	w6, 3264
	mov	w0, 256
	udiv	w2, w2, w3
	ldr	x3, .LC3
	cmp	w2, w1
	csel	w1, w2, w1, ge
	cmp	w4, wzr
	csel	w2, w4, wzr, ge
	cmp	w2, w6
	csel	w2, w2, w6, le
	cmp	w1, w0
	csel	w1, w1, w0, le
	lsr	w2, w2, 6
	add	w0, w5, w1, asr 6
	cmp	w0, wzr
	ldr	w1, [x3, w2, uxtw 2]
	csel	w0, w0, wzr, ge
	cmp	w1, w0
	csel	w0, w1, w0, le
	str	w0, [x19, 208]
.L38:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCModifyQPDeltaOfIP, .-VEDU_DRV_RCModifyQPDeltaOfIP
	.align	3
.LC3:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcQPDeltaByFrameBits
	.type	VEDU_DRV_RCCalcQPDeltaByFrameBits, %function
VEDU_DRV_RCCalcQPDeltaByFrameBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 24]
	ldr	w0, [x19, 104]
	cmp	w1, w0
	bcc	.L46
	sub	w2, w1, w0
	lsl	w2, w2, 6
	udiv	w0, w2, w0
.L47:
	cmn	w0, #128
	mov	w1, -128
	csel	w0, w0, w1, ge
	mov	w1, 256
	cmp	w0, w1
	ldr	x19, [sp, 16]
	csel	w0, w0, w1, le
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L46:
	sub	w1, w0, w1
	lsl	w2, w1, 5
	add	w1, w2, w1, lsl 3
	udiv	w0, w1, w0
	neg	w0, w0
	b	.L47
	.size	VEDU_DRV_RCCalcQPDeltaByFrameBits, .-VEDU_DRV_RCCalcQPDeltaByFrameBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcQPDeltaByInstantBits
	.type	VEDU_DRV_RCCalcQPDeltaByInstantBits, %function
VEDU_DRV_RCCalcQPDeltaByInstantBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w5, [x19, 540]
	mov	w6, 63
	ldr	w3, [x19, 36]
	ldr	w4, [x19, 532]
	lsl	w0, w5, 5
	ldr	x2, .LC4
	udiv	w0, w0, w3
	lsl	w1, w4, 5
	add	x2, x2, 208
	cmp	w0, wzr
	csel	w0, w0, wzr, ge
	udiv	w1, w1, w3
	cmp	w1, wzr
	csel	w1, w1, wzr, ge
	cmp	w1, w6
	csel	w1, w1, w6, le
	cmp	w0, w6
	csel	w0, w0, w6, le
	cmp	w4, w5
	lsr	w6, w3, 4
	ldr	w1, [x2, w1, sxtw 2]
	ldr	w0, [x2, w0, sxtw 2]
	sub	w2, w1, w0
	bls	.L49
	sub	w0, w3, w6
	cmp	w5, w0
	bls	.L49
	lsl	w1, w2, 2
	mov	w3, -128
	sub	w0, w1, w2
	mov	w1, 256
	lsl	w0, w0, 1
	cmp	w0, w3
	csel	w0, w0, w3, ge
	cmp	w0, w1
	csel	w0, w0, w1, le
.L50:
	str	w4, [x19, 540]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L49:
	cmp	w4, w5
	mov	w0, 0
	bcs	.L50
	add	w3, w3, w6
	cmp	w5, w3
	bcs	.L50
	lsl	w0, w2, 2
	mov	w2, -128
	cmp	w0, w2
	mov	w1, 256
	csel	w0, w0, w2, ge
	str	w4, [x19, 540]
	cmp	w0, w1
	csel	w0, w0, w1, le
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCCalcQPDeltaByInstantBits, .-VEDU_DRV_RCCalcQPDeltaByInstantBits
	.align	3
.LC4:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcQPDeltaByBitRate
	.type	VEDU_DRV_RCCalcQPDeltaByBitRate, %function
VEDU_DRV_RCCalcQPDeltaByBitRate:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w19, w20
	bls	.L54
	sub	w19, w19, w20
	lsl	w0, w19, 5
	udiv	w1, w0, w20
.L55:
	cmn	w1, #64
	mov	w0, -64
	csel	w1, w1, w0, ge
	mov	w0, 64
	cmp	w1, w0
	csel	w0, w1, w0, le
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L54:
	sub	w19, w20, w19
	lsl	w19, w19, 4
	udiv	w1, w19, w20
	neg	w1, w1
	b	.L55
	.size	VEDU_DRV_RCCalcQPDeltaByBitRate, .-VEDU_DRV_RCCalcQPDeltaByBitRate
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcQPDeltaByWaterline
	.type	VEDU_DRV_RCCalcQPDeltaByWaterline, %function
VEDU_DRV_RCCalcQPDeltaByWaterline:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldp	w2, w1, [x19, 212]
	mov	w0, -32
	mov	w3, 32
	sub	w1, w1, w2
	ldr	x19, [sp, 16]
	lsl	w1, w1, 2
	ldp	x29, x30, [sp], 32
	sdiv	w1, w1, w2
	cmp	w1, w0
	csel	w0, w1, w0, ge
	cmp	w0, w3
	csel	w0, w0, w3, le
	ret
	.size	VEDU_DRV_RCCalcQPDeltaByWaterline, .-VEDU_DRV_RCCalcQPDeltaByWaterline
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcVBRQPDeltaByInstantBits
	.type	VEDU_DRV_RCCalcVBRQPDeltaByInstantBits, %function
VEDU_DRV_RCCalcVBRQPDeltaByInstantBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 532]
	ldr	w2, [x19, 576]
	lsl	w1, w0, 2
	sub	w1, w1, w0
	lsl	w6, w2, 2
	sub	w5, w6, w2
	lsl	w4, w1, 5
	lsl	w3, w5, 5
	add	w1, w1, w4
	add	w1, w1, w0
	sub	w3, w3, w2
	cmp	w1, w3
	mov	w0, 192
	bls	.L62
.L58:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L62:
	lsl	w3, w5, 4
	mov	w0, 128
	sub	w5, w3, w5
	cmp	w1, w5, lsl 1
	bhi	.L58
	add	w2, w6, w2
	mov	w0, 64
	ldr	x19, [sp, 16]
	add	w2, w2, w2, lsl 4
	cmp	w1, w2
	csel	w0, wzr, w0, ls
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCCalcVBRQPDeltaByInstantBits, .-VEDU_DRV_RCCalcVBRQPDeltaByInstantBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcVBRQPDeltaByPicBits
	.type	VEDU_DRV_RCCalcVBRQPDeltaByPicBits, %function
VEDU_DRV_RCCalcVBRQPDeltaByPicBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w2, [x19, 84]
	cmp	w2, 200
	beq	.L64
	ldr	w3, [x19, 564]
	ldr	w4, [x19, 568]
	sub	w1, w4, w3
	cmp	w1, wzr
	ble	.L65
	lsl	w5, w1, 3
	mov	w0, 64
	add	w1, w5, w1, lsl 1
	cmp	w4, w1
	csel	w0, wzr, w0, ge
.L66:
	cmp	w2, 1000
	str	wzr, [x19, 564]
	str	w3, [x19, 568]
	bne	.L70
	str	wzr, [x19, 84]
.L70:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L64:
	ldr	w1, [x19, 564]
	mov	w0, 0
	str	wzr, [x19, 564]
	str	w1, [x19, 568]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L65:
	neg	w1, w1
	mov	w0, -64
	lsl	w5, w1, 3
	add	w1, w5, w1, lsl 1
	cmp	w4, w1
	csel	w0, wzr, w0, ge
	b	.L66
	.size	VEDU_DRV_RCCalcVBRQPDeltaByPicBits, .-VEDU_DRV_RCCalcVBRQPDeltaByPicBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCGetQPDeltaForVBR
	.type	VEDU_DRV_RCGetQPDeltaForVBR, %function
VEDU_DRV_RCGetQPDeltaForVBR:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, 192
	bl	_mcount
	ldr	w0, [x20, 532]
	ldr	w1, [x20, 576]
	lsl	w2, w0, 2
	sub	w2, w2, w0
	lsl	w6, w1, 2
	sub	w5, w6, w1
	lsl	w4, w2, 5
	lsl	w3, w5, 5
	add	w2, w2, w4
	sub	w3, w3, w1
	add	w0, w2, w0
	cmp	w0, w3
	bhi	.L72
	lsl	w2, w5, 4
	mov	w19, 128
	sub	w5, w2, w5
	cmp	w0, w5, lsl 1
	bls	.L78
.L72:
	ldr	w1, [x20, 84]
	mov	w2, 34079
	movk	w2, 0x51eb, lsl 16
	mov	w3, 200
	mov	w0, 0
	smull	x2, w1, w2
	asr	x2, x2, 38
	sub	w2, w2, w1, asr 31
	msub	w1, w2, w3, w1
	cbnz	w1, .L73
	mov	x0, x20
	bl	VEDU_DRV_RCCalcVBRQPDeltaByPicBits
.L73:
	add	w0, w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L78:
	add	w1, w6, w1
	mov	w19, 64
	add	w1, w1, w1, lsl 4
	cmp	w0, w1
	csel	w19, wzr, w19, ls
	b	.L72
	.size	VEDU_DRV_RCGetQPDeltaForVBR, .-VEDU_DRV_RCGetQPDeltaForVBR
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalculateCurQP
	.type	VEDU_DRV_RCCalculateCurQP, %function
VEDU_DRV_RCCalculateCurQP:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 220]
	cbnz	w0, .L98
.L80:
	ldr	w0, [x19, 24]
	ldr	w1, [x19, 104]
	cmp	w0, w1
	bcc	.L81
	sub	w2, w0, w1
	lsl	w2, w2, 6
	udiv	w1, w2, w1
.L82:
	cmn	w1, #128
	mov	x0, x19
	mov	w20, -128
	csel	w20, w1, w20, ge
	bl	VEDU_DRV_RCCalcQPDeltaByInstantBits
	ldr	w3, [x19, 36]
	cmp	w20, 256
	ldr	w1, [x19, 532]
	mov	w2, 256
	csel	w20, w20, w2, le
	cmp	w1, w3
	add	w0, w20, w0
	bls	.L83
	sub	w1, w1, w3
	lsl	w1, w1, 5
	udiv	w3, w1, w3
.L84:
	ldp	w4, w2, [x19, 212]
	cmn	w3, #64
	mov	w20, -64
	sub	w2, w2, w4
	mov	w1, -32
	lsl	w2, w2, 2
	csel	w3, w3, w20, ge
	mov	w20, 64
	mov	w6, 32
	ldr	w5, [x19, 44]
	sdiv	w2, w2, w4
	ldr	w4, [x19, 164]
	cmp	w2, w1
	csel	w1, w2, w1, ge
	cmp	w3, w20
	csel	w3, w3, w20, le
	cmp	w1, w6
	add	w20, w3, w0
	csel	w1, w1, w6, le
	add	w20, w20, w1
	add	w20, w20, w4
	str	w20, [x19, 164]
	cbz	w5, .L85
	ldr	w0, [x19, 208]
	ldr	w1, [x19, 96]
	add	w0, w1, w0, lsl 6
	add	w20, w0, w20
	lsr	w20, w20, 1
	str	w20, [x19, 164]
.L85:
	ldr	w0, [x19, 220]
	cbz	w0, .L86
	ldr	w0, [x19, 100]
	ldr	w1, [x19, 164]
	cmp	w1, w0
	bcc	.L99
.L86:
	ldr	w0, [x19, 556]
	cbnz	w0, .L87
.L100:
	ldr	w2, [x19, 164]
.L88:
	ldp	w0, w1, [x19, 4]
	lsl	w0, w0, 6
	lsl	w1, w1, 6
	cmp	w2, w0
	csel	w0, w2, w0, ge
	cmp	w1, w0
	csel	w0, w1, w0, le
	str	w0, [x19, 164]
	lsr	w0, w0, 6
	str	w0, [x19, 172]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L83:
	sub	w1, w3, w1
	lsl	w1, w1, 4
	udiv	w3, w1, w3
	neg	w3, w3
	b	.L84
	.p2align 3
.L81:
	sub	w0, w1, w0
	lsl	w20, w0, 5
	add	w0, w20, w0, lsl 3
	udiv	w20, w0, w1
	neg	w1, w20
	b	.L82
	.p2align 3
.L99:
	add	w0, w1, w0
	lsr	w0, w0, 1
	str	w0, [x19, 164]
	ldr	w0, [x19, 556]
	cbz	w0, .L100
.L87:
	mov	x0, x19
	bl	VEDU_DRV_RCGetQPDeltaForVBR
	ldr	w2, [x19, 164]
	add	w2, w0, w2
	b	.L88
	.p2align 3
.L98:
	mov	x0, x19
	bl	VEDU_DRV_RCModifyQPDeltaOfIP
	b	.L80
	.size	VEDU_DRV_RCCalculateCurQP, .-VEDU_DRV_RCCalculateCurQP
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCUpdateFrameBits
	.type	VEDU_DRV_RCUpdateFrameBits, %function
VEDU_DRV_RCUpdateFrameBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 220]
	cbz	w0, .L102
	ldr	w0, [x19, 252]
	add	w0, w0, 1
	cmp	w0, 1
	bls	.L108
	mov	w0, 0
	str	wzr, [x19, 252]
.L104:
	add	x0, x19, x0, uxtw 2
	ldr	w1, [x19, 24]
	str	w1, [x0, 224]
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L102:
	ldr	w1, [x19, 256]
	add	w1, w1, 1
	cmp	w1, 4
	bhi	.L106
	mov	w0, w1
	str	w1, [x19, 256]
.L107:
	add	x0, x19, x0, uxtw 2
	ldr	w1, [x19, 24]
	str	w1, [x0, 232]
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L106:
	str	wzr, [x19, 256]
	b	.L107
	.p2align 3
.L108:
	str	w0, [x19, 252]
	b	.L104
	.size	VEDU_DRV_RCUpdateFrameBits, .-VEDU_DRV_RCUpdateFrameBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCCalcTimeOfP
	.type	VEDU_DRV_RCCalcTimeOfP, %function
VEDU_DRV_RCCalcTimeOfP:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldp	w3, w2, [x19, 224]
	add	x0, x19, 232
	add	x4, x19, 252
	mov	w1, 0
	add	w2, w3, w2
	.p2align 2
.L110:
	ldr	w3, [x0], 4
	cmp	x0, x4
	add	w1, w1, w3
	bne	.L110
	cmp	w1, wzr
	ccmp	w2, wzr, 4, ne
	beq	.L114
	add	w2, w2, w2, lsl 2
	lsl	w1, w1, 1
	sdiv	w1, w2, w1
	cbz	w1, .L112
	str	w1, [x19, 200]
.L113:
	mov	w1, 1
	mov	w0, 0
	str	w1, [x19, 204]
.L111:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L112:
	mov	w0, 1
	str	w0, [x19, 200]
	b	.L113
.L114:
	mov	w0, -1
	b	.L111
	.size	VEDU_DRV_RCCalcTimeOfP, .-VEDU_DRV_RCCalcTimeOfP
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCGetTimeOfP
	.type	VEDU_DRV_RCGetTimeOfP, %function
VEDU_DRV_RCGetTimeOfP:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	VEDU_DRV_RCUpdateFrameBits
	mov	x0, x19
	bl	VEDU_DRV_RCCalcTimeOfP
	cmp	w0, wzr
	csetm	w0, ne
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCGetTimeOfP, .-VEDU_DRV_RCGetTimeOfP
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCGetTargetFrameBits
	.type	VEDU_DRV_RCGetTargetFrameBits, %function
VEDU_DRV_RCGetTargetFrameBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 36]
	ldr	w0, [x19, 552]
	cmp	w0, w1
	beq	.L126
	ldr	w0, [x19, 28]
	str	w1, [x19, 552]
	ldr	w3, [x19, 40]
	udiv	w1, w1, w0
	str	w1, [x19, 92]
	mul	w1, w1, w3
	str	w1, [x19, 88]
.L119:
	ldr	w4, [x19, 44]
	cbz	w4, .L120
	ldp	w1, w0, [x19, 180]
	ldr	w2, [x19, 200]
	cmp	w2, w1
	csel	w1, w2, w1, ge
	cmp	w1, w0
	csel	w1, w1, w0, le
	cmp	w3, 999
	bhi	.L122
.L128:
	ldr	w0, [x19, 548]
	cmp	w0, w3
	beq	.L127
	ldr	w0, [x19, 92]
	str	w3, [x19, 548]
	mul	w0, w3, w0
	str	w0, [x19, 88]
.L124:
	ldr	w2, [x19, 200]
	cmp	w4, wzr
	mul	w1, w1, w0
	sub	w0, w2, #1
	add	w0, w0, w3
	udiv	w0, w1, w0
	str	w0, [x19, 168]
	cset	w0, ne
	str	w0, [x19, 220]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L120:
	cmp	w3, 999
	ldr	w1, [x19, 204]
	bls	.L128
.L122:
	ldr	w0, [x19, 92]
	cmp	w4, wzr
	mul	w0, w1, w0
	str	w0, [x19, 168]
	cset	w0, ne
	str	w0, [x19, 220]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L126:
	ldr	w3, [x19, 40]
	b	.L119
	.p2align 3
.L127:
	ldr	w0, [x19, 88]
	b	.L124
	.size	VEDU_DRV_RCGetTargetFrameBits, .-VEDU_DRV_RCGetTargetFrameBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCofFirstFrame
	.type	VEDU_DRV_RCofFirstFrame, %function
VEDU_DRV_RCofFirstFrame:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldp	w2, w3, [x19, 36]
	ldr	w4, [x19, 28]
	mov	w5, 1
	lsl	w0, w2, 3
	ldr	w6, [x19, 76]
	add	w0, w0, w2, lsl 1
	mov	w7, 5
	udiv	w1, w2, w4
	lsr	w0, w0, 3
	str	w1, [x19, 92]
	stp	w7, w5, [x19, 200]
	stp	wzr, w0, [x19, 208]
	stp	w0, w5, [x19, 216]
	str	w4, [x19, 544]
	str	w3, [x19, 548]
	str	w2, [x19, 552]
	mul	w1, w1, w3
	str	w1, [x19, 88]
	cbz	w6, .L130
	str	wzr, [x19, 84]
.L130:
	mov	x0, x19
	bl	VEDU_DRV_RCInitInstantBits
	mov	x0, x19
	bl	VEDU_DRV_RCGetTargetFrameBits
	ldr	w0, [x19, 168]
	mov	w4, -1
	ldr	w3, [x19, 200]
	ldp	w1, w2, [x19, 12]
	str	w4, [x19, 256]
	udiv	w3, w0, w3
	stp	w3, w4, [x19, 248]
	stp	w3, w3, [x19, 232]
	stp	w3, w3, [x19, 240]
	stp	w0, w0, [x19, 224]
	bl	VEDU_DRV_EflRcInitQp
	lsl	w1, w0, 6
	ldr	w3, [x19, 208]
	add	w2, w1, 128
	str	w1, [x19, 164]
	str	w2, [x19, 100]
	sub	w0, w0, w3
	stp	w0, w0, [x19, 172]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCofFirstFrame, .-VEDU_DRV_RCofFirstFrame
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_DRV_RcOpenOneFrm
	.type	VENC_DRV_RcOpenOneFrm, %function
VENC_DRV_RcOpenOneFrm:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 260]
	str	wzr, [x19, 556]
	cbnz	w0, .L148
	ldr	w0, [x19, 20]
	cbnz	w0, .L149
	mov	x0, x19
	bl	VEDU_DRV_RCCalculateCurQP
	mov	x0, x19
	bl	VEDU_DRV_RCUpdateFrameBits
	mov	x0, x19
	bl	VEDU_DRV_RCCalcTimeOfP
	cbnz	w0, .L141
	mov	x0, x19
	bl	VEDU_DRV_RCGetTargetFrameBits
.L138:
	ldr	w0, [x19, 76]
	cbz	w0, .L139
	ldr	w0, [x19, 172]
	mov	w1, 23
	add	w0, w0, 1
	cmp	w0, 22
	bgt	.L150
.L140:
	str	wzr, [x19, 76]
	str	w1, [x19, 172]
.L139:
	ldr	w1, [x19, 72]
	mov	w0, 0
	cbz	w1, .L136
	ldr	w1, [x19, 172]
	add	w1, w1, 1
	str	w1, [x19, 172]
.L136:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L149:
	mov	x0, x19
	bl	VEDU_DRV_RCofFirstFrame
	b	.L138
	.p2align 3
.L148:
	ldr	w1, [x19, 264]
	mov	w0, 0
	str	w1, [x19, 164]
	lsr	w2, w1, 6
	add	w1, w1, 128
	str	w2, [x19, 172]
	str	w1, [x19, 100]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L150:
	mov	w1, 35
	cmp	w0, w1
	csel	w1, w0, w1, le
	b	.L140
	.p2align 3
.L141:
	mov	w0, -1
	b	.L136
	.size	VENC_DRV_RcOpenOneFrm, .-VENC_DRV_RcOpenOneFrm
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ModifyInfoForBitRatesChange
	.type	ModifyInfoForBitRatesChange, %function
ModifyInfoForBitRatesChange:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldp	w1, w2, [x19, 36]
	ldr	w0, [x19, 28]
	str	w1, [x19, 552]
	udiv	w0, w1, w0
	str	w0, [x19, 92]
	mul	w0, w2, w0
	str	w0, [x19, 88]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ModifyInfoForBitRatesChange, .-ModifyInfoForBitRatesChange
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_EflRCGetCurTimeOfP
	.type	VEDU_DRV_EflRCGetCurTimeOfP, %function
VEDU_DRV_EflRCGetCurTimeOfP:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 44]
	cbz	w0, .L153
	ldp	w0, w2, [x19, 180]
	ldr	w1, [x19, 200]
	ldr	x19, [sp, 16]
	cmp	w1, w0
	csel	w0, w1, w0, ge
	cmp	w0, w2
	csel	w0, w0, w2, le
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L153:
	ldr	w0, [x19, 204]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_EflRCGetCurTimeOfP, .-VEDU_DRV_EflRCGetCurTimeOfP
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_EflRCCalcTargetFrameBits
	.type	VEDU_DRV_EflRCCalcTargetFrameBits, %function
VEDU_DRV_EflRCCalcTargetFrameBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	w2, [x19, 40]
	ldr	w0, [x19, 548]
	cmp	w0, w2
	beq	.L158
	ldr	w3, [x19, 92]
	ldr	w4, [x19, 200]
	str	w2, [x19, 548]
	mul	w3, w2, w3
	sub	w2, w2, #1
	str	w3, [x19, 88]
	add	w2, w2, w4
	mul	w0, w20, w3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	udiv	w0, w0, w2
	ret
	.p2align 3
.L158:
	ldr	w3, [x19, 88]
	sub	w2, w2, #1
	ldr	w4, [x19, 200]
	mul	w0, w20, w3
	add	w2, w2, w4
	ldp	x19, x20, [sp, 16]
	udiv	w0, w0, w2
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_EflRCCalcTargetFrameBits, .-VEDU_DRV_EflRCCalcTargetFrameBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ModifyInfoForGopChange
	.type	ModifyInfoForGopChange, %function
ModifyInfoForGopChange:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 40]
	ldr	w0, [x19, 92]
	str	w1, [x19, 548]
	mul	w0, w0, w1
	str	w0, [x19, 88]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ModifyInfoForGopChange, .-ModifyInfoForGopChange
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCModifyQPForSpecialScene
	.type	VEDU_DRV_RCModifyQPForSpecialScene, %function
VEDU_DRV_RCModifyQPForSpecialScene:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 76]
	cbz	w0, .L161
	ldr	w0, [x19, 172]
	mov	w1, 23
	add	w0, w0, 1
	cmp	w0, 22
	bgt	.L171
.L162:
	str	wzr, [x19, 76]
	str	w1, [x19, 172]
.L161:
	ldr	w0, [x19, 72]
	cbz	w0, .L160
	ldr	w0, [x19, 172]
	add	w0, w0, 1
	str	w0, [x19, 172]
.L160:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L171:
	mov	w1, 35
	cmp	w0, w1
	csel	w1, w0, w1, le
	b	.L162
	.size	VEDU_DRV_RCModifyQPForSpecialScene, .-VEDU_DRV_RCModifyQPForSpecialScene
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCUpdateWaterline
	.type	VEDU_DRV_RCUpdateWaterline, %function
VEDU_DRV_RCUpdateWaterline:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 260]
	cbz	w0, .L175
	ldr	w2, [x19, 536]
	ldr	w0, [x19, 168]
	ldr	w1, [x19, 216]
	add	x2, x19, x2, lsl 2
	add	w1, w0, w1
	ldr	w2, [x2, 268]
	sub	w1, w1, w2
	ldr	w2, [x19, 24]
	sub	w0, w1, w0
	add	w0, w2, w0
	str	w0, [x19, 216]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L175:
	ldr	w1, [x19, 216]
	ldr	w0, [x19, 168]
	ldr	w2, [x19, 24]
	sub	w0, w1, w0
	add	w0, w2, w0
	str	w0, [x19, 216]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCUpdateWaterline, .-VEDU_DRV_RCUpdateWaterline
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ModifyInfoForFrmRateChange
	.type	ModifyInfoForFrmRateChange, %function
ModifyInfoForFrmRateChange:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w2, [x19, 528]
	ldr	w4, [x19, 544]
	cmp	w2, w4
	bls	.L177
	ldr	w3, [x19, 92]
	mov	w0, w4
	.p2align 2
.L178:
	add	x1, x19, x0, uxtw 2
	add	w0, w0, 1
	cmp	w2, w0
	str	w3, [x1, 268]
	bne	.L178
	ldr	w0, [x19, 532]
	sub	w4, w2, w4
	madd	w3, w4, w3, w0
	str	w3, [x19, 532]
.L179:
	str	w2, [x19, 544]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L177:
	bcs	.L179
	ldr	w1, [x19, 536]
	cmp	w2, w1
	bls	.L188
	sub	w4, w4, w2
	add	w0, w1, 1
	add	w4, w4, w1
	cmp	w0, w4
	bhi	.L179
	ldr	w1, [x19, 532]
	.p2align 2
.L184:
	add	x3, x19, x0, uxtw 2
	add	w0, w0, 1
	cmp	w0, w4
	ldr	w3, [x3, 268]
	sub	w1, w1, w3
	bls	.L184
	str	w1, [x19, 532]
	str	w2, [x19, 544]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L188:
	ldr	w3, [x19, 532]
	sub	w1, w1, w2
	mov	w0, 0
	.p2align 2
.L182:
	add	x4, x19, x0, uxtw 2
	add	w0, w0, 1
	cmp	w0, w1
	ldr	w4, [x4, 268]
	sub	w3, w3, w4
	bls	.L182
	str	w3, [x19, 532]
	str	w2, [x19, 544]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ModifyInfoForFrmRateChange, .-ModifyInfoForFrmRateChange
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCUpdateInstantBits
	.type	VEDU_DRV_RCUpdateInstantBits, %function
VEDU_DRV_RCUpdateInstantBits:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	ldr	w20, [x19, 528]
	bl	ModifyInfoForFrmRateChange
	ldr	w0, [x19, 536]
	ldr	w1, [x19, 532]
	add	w2, w0, 1
	ldr	w3, [x19, 24]
	add	x0, x19, x0, uxtw 2
	cmp	w20, w2
	add	w1, w3, w1
	csel	w2, w2, wzr, hi
	ldr	w4, [x0, 268]
	sub	w1, w1, w4
	str	w1, [x19, 532]
	str	w3, [x0, 268]
	str	w2, [x19, 536]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VEDU_DRV_RCUpdateInstantBits, .-VEDU_DRV_RCUpdateInstantBits
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCUpdateInfo
	.type	VEDU_DRV_RCUpdateInfo, %function
VEDU_DRV_RCUpdateInfo:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 260]
	cbz	w0, .L193
	ldr	w0, [x19, 536]
	cbz	w0, .L198
	sub	w0, w0, #1
	str	w0, [x19, 536]
.L195:
	add	x3, x19, x0, uxtw 2
	ldr	w1, [x19, 168]
	ldr	w2, [x19, 216]
	ldr	w0, [x3, 268]
	add	w2, w1, w2
	sub	w0, w2, w0
	b	.L196
	.p2align 3
.L193:
	ldr	w1, [x19, 168]
	ldr	w0, [x19, 216]
.L196:
	ldr	w2, [x19, 24]
	sub	w1, w0, w1
	mov	x0, x19
	add	w1, w2, w1
	str	w1, [x19, 216]
	bl	VEDU_DRV_RCUpdateInstantBits
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L198:
	ldr	w0, [x19, 528]
	sub	w0, w0, #1
	str	w0, [x19, 536]
	b	.L195
	.size	VEDU_DRV_RCUpdateInfo, .-VEDU_DRV_RCUpdateInfo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCIsNeedRecoding
	.type	VEDU_DRV_RCIsNeedRecoding, %function
VEDU_DRV_RCIsNeedRecoding:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w3, [x19, 36]
	mov	w1, 52429
	movk	w1, 0xcccc, lsl 16
	ldr	w0, [x19, 68]
	umull	x1, w3, w1
	lsr	x1, x1, 32
	lsr	w1, w1, 4
	lsl	w2, w1, 1
	sub	w5, w3, w2
	add	w2, w3, w2
	cbnz	w0, .L202
	ldr	w3, [x19, 260]
	cbnz	w3, .L202
	ldr	w3, [x19, 44]
	ldr	w6, [x19, 24]
	ldr	w4, [x19, 168]
	cbnz	w3, .L208
	lsl	w3, w4, 2
	sub	w3, w3, w4
	cmp	w6, w3
	cset	w3, hi
.L204:
	ldr	w4, [x19, 532]
	ldr	w6, [x19, 540]
	cmp	w5, w4
	cset	w8, hi
	cmp	w2, w4
	cset	w2, cc
	sub	w7, w4, w6
	cmp	w4, w6
	sub	w5, w6, w4
	csel	w4, w5, w7, ls
	orr	w2, w8, w2
	cmp	w4, w1
	and	w2, w2, w3
	cset	w1, hi
	ldr	w3, [x19, 20]
	and	w1, w1, w2
	str	w1, [x19, 260]
	cbz	w3, .L209
	mov	w1, 1
	str	w1, [x19, 260]
.L201:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L208:
	lsl	w3, w4, 2
	sub	w3, w3, w4
	cmp	w6, w3, lsr 1
	cset	w3, hi
	b	.L204
	.p2align 3
.L209:
	eor	w1, w1, 1
	ldr	x19, [sp, 16]
	neg	w0, w1
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L202:
	mov	w0, -1
	str	wzr, [x19, 260]
	b	.L201
	.size	VEDU_DRV_RCIsNeedRecoding, .-VEDU_DRV_RCIsNeedRecoding
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCStartQPChange
	.type	VEDU_DRV_RCStartQPChange, %function
VEDU_DRV_RCStartQPChange:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w7, [x19, 24]
	ldr	w0, [x19, 168]
	ldr	w3, [x19, 36]
	cmp	w7, w0
	ldr	w6, [x19, 532]
	ldr	w4, [x19, 540]
	bcc	.L211
	sub	w7, w7, w0
	lsl	w7, w7, 5
	udiv	w7, w7, w0
.L212:
	lsl	w0, w4, 5
	lsl	w2, w6, 5
	mov	w5, 63
	ldr	x1, .LC6
	add	x1, x1, 208
	udiv	w0, w0, w3
	cmp	w0, wzr
	csel	w0, w0, wzr, ge
	udiv	w2, w2, w3
	cmp	w2, wzr
	csel	w2, w2, wzr, ge
	cmp	w2, w5
	csel	w2, w2, w5, le
	cmp	w0, w5
	csel	w0, w0, w5, le
	cmp	w6, w4
	ccmp	w3, w4, 2, hi
	ldr	w2, [x1, w2, sxtw 2]
	ldr	w0, [x1, w0, sxtw 2]
	sub	w0, w2, w0
	bcc	.L213
	cmp	w6, w4
	ccmp	w3, w4, 0, cc
	bls	.L214
.L213:
	lsl	w2, w0, 2
	mov	w5, -192
	sub	w0, w2, w0
	mov	w2, 256
	lsl	w0, w0, 1
	cmp	w0, w5
	csel	w5, w0, w5, ge
	cmp	w5, w2
	csel	w5, w5, w2, le
.L215:
	sub	w6, w6, w3
	cmn	w7, #128
	ldp	w0, w2, [x19, 212]
	lsl	w4, w6, 2
	mov	w1, -128
	sub	w4, w4, w6
	sub	w2, w2, w0
	lsl	w4, w4, 5
	lsl	w2, w2, 5
	udiv	w3, w4, w3
	csel	w4, w7, w1, ge
	mov	w1, 256
	sdiv	w2, w2, w0
	mov	w0, -32
	cmp	w3, w0
	csel	w3, w3, w0, ge
	cmp	w2, w0
	csel	w2, w2, w0, ge
	mov	w0, 32
	cmp	w4, w1
	csel	w1, w4, w1, le
	cmp	w3, w0
	csel	w3, w3, w0, le
	add	w1, w1, w5
	cmp	w2, w0
	add	w1, w1, w3
	csel	w2, w2, w0, le
	ldr	w4, [x19, 164]
	add	w1, w1, w2
	ldp	w0, w2, [x19, 4]
	add	w1, w1, w4
	lsl	w0, w0, 6
	lsl	w2, w2, 6
	cmp	w1, w0
	csel	w0, w1, w0, ge
	cmp	w2, w0
	csel	w0, w2, w0, le
	str	w0, [x19, 264]
	cmp	w4, w0
	csetm	w0, eq
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L211:
	sub	w0, w0, w7
	lsl	w0, w0, 5
	udiv	w0, w0, w7
	neg	w7, w0
	b	.L212
	.p2align 3
.L214:
	ldr	w1, [x19, 44]
	mov	w5, 0
	cbz	w1, .L215
	lsl	w2, w0, 2
	mov	w5, -192
	sub	w0, w2, w0
	mov	w2, 256
	cmp	w0, w5
	csel	w5, w0, w5, ge
	cmp	w5, w2
	csel	w5, w5, w2, le
	b	.L215
	.size	VEDU_DRV_RCStartQPChange, .-VEDU_DRV_RCStartQPChange
	.align	3
.LC6:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	D_VENC_RC_CLIP3
	.type	D_VENC_RC_CLIP3, %function
D_VENC_RC_CLIP3:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w21, w1
	mov	w20, w2
	bl	_mcount
	cmp	w19, w21
	csel	w0, w19, w21, ge
	ldr	x21, [sp, 32]
	cmp	w20, w0
	csel	w0, w20, w0, le
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	D_VENC_RC_CLIP3, .-D_VENC_RC_CLIP3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VEDU_DRV_RCUpdateIMBInfo
	.type	VEDU_DRV_RCUpdateIMBInfo, %function
VEDU_DRV_RCUpdateIMBInfo:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 80]
	cmp	w0, 1
	beq	.L220
	ldr	w1, [x19, 108]
.L221:
	mov	x0, x19
	ldr	w6, [x19, 128]
	ldp	w2, w3, [x19, 112]
	ldp	w4, w5, [x19, 120]
	stp	w4, w5, [x19, 124]
	stp	w2, w3, [x19, 116]
	str	w6, [x19, 132]
	add	x5, x19, 136
	str	w1, [x0, 112]!
	mov	w3, 0
	mov	w2, 6
	b	.L224
	.p2align 3
.L228:
	ldr	w1, [x0]
.L224:
	cmp	w1, wzr
	add	x0, x0, 4
	cset	w4, eq
	cmp	x5, x0
	add	w3, w3, w1
	sub	w2, w2, w4
	bne	.L228
	mov	w0, 0
	cbz	w2, .L225
	sdiv	w0, w3, w2
.L225:
	str	w0, [x19, 136]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L220:
	ldr	w1, [x19, 136]
	str	w1, [x19, 108]
	b	.L221
	.size	VEDU_DRV_RCUpdateIMBInfo, .-VEDU_DRV_RCUpdateIMBInfo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_DRV_RcCloseOneFrm
	.type	VENC_DRV_RcCloseOneFrm, %function
VENC_DRV_RcCloseOneFrm:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	x21, x2
	mov	x22, x3
	bl	_mcount
	ldr	w0, [x19, 60]
	cbnz	w0, .L230
	ldr	w0, [x19, 64]
	cbz	w0, .L231
.L230:
	ldp	w3, w2, [x19, 48]
	ldr	w1, [x19, 56]
	mov	w0, 2
	cmp	w3, wzr
	sub	w5, w2, #1
	cset	w4, eq
	ldr	w3, [x19, 20]
	cmp	w2, wzr
	sub	w1, w1, #1
	csel	w0, w5, w0, ne
	str	w1, [x19, 56]
	stp	w4, w0, [x19, 48]
	cbz	w3, .L233
.L235:
	mov	w0, -1
.L234:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L231:
	str	wzr, [x19, 20]
	mov	w2, 0
	ldr	w3, [x19, 32]
	ldr	w1, [x21]
	sub	w1, w1, w3
	str	w1, [x21]
	ldr	w1, [x19, 44]
	cbz	w1, .L253
.L238:
	str	w2, [x22]
	ldr	x1, [x20]
	ldr	w2, [x19, 24]
	cbz	x1, .L240
	ldp	w3, w4, [x20, 16]
	cmp	w3, w4
	bhi	.L254
	ldr	w3, [x20, 12]
	add	x1, x1, x3, lsl 2
.L242:
	add	w0, w2, w0
	str	w0, [x20, 16]
	str	w2, [x1]
	ldp	w1, w0, [x20, 8]
	add	w0, w0, 1
	str	w0, [x20, 12]
	cmp	w0, w1
	bcc	.L240
	sub	w0, w0, w1
	str	w0, [x20, 12]
.L240:
	mov	x0, x19
	bl	VEDU_DRV_RCUpdateIMBInfo
	ldr	w0, [x19, 44]
	cbnz	w0, .L255
	ldr	w0, [x19, 164]
	str	w0, [x19, 100]
.L245:
	ldr	w0, [x19, 168]
	ldr	w1, [x19, 556]
	str	w0, [x19, 104]
	mov	w0, 0
	cbz	w1, .L234
	ldr	w2, [x19, 24]
	ldr	w1, [x19, 564]
	ldr	w3, [x19, 84]
	ldr	w6, [x19, 560]
	add	w1, w1, w2
	ldr	w5, [x19, 92]
	add	w2, w3, 1
	ldr	w4, [x19, 172]
	str	w2, [x19, 84]
	sub	w1, w1, w5
	add	w3, w6, w4
	str	w1, [x19, 564]
	str	w3, [x19, 560]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L233:
	ldr	x0, [x20]
	cbz	x0, .L235
	ldp	w1, w2, [x20, 16]
	cmp	w1, w2
	bls	.L236
	ldr	w2, [x20, 12]
	ldr	w3, [x0, w2, uxtw 2]
	add	x0, x0, x2, uxtw 2
	sub	w1, w1, w3
	str	w1, [x20, 16]
.L237:
	str	wzr, [x0]
	ldp	w1, w0, [x20, 8]
	add	w0, w0, 1
	str	w0, [x20, 12]
	cmp	w0, w1
	bcc	.L235
	sub	w1, w0, w1
	str	w1, [x20, 12]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L253:
	ldr	w2, [x22]
	add	w2, w2, 1
	b	.L238
	.p2align 3
.L255:
	ldr	w0, [x19, 164]
	str	w0, [x19, 96]
	b	.L245
	.p2align 3
.L254:
	ldr	w4, [x20, 12]
	ldr	w0, [x1, w4, uxtw 2]
	add	x1, x1, x4, uxtw 2
	sub	w0, w3, w0
	b	.L242
	.p2align 3
.L236:
	ldr	w1, [x20, 12]
	str	wzr, [x20, 16]
	add	x0, x0, x1, lsl 2
	b	.L237
	.size	VENC_DRV_RcCloseOneFrm, .-VENC_DRV_RcCloseOneFrm
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_DRV_EflRcAverage
	.type	VENC_DRV_EflRcAverage, %function
VENC_DRV_EflRcAverage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, wzr
	mov	w3, w19
	ble	.L261
	mov	x0, 0
	mov	w4, 0
	.p2align 2
.L259:
	ldr	w2, [x20, x0, lsl 2]
	add	x0, x0, 1
	cmp	w2, wzr
	add	w4, w4, w2
	cset	w2, eq
	cmp	w19, w0
	sub	w3, w3, w2
	bgt	.L259
.L257:
	mov	w0, 0
	cbz	w3, .L260
	sdiv	w0, w4, w3
.L260:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L261:
	mov	w4, 0
	b	.L257
	.size	VENC_DRV_EflRcAverage, .-VENC_DRV_EflRcAverage
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	VENC_DRV_CalRCRatio
	.type	VENC_DRV_CalRCRatio, %function
VENC_DRV_CalRCRatio:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w19, w20
	bhi	.L268
	sub	w19, w20, w19
	lsl	w0, w19, 2
	sub	w0, w0, w19
	lsl	w1, w0, 5
	add	w0, w0, w1
	add	w0, w0, w19
	udiv	w0, w0, w20
	ldp	x19, x20, [sp, 16]
	neg	w0, w0
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L268:
	sub	w19, w19, w20
	lsl	w0, w19, 2
	sub	w0, w0, w19
	lsl	w1, w0, 5
	add	w0, w0, w1
	add	w0, w0, w19
	udiv	w0, w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	VENC_DRV_CalRCRatio, .-VENC_DRV_CalRCRatio
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	MaxIPropDeltaQP, %object
	.size	MaxIPropDeltaQP, 208
MaxIPropDeltaQP:
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	10
	.word	9
	.word	9
	.word	8
	.word	8
	.word	7
	.word	7
	.word	6
	.word	6
	.word	5
	.word	5
	.word	5
	.word	4
	.word	4
	.word	4
	.word	3
	.word	3
	.word	3
	.word	3
	.word	3
	.word	2
	.word	2
	.word	2
	.word	2
	.word	2
	.word	2
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.type	TabLnx, %object
	.size	TabLnx, 256
TabLnx:
	.word	0
	.word	-1216
	.word	-972
	.word	-830
	.word	-729
	.word	-651
	.word	-587
	.word	-533
	.word	-486
	.word	-445
	.word	-408
	.word	-374
	.word	-344
	.word	-316
	.word	-290
	.word	-265
	.word	-243
	.word	-221
	.word	-201
	.word	-182
	.word	-164
	.word	-147
	.word	-131
	.word	-115
	.word	-100
	.word	-86
	.word	-72
	.word	-59
	.word	-46
	.word	-34
	.word	-22
	.word	-11
	.word	0
	.word	10
	.word	21
	.word	31
	.word	41
	.word	50
	.word	60
	.word	69
	.word	78
	.word	86
	.word	95
	.word	103
	.word	111
	.word	119
	.word	127
	.word	134
	.word	142
	.word	149
	.word	156
	.word	163
	.word	170
	.word	177
	.word	183
	.word	190
	.word	196
	.word	202
	.word	208
	.word	214
	.word	220
	.word	226
	.word	232
	.word	237
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
