#include "hi_asm_define.h"
	.arch armv7-a
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"dsp_util.c"
	.text
	.align	2
	.type	VolumedB2Coef, %function
VolumedB2Coef:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	uxtb	r0, r0
	cmp	r0, #138
	bhi	.L5
	rsb	r3, r0, #139
	cmp	r3, #98
	bhi	.L6
	cmp	r1, #0
	ldr	r2, .L9
	beq	.L8
	ble	.L4
	rsb	r0, r0, #138
	ldr	r3, [r2, r3, asl #2]
	ldr	r0, [r2, r0, asl #2]
	rsb	r0, r3, r0
	mul	r0, r1, r0
	add	r0, r3, r0, lsr #3
	ldmfd	sp, {fp, sp, pc}
.L6:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L5:
	movw	r0, #48505
	movt	r0, 1016
	ldmfd	sp, {fp, sp, pc}
.L4:
	rsb	r0, r0, #140
	ldr	r3, [r2, r3, asl #2]
	ldr	r0, [r2, r0, asl #2]
	rsb	r0, r3, r0
	mul	r0, r1, r0
	sub	r0, r3, r0, lsr #3
	ldmfd	sp, {fp, sp, pc}
.L8:
	ldr	r0, [r2, r3, asl #2]
	ldmfd	sp, {fp, sp, pc}
.L10:
	.align	2
.L9:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VolumedB2Coef, .-VolumedB2Coef
	.align	2
	.type	AOE_FMUL_16_Q15.part.0, %function
AOE_FMUL_16_Q15.part.0:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	sxth	r0, r0
	cmp	r1, #8388608
	smull	r0, r1, r1, r0
	mov	r0, r0, lsr #23
	orr	r0, r0, r1, asl #9
	bls	.L14
	mov	r3, r0, asr #31
	cmp	r3, r0, asr #15
	eorne	r0, r3, #32512
	eorne	r0, r0, #255
.L14:
	uxth	r0, r0
	sxth	r0, r0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AOE_FMUL_16_Q15.part.0, .-AOE_FMUL_16_Q15.part.0
	.align	2
	.type	Volume16BitPro_Coef, %function
Volume16BitPro_Coef:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	mov	r4, r0
	mov	r5, r2
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r6, r0, r1, lsl #1
.L18:
	cmp	r5, #0
	mov	r1, r5
	mov	r0, r5
	ldrh	r3, [r4]
	beq	.L17
	sxth	r0, r3
	bl	AOE_FMUL_16_Q15.part.0
.L17:
	strh	r0, [r4], #2
	cmp	r4, r6
	bne	.L18
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Volume16BitPro_Coef, .-Volume16BitPro_Coef
	.align	2
	.type	AOE_FMUL_24_Q15, %function
AOE_FMUL_24_Q15:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	beq	.L26
	cmp	r1, #8388608
	bls	.L27
	mov	r0, r0, asr #8
	smull	r0, r1, r0, r1
	mov	r0, r0, lsr #23
	orr	r0, r0, r1, asl #9
	mov	r3, r0, asr #31
	cmp	r3, r0, asr #23
	movwne	r0, #65535
	movtne	r0, 127
	eorne	r0, r0, r3
	mov	r0, r0, asl #8
	ldmfd	sp, {fp, sp, pc}
.L27:
	smull	r0, r1, r0, r1
	mov	r0, r0, lsr #23
	orr	r0, r0, r1, asl #9
	ldmfd	sp, {fp, sp, pc}
.L26:
	mov	r0, r1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AOE_FMUL_24_Q15, .-AOE_FMUL_24_Q15
	.align	2
	.type	Volume24BitPro_Coef, %function
Volume24BitPro_Coef:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	sub	r4, r1, #1
	mov	r5, r0
	mov	r6, r2
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L30:
	ldr	r0, [r5]
	mov	r1, r6
	bl	AOE_FMUL_24_Q15
	sub	r4, r4, #1
	cmn	r4, #1
	str	r0, [r5], #4
	bne	.L30
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Volume24BitPro_Coef, .-Volume24BitPro_Coef
	.align	2
	.global	UTIL_CalcFrameSize
	.type	UTIL_CalcFrameSize, %function
UTIL_CalcFrameSize:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #16
	moveq	r0, r0, asl #1
	movne	r0, r0, asl #2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_CalcFrameSize, .-UTIL_CalcFrameSize
	.align	2
	.global	UTIL_LatencyMs2ByteSize
	.type	UTIL_LatencyMs2ByteSize, %function
UTIL_LatencyMs2ByteSize:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mul	r1, r1, r0
	movw	r0, #19923
	movt	r0, 4194
	mul	r2, r2, r1
	umull	r2, r3, r2, r0
	mov	r0, r3, lsr #6
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_LatencyMs2ByteSize, .-UTIL_LatencyMs2ByteSize
	.global	__aeabi_uidiv
	.align	2
	.global	UTIL_LatencyMs
	.type	UTIL_LatencyMs, %function
UTIL_LatencyMs:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r3, #1000
	mul	r1, r2, r1
	mul	r0, r3, r0
	bl	__aeabi_uidiv
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_LatencyMs, .-UTIL_LatencyMs
	.align	2
	.global	UTIL_LRLinearGainProcess
	.type	UTIL_LRLinearGainProcess, %function
UTIL_LRLinearGainProcess:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrsh	lr, [fp, #4]
	cmp	r3, lr
	beq	.L39
	cmp	r2, #0
	ble	.L49
	sub	r1, r1, #2
	mov	r4, #0
	b	.L45
.L51:
#APP
	smulwb  ip, ip, lr
	qadd    ip, ip, ip
.L44:
	add	r4, r4, #1
	strh	ip, [r1, #2]!
	cmp	r4, r2
	add	r0, r0, #2
	beq	.L50
.L45:
	tst	r4, #1
	ldrsh	ip, [r0]
	bne	.L51
#APP
	smulwb  ip, ip, r3
	qadd    ip, ip, ip
	b	.L44
.L50:
	ldmfd	sp, {r4, fp, sp, pc}
.L39:
	cmp	r2, #0
	ldmlefd	sp, {r4, fp, sp, pc}
	sub	r1, r1, #2
	add	r2, r0, r2, lsl #1
.L42:
	ldrsh	ip, [r0], #2
#APP
	smulwb  ip, ip, r3
	qadd    ip, ip, ip
	cmp	r0, r2
	strh	ip, [r1, #2]!
	bne	.L42
	ldmfd	sp, {r4, fp, sp, pc}
.L49:
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_LRLinearGainProcess, .-UTIL_LRLinearGainProcess
	.align	2
	.global	UTIL_UpmixPcmData
	.type	UTIL_UpmixPcmData, %function
UTIL_UpmixPcmData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r2, #12]
	cmp	r3, #24
	beq	.L88
	cmp	r3, #16
	ldmnefd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	ldr	r3, [r2, #8]
	ldr	r5, [r2]
	cmp	r3, #0
	beq	.L64
	mov	r6, #0
	mov	r8, r0, asl #1
	mov	r4, r6
	mov	r7, r1
.L65:
	cmp	r0, #0
	beq	.L71
	add	ip, r7, #2
	mov	r3, #0
.L68:
	ldr	lr, [r2, #16]
	add	ip, ip, #2
	cmp	lr, r3
	add	r3, r3, #1
	ldrhih	r4, [r5], #2
	cmp	r3, r0
	strh	r4, [ip, #-4]
	bne	.L68
	add	r7, r7, r8
.L71:
	ldr	r3, [r2, #8]
	add	r6, r6, #1
	cmp	r3, r6
	bhi	.L65
	ldr	ip, [r2, #12]
	str	r1, [r2]
	cmp	ip, #16
	str	r0, [r2, #16]
	movne	r8, r0, asl #2
.L72:
	mul	r3, r3, r8
	str	r3, [r2, #4]
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L88:
	ldr	r3, [r2, #8]
	ldr	r5, [r2]
	cmp	r3, #0
	beq	.L54
	mov	r6, #0
	mov	r8, r0, asl #2
	mov	r4, r6
	mov	r7, r1
.L55:
	cmp	r0, #0
	beq	.L61
	add	ip, r7, #4
	mov	r3, #0
.L58:
	ldr	lr, [r2, #16]
	add	ip, ip, #4
	cmp	lr, r3
	add	r3, r3, #1
	ldrhi	r4, [r5], #4
	cmp	r3, r0
	str	r4, [ip, #-8]
	bne	.L58
	add	r7, r7, r8
.L61:
	ldr	r3, [r2, #8]
	add	r6, r6, #1
	cmp	r3, r6
	bhi	.L55
	ldr	ip, [r2, #12]
	str	r1, [r2]
	cmp	ip, #16
	str	r0, [r2, #16]
	moveq	r8, r0, asl #1
	b	.L72
.L54:
	str	r1, [r2]
	mov	r8, r0, asl #2
	str	r0, [r2, #16]
	b	.L72
.L64:
	str	r1, [r2]
	mov	r8, r0, asl #1
	str	r0, [r2, #16]
	b	.L72
	UNWIND(.fnend)
	.size	UTIL_UpmixPcmData, .-UTIL_UpmixPcmData
	.align	2
	.global	UTIL_DownmixPcmData
	.type	UTIL_DownmixPcmData, %function
UTIL_DownmixPcmData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r2, #12]
	cmp	r3, #24
	beq	.L123
	cmp	r3, #16
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r2, #8]
	ldr	r6, [r2]
	cmp	r3, #0
	beq	.L100
	mov	r7, r0, asl #1
	mov	lr, r1
	mov	r5, #0
.L101:
	cmp	r0, #0
	beq	.L106
	add	ip, r6, #2
	mov	r3, #0
.L103:
	cmp	r0, r3
	add	r3, r3, #1
	add	ip, ip, #2
	ldrhih	r4, [ip, #-4]
	strhih	r4, [lr], #2
	cmp	r3, r0
	bne	.L103
	add	r6, r6, r7
.L106:
	ldr	r3, [r2, #8]
	add	r5, r5, #1
	cmp	r3, r5
	bhi	.L101
	ldr	ip, [r2, #12]
	str	r1, [r2]
	cmp	ip, #16
	str	r0, [r2, #16]
	movne	r7, r0, asl #2
.L107:
	mul	r3, r3, r7
	str	r3, [r2, #4]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L123:
	ldr	r3, [r2, #8]
	ldr	r6, [r2]
	cmp	r3, #0
	beq	.L91
	mov	r7, r0, asl #2
	mov	lr, r1
	mov	r5, #0
.L92:
	cmp	r0, #0
	beq	.L97
	add	ip, r6, #4
	mov	r3, #0
.L94:
	cmp	r0, r3
	add	r3, r3, #1
	add	ip, ip, #4
	ldrhi	r4, [ip, #-8]
	strhi	r4, [lr], #4
	cmp	r3, r0
	bne	.L94
	add	r6, r6, r7
.L97:
	ldr	r3, [r2, #8]
	add	r5, r5, #1
	cmp	r3, r5
	bhi	.L92
	ldr	ip, [r2, #12]
	str	r1, [r2]
	cmp	ip, #16
	str	r0, [r2, #16]
	moveq	r7, r0, asl #1
	b	.L107
.L91:
	str	r1, [r2]
	mov	r7, r0, asl #2
	str	r0, [r2, #16]
	b	.L107
.L100:
	str	r1, [r2]
	mov	r7, r0, asl #1
	str	r0, [r2, #16]
	b	.L107
	UNWIND(.fnend)
	.size	UTIL_DownmixPcmData, .-UTIL_DownmixPcmData
	.align	2
	.global	UTIL_UnifyBitDepthPcmData
	.type	UTIL_UnifyBitDepthPcmData, %function
UTIL_UnifyBitDepthPcmData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r2, #16]
	cmp	r0, #16
	ldr	ip, [r2, #8]
	mul	r3, ip, r3
	beq	.L135
	cmp	r0, #24
	ldmnefd	sp, {r4, fp, sp, pc}
	ldr	r0, [r2, #12]
	cmp	r0, #16
	ldmnefd	sp, {r4, fp, sp, pc}
	cmp	r3, #0
	ldr	lr, [r2]
	movne	r4, r1
	subne	lr, lr, #2
	movne	ip, #0
	beq	.L132
.L131:
	ldrh	r0, [lr, #2]!
	add	ip, ip, #1
	cmp	ip, r3
	mov	r0, r0, asl #16
	str	r0, [r4], #4
	bne	.L131
.L132:
	ldr	r3, [r2, #4]
	mov	r0, #24
	str	r1, [r2]
	str	r0, [r2, #12]
	mov	r3, r3, asl #1
	str	r3, [r2, #4]
	ldmfd	sp, {r4, fp, sp, pc}
.L135:
	ldr	r0, [r2, #12]
	cmp	r0, #24
	ldmnefd	sp, {r4, fp, sp, pc}
	cmp	r3, #0
	ldr	lr, [r2]
	movne	r4, r1
	subne	lr, lr, #4
	movne	ip, #0
	beq	.L129
.L128:
	ldr	r0, [lr, #4]!
	add	ip, ip, #1
	cmp	ip, r3
	mov	r0, r0, lsr #16
	strh	r0, [r4], #2
	bne	.L128
.L129:
	ldr	r3, [r2, #4]
	mov	r0, #16
	str	r1, [r2]
	str	r0, [r2, #12]
	mov	r3, r3, lsr #1
	str	r3, [r2, #4]
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_UnifyBitDepthPcmData, .-UTIL_UnifyBitDepthPcmData
	.align	2
	.global	UTILS_GainBalanceCoef
	.type	UTILS_GainBalanceCoef, %function
UTILS_GainBalanceCoef:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	blt	.L142
	ldr	r3, [r0]
	rsb	r2, r2, #50
	movw	r1, #34079
	movt	r1, 20971
	mul	r2, r3, r2
	umull	r2, r3, r2, r1
	mov	r3, r3, lsr #4
	str	r3, [r0]
	ldmfd	sp, {fp, sp, pc}
.L142:
	ldr	r3, [r1]
	add	r2, r2, #50
	movw	r0, #34079
	movt	r0, 20971
	mul	r2, r3, r2
	umull	r2, r3, r2, r0
	mov	r3, r3, lsr #4
	str	r3, [r1]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UTILS_GainBalanceCoef, .-UTILS_GainBalanceCoef
	.align	2
	.global	UTIL_LRGainProcess
	.type	UTIL_LRGainProcess, %function
UTIL_LRGainProcess:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	mov	r5, r0
	ldr	r0, [r2]
	mov	r6, r2
	ldr	r3, [r5, #16]
	mov	r7, r1
	ldr	r4, [r5, #8]
	cmp	r0, #1
	mul	r4, r4, r3
	beq	.L199
	ldr	r1, [r2, #12]
	ldr	r0, [r2, #4]
	bl	VolumedB2Coef
	ldr	r1, [r6, #16]
	str	r0, [fp, #-36]
	ldr	r0, [r6, #8]
	bl	VolumedB2Coef
	sub	r1, fp, #28
	mov	r2, r7
	str	r0, [r1, #-4]!
	sub	r0, fp, #36
	bl	UTILS_GainBalanceCoef
	ldr	r6, [fp, #-36]
	cmp	r6, #8388608
	beq	.L200
	ldr	r3, [r5, #12]
	cmp	r3, #16
	beq	.L164
	cmp	r3, #24
	bne	.L143
	ldr	r7, [fp, #-32]
.L173:
	cmp	r6, r7
	ldr	r0, [r5]
	beq	.L174
	cmp	r4, #0
	sub	r4, r4, #1
	subne	r5, r0, #4
	beq	.L143
.L178:
	tst	r4, #1
	mov	r1, r6
	ldr	r0, [r5, #4]!
	moveq	r1, r7
	bl	AOE_FMUL_24_Q15
	subs	r4, r4, #1
	str	r0, [r5]
	bcs	.L178
.L143:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L199:
	ldr	r3, [r5, #12]
	cmp	r3, #16
	beq	.L146
	cmp	r3, #24
	bne	.L143
	ldrsh	r0, [r2, #4]
	ldrsh	ip, [r2, #8]
	ldr	r2, [r5]
	cmp	r0, ip
	beq	.L155
	cmp	r4, #0
	movgt	r1, #0
	bgt	.L160
	b	.L143
.L201:
#APP
	smulwb  r3, r3, ip
	qadd    r3, r3, r3
.L159:
	add	r1, r1, #1
	str	r3, [r2], #4
	cmp	r4, r1
	beq	.L143
.L160:
	tst	r1, #1
	ldr	r3, [r2]
	bne	.L201
#APP
	smulwb  r3, r3, r0
	qadd    r3, r3, r3
	b	.L159
.L200:
	ldr	r7, [fp, #-32]
	cmp	r7, #8388608
	beq	.L143
	ldr	r3, [r5, #12]
	cmp	r3, #16
	beq	.L180
	cmp	r3, #24
	beq	.L173
	b	.L143
.L146:
	ldrsh	r0, [r2, #4]
	ldrsh	ip, [r2, #8]
	ldr	r2, [r5]
	cmp	r0, ip
	beq	.L148
	cmp	r4, #0
	movgt	r1, #0
	bgt	.L154
	b	.L143
.L202:
#APP
	smulwb  r3, r3, ip
	qadd    r3, r3, r3
.L153:
	add	r1, r1, #1
	strh	r3, [r2], #2
	cmp	r4, r1
	beq	.L143
.L154:
	tst	r1, #1
	ldrsh	r3, [r2]
	bne	.L202
#APP
	smulwb  r3, r3, r0
	qadd    r3, r3, r3
	b	.L153
.L164:
	ldr	r7, [fp, #-32]
	ldr	r0, [r5]
	cmp	r6, r7
	beq	.L166
.L181:
	cmp	r4, #0
	sub	r4, r4, #1
	subne	r5, r0, #2
	bne	.L172
	b	.L143
.L203:
	cmp	r6, #0
	mov	r1, r6
	mov	r3, r6
	beq	.L171
.L198:
	bl	AOE_FMUL_16_Q15.part.0
	mov	r3, r0
.L171:
	subs	r4, r4, #1
	strh	r3, [r5]
	bcc	.L143
.L172:
	tst	r4, #1
	ldrsh	r0, [r5, #2]!
	bne	.L203
	cmp	r7, #0
	mov	r1, r7
	mov	r3, r7
	beq	.L171
	b	.L198
.L155:
	cmp	r4, #0
	ble	.L143
	sub	r2, r2, #4
	mov	r1, #0
.L157:
	ldr	r3, [r2, #4]!
#APP
	smulwb  r3, r3, r0
	qadd    r3, r3, r3
	add	r1, r1, #1
	str	r3, [r2]
	cmp	r4, r1
	bne	.L157
	b	.L143
.L148:
	cmp	r4, #0
	ble	.L143
	add	r4, r2, r4, lsl #1
.L151:
	ldrsh	r3, [r2]
#APP
	smulwb  r3, r3, r0
	qadd    r3, r3, r3
	strh	r3, [r2], #2
	cmp	r2, r4
	bne	.L151
	b	.L143
.L166:
	mov	r2, r6
	mov	r1, r4
	bl	Volume16BitPro_Coef
	b	.L143
.L174:
	mov	r2, r6
	mov	r1, r4
	bl	Volume24BitPro_Coef
	b	.L143
.L180:
	ldr	r0, [r5]
	b	.L181
	UNWIND(.fnend)
	.size	UTIL_LRGainProcess, .-UTIL_LRGainProcess
	.global	__aeabi_idiv
	.align	2
	.global	UTIL_LRFadeGainProcess
	.type	UTIL_LRFadeGainProcess, %function
UTIL_LRFadeGainProcess:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	mov	r5, r0
	mov	r9, r1
	ldr	r0, [r2, #4]
	ldr	r1, [r2, #12]
	mov	r6, r3
	bl	VolumedB2Coef
	ldr	r1, [r6, #12]
	mov	r4, r0
	ldr	r0, [r6, #4]
	bl	VolumedB2Coef
	ldr	r7, [r5, #8]
	ldr	r6, [r5]
	mov	r1, r7
	rsb	r0, r4, r0
	bl	__aeabi_idiv
	cmp	r7, #0
	str	r4, [fp, #-52]
	str	r4, [fp, #-48]
	mov	r8, r0
	beq	.L204
	mov	r7, r6
	mov	r10, #0
	b	.L210
.L206:
	cmp	r3, #24
	beq	.L215
.L209:
	ldr	r2, [r5, #8]
	add	r10, r10, #1
	add	r3, r4, r8
	str	r3, [fp, #-52]
	cmp	r2, r10
	str	r3, [fp, #-48]
	mov	r4, r3
	bls	.L204
.L210:
	mov	r2, r9
	sub	r1, fp, #48
	sub	r0, fp, #52
	bl	UTILS_GainBalanceCoef
	ldr	r3, [r5, #12]
	cmp	r3, #16
	bne	.L206
	ldr	r1, [fp, #-52]
	ldrh	r0, [r6]
	cmp	r1, #0
	moveq	r0, r1
	beq	.L207
	sxth	r0, r0
	bl	AOE_FMUL_16_Q15.part.0
.L207:
	strh	r0, [r6]
	ldr	r1, [fp, #-48]
	ldrh	r0, [r6, #2]
	cmp	r1, #0
	moveq	r0, r1
	beq	.L208
	sxth	r0, r0
	bl	AOE_FMUL_16_Q15.part.0
.L208:
	strh	r0, [r6, #2]
	add	r6, r6, #4
	b	.L209
.L215:
	ldr	r1, [fp, #-52]
	add	r7, r7, #8
	ldr	r0, [r7, #-8]
	bl	AOE_FMUL_24_Q15
	str	r0, [r7, #-8]
	ldr	r1, [fp, #-48]
	ldr	r0, [r7, #-4]
	bl	AOE_FMUL_24_Q15
	str	r0, [r7, #-4]
	b	.L209
.L204:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_LRFadeGainProcess, .-UTIL_LRFadeGainProcess
	.align	2
	.global	UTIL_MixerPro
	.type	UTIL_MixerPro, %function
UTIL_MixerPro:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r3, #16
	beq	.L225
	cmp	r3, #24
	ldmnefd	sp, {fp, sp, pc}
	sub	r1, r1, #4
	mov	ip, #0
.L222:
	ldr	r3, [r0]
	ldr	lr, [r1, #4]!
#APP
	qadd r3, r3, lr
	add	ip, ip, #1
	str	r3, [r0], #4
	cmp	ip, r2
	bne	.L222
	ldmfd	sp, {fp, sp, pc}
.L225:
	sub	r1, r1, #2
	mov	ip, #0
.L220:
	ldrsh	lr, [r1, #2]!
	add	ip, ip, #1
	ldrsh	r3, [r0]
	add	r3, r3, lr
	mov	lr, r3, asr #31
	cmp	lr, r3, asr #15
	eorne	r3, lr, #32512
	eorne	r3, r3, #255
	cmp	ip, r2
	strh	r3, [r0], #2
	bne	.L220
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_MixerPro, .-UTIL_MixerPro
	.align	2
	.global	UTIL_ChannelModeProcess
	.type	UTIL_ChannelModeProcess, %function
UTIL_ChannelModeProcess:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #16]
	sub	r1, r1, #1
	ldr	ip, [r0, #8]
	mul	r3, ip, r3
	cmp	r1, #6
	ldrls	pc, [pc, r1, asl #2]
	b	.L226
.L229:
	.word	.L228
	.word	.L230
	.word	.L231
	.word	.L232
	.word	.L233
	.word	.L234
	.word	.L235
.L235:
	cmp	r3, #0
	ldr	r1, [r0]
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #16
	beq	.L328
	cmp	r2, #24
	ldmnefd	sp, {fp, sp, pc}
	mov	r2, #0
	mov	r0, r2
.L259:
	add	r2, r2, #1
	str	r0, [r1], #4
	cmp	r2, r3
	bne	.L259
.L226:
	ldmfd	sp, {fp, sp, pc}
.L234:
	cmp	r3, #0
	ldr	r1, [r0]
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #16
	beq	.L329
	cmp	r2, #24
	ldmnefd	sp, {fp, sp, pc}
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	mov	r2, #0
	mov	r0, r2
.L256:
	add	r2, r2, #1
	str	r0, [r1, #4]
	cmp	r2, r3
	add	r1, r1, #8
	bne	.L256
	ldmfd	sp, {fp, sp, pc}
.L228:
	cmp	r3, #0
	ldr	r1, [r0]
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #16
	beq	.L330
	cmp	r2, #24
	ldmnefd	sp, {fp, sp, pc}
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #8
	mov	r0, #0
.L241:
	ldmdb	r2, {r1, ip}
#APP
	qadd r1, r1, ip
	add	r0, r0, #1
	str	r1, [r2, #-8]
	cmp	r0, r3
	str	r1, [r2, #-4]
	add	r2, r2, #8
	bne	.L241
	ldmfd	sp, {fp, sp, pc}
.L230:
	cmp	r3, #0
	ldr	r1, [r0]
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #16
	beq	.L331
	cmp	r2, #24
	ldmnefd	sp, {fp, sp, pc}
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #8
	mov	r1, #0
.L244:
	ldr	r0, [r2, #-8]
	add	r1, r1, #1
	cmp	r1, r3
	add	r2, r2, #8
	str	r0, [r2, #-12]
	bne	.L244
	ldmfd	sp, {fp, sp, pc}
.L231:
	cmp	r3, #0
	ldr	r1, [r0]
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #16
	beq	.L332
	cmp	r2, #24
	ldmnefd	sp, {fp, sp, pc}
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #8
	mov	r1, #0
.L247:
	ldr	r0, [r2, #-4]
	add	r1, r1, #1
	cmp	r1, r3
	add	r2, r2, #8
	str	r0, [r2, #-16]
	bne	.L247
	ldmfd	sp, {fp, sp, pc}
.L232:
	cmp	r3, #0
	ldr	r1, [r0]
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #16
	beq	.L333
	cmp	r2, #24
	ldmnefd	sp, {fp, sp, pc}
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #8
	mov	r1, #0
.L250:
	ldr	ip, [r2, #-8]
	add	r1, r1, #1
	ldr	r0, [r2, #-4]
	cmp	r1, r3
	stmdb	r2, {r0, ip}
	add	r2, r2, #8
	bne	.L250
	ldmfd	sp, {fp, sp, pc}
.L233:
	cmp	r3, #0
	ldr	r1, [r0]
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #16
	beq	.L334
	cmp	r2, #24
	ldmnefd	sp, {fp, sp, pc}
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	mov	r2, #0
	mov	r0, r2
.L253:
	str	r0, [r1, r2, asl #3]
	add	r2, r2, #1
	cmp	r2, r3
	bne	.L253
	ldmfd	sp, {fp, sp, pc}
.L330:
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #4
	mov	r0, #0
.L240:
	ldrsh	ip, [r2, #-2]
	add	r0, r0, #1
	ldrsh	r1, [r2, #-4]
	add	r2, r2, #4
	add	r1, r1, ip
	mov	ip, r1, asr #31
	cmp	ip, r1, asr #15
	eorne	r1, ip, #32512
	eorne	r1, r1, #255
	cmp	r0, r3
	uxth	r1, r1
	strh	r1, [r2, #-8]
	strh	r1, [r2, #-6]
	bne	.L240
	ldmfd	sp, {fp, sp, pc}
.L329:
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	mov	r2, #0
	mov	r0, r2
.L255:
	add	r2, r2, #1
	strh	r0, [r1, #2]
	cmp	r2, r3
	add	r1, r1, #4
	bne	.L255
	ldmfd	sp, {fp, sp, pc}
.L334:
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #4
	mov	r1, #0
	mov	r0, r1
.L252:
	add	r1, r1, #1
	strh	r0, [r2, #-4]
	cmp	r1, r3
	add	r2, r2, #4
	bne	.L252
	ldmfd	sp, {fp, sp, pc}
.L333:
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #4
	mov	r1, #0
.L249:
	ldrh	ip, [r2, #-4]
	add	r1, r1, #1
	ldrh	r0, [r2, #-2]
	cmp	r1, r3
	add	r2, r2, #4
	strh	ip, [r2, #-6]
	strh	r0, [r2, #-8]
	bne	.L249
	ldmfd	sp, {fp, sp, pc}
.L332:
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #4
	mov	r1, #0
.L246:
	ldrh	r0, [r2, #-2]
	add	r1, r1, #1
	cmp	r1, r3
	add	r2, r2, #4
	strh	r0, [r2, #-8]
	bne	.L246
	ldmfd	sp, {fp, sp, pc}
.L331:
	movs	r3, r3, lsr #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r2, r1, #4
	mov	r1, #0
.L243:
	ldrh	r0, [r2, #-4]
	add	r1, r1, #1
	cmp	r1, r3
	add	r2, r2, #4
	strh	r0, [r2, #-6]
	bne	.L243
	ldmfd	sp, {fp, sp, pc}
.L328:
	mov	r2, #0
	mov	r0, r2
.L258:
	add	r2, r2, #1
	strh	r0, [r1], #2
	cmp	r2, r3
	bne	.L258
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_ChannelModeProcess, .-UTIL_ChannelModeProcess
	.align	2
	.global	FADE_Create
	.type	FADE_Create, %function
FADE_Create:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	movw	ip, #60927
	mov	r6, r1
	sub	r1, r1, #1
	movt	ip, 2
	cmp	r1, ip
	mov	r4, r3
	mov	r5, r0
	mov	r3, #0
	mov	r7, r2
	str	r3, [r0]
	ldmhifd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	bic	r3, r2, #8
	cmp	r3, #16
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	sub	r1, r4, #1
	cmp	r4, #8
	cmpne	r1, #1
	movhi	r8, #1
	movls	r8, #0
	ldmhifd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r1, #36
	mov	r0, #26
	bl	HI_ADSP_MALLOC
	subs	r9, r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, .L342
	mov	r2, #36
	mov	r1, r8
	ldr	r3, [r3, #20]
	blx	r3
	str	r7, [r9]
	stmib	r9, {r4, r6}
	str	r8, [r9, #32]
	str	r9, [r5]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L343:
	.align	2
.L342:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	FADE_Create, .-FADE_Create
	.align	2
	.global	FADE_Destroy
	.type	FADE_Destroy, %function
FADE_Destroy:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r1, r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	mov	r0, #26
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HI_ADSP_FREE
	UNWIND(.fnend)
	.size	FADE_Destroy, .-FADE_Destroy
	.align	2
	.global	FADE_Active
	.type	FADE_Active, %function
FADE_Active:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	cmp	r2, #0
	mov	r3, #0
	str	r3, [r0, #20]
	ldrne	r3, [r0, #8]
	movwne	ip, #18849
	str	r2, [r0, #12]
	movtne	ip, 53024
	streq	r2, [r0, #24]
	mulne	r2, r3, r2
	str	r1, [r0, #32]
	umullne	r2, r3, r2, ip
	movne	r3, r3, lsr #16
	addne	r3, r3, #1
	strne	r3, [r0, #24]
	cmp	r1, #1
	beq	.L353
	cmp	r1, #2
	moveq	r3, #18
	streq	r3, [r0, #16]
	ldmfd	sp, {fp, sp, pc}
.L353:
	mov	r2, #99
	mov	r3, #0
	str	r2, [r0, #16]
	str	r3, [r0, #28]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	FADE_Active, .-FADE_Active
	.align	2
	.global	FADE_Flush
	.type	FADE_Flush, %function
FADE_Flush:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	movw	ip, #60927
	sub	lr, r1, #1
	movt	ip, 2
	cmp	lr, ip
	mov	ip, r0
	ldmhifd	sp, {fp, sp, pc}
	bic	r0, r2, #8
	cmp	r0, #16
	ldmnefd	sp, {fp, sp, pc}
	adds	lr, ip, #0
	sub	r0, r3, #1
	movne	lr, #1
	cmp	r3, #8
	cmpne	r0, #1
	movhi	r0, #1
	movls	r0, #0
	cmp	lr, r0
	ldmlsfd	sp, {fp, sp, pc}
	ldr	r0, [ip, #4]
	ldr	lr, [ip]
	cmp	r3, r0
	beq	.L357
.L356:
	str	r2, [ip]
	mov	r0, ip
	str	r1, [ip, #8]
	str	r3, [ip, #4]
	ldr	r2, [ip, #12]
	ldr	r1, [ip, #32]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	FADE_Active
.L357:
	ldr	r0, [ip, #8]
	cmp	r0, r1
	bne	.L356
	cmp	r2, lr
	ldmeqfd	sp, {fp, sp, pc}
	b	.L356
	UNWIND(.fnend)
	.size	FADE_Flush, .-FADE_Flush
	.align	2
	.global	FADE_InProcess
	.type	FADE_InProcess, %function
FADE_InProcess:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r5, r0, #0
	mov	r4, r1
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, [r5, #32]
	cmp	r3, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	cmp	r2, #0
	ldr	r6, [r5, #4]
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r9, .L391
	sub	r10, r2, #1
	mov	r8, r6, asl #1
	mov	r7, r6, asl #2
.L376:
	ldr	r3, [r5, #28]
	cmp	r3, #0
	bne	.L361
	ldmia	r5, {r1, r2}
	cmp	r2, #0
	bne	.L367
	b	.L368
.L363:
	cmp	r1, #24
	beq	.L388
.L364:
	cmp	r3, r2
	beq	.L368
.L367:
	cmp	r1, #16
	add	r3, r3, #1
	bne	.L363
	ldrsh	r0, [r4]
	cmp	r0, #0
	beq	.L364
.L366:
	mov	r3, #1
	str	r3, [r5, #28]
	ldr	r3, [r5, #16]
	cmp	r1, #16
	ldr	r2, [r9, r3, asl #2]
	beq	.L373
.L390:
	cmp	r1, #24
	bne	.L374
	mov	r0, r4
	mov	r1, r6
	add	r4, r4, r7
	bl	Volume24BitPro_Coef
.L374:
	ldr	r3, [r5, #20]
	ldr	r2, [r5, #24]
	add	r3, r3, #1
	str	r3, [r5, #20]
	cmp	r3, r2
	beq	.L389
.L372:
	cmp	r10, #0
	sub	r10, r10, #1
	bne	.L376
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L388:
	ldr	r0, [r4]
	cmp	r0, #0
	bne	.L366
	cmp	r3, r2
	bne	.L367
.L368:
	cmp	r1, #16
	addeq	r4, r4, r8
	beq	.L372
	cmp	r1, #24
	addeq	r4, r4, r7
	b	.L372
.L361:
	ldr	r1, [r5]
	ldr	r3, [r5, #16]
	cmp	r1, #16
	ldr	r2, [r9, r3, asl #2]
	bne	.L390
.L373:
	mov	r0, r4
	mov	r1, r6
	add	r4, r4, r8
	bl	Volume16BitPro_Coef
	b	.L374
.L389:
	ldr	r3, [r5, #32]
	mov	r2, #0
	str	r2, [r5, #20]
	cmp	r3, #1
	bne	.L372
	ldr	r3, [r5, #16]
	sub	r3, r3, #1
	str	r3, [r5, #16]
	cmp	r3, #18
	moveq	r3, r2
	streq	r3, [r5, #32]
	b	.L372
.L392:
	.align	2
.L391:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FADE_InProcess, .-FADE_InProcess
	.align	2
	.global	FADE_OutProcess
	.type	FADE_OutProcess, %function
FADE_OutProcess:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	mov	r9, r1
	mov	r10, r2
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	cmp	r2, #0
	ldr	r5, [r4, #4]
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r6, .L410
	mov	r8, r5, asl #1
	mov	r7, r5, asl #2
	b	.L402
.L409:
	cmp	r3, #24
	mov	r0, r9
	mov	r1, r5
	bne	.L396
	bl	Volume24BitPro_Coef
	add	r9, r9, r7
.L396:
	ldr	r3, [r4, #20]
	sub	r10, r10, #1
	ldr	r2, [r4, #24]
	add	r3, r3, #1
	str	r3, [r4, #20]
	cmp	r3, r2
	beq	.L407
.L400:
	cmp	r10, #0
	beq	.L408
.L402:
	ldr	r3, [r4]
	ldr	r2, [r4, #16]
	cmp	r3, #16
	ldr	r2, [r6, r2, asl #2]
	bne	.L409
	mov	r0, r9
	mov	r1, r5
	add	r9, r9, r8
	bl	Volume16BitPro_Coef
	b	.L396
.L407:
	ldr	r3, [r4, #32]
	mov	r2, #0
	str	r2, [r4, #20]
	cmp	r3, #2
	bne	.L400
	ldr	r3, [r4, #16]
	add	r3, r3, #1
	str	r3, [r4, #16]
	cmp	r3, #99
	moveq	r3, r2
	streq	r3, [r4, #32]
	cmp	r10, #0
	bne	.L402
.L408:
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L411:
	.align	2
.L410:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	FADE_OutProcess, .-FADE_OutProcess
	.align	2
	.global	Util_sqrt
	.type	Util_sqrt, %function
Util_sqrt:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r5, r0, #0
	beq	.L421
	tst	r5, #1
	bne	.L422
	mov	r2, #1
.L416:
	mov	r3, r5, lsr r2
	tst	r3, #1
	bne	.L424
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L416
	mov	r2, #8
.L414:
	mov	r4, r3, asl r2
	mov	r0, r5
	mov	r1, r4
	bl	__aeabi_uidiv
	b	.L417
.L426:
	cmp	r2, #1
	add	r3, r4, r0
	mov	r0, r5
	movls	r2, #0
	movhi	r2, #1
	cmp	r2, #0
	beq	.L425
.L420:
	mov	r4, r3, lsr #1
	mov	r1, r4
	bl	__aeabi_uidiv
.L417:
	cmp	r0, r4
	rsb	r3, r0, r4
	rsb	r2, r4, r0
	bhi	.L426
	cmp	r3, #1
	add	r3, r4, r0
	mov	r0, r5
	movls	r2, #0
	movhi	r2, #1
	cmp	r2, #0
	bne	.L420
.L425:
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L421:
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L424:
	mov	r2, r2, lsr #1
	b	.L414
.L422:
	mov	r3, r5
	mov	r2, #0
	b	.L414
	UNWIND(.fnend)
	.size	Util_sqrt, .-Util_sqrt
	.align	2
	.global	UTIL_DRE_16BIT_Process
	.type	UTIL_DRE_16BIT_Process, %function
UTIL_DRE_16BIT_Process:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r2, [r0, #20]
	mov	r7, r0
	mov	r3, #1000
	ldr	r10, [r7, #8]
	mov	r4, r1
	mul	r3, r3, r2
	subs	r2, r10, #1
	str	r2, [fp, #-56]
	movw	r0, #19923
	mov	r9, r2
	ldr	r2, [r4, #16]
	movt	r0, 4194
	ldr	r1, [r7]
	mov	r6, #0
	ldr	r8, [r4]
	str	r2, [fp, #-52]
	umull	r2, r3, r3, r0
	mov	ip, r1
	str	r1, [fp, #-60]
	ldr	r5, [r4, #4]
	mov	r3, r3, lsr #6
	str	r3, [fp, #-48]
	bmi	.L446
	mov	r1, #32768
	add	lr, ip, #4
	mov	r0, r6
	movt	r1, 65535
	movw	ip, #32767
.L433:
	ldrsh	r2, [lr, #-4]
	add	lr, lr, #4
	ldrsh	r3, [lr, #-6]
	sub	r2, r2, r8, asr #16
	cmp	r2, r1
	sub	r3, r3, r5, asr #16
	movlt	r2, r1
	cmp	r2, ip
	movge	r2, ip
	cmp	r2, #0
	sublt	r8, r8, #512
	addge	r8, r8, #512
	cmp	r3, r1
	mla	r0, r2, r2, r0
	movlt	r3, r1
	cmp	r3, ip
	movge	r3, ip
	cmp	r3, #0
	sublt	r5, r5, #512
	addge	r5, r5, #512
	subs	r9, r9, #1
	mla	r6, r3, r3, r6
	bpl	.L433
.L428:
	str	r8, [r4]
	mov	r1, r10
	str	r5, [r4, #4]
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-52]
	cmp	r3, r0
	movls	r3, #0
	strls	r3, [r4, #8]
	bls	.L435
	ldr	r3, [r4, #8]
	ldr	r2, [fp, #-48]
	cmp	r2, r3
	addhi	r3, r3, r10
	strhi	r3, [r4, #8]
.L435:
	mov	r0, r6
	mov	r1, r10
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-52]
	cmp	r3, r0
	movls	r3, #0
	strls	r3, [r4, #12]
	bls	.L437
	ldr	r3, [r4, #12]
	ldr	r2, [fp, #-48]
	cmp	r2, r3
	addhi	r10, r3, r10
	strhi	r10, [r4, #12]
.L437:
	ldr	r2, [r7, #12]
	ldr	r3, [r4, #20]
	cmp	r2, r3
	bls	.L427
	ldr	r2, [r4, #8]
	ldr	r1, [fp, #-48]
	cmp	r1, r2
	bhi	.L439
	ldr	r2, [fp, #-56]
	cmp	r2, #0
	blt	.L427
	ldr	r1, [fp, #-60]
	movw	r10, #26125
	movt	r10, 25
	mov	r6, r2
	add	r5, r1, #4
	mov	r8, #1
	b	.L442
.L450:
	ldr	r3, [r4, #20]
.L442:
	ldr	r9, [r4, #36]
	movw	ip, #62303
	ldr	r1, [r4, #24]
	movt	ip, 15470
	ldr	lr, [r4, #28]
	subs	r6, r6, #1
	mul	r7, r10, r9
	ldr	r2, [r4, #32]
	rsb	r0, lr, r1
	add	r1, r1, r1, lsr #31
	add	r2, r0, r2
	ldrsh	r0, [r5, #-4]
	add	ip, r7, ip
	rsb	r7, r3, #32
	add	r0, r0, r2
	mov	r9, r9, asr r7
	sub	r2, r3, #1
	mov	r1, r1, asr #1
	add	r2, r0, r8, asl r2
	str	r1, [r4, #28]
	rsb	r7, r9, ip, asr r7
	mov	r1, r8, asl r3
	add	r2, r2, r7
	rsb	r1, r1, #0
	and	r2, r2, r1
	str	lr, [r4, #32]
	rsb	r0, r2, r0
	str	ip, [r4, #36]
	mov	r2, r2, asr r3
	str	r0, [r4, #24]
	add	r5, r5, #4
	strh	r2, [r5, #-8]
	bpl	.L450
	ldr	r3, [r4, #12]
	ldr	r2, [fp, #-48]
	cmp	r2, r3
	bhi	.L427
	ldr	r3, [fp, #-60]
	add	r7, r3, #2
	ldr	r3, [r4, #20]
	b	.L444
.L439:
	ldr	r2, [r4, #12]
	ldr	r1, [fp, #-48]
	cmp	r1, r2
	bhi	.L427
	ldr	r2, [fp, #-56]
	cmp	r2, #0
	ldr	r2, [fp, #-60]
	add	r7, r2, #2
	blt	.L427
.L444:
	movw	r10, #26125
	add	r7, r7, #4
	movt	r10, 25
	mov	r9, #1
	ldr	r6, [fp, #-56]
	b	.L443
.L451:
	ldr	r3, [r4, #20]
.L443:
	ldr	r1, [r4, #36]
	movw	lr, #62303
	ldr	r0, [r4, #24]
	rsb	r8, r3, #32
	ldr	ip, [r4, #28]
	movt	lr, 15470
	mul	r5, r10, r1
	ldr	r2, [r4, #32]
	rsb	ip, ip, r0
	mov	r1, r1, asr r8
	add	r2, ip, r2
	ldrsh	ip, [r7, #-4]
	add	lr, r5, lr
	sub	r5, r3, #1
	add	ip, ip, r2
	mov	r2, r9, asl r3
	add	r5, ip, r9, asl r5
	rsb	r1, r1, lr, asr r8
	add	r5, r5, r1
	rsb	r2, r2, #0
	ldr	r1, [r4, #28]
	and	r2, r2, r5
	add	r0, r0, r0, lsr #31
	subs	r6, r6, #1
	rsb	ip, r2, ip
	str	lr, [r4, #36]
	str	r1, [r4, #32]
	mov	r0, r0, asr #1
	mov	r2, r2, asr r3
	str	r0, [r4, #28]
	str	ip, [r4, #24]
	add	r7, r7, #4
	strh	r2, [r7, #-8]
	bpl	.L451
.L427:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L446:
	mov	r0, r6
	b	.L428
	UNWIND(.fnend)
	.size	UTIL_DRE_16BIT_Process, .-UTIL_DRE_16BIT_Process
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	g_u32VolCoef, %object
	.size	g_u32VolCoef, 400
g_u32VolCoef:
	.word	66633081
	.word	59386796
	.word	52928538
	.word	47172609
	.word	42042632
	.word	37470535
	.word	33395649
	.word	29763904
	.word	26527107
	.word	23642309
	.word	21071230
	.word	18779754
	.word	16737473
	.word	14917288
	.word	13295047
	.word	11849223
	.word	10560631
	.word	9412172
	.word	8388608
	.word	7476354
	.word	6663308
	.word	5938679
	.word	5292853
	.word	4717260
	.word	4204263
	.word	3747053
	.word	3339564
	.word	2976390
	.word	2652710
	.word	2364230
	.word	2107123
	.word	1877975
	.word	1673747
	.word	1491728
	.word	1329504
	.word	1184922
	.word	1056063
	.word	941217
	.word	838860
	.word	747635
	.word	666330
	.word	593867
	.word	529285
	.word	471726
	.word	420426
	.word	374705
	.word	333956
	.word	297639
	.word	265271
	.word	236423
	.word	210712
	.word	187797
	.word	167374
	.word	149172
	.word	132950
	.word	118492
	.word	105606
	.word	94121
	.word	83886
	.word	74763
	.word	66633
	.word	59386
	.word	52928
	.word	47172
	.word	42042
	.word	37470
	.word	33395
	.word	29763
	.word	26527
	.word	23642
	.word	21071
	.word	18779
	.word	16737
	.word	14917
	.word	13295
	.word	11849
	.word	10560
	.word	9412
	.word	8388
	.word	7476
	.word	6663
	.word	5938
	.word	5292
	.word	4717
	.word	4204
	.word	3747
	.word	3339
	.word	2976
	.word	2652
	.word	2364
	.word	2107
	.word	1877
	.word	1673
	.word	1491
	.word	1329
	.word	1184
	.word	1056
	.word	941
	.word	838
	.word	0
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
