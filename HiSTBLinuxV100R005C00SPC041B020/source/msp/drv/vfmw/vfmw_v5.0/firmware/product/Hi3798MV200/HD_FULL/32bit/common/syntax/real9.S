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
	.file	"real9.c"
	.text
	.align	2
	.global	Real9_CB_GetBits
	.type	Real9_CB_GetBits, %function
Real9_CB_GetBits:
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
	.size	Real9_CB_GetBits, .-Real9_CB_GetBits
	.align	2
	.global	Real9_CB_Get1Bit
	.type	Real9_CB_Get1Bit, %function
Real9_CB_Get1Bit:
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
	.size	Real9_CB_Get1Bit, .-Real9_CB_Get1Bit
	.align	2
	.global	Real9_CB_GetVLCBits
	.type	Real9_CB_GetVLCBits, %function
Real9_CB_GetVLCBits:
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
	.size	Real9_CB_GetVLCBits, .-Real9_CB_GetVLCBits
	.align	2
	.global	Real9_GetImageBuffer
	.type	Real9_GetImageBuffer, %function
Real9_GetImageBuffer:
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
	ldr	r0, [r4, #920]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #520]
	blt	.L17
	mov	r1, r0
	ldr	r0, [r4, #920]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L18
	ldr	r2, [r4, #520]
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
	ldr	r1, [r4, #516]
	ldr	r0, [r4, #920]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #512]
	mov	r5, #0
	ldr	r0, [r4, #920]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #512]
	str	r5, [r4, #516]
	ldr	r0, [r4, #920]
	bl	FSP_GetNullLogicFs
	add	r1, r6, #53760
	add	r1, r1, #56
	str	r0, [r4, #516]
	str	r0, [r4, #512]
	strb	r5, [r7, #3189]
	ldr	r0, [r4, #920]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L18:
	mov	r2, #516
	ldr	r1, .L19+12
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r5
	b	.L12
.L20:
	.align	2
.L19:
	.word	.LC5
	.word	.LC6
	.word	.LC3
	.word	.LC4
	UNWIND(.fnend)
	.size	Real9_GetImageBuffer, .-Real9_GetImageBuffer
	.align	2
	.global	Real9_ArrangeVHBMem
	.type	Real9_ArrangeVHBMem, %function
Real9_ArrangeVHBMem:
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
	ldr	r2, [r2, #24]
	mov	r5, r1
	ldr	r3, [r4, #20]
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
	ldr	r9, [r4, #16]
	mov	r7, #5
	ldr	r10, [r4, #12]
	mov	r6, #2
	mov	r8, #4
.L32:
	ldr	r3, .L56
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r0, [r4, #12]
	ldr	r1, [r5]
	mov	r3, #0
	ldr	r2, [r4, #16]
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
	ldr	r0, [r6, #920]
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
	ldr	r2, [r4, #16]
	ldr	r0, [r1, #668]
	cmp	r2, r0
	bhi	.L52
	ldr	r3, [r4, #12]
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
	ldr	r0, [r6, #920]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L54
	cmp	r0, #0
	addeq	r5, r5, #8192
	moveq	r3, #1
	moveq	r0, r3
	streqb	r3, [r5, #3192]
	bne	.L40
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L38:
	mov	r3, #1
	strb	r7, [fp, #-71]
	strb	r3, [fp, #-73]
	b	.L42
.L50:
	ldr	r2, [r4, #16]
	ldr	r9, [r1, #668]
	cmp	r2, r9
	bhi	.L55
	ldr	r3, [r4, #12]
	ldr	r10, [r1, #672]
	cmp	r3, r10
	bhi	.L29
	add	r3, r5, #8192
	ldrb	r0, [r3, #3192]	@ zero_extendqisi2
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
	ldr	r3, [r4, #12]
.L35:
	stmia	sp, {r0, ip}
	mov	r0, #0
	ldr	r1, .L56+8
	bl	dprint_vfmw
	mov	r0, #0
	b	.L47
.L55:
	ldr	r10, [r1, #672]
	ldr	r3, [r4, #12]
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
	add	r0, r0, #56
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
	.word	.LC7
	.word	.LC8
	.word	.LC9
	UNWIND(.fnend)
	.size	Real9_ArrangeVHBMem, .-Real9_ArrangeVHBMem
	.align	2
	.global	Real9_CB_GetCPFMT
	.type	Real9_CB_GetCPFMT, %function
Real9_CB_GetCPFMT:
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
	str	r4, [r6, #16]
	bl	Real9_CB_Get1Bit
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
	str	r7, [r6, #12]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real9_CB_GetCPFMT, .-Real9_CB_GetCPFMT
	.align	2
	.global	Real9_CB_SetDimensions
	.type	Real9_CB_SetDimensions, %function
Real9_CB_SetDimensions:
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
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #12]
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
	str	r3, [r0, #76]
	mul	r3, r3, r2
	str	r2, [r0, #72]
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
	str	r3, [r0, #80]
	ldmfd	sp, {fp, sp, pc}
.L83:
	cmp	r2, #1152
	bne	.L64
	mov	r2, #88
	mov	r3, #72
	str	r2, [r0, #76]
	str	r3, [r0, #72]
.L78:
	mov	r2, #4
	b	.L65
.L79:
	cmp	r2, #96
	bne	.L64
	mov	r2, #8
	mov	r3, #6
	str	r2, [r0, #76]
	mov	r2, #0
	str	r3, [r0, #72]
	b	.L65
.L80:
	cmp	r2, #144
	bne	.L64
	mov	r2, #11
	mov	r3, #9
	str	r2, [r0, #76]
	str	r3, [r0, #72]
.L74:
	mov	r2, #1
	b	.L65
.L81:
	cmp	r2, #288
	bne	.L64
	mov	r2, #22
	mov	r3, #18
	str	r2, [r0, #76]
	str	r3, [r0, #72]
.L75:
	mov	r2, #2
	b	.L65
.L82:
	cmp	r2, #576
	bne	.L64
	mov	r2, #44
	mov	r3, #36
	str	r2, [r0, #76]
	str	r3, [r0, #72]
.L76:
	mov	r2, #3
	b	.L65
.L85:
	.align	2
.L84:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	Real9_CB_SetDimensions, .-Real9_CB_SetDimensions
	.align	2
	.global	Real9_CB_GetPicSize
	.type	Real9_CB_GetPicSize, %function
Real9_CB_GetPicSize:
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
	ldr	ip, [r9, #20]
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
	bl	Real9_CB_Get1Bit
	cmp	r0, #0
	beq	.L87
	ldr	r3, [r9, #52]
	str	r3, [r8]
	ldr	r3, [r9, #56]
	str	r3, [r7]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L101:
	mov	r0, r4
	mov	r9, r9, asl #1
	bl	Real9_CB_Get1Bit
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
	.size	Real9_CB_GetPicSize, .-Real9_CB_GetPicSize
	.align	2
	.global	Real9_CB_FindNextSliceStartCode
	.type	Real9_CB_FindNextSliceStartCode, %function
Real9_CB_FindNextSliceStartCode:
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
	movwgt	r7, #7184
	movgt	r4, #0
	movtgt	r7, 29
	bgt	.L107
	b	.L108
.L111:
	bl	BsSkip
	cmp	r4, r6
	beq	.L108
.L107:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	cmp	r0, r7
	mov	r0, r5
	bne	.L111
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L108:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real9_CB_FindNextSliceStartCode, .-Real9_CB_FindNextSliceStartCode
	.align	2
	.global	Real9_CB_SearchBits_IgnoreOnly0
	.type	Real9_CB_SearchBits_IgnoreOnly0, %function
Real9_CB_SearchBits_IgnoreOnly0:
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
	beq	.L113
	ands	r5, r4, r6
	bne	.L113
	ldr	r3, .L119
	add	r10, r3, r10, lsl #2
	ldr	r10, [r10, #360]
	b	.L114
.L115:
	tst	r4, r6
	bne	.L113
.L114:
	mov	r0, r8
	and	r4, r10, r4, asl #1
	bl	Real9_CB_Get1Bit
	add	r5, r5, #1
	orr	r4, r0, r4
	cmp	r7, r4
	cmpne	r9, r5
	bhi	.L115
.L113:
	rsb	r0, r7, r4
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L120:
	.align	2
.L119:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	Real9_CB_SearchBits_IgnoreOnly0, .-Real9_CB_SearchBits_IgnoreOnly0
	.align	2
	.global	Real9_CB_GetSSC
	.type	Real9_CB_GetSSC, %function
Real9_CB_GetSSC:
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
	ldr	r3, [r1, #4]
	cmp	r3, #1
	beq	.L123
	bl	Real9_CB_FindNextSliceStartCode
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L123:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Real9_CB_GetSSC, .-Real9_CB_GetSSC
	.align	2
	.global	Real9_CB_GetSliceHeader
	.type	Real9_CB_GetSliceHeader, %function
Real9_CB_GetSliceHeader:
	UNWIND(.fnstart)
	@ args = 8, pretend = 0, frame = 8
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
	ldr	r4, [fp, #8]
	mov	r5, r1
	mov	r7, r2
	mov	r8, r3
	mov	r9, r0
	ldr	r6, [r4, #4]
	cmp	r6, #1
	beq	.L160
	cmp	r1, #0
	beq	.L161
	mov	r2, #0
	str	r2, [r7]
	ldr	r2, [fp, #4]
	mov	r0, #1
	mvn	r3, #0
	str	r3, [r2]
.L149:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L160:
	cmp	r1, #0
	bne	.L126
	ldr	r3, [r4, #88]
	cmn	r3, #1
	beq	.L162
	bl	Real9_CB_Get1Bit
	cmp	r0, #0
	bne	.L139
	mov	r1, #2
	mov	r0, r9
	bl	BsShow
	mov	r1, #2
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	ldr	r3, [r4, #84]
	cmp	r3, r6
	beq	.L129
.L139:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L161:
	bl	Real9_CB_Get1Bit
	cmp	r0, #1
	bne	.L139
	ldr	r5, [r4, #80]
	mov	r0, r9
	mov	r1, r5
	bl	BsShow
	mov	r1, r5
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	str	r6, [r7]
	ldr	r3, [r4, #80]
	cmp	r3, #11
	bls	.L144
	mov	r0, r9
	bl	Real9_CB_Get1Bit
	cmp	r0, #1
	bne	.L139
.L144:
	mov	r1, #5
	mov	r0, r9
	bl	BsShow
	mov	r1, #5
	mov	r5, r0
	mov	r0, r9
	bl	BsSkip
	ldr	r3, [fp, #4]
	cmp	r5, #31
	str	r5, [r3]
	bgt	.L139
	mov	r1, #2
	mov	r0, r9
	bl	BsShow
	mov	r1, #2
	mov	r5, r0
	mov	r0, r9
	bl	BsSkip
	str	r5, [r4, #52]
	mov	r0, r9
	bl	Real9_CB_Get1Bit
	cmp	r0, #1
	mov	r4, r0
	bne	.L139
	mov	r1, #2
	mov	r0, r9
	bl	BsShow
	mov	r1, #2
	mov	r5, r0
	mov	r0, r9
	bl	BsSkip
	str	r5, [r8]
	mov	r0, r4
	b	.L149
.L162:
	bl	Real9_CB_Get1Bit
	cmp	r0, #0
	bne	.L139
	mov	r1, #2
	mov	r0, r9
	bl	BsShow
	mov	r5, r6
	mov	r1, #2
	mov	r10, r0
	mov	r0, r9
	bl	BsSkip
	str	r10, [r4, #84]
.L129:
	mov	r1, #5
	mov	r0, r9
	bl	BsShow
	mov	r1, #5
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	uxtb	r0, r6
	cmp	r0, #31
	strb	r0, [r4, #66]
	bhi	.L139
	mov	r0, r9
	bl	Real9_CB_Get1Bit
	cmp	r0, #0
	bne	.L139
	mov	r0, r9
	bl	Real9_CB_Get1Bit
	subs	r10, r0, #0
	bne	.L139
	mov	r1, #2
	mov	r0, r9
	bl	BsShow
	mov	r1, #2
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	str	r6, [r4, #52]
	mov	r1, #1
	mov	r0, r9
	bl	BsShow
	mov	r1, #1
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	adds	r6, r6, #0
	mov	r1, #13
	mov	r0, r9
	movne	r6, #1
	cmp	r5, #0
	strb	r6, [r4, #65]
	add	r5, r4, #64
	beq	.L132
	bl	BsShow
	mov	r1, #13
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	ldr	r3, [r4, #84]
	str	r6, [r4, #92]
	cmp	r3, #1
	bls	.L134
	cmp	r3, #2
	ldreq	r2, [r4, #108]
	ldreq	r3, [r4, #112]
	streq	r2, [r4, #116]
	streq	r3, [r4, #120]
.L150:
	mov	r1, r5
	sub	r3, fp, #48
	sub	r2, fp, #52
	mov	r0, r9
	bl	Real9_CB_GetPicSize
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-48]
	movw	r1, #8144
	sub	r0, r2, #48
	cmp	r0, r1
	str	r2, [r4, #108]
	str	r2, [r4, #16]
	str	r3, [r4, #112]
	str	r3, [r4, #12]
	bhi	.L139
	sub	r3, r3, #48
	cmp	r3, r1
	bhi	.L139
	mov	r0, r4
	bl	Real9_CB_SetDimensions
.L140:
	ldr	r5, [r4, #80]
	mov	r0, r9
	mov	r1, r5
	bl	BsShow
	mov	r1, r5
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	str	r6, [r4, #88]
.L126:
	ldr	r2, [r4, #88]
	mov	r3, #0
	ldr	r1, [fp, #4]
	mov	r0, #1
	str	r2, [r7]
	ldrb	r2, [r4, #66]	@ zero_extendqisi2
	str	r2, [r1]
	str	r3, [r8]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L132:
	bl	BsShow
	mov	r1, #13
	mov	r6, r0
	mov	r0, r9
	bl	BsSkip
	ldr	r3, [r4, #92]
	cmp	r3, r6
	bne	.L139
	sub	r3, fp, #48
	sub	r2, fp, #52
	mov	r1, r5
	mov	r0, r9
	bl	Real9_CB_GetPicSize
	ldr	r2, [r4, #108]
	ldr	r3, [fp, #-52]
	cmp	r2, r3
	bne	.L139
	ldr	r2, [r4, #112]
	ldr	r3, [fp, #-48]
	cmp	r2, r3
	bne	.L139
	b	.L140
.L134:
	str	r10, [r4, #116]
	str	r10, [r4, #120]
	b	.L150
	UNWIND(.fnend)
	.size	Real9_CB_GetSliceHeader, .-Real9_CB_GetSliceHeader
	.align	2
	.global	Real9_CB_GetPictureHeader
	.type	Real9_CB_GetPictureHeader, %function
Real9_CB_GetPictureHeader:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	ldr	r3, [r1, #4]
	mov	r4, r1
	mov	r5, r0
	cmp	r3, #1
	beq	.L187
	ldr	r3, .L190
	mov	r2, #128
	mov	r1, #0
	mov	r0, r4
	ldr	r3, [r3, #48]
	blx	r3
	mov	r1, #16
	mov	r0, r5
	bl	BsShow
	mov	r1, #16
	mov	r6, r0
	mov	r0, r5
	bl	BsSkip
	movw	r3, #21845
	cmp	r6, r3
	bne	.L183
	mov	r1, #16
	mov	r0, r5
	bl	BsShow
	mov	r1, #16
	mov	r7, r0
	mov	r0, r5
	bl	BsSkip
	cmp	r7, r6
	bne	.L183
	mov	r0, r5
	sub	r1, fp, #32
	bl	Real9_CB_GetVLCBits
	cmp	r0, #30
	bls	.L183
	ldr	r3, [fp, #-32]
	ands	r2, r3, #1
	bne	.L183
	tst	r3, #2
	mov	r0, r5
	strne	r2, [r4, #16]
	moveq	r1, #176
	moveq	r2, #144
	streq	r1, [r4, #16]
	sub	r1, fp, #32
	str	r2, [r4, #12]
	ubfx	r2, r3, #2, #5
	ubfx	r3, r3, #7, #8
	str	r2, [r4, #48]
	str	r3, [r4, #36]
	bl	Real9_CB_GetVLCBits
	cmp	r0, #1
	streq	r0, [r4, #8]
	beq	.L177
	cmp	r0, #3
	beq	.L188
	cmp	r0, #5
	bne	.L183
	ldr	r3, [fp, #-32]
	cmp	r3, #0
	bne	.L183
	mov	r3, #2
	str	r3, [r4, #8]
.L177:
	ldr	r3, [r4, #16]
	cmp	r3, #0
	beq	.L189
.L179:
	sub	r3, r3, #48
	movw	r2, #8144
	cmp	r3, r2
	bhi	.L183
	ldr	r3, [r4, #12]
	sub	r3, r3, #48
	cmp	r3, r2
	bhi	.L183
	mov	r0, r4
	mov	r6, #1
	bl	Real9_CB_SetDimensions
	mov	r1, #2
	mov	r0, r5
	bl	BsShow
	mov	r1, #2
	mov	r7, r0
	mov	r0, r5
	bl	BsSkip
	str	r7, [r4, #52]
.L165:
	ldr	r3, [r4, #16]
	mov	r0, r6
	ldr	r2, [r4, #12]
	add	r3, r3, #15
	add	r2, r2, #15
	mov	r3, r3, lsr #4
	str	r3, [r4, #24]
	mov	r2, r2, lsr #4
	str	r2, [r4, #20]
	mul	r3, r3, r2
	str	r3, [r4, #28]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L188:
	ldr	r3, [fp, #-32]
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [r4, #8]
	beq	.L177
.L183:
	mvn	r0, #1
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L187:
	str	r1, [sp, #4]
	mvn	r2, #0
	sub	r3, fp, #32
	str	r2, [r1, #88]
	str	r3, [sp]
	sub	r2, fp, #36
	sub	r3, fp, #40
	mov	r1, #0
	bl	Real9_CB_GetSliceHeader
	cmp	r0, #1
	mvnne	r6, #1
	bne	.L165
	ldr	r3, [r4, #84]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L181
.L168:
	.word	.L167
	.word	.L169
	.word	.L170
	.word	.L171
.L189:
	mov	r1, r4
	mov	r0, r5
	bl	Real9_CB_GetCPFMT
	cmp	r0, #0
	beq	.L183
	ldr	r3, [r4, #16]
	b	.L179
.L171:
	mov	r6, #1
	mov	r3, #2
	str	r3, [r4, #8]
.L166:
	ldr	r3, [r4, #16]
	movw	r2, #8144
	ldrb	lr, [r4, #65]	@ zero_extendqisi2
	sub	r3, r3, #48
	ldrb	ip, [r4, #66]	@ zero_extendqisi2
	ldr	r0, [r4, #92]
	cmp	r3, r2
	ldr	r1, [r4, #96]
	ldr	r3, [r4, #100]
	str	lr, [r4, #44]
	str	ip, [r4, #48]
	str	r0, [r4, #36]
	str	r1, [r4, #56]
	str	r3, [r4, #60]
	bhi	.L183
	ldr	r3, [r4, #12]
	sub	r3, r3, #48
	cmp	r3, r2
	bhi	.L183
	mov	r0, r4
	bl	Real9_CB_SetDimensions
	b	.L165
.L170:
	mov	r0, #1
	str	r0, [r4, #8]
	mov	r6, r0
	b	.L166
.L169:
	mov	r3, #0
	mov	r6, #1
	str	r3, [r4, #8]
	str	r3, [r4, #68]
	b	.L166
.L167:
	mov	r0, #1
	mov	r3, #0
	str	r0, [r4, #68]
	mov	r6, r0
	str	r3, [r4, #8]
	b	.L166
.L181:
	mvn	r6, #1
	b	.L166
.L191:
	.align	2
.L190:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	Real9_CB_GetPictureHeader, .-Real9_CB_GetPictureHeader
	.align	2
	.global	Real9_SetImgFormat
	.type	Real9_SetImgFormat, %function
Real9_SetImgFormat:
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
	ldr	r1, [r3, #520]
	ldr	r0, [r3, #920]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L192
	add	ip, r6, #49152
	ldr	r2, [ip, #3272]
	and	r2, r2, #3
	orr	r3, r2, #7168
	orr	r3, r3, #32
	str	r3, [r4, #108]
	ldr	r1, [ip, #3280]
	cmp	r1, #1920
	orrls	r3, r2, #138240
	orrls	r3, r3, #32
	strls	r3, [r4, #108]
	ldr	r1, [r6]
	ldr	r1, [r1, #592]
	cmp	r1, #1
	movne	r1, #0
	beq	.L202
.L195:
	orr	r3, r1, r3
	str	r2, [r4, #100]
	str	r3, [r4, #108]
	mov	r5, #0
	str	r2, [r4, #104]
	mov	r1, r5
	str	r5, [r4, #96]
	mov	r0, r4
	ldr	r3, [ip, #3280]
	str	r3, [r4, #112]
	ldr	r3, [ip, #3276]
	str	r3, [r4, #116]
	bl	SetAspectRatio
	ldr	r3, [r6]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L203
.L196:
	mov	r2, #9
	mov	r3, #0
	str	r2, [r4, #376]
	mov	r2, #1
	strb	r3, [r4, #364]
	mov	r3, #8
	strb	r2, [r4, #371]
	strb	r2, [r4, #369]
	str	r3, [r4, #380]
	str	r3, [r4, #388]
	str	r3, [r4, #384]
.L192:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L202:
	tst	r3, #131072
	movne	r1, #0
	moveq	r1, #262144
	b	.L195
.L203:
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
	b	.L196
	UNWIND(.fnend)
	.size	Real9_SetImgFormat, .-Real9_SetImgFormat
	.align	2
	.global	RV9_RecordVFMWState
	.type	RV9_RecordVFMWState, %function
RV9_RecordVFMWState:
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
	.size	RV9_RecordVFMWState, .-RV9_RecordVFMWState
	.align	2
	.global	Real9_WriteSliceMsg
	.type	Real9_WriteSliceMsg, %function
Real9_WriteSliceMsg:
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
	mov	r3, #44
	add	r2, r0, #49152
	mla	r3, r3, r1, r0
	ldr	ip, [r2, #3420]
	add	r1, r1, #1
	str	ip, [r3, #104]
	ldr	ip, [r2, #3412]
	str	ip, [r3, #112]
	ldr	ip, [r2, #3416]
	str	ip, [r3, #120]
	ldr	ip, [r2, #3432]
	str	ip, [r3, #108]
	ldr	ip, [r2, #3424]
	str	ip, [r3, #116]
	ldr	ip, [r2, #3428]
	str	ip, [r3, #124]
	ldr	ip, [r2, #3400]
	str	ip, [r3, #128]
	ldr	ip, [r2, #3396]
	str	ip, [r3, #132]
	ldr	ip, [r2, #3392]
	str	ip, [r3, #136]
	ldr	r2, [r2, #3404]
	str	r2, [r3, #140]
	str	r1, [r0, #40]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Real9_WriteSliceMsg, .-Real9_WriteSliceMsg
	.align	2
	.global	Real9_WritePicMsg
	.type	Real9_WritePicMsg, %function
Real9_WritePicMsg:
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
	ldr	r1, [r3, #3272]
	cmp	r1, #2
	strb	r1, [r0, #9]
	ldr	r1, [r3, #3288]
	str	r1, [r0, #12]
	ldr	r1, [r3, #3284]
	str	r1, [r0, #16]
	ldr	r1, [r2, #436]
	str	r1, [r0, #20]
	ldr	r2, [r2, #440]
	str	r2, [r0, #24]
	ldr	r2, [r3, #3312]
	str	r2, [r0, #28]
	ldr	r1, [r3, #3436]
	str	r1, [r0, #32]
	ldr	r1, [r3, #3440]
	str	r1, [r0, #36]
	strne	r2, [r3, #3436]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Real9_WritePicMsg, .-Real9_WritePicMsg
	.align	2
	.global	Real9_WriteReg
	.type	Real9_WriteReg, %function
Real9_WriteReg:
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
	str	r1, [r0, #44]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Real9_WriteReg, .-Real9_WriteReg
	.align	2
	.global	Real9_ModifySliceMsg
	.type	Real9_ModifySliceMsg, %function
Real9_ModifySliceMsg:
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
	ldr	r2, [r0, #140]
	ldr	r3, [r0, #12]
	cmp	r2, #0
	ldr	ip, [r0, #16]
	bne	.L216
	mul	ip, r3, ip
	ldr	r5, [r0, #40]
	cmp	r5, #2
	sub	ip, ip, #1
	bls	.L215
	ldr	r3, [r0, #184]
	sub	lr, r5, #1
	cmp	r3, #0
	cmpgt	ip, r3
	ble	.L216
	mov	r2, r0
	mov	r3, #1
	b	.L217
.L218:
	ldr	r1, [r2, #228]
	add	r2, r2, #44
	cmp	ip, r1
	cmpgt	r1, #0
	ble	.L216
.L217:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L218
.L215:
	cmp	r5, #1
	bls	.L219
	sub	r4, r5, #1
	mov	r3, r0
	mov	r2, #0
	b	.L220
.L221:
	ldr	lr, [r3, #140]
	add	r3, r3, #44
	add	r2, r2, #1
	ldr	r1, [r3, #140]
	cmp	lr, r1
	bge	.L216
.L220:
	cmp	r2, r4
	bne	.L221
	mov	r3, #44
	mla	r2, r3, r2, r0
	ldr	r3, [r2, #140]
	cmp	ip, r3
	cmpge	r3, #0
	ble	.L216
.L219:
	cmp	r5, #0
	beq	.L222
	sub	r7, r5, #1
	mov	r6, r0
	mov	lr, r0
	mov	r4, #0
	b	.L225
.L240:
	cmp	r4, r5
	beq	.L239
.L225:
	cmp	r4, r7
	streq	ip, [lr, #144]
	add	r4, r4, #1
	add	lr, lr, #44
	ldrne	r3, [lr, #140]
	moveq	r1, ip
	subne	r1, r3, #1
	ldr	r3, [lr, #96]
	strne	r1, [lr, #100]
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
	beq	.L240
.L216:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L239:
	ldr	r2, [r0, #104]
	mov	r1, r2
	b	.L228
.L241:
	ldr	r1, [r6, #104]
.L228:
	cmp	r2, r1
	ldr	ip, [r6, #108]
	add	r3, r3, #1
	movcs	r2, r1
	cmp	ip, r2
	bcs	.L226
	ldr	r1, [r6, #124]
	cmp	r1, #0
	movne	r2, ip
.L226:
	cmp	r5, r3
	add	r6, r6, #44
	bne	.L241
	bic	r2, r2, #15
	ldr	lr, [r0, #40]
	str	r2, [r0, #48]
	mov	r3, r0
	mov	ip, #0
	b	.L232
.L242:
	ldr	r4, [r3, #112]
	rsb	r2, r2, r1
	and	r1, r4, #7
	add	r2, r2, r4, lsr #3
	and	r4, r2, #15
	bic	r2, r2, #15
	str	r2, [r3, #104]
	add	r2, r1, r4, lsl #3
	str	r2, [r3, #112]
.L230:
	add	ip, ip, #1
	add	r3, r3, #44
	cmp	lr, ip
	bls	.L231
	ldr	r2, [r0, #48]
.L232:
	ldr	r1, [r3, #108]
	cmp	r1, #0
	rsb	r4, r2, r1
	ldr	r1, [r3, #104]
	beq	.L242
	ldr	r5, [r3, #112]
	rsb	r2, r2, r1
	ldr	r7, [r3, #116]
	and	r6, r5, #7
	add	r2, r2, r5, lsr #3
	and	r5, r7, #7
	add	r1, r4, r7, lsr #3
	and	r4, r2, #15
	and	r7, r1, #15
	bic	r2, r2, #15
	add	r4, r6, r4, lsl #3
	str	r2, [r3, #104]
	bic	r1, r1, #15
	add	r2, r5, r7, lsl #3
	str	r4, [r3, #112]
	str	r2, [r3, #116]
	str	r1, [r3, #108]
	b	.L230
.L222:
	ldr	r3, [r0, #104]
	bic	r3, r3, #15
	str	r3, [r0, #48]
.L231:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Real9_ModifySliceMsg, .-Real9_ModifySliceMsg
	.align	2
	.global	REAL9DEC_Init
	.type	REAL9DEC_Init, %function
REAL9DEC_Init:
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
	ldr	r3, .L247
	mov	r6, r1
	movw	r2, #54184
	mov	r1, #0
	mov	r4, r0
	mov	r5, r1
	ldr	r3, [r3, #48]
	blx	r3
	add	r0, r4, #53760
	add	r0, r0, #56
	bl	ResetVoQueue
	add	r3, r4, #49152
	str	r6, [r4]
	mov	r2, #15
	str	r2, [r3, #3436]
	add	ip, r4, #8192
	str	r2, [r3, #3440]
	mov	r1, #8192
	ldr	lr, [r6, #8]
	mov	r2, #1
	strb	r5, [ip, #3192]
	mov	r0, r4
	add	r4, r4, #53248
	strb	lr, [ip, #3188]
	str	r1, [r3, #3296]
	str	r2, [r3, #3268]
	strb	r2, [r3, #3264]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #920]
	mov	r0, r5
	beq	.L246
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L246:
	ldr	r1, .L247+4
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L248:
	.align	2
.L247:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC10
	UNWIND(.fnend)
	.size	REAL9DEC_Init, .-REAL9DEC_Init
	.align	2
	.global	REAL9DEC_Destroy
	.type	REAL9DEC_Destroy, %function
REAL9DEC_Destroy:
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
	ldr	r1, .L250
	mov	r0, #2
	ldr	r2, [r3, #412]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L251:
	.align	2
.L250:
	.word	.LC11
	UNWIND(.fnend)
	.size	REAL9DEC_Destroy, .-REAL9DEC_Destroy
	.global	__aeabi_idiv
	.align	2
	.global	REAL9DEC_Decode
	.type	REAL9DEC_Decode, %function
REAL9DEC_Decode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 192
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #212)
	sub	sp, sp, #212
	add	r10, r0, #49152
	mov	r3, r0
	str	r0, [fp, #-192]
	mov	r4, r1
	mov	r2, #128
	mov	r1, #0
	sub	r0, fp, #172
	add	r3, r3, #8
	str	r3, [fp, #-196]
	bl	memset
	ldr	r2, [r10, #3372]
	ldr	r3, [r10, #3376]
	cmp	r4, #0
	mov	r0, #1
	mov	r1, #8192
	str	r2, [r10, #3380]
	str	r2, [fp, #-56]
	str	r2, [fp, #-64]
	str	r3, [r10, #3384]
	str	r3, [fp, #-52]
	str	r3, [fp, #-60]
	str	r0, [r10, #3268]
	str	r0, [fp, #-168]
	str	r1, [r10, #3296]
	str	r1, [fp, #-140]
	beq	.L397
	ldr	ip, [fp, #-192]
	movw	r2, #11360
	ldr	r3, .L418
	mov	r1, #0
	add	r5, ip, #53248
	ldr	r0, [fp, #-196]
	add	r8, ip, #8192
	mov	r9, r1
	ldr	r3, [r3, #48]
	blx	r3
	str	r9, [r5, #376]
	ldr	r3, [r4, #16]
	str	r3, [r5, #396]
	ldr	r3, [r4, #12]
	str	r3, [r5, #380]
	ldr	r3, [r4]
	str	r3, [r8, #3176]
	ldr	r2, [r4, #16]
	str	r2, [r5, #384]
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	strb	r3, [r8, #3180]
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L255
	ldr	r3, [r4, #92]
	str	r3, [r5, #388]
	ldr	r3, [r4, #80]
	str	r3, [r8, #3184]
	ldr	r3, [r4, #96]
	str	r3, [r5, #392]
	ldrb	r3, [r4, #86]	@ zero_extendqisi2
	strb	r3, [r8, #3181]
	ldrb	r3, [r4, #86]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L398
.L256:
	movw	r3, #65525
	sub	r1, r2, #10
	movt	r3, 47
	cmp	r1, r3
	bhi	.L393
	ldr	r1, [r8, #3176]
	cmp	r1, #0
	beq	.L393
	ldr	r7, [fp, #-192]
	add	r4, r7, #53760
	add	r4, r4, #24
	mov	r0, r4
	bl	BsInit
	mov	r1, #8
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	mov	r6, r0
	mov	r0, r4
	bl	BsSkip
	cmp	r6, #255
	add	r0, r6, #1
	str	r0, [r10, #3444]
	bhi	.L393
	cmp	r0, #0
	beq	.L259
	add	r9, r7, #52480
	movw	r3, #65535
	add	r9, r9, #116
	movt	r3, 47
	str	r5, [fp, #-200]
	mov	r6, #0
	mov	r5, r9
	mov	r9, r3
	b	.L260
.L400:
	ldr	r0, [r10, #3444]
	cmp	r0, r6
	bls	.L399
.L260:
	mov	r1, #32
	mov	r0, r4
	bl	BsSkip
	mov	r1, #32
	mov	r0, r4
	add	r6, r6, #1
	bl	BsShow
	mov	r1, #32
	mov	r7, r0
	mov	r0, r4
	bl	BsSkip
	rev	r0, r7
	cmp	r0, r9
	str	r0, [r5, #4]!
	ble	.L400
.L393:
	mov	r9, #0
.L382:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L398:
	ldr	r1, [r4, #20]
	ldr	r0, [r5, #920]
	bl	SM_ReleaseStreamSeg
	ldr	r1, [r4, #100]
	ldr	r0, [r5, #920]
	bl	SM_ReleaseStreamSeg
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L255:
	str	r9, [r5, #388]
	str	r9, [r8, #3184]
	str	r9, [r5, #392]
	strb	r9, [r8, #3181]
	b	.L256
.L397:
	ldr	r3, [fp, #-192]
	mov	r9, r4
	ldr	r1, .L418+4
	add	r7, r3, #53248
	ldr	r2, [r7, #412]
	bl	dprint_vfmw
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L399:
	ldr	r5, [fp, #-200]
.L259:
	ldr	r7, [fp, #-192]
	sub	r6, r0, #1
	mov	r2, #0
	add	r3, r7, #52480
	add	r3, r3, #120
	b	.L261
.L262:
	ldr	ip, [r3, #4]!
	ldr	r1, [r3, #-4]
	cmp	ip, r1
	ble	.L393
.L261:
	cmp	r2, r6
	add	r2, r2, #1
	bne	.L262
	ldr	r2, [r10, #3448]
	mov	r0, r0, asl #3
	add	r1, r0, #1
	movw	r3, #65534
	add	r0, r1, r2
	cmp	r1, #3145728
	sub	r6, r0, #1
	movt	r3, 47
	movlt	r2, #0
	movge	r2, #1
	cmp	r6, r3
	movls	r6, r2
	orrhi	r6, r2, #1
	str	r7, [fp, #-192]
	cmp	r6, #0
	str	r1, [fp, #-200]
	str	r0, [fp, #-204]
	bne	.L393
	ldr	r2, [r5, #384]
	mov	r3, r0
	rsb	r2, r0, r2
	cmp	r2, #0
	ble	.L393
	ldr	r1, [r8, #3176]
	mov	r0, r4
	add	r1, r1, r3
	bl	BsInit
	sub	r1, fp, #172
	mov	r0, r4
	bl	Real9_CB_GetPictureHeader
	cmp	r0, #1
	mov	r9, r0
	bne	.L401
	ldrb	r3, [r8, #3192]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L264
	ldr	r2, [fp, #-148]
	ldr	r3, [r10, #3288]
	cmp	r2, r3
	beq	.L402
.L264:
	ldr	r3, .L418+8
	ldr	r6, [r3]
	cmp	r6, #0
	beq	.L266
	ldr	r0, [fp, #-148]
	mov	r3, #8
	ldr	r1, [r10, #3284]
	sub	r2, fp, #180
	ldr	lr, [r10, #3288]
	strh	r0, [fp, #-176]	@ movhi
	ldrh	r0, [fp, #-152]
	strh	r1, [fp, #-178]	@ movhi
	mov	r1, #2
	strh	lr, [fp, #-180]	@ movhi
	strh	r0, [fp, #-174]	@ movhi
	ldr	r0, [r5, #920]
	blx	r6
.L266:
	ldr	r0, [r5, #920]
	bl	FSP_GetFspType
	subs	lr, r0, #0
	bne	.L267
	sub	r3, fp, #44
	sub	r2, fp, #184
	sub	r1, fp, #188
	ldr	r0, [r5, #920]
	str	lr, [r3, #-136]!
	str	lr, [fp, #-188]
	str	lr, [fp, #-184]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [fp, #-192]
	ldr	r3, [r3]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L403
.L267:
	ldr	r3, [fp, #-192]
	mov	r2, #1
	ldr	r1, [r5, #516]
	ldr	r0, [r5, #920]
	add	r6, r3, #53760
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r5, #512]
	add	r6, r6, #56
	ldr	r0, [r5, #920]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r5, #920]
	str	r3, [r5, #512]
	str	r3, [r5, #516]
	str	r3, [fp, #-208]
	bl	FSP_GetNullLogicFs
	ldr	r3, [fp, #-208]
	mov	r1, r6
	str	r0, [r5, #516]
	str	r0, [r5, #512]
	strb	r3, [r8, #3189]
	ldr	r0, [r5, #920]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [fp, #-148]
	ldr	r3, [fp, #-152]
	cmp	r2, #512
	bhi	.L274
	cmp	r3, #512
	bhi	.L274
	ldr	r3, [fp, #-192]
	ldr	r3, [r3]
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L276
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L404
.L276:
	ldr	r0, [r5, #920]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L388
.L278:
	ldr	r3, [fp, #-152]
	mov	r0, #1
	ldr	r2, [fp, #-148]
	ldr	r1, .L418+12
	bl	dprint_vfmw
	sub	r2, fp, #172
	ldr	r1, [fp, #-192]
	mov	r0, #6
	bl	Real9_ArrangeVHBMem
	subs	r3, r0, #0
	beq	.L405
.L265:
	ldr	r6, [fp, #-192]
	sub	r1, fp, #172
	add	r3, r6, #52224
	mov	r2, r3
	ldr	r3, .L418
	add	r2, r2, #192
	str	r2, [fp, #-212]
	ldr	r3, [r3, #52]
	mov	r0, r2
	mov	r2, #128
	blx	r3
	ldr	r3, [r6]
	mov	r0, r6
	ldr	r3, [r3, #8]
	strb	r3, [r8, #3188]
	bl	Real9_GetImageBuffer
	subs	r3, r0, #0
	beq	.L406
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	bl	FSP_GetFsImagePtr
	subs	lr, r0, #0
	beq	.L281
	ldr	r2, [fp, #-192]
	add	r3, lr, #608
	add	ip, lr, #624
	ldr	r1, .L418+16
	mov	r0, #29
	ldr	r2, [r2]
	ldrd	r6, [r2, #56]
	ldr	r2, [fp, #-192]
	strd	r6, [r3, #-8]
	ldr	r2, [r2]
	ldrd	r6, [r2, #72]
	ldr	r2, [fp, #-192]
	strd	r6, [r3]
	ldr	r2, [r2]
	ldrd	r6, [r2, #88]
	strd	r6, [ip, #-8]
	mvn	r6, #0
	ldr	ip, [fp, #-192]
	mvn	r7, #0
	ldr	r2, [ip]
	ldr	r2, [r2, #104]
	str	r2, [lr, #8]
	ldr	r2, [ip]
	ldr	r2, [r2, #108]
	str	r2, [lr, #12]
	ldr	r2, [ip]
	ldr	r2, [r2, #112]
	str	r2, [lr, #16]
	ldr	r2, [ip]
	ldr	r2, [r2, #128]
	str	r2, [lr, #20]
	ldr	r2, [ip]
	strd	r6, [r2, #56]
	ldrd	r6, [r3]
	ldrd	r2, [r3, #-8]
	strd	r6, [sp]
	bl	dprint_vfmw
	ldr	r3, [r10, #3272]
	cmp	r3, #2
	beq	.L407
	cmp	r3, #1
	ldrb	r3, [r8, #3189]	@ zero_extendqisi2
	beq	.L408
	cmp	r3, #0
	movne	r3, #2
	moveq	r3, #1
.L285:
	strb	r3, [r8, #3189]
	ldr	r2, [r5, #512]
	ldr	r3, [r5, #516]
	cmp	r2, r3
	beq	.L286
	ldr	r1, .L418+20
	mov	r0, #13
	bl	dprint_vfmw
	mov	r2, #0
	ldr	r1, [r5, #512]
	ldr	r0, [r5, #920]
	bl	FSP_SetRef
.L286:
	ldr	r3, [r5, #516]
	mov	r2, #1
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	str	r3, [r5, #512]
	str	r1, [r5, #516]
	bl	FSP_SetRef
	ldr	r1, [r5, #512]
	ldr	r0, [r5, #920]
	bl	FSP_RemovePmv
	ldr	r2, [r5, #520]
	ldr	r1, .L418+24
	mov	r0, #13
	bl	dprint_vfmw
	ldrb	r3, [r8, #3188]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L288
	ldr	r3, [r10, #3272]
	cmp	r3, #0
	beq	.L389
	cmp	r3, #2
	beq	.L329
	cmp	r3, #1
	beq	.L389
.L288:
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	bl	FSP_GetLogicFs
	ldr	r1, [r5, #512]
	str	r0, [fp, #-208]
	ldr	r0, [r5, #920]
	bl	FSP_GetLogicFs
	ldr	r1, [r5, #516]
	mov	r6, r0
	str	r0, [fp, #-220]
	ldr	r0, [r5, #920]
	bl	FSP_GetLogicFs
	ldr	ip, [fp, #-208]
	mov	r3, r6
	cmp	ip, #0
	cmpne	r6, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	mov	r6, r0
	cmp	r3, #0
	bne	.L409
	ldr	r3, [ip, #640]
	cmp	r3, #0
	beq	.L294
	ldr	r2, [ip, #648]
	cmp	r2, #0
	beq	.L294
	ldr	r2, [ip, #644]
	cmp	r2, #0
	beq	.L294
	ldr	r2, [r0, #640]
	mov	r0, #13
	ldr	lr, [fp, #-192]
	cmp	r2, #0
	ldr	r1, .L418+28
	ldrne	r3, [r2, #16]
	ldreq	r3, [r3, #16]
	ldr	r2, [fp, #-192]
	str	r3, [fp, #-208]
	ldr	r3, [fp, #-208]
	str	r3, [r2, #52]
	ldr	r3, [fp, #-220]
	ldr	r3, [r3, #640]
	cmp	r3, #0
	ldreq	r3, [ip, #640]
	ldr	r3, [r3, #16]
	str	ip, [fp, #-228]
	str	r3, [fp, #-216]
	ldr	r3, [fp, #-216]
	str	r3, [lr, #56]
	ldr	r3, [ip, #640]
	ldr	r2, [r3, #16]
	str	r2, [lr, #60]
	ldr	r3, [ip, #652]
	str	r3, [lr, #76]
	mov	r7, r3
	ldr	r3, [r6, #652]
	str	r3, [fp, #-224]
	str	r3, [lr, #80]
	ldr	r3, [ip, #644]
	ldr	ip, [fp, #-224]
	ldr	r3, [r3, #16]
	str	ip, [sp, #12]
	ldr	ip, [fp, #-208]
	str	r7, [sp, #8]
	mov	r7, lr
	str	r3, [lr, #64]
	str	ip, [sp, #4]
	ldr	ip, [fp, #-216]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	ip, [fp, #-228]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #32]
	str	r3, [r7, #68]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #32]
	str	r3, [r5, #484]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #76]
	str	r3, [r7, #92]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #60]
	str	r3, [r7, #96]
	ldr	r3, [ip, #644]
	ldr	r3, [r3, #64]
	str	r3, [r7, #100]
	ldr	r3, [r10, #3272]
	cmp	r3, #2
	beq	.L410
.L300:
	ldr	r3, [r10, #3300]
	mov	r0, r4
	ldr	r1, [fp, #-192]
	str	r3, [ip, #660]
	ldr	r3, [r10, #3272]
	ldr	r2, [r10, #3312]
	cmp	r3, #2
	strb	r3, [r1, #9]
	ldr	r3, [r10, #3288]
	str	r2, [fp, #-208]
	str	r3, [r1, #12]
	ldr	r3, [r10, #3284]
	str	r3, [r1, #16]
	ldr	r3, [r5, #436]
	str	r3, [r1, #20]
	ldr	r3, [r5, #440]
	str	r2, [r1, #28]
	str	r3, [r1, #24]
	ldr	r3, [r10, #3436]
	str	r3, [r1, #32]
	ldr	r3, [r10, #3440]
	str	r3, [r1, #36]
	strne	r2, [r10, #3436]
	bl	BsPos
	ldr	r1, [r10, #3444]
	cmp	r1, #1
	ldreq	r6, [r10, #3448]
	ldrne	r6, [r10, #3448]
	mov	r3, r0, asr #31
	mov	r2, r3, lsr #29
	add	r3, r0, r2
	moveq	r0, r4
	and	r3, r3, #7
	movne	r0, r4
	rsb	r3, r2, r3
	ldreq	r2, [fp, #-200]
	str	r3, [r10, #3412]
	ldreq	r3, [r5, #384]
	ldrne	r3, [r10, #3452]
	rsbeq	r3, r2, r3
	rsb	r6, r6, r3
	bl	BsPos
	ldr	r3, [fp, #-204]
	mov	r6, r6, asl #3
	rsb	r0, r0, r6
	str	r0, [r10, #3416]
	ldr	r6, [r5, #380]
	mov	r0, r4
	add	r6, r3, r6
	bl	BsPos
	ldr	r2, [r10, #3316]
	mov	r1, #1
	ldr	lr, [r10, #3308]
	ldr	ip, [fp, #-208]
	str	r2, [r10, #3396]
	str	lr, [r10, #3400]
	str	ip, [r10, #3392]
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	mov	r3, #0
	add	r0, r6, r0, asr #3
	ldr	r6, [fp, #-192]
	str	r0, [r10, #3420]
	str	r3, [r10, #3424]
	str	r3, [r10, #3428]
	str	r3, [r10, #3432]
	str	r3, [r10, #3404]
	str	r0, [r6, #104]
	ldr	r0, [r10, #3412]
	str	r0, [r6, #112]
	ldr	r0, [r10, #3416]
	str	lr, [r6, #128]
	mov	lr, r6
	str	r3, [r6, #108]
	str	r0, [r6, #120]
	mov	r0, r6
	str	r3, [r6, #116]
	str	r3, [r6, #124]
	str	r2, [r6, #132]
	str	r3, [r6, #140]
	str	ip, [r0, #136]
	str	r1, [r6, #40]
	ldr	r2, [r10, #3444]
	cmp	r2, r1
	bls	.L325
	ldr	r2, [r10, #3452]
	movw	r0, #65535
	ldr	ip, [fp, #-200]
	movt	r0, 47
	add	ip, ip, r2
	cmp	ip, r0
	bgt	.L313
	cmp	ip, #0
	ble	.L313
	ldr	r2, [r5, #384]
	rsb	r2, ip, r2
	cmp	r2, #0
	ble	.L313
	ldr	lr, [fp, #-192]
	mov	r6, r1
	str	r9, [fp, #-232]
	mov	r7, r5
	add	lr, lr, #52480
	str	r3, [fp, #-228]
	add	r1, lr, #124
	str	r0, [fp, #-216]
	sub	lr, fp, #180
	str	r6, [fp, #-220]
	str	lr, [fp, #-204]
	mov	r9, ip
	mov	r5, r1
	b	.L316
.L317:
	ldr	r3, [r10, #3444]
	add	r6, r6, #1
	cmp	r3, r6
	bls	.L411
	ldr	r3, [fp, #-200]
	ldr	r9, [r5, #4]!
	add	r9, r3, r9
	ldr	r3, [fp, #-216]
	cmp	r9, r3
	bgt	.L387
	cmp	r9, #0
	ble	.L387
	ldr	r2, [r7, #384]
	rsb	r2, r9, r2
	cmp	r2, #0
	ble	.L387
.L316:
	ldr	r1, [r8, #3176]
	mov	r0, r4
	add	r1, r1, r9
	bl	BsInit
	ldr	r3, [fp, #-212]
	sub	r2, fp, #188
	mov	r1, #0
	mov	r0, r4
	str	r3, [sp, #4]
	ldr	r3, [fp, #-204]
	str	r3, [sp]
	sub	r3, fp, #184
	bl	Real9_CB_GetSliceHeader
	cmp	r0, #1
	bne	.L317
	ldr	r3, [r10, #3288]
	ldr	r2, [fp, #-188]
	cmp	r3, #0
	str	r2, [fp, #-224]
	beq	.L412
	ldr	r3, [fp, #-180]
	mov	r0, r4
	ldr	r2, [fp, #-208]
	cmp	r3, #0
	movge	r2, r3
	str	r2, [fp, #-208]
	bl	BsPos
	ldr	r2, [r10, #3444]
	sub	lr, r2, #1
	cmp	lr, r6
	mov	r3, r0, asr #31
	mov	r1, r3, lsr #29
	add	r3, r0, r1
	and	r3, r3, #7
	rsb	r3, r1, r3
	str	r3, [r10, #3412]
	beq	.L413
	cmp	r6, #254
	bhi	.L414
	ldr	r3, [r5, #4]
	mov	r0, r4
	ldr	r2, [r5]
.L392:
	rsb	r3, r2, r3
	mov	r3, r3, asl #3
	str	r3, [fp, #-236]
	bl	BsPos
	ldr	r3, [fp, #-236]
	rsb	r0, r0, r3
	str	r0, [r10, #3416]
	ldr	r3, [r7, #380]
	mov	r0, r4
	add	r9, r9, r3
	bl	BsPos
	ldr	ip, [fp, #-228]
	mov	r3, #44
	ldr	lr, [fp, #-220]
	ldr	r1, [fp, #-192]
	str	ip, [r10, #3424]
	str	ip, [r10, #3428]
	mla	r3, r3, lr, r1
	str	ip, [r10, #3432]
	ldr	ip, [fp, #-208]
	add	lr, lr, #1
	ldr	r1, [r10, #3308]
	str	lr, [fp, #-220]
	str	ip, [r10, #3392]
	ldr	ip, [fp, #-224]
	str	r1, [r10, #3400]
	str	ip, [r10, #3404]
	add	r2, r0, #7
	cmp	r0, #0
	movlt	r0, r2
	ldr	r2, [r10, #3316]
	add	r0, r9, r0, asr #3
	str	r0, [r10, #3420]
	cmp	lr, #131
	str	r2, [r10, #3396]
	str	r0, [r3, #104]
	ldr	r2, [r10, #3412]
	str	r2, [r3, #112]
	ldr	r2, [r10, #3416]
	str	r2, [r3, #120]
	ldr	r2, [r10, #3432]
	str	r2, [r3, #108]
	ldr	r2, [r10, #3424]
	str	r2, [r3, #116]
	ldr	r2, [r10, #3428]
	str	r2, [r3, #124]
	ldr	r2, [r10, #3400]
	str	r2, [r3, #128]
	ldr	r2, [r10, #3396]
	str	r2, [r3, #132]
	ldr	r2, [r10, #3392]
	str	r2, [r3, #136]
	ldr	r2, [r10, #3404]
	ldr	r1, [fp, #-192]
	str	r2, [r3, #140]
	str	lr, [r1, #40]
	ble	.L317
	mov	r0, r1
	bl	Real9_ModifySliceMsg
	cmp	r0, #1
	bne	.L387
	ldr	r3, [fp, #-192]
	ldr	r2, [fp, #-196]
	str	r2, [r3, #4]
	b	.L317
.L401:
	ldr	r1, .L418+32
	mov	r0, #1
	mov	r9, r6
	bl	dprint_vfmw
	b	.L382
.L274:
	ldr	r1, .L418+36
	mov	r0, #18
	bl	dprint_vfmw
	b	.L393
.L406:
	ldr	r2, [r5, #412]
	mov	r9, r3
	ldr	r1, .L418+40
	bl	dprint_vfmw
	b	.L382
.L403:
	ldr	r3, [fp, #-180]
	cmp	r3, #0
	bgt	.L270
	ldr	r3, [fp, #-184]
	cmp	r3, #32
	bgt	.L270
	ldr	r3, [fp, #-192]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L270
	ldr	r3, [r5, #408]
	cmp	r3, #0
	beq	.L267
	ldr	r6, [r10, #3288]
	cmp	r6, #0
	beq	.L267
	ldr	r0, [fp, #-152]
	ldr	r1, [r5, #924]
	cmp	r0, r1
	beq	.L267
	ldr	r1, [fp, #-148]
	ldr	lr, [r5, #928]
	cmp	r1, lr
	beq	.L267
	ldr	lr, [r10, #3284]
	str	r0, [sp, #12]
	mov	r0, r2
	str	r1, [sp, #8]
	stmia	sp, {r6, lr}
	ldr	r1, .L418+44
	bl	dprint_vfmw
	ldr	r3, .L418+8
	ldr	r2, [fp, #-152]
	ldr	r6, [r3]
	ldr	r3, [fp, #-148]
	cmp	r6, #0
	str	r2, [r5, #924]
	str	r3, [r5, #928]
	beq	.L267
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r5, #920]
	blx	r6
	b	.L267
.L402:
	ldr	r2, [fp, #-152]
	ldr	r3, [r10, #3284]
	cmp	r2, r3
	bne	.L264
	b	.L265
.L404:
	ldrb	r3, [r8, #3192]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L278
.L388:
	mov	r0, r6
	bl	ResetVoQueue
	b	.L278
.L405:
	ldr	r1, .L418+48
	mov	r0, #1
	mov	r9, r3
	bl	dprint_vfmw
	b	.L382
.L387:
	mov	r5, r7
.L313:
	mov	r2, #1
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	mov	r9, #0
	bl	FSP_ClearLogicFs
	b	.L382
.L407:
	ldrb	r3, [r8, #3189]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L313
	ldrb	r3, [r8, #3188]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L288
.L329:
	ldr	r3, [r5, #528]
	cmp	r3, #0
	bne	.L288
	mov	r2, #1
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	mov	r9, r3
	bl	FSP_ClearLogicFs
	b	.L382
.L389:
	str	r3, [r5, #528]
	b	.L288
.L409:
	movw	r3, #1997
	ldr	r2, .L418+52
	ldr	r1, .L418+56
	mov	r0, #0
	bl	dprint_vfmw
	mov	r9, #0
	b	.L382
.L408:
	cmp	r3, #0
	movne	r3, #2
	bne	.L285
	b	.L393
.L413:
	add	r2, r2, #10304
	ldr	r3, [r7, #384]
	add	r2, r2, #3
	ldr	r1, [fp, #-200]
	mov	r0, r4
	add	r2, r8, r2, lsl #2
	rsb	r3, r1, r3
	ldr	r2, [r2, #3176]
	b	.L392
.L270:
	mvn	r9, #1
	b	.L382
.L281:
	mov	r2, #1
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	mov	r9, lr
	bl	FSP_ClearLogicFs
	b	.L382
.L411:
	ldr	r9, [fp, #-232]
	mov	r5, r7
.L325:
	ldr	r3, [fp, #-192]
	mov	r6, #1
	ldr	r4, [r3, #4]
	str	r6, [r3, #44]
	cmp	r4, #0
	beq	.L415
	ldr	r3, [fp, #-192]
	ldr	r3, [r3]
	ldr	r2, [r3, #864]
	cmp	r2, #1
	beq	.L330
.L327:
	cmp	r2, #2
	beq	.L416
.L328:
	ldr	r2, [r3, #592]
	ldr	r1, [fp, #-192]
	str	r2, [r1, #84]
	ldr	r3, [r3, #632]
	str	r3, [r1, #88]
	b	.L382
.L410:
	ldr	r3, [fp, #-220]
	ldr	r6, [r6, #660]
	ldr	r2, [r3, #660]
	ldr	r3, [r10, #3300]
	rsb	r6, r2, r6
	subs	r3, r3, r2
	ldrmi	r2, [r10, #3296]
	addmi	r3, r3, r2
	cmp	r6, #0
	ldrlt	r2, [r10, #3296]
	addlt	r6, r6, r2
	cmp	r3, r6
	movgt	r3, #0
	cmp	r6, #0
	ble	.L332
	mov	r1, r6
	mov	r0, r3, asl #14
	str	ip, [fp, #-220]
	str	r3, [fp, #-208]
	bl	__aeabi_idiv
	ldr	r3, [fp, #-208]
	mov	r1, r6
	str	r0, [fp, #-216]
	rsb	r0, r3, r6
	mov	r0, r0, asl #14
	bl	__aeabi_idiv
	ldr	r3, [fp, #-208]
	ldr	r2, [fp, #-216]
	ldr	ip, [fp, #-220]
.L304:
	str	r3, [r5, #428]
	str	r6, [r5, #432]
	str	r2, [r5, #436]
	str	r0, [r5, #440]
	b	.L300
.L294:
	ldr	r1, [r5, #520]
	mov	r2, #1
	ldr	r0, [r5, #920]
	mov	r9, #0
	bl	FSP_ClearLogicFs
	ldr	r1, .L418+60
	mov	r0, r9
	bl	dprint_vfmw
	b	.L382
.L414:
	movw	r2, #2229
	ldr	r1, .L418+64
	mov	r0, #22
	mov	r9, #0
	bl	dprint_vfmw
	b	.L382
.L412:
	mov	r2, r0
	ldr	r1, [r7, #520]
	ldr	r0, [r7, #920]
	mov	r9, r3
	bl	FSP_ClearLogicFs
	b	.L382
.L415:
	ldr	r0, [fp, #-192]
	bl	Real9_ModifySliceMsg
	cmp	r0, #1
	bne	.L417
	ldr	r1, [fp, #-192]
	ldr	r2, [fp, #-196]
	ldr	r3, [r1]
	str	r2, [r1, #4]
	ldr	r2, [r3, #864]
	cmp	r2, #1
	bne	.L327
	ldr	r2, [fp, #-196]
	cmp	r2, #0
	beq	.L328
	mov	r4, r2
.L330:
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L328
	ldr	r4, [fp, #-192]
	mov	r2, #0
	mov	r9, r2
	str	r2, [r4, #4]
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	bl	FSP_SetDisplay
	mov	r1, r4
	mov	r0, #9
	bl	ReleasePacket
	b	.L382
.L416:
	ldr	r2, [r3, #868]
	cmp	r2, #0
	ble	.L328
	ldr	r2, [fp, #-192]
	ldr	r2, [r2, #4]
	cmp	r2, #0
	beq	.L328
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L328
	ldr	r4, [fp, #-192]
	mov	r2, #0
	mov	r9, r2
	str	r2, [r4, #4]
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	bl	FSP_SetDisplay
	ldr	r2, [r4]
	mov	r1, r4
	mov	r0, #9
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	bl	ReleasePacket
	b	.L382
.L332:
	mov	r0, #0
	mov	r2, r0
	b	.L304
.L417:
	mov	r2, r6
	ldr	r1, [r5, #520]
	ldr	r0, [r5, #920]
	mov	r9, r4
	bl	FSP_ClearLogicFs
	b	.L382
.L419:
	.align	2
.L418:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
	.word	g_event_report
	.word	.LC16
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC24
	.word	.LC13
	.word	.LC15
	.word	.LC18
	.word	.LC14
	.word	.LC17
	.word	.LANCHOR0+460
	.word	.LC22
	.word	.LC23
	.word	.LC25
	UNWIND(.fnend)
	.size	REAL9DEC_Decode, .-REAL9DEC_Decode
	.align	2
	.global	REAL9DEC_RecycleImage
	.type	REAL9DEC_RecycleImage, %function
REAL9DEC_RecycleImage:
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
	ldr	r0, [r4, #920]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #920]
	bl	FSP_GetFsImagePtr
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL9DEC_RecycleImage, .-REAL9DEC_RecycleImage
	.align	2
	.global	REAL9DEC_GetRemainImg
	.type	REAL9DEC_GetRemainImg, %function
REAL9DEC_GetRemainImg:
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
	beq	.L438
	add	r6, r0, #53248
	ldr	r1, [r6, #516]
	ldr	r0, [r6, #920]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L438
	ldr	r1, [r6, #516]
	ldr	r0, [r6, #920]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L437
.L438:
	add	r5, r4, #53760
	add	r5, r5, #56
.L428:
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	GetVoLastImageID
.L437:
	mov	r0, r4
	add	r5, r4, #53760
	bl	Real9_SetImgFormat
	add	r5, r5, #56
	mov	r3, #1
	str	r3, [r7, #188]
	mov	r2, r3
	ldr	r1, [r6, #516]
	ldr	r0, [r6, #920]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #920]
	mov	r2, r4
	str	r7, [sp]
	mov	r3, r5
	mov	r1, #9
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L425
	mov	r2, #0
	ldr	r1, [r6, #516]
	ldr	r0, [r6, #920]
	bl	FSP_SetDisplay
	b	.L428
.L425:
	ldr	r3, [r7, #160]
	cmp	r3, #100
	bhi	.L428
	ldr	r3, [r7, #112]
	cmp	r3, #31
	bls	.L428
	ldr	r3, [r7, #116]
	cmp	r3, #31
	bls	.L428
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL9DEC_GetRemainImg, .-REAL9DEC_GetRemainImg
	.align	2
	.global	REAL9DEC_VDMPostProc
	.type	REAL9DEC_VDMPostProc, %function
REAL9DEC_VDMPostProc:
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
	ldr	r1, .L468
	add	r4, r5, #53248
	mov	r8, r2
	mov	r0, #22
	mov	r2, r6
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, #9
	add	r7, r5, #8192
	bl	ReleasePacket
	ldr	r1, [r4, #520]
	ldr	r0, [r4, #920]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L440
	add	r9, r5, #49152
	ldr	r3, [r9, #3272]
	cmp	r3, #2
	andne	r8, r8, #31
	strb	r3, [r7, #3190]
	strne	r8, [r9, #3440]
	cmp	r6, #0
	movlt	r6, #0
	blt	.L442
	cmp	r6, #100
	movge	r6, #100
.L442:
	str	r6, [r0, #160]
	mov	r0, #22
	ldr	r1, .L468+4
	bl	dprint_vfmw
	mov	r0, r5
	bl	Real9_SetImgFormat
	ldrb	r3, [r7, #3188]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L445
	ldr	r3, [r5]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L445
	ldr	r3, [r9, #3272]
	cmp	r3, #2
	beq	.L445
	ldrb	r3, [r7, #3189]	@ zero_extendqisi2
	cmp	r3, #1
	ldrhi	r1, [r4, #512]
	mvnls	r1, #0
	cmn	r1, #1
	str	r1, [r4, #524]
	beq	.L447
.L467:
	ldr	r0, [r4, #920]
	bl	FSP_GetFsImagePtr
	ldr	r6, [r4, #524]
	mov	r1, r6
	mov	r7, r0
	ldr	r0, [r4, #920]
	bl	FSP_GetRef
	mov	r2, r6
	ldr	r1, .L468+8
	mov	r3, r0
	mov	r0, #12
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r4, #524]
	ldr	r0, [r4, #920]
	bl	FSP_SetDisplay
	add	r3, r5, #53760
	ldr	r0, [r4, #920]
	add	r3, r3, #56
	str	r7, [sp]
	mov	r2, r5
	mov	r1, #9
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L466
.L447:
	ldr	r3, [r4, #412]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r4, #412]
.L440:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L445:
	ldr	r1, [r4, #520]
	cmn	r1, #1
	str	r1, [r4, #524]
	bne	.L467
	b	.L447
.L466:
	ldr	r0, [r4, #920]
	mov	r2, #0
	ldr	r1, [r4, #524]
	bl	FSP_SetDisplay
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L469:
	.align	2
.L468:
	.word	.LC26
	.word	.LC27
	.word	.LC28
	UNWIND(.fnend)
	.size	REAL9DEC_VDMPostProc, .-REAL9DEC_VDMPostProc
	.align	2
	.global	REAL9DEC_GetImageBuffer
	.type	REAL9DEC_GetImageBuffer, %function
REAL9DEC_GetImageBuffer:
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
	ldr	r0, [r4, #920]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #920]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L476
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L476:
	add	r1, r5, #53760
	ldr	r0, [r4, #920]
	add	r1, r1, #56
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL9DEC_GetImageBuffer, .-REAL9DEC_GetImageBuffer
	.align	2
	.global	REAL9DEC_Support
	.type	REAL9DEC_Support, %function
REAL9DEC_Support:
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
	.size	REAL9DEC_Support, .-REAL9DEC_Support
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
	.type	__FUNCTION__.14532, %object
	.size	__FUNCTION__.14532, 16
__FUNCTION__.14532:
	.ascii	"REAL9DEC_Decode\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC3:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"image size abnormal (%dx%d) in MB\012\000" )
	.space	1
.LC8:
	ASCII(.ascii	"real9 actual frame size(%dx%d) exeed max config(%dx" )
	ASCII(.ascii	"%d)\012\000" )
.LC9:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC11:
	ASCII(.ascii	"Decoding quits at frm %d\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet pointer!\012\000" )
.LC13:
	ASCII(.ascii	"Real9_CB_GetPictureHeader Error!\012\000" )
	.space	2
.LC14:
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC15:
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC16:
	ASCII(.ascii	"new seq size %d * %d\012\000" )
	.space	2
.LC17:
	ASCII(.ascii	"Real9_ArrangeVHBMem err\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"Fatal Error: frm%d, get image buffer failed\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC20:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC21:
	ASCII(.ascii	"++++++++++ ref %d\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"CurFsID abnormal!,%s %d (NULL == pCur) || (NULL == " )
	ASCII(.ascii	"pFwd) || (NULL == pBwd)!!\012\000" )
	.space	2
.LC23:
	ASCII(.ascii	"CurFsID abnormal!\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,bwd,currpmv,colpmv = %p,%p," )
	ASCII(.ascii	"%p,%p,%p,%p\012\000" )
.LC25:
	ASCII(.ascii	"line:%d! i > (RV9_MAX_SLC_PARAM_NUM - 2)\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC28:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
