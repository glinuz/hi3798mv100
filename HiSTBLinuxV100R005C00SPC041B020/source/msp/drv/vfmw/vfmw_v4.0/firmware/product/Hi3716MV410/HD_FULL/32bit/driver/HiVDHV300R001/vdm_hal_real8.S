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
	.file	"vdm_hal_real8.c"
	.text
	.align	2
	.global	RV8HAL_V300R001_InitHal
	.type	RV8HAL_V300R001_InitHal, %function
RV8HAL_V300R001_InitHal:
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
	.size	RV8HAL_V300R001_InitHal, .-RV8HAL_V300R001_InitHal
	.align	2
	.global	RV8HAL_V300R001_WriteReg
	.type	RV8HAL_V300R001_WriteReg, %function
RV8HAL_V300R001_WriteReg:
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
	ldmib	r4, {r2, ip}
	mov	r1, #0
	cmp	r5, r1
	mov	r3, r1
	mov	r0, #1
	mul	r2, r2, ip
	bfi	r0, r1, #1, #1
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
	mov	r1, #0
	ldr	lr, [r4, #64]
	mov	r2, #8
	str	r1, [fp, #-48]
	add	ip, r4, #8192
	ldrh	r3, [fp, #-46]
	cmp	r5, r1
	strb	r2, [fp, #-48]
	mov	lr, lr, lsr #6
	bfi	r3, r1, #0, #12
	ldrh	r0, [fp, #-48]
	strh	r3, [fp, #-46]	@ movhi
	bfi	r0, lr, #4, #9
	mov	r2, r3, lsr r2
	and	r2, r2, #239
	strh	r0, [fp, #-48]	@ movhi
	bfi	r2, r1, #5, #1
	mov	r3, r0, lsr #8
	strb	r2, [fp, #-45]
	and	r3, r3, #223
	ldr	lr, [r4, #40]
	uxtb	r2, r2
	ldr	ip, [ip, #3152]
	bfi	r3, lr, #6, #1
	ldr	r6, .L27
	bfi	r2, ip, #6, #1
	mvn	r3, r3, asl #25
	bfi	r2, r1, #7, #1
	mvn	r3, r3, lsr #25
	strb	r2, [fp, #-45]
	strb	r3, [fp, #-47]
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
	ldr	r9, [r4, #44]
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
	ldr	r3, [r4, #4]
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
	ldr	r2, [r4, #56]
	mov	r0, #3
	ldr	r1, .L27+56
	bic	r2, r2, #15
	str	r2, [r3, #96]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r6, r9]
	ldr	r2, [r4, #64]
	mov	r0, #3
	ldr	r9, [r4, #8]
	ldr	r1, .L27+60
	str	r2, [r3, #100]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, #4]
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
	ldr	r3, [r4, #8]
	add	r9, r9, #1
	ldr	r2, [r4, #64]
	movw	ip, #1208
	mov	lr, r9, lsr #1
	ldr	r1, .L27+64
	mov	r3, r3, asl #4
	mov	r0, #3
	add	r3, r3, #31
	mov	r9, #0
	mul	r5, ip, r5
	mov	r3, r3, lsr #5
	mul	r2, r2, lr
	mov	r3, r3, asl #4
	mla	r3, r10, r3, r2
	ldr	ip, [r6, r5]
	mov	r2, r3, asl #1
	str	r2, [ip, #104]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4, #8]
	ldr	r2, [r6, r5]
	mov	r0, #3
	ldr	r1, .L27+68
	mov	r3, r3, asl #4
	add	r3, r3, #31
	bic	r3, r3, #31
	mul	r10, r10, r3
	str	r10, [r2, #108]
	ldr	r3, [r8, #1184]
	ldr	ip, [r6, r5]
	mov	r2, r3
	str	r3, [ip, #144]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r6, r5]
	mov	r2, r9
	ldr	r1, .L27+72
	mov	r0, #3
	str	r9, [fp, #-48]
	str	r9, [r3, #148]
	ldr	r3, [r7, #68]
	blx	r3
	ldrb	r3, [fp, #-48]	@ zero_extendqisi2
	ldr	ip, [r6, r5]
	mov	r0, #3
	bfi	r3, r9, #0, #1
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
	ldr	r9, [r4, #44]
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
	.size	RV8HAL_V300R001_WriteReg, .-RV8HAL_V300R001_WriteReg
	.align	2
	.global	RV8HAL_V300R001_WritePicMsg
	.type	RV8HAL_V300R001_WritePicMsg, %function
RV8HAL_V300R001_WritePicMsg:
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
	ldr	r4, .L32
	mov	r7, r0
	mov	r6, r1
	mov	r0, #2
	ldr	r1, .L32+4
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r0, [r7, #48]
	bic	r0, r0, #15
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L31
	ldrb	r2, [r6, #12]	@ zero_extendqisi2
	mov	r8, #0
	mov	r3, #0
	str	r8, [fp, #-40]
	bfi	r3, r2, #0, #2
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r1, .L32+8
	str	r2, [r5]
	ldr	r3, [r4, #68]
	blx	r3
	ldmib	r6, {r2, r3}
	mov	r0, #0	@ movhi
	sub	r2, r2, #1
	sub	r3, r3, #1
	mov	r1, r0	@ movhi
	bfi	r0, r2, #0, #9
	bfi	r1, r3, #0, #9
	strh	r0, [fp, #-40]	@ movhi
	strh	r1, [fp, #-38]	@ movhi
	mov	r0, #4
	ldr	r2, [fp, #-40]
	ldr	r1, .L32+12
	str	r2, [r5, #4]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #20]
	mov	r0, #4
	ldr	r1, .L32+16
	strh	r2, [fp, #-40]	@ movhi
	strh	r3, [fp, #-38]	@ movhi
	ldr	r2, [fp, #-40]
	str	r2, [r5, #8]
	ldr	r3, [r4, #68]
	blx	r3
	str	r8, [fp, #-40]
	mov	r3, #0
	bfi	r3, r8, #1, #3
	strb	r3, [fp, #-40]
	mov	r0, #4
	ldr	r2, [fp, #-40]
	ldr	r1, .L32+20
	str	r2, [r5, #12]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r6, #24]
	str	r8, [fp, #-40]
	mov	r3, #0
	bfi	r3, r2, #0, #5
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r1, .L32+24
	str	r2, [r5, #16]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r6, #28]
	str	r8, [fp, #-40]
	mov	r3, #0
	bfi	r3, r2, #0, #5
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r1, .L32+28
	str	r2, [r5, #20]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r6, #32]
	str	r8, [fp, #-40]
	mov	r3, #0
	bfi	r3, r2, #0, #5
	strb	r3, [fp, #-40]
	ldr	r2, [fp, #-40]
	mov	r0, #4
	ldr	r1, .L32+32
	str	r2, [r5, #24]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r6, #60]
	ldr	r1, .L32+36
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #64]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r6, #52]
	ldr	r1, .L32+40
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #68]
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r2, [r6, #48]
	ldr	r3, [r4, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+44
	str	r2, [r5, #72]
	blx	r3
	ldr	r2, [r6, #72]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L32+48
	str	r2, [r5, #76]
	blx	r3
	ldr	r2, [r6, #76]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L32+52
	str	r2, [r5, #80]
	blx	r3
	ldr	r2, [r7, #1136]
	ldr	r3, [r4, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+56
	str	r2, [r5, #84]
	blx	r3
	ldr	r2, [r7, #1140]
	ldr	r3, [r4, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+60
	str	r2, [r5, #88]
	blx	r3
	ldr	r2, [r7, #1144]
	ldr	r3, [r4, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+64
	str	r2, [r5, #92]
	blx	r3
	ldr	r2, [r7, #1152]
	ldr	r3, [r4, #68]
	mov	r0, #4
	ldr	r1, .L32+68
	str	r2, [r5, #96]
	blx	r3
	ldr	r2, [r7, #48]
	ldr	r3, [r4, #68]
	mov	r0, #4
	bic	r2, r2, #15
	ldr	r1, .L32+72
	add	r2, r2, #256
	str	r2, [r5, #252]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L31:
	ldr	ip, [r4, #68]
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
	.size	RV8HAL_V300R001_WritePicMsg, .-RV8HAL_V300R001_WritePicMsg
	.align	2
	.global	RV8HAL_V300R001_WriteSliceMsg
	.type	RV8HAL_V300R001_WriteSliceMsg, %function
RV8HAL_V300R001_WriteSliceMsg:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r7, .L55
	mov	r4, r0
	mov	r10, r1
	mov	r0, #2
	ldr	r1, .L55+4
	add	r3, r10, #80
	str	r3, [fp, #-56]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r4, [r4, #48]
	bic	r4, r4, #15
	add	r5, r4, #256
	mov	r0, r5
	bl	MEM_Phy2Vir
	subs	r8, r0, #0
	beq	.L52
	ldr	r3, [r10, #116]
	cmp	r3, #0
	bne	.L53
.L36:
	ldr	r2, [r10, #36]
	cmp	r2, #0
	beq	.L34
	mov	r3, r3, asl #6
	mov	r4, #0
	str	r3, [fp, #-64]
	add	r3, r3, r5
	str	r3, [fp, #-68]
.L46:
	cmp	r4, #0
	beq	.L49
	ldr	r3, [fp, #-56]
	mov	r5, #44
	mla	r5, r5, r4, r3
	ldr	r2, [r5, #36]
	ldr	r3, [r5, #-8]
	cmp	r2, r3
	ble	.L40
.L39:
	ldr	r2, [r5, #16]
	mov	r9, #0
	ldr	ip, [r5, #8]
	mov	r3, r9
	bfi	r3, r2, #0, #24
	str	r3, [fp, #-48]
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	mov	r0, #4
	ldr	r3, [fp, #-64]
	bfi	r2, ip, #0, #7
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	add	r3, r3, r4, lsl #6
	ldr	r1, .L55+8
	add	r4, r4, #1
	mov	r6, r3, asl #2
	str	r2, [r8, r3, asl #2]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r1, [r5]
	add	r3, r6, #4
	mov	r2, r9
	mov	r0, #4
	bfi	r2, r1, #0, #24
	ldr	r1, .L55+12
	str	r2, [fp, #-48]
	str	r2, [r8, r3]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r1, [r5, #20]
	ldr	r2, [r5, #12]
	mov	r3, r9
	bfi	r3, r1, #0, #24
	str	r3, [fp, #-48]
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	add	ip, r6, #8
	ldr	r1, .L55+16
	mov	r0, #4
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r8, ip]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r1, [r5, #4]
	add	r3, r6, #12
	mov	r2, r9
	mov	r0, #4
	bfi	r2, r1, #0, #24
	ldr	r1, .L55+20
	str	r2, [fp, #-48]
	str	r2, [r8, r3]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r5, #24]
	ldr	r1, [r5, #28]
	add	ip, r6, #16
	ldr	r2, [r5, #32]
	and	r3, r3, #1
	bfi	r3, r1, #1, #2
	str	r9, [fp, #-48]
	bfi	r3, r2, #3, #5
	strb	r3, [fp, #-46]
	ldr	r2, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L55+24
	str	r2, [r8, ip]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r2, [r10, #36]
	ldr	r1, [r5, #36]
	cmp	r4, r2
	str	r9, [fp, #-48]
	strh	r1, [fp, #-48]	@ movhi
	bcs	.L41
	mov	r3, #44
	ldr	ip, [fp, #-56]
	mul	r3, r3, r4
	add	r0, ip, r3
	ldr	r0, [r0, #36]
	cmp	r1, r0
	blt	.L41
	sub	r3, r3, #44
	add	r3, ip, r3
	b	.L42
.L43:
	ldr	r0, [r3, #80]
	cmp	r1, r0
	blt	.L41
.L42:
	add	r4, r4, #1
	add	r3, r3, #44
	cmp	r4, r2
	bcc	.L43
.L41:
	cmp	r2, r4
	beq	.L54
	ldr	r3, [fp, #-68]
	mov	r2, #44
	add	r9, r3, r4, lsl #8
	ldr	r3, [fp, #-56]
	mla	r2, r2, r4, r3
	mov	r3, r9
	ldr	r2, [r2, #36]
	sub	r2, r2, #1
	str	r2, [r10, #120]
.L45:
	ldr	r2, [r5, #40]
	add	ip, r6, #20
	ldr	r1, .L55+28
	mov	r0, #4
	str	r3, [fp, #-60]
	add	r6, r6, #252
	strh	r2, [fp, #-46]	@ movhi
	sub	r4, r4, #1
	ldr	r2, [fp, #-48]
	str	r2, [r8, ip]
	ldr	ip, [r7, #68]
	blx	ip
	ldr	r3, [fp, #-60]
	mov	r2, r9
	ldr	r1, .L55+32
	mov	r0, #4
	str	r9, [fp, #-48]
	str	r3, [r8, r6]
	ldr	r3, [r7, #68]
	blx	r3
.L40:
	ldr	r3, [r10, #36]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L46
.L34:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L49:
	ldr	r5, [fp, #-56]
	b	.L39
.L54:
	ldr	r1, [r10, #8]
	mov	r3, #0
	ldr	r2, [r10, #4]
	mov	r9, r3
	mul	r2, r2, r1
	sub	r2, r2, #1
	str	r2, [r10, #120]
	b	.L45
.L53:
	mov	r6, #0
	ldr	r2, [r10, #88]
	mov	r3, r6
	bfi	r3, r6, #0, #24
	str	r3, [fp, #-48]
	mov	r0, #4
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	add	r4, r4, #512
	ldr	r1, .L55+8
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r8]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r2, [r10, #80]
	mov	r3, r6
	ldr	r1, .L55+12
	bfi	r3, r2, #0, #24
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r8, #4]
	ldr	r3, [r7, #68]
	blx	r3
	mov	r3, r6
	bfi	r3, r6, #0, #24
	str	r3, [fp, #-48]
	ldr	r2, [r10, #92]
	mov	r0, #4
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	ldr	r1, .L55+16
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r8, #8]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r2, [r10, #84]
	mov	r3, r6
	ldr	r1, .L55+20
	bfi	r3, r2, #0, #24
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r8, #12]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r10, #104]
	ldr	r1, [r10, #108]
	mov	r0, #4
	ldr	r2, [r10, #112]
	and	r3, r3, #1
	bfi	r3, r1, #1, #2
	str	r6, [fp, #-48]
	bfi	r3, r2, #3, #5
	strb	r3, [fp, #-46]
	ldr	r2, [fp, #-48]
	ldr	r1, .L55+24
	str	r2, [r8, #16]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r10, #116]
	strh	r6, [fp, #-48]	@ movhi
	mov	r0, #4
	sub	r3, r3, #1
	strh	r3, [fp, #-46]	@ movhi
	ldr	r2, [fp, #-48]
	ldr	r1, .L55+28
	str	r2, [r8, #20]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r7, #68]
	mov	r0, #4
	str	r4, [r8, #252]
	mov	r2, r4
	ldr	r1, .L55+32
	str	r4, [fp, #-48]
	blx	r3
	mov	r3, #1
	b	.L36
.L52:
	ldr	ip, [r7, #68]
	ldr	r3, .L55+36
	ldr	r2, .L55+40
	ldr	r1, .L55+44
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L56:
	.align	2
.L55:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC35
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC34
	.word	.LC16
	.word	.LANCHOR0+56
	.word	.LC17
	UNWIND(.fnend)
	.size	RV8HAL_V300R001_WriteSliceMsg, .-RV8HAL_V300R001_WriteSliceMsg
	.align	2
	.global	RV8HAL_V300R001_StartDec
	.type	RV8HAL_V300R001_StartDec, %function
RV8HAL_V300R001_StartDec:
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
	ldr	r7, .L68
	cmp	r1, #1
	mov	r2, r1
	mov	r6, r0
	add	r5, r4, r7
	bhi	.L65
	cmp	r1, #0
	bgt	.L66
	ldr	r3, [r4, r7]
	cmp	r3, #0
	beq	.L67
.L62:
	ldr	ip, .L68+4
	mov	r1, r6
	mov	r0, r5
	ldr	r3, [ip]
	add	r3, r3, #1
	str	r3, [ip]
	bl	RV8HAL_V300R001_WriteReg
	mov	r1, r6
	mov	r0, r5
	bl	RV8HAL_V300R001_WritePicMsg
	mov	r1, r6
	mov	r0, r5
	bl	RV8HAL_V300R001_WriteSliceMsg
	ldr	r3, .L68+8
	ldr	r1, .L68+12
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
.L60:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L67:
	mov	r0, #0
	str	r1, [fp, #-32]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L63
	str	r3, [r4, r7]
	ldr	r2, [fp, #-32]
	b	.L62
.L66:
	ldr	r1, .L68+8
	mov	r0, #0
	mov	r3, r2
	str	r0, [sp]
	ldr	r2, .L68+16
	ldr	ip, [r1, #68]
	ldr	r1, .L68+20
	blx	ip
	mvn	r0, #0
	b	.L60
.L65:
	ldr	r3, .L68+8
	mov	r0, #0
	ldr	r1, .L68+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L60
.L63:
	ldr	r3, .L68+8
	ldr	r1, .L68+28
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L60
.L69:
	.align	2
.L68:
	.word	g_HwMem
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC38
	.word	.LANCHOR0+88
	.word	.LC7
	.word	.LC36
	.word	.LC37
	UNWIND(.fnend)
	.size	RV8HAL_V300R001_StartDec, .-RV8HAL_V300R001_StartDec
	.align	2
	.global	RV8HAL_V300R001_StartVDH
	.type	RV8HAL_V300R001_StartVDH, %function
RV8HAL_V300R001_StartVDH:
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
	ldr	r3, .L71
	mov	r0, #2
	ldr	r1, .L71+4
	ldr	r3, [r3, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L72:
	.align	2
.L71:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC38
	UNWIND(.fnend)
	.size	RV8HAL_V300R001_StartVDH, .-RV8HAL_V300R001_StartVDH
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14193, %object
	.size	__func__.14193, 25
__func__.14193:
	.ascii	"RV8HAL_V300R001_WriteReg\000"
	.space	3
	.type	__func__.14205, %object
	.size	__func__.14205, 28
__func__.14205:
	.ascii	"RV8HAL_V300R001_WritePicMsg\000"
	.type	__func__.14218, %object
	.size	__func__.14218, 30
__func__.14218:
	.ascii	"RV8HAL_V300R001_WriteSliceMsg\000"
	.space	2
	.type	__func__.14184, %object
	.size	__func__.14184, 25
__func__.14184:
	.ascii	"RV8HAL_V300R001_StartDec\000"
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
	ASCII(.ascii	"BASIC_V300R001_CFG0=0x%x\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"BASIC_V300R001_CFG1=0x%x\012\000" )
	.space	2
.LC4:
	ASCII(.ascii	"AVM_V300R001_ADDR=0x%x\012\000" )
.LC5:
	ASCII(.ascii	"VAM_V300R001_ADDR=0x%x\012\000" )
.LC6:
	ASCII(.ascii	"STREAM_V300R001_BASE_ADDR=0x%x\012\000" )
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
	ASCII(.ascii	"D4 = 0x%x\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"D5 = 0x%x\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"D6 = 0x%x\012\000" )
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
	ASCII(.ascii	"D024 = 0x%x\012\000" )
	.space	3
.LC34:
	ASCII(.ascii	"D63 = 0x%x\012\000" )
.LC35:
	ASCII(.ascii	"configuring Slice Msg...\012\000" )
	.space	2
.LC36:
	ASCII(.ascii	"VdhId is wrong! RV8HAL_V200R003_StartDec\012\000" )
	.space	2
.LC37:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC38:
	ASCII(.ascii	"start VDM...\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
