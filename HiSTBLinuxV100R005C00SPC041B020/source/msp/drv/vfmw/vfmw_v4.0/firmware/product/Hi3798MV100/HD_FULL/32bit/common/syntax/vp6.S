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
	.file	"vp6.c"
	.text
	.align	2
	.global	Vp6_Vfmw_DecodeBool
	.type	Vp6_Vfmw_DecodeBool, %function
Vp6_Vfmw_DecodeBool:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r7, r1
	ldmib	r1, {r1, r5}
	mov	r6, r0
	ldr	r3, [r7, #12]
	sub	r4, r1, #1
	mul	r2, r4, r2
	mov	r4, r2, lsr #8
	add	r4, r4, #1
	mov	r2, r4, asl #16
	cmp	r5, r2
	rsbcs	r4, r4, r1
	rsbcs	r5, r2, r5
	movcs	r8, #1
	movcc	r8, #0
	cmp	r4, #127
	bls	.L6
	b	.L9
.L5:
	cmp	r4, #127
	bhi	.L11
.L6:
	subs	r3, r3, #1
	mov	r4, r4, asl #1
	mov	r5, r5, asl #1
	bne	.L5
	mov	r1, #8
	mov	r0, r6
	bl	BsGet
	cmp	r4, #127
	mov	r3, #8
	orr	r5, r5, r0
	bls	.L6
.L11:
	str	r3, [r7, #12]
.L9:
	stmib	r7, {r4, r5}
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	Vp6_Vfmw_DecodeBool, .-Vp6_Vfmw_DecodeBool
	.align	2
	.global	Vp6_Vfmw_DecodeBool128
	.type	Vp6_Vfmw_DecodeBool128, %function
Vp6_Vfmw_DecodeBool128:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, [r1, #4]
	mov	r4, r1
	ldr	r2, [r1, #8]
	add	r3, r5, #1
	ldr	r1, [r1, #12]
	mov	r3, r3, lsr #1
	mov	r6, r3, asl #16
	cmp	r2, r6
	rsbcs	r3, r3, r5
	rsbcs	r2, r6, r2
	movcs	r7, #1
	movcc	r5, r3, asl #1
	movcs	r5, r3, asl r7
	movcs	r6, r2, asl r7
	movcc	r6, r2, asl #1
	movcc	r7, #0
	subs	r1, r1, #1
	bne	.L15
	mov	r1, #8
	bl	BsGet
	mov	r1, #8
	orr	r6, r6, r0
.L15:
	str	r1, [r4, #12]
	mov	r0, r7
	stmib	r4, {r5, r6}
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Vp6_Vfmw_DecodeBool128, .-Vp6_Vfmw_DecodeBool128
	.align	2
	.global	Vp6_Vfmw_BoolRead
	.type	Vp6_Vfmw_BoolRead, %function
Vp6_Vfmw_BoolRead:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r2, #1
	mov	r5, #0
	mov	r6, r0
	mov	r7, r1
	bmi	.L17
.L18:
	mov	r1, r7
	mov	r0, r6
	bl	Vp6_Vfmw_DecodeBool128
	orr	r5, r5, r0, asl r4
	subs	r4, r4, #1
	bcs	.L18
.L17:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Vp6_Vfmw_BoolRead, .-Vp6_Vfmw_BoolRead
	.align	2
	.global	Vp6_Vfmw_StartDecode
	.type	Vp6_Vfmw_StartDecode, %function
Vp6_Vfmw_StartDecode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r1
	mov	r2, #0
	mov	r3, #255
	stmia	r1, {r2, r3}
	mov	r3, #8
	mov	r1, #24
	str	r3, [r4, #12]
	bl	BsGet
	str	r0, [r4, #8]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	Vp6_Vfmw_StartDecode, .-Vp6_Vfmw_StartDecode
	.align	2
	.global	VP6_ArrangeVHBMem
	.type	VP6_ArrangeVHBMem, %function
VP6_ArrangeVHBMem:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #60)
	sub	sp, sp, #60
	add	r4, r0, #12288
	mov	r5, r0
	ldr	r3, [r4, #208]
	ldr	r2, [r4, #212]
	cmp	r3, #2
	bls	.L24
	sub	r0, r2, #3
	movw	r1, #509
	cmp	r3, #512
	cmpls	r0, r1
	bhi	.L24
	ldr	r1, [r5]
	ldr	r0, [r1, #776]
	cmp	r0, #0
	ldr	r0, [r1, #28]
	bne	.L27
	cmp	r0, #25
	beq	.L50
.L28:
	mov	r7, r2, asl #4
	mov	r10, r3, asl #4
	mov	r9, #5
	mov	r6, #3
	str	r6, [fp, #-88]
.L33:
	ldr	r8, .L52
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r2, [r5]
	str	r10, [fp, #-48]
	mov	r3, #0
	str	r7, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r10, [fp, #-64]
	cmp	r2, #24
	str	r7, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L39
	ldrb	r3, [fp, #-88]	@ zero_extendqisi2
	uxtb	r6, r6
	strb	r9, [fp, #-70]
	strb	r3, [fp, #-72]
.L42:
	sub	r1, fp, #76
	ldr	r0, [r4, #712]
	strb	r6, [fp, #-69]
	mov	r6, #0
	strb	r6, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r7, r0, #0
	bne	.L40
	ldr	r1, [r5]
	sub	r3, fp, #80
	ldr	r0, [r4, #712]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_PartitionFsMemory
	cmp	r0, r6
	moveq	r0, #1
	streq	r0, [r4, #8]
	beq	.L46
	ldr	r3, [r8, #68]
	mov	r0, r7
	ldr	r1, .L52+4
	blx	r3
	mov	r0, r7
	b	.L46
.L24:
	ldr	ip, .L52
	mov	r0, #0
	ldr	r1, .L52+8
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
.L46:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L27:
	cmp	r0, #25
	bne	.L28
	ldr	r0, [r1, #736]
	mov	r2, r2, asl #4
	ldr	ip, [r1, #740]
	mov	r3, r3, asl #4
	cmp	r2, r0
	bhi	.L36
	cmp	r3, ip
	bhi	.L36
	ldr	r0, [r1, #764]
	mov	r7, r2
	ldr	r2, [r1, #760]
	mov	r10, r3
	cmp	r0, #0
	add	r3, r2, #2
	str	r3, [fp, #-88]
	beq	.L44
	add	r6, r2, #1
	cmp	r6, #17
	movge	r6, #17
.L38:
	ldrb	r9, [r1, #784]	@ zero_extendqisi2
	b	.L33
.L40:
	ldr	r3, [r8, #68]
	mov	r0, r6
	ldr	r1, .L52+12
	blx	r3
	mov	r0, r6
	b	.L46
.L39:
	strb	r3, [fp, #-72]
	mov	r6, #1
	strb	r9, [fp, #-70]
	b	.L42
.L50:
	ldr	r7, [r1, #736]
	mov	r2, r2, asl #4
	cmp	r2, r7
	movhi	r3, r3, asl #4
	ldrhi	ip, [r1, #740]
	bhi	.L30
	ldr	r10, [r1, #740]
	mov	r3, r3, asl #4
	cmp	r3, r10
	bhi	.L51
	ldr	r0, [r4, #8]
	cmp	r0, #1
	beq	.L46
	ldr	r2, [r1, #764]
	ldr	r3, [r1, #760]
	cmp	r2, #0
	add	r2, r3, #2
	str	r2, [fp, #-88]
	beq	.L44
	add	r6, r3, #1
	cmp	r6, #17
	movge	r6, #17
	b	.L38
.L36:
	ldr	r1, .L52
	stmia	sp, {r0, ip}
.L49:
	ldr	ip, [r1, #68]
	mov	r0, #0
	ldr	r1, .L52+16
	blx	ip
	mov	r0, #0
	b	.L46
.L44:
	mov	r6, #1
	b	.L38
.L51:
	mov	ip, r10
.L30:
	ldr	r1, .L52
	stmia	sp, {r7, ip}
	b	.L49
.L53:
	.align	2
.L52:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC0
	.word	.LC3
	.word	.LC1
	UNWIND(.fnend)
	.size	VP6_ArrangeVHBMem, .-VP6_ArrangeVHBMem
	.align	2
	.global	VP6_GetImageBuffer
	.type	VP6_GetImageBuffer, %function
VP6_GetImageBuffer:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r4, r0, #12288
	mov	r6, r0
	mov	r1, #1
	ldr	r0, [r4, #712]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #20]
	blt	.L61
	mov	r1, r0
	ldr	r0, [r4, #712]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L62
	ldr	r6, .L63
	mov	r0, #18
	ldr	r2, [r4, #20]
	ldr	r1, .L63+4
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r5, #520]
	cmp	r3, #0
	beq	.L60
	ldr	r0, [r5, #524]
	cmp	r0, #0
	beq	.L56
	ldr	r3, [r5, #528]
	cmp	r3, #0
	beq	.L60
	ldr	r3, [r0, #20]
	add	r2, r5, #520
	ldr	ip, [r6, #68]
	mov	r0, #18
	ldr	r1, .L63+8
	str	r3, [r4, #36]
	ldmia	r2, {r2, r3, lr}
	ldr	lr, [lr, #12]
	ldr	r3, [r3, #12]
	ldr	r2, [r2, #12]
	str	lr, [sp]
	blx	ip
	mov	r0, #1
.L56:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L60:
	mov	r0, r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L61:
	ldr	r3, .L63
	mov	r0, #0
	ldr	r1, .L63+12
	mov	r5, r0
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r4, #24]
	ldr	r0, [r4, #712]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #712]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #24]
	str	r5, [r4, #28]
	ldr	r0, [r4, #712]
	bl	FSP_GetNullLogicFs
	add	r1, r6, #12608
	str	r5, [r4, #4]
	add	r1, r1, #40
	str	r0, [r4, #28]
	str	r0, [r4, #24]
	ldr	r0, [r4, #712]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L62:
	ldr	r3, .L63
	mov	r2, #656
	ldr	r1, .L63+16
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	b	.L56
.L64:
	.align	2
.L63:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC7
	.word	.LC4
	.word	.LC5
	UNWIND(.fnend)
	.size	VP6_GetImageBuffer, .-VP6_GetImageBuffer
	.align	2
	.global	VP6DEC_GetImageBuffer
	.type	VP6DEC_GetImageBuffer, %function
VP6DEC_GetImageBuffer:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #12288
	mov	r5, r0
	ldr	r0, [r4, #712]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #712]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L71
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L71:
	add	r1, r5, #12608
	ldr	r0, [r4, #712]
	add	r1, r1, #40
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6DEC_GetImageBuffer, .-VP6DEC_GetImageBuffer
	.align	2
	.global	VP6_BuildScanOrder
	.type	VP6_BuildScanOrder, %function
VP6_BuildScanOrder:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #1088
	mov	r4, #1
	mov	lr, #0
	strb	lr, [r0, #960]
.L73:
	mov	r2, r5
	mov	r3, #1
.L75:
	ldrb	ip, [r2, #1]!	@ zero_extendqisi2
	cmp	ip, lr
	addeq	ip, r0, r4
	addeq	r4, r4, #1
	streqb	r3, [ip, #960]
	add	r3, r3, #1
	cmp	r3, #64
	bne	.L75
	add	lr, lr, #1
	cmp	lr, #16
	bne	.L73
	add	r5, r0, #1020
	add	r4, r0, #956
	add	r5, r5, #3
	add	r4, r4, #3
	mov	lr, #0
.L76:
	mov	r3, #0
	mov	ip, r4
	mov	r2, r3
.L77:
	ldrb	r0, [ip, #1]!	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r3, r0
	movcc	r3, r0
	cmp	r2, lr
	bls	.L77
	uxtb	r3, r3
	strb	r3, [r5, #1]!
	ldr	r2, [r1, #12]
	add	lr, lr, #1
	cmp	r2, #6
	addhi	r3, r3, #1
	strhib	r3, [r5]
	cmp	lr, #64
	bne	.L76
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6_BuildScanOrder, .-VP6_BuildScanOrder
	.global	__aeabi_idiv
	.global	__aeabi_uidiv
	.align	2
	.global	VP6_BuildModeTree
	.type	VP6_BuildModeTree, %function
VP6_BuildModeTree:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 96
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #100)
	sub	sp, sp, #100
	mov	r9, #0
	mov	r10, #100
	mov	r7, r9
	add	r2, r0, #404
	add	r3, r0, #464
	str	r0, [fp, #-128]
	str	r0, [fp, #-132]
	str	r2, [fp, #-140]
	str	r3, [fp, #-136]
.L84:
	ldr	r3, [fp, #-128]
	ldr	r8, [fp, #-140]
	ldr	r9, [fp, #-132]
	str	r3, [fp, #-92]
	str	r3, [fp, #-88]
.L90:
	mov	r4, #0
	sub	r1, fp, #84
	mov	r2, r4
	mov	r0, r8
.L87:
	cmp	r7, r2
	add	r2, r2, #1
	subeq	r3, fp, #84
	add	r1, r1, #4
	ldrneb	r3, [r0]	@ zero_extendqisi2
	moveq	ip, #0
	streq	ip, [r3, r7, asl #2]
	add	r0, r0, #1
	ldreq	r3, [r1, #-4]
	smulbbne	r3, r3, r10
	add	r4, r4, r3
	strne	r3, [r1, #-4]
	cmp	r2, #10
	bne	.L87
	ldr	r2, [fp, #-88]
	add	r8, r8, #20
	add	r9, r9, #90
	add	r2, r2, #20
	ldrb	r0, [r2, #394]	@ zero_extendqisi2
	ldrb	r3, [r2, #384]	@ zero_extendqisi2
	str	r2, [fp, #-88]
	mvn	r1, r0
	rsb	r0, r0, r0, lsl #8
	rsb	r1, r3, r1
	bl	__aeabi_idiv
	ldr	r3, [fp, #-72]
	ldr	r2, [fp, #-84]
	add	r1, r4, #1
	ldr	r6, [fp, #-76]
	mov	ip, r3
	ldr	r3, [fp, #-68]
	add	r6, r2, r6
	ldr	lr, [fp, #-92]
	str	ip, [fp, #-96]
	add	ip, ip, r3
	add	r4, ip, r6
	str	r2, [fp, #-124]
	add	lr, lr, #10
	str	ip, [fp, #-104]
	sub	r3, r0, #1
	rsb	r0, r4, r4, lsl #8
	strb	r3, [lr, #-6]
	str	lr, [fp, #-92]
	bl	__aeabi_uidiv
	add	r1, r4, #1
	add	r3, r0, #1
	rsb	r0, r6, r6, lsl #8
	strb	r3, [r9, #-46]
	bl	__aeabi_uidiv
	ldr	lr, [fp, #-64]
	ldr	r3, [fp, #-80]
	ldr	r1, [fp, #-52]
	ldr	r5, [fp, #-56]
	ldr	ip, [fp, #-48]
	ldr	r4, [fp, #-60]
	add	r5, r3, r5
	add	ip, r1, ip
	str	r3, [fp, #-120]
	add	r4, lr, r4
	add	r3, ip, #1
	str	r1, [fp, #-108]
	add	r1, ip, r5
	add	ip, r4, #1
	str	r3, [fp, #-112]
	add	r1, r1, ip
	str	ip, [fp, #-116]
	str	lr, [fp, #-100]
	add	r0, r0, #1
	strb	r0, [r9, #-45]
	rsb	r0, r5, r5, lsl #8
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-124]
	add	r1, r6, #1
	add	lr, r0, #1
	rsb	r0, r2, r2, lsl #8
	strb	lr, [r9, #-44]
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-96]
	ldr	ip, [fp, #-104]
	add	r1, ip, #1
	add	r2, r0, #1
	rsb	r0, r3, r3, lsl #8
	strb	r2, [r9, #-43]
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-120]
	add	r1, r5, #1
	add	r2, r0, #1
	rsb	r0, r3, r3, lsl #8
	strb	r2, [r9, #-42]
	bl	__aeabi_uidiv
	ldr	r5, [fp, #-112]
	add	r1, r5, r4
	add	r3, r0, #1
	rsb	r0, r4, r4, lsl #8
	strb	r3, [r9, #-41]
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-116]
	ldr	lr, [fp, #-100]
	mov	r1, ip
	add	r3, r0, #1
	rsb	r0, lr, lr, lsl #8
	strb	r3, [r9, #-40]
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-108]
	mov	r1, r5
	add	r3, r0, #1
	rsb	r0, r2, r2, lsl #8
	strb	r3, [r9, #-39]
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-136]
	cmp	r8, r3
	add	r0, r0, #1
	strb	r0, [r9, #-38]
	bne	.L90
	ldr	r3, [fp, #-128]
	add	r7, r7, #1
	cmp	r7, #10
	add	r3, r3, #1
	str	r3, [fp, #-128]
	ldr	r3, [fp, #-132]
	add	r3, r3, #9
	str	r3, [fp, #-132]
	bne	.L84
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6_BuildModeTree, .-VP6_BuildModeTree
	.align	2
	.global	VP6_ConfigureContexts
	.type	VP6_ConfigureContexts, %function
VP6_ConfigureContexts:
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
	add	r7, r0, #1184
	add	lr, r0, #516
	add	r7, r7, #5
	add	lr, lr, #2
	add	r0, r0, #540
.L93:
	ldr	r6, .L101
	sub	r5, r7, #33
	add	r4, lr, #6
	sub	r8, lr, #5
.L98:
	sub	ip, r5, #5
	mov	r1, r8
	mov	r2, r6
.L94:
	ldmia	r2, {r3, r9}
	add	r2, r2, #24
	ldrb	r10, [r1, #1]!	@ zero_extendqisi2
	mul	r3, r3, r10
	add	r3, r3, #128
	add	r3, r9, r3, asr #8
	cmp	r3, #255
	movge	r3, #255
	cmp	r3, #1
	movlt	r3, #1
	cmp	r1, lr
	strb	r3, [ip, #1]!
	bne	.L94
	mov	r2, r5
	mov	r3, lr
.L95:
	ldrb	r1, [r3, #1]!	@ zero_extendqisi2
	cmp	r4, r3
	strb	r1, [r2, #1]!
	bne	.L95
	add	r5, r5, #11
	add	r6, r6, #8
	cmp	r7, r5
	bne	.L98
	add	lr, lr, #11
	add	r7, r7, #33
	cmp	lr, r0
	bne	.L93
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L102:
	.align	2
.L101:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VP6_ConfigureContexts, .-VP6_ConfigureContexts
	.align	2
	.global	VP6_ConfigureEntropyDecoder
	.type	VP6_ConfigureEntropyDecoder, %function
VP6_ConfigureEntropyDecoder:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #60)
	sub	sp, sp, #60
	mov	lr, r2
	str	r2, [fp, #-100]
	mov	r9, r1
	ldr	r2, .L151
	mov	r1, #128
	str	r0, [fp, #-72]
	sub	r0, fp, #56
	str	r3, [fp, #-64]
	add	r8, lr, #72
	ldr	ip, [r2, #48]
	mov	r2, #11
	blx	ip
	mov	r3, #0
	str	r3, [fp, #-68]
.L104:
	ldr	r3, [fp, #-68]
	mov	r4, #0
	add	r5, r3, #512
	sub	r6, r3, #1
	ldr	r3, .L151+4
	add	r5, r5, #2
	add	r6, r3, r6
	ldr	r3, [fp, #-72]
	add	r5, r3, r5
.L108:
	ldrb	r2, [r6, #1]!	@ zero_extendqisi2
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L105
	mov	r10, #0
	mov	r7, #6
.L106:
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r7
	subs	r7, r7, #1
	bcs	.L106
	mov	r3, r10, asl #1
	ands	r3, r3, #255
	moveq	r2, #1
	movne	r2, #0
	add	r3, r3, r2
	sub	r2, fp, #56
	uxtb	r3, r3
	strb	r3, [r2, r4]
	strb	r3, [r5]
.L107:
	add	r4, r4, #1
	add	r5, r5, #1
	cmp	r4, #11
	bne	.L108
	ldr	r3, [fp, #-68]
	add	r3, r3, #11
	str	r3, [fp, #-68]
	cmp	r3, #22
	bne	.L104
	ldr	r3, [fp, #-64]
	cmp	r3, #0
	bne	.L110
	ldr	r3, .L151
	mov	r2, #28
	ldr	r0, [fp, #-72]
	ldr	r1, .L151+8
	ldr	r3, [r3, #52]
	add	r0, r0, #536
	blx	r3
.L110:
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool128
	cmp	r0, #0
	beq	.L114
	ldr	r3, [fp, #-72]
	ldr	r4, .L151+12
	add	r5, r3, #1088
	add	r5, r5, #1
	add	r6, r4, #63
.L113:
	ldrb	r2, [r4, #1]!	@ zero_extendqisi2
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L111
	mov	r10, #0
	mov	r7, #3
.L112:
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r7
	subs	r7, r7, #1
	bcs	.L112
	strb	r10, [r5]
.L111:
	cmp	r4, r6
	add	r5, r5, #1
	bne	.L113
	ldr	r1, [fp, #-100]
	ldr	r0, [fp, #-72]
	bl	VP6_BuildScanOrder
.L114:
	mov	r3, #0
	str	r3, [fp, #-68]
.L118:
	ldr	r3, [fp, #-68]
	sub	r5, r3, #1
	add	r6, r3, #536
	add	r7, r3, #13
	ldr	r3, .L151+16
	add	r5, r3, r5
	ldr	r3, [fp, #-72]
	add	r6, r3, r6
	ldr	r3, .L151+16
	add	r7, r3, r7
.L117:
	ldrb	r2, [r5, #1]!	@ zero_extendqisi2
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L115
	mov	r10, #0
	mov	r4, #6
.L116:
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r4
	subs	r4, r4, #1
	bcs	.L116
	mov	r10, r10, asl #1
	ands	r10, r10, #255
	moveq	r3, #1
	movne	r3, #0
	add	r3, r10, r3
	strb	r3, [r6]
.L115:
	cmp	r5, r7
	add	r6, r6, #1
	bne	.L117
	ldr	r3, [fp, #-68]
	add	r3, r3, #14
	str	r3, [fp, #-68]
	cmp	r3, #28
	bne	.L118
	mov	r3, #0
	str	r3, [fp, #-92]
.L127:
	ldr	r3, [fp, #-92]
	add	r2, r3, #396
	str	r2, [fp, #-96]
	str	r3, [fp, #-88]
	str	r3, [fp, #-84]
.L129:
	ldr	r3, [fp, #-84]
	add	r2, r3, #66
	str	r2, [fp, #-76]
	str	r3, [fp, #-68]
	ldr	r3, [fp, #-88]
	sub	r3, r3, #1
	str	r3, [fp, #-80]
.L126:
	ldr	r3, [fp, #-68]
	mov	r4, #0
	ldr	r2, [fp, #-80]
	add	r5, r3, #564
	add	r6, r2, r3
	ldr	r3, .L151+20
	add	r6, r3, r6
	ldr	r3, [fp, #-72]
	add	r5, r3, r5
.L123:
	ldrb	r2, [r6, #1]!	@ zero_extendqisi2
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L120
	mov	r10, #0
	mov	r7, #6
.L121:
	mov	r1, r8
	mov	r0, r9
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r7
	subs	r7, r7, #1
	bcs	.L121
	mov	r3, r10, asl #1
	ands	r3, r3, #255
	moveq	r2, #1
	movne	r2, #0
	add	r3, r3, r2
	sub	r2, fp, #56
	uxtb	r3, r3
	strb	r3, [r2, r4]
	strb	r3, [r5]
.L122:
	add	r4, r4, #1
	add	r5, r5, #1
	cmp	r4, #11
	bne	.L123
	ldr	r3, [fp, #-68]
	ldr	r2, [fp, #-76]
	add	r3, r3, #11
	str	r3, [fp, #-68]
	cmp	r3, r2
	bne	.L126
	ldr	r2, [fp, #-88]
	ldr	r3, [fp, #-84]
	sub	r2, r2, #132
	str	r2, [fp, #-88]
	ldr	r2, [fp, #-96]
	add	r3, r3, #198
	str	r3, [fp, #-84]
	cmp	r3, r2
	bne	.L129
	ldr	r3, [fp, #-92]
	add	r3, r3, #66
	str	r3, [fp, #-92]
	cmp	r3, #198
	bne	.L127
	ldr	r1, [fp, #-100]
	ldr	r0, [fp, #-72]
	bl	VP6_ConfigureContexts
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L120:
	ldr	r3, [fp, #-64]
	cmp	r3, #0
	subeq	r3, fp, #56
	ldreqb	r3, [r3, r4]	@ zero_extendqisi2
	streqb	r3, [r5]
	b	.L122
.L105:
	ldr	r3, [fp, #-64]
	cmp	r3, #0
	subeq	r3, fp, #56
	ldreqb	r3, [r3, r4]	@ zero_extendqisi2
	streqb	r3, [r5]
	b	.L107
.L152:
	.align	2
.L151:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+120
	.word	.LANCHOR0+144
	.word	.LANCHOR0+596
	.word	.LANCHOR0+172
	.word	.LANCHOR0+200
	UNWIND(.fnend)
	.size	VP6_ConfigureEntropyDecoder, .-VP6_ConfigureEntropyDecoder
	.align	2
	.global	VP6_decodeModeDiff
	.type	VP6_decodeModeDiff, %function
VP6_decodeModeDiff:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r1, #72
	mov	r2, #205
	mov	r7, r0
	mov	r1, r5
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	mov	r2, #171
	mov	r1, r5
	mov	r6, r0, asl #1
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool
	rsb	r6, r6, #1
	cmp	r0, #0
	beq	.L163
	mov	r2, #199
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	movne	r8, #0
	movne	r4, #6
	beq	.L164
.L160:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	orr	r8, r8, r0, asl r4
	subs	r4, r4, #1
	bcs	.L160
	mul	r0, r8, r6
	mov	r0, r0, asl #2
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L164:
	mov	r2, #140
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L157
	mov	r0, r6, asl #4
	sub	r0, r0, r6, asl #2
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L163:
	mov	r1, r5
	mov	r0, r7
	mov	r2, #83
	bl	Vp6_Vfmw_DecodeBool
	rsb	r0, r0, #3
	mov	r0, r6, asl r0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L157:
	mov	r2, #125
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L158
	mov	r0, r6, asl #4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L158:
	mov	r1, r5
	mov	r0, r7
	mov	r2, #104
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	movne	r0, r6, asl #4
	addne	r0, r0, r6, lsl #2
	moveq	r0, r6, asl #5
	subeq	r0, r0, r6, asl #3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6_decodeModeDiff, .-VP6_decodeModeDiff
	.align	2
	.global	VP6_DecodeModeProbs
	.type	VP6_DecodeModeProbs, %function
VP6_DecodeModeProbs:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r9, r0, #412
	add	r9, r9, #1
	add	r7, r2, #72
	mov	r6, r2
	mov	r5, r1
	mov	r8, #0
	str	r0, [fp, #-48]
.L172:
	mov	r2, #174
	mov	r1, r7
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L166
	mov	r10, #0
	mov	r4, #3
.L167:
	mov	r1, r7
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r4
	subs	r4, r4, #1
	bcs	.L167
	mov	r3, r10, asl #4
	add	r0, r8, #20
	add	r1, r3, r10, lsl #2
	mov	r2, r9
	add	r3, r8, r1
	add	r0, r1, r0
	ldr	r1, .L186
	add	r3, r1, r3
	add	r0, r1, r0
.L168:
	ldrb	r1, [r3]	@ zero_extendqisi2
	add	r3, r3, #2
	strb	r1, [r2, #1]!
	ldrb	r1, [r3, #-1]	@ zero_extendqisi2
	cmp	r3, r0
	strb	r1, [r2, #-10]
	bne	.L168
.L166:
	mov	r2, #254
	mov	r1, r7
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L173
	sub	r10, r9, #412
	mov	r4, #0
	sub	r10, r10, #1
.L171:
	mov	r1, r6
	mov	r0, r5
	bl	VP6_decodeModeDiff
	ldrb	r3, [r10, #414]	@ zero_extendqisi2
	adds	r0, r3, r0
	movmi	r0, #0
	bmi	.L169
	cmp	r0, #255
	movge	r0, #255
	uxtb	r0, r0
.L169:
	strb	r0, [r10, #414]
	mov	r1, r6
	mov	r0, r5
	bl	VP6_decodeModeDiff
	ldrb	r3, [r10, #404]	@ zero_extendqisi2
	adds	r0, r3, r0
	movmi	r0, #0
	bmi	.L170
	cmp	r0, #255
	movge	r0, #255
	uxtb	r0, r0
.L170:
	add	r4, r4, #1
	strb	r0, [r10, #404]
	cmp	r4, #10
	add	r10, r10, #1
	bne	.L171
.L173:
	add	r8, r8, #320
	add	r9, r9, #20
	cmp	r8, #960
	bne	.L172
	mov	r1, r6
	ldr	r0, [fp, #-48]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	VP6_BuildModeTree
.L187:
	.align	2
.L186:
	.word	.LANCHOR0+660
	UNWIND(.fnend)
	.size	VP6_DecodeModeProbs, .-VP6_DecodeModeProbs
	.align	2
	.global	VP6_ConfigureMvEntropyDecoder
	.type	VP6_ConfigureMvEntropyDecoder, %function
VP6_ConfigureMvEntropyDecoder:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, .L227
	add	r4, r2, #72
	mov	r5, r1
	add	r8, r3, #34
	mov	r7, r0
	mov	r6, r3
	str	r0, [fp, #-52]
.L196:
	ldrb	r2, [r6]	@ zero_extendqisi2
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L192
	mov	r10, #0
	mov	r9, #6
.L190:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r9
	subs	r9, r9, #1
	bcs	.L190
	mov	r3, r10, asl #1
	ands	r3, r3, #255
	moveq	r3, #1
	strb	r3, [r7, #2]
.L192:
	ldrb	r2, [r6, #1]	@ zero_extendqisi2
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L195
	mov	r10, #0
	mov	r9, #6
.L193:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r9
	subs	r9, r9, #1
	bcs	.L193
	mov	r3, r10, asl #1
	ands	r3, r3, #255
	moveq	r3, #1
	strb	r3, [r7]
.L195:
	add	r6, r6, #17
	add	r7, r7, #1
	cmp	r6, r8
	bne	.L196
	ldr	r3, [fp, #-52]
	add	r8, r3, #488
	mov	r3, #0
	add	r8, r8, #3
	str	r3, [fp, #-48]
.L197:
	ldr	r3, [fp, #-48]
	sub	r6, r8, #7
	add	r7, r3, #1
	ldr	r3, .L227
	add	r7, r3, r7
.L203:
	ldrb	r2, [r7, #1]!	@ zero_extendqisi2
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L199
	mov	r10, #0
	mov	r9, #6
.L200:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r9
	subs	r9, r9, #1
	bcs	.L200
	mov	r2, r10, asl #1
	ands	r2, r2, #255
	strneb	r2, [r6]
	moveq	r3, #1
	streqb	r3, [r6]
.L199:
	add	r6, r6, #1
	cmp	r8, r6
	bne	.L203
	ldr	r3, [fp, #-48]
	add	r8, r8, #7
	add	r3, r3, #17
	str	r3, [fp, #-48]
	cmp	r3, #34
	bne	.L197
	ldr	r3, [fp, #-52]
	add	r8, r3, #504
	mov	r3, #0
	add	r8, r8, #2
	str	r3, [fp, #-48]
.L204:
	ldr	r3, [fp, #-48]
	sub	r6, r8, #8
	add	r7, r3, #8
	ldr	r3, .L227
	add	r7, r3, r7
.L210:
	ldrb	r2, [r7, #1]!	@ zero_extendqisi2
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L206
	mov	r10, #0
	mov	r9, #6
.L207:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r9
	subs	r9, r9, #1
	bcs	.L207
	mov	r2, r10, asl #1
	ands	r2, r2, #255
	strneb	r2, [r6]
	moveq	r3, #1
	streqb	r3, [r6]
.L206:
	add	r6, r6, #1
	cmp	r8, r6
	bne	.L210
	ldr	r3, [fp, #-48]
	add	r8, r8, #8
	add	r3, r3, #17
	str	r3, [fp, #-48]
	cmp	r3, #34
	bne	.L204
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L228:
	.align	2
.L227:
	.word	.LANCHOR0+1620
	UNWIND(.fnend)
	.size	VP6_ConfigureMvEntropyDecoder, .-VP6_ConfigureMvEntropyDecoder
	.align	2
	.global	VP6_BoolTreeToHuffCodes
	.type	VP6_BoolTreeToHuffCodes, %function
VP6_BoolTreeToHuffCodes:
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
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	ldrb	r3, [r0]	@ zero_extendqisi2
	smulbb	r3, r2, r3
	mov	r3, r3, lsr #8
	str	r3, [r1, #44]
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	ldrb	r2, [r0]	@ zero_extendqisi2
	rsb	r3, r3, #255
	mul	r3, r2, r3
	mov	r3, r3, lsr #8
	str	r3, [r1]
	ldrb	ip, [r0]	@ zero_extendqisi2
	ldrb	r3, [r0, #2]	@ zero_extendqisi2
	rsb	ip, ip, #255
	mul	r3, r3, ip
	mov	r3, r3, lsr #8
	str	r3, [r1, #4]
	ldrb	r3, [r0, #2]	@ zero_extendqisi2
	ldrb	r2, [r0, #3]	@ zero_extendqisi2
	rsb	r3, r3, #255
	ldrb	lr, [r0, #4]	@ zero_extendqisi2
	mul	r3, ip, r3
	mov	r3, r3, lsr #8
	mul	r2, r2, r3
	mov	ip, r2, lsr #8
	mul	r2, lr, ip
	mov	r2, r2, lsr #8
	str	r2, [r1, #8]
	ldrb	r2, [r0, #4]	@ zero_extendqisi2
	ldrb	lr, [r0, #5]	@ zero_extendqisi2
	rsb	r2, r2, #255
	mul	r2, r2, ip
	mov	r2, r2, lsr #8
	mul	ip, lr, r2
	mov	ip, ip, lsr #8
	str	ip, [r1, #12]
	ldrb	ip, [r0, #5]	@ zero_extendqisi2
	rsb	ip, ip, #255
	mul	r2, ip, r2
	mov	r2, r2, lsr #8
	str	r2, [r1, #16]
	ldrb	ip, [r0, #3]	@ zero_extendqisi2
	ldrb	r2, [r0, #6]	@ zero_extendqisi2
	rsb	ip, ip, #255
	ldrb	lr, [r0, #7]	@ zero_extendqisi2
	mul	r3, ip, r3
	mov	r3, r3, lsr #8
	mul	r2, r2, r3
	mov	ip, r2, lsr #8
	mul	r2, lr, ip
	mov	r2, r2, lsr #8
	str	r2, [r1, #20]
	ldrb	r2, [r0, #7]	@ zero_extendqisi2
	rsb	r2, r2, #255
	mul	r2, r2, ip
	mov	r2, r2, lsr #8
	str	r2, [r1, #24]
	ldrb	ip, [r0, #6]	@ zero_extendqisi2
	ldrb	r2, [r0, #8]	@ zero_extendqisi2
	rsb	ip, ip, #255
	ldrb	lr, [r0, #9]	@ zero_extendqisi2
	mul	r3, ip, r3
	mov	r3, r3, lsr #8
	mul	r2, r2, r3
	mov	ip, r2, lsr #8
	mul	r2, lr, ip
	mov	r2, r2, lsr #8
	str	r2, [r1, #28]
	ldrb	r2, [r0, #9]	@ zero_extendqisi2
	rsb	r2, r2, #255
	mul	r2, r2, ip
	mov	r2, r2, lsr #8
	str	r2, [r1, #32]
	ldrb	r2, [r0, #8]	@ zero_extendqisi2
	ldrb	ip, [r0, #10]	@ zero_extendqisi2
	rsb	r2, r2, #255
	mul	r3, r2, r3
	mov	r3, r3, lsr #8
	mul	r2, ip, r3
	mov	r2, r2, lsr #8
	str	r2, [r1, #36]
	ldrb	r2, [r0, #10]	@ zero_extendqisi2
	rsb	r2, r2, #255
	mul	r3, r2, r3
	mov	r3, r3, lsr #8
	str	r3, [r1, #40]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6_BoolTreeToHuffCodes, .-VP6_BoolTreeToHuffCodes
	.align	2
	.global	VP6_ZerosBoolTreeToHuffCodes
	.type	VP6_ZerosBoolTreeToHuffCodes, %function
VP6_ZerosBoolTreeToHuffCodes:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	ip, [r0, #1]	@ zero_extendqisi2
	ldrb	r3, [r0]	@ zero_extendqisi2
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	smulbb	r3, ip, r3
	mov	r3, r3, lsr #8
	mul	r2, r2, r3
	mov	r2, r2, lsr #8
	str	r2, [r1]
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	rsb	r2, r2, #255
	mul	r3, r3, r2
	mov	r3, r3, lsr #8
	str	r3, [r1, #4]
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	ldrb	ip, [r0]	@ zero_extendqisi2
	rsb	r3, r3, #255
	ldrb	lr, [r0, #3]	@ zero_extendqisi2
	mul	r3, ip, r3
	mov	r2, r3, lsr #8
	mul	r3, lr, r2
	mov	r3, r3, lsr #8
	str	r3, [r1, #8]
	ldrb	r3, [r0, #3]	@ zero_extendqisi2
	rsb	r3, r3, #255
	mul	r3, r3, r2
	mov	r3, r3, lsr #8
	str	r3, [r1, #12]
	ldrb	r3, [r0]	@ zero_extendqisi2
	ldrb	r4, [r0, #4]	@ zero_extendqisi2
	rsb	r3, r3, #255
	ldrb	ip, [r0, #5]	@ zero_extendqisi2
	ldrb	lr, [r0, #6]	@ zero_extendqisi2
	mul	r3, r4, r3
	mov	r3, r3, lsr #8
	mul	r3, ip, r3
	mov	r2, r3, lsr #8
	mul	r3, lr, r2
	mov	r3, r3, lsr #8
	str	r3, [r1, #16]
	ldrb	r3, [r0, #6]	@ zero_extendqisi2
	rsb	r3, r3, #255
	mul	r3, r3, r2
	mov	r3, r3, lsr #8
	str	r3, [r1, #20]
	ldrb	r3, [r0]	@ zero_extendqisi2
	ldrb	lr, [r0, #4]	@ zero_extendqisi2
	rsb	r3, r3, #255
	ldrb	r2, [r0, #5]	@ zero_extendqisi2
	ldrb	ip, [r0, #7]	@ zero_extendqisi2
	mul	r3, lr, r3
	rsb	r2, r2, #255
	mov	r3, r3, lsr #8
	mul	r3, r2, r3
	mov	r3, r3, lsr #8
	mul	r2, ip, r3
	mov	r2, r2, lsr #8
	str	r2, [r1, #24]
	ldrb	r2, [r0, #7]	@ zero_extendqisi2
	rsb	r2, r2, #255
	mul	r3, r2, r3
	mov	r3, r3, lsr #8
	str	r3, [r1, #28]
	ldrb	r3, [r0, #4]	@ zero_extendqisi2
	ldrb	r2, [r0]	@ zero_extendqisi2
	rsb	r0, r3, #255
	rsb	r3, r2, #255
	mul	r3, r3, r0
	mov	r3, r3, lsr #8
	str	r3, [r1, #32]
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6_ZerosBoolTreeToHuffCodes, .-VP6_ZerosBoolTreeToHuffCodes
	.align	2
	.global	VP6_InsertSorted
	.type	VP6_InsertSorted, %function
VP6_InsertSorted:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, [r2]
	mov	r7, r1, asl #4
	cmn	r5, #1
	beq	.L232
	mov	r3, r5, asl #4
	sub	r7, r7, r1, asl #2
	sub	r3, r3, r5, asl #2
	add	r7, r0, r7
	add	r3, r0, r3
	ldr	r6, [r7, #8]
	ldr	ip, [r3, #8]
	cmp	r6, ip
	bls	.L233
	mov	r4, r5
	b	.L235
.L243:
	add	r3, r0, r3
	ldr	lr, [r3, #8]
	cmp	lr, r6
	bcs	.L242
	mov	r4, ip
.L235:
	ldr	ip, [r3, #4]
	cmn	ip, #1
	mov	r3, ip, asl #4
	sub	r3, r3, ip, asl #2
	bne	.L243
.L234:
	mov	r3, r4, asl #4
	sub	r4, r3, r4, asl #2
	add	r0, r0, r4
	str	r1, [r0, #4]
	str	ip, [r7, #4]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L242:
	cmp	r5, ip
	bne	.L234
.L237:
	str	r1, [r2]
	str	ip, [r7, #4]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L232:
	sub	r7, r7, r1, asl #2
	add	r7, r0, r7
.L233:
	mov	ip, r5
	b	.L237
	UNWIND(.fnend)
	.size	VP6_InsertSorted, .-VP6_InsertSorted
	.align	2
	.global	VP6_BuildHuffTree
	.type	VP6_BuildHuffTree, %function
VP6_BuildHuffTree:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 376
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #380)
	sub	sp, sp, #380
	subs	r10, r2, #0
	mov	r9, r0
	mov	r0, #0
	str	r0, [fp, #-408]
	beq	.L249
	sub	r6, fp, #404
	sub	r1, r1, #4
	mov	r5, #1
	mvn	r4, #0
	mov	ip, r6
.L248:
	mov	r2, r0, asl #4
	sub	r3, fp, #44
	sub	r2, r2, r0, asl #2
	ldr	lr, [r1, #4]!
	add	r2, r3, r2
	cmp	lr, #0
	ldrb	r3, [r2, #-360]	@ zero_extendqisi2
	moveq	lr, #1
	streq	r5, [r1]
	orr	r3, r3, #1
	bfi	r3, r0, #1, #4
	add	r0, r0, #1
	cmp	r0, r10
	bfc	r3, #5, #3
	strb	r3, [r2, #-360]
	stmib	ip, {r4, lr}
	add	ip, ip, #12
	bne	.L248
	cmp	r10, #1
	bls	.L249
	sub	r7, fp, #408
	mov	r4, #1
.L250:
	mov	r1, r4
	mov	r2, r7
	mov	r0, r6
	add	r4, r4, #1
	bl	VP6_InsertSorted
	cmp	r4, r10
	bne	.L250
	ldr	r1, [fp, #-408]
.L246:
	mov	r2, r1, asl #2
	mov	r1, r1, asl #4
	rsb	r3, r2, r1
	sub	r0, fp, #44
	add	r3, r0, r3
	ldr	r3, [r3, #-356]
	cmn	r3, #1
	beq	.L244
	mov	r5, r10, asl #4
	sub	r8, r10, #2
	sub	r6, fp, #404
	sub	r5, r5, r10, asl #2
	add	r8, r8, r10
	add	r9, r9, r10, lsl #1
	and	r8, r8, #15
	add	r5, r6, r5
	sub	r7, fp, #408
	mov	r4, r10
	str	r6, [fp, #-416]
.L252:
	mov	ip, r4, asl #4
	rsb	lr, r2, r1
	sub	ip, ip, r4, asl #2
	sub	r2, fp, #44
	add	ip, r2, ip
	mov	r0, r3, asl #4
	sub	r3, r0, r3, asl #2
	add	lr, r2, lr
	ldrb	r1, [ip, #-360]	@ zero_extendqisi2
	add	r3, r2, r3
	rsb	r10, r4, r8
	ldr	r0, [lr, #-352]
	and	r1, r1, #254
	ldrb	lr, [lr, #-360]	@ zero_extendqisi2
	bfi	r1, r10, #1, #4
	ldrb	r10, [r3, #-360]	@ zero_extendqisi2
	strb	r1, [ip, #-360]
	mvn	r6, #0
	ldr	ip, [r3, #-352]
	mov	r1, r4
	str	r6, [r5, #4]
	mov	r2, r7
	add	r0, ip, r0
	str	r0, [r5, #8]
	ldr	r3, [r3, #-356]
	add	r4, r4, #1
	ldr	r0, [fp, #-416]
	add	r5, r5, #12
	strb	r10, [r9, #-3]
	sub	r9, r9, #2
	strb	lr, [r9, #-2]
	str	r3, [fp, #-408]
	bl	VP6_InsertSorted
	ldr	r1, [fp, #-408]
	sub	r0, fp, #44
	mov	r2, r1, asl #2
	mov	r1, r1, asl #4
	rsb	r3, r2, r1
	add	r3, r0, r3
	ldr	r3, [r3, #-356]
	cmp	r3, r6
	bne	.L252
.L244:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L249:
	mov	r1, #0
	b	.L246
	UNWIND(.fnend)
	.size	VP6_BuildHuffTree, .-VP6_BuildHuffTree
	.align	2
	.global	VP6_BuildHuffLookupTable
	.type	VP6_BuildHuffLookupTable, %function
VP6_BuildHuffLookupTable:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, #0
.L261:
	mov	r2, #0
	mov	lr, r2
.L260:
	rsb	r3, lr, #5
	add	r5, r0, r2, lsl #1
	mov	r3, r4, asr r3
	add	ip, lr, #1
	tst	r3, #1
	mov	lr, ip
	ldrneb	r3, [r5, #1]	@ zero_extendqisi2
	ldreqb	r3, [r0, r2, asl #1]	@ zero_extendqisi2
	and	r5, r3, #1
	ubfx	r2, r3, #1, #4
	bfi	r3, r5, #0, #1
	bfi	r3, r2, #1, #4
	eor	r3, r3, #1
	cmp	ip, #5
	movhi	r3, #0
	andls	r3, r3, #1
	cmp	r3, #0
	bne	.L260
	ldrb	r3, [r1, r4]	@ zero_extendqisi2
	bfi	r3, r2, #4, #4
	bfi	r3, ip, #1, #3
	bfi	r3, r5, #0, #1
	strb	r3, [r1, r4]
	add	r4, r4, #1
	cmp	r4, #64
	bne	.L261
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6_BuildHuffLookupTable, .-VP6_BuildHuffLookupTable
	.align	2
	.global	VP6_BuildTreeTab
	.type	VP6_BuildTreeTab, %function
VP6_BuildTreeTab:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #9088
	add	r7, r0, #9920
	add	r6, r6, #30
	add	r7, r7, #62
	mov	r1, #0
.L265:
	sub	r5, r6, #432
.L274:
	sub	r4, r5, #96
.L272:
	ldrb	lr, [r4, #-2]	@ zero_extendqisi2
	add	r3, r0, r1
	add	ip, r1, #1
	mov	r2, r4
	add	ip, r0, ip
	add	r1, r1, #2
	strb	lr, [r3, #1218]
	mov	r3, #0
	ldrb	lr, [r4, #-1]	@ zero_extendqisi2
	strb	lr, [ip, #1218]
	b	.L270
.L268:
	ldrb	lr, [r2]	@ zero_extendqisi2
	cmp	r3, #5
	cmpne	r3, #11
	addne	r1, r1, #2
	addeq	r1, r1, #6
	add	r2, r2, #2
	strb	lr, [ip, #1218]
	ldrb	lr, [r2, #-1]	@ zero_extendqisi2
	strb	lr, [ip, #1219]
.L270:
	add	r3, r3, #1
	add	ip, r0, r1
	cmp	r3, #12
	bne	.L268
	add	r4, r4, #24
	cmp	r4, r5
	bne	.L272
	add	r5, r4, #144
	cmp	r6, r5
	bne	.L274
	add	r6, r4, #576
	cmp	r6, r7
	bne	.L265
	add	r4, r0, #9408
	add	r5, r0, #9472
	add	r4, r4, #46
	add	r5, r5, #30
	mov	r1, #768
.L273:
	ldrb	lr, [r4, #-2]	@ zero_extendqisi2
	add	r3, r0, r1
	add	ip, r1, #1
	mov	r2, r4
	add	ip, r0, ip
	add	r1, r1, #2
	strb	lr, [r3, #1218]
	mov	r3, #0
	ldrb	lr, [r4, #-1]	@ zero_extendqisi2
	strb	lr, [ip, #1218]
	b	.L279
.L277:
	ldrb	lr, [r2]	@ zero_extendqisi2
	cmp	r3, #5
	cmpne	r3, #11
	addne	r1, r1, #2
	addeq	r1, r1, #6
	add	r2, r2, #2
	strb	lr, [ip, #1218]
	ldrb	lr, [r2, #-1]	@ zero_extendqisi2
	strb	lr, [ip, #1219]
.L279:
	add	r3, r3, #1
	add	ip, r0, r1
	cmp	r3, #12
	bne	.L277
	add	r4, r4, #24
	cmp	r4, r5
	bne	.L273
	add	r4, r0, #8512
	add	r5, r0, #8576
	add	r4, r4, #20
	add	r5, r5, #12
	mov	ip, #832
.L278:
	mov	r1, r4
	mov	r3, #0
.L284:
	ldrb	lr, [r1]	@ zero_extendqisi2
	add	r2, r0, ip
	cmp	r3, #5
	cmpne	r3, #11
	addeq	ip, ip, #6
	strb	lr, [r2, #1218]
	ldrb	lr, [r1, #1]	@ zero_extendqisi2
	strb	lr, [r2, #1219]
	beq	.L281
	cmp	r3, #13
	beq	.L289
	add	ip, ip, #2
.L281:
	add	r3, r3, #1
	add	r1, r1, #2
	cmp	r3, #14
	bne	.L284
.L283:
	add	r4, r4, #28
	cmp	r4, r5
	bne	.L278
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L289:
	add	ip, ip, #30
	b	.L283
	UNWIND(.fnend)
	.size	VP6_BuildTreeTab, .-VP6_BuildTreeTab
	.align	2
	.global	VP6_ConvertBoolToHuff
	.type	VP6_ConvertBoolToHuff, %function
VP6_ConvertBoolToHuff:
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
	ldr	r3, .L297
	add	r6, r0, #6592
	mov	r7, r0
	add	r6, r6, #4
	ldr	r1, .L297+4
	ldr	r3, [r3, #68]
	mov	r0, #22
	add	r5, r7, #9408
	blx	r3
	add	r5, r5, #44
	add	r0, r7, #512
	mov	r1, r6
	add	r0, r0, #2
	bl	VP6_BoolTreeToHuffCodes
	mov	r2, #12
	mov	r1, r6
	mov	r0, r5
	bl	VP6_BuildHuffTree
	add	r4, r7, #6592
	add	r1, r7, #9472
	mov	r0, r5
	add	r4, r4, #52
	add	r1, r1, #28
	add	r6, r7, #9472
	bl	VP6_BuildHuffLookupTable
	add	r6, r6, #4
	add	r0, r7, #524
	mov	r1, r4
	add	r0, r0, #1
	bl	VP6_BoolTreeToHuffCodes
	add	r5, r7, #6656
	mov	r2, #12
	mov	r1, r4
	mov	r0, r6
	add	r5, r5, #36
	bl	VP6_BuildHuffTree
	add	r4, r7, #8512
	add	r1, r7, #9536
	mov	r0, r6
	add	r4, r4, #20
	add	r1, r1, #28
	bl	VP6_BuildHuffLookupTable
	mov	r1, r5
	add	r0, r7, #536
	bl	VP6_ZerosBoolTreeToHuffCodes
	mov	r2, #9
	mov	r1, r5
	mov	r0, r4
	bl	VP6_BuildHuffTree
	add	r5, r7, #6720
	add	r1, r7, #11904
	mov	r0, r4
	add	r5, r5, #28
	add	r1, r1, #28
	add	r4, r7, #8512
	bl	VP6_BuildHuffLookupTable
	add	r4, r4, #48
	add	r0, r7, #548
	mov	r1, r5
	add	r0, r0, #2
	bl	VP6_ZerosBoolTreeToHuffCodes
	mov	r1, r5
	mov	r0, r4
	mov	r2, #9
	bl	VP6_BuildHuffTree
	add	r3, r7, #760
	mov	r0, r4
	add	r1, r7, #11968
	mov	r4, r3
	add	r3, r7, #6784
	add	r1, r1, #28
	mov	r5, r3
	add	r3, r7, #8576
	mov	r6, r3
	add	r3, r7, #9600
	mov	r8, r3
	add	r3, r7, #1152
	mov	r9, r3
	bl	VP6_BuildHuffLookupTable
	add	r3, r4, #2
	str	r7, [fp, #-72]
	str	r3, [fp, #-52]
	add	r3, r5, #20
	str	r3, [fp, #-60]
	add	r3, r6, #12
	str	r3, [fp, #-64]
	add	r3, r8, #28
	str	r3, [fp, #-56]
	add	r3, r9, #6
	str	r3, [fp, #-68]
.L291:
	ldr	r3, [fp, #-52]
	ldr	r9, [fp, #-56]
	sub	r10, r3, #198
	ldr	r8, [fp, #-64]
	ldr	r3, [fp, #-60]
	str	r3, [fp, #-48]
.L295:
	ldr	r7, [fp, #-48]
	add	r6, r10, #66
	mov	r5, r9
	mov	r4, r8
.L292:
	mov	r1, r7
	mov	r0, r10
	bl	VP6_BoolTreeToHuffCodes
	mov	r1, r7
	mov	r0, r4
	mov	r2, #12
	bl	VP6_BuildHuffTree
	add	r10, r10, #11
	mov	r1, r5
	mov	r0, r4
	bl	VP6_BuildHuffLookupTable
	cmp	r10, r6
	add	r7, r7, #48
	add	r5, r5, #64
	add	r4, r4, #24
	bne	.L292
	ldr	r3, [fp, #-52]
	add	r8, r8, #144
	add	r9, r9, #384
	cmp	r3, r10
	ldr	r3, [fp, #-48]
	add	r3, r3, #288
	str	r3, [fp, #-48]
	bne	.L295
	ldr	r2, [fp, #-60]
	ldr	r3, [fp, #-52]
	add	r2, r2, #864
	str	r2, [fp, #-60]
	ldr	r2, [fp, #-68]
	add	r3, r3, #198
	str	r3, [fp, #-52]
	cmp	r3, r2
	ldr	r3, [fp, #-64]
	add	r3, r3, #432
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-56]
	add	r3, r3, #1152
	str	r3, [fp, #-56]
	bne	.L291
	ldr	r7, [fp, #-72]
	mov	r0, r7
	bl	VP6_BuildTreeTab
	ldr	r3, .L297
	ldr	r1, .L297+8
	mov	r0, #22
	ldr	r3, [r3, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L298:
	.align	2
.L297:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC8
	.word	.LC9
	UNWIND(.fnend)
	.size	VP6_ConvertBoolToHuff, .-VP6_ConvertBoolToHuff
	.align	2
	.global	VP6_IntraFrmHeader
	.type	VP6_IntraFrmHeader, %function
VP6_IntraFrmHeader:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r9, .L338
	mov	r5, r1
	mov	r4, r0
	ldr	r1, .L338+4
	mov	r0, #22
	ldr	r3, [r9, #68]
	add	r7, r4, #8192
	blx	r3
	mov	r1, #5
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	uxtb	r3, r0
	mov	r0, r5
	str	r3, [r7, #4072]
	bl	BsGet
	ldr	r2, [r7, #4072]
	cmp	r2, #8
	uxtb	r0, r0
	str	r0, [r7, #4076]
	bhi	.L334
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r6, r0
	cmp	r6, #0
	bne	.L335
	ldrb	r3, [r7, #4062]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L303
	mov	r1, #16
	mov	r0, r5
	bl	BsGet
	add	r3, r4, #12288
	mov	r2, #255
	str	r3, [fp, #-48]
	mov	ip, r3
	mov	r1, #24
	mov	r3, #8
	str	r6, [ip, #36]
	str	r2, [ip, #40]
	mov	r6, ip
	str	r3, [ip, #48]
	str	r0, [ip, #16]
	mov	r0, r5
	bl	BsGet
	str	r0, [r6, #44]
.L304:
	add	r4, r4, #12288
	mov	r8, #0
	add	r4, r4, #36
	mov	r6, #7
.L305:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r8, r8, r0, asl r6
	subs	r6, r6, #1
	bcs	.L305
	mov	r6, #0
	mov	r10, #7
	uxtb	r3, r8
	str	r3, [r7, #4084]
.L306:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r6, r6, r0, asl r10
	subs	r10, r10, #1
	bcs	.L306
	ldr	r3, [r7, #4084]
	uxtb	r2, r6
	mov	r10, #0
	mov	r6, #7
	str	r2, [r7, #4088]
	mov	r8, r2, asl #1
	mov	r3, r3, asl #1
	str	r3, [r7, #4092]
	str	r3, [fp, #-52]
	ldr	r3, [fp, #-48]
	str	r8, [r3]
.L307:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r6
	subs	r6, r6, #1
	bcs	.L307
	ldr	r3, [fp, #-48]
	uxtb	r1, r10
	mov	r6, #0
	mov	r10, #7
	mov	r1, r1, asl #1
	str	r1, [r3, #4]
.L308:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r6, r6, r0, asl r10
	subs	r10, r10, #1
	bcs	.L308
	ldr	r3, [fp, #-48]
	uxtb	r2, r6
	mov	r1, r4
	mov	r0, r5
	mov	r2, r2, asl #1
	str	r2, [r3, #8]
	bl	Vp6_Vfmw_DecodeBool128
	mov	r1, r4
	str	r0, [fp, #-56]
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	ldr	r2, [fp, #-52]
	ldr	r3, [r7, #3872]
	cmp	r2, r3
	ldr	r2, [fp, #-56]
	orr	r0, r0, r2, asl #1
	ldr	r2, [r7, #3868]
	strb	r0, [r7, #4063]
	bne	.L310
	cmp	r8, r2
	beq	.L311
.L310:
	ldr	r1, [fp, #-52]
	mov	r0, #22
	str	r8, [sp]
	ldr	ip, [r9, #68]
	str	r1, [sp, #4]
	ldr	r1, .L338+8
	blx	ip
.L311:
	ldr	r3, [r7, #4076]
	ldr	r2, [fp, #-52]
	cmp	r3, #0
	str	r8, [r7, #3868]
	str	r2, [r7, #3872]
	bne	.L336
	mov	r2, #14
	strb	r2, [r7, #4066]
	ldr	r2, [fp, #-48]
	str	r3, [r2, #20]
.L320:
	ldr	r3, [r9, #68]
	mov	r0, #22
	ldr	r1, .L338+12
	blx	r3
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L334:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L338+16
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L303:
	add	r2, r4, #12288
	mov	ip, #255
	str	r2, [fp, #-48]
	mov	r1, #24
	mov	r6, r2
	mov	r0, r5
	mov	r2, #8
	str	r3, [r6, #36]
	str	ip, [r6, #40]
	str	r2, [r6, #48]
	bl	BsGet
	str	r0, [r6, #44]
	b	.L304
.L335:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L338+20
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L336:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	cmp	r0, #0
	beq	.L313
	ldr	r2, [fp, #-48]
	mov	r10, #0
	mov	r6, #4
	mov	r3, #2
	str	r3, [r2, #20]
.L314:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r10, r10, r0, asl r6
	subs	r6, r6, #1
	bcs	.L314
	ldr	r3, [r7, #4072]
	mov	r8, #0
	mov	r6, #2
	cmp	r3, #7
	movhi	r3, r8
	movls	r3, #5
	mov	r10, r10, asl r3
	strb	r10, [r7, #4064]
.L316:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r8, r8, r0, asl r6
	subs	r6, r6, #1
	bcs	.L316
	strb	r8, [r7, #4065]
.L317:
	ldr	r3, [r7, #4072]
	cmp	r3, #7
	bls	.L337
	mov	r8, #0
	mov	r6, #3
.L319:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	orr	r8, r8, r0, asl r6
	subs	r6, r6, #1
	bcs	.L319
	strb	r8, [r7, #4066]
	b	.L320
.L337:
	mov	r3, #16
	strb	r3, [r7, #4066]
	b	.L320
.L313:
	mov	r1, r4
	mov	r0, r5
	bl	Vp6_Vfmw_DecodeBool128
	cmp	r0, #0
	ldrne	r2, [fp, #-48]
	movne	r3, #1
	ldreq	r3, [fp, #-48]
	strne	r3, [r2, #20]
	streq	r0, [r3, #20]
	b	.L317
.L339:
	.align	2
.L338:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC10
	.word	.LC13
	.word	.LC14
	.word	.LC11
	.word	.LC12
	UNWIND(.fnend)
	.size	VP6_IntraFrmHeader, .-VP6_IntraFrmHeader
	.align	2
	.global	VP6_InterFrmHeader
	.type	VP6_InterFrmHeader, %function
VP6_InterFrmHeader:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #8192
	mov	r5, r0
	mov	r7, r1
	ldr	r2, [r4, #3928]
	cmp	r2, #0
	beq	.L362
	ldrb	r3, [r4, #4062]	@ zero_extendqisi2
	ldr	r1, [r4, #3932]
	cmp	r3, #0
	str	r2, [r4, #4072]
	str	r1, [r4, #4076]
	bne	.L363
	add	r6, r0, #12288
	mov	ip, #255
	mov	r2, #8
	mov	r1, #24
	str	r3, [r6, #36]
	mov	r0, r7
	str	ip, [r6, #40]
	str	r2, [r6, #48]
	bl	BsGet
	str	r0, [r6, #44]
.L344:
	add	r5, r5, #12288
	mov	r0, r7
	add	r5, r5, #36
	mov	r1, r5
	bl	Vp6_Vfmw_DecodeBool128
	str	r0, [r6, #24]
	ldr	r3, [r4, #3932]
	str	r0, [r4, #3892]
	cmp	r3, #0
	bne	.L364
	mov	r2, #14
	mov	r0, #1
	strb	r2, [r4, #4066]
	str	r3, [r6, #20]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L363:
	mov	r1, #16
	mov	r0, r7
	bl	BsGet
	add	r6, r5, #12288
	mov	r3, #0
	mov	r2, #255
	str	r3, [r6, #36]
	mov	r1, #24
	mov	r3, #8
	str	r2, [r6, #40]
	str	r3, [r6, #48]
	str	r0, [r6, #16]
	mov	r0, r7
	bl	BsGet
	str	r0, [r6, #44]
	b	.L344
.L364:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	cmp	r0, #0
	mov	r8, r0
	str	r0, [r6, #28]
	bne	.L365
.L346:
	ldr	r3, [r4, #3928]
	cmp	r3, #7
	bhi	.L366
	mov	r3, #16
	mov	r0, #1
	strb	r3, [r4, #4066]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L366:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	cmp	r0, #0
	bne	.L367
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L365:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	orr	r0, r0, r8, asl #1
	str	r0, [r6, #28]
	b	.L346
.L367:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	cmp	r0, #0
	beq	.L348
	mov	r9, #0
	mov	r8, #4
	mov	r3, #2
	str	r3, [r6, #20]
.L349:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	orr	r9, r9, r0, asl r8
	subs	r8, r8, #1
	bcs	.L349
	mov	r8, #0
	mov	r6, #2
	strb	r9, [r4, #4064]
.L350:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	orr	r8, r8, r0, asl r6
	subs	r6, r6, #1
	bcs	.L350
	strb	r8, [r4, #4065]
.L351:
	mov	r8, #0
	mov	r6, #3
.L353:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	orr	r8, r8, r0, asl r6
	subs	r6, r6, #1
	bcs	.L353
	strb	r8, [r4, #4066]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L362:
	ldr	ip, .L368
	mov	r0, #1
	ldr	r3, [r4, #4072]
	ldr	r1, .L368+4
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #87
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L348:
	mov	r1, r5
	mov	r0, r7
	bl	Vp6_Vfmw_DecodeBool128
	cmp	r0, #0
	streq	r0, [r6, #20]
	movne	r3, #1
	strne	r3, [r6, #20]
	b	.L351
.L369:
	.align	2
.L368:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC15
	UNWIND(.fnend)
	.size	VP6_InterFrmHeader, .-VP6_InterFrmHeader
	.align	2
	.global	VP6_GetPictureHeader
	.type	VP6_GetPictureHeader, %function
VP6_GetPictureHeader:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	cmpne	r0, #0
	mov	r7, r0
	mov	r6, r1
	beq	.L376
	ldr	r8, .L379
	mov	r0, #22
	ldr	r1, .L379+4
	add	r4, r7, #8192
	ldr	r3, [r8, #68]
	blx	r3
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	mov	r1, #6
	uxtb	r3, r0
	mov	r0, r6
	str	r3, [r4, #4080]
	strb	r3, [r4, #3888]
	bl	BsGet
	ldr	r2, .L379+8
	add	ip, r7, #12224
	mov	r1, #1
	mov	lr, r0
	uxtb	r3, r0
	ldr	r0, .L379+12
	add	r2, r2, r3
	mov	r3, r3, asl #1
	strb	lr, [r4, #4060]
	add	r5, r0, #128
	ldrb	r2, [r2, #1656]	@ zero_extendqisi2
	ldrh	r9, [r0, r3]
	mov	r0, r6
	ldrh	r3, [r5, r3]
	strb	r2, [r4, #4061]
	strh	r9, [ip, #36]	@ movhi
	strh	r3, [ip, #38]	@ movhi
	bl	BsGet
	ldr	r3, [r4, #4080]
	mov	r1, r6
	cmp	r3, #0
	strb	r0, [r4, #4062]
	mov	r0, r7
	beq	.L377
	bl	VP6_InterFrmHeader
	mov	r5, r0
	cmp	r5, #1
	beq	.L378
.L375:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L377:
	bl	VP6_IntraFrmHeader
	mov	r5, r0
	cmp	r5, #1
	bne	.L375
.L378:
	add	r1, r7, #12288
	mov	r0, r6
	add	r1, r1, #36
	add	r7, r7, #12288
	bl	Vp6_Vfmw_DecodeBool128
	ldr	r3, [r8, #68]
	ldr	r1, .L379+16
	str	r0, [r7, #32]
	mov	r0, #22
	ldrb	r2, [r4, #4060]	@ zero_extendqisi2
	str	r2, [r7, #12]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L376:
	ldr	r3, .L379
	mov	r0, #22
	ldr	r1, .L379+20
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L380:
	.align	2
.L379:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC17
	.word	.LANCHOR0
	.word	.LANCHOR0+1720
	.word	.LC18
	.word	.LC16
	UNWIND(.fnend)
	.size	VP6_GetPictureHeader, .-VP6_GetPictureHeader
	.align	2
	.global	VP6DEC_Init
	.type	VP6DEC_Init, %function
VP6DEC_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L385
	add	r5, r0, #12288
	movw	r2, #13008
	mov	r7, r1
	ldr	r8, [r5, #704]
	mov	r1, #0
	ldr	r3, [r6, #48]
	mov	r4, r0
	blx	r3
	add	r0, r4, #12608
	add	r0, r0, #40
	str	r8, [r5, #704]
	bl	ResetVoQueue
	str	r7, [r4]
	mov	r0, r4
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r5, #712]
	beq	.L384
	ldr	r3, [r6, #48]
	movw	r2, #12396
	mov	r1, #0
	add	r0, r4, #220
	blx	r3
	mov	r2, #1024
	mov	r3, #0
	str	r2, [r5, #36]
	str	r3, [r5, #8]
	mov	r0, #22
	ldr	r2, [r4]
	ldr	r3, [r6, #68]
	ldr	r1, .L385+4
	ldr	r2, [r2, #8]
	str	r2, [r5]
	blx	r3
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L384:
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r1, .L385+8
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L386:
	.align	2
.L385:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC20
	.word	.LC19
	UNWIND(.fnend)
	.size	VP6DEC_Init, .-VP6DEC_Init
	.align	2
	.global	VP6DEC_Destroy
	.type	VP6DEC_Destroy, %function
VP6DEC_Destroy:
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
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6DEC_Destroy, .-VP6DEC_Destroy
	.align	2
	.global	VP6_DecodeDecide
	.type	VP6_DecodeDecide, %function
VP6_DecodeDecide:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #12288
	mov	r6, r0
	ldr	r5, [r4, #8]
	cmp	r5, #0
	beq	.L414
	ldr	r3, [r4]
	sub	r3, r3, #2
	cmp	r3, #1
	ldr	r3, [r0]
	ldr	r2, [r3, #8]
	bls	.L415
.L392:
	str	r2, [r4]
	cmp	r2, #4
	ldrls	pc, [pc, r2, asl #2]
	b	.L397
.L398:
	.word	.L396
	.word	.L396
	.word	.L393
	.word	.L395
	.word	.L396
.L416:
	ldr	r3, .L418
	mov	r0, #18
	ldr	r1, .L418+4
	ldr	r3, [r3, #68]
	blx	r3
.L395:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L415:
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L392
	ldr	r3, .L418
	mov	r0, #2
	ldr	r1, .L418+8
	mov	r5, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r2, #1
	ldr	r1, [r4, #24]
	ldr	r0, [r4, #712]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #712]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #24]
	str	r5, [r4, #28]
	ldr	r0, [r4, #712]
	bl	FSP_GetNullLogicFs
	str	r5, [r4, #4]
	str	r0, [r4, #28]
	str	r0, [r4, #24]
	ldr	r3, [r6]
	ldr	r2, [r3, #8]
	b	.L392
.L414:
	ldr	r3, .L418
	mov	r0, #1
	ldr	r1, .L418+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L393:
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L416
.L394:
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #712]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L417
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L403
	ldr	r3, [r6]
	ldr	r2, [r5, #200]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L403
	ldr	ip, .L418
	mov	r0, #1
	ldr	r1, .L418+16
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L396:
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L394
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne	.L394
	ldr	ip, .L418
	mov	r0, #18
	ldr	r2, .L418+20
	ldr	r1, .L418+24
	ldr	ip, [ip, #68]
	blx	ip
	b	.L395
.L397:
	ldr	r3, .L418
	mov	r0, #1
	ldr	r1, .L418+28
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L403:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L417:
	ldr	ip, .L418
	movw	r3, #2161
	ldr	r2, .L418+32
	mov	r0, #22
	ldr	r1, .L418+36
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L419:
	.align	2
.L418:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC23
	.word	.LC22
	.word	.LC21
	.word	.LC28
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LANCHOR0+1976
	.word	.LC27
	UNWIND(.fnend)
	.size	VP6_DecodeDecide, .-VP6_DecodeDecide
	.align	2
	.global	VP6_Set_DecParam
	.type	VP6_Set_DecParam, %function
VP6_Set_DecParam:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r6, r0, #12288
	mov	r4, r0
	add	r5, r0, #8
	ldrb	r3, [r6, #12]	@ zero_extendqisi2
	ldr	r8, [r0, #204]
	cmp	r3, #3
	beq	.L442
	ldr	r10, .L447
	mov	r2, #196
	mov	r1, #0
	mov	r0, r5
	mov	r7, r1
	ldr	r3, [r10, #48]
	blx	r3
	ldr	r3, [r6, #204]
	strb	r7, [r4, #9]
	str	r3, [r4, #68]
	ldrb	r3, [r6, #184]	@ zero_extendqisi2
	str	r3, [r4, #72]
	ldrb	r3, [r6, #186]	@ zero_extendqisi2
	str	r3, [r4, #76]
	ldr	r3, [r6, #252]
	str	r3, [r4, #80]
	ldr	r3, [r6, #240]
	str	r3, [r4, #92]
	ldrb	r3, [r6, #188]	@ zero_extendqisi2
	str	r3, [r4, #96]
	ldrb	r3, [r6, #189]	@ zero_extendqisi2
	str	r3, [r4, #100]
	ldrb	r3, [r6, #190]	@ zero_extendqisi2
	str	r3, [r4, #104]
	ldr	r3, [r6, #248]
	str	r3, [r4, #116]
	ldrb	r3, [r6, #185]	@ zero_extendqisi2
	strb	r3, [r4, #8]
	ldrh	r3, [r6, #192]
	str	r3, [r4, #84]
	ldrh	r3, [r6, #194]
	str	r3, [r4, #88]
	ldr	r3, [r6, #200]
	str	r3, [r4, #112]
	ldr	r3, [r6, #196]
	str	r3, [r4, #108]
	ldr	r3, [r6, #212]
	strh	r3, [r4, #60]	@ movhi
	ldr	r3, [r6, #208]
	strh	r3, [r4, #62]	@ movhi
	ldr	r1, [r6, #20]
	ldr	r0, [r6, #712]
	bl	FSP_GetLogicFs
	ldr	r1, [r6, #28]
	mov	r9, r0
	ldr	r0, [r6, #712]
	bl	FSP_GetLogicFs
	ldr	r1, [r6, #24]
	str	r0, [fp, #-48]
	ldr	r0, [r6, #712]
	bl	FSP_GetLogicFs
	ldr	r2, [fp, #-48]
	cmp	r9, r7
	cmpne	r2, r7
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, r7
	orreq	r3, r3, #1
	cmp	r3, r7
	bne	.L443
	ldr	r7, [r9, #520]
	cmp	r7, #0
	beq	.L444
	ldr	r3, [r7, #12]
	add	r7, r4, #12608
	add	r7, r7, #8
	str	r3, [r4, #180]
	ldr	r3, [r9, #532]
	str	r3, [r4, #192]
	ldr	r3, [r0, #520]
	mov	r0, r7
	cmp	r3, #0
	ldreq	r3, [r9, #520]
	ldr	r3, [r3, #12]
	str	r3, [r4, #184]
	ldr	r3, [r2, #520]
	cmp	r3, #0
	ldreq	r3, [r9, #520]
	ldr	r3, [r3, #12]
	str	r3, [r4, #188]
	ldr	r3, [r6, #36]
	str	r3, [r4, #196]
	ldr	r3, [r6, #264]
	str	r3, [r4, #168]
	ldr	r3, [r6, #260]
	str	r3, [r4, #172]
	ldr	r3, [r6, #268]
	str	r3, [r4, #176]
	ldr	r9, [r8]
	bl	BsPos
	mov	r3, r0
	add	r0, r0, #7
	cmp	r3, #0
	movlt	r3, r0
	add	r9, r9, r3, asr #3
	mov	r0, r9
	bl	MEM_Vir2Phy
	bic	r3, r0, #15
	mov	r0, r9
	sub	r3, r3, #16
	str	r3, [r4, #64]
	bl	MEM_Vir2Phy
	ldrb	r9, [r6, #186]	@ zero_extendqisi2
	cmp	r9, #0
	and	r0, r0, #15
	mov	r0, r0, asl #3
	bne	.L435
	ldr	r3, [r6, #200]
	cmp	r3, #0
	bne	.L436
.L435:
	mov	r3, #16
	str	r0, [r4, #124]
	str	r3, [r4, #128]
	mov	r0, r7
	ldr	r9, [r6, #236]
	bl	BsPos
	mov	r7, #0
	str	r7, [r4, #140]
	str	r7, [r4, #136]
	mov	r9, r9, asl #3
	str	r7, [r4, #132]
	rsb	r0, r0, r9
	str	r0, [r4, #120]
	ldr	r3, [r8]
	ldr	r10, [r6, #236]
	add	r10, r3, r10
	mov	r0, r10
	bl	MEM_Vir2Phy
	mov	r9, r0
	mov	r0, r10
	bl	MEM_Vir2Phy
	and	r9, r9, #15
	ldr	r3, [r4, #64]
	mov	r9, r9, asl #3
	str	r9, [r4, #148]
	bic	r0, r0, #15
	rsb	r0, r3, r0
	str	r0, [r4, #152]
	ldr	r3, [r6, #236]
	ldr	r2, [r8, #8]
	str	r7, [r4, #164]
	mov	r3, r3, asl #3
	str	r7, [r4, #160]
	rsb	r3, r3, r2, lsl #3
	str	r7, [r4, #156]
	str	r3, [r4, #144]
.L437:
	add	r3, r4, #5120
	add	r10, r4, #4352
	add	r3, r3, #30
	str	r3, [r4, #52]
	add	r3, r4, #4928
	add	r9, r4, #4992
	add	r3, r3, #62
	str	r3, [r4, #44]
	ldr	r3, [r4]
	add	r8, r4, #4288
	add	r7, r4, #3280
	add	r6, r4, #3344
	add	lr, r4, #3216
	add	ip, r4, #2512
	add	r0, r4, #2448
	add	r1, r4, #3152
	add	r2, r4, #6656
	add	r0, r0, #14
	add	r10, r10, #62
	str	r0, [r4, #12]
	str	r10, [r4, #40]
	add	r9, r9, #30
	add	r8, r8, #30
	str	r9, [r4, #48]
	str	r8, [r4, #36]
	add	r7, r7, #14
	add	r6, r6, #14
	str	r7, [r4, #28]
	str	r6, [r4, #32]
	add	lr, lr, #14
	add	ip, ip, #14
	str	lr, [r4, #24]
	str	ip, [r4, #16]
	add	r1, r1, #14
	add	r2, r2, #30
	str	r1, [r4, #20]
	str	r2, [r4, #56]
	mov	r0, #1
	ldr	r3, [r3, #676]
	str	r5, [r4, #4]
	str	r3, [r4, #200]
	ldr	r3, [r4]
	ldr	r3, [r3, #676]
	str	r3, [r4, #200]
.L423:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L442:
	str	r3, [r0, #68]
	mov	r9, #1
	strb	r9, [r0, #9]
	ldr	r1, [r6, #20]
	ldr	r0, [r6, #712]
	bl	FSP_GetLogicFs
	ldr	r1, [r6, #28]
	mov	r7, r0
	ldr	r0, [r6, #712]
	bl	FSP_GetLogicFs
	ldr	r1, [r6, #24]
	mov	r8, r0
	ldr	r0, [r6, #712]
	bl	FSP_GetLogicFs
	cmp	r7, #0
	cmpne	r8, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L445
	ldr	r6, [r7, #520]
	cmp	r6, #0
	beq	.L446
	ldr	r3, [r6, #12]
	str	r3, [r4, #180]
	ldr	r3, [r7, #532]
	str	r3, [r4, #192]
	ldr	r3, [r0, #520]
	mov	r0, #1
	cmp	r3, #0
	ldreq	r3, [r7, #520]
	ldr	r3, [r3, #12]
	str	r3, [r4, #184]
	ldr	r3, [r8, #520]
	cmp	r3, #0
	ldreq	r3, [r7, #520]
	ldr	r3, [r3, #12]
	str	r5, [r4, #4]
	str	r3, [r4, #188]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L443:
	ldr	r3, [r10, #68]
	mov	r0, #1
	ldr	r1, .L447+4
	blx	r3
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L436:
	str	r0, [r4, #124]
	mov	r3, #16
	mov	r0, r7
	str	r3, [r4, #128]
	ldr	r6, [r8, #8]
	bl	BsPos
	str	r9, [r4, #140]
	str	r9, [r4, #136]
	mov	r6, r6, asl #3
	str	r9, [r4, #132]
	str	r9, [r4, #152]
	str	r9, [r4, #148]
	str	r9, [r4, #144]
	str	r9, [r4, #164]
	str	r9, [r4, #160]
	str	r9, [r4, #156]
	rsb	r0, r0, r6
	str	r0, [r4, #120]
	b	.L437
.L445:
	ldr	r3, .L447
	mov	r0, r9
	ldr	r1, .L447+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	b	.L423
.L444:
	ldr	r3, [r10, #68]
	mov	r0, #1
	ldr	r1, .L447+8
	blx	r3
	mov	r0, r7
	b	.L423
.L446:
	ldr	r3, .L447
	mov	r0, r9
	ldr	r1, .L447+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r6
	b	.L423
.L448:
	.align	2
.L447:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC29
	.word	.LC30
	UNWIND(.fnend)
	.size	VP6_Set_DecParam, .-VP6_Set_DecParam
	.align	2
	.global	VP6_CfgTable
	.type	VP6_CfgTable, %function
VP6_CfgTable:
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
	ldr	r5, .L474
	add	r4, r0, #2240
	add	r7, r0, #2288
	add	r4, r4, #2
	add	r7, r7, #2
	add	r6, r0, #4
	mov	r9, r0
	mov	r3, #0
	str	r1, [fp, #-64]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-52]
	str	r3, [fp, #-48]
.L450:
	mov	r1, r6
	mov	r0, r4
	ldr	r3, [r5, #52]
	mov	r2, #10
	add	r4, r4, #16
	blx	r3
	cmp	r4, r7
	add	r6, r6, #10
	bne	.L450
	add	r3, r9, #2944
	add	r7, r9, #2304
	add	r3, r3, #2
	add	r7, r7, #2
	add	r8, r9, #44
	str	r9, [fp, #-68]
	mov	r10, r3
.L451:
	add	r6, r7, #160
	mov	r9, r8
.L452:
	mov	r1, r9
	mov	r0, r7
	ldr	ip, [r5, #52]
	mov	r2, #9
	add	r7, r7, #16
	blx	ip
	cmp	r7, r6
	add	r9, r9, #9
	ldr	r4, .L474
	bne	.L452
	cmp	r7, r10
	add	r8, r8, #90
	bne	.L451
	ldr	r9, [fp, #-68]
	mov	r1, #0
	ldr	r3, [r4, #48]
	mov	r2, #16
	sub	r0, fp, #60
	mov	r6, r1
	blx	r3
	ldrb	r10, [r9]	@ zero_extendqisi2
	ldrb	r8, [r9, #2]	@ zero_extendqisi2
	mov	r0, r7
	ldrb	r7, [r9, #484]	@ zero_extendqisi2
	sub	r1, fp, #60
	ldrb	lr, [r9, #485]	@ zero_extendqisi2
	mov	r2, #16
	ldrb	ip, [r9, #486]	@ zero_extendqisi2
	ldrb	r3, [r9, #487]	@ zero_extendqisi2
	strb	r10, [fp, #-60]
	strb	r8, [fp, #-59]
	ldrb	r10, [r9, #488]	@ zero_extendqisi2
	ldrb	r8, [r9, #489]	@ zero_extendqisi2
	strb	r7, [fp, #-58]
	ldrb	r7, [r9, #490]	@ zero_extendqisi2
	strb	lr, [fp, #-57]
	strb	ip, [fp, #-56]
	strb	r3, [fp, #-55]
	strb	r10, [fp, #-54]
	strb	r8, [fp, #-53]
	strb	r7, [fp, #-52]
	ldr	r3, [r4, #52]
	blx	r3
	mov	r1, r6
	ldr	r3, [r4, #48]
	mov	r2, #16
	sub	r0, fp, #60
	blx	r3
	ldrb	lr, [r9]	@ zero_extendqisi2
	ldrb	ip, [r9, #498]	@ zero_extendqisi2
	add	r0, r9, #2960
	ldrb	r3, [r9, #499]	@ zero_extendqisi2
	sub	r1, fp, #60
	ldrb	r10, [r9, #500]	@ zero_extendqisi2
	mov	r2, #16
	ldrb	r8, [r9, #501]	@ zero_extendqisi2
	add	r0, r0, #2
	ldrb	r7, [r9, #502]	@ zero_extendqisi2
	strb	lr, [fp, #-60]
	strb	ip, [fp, #-59]
	ldrb	lr, [r9, #503]	@ zero_extendqisi2
	ldrb	ip, [r9, #504]	@ zero_extendqisi2
	strb	r3, [fp, #-58]
	ldrb	r3, [r9, #505]	@ zero_extendqisi2
	strb	lr, [fp, #-54]
	strb	ip, [fp, #-53]
	strb	r10, [fp, #-57]
	strb	r8, [fp, #-56]
	strb	r7, [fp, #-55]
	strb	r3, [fp, #-52]
	strb	r6, [fp, #-51]
	strb	r6, [fp, #-50]
	ldr	r3, [r4, #52]
	blx	r3
	mov	r1, r6
	ldr	r3, [r4, #48]
	mov	r2, #16
	sub	r0, fp, #60
	blx	r3
	ldrb	lr, [r9, #1]	@ zero_extendqisi2
	ldrb	ip, [r9, #3]	@ zero_extendqisi2
	add	r0, r9, #2976
	ldrb	r3, [r9, #491]	@ zero_extendqisi2
	sub	r1, fp, #60
	ldrb	r10, [r9, #492]	@ zero_extendqisi2
	mov	r2, #16
	ldrb	r8, [r9, #493]	@ zero_extendqisi2
	add	r0, r0, #2
	ldrb	r7, [r9, #494]	@ zero_extendqisi2
	strb	lr, [fp, #-60]
	strb	ip, [fp, #-59]
	ldrb	lr, [r9, #495]	@ zero_extendqisi2
	ldrb	ip, [r9, #496]	@ zero_extendqisi2
	strb	r3, [fp, #-58]
	ldrb	r3, [r9, #497]	@ zero_extendqisi2
	strb	lr, [fp, #-54]
	strb	ip, [fp, #-53]
	strb	r10, [fp, #-57]
	strb	r8, [fp, #-56]
	strb	r7, [fp, #-55]
	strb	r3, [fp, #-52]
	ldr	r3, [r4, #52]
	blx	r3
	mov	r1, r6
	ldr	r3, [r4, #48]
	mov	r2, #16
	sub	r0, fp, #60
	blx	r3
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	ldrb	lr, [r9, #508]	@ zero_extendqisi2
	add	r0, r9, #2992
	ldrb	r2, [r9, #509]	@ zero_extendqisi2
	sub	r1, fp, #60
	ldrb	r8, [r9, #506]	@ zero_extendqisi2
	add	r0, r0, #2
	ldrb	r7, [r9, #507]	@ zero_extendqisi2
	str	r2, [fp, #-68]
	mov	r2, #16
	ldrb	r10, [r9, #510]	@ zero_extendqisi2
	ldrb	ip, [r9, #511]	@ zero_extendqisi2
	strb	r6, [fp, #-51]
	strb	r6, [fp, #-50]
	strb	r3, [fp, #-60]
	ldrb	r6, [r9, #512]	@ zero_extendqisi2
	ldrb	r3, [r9, #513]	@ zero_extendqisi2
	strb	lr, [fp, #-57]
	ldrb	lr, [fp, #-68]	@ zero_extendqisi2
	strb	r3, [fp, #-52]
	strb	r8, [fp, #-59]
	ldr	r3, [r4, #52]
	strb	r7, [fp, #-58]
	strb	lr, [fp, #-56]
	strb	r10, [fp, #-55]
	strb	ip, [fp, #-54]
	strb	r6, [fp, #-53]
	blx	r3
	ldr	r3, [fp, #-64]
	ldr	r3, [r3, #68]
	cmp	r3, #0
	bne	.L454
	add	r6, r9, #4096
	add	r7, r9, #1152
	mov	r4, r6
	add	r6, r6, #2
	add	r4, r4, #50
.L455:
	mov	r1, r7
	mov	r0, r6
	ldr	r3, [r5, #52]
	mov	r2, #11
	add	r6, r6, #16
	blx	r3
	cmp	r6, r4
	add	r7, r7, #11
	bne	.L455
	add	r6, r9, #1184
	add	r7, r9, #4160
	add	r6, r6, #1
	add	r7, r7, #34
.L456:
	mov	r1, r6
	mov	r0, r4
	ldr	r3, [r5, #52]
	mov	r2, #11
	add	r4, r4, #16
	blx	r3
	cmp	r4, r7
	add	r6, r6, #11
	ldr	r10, .L474
	bne	.L456
	add	r0, r9, #4736
	ldr	r3, [r10, #52]
	mov	r2, #14
	add	r1, r9, #536
	add	r0, r0, #34
	add	r8, r9, #4480
	blx	r3
	add	ip, r9, #5056
	add	r1, r9, #548
	add	r0, r9, #4736
	ldr	r3, [r10, #52]
	add	r1, r1, #2
	add	r0, r0, #50
	mov	r2, #14
	mov	r4, ip
	blx	r3
	add	r3, r4, #2
	str	r3, [fp, #-64]
	add	r3, r9, #564
	add	r8, r8, #2
	str	r9, [fp, #-68]
	mov	r10, r3
.L463:
	sub	r4, r8, #288
	mov	r7, r10
.L466:
	add	r9, r4, #48
	mov	r6, r7
.L464:
	mov	r1, r6
	mov	r0, r4
	ldr	r3, [r5, #52]
	mov	r2, #11
	add	r4, r4, #16
	blx	r3
	cmp	r9, r4
	add	r6, r6, #66
	bne	.L464
	cmp	r9, r8
	add	r7, r7, #11
	mov	r4, r9
	bne	.L466
	ldr	r3, [fp, #-64]
	add	r8, r8, #288
	add	r10, r10, #198
	cmp	r8, r3
	bne	.L463
	ldr	r9, [fp, #-68]
.L462:
	add	r0, r9, #3008
	ldr	r3, [r5, #52]
	add	r1, r9, #960
	mov	r2, #64
	add	r0, r0, #2
	blx	r3
	add	r0, r9, #3072
	ldr	r3, [r5, #52]
	add	r1, r9, #1024
	mov	r2, #64
	add	r0, r0, #2
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L454:
	add	r1, r9, #9472
	add	r0, r9, #4800
	ldr	r3, [r4, #52]
	add	r1, r1, #28
	mov	r2, #128
	add	r0, r0, #2
	blx	r3
	add	ip, r9, #9600
	ldr	r3, [r4, #52]
	add	r1, r9, #11904
	mov	r4, ip
	add	r0, r9, #6464
	add	ip, r9, #7232
	mov	r2, #128
	add	r1, r1, #28
	add	r0, r0, #2
	mov	r6, ip
	add	r8, r9, #5696
	blx	r3
	add	r3, r4, #28
	add	r8, r8, #2
	add	r2, r6, #2
	mov	r10, r3
	str	r2, [fp, #-64]
	str	r9, [fp, #-68]
.L457:
	sub	r4, r8, #768
	mov	r7, r10
.L461:
	add	r6, r4, #256
	mov	r9, r7
.L458:
	ldr	r3, [r5, #52]
	mov	r2, #64
	mov	r1, r9
	mov	r0, r4
	add	r4, r4, r2
	blx	r3
	cmp	r6, r4
	add	r9, r9, #64
	ldr	r3, .L474
	bne	.L458
	cmp	r6, r8
	add	r7, r7, #384
	mov	r4, r6
	bne	.L461
	ldr	r2, [fp, #-64]
	add	r8, r6, #768
	add	r10, r10, #1152
	cmp	r8, r2
	bne	.L457
	ldr	r9, [fp, #-68]
	mov	r2, #960
	ldr	r3, [r3, #52]
	add	r1, r9, #1216
	add	r0, r9, #3136
	add	r1, r1, #2
	add	r0, r0, #2
	blx	r3
	b	.L462
.L475:
	.align	2
.L474:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VP6_CfgTable, .-VP6_CfgTable
	.align	2
	.global	VP6_BuildTables
	.type	VP6_BuildTables, %function
VP6_BuildTables:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r2, #0
	cmpne	r1, #0
	mov	r4, r1
	mov	r6, r2
	mov	r5, r0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L481
	ldr	r3, [r1, #20]
	cmp	r3, #0
	bne	.L485
	ldr	r7, .L487
	mov	r2, #80
	ldr	r1, .L487+4
	add	r0, r0, #404
	ldr	r3, [r7, #52]
	blx	r3
	mov	r2, #2
	ldr	r3, [r7, #52]
	add	r0, r5, r2
	ldr	r1, .L487+8
	blx	r3
	ldr	r3, [r7, #52]
	mov	r2, #14
	ldr	r1, .L487+12
	add	r0, r5, #484
	blx	r3
	ldr	r3, [r7, #52]
	mov	r2, #2
	ldr	r1, .L487+16
	mov	r0, r5
	blx	r3
	add	r0, r5, #496
	ldr	r3, [r7, #52]
	mov	r2, #16
	ldr	r1, .L487+20
	add	r0, r0, #2
	blx	r3
	ldr	r1, .L487+24
	ldr	r3, [r7, #52]
	mov	r2, #64
	add	r0, r5, #1088
	blx	r3
	mov	r1, r4
	mov	r0, r5
	bl	VP6_BuildScanOrder
.L479:
	ldr	r3, [r4, #20]
	mov	r1, r6
	mov	r2, r4
	mov	r0, r5
	bl	VP6_ConfigureEntropyDecoder
	ldr	r3, [r4, #68]
	cmp	r3, #0
	bne	.L486
.L480:
	mov	r1, r4
	mov	r0, r5
	bl	VP6_CfgTable
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L485:
	mov	r2, r1
	mov	r1, r6
	bl	VP6_DecodeModeProbs
	mov	r2, r4
	mov	r1, r6
	mov	r0, r5
	bl	VP6_ConfigureMvEntropyDecoder
	b	.L479
.L486:
	mov	r0, r5
	bl	VP6_ConvertBoolToHuff
	b	.L480
.L481:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L488:
	.align	2
.L487:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+1996
	.word	.LANCHOR0+2076
	.word	.LANCHOR0+2080
	.word	.LANCHOR0+2096
	.word	.LANCHOR0+2100
	.word	.LANCHOR0+2116
	UNWIND(.fnend)
	.size	VP6_BuildTables, .-VP6_BuildTables
	.align	2
	.global	IsFrameSizeChanged
	.type	IsFrameSizeChanged, %function
IsFrameSizeChanged:
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
	add	r0, r0, #8192
	ldr	r2, [r0, #3940]
	ldr	r3, [r0, #4084]
	cmp	r2, r3
	beq	.L492
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L492:
	ldr	r3, [r0, #3944]
	ldr	r0, [r0, #4088]
	subs	r0, r3, r0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	IsFrameSizeChanged, .-IsFrameSizeChanged
	.align	2
	.global	VP6DEC_Decode
	.type	VP6DEC_Decode, %function
VP6DEC_Decode:
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
	ldr	r3, [r1]
	mov	r4, r0
	str	r3, [r0, #208]
	ldr	r2, [r1, #8]
	str	r2, [r0, #212]
	ldr	r2, [r1, #12]
	cmp	r2, #0
	str	r2, [r0, #216]
	bne	.L494
	add	r5, r0, #12288
	mov	r3, #3
	strb	r3, [r5, #12]
.L495:
	mov	r0, r4
	bl	VP6_DecodeDecide
	cmp	r0, #1
	mov	r7, r0
	beq	.L510
	ldr	r3, .L531
	mov	r0, #1
	ldr	r1, .L531+4
	mov	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
.L497:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L494:
	cmp	r2, #2
	ble	.L526
	add	r5, r0, #12608
	add	r6, r0, #220
	add	r5, r5, #8
	mov	r1, r3
	mov	r0, r5
	bl	BsInit
	add	r3, r4, #208
	mov	r1, r5
	mov	r0, r6
	str	r3, [r4, #204]
	bl	VP6_GetPictureHeader
	cmp	r0, #1
	mov	r7, r0
	beq	.L498
	ldr	r3, .L531
	mov	r0, #1
	ldr	r1, .L531+8
	mov	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L510:
	mov	r0, r4
	bl	VP6_GetImageBuffer
	cmp	r0, #1
	mov	r6, r0
	beq	.L511
	ldr	r3, .L531
	mov	r0, r7
	ldr	r1, .L531+12
	mov	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L526:
	ldr	r3, .L531
	mov	r0, #1
	ldr	r1, .L531+16
	mov	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L498:
	add	r1, r4, #12416
	mov	r2, r5
	mov	r0, r6
	add	r1, r1, #56
	bl	VP6_BuildTables
	cmp	r0, #1
	bne	.L527
	add	r5, r4, #12288
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L500
	ldr	r2, [r5, #64]
	ldr	r3, [r5, #208]
	cmp	r2, r3
	bne	.L500
	ldr	r2, [r5, #68]
	ldr	r3, [r5, #212]
	cmp	r2, r3
	beq	.L495
.L500:
	ldr	r3, .L531+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L502
	ldr	r0, [r5, #208]
	mov	r3, #8
	ldr	r7, [r5, #68]
	sub	r2, fp, #52
	ldr	r6, [r5, #64]
	mov	r1, #2
	ldr	lr, [r5, #212]
	strh	r0, [fp, #-46]	@ movhi
	strh	r7, [fp, #-52]	@ movhi
	strh	r6, [fp, #-50]	@ movhi
	strh	lr, [fp, #-48]	@ movhi
	ldr	r0, [r5, #712]
	blx	ip
.L502:
	mov	r2, #1
	ldr	r1, [r5, #24]
	ldr	r0, [r5, #712]
	mov	r7, #0
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r5, #28]
	add	r6, r4, #12608
	ldr	r0, [r5, #712]
	add	r6, r6, #40
	bl	FSP_ClearLogicFs
	str	r7, [r5, #24]
	str	r7, [r5, #28]
	ldr	r0, [r5, #712]
	bl	FSP_GetNullLogicFs
	str	r7, [r5, #4]
	mov	r1, r6
	str	r0, [r5, #28]
	str	r0, [r5, #24]
	ldr	r0, [r5, #712]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [r5, #212]
	ldr	r3, [r5, #208]
	cmp	r2, #512
	bhi	.L504
	cmp	r3, #512
	bhi	.L504
	ldr	r3, [r4]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L506
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L528
.L506:
	ldr	r0, [r5, #712]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L525
.L508:
	mov	r0, r4
	bl	VP6_ArrangeVHBMem
	cmp	r0, #1
	beq	.L495
	ldr	r3, .L531
	mov	r0, #1
	ldr	r1, .L531+24
	mov	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L497
.L511:
	mov	r2, r0
	ldr	r1, [r5, #20]
	ldr	r0, [r5, #712]
	bl	FSP_SetRef
	ldr	r1, [r5, #20]
	ldr	r0, [r5, #712]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L529
	ldr	lr, [r4]
	mov	r10, #0
	mvn	r2, #0
	mvn	r3, #0
	ldr	r7, .L531
	mov	r0, #29
	ldrd	r8, [lr, #48]
	ldr	r1, .L531+28
	strd	r8, [ip, #8]
	ldr	lr, [r4]
	ldrd	r8, [lr, #64]
	strd	r8, [ip, #16]
	ldr	lr, [r4]
	ldrd	r8, [lr, #80]
	strd	r8, [ip, #24]
	ldr	lr, [r4]
	ldr	lr, [lr, #96]
	str	lr, [ip, #68]
	ldr	lr, [r4]
	ldr	lr, [lr, #100]
	str	lr, [ip, #72]
	ldr	lr, [r4]
	ldr	lr, [lr, #104]
	str	lr, [ip, #76]
	ldr	lr, [r4]
	ldr	lr, [lr, #120]
	str	r10, [ip, #148]
	str	lr, [ip, #80]
	ldr	lr, [r4]
	strd	r2, [lr, #48]
	ldrd	r2, [ip, #16]
	mov	r9, r3
	mov	r8, r2
	ldrd	r2, [ip, #8]
	ldr	ip, [r7, #68]
	strd	r8, [sp]
	blx	ip
	mov	r0, r4
	bl	VP6_Set_DecParam
	cmp	r0, #1
	mov	r9, r0
	beq	.L513
	ldr	r3, [r7, #68]
	mov	r0, r6
	ldr	r1, .L531+32
	mov	r9, r10
	blx	r3
	b	.L497
.L513:
	ldrb	r3, [r5, #12]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L530
	ldr	r8, [r5, #52]
	add	r1, r4, #12416
	ldr	r6, [r5, #56]
	add	r0, r4, #12288
	add	r1, r1, #56
	add	r0, r0, #40
	ldr	r3, [r7, #52]
	mov	r2, #144
	blx	r3
	str	r8, [r5, #52]
	str	r6, [r5, #56]
.L515:
	ldr	r3, [r7, #68]
	mov	r0, #22
	ldr	r1, .L531+36
	blx	r3
	b	.L497
.L504:
	ldr	ip, .L531
	mov	r0, #18
	ldr	r1, .L531+40
	mov	r9, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L497
.L527:
	ldr	r3, .L531
	mov	r0, r7
	ldr	r1, .L531+44
	mov	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L497
.L530:
	add	r1, r4, #12416
	add	r0, r4, #12288
	add	r1, r1, #56
	ldr	r3, [r7, #52]
	mov	r2, #144
	add	r0, r0, #40
	blx	r3
	b	.L515
.L529:
	ldr	r3, .L531
	movw	r2, #2739
	ldr	r1, .L531+48
	mov	r0, #22
	mov	r9, ip
	ldr	r3, [r3, #68]
	blx	r3
	b	.L497
.L528:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne	.L508
.L525:
	mov	r0, r6
	bl	ResetVoQueue
	b	.L508
.L532:
	.align	2
.L531:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC36
	.word	.LC32
	.word	.LC37
	.word	.LC31
	.word	g_event_report
	.word	.LC35
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC34
	.word	.LC33
	.word	.LC5
	UNWIND(.fnend)
	.size	VP6DEC_Decode, .-VP6DEC_Decode
	.align	2
	.global	VP6_SetImgFormat
	.type	VP6_SetImgFormat, %function
VP6_SetImgFormat:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #12288
	mov	r6, r0
	ldr	r1, [r5, #20]
	ldr	r0, [r5, #712]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldrb	r3, [r5, #12]	@ zero_extendqisi2
	mov	r2, #0
	mov	r1, r2
	strb	r2, [r4, #372]
	and	r3, r3, #3
	mov	ip, #10
	orr	r2, r3, #7168
	str	r3, [r4, #152]
	orr	r2, r2, #32
	str	r3, [r4, #156]
	str	ip, [r4, #384]
	mov	r3, #8
	mov	ip, #1
	str	r2, [r4, #160]
	str	r3, [r4, #388]
	str	r3, [r4, #396]
	str	r3, [r4, #392]
	strb	ip, [r4, #379]
	strb	ip, [r4, #377]
	bl	SetAspectRatio
	ldr	r2, [r5, #212]
	mov	r2, r2, asl #4
	str	r2, [r4, #164]
	ldr	r3, [r5, #208]
	mov	r3, r3, asl #4
	str	r3, [r4, #168]
	ldr	r1, [r6]
	ldr	r1, [r1, #28]
	cmp	r1, #25
	beq	.L539
.L535:
	str	r2, [r4, #172]
	mov	r2, r2, lsr #1
	str	r3, [r4, #176]
	mov	r3, r3, lsr #1
	str	r2, [r4, #180]
	str	r3, [r4, #184]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L539:
	ldr	r2, [r4, #192]
	mov	r3, r3, lsr #4
	ldr	r0, [r4, #104]
	ldr	r1, [r4, #32]
	mov	lr, r2, asl #4
	add	ip, r0, r2
	str	r0, [r4, #88]
	mul	r3, r3, lr
	str	r0, [r4, #120]
	str	r1, [r4, #48]
	str	ip, [r4, #96]
	add	r0, r3, r0
	add	r3, r1, r3
	add	r2, r0, r2
	str	r3, [r4, #36]
	str	r2, [r4, #100]
	str	r0, [r4, #108]
	str	r0, [r4, #92]
	bl	MEM_Phy2Vir
	ldr	r1, [r4, #92]
	ldr	r2, [r4, #164]
	ldr	r3, [r4, #168]
	str	r1, [r4, #124]
	str	r0, [r4, #52]
	b	.L535
	UNWIND(.fnend)
	.size	VP6_SetImgFormat, .-VP6_SetImgFormat
	.align	2
	.global	VP6DEC_VDMPostProc
	.type	VP6DEC_VDMPostProc, %function
VP6DEC_VDMPostProc:
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
	ldr	r7, .L576
	mov	r2, r1
	mov	r6, r0
	mov	r5, r1
	add	r4, r6, #12288
	ldr	r3, [r7, #68]
	mov	r0, #22
	ldr	r1, .L576+4
	blx	r3
	mov	r1, r6
	mov	r0, #10
	bl	ReleasePacket
	ldr	r1, [r4, #20]
	ldr	r0, [r4, #712]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L569
	cmp	r5, #0
	blt	.L543
	cmp	r5, #100
	movlt	r2, r5
	movge	r2, #100
	str	r2, [r3, #200]
	ldr	r3, [r6]
	ldr	r3, [r3]
	cmp	r2, r3
	bhi	.L570
.L544:
	mov	r0, r6
	bl	VP6_SetImgFormat
	ldr	r3, [r4]
	mov	r2, #1
	str	r2, [r4, #4]
	cmp	r3, #2
	beq	.L571
.L545:
	ldr	r1, [r4, #20]
	str	r1, [r4, #32]
.L546:
	cmn	r1, #1
	beq	.L547
	mov	r2, #1
	ldr	r0, [r4, #712]
	bl	FSP_SetDisplay
	ldr	r1, [r4, #32]
	ldr	r0, [r4, #712]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L572
	add	r3, r6, #12608
	ldr	r0, [r4, #712]
	add	r3, r3, #40
	str	r8, [sp]
	mov	r2, r6
	mov	r1, #10
	bl	InsertImgToVoQueue
	cmp	r0, #1
	mov	r5, r0
	bne	.L573
.L547:
	ldr	r3, [r4, #28]
	ldr	r2, [r4, #24]
	cmp	r3, r2
	beq	.L549
	ldr	r1, [r4, #20]
	cmp	r3, r1
	beq	.L550
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r4, #712]
	bl	FSP_SetRef
	ldr	r1, [r4, #20]
.L550:
	str	r1, [r4, #28]
	mov	r2, #1
	ldr	r0, [r4, #712]
	bl	FSP_SetRef
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L574
	ldr	r3, [r4, #24]
	ldr	r1, [r4, #20]
	cmp	r3, r1
	beq	.L553
.L575:
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r4, #712]
	bl	FSP_SetRef
	ldr	r1, [r4, #20]
.L553:
	ldr	r0, [r4, #712]
	mov	r2, #1
	str	r1, [r4, #24]
	bl	FSP_SetRef
	mov	r0, #1
.L567:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L574:
	ldr	r3, [r4, #16]
	cmp	r3, #0
	moveq	r0, #1
	beq	.L567
	ldr	r3, [r4, #24]
	ldr	r1, [r4, #20]
	cmp	r3, r1
	bne	.L575
	b	.L553
.L543:
	mov	r2, #0
	str	r2, [r3, #200]
	b	.L544
.L571:
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L545
	mvn	r3, #0
	ldr	r1, [r4, #20]
	mov	r2, #0
	str	r3, [r4, #32]
	ldr	r0, [r4, #712]
	bl	FSP_SetDisplay
	ldr	r1, [r4, #32]
	b	.L546
.L573:
	ldr	r1, [r8, #212]
	mov	r6, #0
	ldr	r0, [r4, #712]
	bl	FreeUsdByDec
	str	r6, [r8, #212]
	ldr	r1, [r8, #216]
	ldr	r0, [r4, #712]
	bl	FreeUsdByDec
	str	r6, [r8, #216]
	mov	r2, r6
	ldr	r1, [r4, #32]
	ldr	r0, [r4, #712]
	bl	FSP_SetDisplay
	mov	r0, r6
	ldr	r3, [r7, #68]
	ldr	r1, .L576+8
	blx	r3
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L549:
	mov	r1, r3
	ldr	r0, [r4, #712]
	bl	FSP_GetRef
	subs	r2, r0, #0
	bne	.L568
	ldr	r3, [r4, #28]
	ldr	r1, [r4, #20]
	cmp	r3, r1
	beq	.L550
	mov	r1, r3
	ldr	r0, [r4, #712]
	bl	FSP_SetRef
.L568:
	ldr	r1, [r4, #20]
	b	.L550
.L570:
	ldr	ip, [r7, #68]
	mov	r0, #1
	ldr	r1, .L576+12
	mov	r5, #0
	blx	ip
	mov	r2, #1
	ldr	r1, [r4, #24]
	ldr	r0, [r4, #712]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #712]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #24]
	str	r5, [r4, #28]
	ldr	r0, [r4, #712]
	bl	FSP_GetNullLogicFs
	str	r5, [r4, #4]
	mov	r3, r0
	mvn	r0, #0
	str	r3, [r4, #28]
	str	r3, [r4, #24]
	b	.L567
.L572:
	ldr	r3, [r7, #68]
	mov	r2, r8
	ldr	r1, .L576+16
	mov	r0, #22
	blx	r3
	mvn	r0, #0
	b	.L567
.L569:
	ldr	r3, [r7, #68]
	ldr	r1, .L576+20
	blx	r3
	mvn	r0, #0
	b	.L567
.L577:
	.align	2
.L576:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC41
	.word	.LC45
	.word	.LC43
	.word	.LC44
	.word	.LC42
	UNWIND(.fnend)
	.size	VP6DEC_VDMPostProc, .-VP6DEC_VDMPostProc
	.align	2
	.global	VP6DEC_RecycleImage
	.type	VP6DEC_RecycleImage, %function
VP6DEC_RecycleImage:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r5, r1
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r4, r0, #12288
	mov	r2, #0
	ldr	r0, [r4, #712]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #712]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L581
	ldr	r1, [r5, #212]
	mov	r6, #0
	ldr	r0, [r4, #712]
	bl	FreeUsdByDec
	str	r6, [r5, #212]
	ldr	r1, [r5, #216]
	ldr	r0, [r4, #712]
	bl	FreeUsdByDec
	str	r6, [r5, #216]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L581:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP6DEC_RecycleImage, .-VP6DEC_RecycleImage
	.align	2
	.global	VP6DEC_GetRemainImg
	.type	VP6DEC_GetRemainImg, %function
VP6DEC_GetRemainImg:
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
	add	r0, r0, #12608
	add	r0, r0, #40
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	GetVoLastImageID
	UNWIND(.fnend)
	.size	VP6DEC_GetRemainImg, .-VP6DEC_GetRemainImg
	.global	DefaultScanBands
	.global	ZrlUpdateProbs
	.global	ScanBandUpdateProbs
	.global	ZeroRunProbDefaults
	.global	VP6_AcUpdateProbs
	.global	VP6_DcUpdateProbs
	.global	DefaultSignProbs
	.global	DefaultIsShortProbs
	.global	DefaultMvLongProbs
	.global	DefaultMvShortProbs
	.global	VP6_MvUpdateProbs
	.global	VP6_BaselineXmittedProbs
	.global	VP6_ModeVq
	.global	VP6_DcNodeEqs
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	VP6_DcNodeEqs, %object
	.size	VP6_DcNodeEqs, 120
VP6_DcNodeEqs:
	.word	122
	.word	133
	.word	133
	.word	51
	.word	142
	.word	-16
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	1
	.word	78
	.word	171
	.word	169
	.word	71
	.word	221
	.word	-30
	.word	139
	.word	117
	.word	214
	.word	44
	.word	246
	.word	-3
	.word	168
	.word	79
	.word	210
	.word	38
	.word	203
	.word	17
	.type	VP6_DcUpdateProbs, %object
	.size	VP6_DcUpdateProbs, 22
VP6_DcUpdateProbs:
	.byte	-110
	.byte	-1
	.byte	-75
	.byte	-49
	.byte	-24
	.byte	-13
	.byte	-18
	.byte	-5
	.byte	-12
	.byte	-6
	.byte	-7
	.byte	-77
	.byte	-1
	.byte	-42
	.byte	-16
	.byte	-6
	.byte	-1
	.byte	-12
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.space	2
	.type	ZeroRunProbDefaults, %object
	.size	ZeroRunProbDefaults, 28
ZeroRunProbDefaults:
	.byte	-58
	.byte	-59
	.byte	-60
	.byte	-110
	.byte	-58
	.byte	-52
	.byte	-87
	.byte	-114
	.byte	-126
	.byte	-120
	.byte	-107
	.byte	-107
	.byte	-65
	.byte	-7
	.byte	-121
	.byte	-55
	.byte	-75
	.byte	-102
	.byte	98
	.byte	117
	.byte	-124
	.byte	126
	.byte	-110
	.byte	-87
	.byte	-72
	.byte	-16
	.byte	-10
	.byte	-2
	.type	ZrlUpdateProbs, %object
	.size	ZrlUpdateProbs, 28
ZrlUpdateProbs:
	.byte	-37
	.byte	-10
	.byte	-18
	.byte	-7
	.byte	-24
	.byte	-17
	.byte	-7
	.byte	-1
	.byte	-8
	.byte	-3
	.byte	-17
	.byte	-12
	.byte	-15
	.byte	-8
	.byte	-58
	.byte	-24
	.byte	-5
	.byte	-3
	.byte	-37
	.byte	-15
	.byte	-3
	.byte	-1
	.byte	-8
	.byte	-7
	.byte	-12
	.byte	-18
	.byte	-5
	.byte	-1
	.type	VP6_AcUpdateProbs, %object
	.size	VP6_AcUpdateProbs, 396
VP6_AcUpdateProbs:
	.byte	-29
	.byte	-10
	.byte	-26
	.byte	-9
	.byte	-12
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-47
	.byte	-25
	.byte	-25
	.byte	-7
	.byte	-7
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-31
	.byte	-14
	.byte	-15
	.byte	-5
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-15
	.byte	-3
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-16
	.byte	-1
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-16
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-50
	.byte	-53
	.byte	-29
	.byte	-17
	.byte	-9
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-49
	.byte	-57
	.byte	-36
	.byte	-20
	.byte	-13
	.byte	-4
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-44
	.byte	-37
	.byte	-26
	.byte	-13
	.byte	-12
	.byte	-3
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-20
	.byte	-19
	.byte	-9
	.byte	-4
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-16
	.byte	-16
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-26
	.byte	-23
	.byte	-7
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-18
	.byte	-18
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-5
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-31
	.byte	-17
	.byte	-29
	.byte	-25
	.byte	-12
	.byte	-3
	.byte	-13
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-24
	.byte	-22
	.byte	-32
	.byte	-28
	.byte	-14
	.byte	-7
	.byte	-14
	.byte	-4
	.byte	-5
	.byte	-5
	.byte	-1
	.byte	-21
	.byte	-7
	.byte	-18
	.byte	-16
	.byte	-5
	.byte	-1
	.byte	-7
	.byte	-1
	.byte	-3
	.byte	-3
	.byte	-1
	.byte	-7
	.byte	-3
	.byte	-5
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-6
	.byte	-7
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-13
	.byte	-12
	.byte	-6
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-8
	.byte	-6
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.type	ScanBandUpdateProbs, %object
	.size	ScanBandUpdateProbs, 64
ScanBandUpdateProbs:
	.byte	-1
	.byte	-124
	.byte	-124
	.byte	-97
	.byte	-103
	.byte	-105
	.byte	-95
	.byte	-86
	.byte	-92
	.byte	-94
	.byte	-120
	.byte	110
	.byte	103
	.byte	114
	.byte	-127
	.byte	118
	.byte	124
	.byte	125
	.byte	-124
	.byte	-120
	.byte	114
	.byte	110
	.byte	-114
	.byte	-121
	.byte	-122
	.byte	123
	.byte	-113
	.byte	126
	.byte	-103
	.byte	-73
	.byte	-90
	.byte	-95
	.byte	-85
	.byte	-76
	.byte	-77
	.byte	-92
	.byte	-53
	.byte	-38
	.byte	-31
	.byte	-39
	.byte	-41
	.byte	-50
	.byte	-53
	.byte	-39
	.byte	-27
	.byte	-15
	.byte	-8
	.byte	-13
	.byte	-3
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.type	VP6_ModeVq, %object
	.size	VP6_ModeVq, 960
VP6_ModeVq:
	.byte	9
	.byte	15
	.byte	32
	.byte	25
	.byte	7
	.byte	19
	.byte	9
	.byte	21
	.byte	1
	.byte	12
	.byte	14
	.byte	12
	.byte	3
	.byte	18
	.byte	14
	.byte	23
	.byte	3
	.byte	10
	.byte	0
	.byte	4
	.byte	48
	.byte	39
	.byte	1
	.byte	2
	.byte	11
	.byte	27
	.byte	29
	.byte	44
	.byte	7
	.byte	27
	.byte	1
	.byte	4
	.byte	0
	.byte	3
	.byte	1
	.byte	6
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	21
	.byte	32
	.byte	1
	.byte	2
	.byte	4
	.byte	10
	.byte	32
	.byte	43
	.byte	6
	.byte	23
	.byte	2
	.byte	3
	.byte	1
	.byte	19
	.byte	1
	.byte	6
	.byte	12
	.byte	21
	.byte	0
	.byte	7
	.byte	69
	.byte	83
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	10
	.byte	29
	.byte	3
	.byte	12
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	0
	.byte	3
	.byte	2
	.byte	2
	.byte	0
	.byte	0
	.byte	11
	.byte	20
	.byte	1
	.byte	4
	.byte	18
	.byte	36
	.byte	43
	.byte	48
	.byte	13
	.byte	35
	.byte	0
	.byte	2
	.byte	0
	.byte	5
	.byte	3
	.byte	12
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	70
	.byte	44
	.byte	0
	.byte	1
	.byte	2
	.byte	10
	.byte	37
	.byte	46
	.byte	8
	.byte	26
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	8
	.byte	15
	.byte	0
	.byte	1
	.byte	8
	.byte	21
	.byte	74
	.byte	53
	.byte	22
	.byte	42
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	3
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	-115
	.byte	42
	.byte	0
	.byte	0
	.byte	1
	.byte	4
	.byte	11
	.byte	24
	.byte	1
	.byte	11
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	8
	.byte	19
	.byte	4
	.byte	10
	.byte	24
	.byte	45
	.byte	21
	.byte	37
	.byte	9
	.byte	29
	.byte	0
	.byte	3
	.byte	1
	.byte	7
	.byte	11
	.byte	25
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	46
	.byte	42
	.byte	0
	.byte	1
	.byte	2
	.byte	10
	.byte	54
	.byte	51
	.byte	10
	.byte	30
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	28
	.byte	32
	.byte	0
	.byte	0
	.byte	3
	.byte	10
	.byte	75
	.byte	51
	.byte	14
	.byte	33
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	100
	.byte	46
	.byte	0
	.byte	1
	.byte	3
	.byte	9
	.byte	21
	.byte	37
	.byte	5
	.byte	20
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	1
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	27
	.byte	29
	.byte	0
	.byte	1
	.byte	9
	.byte	25
	.byte	53
	.byte	51
	.byte	12
	.byte	34
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	1
	.byte	5
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	80
	.byte	38
	.byte	0
	.byte	0
	.byte	1
	.byte	4
	.byte	69
	.byte	33
	.byte	5
	.byte	16
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	16
	.byte	20
	.byte	0
	.byte	0
	.byte	2
	.byte	8
	.byte	104
	.byte	49
	.byte	15
	.byte	33
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-62
	.byte	16
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	9
	.byte	1
	.byte	3
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	41
	.byte	22
	.byte	1
	.byte	0
	.byte	1
	.byte	31
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	7
	.byte	0
	.byte	1
	.byte	98
	.byte	25
	.byte	4
	.byte	10
	.byte	123
	.byte	37
	.byte	6
	.byte	4
	.byte	1
	.byte	27
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	5
	.byte	8
	.byte	1
	.byte	7
	.byte	0
	.byte	1
	.byte	12
	.byte	10
	.byte	0
	.byte	2
	.byte	26
	.byte	14
	.byte	14
	.byte	12
	.byte	0
	.byte	24
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	55
	.byte	17
	.byte	1
	.byte	9
	.byte	0
	.byte	36
	.byte	5
	.byte	7
	.byte	1
	.byte	3
	.byte	-47
	.byte	5
	.byte	0
	.byte	0
	.byte	0
	.byte	27
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	5
	.byte	4
	.byte	5
	.byte	0
	.byte	121
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	2
	.byte	4
	.byte	1
	.byte	4
	.byte	2
	.byte	2
	.byte	0
	.byte	1
	.byte	-81
	.byte	5
	.byte	0
	.byte	1
	.byte	0
	.byte	48
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	83
	.byte	5
	.byte	2
	.byte	3
	.byte	0
	.byte	102
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	3
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	-23
	.byte	6
	.byte	0
	.byte	0
	.byte	0
	.byte	8
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	34
	.byte	16
	.byte	112
	.byte	21
	.byte	1
	.byte	28
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	6
	.byte	8
	.byte	1
	.byte	7
	.byte	0
	.byte	3
	.byte	2
	.byte	5
	.byte	0
	.byte	2
	.byte	-97
	.byte	35
	.byte	2
	.byte	2
	.byte	0
	.byte	25
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	6
	.byte	0
	.byte	5
	.byte	0
	.byte	1
	.byte	4
	.byte	4
	.byte	0
	.byte	1
	.byte	75
	.byte	39
	.byte	5
	.byte	7
	.byte	2
	.byte	48
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	11
	.byte	2
	.byte	16
	.byte	1
	.byte	4
	.byte	7
	.byte	10
	.byte	0
	.byte	2
	.byte	-44
	.byte	21
	.byte	0
	.byte	1
	.byte	0
	.byte	9
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	2
	.byte	2
	.byte	0
	.byte	0
	.byte	4
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	-84
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	-69
	.byte	22
	.byte	1
	.byte	1
	.byte	0
	.byte	17
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	6
	.byte	0
	.byte	4
	.byte	0
	.byte	1
	.byte	4
	.byte	4
	.byte	0
	.byte	1
	.byte	-123
	.byte	6
	.byte	1
	.byte	2
	.byte	1
	.byte	70
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	4
	.byte	0
	.byte	3
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-5
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	3
	.byte	2
	.byte	3
	.byte	0
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	11
	.byte	4
	.byte	1
	.byte	4
	.byte	0
	.byte	2
	.byte	3
	.byte	2
	.byte	0
	.byte	4
	.byte	49
	.byte	46
	.byte	3
	.byte	4
	.byte	7
	.byte	31
	.byte	42
	.byte	41
	.byte	0
	.byte	0
	.byte	2
	.byte	6
	.byte	1
	.byte	7
	.byte	1
	.byte	4
	.byte	2
	.byte	4
	.byte	0
	.byte	1
	.byte	26
	.byte	25
	.byte	1
	.byte	1
	.byte	2
	.byte	10
	.byte	67
	.byte	39
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	0
	.byte	14
	.byte	0
	.byte	2
	.byte	31
	.byte	26
	.byte	1
	.byte	6
	.byte	103
	.byte	46
	.byte	1
	.byte	2
	.byte	2
	.byte	10
	.byte	33
	.byte	42
	.byte	0
	.byte	0
	.byte	1
	.byte	4
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	1
	.byte	3
	.byte	0
	.byte	0
	.byte	14
	.byte	31
	.byte	9
	.byte	13
	.byte	14
	.byte	54
	.byte	22
	.byte	29
	.byte	0
	.byte	0
	.byte	2
	.byte	6
	.byte	4
	.byte	18
	.byte	6
	.byte	13
	.byte	1
	.byte	5
	.byte	0
	.byte	1
	.byte	85
	.byte	39
	.byte	0
	.byte	0
	.byte	1
	.byte	9
	.byte	69
	.byte	40
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	0
	.byte	0
	.byte	31
	.byte	28
	.byte	0
	.byte	0
	.byte	3
	.byte	14
	.byte	-126
	.byte	34
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	3
	.byte	3
	.byte	0
	.byte	1
	.byte	-85
	.byte	25
	.byte	0
	.byte	0
	.byte	1
	.byte	5
	.byte	25
	.byte	21
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	17
	.byte	21
	.byte	68
	.byte	29
	.byte	6
	.byte	15
	.byte	13
	.byte	22
	.byte	0
	.byte	0
	.byte	6
	.byte	12
	.byte	3
	.byte	14
	.byte	4
	.byte	10
	.byte	1
	.byte	7
	.byte	0
	.byte	3
	.byte	51
	.byte	39
	.byte	0
	.byte	1
	.byte	2
	.byte	12
	.byte	91
	.byte	44
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	3
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	0
	.byte	1
	.byte	81
	.byte	25
	.byte	0
	.byte	0
	.byte	2
	.byte	9
	.byte	106
	.byte	26
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-116
	.byte	37
	.byte	0
	.byte	1
	.byte	1
	.byte	8
	.byte	24
	.byte	33
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	0
	.byte	2
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	14
	.byte	23
	.byte	1
	.byte	3
	.byte	11
	.byte	53
	.byte	90
	.byte	31
	.byte	0
	.byte	0
	.byte	0
	.byte	3
	.byte	1
	.byte	5
	.byte	2
	.byte	6
	.byte	1
	.byte	2
	.byte	0
	.byte	0
	.byte	123
	.byte	29
	.byte	0
	.byte	0
	.byte	1
	.byte	7
	.byte	57
	.byte	30
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	13
	.byte	14
	.byte	0
	.byte	0
	.byte	4
	.byte	20
	.byte	-81
	.byte	20
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	-54
	.byte	23
	.byte	0
	.byte	0
	.byte	1
	.byte	3
	.byte	2
	.byte	9
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.type	VP6_MvUpdateProbs, %object
	.size	VP6_MvUpdateProbs, 34
VP6_MvUpdateProbs:
	.byte	-19
	.byte	-10
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-6
	.byte	-6
	.byte	-4
	.byte	-25
	.byte	-13
	.byte	-11
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-5
	.byte	-5
	.byte	-2
	.space	2
	.type	VP6_LoopFilterLimitValues, %object
	.size	VP6_LoopFilterLimitValues, 64
VP6_LoopFilterLimitValues:
	.byte	14
	.byte	14
	.byte	13
	.byte	13
	.byte	12
	.byte	12
	.byte	10
	.byte	10
	.byte	10
	.byte	10
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.type	DcQuantTab, %object
	.size	DcQuantTab, 128
DcQuantTab:
	.short	188
	.short	188
	.short	188
	.short	188
	.short	180
	.short	172
	.short	172
	.short	172
	.short	172
	.short	172
	.short	168
	.short	164
	.short	164
	.short	160
	.short	160
	.short	160
	.short	160
	.short	140
	.short	140
	.short	140
	.short	140
	.short	132
	.short	132
	.short	132
	.short	132
	.short	128
	.short	128
	.short	128
	.short	108
	.short	108
	.short	104
	.short	104
	.short	100
	.short	100
	.short	96
	.short	96
	.short	92
	.short	92
	.short	76
	.short	76
	.short	76
	.short	76
	.short	72
	.short	72
	.short	68
	.short	64
	.short	64
	.short	64
	.short	64
	.short	64
	.short	60
	.short	44
	.short	44
	.short	44
	.short	40
	.short	40
	.short	36
	.short	32
	.short	28
	.short	20
	.short	12
	.short	12
	.short	8
	.short	8
	.type	AcQuantTab, %object
	.size	AcQuantTab, 128
AcQuantTab:
	.short	376
	.short	368
	.short	360
	.short	352
	.short	344
	.short	328
	.short	312
	.short	296
	.short	280
	.short	264
	.short	248
	.short	232
	.short	216
	.short	212
	.short	208
	.short	204
	.short	200
	.short	196
	.short	192
	.short	188
	.short	184
	.short	180
	.short	176
	.short	172
	.short	168
	.short	160
	.short	156
	.short	148
	.short	144
	.short	140
	.short	136
	.short	132
	.short	128
	.short	124
	.short	120
	.short	116
	.short	112
	.short	108
	.short	104
	.short	100
	.short	96
	.short	92
	.short	88
	.short	84
	.short	80
	.short	76
	.short	72
	.short	68
	.short	64
	.short	60
	.short	56
	.short	52
	.short	48
	.short	44
	.short	40
	.short	36
	.short	32
	.short	28
	.short	24
	.short	20
	.short	16
	.short	12
	.short	8
	.short	4
	.type	__FUNCTION__.13911, %object
	.size	__FUNCTION__.13911, 17
__FUNCTION__.13911:
	.ascii	"VP6_DecodeDecide\000"
	.space	3
	.type	VP6_BaselineXmittedProbs, %object
	.size	VP6_BaselineXmittedProbs, 80
VP6_BaselineXmittedProbs:
	.byte	42
	.byte	2
	.byte	7
	.byte	42
	.byte	22
	.byte	3
	.byte	2
	.byte	5
	.byte	1
	.byte	0
	.byte	69
	.byte	1
	.byte	1
	.byte	44
	.byte	6
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	8
	.byte	1
	.byte	8
	.byte	0
	.byte	0
	.byte	2
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	-27
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	35
	.byte	1
	.byte	6
	.byte	34
	.byte	0
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	122
	.byte	1
	.byte	1
	.byte	46
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	64
	.byte	0
	.byte	64
	.byte	64
	.byte	64
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	64
	.byte	0
	.byte	64
	.byte	64
	.byte	64
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.type	DefaultIsShortProbs, %object
	.size	DefaultIsShortProbs, 2
DefaultIsShortProbs:
	.byte	-94
	.byte	-92
	.space	2
	.type	DefaultMvShortProbs, %object
	.size	DefaultMvShortProbs, 14
DefaultMvShortProbs:
	.byte	-31
	.byte	-110
	.byte	-84
	.byte	-109
	.byte	-42
	.byte	39
	.byte	-100
	.byte	-52
	.byte	-86
	.byte	119
	.byte	-21
	.byte	-116
	.byte	-26
	.byte	-28
	.space	2
	.type	DefaultSignProbs, %object
	.size	DefaultSignProbs, 2
DefaultSignProbs:
	.byte	-128
	.byte	-128
	.space	2
	.type	DefaultMvLongProbs, %object
	.size	DefaultMvLongProbs, 16
DefaultMvLongProbs:
	.byte	-9
	.byte	-46
	.byte	-121
	.byte	68
	.byte	-118
	.byte	-36
	.byte	-17
	.byte	-10
	.byte	-12
	.byte	-72
	.byte	-55
	.byte	44
	.byte	-83
	.byte	-35
	.byte	-17
	.byte	-3
	.type	DefaultScanBands, %object
	.size	DefaultScanBands, 64
DefaultScanBands:
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	3
	.byte	3
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	6
	.byte	6
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	8
	.byte	8
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	10
	.byte	10
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	12
	.byte	12
	.byte	12
	.byte	12
	.byte	12
	.byte	12
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	14
	.byte	14
	.byte	14
	.byte	14
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"image_size_in_mb abnormal(%d x %d)\012\000" )
.LC1:
	ASCII(.ascii	"vp6 actual frame size(%dx%d) exeed max config(%dx%d" )
	ASCII(.ascii	")\012\000" )
	.space	2
.LC2:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC3:
	ASCII(.ascii	"FSP_ConfigInstance fail!\012\000" )
	.space	2
.LC4:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC6:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC7:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC8:
	ASCII(.ascii	"enter VP6_ConvertBoolToHuff\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"leave VP6_ConvertBoolToHuff\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"enter VP6_IntraFrmHeader\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"error: Vp3VersionNo %d > CURRENT_DECODE_VERSION(8)\012" )
	ASCII(.ascii	"\000" )
.LC12:
	ASCII(.ascii	"error: bitstream error!\012\000" )
	.space	3
.LC13:
	ASCII(.ascii	"We have a new size image!\012 The old size is %d x " )
	ASCII(.ascii	"%d, and the new is %d x %d\012\000" )
.LC14:
	ASCII(.ascii	"leave VP6_IntraFrmHeader\012\000" )
	.space	2
.LC15:
	ASCII(.ascii	"VP6_InterFrmHeader VersionNo fault,old:%d,new:%d\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC16:
	ASCII(.ascii	"[ERR]VP6_GetPictureHeader invalid param!\012\000" )
	.space	2
.LC17:
	ASCII(.ascii	"VP6_GetPictureHeader enter!\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"VP6_GetPictureHeader leave!\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC20:
	ASCII(.ascii	"VP6DEC_Init leave\012\000" )
	.space	1
.LC21:
	ASCII(.ascii	"ERROR: No Img buffer is allocated\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"----------------- vp6 Imode -> %d -----------------" )
	ASCII(.ascii	"-\012\000" )
	.space	2
.LC23:
	ASCII(.ascii	"-----I_MODE, but FrameType is %d-----\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"P-frm\000" )
	.space	2
.LC25:
	ASCII(.ascii	"-----cur framtype is %s, but refnum=%d-----\012\000" )
	.space	3
.LC26:
	ASCII(.ascii	"error: DecMode illegal\012\000" )
.LC27:
	ASCII(.ascii	"%s %d NULL == pFwdImg!!\012\000" )
	.space	3
.LC28:
	ASCII(.ascii	"dec P frame, fwd ref err(%d) > ref_thr(%d)\012\000" )
.LC29:
	ASCII(.ascii	"VP6_Set_DecParam but logic fs is null!\012\000" )
.LC30:
	ASCII(.ascii	"Decord FS is NULL!\012\000" )
.LC31:
	ASCII(.ascii	"Error! Truncated packet\012\000" )
	.space	3
.LC32:
	ASCII(.ascii	"[ERR]VP6_GetPictureHeader Error!\012\000" )
	.space	2
.LC33:
	ASCII(.ascii	"[ERR] VP6_DecodeDecide Error!\012\000" )
	.space	1
.LC34:
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC35:
	ASCII(.ascii	"VP6_ArrangeVHBMem err\012\000" )
	.space	1
.LC36:
	ASCII(.ascii	"VP6_DecodeDecide err\012\000" )
	.space	2
.LC37:
	ASCII(.ascii	"VP6DEC_GetImageBuffer err\012\000" )
	.space	1
.LC38:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC39:
	ASCII(.ascii	"VP6_PrePare_decode Error!\012\000" )
	.space	1
.LC40:
	ASCII(.ascii	"leave VP6DEC_Decode\012\000" )
	.space	3
.LC41:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC42:
	ASCII(.ascii	"pCurImg is null\012\000" )
	.space	3
.LC43:
	ASCII(.ascii	"err_level(%d) over ref_thr(%d)\012\000" )
.LC44:
	ASCII(.ascii	"line: %d pToQueImg is null!\012\000" )
	.space	3
.LC45:
	ASCII(.ascii	"insert img to Voqueue failed!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
