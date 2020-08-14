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
	.file	"real8.c"
	.text
	.align	2
	.global	Real8_CB_GetBits
	.type	Real8_CB_GetBits, %function
Real8_CB_GetBits:
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
	mov	r5, r0
	mov	r6, r1
	bl	BsShow
	mov	r1, r6
	mov	r4, r0
	mov	r0, r5
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_CB_GetBits, .-Real8_CB_GetBits
	.align	2
	.global	Real8_CB_Get1Bit
	.type	Real8_CB_Get1Bit, %function
Real8_CB_Get1Bit:
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
	mov	r1, #1
	mov	r5, r0
	bl	BsShow
	mov	r1, #1
	mov	r4, r0
	mov	r0, r5
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_CB_Get1Bit, .-Real8_CB_Get1Bit
	.align	2
	.global	Real8_CB_GetVLCBits
	.type	Real8_CB_GetVLCBits, %function
Real8_CB_GetVLCBits:
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
	ldr	r7, .L8
	mov	r4, #0
	mov	r6, r0
	mov	r8, r1
	mov	r5, r4
.L4:
	mov	r1, #8
	mov	r0, r6
	bl	BsShow
	ldrb	r3, [r7, r0]	@ zero_extendqisi2
	mov	r0, r6
	and	r2, r3, #15
	mov	r1, r3, lsr #4
	mov	r3, r3, lsr #5
	add	r4, r4, r1
	orr	r5, r2, r5, asl r3
	bl	BsSkip
	tst	r4, #1
	beq	.L4
	str	r5, [r8]
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L9:
	.align	2
.L8:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	Real8_CB_GetVLCBits, .-Real8_CB_GetVLCBits
	.align	2
	.global	Real8_GetImageBuffer
	.type	Real8_GetImageBuffer, %function
