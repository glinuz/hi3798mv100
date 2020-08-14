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
	.file	"vdm_hal_mpeg2.c"
	.text
	.align	2
	.global	MP2HAL_V300R001_InitHal
	.type	MP2HAL_V300R001_InitHal, %function
MP2HAL_V300R001_InitHal:
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
	.size	MP2HAL_V300R001_InitHal, .-MP2HAL_V300R001_InitHal
	.align	2
	.global	MP2HAL_V300R001_WriteSliceMsg
	.type	MP2HAL_V300R001_WriteSliceMsg, %function
MP2HAL_V300R001_WriteSliceMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
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
	mov	lr, r0
	add	ip, r0, #45056
	str	r0, [fp, #-76]
	ldr	r0, [r0, #284]
	mov	r9, r1
	str	r3, [fp, #-64]
	mov	r6, r2
	cmp	r0, #0
	add	r3, lr, #252
	ldr	r8, [ip, #252]
	mov	r4, #0
	str	r3, [fp, #-56]
	bne	.L25
.L3:
	cmp	r8, #0
	ble	.L22
	mov	r3, r0, asl #5
	ldr	r10, .L26
	str	r3, [fp, #-68]
	mov	r5, #0
	add	r3, r3, r6
	str	r3, [fp, #-72]
	b	.L17
.L7:
	add	r5, r5, #1
	cmp	r8, r5
	ble	.L22
.L17:
	mov	r3, #44
	ldr	r1, [fp, #-56]
	mul	r3, r3, r5
	cmp	r5, #0
	movgt	r2, #1
	movle	r2, #0
	add	r4, r1, r3
	ldr	r7, [r4, #32]
	ldr	r1, [r4, #-12]
	cmp	r7, r1
	movhi	r7, #0
	andls	r7, r2, #1
	cmp	r7, #0
	bne	.L7
	ldr	r2, [fp, #-56]
	mov	r0, r7
	ldr	lr, [r4, #8]
	mov	r1, #0
	ldr	r3, [r2, r3]
	bfi	r0, lr, #0, #24
	ldr	r2, [r4, #16]
	and	ip, r3, #15
	str	r0, [fp, #-48]
	str	r3, [fp, #-60]
	mov	r0, #4
	add	r2, r2, ip, lsl #3
	ldr	r3, [fp, #-68]
	bfi	r1, r2, #0, #7
	strb	r1, [fp, #-45]
	ldr	r2, [fp, #-48]
	add	r6, r3, r5, lsl #3
	ldr	r1, .L26+4
	add	r5, r5, #1
	str	r2, [r9, r6, asl #2]
	add	r6, r6, #1
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-64]
	mov	ip, r7
	bic	r3, r3, #15
	ldr	r1, .L26+8
	rsb	r3, r2, r3
	mov	r0, #4
	bfi	ip, r3, #0, #24
	str	ip, [r9, r6, asl #2]
	str	ip, [fp, #-48]
	mov	r6, r6, asl #2
	mov	r2, ip
	ldr	r3, [r10, #68]
	blx	r3
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #20]
	mov	r0, r7
	and	ip, r3, #15
	ldr	lr, [r4, #12]
	mov	r1, #0
	str	r3, [fp, #-60]
	add	r2, r2, ip, lsl #3
	bfi	r0, lr, #0, #24
	bfi	r1, r2, #0, #7
	str	r0, [fp, #-48]
	strb	r1, [fp, #-45]
	add	ip, r6, #4
	ldr	r2, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L26+12
	str	r2, [r9, ip]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r2, [r4, #4]
	ldr	r3, [fp, #-60]
	mov	r0, #4
	cmp	r2, #0
	ldr	r1, .L26+16
	bicne	r3, r3, #15
	streq	r2, [fp, #-48]
	ldrne	r2, [fp, #-64]
	rsbne	r3, r2, r3
	bfine	r7, r3, #0, #24
	strne	r7, [fp, #-48]
	ldr	r2, [fp, #-48]
	add	r3, r6, #8
	mov	r7, #0
	str	r2, [r9, r3]
	ldr	r3, [r10, #68]
	blx	r3
	ldr	r3, [r4, #40]
	ldr	r2, [r4, #36]
	add	ip, r6, #12
	and	r3, r3, #63
	str	r7, [fp, #-48]
	bfi	r3, r2, #6, #1
	strb	r3, [fp, #-48]
	ldr	r2, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L26+20
	str	r2, [r9, ip]
	ldr	r3, [r10, #68]
	blx	r3
	ldr	r1, [r4, #32]
	add	r3, r6, #16
	mov	r2, r7
	mov	r0, #4
	bfi	r2, r1, #0, #20
	ldr	r1, .L26+24
	str	r2, [fp, #-48]
	str	r2, [r9, r3]
	ldr	r3, [r10, #68]
	blx	r3
	cmp	r8, r5
	ble	.L10
	ldr	r2, [r4, #32]
	ldr	r3, [r4, #76]
	cmp	r2, r3
	bcs	.L11
	b	.L10
.L13:
	ldr	r3, [r4, #76]
	cmp	r3, r2
	bhi	.L15
.L11:
	add	r5, r5, #1
	add	r4, r4, #44
	cmp	r8, r5
	bne	.L13
.L19:
	ldr	r3, [fp, #-76]
	mov	r7, #0
	mov	r4, r7
	ldrh	r1, [r3, #152]
	ldrh	r3, [r3, #148]
	mul	r1, r3, r1
	sub	r1, r1, #1
.L16:
	add	r3, r6, #20
	add	r6, r6, #24
	mov	r2, #0
	mov	r0, #4
	bfi	r2, r1, #0, #20
	sub	r5, r5, #1
	str	r2, [fp, #-48]
	add	r5, r5, #1
	str	r2, [r9, r3]
	ldr	r1, .L26+28
	ldr	r3, [r10, #68]
	blx	r3
	str	r7, [r9, r6]
	mov	r2, r4
	ldr	r1, .L26+32
	mov	r0, #4
	ldr	r3, [r10, #68]
	str	r4, [fp, #-48]
	blx	r3
	cmp	r8, r5
	bgt	.L17
.L22:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L10:
	cmp	r8, r5
	beq	.L19
.L15:
	ldr	r2, [fp, #-72]
	mov	r3, #44
	add	r4, r2, r5, lsl #5
	ldr	r2, [fp, #-56]
	mov	r7, r4
	mla	r3, r3, r5, r2
	ldr	r1, [r3, #32]
	sub	r1, r1, #1
	b	.L16
.L25:
	ldr	r7, [lr, #252]
	mov	r0, #1
	ldr	r3, [lr, #268]
	mov	r2, #0
	and	r1, r7, #15
	str	r0, [fp, #-48]
	ldr	r5, .L26
	mov	r0, #4
	add	r3, r3, r1, lsl #3
	ldr	r1, .L26+4
	bfi	r2, r3, #0, #7
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	mov	r10, lr
	str	r2, [r9]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [fp, #-64]
	bic	r3, r7, #15
	ldr	r1, .L26+8
	rsb	r3, r2, r3
	mov	r0, #4
	mov	r2, r4
	bfi	r2, r3, #0, #24
	str	r2, [r9, #4]
	str	r2, [fp, #-48]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r7, [r10, #256]
	ldr	r3, [r10, #272]
	mov	r2, #0
	and	ip, r7, #15
	str	r4, [fp, #-48]
	ldr	r1, .L26+12
	mov	r0, #4
	add	r3, r3, ip, lsl #3
	bfi	r2, r3, #0, #7
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r9, #8]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r10, #256]
	ldr	r1, .L26+16
	mov	r0, #4
	cmp	r3, r4
	bicne	r3, r7, #15
	streq	r3, [fp, #-48]
	ldrne	r2, [fp, #-64]
	rsbne	r3, r2, r3
	bfine	r4, r3, #0, #24
	strne	r4, [fp, #-48]
	ldr	r2, [fp, #-48]
	mov	r4, #0
	str	r2, [r9, #12]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r7, [fp, #-76]
	str	r4, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L26+20
	ldr	r3, [r7, #292]
	ldr	r2, [r7, #288]
	and	r3, r3, #63
	bfi	r3, r2, #6, #1
	strb	r3, [fp, #-48]
	ldr	r2, [fp, #-48]
	str	r2, [r9, #16]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r1, .L26+24
	mov	r2, r4
	mov	r0, #4
	bfi	r2, r4, #0, #20
	str	r2, [r9, #20]
	str	r2, [fp, #-48]
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r7, #284]
	mov	r2, r4
	ldr	r1, .L26+28
	sub	r3, r3, #1
	mov	r0, #4
	bfi	r2, r3, #0, #20
	str	r2, [r9, #24]
	str	r2, [fp, #-48]
	ldr	r3, [r5, #68]
	blx	r3
	add	r3, r6, #32
	mov	r0, #1
	str	r3, [r9, #28]
	b	.L3
.L27:
	.align	2
.L26:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	UNWIND(.fnend)
	.size	MP2HAL_V300R001_WriteSliceMsg, .-MP2HAL_V300R001_WriteSliceMsg
	.align	2
	.global	MP2HAL_V300R001_MakeReg
	.type	MP2HAL_V300R001_MakeReg, %function
MP2HAL_V300R001_MakeReg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrh	r6, [r2, #152]
	mov	r4, #3
	ldrh	r3, [r2, #148]
	mov	lr, #1
	ldr	r5, [r0, #8]
	mov	ip, #0
	mul	r3, r3, r6
	ldr	r6, [r5]
	sub	r3, r3, #1
	bfi	r6, r3, #0, #20
	str	r6, [r5]
	ldr	r5, [r0, #8]
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	orr	r3, r3, #64
	strb	r3, [r5, #3]
	ldr	r3, [r0, #8]
	ldrb	r5, [r3, #3]	@ zero_extendqisi2
	bfc	r5, #7, #1
	strb	r5, [r3, #3]
	ldr	r3, [r0, #12]
	ldrb	r5, [r3]	@ zero_extendqisi2
	bfi	r5, r4, #0, #4
	strb	r5, [r3]
	ldr	r4, [r0, #12]
	ldr	r3, [r2, #232]
	ldrh	r5, [r4]
	mov	r3, r3, lsr #6
	bfi	r5, r3, #4, #10
	strh	r5, [r4]	@ movhi
	ldr	r4, [r0, #12]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	orr	r3, r3, #64
	strb	r3, [r4, #1]
	ldr	r3, [r0, #12]
	ldrb	r4, [r3, #1]	@ zero_extendqisi2
	bfc	r4, #7, #1
	strb	r4, [r3, #1]
	ldr	r3, [r0, #12]
	ldrh	r4, [r3, #2]
	bfi	r4, lr, #0, #12
	strh	r4, [r3, #2]	@ movhi
	ldr	lr, [r0, #12]
	ldrb	r3, [lr, #3]	@ zero_extendqisi2
	orr	r3, r3, #32
	strb	r3, [lr, #3]
	ldr	r3, [r0, #12]
	ldrb	lr, [r3, #3]	@ zero_extendqisi2
	bfc	lr, #7, #1
	strb	lr, [r3, #3]
	ldr	r3, [r0, #12]
	ldrb	lr, [r3, #3]	@ zero_extendqisi2
	bfc	lr, #4, #1
	strb	lr, [r3, #3]
	ldr	r3, [r0, #12]
	ldrb	lr, [r3, #3]	@ zero_extendqisi2
	bfc	lr, #6, #1
	strb	lr, [r3, #3]
	ldr	r3, [r0, #60]
	ldrb	lr, [r3]	@ zero_extendqisi2
	bfc	lr, #0, #1
	strb	lr, [r3]
	ldr	r3, [r0, #56]
	str	ip, [r3]
	ldr	r3, [r0, #56]
	ldr	lr, [r2, #208]
	ldrb	ip, [r3]	@ zero_extendqisi2
	bfi	ip, lr, #0, #2
	strb	ip, [r3]
	ldr	r3, [r0, #56]
	ldr	lr, [r2, #204]
	ldrb	ip, [r3]	@ zero_extendqisi2
	bfi	ip, lr, #2, #2
	strb	ip, [r3]
	ldr	ip, [r0, #16]
	ldr	r3, [r1, #48]
	bic	r3, r3, #15
	str	r3, [ip]
	ldr	ip, [r0, #20]
	ldr	r3, [r1, #32]
	bic	r3, r3, #15
	str	r3, [ip]
	ldr	r1, [r0, #28]
	ldr	r3, [r2, #200]
	bic	r3, r3, #255
	str	r3, [r1]
	ldrb	r1, [r2, #3]	@ zero_extendqisi2
	ldrh	lr, [r2, #148]
	sub	r1, r1, #1
	ldr	ip, [r0, #32]
	cmp	r1, #1
	ldrh	r1, [r2, #152]
	movls	r3, #2
	movhi	r3, #1
	mul	r3, r3, lr
	str	r1, [ip]
	ldr	ip, [r0, #36]
	ldrh	r1, [r2, #152]
	mul	r3, r1, r3
	mov	r3, r3, asl #8
	str	r3, [ip]
	ldr	r3, [r2, #176]
	cmp	r3, #0
	ldreq	ip, [r2, #164]
	ldrne	ip, [r2, #168]
	ldreq	r1, [r0, #24]
	ldreq	r3, [r2, #156]
	ldrne	r1, [r0, #24]
	mov	r0, #0
	ldrne	r3, [r2, #160]
	add	r3, r3, ip, lsr #3
	bic	r3, r3, #15
	str	r3, [r1]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2HAL_V300R001_MakeReg, .-MP2HAL_V300R001_MakeReg
	.align	2
	.global	MP2HAL_V300R001_MakeDnMsg
	.type	MP2HAL_V300R001_MakeDnMsg, %function
MP2HAL_V300R001_MakeDnMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
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
	ldrh	r3, [r2, #152]
	mov	r4, r2
	mov	r5, r0
	cmp	r3, #512
	mov	r9, r1
	bhi	.L56
	ldrh	r2, [r2, #148]
	cmp	r2, #512
	bhi	.L57
	ldr	r2, [r0]
	sub	r3, r3, #1
	ldrh	r1, [r2]
	bfi	r1, r3, #0, #9
	strh	r1, [r2]	@ movhi
	ldr	r2, [r0]
	ldrh	r3, [r4, #148]
	ldrh	r1, [r2, #2]
	sub	r3, r3, #1
	bfi	r1, r3, #0, #9
	strh	r1, [r2, #2]	@ movhi
	ldr	r3, [r0]
	ldr	r1, [r4, #180]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bfi	r2, r1, #1, #1
	strb	r2, [r3, #3]
	ldr	r3, [r0, #4]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	ldrb	r2, [r3]	@ zero_extendqisi2
	bfi	r2, r1, #0, #1
	strb	r2, [r3]
	ldr	r3, [r0, #4]
	ldrb	r1, [r4, #3]	@ zero_extendqisi2
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	bfi	r2, r1, #0, #2
	strb	r2, [r3, #1]
	ldr	r3, [r0, #4]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	bfi	r2, r1, #2, #1
	strb	r2, [r3, #1]
	ldr	r3, [r0, #4]
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	bfi	r2, r1, #0, #1
	strb	r2, [r3, #2]
	ldr	r3, [r0, #4]
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bfi	r2, r1, #0, #3
	strb	r2, [r3, #3]
	ldr	r3, [r0, #4]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	bfi	r2, r1, #3, #1
	strb	r2, [r3, #3]
	ldr	r3, [r0, #4]
	ldrb	r1, [r4, #1]	@ zero_extendqisi2
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bfi	r2, r1, #4, #1
	strb	r2, [r3, #3]
	ldr	r3, [r0, #8]
	ldrb	r1, [r4, #15]	@ zero_extendqisi2
	ldrb	r2, [r3]	@ zero_extendqisi2
	bfi	r2, r1, #0, #4
	strb	r2, [r3]
	ldr	r3, [r0, #8]
	ldrb	r1, [r4, #14]	@ zero_extendqisi2
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	bfi	r2, r1, #0, #4
	strb	r2, [r3, #1]
	ldr	r3, [r0, #8]
	ldrb	r1, [r4, #13]	@ zero_extendqisi2
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	bfi	r2, r1, #0, #4
	strb	r2, [r3, #2]
	ldr	r3, [r0, #8]
	ldrb	r1, [r4, #12]	@ zero_extendqisi2
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bfi	r2, r1, #0, #4
	strb	r2, [r3, #3]
	ldr	r3, [r0, #8]
	ldrb	r1, [r4, #6]	@ zero_extendqisi2
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bfi	r2, r1, #7, #1
	strb	r2, [r3, #3]
	ldr	r3, [r0, #12]
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	ldrb	r2, [r3]	@ zero_extendqisi2
	bfi	r2, r1, #0, #2
	strb	r2, [r3]
	ldr	r3, [r0, #12]
	ldrb	r1, [r4, #10]	@ zero_extendqisi2
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	bfi	r2, r1, #0, #1
	strb	r2, [r3, #1]
	ldr	r3, [r0, #12]
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	bfi	r2, r1, #0, #1
	strb	r2, [r3, #2]
	ldr	r3, [r0, #12]
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bfi	r2, r1, #0, #1
	strb	r2, [r3, #3]
	ldr	r2, [r0, #16]
	ldr	r3, [r4, #192]
	bic	r3, r3, #15
	str	r3, [r2]
	ldr	r2, [r0, #20]
	ldr	r3, [r4, #196]
	bic	r3, r3, #15
	str	r3, [r2]
	ldr	r2, [r0, #24]
	ldr	r3, [r4, #224]
	bic	r3, r3, #15
	str	r3, [r2]
	ldr	r2, [r0, #28]
	ldr	r3, [r4, #240]
	bic	r3, r3, #15
	str	r3, [r2]
	ldr	r0, [r4, #156]
	bl	MEM_Phy2Vir
	mov	r6, r0
	ldr	r0, [r4, #160]
	bl	MEM_Phy2Vir
	ldr	r1, [r4, #176]
	cmp	r1, #0
	mov	r8, r0
	beq	.L58
	ldr	r2, [r4, #164]
	cmp	r6, #0
	ldr	r7, [r4, #168]
	ldr	r1, [r4, #160]
	ldr	r0, [r5, #32]
	ldr	r3, [r4, #156]
	add	r7, r1, r7, lsr #3
	bic	lr, r7, #15
	add	r3, r3, r2, lsr #3
	ldr	ip, [r0]
	bic	r1, r3, #15
	and	r2, r3, #15
	rsb	r3, lr, r1
	bfi	ip, r3, #0, #24
	str	ip, [r0]
	ldr	r3, [r5, #36]
	ldr	r0, [r4, #172]
	ldr	r1, [r3]
	bfi	r1, r0, #0, #24
	str	r1, [r3]
	ldr	r1, [r5, #36]
	ldr	r3, [r4, #164]
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	and	r3, r3, #7
	add	r3, r3, r2, lsl #3
	bfi	r0, r3, #0, #7
	strb	r0, [r1, #3]
	beq	.L40
	ldr	r3, [r4, #164]
	mov	r0, #8
	ldr	r10, .L59
	ldr	r1, .L59+4
	add	ip, r6, r3, lsr #3
	ldrb	r2, [r6, r3, lsr #3]	@ zero_extendqisi2
	ldrb	lr, [ip, #7]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	str	lr, [sp, #20]
	ldrb	lr, [ip, #6]	@ zero_extendqisi2
	str	lr, [sp, #16]
	ldrb	lr, [ip, #5]	@ zero_extendqisi2
	str	lr, [sp, #12]
	ldrb	lr, [ip, #4]	@ zero_extendqisi2
	str	lr, [sp, #8]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	str	lr, [sp, #4]
	ldrb	ip, [ip, #2]	@ zero_extendqisi2
	str	ip, [sp]
	ldr	ip, [r10, #68]
	blx	ip
	ldr	r2, [r4, #172]
	ldr	r3, [r4, #164]
	mov	r0, #9
	add	r1, r2, #7
	cmp	r2, #0
	ldr	ip, [r10, #68]
	mov	r3, r3, lsr #3
	movlt	r2, r1
	sub	r3, r3, #8
	ldr	r1, .L59+8
	add	r3, r3, r2, asr #3
	add	lr, r6, r3
	ldrb	r2, [r6, r3]	@ zero_extendqisi2
	ldrb	r6, [lr, #7]	@ zero_extendqisi2
	ldrb	r3, [lr, #1]	@ zero_extendqisi2
	str	r6, [sp, #20]
	ldrb	r6, [lr, #6]	@ zero_extendqisi2
	str	r6, [sp, #16]
	ldrb	r6, [lr, #5]	@ zero_extendqisi2
	str	r6, [sp, #12]
	ldrb	r6, [lr, #4]	@ zero_extendqisi2
	str	r6, [sp, #8]
	ldrb	r6, [lr, #3]	@ zero_extendqisi2
	str	r6, [sp, #4]
	ldrb	lr, [lr, #2]	@ zero_extendqisi2
	str	lr, [sp]
	blx	ip
	ldr	r1, [r4, #172]
	ldr	r2, [r4, #156]
	mov	r0, #22
	ldr	r3, [r10, #68]
	add	r2, r1, r2
	ldr	r1, .L59+12
	blx	r3
.L40:
	ldr	r3, [r5, #40]
	and	r7, r7, #15
	cmp	r8, #0
	ldr	r2, [r3]
	bfc	r2, #0, #24
	str	r2, [r3]
	ldr	r2, [r5, #44]
	ldr	r3, [r4, #176]
	ldr	r1, [r2]
	add	r3, r3, #24
	bfi	r1, r3, #0, #24
	str	r1, [r2]
	ldr	r2, [r5, #44]
	ldr	r3, [r4, #168]
	ldrb	r1, [r2, #3]	@ zero_extendqisi2
	and	r3, r3, #7
	add	r7, r3, r7, lsl #3
	bfi	r1, r7, #0, #7
	strb	r1, [r2, #3]
	beq	.L39
	ldr	r3, [r4, #168]
	mov	r0, #8
	ldr	r6, .L59
	ldr	r1, .L59+16
	add	ip, r8, r3, lsr #3
	ldrb	r2, [r8, r3, lsr #3]	@ zero_extendqisi2
	ldr	r7, [r6, #68]
	ldrb	lr, [ip, #7]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	str	lr, [sp, #20]
	ldrb	lr, [ip, #6]	@ zero_extendqisi2
	str	lr, [sp, #16]
	ldrb	lr, [ip, #5]	@ zero_extendqisi2
	str	lr, [sp, #12]
	ldrb	lr, [ip, #4]	@ zero_extendqisi2
	str	lr, [sp, #8]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	str	lr, [sp, #4]
	ldrb	ip, [ip, #2]	@ zero_extendqisi2
	str	ip, [sp]
	blx	r7
	ldr	r2, [r4, #176]
	ldr	r3, [r4, #168]
	mov	r0, #9
	add	r1, r2, #7
	cmp	r2, #0
	ldr	ip, [r6, #68]
	mov	r3, r3, lsr #3
	movlt	r2, r1
	sub	r3, r3, #8
	ldr	r1, .L59+20
	add	r3, r3, r2, asr #3
	add	lr, r8, r3
	ldrb	r2, [r8, r3]	@ zero_extendqisi2
	ldrb	r6, [lr, #7]	@ zero_extendqisi2
	ldrb	r3, [lr, #1]	@ zero_extendqisi2
	str	r6, [sp, #20]
	ldrb	r6, [lr, #6]	@ zero_extendqisi2
	str	r6, [sp, #16]
	ldrb	r6, [lr, #5]	@ zero_extendqisi2
	str	r6, [sp, #12]
	ldrb	r6, [lr, #4]	@ zero_extendqisi2
	str	r6, [sp, #8]
	ldrb	r6, [lr, #3]	@ zero_extendqisi2
	str	r6, [sp, #4]
	ldrb	lr, [lr, #2]	@ zero_extendqisi2
	str	lr, [sp]
	blx	ip
.L39:
	add	r6, r5, #64
	mov	r1, r5
.L43:
	mov	r2, r4
	mov	r3, r1
	mov	r0, #2
.L41:
	ldr	ip, [r3, #48]
	subs	r0, r0, #1
	ldrb	lr, [r2, #80]	@ zero_extendqisi2
	add	r3, r3, #4
	add	r2, r2, #8
	strb	lr, [ip]
	ldr	ip, [r3, #44]
	ldrb	lr, [r2, #88]	@ zero_extendqisi2
	strb	lr, [ip, #1]
	ldr	ip, [r3, #44]
	ldrb	lr, [r2, #104]	@ zero_extendqisi2
	strb	lr, [ip, #2]
	ldr	ip, [r3, #44]
	ldrb	lr, [r2, #120]	@ zero_extendqisi2
	strb	lr, [ip, #3]
	ldr	ip, [r3, #172]
	ldrb	lr, [r2, #8]	@ zero_extendqisi2
	strb	lr, [ip]
	ldr	ip, [r3, #172]
	ldrb	lr, [r2, #24]	@ zero_extendqisi2
	strb	lr, [ip, #1]
	ldr	ip, [r3, #172]
	ldrb	lr, [r2, #40]	@ zero_extendqisi2
	strb	lr, [ip, #2]
	ldr	ip, [r3, #172]
	ldrb	lr, [r2, #56]	@ zero_extendqisi2
	strb	lr, [ip, #3]
	bne	.L41
	add	r1, r1, #8
	add	r4, r4, #1
	cmp	r1, r6
	bne	.L43
	ldr	r2, [r5, #304]
	ldr	r3, [r9, #1140]
	bic	r3, r3, #15
	str	r3, [r2]
.L54:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L58:
	ldr	r0, [r5, #32]
	cmp	r6, #0
	ldr	r2, [r4, #164]
	ldr	r3, [r4, #156]
	ldr	ip, [r0]
	add	r3, r3, r2, lsr #3
	bfi	ip, r1, #0, #24
	str	ip, [r0]
	ldr	r1, [r5, #36]
	and	r2, r3, #15
	ldr	r3, [r4, #172]
	ldr	r0, [r1]
	add	r3, r3, #24
	bfi	r0, r3, #0, #24
	str	r0, [r1]
	ldr	r1, [r5, #36]
	ldr	r3, [r4, #164]
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	and	r3, r3, #7
	add	r3, r3, r2, lsl #3
	bfi	r0, r3, #0, #7
	strb	r0, [r1, #3]
	beq	.L38
	ldr	r3, [r4, #164]
	mov	r0, #8
	ldr	r7, .L59
	ldr	r1, .L59+24
	add	ip, r6, r3, lsr #3
	ldrb	r2, [r6, r3, lsr #3]	@ zero_extendqisi2
	ldr	r8, [r7, #68]
	ldrb	lr, [ip, #7]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	str	lr, [sp, #20]
	ldrb	lr, [ip, #6]	@ zero_extendqisi2
	str	lr, [sp, #16]
	ldrb	lr, [ip, #5]	@ zero_extendqisi2
	str	lr, [sp, #12]
	ldrb	lr, [ip, #4]	@ zero_extendqisi2
	str	lr, [sp, #8]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	str	lr, [sp, #4]
	ldrb	ip, [ip, #2]	@ zero_extendqisi2
	str	ip, [sp]
	blx	r8
	ldr	r2, [r4, #172]
	ldr	r3, [r4, #164]
	mov	r0, #9
	add	r1, r2, #7
	cmp	r2, #0
	ldr	ip, [r7, #68]
	mov	r3, r3, lsr #3
	movlt	r2, r1
	sub	r3, r3, #8
	ldr	r1, .L59+28
	add	r3, r3, r2, asr #3
	add	lr, r6, r3
	ldrb	r2, [r6, r3]	@ zero_extendqisi2
	ldrb	r6, [lr, #7]	@ zero_extendqisi2
	ldrb	r3, [lr, #1]	@ zero_extendqisi2
	str	r6, [sp, #20]
	ldrb	r6, [lr, #6]	@ zero_extendqisi2
	str	r6, [sp, #16]
	ldrb	r6, [lr, #5]	@ zero_extendqisi2
	str	r6, [sp, #12]
	ldrb	r6, [lr, #4]	@ zero_extendqisi2
	str	r6, [sp, #8]
	ldrb	r6, [lr, #3]	@ zero_extendqisi2
	str	r6, [sp, #4]
	ldrb	lr, [lr, #2]	@ zero_extendqisi2
	str	lr, [sp]
	blx	ip
.L38:
	ldr	r3, [r5, #40]
	ldr	r2, [r3]
	bfc	r2, #0, #24
	str	r2, [r3]
	ldr	r3, [r5, #44]
	ldr	r2, [r3]
	bfc	r2, #0, #24
	str	r2, [r3]
	ldr	r3, [r5, #44]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bfc	r2, #0, #7
	strb	r2, [r3, #3]
	b	.L39
.L57:
	ldr	r1, .L59
	mov	r0, #0
	ldr	r3, .L59+32
.L55:
	ldr	ip, [r1, #68]
	ldr	r2, .L59+36
	ldr	r1, .L59+40
	blx	ip
	mvn	r0, #0
	b	.L54
.L56:
	ldr	r1, .L59
	mov	r0, #0
	ldr	r3, .L59+44
	b	.L55
.L60:
	.align	2
.L59:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC11
	.word	.LC12
	.word	.LC10
	.word	.LANCHOR0
	.word	.LC9
	.word	.LC8
	UNWIND(.fnend)
	.size	MP2HAL_V300R001_MakeDnMsg, .-MP2HAL_V300R001_MakeDnMsg
	.align	2
	.global	MP2HAL_V300R001_CfgReg
	.type	MP2HAL_V300R001_CfgReg, %function
MP2HAL_V300R001_CfgReg:
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
	cmp	r2, #1
	mov	r6, r0
	mov	r0, #0
	str	r0, [fp, #-48]
	bhi	.L102
	cmp	r2, #0
	bgt	.L103
	ldr	r0, [r1]
	cmp	r0, #0
	beq	.L104
.L66:
	movw	r0, #1208
	ldr	r5, .L106
	mul	r0, r0, r2
	mvn	lr, #0
	mov	ip, #0
	mov	r9, #3
	add	r8, r6, #45056
	ldr	r4, [r5, r0]
	str	lr, [r4, #32]
	ldrh	r10, [r6, #152]
	ldrh	r7, [r6, #148]
	ldr	r4, [fp, #-48]
	mul	r7, r7, r10
	ldr	r10, [r5, r0]
	add	r7, r7, lr
	bfi	r4, r7, #0, #20
	str	r4, [fp, #-48]
	ubfx	r7, r4, #16, #8
	mov	r4, r4, lsr #24
	orr	r7, r7, #64
	and	r4, r4, #62
	bfc	r7, #7, #1
	orr	r4, r4, #65
	strb	r7, [fp, #-46]
	bfc	r4, #1, #1
	strb	r4, [fp, #-45]
	ldr	r4, [fp, #-48]
	str	ip, [fp, #-48]
	ldrh	r7, [fp, #-46]
	str	r4, [r10, #8]
	bfi	r7, r9, #0, #12
	ldr	r10, [r6, #232]
	ubfx	r4, r7, #8, #8
	strb	r9, [fp, #-48]
	orr	r4, r4, #32
	ldrh	r9, [fp, #-48]
	and	r4, r4, #127
	mov	r10, r10, lsr #6
	bfi	r4, ip, #4, #1
	bfi	r9, r10, #4, #10
	strh	r7, [fp, #-46]	@ movhi
	strb	r4, [fp, #-45]
	uxtb	r7, r4
	ldr	r10, [r8, #260]
	ubfx	r4, r9, #8, #8
	orr	r4, r4, #64
	strh	r9, [fp, #-48]	@ movhi
	bfi	r4, ip, #7, #1
	bfi	r7, r10, #6, #1
	strb	r4, [fp, #-47]
	strb	r7, [fp, #-45]
	ldr	r4, [r5, r0]
	ldr	r7, [fp, #-48]
	str	ip, [fp, #-48]
	str	r7, [r4, #12]
	ldr	r7, [r5, r0]
	ldr	r4, [r1, #48]
	bic	r4, r4, #15
	str	r4, [r7, #16]
	ldr	r0, [r5, r0]
	ldr	r1, [r1, #32]
	bic	r1, r1, #15
	str	r1, [r0, #20]
	ldr	r4, [r8, #252]
	cmp	r4, ip
	beq	.L74
	mov	r1, r6
.L75:
	ldr	r0, [r1, #252]
	cmp	r0, #0
	beq	.L70
	ldr	r7, [r1, #260]
	bic	r0, r0, #15
	cmp	r7, #0
	ble	.L70
	cmp	lr, r0
	movcs	lr, r0
.L70:
	ldr	r0, [r1, #256]
	cmp	r0, #0
	beq	.L68
	ldr	r7, [r1, #264]
	bic	r0, r0, #15
	cmp	r7, #0
	ble	.L68
	cmp	lr, r0
	movcs	lr, r0
.L68:
	add	ip, ip, #1
	add	r1, r1, #44
	cmp	ip, r4
	bne	.L75
	cmn	lr, #1
	beq	.L74
	movw	r4, #1208
	str	lr, [r3]
	mul	r4, r4, r2
	movw	r0, #49156
	movt	r0, 63683
	str	r2, [fp, #-56]
	ldr	r3, [r5, r4]
	str	lr, [r3, #24]
	ldrh	r3, [r6, #152]
	cmp	r3, #120
	movhi	r7, #0
	movls	r7, #65536
	bl	MEM_ReadPhyWord
	ldr	r3, .L106+4
	ldr	r3, [r3]
	orr	r1, r0, r7
	add	r0, r3, #4
	bl	MEM_WritePhyWord
	ldr	r1, [r5, r4]
	movw	r3, #3075
	ldr	r2, [fp, #-56]
	movt	r3, 48
	str	r3, [r1, #60]
	ldr	r1, [r5, r4]
	str	r3, [r1, #64]
	ldr	r1, [r5, r4]
	str	r3, [r1, #68]
	ldr	r1, [r5, r4]
	str	r3, [r1, #72]
	ldr	r1, [r5, r4]
	str	r3, [r1, #76]
	ldr	r1, [r5, r4]
	str	r3, [r1, #80]
	ldr	r1, [r5, r4]
	str	r3, [r1, #84]
	ldr	r1, [r5, r4]
	ldr	r3, [r6, #200]
	bic	r3, r3, #15
	str	r3, [r1, #96]
	ldr	r0, [r5, r4]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	ldr	ip, [r6, #232]
	sub	r3, r3, #1
	ldrh	r1, [r6, #148]
	cmp	r3, #1
	str	ip, [r0, #100]
	ldrh	r0, [r6, #152]
	movls	r3, #2
	movhi	r3, #1
	cmp	r0, #0
	mul	r1, r3, r1
	ble	.L105
	mov	r3, r0, asl #4
	cmp	r3, #2048
	movle	r0, #16
	bgt	.L80
.L73:
	mov	r3, r1, asl #4
	add	ip, r1, #1
	add	r3, r3, #31
	movw	lr, #1208
	ldr	r1, [r6, #232]
	mov	ip, ip, lsr #1
	mov	r3, r3, lsr #5
	mov	r4, #0
	mul	lr, lr, r2
	str	r4, [fp, #-48]
	mul	r3, r3, r0
	mov	r0, #3
	mul	ip, r1, ip
	mov	r7, #0
	ldr	r2, [r5, lr]
	bfi	r7, r4, #0, #1
	ldr	r8, .L106+8
	add	ip, ip, r3, lsl #4
	mov	r3, r3, asl #5
	ldr	r1, .L106+12
	mov	ip, ip, asl #1
	str	ip, [r2, #104]
	ldr	ip, [r6, #208]
	mov	r2, r3
	ldr	r9, [r6, #204]
	and	ip, ip, r0
	ldr	r6, [r5, lr]
	bfi	ip, r9, #2, #2
	strb	ip, [fp, #-48]
	ldr	ip, [fp, #-48]
	str	r4, [fp, #-48]
	strb	r7, [fp, #-48]
	str	ip, [r6, #148]
	ldr	r6, [fp, #-48]
	ldr	ip, [r5, lr]
	str	r6, [ip, #152]
	ldr	ip, [r5, lr]
	str	r3, [ip, #108]
	ldr	r3, [r8, #68]
	blx	r3
	mov	r0, r4
.L98:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L105:
	mov	r3, r0, asl #4
.L80:
	sub	r0, r3, #2048
	sub	r0, r0, #1
	cmp	r0, #2048
	movcc	r0, #32
	bcc	.L73
	sub	r0, r3, #4096
	sub	r0, r0, #1
	cmp	r0, #2048
	movcc	r0, #48
	bcc	.L73
	sub	r3, r3, #6144
	sub	r3, r3, #1
	cmp	r3, #2048
	movcs	r0, #16
	movcc	r0, #64
	b	.L73
.L104:
	movt	r0, 63683
	str	r3, [fp, #-64]
	str	r2, [fp, #-60]
	str	r1, [fp, #-56]
	bl	MEM_Phy2Vir
	subs	ip, r0, #0
	beq	.L67
	ldr	r1, [fp, #-56]
	ldr	r2, [fp, #-60]
	ldr	r3, [fp, #-64]
	str	ip, [r1]
	b	.L66
.L103:
	ldr	r1, .L106+8
	mov	r3, r2
	str	r0, [sp]
	ldr	r2, .L106+16
	ldr	ip, [r1, #68]
	ldr	r1, .L106+20
	blx	ip
	mvn	r0, #0
	b	.L98
.L102:
	ldr	r3, .L106+8
	ldr	r1, .L106+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L98
.L74:
	ldr	ip, .L106+8
	mvn	r2, #0
	ldr	r1, .L106+28
	mov	r0, #0
	str	r3, [fp, #-56]
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r3, [fp, #-56]
	mov	r2, #0
	mvn	r0, #0
	str	r2, [r3]
	b	.L98
.L67:
	ldr	r3, .L106+8
	ldr	r1, .L106+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L98
.L107:
	.align	2
.L106:
	.word	g_HwMem
	.word	s_RegPhyBaseAddr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC22
	.word	.LANCHOR0+28
	.word	.LC19
	.word	.LC18
	.word	.LC21
	.word	.LC20
	UNWIND(.fnend)
	.size	MP2HAL_V300R001_CfgReg, .-MP2HAL_V300R001_CfgReg
	.align	2
	.global	MP2HAL_V300R001_CfgDnMsg
	.type	MP2HAL_V300R001_CfgDnMsg, %function
MP2HAL_V300R001_CfgDnMsg:
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
	UNWIND(.pad #44)
	sub	sp, sp, #44
	mov	r5, r0
	ldr	r0, [r1, #48]
	str	r1, [fp, #-56]
	str	r3, [fp, #-60]
	mov	r3, #0
	str	r3, [fp, #-48]
	bl	MEM_Phy2Vir
	subs	r4, r0, #0
	ldreq	r1, .L141
	ldreq	r3, .L141+4
	beq	.L135
	ldrh	r2, [r5, #152]
	cmp	r2, #512
	bhi	.L136
	ldrh	r1, [r5, #148]
	cmp	r1, #512
	bhi	.L137
	ldrh	r3, [fp, #-46]
	sub	r1, r1, #1
	ldr	r0, [r5, #180]
	sub	r2, r2, #1
	bfi	r3, r1, #0, #9
	ldrh	r1, [fp, #-48]
	strh	r3, [fp, #-46]	@ movhi
	mov	r8, #0
	mov	r3, r3, lsr #8
	bfi	r1, r2, #0, #9
	bfi	r3, r0, #1, #1
	strh	r1, [fp, #-48]	@ movhi
	strb	r3, [fp, #-45]
	mov	r7, r8
	ldr	r3, [fp, #-48]
	mov	r6, r8
	mov	lr, r8
	mov	ip, r8
	mov	r0, r8
	mov	r1, r8
	str	r3, [r4]
	ldrb	r3, [r5]	@ zero_extendqisi2
	ldrb	r9, [r5, #2]	@ zero_extendqisi2
	and	r3, r3, #7
	ldrb	r2, [r5, #3]	@ zero_extendqisi2
	bfi	r3, r9, #3, #1
	ldrb	r9, [r5, #1]	@ zero_extendqisi2
	ldrb	r10, [r5, #4]	@ zero_extendqisi2
	and	r2, r2, #3
	bfi	r3, r9, #4, #1
	strb	r3, [fp, #-45]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	mov	r9, r8
	bfi	r2, r10, #2, #1
	strb	r2, [fp, #-47]
	bfi	r8, r3, #0, #1
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	strb	r8, [fp, #-48]
	mov	r2, r7
	bfi	r7, r3, #0, #1
	strb	r7, [fp, #-46]
	ldr	r3, [fp, #-48]
	str	r3, [r4, #4]
	ldrb	r3, [r5, #12]	@ zero_extendqisi2
	ldrb	r8, [r5, #6]	@ zero_extendqisi2
	ldrb	r7, [r5, #15]	@ zero_extendqisi2
	and	r3, r3, #15
	bfi	r3, r8, #7, #1
	strb	r3, [fp, #-45]
	bfi	r6, r7, #0, #4
	ldrb	r3, [r5, #14]	@ zero_extendqisi2
	ldrb	r7, [r5, #13]	@ zero_extendqisi2
	bfi	lr, r3, #0, #4
	strb	r6, [fp, #-48]
	strb	lr, [fp, #-47]
	bfi	ip, r7, #0, #4
	strb	ip, [fp, #-46]
	ldr	r3, [fp, #-48]
	str	r3, [r4, #8]
	ldrb	r3, [r5, #11]	@ zero_extendqisi2
	bfi	r0, r3, #0, #2
	strb	r0, [fp, #-48]
	ldrb	ip, [r5, #10]	@ zero_extendqisi2
	ldrb	r0, [r5, #9]	@ zero_extendqisi2
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	bfi	r1, ip, #0, #1
	bfi	r9, r0, #0, #1
	strb	r1, [fp, #-47]
	bfi	r2, r3, #0, #1
	strb	r9, [fp, #-46]
	strb	r2, [fp, #-45]
	ldr	r3, [fp, #-48]
	str	r3, [r4, #12]
	ldr	r3, [r5, #192]
	bic	r3, r3, #15
	str	r3, [r4, #16]
	ldr	r3, [r5, #196]
	bic	r3, r3, #15
	str	r3, [r4, #20]
	ldr	r3, [r5, #224]
	bic	r3, r3, #15
	str	r3, [r4, #24]
	ldr	r3, [r5, #240]
	bic	r3, r3, #15
	str	r3, [r4, #28]
	ldr	r0, [r5, #156]
	str	r3, [fp, #-48]
	bl	MEM_Phy2Vir
	mov	r6, r0
	ldr	r0, [r5, #160]
	bl	MEM_Phy2Vir
	ldr	r3, [r5, #176]
	cmp	r3, #0
	mov	r8, r0
	beq	.L138
	ldr	r2, [r5, #164]
	mov	r1, #0
	ldr	r7, [r5, #168]
	mov	r0, r1
	ldr	ip, [r5, #160]
	cmp	r6, r1
	ldr	r3, [r5, #156]
	add	r7, ip, r7, lsr #3
	add	r3, r3, r2, lsr #3
	bic	lr, r7, #15
	bic	ip, r3, #15
	and	r2, r3, #15
	rsb	r3, lr, ip
	bfi	r1, r3, #0, #24
	str	r1, [r4, #32]
	ldr	r1, [r5, #172]
	ldr	r3, [r5, #164]
	bfi	r0, r1, #0, #24
	str	r0, [fp, #-48]
	and	r3, r3, #7
	ldrb	r1, [fp, #-45]	@ zero_extendqisi2
	add	r3, r3, r2, lsl #3
	bfi	r1, r3, #0, #7
	strb	r1, [fp, #-45]
	ldr	r3, [fp, #-48]
	str	r3, [r4, #36]
	beq	.L116
	ldr	r3, [r5, #164]
	mov	r0, #8
	ldr	r9, .L141
	ldr	r1, .L141+8
	add	ip, r6, r3, lsr #3
	ldrb	r2, [r6, r3, lsr #3]	@ zero_extendqisi2
	ldrb	lr, [ip, #7]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	str	lr, [sp, #20]
	ldrb	lr, [ip, #6]	@ zero_extendqisi2
	ldr	r10, [r9, #68]
	str	lr, [sp, #16]
	ldrb	lr, [ip, #5]	@ zero_extendqisi2
	str	lr, [sp, #12]
	ldrb	lr, [ip, #4]	@ zero_extendqisi2
	str	lr, [sp, #8]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	str	lr, [sp, #4]
	ldrb	ip, [ip, #2]	@ zero_extendqisi2
	str	ip, [sp]
	blx	r10
	ldr	r2, [r5, #172]
	ldr	r3, [r5, #164]
	mov	r0, #9
	add	r1, r2, #7
	cmp	r2, #0
	ldr	r10, [r9, #68]
	mov	r3, r3, lsr #3
	movlt	r2, r1
	sub	r3, r3, #8
	ldr	r1, .L141+12
	add	r3, r3, r2, asr #3
	add	ip, r6, r3
	ldrb	r2, [r6, r3]	@ zero_extendqisi2
	ldrb	lr, [ip, #7]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	str	lr, [sp, #20]
	ldrb	lr, [ip, #6]	@ zero_extendqisi2
	str	lr, [sp, #16]
	ldrb	lr, [ip, #5]	@ zero_extendqisi2
	str	lr, [sp, #12]
	ldrb	lr, [ip, #4]	@ zero_extendqisi2
	str	lr, [sp, #8]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	str	lr, [sp, #4]
	ldrb	ip, [ip, #2]	@ zero_extendqisi2
	str	ip, [sp]
	blx	r10
	ldr	r1, [r5, #172]
	ldr	r2, [r5, #156]
	mov	r0, #22
	ldr	r3, [r9, #68]
	add	r2, r1, r2
	ldr	r1, .L141+16
	blx	r3
.L116:
	mov	r3, #0
	and	r7, r7, #15
	mov	r2, r3
	bfi	r2, r3, #0, #24
	str	r2, [r4, #40]
	cmp	r8, r3
	ldr	r1, [r5, #176]
	ldr	r2, [r5, #168]
	add	r1, r1, #24
	bfi	r3, r1, #0, #24
	str	r3, [fp, #-48]
	and	r3, r2, #7
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	add	r7, r3, r7, lsl #3
	bfi	r2, r7, #0, #7
	strb	r2, [fp, #-45]
	ldr	r3, [fp, #-48]
	str	r3, [r4, #44]
	beq	.L115
	ldr	r3, [r5, #168]
	mov	r0, #8
	ldr	r6, .L141
	ldr	r1, .L141+20
	add	ip, r8, r3, lsr #3
	ldrb	r2, [r8, r3, lsr #3]	@ zero_extendqisi2
	ldr	r7, [r6, #68]
	ldrb	lr, [ip, #7]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	str	lr, [sp, #20]
	ldrb	lr, [ip, #6]	@ zero_extendqisi2
	str	lr, [sp, #16]
	ldrb	lr, [ip, #5]	@ zero_extendqisi2
	str	lr, [sp, #12]
	ldrb	lr, [ip, #4]	@ zero_extendqisi2
	str	lr, [sp, #8]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	str	lr, [sp, #4]
	ldrb	ip, [ip, #2]	@ zero_extendqisi2
	str	ip, [sp]
	blx	r7
	ldr	r2, [r5, #176]
	ldr	r3, [r5, #168]
	mov	r0, #9
	add	r1, r2, #7
	cmp	r2, #0
	ldr	ip, [r6, #68]
	mov	r3, r3, lsr #3
	movlt	r2, r1
	sub	r3, r3, #8
	ldr	r1, .L141+24
	add	r3, r3, r2, asr #3
	add	lr, r8, r3
	ldrb	r2, [r8, r3]	@ zero_extendqisi2
	ldrb	r6, [lr, #7]	@ zero_extendqisi2
	ldrb	r3, [lr, #1]	@ zero_extendqisi2
	str	r6, [sp, #20]
	ldrb	r6, [lr, #6]	@ zero_extendqisi2
	str	r6, [sp, #16]
	ldrb	r6, [lr, #5]	@ zero_extendqisi2
	str	r6, [sp, #12]
	ldrb	r6, [lr, #4]	@ zero_extendqisi2
	str	r6, [sp, #8]
	ldrb	r6, [lr, #3]	@ zero_extendqisi2
	str	r6, [sp, #4]
	ldrb	lr, [lr, #2]	@ zero_extendqisi2
	str	lr, [sp]
	blx	ip
.L115:
	mov	r9, r5
	mov	r1, #64
.L121:
	add	r8, r1, #64
	mov	r3, r9
	mov	r2, #0
.L117:
	ldrb	r7, [r3, #96]	@ zero_extendqisi2
	add	ip, r1, r2
	ldrb	r6, [r3, #112]	@ zero_extendqisi2
	add	r0, r8, r2
	ldrb	lr, [r3, #128]	@ zero_extendqisi2
	add	r2, r2, #4
	ldrb	r10, [r3, #80]	@ zero_extendqisi2
	cmp	r2, #8
	strb	r7, [fp, #-47]
	add	r3, r3, #8
	strb	r6, [fp, #-46]
	strb	r10, [fp, #-48]
	strb	lr, [fp, #-45]
	ldr	lr, [fp, #-48]
	str	lr, [r4, ip]
	ldrb	ip, [r3, #56]	@ zero_extendqisi2
	ldrb	r7, [r3, #8]	@ zero_extendqisi2
	ldrb	r6, [r3, #24]	@ zero_extendqisi2
	ldrb	lr, [r3, #40]	@ zero_extendqisi2
	strb	r7, [fp, #-48]
	strb	r6, [fp, #-47]
	strb	lr, [fp, #-46]
	strb	ip, [fp, #-45]
	ldr	ip, [fp, #-48]
	str	ip, [r4, r0]
	bne	.L117
	add	r1, r1, #8
	add	r9, r9, #1
	cmp	r1, #128
	bne	.L121
	ldr	r2, [fp, #-56]
	ldr	r3, [r2, #1140]
	bic	r3, r3, #15
	str	r3, [r4, #192]
	ldr	r6, [r2, #48]
	str	r3, [fp, #-48]
	bic	r6, r6, #15
	add	r6, r6, #256
	mov	r0, r6
	bl	MEM_Phy2Vir
	subs	r1, r0, #0
	beq	.L139
	ldr	r3, [fp, #-60]
	mov	r0, r5
	str	r6, [r4, #252]
	mov	r2, r6
	str	r6, [fp, #-48]
	bl	MP2HAL_V300R001_WriteSliceMsg
	ldr	r3, .L141+28
	ldr	r3, [r3]
	ands	r0, r3, #16
	bne	.L140
.L134:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L138:
	ldr	ip, [r5, #164]
	mov	r0, r3
	ldr	r2, [r5, #156]
	cmp	r6, #0
	str	r3, [r4, #32]
	ldr	r1, [r5, #172]
	add	r2, r2, ip, lsr #3
	ldr	r3, [r5, #164]
	and	r2, r2, #15
	add	r1, r1, #24
	bfi	r0, r1, #0, #24
	and	r3, r3, #7
	str	r0, [fp, #-48]
	add	r3, r3, r2, lsl #3
	ldrb	r1, [fp, #-45]	@ zero_extendqisi2
	bfi	r1, r3, #0, #7
	strb	r1, [fp, #-45]
	ldr	r3, [fp, #-48]
	str	r3, [r4, #36]
	beq	.L114
	ldr	r3, [r5, #164]
	mov	r0, #8
	ldr	r7, .L141
	ldr	r1, .L141+32
	add	ip, r6, r3, lsr #3
	ldrb	r2, [r6, r3, lsr #3]	@ zero_extendqisi2
	ldr	r8, [r7, #68]
	ldrb	lr, [ip, #7]	@ zero_extendqisi2
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	str	lr, [sp, #20]
	ldrb	lr, [ip, #6]	@ zero_extendqisi2
	str	lr, [sp, #16]
	ldrb	lr, [ip, #5]	@ zero_extendqisi2
	str	lr, [sp, #12]
	ldrb	lr, [ip, #4]	@ zero_extendqisi2
	str	lr, [sp, #8]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	str	lr, [sp, #4]
	ldrb	ip, [ip, #2]	@ zero_extendqisi2
	str	ip, [sp]
	blx	r8
	ldr	r2, [r5, #172]
	ldr	r3, [r5, #164]
	mov	r0, #9
	add	r1, r2, #7
	cmp	r2, #0
	ldr	ip, [r7, #68]
	mov	r3, r3, lsr #3
	movlt	r2, r1
	sub	r3, r3, #8
	ldr	r1, .L141+36
	add	r3, r3, r2, asr #3
	add	lr, r6, r3
	ldrb	r2, [r6, r3]	@ zero_extendqisi2
	ldrb	r6, [lr, #7]	@ zero_extendqisi2
	ldrb	r3, [lr, #1]	@ zero_extendqisi2
	str	r6, [sp, #20]
	ldrb	r6, [lr, #6]	@ zero_extendqisi2
	str	r6, [sp, #16]
	ldrb	r6, [lr, #5]	@ zero_extendqisi2
	str	r6, [sp, #12]
	ldrb	r6, [lr, #4]	@ zero_extendqisi2
	str	r6, [sp, #8]
	ldrb	r6, [lr, #3]	@ zero_extendqisi2
	str	r6, [sp, #4]
	ldrb	lr, [lr, #2]	@ zero_extendqisi2
	str	lr, [sp]
	blx	ip
.L114:
	mov	r3, #0
	mov	r2, #0
	mov	r0, r3
	mov	r1, r3
	bfi	r0, r3, #0, #24
	bfi	r1, r3, #0, #24
	str	r0, [r4, #40]
	bfi	r2, r3, #0, #7
	str	r1, [fp, #-48]
	strb	r2, [fp, #-45]
	ldr	r3, [fp, #-48]
	str	r3, [r4, #44]
	b	.L115
.L140:
	ldr	ip, .L141+40
	mov	r0, #4
	ldr	r7, .L141
	mov	r6, r4
	ldr	r3, [fp, #-56]
	mov	r5, #0
	ldr	r2, [ip]
	ldr	r8, [r7, #68]
	add	r2, r2, #1
	ldr	r3, [r3, #48]
	ldr	r1, .L141+44
	str	r2, [ip]
	blx	r8
.L120:
	ldr	r1, [r6, #12]
	mov	r2, r5
	ldr	r3, [r4, r5, asl #2]
	mov	r0, #4
	ldr	ip, [r7, #68]
	add	r5, r5, r0
	str	r1, [sp, #8]
	add	r6, r6, #16
	ldr	lr, [r6, #-8]
	ldr	r1, .L141+48
	ldr	r8, .L141
	str	lr, [sp, #4]
	ldr	lr, [r6, #-12]
	str	lr, [sp]
	blx	ip
	cmp	r5, #64
	bne	.L120
	ldr	r3, [r8, #68]
	mov	r0, #4
	ldr	r1, .L141+52
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L136:
	ldr	r1, .L141
	mov	r0, #0
	ldr	r3, .L141+56
.L135:
	ldr	ip, [r1, #68]
	ldr	r2, .L141+60
	ldr	r1, .L141+64
	blx	ip
	mvn	r0, #0
	b	.L134
.L137:
	ldr	r1, .L141
	mov	r0, #0
	ldr	r3, .L141+68
	b	.L135
.L139:
	ldr	r3, .L141
	ldr	r1, .L141+72
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L134
.L142:
	.align	2
.L141:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC23
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	g_PrintEnable
	.word	.LC11
	.word	.LC12
	.word	.LANCHOR1
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC8
	.word	.LANCHOR0+52
	.word	.LC9
	.word	.LC10
	.word	.LC24
	UNWIND(.fnend)
	.size	MP2HAL_V300R001_CfgDnMsg, .-MP2HAL_V300R001_CfgDnMsg
	.align	2
	.global	MP2HAL_V300R001_StartDec
	.type	MP2HAL_V300R001_StartDec, %function
MP2HAL_V300R001_StartDec:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	movw	r5, #1208
	mul	r5, r5, r1
	ldr	r6, .L156
	cmp	r1, #1
	mov	r4, r1
	mov	r7, r0
	add	r8, r5, r6
	bhi	.L152
	cmp	r1, #0
	bgt	.L153
	ldr	r3, [r5, r6]
	cmp	r3, #0
	beq	.L154
.L148:
	ldr	lr, .L156+4
	sub	r3, fp, #40
	mov	r2, r4
	mov	r1, r8
	mov	r0, r7
	ldr	ip, [lr]
	add	ip, ip, #1
	str	ip, [lr]
	bl	MP2HAL_V300R001_CfgReg
	subs	r5, r0, #0
	bne	.L155
	mov	r2, r4
	mov	r1, r8
	mov	r0, r7
	ldr	r3, [fp, #-40]
	bl	MP2HAL_V300R001_CfgDnMsg
.L146:
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L154:
	mov	r0, #0
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	beq	.L149
	str	r3, [r5, r6]
	b	.L148
.L153:
	ldr	r1, .L156+8
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L156+12
	mvn	r5, #0
	ldr	ip, [r1, #68]
	ldr	r1, .L156+16
	blx	ip
	b	.L146
.L155:
	ldr	r3, .L156+8
	mov	r0, #1
	ldr	r1, .L156+20
	mvn	r5, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L146
.L152:
	ldr	r3, .L156+8
	mov	r0, #0
	ldr	r1, .L156+24
	mvn	r5, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L146
.L149:
	ldr	r3, .L156+8
	mvn	r5, #0
	ldr	r1, .L156+28
	ldr	r3, [r3, #68]
	blx	r3
	b	.L146
.L157:
	.align	2
.L156:
	.word	g_HwMem
	.word	.LANCHOR2
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+80
	.word	.LC19
	.word	.LC29
	.word	.LC28
	.word	.LC20
	UNWIND(.fnend)
	.size	MP2HAL_V300R001_StartDec, .-MP2HAL_V300R001_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14091, %object
	.size	__func__.14091, 26
__func__.14091:
	.ascii	"MP2HAL_V300R001_MakeDnMsg\000"
	.space	2
	.type	__func__.14121, %object
	.size	__func__.14121, 23
__func__.14121:
	.ascii	"MP2HAL_V300R001_CfgReg\000"
	.space	1
	.type	__func__.14144, %object
	.size	__func__.14144, 25
__func__.14144:
	.ascii	"MP2HAL_V300R001_CfgDnMsg\000"
	.space	3
	.type	__func__.14047, %object
	.size	__func__.14047, 25
__func__.14047:
	.ascii	"MP2HAL_V300R001_StartDec\000"
	.data
	.align	2
.LANCHOR2 = . + 0
	.type	FrameNum, %object
	.size	FrameNum, 4
FrameNum:
	.word	-1
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"D0 = %#x \012\000" )
	.space	1
.LC1:
	ASCII(.ascii	"D1 = %#x \012\000" )
	.space	1
.LC2:
	ASCII(.ascii	"D2 = %#x \012\000" )
	.space	1
.LC3:
	ASCII(.ascii	"D3 = %#x \012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"D4 = %#x \012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"D5 = %#x \012\000" )
	.space	1
.LC6:
	ASCII(.ascii	"D6 = %#x \012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"D7 = %#x \012\000" )
	.space	1
.LC8:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC9:
	ASCII(.ascii	"%s: %s\012\000" )
.LC10:
	ASCII(.ascii	"picture height out of range\000" )
.LC11:
	ASCII(.ascii	"Stream Head (8bytes): 0x%02x 0x%02x 0x%02x 0x%02x 0" )
	ASCII(.ascii	"x%02x 0x%02x 0x%02x 0x%02x\012\000" )
	.space	1
.LC12:
	ASCII(.ascii	"Stream Tail (8bytes): 0x%02x 0x%02x 0x%02x 0x%02x 0" )
	ASCII(.ascii	"x%02x 0x%02x 0x%02x 0x%02x\012\000" )
	.space	1
.LC13:
	ASCII(.ascii	"1p Stream Head (8bytes): 0x%02x 0x%02x 0x%02x 0x%02" )
	ASCII(.ascii	"x 0x%02x 0x%02x 0x%02x 0x%02x\012\000" )
	.space	2
.LC14:
	ASCII(.ascii	"1p Stream Tail (8bytes): 0x%02x 0x%02x 0x%02x 0x%02" )
	ASCII(.ascii	"x 0x%02x 0x%02x 0x%02x 0x%02x\012\000" )
	.space	2
.LC15:
	ASCII(.ascii	"1p last phy addr = 0x%x\012\000" )
	.space	3
.LC16:
	ASCII(.ascii	"2p Stream Head (8bytes): 0x%02x 0x%02x 0x%02x 0x%02" )
	ASCII(.ascii	"x 0x%02x 0x%02x 0x%02x 0x%02x\012\000" )
	.space	2
.LC17:
	ASCII(.ascii	"2p Stream Tail (8bytes): 0x%02x 0x%02x 0x%02x 0x%02" )
	ASCII(.ascii	"x 0x%02x 0x%02x 0x%02x 0x%02x\012\000" )
	.space	2
.LC18:
	ASCII(.ascii	"VdhId is wrong! MP2HAL_V200R003_CfgReg\012\000" )
.LC19:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC20:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
.LC21:
	ASCII(.ascii	"stream_base_addr = %#x\012\000" )
.LC22:
	ASCII(.ascii	"HEAD_INF_OFFSET = 0x%x\012\000" )
.LC23:
	ASCII(.ascii	"can not map down msg virtual address!\000" )
	.space	2
.LC24:
	ASCII(.ascii	"Map SlcDnMsgPhyAddr to SlcDnMsgVirAddr is NULL \012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC25:
	ASCII(.ascii	"\012*****No.%2d Down Msg (phy addr: %#8x) *****\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC26:
	ASCII(.ascii	"\0120x%02x 0x%08x 0x%08x 0x%08x 0x%08x\012\000" )
	.space	3
.LC27:
	ASCII(.ascii	"\012***** Down Msg print finished *****\012\000" )
	.space	2
.LC28:
	ASCII(.ascii	"VdhId is wrong! MP2HAL_V200R003_StartDec\012\000" )
	.space	2
.LC29:
	ASCII(.ascii	"MP2HAL_V200R003_CfgReg ERROR!\012\000" )
	.bss
	.align	2
.LANCHOR1 = . + 0
	.type	num.14155, %object
	.size	num.14155, 4
num.14155:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
