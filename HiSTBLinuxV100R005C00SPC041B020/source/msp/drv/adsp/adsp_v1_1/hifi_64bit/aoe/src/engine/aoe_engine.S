#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"aoe_engine.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	ENGINEDestroyChn, %function
ENGINEDestroyChn:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L1
	ldr	w0, [x19, 12]
	cbz	w0, .L11
.L3:
	mov	x1, x19
	mov	w0, 26
	bl	HI_ADSP_FREE
.L1:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L11:
	ldr	x1, [x19, 176]
	cbz	x1, .L3
	mov	w0, 26
	bl	HI_ADSP_FREE
	str	xzr, [x19, 176]
	b	.L3
	.size	ENGINEDestroyChn, .-ENGINEDestroyChn
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINEOutRawFrame, %function
ENGINEOutRawFrame:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	bl	_mcount
	mov	w0, w19
	bl	AIP_GetRingIn
	mov	x19, x0
	mov	w0, w20
	bl	AOP_GetRingOut
	cmp	x19, xzr
	ccmp	x0, xzr, 4, ne
	beq	.L12
	mov	x1, x0
	mov	w3, 3
	mov	w2, w21
	mov	x0, x19
	bl	RBUF_CopyData
.L12:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	ENGINEOutRawFrame, .-ENGINEOutRawFrame
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINEMixData, %function
ENGINEMixData:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	x23, x2
	bl	_mcount
	str	xzr, [x29, 104]
	add	x0, x29, 72
	add	x2, x29, 88
	mov	x1, x0
	ldp	w24, w22, [x19]
	stp	xzr, xzr, [x0]
	mov	w0, w20
	stp	xzr, xzr, [x2]
	bl	AIP_QueryFiFoInfo
	ldr	w2, [x19, 120]
	ldr	w0, [x29, 96]
	cmp	w0, w2
	bls	.L17
	ldr	w21, [x29, 92]
	cmp	w21, w2
	bcc	.L21
	ldr	w5, [x19, 124]
	mov	w3, w24
	ldr	w1, [x29, 80]
	mov	x0, x23
	ldr	x4, [x29, 72]
	udiv	w2, w2, w5
	add	x1, x4, x1
	mul	w2, w2, w22
	bl	UTIL_MixerPro
.L20:
	ldr	w1, [x19, 120]
	mov	w0, w20
	bl	AIP_ReadDataComplete
.L17:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L21:
	ldr	w2, [x19, 124]
	mov	w3, w24
	ldr	w0, [x29, 80]
	ldr	x1, [x29, 72]
	udiv	w2, w21, w2
	add	x1, x1, x0
	mov	x0, x23
	mul	w2, w2, w22
	bl	UTIL_MixerPro
	ldp	w2, w4, [x19, 120]
	ldr	x1, [x29, 72]
	mov	w3, w24
	sub	w2, w2, w21
	add	x0, x23, x21, uxtw
	udiv	w2, w2, w4
	mul	w2, w2, w22
	bl	UTIL_MixerPro
	b	.L20
	.size	ENGINEMixData, .-ENGINEMixData
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINEOutFrame, %function
ENGINEOutFrame:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldp	w20, w1, [x21, 84]
	ldr	w0, [x21, 12]
	stp	w20, w1, [x29, 80]
	cbnz	w0, .L22
	ldr	x23, .LC2
	mov	x22, 0
	mov	x24, x23
	mov	x25, x23
.L35:
	cbz	w20, .L34
	uxtw	x20, w20
	mov	w19, 0
	b	.L33
	.p2align 3
.L28:
	add	w19, w19, 1
	cmp	w19, 8
	beq	.L34
.L33:
	asr	x0, x20, x19
	tbz	x0, 0, .L28
	mov	w0, w19
	bl	AOP_WriteBuffProcTryCnt
	mov	w0, w19
	add	x1, x29, 120
	bl	AOP_GetChnStatus
	ldr	w0, [x21, 100]
	asr	x0, x0, x19
	tbz	x0, 0, .L28
	mov	w0, w19
	bl	AOP_WriteBuffProcOKCnt
	ldr	w0, [x21, 104]
	cbnz	w0, .L50
.L30:
	ldr	w0, [x29, 144]
	cbz	w0, .L32
	ldr	w0, [x21, 164]
	cbnz	w0, .L51
.L32:
	ldr	x3, [x23, 48]
	mov	x2, 32
	add	x1, x21, 176
	add	x0, x29, 88
	blr	x3
.L31:
	add	x1, x29, 88
	mov	w0, w19
	bl	AOP_WriteRbfData
	mov	w0, w19
	add	x1, x29, 120
	add	w19, w19, 1
	bl	AOP_ProcDelayCompensation
	cmp	w19, 8
	bne	.L33
	.p2align 2
.L34:
	add	x22, x22, 4
	cmp	x22, 8
	beq	.L22
	add	x0, x29, 80
	ldr	w20, [x22, x0]
	b	.L35
	.p2align 3
.L22:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 160
	ret
	.p2align 3
.L50:
	ldr	w0, [x29, 140]
	cbnz	w0, .L30
	ldr	x3, [x25, 48]
	mov	x2, 32
	add	x1, x21, 368
	add	x0, x29, 88
	blr	x3
	b	.L31
	.p2align 3
