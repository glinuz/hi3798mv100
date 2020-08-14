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
	.file	"vc1.c"
	.text
	.align	2
	.type	VC1_Vfmw_GenerateIntensityMap, %function
VC1_Vfmw_GenerateIntensityMap:
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
	cmp	r2, #31
	subgt	r2, r2, #64
	cmp	r1, #0
	addne	ip, r1, #32
	moveq	r2, r2, asl #7
	mvneq	ip, #63
	rsbeq	r2, r2, #16320
	movne	r2, r2, asl #6
	cmp	r3, #0
	bne	.L13
	add	r3, r2, #32
	sub	r2, r0, #1
	add	r0, r0, #255
.L5:
	usat	r1, #8, r3, asr #6
	strb	r1, [r2, #1]!
	cmp	r2, r0
	add	r3, r3, ip
	bne	.L5
	ldmfd	sp, {fp, sp, pc}
.L13:
	mov	r3, ip, asl #7
	sub	r2, r0, #1
	rsb	r3, r3, #8192
	add	r0, r0, #255
	add	r3, r3, #32
.L6:
	usat	r1, #8, r3, asr #6
	strb	r1, [r2, #1]!
	cmp	r2, r0
	add	r3, r3, ip
	bne	.L6
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_Vfmw_GenerateIntensityMap, .-VC1_Vfmw_GenerateIntensityMap
	.align	2
	.type	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5, %function
VC1_Vfmw_UpdataLastPicRNDCTRL.part.5:
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
	add	r2, r0, #8192
	ldrb	r3, [r2, #2415]	@ zero_extendqisi2
	tst	r3, #251
	moveq	r3, #1
	moveq	r1, r3
	beq	.L15
	cmp	r3, #1
	ldreqsb	r3, [r0, #10]
	ldrneb	r1, [r0, #10]	@ zero_extendqisi2
	clzeq	r3, r3
	moveq	r3, r3, lsr #5
	uxtbne	r3, r1
	uxtbeq	r1, r3
.L15:
	strb	r1, [r0, #10]
	strb	r3, [r2, #2434]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5, .-VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
	.align	2
	.global	emp_print
	.type	emp_print, %function
emp_print:
	UNWIND(.fnstart)
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r1, r2, r3}
	UNWIND(.save {r1, r2, r3})
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-16)
	sub	fp, ip, #16
	mov	r0, #0
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	emp_print, .-emp_print
	.align	2
	.global	AbandonBitsLenthNoUse
	.type	AbandonBitsLenthNoUse, %function
AbandonBitsLenthNoUse:
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
	ldrb	r3, [r0]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r2, r3
	bne	.L31
.L23:
	ldrb	r3, [r0, #-1]!	@ zero_extendqisi2
	add	r2, r2, #8
	cmp	r3, #0
	beq	.L23
	add	r0, r2, #1
.L24:
	tst	r3, #1
	bne	.L32
.L25:
	mov	r3, r3, lsr #1
	add	r2, r2, #1
	tst	r3, #1
	beq	.L25
	add	r0, r2, #1
	ldmfd	sp, {fp, sp, pc}
.L31:
	mov	r0, #1
	mov	r2, #0
	b	.L24
.L32:
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	AbandonBitsLenthNoUse, .-AbandonBitsLenthNoUse
	.align	2
	.global	VC1DEC_Init
	.type	VC1DEC_Init, %function
VC1DEC_Init:
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
	ldr	r8, .L37
	add	r4, r0, #45056
	mov	r9, r1
	movw	r2, #48840
	ldr	r10, [r4, #3768]
	mov	r1, #0
	ldr	r3, [r8, #48]
	mov	r7, r0
	mov	r6, r1
	blx	r3
	add	r0, r7, #48384
	str	r10, [r4, #3768]
	add	r0, r0, #96
	mov	r5, #1
	bl	ResetVoQueue
	add	r2, r7, #12288
	ldr	r3, [r9, #32]
	mov	r0, r7
	str	r9, [r7]
	strb	r5, [r2, #433]
	add	r3, r3, #255
	ldr	r1, [r9, #8]
	bic	r3, r3, #255
	add	ip, r3, #2048
	str	r3, [r4, #3352]
	str	ip, [r4, #3356]
	add	ip, r3, #4096
	str	r1, [r4, #3380]
	add	r1, r3, #6144
	str	ip, [r4, #3360]
	add	ip, r3, #8192
	str	r1, [r4, #3364]
	add	r1, r3, #10240
	str	ip, [r4, #3368]
	add	r3, r3, #12288
	str	r1, [r4, #3372]
	str	r3, [r4, #3376]
	strb	r5, [r2, #434]
	str	r6, [r4, #3312]
	str	r6, [r4, #3316]
	bl	VCTRL_GetChanIDByCtx
	ldr	r3, [r8, #68]
	cmn	r0, #1
	str	r0, [r4, #3776]
	beq	.L36
	ldr	r1, .L37+4
	mov	r0, #22
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L36:
	mov	r0, r6
	ldr	r1, .L37+8
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L38:
	.align	2
.L37:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC14
	.word	.LC13
	UNWIND(.fnend)
	.size	VC1DEC_Init, .-VC1DEC_Init
	.align	2
	.global	Vc1SMPSeqHdr
	.type	Vc1SMPSeqHdr, %function
Vc1SMPSeqHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	mov	r4, r2
	mov	r2, r1, asl #3
	mov	r1, r0
	sub	r0, fp, #60
	bl	BsInit
	mov	r1, #8
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	mov	r5, r0
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #8
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #16
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #24
	sub	r0, fp, #60
	str	r5, [r4, #24]
	bl	BsGet
	mov	r1, #8
	mov	r5, r0
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #8
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #16
	sub	r0, fp, #60
	bl	BsGet
	ldr	r3, [r4, #24]
	cmp	r3, #8192
	add	r0, r5, r0, lsl #24
	str	r0, [r4, #28]
	bgt	.L55
	cmp	r0, #8192
	bgt	.L54
	cmp	r3, #31
	ble	.L55
	cmp	r0, #31
	ble	.L54
	mov	r1, #2
	sub	r0, fp, #60
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3]
	cmp	r0, #1
	bhi	.L56
	mov	r1, #2
	sub	r0, fp, #60
	bl	BsGet
	subs	r5, r0, #0
	bne	.L56
	mov	r1, #3
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #5
	strb	r0, [r4, #4]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #5]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #6]
	sub	r0, fp, #60
	bl	BsGet
	subs	r6, r0, #0
	bne	.L57
	mov	r1, #1
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #7]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #8]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #9]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #10]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #11]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #12]
	sub	r0, fp, #60
	bl	BsGet
	subs	r5, r0, #0
	bne	.L58
	mov	r1, #1
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #13]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #14]
	sub	r0, fp, #60
	bl	BsGet
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	mov	r1, #3
	cmp	r3, #0
	strb	r0, [r4, #15]
	sub	r0, fp, #60
	streqb	r5, [r4, #15]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #16]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #17]
	sub	r0, fp, #60
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #18]
	sub	r0, fp, #60
	bl	BsGet
	sub	r0, fp, #60
	mov	r1, #32
	bl	BsGet
	cmp	r0, #0
	bne	.L59
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L59:
	ldr	r3, .L60
	mov	r0, #0
	ldr	r1, .L60+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L56:
	ldr	r1, .L60
	mov	r0, #0
	ldr	r3, .L60+8
.L53:
	ldr	ip, [r1, #68]
	mvn	r5, #0
	ldr	r2, .L60+12
	ldr	r1, .L60+16
	blx	ip
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L54:
	ldr	r1, .L60
	mov	r0, #0
	ldr	r3, .L60+20
	b	.L53
.L55:
	ldr	r1, .L60
	mov	r0, #0
	ldr	r3, .L60+24
	b	.L53
.L57:
	ldr	r1, .L60
	mov	r0, r5
	ldr	r3, .L60+8
	b	.L53
.L58:
	ldr	r1, .L60
	mov	r0, r6
	ldr	r3, .L60+8
	b	.L53
.L61:
	.align	2
.L60:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC19
	.word	.LC18
	.word	.LANCHOR0
	.word	.LC16
	.word	.LC17
	.word	.LC15
	UNWIND(.fnend)
	.size	Vc1SMPSeqHdr, .-Vc1SMPSeqHdr
	.align	2
	.global	VC1_SetImgFormat
	.type	VC1_SetImgFormat, %function
VC1_SetImgFormat:
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
	add	r3, r0, #45056
	mov	r6, r0
	add	r7, r0, #12288
	ldr	r1, [r5, #3252]
	ldr	r0, [r3, #3776]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r3, #1
	str	r3, [r4, #384]
	ldrb	r3, [r7, #427]	@ zero_extendqisi2
	cmp	r3, #3
	movne	r2, #0
	movne	r3, #7168
	strneb	r2, [r4, #372]
	beq	.L72
.L71:
	str	r3, [r4, #160]
	mov	r1, #0
	ldr	r3, [r5, #3196]
	mov	r0, r4
	str	r3, [r4, #168]
	ldr	r3, [r5, #3192]
	str	r3, [r4, #164]
	bl	SetAspectRatio
	ldr	r2, [r7, #228]
	ldr	r3, [r4, #104]
	ldr	ip, [r4, #32]
	ldr	r1, [r4, #192]
	add	r0, r3, r2
	str	r3, [r4, #88]
	add	r2, ip, r2
	str	r3, [r4, #120]
	add	r3, r3, r1
	str	r2, [r4, #36]
	add	r1, r0, r1
	str	r3, [r4, #96]
	str	r1, [r4, #100]
	str	r0, [r4, #108]
	str	r0, [r4, #92]
	str	ip, [r4, #48]
	bl	MEM_Phy2Vir
	ldr	r1, [r4, #92]
	mov	r3, #8
	mov	r2, #1
	str	r1, [r4, #124]
	str	r0, [r4, #52]
	ldr	r1, [r5, #3196]
	str	r1, [r4, #176]
	ldr	r1, [r5, #3192]
	strb	r2, [r4, #379]
	strb	r2, [r4, #377]
	str	r1, [r4, #172]
	str	r3, [r4, #388]
	str	r3, [r4, #396]
	str	r3, [r4, #392]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L72:
	mov	r3, #4
	strb	r3, [r4, #372]
	ldr	r2, [r6, #4]
	ldr	r3, [r4, #160]
	ldrb	r1, [r2, #8]	@ zero_extendqisi2
	ubfx	r2, r3, #10, #2
	add	r3, r1, #1
	orr	r3, r3, r2
	cmp	r3, #3
	movls	r3, r3, asl #10
	orrls	r3, r3, #4352
	movhi	r3, #4352
	b	.L71
	UNWIND(.fnend)
	.size	VC1_SetImgFormat, .-VC1_SetImgFormat
	.align	2
	.global	VC1DEC_Destroy
	.type	VC1DEC_Destroy, %function
VC1DEC_Destroy:
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
	.size	VC1DEC_Destroy, .-VC1DEC_Destroy
	.align	2
	.global	VC1DEC_RecycleImage
	.type	VC1DEC_RecycleImage, %function
VC1DEC_RecycleImage:
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
	add	r4, r0, #45056
	mov	r2, #0
	ldr	r0, [r4, #3776]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #3776]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L77
	ldr	r1, [r5, #212]
	mov	r6, #0
	ldr	r0, [r4, #3776]
	bl	FreeUsdByDec
	str	r6, [r5, #212]
	ldr	r1, [r5, #216]
	ldr	r0, [r4, #3776]
	bl	FreeUsdByDec
	str	r6, [r5, #216]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L77:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1DEC_RecycleImage, .-VC1DEC_RecycleImage
	.align	2
	.global	VC1DEC_GetRemainImg
	.type	VC1DEC_GetRemainImg, %function
VC1DEC_GetRemainImg:
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
	add	r5, r0, #32768
	mov	r4, r0
	add	r7, r0, #12288
	ldr	r3, [r5, #3164]
	cmp	r3, #0
	beq	.L85
	ldr	r3, [r0]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L96
	add	r6, r0, #45056
	ldr	r1, [r5, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L96
	ldr	r1, [r5, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L94
.L96:
	add	r5, r4, #48384
	add	r5, r5, #96
.L84:
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	GetVoLastImageID
.L94:
	mov	r0, r4
	mov	r9, #0
	bl	VC1_SetImgFormat
	mov	r3, #1
	mov	r2, r3
	str	r3, [r8, #244]
	str	r9, [r8, #212]
	str	r9, [r8, #216]
	ldr	r1, [r8, #196]
	ldr	r0, [r6, #3776]
	bl	FSP_SetDisplay
	ldr	r3, [r5, #3164]
	mov	r1, #1
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	streqb	r2, [r3, #1]
	ldreq	r2, [r5, #3164]
	add	r5, r4, #48384
	add	r5, r5, #96
	moveq	r3, #3
	streqb	r9, [r2]
	mov	r2, r4
	streqb	r3, [r7, #425]
	mov	r3, r5
	ldr	r0, [r6, #3776]
	str	r8, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L82
	mov	r0, r4
	ldr	r1, [r8, #196]
	bl	VC1DEC_RecycleImage
	ldr	r1, [r8, #212]
	ldr	r0, [r6, #3776]
	mov	r4, #0
	bl	FreeUsdByDec
	str	r4, [r8, #212]
	ldr	r1, [r8, #216]
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	str	r4, [r8, #216]
	b	.L84
.L82:
	ldr	r3, [r8, #200]
	cmp	r3, #100
	bhi	.L84
	ldr	r3, [r8, #164]
	cmp	r3, #31
	bls	.L84
	ldr	r3, [r8, #168]
	cmp	r3, #31
	movhi	r0, #0
	bls	.L84
.L95:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L85:
	mvn	r0, #0
	b	.L95
	UNWIND(.fnend)
	.size	VC1DEC_GetRemainImg, .-VC1DEC_GetRemainImg
	.align	2
	.global	VC1_ArrangeVahbMem
	.type	VC1_ArrangeVahbMem, %function
VC1_ArrangeVahbMem:
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
	sub	r1, r1, #32
	movw	r3, #8160
	cmp	r1, r3
	mov	r7, r0
	mov	r6, r2
	bhi	.L98
	sub	r2, r2, #32
	cmp	r2, r3
	bhi	.L98
	add	ip, r0, #28160
	mov	lr, #0
	add	ip, ip, #48
	mov	r4, lr
.L101:
	str	lr, [ip, #-316]
	add	lr, lr, #1
	cmp	lr, #5
	strb	r4, [ip, #-1543]
	strb	r4, [ip, #-1544]
	add	ip, ip, #1536
	add	ip, ip, #8
	bne	.L101
	ldr	r3, [r7]
	ldr	r4, [r3, #776]
	ldr	r2, [r3, #28]
	cmp	r4, #0
	bne	.L102
	cmp	r2, #25
	beq	.L124
.L120:
	mov	r4, lr
	mov	r10, #3
	mov	r8, #2
.L103:
	ldr	r9, .L127
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r9, #48]
	blx	r3
	ldr	r2, [r7]
	str	r6, [fp, #-48]
	mov	r3, #0
	str	r5, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r6, [fp, #-64]
	cmp	r2, #24
	str	r5, [fp, #-52]
	strb	r3, [fp, #-76]
	strb	r3, [fp, #-75]
	beq	.L113
	uxtb	r8, r8
	strb	r10, [fp, #-72]
	strb	r4, [fp, #-70]
.L116:
	add	r6, r7, #45056
	sub	r1, fp, #76
	strb	r8, [fp, #-69]
	mov	r5, #0
	ldr	r0, [r6, #3776]
	strb	r5, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r4, r0, #0
	bne	.L114
	ldr	r1, [r7]
	sub	r3, fp, #80
	ldr	r0, [r6, #3776]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_PartitionFsMemory
	cmp	r0, r5
	bne	.L125
.L118:
	mov	r4, #1
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L98:
	ldr	ip, .L127
	mov	r0, #0
	mov	r3, r6
	mov	r2, r5
	ldr	r1, .L127+4
	mov	r4, r0
	ldr	ip, [ip, #68]
	blx	ip
.L123:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L102:
	cmp	r2, #25
	bne	.L120
	ldr	r1, [r3, #736]
	ldr	r2, [r3, #740]
	cmp	r5, r1
	bhi	.L110
	cmp	r6, r2
	bhi	.L110
	ldr	r1, [r3, #764]
	ldr	r2, [r3, #760]
	cmp	r1, #0
	add	r10, r2, #2
	moveq	r8, #1
	bne	.L126
.L112:
	ldrb	r4, [r3, #784]	@ zero_extendqisi2
	b	.L103
.L114:
	ldr	r3, [r9, #68]
	mov	r0, r5
	ldr	r1, .L127+8
	mov	r4, r5
	blx	r3
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L124:
	ldr	r1, [r3, #736]
	cmp	r5, r1
	ldrhi	r0, [r3, #740]
	bhi	.L105
	ldr	r2, [r3, #740]
	cmp	r6, r2
	mov	r0, r2
	bhi	.L105
	add	r0, r7, #12288
	ldrb	r0, [r0, #424]	@ zero_extendqisi2
	tst	r0, #8
	bne	.L118
	ldr	ip, [r3, #764]
	ldr	r0, [r3, #760]
	cmp	ip, #0
	add	r10, r0, #2
	moveq	r8, #1
	beq	.L108
	add	r8, r0, #1
	cmp	r8, #17
	movcs	r8, #17
.L108:
	ldrb	r4, [r3, #784]	@ zero_extendqisi2
	mov	r6, r2
	mov	r5, r1
	b	.L103
.L113:
	strb	r3, [fp, #-72]
	mov	r8, #1
	strb	r4, [fp, #-70]
	b	.L116
.L110:
	ldr	ip, .L127
	mov	r3, r6
	str	r2, [sp, #4]
	mov	r0, #0
	str	r1, [sp]
	mov	r2, r5
	ldr	ip, [ip, #68]
	mov	r4, r0
	ldr	r1, .L127+12
	blx	ip
	b	.L123
.L125:
	ldr	r3, [r9, #68]
	mov	r0, r4
	ldr	r1, .L127+16
	blx	r3
	b	.L123
.L126:
	add	r8, r2, #1
	cmp	r8, #17
	movcs	r8, #17
	b	.L112
.L105:
	ldr	ip, .L127
	mov	r3, r6
	str	r0, [sp, #4]
	mov	r2, r5
	str	r1, [sp]
	mov	r0, #0
	ldr	ip, [ip, #68]
	ldr	r1, .L127+12
	blx	ip
	b	.L123
.L128:
	.align	2
.L127:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC20
	.word	.LC23
	.word	.LC21
	.word	.LC22
	UNWIND(.fnend)
	.size	VC1_ArrangeVahbMem, .-VC1_ArrangeVahbMem
	.align	2
	.global	VC1_WritePicMsg
	.type	VC1_WritePicMsg, %function
VC1_WritePicMsg:
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
	add	r7, r0, #32768
	add	r5, r0, #45056
	mov	r6, r0
	mov	r4, r1
	ldr	r0, [r5, #3776]
	ldr	r1, [r7, #3252]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L152
	add	r3, r6, #15808
	ldrsb	r1, [r3]
	add	r3, r6, #20480
	cmp	r1, #0
	bne	.L153
	strb	r1, [r4, #5]
	add	r2, r6, #12288
	ldrb	ip, [r3, #2588]	@ zero_extendqisi2
	strb	ip, [r4, #4]
	ldrb	ip, [r3, #2839]	@ zero_extendqisi2
	cmp	ip, #4
	moveq	ip, #3
	strb	ip, [r4, #3]
	ldr	ip, [r5, #3332]
	sub	ip, ip, #1
	strh	ip, [r4, #80]	@ movhi
	ldr	ip, [r5, #3328]
	strb	r1, [r4, #8]
	sub	ip, ip, #1
	strh	ip, [r4, #78]	@ movhi
	ldrb	ip, [r3, #2591]	@ zero_extendqisi2
	strb	r1, [r4, #9]
	strb	ip, [r4, #6]
	ldrb	ip, [r2, #432]	@ zero_extendqisi2
	strb	r1, [r4, #10]
	strb	r1, [r4, #11]
	strb	ip, [r4, #7]
	ldrb	ip, [r3, #2594]	@ zero_extendqisi2
	strb	ip, [r4, #13]
	ldrb	ip, [r3, #2598]	@ zero_extendqisi2
	strb	ip, [r4, #14]
	ldrb	ip, [r3, #2858]	@ zero_extendqisi2
	strb	ip, [r4, #12]
	ldrb	ip, [r3, #2868]	@ zero_extendqisi2
	strb	ip, [r4, #18]
	ldrb	ip, [r3, #2843]	@ zero_extendqisi2
	strb	ip, [r4, #19]
	ldrb	ip, [r3, #2842]	@ zero_extendqisi2
	strb	ip, [r4, #17]
	ldr	ip, [r3, #2928]
	strb	ip, [r4, #16]
	ldrb	ip, [r3, #2602]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L144
	ldrb	r1, [r3, #2842]	@ zero_extendqisi2
	cmp	r1, #8
	movhi	r1, #0
	movls	r1, #1
	strb	r1, [r4, #20]
.L145:
	ldrb	ip, [r3, #2862]	@ zero_extendqisi2
	add	r1, r4, #12288
	strb	ip, [r4, #24]
	ldrb	ip, [r3, #2861]	@ zero_extendqisi2
	strb	ip, [r4, #23]
	ldrb	ip, [r3, #2596]	@ zero_extendqisi2
	strb	ip, [r4, #22]
	ldrb	ip, [r3, #2860]	@ zero_extendqisi2
	strb	ip, [r4, #25]
	ldr	ip, [r3, #2872]
	strb	ip, [r4, #26]
	ldrb	ip, [r3, #2863]	@ zero_extendqisi2
	strb	ip, [r4, #21]
	ldrb	ip, [r3, #2847]	@ zero_extendqisi2
	strb	ip, [r4, #27]
	ldrb	ip, [r3, #2848]	@ zero_extendqisi2
	strb	ip, [r4, #28]
	ldr	ip, [r7, #3164]
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #30]
	ldr	ip, [r7, #3160]
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #29]
	ldrb	ip, [r3, #2845]	@ zero_extendqisi2
	strb	ip, [r4, #31]
	ldrb	ip, [r3, #2967]	@ zero_extendqisi2
	strb	ip, [r4, #32]
	ldrb	ip, [r3, #2857]	@ zero_extendqisi2
	strb	ip, [r4, #35]
	ldrb	ip, [r3, #2597]	@ zero_extendqisi2
	strb	ip, [r4, #36]
	ldrb	ip, [r3, #2853]	@ zero_extendqisi2
	strb	ip, [r4, #37]
	ldrb	ip, [r3, #2855]	@ zero_extendqisi2
	strb	ip, [r4, #38]
	ldrb	ip, [r3, #2856]	@ zero_extendqisi2
	strb	ip, [r4, #39]
	ldrb	ip, [r3, #2854]	@ zero_extendqisi2
	strb	ip, [r4, #40]
	ldr	ip, [r3, #2920]
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #42]
	ldr	ip, [r3, #2916]
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #43]
	ldr	ip, [r3, #2924]
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #45]
	ldrb	ip, [r3, #2840]	@ zero_extendqisi2
	strb	ip, [r4, #50]
	ldrb	ip, [r3, #2852]	@ zero_extendqisi2
	strb	ip, [r4, #49]
	ldrb	ip, [r3, #2851]	@ zero_extendqisi2
	strb	ip, [r4, #48]
	ldr	ip, [r3, #2932]
	str	ip, [r1, #84]
	ldrb	ip, [r3, #2600]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L154
	mov	ip, #0
	mov	lr, ip
	mov	r9, ip
	mov	r8, ip
.L148:
	strb	r8, [r4, #59]
	strb	r9, [r4, #60]
	strb	lr, [r4, #61]
	str	ip, [r0, #408]
	ldrb	r3, [r3, #2588]	@ zero_extendqisi2
	strb	r3, [r0, #386]
.L142:
	ldr	r3, [r7, #3260]
	add	r6, r6, #12672
	add	r6, r6, #53
	strh	r3, [r4, #72]	@ movhi
	ldr	r3, [r7, #3192]
	strh	r3, [r4, #74]	@ movhi
	ldr	r3, [r7, #3196]
	strh	r3, [r4, #76]	@ movhi
	ldrb	r3, [r2, #435]	@ zero_extendqisi2
	strb	r3, [r4, #63]
	ldrb	r3, [r2, #436]	@ zero_extendqisi2
	strb	r3, [r4, #62]
	ldr	r2, [r7, #3176]
	adds	r3, r2, #127
	addmi	r3, r2, #254
	mov	r3, r3, asr #7
	mov	r3, r3, asl #4
	str	r3, [r1, #116]
	ldr	r3, [r5, #3352]
	str	r3, [r1, #120]
	ldr	r3, [r5, #3356]
	str	r3, [r1, #124]
	ldr	r3, [r5, #3360]
	str	r3, [r1, #128]
	ldr	r3, [r5, #3364]
	str	r3, [r1, #132]
	ldr	r3, [r5, #3368]
	str	r3, [r1, #136]
	ldr	r3, [r5, #3372]
	str	r3, [r1, #140]
	ldr	r3, [r5, #3376]
	str	r3, [r1, #144]
	str	r6, [r4, #64]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L153:
	ldr	r2, [r3, #2980]
	movw	lr, #1544
	strb	r2, [r4, #5]
	ldrb	r2, [r3, #2604]	@ zero_extendqisi2
	strb	r2, [r4, #4]
	ldrb	r2, [r3, #2937]	@ zero_extendqisi2
	cmp	r2, #4
	moveq	r2, #3
	strb	r2, [r4, #3]
	ldr	r2, [r3, #2980]
	cmp	r2, #3
	ldr	r2, [r5, #3332]
	addeq	r2, r2, #1
	addeq	r2, r2, r2, lsr #31
	moveq	r2, r2, asr #1
	sub	r2, r2, #1
	strh	r2, [r4, #80]	@ movhi
	ldr	ip, [r5, #3328]
	add	r2, r6, #12288
	sub	ip, ip, #1
	strh	ip, [r4, #78]	@ movhi
	ldrb	r1, [r3, #2977]	@ zero_extendqisi2
	strb	r1, [r4, #8]
	ldrb	r1, [r3, #2780]	@ zero_extendqisi2
	strb	r1, [r4, #6]
	ldrb	r1, [r3, #2968]	@ zero_extendqisi2
	strb	r1, [r4, #9]
	ldrb	r1, [r2, #432]	@ zero_extendqisi2
	strb	r1, [r4, #7]
	ldr	ip, [r7, #3216]
	mla	ip, lr, ip, r2
	add	ip, ip, #15360
	ldrb	r1, [ip, #43]	@ zero_extendqisi2
	strb	r1, [r4, #10]
	ldr	r1, [r7, #3232]
	mla	r1, lr, r1, r2
	add	r1, r1, #15360
	add	r1, r1, #40
	ldrb	r1, [r1, #3]	@ zero_extendqisi2
	strb	r1, [r4, #11]
	ldrb	r1, [r3, #2966]	@ zero_extendqisi2
	strb	r1, [r4, #15]
	ldrb	r1, [r3, #2781]	@ zero_extendqisi2
	strb	r1, [r4, #13]
	ldrb	r1, [r3, #2785]	@ zero_extendqisi2
	strb	r1, [r4, #14]
	ldrb	r1, [r3, #2936]	@ zero_extendqisi2
	strb	r1, [r4, #12]
	ldrb	r1, [r3, #3104]	@ zero_extendqisi2
	strb	r1, [r4, #18]
	ldrb	r1, [r3, #2939]	@ zero_extendqisi2
	strb	r1, [r4, #19]
	ldrb	r1, [r3, #2946]	@ zero_extendqisi2
	strb	r1, [r4, #17]
	ldr	r1, [r3, #3052]
	strb	r1, [r4, #16]
	ldrb	r1, [r3, #2786]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L135
	ldrb	r1, [r3, #2946]	@ zero_extendqisi2
	cmp	r1, #8
	movhi	r1, #0
	movls	r1, #1
	strb	r1, [r4, #20]
.L136:
	ldrb	ip, [r3, #3098]	@ zero_extendqisi2
	add	r1, r4, #12288
	strb	ip, [r4, #24]
	ldrb	ip, [r3, #3097]	@ zero_extendqisi2
	strb	ip, [r4, #23]
	ldrb	ip, [r3, #2783]	@ zero_extendqisi2
	strb	ip, [r4, #22]
	ldrb	ip, [r3, #3096]	@ zero_extendqisi2
	strb	ip, [r4, #25]
	ldr	ip, [r3, #3108]
	strb	ip, [r4, #26]
	ldrb	ip, [r3, #3099]	@ zero_extendqisi2
	strb	ip, [r4, #21]
	ldrb	ip, [r3, #2950]	@ zero_extendqisi2
	strb	ip, [r4, #27]
	ldrb	ip, [r3, #2951]	@ zero_extendqisi2
	strb	ip, [r4, #28]
	ldrb	ip, [r3, #2969]	@ zero_extendqisi2
	strb	ip, [r4, #34]
	ldrb	ip, [r3, #2961]	@ zero_extendqisi2
	strb	ip, [r4, #33]
	ldrb	lr, [r3, #2977]	@ zero_extendqisi2
	ldr	ip, [r7, #3164]
	add	ip, ip, lr
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #30]
	ldr	ip, [r7, #3160]
	ldrb	lr, [r3, #2977]	@ zero_extendqisi2
	add	ip, ip, lr
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #29]
	ldrb	ip, [r3, #2949]	@ zero_extendqisi2
	strb	ip, [r4, #31]
	ldrb	ip, [r3, #2967]	@ zero_extendqisi2
	strb	ip, [r4, #32]
	ldrb	ip, [r3, #2960]	@ zero_extendqisi2
	strb	ip, [r4, #35]
	ldrb	ip, [r3, #2784]	@ zero_extendqisi2
	strb	ip, [r4, #36]
	ldrb	ip, [r3, #2956]	@ zero_extendqisi2
	strb	ip, [r4, #37]
	ldrb	ip, [r3, #2958]	@ zero_extendqisi2
	strb	ip, [r4, #38]
	ldrb	ip, [r3, #2959]	@ zero_extendqisi2
	strb	ip, [r4, #39]
	ldrb	ip, [r3, #2957]	@ zero_extendqisi2
	strb	ip, [r4, #40]
	ldrb	ip, [r3, #2973]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #46]
	ldrb	ip, [r3, #2971]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #42]
	ldrb	ip, [r3, #2975]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #44]
	ldrb	ip, [r3, #2976]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #41]
	ldrb	ip, [r3, #2970]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #43]
	ldrb	ip, [r3, #2974]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #47]
	ldrb	ip, [r3, #2972]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #45]
	ldrb	ip, [r3, #2938]	@ zero_extendqisi2
	strb	ip, [r4, #50]
	ldrb	ip, [r3, #2955]	@ zero_extendqisi2
	strb	ip, [r4, #49]
	ldrb	ip, [r3, #2954]	@ zero_extendqisi2
	strb	ip, [r4, #48]
	ldrb	ip, [r3, #2965]	@ zero_extendqisi2
	strb	ip, [r4, #53]
	ldrb	ip, [r3, #2962]	@ zero_extendqisi2
	strb	ip, [r4, #54]
	ldrb	ip, [r3, #2963]	@ zero_extendqisi2
	strb	ip, [r4, #51]
	ldrb	ip, [r3, #2964]	@ zero_extendqisi2
	strb	ip, [r4, #52]
	ldr	ip, [r3, #3084]
	str	ip, [r1, #84]
	ldr	ip, [r5, #3304]
	str	ip, [r1, #88]
	ldr	ip, [r5, #3308]
	str	ip, [r1, #92]
	ldrb	ip, [r3, #2789]	@ zero_extendqisi2
	strb	ip, [r4, #55]
	ldrb	ip, [r3, #2790]	@ zero_extendqisi2
	strb	ip, [r4, #56]
	ldrb	ip, [r3, #2791]	@ zero_extendqisi2
	strb	ip, [r4, #57]
	ldrb	ip, [r3, #2792]	@ zero_extendqisi2
	strb	ip, [r4, #58]
	ldr	ip, [r3, #2980]
	cmp	ip, #3
	beq	.L155
	ldrb	ip, [r3, #2789]	@ zero_extendqisi2
	strb	ip, [r0, #381]
	ldrb	ip, [r3, #2790]	@ zero_extendqisi2
	strb	ip, [r0, #382]
	ldrb	ip, [r3, #2791]	@ zero_extendqisi2
	strb	ip, [r0, #383]
	ldrb	ip, [r3, #2792]	@ zero_extendqisi2
	strb	ip, [r0, #384]
.L141:
	ldrb	r3, [r3, #2604]	@ zero_extendqisi2
	strb	r3, [r0, #386]
	ldrb	r3, [r2, #435]	@ zero_extendqisi2
	strb	r3, [r4, #63]
	b	.L142
.L144:
	cmp	ip, #1
	beq	.L156
	cmp	ip, #2
	movne	r1, #1
	strb	r1, [r4, #20]
	b	.L145
.L135:
	cmp	r1, #1
	beq	.L157
	cmp	r1, #2
	moveq	r1, #0
	movne	r1, #1
	strb	r1, [r4, #20]
	b	.L136
.L157:
	ldrb	r1, [r3, #2947]	@ zero_extendqisi2
	adds	r1, r1, #0
	movne	r1, #1
	strb	r1, [r4, #20]
	b	.L136
.L156:
	ldrb	r1, [r3, #2844]	@ zero_extendqisi2
	adds	r1, r1, #0
	movne	r1, #1
	strb	r1, [r4, #20]
	b	.L145
.L155:
	ldrb	ip, [r3, #2977]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L140
	ldrb	ip, [r3, #2789]	@ zero_extendqisi2
	strb	ip, [r0, #381]
	ldrb	ip, [r3, #2790]	@ zero_extendqisi2
	strb	ip, [r0, #382]
	ldrb	ip, [r3, #2791]	@ zero_extendqisi2
	strb	ip, [r0, #383]
	ldrb	ip, [r3, #2792]	@ zero_extendqisi2
	strb	ip, [r0, #384]
	ldrb	ip, [r3, #2977]	@ zero_extendqisi2
.L140:
	cmp	ip, #1
	bne	.L141
	ldrb	ip, [r3, #2789]	@ zero_extendqisi2
	strb	ip, [r0, #376]
	ldrb	ip, [r3, #2790]	@ zero_extendqisi2
	strb	ip, [r0, #377]
	ldrb	ip, [r3, #2791]	@ zero_extendqisi2
	strb	ip, [r0, #378]
	ldrb	ip, [r3, #2792]	@ zero_extendqisi2
	strb	ip, [r0, #379]
	b	.L141
.L154:
	ldr	lr, [r7, #3216]
	movw	r9, #1544
	ldr	ip, [r7, #3232]
	ldrb	r8, [r3, #2839]	@ zero_extendqisi2
	mla	lr, r9, lr, r2
	sub	r8, r8, #2
	mla	ip, r9, ip, r2
	tst	r8, #253
	add	lr, lr, #14336
	ldrneb	r8, [r3, #2838]	@ zero_extendqisi2
	add	ip, ip, #14336
	ldrb	r9, [lr, #42]	@ zero_extendqisi2
	ldrb	lr, [ip, #42]	@ zero_extendqisi2
	movne	ip, r8
	moveq	ip, lr
	moveq	r8, lr
	b	.L148
.L152:
	ldr	r3, .L158
	movw	r2, #1075
	ldr	r1, .L158+4
	mov	r0, #22
	ldr	r3, [r3, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L159:
	.align	2
.L158:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC24
	UNWIND(.fnend)
	.size	VC1_WritePicMsg, .-VC1_WritePicMsg
	.align	2
	.global	VC1_WriteSliceMsg
	.type	VC1_WriteSliceMsg, %function
VC1_WriteSliceMsg:
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
	add	r3, r0, #20480
	mov	r2, r1
	ldr	ip, .L161
	add	r1, r0, #23296
	ldr	lr, [r3, #2836]
	add	r0, r2, #84
	add	r1, r1, #24
	ldr	ip, [ip, #52]
	strb	lr, [r2, #70]
	ldr	r3, [r3, #2836]
	mov	r2, r3, asl #6
	sub	r2, r2, r3, asl #4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L162:
	.align	2
.L161:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VC1_WriteSliceMsg, .-VC1_WriteSliceMsg
	.align	2
	.global	VC1_CfgVDH
	.type	VC1_CfgVDH, %function
VC1_CfgVDH:
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
	ldrb	r3, [r0, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L164
	add	r3, r0, #8192
	ldr	r3, [r3, #2556]
	cmp	r3, #3
	beq	.L169
.L164:
	add	r3, r0, #32768
	add	r6, r1, #12288
	ldr	r3, [r3, #2912]
	sub	r3, r3, #1
	str	r3, [r6, #164]
.L165:
	add	r5, r0, #20480
	ldr	r3, [r6, #244]
	mov	ip, #0
	movw	r4, #1544
	ldr	r2, [r5, #2776]
	and	lr, r3, #1
	cmp	r2, ip
	add	r3, r2, #63
	movlt	r2, r3
	mov	r2, r2, asr #6
	mov	r2, r2, asl #4
	add	r3, r2, #536870913
	add	r3, r3, #81920
	orr	r3, r3, lr, asl #30
	str	r3, [r6, #168]
	ldr	lr, [r5, #2808]
	ldr	r3, [r5, #2792]
	ldr	r2, [r5, #2736]
	mla	lr, r4, lr, r0
	mla	r0, r4, r3, r0
	ldr	r2, [r2, #1180]
	add	r3, lr, #15104
	add	r3, r3, #24
	and	r2, r2, #3
	add	r0, r0, #15104
	ldr	r3, [r3, #4]
	add	r0, r0, #24
	mov	r2, r2, asl #4
	ldr	r0, [r0, #4]
	and	r3, r3, #3
	strb	ip, [r1, #69]
	orr	r3, r2, r3, asl #2
	and	r2, r0, #3
	orr	r3, r3, r2
	strb	r3, [r1, #68]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L169:
	add	r2, r0, #32768
	add	r6, r1, #12288
	ldr	r3, [r2, #2908]
	ldr	r2, [r2, #2904]
	add	r3, r3, #1
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	mul	r3, r2, r3
	sub	r3, r3, #1
	str	r3, [r6, #164]
	b	.L165
	UNWIND(.fnend)
	.size	VC1_CfgVDH, .-VC1_CfgVDH
	.align	2
	.global	VC1_Vfmw_GenerateDecPara
	.type	VC1_Vfmw_GenerateDecPara, %function
VC1_Vfmw_GenerateDecPara:
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
	.size	VC1_Vfmw_GenerateDecPara, .-VC1_Vfmw_GenerateDecPara
	.align	2
	.global	VC1DEC_GetImageBuffer
	.type	VC1DEC_GetImageBuffer, %function
VC1DEC_GetImageBuffer:
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
	add	r3, r0, #12288
	mov	r4, r0
	ldrb	r2, [r3, #427]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L182
.L172:
	add	r5, r4, #45056
	ldr	r0, [r5, #3776]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L174
	ldr	r0, [r5, #3776]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L183
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L182:
	ldrb	r3, [r3, #433]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L172
.L174:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L183:
	add	r1, r4, #48384
	ldr	r0, [r5, #3776]
	add	r1, r1, #96
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1DEC_GetImageBuffer, .-VC1DEC_GetImageBuffer
	.align	2
	.global	VC1_Vfmw_GetImageBuffer
	.type	VC1_Vfmw_GetImageBuffer, %function
VC1_Vfmw_GetImageBuffer:
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
	add	r6, r0, #45056
	add	r4, r0, #32768
	mov	r5, r0
	mov	r1, #1
	ldr	r0, [r6, #3776]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #3252]
	blt	.L204
	add	r3, r5, #26624
	add	r2, r3, #40
	ldrh	r3, [r3, #40]
	cmp	r3, #0
	beq	.L188
	add	r3, r5, #28160
	add	lr, r5, #34304
	add	r3, r3, #48
	add	lr, lr, #80
.L191:
	ldrh	ip, [r3]
	mov	r2, r3
	add	r3, r3, #1536
	cmp	ip, #0
	add	r3, r3, #8
	beq	.L188
	cmp	r3, lr
	bne	.L191
	mov	r1, r0
	mov	r2, #1
	ldr	r0, [r6, #3776]
	bl	FSP_ClearLogicFs
	ldr	r3, .L205
	ldr	r1, .L205+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	add	r3, r5, #26624
	add	r5, r5, #34304
	add	r3, r3, #41
	add	r5, r5, #81
	mov	r2, #0
.L193:
	strb	r2, [r3]
	add	r3, r3, #1536
	strb	r2, [r3, #-1537]
	add	r3, r3, #8
	cmp	r3, r5
	bne	.L193
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L188:
	mov	r3, #0
	str	r2, [r4, #3160]
	mov	r1, #1
	strb	r1, [r2, #1]
.L190:
	ldr	r1, [r4, #3160]
	uxtb	r2, r3
	add	r1, r1, r3
	strb	r2, [r1, #3]
	ldr	r1, [r4, #3160]
	add	r1, r1, r3
	strb	r2, [r1, #259]
	ldr	r1, [r4, #3160]
	add	r1, r1, r3
	strb	r2, [r1, #515]
	ldr	r1, [r4, #3160]
	add	r1, r1, r3
	add	r3, r3, #1
	cmp	r3, #256
	strb	r2, [r1, #771]
	bne	.L190
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L204:
	ldr	r9, .L205
	mov	r0, #0
	ldr	r1, .L205+8
	ldr	r3, [r9, #68]
	blx	r3
	ldr	r1, [r4, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L186
	ldr	r1, [r7, #212]
	mov	r8, #0
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r7, #216]
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r7, #220]
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r7, #224]
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	str	r8, [r7, #212]
	str	r8, [r7, #216]
	mov	r2, #1
	str	r8, [r7, #220]
	str	r8, [r7, #224]
	ldr	r1, [r4, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #3244]
	ldr	r0, [r6, #3776]
	bl	FSP_ClearLogicFs
	str	r8, [r4, #3244]
	str	r8, [r4, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #3248]
	str	r0, [r4, #3244]
.L186:
	add	r1, r5, #48384
	mov	r5, #0
	add	r1, r1, #96
	str	r5, [r4, #3240]
	ldr	r0, [r6, #3776]
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r9, #68]
	ldr	r1, .L205+12
	mov	r0, #1
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L206:
	.align	2
.L205:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC27
	.word	.LC25
	.word	.LC26
	UNWIND(.fnend)
	.size	VC1_Vfmw_GetImageBuffer, .-VC1_Vfmw_GetImageBuffer
	.align	2
	.global	VC1_Vfmw_MemMove
	.type	VC1_Vfmw_MemMove, %function
VC1_Vfmw_MemMove:
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
	add	r0, r0, #20480
	ldr	ip, .L208
	ldr	r3, [r0, #2756]
	ldr	r2, [r0, #2776]
	add	r3, r3, #1
	ldr	r1, [r0, #2740]
	ldr	r0, [r0, #2736]
	add	r3, r3, r3, lsr #31
	ldr	ip, [ip, #52]
	ldr	r1, [r1, #1064]
	mov	r3, r3, asr #1
	ldr	r0, [r0, #1064]
	mov	r3, r3, asl #5
	mul	r3, r2, r3
	add	r2, r3, r3, lsl #1
	mov	r2, r2, asr #1
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L209:
	.align	2
.L208:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VC1_Vfmw_MemMove, .-VC1_Vfmw_MemMove
	.align	2
	.global	VC1_Vfmw_BuildRefList
	.type	VC1_Vfmw_BuildRefList, %function
VC1_Vfmw_BuildRefList:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 536
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #540)
	sub	sp, sp, #540
	ldr	r6, .L285
	mov	r7, r0
	add	r0, r0, #23040
	mov	r10, r1
	mov	r4, r2
	ldr	ip, [r6, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r0, #224
	mov	r5, r3
	blx	ip
	add	r0, r7, #23040
	ldr	r3, [r6, #48]
	add	r0, r0, #240
	mov	r2, #16
	mov	r1, #0
	blx	r3
	ldrsb	r3, [r7, #4]
	bics	r3, r3, #2
	bne	.L211
	cmp	r10, #1
	beq	.L272
	cmp	r10, #2
	addne	ip, r7, #20480
	beq	.L273
.L220:
	mov	r0, #1
	mov	r3, #0
	str	r3, [ip, #2804]
	str	r3, [ip, #2788]
.L254:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L211:
	add	r6, r7, #8192
	ldrb	r3, [r6, #2516]	@ zero_extendqisi2
	cmp	r3, #1
	ldrb	r3, [r6, #2553]	@ zero_extendqisi2
	beq	.L274
	cmp	r3, #0
	beq	.L275
.L230:
	add	ip, r7, #20480
	ldr	r3, [ip, #2744]
	ldr	r9, [r3, #1228]
	mov	r8, r9
.L229:
	cmp	r10, #1
	beq	.L276
.L231:
	cmp	r10, #2
	bne	.L220
	cmp	r4, #0
	mov	r2, #0
	mov	r3, #1
	str	r10, [ip, #2804]
	str	r10, [ip, #2788]
	strb	r2, [ip, #2784]
	strb	r3, [ip, #2785]
	beq	.L243
	cmp	r5, r2
	beq	.L277
.L243:
	ldr	r3, [ip, #2744]
	ldr	lr, [ip, #2736]
	ldr	r2, [r3, #1228]
.L270:
	ldr	r0, [ip, #2740]
	mov	r1, #1
	str	r2, [ip, #2792]
	mov	r3, #0
	strb	r1, [ip, #2801]
	strb	r3, [ip, #2800]
	ldr	r1, [r0, #1228]
	str	r1, [ip, #2808]
	ldr	r0, [r0, #1228]
	str	r0, [ip, #2812]
	ldr	r0, [r6, #2640]
	ldr	r8, [lr, #1228]
	cmp	r0, #1
	bls	.L278
	bics	r0, r0, #2
	beq	.L249
.L248:
	movw	ip, #1544
	mov	r3, r7
	mul	r8, ip, r8
	mul	r0, ip, r2
	mov	r2, #0
	mul	r1, ip, r1
	str	r2, [fp, #-572]
	add	r2, r8, #13952
	str	r2, [fp, #-568]
	add	r2, r8, #14208
	str	r2, [fp, #-560]
	ldr	r2, [fp, #-568]
	add	r10, r0, #13952
	add	r9, r0, #14208
	add	r5, r1, #13952
	add	r2, r2, #2
	str	r2, [fp, #-568]
	ldr	r2, [fp, #-560]
	add	r4, r1, #14208
	add	lr, r1, #14464
	add	ip, r1, #14720
	add	r1, r8, #14464
	add	r9, r9, #2
	add	r2, r2, #2
	add	r1, r1, #2
	add	r10, r10, #2
	str	r1, [fp, #-576]
	add	ip, ip, #2
	add	r7, r7, r10
	add	r8, r8, #14720
	str	r7, [fp, #-560]
	add	r7, r3, r9
	add	r9, r3, r2
	ldr	r2, [fp, #-576]
	add	r6, r0, #14464
	add	r8, r8, #2
	add	r1, r3, ip
	str	r8, [fp, #-580]
	add	r0, r0, #14720
	ldr	ip, [fp, #-568]
	add	r6, r6, #2
	add	r0, r0, #2
	add	r5, r5, #2
	add	r4, r4, #2
	add	lr, lr, #2
	add	r8, r3, r2
	ldr	r2, [fp, #-580]
	add	r6, r3, r6
	add	r0, r3, r0
	add	r5, r3, r5
	add	r4, r3, r4
	add	lr, r3, lr
	add	ip, r3, ip
	str	r7, [fp, #-564]
	add	r7, r3, r2
	mov	r2, #0
	str	r1, [fp, #-568]
.L250:
	ldr	r1, [fp, #-560]
	add	r2, r2, #1
	cmp	r2, #256
	add	r3, r3, #1
	ldrb	r10, [r1, #1]!	@ zero_extendqisi2
	str	r1, [fp, #-560]
	ldr	r1, [fp, #-564]
	strb	r10, [r3, #12]
	ldrb	r10, [r1, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #268]
	ldrb	r10, [r6, #1]!	@ zero_extendqisi2
	str	r1, [fp, #-564]
	ldr	r1, [fp, #-568]
	strb	r10, [r3, #524]
	ldrb	r10, [r0, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #780]
	ldrb	r10, [r5, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #1036]
	ldrb	r10, [r4, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #1292]
	ldrb	r10, [lr, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #1548]
	ldrb	r10, [r1, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #1804]
	ldrb	r10, [ip, #1]!	@ zero_extendqisi2
	str	r1, [fp, #-568]
	strb	r10, [r3, #2060]
	ldrb	r10, [r9, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #2316]
	ldrb	r10, [r8, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #2572]
	ldrb	r10, [r7, #1]!	@ zero_extendqisi2
	strb	r10, [r3, #2828]
	bne	.L250
	mov	r0, #1
.L282:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L275:
	add	ip, r7, #20480
	cmp	r10, #1
	ldr	r2, [ip, #2744]
	ldr	r3, [ip, #2740]
	ldr	r9, [r2, #1228]
	ldr	r8, [r3, #1228]
	bne	.L231
.L276:
	ldrb	r3, [r6, #2544]	@ zero_extendqisi2
	cmp	r4, #0
	mov	r2, #0
	strb	r10, [ip, #2785]
	add	r3, r3, #1
	str	r2, [ip, #2804]
	str	r3, [ip, #2788]
	strb	r2, [ip, #2784]
	beq	.L232
	cmp	r5, r2
	beq	.L279
.L232:
	ldr	r3, [ip, #2744]
	ldr	r2, [ip, #2736]
	ldr	r5, [r3, #1228]
.L269:
	str	r5, [ip, #2792]
	ldr	r3, [r3, #1228]
	str	r3, [ip, #2796]
	ldr	r3, [r6, #2640]
	ldr	r4, [r2, #1228]
	cmp	r3, #1
	bls	.L280
.L236:
	bics	r2, r3, #2
	bne	.L237
	ldrb	r2, [r6, #2526]	@ zero_extendqisi2
	cmp	r2, #4
	beq	.L281
.L237:
	movw	r1, #1544
	mov	r3, r7
	mul	r0, r1, r5
	mov	r2, #0
	mul	r1, r1, r4
	add	r8, r0, #13952
	add	r6, r0, #14208
	add	r5, r0, #14464
	add	r0, r0, #14720
	add	r4, r1, #13952
	add	lr, r1, #14208
	add	ip, r1, #14464
	add	r1, r1, #14720
	add	r8, r8, #2
	add	r6, r6, #2
	add	r5, r5, #2
	add	r0, r0, #2
	add	r4, r4, #2
	add	lr, lr, #2
	add	ip, ip, #2
	add	r1, r1, #2
	add	r7, r7, r8
	add	r6, r3, r6
	add	r5, r3, r5
	add	r0, r3, r0
	add	r4, r3, r4
	add	lr, r3, lr
	add	ip, r3, ip
	add	r1, r3, r1
.L242:
	ldrb	r8, [r7, #1]!	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r2, #256
	add	r3, r3, #1
	strb	r8, [r3, #12]
	ldrb	r8, [r6, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #268]
	ldrb	r8, [r5, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #524]
	ldrb	r8, [r0, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #780]
	ldrb	r8, [r4, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #2060]
	ldrb	r8, [lr, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #2316]
	ldrb	r8, [ip, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #2572]
	ldrb	r8, [r1, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #2828]
	bne	.L242
	mov	r0, #1
	b	.L282
.L272:
	add	r2, r7, #20480
	ldr	r1, [r2, #2744]
	str	r10, [r2, #2788]
	str	r3, [r2, #2804]
	strb	r3, [r2, #2784]
	ldr	r4, [r1, #1228]
	str	r4, [r2, #2792]
	ldrb	r3, [r7, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r7, #8192
	beq	.L213
	ldr	r2, [r3, #2556]
	ldrb	r5, [r3, #2528]	@ zero_extendqisi2
	cmp	r2, #2
	ldrb	r6, [r3, #2529]	@ zero_extendqisi2
	ldrb	r0, [r3, #2526]	@ zero_extendqisi2
	beq	.L283
.L214:
	cmp	r0, #4
	beq	.L215
.L216:
	movw	r3, #1544
	add	r6, r7, #12
	mul	r2, r3, r4
	add	r5, r7, #268
	add	r4, r7, #524
	add	lr, r7, #780
	mov	r3, #0
	add	ip, r2, #13952
	add	r0, r2, #14208
	add	r1, r2, #14464
	add	r2, r2, #14720
	add	ip, ip, #2
	add	r0, r0, #2
	add	r1, r1, #2
	add	r2, r2, #2
	add	ip, r7, ip
	add	r0, r7, r0
	add	r1, r7, r1
	add	r7, r7, r2
.L218:
	ldrb	r2, [ip, #1]!	@ zero_extendqisi2
	add	r3, r3, #1
	cmp	r3, #256
	strb	r2, [r6, #1]!
	ldrb	r2, [r0, #1]!	@ zero_extendqisi2
	strb	r2, [r5, #1]!
	ldrb	r2, [r1, #1]!	@ zero_extendqisi2
	strb	r2, [r4, #1]!
	ldrb	r2, [r7, #1]!	@ zero_extendqisi2
	strb	r2, [lr, #1]!
	bne	.L218
	mov	r0, #1
	b	.L282
.L274:
	cmp	r3, #0
	beq	.L230
	add	ip, r7, #20480
	ldr	r2, [ip, #2740]
	ldr	r3, [ip, #2744]
	ldr	r9, [r2, #1228]
	ldr	r8, [r3, #1228]
	b	.L229
.L273:
	add	r2, r7, #20480
	mov	r1, #1
	ldr	r0, [r2, #2744]
	str	r1, [r2, #2804]
	str	r1, [r2, #2788]
	strb	r3, [r2, #2784]
	ldr	r1, [r2, #2740]
	ldr	r5, [r0, #1228]
	strb	r3, [r2, #2800]
	str	r5, [r2, #2792]
	ldr	r4, [r1, #1228]
	str	r4, [r2, #2808]
	ldrb	r3, [r7, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r7, #8192
	ldrneb	r6, [r3, #2528]	@ zero_extendqisi2
	ldrneb	r8, [r3, #2529]	@ zero_extendqisi2
	ldreqb	r6, [r3, #2425]	@ zero_extendqisi2
	ldreqb	r8, [r3, #2426]	@ zero_extendqisi2
	ldrneb	r3, [r3, #2526]	@ zero_extendqisi2
	ldreqb	r3, [r3, #2423]	@ zero_extendqisi2
	cmp	r3, #4
	beq	.L284
.L223:
	movw	r0, #1544
	mov	r3, r7
	mul	r1, r0, r5
	mov	r2, #0
	mul	r0, r0, r4
	add	r8, r1, #13952
	add	r6, r1, #14208
	add	r5, r1, #14464
	add	r1, r1, #14720
	add	r4, r0, #13952
	add	lr, r0, #14208
	add	ip, r0, #14464
	add	r0, r0, #14720
	add	r8, r8, #2
	add	r6, r6, #2
	add	r5, r5, #2
	add	r1, r1, #2
	add	r4, r4, #2
	add	lr, lr, #2
	add	ip, ip, #2
	add	r0, r0, #2
	add	r7, r7, r8
	add	r6, r3, r6
	add	r5, r3, r5
	add	r1, r3, r1
	add	r4, r3, r4
	add	lr, r3, lr
	add	ip, r3, ip
	add	r0, r3, r0
.L225:
	ldrb	r8, [r7, #1]!	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r2, #256
	add	r3, r3, #1
	strb	r8, [r3, #12]
	ldrb	r8, [r6, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #268]
	ldrb	r8, [r5, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #524]
	ldrb	r8, [r1, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #780]
	ldrb	r8, [r4, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #1036]
	ldrb	r8, [lr, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #1292]
	ldrb	r8, [ip, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #1548]
	ldrb	r8, [r0, #1]!	@ zero_extendqisi2
	strb	r8, [r3, #1804]
	bne	.L225
	mov	r0, #1
	b	.L282
.L213:
	ldrb	r5, [r3, #2425]	@ zero_extendqisi2
	ldrb	r6, [r3, #2426]	@ zero_extendqisi2
	ldrb	r0, [r3, #2423]	@ zero_extendqisi2
	b	.L214
.L249:
	ldrb	r3, [r6, #2526]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L248
	b	.L254
.L279:
	ldr	r2, [ip, #2736]
	ldr	r3, [ip, #2744]
	ldr	r5, [r2, #1228]
	b	.L269
.L277:
	ldr	lr, [ip, #2736]
	ldr	r2, [lr, #1228]
	b	.L270
.L280:
	ldrb	r3, [r6, #2526]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L237
	ldrb	ip, [r6, #2529]	@ zero_extendqisi2
	mov	r3, #0
	ldrb	r10, [r6, #2528]	@ zero_extendqisi2
	sub	r0, fp, #556
	mov	r2, ip
	str	ip, [fp, #-560]
	mov	r1, r10
	bl	VC1_Vfmw_GenerateIntensityMap
	ldr	ip, [fp, #-560]
	mov	r1, r10
	sub	r0, fp, #300
	mov	r3, #1
	mov	r2, ip
	bl	VC1_Vfmw_GenerateIntensityMap
	movw	r2, #1544
	mul	r2, r2, r9
	mov	r0, #0
	add	r1, r2, #13952
	add	r2, r2, #14208
	add	r1, r1, #3
	add	r2, r2, #3
	add	r1, r7, r1
	add	r2, r7, r2
.L238:
	ldrb	r3, [r1]	@ zero_extendqisi2
	sub	ip, fp, #44
	add	r0, r0, #1
	add	r3, ip, r3
	cmp	r0, #256
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [r1], #1
	ldrb	r3, [r2]	@ zero_extendqisi2
	add	r3, ip, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [r2], #1
	bne	.L238
	ldr	r3, [r6, #2640]
	b	.L236
.L278:
	ldrb	r0, [r6, #2526]	@ zero_extendqisi2
	cmp	r0, #4
	bne	.L248
	mov	r0, r3
	b	.L254
.L281:
	cmp	r3, #0
	sub	r0, fp, #556
	mov	r3, #0
	ldreqb	r9, [r6, #2644]	@ zero_extendqisi2
	ldrneb	r9, [r6, #2528]	@ zero_extendqisi2
	ldreqb	r6, [r6, #2648]	@ zero_extendqisi2
	ldrneb	r6, [r6, #2529]	@ zero_extendqisi2
	mov	r1, r9
	mov	r2, r6
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r2, r6
	mov	r1, r9
	mov	r3, #1
	sub	r0, fp, #300
	bl	VC1_Vfmw_GenerateIntensityMap
	movw	r3, #1544
	mul	r2, r3, r8
	mov	r0, #0
	add	r1, r2, #14464
	add	r2, r2, #14720
	add	r1, r1, #3
	add	r2, r2, #3
	add	r1, r7, r1
	add	r2, r7, r2
.L241:
	ldrb	r3, [r1]	@ zero_extendqisi2
	sub	ip, fp, #44
	add	r0, r0, #1
	add	r3, ip, r3
	cmp	r0, #256
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [r1], #1
	ldrb	r3, [r2]	@ zero_extendqisi2
	add	r3, ip, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [r2], #1
	bne	.L241
	b	.L237
.L283:
	ldr	r3, [r3, #2632]
	cmp	r3, #1
	bne	.L214
.L215:
	mov	r3, #0
	mov	r2, r6
	mov	r1, r5
	sub	r0, fp, #556
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r2, r6
	mov	r1, r5
	mov	r3, #1
	sub	r0, fp, #300
	bl	VC1_Vfmw_GenerateIntensityMap
	movw	r3, #1544
	mul	r3, r3, r4
	mov	lr, #0
	add	ip, r3, #13952
	add	r0, r3, #14208
	add	r1, r3, #14464
	add	r3, r3, #14720
	add	ip, ip, #3
	add	r0, r0, #3
	add	r1, r1, #3
	add	r3, r3, #3
	add	ip, r7, ip
	add	r0, r7, r0
	add	r1, r7, r1
	add	r2, r7, r3
.L217:
	ldrb	r3, [ip]	@ zero_extendqisi2
	sub	r5, fp, #44
	add	lr, lr, #1
	add	r3, r5, r3
	cmp	lr, #256
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [ip], #1
	ldrb	r3, [r0]	@ zero_extendqisi2
	add	r3, r5, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [r0], #1
	ldrb	r3, [r1]	@ zero_extendqisi2
	add	r3, r5, r3
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [r1], #1
	ldrb	r3, [r2]	@ zero_extendqisi2
	add	r3, r5, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [r2], #1
	bne	.L217
	b	.L216
.L284:
	mov	r2, r8
	mov	r1, r6
	mov	r3, #0
	sub	r0, fp, #556
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r2, r8
	mov	r1, r6
	sub	r0, fp, #300
	mov	r3, #1
	mov	r10, #0
	bl	VC1_Vfmw_GenerateIntensityMap
	movw	r2, #1544
	mul	r1, r2, r5
	str	r4, [fp, #-560]
	mul	r2, r2, r4
	add	r9, r1, #13952
	add	r8, r1, #14208
	add	r6, r1, #14464
	add	r1, r1, #14720
	add	lr, r2, #13952
	add	ip, r2, #14208
	add	r0, r2, #14464
	add	r2, r2, #14720
	add	r9, r9, #3
	add	r8, r8, #3
	add	r6, r6, #3
	add	r1, r1, #3
	add	lr, lr, #3
	add	ip, ip, #3
	add	r0, r0, #3
	add	r2, r2, #3
	add	r9, r7, r9
	add	r8, r7, r8
	add	r6, r7, r6
	add	r1, r7, r1
	add	lr, r7, lr
	add	ip, r7, ip
	add	r0, r7, r0
	add	r2, r7, r2
.L224:
	ldrb	r3, [r9]	@ zero_extendqisi2
	sub	r4, fp, #44
	add	r10, r10, #1
	add	r3, r4, r3
	cmp	r10, #256
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [r9], #1
	ldrb	r3, [r8]	@ zero_extendqisi2
	add	r3, r4, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [r8], #1
	ldrb	r3, [r6]	@ zero_extendqisi2
	add	r3, r4, r3
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [r6], #1
	ldrb	r3, [r1]	@ zero_extendqisi2
	add	r3, r4, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [r1], #1
	ldrb	r3, [lr]	@ zero_extendqisi2
	add	r3, r4, r3
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [lr], #1
	ldrb	r3, [ip]	@ zero_extendqisi2
	add	r3, r4, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [ip], #1
	ldrb	r3, [r0]	@ zero_extendqisi2
	add	r3, r4, r3
	ldrb	r3, [r3, #-512]	@ zero_extendqisi2
	strb	r3, [r0], #1
	ldrb	r3, [r2]	@ zero_extendqisi2
	add	r3, r4, r3
	ldrb	r3, [r3, #-256]	@ zero_extendqisi2
	strb	r3, [r2], #1
	bne	.L224
	ldr	r4, [fp, #-560]
	b	.L223
.L286:
	.align	2
.L285:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VC1_Vfmw_BuildRefList, .-VC1_Vfmw_BuildRefList
	.align	2
	.global	VC1_Vfmw_GetStreamInfo
	.type	VC1_Vfmw_GetStreamInfo, %function
VC1_Vfmw_GetStreamInfo:
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
	add	r3, r0, #15808
	str	r3, [fp, #-48]
	add	r4, r0, #32768
	ldrsb	r3, [r3]
	mov	r5, r0
	ldr	r6, [r4, #3176]
	cmp	r3, #0
	beq	.L288
	add	r3, r0, #20480
	ldr	r3, [r3, #2980]
	cmp	r3, #3
	ldr	r3, [r4, #3180]
	addeq	r3, r3, #1
	addeq	r3, r3, r3, lsr #31
	moveq	r3, r3, asr #1
	mul	r3, r3, r6
	str	r3, [fp, #-52]
	add	r3, r0, #12288
	add	r2, r3, #3760
	mov	r3, r2
	ldrh	r2, [r2, #2]
	ldrh	r3, [r3]
	adds	r1, r2, #15
	addmi	r1, r2, #30
	adds	r0, r3, #15
	addmi	r0, r3, #30
	mov	r1, r1, asr #4
	mov	r0, r0, asr #4
.L339:
	mov	r2, r0, asl #4
	movw	ip, #8160
	sub	r3, r2, #32
	cmp	r3, ip
	mov	r3, r1, asl #4
	bhi	.L291
	sub	lr, r3, #32
	cmp	lr, ip
	bhi	.L291
	ldr	r3, [r4, #3188]
	add	r7, r5, #12288
	cmp	r3, r1
	beq	.L374
.L294:
	ldr	r2, [r4, #3180]
	mov	r3, r6, asl #4
	add	r3, r3, #63
	str	r6, [r4, #3184]
	bic	r3, r3, #63
	str	r3, [r4, #3200]
	str	r2, [r4, #3188]
	ldrb	r3, [r7, #432]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L302
	mov	r3, #1
.L296:
	ldrb	r2, [r7, #427]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L375
.L299:
	ldrb	r2, [r7, #424]	@ zero_extendqisi2
	eor	r2, r2, #8
	ubfx	r2, r2, #3, #1
	orrs	r3, r3, r2
	bne	.L302
.L306:
	mov	r0, r5
	bl	VC1_Vfmw_GetImageBuffer
	subs	r9, r0, #0
	beq	.L376
	add	r10, r5, #45056
	ldr	r1, [r4, #3252]
	ldr	r0, [r10, #3776]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L308
	ldr	r2, [r5]
	mov	r0, #29
	ldr	ip, .L384
	ldr	r1, .L384+4
	ldrd	r8, [r2, #48]
	strd	r8, [r3, #8]
	ldr	r2, [r5]
	ldrd	r8, [r2, #64]
	strd	r8, [r3, #16]
	ldr	r2, [r5]
	ldrd	r8, [r2, #80]
	strd	r8, [r3, #24]
	mvn	r8, #0
	ldr	r2, [r5]
	mvn	r9, #0
	ldr	r2, [r2, #96]
	str	r2, [r3, #68]
	ldr	r2, [r5]
	ldr	r2, [r2, #100]
	str	r2, [r3, #72]
	ldr	r2, [r5]
	ldr	r2, [r2, #104]
	str	r2, [r3, #76]
	ldr	r2, [r5]
	ldr	r2, [r2, #120]
	str	r2, [r3, #80]
	ldr	r2, [r5]
	strd	r8, [r2, #48]
	ldrd	r8, [r3, #16]
	ldr	ip, [ip, #68]
	ldrd	r2, [r3, #8]
	strd	r8, [sp]
	blx	ip
.L308:
	ldr	r3, [fp, #-48]
	ldrsb	r3, [r3]
	cmp	r3, #0
	beq	.L309
	add	r3, r5, #20480
	ldr	r3, [r3, #2980]
	cmp	r3, #3
	beq	.L377
.L309:
	ldr	r3, [r4, #3160]
	mov	r2, #0
	str	r2, [r3, #1180]
.L310:
	ldr	r3, [r4, #3160]
	mov	r2, #0
	strb	r2, [r3, #2]
	ldr	r3, [fp, #-48]
	ldrsb	r3, [r3]
	cmp	r3, r2
	bne	.L311
	add	r3, r5, #20480
	ldrb	r2, [r3, #2600]	@ zero_extendqisi2
	cmp	r2, #1
	ldreqb	r2, [r3, #2838]	@ zero_extendqisi2
	ldreq	r3, [r4, #3160]
	streqb	r2, [r3, #2]
.L311:
	ldrb	r3, [r7, #426]	@ zero_extendqisi2
	cmp	r3, #5
	cmpne	r3, #1
	ldr	r3, [r4, #3240]
	movls	ip, #1
	movhi	ip, #0
	bhi	.L312
	cmp	r3, #0
	ldr	r2, [r4, #3244]
	ldr	r3, [r4, #3248]
	movne	r1, #2
	moveq	r1, #1
	cmp	r2, r3
	str	r1, [r4, #3240]
	beq	.L314
	ldr	r3, .L384
	mov	r0, #13
	ldr	r1, .L384+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r2, #0
	ldr	r1, [r4, #3244]
	ldr	r0, [r10, #3776]
	bl	FSP_SetRef
	ldr	r3, [r4, #3248]
.L314:
	ldr	r1, [r4, #3252]
	mov	r2, #1
	str	r3, [r4, #3244]
	str	r1, [r4, #3248]
	ldr	r0, [r10, #3776]
	bl	FSP_SetRef
	ldr	r1, [r4, #3244]
	ldr	r0, [r10, #3776]
	bl	FSP_RemovePmv
	ldrb	r3, [r7, #426]	@ zero_extendqisi2
	cmp	r3, #5
	cmpne	r3, #1
	bhi	.L316
	ldr	r3, [r4, #3168]
	mov	r1, #0
	mov	r2, #1
	strb	r1, [r3]
	ldr	r3, [r4, #3160]
	strb	r2, [r3]
	ldr	r2, [r4, #3164]
	ldr	r3, [r4, #3160]
	str	r2, [r4, #3168]
	str	r3, [r4, #3164]
.L316:
	ldr	r1, [r4, #3252]
	ldr	r0, [r10, #3776]
	bl	FSP_GetLogicFs
	ldr	r1, [r4, #3244]
	mov	r9, r0
	ldr	r0, [r10, #3776]
	bl	FSP_GetLogicFs
	ldr	r1, [r4, #3248]
	mov	r8, r0
	ldr	r0, [r10, #3776]
	bl	FSP_GetLogicFs
	cmp	r9, #0
	cmpne	r8, #0
	mov	r3, r8
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	mov	ip, r0
	cmp	r3, #0
	bne	.L378
	ldr	r3, [fp, #-48]
	ldrsb	r3, [r3]
	cmp	r3, #0
	bne	.L379
.L320:
	mov	r3, #0
.L368:
	str	r3, [r9, #156]
	ldrb	r3, [r7, #426]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L380
.L322:
	ldr	r3, [r9, #520]
	cmp	r3, #0
	beq	.L323
	ldr	r2, [r9, #528]
	cmp	r2, #0
	beq	.L323
	ldr	r2, [r9, #524]
	cmp	r2, #0
	beq	.L323
	ldr	r2, [ip, #520]
	cmp	r2, #0
	ldrne	r3, [r2, #12]
	ldreq	r3, [r3, #12]
	str	r3, [r7, #112]
	ldr	r3, [r8, #520]
	cmp	r3, #0
	ldreq	r3, [r9, #520]
	ldr	r3, [r3, #12]
	str	r3, [r7, #108]
	ldr	r3, [r9, #520]
	ldr	r3, [r3, #12]
	str	r3, [r7, #220]
	ldr	r3, [r9, #532]
	str	r3, [r7, #116]
	ldr	r3, [ip, #532]
	str	r3, [r7, #120]
	ldr	r3, [r9, #524]
	ldr	r3, [r3, #12]
	str	r3, [r7, #104]
	ldr	r3, [r9, #524]
	ldr	r3, [r3, #20]
	str	r3, [r7, #184]
	ldr	r3, [r9, #520]
	ldr	r9, [r4, #3160]
	ldr	r0, [r3, #12]
	bl	MEM_Phy2Vir
	str	r0, [r9, #1064]
	ldr	r0, [r7, #108]
	ldr	r9, [r4, #3168]
	bl	MEM_Phy2Vir
	str	r0, [r9, #1064]
	ldr	r0, [r7, #112]
	ldr	r9, [r4, #3164]
	bl	MEM_Phy2Vir
	str	r0, [r9, #1064]
.L301:
	ldr	r3, [fp, #-48]
	ldrsb	r3, [r3]
	cmp	r3, #0
	beq	.L381
	add	ip, r5, #20480
	ldr	r3, [r4, #3160]
	ldr	r2, [ip, #2980]
	strb	r2, [r3, #1027]
	ldr	r3, [r4, #3160]
	ldrb	r2, [ip, #2937]	@ zero_extendqisi2
	strb	r2, [r3, #1028]
	ldrb	r2, [ip, #2950]	@ zero_extendqisi2
	ldr	r3, [r4, #3160]
	sub	r0, r2, #2
	ldrb	r1, [ip, #2977]	@ zero_extendqisi2
	cmp	r0, #1
	movls	r2, #1
	bls	.L333
	cmp	r2, #4
	movne	r2, #0
	beq	.L382
.L333:
	add	r3, r3, r1
	strb	r2, [r3, #1029]
	ldr	r0, [r4, #3260]
	cmp	r0, #0
	beq	.L334
	sub	r0, r0, #1
	cmp	r0, #0
	mov	lr, r0
	uxtble	r1, r0
	ble	.L336
	add	r2, r5, #35840
	mov	r1, #0
	add	r2, r2, #200
.L337:
	ldr	r3, [r2, #64]
	add	r2, r2, #48
	strb	r1, [r2, #-48]
	add	r1, r1, #1
	cmp	r1, lr
	mul	r3, r3, r6
	sub	r3, r3, #1
	str	r3, [r2, #-32]
	bne	.L337
	uxtb	r1, r1
.L336:
	mov	r3, r0, asl #6
	movw	r2, #23768
	sub	r0, r3, r0, asl #4
	ldr	r3, [fp, #-52]
	add	r0, r7, r0
	movw	lr, #23752
	sub	r3, r3, #1
	strb	r1, [r0, lr]
	str	r3, [r0, r2]
	ldrb	r1, [r7, #426]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L332
	ldrb	r3, [ip, #2967]	@ zero_extendqisi2
	add	r0, r5, #45056
	ldr	r2, [ip, #3084]
	mul	r2, r2, r3
	mov	r2, r2, asr #8
	str	r2, [r0, #3304]
	rsb	r3, r2, r3
	subs	r3, r3, #1
	movmi	r3, #0
	str	r3, [r0, #3308]
	b	.L332
.L291:
	ldr	ip, .L384
	mov	r0, #0
	ldr	r1, .L384+12
	mov	r9, r0
	ldr	ip, [ip, #68]
	blx	ip
.L366:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L288:
	ldr	r1, [r4, #3180]
	mov	r0, r6
	mul	r3, r1, r6
	str	r3, [fp, #-52]
	b	.L339
.L302:
	add	r10, r5, #45056
	add	r3, r5, #34304
	add	r3, r3, #80
	ldr	r1, [r4, #3248]
	str	r3, [r4, #3160]
	str	r3, [r4, #3172]
	str	r3, [r4, #3168]
	str	r3, [r4, #3164]
	ldr	r0, [r10, #3776]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L298
	ldr	r1, [r9, #212]
	mov	r3, #0
	ldr	r0, [r10, #3776]
	str	r3, [fp, #-56]
	bl	FreeUsdByDec
	ldr	r1, [r9, #216]
	ldr	r0, [r10, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r9, #220]
	ldr	r0, [r10, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r9, #224]
	ldr	r0, [r10, #3776]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-56]
	mov	r2, #1
	str	r3, [r9, #212]
	str	r3, [r9, #216]
	str	r3, [r9, #220]
	str	r3, [r9, #224]
	ldr	r1, [r4, #3248]
	ldr	r0, [r10, #3776]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #3244]
	ldr	r0, [r10, #3776]
	bl	FSP_ClearLogicFs
	ldr	r3, [fp, #-56]
	str	r3, [r4, #3244]
	str	r3, [r4, #3248]
	ldr	r0, [r10, #3776]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #3248]
	str	r0, [r4, #3244]
.L298:
	add	r3, r5, #26624
	add	r1, r5, #34304
	ldr	r8, [fp, #-48]
	add	r3, r3, #41
	add	r1, r1, #81
	mov	r2, #0
	str	r2, [r4, #3240]
.L304:
	strb	r2, [r3]
	add	r3, r3, #1536
	strb	r2, [r3, #-1537]
	add	r3, r3, #8
	cmp	r3, r1
	bne	.L304
	add	r3, r5, #48384
	ldr	r0, [r10, #3776]
	add	r3, r3, #96
	str	r8, [fp, #-48]
	str	r3, [fp, #-56]
	mov	r1, r3
	bl	FSP_ClearNotInVoQueue
	ldrb	r9, [r7, #424]	@ zero_extendqisi2
	ldr	r3, [fp, #-56]
	and	r9, r9, #8
	ands	r9, r9, #255
	bne	.L306
	mov	r0, r3
	bl	ResetVoQueue
	ldr	r1, [r5]
	mov	r3, #2
	mov	r0, r5
	ldr	r2, [r1, #736]
	ldr	ip, [r1, #740]
	adds	r1, r2, #15
	addmi	r1, r2, #30
	adds	r2, ip, #15
	addmi	r2, ip, #30
	mov	r1, r1, asr #4
	str	r1, [r7, #256]
	mov	r2, r2, asr #4
	str	r2, [r7, #260]
	add	r2, r2, #1
	mov	r1, r1, asl #4
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
	mov	r2, r2, asl #5
	bl	VC1_ArrangeVahbMem
	cmp	r0, #1
	bne	.L305
	ldrb	r3, [r7, #424]	@ zero_extendqisi2
	orr	r3, r3, #8
	strb	r3, [r7, #424]
	b	.L306
.L376:
	ldr	r3, .L384
	mov	r0, #1
	ldr	r1, .L384+16
	ldr	r3, [r3, #68]
	blx	r3
	b	.L366
.L374:
	ldr	r3, [r4, #3184]
	cmp	r3, r0
	bne	.L294
	mov	r3, r6, asl #4
	add	r3, r3, #63
	bic	r3, r3, #63
	str	r3, [r4, #3200]
	ldrb	r3, [r7, #432]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #0
	bne	.L296
	b	.L299
.L312:
	cmp	r3, #1
	bhi	.L316
	ldr	r0, [r10, #3776]
	mov	r2, #1
	ldr	r1, [r4, #3252]
	mov	r9, ip
	str	ip, [fp, #-48]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #3160]
	mov	r0, #1
	ldr	ip, [fp, #-48]
	ldr	r3, .L384
	ldr	r1, .L384+20
	strb	ip, [r2]
	ldr	r2, [r4, #3160]
	strb	ip, [r2, #1]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L366
.L378:
	ldr	r1, [r4, #3252]
	mov	r2, #1
	str	r0, [fp, #-48]
	ldr	r0, [r10, #3776]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #3160]
	mov	lr, #0
	ldr	ip, [fp, #-48]
	ldr	r5, .L384
	mov	r3, r9
	strb	lr, [r2]
	mov	r0, lr
	ldr	r4, [r4, #3160]
	movw	r2, #2351
	ldr	r1, .L384+24
	strb	lr, [r4, #1]
	str	ip, [sp, #4]
	str	r8, [sp]
	ldr	ip, [r5, #68]
	blx	ip
	ldrb	r3, [r7, #426]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L373
.L325:
	ldr	r3, [r5, #68]
	mov	r0, #1
	ldr	r1, .L384+28
	mov	r9, #0
	blx	r3
	b	.L366
.L381:
	ldr	r1, [r4, #3160]
	add	r2, r5, #20480
	strb	r3, [r1, #1027]
	ldr	r1, [r4, #3160]
	ldrb	r0, [r2, #2839]	@ zero_extendqisi2
	strb	r0, [r1, #1028]
	ldrb	r0, [r2, #2847]	@ zero_extendqisi2
	ldr	r1, [r4, #3160]
	sub	ip, r0, #2
	cmp	ip, #1
	movls	r3, #1
	bls	.L331
	cmp	r0, #4
	beq	.L383
.L331:
	add	r6, r5, #45056
	strb	r3, [r1, #1030]
	strb	r3, [r1, #1029]
	mov	r2, #1
	mov	r3, #0
	str	r2, [r4, #3260]
	strb	r3, [r4, #3272]
	ldr	r0, [r7, #3552]
	ldr	r8, [r6, #3268]
	bl	BsPos
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	add	r0, r8, r0, asr #3
	str	r0, [r4, #3264]
	ldr	r0, [r7, #3552]
	ldr	r8, [r6, #3280]
	bl	BsPos
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	add	r0, r8, r0, asr #3
	str	r0, [r4, #3276]
	ldr	r0, [r7, #3552]
	bl	BsPos
	and	r0, r0, #7
	str	r0, [r4, #3280]
	ldr	r0, [r7, #3552]
	ldr	r8, [r6, #3288]
	bl	BsPos
	mov	r8, r8, asl #3
	rsb	r0, r0, r8
	str	r0, [r4, #3284]
	ldr	r2, [r6, #3328]
	ldr	r3, [r6, #3332]
	mul	r3, r3, r2
	sub	r3, r3, #1
	str	r3, [r4, #3288]
	ldrb	r1, [r7, #426]	@ zero_extendqisi2
.L332:
	add	r0, r5, #12672
	mov	r3, #1
	add	r0, r0, #40
	mov	r2, #0
	bl	VC1_Vfmw_BuildRefList
	subs	r9, r0, #0
	movne	r9, #1
	bne	.L366
	add	r5, r5, #45056
	ldr	r1, [r4, #3252]
	mov	r2, #1
	ldr	r0, [r5, #3776]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #3160]
	ldr	r3, .L384
	mov	r0, #1
	ldr	r1, .L384+32
	strb	r9, [r2]
	ldr	r2, [r4, #3160]
	strb	r9, [r2, #1]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L366
.L379:
	add	r3, r5, #20480
	ldr	r3, [r3, #2980]
	cmp	r3, #3
	bne	.L320
	ldr	r3, .L384+36
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L320
	b	.L368
.L323:
	ldr	r1, [r4, #3252]
	mov	r2, #1
	ldr	r0, [r10, #3776]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #3160]
	mov	r3, #0
	ldr	r5, .L384
	mov	r0, r3
	ldr	r1, .L384+40
	strb	r3, [r2]
	ldr	r2, [r4, #3160]
	strb	r3, [r2, #1]
	ldr	ip, [r9, #524]
	ldr	r3, [r4, #3252]
	ldr	r2, [r4, #3244]
	str	ip, [sp, #12]
	ldr	lr, [r9, #528]
	ldr	r4, [r4, #3248]
	ldr	ip, [r5, #68]
	str	lr, [sp, #8]
	ldr	lr, [r9, #520]
	stmia	sp, {r4, lr}
	blx	ip
	ldrb	r3, [r7, #426]	@ zero_extendqisi2
	cmp	r3, #5
	bne	.L325
.L373:
	ldr	r3, [r10, #3348]
	cmp	r3, #0
	subgt	r3, r3, #1
	strgt	r3, [r10, #3348]
	b	.L325
.L375:
	ldrb	r2, [r7, #433]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L301
	b	.L299
.L380:
	ldr	r3, [r8, #156]
	str	r3, [r9, #156]
	ldr	r2, [r4, #3168]
	ldr	r3, [r4, #3160]
	ldr	r2, [r2, #1180]
	str	r2, [r3, #1180]
	ldr	r3, [r10, #3348]
	add	r3, r3, #1
	str	r3, [r10, #3348]
	b	.L322
.L334:
	mov	r3, #1
	mov	r1, r0
	str	r3, [r4, #3260]
	b	.L336
.L377:
	ldr	r3, .L384+36
	ldr	r3, [r3]
	cmp	r3, #1
	ldreq	r2, [r4, #3160]
	streq	r3, [r2, #1180]
	bne	.L309
	b	.L310
.L305:
	ldr	r3, .L384
	mov	r0, #1
	ldr	r1, .L384+44
	ldr	r3, [r3, #68]
	blx	r3
	ldrb	r3, [r7, #424]	@ zero_extendqisi2
	bic	r3, r3, #8
	strb	r3, [r7, #424]
	b	.L366
.L382:
	ldrb	r2, [ip, #2951]	@ zero_extendqisi2
	sub	r2, r2, #2
	cmp	r2, #1
	movhi	r2, #0
	movls	r2, #1
	b	.L333
.L383:
	ldrb	r3, [r2, #2848]	@ zero_extendqisi2
	sub	r3, r3, #2
	cmp	r3, #1
	movhi	r3, #0
	movls	r3, #1
	b	.L331
.L385:
	.align	2
.L384:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC30
	.word	.LC31
	.word	.LC20
	.word	.LC29
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC36
	.word	USE_FF_APT_EN
	.word	.LC35
	.word	.LC28
	UNWIND(.fnend)
	.size	VC1_Vfmw_GetStreamInfo, .-VC1_Vfmw_GetStreamInfo
	.align	2
	.global	VC1_Vfmw_PrepareDecode
	.type	VC1_Vfmw_PrepareDecode, %function
VC1_Vfmw_PrepareDecode:
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
	bl	VC1_Vfmw_GetStreamInfo
	subs	r4, r0, #0
	movne	r4, #1
	bne	.L387
	ldr	r3, .L389
	ldr	r1, .L389+4
	ldr	r3, [r3, #68]
	blx	r3
.L387:
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L390:
	.align	2
.L389:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC37
	UNWIND(.fnend)
	.size	VC1_Vfmw_PrepareDecode, .-VC1_Vfmw_PrepareDecode
	.align	2
	.global	VC1_DecMvrange
	.type	VC1_DecMvrange, %function
VC1_DecMvrange:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	lr, .L392
	mov	r4, r0
	sub	ip, fp, #84
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	mov	r0, r4
	mov	r1, #3
	bl	BsShow
	sub	r3, fp, #20
	add	r3, r3, r0, lsl #3
	mov	r0, r4
	ldr	r1, [r3, #-60]
	ldr	r4, [r3, #-64]
	bl	BsSkip
	mov	r0, r4
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L393:
	.align	2
.L392:
	.word	.LANCHOR0+16
	UNWIND(.fnend)
	.size	VC1_DecMvrange, .-VC1_DecMvrange
	.align	2
	.global	VC1_DecTransacfrm
	.type	VC1_DecTransacfrm, %function
VC1_DecTransacfrm:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	mov	r2, #64
	mov	r4, r0
	mov	r1, #0
	sub	r0, fp, #84
	bl	memset
	mov	r3, #2
	mov	r1, r3
	mov	r0, r4
	mov	r2, #1
	str	r3, [fp, #-64]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r2, [fp, #-80]
	str	r2, [fp, #-72]
	str	r2, [fp, #-68]
	bl	BsShow
	sub	r3, fp, #20
	add	r3, r3, r0, lsl #3
	mov	r0, r4
	ldr	r1, [r3, #-60]
	ldr	r4, [r3, #-64]
	bl	BsSkip
	mov	r0, r4
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_DecTransacfrm, .-VC1_DecTransacfrm
	.align	2
	.global	VC1_SetCurPicResolutionInfo
	.type	VC1_SetCurPicResolutionInfo, %function
VC1_SetCurPicResolutionInfo:
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
	add	r3, r0, #20480
	cmp	r1, #1
	add	r0, r0, #32768
	ldr	r2, [r3, #2752]
	beq	.L397
	cmp	r1, #2
	beq	.L403
	cmp	r1, #0
	beq	.L402
	add	r2, r2, #1
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
.L403:
	str	r2, [r0, #2904]
	ldr	r3, [r3, #2756]
	add	r3, r3, #1
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	str	r3, [r0, #2908]
	b	.L400
.L397:
	add	r2, r2, #1
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
.L402:
	str	r2, [r0, #2904]
	ldr	r3, [r3, #2756]
	str	r3, [r0, #2908]
.L400:
	mul	r1, r3, r2
	mov	r2, r2, asl #4
	mov	r3, r3, asl #4
	str	r2, [r0, #2896]
	str	r3, [r0, #2900]
	str	r1, [r0, #2912]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_SetCurPicResolutionInfo, .-VC1_SetCurPicResolutionInfo
	.align	2
	.global	VC1_DecMvmodeProgrP
	.type	VC1_DecMvmodeProgrP, %function
VC1_DecMvmodeProgrP:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	mov	r6, r1
	mov	r1, #4
	mov	r7, r0
	bl	BsShow
	ldr	ip, .L415
	sub	r4, fp, #76
	sub	lr, fp, #56
	mov	r5, r0
	cmp	r0, #1
	ldmia	ip!, {r0, r1, r2, r3}
	ldr	r8, [ip], #4
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	str	r8, [r4]
	ldr	ip, [ip]
	stmia	lr!, {r0, r1, r2, r3}
	str	ip, [lr]
	ble	.L413
	cmp	r5, #3
	movle	r1, #3
	movle	r4, #2
	bgt	.L414
.L406:
	mov	r0, r7
	bl	BsSkip
	sub	r3, fp, #36
	add	r4, r3, r4, lsl #2
	cmp	r6, #12
	ldrhi	r0, [r4, #-40]
	ldrls	r0, [r4, #-20]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L414:
	cmp	r5, #8
	movlt	r1, #2
	movge	r1, #1
	movlt	r4, #1
	movge	r4, #0
	b	.L406
.L413:
	cmp	r5, #0
	mov	r1, #4
	moveq	r4, #3
	movne	r4, r1
	b	.L406
.L416:
	.align	2
.L415:
	.word	.LANCHOR0+80
	UNWIND(.fnend)
	.size	VC1_DecMvmodeProgrP, .-VC1_DecMvmodeProgrP
	.align	2
	.global	VC1_DecMvmodeProgrB
	.type	VC1_DecMvmodeProgrB, %function
VC1_DecMvmodeProgrB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	mov	r1, #1
	bl	BsSkip
	ldr	r2, .L418
	sub	r3, fp, #12
	add	r3, r3, r0, lsl #2
	ldmia	r2, {r0, r1}
	sub	r2, fp, #12
	stmdb	r2, {r0, r1}
	ldr	r0, [r3, #-8]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L419:
	.align	2
.L418:
	.word	.LANCHOR0+120
	UNWIND(.fnend)
	.size	VC1_DecMvmodeProgrB, .-VC1_DecMvmodeProgrB
	.align	2
	.global	VC1_DecMvmodeInterB
	.type	VC1_DecMvmodeInterB, %function
VC1_DecMvmodeInterB:
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
	UNWIND(.pad #40)
	sub	sp, sp, #40
	mov	r5, r1
	mov	r1, #3
	mov	r6, r0
	bl	BsShow
	ldr	r3, .L428
	sub	lr, fp, #72
	mov	r8, #0
	mov	r9, #1
	mov	ip, #2
	mov	r4, #3
	str	r8, [fp, #-52]
	str	r8, [fp, #-40]
	str	r9, [fp, #-56]
	str	ip, [fp, #-48]
	str	r4, [fp, #-44]
	mov	r7, r0
	cmp	r0, #1
	ldmia	r3, {r0, r1, r2, r3}
	stmia	lr, {r0, r1, r2, r3}
	ble	.L427
	cmp	r7, #4
	movlt	r1, #2
	movge	r1, #1
	movlt	r4, #1
	movge	r4, #0
.L422:
	mov	r0, r6
	bl	BsSkip
	sub	r3, fp, #36
	add	ip, r3, r4, lsl #2
	cmp	r5, #12
	ldrhi	r0, [ip, #-36]
	ldrls	r0, [ip, #-20]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L427:
	cmp	r7, r8
	mov	r1, r4
	movne	r4, ip
	b	.L422
.L429:
	.align	2
.L428:
	.word	.LANCHOR0+128
	UNWIND(.fnend)
	.size	VC1_DecMvmodeInterB, .-VC1_DecMvmodeInterB
	.align	2
	.global	VC1_DecMvmode2
	.type	VC1_DecMvmode2, %function
VC1_DecMvmode2:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	mov	r5, r1
	mov	r1, #3
	mov	r6, r0
	bl	BsShow
	ldr	r3, .L439
	sub	lr, fp, #60
	add	ip, r3, #16
	mov	r4, r0
	cmp	r0, #1
	ldmia	r3, {r0, r1, r2, r3}
	stmia	lr, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	sub	ip, fp, #28
	stmdb	ip, {r0, r1, r2, r3}
	ble	.L438
	cmp	r4, #4
	movlt	r1, #2
	movge	r1, #1
	movlt	r4, #1
	movge	r4, #0
.L432:
	mov	r0, r6
	bl	BsSkip
	sub	r3, fp, #28
	add	r4, r3, r4, lsl #2
	cmp	r5, #12
	ldrhi	r0, [r4, #-32]
	ldrls	r0, [r4, #-16]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L438:
	cmp	r4, #0
	mov	r1, #3
	moveq	r4, r1
	movne	r4, #2
	b	.L432
.L440:
	.align	2
.L439:
	.word	.LANCHOR0+128
	UNWIND(.fnend)
	.size	VC1_DecMvmode2, .-VC1_DecMvmode2
	.align	2
	.global	VC1_DecBfractionSMP
	.type	VC1_DecBfractionSMP, %function
VC1_DecBfractionSMP:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 216
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
	ldr	ip, .L447
	str	r0, [fp, #-248]
	sub	r10, fp, #244
	str	r1, [fp, #-252]
	sub	r8, fp, #216
	str	r2, [fp, #-256]
	sub	r5, fp, #156
	ldmia	ip!, {r0, r1, r2, r3}
	sub	lr, fp, #100
	sub	r7, fp, #188
	add	r9, ip, #12
	add	r6, ip, #40
	stmia	r10!, {r0, r1, r2, r3}
	add	r4, ip, #96
	ldmia	ip, {r0, r1, r2}
	add	ip, ip, #152
	stmia	r10, {r0, r1, r2}
	ldmia	r9!, {r0, r1, r2, r3}
	stmia	r8!, {r0, r1, r2, r3}
	ldmia	r9, {r0, r1, r2}
	stmia	r8, {r0, r1, r2}
	ldmia	r6!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldmia	r6!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldmia	r6!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldmia	r6, {r0, r1}
	stmia	r5, {r0, r1}
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r4, {r0, r1}
	stmia	lr, {r0, r1}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	r7!, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	stmia	r7, {r0, r1, r2, r3}
	mov	r1, #3
	ldr	r0, [fp, #-248]
	bl	BsShow
	mov	r4, r0
	cmp	r4, #6
	ldr	r0, [fp, #-248]
	bgt	.L442
	mov	r1, #3
	bl	BsSkip
	sub	r3, fp, #44
	add	r3, r3, r4, lsl #2
	ldr	r1, [r3, #-200]
	ldr	r2, [r3, #-172]
.L443:
	sub	r3, fp, #44
	mov	r0, r4
	add	r2, r3, r2, lsl #2
	ldr	r3, [r2, #-148]
	ldr	r2, [fp, #-252]
	mul	r3, r3, r1
	str	r3, [r2]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L442:
	mov	r1, #7
	bl	BsGet
	and	r4, r0, #15
	cmp	r4, #15
	beq	.L446
	sub	r0, r0, #112
	sub	r3, fp, #44
	add	r4, r4, #7
	add	r0, r3, r0, lsl #2
	ldr	r1, [r0, #-112]
	ldr	r2, [r0, #-56]
	b	.L443
.L446:
	ldr	r2, [fp, #-256]
	mov	r3, #4
	mov	r0, #22
	strb	r3, [r2]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L448:
	.align	2
.L447:
	.word	.LANCHOR0+160
	UNWIND(.fnend)
	.size	VC1_DecBfractionSMP, .-VC1_DecBfractionSMP
	.align	2
	.global	VC1_DecBfraction
	.type	VC1_DecBfraction, %function
VC1_DecBfraction:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 216
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #228)
	sub	sp, sp, #228
	ldr	r6, .L459
	str	r0, [fp, #-248]
	sub	r10, fp, #244
	str	r1, [fp, #-256]
	sub	r8, fp, #216
	ldmia	r6!, {r0, r1, r2, r3}
	sub	lr, fp, #188
	str	lr, [fp, #-252]
	sub	r4, fp, #156
	sub	ip, fp, #100
	add	r9, r6, #12
	add	r5, r6, #40
	stmia	r10!, {r0, r1, r2, r3}
	add	lr, r6, #96
	ldmia	r6, {r0, r1, r2}
	add	r7, r6, #152
	stmia	r10, {r0, r1, r2}
	ldmia	r9!, {r0, r1, r2, r3}
	stmia	r8!, {r0, r1, r2, r3}
	ldmia	r9, {r0, r1, r2}
	stmia	r8, {r0, r1, r2}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5, {r0, r1}
	stmia	r4, {r0, r1}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1}
	ldr	lr, [fp, #-252]
	stmia	ip, {r0, r1}
	ldmia	r7!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r7, {r0, r1, r2, r3}
	stmia	lr, {r0, r1, r2, r3}
	mov	r1, #3
	ldr	r0, [fp, #-248]
	bl	BsShow
	mov	r4, r0
	cmp	r4, #6
	ldr	r0, [fp, #-248]
	bgt	.L450
	mov	r1, #3
	bl	BsSkip
	sub	r3, fp, #44
	add	r2, r3, r4, lsl #2
	ldr	r1, [r2, #-200]
	ldr	r2, [r2, #-172]
	sub	r3, r2, #1
	cmp	r3, #7
	ldrhi	r5, .L459+4
	bhi	.L454
.L458:
	sub	r2, fp, #44
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-144]
	mul	r3, r3, r1
.L455:
	ldr	r2, [fp, #-256]
	str	r3, [r2]
.L452:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L450:
	mov	r1, #7
	bl	BsGet
	and	r4, r0, #15
	sub	r3, r4, #14
	add	r4, r4, #7
	cmp	r3, #1
	bls	.L452
	sub	r3, r0, #112
	cmp	r3, #13
	bhi	.L453
	sub	r2, fp, #44
	add	r2, r2, r3, lsl #2
	ldr	r1, [r2, #-112]
	ldr	r2, [r2, #-56]
	sub	r3, r2, #1
	cmp	r3, #7
	ldrhi	r5, .L459+4
	bls	.L458
.L454:
	str	r2, [sp]
	mov	r3, #3520
	ldr	ip, [r5, #68]
	mov	r0, #1
	ldr	r2, .L459+8
	ldr	r1, .L459+12
	blx	ip
	mov	r3, #0
	b	.L455
.L453:
	ldr	r5, .L459+4
	add	r2, r6, #184
	str	r0, [sp]
	movw	r3, #3510
	ldr	r1, .L459+16
	mov	r0, #1
	ldr	ip, [r5, #68]
	blx	ip
	mov	r2, #0
	b	.L454
.L460:
	.align	2
.L459:
	.word	.LANCHOR0+160
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+360
	.word	.LC39
	.word	.LC38
	UNWIND(.fnend)
	.size	VC1_DecBfraction, .-VC1_DecBfraction
	.align	2
	.global	VC1_VopDQuant
	.type	VC1_VopDQuant, %function
VC1_VopDQuant:
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
	cmp	r1, #2
	mov	r6, r2
	mov	r4, r3
	mov	r7, r0
	beq	.L479
	mov	r1, #1
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r4]
	cmp	r5, #1
	beq	.L480
.L465:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L479:
	mov	r3, #1
	mov	r1, #3
	strb	r3, [r4, #3]
	strb	r3, [r4]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #4]
	cmp	r0, #7
	addne	r6, r6, #1
	addne	r3, r0, r6
	strneb	r3, [r4, #8]
	beq	.L481
.L464:
	mov	r3, #1
	mov	r0, #1
	str	r3, [r4, #12]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L480:
	mov	r1, #2
	mov	r0, r7
	bl	BsGet
	uxtb	r1, r0
	strb	r1, [r4, #1]
	cmp	r1, #0
	streq	r5, [r4, #12]
	beq	.L468
	cmp	r1, #2
	beq	.L482
	cmp	r1, #1
	beq	.L483
	cmp	r1, #3
	movne	r3, #0
	strne	r3, [r4, #12]
	beq	.L484
.L468:
	ldr	r3, [r4]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #768
	beq	.L465
	mov	r3, #1
	mov	r1, #3
	strb	r3, [r4, #3]
	mov	r0, r7
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #4]
	cmp	r0, #7
	addne	r6, r6, #1
	addne	r6, r0, r6
	strneb	r6, [r4, #8]
	bne	.L465
	mov	r0, r7
	mov	r1, #5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #5]
	strb	r0, [r4, #8]
	b	.L465
.L484:
	mov	r1, r5
	mov	r0, r7
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #2]
	cmp	r0, #0
	movne	r3, #10
	moveq	r3, #11
	str	r3, [r4, #12]
	b	.L468
.L481:
	mov	r0, r7
	mov	r1, #5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #5]
	strb	r0, [r4, #8]
	b	.L464
.L482:
	mov	r0, r7
	bl	BsGet
	uxtb	r3, r0
	strb	r0, [r4, #6]
	add	r3, r3, #6
	str	r3, [r4, #12]
	b	.L468
.L483:
	mov	r1, #2
	mov	r0, r7
	bl	BsGet
	uxtb	r3, r0
	strb	r0, [r4, #7]
	add	r3, r3, #2
	str	r3, [r4, #12]
	b	.L468
	UNWIND(.fnend)
	.size	VC1_VopDQuant, .-VC1_VopDQuant
	.align	2
	.global	VC1_DecNormDiff6Tile
	.type	VC1_DecNormDiff6Tile, %function
VC1_DecNormDiff6Tile:
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
	ldr	r9, [r1]
	mov	r8, r1
	mov	r5, r0
	mov	r6, #0
.L486:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, #3
	subs	r7, r0, #0
	mov	r0, r5
	bne	.L495
	bl	BsGet
	mov	r4, r0
	cmp	r4, #1
	mov	r1, r0
	mov	r0, r5
	bgt	.L505
	bne	.L489
	bl	BsGet
	cmp	r0, #0
	beq	.L490
	cmp	r6, #0
	mov	r6, r4
	beq	.L486
.L503:
	str	r9, [r8]
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L495:
	mov	r0, #0
.L487:
	cmp	r6, #0
	mov	r7, #1
	eorne	r0, r0, #63
.L504:
	str	r0, [r8]
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L505:
	sub	r4, r4, #2
	mov	r0, #1
	mov	r0, r0, asl r4
	b	.L487
.L489:
	mov	r0, r5
	mov	r1, #4
	bl	BsGet
	cmp	r0, #255
	beq	.L503
	ldr	r3, .L506
	add	r3, r3, r0, lsl #2
	ldr	r0, [r3, #508]
	b	.L487
.L490:
	mov	r0, r5
	mov	r1, #5
	bl	BsGet
	ldr	r3, .L506
	add	r3, r3, r0, lsl #2
	ldr	r3, [r3, #380]
	cmp	r3, #2
	orreq	r0, r0, #32
	beq	.L487
	cmp	r3, #3
	bne	.L504
	b	.L487
.L507:
	.align	2
.L506:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VC1_DecNormDiff6Tile, .-VC1_DecNormDiff6Tile
	.align	2
	.global	VC1_Vfmw_ParseSMPIPicHdr
	.type	VC1_Vfmw_ParseSMPIPicHdr, %function
VC1_Vfmw_ParseSMPIPicHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r8, r1
	mov	r1, #7
	mov	r4, r2
	mov	r6, r0
	bl	BsGet
	mov	r1, #5
	strb	r0, [r4, #253]
	mov	r0, r6
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r4, #254]
	cmp	r5, #0
	beq	.L509
	ldrb	r7, [r4, #14]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L524
	sub	r0, fp, #164
	bl	memcpy
	cmp	r7, #0
	mov	r3, r5
	subeq	r3, fp, #36
	addeq	r3, r3, r5, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r5, #8
	str	r3, [r4, #340]
	movhi	r3, #0
	strhib	r3, [r4, #255]
	bls	.L521
.L512:
	cmp	r7, #1
	movne	r3, #0
	strneb	r3, [r4, #256]
	beq	.L522
.L514:
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r3, #0
	strneb	r3, [r4, #257]
	beq	.L523
.L516:
	ldrb	r3, [r4, #251]	@ zero_extendqisi2
	sub	r3, r3, #2
	tst	r3, #253
	bne	.L517
.L519:
	ldrb	r1, [r4, #258]	@ zero_extendqisi2
.L518:
	mov	r0, r8
	mov	r5, #1
	bl	VC1_SetCurPicResolutionInfo
	mov	r0, r6
	bl	VC1_DecTransacfrm
	strb	r0, [r4, #267]
	mov	r0, r6
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #268]
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #269]
.L509:
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L517:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L519
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	uxtb	r1, r0
	strb	r1, [r4, #258]
	b	.L518
.L521:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	ldrb	r7, [r4, #14]	@ zero_extendqisi2
	strb	r0, [r4, #255]
	b	.L512
.L522:
	mov	r1, r7
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #256]
	b	.L514
.L523:
	mov	r0, r6
	bl	VC1_DecMvrange
	strb	r0, [r4, #257]
	b	.L516
.L525:
	.align	2
.L524:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseSMPIPicHdr, .-VC1_Vfmw_ParseSMPIPicHdr
	.align	2
	.global	VC1_Vfmw_ParseSMPPPicHdr
	.type	VC1_Vfmw_ParseSMPPPicHdr, %function
VC1_Vfmw_ParseSMPPPicHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r7, r1
	mov	r1, #5
	mov	r4, r2
	mov	r5, r0
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r4, #254]
	cmp	r6, #0
	beq	.L539
	ldrb	r8, [r4, #14]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L556
	sub	r0, fp, #164
	bl	memcpy
	cmp	r8, #0
	mov	r3, r6
	subeq	r3, fp, #36
	addeq	r3, r3, r6, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r6, #8
	str	r3, [r4, #340]
	movhi	r3, #0
	strhib	r3, [r4, #255]
	bls	.L550
.L531:
	cmp	r8, #1
	movne	r3, #0
	strneb	r3, [r4, #256]
	beq	.L551
.L533:
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r3, #0
	strneb	r3, [r4, #257]
	beq	.L552
.L535:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L536
	ldrb	r1, [r4, #258]	@ zero_extendqisi2
.L537:
	ldr	r6, .L556+4
	mov	r0, r7
	bl	VC1_SetCurPicResolutionInfo
	mov	r0, r5
	ldr	r8, [r6, #68]
	bl	BsPos
	ldr	r1, .L556+8
	mov	r2, r0
	mov	r0, #18
	blx	r8
	ldrb	r1, [r4, #340]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_DecMvmodeProgrP
	uxtb	r0, r0
	strb	r0, [r4, #259]
	cmp	r0, #4
	beq	.L553
.L538:
	mov	r1, r7
	mov	r3, #0
	mov	r2, r4
	mov	r0, r5
	bl	BPD_Drv
	cmp	r0, #1
	beq	.L539
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #263]
	mov	r0, r5
	bl	BsGet
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	cmp	r1, #0
	strb	r0, [r4, #264]
	bne	.L554
.L540:
	mov	r0, r5
	ldr	r6, [r6, #68]
	bl	BsPos
	ldr	r1, .L556+12
	mov	r2, r0
	mov	r0, #18
	blx	r6
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L555
.L542:
	mov	r0, r5
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #267]
	mov	r0, r5
	bl	BsGet
	mov	r3, #1
	strb	r0, [r4, #269]
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L539:
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L550:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	ldrb	r8, [r4, #14]	@ zero_extendqisi2
	strb	r0, [r4, #255]
	b	.L531
.L554:
	add	r3, r4, #272
	ldrb	r2, [r4, #340]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_VopDQuant
	b	.L540
.L552:
	mov	r0, r5
	bl	VC1_DecMvrange
	strb	r0, [r4, #257]
	b	.L535
.L536:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	uxtb	r1, r0
	strb	r1, [r4, #258]
	b	.L537
.L551:
	mov	r1, r8
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #256]
	b	.L533
.L553:
	ldrb	r1, [r4, #340]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_DecMvmode2
	mov	r1, #6
	strb	r0, [r4, #260]
	mov	r0, r5
	bl	BsGet
	mov	r1, #6
	strb	r0, [r4, #261]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #262]
	b	.L538
.L555:
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #265]
	cmp	r0, #1
	bne	.L542
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #266]
	b	.L542
.L557:
	.align	2
.L556:
	.word	.LANCHOR0+568
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC40
	.word	.LC41
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseSMPPPicHdr, .-VC1_Vfmw_ParseSMPPPicHdr
	.align	2
	.global	VC1_Vfmw_ParseMPBPicHdr
	.type	VC1_Vfmw_ParseMPBPicHdr, %function
VC1_Vfmw_ParseMPBPicHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r8, r1
	mov	r1, #5
	mov	r4, r2
	mov	r6, r0
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r4, #254]
	cmp	r5, #0
	beq	.L569
	ldrb	r7, [r4, #14]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L586
	sub	r0, fp, #164
	bl	memcpy
	cmp	r7, #0
	mov	r3, r5
	subeq	r3, fp, #36
	addeq	r3, r3, r5, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r5, #8
	str	r3, [r4, #340]
	movhi	r3, #0
	strhib	r3, [r4, #255]
	bls	.L581
.L563:
	cmp	r7, #1
	movne	r3, #0
	strneb	r3, [r4, #256]
	beq	.L582
.L565:
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r3, #0
	strneb	r3, [r4, #257]
	beq	.L583
.L567:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	mov	r3, #0
	mov	r1, r8
	mov	r2, r4
	cmp	r0, r3
	mov	r0, r6
	moveq	ip, #3
	movne	ip, #1
	strb	ip, [r4, #259]
	bl	BPD_Drv
	cmp	r0, #1
	beq	.L569
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #263]
	mov	r0, r6
	bl	BsGet
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	cmp	r1, #0
	strb	r0, [r4, #264]
	bne	.L584
.L570:
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L585
.L572:
	mov	r0, r6
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #267]
	mov	r0, r6
	bl	BsGet
	mov	r3, #1
	strb	r0, [r4, #269]
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L569:
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L581:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	ldrb	r7, [r4, #14]	@ zero_extendqisi2
	strb	r0, [r4, #255]
	b	.L563
.L584:
	add	r3, r4, #272
	ldrb	r2, [r4, #340]	@ zero_extendqisi2
	mov	r0, r6
	bl	VC1_VopDQuant
	b	.L570
.L582:
	mov	r1, r7
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #256]
	b	.L565
.L583:
	mov	r0, r6
	bl	VC1_DecMvrange
	strb	r0, [r4, #257]
	b	.L567
.L585:
	mov	r0, r6
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #265]
	cmp	r0, #1
	bne	.L572
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #266]
	b	.L572
.L587:
	.align	2
.L586:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseMPBPicHdr, .-VC1_Vfmw_ParseMPBPicHdr
	.align	2
	.global	VC1_Vfmw_ParseSMPPicHdr
	.type	VC1_Vfmw_ParseSMPPicHdr, %function
VC1_Vfmw_ParseSMPPicHdr:
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
	ldrb	r8, [r2, #258]	@ zero_extendqisi2
	mov	r5, r0
	mov	r6, r1
	mov	r0, r1
	mov	r4, r2
	mov	r1, r8
	bl	VC1_SetCurPicResolutionInfo
	ldr	r3, [r5, #16]
	cmp	r3, #1
	ble	.L626
	ldr	r7, .L631
	mov	r2, #100
	mov	r1, #0
	add	r0, r4, #248
	add	r9, r6, #32768
	ldr	r3, [r7, #48]
	blx	r3
	add	r0, r6, #23296
	ldr	r3, [r7, #48]
	mov	r2, #12288
	mov	r1, #0
	strb	r8, [r4, #258]
	add	r0, r0, #24
	blx	r3
	ldr	r3, [r7, #68]
	ldr	r2, [r9, #2840]
	mov	r0, #18
	ldr	r1, .L631+4
	blx	r3
	mov	r0, r5
	bl	BsPos
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r4, #324]
	bne	.L591
	ldrb	r2, [r4, #248]	@ zero_extendqisi2
.L592:
	ldr	r3, [r7, #68]
	mov	r0, #18
	ldr	r1, .L631+8
	blx	r3
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #249]
	bne	.L627
.L593:
	ldrb	r3, [r4, #13]	@ zero_extendqisi2
	mov	r1, #1
	mov	r0, r5
	cmp	r3, #0
	bne	.L594
	bl	BsGet
	adds	r0, r0, #0
	movne	r0, #1
	strb	r0, [r4, #251]
.L595:
	mov	r3, #0
	strb	r0, [r6, #2]
	strb	r3, [r6, #4]
	ldrb	r3, [r4, #251]	@ zero_extendqisi2
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L607
.L600:
	.word	.L599
	.word	.L601
	.word	.L602
	.word	.L607
	.word	.L603
.L603:
	ldrb	r8, [r6, #3099]	@ zero_extendqisi2
	cmp	r8, #1
	beq	.L628
.L607:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L626:
	mov	r3, #5
	mov	r0, #1
	strb	r3, [r4, #251]
	strb	r3, [r6, #2]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L594:
	bl	BsGet
	cmp	r0, #1
	beq	.L624
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #1
	beq	.L629
	mov	r2, r4
	mov	r3, #2
	add	r1, r4, #344
	strb	r3, [r2, #251]!
	mov	r0, r5
	bl	VC1_DecBfractionSMP
	uxtb	r0, r0
	strb	r0, [r4, #252]
	cmp	r0, #21
	ldrneb	r0, [r4, #251]	@ zero_extendqisi2
	bne	.L595
	b	.L607
.L591:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r2, r0
	strb	r2, [r4, #248]
	b	.L592
.L629:
	mov	r0, #0
.L624:
	strb	r0, [r4, #251]
	b	.L595
.L602:
	ldrb	r8, [r6, #3099]	@ zero_extendqisi2
	cmp	r8, #1
	bne	.L607
	mov	r1, r6
	mov	r2, r4
	mov	r0, r5
	bl	VC1_Vfmw_ParseMPBPicHdr
	cmp	r0, #1
	mov	r6, r0
	beq	.L604
	mov	r0, r8
	ldr	r3, [r7, #68]
	ldr	r1, .L631+12
	blx	r3
	b	.L604
.L601:
	mov	r1, r6
	mov	r2, r4
	mov	r0, r5
	bl	VC1_Vfmw_ParseSMPPPicHdr
	mov	r6, r0
.L604:
	mov	r0, r5
	bl	BsPos
	ldr	r3, [r4, #324]
	rsb	r0, r3, r0
	str	r0, [r4, #324]
	mov	r0, r5
	ldr	r4, [r9, #2864]
	bl	BsPos
	mov	r4, r4, asl #3
	rsb	r0, r0, r4
	cmp	r0, #0
	ble	.L630
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L599:
	mov	r1, r6
	mov	r2, r4
	mov	r0, r5
	bl	VC1_Vfmw_ParseSMPIPicHdr
	mov	r6, r0
	b	.L604
.L627:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #250]
	b	.L593
.L630:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L631+16
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L628:
	mov	r1, r6
	mov	r2, r4
	mov	r0, r5
	bl	VC1_Vfmw_ParseSMPIPicHdr
	cmp	r0, #1
	mov	r6, r0
	beq	.L604
	mov	r0, r8
	ldr	r3, [r7, #68]
	ldr	r1, .L631+20
	blx	r3
	b	.L604
.L632:
	.align	2
.L631:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC42
	.word	.LC43
	.word	.LC44
	.word	.LC46
	.word	.LC45
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseSMPPicHdr, .-VC1_Vfmw_ParseSMPPicHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvSeqHdr
	.type	VC1_Vfmw_ParseAdvSeqHdr, %function
VC1_Vfmw_ParseAdvSeqHdr:
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
	ldr	r3, .L650
	mov	r5, r0
	mov	r4, r2
	mov	r1, #0
	add	r0, r2, #16
	mov	r2, #172
	ldr	r3, [r3, #48]
	mov	r6, #2
	blx	r3
	mov	r0, r5
	bl	BsPos
	mov	r1, r6
	str	r0, [r4, #184]
	mov	r0, r5
	bl	BsGet
	strb	r6, [r4, #16]
	mov	r1, #3
	mov	r0, r5
	bl	BsGet
	mov	r1, r6
	strb	r0, [r4, #17]
	mov	r0, r5
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #18]
	mov	r0, r5
	bl	BsGet
	mov	r1, #5
	strb	r0, [r4, #19]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #20]
	mov	r0, r5
	bl	BsGet
	mov	r1, #12
	strb	r0, [r4, #21]
	mov	r0, r5
	bl	BsGet
	mov	r1, #12
	uxth	r3, r0
	mov	r0, r5
	add	r3, r3, #1
	mov	r3, r3, asl #1
	strh	r3, [r4, #172]	@ movhi
	bl	BsGet
	mov	r1, #1
	uxth	r3, r0
	mov	r0, r5
	add	r3, r3, r1
	mov	r3, r3, asl r1
	strh	r3, [r4, #174]	@ movhi
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #22]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #23]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #24]
	mov	r0, r5
	bl	BsGet
	mov	r1, r6
	strb	r0, [r4, #25]
	mov	r0, r5
	bl	BsSkip
	mov	r0, r5
	mov	r1, #1
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r4, #26]
	cmp	r6, #1
	beq	.L647
	ldrh	r2, [r4, #172]
	ldrh	r3, [r4, #174]
	strh	r2, [r4, #176]	@ movhi
	strh	r3, [r4, #178]	@ movhi
.L640:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #39]
	cmp	r0, #1
	beq	.L641
.L643:
	mov	r0, r5
	bl	BsPos
	ldr	r3, [r4, #184]
	mov	r2, #1
	rsb	r0, r3, r0
	str	r0, [r4, #184]
.L642:
	mov	r0, r2
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L641:
	mov	r1, #5
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #40]
	cmp	r0, #31
	movhi	r2, #0
	bhi	.L642
	mov	r1, #4
	mov	r0, r5
	bl	BsGet
	mov	r1, #4
	strb	r0, [r4, #41]
	mov	r0, r5
	bl	BsGet
	ldrb	r3, [r4, #40]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #42]
	ble	.L643
	add	r7, r4, #42
	mov	r6, #0
.L644:
	mov	r1, #16
	mov	r0, r5
	bl	BsGet
	mov	r1, #16
	add	r6, r6, #1
	strh	r0, [r7, #2]!	@ movhi
	mov	r0, r5
	bl	BsGet
	strh	r0, [r7, #64]	@ movhi
	ldrb	r3, [r4, #40]	@ zero_extendqisi2
	cmp	r3, r6
	bgt	.L644
	b	.L643
.L647:
	mov	r1, #14
	mov	r0, r5
	bl	BsGet
	mov	r1, #14
	add	r3, r0, #1
	mov	r0, r5
	strh	r3, [r4, #176]	@ movhi
	bl	BsGet
	mov	r1, r6
	add	r3, r0, #1
	mov	r0, r5
	strh	r3, [r4, #178]	@ movhi
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #27]
	cmp	r0, #1
	beq	.L648
.L636:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r1, r0
	strb	r1, [r4, #31]
	cmp	r1, #1
	beq	.L649
.L638:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #35]
	cmp	r0, #1
	bne	.L640
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	mov	r1, #8
	strb	r0, [r4, #36]
	mov	r0, r5
	bl	BsGet
	mov	r1, #8
	strb	r0, [r4, #37]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #38]
	b	.L640
.L649:
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #32]
	cmp	r0, #0
	bne	.L639
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	mov	r1, #4
	strb	r0, [r4, #33]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #34]
	b	.L638
.L648:
	mov	r1, #4
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #28]
	cmp	r0, #15
	bne	.L636
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	mov	r1, #8
	strb	r0, [r4, #29]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #30]
	b	.L636
.L639:
	mov	r1, #16
	mov	r0, r5
	bl	BsGet
	strh	r0, [r4, #180]	@ movhi
	b	.L638
.L651:
	.align	2
.L650:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvSeqHdr, .-VC1_Vfmw_ParseAdvSeqHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvEntptHdr
	.type	VC1_Vfmw_ParseAdvEntptHdr, %function
VC1_Vfmw_ParseAdvEntptHdr:
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
	ldr	r3, .L677
	mov	r4, r2
	mov	r5, r0
	mov	r1, #0
	add	r0, r2, #188
	mov	r2, #60
	ldr	r3, [r3, #48]
	blx	r3
	mov	r0, r5
	bl	BsPos
	mov	r1, #1
	str	r0, [r4, #244]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #188]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #189]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #190]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #191]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #192]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #193]
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #194]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #195]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #196]
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #197]
	mov	r0, r5
	bl	BsGet
	ldrb	r3, [r4, #39]	@ zero_extendqisi2
	cmp	r3, #1
	strb	r0, [r4, #198]
	beq	.L653
.L656:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #199]
	cmp	r0, #1
	ldrneh	r2, [r4, #240]
	beq	.L673
.L658:
	ldrh	r1, [r4, #238]
	ldrh	r0, [r4, #172]
	cmp	r1, #15
	movhi	r3, #0
	movls	r3, #1
	cmp	r1, r0
	orrhi	r3, r3, #1
	ldrb	r1, [r4, #194]	@ zero_extendqisi2
	cmp	r3, #0
	ldrh	r3, [r4, #174]
	strneh	r0, [r4, #238]	@ movhi
	cmp	r3, r2
	cmpcs	r2, #15
	strlsh	r3, [r4, #240]	@ movhi
	cmp	r1, #1
	beq	.L674
.L661:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #201]
	cmp	r0, #1
	beq	.L675
.L662:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #203]
	cmp	r0, #1
	beq	.L676
.L663:
	mov	r0, r5
	bl	BsPos
	ldr	r3, [r4, #244]
	rsb	r3, r3, r0
	mov	r0, #1
	str	r3, [r4, #244]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L653:
	ldrb	r3, [r4, #40]	@ zero_extendqisi2
	cmp	r3, #0
	ble	.L656
	add	r7, r4, #204
	mov	r6, #0
.L657:
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	add	r6, r6, #1
	strb	r0, [r7, #1]!
	ldrb	r3, [r4, #40]	@ zero_extendqisi2
	cmp	r3, r6
	bgt	.L657
	b	.L656
.L676:
	mov	r1, #3
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #204]
	b	.L663
.L673:
	mov	r1, #12
	mov	r0, r5
	bl	BsGet
	mov	r1, #12
	add	r3, r0, #1
	mov	r0, r5
	mov	r3, r3, asl #1
	strh	r3, [r4, #238]	@ movhi
	bl	BsGet
	add	r0, r0, #1
	mov	r2, r0, asl #1
	uxth	r2, r2
	strh	r2, [r4, #240]	@ movhi
	b	.L658
.L674:
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #200]
	b	.L661
.L675:
	mov	r1, #3
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #202]
	b	.L662
.L678:
	.align	2
.L677:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvEntptHdr, .-VC1_Vfmw_ParseAdvEntptHdr
	.align	2
	.global	VC1_DecFcm
	.type	VC1_DecFcm, %function
VC1_DecFcm:
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
	mov	r1, #2
	mov	r5, r0
	bl	BsShow
	mov	r4, r0
	cmp	r4, #1
	mov	r0, r5
	ble	.L682
	mov	r1, #2
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L682:
	mov	r1, #1
	mov	r4, #0
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_DecFcm, .-VC1_DecFcm
	.align	2
	.global	VC1_DecAdvPtype
	.type	VC1_DecAdvPtype, %function
VC1_DecAdvPtype:
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
	mov	r1, #4
	mov	r4, r0
	bl	BsShow
	cmp	r0, #7
	movle	r1, #1
	movle	r5, r1
	ble	.L684
	cmp	r0, #11
	movle	r1, #2
	movle	r5, r1
	bgt	.L689
.L684:
	mov	r0, r4
	bl	BsSkip
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L689:
	cmp	r0, #13
	movle	r1, #3
	movle	r5, #0
	ble	.L684
	cmp	r0, #14
	mov	r1, #4
	movne	r5, #5
	moveq	r5, r1
	b	.L684
	UNWIND(.fnend)
	.size	VC1_DecAdvPtype, .-VC1_DecAdvPtype
	.align	2
	.global	VC1_Vfmw_ParseAdvIPicHdr
	.type	VC1_Vfmw_ParseAdvIPicHdr, %function
VC1_Vfmw_ParseAdvIPicHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r8, r1
	mov	r1, #1
	mov	r4, r2
	mov	r6, r0
	bl	BsGet
	ldrb	r1, [r4, #23]	@ zero_extendqisi2
	cmp	r1, #1
	strb	r0, [r4, #348]
	beq	.L706
.L691:
	ldr	r3, [r4, #392]
	cmp	r3, #0
	bne	.L692
	ldrb	r1, [r4, #25]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L707
.L692:
	mov	r1, #5
	mov	r0, r6
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r4, #358]
	cmp	r5, #0
	beq	.L701
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L711
	sub	r0, fp, #164
	bl	memcpy
	cmp	r7, #0
	mov	r3, r5
	subeq	r3, fp, #36
	addeq	r3, r3, r5, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r5, #8
	str	r3, [r4, #464]
	movhi	r3, #0
	strhib	r3, [r4, #351]
	bls	.L708
.L697:
	cmp	r7, #1
	movne	r3, #0
	strneb	r3, [r4, #359]
	beq	.L709
.L699:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L710
.L700:
	mov	r1, r8
	mov	r3, #0
	mov	r2, r4
	mov	r0, r6
	bl	BPD_Drv
	cmp	r0, #1
	beq	.L701
	mov	r0, r6
	bl	VC1_DecTransacfrm
	strb	r0, [r4, #370]
	mov	r0, r6
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #371]
	mov	r0, r6
	bl	BsGet
	ldrb	r1, [r4, #195]	@ zero_extendqisi2
	cmp	r1, #0
	strb	r0, [r4, #372]
	moveq	r0, #1
	beq	.L694
	mov	r0, r6
	add	r3, r4, #508
	ldrb	r2, [r4, #464]	@ zero_extendqisi2
	bl	VC1_VopDQuant
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L701:
	mov	r0, #0
.L694:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L708:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	strb	r0, [r4, #351]
	b	.L697
.L707:
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #357]
	b	.L692
.L706:
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #356]
	b	.L691
.L709:
	mov	r1, r7
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #359]
	b	.L699
.L710:
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #360]
	b	.L700
.L712:
	.align	2
.L711:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvIPicHdr, .-VC1_Vfmw_ParseAdvIPicHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvPPicHdr
	.type	VC1_Vfmw_ParseAdvPPicHdr, %function
VC1_Vfmw_ParseAdvPPicHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r8, r1
	mov	r1, #1
	mov	r4, r2
	mov	r5, r0
	bl	BsGet
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	cmp	r3, #1
	strb	r0, [r4, #348]
	beq	.L714
	ldr	r3, [r4, #392]
	cmp	r3, #2
	beq	.L714
	ldrb	r1, [r4, #25]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L751
.L716:
	mov	r1, #5
	mov	r0, r5
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r4, #358]
	cmp	r6, #0
	beq	.L731
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L764
	sub	r0, fp, #164
	bl	memcpy
	cmp	r7, #0
	mov	r3, r6
	subeq	r3, fp, #36
	addeq	r3, r3, r6, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r6, #8
	str	r3, [r4, #464]
	movhi	r3, #0
	strhib	r3, [r4, #351]
	bls	.L752
.L721:
	cmp	r7, #1
	movne	r3, #0
	strneb	r3, [r4, #359]
	beq	.L753
.L723:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L754
.L724:
	ldrb	r3, [r4, #194]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L755
.L725:
	ldr	r3, [r4, #392]
	cmp	r3, #0
	beq	.L756
	ldrb	r3, [r4, #200]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r3, #0
	strneb	r3, [r4, #373]
	beq	.L757
.L729:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #374]
	mov	r0, r5
	bl	BsGet
	cmp	r0, #1
	str	r0, [r4, #468]
	beq	.L758
.L730:
	mov	r3, #0
	strb	r3, [r4, #382]
.L727:
	mov	r1, r8
	mov	r3, #0
	mov	r2, r4
	mov	r0, r5
	bl	BPD_Drv
	cmp	r0, #1
	beq	.L731
	ldr	r1, [r4, #392]
	cmp	r1, #2
	beq	.L759
.L732:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	ldr	r3, [r4, #392]
	cmp	r3, #0
	moveq	r1, #2
	movne	r1, #3
	strb	r0, [r4, #366]
	mov	r0, r5
	bl	BsGet
	ldr	r6, [r4, #392]
	cmp	r6, #2
	uxtb	r0, r0
	strb	r0, [r4, #367]
	beq	.L760
.L735:
	ldrb	r1, [r4, #195]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L761
.L737:
	ldrb	r1, [r4, #196]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L762
.L739:
	mov	r0, r5
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #370]
	mov	r0, r5
	bl	BsGet
	mov	r3, #1
	strb	r0, [r4, #372]
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L731:
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L714:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	ldrb	r1, [r4, #25]	@ zero_extendqisi2
	cmp	r1, #1
	strb	r0, [r4, #356]
	bne	.L716
.L751:
	ldr	r2, [r4, #392]
	cmp	r2, #0
	bne	.L716
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #357]
	b	.L716
.L756:
	ldrb	r1, [r4, #464]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_DecMvmodeProgrP
	uxtb	r0, r0
	strb	r0, [r4, #362]
	cmp	r0, #4
	bne	.L727
	ldrb	r1, [r4, #464]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_DecMvmode2
	mov	r1, #6
	strb	r0, [r4, #363]
	mov	r0, r5
	bl	BsGet
	mov	r1, #6
	strb	r0, [r4, #364]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #365]
	b	.L727
.L752:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	strb	r0, [r4, #351]
	b	.L721
.L761:
	add	r3, r4, #508
	ldrb	r2, [r4, #464]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_VopDQuant
	b	.L737
.L755:
	mov	r0, r5
	bl	VC1_DecMvrange
	strb	r0, [r4, #361]
	b	.L725
.L753:
	mov	r1, r7
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #359]
	b	.L723
.L754:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #360]
	b	.L724
.L762:
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #368]
	cmp	r0, #1
	bne	.L739
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #369]
	b	.L739
.L760:
	mov	r1, r6
	mov	r0, r5
	bl	BsGet
	ldrb	r3, [r4, #374]	@ zero_extendqisi2
	cmp	r3, #1
	strb	r0, [r4, #376]
	movne	r0, #0
	beq	.L763
.L736:
	strb	r0, [r4, #377]
	b	.L735
.L759:
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #375]
	b	.L732
.L757:
	mov	r0, r5
	bl	VC1_DecMvrange
	strb	r0, [r4, #373]
	b	.L729
.L758:
	mov	r1, #6
	mov	r0, r5
	bl	BsGet
	mov	r1, #6
	strb	r0, [r4, #364]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #365]
	b	.L730
.L763:
	mov	r1, r6
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	b	.L736
.L765:
	.align	2
.L764:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvPPicHdr, .-VC1_Vfmw_ParseAdvPPicHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvBPicHdr
	.type	VC1_Vfmw_ParseAdvBPicHdr, %function
VC1_Vfmw_ParseAdvBPicHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r7, r1
	mov	r1, #1
	mov	r4, r2
	mov	r5, r0
	bl	BsGet
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	cmp	r3, #1
	strb	r0, [r4, #348]
	beq	.L767
	ldr	r3, [r4, #392]
	cmp	r3, #2
	beq	.L767
	ldrb	r1, [r4, #25]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L809
.L769:
	cmp	r3, #0
	bne	.L773
	add	r1, r4, #496
	mov	r0, r5
	bl	VC1_DecBfraction
	uxtb	r0, r0
	strb	r0, [r4, #350]
	cmp	r0, #21
	beq	.L771
.L773:
	mov	r1, #5
	mov	r0, r5
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r4, #358]
	cmp	r6, #0
	beq	.L771
	ldrb	r8, [r4, #198]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L821
	sub	r0, fp, #164
	bl	memcpy
	cmp	r8, #0
	mov	r3, r6
	subeq	r3, fp, #36
	addeq	r3, r3, r6, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r6, #8
	str	r3, [r4, #464]
	movhi	r3, #0
	strhib	r3, [r4, #351]
	bls	.L810
	cmp	r8, #1
	movne	r3, #0
	strneb	r3, [r4, #359]
	beq	.L811
.L779:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L812
.L780:
	ldr	r3, [r4, #392]
	cmp	r3, #2
	beq	.L813
.L781:
	ldrb	r3, [r4, #194]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L814
.L782:
	ldr	r3, [r4, #392]
	cmp	r3, #0
	beq	.L815
	ldrb	r3, [r4, #200]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L816
.L786:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	str	r0, [r4, #468]
.L785:
	mov	r1, r7
	mov	r3, #0
	mov	r2, r4
	mov	r0, r5
	bl	BPD_Drv
	cmp	r0, #1
	beq	.L771
	ldr	r1, [r4, #392]
	cmp	r1, #2
	beq	.L817
.L787:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	ldr	r3, [r4, #392]
	cmp	r3, #0
	moveq	r1, #2
	movne	r1, #3
	strb	r0, [r4, #366]
	mov	r0, r5
	bl	BsGet
	ldr	r6, [r4, #392]
	cmp	r6, #2
	uxtb	r0, r0
	strb	r0, [r4, #367]
	beq	.L818
.L790:
	ldrb	r1, [r4, #195]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L819
.L791:
	ldrb	r1, [r4, #196]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L820
.L793:
	mov	r0, r5
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #370]
	mov	r0, r5
	bl	BsGet
	mov	r3, #1
	strb	r0, [r4, #372]
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L771:
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L767:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	ldrb	r1, [r4, #25]	@ zero_extendqisi2
	ldr	r3, [r4, #392]
	cmp	r1, #1
	strb	r0, [r4, #356]
	bne	.L769
.L809:
	cmp	r3, #0
	bne	.L773
	mov	r0, r5
	bl	BsGet
	ldr	r3, [r4, #392]
	strb	r0, [r4, #357]
	b	.L769
.L815:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	moveq	r3, #3
	movne	r3, #1
	strb	r3, [r4, #362]
	b	.L785
.L810:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	ldrb	r8, [r4, #198]	@ zero_extendqisi2
	cmp	r8, #1
	movne	r3, #0
	strb	r0, [r4, #351]
	strneb	r3, [r4, #359]
	bne	.L779
.L811:
	mov	r1, r8
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #359]
	b	.L779
.L813:
	add	r1, r4, #496
	mov	r0, r5
	bl	VC1_DecBfraction
	strb	r0, [r4, #350]
	b	.L781
.L814:
	mov	r0, r5
	bl	VC1_DecMvrange
	strb	r0, [r4, #361]
	b	.L782
.L812:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #360]
	b	.L780
.L819:
	add	r3, r4, #508
	ldrb	r2, [r4, #464]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_VopDQuant
	b	.L791
.L820:
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #368]
	cmp	r0, #1
	bne	.L793
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #369]
	b	.L793
.L818:
	mov	r1, r6
	mov	r0, r5
	bl	BsGet
	mov	r1, r6
	strb	r0, [r4, #376]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #377]
	b	.L790
.L817:
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #375]
	b	.L787
.L816:
	mov	r0, r5
	bl	VC1_DecMvrange
	strb	r0, [r4, #373]
	b	.L786
.L822:
	.align	2
.L821:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvBPicHdr, .-VC1_Vfmw_ParseAdvBPicHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvFramePicHdr
	.type	VC1_Vfmw_ParseAdvFramePicHdr, %function
VC1_Vfmw_ParseAdvFramePicHdr:
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
	mov	r6, r1
	mov	r1, #4
	mov	r4, r2
	mov	r5, r0
	bl	BsShow
	cmp	r0, #7
	movle	r7, #1
	movle	r1, r7
	ble	.L824
	cmp	r0, #11
	movle	r7, #2
	movle	r1, r7
	bgt	.L855
.L824:
	mov	r0, r5
	bl	BsSkip
	strb	r7, [r4, #349]
	ldrb	r3, [r6, #3513]	@ zero_extendqisi2
	strb	r3, [r6, #2]
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L856
.L825:
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	beq	.L826
	cmp	r3, #0
	beq	.L857
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #352]
	mov	r0, r5
	bl	BsGet
	ldrb	r2, [r4, #23]	@ zero_extendqisi2
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r2, #1
	strb	r0, [r4, #353]
	beq	.L858
.L830:
	cmp	r3, #1
	bne	.L832
	ldrb	r1, [r4, #190]	@ zero_extendqisi2
	ldrb	r3, [r4, #354]	@ zero_extendqisi2
	cmp	r1, #1
	add	r3, r3, #1
	str	r3, [r4, #492]
	bne	.L841
.L833:
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #355]
	cmp	r0, #1
	bne	.L841
	ldr	r3, [r4, #492]
	cmp	r3, #0
	ble	.L841
	mov	r7, r4
	mov	r8, #0
.L842:
	mov	r1, #18
	mov	r0, r5
	bl	BsGet
	mov	r1, #18
	add	r8, r8, #1
	add	r7, r7, #4
	str	r0, [r7, #396]
	mov	r0, r5
	bl	BsGet
	mov	r1, #14
	str	r0, [r7, #412]
	mov	r0, r5
	bl	BsGet
	mov	r1, #14
	str	r0, [r7, #428]
	mov	r0, r5
	bl	BsGet
	str	r0, [r7, #444]
	ldr	r3, [r4, #492]
	cmp	r3, r8
	bgt	.L842
	b	.L841
.L826:
	mov	r2, #1
	cmp	r3, r2
	strb	r2, [r4, #352]
	beq	.L829
.L832:
	mov	r3, #1
	str	r3, [r4, #492]
.L831:
	ldrb	r1, [r4, #190]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L833
.L841:
	ldrb	r3, [r4, #349]	@ zero_extendqisi2
	cmp	r3, #5
	ldrls	pc, [pc, r3, asl #2]
	b	.L834
.L836:
	.word	.L839
	.word	.L837
	.word	.L838
	.word	.L834
	.word	.L839
	.word	.L847
.L839:
	mov	r2, r4
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvIPicHdr
.L857:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	ldrb	r2, [r4, #23]	@ zero_extendqisi2
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r2, #1
	strb	r0, [r4, #354]
	bne	.L830
.L858:
	cmp	r3, #1
	beq	.L859
.L829:
	mov	r3, #2
	str	r3, [r4, #492]
	b	.L831
.L855:
	cmp	r0, #13
	movle	r7, #0
	movle	r1, #3
	ble	.L824
	cmp	r0, #14
	moveq	r7, #4
	movne	r7, #5
	movne	r1, #4
	moveq	r1, r7
	b	.L824
.L856:
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	str	r0, [r4, #396]
	b	.L825
.L847:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L837:
	mov	r2, r4
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvPPicHdr
.L838:
	mov	r2, r4
	mov	r1, r6
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvBPicHdr
.L834:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L859:
	ldrb	r3, [r4, #353]	@ zero_extendqisi2
	add	r3, r3, #2
	str	r3, [r4, #492]
	b	.L831
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvFramePicHdr, .-VC1_Vfmw_ParseAdvFramePicHdr
	.align	2
	.global	VC1_DecRefdist
	.type	VC1_DecRefdist, %function
VC1_DecRefdist:
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
	mov	r1, #5
	mov	r5, r0
	bl	BsShow
	cmp	r0, #23
	bgt	.L861
	mov	r4, r0, asr #3
	mov	r1, #2
	mov	r0, r5
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L861:
	cmp	r0, #27
	movle	r4, #3
	bgt	.L867
.L863:
	mov	r0, r5
	mov	r1, r4
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L867:
	cmp	r0, #29
	movgt	r4, #5
	movle	r4, #4
	b	.L863
	UNWIND(.fnend)
	.size	VC1_DecRefdist, .-VC1_DecRefdist
	.align	2
	.global	VC1_DecIntCompField
	.type	VC1_DecIntCompField, %function
VC1_DecIntCompField:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	lr, .L869
	mov	r4, r0
	sub	ip, fp, #52
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	mov	r0, r4
	mov	r1, #2
	bl	BsShow
	sub	r3, fp, #20
	add	r3, r3, r0, lsl #3
	mov	r0, r4
	ldr	r1, [r3, #-28]
	ldr	r4, [r3, #-32]
	bl	BsSkip
	mov	r0, r4
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L870:
	.align	2
.L869:
	.word	.LANCHOR0+696
	UNWIND(.fnend)
	.size	VC1_DecIntCompField, .-VC1_DecIntCompField
	.align	2
	.global	VC1_Vfmw_ParseAdvIFieldHdr
	.type	VC1_Vfmw_ParseAdvIFieldHdr, %function
VC1_Vfmw_ParseAdvIFieldHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r8, r1
	mov	r1, #5
	mov	r4, r2
	mov	r6, r0
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r4, #358]
	cmp	r5, #0
	beq	.L880
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L888
	sub	r0, fp, #164
	bl	memcpy
	cmp	r7, #0
	mov	r3, r5
	subeq	r3, fp, #36
	addeq	r3, r3, r5, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r5, #8
	str	r3, [r4, #464]
	movhi	r3, #0
	strhib	r3, [r4, #351]
	bls	.L885
.L876:
	cmp	r7, #1
	movne	r3, #0
	strneb	r3, [r4, #359]
	beq	.L886
.L878:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L887
.L879:
	mov	r1, r8
	mov	r3, #0
	mov	r2, r4
	mov	r0, r6
	bl	BPD_Drv
	cmp	r0, #1
	beq	.L880
	mov	r0, r6
	bl	VC1_DecTransacfrm
	strb	r0, [r4, #370]
	mov	r0, r6
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #371]
	mov	r0, r6
	bl	BsGet
	ldrb	r1, [r4, #195]	@ zero_extendqisi2
	cmp	r1, #0
	strb	r0, [r4, #372]
	moveq	r0, #1
	beq	.L873
	mov	r0, r6
	add	r3, r4, #508
	ldrb	r2, [r4, #464]	@ zero_extendqisi2
	bl	VC1_VopDQuant
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L880:
	mov	r0, #0
.L873:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L885:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	strb	r0, [r4, #351]
	b	.L876
.L886:
	mov	r1, r7
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #359]
	b	.L878
.L887:
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #360]
	b	.L879
.L889:
	.align	2
.L888:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvIFieldHdr, .-VC1_Vfmw_ParseAdvIFieldHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvPFieldHdr
	.type	VC1_Vfmw_ParseAdvPFieldHdr, %function
VC1_Vfmw_ParseAdvPFieldHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #128)
	sub	sp, sp, #128
	mov	r1, #5
	mov	r4, r2
	mov	r6, r0
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r4, #358]
	cmp	r5, #0
	beq	.L891
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L928
	sub	r0, fp, #156
	bl	memcpy
	cmp	r7, #0
	mov	r3, r5
	subeq	r3, fp, #28
	addeq	r3, r3, r5, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r5, #8
	str	r3, [r4, #464]
	movhi	r3, #0
	strhib	r3, [r4, #351]
	bls	.L919
.L894:
	cmp	r7, #1
	movne	r3, #0
	strneb	r3, [r4, #359]
	beq	.L920
.L896:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L921
.L897:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #380]
	cmp	r0, #0
	beq	.L922
.L898:
	ldrb	r3, [r4, #194]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L923
.L899:
	ldrb	r3, [r4, #200]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L924
.L900:
	ldrb	r1, [r4, #464]	@ zero_extendqisi2
	mov	r0, r6
	bl	VC1_DecMvmodeProgrP
	uxtb	r0, r0
	strb	r0, [r4, #362]
	cmp	r0, #4
	beq	.L925
.L902:
	mov	r1, #3
	mov	r0, r6
	bl	BsGet
	ldrb	r3, [r4, #380]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r1, #2
	movne	r1, #3
	strb	r0, [r4, #375]
	mov	r0, r6
	bl	BsGet
	mov	r1, #3
	uxtb	r0, r0
	strb	r0, [r4, #366]
	mov	r0, r6
	bl	BsGet
	ldrb	r3, [r4, #362]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #367]
	beq	.L906
	movw	r3, #362
	ldrh	r3, [r4, r3]
	cmp	r3, #4
	beq	.L906
	ldrb	r1, [r4, #195]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L926
.L908:
	ldrb	r1, [r4, #196]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L927
.L910:
	mov	r0, r6
	mov	r5, #1
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #370]
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #372]
.L891:
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L906:
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	ldrb	r1, [r4, #195]	@ zero_extendqisi2
	cmp	r1, #0
	strb	r0, [r4, #377]
	beq	.L908
.L926:
	add	r3, r4, #508
	ldrb	r2, [r4, #464]	@ zero_extendqisi2
	mov	r0, r6
	bl	VC1_VopDQuant
	b	.L908
.L922:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	clz	r0, r0
	mov	r0, r0, lsr #5
	strb	r0, [r4, #381]
	b	.L898
.L919:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	strb	r0, [r4, #351]
	b	.L894
.L923:
	mov	r0, r6
	bl	VC1_DecMvrange
	strb	r0, [r4, #361]
	b	.L899
.L924:
	mov	r0, r6
	bl	VC1_DecMvrange
	strb	r0, [r4, #373]
	b	.L900
.L920:
	mov	r1, r7
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #359]
	b	.L896
.L927:
	mov	r0, r6
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #368]
	cmp	r0, #1
	bne	.L910
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #369]
	b	.L910
.L921:
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	strb	r0, [r4, #360]
	b	.L897
.L925:
	ldrb	r1, [r4, #464]	@ zero_extendqisi2
	mov	r0, r6
	bl	VC1_DecMvmode2
	strb	r0, [r4, #363]
	mov	r0, r6
	bl	VC1_DecIntCompField
	mov	r1, #6
	str	r0, [r4, #476]
	mov	r0, r6
	bl	BsGet
	mov	r1, #6
	strb	r0, [r4, #364]
	mov	r0, r6
	bl	BsGet
	ldr	r3, [r4, #476]
	cmp	r3, #0
	strb	r0, [r4, #365]
	bne	.L902
	mov	r1, #6
	mov	r0, r6
	bl	BsGet
	mov	r1, #6
	str	r0, [r4, #480]
	mov	r0, r6
	bl	BsGet
	str	r0, [r4, #484]
	b	.L902
.L929:
	.align	2
.L928:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvPFieldHdr, .-VC1_Vfmw_ParseAdvPFieldHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvBFieldHdr
	.type	VC1_Vfmw_ParseAdvBFieldHdr, %function
VC1_Vfmw_ParseAdvBFieldHdr:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r3, #1
	mov	r8, r1
	strb	r3, [r2, #380]
	mov	r1, #5
	mov	r4, r2
	mov	r5, r0
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r4, #358]
	cmp	r6, #0
	beq	.L941
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	mov	r2, #128
	ldr	r1, .L961
	sub	r0, fp, #164
	bl	memcpy
	cmp	r7, #0
	mov	r3, r6
	subeq	r3, fp, #36
	addeq	r3, r3, r6, lsl #2
	ldreq	r3, [r3, #-128]
	cmp	r6, #8
	str	r3, [r4, #464]
	movhi	r3, #0
	strhib	r3, [r4, #351]
	bls	.L953
.L935:
	cmp	r7, #1
	movne	r3, #0
	strneb	r3, [r4, #359]
	beq	.L954
.L937:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L955
.L938:
	ldrb	r3, [r4, #194]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L956
.L939:
	ldrb	r3, [r4, #200]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L957
.L940:
	ldrb	r1, [r4, #464]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_DecMvmodeInterB
	mov	r1, r8
	mov	r3, #0
	mov	r2, r4
	strb	r0, [r4, #362]
	mov	r0, r5
	bl	BPD_Drv
	cmp	r0, #1
	beq	.L941
	mov	r1, #3
	mov	r0, r5
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #375]
	mov	r0, r5
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #366]
	mov	r0, r5
	bl	BsGet
	ldrb	r3, [r4, #362]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #367]
	beq	.L958
.L942:
	ldrb	r1, [r4, #195]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L959
.L943:
	ldrb	r1, [r4, #196]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L960
.L945:
	mov	r0, r5
	bl	VC1_DecTransacfrm
	mov	r1, #1
	strb	r0, [r4, #370]
	mov	r0, r5
	bl	BsGet
	mov	r3, #1
	strb	r0, [r4, #372]
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L941:
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L953:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	ldrb	r7, [r4, #198]	@ zero_extendqisi2
	strb	r0, [r4, #351]
	b	.L935
.L959:
	add	r3, r4, #508
	ldrb	r2, [r4, #464]	@ zero_extendqisi2
	mov	r0, r5
	bl	VC1_VopDQuant
	b	.L943
.L958:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #377]
	b	.L942
.L955:
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #360]
	b	.L938
.L956:
	mov	r0, r5
	bl	VC1_DecMvrange
	strb	r0, [r4, #361]
	b	.L939
.L954:
	mov	r1, r7
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #359]
	b	.L937
.L957:
	mov	r0, r5
	bl	VC1_DecMvrange
	strb	r0, [r4, #373]
	b	.L940
.L960:
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #368]
	cmp	r0, #1
	bne	.L945
	mov	r1, #2
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #369]
	b	.L945
.L962:
	.align	2
.L961:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvBFieldHdr, .-VC1_Vfmw_ParseAdvBFieldHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvFieldPicHdr
	.type	VC1_Vfmw_ParseAdvFieldPicHdr, %function
VC1_Vfmw_ParseAdvFieldPicHdr:
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
	ldrb	r3, [r1, #8]	@ zero_extendqisi2
	mov	r5, r1
	mov	r6, r0
	mov	r4, r2
	cmp	r3, #0
	beq	.L964
	add	r3, r1, #20480
	ldr	r3, [r3, #2836]
	cmp	r3, #0
	ble	.L965
.L964:
	mov	r1, #3
	mov	r0, r6
	bl	BsGet
	ldr	r3, .L1009
	str	r0, [r4, #472]
	add	r0, r3, r0, lsl #3
	ldr	r3, [r0, #732]
	ldr	r2, [r0, #728]
	str	r3, [r4, #504]
	str	r2, [r4, #500]
	ldrb	r1, [r5, #8]	@ zero_extendqisi2
	cmp	r1, #0
	uxtbeq	r3, r2
	uxtbne	r3, r3
	strb	r3, [r4, #349]
	ldrb	r3, [r5, #3513]	@ zero_extendqisi2
	strb	r3, [r5, #2]
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1003
.L968:
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	bne	.L1004
	mov	r2, #1
	cmp	r3, r2
	strb	r2, [r4, #352]
	beq	.L972
.L975:
	mov	r3, #1
	str	r3, [r4, #492]
.L974:
	ldrb	r1, [r4, #190]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L976
.L979:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #348]
	mov	r0, r6
	bl	BsGet
	ldrb	r3, [r4, #191]	@ zero_extendqisi2
	cmp	r3, #1
	strb	r0, [r4, #356]
	beq	.L1005
.L978:
	ldr	r3, [r4, #472]
	cmp	r3, #3
	bgt	.L983
.L982:
	ldrb	r3, [r4, #349]	@ zero_extendqisi2
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L991
.L986:
	.word	.L985
	.word	.L987
	.word	.L988
	.word	.L991
	.word	.L985
.L1004:
	cmp	r3, #0
	bne	.L970
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	strb	r0, [r4, #354]
.L971:
	ldrb	r2, [r4, #23]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1006
	cmp	r3, #1
	bne	.L975
	ldrb	r1, [r4, #190]	@ zero_extendqisi2
	ldrb	r3, [r4, #354]	@ zero_extendqisi2
	cmp	r1, #1
	add	r3, r3, #1
	str	r3, [r4, #492]
	bne	.L979
.L976:
	mov	r0, r6
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #355]
	cmp	r0, #1
	bne	.L979
	ldr	r3, [r4, #492]
	cmp	r3, #0
	ble	.L979
	mov	r7, r4
	mov	r8, #0
.L980:
	mov	r1, #18
	mov	r0, r6
	bl	BsGet
	mov	r1, #18
	add	r8, r8, #1
	add	r7, r7, #4
	str	r0, [r7, #396]
	mov	r0, r6
	bl	BsGet
	mov	r1, #14
	str	r0, [r7, #412]
	mov	r0, r6
	bl	BsGet
	mov	r1, #14
	str	r0, [r7, #428]
	mov	r0, r6
	bl	BsGet
	str	r0, [r7, #444]
	ldr	r3, [r4, #492]
	cmp	r3, r8
	bgt	.L980
	b	.L979
.L988:
	mov	r0, r6
	mov	r2, r4
	mov	r1, r5
	bl	VC1_Vfmw_ParseAdvBFieldHdr
.L984:
	ldrb	r3, [r4, #352]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	bne	.L1007
.L989:
	subs	r3, r3, #1
	movne	r3, #1
	strb	r3, [r4, #389]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L987:
	mov	r0, r6
	mov	r2, r4
	mov	r1, r5
	bl	VC1_Vfmw_ParseAdvPFieldHdr
	ldrb	r3, [r4, #352]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	beq	.L989
.L1007:
	adds	r3, r3, #0
	movne	r3, #1
	strb	r3, [r4, #389]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L985:
	mov	r0, r6
	mov	r2, r4
	mov	r1, r5
	bl	VC1_Vfmw_ParseAdvIFieldHdr
	b	.L984
.L1005:
	ldr	r3, [r4, #472]
	cmp	r3, #3
	ble	.L981
.L983:
	add	r1, r4, #496
	mov	r0, r6
	bl	VC1_DecBfraction
	strb	r0, [r4, #350]
	b	.L982
.L1003:
	mov	r1, #8
	mov	r0, r6
	bl	BsGet
	str	r0, [r4, #396]
	b	.L968
.L1006:
	cmp	r3, #1
	beq	.L1008
.L972:
	mov	r3, #2
	str	r3, [r4, #492]
	b	.L974
.L970:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #352]
	mov	r0, r6
	bl	BsGet
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	strb	r0, [r4, #353]
	b	.L971
.L991:
	mov	r0, #0
	b	.L984
.L981:
	mov	r0, r6
	bl	VC1_DecRefdist
	strb	r0, [r4, #379]
	b	.L978
.L1008:
	ldrb	r3, [r4, #353]	@ zero_extendqisi2
	add	r3, r3, #2
	str	r3, [r4, #492]
	b	.L974
.L965:
	ldr	r3, [r2, #504]
	strb	r3, [r2, #349]
	ldrb	r3, [r1, #3513]	@ zero_extendqisi2
	strb	r3, [r1, #2]
	b	.L982
.L1010:
	.align	2
.L1009:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvFieldPicHdr, .-VC1_Vfmw_ParseAdvFieldPicHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvPicHdr
	.type	VC1_Vfmw_ParseAdvPicHdr, %function
VC1_Vfmw_ParseAdvPicHdr:
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
	mov	r4, r1
	mov	r6, r0
	mov	r1, #0
	mov	r0, r4
	mov	r5, r2
	bl	VC1_SetCurPicResolutionInfo
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1012
	add	r3, r4, #20480
	ldr	r3, [r3, #2836]
	cmp	r3, #0
	ldrle	r3, [r5, #392]
	bicle	r3, r3, #2
	ble	.L1013
.L1012:
	ldrb	r8, [r5, #23]	@ zero_extendqisi2
	cmp	r8, #1
	movne	r3, #0
	movne	r0, r3
	beq	.L1021
.L1014:
	str	r0, [r5, #392]
.L1013:
	cmp	r3, #0
	mov	r2, r5
	mov	r1, r4
	mov	r0, r6
	beq	.L1022
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvFieldPicHdr
.L1022:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvFramePicHdr
.L1021:
	mov	r1, #2
	mov	r0, r6
	bl	BsShow
	cmp	r0, #1
	mov	r7, r0
	ble	.L1023
	mov	r1, #2
	mov	r0, r6
	bl	BsSkip
	bic	r3, r7, #2
	mov	r0, r7
	b	.L1014
.L1023:
	mov	r1, r8
	mov	r0, r6
	bl	BsSkip
	mov	r3, #0
	mov	r0, r3
	b	.L1014
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvPicHdr, .-VC1_Vfmw_ParseAdvPicHdr
	.align	2
	.global	VC1_Vfmw_ParseAdvSlcHdr
	.type	VC1_Vfmw_ParseAdvSlcHdr, %function
VC1_Vfmw_ParseAdvSlcHdr:
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
	add	r3, r1, #20480
	mov	r7, r1
	mov	r1, #9
	mov	r5, r2
	ldr	r4, [r3, #2836]
	mov	r6, r0
	bl	BsGet
	mov	r1, #1
	mov	r3, r4, asl #4
	sub	r4, r3, r4, asl #2
	add	r4, r5, r4
	add	r4, r4, #520
	str	r0, [r4, #8]
	mov	r0, r6
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #4]
	cmp	r0, #1
	beq	.L1026
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1026:
	add	lr, r5, #188
	add	ip, r7, #6912
	add	ip, ip, #36
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	add	r0, r7, #6720
	add	r1, r5, #16
	mov	r2, #172
	add	r0, r0, #56
	bl	memcpy
	add	r0, r7, #7104
	add	r1, r5, #348
	mov	r2, #176
	add	r0, r0, #4
	bl	memcpy
	add	r2, r7, #6720
	mov	r0, r6
	add	r2, r2, #40
	mov	r1, r7
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvPicHdr
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvSlcHdr, .-VC1_Vfmw_ParseAdvSlcHdr
	.align	2
	.global	VC1_Vfmw_next_start_code
	.type	VC1_Vfmw_next_start_code, %function
VC1_Vfmw_next_start_code:
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
	cmp	r6, #3
	subgt	r6, r6, #3
	movgt	r4, #0
	bgt	.L1030
	b	.L1031
.L1036:
	bl	BsSkip
	cmp	r4, r6
	beq	.L1031
.L1030:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r5
	bne	.L1036
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1031:
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_Vfmw_next_start_code, .-VC1_Vfmw_next_start_code
	.align	2
	.global	VC1_Vfmw_UserData
	.type	VC1_Vfmw_UserData, %function
VC1_Vfmw_UserData:
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
	.size	VC1_Vfmw_UserData, .-VC1_Vfmw_UserData
	.align	2
	.global	VC1_RecordSlcStream
	.type	VC1_RecordSlcStream, %function
VC1_RecordSlcStream:
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
	add	r9, r0, #20480
	mov	r5, r0
	ldr	r0, [r0, #3128]
	ldr	r6, [r9, #2836]
	movw	r3, #23328
	str	r1, [fp, #-48]
	movw	ip, #23320
	mov	r8, r6, asl #4
	mov	r7, r6, asl #6
	rsb	r4, r8, r7
	add	r4, r5, r4
	add	lr, r4, #23296
	strb	r6, [r4, r3]
	ldr	r3, [r0, #24]
	mov	r10, lr
	cmp	r3, #0
	add	r1, r3, #7
	movlt	r3, r1
	add	r1, lr, #24
	add	r2, r2, r3, asr #3
	str	r2, [r4, ip]
	ldr	r3, [r0, #24]
	movw	r2, #23336
	str	r1, [fp, #-52]
	and	r3, r3, #7
	str	r3, [r4, r2]
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #24]
	str	r1, [fp, #-56]
	rsb	r3, r2, r3, lsl #3
	str	r3, [lr, #44]
	ldr	r0, [r4, ip]
	bl	MEM_Vir2Phy
	add	r3, r5, #32768
	movw	ip, #23360
	add	r2, r10, #64
	str	r0, [r10, #36]
	ldr	r0, [r3, #2872]
	str	r0, [r4, ip]
	ldr	r0, [r3, #2848]
	cmp	r0, #0
	beq	.L1039
	ldr	ip, [r3, #2860]
	ldr	r1, [fp, #-52]
	cmp	ip, #0
	bne	.L1058
.L1039:
	rsb	r3, r8, r7
	movw	ip, #23352
	add	r3, r5, r3
	mov	r2, #0
	add	r4, r3, #23296
	str	r2, [r4, #28]
	str	r2, [r3, ip]
	str	r2, [r4, #60]
	str	r2, [r4, #52]
	str	r2, [r4, #68]
.L1040:
	ldr	lr, [r9, #2756]
	sub	r6, r8, r6, asl #2
	ldr	ip, [r9, #2752]
	ldrb	r4, [r5, #8]	@ zero_extendqisi2
	mul	ip, ip, lr
	cmp	r4, #0
	sub	ip, ip, #1
	beq	.L1041
	ldr	r2, [fp, #-48]
	add	r3, lr, #1
	rsb	r1, r8, r7
	movw	r0, #23344
	add	r6, r2, r6
	add	r3, r3, r3, lsr #31
	add	r1, r5, r1
	add	r6, r6, #520
	ldr	r2, [r6, #8]
	sub	r3, r2, r3, asr #1
	str	r3, [r1, r0]
	cmp	ip, r3
	movcs	ip, #0
	movcc	ip, #1
	cmp	r3, #0
	movle	ip, #0
	cmp	ip, #0
	bne	.L1059
.L1042:
	rsb	r7, r8, r7
	add	r5, r5, r7
	add	r3, r5, #23296
	add	r3, r3, #40
	ldr	r3, [r3, #4]
	cmp	r3, #0
	ble	.L1044
	add	r3, r5, #23296
	add	r3, r3, #24
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1045
	add	r5, r5, #23296
	ldr	r3, [r5, #60]
	cmp	r3, #0
	ble	.L1044
.L1045:
	ldr	r3, [r9, #2836]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r9, #2836]
.L1043:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1041:
	ldr	r3, [fp, #-48]
	rsb	r2, r8, r7
	add	r2, r5, r2
	movw	r1, #23344
	add	r6, r3, r6
	add	r6, r6, #520
	ldr	r3, [r6, #8]
	cmp	ip, r3
	str	r3, [r2, r1]
	bcs	.L1042
	ldr	lr, .L1060
	mov	r0, #1
	ldr	r2, [r6, #8]
	ldr	r1, .L1060+4
	str	ip, [sp]
	ldr	ip, [lr, #68]
	blx	ip
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1058:
	movw	lr, #23352
	str	r0, [r1, #4]
	mov	r0, #0
	str	r0, [r4, lr]
	ldr	r0, [r3, #2868]
	add	r1, r4, #23296
	str	ip, [r1, #52]
	mov	r0, r0, asl #3
	str	r0, [r1, #60]
	ldr	r3, [r3, #2876]
	str	r3, [r2, #4]
	b	.L1040
.L1059:
	ldr	ip, .L1060
	mov	r0, #1
	ldr	r2, [r6, #8]
	ldr	r1, .L1060+8
	str	lr, [sp]
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	b	.L1043
.L1044:
	ldr	r3, .L1060
	mov	r0, #1
	ldr	r1, .L1060+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1061:
	.align	2
.L1060:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC48
	.word	.LC47
	.word	.LC49
	UNWIND(.fnend)
	.size	VC1_RecordSlcStream, .-VC1_RecordSlcStream
	.align	2
	.global	VC1_Vfmw_UpdateAdvSPS
	.type	VC1_Vfmw_UpdateAdvSPS, %function
VC1_Vfmw_UpdateAdvSPS:
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
	add	r5, r0, #8192
	movw	r3, #2402
	ldrh	r2, [r1, #238]
	mov	r4, r1
	ldrh	r3, [r5, r3]
	cmp	r3, r2
	beq	.L1063
	adds	r3, r2, #15
	add	r0, r0, #20480
	addmi	r3, r2, #30
	movw	r1, #2404
	mov	r3, r3, asr #4
	str	r3, [r0, #2752]
	ldrh	r3, [r4, #238]
	str	r3, [r0, #2768]
	ldrh	r2, [r4, #240]
	ldrh	r3, [r5, r1]
	cmp	r3, r2
	beq	.L1064
.L1065:
	adds	r3, r2, #15
	addmi	r3, r2, #30
	mov	r3, r3, asr #4
	str	r3, [r0, #2756]
	ldrh	r3, [r4, #240]
	str	r3, [r0, #2772]
.L1064:
	add	r0, r5, #2176
	add	r1, r4, #16
	mov	r2, #172
	add	r0, r0, #4
	bl	memcpy
	add	lr, r4, #188
	add	ip, r5, #2352
	mov	r4, #1
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
.L1068:
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1063:
	movw	r3, #2404
	ldrh	r2, [r1, #240]
	ldrh	r3, [r5, r3]
	cmp	r3, r2
	addne	r0, r0, #20480
	bne	.L1065
	mov	r4, #0
	b	.L1068
	UNWIND(.fnend)
	.size	VC1_Vfmw_UpdateAdvSPS, .-VC1_Vfmw_UpdateAdvSPS
	.align	2
	.global	VC1_Vfmw_UpdataLastPicRNDCTRL
	.type	VC1_Vfmw_UpdataLastPicRNDCTRL, %function
VC1_Vfmw_UpdataLastPicRNDCTRL:
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
	ldrb	r3, [r0, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1071
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
.L1071:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_Vfmw_UpdataLastPicRNDCTRL, .-VC1_Vfmw_UpdataLastPicRNDCTRL
	.align	2
	.global	VC1_Vfmw_DecPQUANT
	.type	VC1_Vfmw_DecPQUANT, %function
VC1_Vfmw_DecPQUANT:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #128)
	sub	sp, sp, #128
	mov	r2, #128
	mov	r4, r1
	mov	r5, r0
	ldr	r1, .L1075
	sub	r0, fp, #148
	bl	memcpy
	cmp	r5, #0
	subeq	r3, fp, #20
	movne	r0, r4
	addeq	r4, r3, r4, lsl #2
	ldreq	r0, [r4, #-128]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1076:
	.align	2
.L1075:
	.word	.LANCHOR0+568
	UNWIND(.fnend)
	.size	VC1_Vfmw_DecPQUANT, .-VC1_Vfmw_DecPQUANT
	.align	2
	.global	VC1_ClearCurPacket
	.type	VC1_ClearCurPacket, %function
VC1_ClearCurPacket:
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
	ldr	r1, [r1, #28]
	mov	r5, r0
	bl	SM_ReleaseStreamSeg
	ldrsb	r3, [r4, #8]
	cmp	r3, #1
	beq	.L1078
	mov	r0, r5
	ldr	r1, [r4, #32]
	bl	SM_ReleaseStreamSeg
.L1078:
	ldr	r3, .L1082
	mov	r0, r4
	mov	r2, #36
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L1083:
	.align	2
.L1082:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VC1_ClearCurPacket, .-VC1_ClearCurPacket
	.align	2
	.global	VC1_ClearCurPic
	.type	VC1_ClearCurPic, %function
VC1_ClearCurPic:
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
	ldrb	r3, [r0, #3096]	@ zero_extendqisi2
	mov	r6, r1
	cmp	r3, #0
	beq	.L1085
	add	r7, r0, #20480
	ldr	r3, [r7, #2836]
	cmp	r3, #0
	ble	.L1099
	add	r4, r0, #23296
	mov	r5, #0
	add	r4, r4, #64
	b	.L1089
.L1088:
	ldr	r3, [r7, #2836]
	add	r4, r4, #48
	cmp	r3, r5
	ble	.L1100
.L1089:
	ldr	r1, [r4]
	mov	r0, r6
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #-36]
	add	r5, r5, #1
	cmp	r3, #0
	beq	.L1088
	ldr	r3, [r4, #-12]
	cmp	r3, #0
	beq	.L1088
	ldr	r1, [r4, #4]
	mov	r0, r6
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r7, #2836]
	add	r4, r4, #48
	cmp	r3, r5
	bgt	.L1089
.L1100:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1085:
	add	r1, r0, #35584
	mov	r0, r6
	add	r1, r1, #28
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VC1_ClearCurPacket
.L1099:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_ClearCurPic, .-VC1_ClearCurPic
	.align	2
	.global	VC1DEC_VDMPostProc
	.type	VC1DEC_VDMPostProc, %function
VC1DEC_VDMPostProc:
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
	add	r4, r0, #45056
	mov	r7, r0
	add	r0, r0, #12672
	add	r8, r7, #32768
	add	r0, r0, #40
	mov	r5, r1
	ldr	r1, [r4, #3776]
	bl	VC1_ClearCurPic
	ldr	r1, [r8, #3252]
	ldr	r0, [r4, #3776]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r8, #3248]
	mov	r6, r0
	ldr	r0, [r4, #3776]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r8, #3244]
	mov	r10, r0
	ldr	r0, [r4, #3776]
	bl	FSP_GetFsImagePtr
	cmp	r6, #0
	beq	.L1134
	cmp	r5, #0
	movlt	r5, #0
	blt	.L1104
	cmp	r5, #100
	movge	r5, #100
.L1104:
	add	r9, r7, #12288
	str	r5, [r6, #200]
	mov	r0, r7
	bl	VC1_SetImgFormat
	ldrb	r3, [r9, #427]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L1135
.L1105:
	mov	r5, #0
	str	r5, [r6, #212]
	str	r5, [r6, #216]
	ldr	r3, [r7]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L1106
	ldrb	r5, [r9, #426]	@ zero_extendqisi2
	sub	r5, r5, #2
	ands	r5, r5, #253
	beq	.L1107
	ldr	r5, [r8, #3240]
	cmp	r5, #1
	ldrhi	ip, [r8, #3244]
	mvnls	ip, #0
	cmp	r5, #1
	movls	r5, #0
	movhi	r5, #1
	str	ip, [r8, #3256]
.L1109:
	ldr	r3, [r4, #3264]
	cmn	ip, #1
	add	r3, r3, #1
	str	r3, [r4, #3264]
	beq	.L1110
	mov	r1, ip
	ldr	r0, [r4, #3776]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L1136
	ldr	r2, [r6, #200]
	cmp	r2, #0
	beq	.L1112
	ldr	r3, [r7]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bhi	.L1113
.L1112:
	ldr	r3, .L1139
	ldr	r10, [r8, #3256]
	ldr	r0, [r4, #3776]
	ldr	ip, [r3, #68]
	mov	r1, r10
	str	ip, [fp, #-48]
	bl	FSP_GetRef
	ldr	ip, [fp, #-48]
	mov	r2, r10
	ldr	r1, .L1139+4
	mov	r3, r0
	mov	r0, #12
	blx	ip
	mov	r2, #1
	ldr	r1, [r8, #3256]
	ldr	r0, [r4, #3776]
	bl	FSP_SetDisplay
	cmp	r5, #1
	ldreq	r3, [r8, #3168]
	movne	r1, #1
	ldrne	r2, [r8, #3160]
	movne	r3, #2
	strneb	r1, [r2, #1]
	mov	r2, r7
	streqb	r5, [r3, #1]
	mov	r1, #1
	strneb	r3, [r9, #425]
	add	r3, r7, #48384
	streqb	r5, [r9, #425]
	add	r3, r3, #96
	ldr	r0, [r4, #3776]
	str	r6, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L1116
	ldr	r1, [r6, #212]
	mov	r7, #0
	ldr	r0, [r4, #3776]
	bl	FreeUsdByDec
	str	r7, [r6, #212]
	ldr	r1, [r6, #216]
	ldr	r0, [r4, #3776]
	bl	FreeUsdByDec
	str	r7, [r6, #216]
	mov	r2, r7
	ldr	r0, [r4, #3776]
	ldr	r1, [r8, #3256]
	bl	FSP_SetDisplay
	cmp	r5, #1
	mov	r0, r7
	ldreq	r3, [r8, #3168]
	ldrne	r3, [r8, #3160]
	strb	r7, [r3, #1]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1107:
	ldr	ip, [r8, #3252]
	cmp	r10, #0
	moveq	r5, r10
	str	ip, [r8, #3256]
	beq	.L1109
	ldrd	r2, [r10, #8]
	ldrd	r0, [r6, #8]
	cmp	r2, r0
	sbcs	lr, r3, r1
	strltd	r2, [r6, #8]
	strltd	r0, [r10, #8]
	ldrlt	ip, [r8, #3256]
	b	.L1109
.L1137:
	ldr	r2, [r8, #3164]
	mov	r3, #0
	strb	r3, [r2, #1]
	strb	r3, [r9, #425]
.L1110:
	mov	r0, #1
.L1103:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1135:
	ldrb	r3, [r9, #433]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1110
	b	.L1105
.L1106:
	ldr	ip, [r8, #3252]
	str	ip, [r8, #3256]
	b	.L1109
.L1116:
	ldrb	r5, [r9, #425]	@ zero_extendqisi2
	sxtb	r5, r5
	cmp	r5, #3
	beq	.L1137
	cmp	r5, #1
	beq	.L1138
	cmp	r5, #2
	bne	.L1110
	ldr	r2, [r8, #3160]
	mov	r3, #0
	strb	r3, [r2, #1]
	strb	r3, [r9, #425]
	b	.L1103
.L1113:
	ldr	ip, .L1139
	mov	r0, #1
	ldr	r1, .L1139+8
	mov	r7, #0
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r1, [r6, #212]
	ldr	r0, [r4, #3776]
	bl	FreeUsdByDec
	str	r7, [r6, #212]
	ldr	r1, [r6, #216]
	ldr	r0, [r4, #3776]
	bl	FreeUsdByDec
	str	r7, [r6, #216]
	mov	r2, r7
	ldr	r0, [r4, #3776]
	ldr	r1, [r8, #3256]
	bl	FSP_SetDisplay
	cmp	r5, #1
	ldreq	r3, [r8, #3168]
	moveq	r0, r5
	ldrne	r3, [r8, #3160]
	movne	r0, #1
	strb	r7, [r3, #1]
	b	.L1103
.L1138:
	ldr	r2, [r8, #3168]
	mov	r3, #0
	mov	r0, r5
	strb	r3, [r2, #1]
	strb	r3, [r9, #425]
	b	.L1103
.L1134:
	ldr	ip, .L1139
	mov	r3, r10
	str	r0, [sp]
	mov	r2, r6
	ldr	r1, .L1139+12
	mov	r0, r6
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r6
	b	.L1103
.L1136:
	ldr	r3, .L1139
	movw	r2, #685
	ldr	r1, .L1139+16
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r6
	b	.L1103
.L1140:
	.align	2
.L1139:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC52
	.word	.LC53
	.word	.LC50
	.word	.LC51
	UNWIND(.fnend)
	.size	VC1DEC_VDMPostProc, .-VC1DEC_VDMPostProc
	.align	2
	.global	VC1_Vfmw_DecodeArbiter
	.type	VC1_Vfmw_DecodeArbiter, %function
VC1_Vfmw_DecodeArbiter:
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
	add	r5, r0, #12288
	mov	r4, r0
	ldrb	r3, [r5, #424]	@ zero_extendqisi2
	tst	r3, #16
	beq	.L1194
	add	r6, r0, #45056
	bic	r3, r3, #20
	strb	r3, [r5, #424]
	ldr	r3, [r6, #3380]
	sub	r3, r3, #2
	cmp	r3, #1
	bls	.L1144
.L1146:
	ldr	r3, [r4]
	add	r7, r4, #32768
	ldr	r1, [r7, #3240]
	ldr	r2, [r3, #8]
.L1145:
	sub	r3, r2, #1
	str	r2, [r6, #3380]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L1175
.L1156:
	.word	.L1150
	.word	.L1152
	.word	.L1174
	.word	.L1154
.L1173:
	mov	r1, #0
.L1154:
	ldrb	r3, [r5, #426]	@ zero_extendqisi2
	cmp	r3, #0
	streq	r3, [r6, #3312]
	moveq	r3, #1
	beq	.L1157
	cmp	r3, #2
	bne	.L1159
	ldr	r3, [r6, #3312]
	adds	r3, r3, #0
	movne	r3, #1
	cmp	r1, #0
	bne	.L1161
.L1160:
	ldr	r3, .L1196
	mov	r0, #1
	ldr	r1, .L1196+4
	ldr	r3, [r3, #68]
	blx	r3
.L1168:
	add	r0, r4, #12672
	ldr	r1, [r6, #3776]
	add	r0, r0, #40
	bl	VC1_ClearCurPic
	ldr	r3, .L1196+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1194
	mov	r3, #0
	ldr	r0, [r6, #3776]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L1194:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1144:
	ldr	r3, [r0]
	ldr	r2, [r3, #8]
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L1146
	ldr	r3, .L1196
	add	r7, r0, #32768
	ldr	r1, .L1196+12
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r7, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L1147
	ldr	r1, [r8, #212]
	mov	r9, #0
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r8, #216]
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r8, #220]
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	ldr	r1, [r8, #224]
	ldr	r0, [r6, #3776]
	bl	FreeUsdByDec
	str	r9, [r8, #212]
	str	r9, [r8, #216]
	mov	r2, #1
	str	r9, [r8, #220]
	str	r9, [r8, #224]
	ldr	r1, [r7, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r7, #3244]
	ldr	r0, [r6, #3776]
	bl	FSP_ClearLogicFs
	str	r9, [r7, #3244]
	str	r9, [r7, #3248]
	ldr	r0, [r6, #3776]
	bl	FSP_GetNullLogicFs
	str	r0, [r7, #3248]
	str	r0, [r7, #3244]
.L1147:
	ldr	r2, [r7, #3164]
	mov	r3, #0
	str	r3, [r7, #3240]
	cmp	r2, r3
	beq	.L1155
	ldr	r1, [r7, #3168]
	cmp	r1, #0
	beq	.L1155
	strb	r3, [r2, #1]
	ldr	r2, [r7, #3168]
	strb	r3, [r2, #1]
	ldr	r2, [r7, #3164]
	strb	r3, [r2]
	ldr	r2, [r7, #3168]
	strb	r3, [r2]
	ldr	r3, [r4]
	ldr	r1, [r7, #3240]
	ldr	r2, [r3, #8]
	b	.L1145
.L1174:
	mov	r3, #0
.L1157:
	cmp	r1, #0
	beq	.L1153
.L1161:
	cmp	r1, #1
	bls	.L1195
.L1164:
	cmp	r3, #0
	beq	.L1168
	add	r3, r4, #15808
	ldrsb	r3, [r3]
	cmp	r3, #0
	bne	.L1167
	add	r3, r5, #3584
	add	ip, r4, #23040
	add	r3, r3, #4
	add	ip, ip, #28
	ldmia	r3, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	add	r1, r5, #3824
	add	r0, r4, #23296
	add	r1, r1, #12
	mov	r2, #100
	add	r0, r0, #20
	bl	memcpy
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1170:
	mov	r1, #0
.L1150:
	ldrb	r3, [r5, #426]	@ zero_extendqisi2
	sub	r3, r3, #2
	tst	r3, #253
	movne	r3, #1
	moveq	r3, #0
	cmp	r1, #0
	bne	.L1161
.L1153:
	ldrb	r2, [r5, #426]	@ zero_extendqisi2
	sub	r0, r2, #4
	sub	r1, r2, #1
	cmp	r1, #1
	cmphi	r0, #1
	bls	.L1160
.L1162:
	sub	r2, r2, #2
	tst	r2, #253
	bne	.L1164
	ldr	r3, .L1196
	mov	r0, #1
	ldr	r1, .L1196+16
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1168
.L1171:
	mov	r1, #0
.L1152:
	ldrb	r3, [r5, #426]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	b	.L1157
.L1167:
	add	r1, r5, #3600
	add	r0, r4, #23040
	add	r1, r1, #4
	mov	r2, #172
	add	r0, r0, #44
	bl	memcpy
	add	lr, r4, #16064
	add	ip, r4, #23040
	ldmia	lr!, {r0, r1, r2, r3}
	add	ip, ip, #216
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	add	r0, r4, #23296
	add	r1, r5, #3936
	mov	r2, #176
	add	r0, r0, #120
	bl	memcpy
	ldr	r3, [r5, #3980]
	ldr	r2, .L1196
	add	r1, r4, #16384
	add	r0, r4, #23552
	add	r1, r1, #16
	strb	r3, [r5, #428]
	add	r0, r0, #40
	ldr	r3, [r7, #3260]
	ldr	ip, [r2, #52]
	mov	r2, r3, asl #4
	sub	r2, r2, r3, asl #2
	blx	ip
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1169:
	mov	r1, #0
.L1175:
	mov	r3, #1
	b	.L1157
.L1159:
	and	r3, r3, #251
	cmp	r3, #1
	bne	.L1175
	str	r3, [r6, #3312]
	b	.L1157
.L1155:
	ldr	r3, [r4]
	ldr	r2, [r3, #8]
	sub	r3, r2, #1
	str	r2, [r6, #3380]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L1169
.L1151:
	.word	.L1170
	.word	.L1171
	.word	.L1172
	.word	.L1173
.L1172:
	mov	r3, #0
	b	.L1153
.L1195:
	ldrb	r2, [r5, #426]	@ zero_extendqisi2
	b	.L1162
.L1197:
	.align	2
.L1196:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC55
	.word	g_event_report
	.word	.LC54
	.word	.LC56
	UNWIND(.fnend)
	.size	VC1_Vfmw_DecodeArbiter, .-VC1_Vfmw_DecodeArbiter
	.align	2
	.global	VC1_Vfmw_StartDecode
	.type	VC1_Vfmw_StartDecode, %function
VC1_Vfmw_StartDecode:
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
	ldr	r2, [r0]
	add	r5, r0, #12288
	add	r3, r0, #15808
	add	r6, r0, #12672
	mov	r4, r0
	add	r8, r0, #8
	ldr	r2, [r2, #676]
	add	r6, r6, #40
	str	r2, [r5, #252]
	ldrsb	r3, [r3]
	cmp	r3, #0
	beq	.L1213
.L1199:
	add	r7, r4, #32768
	mov	r1, r8
	mov	r0, r4
	bl	VC1_WritePicMsg
	ldr	r3, [r7, #3260]
	ldr	ip, .L1218
	add	r1, r6, #23296
	add	r1, r1, #24
	add	r0, r4, #92
	mov	r2, r3, asl #6
	strb	r3, [r4, #78]
	sub	r2, r2, r3, asl #4
	ldr	r3, [ip, #52]
	blx	r3
	mov	r1, r8
	mov	r0, r6
	bl	VC1_CfgVDH
	ldrb	r3, [r5, #426]	@ zero_extendqisi2
	cmp	r3, #1
	ldrlsb	r2, [r5, #428]	@ zero_extendqisi2
	strlsb	r2, [r5, #429]
	cmp	r3, #5
	beq	.L1214
	mov	r3, #0
	strb	r3, [r4, #9]
	ldrb	r3, [r5, #428]	@ zero_extendqisi2
	ldrb	r2, [r5, #432]	@ zero_extendqisi2
.L1202:
	strb	r3, [r4, #8]
	strb	r3, [r5, #427]
	strb	r2, [r5, #433]
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1215
	and	r2, r3, #251
	cmp	r2, #1
	addeq	r1, r4, #45056
	streq	r2, [r1, #3312]
.L1204:
	ldr	r2, [r4]
	str	r8, [r4, #4]
	ldr	r0, [r2, #884]
	cmp	r0, #1
	beq	.L1216
	cmp	r0, #2
	beq	.L1217
.L1209:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1217:
	ldr	r2, [r2, #888]
	cmp	r8, #0
	cmpne	r2, #0
	ble	.L1209
	sub	r3, r3, #2
	ands	r5, r3, #253
	bne	.L1209
	ldr	r3, [r7, #3160]
	add	r1, r4, #45056
	mov	r0, r6
	strb	r5, [r3, #1]
	ldr	r2, [r4]
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	ldr	r1, [r1, #3776]
	bl	VC1_ClearCurPic
	str	r5, [r4, #4]
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1215:
	add	r9, r4, #45056
	ldr	r5, [r9, #3312]
	cmp	r5, #0
	bne	.L1204
	mov	r0, r6
	ldr	r1, [r9, #3776]
	bl	VC1_ClearCurPic
	ldr	r3, [r7, #3160]
	mov	r0, r5
	strb	r5, [r3]
	ldr	r3, [r9, #3316]
	add	r3, r3, #1
	str	r3, [r9, #3316]
	str	r5, [r4, #4]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1214:
	mov	r2, #1
	strb	r2, [r4, #9]
	ldrb	r3, [r5, #429]	@ zero_extendqisi2
	add	r1, r4, #45056
	strb	r2, [r5, #432]
	strb	r3, [r5, #428]
	ldr	r1, [r1, #3336]
	sub	r1, r1, #1
	str	r1, [r5, #172]
	b	.L1202
.L1216:
	cmp	r8, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	sub	r3, r3, #2
	ands	r5, r3, #253
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r2, [r7, #3160]
	add	r3, r4, #45056
	mov	r0, r6
	strb	r5, [r2, #1]
	ldr	r1, [r3, #3776]
	bl	VC1_ClearCurPic
	str	r5, [r4, #4]
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1213:
	mov	r0, r6
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
	b	.L1199
.L1219:
	.align	2
.L1218:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VC1_Vfmw_StartDecode, .-VC1_Vfmw_StartDecode
	.align	2
	.global	VC1_Vfmw_ParseAdvHdr
	.type	VC1_Vfmw_ParseAdvHdr, %function
VC1_Vfmw_ParseAdvHdr:
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
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	r9, [r0, #3128]
	add	r8, r0, #32768
	mov	r4, r0
	str	r1, [fp, #-52]
	mov	r5, #0
	mov	r1, #32
	strb	r5, [r0, #6]
	mov	r0, r9
	str	r2, [fp, #-48]
	bl	BsGet
	ldr	r1, [r9, #24]
	ldr	r6, [r8, #2844]
	add	r7, r4, #35584
	cmp	r1, r5
	add	lr, r1, #7
	ldr	r2, [r8, #2864]
	add	r7, r7, #28
	movlt	r1, lr
	mov	r1, r1, asr #3
	add	r6, r6, r1
	rsb	r2, r1, r2
	mov	r1, r6
	uxtb	r10, r0
	mov	r0, r9
	bl	BsInit
	sub	r2, r10, #10
	cmp	r2, #5
	bhi	.L1221
	cmp	r10, #11
	beq	.L1283
	ldrb	r1, [r4]	@ zero_extendqisi2
	and	r0, r1, #7
	cmp	r0, #7
	beq	.L1284
	add	r10, r4, #20480
	strb	r5, [r4, #8]
	str	r5, [r10, #2836]
	cmp	r2, #5
	ldrls	pc, [pc, r2, asl #2]
	b	.L1253
.L1236:
	.word	.L1235
	.word	.L1253
	.word	.L1237
	.word	.L1254
	.word	.L1239
	.word	.L1240
.L1221:
	sub	r2, r10, #27
	cmp	r2, #4
	bls	.L1285
	cmp	r10, #254
	beq	.L1286
.L1253:
	ldr	r0, [fp, #-48]
	mov	r1, r7
	bl	VC1_ClearCurPacket
	mov	r0, #0
.L1226:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1285:
	ldr	r0, [fp, #-48]
	mov	r1, r7
	bl	VC1_ClearCurPacket
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1283:
	ldrb	r2, [r4]	@ zero_extendqisi2
	and	r2, r2, #7
	cmp	r2, #7
	beq	.L1287
.L1251:
	ldr	r0, [fp, #-48]
	mov	r1, r7
	bl	VC1_ClearCurPacket
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1254:
	ldr	r3, .L1292
	str	r3, [fp, #-56]
.L1282:
	ldr	r0, [fp, #-52]
	mov	r2, #176
	ldr	r3, [fp, #-56]
	mov	r1, #0
	add	r0, r0, #348
	ldrb	r5, [r0, #31]	@ zero_extendqisi2
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r0, [fp, #-52]
	strb	r5, [r0, #379]
.L1241:
	ldr	r3, [fp, #-56]
	add	r0, r4, #23296
	mov	r2, #12288
	mov	r1, #0
	add	r0, r0, #24
	ldr	r5, [r3, #48]
	blx	r5
	mov	r0, r9
	bl	BsPos
	ldr	r3, [fp, #-52]
	mov	r1, r4
	mov	r2, r3
	str	r0, [r3, #488]
	mov	r0, r9
	bl	VC1_Vfmw_ParseAdvPicHdr
	cmp	r0, #1
	mov	r5, r0
	beq	.L1288
	mov	r1, r7
	ldr	r0, [fp, #-48]
	bl	VC1_ClearCurPacket
	ldr	r3, .L1292
	movw	r2, #6067
	ldr	r1, .L1292+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	b	.L1226
.L1286:
	mov	r1, #32
	mov	r0, r9
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r9
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L1253
	ldrb	r3, [r4]	@ zero_extendqisi2
	mov	r2, #1
	mov	r0, r2
	strb	r2, [r4, #7]
	orr	r3, r3, #16
	strb	r2, [r4, #6]
	strb	r3, [r4]
	b	.L1226
.L1284:
	mov	r5, #1
	orr	r1, r1, #16
	strb	r5, [r4, #6]
	mov	r0, r5
	strb	r1, [r4]
	b	.L1226
.L1237:
	ldr	r3, .L1292
	mov	r1, #1
	ldr	r0, [fp, #-52]
	mov	r2, #16
	strb	r1, [r4, #8]
	mov	r1, #0
	add	r0, r0, #508
	ldr	r5, [r3, #48]
	str	r3, [fp, #-56]
	blx	r5
	ldrb	r2, [r4, #8]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1241
	b	.L1282
.L1240:
	ldr	r2, [fp, #-52]
	mov	r0, r9
	mov	r1, r4
	bl	VC1_Vfmw_ParseAdvSeqHdr
	cmp	r0, #1
	mov	r5, r0
	ldreqb	r3, [r4]	@ zero_extendqisi2
	orreq	r3, r3, #1
	streqb	r3, [r4]
	b	.L1251
.L1239:
	mov	r0, r9
	ldr	r2, [fp, #-52]
	mov	r1, r4
	bl	VC1_Vfmw_ParseAdvEntptHdr
	cmp	r0, #1
	mov	r5, r0
	bne	.L1251
	ldr	r1, [fp, #-52]
	mov	r0, r4
	bl	VC1_Vfmw_UpdateAdvSPS
	ldrb	r3, [r4]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r4]
	b	.L1251
.L1235:
	orr	r3, r1, #32
	ldr	r0, [fp, #-48]
	mov	r1, r7
	strb	r3, [r4]
	bl	VC1_ClearCurPacket
	mov	r0, #1
	b	.L1226
.L1287:
	mov	r0, r9
	ldr	r2, [fp, #-52]
	mov	r1, r4
	bl	VC1_Vfmw_ParseAdvSlcHdr
	cmp	r0, #1
	mov	r9, r0
	bne	.L1224
	add	r10, r4, #20480
	ldr	r3, [r10, #2836]
	cmp	r3, #255
	bgt	.L1289
	mov	r2, r3, asl #6
	mov	r9, r3, asl #4
	str	r2, [fp, #-48]
	rsb	r7, r9, r2
	ldr	r2, [r4, #3128]
	add	r7, r4, r7
	movw	r1, #23328
	movw	lr, #23320
	strb	r3, [r7, r1]
	movw	ip, #23336
	ldr	r0, [r2, #24]
	str	r3, [fp, #-56]
	cmp	r0, #0
	add	r1, r0, #7
	movlt	r0, r1
	add	r1, r7, #23296
	add	r0, r6, r0, asr #3
	str	r0, [r7, lr]
	ldr	r0, [r2, #24]
	mov	r6, r1
	and	r0, r0, #7
	str	r0, [r7, ip]
	ldr	r0, [r2, #16]
	add	ip, r1, #24
	ldr	r2, [r2, #24]
	str	ip, [fp, #-60]
	rsb	r2, r2, r0, lsl #3
	str	r2, [r1, #44]
	ldr	r0, [r7, lr]
	str	ip, [fp, #-64]
	bl	MEM_Vir2Phy
	movw	r1, #23360
	add	r2, r6, #64
	ldr	r3, [fp, #-56]
	str	r0, [r6, #36]
	ldr	r0, [r8, #2872]
	str	r0, [r7, r1]
	ldr	r1, [r8, #2848]
	cmp	r1, #0
	beq	.L1227
	ldr	r0, [r8, #2860]
	ldr	ip, [fp, #-60]
	cmp	r0, #0
	beq	.L1227
	movw	lr, #23352
	str	r1, [ip, #4]
	str	r5, [r7, lr]
	add	r1, r7, #23296
	ldr	lr, [r8, #2868]
	str	r0, [r1, #52]
	mov	lr, lr, asl #3
	str	lr, [r1, #60]
	ldr	r1, [r8, #2876]
	str	r1, [r2, #4]
.L1228:
	ldr	ip, [r10, #2756]
	ldr	r1, [r10, #2752]
	ldrb	r5, [r4, #8]	@ zero_extendqisi2
	mul	r1, r1, ip
	cmp	r5, #0
	sub	r1, r1, #1
	beq	.L1229
	ldr	r2, [fp, #-52]
	sub	r0, r9, r3, asl #2
	add	r3, ip, #1
	movw	r5, #23344
	add	r0, r2, r0
	ldr	r2, [fp, #-48]
	add	r3, r3, r3, lsr #31
	add	r0, r0, #520
	rsb	lr, r9, r2
	ldr	r2, [r0, #8]
	add	lr, r4, lr
	sub	r3, r2, r3, asr #1
	cmp	r1, r3
	str	r3, [lr, r5]
	movcs	r1, #0
	movcc	r1, #1
	cmp	r3, #0
	movle	r1, #0
	cmp	r1, #0
	bne	.L1290
.L1230:
	ldr	r3, [fp, #-48]
	rsb	r8, r9, r3
	add	r4, r4, r8
	add	r3, r4, #23296
	add	r3, r3, #40
	ldr	r3, [r3, #4]
	cmp	r3, #0
	ble	.L1231
	add	r3, r4, #23296
	add	r3, r3, #24
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1249
	add	r4, r4, #23296
	ldr	r3, [r4, #60]
	cmp	r3, #0
	ble	.L1231
.L1249:
	ldr	r3, [r10, #2836]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r10, #2836]
	b	.L1226
.L1288:
	mov	r0, r9
	bl	BsPos
	ldr	r3, [fp, #-52]
	ldr	r2, [r3, #488]
	rsb	r0, r2, r0
	str	r0, [r3, #488]
	ldrb	r2, [r4]	@ zero_extendqisi2
	mov	r0, r9
	orr	r3, r2, #4
	strb	r3, [r4]
	bl	BsPos
	ldr	r3, [r8, #2864]
	add	r0, r0, #32
	cmp	r0, r3, asl #3
	bge	.L1243
	ldr	r7, [r10, #2836]
	movw	r3, #23328
	ldr	r2, [r4, #3128]
	movw	lr, #23336
	mov	r1, r7, asl #4
	mov	r9, r7, asl #6
	rsb	r5, r1, r9
	str	r1, [fp, #-48]
	add	r5, r4, r5
	add	r0, r5, #23296
	strb	r7, [r5, r3]
	ldr	r3, [r2, #24]
	cmp	r3, #0
	add	r1, r3, #7
	movlt	r3, r1
	add	r1, r0, #32
	add	r6, r6, r3, asr #3
	movw	r3, #23320
	str	r6, [r5, r3]
	add	r6, r0, #24
	ldr	r3, [r2, #24]
	str	r1, [fp, #-60]
	and	r3, r3, #7
	str	r3, [r5, lr]
	ldr	r3, [r2, #16]
	ldr	r2, [r2, #24]
	str	r1, [fp, #-64]
	rsb	r3, r2, r3, lsl #3
	str	r3, [r0, #44]
	movw	r3, #23320
	ldr	r0, [r5, r3]
	bl	MEM_Vir2Phy
	ldr	r1, [fp, #-60]
	movw	r2, #23360
	add	r3, r5, #23296
	add	r3, r3, #64
	str	r0, [r1, #4]
	ldr	r1, [r8, #2872]
	str	r1, [r5, r2]
	ldr	r2, [r8, #2848]
	cmp	r2, #0
	beq	.L1244
	ldr	r1, [r8, #2860]
	cmp	r1, #0
	beq	.L1244
	movw	r0, #23352
	str	r2, [r6, #4]
	mov	r2, #0
	str	r2, [r5, r0]
	ldr	r0, [r8, #2868]
	add	r2, r5, #23296
	str	r1, [r2, #52]
	mov	r0, r0, asl #3
	str	r0, [r2, #60]
	ldr	r2, [r8, #2876]
	str	r2, [r3, #4]
.L1245:
	ldr	r0, [r10, #2756]
	ldr	r1, [r10, #2752]
	ldrb	r5, [r4, #8]	@ zero_extendqisi2
	mul	r1, r1, r0
	cmp	r5, #0
	sub	r1, r1, #1
	beq	.L1246
	ldr	ip, [fp, #-48]
	add	r3, r0, #1
	ldr	lr, [fp, #-52]
	movw	r5, #23344
	sub	r2, ip, r7, asl #2
	rsb	ip, ip, r9
	add	r2, lr, r2
	add	lr, r3, r3, lsr #31
	add	ip, r4, ip
	add	r2, r2, #520
	ldr	r3, [r2, #8]
	sub	r3, r3, lr, asr #1
	str	r3, [ip, r5]
	cmp	r1, r3
	movcs	r1, #0
	movcc	r1, #1
	cmp	r3, #0
	movle	r1, #0
	cmp	r1, #0
	bne	.L1291
.L1247:
	ldr	r3, [fp, #-48]
	rsb	r8, r3, r9
	add	r4, r4, r8
	add	r3, r4, #23296
	add	r3, r3, #40
	ldr	r3, [r3, #4]
	cmp	r3, #0
	ble	.L1248
	add	r3, r4, #23296
	add	r3, r3, #24
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1249
	add	r4, r4, #23296
	ldr	r3, [r4, #60]
	cmp	r3, #0
	bgt	.L1249
.L1248:
	ldr	r3, [fp, #-56]
	mov	r0, #1
	ldr	r1, .L1292+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	b	.L1226
.L1224:
	mov	r1, r7
	ldr	r0, [fp, #-48]
	bl	VC1_ClearCurPacket
	ldr	r3, .L1292
	movw	r2, #5978
	ldr	r1, .L1292+12
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r9
	b	.L1226
.L1243:
	mov	r1, r7
	ldr	r0, [fp, #-48]
	bl	VC1_ClearCurPacket
	ldr	r3, .L1292
	movw	r2, #6061
	ldr	r1, .L1292+4
	mov	r0, r5
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	b	.L1226
.L1244:
	ldr	r3, [fp, #-48]
	movw	r5, #23352
	mov	r2, #0
	rsb	r3, r3, r9
	add	r3, r4, r3
	add	lr, r3, #23296
	mov	ip, lr
	str	r2, [lr, #28]
	str	r2, [r3, r5]
	add	lr, lr, #24
	str	r2, [ip, #60]
	str	r2, [ip, #52]
	str	r2, [ip, #68]
	b	.L1245
.L1289:
	ldr	r1, [fp, #-48]
	mov	r0, r4
	bl	VC1_ClearCurPic
	ldr	r3, .L1292
	ldr	r2, [r10, #2836]
	mov	r0, r9
	ldr	r1, .L1292+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	b	.L1226
.L1227:
	ldr	r2, [fp, #-48]
	movw	lr, #23352
	mov	r1, #0
	rsb	r2, r9, r2
	add	r2, r4, r2
	add	r6, r2, #23296
	str	r1, [r6, #28]
	str	r1, [r2, lr]
	str	r1, [r6, #60]
	str	r1, [r6, #52]
	str	r1, [r6, #68]
	b	.L1228
.L1246:
	ldr	r3, [fp, #-48]
	movw	ip, #23344
	sub	r2, r3, r7, asl #2
	rsb	r0, r3, r9
	ldr	r3, [fp, #-52]
	add	r0, r4, r0
	add	r2, r3, r2
	add	r2, r2, #520
	ldr	r3, [r2, #8]
	cmp	r1, r3
	str	r3, [r0, ip]
	bcs	.L1247
	ldr	r2, [r2, #8]
	mov	r0, #1
	str	r1, [sp]
	ldr	r1, [fp, #-56]
	ldr	ip, [r1, #68]
	ldr	r1, .L1292+20
	blx	ip
	mov	r0, r5
	b	.L1226
.L1229:
	sub	r2, r9, r3, asl #2
	ldr	r3, [fp, #-48]
	movw	ip, #23344
	rsb	r0, r9, r3
	ldr	r3, [fp, #-52]
	add	r0, r4, r0
	add	r2, r3, r2
	add	r2, r2, #520
	ldr	r3, [r2, #8]
	cmp	r1, r3
	str	r3, [r0, ip]
	bcs	.L1230
	ldr	ip, .L1292
	mov	r0, #1
	ldr	r2, [r2, #8]
	str	r1, [sp]
	ldr	r1, .L1292+20
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	b	.L1226
.L1231:
	ldr	r3, .L1292
	mov	r0, #1
	ldr	r1, .L1292+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	b	.L1226
.L1291:
	ldr	r1, [fp, #-56]
	ldr	r2, [r2, #8]
	str	r0, [sp]
	mov	r0, #1
	ldr	ip, [r1, #68]
	ldr	r1, .L1292+24
	blx	ip
	mov	r0, #0
	b	.L1226
.L1290:
	ldr	lr, .L1292
	ldr	r2, [r0, #8]
	mov	r0, #1
	str	ip, [sp]
	ldr	r1, .L1292+24
	ldr	ip, [lr, #68]
	blx	ip
	mov	r0, #0
	b	.L1226
.L1293:
	.align	2
.L1292:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC59
	.word	.LC49
	.word	.LC58
	.word	.LC57
	.word	.LC48
	.word	.LC47
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseAdvHdr, .-VC1_Vfmw_ParseAdvHdr
	.align	2
	.global	VC1_Vfmw_ParseHeader
	.type	VC1_Vfmw_ParseHeader, %function
VC1_Vfmw_ParseHeader:
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
	ldrb	r3, [r0, #3096]	@ zero_extendqisi2
	mov	r4, r0
	cmp	r3, #0
	bne	.L1295
	ldrb	r3, [r0]	@ zero_extendqisi2
	mov	r2, r1
	mov	r1, r0
	ldr	r0, [r0, #3128]
	orr	r3, r3, #3
	strb	r3, [r4]
	bl	VC1_Vfmw_ParseSMPPicHdr
	cmp	r0, #1
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldrb	r3, [r4]	@ zero_extendqisi2
	orr	r3, r3, #20
	strb	r3, [r4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1295:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvHdr
	UNWIND(.fnend)
	.size	VC1_Vfmw_ParseHeader, .-VC1_Vfmw_ParseHeader
	.align	2
	.global	VC1_SetCodeInfo
	.type	VC1_SetCodeInfo, %function
VC1_SetCodeInfo:
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
	add	r0, r0, #15808
	ldrb	r3, [r4, #3522]	@ zero_extendqisi2
	add	r0, r0, #36
	str	r0, [r4, #3552]
	cmp	r3, #8
	strneb	r3, [r4, #435]
	moveq	r3, #6
	movne	r3, #0
	streqb	r3, [r4, #435]
	moveq	r2, #1
	strneb	r3, [r4, #436]
	add	r3, r5, #15808
	streqb	r2, [r4, #436]
	ldrsb	r3, [r3]
	ldr	r2, [r1, #20]
	cmp	r3, #0
	ldr	r1, [r1]
	beq	.L1303
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	BsInit
.L1303:
	bl	BsInit
	ldr	lr, [r4, #3544]
	add	r5, r5, #32768
	add	r1, r4, #3520
	adds	r3, lr, #15
	add	r0, r4, #3584
	addmi	r3, lr, #30
	add	r1, r1, #3
	mov	r2, #16
	add	r0, r0, #4
	mov	r3, r3, asr #4
	str	r3, [r5, #3176]
	ldr	ip, [r4, #3548]
	str	lr, [r5, #3192]
	adds	r3, ip, #15
	addmi	r3, ip, #30
	str	ip, [r5, #3196]
	mov	r3, r3, asr #4
	str	r3, [r5, #3180]
	bl	memcpy
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_SetCodeInfo, .-VC1_SetCodeInfo
	.align	2
	.global	VC1_ReceivePacket
	.type	VC1_ReceivePacket, %function
VC1_ReceivePacket:
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
	subs	r4, r1, #0
	add	r1, r0, #48128
	add	r1, r1, #196
	beq	.L1312
	ldr	r2, [r4, #8]
	add	r3, r0, #45056
	str	r2, [r3, #3280]
	ldr	r2, [r4]
	str	r2, [r3, #3268]
	ldr	r2, [r4, #12]
	str	r2, [r3, #3288]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #3276]
	ldr	r2, [r4, #16]
	str	r2, [r3, #3296]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1307
	ldr	r2, [r4, #88]
	str	r2, [r3, #3284]
	ldr	r2, [r4, #80]
	str	r2, [r3, #3272]
	ldr	r2, [r4, #92]
	str	r2, [r3, #3292]
	ldrb	r2, [r4, #85]	@ zero_extendqisi2
	strb	r2, [r3, #3277]
	ldr	r2, [r4, #96]
	str	r2, [r3, #3300]
	ldrb	r2, [r4, #85]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1313
.L1308:
	bl	VC1_SetCodeInfo
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1313:
	ldr	r0, [r3, #3776]
	bl	VC1_ClearCurPacket
	ldr	r3, .L1314
	ldrb	r2, [r4, #85]	@ zero_extendqisi2
	mov	r0, #1
	ldr	r1, .L1314+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1307:
	mov	r2, #0
	str	r2, [r3, #3284]
	str	r2, [r3, #3272]
	str	r2, [r3, #3292]
	strb	r2, [r3, #3277]
	str	r2, [r3, #3300]
	b	.L1308
.L1312:
	ldr	r3, .L1314
	mov	r0, r4
	ldr	r1, .L1314+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1315:
	.align	2
.L1314:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC61
	.word	.LC60
	UNWIND(.fnend)
	.size	VC1_ReceivePacket, .-VC1_ReceivePacket
	.align	2
	.global	VC1DEC_Decode
	.type	VC1DEC_Decode, %function
VC1DEC_Decode:
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
	ldr	r6, .L1345
	add	r5, r0, #12544
	add	r5, r5, #8
	mov	r4, r0
	mov	r2, #160
	ldr	r3, [r6, #52]
	mov	r0, r5
	blx	r3
	mov	r1, r5
	mov	r0, r4
	bl	VC1_ReceivePacket
	subs	r7, r0, #0
	beq	.L1340
	ldr	r3, [r4]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L1341
.L1319:
	add	r3, r4, #12288
	add	r6, r4, #45056
	ldrb	r3, [r3, #424]	@ zero_extendqisi2
	strb	r3, [r6, #3384]
.L1320:
	add	r5, r4, #12672
	add	r1, r4, #15872
	add	r5, r5, #40
	add	r1, r1, #4
	ldr	r2, [r6, #3776]
	mov	r0, r5
	bl	VC1_Vfmw_ParseHeader
	subs	r8, r0, #0
	beq	.L1342
	mov	r0, r4
	bl	VC1_Vfmw_DecodeArbiter
	cmp	r0, #0
	bne	.L1343
.L1323:
	mov	r7, #0
.L1318:
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1342:
	add	r1, r4, #48128
	ldr	r0, [r6, #3776]
	add	r1, r1, #196
	bl	VC1_ClearCurPacket
	ldr	r3, .L1345+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1323
	ldr	r0, [r6, #3776]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	mov	r7, r8
	blx	ip
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1341:
	ldr	r3, [r3, #892]
	cmp	r3, #1
	bne	.L1319
	add	r6, r4, #45056
	add	r3, r4, #12288
	ldrb	r2, [r6, #3384]	@ zero_extendqisi2
	strb	r2, [r3, #424]
	b	.L1320
.L1340:
	add	r4, r4, #45056
	ldr	r3, [r6, #68]
	ldr	r1, .L1345+8
	ldr	r2, [r4, #3264]
	blx	r3
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1343:
	mov	r0, r4
	bl	VC1_Vfmw_PrepareDecode
	subs	r7, r0, #0
	bne	.L1324
	ldr	r3, [r4]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L1344
.L1325:
	mov	r0, r5
	ldr	r1, [r6, #3776]
	bl	VC1_ClearCurPic
.L1326:
	ldr	r3, .L1345+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1323
	mov	r3, #0
	ldr	r0, [r6, #3776]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L1318
.L1344:
	ldr	r3, [r3, #892]
	cmp	r3, #1
	bne	.L1325
	b	.L1326
.L1324:
	mov	r0, r4
	bl	VC1_Vfmw_StartDecode
	subs	r7, r0, #0
	movne	r7, r8
	bne	.L1318
	add	r4, r4, #32768
	mov	r2, #1
	ldr	r0, [r6, #3776]
	ldr	r1, [r4, #3252]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #3160]
	ldr	r3, .L1345+4
	strb	r7, [r2]
	ldr	r2, [r4, #3160]
	strb	r7, [r2, #1]
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1323
	ldr	r0, [r6, #3776]
	mov	r3, r7
	mov	r2, r7
	mov	r1, #113
	blx	ip
	b	.L1318
.L1346:
	.align	2
.L1345:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC62
	UNWIND(.fnend)
	.size	VC1DEC_Decode, .-VC1DEC_Decode
	.align	2
	.global	VC1_GetPacketState
	.type	VC1_GetPacketState, %function
VC1_GetPacketState:
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
	add	r0, r0, #12672
	ldrsb	r3, [r0, #46]
	cmp	r3, #1
	moveq	r0, #2
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_GetPacketState, .-VC1_GetPacketState
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.13662, %object
	.size	__func__.13662, 13
__func__.13662:
	.ascii	"Vc1SMPSeqHdr\000"
	.space	3
.LC0:
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	1
	.word	1
	.word	2
	.word	1
	.word	2
	.word	2
	.word	3
	.word	3
	.word	3
.LC1:
	.word	3
	.word	1
	.word	2
	.word	0
	.word	4
.LC2:
	.word	1
	.word	0
	.word	2
	.word	3
	.word	4
.LC3:
	.word	3
	.word	1
.LC4:
	.word	3
	.word	1
	.word	2
	.word	0
.LC5:
	.word	1
	.word	0
	.word	2
	.word	3
.LC6:
	.word	1
	.word	1
	.word	2
	.word	1
	.word	3
	.word	1
	.word	2
.LC7:
	.word	2
	.word	3
	.word	3
	.word	4
	.word	4
	.word	5
	.word	5
.LC8:
	.word	3
	.word	4
	.word	1
	.word	5
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	1
	.word	3
	.word	5
	.word	7
.LC9:
	.word	5
	.word	5
	.word	6
	.word	6
	.word	7
	.word	7
	.word	7
	.word	7
	.word	7
	.word	7
	.word	8
	.word	8
	.word	8
	.word	8
.LC10:
	.word	256
	.word	128
	.word	85
	.word	64
	.word	51
	.word	43
	.word	37
	.word	32
	.type	__func__.13973, %object
	.size	__func__.13973, 17
__func__.13973:
	.ascii	"VC1_DecBfraction\000"
	.space	3
	.type	aCount, %object
	.size	aCount, 128
aCount:
	.word	0
	.word	1
	.word	1
	.word	2
	.word	1
	.word	2
	.word	2
	.word	3
	.word	1
	.word	2
	.word	2
	.word	3
	.word	2
	.word	3
	.word	3
	.word	4
	.word	1
	.word	2
	.word	2
	.word	3
	.word	2
	.word	3
	.word	3
	.word	4
	.word	2
	.word	3
	.word	3
	.word	4
	.word	3
	.word	4
	.word	4
	.word	5
	.type	a2On4Off, %object
	.size	a2On4Off, 60
a2On4Off:
	.word	3
	.word	5
	.word	6
	.word	9
	.word	10
	.word	12
	.word	17
	.word	18
	.word	20
	.word	24
	.word	33
	.word	34
	.word	36
	.word	40
	.word	48
.LC12:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	7
	.word	8
	.word	6
	.word	7
	.word	8
	.word	9
	.word	10
	.word	11
	.word	12
	.word	13
	.word	14
	.word	15
	.word	16
	.word	17
	.word	18
	.word	19
	.word	20
	.word	21
	.word	22
	.word	23
	.word	24
	.word	25
	.word	27
	.word	29
	.word	31
.LC11:
	.word	1
	.word	2
	.word	2
	.word	2
	.word	0
	.word	1
	.word	0
	.word	1
	.type	FldPTypeTab, %object
	.size	FldPTypeTab, 64
FldPTypeTab:
	.word	0
	.word	0
	.word	0
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	2
	.word	2
	.word	2
	.word	4
	.word	4
	.word	2
	.word	4
	.word	4
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC13:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC14:
	ASCII(.ascii	"VC1 INIT!!!\012\000" )
	.space	3
.LC15:
	ASCII(.ascii	"picture width out of range\000" )
	.space	1
.LC16:
	ASCII(.ascii	"%s: %s\012\000" )
.LC17:
	ASCII(.ascii	"picture height out of range\000" )
.LC18:
	ASCII(.ascii	"Vc1SMPSeqHdr Err, BS is wrong\000" )
	.space	2
.LC19:
	ASCII(.ascii	"Vc1SMPSeqHdr Err, BS is wrong\012\000" )
	.space	1
.LC20:
	ASCII(.ascii	"image size abnormal(%dx%d)\012\000" )
.LC21:
	ASCII(.ascii	"vc1 actual frame size(%dx%d) exeed max config(%dx%d" )
	ASCII(.ascii	")\012\000" )
	.space	2
.LC22:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"ERROR: FSP_ConfigInstance fail!\012\000" )
	.space	3
.LC24:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"VC1_Vfmw_GetImageBuffer get imgbuf err, clear all r" )
	ASCII(.ascii	"ef!\012\000" )
.LC27:
	ASCII(.ascii	"VC1_Vfmw_GetImageBuffer get_flag!=1, FSP_ClearLogic" )
	ASCII(.ascii	"Fs.\012\000" )
.LC28:
	ASCII(.ascii	"VC1_Vfmw_GetStreamInfo VC1_ArrangeVahbMem err!\012\000" )
.LC29:
	ASCII(.ascii	"VC1_Vfmw_GetStreamInfo VC1_Vfmw_GetImageBuffer err!" )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC30:
	ASCII(.ascii	"dec_pts: %lld Usertag = %lld\012\000" )
	.space	2
.LC31:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC32:
	ASCII(.ascii	"VC1_Vfmw_GetStreamInfo B pic pCodecInf->RefNum < 2 " )
	ASCII(.ascii	"err!\012\000" )
	.space	3
.LC33:
	ASCII(.ascii	"line: %d (pCur,pFwd,pBwd)(%p,%p,%p)\012\000" )
	.space	3
.LC34:
	ASCII(.ascii	"VC1_Vfmw_GetStreamInfo imgbuf = NULL err!\012\000" )
	.space	1
.LC35:
	ASCII(.ascii	"CurFsID(fwd%d,%d,bwd%d) abnormal!(%p,%p,%p)\012\000" )
	.space	3
.LC36:
	ASCII(.ascii	"VC1_Vfmw_GetStreamInfo VC1_Vfmw_BuildRefList err!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC37:
	ASCII(.ascii	"VC1_Vfmw_GetStreamInfo error! Release all packets!\012" )
	ASCII(.ascii	"\000" )
.LC38:
	ASCII(.ascii	"%s:%d Code(%d) is invalid\012\000" )
	.space	1
.LC39:
	ASCII(.ascii	"%s:%d Denominator(%d) is invalid\012\000" )
	.space	2
.LC40:
	ASCII(.ascii	"Check Pos 2: %d\012\000" )
	.space	3
.LC41:
	ASCII(.ascii	"Check Pos 3: %d\012\000" )
	.space	3
.LC42:
	ASCII(.ascii	"\012======PicNum  :%6d=========\012\000" )
	.space	2
.LC43:
	ASCII(.ascii	"INTERPFRM  :%#x\012\000" )
	.space	3
.LC44:
	ASCII(.ascii	"VC1_Vfmw_ParseMPBPicHdr err!\012\000" )
	.space	2
.LC45:
	ASCII(.ascii	"VC1_Vfmw_ParseSMPIPicHdr err!\012\000" )
	.space	1
.LC46:
	ASCII(.ascii	"After VC1_Vfmw_ParseSMPPicHdr BS Len <= 0 err!\012\000" )
.LC47:
	ASCII(.ascii	"SLICE_ADDR = %d,SlcEndMbn = %d, MbHeight = %d\012\000" )
	.space	1
.LC48:
	ASCII(.ascii	"SLICE_ADDR = %d,SlcEndMbn = %d, totalMbMin1 = %d\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC49:
	ASCII(.ascii	"VC1_RecordSlcStream stream len <= 0\012\000" )
	.space	3
.LC50:
	ASCII(.ascii	"pImg is null, pCurImg %#x pBwdImg %#x pFwdImg %#x\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC51:
	ASCII(.ascii	"line: %d, pToQueImg is Null!\012\000" )
	.space	2
.LC52:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.space	2
.LC53:
	ASCII(.ascii	"err(%d) > out_thr(%d)\012\000" )
	.space	1
.LC54:
	ASCII(.ascii	"----------------- VC1 Imode -> %d -----------------" )
	ASCII(.ascii	"-\012\000" )
	.space	2
.LC55:
	ASCII(.ascii	"NOTE: Drop non-I\012\000" )
	.space	2
.LC56:
	ASCII(.ascii	"NOTE: Drop B\012\000" )
	.space	2
.LC57:
	ASCII(.ascii	" pCodecInf->SlcNum = %d >= 256 \012\000" )
	.space	3
.LC58:
	ASCII(.ascii	"line %d, Code = VC1_SCSLC failed!\012\000" )
	.space	1
.LC59:
	ASCII(.ascii	"line %d, Code = VC1_SCFRM failed!\012\000" )
	.space	1
.LC60:
	ASCII(.ascii	"bitsream para error -----\012\000" )
	.space	1
.LC61:
	ASCII(.ascii	"line %d, pPacket->StreamPack[1].IsLastSeg = %d\012\000" )
.LC62:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet pointer!\012\000" )
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
