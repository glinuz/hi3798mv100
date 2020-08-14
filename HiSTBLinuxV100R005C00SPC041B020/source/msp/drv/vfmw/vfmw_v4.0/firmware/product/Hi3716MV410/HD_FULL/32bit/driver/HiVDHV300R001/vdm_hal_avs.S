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
	.file	"vdm_hal_avs.c"
	.text
	.align	2
	.global	AVSHAL_V300R001_InitHal
	.type	AVSHAL_V300R001_InitHal, %function
AVSHAL_V300R001_InitHal:
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
	.size	AVSHAL_V300R001_InitHal, .-AVSHAL_V300R001_InitHal
	.align	2
	.global	AVSHAL_V300R001_WirteSlicMsg
	.type	AVSHAL_V300R001_WirteSlicMsg, %function
AVSHAL_V300R001_WirteSlicMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	ldr	ip, .L49
	movw	r3, #1208
	mov	r6, r0
	str	r0, [fp, #-52]
	mla	r3, r3, r2, ip
	str	r1, [fp, #-68]
	add	r1, r0, #208
	str	r2, [fp, #-76]
	str	r1, [fp, #-48]
	ldr	r5, [r3, #48]
	add	r4, r5, #320
	mov	r0, r5
	bl	MEM_Phy2Vir
	ldr	r3, [r6, #224]
	mov	r1, r6
	cmp	r3, #0
	moveq	r1, r3
	add	r2, r0, #320
	beq	.L3
	ldr	r3, [fp, #-52]
	mov	ip, #0
	ldr	r1, [fp, #-68]
	add	r5, r5, #576
	ldr	r3, [r3, #208]
	add	r3, r3, #4
	rsb	r3, r1, r3
	and	r1, r3, #15
	bic	r3, r3, #15
	mov	r1, r1, asl #3
	cmp	r1, #7
	addls	r1, r1, #120
	subhi	r1, r1, #8
	subls	r3, r3, #4
	mov	r1, r1, asl #25
	bic	r3, r3, #-16777216
	orr	r1, r1, #8
	str	r1, [r0, #320]
	str	r3, [r0, #324]
	mov	r1, #1
	ldr	r3, [fp, #-52]
	str	ip, [r0, #328]
	str	ip, [r0, #332]
	ldr	r3, [r3, #224]
	sub	r3, r3, #1
	mov	r3, r3, asl #16
	str	r3, [r0, #336]
	str	r5, [r0, #572]
.L3:
	ldr	r3, [fp, #-52]
	add	r3, r3, #12288
	ldr	r0, [r3, #2256]
	cmp	r0, #0
	ble	.L2
	add	r1, r4, r1, lsl #8
	str	r1, [fp, #-72]
	mov	r1, #0
	mov	r10, r1
	mov	r5, r1
	b	.L27
.L44:
	ldr	r0, [fp, #-48]
	mov	ip, r5, asl #2
	mov	r7, r5, asl #5
	cmp	r5, #0
	rsb	r8, ip, r7
	add	r4, r0, r8
	movgt	r0, #1
	movle	r0, #0
	ldr	r6, [r4, #16]
	ldr	lr, [r4, #-12]
	cmp	r6, lr
	movhi	r6, #0
	andls	r6, r0, #1
	cmp	r6, #0
	beq	.L43
	ldr	r0, [r3, #2256]
	add	r5, r5, #1
	cmp	r0, r5
	ble	.L2
.L27:
	cmp	r10, #512
	blt	.L44
	sub	r5, r1, #1
.L47:
	ldr	r0, [r3, #2256]
	add	r5, r5, #1
	cmp	r0, r5
	bgt	.L27
.L2:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L43:
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-48]
	str	r2, [fp, #-60]
	str	ip, [fp, #-56]
	ldr	r0, [r3, r8]
	ldr	r3, .L49+4
	add	r0, r0, #3
	ldr	r1, [r3, #184]
	blx	r1
	ldr	ip, [fp, #-56]
	ldr	r2, [fp, #-60]
	ldr	r3, [fp, #-64]
	ldr	r1, [r0]
	bic	r1, r1, #-16777216
	cmp	r1, #131072
	beq	.L45
	ldr	r1, [fp, #-48]
	ldr	r9, [r1, r8]
	ldr	r1, [fp, #-68]
	add	r9, r9, #4
	rsb	r9, r1, r9
	and	r8, r9, #15
	bic	r9, r9, #15
	mov	r8, r8, asl #3
.L15:
	ldmib	r4, {r0, r1}
	cmp	r0, #0
	mov	r1, r1, asl #3
	moveq	ip, r0
	moveq	lr, ip
	beq	.L16
	rsb	r7, ip, r7
	ldr	ip, [fp, #-52]
	add	r7, ip, r7
	ldr	ip, [r7, #220]
	cmp	ip, #0
	ble	.L31
	ldr	ip, [fp, #-68]
	cmp	r1, #0
	ldr	r7, [r4, #12]
	rsb	ip, ip, r0
	and	lr, ip, #15
	bic	r6, ip, #15
	mov	r0, lr, asl #3
	mov	lr, r7, asl #3
	bne	.L46
	mov	ip, #0
	mov	r1, lr
	mov	r9, r6
	mov	r8, r0
	mov	lr, ip
.L16:
	cmp	r8, #7
	add	r1, r1, #8
	addls	r8, r8, #120
	subhi	r8, r8, #8
	mov	r0, r5, asl #8
	bic	r1, r1, #-33554432
	orr	r8, r1, r8, asl #25
	add	r7, r0, #8
	add	r1, r0, #4
	add	r6, r0, #12
	subls	r9, r9, #4
	str	r8, [r2, r5, asl #8]
	bic	r9, r9, #-16777216
	str	r9, [r2, r1]
	str	lr, [r2, r7]
	add	r1, r5, #1
	str	ip, [r2, r6]
	ldr	lr, [r3, #2256]
	ldr	ip, [r4, #16]
	cmp	r1, lr
	bge	.L20
	ldr	r5, [r4, #44]
	cmp	ip, r5
	bcs	.L21
	b	.L20
.L23:
	ldr	r5, [r4, #44]
	cmp	r5, ip
	bhi	.L25
.L21:
	add	r1, r1, #1
	add	r4, r4, #28
	cmp	r1, lr
	bne	.L23
.L24:
	ldr	r5, [fp, #-52]
	mov	r1, lr
	ldr	r4, [r5, #168]
	ldr	lr, [r5, #164]
	mov	r5, #0
	mul	lr, lr, r4
	sub	lr, lr, #1
.L26:
	add	r4, r0, #16
	add	r0, r0, #252
	uxth	ip, ip
	add	r10, r10, #1
	orr	ip, ip, lr, asl #16
	str	ip, [r2, r4]
	str	r5, [r2, r0]
	sub	r5, r1, #1
	b	.L47
.L20:
	cmp	r1, lr
	beq	.L24
.L25:
	ldr	r4, [fp, #-72]
	mov	lr, r1, asl #5
	sub	lr, lr, r1, asl #2
	add	r5, r4, r1, lsl #8
	ldr	r4, [fp, #-48]
	add	lr, r4, lr
	ldr	lr, [lr, #16]
	sub	lr, lr, #1
	b	.L26
.L46:
	bic	lr, lr, #-33554432
	bic	ip, ip, #-16777216
	orr	lr, lr, r0, asl #25
	bic	ip, ip, #15
	b	.L16
.L45:
	ldr	r3, [fp, #-48]
	mov	r9, #64
	ldr	r2, .L49+4
	ldr	r0, [r3, r8]
	ldr	r1, [r2, #184]
	add	r0, r0, #2
	blx	r1
	ldr	r3, [fp, #-48]
	ldr	r2, .L49+4
	strb	r6, [r0]
	ldr	r0, [r3, r8]
	ldr	r1, [r2, #184]
	add	r0, r0, #4
	blx	r1
	ldr	r3, [fp, #-48]
	ldr	r2, .L49+4
	strb	r9, [r0]
	ldr	r0, [r3, r8]
	ldr	r1, [r2, #184]
	add	r0, r0, #5
	blx	r1
	strb	r9, [r0]
	ldr	r0, [fp, #-76]
	bl	VDMHAL_V300R001_ResetVdm
	ldr	r3, [fp, #-48]
	ldr	r1, [fp, #-76]
	ldr	ip, [fp, #-56]
	ldr	r9, [r3, r8]
	cmp	r1, #0
	ldr	r3, [fp, #-68]
	add	r9, r9, #2
	ldr	r2, [fp, #-60]
	rsb	r9, r3, r9
	ldr	r3, [fp, #-64]
	and	r8, r9, #15
	bic	r9, r9, #15
	mov	r8, r8, asl #3
	beq	.L11
	cmp	r1, #1
	bne	.L48
	str	r3, [fp, #-80]
	ldr	r3, .L49+8
	ldr	r1, [fp, #-76]
	str	r2, [fp, #-64]
	ldr	r0, [r3]
	str	ip, [fp, #-60]
	add	r0, r0, #2032
	str	r3, [fp, #-56]
	bl	MEM_WritePhyWord
	ldr	r3, .L49+8
	ldr	r1, [fp, #-76]
	ldr	r0, [r3]
	add	r0, r0, #1792
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r3, .L49+8
	mov	r1, r6
	ldr	r0, [r3]
	add	r0, r0, #2032
	bl	MEM_WritePhyWord
	ldr	ip, [fp, #-60]
	ldr	r2, [fp, #-64]
	ldr	r3, [fp, #-80]
.L14:
	str	r3, [fp, #-80]
	mov	r1, #0
	ldr	r3, [fp, #-56]
	str	r2, [fp, #-64]
	str	ip, [fp, #-60]
	ldr	r0, [r3]
	add	r0, r0, #8
	bl	MEM_WritePhyWord
	ldr	ip, [fp, #-60]
	ldr	r2, [fp, #-64]
	ldr	r3, [fp, #-80]
	b	.L15
.L31:
	mov	ip, #0
	mov	lr, ip
	b	.L16
.L48:
	ldr	r1, [fp, #-76]
	cmp	r1, #0
	ldreq	r6, .L49+12
	beq	.L13
	ldr	r1, [fp, #-76]
	cmp	r1, #1
	ldreq	r1, .L49+8
	streq	r1, [fp, #-56]
	bne	.L15
	b	.L14
.L11:
	str	r3, [fp, #-64]
	mov	r1, #1
	ldr	r3, .L49+12
	str	r2, [fp, #-60]
	str	ip, [fp, #-56]
	ldr	r0, [r3]
	mov	r6, r3
	add	r0, r0, #2032
	bl	MEM_WritePhyWord
	ldr	r0, [r6]
	mov	r1, #1
	add	r0, r0, #1792
	add	r0, r0, #12
	bl	MEM_WritePhyWord
	ldr	r0, [r6]
	ldr	r1, [fp, #-76]
	add	r0, r0, #2032
	bl	MEM_WritePhyWord
	ldr	ip, [fp, #-56]
	ldr	r2, [fp, #-60]
	ldr	r3, [fp, #-64]
.L13:
	ldr	r0, [r6]
	mov	r1, #0
	str	r3, [fp, #-64]
	add	r0, r0, #8
	str	r2, [fp, #-60]
	str	ip, [fp, #-56]
	bl	MEM_WritePhyWord
	ldr	ip, [fp, #-56]
	ldr	r2, [fp, #-60]
	ldr	r3, [fp, #-64]
	b	.L15
.L50:
	.align	2
.L49:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	s_RegPhyBaseAddr_1
	.word	s_RegPhyBaseAddr
	UNWIND(.fnend)
	.size	AVSHAL_V300R001_WirteSlicMsg, .-AVSHAL_V300R001_WirteSlicMsg
	.align	2
	.global	AVSHAL_V300R001_StartDec
	.type	AVSHAL_V300R001_StartDec, %function
AVSHAL_V300R001_StartDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	r3, [r0, #164]
	mov	r4, r0
	str	r1, [fp, #-60]
	cmp	r3, #512
	bhi	.L92
	ldr	r3, [r0, #168]
	cmp	r3, #512
	bhi	.L92
	ldr	r3, [fp, #-60]
	cmp	r3, #1
	bhi	.L94
	ldr	r3, [fp, #-60]
	cmp	r3, #0
	bgt	.L95
	ldr	r3, [fp, #-60]
	movw	r5, #1208
	ldr	r8, .L99
	mul	r5, r5, r3
	ldr	r3, [r8, r5]
	cmp	r3, #0
	beq	.L96
.L58:
	add	r3, r4, #12288
	str	r3, [fp, #-56]
	ldr	ip, [r3, #2256]
	cmp	ip, #0
	ble	.L60
	mov	r2, r4
	mov	r0, #0
	mvn	r5, #0
.L61:
	ldr	r3, [r2, #208]
	cmp	r3, #0
	beq	.L64
	ldr	r1, [r2, #216]
	bic	r3, r3, #15
	cmp	r1, #0
	ble	.L64
	cmp	r5, r3
	movcs	r5, r3
.L64:
	ldr	r3, [r2, #212]
	cmp	r3, #0
	beq	.L62
	ldr	r1, [r2, #220]
	bic	r3, r3, #15
	cmp	r1, #0
	ble	.L62
	cmp	r5, r3
	movcs	r5, r3
.L62:
	add	r0, r0, #1
	add	r2, r2, #28
	cmp	r0, ip
	bne	.L61
	cmn	r5, #1
	beq	.L60
	ldr	r1, [r4, #164]
	movw	r6, #1208
	ldr	ip, [r4, #168]
	mov	r7, #0
	ldr	r3, [fp, #-60]
	mov	r2, r7
	mov	lr, #65
	bfi	lr, r7, #1, #1
	mul	ip, r1, ip
	ldr	r10, .L99+4
	mul	r6, r6, r3
	ldr	r1, .L99+8
	sub	ip, ip, #1
	mov	r0, #3
	bfi	r2, ip, #0, #20
	str	r2, [fp, #-48]
	strb	lr, [fp, #-45]
	add	r9, r8, r6
	mov	r2, r2, lsr #16
	orr	r2, r2, #64
	ldr	r3, [r8, r6]
	bfi	r2, r7, #7, #1
	strb	r2, [fp, #-46]
	ldr	r2, [fp, #-48]
	str	r2, [r3, #8]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r3, [fp, #-56]
	mov	r2, #805306368
	str	r2, [fp, #-48]
	ldrh	ip, [fp, #-46]
	mov	r1, #1
	ldr	lr, [r3, #2676]
	mov	r2, #6
	bfi	ip, r1, #0, #12
	strb	r2, [fp, #-48]
	ldrh	r1, [fp, #-48]
	mov	r0, #3
	mov	r2, ip, lsr #8
	mov	lr, lr, lsr #6
	and	r2, r2, #239
	bfi	r1, lr, #4, #9
	bfi	r2, r7, #5, #1
	strh	ip, [fp, #-46]	@ movhi
	strb	r2, [fp, #-45]
	uxtb	ip, r2
	ldr	lr, [r3, #2696]
	mov	r2, r1, lsr #8
	strh	r1, [fp, #-48]	@ movhi
	and	r2, r2, #223
	bfi	ip, lr, #6, #1
	orr	r2, r2, #64
	bfi	ip, r7, #7, #1
	bfi	r2, r7, #7, #1
	strb	ip, [fp, #-45]
	strb	r2, [fp, #-47]
	ldr	lr, [r8, r6]
	ldr	ip, [fp, #-48]
	ldr	r1, .L99+12
	str	ip, [lr, #12]
	mov	r2, ip
	ldr	ip, [r10, #68]
	blx	ip
	ldr	ip, [r8, r6]
	ldr	r2, [r9, #48]
	mov	r0, #3
	ldr	r1, .L99+16
	bic	r2, r2, #15
	str	r2, [ip, #16]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	ip, [r8, r6]
	ldr	r2, [r9, #32]
	mov	r0, #3
	ldr	r1, .L99+20
	bic	r2, r2, #15
	str	r2, [ip, #20]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	ip, [r8, r6]
	ldr	r1, .L99+24
	mov	r2, r5
	mov	r0, #3
	str	r5, [fp, #-48]
	str	r5, [ip, #24]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r2, [r4, #164]
	movw	r0, #49156
	cmp	r2, #120
	movt	r0, 63683
	movls	r7, #65536
	bl	MEM_ReadPhyWord
	ldr	r2, .L99+28
	ldr	r2, [r2]
	orr	r1, r0, r7
	add	r0, r2, #4
	bl	MEM_WritePhyWord
	ldr	ip, [r8, r6]
	movw	r2, #3075
	ldr	r7, [fp, #-56]
	movt	r2, 48
	ldr	r1, .L99+32
	str	r2, [ip, #60]
	mov	r0, #3
	ldr	ip, [r8, r6]
	str	r2, [ip, #64]
	ldr	ip, [r8, r6]
	str	r2, [ip, #68]
	ldr	ip, [r8, r6]
	str	r2, [ip, #72]
	ldr	ip, [r8, r6]
	str	r2, [ip, #76]
	ldr	ip, [r8, r6]
	str	r2, [ip, #80]
	ldr	ip, [r8, r6]
	str	r2, [ip, #84]
	ldr	ip, [r8, r6]
	ldr	r2, [r7, #2272]
	bic	r2, r2, #15
	str	r2, [ip, #96]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	ip, [r8, r6]
	ldr	r2, [r7, #2676]
	mov	r0, #3
	ldr	r6, [r4, #168]
	ldr	r1, .L99+36
	str	r2, [ip, #100]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r1, [r4, #164]
	mov	r1, r1, asl #4
	sub	r2, r1, #1
	cmp	r2, #2048
	movcc	r9, #16
	bcs	.L97
.L68:
	ldr	r3, [fp, #-56]
	mov	r7, r6, asl #4
	add	r7, r7, #31
	movw	ip, #1208
	add	r2, r6, #1
	ldr	r1, .L99+40
	ldr	lr, [r3, #2676]
	mov	r6, r7, lsr #5
	ldr	r3, [fp, #-60]
	mov	r2, r2, lsr #1
	mul	r9, r6, r9
	mov	r0, #3
	mul	r7, ip, r3
	mov	r6, #0
	mul	r2, lr, r2
	add	r3, r8, r7
	str	r3, [fp, #-64]
	ldr	lr, [r8, r7]
	add	r2, r2, r9, lsl #4
	mov	r2, r2, asl #1
	str	r2, [lr, #104]
	ldr	r3, [r10, #68]
	blx	r3
	ldr	r2, [r8, r7]
	mov	r3, r9, asl #5
	ldr	r1, .L99+44
	mov	r0, #3
	ldr	r9, .L99+4
	str	r3, [r2, #108]
	ldr	r3, [fp, #-64]
	ldr	lr, [r8, r7]
	ldr	r2, [r3, #1184]
	str	r2, [lr, #144]
	ldr	r3, [r10, #68]
	blx	r3
	ldr	lr, [r8, r7]
	mov	r2, r6
	ldr	r1, .L99+48
	mov	r0, #3
	str	r6, [fp, #-48]
	str	r6, [lr, #148]
	ldr	r3, [r10, #68]
	blx	r3
	ldr	lr, [r8, r7]
	mov	r2, r6
	ldr	r1, .L99+52
	mov	r0, #3
	str	r6, [lr, #152]
	ldr	r3, [r10, #68]
	blx	r3
	ldr	r2, [r8, r7]
	ldr	r3, [fp, #-64]
	mvn	r1, #0
	str	r1, [r2, #32]
	ldr	r0, [r3, #48]
	bl	MEM_Phy2Vir
	subs	r7, r0, #0
	beq	.L98
	ldr	r2, [r4, #168]
	mov	r0, #4
	ldrh	r1, [fp, #-46]
	sub	r2, r2, #1
	ldr	lr, [r4, #124]
	bfi	r1, r2, #0, #9
	ldr	ip, [r4, #164]
	strh	r1, [fp, #-46]	@ movhi
	ldrh	r2, [fp, #-48]
	mov	r1, r1, lsr #8
	bfi	r1, lr, #1, #2
	sub	ip, ip, #1
	strb	r1, [fp, #-45]
	bfi	r2, ip, #0, #9
	ldr	lr, [r4, #120]
	uxtb	r1, r1
	strh	r2, [fp, #-48]	@ movhi
	mov	r2, r2, lsr #8
	bfi	r1, lr, #3, #2
	bfi	r2, r6, #1, #7
	bfi	r1, r6, #5, #3
	strb	r2, [fp, #-47]
	strb	r1, [fp, #-45]
	ldr	r2, [fp, #-48]
	ldr	r1, .L99+56
	str	r2, [r7]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r1, [r4, #64]
	ldr	ip, [r4, #32]
	mov	r2, r6
	ldr	lr, [r4, #28]
	bfi	r2, r1, #0, #1
	bfi	r2, ip, #1, #2
	ldr	ip, [r4, #60]
	ldr	r0, [r4, #40]
	bfi	r2, lr, #3, #2
	mov	r1, r6, lsr #24
	bfi	r2, ip, #5, #1
	ldr	ip, [r4, #36]
	bfi	r1, r0, #0, #1
	ldr	lr, [r4, #48]
	mov	r0, r6, lsr #24
	bfi	r1, ip, #1, #1
	ldr	ip, [r4, #44]
	ldr	r3, [r4, #56]
	bfi	r0, lr, #0, #6
	str	r6, [fp, #-48]
	bfi	r0, ip, #6, #1
	ldr	lr, [r4, #52]
	bfi	r2, r3, #6, #1
	ldr	ip, [r4, #88]
	strb	r1, [fp, #-46]
	bfi	r2, lr, #7, #1
	ldrh	r1, [fp, #-46]
	bfi	r0, ip, #7, #1
	strb	r2, [fp, #-48]
	bfi	r1, r6, #2, #14
	strb	r0, [fp, #-47]
	strh	r1, [fp, #-46]	@ movhi
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L99+60
	str	r2, [r7, #4]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r1, [r4, #80]
	ldr	ip, [r4, #76]
	mov	r2, r6
	bfi	r2, r1, #0, #1
	ldr	r1, [r4, #72]
	bfi	r2, ip, #1, #1
	str	r6, [fp, #-48]
	ldr	ip, [r4, #68]
	bfi	r2, r1, #2, #5
	strb	r2, [fp, #-48]
	mov	r0, #4
	ldrh	r2, [fp, #-48]
	ldr	r1, .L99+64
	bfi	r2, ip, #7, #5
	strh	r2, [fp, #-48]	@ movhi
	ldr	r2, [fp, #-48]
	str	r2, [r7, #8]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r1, [r4, #100]
	ldr	ip, [r4, #104]
	mov	r2, r6
	bfi	r2, r1, #0, #1
	ldr	r1, [r4, #108]
	ldr	lr, [r4, #112]
	bfi	r2, ip, #1, #2
	bfi	r2, r1, #3, #1
	ldr	r1, [r4, #92]
	bfi	r2, lr, #4, #2
	str	r6, [fp, #-48]
	ldr	ip, [r4, #96]
	bfi	r2, r1, #6, #1
	strb	r2, [fp, #-48]
	mov	r0, #4
	ldrh	r2, [fp, #-48]
	ldr	r1, .L99+68
	bfi	r2, ip, #7, #2
	strh	r2, [fp, #-48]	@ movhi
	ldr	r2, [fp, #-48]
	bfi	r2, r6, #9, #23
	str	r2, [fp, #-48]
	str	r2, [r7, #12]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r3, [fp, #-56]
	ldr	r1, .L99+72
	mov	r0, #4
	ldr	r2, [r3, #2260]
	bic	r2, r2, #15
	str	r2, [r7, #16]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r3, [fp, #-56]
	ldr	r1, .L99+76
	mov	r0, #4
	ldr	r2, [r3, #2264]
	bic	r2, r2, #15
	str	r2, [r7, #20]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r3, [fp, #-56]
	ldr	r1, .L99+80
	mov	r0, #4
	ldr	r2, [r3, #2268]
	bic	r2, r2, #15
	str	r2, [r7, #24]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r3, [fp, #-56]
	ldr	r1, .L99+84
	mov	r0, #4
	ldr	r2, [r3, #2284]
	bic	r2, r2, #15
	str	r2, [r7, #28]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r0, [r4, #116]
	ldr	ip, [r4, #84]
	mov	r1, r6, lsr #24
	str	r6, [fp, #-48]
	mov	r2, r6
	bfi	r1, ip, #0, #2
	bfi	r2, r0, #0, #1
	strb	r1, [fp, #-45]
	mov	r0, #4
	strb	r2, [fp, #-48]
	ldr	r2, [fp, #-48]
	ldr	r1, .L99+88
	str	r2, [r7, #32]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r2, [r4, #184]
	mov	ip, r6
	ldr	r1, .L99+92
	bfi	ip, r2, #0, #24
	mov	r0, #4
	str	ip, [fp, #-48]
	mov	r2, ip
	str	ip, [r7, #36]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r1, [r4, #200]
	ldr	ip, [r4, #192]
	mov	r2, r6
	bfi	r2, r1, #0, #24
	str	r2, [fp, #-48]
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	mov	r0, #4
	ldr	r1, .L99+96
	bfi	r2, ip, #0, #7
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r7, #40]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r2, [r4, #188]
	mov	ip, r6
	ldr	r1, .L99+100
	bfi	ip, r2, #0, #24
	mov	r0, #4
	str	ip, [fp, #-48]
	mov	r2, ip
	str	ip, [r7, #44]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r0, [r4, #128]
	ldr	ip, [r4, #136]
	mov	r2, r6, lsr #24
	mov	r1, r6
	bfi	r1, r0, #0, #1
	bfi	r2, ip, #0, #6
	ldr	r0, [r4, #132]
	ldr	ip, [r4, #144]
	ldr	lr, [r4, #148]
	bfi	r1, r0, #1, #1
	str	r6, [fp, #-48]
	bfi	r2, ip, #6, #1
	ldr	r0, [r4, #140]
	mov	ip, r6, lsr #24
	ldr	r6, [r4, #152]
	bfi	r2, lr, #7, #1
	bfi	r1, r0, #2, #6
	strb	r2, [fp, #-47]
	strb	r1, [fp, #-48]
	bfi	ip, r6, #0, #1
	strb	ip, [fp, #-46]
	mov	r0, #4
	ldr	r2, [fp, #-48]
	ldr	r1, .L99+104
	str	r2, [r7, #48]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r6, [fp, #-64]
	ldr	r1, .L99+108
	mov	r0, #4
	ldr	r2, [r6, #1140]
	bic	r2, r2, #15
	str	r2, [r7, #52]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r2, [r6, #1144]
	ldr	r1, .L99+112
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r7, #56]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r3, [fp, #-56]
	ldr	r1, .L99+116
	mov	r0, #4
	ldr	r2, [r3, #2280]
	bic	r2, r2, #15
	str	r2, [r7, #64]
	ldr	ip, [r9, #68]
	blx	ip
	add	r2, r4, #14528
	add	ip, r4, #14656
	add	r2, r2, #48
	add	ip, ip, #48
	add	r1, r7, #68
.L69:
	ldr	r3, [r2, #4]!
	cmp	r2, ip
	str	r3, [r1], #4
	bne	.L69
	ldr	r3, [fp, #-56]
	mov	r0, #4
	ldr	r1, .L99+120
	ldr	r2, [r3, #2276]
	ldr	r3, [r10, #68]
	bic	r2, r2, #15
	str	r2, [r7, #196]
	blx	r3
	ldr	r2, [fp, #-60]
	movw	r3, #1208
	ldr	r1, .L99+124
	mov	r0, #4
	mla	r8, r3, r2, r8
	ldr	r3, [r10, #68]
	ldr	r2, [r8, #1152]
	bic	r2, r2, #15
	str	r2, [r7, #200]
	blx	r3
	ldr	r2, [r8, #1136]
	ldr	r3, [r10, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L99+128
	str	r2, [r7, #204]
	blx	r3
	ldr	r3, [r8, #48]
	ldr	ip, [r10, #68]
	mov	r0, #4
	add	r3, r3, #320
	ldr	r1, .L99+132
	str	r3, [r7, #252]
	mov	r2, r3
	str	r3, [fp, #-48]
	blx	ip
	ldr	ip, [r4, #128]
	cmp	ip, #1
	beq	.L70
.L74:
	mov	r0, r4
	ldr	r2, [fp, #-60]
	mov	r1, r5
	bl	AVSHAL_V300R001_WirteSlicMsg
	mov	r0, #0
.L88:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L97:
	sub	r2, r1, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r9, #32
	bcc	.L68
	sub	r2, r1, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r9, #48
	bcc	.L68
	sub	r1, r1, #6144
	sub	r1, r1, #1
	cmp	r1, #2048
	movcs	r9, #16
	movcc	r9, #64
	b	.L68
.L70:
	add	r0, r4, #14720
	add	lr, r7, #256
	add	r0, r0, #12
	add	r7, r7, #320
.L73:
	tst	ip, #1
	add	r0, r0, #16
	add	ip, ip, #1
	ldrneb	r1, [r0, #-24]	@ zero_extendqisi2
	ldreqb	r1, [r0, #-36]	@ zero_extendqisi2
	ldrneb	r3, [r0, #-16]	@ zero_extendqisi2
	ldreqb	r3, [r0, #-28]	@ zero_extendqisi2
	movne	r1, r1, asl #16
	ldrneb	r2, [r0, #-40]	@ zero_extendqisi2
	moveq	r1, r1, asl #16
	ldrneb	r6, [r0, #-32]	@ zero_extendqisi2
	ldreqb	r2, [r0, #-52]	@ zero_extendqisi2
	orr	r3, r1, r3, asl #24
	ldreqb	r6, [r0, #-44]	@ zero_extendqisi2
	orr	r3, r3, r2
	orr	r3, r3, r6, asl #8
	str	r3, [lr], #4
	cmp	lr, r7
	bne	.L73
	b	.L74
.L96:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L59
	str	r3, [r8, r5]
	b	.L58
.L95:
	ldr	r1, .L99+4
	mov	r0, #0
	ldr	r2, .L99+136
	str	r0, [sp]
	ldr	ip, [r1, #68]
	ldr	r1, .L99+140
	blx	ip
	mvn	r0, #0
	b	.L88
.L92:
	ldr	r1, .L99+4
	mov	r0, #0
	ldr	r3, .L99+144
	ldr	r2, .L99+136
	ldr	ip, [r1, #68]
	ldr	r1, .L99+148
	blx	ip
	mvn	r0, #0
	b	.L88
.L94:
	ldr	r3, .L99+4
	mov	r0, #0
	ldr	r1, .L99+152
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L88
.L60:
	ldr	r3, .L99+4
	mvn	r2, #0
	ldr	r1, .L99+156
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L88
.L98:
	ldr	ip, [r9, #68]
	ldr	r3, .L99+160
	ldr	r2, .L99+136
	ldr	r1, .L99+148
	blx	ip
	mvn	r0, #0
	b	.L88
.L59:
	ldr	r3, .L99+4
	ldr	r1, .L99+164
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L88
.L100:
	.align	2
.L99:
	.word	g_HwMem
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	s_RegPhyBaseAddr
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC12
	.word	.LC13
	.word	.LC14
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
	.word	.LANCHOR0
	.word	.LC3
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC5
	.word	.LC11
	.word	.LC4
	UNWIND(.fnend)
	.size	AVSHAL_V300R001_StartDec, .-AVSHAL_V300R001_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.13999, %object
	.size	__func__.13999, 25
__func__.13999:
	.ascii	"AVSHAL_V300R001_StartDec\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC1:
	ASCII(.ascii	"%s: %s\012\000" )
.LC2:
	ASCII(.ascii	"VdhId is wrong! AVSHAL_V200R003_StartDec\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC4:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC5:
	ASCII(.ascii	"stream_base_addr = %#x\012\000" )
.LC6:
	ASCII(.ascii	"BASIC_V300R001_CFG0 = 0x%x\012\000" )
.LC7:
	ASCII(.ascii	"BASIC_V300R001_CFG1 = 0x%x\012\000" )
.LC8:
	ASCII(.ascii	"AVM_V300R001_ADDR = 0x%x\012\000" )
	.space	2
.LC9:
	ASCII(.ascii	"VAM_V300R001_ADDR = 0x%x\012\000" )
	.space	2
.LC10:
	ASCII(.ascii	"STREAM_V300R001_BASE_ADDR = 0x%x\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"can not map down msg virtual address!\000" )
	.space	2
.LC12:
	ASCII(.ascii	"D0 = 0x%x\012\000" )
	.space	1
.LC13:
	ASCII(.ascii	"D1 = 0x%x\012\000" )
	.space	1
.LC14:
	ASCII(.ascii	"D2 = 0x%x\012\000" )
	.space	1
.LC15:
	ASCII(.ascii	"D3 = 0x%x\012\000" )
	.space	1
.LC16:
	ASCII(.ascii	"D4 = 0x%x\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"D5 = 0x%x\012\000" )
	.space	1
.LC18:
	ASCII(.ascii	"D6 = 0x%x\012\000" )
	.space	1
.LC19:
	ASCII(.ascii	"D7 = 0x%x\012\000" )
	.space	1
.LC20:
	ASCII(.ascii	"D8 = 0x%x\012\000" )
	.space	1
.LC21:
	ASCII(.ascii	"D9 = 0x%x\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"D10 = 0x%x\012\000" )
.LC23:
	ASCII(.ascii	"D11 = 0x%x\012\000" )
.LC24:
	ASCII(.ascii	"D12 = 0x%x\012\000" )
.LC25:
	ASCII(.ascii	"D13 = 0x%x\012\000" )
.LC26:
	ASCII(.ascii	"D14 = 0x%x\012\000" )
.LC27:
	ASCII(.ascii	"D16 = 0x%x\012\000" )
.LC28:
	ASCII(.ascii	"D49 = 0x%x\012\000" )
.LC29:
	ASCII(.ascii	"D50 = 0x%x\012\000" )
.LC30:
	ASCII(.ascii	"D51 = 0x%x\012\000" )
.LC31:
	ASCII(.ascii	"D63 = 0x%x\012\000" )
.LC32:
	ASCII(.ascii	"YSTADDR_V300R001_1D = 0x%x\012\000" )
.LC33:
	ASCII(.ascii	"YSTRIDE_V300R001_1D = 0x%x\012\000" )
.LC34:
	ASCII(.ascii	"UVOFFSET_V300R001_1D = 0x%x\012\000" )
	.space	3
.LC35:
	ASCII(.ascii	"VREG_V300R001_DNR_MBINFO_STADDR = 0x%x\012\000" )
.LC36:
	ASCII(.ascii	"VREG_V200R003_REF_PIC_TYPE = 0x%x\012\000" )
	.space	1
.LC37:
	ASCII(.ascii	"VREG_V300R001_FF_APT_EN = 0x%x\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