.L51:
	ldr	x3, [x24, 48]
	mov	x2, 32
	add	x1, x21, 208
	add	x0, x29, 88
	blr	x3
	b	.L31
	.size	ENGINEOutFrame, .-ENGINEOutFrame
	.align	3
.LC2:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINECheckAipData.isra.0, %function
ENGINECheckAipData.isra.0:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	uxtw	x20, w0
	mov	x0, x30
	uxtw	x19, w1
	uxtw	x21, w2
	bl	_mcount
	cmp	w19, wzr
	ccmp	w20, wzr, 4, ne
	beq	.L53
	mov	w0, 0
	cbz	w20, .L54
	mov	w3, 0
	uxtw	x2, w21
	.p2align 2
.L56:
	asr	x4, x20, x3
	asr	x1, x2, x3
	tbz	x4, 0, .L58
	tbz	x1, 0, .L74
	orr	w0, w0, 1
.L58:
	add	w3, w3, 1
	cmp	w3, 8
	bne	.L56
.L54:
	cbz	w19, .L59
.L75:
	mov	w3, 0
	.p2align 2
.L62:
	asr	x4, x19, x3
	asr	x1, x21, x3
	tbz	x4, 0, .L60
	tbz	x1, 0, .L61
	orr	w0, w0, 2
.L60:
	add	w3, w3, 1
	cmp	w3, 8
	bne	.L62
.L59:
	and	w0, w0, 1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L53:
	cmp	w21, wzr
	ldr	x21, [sp, 32]
	cset	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L61:
	and	w0, w0, -3
	b	.L59
	.p2align 3
.L74:
	and	w0, w0, -2
	cbz	w19, .L59
	b	.L75
	.size	ENGINECheckAipData.isra.0, .-ENGINECheckAipData.isra.0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINECheckAopSpace.isra.1, %function
ENGINECheckAopSpace.isra.1:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	uxtw	x19, w2
	mov	w20, w1
	bl	_mcount
	mov	w0, 0
	cbz	w19, .L98
	cbz	w21, .L78
	uxtw	x5, w21
	mov	w3, 0
	uxtw	x2, w19
	.p2align 2
.L80:
	asr	x4, x5, x3
	asr	x1, x2, x3
	tbz	x4, 0, .L82
	tbz	x1, 0, .L100
	orr	w0, w0, 1
.L82:
	add	w3, w3, 1
	cmp	w3, 8
	bne	.L80
.L78:
	cbz	w20, .L83
.L102:
	uxtw	x1, w20
	mov	w3, 0
	.p2align 2
.L87:
	asr	x4, x1, x3
	asr	x2, x19, x3
	tbz	x4, 0, .L84
	tbz	x2, 0, .L85
	orr	w0, w0, 2
.L84:
	add	w3, w3, 1
	cmp	w3, 8
	bne	.L87
.L86:
	cbnz	w21, .L88
	and	w0, w0, 2
.L98:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L83:
	cbz	w21, .L101
.L88:
	and	w0, w0, 1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L101:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L85:
	and	w0, w0, -3
	b	.L86
	.p2align 3
.L100:
	and	w0, w0, -2
	cbz	w20, .L83
	b	.L102
	.size	ENGINECheckAopSpace.isra.1, .-ENGINECheckAopSpace.isra.1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINECreateChn, %function
ENGINECreateChn:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w21, w0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC4
	ldr	x22, .LC3
	mov	x2, 64
	mov	w1, 0
	add	x0, x29, x2
	ldr	x3, [x20, 40]
	ldr	x19, [x22, w21, uxtw 3]
	blr	x3
	ldr	w1, [x19]
	add	x0, x29, 64
	and	w1, w1, 3
	bl	AOE_REG_ReadBitDepth
	cbnz	w0, .L121
	ldr	w1, [x19]
	add	x0, x29, 68
	ubfx	x1, x1, 2, 2
	bl	AOE_REG_ReadChannel
	cbnz	w0, .L121
	ldr	w1, [x19]
	add	x0, x29, 72
	ubfx	x1, x1, 4, 4
	bl	AOE_REG_ReadSampelRate
	cbnz	w0, .L121
	ldr	w1, [x19]
	add	x0, x29, 76
	ubfx	x1, x1, 8, 2
	bl	AOE_REG_ReadDataFormat
	cbnz	w0, .L121
	ldr	x2, [x22, w21, uxtw 3]
	mov	x4, 0
	mov	x3, 0
	mov	w1, 400
	mov	w0, 26
	ldr	w6, [x2, 80]
	ldr	w5, [x2, 84]
	bfi	x4, x6, 0, 32
	bfi	x4, x5, 32, 32
	str	x4, [x29, 96]
	ldr	w4, [x2, 88]
	ubfx	x4, x4, 0, 24
	str	w4, [x29, 104]
	ldr	w4, [x2, 88]
	ldr	w6, [x2, 100]
	ldr	w5, [x2, 104]
	ubfx	x4, x4, 24, 4
	bfi	x3, x6, 0, 32
	str	w4, [x29, 108]
	bfi	x3, x5, 32, 32
	str	x3, [x29, 112]
	ldr	w3, [x2, 108]
	ubfx	x3, x3, 0, 24
	str	w3, [x29, 120]
	ldr	w2, [x2, 108]
	ubfx	x2, x2, 24, 4
	str	w2, [x29, 124]
	bl	HI_ADSP_MALLOC
	mov	x19, x0
	cbz	x0, .L121
	ldr	x3, [x20, 40]
	mov	x2, 400
	mov	w1, 0
	blr	x3
	mov	x2, 64
	ldr	x3, [x20, 48]
	add	x1, x29, x2
	mov	x0, x19
	blr	x3
	ldp	w1, w0, [x19]
	bl	UTIL_CalcFrameSize
	ldr	w2, [x19, 8]
	mov	w1, 19923
	mov	w3, 48000
	movk	w1, 0x1062, lsl 16
	cmp	w2, w3
	add	w2, w2, w2, lsl 2
	umull	x1, w2, w1
	lsr	x1, x1, 32
	lsr	w1, w1, 6
	beq	.L122
	and	w2, w1, 3
	cbz	w2, .L107
	and	w1, w1, -4
	add	w1, w1, 4
