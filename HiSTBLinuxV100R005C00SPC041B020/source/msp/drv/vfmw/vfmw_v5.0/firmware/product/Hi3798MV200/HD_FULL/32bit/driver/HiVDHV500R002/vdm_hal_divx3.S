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
	.global	DIVX3HAL_V400R003_InitHal
	.type	DIVX3HAL_V400R003_InitHal, %function
DIVX3HAL_V400R003_InitHal:
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
	.size	DIVX3HAL_V400R003_InitHal, .-DIVX3HAL_V400R003_InitHal
	.align	2
	.global	DIVX3HAL_V400R003_WriteReg
	.type	DIVX3HAL_V400R003_WriteReg, %function
DIVX3HAL_V400R003_WriteReg:
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
	mov	r7, r0
	mov	r6, r1
	mov	r0, #2
	ldr	r1, .L11
	mov	r4, r2
	mov	r5, r3
	bl	dprint_vfmw
	cmp	r4, #0
	ble	.L3
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L11+4
	ldr	r1, .L11+8
	bl	dprint_vfmw
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3:
	ldr	r0, [r6, #8]
	mov	r8, #0
	ldr	ip, [r6, #12]
	mov	r1, r8
	mov	lr, #0
	bfi	lr, r8, #1, #1
	mov	r3, r5
	mov	r2, r4
	mul	ip, r0, ip
	mov	r0, #8
	sub	ip, ip, #1
	bfi	r1, ip, #0, #20
	str	r1, [fp, #-40]
	strb	lr, [fp, #-37]
	mov	r1, r1, lsr #16
	bfi	r1, r8, #7, #1
	strb	r1, [fp, #-38]
	ldr	r9, [fp, #-40]
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+12
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r1, [r6, #168]
	ldr	r2, [r6, #8]
	mov	r3, #0
	bfi	r3, r1, #4, #1
	mov	r1, #0	@ movhi
	and	r3, r3, #16
	bfi	r1, r8, #0, #12
	mov	r2, r2, asl #4
	orr	r3, r3, #64
	cmp	r2, #1920
	strh	r1, [fp, #-38]	@ movhi
	mvn	r3, r3, asl #25
	mov	r2, #0
	mvn	r3, r3, lsr #25
	bfi	r2, r8, #4, #1
	strb	r3, [fp, #-39]
	mov	r3, #5
	strb	r2, [fp, #-37]
	movls	r1, r8
	strb	r3, [fp, #-40]
	uxtbls	r3, r2
	ldrhib	r3, [fp, #-37]	@ zero_extendqisi2
	orrls	r3, r3, #32
	ldrhi	r1, [r6, #164]
	mov	r2, r4
	strls	r8, [r6, #164]
	bfihi	r3, r8, #5, #1
	strb	r3, [fp, #-37]
	andhi	r1, r1, #1
	ldrb	ip, [fp, #-37]	@ zero_extendqisi2
	mov	r3, r5
	mov	r0, #12
	mov	r8, #0
	bfi	ip, r1, #6, #1
	bfc	ip, #7, #1
	strb	ip, [fp, #-37]
	ldr	r9, [fp, #-40]
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+16
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r7, #56]
	mov	r3, r5
	mov	r2, r4
	bic	r9, r9, #15
	mov	r0, #16
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+20
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r7, #40]
	mov	r3, r5
	mov	r2, r4
	bic	r9, r9, #15
	mov	r0, #20
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+24
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #96]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #24
	mov	r1, r9
	str	r9, [fp, #-40]
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+28
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r1, [r6, #8]
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
	ldr	r1, .L11+32
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #128]
	mov	r3, r5
	mov	r2, r4
	bic	r9, r9, #15
	mov	r0, #96
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+36
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #136]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #100
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+40
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r9, [r6, #140]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #104
	mov	r1, r9
	bl	MFDE_ConfigReg
	mov	r2, r9
	ldr	r1, .L11+44
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r6, #148]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #108
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L11+48
	mov	r0, #3
	bl	dprint_vfmw
	ldr	r6, [r7, #1204]
	mov	r3, r5
	mov	r2, r4
	mov	r0, #144
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L11+52
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r5
	mov	r1, r8
	mov	r2, r4
	mov	r0, #148
	str	r8, [fp, #-40]
	bl	MFDE_ConfigReg
	mov	r2, r8
	ldr	r1, .L11+56
	mov	r0, #3
	bl	dprint_vfmw
	ldrb	r1, [fp, #-40]	@ zero_extendqisi2
	mov	r3, r5
	mov	r2, r4
	bfi	r1, r8, #0, #1
	strb	r1, [fp, #-40]
	ldr	r6, [fp, #-40]
	mov	r0, #152
	mov	r1, r6
	bl	MFDE_ConfigReg
	mov	r2, r6
	ldr	r1, .L11+60
	mov	r0, #3
	bl	dprint_vfmw
	mov	r3, r5
	mov	r2, r4
	mvn	r1, #0
	mov	r0, #32
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MFDE_ConfigReg
.L12:
	.align	2
.L11:
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
	.size	DIVX3HAL_V400R003_WriteReg, .-DIVX3HAL_V400R003_WriteReg
	.align	2
	.global	DIVX3HAL_V400R003_WritePicMsg
	.type	DIVX3HAL_V400R003_WritePicMsg, %function
DIVX3HAL_V400R003_WritePicMsg:
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
	mov	r6, r0
	mov	r4, r1
	mov	r0, #2
	ldr	r1, .L16
	bl	dprint_vfmw
	ldr	r0, [r6, #56]
	bic	r0, r0, #15
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L15
	ldr	r2, [r4, #4]
	mov	r7, #0
	mov	r3, #0
	str	r7, [fp, #-32]
	bfi	r3, r2, #0, #1
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	mov	r0, #4
	ldr	r1, .L16+4
	str	r2, [r5]
	bl	dprint_vfmw
	ldr	r2, [r4, #8]
	ldr	r3, [r4, #12]
	mov	r0, #0	@ movhi
	sub	r2, r2, #1
	mov	r1, r0	@ movhi
	sub	r3, r3, #1
	bfi	r0, r2, #0, #9
	bfi	r1, r3, #0, #9
	strh	r0, [fp, #-32]	@ movhi
	strh	r1, [fp, #-30]	@ movhi
	mov	r0, #4
	ldr	r2, [fp, #-32]
	ldr	r1, .L16+8
	str	r2, [r5, #4]
	bl	dprint_vfmw
	ldr	r3, [r4, #16]
	ldr	r1, [r4, #20]
	mov	r0, #4
	ldr	r2, [r4, #24]
	and	r3, r3, #31
	bfi	r3, r1, #5, #1
	str	r7, [fp, #-32]
	bfi	r3, r2, #6, #1
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	ldr	r1, .L16+12
	str	r2, [r5, #8]
	bl	dprint_vfmw
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
	str	r7, [fp, #-32]
	bfi	r3, r1, #7, #1
	bfi	r2, ip, #0, #1
	strb	r3, [fp, #-32]
	strb	r2, [fp, #-31]
	mov	r0, #4
	ldr	r2, [fp, #-32]
	ldr	r1, .L16+16
	str	r2, [r5, #12]
	bl	dprint_vfmw
	ldr	r2, [r4, #100]
	ldr	r1, .L16+20
	mov	r0, #4
	str	r2, [r5, #64]
	bl	dprint_vfmw
	ldr	r2, [r4, #108]
	str	r7, [fp, #-32]
	mov	r3, #0
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	mov	r0, #4
	ldr	r1, .L16+24
	str	r2, [r5, #68]
	bl	dprint_vfmw
	ldr	r2, [r4, #116]
	ldr	r1, .L16+28
	mov	r0, #4
	str	r2, [r5, #72]
	bl	dprint_vfmw
	ldr	r2, [r4, #104]
	ldr	r1, .L16+32
	mov	r0, #4
	str	r2, [r5, #76]
	bl	dprint_vfmw
	ldr	r2, [r4, #112]
	str	r7, [fp, #-32]
	mov	r3, #0
	bfi	r3, r2, #0, #7
	strb	r3, [fp, #-32]
	ldr	r2, [fp, #-32]
	mov	r0, #4
	ldr	r1, .L16+36
	str	r2, [r5, #80]
	bl	dprint_vfmw
	ldr	r2, [r4, #120]
	ldr	r1, .L16+40
	mov	r0, #4
	str	r2, [r5, #84]
	bl	dprint_vfmw
	ldr	r2, [r4, #132]
	ldr	r1, .L16+44
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #128]
	bl	dprint_vfmw
	ldr	r2, [r4, #124]
	ldr	r1, .L16+48
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #132]
	bl	dprint_vfmw
	ldr	r2, [r6, #1144]
	ldr	r1, .L16+52
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #136]
	bl	dprint_vfmw
	ldr	r2, [r6, #1148]
	ldr	r1, .L16+56
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #140]
	bl	dprint_vfmw
	ldr	r2, [r6, #1156]
	ldr	r1, .L16+60
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #144]
	bl	dprint_vfmw
	ldr	r2, [r4, #156]
	ldr	r1, .L16+64
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #148]
	bl	dprint_vfmw
	ldr	r2, [r6, #60]
	ldr	r1, .L16+68
	mov	r0, #4
	bic	r2, r2, #15
	str	r2, [r5, #252]
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
	.size	DIVX3HAL_V400R003_WritePicMsg, .-DIVX3HAL_V400R003_WritePicMsg
	.align	2
	.global	DIVX3HAL_V400R003_WriteSliceMsg
	.type	DIVX3HAL_V400R003_WriteSliceMsg, %function
DIVX3HAL_V400R003_WriteSliceMsg:
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
	ldr	r9, [r0, #60]
	mov	r10, r1
	bic	r9, r9, #15
	mov	r0, r9
	bl	MEM_Phy2Vir
	subs	r7, r0, #0
	beq	.L35
	ldr	r8, [r10, #56]
	cmp	r8, #0
	bne	.L36
.L20:
	ldr	r3, [r10, #52]
	cmp	r3, #0
	beq	.L18
	mov	r8, r8, asl #6
	mov	r5, #0
	add	r3, r8, r9
	str	r3, [fp, #-56]
.L31:
	cmp	r5, #0
	ldreq	r3, [r10, #56]
	beq	.L24
	add	r2, r10, r5, lsl #2
	ldr	r3, [r2, #56]
	ldr	r2, [r2, #52]
	cmp	r3, r2
	bls	.L25
.L24:
	add	r6, r8, r5, lsl #6
	ldr	r1, .L37
	mov	r2, #0
	mov	r0, #4
	bfi	r2, r3, #0, #20
	str	r2, [fp, #-48]
	str	r2, [r7, r6, asl #2]
	add	r4, r5, #1
	bl	dprint_vfmw
	ldr	r1, [r10, #52]
	mov	r6, r6, asl #2
	cmp	r4, r1
	bcs	.L26
	add	r3, r10, r5, lsl #2
	ldr	r0, [r3, #56]
	ldr	r3, [r3, #60]
	cmp	r0, r3
	bcc	.L26
	add	r3, r5, #15
	add	r3, r10, r3, lsl #2
	b	.L27
.L28:
	ldr	r2, [r3, #4]!
	cmp	r2, r0
	bhi	.L26
.L27:
	add	r4, r4, #1
	cmp	r4, r1
	bcc	.L28
.L26:
	cmp	r1, r4
	add	ip, r6, #4
	addne	r3, r4, #14
	add	r6, r6, #252
	ldreq	r1, [r10, #12]
	moveq	r9, #0
	ldreq	r2, [r10, #8]
	moveq	r5, r9
	ldrne	r1, [r10, r3, asl #2]
	mov	r0, #4
	ldrne	r2, [fp, #-56]
	muleq	r1, r2, r1
	addne	r5, r2, r4, lsl #8
	mov	r2, #0
	movne	r9, r5
	sub	r1, r1, #1
	bfi	r2, r1, #0, #20
	ldr	r1, .L37+4
	str	r2, [fp, #-48]
	str	r2, [r7, ip]
	bl	dprint_vfmw
	mov	r2, r5
	ldr	r1, .L37+8
	mov	r0, #4
	str	r9, [r7, r6]
	str	r5, [fp, #-48]
	bl	dprint_vfmw
	sub	r5, r4, #1
.L25:
	ldr	r3, [r10, #52]
	add	r5, r5, #1
	cmp	r3, r5
	bhi	.L31
.L18:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L36:
	mov	r4, #0
	ldr	r1, .L37
	mov	r3, r4
	bfi	r3, r4, #0, #20
	mov	r0, #4
	str	r3, [fp, #-48]
	mov	r2, r3
	str	r3, [r7]
	bl	dprint_vfmw
	ldr	r3, [r10, #56]
	ldr	r1, .L37+4
	mov	r2, r4
	sub	r3, r3, #1
	mov	r0, #4
	bfi	r2, r3, #0, #20
	str	r2, [fp, #-48]
	str	r2, [r7, #4]
	mov	r8, #1
	bl	dprint_vfmw
	add	r3, r9, #256
	ldr	r1, .L37+8
	mov	r0, #4
	str	r3, [r7, #252]
	mov	r2, r3
	str	r3, [fp, #-48]
	bl	dprint_vfmw
	b	.L20
.L35:
	ldr	r3, .L37+12
	ldr	r2, .L37+16
	ldr	r1, .L37+20
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L38:
	.align	2
.L37:
	.word	.LC18
	.word	.LC19
	.word	.LC34
	.word	.LC16
	.word	.LANCHOR0+60
	.word	.LC17
	UNWIND(.fnend)
	.size	DIVX3HAL_V400R003_WriteSliceMsg, .-DIVX3HAL_V400R003_WriteSliceMsg
	.align	2
	.global	DIVX3HAL_V400R003_StartDec
	.type	DIVX3HAL_V400R003_StartDec, %function
DIVX3HAL_V400R003_StartDec:
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
	beq	.L41
	cmp	r1, #1
	bne	.L46
	mov	r0, #0
	mov	r3, r1
	str	r0, [sp]
	ldr	r2, .L48
	ldr	r1, .L48+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L43
.L41:
	ldr	r5, .L48+8
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L47
.L44:
	ldr	lr, .L48+12
	mov	r3, r2
	mov	r1, r4
	mov	r2, #0
	ldr	r0, .L48+8
	ldr	ip, [lr]
	add	ip, ip, #1
	str	ip, [lr]
	bl	DIVX3HAL_V400R003_WriteReg
	mov	r1, r4
	ldr	r0, .L48+8
	bl	DIVX3HAL_V400R003_WritePicMsg
	ldr	r0, .L48+8
	mov	r1, r4
	bl	DIVX3HAL_V400R003_WriteSliceMsg
	mov	r0, #0
.L43:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L46:
	ldr	r1, .L48+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L47:
	mov	r0, #0
	str	r2, [fp, #-24]
	movt	r0, 63683
	bl	MEM_Phy2Vir
	subs	r3, r0, #0
	strne	r3, [r5]
	ldrne	r2, [fp, #-24]
	bne	.L44
.L45:
	ldr	r1, .L48+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L43
.L49:
	.align	2
.L48:
	.word	.LANCHOR0+92
	.word	.LC1
	.word	g_HwMem
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC36
	UNWIND(.fnend)
	.size	DIVX3HAL_V400R003_StartDec, .-DIVX3HAL_V400R003_StartDec
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14885, %object
	.size	__func__.14885, 27
__func__.14885:
	.ascii	"DIVX3HAL_V400R003_WriteReg\000"
	.space	1
	.type	__func__.14893, %object
	.size	__func__.14893, 30
__func__.14893:
	.ascii	"DIVX3HAL_V400R003_WritePicMsg\000"
	.space	2
	.type	__func__.14907, %object
	.size	__func__.14907, 32
__func__.14907:
	.ascii	"DIVX3HAL_V400R003_WriteSliceMsg\000"
	.type	__func__.14877, %object
	.size	__func__.14877, 27
__func__.14877:
	.ascii	"DIVX3HAL_V400R003_StartDec\000"
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
	ASCII(.ascii	"BASIC_V200R003_CFG0 = 0x%x\012\000" )
.LC3:
	ASCII(.ascii	"BASIC_V400R003_CFG1=0x%x\012\000" )
	.space	2
.LC4:
	ASCII(.ascii	"AVM_V400R003_ADDR=0x%x\012\000" )
.LC5:
	ASCII(.ascii	"VAM_V400R003_ADDR=0x%x\012\000" )
.LC6:
	ASCII(.ascii	"STREAM_V200R003_BASE_ADDR=0x%x\012\000" )
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
	ASCII(.ascii	"REF_V400R003_PIC_TYPE=0x%x\012\000" )
.LC14:
	ASCII(.ascii	"FF_V400R003_APT_EN=0x%x\012\000" )
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
	ASCII(.ascii	"VdhId is wrong! DIVX3HAL_V400R003_StartDec\012\000" )
.LC36:
	ASCII(.ascii	"vdm register virtual address not mapped, reset fail" )
	ASCII(.ascii	"ed!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
