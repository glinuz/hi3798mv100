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
	.global	__aeabi_uidiv
	.global	__aeabi_idiv
	.text
	.align	2
	.global	PQ_MNG_ALG_UpdateDMCfg
	.type	PQ_MNG_ALG_UpdateDMCfg, %function
PQ_MNG_ALG_UpdateDMCfg:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r2, #0
	mov	r5, r0
	mov	r4, r1
	beq	.L22
	ldr	r3, [r2, #8]
	mov	r1, #6
	str	r1, [r0, #140]
	cmp	r3, #720
	mov	r3, #1
	str	r3, [r4, #4]
	ldrls	r7, .L43
	ldrhi	r6, .L43+4
	ldr	r1, [r2, #8]
	addls	r8, r7, #408
	addhi	r8, r6, #408
	subhi	r7, r6, #424
	movls	r6, r7
	cmp	r1, #0
	beq	.L4
	ldr	r3, [r5, #148]
	ldr	r0, [r0, #144]
	mul	r0, r3, r0
	mov	r0, r0, asl #4
	bl	__aeabi_uidiv
	cmp	r0, #0
	moveq	r1, r0
	beq	.L4
	cmp	r0, #15
	movcc	r1, r0
	movcs	r1, #15
	uxth	r1, r1
.L4:
	cmp	r5, #0
	beq	.L26
	ldr	r2, .L43+8
	ldr	r3, [r2]
	cmp	r3, #31
	addls	r0, r3, #1
	movhi	r3, #0
	add	r3, r3, #2
	movhi	r0, #1
	str	r0, [r2]
	add	r2, r5, r3, lsl #2
	mov	r3, r2
	str	r1, [r3, #4]!
	ldr	r1, [r5, #140]
	str	r3, [r5, #152]
	cmp	r1, #0
	ldreqh	r0, [r2, #4]
	beq	.L10
	movle	ip, #0
	ble	.L9
	mov	r2, #0
	add	r0, r5, #12
	mov	ip, r2
.L12:
	ldr	lr, [r3], #-4
	add	r2, r2, #1
	cmp	r3, r0
	add	ip, ip, lr
	addcc	r3, r5, #136
	cmp	r2, r1
	bne	.L12
.L9:
	mov	r0, ip
	bl	__aeabi_idiv
	uxth	r0, r0
.L10:
	ldrh	r3, [r8]
	cmp	r3, r0
	ldrhib	r3, [r8, #8]
	bls	.L5
.L13:
	cmp	r3, #5
	mov	r1, #0
	str	r1, [r4, #8]
	movcs	r3, #5
	mov	r2, r3, asl #6
	add	r2, r2, r3, lsl #2
	add	r3, r6, r2
	ldrh	r2, [r6, r2]
	str	r2, [r4, #12]
	ldrh	r2, [r3, #2]
	str	r2, [r4, #16]
	ldrh	r2, [r3, #4]
	str	r2, [r4, #20]
	ldrh	r2, [r3, #6]
	str	r2, [r4, #24]
	ldrb	r2, [r3, #8]
	str	r2, [r4, #28]
	ldrb	r2, [r3, #9]
	str	r2, [r4, #32]
	ldrsh	r5, [r3, #10]
	str	r5, [r4, #36]
	ldrsh	ip, [r3, #12]
	str	ip, [r4, #40]
	ldrsh	lr, [r3, #14]
	str	lr, [r4, #44]
	ldrsh	r0, [r3, #16]
	str	r0, [r4, #48]
	ldrb	r2, [r3, #18]
	cmp	r2, #1
	str	r2, [r4, #52]
	beq	.L40
.L17:
	ldrh	r1, [r3, #20]
	cmp	r2, #1
	str	r1, [r4, #56]
	ldrh	r2, [r3, #22]
	str	r2, [r4, #60]
	ldrh	ip, [r3, #24]
	str	ip, [r4, #64]
	ldrh	r1, [r3, #26]
	str	r1, [r4, #68]
	ldrh	r0, [r3, #28]
	str	r0, [r4, #72]
	ldrh	r2, [r3, #30]
	str	r2, [r4, #76]
	beq	.L41
.L18:
	ldrh	r2, [r3, #32]
	str	r2, [r4, #80]
	ldrh	r2, [r3, #34]
	str	r2, [r4, #84]
	ldrb	r2, [r3, #36]
	str	r2, [r4, #88]
	ldrb	r2, [r3, #37]
	str	r2, [r4, #92]
	ldrb	r2, [r3, #38]
	str	r2, [r4, #96]
	ldrh	r2, [r3, #40]
	str	r2, [r4, #100]
	ldrh	r2, [r3, #42]
	str	r2, [r4, #104]
	ldrh	r2, [r3, #44]
	str	r2, [r4, #108]
	ldrh	r2, [r3, #46]
	str	r2, [r4, #112]
	ldrb	r2, [r3, #48]
	str	r2, [r4, #116]
	ldrsh	lr, [r3, #50]
	str	lr, [r4, #120]
	ldrsh	r0, [r3, #52]
	str	r0, [r4, #124]
	ldrsh	ip, [r3, #54]
	str	ip, [r4, #128]
	ldrsh	r1, [r3, #56]
	str	r1, [r4, #132]
	ldrb	r2, [r3, #58]
	cmp	r2, #1
	beq	.L42
.L38:
	str	r2, [r4, #136]
	mov	ip, #24
	ldrh	r5, [r3, #60]
	mov	r2, #2
	ldr	lr, [r7, #848]
	mov	r1, #4
	mov	r0, #0
	str	r5, [r4, #140]
	ldrh	r5, [r3, #62]
	str	r5, [r4, #144]
	ldrh	r5, [r3, #64]
	str	r5, [r4, #148]
	ldrh	r3, [r3, #66]
	str	lr, [r4, #192]
	str	ip, [r4, #156]
	str	r3, [r4, #152]
	str	r2, [r4, #160]
	str	r2, [r4, #164]
	str	r1, [r4, #168]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L26:
	movw	r0, #65535
.L5:
	ldrh	r3, [r8, #2]
	cmp	r3, r0
	ldrhib	r3, [r8, #9]
	bhi	.L13
	ldrh	r3, [r8, #4]
	cmp	r3, r0
	ldrhib	r3, [r8, #10]
	bhi	.L13
	ldrh	r3, [r8, #6]
	cmp	r3, r0
	ldrhib	r3, [r8, #11]
	ldrlsb	r3, [r8, #12]
	b	.L13
.L40:
	cmp	ip, r0
	cmpgt	r5, lr
	strle	r1, [r4, #52]
	movle	r2, r1
	b	.L17
.L41:
	cmp	r1, r2
	cmpls	ip, r0
	movhi	r2, #0
	strhi	r2, [r4, #52]
	b	.L18
.L42:
	cmp	r0, r1
	cmpgt	lr, ip
	movle	r2, #0
	b	.L38
.L22:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L44:
	.align	2
.L43:
	.word	.LANCHOR0
	.word	.LANCHOR0+424
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_UpdateDMCfg, .-PQ_MNG_ALG_UpdateDMCfg
	.global	DMParaHD4k
	.global	DMParaSD
	.global	QPLimitFlagLutHD4k
	.global	QPLimitFlagLutSD
	.global	g_u32DmStr
	.data
	.align	2
.LANCHOR0 = . + 0
	.type	DMParaSD, %object
	.size	DMParaSD, 408
DMParaSD:
	.short	15
	.short	9
	.short	35
	.short	140
	.byte	4
	.byte	5
	.short	20
	.short	80
	.short	1
	.short	4
	.byte	1
	.space	1
	.short	2
	.short	8
	.short	3
	.short	12
	.short	6
	.short	24
	.short	10
	.short	40
	.byte	6
	.byte	6
	.byte	6
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	20
	.short	18
	.short	40
	.short	160
	.byte	4
	.byte	5
	.short	25
	.short	100
	.short	0
	.short	0
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	18
	.short	72
	.byte	7
	.byte	8
	.byte	7
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	20
	.short	18
	.short	45
	.short	180
	.byte	4
	.byte	5
	.short	25
	.short	100
	.short	-1
	.short	-4
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	20
	.short	80
	.byte	7
	.byte	8
	.byte	7
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	25
	.short	18
	.short	50
	.short	200
	.byte	4
	.byte	4
	.short	30
	.short	120
	.short	-2
	.short	-8
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	25
	.short	100
	.byte	8
	.byte	8
	.byte	8
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	25
	.short	18
	.short	50
	.short	200
	.byte	4
	.byte	4
	.short	30
	.short	120
	.short	-3
	.short	-12
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	25
	.short	100
	.byte	8
	.byte	8
	.byte	8
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	18
	.short	25
	.short	45
	.short	180
	.byte	4
	.byte	4
	.short	25
	.short	100
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	2
	.short	8
	.short	3
	.short	12
	.short	6
	.short	24
	.short	8
	.short	32
	.byte	8
	.byte	8
	.byte	6
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.type	QPLimitFlagLutSD, %object
	.size	QPLimitFlagLutSD, 14
QPLimitFlagLutSD:
	.short	2
	.short	6
	.short	10
	.short	14
	.byte	1
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.space	1
	.space	2
	.type	DMParaHD4k, %object
	.size	DMParaHD4k, 408
DMParaHD4k:
	.short	15
	.short	9
	.short	35
	.short	140
	.byte	4
	.byte	4
	.short	20
	.short	80
	.short	1
	.short	4
	.byte	1
	.space	1
	.short	2
	.short	8
	.short	3
	.short	12
	.short	6
	.short	24
	.short	10
	.short	40
	.byte	8
	.byte	8
	.byte	6
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	20
	.short	18
	.short	40
	.short	160
	.byte	4
	.byte	4
	.short	25
	.short	100
	.short	0
	.short	0
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	18
	.short	72
	.byte	8
	.byte	8
	.byte	7
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	20
	.short	18
	.short	45
	.short	180
	.byte	4
	.byte	4
	.short	25
	.short	100
	.short	-1
	.short	-4
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	20
	.short	80
	.byte	8
	.byte	8
	.byte	7
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	25
	.short	18
	.short	50
	.short	200
	.byte	3
	.byte	3
	.short	30
	.short	120
	.short	-2
	.short	-8
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	25
	.short	100
	.byte	8
	.byte	8
	.byte	8
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	25
	.short	18
	.short	50
	.short	200
	.byte	3
	.byte	3
	.short	30
	.short	120
	.short	-3
	.short	-12
	.byte	1
	.space	1
	.short	3
	.short	12
	.short	2
	.short	8
	.short	5
	.short	20
	.short	25
	.short	100
	.byte	8
	.byte	8
	.byte	8
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.short	18
	.short	25
	.short	45
	.short	180
	.byte	4
	.byte	4
	.short	25
	.short	100
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	2
	.short	8
	.short	3
	.short	12
	.short	6
	.short	24
	.short	8
	.short	32
	.byte	8
	.byte	8
	.byte	6
	.space	1
	.short	2
	.short	8
	.short	0
	.short	0
	.byte	9
	.space	1
	.short	20
	.short	80
	.short	0
	.short	0
	.byte	0
	.space	1
	.short	30
	.short	120
	.short	0
	.short	0
	.type	QPLimitFlagLutHD4k, %object
	.size	QPLimitFlagLutHD4k, 14
QPLimitFlagLutHD4k:
	.short	1
	.short	6
	.short	10
	.short	14
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.space	1
	.space	2
	.type	g_u32DmStr, %object
	.size	g_u32DmStr, 4
g_u32DmStr:
	.word	8
	.bss
	.align	2
.LANCHOR1 = . + 0
	.type	i.4573, %object
	.size	i.4573, 4
i.4573:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
