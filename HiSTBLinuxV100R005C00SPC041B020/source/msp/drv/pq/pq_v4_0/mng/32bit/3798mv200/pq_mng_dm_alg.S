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
	.file	"pq_mng_dm_alg.c"
	.global	__aeabi_idiv
	.text
	.align	2
	.global	DM_Blend
	.type	DM_Blend, %function
DM_Blend:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	rsb	ip, r2, r3
	add	lr, r3, r3, lsr #31
	mul	ip, r1, ip
	mov	r1, r3
	mla	r0, r2, r0, ip
	add	r0, r0, lr, asr #1
	bl	__aeabi_idiv
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	DM_Blend, .-DM_Blend
	.align	2
	.global	PQ_MNG_ALG_UpdateDMCfg
	.type	PQ_MNG_ALG_UpdateDMCfg, %function
PQ_MNG_ALG_UpdateDMCfg:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #160]
	mov	r5, r0
	mov	r4, r1
	add	r6, r0, #180
	cmp	r3, #720
	bgt	.L3
	ldrb	r2, [r0, #165]
	cmp	r2, #1
	beq	.L29
.L3:
	ldrb	r2, [r5, #164]
.L13:
	mov	r3, #44
	smulbb	r3, r2, r3
	add	r5, r6, r3
	ldrh	r3, [r6, r3]
	strh	r3, [r4, #82]
	ldrh	r3, [r5, #2]
	strh	r3, [r4, #84]
	ldrh	r3, [r5, #4]
	strh	r3, [r4, #92]
	ldrh	r3, [r5, #6]
	strh	r3, [r4, #94]
	ldrb	r3, [r5, #8]
	strb	r3, [r4, #110]
	ldrb	r3, [r5, #9]
	strb	r3, [r4, #111]
	ldrh	ip, [r5, #10]
	strh	ip, [r4, #100]
	ldrh	r1, [r5, #12]
	strh	r1, [r4, #102]
	ldrh	r0, [r5, #14]
	strh	r0, [r4, #104]
	ldrh	r2, [r5, #16]
	strh	r2, [r4, #106]
	ldrb	r3, [r5, #20]
	cmp	r3, #1
	beq	.L30
.L27:
	strb	r3, [r4, #108]
	mov	r2, #62
	ldrb	r3, [r5, #19]
	add	r0, r4, #116
	strb	r3, [r4, #109]
	ldrb	r3, [r5, #18]
	strb	r3, [r4, #114]
	ldr	r1, [r5, #40]
	bl	memcpy
	ldrh	r3, [r5, #22]
	mov	r0, #0
	strh	r3, [r4, #76]
	ldrh	r3, [r5, #24]
	strh	r3, [r4, #78]
	ldrb	r3, [r5, #26]
	strb	r3, [r4, #112]
	ldrb	r3, [r5, #27]
	strb	r3, [r4, #113]
	ldrb	r3, [r5, #28]
	strb	r3, [r4, #86]
	ldrh	r3, [r5, #30]
	strh	r3, [r4, #88]
	ldrh	r3, [r5, #32]
	strh	r3, [r4, #90]
	ldrh	r3, [r5, #34]
	strh	r3, [r4, #96]
	ldrh	r3, [r5, #36]
	strh	r3, [r4, #98]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L29:
	cmp	r3, #0
	movgt	r0, #15
	movle	r0, #0
	cmp	r5, #0
	beq	.L21
	ldr	r1, .L31
	ldr	r3, [r1]
	cmp	r3, #31
	addls	r2, r3, #1
	str	r2, [r1]
	movhi	r3, #0
	add	r3, r3, #2
	add	r2, r5, r3, lsl #2
	mov	r3, r2
	str	r0, [r3, #4]!
	ldr	r1, [r5, #140]
	str	r3, [r5, #152]
	cmp	r1, #0
	ldreqh	r0, [r2, #4]
	beq	.L10
	movle	r0, #0
	ble	.L9
	mov	r2, #0
	add	lr, r5, #12
	mov	r0, r2
.L12:
	ldr	ip, [r3], #-4
	add	r2, r2, #1
	cmp	r3, lr
	add	r0, r0, ip
	addcc	r3, r5, #136
	cmp	r2, r1
	bne	.L12
.L9:
	bl	__aeabi_idiv
	uxth	r0, r0
.L10:
	ldrh	r3, [r5, #166]
	cmp	r3, r0
	ldrhib	r2, [r5, #174]
	bhi	.L13
.L5:
	ldrh	r3, [r5, #168]
	cmp	r3, r0
	ldrhib	r2, [r5, #175]
	bhi	.L13
	ldrh	r3, [r5, #170]
	cmp	r3, r0
	ldrhib	r2, [r5, #176]
	bhi	.L13
	ldrh	r3, [r5, #172]
	cmp	r3, r0
	ldrhib	r2, [r5, #177]
	ldrlsb	r2, [r5, #178]
	b	.L13
.L30:
	sxth	ip, ip
	sxth	r0, r0
	sxth	r1, r1
	sxth	r2, r2
	cmp	r1, r2
	cmpgt	ip, r0
	movle	r3, #0
	b	.L27
.L21:
	movw	r0, #65535
	b	.L5
.L32:
	.align	2
.L31:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_UpdateDMCfg, .-PQ_MNG_ALG_UpdateDMCfg
	.global	g_u32DmStr
	.data
	.align	2
	.type	g_u32DmStr, %object
	.size	g_u32DmStr, 4
g_u32DmStr:
	.word	8
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	i.4666, %object
	.size	i.4666, 4
i.4666:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
