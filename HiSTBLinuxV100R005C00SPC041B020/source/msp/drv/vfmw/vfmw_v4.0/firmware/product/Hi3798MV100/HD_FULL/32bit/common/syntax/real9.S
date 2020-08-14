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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r7, r0, #77824
	mov	r5, r0
	add	r6, r0, #45056
	mov	r1, #1
	ldr	r0, [r7, #2456]
	bl	FSP_NewLogicFs
	add	r8, r5, #8192
	cmp	r0, #0
	str	r0, [r6, #1016]
	blt	.L17
	mov	r1, r0
	ldr	r0, [r7, #2456]
	bl	FSP_GetLogicFs
	subs	r4, r0, #0
	beq	.L18
	ldr	r5, .L19
	mov	r0, #18
	ldr	r2, [r6, #1016]
	ldr	r1, .L19+4
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [r4, #520]
	cmp	r2, #0
	beq	.L16
	ldr	r3, [r4, #524]
	cmp	r3, #0
	beq	.L16
	ldr	r1, [r4, #528]
	cmp	r1, #0
	beq	.L16
	ldr	lr, [r1, #12]
	mov	r0, #18
	ldr	r3, [r3, #12]
	ldr	r2, [r2, #12]
	ldr	ip, [r5, #68]
	ldr	r1, .L19+8
	str	lr, [sp]
	blx	ip
	mov	r0, #1
.L12:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L16:
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L17:
	ldr	r3, .L19
	mov	r0, #0
	ldr	r1, .L19+12
	mov	r4, r0
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r6, #1012]
	ldr	r0, [r7, #2456]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r6, #1008]
	ldr	r0, [r7, #2456]
	bl	FSP_ClearLogicFs
	str	r4, [r6, #1008]
	str	r4, [r6, #1012]
	ldr	r0, [r7, #2456]
	bl	FSP_GetNullLogicFs
	add	r1, r5, #46080
	add	r1, r1, #40
	str	r0, [r6, #1012]
	str	r0, [r6, #1008]
	strb	r4, [r8, #3173]
	ldr	r0, [r7, #2456]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L18:
	ldr	r3, .L19
	movw	r2, #537
	ldr	r1, .L19+16
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	b	.L12
.L20:
	.align	2
.L19:
	.word	vfmw_Osal_Func_Ptr_S
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
	mov	r4, r2
	ldr	r2, [r2, #24]
	mov	r5, r1
	ldr	r3, [r4, #20]
	cmp	r2, #1
	bls	.L23
	cmp	r3, #1
	bls	.L23
	ldr	r1, [r1]
	ldr	r3, [r1, #776]
	cmp	r3, #0
	ldr	r3, [r1, #28]
	bne	.L26
	cmp	r3, #25
	beq	.L50
.L27:
	ldr	r7, [r4, #16]
	mov	r9, #5
	ldr	r10, [r4, #12]
	mov	r6, #2
	mov	r3, #4
	str	r3, [fp, #-88]
.L32:
	ldr	r8, .L56
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r0, [r4, #12]
	ldr	r1, [r5]
	mov	r3, #0
	ldr	r2, [r4, #16]
	str	r0, [fp, #-48]
	ldr	r1, [r1, #28]
	str	r7, [fp, #-68]
	cmp	r1, #24
	str	r10, [fp, #-64]
	str	r2, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L38
	ldrb	r3, [fp, #-88]	@ zero_extendqisi2
	uxtb	r6, r6
	strb	r9, [fp, #-70]
	strb	r3, [fp, #-72]
.L40:
	add	r7, r5, #77824
	sub	r1, fp, #76
	strb	r6, [fp, #-69]
	mov	r4, #0
	ldr	r0, [r7, #2456]
	strb	r4, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r6, r0, #0
	beq	.L51
.L48:
	mov	r0, #0
.L45:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L26:
	cmp	r3, #25
	bne	.L27
	ldr	r2, [r4, #16]
	ldr	r0, [r1, #736]
	cmp	r2, r0
	bhi	.L52
	ldr	r3, [r4, #12]
	ldr	ip, [r1, #740]
	cmp	r3, ip
	bhi	.L35
	ldr	r0, [r1, #764]
	mov	r10, r3
	ldr	r6, [r1, #760]
	mov	r7, r2
	cmp	r0, #0
	add	r3, r6, #2
	str	r3, [fp, #-88]
	bne	.L49
.L43:
	mov	r6, #1
.L37:
	ldrb	r9, [r1, #784]	@ zero_extendqisi2
	b	.L32
.L23:
	ldr	ip, .L56
	mov	r0, #0
	ldr	r1, .L56+4
	ldr	ip, [ip, #68]
	blx	ip
	b	.L48
.L51:
	ldr	r1, [r5]
	sub	r3, fp, #80
	ldr	r0, [r7, #2456]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_PartitionFsMemory
	cmp	r0, r4
	bne	.L53
	add	r5, r5, #8192
	mov	r3, #1
	mov	r0, r3
	strb	r3, [r5, #3176]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L50:
	ldr	r2, [r4, #16]
	ldr	r7, [r1, #736]
	cmp	r2, r7
	bhi	.L54
	ldr	r3, [r4, #12]
	ldr	r10, [r1, #740]
	cmp	r3, r10
	bhi	.L55
	add	r3, r5, #8192
	ldrb	r0, [r3, #3176]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L45
	ldr	r3, [r1, #764]
	ldr	r6, [r1, #760]
	cmp	r3, #0
	add	r3, r6, #2
	str	r3, [fp, #-88]
	beq	.L43
.L49:
	add	r6, r6, #1
	cmp	r6, #17
	movge	r6, #17
	b	.L37
.L38:
	strb	r3, [fp, #-72]
	mov	r6, #1
	strb	r9, [fp, #-70]
	b	.L40
.L52:
	ldr	ip, [r1, #740]
	ldr	r3, [r4, #12]
.L35:
	ldr	r1, .L56
	stmia	sp, {r0, ip}
.L47:
	ldr	ip, [r1, #68]
	mov	r0, #0
	ldr	r1, .L56+8
	blx	ip
	b	.L48
.L53:
	ldr	r3, [r8, #68]
	mov	r0, r6
	ldr	r1, .L56+12
	blx	r3
	mov	r0, r6
	b	.L45
.L55:
	mov	ip, r10
.L29:
	ldr	r1, .L56
	stmia	sp, {r7, ip}
	b	.L47
.L54:
	ldr	ip, [r1, #740]
	ldr	r3, [r4, #12]
	b	.L29
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #45056
	add	r3, r0, #77824
	mov	r5, r0
	ldr	r1, [r2, #1016]
	ldr	r0, [r3, #2456]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r2, r5, #40960
	mov	ip, #0
	mov	r1, ip
	ldr	r3, [r2, #3768]
	str	ip, [r4, #148]
	and	r3, r3, #3
	str	r3, [r4, #152]
	orr	ip, r3, #7168
	str	r3, [r4, #156]
	orr	ip, ip, #32
	str	ip, [r4, #160]
	ldr	r3, [r2, #3776]
	str	r3, [r4, #164]
	ldr	r3, [r2, #3772]
	str	r3, [r4, #168]
	bl	SetAspectRatio
	ldr	r3, [r5]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L198
.L194:
	mov	r2, #9
	mov	r3, #0
	str	r2, [r4, #384]
	mov	r2, #1
	strb	r3, [r4, #372]
	mov	r3, #8
	strb	r2, [r4, #379]
	strb	r2, [r4, #377]
	str	r3, [r4, #388]
	str	r3, [r4, #396]
	str	r3, [r4, #392]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L198:
	ldr	r3, [r4, #168]
	ldr	r1, [r4, #192]
	add	r3, r3, #15
	ldr	r0, [r4, #104]
	ldr	ip, [r4, #32]
	mov	r2, r3, lsr #4
	mov	r3, r1, asl #4
	add	lr, r0, r1
	str	r0, [r4, #88]
	mul	r3, r2, r3
	str	r0, [r4, #120]
	str	lr, [r4, #96]
	str	ip, [r4, #48]
	add	r0, r3, r0
	add	r3, ip, r3
	add	r1, r0, r1
	str	r3, [r4, #36]
	str	r0, [r4, #108]
	str	r0, [r4, #92]
	str	r1, [r4, #100]
	bl	MEM_Phy2Vir
	ldr	r3, [r4, #92]
	str	r3, [r4, #124]
	str	r0, [r4, #52]
	b	.L194
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
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r2, #44
	add	r3, r0, #40960
	mla	r2, r2, r1, r0
	ldr	r4, [r3, #3916]
	add	r1, r1, #1
	add	ip, r2, #104
	add	lr, r2, #112
	str	r4, [r2, #84]
	ldr	r4, [r3, #3908]
	str	r4, [r2, #92]
	ldr	r4, [r3, #3912]
	str	r4, [r2, #100]
	ldr	r4, [r3, #3928]
	str	r4, [r2, #88]
	ldr	r4, [r3, #3920]
	str	r4, [r2, #96]
	ldr	r4, [r3, #3924]
	str	r4, [r2, #104]
	ldr	r2, [r3, #3896]
	str	r2, [ip, #4]
	ldr	r2, [r3, #3892]
	str	r2, [ip, #8]
	ldr	r2, [r3, #3888]
	str	r2, [lr, #4]
	ldr	r3, [r3, #3900]
	str	r3, [lr, #8]
	str	r1, [r0, #40]
	ldmfd	sp, {r4, fp, sp, pc}
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
	add	r3, r0, #40960
	add	r2, r0, #45056
	ldr	r1, [r3, #3768]
	cmp	r1, #2
	strb	r1, [r0, #9]
	ldr	r1, [r3, #3784]
	str	r1, [r0, #12]
	ldr	r1, [r3, #3780]
	str	r1, [r0, #16]
	ldr	r1, [r2, #932]
	str	r1, [r0, #20]
	ldr	r2, [r2, #936]
	str	r2, [r0, #24]
	ldr	r2, [r3, #3808]
	str	r2, [r0, #28]
	ldr	r1, [r3, #3932]
	str	r1, [r0, #32]
	ldr	r1, [r3, #3936]
	str	r1, [r0, #36]
	strne	r2, [r3, #3932]
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
	ldr	r2, [r0, #120]
	ldr	r3, [r0, #12]
	cmp	r2, #0
	ldr	ip, [r0, #16]
	bne	.L211
	mul	ip, r3, ip
	ldr	r5, [r0, #40]
	cmp	r5, #2
	sub	ip, ip, #1
	bls	.L210
	ldr	r3, [r0, #164]
	sub	lr, r5, #1
	cmp	r3, #0
	cmpgt	ip, r3
	ble	.L211
	mov	r2, r0
	mov	r3, #1
	b	.L212
.L213:
	ldr	r1, [r2, #208]
	add	r2, r2, #44
	cmp	ip, r1
	cmpgt	r1, #0
	ble	.L211
.L212:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L213
.L210:
	cmp	r5, #1
	bls	.L214
	sub	r4, r5, #1
	mov	r3, r0
	mov	r2, #0
	b	.L215
.L216:
	ldr	lr, [r3, #120]
	add	r3, r3, #44
	add	r2, r2, #1
	ldr	r1, [r3, #120]
	cmp	lr, r1
	bge	.L211
.L215:
	cmp	r2, r4
	bne	.L216
	mov	r3, #44
	mla	r2, r3, r2, r0
	ldr	r3, [r2, #120]
	cmp	ip, r3
	cmpge	r3, #0
	ble	.L211
.L214:
	cmp	r5, #0
	beq	.L217
	sub	r7, r5, #1
	mov	r6, r0
	mov	lr, r0
	mov	r4, #0
	b	.L220
.L235:
	cmp	r4, r5
	beq	.L234
.L220:
	cmp	r4, r7
	streq	ip, [lr, #124]
	add	r4, r4, #1
	add	lr, lr, #44
	ldrne	r3, [lr, #120]
	moveq	r1, ip
	subne	r1, r3, #1
	ldr	r3, [lr, #76]
	strne	r1, [lr, #80]
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
	beq	.L235
.L211:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L234:
	ldr	r2, [r0, #84]
	mov	r1, r2
	b	.L223
.L236:
	ldr	r1, [r6, #84]
.L223:
	cmp	r2, r1
	ldr	ip, [r6, #88]
	add	r3, r3, #1
	movcs	r2, r1
	cmp	ip, r2
	bcs	.L221
	ldr	r1, [r6, #104]
	cmp	r1, #0
	movne	r2, ip
.L221:
	cmp	r5, r3
	add	r6, r6, #44
	bne	.L236
	bic	r2, r2, #15
	ldr	lr, [r0, #40]
	str	r2, [r0, #48]
	mov	r3, r0
	mov	ip, #0
	b	.L227
.L237:
	ldr	r4, [r3, #92]
	rsb	r2, r2, r1
	and	r1, r4, #7
	add	r2, r2, r4, lsr #3
	and	r4, r2, #15
	bic	r2, r2, #15
	str	r2, [r3, #84]
	add	r2, r1, r4, lsl #3
	str	r2, [r3, #92]
.L225:
	add	ip, ip, #1
	add	r3, r3, #44
	cmp	lr, ip
	bls	.L226
	ldr	r2, [r0, #48]
.L227:
	ldr	r1, [r3, #88]
	cmp	r1, #0
	rsb	r4, r2, r1
	ldr	r1, [r3, #84]
	beq	.L237
	ldr	r5, [r3, #92]
	rsb	r2, r2, r1
	ldr	r7, [r3, #96]
	and	r6, r5, #7
	add	r2, r2, r5, lsr #3
	and	r5, r7, #7
	add	r1, r4, r7, lsr #3
	and	r4, r2, #15
	and	r7, r1, #15
	bic	r2, r2, #15
	add	r4, r6, r4, lsl #3
	str	r2, [r3, #84]
	bic	r1, r1, #15
	add	r2, r5, r7, lsl #3
	str	r4, [r3, #92]
	str	r2, [r3, #96]
	str	r1, [r3, #88]
	b	.L225
.L217:
	ldr	r3, [r0, #84]
	bic	r3, r3, #15
	str	r3, [r0, #48]
.L226:
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
	ldr	r7, .L242
	movw	r2, #14752
	mov	r6, r1
	movt	r2, 1
	mov	r1, #0
	mov	r4, r0
	ldr	r3, [r7, #48]
	mov	r5, r1
	blx	r3
	add	r0, r4, #46080
	add	r0, r0, #40
	bl	ResetVoQueue
	add	r3, r4, #40960
	str	r6, [r4]
	mov	r2, #15
	str	r2, [r3, #3932]
	add	ip, r4, #8192
	str	r2, [r3, #3936]
	mov	r1, #8192
	ldr	lr, [r6, #8]
	mov	r2, #1
	strb	r5, [ip, #3176]
	mov	r0, r4
	add	r4, r4, #77824
	strb	lr, [ip, #3172]
	str	r1, [r3, #3792]
	str	r2, [r3, #3764]
	strb	r2, [r3, #3760]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #2456]
	beq	.L241
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L241:
	ldr	r3, [r7, #68]
	mov	r0, r5
	ldr	r1, .L242+4
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L243:
	.align	2
.L242:
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
	ldr	r3, .L245
	add	r2, r0, #45056
	ldr	r1, .L245+4
	mov	r0, #2
	ldr	r2, [r2, #908]
	ldr	r3, [r3, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L246:
	.align	2
.L245:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC11
	UNWIND(.fnend)
	.size	REAL9DEC_Destroy, .-REAL9DEC_Destroy
	.global	__aeabi_idiv
	.align	2
	.global	REAL9DEC_Decode
	.type	REAL9DEC_Decode, %function
REAL9DEC_Decode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 200
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #220)
	sub	sp, sp, #220
	add	r4, r0, #40960
	mov	r6, r0
	mov	r5, r1
	mov	r2, #128
	mov	r1, #0
	sub	r0, fp, #172
	add	r3, r6, #8
	str	r3, [fp, #-196]
	bl	memset
	ldr	r2, [r4, #3868]
	cmp	r5, #0
	ldr	r3, [r4, #3872]
	mov	r0, #1
	mov	r1, #8192
	str	r0, [r4, #3764]
	str	r2, [r4, #3876]
	str	r2, [fp, #-56]
	str	r2, [fp, #-64]
	str	r3, [r4, #3880]
	str	r3, [fp, #-52]
	str	r3, [fp, #-60]
	str	r0, [fp, #-168]
	str	r1, [r4, #3792]
	str	r1, [fp, #-140]
	beq	.L379
	ldr	r3, .L398
	add	r7, r6, #45056
	add	ip, r6, #8192
	movw	r2, #11344
	mov	r1, #0
	ldr	r0, [fp, #-196]
	ldr	r3, [r3, #48]
	mov	r9, ip
	mov	r8, r1
	str	ip, [fp, #-200]
	blx	r3
	str	r8, [r7, #872]
	ldr	r3, [r5, #12]
	str	r3, [r7, #892]
	ldr	r3, [r5, #8]
	str	r3, [r7, #876]
	ldr	r3, [r5]
	str	r3, [r9, #3160]
	ldr	r2, [r5, #12]
	str	r2, [r7, #880]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	strb	r3, [r9, #3164]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L250
	ldr	r3, [r5, #88]
	str	r3, [r7, #884]
	ldr	r3, [r5, #80]
	str	r3, [r9, #3168]
	ldr	r3, [r5, #92]
	str	r3, [r7, #888]
	ldrb	r3, [r5, #85]	@ zero_extendqisi2
	strb	r3, [r9, #3165]
	ldrb	r3, [r5, #85]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L380
.L251:
	movw	r3, #65525
	sub	r1, r2, #10
	movt	r3, 47
	cmp	r1, r3
	bhi	.L373
	ldr	r9, [fp, #-200]
	ldr	r1, [r9, #3160]
	cmp	r1, #0
	beq	.L373
	add	r5, r6, #46080
	add	r5, r5, #8
	mov	r0, r5
	bl	BsInit
	mov	r1, #8
	mov	r0, r5
	bl	BsShow
	mov	r1, #8
	mov	r8, r0
	mov	r0, r5
	bl	BsSkip
	cmp	r8, #255
	add	r0, r8, #1
	str	r0, [r4, #3940]
	bhi	.L373
	cmp	r0, #0
	beq	.L254
	add	r10, r6, #44800
	movw	r3, #65535
	add	r10, r10, #100
	movt	r3, 47
	str	r6, [fp, #-192]
	mov	r8, #0
	mov	r6, r10
	str	r7, [fp, #-200]
	mov	r10, r3
	b	.L255
.L382:
	ldr	r0, [r4, #3940]
	cmp	r0, r8
	bls	.L381
.L255:
	mov	r1, #32
	mov	r0, r5
	bl	BsSkip
	mov	r1, #32
	mov	r0, r5
	add	r8, r8, #1
	bl	BsShow
	mov	r1, #32
	mov	r7, r0
	mov	r0, r5
	bl	BsSkip
	rev	r0, r7
	cmp	r0, r10
	str	r0, [r6, #4]!
	ble	.L382
.L373:
	mov	r3, #0
	str	r3, [fp, #-192]
.L361:
	ldr	r0, [fp, #-192]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L380:
	add	r6, r6, #77824
	ldr	r1, [r5, #16]
	str	r8, [fp, #-192]
	ldr	r0, [r6, #2456]
	bl	SM_ReleaseStreamSeg
	ldr	r0, [r6, #2456]
	ldr	r1, [r5, #96]
	bl	SM_ReleaseStreamSeg
	ldr	r0, [fp, #-192]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L250:
	ldr	r3, [fp, #-200]
	str	r8, [r7, #884]
	str	r8, [r3, #3168]
	str	r8, [r7, #888]
	strb	r8, [r3, #3165]
	b	.L251
.L379:
	ldr	r3, .L398
	add	r6, r6, #45056
	ldr	r1, .L398+4
	ldr	r2, [r6, #908]
	str	r5, [fp, #-192]
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r0, [fp, #-192]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L381:
	ldr	r6, [fp, #-192]
	ldr	r7, [fp, #-200]
	str	r9, [fp, #-200]
.L254:
	add	r3, r6, #44800
	sub	lr, r0, #1
	add	r3, r3, #104
	mov	r2, #0
	ldr	r9, [fp, #-200]
	b	.L256
.L257:
	ldr	ip, [r3, #4]!
	ldr	r1, [r3, #-4]
	cmp	ip, r1
	ble	.L373
.L256:
	cmp	r2, lr
	add	r2, r2, #1
	bne	.L257
	mov	r0, r0, asl #3
	ldr	r10, [r4, #3944]
	add	r2, r0, #1
	movw	r3, #65534
	cmp	r2, #3145728
	add	r10, r2, r10
	sub	r8, r10, #1
	str	r2, [fp, #-204]
	movt	r3, 47
	movlt	r2, #0
	movge	r2, #1
	cmp	r8, r3
	movls	ip, r2
	orrhi	ip, r2, #1
	cmp	ip, #0
	str	r9, [fp, #-200]
	str	ip, [fp, #-208]
	bne	.L373
	ldr	r2, [r7, #880]
	rsb	r2, r10, r2
	cmp	r2, #0
	ble	.L373
	ldr	r1, [r9, #3160]
	mov	r0, r5
	add	r1, r1, r10
	bl	BsInit
	sub	r1, fp, #172
	mov	r0, r5
	bl	Real9_CB_GetPictureHeader
	ldr	ip, [fp, #-208]
	cmp	r0, #1
	str	r0, [fp, #-192]
	bne	.L383
	ldr	r3, [fp, #-200]
	ldrb	r3, [r3, #3176]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L259
	ldr	r2, [fp, #-148]
	ldr	r3, [r4, #3784]
	cmp	r2, r3
	beq	.L384
.L259:
	ldr	r3, .L398+8
	ldr	ip, [r3]
	cmp	ip, #0
	addeq	r3, r6, #77824
	streq	r3, [fp, #-208]
	beq	.L261
	ldr	r0, [fp, #-148]
	add	r3, r6, #77824
	ldr	r1, [r4, #3780]
	sub	r2, fp, #180
	ldr	lr, [r4, #3784]
	mov	r8, r3
	strh	r0, [fp, #-176]	@ movhi
	ldrh	r0, [fp, #-152]
	str	r3, [fp, #-208]
	mov	r3, #8
	strh	r1, [fp, #-178]	@ movhi
	mov	r1, #2
	strh	r0, [fp, #-174]	@ movhi
	strh	lr, [fp, #-180]	@ movhi
	ldr	r0, [r8, #2456]
	blx	ip
.L261:
	ldr	r8, [fp, #-208]
	add	ip, r6, #46080
	mov	r2, #1
	ldr	r1, [r7, #1012]
	mov	r9, ip
	ldr	r0, [r8, #2456]
	bl	FSP_ClearLogicFs
	add	r2, r9, #40
	ldr	r1, [r7, #1008]
	mov	r9, r2
	ldr	r0, [r8, #2456]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r3, #0
	str	r3, [r7, #1008]
	str	r3, [r7, #1012]
	ldr	r0, [r8, #2456]
	str	r3, [fp, #-212]
	bl	FSP_GetNullLogicFs
	ldr	r3, [fp, #-212]
	mov	r1, r9
	ldr	r2, [fp, #-200]
	str	r0, [r7, #1012]
	str	r0, [r7, #1008]
	strb	r3, [r2, #3173]
	ldr	r0, [r8, #2456]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [fp, #-148]
	ldr	r3, [fp, #-152]
	cmp	r2, #512
	bhi	.L263
	cmp	r3, #512
	bhi	.L263
	ldr	r3, [r6]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L265
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L385
.L265:
	ldr	r3, [fp, #-208]
	ldr	r0, [r3, #2456]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L368
.L267:
	ldr	r3, .L398
	mov	r0, #1
	ldr	r2, [fp, #-148]
	ldr	r1, .L398+12
	ldr	ip, [r3, #68]
	ldr	r3, [fp, #-152]
	blx	ip
	sub	r2, fp, #172
	mov	r1, r6
	mov	r0, #6
	bl	Real9_ArrangeVHBMem
	subs	r3, r0, #0
	beq	.L386
.L260:
	add	r3, r6, #44544
	sub	r1, fp, #172
	add	r3, r3, #176
	str	r3, [fp, #-220]
	mov	r2, #128
	mov	r0, r3
	ldr	r3, .L398
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r3, [r6]
	ldr	r2, [fp, #-200]
	mov	r0, r6
	ldr	r3, [r3, #8]
	strb	r3, [r2, #3172]
	bl	Real9_GetImageBuffer
	subs	r2, r0, #0
	beq	.L387
	add	r3, r6, #77824
	ldr	r1, [r7, #1016]
	str	r3, [fp, #-208]
	ldr	r0, [r3, #2456]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L374
	ldr	r2, [r6]
	mov	r0, #29
	ldr	ip, .L398
	ldr	r1, .L398+16
	ldrd	r8, [r2, #48]
	strd	r8, [r3, #8]
	ldr	r2, [r6]
	ldrd	r8, [r2, #64]
	strd	r8, [r3, #16]
	ldr	r2, [r6]
	ldrd	r8, [r2, #80]
	strd	r8, [r3, #24]
	mvn	r8, #0
	ldr	r2, [r6]
	mvn	r9, #0
	ldr	r2, [r2, #96]
	str	r2, [r3, #68]
	ldr	r2, [r6]
	ldr	r2, [r2, #100]
	str	r2, [r3, #72]
	ldr	r2, [r6]
	ldr	r2, [r2, #104]
	str	r2, [r3, #76]
	ldr	r2, [r6]
	ldr	r2, [r2, #120]
	str	r2, [r3, #80]
	ldr	r2, [r6]
	strd	r8, [r2, #48]
	ldrd	r8, [r3, #16]
	ldr	ip, [ip, #68]
	ldrd	r2, [r3, #8]
	strd	r8, [sp]
	blx	ip
	ldr	r3, [r4, #3768]
	cmp	r3, #2
	beq	.L376
	cmp	r3, #1
	ldr	r3, [fp, #-200]
	ldrb	r3, [r3, #3173]	@ zero_extendqisi2
	beq	.L388
	cmp	r3, #0
	movne	r3, #2
	moveq	r3, #1
.L274:
	ldr	r2, [fp, #-200]
	strb	r3, [r2, #3173]
	ldr	r2, [r7, #1008]
	ldr	r3, [r7, #1012]
	cmp	r2, r3
	beq	.L275
	ldr	r3, .L398
	mov	r0, #13
	ldr	r1, .L398+20
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [fp, #-208]
	mov	r2, #0
	ldr	r1, [r7, #1008]
	ldr	r0, [r3, #2456]
	bl	FSP_SetRef
.L275:
	ldr	r3, [r7, #1012]
	mov	r2, #1
	ldr	r1, [r7, #1016]
	ldr	r8, [fp, #-208]
	str	r3, [r7, #1008]
	str	r1, [r7, #1012]
	ldr	r0, [r8, #2456]
	bl	FSP_SetRef
	ldr	r1, [r7, #1008]
	ldr	r0, [r8, #2456]
	bl	FSP_RemovePmv
	ldr	r3, .L398
	ldr	r2, [r7, #1016]
	mov	r0, #13
	ldr	r1, .L398+24
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [fp, #-200]
	ldrb	r3, [r3, #3172]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L277
	ldr	r3, [r4, #3768]
	cmp	r3, #0
	beq	.L369
	cmp	r3, #2
	beq	.L318
	cmp	r3, #1
	beq	.L369
.L277:
	ldr	r8, [fp, #-208]
	ldr	r1, [r7, #1016]
	ldr	r0, [r8, #2456]
	bl	FSP_GetLogicFs
	ldr	r1, [r7, #1008]
	str	r0, [fp, #-212]
	ldr	r0, [r8, #2456]
	bl	FSP_GetLogicFs
	ldr	r1, [r7, #1012]
	mov	r9, r0
	str	r0, [fp, #-224]
	ldr	r0, [r8, #2456]
	bl	FSP_GetLogicFs
	ldr	ip, [fp, #-212]
	mov	r3, r9
	cmp	ip, #0
	cmpne	r9, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	str	r0, [fp, #-216]
	cmp	r3, #0
	bne	.L389
	ldr	r3, [ip, #520]
	cmp	r3, #0
	beq	.L283
	ldr	r2, [ip, #528]
	cmp	r2, #0
	beq	.L283
	ldr	r2, [ip, #524]
	cmp	r2, #0
	beq	.L283
	ldr	r2, [fp, #-216]
	mov	r0, #13
	ldr	r1, .L398+28
	ldr	r2, [r2, #520]
	cmp	r2, #0
	ldrne	r3, [r2, #12]
	ldreq	r3, [r3, #12]
	str	r3, [fp, #-212]
	ldr	r3, [fp, #-212]
	str	r3, [r6, #52]
	ldr	r3, [fp, #-224]
	ldr	r3, [r3, #520]
	cmp	r3, #0
	ldreq	r3, [ip, #520]
	ldr	lr, [r3, #12]
	str	ip, [fp, #-228]
	str	lr, [r6, #56]
	ldr	r3, [ip, #520]
	ldr	r2, [r3, #12]
	str	r2, [r6, #60]
	ldr	r3, [ip, #532]
	str	r3, [r6, #76]
	mov	r8, r3
	ldr	r3, [fp, #-216]
	ldr	r3, [r3, #532]
	str	r3, [r6, #80]
	mov	r9, r3
	ldr	r3, [ip, #524]
	ldr	ip, [fp, #-212]
	ldr	r3, [r3, #12]
	str	r8, [sp, #8]
	str	r9, [sp, #12]
	str	r3, [r6, #64]
	str	ip, [sp, #4]
	ldr	ip, .L398
	str	lr, [sp]
	ldr	ip, [ip, #68]
	blx	ip
	ldr	ip, [fp, #-228]
	ldr	r3, [ip, #524]
	ldr	r3, [r3, #20]
	str	r3, [r6, #68]
	ldr	r3, [ip, #524]
	ldr	r3, [r3, #20]
	str	r3, [r7, #980]
	ldr	r3, [r4, #3768]
	cmp	r3, #2
	beq	.L390
.L289:
	ldr	r3, [r4, #3796]
	mov	r0, r5
	str	r3, [ip, #540]
	ldr	r3, [r4, #3768]
	ldr	r2, [r4, #3808]
	cmp	r3, #2
	strb	r3, [r6, #9]
	ldr	r3, [r4, #3784]
	str	r2, [fp, #-212]
	str	r3, [r6, #12]
	ldr	r3, [r4, #3780]
	str	r3, [r6, #16]
	ldr	r3, [r7, #932]
	str	r3, [r6, #20]
	ldr	r3, [r7, #936]
	str	r2, [r6, #28]
	str	r3, [r6, #24]
	ldr	r3, [r4, #3932]
	str	r3, [r6, #32]
	ldr	r3, [r4, #3936]
	str	r3, [r6, #36]
	strne	r2, [r4, #3932]
	bl	BsPos
	ldr	r1, [r4, #3940]
	cmp	r1, #1
	ldreq	r1, [fp, #-204]
	mov	r3, r0, asr #31
	mov	r2, r3, lsr #29
	add	r3, r0, r2
	moveq	r0, r5
	and	r3, r3, #7
	movne	r0, r5
	rsb	r3, r2, r3
	str	r3, [r4, #3908]
	ldreq	r3, [r7, #880]
	ldreq	r2, [r4, #3944]
	ldrne	r2, [r4, #3944]
	rsbeq	r3, r1, r3
	ldrne	r3, [r4, #3948]
	rsb	r3, r2, r3
	mov	r3, r3, asl #3
	str	r3, [fp, #-216]
	bl	BsPos
	ldr	r3, [fp, #-216]
	rsb	r0, r0, r3
	str	r0, [r4, #3912]
	ldr	r3, [r7, #876]
	mov	r0, r5
	add	r10, r10, r3
	bl	BsPos
	ldr	r2, [r4, #3812]
	mov	r1, #1
	ldr	ip, [r4, #3804]
	ldr	lr, [fp, #-212]
	str	r2, [r4, #3892]
	str	ip, [r4, #3896]
	str	lr, [r4, #3888]
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	mov	r3, #0
	add	r0, r10, r0, asr #3
	str	r3, [r4, #3920]
	str	r0, [r4, #3916]
	str	r3, [r4, #3924]
	str	r3, [r4, #3928]
	str	r3, [r4, #3900]
	str	r0, [r6, #84]
	ldr	r0, [r4, #3908]
	str	r0, [r6, #92]
	ldr	r0, [r4, #3912]
	str	ip, [r6, #108]
	str	r3, [r6, #88]
	str	r0, [r6, #100]
	str	r3, [r6, #96]
	str	r3, [r6, #104]
	str	r2, [r6, #112]
	str	r3, [r6, #120]
	str	lr, [r6, #116]
	str	r1, [r6, #40]
	ldr	r2, [r4, #3940]
	cmp	r2, r1
	bls	.L314
	ldr	r10, [r4, #3948]
	movw	r0, #65535
	ldr	r2, [fp, #-204]
	movt	r0, 47
	add	r10, r2, r10
	cmp	r10, r0
	bgt	.L375
	cmp	r10, #0
	ble	.L375
	ldr	r2, [r7, #880]
	rsb	r2, r10, r2
	cmp	r2, #0
	ble	.L375
	add	ip, r6, #44800
	str	r6, [fp, #-216]
	add	ip, ip, #108
	mov	r6, r4
	str	r3, [fp, #-240]
	mov	r8, r1
	str	r0, [fp, #-224]
	mov	r4, ip
	str	r1, [fp, #-228]
	ldr	r9, [fp, #-200]
	b	.L305
.L306:
	ldr	r3, [r6, #3940]
	add	r8, r8, #1
	cmp	r3, r8
	bls	.L391
	ldr	r3, [fp, #-204]
	ldr	r10, [r4, #4]!
	add	r10, r3, r10
	ldr	r3, [fp, #-224]
	cmp	r10, r3
	bgt	.L375
	cmp	r10, #0
	ble	.L375
	ldr	r2, [r7, #880]
	rsb	r2, r10, r2
	cmp	r2, #0
	ble	.L375
.L305:
	ldr	r1, [r9, #3160]
	mov	r0, r5
	add	r1, r1, r10
	bl	BsInit
	ldr	r3, [fp, #-220]
	sub	r2, fp, #188
	mov	r1, #0
	mov	r0, r5
	str	r3, [sp, #4]
	sub	r3, fp, #180
	str	r3, [sp]
	sub	r3, fp, #184
	bl	Real9_CB_GetSliceHeader
	cmp	r0, #1
	bne	.L306
	ldr	r3, [r6, #3784]
	ldr	r2, [fp, #-188]
	cmp	r3, #0
	str	r2, [fp, #-200]
	beq	.L392
	ldr	r3, [fp, #-180]
	mov	r0, r5
	ldr	r2, [fp, #-212]
	cmp	r3, #0
	movge	r2, r3
	str	r2, [fp, #-212]
	bl	BsPos
	ldr	r2, [r6, #3940]
	sub	lr, r2, #1
	cmp	lr, r8
	mov	r3, r0, asr #31
	mov	r1, r3, lsr #29
	add	r3, r0, r1
	and	r3, r3, #7
	rsb	r3, r1, r3
	str	r3, [r6, #3908]
	beq	.L393
	cmp	r8, #254
	bhi	.L394
	ldr	r3, [r4, #4]
	mov	r0, r5
	ldr	r2, [r4]
.L372:
	rsb	r3, r2, r3
	mov	r3, r3, asl #3
	str	r3, [fp, #-232]
	bl	BsPos
	ldr	r3, [fp, #-232]
	rsb	r0, r0, r3
	str	r0, [r6, #3912]
	ldr	r3, [r7, #876]
	mov	r0, r5
	add	r10, r10, r3
	bl	BsPos
	ldr	lr, [fp, #-228]
	mov	r3, #44
	ldr	r1, [fp, #-216]
	ldr	ip, [r6, #3812]
	mla	r3, r3, lr, r1
	add	lr, lr, #1
	str	lr, [fp, #-228]
	ldr	r1, [r6, #3804]
	str	ip, [r6, #3892]
	str	r1, [r6, #3896]
	add	r2, r0, #7
	cmp	r0, #0
	movlt	r0, r2
	cmp	lr, #131
	ldr	lr, [fp, #-212]
	add	r2, r10, r0, asr #3
	ldr	r0, [fp, #-240]
	add	r10, r3, #84
	str	r2, [r6, #3916]
	str	lr, [r6, #3888]
	ldr	lr, [fp, #-200]
	str	r0, [r6, #3920]
	str	r0, [r6, #3924]
	str	lr, [r6, #3900]
	str	r0, [r6, #3928]
	str	r2, [r3, #84]
	ldr	r2, [r6, #3908]
	str	r10, [fp, #-232]
	str	r2, [r3, #92]
	add	r2, r3, #100
	ldr	r10, [r6, #3912]
	str	r2, [fp, #-236]
	ldr	lr, [fp, #-236]
	str	r10, [r3, #100]
	ldr	r2, [fp, #-212]
	ldr	r10, [fp, #-232]
	str	r0, [r10, #4]
	str	r0, [r3, #96]
	str	r0, [lr, #4]
	str	r2, [r3, #116]
	ldr	r2, [fp, #-200]
	str	r1, [r3, #108]
	str	ip, [r3, #112]
	str	r2, [r3, #120]
	ldr	r1, [fp, #-216]
	ldr	lr, [fp, #-228]
	str	lr, [r1, #40]
	ble	.L306
	mov	r0, r1
	bl	Real9_ModifySliceMsg
	cmp	r0, #1
	bne	.L375
	ldr	r3, [fp, #-216]
	ldr	r2, [fp, #-196]
	str	r2, [r3, #4]
	b	.L306
.L383:
	ldr	r3, .L398
	mov	r0, #1
	ldr	r1, .L398+32
	str	ip, [fp, #-192]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L361
.L263:
	ldr	r1, .L398
	mov	lr, #0
	mov	r0, #18
	str	lr, [fp, #-192]
	ldr	ip, [r1, #68]
	ldr	r1, .L398+36
	blx	ip
	b	.L361
.L387:
	ldr	r3, .L398
	str	r2, [fp, #-192]
	ldr	r1, .L398+40
	ldr	r3, [r3, #68]
	ldr	r2, [r7, #908]
	blx	r3
	b	.L361
.L385:
	ldr	r3, [fp, #-200]
	ldrb	r3, [r3, #3176]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L267
.L368:
	mov	r0, r9
	bl	ResetVoQueue
	b	.L267
.L384:
	ldr	r2, [fp, #-152]
	ldr	r3, [r4, #3780]
	cmp	r2, r3
	bne	.L259
	b	.L260
.L386:
	ldr	r2, .L398
	mov	r0, #1
	ldr	r1, .L398+44
	str	r3, [fp, #-192]
	ldr	r2, [r2, #68]
	blx	r2
	b	.L361
.L375:
	ldr	r3, [fp, #-208]
	mov	r2, #1
	ldr	r1, [r7, #1016]
	ldr	r0, [r3, #2456]
	mov	r3, #0
	str	r3, [fp, #-192]
	bl	FSP_ClearLogicFs
	b	.L361
.L376:
	ldr	r3, [fp, #-200]
	ldrb	r3, [r3, #3173]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L375
	ldr	r3, [fp, #-200]
	ldrb	r3, [r3, #3172]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L277
.L318:
	ldr	r3, [r7, #1024]
	cmp	r3, #0
	bne	.L277
.L374:
	ldr	r0, [fp, #-208]
	mov	r2, #1
	ldr	r1, [r7, #1016]
	str	r3, [fp, #-192]
	ldr	r0, [r0, #2456]
	bl	FSP_ClearLogicFs
	b	.L361
.L369:
	str	r3, [r7, #1024]
	b	.L277
.L393:
	add	r2, r2, #8384
	ldr	r3, [r7, #880]
	add	r2, r2, #3
	ldr	r1, [fp, #-204]
	mov	r0, r5
	add	r2, r9, r2, lsl #2
	rsb	r3, r1, r3
	ldr	r2, [r2, #3160]
	b	.L372
.L388:
	cmp	r3, #0
	movne	r3, #2
	bne	.L274
	b	.L373
.L389:
	ldr	r3, .L398
	mov	r0, #0
	ldr	r2, .L398+48
	ldr	r1, .L398+52
	ldr	ip, [r3, #68]
	movw	r3, #1966
	str	r0, [fp, #-192]
	blx	ip
	b	.L361
.L391:
	ldr	r6, [fp, #-216]
	str	r9, [fp, #-200]
.L314:
	ldr	r4, [r6, #4]
	mov	r5, #1
	str	r5, [r6, #44]
	cmp	r4, #0
	beq	.L395
	ldr	r3, [r6]
	ldr	r2, [r3, #884]
	cmp	r2, #1
	beq	.L319
.L316:
	cmp	r2, #2
	beq	.L396
.L317:
	ldr	r3, [r3, #676]
	ldr	r2, [fp, #-200]
	str	r3, [r2, #3156]
	b	.L361
.L390:
	ldr	r3, [fp, #-224]
	ldr	r1, [fp, #-216]
	ldr	r2, [r3, #540]
	ldr	r3, [r4, #3796]
	ldr	r1, [r1, #540]
	subs	r3, r3, r2
	rsb	r2, r2, r1
	ldrmi	r1, [r4, #3792]
	addmi	r3, r3, r1
	cmp	r2, #0
	ldrlt	r1, [r4, #3792]
	addlt	r2, r2, r1
	cmp	r3, r2
	movgt	r3, #0
	cmp	r2, #0
	ble	.L321
	mov	r1, r2
	mov	r0, r3, asl #14
	str	ip, [fp, #-224]
	str	r2, [fp, #-216]
	str	r3, [fp, #-212]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-216]
	ldr	r3, [fp, #-212]
	mov	r1, r2
	str	r2, [fp, #-212]
	str	r3, [fp, #-216]
	mov	r8, r0
	rsb	r0, r3, r2
	mov	r0, r0, asl #14
	bl	__aeabi_idiv
	ldr	r2, [fp, #-212]
	ldr	r3, [fp, #-216]
	ldr	ip, [fp, #-224]
.L293:
	str	r3, [r7, #924]
	str	r2, [r7, #928]
	str	r8, [r7, #932]
	str	r0, [r7, #936]
	b	.L289
.L283:
	ldr	r3, [fp, #-208]
	mov	r2, #1
	ldr	r1, [r7, #1016]
	ldr	r0, [r3, #2456]
	mov	r3, #0
	str	r3, [fp, #-192]
	bl	FSP_ClearLogicFs
	ldr	r3, .L398
	mov	r0, #0
	ldr	r1, .L398+56
	ldr	r3, [r3, #68]
	blx	r3
	b	.L361
.L395:
	mov	r0, r6
	bl	Real9_ModifySliceMsg
	cmp	r0, #1
	bne	.L397
	ldr	r2, [fp, #-196]
	ldr	r3, [r6]
	str	r2, [r6, #4]
	ldr	r2, [r3, #884]
	cmp	r2, #1
	bne	.L316
	ldr	r2, [fp, #-196]
	cmp	r2, #0
	beq	.L317
	mov	r4, r2
.L319:
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L317
	ldr	r3, [fp, #-208]
	mov	r2, #0
	str	r2, [r6, #4]
	ldr	r1, [r7, #1016]
	ldr	r0, [r3, #2456]
	str	r2, [fp, #-192]
	bl	FSP_SetDisplay
	mov	r1, r6
	mov	r0, #9
	bl	ReleasePacket
	b	.L361
.L394:
	ldr	r3, .L398
	movw	r2, #2195
	ldr	r1, .L398+60
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	b	.L373
.L396:
	ldr	r2, [r3, #888]
	cmp	r2, #0
	ble	.L317
	ldr	r2, [r6, #4]
	cmp	r2, #0
	beq	.L317
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L317
	ldr	r3, [fp, #-208]
	mov	r2, #0
	str	r2, [r6, #4]
	ldr	r1, [r7, #1016]
	ldr	r0, [r3, #2456]
	str	r2, [fp, #-192]
	bl	FSP_SetDisplay
	ldr	r2, [r6]
	mov	r1, r6
	mov	r0, #9
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	bl	ReleasePacket
	b	.L361
.L321:
	mov	r0, #0
	mov	r8, r0
	b	.L293
.L392:
	mov	r2, r0
	ldr	r0, [fp, #-208]
	ldr	r1, [r7, #1016]
	str	r3, [fp, #-192]
	ldr	r0, [r0, #2456]
	bl	FSP_ClearLogicFs
	b	.L361
.L397:
	ldr	r3, [fp, #-208]
	mov	r2, r5
	ldr	r1, [r7, #1016]
	str	r4, [fp, #-192]
	ldr	r0, [r3, #2456]
	bl	FSP_ClearLogicFs
	b	.L361
.L399:
	.align	2
.L398:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
	.word	g_event_report
	.word	.LC15
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC23
	.word	.LC13
	.word	.LC14
	.word	.LC17
	.word	.LC16
	.word	.LANCHOR0+460
	.word	.LC21
	.word	.LC22
	.word	.LC24
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
	add	r4, r0, #77824
	mov	r2, #0
	ldr	r0, [r4, #2456]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #2456]
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, [r0]
	mov	r4, r0
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L418
	add	r7, r0, #45056
	add	r6, r0, #77824
	ldr	r1, [r7, #1012]
	ldr	r0, [r6, #2456]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L418
	ldr	r1, [r7, #1012]
	ldr	r0, [r6, #2456]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L417
.L418:
	add	r5, r4, #46080
	add	r5, r5, #40
.L408:
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	GetVoLastImageID
.L417:
	mov	r0, r4
	add	r5, r4, #46080
	bl	Real9_SetImgFormat
	add	r5, r5, #40
	mov	r3, #1
	str	r3, [r8, #244]
	mov	r2, r3
	ldr	r1, [r7, #1012]
	ldr	r0, [r6, #2456]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #2456]
	mov	r2, r4
	str	r8, [sp]
	mov	r3, r5
	mov	r1, #9
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L405
	ldr	r1, [r7, #1012]
	mov	r2, #0
	ldr	r0, [r6, #2456]
	bl	FSP_SetDisplay
	b	.L408
.L405:
	ldr	r3, [r8, #200]
	cmp	r3, #100
	bhi	.L408
	ldr	r3, [r8, #164]
	cmp	r3, #31
	bls	.L408
	ldr	r3, [r8, #168]
	cmp	r3, #31
	bls	.L408
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL9DEC_GetRemainImg, .-REAL9DEC_GetRemainImg
	.align	2
	.global	REAL9DEC_VDMPostProc
	.type	REAL9DEC_VDMPostProc, %function
REAL9DEC_VDMPostProc:
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
	ldr	r10, .L448
	mov	r6, r1
	mov	r4, r0
	ldr	r1, .L448+4
	mov	r9, r2
	ldr	r3, [r10, #68]
	mov	r2, r6
	mov	r0, #22
	add	r5, r4, #45056
	add	r7, r4, #77824
	blx	r3
	mov	r1, r4
	mov	r0, #9
	bl	ReleasePacket
	ldr	r1, [r5, #1016]
	ldr	r0, [r7, #2456]
	add	r8, r4, #8192
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L420
	add	r3, r4, #40960
	ldr	r2, [r3, #3768]
	cmp	r2, #2
	andne	r9, r9, #31
	strb	r2, [r8, #3174]
	strne	r9, [r3, #3936]
	cmp	r6, #0
	movlt	r6, #0
	blt	.L422
	cmp	r6, #100
	movge	r6, #100
.L422:
	ldr	r2, [r10, #68]
	str	r6, [r0, #200]
	mov	r0, #22
	ldr	r1, .L448+8
	str	r3, [fp, #-48]
	blx	r2
	mov	r0, r4
	bl	Real9_SetImgFormat
	ldrb	r2, [r8, #3172]	@ zero_extendqisi2
	ldr	r3, [fp, #-48]
	cmp	r2, #2
	beq	.L425
	ldr	r2, [r4]
	ldr	r2, [r2, #12]
	cmp	r2, #1
	beq	.L425
	ldr	r3, [r3, #3768]
	cmp	r3, #2
	beq	.L425
	ldrb	r3, [r8, #3173]	@ zero_extendqisi2
	cmp	r3, #1
	ldrhi	r1, [r5, #1008]
	mvnls	r1, #0
	cmn	r1, #1
	str	r1, [r5, #1020]
	beq	.L427
.L447:
	ldr	r0, [r7, #2456]
	bl	FSP_GetFsImagePtr
	ldr	r6, [r5, #1020]
	ldr	r9, [r10, #68]
	mov	r1, r6
	mov	r8, r0
	ldr	r0, [r7, #2456]
	bl	FSP_GetRef
	mov	r2, r6
	ldr	r1, .L448+12
	mov	r3, r0
	mov	r0, #12
	blx	r9
	mov	r2, #1
	ldr	r1, [r5, #1020]
	ldr	r0, [r7, #2456]
	bl	FSP_SetDisplay
	add	r3, r4, #46080
	ldr	r0, [r7, #2456]
	add	r3, r3, #40
	str	r8, [sp]
	mov	r2, r4
	mov	r1, #9
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L446
.L427:
	ldr	r3, [r5, #908]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r5, #908]
.L420:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L425:
	ldr	r1, [r5, #1016]
	cmn	r1, #1
	str	r1, [r5, #1020]
	bne	.L447
	b	.L427
.L446:
	ldr	r0, [r7, #2456]
	mov	r2, #0
	ldr	r1, [r5, #1020]
	bl	FSP_SetDisplay
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L449:
	.align	2
.L448:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC25
	.word	.LC26
	.word	.LC27
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
	add	r4, r0, #77824
	mov	r5, r0
	ldr	r0, [r4, #2456]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #2456]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L456
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L456:
	add	r1, r5, #46080
	ldr	r0, [r4, #2456]
	add	r1, r1, #40
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
	.type	__FUNCTION__.13835, %object
	.size	__FUNCTION__.13835, 16
__FUNCTION__.13835:
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
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC15:
	ASCII(.ascii	"new seq size %d * %d\012\000" )
	.space	2
.LC16:
	ASCII(.ascii	"Real9_ArrangeVHBMem err\012\000" )
	.space	3
.LC17:
	ASCII(.ascii	"Fatal Error: frm%d, get image buffer failed\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC19:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"++++++++++ ref %d\012\000" )
	.space	1
.LC21:
	ASCII(.ascii	"CurFsID abnormal!,%s %d (NULL == pCur) || (NULL == " )
	ASCII(.ascii	"pFwd) || (NULL == pBwd)!!\012\000" )
	.space	2
.LC22:
	ASCII(.ascii	"CurFsID abnormal!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,bwd,currpmv,colpmv = %p,%p," )
	ASCII(.ascii	"%p,%p,%p,%p\012\000" )
.LC24:
	ASCII(.ascii	"line:%d! i > (RV9_MAX_SLC_PARAM_NUM - 2)\012\000" )
	.space	2
.LC25:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC27:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