.L107:
	mul	w2, w0, w1
	ldp	w1, w0, [x19]
	str	w2, [x19, 120]
	bl	UTIL_CalcFrameSize
	str	w0, [x19, 124]
	ldr	w2, [x19, 120]
	ldr	w1, [x19, 12]
	udiv	w0, w2, w0
	str	w0, [x19, 16]
	cbz	w1, .L123
.L108:
	str	w21, [x19, 116]
	mov	w0, 1
	str	w0, [x19, 128]
	mov	x0, x19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L123:
	ldp	w1, w5, [x19]
	ldr	w4, [x19, 8]
	add	x22, x19, 176
	add	x23, x19, 208
	ldr	x3, [x20, 48]
	stp	w1, w5, [x19, 192]
	stp	w4, wzr, [x19, 200]
	stp	w2, w0, [x19, 184]
	mov	x1, x22
	mov	x2, 32
	mov	x0, x23
	str	xzr, [x19, 176]
	blr	x3
	ldr	x3, [x20, 48]
	mov	x1, x22
	mov	x2, 32
	add	x0, x19, 240
	blr	x3
	ldr	x3, [x20, 48]
	mov	x2, 32
	mov	x1, x22
	add	x0, x19, 272
	blr	x3
	ldp	w24, w1, [x19, 252]
	mov	w0, 8
	str	w0, [x19, 260]
	bl	UTIL_CalcFrameSize
	ldr	x3, [x20, 48]
	mov	x1, x22
	mul	w0, w24, w0
	mov	x2, 32
	str	w0, [x19, 248]
	add	x0, x19, 304
	blr	x3
	ldr	x3, [x20, 48]
	mov	x1, x23
	mov	x2, 32
	add	x0, x19, 336
	blr	x3
	ldr	x3, [x20, 48]
	mov	x1, x22
	mov	x2, 32
	add	x0, x19, 368
	blr	x3
	ldr	w22, [x19, 376]
	ldr	w3, [x19, 184]
	ldr	w2, [x19, 216]
	ldr	w4, [x19, 248]
	ldr	w1, [x19, 312]
	ldr	w0, [x19, 344]
	add	w2, w2, w4
	add	w2, w2, w3, lsl 1
	add	w0, w1, w0
	add	w1, w2, w0
	mov	w0, 26
	add	w22, w1, w22
	mov	w1, w22
	bl	HI_ADSP_MALLOC
	mov	x23, x0
	cbz	x0, .L109
	ldr	x3, [x20, 40]
	uxtw	x2, w22
	mov	w1, 0
	blr	x3
	str	x23, [x19, 176]
	ldr	w4, [x19, 184]
	ldr	w3, [x19, 216]
	add	x4, x23, x4
	ldr	w2, [x19, 248]
	add	x3, x4, x3
	ldr	w1, [x19, 280]
	add	x2, x3, x2
	ldr	w0, [x19, 312]
	add	x1, x2, x1
	ldr	w5, [x19, 344]
	add	x0, x1, x0
	str	x4, [x19, 208]
	str	x3, [x19, 240]
	add	x4, x0, x5
	str	x2, [x19, 272]
	str	x1, [x19, 304]
	str	x0, [x19, 336]
	str	x4, [x19, 368]
	b	.L108
.L109:
	mov	x0, x19
	bl	ENGINEDestroyChn
.L121:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	x0, 0
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L122:
	and	w2, w1, 255
	cbz	w2, .L107
	and	w1, w1, -256
	add	w1, w1, 256
	b	.L107
	.size	ENGINECreateChn, .-ENGINECreateChn
	.align	3
.LC3:
	.xword	.LANCHOR0
	.align	3
.LC4:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINECheckRbfInData, %function
ENGINECheckRbfInData:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 12]
	cbz	w0, .L125
	ldr	w19, [x19, 120]
	mov	w0, w20
	bl	AIP_QueryBusy
	cmp	w19, w0
	csel	w0, w19, wzr, ls
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L125:
	mov	w0, w20
	add	x1, x29, 44
	bl	AIP_GetPcmType
	ldr	w0, [x29, 44]
	cmp	w0, 3
	beq	.L132
	cmp	w0, 4
	beq	.L133
	ldr	w19, [x19, 184]
.L128:
	mov	w0, w20
	bl	AIP_QueryFiFoBusy
	cmp	w0, w19
	csel	w0, wzr, w19, ls
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L132:
	ldr	w19, [x19, 248]
	b	.L128
	.p2align 3
