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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r4, r0, #40960
	mov	r5, r0
	mov	r1, #1
	ldr	r0, [r4, #896]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #500]
	blt	.L9
	mov	r1, r0
	ldr	r0, [r4, #896]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L10
	ldr	r2, [r4, #500]
	mov	r0, #18
	ldr	r1, .L11
	bl	dprint_vfmw
	ldr	r2, [r5, #640]
	cmp	r2, #0
	beq	.L7
	ldr	r3, [r5, #644]
	cmp	r3, #0
	beq	.L7
	ldr	r1, [r5, #648]
	cmp	r1, #0
	beq	.L7
	ldr	ip, [r1, #16]
	mov	r0, #18
	ldr	r3, [r3, #16]
	mov	r5, #1
	ldr	r2, [r2, #16]
	ldr	r1, .L11+4
	str	ip, [sp]
	bl	dprint_vfmw
.L3:
	mov	r0, r5
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L7:
	mov	r5, #1
	mov	r0, r5
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L9:
	ldr	r1, .L11+8
	mov	r0, #0
	bl	dprint_vfmw
	add	r1, r5, #41472
	add	r1, r1, #32
	ldr	r0, [r4, #896]
	bl	FSP_ClearNotInVoQueue
	mov	r2, #1
	ldr	r1, [r4, #496]
	mov	r5, #0
	ldr	r0, [r4, #896]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #496]
	ldr	r0, [r4, #896]
	bl	FSP_GetNullLogicFs
	str	r5, [r4, #412]
	str	r0, [r4, #496]
	mov	r0, r5
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L10:
	mov	r2, #79
	ldr	r1, .L11+12
	bl	dprint_vfmw
	b	.L3
.L12:
	.align	2
.L11:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #48)
	sub	sp, sp, #48
	mov	r5, r1
	ldr	r1, [r2, #48]
	ldr	r3, [r2, #52]
	cmp	r1, #1
	bls	.L15
	cmp	r3, #1
	bls	.L15
	ldr	r1, [r5]
	ldr	r3, [r1, #708]
	cmp	r3, #0
	ldr	r3, [r1, #28]
	bne	.L18
	cmp	r3, #25
	beq	.L42
.L19:
	ldr	r6, [r2, #56]
	mov	r7, #5
	ldr	r9, [r2, #60]
	mov	r4, #2
	mov	r8, #4
.L24:
	ldr	r3, .L48
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #68
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, [r5]
	mov	r2, #0
	str	r6, [fp, #-60]
	str	r9, [fp, #-56]
	ldr	r3, [r3, #28]
	strb	r2, [fp, #-67]
	cmp	r3, #24
	beq	.L30
	uxtb	r3, r4
	strb	r8, [fp, #-65]
	strb	r7, [fp, #-63]
.L34:
	add	r6, r5, #40960
	sub	r1, fp, #68
	mov	r4, #0
	strb	r3, [fp, #-62]
	ldr	r0, [r6, #896]
	strb	r4, [fp, #-66]
	bl	FSP_ConfigInstance
	cmp	r0, r4
	beq	.L43
.L40:
	mov	r0, #0
.L39:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L18:
	cmp	r3, #25
	bne	.L19
	ldr	r6, [r2, #56]
	ldr	r0, [r1, #668]
	cmp	r6, r0
	bhi	.L44
	ldr	r3, [r2, #60]
	ldr	ip, [r1, #672]
	cmp	r3, ip
	bhi	.L27
	ldr	r2, [r1, #696]
	mov	r9, r3
	ldr	r4, [r1, #692]
	cmp	r2, #0
	add	r8, r4, #2
	bne	.L45
.L37:
	mov	r4, #1
.L29:
	ldrb	r7, [r1, #716]	@ zero_extendqisi2
	b	.L24
.L15:
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L48+4
	bl	dprint_vfmw
	b	.L40
.L43:
	ldr	r1, [r5]
	sub	r3, fp, #72
	ldr	r0, [r6, #896]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L46
	cmp	r0, #0
	moveq	r0, #1
	streqb	r0, [r5, #186]
	bne	.L32
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L30:
	mov	r3, #1
	strb	r7, [fp, #-63]
	strb	r3, [fp, #-65]
	b	.L34
.L42:
	ldr	r0, [r2, #56]
	ldr	r6, [r1, #668]
	cmp	r0, r6
	bhi	.L47
	ldr	r3, [r2, #60]
	ldr	r9, [r1, #672]
	cmp	r3, r9
	bhi	.L21
	ldrb	r0, [r5, #186]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L39
	ldr	r2, [r1, #696]
	ldr	r3, [r1, #692]
	cmp	r2, #0
	add	r8, r3, #2
	beq	.L37
	add	r3, r3, #1
	cmp	r3, #17
	movlt	r4, r3
	movge	r4, #17
	b	.L29
.L44:
	ldr	ip, [r1, #672]
	ldr	r3, [r2, #60]
.L27:
	stmia	sp, {r0, ip}
	mov	r2, r6
	ldr	r1, .L48+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	b	.L39
.L47:
	ldr	r9, [r1, #672]
	ldr	r3, [r2, #60]
.L21:
	mov	r2, r0
	stmia	sp, {r6, r9}
	ldr	r1, .L48+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	b	.L39
.L45:
	add	r3, r4, #1
	cmp	r3, #17
	movlt	r4, r3
	movge	r4, #17
	b	.L29
.L46:
	add	r0, r5, #41472
	add	r0, r0, #32
	bl	ResetVoQueue
.L32:
	ldr	r1, .L48+12
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	b	.L39
.L49:
	.align	2
.L48:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	.word	.LC5
	.word	.LC6
	UNWIND(.fnend)
	.size	Divx3_ArrangeVHBMem, .-Divx3_ArrangeVHBMem
	.align	2
	.global	Divx3_SetImgFormat
	.type	Divx3_SetImgFormat, %function
Divx3_SetImgFormat:
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
	add	r6, r0, #40960
	mov	r7, r0
	ldr	r1, [r6, #500]
	ldr	r0, [r6, #896]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L50
	ldr	r2, [r6, #288]
	mov	r5, #0
	mov	r1, r5
	and	r2, r2, #3
	orr	r3, r2, #7168
	orr	r3, r3, #32
	str	r3, [r4, #108]
	ldr	ip, [r7]
	ldr	ip, [ip, #592]
	str	r2, [r4, #100]
	cmp	ip, #1
	str	r2, [r4, #104]
	str	r5, [r4, #96]
	moveq	r2, #262144
	movne	r2, r5
	orr	r3, r2, r3
	str	r3, [r4, #108]
	ldr	r3, [r6, #336]
	mov	r3, r3, asl #4
	str	r3, [r4, #112]
	ldr	r3, [r6, #340]
	mov	r3, r3, asl #4
	str	r3, [r4, #116]
	bl	SetAspectRatio
	ldr	r3, [r7]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L58
.L53:
	mov	r2, #5
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
.L50:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L58:
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
	b	.L53
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
	add	r3, r0, #40960
	ldr	r2, [r3, #288]
	str	r2, [r0, #12]
	ldr	r2, [r3, #336]
	str	r2, [r0, #16]
	ldr	r2, [r3, #340]
	str	r2, [r0, #20]
	ldr	r2, [r3, #292]
	str	r2, [r0, #24]
	ldr	r2, [r3, #296]
	str	r2, [r0, #28]
	ldr	r2, [r3, #304]
	str	r2, [r0, #32]
	ldr	r2, [r3, #312]
	str	r2, [r0, #36]
	ldr	r2, [r3, #316]
	str	r2, [r0, #40]
	ldr	r2, [r3, #320]
	str	r2, [r0, #44]
	ldr	r2, [r3, #324]
	str	r2, [r0, #48]
	ldr	r2, [r3, #328]
	str	r2, [r0, #52]
	ldr	r3, [r3, #332]
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r5, [r0, #186]	@ zero_extendqisi2
	add	r6, r0, #40960
	mov	r4, r0
	cmp	r5, #0
	ldr	r7, [r6, #288]
	beq	.L91
	ldrb	r3, [r0, #187]	@ zero_extendqisi2
	sub	r3, r3, #2
	cmp	r3, #1
	ldr	r3, [r0]
	ldr	r2, [r3, #8]
	bls	.L92
.L66:
	uxtb	r3, r2
	strb	r3, [r4, #187]
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L72
.L74:
	.word	.L70
	.word	.L70
	.word	.L67
	.word	.L69
	.word	.L70
.L93:
	mov	r2, r7
	ldr	r1, .L96
	mov	r0, #18
	bl	dprint_vfmw
.L69:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L92:
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L66
	ldr	r1, .L96+4
	mov	r0, #2
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r6, #496]
	mov	r5, #0
	ldr	r0, [r6, #896]
	bl	FSP_ClearLogicFs
	str	r5, [r6, #496]
	ldr	r0, [r6, #896]
	bl	FSP_GetNullLogicFs
	str	r5, [r6, #412]
	str	r0, [r6, #496]
	ldr	r3, [r4]
	ldr	r2, [r3, #8]
	b	.L66
.L91:
	ldr	r1, .L96+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L67:
	cmp	r7, #0
	bne	.L93
.L68:
	ldr	r1, [r6, #496]
	ldr	r0, [r6, #896]
	bl	FSP_GetFsImagePtr
.L89:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L70:
	cmp	r7, #0
	beq	.L68
	ldr	r3, [r6, #412]
	cmp	r3, #0
	beq	.L94
	ldr	r1, [r6, #496]
	ldr	r0, [r6, #896]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L95
	ldr	r3, [r4]
	ldr	r2, [r5, #160]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L89
	ldr	r1, .L96+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L72:
	ldr	r1, .L96+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L94:
	ldr	r2, .L96+20
	mov	r0, #18
	ldr	r1, .L96+24
	bl	dprint_vfmw
	b	.L69
.L95:
	movw	r3, #445
	ldr	r2, .L96+28
	ldr	r1, .L96+32
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L97:
	.align	2
.L96:
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
	bls	.L99
.L102:
	mov	r3, #0
.L100:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L99:
	mov	r1, #5
	mov	r0, r5
	bl	BsGet
	ldr	r3, [r4]
	cmp	r3, #0
	str	r0, [r4, #4]
	beq	.L120
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
	b	.L115
.L117:
	.word	.L116
	.word	.L116
	.word	.L119
	.word	.L119
.L119:
	mov	r1, #2
	mov	r0, r5
	bl	BsSkip
.L115:
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
.L120:
	mov	r3, #1
	mov	r1, #5
	str	r3, [r4, #8]
	mov	r0, r5
	bl	BsGet
	sub	r3, r0, #23
	str	r0, [r4, #12]
	cmp	r0, #26
	cmpne	r3, #1
	bhi	.L102
	mov	r1, #2
	mov	r0, r5
	bl	BsShow
	str	r0, [r4, #24]
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L103
.L105:
	.word	.L104
	.word	.L104
	.word	.L107
	.word	.L107
.L116:
	mov	r1, #1
	mov	r0, r5
	bl	BsSkip
	b	.L115
.L107:
	mov	r1, #2
	mov	r0, r5
	bl	BsSkip
.L103:
	mov	r1, #2
	mov	r0, r5
	bl	BsShow
	str	r0, [r4, #28]
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L108
.L110:
	.word	.L109
	.word	.L109
	.word	.L112
	.word	.L112
.L112:
	mov	r1, #2
	mov	r0, r5
	bl	BsSkip
.L108:
	mov	r0, r5
	mov	r1, #1
	bl	BsGet
	mov	r3, #1
	str	r0, [r4, #32]
	b	.L100
.L104:
	mov	r1, #1
	mov	r0, r5
	bl	BsSkip
	b	.L103
.L109:
	mov	r1, #1
	mov	r0, r5
	bl	BsSkip
	b	.L108
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
	ldr	r3, .L125
	add	r5, r0, #40960
	mov	r7, r1
	movw	r2, #41872
	ldr	r8, [r5, #888]
	mov	r1, #0
	ldr	r3, [r3, #48]
	mov	r4, r0
	mov	r6, r1
	blx	r3
	add	r0, r4, #41472
	str	r8, [r5, #888]
	add	r0, r0, #32
	bl	ResetVoQueue
	str	r7, [r4]
	mov	r3, #1
	str	r3, [r5, #384]
	ldr	r3, [r7, #8]
	mov	r0, r4
	strb	r6, [r4, #186]
	strb	r3, [r4, #187]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r5, #896]
	mov	r0, r6
	beq	.L124
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L124:
	ldr	r1, .L125+4
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L126:
	.align	2
.L125:
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
	add	r3, r0, #40960
	ldr	r1, .L128
	mov	r0, #2
	ldr	r2, [r3, #408]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L129:
	.align	2
.L128:
	.word	.LC16
	UNWIND(.fnend)
	.size	DIVX3DEC_Destroy, .-DIVX3DEC_Destroy
	.align	2
	.global	DIVX3DEC_Decode
	.type	DIVX3DEC_Decode, %function
DIVX3DEC_Decode:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 168
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #188)
	sub	sp, sp, #188
	mov	r2, #96
	mov	r5, r1
	mov	r9, r0
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
	beq	.L216
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L217
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L218
	ldr	r3, [r5, #12]
	cmp	r3, #9
	ble	.L219
	ldr	r7, .L230
	add	r3, r9, #40960
	add	r10, r9, #8
	mov	r2, #172
	mov	r6, r3
	str	r3, [fp, #-200]
	mov	r1, r4
	ldr	r3, [r7, #48]
	mov	r0, r10
	blx	r3
	str	r4, [r9, #260]
	ldr	r3, [r5, #12]
	str	r3, [r6, #392]
	ldr	r3, [r5, #8]
	str	r3, [r9, #264]
	ldr	r3, [r5]
	str	r3, [r9, #212]
	ldr	r2, [r5, #12]
	str	r2, [r9, #268]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	str	r3, [r9, #272]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L136
	ldr	r3, [r5, #88]
	str	r3, [r9, #276]
	ldr	r3, [r5, #80]
	str	r3, [r9, #216]
	ldr	r3, [r5, #92]
	str	r3, [r9, #280]
	ldrb	r3, [r5, #85]	@ zero_extendqisi2
	str	r3, [r9, #284]
	ldrb	r3, [r5, #85]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L220
.L137:
	movw	r3, #65525
	sub	r1, r2, #10
	movt	r3, 47
	cmp	r1, r3
	movhi	r4, #0
	bhi	.L132
	ldr	r1, [r9, #212]
	cmp	r1, #0
	moveq	r4, r1
	beq	.L132
	add	r8, r9, #41472
	mov	r0, r8
	bl	BsInit
	mov	r0, r8
	bl	BsResidBits
	mov	r1, #32
	str	r0, [fp, #-204]
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
	bhi	.L221
	ldr	r2, [fp, #-200]
	sub	r1, fp, #140
	mov	r0, r8
	str	ip, [fp, #-208]
	ldr	r3, [r2, #296]
	ldr	r2, [r2, #384]
	str	r3, [fp, #-132]
	bl	Divx3_GetPictureHeader
	ldr	ip, [fp, #-208]
	cmp	r0, #1
	mov	r4, r0
	bne	.L222
	ldr	r3, [fp, #-200]
	ldr	r3, [r3, #412]
	cmp	r3, #0
	bne	.L140
	ldr	r2, [fp, #-140]
	cmp	r2, #0
	bne	.L223
.L140:
	ldr	r2, [fp, #-92]
	ldr	r1, [fp, #-88]
	ldrb	r0, [r9, #186]	@ zero_extendqisi2
	mul	r3, r2, r1
	cmp	r0, #0
	str	r3, [fp, #-76]
	beq	.L141
	ldr	r3, [fp, #-200]
	ldr	r3, [r3, #336]
	cmp	r2, r3
	beq	.L224
.L141:
	ldr	r3, .L230+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L143
	ldr	r6, [fp, #-200]
	mov	r3, #8
	strh	r2, [fp, #-184]	@ movhi
	sub	r2, fp, #188
	strh	r1, [fp, #-182]	@ movhi
	mov	r1, #2
	ldr	r0, [r6, #340]
	ldr	lr, [r6, #336]
	strh	r0, [fp, #-186]	@ movhi
	strh	lr, [fp, #-188]	@ movhi
	ldr	r0, [r6, #896]
	blx	ip
.L143:
	ldr	r3, [fp, #-200]
	ldr	r0, [r3, #896]
	bl	FSP_GetFspType
	subs	lr, r0, #0
	bne	.L144
	ldr	r0, [fp, #-200]
	sub	r3, fp, #44
	sub	r2, fp, #192
	sub	r1, fp, #196
	str	lr, [r3, #-144]!
	ldr	r0, [r0, #896]
	str	lr, [fp, #-196]
	str	lr, [fp, #-192]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r9]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L225
.L144:
	ldr	r6, [fp, #-200]
	mov	r2, #1
	ldr	r1, [r6, #496]
	ldr	r0, [r6, #896]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	str	r3, [r6, #496]
	add	r3, r9, #41472
	ldr	r0, [r6, #896]
	str	r6, [fp, #-200]
	mov	r6, r3
	bl	FSP_GetNullLogicFs
	ldr	ip, [fp, #-200]
	add	r3, r6, #32
	mov	r2, #0
	mov	r1, r3
	str	r2, [ip, #412]
	mov	r6, r3
	str	r0, [ip, #496]
	ldr	r0, [ip, #896]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [fp, #-92]
	ldr	r3, [fp, #-88]
	cmp	r2, #512
	bhi	.L151
	cmp	r3, #512
	bhi	.L151
	ldr	r1, [r9]
	ldr	r0, [r1, #708]
	cmp	r0, #0
	bne	.L153
	ldr	r1, [r1, #28]
	cmp	r1, #25
	beq	.L226
.L153:
	ldr	r3, [fp, #-200]
	ldr	r0, [r3, #896]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L155
.L215:
	ldr	r2, [fp, #-92]
	ldr	r3, [fp, #-88]
.L154:
	ldr	r1, .L230+8
	mov	r0, #1
	bl	dprint_vfmw
	sub	r2, fp, #140
	mov	r1, r9
	mov	r0, #6
	bl	Divx3_ArrangeVHBMem
	cmp	r0, #1
	bne	.L227
.L142:
	ldr	r3, [fp, #-140]
	add	r0, r9, #41216
	mov	r2, #96
	sub	r1, fp, #140
	cmp	r3, #0
	add	r0, r0, #32
	ldrne	r3, [fp, #-200]
	moveq	r3, #23
	ldrne	r3, [r3, #300]
	mov	r6, r3
	ldr	r3, [r7, #52]
	blx	r3
	ldr	r3, [fp, #-140]
	mov	r0, r9
	cmp	r3, #0
	ldrne	r2, [fp, #-200]
	movne	r3, r6
	strne	r3, [r2, #300]
	bl	Divx3_DecodeDecide
	cmp	r0, #1
	mov	r7, r0
	beq	.L158
	ldr	r1, .L230+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r4, #0
	b	.L132
.L219:
	ldr	r1, .L230+16
	mov	r0, #1
	bl	dprint_vfmw
.L132:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L220:
	ldr	r6, [fp, #-200]
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #896]
	bl	SM_ReleaseStreamSeg
	ldr	r1, [r5, #96]
	ldr	r0, [r6, #896]
	bl	SM_ReleaseStreamSeg
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L217:
	ldr	r1, .L230+20
	mov	r0, #1
	mov	r4, r3
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L136:
	str	r4, [r9, #276]
	str	r4, [r9, #216]
	str	r4, [r9, #280]
	str	r4, [r9, #284]
	b	.L137
.L221:
	mov	r2, r4
	ldr	r1, .L230+24
	mov	r0, #1
	mov	r4, #0
	bl	dprint_vfmw
	b	.L132
.L222:
	ldr	r1, .L230+28
	mov	r0, #1
	mov	r4, ip
	bl	dprint_vfmw
	b	.L132
.L216:
	add	ip, r9, #40960
	ldr	r1, .L230+32
	mov	r0, #1
	mov	r4, r5
	ldr	r2, [ip, #408]
	bl	dprint_vfmw
	b	.L132
.L218:
	ldr	r1, .L230+36
	mov	r0, #1
	mov	r4, r3
	bl	dprint_vfmw
	b	.L132
.L151:
	ldr	r1, .L230+40
	mov	r0, #18
	bl	dprint_vfmw
	mov	r4, #0
	b	.L132
.L227:
	ldr	r1, .L230+44
	mov	r0, #1
	bl	dprint_vfmw
	mov	r4, #0
	b	.L132
.L224:
	ldr	r3, [fp, #-200]
	ldr	r3, [r3, #340]
	cmp	r1, r3
	bne	.L141
	b	.L142
.L225:
	ldr	r3, [fp, #-188]
	cmp	r3, #0
	bgt	.L147
	ldr	r3, [fp, #-192]
	cmp	r3, #32
	bgt	.L147
	ldr	r3, [r9, #4]
	cmp	r3, #0
	bne	.L147
	ldr	r3, [fp, #-200]
	ldr	ip, [r3, #404]
	cmp	ip, #0
	beq	.L144
	ldr	r3, [r3, #336]
	cmp	r3, #0
	beq	.L144
	ldr	r6, [fp, #-200]
	ldr	r3, [fp, #-88]
	ldr	r0, [r6, #900]
	cmp	r3, r0
	beq	.L144
	ldr	lr, [fp, #-92]
	ldr	r1, [r6, #904]
	cmp	lr, r1
	beq	.L144
	str	r3, [sp, #12]
	mov	r3, ip
	stmib	sp, {r0, lr}
	mov	r0, r2
	str	r1, [sp]
	ldr	r1, .L230+48
	bl	dprint_vfmw
	ldr	r3, .L230+4
	ldr	r2, [fp, #-88]
	ldr	ip, [r3]
	ldr	r3, [fp, #-92]
	cmp	ip, #0
	str	r2, [r6, #900]
	str	r3, [r6, #904]
	beq	.L144
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r6, #896]
	blx	ip
	b	.L144
.L226:
	ldrb	r1, [r9, #186]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L154
.L155:
	mov	r0, r6
	bl	ResetVoQueue
	b	.L215
.L223:
	ldr	r1, .L230+52
	mov	r4, r3
	bl	dprint_vfmw
	b	.L132
.L158:
	mov	r0, r9
	bl	Divx3_GetImageBuffer
	subs	r3, r0, #0
	beq	.L228
	ldr	r3, [fp, #-200]
	ldr	r1, [r3, #500]
	ldr	r0, [r3, #896]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L160
	ldr	r7, [r9]
	add	r2, r3, #608
	add	lr, r3, #624
	ldr	r1, .L230+56
	mov	r0, #29
	ldrd	r6, [r7, #56]
	strd	r6, [r2, #-8]
	ldr	r7, [r9]
	ldrd	r6, [r7, #72]
	strd	r6, [r2]
	ldr	r7, [r9]
	ldrd	r6, [r7, #88]
	strd	r6, [lr, #-8]
	mvn	r6, #0
	ldr	lr, [r9]
	mvn	r7, #0
	ldr	lr, [lr, #104]
	str	lr, [r3, #8]
	ldr	lr, [r9]
	ldr	lr, [lr, #108]
	str	lr, [r3, #12]
	ldr	lr, [r9]
	ldr	lr, [lr, #112]
	str	lr, [r3, #16]
	ldr	lr, [r9]
	ldr	lr, [lr, #128]
	str	lr, [r3, #20]
	ldr	r3, [r9]
	strd	r6, [r3, #56]
	ldrd	r6, [r2]
	ldrd	r2, [r2, #-8]
	strd	r6, [sp]
	bl	dprint_vfmw
	ldr	r6, [fp, #-200]
	ldr	r1, [r6, #500]
	ldr	r0, [r6, #896]
	bl	FSP_GetLogicFs
	ldr	r1, [r6, #496]
	mov	r7, r0
	ldr	r0, [r6, #896]
	bl	FSP_GetLogicFs
	cmp	r7, #0
	cmpne	r0, #0
	beq	.L161
	ldr	r3, [r7, #640]
	cmp	r3, #0
	beq	.L161
	ldr	r2, [r7, #648]
	cmp	r2, #0
	beq	.L161
	ldr	r2, [r7, #644]
	cmp	r2, #0
	beq	.L161
	ldr	r2, [r0, #640]
	mov	ip, #0
	str	ip, [fp, #-208]
	mov	r0, #13
	cmp	r2, #0
	ldrne	r1, [r2, #16]
	ldreq	r1, [r3, #16]
	ldr	r2, [r9, #168]
	str	r1, [r9, #132]
	ldr	r3, [r7, #640]
	mov	ip, r2
	ldr	r2, [r3, #16]
	str	r2, [r9, #136]
	ldr	lr, [r7, #652]
	str	lr, [r9, #164]
	ldr	r3, [r7, #644]
	ldr	r3, [r3, #16]
	stmia	sp, {r1, lr}
	ldr	r1, .L230+60
	str	r3, [r9, #140]
	str	ip, [sp, #8]
	bl	dprint_vfmw
	ldr	r3, [r7, #644]
	mov	r0, r8
	ldr	r3, [r3, #32]
	str	r3, [r9, #144]
	ldr	r3, [r7, #644]
	ldr	r3, [r3, #76]
	str	r3, [r9, #148]
	ldr	r3, [r7, #644]
	ldr	r3, [r3, #60]
	str	r3, [r9, #152]
	ldr	r3, [r7, #644]
	ldr	r3, [r3, #64]
	str	r3, [r9, #156]
	bl	BsResidBits
	ldr	r3, [r5, #8]
	ldr	r2, [fp, #-204]
	bic	r3, r3, #15
	str	r3, [r9, #104]
	ldr	r3, [r5, #8]
	and	r3, r3, #15
	add	r3, r2, r3, lsl #3
	rsb	r3, r0, r3
	mov	r0, r8
	and	r2, r3, #127
	str	r2, [r9, #116]
	mov	r3, r3, lsr #7
	mov	r3, r3, asl #4
	str	r3, [r9, #108]
	bl	BsResidBits
	ldr	ip, [fp, #-208]
	ldr	r3, [fp, #-200]
	str	ip, [r9, #112]
	str	ip, [r9, #120]
	str	ip, [r9, #128]
	str	r0, [r9, #124]
	ldr	r3, [r3, #300]
	cmp	r3, #24
	beq	.L167
	cmp	r3, #26
	beq	.L168
	cmp	r3, #23
	beq	.L229
	ldr	r1, .L230+64
	mov	r0, #1
	mov	r4, ip
	bl	dprint_vfmw
	b	.L132
.L228:
	ldr	r2, [fp, #-200]
	mov	r4, r3
	ldr	r1, .L230+68
	ldr	r2, [r2, #408]
	bl	dprint_vfmw
	b	.L132
.L147:
	mvn	r4, #1
	b	.L132
.L161:
	ldr	r3, [fp, #-200]
	mov	r2, #1
	mov	r4, #0
	ldr	r1, [r3, #500]
	ldr	r0, [r3, #896]
	bl	FSP_ClearLogicFs
	ldr	r1, .L230+72
	mov	r0, r4
	bl	dprint_vfmw
	b	.L132
.L160:
	mov	r0, r7
	ldr	r1, .L230+76
	mov	r4, r3
	bl	dprint_vfmw
	b	.L132
.L167:
	ldr	r3, [fp, #-200]
	str	ip, [fp, #-180]
	ldr	r2, [r3, #340]
	ldr	r3, [r3, #336]
	tst	r2, #1
	mov	r2, r2, lsr #1
	moveq	r5, #2
	mul	r3, r3, r2
	sub	r2, r3, #1
	str	r2, [fp, #-160]
	mov	r2, r3, asl #1
	str	r3, [fp, #-176]
	sub	r3, r2, #1
	str	r3, [fp, #-156]
	beq	.L170
	ldr	r3, [fp, #-200]
	mov	r5, #3
	str	r2, [fp, #-172]
	ldr	r3, [r3, #352]
	sub	r3, r3, #1
	str	r3, [fp, #-152]
.L170:
	ldr	r2, [fp, #-200]
	cmp	r5, #0
	ldr	r3, [r2, #288]
	str	r3, [r9, #12]
	ldr	r3, [r2, #336]
	str	r3, [r9, #16]
	ldr	r3, [r2, #340]
	str	r3, [r9, #20]
	ldr	r3, [r2, #292]
	str	r3, [r9, #24]
	ldr	r3, [r2, #296]
	str	r3, [r9, #28]
	ldr	r3, [r2, #304]
	str	r3, [r9, #32]
	ldr	r3, [r2, #312]
	str	r3, [r9, #36]
	ldr	r3, [r2, #316]
	str	r3, [r9, #40]
	ldr	r3, [r2, #320]
	str	r3, [r9, #44]
	ldr	r3, [r2, #324]
	str	r3, [r9, #48]
	ldr	r3, [r2, #328]
	str	r3, [r9, #52]
	ldr	r3, [r2, #332]
	str	r5, [r9, #60]
	str	r3, [r9, #56]
	beq	.L173
	sub	lr, fp, #180
	add	r0, r9, #60
	sub	r1, fp, #160
	add	r2, r9, #80
	mov	r3, #0
.L172:
	add	r3, r3, #1
	ldr	r6, [lr], #4
	ldr	ip, [r1], #4
	cmp	r3, r5
	str	r6, [r0, #4]!
	str	ip, [r2, #4]!
	bne	.L172
.L173:
	ldr	r3, [r9]
	str	r10, [r9, #4]
	ldr	r2, [r3, #592]
	str	r2, [r9, #172]
	ldr	r3, [r3, #632]
	str	r3, [r9, #176]
	b	.L132
.L229:
	ldr	r3, [fp, #-200]
	mov	r5, #1
	str	ip, [fp, #-180]
	ldr	r3, [r3, #352]
	sub	r3, r3, #1
	str	r3, [fp, #-160]
	b	.L170
.L168:
	ldr	r3, [fp, #-200]
	str	ip, [fp, #-180]
	ldr	r2, [r3, #340]
	ldr	r3, [r3, #336]
	tst	r2, #3
	mov	r2, r2, lsr #2
	moveq	r5, #4
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
	beq	.L170
	ldr	r3, [fp, #-200]
	mov	r5, #5
	str	r1, [fp, #-164]
	ldr	r3, [r3, #352]
	sub	r3, r3, #1
	str	r3, [fp, #-144]
	b	.L170
.L231:
	.align	2
.L230:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC26
	.word	.LC28
	.word	.LC20
	.word	.LC18
	.word	.LC21
	.word	.LC22
	.word	.LC17
	.word	.LC19
	.word	.LC25
	.word	.LC27
	.word	.LC24
	.word	.LC23
	.word	.LC30
	.word	.LC33
	.word	.LC34
	.word	.LC29
	.word	.LC32
	.word	.LC31
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
	add	r4, r0, #40960
	mov	r2, #0
	ldr	r0, [r4, #896]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #896]
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, [r0]
	mov	r4, r0
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L250
	add	r6, r0, #40960
	ldr	r1, [r6, #500]
	ldr	r0, [r6, #896]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L250
	ldr	r1, [r6, #500]
	ldr	r0, [r6, #896]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L249
.L250:
	add	r5, r4, #41472
	add	r5, r5, #32
.L240:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	GetVoLastImageID
.L249:
	mov	r0, r4
	add	r5, r4, #41472
	bl	Divx3_SetImgFormat
	add	r5, r5, #32
	mov	r3, #1
	str	r3, [r7, #188]
	mov	r2, r3
	ldr	r1, [r6, #500]
	ldr	r0, [r6, #896]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #896]
	mov	r2, r4
	str	r7, [sp]
	mov	r3, r5
	mov	r1, #5
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L237
	mov	r2, #0
	ldr	r1, [r6, #500]
	ldr	r0, [r6, #896]
	add	r10, r7, #560
	bl	FSP_SetDisplay
	add	r4, r7, #576
	ldr	r1, [r7, #560]
	mov	r8, #0
	ldr	r0, [r6, #896]
	mov	r9, #0
	bl	FreeUsdByDec
	strd	r8, [r10]
	ldr	r0, [r6, #896]
	ldr	r1, [r7, #568]
	bl	FreeUsdByDec
	strd	r8, [r4, #-8]
	b	.L240
.L237:
	ldr	r3, [r7, #160]
	cmp	r3, #100
	bhi	.L240
	ldr	r3, [r7, #112]
	cmp	r3, #31
	bls	.L240
	ldr	r3, [r7, #116]
	cmp	r3, #31
	bls	.L240
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	mov	r6, r1
	mov	r5, r0
	ldr	r1, .L281
	add	r4, r5, #40960
	mov	r7, r2
	mov	r0, #22
	mov	r2, r6
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, #5
	bl	ReleasePacket
	ldr	r1, [r4, #500]
	ldr	r0, [r4, #896]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L278
	ldr	r3, [r4, #288]
	cmp	r3, #0
	andeq	r7, r7, #1
	streq	r7, [r4, #384]
	cmp	r6, #0
	movlt	r6, #0
	blt	.L255
	cmp	r6, #100
	movge	r6, #100
.L255:
	str	r6, [r0, #160]
	mov	r0, #22
	ldr	r1, .L281+4
	bl	dprint_vfmw
	mov	r0, r5
	bl	Divx3_SetImgFormat
	ldrb	r3, [r5, #187]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L258
	ldr	r3, [r5]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L258
	ldr	r3, [r4, #412]
	cmp	r3, #0
	ldrne	r1, [r4, #496]
	mvneq	r1, #0
	str	r1, [r4, #504]
.L257:
	cmn	r1, #1
	beq	.L260
	ldr	r0, [r4, #896]
	bl	FSP_GetFsImagePtr
	ldr	r6, [r4, #504]
	mov	r1, r6
	mov	r7, r0
	ldr	r0, [r4, #896]
	bl	FSP_GetRef
	mov	r2, r6
	ldr	r1, .L281+8
	mov	r3, r0
	mov	r0, #12
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r4, #504]
	ldr	r0, [r4, #896]
	bl	FSP_SetDisplay
	add	r3, r5, #41472
	ldr	r0, [r4, #896]
	add	r3, r3, #32
	str	r7, [sp]
	mov	r2, r5
	mov	r1, #5
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L279
.L260:
	ldr	r2, [r4, #288]
	ldr	r3, [r4, #408]
	cmp	r2, #1
	add	r3, r3, #1
	str	r3, [r4, #408]
	beq	.L280
.L261:
	ldr	r2, [r4, #496]
	mov	r3, #1
	ldr	r1, [r4, #500]
	str	r3, [r4, #412]
	cmp	r2, r1
	beq	.L263
	ldr	r1, .L281+12
	mov	r0, #13
	bl	dprint_vfmw
	mov	r2, #0
	ldr	r1, [r4, #496]
	ldr	r0, [r4, #896]
	bl	FSP_SetRef
	ldr	r1, [r4, #496]
	ldr	r0, [r4, #896]
	bl	FSP_RemovePmv
	ldr	r1, [r4, #500]
.L263:
	str	r1, [r4, #496]
	mov	r2, #1
	ldr	r0, [r4, #896]
	bl	FSP_SetRef
	ldr	r2, [r4, #500]
	ldr	r1, .L281+16
	mov	r0, #13
	bl	dprint_vfmw
	mov	r0, #1
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L258:
	ldr	r1, [r4, #500]
	str	r1, [r4, #504]
	b	.L257
.L279:
	mov	r2, #0
	ldr	r1, [r4, #504]
	ldr	r0, [r4, #896]
	bl	FSP_SetDisplay
.L278:
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L280:
	ldr	r3, [r4, #412]
	cmp	r3, #0
	bne	.L261
	b	.L278
.L282:
	.align	2
.L281:
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
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
	add	r4, r0, #40960
	mov	r5, r0
	ldr	r0, [r4, #896]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #896]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L289
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L289:
	add	r1, r5, #41472
	ldr	r0, [r4, #896]
	add	r1, r1, #32
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	DIVX3DEC_GetImageBuffer, .-DIVX3DEC_GetImageBuffer
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__FUNCTION__.14249, %object
	.size	__FUNCTION__.14249, 19
__FUNCTION__.14249:
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
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC25:
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC26:
	ASCII(.ascii	"new seq size %d * %d\012\000" )
	.space	2
.LC27:
	ASCII(.ascii	"Divx3_ArrangeVHBMem err\012\000" )
	.space	3
.LC28:
	ASCII(.ascii	"Divx3_DecodeDecide err\012\000" )
.LC29:
	ASCII(.ascii	"Fatal Error: frm%d, get image buffer failed\012\000" )
	.space	3
.LC30:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC31:
	ASCII(.ascii	"FSP_GetFsImagePtr Error!\012\000" )
	.space	2
.LC32:
	ASCII(.ascii	"CurFsID abnormal!\012\000" )
	.space	1
.LC33:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,currpmv,colpmv = %p,%p,%p,%" )
	ASCII(.ascii	"p,%p,%p\012\000" )
.LC34:
	ASCII(.ascii	"vol_mode Error!\012\000" )
	.space	3
.LC35:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC36:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC37:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.space	2
.LC38:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC39:
	ASCII(.ascii	"++++++++++ ref %d\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
