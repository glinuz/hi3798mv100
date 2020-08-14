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
	.file	"vdm_hal_vp8.c"
	.text
	.align	2
	.global	VP8HAL_V400R003_InitHal
	.type	VP8HAL_V400R003_InitHal, %function
VP8HAL_V400R003_InitHal:
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
	.size	VP8HAL_V400R003_InitHal, .-VP8HAL_V400R003_InitHal
	.align	2
	.global	VP8HAL_V400R003_CfgReg
	.type	VP8HAL_V400R003_CfgReg, %function
VP8HAL_V400R003_CfgReg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	cmp	r2, #0
	mov	r5, r0
	mov	r6, r1
	mov	r0, #0
	mov	r4, r3
	str	r0, [fp, #-32]
	beq	.L4
	cmp	r2, #1
	bne	.L12
	mov	r3, r2
	str	r0, [sp]
	ldr	r2, .L14
	ldr	r1, .L14+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L6
.L4:
	ldr	r3, [r1]
	cmp	r3, #0
	beq	.L13
.L7:
	ldr	lr, [r5, #2788]
	mov	r3, r4
	ldr	r1, [r5, #2784]
	mov	r2, #0
	ldr	ip, [fp, #-32]
	mov	r0, #8
	mul	r1, r1, lr
	sub	r1, r1, #1
	bfi	ip, r1, #0, #20
	str	ip, [fp, #-32]
	mov	ip, ip, lsr #24
	and	ip, ip, #191
	bfc	ip, #7, #1
	strb	ip, [fp, #-29]
	ldr	r7, [fp, #-32]
	mov	r1, r7
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L14+8
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r2, [r5, #3144]
	mov	r1, #0
	mov	r3, #8192	@ movhi
	strh	r3, [fp, #-30]	@ movhi
	mov	r3, r1
	bfi	r3, r2, #4, #1
	strb	r1, [fp, #-29]
	uxtb	r2, r1
	and	r3, r3, #16
	ldr	r1, [r5, #3140]
	orr	r3, r3, #64
	mov	r0, #12
	strb	r0, [fp, #-32]
	bfi	r2, r1, #6, #1
	mvn	r3, r3, asl #25
	mvn	r3, r3, lsr #25
	strb	r3, [fp, #-31]
	mov	r3, r2
	bfc	r3, #7, #1
	strb	r3, [fp, #-29]
	mov	r2, #0
	ldr	r7, [fp, #-32]
	mov	r3, r4
	mov	r1, r7
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L14+12
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r7, [r6, #56]
	mov	r3, r4
	mov	r2, #0
	bic	r7, r7, #15
	mov	r0, #16
	mov	r1, r7
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L14+16
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r7, [r6, #40]
	mov	r3, r4
	mov	r2, #0
	bic	r7, r7, #15
	mov	r0, #20
	mov	r1, r7
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L14+20
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r7, [r5, #2812]
	mov	r3, r4
	mov	r2, #0
	mov	r0, #24
	mov	r1, r7
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L14+24
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r7, [r6, #1164]
	mov	r3, r4
	mov	r2, #0
	bic	r7, r7, #15
	mov	r0, #128
	mov	r1, r7
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L14+28
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r6, #1168]
	mov	r3, r4
	mov	r2, #0
	mov	r0, #132
	mov	r1, r6
	str	r6, [fp, #-32]
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L14+32
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r1, [r5, #2784]
	mov	r3, r4
	mov	r2, #0
	cmp	r1, #256
	mov	r0, #4
	movhi	r1, #0
	movls	r1, #1
	bl	SCD_ConfigReg
	ldr	r6, [r5, #3116]
	mov	r3, r4
	mov	r2, #0
	bic	r6, r6, #15
	mov	r0, #96
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L14+36
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r5, #2792]
	mov	r3, r4
	mov	r2, #0
	mov	r0, #100
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L14+40
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r5, #3148]
	mov	r3, r4
	mov	r2, #0
	mov	r0, #104
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L14+44
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r4
	ldr	r1, [r5, #3156]
	mov	r2, #0
	mov	r0, #108
	bl	MFDE_ConfigReg
	mov	r0, #32
	mov	r3, r4
	mov	r2, #0
	mvn	r1, #0
	bl	MFDE_ConfigReg
	mov	r0, #0
.L6:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L12:
	ldr	r1, .L14+48
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L13:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r6]
	bne	.L7
.L8:
	ldr	r1, .L14+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L6
.L15:
	.align	2
.L14:
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC0
	.word	.LC2
	UNWIND(.fnend)
	.size	VP8HAL_V400R003_CfgReg, .-VP8HAL_V400R003_CfgReg
	.align	2
	.global	VP8HAL_V400R003_CfgDnMsg
	.type	VP8HAL_V400R003_CfgDnMsg, %function
VP8HAL_V400R003_CfgDnMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	mov	r4, r0
	ldr	r0, [r1, #56]
	mov	r9, r1
	bl	MEM_Phy2Vir
	subs	r8, r0, #0
	beq	.L24
	ldr	r3, [r4, #2796]
	mov	r5, #0
	ldr	r2, [r4, #2800]
	mov	r0, #4
	and	r3, r3, #1
	str	r5, [fp, #-40]
	bfi	r3, r2, #1, #2
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26
	str	r2, [r8]
	bl	dprint_vfmw
	ldr	r2, [r4, #2808]
	ldr	r3, [r4, #2804]
	mov	r0, #4
	ldr	r1, .L26+4
	rsb	r2, r3, r2, lsl #1
	add	r2, r2, #1
	str	r2, [r8, #4]
	bl	dprint_vfmw
	ldr	r2, [r4, #2784]
	ldr	r3, [r4, #2788]
	mov	r0, #0	@ movhi
	sub	r2, r2, #1
	mov	r1, r0	@ movhi
	sub	r3, r3, #1
	bfi	r0, r2, #0, #9
	bfi	r1, r3, #0, #9
	strh	r0, [fp, #-40]	@ movhi
	strh	r1, [fp, #-38]	@ movhi
	mov	r0, #4
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+8
	str	r2, [r8, #8]
	bl	dprint_vfmw
	ldrb	r2, [r4, #2752]	@ zero_extendqisi2
	ldrb	r3, [r4, #2753]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	r1, [r4, #2754]	@ zero_extendqisi2
	and	r3, r3, #1
	str	r5, [fp, #-40]
	bfi	r3, r1, #1, #2
	strb	r2, [fp, #-40]
	strb	r3, [fp, #-39]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+12
	str	r2, [r8, #12]
	bl	dprint_vfmw
	ldrb	r3, [r4, #2755]	@ zero_extendqisi2
	ldrb	r2, [r4, #2756]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	r1, [r4, #2757]	@ zero_extendqisi2
	and	r3, r3, #1
	bfi	r3, r2, #1, #1
	ldrb	r2, [r4, #2758]	@ zero_extendqisi2
	bfi	r3, r1, #2, #1
	str	r5, [fp, #-40]
	bfi	r3, r2, #3, #1
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+16
	str	r2, [r8, #16]
	bl	dprint_vfmw
	ldrb	r3, [r4, #2760]	@ zero_extendqisi2
	ldrb	r2, [r4, #2761]	@ zero_extendqisi2
	mov	r0, #4
	and	r3, r3, #1
	str	r5, [fp, #-40]
	bfi	r3, r2, #1, #2
	ldrb	r2, [r4, #2762]	@ zero_extendqisi2
	strb	r3, [fp, #-40]
	ldrh	r3, [fp, #-40]
	ldrb	ip, [r4, #2763]	@ zero_extendqisi2
	bfi	r3, r2, #3, #6
	ldrb	r2, [r4, #2764]	@ zero_extendqisi2
	strh	r3, [fp, #-40]	@ movhi
	mov	r3, r3, lsr #8
	ldr	r1, .L26+20
	bfi	r3, ip, #1, #3
	bfi	r3, r2, #4, #3
	strb	r3, [fp, #-39]
	ldr	r2, [fp, #-40]
	str	r2, [r8, #20]
	bl	dprint_vfmw
	ldrb	r3, [r4, #2765]	@ zero_extendqisi2
	ldrb	r1, [r4, #2766]	@ zero_extendqisi2
	mov	r0, #4
	ldrb	r2, [r4, #2767]	@ zero_extendqisi2
	and	r3, r3, #1
	bfi	r3, r1, #1, #4
	str	r5, [fp, #-40]
	ldrb	ip, [r4, #2768]	@ zero_extendqisi2
	bfi	r3, r2, #5, #1
	strb	r3, [fp, #-40]
	ldrh	r1, [fp, #-40]
	ldrb	lr, [r4, #2769]	@ zero_extendqisi2
	bfi	r1, ip, #6, #4
	ldrb	r2, [fp, #-38]	@ zero_extendqisi2
	ldrb	ip, [r4, #2772]	@ zero_extendqisi2
	mov	r3, r1, lsr #8
	bfi	r2, ip, #0, #4
	bfi	r3, lr, #2, #1
	ldrb	ip, [r4, #2773]	@ zero_extendqisi2
	ldrb	lr, [r4, #2770]	@ zero_extendqisi2
	bfi	r2, ip, #4, #1
	ldrb	ip, [r4, #2771]	@ zero_extendqisi2
	strb	r2, [fp, #-38]
	bfi	r3, lr, #3, #4
	ldrh	r2, [fp, #-38]
	bfi	r3, ip, #7, #1
	ldrb	lr, [r4, #2774]	@ zero_extendqisi2
	strh	r1, [fp, #-40]	@ movhi
	bfi	r2, lr, #5, #4
	strb	r3, [fp, #-39]
	strh	r2, [fp, #-38]	@ movhi
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+24
	str	r2, [r8, #24]
	bl	dprint_vfmw
	ldrb	r2, [r4, #2759]	@ zero_extendqisi2
	str	r5, [fp, #-40]
	mov	r3, #0
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r1, .L26+28
	str	r2, [r8, #28]
	bl	dprint_vfmw
	ldr	r2, [r4, #2780]
	ldr	r1, .L26+32
	mov	r0, #4
	str	r2, [r8, #32]
	bl	dprint_vfmw
	ldrb	r2, [r4, #2778]	@ zero_extendqisi2
	ldrb	r1, [r4, #2779]	@ zero_extendqisi2
	mov	r3, #0
	str	r5, [fp, #-40]
	mov	r0, #4
	bfi	r3, r1, #0, #6
	strb	r2, [fp, #-40]
	strb	r3, [fp, #-38]
	mov	r5, #0
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+36
	str	r2, [r8, #36]
	bl	dprint_vfmw
	ldr	r2, [r4, #2820]
	ldrb	r3, [r4, #2779]	@ zero_extendqisi2
	ldr	r0, [r4, #2816]
	cmp	r3, r2
	ldr	r1, .L26+40
	addhi	r2, r2, #128
	add	r0, r3, r0
	rsbls	r2, r3, r2
	rsbhi	r2, r3, r2
	mov	r3, r5
	bfi	r3, r0, #0, #25
	str	r3, [fp, #-40]
	mov	r0, #4
	mov	r3, r3, lsr #24
	ldrhi	r6, [r4, #2824]
	ldrls	r6, [r4, #2824]
	bfi	r3, r2, #1, #7
	strb	r3, [fp, #-37]
	subhi	r6, r6, #16
	ldr	r2, [fp, #-40]
	str	r2, [r8, #64]
	bl	dprint_vfmw
	ldr	r1, .L26+44
	mov	r2, r5
	mov	r0, #4
	bfi	r2, r6, #0, #24
	str	r2, [fp, #-40]
	str	r2, [r8, #68]
	bl	dprint_vfmw
	ldr	r1, [r4, #2828]
	mov	r3, r5
	ldr	r2, [r4, #2832]
	bfi	r3, r1, #0, #25
	str	r3, [fp, #-40]
	ldr	r1, .L26+48
	mov	r0, #4
	mov	r3, r3, lsr #24
	bfi	r3, r2, #1, #7
	strb	r3, [fp, #-37]
	ldr	r2, [fp, #-40]
	str	r2, [r8, #72]
	bl	dprint_vfmw
	ldr	r2, [r4, #2836]
	mov	r3, r5
	ldr	r1, .L26+52
	bfi	r3, r2, #0, #24
	mov	r0, #4
	str	r3, [fp, #-40]
	mov	r2, r3
	str	r3, [r8, #76]
	bl	dprint_vfmw
	ldr	ip, [r4, #3036]
	ldr	r1, [r4, #3040]
	mov	r0, #4
	ldr	r2, [r4, #3044]
	ldr	r3, [r4, #3048]
	strb	ip, [fp, #-40]
	strb	r1, [fp, #-39]
	strb	r3, [fp, #-37]
	strb	r2, [fp, #-38]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+56
	str	r2, [r8, #80]
	bl	dprint_vfmw
	ldr	ip, [r4, #3052]
	ldr	r1, [r4, #3056]
	mov	r0, #4
	ldr	r2, [r4, #3060]
	ldr	r3, [r4, #3064]
	strb	ip, [fp, #-40]
	strb	r1, [fp, #-39]
	strb	r3, [fp, #-37]
	strb	r2, [fp, #-38]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+60
	str	r2, [r8, #84]
	bl	dprint_vfmw
	ldr	ip, [r4, #3068]
	ldr	r1, [r4, #3072]
	mov	r0, #4
	ldr	r2, [r4, #3076]
	ldr	r3, [r4, #3080]
	strb	ip, [fp, #-40]
	strb	r1, [fp, #-39]
	strb	r3, [fp, #-37]
	strb	r2, [fp, #-38]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+64
	str	r2, [r8, #88]
	bl	dprint_vfmw
	ldr	ip, [r4, #3084]
	ldr	r1, [r4, #3088]
	mov	r0, #4
	ldr	r2, [r4, #3092]
	ldr	r3, [r4, #3096]
	strb	ip, [fp, #-40]
	strb	r1, [fp, #-39]
	strb	r3, [fp, #-37]
	strb	r2, [fp, #-38]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+68
	str	r2, [r8, #92]
	bl	dprint_vfmw
	ldr	ip, [r4, #3100]
	ldr	r3, [r4, #3112]
	mov	r0, #4
	ldr	r1, [r4, #3104]
	ldr	r2, [r4, #3108]
	strb	ip, [fp, #-40]
	strb	r3, [fp, #-37]
	strb	r1, [fp, #-39]
	strb	r2, [fp, #-38]
	ldr	r2, [fp, #-40]
	ldr	r1, .L26+72
	str	r2, [r8, #96]
	bl	dprint_vfmw
	ldr	r2, [r4, #3136]
	ldr	r1, .L26+76
	mov	r0, #4
	str	r2, [r8, #100]
	bl	dprint_vfmw
	ldr	r2, [r4, #3116]
	ldr	r1, .L26+80
	mov	r0, #4
	str	r2, [r8, #128]
	bl	dprint_vfmw
	ldr	r2, [r4, #3120]
	ldr	r1, .L26+84
	mov	r0, #4
	str	r2, [r8, #132]
	bl	dprint_vfmw
	ldr	r2, [r4, #3124]
	ldr	r1, .L26+88
	mov	r0, #4
	str	r2, [r8, #136]
	bl	dprint_vfmw
	ldr	r2, [r4, #3128]
	ldr	r1, .L26+92
	mov	r0, #4
	str	r2, [r8, #140]
	bl	dprint_vfmw
	ldr	r2, [r9, #1144]
	ldr	r1, .L26+96
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r8, #144]
	bl	dprint_vfmw
	ldr	r2, [r9, #1148]
	ldr	r1, .L26+100
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r8, #148]
	bl	dprint_vfmw
	ldr	r2, [r9, #1152]
	ldr	r1, .L26+104
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r8, #152]
	bl	dprint_vfmw
	ldr	r6, [r9, #1188]
	ldr	r1, .L26+108
	mov	r0, #4
	bic	r6, r6, #15
	str	r6, [r8, #156]
	str	r6, [fp, #-40]
	mov	r2, r6
	bl	dprint_vfmw
	mov	r0, r6
	bl	MEM_Phy2Vir
	cmp	r0, r5
	beq	.L25
	ldr	r3, .L26+112
	mov	r2, #2752
	mov	r1, r4
	add	r7, r4, #2832
	add	r6, r8, #252
	add	r7, r7, #8
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r2, [r9, #1160]
	ldr	r1, .L26+116
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r8, #160]
	bl	dprint_vfmw
	mov	r0, r8
	bl	MEM_Vir2Phy
	ldr	r1, .L26+120
	add	r2, r0, #256
	mov	r0, #4
	str	r2, [r8, #252]
	bl	dprint_vfmw
.L22:
	ldr	r1, [r7, #4]!
	mov	r3, #0
	ldr	ip, [r4, #2876]
	add	r2, r5, #64
	bfi	r3, r1, #0, #25
	str	r3, [fp, #-40]
	ldr	r1, .L26+124
	mov	r0, #4
	mov	r3, r3, lsr #24
	add	r8, r6, #16
	bfi	r3, ip, #1, #7
	strb	r3, [fp, #-37]
	ldr	r3, [fp, #-40]
	str	r3, [r6, #4]
	bl	dprint_vfmw
	ldr	r1, [r4, #2908]
	add	r2, r5, #65
	mov	r3, #0
	mov	r0, #4
	bfi	r3, r1, #0, #24
	ldr	r1, .L26+124
	str	r3, [fp, #-40]
	str	r3, [r6, #8]
	bl	dprint_vfmw
	ldr	r2, [r4, #2940]
	mov	r3, #0
	ldr	ip, [r4, #2972]
	bfi	r3, r2, #0, #25
	str	r3, [fp, #-40]
	add	r2, r5, #66
	ldr	r1, .L26+124
	mov	r3, r3, lsr #24
	mov	r0, #4
	bfi	r3, ip, #1, #7
	strb	r3, [fp, #-37]
	ldr	r3, [fp, #-40]
	str	r3, [r6, #12]
	bl	dprint_vfmw
	ldr	r1, [r4, #3004]
	add	r2, r5, #67
	mov	r0, #4
	mov	r3, #0
	add	r5, r5, #4
	bfi	r3, r1, #0, #24
	ldr	r1, .L26+124
	str	r3, [fp, #-40]
	add	r4, r4, r0
	str	r3, [r6, #16]
	mov	r6, r8
	bl	dprint_vfmw
	cmp	r5, #32
	bne	.L22
	mov	r0, #0
.L18:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L24:
	ldr	r3, .L26+128
	ldr	r2, .L26+132
	ldr	r1, .L26+136
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L18
.L25:
	ldr	r2, .L26+132
	ldr	r1, .L26+140
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L18
.L27:
	.align	2
.L26:
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
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
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC44
	.word	.LC45
	.word	.LC46
	.word	.LC13
	.word	.LANCHOR0+24
	.word	.LC14
	.word	.LC43
	UNWIND(.fnend)
	.size	VP8HAL_V400R003_CfgDnMsg, .-VP8HAL_V400R003_CfgDnMsg
	.align	2
	.global	VP8HAL_V400R003_StartDec
	.type	VP8HAL_V400R003_StartDec, %function
VP8HAL_V400R003_StartDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	cmp	r1, #0
	mov	r4, r0
	beq	.L30
	cmp	r1, #1
	bne	.L37
	mov	r0, #0
	mov	r3, r1
	str	r0, [sp]
	ldr	r2, .L39
	ldr	r1, .L39+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L32
.L30:
	ldr	r3, [r0, #2784]
	cmp	r3, #512
	bhi	.L34
	ldr	r3, [r0, #2788]
	cmp	r3, #512
	bhi	.L34
	ldr	r5, .L39+8
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L38
.L35:
	mov	r3, r2
	ldr	r1, .L39+8
	mov	r2, #0
	mov	r0, r4
	bl	VP8HAL_V400R003_CfgReg
	mov	r0, r4
	mov	r2, #0
	ldr	r1, .L39+8
	bl	VP8HAL_V400R003_CfgDnMsg
	mov	r0, #0
.L32:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L37:
	ldr	r1, .L39+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L38:
	mov	r0, #0
	str	r2, [fp, #-24]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L36
	str	r3, [r5]
	ldr	r2, [fp, #-24]
	b	.L35
.L34:
	ldr	r3, .L39+16
	mov	r0, #0
	ldr	r2, .L39
	ldr	r1, .L39+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L32
.L36:
	ldr	r1, .L39+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L32
.L40:
	.align	2
.L39:
	.word	.LANCHOR0+52
	.word	.LC1
	.word	g_HwMem
	.word	.LC47
	.word	.LC48
	.word	.LC14
	.word	.LC2
	UNWIND(.fnend)
	.size	VP8HAL_V400R003_StartDec, .-VP8HAL_V400R003_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14764, %object
	.size	__func__.14764, 23
__func__.14764:
	.ascii	"VP8HAL_V400R003_CfgReg\000"
	.space	1
	.type	__func__.14791, %object
	.size	__func__.14791, 25
__func__.14791:
	.ascii	"VP8HAL_V400R003_CfgDnMsg\000"
	.space	3
	.type	__func__.14777, %object
	.size	__func__.14777, 25
__func__.14777:
	.ascii	"VP8HAL_V400R003_StartDec\000"
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
	ASCII(.ascii	"BASIC_CFG0 = 0x%x\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"BASIC_CFG1 = 0x%x\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"AVM_ADDR = 0x%x\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"VAM_ADDR = 0x%x\012\000" )
	.space	3
.LC7:
	ASCII(.ascii	"STREAM_BASE_ADDR = 0x%x\012\000" )
	.space	3
.LC8:
	ASCII(.ascii	"PPFD_BUF_ADDR = 0x%x\012\000" )
	.space	2
.LC9:
	ASCII(.ascii	"PPFD_BUF_LEN = 0x%x\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"YSTADDR_1D = 0x%x\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"YSTRIDE_1D = 0x%x\012\000" )
	.space	1
.LC12:
	ASCII(.ascii	"UVOFFSET_1D = 0x%x\012\000" )
.LC13:
	ASCII(.ascii	"can not map down msg virtual address!\000" )
	.space	2
.LC14:
	ASCII(.ascii	"%s: %s\012\000" )
.LC15:
	ASCII(.ascii	"D0 = 0x%x\012\000" )
	.space	1
.LC16:
	ASCII(.ascii	"D1 = 0x%x\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"D2 = 0x%x\012\000" )
	.space	1
.LC18:
	ASCII(.ascii	"D3 = 0x%x\012\000" )
	.space	1
.LC19:
	ASCII(.ascii	"D4 = 0x%x\012\000" )
	.space	1
.LC20:
	ASCII(.ascii	"D5 = 0x%x\012\000" )
	.space	1
.LC21:
	ASCII(.ascii	"D6 = 0x%x\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"D7 = 0x%x\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"D8 = 0x%x\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"D9 = 0x%x\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"D16 = 0x%x\012\000" )
.LC26:
	ASCII(.ascii	"D17 = 0x%x\012\000" )
.LC27:
	ASCII(.ascii	"D18 = 0x%x\012\000" )
.LC28:
	ASCII(.ascii	"D19 = 0x%x\012\000" )
.LC29:
	ASCII(.ascii	"D20 = 0x%x\012\000" )
.LC30:
	ASCII(.ascii	"D21 = 0x%x\012\000" )
.LC31:
	ASCII(.ascii	"D22 = 0x%x\012\000" )
.LC32:
	ASCII(.ascii	"D23 = 0x%x\012\000" )
.LC33:
	ASCII(.ascii	"D24 = 0x%x\012\000" )
.LC34:
	ASCII(.ascii	"D25 = 0x%x\012\000" )
.LC35:
	ASCII(.ascii	"D32 = 0x%x\012\000" )
.LC36:
	ASCII(.ascii	"D33 = 0x%x\012\000" )
.LC37:
	ASCII(.ascii	"D34 = 0x%x\012\000" )
.LC38:
	ASCII(.ascii	"D35 = 0x%x\012\000" )
.LC39:
	ASCII(.ascii	"D36 = 0x%x\012\000" )
.LC40:
	ASCII(.ascii	"D37 = 0x%x\012\000" )
.LC41:
	ASCII(.ascii	"D38 = 0x%x\012\000" )
.LC42:
	ASCII(.ascii	"D39 = 0x%x\012\000" )
.LC43:
	ASCII(.ascii	"%s: u8Tmp = NULL\012\000" )
	.space	2
.LC44:
	ASCII(.ascii	"D40 = 0x%x\012\000" )
.LC45:
	ASCII(.ascii	"D63 = 0x%x\012\000" )
.LC46:
	ASCII(.ascii	"D%d = 0x%x\012\000" )
.LC47:
	ASCII(.ascii	"VdhId is wrong! VP8HAL_V200R003_StartDec\012\000" )
	.space	2
.LC48:
	ASCII(.ascii	"picture width out of range\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