.L133:
	ldr	w19, [x19, 216]
	b	.L128
	.size	ENGINECheckRbfInData, .-ENGINECheckRbfInData
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UTIL_MixerMcPro
	.type	UTIL_MixerMcPro, %function
UTIL_MixerMcPro:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	w19, w2
	mov	x0, x30
	mov	x21, x1
	mov	w22, w3
	bl	_mcount
	cbz	w19, .L134
	cmp	w22, 16
	beq	.L142
	cmp	w22, 24
	beq	.L143
.L134:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L142:
	mov	x0, 0
	.p2align 2
.L137:
	ldrsh	w1, [x21, x0, lsl 1]
	strh	w1, [x20, x0, lsl 1]
	add	x0, x0, 1
	cmp	w19, w0
	bhi	.L137
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L143:
	mov	x0, 0
	.p2align 2
.L139:
	ldr	w1, [x21, x0, lsl 2]
	str	w1, [x20, x0, lsl 2]
	add	x0, x0, 1
	cmp	w19, w0
	bhi	.L139
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	UTIL_MixerMcPro, .-UTIL_MixerMcPro
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	ENGINEMixFrame, %function
ENGINEMixFrame:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x20, 12]
	mov	w1, 5
	str	w1, [x29, 104]
	cbz	w0, .L188
.L153:
	ldr	w0, [x20, 140]
	cmp	w0, 2
	bne	.L144
	ldr	w0, [x20, 132]
	add	w0, w0, 1
	str	w0, [x20, 132]
.L144:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L188:
	ldr	x21, .LC7
	mov	w1, 0
	ldr	w2, [x20, 184]
	mov	w19, 0
	ldr	x0, [x20, 176]
	ldr	x3, [x21, 40]
	blr	x3
	.p2align 2
.L150:
	ldr	w2, [x20, 64]
	mov	w0, w19
	add	x1, x29, 104
	asr	x2, x2, x19
	tbz	x2, 0, .L147
	ldr	w2, [x20, 12]
	cbnz	w2, .L148
	bl	AIP_GetPcmType
	ldr	w0, [x29, 104]
	cmp	w0, 1
	bls	.L189
.L147:
	add	w19, w19, 1
	cmp	w19, 8
	bne	.L150
	ldr	w2, [x20, 12]
.L148:
	mov	w0, 5
	str	w0, [x29, 84]
	cbnz	w2, .L153
	mov	w19, 0
	ldr	w0, [x20, 64]
	str	wzr, [x20, 168]
	b	.L162
	.p2align 3
.L154:
	add	w19, w19, 1
	cmp	w19, 8
	beq	.L163
.L162:
	asr	x1, x0, x19
	tbz	x1, 0, .L154
	mov	w0, w19
	add	x1, x29, 84
	bl	AIP_GetPcmType
	ldr	w0, [x29, 84]
	cmp	w0, 3
	beq	.L155
	add	w19, w19, 1
	ldr	w0, [x20, 64]
	cmp	w19, 8
	bne	.L162
.L163:
	mov	w19, 0
	b	.L161
	.p2align 3
.L164:
	add	w19, w19, 1
	cmp	w19, 8
	beq	.L166
.L161:
	asr	x1, x0, x19
	tbz	x1, 0, .L164
	mov	w0, w19
	add	x1, x29, 84
	bl	AIP_GetPcmType
	ldr	w0, [x29, 84]
	cmp	w0, 2
	beq	.L165
	add	w19, w19, 1
	ldr	w0, [x20, 64]
	cmp	w19, 8
	bne	.L161
.L166:
	mov	w19, 0
	str	wzr, [x20, 164]
	b	.L168
	.p2align 3
.L167:
	add	w19, w19, 1
	cmp	w19, 8
	beq	.L153
	ldr	w0, [x20, 64]
.L168:
	asr	x0, x0, x19
	tbz	x0, 0, .L167
	mov	w0, w19
	add	x1, x29, 84
	bl	AIP_GetPcmType
	ldr	w0, [x29, 84]
	cmp	w0, 4
	bne	.L167
	ldr	w2, [x20, 184]
	ldr	x1, [x20, 176]
	ldr	x3, [x21, 48]
	ldr	x0, [x20, 208]
	blr	x3
	ldr	x2, [x20, 208]
	mov	x0, x20
	mov	w1, w19
	bl	ENGINEMixData
	mov	w0, 1
	str	w0, [x20, 164]
	b	.L153
	.p2align 3
.L189:
	ldr	x2, [x20, 176]
	mov	w1, w19
	mov	x0, x20
	bl	ENGINEMixData
	b	.L147
	.p2align 3
.L165:
	ldr	x2, [x20, 176]
	mov	x0, x20
	mov	w1, w19
	bl	ENGINEMixData
	ldr	w0, [x20, 64]
	b	.L166
	.p2align 3
