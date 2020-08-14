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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #49152
	ldr	r3, .L37
	mov	r8, r1
	movw	r2, #49576
	ldr	r9, [r5, #408]
	mov	r1, #0
	ldr	r3, [r3, #48]
	mov	r4, r0
	mov	r7, r1
	blx	r3
	str	r9, [r5, #408]
	add	r0, r5, #64
	bl	ResetVoQueue
	add	r1, r4, #12288
	ldr	r3, [r8, #32]
	mov	r6, #1
	str	r8, [r4]
	add	r2, r4, #45056
	strb	r6, [r1, #449]
	add	r3, r3, #255
	ldr	lr, [r8, #8]
	bic	r3, r3, #255
	add	ip, r3, #2048
	mov	r0, r4
	add	r9, r3, #4096
	add	r8, r3, #6144
	str	lr, [r5, #20]
	add	r4, r3, #8192
	str	ip, [r2, #4092]
	add	lr, r3, #10240
	str	r3, [r2, #4088]
	add	ip, r3, #12288
	str	r9, [r5]
	str	r8, [r5, #4]
	str	r4, [r5, #8]
	str	lr, [r5, #12]
	str	ip, [r5, #16]
	strb	r6, [r1, #450]
	str	r7, [r2, #4048]
	str	r7, [r2, #4052]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r5, #416]
	beq	.L36
	ldr	r1, .L37+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L36:
	mov	r0, r7
	ldr	r1, .L37+8
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #32)
	sub	sp, sp, #32
	mov	r4, r2
	mov	r2, r1, asl #3
	mov	r1, r0
	sub	r0, fp, #52
	bl	BsInit
	mov	r1, #8
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #8
	mov	r5, r0
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #8
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #16
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #24
	sub	r0, fp, #52
	str	r5, [r4, #8]
	bl	BsGet
	mov	r1, #8
	mov	r5, r0
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #8
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #8
	add	r5, r5, r0, lsl #16
	sub	r0, fp, #52
	bl	BsGet
	ldr	r3, [r4, #8]
	cmp	r3, #8192
	add	r0, r5, r0, lsl #24
	str	r0, [r4, #12]
	bgt	.L43
	cmp	r0, #8192
	bgt	.L44
	cmp	r3, #31
	ble	.L43
	cmp	r0, #31
	ble	.L44
	mov	r1, #2
	sub	r0, fp, #52
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #16]
	cmp	r0, #1
	bhi	.L46
	mov	r1, #2
	sub	r0, fp, #52
	bl	BsGet
	cmp	r0, #0
	bne	.L46
	mov	r1, #3
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #5
	strb	r0, [r4, #17]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #18]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #19]
	sub	r0, fp, #52
	bl	BsGet
	cmp	r0, #0
	bne	.L46
	mov	r1, #1
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #20]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #21]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #22]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #23]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #24]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #25]
	sub	r0, fp, #52
	bl	BsGet
	subs	r5, r0, #0
	bne	.L46
	mov	r1, #1
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #26]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #27]
	sub	r0, fp, #52
	bl	BsGet
	ldrb	r3, [r4, #16]	@ zero_extendqisi2
	mov	r1, #3
	cmp	r3, #0
	strb	r0, [r4, #28]
	sub	r0, fp, #52
	streqb	r5, [r4, #28]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #29]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #30]
	sub	r0, fp, #52
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #31]
	sub	r0, fp, #52
	bl	BsGet
	sub	r0, fp, #52
	mov	r1, #32
	bl	BsGet
	cmp	r0, #0
	bne	.L52
	mov	r0, r5
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L52:
	ldr	r1, .L53
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r5
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L46:
	ldr	r3, .L53+4
.L51:
	ldr	r2, .L53+8
	mov	r0, #0
	ldr	r1, .L53+12
	mvn	r5, #0
	bl	dprint_vfmw
	mov	r0, r5
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L44:
	ldr	r3, .L53+16
	b	.L51
.L43:
	ldr	r3, .L53+20
	b	.L51
.L54:
	.align	2
.L53:
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
	add	r5, r0, #32768
	add	r3, r0, #49152
	mov	r7, r0
	add	r6, r0, #12288
	ldr	r1, [r5, #3988]
	ldr	r0, [r3, #416]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L55
	mov	r3, #1
	str	r3, [r4, #376]
	ldrb	r3, [r6, #443]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L69
	mov	r2, #7168
	mov	r1, #0
	mov	r3, r2
	str	r2, [r4, #108]
	strb	r1, [r4, #364]
.L59:
	ldr	r2, [r5, #3928]
	cmp	r2, #1920
	orrls	r3, r3, #131072
	bichi	r3, r3, #131072
	str	r3, [r4, #108]
	ldr	r2, [r7]
	ldr	r2, [r2, #592]
	cmp	r2, #1
	movne	r2, #0
	beq	.L70
.L62:
	orr	r3, r2, r3
	str	r3, [r4, #108]
	ldr	r3, [r5, #3932]
	mov	r1, #0
	mov	r0, r4
	add	r10, r4, #528
	str	r3, [r4, #116]
	ldr	r3, [r5, #3928]
	str	r3, [r4, #112]
	bl	SetAspectRatio
	ldr	lr, [r6, #240]
	ldr	r3, [r4, #44]
	add	ip, r4, #512
	ldr	r2, [r4, #140]
	add	r1, r3, lr
	str	r1, [r4, #48]
	ldrd	r6, [ip, #-8]
	add	r0, r1, r2
	str	r0, [fp, #-48]
	mov	r0, r1
	adds	r8, r6, lr
	add	r2, r3, r2
	adc	r9, r7, #0
	strd	r8, [ip]
	str	r1, [r4, #32]
	ldr	r1, [fp, #-48]
	str	r3, [r4, #28]
	str	r2, [r4, #36]
	str	r1, [r4, #40]
	strd	r6, [r10, #-8]
	str	r3, [r4, #60]
	bl	MEM_Phy2Vir
	mov	r3, #0
	str	r3, [r4, #532]
	mov	r2, #8
	ldr	r3, [r4, #112]
	mov	r1, #1
	add	r3, r3, #508
	add	r3, r3, #3
	bic	r3, r3, #508
	bic	r3, r3, #3
	mov	r3, r3, lsr #5
	str	r0, [r4, #528]
	ldr	r0, [r4, #32]
	str	r0, [r4, #64]
	ldr	r0, [r5, #3932]
	str	r0, [r4, #124]
	ldr	r0, [r5, #3928]
	strb	r1, [r4, #371]
	str	r3, [r4, #144]
	str	r0, [r4, #120]
	strb	r1, [r4, #369]
	str	r2, [r4, #380]
	str	r2, [r4, #388]
	str	r2, [r4, #384]
.L55:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L69:
	ldr	r3, [r4, #108]
	mov	r2, #4
	strb	r2, [r4, #364]
	ldrb	r1, [r7, #16]	@ zero_extendqisi2
	ubfx	r2, r3, #10, #2
	add	r3, r1, #1
	orr	r3, r3, r2
	cmp	r3, #3
	movls	r3, r3, asl #10
	orrls	r3, r3, #4352
	movhi	r3, #4352
	str	r3, [r4, #108]
	b	.L59
.L70:
	tst	r3, #131072
	movne	r2, #0
	moveq	r2, #262144
	b	.L62
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r5, r1
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r4, r0, #49152
	mov	r2, #0
	ldr	r0, [r4, #416]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #416]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L75
	add	r5, r0, #576
	add	r8, r0, #560
	ldr	r1, [r0, #560]
	mov	r6, #0
	ldr	r0, [r4, #416]
	mov	r7, #0
	bl	FreeUsdByDec
	strd	r6, [r8]
	ldr	r0, [r4, #416]
	ldr	r1, [r5, #-8]
	bl	FreeUsdByDec
	strd	r6, [r5, #-8]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L75:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r5, r0, #32768
	mov	r4, r0
	add	r9, r0, #12288
	ldr	r3, [r5, #3900]
	cmp	r3, #0
	beq	.L83
	ldr	r3, [r0]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L94
	add	r6, r0, #49152
	ldr	r1, [r5, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	moveq	r5, r6
	addeq	r5, r5, #64
	beq	.L82
	ldr	r1, [r5, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L92
.L94:
	add	r5, r4, #49152
	add	r5, r5, #64
.L82:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	GetVoLastImageID
.L92:
	add	r10, r8, #560
	add	r7, r8, #576
	mov	r0, r4
	bl	VC1_SetImgFormat
	mov	r2, #1
	mov	r0, #0
	str	r2, [r8, #188]
	mov	r1, #0
	strd	r0, [r10]
	strd	r0, [r7, #-8]
	ldr	r1, [r8, #156]
	ldr	r0, [r6, #416]
	bl	FSP_SetDisplay
	ldr	r3, [r5, #3900]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L95
.L79:
	add	r5, r4, #49152
	ldr	r0, [r6, #416]
	add	r5, r5, #64
	str	r8, [sp]
	mov	r2, r4
	mov	r1, #1
	mov	r3, r5
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L80
	ldr	r1, [r8, #156]
	mov	r0, r4
	bl	VC1DEC_RecycleImage
	ldr	r1, [r8, #560]
	ldr	r0, [r6, #416]
	mov	r8, #0
	bl	FreeUsdByDec
	mov	r9, #0
	strd	r8, [r10]
	ldr	r0, [r6, #416]
	ldr	r1, [r7, #-8]
	bl	FreeUsdByDec
	strd	r8, [r7, #-8]
	b	.L82
.L80:
	ldr	r3, [r8, #160]
	cmp	r3, #100
	bhi	.L82
	ldr	r3, [r8, #112]
	cmp	r3, #31
	bls	.L82
	ldr	r3, [r8, #116]
	cmp	r3, #31
	movhi	r0, #0
	bls	.L82
.L93:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L95:
	strb	r2, [r3, #1]
	mov	r1, #0
	ldr	r2, [r5, #3900]
	mov	r3, #3
	strb	r1, [r2]
	strb	r3, [r9, #441]
	b	.L79
.L83:
	mvn	r0, #0
	b	.L93
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #48)
	sub	sp, sp, #48
	mov	r5, r1
	sub	r1, r1, #32
	movw	r3, #8160
	cmp	r1, r3
	mov	r7, r0
	mov	r6, r2
	bhi	.L97
	sub	r2, r2, #32
	cmp	r2, r3
	bhi	.L97
	add	ip, r0, #28160
	mov	lr, #0
	add	ip, ip, #184
	mov	r4, lr
.L100:
	str	lr, [ip, #-468]
	add	lr, lr, #1
	cmp	lr, #5
	strb	r4, [ip, #-1663]
	strb	r4, [ip, #-1664]
	add	ip, ip, #1664
	bne	.L100
	ldr	r3, [r7]
	ldr	r4, [r3, #708]
	ldr	r2, [r3, #28]
	cmp	r4, #0
	bne	.L101
	cmp	r2, #25
	beq	.L125
.L121:
	mov	r4, lr
	mov	r8, #3
	mov	r9, #2
.L102:
	ldr	r3, .L128
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #68
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r2, [r7]
	str	r6, [fp, #-40]
	mov	r3, #0
	str	r5, [fp, #-60]
	ldr	r2, [r2, #28]
	str	r6, [fp, #-56]
	cmp	r2, #24
	str	r5, [fp, #-44]
	strb	r3, [fp, #-68]
	strb	r3, [fp, #-67]
	beq	.L112
	uxtb	r3, r9
	strb	r8, [fp, #-65]
	strb	r4, [fp, #-63]
.L117:
	add	r6, r7, #49152
	sub	r1, fp, #68
	strb	r3, [fp, #-62]
	mov	r5, #0
	ldr	r0, [r6, #416]
	strb	r5, [fp, #-66]
	bl	FSP_ConfigInstance
	subs	r4, r0, #0
	bne	.L113
	ldr	r1, [r7]
	sub	r3, fp, #72
	ldr	r0, [r6, #416]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L126
	cmp	r0, #0
	bne	.L115
.L119:
	mov	r4, #1
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L97:
	mov	r3, r6
	mov	r2, r5
	ldr	r1, .L128+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r4, #0
.L124:
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L101:
	cmp	r2, #25
	bne	.L121
	ldr	r2, [r3, #668]
	ldr	r1, [r3, #672]
	cmp	r5, r2
	bhi	.L109
	cmp	r6, r1
	bhi	.L109
	ldr	r1, [r3, #696]
	ldr	r2, [r3, #692]
	cmp	r1, #0
	add	r8, r2, #2
	moveq	r9, #1
	bne	.L127
.L111:
	ldrb	r4, [r3, #716]	@ zero_extendqisi2
	b	.L102
.L113:
	ldr	r1, .L128+8
	mov	r0, r5
	bl	dprint_vfmw
	mov	r4, r5
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L125:
	ldr	r2, [r3, #668]
	cmp	r5, r2
	ldrhi	r0, [r3, #672]
	bhi	.L104
	ldr	r1, [r3, #672]
	cmp	r6, r1
	mov	r0, r1
	bhi	.L104
	add	r0, r7, #12288
	ldrb	r0, [r0, #440]	@ zero_extendqisi2
	tst	r0, #8
	bne	.L119
	ldr	ip, [r3, #696]
	ldr	r0, [r3, #692]
	cmp	ip, #0
	add	r8, r0, #2
	moveq	r9, #1
	beq	.L107
	add	r9, r0, #1
	cmp	r9, #17
	movcs	r9, #17
.L107:
	ldrb	r4, [r3, #716]	@ zero_extendqisi2
	mov	r6, r1
	mov	r5, r2
	b	.L102
.L112:
	mov	r3, #1
	strb	r4, [fp, #-63]
	strb	r3, [fp, #-65]
	b	.L117
.L109:
	str	r1, [sp, #4]
	mov	r3, r6
	str	r2, [sp]
	mov	r0, #0
	mov	r2, r5
	ldr	r1, .L128+12
	bl	dprint_vfmw
	mov	r4, #0
	b	.L124
.L126:
	add	r0, r6, #64
	bl	ResetVoQueue
.L115:
	ldr	r1, .L128+16
	mov	r0, #0
	bl	dprint_vfmw
	b	.L124
.L127:
	add	r9, r2, #1
	cmp	r9, #17
	movcs	r9, #17
	b	.L111
.L104:
	str	r0, [sp, #4]
	mov	r3, r6
	str	r2, [sp]
	mov	r0, #0
	mov	r2, r5
	ldr	r1, .L128+12
	bl	dprint_vfmw
	b	.L124
.L129:
	.align	2
.L128:
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #32768
	add	r7, r0, #49152
	mov	r5, r0
	mov	r4, r1
	add	r8, r0, #12288
	ldr	r1, [r6, #3988]
	ldr	r0, [r7, #416]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L153
	add	r3, r8, #3536
	ldrsb	r2, [r3]
	add	r3, r5, #20480
	cmp	r2, #0
	bne	.L154
	strb	r2, [r4, #5]
	add	r1, r5, #45056
	ldrb	ip, [r3, #2604]	@ zero_extendqisi2
	strb	ip, [r4, #4]
	ldrb	ip, [r3, #2855]	@ zero_extendqisi2
	cmp	ip, #4
	moveq	ip, #3
	strb	ip, [r4, #3]
	ldr	ip, [r1, #4068]
	sub	ip, ip, #1
	strh	ip, [r4, #80]	@ movhi
	ldr	ip, [r1, #4064]
	strb	r2, [r4, #8]
	sub	ip, ip, #1
	strh	ip, [r4, #78]	@ movhi
	ldrb	ip, [r3, #2607]	@ zero_extendqisi2
	strb	r2, [r4, #9]
	strb	ip, [r4, #6]
	ldrb	ip, [r8, #448]	@ zero_extendqisi2
	strb	r2, [r4, #10]
	strb	r2, [r4, #11]
	strb	ip, [r4, #7]
	ldrb	ip, [r3, #2610]	@ zero_extendqisi2
	strb	ip, [r4, #13]
	ldrb	ip, [r3, #2614]	@ zero_extendqisi2
	strb	ip, [r4, #14]
	ldrb	ip, [r3, #2874]	@ zero_extendqisi2
	strb	ip, [r4, #12]
	ldrb	ip, [r3, #2884]	@ zero_extendqisi2
	strb	ip, [r4, #18]
	ldrb	ip, [r3, #2859]	@ zero_extendqisi2
	strb	ip, [r4, #19]
	ldrb	ip, [r3, #2858]	@ zero_extendqisi2
	strb	ip, [r4, #17]
	ldr	ip, [r3, #2944]
	strb	ip, [r4, #16]
	ldrb	ip, [r3, #2618]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L145
	ldrb	r2, [r3, #2858]	@ zero_extendqisi2
	cmp	r2, #8
	movhi	r2, #0
	movls	r2, #1
	strb	r2, [r4, #20]
.L146:
	ldrb	ip, [r3, #2878]	@ zero_extendqisi2
	add	r2, r4, #12288
	strb	ip, [r4, #24]
	ldrb	ip, [r3, #2877]	@ zero_extendqisi2
	strb	ip, [r4, #23]
	ldrb	ip, [r3, #2612]	@ zero_extendqisi2
	strb	ip, [r4, #22]
	ldrb	ip, [r3, #2876]	@ zero_extendqisi2
	strb	ip, [r4, #25]
	ldr	ip, [r3, #2888]
	strb	ip, [r4, #26]
	ldrb	ip, [r3, #2879]	@ zero_extendqisi2
	strb	ip, [r4, #21]
	ldrb	ip, [r3, #2863]	@ zero_extendqisi2
	strb	ip, [r4, #27]
	ldrb	ip, [r3, #2864]	@ zero_extendqisi2
	strb	ip, [r4, #28]
	ldr	ip, [r6, #3900]
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #30]
	ldr	ip, [r6, #3896]
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #29]
	ldrb	ip, [r3, #2861]	@ zero_extendqisi2
	strb	ip, [r4, #31]
	ldrb	ip, [r3, #2983]	@ zero_extendqisi2
	strb	ip, [r4, #32]
	ldrb	ip, [r3, #2873]	@ zero_extendqisi2
	strb	ip, [r4, #35]
	ldrb	ip, [r3, #2613]	@ zero_extendqisi2
	strb	ip, [r4, #36]
	ldrb	ip, [r3, #2869]	@ zero_extendqisi2
	strb	ip, [r4, #37]
	ldrb	ip, [r3, #2871]	@ zero_extendqisi2
	strb	ip, [r4, #38]
	ldrb	ip, [r3, #2872]	@ zero_extendqisi2
	strb	ip, [r4, #39]
	ldrb	ip, [r3, #2870]	@ zero_extendqisi2
	strb	ip, [r4, #40]
	ldr	ip, [r3, #2936]
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #42]
	ldr	ip, [r3, #2932]
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #43]
	ldr	ip, [r3, #2940]
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #45]
	ldrb	ip, [r3, #2856]	@ zero_extendqisi2
	strb	ip, [r4, #50]
	ldrb	ip, [r3, #2868]	@ zero_extendqisi2
	strb	ip, [r4, #49]
	ldrb	ip, [r3, #2867]	@ zero_extendqisi2
	strb	ip, [r4, #48]
	ldr	ip, [r3, #2948]
	str	ip, [r2, #84]
	ldrb	ip, [r3, #2616]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L155
	mov	ip, #0
	mov	lr, ip
	mov	r9, ip
	mov	r10, ip
.L149:
	strb	r10, [r4, #59]
	strb	r9, [r4, #60]
	strb	lr, [r4, #61]
	str	ip, [r0, #400]
	ldrb	r3, [r3, #2604]	@ zero_extendqisi2
	strb	r3, [r0, #378]
.L143:
	ldr	r3, [r6, #3996]
	add	r5, r5, #12736
	ldr	r0, .L159
	add	r5, r5, #5
	strh	r3, [r4, #72]	@ movhi
	ldr	r3, [r6, #3928]
	strh	r3, [r4, #74]	@ movhi
	ldr	r3, [r6, #3932]
	strh	r3, [r4, #76]	@ movhi
	ldrb	r3, [r8, #451]	@ zero_extendqisi2
	strb	r3, [r4, #63]
	ldrb	r3, [r8, #452]	@ zero_extendqisi2
	strb	r3, [r4, #62]
	ldr	ip, [r6, #3912]
	adds	r3, ip, #127
	addmi	r3, ip, #254
	mov	r3, r3, asr #7
	mov	r3, r3, asl #4
	str	r3, [r2, #116]
	ldr	r3, [r1, #4088]
	str	r3, [r2, #120]
	ldr	r3, [r1, #4092]
	str	r3, [r2, #124]
	ldr	r3, [r7]
	str	r3, [r2, #128]
	ldr	r3, [r7, #4]
	str	r3, [r2, #132]
	ldr	r3, [r7, #8]
	str	r3, [r2, #136]
	ldr	r3, [r7, #12]
	str	r3, [r2, #140]
	ldr	r3, [r7, #16]
	str	r3, [r2, #144]
	str	r5, [r4, #64]
	ldr	r2, [r7, #416]
	ldrh	r3, [r4, #78]
	ldr	r2, [r0, r2, asl #2]
	add	r3, r3, #1
	str	r3, [r2, #1484]
	ldr	r3, [r7, #416]
	ldr	r2, [r0, r3, asl #2]
	ldrh	r3, [r4, #80]
	add	r3, r3, #1
	str	r3, [r2, #1488]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L154:
	ldr	r2, [r3, #2996]
	add	r1, r5, #45056
	mov	lr, #1664
	strb	r2, [r4, #5]
	ldrb	r2, [r3, #2620]	@ zero_extendqisi2
	strb	r2, [r4, #4]
	ldrb	r2, [r3, #2953]	@ zero_extendqisi2
	cmp	r2, #4
	moveq	r2, #3
	strb	r2, [r4, #3]
	ldr	r2, [r3, #2996]
	cmp	r2, #3
	ldr	r2, [r1, #4068]
	addeq	r2, r2, #1
	addeq	r2, r2, r2, lsr #31
	moveq	r2, r2, asr #1
	sub	r2, r2, #1
	strh	r2, [r4, #80]	@ movhi
	ldr	r2, [r1, #4064]
	sub	r2, r2, #1
	strh	r2, [r4, #78]	@ movhi
	ldrb	r2, [r3, #2993]	@ zero_extendqisi2
	strb	r2, [r4, #8]
	ldrb	r2, [r3, #2796]	@ zero_extendqisi2
	strb	r2, [r4, #6]
	ldrb	r2, [r3, #2984]	@ zero_extendqisi2
	strb	r2, [r4, #9]
	ldrb	r2, [r8, #448]	@ zero_extendqisi2
	strb	r2, [r4, #7]
	ldr	ip, [r6, #3952]
	mla	ip, lr, ip, r8
	add	ip, ip, #15360
	ldrb	r2, [ip, #59]	@ zero_extendqisi2
	strb	r2, [r4, #10]
	ldr	r2, [r6, #3968]
	mla	r2, lr, r2, r8
	add	r2, r2, #15360
	add	r2, r2, #56
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	strb	r2, [r4, #11]
	ldrb	r2, [r3, #2982]	@ zero_extendqisi2
	strb	r2, [r4, #15]
	ldrb	r2, [r3, #2797]	@ zero_extendqisi2
	strb	r2, [r4, #13]
	ldrb	r2, [r3, #2801]	@ zero_extendqisi2
	strb	r2, [r4, #14]
	ldrb	r2, [r3, #2952]	@ zero_extendqisi2
	strb	r2, [r4, #12]
	ldrb	r2, [r3, #3120]	@ zero_extendqisi2
	strb	r2, [r4, #18]
	ldrb	r2, [r3, #2955]	@ zero_extendqisi2
	strb	r2, [r4, #19]
	ldrb	r2, [r3, #2962]	@ zero_extendqisi2
	strb	r2, [r4, #17]
	ldr	r2, [r3, #3068]
	strb	r2, [r4, #16]
	ldrb	r2, [r3, #2802]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L136
	ldrb	r2, [r3, #2962]	@ zero_extendqisi2
	cmp	r2, #8
	movhi	r2, #0
	movls	r2, #1
	strb	r2, [r4, #20]
.L137:
	ldrb	ip, [r3, #3114]	@ zero_extendqisi2
	add	r2, r4, #12288
	strb	ip, [r4, #24]
	ldrb	ip, [r3, #3113]	@ zero_extendqisi2
	strb	ip, [r4, #23]
	ldrb	ip, [r3, #2799]	@ zero_extendqisi2
	strb	ip, [r4, #22]
	ldrb	ip, [r3, #3112]	@ zero_extendqisi2
	strb	ip, [r4, #25]
	ldr	ip, [r3, #3124]
	strb	ip, [r4, #26]
	ldrb	ip, [r3, #3115]	@ zero_extendqisi2
	strb	ip, [r4, #21]
	ldrb	ip, [r3, #2966]	@ zero_extendqisi2
	strb	ip, [r4, #27]
	ldrb	ip, [r3, #2967]	@ zero_extendqisi2
	strb	ip, [r4, #28]
	ldrb	ip, [r3, #2985]	@ zero_extendqisi2
	strb	ip, [r4, #34]
	ldrb	ip, [r3, #2977]	@ zero_extendqisi2
	strb	ip, [r4, #33]
	ldrb	lr, [r3, #2993]	@ zero_extendqisi2
	ldr	ip, [r6, #3900]
	add	ip, ip, lr
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #30]
	ldr	ip, [r6, #3896]
	ldrb	lr, [r3, #2993]	@ zero_extendqisi2
	add	ip, ip, lr
	ldrb	ip, [ip, #1029]	@ zero_extendqisi2
	strb	ip, [r4, #29]
	ldrb	ip, [r3, #2965]	@ zero_extendqisi2
	strb	ip, [r4, #31]
	ldrb	ip, [r3, #2983]	@ zero_extendqisi2
	strb	ip, [r4, #32]
	ldrb	ip, [r3, #2976]	@ zero_extendqisi2
	strb	ip, [r4, #35]
	ldrb	ip, [r3, #2800]	@ zero_extendqisi2
	strb	ip, [r4, #36]
	ldrb	ip, [r3, #2972]	@ zero_extendqisi2
	strb	ip, [r4, #37]
	ldrb	ip, [r3, #2974]	@ zero_extendqisi2
	strb	ip, [r4, #38]
	ldrb	ip, [r3, #2975]	@ zero_extendqisi2
	strb	ip, [r4, #39]
	ldrb	ip, [r3, #2973]	@ zero_extendqisi2
	strb	ip, [r4, #40]
	ldrb	ip, [r3, #2989]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #46]
	ldrb	ip, [r3, #2987]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #42]
	ldrb	ip, [r3, #2991]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #44]
	ldrb	ip, [r3, #2992]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #41]
	ldrb	ip, [r3, #2986]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #43]
	ldrb	ip, [r3, #2990]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #47]
	ldrb	ip, [r3, #2988]	@ zero_extendqisi2
	clz	ip, ip
	mov	ip, ip, lsr #5
	strb	ip, [r4, #45]
	ldrb	ip, [r3, #2954]	@ zero_extendqisi2
	strb	ip, [r4, #50]
	ldrb	ip, [r3, #2971]	@ zero_extendqisi2
	strb	ip, [r4, #49]
	ldrb	ip, [r3, #2970]	@ zero_extendqisi2
	strb	ip, [r4, #48]
	ldrb	ip, [r3, #2981]	@ zero_extendqisi2
	strb	ip, [r4, #53]
	ldrb	ip, [r3, #2978]	@ zero_extendqisi2
	strb	ip, [r4, #54]
	ldrb	ip, [r3, #2979]	@ zero_extendqisi2
	strb	ip, [r4, #51]
	ldrb	ip, [r3, #2980]	@ zero_extendqisi2
	strb	ip, [r4, #52]
	ldr	ip, [r3, #3100]
	str	ip, [r2, #84]
	ldr	ip, [r1, #4040]
	str	ip, [r2, #88]
	ldr	ip, [r1, #4044]
	str	ip, [r2, #92]
	ldrb	ip, [r3, #2805]	@ zero_extendqisi2
	strb	ip, [r4, #55]
	ldrb	ip, [r3, #2806]	@ zero_extendqisi2
	strb	ip, [r4, #56]
	ldrb	ip, [r3, #2807]	@ zero_extendqisi2
	strb	ip, [r4, #57]
	ldrb	ip, [r3, #2808]	@ zero_extendqisi2
	strb	ip, [r4, #58]
	ldr	ip, [r3, #2996]
	cmp	ip, #3
	beq	.L156
	ldrb	ip, [r3, #2805]	@ zero_extendqisi2
	strb	ip, [r0, #373]
	ldrb	ip, [r3, #2806]	@ zero_extendqisi2
	strb	ip, [r0, #374]
	ldrb	ip, [r3, #2807]	@ zero_extendqisi2
	strb	ip, [r0, #375]
	ldrb	ip, [r3, #2808]	@ zero_extendqisi2
	strb	ip, [r0, #376]
.L142:
	ldrb	r3, [r3, #2620]	@ zero_extendqisi2
	strb	r3, [r0, #378]
	ldrb	r3, [r8, #451]	@ zero_extendqisi2
	strb	r3, [r4, #63]
	b	.L143
.L145:
	cmp	ip, #1
	beq	.L157
	cmp	ip, #2
	movne	r2, #1
	strb	r2, [r4, #20]
	b	.L146
.L136:
	cmp	r2, #1
	beq	.L158
	cmp	r2, #2
	moveq	r2, #0
	movne	r2, #1
	strb	r2, [r4, #20]
	b	.L137
.L158:
	ldrb	r2, [r3, #2963]	@ zero_extendqisi2
	adds	r2, r2, #0
	movne	r2, #1
	strb	r2, [r4, #20]
	b	.L137
.L157:
	ldrb	r2, [r3, #2860]	@ zero_extendqisi2
	adds	r2, r2, #0
	movne	r2, #1
	strb	r2, [r4, #20]
	b	.L146
.L156:
	ldrb	ip, [r3, #2993]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L141
	ldrb	ip, [r3, #2805]	@ zero_extendqisi2
	strb	ip, [r0, #373]
	ldrb	ip, [r3, #2806]	@ zero_extendqisi2
	strb	ip, [r0, #374]
	ldrb	ip, [r3, #2807]	@ zero_extendqisi2
	strb	ip, [r0, #375]
	ldrb	ip, [r3, #2808]	@ zero_extendqisi2
	strb	ip, [r0, #376]
	ldrb	ip, [r3, #2993]	@ zero_extendqisi2
.L141:
	cmp	ip, #1
	bne	.L142
	ldrb	ip, [r3, #2805]	@ zero_extendqisi2
	strb	ip, [r0, #368]
	ldrb	ip, [r3, #2806]	@ zero_extendqisi2
	strb	ip, [r0, #369]
	ldrb	ip, [r3, #2807]	@ zero_extendqisi2
	strb	ip, [r0, #370]
	ldrb	ip, [r3, #2808]	@ zero_extendqisi2
	strb	ip, [r0, #371]
	b	.L142
.L155:
	ldr	lr, [r6, #3952]
	mov	r10, #1664
	ldr	ip, [r6, #3968]
	ldrb	r9, [r3, #2855]	@ zero_extendqisi2
	mla	lr, r10, lr, r8
	sub	r9, r9, #2
	mla	ip, r10, ip, r8
	tst	r9, #253
	add	lr, lr, #14336
	ldrneb	r10, [r3, #2854]	@ zero_extendqisi2
	add	ip, ip, #14336
	ldrb	r9, [lr, #58]	@ zero_extendqisi2
	ldrb	lr, [ip, #58]	@ zero_extendqisi2
	movne	ip, r10
	moveq	ip, lr
	moveq	r10, lr
	b	.L149
.L153:
	movw	r2, #1126
	ldr	r1, .L159+4
	mov	r0, #22
	sub	sp, fp, #44
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L160:
	.align	2
.L159:
	.word	s_pstVfmwChan
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
	ldr	ip, .L162
	add	r1, r0, #23808
	ldr	lr, [r3, #3556]
	add	r0, r2, #84
	add	r1, r1, #232
	ldr	ip, [ip, #52]
	strb	lr, [r2, #70]
	ldr	r3, [r3, #3556]
	mov	r2, r3, asl #6
	sub	r2, r2, r3, asl #4
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L163:
	.align	2
.L162:
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r0, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L165
	add	r3, r0, #8192
	ldr	r3, [r3, #2556]
	cmp	r3, #3
	beq	.L170
.L165:
	add	r3, r0, #32768
	add	r6, r1, #12288
	ldr	r3, [r3, #3632]
	sub	r3, r3, #1
	str	r3, [r6, #164]
.L166:
	add	r4, r0, #20480
	ldr	r3, [r6, #256]
	mov	ip, #0
	mov	r5, #1664
	ldr	r2, [r4, #3496]
	and	r7, r3, #1
	movw	lr, #15088
	add	r3, r2, #63
	cmp	r2, ip
	movlt	r2, r3
	mov	r2, r2, asr #6
	mov	r2, r2, asl #4
	add	r3, r2, #536870913
	add	r3, r3, #81920
	orr	r3, r3, r7, asl #30
	str	r3, [r6, #168]
	ldr	r2, [r4, #3528]
	ldr	r3, [r4, #3512]
	ldr	r6, [r4, #3456]
	mla	r4, r5, r2, r0
	mla	r0, r5, r3, r0
	ldr	r2, [r6, #1136]
	and	r2, r2, #3
	ldr	r3, [r4, lr]
	mov	r2, r2, asl #4
	ldr	r0, [r0, lr]
	and	r3, r3, #3
	strb	ip, [r1, #69]
	orr	r3, r2, r3, asl #2
	and	r2, r0, #3
	orr	r3, r3, r2
	strb	r3, [r1, #68]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L170:
	add	r2, r0, #32768
	add	r6, r1, #12288
	ldr	r3, [r2, #3628]
	ldr	r2, [r2, #3624]
	add	r3, r3, #1
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	mul	r3, r2, r3
	sub	r3, r3, #1
	str	r3, [r6, #164]
	b	.L166
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
	ldrb	r2, [r3, #443]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L183
.L173:
	add	r5, r4, #49152
	ldr	r0, [r5, #416]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L175
	ldr	r0, [r5, #416]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L184
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L183:
	ldrb	r3, [r3, #449]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L173
.L175:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L184:
	ldr	r0, [r5, #416]
	add	r1, r5, #64
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #49152
	add	r4, r0, #32768
	mov	r5, r0
	mov	r1, #1
	ldr	r0, [r6, #416]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #3988]
	blt	.L205
	add	r3, r5, #26624
	add	r2, r3, #56
	ldrh	r3, [r3, #56]
	cmp	r3, #0
	beq	.L189
	add	r3, r5, #28160
	add	lr, r5, #34816
	add	r3, r3, #184
	add	lr, lr, #184
.L192:
	ldrh	ip, [r3]
	mov	r2, r3
	add	r3, r3, #1664
	cmp	ip, #0
	beq	.L189
	cmp	r3, lr
	bne	.L192
	mov	r1, r0
	mov	r2, #1
	ldr	r0, [r6, #416]
	bl	FSP_ClearLogicFs
	ldr	r1, .L206
	mov	r0, #1
	bl	dprint_vfmw
	add	r3, r5, #26624
	add	r5, r5, #34816
	add	r3, r3, #57
	add	r5, r5, #185
	mov	r2, #0
.L194:
	strb	r2, [r3]
	add	r3, r3, #1664
	strb	r2, [r3, #-1665]
	cmp	r3, r5
	bne	.L194
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L189:
	mov	r3, #0
	str	r2, [r4, #3896]
	mov	r1, #1
	strb	r1, [r2, #1]
.L191:
	ldr	r1, [r4, #3896]
	uxtb	r2, r3
	add	r1, r1, r3
	strb	r2, [r1, #3]
	ldr	r1, [r4, #3896]
	add	r1, r1, r3
	strb	r2, [r1, #259]
	ldr	r1, [r4, #3896]
	add	r1, r1, r3
	strb	r2, [r1, #515]
	ldr	r1, [r4, #3896]
	add	r1, r1, r3
	add	r3, r3, #1
	cmp	r3, #256
	strb	r2, [r1, #771]
	bne	.L191
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L205:
	ldr	r1, .L206+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r4, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L187
	ldr	r1, [r7, #560]
	add	r9, r7, #576
	ldr	r0, [r6, #416]
	add	r8, r7, #592
	bl	FreeUsdByDec
	ldr	r1, [r7, #568]
	ldr	r0, [r6, #416]
	add	r10, r7, #560
	bl	FreeUsdByDec
	ldr	r1, [r7, #576]
	ldr	r0, [r6, #416]
	bl	FreeUsdByDec
	ldr	r1, [r7, #584]
	ldr	r0, [r6, #416]
	bl	FreeUsdByDec
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r10]
	strd	r0, [r9, #-8]
	strd	r0, [r9]
	strd	r0, [r8, #-8]
	ldr	r1, [r4, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #3980]
	ldr	r0, [r6, #416]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	str	r3, [r4, #3980]
	str	r3, [r4, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #3984]
	str	r0, [r4, #3980]
.L187:
	add	r1, r5, #49152
	mov	r5, #0
	add	r1, r1, #64
	str	r5, [r4, #3976]
	ldr	r0, [r6, #416]
	bl	FSP_ClearNotInVoQueue
	ldr	r1, .L206+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L207:
	.align	2
.L206:
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
	ldr	ip, .L209
	ldr	r3, [r0, #3476]
	ldr	r2, [r0, #3496]
	add	r3, r3, #1
	ldr	r1, [r0, #3460]
	ldr	r0, [r0, #3456]
	add	r3, r3, r3, lsr #31
	ldr	ip, [ip, #52]
	ldr	r1, [r1, #1544]
	mov	r3, r3, asr #1
	ldr	r0, [r0, #1544]
	mov	r3, r3, asl #5
	mul	r3, r2, r3
	add	r2, r3, r3, lsl #1
	mov	r2, r2, asr #1
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L210:
	.align	2
.L209:
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
	ldr	r6, .L286
	mov	r7, r0
	mov	r10, r1
	mov	r4, r2
	mov	r1, #0
	mov	r2, #256
	sub	r0, fp, #556
	mov	r5, r3
	bl	memset
	mov	r2, #256
	mov	r1, #0
	sub	r0, fp, #300
	bl	memset
	add	r0, r7, #23808
	ldr	r3, [r6, #48]
	mov	r2, #16
	mov	r1, #0
	add	r0, r0, #176
	blx	r3
	add	r0, r7, #23808
	ldr	r3, [r6, #48]
	add	r0, r0, #192
	mov	r2, #16
	mov	r1, #0
	blx	r3
	ldrsb	r3, [r7, #4]
	bics	r3, r3, #2
	bne	.L212
	cmp	r10, #1
	beq	.L273
	cmp	r10, #2
	addne	ip, r7, #20480
	beq	.L274
.L221:
	mov	r0, #1
	mov	r3, #0
	str	r3, [ip, #3524]
	str	r3, [ip, #3508]
.L255:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L212:
	add	r6, r7, #8192
	ldrb	r3, [r6, #2516]	@ zero_extendqisi2
	cmp	r3, #1
	ldrb	r3, [r6, #2553]	@ zero_extendqisi2
	beq	.L275
	cmp	r3, #0
	beq	.L276
.L231:
	add	ip, r7, #20480
	ldr	r3, [ip, #3464]
	ldr	r9, [r3, #1196]
	mov	r8, r9
.L230:
	cmp	r10, #1
	beq	.L277
.L232:
	cmp	r10, #2
	bne	.L221
	cmp	r4, #0
	mov	r2, #0
	mov	r3, #1
	str	r10, [ip, #3524]
	str	r10, [ip, #3508]
	strb	r2, [ip, #3504]
	strb	r3, [ip, #3505]
	beq	.L244
	cmp	r5, r2
	beq	.L278
.L244:
	ldr	r3, [ip, #3464]
	ldr	lr, [ip, #3456]
	ldr	r2, [r3, #1196]
.L271:
	ldr	r0, [ip, #3460]
	mov	r1, #1
	str	r2, [ip, #3512]
	mov	r3, #0
	strb	r1, [ip, #3521]
	strb	r3, [ip, #3520]
	ldr	r1, [r0, #1196]
	str	r1, [ip, #3528]
	ldr	r0, [r0, #1196]
	str	r0, [ip, #3532]
	ldr	r0, [r6, #2640]
	ldr	r8, [lr, #1196]
	cmp	r0, #1
	bls	.L279
	bics	r0, r0, #2
	beq	.L250
.L249:
	mov	ip, #1664
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
.L251:
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
	bne	.L251
	mov	r0, #1
.L283:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L276:
	add	ip, r7, #20480
	cmp	r10, #1
	ldr	r2, [ip, #3464]
	ldr	r3, [ip, #3460]
	ldr	r9, [r2, #1196]
	ldr	r8, [r3, #1196]
	bne	.L232
.L277:
	ldrb	r3, [r6, #2544]	@ zero_extendqisi2
	cmp	r4, #0
	mov	r2, #0
	strb	r10, [ip, #3505]
	add	r3, r3, #1
	str	r2, [ip, #3524]
	str	r3, [ip, #3508]
	strb	r2, [ip, #3504]
	beq	.L233
	cmp	r5, r2
	beq	.L280
.L233:
	ldr	r3, [ip, #3464]
	ldr	r2, [ip, #3456]
	ldr	r5, [r3, #1196]
.L270:
	str	r5, [ip, #3512]
	ldr	r3, [r3, #1196]
	str	r3, [ip, #3516]
	ldr	r3, [r6, #2640]
	ldr	r4, [r2, #1196]
	cmp	r3, #1
	bls	.L281
.L237:
	bics	r2, r3, #2
	bne	.L238
	ldrb	r2, [r6, #2526]	@ zero_extendqisi2
	cmp	r2, #4
	beq	.L282
.L238:
	mov	r1, #1664
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
.L243:
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
	bne	.L243
	mov	r0, #1
	b	.L283
.L273:
	add	r2, r7, #20480
	ldr	r1, [r2, #3464]
	str	r10, [r2, #3508]
	str	r3, [r2, #3524]
	strb	r3, [r2, #3504]
	ldr	r4, [r1, #1196]
	str	r4, [r2, #3512]
	ldrb	r3, [r7, #3096]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r7, #8192
	beq	.L214
	ldr	r2, [r3, #2556]
	ldrb	r5, [r3, #2528]	@ zero_extendqisi2
	cmp	r2, #2
	ldrb	r6, [r3, #2529]	@ zero_extendqisi2
	ldrb	r0, [r3, #2526]	@ zero_extendqisi2
	beq	.L284
.L215:
	cmp	r0, #4
	beq	.L216
.L217:
	mov	r3, #1664
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
.L219:
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
	bne	.L219
	mov	r0, #1
	b	.L283
.L275:
	cmp	r3, #0
	beq	.L231
	add	ip, r7, #20480
	ldr	r2, [ip, #3460]
	ldr	r3, [ip, #3464]
	ldr	r9, [r2, #1196]
	ldr	r8, [r3, #1196]
	b	.L230
.L274:
	add	r2, r7, #20480
	mov	r1, #1
	ldr	r0, [r2, #3464]
	str	r1, [r2, #3524]
	str	r1, [r2, #3508]
	strb	r3, [r2, #3504]
	ldr	r1, [r2, #3460]
	ldr	r5, [r0, #1196]
	strb	r3, [r2, #3520]
	str	r5, [r2, #3512]
	ldr	r4, [r1, #1196]
	str	r4, [r2, #3528]
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
	beq	.L285
.L224:
	mov	r0, #1664
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
.L226:
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
	bne	.L226
	mov	r0, #1
	b	.L283
.L214:
	ldrb	r5, [r3, #2425]	@ zero_extendqisi2
	ldrb	r6, [r3, #2426]	@ zero_extendqisi2
	ldrb	r0, [r3, #2423]	@ zero_extendqisi2
	b	.L215
.L250:
	ldrb	r3, [r6, #2526]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L249
	b	.L255
.L280:
	ldr	r2, [ip, #3456]
	ldr	r3, [ip, #3464]
	ldr	r5, [r2, #1196]
	b	.L270
.L278:
	ldr	lr, [ip, #3456]
	ldr	r2, [lr, #1196]
	b	.L271
.L281:
	ldrb	r3, [r6, #2526]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L238
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
	mov	r2, #1664
	mul	r2, r2, r9
	mov	r0, #0
	add	r1, r2, #13952
	add	r2, r2, #14208
	add	r1, r1, #3
	add	r2, r2, #3
	add	r1, r7, r1
	add	r2, r7, r2
.L239:
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
	bne	.L239
	ldr	r3, [r6, #2640]
	b	.L237
.L279:
	ldrb	r0, [r6, #2526]	@ zero_extendqisi2
	cmp	r0, #4
	bne	.L249
	mov	r0, r3
	b	.L255
.L282:
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
	sub	r0, fp, #300
	mov	r3, #1
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r3, #1664
	mul	r2, r3, r8
	mov	r0, #0
	add	r1, r2, #14464
	add	r2, r2, #14720
	add	r1, r1, #3
	add	r2, r2, #3
	add	r1, r7, r1
	add	r2, r7, r2
.L242:
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
	bne	.L242
	b	.L238
.L284:
	ldr	r3, [r3, #2632]
	cmp	r3, #1
	bne	.L215
.L216:
	sub	r0, fp, #556
	mov	r3, #0
	mov	r2, r6
	mov	r1, r5
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r2, r6
	mov	r1, r5
	sub	r0, fp, #300
	mov	r3, #1
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r3, #1664
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
.L218:
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
	bne	.L218
	b	.L217
.L285:
	mov	r2, r8
	mov	r1, r6
	sub	r0, fp, #556
	mov	r3, #0
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r2, r8
	mov	r1, r6
	sub	r0, fp, #300
	mov	r3, #1
	mov	r10, #0
	bl	VC1_Vfmw_GenerateIntensityMap
	mov	r2, #1664
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
.L225:
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
	bne	.L225
	ldr	r4, [fp, #-560]
	b	.L224
.L287:
	.align	2
.L286:
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
	add	r8, r0, #12288
	add	r4, r0, #32768
	mov	r5, r0
	ldrb	r3, [r8, #3536]	@ zero_extendqisi2
	ldr	r9, [r4, #3912]
	cmp	r3, #0
	beq	.L289
	add	r3, r0, #20480
	add	r2, r8, #3776
	ldr	r3, [r3, #2996]
	cmp	r3, #3
	ldr	r3, [r4, #3916]
	addeq	r3, r3, #1
	addeq	r3, r3, r3, lsr #31
	moveq	r3, r3, asr #1
	mul	r3, r3, r9
	str	r3, [fp, #-48]
	mov	r3, r2
	ldrh	r2, [r2, #2]
	ldrh	r3, [r3]
	adds	r1, r2, #15
	addmi	r1, r2, #30
	adds	r0, r3, #15
	addmi	r0, r3, #30
	mov	r1, r1, asr #4
	mov	r0, r0, asr #4
.L341:
	mov	r2, r0, asl #4
	movw	ip, #8160
	sub	r3, r2, #32
	cmp	r3, ip
	mov	r3, r1, asl #4
	bhi	.L292
	sub	lr, r3, #32
	cmp	lr, ip
	bhi	.L292
	ldr	r3, [r4, #3924]
	cmp	r3, r1
	beq	.L376
.L295:
	ldr	r2, [r4, #3916]
	mov	r3, r9, asl #4
	add	r3, r3, #63
	str	r9, [r4, #3920]
	bic	r3, r3, #63
	str	r3, [r4, #3936]
	str	r2, [r4, #3924]
	ldrb	r3, [r8, #448]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L303
	mov	r3, #1
.L297:
	ldrb	r2, [r8, #443]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L377
.L300:
	ldrb	r2, [r8, #440]	@ zero_extendqisi2
	eor	r2, r2, #8
	ubfx	r2, r2, #3, #1
	orrs	r3, r3, r2
	bne	.L303
.L307:
	mov	r0, r5
	bl	VC1_Vfmw_GetImageBuffer
	subs	r6, r0, #0
	beq	.L378
	add	r10, r5, #49152
	ldr	r1, [r4, #3988]
	ldr	r0, [r10, #416]
	bl	FSP_GetFsImagePtr
	subs	r3, r0, #0
	beq	.L309
	ldr	r0, [r5]
	add	r2, r3, #608
	add	ip, r3, #624
	ldr	r1, .L386
	ldrd	r6, [r0, #56]
	mov	r0, #29
	strd	r6, [r2, #-8]
	ldr	lr, [r5]
	ldrd	r6, [lr, #72]
	strd	r6, [r2]
	ldr	lr, [r5]
	ldrd	r6, [lr, #88]
	strd	r6, [ip, #-8]
	mvn	r6, #0
	ldr	ip, [r5]
	mvn	r7, #0
	ldr	ip, [ip, #104]
	str	ip, [r3, #8]
	ldr	ip, [r5]
	ldr	ip, [ip, #108]
	str	ip, [r3, #12]
	ldr	ip, [r5]
	ldr	ip, [ip, #112]
	str	ip, [r3, #16]
	ldr	ip, [r5]
	ldr	ip, [ip, #128]
	str	ip, [r3, #20]
	ldr	r3, [r5]
	strd	r6, [r3, #56]
	ldrd	r6, [r2]
	ldrd	r2, [r2, #-8]
	strd	r6, [sp]
	bl	dprint_vfmw
.L309:
	ldrb	r3, [r8, #3536]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L310
	add	r3, r5, #20480
	ldr	r3, [r3, #2996]
	cmp	r3, #3
	beq	.L379
.L310:
	ldr	r3, [r4, #3896]
	mov	r2, #0
	str	r2, [r3, #1136]
.L311:
	ldr	r3, [r4, #3896]
	mov	r2, #0
	strb	r2, [r3, #2]
	ldrb	r3, [r8, #3536]	@ zero_extendqisi2
	cmp	r3, r2
	bne	.L312
	add	r3, r5, #20480
	ldrb	r2, [r3, #2616]	@ zero_extendqisi2
	cmp	r2, #1
	ldreqb	r2, [r3, #2854]	@ zero_extendqisi2
	ldreq	r3, [r4, #3896]
	streqb	r2, [r3, #2]
.L312:
	ldrb	r3, [r8, #442]	@ zero_extendqisi2
	cmp	r3, #5
	cmpne	r3, #1
	ldr	r3, [r4, #3976]
	movls	r7, #1
	movhi	r7, #0
	bhi	.L313
	cmp	r3, #0
	ldr	r2, [r4, #3980]
	ldr	r3, [r4, #3984]
	movne	r1, #2
	moveq	r1, #1
	cmp	r2, r3
	str	r1, [r4, #3976]
	beq	.L315
	ldr	r1, .L386+4
	mov	r0, #13
	bl	dprint_vfmw
	mov	r2, #0
	ldr	r1, [r4, #3980]
	ldr	r0, [r10, #416]
	bl	FSP_SetRef
	ldr	r3, [r4, #3984]
.L315:
	ldr	r1, [r4, #3988]
	mov	r2, #1
	str	r3, [r4, #3980]
	str	r1, [r4, #3984]
	ldr	r0, [r10, #416]
	bl	FSP_SetRef
	ldr	r1, [r4, #3980]
	ldr	r0, [r10, #416]
	bl	FSP_RemovePmv
	ldrb	r3, [r8, #442]	@ zero_extendqisi2
	cmp	r3, #5
	cmpne	r3, #1
	bhi	.L317
	ldr	r3, [r4, #3904]
	mov	r1, #0
	mov	r2, #1
	strb	r1, [r3]
	ldr	r3, [r4, #3896]
	strb	r2, [r3]
	ldr	r2, [r4, #3900]
	ldr	r3, [r4, #3896]
	str	r2, [r4, #3904]
	str	r3, [r4, #3900]
.L317:
	ldr	r1, [r4, #3988]
	ldr	r0, [r10, #416]
	bl	FSP_GetLogicFs
	ldr	r1, [r4, #3980]
	mov	r6, r0
	ldr	r0, [r10, #416]
	bl	FSP_GetLogicFs
	ldr	r1, [r4, #3984]
	mov	r7, r0
	ldr	r0, [r10, #416]
	bl	FSP_GetLogicFs
	cmp	r6, #0
	cmpne	r7, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	mov	ip, r0
	cmp	r3, #0
	bne	.L380
	ldrb	r3, [r8, #3536]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L381
.L323:
	mov	r3, #0
.L371:
	str	r3, [r6, #104]
	ldrb	r3, [r8, #442]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L382
.L325:
	ldr	r3, [r6, #640]
	cmp	r3, #0
	beq	.L326
	ldr	r2, [r6, #648]
	cmp	r2, #0
	beq	.L326
	ldr	r2, [r6, #644]
	cmp	r2, #0
	beq	.L326
	ldr	r2, [ip, #640]
	cmp	r2, #0
	ldrne	r3, [r2, #16]
	ldreq	r3, [r3, #16]
	str	r3, [r8, #112]
	ldr	r3, [r7, #640]
	mov	r7, #0
	cmp	r3, #0
	ldreq	r3, [r6, #640]
	ldr	r3, [r3, #16]
	str	r3, [r8, #108]
	ldr	r3, [r6, #640]
	ldr	r3, [r3, #16]
	str	r3, [r8, #232]
	ldr	r3, [r6, #652]
	str	r3, [r8, #116]
	ldr	r3, [ip, #652]
	str	r3, [r8, #120]
	ldr	r3, [r6, #644]
	ldr	r3, [r3, #16]
	str	r3, [r8, #104]
	ldr	r3, [r6, #644]
	ldr	r3, [r3, #32]
	str	r3, [r8, #184]
	ldr	r3, [r6, #644]
	ldr	r3, [r3, #76]
	str	r3, [r8, #188]
	ldr	r3, [r6, #644]
	ldr	r3, [r3, #60]
	str	r3, [r8, #192]
	ldr	r3, [r6, #644]
	ldr	r3, [r3, #64]
	str	r3, [r8, #196]
	ldr	r3, [r6, #640]
	ldr	r6, [r4, #3896]
	ldr	r0, [r3, #16]
	bl	MEM_Phy2Vir
	str	r7, [r6, #1548]
	str	r0, [r6, #1544]
	ldr	r0, [r8, #108]
	ldr	r6, [r4, #3904]
	bl	MEM_Phy2Vir
	str	r7, [r6, #1548]
	str	r0, [r6, #1544]
	ldr	r0, [r8, #112]
	ldr	r6, [r4, #3900]
	bl	MEM_Phy2Vir
	str	r7, [r6, #1548]
	str	r0, [r6, #1544]
.L302:
	add	r3, r8, #3536
	ldrsb	r3, [r3]
	cmp	r3, #0
	beq	.L383
	add	ip, r5, #20480
	ldr	r3, [r4, #3896]
	ldr	r2, [ip, #2996]
	strb	r2, [r3, #1027]
	ldr	r3, [r4, #3896]
	ldrb	r2, [ip, #2953]	@ zero_extendqisi2
	strb	r2, [r3, #1028]
	ldrb	r2, [ip, #2966]	@ zero_extendqisi2
	ldr	r3, [r4, #3896]
	sub	r0, r2, #2
	ldrb	r1, [ip, #2993]	@ zero_extendqisi2
	cmp	r0, #1
	movls	r2, #1
	bls	.L335
	cmp	r2, #4
	movne	r2, #0
	beq	.L384
.L335:
	add	r3, r3, r1
	strb	r2, [r3, #1029]
	ldr	r0, [r4, #3996]
	cmp	r0, #0
	beq	.L336
	sub	r0, r0, #1
	cmp	r0, #0
	mov	lr, r0
	moveq	r1, r0
	beq	.L338
	add	r2, r5, #36608
	mov	r1, #0
	add	r2, r2, #168
.L339:
	ldr	r3, [r2, #64]
	add	r2, r2, #48
	strb	r1, [r2, #-48]
	add	r1, r1, #1
	cmp	r1, lr
	mul	r3, r3, r9
	sub	r3, r3, #1
	str	r3, [r2, #-32]
	bne	.L339
	uxtb	r1, r1
.L338:
	mov	r3, r0, asl #6
	movw	r2, #24504
	sub	r0, r3, r0, asl #4
	ldr	r3, [fp, #-48]
	add	r0, r8, r0
	movw	lr, #24488
	sub	r3, r3, #1
	strb	r1, [r0, lr]
	str	r3, [r0, r2]
	ldrb	r1, [r8, #442]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L334
	ldrb	r3, [ip, #2983]	@ zero_extendqisi2
	add	r0, r5, #45056
	ldr	r2, [ip, #3100]
	mul	r2, r2, r3
	mov	r2, r2, asr #8
	str	r2, [r0, #4040]
	rsb	r3, r2, r3
	subs	r3, r3, #1
	movmi	r3, #0
	str	r3, [r0, #4044]
	b	.L334
.L292:
	ldr	r1, .L386+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r6, #0
.L368:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L289:
	ldr	r1, [r4, #3916]
	mov	r0, r9
	mul	r3, r1, r9
	str	r3, [fp, #-48]
	b	.L341
.L303:
	add	r10, r5, #49152
	add	r3, r5, #34816
	add	r3, r3, #184
	ldr	r1, [r4, #3984]
	str	r3, [r4, #3896]
	str	r3, [r4, #3908]
	str	r3, [r4, #3904]
	str	r3, [r4, #3900]
	ldr	r0, [r10, #416]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L299
	ldr	r1, [r6, #560]
	add	r3, r6, #592
	ldr	r0, [r10, #416]
	add	r7, r6, #576
	str	r3, [fp, #-52]
	bl	FreeUsdByDec
	ldr	r1, [r6, #568]
	add	r3, r6, #560
	ldr	r0, [r10, #416]
	str	r3, [fp, #-56]
	bl	FreeUsdByDec
	ldr	r1, [r6, #576]
	ldr	r0, [r10, #416]
	bl	FreeUsdByDec
	ldr	r6, [fp, #-52]
	ldr	r0, [r10, #416]
	ldr	r1, [r6, #-8]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-56]
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r3]
	strd	r0, [r7, #-8]
	strd	r0, [r7]
	strd	r0, [r6, #-8]
	ldr	r1, [r4, #3984]
	ldr	r0, [r10, #416]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #3980]
	ldr	r0, [r10, #416]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	str	r3, [r4, #3980]
	str	r3, [r4, #3984]
	ldr	r0, [r10, #416]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #3984]
	str	r0, [r4, #3980]
.L299:
	add	r3, r5, #26624
	add	r1, r5, #34816
	add	r3, r3, #57
	add	r1, r1, #185
	mov	r2, #0
	str	r2, [r4, #3976]
.L305:
	strb	r2, [r3]
	add	r3, r3, #1664
	strb	r2, [r3, #-1665]
	cmp	r3, r1
	bne	.L305
	add	r7, r5, #49152
	ldr	r0, [r10, #416]
	add	r7, r7, #64
	mov	r1, r7
	bl	FSP_ClearNotInVoQueue
	ldrb	r6, [r8, #440]	@ zero_extendqisi2
	and	r6, r6, #8
	ands	r6, r6, #255
	bne	.L307
	mov	r0, r7
	bl	ResetVoQueue
	ldr	r1, [r5]
	mov	r3, #2
	mov	r0, r5
	ldr	r2, [r1, #668]
	ldr	ip, [r1, #672]
	adds	r1, r2, #15
	addmi	r1, r2, #30
	adds	r2, ip, #15
	addmi	r2, ip, #30
	mov	r1, r1, asr #4
	str	r1, [r8, #272]
	mov	r2, r2, asr #4
	str	r2, [r8, #276]
	add	r2, r2, #1
	mov	r1, r1, asl #4
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
	mov	r2, r2, asl #5
	bl	VC1_ArrangeVahbMem
	cmp	r0, #1
	bne	.L306
	ldrb	r3, [r8, #440]	@ zero_extendqisi2
	orr	r3, r3, #8
	strb	r3, [r8, #440]
	b	.L307
.L378:
	ldr	r1, .L386+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L368
.L376:
	ldr	r3, [r4, #3920]
	cmp	r3, r0
	bne	.L295
	mov	r3, r9, asl #4
	add	r3, r3, #63
	bic	r3, r3, #63
	str	r3, [r4, #3936]
	ldrb	r3, [r8, #448]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #0
	bne	.L297
	b	.L300
.L313:
	cmp	r3, #1
	bhi	.L317
	ldr	r0, [r10, #416]
	mov	r2, #1
	ldr	r1, [r4, #3988]
	mov	r6, r7
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #3896]
	ldr	r1, .L386+16
	mov	r0, #1
	strb	r7, [r3]
	ldr	r3, [r4, #3896]
	strb	r7, [r3, #1]
	bl	dprint_vfmw
	b	.L368
.L380:
	ldr	r1, [r4, #3988]
	mov	r2, #1
	str	r0, [fp, #-48]
	ldr	r0, [r10, #416]
	bl	FSP_ClearLogicFs
	ldr	r1, [r4, #3896]
	mov	lr, #0
	ldr	ip, [fp, #-48]
	mov	r3, r6
	mov	r0, lr
	strb	lr, [r1]
	movw	r2, #2406
	ldr	r4, [r4, #3896]
	ldr	r1, .L386+20
	strb	lr, [r4, #1]
	stmia	sp, {r7, ip}
	bl	dprint_vfmw
	ldrb	r3, [r8, #442]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L375
.L321:
	ldr	r1, .L386+24
	mov	r0, #1
	bl	dprint_vfmw
	mov	r6, #0
	b	.L368
.L383:
	ldr	r1, [r4, #3896]
	add	r2, r5, #20480
	strb	r3, [r1, #1027]
	ldr	r1, [r4, #3896]
	ldrb	r0, [r2, #2855]	@ zero_extendqisi2
	strb	r0, [r1, #1028]
	ldrb	r0, [r2, #2863]	@ zero_extendqisi2
	ldr	r1, [r4, #3896]
	sub	ip, r0, #2
	cmp	ip, #1
	movls	r3, #1
	bls	.L333
	cmp	r0, #4
	beq	.L385
.L333:
	add	r6, r5, #45056
	strb	r3, [r1, #1030]
	strb	r3, [r1, #1029]
	mov	r2, #1
	mov	r3, #0
	str	r2, [r4, #3996]
	strb	r3, [r4, #4008]
	ldr	r0, [r8, #3568]
	ldr	r7, [r6, #4004]
	bl	BsPos
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	add	r0, r7, r0, asr #3
	str	r0, [r4, #4000]
	ldr	r0, [r8, #3568]
	ldr	r7, [r6, #4016]
	bl	BsPos
	add	r3, r0, #7
	cmp	r0, #0
	movlt	r0, r3
	add	r0, r7, r0, asr #3
	str	r0, [r4, #4012]
	ldr	r0, [r8, #3568]
	bl	BsPos
	and	r0, r0, #7
	str	r0, [r4, #4016]
	ldr	r0, [r8, #3568]
	ldr	r7, [r6, #4024]
	bl	BsPos
	mov	r7, r7, asl #3
	rsb	r0, r0, r7
	str	r0, [r4, #4020]
	ldr	r2, [r6, #4064]
	ldr	r3, [r6, #4068]
	mul	r3, r3, r2
	sub	r3, r3, #1
	str	r3, [r4, #4024]
	ldrb	r1, [r8, #442]	@ zero_extendqisi2
.L334:
	add	r0, r5, #12672
	mov	r3, #1
	add	r0, r0, #56
	mov	r2, #0
	bl	VC1_Vfmw_BuildRefList
	subs	r6, r0, #0
	movne	r6, #1
	bne	.L368
	add	r5, r5, #49152
	ldr	r1, [r4, #3988]
	mov	r2, #1
	ldr	r0, [r5, #416]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #3896]
	ldr	r1, .L386+28
	mov	r0, #1
	strb	r6, [r3]
	ldr	r3, [r4, #3896]
	strb	r6, [r3, #1]
	bl	dprint_vfmw
	b	.L368
.L381:
	add	r3, r5, #20480
	ldr	r3, [r3, #2996]
	cmp	r3, #3
	bne	.L323
	ldr	r3, .L386+32
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L323
	b	.L371
.L326:
	ldr	r0, [r10, #416]
	mov	r2, #1
	ldr	r1, [r4, #3988]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #3896]
	mov	r3, #0
	ldr	r1, .L386+36
	mov	r0, r3
	strb	r3, [r2]
	ldr	r2, [r4, #3896]
	strb	r3, [r2, #1]
	ldr	ip, [r6, #644]
	ldr	r3, [r4, #3988]
	ldr	r2, [r4, #3980]
	str	ip, [sp, #12]
	ldr	ip, [r6, #648]
	ldr	lr, [r4, #3984]
	str	ip, [sp, #8]
	ldr	ip, [r6, #640]
	str	lr, [sp]
	str	ip, [sp, #4]
	bl	dprint_vfmw
	ldrb	r3, [r8, #442]	@ zero_extendqisi2
	cmp	r3, #5
	bne	.L321
.L375:
	add	r5, r5, #45056
	ldr	r3, [r5, #4084]
	cmp	r3, #0
	subgt	r3, r3, #1
	strgt	r3, [r5, #4084]
	b	.L321
.L377:
	ldrb	r2, [r8, #449]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L302
	b	.L300
.L382:
	ldr	r3, [r7, #104]
	add	r2, r5, #45056
	str	r3, [r6, #104]
	ldr	r1, [r4, #3904]
	ldr	r3, [r4, #3896]
	ldr	r1, [r1, #1136]
	str	r1, [r3, #1136]
	ldr	r3, [r2, #4084]
	add	r3, r3, #1
	str	r3, [r2, #4084]
	b	.L325
.L336:
	mov	r3, #1
	mov	r1, r0
	str	r3, [r4, #3996]
	b	.L338
.L379:
	ldr	r3, .L386+32
	ldr	r3, [r3]
	cmp	r3, #1
	ldreq	r2, [r4, #3896]
	streq	r3, [r2, #1136]
	bne	.L310
	b	.L311
.L306:
	ldr	r1, .L386+40
	mov	r0, #1
	bl	dprint_vfmw
	ldrb	r3, [r8, #440]	@ zero_extendqisi2
	bic	r3, r3, #8
	strb	r3, [r8, #440]
	b	.L368
.L384:
	ldrb	r2, [ip, #2967]	@ zero_extendqisi2
	sub	r2, r2, #2
	cmp	r2, #1
	movhi	r2, #0
	movls	r2, #1
	b	.L335
.L385:
	ldrb	r3, [r2, #2864]	@ zero_extendqisi2
	sub	r3, r3, #2
	cmp	r3, #1
	movhi	r3, #0
	movls	r3, #1
	b	.L333
.L387:
	.align	2
.L386:
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
	bne	.L389
	ldr	r1, .L391
	bl	dprint_vfmw
.L389:
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L392:
	.align	2
.L391:
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
	ldr	lr, .L394
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
.L395:
	.align	2
.L394:
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
	ldr	r2, [r3, #3472]
	beq	.L399
	cmp	r1, #2
	beq	.L405
	cmp	r1, #0
	beq	.L404
	add	r2, r2, #1
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
.L405:
	str	r2, [r0, #3624]
	ldr	r3, [r3, #3476]
	add	r3, r3, #1
	add	r3, r3, r3, lsr #31
	mov	r3, r3, asr #1
	str	r3, [r0, #3628]
	b	.L402
.L399:
	add	r2, r2, #1
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
.L404:
	str	r2, [r0, #3624]
	ldr	r3, [r3, #3476]
	str	r3, [r0, #3628]
.L402:
	mul	r1, r3, r2
	mov	r2, r2, asl #4
	mov	r3, r3, asl #4
	str	r2, [r0, #3616]
	str	r3, [r0, #3620]
	str	r1, [r0, #3632]
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
	ldr	ip, .L417
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
	ble	.L415
	cmp	r5, #3
	movle	r1, #3
	movle	r4, #2
	bgt	.L416
.L408:
	mov	r0, r7
	bl	BsSkip
	sub	r3, fp, #36
	add	r4, r3, r4, lsl #2
	cmp	r6, #12
	ldrhi	r0, [r4, #-40]
	ldrls	r0, [r4, #-20]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L416:
	cmp	r5, #8
	movlt	r1, #2
	movge	r1, #1
	movlt	r4, #1
	movge	r4, #0
	b	.L408
.L415:
	cmp	r5, #0
	mov	r1, #4
	moveq	r4, #3
	movne	r4, r1
	b	.L408
.L418:
	.align	2
.L417:
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
	ldr	r2, .L420
	sub	r3, fp, #12
	add	r3, r3, r0, lsl #2
	ldmia	r2, {r0, r1}
	sub	r2, fp, #12
	stmdb	r2, {r0, r1}
	ldr	r0, [r3, #-8]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L421:
	.align	2
.L420:
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
	ldr	r3, .L430
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
	ble	.L429
	cmp	r7, #4
	movlt	r1, #2
	movge	r1, #1
	movlt	r4, #1
	movge	r4, #0
.L424:
	mov	r0, r6
	bl	BsSkip
	sub	r3, fp, #36
	add	ip, r3, r4, lsl #2
	cmp	r5, #12
	ldrhi	r0, [ip, #-36]
	ldrls	r0, [ip, #-20]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L429:
	cmp	r7, r8
	mov	r1, r4
	movne	r4, ip
	b	.L424
.L431:
	.align	2
.L430:
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
	ldr	r3, .L441
	sub	lr, fp, #60
	add	ip, r3, #16
	mov	r4, r0
	cmp	r0, #1
	ldmia	r3, {r0, r1, r2, r3}
	stmia	lr, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	sub	ip, fp, #28
	stmdb	ip, {r0, r1, r2, r3}
	ble	.L440
	cmp	r4, #4
	movlt	r1, #2
	movge	r1, #1
	movlt	r4, #1
	movge	r4, #0
.L434:
	mov	r0, r6
	bl	BsSkip
	sub	r3, fp, #28
	add	r4, r3, r4, lsl #2
	cmp	r5, #12
	ldrhi	r0, [r4, #-32]
	ldrls	r0, [r4, #-16]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L440:
	cmp	r4, #0
	mov	r1, #3
	moveq	r4, r1
	movne	r4, #2
	b	.L434
.L442:
	.align	2
.L441:
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
	ldr	ip, .L449
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
	bgt	.L444
	mov	r1, #3
	bl	BsSkip
	sub	r3, fp, #44
	add	r3, r3, r4, lsl #2
	ldr	r1, [r3, #-200]
	ldr	r2, [r3, #-172]
.L445:
	sub	r3, fp, #44
	mov	r0, r4
	add	r2, r3, r2, lsl #2
	ldr	r3, [r2, #-148]
	ldr	r2, [fp, #-252]
	mul	r3, r3, r1
	str	r3, [r2]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L444:
	mov	r1, #7
	bl	BsGet
	and	r4, r0, #15
	cmp	r4, #15
	beq	.L448
	sub	r0, r0, #112
	sub	r3, fp, #44
	add	r4, r4, #7
	add	r0, r3, r0, lsl #2
	ldr	r1, [r0, #-112]
	ldr	r2, [r0, #-56]
	b	.L445
.L448:
	ldr	r2, [fp, #-256]
	mov	r3, #4
	mov	r0, #22
	strb	r3, [r2]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L450:
	.align	2
.L449:
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
	bgt	.L452
	mov	r1, #3
	bl	BsSkip
	sub	r3, fp, #44
	add	r2, r3, r4, lsl #2
	ldr	r1, [r2, #-200]
	ldr	r2, [r2, #-172]
	sub	r3, r2, #1
	cmp	r3, #7
	bhi	.L456
.L458:
	sub	r2, fp, #44
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-144]
	mul	r3, r3, r1
.L457:
	ldr	r2, [fp, #-256]
	str	r3, [r2]
.L454:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L452:
	mov	r1, #7
	bl	BsGet
	and	r4, r0, #15
	sub	r3, r4, #14
	add	r4, r4, #7
	cmp	r3, #1
	bls	.L454
	sub	r3, r0, #112
	cmp	r3, #13
	bhi	.L455
	sub	r2, fp, #44
	add	r2, r2, r3, lsl #2
	ldr	r1, [r2, #-112]
	ldr	r2, [r2, #-56]
	sub	r3, r2, #1
	cmp	r3, #7
	bls	.L458
.L456:
	str	r2, [sp]
	movw	r3, #3591
	ldr	r2, .L459+4
	mov	r0, #1
	ldr	r1, .L459+8
	bl	dprint_vfmw
	mov	r3, #0
	b	.L457
.L455:
	str	r0, [sp]
	add	r2, r6, #184
	movw	r3, #3581
	ldr	r1, .L459+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, #0
	b	.L456
.L460:
	.align	2
.L459:
	.word	.LANCHOR0+160
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
	mov	r0, r7
	bl	VC1_SetCurPicResolutionInfo
	mov	r0, r5
	bl	BsPos
	ldr	r1, .L556+4
	mov	r2, r0
	mov	r0, #18
	bl	dprint_vfmw
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
	bl	BsPos
	ldr	r1, .L556+8
	mov	r2, r0
	mov	r0, #18
	bl	dprint_vfmw
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
	ldrb	r7, [r2, #258]	@ zero_extendqisi2
	mov	r5, r0
	mov	r6, r1
	mov	r0, r1
	mov	r4, r2
	mov	r1, r7
	bl	VC1_SetCurPicResolutionInfo
	ldr	r3, [r5, #16]
	cmp	r3, #1
	ble	.L626
	ldr	r9, .L631
	mov	r2, #100
	mov	r1, #0
	add	r0, r4, #248
	add	r8, r6, #32768
	ldr	r3, [r9, #48]
	blx	r3
	add	r0, r6, #23808
	ldr	r3, [r9, #48]
	mov	r2, #12288
	mov	r1, #0
	add	r0, r0, #232
	strb	r7, [r4, #258]
	blx	r3
	ldr	r2, [r8, #3560]
	ldr	r1, .L631+4
	mov	r0, #18
	bl	dprint_vfmw
	mov	r0, r5
	bl	BsPos
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r4, #324]
	bne	.L591
	ldrb	r2, [r4, #248]	@ zero_extendqisi2
.L592:
	ldr	r1, .L631+8
	mov	r0, #18
	bl	dprint_vfmw
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
	ldrb	r7, [r6, #3112]	@ zero_extendqisi2
	cmp	r7, #1
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
	ldrb	r7, [r6, #3112]	@ zero_extendqisi2
	cmp	r7, #1
	bne	.L607
	mov	r1, r6
	mov	r2, r4
	mov	r0, r5
	bl	VC1_Vfmw_ParseMPBPicHdr
	cmp	r0, #1
	mov	r6, r0
	beq	.L604
	mov	r0, r7
	ldr	r1, .L631+12
	bl	dprint_vfmw
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
	ldr	r4, [r8, #3584]
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
	ldr	r1, .L631+16
	mov	r0, #1
	bl	dprint_vfmw
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
	mov	r0, r7
	ldr	r1, .L631+20
	bl	dprint_vfmw
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
	ldr	r3, [r3, #3556]
	cmp	r3, #0
	bne	.L964
	ldr	r3, [r2, #504]
	strb	r3, [r2, #349]
	ldrb	r3, [r1, #3513]	@ zero_extendqisi2
	strb	r3, [r1, #2]
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
.L964:
	mov	r1, #3
	mov	r0, r6
	bl	BsGet
	ldr	r3, .L1010
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
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	bne	.L1004
.L969:
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
	ble	.L982
.L983:
	add	r1, r4, #496
	mov	r0, r6
	bl	VC1_DecBfraction
	strb	r0, [r4, #350]
	b	.L982
.L1004:
	cmp	r3, #0
	beq	.L1006
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #352]
	mov	r0, r6
	bl	BsGet
	ldrb	r2, [r4, #23]	@ zero_extendqisi2
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r2, #1
	strb	r0, [r4, #353]
	beq	.L1007
.L973:
	cmp	r3, #1
	bne	.L975
	ldrb	r3, [r4, #354]	@ zero_extendqisi2
	add	r3, r3, #1
	str	r3, [r4, #492]
	b	.L974
.L988:
	mov	r0, r6
	mov	r2, r4
	mov	r1, r5
	bl	VC1_Vfmw_ParseAdvBFieldHdr
.L984:
	ldrb	r3, [r4, #352]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	bne	.L1008
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
.L1008:
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
.L1003:
	mov	r1, #8
	mov	r0, r6
	bl	BsGet
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	str	r0, [r4, #396]
	beq	.L969
	b	.L1004
.L1006:
	mov	r1, #2
	mov	r0, r6
	bl	BsGet
	ldrb	r2, [r4, #23]	@ zero_extendqisi2
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r2, #1
	strb	r0, [r4, #354]
	bne	.L973
.L1007:
	cmp	r3, #1
	beq	.L1009
.L972:
	mov	r3, #2
	str	r3, [r4, #492]
	b	.L974
.L991:
	mov	r0, #0
	b	.L984
.L1005:
	ldr	r3, [r4, #472]
	cmp	r3, #3
	bgt	.L983
	mov	r0, r6
	bl	VC1_DecRefdist
	strb	r0, [r4, #379]
	b	.L978
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
.L1009:
	ldrb	r3, [r4, #353]	@ zero_extendqisi2
	add	r3, r3, #2
	str	r3, [r4, #492]
	b	.L974
.L1011:
	.align	2
.L1010:
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
	beq	.L1013
	add	r3, r4, #20480
	ldr	r3, [r3, #3556]
	cmp	r3, #0
	beq	.L1022
.L1013:
	ldrb	r8, [r5, #23]	@ zero_extendqisi2
	cmp	r8, #1
	movne	r3, #0
	movne	r0, r3
	beq	.L1023
.L1015:
	str	r0, [r5, #392]
.L1014:
	cmp	r3, #0
	mov	r2, r5
	mov	r1, r4
	mov	r0, r6
	beq	.L1024
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvFieldPicHdr
.L1022:
	ldr	r3, [r5, #392]
	bic	r3, r3, #2
	b	.L1014
.L1024:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VC1_Vfmw_ParseAdvFramePicHdr
.L1023:
	mov	r1, #2
	mov	r0, r6
	bl	BsShow
	cmp	r0, #1
	mov	r7, r0
	ble	.L1025
	mov	r1, #2
	mov	r0, r6
	bl	BsSkip
	bic	r3, r7, #2
	mov	r0, r7
	b	.L1015
.L1025:
	mov	r1, r8
	mov	r0, r6
	bl	BsSkip
	mov	r3, #0
	mov	r0, r3
	b	.L1015
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
	ldr	r4, [r3, #3556]
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
	beq	.L1028
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1028:
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
	bgt	.L1032
	b	.L1033
.L1038:
	bl	BsSkip
	cmp	r4, r6
	beq	.L1033
.L1032:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r5
	bne	.L1038
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1033:
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
	ldr	r3, [r0, #3128]
	mov	r5, r0
	ldr	r6, [r9, #3556]
	movw	ip, #24048
	str	r1, [fp, #-48]
	movw	r0, #24040
	movw	lr, #24056
	mov	r8, r6, asl #4
	mov	r7, r6, asl #6
	rsb	r4, r8, r7
	add	r4, r5, r4
	add	r1, r4, #23808
	strb	r6, [r4, ip]
	add	ip, r1, #232
	str	ip, [fp, #-52]
	mov	r10, r1
	str	ip, [fp, #-56]
	ldr	ip, [r3, #24]
	add	r2, r2, ip, lsr #3
	str	r2, [r4, r0]
	ldr	r2, [r3, #24]
	and	r2, r2, #7
	str	r2, [r4, lr]
	ldr	r2, [r3, #16]
	ldr	r3, [r3, #24]
	rsb	r3, r3, r2, lsl #3
	str	r3, [r1, #252]
	ldr	r0, [r4, r0]
	bl	MEM_Vir2Phy
	add	r3, r5, #32768
	movw	r1, #24080
	add	r2, r4, #24064
	add	r2, r2, #16
	str	r0, [r10, #244]
	ldr	r0, [r3, #3592]
	str	r0, [r4, r1]
	ldr	r1, [r3, #3568]
	cmp	r1, #0
	beq	.L1041
	ldr	r0, [r3, #3580]
	ldr	ip, [fp, #-52]
	cmp	r0, #0
	bne	.L1060
.L1041:
	rsb	r3, r8, r7
	movw	ip, #24072
	add	r3, r5, r3
	mov	r2, #0
	add	r0, r3, #24064
	add	lr, r3, #23808
	str	r2, [lr, #236]
	str	r2, [r3, ip]
	str	r2, [r0, #12]
	str	r2, [r0, #4]
	str	r2, [r0, #20]
.L1042:
	ldr	r0, [r9, #3476]
	sub	r6, r8, r6, asl #2
	ldr	ip, [r9, #3472]
	ldrb	r4, [r5, #8]	@ zero_extendqisi2
	mul	ip, ip, r0
	cmp	r4, #0
	sub	lr, ip, #1
	beq	.L1043
	ldr	r2, [fp, #-48]
	add	r3, r0, #1
	add	r6, r2, r6
	add	r3, r3, r3, lsr #31
	rsb	r2, r8, r7
	add	r6, r6, #520
	ldr	r1, [r6, #8]
	add	r2, r5, r2
	add	r2, r2, #24064
	sub	r3, r1, r3, asr #1
	cmp	lr, r3
	str	r3, [r2]
	movcs	ip, #0
	movcc	ip, #1
	cmp	r3, #0
	movle	ip, #0
	cmp	ip, #0
	bne	.L1061
.L1044:
	rsb	r7, r8, r7
	add	r5, r5, r7
	add	r3, r5, #23808
	add	r3, r3, #248
	ldr	r3, [r3, #4]
	cmp	r3, #0
	ble	.L1046
	add	r3, r5, #23808
	add	r3, r3, #232
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1047
	add	r5, r5, #24064
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L1046
.L1047:
	ldr	r3, [r9, #3556]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r9, #3556]
.L1045:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1043:
	ldr	r3, [fp, #-48]
	rsb	r2, r8, r7
	add	r2, r5, r2
	add	r6, r3, r6
	add	r2, r2, #24064
	add	r6, r6, #520
	ldr	r3, [r6, #8]
	cmp	lr, r3
	str	r3, [r2]
	bcs	.L1044
	ldr	r2, [r6, #8]
	mov	r0, #1
	str	lr, [sp]
	ldr	r1, .L1062
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1060:
	movw	lr, #24072
	str	r1, [ip, #4]
	mov	ip, #0
	str	ip, [r4, lr]
	ldr	ip, [r3, #3588]
	add	r1, r4, #24064
	str	r0, [r1, #4]
	mov	ip, ip, asl #3
	str	ip, [r1, #12]
	ldr	r3, [r3, #3596]
	str	r3, [r2, #4]
	b	.L1042
.L1061:
	ldr	r2, [r6, #8]
	ldr	r1, .L1062+4
	str	r0, [sp]
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	b	.L1045
.L1046:
	ldr	r1, .L1062+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1063:
	.align	2
.L1062:
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
	beq	.L1065
	adds	r3, r2, #15
	add	r0, r0, #20480
	addmi	r3, r2, #30
	movw	r1, #2404
	mov	r3, r3, asr #4
	str	r3, [r0, #3472]
	ldrh	r3, [r4, #238]
	str	r3, [r0, #3488]
	ldrh	r2, [r4, #240]
	ldrh	r3, [r5, r1]
	cmp	r3, r2
	beq	.L1066
.L1067:
	adds	r3, r2, #15
	addmi	r3, r2, #30
	mov	r3, r3, asr #4
	str	r3, [r0, #3476]
	ldrh	r3, [r4, #240]
	str	r3, [r0, #3492]
.L1066:
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
.L1070:
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1065:
	movw	r3, #2404
	ldrh	r2, [r1, #240]
	ldrh	r3, [r5, r3]
	cmp	r3, r2
	addne	r0, r0, #20480
	bne	.L1067
	mov	r4, #0
	b	.L1070
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
	bne	.L1073
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
.L1073:
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
	ldr	r1, .L1077
	sub	r0, fp, #148
	bl	memcpy
	cmp	r5, #0
	subeq	r3, fp, #20
	movne	r0, r4
	addeq	r4, r3, r4, lsl #2
	ldreq	r0, [r4, #-128]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1078:
	.align	2
.L1077:
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
	beq	.L1080
	mov	r0, r5
	ldr	r1, [r4, #32]
	bl	SM_ReleaseStreamSeg
.L1080:
	ldr	r3, .L1084
	mov	r0, r4
	mov	r2, #36
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L1085:
	.align	2
.L1084:
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
	beq	.L1087
	add	r7, r0, #20480
	ldr	r3, [r7, #3556]
	cmp	r3, #0
	beq	.L1101
	add	r4, r0, #24064
	mov	r5, #0
	add	r4, r4, #16
	b	.L1091
.L1090:
	ldr	r3, [r7, #3556]
	add	r4, r4, #48
	cmp	r5, r3
	bcs	.L1102
.L1091:
	ldr	r1, [r4]
	mov	r0, r6
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #-36]
	add	r5, r5, #1
	cmp	r3, #0
	beq	.L1090
	ldr	r3, [r4, #-12]
	cmp	r3, #0
	beq	.L1090
	ldr	r1, [r4, #4]
	mov	r0, r6
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r7, #3556]
	add	r4, r4, #48
	cmp	r5, r3
	bcc	.L1091
.L1102:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1087:
	add	r1, r0, #36096
	mov	r0, r6
	add	r1, r1, #236
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VC1_ClearCurPacket
.L1101:
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
	add	r5, r0, #49152
	mov	r7, r0
	add	r0, r0, #12672
	add	r4, r7, #32768
	add	r0, r0, #56
	mov	r6, r1
	ldr	r1, [r5, #416]
	bl	VC1_ClearCurPic
	ldr	r1, [r4, #3988]
	ldr	r0, [r5, #416]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #3984]
	mov	r8, r0
	ldr	r0, [r5, #416]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #3980]
	mov	r9, r0
	ldr	r0, [r5, #416]
	bl	FSP_GetFsImagePtr
	cmp	r8, #0
	beq	.L1136
	cmp	r6, #0
	movlt	r3, #0
	blt	.L1106
	cmp	r6, #100
	movlt	r3, r6
	movge	r3, #100
.L1106:
	add	r6, r7, #12288
	str	r3, [r8, #160]
	mov	r0, r7
	bl	VC1_SetImgFormat
	ldrb	r3, [r6, #443]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L1137
.L1107:
	add	r3, r8, #576
	add	r2, r8, #560
	mov	r0, #0
	mov	r1, #0
	strd	r0, [r2]
	strd	r0, [r3, #-8]
	ldr	r3, [r7]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L1108
	ldrb	r10, [r6, #442]	@ zero_extendqisi2
	sub	r10, r10, #2
	ands	r10, r10, #253
	beq	.L1109
	ldr	r10, [r4, #3976]
	cmp	r10, #1
	ldrhi	r1, [r4, #3980]
	mvnls	r1, #0
	cmp	r10, #1
	movls	r10, #0
	movhi	r10, #1
	str	r1, [r4, #3992]
.L1111:
	add	r2, r7, #45056
	cmn	r1, #1
	ldr	r3, [r2, #4000]
	add	r3, r3, #1
	str	r3, [r2, #4000]
	beq	.L1112
	ldr	r0, [r5, #416]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L1138
	ldr	r2, [r9, #160]
	cmp	r2, #0
	beq	.L1114
	ldr	r3, [r7]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bhi	.L1115
.L1114:
	ldr	r8, [r4, #3992]
	ldr	r0, [r5, #416]
	mov	r1, r8
	bl	FSP_GetRef
	mov	r2, r8
	ldr	r1, .L1141
	mov	r3, r0
	mov	r0, #12
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r4, #3992]
	ldr	r0, [r5, #416]
	bl	FSP_SetDisplay
	cmp	r10, #1
	ldreq	r3, [r4, #3904]
	movne	r1, #1
	ldrne	r2, [r4, #3896]
	movne	r3, #2
	strneb	r1, [r2, #1]
	mov	r2, r7
	streqb	r10, [r3, #1]
	mov	r1, #1
	strneb	r3, [r6, #441]
	add	r3, r7, #49152
	streqb	r10, [r6, #441]
	add	r3, r3, #64
	ldr	r0, [r5, #416]
	str	r9, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L1118
	ldr	r1, [r9, #560]
	add	r3, r9, #560
	ldr	r0, [r5, #416]
	add	r8, r9, #576
	str	r3, [fp, #-48]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-48]
	mov	r6, #0
	mov	r7, #0
	strd	r6, [r3]
	ldr	r1, [r9, #568]
	ldr	r0, [r5, #416]
	bl	FreeUsdByDec
	mov	r2, #0
	strd	r6, [r8, #-8]
	ldr	r0, [r5, #416]
	ldr	r1, [r4, #3992]
	bl	FSP_SetDisplay
	cmp	r10, #1
	mov	r3, #0
	ldreq	r2, [r4, #3904]
	mov	r0, r3
	ldrne	r2, [r4, #3896]
	strb	r3, [r2, #1]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1109:
	ldr	r1, [r4, #3988]
	cmp	r9, #0
	moveq	r10, r9
	str	r1, [r4, #3992]
	beq	.L1111
	add	ip, r9, #608
	add	r0, r8, #608
	ldrd	r2, [ip, #-8]
	ldrd	r8, [r0, #-8]
	cmp	r2, r8
	sbcs	lr, r3, r9
	strltd	r2, [r0, #-8]
	strltd	r8, [ip, #-8]
	ldrlt	r1, [r4, #3992]
	b	.L1111
.L1139:
	ldr	r2, [r4, #3900]
	mov	r3, #0
	strb	r3, [r2, #1]
	strb	r3, [r6, #441]
.L1112:
	mov	r0, #1
.L1105:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1137:
	ldrb	r3, [r6, #449]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1112
	b	.L1107
.L1108:
	ldr	r1, [r4, #3988]
	mov	r10, #0
	str	r1, [r4, #3992]
	b	.L1111
.L1118:
	ldrb	r3, [r6, #441]	@ zero_extendqisi2
	sxtb	r3, r3
	cmp	r3, #3
	beq	.L1139
	cmp	r3, #1
	beq	.L1140
	cmp	r3, #2
	bne	.L1112
	ldr	r2, [r4, #3896]
	mov	r3, #0
	strb	r3, [r2, #1]
	strb	r3, [r6, #441]
	b	.L1105
.L1115:
	ldr	r1, .L1141+4
	mov	r0, #1
	add	ip, r9, #560
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	r1, [r9, #560]
	ldr	r0, [r5, #416]
	add	r8, r9, #576
	bl	FreeUsdByDec
	ldr	ip, [fp, #-48]
	mov	r6, #0
	mov	r7, #0
	strd	r6, [ip]
	ldr	r1, [r9, #568]
	ldr	r0, [r5, #416]
	bl	FreeUsdByDec
	strd	r6, [r8, #-8]
	mov	r2, #0
	ldr	r0, [r5, #416]
	ldr	r1, [r4, #3992]
	bl	FSP_SetDisplay
	cmp	r10, #1
	mov	r2, #0
	ldreq	r3, [r4, #3904]
	moveq	r0, r10
	ldrne	r3, [r4, #3896]
	movne	r0, #1
	strb	r2, [r3, #1]
	b	.L1105
.L1140:
	ldr	r2, [r4, #3904]
	mov	r0, r3
	mov	r3, #0
	strb	r3, [r2, #1]
	strb	r3, [r6, #441]
	b	.L1105
.L1136:
	str	r0, [sp]
	mov	r3, r9
	mov	r2, r8
	ldr	r1, .L1141+8
	mov	r0, r8
	bl	dprint_vfmw
	mov	r0, r8
	b	.L1105
.L1138:
	movw	r2, #733
	ldr	r1, .L1141+12
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r9
	b	.L1105
.L1142:
	.align	2
.L1141:
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
	add	r5, r0, #12288
	mov	r4, r0
	ldrb	r3, [r5, #440]	@ zero_extendqisi2
	tst	r3, #16
	beq	.L1196
	add	r6, r0, #49152
	bic	r3, r3, #20
	strb	r3, [r5, #440]
	ldr	r3, [r6, #20]
	sub	r3, r3, #2
	cmp	r3, #1
	bls	.L1146
.L1148:
	ldr	r3, [r4]
	add	r7, r4, #32768
	ldr	r1, [r7, #3976]
	ldr	r2, [r3, #8]
.L1147:
	sub	r3, r2, #1
	str	r2, [r6, #20]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L1177
.L1158:
	.word	.L1152
	.word	.L1154
	.word	.L1176
	.word	.L1156
.L1160:
	cmp	r2, #2
	bne	.L1161
	add	r3, r4, #45056
	ldr	r3, [r3, #4048]
	adds	r3, r3, #0
	movne	r3, #1
	cmp	r1, #0
	bne	.L1163
.L1162:
	ldr	r1, .L1198
	mov	r0, #1
	bl	dprint_vfmw
.L1170:
	add	r0, r4, #12672
	ldr	r1, [r6, #416]
	add	r0, r0, #56
	bl	VC1_ClearCurPic
	ldr	r3, .L1198+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1196
	mov	r3, #0
	ldr	r0, [r6, #416]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L1196:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1146:
	ldr	r3, [r0]
	ldr	r2, [r3, #8]
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L1148
	add	r7, r0, #32768
	ldr	r1, .L1198+8
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r1, [r7, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L1149
	ldr	r1, [r8, #560]
	add	r9, r8, #576
	ldr	r0, [r6, #416]
	add	r10, r8, #592
	bl	FreeUsdByDec
	ldr	r1, [r8, #568]
	ldr	r0, [r6, #416]
	add	r3, r8, #560
	str	r3, [fp, #-48]
	bl	FreeUsdByDec
	ldr	r1, [r8, #576]
	ldr	r0, [r6, #416]
	bl	FreeUsdByDec
	ldr	r1, [r8, #584]
	ldr	r0, [r6, #416]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-48]
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r3]
	strd	r0, [r9, #-8]
	strd	r0, [r9]
	strd	r0, [r10, #-8]
	ldr	r1, [r7, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r7, #3980]
	ldr	r0, [r6, #416]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	str	r3, [r7, #3980]
	str	r3, [r7, #3984]
	ldr	r0, [r6, #416]
	bl	FSP_GetNullLogicFs
	str	r0, [r7, #3984]
	str	r0, [r7, #3980]
.L1149:
	ldr	r2, [r7, #3900]
	mov	r3, #0
	str	r3, [r7, #3976]
	cmp	r2, r3
	beq	.L1157
	ldr	r1, [r7, #3904]
	cmp	r1, #0
	beq	.L1157
	strb	r3, [r2, #1]
	ldr	r2, [r7, #3904]
	strb	r3, [r2, #1]
	ldr	r2, [r7, #3900]
	strb	r3, [r2]
	ldr	r2, [r7, #3904]
	strb	r3, [r2]
	ldr	r3, [r4]
	ldr	r1, [r7, #3976]
	ldr	r2, [r3, #8]
	b	.L1147
.L1176:
	mov	r3, #0
.L1159:
	cmp	r1, #0
	beq	.L1155
.L1163:
	cmp	r1, #1
	bls	.L1197
.L1166:
	cmp	r3, #0
	beq	.L1170
	ldrb	r3, [r5, #3536]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1169
	add	r3, r5, #3600
	add	ip, r4, #23040
	add	r3, r3, #4
	add	ip, ip, #44
	ldmia	r3, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	add	r1, r5, #3840
	add	r0, r4, #23296
	add	r1, r1, #12
	mov	r2, #100
	add	r0, r0, #36
	bl	memcpy
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1175:
	mov	r1, #0
.L1156:
	ldrb	r2, [r5, #442]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1160
	add	r0, r4, #45056
	mov	r3, #1
	str	r2, [r0, #4048]
	b	.L1159
.L1172:
	mov	r1, #0
.L1152:
	ldrb	r3, [r5, #442]	@ zero_extendqisi2
	sub	r3, r3, #2
	tst	r3, #253
	movne	r3, #1
	moveq	r3, #0
	cmp	r1, #0
	bne	.L1163
.L1155:
	ldrb	r2, [r5, #442]	@ zero_extendqisi2
	sub	r0, r2, #4
	sub	r1, r2, #1
	cmp	r1, #1
	cmphi	r0, #1
	bls	.L1162
.L1164:
	sub	r2, r2, #2
	tst	r2, #253
	bne	.L1166
	ldr	r1, .L1198+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1170
.L1173:
	mov	r1, #0
.L1154:
	ldrb	r3, [r5, #442]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	b	.L1159
.L1169:
	add	r1, r5, #3616
	add	r0, r4, #23040
	add	r1, r1, #4
	mov	r2, #172
	add	r0, r0, #60
	bl	memcpy
	add	lr, r5, #3792
	add	ip, r4, #23040
	ldmia	lr!, {r0, r1, r2, r3}
	add	ip, ip, #232
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	add	r0, r4, #23296
	add	r1, r5, #3952
	mov	r2, #176
	add	r0, r0, #136
	bl	memcpy
	ldr	r3, [r5, #3996]
	ldr	r2, .L1198+16
	add	r1, r4, #16384
	add	r0, r4, #23552
	add	r1, r1, #32
	strb	r3, [r5, #444]
	add	r0, r0, #56
	ldr	r3, [r7, #3996]
	ldr	ip, [r2, #52]
	mov	r2, r3, asl #4
	sub	r2, r2, r3, asl #2
	blx	ip
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1171:
	mov	r1, #0
.L1177:
	mov	r3, #1
	b	.L1159
.L1161:
	and	r2, r2, #251
	cmp	r2, #1
	bne	.L1177
	add	r0, r4, #45056
	mov	r3, r2
	str	r2, [r0, #4048]
	b	.L1159
.L1157:
	ldr	r3, [r4]
	ldr	r2, [r3, #8]
	sub	r3, r2, #1
	str	r2, [r6, #20]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L1171
.L1153:
	.word	.L1172
	.word	.L1173
	.word	.L1174
	.word	.L1175
.L1174:
	mov	r3, #0
	b	.L1155
.L1197:
	ldrb	r2, [r5, #442]	@ zero_extendqisi2
	b	.L1164
.L1199:
	.align	2
.L1198:
	.word	.LC55
	.word	g_event_report
	.word	.LC54
	.word	.LC56
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r3, [r0]
	add	r5, r0, #12288
	add	r6, r0, #12672
	mov	r4, r0
	ldrb	r1, [r5, #3536]	@ zero_extendqisi2
	add	r6, r6, #56
	ldr	r2, [r3, #592]
	add	r8, r0, #8
	cmp	r1, #0
	str	r2, [r5, #264]
	ldr	r3, [r3, #632]
	str	r3, [r5, #268]
	beq	.L1217
.L1201:
	add	r7, r4, #32768
	mov	r1, r8
	mov	r0, r4
	bl	VC1_WritePicMsg
	ldr	r3, [r7, #3996]
	ldr	ip, .L1222
	add	r1, r6, #23808
	add	r1, r1, #232
	add	r0, r4, #92
	mov	r2, r3, asl #6
	strb	r3, [r4, #78]
	sub	r2, r2, r3, asl #4
	ldr	r3, [ip, #52]
	blx	r3
	mov	r1, r8
	mov	r0, r6
	bl	VC1_CfgVDH
	ldrb	r3, [r5, #442]	@ zero_extendqisi2
	cmp	r3, #1
	ldrlsb	r2, [r5, #444]	@ zero_extendqisi2
	strlsb	r2, [r5, #445]
	cmp	r3, #5
	beq	.L1218
	mov	r3, #0
	strb	r3, [r4, #9]
	ldrb	r3, [r5, #444]	@ zero_extendqisi2
	ldrb	r2, [r5, #448]	@ zero_extendqisi2
.L1204:
	ldrh	r1, [r4, #86]
	strb	r3, [r4, #8]
	cmp	r1, #120
	movle	r1, #1
	movgt	r1, #0
	strb	r1, [r4, #79]
	strb	r3, [r5, #443]
	strb	r2, [r5, #449]
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1219
	and	r2, r3, #251
	cmp	r2, #1
	addeq	r1, r4, #45056
	streq	r2, [r1, #4048]
.L1208:
	ldr	r2, [r4]
	str	r8, [r4, #4]
	ldr	r0, [r2, #864]
	cmp	r0, #1
	beq	.L1220
	cmp	r0, #2
	beq	.L1221
.L1213:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1221:
	ldr	r2, [r2, #868]
	cmp	r8, #0
	cmpne	r2, #0
	ble	.L1213
	sub	r3, r3, #2
	ands	r5, r3, #253
	bne	.L1213
	ldr	r3, [r7, #3896]
	add	r1, r4, #49152
	mov	r0, r6
	strb	r5, [r3, #1]
	ldr	r2, [r4]
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	ldr	r1, [r1, #416]
	bl	VC1_ClearCurPic
	str	r5, [r4, #4]
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1219:
	add	r9, r4, #45056
	ldr	r5, [r9, #4048]
	cmp	r5, #0
	bne	.L1208
	add	r3, r4, #49152
	mov	r0, r6
	ldr	r1, [r3, #416]
	bl	VC1_ClearCurPic
	ldr	r3, [r7, #3896]
	mov	r0, r5
	strb	r5, [r3]
	ldr	r3, [r9, #4052]
	add	r3, r3, #1
	str	r3, [r9, #4052]
	str	r5, [r4, #4]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1218:
	mov	r2, #1
	strb	r2, [r4, #9]
	ldrb	r3, [r5, #445]	@ zero_extendqisi2
	add	r1, r4, #45056
	strb	r2, [r5, #448]
	strb	r3, [r5, #444]
	ldr	r1, [r1, #4072]
	sub	r1, r1, #1
	str	r1, [r5, #172]
	b	.L1204
.L1220:
	cmp	r8, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	sub	r3, r3, #2
	ands	r5, r3, #253
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r2, [r7, #3896]
	add	r3, r4, #49152
	mov	r0, r6
	strb	r5, [r2, #1]
	ldr	r1, [r3, #416]
	bl	VC1_ClearCurPic
	str	r5, [r4, #4]
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1217:
	mov	r0, r6
	bl	VC1_Vfmw_UpdataLastPicRNDCTRL.part.5
	b	.L1201
.L1223:
	.align	2
.L1222:
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
	ldr	r5, [r0, #3128]
	add	r8, r0, #32768
	mov	r4, r0
	str	r1, [fp, #-52]
	mov	r10, #0
	mov	r1, #32
	strb	r10, [r0, #6]
	mov	r0, r5
	str	r2, [fp, #-48]
	bl	BsGet
	ldr	r3, [r5, #24]
	ldr	r6, [r8, #3564]
	add	r7, r4, #36096
	ldr	r2, [r8, #3584]
	add	r7, r7, #236
	mov	r3, r3, lsr #3
	add	r6, r6, r3
	rsb	r2, r3, r2
	mov	r1, r6
	uxtb	r9, r0
	mov	r0, r5
	bl	BsInit
	sub	r2, r9, #10
	cmp	r2, #5
	bhi	.L1225
	cmp	r9, #11
	ldrb	r3, [r4]	@ zero_extendqisi2
	beq	.L1294
	and	r1, r3, #7
	cmp	r1, #7
	beq	.L1295
	add	r9, r4, #20480
	strb	r10, [r4, #8]
	str	r10, [r9, #3556]
	cmp	r2, #5
	ldrls	pc, [pc, r2, asl #2]
	b	.L1257
.L1241:
	.word	.L1240
	.word	.L1257
	.word	.L1242
	.word	.L1259
	.word	.L1244
	.word	.L1245
.L1225:
	sub	r2, r9, #27
	cmp	r2, #4
	bls	.L1258
	cmp	r9, #254
	beq	.L1296
.L1257:
	ldr	r0, [fp, #-48]
	mov	r1, r7
	bl	VC1_ClearCurPacket
	mov	r0, #0
.L1230:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1296:
	mov	r1, #32
	mov	r0, r5
	bl	BsGet
	mov	r1, #32
	mov	r6, r0
	mov	r0, r5
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r6, r3
	bne	.L1257
	ldrb	r3, [r4]	@ zero_extendqisi2
	mov	r2, #1
	strb	r2, [r4, #7]
	and	r2, r3, #7
	cmp	r2, #7
	orreq	r3, r3, #16
	streqb	r3, [r4]
.L1258:
	ldr	r0, [fp, #-48]
	mov	r1, r7
	bl	VC1_ClearCurPacket
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1294:
	and	r3, r3, #7
	cmp	r3, #7
	beq	.L1297
.L1255:
	ldr	r0, [fp, #-48]
	mov	r1, r7
	bl	VC1_ClearCurPacket
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1259:
	ldr	r10, .L1300
.L1287:
	ldr	r0, [fp, #-52]
	mov	r2, #176
	ldr	ip, [r10, #48]
	mov	r1, #0
	add	r0, r0, #348
	ldrb	r3, [r0, #31]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	blx	ip
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-56]
	strb	r3, [r2, #379]
.L1246:
	add	r0, r4, #23808
	ldr	r3, [r10, #48]
	mov	r2, #12288
	mov	r1, #0
	add	r0, r0, #232
	blx	r3
	mov	r0, r5
	bl	BsPos
	ldr	r3, [fp, #-52]
	mov	r1, r4
	mov	r2, r3
	str	r0, [r3, #488]
	mov	r0, r5
	bl	VC1_Vfmw_ParseAdvPicHdr
	cmp	r0, #1
	mov	r10, r0
	beq	.L1298
	mov	r1, r7
	ldr	r0, [fp, #-48]
	bl	VC1_ClearCurPacket
	movw	r2, #6128
	ldr	r1, .L1300+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r10
	b	.L1230
.L1295:
	mov	r10, #1
	orr	r3, r3, #16
	strb	r10, [r4, #6]
	mov	r0, r10
	strb	r3, [r4]
	b	.L1230
.L1242:
	ldr	r10, .L1300
	mov	r3, #1
	strb	r3, [r4, #8]
	mov	r2, #16
	ldr	r3, [fp, #-52]
	mov	r1, #0
	add	r0, r3, #508
	ldr	r3, [r10, #48]
	blx	r3
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1246
	b	.L1287
.L1245:
	ldr	r2, [fp, #-52]
	mov	r0, r5
	mov	r1, r4
	bl	VC1_Vfmw_ParseAdvSeqHdr
	cmp	r0, #1
	mov	r10, r0
	ldreqb	r3, [r4]	@ zero_extendqisi2
	orreq	r3, r3, #1
	streqb	r3, [r4]
	b	.L1255
.L1244:
	mov	r0, r5
	ldr	r2, [fp, #-52]
	mov	r1, r4
	bl	VC1_Vfmw_ParseAdvEntptHdr
	cmp	r0, #1
	mov	r10, r0
	bne	.L1255
	ldr	r1, [fp, #-52]
	mov	r0, r4
	bl	VC1_Vfmw_UpdateAdvSPS
	ldrb	r3, [r4]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r4]
	b	.L1255
.L1240:
	orr	r3, r3, #32
	ldr	r0, [fp, #-48]
	mov	r1, r7
	strb	r3, [r4]
	bl	VC1_ClearCurPacket
	mov	r0, #1
	b	.L1230
.L1297:
	mov	r0, r5
	ldr	r2, [fp, #-52]
	mov	r1, r4
	bl	VC1_Vfmw_ParseAdvSlcHdr
	cmp	r0, #1
	mov	r5, r0
	bne	.L1228
	add	r9, r4, #20480
	ldr	r7, [r9, #3556]
	cmp	r7, #255
	bhi	.L1299
	mov	r3, r7, asl #4
	mov	r2, r7, asl #6
	rsb	r5, r3, r2
	str	r3, [fp, #-48]
	ldr	r3, [r4, #3128]
	add	r5, r4, r5
	str	r2, [fp, #-56]
	movw	r2, #24048
	strb	r7, [r5, r2]
	movw	ip, #24040
	ldr	r1, [r3, #24]
	movw	r0, #24056
	add	r2, r5, #23808
	add	r1, r6, r1, lsr #3
	str	r1, [r5, ip]
	ldr	r1, [r3, #24]
	mov	r6, r2
	and	r1, r1, #7
	str	r1, [r5, r0]
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #24]
	add	r3, r2, #232
	str	r3, [fp, #-60]
	rsb	r1, r0, r1, lsl #3
	str	r1, [r2, #252]
	ldr	r0, [r5, ip]
	str	r3, [fp, #-64]
	bl	MEM_Vir2Phy
	movw	r1, #24080
	add	r2, r5, #24064
	add	r2, r2, #16
	str	r0, [r6, #244]
	ldr	r0, [r8, #3592]
	str	r0, [r5, r1]
	ldr	r1, [r8, #3568]
	cmp	r1, #0
	beq	.L1231
	ldr	r0, [r8, #3580]
	ldr	r3, [fp, #-60]
	cmp	r0, #0
	beq	.L1231
	movw	ip, #24072
	str	r1, [r3, #4]
	str	r10, [r5, ip]
	add	r3, r5, #24064
	ldr	r1, [r8, #3588]
	str	r0, [r3, #4]
	mov	r1, r1, asl #3
	str	r1, [r3, #12]
	ldr	r3, [r8, #3596]
	str	r3, [r2, #4]
.L1232:
	ldr	r0, [r9, #3476]
	ldr	r1, [r9, #3472]
	ldrb	r10, [r4, #8]	@ zero_extendqisi2
	mul	r1, r1, r0
	cmp	r10, #0
	sub	r1, r1, #1
	beq	.L1233
	ldr	r2, [fp, #-48]
	add	r3, r0, #1
	ldr	lr, [fp, #-52]
	sub	ip, r2, r7, asl #2
	add	r3, r3, r3, lsr #31
	add	ip, lr, ip
	ldr	lr, [fp, #-56]
	add	ip, ip, #520
	rsb	r2, r2, lr
	ldr	lr, [ip, #8]
	add	r2, r4, r2
	sub	r3, lr, r3, asr #1
	add	r2, r2, #24064
	cmp	r1, r3
	str	r3, [r2]
	movcs	r1, #0
	movcc	r1, #1
	cmp	r3, #0
	movle	r1, #0
	cmp	r1, #0
	bne	.L1288
.L1234:
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-48]
	rsb	r3, r2, r3
	add	r4, r4, r3
.L1293:
	add	r3, r4, #23808
	add	r3, r3, #248
	ldr	r3, [r3, #4]
	cmp	r3, #0
	ble	.L1237
	add	r3, r4, #23808
	add	r3, r3, #232
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1253
	add	r4, r4, #24064
	ldr	r3, [r4, #12]
	cmp	r3, #0
	ble	.L1237
.L1253:
	ldr	r3, [r9, #3556]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r9, #3556]
	b	.L1230
.L1298:
	mov	r0, r5
	bl	BsPos
	ldr	r2, [fp, #-52]
	ldr	r3, [r2, #488]
	rsb	r0, r3, r0
	str	r0, [r2, #488]
	ldrb	r3, [r4]	@ zero_extendqisi2
	mov	r0, r5
	orr	r3, r3, #4
	strb	r3, [r4]
	bl	BsPos
	ldr	r3, [r8, #3584]
	add	r0, r0, #32
	cmp	r0, r3, asl #3
	bge	.L1248
	ldr	r7, [r9, #3556]
	movw	r3, #24048
	ldr	r2, [r4, #3128]
	movw	ip, #24056
	mov	r1, r7, asl #6
	mov	r10, r7, asl #4
	rsb	r5, r10, r1
	str	r1, [fp, #-48]
	add	r5, r4, r5
	add	r0, r5, #23808
	strb	r7, [r5, r3]
	add	r3, r0, #240
	ldr	r1, [r2, #24]
	str	r3, [fp, #-56]
	str	r3, [fp, #-60]
	movw	r3, #24040
	add	r1, r6, r1, lsr #3
	str	r1, [r5, r3]
	ldr	r1, [r2, #24]
	add	r6, r0, #232
	and	r1, r1, #7
	str	r1, [r5, ip]
	ldr	r1, [r2, #16]
	ldr	r2, [r2, #24]
	rsb	r2, r2, r1, lsl #3
	str	r2, [r0, #252]
	ldr	r0, [r5, r3]
	bl	MEM_Vir2Phy
	ldr	r3, [fp, #-56]
	movw	r1, #24080
	add	r2, r5, #24064
	add	r2, r2, #16
	str	r0, [r3, #4]
	ldr	r3, [r8, #3592]
	str	r3, [r5, r1]
	ldr	r3, [r8, #3568]
	cmp	r3, #0
	beq	.L1249
	ldr	r1, [r8, #3580]
	cmp	r1, #0
	beq	.L1249
	movw	r0, #24072
	str	r3, [r6, #4]
	mov	ip, #0
	str	ip, [r5, r0]
	ldr	r0, [r8, #3588]
	add	r3, r5, #24064
	str	r1, [r3, #4]
	mov	r0, r0, asl #3
	str	r0, [r3, #12]
	ldr	r3, [r8, #3596]
	str	r3, [r2, #4]
.L1250:
	ldr	r0, [r9, #3476]
	ldr	r1, [r9, #3472]
	ldrb	r5, [r4, #8]	@ zero_extendqisi2
	mul	r1, r1, r0
	cmp	r5, #0
	sub	r1, r1, #1
	beq	.L1251
	ldr	r2, [fp, #-52]
	sub	ip, r10, r7, asl #2
	add	r3, r0, #1
	add	ip, r2, ip
	ldr	r2, [fp, #-48]
	add	r3, r3, r3, lsr #31
	add	ip, ip, #520
	ldr	lr, [ip, #8]
	rsb	r2, r10, r2
	add	r2, r4, r2
	sub	r3, lr, r3, asr #1
	add	r2, r2, #24064
	cmp	r1, r3
	str	r3, [r2]
	movcs	r1, #0
	movcc	r1, #1
	cmp	r3, #0
	movle	r1, #0
	cmp	r1, #0
	bne	.L1288
.L1252:
	ldr	r3, [fp, #-48]
	rsb	r7, r10, r3
	add	r4, r4, r7
	b	.L1293
.L1228:
	mov	r1, r7
	ldr	r0, [fp, #-48]
	bl	VC1_ClearCurPacket
	movw	r2, #6039
	ldr	r1, .L1300+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	b	.L1230
.L1248:
	mov	r1, r7
	ldr	r0, [fp, #-48]
	bl	VC1_ClearCurPacket
	movw	r2, #6122
	ldr	r1, .L1300+4
	mov	r0, r10
	bl	dprint_vfmw
	mov	r0, r10
	b	.L1230
.L1249:
	ldr	r3, [fp, #-48]
	movw	ip, #24072
	mov	r2, #0
	rsb	r3, r10, r3
	add	r3, r4, r3
	add	r0, r3, #24064
	add	lr, r3, #23808
	add	lr, lr, #232
	str	r2, [lr, #4]
	str	r2, [r3, ip]
	str	r2, [r0, #12]
	str	r2, [r0, #4]
	str	r2, [r0, #20]
	b	.L1250
.L1299:
	ldr	r1, [fp, #-48]
	mov	r0, r4
	bl	VC1_ClearCurPic
	ldr	r2, [r9, #3556]
	mov	r0, r5
	ldr	r1, .L1300+12
	bl	dprint_vfmw
	mov	r0, r10
	b	.L1230
.L1231:
	ldr	r2, [fp, #-48]
	movw	ip, #24072
	ldr	r3, [fp, #-56]
	rsb	r3, r2, r3
	mov	r2, #0
	add	r3, r4, r3
	add	r0, r3, #24064
	add	lr, r3, #23808
	add	lr, lr, #232
	str	r2, [lr, #4]
	str	r2, [r3, ip]
	str	r2, [r0, #12]
	str	r2, [r0, #4]
	str	r2, [r0, #20]
	b	.L1232
.L1251:
	ldr	r3, [fp, #-48]
	sub	r2, r10, r7, asl #2
	rsb	r0, r10, r3
	ldr	r3, [fp, #-52]
	add	r0, r4, r0
	add	r2, r3, r2
	add	r0, r0, #24064
	add	r2, r2, #520
	ldr	r3, [r2, #8]
	cmp	r1, r3
	str	r3, [r0]
	bcs	.L1252
	ldr	r2, [r2, #8]
	mov	r0, #1
	str	r1, [sp]
	ldr	r1, .L1300+16
	bl	dprint_vfmw
	mov	r0, r5
	b	.L1230
.L1233:
	ldr	r3, [fp, #-48]
	ldr	r0, [fp, #-56]
	sub	r2, r3, r7, asl #2
	rsb	r0, r3, r0
	ldr	r3, [fp, #-52]
	add	r0, r4, r0
	add	r2, r3, r2
	add	r0, r0, #24064
	add	r2, r2, #520
	ldr	r3, [r2, #8]
	cmp	r1, r3
	str	r3, [r0]
	bcs	.L1234
	ldr	r2, [r2, #8]
	mov	r0, #1
	str	r1, [sp]
	ldr	r1, .L1300+16
	bl	dprint_vfmw
	mov	r0, r10
	b	.L1230
.L1237:
	ldr	r1, .L1300+20
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	b	.L1230
.L1288:
	ldr	r2, [ip, #8]
	ldr	r1, .L1300+24
	str	r0, [sp]
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	b	.L1230
.L1301:
	.align	2
.L1300:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC59
	.word	.LC58
	.word	.LC57
	.word	.LC48
	.word	.LC49
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
	bne	.L1303
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
.L1303:
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
	ldrb	r3, [r4, #3538]	@ zero_extendqisi2
	add	r0, r0, #52
	str	r0, [r4, #3568]
	cmp	r3, #8
	strneb	r3, [r4, #451]
	moveq	r3, #6
	movne	r3, #0
	streqb	r3, [r4, #451]
	moveq	r2, #1
	strneb	r3, [r4, #452]
	ldrb	r3, [r4, #3536]	@ zero_extendqisi2
	streqb	r2, [r4, #452]
	cmp	r3, #0
	ldr	r2, [r1, #20]
	ldr	r1, [r1]
	beq	.L1311
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	BsInit
.L1311:
	bl	BsInit
	ldr	r0, [r4, #3544]
	add	r5, r5, #32768
	add	r2, r4, #3552
	adds	r3, r0, #15
	add	ip, r4, #3600
	addmi	r3, r0, #30
	add	ip, ip, #4
	mov	r3, r3, asr #4
	str	r3, [r5, #3912]
	ldr	r1, [r4, #3548]
	str	r0, [r5, #3928]
	adds	r3, r1, #15
	addmi	r3, r1, #30
	str	r1, [r5, #3932]
	mov	r3, r3, asr #4
	str	r3, [r5, #3916]
	ldmia	r2, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
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
	add	r1, r0, #48896
	add	r1, r1, #164
	beq	.L1320
	ldr	r2, [r4, #12]
	add	r3, r0, #45056
	str	r2, [r3, #4016]
	ldr	r2, [r4]
	str	r2, [r3, #4004]
	ldr	r2, [r4, #16]
	str	r2, [r3, #4024]
	ldrb	r2, [r4, #6]	@ zero_extendqisi2
	strb	r2, [r3, #4012]
	ldr	r2, [r4, #20]
	str	r2, [r3, #4032]
	ldrb	r2, [r4, #6]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1315
	ldr	r2, [r4, #92]
	str	r2, [r3, #4020]
	ldr	r2, [r4, #80]
	str	r2, [r3, #4008]
	ldr	r2, [r4, #96]
	str	r2, [r3, #4028]
	ldrb	r2, [r4, #86]	@ zero_extendqisi2
	strb	r2, [r3, #4013]
	ldr	r2, [r4, #100]
	str	r2, [r3, #4036]
	ldrb	r3, [r4, #86]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1321
.L1316:
	bl	VC1_SetCodeInfo
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1321:
	add	r0, r0, #49152
	ldr	r0, [r0, #416]
	bl	VC1_ClearCurPacket
	ldrb	r2, [r4, #86]	@ zero_extendqisi2
	ldr	r1, .L1322
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1315:
	mov	r2, #0
	str	r2, [r3, #4020]
	str	r2, [r3, #4008]
	str	r2, [r3, #4028]
	strb	r2, [r3, #4013]
	str	r2, [r3, #4036]
	b	.L1316
.L1320:
	ldr	r1, .L1322+4
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1323:
	.align	2
.L1322:
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
	ldr	r3, .L1353
	add	r5, r0, #12544
	add	r5, r5, #24
	mov	r4, r0
	mov	r2, #160
	ldr	r3, [r3, #52]
	mov	r0, r5
	blx	r3
	mov	r1, r5
	mov	r0, r4
	bl	VC1_ReceivePacket
	subs	r7, r0, #0
	beq	.L1348
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L1349
.L1327:
	add	r3, r4, #12288
	add	r6, r4, #49152
	ldrb	r3, [r3, #440]	@ zero_extendqisi2
	strb	r3, [r6, #24]
.L1328:
	add	r5, r4, #12672
	add	r1, r4, #15872
	add	r5, r5, #56
	add	r1, r1, #20
	ldr	r2, [r6, #416]
	mov	r0, r5
	bl	VC1_Vfmw_ParseHeader
	subs	r8, r0, #0
	beq	.L1350
	mov	r0, r4
	bl	VC1_Vfmw_DecodeArbiter
	cmp	r0, #0
	bne	.L1351
.L1331:
	mov	r7, #0
.L1326:
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1350:
	add	r1, r4, #48896
	ldr	r0, [r6, #416]
	add	r1, r1, #164
	bl	VC1_ClearCurPacket
	ldr	r3, .L1353+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1331
	ldr	r0, [r6, #416]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	mov	r7, r8
	blx	ip
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1349:
	ldr	r3, [r3, #872]
	cmp	r3, #1
	bne	.L1327
	add	r6, r4, #49152
	add	r3, r4, #12288
	ldrb	r2, [r6, #24]	@ zero_extendqisi2
	strb	r2, [r3, #440]
	b	.L1328
.L1348:
	add	r4, r4, #45056
	ldr	r1, .L1353+8
	ldr	r2, [r4, #4000]
	bl	dprint_vfmw
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1351:
	mov	r0, r4
	bl	VC1_Vfmw_PrepareDecode
	subs	r7, r0, #0
	bne	.L1332
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L1352
.L1333:
	mov	r0, r5
	ldr	r1, [r6, #416]
	bl	VC1_ClearCurPic
.L1334:
	ldr	r3, .L1353+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1331
	mov	r3, #0
	ldr	r0, [r6, #416]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L1326
.L1352:
	ldr	r3, [r3, #872]
	cmp	r3, #1
	bne	.L1333
	b	.L1334
.L1332:
	mov	r0, r4
	bl	VC1_Vfmw_StartDecode
	subs	r7, r0, #0
	movne	r7, r8
	bne	.L1326
	add	r4, r4, #32768
	mov	r2, #1
	ldr	r0, [r6, #416]
	ldr	r1, [r4, #3988]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #3896]
	ldr	r3, .L1353+4
	strb	r7, [r2]
	ldr	r2, [r4, #3896]
	strb	r7, [r2, #1]
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1331
	ldr	r0, [r6, #416]
	mov	r3, r7
	mov	r2, r7
	mov	r1, #113
	blx	ip
	b	.L1326
.L1354:
	.align	2
.L1353:
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
	ldrsb	r3, [r0, #62]
	cmp	r3, #1
	moveq	r0, #2
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VC1_GetPacketState, .-VC1_GetPacketState
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.14355, %object
	.size	__func__.14355, 13
__func__.14355:
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
	.type	__func__.14666, %object
	.size	__func__.14666, 17
__func__.14666:
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
	ASCII(.ascii	"pImg is null, pCurImg %p pBwdImg %p pFwdImg %p\012\000" )
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
