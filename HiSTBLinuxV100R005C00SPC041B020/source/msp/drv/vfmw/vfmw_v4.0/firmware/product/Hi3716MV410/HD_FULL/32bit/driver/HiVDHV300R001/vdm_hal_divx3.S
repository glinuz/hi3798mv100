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
	.file	"vdm_hal_divx3.c"
	.text
	.align	2
	.global	DIVX3HAL_V300R001_InitHal
	.type	DIVX3HAL_V300R001_InitHal, %function
DIVX3HAL_V300R001_InitHal:
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
	.size	DIVX3HAL_V300R001_InitHal, .-DIVX3HAL_V300R001_InitHal
	.align	2
	.global	DIVX3HAL_V300R001_WriteReg
	.type	DIVX3HAL_V300R001_WriteReg, %function
DIVX3HAL_V300R001_WriteReg:
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
	ldr	r7, .L27
	mov	r4, r1
	mov	r8, r0
	ldr	r1, .L27+4
	mov	r0, #2
	ldr	r3, [r7, #68]
	mov	r5, r2
	blx	r3
	ldr	ip, [r4, #12]
	ldr	r2, [r4, #8]
	mov	r1, #0
	mov	r3, r1
	cmp	r5, r1
	mov	r0, #0
	bfi	r0, r1, #1, #1
	mul	r2, r2, ip
	sub	r2, r2, #1
	bfi	r3, r2, #0, #20
	str	r3, [fp, #-48]
	strb	r0, [fp, #-45]
	mov	r3, r3, lsr #16
	bfi	r3, r1, #7, #1
	strb	r3, [fp, #-46]
	ble	.L23
	mov	r3, #1
	ldr	r2, .L27+8
	str	r3, [sp]
	mov	r0, #32
	ldr	ip, [r7, #68]
	mov	r3, r5
	ldr	r1, .L27+12
	blx	ip
	ldr	r2, [fp, #-48]
.L4:
	ldr	r3, [r7, #68]
	mov	r0, #3
	ldr	r1, .L27+16
	blx	r3
	mov	r2, #0
	str	r2, [fp, #-48]
	mov	r0, #5
	ldrh	r3, [fp, #-46]
	cmp	r5, r2
	ldr	r1, [r4, #136]
	bfi	r3, r2, #0, #12
	strh	r3, [fp, #-46]	@ movhi
	strb	r0, [fp, #-48]
	mov	r3, r3, lsr #8
	ldrh	r0, [fp, #-48]
	and	r3, r3, #239
	ldr	ip, [r4, #152]
	bfi	r3, r2, #5, #1
	mov	r1, r1, lsr #6
	bfi	r0, r1, #4, #9
	ldr	r6, .L27
	uxtb	r3, r3
	ubfx	r1, r0, #8, #5
	bfi	r3, ip, #6, #1
	strh	r0, [fp, #-48]	@ movhi
	orr	r1, r1, #192
	bfi	r3, r2, #7, #1
	strb	r1, [fp, #-47]
	strb	r3, [fp, #-45]
	ble	.L24
	mov	r10, #1
	ldr	ip, [r6, #68]
	mov	r3, r5
	ldr	r2, .L27+8
	ldr	r1, .L27+12
	mov	r0, #32
	str	r10, [sp]
	blx	ip
	ldr	r3, [r6, #68]
	ldr	r2, [fp, #-48]
	mov	r0, #3
	ldr	r1, .L27+20
	blx	r3
	ldr	r9, [r8, #48]
	mov	r3, r5
	str	r10, [sp]
	bic	r9, r9, #15
	ldr	ip, [r6, #68]
	ldr	r2, .L27+8
	mov	r0, #32
	ldr	r1, .L27+12
	blx	ip
	mov	r2, r9
	ldr	r3, [r6, #68]
	mov	r0, #3
	ldr	r1, .L27+24
	blx	r3
	ldr	r9, [r8, #32]
	mov	r3, r5
	str	r10, [sp]
	bic	r9, r9, #15
	ldr	ip, [r6, #68]
	ldr	r2, .L27+8
	mov	r0, #32
	ldr	r1, .L27+12
	blx	ip
	mov	r2, r9
	ldr	r3, [r6, #68]
	mov	r0, #3
	ldr	r1, .L27+28
	blx	r3
	ldr	r9, [r4, #96]
	mov	r3, r5
	str	r10, [sp]
	mov	r0, #32
	ldr	ip, [r6, #68]
	ldr	r2, .L27+8
	ldr	r1, .L27+12
	str	r9, [fp, #-48]
	blx	ip
.L6:
	ldr	r3, [r7, #68]
	mov	r2, r9
	ldr	r1, .L27+32
	mov	r0, #3
	blx	r3
	ldr	r3, [r4, #8]
	movw	r0, #49156
	cmp	r3, #256
	movt	r0, 63683
	bls	.L7
	bl	MEM_ReadPhyWord
	cmp	r5, #0
	uxth	r0, r0
	str	r0, [fp, #-48]
	beq	.L10
.L26:
	cmp	r5, #1
	bne	.L25
	ldr	r3, .L27+36
	ldr	r1, [fp, #-48]
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
.L13:
	mov	r0, #0
	ldr	ip, [r7, #68]
	mov	r3, r5
	str	r0, [sp]
	ldr	r2, .L27+8
	ldr	r1, .L27+40
	blx	ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L7:
	bl	MEM_ReadPhyWord
	cmp	r5, #0
	orr	r0, r0, #65536
	str	r0, [fp, #-48]
	bne	.L26
.L10:
	ldr	r3, .L27+44
	ldr	r1, [fp, #-48]
	ldr	r0, [r3]
	add	r0, r0, #4
	bl	MEM_WritePhyWord
	b	.L12
.L25:
	cmp	r5, #0
	bgt	.L13
.L12:
	movw	r9, #1208
	ldr	r6, .L27+48
	mul	r9, r9, r5
	movw	r3, #3075
	movt	r3, 48
	ldr	r1, .L27+52
	mov	r2, r3
	mov	r0, #3
	ldr	ip, [r6, r9]
	str	r3, [ip, #60]
	ldr	ip, [r6, r9]
	str	r3, [ip, #64]
	ldr	ip, [r6, r9]
	str	r3, [ip, #68]
	ldr	ip, [r6, r9]
	str	r3, [ip, #72]
	ldr	ip, [r6, r9]
	str	r3, [ip, #76]
	ldr	ip, [r6, r9]
	str	r3, [ip, #80]
	ldr	ip, [r6, r9]
	str	r3, [ip, #84]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r6, r9]
	ldr	r2, [r4, #128]
	mov	r0, #3
	ldr	r1, .L27+56
	bic	r2, r2, #15
	str	r2, [r3, #96]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r6, r9]
	ldr	r2, [r4, #136]
	mov	r0, #3
	ldr	r9, [r4, #12]
	ldr	r1, .L27+60
	str	r2, [r3, #100]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, #8]
	mov	r3, r3, asl #4
	sub	r2, r3, #1
	cmp	r2, #2048
	movcc	r10, #16
	bcc	.L15
	sub	r2, r3, #2048
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r10, #32
	bcc	.L15
	sub	r2, r3, #4096
	sub	r2, r2, #1
	cmp	r2, #2048
	movcc	r10, #48
	bcc	.L15
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	r10, #16
	movcc	r10, #64
.L15:
	ldr	r3, [r4, #12]
	add	lr, r9, #1
	ldr	r2, [r4, #136]
	movw	ip, #1208
	mul	r5, ip, r5
	mov	r3, r3, asl #4
	add	r3, r3, #31
	mov	lr, lr, lsr #1
	mov	r9, r9, asl #4
	ldr	r1, .L27+64
	mov	r3, r3, lsr #5
	add	r9, r9, #31
	mul	r2, r2, lr
	mov	r3, r3, asl #4
	bic	r9, r9, #31
	ldr	ip, [r6, r5]
	mla	r3, r10, r3, r2
	mov	r0, #3
	mul	r10, r10, r9
	mov	r4, #0
	mov	r2, r3, asl #1
	str	r2, [ip, #104]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r6, r5]
	ldr	r1, .L27+68
	mov	r0, #3
	str	r10, [r3, #108]
	ldr	r3, [r8, #1184]
	ldr	ip, [r6, r5]
	mov	r2, r3
	str	r3, [ip, #144]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r6, r5]
	mov	r2, r4
	ldr	r1, .L27+72
	mov	r0, #3
	str	r4, [fp, #-48]
	str	r4, [r3, #148]
	ldr	r3, [r7, #68]
	blx	r3
	ldrb	r3, [fp, #-48]	@ zero_extendqisi2
	ldr	ip, [r6, r5]
	mov	r0, #3
	bfi	r3, r4, #0, #1
	strb	r3, [fp, #-48]
	ldr	r2, [fp, #-48]
	ldr	r1, .L27+76
	str	r2, [ip, #152]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r6, r5]
	mvn	r2, #0
	str	r2, [r3, #32]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L24:
	movw	r9, #1208
	ldr	r10, .L27+48
	mul	r9, r9, r5
	ldr	r3, [fp, #-48]
	ldr	r1, .L27+20
	mov	r0, #3
	mov	r2, r3
	ldr	ip, [r10, r9]
	str	r3, [ip, #12]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r10, r9]
	ldr	r2, [r8, #48]
	mov	r0, #3
	ldr	r1, .L27+24
	bic	r2, r2, #15
	str	r2, [r3, #16]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r10, r9]
	ldr	r2, [r8, #32]
	mov	r0, #3
	ldr	r1, .L27+28
	bic	r2, r2, #15
	str	r2, [r3, #20]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r10, r9]
	ldr	r9, [r4, #96]
	str	r9, [r3, #24]
	str	r9, [fp, #-48]
	b	.L6
.L23:
	movw	r3, #1208
	ldr	r1, .L27+48
	mul	r3, r3, r5
	ldr	r2, [fp, #-48]
	ldr	r3, [r1, r3]
	str	r2, [r3, #8]
	b	.L4
.L28:
	.align	2
.L27:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	s_RegPhyBaseAddr_1
	.word	.LC7
	.word	s_RegPhyBaseAddr
	.word	g_HwMem
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	UNWIND(.fnend)
	.size	DIVX3HAL_V300R001_WriteReg, .-DIVX3HAL_V300R001_WriteReg
	.align	2
	.global	DIVX3HAL_V300R001_WritePicMsg
	.type	DIVX3HAL_V300R001_WritePicMsg, %function
DIVX3HAL_V300R001_WritePicMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
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
	ldr	r5, .L32
	mov	r7, r0
	mov	r4, r1
	mov	r0, #2
	ldr	r1, .L32+4
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r0, [r7, #48]
	bic	r0, r0, #15
	bl	MEM_Phy2Vir
	subs	r6, r0, #0
	beq	.L31
	ldr	r2, [r4, #4]
	mov	r8, #0
	mov	r3, #0
	str	r8, [fp, #-40]
	bfi	r3, r2, #0, #1
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r1, .L32+8
	str	r2, [r6]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [r4, #8]
	ldr	r3, [r4, #12]
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
	ldr	r1, .L32+12
	str	r2, [r6, #4]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r4, #16]
	ldr	r1, [r4, #20]
	mov	r0, #4
	ldr	r2, [r4, #24]
	and	r3, r3, #31
	bfi	r3, r1, #5, #1
	str	r8, [fp, #-40]
	bfi	r3, r2, #6, #1
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	ldr	r1, .L32+16
	str	r2, [r6, #8]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r4, #28]
	ldr	r1, [r4, #32]
	mov	r2, #0
	ldr	ip, [r4, #36]
	and	r3, r3, #3
	ldr	lr, [r4, #40]
	bfi	r3, r1, #2, #2
	bfi	r3, ip, #4, #1
	ldr	r1, [r4, #44]
	ldr	ip, [r4, #48]
	bfi	r3, lr, #5, #2
	str	r8, [fp, #-40]
	bfi	r3, r1, #7, #1
	bfi	r2, ip, #0, #1
	strb	r3, [fp, #-40]
	strb	r2, [fp, #-39]
	mov	r0, #4
	ldr	r2, [fp, #-40]
	ldr	r1, .L32+20
	str	r2, [r6, #12]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [r4, #100]
	ldr	r1, .L32+24
	mov	r0, #4
	str	r2, [r6, #64]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [r4, #108]
	str	r8, [fp, #-40]
	mov	r3, #0
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r1, .L32+28
	str	r2, [r6, #68]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [r4, #116]
	ldr	r3, [r5, #68]
	mov	r0, #4
	ldr	r1, .L32+32
	str	r2, [r6, #72]
	blx	r3
	ldr	r2, [r4, #104]
	ldr	r3, [r5, #68]
	mov	r0, #4
	ldr	r1, .L32+36
	str	r2, [r6, #76]
	blx	r3
	ldr	r2, [r4, #112]
	str	r8, [fp, #-40]
	mov	r3, #0
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r3, [r5, #68]
	ldr	r1, .L32+40
	str	r2, [r6, #80]
	blx	r3
	ldr	r2, [r4, #120]
	ldr	r3, [r5, #68]
	mov	r0, #4
	ldr	r1, .L32+44
	str	r2, [r6, #84]
	blx	r3
	ldr	r2, [r4, #132]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+48
	str	r2, [r6, #128]
	blx	r3
	ldr	r2, [r4, #124]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+52
	str	r2, [r6, #132]
	blx	r3
	ldr	r2, [r7, #1136]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+56
	str	r2, [r6, #136]
	blx	r3
	ldr	r2, [r7, #1140]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+60
	str	r2, [r6, #140]
	blx	r3
	ldr	r2, [r7, #1148]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+64
	str	r2, [r6, #144]
	blx	r3
	ldr	r2, [r4, #144]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+68
	str	r2, [r6, #148]
	blx	r3
	ldr	r2, [r7, #52]
	ldr	r3, [r5, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+72
	str	r2, [r6, #252]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L31:
	ldr	ip, [r5, #68]
	ldr	r3, .L32+76
	ldr	r2, .L32+80
	ldr	r1, .L32+84
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	ip
.L33:
	.align	2
.L32:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC15
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
	.word	.LC16
	.word	.LANCHOR0+28
	.word	.LC17
	UNWIND(.fnend)
	.size	DIVX3HAL_V300R001_WritePicMsg, .-DIVX3HAL_V300R001_WritePicMsg
	.align	2
	.global	DIVX3HAL_V300R001_WriteSliceMsg
	.type	DIVX3HAL_V300R001_WriteSliceMsg, %function
DIVX3HAL_V300R001_WriteSliceMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
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
	ldr	r10, [r0, #52]
	mov	r6, r1
	bic	r10, r10, #15
	mov	r0, r10
	bl	MEM_Phy2Vir
	subs	r8, r0, #0
	beq	.L52
	ldr	r9, [r6, #56]
	cmp	r9, #0
	bne	.L53
.L36:
	ldr	r3, [r6, #52]
	cmp	r3, #0
	beq	.L34
	mov	r3, r9, asl #6
	ldr	r9, .L54
	mov	r4, #0
	str	r3, [fp, #-56]
	add	r3, r3, r10
	str	r3, [fp, #-60]
.L47:
	cmp	r4, #0
	ldreq	r3, [r6, #56]
	beq	.L40
	add	r2, r6, r4, lsl #2
	ldr	r3, [r2, #56]
	ldr	r2, [r2, #52]
	cmp	r3, r2
	bls	.L41
.L40:
	ldr	r1, [fp, #-56]
	mov	r2, #0
	mov	r0, #4
	bfi	r2, r3, #0, #20
	add	r5, r1, r4, lsl #6
	str	r2, [fp, #-48]
	ldr	r1, .L54+4
	str	r2, [r8, r5, asl #2]
	mov	r5, r5, asl #2
	ldr	r3, [r9, #68]
	blx	r3
	ldr	r2, [r6, #52]
	add	lr, r4, #1
	cmp	lr, r2
	bcs	.L42
	add	r3, r6, r4, lsl #2
	ldr	r1, [r3, #56]
	ldr	r3, [r3, #60]
	cmp	r1, r3
	bcc	.L42
	add	r4, r4, #15
	add	r4, r6, r4, lsl #2
	b	.L43
.L44:
	ldr	r3, [r4, #4]!
	cmp	r3, r1
	bhi	.L42
.L43:
	add	lr, lr, #1
	cmp	lr, r2
	bcc	.L44
.L42:
	cmp	r2, lr
	add	r4, r5, #4
	addne	r2, lr, #14
	add	r5, r5, #252
	ldreq	r1, [r6, #12]
	moveq	r7, #0
	ldreq	r2, [r6, #8]
	moveq	r10, r7
	ldrne	r1, [r6, r2, asl #2]
	mov	r0, #4
	ldrne	r3, [fp, #-60]
	muleq	r1, r2, r1
	addne	r10, r3, lr, lsl #8
	mov	r2, #0
	movne	r7, r10
	sub	r1, r1, #1
	bfi	r2, r1, #0, #20
	ldr	r1, .L54+8
	str	r2, [fp, #-48]
	str	r2, [r8, r4]
	sub	r4, lr, #1
	ldr	r3, [r9, #68]
	blx	r3
	str	r7, [r8, r5]
	mov	r2, r10
	ldr	r1, .L54+12
	mov	r0, #4
	ldr	ip, [r9, #68]
	str	r10, [fp, #-48]
	blx	ip
.L41:
	ldr	r3, [r6, #52]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L47
.L34:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L53:
	mov	r4, #0
	ldr	r5, .L54
	mov	r3, r4
	bfi	r3, r4, #0, #20
	ldr	r1, .L54+4
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r8]
	mov	r9, #1
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r6, #56]
	ldr	r1, .L54+8
	mov	r2, r4
	sub	r3, r3, #1
	mov	r0, #4
	bfi	r2, r3, #0, #20
	str	r2, [fp, #-48]
	str	r2, [r8, #4]
	ldr	r3, [r5, #68]
	blx	r3
	add	r3, r10, #256
	ldr	ip, [r5, #68]
	mov	r0, #4
	str	r3, [r8, #252]
	mov	r2, r3
	ldr	r1, .L54+12
	str	r3, [fp, #-48]
	blx	ip
	b	.L36
.L52:
	ldr	r1, .L54
	ldr	r3, .L54+16
	ldr	r2, .L54+20
	ldr	ip, [r1, #68]
	ldr	r1, .L54+24
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L55:
	.align	2
.L54:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC18
	.word	.LC19
	.word	.LC34
	.word	.LC16
	.word	.LANCHOR0+60
	.word	.LC17
	UNWIND(.fnend)
	.size	DIVX3HAL_V300R001_WriteSliceMsg, .-DIVX3HAL_V300R001_WriteSliceMsg
	.align	2
	.global	DIVX3HAL_V300R001_StartDec
	.type	DIVX3HAL_V300R001_StartDec, %function
DIVX3HAL_V300R001_StartDec:
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
	movw	r4, #1208
	mul	r4, r4, r1
	ldr	r7, .L67
	cmp	r1, #1
	mov	r2, r1
	mov	r6, r0
	add	r5, r4, r7
	bhi	.L64
	cmp	r1, #0
	bgt	.L65
	ldr	r3, [r4, r7]
	cmp	r3, #0
	beq	.L66
.L61:
	ldr	ip, .L67+4
	mov	r1, r6
	mov	r0, r5
	ldr	r3, [ip]
	add	r3, r3, #1
	str	r3, [ip]
	bl	DIVX3HAL_V300R001_WriteReg
	mov	r1, r6
	mov	r0, r5
	bl	DIVX3HAL_V300R001_WritePicMsg
	mov	r1, r6
	mov	r0, r5
	bl	DIVX3HAL_V300R001_WriteSliceMsg
	ldr	r3, .L67+8
	ldr	r1, .L67+12
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
.L59:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L66:
	mov	r0, #0
	str	r1, [fp, #-32]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L62
	str	r3, [r4, r7]
	ldr	r2, [fp, #-32]
	b	.L61
.L65:
	ldr	r1, .L67+8
	mov	r0, #0
	mov	r3, r2
	str	r0, [sp]
	ldr	r2, .L67+16
	ldr	ip, [r1, #68]
	ldr	r1, .L67+20
	blx	ip
	mvn	r0, #0
	b	.L59
.L64:
	ldr	r3, .L67+8
	mov	r0, #0
	ldr	r1, .L67+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L59
.L62:
	ldr	r3, .L67+8
	ldr	r1, .L67+28
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L59
.L68:
	.align	2
.L67:
	.word	g_HwMem
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC37
	.word	.LANCHOR0+92
	.word	.LC7
	.word	.LC35
	.word	.LC36
	UNWIND(.fnend)
	.size	DIVX3HAL_V300R001_StartDec, .-DIVX3HAL_V300R001_StartDec
	.align	2
	.global	DIVX3HAL_V300R001_StartVDH
	.type	DIVX3HAL_V300R001_StartVDH, %function
DIVX3HAL_V300R001_StartVDH:
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
	ldr	r3, .L70
	mov	r0, #2
	ldr	r1, .L70+4
	ldr	r3, [r3, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L71:
	.align	2
.L70:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC37
	UNWIND(.fnend)
	.size	DIVX3HAL_V300R001_StartVDH, .-DIVX3HAL_V300R001_StartVDH
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14128, %object
	.size	__func__.14128, 27
__func__.14128:
	.ascii	"DIVX3HAL_V300R001_WriteReg\000"
	.space	1
	.type	__func__.14140, %object
	.size	__func__.14140, 30
__func__.14140:
	.ascii	"DIVX3HAL_V300R001_WritePicMsg\000"
	.space	2
	.type	__func__.14154, %object
	.size	__func__.14154, 32
__func__.14154:
	.ascii	"DIVX3HAL_V300R001_WriteSliceMsg\000"
	.type	__func__.14119, %object
	.size	__func__.14119, 27
__func__.14119:
	.ascii	"DIVX3HAL_V300R001_StartDec\000"
	.data
	.align	2
.LANCHOR1 = . + 0
	.type	FrameNum, %object
	.size	FrameNum, 4
FrameNum:
	.word	-1
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"configuring VDM registers...\012\000" )
	.space	2
.LC1:
	ASCII(.ascii	"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\012\000" )
	.space	3
.LC2:
	ASCII(.ascii	"BASIC_V200R003_CFG0 = 0x%x\012\000" )
.LC3:
	ASCII(.ascii	"BASIC_V300R001_CFG1=0x%x\012\000" )
	.space	2
.LC4:
	ASCII(.ascii	"AVM_V300R001_ADDR=0x%x\012\000" )
.LC5:
	ASCII(.ascii	"VAM_V300R001_ADDR=0x%x\012\000" )
.LC6:
	ASCII(.ascii	"STREAM_V200R003_BASE_ADDR=0x%x\012\000" )
.LC7:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC8:
	ASCII(.ascii	"TIME_OUT = 0x%x\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"YSTADDR_V300R001_1D = 0x%x\012\000" )
.LC10:
	ASCII(.ascii	"YSTRIDE_V300R001_1D = 0x%x\012\000" )
.LC11:
	ASCII(.ascii	"UVOFFSET_V300R001_1D = 0x%x\012\000" )
	.space	3
.LC12:
	ASCII(.ascii	"DNR_MBINFO_STADDR=0x%x\012\000" )
.LC13:
	ASCII(.ascii	"REF_V300R001_PIC_TYPE=0x%x\012\000" )
.LC14:
	ASCII(.ascii	"FF_V300R001_APT_EN=0x%x\012\000" )
	.space	3
.LC15:
	ASCII(.ascii	"configuring Pic Msg...\012\000" )
.LC16:
	ASCII(.ascii	"can not map dn msg virtual address!\012\000" )
	.space	3
.LC17:
	ASCII(.ascii	"%s: %s\012\000" )
.LC18:
	ASCII(.ascii	"D0 = 0x%x\012\000" )
	.space	1
.LC19:
	ASCII(.ascii	"D1 = 0x%x\012\000" )
	.space	1
.LC20:
	ASCII(.ascii	"D2 = 0x%x\012\000" )
	.space	1
.LC21:
	ASCII(.ascii	"D3 = 0x%x\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"D16 = 0x%x\012\000" )
.LC23:
	ASCII(.ascii	"D17 = 0x%x\012\000" )
.LC24:
	ASCII(.ascii	"D18 = 0x%x\012\000" )
.LC25:
	ASCII(.ascii	"D19 = 0x%x\012\000" )
.LC26:
	ASCII(.ascii	"D20 = 0x%x\012\000" )
.LC27:
	ASCII(.ascii	"D21 = 0x%x\012\000" )
.LC28:
	ASCII(.ascii	"D32 = 0x%x\012\000" )
.LC29:
	ASCII(.ascii	"D33 = 0x%x\012\000" )
.LC30:
	ASCII(.ascii	"D34 = 0x%x\012\000" )
.LC31:
	ASCII(.ascii	"D35 = 0x%x\012\000" )
.LC32:
	ASCII(.ascii	"D36 = 0x%x\012\000" )
.LC33:
	ASCII(.ascii	"D37 = 0x%x\012\000" )
.LC34:
	ASCII(.ascii	"D63 = 0x%x\012\000" )
.LC35:
	ASCII(.ascii	"VdhId is wrong! DIVX3HAL_V300R001_StartDec\012\000" )
.LC36:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC37:
	ASCII(.ascii	"start VDM...\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