.L155:
	ldr	x3, [x21, 48]
	ldr	w2, [x20, 184]
	ldr	x1, [x20, 176]
	ldr	x0, [x20, 240]
	blr	x3
	ldr	w25, [x20]
	add	x2, x29, 104
	ldr	w1, [x20, 256]
	mov	w0, 8
	ldr	w22, [x20, 172]
	ldr	w23, [x20, 260]
	stp	xzr, xzr, [x2]
	add	x2, x29, 120
	ldr	x24, [x20, 240]
	str	xzr, [x29, 136]
	stp	xzr, xzr, [x2]
	bl	UTIL_CalcFrameSize
	mov	w26, w0
	add	x1, x29, 104
	mov	w0, w19
	bl	AIP_QueryFiFoInfo
	ldr	w1, [x20, 248]
	ldr	w0, [x29, 128]
	cmp	w0, w1
	bcc	.L156
	cbz	w22, .L157
	ldr	w22, [x29, 124]
	cmp	w22, w1
	bcs	.L158
	udiv	w2, w22, w26
	ldr	w0, [x29, 112]
	ldr	x1, [x29, 104]
	mov	w3, w25
	add	x1, x1, x0
	mov	x0, x24
	mul	w2, w2, w23
	bl	UTIL_MixerMcPro
	ldr	w2, [x20, 248]
	mov	w3, w25
	ldr	x1, [x29, 104]
	add	x0, x24, x22, uxtw
	sub	w2, w2, w22
	udiv	w2, w2, w26
	mul	w2, w2, w23
	bl	UTIL_MixerMcPro
	ldr	w1, [x20, 248]
.L157:
	mov	w0, w19
	bl	AIP_ReadDataComplete
.L159:
	mov	w1, 1
	ldr	w0, [x20, 64]
	str	w1, [x20, 168]
	b	.L163
.L156:
	mov	w0, w19
	add	x1, x29, 88
	bl	AIP_GetChnStatus
	ldr	w0, [x29, 88]
	cmp	w0, 1
	bne	.L159
	mov	w0, w19
	bl	AIP_UnderFlowCalc
	b	.L159
.L158:
	udiv	w2, w1, w26
	ldr	x4, [x29, 104]
	ldr	w1, [x29, 112]
	mov	w3, w25
	mov	x0, x24
	add	x1, x4, x1
	mul	w2, w2, w23
	bl	UTIL_MixerMcPro
	ldr	w1, [x20, 248]
	b	.L157
	.size	ENGINEMixFrame, .-ENGINEMixFrame
	.align	3
.LC7:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ENGINE_Process
	.type	ENGINE_Process, %function
ENGINE_Process:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x0, x30
	mov	w19, 0
	mov	x20, 1
	bl	_mcount
	ldr	x28, .LC10
	.p2align 2
.L265:
	ldr	x1, [x28]
	ldr	w0, [x1, 4]
	tbnz	x0, 2, .L191
	ldr	w0, [x1, 4]
	ands	w0, w0, 3
	beq	.L193
	cmp	w0, 1
	beq	.L194
	ldr	w0, [x1, 4]
	mov	w2, 4
	bfi	w0, w2, 3, 4
	str	w0, [x1, 4]
	ldr	w0, [x1, 4]
	orr	w0, w0, w2
	str	w0, [x1, 4]
.L191:
	ldr	x27, [x28, 48]
.L202:
	cbz	x27, .L205
.L198:
	ldr	w0, [x27, 128]
	cmp	w0, 1
	beq	.L358
.L205:
	add	w19, w19, 1
	add	x28, x28, 8
	cmp	w19, 6
	bne	.L265
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L194:
	ldr	x27, [x28, 48]
	cbz	x27, .L205
	ldr	w0, [x27, 128]
	cmp	w0, 1
	beq	.L359
.L203:
	ldr	w0, [x1, 4]
	tbnz	x0, 2, .L198
	ldr	w0, [x1, 4]
	and	w0, w0, -121
	str	w0, [x1, 4]
	ldr	w0, [x1, 4]
	orr	w0, w0, 4
	str	w0, [x1, 4]
	ldr	w0, [x27, 128]
	cmp	w0, 1
	bne	.L205
	.p2align 2
.L358:
	ldr	w0, [x27]
	mov	w22, 0
	str	w0, [x29, 112]
	mov	w23, 0
	ldr	w0, [x27, 4]
	mov	w26, 0
	str	w0, [x29, 100]
	ldr	x24, [x28]
	ldr	w0, [x27, 16]
	str	w0, [x29, 108]
	ldr	w21, [x24, 12]
	stp	wzr, wzr, [x29, 116]
	str	wzr, [x29, 124]
	uxth	w21, w21
	uxth	x25, w21
	b	.L212
	.p2align 3
.L207:
	add	w26, w26, 1
	cmp	w26, 8
	beq	.L360
.L212:
	asr	x0, x25, x26
	tbz	x0, 0, .L207
	mov	w0, w26
	add	x1, x29, 152
	bl	AOP_GetChnStatus
	ldr	w0, [x29, 152]
	cmp	w0, 1
	bne	.L207
	ldr	w0, [x29, 156]
	lsl	x6, x20, x26
	cmp	w0, 1
	beq	.L361
	ldr	w0, [x29, 124]
	orr	w0, w0, w6
	str	w0, [x29, 124]
.L209:
	ldr	w1, [x29, 112]
	ldr	w0, [x29, 160]
	cmp	w1, w0
	beq	.L362
.L210:
	orr	w23, w23, w6
.L211:
	ldr	w0, [x29, 108]
	ldr	w1, [x29, 168]
	str	w6, [x29, 104]
	mul	w1, w0, w1
	mov	w0, w26
	add	w26, w26, 1
	bl	AOP_QueryRbfFree
	cmp	w0, wzr
	ldr	w6, [x29, 104]
	orr	w6, w22, w6
	csel	w22, w6, w22, ne
	cmp	w26, 8
	bne	.L212
	.p2align 2
