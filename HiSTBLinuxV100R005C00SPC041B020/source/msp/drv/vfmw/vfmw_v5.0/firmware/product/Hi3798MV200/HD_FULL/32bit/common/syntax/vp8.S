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
	add	r6, r0, #4096
	add	r3, r0, #8192
	mov	r7, r0
	ldr	r1, [r6, #4072]
	ldr	r0, [r3, #344]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L20
	ldr	r2, [r6, #4080]
	and	r2, r2, #3
	orr	r3, r2, #7168
	orr	r3, r3, #32
	str	r3, [r4, #108]
	ldr	r1, [r6, #1684]
	cmp	r1, #1920
	orrls	r3, r2, #138240
	orrls	r3, r3, #32
	strls	r3, [r4, #108]
	ldr	r1, [r7]
	ldr	r1, [r1, #592]
	cmp	r1, #1
	movne	r1, #0
	beq	.L30
.L23:
	mov	r5, #0
	orr	r3, r1, r3
	str	r2, [r4, #100]
	mov	r1, r5
	str	r3, [r4, #108]
	mov	r0, #13
	mov	r3, #8
	str	r2, [r4, #104]
	str	r0, [r4, #376]
	mov	r2, #1
	str	r3, [r4, #380]
	mov	r0, r4
	strb	r2, [r4, #371]
	strb	r2, [r4, #369]
	str	r3, [r4, #388]
	str	r3, [r4, #384]
	strb	r5, [r4, #364]
	bl	SetAspectRatio
	ldr	r2, [r6, #1684]
	str	r2, [r4, #112]
	ldr	r3, [r6, #1688]
	str	r3, [r4, #116]
	ldr	r1, [r7]
	ldr	r1, [r1, #28]
	cmp	r1, #25
	beq	.L31
.L24:
	str	r2, [r4, #120]
	mov	r2, r2, lsr #1
	str	r3, [r4, #124]
	mov	r3, r3, lsr #1
	str	r2, [r4, #128]
	str	r3, [r4, #132]
.L20:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L30:
	tst	r3, #131072
	movne	r1, #0
	moveq	r1, #262144
	b	.L23
.L31:
	ldr	r2, [r4, #140]
	add	r3, r3, #15
	ldr	ip, [r4, #44]
	add	r8, r4, #512
	mov	r3, r3, lsr #4
	add	r1, r4, #528
	mov	r0, r2, asl #4
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
	ldr	r1, [r4, #32]
	ldr	r2, [r4, #112]
	ldr	r3, [r4, #116]
	str	r1, [r4, #64]
	b	.L24
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
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
	ldr	r1, [r4, #4072]
	ldr	r0, [r6, #344]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L100
	cmp	r7, #0
	blt	.L35
	cmp	r7, #100
	movlt	r2, r7
	movge	r2, #100
	str	r2, [r3, #160]
	ldr	r3, [r5]
	ldr	r3, [r3]
	cmp	r2, r3
	bhi	.L101
.L36:
	mov	r0, r5
	bl	VP8_SetImgFormat
	ldr	r3, [r4, #4036]
	cmp	r3, #2
	beq	.L102
.L37:
	ldr	r3, [r5, #3284]
	cmp	r3, #0
	bne	.L39
.L38:
	mvn	r3, #0
	ldr	r1, [r4, #4072]
	str	r3, [r4, #4076]
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetDisplay
	ldr	r1, [r4, #4076]
.L40:
	cmn	r1, #1
	beq	.L41
	ldr	r0, [r6, #344]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L103
	mov	r2, #1
	ldr	r1, [r4, #4076]
	ldr	r0, [r6, #344]
	bl	FSP_SetDisplay
	add	r3, r5, #8128
	ldr	r0, [r6, #344]
	add	r3, r3, #56
	str	r8, [sp]
	mov	r2, r5
	mov	r1, #13
	bl	InsertImgToVoQueue
	cmp	r0, #1
	mov	r7, r0
	bne	.L104
.L41:
	ldr	r2, [r4, #4080]
	ldr	r3, [r4, #4040]
	cmp	r2, #0
	ldr	r8, [r4, #4068]
	add	r3, r3, #1
	str	r3, [r4, #4040]
	bne	.L105
	ldr	r3, [r4, #4060]
	cmp	r8, r3
	beq	.L47
.L45:
	ldr	r3, [r4, #4064]
	cmp	r8, r3
	beq	.L47
	ldr	r1, [r4, #4072]
	subs	r7, r8, r1
	movne	r7, #1
.L46:
	str	r1, [r4, #4068]
	mov	r2, #1
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L44:
	ldrb	r3, [r5, #3268]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L106
	cmp	r3, #2
	beq	.L107
.L50:
	ldrb	r3, [r5, #3267]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L108
	cmp	r3, #2
	beq	.L109
.L55:
	ldr	r3, [r4, #4080]
	cmp	r3, #0
	beq	.L58
	ldrb	r3, [r5, #3265]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L59
.L58:
	ldr	r3, [r4, #4060]
	ldr	r2, [r4, #4068]
	cmp	r3, r2
	beq	.L98
	ldr	r2, [r4, #4064]
	cmp	r3, r2
	beq	.L98
	ldr	r1, [r4, #4072]
	cmp	r3, r1
	beq	.L61
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L98:
	ldr	r1, [r4, #4072]
.L61:
	str	r1, [r4, #4060]
	mov	r2, #1
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L59:
	ldr	r3, [r5, #3280]
	cmp	r3, #0
	beq	.L63
	ldrb	r3, [r5, #3266]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L64
.L63:
	ldr	r3, [r4, #4064]
	ldr	r2, [r4, #4068]
	cmp	r3, r2
	beq	.L99
	ldr	r2, [r4, #4060]
	cmp	r3, r2
	beq	.L99
	ldr	r1, [r4, #4072]
	cmp	r3, r1
	beq	.L66
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L99:
	ldr	r1, [r4, #4072]
.L66:
	str	r1, [r4, #4064]
	mov	r2, #1
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L64:
	cmp	r7, #1
	movne	r0, #1
	beq	.L110
.L97:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L39:
	ldr	r1, [r4, #4072]
	str	r1, [r4, #4076]
	b	.L40
.L105:
	ldrb	r7, [r5, #3264]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L44
	ldr	r3, [r4, #4060]
	cmp	r8, r3
	bne	.L45
.L47:
	ldr	r1, [r4, #4072]
	mov	r7, #0
	b	.L46
.L35:
	mov	r2, #0
	str	r2, [r3, #160]
	b	.L36
.L102:
	ldr	r3, [r4, #4080]
	cmp	r3, #0
	bne	.L38
	b	.L37
.L108:
	ldr	r1, [r4, #4060]
	ldr	r3, [r4, #4068]
	cmp	r1, r3
	beq	.L54
	ldr	r3, [r4, #4064]
	cmp	r1, r3
	cmpne	r8, r1
	beq	.L54
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L54:
	str	r8, [r4, #4060]
	mov	r2, #1
	ldr	r0, [r6, #344]
	mov	r1, r8
	bl	FSP_SetRef
	mov	r7, #0
	b	.L55
.L106:
	ldr	r1, [r4, #4064]
	ldr	r3, [r4, #4068]
	cmp	r1, r3
	beq	.L49
	ldr	r3, [r4, #4060]
	cmp	r1, r3
	cmpne	r8, r1
	beq	.L49
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
.L49:
	str	r8, [r4, #4064]
	mov	r2, #1
	ldr	r0, [r6, #344]
	mov	r1, r8
	bl	FSP_SetRef
	mov	r7, #0
	b	.L50
.L104:
	add	r5, r8, #576
	add	r10, r8, #560
	ldr	r1, [r8, #560]
	mov	r9, #0
	ldr	r0, [r6, #344]
	mov	r8, #0
	bl	FreeUsdByDec
	strd	r8, [r10]
	ldr	r1, [r5, #-8]
	ldr	r0, [r6, #344]
	bl	FreeUsdByDec
	strd	r8, [r5, #-8]
	mov	r2, #0
	ldr	r1, [r4, #4076]
	ldr	r0, [r6, #344]
	bl	FSP_SetDisplay
	ldr	r1, .L111
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L109:
	ldr	r3, [r4, #4060]
	ldr	r2, [r4, #4068]
	ldr	r1, [r4, #4064]
	cmp	r3, r2
	beq	.L57
	cmp	r3, r1
	beq	.L57
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
	ldr	r1, [r4, #4064]
.L57:
	str	r1, [r4, #4060]
	mov	r2, #1
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
	b	.L55
.L107:
	ldr	r3, [r4, #4064]
	ldr	r2, [r4, #4068]
	ldr	r1, [r4, #4060]
	cmp	r3, r2
	beq	.L52
	cmp	r3, r1
	beq	.L52
	mov	r1, r3
	mov	r2, #0
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
	ldr	r1, [r4, #4060]
.L52:
	str	r1, [r4, #4064]
	mov	r2, #1
	ldr	r0, [r6, #344]
	bl	FSP_SetRef
	b	.L50
.L110:
	ldr	r0, [r6, #344]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetRef
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L101:
	ldr	r1, .L111+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r4, #4060]
	mov	r5, #0
	ldr	r0, [r6, #344]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #4064]
	ldr	r0, [r6, #344]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #4068]
	ldr	r0, [r6, #344]
	bl	FSP_ClearLogicFs
	str	r5, [r4, #4060]
	str	r5, [r4, #4064]
	str	r5, [r4, #4068]
	ldr	r0, [r6, #344]
	bl	FSP_GetNullLogicFs
	str	r5, [r4, #4040]
	mov	r3, r0
	mvn	r0, #0
	str	r3, [r4, #4068]
	str	r3, [r4, #4064]
	str	r3, [r4, #4060]
	b	.L97
.L103:
	mov	r2, #636
	ldr	r1, .L111+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L97
.L100:
	ldr	r1, .L111+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L97
.L112:
	.align	2
.L111:
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L117
	add	r5, r0, #8192
	movw	r2, #8552
	mov	r6, r1
	ldr	r7, [r5, #336]
	mov	r1, #0
	ldr	r3, [r3, #48]
	mov	r4, r0
	blx	r3
	add	r0, r4, #8128
	add	r0, r0, #56
	str	r7, [r5, #336]
	bl	ResetVoQueue
	str	r6, [r4]
	mov	r0, r4
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r5, #344]
	beq	.L116
	add	r3, r4, #4096
	mov	ip, #0
	mov	r2, #1024
	mov	r1, #3
	str	r2, [r3, #4032]
	mov	r0, #1
	str	ip, [r3, #4044]
	ldr	r2, [r4]
	ldr	lr, [r2, #8]
	str	ip, [r3, #4052]
	str	r1, [r3, #4084]
	str	lr, [r3, #4036]
	str	r1, [r3, #4048]
	ldr	r2, [r2, #40]
	str	r2, [r3, #4056]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L116:
	ldr	r1, .L117+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L118:
	.align	2
.L117:
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
	b	.L121
.L123:
	.word	.L121
	.word	.L124
	.word	.L125
	.word	.L126
.L121:
	mov	r3, #0
	str	r3, [r0, #76]
	str	r3, [r0, #80]
	str	r3, [r0, #84]
	str	r3, [r0, #88]
	mov	r3, #0
	str	r3, [r0, #92]
	ldmfd	sp, {fp, sp, pc}
.L125:
	mov	r2, #1
	mov	r3, #0
	str	r2, [r0, #76]
	str	r2, [r0, #84]
	str	r3, [r0, #80]
	str	r3, [r0, #88]
.L128:
	mov	r3, #1
	str	r3, [r0, #92]
	ldmfd	sp, {fp, sp, pc}
.L124:
	mov	r2, #0
	mov	r3, #1
	str	r2, [r0, #76]
	str	r2, [r0, #88]
	str	r3, [r0, #80]
	str	r3, [r0, #84]
	b	.L128
.L126:
	mov	r3, #1
	str	r3, [r0, #76]
	str	r3, [r0, #80]
	str	r3, [r0, #84]
	str	r3, [r0, #88]
	b	.L128
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
	ldr	r9, .L142
	mov	r6, r0
	mov	r5, r1
	mov	r8, #0
	str	r2, [fp, #-48]
.L134:
	ldr	r3, [fp, #-48]
	add	r10, r9, r8
	add	r4, r3, r8
	add	r8, r8, #19
	add	r7, r9, r8
	b	.L133
.L131:
	cmp	r10, r7
	add	r4, r4, #1
	beq	.L141
.L133:
	ldrb	r2, [r10], #1	@ zero_extendqisi2
	mov	r1, r5
	mov	r0, r6
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L131
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
	bne	.L133
.L141:
	cmp	r8, #38
	bne	.L134
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L143:
	.align	2
.L142:
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
	beq	.L146
.L150:
	mov	r2, #128
	mov	r1, r5
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #0
	beq	.L148
	add	r7, r6, #1312
	mov	r8, r7
	add	r7, r7, #1
	add	r8, r8, #4
.L151:
	mov	r2, #8
	mov	r1, r5
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeValue
	strb	r0, [r7, #1]!
	cmp	r7, r8
	bne	.L151
.L148:
	add	r2, r6, #2368
	mov	r1, r5
	add	r2, r2, #6
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	read_mvcontexts
.L146:
	add	r7, r6, #1296
	add	r8, r6, #1312
	add	r7, r7, #13
	add	r8, r8, #1
.L149:
	mov	r2, #8
	mov	r1, r5
	mov	r0, r4
	bl	Vp8_Vfmw_DecodeValue
	strb	r0, [r7, #1]!
	cmp	r7, r8
	bne	.L149
	b	.L150
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
	bne	.L158
.L155:
	cmp	r0, r5
	movne	r3, #1
	strne	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L158:
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
	b	.L155
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
	ldr	r3, .L252
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
	bhi	.L242
	ldr	r4, [fp, #-68]
	add	r3, r4, #2448
	add	r3, r3, #12
	str	r3, [fp, #-80]
	mov	r0, r3
	bl	VP8_setup_version
	ldr	r7, [r4, #2516]
	cmp	r7, #0
	beq	.L243
.L165:
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
	bne	.L170
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
.L170:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2474]
	cmp	r0, #0
	bne	.L171
.L178:
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
	bne	.L244
.L173:
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
	bne	.L245
.L182:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2505]
	cmp	r0, #0
	bne	.L193
	ldr	r0, [fp, #-80]
	movw	r2, #1149
	ldr	r3, .L252+4
	add	r1, r0, #1296
	add	r0, r0, #152
	add	r1, r1, #5
	ldr	r3, [r3, #52]
	blx	r3
.L193:
	ldr	r2, [fp, #-68]
	ldr	r3, [r2, #2516]
	cmp	r3, #0
	bne	.L194
	mov	r3, #1
	strb	r3, [r2, #2500]
.L195:
	mov	r3, #0
	str	r3, [fp, #-76]
.L206:
	ldr	r2, [fp, #-76]
	add	r10, r2, #3776
	add	r3, r2, #304
	add	r9, r2, #43
	ldr	r2, [fp, #-68]
	add	r10, r10, #2
	add	r3, r3, #3
	add	r2, r2, r10
	str	r2, [fp, #-64]
	ldr	r2, .L252+8
	add	r9, r2, r9
	add	r3, r2, r3
	str	r3, [fp, #-72]
.L202:
	ldr	r8, [fp, #-64]
	sub	r5, r9, #33
.L200:
	sub	r10, r5, #11
	mov	r4, r8
	b	.L197
.L196:
	cmp	r10, r5
	add	r4, r4, #1
	beq	.L246
.L197:
	ldrb	r2, [r10, #1]!	@ zero_extendqisi2
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	cmp	r0, #1
	bne	.L196
	mov	r2, #8
	mov	r1, r6
	mov	r0, r7
	add	r4, r4, #1
	bl	Vp8_Vfmw_DecodeValue
	cmp	r10, r5
	strb	r0, [r4, #-1]
	bne	.L197
.L246:
	add	r5, r10, #11
	add	r8, r8, #11
	cmp	r5, r9
	bne	.L200
	ldr	r3, [fp, #-64]
	add	r9, r10, #44
	add	r3, r3, #33
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-72]
	cmp	r9, r3
	bne	.L202
	ldr	r3, [fp, #-76]
	add	r3, r3, #264
	str	r3, [fp, #-76]
	cmp	r3, #1056
	bne	.L206
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	cmp	r0, #0
	str	r0, [r3, #2532]
	ldreq	r3, [fp, #-68]
	streqb	r0, [r3, #2513]
	beq	.L204
	mov	r2, #8
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	ldr	r3, [fp, #-68]
	strb	r0, [r3, #2513]
.L204:
	mov	r2, r6
	mov	r1, r7
	ldr	r0, [fp, #-80]
	bl	VP8_mb_mode_mv_init
	ldr	r3, [fp, #-68]
	ldr	r3, [r3, #2516]
	cmp	r3, #0
	beq	.L205
	ldr	r3, [fp, #-68]
	ldr	r3, [r3, #2500]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #0
	moveq	r0, #1
	beq	.L238
.L205:
	ldr	r2, [fp, #-68]
	mov	r0, #1
	ldrb	r3, [r2, #2463]	@ zero_extendqisi2
	strb	r3, [r2, #2464]
.L238:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L243:
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
	beq	.L166
	str	r0, [sp]
	mov	r0, #1
	ldr	r1, .L252+12
	bl	dprint_vfmw
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L242:
	ldr	r1, .L252+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L166:
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
	bne	.L247
	ldr	r8, [fp, #-68]
	ldr	r7, [r8, #116]
	ldr	r4, [r8, #112]
	cmp	r2, r4
	cmpeq	r3, r7
	beq	.L168
	ldr	r1, .L252+20
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r0, [r8, #2560]
	ldr	ip, [r8, #2556]
	mov	r3, r7
	mov	r2, r4
	ldr	r1, .L252+24
	str	r0, [sp, #4]
	mov	r0, #22
	str	ip, [sp]
	bl	dprint_vfmw
.L168:
	ldr	r3, [fp, #-68]
	ldr	r3, [r3, #2516]
	cmp	r3, #0
	bne	.L165
	ldr	r0, [fp, #-80]
	bl	VP8_init_frame.part.0
	b	.L165
.L194:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	strb	r0, [r3, #2500]
	b	.L195
.L245:
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
	bne	.L191
	mov	r2, #2
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	ldr	r2, [fp, #-68]
	ldrb	r3, [r2, #2502]	@ zero_extendqisi2
	strb	r0, [r2, #2503]
.L191:
	ldr	r2, [fp, #-68]
	cmp	r3, #0
	mov	r3, #0
	strb	r3, [r2, #2504]
	bne	.L192
	mov	r2, #2
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	ldr	r3, [fp, #-68]
	strb	r0, [r3, #2504]
.L192:
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
	b	.L182
.L244:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2461]
	cmp	r0, #0
	beq	.L173
	add	r4, r3, #2480
	add	r5, r3, #2496
	add	r4, r4, #12
	mov	r8, r3
.L186:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	uxtb	r0, r0
	strb	r0, [r8, #2490]
	cmp	r0, #1
	beq	.L248
.L184:
	add	r4, r4, #1
	cmp	r4, r5
	bne	.L186
	ldr	r3, [fp, #-68]
	add	r4, r3, #2496
	add	r4, r4, #4
.L190:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2491]
	cmp	r0, #1
	beq	.L249
.L188:
	add	r5, r5, #1
	cmp	r5, r4
	bne	.L190
	b	.L173
.L171:
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
	bne	.L250
.L174:
	ldr	r3, [fp, #-68]
	ldrb	r3, [r3, #2475]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L178
	ldr	r3, .L252+4
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
	b	.L180
.L179:
	add	r4, r4, #1
	cmp	r4, r5
	beq	.L178
.L180:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2481]
	cmp	r0, #1
	bne	.L179
	mov	r2, #8
	mov	r1, r6
	mov	r0, r7
	bl	Vp8_Vfmw_DecodeValue
	strb	r0, [r4]
	b	.L179
.L248:
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
	b	.L184
.L249:
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
	b	.L188
.L250:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	ldr	r9, [fp, #-68]
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, .L252+4
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
.L175:
	sub	r10, r4, #4
	str	r4, [fp, #-64]
	mov	r4, r10
	b	.L177
.L176:
	ldr	r3, [fp, #-64]
	strb	r10, [r4, #1]!
	cmp	r4, r3
	beq	.L251
.L177:
	mov	r2, #128
	mov	r1, r6
	mov	r0, r7
	mov	r10, #0
	bl	Vp8_Vfmw_DecodeBool
	ldr	r3, [fp, #-68]
	uxtb	r0, r0
	strb	r0, [r3, #2460]
	cmp	r0, #1
	bne	.L176
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
	b	.L176
.L251:
	add	r8, r8, #1
	add	r4, r3, #4
	cmp	r8, #2
	bne	.L175
	b	.L174
.L247:
	ldr	r1, .L252+28
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	b	.L238
.L253:
	.align	2
.L252:
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
	add	r6, r0, #4096
	mov	r5, r0
	ldr	r3, [r6, #1688]
	ldr	r2, [r6, #1684]
	cmp	r3, #31
	bls	.L256
	cmp	r2, #31
	bls	.L256
	ldr	r1, [r0]
	ldr	r0, [r1, #708]
	cmp	r0, #0
	ldr	r0, [r1, #28]
	bne	.L259
	cmp	r0, #25
	beq	.L283
.L260:
	mov	r4, #4
	mov	r7, r2
	mov	r8, r3
	mov	r9, r4
	mov	r10, #5
.L265:
	ldr	r3, .L286
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r2, [r5]
	str	r8, [fp, #-48]
	mov	r3, #0
	str	r7, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r8, [fp, #-64]
	cmp	r2, #24
	str	r7, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L271
	uxtb	r4, r4
	strb	r9, [fp, #-73]
	strb	r10, [fp, #-71]
.L276:
	add	r7, r5, #8192
	sub	r1, fp, #76
	strb	r4, [fp, #-70]
	mov	r4, #0
	ldr	r0, [r7, #344]
	strb	r4, [fp, #-74]
	bl	FSP_ConfigInstance
	cmp	r0, r4
	bne	.L272
	ldr	r1, [r5]
	sub	r3, fp, #80
	ldr	r0, [r7, #344]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L284
	cmp	r0, #0
	moveq	r0, #1
	streq	r0, [r6, #4044]
	beq	.L280
.L274:
	ldr	r1, .L286+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	b	.L280
.L256:
	ldr	r1, .L286+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
.L280:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L259:
	cmp	r0, #25
	bne	.L260
	ldr	r0, [r1, #668]
	ldr	ip, [r1, #672]
	cmp	r2, r0
	bhi	.L268
	cmp	r3, ip
	bhi	.L268
	ldr	r0, [r1, #696]
	mov	r7, r2
	ldr	r4, [r1, #692]
	mov	r8, r3
	cmp	r0, #0
	add	r9, r4, #2
	bne	.L285
.L278:
	mov	r4, #1
	b	.L270
.L272:
	ldr	r1, .L286+12
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L283:
	ldr	r7, [r1, #668]
	ldr	r8, [r1, #672]
	cmp	r2, r7
	bhi	.L262
	cmp	r3, r8
	bhi	.L262
	ldr	r0, [r6, #4044]
	cmp	r0, #1
	beq	.L280
	ldr	r2, [r1, #696]
	ldr	r3, [r1, #692]
	cmp	r2, #0
	add	r9, r3, #2
	beq	.L278
	add	r4, r3, #1
	cmp	r4, #17
	movge	r4, #17
.L270:
	ldrb	r10, [r1, #716]	@ zero_extendqisi2
	b	.L265
.L271:
	mov	r4, #1
	strb	r10, [fp, #-71]
	strb	r4, [fp, #-73]
	b	.L276
.L268:
	stmia	sp, {r0, ip}
	mov	r0, #0
	ldr	r1, .L286+16
	bl	dprint_vfmw
	mov	r0, #0
	b	.L280
.L285:
	add	r4, r4, #1
	cmp	r4, #17
	movge	r4, #17
	b	.L270
.L262:
	stmia	sp, {r7, r8}
	mov	r0, #0
	ldr	r1, .L286+16
	bl	dprint_vfmw
	mov	r0, #0
	b	.L280
.L284:
	add	r0, r5, #8128
	add	r0, r0, #56
	bl	ResetVoQueue
	b	.L274
.L287:
	.align	2
.L286:
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
	ldr	r5, [r4, #4044]
	cmp	r5, #0
	beq	.L314
	ldr	r3, [r4, #4036]
	sub	r3, r3, #2
	cmp	r3, #1
	ldr	r3, [r0]
	ldr	r2, [r3, #8]
	bls	.L315
.L292:
	str	r2, [r4, #4036]
	cmp	r2, #4
	ldrls	pc, [pc, r2, asl #2]
	b	.L297
.L298:
	.word	.L296
	.word	.L296
	.word	.L293
	.word	.L295
	.word	.L296
.L316:
	ldr	r1, .L318
	mov	r0, #18
	bl	dprint_vfmw
.L295:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L315:
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L292
	add	r5, r0, #8192
	ldr	r1, .L318+4
	mov	r0, #2
	mov	r7, #0
	bl	dprint_vfmw
	ldr	r0, [r5, #344]
	mov	r2, #1
	ldr	r1, [r4, #4060]
	bl	FSP_ClearLogicFs
	ldr	r0, [r5, #344]
	mov	r2, #1
	ldr	r1, [r4, #4064]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r0, [r5, #344]
	ldr	r1, [r4, #4068]
	bl	FSP_ClearLogicFs
	str	r7, [r4, #4060]
	str	r7, [r4, #4064]
	str	r7, [r4, #4068]
	ldr	r0, [r5, #344]
	bl	FSP_GetNullLogicFs
	str	r7, [r4, #4040]
	str	r0, [r4, #4068]
	str	r0, [r4, #4064]
	str	r0, [r4, #4060]
	ldr	r3, [r6]
	ldr	r2, [r3, #8]
	b	.L292
.L314:
	ldr	r1, .L318+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L293:
	ldr	r2, [r4, #4080]
	cmp	r2, #0
	bne	.L316
.L294:
	add	r3, r6, #8192
	ldr	r1, [r4, #4068]
	ldr	r0, [r3, #344]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L317
	ldr	r3, [r4, #4080]
	cmp	r3, #0
	beq	.L303
	ldr	r3, [r6]
	ldr	r2, [r5, #160]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L303
	ldr	r1, .L318+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L296:
	ldr	r3, [r4, #4080]
	cmp	r3, #0
	beq	.L294
	ldr	r3, [r4, #4040]
	cmp	r3, #0
	bne	.L294
	ldr	r2, .L318+16
	mov	r0, #18
	ldr	r1, .L318+20
	bl	dprint_vfmw
	b	.L295
.L297:
	ldr	r1, .L318+24
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L303:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L317:
	mov	r3, #1504
	ldr	r2, .L318+28
	ldr	r1, .L318+32
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L319:
	.align	2
.L318:
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
	ldr	r0, [r6, #344]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r5, #4072]
	blt	.L327
	mov	r1, r0
	ldr	r0, [r6, #344]
	bl	FSP_GetLogicFs
	subs	r4, r0, #0
	beq	.L328
	ldr	r2, [r5, #4072]
	mov	r0, #18
	ldr	r1, .L329
	bl	dprint_vfmw
	ldr	r2, [r4, #640]
	cmp	r2, #0
	beq	.L324
	ldr	r0, [r4, #644]
	cmp	r0, #0
	beq	.L322
	ldr	r1, [r4, #648]
	cmp	r1, #0
	moveq	r0, r1
	beq	.L322
	ldr	ip, [r1, #16]
	ldr	r3, [r0, #16]
	mov	r0, #18
	ldr	r2, [r2, #16]
	ldr	r1, .L329+4
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r0, #1
.L322:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L327:
	ldr	r1, .L329+8
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r5, #4060]
	ldr	r0, [r6, #344]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	ldr	r1, [r5, #4064]
	ldr	r0, [r6, #344]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r5, #4068]
	mov	r4, #0
	ldr	r0, [r6, #344]
	bl	FSP_ClearLogicFs
	str	r4, [r5, #4060]
	str	r4, [r5, #4064]
	str	r4, [r5, #4068]
	ldr	r0, [r6, #344]
	bl	FSP_GetNullLogicFs
	add	r1, r7, #8128
	str	r4, [r5, #4040]
	add	r1, r1, #56
	str	r0, [r5, #4068]
	str	r0, [r5, #4064]
	str	r0, [r5, #4060]
	ldr	r0, [r6, #344]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r4
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L328:
	movw	r2, #1560
	ldr	r1, .L329+12
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	b	.L322
.L324:
	mov	r0, r2
	b	.L322
.L330:
	.align	2
.L329:
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
	ldr	r9, .L372
	mov	r10, r0
	add	r3, r0, #8
	ldr	ip, [r10, #3180]
	add	r8, r0, #4096
	str	r3, [fp, #-112]
	movw	r2, #3160
	mov	r1, #0
	mov	r0, r3
	ldr	r3, [r9, #48]
	add	r5, r10, #8192
	str	ip, [fp, #-124]
	blx	r3
	ldr	r3, [r8, #1636]
	str	r3, [r10, #2804]
	ldr	r3, [r8, #4084]
	str	r3, [r10, #2808]
	ldr	r3, [r8, #1676]
	str	r3, [r10, #2792]
	ldr	r3, [r8, #1680]
	str	r3, [r10, #2796]
	ldr	r3, [r8, #1664]
	str	r3, [r10, #2812]
	ldr	r3, [r8, #1668]
	str	r3, [r10, #2816]
	ldrb	r3, [r8, #1633]	@ zero_extendqisi2
	strb	r3, [r10, #2760]
	ldr	r3, [r8, #1652]
	strb	r3, [r10, #2761]
	ldrb	r3, [r8, #1593]	@ zero_extendqisi2
	strb	r3, [r10, #2762]
	ldrb	r3, [r8, #1594]	@ zero_extendqisi2
	strb	r3, [r10, #2763]
	ldrb	r3, [r8, #1595]	@ zero_extendqisi2
	strb	r3, [r10, #2764]
	ldrb	r3, [r8, #1597]	@ zero_extendqisi2
	strb	r3, [r10, #2765]
	ldrb	r3, [r8, #1582]	@ zero_extendqisi2
	strb	r3, [r10, #2766]
	ldr	r3, [r8, #1716]
	strb	r3, [r10, #2768]
	ldrb	r3, [r8, #1590]	@ zero_extendqisi2
	strb	r3, [r10, #2770]
	ldrb	r3, [r8, #1591]	@ zero_extendqisi2
	strb	r3, [r10, #2771]
	ldr	r3, [r8, #4048]
	strb	r3, [r10, #2769]
	ldr	r3, [r8, #4052]
	strb	r3, [r10, #2772]
	ldrb	r3, [r8, #1583]	@ zero_extendqisi2
	strb	r3, [r10, #2767]
	ldrb	r3, [r8, #1585]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2773]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2774]
	ldrb	r3, [r8, #1586]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2775]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2776]
	ldrb	r3, [r8, #1587]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2777]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2778]
	ldrb	r3, [r8, #1588]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2779]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2780]
	ldrb	r3, [r8, #1589]	@ zero_extendqisi2
	sxtb	r2, r3
	mov	r3, r3, lsr #7
	cmp	r2, #0
	strb	r3, [r10, #2781]
	rsblt	r2, r2, #0
	strb	r2, [r10, #2782]
	ldrb	r3, [r8, #1630]	@ zero_extendqisi2
	strb	r3, [r10, #2783]
	ldrb	r3, [r8, #1631]	@ zero_extendqisi2
	strb	r3, [r10, #2784]
	ldrb	r3, [r8, #1632]	@ zero_extendqisi2
	strb	r3, [r10, #2785]
	ldr	r3, [r8, #1724]
	str	r3, [r10, #2788]
	ldr	r3, [r8, #1720]
	strb	r3, [r10, #2786]
	ldr	r3, [r8, #1728]
	strb	r3, [r10, #2787]
	ldr	r3, [r8, #4056]
	str	r3, [r10, #3144]
	ldr	r1, [r8, #4072]
	ldr	r0, [r5, #344]
	bl	FSP_GetLogicFs
	ldr	r1, [r8, #4060]
	mov	r4, r0
	ldr	r0, [r5, #344]
	bl	FSP_GetLogicFs
	ldr	r1, [r8, #4064]
	mov	r6, r0
	ldr	r0, [r5, #344]
	bl	FSP_GetLogicFs
	ldr	r1, [r8, #4068]
	mov	r7, r0
	ldr	r0, [r5, #344]
	bl	FSP_GetLogicFs
	cmp	r6, #0
	cmpne	r4, #0
	beq	.L332
	cmp	r0, #0
	cmpne	r7, #0
	beq	.L332
	ldr	r5, [r4, #640]
	cmp	r5, #0
	beq	.L371
	ldr	r3, [r5, #16]
	add	r5, r10, #5696
	mov	lr, r5
	mov	r1, r5
	str	r3, [r10, #3124]
	add	lr, lr, #11
	ldr	r3, [r4, #652]
	add	r1, r1, #15
	str	r3, [r10, #3140]
	ldr	r3, [r6, #640]
	cmp	r3, #0
	ldreq	r3, [r4, #640]
	ldr	r3, [r3, #16]
	str	r3, [r10, #3132]
	ldr	r3, [r7, #640]
	mov	r7, r5
	add	r7, r7, #25
	cmp	r3, #0
	ldreq	r3, [r4, #640]
	ldr	r3, [r3, #16]
	str	r3, [r10, #3128]
	ldr	r3, [r0, #640]
	mov	r0, r5
	add	r5, r5, #1
	add	r0, r0, #5
	cmp	r3, #0
	ldreq	r3, [r4, #640]
	ldr	r3, [r3, #16]
	str	r10, [fp, #-128]
	str	r3, [r10, #3136]
	ldr	r3, [r4, #644]
	ldr	r2, [r3, #32]
	mov	r3, r10
	str	r2, [r8, #4032]
	str	r2, [r10, #2800]
	mov	r2, #0
	ldr	r6, [r4, #644]
	ldr	r6, [r6, #76]
	str	r6, [r10, #3156]
	ldr	r6, [r4, #644]
	ldr	r6, [r6, #60]
	str	r6, [r10, #3160]
	ldr	r4, [r4, #644]
	ldr	r4, [r4, #64]
	str	r4, [r10, #3164]
.L342:
	ldrsb	ip, [r5, #1]!
	add	r2, r2, #1
	cmp	r2, #4
	add	r3, r3, #4
	str	ip, [r3, #3040]
	ldrsb	ip, [r0, #1]!
	str	ip, [r3, #3056]
	ldrsb	ip, [lr, #1]!
	str	ip, [r3, #3072]
	ldrsb	ip, [r1, #1]!
	str	ip, [r3, #3088]
	ldrb	ip, [r7, #1]!	@ zero_extendqisi2
	str	ip, [r3, #3104]
	bne	.L342
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	ldr	r0, [fp, #-112]
	blx	r3
	ldrb	r3, [r8, #1633]	@ zero_extendqisi2
	add	ip, r10, #6976
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #24
	strb	r3, [r10, #8]
	add	r3, ip, #18
	str	r3, [fp, #-120]
	mov	ip, #384
	ldrb	r3, [r8, #1598]	@ zero_extendqisi2
	str	ip, [fp, #-116]
	strb	r3, [r10, #9]
	ldrb	r3, [r8, #1599]	@ zero_extendqisi2
	strb	r3, [r10, #10]
	ldrb	r3, [r8, #1600]	@ zero_extendqisi2
	strb	r3, [r10, #11]
	ldrb	r3, [r8, #1630]	@ zero_extendqisi2
	strb	r3, [r10, #12]
	ldrb	r3, [r8, #1631]	@ zero_extendqisi2
	strb	r3, [r10, #13]
	ldrb	r3, [r8, #1632]	@ zero_extendqisi2
	strb	r3, [r10, #14]
	ldr	r3, [r9, #48]
	blx	r3
	ldrb	r3, [r8, #2890]	@ zero_extendqisi2
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #40
	strb	r3, [r10, #24]
	ldrb	r3, [r8, #2891]	@ zero_extendqisi2
	strb	r3, [r10, #25]
	ldrb	r3, [r8, #2892]	@ zero_extendqisi2
	strb	r3, [r10, #26]
	ldrb	r3, [r8, #2893]	@ zero_extendqisi2
	strb	r3, [r10, #27]
	ldrb	r3, [r8, #2894]	@ zero_extendqisi2
	strb	r3, [r10, #28]
	ldrb	r3, [r8, #2895]	@ zero_extendqisi2
	strb	r3, [r10, #29]
	ldrb	r3, [r8, #2896]	@ zero_extendqisi2
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
	ldrb	lr, [r8, #3955]	@ zero_extendqisi2
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #728
	strb	lr, [r10, #712]
	ldrb	lr, [r8, #3954]	@ zero_extendqisi2
	strb	lr, [r10, #713]
	ldrb	lr, [r8, #3956]	@ zero_extendqisi2
	strb	lr, [r10, #714]
	ldrb	lr, [r8, #3957]	@ zero_extendqisi2
	strb	lr, [r10, #715]
	ldrb	lr, [r8, #3958]	@ zero_extendqisi2
	strb	lr, [r10, #716]
	ldrb	lr, [r8, #3959]	@ zero_extendqisi2
	strb	lr, [r10, #717]
	ldrb	lr, [r8, #3960]	@ zero_extendqisi2
	strb	lr, [r10, #718]
	ldrb	lr, [r8, #3961]	@ zero_extendqisi2
	strb	lr, [r10, #719]
	ldrb	lr, [r8, #3962]	@ zero_extendqisi2
	strb	lr, [r10, #720]
	blx	r3
	ldrb	lr, [r8, #3955]	@ zero_extendqisi2
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #744
	strb	lr, [r10, #728]
	ldrb	lr, [r8, #3963]	@ zero_extendqisi2
	strb	lr, [r10, #729]
	ldrb	lr, [r8, #3964]	@ zero_extendqisi2
	strb	lr, [r10, #730]
	ldrb	lr, [r8, #3965]	@ zero_extendqisi2
	strb	lr, [r10, #731]
	ldrb	lr, [r8, #3966]	@ zero_extendqisi2
	strb	lr, [r10, #732]
	ldrb	lr, [r8, #3967]	@ zero_extendqisi2
	strb	lr, [r10, #733]
	ldrb	lr, [r8, #3968]	@ zero_extendqisi2
	strb	lr, [r10, #734]
	ldrb	lr, [r8, #3969]	@ zero_extendqisi2
	strb	lr, [r10, #735]
	ldrb	lr, [r8, #3970]	@ zero_extendqisi2
	strb	lr, [r10, #736]
	ldrb	lr, [r8, #3971]	@ zero_extendqisi2
	strb	lr, [r10, #737]
	ldrb	lr, [r8, #3972]	@ zero_extendqisi2
	strb	lr, [r10, #738]
	blx	r3
	ldrb	lr, [r8, #3974]	@ zero_extendqisi2
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r10, #760
	strb	lr, [r10, #744]
	ldrb	lr, [r8, #3973]	@ zero_extendqisi2
	strb	lr, [r10, #745]
	ldrb	lr, [r8, #3975]	@ zero_extendqisi2
	strb	lr, [r10, #746]
	ldrb	lr, [r8, #3976]	@ zero_extendqisi2
	strb	lr, [r10, #747]
	ldrb	lr, [r8, #3977]	@ zero_extendqisi2
	strb	lr, [r10, #748]
	ldrb	lr, [r8, #3978]	@ zero_extendqisi2
	strb	lr, [r10, #749]
	ldrb	lr, [r8, #3979]	@ zero_extendqisi2
	strb	lr, [r10, #750]
	ldrb	lr, [r8, #3980]	@ zero_extendqisi2
	strb	lr, [r10, #751]
	ldrb	lr, [r8, #3981]	@ zero_extendqisi2
	strb	lr, [r10, #752]
	blx	r3
	ldrb	r3, [r8, #3974]	@ zero_extendqisi2
	str	r10, [fp, #-132]
	str	r8, [fp, #-136]
	strb	r3, [r10, #760]
	ldrb	r3, [r8, #3982]	@ zero_extendqisi2
	strb	r3, [r10, #761]
	ldrb	r3, [r8, #3983]	@ zero_extendqisi2
	strb	r3, [r10, #762]
	ldrb	r3, [r8, #3984]	@ zero_extendqisi2
	strb	r3, [r10, #763]
	ldrb	r3, [r8, #3985]	@ zero_extendqisi2
	strb	r3, [r10, #764]
	ldrb	r3, [r8, #3986]	@ zero_extendqisi2
	strb	r3, [r10, #765]
	ldrb	r3, [r8, #3987]	@ zero_extendqisi2
	strb	r3, [r10, #766]
	ldrb	r3, [r8, #3988]	@ zero_extendqisi2
	strb	r3, [r10, #767]
	ldrb	r3, [r8, #3989]	@ zero_extendqisi2
	strb	r3, [r10, #768]
	ldrb	r3, [r8, #3990]	@ zero_extendqisi2
	strb	r3, [r10, #769]
	ldrb	r3, [r8, #3991]	@ zero_extendqisi2
	strb	r3, [r10, #770]
.L343:
	ldr	r3, [fp, #-116]
	ldr	r7, [fp, #-120]
	sub	r5, r3, #384
.L347:
	add	r6, r5, #768
	mov	r10, #0
	mov	r4, r7
	mov	r3, r6
	mov	r6, r10
	mov	r10, r3
.L344:
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
	bne	.L344
	ldr	r3, [fp, #-116]
	add	r5, r5, #48
	add	r7, r7, #33
	cmp	r3, r5
	bne	.L347
	ldr	r3, [fp, #-116]
	ldr	r2, [fp, #-120]
	add	r3, r3, #384
	str	r3, [fp, #-116]
	cmp	r3, #1920
	add	r2, r2, #264
	str	r2, [fp, #-120]
	bne	.L343
	ldr	r8, [fp, #-136]
	ldr	r6, [fp, #-124]
	ldr	r10, [fp, #-132]
	ldr	r2, [r8, #1636]
	ldr	r3, [r6, #4]
	add	r7, r10, #3184
	cmp	r2, #0
	bic	r3, r3, #15
	mov	r0, r7
	sub	r3, r3, #16
	str	r3, [r10, #2820]
	moveq	r4, #10
	movne	r4, #3
	ldr	r5, [r8, #1648]
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
	ldrb	r9, [r8, #1593]	@ zero_extendqisi2
	mov	r3, #1
	ldr	r1, [r8, #1648]
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
	ble	.L358
	rsb	r3, r9, #1
	sub	lr, fp, #76
	sub	r6, r9, #1
	mov	ip, #0
	add	r3, r3, r3, lsl #1
	mov	r5, lr
	rsb	r3, r1, r3
	add	r7, r3, r2
.L353:
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
	bne	.L353
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
.L357:
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
	bne	.L357
	ldr	r10, [fp, #-116]
.L358:
	ldr	r3, [r10]
	mov	r0, #1
	ldr	r2, [fp, #-112]
	str	r2, [r10, #4]
	ldr	r2, [r3, #592]
	str	r2, [r10, #3148]
	ldr	r3, [r3, #632]
	str	r3, [r10, #3152]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L332:
	ldr	r1, .L372+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
.L365:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L371:
	ldr	r1, .L372+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	b	.L365
.L373:
	.align	2
.L372:
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
	ldr	r6, .L378
	bne	.L375
	add	r0, r0, #3760
	movw	r2, #1149
	add	r0, r0, #1
	add	r1, r5, #152
	ldr	r3, [r6, #52]
	blx	r3
.L375:
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
.L379:
	.align	2
.L378:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VP8_Update_PictHeader, .-VP8_Update_PictHeader
	.align	2
	.global	VP8DEC_Decode
	.type	VP8DEC_Decode, %function
VP8DEC_Decode:
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
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	r3, [r1]
	mov	r4, r0
	str	r3, [r0, #3168]
	ldr	r2, [r1, #12]
	str	r2, [r0, #3172]
	ldr	r2, [r1, #16]
	cmp	r2, #2
	str	r2, [r0, #3176]
	ble	.L419
	add	r5, r0, #3184
	add	r8, r0, #3216
	mov	r1, r3
	mov	r0, r5
	bl	BsInit
	add	r3, r4, #3168
	mov	r0, r5
	mov	r1, r8
	ldr	r2, [r4, #3176]
	str	r3, [r4, #3180]
	bl	VP8_GetPictureHeader
	cmp	r0, #1
	bne	.L420
	add	r9, r4, #4096
	ldr	r3, [r9, #4044]
	cmp	r3, #0
	bne	.L421
.L384:
	ldr	r6, .L426
	ldr	ip, [r6]
	cmp	ip, #0
	addeq	r5, r4, #8192
	beq	.L386
	ldr	r0, [r9, #1680]
	add	r5, r4, #8192
	ldr	r10, [r4, #3320]
	mov	r3, #8
	ldr	r7, [r4, #3324]
	sub	r2, fp, #52
	ldr	lr, [r9, #1676]
	mov	r1, #2
	strh	r0, [fp, #-46]	@ movhi
	strh	r10, [fp, #-52]	@ movhi
	strh	r7, [fp, #-50]	@ movhi
	strh	lr, [fp, #-48]	@ movhi
	ldr	r0, [r5, #344]
	blx	ip
.L386:
	ldr	r0, [r5, #344]
	bl	FSP_GetFspType
	subs	ip, r0, #0
	beq	.L422
.L387:
	mov	r2, #1
	ldr	r1, [r9, #4060]
	ldr	r0, [r5, #344]
	mov	r6, #0
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r9, #4064]
	add	r7, r4, #8128
	ldr	r0, [r5, #344]
	add	r7, r7, #56
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r9, #4068]
	ldr	r0, [r5, #344]
	bl	FSP_ClearLogicFs
	str	r6, [r9, #4060]
	str	r6, [r9, #4064]
	str	r6, [r9, #4068]
	ldr	r0, [r5, #344]
	bl	FSP_GetNullLogicFs
	str	r6, [r9, #4040]
	mov	r1, r7
	str	r0, [r9, #4068]
	str	r0, [r9, #4064]
	str	r0, [r9, #4060]
	ldr	r0, [r5, #344]
	bl	FSP_ClearNotInVoQueue
	ldr	r2, [r9, #1676]
	ldr	r3, [r9, #1680]
	cmp	r2, #512
	bhi	.L394
	cmp	r3, #512
	bhi	.L394
	ldr	r3, [r4]
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L396
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L423
.L396:
	mov	r0, r4
	bl	VP8_ArrangeVHBMem
	cmp	r0, #1
	bne	.L424
.L385:
	mov	r0, r4
	bl	VP8_DecodeDecide
	cmp	r0, #1
	mov	r5, r0
	beq	.L397
	ldr	r1, .L426+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	b	.L382
.L420:
	ldr	r1, .L426+8
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, #0
.L382:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L419:
	ldr	r1, .L426+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L421:
	ldr	r2, [r4, #3320]
	ldr	r3, [r9, #1676]
	cmp	r2, r3
	bne	.L384
	ldr	r2, [r4, #3324]
	ldr	r3, [r9, #1680]
	cmp	r2, r3
	bne	.L384
	b	.L385
.L422:
	sub	r3, fp, #44
	sub	r2, fp, #56
	sub	r1, fp, #60
	ldr	r0, [r5, #344]
	str	ip, [r3, #-8]!
	str	ip, [fp, #-60]
	str	ip, [fp, #-56]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r4]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	bne	.L387
	ldr	r3, [fp, #-52]
	cmp	r3, #0
	bgt	.L390
	ldr	r3, [fp, #-56]
	cmp	r3, #32
	bgt	.L390
	ldr	r7, [r4, #4]
	cmp	r7, #0
	bne	.L390
	ldr	r3, [r4, #3320]
	cmp	r3, #0
	beq	.L387
	ldr	r1, [r9, #1680]
	ldr	r0, [r5, #348]
	cmp	r1, r0
	beq	.L387
	ldr	r0, [r9, #1676]
	ldr	ip, [r5, #352]
	cmp	r0, ip
	beq	.L387
	ldr	ip, [r4, #3324]
	stmib	sp, {r0, r1}
	mov	r0, r2
	ldr	r1, .L426+16
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r9, #1680]
	ldr	ip, [r6]
	str	r3, [r5, #348]
	cmp	ip, #0
	ldr	r3, [r9, #1676]
	str	r3, [r5, #352]
	beq	.L387
	mov	r3, r7
	mov	r2, r7
	mov	r1, #120
	ldr	r0, [r5, #344]
	blx	ip
	b	.L387
.L394:
	ldr	r1, .L426+20
	mov	r0, #18
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L424:
	ldr	r1, .L426+24
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	b	.L382
.L397:
	mov	r0, r4
	bl	VP8_GetImageBuffer
	cmp	r0, #1
	beq	.L398
	mov	r0, r5
	ldr	r1, .L426+28
	bl	dprint_vfmw
	mov	r0, #0
	b	.L382
.L423:
	ldr	r3, [r9, #4044]
	cmp	r3, #0
	bne	.L396
	mov	r0, r7
	bl	ResetVoQueue
	b	.L396
.L398:
	add	r5, r4, #8192
	ldr	r1, [r9, #4072]
	ldr	r0, [r5, #344]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L425
	ldr	r0, [r4]
	add	ip, r6, #608
	add	r7, r6, #624
	mov	lr, #0
	ldr	r1, .L426+32
	ldrd	r2, [r0, #56]
	mov	r0, #29
	strd	r2, [ip, #-8]
	ldr	r10, [r4]
	ldrd	r2, [r10, #72]
	strd	r2, [ip]
	ldr	r10, [r4]
	ldrd	r2, [r10, #88]
	strd	r2, [r7, #-8]
	mvn	r2, #0
	ldr	r7, [r4]
	mvn	r3, #0
	ldr	r7, [r7, #104]
	str	r7, [r6, #8]
	ldr	r7, [r4]
	ldr	r7, [r7, #108]
	str	r7, [r6, #12]
	ldr	r7, [r4]
	ldr	r7, [r7, #112]
	str	r7, [r6, #16]
	ldr	r7, [r4]
	ldr	r7, [r7, #128]
	str	lr, [r6, #96]
	str	r7, [r6, #20]
	ldr	lr, [r4]
	strd	r2, [lr, #56]
	ldrd	r6, [ip]
	ldrd	r2, [ip, #-8]
	strd	r6, [sp]
	bl	dprint_vfmw
	ldr	r2, [r9, #4080]
	cmp	r2, #0
	bne	.L400
	ldr	r1, [r9, #4064]
	ldr	r3, [r9, #4072]
	cmp	r1, r3
	beq	.L401
	ldr	r0, [r5, #344]
	bl	FSP_SetRef
	ldr	r3, [r9, #4072]
.L401:
	ldr	r1, [r9, #4068]
	cmp	r1, r3
	beq	.L402
	mov	r2, #0
	ldr	r0, [r5, #344]
	bl	FSP_SetRef
	ldr	r3, [r9, #4072]
.L402:
	ldr	r1, [r9, #4060]
	cmp	r1, r3
	beq	.L403
	ldr	r0, [r5, #344]
	mov	r2, #0
	bl	FSP_SetRef
.L403:
	mov	r3, #0
	str	r3, [r9, #4040]
.L400:
	mov	r0, r4
	bl	VP8_Set_DecParam
	mov	r0, r8
	bl	VP8_Update_PictHeader
	mov	r0, #0
	b	.L382
.L390:
	mvn	r0, #1
	b	.L382
.L425:
	movw	r2, #2044
	ldr	r1, .L426+36
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	b	.L382
.L427:
	.align	2
.L426:
	.word	g_event_report
	.word	.LC34
	.word	.LC30
	.word	.LC29
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC35
	.word	.LC36
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r5, r1
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r4, r0, #8192
	mov	r2, #0
	ldr	r0, [r4, #344]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #344]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L431
	add	r5, r0, #576
	add	r8, r0, #560
	ldr	r1, [r0, #560]
	mov	r6, #0
	ldr	r0, [r4, #344]
	mov	r7, #0
	bl	FreeUsdByDec
	strd	r6, [r8]
	ldr	r0, [r4, #344]
	ldr	r1, [r5, #-8]
	bl	FreeUsdByDec
	strd	r6, [r5, #-8]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L431:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
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
	add	r0, r0, #56
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
	ldr	r0, [r4, #344]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r0, [r4, #344]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L439
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L439:
	add	r1, r5, #8128
	ldr	r0, [r4, #344]
	add	r1, r1, #56
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
	.type	__FUNCTION__.14425, %object
	.size	__FUNCTION__.14425, 17
__FUNCTION__.14425:
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
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC32:
	ASCII(.ascii	"pic width/height to large,MbWidth = %d, MbHeight = " )
	ASCII(.ascii	"%d\000" )
	.space	2
.LC33:
	ASCII(.ascii	"VP8_ArrangeVHBMem error\012\000" )
	.space	3
.LC34:
	ASCII(.ascii	"VP8_DecodeDecide err\012\000" )
	.space	2
.LC35:
	ASCII(.ascii	"VP8_GetBufFromQueue err\012\000" )
	.space	3
.LC36:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
