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
	ldr	r0, [r7, #2472]
	bl	FSP_NewLogicFs
	add	r8, r5, #8192
	cmp	r0, #0
	str	r0, [r6, #1036]
	blt	.L17
	mov	r1, r0
	ldr	r0, [r7, #2472]
	bl	FSP_GetLogicFs
	subs	r4, r0, #0
	beq	.L18
	ldr	r5, .L19
	mov	r0, #18
	ldr	r2, [r6, #1036]
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
	ldr	r1, [r6, #1032]
	ldr	r0, [r7, #2472]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r6, #1028]
	ldr	r0, [r7, #2472]
	bl	FSP_ClearLogicFs
	str	r4, [r6, #1028]
	str	r4, [r6, #1032]
	ldr	r0, [r7, #2472]
	bl	FSP_GetNullLogicFs
	add	r1, r5, #46080
	add	r1, r1, #56
	str	r0, [r6, #1032]
	str	r0, [r6, #1028]
	strb	r4, [r8, #3183]
	ldr	r0, [r7, #2472]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L18:
	ldr	r3, .L19
	movw	r2, #538
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
	ldr	r2, [r2, #96]
	mov	r5, r1
	ldr	r3, [r4, #92]
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
	ldr	r7, [r4, #88]
	mov	r9, #5
	ldr	r10, [r4, #84]
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
	ldr	r0, [r4, #84]
	ldr	r1, [r5]
	mov	r3, #0
	ldr	r2, [r4, #88]
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
	ldr	r0, [r7, #2472]
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
	ldr	r2, [r4, #88]
	ldr	r0, [r1, #736]
	cmp	r2, r0
	bhi	.L52
	ldr	r3, [r4, #84]
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
	ldr	r0, [r7, #2472]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_PartitionFsMemory
	cmp	r0, r4
	bne	.L53
	add	r5, r5, #8192
	mov	r3, #1
	mov	r0, r3
	strb	r3, [r5, #3180]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L50:
	ldr	r2, [r4, #88]
	ldr	r7, [r1, #736]
	cmp	r2, r7
	bhi	.L54
	ldr	r3, [r4, #84]
	ldr	r10, [r1, #740]
	cmp	r3, r10
	bhi	.L55
	add	r3, r5, #8192
	ldrb	r0, [r3, #3180]	@ zero_extendqisi2
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
	ldr	r3, [r4, #84]
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
	ldr	r3, [r4, #84]
	b	.L29
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
	ldr	r1, [r2, #1036]
	ldr	r0, [r3, #2472]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r2, r5, #40960
	mov	ip, #0
	mov	r1, ip
	ldr	r3, [r2, #3848]
	str	ip, [r4, #148]
	and	r3, r3, #3
	str	r3, [r4, #152]
	orr	ip, r3, #7168
	str	r3, [r4, #156]
	orr	ip, ip, #32
	str	ip, [r4, #160]
	ldr	r3, [r2, #3856]
	str	r3, [r4, #164]
	ldr	r3, [r2, #3852]
	str	r3, [r4, #168]
	bl	SetAspectRatio
	ldr	r3, [r5]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L206
.L202:
	mov	r3, #8
	mov	r2, #1
	mov	r1, #0
	str	r3, [r4, #384]
	str	r3, [r4, #388]
	str	r3, [r4, #396]
	str	r3, [r4, #392]
	strb	r1, [r4, #372]
	strb	r2, [r4, #379]
	strb	r2, [r4, #377]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L206:
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
	b	.L202
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
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r3, #44
	add	r2, r0, #40960
	mla	r3, r3, r1, r0
	ldr	ip, [r2, #3936]
	add	r1, r1, #1
	str	ip, [r3, #88]
	ldr	ip, [r2, #3928]
	str	ip, [r3, #96]
	ldr	ip, [r2, #3932]
	str	ip, [r3, #104]
	ldr	ip, [r2, #3948]
	str	ip, [r3, #92]
	ldr	ip, [r2, #3940]
	str	ip, [r3, #100]
	ldr	ip, [r2, #3944]
	str	ip, [r3, #108]
	ldr	ip, [r2, #3916]
	str	ip, [r3, #112]
	ldr	ip, [r2, #3912]
	str	ip, [r3, #116]
	ldr	ip, [r2, #3908]
	str	ip, [r3, #120]
	ldr	r2, [r2, #3920]
	str	r2, [r3, #124]
	str	r1, [r0, #44]
	ldmfd	sp, {fp, sp, pc}
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
	add	r3, r0, #40960
	add	r2, r0, #45056
	ldr	r1, [r3, #3848]
	cmp	r1, #2
	strb	r1, [r0, #20]
	ldr	r1, [r3, #3864]
	str	r1, [r0, #12]
	ldr	r1, [r3, #3860]
	str	r1, [r0, #16]
	ldr	r1, [r2, #952]
	str	r1, [r0, #24]
	ldr	r2, [r2, #956]
	str	r2, [r0, #28]
	ldr	r2, [r3, #3888]
	str	r2, [r0, #32]
	ldr	r1, [r3, #3952]
	str	r1, [r0, #36]
	ldr	r1, [r3, #3956]
	str	r1, [r0, #40]
	strne	r2, [r3, #3952]
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
	ldr	r2, [r0, #124]
	ldr	r3, [r0, #12]
	cmp	r2, #0
	ldr	ip, [r0, #16]
	bne	.L219
	mul	ip, r3, ip
	ldr	r5, [r0, #44]
	cmp	r5, #2
	sub	ip, ip, #1
	bls	.L218
	ldr	r3, [r0, #168]
	sub	lr, r5, #1
	cmp	r3, #0
	cmpgt	ip, r3
	ble	.L219
	mov	r2, r0
	mov	r3, #1
	b	.L220
.L221:
	ldr	r1, [r2, #212]
	add	r2, r2, #44
	cmp	ip, r1
	cmpgt	r1, #0
	ble	.L219
.L220:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L221
.L218:
	cmp	r5, #1
	bls	.L222
	sub	r4, r5, #1
	mov	r3, r0
	mov	r2, #0
	b	.L223
.L224:
	ldr	lr, [r3, #124]
	add	r3, r3, #44
	add	r2, r2, #1
	ldr	r1, [r3, #124]
	cmp	lr, r1
	bge	.L219
.L223:
	cmp	r2, r4
	bne	.L224
	mov	r3, #44
	mla	r2, r3, r2, r0
	ldr	r3, [r2, #124]
	cmp	ip, r3
	cmpge	r3, #0
	ble	.L219
.L222:
	cmp	r5, #0
	beq	.L225
	sub	r7, r5, #1
	mov	r6, r0
	mov	lr, r0
	mov	r4, #0
	b	.L228
.L243:
	cmp	r4, r5
	beq	.L242
.L228:
	cmp	r4, r7
	streq	ip, [lr, #128]
	add	r4, r4, #1
	add	lr, lr, #44
	ldrne	r3, [lr, #124]
	moveq	r1, ip
	subne	r1, r3, #1
	ldr	r3, [lr, #80]
	strne	r1, [lr, #84]
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
	beq	.L243
.L219:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L242:
	ldr	r2, [r0, #88]
	mov	r1, r2
	b	.L231
.L244:
	ldr	r1, [r6, #88]
.L231:
	cmp	r2, r1
	ldr	ip, [r6, #92]
	add	r3, r3, #1
	movcs	r2, r1
	cmp	ip, r2
	bcs	.L229
	ldr	r1, [r6, #108]
	cmp	r1, #0
	movne	r2, ip
.L229:
	cmp	r5, r3
	add	r6, r6, #44
	bne	.L244
	bic	r2, r2, #15
	ldr	lr, [r0, #44]
	str	r2, [r0, #52]
	mov	r3, r0
	mov	ip, #0
	b	.L235
.L245:
	ldr	r4, [r3, #96]
	rsb	r2, r2, r1
	and	r1, r4, #7
	add	r2, r2, r4, lsr #3
	and	r4, r2, #15
	bic	r2, r2, #15
	str	r2, [r3, #88]
	add	r2, r1, r4, lsl #3
	str	r2, [r3, #96]
.L233:
	add	ip, ip, #1
	add	r3, r3, #44
	cmp	lr, ip
	bls	.L234
	ldr	r2, [r0, #52]
.L235:
	ldr	r1, [r3, #92]
	cmp	r1, #0
	rsb	r4, r2, r1
	ldr	r1, [r3, #88]
	beq	.L245
	ldr	r5, [r3, #96]
	rsb	r2, r2, r1
	ldr	r7, [r3, #100]
	and	r6, r5, #7
	add	r2, r2, r5, lsr #3
	and	r5, r7, #7
	add	r1, r4, r7, lsr #3
	and	r4, r2, #15
	and	r7, r1, #15
	bic	r2, r2, #15
	add	r4, r6, r4, lsl #3
	str	r2, [r3, #88]
	bic	r1, r1, #15
	add	r2, r5, r7, lsl #3
	str	r4, [r3, #96]
	str	r2, [r3, #100]
	str	r1, [r3, #92]
	b	.L233
.L225:
	ldr	r3, [r0, #88]
	bic	r3, r3, #15
	str	r3, [r0, #52]
.L234:
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
	ldr	r7, .L250
	add	r8, r0, #45056
	movw	r2, #14768
	mov	r6, r1
	ldr	r9, [r8, #1424]
	mov	r1, #0
	movt	r2, 1
	ldr	r3, [r7, #48]
	mov	r4, r0
	mov	r5, r1
	blx	r3
	add	r0, r4, #46080
	add	r0, r0, #56
	str	r9, [r8, #1424]
	bl	ResetVoQueue
	add	r3, r4, #40960
	str	r6, [r4]
	mov	r2, #15
	str	r2, [r3, #3952]
	add	ip, r4, #8192
	str	r2, [r3, #3956]
	mov	r1, #8192
	ldr	lr, [r6, #8]
	mov	r2, #1
	strb	r5, [ip, #3180]
	mov	r0, r4
	add	r4, r4, #77824
	strb	lr, [ip, #3178]
	str	r1, [r3, #3872]
	str	r2, [r3, #3844]
	strb	r2, [r3, #3768]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #2472]
	beq	.L249
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L249:
	ldr	r3, [r7, #68]
	mov	r0, r5
	ldr	r1, .L250+4
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L251:
	.align	2
.L250:
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
	ldr	r3, .L253
	add	r2, r0, #45056
	ldr	r1, .L253+4
	mov	r0, #2
	ldr	r2, [r2, #928]
	ldr	r3, [r3, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L254:
	.align	2
.L253:
	.word	vfmw_Osal_Func_Ptr_S
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
	mov	r6, r0
	mov	r4, r1
	add	r5, r6, #40960
	mov	r1, #0
	sub	r0, fp, #184
	bl	memset
	mov	r2, #64
	mov	r1, #0
	sub	r0, fp, #248
	bl	memset
	ldr	r2, [r5, #3828]
	ldr	r3, [r5, #3832]
	cmp	r4, #0
	mov	r0, #1
	mov	r1, #8192
	add	ip, r6, #8
	str	r2, [r5, #3836]
	str	ip, [fp, #-300]
	str	r2, [fp, #-116]
	str	r2, [fp, #-124]
	str	r3, [r5, #3840]
	str	r3, [fp, #-112]
	str	r3, [fp, #-120]
	str	r0, [r5, #3844]
	str	r0, [fp, #-108]
	str	r1, [r5, #3872]
	str	r1, [fp, #-80]
	beq	.L374
	ldr	r3, .L394
	add	r7, r6, #45056
	movw	r2, #11348
	mov	r1, #0
	ldr	r0, [fp, #-300]
	mov	r9, r1
	ldr	r3, [r3, #48]
	add	r10, r6, #8192
	blx	r3
	str	r9, [r7, #892]
	ldr	r3, [r4, #12]
	str	r3, [r7, #912]
	ldr	r3, [r4, #8]
	str	r3, [r7, #896]
	ldr	r3, [r4]
	str	r3, [r10, #3168]
	ldr	r2, [r4, #12]
	str	r2, [r7, #900]
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	strb	r3, [r10, #3176]
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L258
	ldr	r3, [r4, #88]
	str	r3, [r7, #904]
	ldr	r3, [r4, #80]
	str	r3, [r10, #3172]
	ldr	r3, [r4, #92]
	str	r3, [r7, #908]
	ldrb	r3, [r4, #85]	@ zero_extendqisi2
	strb	r3, [r10, #3177]
	ldrb	r3, [r4, #85]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L375
.L259:
	movw	r3, #65460
	sub	r1, r2, #75
	movt	r3, 47
	cmp	r1, r3
	bhi	.L369
	ldr	r1, [r10, #3168]
	cmp	r1, #0
	beq	.L369
	add	r4, r6, #46080
	add	r4, r4, #24
	mov	r0, r4
	bl	BsInit
	mov	r1, #8
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	mov	r8, r0
	str	r0, [fp, #-296]
	mov	r0, r4
	bl	BsSkip
	cmp	r8, #8
	bhi	.L369
	sub	r8, fp, #252
.L262:
	mov	r1, #32
	mov	r0, r4
	bl	BsShow
	mov	r1, #32
	mov	r9, r0
	mov	r0, r4
	bl	BsSkip
	sub	r3, fp, #188
	rev	r9, r9
	str	r9, [r8, #4]!
	cmp	r8, r3
	bne	.L262
	mov	r1, #8
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	mov	r8, r0
	mov	r0, r4
	bl	BsSkip
	cmp	r8, #255
	add	r0, r8, #1
	str	r0, [r5, #3960]
	bhi	.L369
	cmp	r0, #0
	beq	.L263
	add	r9, r6, #44800
	movw	r3, #65535
	add	r9, r9, #120
	movt	r3, 47
	str	r6, [fp, #-304]
	mov	r8, #0
	mov	r6, r9
	str	r7, [fp, #-308]
	mov	r9, r3
	b	.L264
.L377:
	ldr	r0, [r5, #3960]
	cmp	r0, r8
	bls	.L376
.L264:
	mov	r1, #32
	mov	r0, r4
	bl	BsSkip
	mov	r1, #32
	mov	r0, r4
	add	r8, r8, #1
	bl	BsShow
	mov	r1, #32
	mov	r7, r0
	mov	r0, r4
	bl	BsSkip
	rev	r0, r7
	cmp	r0, r9
	str	r0, [r6, #4]!
	ble	.L377
.L369:
	mov	r3, #0
	str	r3, [fp, #-296]
.L358:
	ldr	r0, [fp, #-296]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L375:
	add	r6, r6, #77824
	ldr	r1, [r4, #16]
	str	r9, [fp, #-296]
	ldr	r0, [r6, #2472]
	bl	SM_ReleaseStreamSeg
	ldr	r0, [r6, #2472]
	ldr	r1, [r4, #96]
	bl	SM_ReleaseStreamSeg
	ldr	r0, [fp, #-296]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L258:
	str	r9, [r7, #904]
	str	r9, [r10, #3172]
	str	r9, [r7, #908]
	strb	r9, [r10, #3177]
	b	.L259
.L374:
	ldr	r3, .L394
	add	r6, r6, #45056
	ldr	r1, .L394+4
	ldr	r2, [r6, #928]
	str	r4, [fp, #-296]
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r0, [fp, #-296]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L376:
	ldr	r6, [fp, #-304]
	ldr	r7, [fp, #-308]
.L263:
	add	r3, r6, #44800
	sub	lr, r0, #1
	add	r3, r3, #124
	mov	r2, #0
	b	.L265
.L266:
	ldr	ip, [r3, #4]!
	ldr	r1, [r3, #-4]
	cmp	ip, r1
	ble	.L369
.L265:
	cmp	r2, lr
	add	r2, r2, #1
	bne	.L266
	ldr	r2, [r5, #3964]
	mov	r0, r0, asl #3
	add	r1, r0, #66
	movw	r3, #65461
	add	r0, r1, r2
	cmp	r1, #3145728
	sub	r8, r0, #74
	movt	r3, 47
	movlt	r2, #0
	movge	r2, #1
	cmp	r8, r3
	movls	r8, r2
	orrhi	r8, r2, #1
	str	r1, [fp, #-308]
	cmp	r8, #0
	str	r0, [fp, #-312]
	bne	.L369
	ldr	r2, [r7, #900]
	mov	r3, r0
	rsb	r2, r0, r2
	cmp	r2, #0
	ble	.L369
	ldr	r1, [r10, #3168]
	mov	r0, r4
	sub	r9, fp, #248
	add	r1, r1, r3
	bl	BsInit
	ldr	r3, [fp, #-296]
	ldr	lr, .L394+8
	sub	r2, fp, #44
	sub	ip, fp, #284
	str	r9, [fp, #-172]
	add	r2, r2, r3, lsl #2
	str	r3, [fp, #-168]
	str	r2, [fp, #-296]
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	sub	r1, fp, #184
	ldr	lr, [lr]
	mov	r0, r4
	ldr	r3, [fp, #-296]
	str	lr, [ip]
	ldr	r3, [r3, #-240]
	str	r3, [fp, #-176]
	bl	Real8_CB_GetPictureHeader
	cmp	r0, #1
	str	r0, [fp, #-296]
	bne	.L378
	ldrb	r3, [r10, #3180]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L268
	ldr	r3, .L394+12
	ldr	ip, [r3]
	cmp	ip, #0
	addeq	r3, r6, #77824
	streq	r3, [fp, #-304]
	beq	.L269
	add	r3, r6, #77824
	str	r3, [fp, #-304]
	ldr	lr, [r5, #3864]
	sub	r2, fp, #284
	mov	r9, r3
	ldr	r3, [r5, #3860]
	sub	r1, fp, #280
	ldr	r0, [fp, #-88]
	strh	lr, [r2]	@ movhi
	sub	r1, r1, #2
	str	r3, [fp, #-316]
	mov	r3, #8
	ldr	lr, [fp, #-316]
	ldr	r8, [fp, #-92]
	strh	lr, [r1]	@ movhi
	sub	lr, fp, #280
	mov	r1, #2
	strh	r0, [lr]	@ movhi
	sub	r0, fp, #276
	sub	r0, r0, #2
	strh	r8, [r0]	@ movhi
	ldr	r0, [r9, #2472]
	blx	ip
.L269:
	ldr	r9, [fp, #-304]
	mov	r2, #1
	ldr	r1, [r7, #1032]
	add	r8, r6, #46080
	add	r8, r8, #56
	ldr	r0, [r9, #2472]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r7, #1028]
	ldr	r0, [r9, #2472]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	str	r3, [r7, #1028]
	str	r3, [r7, #1032]
	ldr	r0, [r9, #2472]
	str	r3, [fp, #-316]
	bl	FSP_GetNullLogicFs
	ldr	r3, [fp, #-316]
	mov	r1, r8
	str	r0, [r7, #1032]
	str	r0, [r7, #1028]
	strb	r3, [r10, #3183]
	ldr	r0, [r9, #2472]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [fp, #-88]
	ldr	r3, [fp, #-92]
	cmp	r2, #512
	bhi	.L271
	cmp	r3, #512
	bhi	.L271
	ldr	r3, [r6]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L273
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L379
.L273:
	ldr	r3, [fp, #-304]
	ldr	r0, [r3, #2472]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L364
.L275:
	ldr	r3, .L394
	mov	r0, #1
	ldr	r2, [fp, #-88]
	ldr	r1, .L394+16
	ldr	ip, [r3, #68]
	ldr	r3, [fp, #-92]
	blx	ip
	sub	r2, fp, #184
	mov	r1, r6
	mov	r0, #6
	bl	Real8_ArrangeVHBMem
	ldr	r8, .L394
	subs	r2, r0, #0
	beq	.L380
.L268:
	add	r3, r6, #44544
	mov	r2, #140
	add	r3, r3, #184
	str	r3, [fp, #-328]
	sub	r1, fp, #184
	mov	r0, r3
	ldr	r3, .L394
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r3, [r6]
	mov	r0, r6
	ldr	r3, [r3, #8]
	strb	r3, [r10, #3178]
	bl	Real8_GetImageBuffer
	subs	r2, r0, #0
	beq	.L381
	add	r3, r6, #77824
	ldr	r1, [r7, #1036]
	str	r3, [fp, #-304]
	ldr	r0, [r3, #2472]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L370
	ldr	r2, [r6]
	mov	r0, #29
	ldr	ip, .L394
	ldr	r1, .L394+20
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
	ldr	r3, [r5, #3848]
	cmp	r3, #2
	beq	.L371
	cmp	r3, #1
	ldrb	r3, [r10, #3183]	@ zero_extendqisi2
	beq	.L382
	cmp	r3, #0
	movne	r3, #2
	moveq	r3, #1
.L282:
	strb	r3, [r10, #3183]
	ldr	r2, [r7, #1028]
	ldr	r3, [r7, #1032]
	cmp	r2, r3
	beq	.L283
	ldr	r3, .L394
	mov	r0, #13
	ldr	r1, .L394+24
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [fp, #-304]
	mov	r2, #0
	ldr	r1, [r7, #1028]
	ldr	r0, [r3, #2472]
	bl	FSP_SetRef
.L283:
	ldr	r3, [r7, #1032]
	mov	r2, #1
	ldr	r1, [r7, #1036]
	ldr	r8, [fp, #-304]
	str	r3, [r7, #1028]
	str	r1, [r7, #1032]
	ldr	r0, [r8, #2472]
	bl	FSP_SetRef
	ldr	r1, [r7, #1028]
	ldr	r0, [r8, #2472]
	bl	FSP_RemovePmv
	ldr	r3, .L394
	ldr	r2, [r7, #1036]
	mov	r0, #13
	ldr	r1, .L394+28
	ldr	r3, [r3, #68]
	blx	r3
	ldrb	r3, [r10, #3178]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L285
	ldr	r3, [r5, #3848]
	cmp	r3, #0
	bne	.L383
.L365:
	str	r3, [r7, #1044]
.L285:
	ldr	r9, [fp, #-304]
	ldr	r1, [r7, #1036]
	ldr	r0, [r9, #2472]
	bl	FSP_GetLogicFs
	ldr	r1, [r7, #1028]
	str	r0, [fp, #-316]
	ldr	r0, [r9, #2472]
	bl	FSP_GetLogicFs
	ldr	r1, [r7, #1032]
	mov	r8, r0
	ldr	r0, [r9, #2472]
	bl	FSP_GetLogicFs
	ldr	ip, [fp, #-316]
	cmp	ip, #0
	cmpne	r8, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	str	r0, [fp, #-320]
	cmp	r3, #0
	bne	.L384
	ldr	r3, [ip, #520]
	cmp	r3, #0
	beq	.L291
	ldr	r2, [ip, #528]
	cmp	r2, #0
	beq	.L291
	ldr	r2, [ip, #524]
	cmp	r2, #0
	beq	.L291
	ldr	r2, [fp, #-320]
	mov	r0, #13
	ldr	r1, .L394+32
	ldr	r2, [r2, #520]
	cmp	r2, #0
	ldrne	r3, [r2, #12]
	ldreq	r3, [r3, #12]
	str	r3, [fp, #-316]
	ldr	r3, [fp, #-316]
	str	r3, [r6, #56]
	ldr	r3, [r8, #520]
	cmp	r3, #0
	ldreq	r3, [ip, #520]
	ldr	lr, [r3, #12]
	str	ip, [fp, #-332]
	str	lr, [r6, #60]
	ldr	r3, [ip, #520]
	ldr	r2, [r3, #12]
	str	r2, [r6, #64]
	ldr	r3, [ip, #532]
	str	r3, [r6, #80]
	mov	r9, r3
	ldr	r3, [fp, #-320]
	ldr	r3, [r3, #532]
	str	r3, [fp, #-324]
	str	r3, [r6, #84]
	ldr	r3, [ip, #524]
	ldr	ip, [fp, #-324]
	ldr	r3, [r3, #12]
	str	ip, [sp, #12]
	ldr	ip, [fp, #-316]
	str	r9, [sp, #8]
	str	r3, [r6, #68]
	str	ip, [sp, #4]
	ldr	ip, .L394
	str	lr, [sp]
	ldr	ip, [ip, #68]
	blx	ip
	ldr	ip, [fp, #-332]
	ldr	r3, [ip, #524]
	ldr	r3, [r3, #20]
	str	r3, [r6, #72]
	ldr	r3, [r5, #3848]
	cmp	r3, #2
	beq	.L385
.L297:
	ldr	r3, [r5, #3876]
	mov	r0, r4
	str	r3, [ip, #540]
	ldr	r3, [r5, #3848]
	ldr	r2, [r5, #3888]
	cmp	r3, #2
	strb	r3, [r6, #20]
	ldr	r3, [r5, #3864]
	str	r2, [fp, #-316]
	str	r3, [r6, #12]
	ldr	r3, [r5, #3860]
	str	r3, [r6, #16]
	ldr	r3, [r7, #952]
	str	r3, [r6, #24]
	ldr	r3, [r7, #956]
	str	r2, [r6, #32]
	str	r3, [r6, #28]
	ldr	r3, [r5, #3952]
	str	r3, [r6, #36]
	ldr	r3, [r5, #3956]
	str	r3, [r6, #40]
	strne	r2, [r5, #3952]
	bl	BsPos
	ldr	r3, [r5, #3960]
	ands	r0, r0, #-2147483641
	submi	r0, r0, #1
	mvnmi	r0, r0, asl #29
	mvnmi	r0, r0, lsr #29
	addmi	r0, r0, #1
	cmp	r3, #1
	str	r0, [r5, #3928]
	ldreq	r2, [fp, #-308]
	moveq	r0, r4
	ldreq	r3, [r7, #900]
	movne	r0, r4
	ldrne	r3, [r5, #3968]
	ldreq	r8, [r5, #3964]
	rsbeq	r3, r2, r3
	ldrne	r8, [r5, #3964]
	rsb	r8, r8, r3
	bl	BsPos
	ldr	r3, [fp, #-312]
	mov	r8, r8, asl #3
	rsb	r0, r0, r8
	str	r0, [r5, #3932]
	ldr	r8, [r7, #896]
	mov	r0, r4
	add	r8, r3, r8
	bl	BsPos
	ldr	r2, [r5, #3892]
	mov	r1, #1
	ldr	ip, [r5, #3884]
	ldr	lr, [fp, #-316]
	str	r2, [r5, #3912]
	str	ip, [r5, #3916]
	str	lr, [r5, #3908]
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	mov	r3, #0
	add	r0, r8, r0, asr #3
	str	r3, [r5, #3940]
	str	r0, [r5, #3936]
	str	r3, [r5, #3944]
	str	r3, [r5, #3948]
	str	r3, [r5, #3920]
	str	r0, [r6, #88]
	ldr	r0, [r5, #3928]
	str	r0, [r6, #96]
	ldr	r0, [r5, #3932]
	str	ip, [r6, #112]
	str	r3, [r6, #92]
	str	r0, [r6, #104]
	str	r3, [r6, #100]
	str	r3, [r6, #108]
	str	r2, [r6, #116]
	str	r3, [r6, #124]
	str	lr, [r6, #120]
	str	r1, [r6, #44]
	ldr	r2, [r5, #3960]
	cmp	r2, r1
	bls	.L325
	ldr	r2, [r5, #3968]
	movw	r0, #65535
	ldr	ip, [fp, #-308]
	movt	r0, 47
	add	r8, ip, r2
	cmp	r8, r0
	bgt	.L311
	cmp	r8, #73
	ble	.L311
	ldr	r2, [r7, #900]
	rsb	r2, r8, r2
	cmp	r2, #0
	ble	.L311
	add	ip, r6, #44800
	str	r3, [fp, #-336]
	add	ip, ip, #128
	str	r0, [fp, #-332]
	str	ip, [fp, #-312]
	mov	r9, r1
	str	r1, [fp, #-324]
	str	r10, [fp, #-320]
	b	.L314
.L315:
	ldr	r3, [r5, #3960]
	add	r9, r9, #1
	cmp	r3, r9
	bls	.L386
	ldr	r3, [fp, #-312]
	ldr	r8, [r3, #4]!
	str	r3, [fp, #-312]
	ldr	r3, [fp, #-308]
	add	r8, r3, r8
	ldr	r3, [fp, #-332]
	cmp	r8, r3
	bgt	.L311
	cmp	r8, #73
	ble	.L311
	ldr	r2, [r7, #900]
	rsb	r2, r8, r2
	cmp	r2, #0
	ble	.L311
.L314:
	ldr	r3, [fp, #-320]
	mov	r0, r4
	ldr	r1, [r3, #3168]
	add	r1, r1, r8
	bl	BsInit
	ldr	r3, [fp, #-328]
	sub	r2, fp, #292
	mov	r1, #0
	mov	r0, r4
	str	r3, [sp, #4]
	sub	r3, fp, #284
	str	r3, [sp]
	sub	r3, fp, #288
	bl	Real8_CB_GetSliceHeader
	cmp	r0, #1
	bne	.L315
	ldr	r3, [r5, #3864]
	ldr	r2, [fp, #-292]
	cmp	r3, #0
	mov	r10, r2
	beq	.L387
	ldr	r3, [fp, #-284]
	mov	r0, r4
	ldr	r2, [fp, #-316]
	cmp	r3, #0
	movge	r2, r3
	str	r2, [fp, #-316]
	bl	BsPos
	ldr	r2, [r5, #3960]
	sub	ip, r2, #1
	cmp	ip, r9
	mov	r3, r0, asr #31
	mov	r1, r3, lsr #29
	add	r3, r0, r1
	and	r3, r3, #7
	rsb	r3, r1, r3
	str	r3, [r5, #3928]
	beq	.L388
	cmp	r9, #254
	bhi	.L389
	ldr	r2, [fp, #-312]
	mov	r0, r4
	ldr	r3, [r2, #4]
	ldr	r2, [r2]
.L368:
	rsb	r3, r2, r3
	mov	r3, r3, asl #3
	str	r3, [fp, #-340]
	bl	BsPos
	ldr	r3, [fp, #-340]
	mov	ip, #0
	str	ip, [fp, #-340]
	rsb	r0, r0, r3
	str	r0, [r5, #3932]
	ldr	r2, [r7, #896]
	mov	r0, r4
	add	r8, r8, r2
	bl	BsPos
	ldr	ip, [fp, #-324]
	mov	r3, #44
	ldr	r1, [r5, #3892]
	ldr	lr, [r5, #3884]
	mla	r3, r3, ip, r6
	str	r10, [r5, #3920]
	str	r1, [r5, #3912]
	add	ip, ip, #1
	str	lr, [r5, #3916]
	str	ip, [fp, #-324]
	add	r2, r0, #7
	cmp	r0, #0
	movlt	r0, r2
	cmp	ip, #131
	add	r2, r8, r0, asr #3
	ldr	r0, [fp, #-336]
	ldr	r8, [fp, #-316]
	str	r2, [r5, #3936]
	str	r0, [r5, #3940]
	str	r0, [r5, #3944]
	str	r0, [r5, #3948]
	str	r8, [r5, #3908]
	str	r2, [r3, #88]
	ldr	r2, [r5, #3928]
	str	r2, [r3, #96]
	ldr	r2, [r5, #3932]
	str	r0, [r3, #92]
	str	r0, [r3, #100]
	str	r2, [r3, #104]
	str	r0, [r3, #108]
	str	lr, [r3, #112]
	str	r1, [r3, #116]
	str	r8, [r3, #120]
	str	r10, [r3, #124]
	str	ip, [r6, #44]
	ble	.L315
	mov	r0, r6
	bl	Real8_ModifySliceMsg
	ldr	ip, [fp, #-340]
	cmp	r0, #1
	bne	.L390
	ldr	r3, [fp, #-300]
	cmp	r3, #0
	str	r3, [r6, #4]
	beq	.L391
	ldr	r3, [r6]
	ldr	r2, [r3, #884]
	cmp	r2, #1
	beq	.L392
	cmp	r2, #2
	bne	.L315
	ldr	r3, [r3, #888]
	cmp	r3, #0
	ble	.L315
	ldrb	r3, [r6, #20]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L315
	ldr	r3, [fp, #-304]
	mov	r2, ip
	str	ip, [r6, #4]
	ldr	r1, [r7, #1036]
	ldr	r0, [r3, #2472]
	str	ip, [fp, #-296]
	bl	FSP_SetDisplay
	ldr	r2, [r6]
	mov	r1, r6
	mov	r0, #8
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	bl	ReleasePacket
	b	.L358
.L378:
	ldr	r3, .L394
	mov	r0, #1
	ldr	r1, .L394+36
	str	r8, [fp, #-296]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L358
.L271:
	ldr	r1, .L394
	mov	lr, #0
	mov	r0, #18
	str	lr, [fp, #-296]
	ldr	ip, [r1, #68]
	ldr	r1, .L394+40
	blx	ip
	b	.L358
.L381:
	ldr	r3, .L394
	str	r2, [fp, #-296]
	ldr	r1, .L394+44
	ldr	r3, [r3, #68]
	ldr	r2, [r7, #928]
	blx	r3
	b	.L358
.L383:
	cmp	r3, #2
	beq	.L327
	cmp	r3, #1
	bne	.L285
	b	.L365
.L382:
	cmp	r3, #0
	movne	r3, #2
	bne	.L282
	b	.L369
.L371:
	ldrb	r3, [r10, #3183]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L311
	ldrb	r3, [r10, #3178]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L285
.L327:
	ldr	r3, [r7, #1044]
	cmp	r3, #0
	bne	.L285
.L370:
	ldr	r0, [fp, #-304]
	mov	r2, #1
	ldr	r1, [r7, #1036]
	str	r3, [fp, #-296]
	ldr	r0, [r0, #2472]
	bl	FSP_ClearLogicFs
	b	.L358
.L291:
	ldr	r3, [fp, #-304]
	mov	r2, #1
	ldr	r1, [r7, #1036]
	ldr	r0, [r3, #2472]
	mov	r3, #0
	str	r3, [fp, #-296]
	bl	FSP_ClearLogicFs
	ldr	r3, .L394
	mov	r0, #0
	ldr	r1, .L394+48
	ldr	r3, [r3, #68]
	blx	r3
	b	.L358
.L384:
	ldr	r3, .L394
	mov	lr, #0
	ldr	r2, .L394+52
	mov	r0, #22
	ldr	r1, .L394+56
	ldr	ip, [r3, #68]
	movw	r3, #1992
	str	lr, [fp, #-296]
	blx	ip
	b	.L358
.L385:
	ldr	r1, [fp, #-320]
	ldr	r3, [r8, #540]
	ldr	r2, [r5, #3876]
	ldr	r8, [r1, #540]
	subs	r2, r2, r3
	rsb	r8, r3, r8
	ldrmi	r3, [r5, #3872]
	addmi	r2, r2, r3
	cmp	r8, #0
	ldrlt	r3, [r5, #3872]
	addlt	r8, r8, r3
	cmp	r2, r8
	movgt	r2, #0
	cmp	r8, #0
	ble	.L329
	mov	r1, r8
	mov	r0, r2, asl #14
	str	ip, [fp, #-324]
	str	r2, [fp, #-316]
	bl	__aeabi_idiv
	ldr	r2, [fp, #-316]
	mov	r1, r8
	str	r0, [fp, #-320]
	rsb	r0, r2, r8
	mov	r0, r0, asl #14
	bl	__aeabi_idiv
	ldr	r2, [fp, #-316]
	ldr	r3, [fp, #-320]
	ldr	ip, [fp, #-324]
.L301:
	str	r2, [r7, #944]
	str	r8, [r7, #948]
	str	r3, [r7, #952]
	str	r0, [r7, #956]
	b	.L297
.L380:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L394+60
	str	r2, [fp, #-296]
	blx	r3
	b	.L358
.L379:
	ldrb	r3, [r10, #3180]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L275
.L364:
	mov	r0, r8
	bl	ResetVoQueue
	b	.L275
.L388:
	ldr	r1, [fp, #-308]
	add	r2, r2, #8384
	ldr	r3, [r7, #900]
	add	r2, r2, #5
	mov	r0, r4
	rsb	r3, r1, r3
	ldr	r1, [fp, #-320]
	add	r2, r1, r2, lsl #2
	ldr	r2, [r2, #3172]
	b	.L368
.L387:
	mov	r2, r0
	ldr	r0, [fp, #-304]
	ldr	r1, [r7, #1036]
	str	r3, [fp, #-296]
	ldr	r0, [r0, #2472]
	bl	FSP_ClearLogicFs
	b	.L358
.L386:
	ldr	r10, [fp, #-320]
.L325:
	ldr	r4, [r6, #4]
	mov	r5, #1
	str	r5, [r6, #48]
	cmp	r4, #0
	beq	.L393
.L308:
	ldr	r3, [r6]
	ldr	r3, [r3, #676]
	str	r3, [r10, #3160]
	b	.L358
.L390:
	ldr	r3, [fp, #-304]
	mov	r2, #1
	ldr	r1, [r7, #1036]
	str	ip, [fp, #-296]
	ldr	r0, [r3, #2472]
	bl	FSP_ClearLogicFs
	b	.L358
.L389:
	ldr	r3, .L394
	movw	r2, #2156
	ldr	r1, .L394+64
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	b	.L369
.L393:
	mov	r0, r6
	bl	Real8_ModifySliceMsg
	cmp	r0, #1
	ldreq	r3, [fp, #-300]
	streq	r3, [r6, #4]
	beq	.L308
	ldr	r3, [fp, #-304]
	mov	r2, r5
	ldr	r1, [r7, #1036]
	str	r4, [fp, #-296]
	ldr	r0, [r3, #2472]
	bl	FSP_ClearLogicFs
	b	.L358
.L329:
	mov	r0, #0
	mov	r3, r0
	b	.L301
.L311:
	ldr	r3, [fp, #-304]
	mov	r2, #1
	ldr	r1, [r7, #1036]
	ldr	r0, [r3, #2472]
	mov	r3, #0
	str	r3, [fp, #-296]
	bl	FSP_ClearLogicFs
	b	.L358
.L392:
	ldrb	r3, [r6, #20]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L315
	ldr	r3, [fp, #-304]
	mov	r2, ip
	str	ip, [r6, #4]
	ldr	r1, [r7, #1036]
	ldr	r0, [r3, #2472]
	str	ip, [fp, #-296]
	bl	FSP_SetDisplay
	mov	r1, r6
	mov	r0, #8
	bl	ReleasePacket
	b	.L358
.L391:
	ldr	r3, .L394
	mov	r0, #22
	ldr	lr, [fp, #-300]
	ldr	r2, .L394+52
	ldr	ip, [r3, #68]
	movw	r3, #2189
	ldr	r1, .L394+68
	str	lr, [fp, #-296]
	blx	ip
	b	.L358
.L395:
	.align	2
.L394:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC13
	.word	.LANCHOR0+460
	.word	g_event_report
	.word	.LC16
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC24
	.word	.LC14
	.word	.LC15
	.word	.LC18
	.word	.LC23
	.word	.LANCHOR0+528
	.word	.LC22
	.word	.LC17
	.word	.LC25
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
	add	r4, r0, #77824
	mov	r2, #0
	ldr	r0, [r4, #2472]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #2472]
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
	beq	.L414
	add	r7, r0, #45056
	add	r6, r0, #77824
	ldr	r1, [r7, #1032]
	ldr	r0, [r6, #2472]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L414
	ldr	r1, [r7, #1032]
	ldr	r0, [r6, #2472]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L413
.L414:
	add	r5, r4, #46080
	add	r5, r5, #56
.L404:
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	GetVoLastImageID
.L413:
	mov	r0, r4
	add	r5, r4, #46080
	bl	Real8_SetImgFormat
	add	r5, r5, #56
	mov	r3, #1
	str	r3, [r8, #244]
	mov	r2, r3
	ldr	r1, [r7, #1032]
	ldr	r0, [r6, #2472]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #2472]
	mov	r2, r4
	str	r8, [sp]
	mov	r3, r5
	mov	r1, #8
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L401
	ldr	r1, [r7, #1032]
	mov	r2, #0
	ldr	r0, [r6, #2472]
	bl	FSP_SetDisplay
	b	.L404
.L401:
	ldr	r3, [r8, #200]
	cmp	r3, #100
	bhi	.L404
	ldr	r3, [r8, #164]
	cmp	r3, #31
	bls	.L404
	ldr	r3, [r8, #168]
	cmp	r3, #31
	bls	.L404
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	REAL8DEC_GetRemainImg, .-REAL8DEC_GetRemainImg
	.align	2
	.global	REAL8DEC_VDMPostProc
	.type	REAL8DEC_VDMPostProc, %function
REAL8DEC_VDMPostProc:
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
	ldr	r10, .L444
	mov	r6, r1
	mov	r4, r0
	ldr	r1, .L444+4
	mov	r9, r2
	ldr	r3, [r10, #68]
	mov	r2, r6
	mov	r0, #22
	add	r5, r4, #45056
	add	r7, r4, #77824
	blx	r3
	mov	r1, r4
	mov	r0, #8
	bl	ReleasePacket
	ldr	r1, [r5, #1036]
	ldr	r0, [r7, #2472]
	add	r8, r4, #8192
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L416
	add	r3, r4, #40960
	ldr	r2, [r3, #3848]
	cmp	r2, #2
	andne	r9, r9, #31
	strb	r2, [r8, #3181]
	strne	r9, [r3, #3956]
	cmp	r6, #0
	movlt	r6, #0
	blt	.L418
	cmp	r6, #100
	movge	r6, #100
.L418:
	ldr	r2, [r10, #68]
	str	r6, [r0, #200]
	mov	r0, #22
	ldr	r1, .L444+8
	str	r3, [fp, #-48]
	blx	r2
	mov	r0, r4
	bl	Real8_SetImgFormat
	ldrb	r2, [r8, #3178]	@ zero_extendqisi2
	ldr	r3, [fp, #-48]
	cmp	r2, #2
	beq	.L421
	ldr	r2, [r4]
	ldr	r2, [r2, #12]
	cmp	r2, #1
	beq	.L421
	ldr	r3, [r3, #3848]
	cmp	r3, #2
	beq	.L421
	ldrb	r3, [r8, #3183]	@ zero_extendqisi2
	cmp	r3, #1
	ldrhi	r1, [r5, #1028]
	mvnls	r1, #0
	cmn	r1, #1
	str	r1, [r5, #1040]
	beq	.L423
.L443:
	ldr	r0, [r7, #2472]
	bl	FSP_GetFsImagePtr
	ldr	r6, [r5, #1040]
	ldr	r9, [r10, #68]
	mov	r1, r6
	mov	r8, r0
	ldr	r0, [r7, #2472]
	bl	FSP_GetRef
	mov	r2, r6
	ldr	r1, .L444+12
	mov	r3, r0
	mov	r0, #12
	blx	r9
	mov	r2, #1
	ldr	r1, [r5, #1040]
	ldr	r0, [r7, #2472]
	bl	FSP_SetDisplay
	add	r3, r4, #46080
	ldr	r0, [r7, #2472]
	add	r3, r3, #56
	str	r8, [sp]
	mov	r2, r4
	mov	r1, #8
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L442
.L423:
	ldr	r3, [r5, #928]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r5, #928]
.L416:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L421:
	ldr	r1, [r5, #1036]
	cmn	r1, #1
	str	r1, [r5, #1040]
	bne	.L443
	b	.L423
.L442:
	ldr	r0, [r7, #2472]
	mov	r2, #0
	ldr	r1, [r5, #1040]
	bl	FSP_SetDisplay
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L445:
	.align	2
.L444:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC27
	.word	.LC28
	.word	.LC29
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
	add	r4, r0, #77824
	mov	r5, r0
	ldr	r0, [r4, #2472]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #2472]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L452
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L452:
	add	r1, r5, #46080
	ldr	r0, [r4, #2472]
	add	r1, r1, #56
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
	.type	__FUNCTION__.13841, %object
	.size	__FUNCTION__.13841, 16
__FUNCTION__.13841:
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
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC16:
	ASCII(.ascii	"new seq size %d * %d\012\000" )
	.space	2
.LC17:
	ASCII(.ascii	"Real8_ArrangeVHBMem err\012\000" )
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
	ASCII(.ascii	"%s %d unknow error!!\012\000" )
	.space	2
.LC23:
	ASCII(.ascii	"CurFsID abnormal!\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,bwd,currpmv,colpmv = %p,%p," )
	ASCII(.ascii	"%p,%p,%p,%p\012\000" )
.LC25:
	ASCII(.ascii	"line:%d! i > (RV8_MAX_SLC_PARAM_NUM - 2)\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"%s %d NULL == pCtx->pRv8DecParam!!\012\000" )
.LC27:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC28:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
