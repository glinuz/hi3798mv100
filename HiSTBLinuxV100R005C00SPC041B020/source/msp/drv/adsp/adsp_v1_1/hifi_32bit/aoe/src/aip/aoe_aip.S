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
	.file	"aoe_aip.c"
	.text
	.align	2
	.type	AIPDestroyChn, %function
AIPDestroyChn:
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
	ldr	r0, [r4, #128]
	mov	r3, #0
	str	r3, [r4, #152]
	cmp	r0, r3
	beq	.L3
	bl	RBUF_Destroy
.L3:
	ldr	r0, [r4, #132]
	cmp	r0, #0
	beq	.L4
	bl	RBUF_Destroy
.L4:
	ldr	r0, [r4, #144]
	cmp	r0, #0
	beq	.L5
	bl	FADE_Destroy
.L5:
	ldr	r0, [r4, #136]
	cmp	r0, #0
	beq	.L6
	bl	LSRC_Destroy
.L6:
	ldr	r0, [r4, #140]
	cmp	r0, #0
	beq	.L7
	ldr	r3, [r4, #240]
	cmp	r3, #2
	beq	.L32
.L8:
	bl	ResamplerFree
.L7:
	ldr	r1, [r4, #216]
	cmp	r1, #0
	beq	.L9
	mov	r0, #26
	bl	HI_ADSP_FREE
.L9:
	mov	r1, r4
	mov	r0, #26
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	HI_ADSP_FREE
.L32:
	ldr	r1, [r4, #148]
	cmp	r1, #0
	beq	.L8
	mov	r0, #26
	bl	HI_ADSP_FREE
	b	.L7
	UNWIND(.fnend)
	.size	AIPDestroyChn, .-AIPDestroyChn
	.align	2
	.type	AIPGainPcmData, %function
AIPGainPcmData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r3, .L46
	mov	r4, #0
	mov	r6, r0
	mov	r2, #20
	str	r4, [fp, #-48]
	ldr	r3, [r3, #24]
	mov	r5, r1
	str	r4, [fp, #-44]
	add	r1, r6, #156
	str	r4, [fp, #-40]
	sub	r0, fp, #48
	str	r4, [fp, #-36]
	str	r4, [fp, #-32]
	blx	r3
	ldr	r3, [r6, #176]
	ldr	r2, [fp, #-44]
	ldr	r1, [fp, #-32]
	add	r2, r3, r2
	cmp	r2, #120
	subhi	r4, r2, #121
	ldr	r2, [fp, #-40]
	str	r4, [fp, #-44]
	add	r3, r3, r2
	ldr	r2, [fp, #-36]
	cmp	r3, #120
	subhi	r3, r3, #121
	movls	r3, #0
	cmp	r4, #121
	str	r3, [fp, #-40]
	ldr	r3, [r6, #180]
	add	r2, r3, r2
	add	r3, r3, r1
	str	r2, [fp, #-36]
	str	r3, [fp, #-32]
	beq	.L45
.L38:
	sub	r2, fp, #48
	mov	r0, r5
	mov	r1, #0
	bl	UTIL_LRGainProcess
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L45:
	ldr	r1, [fp, #-40]
	cmp	r2, #0
	cmpeq	r1, #121
	movne	r2, #1
	moveq	r2, #0
	cmp	r3, #0
	moveq	r3, r2
	orrne	r3, r2, #1
	cmp	r3, #0
	bne	.L38
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L47:
	.align	2
.L46:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	AIPGainPcmData, .-AIPGainPcmData
	.align	2
	.global	AIP_GetPcmType
	.type	AIP_GetPcmType, %function
AIP_GetPcmType:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L51
	add	r0, r0, #8
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r3, [r3]
	moveq	r3, #5
	str	r3, [r1]
	ldmfd	sp, {fp, sp, pc}
.L52:
	.align	2
.L51:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_GetPcmType, .-AIP_GetPcmType
	.global	__aeabi_uidiv
	.align	2
	.global	AIPFadeoutChnData
	.type	AIPFadeoutChnData, %function
AIPFadeoutChnData:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	mov	r4, r0
	sub	r1, fp, #64
	ldr	r0, [r0, #132]
	mov	r3, #0
	str	r3, [fp, #-64]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-52]
	str	r3, [fp, #-48]
	str	r3, [fp, #-44]
	str	r3, [fp, #-40]
	str	r3, [fp, #-36]
	str	r3, [fp, #-32]
	bl	RBUF_QueryInfo
	ldr	r3, [fp, #-64]
	cmp	r3, #0
	beq	.L53
	ldr	r6, [r4, #116]
	ldr	r0, [fp, #-48]
	mov	r1, r6
	bl	__aeabi_uidiv
	mov	r1, r6
	mov	r5, r0
	ldr	r0, [fp, #-44]
	bl	__aeabi_uidiv
	mov	r2, #8
	mov	r1, #2
	mov	r6, r0
	ldr	r0, [r4, #144]
	bl	FADE_Active
	ldr	r3, [r4, #76]
	ldr	r2, [r4, #72]
	ldr	r1, [r4, #80]
	ldr	r0, [r4, #144]
	bl	FADE_Flush
	ldr	r1, [fp, #-64]
	ldr	r3, [fp, #-60]
	mov	r2, r5
	ldr	r0, [r4, #144]
	add	r1, r1, r3
	bl	FADE_OutProcess
	rsb	r2, r5, r6
	ldr	r0, [r4, #144]
	ldr	r1, [fp, #-64]
	bl	FADE_OutProcess
.L53:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	AIPFadeoutChnData, .-AIPFadeoutChnData
	.align	2
	.global	AIPGetRbfFiFoAttr
	.type	AIPGetRbfFiFoAttr, %function
AIPGetRbfFiFoAttr:
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
	ldr	r3, .L62
	mov	r4, r1
	ldr	r5, [r3, r0, asl #2]
	add	r0, r1, #72
	ldr	r1, [r5, #4]
	and	r1, r1, #3
	bl	AOE_REG_ReadBitDepth
	cmp	r0, #0
	bne	.L61
	ldr	r1, [r5, #4]
	add	r0, r4, #76
	ubfx	r1, r1, #2, #2
	bl	AOE_REG_ReadChannel
	cmp	r0, #0
	bne	.L61
	ldr	r1, [r5, #4]
	add	r0, r4, #80
	ubfx	r1, r1, #4, #4
	bl	AOE_REG_ReadSampelRate
	cmp	r0, #0
	bne	.L61
	ldr	r1, [r5, #4]
	add	r0, r4, #84
	ubfx	r1, r1, #8, #2
	bl	AOE_REG_ReadDataFormat
	cmp	r0, #0
	bne	.L61
	ldr	r1, [r5, #4]
	sub	r0, fp, #32
	ubfx	r1, r1, #10, #6
	bl	AOE_REG_ReadBufLatencyMs
	subs	r6, r0, #0
	bne	.L61
	ldr	r1, [r4, #72]
	ldr	r0, [r4, #76]
	bl	UTIL_CalcFrameSize
	ldr	r2, [r4, #80]
	mov	r7, r0
	mov	r1, r0
	ldr	r0, [fp, #-32]
	bl	UTIL_LatencyMs2ByteSize
	ldr	r3, [fp, #-32]
	add	r1, r5, #68
	str	r7, [r4, #116]
	add	r5, r5, #72
	str	r6, [r4, #92]
	str	r6, [r4, #112]
	str	r3, [r4, #124]
	str	r3, [r4, #88]
	str	r1, [r4, #104]
	str	r5, [r4, #108]
	str	r0, [r4, #100]
	str	r0, [r4, #120]
	mov	r0, r6
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L61:
	mvn	r6, #0
	mov	r0, r6
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L63:
	.align	2
.L62:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPGetRbfFiFoAttr, .-AIPGetRbfFiFoAttr
	.align	2
	.global	AIPGetRbfInAttr
	.type	AIPGetRbfInAttr, %function
AIPGetRbfInAttr:
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
	ldr	r3, .L80
	mov	r4, r1
	ldr	r5, [r3, r0, asl #2]
	add	r0, r1, #16
	ldr	r1, [r5]
	and	r1, r1, #3
	bl	AOE_REG_ReadBitDepth
	cmp	r0, #0
	bne	.L67
	ldr	r1, [r5]
	add	r0, r4, #20
	ubfx	r1, r1, #2, #3
	bl	AOE_REG_ReadChannel
	cmp	r0, #0
	bne	.L67
	ldr	r1, [r5]
	add	r0, r4, #24
	ubfx	r1, r1, #5, #4
	bl	AOE_REG_ReadSampelRate
	cmp	r0, #0
	beq	.L70
	ldr	r3, [r5]
	ubfx	r3, r3, #5, #4
	cmp	r3, #15
	beq	.L79
.L70:
	ldr	r1, [r5]
	add	r0, r4, #28
	ubfx	r1, r1, #9, #2
	bl	AOE_REG_ReadDataFormat
	subs	r6, r0, #0
	bne	.L67
	ldr	r3, [r4, #28]
	cmp	r3, #0
	bne	.L74
	ldr	r1, [r5]
	mov	r0, r4
	ubfx	r1, r1, #11, #3
	bl	AOE_REG_ReadPcmType
	cmp	r0, #0
	bne	.L67
.L74:
	ldr	r3, [r5]
	ldr	r1, [r4, #16]
	ubfx	r3, r3, #14, #1
	ldr	r0, [r4, #20]
	str	r3, [r4, #4]
	bl	UTIL_CalcFrameSize
	mov	r2, #0
	mov	r3, #1
	str	r2, [fp, #-36]
	str	r3, [r4, #36]
	str	r2, [fp, #-32]
	ldr	r3, [r5, #16]
	str	r3, [fp, #-36]
	ldr	r1, [fp, #-36]
	ldr	r3, [r5, #20]
	str	r1, [r4, #40]
	str	r3, [fp, #-32]
	ldr	r3, [r5, #24]
	bic	r3, r3, #-16777216
	str	r3, [r4, #44]
	ldr	r1, [r5, #24]
	tst	r1, #16777216
	bne	.L72
	add	r1, r5, #28
	add	r2, r5, #36
	str	r1, [r4, #48]
	str	r2, [r4, #52]
.L75:
	ldr	r2, [r5, #24]
	mov	r1, #0
	str	r0, [r4, #60]
	mov	r0, #16
	ubfx	r2, r2, #24, #1
	str	r3, [r4, #64]
	str	r0, [r4, #8]
	mov	r3, #8
	mov	r0, r6
	str	r2, [r4, #56]
	str	r1, [r4, #68]
	str	r1, [r4, #32]
	str	r3, [r4, #12]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L72:
	str	r2, [fp, #-36]
	str	r2, [fp, #-32]
	ldr	r1, [r5, #28]
	str	r1, [fp, #-36]
	ldr	ip, [fp, #-36]
	ldr	r1, [r5, #32]
	str	ip, [r4, #48]
	str	r1, [fp, #-32]
	str	r2, [fp, #-36]
	str	r2, [fp, #-32]
	ldr	r2, [r5, #36]
	str	r2, [fp, #-36]
	ldr	r1, [fp, #-36]
	ldr	r2, [r5, #40]
	str	r1, [r4, #52]
	str	r2, [fp, #-32]
	b	.L75
.L79:
	ldr	r3, [r5, #12]
	movw	r2, #56928
	movt	r2, 2
	bic	r3, r3, #-16777216
	str	r3, [r4, #24]
	sub	r3, r3, #4000
	cmp	r3, r2
	bhi	.L67
	ldr	r3, [r4, #20]
	cmp	r3, #2
	bhi	.L67
	ldr	r3, [r4, #16]
	cmp	r3, #16
	beq	.L70
.L67:
	mvn	r6, #0
	mov	r0, r6
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L81:
	.align	2
.L80:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPGetRbfInAttr, .-AIPGetRbfInAttr
	.align	2
	.global	AIPGetChnCreateAttr
	.type	AIPGetChnCreateAttr, %function
AIPGetChnCreateAttr:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	mov	r5, r1
	bl	AIPGetRbfInAttr
	cmp	r0, #0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	mov	r1, r5
	mov	r0, r4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	AIPGetRbfFiFoAttr
	UNWIND(.fnend)
	.size	AIPGetChnCreateAttr, .-AIPGetChnCreateAttr
	.align	2
	.global	AIPIsChnAttrChange
	.type	AIPIsChnAttrChange, %function
AIPIsChnAttrChange:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	ldr	r3, .L85
	add	r2, r0, #8
	sub	r1, fp, #156
	ldr	r5, [r3, r2, asl #2]
	bl	AIPGetChnCreateAttr
	ldr	lr, [fp, #-136]
	ldr	r3, [fp, #-140]
	ldr	r0, [r5, #20]
	ldr	ip, [r5, #16]
	subs	r4, r0, lr
	ldr	r2, [r5, #24]
	ldr	r0, [fp, #-132]
	movne	r4, #1
	cmp	ip, r3
	moveq	lr, r4
	addne	lr, r4, #1
	ldr	r3, [r5, #28]
	cmp	r2, r0
	moveq	ip, lr
	addne	ip, lr, #1
	ldr	r4, [fp, #-128]
	ldr	r0, [r5]
	ldr	lr, [fp, #-156]
	cmp	r3, r4
	moveq	r1, ip
	addne	r1, ip, #1
	ldr	r4, [r5, #4]
	ldr	ip, [fp, #-152]
	cmp	r0, lr
	moveq	r2, r1
	addne	r2, r1, #1
	ldr	lr, [r5, #40]
	ldr	r1, [fp, #-116]
	cmp	r4, ip
	moveq	r3, r2
	addne	r3, r2, #1
	ldr	ip, [r5, #44]
	ldr	r2, [fp, #-112]
	cmp	lr, r1
	moveq	r0, r3
	addne	r0, r3, #1
	ldr	r1, [r5, #56]
	ldr	r3, [fp, #-100]
	cmp	ip, r2
	moveq	r4, r0
	addne	r4, r0, #1
	ldr	r2, [r5, #72]
	ldr	r0, [fp, #-84]
	cmp	r1, r3
	moveq	lr, r4
	addne	lr, r4, #1
	ldr	r6, [fp, #-80]
	ldr	r3, [r5, #76]
	cmp	r2, r0
	moveq	ip, lr
	addne	ip, lr, #1
	ldr	r4, [fp, #-76]
	ldr	r0, [r5, #80]
	cmp	r3, r6
	moveq	r1, ip
	addne	r1, ip, #1
	ldr	lr, [r5, #84]
	ldr	ip, [fp, #-72]
	cmp	r0, r4
	moveq	r2, r1
	addne	r2, r1, #1
	ldr	r0, [r5, #88]
	ldr	r1, [fp, #-68]
	cmp	lr, ip
	moveq	r3, r2
	addne	r3, r2, #1
	cmp	r0, r1
	moveq	r0, r3
	addne	r0, r3, #1
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L86:
	.align	2
.L85:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPIsChnAttrChange, .-AIPIsChnAttrChange
	.align	2
	.global	AIPCreateChn
	.type	AIPCreateChn, %function
AIPCreateChn:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #252)
	sub	sp, sp, #252
	ldr	r3, .L155
	sub	r1, fp, #164
	mov	r4, #0
	mov	r7, r0
	str	r4, [fp, #-272]
	ldr	r5, [r3, r0, asl #2]
	str	r4, [fp, #-268]
	str	r4, [fp, #-264]
	str	r4, [fp, #-260]
	str	r4, [fp, #-256]
	bl	AIPGetChnCreateAttr
	subs	r6, r0, #0
	movne	r0, r4
	bne	.L151
	mov	r1, #312
	mov	r0, #26
	bl	HI_ADSP_MALLOC
	subs	r4, r0, #0
	beq	.L89
	ldr	r8, .L155+4
	mov	r2, #312
	mov	r1, r6
	ldr	r3, [r8, #20]
	blx	r3
	ldr	r3, [r8, #24]
	sub	r1, fp, #164
	mov	r2, #128
	mov	r0, r4
	blx	r3
	str	r7, [r4, #236]
	sub	r1, fp, #128
	sub	r0, fp, #260
	bl	RBUF_Create_EXT
	ldr	r3, [fp, #-260]
	cmp	r3, #0
	beq	.L89
	ldr	r2, [fp, #-136]
	str	r3, [r4, #128]
	cmp	r2, #0
	beq	.L152
	str	r6, [r5, #60]
	ldr	r3, [r5, #64]
	bfi	r3, r6, #0, #16
	str	r3, [r5, #64]
.L91:
	mov	r1, #768
	mov	r0, #26
	bl	HI_ADSP_MALLOC
	cmp	r0, #0
	str	r0, [r4, #216]
	beq	.L89
	ldr	r3, [fp, #-136]
	cmp	r3, #0
	bne	.L93
	ldr	ip, [fp, #-140]
	ldr	r2, [fp, #-84]
	cmp	ip, r2
	beq	.L94
	cmp	ip, #32000
	beq	.L95
	bls	.L153
	movw	r3, #22664
	movt	r3, 1
	cmp	ip, r3
	beq	.L95
	bhi	.L99
	movw	r3, #44100
	cmp	ip, r3
	bne	.L95
	ldr	lr, [fp, #-88]
	mov	r0, #2
	ldr	r3, [fp, #-92]
	sub	r1, fp, #248
	str	r0, [r4, #240]
	sub	r0, fp, #252
	str	ip, [fp, #-248]
	str	r2, [fp, #-244]
	str	lr, [fp, #-236]
	str	r3, [fp, #-240]
	bl	iMedia_ASRC_GetSize
	cmp	r0, #0
	bne	.L89
	ldr	r1, [fp, #-252]
	mov	r0, #26
	bl	HI_ADSP_MALLOC
	cmp	r0, #0
	str	r0, [r4, #148]
	beq	.L89
	ldr	r7, [fp, #-88]
	mov	r1, r0
	ldr	r6, [fp, #-92]
	sub	r3, fp, #232
	ldr	lr, [fp, #-140]
	sub	r0, fp, #272
	ldr	ip, [fp, #-84]
	ldr	r2, [fp, #-252]
	str	r7, [fp, #-220]
	str	r6, [fp, #-224]
	str	lr, [fp, #-232]
	str	ip, [fp, #-228]
	bl	iMedia_ASRC_Init
	cmp	r0, #0
	bne	.L89
	ldr	r0, [fp, #-272]
	b	.L106
.L153:
	movw	r3, #12000
	cmp	ip, r3
	beq	.L95
.L95:
	mov	r3, #1
	mov	r1, r2
	str	r3, [r4, #240]
	mov	r0, ip
	ldr	r3, [fp, #-92]
	mov	ip, #0
	ldr	r2, [fp, #-88]
	str	ip, [sp]
	bl	ResamplerCreate
	cmp	r0, #0
	str	r0, [fp, #-272]
	beq	.L89
.L106:
	ldr	r2, [fp, #-84]
	str	r0, [r4, #140]
.L94:
	ldr	ip, [fp, #-88]
	mov	r1, r2
	ldr	r3, [fp, #-92]
	sub	r0, fp, #268
	str	ip, [sp]
	bl	LSRC_Create
	ldr	r3, [fp, #-268]
	cmp	r3, #0
	beq	.L89
	str	r3, [r4, #136]
	mov	r6, #0
	str	r6, [r5, #80]
	ldr	r3, [r4]
	cmp	r3, r6
	beq	.L93
	ldr	r3, [fp, #-144]
	sub	r0, fp, #264
	ldr	r2, [fp, #-148]
	ldr	r1, [fp, #-140]
	bl	FADE_Create
	ldr	r3, [fp, #-264]
	cmp	r3, r6
	beq	.L89
	str	r3, [r4, #144]
	str	r6, [r4, #204]
.L93:
	mov	r3, #0
	mov	r2, #3
	mov	r1, #256
	str	r3, [r4, #156]
	str	r3, [r4, #160]
	str	r3, [r4, #164]
	str	r3, [r4, #188]
	str	r1, [r4, #288]
	str	r2, [r4, #292]
	ldr	r1, [r4, #72]
	ldr	r0, [r4, #76]
	bl	UTIL_CalcFrameSize
	ldr	r3, [r4, #80]
	movw	r2, #19923
	movw	r1, #48000
	cmp	r3, r1
	movt	r2, 4194
	add	r3, r3, r3, lsl #2
	umull	r2, r3, r3, r2
	mov	r3, r3, lsr #6
	beq	.L154
	tst	r3, #3
	bicne	r3, r3, #3
	addne	r3, r3, #4
.L105:
	mul	r3, r0, r3
	ldr	r1, [r4, #72]
	ldr	r0, [r4, #76]
	str	r3, [r4, #220]
	bl	UTIL_CalcFrameSize
	ldr	r2, [r4, #88]
	ldr	r3, [r4, #80]
	movw	ip, #19923
	movt	ip, 4194
	ldr	r1, [r4, #72]
	mul	r3, r3, r2
	umull	r2, r3, r3, ip
	mov	r3, r3, lsr #6
	mul	r3, r3, r0
	ldr	r0, [r4, #76]
	str	r3, [r4, #232]
	bl	UTIL_CalcFrameSize
	ldr	r1, [r4, #16]
	str	r0, [r4, #224]
	ldr	r0, [r4, #20]
	bl	UTIL_CalcFrameSize
	mov	r3, #1
	mov	r1, r3
	str	r3, [r4, #152]
	mov	r2, #8
	str	r0, [r4, #228]
	ldr	r0, [r4, #144]
	bl	FADE_Active
	mov	r0, r4
.L151:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L152:
	sub	r0, fp, #256
	sub	r1, fp, #72
	bl	RBUF_Create
	ldr	r0, [fp, #-256]
	cmp	r0, #0
	beq	.L89
	str	r0, [r4, #132]
	sub	r1, fp, #200
	bl	RBUF_QueryInfo
	ldrh	r2, [fp, #-188]
	ldr	r3, [r5, #64]
	bfi	r3, r2, #0, #16
	str	r3, [r5, #64]
	b	.L91
.L154:
	ands	r2, r3, #255
	addne	r3, r3, #256
	rsbne	r3, r2, r3
	b	.L105
.L99:
	b	.L95
.L89:
	mov	r0, r4
	bl	AIPDestroyChn
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L156:
	.align	2
.L155:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	AIPCreateChn, .-AIPCreateChn
	.align	2
	.global	AIPFlushChnCtx
	.type	AIPFlushChnCtx, %function
AIPFlushChnCtx:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	ldr	r0, [r0, #140]
	cmp	r0, #0
	beq	.L158
	ldr	r3, [r4, #240]
	cmp	r3, #2
	beq	.L168
	bl	ResampleFlush
.L158:
	ldr	r0, [r4, #136]
	cmp	r0, #0
	beq	.L160
	bl	LSRC_Flush
.L160:
	ldr	r0, [r4, #144]
	mov	r3, #0
	str	r3, [r4, #212]
	cmp	r0, r3
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	mov	r2, #8
	mov	r1, #1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	FADE_Active
.L168:
	bl	iMedia_ASRC_Flush
	b	.L158
	UNWIND(.fnend)
	.size	AIPFlushChnCtx, .-AIPFlushChnCtx
	.align	2
	.global	AIPFlushChnRufIn
	.type	AIPFlushChnRufIn, %function
AIPFlushChnRufIn:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r0, [r0, #128]
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_Flush
	UNWIND(.fnend)
	.size	AIPFlushChnRufIn, .-AIPFlushChnRufIn
	.align	2
	.global	AIPFlushChnFifo
	.type	AIPFlushChnFifo, %function
AIPFlushChnFifo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r0, [r0, #132]
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_Flush
	UNWIND(.fnend)
	.size	AIPFlushChnFifo, .-AIPFlushChnFifo
	.align	2
	.global	AIPFlushChn
	.type	AIPFlushChn, %function
AIPFlushChn:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	tst	r1, #1
	mov	r4, r1
	mov	r5, r0
	bne	.L192
.L174:
	tst	r4, #2
	beq	.L175
	ldr	r0, [r5, #128]
	cmp	r0, #0
	beq	.L175
	bl	RBUF_Flush
.L175:
	tst	r4, #4
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r5, #132]
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	RBUF_Flush
.L192:
	bl	AIPFlushChnCtx
	b	.L174
	UNWIND(.fnend)
	.size	AIPFlushChn, .-AIPFlushChn
	.align	2
	.global	AIPWriteMuteFrame
	.type	AIPWriteMuteFrame, %function
AIPWriteMuteFrame:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	ldr	r0, [r0, #132]
	ldr	r3, [r4, #224]
	ldr	r5, [r4, #220]
	cmp	r0, #0
	add	r5, r3, r5
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	bl	RBUF_QueryFree
	cmp	r5, r0
	ldmcsfd	sp, {r4, r5, fp, sp, pc}
	mov	r2, r5
	ldr	r0, [r4, #132]
	mov	r1, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	RBUF_WriteData
	UNWIND(.fnend)
	.size	AIPWriteMuteFrame, .-AIPWriteMuteFrame
	.align	2
	.global	AIPGetChnCmd
	.type	AIPGetChnCmd, %function
AIPGetChnCmd:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L207
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #8]
	tst	r2, #8
	movne	r0, #4
	beq	.L206
.L198:
	str	r0, [r1]
	ldmfd	sp, {fp, sp, pc}
.L206:
	ldr	r2, [r3, #8]
	and	r2, r2, #3
	cmp	r2, #3
	ldrls	pc, [pc, r2, asl #2]
	b	.L199
.L200:
	.word	.L205
	.word	.L201
	.word	.L202
	.word	.L203
.L205:
	mov	r0, #0
	b	.L198
.L203:
	mov	r0, #3
	b	.L198
.L202:
	mov	r0, #2
	b	.L198
.L201:
	mov	r0, #1
	b	.L198
.L199:
	ldr	r2, [r3, #8]
	mov	ip, #4
	mov	r0, ip
	bfi	r2, ip, #4, #4
	str	r2, [r3, #8]
	ldr	r2, [r3, #8]
	orr	r2, r2, #8
	str	r2, [r3, #8]
	b	.L198
.L208:
	.align	2
.L207:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPGetChnCmd, .-AIPGetChnCmd
	.align	2
	.global	AIPAckChnCmd
	.type	AIPAckChnCmd, %function
AIPAckChnCmd:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L211
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #8]
	tst	r2, #8
	ldmnefd	sp, {fp, sp, pc}
	ldr	r2, [r3, #8]
	bfi	r2, r1, #4, #4
	str	r2, [r3, #8]
	ldr	r2, [r3, #8]
	orr	r2, r2, #8
	str	r2, [r3, #8]
	ldmfd	sp, {fp, sp, pc}
.L212:
	.align	2
.L211:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPAckChnCmd, .-AIPAckChnCmd
	.align	2
	.global	IsDataEnoughForFadeout
	.type	IsDataEnoughForFadeout, %function
IsDataEnoughForFadeout:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	ldr	r0, [r0, #128]
	bl	RBUF_QueryBusy
	ldr	r1, [r4, #16]
	mov	r5, r0
	ldr	r0, [r4, #20]
	bl	UTIL_CalcFrameSize
	ldr	r2, [r4, #24]
	mov	r1, r0
	mov	r0, r5
	bl	UTIL_LatencyMs
	mov	r5, r0
	ldr	r0, [r4, #132]
	bl	RBUF_QueryBusy
	ldr	r1, [r4, #72]
	mov	r6, r0
	ldr	r0, [r4, #76]
	bl	UTIL_CalcFrameSize
	ldr	r2, [r4, #80]
	mov	r1, r0
	mov	r0, r6
	bl	UTIL_LatencyMs
	ldr	r1, [r4, #80]
	mov	r6, r0
	ldr	r0, [r4, #212]
	bl	__aeabi_uidiv
	mov	r3, #1000
	mla	r0, r3, r0, r6
	cmp	r0, #4
	cmpls	r5, #4
	movls	r0, #1
	movhi	r0, #0
	eor	r0, r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	IsDataEnoughForFadeout, .-IsDataEnoughForFadeout
	.align	2
	.global	AIPStopChnCmdProc
	.type	AIPStopChnCmdProc, %function
AIPStopChnCmdProc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L224
	add	r3, r0, #8
	mov	r7, r0
	ldr	r5, [r6, r0, asl #2]
	ldr	r4, [r6, r3, asl #2]
	cmp	r4, #0
	cmpne	r5, #0
	moveq	r8, #1
	movne	r8, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r4, #152]
	sub	r3, r3, #1
	cmp	r3, #1
	movhi	r2, r5
	bls	.L222
.L217:
	ldr	r3, [r2, #8]
	ubfx	r3, r3, #3, #1
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r1, [r2, #8]
	bfi	r1, r3, #4, #4
	str	r1, [r2, #8]
	ldr	r3, [r2, #8]
	orr	r3, r3, #8
	str	r3, [r2, #8]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L222:
	mov	r0, r4
	bl	IsDataEnoughForFadeout
	cmp	r0, #0
	bne	.L223
	mov	r1, #4
	mov	r0, r4
	bl	AIPFlushChn
	mov	r3, #1
	str	r3, [r4, #244]
.L219:
	mov	r1, #3
	mov	r0, r4
	bl	AIPFlushChn
	mov	r0, r4
	bl	AIPWriteMuteFrame
	mov	r3, #0
	ldr	r2, [r6, r7, asl #2]
	str	r3, [r4, #152]
	str	r3, [r5, #80]
	b	.L217
.L223:
	mov	r0, r4
	bl	AIPFadeoutChnData
	str	r8, [r4, #244]
	b	.L219
.L225:
	.align	2
.L224:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPStopChnCmdProc, .-AIPStopChnCmdProc
	.align	2
	.global	AIFlushChnCmdProc
	.type	AIFlushChnCmdProc, %function
AIFlushChnCmdProc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L232
	add	r3, r0, #8
	mov	r6, r0
	ldr	r4, [r5, r3, asl #2]
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r7, [r4, #152]
	cmp	r7, #1
	beq	.L231
.L229:
	ldr	r2, [r5, r6, asl #2]
	ldr	r3, [r2, #8]
	ubfx	r3, r3, #3, #1
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r1, [r2, #8]
	bfi	r1, r3, #4, #4
	str	r1, [r2, #8]
	ldr	r3, [r2, #8]
	orr	r3, r3, #8
	str	r3, [r2, #8]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L231:
	mov	r0, r4
	bl	AIPFadeoutChnData
	mov	r1, #3
	mov	r0, r4
	bl	AIPFlushChn
	mov	r0, r4
	bl	AIPWriteMuteFrame
	mov	r1, r7
	ldr	r0, [r4, #144]
	mov	r2, #8
	bl	FADE_Active
	b	.L229
.L233:
	.align	2
.L232:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIFlushChnCmdProc, .-AIFlushChnCmdProc
	.align	2
	.global	AIPPauseChnCmdProc
	.type	AIPPauseChnCmdProc, %function
AIPPauseChnCmdProc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L240
	add	r3, r0, #8
	mov	r6, r0
	ldr	r4, [r5, r3, asl #2]
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r7, [r4, #152]
	cmp	r7, #1
	beq	.L239
.L237:
	ldr	r2, [r5, r6, asl #2]
	ldr	r3, [r2, #8]
	ubfx	r3, r3, #3, #1
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r1, [r2, #8]
	bfi	r1, r3, #4, #4
	str	r1, [r2, #8]
	ldr	r3, [r2, #8]
	orr	r3, r3, #8
	str	r3, [r2, #8]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L239:
	mov	r0, r4
	bl	AIPFadeoutChnData
	mov	r1, r7
	mov	r0, r4
	bl	AIPFlushChn
	mov	r3, #2
	str	r3, [r4, #152]
	b	.L237
.L241:
	.align	2
.L240:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPPauseChnCmdProc, .-AIPPauseChnCmdProc
	.align	2
	.global	AIPStartChnCmdProc
	.type	AIPStartChnCmdProc, %function
AIPStartChnCmdProc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L255
	add	r7, r0, #8
	mov	r6, r0
	ldr	r5, [r4, r7, asl #2]
	cmp	r5, #0
	beq	.L251
	ldr	r3, [r5, #152]
	cmp	r3, #0
	beq	.L253
	cmp	r3, #2
	beq	.L252
.L247:
	ldr	r2, [r4, r6, asl #2]
	ldr	r3, [r2, #8]
	ubfx	r3, r3, #3, #1
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r1, [r2, #8]
	bfi	r1, r3, #4, #4
	str	r1, [r2, #8]
	ldr	r3, [r2, #8]
	orr	r3, r3, #8
	str	r3, [r2, #8]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L253:
	bl	AIPIsChnAttrChange
	cmp	r0, #0
	bne	.L254
.L252:
	mov	r1, #1
	ldr	r0, [r5, #144]
	str	r1, [r5, #152]
	mov	r2, #8
	bl	FADE_Active
	ldr	r0, [r4, r7, asl #2]
.L244:
	cmp	r0, #0
	bne	.L247
	ldr	r3, [r4, r6, asl #2]
	ldr	r2, [r3, #8]
	tst	r2, #8
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r2, [r3, #8]
	mov	r1, #1
	bfi	r2, r1, #4, #4
	str	r2, [r3, #8]
	ldr	r2, [r3, #8]
	orr	r2, r2, #8
	str	r2, [r3, #8]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L254:
	ldr	r0, [r4, r7, asl #2]
	bl	AIPDestroyChn
	mov	r0, r6
.L251:
	bl	AIPCreateChn
	str	r0, [r4, r7, asl #2]
	b	.L244
.L256:
	.align	2
.L255:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIPStartChnCmdProc, .-AIPStartChnCmdProc
	.align	2
	.global	AIP_Init
	.type	AIP_Init, %function
AIP_Init:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L260
	mov	r4, r0
	ldr	r8, .L260+4
	mov	r7, #0
	add	r6, r5, #32
.L258:
	str	r4, [r5, #4]!
	mov	r2, #96
	ldr	r3, [r8, #20]
	mov	r1, #0
	mov	r0, r4
	blx	r3
	ldr	r3, [r4, #8]
	mov	r1, #121
	cmp	r5, r6
	orr	r3, r3, #8
	str	r3, [r4, #8]
	ldr	r3, [r4, #8]
	mov	r2, #5
	bfi	r3, r1, #8, #7
	str	r3, [r4, #8]
	ldr	r3, [r4]
	bfi	r3, r2, #11, #3
	str	r3, [r4], #96
	str	r7, [r5, #32]
	bne	.L258
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L261:
	.align	2
.L260:
	.word	.LANCHOR0-4
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	AIP_Init, .-AIP_Init
	.align	2
	.global	AIP_DeInit
	.type	AIP_DeInit, %function
AIP_DeInit:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L265
	mov	r6, #0
	add	r5, r4, #32
.L263:
	ldr	r0, [r4, #4]!
	bl	AIPDestroyChn
	cmp	r4, r5
	str	r6, [r4]
	bne	.L263
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L266:
	.align	2
.L265:
	.word	.LANCHOR0+28
	UNWIND(.fnend)
	.size	AIP_DeInit, .-AIP_DeInit
	.align	2
	.global	AIP_Process
	.type	AIP_Process, %function
AIP_Process:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #108)
	sub	sp, sp, #108
	ldr	r6, .L402
	ldr	r10, .L402+4
	movw	r8, #19923
	movt	r8, 4194
	mov	r5, #0
.L322:
	sub	r1, fp, #80
	mov	r0, r5
	bl	AIPGetChnCmd
	cmp	r0, #4
	beq	.L268
	ldr	r3, [fp, #-80]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L268
.L270:
	.word	.L269
	.word	.L271
	.word	.L272
	.word	.L273
.L273:
	mov	r0, r5
	bl	AIFlushChnCmdProc
.L268:
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	beq	.L274
	ldr	r3, [r4, #152]
	cmp	r3, #1
	beq	.L386
.L274:
	add	r5, r5, #1
	cmp	r5, #8
	bne	.L322
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L272:
	mov	r0, r5
	bl	AIPPauseChnCmdProc
	b	.L268
.L269:
	mov	r0, r5
	bl	AIPStartChnCmdProc
	b	.L268
.L271:
	mov	r0, r5
	bl	AIPStopChnCmdProc
	b	.L268
.L386:
	ldr	r2, [r4, #28]
	cmp	r2, #0
	bne	.L274
	ldr	r3, [r6, #-32]
	str	r2, [r4, #156]
	ldr	r2, [r3, #48]
	uxtb	r2, r2
	str	r2, [r4, #160]
	ldr	r2, [r3, #48]
	ubfx	r2, r2, #8, #8
	str	r2, [r4, #164]
	ldr	r2, [r3, #76]
	uxtb	r2, r2
	str	r2, [r4, #176]
	ldr	r2, [r3, #76]
	tst	r2, #2048
	ldr	r2, [r3, #76]
	ubfxeq	r2, r2, #8, #3
	streq	r2, [r4, #180]
	beq	.L276
	tst	r2, #2048
	ldrne	r2, [r3, #76]
	ubfxne	r2, r2, #8, #3
	rsbne	r2, r2, #0
	strne	r2, [r4, #180]
.L276:
	ldr	r2, [r3, #48]
	ubfx	r2, r2, #16, #1
	str	r2, [r4, #184]
	ldr	r2, [r3, #48]
	ubfx	r2, r2, #17, #3
	str	r2, [r4, #188]
	ldr	r2, [r3, #48]
	ubfx	r2, r2, #23, #2
	str	r2, [r4, #192]
	cmp	r2, #1
	ldrne	r2, [r3, #48]
	moveq	r2, #256
	movne	r2, r2, lsr #25
	rsbne	r2, r2, #0
	str	r2, [r4, #288]
	ldr	r2, [r3, #48]
	ubfx	r2, r2, #20, #3
	str	r2, [r4, #292]
	ldr	r2, [r3, #8]
	tst	r2, #1610612736
	ldr	r2, [r3, #8]
	ubfxeq	r2, r2, #24, #5
	streq	r2, [r4, #208]
	bne	.L387
.L280:
	ldr	r2, [r3]
	ubfx	r2, r2, #14, #1
	str	r2, [r4, #4]
	ldr	r2, [r3, #4]
	ubfx	r2, r2, #22, #1
	str	r2, [r4, #248]
	ldr	r1, [r3, #76]
	cmp	r2, #1
	ubfx	r2, r1, #24, #1
	str	r2, [r4, #252]
	ldr	r0, [r3, #76]
	ubfx	r0, r0, #12, #4
	str	r0, [r4, #256]
	ldr	r3, [r3, #76]
	ubfx	r3, r3, #16, #8
	str	r3, [r4, #260]
	beq	.L282
	cmp	r2, #0
	ldr	r3, [r4, #264]
	beq	.L388
	cmp	r3, #0
	beq	.L286
	ldr	r3, [r4, #268]
.L287:
	ldr	r7, [r4, #220]
	str	r3, [fp, #-112]
	mov	r1, r7
	bl	AOP_QueryRbfFree
	cmp	r0, #0
	bne	.L389
.L285:
	ldr	r9, [r4, #212]
	ldr	r1, [r4, #224]
	ldr	r7, [r4, #220]
	mul	r3, r9, r1
	cmp	r3, r7
	bcs	.L390
	ldr	r3, [r4, #28]
	ldr	r2, [r4, #80]
	cmp	r3, #0
	ldr	ip, [r4, #76]
	ldr	r3, [r4, #72]
	movne	r9, r7
	beq	.L391
.L297:
	ldr	r0, [r4, #128]
	bl	RBUF_QueryBusy
	cmp	r0, r9
	bls	.L282
	cmp	r9, #0
	beq	.L282
	ldr	r0, [r4, #132]
	cmp	r0, #0
	beq	.L282
	mov	r1, r7
	bl	RBUF_CheckFree
	cmp	r0, #0
	beq	.L282
	cmp	r7, #0
	beq	.L282
	bl	AOE_GetWKBuf
	ldr	r1, [r4, #228]
	str	r9, [fp, #-104]
	mov	r7, r0
	mov	r0, r9
	str	r7, [fp, #-108]
	bl	__aeabi_uidiv
	ldr	r3, [r4, #20]
	mov	r1, r7
	ldr	lr, [r4, #28]
	mov	r2, r9
	ldr	r7, [r4, #24]
	str	r3, [fp, #-92]
	mov	r3, #0
	ldr	ip, [r4, #16]
	str	r7, [fp, #-88]
	str	lr, [fp, #-84]
	str	ip, [fp, #-96]
	str	r0, [fp, #-100]
	ldr	r0, [r4, #128]
	bl	RBUF_ReadDataDma
	ldr	r3, [r4, #28]
	cmp	r3, #0
	beq	.L392
.L303:
	ldr	r3, [r4, #184]
	cmp	r3, #1
	beq	.L393
.L317:
	ldr	r2, [r4, #224]
	ldr	r3, [r4, #212]
	ldr	r7, [r4, #220]
	mul	r2, r3, r2
	cmp	r2, #0
	bne	.L394
.L318:
	cmp	r7, #0
	ldr	r3, [fp, #-104]
	beq	.L320
	cmp	r7, r3
	movcs	r7, r3
	cmp	r7, #0
	beq	.L320
	mov	r2, r7
	ldr	r1, [fp, #-108]
	ldr	r0, [r4, #132]
	bl	RBUF_WriteData
	ldr	r3, [fp, #-104]
.L320:
	subs	r9, r3, r7
	beq	.L321
	ldr	r1, [fp, #-108]
	mov	r2, r9
	ldr	r3, [r10, #24]
	add	r1, r1, r7
	ldr	r0, [r4, #216]
	blx	r3
.L321:
	mov	r0, r9
	ldr	r1, [r4, #224]
	bl	__aeabi_uidiv
	str	r0, [r4, #212]
.L282:
	ldr	r4, [r6]
	ldr	r7, [r6, #-32]
	ldr	r0, [r4, #132]
	bl	RBUF_QueryBusy
	ldr	r2, [r4, #80]
	ldr	r1, [r4, #224]
	bl	UTIL_LatencyMs
	ldr	r3, [r7, #4]
	bfi	r3, r0, #16, #6
	str	r3, [r7, #4]
	b	.L274
.L387:
	ubfx	r2, r2, #29, #2
	cmp	r2, #1
	ldreq	r2, [r3, #8]
	ubfxeq	r2, r2, #24, #5
	rsbeq	r2, r2, #0
	streq	r2, [r4, #208]
	b	.L280
.L388:
	cmp	r3, #1
	streq	r2, [r4, #264]
	b	.L285
.L390:
	ldr	r0, [r4, #132]
	cmp	r0, #0
	beq	.L282
	mov	r1, r7
	bl	RBUF_CheckFree
	cmp	r0, #0
	beq	.L282
	cmp	r7, #0
	beq	.L282
	ldr	r2, [r4, #220]
	ldr	r1, [r4, #216]
	ldr	r0, [r4, #132]
	bl	RBUF_WriteData
	ldr	r1, [r4, #224]
	ldr	r0, [r4, #220]
	bl	__aeabi_uidiv
	ldr	r3, [r4, #212]
	rsb	r0, r0, r3
	str	r0, [r4, #212]
	b	.L282
.L389:
	ldr	r0, [r4, #128]
	sub	r1, fp, #112
	bl	RBUF_QueryBusy_ProvideRptr
	cmp	r7, r0
	bhi	.L285
	bl	AOE_GetWKBuf
	ldr	r1, [r4, #228]
	str	r7, [fp, #-104]
	mov	r9, r0
	mov	r0, r7
	str	r9, [fp, #-108]
	bl	__aeabi_uidiv
	ldr	ip, [r4, #20]
	mov	r1, r9
	ldr	lr, [r4, #28]
	sub	r3, fp, #112
	ldr	r9, [r4, #24]
	mov	r2, r7
	str	ip, [fp, #-92]
	sub	r7, fp, #108
	ldr	ip, [r4, #16]
	str	r9, [fp, #-88]
	str	lr, [fp, #-84]
	str	ip, [fp, #-96]
	str	r0, [fp, #-100]
	ldr	r0, [r4, #128]
	bl	RBUF_ReadDataDma
	ldr	r3, [r4, #192]
	cmp	r3, #1
	beq	.L395
.L291:
	mov	r1, r7
	mov	r0, r4
	bl	AIPGainPcmData
	ldr	r1, [r4, #188]
	ldr	r2, [r4, #72]
	sub	r3, r1, #1
	cmp	r3, #6
	bls	.L396
.L292:
	ldr	r3, [r4, #184]
	cmp	r3, #1
	beq	.L397
.L293:
	mov	r1, r7
	ldr	r0, [r4, #256]
	bl	AOP_WriteRbfData
	ldr	r3, [fp, #-112]
	str	r3, [r4, #268]
	b	.L285
.L391:
	mov	r0, r7
	str	r3, [fp, #-128]
	str	ip, [fp, #-124]
	str	r2, [fp, #-120]
	bl	__aeabi_uidiv
	ldr	lr, [r4, #136]
	ldr	ip, [fp, #-124]
	ldr	r2, [fp, #-120]
	ldr	r3, [fp, #-128]
	str	ip, [sp]
	mov	r1, r2
	rsb	r9, r9, r0
	mov	r0, lr
	bl	LSRC_UpdateChange
	ldr	r1, [r4, #208]
	ldr	r0, [r4, #136]
	bl	LSRC_SetSpeedAdjust
	mov	r1, r9
	ldr	r0, [r4, #136]
	bl	LSRC_GetMinInputNum
	ldr	r3, [r4, #140]
	cmp	r3, #0
	beq	.L298
	ldr	r2, [r4, #240]
	cmp	r2, #2
	beq	.L398
	mov	r1, r0
	mov	r0, r3
	bl	ResamplerGetMinInputNum
.L298:
	tst	r0, #3
	bicne	r0, r0, #3
	addne	r0, r0, #4
.L300:
	ldr	r9, [r4, #228]
	mul	r9, r9, r0
	b	.L297
.L286:
	ldr	r1, [r4, #72]
	ldr	r0, [r4, #76]
	bl	UTIL_CalcFrameSize
	ldr	r3, [r4, #260]
	ldr	r2, [r4, #24]
	sub	r1, fp, #80
	mul	r2, r3, r2
	mul	r2, r0, r2
	ldr	r0, [r4, #128]
	umull	r2, r3, r2, r8
	mov	r7, r3, lsr #6
	bl	RBUF_QueryInfo
	ldr	r1, [fp, #-52]
	ldr	r2, [fp, #-76]
	cmp	r7, r1
	ldr	r0, [fp, #-68]
	movcs	r7, r1
	add	r3, r7, r2
	cmp	r3, r0
	rsbcs	r3, r0, r2
	mov	r2, #1
	addcs	r3, r3, r7
	ldr	r0, [r4, #256]
	str	r3, [r4, #268]
	str	r2, [r4, #264]
	b	.L287
.L396:
	mov	r0, r7
	bl	UTIL_ChannelModeProcess
	b	.L292
.L395:
	add	r1, r4, #272
	mov	r0, r7
	bl	UTIL_DRE_16BIT_Process
	b	.L291
.L397:
	ldr	r3, [r10, #20]
	mov	r1, #0
	ldr	r2, [fp, #-104]
	ldr	r0, [fp, #-108]
	blx	r3
	b	.L293
.L398:
	ldr	r3, [r4, #24]
	ldr	r1, [r4, #80]
	mul	r0, r3, r0
	bl	__aeabi_uidiv
	cmp	r0, #0
	addne	r0, r0, #1
	bne	.L298
	b	.L300
.L394:
	ldr	r1, [r4, #216]
	rsb	r7, r2, r7
	ldr	r0, [r4, #132]
	bl	RBUF_WriteData
	b	.L318
.L392:
	ldr	r3, [r4, #76]
	ldr	r2, [fp, #-92]
	ldr	r9, [r4, #72]
	cmp	r3, r2
	subeq	r7, fp, #108
	beq	.L304
	str	r3, [fp, #-120]
	bls	.L305
	bl	AOE_GetWKBuf
	ldr	r3, [fp, #-120]
	sub	r7, fp, #108
	mov	r2, r7
	mov	r1, r0
	mov	r0, r3
	bl	UTIL_UpmixPcmData
.L304:
	ldr	r3, [fp, #-96]
	cmp	r9, r3
	beq	.L306
	bl	AOE_GetWKBuf
	mov	r2, r7
	mov	r1, r0
	mov	r0, r9
	bl	UTIL_UnifyBitDepthPcmData
.L306:
	ldr	r3, [r4, #140]
	ldr	r9, [r4, #80]
	cmp	r3, #0
	beq	.L310
	bl	AOE_GetWKBuf
	ldr	r3, [r4, #240]
	cmp	r3, #2
	beq	.L399
	ldr	lr, [fp, #-88]
	mov	r2, r0
	mov	ip, #1
	ldr	r3, [fp, #-100]
	str	r0, [fp, #-120]
	ldr	r1, [fp, #-108]
	ldr	r0, [r4, #140]
	stmib	sp, {r9, ip}
	str	lr, [sp]
	bl	ResamplerProcessFrame
	ldr	r3, [fp, #-92]
	ldr	ip, [fp, #-120]
	mov	r2, r0
.L309:
	mov	r0, r3
	ldr	r1, [fp, #-96]
	str	ip, [fp, #-108]
	str	r2, [fp, #-100]
	str	r9, [fp, #-88]
	bl	UTIL_CalcFrameSize
	ldr	r3, [fp, #-100]
	mul	r0, r3, r0
	str	r0, [fp, #-104]
.L310:
	ldr	r0, [r4, #136]
	ldr	r3, [r4, #236]
	ldr	r2, .L402+8
	cmp	r0, #0
	ldr	r9, [r2, r3, asl #2]
	beq	.L312
	bl	LSRC_CheckDoSRC
	cmp	r0, #0
	beq	.L312
	ldr	r3, [fp, #-100]
	str	r3, [fp, #-120]
	bl	AOE_GetWKBuf
	ldr	r3, [fp, #-120]
	ldr	r1, [fp, #-108]
	mov	r2, r0
	str	r0, [fp, #-120]
	ldr	r0, [r4, #136]
	bl	LSRC_ProcessFrame
	ldr	ip, [fp, #-120]
	ldr	r1, [fp, #-96]
	str	ip, [fp, #-108]
	str	r0, [fp, #-100]
	ldr	r0, [fp, #-92]
	bl	UTIL_CalcFrameSize
	ldr	r3, [fp, #-100]
	ldr	r2, [r9, #80]
	add	r2, r2, #1
	str	r2, [r9, #80]
	mul	r0, r3, r0
	str	r0, [fp, #-104]
.L312:
	ldr	r3, [r4, #192]
	cmp	r3, #1
	beq	.L400
.L314:
	mov	r1, r7
	mov	r0, r4
	bl	AIPGainPcmData
	ldr	r1, [r4, #188]
	ldr	r2, [r4, #72]
	sub	r3, r1, #1
	cmp	r3, #6
	bls	.L401
.L315:
	ldr	r0, [r4, #144]
	cmp	r0, #0
	beq	.L303
	ldr	r2, [fp, #-96]
	ldr	r1, [fp, #-88]
	ldr	r3, [fp, #-92]
	bl	FADE_Flush
	ldr	r2, [fp, #-100]
	ldr	r1, [fp, #-108]
	ldr	r0, [r4, #144]
	bl	FADE_InProcess
	b	.L303
.L393:
	ldr	r3, [r10, #20]
	mov	r1, #0
	ldr	r2, [fp, #-104]
	ldr	r0, [fp, #-108]
	blx	r3
	b	.L317
.L305:
	bl	AOE_GetWKBuf
	ldr	r3, [fp, #-120]
	sub	r7, fp, #108
	mov	r2, r7
	mov	r1, r0
	mov	r0, r3
	bl	UTIL_DownmixPcmData
	b	.L304
.L399:
	ldr	r1, [fp, #-92]
	ldr	r3, [fp, #-100]
	ldr	r2, [fp, #-108]
	str	r0, [fp, #-76]
	mul	r3, r3, r1
	str	r2, [fp, #-80]
	ldr	r2, [r10, #32]
	str	r0, [fp, #-124]
	str	r3, [fp, #-72]
	blx	r2
	sub	r1, fp, #80
	ldr	r0, [r4, #140]
	bl	iMedia_ASRC_Apply
	ldr	r3, [r10, #36]
	blx	r3
	ldr	r3, [fp, #-92]
	ldr	r0, [fp, #-68]
	mov	r1, r3
	str	r3, [fp, #-120]
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-120]
	ldr	ip, [fp, #-124]
	mov	r2, r0
	b	.L309
.L401:
	mov	r0, r7
	bl	UTIL_ChannelModeProcess
	b	.L315
.L400:
	add	r1, r4, #272
	mov	r0, r7
	bl	UTIL_DRE_16BIT_Process
	b	.L314
.L403:
	.align	2
.L402:
	.word	.LANCHOR0+28
	.word	g_AdspOsalFunc
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_Process, .-AIP_Process
	.align	2
	.global	AIP_GetChnStatus
	.type	AIP_GetChnStatus, %function
AIP_GetChnStatus:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L407
	add	r0, r0, #8
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	streq	r3, [r1]
	ldrne	r2, [r3, #152]
	strne	r2, [r1]
	ldrne	r2, [r3, #4]
	strne	r2, [r1, #4]
	ldrne	r3, [r3, #244]
	strne	r3, [r1, #8]
	ldmfd	sp, {fp, sp, pc}
.L408:
	.align	2
.L407:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_GetChnStatus, .-AIP_GetChnStatus
	.align	2
	.global	AIP_FlushCutoffFlag
	.type	AIP_FlushCutoffFlag, %function
AIP_FlushCutoffFlag:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L414
	add	r0, r0, #8
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	movne	r2, #0
	strne	r2, [r3, #244]
	ldmfd	sp, {fp, sp, pc}
.L415:
	.align	2
.L414:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_FlushCutoffFlag, .-AIP_FlushCutoffFlag
	.align	2
	.global	AIP_QueryFiFoInfo
	.type	AIP_QueryFiFoInfo, %function
AIP_QueryFiFoInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L423
	add	r2, r0, #8
	mov	r0, r1
	ldr	r3, [r3, r2, asl #2]
	cmp	r3, #0
	beq	.L420
	ldr	r2, [r3, #248]
	cmp	r2, #0
	beq	.L421
	ldr	r2, [r3, #152]
	cmp	r2, #1
	beq	.L422
	ldr	r3, .L423+4
	mov	r2, #36
	mov	r1, #0
	ldr	r3, [r3, #20]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L422:
	ldr	r0, [r3, #128]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_QueryInfo
.L421:
	ldr	r0, [r3, #132]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_QueryInfo
.L420:
	ldr	ip, .L423+4
	mov	r1, r3
	mov	r2, #36
	ldr	r3, [ip, #20]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L424:
	.align	2
.L423:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	AIP_QueryFiFoInfo, .-AIP_QueryFiFoInfo
	.align	2
	.global	AIP_QueryFiFoBusy
	.type	AIP_QueryFiFoBusy, %function
AIP_QueryFiFoBusy:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L429
	add	r0, r0, #8
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, [r0, #248]
	cmp	r3, #0
	beq	.L428
	ldr	r0, [r0, #128]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_QueryBusy
.L428:
	ldr	r0, [r0, #132]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_QueryBusy
.L430:
	.align	2
.L429:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_QueryFiFoBusy, .-AIP_QueryFiFoBusy
	.align	2
	.global	AIP_QueryBusy
	.type	AIP_QueryBusy, %function
AIP_QueryBusy:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L437
	add	r0, r0, #8
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L432
	ldr	r0, [r3, #128]
	cmp	r0, #0
	beq	.L432
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_QueryBusy
.L432:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L438:
	.align	2
.L437:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_QueryBusy, .-AIP_QueryBusy
	.align	2
	.global	AIP_ReadDataComplete
	.type	AIP_ReadDataComplete, %function
AIP_ReadDataComplete:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L446
	add	r2, r0, #8
	ldr	r4, [r3, r2, asl #2]
	ldr	r5, [r3, r0, asl #2]
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, [r4, #248]
	cmp	r3, #0
	beq	.L445
	ldr	r0, [r4, #128]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	RBUF_ReadDataComplete
.L445:
	ldr	r0, [r4, #132]
	bl	RBUF_ReadDataComplete
	ldr	r0, [r4, #132]
	bl	RBUF_QueryBusy
	ldr	r2, [r4, #80]
	ldr	r1, [r4, #224]
	bl	UTIL_LatencyMs
	ldr	r3, [r5, #4]
	bfi	r3, r0, #16, #6
	str	r3, [r5, #4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L447:
	.align	2
.L446:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_ReadDataComplete, .-AIP_ReadDataComplete
	.align	2
	.global	AIP_UnderFlowCalc
	.type	AIP_UnderFlowCalc, %function
AIP_UnderFlowCalc:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L453
	add	r2, r0, #8
	ldr	r1, [r3, r2, asl #2]
	ldr	r2, [r3, r0, asl #2]
	cmp	r1, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, [r2, #64]
	ldr	r1, [r2, #64]
	mov	r3, r3, lsr #16
	add	r3, r3, #1
	uxth	r3, r3
	bfi	r1, r3, #16, #16
	str	r1, [r2, #64]
	ldmfd	sp, {fp, sp, pc}
.L454:
	.align	2
.L453:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_UnderFlowCalc, .-AIP_UnderFlowCalc
	.align	2
	.global	AIP_GetRingIn
	.type	AIP_GetRingIn, %function
AIP_GetRingIn:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L458
	add	r0, r0, #8
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	ldrne	r0, [r0, #128]
	ldmfd	sp, {fp, sp, pc}
.L459:
	.align	2
.L458:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_GetRingIn, .-AIP_GetRingIn
	.align	2
	.global	AIP_GetCompressInfo
	.type	AIP_GetCompressInfo, %function
AIP_GetCompressInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L464
	add	r0, r0, #8
	mov	lr, r1
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	streq	r3, [r1]
	streq	r3, [r2]
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r0, [r3, #128]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	RBUF_GetCompressInfo
.L465:
	.align	2
.L464:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	AIP_GetCompressInfo, .-AIP_GetCompressInfo
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stAipRm, %object
	.size	g_stAipRm, 64
g_stAipRm:
	.space	64
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
