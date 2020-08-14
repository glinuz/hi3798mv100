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
	.file	"divx3.c"
	.text
	.align	2
	.global	Divx3_GetImageBuffer
	.type	Divx3_GetImageBuffer, %function
Divx3_GetImageBuffer:
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
	add	r6, r0, #65536
	add	r5, r0, #32768
	mov	r7, r0
	mov	r1, #1
	ldr	r0, [r6, #2432]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r5, #996]
	blt	.L9
	mov	r1, r0
	ldr	r0, [r6, #2432]
	bl	FSP_GetLogicFs
	subs	r4, r0, #0
	beq	.L10
	ldr	r6, .L11
	mov	r0, #18
	ldr	r2, [r5, #996]
	ldr	r1, .L11+4
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r2, [r4, #520]
	cmp	r2, #0
	beq	.L7
	ldr	r3, [r4, #524]
	cmp	r3, #0
	beq	.L7
	ldr	r1, [r4, #528]
	cmp	r1, #0
	beq	.L7
	ldr	lr, [r1, #12]
	mov	r0, #18
	ldr	r3, [r3, #12]
	mov	r4, #1
	ldr	r2, [r2, #12]
	ldr	ip, [r6, #68]
	ldr	r1, .L11+8
	str	lr, [sp]
	blx	ip
.L3:
	mov	r0, r4
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L7:
	mov	r4, #1
	mov	r0, r4
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L9:
	ldr	r3, .L11
	mov	r0, #0
	ldr	r1, .L11+12
	mov	r4, r0
	ldr	r3, [r3, #68]
	blx	r3
	add	r1, r7, #33792
	add	r1, r1, #16
	ldr	r0, [r6, #2432]
	bl	FSP_ClearNotInVoQueue
	mov	r2, #1
	ldr	r1, [r5, #992]
	ldr	r0, [r6, #2432]
	bl	FSP_ClearLogicFs
	str	r4, [r5, #992]
	ldr	r0, [r6, #2432]
	bl	FSP_GetNullLogicFs
	str	r4, [r5, #908]
	str	r0, [r5, #992]
	mov	r0, r4
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L10:
	ldr	r3, .L11
	mov	r2, #100
	ldr	r1, .L11+16
	ldr	r3, [r3, #68]
	blx	r3
	b	.L3
.L12:
	.align	2
.L11:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC3
	.word	.LC0
	.word	.LC1
	UNWIND(.fnend)
	.size	Divx3_GetImageBuffer, .-Divx3_GetImageBuffer
	.align	2
	.global	Divx3_ArrangeVHBMem
	.type	Divx3_ArrangeVHBMem, %function
Divx3_ArrangeVHBMem:
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
	mov	r5, r1
	ldr	r1, [r2, #48]
	ldr	r3, [r2, #52]
	cmp	r1, #1
	bls	.L15
	cmp	r3, #1
	bls	.L15
	ldr	r1, [r5]
	ldr	r3, [r1, #776]
	cmp	r3, #0
	ldr	r3, [r1, #28]
	bne	.L18
	cmp	r3, #25
	beq	.L41
.L19:
	ldr	r4, [r2, #56]
	mov	r7, #5
	ldr	r10, [r2, #60]
	mov	r9, #2
	mov	r8, #4
.L24:
	ldr	r6, .L46
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r6, #48]
	blx	r3
	ldr	r2, [r5]
	mov	r3, #0
	str	r4, [fp, #-68]
	str	r10, [fp, #-64]
	ldr	r2, [r2, #28]
	strb	r3, [fp, #-75]
	cmp	r2, #24
	beq	.L30
	uxtb	r3, r9
	strb	r8, [fp, #-72]
	strb	r7, [fp, #-70]
.L32:
	add	r7, r5, #65536
	sub	r1, fp, #76
	strb	r3, [fp, #-69]
	mov	r4, #0
	ldr	r0, [r7, #2432]
	strb	r4, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r8, r0, #0
	beq	.L42
.L40:
	mov	r0, #0
.L37:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L18:
	cmp	r3, #25
	bne	.L19
	ldr	r4, [r2, #56]
	ldr	r0, [r1, #736]
	cmp	r4, r0
	bhi	.L43
	ldr	r3, [r2, #60]
	ldr	ip, [r1, #740]
	cmp	r3, ip
	bhi	.L27
	ldr	r2, [r1, #764]
	mov	r10, r3
	ldr	r9, [r1, #760]
	cmp	r2, #0
	add	r8, r9, #2
	bne	.L44
.L35:
	mov	r9, #1
.L29:
	ldrb	r7, [r1, #784]	@ zero_extendqisi2
	b	.L24
.L15:
	ldr	ip, .L46
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L46+4
	ldr	ip, [ip, #68]
	blx	ip
	b	.L40
.L42:
	ldr	r1, [r5]
	sub	r3, fp, #80
	ldr	r0, [r7, #2432]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, r4
	moveq	r0, #1
	streqb	r0, [r5, #178]
	beq	.L37
	ldr	r3, [r6, #68]
	mov	r0, r8
	ldr	r1, .L46+8
	blx	r3
	mov	r0, r8
	b	.L37
.L30:
	strb	r3, [fp, #-72]
	mov	r3, #1
	strb	r7, [fp, #-70]
	b	.L32
.L41:
	ldr	r0, [r2, #56]
	ldr	r4, [r1, #736]
	cmp	r0, r4
	bhi	.L45
	ldr	r3, [r2, #60]
	ldr	r10, [r1, #740]
	cmp	r3, r10
	bhi	.L21
	ldrb	r0, [r5, #178]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L37
	ldr	r2, [r1, #764]
	ldr	r3, [r1, #760]
	cmp	r2, #0
	add	r8, r3, #2
	beq	.L35
	add	r3, r3, #1
	cmp	r3, #17
	movlt	r9, r3
	movge	r9, #17
	b	.L29
.L43:
	ldr	ip, [r1, #740]
	ldr	r3, [r2, #60]
.L27:
	ldr	r1, .L46
	mov	r2, r4
	str	ip, [sp, #4]
	str	r0, [sp]
.L39:
	ldr	ip, [r1, #68]
	mov	r0, #0
	ldr	r1, .L46+12
	blx	ip
	b	.L40
.L45:
	ldr	r10, [r1, #740]
	ldr	r3, [r2, #60]
.L21:
	ldr	r1, .L46
	mov	r2, r0
	str	r10, [sp, #4]
	str	r4, [sp]
	b	.L39
.L44:
	add	r3, r9, #1
	cmp	r3, #17
	movlt	r9, r3
	movge	r9, #17
	b	.L29
.L47:
	.align	2
.L46:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	.word	.LC6
	.word	.LC5
	UNWIND(.fnend)
	.size	Divx3_ArrangeVHBMem, .-Divx3_ArrangeVHBMem
	.align	2
	.global	Divx3_SetImgFormat
	.type	Divx3_SetImgFormat, %function
Divx3_SetImgFormat:
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
	add	r5, r0, #32768
	add	r3, r0, #65536
	mov	r6, r0
	ldr	r1, [r5, #996]
	ldr	r0, [r3, #2432]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r5, #784]
	mov	r1, #0
	str	r1, [r4, #148]
	and	r3, r3, #3
	str	r3, [r4, #152]
	orr	r2, r3, #7168
	str	r3, [r4, #156]
	orr	r2, r2, #32
	str	r2, [r4, #160]
	ldr	r3, [r5, #832]
	mov	r3, r3, asl #4
	str	r3, [r4, #164]
	ldr	r3, [r5, #836]
	mov	r3, r3, asl #4
	str	r3, [r4, #168]
	bl	SetAspectRatio
	ldr	r3, [r6]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L54
.L50:
	mov	r2, #5
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
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L54:
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
	b	.L50
	UNWIND(.fnend)
	.size	Divx3_SetImgFormat, .-Divx3_SetImgFormat
	.align	2
	.global	Divx3_RecordVFMWState
	.type	Divx3_RecordVFMWState, %function
Divx3_RecordVFMWState:
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
	.size	Divx3_RecordVFMWState, .-Divx3_RecordVFMWState
	.align	2
	.global	Divx3_WritePicMsg
	.type	Divx3_WritePicMsg, %function
Divx3_WritePicMsg:
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
	add	r3, r0, #32768
	ldr	r2, [r3, #784]
	str	r2, [r0, #12]
	ldr	r2, [r3, #832]
	str	r2, [r0, #16]
	ldr	r2, [r3, #836]
	str	r2, [r0, #20]
	ldr	r2, [r3, #788]
	str	r2, [r0, #24]
	ldr	r2, [r3, #792]
	str	r2, [r0, #28]
	ldr	r2, [r3, #800]
	str	r2, [r0, #32]
	ldr	r2, [r3, #808]
	str	r2, [r0, #36]
	ldr	r2, [r3, #812]
	str	r2, [r0, #40]
	ldr	r2, [r3, #816]
	str	r2, [r0, #44]
	ldr	r2, [r3, #820]
	str	r2, [r0, #48]
	ldr	r2, [r3, #824]
	str	r2, [r0, #52]
	ldr	r3, [r3, #828]
	str	r3, [r0, #56]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	Divx3_WritePicMsg, .-Divx3_WritePicMsg
	.align	2
	.global	Divx3_WriteReg
	.type	Divx3_WriteReg, %function
Divx3_WriteReg:
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
	.size	Divx3_WriteReg, .-Divx3_WriteReg
	.align	2
	.global	Divx3_DecodeDecide
	.type	Divx3_DecodeDecide, %function
Divx3_DecodeDecide:
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
	ldrb	r5, [r0, #178]	@ zero_extendqisi2
	add	r6, r0, #32768
	mov	r4, r0
	cmp	r5, #0
	ldr	r7, [r6, #784]
	beq	.L87
	ldrb	r3, [r0, #179]	@ zero_extendqisi2
	sub	r3, r3, #2
	cmp	r3, #1
	ldr	r3, [r0]
	ldr	r2, [r3, #8]
	bls	.L88
.L62:
	uxtb	r3, r2
	strb	r3, [r4, #179]
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L68
.L70:
	.word	.L66
	.word	.L66
	.word	.L63
	.word	.L65
	.word	.L66
.L89:
	ldr	r3, .L92
	mov	r2, r7
	ldr	r1, .L92+4
	mov	r0, #18
	ldr	r3, [r3, #68]
	blx	r3
.L65:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L88:
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L62
	ldr	ip, .L92
	add	r5, r0, #65536
	ldr	r1, .L92+8
	mov	r0, #2
	mov	r8, #0
	ldr	r3, [ip, #68]
	blx	r3
	mov	r2, #1
	ldr	r0, [r5, #2432]
	ldr	r1, [r6, #992]
	bl	FSP_ClearLogicFs
	str	r8, [r6, #992]
	ldr	r0, [r5, #2432]
	bl	FSP_GetNullLogicFs
	str	r8, [r6, #908]
	str	r0, [r6, #992]
	ldr	r3, [r4]
	ldr	r2, [r3, #8]
	b	.L62
.L87:
	ldr	r3, .L92
	mov	r0, #1
	ldr	r1, .L92+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L63:
	cmp	r7, #0
	bne	.L89
.L64:
	add	r4, r4, #65536
	ldr	r1, [r6, #992]
	ldr	r0, [r4, #2432]
	bl	FSP_GetFsImagePtr
.L85:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L66:
	cmp	r7, #0
	beq	.L64
	ldr	r3, [r6, #908]
	cmp	r3, #0
	beq	.L90
	add	r3, r4, #65536
	ldr	r1, [r6, #992]
	ldr	r0, [r3, #2432]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L91
	ldr	r3, [r4]
	ldr	r2, [r5, #200]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L85
	ldr	ip, .L92
	mov	r0, #1
	ldr	r1, .L92+16
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L68:
	ldr	r3, .L92
	mov	r0, #1
	ldr	r1, .L92+20
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L90:
	ldr	ip, .L92
	mov	r0, #18
	ldr	r2, .L92+24
	ldr	r1, .L92+28
	ldr	ip, [ip, #68]
	blx	ip
	b	.L65
.L91:
	ldr	ip, .L92
	mov	r3, #456
	ldr	r2, .L92+32
	ldr	r1, .L92+36
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L93:
	.align	2
.L92:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC9
	.word	.LC8
	.word	.LC7
	.word	.LC14
	.word	.LC12
	.word	.LC10
	.word	.LC11
	.word	.LANCHOR0
	.word	.LC13
	UNWIND(.fnend)
	.size	Divx3_DecodeDecide, .-Divx3_DecodeDecide
	.align	2
	.global	Divx3_GetPictureHeader
	.type	Divx3_GetPictureHeader, %function
Divx3_GetPictureHeader:
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
	mov	r4, r1
	mov	r1, #2
	mov	r6, r2
	mov	r5, r0
	bl	BsGet
	cmp	r0, #1
	str	r0, [r4]
	bls	.L95
.L98:
	mov	r3, #0
.L96:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L95:
	mov	r1, #5
	mov	r0, r5
	bl	BsGet
	ldr	r3, [r4]
	cmp	r3, #0
	str	r0, [r4, #4]
	beq	.L116
	cmp	r6, #0
	mov	r1, #1
	streq	r6, [r4, #8]
	mov	r0, r5
	ldrne	r3, [r4, #8]
	str	r6, [r4, #20]
	rsbne	r3, r3, #1
	strne	r3, [r4, #8]
	bl	BsGet
	mov	r1, #2
	str	r0, [r4, #16]
	mov	r0, r5
	bl	BsShow
	str	r0, [r4, #36]
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L111
.L113:
	.word	.L112
	.word	.L112
	.word	.L115
	.word	.L115
.L115:
	mov	r1, #2
	mov	r0, r5
	bl	BsSkip
.L111:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #40]
	mov	r0, r5
	bl	BsGet
	mov	r3, #1
	str	r0, [r4, #44]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L116:
	mov	r3, #1
	mov	r1, #5
	str	r3, [r4, #8]
	mov	r0, r5
	bl	BsGet
	sub	r3, r0, #23
	str	r0, [r4, #12]
	cmp	r0, #26
	cmpne	r3, #1
	bhi	.L98
	mov	r1, #2
	mov	r0, r5
	bl	BsShow
	str	r0, [r4, #24]
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L99
.L101:
	.word	.L100
	.word	.L100
	.word	.L103
	.word	.L103
.L112:
	mov	r1, #1
	mov	r0, r5
	bl	BsSkip
	b	.L111
.L103:
	mov	r1, #2
	mov	r0, r5
	bl	BsSkip
.L99:
	mov	r1, #2
	mov	r0, r5
	bl	BsShow
	str	r0, [r4, #28]
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L104
.L106:
	.word	.L105
	.word	.L105
	.word	.L108
	.word	.L108
.L108:
	mov	r1, #2
	mov	r0, r5
	bl	BsSkip
.L104:
	mov	r0, r5
	mov	r1, #1
	bl	BsGet
	mov	r3, #1
	str	r0, [r4, #32]
	b	.L96
.L100:
	mov	r1, #1
	mov	r0, r5
	bl	BsSkip
	b	.L99
.L105:
	mov	r1, #1
	mov	r0, r5
	bl	BsSkip
	b	.L104
	UNWIND(.fnend)
	.size	Divx3_GetPictureHeader, .-Divx3_GetPictureHeader
	.align	2
	.global	DIVX3DEC_Init
	.type	DIVX3DEC_Init, %function
DIVX3DEC_Init:
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
	ldr	r8, .L121
	add	r6, r0, #32768
	movw	r2, #2440
	mov	r7, r1
	ldr	r9, [r6, #1384]
	mov	r1, #0
	movt	r2, 1
	ldr	r3, [r8, #48]
	mov	r4, r0
	mov	r5, r1
	blx	r3
	add	r0, r4, #33792
	add	r0, r0, #16
	str	r9, [r6, #1384]
	bl	ResetVoQueue
	str	r7, [r4]
	mov	r3, #1
	str	r3, [r6, #880]
	ldr	r3, [r7, #8]
	mov	r0, r4
	strb	r5, [r4, #178]
	strb	r3, [r4, #179]
	add	r4, r4, #65536
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #2432]
	beq	.L120
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L120:
	ldr	r3, [r8, #68]
	mov	r0, r5
	ldr	r1, .L121+4
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L122:
	.align	2
.L121:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC15
	UNWIND(.fnend)
	.size	DIVX3DEC_Init, .-DIVX3DEC_Init
	.align	2
	.global	DIVX3DEC_Destroy
	.type	DIVX3DEC_Destroy, %function
DIVX3DEC_Destroy:
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
	ldr	r3, .L124
	add	r2, r0, #32768
	ldr	r1, .L124+4
	mov	r0, #2
	ldr	r2, [r2, #904]
	ldr	r3, [r3, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L125:
	.align	2
.L124:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC16
	UNWIND(.fnend)
	.size	DIVX3DEC_Destroy, .-DIVX3DEC_Destroy
	.align	2
	.global	DIVX3DEC_Decode
	.type	DIVX3DEC_Decode, %function
DIVX3DEC_Decode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 160
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #180)
	sub	sp, sp, #180
	mov	r2, #96
	mov	r5, r1
	mov	r6, r0
	mov	r1, #0
	sub	r0, fp, #140
	bl	memset
	cmp	r5, #0
	mov	r4, #0
	str	r4, [fp, #-180]
	str	r4, [fp, #-176]
	str	r4, [fp, #-172]
	str	r4, [fp, #-168]
	str	r4, [fp, #-164]
	str	r4, [fp, #-160]
	str	r4, [fp, #-156]
	str	r4, [fp, #-152]
	str	r4, [fp, #-148]
	str	r4, [fp, #-144]
	beq	.L197
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L198
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L199
	ldr	r3, [r5, #12]
	cmp	r3, #9
	ble	.L200
	ldr	r3, .L210
	add	r10, r6, #8
	mov	r2, #156
	mov	r1, r4
	mov	r0, r10
	add	r7, r6, #32768
	ldr	r3, [r3, #48]
	blx	r3
	str	r4, [r6, #244]
	ldr	r3, [r5, #12]
	str	r3, [r7, #888]
	ldr	r3, [r5, #8]
	str	r3, [r6, #248]
	ldr	r3, [r5]
	str	r3, [r6, #168]
	ldr	r2, [r5, #12]
	str	r2, [r6, #252]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	str	r3, [r6, #256]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L132
	ldr	r3, [r5, #88]
	str	r3, [r6, #260]
	ldr	r3, [r5, #80]
	str	r3, [r6, #172]
	ldr	r3, [r5, #92]
	str	r3, [r6, #264]
	ldrb	r3, [r5, #85]	@ zero_extendqisi2
	str	r3, [r6, #268]
	ldrb	r3, [r5, #85]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L201
.L133:
	movw	r3, #65525
	sub	r1, r2, #10
	movt	r3, 47
	cmp	r1, r3
	movhi	r4, #0
	bhi	.L128
	ldr	r1, [r6, #168]
	cmp	r1, #0
	moveq	r4, r1
	beq	.L128
	add	r3, r6, #33536
	add	r3, r3, #240
	str	r3, [fp, #-192]
	mov	r8, r3
	mov	r0, r3
	bl	BsInit
	mov	r0, r8
	bl	BsResidBits
	mov	r1, #32
	str	r0, [fp, #-196]
	mov	r0, r8
	bl	BsGet
	mov	r1, #32
	rev	r4, r0
	mov	r0, r8
	add	r3, r4, #15
	str	r4, [fp, #-84]
	mov	r3, r3, lsr #4
	str	r3, [fp, #-92]
	bl	BsGet
	sub	r1, r4, #32
	rev	r3, r0
	str	r3, [fp, #-80]
	sub	r0, r3, #32
	add	r2, r3, #15
	cmp	r1, #1888
	cmpls	r0, #1056
	mov	r2, r2, lsr #4
	str	r2, [fp, #-88]
	movhi	ip, #1
	movls	ip, #0
	bhi	.L202
	ldr	r3, [r7, #792]
	sub	r1, fp, #140
	ldr	r2, [r7, #880]
	ldr	r0, [fp, #-192]
	str	ip, [fp, #-200]
	str	r3, [fp, #-132]
	bl	Divx3_GetPictureHeader
	ldr	ip, [fp, #-200]
	cmp	r0, #1
	mov	r4, r0
	bne	.L203
	ldr	r3, [r7, #908]
	cmp	r3, #0
	bne	.L136
	ldr	r2, [fp, #-140]
	cmp	r2, #0
	bne	.L204
.L136:
	ldr	r2, [fp, #-92]
	ldr	r1, [fp, #-88]
	ldrb	r0, [r6, #178]	@ zero_extendqisi2
	mul	r3, r2, r1
	cmp	r0, #0
	str	r3, [fp, #-76]
	beq	.L137
	ldr	r3, [r7, #832]
	cmp	r2, r3
	beq	.L205
.L137:
	ldr	r3, .L210+4
	ldr	ip, [r3]
	cmp	ip, #0
	addeq	r3, r6, #65536
	streq	r3, [fp, #-200]
	beq	.L139
	add	r3, r6, #65536
	ldr	r0, [r7, #836]
	ldr	lr, [r7, #832]
	mov	r8, r3
	str	r3, [fp, #-200]
	strh	r2, [fp, #-184]	@ movhi
	mov	r3, #8
	strh	r1, [fp, #-182]	@ movhi
	sub	r2, fp, #188
	strh	r0, [fp, #-186]	@ movhi
	mov	r1, #2
	strh	lr, [fp, #-188]	@ movhi
	ldr	r0, [r8, #2432]
	blx	ip
.L139:
	ldr	r8, [fp, #-200]
	mov	r2, #1
	ldr	r1, [r7, #992]
	ldr	r0, [r8, #2432]
	bl	FSP_ClearLogicFs
	mov	r2, #0
	str	r2, [r7, #992]
	add	r2, r6, #33792
	ldr	r0, [r8, #2432]
	mov	r9, r2
	bl	FSP_GetNullLogicFs
	add	r1, r9, #16
	mov	r2, #0
	str	r2, [r7, #908]
	mov	r9, r1
	str	r0, [r7, #992]
	ldr	r0, [r8, #2432]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [fp, #-92]
	ldr	r3, [fp, #-88]
	cmp	r2, #512
	bhi	.L141
	cmp	r3, #512
	bhi	.L141
	ldr	r1, [r6]
	ldr	r0, [r1, #776]
	cmp	r0, #0
	bne	.L143
	ldr	r1, [r1, #28]
	cmp	r1, #25
	beq	.L206
.L143:
	ldr	r3, [fp, #-200]
	ldr	r0, [r3, #2432]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L145
.L196:
	ldr	r2, [fp, #-92]
	ldr	r3, [fp, #-88]
.L144:
	ldr	r1, .L210
	mov	r0, #1
	ldr	ip, [r1, #68]
	ldr	r1, .L210+8
	blx	ip
	sub	r2, fp, #140
	mov	r1, r6
	mov	r0, #6
	bl	Divx3_ArrangeVHBMem
	cmp	r0, #1
	bne	.L207
.L138:
	ldr	r3, [fp, #-140]
	add	r0, r6, #33536
	sub	r1, fp, #140
	mov	r2, #96
	cmp	r3, #0
	add	r0, r0, #16
	ldrne	r3, [r7, #796]
	moveq	r3, #23
	mov	r8, r3
	ldr	r3, .L210
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r3, [fp, #-140]
	mov	r0, r6
	cmp	r3, #0
	movne	r3, r8
	strne	r3, [r7, #796]
	bl	Divx3_DecodeDecide
	cmp	r0, #1
	beq	.L148
	ldr	r3, .L210
	mov	r0, #1
	ldr	r1, .L210+12
	mov	r4, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L128
.L200:
	ldr	r3, .L210
	mov	r0, #1
	ldr	r1, .L210+16
	ldr	r3, [r3, #68]
	blx	r3
.L128:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L201:
	add	r6, r6, #65536
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #2432]
	bl	SM_ReleaseStreamSeg
	ldr	r0, [r6, #2432]
	ldr	r1, [r5, #96]
	bl	SM_ReleaseStreamSeg
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L198:
	ldr	r2, .L210
	mov	r4, r3
	ldr	r1, .L210+20
	mov	r0, #1
	ldr	r3, [r2, #68]
	blx	r3
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L132:
	str	r4, [r6, #260]
	str	r4, [r6, #172]
	str	r4, [r6, #264]
	str	r4, [r6, #268]
	b	.L133
.L202:
	ldr	r2, .L210
	mov	r0, #1
	ldr	r1, .L210+24
	ldr	ip, [r2, #68]
	mov	r2, r4
	mov	r4, #0
	blx	ip
	b	.L128
.L203:
	ldr	r3, .L210
	mov	r0, #1
	ldr	r1, .L210+28
	mov	r4, ip
	ldr	r3, [r3, #68]
	blx	r3
	b	.L128
.L197:
	ldr	r3, .L210
	add	r6, r6, #32768
	ldr	r1, .L210+32
	mov	r0, #1
	ldr	r2, [r6, #904]
	mov	r4, r5
	ldr	r3, [r3, #68]
	blx	r3
	b	.L128
.L199:
	ldr	r2, .L210
	mov	r4, r3
	ldr	r1, .L210+36
	mov	r0, #1
	ldr	r3, [r2, #68]
	blx	r3
	b	.L128
.L141:
	ldr	r1, .L210
	mov	r0, #18
	mov	r4, #0
	ldr	ip, [r1, #68]
	ldr	r1, .L210+40
	blx	ip
	b	.L128
.L207:
	ldr	r3, .L210
	mov	r0, #1
	ldr	r1, .L210+44
	mov	r4, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L128
.L205:
	ldr	r3, [r7, #836]
	cmp	r1, r3
	bne	.L137
	b	.L138
.L206:
	ldrb	r1, [r6, #178]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L144
.L145:
	mov	r0, r9
	bl	ResetVoQueue
	b	.L196
.L148:
	str	r0, [fp, #-200]
	mov	r0, r6
	bl	Divx3_GetImageBuffer
	ldr	ip, [fp, #-200]
	subs	r3, r0, #0
	beq	.L208
	add	r3, r6, #65536
	ldr	r1, [r7, #996]
	str	ip, [fp, #-204]
	ldr	r0, [r3, #2432]
	str	r3, [fp, #-200]
	bl	FSP_GetFsImagePtr
	ldr	ip, [fp, #-204]
	subs	r3, r0, #0
	beq	.L150
	ldr	r2, [r6]
	mov	r0, #29
	ldr	ip, .L210
	ldr	r1, .L210+48
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
	ldrd	r2, [r3, #8]
	strd	r8, [sp]
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r8, [fp, #-200]
	ldr	r1, [r7, #996]
	ldr	r0, [r8, #2432]
	bl	FSP_GetLogicFs
	ldr	r1, [r7, #992]
	mov	r9, r0
	ldr	r0, [r8, #2432]
	bl	FSP_GetLogicFs
	cmp	r9, #0
	cmpne	r0, #0
	beq	.L151
	ldr	r3, [r9, #520]
	mov	r1, r9
	cmp	r3, #0
	beq	.L151
	ldr	r2, [r9, #528]
	cmp	r2, #0
	beq	.L151
	ldr	r2, [r9, #524]
	cmp	r2, #0
	beq	.L151
	ldr	r2, [r0, #520]
	mov	r8, r9
	mov	ip, #0
	mov	r0, #13
	cmp	r2, #0
	str	ip, [fp, #-204]
	ldrne	r1, [r2, #12]
	ldreq	r1, [r3, #12]
	ldr	r2, .L210
	str	r1, [r6, #132]
	ldr	r2, [r2, #68]
	ldr	r3, [r9, #520]
	str	r2, [fp, #-200]
	ldr	r2, [r3, #12]
	ldr	ip, [fp, #-200]
	str	r2, [r6, #136]
	ldr	lr, [r9, #532]
	str	lr, [r6, #152]
	ldr	r3, [r9, #524]
	ldr	r9, [r6, #156]
	ldr	r3, [r3, #12]
	stmia	sp, {r1, lr}
	ldr	r1, .L210+52
	str	r3, [r6, #140]
	str	r9, [sp, #8]
	blx	ip
	ldr	r3, [r8, #524]
	ldr	r8, [fp, #-192]
	ldr	r9, .L210
	ldr	r3, [r3, #20]
	mov	r0, r8
	str	r3, [r6, #144]
	bl	BsResidBits
	ldr	r3, [r5, #8]
	ldr	r2, [fp, #-196]
	bic	r3, r3, #15
	str	r3, [r6, #104]
	ldr	r3, [r5, #8]
	and	r3, r3, #15
	add	r3, r2, r3, lsl #3
	rsb	r3, r0, r3
	mov	r0, r8
	and	r2, r3, #127
	str	r2, [r6, #116]
	mov	r3, r3, lsr #7
	mov	r3, r3, asl #4
	str	r3, [r6, #108]
	bl	BsResidBits
	ldr	ip, [fp, #-204]
	str	ip, [r6, #112]
	str	ip, [r6, #120]
	str	ip, [r6, #128]
	str	r0, [r6, #124]
	ldr	r3, [r7, #796]
	cmp	r3, #24
	beq	.L157
	cmp	r3, #26
	beq	.L158
	cmp	r3, #23
	beq	.L209
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L210+56
	mov	r4, ip
	blx	r3
	b	.L128
.L204:
	ldr	r2, .L210
	mov	r4, r3
	ldr	r1, .L210+60
	ldr	r2, [r2, #68]
	blx	r2
	b	.L128
.L208:
	ldr	r2, .L210
	mov	r4, r3
	ldr	r1, .L210+64
	ldr	ip, [r2, #68]
	ldr	r2, [r7, #904]
	blx	ip
	b	.L128
.L151:
	ldr	r3, [fp, #-200]
	mov	r2, #1
	ldr	r1, [r7, #996]
	mov	r4, #0
	ldr	r0, [r3, #2432]
	bl	FSP_ClearLogicFs
	ldr	r3, .L210
	ldr	r1, .L210+68
	mov	r0, r4
	ldr	r3, [r3, #68]
	blx	r3
	b	.L128
.L150:
	ldr	r2, .L210
	mov	r0, ip
	ldr	r1, .L210+72
	mov	r4, r3
	ldr	r2, [r2, #68]
	blx	r2
	b	.L128
.L157:
	ldr	r2, [r7, #836]
	ldr	r3, [r7, #832]
	tst	r2, #1
	str	ip, [fp, #-180]
	mov	r2, r2, lsr #1
	moveq	lr, #2
	mul	r3, r3, r2
	sub	r2, r3, #1
	str	r2, [fp, #-160]
	mov	r2, r3, asl #1
	str	r3, [fp, #-176]
	sub	r3, r2, #1
	str	r3, [fp, #-156]
	beq	.L160
	ldr	r3, [r7, #848]
	mov	lr, #3
	str	r2, [fp, #-172]
	sub	r3, r3, #1
	str	r3, [fp, #-152]
.L160:
	ldr	r3, [r7, #784]
	cmp	lr, #0
	str	r3, [r6, #12]
	ldr	r3, [r7, #832]
	str	r3, [r6, #16]
	ldr	r3, [r7, #836]
	str	r3, [r6, #20]
	ldr	r3, [r7, #788]
	str	r3, [r6, #24]
	ldr	r3, [r7, #792]
	str	r3, [r6, #28]
	ldr	r3, [r7, #800]
	str	r3, [r6, #32]
	ldr	r3, [r7, #808]
	str	r3, [r6, #36]
	ldr	r3, [r7, #812]
	str	r3, [r6, #40]
	ldr	r3, [r7, #816]
	str	r3, [r6, #44]
	ldr	r3, [r7, #820]
	str	r3, [r6, #48]
	ldr	r3, [r7, #824]
	str	r3, [r6, #52]
	ldr	r3, [r7, #828]
	str	lr, [r6, #60]
	str	r3, [r6, #56]
	beq	.L163
	sub	ip, fp, #180
	add	r0, r6, #60
	sub	r1, fp, #160
	add	r2, r6, #80
	mov	r3, #0
.L162:
	add	r3, r3, #1
	ldr	r7, [ip], #4
	ldr	r5, [r1], #4
	cmp	r3, lr
	str	r7, [r0, #4]!
	str	r5, [r2, #4]!
	bne	.L162
.L163:
	ldr	r3, [r6]
	str	r10, [r6, #4]
	ldr	r3, [r3, #676]
	str	r3, [r6, #160]
	b	.L128
.L209:
	ldr	r3, [r7, #848]
	mov	lr, #1
	str	ip, [fp, #-180]
	sub	r3, r3, #1
	str	r3, [fp, #-160]
	b	.L160
.L158:
	ldr	r2, [r7, #836]
	ldr	r3, [r7, #832]
	tst	r2, #3
	str	ip, [fp, #-180]
	mov	r2, r2, lsr #2
	moveq	lr, #4
	mul	r3, r3, r2
	sub	r2, r3, #1
	str	r2, [fp, #-160]
	mov	r2, r3, asl #1
	mov	r1, r3, asl #2
	add	r0, r2, r3
	str	r3, [fp, #-176]
	str	r2, [fp, #-172]
	sub	r3, r0, #1
	sub	r2, r2, #1
	str	r3, [fp, #-152]
	str	r0, [fp, #-168]
	sub	r3, r1, #1
	str	r2, [fp, #-156]
	str	r3, [fp, #-148]
	beq	.L160
	ldr	r3, [r7, #848]
	mov	lr, #5
	str	r1, [fp, #-164]
	sub	r3, r3, #1
	str	r3, [fp, #-144]
	b	.L160
.L211:
	.align	2
.L210:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC25
	.word	.LC27
	.word	.LC20
	.word	.LC18
	.word	.LC21
	.word	.LC22
	.word	.LC17
	.word	.LC19
	.word	.LC24
	.word	.LC26
	.word	.LC29
	.word	.LC32
	.word	.LC33
	.word	.LC23
	.word	.LC28
	.word	.LC31
	.word	.LC30
	UNWIND(.fnend)
	.size	DIVX3DEC_Decode, .-DIVX3DEC_Decode
	.align	2
	.global	DIVX3DEC_RecycleImage
	.type	DIVX3DEC_RecycleImage, %function
DIVX3DEC_RecycleImage:
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
	add	r4, r0, #65536
	mov	r2, #0
	ldr	r0, [r4, #2432]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #2432]
	bl	FSP_GetFsImagePtr
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	DIVX3DEC_RecycleImage, .-DIVX3DEC_RecycleImage
	.align	2
	.global	DIVX3DEC_GetRemainImg
	.type	DIVX3DEC_GetRemainImg, %function
DIVX3DEC_GetRemainImg:
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
	beq	.L230
	add	r7, r0, #32768
	add	r6, r0, #65536
	ldr	r1, [r7, #996]
	ldr	r0, [r6, #2432]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L230
	ldr	r1, [r7, #996]
	ldr	r0, [r6, #2432]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L229
.L230:
	add	r5, r4, #33792
	add	r5, r5, #16
.L220:
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	GetVoLastImageID
.L229:
	mov	r0, r4
	add	r5, r4, #33792
	bl	Divx3_SetImgFormat
	add	r5, r5, #16
	mov	r3, #1
	str	r3, [r8, #244]
	mov	r2, r3
	ldr	r1, [r7, #996]
	ldr	r0, [r6, #2432]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #2432]
	mov	r2, r4
	str	r8, [sp]
	mov	r3, r5
	mov	r1, #5
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L217
	mov	r2, #0
	ldr	r1, [r7, #996]
	ldr	r0, [r6, #2432]
	mov	r4, r2
	bl	FSP_SetDisplay
	ldr	r1, [r8, #212]
	ldr	r0, [r6, #2432]
	bl	FreeUsdByDec
	str	r4, [r8, #212]
	ldr	r1, [r8, #216]
	ldr	r0, [r6, #2432]
	bl	FreeUsdByDec
	str	r4, [r8, #216]
	b	.L220
.L217:
	ldr	r3, [r8, #200]
	cmp	r3, #100
	bhi	.L220
	ldr	r3, [r8, #164]
	cmp	r3, #31
	bls	.L220
	ldr	r3, [r8, #168]
	cmp	r3, #31
	bls	.L220
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	DIVX3DEC_GetRemainImg, .-DIVX3DEC_GetRemainImg
	.align	2
	.global	DIVX3DEC_VDMPostProc
	.type	DIVX3DEC_VDMPostProc, %function
DIVX3DEC_VDMPostProc:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r8, .L261
	mov	r6, r1
	mov	r5, r0
	ldr	r1, .L261+4
	mov	r9, r2
	ldr	r3, [r8, #68]
	mov	r2, r6
	mov	r0, #22
	add	r4, r5, #32768
	add	r7, r5, #65536
	blx	r3
	mov	r1, r5
	mov	r0, #5
	bl	ReleasePacket
	ldr	r1, [r4, #996]
	ldr	r0, [r7, #2432]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L258
	ldr	r3, [r4, #784]
	cmp	r3, #0
	andeq	r9, r9, #1
	streq	r9, [r4, #880]
	cmp	r6, #0
	movlt	r6, #0
	blt	.L235
	cmp	r6, #100
	movge	r6, #100
.L235:
	ldr	r3, [r8, #68]
	str	r6, [r0, #200]
	mov	r0, #22
	ldr	r1, .L261+8
	blx	r3
	mov	r0, r5
	bl	Divx3_SetImgFormat
	ldrb	r3, [r5, #179]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L238
	ldr	r3, [r5]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L238
	ldr	r3, [r4, #908]
	cmp	r3, #0
	ldrne	r1, [r4, #992]
	mvneq	r1, #0
	str	r1, [r4, #1000]
.L237:
	cmn	r1, #1
	beq	.L240
	ldr	r0, [r7, #2432]
	bl	FSP_GetFsImagePtr
	ldr	r6, [r4, #1000]
	ldr	r10, [r8, #68]
	mov	r1, r6
	mov	r9, r0
	ldr	r0, [r7, #2432]
	bl	FSP_GetRef
	mov	r2, r6
	ldr	r1, .L261+12
	mov	r3, r0
	mov	r0, #12
	blx	r10
	mov	r2, #1
	ldr	r1, [r4, #1000]
	ldr	r0, [r7, #2432]
	bl	FSP_SetDisplay
	add	r3, r5, #33792
	ldr	r0, [r7, #2432]
	add	r3, r3, #16
	str	r9, [sp]
	mov	r2, r5
	mov	r1, #5
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L259
.L240:
	ldr	r2, [r4, #784]
	ldr	r3, [r4, #904]
	cmp	r2, #1
	add	r3, r3, #1
	str	r3, [r4, #904]
	beq	.L260
.L241:
	ldr	r2, [r4, #992]
	mov	r3, #1
	ldr	r1, [r4, #996]
	str	r3, [r4, #908]
	cmp	r2, r1
	beq	.L243
	ldr	r3, [r8, #68]
	mov	r0, #13
	ldr	r1, .L261+16
	blx	r3
	mov	r2, #0
	ldr	r1, [r4, #992]
	ldr	r0, [r7, #2432]
	bl	FSP_SetRef
	ldr	r1, [r4, #992]
	ldr	r0, [r7, #2432]
	bl	FSP_RemovePmv
	ldr	r1, [r4, #996]
.L243:
	str	r1, [r4, #992]
	mov	r2, #1
	ldr	r0, [r7, #2432]
	bl	FSP_SetRef
	ldr	r3, [r8, #68]
	ldr	r2, [r4, #996]
	mov	r0, #13
	ldr	r1, .L261+20
	blx	r3
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L238:
	ldr	r1, [r4, #996]
	str	r1, [r4, #1000]
	b	.L237
.L259:
	ldr	r1, [r4, #1000]
	mov	r2, #0
	ldr	r0, [r7, #2432]
	bl	FSP_SetDisplay
.L258:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L260:
	ldr	r3, [r4, #908]
	cmp	r3, #0
	bne	.L241
	b	.L258
.L262:
	.align	2
.L261:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	UNWIND(.fnend)
	.size	DIVX3DEC_VDMPostProc, .-DIVX3DEC_VDMPostProc
	.align	2
	.global	DIVX3DEC_GetImageBuffer
	.type	DIVX3DEC_GetImageBuffer, %function
DIVX3DEC_GetImageBuffer:
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
	add	r4, r0, #65536
	mov	r5, r0
	ldr	r0, [r4, #2432]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #2432]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L269
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L269:
	add	r1, r5, #33792
	ldr	r0, [r4, #2432]
	add	r1, r1, #16
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	DIVX3DEC_GetImageBuffer, .-DIVX3DEC_GetImageBuffer
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__FUNCTION__.13609, %object
	.size	__FUNCTION__.13609, 19
__FUNCTION__.13609:
	.ascii	"Divx3_DecodeDecide\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC1:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC2:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC3:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"image size abnormal (%dx%d) in MB\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"divx3 actual frame size(%dx%d) exeed max config(%dx" )
	ASCII(.ascii	"%d)\012\000" )
.LC6:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"ERROR: No Img buffer is allocated\012\000" )
	.space	1
.LC8:
	ASCII(.ascii	"----------------- divx3 Imode -> %d ---------------" )
	ASCII(.ascii	"---\012\000" )
.LC9:
	ASCII(.ascii	"-----I_MODE, but FrameType is %d-----\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"P-frm\000" )
	.space	2
.LC11:
	ASCII(.ascii	"-----cur framtype is %s, but refnum=%d-----\012\000" )
	.space	3
.LC12:
	ASCII(.ascii	"error: DecMode illegal\012\000" )
.LC13:
	ASCII(.ascii	"fuction return value is null,%s %d unknow error!!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC14:
	ASCII(.ascii	"dec P frame, fwd ref err(%d) > ref_thr(%d)\012\000" )
.LC15:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC16:
	ASCII(.ascii	"Decoding quits at frm %d\012\000" )
	.space	2
.LC17:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet pointer!\012\000" )
.LC18:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet PhyAddr!\012\000" )
.LC19:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet VirAddr!\012\000" )
.LC20:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet len!\012\000" )
.LC21:
	ASCII(.ascii	"ERROR: frm %d, Invalid WxH: %dx%d!\012\000" )
.LC22:
	ASCII(.ascii	"Divx3_GetPictureHeader Error!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"Can't decode from P frame!\012\000" )
.LC24:
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC25:
	ASCII(.ascii	"new seq size %d * %d\012\000" )
	.space	2
.LC26:
	ASCII(.ascii	"Divx3_ArrangeVHBMem err\012\000" )
	.space	3
.LC27:
	ASCII(.ascii	"Divx3_DecodeDecide err\012\000" )
.LC28:
	ASCII(.ascii	"Fatal Error: frm%d, get image buffer failed\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC30:
	ASCII(.ascii	"FSP_GetFsImagePtr Error!\012\000" )
	.space	2
.LC31:
	ASCII(.ascii	"CurFsID abnormal!\012\000" )
	.space	1
.LC32:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,currpmv,colpmv = %p,%p,%p,%" )
	ASCII(.ascii	"p,%p,%p\012\000" )
.LC33:
	ASCII(.ascii	"vol_mode Error!\012\000" )
	.space	3
.LC34:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC35:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC36:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.space	2
.LC37:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC38:
	ASCII(.ascii	"++++++++++ ref %d\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
