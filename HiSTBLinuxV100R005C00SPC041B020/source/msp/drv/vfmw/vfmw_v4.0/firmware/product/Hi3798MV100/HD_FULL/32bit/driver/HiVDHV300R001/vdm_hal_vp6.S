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
	.file	"vdm_hal_vp6.c"
	.text
	.align	2
	.global	VP6HAL_V300R001_InitHal
	.type	VP6HAL_V300R001_InitHal, %function
VP6HAL_V300R001_InitHal:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6HAL_V300R001_InitHal, .-VP6HAL_V300R001_InitHal
	.align	2
	.global	VP6HAL_V300R001_CfgReg
	.type	VP6HAL_V300R001_CfgReg, %function
VP6HAL_V300R001_CfgReg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	cmp	r2, #1
	mov	r10, r2
	mov	r6, r0
	mov	r8, r1
	bhi	.L21
	cmp	r2, #0
	bgt	.L22
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L23
.L7:
	ldrh	r2, [r6, #52]
	movw	r5, #1208
	ldrh	r3, [r6, #54]
	mov	r9, #0
	mul	r5, r5, r10
	ldr	r4, .L24
	mul	r3, r3, r2
	mov	r1, r9
	mov	r2, #0
	bfi	r2, r9, #7, #1
	ldr	r7, .L24+4
	mov	r0, #3
	sub	r3, r3, #1
	ldr	ip, [r4, r5]
	bfi	r1, r3, #0, #20
	str	r1, [fp, #-48]
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	ldr	r1, .L24+8
	str	r2, [ip, #8]
	ldr	r3, [r7, #68]
	blx	r3
	mov	r3, #536870912
	str	r3, [fp, #-48]
	mov	r3, #10
	ldrh	r2, [fp, #-46]
	mov	r0, #3
	strb	r3, [fp, #-48]
	bfi	r2, r9, #0, #12
	ldr	r1, [r6, #188]
	ubfx	r3, r2, #8, #8
	strh	r2, [fp, #-46]	@ movhi
	orr	r3, r3, #32
	strb	r3, [fp, #-45]
	mov	r1, r1, lsr #6
	ldr	ip, [r6, #192]
	uxtb	r2, r3
	ldrh	r3, [fp, #-48]
	bfi	r2, ip, #6, #1
	ldr	ip, [r4, r5]
	bfi	r3, r1, #4, #10
	strh	r3, [fp, #-48]	@ movhi
	ubfx	r3, r3, #8, #8
	bfi	r2, r9, #7, #1
	orr	r3, r3, #192
	strb	r2, [fp, #-45]
	strb	r3, [fp, #-47]
	ldr	r2, [fp, #-48]
	ldr	r1, .L24+12
	str	r2, [ip, #12]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, r5]
	ldr	r2, [r8, #48]
	mov	r0, #3
	ldr	r1, .L24+16
	bic	r2, r2, #15
	str	r2, [r3, #16]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, r5]
	ldr	r2, [r8, #32]
	mov	r0, #3
	ldr	r1, .L24+20
	bic	r2, r2, #15
	str	r2, [r3, #20]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, r5]
	ldr	r2, [r6, #56]
	mov	r0, #3
	ldr	r1, .L24+24
	str	r2, [r3, #24]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, r5]
	ldr	r2, [r8, #1156]
	mov	r0, #3
	ldr	r1, .L24+28
	bic	r2, r2, #15
	str	r2, [r3, #128]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r2, [r8, #1160]
	str	r9, [fp, #-48]
	mov	r0, #3
	ldr	r3, [r4, r5]
	strh	r2, [fp, #-48]	@ movhi
	ldr	r2, [fp, #-48]
	ldr	r1, .L24+32
	str	r2, [r3, #132]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r0, .L24+36
	ldrh	r1, [r6, #52]
	movw	r3, #30480
	movt	r3, 1
	movw	r2, #30480
	cmp	r1, #256
	ldr	r0, [r0]
	movls	r1, r3
	movhi	r1, r2
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	ldr	r2, [r4, r5]
	movw	r3, #3075
	ldr	r1, .L24+40
	movt	r3, 48
	mov	r0, #3
	str	r3, [r2, #60]
	ldr	r2, [r4, r5]
	str	r3, [r2, #64]
	ldr	r2, [r4, r5]
	str	r3, [r2, #68]
	ldr	r2, [r4, r5]
	str	r3, [r2, #72]
	ldr	r2, [r4, r5]
	str	r3, [r2, #76]
	ldr	r2, [r4, r5]
	str	r3, [r2, #80]
	ldr	r2, [r4, r5]
	str	r3, [r2, #84]
	ldr	r3, [r4, r5]
	ldr	r2, [r6, #172]
	bic	r2, r2, #15
	str	r2, [r3, #96]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, r5]
	ldr	r2, [r6, #188]
	mov	r0, #3
	ldr	r1, .L24+44
	str	r2, [r3, #100]
	ldr	r3, [r7, #68]
	blx	r3
	ldrh	r3, [r6, #52]
	cmp	r3, r9
	mov	r3, r3, asl #4
	ble	.L13
	cmp	r3, #2048
	movle	r9, #16
	bgt	.L13
.L10:
	ldrh	lr, [r6, #54]
	movw	r5, #1208
	ldr	r3, [r6, #188]
	mov	r0, #3
	add	r2, lr, #1
	ldr	r1, .L24+48
	mov	lr, lr, asl #4
	mov	r8, #0
	adds	ip, lr, #31
	mov	r2, r2, asr #1
	addmi	ip, lr, #62
	mul	r5, r5, r10
	mov	ip, ip, asr #5
	mul	r2, r3, r2
	mov	ip, ip, asl #4
	mla	ip, r9, ip, r2
	ldr	r3, [r4, r5]
	mov	r2, ip, asl #1
	str	r2, [r3, #104]
	ldr	r3, [r7, #68]
	blx	r3
	ldrh	ip, [r6, #54]
	ldr	lr, [r4, r5]
	mov	r2, r8
	ldr	r1, .L24+52
	mov	r0, #3
	mov	ip, ip, asl #4
	adds	r3, ip, #31
	addmi	r3, ip, #62
	bic	r3, r3, #31
	mul	r9, r9, r3
	str	r9, [lr, #108]
	ldr	r3, [r4, r5]
	str	r8, [r3, #152]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, r5]
	mov	r0, r8
	mvn	r2, #0
	str	r2, [r3, #32]
.L19:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L13:
	sub	r2, r3, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r9, #32
	bcc	.L10
	sub	r2, r3, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r9, #48
	bcc	.L10
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	r9, #16
	movcc	r9, #64
	b	.L10
.L23:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L8
	str	r3, [r8]
	b	.L7
.L22:
	ldr	r1, .L24+4
	mov	r0, #0
	mov	r3, r2
	str	r0, [sp]
	ldr	r2, .L24+56
	ldr	ip, [r1, #68]
	ldr	r1, .L24+60
	blx	ip
	mvn	r0, #0
	b	.L19
.L21:
	ldr	r3, .L24+4
	mov	r0, #0
	ldr	r1, .L24+64
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L19
.L8:
	ldr	r3, .L24+4
	ldr	r1, .L24+68
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L19
.L25:
	.align	2
.L24:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	s_RegPhyBaseAddr
	.word	.LC12
	.word	.LC13
	.word	.LC3
	.word	.LC4
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC0
	.word	.LC2
	UNWIND(.fnend)
	.size	VP6HAL_V300R001_CfgReg, .-VP6HAL_V300R001_CfgReg
	.align	2
	.global	VP6HAL_V300R001_CfgDnMsg
	.type	VP6HAL_V300R001_CfgDnMsg, %function
VP6HAL_V300R001_CfgDnMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	mov	r4, r0
	ldr	r0, [r1, #48]
	mov	r9, r1
	bl	MEM_Phy2Vir
	subs	r6, r0, #0
	beq	.L39
	ldr	r2, [r4, #60]
	mov	r7, #0
	mov	r3, #0
	str	r7, [fp, #-48]
	bfi	r3, r2, #0, #1
	strb	r3, [fp, #-48]
	ldr	r5, .L47
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+4
	ldr	r8, .L47
	str	r2, [r6]
	ldr	r3, [r5, #68]
	blx	r3
	ldrh	r0, [r4, #52]
	ldrh	r1, [r4, #54]
	mov	r2, #0	@ movhi
	sub	r0, r0, #1
	mov	r3, r2	@ movhi
	sub	r1, r1, #1
	bfi	r2, r0, #0, #9
	bfi	r3, r1, #0, #9
	strh	r2, [fp, #-48]	@ movhi
	strh	r3, [fp, #-46]	@ movhi
	mov	r2, r2, lsr #8
	mov	r3, r3, lsr #8
	bfi	r2, r7, #1, #7
	bfi	r3, r7, #1, #7
	strb	r2, [fp, #-47]
	strb	r3, [fp, #-45]
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+8
	str	r2, [r6, #4]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r4, #100]
	ldr	ip, [r4, #104]
	mov	r0, #4
	ldr	r1, [r4, #108]
	and	r3, r3, #31
	ldr	r2, [r4, #72]
	bfi	r3, ip, #5, #2
	ldr	ip, [r4, #68]
	mov	r1, r1, lsr #1
	and	r2, r2, #1
	bfi	r3, ip, #7, #1
	str	r7, [fp, #-48]
	bfi	r2, r1, #1, #1
	strb	r3, [fp, #-48]
	strb	r2, [fp, #-47]
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+12
	bfi	r2, r7, #10, #22
	str	r2, [fp, #-48]
	str	r2, [r6, #8]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r4, #96]
	ldr	r2, [r4, #92]
	mov	r0, #4
	and	r3, r3, #31
	str	r7, [fp, #-48]
	bfi	r3, r2, #5, #3
	strb	r3, [fp, #-48]
	ldr	r3, [fp, #-48]
	ldr	r1, [r4, #88]
	ldr	r2, [r4, #84]
	bfi	r3, r1, #8, #10
	str	r3, [fp, #-48]
	ldrb	r3, [fp, #-46]	@ zero_extendqisi2
	ldr	r1, .L47+16
	bfi	r3, r2, #2, #2
	strb	r3, [fp, #-46]
	ldrh	r3, [fp, #-46]
	bfi	r3, r7, #4, #12
	strh	r3, [fp, #-46]	@ movhi
	ldr	r2, [fp, #-48]
	str	r2, [r6, #12]
	ldr	r3, [r5, #68]
	blx	r3
	ldrb	r2, [r4]	@ zero_extendqisi2
	str	r7, [fp, #-48]
	mov	r3, #0
	ldr	r1, [r4, #76]
	bfi	r3, r2, #0, #4
	strb	r3, [fp, #-48]
	mov	r0, #4
	ldrh	r3, [fp, #-48]
	ldr	r2, [r4, #80]
	bfi	r3, r1, #4, #8
	strh	r3, [fp, #-48]	@ movhi
	ldr	r3, [fp, #-48]
	ldr	r1, .L47+20
	bfi	r3, r2, #12, #9
	str	r3, [fp, #-48]
	mov	r3, r3, lsr #16
	bfi	r3, r7, #5, #11
	strh	r3, [fp, #-46]	@ movhi
	ldr	r2, [fp, #-48]
	str	r2, [r6, #16]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [r4, #160]
	ldr	r1, .L47+24
	mov	r0, #4
	str	r2, [r6, #32]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r1, [r4, #168]
	ldr	r2, [r4, #164]
	mov	r3, #0
	str	r7, [fp, #-48]
	bfi	r3, r1, #0, #4
	strb	r3, [fp, #-46]
	mov	r0, #4
	ldrh	r3, [fp, #-46]
	strb	r7, [fp, #-47]
	bfi	r3, r7, #4, #12
	strb	r2, [fp, #-48]
	strh	r3, [fp, #-46]	@ movhi
	mov	r7, #0
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+28
	str	r2, [r6, #36]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r4, #168]
	ldr	r2, [r4, #116]
	add	r3, r3, #8
	ldr	r0, [r4, #112]
	cmp	r3, r2
	ldr	r1, .L47+32
	addhi	r2, r2, #128
	add	r0, r3, r0
	rsbls	r2, r3, r2
	rsbhi	r2, r3, r2
	mov	r3, r7
	bfi	r3, r0, #0, #25
	str	r3, [fp, #-48]
	mov	r0, #4
	mov	r3, r3, lsr #24
	ldrhi	r10, [r4, #120]
	ldrls	r10, [r4, #120]
	bfi	r3, r2, #1, #7
	strb	r3, [fp, #-45]
	subhi	r10, r10, #16
	ldr	r2, [fp, #-48]
	str	r2, [r6, #64]
	ldr	r3, [r5, #68]
	blx	r3
	mov	r3, r7
	bfi	r3, r10, #0, #24
	str	r3, [fp, #-48]
	strb	r7, [fp, #-45]
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+36
	str	r2, [r6, #68]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r0, [r4, #120]
	ldr	r3, [r4, #56]
	add	r0, r0, r3
	bl	MEM_Phy2Vir
	cmp	r0, r7
	beq	.L40
.L31:
	ldr	r1, [r4, #124]
	mov	r7, #0
	mov	r3, r7
	ldr	r2, [r4, #128]
	bfi	r3, r1, #0, #25
	str	r3, [fp, #-48]
	ldr	ip, [r5, #68]
	mov	r0, #4
	mov	r3, r3, lsr #24
	ldr	r1, .L47+40
	bfi	r3, r2, #1, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	ldr	r10, .L47
	str	r2, [r6, #72]
	blx	ip
	ldr	r2, [r4, #132]
	mov	r3, r7
	ldr	ip, [r5, #68]
	bfi	r3, r2, #0, #24
	str	r3, [fp, #-48]
	strb	r7, [fp, #-45]
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+44
	str	r2, [r6, #76]
	blx	ip
	ldr	r1, [r4, #136]
	mov	r3, r7
	ldr	r2, [r4, #140]
	bfi	r3, r1, #0, #25
	str	r3, [fp, #-48]
	ldr	ip, [r5, #68]
	mov	r0, #4
	mov	r3, r3, lsr #24
	ldr	r1, .L47+48
	bfi	r3, r2, #1, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r6, #80]
	blx	ip
	ldr	r2, [r4, #144]
	mov	r3, r7
	ldr	ip, [r5, #68]
	bfi	r3, r2, #0, #24
	str	r3, [fp, #-48]
	strb	r7, [fp, #-45]
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+52
	str	r2, [r6, #84]
	blx	ip
	ldr	r1, [r4, #148]
	mov	r3, r7
	ldr	r2, [r4, #152]
	bfi	r3, r1, #0, #25
	str	r3, [fp, #-48]
	ldr	ip, [r5, #68]
	mov	r0, #4
	mov	r3, r3, lsr #24
	ldr	r1, .L47+56
	bfi	r3, r2, #1, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r6, #88]
	blx	ip
	ldr	r2, [r4, #156]
	mov	r3, r7
	ldr	ip, [r5, #68]
	bfi	r3, r2, #0, #24
	str	r3, [fp, #-48]
	strb	r7, [fp, #-45]
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L47+60
	str	r2, [r6, #92]
	blx	ip
	ldr	r2, [r4, #172]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L47+64
	str	r2, [r6, #128]
	blx	r3
	ldr	r2, [r4, #176]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L47+68
	str	r2, [r6, #136]
	blx	r3
	ldr	r2, [r4, #180]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L47+72
	str	r2, [r6, #140]
	blx	r3
	ldr	r2, [r9, #1136]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L47+76
	str	r2, [r6, #144]
	blx	r3
	ldr	r2, [r9, #1140]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L47+80
	str	r2, [r6, #148]
	blx	r3
	ldr	r2, [r9, #1144]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L47+84
	str	r2, [r6, #152]
	blx	r3
	ldr	r8, [r9, #1176]
	ldr	r1, .L47+88
	mov	r0, #4
	bic	r8, r8, #15
	ldr	r3, [r5, #68]
	str	r8, [r6, #156]
	mov	r2, r8
	str	r8, [fp, #-48]
	blx	r3
	mov	r0, r8
	bl	MEM_Phy2Vir
	mov	r6, r0
	ldr	r0, [r9, #1176]
	bl	MEM_Phy2Vir
	cmp	r0, r7
	cmpne	r6, r7
	moveq	r1, #1
	movne	r1, #0
	beq	.L41
	ldr	r3, [r10, #48]
	mov	r2, #4096
	mov	r0, r6
	blx	r3
	ldr	r3, [r10, #52]
	mov	r2, #64
	ldr	r1, [r4, #4]
	mov	r0, r6
	blx	r3
	ldr	r3, [r10, #52]
	mov	r2, #640
	ldr	r1, [r4, #8]
	add	r0, r6, #64
	blx	r3
	ldr	r3, [r10, #52]
	add	r0, r6, #704
	mov	r2, #64
	ldr	r1, [r4, #12]
	blx	r3
	ldr	r3, [r4, #72]
	mov	r0, r8
	cmp	r3, #0
	beq	.L42
	bl	MEM_Phy2Vir
	mov	r6, r0
	adds	r0, r0, #768
	beq	.L43
	ldr	r3, [r10, #52]
	mov	r2, #1536
	ldr	r1, [r4, #44]
	blx	r3
	ldr	r3, [r10, #52]
	mov	r2, #128
	ldr	r1, [r4, #40]
	add	r0, r6, #2304
	blx	r3
	ldr	r3, [r10, #52]
	add	r0, r6, #2432
	mov	r2, #128
	ldr	r1, [r4, #48]
	blx	r3
.L35:
	mov	r0, r8
	bl	MEM_Phy2Vir
	mov	r6, r0
	adds	r0, r0, #2560
	beq	.L44
	ldr	r3, [r5, #52]
	mov	r2, #64
	ldr	r1, [r4, #16]
	blx	r3
	ldr	r3, [r5, #52]
	mov	r2, #64
	ldr	r1, [r4, #20]
	add	r0, r6, #2624
	ldr	r7, .L47
	blx	r3
	ldr	r3, [r5, #52]
	mov	r2, #960
	ldr	r1, [r4, #24]
	add	r0, r6, #2688
	blx	r3
	mov	r0, r8
	bl	MEM_Phy2Vir
	adds	r0, r0, #2816
	beq	.L45
	ldr	r3, [r7, #52]
	mov	r2, #960
	ldr	r1, [r4, #24]
	blx	r3
	mov	r0, #0
.L28:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L42:
	bl	MEM_Phy2Vir
	mov	r6, r0
	adds	r0, r0, #768
	beq	.L46
	ldr	r3, [r10, #52]
	mov	r2, #96
	ldr	r1, [r4, #28]
	blx	r3
	ldr	r3, [r10, #52]
	ldr	r1, [r4, #36]
	mov	r2, #32
	add	r0, r6, #864
	blx	r3
	ldr	r3, [r10, #52]
	add	r0, r6, #896
	mov	r2, #576
	ldr	r1, [r4, #32]
	blx	r3
	b	.L35
.L40:
	ldr	r3, [r8, #68]
	mov	r2, #496
	ldr	r1, .L47+92
	mov	r0, #1
	blx	r3
	b	.L31
.L41:
	str	r6, [sp]
	mov	r3, r0
	ldr	ip, [r10, #68]
	mov	r0, r7
	ldr	r2, .L47+96
	ldr	r1, .L47+100
	blx	ip
	mvn	r0, #0
	b	.L28
.L39:
	ldr	r1, .L47
	ldr	r3, .L47+104
	ldr	r2, .L47+96
	ldr	ip, [r1, #68]
	ldr	r1, .L47+108
	blx	ip
	mvn	r0, #0
	b	.L28
.L45:
	ldr	r3, [r7, #68]
	movw	r2, #706
	ldr	r1, .L47+112
	blx	r3
	mvn	r0, #0
	b	.L28
.L44:
	ldr	r3, [r5, #68]
	movw	r2, #685
	ldr	r1, .L47+112
	blx	r3
	mvn	r0, #0
	b	.L28
.L43:
	ldr	r3, [r10, #68]
	movw	r2, #661
	ldr	r1, .L47+112
	blx	r3
	mvn	r0, #0
	b	.L28
.L46:
	ldr	r3, [r10, #68]
	movw	r2, #635
	ldr	r1, .L47+112
	blx	r3
	mvn	r0, #0
	b	.L28
.L48:
	.align	2
.L47:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC25
	.word	.LANCHOR0+24
	.word	.LC39
	.word	.LC14
	.word	.LC15
	.word	.LC40
	UNWIND(.fnend)
	.size	VP6HAL_V300R001_CfgDnMsg, .-VP6HAL_V300R001_CfgDnMsg
	.align	2
	.global	VP6HAL_V300R001_StartDec
	.type	VP6HAL_V300R001_StartDec, %function
VP6HAL_V300R001_StartDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	movw	r5, #1208
	mul	r5, r5, r1
	ldr	r7, .L65
	cmp	r1, #1
	mov	r4, r1
	mov	r6, r0
	add	r8, r5, r7
	bhi	.L60
	cmp	r1, #0
	bgt	.L61
	ldrh	r3, [r0, #52]
	sub	r3, r3, #1
	cmp	r3, #512
	bcs	.L62
	ldrh	r3, [r0, #54]
	sub	r3, r3, #1
	cmp	r3, #512
	bcs	.L63
	ldr	r3, [r5, r7]
	cmp	r3, #0
	beq	.L64
.L56:
	mov	r2, r4
	mov	r1, r8
	mov	r0, r6
	bl	VP6HAL_V300R001_CfgReg
	mov	r2, r4
	mov	r1, r8
	mov	r0, r6
	bl	VP6HAL_V300R001_CfgDnMsg
	mov	r0, #0
.L52:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L64:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L57
	str	r3, [r5, r7]
	b	.L56
.L61:
	ldr	r1, .L65+4
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L65+8
	ldr	ip, [r1, #68]
	ldr	r1, .L65+12
	blx	ip
	mvn	r0, #0
	b	.L52
.L60:
	ldr	r3, .L65+4
	mov	r0, #0
	ldr	r1, .L65+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L52
.L62:
	ldr	r1, .L65+4
	mov	r0, #0
	ldr	r3, .L65+20
.L59:
	ldr	ip, [r1, #68]
	ldr	r2, .L65+8
	ldr	r1, .L65+24
	blx	ip
	mvn	r0, #0
	b	.L52
.L63:
	ldr	r1, .L65+4
	mov	r0, #0
	ldr	r3, .L65+28
	b	.L59
.L57:
	ldr	r3, .L65+4
	ldr	r1, .L65+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L52
.L66:
	.align	2
.L65:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+52
	.word	.LC1
	.word	.LC41
	.word	.LC42
	.word	.LC15
	.word	.LC43
	.word	.LC2
	UNWIND(.fnend)
	.size	VP6HAL_V300R001_StartDec, .-VP6HAL_V300R001_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.8002, %object
	.size	__func__.8002, 23
__func__.8002:
	.ascii	"VP6HAL_V300R001_CfgReg\000"
	.space	1
	.type	__func__.8017, %object
	.size	__func__.8017, 25
__func__.8017:
	.ascii	"VP6HAL_V300R001_CfgDnMsg\000"
	.space	3
	.type	__func__.7987, %object
	.size	__func__.7987, 25
__func__.7987:
	.ascii	"VP6HAL_V300R001_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"VdhId is wrong! MP2HAL_V200R003_CfgReg\012\000" )
.LC1:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC2:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC3:
	ASCII(.ascii	"UVOFFSET_1D = 0x%x\012\000" )
.LC4:
	ASCII(.ascii	"FF_APT_EN = 0x%x\012\000" )
	.space	2
.LC5:
	ASCII(.ascii	"BASIC_CFG0 = 0x%x\012\000" )
	.space	1
.LC6:
	ASCII(.ascii	"BASIC_CFG1 = 0x%x\012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"AVM_ADDR = 0x%x\012\000" )
	.space	3
.LC8:
	ASCII(.ascii	"VAM_ADDR = 0x%x\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"STREAM_BASE_ADDR = 0x%x\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"PPFD_V300R001_BUF_ADDR = 0x%x\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"PPFD_V200R003_BUF_LEN = 0x%x\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"YSTADDR_1D = 0x%x\012\000" )
	.space	1
.LC13:
	ASCII(.ascii	"YSTRIDE_1D = 0x%x\012\000" )
	.space	1
.LC14:
	ASCII(.ascii	"can not map down msg virtual address!\000" )
	.space	2
.LC15:
	ASCII(.ascii	"%s: %s\012\000" )
.LC16:
	ASCII(.ascii	"D0 = 0x%x\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"D1 = 0x%x\012\000" )
	.space	1
.LC18:
	ASCII(.ascii	"D2 = 0x%x\012\000" )
	.space	1
.LC19:
	ASCII(.ascii	"D3 = 0x%x\012\000" )
	.space	1
.LC20:
	ASCII(.ascii	"D4 = 0x%x\012\000" )
	.space	1
.LC21:
	ASCII(.ascii	"D8 = 0x%x\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"D9 = 0x%x\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"D16 = 0x%x\012\000" )
.LC24:
	ASCII(.ascii	"D17 = 0x%x\012\000" )
.LC25:
	ASCII(.ascii	"LINE:%d NULL == tmpAddr.\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"D18 = 0x%x\012\000" )
.LC27:
	ASCII(.ascii	"D19 = 0x%x\012\000" )
.LC28:
	ASCII(.ascii	"D20 = 0x%x\012\000" )
.LC29:
	ASCII(.ascii	"D21 = 0x%x\012\000" )
.LC30:
	ASCII(.ascii	"D22 = 0x%x\012\000" )
.LC31:
	ASCII(.ascii	"D23 = 0x%x\012\000" )
.LC32:
	ASCII(.ascii	"D32 = 0x%x\012\000" )
.LC33:
	ASCII(.ascii	"D34 = 0x%x\012\000" )
.LC34:
	ASCII(.ascii	"D35 = 0x%x\012\000" )
.LC35:
	ASCII(.ascii	"D36 = 0x%x\012\000" )
.LC36:
	ASCII(.ascii	"D37 = 0x%x\012\000" )
.LC37:
	ASCII(.ascii	"D38 = 0x%x\012\000" )
.LC38:
	ASCII(.ascii	"D39 = 0x%x\012\000" )
.LC39:
	ASCII(.ascii	"%s: tmpAddr(%p) = NULL / TabBaseAddr(%p) = NULL\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC40:
	ASCII(.ascii	"line: %d  NULL == TabBaseAddr.\012\000" )
.LC41:
	ASCII(.ascii	"VdhId is wrong! MP2HAL_V200R003_StartDec\012\000" )
	.space	2
.LC42:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC43:
	ASCII(.ascii	"picture height out of range\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
