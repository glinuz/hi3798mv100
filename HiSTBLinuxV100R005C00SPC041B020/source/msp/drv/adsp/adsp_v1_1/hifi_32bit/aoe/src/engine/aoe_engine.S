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
	.file	"aoe_engine.c"
	.text
	.align	2
	.type	ENGINECheckAipData, %function
ENGINECheckAipData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #56]
	ldr	r2, [r0, #60]
	ldr	r0, [r0, #64]
	cmp	r3, #0
	cmpne	r2, #0
	beq	.L2
	cmp	r3, #0
	moveq	lr, r3
	beq	.L3
	mov	r1, #0
	mov	r4, #1
	mov	lr, r1
.L5:
	mov	ip, r4, asl r1
	tst	ip, r3
	beq	.L6
	ands	ip, ip, r0
	mov	lr, #1
	beq	.L23
.L6:
	add	r1, r1, #1
	cmp	r1, #8
	bne	.L5
.L3:
	cmp	r2, #0
	movne	r3, #0
	movne	ip, #1
	beq	.L9
.L8:
	mov	r1, ip, asl r3
	tst	r1, r2
	beq	.L11
	tst	r1, r0
	beq	.L24
	orr	lr, lr, #2
.L11:
	add	r3, r3, #1
	cmp	r3, #8
	bne	.L8
.L9:
	and	r0, lr, #1
	ldmfd	sp, {r4, fp, sp, pc}
.L2:
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {r4, fp, sp, pc}
.L23:
	mov	lr, ip
	b	.L3
.L24:
	bic	lr, lr, #2
	b	.L9
	UNWIND(.fnend)
	.size	ENGINECheckAipData, .-ENGINECheckAipData
	.align	2
	.type	ENGINECheckAopSpace, %function
ENGINECheckAopSpace:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #88]
	ldr	lr, [r0, #72]
	cmp	r3, #0
	ldr	r0, [r0, #76]
	beq	.L40
	cmp	lr, #0
	beq	.L27
	mov	r2, #0
	mov	r4, #1
	mov	ip, r2
.L29:
	mov	r1, r4, asl r2
	tst	r1, lr
	beq	.L31
	ands	r1, r1, r3
	mov	ip, #1
	beq	.L46
.L31:
	add	r2, r2, #1
	cmp	r2, #8
	bne	.L29
.L30:
	cmp	r0, #0
	beq	.L39
.L37:
	mov	r2, #0
	mov	r4, #1
.L33:
	mov	r1, r4, asl r2
	tst	r1, r0
	beq	.L36
	tst	r1, r3
	beq	.L47
	orr	ip, ip, #2
.L36:
	add	r2, r2, #1
	cmp	r2, #8
	bne	.L33
.L34:
	cmp	lr, #0
	bne	.L39
	and	r0, ip, #2
	ldmfd	sp, {r4, fp, sp, pc}
.L27:
	cmp	r0, #0
	movne	ip, lr
	ldmeqfd	sp, {r4, fp, sp, pc}
	b	.L37
.L40:
	mov	r0, r3
	ldmfd	sp, {r4, fp, sp, pc}
.L39:
	and	r0, ip, #1
	ldmfd	sp, {r4, fp, sp, pc}
.L46:
	mov	ip, r1
	b	.L30
.L47:
	bic	ip, ip, #2
	b	.L34
	UNWIND(.fnend)
	.size	ENGINECheckAopSpace, .-ENGINECheckAopSpace
	.align	2
	.type	ENGINEDestroyChn, %function
ENGINEDestroyChn:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r5, [r4, #12]
	cmp	r5, #0
	bne	.L50
	ldr	r1, [r4, #164]
	cmp	r1, #0
	beq	.L50
	mov	r0, #26
	bl	HI_ADSP_FREE
	str	r5, [r4, #164]
.L50:
	mov	r1, r4
	mov	r0, #26
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	HI_ADSP_FREE
	UNWIND(.fnend)
	.size	ENGINEDestroyChn, .-ENGINEDestroyChn
	.global	__aeabi_uidiv
	.align	2
	.type	ENGINECreateChn, %function
ENGINECreateChn:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	r8, .L71
	ldr	r5, .L71+4
	mov	r2, #52
	mov	r1, #0
	mov	r6, r0
	ldr	r4, [r8, r0, asl #2]
	sub	r0, fp, #96
	ldr	r3, [r5, #20]
	blx	r3
	sub	r0, fp, #96
	ldr	r1, [r4]
	and	r1, r1, #3
	bl	AOE_REG_ReadBitDepth
	cmp	r0, #0
	bne	.L55
	ldr	r1, [r4]
	sub	r0, fp, #92
	ubfx	r1, r1, #2, #2
	bl	AOE_REG_ReadChannel
	cmp	r0, #0
	bne	.L55
	ldr	r1, [r4]
	sub	r0, fp, #88
	ubfx	r1, r1, #4, #4
	bl	AOE_REG_ReadSampelRate
	cmp	r0, #0
	bne	.L55
	ldr	r1, [r4]
	sub	r0, fp, #84
	ubfx	r1, r1, #8, #2
	bl	AOE_REG_ReadDataFormat
	subs	r7, r0, #0
	bne	.L55
	ldr	r3, [r8, r6, asl #2]
	mov	r1, #360
	str	r7, [fp, #-104]
	mov	r0, #26
	str	r7, [fp, #-100]
	ldr	r2, [r3, #80]
	str	r2, [fp, #-104]
	ldr	ip, [fp, #-104]
	ldr	r2, [r3, #84]
	str	ip, [fp, #-68]
	str	r2, [fp, #-100]
	ldr	r2, [r3, #88]
	bic	r2, r2, #-16777216
	str	r2, [fp, #-64]
	ldr	r2, [r3, #88]
	str	r7, [fp, #-104]
	str	r7, [fp, #-100]
	ubfx	r2, r2, #24, #4
	ldr	ip, [r3, #100]
	str	r2, [fp, #-60]
	str	ip, [fp, #-104]
	ldr	ip, [fp, #-104]
	ldr	r2, [r3, #104]
	str	ip, [fp, #-56]
	str	r2, [fp, #-100]
	ldr	r2, [r3, #108]
	bic	r2, r2, #-16777216
	str	r2, [fp, #-52]
	ldr	r3, [r3, #108]
	ubfx	r3, r3, #24, #4
	str	r3, [fp, #-48]
	bl	HI_ADSP_MALLOC
	subs	r4, r0, #0
	beq	.L55
	mov	r1, r7
	ldr	r3, [r5, #20]
	mov	r2, #360
	blx	r3
	ldr	r3, [r5, #24]
	mov	r2, #52
	sub	r1, fp, #96
	mov	r0, r4
	blx	r3
	ldr	r1, [r4]
	ldr	r0, [r4, #4]
	bl	UTIL_CalcFrameSize
	ldr	r3, [r4, #8]
	movw	r2, #19923
	movw	r1, #48000
	cmp	r3, r1
	movt	r2, 4194
	add	r3, r3, r3, lsl #2
	umull	r2, r3, r3, r2
	mov	r3, r3, lsr #6
	beq	.L69
	tst	r3, #3
	bicne	r3, r3, #3
	addne	r3, r3, #4
.L58:
	mul	r3, r0, r3
	ldr	r1, [r4]
	ldr	r0, [r4, #4]
	str	r3, [r4, #108]
	bl	UTIL_CalcFrameSize
	ldr	r9, [r4, #108]
	mov	r1, r0
	str	r0, [r4, #112]
	mov	r0, r9
	bl	__aeabi_uidiv
	ldr	r7, [r4, #12]
	cmp	r7, #0
	mov	r3, r0
	str	r0, [r4, #16]
	beq	.L70
.L59:
	mov	r0, r4
	mov	r3, #1
	str	r6, [r4, #104]
	str	r3, [r4, #116]
.L68:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L70:
	ldr	lr, [r4, #4]
	add	r8, r4, #164
	ldr	ip, [r4]
	add	r10, r4, #192
	str	r3, [r4, #172]
	mov	r1, r8
	ldr	r3, [r4, #8]
	mov	r2, #28
	str	lr, [r4, #180]
	mov	r0, r10
	str	ip, [r4, #176]
	str	r9, [r4, #168]
	str	r3, [r4, #184]
	str	r7, [r4, #164]
	ldr	r3, [r5, #24]
	str	r7, [r4, #188]
	blx	r3
	mov	r1, r8
	ldr	r3, [r5, #24]
	mov	r2, #28
	add	r0, r4, #220
	blx	r3
	mov	r1, r8
	ldr	r3, [r5, #24]
	mov	r2, #28
	add	r0, r4, #248
	blx	r3
	mov	r3, #8
	mov	r0, r3
	str	r3, [r4, #236]
	ldr	r1, [r4, #232]
	ldr	r9, [r4, #228]
	bl	UTIL_CalcFrameSize
	ldr	ip, [r5, #24]
	mov	r1, r8
	mov	r2, #28
	mul	r3, r9, r0
	add	r0, r4, #276
	str	r3, [r4, #224]
	blx	ip
	ldr	r3, [r5, #24]
	mov	r1, r10
	mov	r2, #28
	add	r0, r4, #304
	blx	r3
	mov	r1, r8
	ldr	r3, [r5, #24]
	mov	r2, #28
	add	r0, r4, #332
	blx	r3
	ldr	r1, [r4, #168]
	ldr	ip, [r4, #196]
	mov	r0, #26
	ldr	r5, [r4, #224]
	add	r1, ip, r1, lsl #1
	ldr	r3, [r4, #280]
	add	r1, r1, r5
	ldr	r5, [r4, #308]
	add	r3, r1, r3
	ldr	r1, [r4, #336]
	add	r3, r3, r5
	add	r5, r3, r1
	mov	r1, r5
	bl	HI_ADSP_MALLOC
	subs	r8, r0, #0
	beq	.L60
	ldr	r3, .L71+4
	mov	r2, r5
	mov	r1, r7
	ldr	r3, [r3, #20]
	blx	r3
	ldr	ip, [r4, #168]
	ldr	r0, [r4, #196]
	add	ip, r8, ip
	ldr	r1, [r4, #224]
	add	r0, ip, r0
	ldr	r2, [r4, #252]
	add	r1, r0, r1
	ldr	r3, [r4, #280]
	add	r2, r1, r2
	ldr	lr, [r4, #308]
	add	r3, r2, r3
	str	ip, [r4, #192]
	str	r8, [r4, #164]
	add	ip, r3, lr
	str	r0, [r4, #220]
	str	r1, [r4, #248]
	str	r2, [r4, #276]
	str	r3, [r4, #304]
	str	ip, [r4, #332]
	b	.L59
.L55:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L69:
	tst	r3, #255
	bicne	r3, r3, #255
	addne	r3, r3, #256
	b	.L58
.L60:
	mov	r0, r4
	bl	ENGINEDestroyChn
	mov	r0, r8
	b	.L68
.L72:
	.align	2
.L71:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ENGINECreateChn, .-ENGINECreateChn
	.align	2
	.type	ENGINEOutRawFrame, %function
ENGINEOutRawFrame:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r5, r1
	mov	r6, r2
	bl	AIP_GetRingIn
	mov	r4, r0
	mov	r0, r5
	bl	AOP_GetRingOut
	cmp	r0, #0
	cmpne	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r1, r0
	mov	r2, r6
	mov	r0, r4
	mov	r3, #3
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	RBUF_CopyData
	UNWIND(.fnend)
	.size	ENGINEOutRawFrame, .-ENGINEOutRawFrame
	.align	2
	.type	ENGINEMixData, %function
ENGINEMixData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #40)
	sub	sp, sp, #40
	mov	r5, r1
	mov	r4, r0
	sub	r1, fp, #72
	mov	r0, r5
	mov	ip, #0
	mov	r6, r2
	ldr	r7, [r4, #4]
	ldr	r8, [r4]
	str	ip, [fp, #-72]
	str	ip, [fp, #-68]
	str	ip, [fp, #-64]
	str	ip, [fp, #-60]
	str	ip, [fp, #-56]
	str	ip, [fp, #-52]
	str	ip, [fp, #-48]
	str	ip, [fp, #-44]
	str	ip, [fp, #-40]
	bl	AIP_QueryFiFoInfo
	ldr	r0, [r4, #108]
	ldr	r3, [fp, #-52]
	ldr	r9, [fp, #-56]
	cmp	r3, r0
	bls	.L75
	cmp	r9, r0
	ldr	r1, [r4, #112]
	bcc	.L79
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-68]
	ldr	r1, [fp, #-72]
	mov	r3, r8
	add	r1, r1, r2
	mul	r2, r7, r0
	mov	r0, r6
	bl	UTIL_MixerPro
.L78:
	ldr	r1, [r4, #108]
	mov	r0, r5
	bl	AIP_ReadDataComplete
.L75:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L79:
	mov	r0, r9
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-68]
	ldr	r1, [fp, #-72]
	mov	r3, r8
	add	r1, r1, r2
	mul	r2, r7, r0
	mov	r0, r6
	bl	UTIL_MixerPro
	ldr	r0, [r4, #108]
	ldr	r1, [r4, #112]
	rsb	r0, r9, r0
	bl	__aeabi_uidiv
	mov	r3, r8
	ldr	r1, [fp, #-72]
	mul	r2, r7, r0
	add	r0, r6, r9
	bl	UTIL_MixerPro
	b	.L78
	UNWIND(.fnend)
	.size	ENGINEMixData, .-ENGINEMixData
	.align	2
	.type	ENGINEOutFrame, %function
ENGINEOutFrame:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #76)
	sub	sp, sp, #76
	ldr	r2, [r0, #12]
	ldr	r6, [r0, #72]
	mov	r7, r0
	ldr	r3, [r0, #76]
	cmp	r2, #0
	str	r6, [fp, #-116]
	str	r3, [fp, #-112]
	bne	.L80
	sub	r8, fp, #116
	sub	r9, fp, #112
.L93:
	cmp	r6, #0
	beq	.L92
	mov	r10, #1
	mov	r4, #0
	mov	r5, r10, asl r4
	tst	r5, r6
	bne	.L108
.L86:
	add	r4, r4, #1
	cmp	r4, #8
	beq	.L92
.L91:
	mov	r5, r10, asl r4
	tst	r5, r6
	beq	.L86
.L108:
	mov	r0, r4
	bl	AOP_WriteBuffProcTryCnt
	mov	r0, r4
	sub	r1, fp, #80
	bl	AOP_GetChnStatus
	ldr	r2, [r7, #88]
	tst	r5, r2
	beq	.L86
	mov	r0, r4
	bl	AOP_WriteBuffProcOKCnt
	ldr	r2, [r7, #92]
	cmp	r2, #0
	bne	.L109
.L88:
	ldr	r2, [fp, #-56]
	sub	r5, fp, #108
	ldr	r3, .L111
	cmp	r2, #0
	beq	.L90
	ldr	r2, [r7, #152]
	cmp	r2, #0
	bne	.L110
.L90:
	ldr	ip, [r3, #24]
	mov	r2, #28
	add	r1, r7, #164
	mov	r0, r5
	blx	ip
.L89:
	mov	r1, r5
	mov	r0, r4
	bl	AOP_WriteRbfData
	mov	r0, r4
	sub	r1, fp, #80
	add	r4, r4, #1
	bl	AOP_ProcDelayCompensation
	cmp	r4, #8
	bne	.L91
.L92:
	cmp	r8, r9
	ldrne	r6, [r8, #4]!
	bne	.L93
.L80:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L109:
	ldr	r2, [fp, #-60]
	cmp	r2, #0
	bne	.L88
	ldr	r3, .L111
	sub	r5, fp, #108
	mov	r2, #28
	add	r1, r7, #332
	mov	r0, r5
	ldr	ip, [r3, #24]
	blx	ip
	b	.L89
.L110:
	ldr	ip, [r3, #24]
	mov	r2, #28
	add	r1, r7, #192
	mov	r0, r5
	blx	ip
	b	.L89
.L112:
	.align	2
.L111:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ENGINEOutFrame, .-ENGINEOutFrame
	.align	2
	.type	ENGINECheckRbfInData, %function
ENGINECheckRbfInData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, [r1, #12]
	mov	r4, r1
	mov	r6, r0
	cmp	r3, #0
	ldr	r5, [r1, #108]
	beq	.L114
	bl	AIP_QueryBusy
	cmp	r5, r0
	movls	r3, r5
	movhi	r3, #0
	mov	r0, r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L114:
	sub	r1, fp, #32
	bl	AIP_GetPcmType
	ldr	r3, [fp, #-32]
	cmp	r3, #3
	ldreq	r4, [r4, #224]
	beq	.L117
	cmp	r3, #4
	ldreq	r4, [r4, #196]
	ldrne	r4, [r4, #168]
.L117:
	mov	r0, r6
	bl	AIP_QueryFiFoBusy
	cmp	r0, r4
	movhi	r3, r4
	movls	r3, #0
	mov	r0, r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	ENGINECheckRbfInData, .-ENGINECheckRbfInData
	.align	2
	.global	UTIL_MixerMcPro
	.type	UTIL_MixerMcPro, %function
UTIL_MixerMcPro:
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
	beq	.L129
	cmp	r3, #24
	ldmnefd	sp, {fp, sp, pc}
	sub	r1, r1, #4
	mov	r3, #0
.L126:
	add	r3, r3, #1
	ldr	ip, [r1, #4]!
	cmp	r3, r2
	str	ip, [r0], #4
	bne	.L126
	ldmfd	sp, {fp, sp, pc}
.L129:
	sub	r1, r1, #2
	mov	r3, #0
.L124:
	add	r3, r3, #1
	ldrh	ip, [r1, #2]!
	cmp	r3, r2
	strh	ip, [r0], #2
	bne	.L124
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UTIL_MixerMcPro, .-UTIL_MixerMcPro
	.align	2
	.type	ENGINEMixFrame, %function
ENGINEMixFrame:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	r1, [r0, #12]
	mov	r3, #5
	mov	r5, r0
	cmp	r1, #0
	str	r3, [fp, #-80]
	beq	.L174
.L139:
	ldr	r3, [r5, #128]
	cmp	r3, #2
	ldreq	r3, [r5, #120]
	addeq	r3, r3, #1
	streq	r3, [r5, #120]
.L130:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L174:
	ldr	r6, .L176
	mov	r4, r1
	ldr	r2, [r0, #168]
	mov	r7, #1
	ldr	r0, [r0, #164]
	ldr	r3, [r6, #20]
	blx	r3
.L136:
	ldr	r3, [r5, #52]
	mov	r0, r4
	sub	r1, fp, #80
	ands	r3, r3, r7, asl r4
	beq	.L133
	ldr	r2, [r5, #12]
	cmp	r2, #0
	bne	.L134
	bl	AIP_GetPcmType
	ldr	r3, [fp, #-80]
	cmp	r3, #1
	bls	.L175
.L133:
	add	r4, r4, #1
	cmp	r4, #8
	bne	.L136
	ldr	r2, [r5, #12]
.L134:
	cmp	r2, #0
	mov	r3, #5
	str	r3, [fp, #-96]
	bne	.L139
	ldr	r3, [r5, #52]
	mov	r4, r2
	mov	r7, #1
	str	r2, [r5, #156]
	b	.L148
.L140:
	add	r4, r4, #1
	cmp	r4, #8
	beq	.L147
.L148:
	ands	r2, r3, r7, asl r4
	beq	.L140
	sub	r1, fp, #96
	mov	r0, r4
	bl	AIP_GetPcmType
	ldr	r3, [fp, #-96]
	cmp	r3, #3
	beq	.L141
	add	r4, r4, #1
	ldr	r3, [r5, #52]
	cmp	r4, #8
	bne	.L148
.L147:
	mov	r4, #0
	mov	r7, #1
	b	.L152
.L149:
	add	r4, r4, #1
	cmp	r4, #8
	beq	.L151
.L152:
	ands	r2, r3, r7, asl r4
	beq	.L149
	sub	r1, fp, #96
	mov	r0, r4
	bl	AIP_GetPcmType
	ldr	r3, [fp, #-96]
	cmp	r3, #2
	beq	.L150
	add	r4, r4, #1
	ldr	r3, [r5, #52]
	cmp	r4, #8
	bne	.L152
.L151:
	mov	r2, #0
	mov	r7, #1
	mov	r4, r2
	str	r2, [r5, #152]
	b	.L154
.L153:
	add	r4, r4, #1
	cmp	r4, #8
	beq	.L139
	ldr	r3, [r5, #52]
.L154:
	ands	r3, r3, r7, asl r4
	mov	r8, #1
	beq	.L153
	sub	r1, fp, #96
	mov	r0, r4
	bl	AIP_GetPcmType
	ldr	r3, [fp, #-96]
	cmp	r3, #4
	bne	.L153
	ldr	r3, [r6, #24]
	ldr	r2, [r5, #168]
	ldr	r1, [r5, #164]
	ldr	r0, [r5, #192]
	blx	r3
	mov	r1, r4
	ldr	r2, [r5, #192]
	mov	r0, r5
	bl	ENGINEMixData
	str	r8, [r5, #152]
	b	.L139
.L175:
	ldr	r2, [r5, #164]
	mov	r1, r4
	mov	r0, r5
	bl	ENGINEMixData
	b	.L133
.L150:
	mov	r1, r4
	ldr	r2, [r5, #164]
	mov	r0, r5
	bl	ENGINEMixData
	ldr	r3, [r5, #52]
	b	.L151
.L141:
	ldr	r2, [r5, #168]
	ldr	r3, [r6, #24]
	ldr	r1, [r5, #164]
	ldr	r0, [r5, #220]
	blx	r3
	mov	r3, #0
	str	r3, [fp, #-80]
	mov	r0, #8
	str	r3, [fp, #-76]
	str	r3, [fp, #-72]
	str	r3, [fp, #-68]
	str	r3, [fp, #-64]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-52]
	str	r3, [fp, #-48]
	ldr	r3, [r5, #236]
	ldr	r1, [r5, #232]
	ldr	r8, [r5, #220]
	str	r3, [fp, #-104]
	ldr	r7, [r5, #160]
	ldr	r9, [r5]
	bl	UTIL_CalcFrameSize
	sub	r1, fp, #80
	mov	r10, r0
	mov	r0, r4
	bl	AIP_QueryFiFoInfo
	ldr	r0, [r5, #224]
	ldr	r3, [fp, #-60]
	ldr	ip, [fp, #-64]
	cmp	r3, r0
	bcc	.L142
	cmp	r7, #0
	beq	.L143
	cmp	ip, r0
	mov	r1, r10
	bcs	.L144
	mov	r0, ip
	str	ip, [fp, #-108]
	bl	__aeabi_uidiv
	ldr	r7, [fp, #-104]
	ldr	r2, [fp, #-76]
	mov	r3, r9
	ldr	r1, [fp, #-80]
	add	r1, r1, r2
	mul	r2, r7, r0
	mov	r0, r8
	bl	UTIL_MixerMcPro
	ldr	ip, [fp, #-108]
	ldr	r0, [r5, #224]
	mov	r1, r10
	rsb	r0, ip, r0
	str	ip, [fp, #-104]
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-104]
	mov	r3, r9
	ldr	r1, [fp, #-80]
	mul	r2, r7, r0
	add	r0, r8, ip
	bl	UTIL_MixerMcPro
	ldr	r0, [r5, #224]
.L143:
	mov	r1, r0
	mov	r0, r4
	bl	AIP_ReadDataComplete
.L145:
	mov	r2, #1
	ldr	r3, [r5, #52]
	str	r2, [r5, #156]
	b	.L147
.L142:
	sub	r1, fp, #92
	mov	r0, r4
	bl	AIP_GetChnStatus
	ldr	r3, [fp, #-92]
	cmp	r3, #1
	bne	.L145
	mov	r0, r4
	bl	AIP_UnderFlowCalc
	b	.L145
.L144:
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-76]
	ldr	r1, [fp, #-80]
	mov	r3, r9
	add	r1, r1, r2
	ldr	r2, [fp, #-104]
	mul	r2, r2, r0
	mov	r0, r8
	bl	UTIL_MixerMcPro
	ldr	r0, [r5, #224]
	b	.L143
.L177:
	.align	2
.L176:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ENGINEMixFrame, .-ENGINEMixFrame
	.align	2
	.global	ENGINE_Process
	.type	ENGINE_Process, %function
ENGINE_Process:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #100)
	sub	sp, sp, #100
	ldr	r3, .L364
	mov	r10, #0
	add	r9, r3, #24
	mov	r7, r3
	b	.L252
.L195:
	add	r10, r10, #1
	add	r9, r9, #4
	cmp	r10, #6
	beq	.L346
.L252:
	ldr	r3, [r7], #4
	ldr	r2, [r3, #4]
	ubfx	r2, r2, #2, #1
	cmp	r2, #0
	bne	.L341
	ldr	r1, [r3, #4]
	ands	r1, r1, #3
	beq	.L181
	cmp	r1, #1
	beq	.L182
	ldr	r2, [r3, #4]
	mov	r1, #4
	bfi	r2, r1, #3, #4
	str	r2, [r3, #4]
	ldr	r2, [r3, #4]
	orr	r2, r2, r1
	str	r2, [r3, #4]
.L341:
	ldr	r4, [r9]
	cmp	r4, #0
	beq	.L195
.L194:
	ldr	r6, [r4, #116]
.L253:
	cmp	r6, #1
	bne	.L195
	ldr	r2, [r4]
	mov	r8, #0
	ldr	r3, [r7, #-4]
	mov	r5, r8
	str	r2, [fp, #-132]
	ldr	r2, [r4, #4]
	str	r3, [fp, #-120]
	str	r2, [fp, #-140]
	ldr	r2, [r3, #12]
	ldr	r3, [r4, #16]
	str	r4, [fp, #-124]
	uxth	r2, r2
	str	r8, [fp, #-104]
	str	r3, [fp, #-136]
	str	r8, [fp, #-116]
	mov	r4, r2
	str	r8, [fp, #-108]
	str	r8, [fp, #-112]
	str	r7, [fp, #-128]
	b	.L202
.L197:
	add	r5, r5, #1
	cmp	r5, #8
	beq	.L347
.L202:
	mov	r7, r6, asl r5
	tst	r7, r4
	beq	.L197
	sub	r1, fp, #80
	mov	r0, r5
	bl	AOP_GetChnStatus
	ldr	r1, [fp, #-80]
	cmp	r1, #1
	bne	.L197
	ldr	r1, [fp, #-76]
	cmp	r1, #1
	ldr	r1, [fp, #-72]
	ldreq	r3, [fp, #-112]
	ldrne	r3, [fp, #-108]
	orreq	r3, r3, r7
	orrne	r3, r3, r7
	streq	r3, [fp, #-112]
	strne	r3, [fp, #-108]
	ldr	r3, [fp, #-132]
	cmp	r3, r1
	beq	.L348
.L200:
	ldr	r3, [fp, #-104]
	orr	r3, r3, r7
	str	r3, [fp, #-104]
.L201:
	ldr	r1, [fp, #-64]
	mov	r0, r5
	ldr	r3, [fp, #-136]
	add	r5, r5, #1
	mul	r1, r1, r3
	bl	AOP_QueryRbfFree
	cmp	r0, #0
	orrne	r8, r8, r7
	cmp	r5, #8
	bne	.L202
.L347:
	mov	r2, r4
	ldr	r3, [fp, #-112]
	ldr	r4, [fp, #-124]
	mov	r5, #0
	ldr	r7, [fp, #-128]
	mov	r6, #1
	str	r3, [r4, #72]
	ldr	r3, [fp, #-108]
	str	r2, [r4, #68]
	str	r8, [r4, #88]
	str	r3, [r4, #76]
	ldr	r3, [fp, #-116]
	str	r5, [r4, #56]
	str	r5, [r4, #60]
	str	r3, [r4, #80]
	ldr	r3, [fp, #-104]
	str	r5, [r4, #64]
	str	r3, [r4, #84]
	ldr	r3, [fp, #-120]
	ldr	r3, [r3, #8]
	uxth	r3, r3
	str	r3, [r4, #52]
	b	.L213
.L203:
	add	r5, r5, #1
	cmp	r5, #8
	beq	.L212
.L350:
	ldr	r3, [r4, #52]
.L213:
	mov	r8, r6, asl r5
	tst	r3, r8
	beq	.L203
	sub	r1, fp, #80
	mov	r0, r5
	bl	AIP_GetChnStatus
	ldr	r3, [fp, #-80]
	cmp	r3, #1
	beq	.L349
	ldr	r3, [r4, #12]
	cmp	r3, #0
	bne	.L203
	mov	r1, r4
	mov	r0, r5
	bl	ENGINECheckRbfInData
	cmp	r0, #0
	beq	.L203
	ldr	r3, [r4, #64]
	add	r5, r5, #1
	orr	r3, r3, r8
	str	r3, [r4, #64]
	ldr	r3, [fp, #-76]
	cmp	r3, #1
	ldreq	r3, [r4, #56]
	ldrne	r3, [r4, #60]
	orreq	r8, r3, r8
	streq	r8, [r4, #56]
	orrne	r8, r3, r8
	strne	r8, [r4, #60]
	cmp	r5, #8
	bne	.L350
.L212:
	ldr	r6, [r9]
	ldr	r5, [r6, #12]
	cmp	r5, #0
	bne	.L351
	ldr	r2, [r6, #128]
	sub	r1, r2, #1
	cmp	r1, #1
	bls	.L236
	mov	r3, r5
	str	r5, [fp, #-112]
	str	r5, [fp, #-104]
	mov	r4, r5
	mov	ip, #1
	mov	r5, r6
	mov	r6, r3
	b	.L239
.L237:
	add	r4, r4, #1
	cmp	r4, #8
	beq	.L352
.L239:
	ldr	r1, [r5, #52]
	mov	r8, ip, asl r4
	tst	r8, r1
	beq	.L237
	sub	r1, fp, #80
	mov	r0, r4
	str	ip, [fp, #-108]
	bl	AIP_GetChnStatus
	ldr	r1, [fp, #-80]
	ldr	ip, [fp, #-108]
	cmp	r1, #0
	bne	.L238
	ldr	r1, [fp, #-72]
	add	r4, r4, #1
	ldr	r3, [fp, #-104]
	cmp	r1, #1
	orreq	r6, r6, r8
	cmp	r4, #8
	orr	r3, r3, r8
	str	r3, [fp, #-104]
	bne	.L239
.L352:
	mov	r3, r6
	mov	r6, r5
	mov	r5, r3
	ldr	r3, [r6, #124]
	mov	r4, #0
	mov	r8, #1
	str	r3, [fp, #-108]
	ldr	r3, [fp, #-104]
	str	r3, [r6, #124]
	b	.L241
.L240:
	add	r4, r4, #1
	cmp	r4, #8
	beq	.L353
.L241:
	ands	r3, r5, r8, asl r4
	beq	.L240
	mov	r0, r4
	add	r4, r4, #1
	bl	AIP_FlushCutoffFlag
	cmp	r4, #8
	bne	.L241
.L353:
	ldr	r3, [fp, #-112]
	cmp	r3, #0
	ldrne	r2, [r6, #128]
	bne	.L236
	ldr	r3, [fp, #-104]
	ldr	r2, [fp, #-108]
	rsb	r3, r2, r3
	cmp	r3, r5
	bls	.L354
	ldr	r3, [fp, #-112]
	mov	r2, #2
	str	r2, [r6, #128]
	str	r3, [r6, #120]
.L256:
	ldr	r2, [r6, #120]
	cmp	r2, #2
	bne	.L245
.L255:
	mov	r4, #0
	mov	r5, #1
.L248:
	ldr	r2, [r6, #72]
	mov	r3, r5, asl r4
	mov	r0, r4
	add	r4, r4, #1
	tst	r3, r2
	bne	.L246
	ldr	r2, [r6, #76]
	tst	r3, r2
	beq	.L247
.L246:
	bl	AOP_Flush
.L247:
	cmp	r4, #8
	bne	.L248
	ldr	r2, [r6, #92]
	mov	r3, #0
	str	r3, [r6, #128]
	cmp	r2, #0
	str	r3, [r6, #120]
	bne	.L355
.L249:
	mov	r0, r6
	bl	ENGINECheckAopSpace
	cmp	r0, #0
	beq	.L195
	mov	r0, r6
	bl	ENGINECheckAipData
	cmp	r0, #0
	beq	.L195
	mov	r0, r6
	bl	ENGINEMixFrame
	mov	r0, r6
	bl	ENGINEOutFrame
	b	.L195
.L182:
	ldr	r4, [r9]
	cmp	r4, #0
	beq	.L195
	ldr	r6, [r4, #116]
	cmp	r6, #1
	beq	.L356
	ldr	r2, [r3, #4]
	tst	r2, #4
	bne	.L253
.L254:
	ldr	r2, [r3, #4]
	bfc	r2, #3, #4
	str	r2, [r3, #4]
	ldr	r2, [r3, #4]
	orr	r2, r2, #4
	str	r2, [r3, #4]
	b	.L194
.L181:
	ldr	r0, [r9]
	cmp	r0, #0
	beq	.L357
	ldr	r2, [r0, #116]
	cmp	r2, #0
	beq	.L358
.L188:
	mov	r2, #1
	str	r2, [r0, #116]
.L187:
	ldr	r2, [r3, #4]
	ubfx	r2, r2, #2, #1
	cmp	r2, #0
	ldreq	r1, [r3, #4]
	bfieq	r1, r2, #3, #4
	streq	r1, [r3, #4]
	ldreq	r2, [r3, #4]
	orreq	r2, r2, #4
	streq	r2, [r3, #4]
	b	.L341
.L346:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L349:
	ldr	r3, [fp, #-76]
	mov	r1, r4
	mov	r0, r5
	cmp	r3, #1
	ldreq	r3, [r4, #56]
	ldrne	r3, [r4, #60]
	orreq	r3, r3, r8
	orrne	r3, r3, r8
	streq	r3, [r4, #56]
	strne	r3, [r4, #60]
	bl	ENGINECheckRbfInData
	cmp	r0, #0
	ldrne	r3, [r4, #64]
	orrne	r8, r3, r8
	strne	r8, [r4, #64]
	b	.L203
.L238:
	ldr	r3, [fp, #-112]
	orr	r3, r3, r8
	str	r3, [fp, #-112]
	b	.L237
.L236:
	cmp	r2, #1
	beq	.L255
	cmp	r2, #2
	beq	.L256
.L245:
	ldr	r2, [r6, #92]
	cmp	r2, #0
	beq	.L249
.L355:
	tst	r2, #15
	bne	.L249
	ldr	r2, [r6, #96]
	cmp	r2, #0
	beq	.L251
	mov	r0, r6
	bl	ENGINECheckAipData
	cmp	r0, #0
	bne	.L359
.L251:
	ldr	r2, [r6, #100]
	cmp	r2, #0
	beq	.L195
	mov	r0, r6
	bl	ENGINECheckAopSpace
	cmp	r0, #0
	beq	.L195
	mov	r0, r6
	bl	ENGINEOutFrame
	b	.L195
.L351:
	ldr	r2, [r6, #68]
	mov	r3, #0
	str	r3, [fp, #-100]
	tst	r2, #1
	str	r3, [fp, #-96]
	bne	.L215
	mov	r5, #1
	mov	r1, r5
.L219:
	ands	r3, r2, r1, asl r5
	bne	.L360
	add	r5, r5, #1
	cmp	r5, #8
	bne	.L219
	ldr	r2, [r6, #52]
	tst	r2, #1
	bne	.L195
	mov	r0, #1
.L218:
	mov	r4, #1
	mov	r1, r4
	b	.L222
.L221:
	add	r4, r4, #1
	cmp	r4, #8
	beq	.L195
.L222:
	ands	r3, r2, r1, asl r4
	beq	.L221
	sub	r2, r4, #8
	clz	r2, r2
	mov	r2, r2, lsr #5
.L217:
	orrs	r3, r2, r0
	bne	.L195
.L258:
	sub	r1, fp, #92
	mov	r0, r4
	bl	AIP_GetChnStatus
	ldr	r8, [fp, #-92]
	ldr	r2, [r6, #12]
	cmp	r8, #0
	bne	.L223
	cmp	r2, #2
	str	r8, [r6, #20]
	str	r8, [r6, #24]
	beq	.L195
.L224:
	mov	r0, r6
	bl	ENGINECheckAopSpace
	cmp	r0, #0
	beq	.L195
	mov	r0, r6
	bl	ENGINECheckAipData
	cmp	r0, #0
	beq	.L195
	ldr	r2, [r6, #108]
	mov	r0, r4
	mov	r1, r5
	bl	ENGINEOutRawFrame
	sub	r1, fp, #80
	mov	r0, r5
	bl	AOP_GetChnStatus
	sub	r1, fp, #80
	mov	r0, r5
	bl	AOP_ProcDelayCompensation
	b	.L195
.L358:
	bl	ENGINEDestroyChn
	mov	r0, r10
	bl	ENGINECreateChn
	cmp	r0, #0
	str	r0, [r9]
	ldr	r3, [r7, #-4]
	bne	.L188
	ldr	r2, [r3, #4]
	tst	r2, #4
	bne	.L195
.L342:
	ldr	r2, [r3, #4]
	mov	r1, #3
	bfi	r2, r1, #3, #4
	str	r2, [r3, #4]
	ldr	r2, [r3, #4]
	orr	r2, r2, #4
	str	r2, [r3, #4]
	b	.L195
.L356:
	str	r2, [r4, #116]
	ldr	r2, [r3, #4]
	tst	r2, #4
	beq	.L254
	b	.L194
.L348:
	ldr	r1, [fp, #-68]
	ldr	r3, [fp, #-140]
	cmp	r3, r1
	bne	.L200
	ldr	r3, [fp, #-116]
	orr	r3, r3, r7
	str	r3, [fp, #-116]
	b	.L201
.L223:
	cmp	r2, #2
	bne	.L224
	mov	r0, r6
	bl	ENGINECheckAopSpace
	subs	r3, r8, #1
	movne	r3, #1
	cmp	r0, #0
	movne	r0, r3
	orreq	r0, r3, #1
	cmp	r0, #0
	bne	.L195
	ldr	r3, [r6, #20]
	ldr	r2, [r6, #24]
	orrs	r1, r2, r3
	str	r3, [fp, #-100]
	str	r2, [fp, #-96]
	beq	.L361
.L225:
	cmp	r3, #0
	beq	.L228
	ldr	r8, [r6, #108]
	cmp	r8, r3
	bhi	.L229
	mov	r0, r6
	bl	ENGINECheckAipData
	cmp	r0, #0
	beq	.L195
.L230:
	mov	r1, r5
	mov	r0, r4
	mov	r2, r8
	bl	ENGINEOutRawFrame
	ldr	r3, [fp, #-100]
	rsb	r1, r8, r3
	str	r1, [fp, #-100]
	cmp	r1, #0
	bne	.L231
	ldr	r0, [r6, #108]
	cmp	r8, r0
	bcs	.L231
	ldr	r2, [fp, #-96]
	rsb	r4, r8, r0
	cmp	r4, r2
	bls	.L362
.L231:
	mov	r0, r5
	sub	r1, fp, #80
	bl	AOP_GetChnStatus
	sub	r1, fp, #80
	mov	r0, r5
	bl	AOP_ProcDelayCompensation
	ldr	r2, [fp, #-100]
	ldr	r3, [fp, #-96]
.L233:
	str	r2, [r6, #20]
	str	r3, [r6, #24]
	b	.L195
.L354:
	mov	r2, #1
	str	r2, [r6, #128]
	b	.L255
.L357:
	mov	r0, r10
	bl	ENGINECreateChn
	cmp	r0, #0
	str	r0, [r9]
	beq	.L363
	mov	r3, #1
	str	r3, [r0, #116]
	ldr	r3, [r7, #-4]
	b	.L187
.L360:
	ldr	r2, [r6, #52]
	sub	r0, r5, #8
	clz	r0, r0
	tst	r2, #1
	mov	r0, r0, lsr #5
	beq	.L218
	mov	r2, #0
	mov	r4, r2
	b	.L217
.L359:
	mov	r0, r6
	bl	ENGINEMixFrame
	b	.L251
.L215:
	ldr	r2, [r6, #52]
	ands	r0, r2, #1
	bne	.L264
	mov	r5, r0
	b	.L218
.L363:
	ldr	r3, [r7, #-4]
	ldr	r2, [r3, #4]
	tst	r2, #4
	bne	.L195
	b	.L342
.L228:
	ldr	r2, [fp, #-96]
	cmp	r2, #0
	beq	.L233
	ldr	r1, [r6, #108]
	mov	r0, r5
	str	r3, [fp, #-104]
	cmp	r2, r1
	movcc	r4, r2
	movcs	r4, r1
	bl	AOP_GetRingOut
	cmp	r0, #0
	beq	.L234
	ldr	r3, [fp, #-104]
	mov	r2, r4
	mov	r1, r3
	bl	RBUF_WriteDataDma
.L234:
	ldr	r1, [fp, #-96]
	ldr	r2, [fp, #-100]
	rsb	r3, r4, r1
	b	.L233
.L264:
	mov	r4, #0
	mov	r5, r4
	b	.L258
.L229:
	mov	r0, r4
	str	r3, [fp, #-104]
	bl	AIP_QueryBusy
	ldr	r3, [fp, #-104]
	cmp	r0, r3
	bcc	.L195
	mov	r8, r3
	b	.L230
.L361:
	mov	r0, r4
	bl	AIP_QueryBusy
	cmp	r0, #3
	bls	.L195
	sub	r2, fp, #96
	sub	r1, fp, #100
	mov	r0, r4
	bl	AIP_GetCompressInfo
	ldr	r3, [fp, #-100]
	ldr	r2, [fp, #-96]
	cmp	r3, #0
	bne	.L227
	cmp	r2, #0
	bne	.L227
	mov	r0, r4
	bl	AIP_GetRingIn
	bl	RBUF_Flush
	ldr	r3, [fp, #-100]
	ldr	r2, [fp, #-96]
.L227:
	str	r3, [r6, #20]
	str	r2, [r6, #24]
	b	.L225
.L362:
	mov	r0, r5
	str	r1, [fp, #-104]
	bl	AOP_GetRingOut
	cmp	r0, #0
	beq	.L232
	ldr	r1, [fp, #-104]
	mov	r2, r4
	bl	RBUF_WriteDataDma
.L232:
	ldr	r2, [fp, #-96]
	rsb	r4, r4, r2
	str	r4, [fp, #-96]
	b	.L231
.L365:
	.align	2
.L364:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	ENGINE_Process, .-ENGINE_Process
	.align	2
	.global	ENGINE_Init
	.type	ENGINE_Init, %function
ENGINE_Init:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L369
	mov	r4, r0
	ldr	r8, .L369+4
	mov	r7, #0
	add	r6, r5, #24
.L367:
	str	r4, [r5], #4
	mov	r2, #128
	ldr	r3, [r8, #20]
	mov	r0, r4
	mov	r1, #0
	add	r4, r4, #128
	blx	r3
	ldr	r3, [r4, #-124]
	cmp	r5, r6
	mov	r2, #1
	orr	r3, r3, #4
	str	r3, [r4, #-124]
	ldr	r3, [r4, #-124]
	bfi	r3, r2, #0, #2
	str	r3, [r4, #-124]
	str	r7, [r5, #20]
	bne	.L367
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L370:
	.align	2
.L369:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ENGINE_Init, .-ENGINE_Init
	.align	2
	.global	ENGINE_DeInit
	.type	ENGINE_DeInit, %function
ENGINE_DeInit:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L374
	mov	r6, #0
	add	r5, r4, #24
.L372:
	ldr	r0, [r4, #4]!
	bl	ENGINEDestroyChn
	cmp	r4, r5
	str	r6, [r4]
	bne	.L372
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L375:
	.align	2
.L374:
	.word	.LANCHOR0+20
	UNWIND(.fnend)
	.size	ENGINE_DeInit, .-ENGINE_DeInit
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stEneRm, %object
	.size	g_stEneRm, 48
g_stEneRm:
	.space	48
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
