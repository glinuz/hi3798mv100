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
	.file	"vdm_hal_real9.c"
	.text
	.align	2
	.global	RV9HAL_V400R003_InitHal
	.type	RV9HAL_V400R003_InitHal, %function
RV9HAL_V400R003_InitHal:
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
	.size	RV9HAL_V400R003_InitHal, .-RV9HAL_V400R003_InitHal
	.align	2
	.global	RV9HAL_V400R003_WriteReg
	.type	RV9HAL_V400R003_WriteReg, %function
RV9HAL_V400R003_WriteReg:
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
	UNWIND(.pad #16)
	sub	sp, sp, #16
	mov	r8, r0
	mov	r6, r1
	mov	r0, #2
	ldr	r1, .L9
	mov	r4, r2
	mov	r5, r3
	bl	dprint_vfmw
	cmp	r4, #0
	ble	.L3
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L9+4
	ldr	r1, .L9+8
	bl	dprint_vfmw
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3:
	ldr	r0, [r6, #4]
	mov	r7, #0
	ldr	ip, [r6, #8]
	mov	r1, r7
	mov	lr, #1
	bfi	lr, r7, #1, #1
	mov	r3, r5
	mov	r2, r4
	mul	ip, r0, ip
	mov	r0, #8
	sub	ip, ip, #1
	bfi	r1, ip, #0, #20
	str	r1, [fp, #-40]
	strb	lr, [fp, #-37]
	mov	r1, r1, lsr #16
	bfi	r1, r7, #7, #1
	strb	r1, [fp, #-38]
	ldr	r9, [fp, #-40]
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+12
	mov	r0, #3
	bl	dprint_vfmw
	str	r7, [fp, #-40]
	ldrh	r1, [fp, #-38]
	mov	r0, #9
	ldr	ip, [r6, #80]
	mov	r3, #0
	ldr	r2, [r6, #60]
	bfi	r1, r7, #0, #12
	strb	r0, [fp, #-40]
	bfi	r3, ip, #4, #1
	strb	r3, [fp, #-39]
	mov	r3, r1, lsr #8
	ldrh	r0, [fp, #-40]
	mov	r2, r2, lsr #6
	and	r3, r3, #239
	strh	r1, [fp, #-38]	@ movhi
	bfi	r0, r2, #4, #7
	bfi	r3, r7, #5, #1
	strh	r0, [fp, #-40]	@ movhi
	mov	r2, r4
	strb	r3, [fp, #-37]
	mov	r0, #12
	ldr	r9, [r6, #36]
	mov	r3, r5
	ldrb	r1, [fp, #-39]	@ zero_extendqisi2
	ldr	lr, [r6, #76]
	ldrb	ip, [fp, #-37]	@ zero_extendqisi2
	bfi	r1, r9, #6, #1
	bfi	ip, lr, #6, #1
	mvn	r1, r1, asl #25
	mvn	r1, r1, lsr #25
	strb	r1, [fp, #-39]
	mov	r1, ip
	bfi	r1, r7, #7, #1
	strb	r1, [fp, #-37]
	ldr	r9, [fp, #-40]
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+16
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r8, #56]
	mov	r3, r5
	mov	r2, r4
	bic	r9, r9, #15
	mov	r0, #16
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+20
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r8, #40]
	mov	r3, r5
	mov	r2, r4
	bic	r9, r9, #15
	mov	r0, #20
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+24
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #40]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #24
	mov	r1, r9
	str	r9, [fp, #-40]
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+28
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r1, [r6, #4]
	mov	r3, r5
	mov	r2, r4
	cmp	r1, #256
	mov	r0, #4
	movhi	r1, #0
	movls	r1, #1
	bl	SCD_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #60
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #64
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #68
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #72
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #76
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #80
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r1, #3075
	mov	r3, r5
	mov	r2, r4
	mov	r0, #84
	movt	r1, 48
	bl	MFDE_ConfigReg
	movw	r2, #3075
	movt	r2, 48
	ldr	r1, .L9+32
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #52]
	mov	r3, r5
	mov	r2, r4
	bic	r9, r9, #15
	mov	r0, #96
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+36
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #60]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #100
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+40
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #84]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #104
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L9+44
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r6, #92]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #108
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L9+48
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r8, #1204]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #144
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L9+52
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r5
	mov	r1, r7
	mov	r2, r4
	mov	r0, #148
	str	r7, [fp, #-40]
	bl	MFDE_ConfigReg
	mov	r2, r7
	ldr	r1, .L9+56
	mov	r0, #3
	bl	dprint_vfmw
	ldrb	r1, [fp, #-40]	@ zero_extendqisi2
	mov	r3, r5
	mov	r2, r4
	bfi	r1, r7, #0, #1
	strb	r1, [fp, #-40]
	ldr	r6, [fp, #-40]
	mov	r0, #152
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L9+60
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r5
	mov	r2, r4
	mvn	r1, #0
	mov	r0, #32
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MFDE_ConfigReg
.L10:
	.align	2
.L9:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC2
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
	.word	.LC13
	.word	.LC14
	UNWIND(.fnend)
	.size	RV9HAL_V400R003_WriteReg, .-RV9HAL_V400R003_WriteReg
	.align	2
	.global	RV9HAL_V400R003_WritePicMsg
	.type	RV9HAL_V400R003_WritePicMsg, %function
RV9HAL_V400R003_WritePicMsg:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	mov	r7, r0
	mov	r5, r1
	mov	r0, #2
	ldr	r1, .L16
	bl	dprint_vfmw
	ldr	r0, [r7, #56]
	bic	r0, r0, #15
	bl	MEM_Phy2Vir
	subs	r4, r0, #0
	beq	.L15
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	mov	r6, #0
	mov	r3, #0
	str	r6, [fp, #-32]
	bfi	r3, r2, #0, #2
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	mov	r0, #4
	ldr	r1, .L16+4
	str	r2, [r4]
	bl	dprint_vfmw
	ldmib	r5, {r2, r3}
	mov	r0, #0	@ movhi
	sub	r3, r3, #1
	sub	r2, r2, #1
	mov	r1, r0	@ movhi
	bfi	r0, r2, #0, #9
	bfi	r1, r3, #0, #9
	strh	r0, [fp, #-32]	@ movhi
	strh	r1, [fp, #-30]	@ movhi
	mov	r0, #4
	ldr	r2, [fp, #-32]
	ldr	r1, .L16+8
	str	r2, [r4, #4]
	bl	dprint_vfmw
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	mov	r0, #4
	ldr	r1, .L16+12
	strh	r2, [fp, #-32]	@ movhi
	strh	r3, [fp, #-30]	@ movhi
	ldr	r2, [fp, #-32]
	str	r2, [r4, #8]
	bl	dprint_vfmw
	str	r6, [fp, #-32]
	mov	r3, #0
	bfi	r3, r6, #1, #3
	strb	r3, [fp, #-32]
	mov	r0, #4
	ldr	r2, [fp, #-32]
	ldr	r1, .L16+16
	str	r2, [r4, #12]
	bl	dprint_vfmw
	ldr	r2, [r5, #20]
	str	r6, [fp, #-32]
	mov	r3, #0
	bfi	r3, r2, #0, #5
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	mov	r0, #4
	ldr	r1, .L16+20
	str	r2, [r4, #16]
	bl	dprint_vfmw
	ldr	r2, [r5, #24]
	str	r6, [fp, #-32]
	mov	r3, #0
	bfi	r3, r2, #0, #5
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	mov	r0, #4
	ldr	r1, .L16+24
	str	r2, [r4, #20]
	bl	dprint_vfmw
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #4]
	mov	r0, #4
	str	r6, [fp, #-32]
	ldr	r1, .L16+28
	mul	r3, r3, r2
	cmp	r3, #99
	ldrh	r3, [fp, #-32]
	movls	r2, #1
	bfihi	r3, r6, #5, #6
	bfils	r3, r2, #5, #6
	strh	r3, [fp, #-32]	@ movhi
	ldr	r2, [r5, #28]
	ldrb	r3, [fp, #-32]	@ zero_extendqisi2
	bfi	r3, r2, #0, #5
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	str	r2, [r4, #24]
	bl	dprint_vfmw
	ldr	r2, [r5, #56]
	ldr	r1, .L16+32
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r4, #64]
	bl	dprint_vfmw
	ldr	r2, [r5, #48]
	ldr	r1, .L16+36
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r4, #68]
	bl	dprint_vfmw
	ldr	r2, [r5, #44]
	ldr	r1, .L16+40
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r4, #72]
	bl	dprint_vfmw
	ldr	r2, [r5, #68]
	ldr	r1, .L16+44
	mov	r0, #4
	str	r2, [r4, #76]
	bl	dprint_vfmw
	ldr	r2, [r5, #72]
	ldr	r1, .L16+48
	mov	r0, #4
	str	r2, [r4, #80]
	bl	dprint_vfmw
	ldr	r2, [r7, #1144]
	ldr	r1, .L16+52
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r4, #84]
	bl	dprint_vfmw
	ldr	r2, [r7, #1148]
	ldr	r1, .L16+56
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r4, #88]
	bl	dprint_vfmw
	ldr	r2, [r7, #1152]
	ldr	r1, .L16+60
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r4, #92]
	bl	dprint_vfmw
	ldr	r2, [r7, #1160]
	ldr	r1, .L16+64
	mov	r0, #4
	str	r2, [r4, #96]
	bl	dprint_vfmw
	ldr	r2, [r7, #56]
	ldr	r1, .L16+68
	mov	r0, #4
	bic	r2, r2, #15
	add	r2, r2, #256
	str	r2, [r4, #252]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L15:
	ldr	r3, .L16+72
	ldr	r2, .L16+76
	ldr	r1, .L16+80
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L17:
	.align	2
.L16:
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
	.size	RV9HAL_V400R003_WritePicMsg, .-RV9HAL_V400R003_WritePicMsg
	.align	2
	.global	RV9HAL_V400R003_WriteSliceMsg
	.type	RV9HAL_V400R003_WriteSliceMsg, %function
RV9HAL_V400R003_WriteSliceMsg:
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
	mov	r4, r0
	mov	r10, r1
	mov	r0, #2
	ldr	r1, .L39
	add	r9, r10, #96
	bl	dprint_vfmw
	ldr	r4, [r4, #56]
	bic	r4, r4, #15
	add	r5, r4, #256
	mov	r0, r5
	bl	MEM_Phy2Vir
	subs	r7, r0, #0
	beq	.L36
	ldr	r3, [r10, #132]
	cmp	r3, #0
	bne	.L37
.L20:
	ldr	r2, [r10, #32]
	cmp	r2, #0
	beq	.L18
	mov	r3, r3, asl #6
	mov	r4, #0
	str	r3, [fp, #-60]
	add	r3, r3, r5
	str	r3, [fp, #-64]
.L30:
	cmp	r4, #0
	beq	.L33
	mov	r5, #44
	mla	r5, r5, r4, r9
	ldr	r2, [r5, #36]
	ldr	r3, [r5, #-8]
	cmp	r2, r3
	ble	.L24
.L23:
	ldr	r2, [r5, #16]
	mov	r8, #0
	ldr	ip, [r5, #8]
	mov	r3, r8
	bfi	r3, r2, #0, #24
	str	r3, [fp, #-48]
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	mov	r0, #4
	ldr	r3, [fp, #-60]
	bfi	r2, ip, #0, #7
	strb	r2, [fp, #-45]
	ldr	r2, [fp, #-48]
	add	r3, r3, r4, lsl #6
	ldr	r1, .L39+4
	add	r4, r4, #1
	mov	r6, r3, asl #2
	str	r2, [r7, r3, asl #2]
	bl	dprint_vfmw
	ldr	r1, [r5]
	add	r3, r6, #4
	mov	r2, r8
	mov	r0, #4
	bfi	r2, r1, #0, #24
	ldr	r1, .L39+8
	str	r2, [fp, #-48]
	str	r2, [r7, r3]
	bl	dprint_vfmw
	ldr	r1, [r5, #20]
	ldr	r2, [r5, #12]
	mov	r3, r8
	bfi	r3, r1, #0, #24
	str	r3, [fp, #-48]
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	add	ip, r6, #8
	ldr	r1, .L39+12
	mov	r0, #4
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r7, ip]
	bl	dprint_vfmw
	ldr	r1, [r5, #4]
	add	r3, r6, #12
	mov	r2, r8
	mov	r0, #4
	bfi	r2, r1, #0, #24
	ldr	r1, .L39+16
	str	r2, [fp, #-48]
	str	r2, [r7, r3]
	bl	dprint_vfmw
	ldr	r3, [r5, #24]
	ldr	r1, [r5, #28]
	add	ip, r6, #16
	ldr	r2, [r5, #32]
	and	r3, r3, #1
	bfi	r3, r1, #1, #2
	str	r8, [fp, #-48]
	bfi	r3, r2, #3, #5
	strb	r3, [fp, #-46]
	ldr	r2, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L39+20
	str	r2, [r7, ip]
	bl	dprint_vfmw
	ldr	r2, [r10, #32]
	ldr	r1, [r5, #36]
	cmp	r4, r2
	str	r8, [fp, #-48]
	strh	r1, [fp, #-48]	@ movhi
	bcs	.L25
	mov	r3, #44
	mul	r3, r3, r4
	add	r0, r9, r3
	ldr	r0, [r0, #36]
	cmp	r1, r0
	blt	.L25
	sub	r3, r3, #44
	add	r3, r9, r3
	b	.L26
.L27:
	ldr	r0, [r3, #80]
	cmp	r1, r0
	blt	.L25
.L26:
	add	r4, r4, #1
	add	r3, r3, #44
	cmp	r4, r2
	bcc	.L27
.L25:
	cmp	r2, r4
	beq	.L38
	mov	r2, #44
	ldr	r3, [fp, #-64]
	mla	r2, r2, r4, r9
	add	r8, r3, r4, lsl #8
	mov	r3, r8
	ldr	r2, [r2, #36]
	sub	r2, r2, #1
	str	r2, [r5, #40]
.L29:
	strh	r2, [fp, #-46]	@ movhi
	add	ip, r6, #20
	ldr	r2, [fp, #-48]
	mov	r0, #4
	ldr	r1, .L39+24
	add	r6, r6, #252
	str	r3, [fp, #-56]
	sub	r4, r4, #1
	str	r2, [r7, ip]
	bl	dprint_vfmw
	ldr	r3, [fp, #-56]
	mov	r2, r8
	ldr	r1, .L39+28
	mov	r0, #4
	str	r8, [fp, #-48]
	str	r3, [r7, r6]
	bl	dprint_vfmw
.L24:
	ldr	r3, [r10, #32]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L30
.L18:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L33:
	mov	r5, r9
	b	.L23
.L38:
	ldr	r1, [r10, #8]
	mov	r3, #0
	ldr	r2, [r10, #4]
	mov	r8, r3
	mul	r2, r2, r1
	sub	r2, r2, #1
	str	r2, [r5, #40]
	b	.L29
.L37:
	mov	r6, #0
	ldr	r2, [r10, #104]
	mov	r3, r6
	bfi	r3, r6, #0, #24
	str	r3, [fp, #-48]
	mov	r0, #4
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	add	r4, r4, #512
	ldr	r1, .L39+4
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r7]
	bl	dprint_vfmw
	ldr	r2, [r10, #96]
	mov	r3, r6
	ldr	r1, .L39+8
	bfi	r3, r2, #0, #24
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r7, #4]
	bl	dprint_vfmw
	mov	r3, r6
	bfi	r3, r6, #0, #24
	str	r3, [fp, #-48]
	ldr	r2, [r10, #108]
	mov	r0, #4
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	ldr	r1, .L39+12
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-45]
	ldr	r2, [fp, #-48]
	str	r2, [r7, #8]
	bl	dprint_vfmw
	ldr	r2, [r10, #100]
	mov	r3, r6
	ldr	r1, .L39+16
	bfi	r3, r2, #0, #24
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r7, #12]
	bl	dprint_vfmw
	ldr	r3, [r10, #120]
	ldr	r1, [r10, #124]
	mov	r0, #4
	ldr	r2, [r10, #128]
	and	r3, r3, #1
	bfi	r3, r1, #1, #2
	str	r6, [fp, #-48]
	bfi	r3, r2, #3, #5
	strb	r3, [fp, #-46]
	ldr	r2, [fp, #-48]
	ldr	r1, .L39+20
	str	r2, [r7, #16]
	bl	dprint_vfmw
	ldr	r3, [r10, #132]
	strh	r6, [fp, #-48]	@ movhi
	mov	r0, #4
	sub	r3, r3, #1
	strh	r3, [fp, #-46]	@ movhi
	ldr	r2, [fp, #-48]
	ldr	r1, .L39+24
	str	r2, [r7, #20]
	bl	dprint_vfmw
	ldr	r1, .L39+28
	mov	r0, #4
	str	r4, [r7, #252]
	mov	r2, r4
	str	r4, [fp, #-48]
	bl	dprint_vfmw
	mov	r3, #1
	b	.L20
.L36:
	ldr	r3, .L39+32
	ldr	r2, .L39+36
	ldr	r1, .L39+40
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L40:
	.align	2
.L39:
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
	.size	RV9HAL_V400R003_WriteSliceMsg, .-RV9HAL_V400R003_WriteSliceMsg
	.align	2
	.global	RV9HAL_V400R003_StartDec
	.type	RV9HAL_V400R003_StartDec, %function
RV9HAL_V400R003_StartDec:
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
	beq	.L43
	cmp	r1, #1
	bne	.L48
	mov	r0, #0
	mov	r3, r1
	str	r0, [sp]
	ldr	r2, .L50
	ldr	r1, .L50+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L45
.L43:
	ldr	r5, .L50+8
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L49
.L46:
	ldr	lr, .L50+12
	mov	r3, r2
	mov	r1, r4
	mov	r2, #0
	ldr	r0, .L50+8
	ldr	ip, [lr]
	add	ip, ip, #1
	str	ip, [lr]
	bl	RV9HAL_V400R003_WriteReg
	mov	r1, r4
	ldr	r0, .L50+8
	bl	RV9HAL_V400R003_WritePicMsg
	ldr	r0, .L50+8
	mov	r1, r4
	bl	RV9HAL_V400R003_WriteSliceMsg
	mov	r0, #0
.L45:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L48:
	ldr	r1, .L50+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L49:
	mov	r0, #0
	str	r2, [fp, #-24]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r5]
	ldrne	r2, [fp, #-24]
	bne	.L46
.L47:
	ldr	r1, .L50+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L45
.L51:
	.align	2
.L50:
	.word	.LANCHOR0+88
	.word	.LC1
	.word	g_HwMem
	.word	.LANCHOR1
	.word	.LC36
	.word	.LC37
	UNWIND(.fnend)
	.size	RV9HAL_V400R003_StartDec, .-RV9HAL_V400R003_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14901, %object
	.size	__func__.14901, 25
__func__.14901:
	.ascii	"RV9HAL_V400R003_WriteReg\000"
	.space	3
	.type	__func__.14909, %object
	.size	__func__.14909, 28
__func__.14909:
	.ascii	"RV9HAL_V400R003_WritePicMsg\000"
	.type	__func__.14922, %object
	.size	__func__.14922, 30
__func__.14922:
	.ascii	"RV9HAL_V400R003_WriteSliceMsg\000"
	.space	2
	.type	__func__.14893, %object
	.size	__func__.14893, 25
__func__.14893:
	.ascii	"RV9HAL_V400R003_StartDec\000"
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
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC2:
	ASCII(.ascii	"BASIC_V400R003_CFG0 = 0x%x\012\000" )
.LC3:
	ASCII(.ascii	"BASIC_V400R003_CFG1=0x%x\012\000" )
	.space	2
.LC4:
	ASCII(.ascii	"AVM_V200R003_ADDR=0x%x\012\000" )
.LC5:
	ASCII(.ascii	"VAM_V200R003_ADDR=0x%x\012\000" )
.LC6:
	ASCII(.ascii	"STREAM_V400R003_BASE_ADDR=0x%x\012\000" )
.LC7:
	ASCII(.ascii	"TIME_OUT = 0x%x\012\000" )
	.space	3
.LC8:
	ASCII(.ascii	"YSTADDR_V400R003_1D = 0x%x\012\000" )
.LC9:
	ASCII(.ascii	"YSTRIDE_V400R003_1D = 0x%x\012\000" )
.LC10:
	ASCII(.ascii	"UVOFFSET_V400R003_1D = 0x%x\012\000" )
	.space	3
.LC11:
	ASCII(.ascii	"HEAD_INF_OFFSET = 0x%x\012\000" )
.LC12:
	ASCII(.ascii	"DNR_MBINFO_STADDR=0x%x\012\000" )
.LC13:
	ASCII(.ascii	"REF_V200R003_PIC_TYPE=0x%x\012\000" )
.LC14:
	ASCII(.ascii	"FF_V200R003_APT_EN=0x%x\012\000" )
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
	ASCII(.ascii	"VdhId is wrong! RV9HAL_V400R003_StartDec\012\000" )
	.space	2
.LC37:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