.L360:
	ldr	w0, [x29, 120]
	str	w0, [x27, 84]
	ldr	w0, [x29, 124]
	str	w0, [x27, 88]
	ldr	w0, [x29, 116]
	stp	wzr, w21, [x27, 76]
	stp	wzr, wzr, [x27, 68]
	stp	w23, w22, [x27, 96]
	str	w0, [x27, 92]
	mov	w21, 0
	ldr	w0, [x24, 8]
	uxth	w0, w0
	str	w0, [x27, 64]
	b	.L223
	.p2align 3
.L213:
	add	w21, w21, 1
	cmp	w21, 8
	beq	.L222
.L365:
	ldr	w0, [x27, 64]
.L223:
	lsl	x22, x20, x21
	tst	w0, w22
	beq	.L213
	mov	w0, w21
	add	x1, x29, 152
	bl	AIP_GetChnStatus
	ldr	w0, [x29, 152]
	cmp	w0, 1
	beq	.L363
	ldr	w0, [x27, 12]
	cbnz	w0, .L213
	mov	x1, x27
	mov	w0, w21
	bl	ENGINECheckRbfInData
	cbz	w0, .L213
	ldr	w0, [x27, 76]
	orr	w0, w0, w22
	str	w0, [x27, 76]
	ldr	w0, [x29, 156]
	cmp	w0, 1
	beq	.L364
	ldr	w0, [x27, 72]
	add	w21, w21, 1
	cmp	w21, 8
	orr	w22, w0, w22
	str	w22, [x27, 72]
	bne	.L365
	.p2align 2
.L222:
	ldr	x22, [x28, 48]
	ldr	w0, [x22, 12]
	cbnz	w0, .L366
	ldr	w0, [x22, 140]
	sub	w1, w0, #1
	cmp	w1, 1
	bls	.L246
	mov	w25, 0
	mov	w23, 0
	mov	w24, 0
	mov	w21, 0
	b	.L249
	.p2align 3
.L247:
	add	w21, w21, 1
	cmp	w21, 8
	beq	.L367
.L249:
	ldr	w0, [x22, 64]
	asr	x0, x0, x21
	tbz	x0, 0, .L247
	add	x1, x29, 152
	mov	w0, w21
	bl	AIP_GetChnStatus
	ldr	w1, [x29, 152]
	lsl	x2, x20, x21
	orr	w3, w23, w2
	cbnz	w1, .L248
	ldr	w0, [x29, 160]
	add	w21, w21, 1
	orr	w24, w24, w2
	cmp	w0, 1
	csel	w23, w3, w23, eq
	cmp	w21, 8
	bne	.L249
	.p2align 2
.L367:
	ldr	w26, [x22, 136]
	uxtw	x27, w23
	mov	w21, 0
	str	w24, [x22, 136]
	b	.L251
	.p2align 3
.L250:
	add	w21, w21, 1
	cmp	w21, 8
	beq	.L368
.L251:
	asr	x0, x27, x21
	tbz	x0, 0, .L250
	mov	w0, w21
	add	w21, w21, 1
	bl	AIP_FlushCutoffFlag
	cmp	w21, 8
	bne	.L251
.L368:
	cbnz	w25, .L369
	sub	w24, w24, w26
	cmp	w24, w23
	bls	.L370
	mov	w0, 2
	str	wzr, [x22, 132]
	str	w0, [x22, 140]
.L257:
	ldr	w0, [x22, 104]
	cbz	w0, .L262
.L375:
	and	w0, w0, 15
	cbnz	w0, .L262
	ldr	w0, [x22, 108]
	cbz	w0, .L264
	ldp	w0, w1, [x22, 68]
	ldr	w2, [x22, 76]
	bl	ENGINECheckAipData.isra.0
	cbnz	w0, .L371
.L264:
	ldr	w0, [x22, 112]
	cbz	w0, .L205
	ldp	w0, w1, [x22, 84]
	ldr	w2, [x22, 100]
	bl	ENGINECheckAopSpace.isra.1
	cbz	w0, .L205
	mov	x0, x22
	bl	ENGINEOutFrame
	b	.L205
	.p2align 3
.L193:
	ldr	x0, [x28, 48]
	cbz	x0, .L372
	ldr	w2, [x0, 128]
	cbz	w2, .L373
.L200:
	mov	w2, 1
	str	w2, [x0, 128]
	ldr	w2, [x1, 4]
	tbnz	x2, 2, .L269
	mov	x2, x0
.L199:
	ldr	w0, [x1, 4]
	mov	x27, x2
	and	w0, w0, -121
	str	w0, [x1, 4]
	ldr	w0, [x1, 4]
	orr	w0, w0, 4
	str	w0, [x1, 4]
	b	.L202
	.p2align 3
.L363:
	ldr	w0, [x29, 156]
	cmp	w0, 1
	beq	.L374
	ldr	w0, [x27, 72]
	orr	w0, w0, w22
	str	w0, [x27, 72]
.L216:
	mov	x1, x27
	mov	w0, w21
	bl	ENGINECheckRbfInData
	cbz	w0, .L213
	ldr	w0, [x27, 76]
	orr	w22, w0, w22
	str	w22, [x27, 76]
	b	.L213
	.p2align 3
.L248:
	orr	w25, w25, w2
	b	.L247
	.p2align 3
.L269:
	mov	x27, x0
	b	.L202
	.p2align 3
