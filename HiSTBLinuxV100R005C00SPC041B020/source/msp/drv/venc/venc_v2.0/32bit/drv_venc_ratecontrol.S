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
	.file	"drv_venc_ratecontrol.c"
	.text
	.align	2
	.global	VEDU_DRV_RCCalcMinQPForVBR
	.type	VEDU_DRV_RCCalcMinQPForVBR, %function
VEDU_DRV_RCCalcMinQPForVBR:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #20]
	cmp	r3, #0
	bne	.L9
	ldr	r3, [r0, #84]
	movw	r1, #34079
	movt	r1, 20971
	mov	ip, #200
	smull	r4, r5, r3, r1
	mov	r2, r3, asr #31
	rsb	r2, r2, r5, asr #6
	mls	r3, ip, r2, r3
	cmp	r3, #0
	ldrne	r3, [r0, #572]
	beq	.L10
	cmp	r3, #19
	movle	r3, #20
	bgt	.L11
.L6:
	str	r3, [r0, #572]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L11:
	cmp	r3, #30
	movge	r3, #30
	str	r3, [r0, #572]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L10:
	ldr	r4, [r0, #560]
	ldr	r2, [r0, #572]
	add	r4, r4, #200
	str	r3, [r0, #560]
	add	r3, r2, #1
	umull	r4, r5, r4, r1
	cmp	r3, r5, lsr #6
	subhi	r3, r2, #1
	cmp	r3, #19
	movle	r3, #20
	ble	.L6
	b	.L11
.L9:
	ldr	r3, [r0, #176]
	add	r3, r3, #5
	str	r3, [r0, #572]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcMinQPForVBR, .-VEDU_DRV_RCCalcMinQPForVBR
	.align	2
	.global	VEDU_DRV_RCGetVBRMaxBitRate
	.type	VEDU_DRV_RCGetVBRMaxBitRate, %function
VEDU_DRV_RCGetVBRMaxBitRate:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #12]
	cmp	r3, #1280
	movhi	r3, #8388608
	strhi	r3, [r0, #576]
	ldmhifd	sp, {fp, sp, pc}
	cmp	r3, #720
	movhi	r3, #5242880
	strhi	r3, [r0, #576]
	ldmhifd	sp, {fp, sp, pc}
	cmp	r3, #352
	movhi	r3, #3072
	movls	r3, #1536
	str	r3, [r0, #576]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCGetVBRMaxBitRate, .-VEDU_DRV_RCGetVBRMaxBitRate
	.align	2
	.global	VEDU_DRV_RCGetRecodingQP
	.type	VEDU_DRV_RCGetRecodingQP, %function
VEDU_DRV_RCGetRecodingQP:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #264]
	add	r2, r3, #128
	str	r2, [r0, #100]
	str	r3, [r0, #164]
	mov	r3, r3, lsr #6
	str	r3, [r0, #172]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCGetRecodingQP, .-VEDU_DRV_RCGetRecodingQP
	.global	__aeabi_uidiv
	.align	2
	.global	VEDU_DRV_RCInitInfo
	.type	VEDU_DRV_RCInitInfo, %function
VEDU_DRV_RCInitInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, [r0, #36]
	mov	r3, #5
	ldr	r1, [r0, #28]
	mov	r4, r0
	str	r3, [r0, #200]
	mov	r2, #1
	mov	r3, r5, asl #3
	mov	r6, #0
	add	r3, r3, r5, lsl #1
	str	r1, [r4, #544]
	str	r2, [r4, #204]
	mov	r0, r5
	mov	r3, r3, lsr #3
	str	r2, [r4, #220]
	str	r3, [r4, #212]
	str	r3, [r4, #216]
	str	r6, [r4, #208]
	bl	__aeabi_uidiv
	ldr	r3, [r4, #40]
	ldr	r1, [r4, #76]
	str	r5, [r4, #552]
	cmp	r1, r6
	str	r3, [r4, #548]
	strne	r6, [r4, #84]
	mul	r2, r3, r0
	str	r0, [r4, #92]
	str	r2, [r4, #88]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCInitInfo, .-VEDU_DRV_RCInitInfo
	.align	2
	.global	VEDU_DRV_RCInitInstantBits
	.type	VEDU_DRV_RCInitInstantBits, %function
VEDU_DRV_RCInitInstantBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, .L27
	add	r4, r0, #268
	ldr	r1, [r0, #28]
	mov	r5, r0
	mov	r3, #0
	mov	r2, #260
	str	r3, [r5, #536]
	mov	r0, r4
	str	r1, [r5, #528]
	mov	r1, r3
	str	r3, [r5, #532]
	str	r3, [r5, #540]
	ldr	r3, [ip]
	blx	r3
	ldr	r1, [r5, #40]
	ldr	r6, [r5, #200]
	sub	r1, r1, #1
	ldr	r0, [r5, #88]
	add	r1, r1, r6
	bl	__aeabi_uidiv
	ldr	r1, [r5, #528]
	ldr	ip, [r5, #532]
	cmp	r1, #1
	mul	r6, r6, r0
	add	ip, r6, ip
	str	ip, [r5, #532]
	str	r6, [r5, #268]
	bls	.L24
	mov	r2, r4
	mov	r3, #1
.L25:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L25
	sub	r3, r3, #2
	mla	r3, r3, r0, r0
	add	ip, ip, r3
	str	ip, [r5, #532]
.L24:
	str	ip, [r5, #540]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L28:
	.align	2
.L27:
	.word	venc_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VEDU_DRV_RCInitInstantBits, .-VEDU_DRV_RCInitInstantBits
	.align	2
	.global	VEDU_DRV_RCInitFrameBits
	.type	VEDU_DRV_RCInitFrameBits, %function
VEDU_DRV_RCInitFrameBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #168]
	mov	r4, r0
	mvn	r2, #0
	str	r2, [r0, #252]
	str	r2, [r0, #256]
	str	r3, [r0, #224]
	mov	r0, r3
	str	r3, [r4, #228]
	ldr	r1, [r4, #200]
	bl	__aeabi_uidiv
	str	r0, [r4, #232]
	str	r0, [r4, #236]
	str	r0, [r4, #240]
	str	r0, [r4, #244]
	str	r0, [r4, #248]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCInitFrameBits, .-VEDU_DRV_RCInitFrameBits
	.align	2
	.global	VEDU_DRV_EflRcInitQp
	.type	VEDU_DRV_EflRcInitQp, %function
VEDU_DRV_EflRcInitQp:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r2, r2, lsl #1
	mov	r3, #100
	mul	r0, r3, r0
	mul	r1, r1, r2
	mov	r1, r1, lsr #1
	bl	__aeabi_uidiv
	cmp	r0, #170
	bhi	.L32
	sub	r3, r0, #121
	cmp	r3, #49
	bls	.L33
	sub	r3, r0, #81
	cmp	r3, #39
	bls	.L34
	sub	r3, r0, #41
	cmp	r3, #39
	bls	.L35
	sub	r3, r0, #16
	cmp	r3, #24
	bls	.L36
	sub	r3, r0, #6
	cmp	r3, #9
	bls	.L37
	sub	r0, r0, #3
	cmp	r0, #3
	movcc	r0, #40
	movcs	r0, #43
	ldmfd	sp, {fp, sp, pc}
.L33:
	mov	r0, #15
	ldmfd	sp, {fp, sp, pc}
.L32:
	mov	r0, #7
	ldmfd	sp, {fp, sp, pc}
.L35:
	mov	r0, #25
	ldmfd	sp, {fp, sp, pc}
.L34:
	mov	r0, #20
	ldmfd	sp, {fp, sp, pc}
.L36:
	mov	r0, #30
	ldmfd	sp, {fp, sp, pc}
.L37:
	mov	r0, #38
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_EflRcInitQp, .-VEDU_DRV_EflRcInitQp
	.align	2
	.global	VEDU_DRV_RCModifyQPDeltaOfIP
	.type	VEDU_DRV_RCModifyQPDeltaOfIP, %function
VEDU_DRV_RCModifyQPDeltaOfIP:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #88]
	mov	r5, r0
	ldr	r1, [r0, #40]
	ldr	r6, [r0, #24]
	mov	r0, r3, asl #4
	add	r1, r1, #19
	add	r0, r0, r3, lsl #2
	bl	__aeabi_uidiv
	ldr	r4, [r5, #208]
	cmp	r6, r0
	bhi	.L41
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L41:
	rsb	r6, r0, r6
	mov	r1, r0
	mov	r0, r6, asl #8
	bl	__aeabi_uidiv
	ldr	r2, [r5, #96]
	mvn	r3, #255
	ldr	r1, .L46
	bic	r2, r2, r2, asr #31
	cmp	r0, r3
	movlt	r0, r3
	cmp	r2, #3264
	movge	r2, #3264
	cmp	r0, #256
	mov	r2, r2, lsr #6
	movge	r0, #256
	add	r3, r4, r0, asr #6
	ldr	r2, [r1, r2, asl #2]
	bic	r3, r3, r3, asr #31
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r5, #208]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L47:
	.align	2
.L46:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VEDU_DRV_RCModifyQPDeltaOfIP, .-VEDU_DRV_RCModifyQPDeltaOfIP
	.align	2
	.global	VEDU_DRV_RCCalcQPDeltaByFrameBits
	.type	VEDU_DRV_RCCalcQPDeltaByFrameBits, %function
VEDU_DRV_RCCalcQPDeltaByFrameBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #24]
	ldr	r1, [r0, #104]
	cmp	r3, r1
	bcc	.L49
	rsb	r0, r1, r3
	mov	r0, r0, asl #6
	bl	__aeabi_uidiv
.L50:
	mvn	r3, #127
	cmp	r0, r3
	movlt	r0, r3
	cmp	r0, #256
	movge	r0, #256
	ldmfd	sp, {fp, sp, pc}
.L49:
	rsb	r3, r3, r1
	mov	r0, r3, asl #5
	add	r0, r0, r3, lsl #3
	bl	__aeabi_uidiv
	rsb	r0, r0, #0
	b	.L50
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcQPDeltaByFrameBits, .-VEDU_DRV_RCCalcQPDeltaByFrameBits
	.align	2
	.global	VEDU_DRV_RCCalcQPDeltaByInstantBits
	.type	VEDU_DRV_RCCalcQPDeltaByInstantBits, %function
VEDU_DRV_RCCalcQPDeltaByInstantBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r7, [r0, #36]
	mov	r5, r0
	ldr	r8, [r0, #540]
	ldr	r6, [r0, #532]
	mov	r1, r7
	mov	r0, r8, asl #5
	bl	__aeabi_uidiv
	mov	r1, r7
	mov	r4, r0
	mov	r0, r6, asl #5
	bl	__aeabi_uidiv
	usat	r4, #6, r4
	ldr	r2, .L57
	cmp	r6, r8
	mov	r1, r7, lsr #4
	add	r4, r2, r4, lsl #2
	ldr	ip, [r4, #208]
	usat	r0, #6, r0
	add	r2, r2, r0, lsl #2
	ldr	r3, [r2, #208]
	rsb	r0, ip, r3
	bls	.L52
	rsb	r3, r1, r7
	cmp	r8, r3
	bls	.L52
	mov	r3, r0, asl #3
	mvn	r2, #127
	sub	r3, r3, r0, asl #1
	str	r6, [r5, #540]
	cmp	r3, r2
	movge	r0, r3
	movlt	r0, r2
	cmp	r0, #256
	movge	r0, #256
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L52:
	cmp	r6, r8
	bcc	.L56
.L55:
	mov	r0, #0
	str	r6, [r5, #540]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L56:
	add	r7, r1, r7
	cmp	r8, r7
	bcs	.L55
	mov	r0, r0, asl #2
	mvn	r3, #127
	cmp	r0, r3
	str	r6, [r5, #540]
	movlt	r0, r3
	cmp	r0, #256
	movge	r0, #256
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L58:
	.align	2
.L57:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcQPDeltaByInstantBits, .-VEDU_DRV_RCCalcQPDeltaByInstantBits
	.align	2
	.global	VEDU_DRV_RCCalcQPDeltaByBitRate
	.type	VEDU_DRV_RCCalcQPDeltaByBitRate, %function
VEDU_DRV_RCCalcQPDeltaByBitRate:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, r1
	bls	.L60
	rsb	r0, r1, r0
	mov	r0, r0, asl #5
	bl	__aeabi_uidiv
.L61:
	mvn	r3, #63
	cmp	r0, r3
	movlt	r0, r3
	cmp	r0, #64
	movge	r0, #64
	ldmfd	sp, {fp, sp, pc}
.L60:
	rsb	r0, r0, r1
	mov	r0, r0, asl #4
	bl	__aeabi_uidiv
	rsb	r0, r0, #0
	b	.L61
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcQPDeltaByBitRate, .-VEDU_DRV_RCCalcQPDeltaByBitRate
	.global	__aeabi_idiv
	.align	2
	.global	VEDU_DRV_RCCalcQPDeltaByWaterline
	.type	VEDU_DRV_RCCalcQPDeltaByWaterline, %function
VEDU_DRV_RCCalcQPDeltaByWaterline:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, [r0, #212]
	ldr	r0, [r0, #216]
	rsb	r0, r1, r0
	mov	r0, r0, asl #2
	bl	__aeabi_idiv
	mvn	r3, #31
	cmp	r0, r3
	movlt	r0, r3
	cmp	r0, #32
	movge	r0, #32
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcQPDeltaByWaterline, .-VEDU_DRV_RCCalcQPDeltaByWaterline
	.align	2
	.global	VEDU_DRV_RCCalcVBRQPDeltaByInstantBits
	.type	VEDU_DRV_RCCalcVBRQPDeltaByInstantBits, %function
VEDU_DRV_RCCalcVBRQPDeltaByInstantBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #532]
	mov	r2, #100
	ldr	ip, [r0, #576]
	mov	r1, #95
	mul	r3, r2, r3
	mul	r2, r1, ip
	cmp	r3, r2
	bls	.L68
	mov	r0, #192
	ldmfd	sp, {fp, sp, pc}
.L68:
	mov	r2, #90
	mul	r2, r2, ip
	cmp	r3, r2
	bls	.L69
	mov	r0, #128
	ldmfd	sp, {fp, sp, pc}
.L69:
	mov	r0, #85
	mul	r0, r0, ip
	cmp	r3, r0
	movhi	r0, #64
	movls	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcVBRQPDeltaByInstantBits, .-VEDU_DRV_RCCalcVBRQPDeltaByInstantBits
	.align	2
	.global	VEDU_DRV_RCCalcVBRQPDeltaByPicBits
	.type	VEDU_DRV_RCCalcVBRQPDeltaByPicBits, %function
VEDU_DRV_RCCalcVBRQPDeltaByPicBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [r0, #84]
	mov	r3, r0
	cmp	ip, #200
	beq	.L71
	ldr	r2, [r0, #568]
	ldr	lr, [r0, #564]
	rsb	r1, lr, r2
	cmp	r1, #0
	ble	.L72
	mov	r0, r1, asl #3
	add	r0, r0, r1, lsl #1
	cmp	r2, r0
	movlt	r0, #64
	movge	r0, #0
.L73:
	mov	r2, #0
	cmp	ip, #1000
	str	lr, [r3, #568]
	str	r2, [r3, #564]
	streq	r2, [r3, #84]
	ldmfd	sp, {fp, sp, pc}
.L71:
	ldr	r1, [r0, #564]
	mov	r2, #0
	mov	r0, r2
	str	r2, [r3, #564]
	str	r1, [r3, #568]
	ldmfd	sp, {fp, sp, pc}
.L72:
	rsb	r1, r1, #0
	mov	r0, r1, asl #3
	add	r1, r0, r1, lsl #1
	cmp	r2, r1
	movge	r0, #0
	mvnlt	r0, #63
	b	.L73
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcVBRQPDeltaByPicBits, .-VEDU_DRV_RCCalcVBRQPDeltaByPicBits
	.align	2
	.global	VEDU_DRV_RCGetQPDeltaForVBR
	.type	VEDU_DRV_RCGetQPDeltaForVBR, %function
VEDU_DRV_RCGetQPDeltaForVBR:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #532]
	mov	r2, #100
	ldr	ip, [r0, #576]
	mov	r1, #95
	mul	r3, r2, r3
	mul	r2, r1, ip
	cmp	r3, r2
	movhi	r4, #192
	bhi	.L79
	mov	r2, #90
	mul	r2, r2, ip
	cmp	r3, r2
	movhi	r4, #128
	bls	.L85
.L79:
	ldr	r1, [r0, #84]
	movw	r2, #34079
	movt	r2, 20971
	mov	ip, #200
	smull	r2, r3, r1, r2
	mov	r2, r1, asr #31
	rsb	r2, r2, r3, asr #6
	mls	r3, ip, r2, r1
	cmp	r3, #0
	movne	r0, #0
	bne	.L80
	bl	VEDU_DRV_RCCalcVBRQPDeltaByPicBits
.L80:
	add	r0, r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L85:
	mov	r4, #85
	mul	r4, r4, ip
	cmp	r3, r4
	movhi	r4, #64
	movls	r4, #0
	b	.L79
	UNWIND(.fnend)
	.size	VEDU_DRV_RCGetQPDeltaForVBR, .-VEDU_DRV_RCGetQPDeltaForVBR
	.align	2
	.global	VEDU_DRV_RCCalculateCurQP
	.type	VEDU_DRV_RCCalculateCurQP, %function
VEDU_DRV_RCCalculateCurQP:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #220]
	mov	r4, r0
	cmp	r3, #0
	bne	.L105
.L87:
	ldr	r0, [r4, #24]
	ldr	r1, [r4, #104]
	cmp	r0, r1
	bcc	.L88
	rsb	r0, r1, r0
	mov	r0, r0, asl #6
	bl	__aeabi_uidiv
	mov	r3, r0
.L89:
	mvn	r2, #127
	cmp	r3, r2
	mov	r0, r4
	movlt	r6, r2
	movge	r6, r3
	bl	VEDU_DRV_RCCalcQPDeltaByInstantBits
	ldr	r2, [r4, #532]
	ldr	r1, [r4, #36]
	cmp	r6, #256
	addle	r6, r0, r6
	addgt	r6, r0, #256
	cmp	r2, r1
	bls	.L90
	rsb	r0, r1, r2
	mov	r0, r0, asl #5
	bl	__aeabi_uidiv
.L91:
	ldr	r1, [r4, #212]
	mvn	r2, #63
	ldr	r3, [r4, #216]
	cmp	r0, r2
	rsb	r3, r1, r3
	movge	r5, r0
	movlt	r5, r2
	mov	r0, r3, asl #2
	bl	__aeabi_idiv
	mvn	r3, #31
	ldr	r1, [r4, #44]
	ldr	r2, [r4, #164]
	cmp	r0, r3
	movlt	r0, r3
	cmp	r5, #64
	addle	r3, r6, r5
	addgt	r3, r6, #64
	cmp	r0, #32
	addle	r0, r3, r0
	addgt	r0, r3, #32
	cmp	r1, #0
	add	r0, r0, r2
	str	r0, [r4, #164]
	beq	.L92
	ldr	r2, [r4, #208]
	ldr	r3, [r4, #96]
	add	r3, r3, r2, lsl #6
	add	r0, r3, r0
	mov	r0, r0, lsr #1
	str	r0, [r4, #164]
.L92:
	ldr	r3, [r4, #220]
	cmp	r3, #0
	beq	.L93
	ldr	r3, [r4, #100]
	ldr	r2, [r4, #164]
	cmp	r2, r3
	addcc	r3, r3, r2
	movcc	r3, r3, lsr #1
	strcc	r3, [r4, #164]
.L93:
	ldr	r3, [r4, #556]
	cmp	r3, #0
	bne	.L94
	ldr	r0, [r4, #164]
.L95:
	ldmib	r4, {r2, r3}
	mov	r3, r3, asl #6
	mov	r2, r2, asl #6
	cmp	r0, r2
	movlt	r0, r2
	cmp	r0, r3
	movge	r0, r3
	str	r0, [r4, #164]
	mov	r0, r0, lsr #6
	str	r0, [r4, #172]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L90:
	rsb	r0, r2, r1
	mov	r0, r0, asl #4
	bl	__aeabi_uidiv
	rsb	r0, r0, #0
	b	.L91
.L88:
	rsb	r0, r0, r1
	mov	r3, r0, asl #5
	add	r0, r3, r0, lsl #3
	bl	__aeabi_uidiv
	rsb	r3, r0, #0
	b	.L89
.L105:
	bl	VEDU_DRV_RCModifyQPDeltaOfIP
	b	.L87
.L94:
	mov	r0, r4
	bl	VEDU_DRV_RCGetQPDeltaForVBR
	ldr	r3, [r4, #164]
	add	r0, r0, r3
	b	.L95
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalculateCurQP, .-VEDU_DRV_RCCalculateCurQP
	.align	2
	.global	VEDU_DRV_RCUpdateFrameBits
	.type	VEDU_DRV_RCUpdateFrameBits, %function
VEDU_DRV_RCUpdateFrameBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #220]
	cmp	r3, #0
	beq	.L107
	ldr	r3, [r0, #252]
	ldr	r2, [r0, #24]
	add	r3, r3, #1
	cmp	r3, #1
	movhi	r3, #0
	str	r3, [r0, #252]
	add	r3, r3, #56
	str	r2, [r0, r3, asl #2]
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L107:
	ldr	r2, [r0, #256]
	add	r2, r2, #1
	cmp	r2, #4
	strls	r2, [r0, #256]
	strhi	r3, [r0, #256]
	movls	r3, r2
	ldr	r2, [r0, #24]
	add	r3, r3, #58
	str	r2, [r0, r3, asl #2]
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCUpdateFrameBits, .-VEDU_DRV_RCUpdateFrameBits
	.align	2
	.global	VEDU_DRV_RCCalcTimeOfP
	.type	VEDU_DRV_RCCalcTimeOfP, %function
VEDU_DRV_RCCalcTimeOfP:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #228]
	mov	r4, r0
	add	r3, r0, #228
	add	ip, r0, #248
	ldr	r0, [r0, #224]
	mov	r1, #0
	add	r0, r2, r0
.L115:
	ldr	r2, [r3, #4]!
	cmp	r3, ip
	add	r1, r1, r2
	bne	.L115
	cmp	r0, #0
	cmpne	r1, #0
	beq	.L119
	mov	r1, r1, asl #1
	add	r0, r0, r0, lsl #2
	bl	__aeabi_idiv
	cmp	r0, #0
	strne	r0, [r4, #200]
	mov	r0, #0
	moveq	r3, #1
	streq	r3, [r4, #200]
	mov	r3, #1
	str	r3, [r4, #204]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L119:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCCalcTimeOfP, .-VEDU_DRV_RCCalcTimeOfP
	.align	2
	.global	VEDU_DRV_RCGetTimeOfP
	.type	VEDU_DRV_RCGetTimeOfP, %function
VEDU_DRV_RCGetTimeOfP:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	bl	VEDU_DRV_RCUpdateFrameBits
	mov	r0, r4
	bl	VEDU_DRV_RCCalcTimeOfP
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCGetTimeOfP, .-VEDU_DRV_RCGetTimeOfP
	.align	2
	.global	VEDU_DRV_RCGetTargetFrameBits
	.type	VEDU_DRV_RCGetTargetFrameBits, %function
VEDU_DRV_RCGetTargetFrameBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, [r0, #36]
	mov	r4, r0
	ldr	r3, [r0, #552]
	cmp	r3, r5
	ldreq	r2, [r0, #40]
	beq	.L124
	ldr	r1, [r0, #28]
	mov	r0, r5
	bl	__aeabi_uidiv
	ldr	r2, [r4, #40]
	str	r5, [r4, #552]
	str	r0, [r4, #92]
	mul	r0, r2, r0
	str	r0, [r4, #88]
.L124:
	ldr	r5, [r4, #44]
	cmp	r5, #0
	ldreq	r3, [r4, #204]
	beq	.L126
	ldr	r3, [r4, #180]
	ldr	r0, [r4, #200]
	ldr	r1, [r4, #184]
	cmp	r3, r0
	movlt	r3, r0
	cmp	r3, r1
	movge	r3, r1
.L126:
	cmp	r2, #1000
	bcs	.L127
	ldr	r1, [r4, #548]
	cmp	r1, r2
	ldr	r1, [r4, #200]
	strne	r2, [r4, #548]
	ldrne	r0, [r4, #92]
	sub	r1, r1, #1
	ldreq	r0, [r4, #88]
	add	r1, r1, r2
	mulne	r0, r0, r2
	strne	r0, [r4, #88]
	mul	r0, r3, r0
	bl	__aeabi_uidiv
.L130:
	adds	r5, r5, #0
	str	r0, [r4, #168]
	movne	r5, #1
	str	r5, [r4, #220]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L127:
	ldr	r0, [r4, #92]
	mul	r0, r0, r3
	b	.L130
	UNWIND(.fnend)
	.size	VEDU_DRV_RCGetTargetFrameBits, .-VEDU_DRV_RCGetTargetFrameBits
	.align	2
	.global	VEDU_DRV_RCofFirstFrame
	.type	VEDU_DRV_RCofFirstFrame, %function
VEDU_DRV_RCofFirstFrame:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	bl	VEDU_DRV_RCInitInfo
	mov	r0, r4
	bl	VEDU_DRV_RCInitInstantBits
	mov	r0, r4
	bl	VEDU_DRV_RCGetTargetFrameBits
	ldr	r5, [r4, #168]
	mvn	r3, #0
	ldr	r1, [r4, #200]
	str	r3, [r4, #252]
	str	r3, [r4, #256]
	mov	r0, r5
	str	r5, [r4, #224]
	str	r5, [r4, #228]
	bl	__aeabi_uidiv
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #12]
	mov	r3, r0
	mov	r0, r5
	str	r3, [r4, #232]
	str	r3, [r4, #236]
	str	r3, [r4, #240]
	str	r3, [r4, #244]
	str	r3, [r4, #248]
	bl	VEDU_DRV_EflRcInitQp
	ldr	r2, [r4, #208]
	mov	r3, r0, asl #6
	rsb	r0, r2, r0
	add	r1, r3, #128
	str	r3, [r4, #164]
	str	r1, [r4, #100]
	str	r0, [r4, #172]
	str	r0, [r4, #176]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCofFirstFrame, .-VEDU_DRV_RCofFirstFrame
	.align	2
	.global	ModifyInfoForBitRatesChange
	.type	ModifyInfoForBitRatesChange, %function
ModifyInfoForBitRatesChange:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, [r0, #36]
	mov	r4, r0
	ldr	r1, [r0, #28]
	mov	r0, r5
	bl	__aeabi_uidiv
	ldr	r3, [r4, #40]
	str	r5, [r4, #552]
	str	r0, [r4, #92]
	mul	r0, r3, r0
	str	r0, [r4, #88]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	ModifyInfoForBitRatesChange, .-ModifyInfoForBitRatesChange
	.align	2
	.global	VEDU_DRV_EflRCGetCurTimeOfP
	.type	VEDU_DRV_EflRCGetCurTimeOfP, %function
VEDU_DRV_EflRCGetCurTimeOfP:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #44]
	cmp	r3, #0
	beq	.L134
	ldr	r1, [r0, #200]
	ldr	r3, [r0, #180]
	ldr	r2, [r0, #184]
	cmp	r3, r1
	movge	r0, r3
	movlt	r0, r1
	cmp	r0, r2
	movge	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L134:
	ldr	r0, [r0, #204]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_EflRCGetCurTimeOfP, .-VEDU_DRV_EflRCGetCurTimeOfP
	.align	2
	.global	VEDU_DRV_EflRCCalcTargetFrameBits
	.type	VEDU_DRV_EflRCCalcTargetFrameBits, %function
VEDU_DRV_EflRCCalcTargetFrameBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #548]
	ldr	r3, [r0, #40]
	ldr	ip, [r0, #200]
	cmp	r2, r3
	strne	r3, [r0, #548]
	ldrne	r2, [r0, #92]
	ldreq	r2, [r0, #88]
	mulne	r2, r2, r3
	sub	r3, r3, #1
	strne	r2, [r0, #88]
	mul	r0, r1, r2
	add	r1, r3, ip
	bl	__aeabi_uidiv
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_EflRCCalcTargetFrameBits, .-VEDU_DRV_EflRCCalcTargetFrameBits
	.align	2
	.global	ModifyInfoForGopChange
	.type	ModifyInfoForGopChange, %function
ModifyInfoForGopChange:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #40]
	ldr	r2, [r0, #92]
	str	r3, [r0, #548]
	mul	r3, r3, r2
	str	r3, [r0, #88]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	ModifyInfoForGopChange, .-ModifyInfoForGopChange
	.align	2
	.global	VEDU_DRV_RCModifyQPForSpecialScene
	.type	VEDU_DRV_RCModifyQPForSpecialScene, %function
VEDU_DRV_RCModifyQPForSpecialScene:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #76]
	cmp	r3, #0
	beq	.L141
	ldr	r3, [r0, #172]
	add	r3, r3, #1
	cmp	r3, #22
	movle	r3, #23
	bgt	.L151
.L142:
	str	r3, [r0, #172]
	mov	r3, #0
	str	r3, [r0, #76]
.L141:
	ldr	r3, [r0, #72]
	cmp	r3, #0
	ldrne	r3, [r0, #172]
	addne	r3, r3, #1
	strne	r3, [r0, #172]
	ldmfd	sp, {fp, sp, pc}
.L151:
	cmp	r3, #35
	movge	r3, #35
	b	.L142
	UNWIND(.fnend)
	.size	VEDU_DRV_RCModifyQPForSpecialScene, .-VEDU_DRV_RCModifyQPForSpecialScene
	.align	2
	.global	VENC_DRV_RcOpenOneFrm
	.type	VENC_DRV_RcOpenOneFrm, %function
VENC_DRV_RcOpenOneFrm:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #260]
	mov	r3, #0
	mov	r4, r0
	str	r3, [r0, #556]
	cmp	r2, r3
	bne	.L160
	ldr	r3, [r0, #20]
	cmp	r3, #0
	bne	.L161
	bl	VEDU_DRV_RCCalculateCurQP
	mov	r0, r4
	bl	VEDU_DRV_RCGetTimeOfP
	cmn	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	mov	r0, r4
	bl	VEDU_DRV_RCGetTargetFrameBits
.L156:
	mov	r0, r4
	bl	VEDU_DRV_RCModifyQPForSpecialScene
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L161:
	bl	VEDU_DRV_RCofFirstFrame
	b	.L156
.L160:
	ldr	r2, [r0, #264]
	mov	r0, r3
	add	r3, r2, #128
	str	r3, [r4, #100]
	str	r2, [r4, #164]
	mov	r2, r2, lsr #6
	str	r2, [r4, #172]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_DRV_RcOpenOneFrm, .-VENC_DRV_RcOpenOneFrm
	.align	2
	.global	VEDU_DRV_RCUpdateWaterline
	.type	VEDU_DRV_RCUpdateWaterline, %function
VEDU_DRV_RCUpdateWaterline:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #260]
	cmp	r3, #0
	ldrne	r1, [r0, #536]
	ldrne	r2, [r0, #216]
	ldrne	r3, [r0, #168]
	addne	r1, r0, r1, lsl #2
	ldreq	r2, [r0, #216]
	addne	r2, r3, r2
	ldreq	r3, [r0, #168]
	ldrne	r1, [r1, #268]
	rsbne	r2, r1, r2
	ldr	r1, [r0, #24]
	rsb	r3, r3, r2
	add	r3, r3, r1
	str	r3, [r0, #216]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCUpdateWaterline, .-VEDU_DRV_RCUpdateWaterline
	.align	2
	.global	ModifyInfoForFrmRateChange
	.type	ModifyInfoForFrmRateChange, %function
ModifyInfoForFrmRateChange:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, [r0, #528]
	ldr	ip, [r0, #544]
	cmp	r1, ip
	bls	.L166
	add	r2, ip, #66
	ldr	lr, [r0, #92]
	mov	r3, ip
	add	r2, r0, r2, lsl #2
.L167:
	add	r3, r3, #1
	str	lr, [r2, #4]!
	cmp	r3, r1
	bne	.L167
	ldr	r2, [r0, #532]
	rsb	r3, ip, r1
	mla	ip, lr, r3, r2
	str	ip, [r0, #532]
.L168:
	str	r1, [r0, #544]
	ldmfd	sp, {r4, fp, sp, pc}
.L166:
	bcs	.L168
	ldr	r3, [r0, #536]
	cmp	r1, r3
	bls	.L177
	rsb	ip, r1, ip
	add	r2, r3, #1
	add	ip, ip, r3
	cmp	r2, ip
	bhi	.L168
	add	r3, r3, #67
	ldr	lr, [r0, #532]
	add	r3, r0, r3, lsl #2
.L173:
	add	r2, r2, #1
	ldr	r4, [r3, #4]!
	cmp	r2, ip
	rsb	lr, r4, lr
	bls	.L173
	str	lr, [r0, #532]
	str	r1, [r0, #544]
	ldmfd	sp, {r4, fp, sp, pc}
.L177:
	ldr	ip, [r0, #532]
	rsb	r3, r1, r3
	add	lr, r0, #264
	mov	r2, #0
.L171:
	add	r2, r2, #1
	ldr	r4, [lr, #4]!
	cmp	r2, r3
	rsb	ip, r4, ip
	bls	.L171
	str	ip, [r0, #532]
	str	r1, [r0, #544]
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	ModifyInfoForFrmRateChange, .-ModifyInfoForFrmRateChange
	.align	2
	.global	VEDU_DRV_RCUpdateInstantBits
	.type	VEDU_DRV_RCUpdateInstantBits, %function
VEDU_DRV_RCUpdateInstantBits:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	ldr	r5, [r0, #528]
	bl	ModifyInfoForFrmRateChange
	ldr	r3, [r4, #536]
	ldr	r2, [r4, #532]
	add	r1, r3, #1
	ldr	r0, [r4, #24]
	add	r3, r4, r3, lsl #2
	cmp	r5, r1
	add	r2, r0, r2
	ldr	ip, [r3, #268]
	movls	r1, #0
	rsb	r2, ip, r2
	str	r2, [r4, #532]
	str	r0, [r3, #268]
	str	r1, [r4, #536]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCUpdateInstantBits, .-VEDU_DRV_RCUpdateInstantBits
	.align	2
	.global	VEDU_DRV_RCUpdateInfo
	.type	VEDU_DRV_RCUpdateInfo, %function
VEDU_DRV_RCUpdateInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #260]
	mov	r3, r0
	cmp	r2, #0
	beq	.L182
	ldr	r2, [r0, #536]
	cmp	r2, #0
	ldreq	r2, [r0, #528]
	sub	r2, r2, #1
	str	r2, [r0, #536]
	ldr	r1, [r3, #216]
	add	r0, r0, r2, lsl #2
	ldr	r2, [r3, #168]
	ldr	r0, [r0, #268]
	add	r1, r2, r1
	rsb	r1, r0, r1
.L185:
	ldr	ip, [r3, #24]
	rsb	r2, r2, r1
	mov	r0, r3
	add	r2, r2, ip
	str	r2, [r3, #216]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VEDU_DRV_RCUpdateInstantBits
.L182:
	ldr	r2, [r0, #168]
	ldr	r1, [r0, #216]
	b	.L185
	UNWIND(.fnend)
	.size	VEDU_DRV_RCUpdateInfo, .-VEDU_DRV_RCUpdateInfo
	.align	2
	.global	VEDU_DRV_RCIsNeedRecoding
	.type	VEDU_DRV_RCIsNeedRecoding, %function
VEDU_DRV_RCIsNeedRecoding:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, [r0, #36]
	movw	r6, #52429
	movt	r6, 52428
	mov	r3, r0
	ldr	r0, [r0, #68]
	umull	r6, r7, r1, r6
	cmp	r0, #0
	mov	r6, r7, lsr #4
	mov	r2, r6, asl #1
	rsb	r4, r2, r1
	add	r1, r2, r1
	bne	.L189
	ldr	r2, [r3, #260]
	cmp	r2, #0
	bne	.L189
	ldr	r2, [r3, #44]
	cmp	r2, #0
	bne	.L195
	ldr	ip, [r3, #168]
	ldr	lr, [r3, #24]
	add	ip, ip, ip, lsl #1
	cmp	lr, ip
	movls	ip, #0
	movhi	ip, #1
.L191:
	ldr	lr, [r3, #532]
	ldr	r5, [r3, #540]
	cmp	r4, lr
	movls	r4, #0
	movhi	r4, #1
	cmp	r1, lr
	movcs	r1, r4
	orrcc	r1, r4, #1
	cmp	lr, r5
	ldr	r4, [r3, #20]
	rsbhi	lr, r5, lr
	rsbls	lr, lr, r5
	and	r1, r1, ip
	cmp	lr, r6
	movls	r2, #0
	andhi	r2, r1, #1
	cmp	r4, #0
	str	r2, [r3, #260]
	eoreq	r2, r2, #1
	rsbeq	r0, r2, #0
	movne	r2, #1
	strne	r2, [r3, #260]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L195:
	ldr	r2, [r3, #168]
	ldr	ip, [r3, #24]
	add	r2, r2, r2, lsl #1
	cmp	ip, r2, lsr #1
	movhi	ip, #1
	movls	ip, #0
	b	.L191
.L189:
	mov	r2, #0
	mvn	r0, #0
	str	r2, [r3, #260]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCIsNeedRecoding, .-VEDU_DRV_RCIsNeedRecoding
	.align	2
	.global	VEDU_DRV_RCStartQPChange
	.type	VEDU_DRV_RCStartQPChange, %function
VEDU_DRV_RCStartQPChange:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, [r0, #24]
	mov	r5, r0
	ldr	r3, [r0, #168]
	ldr	r7, [r0, #36]
	cmp	r1, r3
	ldr	r6, [r0, #532]
	ldr	r10, [r0, #540]
	bcc	.L197
	rsb	r0, r3, r1
	mov	r1, r3
	mov	r0, r0, asl #5
	bl	__aeabi_uidiv
	mov	r9, r0
.L198:
	mov	r1, r7
	mov	r0, r10, asl #5
	bl	__aeabi_uidiv
	mov	r1, r7
	mov	r8, r0
	mov	r0, r6, asl #5
	bl	__aeabi_uidiv
	usat	r8, #6, r8
	ldr	r2, .L203
	cmp	r10, r7
	cmphi	r6, r10
	add	r8, r2, r8, lsl #2
	ldr	r4, [r8, #208]
	usat	r0, #6, r0
	add	r2, r2, r0, lsl #2
	ldr	r3, [r2, #208]
	rsb	r3, r4, r3
	bhi	.L199
	cmp	r10, r7
	cmpcc	r6, r10
	bcs	.L200
.L199:
	mov	r4, r3, asl #3
	mvn	r2, #191
	sub	r4, r4, r3, asl #1
	cmp	r4, r2
	movlt	r4, r2
	cmp	r4, #256
	movge	r4, #256
.L201:
	rsb	r6, r7, r6
	mov	r1, r7
	mov	r0, r6, asl #7
	sub	r0, r0, r6, asl #5
	bl	__aeabi_uidiv
	ldr	r1, [r5, #212]
	mov	r6, r0
	ldr	r0, [r5, #216]
	rsb	r0, r1, r0
	mov	r0, r0, asl #5
	bl	__aeabi_idiv
	mvn	r2, #127
	cmp	r9, r2
	mvn	r3, #31
	ldr	r1, [r5, #4]
	movlt	r9, r2
	cmp	r6, r3
	ldr	ip, [r5, #164]
	movlt	r6, r3
	ldr	r2, [r5, #8]
	mov	r1, r1, asl #6
	mov	r2, r2, asl #6
	cmp	r0, r3
	movlt	r0, r3
	cmp	r9, #256
	addle	r3, r4, r9
	addgt	r3, r4, #256
	cmp	r6, #32
	addle	r4, r3, r6
	addgt	r4, r3, #32
	cmp	r0, #32
	addle	r4, r4, r0
	addgt	r4, r4, #32
	add	r4, r4, ip
	cmp	r4, r1
	movlt	r4, r1
	cmp	r4, r2
	movlt	r0, r4
	movge	r0, r2
	cmp	ip, r0
	str	r0, [r5, #264]
	movne	r0, #0
	mvneq	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L197:
	rsb	r0, r1, r3
	mov	r0, r0, asl #5
	bl	__aeabi_uidiv
	rsb	r9, r0, #0
	b	.L198
.L200:
	ldr	r4, [r5, #44]
	cmp	r4, #0
	beq	.L201
	add	r4, r3, r3, lsl #1
	mvn	r3, #191
	cmp	r4, r3
	movlt	r4, r3
	cmp	r4, #256
	movge	r4, #256
	b	.L201
.L204:
	.align	2
.L203:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VEDU_DRV_RCStartQPChange, .-VEDU_DRV_RCStartQPChange
	.align	2
	.global	D_VENC_RC_CLIP3
	.type	D_VENC_RC_CLIP3, %function
D_VENC_RC_CLIP3:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, r1
	movlt	r0, r1
	cmp	r0, r2
	movge	r0, r2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	D_VENC_RC_CLIP3, .-D_VENC_RC_CLIP3
	.align	2
	.global	VEDU_DRV_RCUpdateIMBInfo
	.type	VEDU_DRV_RCUpdateIMBInfo, %function
VEDU_DRV_RCUpdateIMBInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #80]
	mov	r4, r0
	ldr	r8, [r0, #128]
	mov	r2, r0
	cmp	r3, #1
	ldr	r7, [r0, #124]
	ldr	r6, [r0, #120]
	add	ip, r0, #132
	ldreq	r3, [r0, #136]
	mov	r1, #6
	ldrne	r3, [r0, #108]
	streq	r3, [r0, #108]
	mov	r0, #0
	ldr	r5, [r4, #116]
	ldr	lr, [r4, #112]
	str	r8, [r4, #132]
	str	r7, [r4, #128]
	str	r6, [r4, #124]
	str	r5, [r4, #120]
	str	lr, [r4, #116]
	str	r3, [r2, #112]!
	b	.L211
.L215:
	ldr	r3, [r2, #4]!
.L211:
	cmp	r3, #0
	add	r0, r0, r3
	subeq	r1, r1, #1
	cmp	r2, ip
	bne	.L215
	cmp	r1, #0
	moveq	r0, r1
	beq	.L212
	bl	__aeabi_idiv
.L212:
	str	r0, [r4, #136]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	VEDU_DRV_RCUpdateIMBInfo, .-VEDU_DRV_RCUpdateIMBInfo
	.align	2
	.global	VENC_DRV_RcCloseOneFrm
	.type	VENC_DRV_RcCloseOneFrm, %function
VENC_DRV_RcCloseOneFrm:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	ldr	r0, [r0, #60]
	cmp	r0, #0
	bne	.L217
	ldr	ip, [r4, #64]
	cmp	ip, #0
	bne	.L217
	str	ip, [r4, #20]
	ldr	lr, [r4, #32]
	ldr	r0, [r2]
	rsb	r0, lr, r0
	str	r0, [r2]
	ldr	r2, [r4, #44]
	cmp	r2, #0
	ldreq	r2, [r3]
	movne	r2, ip
	addeq	r2, r2, #1
	str	r2, [r3]
	ldr	r2, [r1]
	ldr	r0, [r4, #24]
	cmp	r2, #0
	beq	.L227
	ldr	r3, [r1, #16]
	ldr	lr, [r1, #12]
	cmp	lr, r3
	ldrhi	r5, [r1, #8]
	ldrls	r3, [r1, #8]
	ldrhi	r3, [r2, r5, asl #2]
	addhi	r2, r2, r5, lsl #2
	addls	r2, r2, r3, lsl #2
	rsbhi	ip, r3, lr
	add	r3, ip, r0
	str	r3, [r1, #12]
	str	r0, [r2]
	ldmib	r1, {r2, r3}
	add	r3, r3, #1
	str	r3, [r1, #8]
	cmp	r3, r2
	rsbcs	r3, r2, r3
	strcs	r3, [r1, #8]
.L227:
	mov	r0, r4
	bl	VEDU_DRV_RCUpdateIMBInfo
	ldr	r3, [r4, #44]
	ldr	r0, [r4, #556]
	cmp	r3, #0
	ldr	r3, [r4, #164]
	strne	r3, [r4, #96]
	streq	r3, [r4, #100]
	cmp	r0, #0
	ldr	r3, [r4, #168]
	str	r3, [r4, #104]
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r2, [r4, #564]
	mov	r0, #0
	ldr	r3, [r4, #24]
	ldr	r1, [r4, #560]
	ldr	lr, [r4, #92]
	add	r3, r2, r3
	ldr	ip, [r4, #172]
	ldr	r2, [r4, #84]
	rsb	r3, lr, r3
	add	r1, r1, ip
	str	r3, [r4, #564]
	str	r1, [r4, #560]
	add	r3, r2, #1
	str	r3, [r4, #84]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L217:
	ldr	r3, [r4, #48]
	ldr	r2, [r4, #52]
	clz	r3, r3
	ldr	r0, [r4, #56]
	cmp	r2, #0
	mov	r3, r3, lsr #5
	str	r3, [r4, #48]
	ldr	r3, [r4, #20]
	subne	r2, r2, #1
	moveq	r2, #2
	sub	r0, r0, #1
	cmp	r3, #0
	str	r0, [r4, #56]
	str	r2, [r4, #52]
	beq	.L220
.L222:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L220:
	ldr	r2, [r1]
	cmp	r2, #0
	beq	.L222
	ldr	r0, [r1, #12]
	ldr	ip, [r1, #16]
	cmp	r0, ip
	strls	r3, [r1, #12]
	ldrhi	r3, [r1, #8]
	ldrls	r0, [r1, #8]
	ldrhi	ip, [r2, r3, asl #2]
	addhi	r2, r2, r3, lsl #2
	addls	r2, r2, r0, lsl #2
	mov	r3, #0
	rsbhi	r0, ip, r0
	strhi	r0, [r1, #12]
	str	r3, [r2]
	ldmib	r1, {r2, r3}
	add	r3, r3, #1
	str	r3, [r1, #8]
	cmp	r3, r2
	bcc	.L222
	rsb	r3, r2, r3
	mvn	r0, #0
	str	r3, [r1, #8]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_DRV_RcCloseOneFrm, .-VENC_DRV_RcCloseOneFrm
	.align	2
	.global	VENC_DRV_EflRcAverage
	.type	VENC_DRV_EflRcAverage, %function
VENC_DRV_EflRcAverage:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	lr, r1, #0
	ble	.L244
	sub	ip, r0, #4
	mov	r0, #0
	mov	r3, r0
	mov	r1, lr
.L242:
	ldr	r2, [ip, #4]!
	add	r3, r3, #1
	cmp	r2, #0
	add	r0, r0, r2
	subeq	r1, r1, #1
	cmp	r3, lr
	bne	.L242
.L240:
	cmp	r1, #0
	beq	.L245
	bl	__aeabi_idiv
	ldmfd	sp, {fp, sp, pc}
.L245:
	mov	r0, r1
	ldmfd	sp, {fp, sp, pc}
.L244:
	mov	r1, lr
	mov	r0, #0
	b	.L240
	UNWIND(.fnend)
	.size	VENC_DRV_EflRcAverage, .-VENC_DRV_EflRcAverage
	.align	2
	.global	VENC_DRV_CalRCRatio
	.type	VENC_DRV_CalRCRatio, %function
VENC_DRV_CalRCRatio:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, r1
	bhi	.L250
	rsb	r0, r0, r1
	mov	r3, #100
	mul	r0, r3, r0
	bl	__aeabi_uidiv
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
.L250:
	rsb	r0, r1, r0
	mov	r3, #100
	mul	r0, r3, r0
	bl	__aeabi_uidiv
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VENC_DRV_CalRCRatio, .-VENC_DRV_CalRCRatio
	.section	.rodata
	.align	2
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
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
