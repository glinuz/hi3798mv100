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
	.file	"vp8.c"
	.text
	.align	2
	.type	VP8_init_frame.part.0, %function
VP8_init_frame.part.0:
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
	ldr	r6, .L2
	mov	r4, r0
	ldr	r8, .L2+4
	add	r0, r0, #2368
	mov	r2, #38
	add	r0, r0, #6
	ldr	r3, [r6, #52]
	mov	r5, #0
	mov	r1, r8
	ldr	r7, .L2+8
	blx	r3
	add	r0, r4, #1312
	add	r1, r8, #40
	ldr	r3, [r6, #52]
	mov	r2, #1056
	add	r0, r0, #6
	blx	r3
	add	r0, r4, #1296
	ldr	r3, [r6, #52]
	mov	r1, r7
	mov	r2, #4
	add	r0, r0, #14
	blx	r3
	add	r0, r4, #1312
	add	r1, r7, #4
	ldr	r3, [r6, #52]
	mov	r2, #3
	add	r0, r0, #2
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #8
	mov	r1, r5
	add	r0, r4, #22
	blx	r3
	strb	r5, [r4, #17]
	mov	r1, r5
	ldr	r3, [r6, #48]
	mov	r2, #4
	add	r0, r4, #32
	blx	r3
	ldr	r3, [r6, #48]
	mov	r1, r5
	add	r0, r4, #36
	mov	r2, #4
	blx	r3
	mov	r3, #1
	strb	r5, [r4, #43]
	strb	r5, [r4, #44]
	strb	r5, [r4, #48]
	strb	r5, [r4, #49]
	strb	r3, [r4, #41]
	strb	r3, [r4, #42]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3:
	.align	2
.L2:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	VP8_init_frame.part.0, .-VP8_init_frame.part.0
	.align	2
	.global	Vp8_Vfmw_StartBoolDecode
	.type	Vp8_Vfmw_StartBoolDecode, %function
Vp8_Vfmw_StartBoolDecode:
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
	mov	r4, r0
	mov	r2, #255
	mov	r0, r1
	mov	r3, #16
	str	r2, [r4]
	mov	r1, #24
	str	r3, [r4, #8]
	bl	BsGet
	str	r0, [r4, #4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	Vp8_Vfmw_StartBoolDecode, .-Vp8_Vfmw_StartBoolDecode
	.align	2
	.global	Vp8_Vfmw_DecodeBool
	.type	Vp8_Vfmw_DecodeBool, %function
Vp8_Vfmw_DecodeBool:
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
	mov	r8, r1
	ldmia	r0, {r1, r3}
	mov	r7, r0
	ldr	r6, [r0, #8]
	sub	r5, r1, #1
	mul	r2, r5, r2
	mov	r5, r2, lsr #8
	add	r5, r5, #1
	mov	r2, r5, asl #16
	cmp	r3, r2
	rsbcs	r3, r2, r3
	ldr	r2, .L13
	rsbcs	r5, r5, r1
	movcs	r9, #1
	add	r2, r2, r5
	movcc	r9, #0
	ldrb	r2, [r2, #1096]	@ zero_extendqisi2
	subs	r6, r6, r2
	mov	r5, r5, asl r2
	mov	r10, r3, asl r2
	bmi	.L12
.L7:
	mov	r0, r9
	str	r6, [r7, #8]
	stmia	r7, {r5, r10}
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L12:
	rsbs	r3, r6, #8
	str	r3, [fp, #-48]
	bmi	.L7
	mov	r4, r3
.L8:
	mov	r1, #8
	mov	r0, r8
	bl	BsGet
	orr	r10, r10, r0, asl r4
	subs	r4, r4, #8
	bpl	.L8
	ldr	r3, [fp, #-48]
	add	r6, r6, #8
	bic	r3, r3, #7
	add	r6, r6, r3
	b	.L7
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	Vp8_Vfmw_DecodeBool, .-Vp8_Vfmw_DecodeBool
	.align	2
	.global	Vp8_Vfmw_DecodeValue
	.type	Vp8_Vfmw_DecodeValue, %function
Vp8_Vfmw_DecodeValue:
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
	bmi	.L16
.L17:
	mov	r2, #128
	mov	r1, r7
	mov	r0, r6
	bl	Vp8_Vfmw_DecodeBool
	orr	r5, r5, r0, asl r4
	subs	r4, r4, #1
	bcs	.L17
.L16:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	Vp8_Vfmw_DecodeValue, .-Vp8_Vfmw_DecodeValue
	.align	2
	.global	VP8_SetImgFormat
	.type	VP8_SetImgFormat, %function
VP8_SetImgFormat:
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
	add	r5, r0, #4096
	add	r3, r0, #8192
	mov	r6, r0
	ldr	r1, [r5, #4056]
	ldr	r0, [r3, #328]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r5, #4064]
	mov	r2, #0
	mov	r1, r2
	strb	r2, [r4, #372]
	and	r3, r3, #3
	mov	ip, #13
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
	ldr	r2, [r5, #1668]
	str	r2, [r4, #164]
	ldr	r3, [r5, #1672]
	str	r3, [r4, #168]
	ldr	r1, [r6]
	ldr	r1, [r1, #28]
	cmp	r1, #25
	beq	.L26
.L22:
	str	r2, [r4, #172]
	mov	r2, r2, lsr #1
	str	r3, [r4, #176]
	mov	r3, r3, lsr #1
	str	r2, [r4, #180]
	str	r3, [r4, #184]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L26:
	ldr	r2, [r4, #192]
	add	r3, r3, #15
	ldr	r0, [r4, #104]
	mov	r3, r3, lsr #4
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
	b	.L22
	UNWIND(.fnend)
	.size	VP8_SetImgFormat, .-VP8_SetImgFormat
	.align	2
	.global	VP8DEC_VDMPostProc
	.type	VP8DEC_VDMPostProc, %function
VP8DEC_VDMPostProc:
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
	mov	r5, r0
	add	r4, r0, #4096
	add	r6, r0, #8192
	mov	r7, r1
	mov	r0, #13
	mov	r1, r5
	bl	ReleasePacket
	ldr	r1, [r4, #4056]
	ldr	r0, [r6, #328]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L95
	cmp	r7, #0
	blt	.L30
	cmp	r7, #100
	movlt	r2, r7
	movge	r2, #100
	str	r2, [r3, #200]
	ldr	r3, [r5]
	ldr	r3, [r3]
	cmp	r2, r3
	bhi	.L96
.L31:
	mov	r0, r5
	bl	VP8_SetImgFormat
	ldr	r3, [r4, #4020]
	cmp	r3, #2
	beq	.L97
.L32:
	ldr	r3, [r5, #3268]
	cmp	r3, #0
	bne	.L34
.L33:
	mvn	r3, #0
	ldr	r1, [r4, #4056]
	str	r3, [r4, #4060]
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetDisplay
	ldr	r1, [r4, #4060]
.L35:
	cmn	r1, #1
	beq	.L36
	ldr	r0, [r6, #328]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L98
	mov	r2, #1
	ldr	r1, [r4, #4060]
	ldr	r0, [r6, #328]
	bl	FSP_SetDisplay
	add	r3, r5, #8128
	ldr	r0, [r6, #328]
	add	r3, r3, #40
	str	r8, [sp]
	mov	r2, r5
	mov	r1, #13
	bl	InsertImgToVoQueue
	cmp	r0, #1
	mov	r7, r0
	bne	.L99
.L36:
	ldr	r2, [r4, #4064]
	ldr	r3, [r4, #4024]
	cmp	r2, #0
	ldr	r8, [r4, #4052]
	add	r3, r3, #1
	str	r3, [r4, #4024]
	bne	.L100
	ldr	r3, [r4, #4044]
	cmp	r8, r3
	beq	.L42
.L40:
	ldr	r3, [r4, #4048]
	cmp	r8, r3
	beq	.L42
	ldr	r1, [r4, #4056]
	subs	r7, r8, r1
	movne	r7, #1
.L41:
	str	r1, [r4, #4052]
	mov	r2, #1
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
.L39:
	ldrb	r3, [r5, #3252]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L101
	cmp	r3, #2
	beq	.L102
.L45:
	ldrb	r3, [r5, #3251]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L103
	cmp	r3, #2
	beq	.L104
.L50:
	ldr	r3, [r4, #4064]
	cmp	r3, #0
	beq	.L53
	ldrb	r3, [r5, #3249]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L54
.L53:
	ldr	r3, [r4, #4044]
	ldr	r2, [r4, #4052]
	cmp	r3, r2
	beq	.L93
	ldr	r2, [r4, #4048]
	cmp	r3, r2
	beq	.L93
	ldr	r1, [r4, #4056]
	cmp	r3, r1
	beq	.L56
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
.L93:
	ldr	r1, [r4, #4056]
.L56:
	str	r1, [r4, #4044]
	mov	r2, #1
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
.L54:
	ldr	r3, [r5, #3264]
	cmp	r3, #0
	beq	.L58
	ldrb	r3, [r5, #3250]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L59
.L58:
	ldr	r3, [r4, #4048]
	ldr	r2, [r4, #4052]
	cmp	r3, r2
	beq	.L94
	ldr	r2, [r4, #4044]
	cmp	r3, r2
	beq	.L94
	ldr	r1, [r4, #4056]
	cmp	r3, r1
	beq	.L61
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
.L94:
	ldr	r1, [r4, #4056]
.L61:
	str	r1, [r4, #4048]
	mov	r2, #1
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
.L59:
	cmp	r7, #1
	movne	r0, #1
	beq	.L105
.L92:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L34:
	ldr	r1, [r4, #4056]
	str	r1, [r4, #4060]
	b	.L35
.L100:
	ldrb	r7, [r5, #3248]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L39
	ldr	r3, [r4, #4044]
	cmp	r8, r3
	bne	.L40
.L42:
	ldr	r1, [r4, #4056]
	mov	r7, #0
	b	.L41
.L30:
	mov	r2, #0
	str	r2, [r3, #200]
	b	.L31
.L97:
	ldr	r3, [r4, #4064]
	cmp	r3, #0
	bne	.L33
	b	.L32
.L103:
	ldr	r1, [r4, #4044]
	ldr	r3, [r4, #4052]
	cmp	r1, r3
	beq	.L49
	ldr	r3, [r4, #4048]
	cmp	r1, r3
	cmpne	r8, r1
	beq	.L49
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
.L49:
	str	r8, [r4, #4044]
	mov	r2, #1
	ldr	r0, [r6, #328]
	mov	r1, r8
	bl	FSP_SetRef
	mov	r7, #0
	b	.L50
.L101:
	ldr	r1, [r4, #4048]
	ldr	r3, [r4, #4052]
	cmp	r1, r3
	beq	.L44
	ldr	r3, [r4, #4044]
	cmp	r1, r3
	cmpne	r8, r1
	beq	.L44
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
.L44:
	str	r8, [r4, #4048]
	mov	r2, #1
	ldr	r0, [r6, #328]
	mov	r1, r8
	bl	FSP_SetRef
	mov	r7, #0
	b	.L45
.L99:
	ldr	r1, [r8, #212]
	mov	r5, #0
	ldr	r0, [r6, #328]
	bl	FreeUsdByDec
	str	r5, [r8, #212]
	ldr	r1, [r8, #216]
	ldr	r0, [r6, #328]
	bl	FreeUsdByDec
	str	r5, [r8, #216]
	mov	r2, r5
	ldr	r1, [r4, #4060]
	ldr	r0, [r6, #328]
	bl	FSP_SetDisplay
	ldr	r3, .L106
	mov	r0, r5
	ldr	r1, .L106+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r7
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L104:
	ldr	r3, [r4, #4044]
	ldr	r2, [r4, #4052]
	ldr	r1, [r4, #4048]
	cmp	r3, r2
	beq	.L52
	cmp	r3, r1
	beq	.L52
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
	ldr	r1, [r4, #4048]
.L52:
	str	r1, [r4, #4044]
	mov	r2, #1
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
	b	.L50
.L102:
	ldr	r3, [r4, #4048]
	ldr	r2, [r4, #4052]
	ldr	r1, [r4, #4044]
	cmp	r3, r2
	beq	.L47
	cmp	r3, r1
	beq	.L47
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
	ldr	r1, [r4, #4044]
.L47:
	str	r1, [r4, #4048]
	mov	r2, #1
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
	b	.L45
.L105:
	ldr	r0, [r6, #328]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetRef
	mov	r0, r7
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L96:
	ldr	ip, .L106
	mov	r0, #1
	ldr	r1, .L106+8
	mov	r5, #0
	ldr	ip, [ip, #68]
	blx	ip
	mov	r2, #1
	ldr	r1, [r4, #4044]
	ldr	r0, [r6, #328]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #4048]
	ldr	r0, [r6, #328]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #4052]
	ldr	r0, [r6, #328]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #4044]
	str	r5, [r4, #4048]
	str	r5, [r4, #4052]
	ldr	r0, [r6, #328]
	bl	FSP_GetNullLogicFs
	str	r5, [r4, #4024]
	mov	r3, r0
	mvn	r0, #0
	str	r3, [r4, #4052]
	str	r3, [r4, #4048]
	str	r3, [r4, #4044]
	b	.L92
.L98:
	ldr	r3, .L106
	movw	r2, #641
	ldr	r1, .L106+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L92
.L95:
	ldr	r3, .L106
	ldr	r1, .L106+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L92
.L107:
	.align	2
.L106:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC4
	.word	.LC2
	.word	.LC3
	.word	.LC1
	UNWIND(.fnend)
	.size	VP8DEC_VDMPostProc, .-VP8DEC_VDMPostProc
	.align	2
	.global	VP8DEC_Init
	.type	VP8DEC_Init, %function
VP8DEC_Init:
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
	ldr	r6, .L112
	add	r5, r0, #8192
	movw	r2, #8528
	mov	r7, r1
	ldr	r8, [r5, #320]
	mov	r1, #0
	ldr	r3, [r6, #48]
	mov	r4, r0
	blx	r3
	add	r0, r4, #8128
	add	r0, r0, #40
	str	r8, [r5, #320]
	bl	ResetVoQueue
	str	r7, [r4]
	mov	r0, r4
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r5, #328]
	beq	.L111
	add	r3, r4, #4096
	mov	ip, #0
	mov	r2, #1024
	mov	r1, #3
	str	r2, [r3, #4016]
	mov	r0, #1
	str	ip, [r3, #4028]
	ldr	r2, [r4]
	ldr	lr, [r2, #8]
	str	ip, [r3, #4036]
	str	r1, [r3, #4068]
	str	lr, [r3, #4020]
	str	r1, [r3, #4032]
	ldr	r2, [r2, #40]
	str	r2, [r3, #4040]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L111:
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r1, .L112+4
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L113:
	.align	2
.L112:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC5
	UNWIND(.fnend)
	.size	VP8DEC_Init, .-VP8DEC_Init
	.align	2
	.global	VP8DEC_Destroy
	.type	VP8DEC_Destroy, %function
VP8DEC_Destroy:
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
	.size	VP8DEC_Destroy, .-VP8DEC_Destroy
	.align	2
	.global	VP8_setup_version
	.type	VP8_setup_version, %function
VP8_setup_version:
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
	ldr	r3, [r0, #64]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L116
.L118:
	.word	.L116
	.word	.L119
	.word	.L120
	.word	.L121
.L116:
	mov	r3, #0
	str	r3, [r0, #76]
	str	r3, [r0, #80]
	str	r3, [r0, #84]
	str	r3, [r0, #88]
	mov	r3, #0
	str	r3, [r0, #92]
	ldmfd	sp, {fp, sp, pc}
.L120:
	mov	r2, #1
	mov	r3, #0
	str	r2, [r0, #76]
	str	r2, [r0, #84]
	str	r3, [r0, #80]
	str	r3, [r0, #88]
.L123:
	mov	r3, #1
	str	r3, [r0, #92]
	ldmfd	sp, {fp, sp, pc}
.L119:
	mov	r2, #0
	mov	r3, #1
	str	r2, [r0, #76]
	str	r2, [r0, #88]
	str	r3, [r0, #80]
	str	r3, [r0, #84]
	b	.L123
.L121:
	mov	r3, #1
	str	r3, [r0, #76]
	str	r3, [r0, #80]
	str	r3, [r0, #84]
	str	r3, [r0, #88]
	b	.L123
	UNWIND(.fnend)
	.size	VP8_setup_version, .-VP8_setup_version
	.align	2
	.global	read_mvcontexts
	.type	read_mvcontexts, %function
read_mvcontexts:
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
	ldr	r9, .L137
	mov	r6, r0
	mov	r5, r1
	mov	r8, #0
	str	r2, [fp, #-48]
.L129:
	ldr	r3, [fp, #-48]
	add	r10, r9, r8
	add	r4, r3, r8
	add	r8, r8, #19
	add	r7, r9, r8
	b	.L128
.L126:
	cmp	r10, r7
	add	r4, r4, #1
	beq	.L136
.L128:
	ldrb	r2, [r10], #1	@ zero_extendqisi2
	mov	r1, r5
	mov	r0, r6
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L126
	mov	r2, #7
	mov	r1, r5
	mov	r0, r6
	add	r4, r4, #1
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #1
	uxtb	r0, r0
	cmp	r0, #0
	mov	r0, r0, asl r2
	uxtbne	r2, r0
	cmp	r10, r7
	strb	r2, [r4, #-1]
	bne	.L128
.L136:
	cmp	r8, #38
	bne	.L129
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L138:
	.align	2
.L137:
	.word	.LANCHOR0+1352
	UNWIND(.fnend)
	.size	read_mvcontexts, .-read_mvcontexts
	.align	2
	.global	VP8_mb_mode_mv_init
	.type	VP8_mb_mode_mv_init, %function
VP8_mb_mode_mv_init:
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
	ldr	r3, [r0, #56]
	mov	r6, r0
	mov	r4, r1
	mov	r5, r2
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r2, #8
	mov	r1, r5
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #8
	mov	r1, r5
	strb	r0, [r6, #50]
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #8
	mov	r1, r5
	strb	r0, [r6, #51]
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #128
	mov	r1, r5
	strb	r0, [r6, #52]
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #1
	beq	.L141
.L145:
	mov	r2, #128
	mov	r1, r5
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L143
	add	r7, r6, #1312
	mov	r8, r7
	add	r7, r7, #1
	add	r8, r8, #4
.L146:
	mov	r2, #8
	mov	r1, r5
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeValue
	strb	r0, [r7, #1]!
	cmp	r7, r8
	bne	.L146
.L143:
	add	r2, r6, #2368
	mov	r1, r5
	add	r2, r2, #6
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	read_mvcontexts
.L141:
	add	r7, r6, #1296
	add	r8, r6, #1312
	add	r7, r7, #13
	add	r8, r8, #1
.L144:
	mov	r2, #8
	mov	r1, r5
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeValue
	strb	r0, [r7, #1]!
	cmp	r7, r8
	bne	.L144
	b	.L145
	UNWIND(.fnend)
	.size	VP8_mb_mode_mv_init, .-VP8_mb_mode_mv_init
	.align	2
	.global	get_delta_q
	.type	get_delta_q, %function
get_delta_q:
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
	mov	r5, r2
	mov	r2, #128
	mov	r6, r3
	mov	r7, r0
	mov	r8, r1
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #0
	bne	.L153
.L150:
	cmp	r0, r5
	movne	r3, #1
	strne	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L153:
	mov	r2, #4
	mov	r1, r8
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	mov	r1, r8
	mov	r2, #128
	mov	r4, r0
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r3, r0, #0
	eor	r4, r4, r3
	add	r0, r4, r0
	b	.L150
	UNWIND(.fnend)
	.size	get_delta_q, .-get_delta_q
	.align	2
	.global	VP8_init_frame
	.type	VP8_init_frame, %function
VP8_init_frame:
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
	ldr	r3, [r0, #56]
	cmp	r3, #0
	ldmnefd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8_init_frame.part.0
	UNWIND(.fnend)
	.size	VP8_init_frame, .-VP8_init_frame
	.align	2
	.global	VP8_GetPictureHeader
	.type	VP8_GetPictureHeader, %function
VP8_GetPictureHeader:
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
	ldr	r3, .L247
	mov	r9, r1
	mov	r6, r0
	str	r1, [fp, #-68]
	sub	r5, fp, #52
	ldmia	r3, {r0, r1}
	mov	r4, r2
	stmia	r5, {r0, r1}
	mov	r1, #8
	mov	r0, r6
	bl	BsGet
	mov	r1, #8
	mov	r7, r0
	mov	r0, r6
	bl	BsGet
	mov	r1, #8
	mov	r8, r0
	mov	r0, r6
	bl	BsGet
	add	lr, r9, #4096
	uxtb	r3, r7
	uxtb	r2, r8
	ubfx	r1, r3, #4, #1
	and	r7, r7, #1
	str	r7, [r9, #2516]
	ubfx	ip, r3, #1, #3
	str	r7, [lr, #864]
	str	r1, [r9, #2520]
	str	ip, [r9, #2524]
	uxtb	r0, r0
	mov	r1, r0, asl #16
	orr	r1, r1, r2, asl #8
	orr	r3, r1, r3
	mov	r3, r3, asr #5
	str	r3, [r9, #2528]
	cmp	r3, r4
	bhi	.L237
	ldr	r4, [fp, #-68]
	add	r3, r4, #2448
	add	r3, r3, #12
	str	r3, [fp, #-80]
	mov	r0, r3
	bl	VP8_setup_version
	ldr	r7, [r4, #2516]
	cmp	r7, #0
	beq	.L238
.L160:
	ldr	r4, [fp, #-68]
	mov	r3, #16
	mov	r2, #255
	mov	r1, #24
	mov	r0, r6
	str	r3, [r4, #2608]
	str	r2, [r4, #2600]
	bl	BsGet
	ldr	r3, [r4, #2516]
	ldr	r1, [fp, #-80]
	cmp	r3, #0
	add	r7, r1, #140
	str	r0, [r4, #2604]
	bne	.L165
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r4, [fp, #-68]
	mov	r2, #128
	mov	r1, r6
	str	r0, [r4, #2588]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	str	r0, [r4, #2592]
.L165:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2474]
	cmp	r0, #0
	bne	.L166
.L173:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r4, [fp, #-68]
	mov	r2, #6
	mov	r1, r6
	str	r0, [r4, #2596]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #3
	mov	r1, r6
	strb	r0, [r4, #2470]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	mov	r3, #0
	mov	r2, #128
	strb	r3, [r4, #2461]
	mov	r1, r6
	strb	r0, [r4, #2471]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	uxtb	r0, r0
	strb	r0, [r4, #2462]
	cmp	r0, #0
	bne	.L239
.L168:
	mov	r2, #2
	mov	r1, r6
	mov	r0, r7
	sub	r4, fp, #44
	bl	Vp8_Vfmw_DecodeValue
	ldr	r8, [fp, #-68]
	mov	r2, #7
	mov	r1, r6
	mov	r5, #0
	strb	r0, [r8, #2473]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	movw	r2, #2465
	str	r5, [r4, #-12]!
	mov	r1, r6
	ldrsb	r2, [r8, r2]
	mov	r3, r4
	strb	r0, [r8, #2463]
	mov	r0, r7
	bl	get_delta_q
	movw	r2, #2466
	ldrsb	r2, [r8, r2]
	mov	r3, r4
	mov	r1, r6
	strb	r0, [r8, #2465]
	mov	r0, r7
	bl	get_delta_q
	movw	r2, #2467
	ldrsb	r2, [r8, r2]
	mov	r3, r4
	mov	r1, r6
	strb	r0, [r8, #2466]
	mov	r0, r7
	bl	get_delta_q
	movw	r2, #2468
	ldrsb	r2, [r8, r2]
	mov	r3, r4
	mov	r1, r6
	strb	r0, [r8, #2467]
	mov	r0, r7
	bl	get_delta_q
	movw	r2, #2469
	mov	r3, r4
	ldrsb	r2, [r8, r2]
	mov	r1, r6
	strb	r0, [r8, #2468]
	mov	r0, r7
	bl	get_delta_q
	ldr	r3, [r8, #2516]
	cmp	r3, r5
	strb	r0, [r8, #2469]
	bne	.L240
.L177:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2505]
	cmp	r0, #0
	bne	.L188
	ldr	r0, [fp, #-80]
	movw	r2, #1149
	ldr	r3, .L247+4
	add	r1, r0, #1296
	add	r0, r0, #152
	add	r1, r1, #5
	ldr	r3, [r3, #52]
	blx	r3
.L188:
	ldr	r2, [fp, #-68]
	ldr	r3, [r2, #2516]
	cmp	r3, #0
	bne	.L189
	mov	r3, #1
	strb	r3, [r2, #2500]
.L190:
	mov	r3, #0
	str	r3, [fp, #-76]
.L201:
	ldr	r2, [fp, #-76]
	add	r10, r2, #3776
	add	r3, r2, #304
	add	r9, r2, #43
	ldr	r2, [fp, #-68]
	add	r10, r10, #2
	add	r3, r3, #3
	add	r2, r2, r10
	str	r2, [fp, #-64]
	ldr	r2, .L247+8
	add	r9, r2, r9
	add	r3, r2, r3
	str	r3, [fp, #-72]
.L197:
	ldr	r8, [fp, #-64]
	sub	r5, r9, #33
.L195:
	sub	r10, r5, #11
	mov	r4, r8
	b	.L192
.L191:
	cmp	r10, r5
	add	r4, r4, #1
	beq	.L241
.L192:
	ldrb	r2, [r10, #1]!	@ zero_extendqisi2
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #1
	bne	.L191
	mov	r2, #8
	mov	r1, r6
	mov	r0, r7
	add	r4, r4, #1
	bl	Vp8_Vfmw_DecodeValue
	cmp	r10, r5
	strb	r0, [r4, #-1]
	bne	.L192
.L241:
	add	r5, r10, #11
	add	r8, r8, #11
	cmp	r5, r9
	bne	.L195
	ldr	r3, [fp, #-64]
	add	r9, r10, #44
	add	r3, r3, #33
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-72]
	cmp	r9, r3
	bne	.L197
	ldr	r3, [fp, #-76]
	add	r3, r3, #264
	str	r3, [fp, #-76]
	cmp	r3, #1056
	bne	.L201
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	cmp	r0, #0
	str	r0, [r3, #2532]
	ldreq	r3, [fp, #-68]
	streqb	r0, [r3, #2513]
	beq	.L199
	mov	r2, #8
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	ldr	r3, [fp, #-68]
	strb	r0, [r3, #2513]
.L199:
	mov	r2, r6
	mov	r1, r7
	ldr	r0, [fp, #-80]
	bl	VP8_mb_mode_mv_init
	ldr	r3, [fp, #-68]
	ldr	r3, [r3, #2516]
	cmp	r3, #0
	beq	.L200
	ldr	r3, [fp, #-68]
	ldr	r3, [r3, #2500]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #0
	moveq	r0, #1
	beq	.L233
.L200:
	ldr	r2, [fp, #-68]
	mov	r0, #1
	ldrb	r3, [r2, #2463]	@ zero_extendqisi2
	strb	r3, [r2, #2464]
.L233:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L238:
	mov	r1, #8
	mov	r0, r6
	bl	BsGet
	mov	r1, #8
	mov	r4, r0
	mov	r0, r6
	bl	BsGet
	mov	r1, #8
	mov	r8, r0
	mov	r0, r6
	bl	BsGet
	uxtb	r2, r4
	uxtb	r3, r8
	cmp	r2, #157
	cmpeq	r3, #1
	movne	r4, #1
	moveq	r4, #0
	uxtb	r0, r0
	cmp	r0, #42
	orrne	r4, r4, #1
	cmp	r4, #0
	beq	.L161
	ldr	ip, .L247+4
	str	r0, [sp]
	mov	r0, #1
	ldr	r1, .L247+12
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L237:
	ldr	r3, .L247+4
	mov	r0, #1
	ldr	r1, .L247+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L161:
	mov	r1, #8
	mov	r0, r6
	bl	BsGet
	mov	r1, #8
	mov	r8, r0
	mov	r0, r6
	bl	BsGet
	mov	r1, #8
	mov	r7, r0
	mov	r0, r6
	bl	BsGet
	mov	r1, #8
	mov	r9, r0
	mov	r0, r6
	bl	BsGet
	ldr	lr, [fp, #-68]
	uxtb	r3, r7
	uxtb	ip, r9
	uxtb	r2, r8
	orr	r2, r2, r3, asl #8
	mov	r3, r3, lsr #6
	str	r3, [lr, #2572]
	ubfx	r2, r2, #0, #14
	add	r1, r2, #15
	str	r2, [lr, #2564]
	mov	r1, r1, lsr #4
	str	r1, [lr, #2556]
	uxtb	r0, r0
	orr	r3, ip, r0, asl #8
	mov	r0, r0, lsr #6
	ubfx	r3, r3, #0, #14
	str	r0, [lr, #2576]
	add	ip, r3, #15
	cmp	r3, #32
	cmphi	r2, #32
	str	r3, [lr, #2568]
	mov	ip, ip, lsr #4
	movls	r0, #1
	movhi	r0, #0
	cmp	r1, #512
	orrhi	r0, r0, #1
	mul	r1, r1, ip
	cmp	ip, #512
	orrhi	r0, r0, #1
	cmp	r0, #0
	str	ip, [lr, #2560]
	str	r1, [lr, #2580]
	bne	.L242
	ldr	r9, [fp, #-68]
	ldr	r8, [r9, #116]
	ldr	r4, [r9, #112]
	cmp	r2, r4
	cmpeq	r3, r8
	beq	.L163
	ldr	r7, .L247+4
	mov	r0, #22
	ldr	r1, .L247+20
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r0, [r9, #2560]
	ldr	r1, [r9, #2556]
	mov	r3, r8
	ldr	ip, [r7, #68]
	mov	r2, r4
	str	r0, [sp, #4]
	mov	r0, #22
	str	r1, [sp]
	ldr	r1, .L247+24
	blx	ip
.L163:
	ldr	r3, [fp, #-68]
	ldr	r3, [r3, #2516]
	cmp	r3, #0
	bne	.L160
	ldr	r0, [fp, #-80]
	bl	VP8_init_frame.part.0
	b	.L160
.L189:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	strb	r0, [r3, #2500]
	b	.L190
.L240:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r4, [fp, #-68]
	mov	r2, #128
	mov	r1, r6
	strb	r0, [r4, #2501]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldrb	r3, [r4, #2501]	@ zero_extendqisi2
	strb	r5, [r4, #2503]
	cmp	r3, #0
	uxtb	r3, r0
	strb	r3, [r4, #2502]
	bne	.L186
	mov	r2, #2
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	ldr	r2, [fp, #-68]
	ldrb	r3, [r2, #2502]	@ zero_extendqisi2
	strb	r0, [r2, #2503]
.L186:
	ldr	r2, [fp, #-68]
	cmp	r3, #0
	mov	r3, #0
	strb	r3, [r2, #2504]
	bne	.L187
	mov	r2, #2
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	ldr	r3, [fp, #-68]
	strb	r0, [r3, #2504]
.L187:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r4, [fp, #-68]
	mov	r2, #128
	mov	r1, r6
	strb	r0, [r4, #2508]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	strb	r0, [r4, #2509]
	b	.L177
.L239:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2461]
	cmp	r0, #0
	beq	.L168
	add	r4, r3, #2480
	add	r5, r3, #2496
	add	r4, r4, #12
	mov	r8, r3
.L181:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	uxtb	r0, r0
	strb	r0, [r8, #2490]
	cmp	r0, #1
	beq	.L243
.L179:
	add	r4, r4, #1
	cmp	r4, r5
	bne	.L181
	ldr	r3, [fp, #-68]
	add	r4, r3, #2496
	add	r4, r4, #4
.L185:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2491]
	cmp	r0, #1
	beq	.L244
.L183:
	add	r5, r5, #1
	cmp	r5, r4
	bne	.L185
	b	.L168
.L166:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r4, [fp, #-68]
	mov	r2, #128
	mov	r1, r6
	strb	r0, [r4, #2475]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	uxtb	r0, r0
	strb	r0, [r4, #2476]
	cmp	r0, #0
	bne	.L245
.L169:
	ldr	r3, [fp, #-68]
	ldrb	r3, [r3, #2475]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L173
	ldr	r3, .L247+4
	mov	r2, #3
	ldr	ip, [fp, #-68]
	mov	r1, #255
	ldr	r0, [fp, #-80]
	add	r4, ip, #2464
	add	r5, ip, #2480
	add	r0, r0, #18
	ldr	r3, [r3, #48]
	add	r4, r4, #14
	blx	r3
	add	r5, r5, #1
	b	.L175
.L174:
	add	r4, r4, #1
	cmp	r4, r5
	beq	.L173
.L175:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2481]
	cmp	r0, #1
	bne	.L174
	mov	r2, #8
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	strb	r0, [r4]
	b	.L174
.L243:
	mov	r2, #6
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #128
	mov	r1, r6
	strb	r0, [r4]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	uxtb	r0, r0
	strb	r0, [r8, #2472]
	cmp	r0, #1
	ldreqb	r3, [r4]	@ zero_extendqisi2
	rsbeq	r3, r3, #0
	streqb	r3, [r4]
	b	.L179
.L244:
	mov	r2, #6
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #128
	mov	r1, r6
	strb	r0, [r5]
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #0
	ldrneb	r3, [r5]	@ zero_extendqisi2
	rsbne	r3, r3, #0
	strneb	r3, [r5]
	b	.L183
.L245:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	ldr	r9, [fp, #-68]
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, .L247+4
	add	r4, r9, #2480
	mov	r1, #0
	mov	r2, #8
	add	r4, r4, #5
	ldr	r3, [r3, #48]
	mov	r8, r1
	strb	r0, [r9, #2477]
	ldr	r0, [fp, #-80]
	add	r0, r0, #22
	blx	r3
.L170:
	sub	r10, r4, #4
	str	r4, [fp, #-64]
	mov	r4, r10
	b	.L172
.L171:
	ldr	r3, [fp, #-64]
	strb	r10, [r4, #1]!
	cmp	r4, r3
	beq	.L246
.L172:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	mov	r10, #0
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2460]
	cmp	r0, #1
	bne	.L171
	ldr	r2, [r5, r8, asl #2]
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	mov	r2, #128
	mov	r1, r6
	mov	r9, r0
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	uxtb	r10, r9
	rsb	r9, r9, #0
	cmp	r0, #0
	uxtbne	r10, r9
	b	.L171
.L246:
	add	r8, r8, #1
	add	r4, r3, #4
	cmp	r8, #2
	bne	.L170
	b	.L169
.L242:
	ldr	ip, .L247+4
	mov	r0, #1
	ldr	r1, .L247+28
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r4
	b	.L233
.L248:
	.align	2
.L247:
	.word	.LANCHOR0+1392
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+1400
	.word	.LC7
	.word	.LC6
	.word	.LC9
	.word	.LC10
	.word	.LC8
	UNWIND(.fnend)
	.size	VP8_GetPictureHeader, .-VP8_GetPictureHeader
	.align	2
	.global	VP8_ArrangeVHBMem
	.type	VP8_ArrangeVHBMem, %function
VP8_ArrangeVHBMem:
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
	add	r6, r0, #4096
	mov	r5, r0
	ldr	r3, [r6, #1672]
	ldr	r2, [r6, #1668]
	cmp	r3, #31
	bls	.L251
	cmp	r2, #31
	bls	.L251
	ldr	r1, [r0]
	ldr	r0, [r1, #776]
	cmp	r0, #0
	ldr	r0, [r1, #28]
	bne	.L254
	cmp	r0, #25
	beq	.L277
.L255:
	mov	r7, r2
	mov	r4, r3
	mov	r9, #5
	mov	r10, #4
	str	r10, [fp, #-88]
.L260:
	ldr	r8, .L279
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r2, [r5]
	str	r4, [fp, #-48]
	mov	r3, #0
	str	r7, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r4, [fp, #-64]
	cmp	r2, #24
	str	r7, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L266
	ldrb	r3, [fp, #-88]	@ zero_extendqisi2
	uxtb	ip, r10
	strb	r9, [fp, #-70]
	strb	r3, [fp, #-72]
.L269:
	add	r7, r5, #8192
	sub	r1, fp, #76
	strb	ip, [fp, #-69]
	mov	r4, #0
	ldr	r0, [r7, #328]
	strb	r4, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r9, r0, #0
	bne	.L267
	ldr	r1, [r5]
	sub	r3, fp, #80
	ldr	r0, [r7, #328]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_PartitionFsMemory
	cmp	r0, r4
	moveq	r0, #1
	streq	r0, [r6, #4028]
	beq	.L273
	ldr	r3, [r8, #68]
	mov	r0, r9
	ldr	r1, .L279+4
	blx	r3
	mov	r0, r9
	b	.L273
.L251:
	ldr	ip, .L279
	mov	r0, #0
	ldr	r1, .L279+8
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
.L273:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L254:
	cmp	r0, #25
	bne	.L255
	ldr	r0, [r1, #736]
	ldr	ip, [r1, #740]
	cmp	r2, r0
	bhi	.L263
	cmp	r3, ip
	bhi	.L263
	ldr	r0, [r1, #764]
	mov	r4, r3
	ldr	ip, [r1, #760]
	mov	r7, r2
	cmp	r0, #0
	add	r3, ip, #2
	str	r3, [fp, #-88]
	bne	.L278
.L271:
	mov	r10, #1
	b	.L265
.L267:
	ldr	r3, [r8, #68]
	mov	r0, r4
	ldr	r1, .L279+12
	blx	r3
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L277:
	ldr	r7, [r1, #736]
	ldr	r4, [r1, #740]
	cmp	r2, r7
	bhi	.L257
	cmp	r3, r4
	bhi	.L257
	ldr	r0, [r6, #4028]
	cmp	r0, #1
	beq	.L273
	ldr	r2, [r1, #764]
	ldr	r3, [r1, #760]
	cmp	r2, #0
	add	r2, r3, #2
	str	r2, [fp, #-88]
	beq	.L271
	add	ip, r3, #1
	cmp	ip, #17
	movlt	r10, ip
	movge	r10, #17
.L265:
	ldrb	r9, [r1, #784]	@ zero_extendqisi2
	b	.L260
.L266:
	strb	r3, [fp, #-72]
	mov	ip, #1
	strb	r9, [fp, #-70]
	b	.L269
.L263:
	ldr	r1, .L279
	stmia	sp, {r0, ip}
.L276:
	ldr	ip, [r1, #68]
	mov	r0, #0
	ldr	r1, .L279+16
	blx	ip
	mov	r0, #0
	b	.L273
.L278:
	add	ip, ip, #1
	cmp	ip, #17
	movlt	r10, ip
	movge	r10, #17
	b	.L265
.L257:
	ldr	r1, .L279
	str	r4, [sp, #4]
	str	r7, [sp]
	b	.L276
.L280:
	.align	2
.L279:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC13
	.word	.LC11
	.word	.LC14
	.word	.LC12
	UNWIND(.fnend)
	.size	VP8_ArrangeVHBMem, .-VP8_ArrangeVHBMem
	.align	2
	.global	VP8_DecodeDecide
	.type	VP8_DecodeDecide, %function
VP8_DecodeDecide:
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
	add	r4, r0, #4096
	mov	r6, r0
	ldr	r5, [r4, #4028]
	cmp	r5, #0
	beq	.L307
	ldr	r3, [r4, #4020]
	sub	r3, r3, #2
	cmp	r3, #1
	ldr	r3, [r0]
	ldr	r2, [r3, #8]
	bls	.L308
.L285:
	str	r2, [r4, #4020]
	cmp	r2, #4
	ldrls	pc, [pc, r2, asl #2]
	b	.L290
.L291:
	.word	.L289
	.word	.L289
	.word	.L286
	.word	.L288
	.word	.L289
.L309:
	ldr	r3, .L311
	mov	r0, #18
	ldr	r1, .L311+4
	ldr	r3, [r3, #68]
	blx	r3
.L288:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L308:
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L285
	ldr	r3, .L311
	add	r5, r0, #8192
	ldr	r1, .L311+8
	mov	r0, #2
	mov	r7, #0
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r0, [r5, #328]
	mov	r2, #1
	ldr	r1, [r4, #4044]
	bl	FSP_ClearLogicFs
	ldr	r0, [r5, #328]
	mov	r2, #1
	ldr	r1, [r4, #4048]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r0, [r5, #328]
	ldr	r1, [r4, #4052]
	bl	FSP_ClearLogicFs
	str	r7, [r4, #4044]
	str	r7, [r4, #4048]
	str	r7, [r4, #4052]
	ldr	r0, [r5, #328]
	bl	FSP_GetNullLogicFs
	str	r7, [r4, #4024]
	str	r0, [r4, #4052]
	str	r0, [r4, #4048]
	str	r0, [r4, #4044]
	ldr	r3, [r6]
	ldr	r2, [r3, #8]
	b	.L285
.L307:
	ldr	r3, .L311
	mov	r0, #1
	ldr	r1, .L311+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L286:
	ldr	r2, [r4, #4064]
	cmp	r2, #0
	bne	.L309
.L287:
	add	r3, r6, #8192
	ldr	r1, [r4, #4052]
	ldr	r0, [r3, #328]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L310
	ldr	r3, [r4, #4064]
	cmp	r3, #0
	beq	.L296
	ldr	r3, [r6]
	ldr	r2, [r5, #200]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L296
	ldr	ip, .L311
	mov	r0, #1
	ldr	r1, .L311+16
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L289:
	ldr	r3, [r4, #4064]
	cmp	r3, #0
	beq	.L287
	ldr	r3, [r4, #4024]
	cmp	r3, #0
	bne	.L287
	ldr	ip, .L311
	mov	r0, #18
	ldr	r2, .L311+20
	ldr	r1, .L311+24
	ldr	ip, [ip, #68]
	blx	ip
	b	.L288
.L290:
	ldr	r3, .L311
	mov	r0, #1
	ldr	r1, .L311+28
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L296:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L310:
	ldr	ip, .L311
	movw	r3, #1506
	ldr	r2, .L311+32
	mov	r0, #22
	ldr	r1, .L311+36
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L312:
	.align	2
.L311:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC17
	.word	.LC16
	.word	.LC15
	.word	.LC22
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LANCHOR0+2456
	.word	.LC21
	UNWIND(.fnend)
	.size	VP8_DecodeDecide, .-VP8_DecodeDecide
	.align	2
	.global	VP8_GetImageBuffer
	.type	VP8_GetImageBuffer, %function
VP8_GetImageBuffer:
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
	add	r6, r0, #8192
	add	r5, r0, #4096
	mov	r7, r0
	mov	r1, #1
	ldr	r0, [r6, #328]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r5, #4056]
	blt	.L320
	mov	r1, r0
	ldr	r0, [r6, #328]
	bl	FSP_GetLogicFs
	subs	r4, r0, #0
	beq	.L321
	ldr	r6, .L322
	mov	r0, #18
	ldr	r2, [r5, #4056]
	ldr	r1, .L322+4
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r2, [r4, #520]
	cmp	r2, #0
	beq	.L317
	ldr	r0, [r4, #524]
	cmp	r0, #0
	beq	.L315
	ldr	r1, [r4, #528]
	cmp	r1, #0
	moveq	r0, r1
	beq	.L315
	ldr	lr, [r1, #12]
	ldr	r3, [r0, #12]
	mov	r0, #18
	ldr	r2, [r2, #12]
	ldr	ip, [r6, #68]
	ldr	r1, .L322+8
	str	lr, [sp]
	blx	ip
	mov	r0, #1
.L315:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L320:
	ldr	r3, .L322
	mov	r0, #0
	ldr	r1, .L322+12
	mov	r4, r0
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r5, #4044]
	ldr	r0, [r6, #328]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	ldr	r1, [r5, #4048]
	ldr	r0, [r6, #328]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r5, #4052]
	ldr	r0, [r6, #328]
	bl	FSP_ClearLogicFs
	str	r4, [r5, #4044]
	str	r4, [r5, #4048]
	str	r4, [r5, #4052]
	ldr	r0, [r6, #328]
	bl	FSP_GetNullLogicFs
	add	r1, r7, #8128
	str	r4, [r5, #4024]
	add	r1, r1, #40
	str	r0, [r5, #4052]
	str	r0, [r5, #4048]
	str	r0, [r5, #4044]
	ldr	r0, [r6, #328]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r4
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L321:
	ldr	r3, .L322
	movw	r2, #1562
	ldr	r1, .L322+16
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	b	.L315
.L317:
	mov	r0, r2
	b	.L315
.L323:
	.align	2
.L322:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC25
	.word	.LC26
	.word	.LC23
	.word	.LC24
	UNWIND(.fnend)
	.size	VP8_GetImageBuffer, .-VP8_GetImageBuffer
	.align	2
	.global	VP8_Set_DecParam
	.type	VP8_Set_DecParam, %function
VP8_Set_DecParam:
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
	ldr	r9, .L365
	mov	r10, r0
	add	r3, r0, #8
	ldr	ip, [r10, #3164]
	add	r8, r0, #4096
	str	r3, [fp, #-112]
	movw	r2, #3144
	mov	r1, #0
	mov	r0, r3
	ldr	r3, [r9, #48]
	add	r4, r10, #8192
	str	ip, [fp, #-124]
	blx	r3
	ldr	r3, [r8, #1620]
	str	r3, [r10, #2804]
	ldr	r3, [r8, #4068]
	str	r3, [r10, #2808]
	ldr	r3, [r8, #1660]
	str	r3, [r10, #2792]
	ldr	r3, [r8, #1664]
	str	r3, [r10, #2796]
	ldr	r3, [r8, #1648]
	str	r3, [r10, #2812]
	ldr	r3, [r8, #1652]
	str	r3, [r10, #2816]
	ldrb	r3, [r8, #1617]	@ zero_extendqisi2
	strb	r3, [r10, #2760]
	ldr	r3, [r8, #1636]
	strb	r3, [r10, #2761]
	ldrb	r3, [r8, #1577]	@ zero_extendqisi2
	strb	r3, [r10, #2762]
	ldrb	r3, [r8, #1578]	@ zero_extendqisi2
	strb	r3, [r10, #2763]
	ldrb	r3, [r8, #1579]	@ zero_extendqisi2
	strb	r3, [r10, #2764]
	ldrb	r3, [r8, #1581]	@ zero_extendqisi2
	strb	r3, [r10, #2765]
	ldrb	r3, [r8, #1566]	@ zero_extendqisi2
	strb	r3, [r10, #2766]
	ldr	r3, [r8, #1700]
	strb	r3, [r10, #2768]
	ldrb	r3, [r8, #1574]	@ zero_extendqisi2
	strb	r3, [r10, #2770]
	ldrb	r3, [r8, #1575]	@ zero_extendqisi2
	strb	r3, [r10, #2771]
	ldr	r3, [r8, #4032]
	strb	r3, [r10, #2769]
	ldr	r3, [r8, #4036]
	strb	r3, [r10, #2772]
	ldrb	r3, [r8, #1567]	@ zero_extendqisi2
	strb	r3, [r10, #2767]
	ldrb	r3, [r8, #1569]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2773]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2774]
	ldrb	r3, [r8, #1570]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2775]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2776]
	ldrb	r3, [r8, #1571]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2777]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2778]
	ldrb	r3, [r8, #1572]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2779]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2780]
	ldrb	r3, [r8, #1573]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2781]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2782]
	ldrb	r3, [r8, #1614]	@ zero_extendqisi2
	strb	r3, [r10, #2783]
	ldrb	r3, [r8, #1615]	@ zero_extendqisi2
	strb	r3, [r10, #2784]
	ldrb	r3, [r8, #1616]	@ zero_extendqisi2
	strb	r3, [r10, #2785]
	ldr	r3, [r8, #1708]
	str	r3, [r10, #2788]
	ldr	r3, [r8, #1704]
	strb	r3, [r10, #2786]
	ldr	r3, [r8, #1712]
	strb	r3, [r10, #2787]
	ldr	r3, [r8, #4040]
	str	r3, [r10, #3144]
	ldr	r1, [r8, #4056]
	ldr	r0, [r4, #328]
	bl	FSP_GetLogicFs
	ldr	r1, [r8, #4044]
	mov	r6, r0
	ldr	r0, [r4, #328]
	bl	FSP_GetLogicFs
	ldr	r1, [r8, #4048]
	mov	r5, r0
	ldr	r0, [r4, #328]
	bl	FSP_GetLogicFs
	ldr	r1, [r8, #4052]
	mov	r7, r0
	ldr	r0, [r4, #328]
	bl	FSP_GetLogicFs
	cmp	r5, #0
	cmpne	r6, #0
	beq	.L325
	cmp	r0, #0
	cmpne	r7, #0
	beq	.L325
	ldr	r4, [r6, #520]
	cmp	r4, #0
	beq	.L364
	ldr	r3, [r4, #12]
	add	r1, r10, #5696
	add	r1, r1, #9
	mov	r2, #0
	str	r3, [r10, #3124]
	ldr	r3, [r6, #532]
	str	r3, [r10, #3140]
	ldr	r3, [r5, #520]
	add	r5, r10, #5632
	cmp	r3, #0
	mov	r4, r5
	mov	lr, r5
	add	r4, r4, #53
	ldreq	r3, [r6, #520]
	add	lr, lr, #59
	ldr	r3, [r3, #12]
	str	r3, [r10, #3132]
	ldr	r3, [r7, #520]
	cmp	r3, #0
	ldreq	r3, [r6, #520]
	ldr	r3, [r3, #12]
	str	r3, [r10, #3128]
	ldr	r3, [r0, #520]
	mov	r0, r5
	add	r5, r5, #49
	add	r0, r0, #63
	cmp	r3, #0
	ldreq	r3, [r6, #520]
	ldr	r3, [r3, #12]
	str	r10, [fp, #-128]
	str	r3, [r10, #3136]
	ldr	r3, [r6, #524]
	ldr	r6, [r3, #20]
	mov	r3, r10
	str	r6, [r8, #4016]
	str	r6, [r10, #2800]
.L335:
	ldrsb	ip, [r5, #1]!
	add	r2, r2, #1
	cmp	r2, #4
	add	r3, r3, #4
	str	ip, [r3, #3040]
	ldrsb	ip, [r4, #1]!
	str	ip, [r3, #3056]
	ldrsb	ip, [lr, #1]!
	str	ip, [r3, #3072]
	ldrsb	ip, [r0, #1]!
	str	ip, [r3, #3088]
	ldrb	ip, [r1, #1]!	@ zero_extendqisi2
	str	ip, [r3, #3104]
	bne	.L335
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	ldr	r0, [fp, #-112]
	blx	r3
	ldrb	r3, [r8, #1617]	@ zero_extendqisi2
	add	ip, r10, #6976
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #24
	strb	r3, [r10, #8]
	add	r3, ip, #2
	str	r3, [fp, #-120]
	mov	ip, #384
	ldrb	r3, [r8, #1582]	@ zero_extendqisi2
	str	ip, [fp, #-116]
	strb	r3, [r10, #9]
	ldrb	r3, [r8, #1583]	@ zero_extendqisi2
	strb	r3, [r10, #10]
	ldrb	r3, [r8, #1584]	@ zero_extendqisi2
	strb	r3, [r10, #11]
	ldrb	r3, [r8, #1614]	@ zero_extendqisi2
	strb	r3, [r10, #12]
	ldrb	r3, [r8, #1615]	@ zero_extendqisi2
	strb	r3, [r10, #13]
	ldrb	r3, [r8, #1616]	@ zero_extendqisi2
	strb	r3, [r10, #14]
	ldr	r3, [r9, #48]
	blx	r3
	ldrb	r3, [r8, #2874]	@ zero_extendqisi2
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #40
	strb	r3, [r10, #24]
	ldrb	r3, [r8, #2875]	@ zero_extendqisi2
	strb	r3, [r10, #25]
	ldrb	r3, [r8, #2876]	@ zero_extendqisi2
	strb	r3, [r10, #26]
	ldrb	r3, [r8, #2877]	@ zero_extendqisi2
	strb	r3, [r10, #27]
	ldrb	r3, [r8, #2878]	@ zero_extendqisi2
	strb	r3, [r10, #28]
	ldrb	r3, [r8, #2879]	@ zero_extendqisi2
	strb	r3, [r10, #29]
	ldrb	r3, [r8, #2880]	@ zero_extendqisi2
	strb	r3, [r10, #30]
	ldr	r3, [r9, #48]
	blx	r3
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #56
	blx	r3
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #712
	blx	r3
	ldrb	lr, [r8, #3939]	@ zero_extendqisi2
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #728
	strb	lr, [r10, #712]
	ldrb	lr, [r8, #3938]	@ zero_extendqisi2
	strb	lr, [r10, #713]
	ldrb	lr, [r8, #3940]	@ zero_extendqisi2
	strb	lr, [r10, #714]
	ldrb	lr, [r8, #3941]	@ zero_extendqisi2
	strb	lr, [r10, #715]
	ldrb	lr, [r8, #3942]	@ zero_extendqisi2
	strb	lr, [r10, #716]
	ldrb	lr, [r8, #3943]	@ zero_extendqisi2
	strb	lr, [r10, #717]
	ldrb	lr, [r8, #3944]	@ zero_extendqisi2
	strb	lr, [r10, #718]
	ldrb	lr, [r8, #3945]	@ zero_extendqisi2
	strb	lr, [r10, #719]
	ldrb	lr, [r8, #3946]	@ zero_extendqisi2
	strb	lr, [r10, #720]
	blx	r3
	ldrb	lr, [r8, #3939]	@ zero_extendqisi2
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #744
	strb	lr, [r10, #728]
	ldrb	lr, [r8, #3947]	@ zero_extendqisi2
	strb	lr, [r10, #729]
	ldrb	lr, [r8, #3948]	@ zero_extendqisi2
	strb	lr, [r10, #730]
	ldrb	lr, [r8, #3949]	@ zero_extendqisi2
	strb	lr, [r10, #731]
	ldrb	lr, [r8, #3950]	@ zero_extendqisi2
	strb	lr, [r10, #732]
	ldrb	lr, [r8, #3951]	@ zero_extendqisi2
	strb	lr, [r10, #733]
	ldrb	lr, [r8, #3952]	@ zero_extendqisi2
	strb	lr, [r10, #734]
	ldrb	lr, [r8, #3953]	@ zero_extendqisi2
	strb	lr, [r10, #735]
	ldrb	lr, [r8, #3954]	@ zero_extendqisi2
	strb	lr, [r10, #736]
	ldrb	lr, [r8, #3955]	@ zero_extendqisi2
	strb	lr, [r10, #737]
	ldrb	lr, [r8, #3956]	@ zero_extendqisi2
	strb	lr, [r10, #738]
	blx	r3
	ldrb	lr, [r8, #3958]	@ zero_extendqisi2
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #760
	strb	lr, [r10, #744]
	ldrb	lr, [r8, #3957]	@ zero_extendqisi2
	strb	lr, [r10, #745]
	ldrb	lr, [r8, #3959]	@ zero_extendqisi2
	strb	lr, [r10, #746]
	ldrb	lr, [r8, #3960]	@ zero_extendqisi2
	strb	lr, [r10, #747]
	ldrb	lr, [r8, #3961]	@ zero_extendqisi2
	strb	lr, [r10, #748]
	ldrb	lr, [r8, #3962]	@ zero_extendqisi2
	strb	lr, [r10, #749]
	ldrb	lr, [r8, #3963]	@ zero_extendqisi2
	strb	lr, [r10, #750]
	ldrb	lr, [r8, #3964]	@ zero_extendqisi2
	strb	lr, [r10, #751]
	ldrb	lr, [r8, #3965]	@ zero_extendqisi2
	strb	lr, [r10, #752]
	blx	r3
	ldrb	r3, [r8, #3958]	@ zero_extendqisi2
	str	r10, [fp, #-132]
	str	r8, [fp, #-136]
	strb	r3, [r10, #760]
	ldrb	r3, [r8, #3966]	@ zero_extendqisi2
	strb	r3, [r10, #761]
	ldrb	r3, [r8, #3967]	@ zero_extendqisi2
	strb	r3, [r10, #762]
	ldrb	r3, [r8, #3968]	@ zero_extendqisi2
	strb	r3, [r10, #763]
	ldrb	r3, [r8, #3969]	@ zero_extendqisi2
	strb	r3, [r10, #764]
	ldrb	r3, [r8, #3970]	@ zero_extendqisi2
	strb	r3, [r10, #765]
	ldrb	r3, [r8, #3971]	@ zero_extendqisi2
	strb	r3, [r10, #766]
	ldrb	r3, [r8, #3972]	@ zero_extendqisi2
	strb	r3, [r10, #767]
	ldrb	r3, [r8, #3973]	@ zero_extendqisi2
	strb	r3, [r10, #768]
	ldrb	r3, [r8, #3974]	@ zero_extendqisi2
	strb	r3, [r10, #769]
	ldrb	r3, [r8, #3975]	@ zero_extendqisi2
	strb	r3, [r10, #770]
.L336:
	ldr	r3, [fp, #-116]
	ldr	r7, [fp, #-120]
	sub	r5, r3, #384
.L340:
	add	r6, r5, #768
	mov	r10, #0
	mov	r4, r7
	mov	r3, r6
	mov	r6, r10
	mov	r10, r3
.L337:
	ldr	r3, [fp, #-112]
	add	r8, r10, r6
	mov	r2, #16
	ldr	ip, [r9, #48]
	add	r8, r3, r8
	mov	r1, #0
	add	r6, r6, r2
	mov	r0, r8
	blx	ip
	mov	r2, #11
	mov	r1, r4
	mov	r0, r8
	ldr	ip, [r9, #52]
	add	r4, r4, r2
	blx	ip
	cmp	r6, #48
	bne	.L337
	ldr	r3, [fp, #-116]
	add	r5, r5, #48
	add	r7, r7, #33
	cmp	r3, r5
	bne	.L340
	ldr	r3, [fp, #-116]
	ldr	r2, [fp, #-120]
	add	r3, r3, #384
	str	r3, [fp, #-116]
	cmp	r3, #1920
	add	r2, r2, #264
	str	r2, [fp, #-120]
	bne	.L336
	ldr	r8, [fp, #-136]
	ldr	r6, [fp, #-124]
	ldr	r10, [fp, #-132]
	ldr	r2, [r8, #1620]
	ldr	r3, [r6, #4]
	add	r7, r10, #3168
	cmp	r2, #0
	bic	r3, r3, #15
	mov	r0, r7
	sub	r3, r3, #16
	str	r3, [r10, #2820]
	moveq	r4, #10
	movne	r4, #3
	ldr	r5, [r8, #1632]
	bl	BsPos
	str	r6, [fp, #-124]
	add	r5, r4, r5
	mov	r5, r5, asl #3
	rsb	r0, r0, r5
	str	r0, [r10, #2824]
	mov	r0, r7
	ldr	r6, [r6, #4]
	bl	BsPos
	and	r6, r6, #15
	add	r6, r6, #16
	cmp	r0, #0
	add	r5, r0, #7
	movge	r5, r0
	mov	r0, r7
	bl	BsPos
	add	r5, r6, r5, asr #3
	and	r1, r5, #15
	mov	ip, #0
	bic	r5, r5, #15
	str	ip, [r10, #2836]
	str	r5, [r10, #2832]
	str	ip, [r10, #2840]
	str	ip, [r10, #2844]
	mov	r3, r0, asr #31
	mov	r3, r3, lsr #29
	add	r2, r0, r3
	and	r2, r2, #7
	rsb	r3, r3, r2
	add	r3, r3, r1, lsl #3
	str	r3, [r10, #2828]
	ldrb	r9, [r8, #1577]	@ zero_extendqisi2
	mov	r3, #1
	ldr	r1, [r8, #1632]
	mov	r9, r3, asl r9
	ldr	r3, [fp, #-124]
	uxtb	r9, r9
	ldr	r2, [r3, #8]
	cmp	r9, #1
	ldr	r0, [r3]
	add	r3, r4, r1
	rsb	r2, r4, r2
	subhi	r4, r9, #1
	add	r0, r0, r3
	str	r3, [fp, #-116]
	addhi	r4, r4, r4, lsl #1
	str	r9, [r10, #2848]
	addhi	r4, r0, r4
	movls	r4, r0
	cmp	r9, #0
	ble	.L351
	rsb	r3, r9, #1
	sub	lr, fp, #76
	sub	r6, r9, #1
	mov	ip, #0
	add	r3, r3, r3, lsl #1
	mov	r5, lr
	rsb	r3, r1, r3
	add	r7, r3, r2
.L346:
	cmp	ip, r6
	add	ip, ip, #1
	rsbge	r3, r4, r0
	add	r0, r0, #3
	ldrltb	r3, [r0, #-2]	@ zero_extendqisi2
	addge	r3, r3, r7
	ldrltb	r2, [r0, #-3]	@ zero_extendqisi2
	ldrltb	r1, [r0, #-1]	@ zero_extendqisi2
	addlt	r3, r2, r3, lsl #8
	addlt	r3, r3, r1, lsl #16
	cmp	ip, r9
	str	r3, [r5], #4
	add	r4, r4, r3
	bne	.L346
	ldr	r3, [fp, #-124]
	add	r6, r6, r6, lsl #1
	ldr	r1, [fp, #-116]
	mov	r2, #0
	ldr	r0, [fp, #-128]
	add	r8, r10, #2848
	ldr	r3, [r3, #4]
	add	r7, r10, #2880
	add	r5, r10, #2944
	mov	r4, r2
	and	r3, r3, #15
	str	r10, [fp, #-116]
	add	r3, r6, r3
	add	r6, r10, #2912
	add	r3, r3, r1
	sub	r1, fp, #108
	str	r3, [fp, #-120]
.L350:
	cmp	r2, #0
	add	r2, r2, #1
	add	r1, r1, #4
	add	r0, r0, #4
	ldreq	r3, [fp, #-120]
	ldrne	r3, [r1, #-8]
	ldrne	ip, [lr, #-4]
	streq	r3, [fp, #-108]
	ldreq	r3, [r1, #-4]
	addne	r3, ip, r3
	ldr	r10, [lr], #4
	strne	r3, [r1, #-4]
	and	ip, r3, #15
	cmp	r2, r9
	bic	r3, r3, #15
	mov	r10, r10, asl #3
	mov	ip, ip, asl #3
	str	r10, [r8, #4]!
	add	r3, r3, #16
	str	ip, [r7, #4]!
	str	r3, [r6, #4]!
	str	r4, [r5, #4]!
	str	r4, [r0, #2976]
	str	r4, [r0, #3008]
	bne	.L350
	ldr	r10, [fp, #-116]
.L351:
	ldr	r3, [r10]
	mov	r0, #1
	ldr	r2, [fp, #-112]
	str	r2, [r10, #4]
	ldr	r3, [r3, #676]
	str	r3, [r10, #3148]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L325:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L365+4
	blx	r3
	mov	r0, #0
.L358:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L364:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L365+8
	blx	r3
	mov	r0, r4
	b	.L358
.L366:
	.align	2
.L365:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC27
	.word	.LC28
	UNWIND(.fnend)
	.size	VP8_Set_DecParam, .-VP8_Set_DecParam
	.align	2
	.global	VP8_Update_PictHeader
	.type	VP8_Update_PictHeader, %function
VP8_Update_PictHeader:
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
	ldrb	r3, [r0, #2505]	@ zero_extendqisi2
	add	r5, r0, #2448
	mov	r4, r0
	add	r5, r5, #12
	cmp	r3, #0
	add	r7, r0, #8
	ldr	r6, .L371
	bne	.L368
	add	r0, r0, #3760
	movw	r2, #1149
	add	r0, r0, #1
	add	r1, r5, #152
	ldr	r3, [r6, #52]
	blx	r3
.L368:
	ldr	r3, [r6, #52]
	movw	r2, #2452
	mov	r1, r5
	mov	r0, r7
	blx	r3
	ldr	r2, [r4, #2516]
	add	r3, r4, #4096
	str	r2, [r3, #868]
	ldr	r2, [r4, #2596]
	str	r2, [r3, #832]
	ldrb	r2, [r4, #2471]	@ zero_extendqisi2
	str	r2, [r3, #836]
	ldrb	r3, [r4, #2500]	@ zero_extendqisi2
	strb	r3, [r4]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L372:
	.align	2
.L371:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VP8_Update_PictHeader, .-VP8_Update_PictHeader
	.align	2
	.global	VP8DEC_Decode
	.type	VP8DEC_Decode, %function
VP8DEC_Decode:
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
	str	r3, [r0, #3152]
	ldr	r2, [r1, #8]
	str	r2, [r0, #3156]
	ldr	r2, [r1, #12]
	cmp	r2, #2
	str	r2, [r0, #3160]
	ble	.L404
	add	r5, r0, #3168
	add	r7, r0, #3200
	mov	r1, r3
	mov	r0, r5
	bl	BsInit
	add	r3, r4, #3152
	mov	r0, r5
	mov	r1, r7
	ldr	r2, [r4, #3160]
	str	r3, [r4, #3164]
	bl	VP8_GetPictureHeader
	cmp	r0, #1
	bne	.L405
	add	r5, r4, #4096
	ldr	r3, [r5, #4028]
	cmp	r3, #0
	bne	.L406
.L377:
	ldr	r3, .L410
	ldr	ip, [r3]
	cmp	ip, #0
	addeq	r6, r4, #8192
	beq	.L379
	ldr	r0, [r5, #1664]
	add	r6, r4, #8192
	ldr	r9, [r4, #3304]
	mov	r3, #8
	ldr	r8, [r4, #3308]
	sub	r2, fp, #52
	ldr	lr, [r5, #1660]
	mov	r1, #2
	strh	r0, [fp, #-46]	@ movhi
	strh	r9, [fp, #-52]	@ movhi
	strh	r8, [fp, #-50]	@ movhi
	strh	lr, [fp, #-48]	@ movhi
	ldr	r0, [r6, #328]
	blx	ip
.L379:
	mov	r2, #1
	ldr	r1, [r5, #4044]
	ldr	r0, [r6, #328]
	mov	r8, #0
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r5, #4048]
	add	r9, r4, #8128
	ldr	r0, [r6, #328]
	add	r9, r9, #40
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r5, #4052]
	ldr	r0, [r6, #328]
	bl	FSP_ClearLogicFs
	str	r8, [r5, #4044]
	str	r8, [r5, #4048]
	str	r8, [r5, #4052]
	ldr	r0, [r6, #328]
	bl	FSP_GetNullLogicFs
	str	r8, [r5, #4024]
	mov	r1, r9
	str	r0, [r5, #4052]
	str	r0, [r5, #4048]
	str	r0, [r5, #4044]
	ldr	r0, [r6, #328]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [r5, #1660]
	ldr	r3, [r5, #1664]
	cmp	r2, #512
	bhi	.L381
	cmp	r3, #512
	bhi	.L381
	ldr	r3, [r4]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L383
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L407
.L383:
	ldr	r0, [r6, #328]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L403
.L385:
	mov	r0, r4
	bl	VP8_ArrangeVHBMem
	cmp	r0, #1
	bne	.L408
.L378:
	mov	r0, r4
	bl	VP8_DecodeDecide
	cmp	r0, #1
	mov	r6, r0
	beq	.L387
	ldr	r3, .L410+4
	mov	r0, #1
	ldr	r1, .L410+8
	ldr	r3, [r3, #68]
	blx	r3
	b	.L375
.L405:
	ldr	r3, .L410+4
	mov	r0, #2
	ldr	r1, .L410+12
	ldr	r3, [r3, #68]
	blx	r3
.L375:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L404:
	ldr	r3, .L410+4
	mov	r0, #1
	ldr	r1, .L410+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L406:
	ldr	r2, [r4, #3304]
	ldr	r3, [r5, #1660]
	cmp	r2, r3
	bne	.L377
	ldr	r2, [r4, #3308]
	ldr	r3, [r5, #1664]
	cmp	r2, r3
	bne	.L377
	b	.L378
.L381:
	ldr	ip, .L410+4
	mov	r0, #18
	ldr	r1, .L410+20
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L408:
	ldr	r3, .L410+4
	mov	r0, #1
	ldr	r1, .L410+24
	ldr	r3, [r3, #68]
	blx	r3
	b	.L375
.L407:
	ldr	r3, [r5, #4028]
	cmp	r3, #0
	bne	.L385
.L403:
	mov	r0, r9
	bl	ResetVoQueue
	b	.L385
.L387:
	mov	r0, r4
	bl	VP8_GetImageBuffer
	cmp	r0, #1
	beq	.L388
	ldr	r3, .L410+4
	mov	r0, r6
	ldr	r1, .L410+28
	ldr	r3, [r3, #68]
	blx	r3
	b	.L375
.L388:
	add	r6, r4, #8192
	ldr	r1, [r5, #4056]
	ldr	r0, [r6, #328]
	bl	FSP_GetFsImagePtr
	subs	lr, r0, #0
	beq	.L409
	ldr	r8, [r4]
	mov	r10, #0
	mvn	r2, #0
	mvn	r3, #0
	ldr	ip, .L410+4
	mov	r0, #29
	ldrd	r8, [r8, #48]
	ldr	r1, .L410+32
	strd	r8, [lr, #8]
	ldr	r8, [r4]
	ldrd	r8, [r8, #64]
	strd	r8, [lr, #16]
	ldr	r8, [r4]
	ldrd	r8, [r8, #80]
	strd	r8, [lr, #24]
	ldr	r8, [r4]
	ldr	r8, [r8, #96]
	str	r8, [lr, #68]
	ldr	r8, [r4]
	ldr	r8, [r8, #100]
	str	r8, [lr, #72]
	ldr	r8, [r4]
	ldr	r8, [r8, #104]
	str	r8, [lr, #76]
	ldr	r8, [r4]
	ldr	r8, [r8, #120]
	str	r10, [lr, #148]
	str	r8, [lr, #80]
	ldr	r8, [r4]
	strd	r2, [r8, #48]
	ldrd	r8, [lr, #16]
	ldrd	r2, [lr, #8]
	ldr	ip, [ip, #68]
	strd	r8, [sp]
	blx	ip
	ldr	r2, [r5, #4064]
	cmp	r2, r10
	bne	.L390
	ldr	r1, [r5, #4048]
	ldr	r3, [r5, #4056]
	cmp	r1, r3
	beq	.L391
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
	ldr	r3, [r5, #4056]
.L391:
	ldr	r1, [r5, #4052]
	cmp	r1, r3
	beq	.L392
	mov	r2, #0
	ldr	r0, [r6, #328]
	bl	FSP_SetRef
	ldr	r3, [r5, #4056]
.L392:
	ldr	r1, [r5, #4044]
	cmp	r1, r3
	beq	.L393
	ldr	r0, [r6, #328]
	mov	r2, #0
	bl	FSP_SetRef
.L393:
	mov	r3, #0
	str	r3, [r5, #4024]
.L390:
	mov	r0, r4
	bl	VP8_Set_DecParam
	mov	r0, r7
	bl	VP8_Update_PictHeader
	b	.L375
.L409:
	ldr	r3, .L410+4
	movw	r2, #2007
	ldr	r1, .L410+36
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	b	.L375
.L411:
	.align	2
.L410:
	.word	g_event_report
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC33
	.word	.LC30
	.word	.LC29
	.word	.LC31
	.word	.LC32
	.word	.LC34
	.word	.LC35
	.word	.LC24
	UNWIND(.fnend)
	.size	VP8DEC_Decode, .-VP8DEC_Decode
	.align	2
	.global	VP8DEC_RecycleImage
	.type	VP8DEC_RecycleImage, %function
VP8DEC_RecycleImage:
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
	add	r4, r0, #8192
	mov	r2, #0
	ldr	r0, [r4, #328]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #328]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L415
	ldr	r1, [r5, #212]
	mov	r6, #0
	ldr	r0, [r4, #328]
	bl	FreeUsdByDec
	str	r6, [r5, #212]
	ldr	r1, [r5, #216]
	ldr	r0, [r4, #328]
	bl	FreeUsdByDec
	str	r6, [r5, #216]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L415:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP8DEC_RecycleImage, .-VP8DEC_RecycleImage
	.align	2
	.global	VP8DEC_GetRemainImg
	.type	VP8DEC_GetRemainImg, %function
VP8DEC_GetRemainImg:
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
	add	r0, r0, #8128
	add	r0, r0, #40
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	GetVoLastImageID
	UNWIND(.fnend)
	.size	VP8DEC_GetRemainImg, .-VP8DEC_GetRemainImg
	.align	2
	.global	VP8DEC_GetImageBuffer
	.type	VP8DEC_GetImageBuffer, %function
VP8DEC_GetImageBuffer:
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
	add	r4, r0, #8192
	mov	r5, r0
	ldr	r0, [r4, #328]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #328]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L423
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L423:
	add	r1, r5, #8128
	ldr	r0, [r4, #328]
	add	r1, r1, #40
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VP8DEC_GetImageBuffer, .-VP8DEC_GetImageBuffer
	.global	default_uv_mode_prob
	.global	default_ymode_prob
	.global	VP8_VFMWBITREADER_NORM
	.global	vp8_mv_update_probs
	.global	vp8_default_mv_context
	.global	vp8_coef_update_probs
	.global	default_coef_probs
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	vp8_default_mv_context, %object
	.size	vp8_default_mv_context, 38
vp8_default_mv_context:
	.byte	-94
	.byte	-128
	.byte	-31
	.byte	-110
	.byte	-84
	.byte	-109
	.byte	-42
	.byte	39
	.byte	-100
	.byte	-128
	.byte	-127
	.byte	-124
	.byte	75
	.byte	-111
	.byte	-78
	.byte	-50
	.byte	-17
	.byte	-2
	.byte	-2
	.byte	-92
	.byte	-128
	.byte	-52
	.byte	-86
	.byte	119
	.byte	-21
	.byte	-116
	.byte	-26
	.byte	-28
	.byte	-128
	.byte	-126
	.byte	-126
	.byte	74
	.byte	-108
	.byte	-76
	.byte	-53
	.byte	-20
	.byte	-2
	.byte	-2
	.space	2
	.type	default_coef_probs, %object
	.size	default_coef_probs, 1056
default_coef_probs:
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-3
	.byte	-120
	.byte	-2
	.byte	-1
	.byte	-28
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-67
	.byte	-127
	.byte	-14
	.byte	-1
	.byte	-29
	.byte	-43
	.byte	-1
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	106
	.byte	126
	.byte	-29
	.byte	-4
	.byte	-42
	.byte	-47
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	98
	.byte	-8
	.byte	-1
	.byte	-20
	.byte	-30
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-75
	.byte	-123
	.byte	-18
	.byte	-2
	.byte	-35
	.byte	-22
	.byte	-1
	.byte	-102
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	78
	.byte	-122
	.byte	-54
	.byte	-9
	.byte	-58
	.byte	-76
	.byte	-1
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-71
	.byte	-7
	.byte	-1
	.byte	-13
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-72
	.byte	-106
	.byte	-9
	.byte	-1
	.byte	-20
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	77
	.byte	110
	.byte	-40
	.byte	-1
	.byte	-20
	.byte	-26
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	101
	.byte	-5
	.byte	-1
	.byte	-15
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-86
	.byte	-117
	.byte	-15
	.byte	-4
	.byte	-20
	.byte	-47
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	37
	.byte	116
	.byte	-60
	.byte	-13
	.byte	-28
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-52
	.byte	-2
	.byte	-1
	.byte	-11
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-49
	.byte	-96
	.byte	-6
	.byte	-1
	.byte	-18
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	102
	.byte	103
	.byte	-25
	.byte	-1
	.byte	-45
	.byte	-85
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-104
	.byte	-4
	.byte	-1
	.byte	-16
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-79
	.byte	-121
	.byte	-13
	.byte	-1
	.byte	-22
	.byte	-31
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	80
	.byte	-127
	.byte	-45
	.byte	-1
	.byte	-62
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-10
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-58
	.byte	35
	.byte	-19
	.byte	-33
	.byte	-63
	.byte	-69
	.byte	-94
	.byte	-96
	.byte	-111
	.byte	-101
	.byte	62
	.byte	-125
	.byte	45
	.byte	-58
	.byte	-35
	.byte	-84
	.byte	-80
	.byte	-36
	.byte	-99
	.byte	-4
	.byte	-35
	.byte	1
	.byte	68
	.byte	47
	.byte	-110
	.byte	-48
	.byte	-107
	.byte	-89
	.byte	-35
	.byte	-94
	.byte	-1
	.byte	-33
	.byte	-128
	.byte	1
	.byte	-107
	.byte	-15
	.byte	-1
	.byte	-35
	.byte	-32
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-72
	.byte	-115
	.byte	-22
	.byte	-3
	.byte	-34
	.byte	-36
	.byte	-1
	.byte	-57
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	81
	.byte	99
	.byte	-75
	.byte	-14
	.byte	-80
	.byte	-66
	.byte	-7
	.byte	-54
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	-127
	.byte	-24
	.byte	-3
	.byte	-42
	.byte	-59
	.byte	-14
	.byte	-60
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	99
	.byte	121
	.byte	-46
	.byte	-6
	.byte	-55
	.byte	-58
	.byte	-1
	.byte	-54
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	23
	.byte	91
	.byte	-93
	.byte	-14
	.byte	-86
	.byte	-69
	.byte	-9
	.byte	-46
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	-56
	.byte	-10
	.byte	-1
	.byte	-22
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	109
	.byte	-78
	.byte	-15
	.byte	-1
	.byte	-25
	.byte	-11
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	44
	.byte	-126
	.byte	-55
	.byte	-3
	.byte	-51
	.byte	-64
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-124
	.byte	-17
	.byte	-5
	.byte	-37
	.byte	-47
	.byte	-1
	.byte	-91
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	94
	.byte	-120
	.byte	-31
	.byte	-5
	.byte	-38
	.byte	-66
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	22
	.byte	100
	.byte	-82
	.byte	-11
	.byte	-70
	.byte	-95
	.byte	-1
	.byte	-57
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-74
	.byte	-7
	.byte	-1
	.byte	-24
	.byte	-21
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	124
	.byte	-113
	.byte	-15
	.byte	-1
	.byte	-29
	.byte	-22
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	35
	.byte	77
	.byte	-75
	.byte	-5
	.byte	-63
	.byte	-45
	.byte	-1
	.byte	-51
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-99
	.byte	-9
	.byte	-1
	.byte	-20
	.byte	-25
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	121
	.byte	-115
	.byte	-21
	.byte	-1
	.byte	-31
	.byte	-29
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	45
	.byte	99
	.byte	-68
	.byte	-5
	.byte	-61
	.byte	-39
	.byte	-1
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	1
	.byte	-5
	.byte	-1
	.byte	-43
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-53
	.byte	1
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-119
	.byte	1
	.byte	-79
	.byte	-1
	.byte	-32
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-3
	.byte	9
	.byte	-8
	.byte	-5
	.byte	-49
	.byte	-48
	.byte	-1
	.byte	-64
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-81
	.byte	13
	.byte	-32
	.byte	-13
	.byte	-63
	.byte	-71
	.byte	-7
	.byte	-58
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	73
	.byte	17
	.byte	-85
	.byte	-35
	.byte	-95
	.byte	-77
	.byte	-20
	.byte	-89
	.byte	-1
	.byte	-22
	.byte	-128
	.byte	1
	.byte	95
	.byte	-9
	.byte	-3
	.byte	-44
	.byte	-73
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-17
	.byte	90
	.byte	-12
	.byte	-6
	.byte	-45
	.byte	-47
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-101
	.byte	77
	.byte	-61
	.byte	-8
	.byte	-68
	.byte	-61
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	24
	.byte	-17
	.byte	-5
	.byte	-38
	.byte	-37
	.byte	-1
	.byte	-51
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-55
	.byte	51
	.byte	-37
	.byte	-1
	.byte	-60
	.byte	-70
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	69
	.byte	46
	.byte	-66
	.byte	-17
	.byte	-55
	.byte	-38
	.byte	-1
	.byte	-28
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-65
	.byte	-5
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-33
	.byte	-91
	.byte	-7
	.byte	-1
	.byte	-43
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-115
	.byte	124
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	16
	.byte	-8
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-66
	.byte	36
	.byte	-26
	.byte	-1
	.byte	-20
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-107
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-30
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-9
	.byte	-64
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-16
	.byte	-128
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-122
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-43
	.byte	62
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	55
	.byte	93
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-54
	.byte	24
	.byte	-43
	.byte	-21
	.byte	-70
	.byte	-65
	.byte	-36
	.byte	-96
	.byte	-16
	.byte	-81
	.byte	-1
	.byte	126
	.byte	38
	.byte	-74
	.byte	-24
	.byte	-87
	.byte	-72
	.byte	-28
	.byte	-82
	.byte	-1
	.byte	-69
	.byte	-128
	.byte	61
	.byte	46
	.byte	-118
	.byte	-37
	.byte	-105
	.byte	-78
	.byte	-16
	.byte	-86
	.byte	-1
	.byte	-40
	.byte	-128
	.byte	1
	.byte	112
	.byte	-26
	.byte	-6
	.byte	-57
	.byte	-65
	.byte	-9
	.byte	-97
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-90
	.byte	109
	.byte	-28
	.byte	-4
	.byte	-45
	.byte	-41
	.byte	-1
	.byte	-82
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	39
	.byte	77
	.byte	-94
	.byte	-24
	.byte	-84
	.byte	-76
	.byte	-11
	.byte	-78
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	52
	.byte	-36
	.byte	-10
	.byte	-58
	.byte	-57
	.byte	-7
	.byte	-36
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	124
	.byte	74
	.byte	-65
	.byte	-13
	.byte	-73
	.byte	-63
	.byte	-6
	.byte	-35
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	24
	.byte	71
	.byte	-126
	.byte	-37
	.byte	-102
	.byte	-86
	.byte	-13
	.byte	-74
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	-74
	.byte	-31
	.byte	-7
	.byte	-37
	.byte	-16
	.byte	-1
	.byte	-32
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-107
	.byte	-106
	.byte	-30
	.byte	-4
	.byte	-40
	.byte	-51
	.byte	-1
	.byte	-85
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	28
	.byte	108
	.byte	-86
	.byte	-14
	.byte	-73
	.byte	-62
	.byte	-2
	.byte	-33
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	1
	.byte	81
	.byte	-26
	.byte	-4
	.byte	-52
	.byte	-53
	.byte	-1
	.byte	-64
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	123
	.byte	102
	.byte	-47
	.byte	-9
	.byte	-68
	.byte	-60
	.byte	-1
	.byte	-23
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	20
	.byte	95
	.byte	-103
	.byte	-13
	.byte	-92
	.byte	-83
	.byte	-1
	.byte	-53
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	-34
	.byte	-8
	.byte	-1
	.byte	-40
	.byte	-43
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-88
	.byte	-81
	.byte	-10
	.byte	-4
	.byte	-21
	.byte	-51
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	47
	.byte	116
	.byte	-41
	.byte	-1
	.byte	-45
	.byte	-44
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	121
	.byte	-20
	.byte	-3
	.byte	-44
	.byte	-42
	.byte	-1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-115
	.byte	84
	.byte	-43
	.byte	-4
	.byte	-55
	.byte	-54
	.byte	-1
	.byte	-37
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	42
	.byte	80
	.byte	-96
	.byte	-16
	.byte	-94
	.byte	-71
	.byte	-1
	.byte	-51
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	1
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-12
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-18
	.byte	1
	.byte	-1
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.byte	-128
	.type	VP8_VFMWBITREADER_NORM, %object
	.size	VP8_VFMWBITREADER_NORM, 256
VP8_VFMWBITREADER_NORM:
	.byte	0
	.byte	7
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
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
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
	.byte	0
	.byte	0
	.type	vp8_mv_update_probs, %object
	.size	vp8_mv_update_probs, 38
vp8_mv_update_probs:
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
	.byte	-2
	.byte	-2
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
	.byte	-2
	.byte	-2
	.space	2
.LC0:
	.word	7
	.word	6
	.type	vp8_coef_update_probs, %object
	.size	vp8_coef_update_probs, 1056
vp8_coef_update_probs:
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
	.byte	-80
	.byte	-10
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-33
	.byte	-15
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-3
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
	.byte	-12
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-22
	.byte	-2
	.byte	-2
	.byte	-1
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
	.byte	-10
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-17
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-1
	.byte	-2
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
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-3
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-6
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-39
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
	.byte	-4
	.byte	-15
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-22
	.byte	-6
	.byte	-15
	.byte	-6
	.byte	-3
	.byte	-1
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-33
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-18
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-2
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
	.byte	-9
	.byte	-2
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
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-4
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
	.byte	-2
	.byte	-2
	.byte	-1
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
	.byte	-2
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-6
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
	.byte	-2
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
	.byte	-70
	.byte	-5
	.byte	-6
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-22
	.byte	-5
	.byte	-12
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-5
	.byte	-13
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-20
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-2
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
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-2
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
	.byte	-6
	.byte	-2
	.byte	-4
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-8
	.byte	-2
	.byte	-7
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-10
	.byte	-3
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-4
	.byte	-2
	.byte	-5
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-2
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-11
	.byte	-5
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-5
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-4
	.byte	-3
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
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
	.byte	-4
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-7
	.byte	-1
	.byte	-2
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
	.byte	-2
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
	.byte	-3
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-6
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
	.byte	-2
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
	.type	__FUNCTION__.13731, %object
	.size	__FUNCTION__.13731, 17
__FUNCTION__.13731:
	.ascii	"VP8_DecodeDecide\000"
	.data
	.align	2
.LANCHOR1 = . + 0
	.type	default_ymode_prob, %object
	.size	default_ymode_prob, 4
default_ymode_prob:
	.byte	112
	.byte	86
	.byte	-116
	.byte	37
	.type	default_uv_mode_prob, %object
	.size	default_uv_mode_prob, 3
default_uv_mode_prob:
	.byte	-94
	.byte	101
	.byte	-52
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC1:
	ASCII(.ascii	"pCurImg is null\012\000" )
	.space	3
.LC2:
	ASCII(.ascii	"err_level(%d) over ref_thr(%d)\012\000" )
.LC3:
	ASCII(.ascii	"line: %d pToQueImg is null\012\000" )
.LC4:
	ASCII(.ascii	"insert img to Voqueue failed!\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC6:
	ASCII(.ascii	"Error!partition 0 length > streamlength!\012\000" )
	.space	2
.LC7:
	ASCII(.ascii	"Invalid frame sync code,data[0-1]:%02x,%02x,%02x\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC8:
	ASCII(.ascii	"Invalid frame width %d and Height %d!\012\000" )
	.space	1
.LC9:
	ASCII(.ascii	"We have a new size image!\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"The old size is %d x %d,the new is %d x %d\012\000" )
.LC11:
	ASCII(.ascii	"image size abnormal(%dx%d)\012\000" )
.LC12:
	ASCII(.ascii	"vp8 actual frame size(%dx%d) exeed max config(%dx%d" )
	ASCII(.ascii	")\012\000" )
	.space	2
.LC13:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC14:
	ASCII(.ascii	"FSP_ConfigInstance fail!\012\000" )
	.space	2
.LC15:
	ASCII(.ascii	"ERROR: No Img buffer is allocated\012\000" )
	.space	1
.LC16:
	ASCII(.ascii	"----------------- vp8 Imode -> %d -----------------" )
	ASCII(.ascii	"-\012\000" )
	.space	2
.LC17:
	ASCII(.ascii	"-----I_MODE, but FrameType is %d-----\012\000" )
	.space	1
.LC18:
	ASCII(.ascii	"P-frm\000" )
	.space	2
.LC19:
	ASCII(.ascii	"-----cur framtype is %s, but refnum=%d-----\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"error: DecMode illegal\012\000" )
.LC21:
	ASCII(.ascii	"%s %d NULL == pFwdImg!!\012\000" )
	.space	3
.LC22:
	ASCII(.ascii	"dec P frame, fwd ref err(%d) > ref_thr(%d)\012\000" )
.LC23:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC26:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"VP8_Set_DecParam but logic fs is null!\012\000" )
.LC28:
	ASCII(.ascii	"VP8_Set_DecParam but cur logic pstDecodeFs is null!" )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"Error!Truncated packet\012\000" )
.LC30:
	ASCII(.ascii	"VP8_CB_GetPictureHeader Error!\012\000" )
.LC31:
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC32:
	ASCII(.ascii	"VP8_ArrangeVHBMem error\012\000" )
	.space	3
.LC33:
	ASCII(.ascii	"VP8_DecodeDecide err\012\000" )
	.space	2
.LC34:
	ASCII(.ascii	"VP8_GetBufFromQueue err\012\000" )
	.space	3
.LC35:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