.L369:
	ldr	w0, [x22, 140]
.L246:
	cmp	w0, 1
	beq	.L258
	cmp	w0, 2
	bne	.L257
	ldr	w0, [x22, 132]
	cmp	w0, 2
	bne	.L257
.L258:
	mov	w21, 0
	.p2align 2
.L255:
	ldr	w1, [x22, 84]
	mov	w0, w21
	asr	x1, x1, x21
	tbnz	x1, 0, .L260
	ldr	w1, [x22, 88]
	asr	x1, x1, x21
	tbz	x1, 0, .L261
.L260:
	bl	AOP_Flush
.L261:
	add	w21, w21, 1
	cmp	w21, 8
	bne	.L255
	ldr	w0, [x22, 104]
	str	wzr, [x22, 132]
	str	wzr, [x22, 140]
	cbnz	w0, .L375
.L262:
	ldp	w0, w1, [x22, 84]
	ldr	w2, [x22, 100]
	bl	ENGINECheckAopSpace.isra.1
	cbz	w0, .L205
	ldp	w0, w1, [x22, 68]
	ldr	w2, [x22, 76]
	bl	ENGINECheckAipData.isra.0
	cbz	w0, .L205
	mov	x0, x22
	bl	ENGINEMixFrame
	mov	x0, x22
	bl	ENGINEOutFrame
	b	.L205
	.p2align 3
.L362:
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 164]
	cmp	w1, w0
	bne	.L210
	ldr	w0, [x29, 116]
	orr	w0, w0, w6
	str	w0, [x29, 116]
	b	.L211
	.p2align 3
.L366:
	ldr	w0, [x22, 80]
	stp	wzr, wzr, [x29, 128]
	uxtw	x1, w0
	tbnz	x0, 0, .L225
	mov	w23, 1
	.p2align 2
.L229:
	asr	x0, x1, x23
	tbnz	x0, 0, .L376
	add	w23, w23, 1
	cmp	w23, 8
	bne	.L229
	ldr	w0, [x22, 64]
	uxtw	x1, w0
	tbnz	x0, 0, .L205
	mov	w2, 1
.L228:
	mov	w21, 1
	b	.L232
	.p2align 3
.L231:
	add	w21, w21, 1
	cmp	w21, 8
	beq	.L205
.L232:
	asr	x0, x1, x21
	tbz	x0, 0, .L231
	cmp	w21, 8
	cset	w0, eq
.L227:
	orr	w0, w2, w0
	cbnz	w0, .L205
.L267:
	add	x1, x29, 136
	mov	w0, w21
	bl	AIP_GetChnStatus
	ldr	w24, [x29, 136]
	cbnz	w24, .L233
	ldr	w0, [x22, 12]
	stp	wzr, wzr, [x22, 20]
	cmp	w0, 2
	beq	.L205
.L234:
	ldp	w0, w1, [x22, 84]
	ldr	w2, [x22, 100]
	bl	ENGINECheckAopSpace.isra.1
	cbz	w0, .L205
	ldp	w0, w1, [x22, 68]
	ldr	w2, [x22, 76]
	bl	ENGINECheckAipData.isra.0
	cbz	w0, .L205
	ldr	w2, [x22, 120]
	mov	w0, w21
	mov	w1, w23
	bl	ENGINEOutRawFrame
	add	x1, x29, 152
	mov	w0, w23
	bl	AOP_GetChnStatus
	add	x1, x29, 152
	mov	w0, w23
	bl	AOP_ProcDelayCompensation
	b	.L205
	.p2align 3
.L373:
	bl	ENGINEDestroyChn
	mov	w0, w19
	bl	ENGINECreateChn
	str	x0, [x28, 48]
	cbz	x0, .L201
	ldr	x1, [x28]
	b	.L200
	.p2align 3
.L359:
	str	wzr, [x27, 128]
	b	.L203
	.p2align 3
.L374:
	ldr	w0, [x27, 68]
	orr	w0, w0, w22
	str	w0, [x27, 68]
	b	.L216
	.p2align 3
.L361:
	ldr	w0, [x29, 120]
	orr	w0, w0, w6
	str	w0, [x29, 120]
	b	.L209
	.p2align 3
.L370:
	mov	w0, 1
	str	w0, [x22, 140]
	b	.L258
	.p2align 3
.L372:
	mov	w0, w19
	bl	ENGINECreateChn
	str	x0, [x28, 48]
	mov	x27, x0
	cbz	x0, .L201
	mov	w0, 1
	str	w0, [x27, 128]
	mov	x2, x27
	ldr	x1, [x28]
	ldr	w0, [x1, 4]
	tbnz	x0, 2, .L198
	b	.L199
	.p2align 3
.L376:
	ldr	w0, [x22, 64]
	cmp	w23, 8
	cset	w2, eq
	uxtw	x1, w0
	tbz	x0, 0, .L228
	mov	w0, 0
	mov	w21, 0
	b	.L227
	.p2align 3
.L364:
	ldr	w0, [x27, 68]
	orr	w22, w0, w22
	str	w22, [x27, 68]
	b	.L213
.L201:
	ldr	x0, [x28]
	ldr	w1, [x0, 4]
	tbnz	x1, 2, .L205
	ldr	w1, [x0, 4]
	mov	w2, 3
	bfi	w1, w2, 3, 4
	str	w1, [x0, 4]
	ldr	w1, [x0, 4]
	orr	w1, w1, 4
	str	w1, [x0, 4]
	b	.L205