Real8_GetImageBuffer:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r4, r0, #53248
	mov	r6, r0
	mov	r1, #1
	add	r7, r0, #8192
	ldr	r0, [r4, #936]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #540]
	blt	.L17
	mov	r1, r0
	ldr	r0, [r4, #936]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L18
	ldr	r2, [r4, #540]
	mov	r0, #18
	ldr	r1, .L19
	bl	dprint_vfmw
	ldr	r2, [r5, #640]
	cmp	r2, #0
	beq	.L16
	ldr	r3, [r5, #644]
	cmp	r3, #0
	beq	.L16
	ldr	r1, [r5, #648]
	cmp	r1, #0
	beq	.L16
	ldr	ip, [r1, #16]
	mov	r0, #18
	ldr	r3, [r3, #16]
	ldr	r2, [r2, #16]
	ldr	r1, .L19+4
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r0, #1
.L12:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L16:
	mov	r0, #1
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L17:
	ldr	r1, .L19+8
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r4, #536]
	ldr	r0, [r4, #936]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #532]
	mov	r5, #0
	ldr	r0, [r4, #936]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #532]
	str	r5, [r4, #536]
	ldr	r0, [r4, #936]
	bl	FSP_GetNullLogicFs
	add	r1, r6, #53760
	add	r1, r1, #72
	str	r0, [r4, #536]
	str	r0, [r4, #532]
	strb	r5, [r7, #3199]
	ldr	r0, [r4, #936]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L18:
	movw	r2, #517
	ldr	r1, .L19+12
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r5
	b	.L12
.L20:
	.align	2
.L19:
	.word	.LC6
	.word	.LC7
	.word	.LC4
	.word	.LC5
	UNWIND(.fnend)
	.size	Real8_GetImageBuffer, .-Real8_GetImageBuffer
	.align	2
	.global	Real8_ArrangeVHBMem
	.type	Real8_ArrangeVHBMem, %function
Real8_ArrangeVHBMem:
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
	UNWIND(.pad #52)
	sub	sp, sp, #52
	mov	r4, r2
	ldr	r2, [r2, #96]
	mov	r5, r1
	ldr	r3, [r4, #92]
	cmp	r2, #1
	bls	.L23
	cmp	r3, #1
	bls	.L23
	ldr	r1, [r1]
	ldr	r3, [r1, #708]
	cmp	r3, #0
	ldr	r3, [r1, #28]
	bne	.L26
	cmp	r3, #25
	beq	.L50
.L27:
	ldr	r9, [r4, #88]
	mov	r7, #5
	ldr	r10, [r4, #84]
	mov	r6, #2
	mov	r8, #4
.L32:
	ldr	r3, .L56
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r0, [r4, #84]
	ldr	r1, [r5]
	mov	r3, #0
	ldr	r2, [r4, #88]
	str	r0, [fp, #-48]
	ldr	r1, [r1, #28]
	str	r9, [fp, #-68]
	cmp	r1, #24
	str	r10, [fp, #-64]
	str	r2, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L38
	uxtb	r3, r6
	strb	r8, [fp, #-73]
	strb	r7, [fp, #-71]
.L42:
	add	r6, r5, #53248
	sub	r1, fp, #76
	mov	r4, #0
	strb	r3, [fp, #-70]
	ldr	r0, [r6, #936]
	strb	r4, [fp, #-74]
	bl	FSP_ConfigInstance
	cmp	r0, r4
	beq	.L51
.L48:
	mov	r0, #0
.L47:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L26:
	cmp	r3, #25
	bne	.L27
	ldr	r2, [r4, #88]
	ldr	r0, [r1, #668]
	cmp	r2, r0
	bhi	.L52
	ldr	r3, [r4, #84]
	ldr	ip, [r1, #672]
	cmp	r3, ip
	bhi	.L35
	ldr	r0, [r1, #696]
	mov	r9, r2
	ldr	r6, [r1, #692]
	mov	r10, r3
	cmp	r0, #0
	add	r8, r6, #2
	bne	.L53
.L45:
	mov	r6, #1
.L37:
	ldrb	r7, [r1, #716]	@ zero_extendqisi2
	b	.L32
.L23:
	ldr	r1, .L56+4
	mov	r0, #0
	bl	dprint_vfmw
	b	.L48
.L51:
	ldr	r1, [r5]
	sub	r3, fp, #80
	ldr	r0, [r6, #936]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L54
	cmp	r0, #0
	addeq	r5, r5, #8192
	moveq	r3, #1
	moveq	r0, r3
	streqb	r3, [r5, #3196]
	bne	.L40
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L38:
	mov	r3, #1
	strb	r7, [fp, #-71]
	strb	r3, [fp, #-73]
	b	.L42
.L50:
	ldr	r2, [r4, #88]
	ldr	r9, [r1, #668]
	cmp	r2, r9
	bhi	.L55
	ldr	r3, [r4, #84]
	ldr	r10, [r1, #672]
	cmp	r3, r10
	bhi	.L29
	add	r3, r5, #8192
	ldrb	r0, [r3, #3196]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L47
	ldr	r2, [r1, #696]
	ldr	r3, [r1, #692]
	cmp	r2, #0
	add	r8, r3, #2
	beq	.L45
	add	r3, r3, #1
	cmp	r3, #17
	movlt	r6, r3
	movge	r6, #17
	b	.L37
.L52:
	ldr	ip, [r1, #672]
	ldr	r3, [r4, #84]
.L35:
	stmia	sp, {r0, ip}
	mov	r0, #0
	ldr	r1, .L56+8
	bl	dprint_vfmw
	mov	r0, #0
	b	.L47
.L55:
	ldr	r10, [r1, #672]
	ldr	r3, [r4, #84]
.L29:
	stmia	sp, {r9, r10}
	mov	r0, #0
	ldr	r1, .L56+8
	bl	dprint_vfmw
	mov	r0, #0
	b	.L47
.L53:
	add	r3, r6, #1
	cmp	r3, #17
	movlt	r6, r3
	movge	r6, #17
	b	.L37
.L54:
	add	r0, r5, #53760
	add	r0, r0, #72
	bl	ResetVoQueue
.L40:
	ldr	r1, .L56+12
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	b	.L47
.L57:
	.align	2
.L56:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC8
	.word	.LC9
	.word	.LC10
	UNWIND(.fnend)
	.size	Real8_ArrangeVHBMem, .-Real8_ArrangeVHBMem
	.align	2
	.global	Real8_CB_GetCPFMT
	.type	Real8_CB_GetCPFMT, %function
Real8_CB_GetCPFMT:
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
	mov	r6, r1
	mov	r1, #4
	mov	r5, r0
	bl	BsShow
	mov	r1, #4
	mov	r4, r0
	mov	r0, r5
	bl	BsSkip
	strb	r4, [r6]
	mov	r1, #9
	mov	r0, r5
	bl	BsShow
	mov	r1, #9
	mov	r4, r0
	add	r4, r4, #1
	mov	r0, r5
	bl	BsSkip
	mov	r4, r4, asl #2
	mov	r0, r5
	str	r4, [r6, #88]
	bl	Real8_CB_Get1Bit
	cmp	r0, #1
	mov	r4, r0
	beq	.L61
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L61:
	mov	r1, #9
	mov	r0, r5
	bl	BsShow
	mov	r1, #9
	mov	r7, r0
	mov	r0, r5
	bl	BsSkip
	mov	r7, r7, asl #2
	mov	r0, r4
	str	r7, [r6, #84]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_CB_GetCPFMT, .-Real8_CB_GetCPFMT
	.align	2
	.global	Real8_CB_SetDimensions
	.type	Real8_CB_SetDimensions, %function
Real8_CB_SetDimensions:
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
	ldr	r3, [r0, #88]
	ldr	r2, [r0, #84]
	cmp	r3, #128
	beq	.L79
	cmp	r3, #176
	beq	.L80
	cmp	r3, #352
	beq	.L81
	cmp	r3, #704
	beq	.L82
	cmp	r3, #1408
	beq	.L83
.L64:
	add	r3, r3, #15
	add	r2, r2, #15
	mov	r3, r3, lsr #4
	mov	r2, r2, lsr #4
	str	r3, [r0, #28]
	mul	r3, r3, r2
	str	r2, [r0, #24]
	sub	r3, r3, #1
	cmp	r3, #47
	movls	r2, #0
	bls	.L65
	cmp	r3, #98
	bls	.L74
	cmp	r3, #396
	bcc	.L75
	cmp	r3, #1584
	bcc	.L76
	cmp	r3, #6336
	movcs	r2, #5
	bcc	.L78
.L65:
	ldr	r3, .L84
	add	r3, r3, r2, lsl #2
	ldr	r3, [r3, #256]
	str	r3, [r0, #32]
	ldmfd	sp, {fp, sp, pc}
.L83:
	cmp	r2, #1152
	bne	.L64
	mov	r2, #88
	mov	r3, #72
	str	r2, [r0, #28]
	str	r3, [r0, #24]
.L78:
	mov	r2, #4
	b	.L65
.L79:
	cmp	r2, #96
	bne	.L64
	mov	r2, #8
	mov	r3, #6
	str	r2, [r0, #28]
	mov	r2, #0
	str	r3, [r0, #24]
	b	.L65
.L80:
	cmp	r2, #144
	bne	.L64
	mov	r2, #11
	mov	r3, #9
	str	r2, [r0, #28]
	str	r3, [r0, #24]
.L74:
	mov	r2, #1
	b	.L65
.L81:
	cmp	r2, #288
	bne	.L64
	mov	r2, #22
	mov	r3, #18
	str	r2, [r0, #28]
	str	r3, [r0, #24]
.L75:
	mov	r2, #2
	b	.L65
.L82:
	cmp	r2, #576
	bne	.L64
	mov	r2, #44
	mov	r3, #36
	str	r2, [r0, #28]
	str	r3, [r0, #24]
.L76:
	mov	r2, #3
	b	.L65
.L85:
	.align	2
.L84:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	Real8_CB_SetDimensions, .-Real8_CB_SetDimensions
	.align	2
	.global	Real8_CB_GetPicSize
	.type	Real8_CB_GetPicSize, %function
Real8_CB_GetPicSize:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 80
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #84)
	sub	sp, sp, #84
	ldr	ip, .L102
	mov	r9, r1
	mov	r4, r0
	mov	r8, r2
	mov	r7, r3
	ldmia	ip!, {r0, r1, r2, r3}
	sub	r6, fp, #108
	sub	lr, fp, #76
	add	r5, ip, #16
	add	r10, ip, #48
	stmia	r6!, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	stmia	r6, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r5, {r0, r1, r2, r3}
	stmia	lr, {r0, r1, r2, r3}
	ldmia	r10, {r0, r1, r2, r3}
	ldr	ip, [r9, #32]
	sub	ip, ip, #2
	cmp	ip, #1
	sub	ip, fp, #124
	stmia	ip, {r0, r1, r2, r3}
	bls	.L100
.L87:
	mov	r1, #3
	mov	r0, r4
	bl	BsShow
	mov	r1, #3
	mov	r5, r0
	mov	r0, r4
	bl	BsSkip
	sub	r3, fp, #44
	add	r5, r3, r5, lsl #2
	ldr	r6, [r5, #-64]
	cmp	r6, #0
	beq	.L90
.L89:
	mov	r1, #3
	mov	r0, r4
	bl	BsShow
	mov	r1, #3
	mov	r9, r0
	mov	r0, r4
	bl	BsSkip
	sub	r3, fp, #44
	add	r3, r3, r9, lsl #2
	ldr	r5, [r3, #-32]
	cmp	r5, #0
	beq	.L101
.L91:
	str	r6, [r8]
	str	r5, [r7]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L90:
	mov	r1, #8
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	mov	r5, r0
	mov	r0, r4
	bl	BsSkip
	cmp	r5, #255
	add	r6, r6, r5, lsl #2
	beq	.L90
	b	.L89
.L100:
	mov	r0, r4
	bl	Real8_CB_Get1Bit
	cmp	r0, #0
	beq	.L87
	ldr	r3, [r9, #64]
	str	r3, [r8]
	ldr	r3, [r9, #68]
	str	r3, [r7]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L101:
	mov	r0, r4
	mov	r9, r9, asl #1
	bl	Real8_CB_Get1Bit
	sub	r2, fp, #44
	orr	r0, r0, r9
	and	r3, r0, #3
	add	r3, r2, r3, lsl #2
	ldr	r5, [r3, #-80]
	cmp	r5, #0
	bne	.L91
.L92:
	mov	r1, #8
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	mov	r9, r0
	mov	r0, r4
	bl	BsSkip
	cmp	r9, #255
	add	r5, r5, r9, lsl #2
	beq	.L92
	b	.L91
.L103:
	.align	2
.L102:
	.word	.LANCHOR0+280
	UNWIND(.fnend)
	.size	Real8_CB_GetPicSize, .-Real8_CB_GetPicSize
	.align	2
	.global	Real8_CB_FindNextSliceStartCode
	.type	Real8_CB_FindNextSliceStartCode, %function
Real8_CB_FindNextSliceStartCode:
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
	mov	r5, r0
	bl	BsToNextByte
	mov	r0, r5
	bl	BsResidBits
	add	r6, r0, #7
	cmp	r0, #0
	movlt	r0, r6
	mov	r6, r0, asr #3
	cmp	r6, #2
	subgt	r6, r6, #2
	movgt	r4, #0
	bgt	.L107
	b	.L108
.L113:
	bl	BsSkip
	cmp	r4, r6
	beq	.L108
.L107:
	mov	r1, #17
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r5
	bne	.L113
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L108:
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_CB_FindNextSliceStartCode, .-Real8_CB_FindNextSliceStartCode
	.align	2
	.global	Real8_CB_SearchBits_IgnoreOnly0
	.type	Real8_CB_SearchBits_IgnoreOnly0, %function
Real8_CB_SearchBits_IgnoreOnly0:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r6, #1
	sub	ip, r1, #1
	mov	r7, r2
	mov	r9, r3
	mov	r10, r1
	mov	r8, r0
	mov	r6, r6, asl ip
	bl	BsShow
	mov	r1, r10
	mov	r4, r0
	mov	r0, r8
	bl	BsSkip
	cmp	r7, r4
	cmpne	r9, #0
	beq	.L115
	ands	r5, r4, r6
	bne	.L115
	ldr	r3, .L121
	add	r10, r3, r10, lsl #2
	ldr	r10, [r10, #360]
	b	.L116
.L117:
	tst	r4, r6
	bne	.L115
.L116:
	mov	r0, r8
	and	r4, r10, r4, asl #1
	bl	Real8_CB_Get1Bit
	add	r5, r5, #1
	orr	r4, r0, r4
	cmp	r7, r4
	cmpne	r9, r5
	bhi	.L117
.L115:
	rsb	r0, r7, r4
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L122:
	.align	2
.L121:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	Real8_CB_SearchBits_IgnoreOnly0, .-Real8_CB_SearchBits_IgnoreOnly0
	.align	2
	.global	Real8_CB_SetRPRSizes
	.type	Real8_CB_SetRPRSizes, %function
Real8_CB_SetRPRSizes:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #40)
	sub	sp, sp, #40
	ldr	lr, .L124
	sub	r3, fp, #28
	mov	r4, r0
	mov	r5, r1
	mov	r7, r2
	add	r6, r3, r1, lsl #2
	sub	ip, fp, #64
	ldmia	lr!, {r0, r1, r2, r3}
	str	r5, [r4, #16]
	str	r7, [r4, #12]
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	ldr	lr, [lr]
	stmia	ip!, {r0, r1, r2, r3}
	str	lr, [ip]
	ldr	r3, [r6, #-36]
	str	r3, [r4, #8]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L125:
	.align	2
.L124:
	.word	.LANCHOR0+460
	UNWIND(.fnend)
	.size	Real8_CB_SetRPRSizes, .-Real8_CB_SetRPRSizes
	.align	2
	.global	Real8_CB_GetSSC
	.type	Real8_CB_GetSSC, %function
Real8_CB_GetSSC:
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
	ldr	r3, [r1, #76]
	cmp	r3, #1
	beq	.L128
	bl	Real8_CB_FindNextSliceStartCode
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L128:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_CB_GetSSC, .-Real8_CB_GetSSC
	.align	2
	.global	Real8_CB_GetSliceHeader
	.type	Real8_CB_GetSliceHeader, %function
Real8_CB_GetSliceHeader:
	UNWIND(.fnstart)
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, [fp, #8]
	mov	r5, r2
	mov	r7, r3
	mov	r6, r0
	ldr	ip, [r4, #76]
	cmp	ip, #1
	beq	.L163
	cmp	r1, #0
	beq	.L164
	mov	r2, #0
	str	r2, [r5]
	ldr	r2, [fp, #4]
	mvn	r3, #0
	mov	r0, #1
	str	r3, [r2]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L163:
	cmp	r1, #0
	bne	.L132
	mov	r1, #3
	ldr	r9, [r4, #40]
	bl	BsShow
	mov	r1, #3
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	cmp	r8, #7
	bhi	.L135
	ldr	r3, .L167
	add	r8, r3, r8, lsl #2
	ldr	r3, [r8, #496]
	cmp	r3, #2
	bhi	.L135
	cmn	r9, #1
	mov	r1, #2
	mov	r0, r6
	movne	r9, #0
	moveq	r9, #1
	cmp	r9, #0
	bne	.L165
	bl	BsShow
	mov	r1, #2
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	ldr	r3, [r4, #36]
	cmp	r3, r8
	bne	.L135
.L137:
	mov	r0, r6
	bl	Real8_CB_Get1Bit
	cmp	r0, #0
	bne	.L135
	mov	r1, #5
	mov	r0, r6
	bl	BsShow
	mov	r1, #5
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	uxtb	r0, r8
	cmp	r0, #31
	strb	r0, [r4, #6]
	bhi	.L135
	mov	r1, #1
	mov	r0, r6
	bl	BsShow
	mov	r1, #1
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	adds	r8, r8, #0
	mov	r1, #13
	mov	r0, r6
	movne	r8, #1
	cmp	r9, #0
	strb	r8, [r4, #5]
	beq	.L138
	bl	BsShow
	mov	r1, #13
	mov	r9, r0
	mov	r0, r6
	bl	BsSkip
	ldr	r8, [r4, #8]
	str	r9, [r4, #44]
	cmp	r8, #0
	beq	.L156
	mov	r1, r8
	mov	r0, r6
	bl	BsShow
	mov	r1, r8
	mov	r9, r0
	mov	r0, r6
	bl	BsSkip
	ldr	r3, [r4, #12]
	str	r9, [r4, #56]
	cmp	r3, #0
	beq	.L135
	add	r1, r3, r9, lsl #3
	ldr	r2, [r3, r9, asl #3]
	mov	r0, r4
	ldr	r3, [r1, #4]
	str	r2, [r4, #60]
	str	r2, [r4, #88]
	str	r3, [r4, #64]
	str	r3, [r4, #84]
	bl	Real8_CB_SetDimensions
.L156:
	ldr	r8, [r4, #32]
	mov	r0, r6
	mov	r1, r8
	bl	BsShow
	mov	r1, r8
	mov	r9, r0
	mov	r0, r6
	bl	BsSkip
	mov	r1, #1
	str	r9, [r4, #40]
	mov	r0, r6
	bl	BsShow
	mov	r1, #1
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	strb	r8, [r4, #4]
.L132:
	ldr	r2, [r4, #40]
	mov	r3, #0
	ldr	r1, [fp, #4]
	mov	r0, #1
	str	r2, [r5]
	ldrb	r2, [r4, #6]	@ zero_extendqisi2
	str	r2, [r1]
	str	r3, [r7]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L164:
	bl	Real8_CB_Get1Bit
	cmp	r0, #1
	beq	.L166
.L135:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L166:
	ldr	r8, [r4, #32]
	mov	r0, r6
	mov	r1, r8
	bl	BsShow
	mov	r1, r8
	mov	r9, r0
	mov	r0, r6
	bl	BsSkip
	str	r9, [r5]
	ldr	r3, [r4, #32]
	cmp	r3, #11
	bls	.L148
	mov	r0, r6
	bl	Real8_CB_Get1Bit
	cmp	r0, #1
	bne	.L135
.L148:
	mov	r1, #5
	mov	r0, r6
	bl	BsShow
	mov	r1, #5
	mov	r4, r0
	mov	r0, r6
	bl	BsSkip
	ldr	r3, [fp, #4]
	cmp	r4, #31
	str	r4, [r3]
	bgt	.L135
	mov	r0, r6
	bl	Real8_CB_Get1Bit
	cmp	r0, #1
	mov	r4, r0
	bne	.L135
	mov	r1, #2
	mov	r0, r6
	bl	BsShow
	mov	r1, #2
	mov	r5, r0
	mov	r0, r6
	bl	BsSkip
	str	r5, [r7]
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L165:
	bl	BsShow
	mov	r1, #2
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	str	r8, [r4, #36]
	b	.L137
.L138:
	bl	BsShow
	mov	r1, #13
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	ldr	r3, [r4, #44]
	cmp	r3, r8
	bne	.L135
	ldr	r8, [r4, #8]
	cmp	r8, #0
	beq	.L160
	mov	r1, r8
	mov	r0, r6
	bl	BsShow
	mov	r1, r8
	mov	r9, r0
	mov	r0, r6
	bl	BsSkip
	ldr	r3, [r4, #56]
	cmp	r3, r9
	bne	.L135
.L160:
	ldr	r8, [r4, #32]
	mov	r0, r6
	mov	r1, r8
	bl	BsShow
	mov	r1, r8
	mov	r9, r0
	mov	r0, r6
	bl	BsSkip
	mov	r1, #1
	str	r9, [r4, #40]
	mov	r0, r6
	bl	BsShow
	mov	r1, #1
	mov	r8, r0
	mov	r0, r6
	bl	BsSkip
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, r8
	bne	.L135
	b	.L132
.L168:
	.align	2
.L167:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	Real8_CB_GetSliceHeader, .-Real8_CB_GetSliceHeader
	.align	2
	.global	Real8_CB_GetPictureHeader
	.type	Real8_CB_GetPictureHeader, %function
Real8_CB_GetPictureHeader:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r3, [r1, #76]
	mov	r4, r1
	mov	r5, r0
	cmp	r3, #1
	beq	.L195
	ldr	r3, .L198
	mov	r2, #140
	mov	r1, #0
	mov	r0, r4
	ldr	r3, [r3, #48]
	blx	r3
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	mov	r1, #24
	mov	r6, r0
	mov	r0, r5
	bl	BsSkip
	cmp	r6, #1
	bne	.L188
	mov	r0, r5
	sub	r1, fp, #32
	bl	Real8_CB_GetVLCBits
	cmp	r0, #30
	bls	.L188
	ldr	r3, [fp, #-32]
	ands	r2, r3, #1
	bne	.L188
	tst	r3, #2
	mov	r0, r5
	strne	r2, [r4, #88]
	moveq	r1, #176
	moveq	r2, #144
	streq	r1, [r4, #88]
	sub	r1, fp, #32
	str	r2, [r4, #84]
	ubfx	r2, r3, #2, #5
	ubfx	r3, r3, #7, #8
	str	r2, [r4, #120]
	str	r3, [r4, #108]
	bl	Real8_CB_GetVLCBits
	cmp	r0, #1
	streq	r0, [r4, #80]
	beq	.L183
	cmp	r0, #3
	beq	.L196
	cmp	r0, #5
	bne	.L188
	ldr	r3, [fp, #-32]
	cmp	r3, #0
	bne	.L188
	mov	r3, #2
	str	r3, [r4, #80]
.L183:
	ldr	r3, [r4, #88]
	cmp	r3, #0
	beq	.L197
.L185:
	sub	r3, r3, #48
	movw	r2, #8144
	cmp	r3, r2
	bhi	.L188
.L194:
	ldr	r3, [r4, #84]
	sub	r3, r3, #48
	cmp	r3, r2
	bhi	.L188
	mov	r0, r4
	bl	Real8_CB_SetDimensions
	mov	r0, #1
.L171:
	ldr	r3, [r4, #88]
	ldr	r2, [r4, #84]
	add	r3, r3, #15
	add	r2, r2, #15
	mov	r3, r3, lsr #4
	str	r3, [r4, #96]
	mov	r2, r2, lsr #4
	str	r2, [r4, #92]
	mul	r3, r3, r2
	str	r3, [r4, #100]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L196:
	ldr	r3, [fp, #-32]
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [r4, #80]
	beq	.L183
.L188:
	mvn	r0, #1
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L195:
	str	r1, [sp, #4]
	mvn	r2, #0
	sub	r3, fp, #32
	str	r2, [r1, #40]
	str	r3, [sp]
	sub	r2, fp, #36
	sub	r3, fp, #40
	mov	r1, #0
	bl	Real8_CB_GetSliceHeader
	cmp	r0, #1
	mvnne	r0, #1
	bne	.L171
	ldr	r3, [r4, #36]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L172
.L174:
	.word	.L173
	.word	.L175
	.word	.L176
	.word	.L177
.L197:
	mov	r0, r5
	mov	r1, r4
	bl	Real8_CB_GetCPFMT
	cmp	r0, #0
	beq	.L188
	ldr	r3, [r4, #88]
	b	.L185
.L175:
	mov	r3, #0
	str	r3, [r4, #80]
	str	r3, [r4, #20]
.L172:
	ldr	r3, [r4, #88]
	add	r0, r4, #44
	movw	r2, #8144
	ldrb	r5, [r4, #4]	@ zero_extendqisi2
	sub	r3, r3, #48
	ldrb	lr, [r4, #5]	@ zero_extendqisi2
	cmp	r3, r2
	ldrb	ip, [r4, #6]	@ zero_extendqisi2
	ldmia	r0, {r0, r1, r3}
	str	r5, [r4, #112]
	str	lr, [r4, #116]
	str	ip, [r4, #120]
	str	r0, [r4, #108]
	str	r1, [r4, #128]
	str	r3, [r4, #132]
	bls	.L194
	b	.L188
.L177:
	mov	r3, #2
	str	r3, [r4, #80]
	b	.L172
.L176:
	mov	r3, #1
	str	r3, [r4, #80]
	b	.L172
.L173:
	mov	r2, #0
	mov	r3, #1
	str	r2, [r4, #80]
	str	r3, [r4, #20]
	b	.L172
.L199:
	.align	2
.L198:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	Real8_CB_GetPictureHeader, .-Real8_CB_GetPictureHeader
	.align	2
	.global	Real8_SetImgFormat
	.type	Real8_SetImgFormat, %function
Real8_SetImgFormat:
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
	add	r3, r0, #53248
	mov	r6, r0
	ldr	r1, [r3, #540]
	ldr	r0, [r3, #936]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L200
	add	ip, r6, #49152
	ldr	r2, [ip, #3352]
	and	r2, r2, #3
	orr	r3, r2, #7168
	orr	r3, r3, #32
	str	r3, [r4, #108]
	ldr	r1, [ip, #3360]
	cmp	r1, #1920
	orrls	r3, r2, #138240
	orrls	r3, r3, #32
	strls	r3, [r4, #108]
	ldr	r1, [r6]
	ldr	r1, [r1, #592]
	cmp	r1, #1
	movne	r1, #0
	beq	.L210
.L203:
	orr	r3, r1, r3
	str	r2, [r4, #100]
	str	r3, [r4, #108]
	mov	r5, #0
	str	r2, [r4, #104]
	mov	r1, r5
	str	r5, [r4, #96]
	mov	r0, r4
	ldr	r3, [ip, #3360]
	str	r3, [r4, #112]
	ldr	r3, [ip, #3356]
	str	r3, [r4, #116]
	bl	SetAspectRatio
	ldr	r3, [r6]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L211
.L204:
	mov	r3, #8
	mov	r2, #1
	mov	r1, #0
	str	r3, [r4, #376]
	str	r3, [r4, #380]
	str	r3, [r4, #388]
	str	r3, [r4, #384]
	strb	r1, [r4, #364]
	strb	r2, [r4, #371]
	strb	r2, [r4, #369]
.L200:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L210:
	tst	r3, #131072
	movne	r1, #0
	moveq	r1, #262144
	b	.L203
.L211:
	ldr	r3, [r4, #116]
	add	r8, r4, #512
	ldr	r2, [r4, #140]
	add	r1, r4, #528
	add	r3, r3, #15
	ldr	ip, [r4, #44]
	mov	r0, r2, asl #4
	mov	r3, r3, lsr #4
	add	r9, ip, r2
	mul	r3, r3, r0
	add	lr, r3, ip
	str	lr, [r4, #48]
	ldrd	r6, [r8, #-8]
	add	r2, lr, r2
	mov	r0, lr
	strd	r6, [fp, #-44]
	adds	r6, r6, r3
	adc	r7, r7, r3, asr #31
	strd	r6, [r8]
	str	r2, [r4, #40]
	mov	r6, r1
	ldrd	r2, [fp, #-44]
	str	lr, [r4, #32]
	str	ip, [r4, #28]
	str	r9, [r4, #36]
	strd	r2, [r1, #-8]
	str	ip, [r4, #60]
	bl	MEM_Phy2Vir
	str	r0, [r4, #528]
	str	r5, [r6, #4]
	ldr	r3, [r4, #32]
	str	r3, [r4, #64]
	b	.L204
	UNWIND(.fnend)
	.size	Real8_SetImgFormat, .-Real8_SetImgFormat
	.align	2
	.global	RV8_RecordVFMWState
	.type	RV8_RecordVFMWState, %function
RV8_RecordVFMWState:
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
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	RV8_RecordVFMWState, .-RV8_RecordVFMWState
	.align	2
	.global	Real8_WriteSliceMsg
	.type	Real8_WriteSliceMsg, %function
Real8_WriteSliceMsg:
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
	mov	r2, #44
	add	r3, r0, #49152
	mla	r2, r2, r1, r0
	ldr	r4, [r3, #3440]
	add	r1, r1, #1
	add	ip, r2, #128
	add	lr, r2, #136
	str	r4, [r2, #108]
	ldr	r4, [r3, #3432]
	str	r4, [r2, #116]
	ldr	r4, [r3, #3436]
	str	r4, [r2, #124]
	ldr	r4, [r3, #3452]
	str	r4, [r2, #112]
	ldr	r4, [r3, #3444]
	str	r4, [r2, #120]
	ldr	r4, [r3, #3448]
	str	r4, [r2, #128]
	ldr	r2, [r3, #3420]
	str	r2, [ip, #4]
	ldr	r2, [r3, #3416]
	str	r2, [ip, #8]
	ldr	r2, [r3, #3412]
	str	r2, [lr, #4]
	ldr	r3, [r3, #3424]
	str	r3, [lr, #8]
	str	r1, [r0, #44]
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_WriteSliceMsg, .-Real8_WriteSliceMsg
	.align	2
	.global	Real8_WritePicMsg
	.type	Real8_WritePicMsg, %function
Real8_WritePicMsg:
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
	add	r3, r0, #49152
	add	r2, r0, #53248
	ldr	r1, [r3, #3352]
	cmp	r1, #2
	strb	r1, [r0, #20]
	ldr	r1, [r3, #3368]
	str	r1, [r0, #12]
	ldr	r1, [r3, #3364]
	str	r1, [r0, #16]
	ldr	r1, [r2, #456]
	str	r1, [r0, #24]
	ldr	r2, [r2, #460]
	str	r2, [r0, #28]
	ldr	r2, [r3, #3392]
	str	r2, [r0, #32]
	ldr	r1, [r3, #3456]
	str	r1, [r0, #36]
	ldr	r1, [r3, #3460]
	str	r1, [r0, #40]
	strne	r2, [r3, #3456]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_WritePicMsg, .-Real8_WritePicMsg
	.align	2
	.global	Real8_WriteReg
	.type	Real8_WriteReg, %function
Real8_WriteReg:
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
	str	r1, [r0, #48]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_WriteReg, .-Real8_WriteReg
	.align	2
	.global	Real8_ModifySliceMsg
	.type	Real8_ModifySliceMsg, %function
Real8_ModifySliceMsg:
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
	ldr	r2, [r0, #144]
	ldr	r3, [r0, #12]
	cmp	r2, #0
	ldr	ip, [r0, #16]
	bne	.L224
	mul	ip, r3, ip
	ldr	r5, [r0, #44]
	cmp	r5, #2
	sub	ip, ip, #1
	bls	.L223
	ldr	r3, [r0, #188]
	sub	lr, r5, #1
	cmp	r3, #0
	cmpgt	ip, r3
	ble	.L224
	mov	r2, r0
	mov	r3, #1
	b	.L225
.L226:
	ldr	r1, [r2, #232]
	add	r2, r2, #44
	cmp	ip, r1
	cmpgt	r1, #0
	ble	.L224
.L225:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L226
.L223:
	cmp	r5, #1
	bls	.L227
	sub	r4, r5, #1
	mov	r3, r0
	mov	r2, #0
	b	.L228
.L229:
	ldr	lr, [r3, #144]
	add	r3, r3, #44
	add	r2, r2, #1
	ldr	r1, [r3, #144]
	cmp	lr, r1
	bge	.L224
.L228:
	cmp	r2, r4
	bne	.L229
	mov	r3, #44
	mla	r2, r3, r2, r0
	ldr	r3, [r2, #144]
	cmp	ip, r3
	cmpge	r3, #0
	ble	.L224
.L227:
	cmp	r5, #0
	beq	.L230
	sub	r7, r5, #1
	mov	r6, r0
	mov	lr, r0
	mov	r4, #0
	b	.L233
.L248:
	cmp	r4, r5
	beq	.L247
.L233:
	cmp	r4, r7
	streq	ip, [lr, #148]
	add	r4, r4, #1
	add	lr, lr, #44
	ldrne	r3, [lr, #144]
	moveq	r1, ip
	subne	r1, r3, #1
	ldr	r3, [lr, #100]
	strne	r1, [lr, #104]
	mov	r2, r3, lsr #31
	cmp	ip, r3
	orrlt	r2, r2, #1
	cmp	r3, r1
	movle	r3, r2
	orrgt	r3, r2, #1
	orr	r3, r3, r1, lsr #31
	cmp	ip, r1
	orrlt	r3, r3, #1
	cmp	r3, #0
	beq	.L248
.L224:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L247:
	ldr	r2, [r0, #108]
	mov	r1, r2
	b	.L236
.L249:
	ldr	r1, [r6, #108]
.L236:
	cmp	r2, r1
	ldr	ip, [r6, #112]
	add	r3, r3, #1
	movcs	r2, r1
	cmp	ip, r2
	bcs	.L234
	ldr	r1, [r6, #128]
	cmp	r1, #0
	movne	r2, ip
.L234:
	cmp	r5, r3
	add	r6, r6, #44
	bne	.L249
	bic	r2, r2, #15
	ldr	lr, [r0, #44]
	str	r2, [r0, #52]
	mov	r3, r0
	mov	ip, #0
	b	.L240
.L250:
	ldr	r4, [r3, #116]
	rsb	r2, r2, r1
	and	r1, r4, #7
	add	r2, r2, r4, lsr #3
	and	r4, r2, #15
	bic	r2, r2, #15
	str	r2, [r3, #108]
	add	r2, r1, r4, lsl #3
	str	r2, [r3, #116]
.L238:
	add	ip, ip, #1
	add	r3, r3, #44
	cmp	lr, ip
	bls	.L239
	ldr	r2, [r0, #52]
.L240:
	ldr	r1, [r3, #112]
	cmp	r1, #0
	rsb	r4, r2, r1
	ldr	r1, [r3, #108]
	beq	.L250
	ldr	r5, [r3, #116]
	rsb	r2, r2, r1
	ldr	r7, [r3, #120]
	and	r6, r5, #7
	add	r2, r2, r5, lsr #3
	and	r5, r7, #7
	add	r1, r4, r7, lsr #3
	and	r4, r2, #15
	and	r7, r1, #15
	bic	r2, r2, #15
	add	r4, r6, r4, lsl #3
	str	r2, [r3, #108]
	bic	r1, r1, #15
	add	r2, r5, r7, lsl #3
	str	r4, [r3, #116]
	str	r2, [r3, #120]
	str	r1, [r3, #112]
	b	.L238
.L230:
	ldr	r3, [r0, #108]
	bic	r3, r3, #15
	str	r3, [r0, #52]
.L239:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real8_ModifySliceMsg, .-Real8_ModifySliceMsg
	.align	2
	.global	REAL8DEC_Init
	.type	REAL8DEC_Init, %function
REAL8DEC_Init:
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
	ldr	r3, .L255
	add	r6, r0, #53248
	mov	r7, r1
	movw	r2, #54200
	ldr	r8, [r6, #928]
	mov	r1, #0
	ldr	r3, [r3, #48]
	mov	r4, r0
	mov	r5, r1
	blx	r3
	add	r0, r4, #53760
	str	r8, [r6, #928]
	add	r0, r0, #72
	bl	ResetVoQueue
	add	r3, r4, #49152
	str	r7, [r4]
	mov	r2, #15
	str	r2, [r3, #3456]
	add	ip, r4, #8192
	str	r2, [r3, #3460]
	mov	r1, #8192
	ldr	lr, [r7, #8]
	mov	r2, #1
	strb	r5, [ip, #3196]
	mov	r0, r4
	strb	lr, [ip, #3194]
	str	r1, [r3, #3376]
	str	r2, [r3, #3348]
	strb	r2, [r3, #3272]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r6, #936]
	mov	r0, r5
	beq	.L254
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L254:
	ldr	r1, .L255+4
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L256:
	.align	2
.L255:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC11
	UNWIND(.fnend)
	.size	REAL8DEC_Init, .-REAL8DEC_Init
	.align	2
	.global	REAL8DEC_Destroy
	.type	REAL8DEC_Destroy, %function
REAL8DEC_Destroy:
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
	add	r3, r0, #53248
	ldr	r1, .L258
	mov	r0, #2
	ldr	r2, [r3, #432]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L259:
	.align	2
.L258:
	.word	.LC12
	UNWIND(.fnend)
	.size	REAL8DEC_Destroy, .-REAL8DEC_Destroy
	.global	__aeabi_idiv
	.align	2
	.global	REAL8DEC_Decode
	.type	REAL8DEC_Decode, %function
REAL8DEC_Decode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 296
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #316)
	sub	sp, sp, #316
	mov	r2, #140
	mov	r10, r0
	mov	r4, r1
	add	r5, r10, #49152
	mov	r1, #0
	sub	r0, fp, #184
	bl	memset
	mov	r2, #64
	mov	r1, #0
	sub	r0, fp, #248
	bl	memset
	ldr	r2, [r5, #3332]
	ldr	r3, [r5, #3336]
	cmp	r4, #0
	mov	r0, #1
	mov	r1, #8192
	add	ip, r10, #8
	str	r2, [r5, #3340]
	str	ip, [fp, #-296]
	str	r2, [fp, #-116]
	str	r2, [fp, #-124]
	str	r3, [r5, #3344]
	str	r3, [fp, #-112]
	str	r3, [fp, #-120]
	str	r0, [r5, #3348]
	str	r0, [fp, #-108]
	str	r1, [r5, #3376]
	str	r1, [fp, #-80]
	beq	.L393
	ldr	r3, .L415
	add	r6, r10, #53248
	add	ip, r10, #8192
	movw	r2, #11364
	mov	r1, #0
	ldr	r0, [fp, #-296]
	ldr	r3, [r3, #48]
	mov	r8, ip
	mov	r7, r1
	str	ip, [fp, #-300]
	blx	r3
	str	r7, [r6, #396]
	ldr	r3, [r4, #16]
	str	r3, [r6, #416]
	ldr	r3, [r4, #12]
	str	r3, [r6, #400]
	ldr	r3, [r4]
	str	r3, [r8, #3184]
	ldr	r2, [r4, #16]
	str	r2, [r6, #404]
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	strb	r3, [r8, #3192]
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L263
	ldr	r3, [r4, #92]
	str	r3, [r6, #408]
	ldr	r3, [r4, #80]
	str	r3, [r8, #3188]
	ldr	r3, [r4, #96]
	str	r3, [r6, #412]
	ldrb	r3, [r4, #86]	@ zero_extendqisi2
	strb	r3, [r8, #3193]
	ldrb	r3, [r4, #86]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L394
.L264:
	movw	r3, #65460
	sub	r1, r2, #75
	movt	r3, 47
	cmp	r1, r3
	bhi	.L389
	ldr	r8, [fp, #-300]
	ldr	r1, [r8, #3184]
	cmp	r1, #0
	beq	.L389
	add	r4, r10, #53760
	add	r4, r4, #40
	mov	r0, r4
	bl	BsInit
	mov	r1, #8
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	mov	r7, r0
	str	r0, [fp, #-304]
	mov	r0, r4
	bl	BsSkip
	cmp	r7, #8
	bhi	.L389
	sub	r7, fp, #252
.L267:
	mov	r1, #32
	mov	r0, r4
	bl	BsShow
	mov	r1, #32
	mov	r9, r0
	mov	r0, r4
	bl	BsSkip
	sub	r3, fp, #188
	rev	r9, r9
	str	r9, [r7, #4]!
	cmp	r7, r3
	bne	.L267
	mov	r1, #8
	mov	r0, r4
	str	r8, [fp, #-300]
	bl	BsShow
	mov	r1, #8
	mov	r7, r0
	mov	r0, r4
	bl	BsSkip
	cmp	r7, #255
	add	r0, r7, #1
	str	r0, [r5, #3464]
	bhi	.L389
	cmp	r0, #0
	beq	.L268
	add	r9, r10, #52480
	movw	r3, #65535
	add	r9, r9, #136
	movt	r3, 47
	str	r6, [fp, #-308]
	mov	r7, #0
	mov	r6, r9
	mov	r9, r3
	b	.L269
.L396:
	ldr	r0, [r5, #3464]
	cmp	r0, r7
	bls	.L395
.L269:
	mov	r1, #32
	mov	r0, r4
	bl	BsSkip
	mov	r1, #32
	mov	r0, r4
	add	r7, r7, #1
	bl	BsShow
	mov	r1, #32
	mov	r8, r0
	mov	r0, r4
	bl	BsSkip
	rev	r0, r8
	cmp	r0, r9
	str	r0, [r6, #4]!
	ble	.L396
.L389:
	mov	r7, #0
.L378:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L394:
	ldr	r1, [r4, #20]
	ldr	r0, [r6, #936]
	bl	SM_ReleaseStreamSeg
	ldr	r1, [r4, #100]
	ldr	r0, [r6, #936]
	bl	SM_ReleaseStreamSeg
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L263:
	ldr	r3, [fp, #-300]
	str	r7, [r6, #408]
	str	r7, [r3, #3188]
	str	r7, [r6, #412]
	strb	r7, [r3, #3193]
	b	.L264
.L393:
	add	r3, r10, #53248
	ldr	r1, .L415+4
	mov	r7, r4
	ldr	r2, [r3, #432]
	bl	dprint_vfmw
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L395:
	ldr	r6, [fp, #-308]
.L268:
	add	r3, r10, #52480
	sub	lr, r0, #1
	add	r3, r3, #140
	mov	r2, #0
	ldr	r8, [fp, #-300]
	b	.L270
.L271:
	ldr	ip, [r3, #4]!
	ldr	r1, [r3, #-4]
	cmp	ip, r1
	ble	.L389
.L270:
	cmp	r2, lr
	add	r2, r2, #1
	bne	.L271
	ldr	r2, [r5, #3468]
	mov	r0, r0, asl #3
	add	r1, r0, #66
	movw	r3, #65461
	add	r0, r1, r2
	cmp	r1, #3145728
	sub	r9, r0, #74
	movt	r3, 47
	movlt	r2, #0
	movge	r2, #1
	cmp	r9, r3
	movls	r9, r2
	orrhi	r9, r2, #1
	str	r8, [fp, #-300]
	cmp	r9, #0
	str	r1, [fp, #-308]
	str	r0, [fp, #-312]
	bne	.L389
	ldr	r2, [r6, #404]
	mov	r3, r0
	rsb	r2, r0, r2
	cmp	r2, #0
	ble	.L389
	ldr	r1, [r8, #3184]
	mov	r0, r4
	sub	r8, fp, #248
	add	r1, r1, r3
	bl	BsInit
	ldr	r3, [fp, #-304]
	ldr	lr, .L415+8
	sub	r2, fp, #44
	sub	ip, fp, #284
	str	r8, [fp, #-172]
	add	r7, r2, r3, lsl #2
	str	r3, [fp, #-168]
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	ldr	lr, [lr]
	stmia	ip!, {r0, r1, r2, r3}
	sub	r1, fp, #184
	str	lr, [ip]
	mov	r0, r4
	ldr	r3, [r7, #-240]
	str	r3, [fp, #-176]
	bl	Real8_CB_GetPictureHeader
	cmp	r0, #1
	mov	r7, r0
	bne	.L397
	ldr	r3, [fp, #-300]
	ldrb	r3, [r3, #3196]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L273
	ldr	r9, .L415+12
	ldr	ip, [r9]
	cmp	ip, #0
	beq	.L274
	ldr	lr, [r5, #3368]
	sub	r2, fp, #284
	ldr	r1, [r5, #3364]
	mov	r3, #8
	ldr	r0, [fp, #-88]
	strh	lr, [r2]	@ movhi
	sub	lr, fp, #280
	sub	lr, lr, #2
	strh	r1, [lr]	@ movhi
	sub	lr, fp, #280
	mov	r1, #2
	strh	r0, [lr]	@ movhi
	sub	r0, fp, #276
	ldrh	lr, [fp, #-92]
	sub	r0, r0, #2
	strh	lr, [r0]	@ movhi
	ldr	r0, [r6, #936]
	blx	ip
.L274:
	ldr	r0, [r6, #936]
	bl	FSP_GetFspType
	subs	ip, r0, #0
	bne	.L275
	sub	r3, fp, #284
	sub	r2, fp, #288
	sub	r1, fp, #292
	ldr	r0, [r6, #936]
	str	ip, [fp, #-292]
	str	ip, [fp, #-288]
	str	ip, [fp, #-284]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r10]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L398
.L275:
	mov	r2, #1
	ldr	r1, [r6, #536]
	ldr	r0, [r6, #936]
	add	r9, r10, #53760
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r6, #532]
	add	r9, r9, #72
	ldr	r0, [r6, #936]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r6, #936]
	str	r3, [r6, #532]
	str	r3, [r6, #536]
	str	r3, [fp, #-304]
	bl	FSP_GetNullLogicFs
	ldr	r3, [fp, #-304]
	mov	r1, r9
	ldr	r2, [fp, #-300]
	str	r0, [r6, #536]
	str	r0, [r6, #532]
	strb	r3, [r2, #3199]
	ldr	r0, [r6, #936]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [fp, #-88]
	ldr	r3, [fp, #-92]
	cmp	r2, #512
	bhi	.L282
	cmp	r3, #512
	bhi	.L282
	ldr	r3, [r10]
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L284
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L399
.L284:
	ldr	r0, [r6, #936]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L384
.L286:
	ldr	r3, [fp, #-92]
	mov	r0, #1
	ldr	r2, [fp, #-88]
	ldr	r1, .L415+16
	bl	dprint_vfmw
	sub	r2, fp, #184
	mov	r1, r10
	mov	r0, #6
	bl	Real8_ArrangeVHBMem
	subs	r3, r0, #0
	beq	.L400
.L273:
	add	r3, r10, #52224
	sub	r1, fp, #184
	mov	r2, r3
	ldr	r3, .L415
	add	r2, r2, #200
	str	r2, [fp, #-320]
	ldr	r3, [r3, #52]
	mov	r0, r2
	mov	r2, #140
	blx	r3
	ldr	r3, [r10]
	ldr	r2, [fp, #-300]
	mov	r0, r10
	ldr	r3, [r3, #8]
	strb	r3, [r2, #3194]
	bl	Real8_GetImageBuffer
	subs	r3, r0, #0
	beq	.L401
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	bl	FSP_GetFsImagePtr
	subs	ip, r0, #0
	beq	.L289
	ldr	r2, [r10]
	add	r3, ip, #608
	add	lr, ip, #624
	ldr	r1, .L415+20
	mov	r0, #29
	ldrd	r8, [r2, #56]
	strd	r8, [r3, #-8]
	ldr	r2, [r10]
	ldrd	r8, [r2, #72]
	strd	r8, [r3]
	ldr	r2, [r10]
	ldrd	r8, [r2, #88]
	strd	r8, [lr, #-8]
	mvn	r8, #0
	ldr	r2, [r10]
	mvn	r9, #0
	ldr	r2, [r2, #104]
	str	r2, [ip, #8]
	ldr	r2, [r10]
	ldr	r2, [r2, #108]
	str	r2, [ip, #12]
	ldr	r2, [r10]
	ldr	r2, [r2, #112]
	str	r2, [ip, #16]
	ldr	r2, [r10]
	ldr	r2, [r2, #128]
	str	r2, [ip, #20]
	ldr	r2, [r10]
	strd	r8, [r2, #56]
	ldrd	r8, [r3]
	ldrd	r2, [r3, #-8]
	strd	r8, [sp]
	bl	dprint_vfmw
	ldr	r3, [r5, #3352]
	cmp	r3, #2
	beq	.L402
	cmp	r3, #1
	ldr	r3, [fp, #-300]
	ldrb	r3, [r3, #3199]	@ zero_extendqisi2
	beq	.L403
	cmp	r3, #0
	movne	r3, #2
	moveq	r3, #1
.L293:
	ldr	r2, [fp, #-300]
	strb	r3, [r2, #3199]
	ldr	r2, [r6, #532]
	ldr	r3, [r6, #536]
	cmp	r2, r3
	beq	.L294
	ldr	r1, .L415+24
	mov	r0, #13
	bl	dprint_vfmw
	mov	r2, #0
	ldr	r1, [r6, #532]
	ldr	r0, [r6, #936]
	bl	FSP_SetRef
.L294:
	ldr	r3, [r6, #536]
	mov	r2, #1
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	str	r3, [r6, #532]
	str	r1, [r6, #536]
	bl	FSP_SetRef
	ldr	r1, [r6, #532]
	ldr	r0, [r6, #936]
	bl	FSP_RemovePmv
	ldr	r2, [r6, #540]
	ldr	r1, .L415+28
	mov	r0, #13
	bl	dprint_vfmw
	ldr	r3, [fp, #-300]
	ldrb	r3, [r3, #3194]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L296
	ldr	r3, [r5, #3352]
	cmp	r3, #0
	bne	.L404
.L385:
	str	r3, [r6, #548]
.L296:
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	bl	FSP_GetLogicFs
	ldr	r1, [r6, #532]
	str	r0, [fp, #-304]
	ldr	r0, [r6, #936]
	bl	FSP_GetLogicFs
	ldr	r1, [r6, #536]
	mov	r9, r0
	ldr	r0, [r6, #936]
	bl	FSP_GetLogicFs
	ldr	ip, [fp, #-304]
	cmp	ip, #0
	cmpne	r9, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	str	r0, [fp, #-316]
	cmp	r3, #0
	bne	.L405
	ldr	r3, [ip, #640]
	cmp	r3, #0
	beq	.L303
	ldr	r2, [ip, #648]
	cmp	r2, #0
	beq	.L303
	ldr	r2, [ip, #644]
	cmp	r2, #0
	beq	.L303
	ldr	r2, [fp, #-316]
	mov	r0, #13
	ldr	r1, .L415+32
	ldr	r2, [r2, #640]
	cmp	r2, #0
	ldrne	r3, [r2, #16]
	ldreq	r3, [r3, #16]
	str	r3, [fp, #-304]
	ldr	r3, [fp, #-304]
	str	r3, [r10, #56]
	ldr	r3, [r9, #640]
	cmp	r3, #0
	ldreq	r3, [ip, #640]
	ldr	lr, [r3, #16]
	str	ip, [fp, #-328]
	str	lr, [r10, #60]
	ldr	r3, [ip, #640]
	ldr	r2, [r3, #16]
	str	r2, [r10, #64]
	ldr	r3, [ip, #652]
	str	r3, [r10, #80]
	mov	r8, r3
	ldr	r3, [fp, #-316]
	ldr	r3, [r3, #652]
	str	r3, [fp, #-324]
	str	r3, [r10, #84]
	ldr	r3, [ip, #644]
	ldr	ip, [fp, #-324]
	ldr	r3, [r3, #16]
	str	ip, [sp, #12]
	ldr	ip, [fp, #-304]
	str	r8, [sp, #8]
	str	r3, [r10, #68]
	str	ip, [sp, #4]
	str	lr, [sp]
	bl	dprint_vfmw
	ldr	ip, [fp, #-328]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #32]
	str	r3, [r10, #72]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #76]
	str	r3, [r10, #96]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #60]
	str	r3, [r10, #100]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #64]
	str	r3, [r10, #104]
	ldr	r3, [r5, #3352]
	cmp	r3, #2
	beq	.L406
.L309:
	ldr	r3, [r5, #3380]
	mov	r0, r4
	str	r3, [ip, #660]
	ldr	r3, [r5, #3352]
	ldr	r2, [r5, #3392]
	cmp	r3, #2
	strb	r3, [r10, #20]
	ldr	r3, [r5, #3368]
	str	r2, [fp, #-316]
	str	r3, [r10, #12]
	ldr	r3, [r5, #3364]
	str	r3, [r10, #16]
	ldr	r3, [r6, #456]
	str	r3, [r10, #24]
	ldr	r3, [r6, #460]
	str	r2, [r10, #32]
	str	r3, [r10, #28]
	ldr	r3, [r5, #3456]
	str	r3, [r10, #36]
	ldr	r3, [r5, #3460]
	str	r3, [r10, #40]
	strne	r2, [r5, #3456]
	bl	BsPos
	ldr	r3, [r5, #3464]
	ands	r0, r0, #-2147483641
	submi	r0, r0, #1
	mvnmi	r0, r0, asl #29
	mvnmi	r0, r0, lsr #29
	addmi	r0, r0, #1
	cmp	r3, #1
	str	r0, [r5, #3432]
	ldreq	r1, [fp, #-308]
	moveq	r0, r4
	ldreq	r3, [r6, #404]
	movne	r0, r4
	ldreq	r2, [r5, #3468]
	ldrne	r2, [r5, #3468]
	rsbeq	r3, r1, r3
	ldrne	r3, [r5, #3472]
	rsb	r3, r2, r3
	mov	r9, r3, asl #3
	bl	BsPos
	ldr	r2, [fp, #-312]
	rsb	r0, r0, r9
	str	r0, [r5, #3436]
	ldr	r3, [r6, #400]
	mov	r0, r4
	add	r9, r2, r3
	bl	BsPos
	ldr	r2, [r5, #3396]
	mov	r1, #1
	ldr	ip, [r5, #3388]
	ldr	lr, [fp, #-316]
	str	r2, [r5, #3416]
	str	ip, [r5, #3420]
	str	lr, [r5, #3412]
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	mov	r3, #0
	add	r0, r9, r0, asr #3
	str	r3, [r5, #3444]
	str	r0, [r5, #3440]
	str	r3, [r5, #3448]
	str	r3, [r5, #3452]
	str	r3, [r5, #3424]
	str	r0, [r10, #108]
	ldr	r0, [r5, #3432]
	str	r0, [r10, #116]
	ldr	r0, [r5, #3436]
	str	ip, [r10, #132]
	str	r3, [r10, #112]
	str	r0, [r10, #124]
	str	r3, [r10, #120]
	str	r3, [r10, #128]
	str	r2, [r10, #136]
	str	r3, [r10, #144]
	str	lr, [r10, #140]
	str	r1, [r10, #44]
	ldr	r2, [r5, #3464]
	cmp	r2, r1
	bls	.L337
	ldr	r9, [r5, #3472]
	movw	r0, #65535
	ldr	r2, [fp, #-308]
	movt	r0, 47
	add	r9, r2, r9
	cmp	r9, r0
	bgt	.L323
	cmp	r9, #73
	ble	.L323
	ldr	r2, [r6, #404]
	rsb	r2, r9, r2
	cmp	r2, #0
	ble	.L323
	add	ip, r10, #52480
	str	r7, [fp, #-332]
	add	ip, ip, #144
	str	r3, [fp, #-328]
	str	ip, [fp, #-304]
	mov	r7, r1
	str	r0, [fp, #-324]
	str	r1, [fp, #-312]
	ldr	r8, [fp, #-300]
	b	.L326
.L327:
	ldr	r3, [r5, #3464]
	add	r7, r7, #1
	cmp	r3, r7
	bls	.L407
	ldr	r3, [fp, #-304]
	ldr	r9, [r3, #4]!
	str	r3, [fp, #-304]
	ldr	r3, [fp, #-308]
	add	r9, r3, r9
	ldr	r3, [fp, #-324]
	cmp	r9, r3
	bgt	.L323
	cmp	r9, #73
	ble	.L323
	ldr	r2, [r6, #404]
	rsb	r2, r9, r2
	cmp	r2, #0
	ble	.L323
.L326:
	ldr	r1, [r8, #3184]
	mov	r0, r4
	add	r1, r1, r9
	bl	BsInit
	ldr	r3, [fp, #-320]
	sub	r2, fp, #292
	mov	r1, #0
	mov	r0, r4
	str	r3, [sp, #4]
	sub	r3, fp, #284
	str	r3, [sp]
	sub	r3, fp, #288
	bl	Real8_CB_GetSliceHeader
	cmp	r0, #1
	bne	.L327
	ldr	r3, [r5, #3368]
	ldr	r2, [fp, #-292]
	cmp	r3, #0
	str	r2, [fp, #-300]
	beq	.L408
	ldr	r3, [fp, #-284]
	mov	r0, r4
	ldr	r2, [fp, #-316]
	cmp	r3, #0
	movge	r2, r3
	str	r2, [fp, #-316]
	bl	BsPos
	ldr	r3, [r5, #3464]
	sub	lr, r3, #1
	cmp	lr, r7
	mov	r2, r0, asr #31
	mov	r1, r2, lsr #29
	add	r2, r0, r1
	and	r2, r2, #7
	rsb	r2, r1, r2
	str	r2, [r5, #3432]
	beq	.L409
	cmp	r7, #254
	bhi	.L410
	ldr	r2, [fp, #-304]
	mov	r0, r4
	ldr	r3, [r2, #4]
	ldr	r2, [r2]
	rsb	r3, r2, r3
.L388:
	mov	r3, r3, asl #3
	str	r3, [fp, #-336]
	bl	BsPos
	ldr	r3, [fp, #-336]
	rsb	r0, r0, r3
	str	r0, [r5, #3436]
	ldr	r2, [r6, #400]
	mov	r0, r4
	add	r9, r9, r2
	bl	BsPos
	mov	r3, #44
	ldr	ip, [fp, #-328]
	ldr	r1, [r5, #3396]
	ldr	lr, [r5, #3388]
	str	ip, [r5, #3444]
	str	r1, [r5, #3416]
	str	ip, [r5, #3448]
	str	lr, [r5, #3420]
	add	r2, r0, #7
	cmp	r0, #0
	movlt	r0, r2
	ldr	r2, [fp, #-312]
	mla	r3, r3, r2, r10
	add	r2, r9, r0, asr #3
	ldr	r0, [fp, #-312]
	str	r2, [r5, #3440]
	add	r0, r0, #1
	str	r0, [fp, #-312]
	add	r1, r3, #136
	mov	r9, r0
	mov	r0, ip
	str	r0, [r5, #3452]
	cmp	r9, #131
	ldr	r0, [fp, #-316]
	mov	ip, r9
	mov	r9, #0
	str	r0, [r5, #3412]
	ldr	r0, [fp, #-300]
	str	r0, [r5, #3424]
	add	r0, r3, #128
	str	r2, [r3, #108]
	ldr	r2, [r5, #3432]
	str	r2, [r3, #116]
	ldr	r2, [r5, #3436]
	str	r2, [r3, #124]
	ldr	r2, [r5, #3452]
	str	r2, [r3, #112]
	ldr	r2, [r5, #3444]
	str	r2, [r3, #120]
	ldr	r2, [r5, #3448]
	str	r2, [r3, #128]
	ldr	r3, [r5, #3420]
	str	r3, [r0, #4]
	ldr	r3, [r5, #3416]
	str	r3, [r0, #8]
	ldr	r3, [r5, #3412]
	str	r3, [r1, #4]
	ldr	r3, [r5, #3424]
	str	r3, [r1, #8]
	str	ip, [r10, #44]
	ble	.L327
	mov	r0, r10
	bl	Real8_ModifySliceMsg
	cmp	r0, #1
	bne	.L411
	ldr	r3, [fp, #-296]
	cmp	r3, #0
	str	r3, [r10, #4]
	beq	.L412
	ldr	r3, [r10]
	ldr	r2, [r3, #864]
	cmp	r2, #1
	beq	.L413
	cmp	r2, #2
	bne	.L327
	ldr	r3, [r3, #868]
	cmp	r3, #0
	ble	.L327
	ldrb	r3, [r10, #20]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L327
	str	r9, [r10, #4]
	mov	r2, r9
	ldr	r1, [r6, #540]
	mov	r7, r9
	ldr	r0, [r6, #936]
	bl	FSP_SetDisplay
	ldr	r2, [r10]
	mov	r1, r10
	mov	r0, #8
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	bl	ReleasePacket
	b	.L378
.L397:
	ldr	r1, .L415+36
	mov	r0, #1
	mov	r7, r9
	bl	dprint_vfmw
	b	.L378
.L282:
	ldr	r1, .L415+40
	mov	r0, #18
	bl	dprint_vfmw
	b	.L389
.L401:
	ldr	r2, [r6, #432]
	mov	r7, r3
	ldr	r1, .L415+44
	bl	dprint_vfmw
	b	.L378
.L404:
	cmp	r3, #2
	beq	.L339
	cmp	r3, #1
	beq	.L385
	b	.L296
.L403:
	cmp	r3, #0
	movne	r3, #2
	bne	.L293
.L323:
	mov	r2, #1
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	mov	r7, #0
	bl	FSP_ClearLogicFs
	b	.L378
.L405:
	movw	r3, #2023
	ldr	r2, .L415+48
	ldr	r1, .L415+52
	mov	r0, #22
	bl	dprint_vfmw
	mov	r7, #0
	b	.L378
.L402:
	ldr	r3, [fp, #-300]
	ldrb	r3, [r3, #3199]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L323
	ldr	r3, [fp, #-300]
	ldrb	r3, [r3, #3194]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L296
.L339:
	ldr	r3, [r6, #548]
	cmp	r3, #0
	bne	.L296
	mov	r2, #1
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	mov	r7, r3
	bl	FSP_ClearLogicFs
	b	.L378
.L399:
	ldr	r3, [fp, #-300]
	ldrb	r3, [r3, #3196]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L286
.L384:
	mov	r0, r9
	bl	ResetVoQueue
	b	.L286
.L400:
	ldr	r1, .L415+56
	mov	r0, #1
	mov	r7, r3
	bl	dprint_vfmw
	b	.L378
.L406:
	ldr	r1, [fp, #-316]
	ldr	r3, [r9, #660]
	ldr	r2, [r5, #3380]
	ldr	r9, [r1, #660]
	subs	r2, r2, r3
	rsb	r9, r3, r9
	ldrmi	r3, [r5, #3376]
	addmi	r2, r2, r3
	cmp	r9, #0
	ldrlt	r3, [r5, #3376]
	addlt	r9, r9, r3
	cmp	r2, r9
	movgt	r2, #0
	cmp	r9, #0
	ble	.L342
	mov	r1, r9
	mov	r0, r2, asl #14
	str	ip, [fp, #-324]
	str	r2, [fp, #-304]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-304]
	mov	r1, r9
	str	r0, [fp, #-316]
	rsb	r0, r2, r9
	mov	r0, r0, asl #14
	bl	__aeabi_idiv
	ldr	r2, [fp, #-304]
	ldr	r3, [fp, #-316]
	ldr	ip, [fp, #-324]
.L313:
	str	r2, [r6, #448]
	str	r9, [r6, #452]
	str	r3, [r6, #456]
	str	r0, [r6, #460]
	b	.L309
.L303:
	ldr	r1, [r6, #540]
	mov	r2, #1
	ldr	r0, [r6, #936]
	mov	r7, #0
	bl	FSP_ClearLogicFs
	ldr	r1, .L415+60
	mov	r0, r7
	bl	dprint_vfmw
	b	.L378
.L398:
	ldr	r3, [fp, #-284]
	cmp	r3, #0
	bgt	.L278
	ldr	r3, [fp, #-288]
	cmp	r3, #32
	bgt	.L278
	ldr	r3, [r10, #4]
	cmp	r3, #0
	bne	.L278
	ldr	r3, [r6, #428]
	cmp	r3, #0
	beq	.L275
	ldr	lr, [r5, #3368]
	cmp	lr, #0
	beq	.L275
	ldr	r0, [fp, #-92]
	ldr	r1, [r6, #940]
	cmp	r0, r1
	beq	.L275
	ldr	r1, [fp, #-88]
	ldr	ip, [r6, #944]
	cmp	r1, ip
	beq	.L275
	ldr	ip, [r5, #3364]
	str	r0, [sp, #12]
	mov	r0, r2
	str	r1, [sp, #8]
	str	ip, [sp, #4]
	str	lr, [sp]
	ldr	r1, .L415+64
	bl	dprint_vfmw
	ldr	ip, [r9]
	ldr	r2, [fp, #-92]
	ldr	r3, [fp, #-88]
	cmp	ip, #0
	str	r2, [r6, #940]
	str	r3, [r6, #944]
	beq	.L275
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r6, #936]
	blx	ip
	b	.L275
.L409:
	add	r3, r3, #10304
	ldr	r2, [r6, #404]
	add	r3, r3, #5
	ldr	r1, [fp, #-308]
	mov	r0, r4
	add	r3, r8, r3, lsl #2
	rsb	r2, r1, r2
	ldr	r3, [r3, #3188]
	rsb	r3, r3, r2
	b	.L388
.L413:
	ldrb	r3, [r10, #20]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L327
	str	r9, [r10, #4]
	mov	r2, r9
	ldr	r1, [r6, #540]
	mov	r7, r9
	ldr	r0, [r6, #936]
	bl	FSP_SetDisplay
	mov	r1, r10
	mov	r0, #8
	bl	ReleasePacket
	b	.L378
.L412:
	movw	r3, #2223
	ldr	r2, .L415+48
	ldr	r1, .L415+68
	mov	r0, #22
	ldr	r7, [fp, #-296]
	bl	dprint_vfmw
	b	.L378
.L278:
	mvn	r7, #1
	b	.L378
.L408:
	mov	r2, r0
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	mov	r7, r3
	bl	FSP_ClearLogicFs
	b	.L378
.L411:
	mov	r2, #1
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	mov	r7, r9
	bl	FSP_ClearLogicFs
	b	.L378
.L410:
	movw	r2, #2190
	ldr	r1, .L415+72
	mov	r0, #22
	mov	r7, #0
	bl	dprint_vfmw
	b	.L378
.L342:
	mov	r0, #0
	mov	r3, r0
	b	.L313
.L289:
	mov	r2, #1
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	mov	r7, ip
	bl	FSP_ClearLogicFs
	b	.L378
.L407:
	ldr	r7, [fp, #-332]
.L337:
	ldr	r4, [r10, #4]
	mov	r5, #1
	str	r5, [r10, #48]
	cmp	r4, #0
	beq	.L414
.L320:
	ldr	r3, [r10]
	ldr	r2, [r3, #592]
	str	r2, [r10, #88]
	ldr	r3, [r3, #632]
	str	r3, [r10, #92]
	b	.L378
.L414:
	mov	r0, r10
	bl	Real8_ModifySliceMsg
	cmp	r0, #1
	ldreq	r3, [fp, #-296]
	streq	r3, [r10, #4]
	beq	.L320
	mov	r2, r5
	ldr	r1, [r6, #540]
	ldr	r0, [r6, #936]
	mov	r7, r4
	bl	FSP_ClearLogicFs
	b	.L378
.L416:
	.align	2
.L415:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC13
	.word	.LANCHOR0+460
	.word	g_event_report
	.word	.LC17
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC25
	.word	.LC14
	.word	.LC16
	.word	.LC19
	.word	.LANCHOR0+528
	.word	.LC23
	.word	.LC18
	.word	.LC24
	.word	.LC15
	.word	.LC27
	.word	.LC26
	UNWIND(.fnend)
	.size	REAL8DEC_Decode, .-REAL8DEC_Decode
	.align	2
	.global	REAL8DEC_RecycleImage
	.type	REAL8DEC_RecycleImage, %function
REAL8DEC_RecycleImage:
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
	cmp	r0, #0
	mov	r5, r1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r4, r0, #53248
	mov	r2, #0
	ldr	r0, [r4, #936]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #936]
	bl	FSP_GetFsImagePtr
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL8DEC_RecycleImage, .-REAL8DEC_RecycleImage
	.align	2
	.global	REAL8DEC_GetRemainImg
	.type	REAL8DEC_GetRemainImg, %function
REAL8DEC_GetRemainImg:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r3, [r0]
	mov	r4, r0
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L435
	add	r6, r0, #53248
	ldr	r1, [r6, #536]
	ldr	r0, [r6, #936]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L435
	ldr	r1, [r6, #536]
	ldr	r0, [r6, #936]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L434
.L435:
	add	r5, r4, #53760
	add	r5, r5, #72
.L425:
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	GetVoLastImageID
.L434:
	mov	r0, r4
	add	r5, r4, #53760
	bl	Real8_SetImgFormat
	add	r5, r5, #72
	mov	r3, #1
	str	r3, [r7, #188]
	mov	r2, r3
	ldr	r1, [r6, #536]
	ldr	r0, [r6, #936]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #936]
	mov	r2, r4
	str	r7, [sp]
	mov	r3, r5
	mov	r1, #8
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L422
	mov	r2, #0
	ldr	r1, [r6, #536]
	ldr	r0, [r6, #936]
	bl	FSP_SetDisplay
	b	.L425
.L422:
	ldr	r3, [r7, #160]
	cmp	r3, #100
	bhi	.L425
	ldr	r3, [r7, #112]
	cmp	r3, #31
	bls	.L425
	ldr	r3, [r7, #116]
	cmp	r3, #31
	bls	.L425
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL8DEC_GetRemainImg, .-REAL8DEC_GetRemainImg
	.align	2
	.global	REAL8DEC_VDMPostProc
	.type	REAL8DEC_VDMPostProc, %function
REAL8DEC_VDMPostProc:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	mov	r6, r1
	mov	r5, r0
	ldr	r1, .L465
	add	r4, r5, #53248
	mov	r8, r2
	mov	r0, #22
	mov	r2, r6
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, #8
	add	r7, r5, #8192
	bl	ReleasePacket
	ldr	r1, [r4, #540]
	ldr	r0, [r4, #936]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L437
	add	r9, r5, #49152
	ldr	r3, [r9, #3352]
	cmp	r3, #2
	andne	r8, r8, #31
	strb	r3, [r7, #3197]
	strne	r8, [r9, #3460]
	cmp	r6, #0
	movlt	r6, #0
	blt	.L439
	cmp	r6, #100
	movge	r6, #100
.L439:
	str	r6, [r0, #160]
	mov	r0, #22
	ldr	r1, .L465+4
	bl	dprint_vfmw
	mov	r0, r5
	bl	Real8_SetImgFormat
	ldrb	r3, [r7, #3194]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L442
	ldr	r3, [r5]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L442
	ldr	r3, [r9, #3352]
	cmp	r3, #2
	beq	.L442
	ldrb	r3, [r7, #3199]	@ zero_extendqisi2
	cmp	r3, #1
	ldrhi	r1, [r4, #532]
	mvnls	r1, #0
	cmn	r1, #1
	str	r1, [r4, #544]
	beq	.L444
.L464:
	ldr	r0, [r4, #936]
	bl	FSP_GetFsImagePtr
	ldr	r6, [r4, #544]
	mov	r1, r6
	mov	r7, r0
	ldr	r0, [r4, #936]
	bl	FSP_GetRef
	mov	r2, r6
	ldr	r1, .L465+8
	mov	r3, r0
	mov	r0, #12
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r4, #544]
	ldr	r0, [r4, #936]
	bl	FSP_SetDisplay
	add	r3, r5, #53760
	ldr	r0, [r4, #936]
	add	r3, r3, #72
	str	r7, [sp]
	mov	r2, r5
	mov	r1, #8
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L463
.L444:
	ldr	r3, [r4, #432]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r4, #432]
.L437:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L442:
	ldr	r1, [r4, #540]
	cmn	r1, #1
	str	r1, [r4, #544]
	bne	.L464
	b	.L444
.L463:
	ldr	r0, [r4, #936]
	mov	r2, #0
	ldr	r1, [r4, #544]
	bl	FSP_SetDisplay
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L466:
	.align	2
.L465:
	.word	.LC28
	.word	.LC29
	.word	.LC30
	UNWIND(.fnend)
	.size	REAL8DEC_VDMPostProc, .-REAL8DEC_VDMPostProc
	.align	2
	.global	REAL8DEC_GetImageBuffer
	.type	REAL8DEC_GetImageBuffer, %function
REAL8DEC_GetImageBuffer:
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
	add	r4, r0, #53248
	mov	r5, r0
	ldr	r0, [r4, #936]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #936]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L473
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L473:
	add	r1, r5, #53760
	ldr	r0, [r4, #936]
	add	r1, r1, #72
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL8DEC_GetImageBuffer, .-REAL8DEC_GetImageBuffer
	.align	2
	.global	REAL8DEC_Support
	.type	REAL8DEC_Support, %function
REAL8DEC_Support:
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
	mov	r3, #1
	str	r3, [r0]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL8DEC_Support, .-REAL8DEC_Support
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	gs_VLCDecodeTable, %object
	.size	gs_VLCDecodeTable, 256
gs_VLCDecodeTable:
	.byte	-128
	.byte	-127
	.byte	112
	.byte	112
	.byte	-126
	.byte	-125
	.byte	113
	.byte	113
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	-124
	.byte	-123
	.byte	114
	.byte	114
	.byte	-122
	.byte	-121
	.byte	115
	.byte	115
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	81
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	48
	.byte	-120
	.byte	-119
	.byte	116
	.byte	116
	.byte	-118
	.byte	-117
	.byte	117
	.byte	117
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	82
	.byte	-116
	.byte	-115
	.byte	118
	.byte	118
	.byte	-114
	.byte	-113
	.byte	119
	.byte	119
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	49
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.type	MBA_FieldWidth, %object
	.size	MBA_FieldWidth, 24
MBA_FieldWidth:
	.word	6
	.word	7
	.word	9
	.word	11
	.word	13
	.word	14
.LC0:
	.word	160
	.word	176
	.word	240
	.word	320
	.word	352
	.word	640
	.word	704
	.word	0
.LC1:
	.word	120
	.word	132
	.word	144
	.word	240
	.word	288
	.word	480
	.word	0
	.word	0
.LC2:
	.word	180
	.word	360
	.word	576
	.word	0
	.type	GetBitsMask, %object
	.size	GetBitsMask, 100
GetBitsMask:
	.word	0
	.word	1
	.word	3
	.word	7
	.word	15
	.word	31
	.word	63
	.word	127
	.word	255
	.word	511
	.word	1023
	.word	2047
	.word	4095
	.word	8191
	.word	16383
	.word	32767
	.word	65535
	.word	131071
	.word	262143
	.word	524287
	.word	1048575
	.word	2097151
	.word	4194303
	.word	8388607
	.word	16777215
.LC3:
	.word	0
	.word	1
	.word	1
	.word	2
	.word	2
	.word	3
	.word	3
	.word	3
	.word	3
	.type	s_RVVersionEncodingToMinorVersion, %object
	.size	s_RVVersionEncodingToMinorVersion, 32
s_RVVersionEncodingToMinorVersion:
	.word	2
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.word	9999
	.type	__FUNCTION__.14538, %object
	.size	__FUNCTION__.14538, 16
__FUNCTION__.14538:
	.ascii	"REAL8DEC_Decode\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
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
	ASCII(.ascii	"image size abnormal (%dx%d) in MB\012\000" )
	.space	1
.LC9:
	ASCII(.ascii	"real8 actual frame size(%dx%d) exeed max config(%dx" )
	ASCII(.ascii	"%d)\012\000" )
.LC10:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC12:
	ASCII(.ascii	"Decoding quits at frm %d\012\000" )
	.space	2
.LC13:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet pointer!\012\000" )
.LC14:
	ASCII(.ascii	"Real8_CB_GetPictureHeader Error!\012\000" )
	.space	2
.LC15:
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC16:
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC17:
	ASCII(.ascii	"new seq size %d * %d\012\000" )
	.space	2
.LC18:
	ASCII(.ascii	"Real8_ArrangeVHBMem err\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"Fatal Error: frm%d, get image buffer failed\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC21:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC22:
	ASCII(.ascii	"++++++++++ ref %d\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"%s %d unknow error!!\012\000" )
	.space	2
.LC24:
	ASCII(.ascii	"CurFsID abnormal!\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,bwd,currpmv,colpmv = %p,%p," )
	ASCII(.ascii	"%p,%p,%p,%p\012\000" )
.LC26:
	ASCII(.ascii	"line:%d! i > (RV8_MAX_SLC_PARAM_NUM - 2)\012\000" )
	.space	2
.LC27:
	ASCII(.ascii	"%s %d NULL == pCtx->pRv8DecParam!!\012\000" )
.LC28:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC29:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC30:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