.L233:
	ldr	w0, [x22, 12]
	cmp	w0, 2
	bne	.L234
	ldp	w0, w1, [x22, 84]
	ldr	w2, [x22, 100]
	bl	ENGINECheckAopSpace.isra.1
	cmp	w24, 1
	ccmp	w0, wzr, 4, eq
	beq	.L205
	ldp	w24, w0, [x22, 20]
	stp	w24, w0, [x29, 128]
	orr	w0, w24, w0
	cbz	w0, .L377
.L235:
	cbz	w24, .L238
	ldr	w25, [x22, 120]
	cmp	w25, w24
	bhi	.L239
	ldp	w0, w1, [x22, 68]
	ldr	w2, [x22, 76]
	bl	ENGINECheckAipData.isra.0
	cbz	w0, .L205
.L240:
	mov	w0, w21
	mov	w2, w25
	mov	w1, w23
	bl	ENGINEOutRawFrame
	ldr	w0, [x29, 128]
	sub	w0, w0, w25
	str	w0, [x29, 128]
	cbnz	w0, .L241
	ldr	w0, [x22, 120]
	cmp	w25, w0
	bcs	.L241
	sub	w21, w0, w25
	ldr	w0, [x29, 132]
	cmp	w21, w0
	bls	.L378
.L241:
	add	x1, x29, 152
	mov	w0, w23
	bl	AOP_GetChnStatus
	add	x1, x29, 152
	mov	w0, w23
	bl	AOP_ProcDelayCompensation
	ldp	w1, w0, [x29, 128]
.L243:
	stp	w1, w0, [x22, 20]
	b	.L205
.L377:
	mov	w0, w21
	bl	AIP_QueryBusy
	cmp	w0, 3
	bls	.L205
	mov	w0, w21
	add	x2, x29, 132
	add	x1, x29, 128
	bl	AIP_GetCompressInfo
	ldp	w24, w0, [x29, 128]
	cbnz	w24, .L237
	cbnz	w0, .L237
	mov	w0, w21
	bl	AIP_GetRingIn
	bl	RBUF_Flush
	ldp	w24, w0, [x29, 128]
.L237:
	stp	w24, w0, [x22, 20]
	b	.L235
	.p2align 3
.L371:
	mov	x0, x22
	bl	ENGINEMixFrame
	b	.L264
.L225:
	ldr	w0, [x22, 64]
	uxtw	x1, w0
	tbnz	x0, 0, .L273
	mov	w2, 0
	mov	w23, 0
	b	.L228
.L273:
	mov	w21, 0
	mov	w23, 0
	b	.L267
.L238:
	ldr	w1, [x29, 132]
	mov	w0, 0
	cbz	w1, .L243
	ldr	w21, [x22, 120]
	mov	w0, w23
	cmp	w21, w1
	csel	w21, w21, w1, ls
	bl	AOP_GetRingOut
	cbz	x0, .L244
	mov	w2, w21
	mov	x1, 0
	bl	RBUF_WriteDataDma
.L244:
	ldp	w1, w0, [x29, 128]
	sub	w0, w0, w21
	stp	w1, w0, [x22, 20]
	b	.L205
.L239:
	mov	w0, w21
	bl	AIP_QueryBusy
	cmp	w0, w24
	bcc	.L205
	mov	w25, w24
	b	.L240
.L378:
	mov	w0, w23
	bl	AOP_GetRingOut
	cbz	x0, .L242
	mov	w2, w21
	mov	x1, 0
	bl	RBUF_WriteDataDma
.L242:
	ldr	w0, [x29, 132]
	sub	w21, w0, w21
	str	w21, [x29, 132]
	b	.L241
	.size	ENGINE_Process, .-ENGINE_Process
	.align	3
.LC10:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ENGINE_Init
	.type	ENGINE_Init, %function
ENGINE_Init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w23, 1
	add	x21, x19, 768
	bl	_mcount
	ldr	x20, .LC11
	ldr	x22, .LC12
	add	x20, x20, 48
	.p2align 2
.L380:
	mov	x2, 128
	ldr	x3, [x22, 40]
	str	x19, [x20, -48]
	mov	x0, x19
	mov	w1, 0
	add	x19, x19, x2
	blr	x3
	ldr	w0, [x19, -124]
	cmp	x19, x21
	orr	w0, w0, 4
	str	w0, [x19, -124]
	ldr	w0, [x19, -124]
	bfi	w0, w23, 0, 2
	str	w0, [x19, -124]
	str	xzr, [x20], 8
	bne	.L380
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	ENGINE_Init, .-ENGINE_Init
	.align	3
.LC11:
	.xword	.LANCHOR0
	.align	3
.LC12:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ENGINE_DeInit
	.type	ENGINE_DeInit, %function
ENGINE_DeInit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC13
	add	x19, x20, 48
	add	x20, x20, 96
	.p2align 2
.L383:
	ldr	x0, [x19]
	bl	ENGINEDestroyChn
	str	xzr, [x19], 8
	cmp	x19, x20
	bne	.L383
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ENGINE_DeInit, .-ENGINE_DeInit
	.align	3
.LC13:
	.xword	.LANCHOR0
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	g_stEneRm, %object
	.size	g_stEneRm, 96
g_stEneRm:
	.zero	96
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
