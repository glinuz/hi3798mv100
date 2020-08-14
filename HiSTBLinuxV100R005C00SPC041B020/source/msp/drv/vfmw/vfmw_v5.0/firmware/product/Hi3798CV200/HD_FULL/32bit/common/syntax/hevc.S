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
	.file	"hevc.c"
	.text
	.align	2
	.type	compare_pic_by_poc_asc, %function
compare_pic_by_poc_asc:
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
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #12]
	ldr	r3, [r3, #12]
	cmp	r2, r3
	blt	.L3
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L3:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_pic_by_poc_asc, .-compare_pic_by_poc_asc
	.align	2
	.type	HEVC_SetFrmRepeatCount.part.3, %function
HEVC_SetFrmRepeatCount.part.3:
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
	ldr	ip, [r0, #276]
	ldr	r2, [r0, #2504]
	ldr	r3, [r0, #272]
	add	r2, r2, ip
	add	ip, r2, #1
	cmp	r3, ip
	movcc	r3, #0
	strcc	r3, [r1, #24]
	ldmccfd	sp, {fp, sp, pc}
	sub	r3, r3, #1
	rsb	r3, r2, r3
	cmp	r3, #1
	movhi	r3, #2
	movls	r3, #1
	strhi	r3, [r1, #24]
	strls	r3, [r1, #24]
	ldrhi	r3, [r0, #276]
	ldrls	r3, [r0, #276]
	addhi	r3, r3, #2
	addls	r3, r3, #1
	str	r3, [r0, #276]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_SetFrmRepeatCount.part.3, .-HEVC_SetFrmRepeatCount.part.3
	.global	__aeabi_uidiv
	.align	2
	.global	HEVC_InitSigLastScan
	.type	HEVC_InitSigLastScan, %function
HEVC_InitSigLastScan:
	UNWIND(.fnstart)
	@ args = 16, pretend = 0, frame = 24
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
	ldr	r4, [fp, #8]
	str	r0, [fp, #-48]
	cmp	r4, #15
	ldr	r6, [fp, #4]
	bgt	.L10
	cmp	r4, #8
	mul	r9, r4, r4
	beq	.L43
	cmp	r9, #0
	movne	r10, r6
	beq	.L10
.L12:
	mov	r5, #0
	rsb	r7, r4, #1
	mov	r8, r5
.L18:
	cmp	r4, r8
	bgt	.L30
	rsb	lr, r4, r8
	sub	r2, r4, #1
	add	lr, lr, #1
.L19:
	cmp	r2, #0
	cmpge	r4, lr
	ble	.L16
	mla	r0, r2, r4, lr
	add	ip, r10, r5, lsl #2
	mov	r1, r5
.L17:
	add	r1, r1, #1
	sub	r2, r2, #1
	rsb	r3, r5, r1
	str	r0, [ip], #4
	add	r3, r3, lr
	add	r0, r0, r7
	cmp	r2, #0
	cmpge	r4, r3
	bgt	.L17
	mov	r5, r1
.L16:
	cmp	r9, r5
	add	r8, r8, #1
	bhi	.L18
.L10:
	cmp	r4, #4
	ble	.L9
	mov	r3, r4, asr #2
	ldr	r1, [fp, #-48]
	str	r3, [fp, #-64]
	mov	r2, r3
	add	r3, r1, r3
	mul	r2, r2, r2
	mov	r0, r2
	str	r2, [fp, #-60]
	cmp	r0, #0
	ldrsb	r2, [r3, #10]
	add	r2, r2, #1
	beq	.L9
	add	r3, r1, #324
.L14:
	ldr	r1, .L44
	rsb	r7, r4, #1
	str	r3, [fp, #-52]
	mov	r3, #0
	add	r2, r1, r2, lsl #16
	str	r3, [fp, #-48]
	sub	r3, r2, #4
	str	r3, [fp, #-56]
.L28:
	ldr	r3, [fp, #-56]
	cmp	r4, #32
	ldr	r9, [fp, #-64]
	mov	r8, #0
	ldr	r5, [r3, #4]!
	mov	r1, r9
	str	r3, [fp, #-56]
	ldreq	r3, [fp, #-52]
	ldreq	r5, [r3]
	mov	r0, r5
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-48]
	mov	r3, r8
	mov	r2, r8
	mov	lr, r8
	mov	r10, ip, asl #4
	mla	r1, r4, r0, r5
	mls	r1, r9, r0, r1
	mov	r9, r1, asl #2
.L22:
	cmp	lr, #3
	mvn	r1, r2
	mov	r1, r1, lsr #31
	movgt	r1, #0
	cmp	r1, #0
	beq	.L23
	mla	r1, r2, r4, lr
	add	r0, r3, r10
	rsb	lr, r3, lr
	add	r0, r6, r0, lsl #2
	add	r1, r1, r9
.L24:
	add	r3, r3, #1
	sub	r2, r2, #1
	add	r5, lr, r3
	str	r1, [r0], #4
	cmp	r5, #3
	mvn	ip, r2
	mov	ip, ip, lsr #31
	add	r1, r1, r7
	movgt	ip, #0
	cmp	ip, #0
	bne	.L24
.L23:
	cmp	r3, #15
	add	r8, r8, #1
	bhi	.L25
	cmp	r8, #3
	movle	r2, r8
	movle	lr, #0
	subgt	lr, r8, #3
	movgt	r2, #3
	b	.L22
.L25:
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-48]
	add	r2, r2, #4
	str	r2, [fp, #-52]
	ldr	r2, [fp, #-60]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L28
.L9:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L30:
	mov	r2, r8
	mov	lr, #0
	b	.L19
.L43:
	add	r3, r0, #324
	cmp	r9, #0
	mov	r10, r3
	bne	.L12
	ldr	r2, [fp, #-48]
	mov	r1, #2
	str	r1, [fp, #-64]
	mov	r1, #4
	str	r1, [fp, #-60]
	ldrsb	r2, [r2, #12]
	add	r2, r2, #1
	b	.L14
.L45:
	.align	2
.L44:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	HEVC_InitSigLastScan, .-HEVC_InitSigLastScan
	.align	2
	.global	HEVC_InitScalingOrderTable
	.type	HEVC_InitScalingOrderTable, %function
HEVC_InitScalingOrderTable:
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
	UNWIND(.pad #16)
	sub	sp, sp, #16
	add	r3, r0, #9
	add	r1, r0, #138
	mov	r7, r0
	mvn	r2, #0
.L47:
	strb	r2, [r3, #1]!
	cmp	r3, r1
	bne	.L47
	ldr	r6, .L51
	mov	r2, #0
	mov	r3, #2
	mov	r5, r2
	mov	r4, r3
	strb	r2, [r7, #14]
	strb	r3, [r7, #26]
	mov	r2, #1
	mov	r3, #3
	strb	r2, [r7, #18]
	strb	r3, [r7, #42]
	mov	r3, #4
	strb	r3, [r7, #74]
.L48:
	mov	r3, #0
	str	r5, [sp, #12]
	str	r4, [sp, #8]
	add	r5, r5, #1
	str	r4, [sp, #4]
	mov	r2, r3
	str	r6, [sp]
	mov	r1, r3
	mov	r0, r7
	mov	r4, r4, asl #1
	bl	HEVC_InitSigLastScan
	cmp	r5, #7
	add	r6, r6, #65536
	bne	.L48
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L52:
	.align	2
.L51:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	HEVC_InitScalingOrderTable, .-HEVC_InitScalingOrderTable
	.global	__aeabi_idiv
	.align	2
	.global	HEVCGetDar
	.type	HEVCGetDar, %function
HEVCGetDar:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r0, #0
	mov	ip, r1
	moveq	r4, r0
	beq	.L54
	subs	lr, r0, #255
	movne	lr, #1
	cmp	r0, #16
	movle	r4, #0
	andgt	r4, lr, #1
	cmp	r4, #0
	movne	r4, #0
	bne	.L54
	cmp	r2, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
	cmp	r0, #255
	movne	r1, #0
	andeq	r1, r1, #1
	cmp	r1, #0
	bne	.L54
	cmp	r0, #1
	moveq	r4, #5
	beq	.L54
	cmp	lr, #0
	str	r3, [fp, #-24]
	ldrne	r2, .L76
	addne	r1, r2, r0, lsl #3
	ldrne	ip, [r2, r0, asl #3]
	ldrne	r2, [r1, #4]
	mov	r0, ip, asl #10
	mov	r1, r2
	bl	__aeabi_idiv
	ldr	r3, [fp, #-24]
	ldr	r1, [fp, #4]
	mov	r4, r0
	mov	r0, r3, asl #10
	bl	__aeabi_idiv
	movw	r3, #2405
	movw	r2, #2262
	mul	r0, r0, r4
	add	r4, r0, #1020
	cmp	r0, #0
	add	r4, r4, #3
	movlt	r0, r4
	mov	r0, r0, asr #10
	cmp	r0, r3
	rsble	r3, r0, #2400
	subgt	r3, r0, #2400
	addle	r3, r3, #6
	subgt	r3, r3, #6
	cmp	r0, r2
	bgt	.L58
	rsb	r2, r0, #2256
	add	r2, r2, #7
	cmp	r3, r2
	bge	.L59
.L69:
	mov	r4, #4
.L54:
	mov	r2, r4
	ldr	r1, .L76+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L58:
	sub	r2, r0, #2256
	sub	r2, r2, #7
	cmp	r3, r2
	blt	.L69
.L59:
	movw	r3, #1819
	cmp	r0, r3
	bgt	.L60
	rsb	r3, r0, #1808
	add	r3, r3, #12
	cmp	r2, r3
	blt	.L71
.L61:
	movw	r2, #1364
	cmp	r0, r2
	rsble	r0, r0, #1360
	subgt	r0, r0, #1360
	addle	r0, r0, #5
	subgt	r0, r0, #5
	cmp	r3, r0
	movlt	r4, #2
	movge	r4, #1
	b	.L54
.L60:
	sub	r3, r0, #1808
	sub	r3, r3, #12
	cmp	r2, r3
	bge	.L61
.L71:
	mov	r4, #3
	b	.L54
.L77:
	.align	2
.L76:
	.word	.LANCHOR1
	.word	.LC4
	UNWIND(.fnend)
	.size	HEVCGetDar, .-HEVCGetDar
	.align	2
	.global	HEVC_PrintNaluArraySlot
	.type	HEVC_PrintNaluArraySlot, %function
HEVC_PrintNaluArraySlot:
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
	ldr	r1, .L84
	mov	r5, r0
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #160]
	cmn	r3, #1
	beq	.L82
	mov	r8, #0
	movw	r6, #52429
	add	r7, r5, #65536
	mov	r4, r8
	movt	r6, 52428
	b	.L81
.L80:
	ldr	r3, [r5, #160]
	add	r3, r3, #1
	cmp	r3, r4
	bls	.L82
.L81:
	ldr	r3, [r7, #1840]
	mov	r2, r4
	ldr	r1, .L84+4
	mov	r0, #1
	add	r3, r3, r8
	add	r8, r8, #100
	ldr	ip, [r3, #36]
	str	ip, [sp]
	bl	dprint_vfmw
	umull	r2, r3, r4, r6
	mov	r2, r3, lsr #3
	mov	r3, r2, asl #3
	add	r3, r3, r2, lsl #1
	cmp	r4, r3
	add	r4, r4, #1
	bne	.L80
	ldr	r1, .L84+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #160]
	add	r3, r3, #1
	cmp	r3, r4
	bhi	.L81
.L82:
	ldr	r1, .L84+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, .L84+12
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	dprint_vfmw
.L85:
	.align	2
.L84:
	.word	.LC5
	.word	.LC8
	.word	.LC6
	.word	.LC7
	UNWIND(.fnend)
	.size	HEVC_PrintNaluArraySlot, .-HEVC_PrintNaluArraySlot
	.align	2
	.global	HEVC_PrintCurrSlicelistX
	.type	HEVC_PrintCurrSlicelistX, %function
HEVC_PrintCurrSlicelistX:
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
	add	r6, r0, #53248
	mov	r7, r0
	ldr	r1, .L107
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r6, #1176]
	ldr	r1, .L107+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #1176]
	cmp	r3, #0
	addne	r5, r7, #2320
	movne	r4, #0
	addne	r5, r5, #12
	beq	.L94
.L93:
	ldr	r3, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L107+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #1
	ldr	r3, [r5]
	add	r4, r4, r0
	ldr	r1, .L107+12
	cmp	r3, #0
	beq	.L91
	ldr	r2, [r3, #32]
	bl	dprint_vfmw
	ldr	r3, [r5]
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L91
	ldr	r1, [r3, #32]
	ldr	r0, [r7, #244]
	bl	FSP_GetLogicFs
	ldr	r1, .L107+16
	mov	r2, r0
	mov	r0, #1
	bl	dprint_vfmw
.L91:
	ldr	r1, .L107+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #1176]
	cmp	r3, r4
	bhi	.L93
.L94:
	ldr	r2, [r6, #1180]
	mov	r0, #1
	ldr	r1, .L107+24
	bl	dprint_vfmw
	ldr	r3, [r6, #1180]
	cmp	r3, #0
	addne	r5, r7, #2400
	movne	r4, #0
	beq	.L89
.L98:
	ldr	r3, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L107+28
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #1
	ldr	r3, [r5]
	add	r4, r4, r0
	ldr	r1, .L107+12
	cmp	r3, #0
	beq	.L96
	ldr	r2, [r3, #32]
	bl	dprint_vfmw
	ldr	r3, [r5]
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L96
	ldr	r1, [r3, #32]
	ldr	r0, [r7, #244]
	bl	FSP_GetLogicFs
	ldr	r1, .L107+16
	mov	r2, r0
	mov	r0, #1
	bl	dprint_vfmw
.L96:
	ldr	r1, .L107+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #1180]
	cmp	r3, r4
	bhi	.L98
.L89:
	ldr	r1, .L107+32
	mov	r0, #1
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L108:
	.align	2
.L107:
	.word	.LC9
	.word	.LC10
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC6
	.word	.LC11
	.word	.LC16
	.word	.LC15
	UNWIND(.fnend)
	.size	HEVC_PrintCurrSlicelistX, .-HEVC_PrintCurrSlicelistX
	.align	2
	.global	HEVC_PrintFrameStoreState
	.type	HEVC_PrintFrameStoreState, %function
HEVC_PrintFrameStoreState:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r1, .L116
	mov	r10, r0
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r10, #176]
	cmp	r3, #0
	addne	r8, r10, #67584
	movne	r9, #0
	addne	r8, r8, #600
	beq	.L113
.L112:
	ldrb	ip, [r8, #2]	@ zero_extendqisi2
	mov	r3, r8
	mov	r2, r9
	ldr	r1, .L116+4
	mov	r0, #1
	add	r5, r8, #624
	str	ip, [sp, #8]
	mov	r4, #0
	ldrb	ip, [r8, #1]	@ zero_extendqisi2
	str	ip, [sp, #4]
	ldr	ip, [r8, #4]
	str	ip, [sp]
	bl	dprint_vfmw
.L111:
	ldrd	r6, [r5, #8]!
	mov	r2, r4
	ldr	r1, .L116+8
	add	r4, r4, #1
	mov	r0, #1
	strd	r6, [sp]
	bl	dprint_vfmw
	cmp	r4, #4
	bne	.L111
	ldr	r1, .L116+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r10, #176]
	add	r9, r9, #1
	add	r8, r8, #704
	cmp	r3, r9
	bhi	.L112
.L113:
	ldr	r1, .L116+16
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L117:
	.align	2
.L116:
	.word	.LC17
	.word	.LC19
	.word	.LC20
	.word	.LC6
	.word	.LC18
	UNWIND(.fnend)
	.size	HEVC_PrintFrameStoreState, .-HEVC_PrintFrameStoreState
	.align	2
	.global	HEVC_PrintAPCState
	.type	HEVC_PrintAPCState, %function
HEVC_PrintAPCState:
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
	ldr	r1, .L121
	mov	r5, r0
	mov	r7, r0
	mov	r0, #1
	add	r6, r5, #2864
	bl	dprint_vfmw
	ldr	r3, [r5, #2800]
	ldr	r2, [r7, #2804]!
	mov	r0, #1
	ldr	r1, .L121+4
	add	r5, r5, #2928
	bl	dprint_vfmw
	add	r6, r6, #4
	add	r5, r5, #4
	mov	r4, #0
.L119:
	ldr	lr, [r5, #4]!
	mov	r2, r4
	ldr	ip, [r6, #4]!
	add	r4, r4, #1
	ldr	r3, [r7, #4]!
	mov	r0, #1
	ldr	r1, .L121+8
	stmia	sp, {ip, lr}
	bl	dprint_vfmw
	cmp	r4, #16
	bne	.L119
	ldr	r1, .L121+12
	mov	r0, #1
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L122:
	.align	2
.L121:
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	UNWIND(.fnend)
	.size	HEVC_PrintAPCState, .-HEVC_PrintAPCState
	.align	2
	.global	HEVC_PrintDPBState
	.type	HEVC_PrintDPBState, %function
HEVC_PrintDPBState:
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
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	r1, .L139
	mov	r9, r0
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r9, #2520]
	ldr	lr, [r9, #2516]
	mov	r0, #1
	ldr	ip, [r9, #2524]
	ldr	r3, [r9, #2508]
	ldr	r2, [r9, #2504]
	stmia	sp, {r1, ip, lr}
	ldr	r1, .L139+4
	bl	dprint_vfmw
	ldr	r3, [r9, #2508]
	cmp	r3, #0
	addne	r4, r9, #2512
	movne	r5, #0
	addne	r4, r4, #12
	beq	.L128
.L127:
	ldr	r3, [r4, #4]!
	ldr	r0, [r9, #244]
	ldr	ip, [r3, #4]
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldrb	lr, [r3, #1]	@ zero_extendqisi2
	str	ip, [fp, #-52]
	ldr	ip, [r3, #52]
	ldr	r1, [r3, #32]
	str	r2, [fp, #-48]
	str	lr, [fp, #-56]
	str	ip, [fp, #-60]
	ldrb	r8, [r3, #2]	@ zero_extendqisi2
	ldr	r10, [r3, #12]
	bl	FSP_GetLogicFs
	ldr	r3, [r4]
	ldr	r1, [r3, #32]
	mov	r6, r0
	ldr	r0, [r9, #244]
	bl	FSP_GetRef
	ldr	r3, [r4]
	ldr	r1, [r3, #32]
	mov	r7, r0
	ldr	r0, [r9, #244]
	bl	FSP_GetDisplay
	ldr	r1, [r4]
	ldr	ip, [fp, #-60]
	mov	r2, r5
	add	r5, r5, #1
	ldr	lr, [r1, #228]
	mov	r3, r1
	str	lr, [sp, #40]
	ldr	r1, [r1, #32]
	ldr	lr, [fp, #-56]
	str	ip, [sp, #20]
	str	r1, [sp, #36]
	ldr	ip, [fp, #-52]
	ldr	r1, [fp, #-48]
	str	r10, [sp, #16]
	str	r8, [sp, #12]
	str	r1, [sp]
	str	lr, [sp, #8]
	str	ip, [sp, #4]
	str	r7, [sp, #28]
	str	r6, [sp, #24]
	ldr	r1, .L139+8
	str	r0, [sp, #32]
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r9, #2508]
	cmp	r3, r5
	bhi	.L127
.L128:
	ldr	r1, .L139+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r9, #2508]
	cmp	r3, #0
	addne	r5, r9, #2512
	movne	r6, #0
	addne	r5, r5, #12
	bne	.L135
	b	.L126
.L138:
	bl	dprint_vfmw
	ldr	r2, [r4, #632]
	ldr	r1, .L139+16
	mov	r0, #1
	bl	dprint_vfmw
	ldrsb	r2, [r4]
	ldr	r1, .L139+20
	mov	r0, #1
	bl	dprint_vfmw
	ldrsb	r2, [r4, #1]
	ldr	r1, .L139+24
	mov	r0, #1
	bl	dprint_vfmw
.L130:
	cmp	r8, #0
	mov	r2, r8
	ldr	r1, .L139+28
	mov	r0, #1
	beq	.L131
	bl	dprint_vfmw
	ldr	r2, [r8, #88]
	ldr	r1, .L139+32
	mov	r0, #1
	bl	dprint_vfmw
.L132:
	cmp	r7, #0
	mov	r2, r7
	ldr	r1, .L139+36
	mov	r0, #1
	beq	.L133
	bl	dprint_vfmw
	ldr	r2, [r7, #88]
	ldr	r1, .L139+40
	mov	r0, #1
	bl	dprint_vfmw
.L134:
	ldr	r1, .L139+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r9, #2508]
	add	r6, r6, #1
	cmp	r3, r6
	bls	.L126
.L135:
	ldr	r3, [r5, #4]!
	ldr	r0, [r9, #244]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	ldr	r3, [r5]
	ldr	r1, [r3, #32]
	mov	r4, r0
	ldr	r0, [r9, #244]
	bl	FSP_GetDecPhyFs
	ldr	r3, [r5]
	ldr	r1, [r3, #32]
	mov	r8, r0
	ldr	r0, [r9, #244]
	bl	FSP_GetDispPhyFs
	mov	r2, r6
	ldr	r1, .L139+44
	mov	r7, r0
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r4, #0
	mov	r2, r4
	ldr	r1, .L139+48
	mov	r0, #1
	bne	.L138
	ldr	r1, .L139+52
	mov	r0, #1
	bl	dprint_vfmw
	b	.L130
.L133:
	ldr	r1, .L139+56
	mov	r0, #1
	bl	dprint_vfmw
	b	.L134
.L131:
	ldr	r1, .L139+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L132
.L126:
	ldr	r1, .L139+64
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L140:
	.align	2
.L139:
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC6
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC35
	.word	.LC36
	.word	.LC38
	.word	.LC39
	.word	.LC29
	.word	.LC30
	.word	.LC34
	.word	.LC40
	.word	.LC37
	.word	.LC28
	UNWIND(.fnend)
	.size	HEVC_PrintDPBState, .-HEVC_PrintDPBState
	.align	2
	.global	HEVC_IsSliceUnit
	.type	HEVC_IsSliceUnit, %function
HEVC_IsSliceUnit:
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
	cmp	r0, #9
	bls	.L143
	sub	r0, r0, #16
	cmp	r0, #5
	movhi	r0, #0
	movls	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L143:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsSliceUnit, .-HEVC_IsSliceUnit
	.align	2
	.global	HEVC_IsIUnit
	.type	HEVC_IsIUnit, %function
HEVC_IsIUnit:
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
	sub	r0, r0, #16
	cmp	r0, #5
	movhi	r0, #0
	movls	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsIUnit, .-HEVC_IsIUnit
	.align	2
	.global	HEVC_IsNonRefPic
	.type	HEVC_IsNonRefPic, %function
HEVC_IsNonRefPic:
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
	bics	r0, r0, #6
	subne	r0, r0, #8
	clzne	r0, r0
	movne	r0, r0, lsr #5
	moveq	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsNonRefPic, .-HEVC_IsNonRefPic
	.align	2
	.global	HEVC_IsFlushUnit
	.type	HEVC_IsFlushUnit, %function
HEVC_IsFlushUnit:
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
	sub	r0, r0, #16
	cmp	r0, #4
	movhi	r0, #0
	movls	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsFlushUnit, .-HEVC_IsFlushUnit
	.align	2
	.global	HEVC_IsPVRFlushPic
	.type	HEVC_IsPVRFlushPic, %function
HEVC_IsPVRFlushPic:
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
	ldr	r3, [r3, #348]
	cmp	r3, #0
	bne	.L151
	ldr	r2, [r0, #2324]
	ldr	r1, [r2, #640]
	cmp	r1, #1
	beq	.L155
.L153:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L151:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L155:
	ldr	r0, [r2, #636]
	cmp	r0, #0
	bne	.L153
	ldr	r3, [r2, #644]
	cmp	r3, #0
	ldmnefd	sp, {fp, sp, pc}
	ldr	r3, [r2, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	movls	r0, #0
	movhi	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsPVRFlushPic, .-HEVC_IsPVRFlushPic
	.align	2
	.global	HEVC_IsEosNal
	.type	HEVC_IsEosNal, %function
HEVC_IsEosNal:
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
	mov	r1, #32
	mov	r4, r0
	ldr	r0, [r0, #2308]
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	ldr	r0, [r4, #2308]
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r5, r3
	moveq	r0, #1
	movne	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsEosNal, .-HEVC_IsEosNal
	.align	2
	.global	HEVC_CalcTileInfo
	.type	HEVC_CalcTileInfo, %function
HEVC_CalcTileInfo:
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
	add	r5, r0, #24576
	str	r3, [fp, #-48]
	mov	r6, r2
	ldr	r3, [r5, #2840]
	mov	r8, r1
	ldr	r2, [r5, #2836]
	ldr	r7, [r3, #68]
	ldr	r9, [r3, #72]
	ldr	r2, [r2, #148]
	sub	r3, r7, #1
	cmp	r3, #9
	str	r2, [fp, #-52]
	bhi	.L185
	sub	r3, r9, #1
	cmp	r3, #10
	bhi	.L186
	ldr	r4, .L187
	mov	r2, #512
	mov	r1, #0
	mov	r0, r8
	ldr	r3, [r4, #48]
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #80
	mov	r1, #0
	mov	r0, r6
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #88
	mov	r1, #0
	ldr	r0, [fp, #-48]
	blx	r3
	cmp	r7, #0
	ble	.L166
	ldr	r3, [fp, #-52]
	mov	r2, #0
	mov	r1, r2
	mov	r0, r6
	sub	r10, r3, #4
.L165:
	str	r2, [r6, r1, asl #3]
	mov	r3, r2, asl r10
	ldr	ip, [r5, #2840]
	add	ip, ip, r1, lsl #1
	ldrsh	ip, [ip, #176]
	add	r2, ip, r2
	sub	ip, r2, #1
	str	ip, [r0, #4]
	mov	lr, r2, asl r10
	cmp	r3, lr
	bge	.L171
	cmp	r3, #255
	bhi	.L167
	uxtb	r4, r1
	add	ip, r8, r3
	b	.L169
.L170:
	cmp	r3, #256
	beq	.L167
.L169:
	add	r3, r3, #1
	strb	r4, [ip], #1
	cmp	r3, lr
	bne	.L170
.L171:
	add	r1, r1, #1
	add	r0, r0, #8
	cmp	r1, r7
	bne	.L165
.L166:
	cmp	r9, #0
	ble	.L163
	ldr	r3, [fp, #-52]
	mov	ip, #0
	ldr	r7, [fp, #-48]
	mov	lr, ip
	sub	r4, r3, #4
	mov	r6, r7
.L173:
	str	ip, [r7, lr, asl #3]
	mov	r3, ip, asl r4
	ldr	r2, [r5, #2840]
	add	r2, r2, lr, lsl #1
	ldrsh	r2, [r2, #216]
	add	ip, r2, ip
	sub	r2, ip, #1
	str	r2, [r6, #4]
	mov	r1, ip, asl r4
	cmp	r3, r1
	bge	.L178
	cmp	r3, #255
	bhi	.L174
	add	r2, r3, #255
	uxtb	r0, lr
	add	r2, r8, r2
	b	.L176
.L177:
	cmp	r3, #256
	beq	.L174
.L176:
	add	r3, r3, #1
	strb	r0, [r2, #1]!
	cmp	r3, r1
	bne	.L177
.L178:
	add	lr, lr, #1
	add	r6, r6, #8
	cmp	lr, r9
	bne	.L173
.L163:
	mov	r0, #0
	b	.L159
.L167:
	ldr	r2, .L187+4
	mov	r0, #1
	ldr	r1, .L187+8
	bl	dprint_vfmw
	mvn	r0, #0
.L159:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L174:
	ldr	r2, .L187+4
	mov	r0, #1
	ldr	r1, .L187+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L159
.L186:
	mov	r1, #11
	mov	r3, r9
	str	r1, [sp]
	mov	r0, #1
	ldr	r2, .L187+4
	ldr	r1, .L187+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L159
.L185:
	mov	r1, #10
	mov	r3, r7
	str	r1, [sp]
	mov	r0, #1
	ldr	r2, .L187+4
	ldr	r1, .L187+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L159
.L188:
	.align	2
.L187:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+136
	.word	.LC43
	.word	.LC44
	.word	.LC42
	.word	.LC41
	UNWIND(.fnend)
	.size	HEVC_CalcTileInfo, .-HEVC_CalcTileInfo
	.align	2
	.global	HEVC_VPSEqual
	.type	HEVC_VPSEqual, %function
HEVC_VPSEqual:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	subs	r10, r0, #0
	beq	.L213
	cmp	r1, #0
	beq	.L214
	ldr	r4, [r10, #12]
	add	lr, r10, #12288
	ldr	r3, [r10, #16]
	add	ip, r1, #12288
	ldr	r6, [r1, #16]
	add	r2, r10, #12416
	ldr	r5, [r1, #12]
	add	r0, r1, #12416
	ldr	r9, [r1, #20]
	mov	r7, r2
	cmp	r3, r6
	cmpeq	r4, r5
	ldr	r8, [r10, #20]
	ldrb	r5, [r10, #1]	@ zero_extendqisi2
	add	r2, r2, #20
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	moveq	r4, #1
	movne	r4, #0
	cmp	r8, r9
	movne	r4, #0
	andeq	r4, r4, #1
	cmp	r5, r3
	movne	r9, #0
	andeq	r9, r4, #1
	ldr	r6, [r1, #24]
	ldr	r3, [r10, #24]
	add	r0, r0, #20
	ldr	r5, [r10, #8]
	add	r7, r7, #52
	ldr	r4, [r1, #8]
	cmp	r3, r6
	movne	r3, #0
	andeq	r3, r9, #1
	ldr	r9, [ip, #424]
	cmp	r5, r4
	movne	r6, #0
	andeq	r6, r3, #1
	ldr	r5, [lr, #424]
	ldrb	r3, [ip, #148]	@ zero_extendqisi2
	ldrb	r4, [lr, #148]	@ zero_extendqisi2
	cmp	r5, r9
	movne	r5, #0
	andeq	r5, r6, #1
	ldr	r9, [lr, #420]
	ldr	r6, [ip, #420]
	cmp	r4, r3
	movne	r4, #0
	andeq	r4, r5, #1
	cmp	r9, r6
	movne	r3, #0
	andeq	r3, r4, #1
.L193:
	ldrb	r5, [r2, #1]!	@ zero_extendqisi2
	ldrb	r4, [r0, #1]!	@ zero_extendqisi2
	cmp	r5, r4
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r7
	bne	.L193
	ldrb	r2, [lr, #182]	@ zero_extendqisi2
	add	r7, r10, #80
	ldrb	r6, [ip, #182]	@ zero_extendqisi2
	sub	r0, r8, #1
	ldrb	r5, [lr, #181]	@ zero_extendqisi2
	add	r9, r1, #48
	ldrb	r4, [ip, #181]	@ zero_extendqisi2
	add	r8, r1, #80
	str	r7, [fp, #-56]
	cmp	r2, r6
	cmpeq	r5, r4
	ldrb	r7, [lr, #183]	@ zero_extendqisi2
	ldrb	r4, [ip, #183]	@ zero_extendqisi2
	mov	r5, #0
	ldrb	lr, [lr, #184]	@ zero_extendqisi2
	moveq	r2, #1
	ldrb	ip, [ip, #184]	@ zero_extendqisi2
	movne	r2, #0
	ldrb	r6, [r10, #7]	@ zero_extendqisi2
	cmp	r7, r4
	movne	r2, #0
	andeq	r2, r2, #1
	add	r4, r10, #112
	str	r4, [fp, #-52]
	ldrb	r4, [r1, #7]	@ zero_extendqisi2
	cmp	lr, ip
	movne	ip, #0
	andeq	ip, r2, #1
	add	r7, r1, #112
	str	r1, [fp, #-60]
	cmp	r6, r4
	movne	ip, #0
	andeq	ip, ip, #1
	ldr	r1, [fp, #-56]
	str	r0, [fp, #-48]
	and	r6, r3, ip
	add	r0, r10, #48
	ldr	r2, [fp, #-52]
	str	r10, [fp, #-52]
.L194:
	ldr	r3, [r1, #4]!
	add	r5, r5, #1
	ldr	r10, [r8, #4]!
	ldr	r4, [r0, #4]!
	ldr	lr, [r9, #4]!
	ldr	ip, [r2, #4]!
	cmp	r3, r10
	cmpeq	r4, lr
	ldr	r10, [r7, #4]!
	moveq	r3, #1
	movne	r3, #0
	cmp	ip, r10
	movne	r3, #0
	andeq	r3, r3, #1
	ldr	ip, [fp, #-48]
	and	r6, r6, r3
	cmp	r5, ip
	bls	.L194
	ldr	r10, [fp, #-52]
	ldr	r1, [fp, #-60]
	ldr	r8, [r10, #32]
	ldr	r3, [r1, #32]
	ldr	r4, [r10, #28]
	ldr	r2, [r1, #28]
	cmp	r8, r3
	cmpeq	r4, r2
	moveq	r3, #1
	movne	r3, #0
	cmp	r8, #0
	and	r6, r6, r3
	beq	.L195
	add	r7, r10, #148
	add	r5, r1, #148
	mov	r9, #1
.L196:
	mov	r0, r5
	mov	r2, r7
	mov	r3, #0
.L197:
	ldr	lr, [r2, #4]!
	add	r3, r3, #1
	ldr	ip, [r0, #4]!
	cmp	lr, ip
	movne	r6, #0
	andeq	r6, r6, #1
	cmp	r3, r4
	bls	.L197
	add	r9, r9, #1
	add	r7, r7, #4
	cmp	r9, r8
	add	r5, r5, #4
	bls	.L196
.L195:
	ldrb	r3, [r10, #6]	@ zero_extendqisi2
	ldrb	r2, [r1, #6]	@ zero_extendqisi2
	cmp	r2, r3
	movne	r6, #0
	andeq	r6, r6, #1
	cmp	r3, #0
	beq	.L198
	ldr	r3, [r10, #36]
	ldr	r2, [r10, #40]
	ldr	r4, [r1, #40]
	ldr	lr, [r1, #36]
	ldrb	r0, [r10, #5]	@ zero_extendqisi2
	cmp	r2, r4
	cmpeq	r3, lr
	ldrb	ip, [r1, #5]	@ zero_extendqisi2
	moveq	r3, #1
	movne	r3, #0
	cmp	ip, r0
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r0, #0
	and	r6, r6, r3
	bne	.L215
.L199:
	ldr	r2, [r10, #48]
	ldr	r3, [r1, #48]
	cmp	r2, r3
	movne	r6, #0
	andeq	r6, r6, #1
.L198:
	eor	r6, r6, #1
	rsb	r0, r6, #0
.L191:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L215:
	ldr	r2, [r10, #44]
	ldr	r3, [r1, #44]
	cmp	r2, r3
	movne	r6, #0
	andeq	r6, r6, #1
	b	.L199
.L214:
	mov	r0, r1
	movw	r3, #1884
	ldr	r2, .L216
	ldr	r1, .L216+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L191
.L213:
	movw	r3, #1883
	ldr	r2, .L216
	ldr	r1, .L216+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L191
.L217:
	.align	2
.L216:
	.word	.LC45
	.word	.LC46
	UNWIND(.fnend)
	.size	HEVC_VPSEqual, .-HEVC_VPSEqual
	.align	2
	.global	HEVC_Sei_More_Data_In_Payload
	.type	HEVC_Sei_More_Data_In_Payload, %function
HEVC_Sei_More_Data_In_Payload:
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
	ldr	r2, [r0, #2308]
	mov	r5, r1
	ldr	r3, [r2, #24]
	mov	r0, r2
	ldr	r4, [r2, #28]
	rsb	r4, r4, r3
	bl	BsIsByteAligned
	cmp	r4, r5, asl #3
	movne	r3, #1
	moveq	r3, #0
	cmp	r0, #0
	moveq	r0, #0
	andne	r0, r3, #1
	eor	r0, r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_Sei_More_Data_In_Payload, .-HEVC_Sei_More_Data_In_Payload
	.align	2
	.global	HEVC_Sei_Payload_Extension_Present
	.type	HEVC_Sei_Payload_Extension_Present, %function
HEVC_Sei_Payload_Extension_Present:
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
	ldr	r3, .L226
	mov	r5, r1
	mov	r2, #32
	ldr	r1, [r0, #2308]
	sub	r0, fp, #52
	ldr	r3, [r3, #52]
	mov	r5, r5, asl #3
	blx	r3
	ldr	r3, [fp, #-28]
	ldr	r2, [fp, #-24]
	rsb	r3, r2, r3
	cmp	r3, r5
	movcs	r0, #0
	bcc	.L225
.L220:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L225:
	mov	r1, #1
	sub	r0, fp, #52
	bl	BsShow
	cmp	r0, #0
	moveq	r0, #1
	beq	.L220
	mov	r1, #1
	sub	r0, fp, #52
	bl	BsSkip
	ldr	r3, [fp, #-28]
	ldr	r4, [fp, #-24]
	rsb	r4, r4, r3
	b	.L221
.L222:
	bl	BsSkip
	ldr	r3, [fp, #-28]
	ldr	r4, [fp, #-24]
	rsb	r4, r4, r3
.L221:
	mov	r1, #1
	sub	r0, fp, #52
	bl	BsShow
	cmp	r4, r5
	mov	r1, #1
	movcs	r3, #0
	movcc	r3, #1
	cmp	r0, #0
	movne	r2, #0
	andeq	r2, r3, #1
	sub	r0, fp, #52
	cmp	r2, #0
	bne	.L222
	mov	r0, r3
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L227:
	.align	2
.L226:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_Sei_Payload_Extension_Present, .-HEVC_Sei_Payload_Extension_Present
	.align	2
	.global	HEVC_Sei_Decoded_Picture_Hash
	.type	HEVC_Sei_Decoded_Picture_Hash, %function
HEVC_Sei_Decoded_Picture_Hash:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r6, r0
	mov	r7, r1
	beq	.L236
	mov	r1, #8
	ldr	r0, [r0, #2308]
	bl	BsGet
	sub	r4, r7, #1
	add	r8, r7, #47
	mov	r3, r0
	cmp	r3, #0
	str	r0, [r7, #48]
	str	r0, [r7, #52]
	bne	.L230
.L241:
	add	r5, r4, #16
.L231:
	mov	r1, #8
	ldr	r0, [r6, #2308]
	bl	BsGet
	strb	r0, [r4, #1]!
	cmp	r4, r5
	bne	.L231
.L232:
	cmp	r5, r8
	mov	r4, r5
	beq	.L237
.L243:
	ldr	r3, [r7, #52]
	cmp	r3, #0
	beq	.L241
.L230:
	cmp	r3, #1
	beq	.L242
	cmp	r3, #2
	addne	r5, r4, #16
	bne	.L232
	mov	r1, #32
	ldr	r0, [r6, #2308]
	bl	BsGet
	add	r5, r4, #16
	cmp	r5, r8
	strb	r0, [r4, #4]
	mov	r2, r0, lsr #24
	mov	r3, r0, lsr #16
	strb	r2, [r4, #1]
	mov	r0, r0, lsr #8
	strb	r3, [r4, #2]
	strb	r0, [r4, #3]
	mov	r4, r5
	bne	.L243
.L237:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L242:
	mov	r1, #16
	ldr	r0, [r6, #2308]
	bl	BsGet
	add	r5, r4, #16
	mov	r3, r0, lsr #8
	strb	r0, [r4, #2]
	strb	r3, [r4, #1]
	b	.L232
.L236:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_Sei_Decoded_Picture_Hash, .-HEVC_Sei_Decoded_Picture_Hash
	.align	2
	.global	HEVC_Sei_Region_Refresh_Info
	.type	HEVC_Sei_Region_Refresh_Info, %function
HEVC_Sei_Region_Refresh_Info:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r5, r1
	moveq	r4, #1
	movne	r4, #0
	beq	.L246
	ldr	r0, [r0, #2308]
	mov	r1, #1
	bl	BsGet
	mov	r3, r4
	strb	r0, [r5]
.L245:
	mov	r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L246:
	mvn	r3, #0
	b	.L245
	UNWIND(.fnend)
	.size	HEVC_Sei_Region_Refresh_Info, .-HEVC_Sei_Region_Refresh_Info
	.align	2
	.global	HEVC_Sei_Mastering_Display_Colour_Volume
	.type	HEVC_Sei_Mastering_Display_Colour_Volume, %function
HEVC_Sei_Mastering_Display_Colour_Volume:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r5, r0
	mov	r6, r1
	beq	.L250
	add	r7, r1, #12
	mov	r4, r1
	mov	r3, #1
	strb	r3, [r1]
.L249:
	mov	r1, #16
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #4]!
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r4, r7
	str	r0, [r4, #12]
	bne	.L249
	mov	r1, #16
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r6, #28]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r6, #32]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r6, #36]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r3, #0
	str	r0, [r6, #40]
.L248:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L250:
	mvn	r3, #0
	b	.L248
	UNWIND(.fnend)
	.size	HEVC_Sei_Mastering_Display_Colour_Volume, .-HEVC_Sei_Mastering_Display_Colour_Volume
	.align	2
	.global	HEVC_Sei_Content_Light_Level_Information
	.type	HEVC_Sei_Content_Light_Level_Information, %function
HEVC_Sei_Content_Light_Level_Information:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r6, r0
	mov	r5, r1
	moveq	r4, #1
	movne	r4, #0
	beq	.L254
	mov	r3, #1
	mov	r1, #16
	strb	r3, [r5]
	ldr	r0, [r0, #2308]
	bl	BsGet
	mov	r1, #16
	strh	r0, [r5, #2]	@ movhi
	ldr	r0, [r6, #2308]
	bl	BsGet
	mov	r3, r4
	strh	r0, [r5, #4]	@ movhi
.L253:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L254:
	mvn	r3, #0
	b	.L253
	UNWIND(.fnend)
	.size	HEVC_Sei_Content_Light_Level_Information, .-HEVC_Sei_Content_Light_Level_Information
	.align	2
	.global	HEVC_Sei_HLGHDR_Comapitibility_Info
	.type	HEVC_Sei_HLGHDR_Comapitibility_Info, %function
HEVC_Sei_HLGHDR_Comapitibility_Info:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r5, r1
	moveq	r4, #1
	movne	r4, #0
	beq	.L257
	ldr	r0, [r0, #2308]
	mov	r1, #8
	bl	BsGet
	mov	r3, r4
	strb	r0, [r5]
.L256:
	mov	r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L257:
	mvn	r3, #0
	b	.L256
	UNWIND(.fnend)
	.size	HEVC_Sei_HLGHDR_Comapitibility_Info, .-HEVC_Sei_HLGHDR_Comapitibility_Info
	.align	2
	.global	HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	.type	HEVC_Sei_Temporal_Sub_Layer_Zero_Index, %function
HEVC_Sei_Temporal_Sub_Layer_Zero_Index:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r5, r0
	mov	r6, r1
	moveq	r4, #1
	movne	r4, #0
	beq	.L260
	mov	r1, #1
	ldr	r0, [r0, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r6]
	ldr	r0, [r5, #2308]
	bl	BsGet
	str	r0, [r6, #4]
	ldr	r0, [r5, #2308]
	bl	BsToNextByte
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L260:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_Sei_Temporal_Sub_Layer_Zero_Index, .-HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	.align	2
	.global	HEVC_Sei_Display_Orientation
	.type	HEVC_Sei_Display_Orientation, %function
HEVC_Sei_Display_Orientation:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r4, r0
	mov	r5, r1
	beq	.L264
	mov	r1, #1
	ldr	r0, [r0, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #2]
	cmp	r0, #0
	beq	.L265
.L263:
	ldr	r0, [r4, #2308]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L265:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r5, #1]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #4]
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r5, #3]
	b	.L263
.L264:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_Sei_Display_Orientation, .-HEVC_Sei_Display_Orientation
	.align	2
	.global	HEVC_Sei_ParsePts_Init
	.type	HEVC_Sei_ParsePts_Init, %function
HEVC_Sei_ParsePts_Init:
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
	ldr	r3, .L267
	mov	r4, r0
	add	r0, r0, #27136
	mov	r2, #80
	mov	r1, #0
	add	r0, r0, #192
	ldr	r3, [r3, #48]
	mov	r5, #0
	blx	r3
	add	r3, r4, #4544
	add	r0, r4, #4096
	add	r1, r3, #2096
	add	r3, r3, #2080
	mov	r4, #0
	mov	r2, #0
	strd	r4, [r1]
	strd	r4, [r3, #-8]
	strd	r4, [r3]
	strd	r4, [r1, #-8]
	str	r2, [r0, #428]
	str	r2, [r0, #432]
	str	r2, [r0, #420]
	str	r2, [r0, #424]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L268:
	.align	2
.L267:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_Sei_ParsePts_Init, .-HEVC_Sei_ParsePts_Init
	.align	2
	.global	HEVC_Sei_Get_Dpb_Output_Time
	.type	HEVC_Sei_Get_Dpb_Output_Time, %function
HEVC_Sei_Get_Dpb_Output_Time:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	add	r3, r0, #53248
	ldr	r5, [r0, #2320]
	movw	r6, #2024
	ldr	ip, [r3, #1092]
	add	r3, r0, #24576
	ldr	lr, [r1, #12]
	ldr	r4, [r0, #2316]
	mla	ip, r6, ip, r5
	movw	r5, #26164
	ldr	r6, [r3, #2756]
	cmp	r6, #1
	ldr	ip, [ip, #32]
	str	lr, [r3, #2776]
	ldr	lr, [r1, #16]
	mla	r1, r5, ip, r4
	str	lr, [r3, #2780]
	ldr	lr, [r2, #76]
	str	lr, [r3, #2800]
	ldr	ip, [r2, #80]
	str	ip, [r3, #2804]
	ldr	ip, [r1, #56]
	add	r1, r1, ip, lsl #2
	ldrb	r7, [r1, #412]	@ zero_extendqisi2
	beq	.L277
	ldrb	r1, [r3, #2825]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L278
	add	ip, r0, #4544
	add	r6, r0, #4096
	add	r1, ip, #2080
	add	r4, ip, #2096
	ldr	r5, [r6, #452]
	ldrd	r0, [r1, #-8]
	adds	r0, r0, r5
	adc	r1, r1, #0
	strd	r0, [r4]
.L271:
	add	ip, ip, #2096
	mov	r4, #0
	strd	r0, [ip, #-8]
	strb	r4, [r3, #2825]
	ldrb	r2, [r2, #9]	@ zero_extendqisi2
	cmp	r2, r4
	ldr	r2, [r6, #456]
	bne	.L279
	adds	r0, r0, r2
	add	r3, r3, #2752
	adc	r1, r1, #0
	subs	r0, r0, r7
	sbc	r1, r1, #0
	strd	r0, [r3, #40]
	umull	r4, r5, r0, lr
	mla	r5, lr, r1, r5
	strd	r4, [r3, #32]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L278:
	ldrb	r1, [r0, #3473]	@ zero_extendqisi2
	add	ip, r0, #4544
	cmp	r1, #0
	add	r1, ip, #2080
	bne	.L280
	ldrd	r4, [r1, #-8]
	strd	r4, [fp, #-52]
	ldrd	r4, [r1]
	strd	r4, [fp, #-44]
.L274:
	add	r6, r0, #4096
	mov	r5, #0
	add	r9, ip, #2096
	add	r8, ip, #2080
	ldr	r1, [r6, #452]
	mov	r0, r1
	mov	r1, r5
	ldrd	r4, [fp, #-52]
	strd	r0, [fp, #-60]
	adds	r4, r4, r0
	adc	r5, r5, r1
	mov	r0, r4
	mov	r1, r5
	ldrd	r4, [fp, #-44]
	strd	r0, [r9]
	cmp	r0, r4
	strd	r0, [r8, #-8]
	sbcs	r4, r1, r5
	bge	.L271
	ldrd	r0, [fp, #-60]
	ldrd	r4, [fp, #-44]
	adds	r0, r0, r4
	adc	r1, r1, r5
	strd	r0, [r9]
	strd	r0, [r8, #-8]
	b	.L271
.L279:
	adds	r0, r0, r2
	add	ip, r3, #2752
	adc	r1, r1, #0
	movs	r1, r1, asr #1
	mov	r0, r0, rrx
	subs	r0, r0, r7
	sbc	r1, r1, #0
	strd	r0, [ip, #40]
	umull	r2, r3, r0, lr
	mla	r3, lr, r1, r3
	strd	r2, [ip, #32]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L277:
	add	r6, r0, #4096
	add	ip, r0, #4544
	add	r5, ip, #2096
	add	r4, ip, #2080
	ldr	r0, [r6, #420]
	mov	r1, r0, asr #31
	strd	r0, [r5]
	strd	r0, [r4, #-8]
	b	.L271
.L280:
	ldrd	r4, [r1]
	strd	r4, [fp, #-52]
	strd	r4, [fp, #-44]
	b	.L274
	UNWIND(.fnend)
	.size	HEVC_Sei_Get_Dpb_Output_Time, .-HEVC_Sei_Get_Dpb_Output_Time
	.align	2
	.global	HEVC_Get_Last_IDR_Frame_Info
	.type	HEVC_Get_Last_IDR_Frame_Info, %function
HEVC_Get_Last_IDR_Frame_Info:
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
	sub	r3, r1, #19
	cmp	r3, #1
	bls	.L285
.L282:
	sub	r1, r1, #6
	cmp	r1, #3
	ldmlsfd	sp, {r4, r5, fp, sp, pc}
	add	r0, r0, #4544
	add	r3, r0, #2096
	add	r0, r0, #2080
	ldrd	r2, [r3]
	strd	r2, [r0]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L285:
	ldr	ip, [r0, #2324]
	mvn	r3, #0
	mvn	r2, #0
	ldrd	r4, [ip, #56]
	cmp	r5, r3
	cmpeq	r4, r2
	beq	.L283
	add	r3, r0, #27136
	add	ip, r0, #27392
	add	r3, r3, #240
	strd	r4, [ip, #-8]
	ldrd	r2, [r3, #-8]
	strd	r2, [ip]
.L283:
	add	r3, r0, #24576
	mov	r2, #1
	strb	r2, [r3, #2824]
	b	.L282
	UNWIND(.fnend)
	.size	HEVC_Get_Last_IDR_Frame_Info, .-HEVC_Get_Last_IDR_Frame_Info
	.global	__aeabi_uidivmod
	.align	2
	.global	HEVC_Sei_Get_Frame_Pts
	.type	HEVC_Sei_Get_Frame_Pts, %function
HEVC_Sei_Get_Frame_Pts:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r1, r0, #24576
	add	r3, r0, #27136
	add	r2, r0, #27392
	ldr	r5, [r1, #2800]
	mov	r4, r0
	ldr	r10, [r1, #2804]
	ldrd	r6, [r3, #232]
	cmp	r5, #0
	ldrd	r0, [r2, #-8]
	ldrd	r2, [r2]
	strd	r0, [fp, #-52]
	strd	r2, [fp, #-60]
	beq	.L289
	mov	r9, r5
	mov	r0, r10
	b	.L288
.L290:
	mov	r9, r1
.L288:
	mov	r1, r9
	bl	__aeabi_uidivmod
	mov	r0, r9
	cmp	r1, #0
	bne	.L290
.L287:
	ldrd	r2, [fp, #-60]
	mov	r1, r9
	mov	r0, r5
	mov	r8, #1000
	subs	r2, r6, r2
	movw	r5, #54240
	mul	r6, r8, r2
	bl	__aeabi_uidiv
	mov	r1, r9
	mul	r6, r0, r6
	mov	r0, r10
	bl	__aeabi_uidiv
	mov	r1, r0
	mov	r0, r6
	bl	__aeabi_uidiv
	ldrd	r2, [fp, #-52]
	adds	r2, r2, r0
	adc	r3, r3, #0
	mov	r0, r2
	mov	r1, r3
	strd	r0, [r4, r5]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L289:
	mov	r9, r10
	b	.L287
	UNWIND(.fnend)
	.size	HEVC_Sei_Get_Frame_Pts, .-HEVC_Sei_Get_Frame_Pts
	.align	2
	.global	HEVC_Sei_User_Data_Unregistered
	.type	HEVC_Sei_User_Data_Unregistered, %function
HEVC_Sei_User_Data_Unregistered:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	r6, .L323
	mov	r5, r1
	mov	r8, r2
	mov	r7, r0
	str	r2, [fp, #-104]
	ldmia	r6!, {r0, r1, r2, r3}
	sub	r4, fp, #100
	mov	lr, #1
	strb	lr, [r5, #24]
	sub	ip, fp, #72
	cmp	r8, #15
	add	lr, r6, #12
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r6, {r0, r1, r2}
	stmia	r4, {r0, r1, r2}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	bls	.L309
	add	r4, r6, #39
	add	r9, r5, #3
	add	r6, r6, #55
	sub	r10, r8, #1
	mov	r8, #0
.L294:
	mov	r1, #8
	ldr	r0, [r7, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r9, #1]!
	ldrb	r1, [r4, #1]!	@ zero_extendqisi2
	cmp	r1, r0
	strne	r10, [r5, #20]
	strneb	r8, [r5, #24]
	cmp	r4, r6
	sub	r10, r10, #1
	bne	.L294
	ldrb	r4, [r5, #24]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L319
	ldr	r3, [fp, #-104]
	sub	r9, r3, #16
	str	r9, [r5, #20]
	cmp	r9, #0
	beq	.L304
	mov	r4, #0
.L308:
	mov	r1, #8
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldr	r3, [r5, #20]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L308
.L307:
	mov	r0, #0
.L292:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L304:
	mov	r0, #0
	str	r0, [r5]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L319:
	mov	r1, #5
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #3
	uxtb	r6, r0
	ldr	r0, [r7, #2308]
	bl	BsGet
	uxtb	r0, r0
	cmp	r6, #6
	cmpeq	r0, #3
	beq	.L320
	sub	r3, r6, #2
	cmp	r0, #0
	cmpeq	r6, #2
	clz	r3, r3
	mov	r3, r3, lsr #5
	beq	.L321
	cmp	r0, #1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r3, #0
	bne	.L322
	ldr	r3, [fp, #-104]
	sub	r9, r3, #18
	str	r9, [r5, #20]
.L299:
	cmp	r9, #0
	movne	r4, #0
	beq	.L304
.L306:
	mov	r1, #8
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldr	r3, [r5, #20]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L306
	b	.L307
.L322:
	mov	r1, #8
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #8
	mov	r4, r0
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldr	r3, [fp, #-104]
	uxtb	r4, r4
	sub	r9, r3, #19
	str	r9, [r5, #20]
	uxtb	r0, r0
	add	r4, r4, r0, lsl #8
	str	r4, [r5, #64]
	b	.L299
.L309:
	mvn	r0, #0
	b	.L292
.L320:
	mov	r1, #2
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #31]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #30]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #29]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #28]
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldrb	r3, [r5, #29]	@ zero_extendqisi2
	mov	r2, #28
	add	r0, r5, #36
	cmp	r3, #0
	ldr	r3, .L323+4
	subeq	r1, fp, #100
	subne	r1, fp, #72
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, #8
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r3, #50
	mov	r1, #8
	uxtb	r0, r0
	mul	r3, r3, r0
	str	r3, [r5, #32]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #8
	mov	r4, r0
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldr	r3, [fp, #-104]
	uxtb	r4, r4
	sub	r9, r3, #21
	str	r9, [r5, #20]
	uxtb	r0, r0
	add	r4, r4, r0, lsl #8
	str	r4, [r5, #64]
	b	.L299
.L321:
	mov	r1, #2
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #31]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #30]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #29]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #28]
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldrb	r3, [r5, #29]	@ zero_extendqisi2
	mov	r2, #28
	add	r0, r5, #36
	cmp	r3, #0
	ldr	r3, .L323+4
	subeq	r1, fp, #100
	subne	r1, fp, #72
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, #8
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r3, #50
	ldr	r2, [fp, #-104]
	sub	r9, r2, #19
	str	r9, [r5, #20]
	uxtb	r0, r0
	mul	r3, r3, r0
	str	r3, [r5, #32]
	b	.L299
.L324:
	.align	2
.L323:
	.word	.LANCHOR1+156
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_Sei_User_Data_Unregistered, .-HEVC_Sei_User_Data_Unregistered
	.align	2
	.global	HEVC_IsRefListWrong
	.type	HEVC_IsRefListWrong, %function
HEVC_IsRefListWrong:
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
	add	r7, r0, #53248
	mov	r5, r0
	ldr	r3, [r7, #1176]
	cmp	r3, #0
	beq	.L326
	ldr	r4, [r0, #2336]
	cmp	r4, #0
	beq	.L327
	ldr	r2, [r4, #32]
	cmp	r2, #0
	addne	r6, r0, #2336
	movne	r4, #0
	bne	.L332
	b	.L370
.L333:
	ldr	r3, [ip, #640]
	cmp	r3, #0
	beq	.L334
	ldr	r3, [r7, #1176]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L371
	ldr	r2, [r6, #4]!
	cmp	r2, #0
	beq	.L327
	ldr	r2, [r2, #32]
	cmp	r2, #0
	beq	.L328
.L332:
	ldr	r1, [r2, #32]
	ldr	r0, [r5, #244]
	bl	FSP_GetLogicFs
	subs	ip, r0, #0
	bne	.L333
.L334:
	add	r3, r4, #584
	mov	r2, r4
	ldr	r1, .L375
	mov	r0, #1
	ldr	r3, [r5, r3, asl #2]
	ldr	r3, [r3, #32]
	str	ip, [sp]
	bl	dprint_vfmw
.L331:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L371:
	cmp	r3, #0
	beq	.L326
	ldr	r2, [r7, #1180]
	cmp	r2, #0
	beq	.L326
	ldr	r4, [r5, #2404]
	cmp	r4, #0
	beq	.L339
	ldr	r2, [r4, #32]
	cmp	r2, #0
	addne	r6, r5, #2400
	movne	r4, #0
	addne	r6, r6, #4
	bne	.L343
	b	.L372
.L344:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L345
	ldr	r3, [r7, #1180]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L326
	ldr	r3, [r6, #4]!
	cmp	r3, #0
	beq	.L373
	ldr	r2, [r3, #32]
	cmp	r2, #0
	beq	.L374
.L343:
	ldr	r1, [r2, #32]
	ldr	r0, [r5, #244]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L344
.L345:
	mov	r3, r0
	mov	r2, r4
	ldr	r1, .L375+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L365
.L326:
	mov	r0, #0
.L365:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L327:
	mov	r2, r4
	ldr	r1, .L375+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L331
.L370:
	mov	r4, r2
.L328:
	mov	r2, r4
	ldr	r1, .L375+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L331
.L373:
	ldr	r3, [r7, #1176]
.L339:
	mov	r2, r4
	ldr	r1, .L375+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L365
.L374:
	ldr	r3, [r7, #1176]
.L340:
	mov	r2, r4
	ldr	r1, .L375+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L365
.L372:
	mov	r4, r2
	b	.L340
.L376:
	.align	2
.L375:
	.word	.LC49
	.word	.LC52
	.word	.LC47
	.word	.LC48
	.word	.LC50
	.word	.LC51
	UNWIND(.fnend)
	.size	HEVC_IsRefListWrong, .-HEVC_IsRefListWrong
	.align	2
	.global	HEVC_GetUnRefPicWithMinPoc
	.type	HEVC_GetUnRefPicWithMinPoc, %function
HEVC_GetUnRefPicWithMinPoc:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	mvn	r4, #0
	str	r4, [r1]
	ldr	r3, [r0, #2508]
	cmp	r3, #16
	bhi	.L378
	cmp	r3, #0
	addne	ip, r0, #2512
	mvnne	r4, #-2147483648
	addne	ip, ip, #12
	movne	r3, #0
	beq	.L380
.L383:
	ldr	r2, [ip, #4]!
	ldr	lr, [r2, #12]
	cmp	lr, r4
	bge	.L382
	ldr	r2, [r2]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	streq	r3, [r1]
	moveq	r4, lr
.L382:
	ldr	r2, [r0, #2508]
	add	r3, r3, #1
	cmp	r2, r3
	bhi	.L383
	ldr	r3, [r1]
	cmn	r3, #1
	beq	.L380
	str	r4, [sp]
	mov	r0, #1
	ldr	r2, .L386
	ldr	r1, .L386+4
	bl	dprint_vfmw
	mov	r0, #0
.L381:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L378:
	ldr	r2, .L386
	mov	r0, #1
	ldr	r1, .L386+8
	bl	dprint_vfmw
	mov	r0, r4
	b	.L381
.L380:
	ldr	r2, .L386
	mov	r0, #1
	ldr	r1, .L386+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L381
.L387:
	.align	2
.L386:
	.word	.LANCHOR1+228
	.word	.LC55
	.word	.LC53
	.word	.LC54
	UNWIND(.fnend)
	.size	HEVC_GetUnRefPicWithMinPoc, .-HEVC_GetUnRefPicWithMinPoc
	.align	2
	.global	HEVC_IncreaseDPBSize
	.type	HEVC_IncreaseDPBSize, %function
HEVC_IncreaseDPBSize:
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
	ldr	r3, [r0, #2504]
	mov	r4, r0
	cmp	r3, #16
	bhi	.L395
	ldr	r0, [r0, #244]
	bl	FSP_GetTotalValidFsNum
	add	r2, r4, #53248
	ldr	r1, [r2, #352]
	cmp	r1, #1920
	movhi	r2, #1
	bls	.L396
.L391:
	rsb	r2, r2, r0
	bic	r2, r2, r2, asr #31
	cmp	r2, #0
	ble	.L392
	ldr	r3, [r4, #2504]
	add	r3, r3, #1
	cmp	r2, r3
	bcc	.L392
	str	r3, [r4, #2504]
	mov	r0, #13
	ldr	r2, .L397
	ldr	r1, .L397+4
	bl	dprint_vfmw
	mov	r0, #0
.L390:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L396:
	ldr	r2, [r2, #356]
	cmp	r2, #1088
	movhi	r2, #1
	movls	r2, #4
	b	.L391
.L392:
	mov	r3, r2
	ldr	r1, .L397+8
	ldr	r2, .L397
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L390
.L395:
	mov	r5, #17
	ldr	r2, .L397
	mov	r0, #1
	str	r5, [sp]
	ldr	r1, .L397+12
	bl	dprint_vfmw
	str	r5, [r4, #2504]
	mvn	r0, #0
	b	.L390
.L398:
	.align	2
.L397:
	.word	.LANCHOR1+256
	.word	.LC57
	.word	.LC58
	.word	.LC56
	UNWIND(.fnend)
	.size	HEVC_IncreaseDPBSize, .-HEVC_IncreaseDPBSize
	.align	2
	.global	HEVC_PicTypeStatistic
	.type	HEVC_PicTypeStatistic, %function
HEVC_PicTypeStatistic:
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
	add	r0, r0, #53248
	ldr	r3, [r0, #1088]
	sub	r2, r3, #16
	cmp	r2, #5
	bls	.L400
	ldr	r3, [r0, #340]
	sub	r3, r3, #16
	cmp	r3, #5
	bls	.L411
	ldr	r3, [r0, #1076]
	cmp	r3, #1
	beq	.L403
	cmp	r3, #2
	beq	.L404
	cmp	r3, #0
	moveq	r3, #2
	streq	r3, [r0, #348]
	ldmfd	sp, {fp, sp, pc}
.L400:
	str	r3, [r0, #340]
	mov	r3, #0
	str	r3, [r0, #348]
	ldmfd	sp, {fp, sp, pc}
.L403:
	ldr	r2, [r0, #348]
	cmp	r2, #2
	beq	.L412
	str	r3, [r0, #348]
	ldmfd	sp, {fp, sp, pc}
.L404:
	ldr	r3, [r0, #348]
	sub	r3, r3, #1
	cmp	r3, #1
	movhi	r3, #0
	strhi	r3, [r0, #348]
	ldmfd	sp, {fp, sp, pc}
.L412:
	ldmfd	sp, {fp, sp, pc}
.L411:
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_PicTypeStatistic, .-HEVC_PicTypeStatistic
	.align	2
	.global	HEVC_IsReferenceNalu
	.type	HEVC_IsReferenceNalu, %function
HEVC_IsReferenceNalu:
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
	add	r0, r0, #53248
	ldr	r0, [r0, #1088]
	cmp	r0, #15
	bhi	.L414
	tst	r0, #1
	bne	.L416
.L414:
	sub	r0, r0, #16
	cmp	r0, #7
	movhi	r0, #0
	movls	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L416:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsReferenceNalu, .-HEVC_IsReferenceNalu
	.align	2
	.global	HEVC_WriteQmatrix_4x4
	.type	HEVC_WriteQmatrix_4x4, %function
HEVC_WriteQmatrix_4x4:
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
	add	r1, r1, r0, lsl #6
	ldr	r0, [r1, #588]
	ldr	r3, [r1, #592]
	ldrb	ip, [r1, #580]	@ zero_extendqisi2
	ldrb	lr, [r1, #584]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2]
	ldr	r0, [r1, #588]
	ldrb	lr, [r1, #581]	@ zero_extendqisi2
	ldrb	r3, [r1, #593]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #585]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #4]
	ldr	r0, [r1, #584]
	ldrb	lr, [r1, #582]	@ zero_extendqisi2
	ldrb	r3, [r1, #594]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #590]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #8]
	ldr	r0, [r1, #592]
	ldrb	r3, [r1, #587]	@ zero_extendqisi2
	ldrb	lr, [r1, #583]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #591]	@ zero_extendqisi2
	orr	r1, lr, r0
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #12]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_WriteQmatrix_4x4, .-HEVC_WriteQmatrix_4x4
	.align	2
	.global	HEVC_WriteQmatrix_32x32
	.type	HEVC_WriteQmatrix_32x32, %function
HEVC_WriteQmatrix_32x32:
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
	add	r1, r1, r0, lsl #6
	ldr	r0, [r1, #1740]
	ldr	r3, [r1, #1756]
	ldrb	ip, [r1, #1732]	@ zero_extendqisi2
	ldrb	lr, [r1, #1748]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2]
	ldr	r0, [r1, #1772]
	ldr	r3, [r1, #1788]
	ldrb	ip, [r1, #1764]	@ zero_extendqisi2
	ldrb	lr, [r1, #1780]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #4]
	ldr	r0, [r1, #1740]
	ldrb	lr, [r1, #1733]	@ zero_extendqisi2
	ldrb	r3, [r1, #1757]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1749]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #8]
	ldr	r0, [r1, #1772]
	ldrb	lr, [r1, #1765]	@ zero_extendqisi2
	ldrb	r3, [r1, #1789]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1781]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #12]
	ldr	r0, [r1, #1748]
	ldrb	lr, [r1, #1734]	@ zero_extendqisi2
	ldrb	r3, [r1, #1758]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #1742]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #16]
	ldr	r0, [r1, #1780]
	ldrb	lr, [r1, #1766]	@ zero_extendqisi2
	ldrb	r3, [r1, #1790]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #1774]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #20]
	ldr	r0, [r1, #1756]
	ldrb	lr, [r1, #1735]	@ zero_extendqisi2
	ldrb	r3, [r1, #1751]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1743]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #24]
	ldr	r0, [r1, #1788]
	ldrb	lr, [r1, #1767]	@ zero_extendqisi2
	ldrb	r3, [r1, #1783]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1775]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #28]
	ldr	r0, [r1, #1744]
	ldr	r3, [r1, #1760]
	ldrb	ip, [r1, #1736]	@ zero_extendqisi2
	ldrb	lr, [r1, #1752]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #32]
	ldr	r0, [r1, #1776]
	ldr	r3, [r1, #1792]
	ldrb	ip, [r1, #1768]	@ zero_extendqisi2
	ldrb	lr, [r1, #1784]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #36]
	ldr	r0, [r1, #1744]
	ldrb	lr, [r1, #1737]	@ zero_extendqisi2
	ldrb	r3, [r1, #1761]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1753]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #40]
	ldr	r0, [r1, #1776]
	ldrb	lr, [r1, #1769]	@ zero_extendqisi2
	ldrb	r3, [r1, #1793]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1785]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #44]
	ldr	r0, [r1, #1752]
	ldrb	lr, [r1, #1738]	@ zero_extendqisi2
	ldrb	r3, [r1, #1762]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #1746]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #48]
	ldrb	lr, [r1, #1770]	@ zero_extendqisi2
	ldr	r0, [r1, #1784]
	ldrb	r3, [r1, #1794]	@ zero_extendqisi2
	ldrb	ip, [r1, #1778]	@ zero_extendqisi2
	and	r0, r0, #16711680
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #52]
	ldr	r0, [r1, #1760]
	ldrb	lr, [r1, #1739]	@ zero_extendqisi2
	ldrb	r3, [r1, #1755]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1747]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #56]
	ldr	r0, [r1, #1792]
	ldrb	r3, [r1, #1787]	@ zero_extendqisi2
	ldrb	lr, [r1, #1771]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1779]	@ zero_extendqisi2
	orr	r1, lr, r0
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #60]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_WriteQmatrix_32x32, .-HEVC_WriteQmatrix_32x32
	.align	2
	.global	HEVC_WriteQmatrix_8x8
	.type	HEVC_WriteQmatrix_8x8, %function
HEVC_WriteQmatrix_8x8:
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
	add	r1, r1, r0, lsl #6
	ldr	r0, [r1, #980]
	ldr	r3, [r1, #1012]
	ldrb	ip, [r1, #964]	@ zero_extendqisi2
	ldrb	lr, [r1, #996]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2]
	ldr	r0, [r1, #988]
	ldr	r3, [r1, #1020]
	ldrb	ip, [r1, #972]	@ zero_extendqisi2
	ldrb	lr, [r1, #1004]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #4]
	ldr	r0, [r1, #980]
	ldrb	lr, [r1, #965]	@ zero_extendqisi2
	ldrb	r3, [r1, #1013]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #997]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #8]
	ldr	r0, [r1, #988]
	ldrb	lr, [r1, #973]	@ zero_extendqisi2
	ldrb	r3, [r1, #1021]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1005]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #12]
	ldr	r0, [r1, #996]
	ldrb	lr, [r1, #966]	@ zero_extendqisi2
	ldrb	r3, [r1, #1014]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #982]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #16]
	ldr	r0, [r1, #1004]
	ldrb	lr, [r1, #974]	@ zero_extendqisi2
	ldrb	r3, [r1, #1022]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #990]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #20]
	ldr	r0, [r1, #1012]
	ldrb	lr, [r1, #967]	@ zero_extendqisi2
	ldrb	r3, [r1, #999]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #983]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #24]
	ldr	r0, [r1, #1020]
	ldrb	lr, [r1, #975]	@ zero_extendqisi2
	ldrb	r3, [r1, #1007]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #991]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #28]
	ldr	r0, [r1, #984]
	ldr	r3, [r1, #1016]
	ldrb	ip, [r1, #968]	@ zero_extendqisi2
	ldrb	lr, [r1, #1000]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #32]
	ldr	r0, [r1, #992]
	ldr	r3, [r1, #1024]
	ldrb	ip, [r1, #976]	@ zero_extendqisi2
	ldrb	lr, [r1, #1008]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #36]
	ldr	r0, [r1, #984]
	ldrb	lr, [r1, #969]	@ zero_extendqisi2
	ldrb	r3, [r1, #1017]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1001]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #40]
	ldr	r0, [r1, #992]
	ldrb	lr, [r1, #977]	@ zero_extendqisi2
	ldrb	r3, [r1, #1025]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1009]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #44]
	ldr	r0, [r1, #1000]
	ldrb	lr, [r1, #970]	@ zero_extendqisi2
	ldrb	r3, [r1, #1018]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #986]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #48]
	ldrb	lr, [r1, #978]	@ zero_extendqisi2
	ldr	r0, [r1, #1008]
	ldrb	r3, [r1, #1026]	@ zero_extendqisi2
	ldrb	ip, [r1, #994]	@ zero_extendqisi2
	and	r0, r0, #16711680
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #52]
	ldr	r0, [r1, #1016]
	ldrb	lr, [r1, #971]	@ zero_extendqisi2
	ldrb	r3, [r1, #1003]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #987]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #56]
	ldr	r0, [r1, #1024]
	ldrb	r3, [r1, #1011]	@ zero_extendqisi2
	ldrb	lr, [r1, #979]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #995]	@ zero_extendqisi2
	orr	r1, lr, r0
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #60]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_WriteQmatrix_8x8, .-HEVC_WriteQmatrix_8x8
	.align	2
	.global	HEVC_WriteQmatrix_16x16
	.type	HEVC_WriteQmatrix_16x16, %function
HEVC_WriteQmatrix_16x16:
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
	add	r1, r1, r0, lsl #6
	ldr	r0, [r1, #1356]
	ldr	r3, [r1, #1372]
	ldrb	ip, [r1, #1348]	@ zero_extendqisi2
	ldrb	lr, [r1, #1364]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2]
	ldr	r0, [r1, #1388]
	ldr	r3, [r1, #1404]
	ldrb	ip, [r1, #1380]	@ zero_extendqisi2
	ldrb	lr, [r1, #1396]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #4]
	ldr	r0, [r1, #1356]
	ldrb	lr, [r1, #1349]	@ zero_extendqisi2
	ldrb	r3, [r1, #1373]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1365]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #8]
	ldr	r0, [r1, #1388]
	ldrb	lr, [r1, #1381]	@ zero_extendqisi2
	ldrb	r3, [r1, #1405]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1397]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #12]
	ldr	r0, [r1, #1364]
	ldrb	lr, [r1, #1350]	@ zero_extendqisi2
	ldrb	r3, [r1, #1374]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #1358]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #16]
	ldr	r0, [r1, #1396]
	ldrb	lr, [r1, #1382]	@ zero_extendqisi2
	ldrb	r3, [r1, #1406]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #1390]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #20]
	ldr	r0, [r1, #1372]
	ldrb	lr, [r1, #1351]	@ zero_extendqisi2
	ldrb	r3, [r1, #1367]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1359]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #24]
	ldr	r0, [r1, #1404]
	ldrb	lr, [r1, #1383]	@ zero_extendqisi2
	ldrb	r3, [r1, #1399]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1391]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #28]
	ldr	r0, [r1, #1360]
	ldr	r3, [r1, #1376]
	ldrb	ip, [r1, #1352]	@ zero_extendqisi2
	ldrb	lr, [r1, #1368]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #32]
	ldr	r0, [r1, #1392]
	ldr	r3, [r1, #1408]
	ldrb	ip, [r1, #1384]	@ zero_extendqisi2
	ldrb	lr, [r1, #1400]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2, #36]
	ldr	r0, [r1, #1360]
	ldrb	lr, [r1, #1353]	@ zero_extendqisi2
	ldrb	r3, [r1, #1377]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1369]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #40]
	ldr	r0, [r1, #1392]
	ldrb	lr, [r1, #1385]	@ zero_extendqisi2
	ldrb	r3, [r1, #1409]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #1401]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #44]
	ldr	r0, [r1, #1368]
	ldrb	lr, [r1, #1354]	@ zero_extendqisi2
	ldrb	r3, [r1, #1378]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #1362]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #48]
	ldrb	lr, [r1, #1386]	@ zero_extendqisi2
	ldr	r0, [r1, #1400]
	ldrb	r3, [r1, #1410]	@ zero_extendqisi2
	ldrb	ip, [r1, #1394]	@ zero_extendqisi2
	and	r0, r0, #16711680
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #52]
	ldr	r0, [r1, #1376]
	ldrb	lr, [r1, #1355]	@ zero_extendqisi2
	ldrb	r3, [r1, #1371]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1363]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #56]
	ldr	r0, [r1, #1408]
	ldrb	r3, [r1, #1403]	@ zero_extendqisi2
	ldrb	lr, [r1, #1387]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #1395]	@ zero_extendqisi2
	orr	r1, lr, r0
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #60]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_WriteQmatrix_16x16, .-HEVC_WriteQmatrix_16x16
	.global	__aeabi_idivmod
	.align	2
	.global	HEVC_CtbRsAddrToTsAddr
	.type	HEVC_CtbRsAddrToTsAddr, %function
HEVC_CtbRsAddrToTsAddr:
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
	UNWIND(.pad #204)
	sub	sp, sp, #204
	add	r0, r0, #24576
	mov	r6, r1
	ldr	ip, [r0, #2840]
	ldrb	r3, [ip, #13]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L456
	ldr	r3, [ip, #72]
	ldr	r7, [ip, #68]
	ldrb	r4, [ip, #15]	@ zero_extendqisi2
	mov	r2, r3
	str	r3, [fp, #-236]
	ldr	r3, [r0, #2836]
	cmp	r2, #0
	sub	r8, r2, #1
	sub	r2, r7, #1
	str	r2, [fp, #-240]
	ldr	r5, [r3, #72]
	ldr	r1, [r3, #148]
	ble	.L427
	ldr	r3, [fp, #-236]
	add	r2, ip, #214
	sub	r0, fp, #216
	add	lr, r3, #107
	add	lr, ip, lr, lsl #1
.L426:
	ldrsh	r3, [r2, #2]!
	cmp	r2, lr
	sub	r3, r3, #1
	str	r3, [r0, #4]!
	bne	.L426
.L427:
	cmp	r7, #0
	ble	.L425
	add	r3, r7, #87
	add	r2, ip, #174
	sub	r0, fp, #128
	add	ip, ip, r3, lsl #1
.L430:
	ldrsh	r3, [r2, #2]!
	cmp	r2, ip
	sub	r3, r3, #1
	str	r3, [r0, #4]!
	bne	.L430
.L425:
	mov	r10, #1
	mov	r0, r5
	mov	r10, r10, asl r1
	mov	r1, r10
	bl	__aeabi_idiv
	mov	r1, r10
	mov	r9, r0
	mov	r0, r5
	bl	__aeabi_idivmod
	mov	r0, r6
	cmp	r1, #0
	addne	r9, r9, #1
	mov	r1, r9
	bl	__aeabi_idivmod
	mov	r0, r6
	mov	r5, r1
	mov	r1, r9
	bl	__aeabi_idiv
	cmp	r4, #0
	beq	.L468
	cmp	r8, #0
	mov	r3, #0
	blt	.L447
	sub	r10, fp, #212
	mov	r4, r3
	mov	r6, r3
	mov	r8, r3
	str	r3, [fp, #-232]
	str	r3, [fp, #-244]
.L433:
	ldr	r2, [fp, #-240]
	cmp	r2, #0
	blt	.L450
	ldr	r8, [r10]
	mov	r2, #0
	ldr	r3, [fp, #-232]
	mov	r1, r2
	add	r8, r8, #1
	sub	lr, fp, #128
	mov	r6, r4
	add	r9, r3, r8
	b	.L436
.L448:
	mov	r6, r4
.L436:
	ldr	r3, [lr, #4]!
	add	r1, r1, #1
	add	r3, r3, #1
	add	ip, r2, r3
	cmp	r5, ip
	mla	r4, r8, r3, r4
	bge	.L434
	cmp	r0, r9
	blt	.L466
.L434:
	cmp	r1, r7
	mov	r2, ip
	bne	.L448
.L439:
	add	r2, ip, r3
	cmp	r5, r2
	bge	.L449
	cmp	r0, r9
	blt	.L469
.L449:
	ldr	r2, [fp, #-244]
	add	r10, r10, #4
	ldr	r1, [fp, #-236]
	add	r2, r2, #1
	str	r9, [fp, #-232]
	cmp	r2, r1
	str	r2, [fp, #-244]
	bne	.L433
	rsb	r0, r9, r0
	mov	r2, ip
	rsb	r2, r2, r5
	mul	r3, r0, r3
	add	r3, r2, r3
	add	r6, r3, r6
.L456:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L468:
	cmp	r8, #0
	mov	r3, r4
	blt	.L447
	sub	r10, fp, #212
	mov	lr, r4
	mov	r6, r4
	mov	r8, r4
	str	r4, [fp, #-232]
	str	r4, [fp, #-244]
.L432:
	ldr	r2, [fp, #-240]
	cmp	r2, #0
	blt	.L453
	ldr	r8, [r10]
	mov	r2, #0
	ldr	r3, [fp, #-232]
	mov	ip, r2
	add	r8, r8, #1
	sub	r4, fp, #128
	mov	r6, lr
	add	r9, r3, r8
	b	.L442
.L451:
	mov	r6, lr
.L442:
	ldr	r3, [r4, #4]!
	add	ip, ip, #1
	add	r3, r3, #1
	add	r1, r2, r3
	cmp	r5, r1
	mla	lr, r8, r3, lr
	bge	.L440
	cmp	r0, r9
	blt	.L441
.L440:
	cmp	ip, r7
	mov	r2, r1
	bne	.L451
.L445:
	add	r2, r1, r3
	cmp	r5, r2
	bge	.L465
	cmp	r0, r9
	blt	.L470
.L465:
	ldr	r2, [fp, #-244]
	add	r10, r10, #4
	ldr	ip, [fp, #-236]
	add	r2, r2, #1
	str	r9, [fp, #-232]
	cmp	r2, ip
	str	r2, [fp, #-244]
	bne	.L432
	rsb	r0, r9, r0
	mov	r2, r1
	mul	r3, r0, r3
	b	.L431
.L466:
	ldr	r1, [fp, #-232]
	rsb	r0, r1, r0
	mul	r3, r0, r3
.L431:
	rsb	r2, r2, r5
	add	r3, r2, r3
	add	r6, r3, r6
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L441:
	cmp	r1, r5
	bgt	.L466
	mov	r1, r2
	b	.L465
.L450:
	ldr	r2, [fp, #-232]
	mov	ip, #0
	add	r9, r2, r8
	b	.L439
.L453:
	ldr	r2, [fp, #-232]
	mov	r1, #0
	add	r9, r2, r8
	b	.L445
.L469:
	ldr	r2, [fp, #-232]
	rsb	r0, r2, r0
	mov	r2, ip
	mul	r3, r0, r3
	b	.L431
.L470:
	ldr	r2, [fp, #-232]
	rsb	r0, r2, r0
	mov	r2, r1
	mul	r3, r0, r3
	b	.L431
.L447:
	mov	r2, r3
	mov	r6, r3
	b	.L431
	UNWIND(.fnend)
	.size	HEVC_CtbRsAddrToTsAddr, .-HEVC_CtbRsAddrToTsAddr
	.align	2
	.global	HEVC_CtbTsAddrToRsAddr
	.type	HEVC_CtbTsAddrToRsAddr, %function
HEVC_CtbTsAddrToRsAddr:
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
	UNWIND(.pad #204)
	sub	sp, sp, #204
	add	r0, r0, #24576
	mov	r7, r1
	ldr	lr, [r0, #2840]
	ldrb	r3, [lr, #13]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r0, r1
	beq	.L495
	ldr	r3, [lr, #72]
	ldr	r8, [lr, #68]
	mov	r2, r3
	str	r3, [fp, #-232]
	ldr	r3, [r0, #2836]
	cmp	r2, #0
	ldr	r4, [r3, #72]
	ldr	ip, [r3, #148]
	ble	.L477
	ldr	r3, [fp, #-232]
	add	r2, lr, #214
	sub	r1, fp, #216
	add	r0, r3, #107
	add	r0, lr, r0, lsl #1
.L476:
	ldrsh	r3, [r2, #2]!
	cmp	r2, r0
	sub	r3, r3, #1
	str	r3, [r1, #4]!
	bne	.L476
.L477:
	cmp	r8, #0
	ble	.L475
	add	r0, r8, #87
	add	r2, lr, #174
	sub	r1, fp, #128
	add	r0, lr, r0, lsl #1
.L480:
	ldrsh	r3, [r2, #2]!
	cmp	r2, r0
	sub	r3, r3, #1
	str	r3, [r1, #4]!
	bne	.L480
.L475:
	mov	r5, #1
	mov	r0, r4
	mov	r5, r5, asl ip
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, r5
	mov	r6, r0
	mov	r0, r4
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-232]
	cmp	r1, #0
	addne	r6, r6, #1
	cmp	r3, #0
	str	r6, [fp, #-240]
	ble	.L479
	mov	r9, #0
	ldr	r2, [fp, #-124]
	sub	r0, fp, #212
	mov	r10, r9
	mov	r3, r9
	mov	r1, r9
	mov	r6, r9
	mov	r4, r9
	add	r2, r2, #1
	str	r2, [fp, #-236]
.L481:
	cmp	r8, #0
	ble	.L491
	ldr	r6, [r0]
	ldr	r4, [fp, #-236]
	add	r6, r6, #1
	mla	ip, r6, r4, r3
	cmp	r7, ip
	blt	.L492
	mov	r5, #0
	mov	r1, r3
	mov	r2, r5
	mov	r3, ip
	sub	lr, fp, #124
	b	.L486
.L483:
	ldr	r4, [lr, #4]!
	mov	r1, r3
	add	r4, r4, #1
	mla	ip, r6, r4, r3
	cmp	ip, r7
	bgt	.L484
	mov	r3, ip
.L486:
	add	r2, r2, #1
	add	r5, r5, r4
	cmp	r2, r8
	bne	.L483
.L485:
	cmp	r3, r7
	bgt	.L490
	ldr	r2, [fp, #-232]
	add	r10, r10, #1
	add	r9, r9, r6
	add	r0, r0, #4
	cmp	r10, r2
	bne	.L481
.L490:
	mov	r3, r1
.L484:
	cmp	r4, #0
	beq	.L479
	rsb	r7, r3, r7
	mov	r1, r4
	mov	r0, r7
	bl	__aeabi_idiv
	ldr	r3, [fp, #-240]
	mov	r1, r4
	add	r4, r0, r9
	mov	r0, r7
	mul	r4, r4, r3
	bl	__aeabi_idivmod
	add	r1, r1, r5
	add	r0, r4, r1
.L495:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L491:
	mov	r5, #0
	b	.L485
.L492:
	ldr	r4, [fp, #-236]
	mov	r5, #0
	b	.L484
.L479:
	ldr	r1, .L497
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L495
.L498:
	.align	2
.L497:
	.word	.LC59
	UNWIND(.fnend)
	.size	HEVC_CtbTsAddrToRsAddr, .-HEVC_CtbTsAddrToRsAddr
	.align	2
	.global	HEVC_CalcTileMap
	.type	HEVC_CalcTileMap, %function
HEVC_CalcTileMap:
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
	UNWIND(.pad #44)
	sub	sp, sp, #44
	ldr	r4, .L522
	mov	r5, r0
	str	r0, [fp, #-60]
	mov	r2, #147456
	mov	r1, #0
	ldr	r3, [r4, #48]
	add	r10, r5, #24576
	ldr	r0, .L522+4
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #36864
	mov	r1, #0
	ldr	r0, .L522+8
	blx	r3
	add	r0, r5, #64768
	ldr	r3, [r4, #48]
	mov	r2, #880
	mov	r1, #0
	add	r0, r0, #116
	blx	r3
	add	r2, r5, #61440
	mov	r3, #0
	str	r2, [fp, #-56]
	str	r3, [r2, #3440]
	ldr	r9, [r10, #2840]
	ldr	r1, [r10, #2836]
	ldr	r2, [r9, #72]
	ldr	r8, [r1, #240]
	cmp	r2, r3
	strne	r3, [fp, #-72]
	strne	r3, [fp, #-76]
	beq	.L499
.L507:
	ldr	r2, [r9, #68]
	cmp	r2, #0
	ldr	r2, [fp, #-72]
	add	r2, r2, #108
	streq	r2, [fp, #-80]
	beq	.L508
	mov	r1, r2
	str	r2, [fp, #-80]
	mov	r7, r1, asl #1
	ldr	r1, [fp, #-76]
	mov	r2, #0
	str	r2, [fp, #-48]
	str	r2, [fp, #-52]
	mul	r1, r8, r1
	ldr	r2, [fp, #-56]
	ldr	r2, [r2, #3440]
	str	r1, [fp, #-68]
.L506:
	add	r1, r2, #8064
	ldr	r0, [fp, #-60]
	add	r1, r1, #46
	add	r1, r0, r1, lsl #3
	str	r3, [r1, #4]
	ldrsh	r1, [r9, r7]
	cmp	r1, #0
	ldreq	r1, [fp, #-48]
	addeq	r1, r1, #88
	streq	r1, [fp, #-64]
	beq	.L502
	ldr	r2, [fp, #-48]
	mov	r5, #0
	ldr	r1, [fp, #-52]
	add	r2, r2, #88
	ldr	r0, [fp, #-68]
	str	r2, [fp, #-64]
	add	r6, r1, r0
	mov	ip, r2, asl #1
.L503:
	ldrsh	r2, [r9, ip]
	cmp	r2, #0
	beq	.L505
	ldr	r2, .L522+4
	rsb	lr, r3, r6
	mov	r4, r3
	add	r2, r2, r3, lsl #2
.L504:
	add	r1, lr, r3
	str	r1, [r2], #4
	ldr	r9, [r10, #2840]
	add	r3, r3, #1
	rsb	r1, r4, r3
	ldrsh	r0, [r9, ip]
	cmp	r0, r1
	bhi	.L504
.L505:
	ldrsh	r2, [r9, r7]
	add	r5, r5, #1
	add	r6, r6, r8
	cmp	r2, r5
	bhi	.L503
	ldr	r2, [fp, #-56]
	ldr	r2, [r2, #3440]
.L502:
	ldr	r1, [fp, #-60]
	sub	ip, r3, #1
	ldr	r0, [fp, #-64]
	add	r1, r1, r2, lsl #3
	ldr	lr, [fp, #-48]
	add	r1, r1, #64768
	add	r2, r2, #1
	mov	r0, r0, asl #1
	add	lr, lr, #1
	str	ip, [r1, #120]
	ldr	r1, [fp, #-56]
	str	lr, [fp, #-48]
	str	r2, [r1, #3440]
	ldr	r1, [r9, #68]
	ldrsh	r0, [r9, r0]
	cmp	r1, lr
	ldr	r1, [fp, #-52]
	add	r1, r1, r0
	str	r1, [fp, #-52]
	bhi	.L506
.L508:
	ldr	r2, [fp, #-80]
	ldr	r0, [fp, #-72]
	ldr	r1, [r9, #72]
	mov	r2, r2, asl #1
	add	r0, r0, #1
	cmp	r1, r0
	ldr	r1, [fp, #-76]
	ldrsh	r2, [r9, r2]
	str	r0, [fp, #-72]
	add	r2, r1, r2
	str	r2, [fp, #-76]
	bhi	.L507
.L499:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L523:
	.align	2
.L522:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR2+448
	.word	.LANCHOR3+592
	UNWIND(.fnend)
	.size	HEVC_CalcTileMap, .-HEVC_CalcTileMap
	.align	2
	.global	HEVC_WritePicMsg
	.type	HEVC_WritePicMsg, %function
HEVC_WritePicMsg:
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
	add	r7, r0, #53248
	mov	r5, r0
	ldr	r1, .L546
	mov	r0, #2
	ldr	r3, [r7, #372]
	add	r4, r5, #61440
	ldr	r2, [r7, #548]
	add	r8, r5, #24576
	bl	dprint_vfmw
	ldr	r1, .L546+4
	mov	r0, #4
	ldr	r6, .L546+8
	bl	dprint_vfmw
	ldr	r3, [r7, #532]
	add	ip, r5, #65536
	mov	lr, #0
	add	r1, r5, #2928
	add	r0, r5, #61952
	str	r3, [r4, #1932]
	add	r1, r1, #8
	ldr	r3, [r7, #436]
	mov	r2, #64
	ldr	r9, [r7, #440]
	add	r0, r0, #252
	rsb	r9, r3, r9
	str	r9, [r4, #1936]
	ldr	r9, [r7, #548]
	str	r9, [r4, #1924]
	ldr	r9, [r7, #372]
	str	r9, [r4, #1928]
	ldr	r9, [r7, #536]
	str	r9, [r4, #1940]
	ldr	r9, [r7, #540]
	str	r9, [r4, #1944]
	ldr	r9, [r7, #544]
	str	r9, [r4, #1948]
	ldr	r9, [r7, #460]
	rsb	r3, r3, r9
	str	r3, [r4, #1952]
	ldr	r3, [r7, #464]
	rsb	r3, r9, r3
	str	r3, [r4, #1956]
	str	lr, [r5, #208]
	ldr	r3, [ip, #1836]
	str	r3, [r4, #3212]
	ldr	r3, [r8, #2836]
	ldrb	ip, [r3, #12]	@ zero_extendqisi2
	strb	ip, [r4, #644]
	ldrb	ip, [r3, #5]	@ zero_extendqisi2
	strb	ip, [r4, #645]
	ldrb	ip, [r3, #9]	@ zero_extendqisi2
	strb	ip, [r4, #646]
	ldrb	ip, [r3, #8]	@ zero_extendqisi2
	strb	ip, [r4, #647]
	ldrb	ip, [r3, #7]	@ zero_extendqisi2
	strb	ip, [r4, #648]
	ldr	ip, [r3, #64]
	str	ip, [r4, #652]
	ldr	ip, [r3, #116]
	strb	ip, [r4, #656]
	ldr	ip, [r3, #244]
	sub	ip, ip, #1
	str	ip, [r4, #660]
	ldr	ip, [r3, #240]
	sub	ip, ip, #1
	str	ip, [r4, #664]
	ldr	ip, [r7, #344]
	str	ip, [r4, #668]
	ldr	ip, [r8, #2840]
	ldr	lr, [ip, #84]
	sub	lr, lr, #2
	str	lr, [r4, #672]
	ldrb	lr, [ip, #11]	@ zero_extendqisi2
	strb	lr, [r4, #676]
	ldrb	ip, [ip, #10]	@ zero_extendqisi2
	strb	ip, [r4, #677]
	ldr	ip, [r3, #200]
	str	ip, [r4, #680]
	ldr	ip, [r3, #208]
	str	ip, [r4, #684]
	ldr	ip, [r3, #188]
	str	ip, [r4, #688]
	ldr	ip, [r3, #196]
	str	ip, [r4, #692]
	ldr	ip, [r3, #172]
	str	ip, [r4, #696]
	ldr	ip, [r3, #180]
	str	ip, [r4, #700]
	ldr	ip, [r3, #216]
	str	ip, [r4, #704]
	ldr	ip, [r3, #140]
	str	ip, [r4, #708]
	ldr	lr, [r3, #148]
	ldr	ip, [r6, #52]
	str	lr, [r4, #712]
	ldr	lr, [r3, #112]
	str	lr, [r4, #716]
	ldr	lr, [r3, #104]
	str	lr, [r4, #720]
	ldr	lr, [r3, #124]
	str	lr, [r4, #724]
	ldr	lr, [r3, #120]
	str	lr, [r4, #728]
	ldr	lr, [r3, #108]
	str	lr, [r4, #732]
	ldr	r3, [r3, #100]
	str	r3, [r4, #736]
	blx	ip
	ldr	ip, [r8, #2836]
	add	r3, r5, #64256
	add	r1, r5, #63744
	add	r1, r1, #156
	mov	r0, r5
	ldr	lr, [ip, #76]
	mov	r2, r3
	add	r3, r3, #236
	add	r2, r2, #156
	str	lr, [r4, #828]
	ldr	ip, [ip, #72]
	str	ip, [r4, #832]
	bl	HEVC_CalcTileInfo
	subs	r6, r0, #0
	bne	.L544
	ldr	r3, [r8, #2840]
	ldrb	r2, [r3, #16]	@ zero_extendqisi2
	strb	r2, [r4, #856]
	ldrb	r2, [r3, #7]	@ zero_extendqisi2
	strb	r2, [r4, #857]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	strb	r2, [r4, #858]
	ldrb	r2, [r3, #14]	@ zero_extendqisi2
	strb	r2, [r4, #859]
	ldrb	r2, [r3, #15]	@ zero_extendqisi2
	strb	r2, [r4, #860]
	ldrb	r2, [r3, #13]	@ zero_extendqisi2
	strb	r2, [r4, #861]
	ldr	r2, [r8, #2836]
	ldr	r1, [r3, #56]
	ldr	r2, [r2, #148]
	rsb	r2, r1, r2
	str	r2, [r3, #88]
	ldr	r3, [r8, #2840]
	ldr	r2, [r3, #88]
	str	r2, [r4, #864]
	ldr	r2, [r3, #52]
	str	r2, [r4, #868]
	ldrb	r2, [r3, #8]	@ zero_extendqisi2
	strb	r2, [r4, #872]
	ldrb	r2, [r3, #12]	@ zero_extendqisi2
	strb	r2, [r4, #873]
	ldrb	r2, [r3, #6]	@ zero_extendqisi2
	strb	r2, [r4, #874]
	ldr	r2, [r3, #60]
	str	r2, [r4, #876]
	ldr	r2, [r3, #64]
	str	r2, [r4, #880]
	ldr	r2, [r3, #72]
	sub	r2, r2, #1
	str	r2, [r4, #884]
	ldr	r2, [r3, #68]
	sub	r2, r2, #1
	str	r2, [r4, #888]
	ldrb	r2, [r3, #10]	@ zero_extendqisi2
	strb	r2, [r4, #677]
	ldrb	r3, [r3, #11]	@ zero_extendqisi2
	strb	r3, [r4, #676]
	ldr	r3, [r7, #388]
	ldr	r0, [r5, #244]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	subs	r8, r0, #0
	beq	.L545
	ldr	r2, [r8, #640]
	ldr	r3, [r8, #644]
	cmp	r2, #0
	beq	.L529
	cmp	r3, #0
	beq	.L529
	add	r3, r5, #63488
	add	r2, r5, #63232
	add	r2, r2, #188
	ldr	r0, [r5, #244]
	mov	r1, r3
	add	r3, r3, #152
	add	r1, r1, #60
	bl	FSP_GetDecFsAddrTab
	ldr	r3, [r8, #640]
	add	r2, r5, #63488
	ldr	r3, [r3, #8]
	mov	r1, r2
	add	r2, r2, #64
	add	r1, r1, #144
	str	r3, [r4, #1924]
	ldr	r0, [r5, #244]
	bl	FSP_GetPmvAddrTab
	ldr	r3, [r7, #372]
	str	r3, [r4, #1928]
	ldr	r3, [r8, #656]
	str	r3, [r4, #2196]
	ldr	r3, [r8, #644]
	ldr	r3, [r3, #16]
	str	r3, [r4, #3432]
	ldr	r3, [r8, #644]
	ldr	r3, [r3, #76]
	str	r3, [r4, #3436]
	ldr	ip, [r5, #2800]
	cmp	ip, #0
	str	ip, [r4, #3204]
	beq	.L531
	add	r2, r5, #2864
	add	r3, r5, #64512
	add	r2, r2, #4
	add	r3, r3, #64
	mov	r0, r6
.L532:
	add	r0, r0, #1
	ldr	r1, [r2, #4]!
	cmp	r0, ip
	str	r1, [r3, #4]!
	bne	.L532
.L531:
	add	r8, r5, #62208
	mov	r6, #0
	add	r8, r8, #128
.L533:
	mov	r2, r8
	mov	r0, r6
	mov	r1, r5
	add	r6, r6, #1
	bl	HEVC_WriteQmatrix_8x8
	cmp	r6, #4
	add	r8, r8, #64
	bne	.L533
	add	r2, r5, #62464
	mov	r0, r6
	add	r2, r2, #128
	mov	r1, r5
	bl	HEVC_WriteQmatrix_8x8
	add	r8, r5, #62720
	add	r2, r5, #62464
	mov	r1, r5
	add	r2, r2, #192
	mov	r0, #5
	bl	HEVC_WriteQmatrix_8x8
	mov	r2, r8
	mov	r1, r5
	mov	r0, #0
	bl	HEVC_WriteQmatrix_16x16
	add	r2, r8, #64
	mov	r1, r5
	mov	r0, #1
	bl	HEVC_WriteQmatrix_16x16
	add	r8, r8, #128
	mov	r6, #2
.L534:
	mov	r2, r8
	mov	r0, r6
	mov	r1, r5
	add	r6, r6, #1
	bl	HEVC_WriteQmatrix_16x16
	cmp	r6, #6
	add	r8, r8, #64
	bne	.L534
	add	r2, r5, #62976
	mov	r1, r5
	add	r2, r2, #128
	mov	r0, #0
	bl	HEVC_WriteQmatrix_32x32
	add	r2, r5, #62976
	add	r2, r2, #192
	mov	r1, r5
	mov	r0, #1
	add	r8, r5, #63232
	bl	HEVC_WriteQmatrix_32x32
	mov	r6, #0
.L535:
	mov	r2, r8
	mov	r0, r6
	mov	r1, r5
	add	r6, r6, #1
	bl	HEVC_WriteQmatrix_4x4
	cmp	r6, #6
	add	r8, r8, #16
	bne	.L535
	ldr	r1, [r5, #2268]
	mov	r0, #0
	ldr	r3, [r5, #2272]
	ldr	r2, [r5, #2260]
	ldr	ip, [r5, #2264]
	mov	r1, r1, asl #16
	orr	r3, r1, r3, asl #24
	orr	r3, r3, r2
	orr	r3, r3, ip, asl #8
	str	r3, [r4, #1888]
	ldr	r1, [r5, #2284]
	ldr	r3, [r5, #2288]
	ldr	r2, [r5, #2276]
	ldr	ip, [r5, #2280]
	mov	r1, r1, asl #16
	orr	r3, r1, r3, asl #24
	orr	r3, r3, r2
	orr	r3, r3, ip, asl #8
	str	r3, [r4, #1892]
	ldr	r3, [r7, #372]
	str	r3, [r4, #1920]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L544:
	ldr	r2, .L546+12
	mov	r0, #1
	ldr	r1, .L546+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L529:
	ldr	r1, .L546+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L545:
	ldr	r3, [r7, #388]
	mov	r0, #1
	ldr	r1, .L546+24
	ldr	r2, [r3, #32]
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L547:
	.align	2
.L546:
	.word	.LC60
	.word	.LC61
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+280
	.word	.LC62
	.word	.LC64
	.word	.LC63
	UNWIND(.fnend)
	.size	HEVC_WritePicMsg, .-HEVC_WritePicMsg
	.align	2
	.global	HEVC_CheckListX
	.type	HEVC_CheckListX, %function
HEVC_CheckListX:
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
	ldrb	r3, [r3, #1019]	@ zero_extendqisi2
	cmp	r3, #0
	addeq	r3, r0, #54016
	addeq	r3, r3, #248
	beq	.L550
	ldrsb	r3, [r0, #9]
	cmp	r3, #0
	addne	r3, r0, #57856
	addne	r3, r3, #140
	beq	.L583
.L550:
	ldr	ip, [r3, #160]
	ldr	lr, [r3, #164]
	cmp	ip, #16
	bhi	.L584
	cmp	lr, #16
	bhi	.L554
	cmp	ip, #0
	beq	.L555
	ldr	r3, [r0, #2336]
	cmp	r3, #0
	beq	.L568
	ldr	r3, [r3, #32]
	cmp	r3, #0
	addne	r1, r0, #2336
	movne	r2, #0
	bne	.L560
	b	.L585
.L561:
	ldr	r3, [r1, #4]!
	cmp	r3, #0
	beq	.L556
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L557
.L560:
	add	r2, r2, #1
	cmp	r2, ip
	bne	.L561
.L555:
	cmp	lr, #0
	beq	.L580
	ldr	r3, [r0, #2404]
	cmp	r3, #0
	beq	.L571
	ldr	r3, [r3, #32]
	cmp	r3, #0
	addne	r0, r0, #2400
	movne	r2, #0
	addne	r0, r0, #4
	bne	.L566
	b	.L586
.L567:
	ldr	r3, [r0, #4]!
	cmp	r3, #0
	beq	.L562
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L563
.L566:
	add	r2, r2, #1
	cmp	r2, lr
	bne	.L567
.L580:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L568:
	mov	r2, r3
.L556:
	ldr	r1, .L587
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L585:
	mov	r2, r3
.L557:
	ldr	r1, .L587+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L571:
	mov	r2, r3
.L562:
	ldr	r1, .L587+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L586:
	mov	r2, r3
.L563:
	ldr	r1, .L587+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L583:
	mov	r0, r3
	ldr	r2, .L587+16
	ldr	r1, .L587+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L554:
	mov	r2, lr
	mov	r3, #17
	ldr	r1, .L587+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L584:
	mov	r2, ip
	mov	r3, #17
	ldr	r1, .L587+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L588:
	.align	2
.L587:
	.word	.LC68
	.word	.LC69
	.word	.LC70
	.word	.LC71
	.word	.LANCHOR1+300
	.word	.LC65
	.word	.LC67
	.word	.LC66
	UNWIND(.fnend)
	.size	HEVC_CheckListX, .-HEVC_CheckListX
	.align	2
	.global	HEVC_WriteSliceMsg
	.type	HEVC_WriteSliceMsg, %function
HEVC_WriteSliceMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #76)
	sub	sp, sp, #76
	mov	r4, r0
	bl	HEVC_CheckListX
	subs	r3, r0, #0
	str	r3, [fp, #-112]
	bne	.L627
	ldr	r0, [r4, #208]
	ldr	r3, [r4, #144]
	cmp	r0, r3
	bcs	.L663
	add	r9, r4, #65536
	mov	r5, #1360
	ldr	r7, [fp, #-112]
	mov	r2, r5
	ldr	r6, [r9, #1836]
	add	r8, r4, #53248
	ldr	r3, .L666
	mov	r1, r7
	mla	r6, r5, r0, r6
	ldr	r3, [r3, #48]
	mov	r0, r6
	blx	r3
	ldr	r3, .L666
	mov	r2, #64
	mov	r1, r7
	sub	r0, fp, #108
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, [r4, #208]
	ldr	r2, [r4, #144]
	add	r3, r3, #1
	str	r3, [r4, #208]
	cmp	r3, r2
	ldrcc	r2, [r9, #1836]
	ldrcs	r3, [fp, #-112]
	mlacc	r3, r5, r3, r2
	str	r3, [r6, #1356]
	ldr	r2, [r4, #216]
	ldr	r1, [r8, #1092]
	add	r3, r2, #16640
	add	r2, r2, #1
	add	r3, r3, #204
	add	r3, r4, r3, lsl #2
	str	r1, [r3, #4]
	ldr	r3, [r4, #172]
	str	r2, [r4, #216]
	add	r3, r3, #1
	str	r3, [r4, #172]
	ldr	r2, [r9, #2644]
	mov	r0, r3, asl #5
	ldr	r1, [r8, #1076]
	sub	r3, r0, r3, asl #2
	str	r3, [fp, #-116]
	add	r3, r2, r3
	str	r1, [r3, #8]
	ldr	r2, [r4, #2332]
	ldrb	r1, [r2, #1]	@ zero_extendqisi2
	ldr	r3, [r2, #8]
	cmp	r1, #1
	cmpls	r1, r3
	bcs	.L594
	add	r0, r6, r1, lsl #2
.L595:
	mov	r3, r1, asl #5
	sub	r3, r3, r1, asl #2
	add	r1, r1, #1
	add	r2, r2, r3
	ldr	r2, [r2, #60]
	str	r2, [r0, #8]
	ldr	r2, [r4, #2332]
	add	r2, r2, r3
	ldr	r2, [r2, #56]
	str	r2, [r0], #4
	ldr	r2, [r4, #2332]
	add	r3, r2, r3
	ldr	r3, [r3, #64]
	str	r3, [r0, #12]
	ldr	r2, [r4, #2332]
	ldr	r3, [r2, #8]
	cmp	r3, r1
	movhi	ip, #1
	movls	ip, #0
	cmp	r1, #1
	movhi	ip, #0
	cmp	ip, #0
	bne	.L595
.L594:
	cmp	r3, #1
	addls	r3, r6, r3, lsl #2
	addls	r1, r6, #8
	movls	r2, #0
	bhi	.L600
.L599:
	str	r2, [r3, #8]
	str	r2, [r3], #4
	cmp	r3, r1
	str	r2, [r3, #12]
	bne	.L599
.L600:
	ldrb	r3, [r8, #1019]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L664
.L598:
	add	r5, r4, #57344
	ldr	r3, [r5, #812]
	cmp	r3, #0
	beq	.L601
	add	r0, r4, #2336
	add	r10, r6, #112
	add	lr, r6, #368
	mov	r1, #0
.L602:
	ldr	ip, [r0]
	sub	r7, fp, #44
	add	r1, r1, #1
	ldr	r2, [ip, #32]
	ldr	r3, [r2, #24]
	ldr	r2, [r2, #28]
	add	r3, r3, #15872
	add	r3, r3, #14
	add	r2, r7, r2, lsl #2
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #8]
	str	r3, [r2, #-64]
	ldr	r3, [ip, #16]
	str	r3, [r10, #4]!
	ldr	r3, [r0], #4
	ldr	r3, [r3, #32]
	ldr	r3, [r3, #28]
	str	r3, [lr, #4]!
	ldr	r3, [r5, #812]
	cmp	r3, r1
	bhi	.L602
	ldr	r2, [r5, #816]
	cmp	r2, #0
	beq	.L603
.L626:
	add	r1, r4, #2400
	add	r10, r6, #176
	add	r1, r1, #4
	add	lr, r6, #432
	mov	r0, #0
.L604:
	ldr	ip, [r1]
	sub	r7, fp, #44
	add	r0, r0, #1
	ldr	r2, [ip, #32]
	ldr	r3, [r2, #24]
	ldr	r2, [r2, #28]
	add	r3, r3, #15872
	add	r3, r3, #14
	add	r2, r7, r2, lsl #2
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #8]
	str	r3, [r2, #-64]
	ldr	r3, [ip, #16]
	str	r3, [r10, #4]!
	ldr	r3, [r1], #4
	ldr	r3, [r3, #32]
	ldr	r3, [r3, #28]
	str	r3, [lr, #4]!
	ldr	r2, [r5, #816]
	cmp	r2, r0
	bhi	.L604
	ldr	r3, [r5, #812]
.L603:
	cmp	r3, #0
	beq	.L605
	ldr	r1, [r6, #372]
	ldr	lr, [r8, #344]
	add	r1, r1, #732
	add	r1, r1, #2
	ldr	r1, [r4, r1, asl #2]
	cmp	lr, r1
	blt	.L633
	add	ip, r6, #372
	mov	r0, #0
	b	.L607
.L608:
	ldr	r1, [ip, #4]!
	add	r1, r1, #732
	add	r1, r1, #2
	ldr	r1, [r4, r1, asl #2]
	cmp	r1, lr
	bgt	.L633
.L607:
	add	r0, r0, #1
	cmp	r0, r3
	bne	.L608
.L605:
	ldr	r1, [r5, #712]
	cmp	r1, #0
	bne	.L661
	cmp	r2, #0
	beq	.L661
	ldr	r3, [r6, #436]
	ldr	ip, [r8, #344]
	add	r3, r3, #732
	add	r3, r3, #2
	ldr	r3, [r4, r3, asl #2]
	cmp	ip, r3
	blt	.L606
	add	r0, r6, #436
	b	.L609
.L610:
	ldr	r3, [r0, #4]!
	add	r3, r3, #732
	add	r3, r3, #2
	ldr	r3, [r4, r3, asl #2]
	cmp	r3, ip
	bgt	.L633
.L609:
	add	r1, r1, #1
	cmp	r1, r2
	bne	.L610
.L661:
	mov	r1, #1
.L606:
	strb	r1, [r6, #24]
	ldrb	r3, [r8, #1019]	@ zero_extendqisi2
	strb	r3, [r6, #25]
	ldr	r3, [r5, #708]
	str	r3, [r6, #28]
	ldrb	r3, [r5, #662]	@ zero_extendqisi2
	strb	r3, [r6, #32]
	ldr	r3, [r5, #732]
	str	r3, [r6, #36]
	ldr	r3, [r5, #712]
	cmp	r3, #0
	bne	.L611
	ldrb	r3, [r5, #663]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L612
	ldr	r3, [r5, #764]
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #436]
	mov	r7, r3
.L613:
	str	r7, [r6, #40]
	add	r10, r4, #24576
	ldr	r3, [r5, #764]
	str	r3, [r6, #48]
	ldrb	r3, [r5, #663]	@ zero_extendqisi2
	strb	r3, [r6, #52]
	ldr	r3, [r5, #768]
	str	r3, [r6, #44]
	ldr	r3, [r5, #820]
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r6, #60]
	ldr	r3, [r5, #824]
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r6, #56]
	ldrb	r2, [r5, #659]	@ zero_extendqisi2
	strb	r2, [r6, #64]
	ldrb	r2, [r5, #661]	@ zero_extendqisi2
	strb	r2, [r6, #65]
	ldr	r2, [r5, #812]
	str	r2, [r6, #68]
	ldr	r2, [r5, #816]
	str	r2, [r6, #72]
	ldr	r2, [r5, #712]
	str	r2, [r6, #76]
	ldr	r2, [r8, #1096]
	str	r2, [r6, #80]
	ldr	r2, [r10, #2836]
	ldr	r0, [r8, #1096]
	ldr	r1, [r2, #240]
	bl	__aeabi_uidivmod
	str	r1, [r6, #84]
	ldr	r2, [r10, #2836]
	ldr	r0, [r8, #1096]
	ldr	r1, [r2, #240]
	bl	__aeabi_uidiv
	str	r0, [r6, #88]
	mov	r0, r4
	ldr	r1, [r8, #1096]
	bl	HEVC_CtbRsAddrToTsAddr
	str	r0, [r6, #92]
	ldr	r2, [r5, #776]
	str	r2, [r6, #96]
	ldr	r2, [r5, #780]
	str	r2, [r6, #100]
	ldr	r2, [r5, #788]
	str	r2, [r6, #104]
	ldr	r2, [r5, #784]
	str	r2, [r6, #108]
	ldrb	r2, [r5, #658]	@ zero_extendqisi2
	strb	r2, [r6, #112]
	ldrb	r2, [r5, #657]	@ zero_extendqisi2
	strb	r2, [r6, #113]
	ldrb	r2, [r5, #665]	@ zero_extendqisi2
	strb	r2, [r6, #114]
	ldrb	r2, [r5, #666]	@ zero_extendqisi2
	strb	r2, [r6, #115]
	ldr	r3, [r10, #2840]
	ldrb	r2, [r3, #10]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L618
	ldr	r2, [r5, #712]
	cmp	r2, #1
	beq	.L619
.L618:
	ldrb	r3, [r3, #11]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L665
.L621:
	ldrb	r3, [r5, #659]	@ zero_extendqisi2
	add	r1, r4, #61440
	ldr	ip, [fp, #-116]
	cmp	r3, #0
	subne	r3, fp, #44
	movne	r2, r7
	addne	r10, r3, r2, lsl #2
	ldrne	r3, [r10, #-64]
	strne	r3, [r6, #564]
	mov	r3, #0
	ldr	r2, [r9, #2644]
	ldr	r0, [r4, #2332]
	add	r2, r2, ip
	str	r0, [r2, #24]
	str	r3, [r4, #2332]
	ldrb	r2, [r1, #861]	@ zero_extendqisi2
	cmp	r2, r3
	beq	.L642
	ldr	ip, [r4, #216]
	cmp	ip, #1
	ldreq	r2, [r6, #92]
	streq	r2, [r9, #228]
	beq	.L625
	add	r0, r4, ip, lsl #3
	ldr	lr, [r6, #92]
	add	r0, r0, #65536
	add	r2, ip, #8192
	add	r2, r2, #27
	sub	lr, lr, #1
	str	lr, [r0, #216]
	sub	r3, ip, #1
	ldr	r0, [r6, #92]
	add	r2, r4, r2, lsl #3
	str	r0, [r2, #4]
.L625:
	ldr	r2, [r1, #660]
	add	r3, r4, r3, lsl #3
	ldr	r1, [r1, #664]
	add	r3, r3, #65536
	mla	r2, r1, r2, r2
	add	r2, r2, r1
	str	r2, [r3, #232]
	ldr	r1, [r6, #92]
	sub	r1, r1, #1
	cmp	r1, #36864
	bcs	.L642
	mov	r0, r4
	bl	HEVC_CtbTsAddrToRsAddr
	str	r0, [r6, #568]
.L642:
	ldr	r0, [fp, #-112]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L633:
	mov	r1, #0
	b	.L606
.L611:
	cmp	r3, #1
	movne	r3, #0
	movne	r7, r3
	bne	.L613
.L612:
	ldr	r3, [r5, #764]
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #372]
	mov	r7, r3
	b	.L613
.L664:
	ldr	r3, .L666
	add	r1, r4, #54016
	add	r0, r4, #57856
	add	r1, r1, #248
	add	r0, r0, #140
	movw	r2, #3732
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #1
	strb	r3, [r4, #9]
	b	.L598
.L665:
	ldr	r3, [r5, #712]
	cmp	r3, #0
	bne	.L621
.L619:
	ldr	ip, [r5, #2244]
	add	r1, r4, #60416
	ldr	r3, .L666
	add	r1, r1, #208
	ldr	r8, .L666
	mov	r2, #64
	str	ip, [r6, #580]
	add	r0, r6, #588
	ldr	ip, [r5, #3276]
	ldr	r3, [r3, #52]
	str	ip, [r6, #584]
	blx	r3
	add	r1, r4, #59648
	add	r1, r1, #140
	ldr	r3, [r8, #52]
	mov	r2, #64
	add	r0, r6, #716
	blx	r3
	add	r1, r4, #60672
	add	r1, r1, #16
	ldr	r3, [r8, #52]
	mov	r2, #128
	add	r0, r6, #844
	blx	r3
	add	r1, r4, #60672
	add	r0, r6, #1088
	ldr	r3, [r8, #52]
	add	r1, r1, #144
	add	r0, r0, #12
	mov	r2, #128
	blx	r3
	ldr	r3, [r5, #712]
	cmp	r3, #0
	bne	.L621
	add	r1, r4, #60928
	ldr	r3, [r8, #52]
	add	r1, r1, #16
	mov	r2, #64
	add	r0, r6, #652
	blx	r3
	add	r1, r4, #60160
	ldr	r3, [r8, #52]
	add	r1, r1, #140
	mov	r2, #64
	add	r0, r6, #780
	blx	r3
	add	r1, r4, #60928
	ldr	r3, [r8, #52]
	add	r1, r1, #80
	mov	r2, #128
	add	r0, r6, #972
	blx	r3
	add	r1, r4, #60928
	add	r0, r6, #1216
	ldr	r3, [r8, #52]
	add	r1, r1, #208
	mov	r2, #128
	add	r0, r0, #12
	blx	r3
	b	.L621
.L601:
	ldr	r3, [r5, #816]
	cmp	r3, #0
	bne	.L626
	b	.L661
.L627:
	mvn	r3, #0
	str	r3, [fp, #-112]
	b	.L642
.L663:
	mov	r2, r0
	ldr	r1, .L666+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	str	r3, [fp, #-112]
	b	.L642
.L667:
	.align	2
.L666:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC72
	UNWIND(.fnend)
	.size	HEVC_WriteSliceMsg, .-HEVC_WriteSliceMsg
	.align	2
	.global	HEVC_GetVirAddr
	.type	HEVC_GetVirAddr, %function
HEVC_GetVirAddr:
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
	add	r0, r1, r0, lsl #2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_GetVirAddr, .-HEVC_GetVirAddr
	.align	2
	.global	HEVC_GetPhyAddr
	.type	HEVC_GetPhyAddr, %function
HEVC_GetPhyAddr:
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
	add	r0, r1, r0, lsl #2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_GetPhyAddr, .-HEVC_GetPhyAddr
	.align	2
	.global	HEVC_CfgVDMReg
	.type	HEVC_CfgVDMReg, %function
HEVC_CfgVDMReg:
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
	add	ip, r0, #53248
	add	r2, r0, #61440
	mov	r1, r0
	mov	r0, #0
	ldr	r3, [ip, #364]
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r2, #1976]
	ldr	r3, [ip, #368]
	str	r3, [r2, #1968]
	ldr	r3, [r1, #208]
	str	r3, [r2, #3208]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_CfgVDMReg, .-HEVC_CfgVDMReg
	.align	2
	.global	HEVC_IsRefForCurrPic
	.type	HEVC_IsRefForCurrPic, %function
HEVC_IsRefForCurrPic:
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
	UNWIND(.pad #44)
	sub	sp, sp, #44
	add	r3, r0, #53248
	str	r3, [fp, #-48]
	mov	r6, r0
	ldr	r3, [r3, #1092]
	ldr	r2, [r0, #156]
	cmp	r3, #0
	blt	.L673
	cmp	r3, r2
	bge	.L673
	ldr	r2, [r0, #2320]
	movw	r1, #2024
	mla	r3, r1, r3, r2
	ldr	r2, [r0, #152]
	ldr	r3, [r3, #32]
	cmp	r3, #0
	blt	.L677
	cmp	r3, r2
	bge	.L677
	ldr	r2, [r0, #2316]
	movw	r1, #26164
	add	r0, r0, #57344
	mov	r9, #1
	str	r0, [fp, #-52]
	mla	r3, r1, r3, r2
	ldr	r2, [r0, #204]
	ldr	r1, [r0, #208]
	ldr	ip, [r0, #212]
	add	r10, r1, r2
	str	r2, [fp, #-60]
	cmp	r10, ip
	str	r1, [fp, #-64]
	ldr	r3, [r3, #128]
	mov	r4, r10
	mov	r9, r9, asl r3
	bcs	.L688
	cmp	r10, #15
	bgt	.L680
	ldr	r3, [fp, #-48]
	add	r5, r10, #14400
	add	r5, r5, #17
	str	r10, [fp, #-68]
	str	r6, [fp, #-56]
	ldr	r3, [r3, #344]
	add	r5, r6, r5, lsl #2
	mov	r6, ip
	mov	r10, r3
	b	.L682
.L700:
	cmp	r6, r4
	bls	.L698
	cmp	r4, #16
	beq	.L699
.L682:
	ldr	r7, [r5, #4]!
	mov	r1, r9
	mov	r0, r10
	mov	r8, r10
	ldr	r2, [r5, #-1884]
	cmp	r2, #0
	bne	.L683
	bl	__aeabi_idivmod
	mov	r0, r7
	mov	r8, r1
	mov	r1, r9
	bl	__aeabi_idivmod
	mov	r7, r1
.L683:
	cmp	r8, r7
	add	r4, r4, #1
	bne	.L700
	ldr	r10, [fp, #-68]
	mov	r4, #1
	ldr	r6, [fp, #-56]
.L679:
	cmp	r10, #0
	beq	.L675
	ldr	r2, [fp, #-48]
	ldr	r3, [fp, #-52]
	ldr	ip, [r2, #1064]
	ldr	r3, [r3, #260]
	ldr	r0, [r2, #344]
	add	r3, ip, r3
	cmp	r0, r3
	beq	.L692
	add	r6, r6, #57600
	mov	r1, #0
	add	r6, r6, #4
	b	.L685
.L686:
	ldr	r3, [r6, #4]!
	add	r3, ip, r3
	cmp	r3, r0
	beq	.L692
.L685:
	add	r1, r1, #1
	cmp	r1, r10
	beq	.L675
	cmp	r1, #17
	bne	.L686
	ldr	r3, [fp, #-64]
	mov	r0, #0
	ldr	r2, [fp, #-60]
	str	r1, [sp]
	str	r3, [sp, #8]
	mov	r3, r1
	str	r2, [sp, #4]
	movw	r2, #6983
	ldr	r1, .L701
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L677:
	str	r2, [sp]
	mov	r0, #0
	movw	r2, #6934
	ldr	r1, .L701+4
	mov	r4, r0
	bl	dprint_vfmw
.L675:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L673:
	str	r2, [sp]
	mov	r0, #0
	movw	r2, #6922
	ldr	r1, .L701+8
	mov	r4, r0
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L699:
	mov	ip, r6
	ldr	r6, [fp, #-56]
.L680:
	mov	r3, r4
	mov	r1, #16
	movw	r2, #6954
	str	r1, [sp]
	mov	r0, #0
	str	ip, [sp, #4]
	ldr	r1, .L701+12
	mov	r4, r0
	bl	dprint_vfmw
	ldr	r3, [fp, #-52]
	ldr	r2, [r3, #204]
	ldr	r3, [r3, #208]
	str	r2, [fp, #-60]
	add	r10, r3, r2
	str	r3, [fp, #-64]
	b	.L679
.L698:
	ldr	r10, [fp, #-68]
	mov	r4, #0
	ldr	r6, [fp, #-56]
	b	.L679
.L692:
	mov	r4, #1
	b	.L675
.L688:
	mov	r4, #0
	b	.L679
.L702:
	.align	2
.L701:
	.word	.LC76
	.word	.LC74
	.word	.LC73
	.word	.LC75
	UNWIND(.fnend)
	.size	HEVC_IsRefForCurrPic, .-HEVC_IsRefForCurrPic
	.align	2
	.global	HEVC_RepairList
	.type	HEVC_RepairList, %function
HEVC_RepairList:
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
	add	r3, r0, #53248
	str	r3, [fp, #-56]
	mov	ip, r0
	ldr	r3, [r3, #1076]
	cmp	r3, #1
	beq	.L720
	cmp	r3, #2
	beq	.L721
	cmp	r3, #0
	moveq	r3, #2
	streq	r3, [fp, #-48]
	beq	.L705
	ldr	r2, .L730
	mov	r0, #1
	ldr	r1, .L730+4
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L720:
	str	r3, [fp, #-48]
.L705:
	add	r8, ip, #54272
	add	r9, ip, #2336
	add	r8, r8, #148
	mov	r10, #0
.L718:
	ldr	r1, [r8, #4]!
	cmp	r1, #0
	beq	.L708
	ldr	r3, [r9]
	cmp	r3, #0
	bne	.L722
	mov	r2, r9
	mov	r4, r3
	b	.L710
.L712:
	ldr	r6, [r2, #4]!
	cmp	r6, #0
	bne	.L728
.L710:
	add	r4, r4, #1
	cmp	r4, r1
	bne	.L712
	mov	r4, #255
.L711:
	ldr	r2, [ip, #2324]
	ldr	r2, [r2, #12]
	cmp	r2, #2
	bne	.L713
	ldr	r2, [fp, #-56]
	ldr	r6, [r2, #388]
	add	r6, r6, #36
.L709:
	mov	r7, r9
	mov	r5, #0
	b	.L719
.L715:
	ldr	r3, [r7, #4]!
.L719:
	cmp	r3, #0
	beq	.L729
.L714:
	ldr	r3, [r8]
	add	r5, r5, #1
	cmp	r3, r5
	bhi	.L715
.L716:
	ldr	r3, [fp, #-48]
	add	r10, r10, #1
	add	r9, r9, #68
	cmp	r10, r3
	bcc	.L718
.L721:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L708:
	ldr	r3, [ip, #2324]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L716
	mov	r3, #1
	str	r3, [r8]
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #388]
	add	r3, r3, #36
	str	r3, [r9]
	b	.L716
.L729:
	sxtb	r2, r4
	str	r5, [sp]
	str	r2, [sp, #4]
	mov	r3, r10
	ldr	r2, .L730
	mov	r0, #1
	ldr	r1, .L730+8
	str	ip, [fp, #-52]
	bl	dprint_vfmw
	str	r6, [r7]
	ldr	ip, [fp, #-52]
	b	.L714
.L728:
	uxtb	r4, r4
	cmp	r4, #255
	bne	.L709
	b	.L711
.L722:
	mov	r6, r3
	mov	r4, #0
	b	.L709
.L713:
	mov	r3, r10
	ldr	r2, .L730
	ldr	r1, .L730+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L731:
	.align	2
.L730:
	.word	.LANCHOR1+316
	.word	.LC77
	.word	.LC79
	.word	.LC78
	UNWIND(.fnend)
	.size	HEVC_RepairList, .-HEVC_RepairList
	.align	2
	.global	HEVC_InitListX
	.type	HEVC_InitListX, %function
HEVC_InitListX:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 152
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #156)
	sub	sp, sp, #156
	subs	r7, r0, #0
	beq	.L768
	ldr	r3, .L800
	mov	r2, #136
	mov	r1, #0
	sub	r0, fp, #180
	ldr	r3, [r3, #48]
	blx	r3
	ldr	lr, [r7, #2520]
	cmp	lr, #0
	beq	.L769
	mov	r0, #0
	add	r1, r7, #2592
	mov	r3, r0
	b	.L736
.L735:
	cmp	r3, lr
	beq	.L734
.L736:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #1]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L735
	ldr	ip, [r2, #52]
	cmp	ip, #0
	bne	.L735
	ldr	ip, [r2, #56]
	cmp	ip, #1
	subeq	ip, fp, #44
	addeq	r2, r2, #36
	addeq	ip, ip, r0, lsl #2
	addeq	r0, r0, #1
	streq	r2, [ip, #-136]
	cmp	r3, lr
	bne	.L736
.L734:
	ldr	r4, [r7, #2524]
	cmp	r4, #0
	beq	.L770
	add	r1, r7, #2656
	mov	ip, r0
	add	r1, r1, #4
	mov	r3, #0
	b	.L739
.L738:
	cmp	r3, r4
	beq	.L796
.L739:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	lr, [r2, #1]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L738
	ldr	lr, [r2, #52]
	cmp	lr, #0
	bne	.L738
	ldr	lr, [r2, #56]
	cmp	lr, #1
	subeq	lr, fp, #44
	addeq	r2, r2, #36
	addeq	lr, lr, ip, lsl #2
	addeq	ip, ip, #1
	streq	r2, [lr, #-136]
	cmp	r3, r4
	bne	.L739
.L796:
	rsb	r4, r0, ip
.L737:
	ldr	r5, [r7, #2516]
	cmp	r5, #0
	beq	.L771
	add	lr, r7, #2720
	mov	r9, ip
	add	lr, lr, #8
	mov	r2, #0
	b	.L742
.L741:
	cmp	r2, r5
	beq	.L797
.L742:
	ldr	r3, [lr, #4]!
	add	r2, r2, #1
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L741
	ldr	r1, [r3, #52]
	cmp	r1, #1
	bne	.L741
	ldr	r1, [r3, #56]
	sub	r6, fp, #44
	add	r6, r6, r9, lsl #2
	add	r3, r3, #36
	cmp	r1, #0
	addeq	r9, r9, #1
	streq	r3, [r6, #-136]
	cmp	r2, r5
	bne	.L742
.L797:
	rsb	r5, r0, r9
	rsb	r5, r4, r5
.L740:
	cmp	r9, #0
	beq	.L798
	add	r3, r7, #53248
	str	r3, [fp, #-184]
	ldr	r3, [r3, #1076]
	cmp	r3, #0
	str	r3, [fp, #-188]
	beq	.L799
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1184]
	ldr	r5, [r3, #1188]
	cmp	r4, #0
	str	r4, [r3, #1176]
	str	r5, [r3, #1180]
	beq	.L761
.L767:
	add	r1, r7, #57344
	add	r2, r7, #2320
	add	r8, r7, #57600
	add	r2, r2, #12
	ldrb	r10, [r1, #392]	@ zero_extendqisi2
	add	r8, r8, #140
	str	r5, [fp, #-192]
	mov	r6, #0
	mov	r5, r10
	mov	r10, r4
	mov	r4, r2
.L754:
	cmp	r5, #0
	mov	r1, r9
	mov	r0, r6
	ldrne	r1, [r8]
	bne	.L753
	bl	__aeabi_uidivmod
.L753:
	sub	r3, fp, #44
	add	r6, r6, #1
	add	r3, r3, r1, lsl #2
	cmp	r10, r6
	add	r8, r8, #4
	ldr	r3, [r3, #-136]
	str	r3, [r4, #4]!
	bhi	.L754
	ldr	r3, [fp, #-188]
	ldr	r5, [fp, #-192]
	cmp	r3, #0
	bne	.L795
.L755:
	cmp	r5, #0
	beq	.L757
	add	r2, r7, #57344
	add	r6, r7, #57856
	add	r6, r6, #12
	add	r8, r7, #2400
	ldrb	r10, [r2, #393]	@ zero_extendqisi2
	mov	r4, #0
.L760:
	cmp	r10, #0
	mov	r1, r9
	mov	r0, r4
	ldrne	r1, [r6]
	bne	.L759
	bl	__aeabi_uidivmod
.L759:
	sub	r3, fp, #44
	add	r4, r4, #1
	add	r3, r3, r1, lsl #2
	cmp	r4, r5
	add	r6, r6, #4
	ldr	r3, [r3, #-68]
	str	r3, [r8, #4]!
	bne	.L760
.L795:
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1176]
	ldr	r5, [r3, #1180]
.L756:
	cmp	r4, #16
	bhi	.L765
.L761:
	add	r4, r4, #580
	add	r2, r7, #2400
	add	r4, r4, #3
	add	r4, r7, r4, lsl #2
.L764:
	ldr	r3, [r7, #2336]
	str	r3, [r4, #4]!
	cmp	r4, r2
	bne	.L764
.L765:
	cmp	r5, #16
	bhi	.L763
	add	r5, r5, #600
	add	r2, r7, #2464
	add	r2, r2, #4
	add	r5, r7, r5, lsl #2
.L766:
	ldr	r3, [r7, #2404]
	str	r3, [r5, #4]!
	cmp	r5, r2
	bne	.L766
.L763:
	mov	r0, #0
.L794:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L799:
	cmp	r4, #0
	beq	.L746
	sub	r3, fp, #180
	ldr	r2, [fp, #-188]
	add	lr, r3, r0, lsl #2
	sub	r1, fp, #116
.L747:
	add	r2, r2, #1
	ldr	r3, [lr], #4
	cmp	r2, r4
	str	r3, [r1, #4]!
	bne	.L747
.L746:
	cmp	r0, #0
	beq	.L748
	add	r1, r4, #16
	sub	lr, fp, #180
	mov	r2, #0
	add	r1, lr, r1, lsl #2
.L749:
	add	r2, r2, #1
	ldr	r3, [lr], #4
	cmp	r2, r0
	str	r3, [r1, #4]!
	bne	.L749
	add	r4, r4, r2
.L748:
	cmp	r5, #0
	beq	.L750
	add	r4, r4, #16
	sub	r3, fp, #180
	add	ip, r3, ip, lsl #2
	mov	r2, #0
	add	r4, r3, r4, lsl #2
.L751:
	add	r2, r2, #1
	ldr	r3, [ip], #4
	cmp	r2, r5
	str	r3, [r4, #4]!
	bne	.L751
.L750:
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1184]
	ldr	r5, [r3, #1188]
	cmp	r4, #0
	str	r4, [r3, #1176]
	str	r5, [r3, #1180]
	bne	.L767
	b	.L755
.L798:
	ldr	r3, .L800
	add	r4, r7, #53248
	mov	r2, #136
	mov	r1, r9
	str	r9, [r4, #1176]
	add	r0, r7, #2336
	ldr	ip, [r3, #48]
	str	r9, [r4, #1180]
	blx	ip
	ldr	r2, [r7, #2324]
	ldr	r2, [r2, #12]
	cmp	r2, #2
	bne	.L763
	ldr	r2, [r4, #388]
	mov	r3, #1
	mov	r0, r9
	str	r3, [r4, #1176]
	add	r3, r2, #36
	str	r3, [r7, #2336]
	b	.L794
.L769:
	mov	r0, lr
	b	.L734
.L770:
	mov	ip, r0
	b	.L737
.L771:
	mov	r9, ip
	b	.L740
.L757:
	ldr	r3, .L800
	add	r0, r7, #2400
	add	r0, r0, #4
	mov	r2, #68
	add	r1, r7, #2336
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1176]
	str	r4, [r3, #1180]
	mov	r5, r4
	b	.L756
.L768:
	mvn	r0, #0
	b	.L794
.L801:
	.align	2
.L800:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_InitListX, .-HEVC_InitListX
	.align	2
	.global	HEVC_DecList
	.type	HEVC_DecList, %function
HEVC_DecList:
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
	add	r3, r0, #53248
	mov	r4, r0
	ldr	r2, [r3, #1076]
	cmp	r2, #2
	beq	.L807
	bl	HEVC_InitListX
	cmp	r0, #0
	bne	.L808
	mov	r0, r4
	bl	HEVC_RepairList
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, .L809
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L807:
	mov	r0, #0
	str	r0, [r3, #1176]
	str	r0, [r3, #1180]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L808:
	ldr	r1, .L809+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L810:
	.align	2
.L809:
	.word	.LC81
	.word	.LC80
	UNWIND(.fnend)
	.size	HEVC_DecList, .-HEVC_DecList
	.align	2
	.global	HEVC_RemoveApcStore
	.type	HEVC_RemoveApcStore, %function
HEVC_RemoveApcStore:
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
	add	r5, r0, r1, lsl #2
	mov	r4, r0
	ldr	r2, [r5, #2528]
	ldr	r3, [r2, #28]
	cmp	r3, #16
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r3, r3, #700
	mov	ip, #0
	add	r3, r3, #2
	ldr	r1, .L816
	mov	r0, #13
	str	ip, [r4, r3, asl #2]
	ldr	r3, [r2, #28]
	add	r3, r3, #716
	add	r3, r3, #2
	str	ip, [r4, r3, asl #2]
	ldr	r3, [r2, #28]
	add	r3, r3, #732
	add	r3, r3, #2
	str	ip, [r4, r3, asl #2]
	ldr	r3, [r2, #24]
	add	r3, r3, #528
	add	r3, r4, r3, lsl #2
	str	ip, [r3, #4]
	ldr	r3, [r2, #24]
	ldr	r2, [r2, #28]
	bl	dprint_vfmw
	ldr	r3, [r5, #2528]
	mov	r2, #16
	str	r2, [r3, #28]
	ldr	r3, [r4, #2804]
	sub	r3, r3, #1
	str	r3, [r4, #2804]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L817:
	.align	2
.L816:
	.word	.LC82
	UNWIND(.fnend)
	.size	HEVC_RemoveApcStore, .-HEVC_RemoveApcStore
	.align	2
	.global	HEVC_RemoveUnusedApcStore
	.type	HEVC_RemoveUnusedApcStore, %function
HEVC_RemoveUnusedApcStore:
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
	ldr	r3, [r0, #2508]
	mov	r6, r0
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, r0, #2512
	mov	r4, #0
	add	r5, r5, #12
	b	.L821
.L820:
	ldr	r3, [r6, #2508]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L824
.L821:
	ldr	r3, [r5, #4]!
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L820
	mov	r1, r4
	mov	r0, r6
	bl	HEVC_RemoveApcStore
	ldr	r3, [r6, #2508]
	add	r4, r4, #1
	cmp	r4, r3
	bcc	.L821
.L824:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_RemoveUnusedApcStore, .-HEVC_RemoveUnusedApcStore
	.align	2
	.global	HEVC_ApplyReferencePictureSet
	.type	HEVC_ApplyReferencePictureSet, %function
HEVC_ApplyReferencePictureSet:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
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
	add	r3, r0, #53248
	ldr	r2, [r0, #2320]
	movw	r4, #2024
	ldr	r1, [r3, #1092]
	movw	ip, #26164
	str	r3, [fp, #-68]
	mov	r9, r0
	ldr	r3, [r0, #2316]
	mov	r0, #1
	mla	r2, r4, r1, r2
	ldr	lr, [r9, #2508]
	cmp	lr, #0
	ldr	r2, [r2, #32]
	mla	r3, ip, r2, r3
	ldr	r8, [r3, #128]
	mov	r3, r0, asl r8
	str	r3, [fp, #-48]
	beq	.L825
	add	r8, r9, #2528
	add	r3, r9, #57600
	add	r7, r9, #57344
	str	r9, [fp, #-56]
	mov	r9, r8
	mov	r2, #0
	add	r3, r3, #4
	str	r2, [fp, #-60]
	str	r3, [fp, #-72]
	str	r0, [fp, #-64]
.L842:
	ldr	r0, [r7, #204]
	ldr	r2, [r7, #208]
	ldr	r1, [r7, #212]
	add	r2, r2, r0
	ldr	r5, [r9]
	cmp	r2, r1
	mov	r8, r2
	movcs	r4, #0
	ldrcs	r0, [r5, #12]
	bcs	.L828
	add	r6, r2, #14400
	ldr	r3, [fp, #-56]
	add	r6, r6, #18
	ldr	r10, [r5, #12]
	mov	r4, #0
	add	r6, r3, r6, lsl #2
.L834:
	ldr	r3, [r6, #-1884]
	cmp	r3, #0
	ldr	r3, [r5, #52]
	beq	.L829
	cmp	r3, #0
	beq	.L830
	ldr	r3, [r6]
	cmp	r3, r10
	beq	.L860
.L830:
	cmp	r4, #0
	bne	.L847
	ldr	r1, [r6]
	mov	r4, r10
	b	.L844
.L829:
	cmp	r3, #0
	beq	.L832
	ldr	r1, [fp, #-48]
	mov	r0, r10
	bl	__aeabi_idivmod
	ldr	r0, [r6]
	str	r1, [fp, #-52]
	ldr	r1, [fp, #-48]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-52]
	cmp	r3, r1
	beq	.L860
.L832:
	cmp	r4, #0
	bne	.L847
	ldr	r3, [r6]
	mov	r0, r10
	ldr	r1, [fp, #-48]
	str	r3, [fp, #-52]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-52]
	mov	r0, r3
	mov	r4, r1
	ldr	r1, [fp, #-48]
	bl	__aeabi_idivmod
.L844:
	ldrb	r0, [r5, #1]	@ zero_extendqisi2
	rsb	r4, r1, r4
	clz	r4, r4
	cmp	r0, #0
	mov	r4, r4, lsr #5
	moveq	r4, #0
	cmp	r4, #0
	bne	.L861
.L847:
	mov	r0, r10
.L833:
	ldr	r3, [r7, #212]
	add	r8, r8, #1
	add	r6, r6, #4
	mov	r10, r0
	cmp	r8, r3
	bcc	.L834
	ldr	r2, [r7, #208]
	ldr	r1, [r7, #204]
	add	r2, r2, r1
.L828:
	ldr	r3, [fp, #-68]
	cmp	r2, #0
	ldrne	r8, [fp, #-72]
	movne	r6, #0
	ldr	ip, [r3, #1064]
	beq	.L836
.L838:
	ldr	r3, [r5, #52]
	add	r6, r6, #1
	cmp	r3, #0
	bne	.L837
	ldr	r3, [r8]
	add	r3, ip, r3
	cmp	r3, r0
	beq	.L862
.L837:
	ldr	r3, [r7, #208]
	add	r8, r8, #4
	ldr	r2, [r7, #204]
	add	r3, r3, r2
	cmp	r6, r3
	bcc	.L838
.L836:
	eor	r4, r4, #1
	cmp	r0, ip
	moveq	r0, #0
	andne	r0, r4, #1
	cmp	r0, #0
	beq	.L840
	mov	r3, #0
	strb	r3, [r5, #1]
	ldr	r2, [r9]
	str	r3, [r2, #52]
	ldr	r2, [r9]
	ldr	r1, [r2, #8]
	cmp	r1, #1
	beq	.L840
	ldr	r1, [r2, #32]
	mov	r2, r3
	ldr	r3, [fp, #-56]
	ldr	r0, [r3, #244]
	bl	FSP_SetRef
.L840:
	ldr	r3, [fp, #-56]
	add	r9, r9, #4
	ldr	r2, [r3, #2508]
	ldr	r3, [fp, #-60]
	add	r3, r3, #1
	str	r3, [fp, #-60]
	cmp	r3, r2
	bcc	.L842
.L825:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L861:
	ldr	r1, [fp, #-64]
	mov	r2, #1
	mov	r4, r2
	strb	r1, [r5, #1]
	ldr	r3, [r9]
	str	r1, [r3, #52]
	ldr	r3, [r9]
	str	r1, [r3, #4]
	ldr	r1, [fp, #-56]
	ldr	r3, [r9]
	ldr	r0, [r1, #244]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r5, [r9]
	ldr	r0, [r5, #12]
	b	.L833
.L860:
	ldr	r1, [fp, #-64]
	mov	r2, #1
	mov	r4, #1
	strb	r1, [r5, #1]
	ldr	r3, [r9]
	str	r1, [r3, #4]
	ldr	r1, [fp, #-56]
	ldr	r3, [r9]
	ldr	r0, [r1, #244]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r5, [r9]
	ldr	r0, [r5, #12]
	b	.L833
.L862:
	ldr	r1, [fp, #-64]
	mov	r2, #1
	mov	r4, r2
	strb	r1, [r5, #1]
	ldr	r3, [r9]
	str	r1, [r3, #4]
	ldr	r1, [fp, #-56]
	ldr	r3, [r9]
	ldr	r0, [r1, #244]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r5, [r9]
	ldr	r3, [fp, #-68]
	ldr	r0, [r5, #12]
	ldr	ip, [r3, #1064]
	b	.L837
	UNWIND(.fnend)
	.size	HEVC_ApplyReferencePictureSet, .-HEVC_ApplyReferencePictureSet
	.align	2
	.global	HEVC_CheckThatAllRefPicsAreAvailable
	.type	HEVC_CheckThatAllRefPicsAreAvailable, %function
HEVC_CheckThatAllRefPicsAreAvailable:
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
	UNWIND(.pad #44)
	sub	sp, sp, #44
	add	r3, r0, #53248
	ldr	ip, [r0, #2320]
	movw	r6, #2024
	ldr	r2, [r3, #1092]
	add	lr, r0, #57344
	str	r1, [fp, #-84]
	movw	r5, #26164
	ldr	r1, [lr, #204]
	mov	r7, r0
	mla	ip, r6, r2, ip
	ldr	r6, [lr, #208]
	ldr	r4, [r0, #2316]
	add	r1, r6, r1
	ldr	r2, [lr, #212]
	str	r1, [fp, #-80]
	mov	lr, r1
	cmp	r1, r2
	ldr	ip, [ip, #32]
	str	r0, [fp, #-60]
	mov	r0, #1
	mla	r1, r5, ip, r4
	ldr	r10, [r1, #128]
	mov	r10, r0, asl r10
	bcs	.L864
	add	r9, lr, #14400
	ldr	r6, [r7, #2508]
	add	r9, r9, #18
	mov	r0, #0
	add	r2, r2, #57344
	add	r1, lr, #57344
	add	r9, r7, r9, lsl #2
	mov	ip, r0
	add	r2, r2, #241
	add	r1, r1, #241
	str	r0, [fp, #-64]
	mov	r0, r7
	add	r2, r7, r2
	str	ip, [fp, #-72]
	str	r2, [fp, #-68]
	add	r2, r7, r1
	str	r3, [fp, #-76]
	str	r2, [fp, #-56]
.L865:
	cmp	r6, #0
	beq	.L871
	ldr	r3, [fp, #-60]
	mov	r2, #0
	ldr	r8, [r9, #-1884]
	mov	r5, r2
	add	r4, r3, #2528
	str	r2, [fp, #-52]
	b	.L868
.L908:
	ldr	r3, [r4]
	ldr	r1, [r3, #52]
	cmp	r1, #0
	beq	.L867
	ldr	r0, [r3, #12]
	ldr	r1, [r9]
	cmp	r0, r1
	ldreqb	r3, [r3, #1]	@ zero_extendqisi2
	beq	.L906
.L867:
	add	r5, r5, #1
	add	r4, r4, #4
	cmp	r5, r6
	beq	.L907
.L868:
	cmp	r8, #0
	bne	.L908
	ldr	r7, [r4]
	mov	r1, r10
	ldr	r3, [r7, #52]
	cmp	r3, #0
	beq	.L867
	ldr	r0, [r7, #12]
	bl	__aeabi_idivmod
	ldr	r0, [r9]
	str	r1, [fp, #-48]
	mov	r1, r10
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-48]
	cmp	r3, r1
	bne	.L867
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
.L906:
	cmp	r3, #0
	add	r5, r5, #1
	ldr	r3, [fp, #-52]
	add	r4, r4, #4
	movne	r3, #1
	cmp	r5, r6
	str	r3, [fp, #-52]
	bne	.L868
.L907:
	ldr	r2, [fp, #-52]
	cmp	r2, #0
	beq	.L871
.L869:
	ldr	r3, [fp, #-56]
	add	r9, r9, #4
	ldr	r2, [fp, #-68]
	add	r3, r3, #1
	str	r3, [fp, #-56]
	cmp	r3, r2
	bne	.L865
	ldr	r2, [fp, #-80]
	ldr	r3, [fp, #-76]
	cmp	r2, #0
	beq	.L873
.L872:
	ldr	r2, [fp, #-60]
	mov	r7, #0
	ldr	r0, [fp, #-72]
	add	r4, r2, #57600
	add	r8, r2, #57344
	ldr	r10, [fp, #-80]
	add	r4, r4, #4
	add	r8, r8, #241
	mov	r9, r2
.L877:
	cmp	r6, #0
	beq	.L878
	add	lr, r9, #2512
	mov	r5, #0
	add	lr, lr, #12
	mov	r2, r5
	str	r0, [fp, #-48]
	b	.L875
.L874:
	cmp	r2, r6
	beq	.L909
.L875:
	ldr	r1, [lr, #4]!
	add	r2, r2, #1
	ldr	r0, [r1, #52]
	cmp	r0, #0
	bne	.L874
	ldr	ip, [r4]
	ldr	r0, [r3, #1064]
	add	r0, r0, ip
	ldr	ip, [r1, #12]
	cmp	ip, r0
	bne	.L874
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	cmp	r1, #0
	movne	r5, #1
	cmp	r2, r6
	bne	.L875
.L909:
	cmp	r5, #0
	ldr	r0, [fp, #-48]
	beq	.L878
.L876:
	add	r7, r7, #1
	add	r4, r4, #4
	cmp	r7, r10
	add	r8, r8, #1
	bcc	.L877
	str	r0, [fp, #-72]
.L873:
	ldr	r3, [fp, #-72]
	cmp	r3, #0
	ldrne	r3, [fp, #-84]
	movne	r0, #0
	ldrne	r2, [fp, #-64]
	moveq	r0, #1
	strne	r2, [r3]
.L905:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L878:
	ldr	r2, [r3, #1064]
	ldr	ip, [r4]
	ldr	r1, [r9, #200]
	add	r2, r2, ip
	cmp	r2, r1
	blt	.L876
	ldrb	r1, [r8]	@ zero_extendqisi2
	cmp	r1, #0
	ldr	r1, [fp, #-64]
	movne	r0, #1
	movne	r1, r2
	str	r1, [fp, #-64]
	b	.L876
.L871:
	ldr	r3, [fp, #-76]
	ldr	r0, [r9, #-68]
	ldr	r2, [r3, #1064]
	ldr	r3, [fp, #-60]
	add	r2, r2, r0
	ldr	r1, [r3, #200]
	cmp	r2, r1
	blt	.L869
	ldr	r3, [fp, #-56]
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, [fp, #-64]
	cmp	r1, #0
	movne	r3, r2
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-72]
	movne	r3, #1
	str	r3, [fp, #-72]
	b	.L869
.L864:
	ldr	r2, [fp, #-80]
	cmp	r2, #0
	beq	.L905
	ldr	r1, [fp, #-60]
	mov	r2, #0
	str	r2, [fp, #-64]
	str	r2, [fp, #-72]
	ldr	r6, [r1, #2508]
	b	.L872
	UNWIND(.fnend)
	.size	HEVC_CheckThatAllRefPicsAreAvailable, .-HEVC_CheckThatAllRefPicsAreAvailable
	.align	2
	.global	HEVC_SetFrmRepeatCount
	.type	HEVC_SetFrmRepeatCount, %function
HEVC_SetFrmRepeatCount:
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
	ldr	r3, [r0, #2324]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r1, #24]
	ldmccfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVC_SetFrmRepeatCount.part.3
	UNWIND(.fnend)
	.size	HEVC_SetFrmRepeatCount, .-HEVC_SetFrmRepeatCount
	.align	2
	.global	HEVC_GetFreeFrameStoreNum
	.type	HEVC_GetFreeFrameStoreNum, %function
HEVC_GetFreeFrameStoreNum:
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
	add	r2, r0, #67584
	add	r4, r0, #81920
	add	r2, r2, #600
	add	r4, r4, #344
	mov	r5, #0
.L917:
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne	.L913
	cmp	r2, #0
	beq	.L914
	ldr	lr, [r0, #2508]
	cmp	lr, #0
	beq	.L914
	ldr	r1, [r0, #2528]
	cmp	r1, r2
	beq	.L913
	add	r1, r0, #2528
	b	.L915
.L916:
	ldr	ip, [r1, #4]!
	cmp	ip, r2
	beq	.L913
.L915:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L916
.L914:
	add	r5, r5, #1
.L913:
	add	r2, r2, #704
	cmp	r2, r4
	bne	.L917
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_GetFreeFrameStoreNum, .-HEVC_GetFreeFrameStoreNum
	.align	2
	.global	HEVC_UpdateReflist
	.type	HEVC_UpdateReflist, %function
HEVC_UpdateReflist:
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
	add	r10, r0, #57344
	mov	r5, r0
	ldr	r1, [r10, #204]
	cmp	r1, #0
	beq	.L951
	add	r8, r0, #57600
	mov	r7, #0
	add	r9, r10, #240
	add	r8, r8, #4
	add	ip, r0, #53248
	mov	r6, r7
.L935:
	ldrb	r3, [r9, #1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L929
	ldr	r4, [r5, #2508]
	ldr	lr, [ip, #1064]
	ldr	r3, [r8]
	cmp	r4, #0
	add	lr, lr, r3
	beq	.L929
	ldr	r1, [r5, #2528]
	ldr	r3, [r1, #12]
	cmp	r3, lr
	addne	r2, r5, #2528
	movne	r3, #0
	bne	.L932
	b	.L983
.L934:
	ldr	r1, [r2, #4]!
	ldr	r0, [r1, #12]
	cmp	r0, lr
	beq	.L930
.L932:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L934
.L929:
	ldr	r1, [r10, #204]
	add	r6, r6, #1
	add	r8, r8, #4
	cmp	r1, r6
	bhi	.L935
.L928:
	ldr	r2, [r5, #2504]
	str	r7, [r5, #2520]
	cmp	r2, r7
	addhi	r3, r7, #648
	movhi	r0, #0
	addhi	r3, r5, r3, lsl #2
	bls	.L940
.L939:
	add	r7, r7, #1
	str	r0, [r3, #4]!
	cmp	r7, r2
	bne	.L939
.L940:
	ldr	r8, [r10, #208]
	add	r8, r1, r8
	cmp	r8, r6
	movls	r7, #0
	bls	.L938
	add	r9, r6, #14400
	add	r10, r6, #57344
	add	r9, r9, #1
	add	r10, r10, #240
	add	r10, r5, r10
	add	ip, r5, #53248
	add	r9, r5, r9, lsl #2
	mov	r7, #0
.L947:
	ldrb	r3, [r10, #1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L941
	ldr	r4, [r5, #2508]
	ldr	lr, [ip, #1064]
	ldr	r3, [r9]
	cmp	r4, #0
	add	lr, lr, r3
	beq	.L941
	ldr	r1, [r5, #2528]
	ldr	r3, [r1, #12]
	cmp	r3, lr
	addne	r2, r5, #2528
	movne	r3, #0
	bne	.L944
	b	.L984
.L946:
	ldr	r1, [r2, #4]!
	ldr	r0, [r1, #12]
	cmp	r0, lr
	beq	.L942
.L944:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L946
.L941:
	add	r6, r6, #1
	add	r9, r9, #4
	cmp	r8, r6
	bhi	.L947
.L987:
	ldr	r2, [r5, #2504]
.L938:
	cmp	r2, r7
	str	r7, [r5, #2524]
	bls	.L927
	add	r1, r7, #664
	mov	r3, #0
	add	r1, r1, #1
	add	r5, r5, r1, lsl #2
.L949:
	add	r7, r7, #1
	str	r3, [r5, #4]!
	cmp	r7, r2
	bne	.L949
.L927:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L983:
	mov	r3, #0
.L930:
	add	r4, r5, r3, lsl #2
	mov	r2, #1
	strb	r2, [r1, #1]
	ldr	r3, [r4, #2528]
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L985
.L933:
	mov	r2, #1
	str	r2, [r3, #56]
	ldr	r2, [r4, #2528]
	mov	r1, #0
	add	r3, r5, r7, lsl #2
	add	r7, r7, #1
	str	r1, [r2, #52]
	ldr	r2, [r4, #2528]
	str	r2, [r3, #2596]
	b	.L929
.L984:
	mov	r3, #0
.L942:
	add	r4, r5, r3, lsl #2
	mov	r2, #1
	strb	r2, [r1, #1]
	ldr	r3, [r4, #2528]
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L986
.L945:
	mov	r2, #1
	str	r2, [r3, #56]
	ldr	r2, [r4, #2528]
	add	r3, r7, #664
	mov	r1, #0
	add	r6, r6, #1
	add	r3, r3, #2
	cmp	r8, r6
	str	r1, [r2, #52]
	add	r7, r7, #1
	ldr	r2, [r4, #2528]
	add	r9, r9, #4
	str	r2, [r5, r3, asl #2]
	bhi	.L947
	b	.L987
.L985:
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #244]
	str	ip, [fp, #-48]
	bl	FSP_SetRef
	ldr	r3, [r4, #2528]
	ldr	ip, [fp, #-48]
	b	.L933
.L986:
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #244]
	str	ip, [fp, #-48]
	bl	FSP_SetRef
	ldr	r3, [r4, #2528]
	ldr	ip, [fp, #-48]
	b	.L945
.L951:
	mov	r7, r1
	mov	r6, r1
	b	.L928
	UNWIND(.fnend)
	.size	HEVC_UpdateReflist, .-HEVC_UpdateReflist
	.align	2
	.global	HEVC_UpdateLTReflist
	.type	HEVC_UpdateLTReflist, %function
HEVC_UpdateLTReflist:
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
	add	r3, r0, #53248
	ldr	r2, [r0, #2320]
	movw	ip, #2024
	ldr	r3, [r3, #1092]
	add	r10, r0, #57344
	ldr	lr, [r0, #2316]
	movw	r4, #26164
	ldr	r5, [r10, #208]
	mov	r1, #1
	mla	ip, ip, r3, r2
	ldr	r2, [r10, #204]
	ldr	r3, [r10, #212]
	add	r2, r2, r5
	sub	r2, r2, #1
	sub	r9, r3, #1
	cmp	r9, r2
	ldr	ip, [ip, #32]
	mla	r2, r4, ip, lr
	ldr	r8, [r2, #128]
	mov	r8, r1, asl r8
	ble	.L999
	add	r7, r3, #14400
	add	r3, r3, #57344
	add	r7, r7, #17
	add	r3, r3, #241
	add	r3, r0, r3
	sub	r8, r8, #1
	add	r7, r0, r7, lsl #2
	mov	r6, #0
	str	r6, [fp, #-48]
	mov	r6, r3
	str	r1, [fp, #-52]
.L995:
	ldrb	r3, [r6, #-1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L990
	ldrb	r4, [r7, #-1884]	@ zero_extendqisi2
	ldr	lr, [r7]
	ldr	r5, [r0, #2508]
	cmp	r4, #0
	andeq	lr, lr, r8
	cmp	r5, #0
	ble	.L990
	add	r1, r0, #2512
	mov	r3, #0
	add	r1, r1, #12
	b	.L994
.L993:
	add	r3, r3, #1
	cmp	r3, r5
	beq	.L990
.L994:
	ldr	ip, [r1, #4]!
	cmp	r4, #0
	ldr	r2, [ip, #12]
	andeq	r2, r2, r8
	cmp	lr, r2
	bne	.L993
	ldr	r1, [fp, #-48]
	add	r3, r0, r3, lsl #2
	ldr	lr, [fp, #-52]
	add	r2, r0, r1, lsl #2
	add	r1, r1, #1
	strb	lr, [ip, #1]
	mov	ip, #0
	str	r1, [fp, #-48]
	ldr	r1, [r3, #2528]
	str	ip, [r1, #56]
	ldr	r1, [r3, #2528]
	str	lr, [r1, #52]
	ldr	r3, [r3, #2528]
	str	r3, [r2, #2732]
.L990:
	ldr	r3, [r10, #204]
	sub	r9, r9, #1
	ldr	r2, [r10, #208]
	sub	r7, r7, #4
	add	r3, r3, r2
	sub	r3, r3, #1
	cmp	r3, r9
	blt	.L995
	ldr	r6, [fp, #-48]
	mov	r2, r6
.L989:
	ldr	r3, [r0, #2504]
	str	r2, [r0, #2516]
	cmp	r3, r6
	ble	.L988
	add	r1, r6, #680
	mov	r2, #0
	add	r1, r1, #2
	add	r0, r0, r1, lsl #2
.L997:
	add	r6, r6, #1
	str	r2, [r0, #4]!
	cmp	r6, r3
	bne	.L997
.L988:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L999:
	mov	r2, #0
	mov	r6, r2
	b	.L989
	UNWIND(.fnend)
	.size	HEVC_UpdateLTReflist, .-HEVC_UpdateLTReflist
	.align	2
	.global	HEVC_GetAPC
	.type	HEVC_GetAPC, %function
HEVC_GetAPC:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	subs	r6, r1, #0
	mov	r5, r0
	beq	.L1007
	ldr	ip, [r0, #2800]
	cmp	ip, #0
	beq	.L1016
	ldr	r4, [r0, #2808]
	cmp	r4, #0
	addne	r3, r0, #2800
	movne	r4, #0
	addne	r3, r3, #8
	bne	.L1011
	b	.L1009
.L1014:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L1009
.L1011:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L1014
.L1016:
	mvn	r0, #6
.L1020:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1009:
	ldr	r1, [r6, #32]
	ldr	r0, [r5, #244]
	bl	FSP_GetLogicFs
	subs	ip, r0, #0
	beq	.L1012
	ldr	r3, [ip, #640]
	cmp	r3, #0
	beq	.L1012
	add	r2, r5, r4, lsl #2
	mov	r0, #1
	mov	r3, r4
	ldr	r1, .L1021
	str	r0, [r2, #2808]
	mov	r0, #13
	ldr	ip, [ip, #640]
	ldr	ip, [ip, #8]
	str	ip, [r2, #2872]
	ldr	ip, [r6, #12]
	str	ip, [r2, #2936]
	ldr	r2, [r5, #2804]
	add	r2, r2, #1
	str	r2, [r5, #2804]
	ldr	r2, [r6, #228]
	bl	dprint_vfmw
	mov	r0, #0
	str	r4, [r6, #28]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1012:
	ldrb	r1, [r6, #1]	@ zero_extendqisi2
	mov	r0, #0
	ldr	r2, [r6, #32]
	ldr	r3, [r6, #12]
	stmia	sp, {r1, ip}
	str	r0, [sp, #8]
	ldr	r1, .L1021+4
	bl	dprint_vfmw
	mvn	r0, #6
	b	.L1020
.L1007:
	mov	r0, r6
	movw	r3, #9276
	ldr	r2, .L1021+8
	ldr	r1, .L1021+12
	bl	dprint_vfmw
	mvn	r0, #6
	b	.L1020
.L1022:
	.align	2
.L1021:
	.word	.LC83
	.word	.LC84
	.word	.LC45
	.word	.LC46
	UNWIND(.fnend)
	.size	HEVC_GetAPC, .-HEVC_GetAPC
	.align	2
	.global	HEVC_CheckFrameStore
	.type	HEVC_CheckFrameStore, %function
HEVC_CheckFrameStore:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	mov	r4, r1
	ldr	r0, [r0, #244]
	ldr	r1, [r1, #32]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1024
	ldrsb	r2, [r0, #1]
	cmp	r2, #3
	cmpne	r2, #0
	moveq	r0, #1
	movne	r0, #0
	beq	.L1043
	ldr	r3, [r4, #8]
	cmp	r3, #1
	beq	.L1030
	ldr	r2, [r4, #4]
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1029
	cmp	r3, #1
	beq	.L1029
.L1040:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L1043:
	ldr	r3, [r4, #32]
	mov	r0, #13
	str	r2, [sp]
	ldr	r1, .L1044
	ldr	r2, .L1044+4
	bl	dprint_vfmw
.L1026:
	ldr	r3, [r4, #8]
	cmp	r3, #1
	beq	.L1030
	ldr	r3, [r4, #12]
	mov	r0, #1
	ldr	r2, .L1044+4
	ldr	r1, .L1044+8
	bl	dprint_vfmw
	mvn	r0, #2
	b	.L1040
.L1024:
	ldr	lr, [r4, #32]
	mov	r2, r4
	ldr	ip, [r4, #8]
	mov	r0, #1
	ldr	r3, [r4, #12]
	ldr	r1, .L1044+12
	stmia	sp, {ip, lr}
	bl	dprint_vfmw
	b	.L1026
.L1029:
	str	r3, [sp, #4]
	mov	r0, #1
	str	r2, [sp]
	mov	r3, r4
	ldr	r2, .L1044+4
	ldr	r1, .L1044+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1040
.L1030:
	mov	r2, #1
	mov	r3, #2
	strb	r2, [r4, #2]
	mvn	r0, #2
	str	r3, [r4, #8]
	b	.L1040
.L1045:
	.align	2
.L1044:
	.word	.LC85
	.word	.LANCHOR1+332
	.word	.LC87
	.word	.LC86
	.word	.LC88
	UNWIND(.fnend)
	.size	HEVC_CheckFrameStore, .-HEVC_CheckFrameStore
	.align	2
	.global	HEVC_GetImagePara
	.type	HEVC_GetImagePara, %function
HEVC_GetImagePara:
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
	ldr	r1, [r1, #180]
	add	r5, r0, #24576
	mov	r3, #25600
	bic	r1, r1, #16128
	str	r3, [r4, #208]
	bic	r1, r1, #3
	str	r1, [r4, #180]
	ldr	r3, [r5, #2832]
	ldr	lr, [r4, #16]
	add	r3, r3, #12288
	ldr	r2, [r3, #180]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #256
	moveq	ip, #0
	beq	.L1047
	mov	r3, #256
	movt	r3, 1
	cmp	r2, r3
	movne	ip, #1
	bne	.L1047
	add	r3, r0, #4096
	ldr	ip, [r3, #448]
	subs	ip, ip, #1
	movne	ip, #1
.L1047:
	ldr	r3, [r5, #2836]
	ldrb	r3, [r3, #2633]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r2, #3072
	beq	.L1048
	ldr	r3, [r4, #696]
	cmp	r3, #1
	cmpne	r3, #11
	moveq	r2, #1024
	bne	.L1067
.L1049:
	ldr	r3, [r4, #700]
	clz	ip, r3
	mov	ip, ip, lsr #5
.L1048:
	and	r3, lr, #3
	ldr	lr, [r4, #4]
	orr	r1, r3, r1
	orr	r3, r1, r2
	ldr	r1, [r4, #32]
	orr	r3, r3, ip, asl #8
	bic	r3, r3, #131072
	orr	r3, r3, #8192
	str	r3, [r4, #180]
	ldr	r2, [r0, #2324]
	ldr	r2, [r2, #592]
	cmp	r2, #1
	moveq	r2, #262144
	movne	r2, #0
	cmp	lr, #0
	orr	r3, r2, r3
	str	r3, [r4, #180]
	ldrne	r3, [r4, #20]
	strne	r3, [r4, #232]
	ldr	r0, [r0, #244]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L1068
	mov	r2, #624
	add	r1, r4, #72
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #16
	str	r3, [r5, #376]
	mov	r3, #1
	ldr	r2, [r4, #48]
	strb	r3, [r5, #371]
	cmp	r2, #0
	strb	r3, [r5, #369]
	moveq	r3, #0
	movne	r3, #4
	strb	r3, [r5, #364]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1067:
	bic	r3, r3, #8
	cmp	r3, #2
	moveq	r2, #2048
	movne	r2, #3072
	b	.L1049
.L1069:
	.align	2
.L1068:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_GetImagePara, .-HEVC_GetImagePara
	.align	2
	.global	HEVC_OutputCurrPic
	.type	HEVC_OutputCurrPic, %function
HEVC_OutputCurrPic:
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
	add	r6, r0, #53248
	mov	r4, r0
	mov	r2, #624
	ldr	r5, [r6, #388]
	add	r1, r6, #392
	ldr	r3, [r6, #376]
	add	r0, r5, #72
	str	r3, [r5, #20]
	bl	memcpy
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_GetImagePara
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L1092
	ldr	r2, [r5, #20]
	cmp	r2, #0
	str	r2, [r7, #160]
	bne	.L1073
.L1078:
	ldrb	r3, [r6, #336]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1093
.L1075:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_SetDisplay
	add	r3, r4, #61696
	ldr	r0, [r4, #244]
	add	r3, r3, #32
	str	r7, [sp]
	mov	r2, r4
	mov	r1, #16
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L1094
	ldr	r3, [r4, #2324]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r7, #24]
	bcs	.L1095
.L1082:
	ldr	r2, [r4, #284]
	mov	r0, #0
	ldr	r3, [r4, #276]
	add	r2, r2, #1
	str	r2, [r4, #284]
	add	r3, r3, #1
	str	r3, [r4, #276]
.L1072:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1073:
	ldr	r8, [r6, #348]
	cmp	r8, #0
	beq	.L1096
.L1076:
	ldr	r3, [r4, #2324]
	ldr	r3, [r3, #4]
	cmp	r3, r2
	bcs	.L1078
	ldr	r1, [r5, #32]
	mov	r2, #0
	ldr	r0, [r4, #244]
	bl	FSP_SetDisplay
	ldr	r1, [r4, #2324]
	ldr	r3, [r7, #160]
	mov	r0, #1
	ldr	r2, .L1097
	ldr	ip, [r1, #4]
	ldr	r1, .L1097+4
	str	ip, [sp]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1072
.L1093:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_SetRef
	b	.L1075
.L1095:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_SetFrmRepeatCount.part.3
	b	.L1082
.L1096:
	ldr	r2, .L1097
	mov	r0, #1
	ldr	r1, .L1097+8
	bl	dprint_vfmw
	ldr	r3, .L1097+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1077
	mov	r3, r8
	mov	r2, r8
	mov	r1, #111
	ldr	r0, [r4, #244]
	blx	ip
.L1077:
	ldr	r2, [r7, #160]
	cmp	r2, #0
	bne	.L1076
	b	.L1078
.L1094:
	ldr	r2, .L1097
	mov	r0, #1
	ldr	r1, .L1097+16
	bl	dprint_vfmw
	ldr	r0, [r4, #244]
	ldr	r1, [r5, #32]
	mov	r2, #0
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L1072
.L1092:
	ldr	r2, .L1097
	ldr	r1, .L1097+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1072
.L1098:
	.align	2
.L1097:
	.word	.LANCHOR1+356
	.word	.LC92
	.word	.LC90
	.word	g_event_report
	.word	.LC91
	.word	.LC89
	UNWIND(.fnend)
	.size	HEVC_OutputCurrPic, .-HEVC_OutputCurrPic
	.align	2
	.global	HEVC_FirstFrameFastOut
	.type	HEVC_FirstFrameFastOut, %function
HEVC_FirstFrameFastOut:
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
	bl	HEVC_OutputCurrPic
	subs	r5, r0, #0
	bne	.L1102
	add	r3, r4, #53248
	add	r1, r4, #61440
	mov	r2, #1
	mov	ip, #2
	ldr	r0, [r3, #388]
	str	ip, [r0, #8]
	mov	r0, r5
	str	r2, [r1, #640]
	ldr	r1, [r3, #344]
	str	r1, [r4, #212]
	ldr	r3, [r3, #388]
	strb	r2, [r3, #2]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1102:
	ldr	r2, .L1103
	mov	r0, #1
	ldr	r1, .L1103+4
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1104:
	.align	2
.L1103:
	.word	.LANCHOR1+376
	.word	.LC93
	UNWIND(.fnend)
	.size	HEVC_FirstFrameFastOut, .-HEVC_FirstFrameFastOut
	.align	2
	.global	HEVC_CheckOutputImage
	.type	HEVC_CheckOutputImage, %function
HEVC_CheckOutputImage:
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
	cmp	r1, #19
	bhi	.L1107
	mov	r3, #704
	mla	r1, r3, r1, r0
	add	r1, r1, #67584
	ldrb	r0, [r1, #601]	@ zero_extendqisi2
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1107:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_CheckOutputImage, .-HEVC_CheckOutputImage
	.align	2
	.global	HEVC_DECRecycleImage
	.type	HEVC_DECRecycleImage, %function
HEVC_DECRecycleImage:
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
	mov	r5, r0
	ldr	r0, [r0, #244]
	mov	r8, r1
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1113
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	addne	r4, r0, #560
	addne	r9, r0, #592
	movne	r6, #0
	movne	r7, #0
	beq	.L1118
.L1112:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1119
.L1111:
	cmp	r4, r9
	bne	.L1112
	ldr	r0, [r5, #244]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1119:
	mov	r1, r2
	ldr	r0, [r5, #244]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L1111
.L1118:
	ldr	r1, .L1120
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1113:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1121:
	.align	2
.L1120:
	.word	.LC94
	UNWIND(.fnend)
	.size	HEVC_DECRecycleImage, .-HEVC_DECRecycleImage
	.align	2
	.global	HEVC_ArrangeVahbMem
	.type	HEVC_ArrangeVahbMem, %function
HEVC_ArrangeVahbMem:
	UNWIND(.fnstart)
	@ args = 12, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	ldr	r3, .L1132
	mov	r5, r1
	mov	r6, r2
	mov	r4, r0
	mov	r2, #32
	ldr	r3, [r3, #48]
	sub	r0, fp, #60
	mov	r1, #0
	blx	r3
	ldr	r3, [r4, #2324]
	str	r6, [fp, #-32]
	mov	r0, #0
	str	r5, [fp, #-52]
	ldr	r3, [r3, #28]
	str	r6, [fp, #-48]
	cmp	r3, #24
	str	r5, [fp, #-44]
	str	r6, [fp, #-40]
	str	r5, [fp, #-36]
	strb	r0, [fp, #-60]
	strb	r0, [fp, #-59]
	beq	.L1123
	ldr	r2, [r4, #2504]
	mov	r1, #5
	ldrb	r3, [fp, #12]	@ zero_extendqisi2
	strb	r0, [fp, #-56]
	strb	r1, [fp, #-55]
	strb	r2, [fp, #-57]
.L1128:
	sub	r1, fp, #60
	ldr	r0, [r4, #244]
	strb	r3, [fp, #-54]
	mov	r3, #0
	strb	r3, [fp, #-58]
	bl	FSP_ConfigInstance
	cmp	r0, #0
	bne	.L1124
	ldr	r1, [r4, #2324]
	sub	r3, fp, #64
	ldr	r0, [r4, #244]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L1131
	cmp	r0, #0
	moveq	r0, #1
	bne	.L1126
.L1130:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1123:
	mov	r3, #1
	mov	r2, #5
	strb	r0, [fp, #-56]
	strb	r3, [fp, #-57]
	strb	r2, [fp, #-55]
	b	.L1128
.L1131:
	add	r0, r4, #61696
	add	r0, r0, #32
	bl	ResetVoQueue
.L1126:
	ldr	r1, .L1132+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L1130
.L1124:
	ldr	r1, .L1132+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L1130
.L1133:
	.align	2
.L1132:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC95
	.word	.LC96
	UNWIND(.fnend)
	.size	HEVC_ArrangeVahbMem, .-HEVC_ArrangeVahbMem
	.align	2
	.global	HEVC_ConvertHDRInfo
	.type	HEVC_ConvertHDRInfo, %function
HEVC_ConvertHDRInfo:
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
	ldrb	ip, [r1, #4]	@ zero_extendqisi2
	mov	r3, r0
	add	lr, r0, #256
	add	r4, r0, #264
	add	lr, lr, #2
	mov	r0, r2
	strb	ip, [r3, #256]
	mov	ip, #0
	ldr	r5, [r1, #40]
	strb	r5, [r3, #257]
	ldr	r5, [r1, #48]
	strb	r5, [r3, #258]
	ldr	r1, [r1, #44]
	str	r1, [r3, #252]
	ldrb	r1, [r2, #3432]	@ zero_extendqisi2
	strb	r1, [r3, #248]
.L1135:
	ldr	r1, [r0, #3436]
	add	ip, ip, #1
	cmp	ip, #3
	add	r0, r0, #4
	strh	r1, [lr, #2]!	@ movhi
	ldr	r1, [r0, #3444]
	strh	r1, [r4, #2]!	@ movhi
	bne	.L1135
	ldr	r4, [r2, #3460]
	add	r1, r3, #272
	movw	lr, #274
	movw	ip, #3478
	movw	r0, #3480
	strh	r4, [r1]	@ movhi
	ldr	r1, [r2, #3464]
	strh	r1, [r3, lr]	@ movhi
	ldr	r1, [r2, #3468]
	str	r1, [r3, #276]
	ldr	r1, [r2, #3472]
	str	r1, [r3, #280]
	ldrb	r1, [r2, #3476]	@ zero_extendqisi2
	strb	r1, [r3, #249]
	ldrh	r1, [r2, ip]
	str	r1, [r3, #284]
	ldrh	r1, [r2, r0]
	str	r1, [r3, #288]
	ldrb	r1, [r2, #3482]	@ zero_extendqisi2
	strb	r1, [r3, #292]
	ldrb	r1, [r2, #3224]	@ zero_extendqisi2
	cmp	r1, #1
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	strb	r1, [r3, #296]
	add	r1, r2, #3216
	ldr	lr, [r2, #3264]
	add	r0, r3, #296
	ldr	ip, .L1138
	add	r1, r1, #12
	mov	r2, #36
	str	lr, [r3, #336]
	ldr	r3, [ip, #52]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L1139:
	.align	2
.L1138:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_ConvertHDRInfo, .-HEVC_ConvertHDRInfo
	.align	2
	.global	HEVC_AllocFrameStore
	.type	HEVC_AllocFrameStore, %function
HEVC_AllocFrameStore:
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
	add	r6, r0, #53248
	ldr	r3, [r0, #2320]
	mov	r7, r0
	movw	r0, #2024
	ldr	r2, [r6, #1092]
	movw	r5, #26164
	ldr	ip, [r7, #2316]
	add	lr, r7, #67584
	mov	r1, #0
	add	lr, lr, #600
	mla	r3, r0, r2, r3
	mov	r0, r1
	ldr	r3, [r3, #32]
	strb	r1, [r6, #337]
	mla	r5, r5, r3, ip
.L1145:
	ldr	r3, [lr, #4]
	cmp	r3, #0
	bne	.L1141
	cmp	lr, #0
	beq	.L1152
	ldr	r1, [r7, #2508]
	cmp	r1, #0
	beq	.L1152
	ldr	r2, [r7, #2528]
	cmp	r2, lr
	beq	.L1141
	add	r2, r7, #2528
	b	.L1143
.L1144:
	ldr	ip, [r2, #4]!
	cmp	ip, lr
	beq	.L1141
.L1143:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L1144
.L1152:
	mov	r8, #704
	mov	r4, lr
	mla	r8, r8, r0, r7
	mov	r3, #101
	mov	r2, #624
	str	r3, [r6, #376]
	add	r0, r6, #392
	mov	r3, #1
	add	r8, r8, #67584
	strb	r3, [r6, #337]
	add	r1, r8, #672
	mov	r9, #0
	bl	memcpy
	str	r4, [r6, #388]
	str	r9, [r6, #552]
	str	r9, [r8, #620]
	strb	r9, [r8, #602]
	strb	r9, [r4]
	ldr	r3, [r6, #388]
	str	r9, [r3, #52]
	ldr	r3, [r6, #388]
	str	r9, [r3, #56]
	ldr	r3, [r6, #388]
	str	r9, [r3, #8]
	ldrb	r1, [r6, #337]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L1149
	ldr	r0, [r7, #244]
	bl	FSP_NewLogicFs
	mov	r8, r0
	mov	r1, r0
	ldr	r0, [r7, #244]
	bl	FSP_GetLogicFs
	mvn	r3, r8
	mov	r3, r3, lsr #31
	cmp	r0, #0
	mov	r4, r0
	moveq	r3, #0
	cmp	r3, #0
	beq	.L1147
	add	r2, r7, #3440
	add	r1, r5, #2624
	add	r2, r2, #8
	bl	HEVC_ConvertHDRInfo
	ldr	r3, [r7, #2324]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L1156
.L1148:
	ldr	r5, .L1157
	add	r1, r4, #8
	ldr	r0, [r6, #388]
	mov	r2, #624
	ldr	r3, [r5, #52]
	add	r0, r0, #72
	blx	r3
	ldr	r1, [r6, #388]
	add	r0, r7, #53504
	ldr	r3, [r5, #52]
	add	r1, r1, #72
	mov	r2, #624
	add	r0, r0, #136
	blx	r3
	ldr	r3, [r6, #388]
	mov	r0, #0
	str	r8, [r3, #32]
	ldrsb	r3, [r4, #4]
	str	r3, [r6, #372]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1141:
	add	r0, r0, #1
	add	lr, lr, #704
	cmp	r0, #20
	bne	.L1145
.L1149:
	ldr	r1, .L1157+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1156:
	add	r3, r7, #24576
	ldr	r5, [r4, #148]
	ldr	r0, [r4, #52]
	ldr	r2, [r3, #2836]
	ldr	r3, [r2, #76]
	ldr	r2, [r2, #72]
	mul	r5, r5, r3
	str	r2, [r4, #120]
	str	r3, [r4, #124]
	add	r0, r5, r0
	str	r0, [r4, #56]
	bl	MEM_Phy2Vir
	ldr	r2, [r4, #36]
	ldr	r3, [r4, #148]
	str	r9, [r4, #524]
	str	r0, [r4, #520]
	ldr	r0, [r4, #68]
	add	r0, r5, r0
	add	r5, r5, r2
	str	r0, [r4, #72]
	str	r5, [r4, #40]
	add	r5, r5, r3
	str	r5, [r4, #48]
	bl	MEM_Phy2Vir
	str	r9, [r4, #540]
	str	r0, [r4, #536]
	b	.L1148
.L1147:
	strb	r3, [r6, #337]
	mov	r0, r3
	ldr	r1, .L1157+8
	bl	dprint_vfmw
	mvn	r0, #3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1158:
	.align	2
.L1157:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC97
	.word	.LC98
	UNWIND(.fnend)
	.size	HEVC_AllocFrameStore, .-HEVC_AllocFrameStore
	.align	2
	.global	HEVC_IsVoFinish
	.type	HEVC_IsVoFinish, %function
HEVC_IsVoFinish:
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
	mov	r5, r0
	ldr	r0, [r0, #244]
	bl	FSP_GetFspType
	cmn	r0, #1
	mov	r4, r0
	movne	r4, #0
	bne	.L1163
	b	.L1166
.L1162:
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L1164
	cmp	r4, #32
	beq	.L1167
.L1163:
	mov	r1, r4
	ldr	r0, [r5, #244]
	bl	FSP_GetLogicFs
	add	r4, r4, #1
	cmp	r0, #0
	bne	.L1162
	ldr	r1, .L1168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1166:
	ldr	r1, .L1168+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1164:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1167:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1169:
	.align	2
.L1168:
	.word	.LC100
	.word	.LC99
	UNWIND(.fnend)
	.size	HEVC_IsVoFinish, .-HEVC_IsVoFinish
	.align	2
	.global	HEVC_GetBackPicFromVOQueue
	.type	HEVC_GetBackPicFromVOQueue, %function
HEVC_GetBackPicFromVOQueue:
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
	mov	r5, r0
	ldr	r0, [r0, #244]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1184
.L1171:
	add	r9, r5, #68608
	add	r8, r5, #81920
	add	r9, r9, #232
	add	r8, r8, #1000
	mov	r6, #0
	mov	r7, #0
.L1174:
	sub	r4, r9, #32
	mov	r3, #0
	strb	r3, [r9, #-655]
	str	r3, [r9, #-652]
.L1173:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1185
.L1172:
	cmp	r4, r9
	bne	.L1173
	add	r9, r4, #704
	cmp	r9, r8
	bne	.L1174
	ldr	r2, [r5, #180]
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r3, #0
	add	r5, r5, #2112
	mov	r1, r3
.L1176:
	add	r3, r3, #1
	str	r1, [r5, #4]!
	cmp	r3, r2
	bne	.L1176
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1185:
	mov	r1, r2
	ldr	r0, [r5, #244]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L1172
.L1184:
	add	r0, r5, #61696
	add	r0, r0, #32
	bl	ResetVoQueue
	b	.L1171
	UNWIND(.fnend)
	.size	HEVC_GetBackPicFromVOQueue, .-HEVC_GetBackPicFromVOQueue
	.align	2
	.global	HEVC_RemoveFrameStoreOutDPB
	.type	HEVC_RemoveFrameStoreOutDPB, %function
HEVC_RemoveFrameStoreOutDPB:
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
	add	r6, r0, r1, lsl #2
	mov	r7, #0
	mov	r4, r1
	mov	r5, r0
	ldr	r3, [r6, #2528]
	str	r7, [r3, #4]
	ldr	r3, [r6, #2528]
	ldr	r0, [r0, #244]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1187
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L1188
	ldr	r3, [r6, #2528]
	mov	r2, r7
	ldr	r0, [r5, #244]
	ldr	r1, [r3, #32]
	bl	FSP_SetDisplay
.L1188:
	ldr	r3, [r6, #2528]
	mov	r2, #0
	ldr	r0, [r5, #244]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
.L1187:
	ldr	r1, [r5, #2508]
	sub	r1, r1, #1
	cmp	r4, r1
	addls	r3, r4, #632
	addls	r3, r5, r3, lsl #2
	bhi	.L1191
.L1190:
	add	r4, r4, #1
	ldr	r2, [r3, #4]!
	cmp	r4, r1
	str	r2, [r3, #-4]
	bls	.L1190
.L1191:
	add	r3, r1, #632
	mov	r2, #0
	str	r2, [r5, r3, asl #2]
	str	r1, [r5, #2508]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_RemoveFrameStoreOutDPB, .-HEVC_RemoveFrameStoreOutDPB
	.align	2
	.global	HEVC_RemoveDummyFrame
	.type	HEVC_RemoveDummyFrame, %function
HEVC_RemoveDummyFrame:
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
	ldr	r3, [r0, #2508]
	mov	r5, r0
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r7, #0
	mov	r4, r7
	mov	r6, r7
	b	.L1203
.L1202:
	ldr	r3, [r5, #2508]
	add	r4, r4, #1
	cmp	r4, r3
	mov	r7, r4
	bcs	.L1209
.L1203:
	add	r3, r4, #632
	ldr	r3, [r5, r3, asl #2]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1202
	mov	r1, r7
	mov	r0, r5
	strb	r6, [r3]
	sub	r4, r4, #1
	bl	HEVC_RemoveApcStore
	mov	r1, r7
	mov	r0, r5
	bl	HEVC_RemoveFrameStoreOutDPB
	b	.L1202
.L1209:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_RemoveDummyFrame, .-HEVC_RemoveDummyFrame
	.align	2
	.global	HEVC_RemoveUnUsedFrameStore
	.type	HEVC_RemoveUnUsedFrameStore, %function
HEVC_RemoveUnUsedFrameStore:
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
	ldr	r5, [r0, #2508]
	mov	r7, r0
	cmp	r5, #0
	beq	.L1219
	mov	r6, #0
	mvn	r8, #0
	mov	r10, r6
	mov	r9, r6
	b	.L1218
.L1229:
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	cmpne	r3, #3
	ldr	r3, [r4, #2528]
	bne	.L1214
	ldr	r0, [r3, #8]
	cmp	r0, #1
	beq	.L1214
	str	r8, [r3, #32]
	mov	r2, #1
	ldr	r3, [r4, #2528]
	strb	r1, [r3, #1]
	ldr	r3, [r4, #2528]
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2528]
.L1214:
	ldr	r3, [r3]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	addne	r10, r10, #1
	beq	.L1228
	add	r9, r9, #1
	cmp	r9, r5
	beq	.L1211
.L1218:
	add	r4, r7, r10, lsl #2
	ldr	r0, [r7, #244]
	ldr	r3, [r4, #2528]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1212
	ldrsb	r1, [r0]
	cmp	r1, #0
	beq	.L1229
	ldr	r3, [r4, #2528]
	b	.L1214
.L1212:
	ldr	r3, [r4, #2528]
	ldr	r1, [r3, #8]
	cmp	r1, #1
	beq	.L1214
	str	r8, [r3, #32]
	mov	r2, #1
	ldr	r3, [r4, #2528]
	strb	r0, [r3, #1]
	ldr	r3, [r4, #2528]
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2528]
	b	.L1214
.L1228:
	mov	r1, r10
	mov	r0, r7
	add	r9, r9, #1
	bl	HEVC_RemoveFrameStoreOutDPB
	cmp	r9, r5
	mov	r6, #1
	bne	.L1218
.L1211:
	mov	r0, r6
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1219:
	mov	r6, r5
	b	.L1211
	UNWIND(.fnend)
	.size	HEVC_RemoveUnUsedFrameStore, .-HEVC_RemoveUnUsedFrameStore
	.align	2
	.global	HEVC_SimpleSlideDPB
	.type	HEVC_SimpleSlideDPB, %function
HEVC_SimpleSlideDPB:
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
	ldr	r3, [r0, #2504]
	mov	r4, r0
	ldr	r2, [r0, #2508]
	cmp	r2, r3
	ldmccfd	sp, {r4, r5, fp, sp, pc}
	cmp	r3, #15
	bls	.L1233
.L1234:
	ldr	r3, [r4, #2528]
	mov	r5, #0
	mov	r2, r5
	strb	r5, [r3, #1]
	ldr	r1, [r3, #32]
	ldr	r0, [r4, #244]
	bl	FSP_SetRef
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_RemoveApcStore
	mov	r1, r5
	mov	r0, r4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	HEVC_RemoveFrameStoreOutDPB
.L1233:
	bl	HEVC_IncreaseDPBSize
	cmp	r0, #0
	bne	.L1234
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_SimpleSlideDPB, .-HEVC_SimpleSlideDPB
	.align	2
	.global	HEVC_GetMinPOC
	.type	HEVC_GetMinPOC, %function
HEVC_GetMinPOC:
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
	mov	r7, r2
	mvn	r3, #-2147483648
	mov	r2, #0
	str	r2, [r7]
	str	r3, [r1]
	mov	r5, r1
	ldr	r2, [r0, #2508]
	mov	r6, r0
	ldr	r3, [r0, #2504]
	cmp	r2, r3
	bhi	.L1243
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1244:
	add	lr, r6, #2512
	mov	r3, #0
	add	lr, lr, #12
.L1240:
	ldr	r2, [lr, #4]!
	ldr	r4, [r5]
	ldr	ip, [r2, #12]
	cmp	r4, ip
	strgt	ip, [r5]
	strgt	r3, [r7]
	add	r3, r3, #1
	ldr	ip, [r6, #2508]
	cmp	ip, r3
	bhi	.L1240
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1243:
	ldr	r1, .L1245
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r6, #2504]
	cmp	r2, #0
	str	r2, [r6, #2508]
	bne	.L1244
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1246:
	.align	2
.L1245:
	.word	.LC101
	UNWIND(.fnend)
	.size	HEVC_GetMinPOC, .-HEVC_GetMinPOC
	.align	2
	.global	HEVC_GetReRangeFlag
	.type	HEVC_GetReRangeFlag, %function
HEVC_GetReRangeFlag:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #53248
	ldr	r5, [r0, #2320]
	movw	r6, #2024
	movw	r4, #26164
	ldr	ip, [r3, #1092]
	ldr	r3, [r0, #2316]
	ldr	lr, [fp, #4]
	mla	ip, r6, ip, r5
	ldr	ip, [ip, #32]
	mla	r3, r4, ip, r3
	ldr	ip, [r3, #72]
	ldr	r3, [r3, #76]
	cmp	r1, ip
	cmpeq	lr, #0
	bne	.L1251
	cmp	r2, r3
	beq	.L1252
.L1251:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1252:
	ldr	r3, [r0, #168]
	cmp	r3, #0
	beq	.L1251
	ldr	r3, [r0, #2324]
	ldr	r0, [r3, #888]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_GetReRangeFlag, .-HEVC_GetReRangeFlag
	.align	2
	.global	HEVC_CalcStreamBits
	.type	HEVC_CalcStreamBits, %function
HEVC_CalcStreamBits:
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
	ldr	r4, [r0, #2332]
	ldrb	lr, [r4, #1]	@ zero_extendqisi2
	cmp	lr, #0
	beq	.L1264
	mov	r3, #0
	mov	r2, r4
	mov	r1, r3
.L1255:
	add	r3, r3, #1
	ldr	ip, [r2, #48]
	cmp	r3, lr
	add	r2, r2, #28
	add	r1, r1, ip
	bne	.L1255
.L1254:
	ldr	r2, [r0, #2308]
	mov	r3, lr, asl #5
	sub	lr, r3, lr, asl #2
	add	r4, r4, lr
	ldr	r3, [r2, #24]
	and	r3, r3, #7
	str	r3, [r4, #60]
	ldr	r2, [r0, #2332]
	ldr	r3, [r0, #2308]
	ldrb	ip, [r2, #1]	@ zero_extendqisi2
	ldr	lr, [r3, #24]
	mov	r3, ip, asl #5
	sub	r3, r3, ip, asl #2
	add	r3, r2, r3
	ldr	r2, [r3, #52]
	add	r2, r2, lr, lsr #3
	rsb	r2, r1, r2
	str	r2, [r3, #64]
	ldr	ip, [r0, #2332]
	ldr	r3, [r0, #2308]
	ldrb	r2, [ip, #1]	@ zero_extendqisi2
	ldr	lr, [r3, #24]
	mov	r3, r2, asl #5
	sub	r2, r3, r2, asl #2
	add	r2, ip, r2
	ldr	r3, [r2, #48]
	add	r3, r1, r3
	rsb	r3, lr, r3, lsl #3
	str	r3, [r2, #56]
	ldr	r2, [r0, #2332]
	ldrb	r1, [r2, #1]	@ zero_extendqisi2
	mov	r3, r1, asl #5
	sub	r3, r3, r1, asl #2
	add	r3, r2, r3
	ldr	r2, [r3, #64]
	ldr	r1, [r3, #60]
	and	r2, r2, #3
	add	r2, r1, r2, lsl #3
	str	r2, [r3, #60]
	ldr	r2, [r0, #2332]
	ldrb	r1, [r2, #1]	@ zero_extendqisi2
	mov	r3, r1, asl #5
	sub	r3, r3, r1, asl #2
	add	r3, r2, r3
	ldr	r2, [r3, #64]
	bic	r2, r2, #3
	str	r2, [r3, #64]
	ldr	r3, [r0, #2332]
	ldr	r2, [r3, #8]
	sub	ip, r2, #1
	cmp	ip, #1
	bhi	.L1271
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	add	lr, r1, #1
	cmp	r2, lr
	movhi	r2, #1
	movls	r2, #0
	cmp	lr, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L1257
	add	r1, r1, #2
	mov	ip, #0
.L1258:
	sub	r2, r1, #1
	mov	lr, r2, asl #5
	sub	lr, lr, r2, asl #2
	add	r3, r3, lr
	str	ip, [r3, #60]
	ldr	r2, [r0, #2332]
	add	r2, r2, lr
	ldr	r3, [r2, #52]
	str	r3, [r2, #64]
	ldr	r2, [r0, #2332]
	add	r2, r2, lr
	ldr	r3, [r2, #48]
	mov	r3, r3, asl #3
	str	r3, [r2, #56]
	ldr	r3, [r0, #2332]
	add	r3, r3, lr
	ldr	r2, [r3, #64]
	ldr	r4, [r3, #60]
	and	r2, r2, #15
	add	r2, r4, r2, lsl #3
	str	r2, [r3, #60]
	ldr	r2, [r0, #2332]
	add	r3, r2, lr
	ldr	r2, [r3, #64]
	bic	r2, r2, #15
	str	r2, [r3, #64]
	ldr	r3, [r0, #2332]
	ldr	r2, [r3, #8]
	cmp	r1, #1
	cmpls	r1, r2
	add	r1, r1, #1
	movcc	lr, #1
	movcs	lr, #0
	cmp	lr, #0
	bne	.L1258
	sub	ip, r2, #1
.L1257:
	mov	r1, ip, asl #5
	ldr	lr, [r3, #12]
	sub	ip, r1, ip, asl #2
	add	r3, r3, ip
	ldr	r2, [r3, #56]
	rsb	r2, lr, r2
	str	r2, [r3, #56]
	ldr	lr, [r0, #2332]
	ldrb	r1, [lr, #1]	@ zero_extendqisi2
	ldr	r3, [lr, #8]
	cmp	r1, r3
	ldmcsfd	sp, {r4, fp, sp, pc}
	sxth	r3, r1
	add	r0, r0, #53248
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, lr, r3
.L1262:
	ldr	r2, [r3, #64]
	add	r1, r1, #1
	ldr	ip, [r0, #368]
	add	r3, r3, #28
	cmp	r2, ip
	strcc	r2, [r0, #368]
	ldr	r2, [lr, #8]
	cmp	r2, r1
	bhi	.L1262
	ldmfd	sp, {r4, fp, sp, pc}
.L1271:
	ldr	r1, .L1272
	mov	r0, #0
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	dprint_vfmw
.L1264:
	mov	r1, lr
	b	.L1254
.L1273:
	.align	2
.L1272:
	.word	.LC102
	UNWIND(.fnend)
	.size	HEVC_CalcStreamBits, .-HEVC_CalcStreamBits
	.align	2
	.global	HEVC_VpsSpsPpsCheck
	.type	HEVC_VpsSpsPpsCheck, %function
HEVC_VpsSpsPpsCheck:
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
	add	r2, r0, #53248
	ldr	r3, [r0, #156]
	ldr	r2, [r2, #1092]
	cmp	r2, r3
	bge	.L1282
	movw	r3, #2024
	ldr	r1, [r0, #2320]
	mul	r3, r3, r2
	add	ip, r1, r3
	ldrb	r3, [r1, r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1283
	ldr	r2, [ip, #32]
	ldr	r3, [r0, #152]
	cmp	r2, r3
	bge	.L1284
	ldr	r1, [r0, #2316]
	movw	r3, #26164
	mla	r3, r3, r2, r1
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1285
	ldr	r2, [r3, #52]
	ldr	r3, [r0, #148]
	cmp	r2, r3
	bcs	.L1286
	movw	r3, #23196
	ldr	r1, [r0, #2312]
	mul	r3, r3, r2
	ldrb	r3, [r1, r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1287
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1282:
	ldr	r1, .L1288
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1283:
	ldr	r1, .L1288+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1284:
	ldr	r1, .L1288+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1285:
	ldr	r1, .L1288+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1286:
	ldr	r1, .L1288+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1287:
	ldr	r1, .L1288+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1289:
	.align	2
.L1288:
	.word	.LC103
	.word	.LC104
	.word	.LC105
	.word	.LC106
	.word	.LC107
	.word	.LC108
	UNWIND(.fnend)
	.size	HEVC_VpsSpsPpsCheck, .-HEVC_VpsSpsPpsCheck
	.align	2
	.global	HEVC_IsNewPic
	.type	HEVC_IsNewPic, %function
HEVC_IsNewPic:
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
	UNWIND(.pad #16)
	sub	sp, sp, #16
	ldr	r2, [r0, #2332]
	ldr	r1, [r0, #200]
	ldr	r3, [r2, #16]
	cmn	r1, #-2147483647
	sub	ip, r3, #16
	beq	.L1322
	cmp	r3, #21
	cmpne	ip, #2
	addls	ip, r0, #53248
	ldrls	lr, [ip, #1064]
	bls	.L1296
.L1304:
	sub	r3, r3, #19
	add	ip, r0, #53248
	cmp	r3, #1
	movls	r1, #-2147483647
	strls	r1, [r0, #200]
	ldr	lr, [ip, #1064]
.L1296:
	cmp	lr, r1
	bge	.L1294
	ldr	r3, [r2, #16]
	sub	r4, r3, #8
	cmp	r4, #1
	bls	.L1323
.L1297:
	ldrb	r1, [r0, #8]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1298
	ldr	r4, [r0, #204]
	cmp	lr, r4
	bge	.L1298
	ldr	r3, [r2, #16]
	sub	r5, r3, #8
	cmp	r5, #1
	bls	.L1324
.L1298:
	ldrb	r3, [ip, #1019]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1299
	ldr	r2, [r2, #16]
	sub	r1, r2, #19
	cmp	r1, #1
	bls	.L1319
	cmp	r2, #21
	beq	.L1319
	sub	r2, r2, #16
	cmp	r2, #2
	strls	lr, [r0, #204]
	movls	r3, #1
	strlsb	r3, [r0, #8]
.L1299:
	ldr	r2, [r0, #216]
	ldrb	r3, [ip, #1017]	@ zero_extendqisi2
	cmp	r2, #0
	ldreq	r1, [ip, #1092]
	bne	.L1325
	ldr	r2, [r0, #288]
	cmp	r2, r1
	movne	r0, #1
	beq	.L1326
.L1317:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1322:
	cmp	ip, #5
	bhi	.L1320
	cmp	r3, #21
	cmpne	ip, #2
	bhi	.L1304
	add	ip, r0, #53248
	ldr	lr, [ip, #1064]
	str	lr, [r0, #200]
.L1294:
	ldrb	r1, [r0, #3]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1297
	ldr	r3, [r2, #16]
	sub	r4, r3, #8
	cmp	r4, #1
	bhi	.L1297
	str	r3, [sp, #8]
	mov	r0, #1
	str	r1, [sp, #4]
	movw	r3, #10448
	str	lr, [sp]
	ldr	r2, .L1327
	ldr	r1, .L1327+4
	bl	dprint_vfmw
	mov	r0, #2
	b	.L1317
.L1325:
	add	r2, r2, #16640
	ldr	r1, [ip, #1092]
	add	r2, r2, #203
	add	r2, r0, r2, lsl #2
	ldr	r2, [r2, #4]
	cmp	r1, r2
	ldr	r2, [r0, #288]
	movne	r3, #1
	cmp	r2, r1
	movne	r0, #1
	bne	.L1317
	b	.L1326
.L1324:
	str	r3, [sp, #12]
	mov	r0, #1
	stmia	sp, {r1, lr}
	movw	r3, #10458
	str	r4, [sp, #8]
	ldr	r2, .L1327
	ldr	r1, .L1327+8
	bl	dprint_vfmw
.L1320:
	mov	r0, #2
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1326:
	ldr	r0, [r0, #252]
	cmp	r0, lr
	moveq	r0, r3
	movne	r0, #1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1323:
	str	r3, [sp, #8]
	mov	r0, #1
	str	r1, [sp, #4]
	movw	r3, #10440
	str	lr, [sp]
	ldr	r2, .L1327
	ldr	r1, .L1327+12
	bl	dprint_vfmw
	mov	r0, #2
	b	.L1317
.L1319:
	str	lr, [r0, #204]
	strb	r3, [r0, #8]
	b	.L1299
.L1328:
	.align	2
.L1327:
	.word	.LANCHOR1+400
	.word	.LC110
	.word	.LC111
	.word	.LC109
	UNWIND(.fnend)
	.size	HEVC_IsNewPic, .-HEVC_IsNewPic
	.align	2
	.global	HEVC_ReadByteAlignment
	.type	HEVC_ReadByteAlignment, %function
HEVC_ReadByteAlignment:
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
	mov	r4, r0
	ldr	r0, [r0, #2308]
	bl	BsGet
	cmp	r0, #0
	beq	.L1334
	ldr	r0, [r4, #2308]
	bl	BsBitsToNextByte
	subs	r1, r0, #0
	beq	.L1333
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r1, .L1335
	mov	r2, r0
	mov	r0, #19
	bl	dprint_vfmw
.L1333:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1334:
	ldr	r1, .L1335+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1336:
	.align	2
.L1335:
	.word	.LC113
	.word	.LC112
	UNWIND(.fnend)
	.size	HEVC_ReadByteAlignment, .-HEVC_ReadByteAlignment
	.align	2
	.global	HEVC_GetRapPicFlag
	.type	HEVC_GetRapPicFlag, %function
HEVC_GetRapPicFlag:
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
	add	r0, r0, #53248
	ldr	r0, [r0, #1088]
	sub	r0, r0, #16
	cmp	r0, #5
	movhi	r0, #0
	movls	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_GetRapPicFlag, .-HEVC_GetRapPicFlag
	.align	2
	.global	HEVC_GetIdrPicFlag
	.type	HEVC_GetIdrPicFlag, %function
HEVC_GetIdrPicFlag:
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
	add	r0, r0, #53248
	ldr	r0, [r0, #1088]
	sub	r0, r0, #19
	cmp	r0, #1
	movhi	r0, #0
	movls	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_GetIdrPicFlag, .-HEVC_GetIdrPicFlag
	.align	2
	.global	HEVC_DecPTL
	.type	HEVC_DecPTL, %function
HEVC_DecPTL:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 112
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #116)
	sub	sp, sp, #116
	subs	r8, r2, #0
	mov	r5, r1
	mov	r4, r0
	str	r3, [fp, #-152]
	moveq	r1, #8
	bne	.L1378
.L1375:
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r3, [fp, #-152]
	cmp	r3, #0
	str	r0, [r5, #284]
	ble	.L1342
	ldr	r3, [fp, #-152]
	add	r6, r5, #42
	add	r9, r3, #42
	mov	r7, r6
	add	r9, r5, r9
	b	.L1344
.L1343:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r7, #1]!
	cmp	r7, r9
	beq	.L1379
.L1344:
	cmp	r8, #0
	beq	.L1343
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r7, #-5]
	b	.L1343
.L1379:
	ldr	r3, [fp, #-152]
	cmp	r3, #7
	ldrle	r7, [fp, #-152]
	ble	.L1348
	b	.L1354
.L1347:
	cmp	r7, #8
	beq	.L1354
.L1348:
	mov	r1, #2
	ldr	r0, [r4, #2308]
	bl	BsGet
	add	r7, r7, #1
	cmp	r0, #0
	beq	.L1347
	ldr	r1, .L1380
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1342:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1354:
	add	r9, r5, #360
	add	r3, r5, #48
	mov	r7, #0
	b	.L1352
.L1351:
	ldr	r3, [fp, #-152]
	add	r7, r7, #1
	add	r9, r9, #4
	cmp	r3, r7
	mov	r3, r5
	ble	.L1342
.L1352:
	cmp	r8, #0
	beq	.L1376
	ldrb	r2, [r6, #-5]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1374
.L1376:
	add	r5, r3, #32
.L1349:
	ldrb	r3, [r6, #1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1351
	mov	r1, #8
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r9]
	b	.L1351
.L1374:
	mov	r1, #2
	ldr	r0, [r4, #2308]
	str	r3, [fp, #-156]
	bl	BsGet
	ldr	r3, [fp, #-156]
	mov	r1, #1
	add	r5, r3, #32
	mov	r10, r3
	str	r0, [r9, #-72]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #5
	strb	r0, [r6, #199]
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r9, #-48]
.L1350:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1]!
	cmp	r10, r5
	bne	.L1350
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6, #205]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6, #211]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6, #217]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r6, #223]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #16
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #12
	ldr	r0, [r4, #2308]
	bl	BsGet
	b	.L1349
.L1378:
	mov	r1, #2
	ldr	r0, [r0, #2308]
	bl	BsGet
	mov	r1, #1
	ldr	r9, .L1380+4
	mov	r7, r5
	mov	r6, #0
	str	r0, [r5, #276]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #5
	strb	r0, [r5]
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r5, #272]
.L1341:
	mov	r3, r6
	ldr	ip, [r9, #68]
	ldr	r2, .L1380+8
	mov	r1, #100
	sub	r0, fp, #144
	add	r6, r6, #1
	blx	ip
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	cmp	r6, #32
	strb	r0, [r7, #1]!
	bne	.L1341
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #33]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #34]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #35]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r5, #36]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #16
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #12
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r2, [r5, #272]
	add	r3, r4, #81920
	mov	r1, #8
	str	r2, [r3, #3808]
	ldrb	r2, [r5, #2]	@ zero_extendqisi2
	str	r2, [r3, #3812]
	ldrb	r2, [r5, #3]	@ zero_extendqisi2
	str	r2, [r3, #3816]
	b	.L1375
.L1381:
	.align	2
.L1380:
	.word	.LC115
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC114
	UNWIND(.fnend)
	.size	HEVC_DecPTL, .-HEVC_DecPTL
	.align	2
	.global	HEVC_SetDefaultScalingList
	.type	HEVC_SetDefaultScalingList, %function
HEVC_SetDefaultScalingList:
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
	ldr	r9, .L1400
	add	r10, r0, #2192
	mov	r6, #0
	add	r3, r0, #2208
	str	r0, [fp, #-52]
	str	r3, [fp, #-48]
.L1383:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	beq	.L1391
	ldr	r3, [fp, #-52]
	mov	r8, r6, asl #9
	ldr	r7, [fp, #-48]
	sub	r8, r8, r6, asl #7
	add	r5, r3, #580
	mov	r4, #0
.L1389:
	cmp	r6, #0
	add	r0, r5, r8
	mov	r2, #64
	ldr	r3, [r9, #52]
	beq	.L1398
	cmp	r6, #3
	beq	.L1399
	ldr	ip, .L1400+4
	cmp	r4, #2
	ldr	r1, .L1400+8
	movhi	r1, ip
	blx	r3
.L1385:
	mov	r3, #16
	str	r3, [r7, #4]!
	ldr	r3, [r10]
	add	r4, r4, #1
	add	r5, r5, #64
	cmp	r3, r4
	bhi	.L1389
.L1391:
	add	r6, r6, #1
	ldr	r3, [fp, #-48]
	cmp	r6, #4
	add	r3, r3, #24
	str	r3, [fp, #-48]
	bne	.L1383
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1398:
	mov	r2, #16
	ldr	r1, .L1400+12
	mov	r0, r5
	blx	r3
	b	.L1385
.L1399:
	ldr	r0, .L1400+4
	cmp	r4, #0
	ldr	r1, .L1400+8
	mov	r2, #64
	movne	r1, r0
	add	r0, r5, #1152
	blx	r3
	b	.L1385
.L1401:
	.align	2
.L1400:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+416
	.word	.LANCHOR4
	.word	.LANCHOR1+480
	UNWIND(.fnend)
	.size	HEVC_SetDefaultScalingList, .-HEVC_SetDefaultScalingList
	.align	2
	.global	HEVC_CalScalingList
	.type	HEVC_CalScalingList, %function
HEVC_CalScalingList:
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
	add	lr, r0, r2, lsl #2
	mov	ip, r2, asl #3
	sub	r2, ip, r2, asl #1
	mov	r5, #0
	ldr	r7, [lr, #292]
	add	r3, r3, r2
	cmp	r7, #64
	mov	r6, r3, asl #4
	movcs	r7, #64
	mov	r8, r7, lsr #2
.L1403:
	cmp	r5, r8
	ldmcsfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r7, #0
	beq	.L1403
	add	r3, r5, r6
	ldrb	ip, [r1]	@ zero_extendqisi2
	mov	r4, r1
	mov	r2, #0
	add	r3, r0, r3, lsl #2
	ldr	lr, [r3, #580]
	orr	ip, lr, ip
	str	ip, [r3, #580]
.L1409:
	add	r2, r2, #1
	add	r3, r5, r6
	and	lr, r2, #3
	cmp	r2, r7
	add	r3, r0, r3, lsl #2
	mov	ip, lr, asl #3
	beq	.L1403
	ldrb	r9, [r4, #1]!	@ zero_extendqisi2
	cmp	lr, #3
	ldr	lr, [r3, #580]
	addeq	r5, r5, #1
	orr	ip, lr, r9, asl ip
	str	ip, [r3, #580]
	b	.L1409
	UNWIND(.fnend)
	.size	HEVC_CalScalingList, .-HEVC_CalScalingList
	.align	2
	.global	HEVC_SetScalingList
	.type	HEVC_SetScalingList, %function
HEVC_SetScalingList:
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
	add	r6, r0, #24576
	mov	r5, r0
	ldr	r3, [r6, #2836]
	ldrb	r2, [r3, #5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1411
	ldrb	r3, [r3, #6]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r6, #2840]
	bne	.L1412
	ldrb	r3, [r3, #21]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1430
.L1413:
	ldr	r8, .L1431
	mov	r1, #0
	mov	r2, #1536
	add	r0, r5, #580
	add	r10, r5, #2192
	mov	r7, r1
	ldr	r3, [r8, #48]
	blx	r3
.L1415:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	movne	r9, r7, asl #3
	movne	r4, #0
	subne	r9, r9, r7, asl #1
	beq	.L1419
.L1416:
	add	r0, r9, r4
	ldr	r1, [r6, #2840]
	mov	r3, r4
	mov	r2, r7
	add	r1, r1, r0, lsl #6
	mov	r0, r5
	add	r1, r1, #308
	add	r4, r4, #1
	add	r1, r1, #1
	bl	HEVC_CalScalingList
	ldr	r3, [r10]
	cmp	r4, r3
	bcc	.L1416
.L1419:
	add	r7, r7, #1
	cmp	r7, #4
	bne	.L1415
	ldr	r1, [r6, #2840]
	add	r0, r5, #2208
	add	r0, r0, #4
	ldr	r3, [r8, #52]
	add	r1, r1, #1920
	mov	r2, #96
	add	r1, r1, #8
	blx	r3
.L1411:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1412:
	ldrb	r1, [r3, #21]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L1413
	ldr	r8, .L1431
	mov	r2, #1536
	add	r0, r0, #580
	mov	r7, r1
	add	r10, r5, #2192
	ldr	r3, [r8, #48]
	blx	r3
.L1420:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	movne	r9, r7, asl #3
	movne	r4, #0
	subne	r9, r9, r7, asl #1
	beq	.L1424
.L1421:
	add	r0, r9, r4
	ldr	r1, [r6, #2836]
	mov	r3, r4
	mov	r2, r7
	add	r1, r1, r0, lsl #6
	mov	r0, r5
	add	r1, r1, #908
	add	r4, r4, #1
	add	r1, r1, #1
	bl	HEVC_CalScalingList
	ldr	r3, [r10]
	cmp	r4, r3
	bcc	.L1421
.L1424:
	add	r7, r7, #1
	cmp	r7, #4
	bne	.L1420
	ldr	r1, [r6, #2836]
	add	r0, r5, #2208
	ldr	r3, [r8, #52]
	mov	r2, #96
	add	r1, r1, #2528
	add	r0, r0, #4
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1430:
	bl	HEVC_SetDefaultScalingList
	b	.L1411
.L1432:
	.align	2
.L1431:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_SetScalingList, .-HEVC_SetScalingList
	.align	2
	.global	HEVC_GetScalingListDefaultAddress
	.type	HEVC_GetScalingListDefaultAddress, %function
HEVC_GetScalingListDefaultAddress:
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
	cmp	r0, #3
	ldrls	pc, [pc, r0, asl #2]
	b	.L1434
.L1436:
	.word	.L1440
	.word	.L1438
	.word	.L1438
	.word	.L1439
.L1438:
	ldr	r3, .L1444
	cmp	r1, #2
	add	r0, r3, #64
	movhi	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L1440:
	ldr	r0, .L1444+4
	ldmfd	sp, {fp, sp, pc}
.L1439:
	ldr	r3, .L1444
	cmp	r1, #0
	add	r0, r3, #64
	movne	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L1434:
	ldr	r1, .L1444+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1445:
	.align	2
.L1444:
	.word	.LANCHOR4+80
	.word	.LANCHOR4+64
	.word	.LC116
	UNWIND(.fnend)
	.size	HEVC_GetScalingListDefaultAddress, .-HEVC_GetScalingListDefaultAddress
	.align	2
	.global	HEVC_DecRefPicListsModification
	.type	HEVC_DecRefPicListsModification, %function
HEVC_DecRefPicListsModification:
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
	cmp	r2, #0
	cmpne	r1, #0
	mov	r5, r3
	mov	r7, r1
	mov	r8, r2
	moveq	ip, #1
	movne	ip, #0
	cmp	r0, #0
	orreq	ip, ip, #1
	mov	r6, r0
	cmp	ip, #0
	bne	.L1456
	sub	ip, r3, #1
	mov	r4, #1
	movs	ip, ip, asr #1
	beq	.L1448
.L1449:
	movs	ip, ip, asr #1
	add	r4, r4, #1
	bne	.L1449
.L1448:
	mov	r1, #1
	ldr	r0, [r6, #2308]
	bl	BsGet
	cmp	r5, #1
	movls	r10, #0
	movhi	r10, #1
	uxtb	r0, r0
	strb	r0, [r8]
	cmp	r0, #0
	cmpne	r5, #1
	bls	.L1453
	ldr	r3, [r7, #168]
	cmp	r3, #0
	beq	.L1453
	mov	r9, r8
	mov	r5, #0
.L1454:
	mov	r1, r4
	ldr	r0, [r6, #2308]
	bl	BsGet
	add	r5, r5, #1
	str	r0, [r9, #4]!
	ldr	r3, [r7, #168]
	cmp	r3, r5
	bhi	.L1454
.L1453:
	ldr	r5, [r7, #60]
	cmp	r5, #0
	beq	.L1471
.L1452:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1471:
	mov	r1, #1
	ldr	r0, [r6, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r8, #1]
	cmp	r0, #0
	moveq	r10, #0
	andne	r10, r10, #1
	cmp	r10, #0
	beq	.L1452
	ldr	r3, [r7, #172]
	cmp	r3, #0
	beq	.L1452
	add	r8, r8, #128
.L1455:
	mov	r1, r4
	ldr	r0, [r6, #2308]
	bl	BsGet
	add	r5, r5, #1
	str	r0, [r8, #4]!
	ldr	r3, [r7, #172]
	cmp	r3, r5
	bhi	.L1455
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1456:
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_DecRefPicListsModification, .-HEVC_DecRefPicListsModification
	.align	2
	.global	HEVC_MoreRbspData
	.type	HEVC_MoreRbspData, %function
HEVC_MoreRbspData:
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
	ldr	r0, [r0, #2308]
	bl	BsResidBits
	subs	r3, r0, #0
	ble	.L1474
	ldr	r2, [r4, #2332]
	ldr	r0, [r2, #12]
	add	r0, r0, #3
	cmp	r3, r0
	movls	r0, #0
	movhi	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1474:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_MoreRbspData, .-HEVC_MoreRbspData
	.align	2
	.global	HEVC_More_Rbsp_Data
	.type	HEVC_More_Rbsp_Data, %function
HEVC_More_Rbsp_Data:
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
	ldr	r0, [r0, #2308]
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #24]
	mov	r2, r2, asl #3
	cmp	r2, r3
	bls	.L1478
	mov	r1, #8
	bl	BsShow
	subs	r0, r0, #128
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1478:
	ldr	r1, .L1479
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1480:
	.align	2
.L1479:
	.word	.LC117
	UNWIND(.fnend)
	.size	HEVC_More_Rbsp_Data, .-HEVC_More_Rbsp_Data
	.align	2
	.global	HEVC_FindTrailZeros
	.type	HEVC_FindTrailZeros, %function
HEVC_FindTrailZeros:
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
	mov	r1, #0
	mov	r6, r0
.L1493:
	ldr	r3, [r6, #2332]
	ldr	ip, [r3, #8]
	sub	ip, ip, #1
	mov	r0, ip, asl #2
	mov	ip, ip, asl #5
	rsb	r2, r0, ip
	add	r2, r3, r2
	add	r2, r2, #40
	ldr	r3, [r2, #8]
	ldr	r2, [r2, #4]
	sub	r3, r3, #1
	add	r3, r2, r3
.L1482:
	ldrb	r4, [r3]	@ zero_extendqisi2
	rsb	r5, r0, ip
	cmp	r4, #0
	bne	.L1494
	ldr	r2, [r6, #2332]
	add	r1, r1, #1
	add	lr, r2, r5
	ldr	r2, [r2, #44]
	cmp	r2, r3
	beq	.L1495
	ldr	r2, [lr, #44]
	cmp	r2, r3
	sub	r3, r3, #1
	bne	.L1482
	ldr	r1, [lr, #68]
	ldr	r0, [r6, #244]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r6, #2332]
	mov	r1, r4
	add	r5, r3, r5
	str	r4, [r5, #44]
	ldr	r2, [r6, #2332]
	ldr	r3, [r2, #8]
	sub	r3, r3, #1
	str	r3, [r2, #8]
	b	.L1493
.L1494:
	tst	r4, #1
	mov	r3, #1
	moveq	r2, r3
	bne	.L1489
.L1490:
	ands	r0, r4, r2, asl r3
	add	r3, r3, #1
	bne	.L1489
	cmp	r3, #8
	bne	.L1490
	ldr	r1, .L1496
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1495:
	ldr	r1, .L1496+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1489:
	ldr	ip, [r6, #2332]
	mov	r2, #0
	add	r1, r3, r1, lsl #3
	mov	r0, r2
	str	r2, [ip, #4]
	ldr	r3, [r6, #2332]
	str	r1, [r3, #12]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1497:
	.align	2
.L1496:
	.word	.LC119
	.word	.LC118
	UNWIND(.fnend)
	.size	HEVC_FindTrailZeros, .-HEVC_FindTrailZeros
	.align	2
	.global	HEVC_FindNaluArraySlot
	.type	HEVC_FindNaluArraySlot, %function
HEVC_FindNaluArraySlot:
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
	ldr	r6, [r0, #160]
	mov	r9, r0
	ldr	r3, [r0, #140]
	cmp	r6, #0
	blt	.L1499
	add	r7, r6, #1
	mov	r0, r3
	mov	r5, #100
	mov	r1, r7
	bl	__aeabi_uidivmod
	add	r2, r9, #65536
	ldr	r8, [r2, #1840]
	mla	r2, r5, r1, r8
	mov	r3, r1
	ldr	r2, [r2, #36]
	cmp	r2, #0
	beq	.L1500
	mov	r4, #0
	b	.L1502
.L1503:
	bl	__aeabi_uidivmod
	mla	r2, r5, r1, r8
	mov	r3, r1
	ldr	r2, [r2, #36]
	cmp	r2, #0
	beq	.L1500
.L1502:
	add	r4, r4, #1
	mov	r1, r7
	cmp	r4, r6
	add	r0, r3, r4
	ble	.L1503
.L1499:
	mvn	r0, #0
	str	r3, [r9, #140]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1500:
	cmp	r3, r7
	mov	r0, r3
	str	r3, [r9, #140]
	bcs	.L1513
	mov	r3, #100
	mov	r2, #1
	mla	r8, r3, r0, r8
	str	r2, [r8, #36]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1513:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_FindNaluArraySlot, .-HEVC_FindNaluArraySlot
	.align	2
	.global	HEVC_ReleaseNAL
	.type	HEVC_ReleaseNAL, %function
HEVC_ReleaseNAL:
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
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L1522
	add	r7, r5, #56
	mov	r4, r5
	mov	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #40]
.L1517:
	ldr	r3, [r4, #44]
	mov	r0, r6
	cmp	r3, #0
	beq	.L1516
	ldr	r1, [r4, #68]
	bl	SM_ReleaseStreamSeg
	ldr	ip, [r4, #48]
	ldr	r3, [r4, #44]
	mov	r0, #7
	ldr	r2, .L1523
	ldr	r1, .L1523+4
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #44]
	str	r3, [r4, #60]
	str	r3, [r4, #48]
.L1516:
	add	r4, r4, #28
	cmp	r4, r7
	bne	.L1517
	ldr	r3, .L1523+8
	mov	r0, r5
	mov	r2, #100
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L1522:
	mov	r0, r5
	movw	r3, #11740
	ldr	r2, .L1523+12
	ldr	r1, .L1523+16
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1524:
	.align	2
.L1523:
	.word	.LANCHOR1+496
	.word	.LC120
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC45
	.word	.LC46
	UNWIND(.fnend)
	.size	HEVC_ReleaseNAL, .-HEVC_ReleaseNAL
	.align	2
	.global	Hevc_InitDecBuffers
	.type	Hevc_InitDecBuffers, %function
Hevc_InitDecBuffers:
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
	add	r3, r0, #24576
	mov	r7, r0
	add	r0, r0, #2720
	add	r1, r7, #2656
	ldr	r3, [r3, #2836]
	add	r2, r7, #2512
	mov	lr, #0
	add	r1, r1, #4
	add	r2, r2, #12
	add	ip, r7, #2592
	ldr	r3, [r3, #236]
	mov	r6, #17
	add	r3, r3, #1
	cmp	r3, #17
	movhi	r3, #17
	str	r3, [r0, #-216]
	add	r0, r0, #8
	mov	r3, lr
	str	lr, [r7, #2508]
	str	lr, [r7, #2520]
	str	lr, [r7, #2524]
	str	lr, [r7, #2516]
	str	lr, [r7, #2512]
.L1528:
	subs	r6, r6, #1
	str	r3, [r0, #4]!
	str	r3, [ip, #4]!
	str	r3, [r1, #4]!
	str	r3, [r2, #4]!
	bne	.L1528
	add	r4, r7, #67584
	ldr	r10, .L1561
	add	r4, r4, #672
	mov	r8, r6
.L1529:
	ldr	r3, [r10, #48]
	mov	r2, #624
	mov	r0, r4
	mov	r1, #0
	blx	r3
	str	r8, [r4, #156]
	add	r8, r8, #1
	mov	r5, #0
	cmp	r8, #20
	sub	r3, r4, #72
	strb	r5, [r4, #-72]
	mov	r2, #20
	str	r3, [r4, #-4]
	mov	r9, #16
	str	r5, [r4, #-68]
	add	r4, r4, #704
	strb	r5, [r4, #-775]
	str	r5, [r4, #-764]
	strb	r5, [r4, #-774]
	str	r5, [r4, #-728]
	str	r2, [r4, #-752]
	str	r9, [r4, #-748]
	ldr	r3, .L1561
	bne	.L1529
	ldr	r3, [r3, #48]
	mov	r2, #200
	mov	r1, r5
	add	r0, r7, #2800
	blx	r3
	add	r2, r7, #2320
	add	r2, r2, #12
	add	r3, r7, #2400
	mov	r1, r5
	str	r9, [r7, #2800]
	str	r5, [r7, #2804]
.L1530:
	str	r1, [r2, #4]!
	cmp	r2, r3
	mov	r0, #0
	bne	.L1530
	add	r1, r7, #2464
	add	lr, r7, #53248
	add	r1, r1, #4
	mov	ip, r0
	str	r0, [lr, #1176]
.L1531:
	str	ip, [r3, #4]!
	cmp	r3, r1
	mov	r2, #0
	bne	.L1531
	str	r2, [lr, #1180]
	ldr	r1, [r7, #180]
	cmp	r1, r2
	movne	r3, r2
	addne	r2, r7, #2112
	movne	r0, r3
	beq	.L1536
.L1535:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L1535
.L1536:
	ldr	r3, [r7, #160]
	cmp	r3, #0
	beq	.L1560
	mov	r4, #0
	add	r8, r7, #65536
	mov	r5, r4
.L1541:
	ldr	r3, [r8, #2644]
	add	r3, r3, r6
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq	.L1537
	ldr	r3, [r7, #2332]
	cmp	r1, r3
	beq	.L1537
	ldr	r0, [r7, #244]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r8, #2644]
	add	r3, r3, r6
	str	r5, [r3, #24]
	ldr	r3, [r7, #208]
	ldr	r2, [r7, #216]
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r7, #208]
	ldr	r3, [r7, #172]
	cmp	r2, #0
	subne	r2, r2, #1
	str	r2, [r7, #216]
	cmp	r3, #2
	subhi	r3, r3, #1
	movls	r3, #2
	str	r3, [r7, #172]
.L1537:
	ldr	r3, [r7, #160]
	add	r4, r4, #1
	add	r6, r6, #28
	cmp	r3, r4
	bhi	.L1541
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1560:
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1562:
	.align	2
.L1561:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	Hevc_InitDecBuffers, .-Hevc_InitDecBuffers
	.align	2
	.global	HEVC_EmptyFrmFromDPB
	.type	HEVC_EmptyFrmFromDPB, %function
HEVC_EmptyFrmFromDPB:
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
	subs	r6, r0, #0
	beq	.L1574
	ldr	r3, [r6, #2508]
	add	r8, r6, #2512
	cmp	r3, #0
	addne	r8, r8, #12
	movne	r3, #0
	movne	ip, #1
	movne	r1, r8
	beq	.L1589
.L1568:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	streqb	ip, [r2, #2]
	ldr	r2, [r6, #2508]
	cmp	r2, r3
	bhi	.L1568
.L1569:
	mov	r4, r6
	mov	r7, #17
	mov	r5, #0
	mov	r3, #-2147483647
	str	r3, [r6, #212]
.L1566:
	ldr	r3, [r8, #4]!
	mov	r2, #1
	cmp	r3, #0
	beq	.L1570
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #244]
	bl	FSP_ClearLogicFs
.L1570:
	ldr	r3, [r4, #2596]
	mov	r2, #1
	cmp	r3, #0
	beq	.L1571
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #244]
	bl	FSP_ClearLogicFs
.L1571:
	ldr	r3, [r4, #2664]
	mov	r2, #1
	cmp	r3, #0
	beq	.L1572
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #244]
	bl	FSP_ClearLogicFs
.L1572:
	ldr	r3, [r4, #2732]
	mov	r2, #1
	cmp	r3, #0
	beq	.L1573
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #244]
	bl	FSP_ClearLogicFs
.L1573:
	subs	r7, r7, #1
	str	r5, [r4, #2732]
	str	r5, [r4, #2664]
	add	r4, r4, #4
	str	r5, [r4, #2592]
	str	r5, [r4, #2524]
	bne	.L1566
	mov	r0, r6
	bl	Hevc_InitDecBuffers
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1589:
	add	r8, r8, #12
	b	.L1569
.L1574:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_EmptyFrmFromDPB, .-HEVC_EmptyFrmFromDPB
	.align	2
	.global	HEVC_CombinePacket
	.type	HEVC_CombinePacket, %function
HEVC_CombinePacket:
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
	ldr	r0, [r0, #2332]
	cmp	r0, #0
	beq	.L1593
	ldr	r3, [r0, #48]
	cmp	r3, #4096
	ldmcsfd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, .L1594
	mov	r0, #7
	bl	dprint_vfmw
	ldr	r3, [r4, #2332]
	ldr	ip, .L1594+4
	mov	r5, #0
	ldr	r2, [r3, #48]
	ldr	r0, [r3, #72]
	ldr	r1, [r3, #44]
	rsb	r0, r2, r0
	ldr	r3, [ip, #52]
	blx	r3
	ldr	r3, [r4, #2332]
	mov	r0, #1
	ldr	r2, [r3, #96]
	ldr	r1, [r3, #68]
	str	r2, [r3, #68]
	ldr	r3, [r4, #2332]
	str	r5, [r3, #60]
	ldr	r3, [r4, #2332]
	ldr	ip, [r3, #48]
	ldr	r2, [r3, #72]
	rsb	r2, ip, r2
	str	r2, [r3, #44]
	ldr	r3, [r4, #2332]
	ldr	ip, [r3, #48]
	ldr	r2, [r3, #80]
	rsb	r2, ip, r2
	str	r2, [r3, #52]
	ldr	r3, [r4, #2332]
	ldr	r2, [r3, #48]
	ldr	ip, [r3, #76]
	add	r2, r2, ip
	str	r2, [r3, #48]
	ldr	r3, [r4, #2332]
	str	r0, [r3, #8]
	ldr	r0, [r4, #244]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #2332]
	str	r5, [r3, #72]
	ldr	r3, [r4, #2332]
	str	r5, [r3, #88]
	ldr	r3, [r4, #2332]
	str	r5, [r3, #76]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1593:
	ldr	r1, .L1594+8
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L1595:
	.align	2
.L1594:
	.word	.LC122
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC121
	UNWIND(.fnend)
	.size	HEVC_CombinePacket, .-HEVC_CombinePacket
	.align	2
	.global	HEVC_IsOutDPB
	.type	HEVC_IsOutDPB, %function
HEVC_IsOutDPB:
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
	cmp	r1, #0
	beq	.L1601
	ldr	ip, [r0, #2508]
	cmp	ip, #0
	beq	.L1601
	ldr	r3, [r0, #2528]
	cmp	r3, r1
	beq	.L1603
	add	r0, r0, #2528
	mov	r3, #0
	b	.L1598
.L1599:
	ldr	r2, [r0, #4]!
	cmp	r2, r1
	beq	.L1603
.L1598:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1599
.L1601:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1603:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsOutDPB, .-HEVC_IsOutDPB
	.align	2
	.global	HEVC_IsNewPicNal
	.type	HEVC_IsNewPicNal, %function
HEVC_IsNewPicNal:
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
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsNewPicNal, .-HEVC_IsNewPicNal
	.align	2
	.global	HEVC_u_v
	.type	HEVC_u_v, %function
HEVC_u_v:
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
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	BsGet
	UNWIND(.fnend)
	.size	HEVC_u_v, .-HEVC_u_v
	.align	2
	.global	HEVC_f_v
	.type	HEVC_f_v, %function
HEVC_f_v:
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
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	BsGet
	UNWIND(.fnend)
	.size	HEVC_f_v, .-HEVC_f_v
	.align	2
	.global	HEVC_ue_v
	.type	HEVC_ue_v, %function
HEVC_ue_v:
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
	mov	r1, #32
	mov	r5, r0
	bl	BsShow
	bl	ZerosMS_32
	cmp	r0, #31
	bls	.L1610
	movw	r3, #61166
	mov	r2, r0
	movt	r3, 65535
	ldr	r1, .L1611
	mov	r0, #1
	bl	dprint_vfmw
	movw	r0, #61166
	movt	r0, 65535
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1610:
	add	r4, r0, #1
	mov	r1, r0
	mov	r0, r5
	bl	BsSkip
	mov	r1, r4
	mov	r0, r5
	bl	BsShow
	mov	r1, r4
	sub	r4, r0, #1
	mov	r0, r5
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1612:
	.align	2
.L1611:
	.word	.LC123
	UNWIND(.fnend)
	.size	HEVC_ue_v, .-HEVC_ue_v
	.align	2
	.global	HEVC_Sei_Tone_Mapping_Info
	.type	HEVC_Sei_Tone_Mapping_Info, %function
HEVC_Sei_Tone_Mapping_Info:
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
	mov	r5, r0
	ldr	r1, .L1637
	ldr	r0, [r0, #2308]
	bl	HEVC_ue_v
	mov	r1, #1
	str	r0, [r4, #4]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4]
	cmp	r0, #0
	beq	.L1614
	ldr	r3, [r4, #16]
.L1615:
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L1632
.L1619:
	.word	.L1618
	.word	.L1620
	.word	.L1621
	.word	.L1622
	.word	.L1623
.L1614:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #8
	strb	r0, [r4, #1]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #8
	str	r0, [r4, #8]
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r0, #8
	str	r0, [r4, #12]
	bhi	.L1635
	mov	r1, #8
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r3, r0
	str	r0, [r4, #16]
	b	.L1615
.L1623:
	mov	r1, #8
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r0, #255
	str	r0, [r4, #36]
	beq	.L1636
.L1631:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r4, #2]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #52]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #56]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #60]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #64]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #68]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #72]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #52]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r3, #0
	str	r0, [r4, #76]
.L1617:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1618:
	mov	r1, #32
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #20]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r3, #0
	str	r0, [r4, #24]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1620:
	mov	r1, #32
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #28]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r3, #0
	str	r0, [r4, #32]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1621:
	ldr	r8, [r4, #12]
	mov	r3, #1
	movs	r8, r3, asl r8
	addne	r7, r4, #2128
	movne	r6, #0
	beq	.L1626
.L1625:
	ldr	r1, [r4, #8]
	add	r6, r6, #1
	ldr	r0, [r5, #2308]
	add	r1, r1, #7
	bic	r1, r1, #7
	bl	BsGet
	cmp	r8, r6
	str	r0, [r7, #4]!
	bne	.L1625
.L1626:
	ldr	r1, [r4, #8]
	add	r8, r4, r8, lsl #2
	mov	r3, #0
	mov	r2, #1
	mov	r2, r2, asl r1
	mov	r0, r3
	str	r2, [r8, #2132]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1622:
	mov	r1, #16
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r0, #256
	str	r0, [r4, #80]
	bhi	.L1627
	cmp	r0, #0
	addne	r6, r4, #80
	movne	r7, #0
	beq	.L1632
.L1629:
	ldr	r1, [r4, #8]
	add	r7, r7, #1
	ldr	r0, [r5, #2308]
	add	r1, r1, #7
	bic	r1, r1, #7
	bl	BsGet
	str	r0, [r6, #4]!
	ldr	r1, [r4, #12]
	ldr	r0, [r5, #2308]
	add	r1, r1, #7
	bic	r1, r1, #7
	bl	BsGet
	str	r0, [r6, #1024]
	ldr	r3, [r4, #80]
	cmp	r3, r7
	bhi	.L1629
.L1632:
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1636:
	mov	r1, #32
	ldr	r0, [r5, #2308]
	bl	BsGet
	str	r0, [r4, #40]
	b	.L1631
.L1635:
	mov	r2, r0
	ldr	r1, .L1637+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1617
.L1627:
	ldr	r1, .L1637+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1617
.L1638:
	.align	2
.L1637:
	.word	.LC124
	.word	.LC125
	.word	.LC126
	UNWIND(.fnend)
	.size	HEVC_Sei_Tone_Mapping_Info, .-HEVC_Sei_Tone_Mapping_Info
	.align	2
	.global	HEVC_Sei_Frame_Packing_Arrangement
	.type	HEVC_Sei_Frame_Packing_Arrangement, %function
HEVC_Sei_Frame_Packing_Arrangement:
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
	mov	r5, r1
	mov	r4, r0
	ldr	r1, .L1648
	ldr	r0, [r0, #2308]
	bl	HEVC_ue_v
	mov	r1, #1
	str	r0, [r5, #12]
	ldr	r0, [r4, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	bne	.L1640
	mov	r1, #7
	ldr	r0, [r4, #2308]
	bl	BsGet
	sub	r3, r0, #3
	str	r0, [r5, #16]
	cmp	r3, #2
	bhi	.L1647
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #6
	strb	r0, [r5, #1]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #20]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #2]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #3]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #4]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #5]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #6]
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r5, #7]
	bne	.L1643
	ldr	r3, [r5, #16]
	cmp	r3, #5
	beq	.L1643
	mov	r1, #4
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #4
	str	r0, [r5, #24]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #4
	str	r0, [r5, #28]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #4
	str	r0, [r5, #32]
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r5, #36]
.L1643:
	mov	r1, #8
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #40]
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r5, #8]
.L1640:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r5, #9]
	ldr	r0, [r4, #2308]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1647:
	mov	r2, r0
	ldr	r1, .L1648+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1649:
	.align	2
.L1648:
	.word	.LC127
	.word	.LC128
	UNWIND(.fnend)
	.size	HEVC_Sei_Frame_Packing_Arrangement, .-HEVC_Sei_Frame_Packing_Arrangement
	.align	2
	.global	HEVC_Sei_Dec_Unit_Info
	.type	HEVC_Sei_Dec_Unit_Info, %function
HEVC_Sei_Dec_Unit_Info:
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
	cmp	r2, #0
	cmpne	r1, #0
	mov	r5, r1
	mov	r6, r2
	mov	r4, r0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L1655
	ldr	r1, .L1661
	ldr	r0, [r0, #2308]
	bl	HEVC_ue_v
	str	r0, [r5, #4]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	streq	r3, [r5, #8]
	bne	.L1659
.L1653:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	bne	.L1660
.L1654:
	ldr	r0, [r4, #2308]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1659:
	ldr	r1, [r6, #8]
	ldr	r0, [r4, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #8]
	b	.L1653
.L1660:
	ldr	r1, [r6, #12]
	ldr	r0, [r4, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #12]
	b	.L1654
.L1655:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1662:
	.align	2
.L1661:
	.word	.LC129
	UNWIND(.fnend)
	.size	HEVC_Sei_Dec_Unit_Info, .-HEVC_Sei_Dec_Unit_Info
	.align	2
	.global	HEVC_Sei_Active_Param_sets
	.type	HEVC_Sei_Active_Param_sets, %function
HEVC_Sei_Active_Param_sets:
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
	mov	r7, r1
	mov	r6, r0
	mov	r1, #4
	ldr	r0, [r0, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r7, #4]
	ldr	r0, [r6, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7]
	ldr	r0, [r6, #2308]
	bl	BsGet
	ldr	r1, .L1670
	strb	r0, [r7, #1]
	ldr	r0, [r6, #2308]
	bl	HEVC_ue_v
	cmp	r0, #15
	str	r0, [r7, #8]
	bgt	.L1664
	cmp	r0, #0
	addge	r5, r7, #8
	movge	r4, #0
	blt	.L1668
.L1667:
	ldr	r1, .L1670+4
	add	r4, r4, #1
	ldr	r0, [r6, #2308]
	bl	HEVC_ue_v
	str	r0, [r5, #4]!
	ldr	r3, [r7, #8]
	cmp	r3, r4
	bge	.L1667
.L1668:
	ldr	r0, [r6, #2308]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1664:
	ldr	r1, .L1670+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1671:
	.align	2
.L1670:
	.word	.LC130
	.word	.LC132
	.word	.LC131
	UNWIND(.fnend)
	.size	HEVC_Sei_Active_Param_sets, .-HEVC_Sei_Active_Param_sets
	.align	2
	.global	HEVC_Sei_Buf_Period
	.type	HEVC_Sei_Buf_Period, %function
HEVC_Sei_Buf_Period:
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
	add	r4, r0, #24576
	mov	r5, r1
	mov	r6, r0
	ldr	r1, [r4, #2760]
	mov	r0, #0
	ldr	r3, [r4, #2752]
	mov	r8, r2
	str	r0, [r4, #2760]
	str	r1, [r4, #2772]
	add	r3, r3, #1
	ldr	r1, .L1705
	str	r3, [r4, #2752]
	ldr	r0, [r6, #2308]
	bl	HEVC_ue_v
	cmp	r0, #15
	str	r0, [r5, #4]
	bhi	.L1700
	mov	r1, #1
	strb	r1, [r4, #2825]
	ldrb	r3, [r8, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1675
	ldrb	r0, [r5]	@ zero_extendqisi2
.L1676:
	cmp	r0, #0
	bne	.L1701
.L1677:
	mov	r1, #1
	ldr	r0, [r6, #2308]
	bl	BsGet
	strb	r0, [r5, #1]
	ldr	r1, [r8, #32]
	ldr	r0, [r6, #2308]
	add	r1, r1, #1
	bl	BsGet
	add	r0, r0, #1
	str	r0, [r5, #16]
	ldr	r2, [r8, #48]
	cmp	r2, #31
	bhi	.L1702
.L1678:
	mov	r4, #0
	cmp	r4, #0
	mov	r9, r5
	mov	r7, #1
	beq	.L1703
.L1679:
	cmp	r4, #1
	beq	.L1704
.L1683:
	cmp	r7, #1
	bhi	.L1682
.L1681:
	add	r4, r4, #1
	add	r7, r7, #1
	cmp	r4, #0
	add	r9, r9, #16
	bne	.L1679
.L1703:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1683
	ldr	r3, [r8, #48]
	cmn	r3, #1
	movne	r10, r9
	movne	r3, r4
	beq	.L1681
.L1695:
	ldr	r1, [r8, #28]
	add	r3, r3, #1
	ldr	r0, [r6, #2308]
	add	r1, r1, #1
	str	r3, [fp, #-48]
	str	r3, [fp, #-52]
	bl	BsGet
	str	r0, [r10, #20]
	ldr	r1, [r8, #28]
	ldr	r0, [r6, #2308]
	add	r1, r1, #1
	bl	BsGet
	ldr	r3, [fp, #-48]
	str	r0, [r10, #24]
	ldrb	r2, [r8, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1685
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1686
.L1685:
	ldr	r1, [r8, #28]
	ldr	r0, [r6, #2308]
	add	r1, r1, #1
	str	r3, [fp, #-48]
	bl	BsGet
	str	r0, [r10, #28]
	ldr	r1, [r8, #28]
	ldr	r0, [r6, #2308]
	add	r1, r1, #1
	bl	BsGet
	ldr	r3, [fp, #-48]
	str	r0, [r10, #32]
.L1686:
	ldr	r2, [r8, #48]
	add	r10, r10, #32
	add	r2, r2, #1
	cmp	r2, r3
	bhi	.L1695
	b	.L1683
.L1682:
	ldr	r0, [r6, #2308]
	bl	BsToNextByte
	mov	r0, #0
.L1699:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1702:
	ldr	r1, .L1705+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r8, #48]
	b	.L1678
.L1701:
	ldr	r1, [r8, #32]
	ldr	r0, [r6, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #8]
	ldr	r1, [r8, #36]
	ldr	r0, [r6, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #12]
	b	.L1677
.L1675:
	ldr	r0, [r6, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5]
	b	.L1676
.L1700:
	mov	r2, r0
	ldr	r1, .L1705+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1699
.L1704:
	ldrb	r3, [r8, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1682
	ldr	r3, [r8, #48]
	cmn	r3, #1
	beq	.L1682
	mov	r10, r9
	mov	r3, #0
	b	.L1695
.L1706:
	.align	2
.L1705:
	.word	.LC133
	.word	.LC135
	.word	.LC134
	UNWIND(.fnend)
	.size	HEVC_Sei_Buf_Period, .-HEVC_Sei_Buf_Period
	.align	2
	.global	HEVC_Sei_Pic_Timming
	.type	HEVC_Sei_Pic_Timming, %function
HEVC_Sei_Pic_Timming:
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
	add	r3, r0, #24576
	mov	r5, r2
	mov	r6, r1
	mov	r4, r0
	ldr	r1, [r3, #2756]
	ldr	r2, [r3, #2760]
	add	r1, r1, #1
	str	r1, [r3, #2756]
	add	r2, r2, #1
	str	r2, [r3, #2760]
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1733
.L1708:
	ldrh	r3, [r5, #108]
	cmp	r3, #0
	beq	.L1711
	ldr	r1, [r5, #140]
	ldr	r0, [r4, #2308]
	add	r1, r1, #1
	bl	BsGet
	add	r0, r0, #1
	str	r0, [r6, #12]
	ldr	r1, [r5, #144]
	ldr	r0, [r4, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r6, #16]
	ldrb	r3, [r5, #110]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1734
.L1711:
	ldr	r0, [r4, #2308]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1733:
	mov	r1, #4
	ldr	r0, [r0, #2308]
	bl	BsGet
	mov	r1, #2
	str	r0, [r6, #4]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #8]
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r6]
	b	.L1708
.L1734:
	ldr	r1, [r5, #120]
	ldr	r0, [r4, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r6, #20]
	ldrb	r3, [r5, #110]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1711
	ldrb	r3, [r5, #111]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1711
	ldr	r1, .L1736
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	mov	r1, #1
	str	r0, [r6, #24]
	ldr	r0, [r4, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #1]
	cmp	r0, #0
	bne	.L1735
.L1712:
	ldr	r3, [r6, #24]
	cmp	r3, #255
	bhi	.L1715
	add	r8, r6, #28
	mov	r7, #0
	b	.L1714
.L1713:
	ldr	r3, [r6, #24]
	cmp	r3, r7
	bcc	.L1711
.L1714:
	ldr	r1, .L1736+4
	add	r7, r7, #1
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	str	r0, [r8, #4]!
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1713
	ldr	r1, [r5, #116]
	ldr	r0, [r4, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r8, #1024]
	b	.L1713
.L1735:
	ldr	r1, [r5, #116]
	ldr	r0, [r4, #2308]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r6, #28]
	b	.L1712
.L1715:
	ldr	r1, .L1736+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1737:
	.align	2
.L1736:
	.word	.LC136
	.word	.LC138
	.word	.LC137
	UNWIND(.fnend)
	.size	HEVC_Sei_Pic_Timming, .-HEVC_Sei_Pic_Timming
	.align	2
	.global	HEVC_DecShortTermRefPicSet
	.type	HEVC_DecShortTermRefPicSet, %function
HEVC_DecShortTermRefPicSet:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	subs	r7, r3, #0
	mov	r5, r0
	mov	r6, r1
	mov	r4, r2
	streq	r7, [r2]
	bne	.L1780
.L1741:
	ldr	r1, .L1789
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #16
	str	r0, [r4, #16]
	bhi	.L1781
	ldr	r1, .L1789+4
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #16
	str	r0, [r4, #20]
	bhi	.L1782
	ldr	r6, [r4, #16]
	add	r2, r0, r6
	str	r2, [r4, #24]
	cmp	r2, #16
	bhi	.L1766
	cmp	r6, #0
	addne	r10, r4, #68
	addne	r9, r4, #52
	movne	r7, #0
	movne	r8, r7
	bne	.L1770
	b	.L1768
.L1769:
	str	r8, [r10, #4]!
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r9, #1]!
	ldr	r6, [r4, #16]
	cmp	r6, r7
	bls	.L1783
.L1770:
	ldr	r1, .L1789+8
	add	r7, r7, #1
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	mov	r1, #1
	cmp	r0, #32768
	mvn	r3, r0
	add	r8, r8, r3
	bcc	.L1769
	mov	r2, r0
	ldr	r1, .L1789+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1780:
	mov	r1, #1
	ldr	r0, [r0, #2308]
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4]
	beq	.L1741
	ldr	r8, [r6, #228]
	cmp	r8, r7
	movne	r0, #0
	strne	r0, [r4, #4]
	beq	.L1784
.L1744:
	sub	r3, r7, #1
	rsb	r7, r0, r3
	cmp	r7, r3
	bhi	.L1785
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldr	r1, .L1789+16
	str	r0, [r4, #8]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	mov	r3, #204
	mul	r3, r3, r7
	ldr	r1, [r4, #8]
	mov	r1, r1, asl #1
	add	r9, r6, r3
	rsb	r1, r1, #1
	add	r9, r9, #13120
	add	r9, r9, #8
	add	r0, r0, #1
	str	r0, [r4, #12]
	ldr	r2, [r9, #4]
	mul	r1, r1, r0
	cmp	r2, #16
	str	r1, [fp, #-56]
	bhi	.L1786
	add	r2, r3, #13120
	mov	r10, #0
	add	r2, r2, #60
	add	r8, r4, #35
	add	r6, r6, r2
	str	r10, [fp, #-48]
	str	r10, [fp, #-52]
	mov	r7, r10
	b	.L1753
.L1748:
	sub	r2, r0, #1
	cmp	r2, #1
	bls	.L1787
.L1749:
	strb	r0, [r8, #1]!
	add	r10, r10, #1
	ldr	r2, [r9, #4]
	add	r6, r6, #4
	cmp	r2, r10
	bcc	.L1788
.L1753:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r0, #0
	bne	.L1748
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r0, r0, asl #1
	sub	r2, r0, #1
	cmp	r2, #1
	bhi	.L1749
.L1787:
	ldr	r2, [r9, #4]
	add	r1, r7, #18
	ldr	r3, [fp, #-56]
	sub	ip, r0, #1
	cmp	r2, r10
	clz	ip, ip
	ldrhi	r2, [r6]
	movls	r2, #0
	mov	ip, ip, lsr #5
	add	r2, r2, r3
	str	r2, [r4, r1, asl #2]
	cmp	r2, #0
	add	r2, r4, r7
	add	r7, r7, #1
	ldrlt	r3, [fp, #-52]
	ldrge	r3, [fp, #-48]
	addlt	r3, r3, #1
	strb	ip, [r2, #53]
	addge	r3, r3, #1
	strlt	r3, [fp, #-52]
	strge	r3, [fp, #-48]
	b	.L1749
.L1788:
	add	r2, r2, #1
	ldr	r3, [fp, #-52]
	str	r2, [r4, #28]
	cmp	r7, #16
	ldr	r2, [fp, #-48]
	str	r7, [r4, #24]
	str	r3, [r4, #16]
	str	r2, [r4, #20]
	bhi	.L1754
	cmp	r3, #16
	bhi	.L1754
	cmp	r2, #16
	bhi	.L1754
	cmp	r7, #1
	subhi	r7, r7, #1
	addhi	r8, r4, #72
	addhi	r6, r4, #53
	movhi	r5, #0
	bls	.L1760
.L1759:
	ldr	ip, [r8, #4]!
	cmp	r5, #0
	ldrb	lr, [r6, #1]!	@ zero_extendqisi2
	mov	r2, r5
	movge	r1, r8
	movge	r3, r6
	blt	.L1762
.L1758:
	ldr	r0, [r1, #-4]!
	sub	r3, r3, #1
	cmp	ip, r0
	strlt	r0, [r1, #4]
	ldrltb	r0, [r3]	@ zero_extendqisi2
	strltb	r0, [r3, #1]
	strlt	ip, [r1]
	strltb	lr, [r3]
	subs	r2, r2, #1
	bpl	.L1758
.L1762:
	add	r5, r5, #1
	cmp	r5, r7
	bne	.L1759
.L1760:
	ldr	r3, [fp, #-52]
	movs	r6, r3, lsr #1
	beq	.L1757
	ldr	r3, [fp, #-52]
	add	r0, r4, #72
	mov	r1, #0
	add	r2, r3, #17
	add	r3, r3, #52
	add	r3, r4, r3
	add	r2, r4, r2, lsl #2
	add	r4, r4, #53
.L1763:
	ldr	r5, [r2]
	add	r1, r1, #1
	ldr	lr, [r0]
	cmp	r1, r6
	ldrb	ip, [r4]	@ zero_extendqisi2
	str	r5, [r0], #4
	ldrb	r5, [r3]	@ zero_extendqisi2
	strb	r5, [r4], #1
	str	lr, [r2], #-4
	strb	ip, [r3], #-1
	bne	.L1763
.L1757:
	mov	r0, #0
.L1745:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1783:
	ldr	r2, [r4, #24]
.L1768:
	cmp	r2, r6
	bls	.L1757
	add	r8, r6, #17
	add	r9, r6, #52
	add	r9, r4, r9
	mov	r7, #0
	add	r8, r4, r8, lsl #2
.L1771:
	ldr	r1, .L1789+20
	add	r6, r6, #1
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	mov	r1, #1
	add	r0, r0, r1
	add	r7, r7, r0
	str	r7, [r8, #4]!
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r9, #1]!
	ldr	r3, [r4, #24]
	cmp	r3, r6
	bhi	.L1771
	b	.L1757
.L1784:
	ldr	r1, .L1789+24
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	add	r2, r0, #1
	str	r2, [r4, #4]
	cmp	r8, r2
	bcs	.L1744
	mov	r3, r8
	ldr	r1, .L1789+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1745
.L1754:
	ldr	r1, .L1789+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1745
.L1781:
	mov	r2, r0
	ldr	r1, .L1789+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1745
.L1785:
	mov	r2, r7
	ldr	r1, .L1789+40
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1745
.L1786:
	ldr	r1, .L1789+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1745
.L1782:
	ldr	r2, [r4, #16]
	mov	r0, #1
	ldr	r1, .L1789+48
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1745
.L1766:
	mov	r3, #16
	ldr	r1, .L1789+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1745
.L1790:
	.align	2
.L1789:
	.word	.LC145
	.word	.LC147
	.word	.LC150
	.word	.LC151
	.word	.LC142
	.word	.LC152
	.word	.LC139
	.word	.LC140
	.word	.LC144
	.word	.LC146
	.word	.LC141
	.word	.LC143
	.word	.LC148
	.word	.LC149
	UNWIND(.fnend)
	.size	HEVC_DecShortTermRefPicSet, .-HEVC_DecShortTermRefPicSet
	.align	2
	.global	HEVC_DecHrdParam
	.type	HEVC_DecHrdParam, %function
HEVC_DecHrdParam:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r7, r3
	mov	r5, r0
	mov	r8, r1
	beq	.L1813
	cmp	r2, #0
	bne	.L1838
.L1796:
	mov	r4, r8
	mov	r6, #0
.L1810:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #40]
	cmp	r0, #0
	movne	r3, #1
	strneb	r3, [r4, #41]
	beq	.L1839
.L1798:
	ldrb	r2, [r4, #41]	@ zero_extendqisi2
	mov	r3, #0
	strb	r3, [r4, #42]
	cmp	r2, r3
	str	r3, [r4, #48]
	beq	.L1799
	ldr	r1, .L1846
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldrb	r3, [r4, #42]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r4, #44]
	beq	.L1801
.L1842:
	ldr	r2, [r4, #48]
	cmp	r2, #31
	bhi	.L1840
.L1803:
	mov	r2, r4
	mov	r10, #0
.L1812:
	cmp	r10, #0
	beq	.L1841
	cmp	r10, #1
	beq	.L1836
.L1809:
	add	r10, r10, #1
	add	r2, r2, #4
	cmp	r10, #2
	bne	.L1812
.L1808:
	add	r6, r6, #1
	add	r4, r4, #1280
	cmp	r7, r6
	add	r4, r4, #12
	bcs	.L1810
	mov	r0, #0
.L1835:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1799:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r3, r0
	strb	r3, [r4, #42]
	cmp	r3, #0
	bne	.L1842
.L1801:
	ldr	r1, .L1846+4
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	mov	r2, r0
	cmp	r2, #31
	str	r0, [r4, #48]
	bls	.L1803
.L1840:
	ldr	r1, .L1846+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #48]
	b	.L1803
.L1839:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r4, #41]
	b	.L1798
.L1838:
	mov	r1, #1
	ldr	r0, [r0, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8]
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r8, #1]
	ldrh	r3, [r8]
	cmp	r3, #0
	beq	.L1796
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r8, #2]
	cmp	r0, #0
	bne	.L1843
.L1794:
	mov	r1, #4
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #4
	str	r0, [r8, #16]
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldrb	r3, [r8, #2]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r8, #20]
	bne	.L1844
.L1795:
	mov	r1, #5
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #5
	str	r0, [r8, #28]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #5
	str	r0, [r8, #32]
	ldr	r0, [r5, #2308]
	bl	BsGet
	str	r0, [r8, #36]
	b	.L1796
.L1813:
	mvn	r0, #0
	b	.L1835
.L1844:
	mov	r1, #4
	ldr	r0, [r5, #2308]
	bl	BsGet
	str	r0, [r8, #24]
	b	.L1795
.L1843:
	mov	r1, #8
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #5
	str	r0, [r8, #4]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r8, #8]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #5
	strb	r0, [r8, #3]
	ldr	r0, [r5, #2308]
	bl	BsGet
	str	r0, [r8, #36]
	b	.L1794
.L1841:
	ldrb	r1, [r8]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1845
	mov	r9, r2
	str	r10, [fp, #-48]
	b	.L1826
.L1811:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	str	r2, [fp, #-52]
	bl	BsGet
	ldr	r3, [fp, #-48]
	add	r9, r9, #8
	ldr	r2, [fp, #-52]
	str	r0, [r9, #1068]
	ldr	r1, [r4, #48]
	cmp	r1, r3
	bcc	.L1809
.L1826:
	ldr	r3, [fp, #-48]
	ldr	r1, .L1846+12
	ldr	r0, [r5, #2308]
	add	r3, r3, #1
	str	r2, [fp, #-52]
	str	r3, [fp, #-48]
	bl	HEVC_ue_v
	ldr	r1, .L1846+16
	str	r0, [r9, #52]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r2, [fp, #-52]
	str	r0, [r9, #308]
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1811
	ldr	r1, .L1846+20
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1846+24
	str	r0, [r9, #564]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r2, [fp, #-52]
	str	r0, [r9, #820]
	b	.L1811
.L1845:
	add	r2, r2, #4
	mov	r10, #1
.L1836:
	ldrb	r1, [r8, #1]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1808
	mov	r3, #0
	mov	r9, r2
	str	r3, [fp, #-48]
	b	.L1826
.L1847:
	.align	2
.L1846:
	.word	.LC153
	.word	.LC154
	.word	.LC155
	.word	.LC156
	.word	.LC157
	.word	.LC158
	.word	.LC159
	UNWIND(.fnend)
	.size	HEVC_DecHrdParam, .-HEVC_DecHrdParam
	.align	2
	.global	HEVC_ProcessVPS
	.type	HEVC_ProcessVPS, %function
HEVC_ProcessVPS:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 112
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #124)
	sub	sp, sp, #124
	mov	r6, r1
	mov	r7, r0
	mov	r1, #2
	ldr	r0, [r0, #2308]
	bl	BsGet
	cmp	r0, #3
	str	r0, [r6, #12]
	beq	.L1849
	mov	r2, r0
	ldr	r1, .L1901
	mov	r0, #1
	bl	dprint_vfmw
.L1849:
	mov	r1, #6
	ldr	r0, [r7, #2308]
	bl	BsGet
	cmp	r0, #63
	str	r0, [r6, #16]
	bhi	.L1893
	mov	r1, #3
	ldr	r0, [r7, #2308]
	bl	BsGet
	cmp	r0, #7
	str	r0, [fp, #-152]
	bhi	.L1894
	ldr	r3, [fp, #-152]
	mov	r1, #1
	add	r5, r3, #1
	str	r5, [r6, #20]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r6, #1]
	ldr	r0, [r7, #2308]
	bl	BsGet
	movw	r3, #65535
	cmp	r0, r3
	str	r0, [r6, #24]
	bne	.L1895
	add	r1, r6, #12416
	ldr	r3, [fp, #-152]
	add	r1, r1, #20
	mov	r2, #1
	mov	r0, r7
	bl	HEVC_DecPTL
	subs	r4, r0, #0
	bne	.L1896
	mov	r1, #1
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldr	r3, [fp, #-152]
	cmp	r3, #0
	addge	r8, r6, #48
	strb	r0, [r6, #7]
	bge	.L1861
	b	.L1859
.L1858:
	add	r4, r4, #1
	cmp	r4, r5
	beq	.L1859
.L1861:
	ldr	r1, .L1901+4
	ldr	r0, [r7, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1901+8
	add	r0, r0, #1
	str	r0, [r8, #4]!
	ldr	r0, [r7, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1901+12
	str	r0, [r8, #32]
	ldr	r0, [r7, #2308]
	bl	HEVC_ue_v
	str	r0, [r8, #64]
	ldrb	r3, [r6, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1858
	ldr	r3, [fp, #-152]
	add	r2, r4, #1
	cmp	r3, r2
	blt	.L1859
	add	r3, r4, #13
	add	r3, r6, r3, lsl #2
.L1860:
	ldr	r1, [r6, #52]
	add	r2, r2, #1
	cmp	r2, r5
	str	r1, [r3, #4]!
	ldr	r1, [r6, #84]
	str	r1, [r3, #32]
	ldr	r1, [r6, #116]
	str	r1, [r3, #64]
	bne	.L1860
.L1859:
	mov	r1, #6
	ldr	r0, [r7, #2308]
	bl	BsGet
	ldr	r1, .L1901+16
	str	r0, [r6, #28]
	ldr	r0, [r7, #2308]
	bl	HEVC_ue_v
	cmp	r0, #1024
	str	r0, [r6, #32]
	bhi	.L1897
	cmp	r0, #1
	ble	.L1862
	ldr	r10, .L1901+20
	add	r9, r6, #148
	mov	r8, #1
.L1863:
	ldr	r3, [r6, #28]
	cmp	r3, #0
	movgt	r5, r9
	movgt	r4, #0
	ble	.L1865
.L1864:
	ldr	r2, .L1901+24
	mov	r3, r8
	str	r4, [sp]
	mov	r1, #100
	ldr	ip, [r10, #68]
	sub	r0, fp, #144
	blx	ip
	mov	r1, #1
	ldr	r0, [r7, #2308]
	add	r4, r4, r1
	bl	BsGet
	str	r0, [r5, #4]!
	ldr	r2, [r6, #28]
	cmp	r2, r4
	bgt	.L1864
.L1865:
	ldr	r3, [r6, #32]
	add	r8, r8, #1
	add	r9, r9, #4
	cmp	r3, r8
	bgt	.L1863
.L1862:
	mov	r1, #1
	ldr	r0, [r7, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #6]
	cmp	r0, #0
	beq	.L1871
	mov	r1, #32
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r6, #36]
	ldr	r0, [r7, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #40]
	ldr	r0, [r7, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #5]
	cmp	r0, #0
	bne	.L1898
.L1869:
	ldr	r1, .L1901+28
	ldr	r0, [r7, #2308]
	bl	HEVC_ue_v
	cmp	r0, #1024
	str	r0, [r6, #48]
	bcs	.L1899
	cmp	r0, #0
	ble	.L1871
	add	r9, r6, #8192
	mov	r3, #1
	ldr	r1, .L1901+32
	add	r8, r6, #12800
	str	r3, [r9, #148]
	add	r5, r6, #4224
	ldr	r0, [r7, #2308]
	add	r8, r8, #20
	bl	HEVC_ue_v
	add	r3, r6, #4096
	add	r5, r5, #24
	mov	r4, #0
	str	r0, [r3, #148]
	ldr	r0, [r9, #148]
	ldr	r9, [fp, #-152]
.L1872:
	uxtb	r2, r0
	mov	r1, r8
	mov	r3, r9
	mov	r0, r7
	bl	HEVC_DecHrdParam
	add	r4, r4, #1
	ldr	r1, .L1901+32
	cmp	r0, #0
	bne	.L1900
	ldr	r3, [r6, #48]
	cmp	r3, r4
	ble	.L1871
	ldr	r0, [r7, #2308]
	bl	HEVC_ue_v
	cmp	r4, #0
	mov	r1, #1
	str	r0, [r5], #4
	ldrle	r0, [r5, #4092]
	ble	.L1872
	ldr	r0, [r7, #2308]
	bl	BsGet
	str	r0, [r5, #4092]
	b	.L1872
.L1871:
	mov	r1, #1
	ldr	r0, [r7, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #3]
	cmp	r0, #0
	bne	.L1888
.L1868:
	mov	r0, #0
.L1851:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1877:
	ldr	r0, [r7, #2308]
	bl	BsGet
	strb	r0, [r6, #4]
.L1888:
	mov	r0, r7
	bl	HEVC_MoreRbspData
	mov	r1, #1
	cmp	r0, #0
	bne	.L1877
	b	.L1868
.L1900:
	ldr	r1, .L1901+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1898:
	ldr	r1, .L1901+40
	ldr	r0, [r7, #2308]
	bl	HEVC_ue_v
	str	r0, [r6, #44]
	b	.L1869
.L1895:
	ldr	r1, .L1901+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1851
.L1893:
	ldr	r1, .L1901+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1851
.L1896:
	ldr	r1, .L1901+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1851
.L1894:
	mov	r2, r0
	ldr	r1, .L1901+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1851
.L1897:
	mov	r2, r0
	mov	r3, #1024
	ldr	r1, .L1901+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1851
.L1899:
	mov	r2, r0
	mov	r3, #1024
	ldr	r1, .L1901+64
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1851
.L1902:
	.align	2
.L1901:
	.word	.LC160
	.word	.LC166
	.word	.LC167
	.word	.LC168
	.word	.LC165
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC170
	.word	.LC172
	.word	.LC174
	.word	.LC175
	.word	.LC171
	.word	.LC163
	.word	.LC161
	.word	.LC164
	.word	.LC162
	.word	.LC169
	.word	.LC173
	UNWIND(.fnend)
	.size	HEVC_ProcessVPS, .-HEVC_ProcessVPS
	.align	2
	.global	HEVC_DecVPS
	.type	HEVC_DecVPS, %function
HEVC_DecVPS:
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
	mov	r1, #4
	mov	r4, r0
	ldr	r0, [r0, #2308]
	bl	BsGet
	cmp	r0, #15
	mov	r6, r0
	bhi	.L1909
	movw	r7, #23196
	ldr	r3, [r4, #2312]
	mul	r5, r7, r0
	add	r2, r3, r5
	ldrb	r9, [r3, r5]	@ zero_extendqisi2
	cmp	r9, #0
	bne	.L1910
	str	r0, [r2, #8]
	mov	r0, r4
	ldr	r1, [r4, #2312]
	add	r1, r1, r5
	bl	HEVC_ProcessVPS
	subs	r8, r0, #0
	bne	.L1911
	ldr	r3, [r4, #2312]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2312]
	strb	r2, [r3, r5]
.L1905:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1910:
	ldr	r9, .L1913
	mov	r0, r4
	ldr	r1, .L1913+4
	str	r6, [r9, #-3456]
	bl	HEVC_ProcessVPS
	subs	r8, r0, #0
	bne	.L1912
	ldr	ip, .L1913+8
	mov	r3, #1
	ldr	r0, [r4, #2312]
	mov	r2, r7
	ldr	r1, .L1913+4
	add	r0, r0, r5
	ldr	ip, [ip, #56]
	strb	r3, [r9, #-3462]
	strb	r3, [r9, #-3464]
	blx	ip
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1912:
	mov	r2, r6
	ldr	r1, .L1913+12
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L1905
.L1909:
	ldr	r1, .L1913+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1905
.L1911:
	mov	r2, r6
	ldr	r1, .L1913+20
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #2312]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2312]
	strb	r9, [r3, r5]
	b	.L1905
.L1914:
	.align	2
.L1913:
	.word	.LANCHOR5
	.word	.LANCHOR5-3464
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC177
	.word	.LC176
	.word	.LC178
	UNWIND(.fnend)
	.size	HEVC_DecVPS, .-HEVC_DecVPS
	.align	2
	.global	HEVC_DecVuiParam
	.type	HEVC_DecVuiParam, %function
HEVC_DecVuiParam:
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
	ldr	r3, .L1964
	mov	r5, r0
	mov	r4, r1
	add	ip, r3, #16
	mov	r6, r2
	ldmia	r3, {r0, r1, r2, r3}
	sub	lr, fp, #60
	stmia	lr, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	sub	ip, fp, #28
	stmdb	ip, {r0, r1, r2, r3}
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4]
	cmp	r0, #0
	bne	.L1955
.L1917:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #1]
	cmp	r0, #0
	bne	.L1956
.L1919:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3]
	cmp	r0, #0
	bne	.L1957
.L1921:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #6]
	cmp	r0, #0
	bne	.L1958
.L1923:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #7]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #8]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #9]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #10]
	cmp	r0, #0
	beq	.L1924
	ldr	r1, .L1964+4
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	str	r0, [r4, #60]
	ldr	r3, [r6, #64]
	cmp	r3, #3
	bhi	.L1959
	sub	r2, fp, #28
	ldr	r1, .L1964+8
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-32]
	mul	r0, r3, r0
	str	r0, [r4, #60]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	sub	r2, fp, #28
	ldr	r1, .L1964+12
	str	r0, [r4, #64]
	ldr	r3, [r6, #64]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-32]
	mul	r0, r3, r0
	str	r0, [r4, #64]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	sub	r2, fp, #28
	ldr	r1, .L1964+16
	str	r0, [r4, #68]
	ldr	r3, [r6, #64]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-16]
	mul	r0, r3, r0
	str	r0, [r4, #68]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	sub	r2, fp, #28
	str	r0, [r4, #72]
	ldr	r3, [r6, #64]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-16]
	mul	r0, r3, r0
	str	r0, [r4, #72]
.L1924:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #11]
	cmp	r0, #0
	beq	.L1927
	mov	r1, #32
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #76]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #80]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #12]
	cmp	r0, #0
	bne	.L1960
.L1928:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #13]
	cmp	r0, #0
	bne	.L1961
.L1927:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #14]
	cmp	r0, #0
	moveq	r3, r0
	bne	.L1962
.L1926:
	mov	r0, r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1962:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #15]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #16]
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldr	r1, .L1964+20
	strb	r0, [r4, #17]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #4096
	str	r0, [r4, #88]
	bcs	.L1963
.L1929:
	ldr	r1, .L1964+24
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1964+28
	str	r0, [r4, #92]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1964+32
	str	r0, [r4, #96]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1964+36
	str	r0, [r4, #100]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	mov	r3, #0
	str	r0, [r4, #104]
	mov	r0, r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1958:
	ldr	r1, .L1964+40
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1964+44
	str	r0, [r4, #52]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	str	r0, [r4, #56]
	b	.L1923
.L1955:
	mov	r1, #8
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r0, #255
	str	r0, [r4, #24]
	bne	.L1917
	mov	r1, #16
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #28]
	ldr	r0, [r5, #2308]
	bl	BsGet
	str	r0, [r4, #32]
	b	.L1917
.L1957:
	mov	r1, #3
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #36]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #4]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #5]
	cmp	r0, #0
	beq	.L1921
	mov	r1, #8
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #8
	str	r0, [r4, #40]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #8
	str	r0, [r4, #44]
	ldr	r0, [r5, #2308]
	bl	BsGet
	str	r0, [r4, #48]
	b	.L1921
.L1956:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r4, #2]
	b	.L1919
.L1961:
	ldr	r3, [r6, #56]
	mov	r2, #1
	add	r1, r4, #108
	mov	r0, r5
	bl	HEVC_DecHrdParam
	cmp	r0, #0
	beq	.L1927
	ldr	r1, .L1964+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1926
.L1960:
	ldr	r1, .L1964+52
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	str	r0, [r4, #84]
	b	.L1928
.L1963:
	ldr	r1, .L1964+56
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1929
.L1959:
	ldr	r1, .L1964+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1926
.L1965:
	.align	2
.L1964:
	.word	.LANCHOR1+512
	.word	.LC181
	.word	.LC183
	.word	.LC184
	.word	.LC185
	.word	.LC188
	.word	.LC190
	.word	.LC191
	.word	.LC192
	.word	.LC193
	.word	.LC179
	.word	.LC180
	.word	.LC187
	.word	.LC186
	.word	.LC189
	.word	.LC182
	UNWIND(.fnend)
	.size	HEVC_DecVuiParam, .-HEVC_DecVuiParam
	.align	2
	.global	HEVC_se_v
	.type	HEVC_se_v, %function
HEVC_se_v:
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
	mov	r1, #32
	mov	r5, r0
	bl	BsShow
	bl	ZerosMS_32
	cmp	r0, #31
	bls	.L1971
	mvn	r0, #-2147483648
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1971:
	add	r6, r0, #1
	mov	r1, r0
	mov	r0, r5
	bl	BsSkip
	mov	r1, r6
	mov	r0, r5
	bl	BsShow
	mov	r1, r6
	sub	r0, r0, #1
	tst	r0, #1
	mov	r3, r0, asr #1
	mov	r0, r5
	addne	r4, r3, #1
	rsbeq	r4, r3, #0
	bl	BsSkip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_se_v, .-HEVC_se_v
	.align	2
	.global	HEVC_Sei_Structure_Of_Pictures_Info
	.type	HEVC_Sei_Structure_Of_Pictures_Info, %function
HEVC_Sei_Structure_Of_Pictures_Info:
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
	mov	r9, r1
	mov	r5, r0
	ldr	r1, .L1984
	ldr	r0, [r0, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L1984+4
	str	r0, [r9]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #1024
	str	r0, [r9, #4]
	bhi	.L1973
	cmp	r0, #0
	beq	.L1979
	add	r7, r9, #4096
	add	r8, r9, #12288
	add	r7, r7, #8
	add	r8, r8, #8
	add	r6, r9, #4
	mov	r4, #0
	b	.L1978
.L1977:
	ldr	r3, [r9, #4]
	add	r8, r8, #4
	cmp	r3, r4
	bls	.L1979
.L1978:
	mov	r1, #6
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #3
	str	r0, [r6, #4]!
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldr	r1, .L1984+8
	str	r0, [r7], #4
	ldr	r3, [r6]
	sub	r3, r3, #19
	cmp	r3, #1
	bls	.L1976
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	str	r0, [r7, #4092]
.L1976:
	cmp	r4, #0
	add	r4, r4, #1
	beq	.L1977
	ldr	r1, .L1984+12
	add	r8, r8, #4
	ldr	r0, [r5, #2308]
	bl	HEVC_se_v
	str	r0, [r8, #-4]
	ldr	r3, [r9, #4]
	cmp	r3, r4
	bhi	.L1978
.L1979:
	ldr	r0, [r5, #2308]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1973:
	ldr	r1, .L1984+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1985:
	.align	2
.L1984:
	.word	.LC194
	.word	.LC195
	.word	.LC197
	.word	.LC198
	.word	.LC196
	UNWIND(.fnend)
	.size	HEVC_Sei_Structure_Of_Pictures_Info, .-HEVC_Sei_Structure_Of_Pictures_Info
	.align	2
	.global	HEVC_Sei_Recovery_Point
	.type	HEVC_Sei_Recovery_Point, %function
HEVC_Sei_Recovery_Point:
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
	cmp	r1, #0
	cmpne	r0, #0
	mov	r4, r0
	mov	r6, r1
	moveq	r5, #1
	movne	r5, #0
	beq	.L1988
	ldr	r1, .L1989
	ldr	r0, [r0, #2308]
	bl	HEVC_se_v
	mov	r1, #1
	str	r0, [r6, #4]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6]
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r6, #1]
	ldr	r0, [r4, #2308]
	bl	BsToNextByte
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1988:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1990:
	.align	2
.L1989:
	.word	.LC199
	UNWIND(.fnend)
	.size	HEVC_Sei_Recovery_Point, .-HEVC_Sei_Recovery_Point
	.align	2
	.global	HEVC_Sei_Payload
	.type	HEVC_Sei_Payload, %function
HEVC_Sei_Payload:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r6, r1
	ldr	r1, [r0, #2332]
	mov	r4, r0
	ldr	r7, [fp, #4]
	ldr	r1, [r1, #16]
	cmp	r1, #39
	beq	.L2035
	cmp	r3, #5
	beq	.L2019
	cmp	r3, #132
	beq	.L2020
	cmp	r7, #0
	movne	r5, #0
	beq	.L2014
.L2022:
	add	r5, r5, #1
	mov	r1, #8
	ldr	r0, [r4, #2308]
	bl	BsGet
	cmp	r5, r7
	bne	.L2022
	b	.L2014
.L2019:
	mov	r2, r7
	add	r1, r6, #3200
	bl	HEVC_Sei_User_Data_Unregistered
	cmp	r0, #0
	bne	.L2016
.L2014:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_Sei_More_Data_In_Payload
	cmp	r0, #0
	bne	.L2036
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2036:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_Sei_Payload_Extension_Present
	cmp	r0, #0
	bne	.L2037
.L2023:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r6, #12]
	b	.L2024
.L2025:
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r6, #16]
.L2024:
	ldr	r0, [r4, #2308]
	bl	BsIsByteAligned
	mov	r1, #1
	cmp	r0, #0
	beq	.L2025
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2035:
	cmp	r3, #147
	ldrls	pc, [pc, r3, asl #2]
	b	.L1993
.L1995:
	.word	.L1994
	.word	.L1996
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1997
	.word	.L1998
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1999
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L2000
	.word	.L1993
	.word	.L2001
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L2002
	.word	.L2003
	.word	.L2004
	.word	.L2005
	.word	.L1993
	.word	.L2006
	.word	.L2007
	.word	.L1993
	.word	.L1993
	.word	.L2008
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L1993
	.word	.L2009
	.word	.L1993
	.word	.L1993
	.word	.L2010
.L2020:
	add	r1, r6, #23808
	add	r1, r1, #16
	bl	HEVC_Sei_Decoded_Picture_Hash
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2037:
	ldr	r1, .L2038
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	str	r0, [r6, #8]
	b	.L2023
.L2010:
	add	r1, r6, #3472
	add	r1, r1, #10
	bl	HEVC_Sei_HLGHDR_Comapitibility_Info
	cmp	r0, #0
	beq	.L2014
.L2016:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1994:
	add	r2, r2, #2720
	add	r1, r6, #24
	add	r2, r2, #12
	bl	HEVC_Sei_Buf_Period
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L1996:
	add	r2, r2, #2624
	add	r1, r6, #1088
	bl	HEVC_Sei_Pic_Timming
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L1997:
	mov	r2, r7
	add	r1, r6, #3200
	bl	HEVC_Sei_User_Data_Unregistered
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L1998:
	add	r1, r6, #3360
	bl	HEVC_Sei_Recovery_Point
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L1999:
	add	r1, r6, #3472
	add	r1, r1, #12
	bl	HEVC_Sei_Tone_Mapping_Info
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2000:
	add	r1, r6, #3360
	add	r1, r1, #8
	bl	HEVC_Sei_Frame_Packing_Arrangement
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2001:
	add	r1, r6, #3408
	add	r1, r1, #4
	bl	HEVC_Sei_Display_Orientation
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2002:
	add	r1, r6, #6592
	mov	r2, r7
	add	r1, r1, #52
	bl	HEVC_Sei_Structure_Of_Pictures_Info
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2003:
	add	r1, r6, #3264
	add	r1, r1, #4
	bl	HEVC_Sei_Active_Param_sets
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2004:
	add	r2, r2, #2720
	add	r1, r6, #3344
	add	r2, r2, #12
	bl	HEVC_Sei_Dec_Unit_Info
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2005:
	add	r1, r6, #3408
	add	r1, r1, #12
	bl	HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2006:
	mov	r3, r7
	mov	r1, r6
	bl	HEVC_Sei_Scalable_Nesting
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2007:
	add	r1, r6, #3424
	add	r1, r1, #4
	bl	HEVC_Sei_Region_Refresh_Info
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2008:
	add	r1, r6, #3424
	add	r1, r1, #8
	bl	HEVC_Sei_Mastering_Display_Colour_Volume
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L2009:
	add	r1, r6, #3472
	add	r1, r1, #4
	bl	HEVC_Sei_Content_Light_Level_Information
	cmp	r0, #0
	beq	.L2014
	b	.L2016
.L1993:
	cmp	r7, #0
	movne	r5, #0
	beq	.L2014
.L2017:
	add	r5, r5, #1
	mov	r1, #8
	ldr	r0, [r4, #2308]
	bl	BsGet
	cmp	r5, r7
	bne	.L2017
	b	.L2014
.L2039:
	.align	2
.L2038:
	.word	.LC200
	UNWIND(.fnend)
	.size	HEVC_Sei_Payload, .-HEVC_Sei_Payload
	.align	2
	.global	HEVC_SeiMessage
	.type	HEVC_SeiMessage, %function
HEVC_SeiMessage:
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
	mov	r5, #0
	mov	r4, r0
	mov	r6, r1
	mov	r7, r2
	ldr	r0, [r0, #2308]
	b	.L2041
.L2043:
	bl	BsSkip
	ldr	r0, [r4, #2308]
	add	r5, r5, #255
	ldr	r3, [r0, #24]
	ldr	r2, [r0, #16]
	add	r1, r3, #8
	cmp	r1, r2
	bhi	.L2049
.L2041:
	mov	r1, #8
	bl	BsShow
	mov	r1, #8
	cmp	r0, #255
	ldr	r0, [r4, #2308]
	beq	.L2043
	bl	BsGet
	mov	r8, #0
	mov	r9, r0
	str	r0, [r6]
	ldr	r0, [r4, #2308]
	b	.L2044
.L2045:
	bl	BsSkip
	ldr	r0, [r4, #2308]
	add	r8, r8, #255
	ldr	r3, [r0, #24]
	ldr	r2, [r0, #16]
	add	r1, r3, #8
	cmp	r1, r2
	bhi	.L2050
.L2044:
	mov	r1, #8
	bl	BsShow
	mov	r1, #8
	cmp	r0, #255
	ldr	r0, [r4, #2308]
	beq	.L2045
	bl	BsGet
	add	r3, r9, r5
	mov	r2, r7
	mov	r1, r6
	str	r0, [r6, #4]
	add	r8, r0, r8
	mov	r0, r4
	str	r8, [sp]
	bl	HEVC_Sei_Payload
	cmp	r0, #0
	bne	.L2051
.L2042:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2049:
	str	r2, [sp]
	mov	r0, #1
	movw	r2, #3041
	ldr	r1, .L2052
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2050:
	str	r2, [sp]
	mov	r0, #1
	movw	r2, #3063
	ldr	r1, .L2052
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2051:
	ldr	r1, .L2052+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2042
.L2053:
	.align	2
.L2052:
	.word	.LC201
	.word	.LC202
	UNWIND(.fnend)
	.size	HEVC_SeiMessage, .-HEVC_SeiMessage
	.align	2
	.global	HEVC_DecSEI
	.type	HEVC_DecSEI, %function
HEVC_DecSEI:
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
	add	r6, r0, #3440
	add	r5, r0, #27392
	mov	r4, r0
	add	r6, r6, #8
	add	r5, r5, #28
	b	.L2058
.L2055:
	ldr	r0, [r4, #2308]
	bl	BsIsByteAligned
	cmp	r0, #0
	mov	r0, r4
	beq	.L2063
	bl	HEVC_More_Rbsp_Data
	subs	r7, r0, #0
	beq	.L2064
.L2058:
	mov	r2, r5
	mov	r1, r6
	mov	r0, r4
	bl	HEVC_SeiMessage
	cmp	r0, #0
	beq	.L2055
	ldr	r1, .L2065
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
.L2056:
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2063:
	ldr	r1, .L2065+4
	mvn	r7, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2064:
	ldr	r0, [r4, #2308]
	mov	r1, #8
	bl	BsGet
	cmp	r0, #128
	beq	.L2056
	mov	r2, r0
	ldr	r1, .L2065+8
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	b	.L2056
.L2066:
	.align	2
.L2065:
	.word	.LC203
	.word	.LC204
	.word	.LC205
	UNWIND(.fnend)
	.size	HEVC_DecSEI, .-HEVC_DecSEI
	.align	2
	.global	HEVC_Sei_Scalable_Nesting
	.type	HEVC_Sei_Scalable_Nesting, %function
HEVC_Sei_Scalable_Nesting:
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
	mov	r4, r0
	mov	r7, r1
	ldr	r0, [r0, #2308]
	mov	r1, #1
	mov	r9, r2
	bl	BsGet
	add	r8, r7, #20480
	mov	r1, #1
	strb	r0, [r8, #2556]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	uxtb	r0, r0
	strb	r0, [r8, #2557]
	cmp	r0, #0
	ldr	r0, [r4, #2308]
	beq	.L2068
	bl	BsGet
	ldr	r1, .L2084
	strb	r0, [r8, #2558]
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	cmp	r0, #63
	str	r0, [r8, #2564]
	bhi	.L2081
	ldrb	r6, [r8, #2558]	@ zero_extendqisi2
	cmp	r0, r6
	addcs	r5, r6, #5760
	addcs	r5, r5, #1
	addcs	r5, r7, r5, lsl #2
	bcc	.L2074
.L2073:
	mov	r1, #3
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r1, .L2084+4
	add	r6, r6, #1
	str	r0, [r5, #4]!
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	str	r0, [r5, #256]
	ldr	r3, [r8, #2564]
	cmp	r3, r6
	bcs	.L2073
.L2074:
	ldr	r0, [r4, #2308]
	bl	BsToNextByte
	mov	r3, #0
	strb	r3, [r8, #2560]
	b	.L2072
.L2077:
	bl	HEVC_MoreRbspData
	cmp	r0, #0
	beq	.L2082
.L2072:
	mov	r2, r9
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_SeiMessage
	cmp	r0, #0
	mov	r0, r4
	beq	.L2077
	ldr	r1, .L2084+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2068:
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r8, #2559]
	cmp	r5, #0
	bne	.L2074
	mov	r1, #3
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r1, .L2084+12
	str	r0, [r8, #3080]
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	cmp	r0, #63
	str	r0, [r8, #3084]
	addls	r6, r7, #23552
	addls	r6, r6, #12
	bhi	.L2083
.L2076:
	mov	r1, #6
	ldr	r0, [r4, #2308]
	bl	BsGet
	add	r5, r5, #1
	str	r0, [r6, #4]!
	ldr	r3, [r8, #3084]
	cmp	r3, r5
	bcs	.L2076
	b	.L2074
.L2082:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2081:
	mov	r2, #64
	ldr	r1, .L2084+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2083:
	ldr	r1, .L2084+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2085:
	.align	2
.L2084:
	.word	.LC206
	.word	.LC208
	.word	.LC203
	.word	.LC209
	.word	.LC207
	.word	.LC210
	UNWIND(.fnend)
	.size	HEVC_Sei_Scalable_Nesting, .-HEVC_Sei_Scalable_Nesting
	.align	2
	.global	HEVC_DecScalingListData
	.type	HEVC_DecScalingListData, %function
HEVC_DecScalingListData:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
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
	ldr	r3, .L2123
	mov	r9, r1
	mov	r6, r0
	add	ip, r9, #1664
	add	r1, r1, #1264
	add	r0, r9, #1392
	ldr	r3, [r3, #52]
	add	r1, r1, #1
	add	r0, r0, #1
	mov	r2, #64
	mov	r4, ip
	blx	r3
	add	r3, r4, #4
	str	r3, [fp, #-76]
	add	r3, r6, #2192
	str	r3, [fp, #-48]
	add	r3, r9, #24
	str	r3, [fp, #-68]
	add	r3, r9, #49
	str	r3, [fp, #-72]
	mov	r3, #0
	str	r3, [fp, #-52]
.L2087:
	ldr	r2, [fp, #-48]
	ldr	r3, [r2, #4]!
	cmp	r3, #0
	str	r2, [fp, #-48]
	beq	.L2106
	ldr	r3, [fp, #-76]
	ldr	r7, [fp, #-72]
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-68]
	str	r3, [fp, #-56]
	mov	r3, #0
	str	r3, [fp, #-60]
.L2105:
	mov	r1, #1
	ldr	r0, [r6, #2308]
	bl	BsGet
	ldr	r3, [fp, #-56]
	uxtb	r0, r0
	strb	r0, [r3, #1]!
	str	r3, [fp, #-56]
	ldr	r3, [fp, #-48]
	ldr	r5, [r3, #-1904]
	ldr	r3, [fp, #-52]
	cmp	r5, #64
	movge	r5, #64
	cmp	r3, #0
	beq	.L2088
	cmp	r0, #0
	add	r4, r6, #324
	beq	.L2119
	ldr	r3, [fp, #-52]
	cmp	r3, #1
	bls	.L2112
	ldr	r1, .L2123+4
	ldr	r0, [r6, #2308]
	bl	HEVC_se_v
	add	r3, r0, #7
	str	r0, [r9, #1660]
	cmp	r3, #254
	bhi	.L2120
	ldr	r3, [fp, #-64]
	add	r8, r0, #8
	str	r8, [r3]
.L2099:
	cmp	r5, #0
	ble	.L2098
	sub	r4, r4, #4
	mov	r10, #0
	b	.L2103
.L2102:
	mov	r8, r8, lsr #24
	ldr	r2, [r4, #4]!
	add	r3, r3, r8
	cmp	r10, r5
	uxtb	r3, r3
	rsb	r8, r8, r3
	strb	r8, [r7, r2]
	beq	.L2098
.L2103:
	ldr	r1, .L2123+8
	add	r10, r10, #1
	ldr	r0, [r6, #2308]
	bl	HEVC_se_v
	add	r3, r8, r0
	add	r2, r0, #128
	add	r3, r3, #256
	cmp	r2, #255
	str	r0, [r9, #1664]
	mov	r8, r3, asr #31
	bls	.L2102
	ldr	r1, .L2123+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2117:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2107:
	ldr	r1, .L2123+16
	ldr	r0, [r6, #2308]
	bl	HEVC_ue_v
	ldr	r3, [fp, #-60]
	cmp	r0, r3
	bhi	.L2109
	ldr	r2, [fp, #-60]
	uxtb	r0, r0
	ldr	r3, [fp, #-56]
	rsb	r1, r0, r2
	strb	r0, [r3, #1560]
	uxtb	r1, r1
	strb	r1, [r3, #1584]
.L2093:
	ldr	r3, [fp, #-60]
	cmp	r1, r3
	beq	.L2121
	ldr	r2, [fp, #-52]
	mov	r3, r2, asl #3
	sub	r3, r3, r2, asl #1
	add	r1, r3, r1
	add	r1, r9, r1, lsl #6
	add	r1, r1, #49
.L2096:
	cmp	r1, #0
	beq	.L2122
	ldr	r3, .L2123
	mov	r2, r5
	mov	r0, r7
	ldr	r3, [r3, #52]
	blx	r3
.L2098:
	ldr	r3, [fp, #-48]
	add	r7, r7, #64
	ldr	r2, [fp, #-60]
	ldr	r1, [fp, #-64]
	ldr	r3, [r3]
	add	r2, r2, #1
	add	r1, r1, #4
	str	r2, [fp, #-60]
	cmp	r3, r2
	str	r1, [fp, #-64]
	bhi	.L2105
.L2106:
	ldr	r3, [fp, #-52]
	ldr	r2, [fp, #-68]
	add	r3, r3, #1
	str	r3, [fp, #-52]
	cmp	r3, #4
	ldr	r3, [fp, #-72]
	add	r2, r2, #6
	str	r2, [fp, #-68]
	add	r3, r3, #384
	str	r3, [fp, #-72]
	ldr	r3, [fp, #-76]
	add	r3, r3, #24
	str	r3, [fp, #-76]
	bne	.L2087
	mov	r0, #0
	b	.L2117
.L2112:
	mov	r8, #8
	b	.L2099
.L2119:
	ldr	r1, .L2123+16
	ldr	r0, [r6, #2308]
	bl	HEVC_ue_v
	ldr	r3, [fp, #-60]
	cmp	r0, r3
	bhi	.L2109
	ldr	r3, [fp, #-52]
	uxtb	r0, r0
	ldr	ip, [fp, #-60]
	ldr	r2, [fp, #-56]
	cmp	r3, #1
	rsb	r1, r0, ip
	strb	r0, [r2, #1560]
	uxtb	r1, r1
	strb	r1, [r2, #1584]
	bls	.L2093
	cmp	ip, r1
	mov	r2, ip
	movne	r2, r3
	movne	r3, r3, asl #3
	subne	r3, r3, r2, asl #1
	ldr	r2, [fp, #-64]
	addne	r3, r3, r1
	moveq	r3, #16
	addne	r3, r9, r3, lsl #2
	ldrne	r3, [r3, #1668]
	str	r3, [r2]
	b	.L2093
.L2121:
	mov	r1, r3
	ldr	r0, [fp, #-52]
	bl	HEVC_GetScalingListDefaultAddress
	mov	r1, r0
	b	.L2096
.L2120:
	ldr	r1, .L2123+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2117
.L2122:
	ldr	r1, .L2123+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2117
.L2109:
	ldr	r1, .L2123+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2117
.L2088:
	cmp	r0, #0
	beq	.L2107
	ldr	r4, .L2123+32
	mov	r8, #8
	b	.L2099
.L2124:
	.align	2
.L2123:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC214
	.word	.LC216
	.word	.LC217
	.word	.LC211
	.word	.LC215
	.word	.LC213
	.word	.LC212
	.word	.LANCHOR0+65536
	UNWIND(.fnend)
	.size	HEVC_DecScalingListData, .-HEVC_DecScalingListData
	.align	2
	.global	HEVC_ProcessSPS
	.type	HEVC_ProcessSPS, %function
HEVC_ProcessSPS:
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
	mov	r4, r1
	mov	r5, r0
	ldr	r1, .L2279
	ldr	r0, [r0, #2308]
	bl	HEVC_ue_v
	cmp	r0, #1
	mov	r8, r0
	str	r0, [r4, #64]
	beq	.L2126
	cmp	r0, #3
	bhi	.L2256
	mov	r2, r0
	ldr	r1, .L2279+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2128:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2256:
	ldr	r1, .L2279+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2126:
	ldr	r1, .L2279+12
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #8192
	str	r0, [r4, #72]
	bhi	.L2257
	ldr	r1, .L2279+16
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #4096
	str	r0, [r4, #76]
	bhi	.L2258
	ldr	r2, [r4, #760]
	ldr	r3, [r4, #72]
	cmp	r2, #120
	mul	r0, r3, r0
	beq	.L2132
	bgt	.L2133
	cmp	r2, #60
	beq	.L2134
	ble	.L2259
	cmp	r2, #90
	beq	.L2137
	cmp	r2, #93
	beq	.L2138
	cmp	r2, #63
	bne	.L2131
	mov	r2, #61440
	mov	r3, #245760
	b	.L2139
.L2133:
	cmp	r2, #156
	beq	.L2140
	bgt	.L2141
	cmp	r2, #150
	beq	.L2140
	cmp	r2, #153
	beq	.L2140
	cmp	r2, #123
	beq	.L2132
.L2131:
	ldr	r1, .L2279+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2141:
	cmp	r2, #183
	beq	.L2142
	cmp	r2, #186
	beq	.L2142
	cmp	r2, #180
	bne	.L2131
.L2142:
	mov	r2, #8912896
	mov	r3, #35651584
.L2139:
	cmp	r0, r2
	movls	r3, #16
	strls	r3, [r4, #236]
	bls	.L2144
	cmp	r0, r3, lsr #1
	movls	r3, #12
	strls	r3, [r4, #236]
	bls	.L2144
	add	r3, r3, r3, lsl #1
	cmp	r0, r3, lsr #2
	movls	r3, #8
	movhi	r3, #6
	str	r3, [r4, #236]
.L2144:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4, #80]
	beq	.L2147
	ldr	r1, .L2279+24
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r3, [r4, #64]
	ldr	r6, .L2279+28
	ldr	r1, .L2279+32
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #544]
	mul	r0, r3, r0
	str	r0, [r4, #84]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r2, [r4, #64]
	ldr	r3, [r4, #72]
	ldr	r1, [r4, #84]
	add	r2, r6, r2, lsl #2
	rsb	r3, r1, r3
	ldr	r2, [r2, #544]
	mul	r0, r2, r0
	rsb	r3, r0, r3
	cmp	r3, #0
	str	r0, [r4, #88]
	blt	.L2260
	ldr	r1, .L2279+36
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r3, [r4, #64]
	ldr	r1, .L2279+40
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #560]
	mul	r0, r3, r0
	str	r0, [r4, #92]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, [r4, #64]
	ldr	r2, [r4, #92]
	ldr	r3, [r4, #76]
	add	r6, r6, r1, lsl #2
	rsb	r3, r2, r3
	ldr	r2, [r6, #560]
	mul	r0, r2, r0
	rsb	r3, r0, r3
	cmp	r3, #0
	str	r0, [r4, #96]
	blt	.L2261
.L2147:
	ldr	r1, .L2279+44
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #6
	add	r2, r0, #8
	str	r2, [r4, #100]
	bhi	.L2262
	ldr	r1, .L2279+48
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #6
	add	r2, r0, #8
	str	r2, [r4, #108]
	bhi	.L2263
	ldr	r3, [r4, #100]
	ldr	r1, .L2279+52
	str	r3, [r5, #192]
	ldr	r3, [r4, #108]
	str	r3, [r5, #196]
	ldr	r0, [r4, #108]
	ldr	ip, [r4, #100]
	mov	r3, r0, asl #3
	mov	r2, ip, asl #3
	sub	r3, r3, r0, asl #1
	sub	r2, r2, ip, asl #1
	sub	r3, r3, #48
	sub	r2, r2, #48
	str	r3, [r4, #112]
	str	r2, [r4, #104]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #12
	bhi	.L2264
	add	r0, r0, #4
	mov	r1, #1
	str	r0, [r4, #128]
	mov	r6, r4
	str	r0, [r4, #132]
	mov	r7, #0
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r4, #3]
	b	.L2158
.L2152:
	ldr	r3, [r4, #56]
	add	r7, r7, #1
	cmp	r7, r3
	bhi	.L2157
.L2158:
	ldr	r1, .L2279+56
	add	r6, r6, #4
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L2279+60
	add	r0, r0, #1
	str	r0, [r6, #376]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r1, .L2279+64
	ldr	r9, [r6, #376]
	str	r0, [r6, #408]
	ldr	r0, [r5, #2308]
	ldr	r10, [r4, #380]
	bl	HEVC_ue_v
	str	r0, [r6, #440]
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2152
	ldr	r1, [r4, #56]
	add	r3, r7, #1
	cmp	r3, r1
	addls	r7, r7, #95
	addls	r7, r4, r7, lsl #2
	bhi	.L2157
.L2156:
	ldr	r2, [r4, #380]
	add	r3, r3, #1
	cmp	r3, r1
	str	r2, [r7, #4]!
	ldr	r2, [r4, #412]
	str	r2, [r7, #32]
	ldr	r2, [r4, #444]
	str	r2, [r7, #64]
	bls	.L2156
.L2157:
	ldr	r3, [r4, #236]
	cmp	r9, r10
	ldr	r1, .L2279+68
	movcc	r9, r10
	cmp	r3, r9
	movcs	r3, r9
	str	r3, [r4, #236]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #136]
	bhi	.L2265
	ldr	r1, .L2279+72
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #144]
	bhi	.L2266
	ldr	r3, [r4, #136]
	add	r3, r3, #3
	str	r3, [r4, #140]
	add	r6, r0, r3
	str	r6, [r4, #148]
	sub	r2, r6, #4
	cmp	r2, #2
	bhi	.L2267
	mov	r2, #1
	ldr	r7, [r4, #72]
	mov	r6, r2, asl r6
	mov	r3, r2, asl r3
	str	r6, [r4, #156]
	str	r6, [r4, #160]
	mov	r1, r6
	str	r6, [r4, #164]
	mov	r0, r7
	str	r3, [r4, #152]
	bl	__aeabi_uidivmod
	mov	r0, r7
	cmp	r1, #0
	mov	r1, r6
	beq	.L2161
	bl	__aeabi_uidiv
	add	r0, r0, #1
.L2162:
	ldr	r7, [r4, #76]
	mov	r1, r6
	str	r0, [r4, #240]
	mov	r0, r7
	bl	__aeabi_uidivmod
	mov	r0, r7
	cmp	r1, #0
	mov	r1, r6
	beq	.L2163
	bl	__aeabi_uidiv
	add	r0, r0, #1
.L2164:
	str	r0, [r4, #244]
	ldr	r1, .L2279+76
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #168]
	bhi	.L2268
	ldr	r1, .L2279+80
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #176]
	bhi	.L2269
	ldr	r3, [r4, #168]
	ldr	r2, [r4, #140]
	add	r3, r3, #2
	str	r3, [r4, #172]
	cmp	r3, r2
	bcs	.L2270
	add	r0, r0, r3
	str	r0, [r4, #180]
	cmp	r0, #5
	bhi	.L2168
	ldr	r3, [r4, #148]
	cmp	r0, r3
	bhi	.L2168
	ldr	r1, .L2279+84
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r3, [r4, #148]
	ldr	r2, [r4, #172]
	rsb	r3, r2, r3
	cmp	r0, r3
	str	r0, [r4, #200]
	bhi	.L2271
	ldr	r1, .L2279+88
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	ldr	r3, [r4, #148]
	ldr	r2, [r4, #172]
	rsb	r3, r2, r3
	cmp	r0, r3
	str	r0, [r4, #208]
	bhi	.L2272
	ldr	r3, [r4, #200]
	add	r0, r0, #1
	mov	r1, #0
	str	r0, [r4, #212]
	add	r3, r3, #1
	str	r3, [r4, #204]
	str	r1, [r5, #228]
	mov	ip, #1
	ldr	r3, [r4, #172]
	ldr	r0, [r4, #160]
	ldr	r2, [r4, #144]
	mov	r3, ip, asl r3
	cmp	r3, r0, lsr r2
	bcc	.L2173
	b	.L2273
.L2198:
	mov	r8, r0
.L2173:
	str	r8, [r5, #228]
	add	r0, r8, #1
	ldr	r3, [r4, #172]
	ldr	r1, [r4, #160]
	add	r3, r8, r3
	ldr	r2, [r4, #144]
	mov	r3, ip, asl r3
	cmp	r3, r1, lsr r2
	bcc	.L2198
.L2172:
	mov	r6, #1
	add	r8, r2, r8
	str	r6, [r4, #224]
	mov	r3, #0
	str	r8, [r4, #216]
	mov	r1, r6
	str	r3, [r4, #220]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #5]
	cmp	r0, #0
	beq	.L2174
	mov	r1, r6
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #6]
	cmp	r0, #0
	bne	.L2274
.L2174:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #7]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r4, #8]
	cmp	r6, #0
	bne	.L2175
	ldr	r2, [r4, #76]
	cmp	r2, #64
	bls	.L2275
.L2175:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4, #116]
	beq	.L2176
	mov	r1, #4
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #4
	add	r0, r0, #1
	str	r0, [r4, #120]
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldr	r1, .L2279+92
	add	r0, r0, #1
	str	r0, [r4, #124]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #2
	str	r0, [r4, #184]
	bhi	.L2276
.L2177:
	ldr	r1, .L2279+96
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #2
	str	r0, [r4, #192]
	bhi	.L2277
.L2178:
	ldr	r2, [r4, #184]
	add	r2, r2, #3
	str	r2, [r4, #188]
	add	r0, r0, r2
	str	r0, [r4, #196]
	cmp	r0, #5
	bhi	.L2179
	ldr	r3, [r4, #148]
	cmp	r0, r3
	bhi	.L2179
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r4, #9]
.L2176:
	ldr	r1, .L2279+100
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #64
	str	r0, [r4, #228]
	bhi	.L2181
	cmp	r0, #0
	addne	r7, r4, #13056
	movne	r6, #0
	addne	r7, r7, #52
	bne	.L2186
	b	.L2187
.L2185:
	ldr	r3, [r4, #228]
	cmp	r6, r3
	bcs	.L2187
.L2186:
	mov	r3, r6
	mov	r2, r7
	mov	r1, r4
	mov	r0, r5
	bl	HEVC_DecShortTermRefPicSet
	add	r6, r6, #1
	add	r7, r7, #204
	cmp	r0, #0
	beq	.L2185
	ldr	r1, .L2279+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2138:
	mov	r2, #245760
	mov	r3, #983040
	b	.L2139
.L2163:
	bl	__aeabi_uidiv
	b	.L2164
.L2161:
	bl	__aeabi_uidiv
	b	.L2162
.L2187:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #10]
	cmp	r0, #0
	beq	.L2184
	ldr	r1, .L2279+108
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #32
	str	r0, [r4, #232]
	bhi	.L2190
	cmp	r0, #0
	addne	r8, r4, #244
	addne	r7, r4, #15
	movne	r6, #0
	beq	.L2184
.L2192:
	ldr	r1, [r4, #128]
	add	r6, r6, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	str	r0, [r8, #4]!
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r7, #1]!
	ldr	r3, [r4, #232]
	cmp	r6, r3
	bcc	.L2192
.L2184:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #11]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #12]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #13]
	cmp	r0, #0
	beq	.L2189
	mov	r2, r4
	add	r1, r4, #2624
	mov	r0, r5
	bl	HEVC_DecVuiParam
	cmp	r0, #0
	bne	.L2278
.L2189:
	ldr	ip, [r4, #76]
	ldr	r3, [r4, #72]
	ldr	r2, [r4, #2656]
	ldr	r1, [r4, #2652]
	ldr	r0, [r4, #2648]
	str	ip, [sp]
	bl	HEVCGetDar
	mov	r1, #1
	str	r0, [r4, #2644]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #14]
	cmp	r0, #0
	bne	.L2252
.L2195:
	mov	r3, #1
	mov	r0, #0
	strb	r3, [r4]
	b	.L2128
.L2194:
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r4, #15]
.L2252:
	mov	r0, r5
	bl	HEVC_MoreRbspData
	mov	r1, #1
	cmp	r0, #0
	bne	.L2194
	b	.L2195
.L2259:
	cmp	r2, #0
	beq	.L2136
	cmp	r2, #30
	bne	.L2131
.L2136:
	mov	r2, #9216
	mov	r3, #36864
	b	.L2139
.L2275:
	ldr	r1, .L2279+112
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2279+116
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r0, #0
	beq	.L2128
	ldr	r0, [r5, #244]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
	mvn	r0, #0
	b	.L2128
.L2134:
	mov	r2, #30720
	mov	r3, #122880
	b	.L2139
.L2132:
	mov	r2, #557056
	mov	r3, #2228224
	b	.L2139
.L2140:
	mov	r2, #2228224
	mov	r3, #8912896
	b	.L2139
.L2137:
	mov	r2, #138240
	mov	r3, #552960
	b	.L2139
.L2274:
	add	r1, r4, #860
	mov	r0, r5
	bl	HEVC_DecScalingListData
	cmp	r0, #0
	beq	.L2174
	mov	r0, r6
	ldr	r1, .L2279+120
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2168:
	ldr	r1, .L2279+124
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2277:
	mov	r2, r0
	ldr	r1, .L2279+128
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r0, [r4, #192]
	b	.L2178
.L2276:
	mov	r2, r0
	ldr	r1, .L2279+132
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2177
.L2273:
	mov	r8, r1
	b	.L2172
.L2257:
	mov	r0, r8
	ldr	r1, .L2279+136
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2264:
	ldr	r1, .L2279+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2265:
	ldr	r1, .L2279+144
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2268:
	ldr	r1, .L2279+148
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2179:
	ldr	r1, .L2279+152
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2271:
	ldr	r1, .L2279+156
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2258:
	mov	r0, r8
	ldr	r1, .L2279+160
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2267:
	ldr	r1, .L2279+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2266:
	ldr	r1, .L2279+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2262:
	ldr	r1, .L2279+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2263:
	ldr	r1, .L2279+176
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2270:
	ldr	r1, .L2279+180
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2269:
	ldr	r1, .L2279+184
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2272:
	ldr	r1, .L2279+188
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2181:
	mov	r2, #64
	ldr	r1, .L2279+192
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2260:
	ldr	r1, .L2279+196
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2261:
	ldr	r1, .L2279+200
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2190:
	ldr	r1, .L2279+204
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2278:
	ldr	r1, .L2279+208
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2128
.L2280:
	.align	2
.L2279:
	.word	.LC218
	.word	.LC220
	.word	.LC219
	.word	.LC221
	.word	.LC223
	.word	.LC225
	.word	.LC226
	.word	.LANCHOR1
	.word	.LC227
	.word	.LC229
	.word	.LC230
	.word	.LC232
	.word	.LC234
	.word	.LC236
	.word	.LC238
	.word	.LC239
	.word	.LC240
	.word	.LC241
	.word	.LC243
	.word	.LC246
	.word	.LC248
	.word	.LC252
	.word	.LC254
	.word	.LC258
	.word	.LC260
	.word	.LC263
	.word	.LC265
	.word	.LC266
	.word	.LC257
	.word	g_event_report
	.word	.LC256
	.word	.LC251
	.word	.LC261
	.word	.LC259
	.word	.LC222
	.word	.LC237
	.word	.LC242
	.word	.LC247
	.word	.LC262
	.word	.LC253
	.word	.LC224
	.word	.LC245
	.word	.LC244
	.word	.LC233
	.word	.LC235
	.word	.LC250
	.word	.LC249
	.word	.LC255
	.word	.LC264
	.word	.LC228
	.word	.LC231
	.word	.LC267
	.word	.LC268
	UNWIND(.fnend)
	.size	HEVC_ProcessSPS, .-HEVC_ProcessSPS
	.align	2
	.global	HEVC_DecSPS
	.type	HEVC_DecSPS, %function
HEVC_DecSPS:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 400
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #404)
	sub	sp, sp, #404
	ldr	r5, .L2299
	mov	r4, r0
	movw	r2, #26164
	mov	r1, #0
	ldr	r0, .L2299+4
	ldr	r3, [r5, #48]
	blx	r3
	mov	r2, #384
	ldr	r3, [r5, #48]
	mov	r1, #0
	sub	r0, fp, #428
	ldr	r8, .L2299+8
	blx	r3
	ldr	r0, [r4, #2308]
	mov	r1, #4
	bl	BsGet
	ldr	r2, [r4, #148]
	cmp	r2, r0
	str	r0, [fp, #-440]
	bls	.L2292
	mov	r1, #3
	ldr	r0, [r4, #2308]
	bl	BsGet
	cmp	r0, #8
	mov	r7, r0
	bhi	.L2293
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r3, r7
	mov	r2, #1
	sub	r1, fp, #428
	mov	r9, r0
	mov	r0, r4
	bl	HEVC_DecPTL
	cmp	r0, #0
	bne	.L2294
	ldr	r1, .L2299+12
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	ldr	r2, [r4, #152]
	cmp	r0, r2
	mov	r10, r0
	bcs	.L2295
	movw	ip, #26164
	ldr	r2, [r4, #2316]
	mul	r6, ip, r0
	uxtb	r0, r9
	str	ip, [fp, #-444]
	ldr	r3, [fp, #-440]
	add	r2, r2, r6
	ldrb	r9, [r2, #1]	@ zero_extendqisi2
	cmp	r9, #0
	bne	.L2296
	str	r3, [r2, #52]
	sub	r1, fp, #428
	ldr	lr, [r4, #2316]
	mov	r2, #384
	add	lr, lr, r6
	str	r7, [lr, #56]
	ldr	lr, [r4, #2316]
	add	lr, lr, r6
	strb	r0, [lr, #2]
	ldr	r0, [r4, #2316]
	ldr	r7, [r5, #52]
	add	r0, r0, r6
	add	r0, r0, #476
	blx	r7
	ldr	r2, [r4, #2316]
	mov	r0, r4
	add	r2, r2, r6
	str	r10, [r2, #60]
	ldr	r1, [r4, #2316]
	add	r1, r1, r6
	bl	HEVC_ProcessSPS
	ldr	ip, [fp, #-444]
	subs	r7, r0, #0
	bne	.L2297
	ldr	r3, [r4, #2316]
	mov	r1, #1
	add	r0, r4, #27392
	mov	r2, ip
	add	r0, r0, #28
	strb	r1, [r3, r6]
	ldr	r3, [r4, #2316]
	add	r3, r3, r6
	strb	r1, [r3, #1]
	ldr	r1, [r4, #2316]
	ldr	r3, [r5, #52]
	add	r1, r1, r6
	blx	r3
.L2283:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2296:
	str	r3, [r8, #-3412]
	sub	r1, fp, #428
	str	r7, [r8, #-3408]
	mov	r2, #384
	strb	r0, [r8, #-3462]
	ldr	r7, [r5, #52]
	ldr	r0, .L2299+16
	blx	r7
	ldr	r1, .L2299+4
	mov	r0, r4
	str	r10, [r8, #-3404]
	bl	HEVC_ProcessSPS
	ldr	ip, [fp, #-444]
	subs	r7, r0, #0
	bne	.L2298
	ldr	r0, [r4, #2316]
	mov	r3, #1
	mov	r2, ip
	strb	r3, [r8, #-3464]
	strb	r3, [r8, #-3463]
	add	r0, r0, r6
	ldr	r9, [r5, #56]
	ldr	r1, .L2299+4
	str	ip, [fp, #-440]
	blx	r9
	ldr	ip, [fp, #-440]
	add	r0, r4, #27392
	ldr	r1, [r4, #2316]
	add	r0, r0, #28
	ldr	r3, [r5, #52]
	mov	r2, ip
	add	r1, r1, r6
	blx	r3
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2295:
	ldr	r1, .L2299+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r4, #152]
	cmp	r10, r2
	bcc	.L2291
	ldr	r1, .L2299+24
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L2291
	str	r2, [fp, #-432]
	mov	r3, #8
	str	r10, [fp, #-436]
	sub	r2, fp, #436
	ldr	r0, [r4, #244]
	mov	r1, #109
	blx	ip
	mvn	r7, #0
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2291:
	mvn	r7, #0
	b	.L2283
.L2292:
	ldr	r1, .L2299+28
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	b	.L2283
.L2293:
	mov	r2, #8
	ldr	r1, .L2299+32
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	b	.L2283
.L2294:
	ldr	r1, .L2299+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L2283
.L2297:
	mov	r2, r10
	ldr	r1, .L2299+40
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #2316]
	mov	r2, #1
	strb	r2, [r3, r6]
	ldr	r3, [r4, #2316]
	add	r6, r3, r6
	strb	r9, [r6, #1]
	b	.L2283
.L2298:
	ldr	r1, [r4, #2316]
	add	r0, r4, #27392
	mov	r2, ip
	ldr	r4, [r5, #52]
	add	r1, r1, r6
	add	r0, r0, #28
	blx	r4
	mov	r2, r10
	ldr	r1, .L2299+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L2283
.L2300:
	.align	2
.L2299:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR5-3464
	.word	.LANCHOR5
	.word	.LC272
	.word	.LANCHOR5-2988
	.word	.LC273
	.word	g_event_report
	.word	.LC269
	.word	.LC270
	.word	.LC271
	.word	.LC275
	.word	.LC274
	UNWIND(.fnend)
	.size	HEVC_DecSPS, .-HEVC_DecSPS
	.align	2
	.global	HEVC_ProcessPPS
	.type	HEVC_ProcessPPS, %function
HEVC_ProcessPPS:
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
	mov	r4, r1
	mov	r7, #1
	mov	r5, r0
	strb	r7, [r4, #16]
	str	r7, [r4, #68]
	str	r7, [r4, #72]
	ldr	r1, .L2429
	ldr	r0, [r0, #2308]
	bl	HEVC_ue_v
	cmp	r0, #0
	str	r0, [r4, #32]
	blt	.L2302
	ldr	r3, [r5, #152]
	cmp	r0, r3
	bge	.L2302
	ldr	r3, [r5, #2316]
	movw	r6, #26164
	mla	r6, r6, r0, r3
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2409
	mov	r1, r7
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #2]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #4]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, r7
	str	r0, [r4, #36]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #3]
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldr	r1, .L2429+4
	strb	r0, [r4, #5]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	add	r0, r0, #1
	str	r0, [r4, #40]
	cmp	r0, #15
	bhi	.L2410
	ldr	r1, .L2429+8
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	add	r0, r0, #1
	str	r0, [r4, #44]
	cmp	r0, #15
	bhi	.L2411
	ldr	r1, .L2429+12
	ldr	r0, [r5, #2308]
	bl	HEVC_se_v
	add	r2, r0, #26
	str	r2, [r4, #48]
	ldr	r3, [r6, #104]
	cmp	r2, #51
	rsb	r0, r3, #0
	movle	r1, #0
	movgt	r1, #1
	cmp	r2, r0
	orrlt	r1, r1, #1
	cmp	r1, #0
	bne	.L2412
	mov	r1, r7
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #6]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #7]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #8]
	cmp	r0, #0
	streq	r0, [r4, #52]
	bne	.L2413
.L2311:
	str	r0, [r4, #56]
	ldr	r1, .L2429+16
	ldr	r0, [r5, #2308]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r4, #60]
	cmp	r3, #24
	bhi	.L2414
	ldr	r1, .L2429+20
	ldr	r0, [r5, #2308]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r4, #64]
	cmp	r3, #24
	bhi	.L2415
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #9]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #10]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #11]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #12]
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #13]
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldrb	r3, [r4, #13]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #14]
	bne	.L2416
	ldr	r3, [r6, #240]
	strh	r3, [r4, #176]	@ movhi
	ldr	r3, [r6, #244]
	strh	r3, [r4, #216]	@ movhi
.L2350:
	ldr	r0, [r4, #68]
	cmp	r0, #0
	ble	.L2355
.L2349:
	ldr	ip, [r6, #160]
	add	r1, r4, #174
	mov	r3, #0
.L2354:
	ldrsh	r2, [r1, #2]!
	add	r3, r3, #1
	mul	r2, ip, r2
	cmp	r2, #64
	bhi	.L2353
	ldr	lr, [r6, #72]
	cmp	lr, #64
	bhi	.L2417
.L2353:
	cmp	r3, r0
	blt	.L2354
.L2355:
	ldr	r1, [r4, #72]
	cmp	r1, #0
	ble	.L2352
	ldr	r0, [r6, #160]
	ldrsh	r2, [r4, #216]
	mul	r2, r0, r2
	cmp	r2, #63
	addhi	r1, r1, #107
	addhi	r3, r4, #216
	addhi	r1, r4, r1, lsl #1
	bhi	.L2358
	b	.L2356
.L2359:
	ldrsh	r2, [r3, #2]!
	mul	r2, r0, r2
	cmp	r2, #63
	bls	.L2356
.L2358:
	cmp	r3, r1
	bne	.L2359
.L2352:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #17]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #18]
	cmp	r0, #0
	bne	.L2418
.L2361:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #21]
	cmp	r0, #0
	beq	.L2364
	add	r1, r4, #260
	mov	r0, r5
	bl	HEVC_DecScalingListData
	cmp	r0, #0
	bne	.L2419
.L2364:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	ldr	r1, .L2429+24
	strb	r0, [r4, #22]
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	add	r2, r0, #2
	str	r2, [r4, #84]
	cmp	r2, #1
	ble	.L2365
	ldr	r3, [r6, #148]
	cmp	r2, r3
	bhi	.L2365
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #23]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #24]
	cmp	r0, #0
	bne	.L2396
	b	.L2369
.L2368:
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r4, #25]
.L2396:
	mov	r0, r5
	bl	HEVC_MoreRbspData
	mov	r1, #1
	cmp	r0, #0
	bne	.L2368
.L2369:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2413:
	ldr	r1, .L2429+28
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #52]
	bls	.L2311
	mov	r0, r7
	ldr	r1, .L2429+32
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2416:
	ldr	r1, .L2429+36
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #19
	add	r2, r0, #1
	str	r2, [r4, #68]
	bhi	.L2420
	ldr	r1, .L2429+40
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #21
	add	r3, r0, #1
	str	r3, [r4, #72]
	bhi	.L2421
	ldr	r2, [r4, #68]
	cmp	r2, #10
	bgt	.L2422
	cmp	r3, #11
	ble	.L2319
	ldr	r1, .L2429+44
	mov	r2, #11
.L2406:
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2429+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2401
.L2403:
	mov	r3, #0
	ldr	r0, [r5, #244]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L2397:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2356:
	ldr	r1, .L2429+52
.L2405:
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2429+48
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L2403
.L2401:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2417:
	ldr	r1, .L2429+56
	b	.L2405
.L2412:
	mov	r0, r7
	ldr	r1, .L2429+60
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2319:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #15]
	cmp	r0, #0
	bne	.L2333
	ldr	r3, [r4, #68]
	cmp	r3, #1
	addgt	r8, r4, #90
	movgt	r7, r0
	bgt	.L2331
	b	.L2423
.L2425:
	cmp	r3, r2
	bhi	.L2329
	ldr	r3, [r4, #68]
	add	r7, r7, #1
	sub	r3, r3, #1
	cmp	r3, r7
	ble	.L2424
.L2331:
	ldr	r1, .L2429+64
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	add	r0, r0, #1
	uxth	r3, r0
	strh	r3, [r8, #2]!	@ movhi
	ldr	r2, [r6, #240]
	sxth	r3, r3
	cmp	r3, #0
	bge	.L2425
.L2329:
	str	r2, [sp]
	mov	r0, #1
	mov	r2, r7
	ldr	r1, .L2429+68
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2422:
	mov	r3, r2
	ldr	r1, .L2429+72
	mov	r2, #10
	b	.L2406
.L2418:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #19]
	ldr	r0, [r5, #2308]
	bl	BsGet
	uxtb	r7, r0
	strb	r7, [r4, #20]
	cmp	r7, #0
	bne	.L2361
	ldr	r1, .L2429+76
	ldr	r0, [r5, #2308]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r4, #76]
	bhi	.L2426
.L2363:
	ldr	r1, .L2429+80
	ldr	r0, [r5, #2308]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r4, #80]
	bls	.L2361
	ldr	r1, .L2429+84
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #80]
	b	.L2361
.L2333:
	ldr	r7, [r4, #68]
	cmp	r7, #0
	addgt	r3, r4, #174
	movgt	r8, #0
	ble	.L2323
.L2341:
	ldr	r10, [r6, #240]
	add	r8, r8, #1
	mov	r1, r7
	str	r3, [fp, #-48]
	mul	r0, r10, r8
	bl	__aeabi_uidiv
	mov	r1, r7
	mov	r9, r0
	sub	r0, r8, #1
	mul	r0, r10, r0
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-48]
	cmp	r8, r7
	rsb	r0, r0, r9
	strh	r0, [r3, #2]!	@ movhi
	bne	.L2341
.L2323:
	ldr	r10, [r4, #72]
	cmp	r10, #0
	addgt	r2, r4, #214
	movgt	r8, #0
	ble	.L2340
.L2342:
	ldr	r3, [r6, #244]
	add	r8, r8, #1
	mov	r1, r10
	str	r2, [fp, #-52]
	mul	r0, r3, r8
	str	r3, [fp, #-48]
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-48]
	mov	r1, r10
	mov	r9, r0
	sub	r0, r8, #1
	mul	r0, r0, r3
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-52]
	cmp	r8, r10
	rsb	r0, r0, r9
	strh	r0, [r2, #2]!	@ movhi
	bne	.L2342
.L2340:
	cmp	r7, #1
	bne	.L2348
	ldr	r0, [r4, #72]
	cmp	r0, #1
	beq	.L2349
.L2348:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	strb	r0, [r4, #16]
	b	.L2350
.L2302:
	mov	r2, r0
	ldr	r1, .L2429+88
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2409:
	mov	r2, r0
	ldr	r1, .L2429+92
	mov	r0, r7
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2410:
	mov	r0, r7
	ldr	r1, .L2429+96
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2411:
	mov	r0, r7
	ldr	r1, .L2429+100
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2414:
	ldr	r1, .L2429+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2415:
	ldr	r1, .L2429+108
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2365:
	ldr	r1, .L2429+112
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2401
.L2424:
	ldr	r3, [r4, #72]
	cmp	r3, #1
	ble	.L2327
.L2326:
	add	r8, r4, #130
	mov	r7, #0
	b	.L2338
.L2427:
	cmp	r3, r2
	bhi	.L2336
	ldr	r3, [r4, #72]
	add	r7, r7, #1
	sub	r3, r3, #1
	cmp	r3, r7
	ble	.L2327
.L2338:
	ldr	r1, .L2429+116
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	add	r0, r0, #1
	uxth	r3, r0
	strh	r3, [r8, #2]!	@ movhi
	ldr	r2, [r6, #244]
	sxth	r3, r3
	cmp	r3, #0
	bge	.L2427
.L2336:
	str	r2, [sp]
	mov	r0, #1
	mov	r2, r7
	ldr	r1, .L2429+120
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2420:
	mov	r3, #20
	ldr	r1, .L2429+124
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2426:
	ldr	r1, .L2429+128
	mov	r0, #1
	bl	dprint_vfmw
	str	r7, [r4, #76]
	b	.L2363
.L2327:
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2333
.L2334:
	ldr	r7, [r4, #68]
	ldrh	r3, [r6, #240]
	sub	r2, r7, #1
	add	ip, r4, r7, lsl #1
	cmp	r2, #0
	strh	r3, [ip, #174]	@ movhi
	ble	.L2343
	add	r8, r7, #44
	add	r1, r4, #90
	add	lr, r4, #174
	add	r8, r4, r8, lsl #1
.L2344:
	ldrh	r0, [r1, #2]!
	cmp	r1, r8
	strh	r0, [lr, #2]!	@ movhi
	ldrh	r3, [ip, #174]
	rsb	r3, r0, r3
	uxth	r3, r3
	strh	r3, [ip, #174]	@ movhi
	bne	.L2344
.L2343:
	sxth	r3, r3
	cmp	r3, #0
	ble	.L2428
	ldr	r1, [r4, #72]
	ldrh	r3, [r6, #244]
	sub	r2, r1, #1
	add	ip, r4, r1, lsl #1
	cmp	r2, #0
	strh	r3, [ip, #214]	@ movhi
	ble	.L2346
	add	r8, r1, #64
	add	lr, r4, #214
	add	r1, r4, #130
	add	r8, r4, r8, lsl #1
.L2347:
	ldrh	r0, [r1, #2]!
	cmp	r1, r8
	strh	r0, [lr, #2]!	@ movhi
	ldrh	r3, [ip, #214]
	rsb	r3, r0, r3
	uxth	r3, r3
	strh	r3, [ip, #214]	@ movhi
	bne	.L2347
.L2346:
	sxth	r3, r3
	cmp	r3, #0
	bgt	.L2340
	ldr	r1, .L2429+132
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2421:
	mov	r2, r3
	ldr	r1, .L2429+136
	mov	r3, #22
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2419:
	ldr	r1, .L2429+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2423:
	ldr	r3, [r4, #72]
	cmp	r3, #1
	bgt	.L2326
	b	.L2334
.L2428:
	ldr	r1, .L2429+144
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2397
.L2430:
	.align	2
.L2429:
	.word	.LC276
	.word	.LC279
	.word	.LC281
	.word	.LC283
	.word	.LC287
	.word	.LC289
	.word	.LC310
	.word	.LC285
	.word	.LC286
	.word	.LC291
	.word	.LC293
	.word	.LC296
	.word	g_event_report
	.word	.LC304
	.word	.LC303
	.word	.LC284
	.word	.LC297
	.word	.LC298
	.word	.LC295
	.word	.LC305
	.word	.LC307
	.word	.LC308
	.word	.LC277
	.word	.LC278
	.word	.LC280
	.word	.LC282
	.word	.LC288
	.word	.LC290
	.word	.LC311
	.word	.LC299
	.word	.LC300
	.word	.LC292
	.word	.LC306
	.word	.LC302
	.word	.LC294
	.word	.LC309
	.word	.LC301
	UNWIND(.fnend)
	.size	HEVC_ProcessPPS, .-HEVC_ProcessPPS
	.align	2
	.global	HEVC_DecPPS
	.type	HEVC_DecPPS, %function
HEVC_DecPPS:
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
	ldr	r1, .L2441
	mov	r4, r0
	ldr	r0, [r0, #2308]
	bl	HEVC_ue_v
	ldr	r3, [r4, #156]
	cmp	r0, r3
	mov	r6, r0
	bcs	.L2437
	movw	r7, #2024
	ldr	r3, [r4, #2320]
	mul	r5, r7, r0
	add	r2, r3, r5
	ldrb	r9, [r3, r5]	@ zero_extendqisi2
	cmp	r9, #0
	bne	.L2438
	str	r0, [r2, #28]
	mov	r0, r4
	ldr	r1, [r4, #2320]
	add	r1, r1, r5
	bl	HEVC_ProcessPPS
	subs	r8, r0, #0
	bne	.L2439
	ldr	r3, [r4, #2320]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #1]
	ldr	r3, [r4, #2320]
	strb	r2, [r3, r5]
.L2433:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2438:
	ldr	r9, .L2441+4
	mov	r0, r4
	ldr	r1, .L2441+8
	str	r6, [r9, #-3436]
	bl	HEVC_ProcessPPS
	subs	r8, r0, #0
	bne	.L2440
	ldr	ip, .L2441+12
	mov	r3, #1
	ldr	r0, [r4, #2320]
	mov	r2, r7
	ldr	r1, .L2441+8
	add	r0, r0, r5
	ldr	ip, [ip, #56]
	strb	r3, [r9, #-3463]
	strb	r3, [r9, #-3464]
	blx	ip
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2440:
	mov	r2, r6
	ldr	r1, .L2441+16
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L2433
.L2437:
	mov	r2, r0
	ldr	r1, .L2441+20
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L2433
.L2439:
	mov	r2, r6
	ldr	r1, .L2441+16
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #2320]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #1]
	ldr	r3, [r4, #2320]
	strb	r9, [r3, r5]
	b	.L2433
.L2442:
	.align	2
.L2441:
	.word	.LC312
	.word	.LANCHOR5
	.word	.LANCHOR5-3464
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC314
	.word	.LC313
	UNWIND(.fnend)
	.size	HEVC_DecPPS, .-HEVC_DecPPS
	.align	2
	.global	HEVC_DecPredWeightTable
	.type	HEVC_DecPredWeightTable, %function
HEVC_DecPredWeightTable:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	add	r9, r0, #53248
	ldr	r2, [r0, #2320]
	movw	ip, #2024
	ldr	r1, [r9, #1092]
	mov	r5, r0
	ldr	r0, [r9, #1076]
	ldr	r4, [r5, #2316]
	mla	r2, ip, r1, r2
	cmp	r0, #1
	ldr	r6, [r2, #32]
	beq	.L2508
	cmp	r0, #0
	bne	.L2447
	ldrb	r2, [r2, #11]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2447
.L2445:
	ldr	r1, .L2519
	ldr	r0, [r5, #2308]
	bl	HEVC_ue_v
	cmp	r0, #7
	str	r0, [r9, #2608]
	bhi	.L2509
	movw	r2, #26164
	mla	r8, r2, r6, r4
	ldr	r2, [r8, #64]
	cmp	r2, #0
	ldreq	r2, [r9, #2612]
	bne	.L2510
.L2451:
	add	r0, r0, r2
	str	r0, [r9, #3640]
	cmp	r0, #7
	bhi	.L2452
	ldr	r2, [r9, #1184]
	cmp	r2, #0
	addne	r4, r5, #55808
	addne	r4, r4, #52
	movne	r6, #0
	movne	r7, r4
	beq	.L2454
.L2455:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldr	r2, [r9, #1184]
	cmp	r2, r6
	bhi	.L2455
	ldr	r1, [r8, #64]
	cmp	r1, #0
	beq	.L2456
	cmp	r2, #0
	addne	r7, r5, #55808
	movne	r6, #0
	addne	r7, r7, #116
	beq	.L2454
.L2460:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldr	r2, [r9, #1184]
	cmp	r2, r6
	bhi	.L2460
.L2456:
	cmp	r2, #0
	beq	.L2454
	add	r6, r5, #56064
	mov	r7, #0
	add	r6, r6, #56
	mov	r3, r9
	str	r5, [fp, #-48]
	mov	r9, r8
	mov	r10, #1
	mov	r8, r7
	mov	r5, r6
	mov	r7, r3
	b	.L2471
.L2461:
	ldr	r1, [r7, #2608]
	str	r0, [r4, #192]
	mov	r1, r10, asl r1
	str	r1, [r4, #1028]
.L2463:
	ldr	r1, [r9, #64]
	cmp	r1, #0
	beq	.L2465
	ldr	r1, [r4, #64]
	cmp	r1, #0
	bne	.L2466
	ldr	r0, [r7, #3640]
	str	r1, [r5, #964]
	mov	r0, r10, asl r0
	str	r1, [r5, #968]
	str	r0, [r5, #836]
	str	r0, [r5, #840]
.L2465:
	ldr	r1, [r7, #1184]
	add	r8, r8, #1
	add	r5, r5, #8
	cmp	r1, r8
	bls	.L2511
.L2471:
	ldr	r0, [r4, #4]!
	cmp	r0, #0
	beq	.L2461
	ldr	r3, [fp, #-48]
	ldr	r1, .L2519+4
	ldr	r0, [r3, #2308]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r4, #128]
	cmp	r1, #255
	bhi	.L2512
	ldr	r3, [fp, #-48]
	ldr	lr, [r7, #2608]
	ldr	r1, .L2519+8
	add	r0, r0, r10, asl lr
	str	r0, [r4, #1028]
	ldr	r0, [r3, #2308]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r4, #192]
	cmp	r1, #255
	bls	.L2463
	mov	r2, r0
	ldr	r1, .L2519+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2508:
	ldrb	r2, [r2, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2445
.L2447:
	mov	r0, #0
.L2446:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2466:
	add	r3, r5, #832
	add	r1, r5, #128
	add	r2, r5, #960
	str	r9, [fp, #-52]
	str	r4, [fp, #-56]
	mov	r9, r5
	str	r8, [fp, #-60]
	mov	r6, #0
	str	r5, [fp, #-64]
	mov	r4, r3
	mov	r8, r1
	mov	r5, r2
.L2469:
	ldr	r3, [fp, #-48]
	add	r6, r6, #1
	ldr	r1, .L2519+16
	ldr	r0, [r3, #2308]
	bl	HEVC_se_v
	ldr	r1, .L2519+20
	add	r3, r0, #128
	str	r0, [r9]
	cmp	r3, #255
	bhi	.L2513
	ldr	r3, [fp, #-48]
	ldr	r0, [r3, #2308]
	bl	HEVC_se_v
	add	r3, r0, #512
	str	r0, [r8], #4
	cmp	r3, #1024
	bcs	.L2514
	ldr	r1, [r7, #3640]
	cmp	r6, #2
	ldr	r2, [r9], #4
	add	r3, r2, r10, asl r1
	str	r3, [r4, #4]!
	mov	r3, r3, asl #7
	mov	r3, r3, asr r1
	rsb	r3, r3, #128
	add	r3, r0, r3
	ssat	r3, #8, r3
	str	r3, [r5, #4]!
	bne	.L2469
	ldr	r9, [fp, #-52]
	ldr	r4, [fp, #-56]
	ldr	r8, [fp, #-60]
	ldr	r5, [fp, #-64]
	b	.L2465
.L2511:
	ldr	r5, [fp, #-48]
	mov	r8, r9
	mov	r9, r7
.L2454:
	ldr	r4, [r9, #1076]
	cmp	r4, #0
	bne	.L2447
	ldr	r2, [r9, #1188]
	cmp	r2, #0
	beq	.L2447
	add	r10, r5, #56320
	add	r10, r10, #52
	mov	r6, r10
.L2472:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	add	r4, r4, #1
	str	r0, [r6, #4]!
	ldr	r2, [r9, #1188]
	cmp	r2, r4
	bhi	.L2472
	ldr	r1, [r8, #64]
	cmp	r1, #0
	bne	.L2515
.L2473:
	cmp	r2, #0
	beq	.L2447
	add	r7, r5, #56576
	mov	r3, #0
	add	r7, r7, #56
	str	r5, [fp, #-48]
	mov	r5, r9
	mov	r9, r8
	mov	r8, r7
	mov	r7, r3
	b	.L2487
.L2477:
	ldr	r1, [r5, #2608]
	mov	r3, #1
	str	r0, [r10, #192]
	mov	r1, r3, asl r1
	str	r1, [r10, #836]
.L2479:
	ldr	r1, [r9, #64]
	cmp	r1, #0
	beq	.L2481
	ldr	r1, [r10, #64]
	cmp	r1, #0
	bne	.L2482
	ldr	r0, [r5, #3640]
	mov	r3, #1
	str	r1, [r8, #772]
	mov	r0, r3, asl r0
	str	r1, [r8, #776]
	str	r0, [r8, #644]
	str	r0, [r8, #648]
.L2481:
	ldr	r1, [r5, #1188]
	add	r7, r7, #1
	add	r8, r8, #8
	cmp	r1, r7
	bls	.L2447
.L2487:
	ldr	r0, [r10, #4]!
	cmp	r0, #0
	beq	.L2477
	ldr	r3, [fp, #-48]
	ldr	r1, .L2519+24
	ldr	r0, [r3, #2308]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r10, #128]
	cmp	r1, #255
	bhi	.L2516
	ldr	lr, [r5, #2608]
	mov	r3, #1
	ldr	r1, .L2519+28
	add	r0, r0, r3, asl lr
	ldr	r3, [fp, #-48]
	str	r0, [r10, #836]
	ldr	r0, [r3, #2308]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r10, #192]
	cmp	r1, #255
	bls	.L2479
	ldr	r1, .L2519+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2482:
	add	r2, r8, #768
	add	r1, r8, #128
	add	r3, r8, #640
	str	r10, [fp, #-64]
	ldr	r10, [fp, #-48]
	mov	r6, r8
	str	r8, [fp, #-52]
	mov	r4, #0
	str	r7, [fp, #-56]
	mov	r8, r3
	str	r9, [fp, #-60]
	mov	r7, r2
	mov	r9, r1
.L2485:
	ldr	r1, .L2519+36
	add	r4, r4, #1
	ldr	r0, [r10, #2308]
	bl	HEVC_se_v
	ldr	r1, .L2519+40
	add	r3, r0, #128
	str	r0, [r6]
	cmp	r3, #255
	bhi	.L2517
	ldr	r0, [r10, #2308]
	bl	HEVC_se_v
	add	r3, r0, #512
	str	r0, [r9], #4
	cmp	r3, #1024
	bcs	.L2518
	ldr	r1, [r5, #3640]
	mov	r3, #1
	ldr	r2, [r6], #4
	cmp	r4, #2
	add	r3, r2, r3, asl r1
	str	r3, [r8, #4]!
	mov	r3, r3, asl #7
	mov	r3, r3, asr r1
	rsb	r3, r3, #128
	add	r3, r0, r3
	ssat	r3, #8, r3
	str	r3, [r7, #4]!
	bne	.L2485
	ldr	r8, [fp, #-52]
	ldr	r7, [fp, #-56]
	ldr	r9, [fp, #-60]
	ldr	r10, [fp, #-64]
	b	.L2481
.L2510:
	ldr	r1, .L2519+44
	ldr	r0, [r5, #2308]
	bl	HEVC_se_v
	add	r2, r0, #7
	str	r0, [r9, #2612]
	cmp	r2, #14
	bhi	.L2450
	mov	r2, r0
	ldr	r0, [r9, #2608]
	b	.L2451
.L2513:
	ldr	r1, .L2519+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2514:
	ldr	r1, .L2519+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2515:
	cmp	r2, #0
	addne	r6, r5, #56320
	movne	r4, #0
	addne	r6, r6, #116
	beq	.L2447
.L2476:
	mov	r1, #1
	ldr	r0, [r5, #2308]
	bl	BsGet
	add	r4, r4, #1
	str	r0, [r6, #4]!
	ldr	r2, [r9, #1188]
	cmp	r2, r4
	bhi	.L2476
	b	.L2473
.L2512:
	mov	r2, r0
	ldr	r1, .L2519+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2517:
	ldr	r1, .L2519+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2518:
	ldr	r1, .L2519+64
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2516:
	ldr	r1, .L2519+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2452:
	ldr	r1, .L2519+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2509:
	ldr	r1, .L2519+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2450:
	ldr	r1, .L2519+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2446
.L2520:
	.align	2
.L2519:
	.word	.LC315
	.word	.LC320
	.word	.LC322
	.word	.LC323
	.word	.LC324
	.word	.LC326
	.word	.LC328
	.word	.LC330
	.word	.LC331
	.word	.LC332
	.word	.LC334
	.word	.LC317
	.word	.LC325
	.word	.LC327
	.word	.LC321
	.word	.LC333
	.word	.LC335
	.word	.LC329
	.word	.LC319
	.word	.LC316
	.word	.LC318
	UNWIND(.fnend)
	.size	HEVC_DecPredWeightTable, .-HEVC_DecPredWeightTable
	.align	2
	.global	HEVC_DecSliceSegmentHeader
	.type	HEVC_DecSliceSegmentHeader, %function
HEVC_DecSliceSegmentHeader:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 72
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
	mov	r1, #1
	mov	r4, r0
	ldr	r0, [r0, #2308]
	bl	BsGet
	add	r10, r4, #53248
	ldr	r7, [r10, #1088]
	sub	r3, r7, #16
	cmp	r3, #5
	movhi	r3, #0
	movls	r3, #1
	cmp	r3, #0
	str	r3, [fp, #-72]
	strb	r0, [r10, #1017]
	bne	.L2786
.L2522:
	ldr	r1, .L2829
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	cmp	r0, #0
	str	r0, [r10, #1092]
	ldr	r3, [r4, #156]
	blt	.L2524
	cmp	r0, r3
	bge	.L2524
	mov	r0, r4
	bl	HEVC_VpsSpsPpsCheck
	subs	r3, r0, #0
	str	r3, [fp, #-88]
	bne	.L2787
	ldr	r1, [r10, #1092]
	movw	r5, #2024
	ldr	r2, [r4, #2320]
	movw	r6, #26164
	ldr	r3, [r4, #2316]
	mla	r5, r5, r1, r2
	ldrb	r1, [r5, #2]	@ zero_extendqisi2
	ldr	r2, [r5, #32]
	cmp	r1, #0
	mla	r6, r6, r2, r3
	bne	.L2788
.L2530:
	mov	r3, #0
	strb	r3, [r10, #1019]
.L2531:
	ldr	r9, [r6, #160]
	ldr	r3, [r6, #164]
	cmp	r9, #0
	beq	.L2533
	cmp	r3, #0
	beq	.L2533
	ldr	r0, [r6, #76]
	mov	r1, r3
	add	r0, r3, r0
	sub	r0, r0, #1
	bl	__aeabi_uidiv
	mov	r1, r9
	mov	r8, r0
	ldr	r0, [r6, #72]
	add	r0, r9, r0
	sub	r0, r0, #1
	bl	__aeabi_uidiv
	ldr	r9, [r6, #216]
	mov	r3, #1
	mov	r1, #0
	mov	r9, r9, asl r3
	mov	r9, r3, asl r9
	mul	r8, r0, r8
	cmp	r8, r3
	ble	.L2535
.L2536:
	add	r1, r1, #1
	cmp	r8, r3, asl r1
	bgt	.L2536
.L2535:
	ldrb	r3, [r10, #1017]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r2, #0
	beq	.L2789
.L2537:
	mul	r9, r9, r8
	ldrb	r8, [r10, #1019]	@ zero_extendqisi2
	str	r2, [r10, #1168]
	cmp	r8, #0
	str	r9, [r10, #1172]
	ldr	r3, [r4, #188]
	str	r3, [r10, #1064]
	beq	.L2790
.L2540:
	ldr	r3, [r5, #12]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #0
	streq	r3, [r10, #1156]
	bne	.L2791
.L2647:
	ldrb	r3, [r5, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2652
.L2658:
	ldr	r3, [r10, #1168]
	cmp	r3, #0
	bne	.L2792
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2660
	ldr	r3, [fp, #-72]
	cmp	r3, #0
	movne	r3, #1
	strneb	r3, [r4, #3]
	bne	.L2662
.L2663:
	mov	r3, #0
	strb	r3, [r4, #2]
.L2664:
	mov	r0, r4
	bl	HEVC_IsNewPic
	str	r0, [r10, #2604]
	mov	r0, r4
	bl	HEVC_ReadByteAlignment
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
.L2766:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2792:
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2663
.L2662:
	ldr	r3, [r10, #1064]
	ldr	r2, [fp, #-72]
	cmp	r3, #0
	moveq	r2, #0
	andne	r2, r2, #1
	cmp	r2, #0
	beq	.L2663
	ldr	r3, [r10, #1088]
	cmp	r3, #21
	moveq	r3, #1
	streqb	r3, [r4, #2]
	bne	.L2663
	b	.L2664
.L2788:
	ldrb	r3, [r10, #1017]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2530
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1019]
	b	.L2531
.L2790:
	ldr	r3, [r5, #36]
	cmp	r3, #0
	beq	.L2545
.L2544:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r1, .L2829+4
	add	r8, r8, #1
	mov	r2, r0
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r3, [r5, #36]
	cmp	r3, r8
	bhi	.L2544
.L2545:
	ldr	r1, .L2829+8
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	cmp	r0, #2
	str	r0, [r10, #1076]
	bhi	.L2793
	ldrb	r3, [r5, #4]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r3, #1
	streqb	r3, [r10, #1016]
	bne	.L2794
.L2547:
	sub	r7, r7, #19
	cmp	r7, #1
	bhi	.L2795
	mov	r3, #0
	str	r3, [r10, #1064]
	str	r3, [r4, #188]
	ldr	r3, [r10, #1084]
	cmp	r3, #0
	streq	r3, [r10, #1068]
	streq	r3, [r4, #184]
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2777
.L2666:
	mov	r3, #0
	strb	r3, [r10, #1023]
	str	r3, [fp, #-92]
.L2668:
	ldr	r3, [r10, #1076]
	cmp	r3, #2
	beq	.L2591
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1024]
	cmp	r0, #0
	bne	.L2796
	ldr	r3, [r10, #1076]
	ldr	r2, [r5, #40]
	cmp	r3, #0
	str	r2, [r10, #1184]
	bne	.L2597
	ldr	r3, [r5, #44]
	str	r3, [r10, #1188]
.L2598:
	ldr	r3, [fp, #-92]
	cmp	r3, #0
	beq	.L2797
	ldr	r3, [fp, #-92]
	ldr	r2, [r3, #24]
	cmp	r2, #16
	bhi	.L2601
	cmp	r2, #0
	moveq	r3, r2
	beq	.L2603
	ldr	r3, [fp, #-92]
	add	r2, r2, #52
	add	r1, r3, #52
	add	r2, r3, r2
	mov	r3, #0
.L2605:
	ldrb	r0, [r1, #1]!	@ zero_extendqisi2
	cmp	r0, #0
	addne	r3, r3, #1
	cmp	r1, r2
	bne	.L2605
	cmp	r3, #1
	movle	r2, #0
	movgt	r2, #1
.L2603:
	ldrb	r1, [r5, #22]	@ zero_extendqisi2
	cmp	r1, #0
	moveq	r2, #0
	andne	r2, r2, #1
	cmp	r2, #0
	beq	.L2798
	add	r2, r4, #57600
	add	r1, r4, #54016
	add	r2, r2, #136
	add	r1, r1, #248
	mov	r0, r4
	bl	HEVC_DecRefPicListsModification
	cmp	r0, #0
	bne	.L2799
.L2599:
	ldr	r3, [r10, #1076]
	cmp	r3, #0
	beq	.L2670
.L2606:
	cmp	r3, #2
	beq	.L2591
.L2671:
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2800
.L2607:
	ldrb	r3, [r10, #1023]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r10, #1076]
	beq	.L2617
	cmp	r3, #0
	beq	.L2801
	cmp	r3, #2
	mov	r3, #1
	strb	r3, [r10, #1027]
	beq	.L2612
.L2615:
	ldr	r3, [r10, #1184]
	cmp	r3, #1
	bls	.L2774
.L2613:
	ldr	r1, .L2829+12
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	ldrb	r2, [r10, #1027]	@ zero_extendqisi2
	cmp	r2, #0
	str	r0, [r10, #1128]
	bne	.L2618
	ldr	r3, [r10, #1124]
	sub	r3, r3, #1
.L2620:
	cmp	r3, r0
	movcs	r1, #0
	movcc	r1, #1
	orrs	r1, r1, r0, lsr #31
	beq	.L2774
	str	r3, [sp]
	mov	r3, r0
	ldr	r1, .L2829+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2786:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1018]
	b	.L2522
.L2789:
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r10, #1096]
	cmp	r0, #0
	ldr	r2, [r6, #244]
	ldr	r3, [r6, #240]
	mul	r3, r3, r2
	blt	.L2539
	cmp	r3, r0
	bls	.L2539
	mul	r2, r9, r0
	b	.L2537
.L2791:
	ldr	r1, .L2829+20
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	str	r0, [r10, #1156]
	ldr	r3, [r5, #12]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	ldreq	r3, [r6, #244]
	subeq	r3, r3, #1
	beq	.L2643
	cmp	r3, #256
	ldreq	r2, [r5, #68]
	ldreq	r3, [r5, #72]
	ldrne	r3, [r6, #244]
	ldrne	r2, [r5, #68]
	mul	r3, r3, r2
	sub	r3, r3, #1
.L2643:
	cmp	r3, #255
	movcs	r3, #255
	cmp	r0, r3
	bhi	.L2802
	cmp	r0, #0
	beq	.L2647
	ldr	r1, .L2829+24
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	cmp	r0, #31
	add	r1, r0, #1
	str	r1, [r10, #1080]
	bhi	.L2803
	ldr	r3, [r10, #1156]
	cmp	r3, #0
	addne	r7, r4, #54272
	movne	r6, #0
	addne	r7, r7, #168
	bne	.L2651
	b	.L2647
.L2804:
	ldr	r1, [r10, #1080]
.L2651:
	ldr	r0, [r4, #2308]
	add	r6, r6, #1
	bl	BsGet
	str	r0, [r7, #4]!
	ldr	r3, [r10, #1156]
	cmp	r3, r6
	bhi	.L2804
	ldrb	r3, [r5, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2658
.L2652:
	ldr	r1, .L2829+28
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	cmp	r0, #256
	str	r0, [r10, #1160]
	bhi	.L2805
	ldr	r0, [r4, #2308]
	bl	BsResidBits
	ldr	r3, [r10, #1160]
	cmp	r0, r3
	bcc	.L2656
	cmp	r3, #0
	movne	r5, #0
	beq	.L2658
.L2659:
	mov	r1, #8
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r3, [r10, #1160]
	add	r5, r5, #1
	cmp	r3, r5
	str	r0, [r10, #1164]
	bhi	.L2659
	b	.L2658
.L2591:
	ldrb	r3, [r10, #1023]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2612
	mov	r3, #2
	mov	r2, #1
	strb	r2, [r10, #1027]
.L2617:
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2622
	cmp	r3, #1
	beq	.L2625
.L2622:
	ldrb	r2, [r5, #11]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2611
	cmp	r3, #0
	bne	.L2611
.L2625:
	mov	r0, r4
	bl	HEVC_DecPredWeightTable
	cmp	r0, #0
	ldreq	r3, [r10, #1076]
	bne	.L2806
.L2611:
	cmp	r3, #2
	beq	.L2626
	ldr	r1, .L2829+32
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	rsb	r3, r0, #4
	rsb	r2, r0, #5
	cmp	r3, #4
	str	r2, [r10, #1132]
	bhi	.L2807
.L2626:
	ldr	r1, .L2829+36
	ldr	r0, [r4, #2308]
	bl	HEVC_se_v
	str	r0, [r10, #1136]
	ldr	r2, [r5, #48]
	add	r2, r0, r2
	str	r2, [r10, #1072]
	ldr	r3, [r6, #104]
	cmp	r2, #51
	rsb	r0, r3, #0
	movle	r1, #0
	movgt	r1, #1
	cmp	r2, r0
	orrlt	r1, r1, #1
	cmp	r1, #0
	bne	.L2808
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2809
.L2628:
	ldrb	r3, [r5, #18]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2775
	ldrb	r3, [r5, #19]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r3, [r10, #1028]
	bne	.L2810
.L2636:
	ldrb	r0, [r5, #20]	@ zero_extendqisi2
	strb	r0, [r10, #1029]
	ldr	r3, [r5, #76]
	str	r3, [r10, #1148]
	ldr	r3, [r5, #80]
	str	r3, [r10, #1152]
.L2633:
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2639
	ldr	r2, [r10, #1020]
	bic	r3, r2, #-16777216
	bic	r3, r3, #255
	adds	r3, r3, #0
	movne	r3, #1
.L2639:
	ldrb	r2, [r5, #17]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2640
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L2811
.L2640:
	strb	r2, [r10, #1030]
	b	.L2540
.L2660:
	ldr	r2, [fp, #-72]
	cmp	r2, #0
	strneb	r3, [r4, #3]
	b	.L2663
.L2798:
	ldr	r3, [r10, #1076]
	cmp	r3, #0
	bne	.L2671
.L2670:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r3, [r10, #1076]
	strb	r0, [r10, #1025]
	b	.L2606
.L2775:
	ldrb	r0, [r10, #1029]	@ zero_extendqisi2
	b	.L2633
.L2795:
	ldr	r1, [r6, #132]
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r2, #1
	str	r0, [r10, #1104]
	mov	r8, r0
	ldr	r3, [r6, #128]
	ldr	r7, [r4, #184]
	mov	r3, r2, asl r3
	str	r0, [fp, #-100]
	str	r3, [fp, #-104]
	mov	r1, r3
	mov	r0, r7
	bl	__aeabi_uidivmod
	cmp	r1, r8
	rsb	r3, r1, r7
	bls	.L2549
	ldr	r0, [fp, #-104]
	ldr	r2, [fp, #-100]
	rsb	r2, r2, r1
	cmp	r2, r0, lsr #1
	movcs	r2, r0
	addcs	r3, r3, r2
	bcc	.L2549
.L2550:
	ldr	r2, [r10, #1088]
	sub	r2, r2, #16
	cmp	r2, #2
	ldr	r2, [fp, #-100]
	movls	r3, #0
	add	r3, r3, r2
	str	r3, [r10, #1064]
	ldr	r2, [r4, #212]
	cmp	r3, r2
	bge	.L2552
	ldrb	r0, [r10, #1017]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L2812
.L2552:
	str	r3, [r4, #188]
	ldr	r2, [r10, #1084]
	cmp	r2, #0
	bne	.L2553
	ldr	r2, [r10, #1088]
	cmp	r2, #15
	bls	.L2813
.L2554:
	sub	r1, r2, #16
	cmp	r1, #7
	movhi	r1, #0
	movls	r1, #1
.L2555:
	sub	r2, r2, #7
	bics	r2, r2, #2
	andne	r2, r1, #1
	moveq	r2, #0
	cmp	r2, #0
	strne	r3, [r10, #1068]
	strne	r3, [r4, #184]
.L2553:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1020]
	cmp	r0, #0
	beq	.L2556
	ldr	r3, [r6, #228]
	cmp	r3, #1
	movhi	r1, #0
	movhi	r2, #1
	bls	.L2558
.L2560:
	add	r1, r1, #1
	cmp	r3, r2, asl r1
	bhi	.L2560
	cmp	r1, #0
	bne	.L2814
.L2558:
	mov	r1, #0
	str	r1, [r10, #1108]
.L2563:
	mov	r2, #204
	add	r0, r4, #57344
	mla	r1, r2, r1, r6
	add	r0, r0, #188
	str	r0, [fp, #-92]
	add	r1, r1, #13056
	add	r1, r1, #52
	bl	memcpy
.L2559:
	ldrb	r3, [r6, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2564
	add	r3, r4, #57344
	str	r3, [fp, #-84]
	mov	r1, r3
	ldr	r3, [r6, #232]
	ldr	r2, [r1, #208]
	cmp	r3, #0
	ldr	r3, [r1, #204]
	add	r3, r2, r3
	str	r3, [fp, #-108]
	bne	.L2565
.L2568:
	mov	r9, #0
.L2566:
	ldr	r1, .L2829+40
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	ldr	ip, [fp, #-84]
	ldr	r1, [r10, #1112]
	str	r0, [r10, #1116]
	add	r0, r0, r1
	ldr	r2, [ip, #204]
	ldr	r3, [ip, #208]
	str	r0, [ip, #220]
	add	lr, r3, r2
	add	lr, lr, r0
	cmp	lr, #16
	bhi	.L2815
	ldr	r3, [fp, #-108]
	cmp	r0, #0
	sub	r7, r3, #1
	add	r7, r0, r7
	beq	.L2584
	add	r3, r7, #13888
	add	r7, r7, #57344
	add	r3, r3, #43
	add	r7, r7, #241
	add	r7, r4, r7
	str	r7, [fp, #-64]
	mov	r3, r3, asl #2
	add	r0, r4, #55552
	add	ip, r3, #64
	str	ip, [fp, #-56]
	ldr	r7, [fp, #-56]
	add	ip, r4, #55296
	str	ip, [fp, #-48]
	add	lr, r4, #54272
	add	r8, r4, r7
	add	r7, r4, r3
	ldr	r3, [fp, #-48]
	add	r2, r4, #54272
	str	r0, [fp, #-52]
	add	r0, r0, #104
	str	r0, [fp, #-60]
	add	r0, ip, #236
	add	ip, lr, #7
	add	lr, r3, #172
	ldr	r3, [fp, #-52]
	add	r2, r2, #22
	str	r7, [fp, #-96]
	add	r3, r3, #44
	str	lr, [fp, #-48]
	str	r5, [fp, #-112]
	mov	lr, r3
	mov	r3, #0
	str	r6, [fp, #-76]
	mov	r7, lr
	str	r9, [fp, #-80]
	mov	r9, r0
	ldr	r5, [fp, #-64]
	str	r4, [fp, #-52]
	mov	r4, r3
	str	r10, [fp, #-64]
	mov	r10, ip
	str	r2, [fp, #-116]
	str	r3, [fp, #-68]
	str	r2, [fp, #-56]
	ldr	r6, [fp, #-96]
	b	.L2583
.L2818:
	ldr	r3, [fp, #-80]
	cmp	r3, #0
	ldreq	r0, [fp, #-80]
	bne	.L2816
.L2574:
	ldr	r2, [fp, #-76]
	add	r3, r2, r0
	add	r0, r0, #62
	ldrb	r1, [r3, #16]	@ zero_extendqisi2
	ldr	r3, [r2, r0, asl #2]
	strb	r1, [r5]
.L2577:
	ldr	r2, [fp, #-60]
	mov	r1, #1
	str	r3, [r2, #4]!
	ldr	r3, [fp, #-52]
	str	r2, [fp, #-60]
	ldr	r0, [r3, #2308]
	bl	BsGet
	ldr	r3, [fp, #-56]
	uxtb	r0, r0
	strb	r0, [r3, #1]!
	cmp	r0, #0
	str	r3, [fp, #-56]
	bne	.L2817
	cmp	r4, #0
	streq	r4, [fp, #-68]
	beq	.L2581
	ldr	r3, [fp, #-64]
	ldr	r2, [fp, #-68]
	ldr	r3, [r3, #1112]
	cmp	r3, r4
	moveq	r2, #0
	str	r2, [fp, #-68]
.L2581:
	mov	r3, #0
	str	r3, [r8]
.L2580:
	ldr	r3, [fp, #-84]
	add	r4, r4, #1
	ldr	r2, [fp, #-48]
	sub	r6, r6, #4
	sub	r8, r8, #4
	sub	r5, r5, #1
	ldr	r3, [r3, #220]
	add	r2, r2, #4
	add	r9, r9, #4
	add	r10, r10, #1
	cmp	r3, r4
	str	r2, [fp, #-48]
	add	r7, r7, #4
	bls	.L2582
	ldr	r3, [fp, #-64]
	ldr	r1, [r3, #1112]
.L2583:
	cmp	r1, r4
	bhi	.L2818
	ldr	r3, [fp, #-76]
	ldr	r1, [r3, #132]
	ldr	r3, [fp, #-52]
	ldr	r0, [r3, #2308]
	bl	BsGet
	ldr	r3, [fp, #-52]
	mov	r1, #1
	str	r0, [r9]
	str	r0, [fp, #-96]
	ldr	r0, [r3, #2308]
	bl	BsGet
	ldr	r3, [fp, #-96]
	uxtb	r0, r0
	strb	r0, [r10]
	strb	r0, [r5]
	b	.L2577
.L2549:
	ldr	r2, [fp, #-100]
	cmp	r1, r2
	bcs	.L2550
	rsb	r1, r1, r2
	ldr	r2, [fp, #-104]
	cmp	r1, r2, lsr #1
	rsbhi	r3, r2, r3
	b	.L2550
.L2794:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1016]
	b	.L2547
.L2809:
	ldr	r1, .L2829+44
	ldr	r0, [r4, #2308]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r10, #1140]
	cmp	r3, #24
	bhi	.L2819
	ldr	r3, [r5, #60]
	add	r2, r0, r3
	add	r2, r2, #12
	cmp	r2, #24
	bhi	.L2820
	ldr	r1, .L2829+48
	ldr	r0, [r4, #2308]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r10, #1144]
	cmp	r3, #24
	bhi	.L2821
	ldr	r2, [r5, #64]
	add	r2, r0, r2
	add	r3, r2, #12
	cmp	r3, #24
	bls	.L2628
	ldr	r1, .L2829+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2524:
	str	r3, [sp]
	mov	r3, r0
	ldr	r2, .L2829+56
	mov	r0, #1
	ldr	r1, .L2829+60
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2774:
	ldr	r3, [r10, #1076]
	b	.L2617
.L2564:
	ldrb	r3, [r6, #11]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r3, [r10, #1023]
	bne	.L2822
.L2589:
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2668
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r10, #1021]
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1022]
	b	.L2668
.L2777:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r10, #1021]
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1022]
	b	.L2666
.L2800:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1026]
	b	.L2607
.L2597:
	str	r0, [r10, #1188]
.L2596:
	cmp	r3, #2
	bne	.L2598
	b	.L2599
.L2796:
	ldr	r1, .L2829+64
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	add	r2, r0, #1
	str	r2, [r10, #1120]
	cmp	r2, #16
	bhi	.L2823
	ldr	r3, [r10, #1076]
	str	r2, [r10, #1184]
	cmp	r3, #0
	movne	r2, #0
	strne	r2, [r10, #1124]
	strne	r2, [r10, #1188]
	bne	.L2596
	ldr	r1, .L2829+68
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	add	r2, r0, #1
	str	r2, [r10, #1124]
	cmp	r2, #16
	strls	r2, [r10, #1188]
	ldrls	r3, [r10, #1076]
	bls	.L2596
	mov	r3, #16
	ldr	r1, .L2829+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2801:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	ldr	r3, [r10, #1076]
	cmp	r3, #2
	uxtb	r0, r0
	strb	r0, [r10, #1027]
	beq	.L2626
	cmp	r0, #0
	bne	.L2615
	ldr	r2, [r10, #1188]
	cmp	r2, #1
	bhi	.L2613
	b	.L2617
.L2612:
	mov	r3, #2
	b	.L2622
.L2618:
	ldr	r3, [r10, #1120]
	sub	r3, r3, #1
	b	.L2620
.L2813:
	tst	r2, #1
	movne	r1, #1
	bne	.L2555
	b	.L2554
.L2565:
	ldr	r1, .L2829+76
	ldr	r0, [r4, #2308]
	bl	HEVC_ue_v
	str	r0, [r10, #1112]
	mov	r2, r0
	ldr	r3, [r6, #232]
	cmp	r0, r3
	bhi	.L2824
	cmp	r3, #1
	bls	.L2568
	mov	r9, #0
	mov	r2, #1
.L2569:
	add	r9, r9, #1
	cmp	r3, r2, asl r9
	bhi	.L2569
	str	r9, [fp, #-88]
	b	.L2566
.L2812:
	ldr	r1, .L2829+80
	bl	dprint_vfmw
	ldr	r3, [r10, #1064]
	b	.L2552
.L2656:
	mov	r2, r0
	ldr	r1, .L2829+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2817:
	ldr	r3, [fp, #-52]
	ldr	r1, .L2829+88
	ldr	r0, [r3, #2308]
	bl	HEVC_ue_v
	cmp	r4, #0
	str	r0, [r7]
	beq	.L2579
	ldr	r3, [fp, #-64]
	ldr	r3, [r3, #1112]
	cmp	r3, r4
	ldrne	r3, [fp, #-68]
	addne	r0, r0, r3
.L2579:
	str	r0, [r6]
	mov	r3, #1
	str	r0, [fp, #-68]
	str	r3, [r8]
	b	.L2580
.L2816:
	ldr	r3, [fp, #-52]
	ldr	r1, [fp, #-88]
	ldr	r0, [r3, #2308]
	bl	BsGet
	ldr	r3, [fp, #-48]
	cmp	r0, #0
	str	r0, [r3]
	blt	.L2825
	ldr	r3, [fp, #-76]
	ldr	r3, [r3, #232]
	sub	r3, r3, #1
	cmp	r3, r0
	bcs	.L2574
	mov	r7, r4
.L2576:
	str	r3, [sp]
	mov	r2, r7
	mov	r3, r0
	ldr	r1, .L2829+92
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2533:
	mov	r2, r9
	ldr	r1, .L2829+96
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2811:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1030]
	b	.L2540
.L2810:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1028]
	cmp	r0, #0
	beq	.L2636
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1029]
	cmp	r0, #0
	bne	.L2633
	ldr	r1, .L2829+100
	ldr	r0, [r4, #2308]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r10, #1148]
	bhi	.L2826
.L2637:
	ldr	r1, .L2829+104
	ldr	r0, [r4, #2308]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r10, #1152]
	bls	.L2775
	ldr	r1, .L2829+108
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #80]
	ldrb	r0, [r10, #1029]	@ zero_extendqisi2
	str	r3, [r10, #1152]
	b	.L2633
.L2582:
	ldr	r2, [fp, #-108]
	cmp	r3, #0
	ldr	r5, [fp, #-112]
	add	r2, r2, #14400
	ldr	r6, [fp, #-76]
	ldr	r4, [fp, #-52]
	add	r2, r2, #17
	ldr	r10, [fp, #-64]
	beq	.L2584
	add	r2, r2, r3
	add	lr, r4, #55552
	add	lr, lr, #108
	mov	r7, #0
	add	r2, r4, r2, lsl #2
	ldr	r8, [fp, #-104]
	ldr	ip, [fp, #-116]
	ldr	r9, [fp, #-100]
	str	r4, [fp, #-48]
	b	.L2587
.L2828:
	ldr	r0, [r10, #1064]
	ldr	r4, [lr]
	rsb	r1, r9, r0
	add	r1, r1, r4
	ldr	r4, [r2, #-1948]
	mls	r1, r4, r8, r1
	rsb	r0, r0, r1
	str	r0, [r2, #-68]
	str	r1, [r2]
.L2586:
	add	r7, r7, #1
	add	lr, lr, #4
	cmp	r3, r7
	sub	r2, r2, #4
	bls	.L2827
.L2587:
	ldrb	r1, [ip, #1]!	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2828
	ldr	r1, [lr]
	str	r1, [r2]
	ldr	r0, [r10, #1064]
	rsb	r1, r0, r1
	str	r1, [r2, #-68]
	b	.L2586
.L2787:
	ldr	r3, [r4, #216]
	mov	r0, r4
	cmp	r3, #0
	ldrne	r3, [r4, #188]
	str	r3, [r10, #1064]
	bl	HEVC_IsNewPic
	ldr	r1, .L2829+112
	str	r0, [r10, #2604]
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2827:
	ldr	r4, [fp, #-48]
.L2572:
	ldr	r2, [fp, #-108]
	add	r2, r3, r2
	ldr	r3, [fp, #-84]
	cmp	r2, #16
	str	r2, [r3, #212]
	bls	.L2564
	mov	r3, #16
	ldr	r1, .L2829+116
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2556:
	add	r7, r4, #57344
	ldr	r3, [r6, #228]
	add	r7, r7, #188
	mov	r1, r6
	mov	r0, r4
	mov	r2, r7
	bl	HEVC_DecShortTermRefPicSet
	cmp	r0, #0
	streq	r7, [fp, #-92]
	beq	.L2559
	ldr	r1, .L2829+120
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2822:
	mov	r1, #1
	ldr	r0, [r4, #2308]
	bl	BsGet
	strb	r0, [r10, #1023]
	b	.L2589
.L2830:
	.align	2
.L2829:
	.word	.LC336
	.word	.LC342
	.word	.LC341
	.word	.LC361
	.word	.LC362
	.word	.LC378
	.word	.LC380
	.word	.LC382
	.word	.LC364
	.word	.LC366
	.word	.LC349
	.word	.LC368
	.word	.LC371
	.word	.LC373
	.word	.LANCHOR1+576
	.word	.LC337
	.word	.LC354
	.word	.LC356
	.word	.LC357
	.word	.LC347
	.word	.LC344
	.word	.LC384
	.word	.LC352
	.word	.LC351
	.word	.LC339
	.word	.LC374
	.word	.LC376
	.word	.LC377
	.word	.LC338
	.word	.LC353
	.word	.LC345
	.word	.LC346
	.word	.LC367
	.word	.LC340
	.word	.LC383
	.word	.LC343
	.word	.LC359
	.word	.LC365
	.word	.LC379
	.word	.LC375
	.word	.LC370
	.word	.LC363
	.word	.LC372
	.word	.LC369
	.word	.LC350
	.word	.LC360
	.word	.LC355
	.word	.LC358
	.word	.LC381
	.word	.LC348
.L2814:
	ldr	r0, [r4, #2308]
	bl	BsGet
	str	r0, [r10, #1108]
	cmp	r0, #0
	ldr	r3, [r6, #228]
	mov	r1, r0
	sub	r3, r3, #1
	blt	.L2562
	cmp	r3, r0
	bcs	.L2563
.L2562:
	mov	r2, r1
	mov	r0, #1
	ldr	r1, .L2829+124
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2808:
	ldr	r1, .L2829+128
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2539:
	mov	r2, r0
	ldr	r1, .L2829+132
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2805:
	mov	r2, r0
	ldr	r1, .L2829+136
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2825:
	ldr	r6, [fp, #-76]
	mov	r7, r4
	ldr	r3, [r6, #232]
	sub	r3, r3, #1
	b	.L2576
.L2793:
	mov	r2, r0
	ldr	r1, .L2829+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2601:
	mov	r3, #16
	ldr	r1, .L2829+144
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2807:
	ldr	r1, .L2829+148
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2802:
	mov	r2, r0
	ldr	r1, .L2829+152
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2826:
	ldr	r1, .L2829+156
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #76]
	str	r3, [r10, #1148]
	b	.L2637
.L2820:
	mov	r2, r0
	ldr	r1, .L2829+160
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2584:
	mov	r3, #0
	b	.L2572
.L2806:
	ldr	r1, .L2829+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2821:
	mov	r2, r0
	ldr	r1, .L2829+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2819:
	mov	r2, r0
	ldr	r1, .L2829+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2815:
	str	r0, [sp]
	mov	ip, #16
	ldr	r1, .L2829+176
	mov	r0, #1
	str	ip, [sp, #4]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2799:
	ldr	r1, .L2829+180
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2823:
	mov	r3, #16
	ldr	r1, .L2829+184
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2797:
	ldr	r1, .L2829+188
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2803:
	mov	r2, r1
	mov	r0, #1
	ldr	r1, .L2829+192
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
.L2824:
	ldr	r1, .L2829+196
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2766
	UNWIND(.fnend)
	.size	HEVC_DecSliceSegmentHeader, .-HEVC_DecSliceSegmentHeader
	.align	2
	.global	HEVC_ClearCurrNal
	.type	HEVC_ClearCurrNal, %function
HEVC_ClearCurrNal:
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
	ldr	r1, [r0, #2332]
	mov	r4, r0
	cmp	r1, #0
	beq	.L2832
	ldr	r0, [r0, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L2832:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_ClearCurrNal, .-HEVC_ClearCurrNal
	.align	2
	.global	HEVC_FrameStoreRefFlg
	.type	HEVC_FrameStoreRefFlg, %function
HEVC_FrameStoreRefFlg:
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
	ldrb	r0, [r0, #1]	@ zero_extendqisi2
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_FrameStoreRefFlg, .-HEVC_FrameStoreRefFlg
	.align	2
	.global	HEVC_ClearCurrSlice
	.type	HEVC_ClearCurrSlice, %function
HEVC_ClearCurrSlice:
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
	ldr	r1, [r0, #2332]
	mov	r4, r0
	cmp	r1, #0
	beq	.L2838
	ldr	r0, [r0, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L2838:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_ClearCurrSlice, .-HEVC_ClearCurrSlice
	.align	2
	.global	HEVC_ClearAllSlice
	.type	HEVC_ClearAllSlice, %function
HEVC_ClearAllSlice:
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
	ldr	r3, [r0, #160]
	mov	r7, r0
	cmp	r3, #0
	beq	.L2846
	mov	r4, #0
	add	r8, r0, #65536
	mov	r5, r4
	mov	r6, r4
.L2845:
	ldr	r3, [r8, #2644]
	add	r5, r5, #1
	add	r3, r3, r4
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq	.L2844
	ldr	r0, [r7, #244]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r8, #2644]
	add	r3, r3, r4
	str	r6, [r3, #24]
.L2844:
	ldr	r3, [r7, #160]
	add	r4, r4, #28
	cmp	r3, r5
	bhi	.L2845
.L2846:
	mov	r0, #0
	mov	r3, #2
	str	r0, [r7, #208]
	str	r0, [r7, #224]
	str	r0, [r7, #216]
	strb	r0, [r7, #9]
	str	r3, [r7, #172]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_ClearAllSlice, .-HEVC_ClearAllSlice
	.align	2
	.global	HEVC_InitDPB
	.type	HEVC_InitDPB, %function
HEVC_InitDPB:
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
	UNWIND(.pad #16)
	sub	sp, sp, #16
	mov	r5, r0
	mov	r8, r1
	bl	HEVC_ClearAllSlice
	add	r9, r5, #2512
	mov	r4, r5
	add	r9, r9, #12
	mov	r6, #17
	mov	r7, #0
.L2857:
	ldr	r3, [r9, #4]!
	mov	r2, #1
	cmp	r3, #0
	beq	.L2853
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #244]
	bl	FSP_ClearLogicFs
.L2853:
	ldr	r3, [r4, #2596]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2854
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #244]
	bl	FSP_ClearLogicFs
.L2854:
	ldr	r3, [r4, #2664]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2855
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #244]
	bl	FSP_ClearLogicFs
.L2855:
	ldr	r3, [r4, #2732]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2856
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #244]
	bl	FSP_ClearLogicFs
.L2856:
	subs	r6, r6, #1
	str	r7, [r4, #2732]
	str	r7, [r4, #2664]
	add	r4, r4, #4
	str	r7, [r4, #2592]
	str	r7, [r4, #2524]
	bne	.L2857
	add	r4, r5, #24576
	mov	r7, #0
	mov	r1, r7
	mov	r2, #200
	ldr	r3, [r4, #2836]
	add	r0, r5, #2800
	ldr	r3, [r3, #236]
	str	r7, [r5, #2508]
	add	r3, r3, #1
	str	r7, [r5, #2520]
	cmp	r3, #17
	str	r7, [r5, #2524]
	str	r7, [r5, #2516]
	movhi	r3, #17
	str	r3, [r5, #2504]
	ldr	r3, .L2899
	str	r7, [r5, #2512]
	ldr	r3, [r3, #48]
	blx	r3
	add	r2, r5, #2320
	add	r2, r2, #12
	add	r3, r5, #2400
	mov	r0, r7
	mov	r1, #16
	str	r7, [r5, #2804]
	str	r1, [r5, #2800]
.L2860:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L2860
	add	r2, r5, #2464
	add	ip, r5, #53248
	add	r2, r2, #4
	mov	r0, r1
	str	r1, [ip, #1176]
.L2861:
	str	r0, [r3, #4]!
	cmp	r3, r2
	mov	r1, #0
	bne	.L2861
	cmp	r8, r1
	str	r1, [ip, #1180]
	bne	.L2862
	ldr	r3, [r5, #2324]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L2862
.L2863:
	ldr	r1, [r5, #180]
	cmp	r1, #0
	addne	r2, r5, #2112
	movne	r3, #0
	movne	r0, r3
	beq	.L2871
.L2870:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L2870
.L2871:
	ldr	r3, [r5, #160]
	cmp	r3, #0
	addne	r8, r5, #65536
	movne	r4, #0
	movne	r7, r4
	beq	.L2869
.L2873:
	ldr	r3, [r8, #2644]
	add	r4, r4, #1
	add	r3, r3, r6
	ldr	r3, [r3, #24]
	cmp	r3, #0
	mov	r1, r3
	beq	.L2872
	ldr	r2, [r5, #2332]
	cmp	r3, r2
	beq	.L2872
	ldr	r0, [r5, #244]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r8, #2644]
	add	r3, r3, r6
	str	r7, [r3, #24]
.L2872:
	ldr	r3, [r5, #160]
	add	r6, r6, #28
	cmp	r3, r4
	bhi	.L2873
.L2869:
	mov	r0, #0
.L2864:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2862:
	mov	r7, #20
	str	r7, [r5, #176]
	str	r7, [r5, #180]
	mov	r0, r5
	ldr	r1, [r4, #2836]
	ldr	r3, [r1, #240]
	ldr	ip, [r1, #244]
	ldr	r2, [r1, #148]
	str	r7, [sp, #8]
	mov	r1, r3, asl r2
	str	ip, [sp]
	mov	r2, ip, asl r2
	str	r7, [sp, #4]
	bl	HEVC_ArrangeVahbMem
	ldr	r3, [r5, #2324]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L2874
	cmp	r0, #1
	bne	.L2897
	ldr	r0, [r5, #244]
	bl	FSP_GetTotalValidFsNum
	cmp	r0, #20
	movgt	r0, r7
	ble	.L2898
.L2866:
	str	r0, [r5, #176]
	b	.L2863
.L2898:
	ldr	r0, [r5, #244]
	bl	FSP_GetTotalValidFsNum
	b	.L2866
.L2897:
	ldr	r2, .L2899+4
	mov	r0, #0
	ldr	r1, .L2899+8
	bl	dprint_vfmw
	mvn	r0, #4
	b	.L2864
.L2874:
	mvn	r0, #1
	b	.L2864
.L2900:
	.align	2
.L2899:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+604
	.word	.LC385
	UNWIND(.fnend)
	.size	HEVC_InitDPB, .-HEVC_InitDPB
	.align	2
	.global	HEVC_ClearAllNal
	.type	HEVC_ClearAllNal, %function
HEVC_ClearAllNal:
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
	ldr	r3, [r0, #160]
	mov	r6, r0
	cmn	r3, #1
	beq	.L2905
	mov	r4, #0
	add	r7, r0, #65536
	mov	r5, r4
	b	.L2904
.L2903:
	ldr	r3, [r6, #160]
	add	r3, r3, #1
	cmp	r3, r5
	bls	.L2905
.L2904:
	ldr	r1, [r7, #1840]
	add	r5, r5, #1
	add	r1, r1, r4
	add	r4, r4, #100
	ldr	r3, [r1, #36]
	cmp	r3, #1
	bne	.L2903
	ldr	r0, [r6, #244]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r6, #160]
	add	r3, r3, #1
	cmp	r3, r5
	bhi	.L2904
.L2905:
	mov	r0, #0
	strb	r0, [r6, #5]
	str	r0, [r6, #140]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_ClearAllNal, .-HEVC_ClearAllNal
	.align	2
	.global	HEVC_InitDecPara
	.type	HEVC_InitDecPara, %function
HEVC_InitDecPara:
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
	ldr	r1, .L2921
	mov	r4, r0
	mov	r0, #22
	add	r5, r4, #67584
	bl	dprint_vfmw
	mov	r3, #-2147483647
	mov	r0, r4
	str	r3, [r4, #212]
	bl	HEVC_ClearAllNal
	add	r0, r4, #61696
	add	r0, r0, #32
	ldr	r6, .L2921+4
	bl	ResetVoQueue
	mov	r8, #0
	add	r5, r5, #672
	mov	r9, #20
	mov	r7, r8
.L2909:
	strb	r7, [r5, #-72]
	mov	r3, #16
	str	r7, [r5, #-68]
	mov	r0, r5
	strb	r7, [r5, #-71]
	mov	r2, #624
	str	r7, [r5, #-60]
	mov	r1, #0
	strb	r7, [r5, #-70]
	str	r9, [r5, #-48]
	str	r3, [r5, #-44]
	ldr	r3, [r6, #48]
	blx	r3
	str	r8, [r5, #156]
	add	r8, r8, #1
	sub	r3, r5, #72
	cmp	r8, #20
	str	r7, [r5, #-24]
	str	r3, [r5, #-4]
	add	r5, r5, #704
	bne	.L2909
	add	r3, r4, #2112
	add	r1, r4, #2192
	mov	r2, #0
.L2910:
	str	r2, [r3, #4]!
	cmp	r3, r1
	bne	.L2910
	ldr	r3, [r4, #160]
	cmp	r3, #0
	addne	ip, r4, #65536
	movne	r3, #0
	movne	r0, r3
	movne	r1, r3
	beq	.L2914
.L2913:
	ldr	r2, [ip, #2644]
	add	r0, r0, #1
	str	r1, [r2, r3]
	ldr	r2, [ip, #2644]
	add	r2, r2, r3
	str	r1, [r2, #20]
	ldr	r2, [ip, #2644]
	add	r2, r2, r3
	add	r3, r3, #28
	str	r1, [r2, #24]
	ldr	r2, [r4, #160]
	cmp	r2, r0
	bhi	.L2913
.L2914:
	ldr	r3, [r6, #48]
	mov	r2, #200
	mov	r1, #0
	add	r0, r4, #2800
	blx	r3
	add	ip, r4, #2720
	add	r0, r4, #2656
	add	r1, r4, #2512
	add	ip, ip, #8
	add	r0, r0, #4
	add	r1, r1, #12
	add	lr, r4, #2592
	mov	r3, #16
	mov	r2, #17
	str	r3, [r4, #2800]
	mov	r3, #0
.L2912:
	subs	r2, r2, #1
	str	r3, [ip, #4]!
	str	r3, [r0, #4]!
	str	r3, [lr, #4]!
	str	r3, [r1, #4]!
	bne	.L2912
	add	r1, r4, #2320
	add	ip, r4, #2464
	add	r1, r1, #12
	add	ip, ip, #4
	add	r3, r4, #2400
	mov	r0, r2
	mov	lr, #17
	str	r2, [r4, #2508]
	str	r2, [r4, #2512]
	str	r2, [r4, #2516]
	str	r2, [r4, #2520]
	str	r2, [r4, #2524]
	str	lr, [r4, #2504]
.L2915:
	str	r0, [r3, #4]!
	cmp	r3, ip
	str	r0, [r1, #4]!
	mov	r5, #0
	bne	.L2915
	add	r0, r4, #54016
	mov	r9, #2
	mov	r1, r5
	ldr	r3, [r6, #48]
	str	r5, [r4, #2332]
	movw	r2, #3732
	str	r5, [r4, #208]
	add	r0, r0, #248
	str	r5, [r4, #224]
	add	r7, r4, #2464
	str	r5, [r4, #216]
	add	r7, r7, #8
	str	r9, [r4, #172]
	mov	r8, #32
	blx	r3
	add	r2, r4, #53248
	mov	r3, #1
	mov	lr, #20
	str	r9, [r2, #1076]
	mov	ip, #16
	str	r3, [r2, #2604]
	mov	r1, r5
	strb	r3, [r4, #1]
	mov	r2, r8
	strb	r3, [r4, #3]
	mov	r0, r7
	mov	r3, #8
	strb	r5, [r4, #4]
	strb	r5, [r4, #2]
	mov	r10, #64
	strb	r5, [r4, #8]
	str	r9, [r4, #2208]
	mvn	r9, #-2147483648
	str	r5, [r4, #168]
	movw	r5, #1025
	str	r8, [r4, #320]
	mov	r8, #4
	str	lr, [r4, #176]
	str	lr, [r4, #180]
	mov	lr, #6
	str	r3, [r4, #192]
	str	r3, [r4, #196]
	str	r3, [r4, #312]
	mov	r3, #256
	ldr	r6, [r6, #48]
	str	r9, [r4, #200]
	str	ip, [r4, #292]
	str	ip, [r4, #316]
	str	r10, [r4, #296]
	str	r3, [r4, #300]
	str	r5, [r4, #304]
	str	r8, [r4, #308]
	str	lr, [r4, #2196]
	str	lr, [r4, #2200]
	str	lr, [r4, #2204]
	blx	r6
	str	r7, [r4, #2308]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2922:
	.align	2
.L2921:
	.word	.LC386
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_InitDecPara, .-HEVC_InitDecPara
	.align	2
	.global	HEVCDEC_Init
	.type	HEVCDEC_Init, %function
HEVCDEC_Init:
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
	add	r8, r0, #61440
	ldr	r7, .L2936
	movw	r2, #20216
	ldr	r5, [r8, #632]
	mov	r4, r0
	mov	r9, r1
	movt	r2, 1
	ldr	r3, [r7, #48]
	mov	r1, #0
	blx	r3
	str	r5, [r8, #632]
	mov	r3, #0
	str	r9, [r4, #2324]
	str	r3, [r4, #2328]
	ldr	r3, [r9, #28]
	cmp	r3, #25
	beq	.L2931
	mov	r3, #17
	mov	r1, #65
	str	r3, [r4, #148]
	mov	r2, #200
	str	r3, [r4, #152]
	mov	r3, #210
	str	r1, [r4, #156]
	str	r2, [r4, #144]
	str	r3, [r4, #160]
.L2925:
	ldr	r0, [r9, #584]
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L2932
	ldr	r3, [r4, #148]
	movw	r2, #23196
	ldr	r10, [r4, #2324]
	add	r6, r4, #65536
	ldr	r0, [r4, #152]
	mov	lr, #100
	mul	r3, r2, r3
	ldr	ip, [r10, #580]
	ldr	r2, [r4, #156]
	add	r1, r5, ip
	str	r1, [r4, #2312]
	add	ip, r3, ip
	str	r3, [fp, #-52]
	add	r3, r5, ip
	str	r3, [r4, #2316]
	movw	r3, #26164
	mla	ip, r3, r0, ip
	add	r3, r5, ip
	str	r3, [r4, #2320]
	movw	r3, #2024
	mla	ip, r3, r2, ip
	mov	r3, #1360
	add	r0, r5, ip
	str	r0, [r6, #1836]
	ldr	r2, [r4, #144]
	mul	r2, r3, r2
	add	ip, ip, r2
	add	r3, r5, ip
	str	r3, [r6, #1840]
	ldr	r3, [r4, #160]
	mla	lr, r3, lr, lr
	mov	r3, r3, asl #5
	str	lr, [fp, #-48]
	ldr	lr, [r4, #160]
	sub	r3, r3, lr, asl #2
	ldr	lr, [fp, #-48]
	add	ip, ip, lr
	add	r5, r5, ip
	str	r5, [r6, #2644]
	ldr	r5, [r10, #588]
	add	r3, ip, r3
	cmp	r3, r5
	bhi	.L2933
	ldr	r5, [r9, #608]
	cmp	r5, #0
	beq	.L2934
.L2929:
	ldr	r3, [r7, #48]
	mov	r1, #0
	blx	r3
	ldr	ip, [r4, #160]
	mov	r2, #100
	ldr	r3, [r7, #48]
	mov	r1, #0
	ldr	r0, [r6, #1840]
	mla	r2, ip, r2, r2
	blx	r3
	ldr	r3, [r4, #160]
	ldr	ip, [r7, #48]
	mov	r1, #0
	ldr	r0, [r6, #2644]
	mov	r5, #1
	mov	r2, r3, asl #5
	sub	r2, r2, r3, asl #2
	blx	ip
	ldr	lr, [r4, #148]
	movw	r3, #23196
	ldr	r6, [r4, #2312]
	add	ip, r4, #24576
	movw	r1, #2024
	mul	r3, r3, lr
	movw	r2, #26164
	mov	r0, r4
	sub	r3, r3, #23040
	sub	r3, r3, #156
	add	r6, r6, r3
	str	r6, [ip, #2832]
	ldr	r3, [r4, #156]
	ldr	r7, [r4, #2320]
	mul	r3, r1, r3
	sub	r3, r3, #2016
	sub	r3, r3, #8
	add	r3, r7, r3
	str	r3, [ip, #2840]
	ldr	r3, [r4, #152]
	ldr	r1, [r4, #2316]
	mul	r3, r2, r3
	sub	r3, r3, #26112
	sub	r3, r3, #52
	add	r3, r1, r3
	str	r3, [ip, #2836]
	str	lr, [r6, #8]
	ldr	r3, [ip, #2840]
	ldr	r2, [r4, #152]
	str	r2, [r3, #28]
	ldr	r3, [ip, #2836]
	ldr	r2, [r4, #156]
	str	r2, [r3, #60]
	bl	HEVC_InitScalingOrderTable
	add	r3, r4, #81920
	mov	r0, r4
	str	r5, [r3, #3820]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #244]
	beq	.L2935
	mov	r3, #-2147483647
	mov	r0, r4
	str	r3, [r4, #212]
	bl	HEVC_InitDecPara
	mov	r0, #0
	str	r0, [r8, #640]
	movw	r2, #9999
	ldr	r3, [r4, #156]
	strb	r5, [r4]
	str	r2, [r4, #252]
	str	r3, [r4, #288]
.L2927:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2934:
	ldr	r2, [fp, #-52]
	mov	r0, r1
	ldr	r3, [r7, #48]
	mov	r1, r5
	blx	r3
	ldr	r2, [r4, #152]
	movw	ip, #26164
	ldr	r3, [r7, #48]
	mov	r1, r5
	ldr	r0, [r4, #2316]
	mul	r2, ip, r2
	blx	r3
	ldr	r2, [r4, #156]
	movw	ip, #2024
	ldr	r3, [r7, #48]
	mov	r1, r5
	ldr	r0, [r4, #2320]
	mul	r2, ip, r2
	blx	r3
	ldr	r2, [r4, #144]
	mov	r3, #1360
	ldr	r0, [r6, #1836]
	mul	r2, r3, r2
	b	.L2929
.L2931:
	ldr	r3, [r9, #680]
	str	r3, [r4, #148]
	ldr	r3, [r9, #684]
	str	r3, [r4, #152]
	ldr	r3, [r9, #688]
	str	r3, [r4, #156]
	ldr	r3, [r9, #676]
	add	r2, r3, #5
	str	r2, [r4, #160]
	str	r3, [r4, #144]
	b	.L2925
.L2933:
	str	r5, [sp]
	mov	r0, #0
	ldr	r2, .L2936+4
	ldr	r1, .L2936+8
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L2927
.L2935:
	ldr	r2, .L2936+4
	mov	r0, #0
	ldr	r1, .L2936+12
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L2927
.L2932:
	ldr	r2, .L2936+4
	ldr	r1, .L2936+16
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L2927
.L2937:
	.align	2
.L2936:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+620
	.word	.LC388
	.word	.LC389
	.word	.LC387
	UNWIND(.fnend)
	.size	HEVCDEC_Init, .-HEVCDEC_Init
	.align	2
	.global	HEVC_ClearCurrPic
	.type	HEVC_ClearCurrPic, %function
HEVC_ClearCurrPic:
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
	bl	HEVC_ClearAllSlice
	ldr	r3, [r4, #388]
	cmp	r3, #0
	beq	.L2939
	ldr	r1, [r3, #32]
	mov	r2, #1
	ldr	r0, [r5, #244]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #388]
	mov	r3, #0
	str	r3, [r2, #4]
	str	r3, [r4, #388]
.L2939:
	mov	r0, #0
	strb	r0, [r4, #337]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_ClearCurrPic, .-HEVC_ClearCurrPic
	.align	2
	.global	HEVC_ClearAll
	.type	HEVC_ClearAll, %function
HEVC_ClearAll:
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
	mov	r5, r1
	bl	HEVC_ClearCurrPic
	mov	r0, r4
	mov	r1, #0
	bl	HEVC_InitDPB
	cmp	r5, #0
	bne	.L2946
	add	r0, r4, #61696
	add	r0, r0, #32
	bl	ResetVoQueue
	ldr	r0, [r4, #244]
	bl	FSP_EmptyInstance
.L2945:
	mov	r0, r4
	bl	HEVC_InitDecPara
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2946:
	add	r1, r4, #61696
	ldr	r0, [r4, #244]
	add	r1, r1, #32
	bl	FSP_ClearNotInVoQueue
	b	.L2945
	UNWIND(.fnend)
	.size	HEVC_ClearAll, .-HEVC_ClearAll
	.align	2
	.global	HEVC_InitPic
	.type	HEVC_InitPic, %function
HEVC_InitPic:
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
	ldr	r3, [r0, #256]
	mov	r4, r0
	add	r9, r0, #24576
	add	r5, r0, #53248
	str	r3, [fp, #-48]
	ldr	r3, [r4, #260]
	ldr	r2, [r9, #2836]
	ldr	r0, [r5, #1092]
	str	r3, [fp, #-52]
	ldr	r3, [r4, #264]
	cmp	r0, #0
	str	r3, [fp, #-56]
	ldr	r3, [r2, #100]
	str	r3, [fp, #-68]
	ldr	r3, [r2, #108]
	str	r3, [fp, #-72]
	ldr	r3, [r4, #268]
	str	r3, [fp, #-60]
	blt	.L2981
	ldr	r3, [r4, #156]
	cmp	r0, r3
	bge	.L2981
	ldr	lr, [r4, #2320]
	movw	r6, #2024
	movw	ip, #26164
	ldr	r3, [r4, #2316]
	ldr	r7, [r4, #2312]
	movw	r1, #23196
	mla	r6, r6, r0, lr
	ldr	r0, [r9, #2832]
	ldr	r0, [r0, #8]
	ldr	r10, [r6, #32]
	mul	r10, ip, r10
	add	r8, r3, r10
	ldr	ip, [r8, #52]
	mla	r1, r1, ip, r7
	ldr	ip, [r1, #8]
	cmp	r0, ip
	bne	.L2949
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L3004
.L2949:
	mov	r2, #0
	strb	r2, [r1, #2]
	ldr	r0, [r4, #148]
	movw	r7, #23196
	ldr	ip, .L3010
	mov	r2, r7
	ldr	lr, [r4, #2312]
	mul	r0, r7, r0
	ldr	ip, [ip, #52]
	str	r3, [fp, #-64]
	sub	r0, r0, #23040
	sub	r0, r0, #156
	add	r0, lr, r0
	blx	ip
	ldr	r0, [r4, #148]
	ldr	r2, [r9, #2836]
	ldr	r3, [fp, #-64]
	mul	r7, r7, r0
	ldr	r1, [r4, #2312]
	sub	r7, r7, #23040
	sub	r7, r7, #156
	add	r7, r1, r7
	str	r7, [r9, #2832]
.L2950:
	ldr	r0, [r2, #60]
	ldr	r1, [r8, #60]
	cmp	r0, r1
	beq	.L3005
.L2951:
	mov	r2, #0
	strb	r2, [r3, r10]
	ldr	r0, [r4, #152]
	movw	r7, #26164
	ldr	ip, [r9, #2836]
	mov	r2, r7
	ldr	lr, [r8, #236]
	mov	r1, r8
	mul	r0, r7, r0
	ldr	ip, [ip, #236]
	ldr	r3, .L3010
	ldr	r10, [r4, #2316]
	subs	ip, lr, ip
	sub	r0, r0, #26112
	sub	r0, r0, #52
	ldr	r3, [r3, #52]
	add	r0, r10, r0
	movne	ip, #1
	str	ip, [fp, #-64]
	blx	r3
	ldr	r3, [r4, #152]
	ldr	r2, [r4, #2316]
	mul	r7, r7, r3
	sub	r7, r7, #26112
	sub	r7, r7, #52
	add	r2, r2, r7
	str	r2, [r9, #2836]
	ldr	r3, [r2, #60]
	str	r3, [r4, #256]
	ldr	r3, [r2, #72]
	str	r3, [r4, #260]
	ldr	r3, [r2, #76]
	str	r3, [r4, #264]
	ldr	r3, [r2, #148]
	str	r3, [r4, #268]
.L2952:
	ldr	r3, [r2, #108]
	ldr	r2, [r2, #100]
	ldr	r1, [r4, #2324]
	cmp	r3, r2
	movcc	r3, r2
	str	r3, [r1, #884]
	ldr	r2, [r9, #2840]
	ldr	r3, [r6, #28]
	ldr	r2, [r2, #28]
	cmp	r2, r3
	beq	.L3006
.L2953:
	ldrb	r3, [r6, #13]	@ zero_extendqisi2
	mov	r7, #0
	strb	r7, [r6, #1]
	cmp	r3, r7
	beq	.L2955
	ldrb	r3, [r6, #15]	@ zero_extendqisi2
	cmp	r3, r7
	beq	.L2956
	ldr	r10, [r6, #68]
	cmp	r10, r7
	ble	.L2961
	add	ip, r6, #174
	str	r4, [fp, #-80]
	str	r6, [fp, #-76]
	mov	r4, ip
	str	r5, [fp, #-84]
.L2960:
	ldr	r6, [r8, #240]
	add	r7, r7, #1
	mov	r1, r10
	mul	r0, r6, r7
	bl	__aeabi_uidiv
	mov	r1, r10
	mov	r5, r0
	sub	r0, r7, #1
	mul	r0, r0, r6
	bl	__aeabi_uidiv
	cmp	r7, r10
	rsb	r0, r0, r5
	strh	r0, [r4, #2]!	@ movhi
	bne	.L2960
	ldr	r6, [fp, #-76]
	ldr	r4, [fp, #-80]
	ldr	r5, [fp, #-84]
.L2961:
	ldr	r10, [r6, #72]
	cmp	r10, #0
	ble	.L2962
	add	ip, r6, #214
	str	r4, [fp, #-80]
	mov	r7, #0
	str	r6, [fp, #-76]
	mov	r4, ip
	str	r5, [fp, #-84]
.L2963:
	ldr	r6, [r8, #244]
	add	r7, r7, #1
	mov	r1, r10
	mul	r0, r6, r7
	bl	__aeabi_uidiv
	mov	r1, r10
	mov	r5, r0
	sub	r0, r7, #1
	mul	r0, r0, r6
	bl	__aeabi_uidiv
	cmp	r7, r10
	rsb	r0, r0, r5
	strh	r0, [r4, #2]!	@ movhi
	bne	.L2963
	ldr	r6, [fp, #-76]
	ldr	r4, [fp, #-80]
	ldr	r5, [fp, #-84]
	b	.L2962
.L2981:
	mov	r0, #0
.L3003:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2955:
	ldr	r3, [r8, #240]
	strh	r3, [r6, #176]	@ movhi
	ldr	r3, [r8, #244]
	strh	r3, [r6, #216]	@ movhi
.L2962:
	ldr	r0, [r4, #156]
	movw	r7, #2024
	ldr	r3, .L3010
	mov	r2, r7
	ldr	ip, [r4, #2320]
	mov	r1, r6
	mul	r0, r7, r0
	ldr	r3, [r3, #52]
	sub	r0, r0, #2016
	sub	r0, r0, #8
	add	r0, ip, r0
	blx	r3
	ldr	r2, [r4, #156]
	ldr	r3, [r4, #2320]
	ldr	r1, [fp, #-48]
	mul	r7, r7, r2
	ldr	r2, [fp, #-64]
	sub	r7, r7, #2016
	sub	r7, r7, #8
	add	r7, r3, r7
	str	r7, [r9, #2840]
	ldr	r3, [r8, #60]
	cmp	r1, r3
	cmpeq	r2, #0
	bne	.L2969
	ldr	r3, [r4, #168]
	cmp	r3, #0
	beq	.L2969
	ldr	r3, [r8, #72]
	ldr	r2, [fp, #-52]
	cmp	r2, r3
	bne	.L2969
.L2979:
	ldr	r3, [r8, #76]
	ldr	r2, [fp, #-56]
	cmp	r2, r3
	bne	.L2969
	ldr	r3, [r4, #2324]
	ldr	r2, [r3, #872]
	cmp	r2, #1
	bne	.L2971
.L2970:
	ldr	r3, [r3, #888]
	cmp	r3, #1
	beq	.L3007
.L2974:
	add	r3, r4, #81920
	ldr	r3, [r3, #3820]
	cmp	r3, #1
	beq	.L3008
.L2975:
	mov	r1, #1
	mov	r0, r4
	bl	HEVC_InitDPB
	cmn	r0, #2
	beq	.L3003
	cmp	r0, #0
	bne	.L2976
	ldr	r3, [r4, #2324]
.L2973:
	mov	r2, #0
	str	r2, [r3, #888]
.L2971:
	mov	r0, r4
	bl	HEVC_AllocFrameStore
	subs	r6, r0, #0
	bne	.L3009
	ldr	r1, [r5, #1088]
	mov	r0, #1
	ldr	r2, [r5, #1064]
	mvn	r3, #0
	strb	r0, [r5, #337]
	movw	r7, #54240
	str	r1, [r5, #340]
	add	ip, r4, #54016
	str	r2, [r5, #344]
	movw	lr, #54256
	str	r3, [r5, #368]
	add	r0, r4, #53504
	ldr	r1, [r9, #2836]
	add	r0, r0, #136
	ldr	r8, [r1, #72]
	str	r8, [r5, #352]
	ldr	r8, [r1, #76]
	str	r8, [r5, #356]
	ldr	r8, [r1, #240]
	ldr	r1, [r1, #244]
	str	r6, [r5, #364]
	mul	r1, r1, r8
	str	r1, [r5, #360]
	ldr	r1, [r4, #2324]
	ldrd	r2, [r1, #56]
	strd	r2, [r4, r7]
	ldrd	r2, [r1, #72]
	strd	r2, [ip, #232]
	ldrd	r2, [r1, #88]
	strd	r2, [r4, lr]
	mvn	r2, #0
	ldr	ip, [r1, #104]
	mvn	r3, #0
	str	ip, [r5, #400]
	ldr	ip, [r1, #108]
	str	ip, [r5, #404]
	ldr	ip, [r1, #112]
	str	ip, [r5, #408]
	ldr	ip, [r1, #128]
	str	ip, [r5, #412]
	strd	r2, [r1, #56]
	ldr	r2, [r9, #2836]
	ldr	r3, [r2, #72]
	str	r3, [r5, #504]
	ldr	r3, [r2, #76]
	str	r3, [r5, #508]
	ldr	r3, [r2, #2660]
	ldr	r1, [r2, #64]
	mov	r3, r3, asl #5
	subs	r1, r1, #1
	uxtb	r3, r3
	movne	r1, #1
	orr	r3, r3, r1, asl #2
	str	r3, [r5, #500]
	ldr	r1, [r2, #2644]
	and	r1, r1, #7
	bl	SetAspectRatio
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #1064]
	str	r6, [r5, #488]
	str	r2, [r3, #12]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #500]
	str	r2, [r3, #180]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #488]
	str	r2, [r3, #168]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #396]
	str	r2, [r3, #76]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #392]
	str	r2, [r3, #72]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #504]
	str	r2, [r3, #184]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #508]
	str	r2, [r3, #188]
	ldr	r3, [r5, #388]
	ldr	r0, [r4, #244]
	ldr	r1, [r3, #32]
	bl	FSP_GetDispPhyFs
	cmp	r0, #0
	beq	.L2978
	ldr	r3, [r5, #488]
	mov	r1, r6
	ldr	r2, [r0, #8]
	adds	r3, r3, #0
	ldr	r0, [r4, #244]
	movne	r3, #1
	bl	FSP_SetStoreType
.L2978:
	ldr	r3, [r9, #2836]
	mov	r4, #0
	ldr	lr, [r5, #388]
	mov	r0, r4
	ldr	r2, [r3, #84]
	str	r2, [r5, #724]
	ldr	r7, [r3, #88]
	str	r7, [r5, #728]
	ldr	r1, [r3, #92]
	str	r1, [r5, #732]
	ldr	r6, [r3, #96]
	str	r6, [r5, #736]
	ldr	ip, [r3, #72]
	rsb	r2, r2, ip
	rsb	r2, r7, r2
	str	r2, [r5, #512]
	ldr	ip, [r3, #76]
	rsb	r1, r1, ip
	rsb	r1, r6, r1
	str	r1, [r5, #516]
	ldr	r1, [r3, #64]
	str	r1, [r5, #564]
	ldr	r1, [r3, #100]
	str	r1, [r5, #568]
	ldr	r3, [r3, #108]
	str	r3, [r5, #572]
	ldr	r3, [lr, #200]
	str	r3, [r5, #520]
	ldr	r3, [lr, #204]
	str	r3, [r5, #524]
	str	r2, [lr, #192]
	ldr	r2, [r5, #516]
	ldr	r3, [r5, #388]
	str	r2, [r3, #196]
	ldr	r2, [r5, #520]
	ldr	r3, [r5, #388]
	str	r2, [r3, #200]
	ldr	r2, [r5, #524]
	ldr	r3, [r5, #388]
	str	r2, [r3, #204]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #568]
	str	r2, [r3, #248]
	ldr	r3, [r5, #388]
	ldr	r2, [r5, #572]
	str	r2, [r3, #252]
	str	r4, [r5, #348]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3006:
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2953
	ldr	r3, [r4, #168]
	cmp	r3, #0
	beq	.L2953
	ldr	r3, [r8, #72]
	ldr	r2, [fp, #-52]
	cmp	r2, r3
	bne	.L2953
	ldr	r3, [r8, #76]
	ldr	r2, [fp, #-56]
	cmp	r2, r3
	bne	.L2953
	ldr	r3, [r8, #148]
	ldr	r2, [fp, #-60]
	cmp	r2, r3
	bne	.L2953
	ldr	r3, [r8, #60]
	ldr	r2, [fp, #-64]
	ldr	r1, [fp, #-48]
	cmp	r1, r3
	cmpeq	r2, #0
	beq	.L2979
.L2969:
	ldr	r3, [fp, #-64]
	mov	r0, r4
	ldr	r2, [fp, #-56]
	ldr	r1, [fp, #-52]
	str	r3, [sp]
	ldr	r3, [fp, #-60]
	bl	HEVC_GetReRangeFlag
	ldr	r3, [r4, #2324]
	cmp	r0, #0
	bne	.L2970
	ldr	r2, [r3, #872]
	cmp	r2, #1
	bne	.L2973
	b	.L2970
.L2956:
	ldr	lr, [r6, #68]
	ldr	r3, [r8, #240]
	cmp	lr, #1
	add	r0, r6, lr, lsl #1
	strh	r3, [r0, #174]	@ movhi
	ble	.L2967
	add	lr, lr, #44
	add	r1, r6, #90
	add	ip, r6, #174
	add	lr, r6, lr, lsl #1
.L2966:
	ldrh	r2, [r1, #2]!
	cmp	r1, lr
	strh	r2, [ip, #2]!	@ movhi
	ldrh	r3, [r0, #174]
	rsb	r3, r2, r3
	strh	r3, [r0, #174]	@ movhi
	bne	.L2966
.L2967:
	ldr	lr, [r6, #72]
	ldr	r3, [r8, #244]
	cmp	lr, #1
	add	r0, r6, lr, lsl #1
	strh	r3, [r0, #214]	@ movhi
	ble	.L2962
	add	lr, lr, #64
	add	r1, r6, #130
	add	ip, r6, #214
	add	lr, r6, lr, lsl #1
.L2968:
	ldrh	r2, [r1, #2]!
	cmp	r1, lr
	strh	r2, [ip, #2]!	@ movhi
	ldrh	r3, [r0, #214]
	rsb	r3, r2, r3
	strh	r3, [r0, #214]	@ movhi
	bne	.L2968
	b	.L2962
.L3004:
	ldr	r0, [r4, #168]
	cmp	r0, #0
	bne	.L2950
	b	.L2949
.L3009:
	ldr	r1, .L3010+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3005:
	ldrb	r1, [r3, r10]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2951
	ldr	r0, [r4, #168]
	cmp	r0, #0
	beq	.L2951
	ldr	r0, [r8, #72]
	ldr	ip, [fp, #-52]
	cmp	ip, r0
	bne	.L2951
	ldr	r0, [r8, #76]
	ldr	ip, [fp, #-56]
	cmp	ip, r0
	bne	.L2951
	ldr	r0, [r8, #148]
	ldr	ip, [fp, #-60]
	cmp	ip, r0
	streq	r1, [fp, #-64]
	bne	.L2951
	b	.L2952
.L3008:
	mov	r0, r4
	bl	HEVC_GetBackPicFromVOQueue
	b	.L2975
.L3007:
	ldr	ip, [r8, #108]
	mov	r0, #31
	ldr	r1, [fp, #-72]
	ldr	r3, [r8, #100]
	ldr	r2, [fp, #-68]
	str	r1, [sp]
	str	ip, [sp, #4]
	ldr	r1, .L3010+8
	bl	dprint_vfmw
	b	.L2974
.L2976:
	mov	r2, r0
	ldr	r1, .L3010+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	mov	r1, #0
	bl	HEVC_ClearAll
	mvn	r0, #0
	b	.L3003
.L3011:
	.align	2
.L3010:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC392
	.word	.LC390
	.word	.LC391
	UNWIND(.fnend)
	.size	HEVC_InitPic, .-HEVC_InitPic
	.align	2
	.global	HEVC_OutputFrmToVO
	.type	HEVC_OutputFrmToVO, %function
HEVC_OutputFrmToVO:
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
	mov	r4, r0
	mov	r5, r1
	bl	HEVC_GetImagePara
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_CheckFrameStore
	subs	r6, r0, #0
	beq	.L3020
	cmn	r6, #3
	moveq	r0, #0
	bne	.L3021
.L3015:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3020:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_SetDisplay
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L3022
	add	r3, r4, #61696
	ldr	r0, [r4, #244]
	add	r3, r3, #32
	str	r7, [sp]
	mov	r2, r4
	mov	r1, #16
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L3023
	ldr	r3, [r4, #2324]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	strcc	r6, [r7, #24]
	bcs	.L3024
.L3018:
	ldr	r2, [r4, #284]
	mov	r0, #0
	ldr	r3, [r4, #276]
	add	r2, r2, #1
	str	r2, [r4, #284]
	add	r3, r3, #1
	str	r3, [r4, #276]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3024:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_SetFrmRepeatCount.part.3
	b	.L3018
.L3021:
	mov	r3, r6
	movw	r2, #9561
	ldr	r1, .L3025
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r5, #228]
	mov	r0, r4
	bl	HEVC_DECRecycleImage
	mvn	r0, #0
	b	.L3015
.L3023:
	mov	r2, r0
	ldr	r1, .L3025+4
	mov	r0, r6
	bl	dprint_vfmw
	mov	r2, r6
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_SetDisplay
	mov	r0, r4
	mov	r1, #1
	bl	HEVC_ClearAll
	mvn	r0, #0
	b	.L3015
.L3022:
	ldr	r2, .L3025+8
	ldr	r1, .L3025+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3015
.L3026:
	.align	2
.L3025:
	.word	.LC395
	.word	.LC394
	.word	.LANCHOR1+636
	.word	.LC393
	UNWIND(.fnend)
	.size	HEVC_OutputFrmToVO, .-HEVC_OutputFrmToVO
	.align	2
	.global	HEVC_DirectOutput
	.type	HEVC_DirectOutput, %function
HEVC_DirectOutput:
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
	add	r6, r0, #53248
	mov	ip, #2
	mov	r5, #0
	mov	r3, #1
	ldr	r4, [r6, #388]
	mov	r7, r0
	add	r1, r6, #392
	mov	r2, #624
	add	r0, r4, #72
	str	ip, [r4, #4]
	strb	r3, [r4, #2]
	strb	r5, [r4, #1]
	ldr	r3, [r6, #348]
	str	r3, [r4, #16]
	ldr	r3, [r6, #376]
	str	r3, [r4, #20]
	bl	memcpy
	str	r5, [r4, #48]
	ldr	r3, [r6, #376]
	mov	r2, r5
	ldr	r1, [r4, #32]
	str	r3, [r4, #40]
	ldr	r0, [r7, #244]
	bl	FSP_SetRef
	mov	r1, r4
	mov	r0, r7
	bl	HEVC_OutputFrmToVO
	ldr	r3, .L3028
	str	r5, [r4, #4]
	mov	r1, r5
	mov	r2, #680
	ldr	r3, [r3, #48]
	mov	r6, r0
	add	r0, r7, #53504
	add	r0, r0, #80
	blx	r3
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3029:
	.align	2
.L3028:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_DirectOutput, .-HEVC_DirectOutput
	.align	2
	.global	HEVC_IModeProcess
	.type	HEVC_IModeProcess, %function
HEVC_IModeProcess:
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
	bl	HEVC_DirectOutput
	add	r2, r4, #53248
	ldr	r1, [r4, #180]
	ldr	r3, [r2, #372]
	cmp	r3, r1
	addne	r3, r3, #528
	movne	ip, #0
	addne	r4, r4, r3, lsl #2
	strne	ip, [r4, #4]
	strne	r1, [r2, #372]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IModeProcess, .-HEVC_IModeProcess
	.align	2
	.global	HEVC_OutputFrmFromDPB
	.type	HEVC_OutputFrmFromDPB, %function
HEVC_OutputFrmFromDPB:
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
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	lr, [r0, #2508]
	mov	r4, r0
	cmp	lr, #0
	beq	.L3044
	add	r6, r0, #2512
	ldr	ip, [r0, #212]
	add	r6, r6, #12
	mov	r7, #0
	mov	r3, r7
	mov	r1, r6
.L3035:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L3034
	ldr	r2, [r2, #12]
	cmp	r2, ip
	addgt	r7, r7, #1
.L3034:
	cmp	r3, lr
	bne	.L3035
	add	r9, r4, #53248
	add	r2, r4, #24576
	mov	r5, #0
	str	r5, [fp, #-48]
	ldr	r1, [r9, #1084]
	ldr	r3, [r2, #2836]
	add	r3, r3, r1, lsl #2
	ldr	r8, [r3, #412]
.L3043:
	ldr	r1, [r6, #4]!
	mov	r2, r5
	ldrb	r10, [r4, #2]	@ zero_extendqisi2
	mov	r0, #22
	ldr	lr, [r9, #1064]
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	str	r8, [sp, #12]
	str	r7, [sp, #8]
	str	ip, [sp, #4]
	str	r10, [sp, #20]
	str	lr, [sp, #16]
	ldr	ip, [r1, #12]
	ldr	r1, .L3056
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r1, [r6]
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3037
	cmp	r7, r8
	ldr	r0, [r1, #12]
	ldr	r3, [r4, #212]
	movls	ip, #0
	movhi	ip, #1
	cmp	r0, r3
	movle	ip, #0
	cmp	ip, #0
	beq	.L3038
	ldr	ip, [r9, #1064]
	cmp	r0, ip
	blt	.L3039
	ldrb	r0, [r4, #2]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L3039
.L3038:
	cmp	r3, #0
	bne	.L3040
	ldr	r0, [r1, #12]
	cmp	r0, #0
	bge	.L3040
	cmp	r2, #0
	beq	.L3055
.L3041:
	str	r0, [sp]
	mov	r0, #31
	ldr	r2, .L3056+4
	ldr	r1, .L3056+8
	bl	dprint_vfmw
.L3040:
	ldr	r3, [r4, #2508]
	add	r5, r5, #1
	cmp	r3, r5
	bls	.L3053
.L3042:
	ldr	ip, [r4, #212]
	b	.L3043
.L3039:
	mov	r3, #1
	mov	r0, r4
	strb	r3, [r1, #2]
	add	r5, r5, #1
	ldr	r1, [r6]
	sub	r7, r7, #1
	bl	HEVC_OutputFrmToVO
	ldr	r3, [r6]
	ldr	r3, [r3, #12]
	str	r3, [r4, #212]
	ldr	r3, [r4, #2508]
	cmp	r3, r5
	str	r0, [fp, #-48]
	bhi	.L3042
.L3053:
	ldr	r0, [fp, #-48]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3037:
	ldr	r3, [r4, #212]
	b	.L3038
.L3055:
	mov	r2, r3
	ldr	r0, [r4, #244]
	ldr	r1, [r1, #32]
	bl	FSP_SetDisplay
	ldr	r2, [r6]
	ldr	r3, [r4, #212]
	ldr	r0, [r2, #12]
	b	.L3041
.L3044:
	str	lr, [fp, #-48]
	b	.L3053
.L3057:
	.align	2
.L3056:
	.word	.LC396
	.word	.LANCHOR1+656
	.word	.LC397
	UNWIND(.fnend)
	.size	HEVC_OutputFrmFromDPB, .-HEVC_OutputFrmFromDPB
	.align	2
	.global	HEVC_OutputOneFrmFromDPB
	.type	HEVC_OutputOneFrmFromDPB, %function
HEVC_OutputOneFrmFromDPB:
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
	add	r4, r0, r1, lsl #2
	mov	r2, #1
	mov	r6, r1
	mov	r5, r0
	ldr	r3, [r4, #2528]
	strb	r2, [r3, #2]
	ldr	r1, [r4, #2528]
	bl	HEVC_OutputFrmToVO
	ldr	r3, [r4, #2528]
	ldr	r2, [r3, #12]
	str	r2, [r5, #212]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r7, r0
	bne	.L3059
	mov	r1, r6
	mov	r0, r5
	bl	HEVC_RemoveApcStore
	mov	r1, r6
	mov	r0, r5
	bl	HEVC_RemoveFrameStoreOutDPB
.L3059:
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_OutputOneFrmFromDPB, .-HEVC_OutputOneFrmFromDPB
	.align	2
	.global	HEVC_OutputUnRefPicWithMinPoc
	.type	HEVC_OutputUnRefPicWithMinPoc, %function
HEVC_OutputUnRefPicWithMinPoc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	sub	r1, fp, #24
	mov	r4, r0
	bl	HEVC_GetUnRefPicWithMinPoc
	cmp	r0, #0
	bne	.L3064
	mov	r0, r4
	ldr	r1, [fp, #-24]
	bl	HEVC_OutputOneFrmFromDPB
	cmp	r0, #0
	bne	.L3065
.L3062:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3064:
	ldr	r2, .L3066
	mov	r0, #1
	ldr	r1, .L3066+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3062
.L3065:
	ldr	r2, .L3066
	mov	r0, #1
	ldr	r1, .L3066+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3062
.L3067:
	.align	2
.L3066:
	.word	.LANCHOR1+680
	.word	.LC398
	.word	.LC399
	UNWIND(.fnend)
	.size	HEVC_OutputUnRefPicWithMinPoc, .-HEVC_OutputUnRefPicWithMinPoc
	.align	2
	.global	HEVC_FlushOutputFrmFromDPB
	.type	HEVC_FlushOutputFrmFromDPB, %function
HEVC_FlushOutputFrmFromDPB:
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
	subs	r4, r0, #0
	beq	.L3077
	ldr	r3, [r4, #2508]
	cmp	r3, #0
	addne	r5, r4, #2512
	movne	r6, #0
	addne	r5, r5, #12
	movne	r8, #1
	bne	.L3075
	b	.L3076
.L3083:
	strb	r8, [r3, #2]
	ldr	r1, [r5]
	bl	HEVC_OutputFrmToVO
	cmn	r0, #1
	mov	r9, r0
	beq	.L3082
.L3073:
	ldr	r3, [r4, #2508]
	cmp	r3, r6
	bls	.L3076
.L3075:
	ldr	r3, [r5, #4]!
	mov	r2, #0
	ldr	r0, [r4, #244]
	add	r6, r6, #1
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r3, [r5]
	mov	r0, r4
	ldrb	r7, [r3, #2]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L3083
	ldr	r1, [r3, #32]
	ldr	r0, [r4, #244]
	bl	FSP_GetDisplay
	mov	r2, #0
	cmp	r0, #3
	beq	.L3073
	ldr	r3, [r5]
	ldr	r0, [r4, #244]
	ldr	r1, [r3, #32]
	bl	FSP_SetDisplay
	ldr	r3, [r4, #2508]
	cmp	r3, r6
	bhi	.L3075
.L3076:
	mov	r3, #-2147483647
	mov	r0, r4
	str	r3, [r4, #212]
	mov	r9, #0
	bl	Hevc_InitDecBuffers
.L3069:
	mov	r0, r9
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3082:
	mov	r0, r7
	ldr	r2, .L3084
	ldr	r1, .L3084+4
	bl	dprint_vfmw
	mov	r0, r9
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3077:
	mvn	r9, #0
	b	.L3069
.L3085:
	.align	2
.L3084:
	.word	.LANCHOR1+712
	.word	.LC400
	UNWIND(.fnend)
	.size	HEVC_FlushOutputFrmFromDPB, .-HEVC_FlushOutputFrmFromDPB
	.align	2
	.global	HEVC_ReceivePacket
	.type	HEVC_ReceivePacket, %function
HEVC_ReceivePacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r4, r0, #81920
	mov	r3, #0
	mov	r5, r0
	str	r3, [r4, #3800]
	ldr	r6, [r0, #2332]
	cmp	r6, r3
	beq	.L3108
	ldrb	r3, [r1]	@ zero_extendqisi2
	str	r3, [r6, #40]
	ldr	r3, [r0, #2324]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3109
.L3092:
	ldrb	r3, [r1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3093
	ldr	r3, [r5, #2332]
	ldr	r2, [r3, #8]
	cmp	r2, #1
	bls	.L3094
	ldr	r0, [r3, #40]
	cmp	r0, #1
	beq	.L3093
.L3094:
	ldr	r0, [r1, #12]
	cmp	r0, #0
	ble	.L3095
	ldr	ip, [r1, #4]
	cmp	ip, #0
	beq	.L3095
	ldr	r0, [r1, #8]
	cmp	r0, #0
	bne	.L3110
.L3095:
	mov	r2, #1
	str	r2, [r3, #40]
	ldr	r1, [r1, #16]
	ldr	r0, [r5, #244]
	bl	SM_ReleaseStreamSeg
.L3091:
	ldr	r3, [r5, #2332]
	ldr	r0, [r3, #40]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
.L3090:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3109:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	bne	.L3091
	b	.L3092
.L3110:
	mov	r0, r2, asl #5
	mov	lr, #0
	sub	r2, r0, r2, asl #2
	mov	r0, r5
	add	r3, r3, r2
	str	ip, [r3, #44]
	ldr	r3, [r5, #2332]
	ldr	r6, [r1, #12]
	ldr	ip, [r3, #8]
	mov	r2, ip, asl #5
	sub	r2, r2, ip, asl #2
	add	r3, r3, r2
	str	r6, [r3, #48]
	ldr	r3, [r5, #2332]
	ldr	r6, [r1, #8]
	ldr	ip, [r3, #8]
	mov	r2, ip, asl #5
	sub	r2, r2, ip, asl #2
	add	r3, r3, r2
	str	r6, [r3, #52]
	ldr	r3, [r5, #2332]
	ldr	ip, [r1, #16]
	ldr	r1, [r3, #8]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	ip, [r3, #68]
	ldr	r3, [r5, #2332]
	ldr	r1, [r3, #8]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	lr, [r3, #60]
	ldr	r2, [r5, #2332]
	ldr	r3, [r2, #8]
	add	r3, r3, #1
	str	r3, [r2, #8]
	bl	HEVC_CombinePacket
	mov	r3, #1
	str	r3, [r4, #3800]
	b	.L3091
.L3108:
	ldr	r3, [r1, #12]
	cmp	r3, #0
	ble	.L3088
	ldr	r3, [r1, #4]
	cmp	r3, #0
	beq	.L3088
	ldr	r3, [r1, #8]
	str	r1, [fp, #-32]
	cmp	r3, #0
	beq	.L3088
	bl	HEVC_FindNaluArraySlot
	ldr	r1, [fp, #-32]
	cmn	r0, #1
	mov	r4, r0
	beq	.L3111
	add	r3, r5, #65536
	mov	r0, #100
	mov	r2, #1
	ldr	r3, [r3, #1840]
	mla	r4, r0, r4, r3
	str	r4, [r5, #2332]
	ldr	r3, [r1, #4]
	str	r3, [r4, #44]
	ldr	r3, [r5, #2332]
	ldr	r0, [r1, #12]
	str	r0, [r3, #48]
	ldr	r3, [r5, #2332]
	ldr	r0, [r1, #16]
	str	r0, [r3, #68]
	ldr	r3, [r5, #2332]
	ldr	r0, [r1, #8]
	str	r0, [r3, #52]
	ldr	r3, [r5, #2332]
	str	r6, [r3, #60]
	ldr	r3, [r5, #2332]
	str	r2, [r3, #8]
	ldr	r3, [r5, #2332]
	strb	r6, [r3, #1]
	ldrb	r2, [r1]	@ zero_extendqisi2
	ldr	r3, [r5, #2332]
	str	r2, [r3, #40]
	b	.L3091
.L3093:
	ldr	r1, .L3112
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3090
.L3088:
	ldr	r1, .L3112+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3090
.L3111:
	ldr	r1, .L3112+8
	mov	r0, r6
	bl	dprint_vfmw
	mov	r1, r6
	mov	r0, r5
	bl	HEVC_ClearAll
	mov	r0, r4
	b	.L3090
.L3113:
	.align	2
.L3112:
	.word	.LC403
	.word	.LC402
	.word	.LC401
	UNWIND(.fnend)
	.size	HEVC_ReceivePacket, .-HEVC_ReceivePacket
	.align	2
	.global	HEVCDEC_GetImageBuffer
	.type	HEVCDEC_GetImageBuffer, %function
HEVCDEC_GetImageBuffer:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r3, [r0, #176]
	mov	r4, r0
	cmp	r3, #0
	beq	.L3117
	add	r3, r0, #53248
	ldrb	r3, [r3, #337]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3117
	add	r2, r0, #67584
	add	r0, r0, #81920
	add	r2, r2, #600
	add	r0, r0, #344
.L3123:
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne	.L3118
	cmp	r2, #0
	beq	.L3119
	ldr	lr, [r4, #2508]
	cmp	lr, #0
	beq	.L3119
	ldr	r1, [r4, #2528]
	cmp	r1, r2
	beq	.L3118
	add	r1, r4, #2528
	b	.L3120
.L3121:
	ldr	ip, [r1, #4]!
	cmp	ip, r2
	beq	.L3118
.L3120:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L3121
.L3119:
	ldr	r0, [r4, #244]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L3117
	cmn	r0, #1
	beq	.L3142
.L3141:
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3117:
	mov	r0, #1
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3118:
	add	r2, r2, #704
	cmp	r2, r0
	bne	.L3123
	add	r0, r4, #61696
	sub	r2, fp, #24
	sub	r1, fp, #28
	add	r0, r0, #32
	bl	GetQueueImgNum
	mov	r0, r4
	bl	HEVC_PrintFrameStoreState
	ldr	r1, .L3143
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_PrintDPBState
	ldr	r2, [r4, #176]
	ldr	r0, [r4, #2508]
	ldr	r3, [fp, #-28]
	sub	r2, r2, #2
	ldr	r1, [fp, #-24]
	rsb	r2, r0, r2
	add	r0, r3, r1
	cmp	r0, r2
	bge	.L3141
	str	r1, [sp]
	mov	r0, #0
	ldr	r2, .L3143+4
	ldr	r1, .L3143+8
	bl	dprint_vfmw
	mov	r0, r4
	mov	r1, #1
	bl	HEVC_ClearAll
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3142:
	add	r1, r4, #61696
	ldr	r0, [r4, #244]
	add	r1, r1, #32
	bl	FSP_ClearNotInVoQueue
	b	.L3141
.L3144:
	.align	2
.L3143:
	.word	.LC404
	.word	.LANCHOR1+740
	.word	.LC405
	UNWIND(.fnend)
	.size	HEVCDEC_GetImageBuffer, .-HEVCDEC_GetImageBuffer
	.align	2
	.global	HEVC_FlushDPB
	.type	HEVC_FlushDPB, %function
HEVC_FlushDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
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
	ldr	r3, [r0, #2508]
	mov	r4, r0
	cmp	r3, #0
	beq	.L3147
	add	r6, r0, #2512
	mov	r5, #0
	add	r6, r6, #12
	mov	r7, r5
	b	.L3149
.L3148:
	ldr	r3, [r4, #2508]
	cmp	r3, r5
	bls	.L3147
.L3149:
	ldr	r3, [r6, #4]!
	add	r5, r5, #1
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3148
	strb	r7, [r3, #1]
	mov	r2, #0
	ldr	r1, [r3, #32]
	ldr	r0, [r4, #244]
	bl	FSP_SetRef
	ldr	r3, [r4, #2508]
	cmp	r3, r5
	bhi	.L3149
.L3147:
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L3151
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L3147
	b	.L3151
.L3153:
	bl	HEVC_GetMinPOC
	ldr	r1, [fp, #-36]
	mov	r0, r4
	bl	HEVC_OutputOneFrmFromDPB
	cmn	r0, #1
	beq	.L3152
.L3151:
	ldr	r3, [r4, #2508]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	bne	.L3153
	mov	r0, r3
.L3152:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_FlushDPB, .-HEVC_FlushDPB
	.align	2
	.global	HEVC_ClearDPB
	.type	HEVC_ClearDPB, %function
HEVC_ClearDPB:
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
	bl	HEVC_FlushDPB
	subs	r3, r0, #0
	beq	.L3162
	ldr	r2, .L3166
	mov	r0, #0
	ldr	r1, .L3166+4
	bl	dprint_vfmw
.L3162:
	mov	r0, r4
	mov	r1, #0
	bl	HEVC_InitDPB
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3167:
	.align	2
.L3166:
	.word	.LANCHOR1+764
	.word	.LC406
	UNWIND(.fnend)
	.size	HEVC_ClearDPB, .-HEVC_ClearDPB
	.align	2
	.global	HEVC_RecoverDPBProcess
	.type	HEVC_RecoverDPBProcess, %function
HEVC_RecoverDPBProcess:
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	mov	r4, r0
	b	.L3169
.L3171:
	str	lr, [sp]
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_IncreaseDPBSize
	cmp	r0, #0
	bne	.L3177
.L3169:
	ldr	ip, [r4, #2508]
	mov	r0, #13
	ldr	lr, [r4, #2504]
	ldr	r2, .L3178
	cmp	ip, lr
	ldr	r1, .L3178+4
	mov	r3, ip
	bcs	.L3171
	b	.L3173
.L3174:
	str	lr, [sp]
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_OutputUnRefPicWithMinPoc
	cmp	r0, #0
	bne	.L3172
	ldr	ip, [r4, #2508]
	ldr	lr, [r4, #2504]
.L3173:
	cmp	ip, lr
	mov	r3, ip
	ldr	r2, .L3178
	mov	r0, #1
	ldr	r1, .L3178+8
	bcs	.L3174
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3177:
	ldr	r2, .L3178
	mov	r0, #1
	ldr	r1, .L3178+12
	bl	dprint_vfmw
	ldr	ip, [r4, #2508]
	ldr	lr, [r4, #2504]
	b	.L3173
.L3172:
	ldr	r2, .L3178
	mov	r0, #1
	ldr	r1, .L3178+16
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_PrintDPBState
	mov	r0, r4
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	HEVC_ClearDPB
.L3179:
	.align	2
.L3178:
	.word	.LANCHOR1+780
	.word	.LC407
	.word	.LC409
	.word	.LC408
	.word	.LC399
	UNWIND(.fnend)
	.size	HEVC_RecoverDPBProcess, .-HEVC_RecoverDPBProcess
	.align	2
	.global	HEVC_InsertFrmInDPB
	.type	HEVC_InsertFrmInDPB, %function
HEVC_InsertFrmInDPB:
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
	cmp	r1, #16
	mov	r7, r1
	mov	r6, r0
	mov	r4, r2
	bhi	.L3184
	ldr	r3, [r2, #52]
	add	r5, r0, r1, lsl #2
	mov	r1, #1
	mov	r2, r1
	str	r3, [r5, #2528]
	str	r1, [r3, #4]
	ldr	r3, [r5, #2528]
	ldr	r0, [r0, #244]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldrb	r3, [r4]	@ zero_extendqisi2
	ldr	r0, [r5, #2528]
	mov	r2, #624
	adds	r3, r3, #0
	add	r1, r4, #56
	movne	r3, #1
	strb	r3, [r0, #1]
	ldr	r3, [r5, #2528]
	ldr	r0, [r4, #36]
	str	r0, [r3, #24]
	ldr	r0, [r5, #2528]
	add	r0, r0, #72
	bl	memcpy
	ldr	ip, [r5, #2528]
	ldr	lr, [r4, #40]
	add	r1, r7, #1
	ldr	r3, .L3186
	mov	r2, #4
	add	r0, r6, #2528
	str	lr, [ip, #40]
	str	lr, [ip, #20]
	ldr	ip, [r5, #2528]
	ldr	lr, [r4, #52]
	str	lr, [ip, #68]
	ldr	ip, [r5, #2528]
	ldr	lr, [r4, #12]
	str	lr, [ip, #44]
	str	lr, [ip, #16]
	ldr	ip, [r5, #2528]
	ldr	lr, [r4, #8]
	str	lr, [ip, #12]
	ldr	ip, [r5, #2528]
	ldr	lr, [r4, #8]
	str	lr, [ip, #36]
	bl	qsort
	ldr	r3, [r6, #2508]
	mov	r0, r6
	add	r3, r3, #1
	str	r3, [r6, #2508]
	ldr	r1, [r4, #52]
	bl	HEVC_GetAPC
	subs	r3, r0, #0
	bne	.L3185
	mov	r0, r3
	strb	r3, [r4, #1]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3184:
	mov	r3, r1
	ldr	r2, .L3186+4
	ldr	r1, .L3186+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	bl	HEVC_ClearDPB
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3185:
	ldr	r2, .L3186+4
	mov	r0, #0
	ldr	r1, .L3186+12
	bl	dprint_vfmw
	mov	r0, r6
	bl	HEVC_PrintDPBState
	mov	r0, r6
	bl	HEVC_ClearDPB
	mvn	r0, #6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3187:
	.align	2
.L3186:
	.word	compare_pic_by_poc_asc
	.word	.LANCHOR1+804
	.word	.LC410
	.word	.LC411
	UNWIND(.fnend)
	.size	HEVC_InsertFrmInDPB, .-HEVC_InsertFrmInDPB
	.align	2
	.global	HEVC_SimpleDPBProcess
	.type	HEVC_SimpleDPBProcess, %function
HEVC_SimpleDPBProcess:
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
	add	r4, r0, #53248
	mov	r5, r0
	ldrb	r3, [r4, #336]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3201
	ldr	r3, [r4, #388]
	mov	r2, #2
	str	r2, [r3, #4]
.L3190:
	ldr	r3, [r4, #388]
	mov	r2, #1
	mov	r0, r5
	strb	r2, [r3, #2]
	ldr	r1, [r4, #388]
	bl	HEVC_CheckFrameStore
	cmn	r0, #1
	mov	r4, r0
	beq	.L3202
	cmn	r0, #3
	beq	.L3194
	mov	r0, r5
	bl	HEVC_OutputCurrPic
	cmp	r0, #0
	bne	.L3203
.L3194:
	mov	r0, #0
.L3191:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3201:
	bl	HEVC_SimpleSlideDPB
	add	r2, r5, #53504
	add	r2, r2, #80
	ldr	r1, [r5, #2508]
	mov	r0, r5
	bl	HEVC_InsertFrmInDPB
	cmp	r0, #0
	beq	.L3190
	str	r0, [sp]
	movw	r3, #8674
	ldr	r2, .L3204
	mov	r0, #0
	ldr	r1, .L3204+4
	bl	dprint_vfmw
	mvn	r0, #6
	b	.L3191
.L3203:
	ldr	r2, .L3204
	mov	r0, #0
	ldr	r1, .L3204+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3191
.L3202:
	ldr	r2, .L3204
	mov	r0, #0
	ldr	r1, .L3204+12
	bl	dprint_vfmw
	mov	r0, r4
	b	.L3191
.L3205:
	.align	2
.L3204:
	.word	.LANCHOR1+824
	.word	.LC412
	.word	.LC93
	.word	.LC413
	UNWIND(.fnend)
	.size	HEVC_SimpleDPBProcess, .-HEVC_SimpleDPBProcess
	.align	2
	.global	HEVC_DecOrderProcess
	.type	HEVC_DecOrderProcess, %function
HEVC_DecOrderProcess:
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
	ldr	r1, [r0, #2508]
	mov	r4, r0
	ldr	r3, [r0, #2504]
	cmp	r1, r3
	bcs	.L3218
.L3207:
	add	r2, r4, #53504
	mov	r0, r4
	add	r2, r2, #80
	bl	HEVC_InsertFrmInDPB
	subs	r6, r0, #0
	bne	.L3219
	add	r3, r4, #53248
	mov	r0, r4
	ldr	r1, [r3, #388]
	bl	HEVC_CheckFrameStore
	cmn	r0, #1
	mov	r5, r0
	beq	.L3220
	cmn	r0, #3
	beq	.L3213
	mov	r0, r4
	bl	HEVC_OutputCurrPic
	cmp	r0, #0
	bne	.L3221
.L3213:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3218:
	bl	HEVC_RemoveUnusedApcStore
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #1
	beq	.L3217
	ldr	r2, .L3222
	mov	r0, #0
	ldr	r1, .L3222+4
	bl	dprint_vfmw
.L3217:
	ldr	r1, [r4, #2508]
	b	.L3207
.L3219:
	ldr	r2, .L3222
	mov	r0, #0
	ldr	r1, .L3222+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3221:
	mov	r0, r6
	ldr	r2, .L3222
	ldr	r1, .L3222+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3220:
	mov	r0, r6
	ldr	r2, .L3222
	ldr	r1, .L3222+16
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3223:
	.align	2
.L3222:
	.word	.LANCHOR1+848
	.word	.LC414
	.word	.LC415
	.word	.LC93
	.word	.LC413
	UNWIND(.fnend)
	.size	HEVC_DecOrderProcess, .-HEVC_DecOrderProcess
	.align	2
	.global	HEVC_DispOrderProcess
	.type	HEVC_DispOrderProcess, %function
HEVC_DispOrderProcess:
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
	add	r3, r0, #61440
	mov	r4, r0
	ldr	r3, [r3, #640]
	cmp	r3, #1
	beq	.L3225
	bl	HEVC_FirstFrameFastOut
	cmp	r0, #0
	bne	.L3242
.L3225:
	ldr	r1, [r4, #2508]
	ldr	r3, [r4, #2504]
	cmp	r1, r3
	bcs	.L3243
.L3227:
	add	r2, r4, #53504
	mov	r0, r4
	add	r2, r2, #80
	bl	HEVC_InsertFrmInDPB
	cmp	r0, #0
	bne	.L3244
	ldr	r3, [r4, #2508]
	cmp	r3, #0
	bne	.L3230
.L3231:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3230:
	mov	r0, r4
	bl	HEVC_OutputFrmFromDPB
	cmp	r0, #0
	beq	.L3231
	ldr	r2, .L3245
	mov	r0, #1
	ldr	r1, .L3245+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3243:
	mov	r0, r4
	bl	HEVC_RemoveUnusedApcStore
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #1
	beq	.L3241
	ldr	r1, .L3245+8
	mov	r0, #0
	bl	dprint_vfmw
.L3241:
	ldr	r1, [r4, #2508]
	b	.L3227
.L3244:
	mov	r2, r0
	ldr	r1, .L3245+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3242:
	ldr	r2, .L3245
	mov	r0, #0
	ldr	r1, .L3245+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3246:
	.align	2
.L3245:
	.word	.LANCHOR1+872
	.word	.LC419
	.word	.LC417
	.word	.LC418
	.word	.LC416
	UNWIND(.fnend)
	.size	HEVC_DispOrderProcess, .-HEVC_DispOrderProcess
	.align	2
	.global	HEVC_StorePicInDPB
	.type	HEVC_StorePicInDPB, %function
HEVC_StorePicInDPB:
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
	ldr	r3, [r0, #2324]
	mov	r4, r0
	ldr	r3, [r3, #596]
	cmp	r3, #1
	beq	.L3280
	add	r5, r0, #53248
	mov	r3, #0
	str	r3, [r5, #472]
	str	r3, [r5, #476]
.L3250:
	mov	r0, r4
	bl	HEVC_RemoveDummyFrame
	ldrb	r6, [r5, #337]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L3281
	ldr	r3, [r4, #2324]
	ldr	r2, [r5, #376]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bhi	.L3282
	ldr	r3, [r4, #2328]
	ldr	r2, [r5, #388]
	add	r3, r3, #4096
	ldr	r3, [r3, #1184]
	str	r3, [r2, #696]
	ldr	r3, [r4, #2328]
	ldr	r2, [r5, #388]
	add	r3, r3, #4096
	ldr	r3, [r3, #1188]
	str	r3, [r2, #700]
	ldr	r3, [r4, #248]
	cmp	r3, #2
	beq	.L3283
	ldr	r3, [r4, #2324]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	beq	.L3284
	cmp	r3, #1
	beq	.L3285
	cmp	r3, #0
	beq	.L3264
.L3265:
	mov	r0, r4
	mvn	r6, #0
	bl	HEVC_ClearCurrPic
	ldr	r2, .L3288
	ldr	r1, .L3288+4
	mov	r0, #1
	bl	dprint_vfmw
.L3263:
	mov	r3, #0
	mov	r0, r6
	strb	r3, [r5, #337]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3280:
	ldr	r2, .L3288+8
	add	r5, r0, #53248
	ldr	r2, [r2]
	add	r2, r2, #4096
	ldr	r0, [r2, #84]
	ldr	r1, [r2, #88]
	str	r3, [r5, #472]
	cmp	r1, #0
	moveq	r1, #1
	bl	__aeabi_uidiv
	str	r0, [r5, #476]
	b	.L3250
.L3281:
	ldr	r2, .L3288
	mov	r0, #1
	ldr	r1, .L3288+12
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3284:
	mov	r0, r4
	bl	HEVC_SimpleDPBProcess
	subs	r6, r0, #0
	bne	.L3286
.L3259:
	movw	r3, #9003
	mov	r2, r6
	ldr	r1, .L3288+16
	mov	r0, #14
	bl	dprint_vfmw
.L3257:
	cmn	r6, #1
	bne	.L3263
	b	.L3265
.L3282:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	ldr	r3, [r4, #2324]
	ldr	r2, [r5, #376]
	mov	r0, #1
	ldr	r1, .L3288+20
	ldr	r3, [r3, #4]
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3285:
	mov	r0, r4
	bl	HEVC_DecOrderProcess
	subs	r6, r0, #0
	beq	.L3261
	ldr	r2, .L3288
	mov	r0, #0
	ldr	r1, .L3288+24
	bl	dprint_vfmw
	ldr	r3, [r4, #2324]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L3257
.L3264:
	mov	r0, r4
	bl	HEVC_DispOrderProcess
	subs	r6, r0, #0
	bne	.L3262
.L3266:
	mov	r6, #0
	b	.L3263
.L3283:
	mov	r0, r4
	bl	HEVC_DirectOutput
	ldr	r3, [r5, #372]
	ldr	r2, [r4, #180]
	cmp	r3, r2
	addne	r3, r3, #528
	movne	r1, #0
	addne	r3, r4, r3, lsl #2
	strne	r1, [r3, #4]
	mov	r6, r0
	strne	r2, [r5, #372]
	cmp	r0, #0
	bne	.L3287
.L3256:
	movw	r3, #8988
	mov	r2, r6
	ldr	r1, .L3288+16
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3257
.L3261:
	ldr	r3, [r4, #2324]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L3266
	b	.L3264
.L3262:
	ldr	r2, .L3288
	mov	r0, #0
	ldr	r1, .L3288+28
	bl	dprint_vfmw
	b	.L3257
.L3287:
	ldr	r2, .L3288
	mov	r0, #0
	ldr	r1, .L3288+32
	bl	dprint_vfmw
	b	.L3256
.L3286:
	ldr	r2, .L3288
	mov	r0, #0
	ldr	r1, .L3288+36
	bl	dprint_vfmw
	b	.L3259
.L3289:
	.align	2
.L3288:
	.word	.LANCHOR1+896
	.word	.LC427
	.word	g_HwMem
	.word	.LC420
	.word	.LC423
	.word	.LC421
	.word	.LC425
	.word	.LC426
	.word	.LC422
	.word	.LC424
	UNWIND(.fnend)
	.size	HEVC_StorePicInDPB, .-HEVC_StorePicInDPB
	.align	2
	.global	HEVC_CreateLostPicture
	.type	HEVC_CreateLostPicture, %function
HEVC_CreateLostPicture:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 680
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #680)
	sub	sp, sp, #680
	add	r4, r0, #67584
	add	r4, r4, #600
	mov	r5, r0
	mov	r6, r1
	mov	r7, #0
.L3295:
	ldr	r2, [r4, #4]
	cmp	r2, #0
	bne	.L3291
	cmp	r4, #0
	beq	.L3292
	ldr	r3, [r5, #2508]
	cmp	r3, #0
	beq	.L3292
	ldr	r1, [r5, #2528]
	cmp	r4, r1
	beq	.L3291
	add	ip, r5, #2528
	b	.L3293
.L3294:
	ldr	lr, [ip, #4]!
	cmp	lr, r4
	beq	.L3291
.L3293:
	add	r2, r2, #1
	cmp	r2, r3
	bne	.L3294
.L3292:
	mov	r8, #704
	mov	r2, #624
	mla	r8, r8, r7, r5
	sub	r0, fp, #660
	mov	r9, #1
	mov	r3, #101
	str	r6, [fp, #-708]
	add	r8, r8, #67584
	str	r3, [fp, #-676]
	add	r1, r8, #672
	strb	r9, [fp, #-715]
	strb	r9, [fp, #-716]
	bl	memcpy
	cmp	r7, #19
	strb	r9, [r8, #602]
	mov	r7, #0
	strb	r9, [r8, #600]
	str	r4, [fp, #-664]
	str	r7, [r8, #608]
	str	r7, [fp, #-500]
	bgt	.L3296
	ldr	r0, [r5, #2508]
	cmp	r0, #0
	beq	.L3298
	add	r1, r5, #2512
	mov	ip, r7
	add	r1, r1, #12
	mvn	r2, #-2147483648
	mov	r8, r1
.L3301:
	ldr	lr, [r8, #4]!
	add	ip, ip, #1
	ldr	r3, [lr, #12]
	rsb	r3, r6, r3
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, r2
	bge	.L3300
	ldrb	lr, [lr, #1]	@ zero_extendqisi2
	cmp	lr, #0
	movne	r2, r3
	movne	r7, #1
.L3300:
	cmp	ip, r0
	bne	.L3301
	cmp	r7, #0
	beq	.L3298
	mov	r4, #0
	b	.L3302
.L3303:
	add	r4, r4, #1
	cmp	r4, r0
	beq	.L3304
.L3302:
	ldr	ip, [r1, #4]!
	ldr	r3, [ip, #12]
	rsb	r3, r6, r3
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, r2
	bne	.L3303
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3303
	ldr	r1, [ip, #32]
	add	r4, r4, #632
	ldr	r0, [r5, #244]
	bl	FSP_GetLogicFs
	ldr	r2, [r5, r4, asl #2]
	mov	r3, r6
	ldr	r1, .L3335
	ldr	r2, [r2, #12]
	mov	r7, r0
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r7, #0
	beq	.L3304
	ldr	r0, [r7, #640]
	cmp	r0, #0
	beq	.L3333
	mov	r1, #0
	ldr	r0, [r5, #244]
	bl	FSP_NewLogicFs
	mov	r8, r0
	mov	r1, r0
	ldr	r0, [r5, #244]
	bl	FSP_GetLogicFs
	mvn	r2, r8
	mov	r2, r2, lsr #31
	cmp	r0, #0
	mov	r3, r0
	moveq	r2, #0
	cmp	r2, #0
	beq	.L3307
	ldr	r1, [fp, #-664]
	sub	r2, fp, #716
	mov	r0, r5
	str	r8, [r1, #32]
	ldr	r1, [r7, #640]
	str	r1, [r3, #640]
	ldr	r1, [r7, #644]
	str	r1, [r3, #644]
	ldr	r1, [r7, #648]
	str	r1, [r3, #648]
	ldrsb	r3, [r7, #4]
	ldr	r1, [r5, #2508]
	str	r3, [fp, #-680]
	bl	HEVC_InsertFrmInDPB
	subs	r4, r0, #0
	bne	.L3334
	ldr	r0, [r5, #244]
	mov	r1, r8
	mov	r2, r4
	bl	FSP_SetDisplay
	ldr	r3, [r5, #200]
	mov	r0, r4
	cmn	r3, #-2147483647
	streq	r6, [r5, #200]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3291:
	add	r7, r7, #1
	add	r4, r4, #704
	cmp	r7, #20
	bne	.L3295
.L3296:
	ldr	r1, .L3335+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
.L3328:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3304:
	ldr	r1, .L3335+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #3
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3307:
	ldr	r3, [fp, #-664]
	mov	r0, r2
	ldr	r1, .L3335+12
	strb	r2, [r3]
	bl	dprint_vfmw
	mvn	r0, #3
	b	.L3328
.L3334:
	ldr	r3, [fp, #-664]
	mov	r1, r8
	mov	r4, #0
	mov	r2, #1
	strb	r4, [r3]
	ldr	r0, [r5, #244]
	bl	FSP_ClearLogicFs
	mov	r0, r4
	ldr	r1, .L3335+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3328
.L3298:
	mov	r0, #0
	ldr	r1, .L3335+20
	strb	r0, [r4]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3328
.L3333:
	ldr	r1, .L3335+24
	bl	dprint_vfmw
	mov	r0, r5
	bl	HEVC_ClearDPB
	mvn	r0, #3
	b	.L3328
.L3336:
	.align	2
.L3335:
	.word	.LC431
	.word	.LC428
	.word	.LC429
	.word	.LC433
	.word	.LC434
	.word	.LC430
	.word	.LC432
	UNWIND(.fnend)
	.size	HEVC_CreateLostPicture, .-HEVC_CreateLostPicture
	.align	2
	.global	HEVC_RefPicProcess
	.type	HEVC_RefPicProcess, %function
HEVC_RefPicProcess:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	mov	r4, r0
	bl	HEVC_RemoveDummyFrame
	mov	r0, r4
	bl	HEVC_ApplyReferencePictureSet
	mov	r0, r4
	bl	HEVC_RemoveUnusedApcStore
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	b	.L3338
.L3341:
	ldr	r2, [fp, #-24]
	bl	dprint_vfmw
	ldr	r3, [r4, #2324]
	mov	r0, r4
	ldr	r2, [r3, #4]
	cmp	r2, #0
	beq	.L3339
	ldr	r1, [fp, #-24]
	bl	HEVC_CreateLostPicture
	cmp	r0, #0
	bne	.L3344
.L3338:
	sub	r1, fp, #24
	mov	r0, r4
	bl	HEVC_CheckThatAllRefPicsAreAvailable
	ldr	r1, .L3345
	cmp	r0, #0
	mov	r0, #1
	beq	.L3341
	mov	r0, r4
	bl	HEVC_UpdateReflist
	mov	r0, r4
	bl	HEVC_UpdateLTReflist
	mov	r0, r4
	bl	HEVC_RecoverDPBProcess
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3339:
	ldr	ip, [fp, #-24]
	add	r4, r4, #53248
	ldr	r1, .L3345+4
	mov	r0, #1
	ldr	r3, [r4, #1064]
	str	ip, [sp]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3344:
	mov	r0, r4
	bl	HEVC_RemoveDummyFrame
	mvn	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3346:
	.align	2
.L3345:
	.word	.LC435
	.word	.LC436
	UNWIND(.fnend)
	.size	HEVC_RefPicProcess, .-HEVC_RefPicProcess
	.align	2
	.global	HEVC_DecSlice
	.type	HEVC_DecSlice, %function
HEVC_DecSlice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, [r0, #2332]
	add	r5, r0, #53248
	mov	r1, #0
	mov	r4, r0
	ldr	r2, [r3, #16]
	str	r2, [r5, #1088]
	ldr	r3, [r3, #32]
	str	r3, [r5, #1084]
	strb	r1, [r0, #1]
	ldr	r3, [r5, #2604]
	cmp	r3, #1
	beq	.L3393
.L3348:
	ldr	r2, [r4, #208]
	ldr	r3, [r4, #144]
	cmp	r2, r3
	bcs	.L3394
	ldrb	r3, [r5, #337]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3395
	mov	r0, r4
	bl	HEVC_PicTypeStatistic
	mov	r0, r4
	bl	HEVC_CalcStreamBits
	ldr	r3, [r5, #2604]
	cmp	r3, #2
	beq	.L3361
	ldrb	r3, [r5, #1019]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3396
.L3362:
	mov	r0, r4
	bl	HEVC_WriteSliceMsg
	cmp	r0, #0
	bne	.L3397
.L3365:
	ldr	r2, [r5, #1092]
	add	r3, r4, #4096
	mov	r1, #1
	mov	r0, #0
	str	r2, [r4, #288]
	ldr	r2, [r5, #1064]
	str	r1, [r4, #168]
	str	r2, [r4, #252]
	ldr	r2, [r3, #444]
	str	r2, [r5, #380]
	ldr	r3, [r3, #448]
	str	r3, [r5, #384]
.L3351:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3396:
	mov	r0, r4
	bl	HEVC_DecList
	cmp	r0, #0
	bne	.L3398
	mov	r0, r4
	bl	HEVC_IsRefListWrong
	cmp	r0, #0
	beq	.L3362
	mov	r0, r4
	bl	HEVC_PrintDPBState
	ldr	r1, .L3403
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3393:
	sub	r2, r2, #16
	cmp	r2, #4
	bls	.L3349
	bl	HEVC_IsPVRFlushPic
	cmp	r0, #0
	beq	.L3350
.L3349:
	mov	r0, r4
	bl	HEVC_FlushOutputFrmFromDPB
	cmp	r0, #0
	bne	.L3399
.L3350:
	ldrb	r6, [r4, #2]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L3400
.L3352:
	ldr	r3, [r4, #2324]
	ldr	r2, [r3, #8]
	cmp	r2, #2
	beq	.L3354
	ldr	r3, [r3, #12]
	mov	r0, r4
	cmp	r3, #2
	beq	.L3401
	bl	HEVC_RefPicProcess
	cmp	r0, #0
	bne	.L3367
.L3354:
	mov	r0, r4
	bl	HEVC_InitPic
	cmn	r0, #2
	beq	.L3351
	cmp	r0, #0
	bne	.L3402
	ldr	r3, [r4, #220]
	mov	r0, r4
	add	r3, r3, #1
	str	r3, [r4, #220]
	bl	HEVC_SetScalingList
	mov	r0, r4
	bl	HEVC_WritePicMsg
	cmp	r0, #0
	beq	.L3348
	ldr	r1, .L3403+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3361:
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	beq	.L3365
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
	b	.L3365
.L3400:
	ldr	r3, [r4, #2508]
	cmp	r3, #0
	beq	.L3352
	mov	r0, r4
	bl	HEVC_EmptyFrmFromDPB
	cmp	r0, #0
	beq	.L3352
	mov	r0, r6
	ldr	r2, .L3403+8
	ldr	r1, .L3403+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3401:
	bl	HEVC_UpdateReflist
	mov	r0, r4
	bl	HEVC_UpdateLTReflist
	b	.L3354
.L3395:
	ldr	r1, .L3403+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3394:
	ldr	r1, .L3403+20
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #208]
	ldr	r2, [r4, #144]
	cmp	r3, r2
	bcc	.L3359
	ldr	r1, .L3403+24
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L3359
	str	r3, [fp, #-36]
	mov	r1, #108
	str	r2, [fp, #-32]
	mov	r3, #8
	sub	r2, fp, #36
	ldr	r0, [r4, #244]
	blx	ip
.L3359:
	ldr	r1, .L3403+28
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mvn	r0, #0
	b	.L3351
.L3398:
	mov	r2, r0
	ldr	r1, .L3403+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3397:
	ldr	r1, .L3403+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3402:
	ldr	r1, .L3403+40
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3367:
	mvn	r0, #0
	b	.L3351
.L3399:
	ldr	r2, .L3403+8
	mov	r0, #1
	ldr	r1, .L3403+44
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3351
.L3404:
	.align	2
.L3403:
	.word	.LC445
	.word	.LC440
	.word	.LANCHOR1+916
	.word	.LC438
	.word	.LC443
	.word	.LC441
	.word	g_event_report
	.word	.LC442
	.word	.LC444
	.word	.LC446
	.word	.LC439
	.word	.LC437
	UNWIND(.fnend)
	.size	HEVC_DecSlice, .-HEVC_DecSlice
	.align	2
	.global	HEVCDEC_VDMPostProc
	.type	HEVCDEC_VDMPostProc, %function
HEVCDEC_VDMPostProc:
	UNWIND(.fnstart)
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [fp, #4]
	add	ip, r0, #53248
	mov	r4, r0
	mov	r5, r3
	cmp	r2, #0
	str	r1, [ip, #376]
	ldr	r2, [fp, #8]
	beq	.L3406
	mov	ip, r2, asl #6
	ldr	r3, .L3416
	sub	r2, ip, r2, asl #3
	add	r2, r2, r3
	ldr	r2, [r2, #4]
	cmp	r2, #3
	beq	.L3414
	cmp	r2, #1
	beq	.L3406
.L3409:
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_StorePicInDPB
	subs	r2, r0, #0
	bne	.L3415
	mov	r3, #1
	mov	r0, r2
	str	r3, [r4, #168]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3415:
	ldr	r1, .L3416+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mov	r0, r4
	bl	HEVC_ClearDPB
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3406:
	mov	r0, r4
	bl	HEVC_ClearAllSlice
	b	.L3409
.L3414:
	bl	HEVC_ClearAllSlice
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3417:
	.align	2
.L3416:
	.word	g_VdmDrvParam+40
	.word	.LC447
	UNWIND(.fnend)
	.size	HEVCDEC_VDMPostProc, .-HEVCDEC_VDMPostProc
	.align	2
	.global	HEVC_DecVDM
	.type	HEVC_DecVDM, %function
HEVC_DecVDM:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, [r0, #172]
	mov	r4, r0
	cmp	r3, #2
	bhi	.L3419
	bl	HEVC_ClearCurrPic
	ldr	r3, .L3487
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3420
	mov	r3, #0
	mov	r1, #127
	mov	r2, r3
	ldr	r0, [r4, #244]
	blx	ip
.L3420:
	ldr	r3, [r4, #224]
	cmp	r3, #524288
	bhi	.L3481
	ldr	r1, .L3487+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
.L3476:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3419:
	ldr	r3, [r0, #280]
	add	r6, r0, #53248
	ldr	r2, [r0, #284]
	add	r3, r3, #1
	ldr	r1, [r0, #2324]
	cmp	r3, r2
	movlt	r3, r2
	str	r3, [r0, #280]
	rsb	r3, r2, r3
	str	r3, [r1, #576]
	bl	HEVC_IsRefForCurrPic
	strb	r0, [r6, #336]
	ldr	r5, [r4, #232]
	ldr	r2, [r4, #248]
	ubfx	r5, r5, #16, #3
	cmp	r5, r2
	beq	.L3423
	sub	r1, r2, #2
	cmp	r1, #1
	movhi	r1, #0
	movls	r1, #1
	cmp	r5, #2
	moveq	r1, #0
	cmp	r1, #0
	beq	.L3423
	cmp	r5, #4
	cmpne	r5, #1
	bls	.L3482
.L3423:
	cmp	r5, #2
	str	r5, [r4, #248]
	beq	.L3425
	cmp	r5, #3
	beq	.L3426
	cmp	r5, #1
	beq	.L3483
	ldr	r3, [r4, #216]
	cmp	r3, #0
	beq	.L3446
	cmp	r5, #4
	beq	.L3484
.L3432:
	ldr	r2, [r6, #348]
.L3433:
	cmp	r2, #2
	beq	.L3485
	cmp	r2, #1
	streqb	r2, [r4, #4]
.L3438:
	ldr	r2, [r4, #220]
	mov	r0, #2
	ldr	r1, .L3487+8
	sub	r2, r2, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #208]
	cmp	r3, #0
	beq	.L3486
	ldr	r0, [r6, #364]
	add	r2, r4, #61440
	clz	r1, r0
	mov	r1, r1, lsr #5
	str	r1, [r2, #1976]
	ldr	r1, [r6, #368]
	str	r3, [r2, #3208]
	str	r1, [r2, #1968]
	ldr	r3, [r4, #2324]
	ldr	r1, [r3, #592]
	str	r1, [r2, #1960]
	ldr	r3, [r3, #632]
	str	r3, [r2, #1964]
	ldr	r3, [r4, #220]
	cmp	r3, #0
	beq	.L3478
	add	r1, r4, #61952
	add	r3, r4, #65536
	add	r1, r1, #132
	str	r1, [r4, #2328]
	ldr	r1, [r6, #380]
	add	r0, r0, #1
	str	r1, [r3, #1828]
	ldr	r1, [r6, #384]
	str	r1, [r3, #1832]
	ldr	r1, [r6, #348]
	str	r0, [r6, #364]
	cmp	r1, #0
	bne	.L3478
	ldr	r2, [r2, #3212]
	cmp	r2, #0
	beq	.L3447
.L3445:
	ldr	r3, [r2]
	ldr	r0, [r2, #4]
	ldr	r2, [r2, #1356]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3445
.L3444:
	ldr	r3, .L3487
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3478
	sub	r2, fp, #28
	ldr	r0, [r4, #244]
	mov	r3, #4
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
.L3478:
	mov	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3481:
	ldr	r1, .L3487+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #7
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3483:
	ldr	r3, [r6, #348]
	cmp	r0, #0
	cmpeq	r3, #2
	beq	.L3429
.L3428:
	ldr	r3, [r4, #216]
	cmp	r3, #0
	bne	.L3432
.L3446:
	ldr	r3, .L3487
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3429
	mov	r3, #0
	mov	r1, #127
	mov	r2, r3
	ldr	r0, [r4, #244]
	blx	ip
	b	.L3429
.L3482:
	mov	r3, r5
	ldr	r1, .L3487+16
	mov	r0, #1
	bl	dprint_vfmw
	str	r5, [r4, #248]
.L3426:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mov	r0, r4
	bl	HEVC_ClearDPB
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3425:
	ldr	r3, [r6, #348]
	cmp	r3, #0
	beq	.L3428
	b	.L3429
.L3485:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3438
	ldr	r3, [r4, #2324]
	ldr	r5, [r3, #12]
	cmp	r5, #0
	bne	.L3438
	ldr	r3, .L3487+20
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3438
	mov	r3, r0
	ldr	r2, [r6, #344]
	ldr	r1, .L3487+24
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3487
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3429
	mov	r3, r5
	mov	r2, r5
	mov	r1, #127
	ldr	r0, [r4, #244]
	blx	ip
.L3429:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mvn	r0, #0
	b	.L3476
.L3484:
	ldr	r2, [r6, #348]
	cmp	r2, #0
	moveq	r3, #2
	streqb	r3, [r4, #4]
	beq	.L3438
	b	.L3433
.L3486:
	ldr	r2, .L3487
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3441
	mov	r2, r3
	mov	r1, #127
	ldr	r0, [r4, #244]
	blx	ip
.L3441:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	ldr	r2, .L3487+28
	ldr	r1, .L3487+32
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3476
.L3447:
	mov	r1, r2
	b	.L3444
.L3488:
	.align	2
.L3487:
	.word	g_event_report
	.word	.LC449
	.word	.LC452
	.word	.LC448
	.word	.LC450
	.word	g_allow_B_before_P
	.word	.LC451
	.word	.LANCHOR1+932
	.word	.LC453
	UNWIND(.fnend)
	.size	HEVC_DecVDM, .-HEVC_DecVDM
	.align	2
	.global	HEVC_DecOneNal
	.type	HEVC_DecOneNal, %function
HEVC_DecOneNal:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
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
	mov	r4, r0
	ldr	r0, [r0, #2332]
	ldr	r3, [r0, #8]
	cmp	r3, #0
	ldrne	r1, [r4, #224]
	movne	r2, r0
	movne	r3, #0
	beq	.L3494
.L3493:
	ldr	ip, [r2, #48]
	add	r3, r3, #1
	add	r2, r2, #28
	add	r1, r1, ip
	str	r1, [r4, #224]
	ldr	ip, [r0, #8]
	cmp	ip, r3
	bhi	.L3493
.L3494:
	mov	r3, #0
	strb	r3, [r0, #1]
	ldr	r3, [r4, #2332]
	ldr	r2, [r3, #16]
	sub	r2, r2, #36
	cmp	r2, #1
	bls	.L3624
.L3492:
	ldr	r2, [r3, #4]
	add	r2, r2, #16
	str	r2, [r3, #4]
	ldr	r1, [r4, #2332]
	ldr	r2, [r1, #16]
	cmp	r2, #49
	ldrls	pc, [pc, r2, asl #2]
	b	.L3495
.L3497:
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3496
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3498
	.word	.L3499
	.word	.L3500
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3501
	.word	.L3501
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3495
	.word	.L3502
	.word	.L3503
.L3624:
	mov	r2, #1
	strb	r2, [r4, #1]
	b	.L3492
.L3503:
	mov	r0, r4
	bl	HEVC_IsEosNal
	cmp	r0, #1
	beq	.L3625
.L3519:
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	beq	.L3612
.L3614:
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #2332]
.L3611:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3496:
	mov	r0, r4
	bl	HEVC_DecSlice
	cmn	r0, #2
	beq	.L3611
	cmp	r0, #0
	beq	.L3612
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	beq	.L3622
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3622:
	ldr	r3, .L3627
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3613
.L3615:
	mov	r3, #0
	ldr	r0, [r4, #244]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3613:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3498:
	mov	r0, r4
	bl	HEVC_DecVPS
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3504
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3504:
	cmp	r5, #0
	bne	.L3626
.L3612:
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3499:
	mov	r0, r4
	bl	HEVC_DecSPS
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3509
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3509:
	cmp	r5, #0
	beq	.L3612
	ldr	r6, .L3627
	mov	r0, #1
	ldr	r1, .L3627+4
	bl	dprint_vfmw
	ldr	ip, [r6]
	cmp	ip, #0
	bne	.L3617
	b	.L3613
.L3500:
	mov	r0, r4
	bl	HEVC_DecPPS
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3511
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3511:
	cmp	r5, #0
	beq	.L3612
	ldr	r6, .L3627
	mov	r0, #1
	ldr	r1, .L3627+8
	bl	dprint_vfmw
	ldr	ip, [r6]
	cmp	ip, #0
	beq	.L3613
.L3617:
	sub	r2, fp, #28
	mvn	r5, #0
	mov	r3, #4
	mov	r1, #100
	str	r5, [r2, #-4]!
	ldr	r0, [r4, #244]
	blx	ip
	ldr	ip, [r6]
	cmp	ip, #0
	bne	.L3615
	b	.L3613
.L3501:
	mov	r0, r4
	bl	HEVC_DecSEI
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3512
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3512:
	cmp	r5, #0
	beq	.L3612
	ldr	r1, .L3627+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3622
.L3502:
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3517
	cmp	r1, #0
	beq	.L3516
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3516:
	add	r3, r4, #53248
	ldrb	r3, [r3, #337]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3612
.L3517:
	add	r5, r4, #2464
	mov	r1, #32
	add	r5, r5, #8
	mov	r6, #0
	strb	r6, [r4, #6]
	mov	r0, r5
	bl	BsGet
	mov	r1, #32
	mov	r7, r0
	mov	r0, r5
	bl	BsGet
	movw	r1, #20036
	movw	r2, #20553
	movt	r1, 17221
	movt	r2, 18515
	cmp	r0, r1
	cmpeq	r7, r2
	bne	.L3612
	ldr	r5, .L3627
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3518
	mov	r3, r6
	mov	r2, r6
	mov	r1, #112
	ldr	r0, [r4, #244]
	blx	ip
.L3518:
	mov	r0, r4
	bl	HEVC_DecVDM
	cmp	r0, #0
	beq	.L3612
.L3620:
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L3615
	b	.L3613
.L3495:
	ldr	r1, .L3627+16
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r1, [r4, #2332]
	ldr	r3, [r1, #16]
	cmp	r3, #63
	bhi	.L3521
	cmp	r1, #0
	bne	.L3614
	b	.L3612
.L3521:
	ldr	r5, .L3627
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3522
	mov	r3, #0
	mov	r1, #102
	mov	r2, r3
	ldr	r0, [r4, #244]
	blx	ip
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	beq	.L3523
.L3524:
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3523:
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3612
	mov	r3, #0
	ldr	r0, [r4, #244]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L3612
.L3522:
	cmp	r1, #0
	bne	.L3524
	b	.L3612
.L3626:
	ldr	r6, .L3627
	mov	r0, #1
	ldr	r1, .L3627+20
	bl	dprint_vfmw
	ldr	ip, [r6]
	cmp	ip, #0
	bne	.L3617
	b	.L3613
.L3625:
	ldr	r5, .L3627
	strb	r0, [r4, #7]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3520
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #244]
	blx	ip
.L3520:
	mov	r0, r4
	bl	HEVC_DecVDM
	cmp	r0, #0
	beq	.L3519
	b	.L3620
.L3628:
	.align	2
.L3627:
	.word	g_event_report
	.word	.LC455
	.word	.LC456
	.word	.LC457
	.word	.LC458
	.word	.LC454
	UNWIND(.fnend)
	.size	HEVC_DecOneNal, .-HEVC_DecOneNal
	.align	2
	.global	HEVC_UnMarkFrameStoreRef
	.type	HEVC_UnMarkFrameStoreRef, %function
HEVC_UnMarkFrameStoreRef:
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
	mov	r3, #0
	strb	r3, [r1, #1]
	mov	r2, r3
	ldr	r1, [r1, #32]
	ldr	r0, [r0, #244]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	FSP_SetRef
	UNWIND(.fnend)
	.size	HEVC_UnMarkFrameStoreRef, .-HEVC_UnMarkFrameStoreRef
	.align	2
	.global	HEVC_InquireSliceProperty
	.type	HEVC_InquireSliceProperty, %function
HEVC_InquireSliceProperty:
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
	ldr	ip, .L3645
	mov	r4, r0
	mov	r3, #0
	mov	r6, r1
	add	r0, r0, #54016
	str	r3, [r2]
	mov	r1, r3
	str	r3, [r6]
	mov	r5, r2
	ldr	r3, [ip, #48]
	movw	r2, #3732
	add	r0, r0, #248
	add	r7, r4, #53248
	blx	r3
	ldr	r3, [r4, #2332]
	mov	r0, r4
	ldr	r2, [r3, #16]
	str	r2, [r7, #1088]
	ldr	r3, [r3, #32]
	str	r3, [r7, #1084]
	bl	HEVC_DecSliceSegmentHeader
	cmp	r0, #0
	bne	.L3642
	ldr	r3, [r7, #2604]
	movw	r1, #2024
	movw	ip, #26164
	add	lr, r4, #24576
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r6]
	ldr	r3, [r7, #1092]
	ldr	r2, [r4, #2320]
	ldr	r0, [r4, #2316]
	ldr	r7, [lr, #2836]
	mla	r2, r1, r3, r2
	ldr	r6, [r4, #2324]
	ldr	r1, [r7, #236]
	ldr	r8, [r7, #72]
	ldr	r9, [r6, #832]
	ldr	r7, [r7, #76]
	ldr	r2, [r2, #32]
	mla	ip, ip, r2, r0
	ldr	r2, [ip, #236]
	ldr	r3, [ip, #72]
	subs	r1, r2, r1
	ldr	r0, [ip, #76]
	movne	r1, #1
	cmp	r9, #1
	beq	.L3643
.L3634:
	cmp	r3, r8
	moveq	r3, r1
	orrne	r3, r1, #1
	cmp	r0, r7
	moveq	r0, r3
	orrne	r0, r3, #1
	cmp	r0, #0
	movne	r0, #0
	movne	r3, #1
	strne	r3, [r5]
.L3632:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3642:
	ldr	r1, .L3645+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3645+8
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r0, #0
	beq	.L3632
	sub	r2, fp, #36
	ldr	r0, [r4, #244]
	mov	r3, #4
	mvn	r4, #0
	mov	r1, #100
	str	r4, [r2, #-4]!
	blx	ip
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3643:
	ldr	r2, [ip, #108]
	ldr	ip, [ip, #100]
	cmp	r2, ip
	movcc	r2, ip
	str	r2, [r6, #884]
	ldr	ip, [lr, #2836]
	ldr	r4, [r4, #2324]
	ldr	r2, [ip, #108]
	ldr	ip, [ip, #100]
	ldr	lr, [r4, #884]
	cmp	r2, ip
	movcc	r2, ip
	cmp	r2, #8
	movls	ip, #0
	movhi	ip, #1
	cmp	lr, #8
	movne	ip, #0
	cmp	ip, #0
	beq	.L3644
.L3635:
	mov	r2, #1
	str	r2, [r4, #888]
	str	r2, [r5]
	b	.L3634
.L3644:
	sub	r2, r2, #8
	cmp	lr, #8
	clz	r2, r2
	mov	r2, r2, lsr #5
	movle	r2, #0
	cmp	r2, #0
	beq	.L3634
	b	.L3635
.L3646:
	.align	2
.L3645:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC459
	.word	g_event_report
	UNWIND(.fnend)
	.size	HEVC_InquireSliceProperty, .-HEVC_InquireSliceProperty
	.align	2
	.global	HEVCDEC_DecodePacket
	.type	HEVCDEC_DecodePacket, %function
HEVCDEC_DecodePacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
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
	cmp	r1, #0
	mov	ip, #0
	mov	r4, r0
	str	ip, [fp, #-56]
	str	ip, [fp, #-52]
	str	ip, [fp, #-48]
	beq	.L3714
	ldrb	r3, [r0]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3715
	ldr	r2, [r0, #2324]
	ldrb	lr, [r0, #5]	@ zero_extendqisi2
	ldr	r3, [r2, #8]
	cmp	lr, #1
	mov	r3, r3, asl #16
	and	r3, r3, #458752
	str	r3, [r0, #232]
	ldr	r3, [r2]
	str	r3, [r0, #236]
	ldr	r3, [r2, #4]
	str	r3, [r0, #240]
	beq	.L3651
	ldr	r3, [r2, #832]
	cmp	r3, #1
	addne	r6, r0, #81920
	beq	.L3716
.L3652:
	mov	r0, r4
	bl	HEVC_ReceivePacket
	ldr	r3, [r4, #2332]
	str	r3, [r6, #3804]
	mov	r5, r0
.L3653:
	cmp	r5, #0
	mov	r3, #0
	strb	r3, [r4, #5]
	beq	.L3654
.L3673:
	mov	r0, #0
.L3711:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3716:
	ldr	r3, [r2, #872]
	add	r6, r0, #81920
	cmp	r3, #1
	bne	.L3652
	ldr	r3, [r6, #3800]
	cmp	r3, #1
	bne	.L3652
	ldr	r3, [r6, #3804]
	mov	r0, ip
	str	r3, [r4, #2332]
	str	ip, [r6, #3800]
	strb	ip, [r4, #5]
	b	.L3711
.L3654:
	ldr	r3, [r4, #2332]
	str	r5, [r3, #4]
	ldr	r3, [r4, #2332]
	ldr	r0, [r4, #2308]
	ldr	r2, [r3, #48]
	ldr	r1, [r3, #44]
	bl	BsInit
	mov	r1, #24
	ldr	r0, [r4, #2308]
	bl	BsSkip
	mov	r1, #1
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	mov	r1, #6
	str	r0, [r5, #24]
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	mov	r1, #6
	str	r0, [r5, #16]
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	mov	r1, #3
	str	r0, [r5, #28]
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	str	r0, [r5, #32]
	ldr	r5, [r4, #2332]
	ldr	r3, [r5, #32]
	cmp	r3, #0
	subne	r3, r3, #1
	strne	r3, [r5, #32]
	ldrne	r5, [r4, #2332]
	ldr	r0, [r5, #16]
	bl	HEVC_IsSliceUnit
	cmp	r0, #1
	mov	r6, r0
	beq	.L3717
.L3656:
	mov	r3, #0
	str	r3, [r5, #4]
	ldr	r3, [r4, #2332]
	add	r0, r4, #2464
	add	r0, r0, #8
	ldr	r1, [r3, #44]
	ldr	r2, [r3, #48]
	bl	BsInit
	mov	r1, #24
	ldr	r0, [r4, #2308]
	bl	BsSkip
	mov	r1, #1
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	mov	r1, #6
	str	r0, [r5, #24]
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	mov	r1, #6
	str	r0, [r5, #16]
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	mov	r1, #3
	str	r0, [r5, #28]
	ldr	r0, [r4, #2308]
	ldr	r5, [r4, #2332]
	bl	BsGet
	sub	r0, r0, #1
	str	r0, [r5, #32]
.L3672:
	mov	r0, r4
	bl	HEVC_DecOneNal
	cmn	r0, #2
	beq	.L3711
	cmp	r0, #0
	beq	.L3673
	ldr	r1, .L3722
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3711
.L3651:
	ldr	r3, [r0, #2332]
	cmp	r3, #0
	movne	r5, #0
	mvneq	r5, #0
	cmp	r3, #0
	bne	.L3653
	mov	r0, ip
	ldr	r1, .L3722+4
	bl	dprint_vfmw
	mov	r0, r5
	b	.L3711
.L3717:
	sub	r2, fp, #40
	sub	r1, fp, #44
	mov	r0, r4
	bl	HEVC_InquireSliceProperty
	subs	r3, r0, #0
	bne	.L3657
	ldr	r5, [fp, #-44]
	cmp	r5, #1
	beq	.L3718
.L3658:
	ldr	r5, [fp, #-40]
	cmp	r5, #1
	beq	.L3719
.L3670:
	ldr	r5, [r4, #2332]
	ldr	r0, [r5, #16]
	bl	HEVC_IsSliceUnit
	cmp	r0, #1
	beq	.L3672
	b	.L3656
.L3657:
	ldr	r1, .L3722+8
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r1, [r4, #2332]
	cmp	r1, #0
	beq	.L3671
	ldr	r0, [r4, #244]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2332]
.L3671:
	ldr	r3, .L3722+12
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r0, #0
	beq	.L3711
	mov	r3, #0
	ldr	r0, [r4, #244]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L3711
.L3715:
	mov	r0, ip
	ldr	r1, .L3722+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3711
.L3719:
	add	r3, r4, #53248
	ldr	r3, [r3, #1088]
	sub	r3, r3, #16
	cmp	r3, #4
	bls	.L3720
.L3663:
	ldrb	r5, [r4, #2]	@ zero_extendqisi2
	cmp	r5, #1
	beq	.L3721
.L3666:
	ldr	r0, [r4, #244]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L3670
	sub	r3, fp, #48
	ldr	r0, [r4, #244]
	sub	r2, fp, #52
	sub	r1, fp, #56
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r4, #2324]
	ldr	r0, [r3, #708]
	cmp	r0, #1
	bne	.L3670
	ldr	r8, [fp, #-48]
	cmp	r8, #0
	bgt	.L3668
	ldr	r3, [fp, #-52]
	cmp	r3, #32
	bgt	.L3668
	ldr	r7, [r4, #2328]
	cmp	r7, #0
	bne	.L3668
	ldr	r2, [fp, #-40]
	cmp	r2, #0
	beq	.L3670
	ldr	r3, [r4, #220]
	cmp	r3, #0
	beq	.L3670
	add	r6, r4, #24576
	add	r5, r4, #81920
	ldr	r3, [r6, #2836]
	ldr	r1, [r5, #3824]
	ldr	lr, [r3, #76]
	cmp	lr, r1
	beq	.L3670
	ldr	ip, [r3, #72]
	ldr	r3, [r5, #3828]
	subs	r9, ip, r3
	movne	r9, #1
	cmp	r8, #0
	movne	r9, #0
	cmp	r9, #0
	beq	.L3670
	stmia	sp, {r1, ip, lr}
	ldr	r1, .L3722+20
	bl	dprint_vfmw
	str	r7, [r5, #3820]
	ldr	r3, [r6, #2836]
	ldr	r1, .L3722+12
	ldr	r2, [r3, #76]
	ldr	ip, [r1]
	str	r2, [r5, #3824]
	cmp	ip, #0
	ldr	r3, [r3, #72]
	str	r3, [r5, #3828]
	beq	.L3670
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r4, #244]
	blx	ip
	b	.L3670
.L3718:
	ldr	r2, [r4, #208]
	cmp	r2, #0
	bne	.L3659
	add	r3, r4, #53248
	ldrb	r3, [r3, #337]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3658
	ldr	r2, .L3722+24
	ldr	r1, .L3722+28
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	b	.L3658
.L3659:
	ldr	ip, [r4, #2332]
	add	r0, r4, #2464
	mov	r2, #13
	add	r0, r0, #8
	ldr	r1, .L3722+32
	str	r3, [ip, #4]
	bl	BsInit
	mov	r1, #40
	ldr	r0, [r4, #2308]
	bl	BsSkip
	ldr	r3, [r4, #2332]
	mov	r2, #48
	mov	r0, r4
	str	r2, [r3, #16]
	strb	r5, [r4, #6]
	bl	HEVC_DecOneNal
	ldr	r3, [r4, #272]
	mvn	r0, #1
	strb	r5, [r4, #5]
	add	r3, r3, #1
	str	r3, [r4, #272]
	b	.L3711
.L3714:
	mov	r0, r1
	ldr	r1, .L3722+36
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3711
.L3720:
	mov	r0, r4
	bl	HEVC_FlushOutputFrmFromDPB
	cmp	r0, #0
	beq	.L3663
	mov	r0, r5
	ldr	r2, .L3722+40
	ldr	r1, .L3722+44
	bl	dprint_vfmw
	b	.L3663
.L3721:
	mov	r0, r4
	bl	HEVC_EmptyFrmFromDPB
	cmp	r0, #0
	beq	.L3666
	mov	r0, r5
	ldr	r2, .L3722+40
	ldr	r1, .L3722+48
	bl	dprint_vfmw
	b	.L3666
.L3668:
	add	r2, r4, #81920
	mov	r1, #0
	mov	r3, #1
	mvn	r0, #1
	str	r1, [r2, #3820]
	strb	r3, [r4, #5]
	b	.L3711
.L3723:
	.align	2
.L3722:
	.word	.LC466
	.word	.LC462
	.word	.LC465
	.word	g_event_report
	.word	.LC461
	.word	.LC464
	.word	.LANCHOR1+944
	.word	.LC463
	.word	.LANCHOR1+988
	.word	.LC460
	.word	.LANCHOR1+964
	.word	.LC437
	.word	.LC438
	UNWIND(.fnend)
	.size	HEVCDEC_DecodePacket, .-HEVCDEC_DecodePacket
	.align	2
	.global	HEVC_HaveSliceToDec
	.type	HEVC_HaveSliceToDec, %function
HEVC_HaveSliceToDec:
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
	ldr	r0, [r0, #208]
	cmp	r0, #0
	bne	.L3726
	add	r3, r4, #53248
	ldrb	r3, [r3, #337]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3728
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3726:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3728:
	ldr	r2, .L3729
	ldr	r1, .L3729+4
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3730:
	.align	2
.L3729:
	.word	.LANCHOR1+944
	.word	.LC463
	UNWIND(.fnend)
	.size	HEVC_HaveSliceToDec, .-HEVC_HaveSliceToDec
	.align	2
	.global	HEVC_GetPicStreamSize
	.type	HEVC_GetPicStreamSize, %function
HEVC_GetPicStreamSize:
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
	add	r0, r0, #61440
	ldr	r2, [r0, #3212]
	cmp	r2, #0
	beq	.L3734
	mov	r0, #0
.L3733:
	ldr	r3, [r2]
	ldr	r1, [r2, #4]
	ldr	r2, [r2, #1356]
	add	r3, r3, r1
	add	r3, r3, #7
	cmp	r2, #0
	add	r0, r0, r3, lsr #3
	bne	.L3733
	ldmfd	sp, {fp, sp, pc}
.L3734:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_GetPicStreamSize, .-HEVC_GetPicStreamSize
	.align	2
	.global	HEVCDEC_GetRemainImg
	.type	HEVCDEC_GetRemainImg, %function
HEVCDEC_GetRemainImg:
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
	ldr	r3, [r0, #2508]
	mov	r4, r0
	cmp	r3, #0
	addne	r8, r0, #61696
	addne	r8, r8, #32
	mvnne	r6, #0
	movne	r7, #1
	bne	.L3747
	b	.L3748
.L3739:
	mov	r0, r4
	bl	HEVC_RemoveApcStore
	ldr	r1, [fp, #-52]
	mov	r0, r4
	bl	HEVC_RemoveFrameStoreOutDPB
	ldr	r2, [r4, #2508]
	cmp	r2, #0
	beq	.L3756
.L3747:
	sub	r1, fp, #48
	sub	r2, fp, #52
	mov	r0, r4
	bl	HEVC_GetMinPOC
	ldr	r1, [fp, #-52]
	add	r3, r1, #632
	ldr	r5, [r4, r3, asl #2]
	cmp	r5, #0
	beq	.L3739
	ldrb	r3, [r5, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3739
	mov	r1, r5
	mov	r0, r4
	strb	r7, [r5, #2]
	bl	HEVC_GetImagePara
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_CheckFrameStore
	subs	r6, r0, #0
	beq	.L3757
.L3746:
	ldr	r1, [fp, #-52]
	b	.L3739
.L3756:
	cmp	r6, #0
	bne	.L3748
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3757:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_SetDisplay
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L3758
	ldr	r3, [r4, #2508]
	mov	r2, r4
	mov	r1, #16
	cmp	r3, #1
	streq	r3, [r9, #188]
	mov	r3, r8
	ldr	r0, [r4, #244]
	str	r9, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	mov	r10, r0
	beq	.L3743
	mov	r3, r0
	ldr	r2, .L3760
	ldr	r1, .L3760+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #244]
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r1, #1
	mov	r0, r4
	mvn	r10, #0
	bl	HEVC_ClearAll
.L3743:
	ldr	r3, [r4, #2324]
	ldr	r2, [r3, #604]
	add	r2, r2, #2032
	add	r2, r2, #15
	cmp	r2, #4096
	movcc	r3, #0
	strcc	r3, [r9, #24]
	bcs	.L3759
.L3745:
	ldr	r1, [r4, #2508]
	ldr	r3, [r4, #284]
	ldr	r2, [r4, #276]
	cmp	r1, #1
	add	r3, r3, #1
	str	r3, [r4, #284]
	add	r2, r2, #1
	str	r2, [r4, #276]
	beq	.L3746
	mov	r6, r10
	ldr	r1, [fp, #-52]
	b	.L3739
.L3748:
	add	r0, r4, #61696
	add	r0, r0, #32
	bl	GetVoLastImageID
	mov	r6, r0
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3759:
	mov	r1, r9
	mov	r0, r4
	bl	HEVC_SetFrmRepeatCount.part.3
	b	.L3745
.L3758:
	ldr	r1, .L3760+8
	mvn	r6, #0
	ldr	r2, .L3760
	bl	dprint_vfmw
	ldr	r1, [fp, #-52]
	b	.L3739
.L3761:
	.align	2
.L3760:
	.word	.LANCHOR1+1004
	.word	.LC468
	.word	.LC467
	UNWIND(.fnend)
	.size	HEVCDEC_GetRemainImg, .-HEVCDEC_GetRemainImg
	.align	2
	.global	HEVCDEC_GetImageWidth
	.type	HEVCDEC_GetImageWidth, %function
HEVCDEC_GetImageWidth:
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
	add	r0, r0, #24576
	ldr	r2, [r0, #2836]
	ldr	r3, [r2, #152]
	ldr	r2, [r2, #72]
	rsb	r0, r3, #0
	add	r3, r3, r2
	sub	r3, r3, #1
	and	r0, r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVCDEC_GetImageWidth, .-HEVCDEC_GetImageWidth
	.align	2
	.global	HEVCDEC_GetImageHeight
	.type	HEVCDEC_GetImageHeight, %function
HEVCDEC_GetImageHeight:
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
	add	r0, r0, #24576
	ldr	r2, [r0, #2836]
	ldr	r3, [r2, #152]
	ldr	r2, [r2, #76]
	rsb	r0, r3, #0
	add	r3, r3, r2
	sub	r3, r3, #1
	and	r0, r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVCDEC_GetImageHeight, .-HEVCDEC_GetImageHeight
	.global	UUID_ISO_IEC_11578
	.global	g_IsDeced
	.global	g_TsToRsMap
	.global	s_TmpParam
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	SarTable.14267, %object
	.size	SarTable.14267, 136
SarTable.14267:
	.word	1
	.word	1
	.word	1
	.word	1
	.word	12
	.word	11
	.word	10
	.word	11
	.word	16
	.word	11
	.word	40
	.word	33
	.word	24
	.word	11
	.word	20
	.word	11
	.word	32
	.word	11
	.word	80
	.word	33
	.word	18
	.word	11
	.word	15
	.word	11
	.word	64
	.word	33
	.word	160
	.word	99
	.word	4
	.word	3
	.word	3
	.word	2
	.word	2
	.word	1
	.type	__func__.14420, %object
	.size	__func__.14420, 18
__func__.14420:
	.ascii	"HEVC_CalcTileInfo\000"
	.space	2
.LC0:
	.word	32
	.word	70
	.word	175
	.word	403
	.word	-48
	.word	-120
	.word	475
.LC1:
	.word	33
	.word	58
	.word	203
	.word	377
	.word	-42
	.word	-146
	.word	482
	.type	UUID_ISO_IEC_11578, %object
	.size	UUID_ISO_IEC_11578, 16
UUID_ISO_IEC_11578:
	.byte	-15
	.byte	76
	.byte	60
	.byte	-64
	.byte	85
	.byte	-29
	.byte	17
	.byte	-28
	.byte	-119
	.byte	38
	.byte	0
	.byte	2
	.byte	-91
	.byte	-43
	.byte	-59
	.byte	27
	.type	__func__.14872, %object
	.size	__func__.14872, 27
__func__.14872:
	.ascii	"HEVC_GetUnRefPicWithMinPoc\000"
	.space	1
	.type	__func__.14881, %object
	.size	__func__.14881, 21
__func__.14881:
	.ascii	"HEVC_IncreaseDPBSize\000"
	.space	3
	.type	__func__.15112, %object
	.size	__func__.15112, 17
__func__.15112:
	.ascii	"HEVC_WritePicMsg\000"
	.space	3
	.type	__func__.15141, %object
	.size	__func__.15141, 16
__func__.15141:
	.ascii	"HEVC_CheckListX\000"
	.type	__func__.15265, %object
	.size	__func__.15265, 16
__func__.15265:
	.ascii	"HEVC_RepairList\000"
	.type	__func__.15635, %object
	.size	__func__.15635, 21
__func__.15635:
	.ascii	"HEVC_CheckFrameStore\000"
	.space	3
	.type	__func__.15481, %object
	.size	__func__.15481, 19
__func__.15481:
	.ascii	"HEVC_OutputCurrPic\000"
	.space	1
	.type	__func__.15486, %object
	.size	__func__.15486, 23
__func__.15486:
	.ascii	"HEVC_FirstFrameFastOut\000"
	.space	1
	.type	__func__.15781, %object
	.size	__func__.15781, 14
__func__.15781:
	.ascii	"HEVC_IsNewPic\000"
	.space	2
	.type	quant8_inter_default, %object
	.size	quant8_inter_default, 64
quant8_inter_default:
	.word	269488144
	.word	403968529
	.word	286265360
	.word	421008402
	.word	303108112
	.word	471406612
	.word	336728336
	.word	555489560
	.word	403968529
	.word	690035737
	.word	421008402
	.word	908665116
	.word	471406612
	.word	1194731809
	.word	555489560
	.word	1531393577
	.type	quant_ts_default, %object
	.size	quant_ts_default, 16
quant_ts_default:
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.type	__func__.16062, %object
	.size	__func__.16062, 16
__func__.16062:
	.ascii	"HEVC_ReleaseNAL\000"
.LC2:
	.word	1
	.word	2
	.word	2
	.word	1
.LC3:
	.word	1
	.word	2
	.word	1
	.word	1
	.type	g_CropUnitX, %object
	.size	g_CropUnitX, 16
g_CropUnitX:
	.word	1
	.word	2
	.word	2
	.word	1
	.type	g_CropUnitY, %object
	.size	g_CropUnitY, 16
g_CropUnitY:
	.word	1
	.word	2
	.word	1
	.word	1
	.type	__func__.14963, %object
	.size	__func__.14963, 27
__func__.14963:
	.ascii	"HEVC_DecSliceSegmentHeader\000"
	.space	1
	.type	__func__.15344, %object
	.size	__func__.15344, 13
__func__.15344:
	.ascii	"HEVC_InitDPB\000"
	.space	3
	.type	__func__.16114, %object
	.size	__func__.16114, 13
__func__.16114:
	.ascii	"HEVCDEC_Init\000"
	.space	3
	.type	__func__.15627, %object
	.size	__func__.15627, 19
__func__.15627:
	.ascii	"HEVC_OutputFrmToVO\000"
	.space	1
	.type	__func__.15594, %object
	.size	__func__.15594, 22
__func__.15594:
	.ascii	"HEVC_OutputFrmFromDPB\000"
	.space	2
	.type	__func__.14887, %object
	.size	__func__.14887, 30
__func__.14887:
	.ascii	"HEVC_OutputUnRefPicWithMinPoc\000"
	.space	2
	.type	__func__.15607, %object
	.size	__func__.15607, 27
__func__.15607:
	.ascii	"HEVC_FlushOutputFrmFromDPB\000"
	.space	1
	.type	__func__.16102, %object
	.size	__func__.16102, 23
__func__.16102:
	.ascii	"HEVCDEC_GetImageBuffer\000"
	.space	1
	.type	__func__.16153, %object
	.size	__func__.16153, 14
__func__.16153:
	.ascii	"HEVC_ClearDPB\000"
	.space	2
	.type	__func__.14892, %object
	.size	__func__.14892, 23
__func__.14892:
	.ascii	"HEVC_RecoverDPBProcess\000"
	.space	1
	.type	__func__.15574, %object
	.size	__func__.15574, 20
__func__.15574:
	.ascii	"HEVC_InsertFrmInDPB\000"
	.type	__func__.15491, %object
	.size	__func__.15491, 22
__func__.15491:
	.ascii	"HEVC_SimpleDPBProcess\000"
	.space	2
	.type	__func__.15496, %object
	.size	__func__.15496, 21
__func__.15496:
	.ascii	"HEVC_DecOrderProcess\000"
	.space	3
	.type	__func__.15501, %object
	.size	__func__.15501, 22
__func__.15501:
	.ascii	"HEVC_DispOrderProcess\000"
	.space	2
	.type	__func__.15517, %object
	.size	__func__.15517, 19
__func__.15517:
	.ascii	"HEVC_StorePicInDPB\000"
	.space	1
	.type	__func__.14919, %object
	.size	__func__.14919, 14
__func__.14919:
	.ascii	"HEVC_DecSlice\000"
	.space	2
	.type	__func__.15218, %object
	.size	__func__.15218, 12
__func__.15218:
	.ascii	"HEVC_DecVDM\000"
	.type	__func__.16215, %object
	.size	__func__.16215, 20
__func__.16215:
	.ascii	"HEVC_HaveSliceToDec\000"
	.type	__func__.14367, %object
	.size	__func__.14367, 21
__func__.14367:
	.ascii	"HEVCDEC_DecodePacket\000"
	.space	3
	.type	g_HEVCNalTypeEOPIC, %object
	.size	g_HEVCNalTypeEOPIC, 13
g_HEVCNalTypeEOPIC:
	.byte	0
	.byte	0
	.byte	1
	.byte	96
	.byte	0
	.byte	72
	.byte	83
	.byte	80
	.byte	73
	.byte	67
	.byte	69
	.byte	78
	.byte	68
	.space	3
	.type	__func__.16232, %object
	.size	__func__.16232, 21
__func__.16232:
	.ascii	"HEVCDEC_GetRemainImg\000"
	.data
	.align	2
.LANCHOR4 = . + 0
	.type	quant8_intra_default, %object
	.size	quant8_intra_default, 64
quant8_intra_default:
	.word	269488144
	.word	404034065
	.word	269488144
	.word	420877073
	.word	303108112
	.word	488183316
	.word	353505296
	.word	606018328
	.word	403968273
	.word	791225118
	.word	454431506
	.word	1094069283
	.word	521737749
	.word	1480996393
	.word	605886744
	.word	1935163695
	.type	g_quantTSDefault4x4, %object
	.size	g_quantTSDefault4x4, 16
g_quantTSDefault4x4:
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.type	g_quantInterDefault8x8, %object
	.size	g_quantInterDefault8x8, 64
g_quantInterDefault8x8:
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	17
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	18
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	54
	.byte	20
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	54
	.byte	71
	.byte	24
	.byte	25
	.byte	28
	.byte	33
	.byte	41
	.byte	54
	.byte	71
	.byte	91
	.type	g_quantIntraDefault8x8, %object
	.size	g_quantIntraDefault8x8, 64
g_quantIntraDefault8x8:
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	21
	.byte	24
	.byte	16
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	19
	.byte	22
	.byte	25
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	20
	.byte	22
	.byte	25
	.byte	29
	.byte	16
	.byte	16
	.byte	18
	.byte	21
	.byte	24
	.byte	27
	.byte	31
	.byte	36
	.byte	17
	.byte	17
	.byte	20
	.byte	24
	.byte	30
	.byte	35
	.byte	41
	.byte	47
	.byte	18
	.byte	19
	.byte	22
	.byte	27
	.byte	35
	.byte	44
	.byte	54
	.byte	65
	.byte	21
	.byte	22
	.byte	25
	.byte	31
	.byte	41
	.byte	54
	.byte	70
	.byte	88
	.byte	24
	.byte	25
	.byte	29
	.byte	36
	.byte	47
	.byte	65
	.byte	88
	.byte	115
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC4:
	ASCII(.ascii	"dar=%d\012\000" )
.LC5:
	ASCII(.ascii	"--------------- PrintNaluArraySlot START ----------" )
	ASCII(.ascii	"-----\012\000" )
	.space	2
.LC6:
	ASCII(.ascii	"\012\000" )
	.space	2
.LC7:
	ASCII(.ascii	"--------------- PrintNaluArraySlot END ------------" )
	ASCII(.ascii	"---\012\000" )
.LC8:
	ASCII(.ascii	"NalArray[%d]=%p,is_valid=%d   \000" )
	.space	1
.LC9:
	ASCII(.ascii	"--------------- PrintCurrSlicelistX START ---------" )
	ASCII(.ascii	"------\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"List0, size: %d\012\000" )
	.space	3
.LC11:
	ASCII(.ascii	"List1, size: %d\012\000" )
	.space	3
.LC12:
	ASCII(.ascii	"pListX[0][%d]:%p \000" )
	.space	2
.LC13:
	ASCII(.ascii	"pFrameStore:%p \000" )
.LC14:
	ASCII(.ascii	"pLogicFs:%p \000" )
	.space	3
.LC15:
	ASCII(.ascii	"--------------- PrintCurrSlicelistX END -----------" )
	ASCII(.ascii	"----\012\000" )
	.space	3
.LC16:
	ASCII(.ascii	"pListX[1][%d]: %p\000" )
	.space	2
.LC17:
	ASCII(.ascii	"--------------- PrintFrameStoreState START --------" )
	ASCII(.ascii	"-------\012\000" )
.LC18:
	ASCII(.ascii	"--------------- PrintFrameStoreState END ----------" )
	ASCII(.ascii	"-----\012\000" )
	.space	2
.LC19:
	ASCII(.ascii	"Fs=%d, pFs=%p, state=%d, is_reference=%d, is_displa" )
	ASCII(.ascii	"yed=%d \000" )
	.space	1
.LC20:
	ASCII(.ascii	"p_usrdat[%d]=%p \000" )
	.space	3
.LC21:
	ASCII(.ascii	"--------------- PrintAPCState START ---------------" )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC22:
	ASCII(.ascii	"APC.used_size:%d, APC.size:%d\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"%d: is_used=%d, idc=%d, poc=%d\012\000" )
.LC24:
	ASCII(.ascii	"--------------- PrintAPCState END ---------------\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC25:
	ASCII(.ascii	"--------------- PrintDPBState START ---------------" )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC26:
	ASCII(.ascii	"DPB: size:%d, used_size:%d, negative:%d, positive:%" )
	ASCII(.ascii	"d, longterm:%d\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"fs[%d]:%p non_existing:%d state:%d is_ref:%d displa" )
	ASCII(.ascii	"yed:%d poc:%d long_term:%d LogicFs:%p, fsp_ref:%d f" )
	ASCII(.ascii	"sp_disp:%d logic_id:%d img_id:%d\012\000" )
.LC28:
	ASCII(.ascii	"--------------- PrintDPBState END ---------------\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC29:
	ASCII(.ascii	"fs[%d]\011\000" )
.LC30:
	ASCII(.ascii	"LogicFs:%p\011\000" )
.LC31:
	ASCII(.ascii	"LogicTick:%d\011\000" )
	.space	2
.LC32:
	ASCII(.ascii	"IsRef:%d\011\000" )
	.space	2
.LC33:
	ASCII(.ascii	"s32DispState:%d\011\000" )
	.space	3
.LC34:
	ASCII(.ascii	"LogicFs:Null\011\000" )
	.space	2
.LC35:
	ASCII(.ascii	"DecFs:%p\011\000" )
	.space	2
.LC36:
	ASCII(.ascii	"DecTick:%d\011\000" )
.LC37:
	ASCII(.ascii	"DecFs:Null\011\000" )
.LC38:
	ASCII(.ascii	"DispFs:%p\011\000" )
	.space	1
.LC39:
	ASCII(.ascii	"DispTick:%d\011\000" )
	.space	3
.LC40:
	ASCII(.ascii	"DispFs:Null\011\000" )
	.space	3
.LC41:
	ASCII(.ascii	"%s num_tile_columns(%d) out of range(0, %d].\012\000" )
	.space	2
.LC42:
	ASCII(.ascii	"%s num_tile_rows(%d) out of range(0, %d].\012\000" )
	.space	1
.LC43:
	ASCII(.ascii	"%s 0 idx(%d) out of range(0, 512).\012\000" )
.LC44:
	ASCII(.ascii	"%s 1 idx(%d) out of range(0, 512).\012\000" )
.LC45:
	ASCII(.ascii	"firmware/common/syntax/hevc.c\000" )
	.space	2
.LC46:
	ASCII(.ascii	"NULL pointer: %s, L%d\012\000" )
	.space	1
.LC47:
	ASCII(.ascii	"pListX[0][%d] = NULL, listXsize[0] = %d\012\000" )
	.space	3
.LC48:
	ASCII(.ascii	"pListX[0][%d]->frame_store = NULL, listXsize[0] = %" )
	ASCII(.ascii	"d\012\000" )
	.space	2
.LC49:
	ASCII(.ascii	"pListX[0][%d] frame_store(%p), pstLogicFs(%p) inval" )
	ASCII(.ascii	"id\012\000" )
	.space	1
.LC50:
	ASCII(.ascii	"pListX[1][%d] = NULL, listXsize[1] = %d\012\000" )
	.space	3
.LC51:
	ASCII(.ascii	"pListX[1][%d]->frame_store = NULL, listXsize[1] = %" )
	ASCII(.ascii	"d\012\000" )
	.space	2
.LC52:
	ASCII(.ascii	"pListX[i][%d] pstLogicFs(%p) invalid\012\000" )
	.space	2
.LC53:
	ASCII(.ascii	"%s DPB.used_size(%d) >= 17.\012\000" )
	.space	3
.LC54:
	ASCII(.ascii	"%s can't find such pic.\012\000" )
	.space	3
.LC55:
	ASCII(.ascii	"%s find pos=%d, poc=%d.\012\000" )
	.space	3
.LC56:
	ASCII(.ascii	"%s DPB.size(%d) already reached max(%d).\012\000" )
	.space	2
.LC57:
	ASCII(.ascii	"%s increase DPB size to %d\012\000" )
.LC58:
	ASCII(.ascii	"%s no enough fs(total_fs_num=%d) to increase DPB si" )
	ASCII(.ascii	"ze.\012\000" )
.LC59:
	ASCII(.ascii	"TileWidthInCtb is 0\012\000" )
	.space	3
.LC60:
	ASCII(.ascii	"image_id=%d, pmv_idc=%d\012\000" )
	.space	3
.LC61:
	ASCII(.ascii	"pic msg burst0:\012\000" )
	.space	3
.LC62:
	ASCII(.ascii	"%s call HEVC_CalcTileInfo failed.\012\000" )
	.space	1
.LC63:
	ASCII(.ascii	"logic frame id(=%d): get LogicFs error!\012\000" )
	.space	3
.LC64:
	ASCII(.ascii	"phy fs is null: pstDecodeFs = %p, pstDispOutFs = %p" )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC65:
	ASCII(.ascii	"%s ERROR: bHaveIndependentSlice = 0\012\012\000" )
	.space	2
.LC66:
	ASCII(.ascii	"ListX ERROR: list0size >= %d\012\000" )
	.space	2
.LC67:
	ASCII(.ascii	"ListX ERROR: list1size >= %d\012\000" )
	.space	2
.LC68:
	ASCII(.ascii	"pListX[0][%d] = NULL\012\000" )
	.space	2
.LC69:
	ASCII(.ascii	"pListX[0][%d]->frame_store = NULL\012\000" )
	.space	1
.LC70:
	ASCII(.ascii	"pListX[1][%d] = NULL\012\000" )
	.space	2
.LC71:
	ASCII(.ascii	"pListX[1][%d]->frame_store = NULL\012\000" )
	.space	1
.LC72:
	ASCII(.ascii	"NumSlicePara(%d) >= MaxSliceNum(%d)\012\000" )
	.space	3
.LC73:
	ASCII(.ascii	"line:%d pic_parameter_set_id(%d) out of range(0,%d)" )
	ASCII(.ascii	".\012\000" )
	.space	2
.LC74:
	ASCII(.ascii	"line:%d seq_parameter_set_id(%d) out of range[0,%d]" )
	ASCII(.ascii	".\012\000" )
	.space	2
.LC75:
	ASCII(.ascii	"line:%d, i(%d) > Max(%d) num_of_pics %d\012\000" )
	.space	3
.LC76:
	ASCII(.ascii	"line:%d, i(%d) > Max(%d)(neg%d + pos%d)\012\000" )
	.space	3
.LC77:
	ASCII(.ascii	"%s unkown slice type(%d).\012\000" )
	.space	1
.LC78:
	ASCII(.ascii	"%s pListX[%d] no available pic to repair.\012\000" )
	.space	1
.LC79:
	ASCII(.ascii	"%s: pListX[%d][%d] = NULL, repair it with pos(%d).\012" )
	ASCII(.ascii	"\000" )
.LC80:
	ASCII(.ascii	"HEVC_InitListX error.\012\000" )
	.space	1
.LC81:
	ASCII(.ascii	"HEVC_RepairList error.\012\000" )
.LC82:
	ASCII(.ascii	"release(apc=%d, pmvIdc=%d)\011\000" )
.LC83:
	ASCII(.ascii	"framestore %d allocate apc %d\012\000" )
	.space	1
.LC84:
	ASCII(.ascii	"find APC, but logic_fs_id %d abnormal(poc=%d, ref=%" )
	ASCII(.ascii	"d, logic_fs=%p, dec_fs=%p)\012\000" )
	.space	1
.LC85:
	ASCII(.ascii	"%s frame logic_fs_id=%d, state=%d\012\000" )
	.space	1
.LC86:
	ASCII(.ascii	"LogicFs(fs=%p, poc=%d, FirstFrameState=%d) is null," )
	ASCII(.ascii	" logic_fs_id = %d\012\000" )
	.space	2
.LC87:
	ASCII(.ascii	"%s frame poc=%d, is already output\012\000" )
.LC88:
	ASCII(.ascii	"%s frame(%p) invalid, state:%d, non_existing:%d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC89:
	ASCII(.ascii	"%s: pImg is NULL!\012\000" )
	.space	1
.LC90:
	ASCII(.ascii	"%s, REPORT_IFRAME_ERR\012\000" )
	.space	1
.LC91:
	ASCII(.ascii	"%s: InsertImgToVoQueue failed!\012\000" )
.LC92:
	ASCII(.ascii	"%s: ImgErrorLevel = %d, OutErrThr:%d, discarded!\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC93:
	ASCII(.ascii	"%s output current picture failed\012\000" )
	.space	2
.LC94:
	ASCII(.ascii	"HEVC_DECRecycleImage ERROR, DispState = FS_DISP_STA" )
	ASCII(.ascii	"TE_NOT_USED\012\000" )
.LC95:
	ASCII(.ascii	"hevc partition fs memory fail!\012\000" )
.LC96:
	ASCII(.ascii	"ERROR: HEVC FSP_ConfigInstance fail!\012\000" )
	.space	2
.LC97:
	ASCII(.ascii	"FS_ALLOC_ERR, ClearAll\012\000" )
.LC98:
	ASCII(.ascii	"Can not new logic fs! ClearAll\012\000" )
.LC99:
	ASCII(.ascii	"FSP_GetFspType -1\012\000" )
	.space	1
.LC100:
	ASCII(.ascii	"FSP_GetLogicFs return NULL\012\000" )
.LC101:
	ASCII(.ascii	"%s: pHevcCtx->DPB.used_size(%d) > DPB.size(%d), err" )
	ASCII(.ascii	"or resilence.\012\000" )
	.space	2
.LC102:
	ASCII(.ascii	"nal_segment = %d is not expected value\012\000" )
.LC103:
	ASCII(.ascii	"HEVC_VpsSpsPpsCheck pic_parameter_set_id(%d) out of" )
	ASCII(.ascii	" range\012\000" )
	.space	1
.LC104:
	ASCII(.ascii	"pps with this pic_parameter_set_id = %d havn't be d" )
	ASCII(.ascii	"ecoded\012\000" )
	.space	1
.LC105:
	ASCII(.ascii	"HEVC_VpsSpsPpsCheck seq_parameter_set_id(%d) out of" )
	ASCII(.ascii	" range\012\000" )
	.space	1
.LC106:
	ASCII(.ascii	"sps with this seq_parameter_set_id = %d havn't be d" )
	ASCII(.ascii	"ecoded\012\000" )
	.space	1
.LC107:
	ASCII(.ascii	"HEVC_VpsSpsPpsCheck video_parameter_set_id out of r" )
	ASCII(.ascii	"ange\012\000" )
	.space	3
.LC108:
	ASCII(.ascii	"vps with this video_parameter_set_id = %d havn't be" )
	ASCII(.ascii	" decoded\012\000" )
	.space	3
.LC109:
	ASCII(.ascii	"%s, %d, CurrSlice.poc = %d, PocRandomAccess = %d, n" )
	ASCII(.ascii	"al_unit_type = %d, should skip this frame\012\000" )
	.space	2
.LC110:
	ASCII(.ascii	"%s, %d, CurrSlice.poc = %d, bNoRaslOutputFlag = %d," )
	ASCII(.ascii	" nal_unit_type = %d, should skip this frame\012\000" )
.LC111:
	ASCII(.ascii	"%s, %d, PrevRAPIsBLA = %d, CurrSlice.poc = %d, PocC" )
	ASCII(.ascii	"RA = %d, nal_unit_type = %d, should skip this frame" )
	ASCII(.ascii	"\012\000" )
.LC112:
	ASCII(.ascii	"HEVC_ReadByteAlignment: code should not be zero.\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC113:
	ASCII(.ascii	"HEVC_ReadByteAlignment: code:%d\012\000" )
	.space	3
.LC114:
	ASCII(.ascii	"general_profile_compatibility_flag[][%d]\000" )
	.space	3
.LC115:
	ASCII(.ascii	"reserved_zero_2bits not equal 0.\012\000" )
	.space	2
.LC116:
	ASCII(.ascii	"HEVC_GetScalingListDefaultAddress NULL.\012\000" )
	.space	3
.LC117:
	ASCII(.ascii	"Error, BsLen: 0x%x, TotalPos: 0x%x\012\000" )
.LC118:
	ASCII(.ascii	"Panic: All zero data sequence in RBSP \012\000" )
.LC119:
	ASCII(.ascii	"nal_trail_zero_bit_num err\012\000" )
.LC120:
	ASCII(.ascii	"%s release streambuff=%p, bitstream_length=%d\012\000" )
	.space	1
.LC121:
	ASCII(.ascii	"HEVC_CombinePacket FATAL: pCurrNal=NULL!\012\000" )
	.space	2
.LC122:
	ASCII(.ascii	"END of the bit buffer, copy the first packet!\012\000" )
	.space	1
.LC123:
	ASCII(.ascii	"HEVC_ue_v ERROR: LeadingZeros (%d) >= 32, return 0x" )
	ASCII(.ascii	"%x\012\000" )
	.space	1
.LC124:
	ASCII(.ascii	"tone_map_id\000" )
.LC125:
	ASCII(.ascii	"Unsupport: target_bit_depth(%d) > 8\012\000" )
	.space	3
.LC126:
	ASCII(.ascii	"pTon->num_pivots > 256\012\000" )
.LC127:
	ASCII(.ascii	"frame_packing_arrangement_id\000" )
	.space	3
.LC128:
	ASCII(.ascii	"frame_packing_arrangement_type(%d) out of range\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC129:
	ASCII(.ascii	"decoding_unit_idx\000" )
	.space	2
.LC130:
	ASCII(.ascii	"num_sps_ids_minus1\000" )
	.space	1
.LC131:
	ASCII(.ascii	"pAct->num_sps_ids_minus1 > 15\012\000" )
	.space	1
.LC132:
	ASCII(.ascii	"active_seq_parameter_set_id\000" )
.LC133:
	ASCII(.ascii	"bp_seq_parameter_set_id\000" )
.LC134:
	ASCII(.ascii	"Wrong bp_seq_parameter_set_id = %d, should be 0 to " )
	ASCII(.ascii	"15!\012\000" )
.LC135:
	ASCII(.ascii	"HEVC_Sei_Buf_Period cpb_cnt_minus1(%d) out of range" )
	ASCII(.ascii	"(0,31).\012\000" )
.LC136:
	ASCII(.ascii	"num_decoding_units_minus1\000" )
	.space	2
.LC137:
	ASCII(.ascii	"pPic->num_decoding_units_minus1 > 255\012\000" )
	.space	1
.LC138:
	ASCII(.ascii	"num_nalus_in_du_minus1\000" )
	.space	1
.LC139:
	ASCII(.ascii	"delta_idx_minus1\000" )
	.space	3
.LC140:
	ASCII(.ascii	"delta_idx(%d) > idx(%d).\012\000" )
	.space	2
.LC141:
	ASCII(.ascii	"rIdx(%d) > (idx-1)(%d) or < 0).\012\000" )
	.space	3
.LC142:
	ASCII(.ascii	"abs_delta_rps_minus1\000" )
	.space	3
.LC143:
	ASCII(.ascii	"pTempRPset->num_of_pics(%d) out of range(0,15).\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC144:
	ASCII(.ascii	"num_of_pics out of range(0,15).\012\000" )
	.space	3
.LC145:
	ASCII(.ascii	"num_negative_pics\000" )
	.space	2
.LC146:
	ASCII(.ascii	"pTempRPset->num_negative_pics(%d) out of range(0,15" )
	ASCII(.ascii	").\012\000" )
	.space	1
.LC147:
	ASCII(.ascii	"num_positive_pics\000" )
	.space	2
.LC148:
	ASCII(.ascii	"pTempRPset->num_positive_pics(%d) out of range(0,15" )
	ASCII(.ascii	").\012\000" )
	.space	1
.LC149:
	ASCII(.ascii	"pShortTermRpset->num_of_pics(%d) out of range[0,%d]" )
	ASCII(.ascii	".\012\000" )
	.space	2
.LC150:
	ASCII(.ascii	"delta_poc_s0_minus1\000" )
.LC151:
	ASCII(.ascii	"delta_poc_s0_minus1(%d) out of range.\012\000" )
	.space	1
.LC152:
	ASCII(.ascii	"delta_poc_s1_minus1\000" )
.LC153:
	ASCII(.ascii	"elemental_duration_in_tc_minus1\000" )
.LC154:
	ASCII(.ascii	"cpb_cnt_minus1\000" )
	.space	1
.LC155:
	ASCII(.ascii	"cpb_cnt_minus1(%d) out of range(0,31).\012\000" )
.LC156:
	ASCII(.ascii	"bit_rate_value_minus1\000" )
	.space	2
.LC157:
	ASCII(.ascii	"cpb_size_value_minus1\000" )
	.space	2
.LC158:
	ASCII(.ascii	"cpb_size_du_value_minus1\000" )
	.space	3
.LC159:
	ASCII(.ascii	"bit_rate_du_value_minus1\000" )
	.space	3
.LC160:
	ASCII(.ascii	"vps_reserved_three_2bits(%d) not equal 0x3.\012\000" )
	.space	3
.LC161:
	ASCII(.ascii	"vps_max_layers_minus1 out of range(0,63).\012\000" )
	.space	1
.LC162:
	ASCII(.ascii	"vps_max_sub_layers_minus1=%d\012\000" )
	.space	2
.LC163:
	ASCII(.ascii	"vps_reserved_0xffff_16bits not equal 0xffff.\012\000" )
	.space	2
.LC164:
	ASCII(.ascii	"VPS HEVC_DecPTL error.\012\000" )
.LC165:
	ASCII(.ascii	"vps_num_layer_sets_minus1\000" )
	.space	2
.LC166:
	ASCII(.ascii	"vps_max_dec_pic_buffering_minus1[i]\000" )
.LC167:
	ASCII(.ascii	"vps_num_reorder_pics[i]\000" )
.LC168:
	ASCII(.ascii	"vps_max_latency_increase_plus1[i]\000" )
	.space	2
.LC169:
	ASCII(.ascii	"vps_num_layer_sets_minus1(%d) out of range(0,%d).\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC170:
	ASCII(.ascii	"layer_id_included_flag[%d][%d]\000" )
	.space	1
.LC171:
	ASCII(.ascii	"vps_num_ticks_poc_diff_one_minus1\000" )
	.space	2
.LC172:
	ASCII(.ascii	"vps_num_hrd_parameters\000" )
	.space	1
.LC173:
	ASCII(.ascii	"vps_num_hrd_parameters(%d) out of range(0,%d).\012\000" )
.LC174:
	ASCII(.ascii	"hrd_layer_set_idx\000" )
	.space	2
.LC175:
	ASCII(.ascii	"VPS HEVC_DecHrdParam error.\012\000" )
	.space	3
.LC176:
	ASCII(.ascii	"pVPS->vps_video_parameter_set_id out of range(0,15)" )
	ASCII(.ascii	".\012\000" )
	.space	2
.LC177:
	ASCII(.ascii	"VPS[%d] decode error0.\012\000" )
.LC178:
	ASCII(.ascii	"VPS[%d] decode error1.\012\000" )
.LC179:
	ASCII(.ascii	"chroma_sample_loc_type_top_field\000" )
	.space	3
.LC180:
	ASCII(.ascii	"chroma_sample_loc_type_bottom_field\000" )
.LC181:
	ASCII(.ascii	"def_disp_win_left_offset\000" )
	.space	3
.LC182:
	ASCII(.ascii	"VUI pSps->chroma_format_idc out of range(0,3).\012\000" )
.LC183:
	ASCII(.ascii	"def_disp_win_right_offset\000" )
	.space	2
.LC184:
	ASCII(.ascii	"def_disp_win_top_offset\000" )
.LC185:
	ASCII(.ascii	"def_disp_win_bottom_offset\000" )
	.space	1
.LC186:
	ASCII(.ascii	"vui_num_ticks_poc_diff_one_minus1\000" )
	.space	2
.LC187:
	ASCII(.ascii	"VUI HEVC_DecHrdParam error.\012\000" )
	.space	3
.LC188:
	ASCII(.ascii	"min_spatial_segmentation_idc\000" )
	.space	3
.LC189:
	ASCII(.ascii	"VUI min_spatial_segmentation_idc out of range(0,409" )
	ASCII(.ascii	"5).\012\000" )
.LC190:
	ASCII(.ascii	"max_bytes_per_pic_denom\000" )
.LC191:
	ASCII(.ascii	"max_bits_per_min_cu_denom\000" )
	.space	2
.LC192:
	ASCII(.ascii	"log2_max_mv_length_horizontal\000" )
	.space	2
.LC193:
	ASCII(.ascii	"log2_max_mv_length_vertical\000" )
.LC194:
	ASCII(.ascii	"sop_seq_parameter_set_id\000" )
	.space	3
.LC195:
	ASCII(.ascii	"num_entries_in_sop_minus1\000" )
	.space	2
.LC196:
	ASCII(.ascii	"num_entries_in_sop_minus1 out of range(0,1024)\012\000" )
.LC197:
	ASCII(.ascii	"sop_short_term_rps_idx\000" )
	.space	1
.LC198:
	ASCII(.ascii	"sop_poc_delta\000" )
	.space	2
.LC199:
	ASCII(.ascii	"recovery_poc_cnt\000" )
	.space	3
.LC200:
	ASCII(.ascii	"reserved_payload_extension_data\000" )
.LC201:
	ASCII(.ascii	"line:%d Cosume TotalPos(%d > %d)!!!\012\000" )
	.space	3
.LC202:
	ASCII(.ascii	"HEVC_SeiMessage HEVC_Sei_Payload error.\012\000" )
	.space	3
.LC203:
	ASCII(.ascii	"SEI HEVC_SeiMessage error.\012\000" )
.LC204:
	ASCII(.ascii	"SEI BsIsByteAligned error.\012\000" )
.LC205:
	ASCII(.ascii	"SEI rbsp_trailing_bits(%d) not equal 0x80.\012\000" )
.LC206:
	ASCII(.ascii	"nesting_num_ops_minus1\000" )
	.space	1
.LC207:
	ASCII(.ascii	"nesting_num_ops_minus1 out of range(0,%d)\012\000" )
	.space	1
.LC208:
	ASCII(.ascii	"nesting_op_idx\000" )
	.space	1
.LC209:
	ASCII(.ascii	"nesting_num_layers_minus1\000" )
	.space	2
.LC210:
	ASCII(.ascii	"nesting_num_layers_minus1 out of range(0,63)\012\000" )
	.space	2
.LC211:
	ASCII(.ascii	"scaling_list_pred_matrix_id_delta\000" )
	.space	2
.LC212:
	ASCII(.ascii	"scaling_list_pred_matrix_id_delta out of range(0,ma" )
	ASCII(.ascii	"trixId).\012\000" )
	.space	3
.LC213:
	ASCII(.ascii	"HEVC_DecScalingListData pMatrixAddress NULL.\012\000" )
	.space	2
.LC214:
	ASCII(.ascii	"scaling_list_dc_coef_minus8\000" )
.LC215:
	ASCII(.ascii	"scaling_list_dc_coef_minus8 out of range(-7,247).\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC216:
	ASCII(.ascii	"scaling_list_delta_coef\000" )
.LC217:
	ASCII(.ascii	"scaling_list_dc_coef_minus8 out of range(-128,127)." )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC218:
	ASCII(.ascii	"chroma_format_idc\000" )
	.space	2
.LC219:
	ASCII(.ascii	"pSPS->chroma_format_idc out of range(0,3).\012\000" )
.LC220:
	ASCII(.ascii	"pSPS->chroma_format_idc not equal(%d) 1.\012\000" )
	.space	2
.LC221:
	ASCII(.ascii	"pic_width_in_luma_samples\000" )
	.space	2
.LC222:
	ASCII(.ascii	"pic_width_in_luma_samples out of range(0,8192).\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC223:
	ASCII(.ascii	"pic_height_in_luma_samples\000" )
	.space	1
.LC224:
	ASCII(.ascii	"pic_height_in_luma_samples out of range(0,4096).\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC225:
	ASCII(.ascii	"ERROR Unkown level: %d\012\000" )
.LC226:
	ASCII(.ascii	"conf_win_left_offset\000" )
	.space	3
.LC227:
	ASCII(.ascii	"conf_win_right_offset\000" )
	.space	2
.LC228:
	ASCII(.ascii	"conf_win_left_offset+conf_win_right_offset out of r" )
	ASCII(.ascii	"ange.\012\000" )
	.space	2
.LC229:
	ASCII(.ascii	"conf_win_top_offset\000" )
.LC230:
	ASCII(.ascii	"conf_win_bottom_offset\000" )
	.space	1
.LC231:
	ASCII(.ascii	"conf_win_top_offset+conf_win_bottom_offset out of r" )
	ASCII(.ascii	"ange.\012\000" )
	.space	2
.LC232:
	ASCII(.ascii	"bit_depth_luma_minus8\000" )
	.space	2
.LC233:
	ASCII(.ascii	"bit_depth_luma(%d) out of range(8,14).\012\000" )
.LC234:
	ASCII(.ascii	"bit_depth_chroma_minus8\000" )
.LC235:
	ASCII(.ascii	"bit_depth_chroma(%d) out of range[8,14].\012\000" )
	.space	2
.LC236:
	ASCII(.ascii	"log2_max_pic_order_cnt_lsb_minus4\000" )
	.space	2
.LC237:
	ASCII(.ascii	"log2_max_pic_order_cnt_lsb_minus4 out of range[0,12" )
	ASCII(.ascii	"].\012\000" )
	.space	1
.LC238:
	ASCII(.ascii	"sps_max_dec_pic_buffering_minus1\000" )
	.space	3
.LC239:
	ASCII(.ascii	"sps_num_reorder_pics\000" )
	.space	3
.LC240:
	ASCII(.ascii	"sps_max_latency_increase_plus1\000" )
	.space	1
.LC241:
	ASCII(.ascii	"log2_min_coding_block_size_minus3\000" )
	.space	2
.LC242:
	ASCII(.ascii	"log2_min_luma_coding_block_size_minus3 out of range" )
	ASCII(.ascii	"(0,3).\012\000" )
	.space	1
.LC243:
	ASCII(.ascii	"log2_diff_max_min_coding_block_size\000" )
.LC244:
	ASCII(.ascii	"log2_diff_max_min_luma_coding_block_size out of ran" )
	ASCII(.ascii	"ge(0,3).\012\000" )
	.space	3
.LC245:
	ASCII(.ascii	"log2_ctb_sizeY out of range(4,6).\012\000" )
	.space	1
.LC246:
	ASCII(.ascii	"log2_min_transform_block_size_minus2\000" )
	.space	3
.LC247:
	ASCII(.ascii	"log2_min_transform_block_size_minus2 out of range(0" )
	ASCII(.ascii	",3).\012\000" )
	.space	3
.LC248:
	ASCII(.ascii	"log2_diff_max_min_transform_block_size\000" )
	.space	1
.LC249:
	ASCII(.ascii	"log2_diff_max_min_transform_block_size out of range" )
	ASCII(.ascii	"(0,3).\012\000" )
	.space	1
.LC250:
	ASCII(.ascii	"quadtree_tu_log2_min_size not less than log2_min_cb" )
	ASCII(.ascii	"_sizeY.\012\000" )
.LC251:
	ASCII(.ascii	"quadtree_tu_log2_max_size greater than Min( CtbLog2" )
	ASCII(.ascii	"SizeY, 5 ).\012\000" )
.LC252:
	ASCII(.ascii	"max_transform_hierarchy_depth_inter\000" )
.LC253:
	ASCII(.ascii	"max_transform_hierarchy_depth_inter out of range(0," )
	ASCII(.ascii	"CtbLog2SizeY-Log2MinTrafoSize).\012\000" )
.LC254:
	ASCII(.ascii	"max_transform_hierarchy_depth_intra\000" )
.LC255:
	ASCII(.ascii	"max_transform_hierarchy_depth_intra out of range(0," )
	ASCII(.ascii	"CtbLog2SizeY-Log2MinTrafoSize).\012\000" )
.LC256:
	ASCII(.ascii	"SPS HEVC_DecScalingListData error.\012\000" )
.LC257:
	ASCII(.ascii	"SPS sao is off and pic height(%d) is too small.(Log" )
	ASCII(.ascii	"ic Unsupported).\012\000" )
	.space	3
.LC258:
	ASCII(.ascii	"log2_min_pcm_luma_coding_block_size_minus3\000" )
	.space	1
.LC259:
	ASCII(.ascii	"log2_min_pcm_coding_block_size_minus3(%d) out of ra" )
	ASCII(.ascii	"nge[0,2].\012\000" )
	.space	2
.LC260:
	ASCII(.ascii	"log2_diff_max_min_pcm_luma_coding_block_size\000" )
	.space	3
.LC261:
	ASCII(.ascii	"log2_diff_max_min_pcm_coding_block_size(%d) out of " )
	ASCII(.ascii	"range[0,2].\012\000" )
.LC262:
	ASCII(.ascii	"pcm_log2_max_size greater than Min( CtbLog2SizeY, 5" )
	ASCII(.ascii	" ).\012\000" )
.LC263:
	ASCII(.ascii	"num_short_term_ref_pic_sets\000" )
.LC264:
	ASCII(.ascii	"num_short_term_ref_pic_sets out of range[0,%d].\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC265:
	ASCII(.ascii	"SPS HEVC_DecShortTermRefPicSet error.\012\000" )
	.space	1
.LC266:
	ASCII(.ascii	"num_long_term_ref_pic_sps\000" )
	.space	2
.LC267:
	ASCII(.ascii	"num_long_term_ref_pic_sps out of range[0,32].\012\000" )
	.space	1
.LC268:
	ASCII(.ascii	"SPS HEVC_DecVuiParam error.\012\000" )
	.space	3
.LC269:
	ASCII(.ascii	"sps_video_parameter_set_id out of range(0,%d).\012\000" )
.LC270:
	ASCII(.ascii	"sps_max_sub_layers_minus1 out of range(0,%d).\012\000" )
	.space	1
.LC271:
	ASCII(.ascii	"SPS HEVC_DecPTL error.\012\000" )
.LC272:
	ASCII(.ascii	"sps_seq_parameter_set_id\000" )
	.space	3
.LC273:
	ASCII(.ascii	"sps_seq_parameter_set_id out of range(0,%d).\012\000" )
	.space	2
.LC274:
	ASCII(.ascii	"SPS[%d] decode error0.\012\000" )
.LC275:
	ASCII(.ascii	"SPS[%d] decode error.\012\000" )
	.space	1
.LC276:
	ASCII(.ascii	"pps_seq_parameter_set_id\000" )
	.space	3
.LC277:
	ASCII(.ascii	"pic_parameter_set_id(%d) out of range[0,15].\012\000" )
	.space	2
.LC278:
	ASCII(.ascii	"SPS(%d) haven't decode.\012\000" )
	.space	3
.LC279:
	ASCII(.ascii	"num_ref_idx_l0_default_active_minus1\000" )
	.space	3
.LC280:
	ASCII(.ascii	"num_ref_idx_l0_default_active out of range[0,15].\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC281:
	ASCII(.ascii	"num_ref_idx_l1_default_active_minus1\000" )
	.space	3
.LC282:
	ASCII(.ascii	"num_ref_idx_l1_default_active out of range[0,15].\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC283:
	ASCII(.ascii	"init_qp_minus26\000" )
.LC284:
	ASCII(.ascii	"pic_init_qp(%d) out of range[-%d,51].\012\000" )
	.space	1
.LC285:
	ASCII(.ascii	"diff_cu_qp_delta_depth\000" )
	.space	1
.LC286:
	ASCII(.ascii	"diff_cu_qp_delta_depth out of range[0,3].\012\000" )
	.space	1
.LC287:
	ASCII(.ascii	"pps_cb_qp_offset\000" )
	.space	3
.LC288:
	ASCII(.ascii	"pic_cb_qp_offset out of range[-12,12].\012\000" )
.LC289:
	ASCII(.ascii	"pps_cr_qp_offset\000" )
	.space	3
.LC290:
	ASCII(.ascii	"pic_cr_qp_offset out of range[-12,12].\012\000" )
.LC291:
	ASCII(.ascii	"num_tile_columns_minus1\000" )
.LC292:
	ASCII(.ascii	"num_tile_columns(%d) out of range(0,%d).\012\000" )
	.space	2
.LC293:
	ASCII(.ascii	"num_tile_rows_minus1\000" )
	.space	3
.LC294:
	ASCII(.ascii	"num_tile_rows out(%d) of range(0,%d).\012\000" )
	.space	1
.LC295:
	ASCII(.ascii	"Logic limit(%d), num_tile_columns(%d) exceed.\012\000" )
	.space	1
.LC296:
	ASCII(.ascii	"Logic limit(%d), num_tile_rows(%d) exceed.\012\000" )
.LC297:
	ASCII(.ascii	"column_width_minus1\000" )
.LC298:
	ASCII(.ascii	"column_width[%d](%d) out of range (0 %d)\012\000" )
	.space	2
.LC299:
	ASCII(.ascii	"row_height_minus1\000" )
	.space	2
.LC300:
	ASCII(.ascii	"row_height[%d](%d) out of range (0 %d)\012\000" )
.LC301:
	ASCII(.ascii	"column_bd[%d](%d) <= 0, invalid!\012\000" )
	.space	2
.LC302:
	ASCII(.ascii	"row_bd[%d](%d) <= 0, invalid!\012\000" )
	.space	1
.LC303:
	ASCII(.ascii	"PPS tile width(%d) is too small.(Logic Unsupport)\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC304:
	ASCII(.ascii	"PPS tile height(%d) is too small.(Logic Unsupport)\012" )
	ASCII(.ascii	"\000" )
.LC305:
	ASCII(.ascii	"pps_beta_offset_div2\000" )
	.space	3
.LC306:
	ASCII(.ascii	"pps_beta_offset_div2(%d) out of range(-6,6).\012\000" )
	.space	2
.LC307:
	ASCII(.ascii	"pps_tc_offset_div2\000" )
	.space	1
.LC308:
	ASCII(.ascii	"pps_tc_offset_div2(%d) out of range(-6,6).\012\000" )
.LC309:
	ASCII(.ascii	"PPS HEVC_DecScalingListData error.\012\000" )
.LC310:
	ASCII(.ascii	"log2_parallel_merge_level_minus2\000" )
	.space	3
.LC311:
	ASCII(.ascii	"log2_parallel_merge_level(%d) out of range(2,log2_c" )
	ASCII(.ascii	"tb_sizeY).\012\000" )
	.space	1
.LC312:
	ASCII(.ascii	"pps_pic_parameter_set_id\000" )
	.space	3
.LC313:
	ASCII(.ascii	"pic_parameter_set_id(%d) out of range(0,%d).\012\000" )
	.space	2
.LC314:
	ASCII(.ascii	"PPS[%d] decode error.\012\000" )
	.space	1
.LC315:
	ASCII(.ascii	"luma_log2_weight_denom\000" )
	.space	1
.LC316:
	ASCII(.ascii	"luma_log2_weight_denom out of range(0,7).\012\000" )
	.space	1
.LC317:
	ASCII(.ascii	"delta_chroma_log2_weight_denom\000" )
	.space	1
.LC318:
	ASCII(.ascii	"delta_chroma_log2_weight_denom out of range(-7,7).\012" )
	ASCII(.ascii	"\000" )
.LC319:
	ASCII(.ascii	"chroma_log2_weight_denom out of range(0,7).\012\000" )
	.space	3
.LC320:
	ASCII(.ascii	"delta_luma_weight_l0\000" )
	.space	3
.LC321:
	ASCII(.ascii	"delta_luma_weight_l0(%d) out of range(-128,127).\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC322:
	ASCII(.ascii	"luma_offset_l0\000" )
	.space	1
.LC323:
	ASCII(.ascii	"luma_offset_l0(%d) out of range(-128,127).\012\000" )
.LC324:
	ASCII(.ascii	"delta_chroma_weight_l0\000" )
	.space	1
.LC325:
	ASCII(.ascii	"delta_chroma_weight_l0 out of range(-128,127).\012\000" )
.LC326:
	ASCII(.ascii	"delta_chroma_offset_l0\000" )
	.space	1
.LC327:
	ASCII(.ascii	"delta_chroma_offset_l0 out of range(-512,511).\012\000" )
.LC328:
	ASCII(.ascii	"delta_luma_weight_l1\000" )
	.space	3
.LC329:
	ASCII(.ascii	"delta_luma_weight_l1 out of range(-128,127).\012\000" )
	.space	2
.LC330:
	ASCII(.ascii	"luma_offset_l1\000" )
	.space	1
.LC331:
	ASCII(.ascii	"luma_offset_l1 out of range(-128,127).\012\000" )
.LC332:
	ASCII(.ascii	"delta_chroma_weight_l1\000" )
	.space	1
.LC333:
	ASCII(.ascii	"delta_chroma_weight_l1 out of range(-128,127).\012\000" )
.LC334:
	ASCII(.ascii	"delta_chroma_offset_l1\000" )
	.space	1
.LC335:
	ASCII(.ascii	"delta_chroma_offset_l1 out of range(-512,511).\012\000" )
.LC336:
	ASCII(.ascii	"slice_pic_parameter_set_id\000" )
	.space	1
.LC337:
	ASCII(.ascii	"%s: pic_parameter_set_id(%d) out of range(0 %d)!\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC338:
	ASCII(.ascii	"HEVC_VpsSpsPpsCheck != HEVC_DEC_NORMAL\012\000" )
.LC339:
	ASCII(.ascii	"max_cu_width(%d)/max_cu_height(%d) = 0\012\000" )
.LC340:
	ASCII(.ascii	"slice_segment_address(%d) out of range(0,%d).\012\000" )
	.space	1
.LC341:
	ASCII(.ascii	"slice_type\000" )
	.space	1
.LC342:
	ASCII(.ascii	"slice_reserved_flag: %d\012\000" )
	.space	3
.LC343:
	ASCII(.ascii	"slice_type(%d) out of range(0,2].\012\000" )
	.space	1
.LC344:
	ASCII(.ascii	"HEVC_ClearDPB,LastDisplayPoc:%d poc:%d \012\000" )
	.space	3
.LC345:
	ASCII(.ascii	"SH HEVC_DecShortTermRefPicSet error.\012\000" )
	.space	2
.LC346:
	ASCII(.ascii	"short_term_ref_pic_set_idx(%d) out of range[0,%d].\012" )
	ASCII(.ascii	"\000" )
.LC347:
	ASCII(.ascii	"num_long_term_sps\000" )
	.space	2
.LC348:
	ASCII(.ascii	"num_long_term_sps(%d) out of range[0,%d].\012\000" )
	.space	1
.LC349:
	ASCII(.ascii	"num_long_term_pics\000" )
	.space	1
.LC350:
	ASCII(.ascii	"SH ERROR: (num_negative_pics(%d) + num_positive_pic" )
	ASCII(.ascii	"s(%d) + num_of_longterm_pics(%d)) > %d\012\000" )
	.space	1
.LC351:
	ASCII(.ascii	"lt_idx_sps[%d] = %d, but out of range[0,%d].\012\000" )
	.space	2
.LC352:
	ASCII(.ascii	"delta_poc_msb_cycle_lt\000" )
	.space	1
.LC353:
	ASCII(.ascii	"ERROR: pRPS->num_of_pics(%d) > %d.\012\000" )
.LC354:
	ASCII(.ascii	"num_ref_idx_l0_active_minus1\000" )
	.space	3
.LC355:
	ASCII(.ascii	"num_ref_idx_l0_active(%d) out of range(0,%d).\012\000" )
	.space	1
.LC356:
	ASCII(.ascii	"num_ref_idx_l1_active_minus1\000" )
	.space	3
.LC357:
	ASCII(.ascii	"num_ref_idx_l1_active(%d) out of range(0,%d).\012\000" )
	.space	1
.LC358:
	ASCII(.ascii	"pRPS is null.\012\000" )
	.space	1
.LC359:
	ASCII(.ascii	"pRPS->num_of_pics out(%d) of range(0,%d).\012\000" )
	.space	1
.LC360:
	ASCII(.ascii	"SH HEVC_DecRefPicListsModification error.\012\000" )
	.space	1
.LC361:
	ASCII(.ascii	"collocated_ref_idx\000" )
	.space	1
.LC362:
	ASCII(.ascii	"collocated_from_l0_flag=%d, collocated_ref_idx(%d) " )
	ASCII(.ascii	"out of range(0,%d).\012\000" )
.LC363:
	ASCII(.ascii	"SH HEVC_DecPredWeightTable error.\012\000" )
	.space	1
.LC364:
	ASCII(.ascii	"five_minus_max_num_merge_cand\000" )
	.space	2
.LC365:
	ASCII(.ascii	"SH max_num_merge_cand(%d) out of range(1,5).\012\000" )
	.space	2
.LC366:
	ASCII(.ascii	"slice_qp_delta\000" )
	.space	1
.LC367:
	ASCII(.ascii	"SH slice_qp(%d) out of range [-%d,51].\012\000" )
.LC368:
	ASCII(.ascii	"slice_cb_qp_offset\000" )
	.space	1
.LC369:
	ASCII(.ascii	"SH slice_cb_qp_offset(%d) out of range[-12,12].\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC370:
	ASCII(.ascii	"SH slice_cb_qp_offset(%d)+pic_cb_qp_offset(%d) out " )
	ASCII(.ascii	"of range[-12,12].\012\000" )
	.space	2
.LC371:
	ASCII(.ascii	"slice_cr_qp_offset\000" )
	.space	1
.LC372:
	ASCII(.ascii	"SH slice_cr_qp_offset(%d) out of range[-12,12].\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC373:
	ASCII(.ascii	"SH slice_cr_qp_offset+pPPS->pic_cr_qp_offset(%d) ou" )
	ASCII(.ascii	"t of range[-12,12].\012\000" )
.LC374:
	ASCII(.ascii	"slice_beta_offset_div2\000" )
	.space	1
.LC375:
	ASCII(.ascii	"SH slice_beta_offset_div2(%d) out of range(-6,6).\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC376:
	ASCII(.ascii	"slice_tc_offset_div2\000" )
	.space	3
.LC377:
	ASCII(.ascii	"SH slice_tc_offset_div2(%d) out of range(-6,6).\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC378:
	ASCII(.ascii	"num_entry_point_offsets\000" )
.LC379:
	ASCII(.ascii	"CurrSlice.num_entry_point_offsets(%d) out of range[" )
	ASCII(.ascii	"0,%d].\012\000" )
	.space	1
.LC380:
	ASCII(.ascii	"offset_len_minus1\000" )
	.space	2
.LC381:
	ASCII(.ascii	"CurrSlice.offset_len(%d) out of range[1,32].\012\000" )
	.space	2
.LC382:
	ASCII(.ascii	"slice_segment_header_extension_length\000" )
	.space	2
.LC383:
	ASCII(.ascii	"slice_segment_header_extension_length(%d) out of ra" )
	ASCII(.ascii	"nge [0,256]\012\000" )
.LC384:
	ASCII(.ascii	"bitsLeft(%d) < slice_segment_header_extension_lengt" )
	ASCII(.ascii	"h(%d)\012\000" )
	.space	2
.LC385:
	ASCII(.ascii	"%s arrange mem failed.\012\000" )
.LC386:
	ASCII(.ascii	"Init dec para\012\000" )
	.space	1
.LC387:
	ASCII(.ascii	"%s: Get context addr failed!\012\000" )
	.space	2
.LC388:
	ASCII(.ascii	"%s FATAL: MemOffset %d > ContextMemSize %d.\012\000" )
	.space	3
.LC389:
	ASCII(.ascii	"%s: VCTRL_GetChanIDByCtx() return -1!\012\000" )
	.space	1
.LC390:
	ASCII(.ascii	"BitDepthChange ReRange FS Luma(%d-->%d),Chroma(%d -" )
	ASCII(.ascii	"->%d)\012\000" )
	.space	2
.LC391:
	ASCII(.ascii	"HEVC_InitDPB error, return %d\012\000" )
	.space	1
.LC392:
	ASCII(.ascii	"HEVC_AllocFrameStore error\012\000" )
.LC393:
	ASCII(.ascii	"%s: pstLogicFsImage is NULL!\012\000" )
	.space	2
.LC394:
	ASCII(.ascii	"HEVC InsertVO Failed return %d, ClearAll\012\000" )
	.space	2
.LC395:
	ASCII(.ascii	"HEVC.c line %d: frame para err(ret=%d), recycle ima" )
	ASCII(.ascii	"ge self\012\000" )
.LC396:
	ASCII(.ascii	"[%d]disp=%d,poc=%d,lpoc=%d,no=%d,pics=%d,cpoc=%d,fl" )
	ASCII(.ascii	"ag=%d\012\000" )
	.space	2
.LC397:
	ASCII(.ascii	"%s: FirstFrameFastOut LastPoc: %d, CurPos: %d!\012\000" )
.LC398:
	ASCII(.ascii	"%s call HEVC_GetUnRefPicWithMinPoc failed\012\000" )
	.space	1
.LC399:
	ASCII(.ascii	"%s call HEVC_OutputOneFrmFromDPB failed\012\000" )
	.space	3
.LC400:
	ASCII(.ascii	"%s: HEVC_OutputFrmToVO err!\012\000" )
	.space	3
.LC401:
	ASCII(.ascii	"cann't find slot for current nal\012\000" )
	.space	2
.LC402:
	ASCII(.ascii	"receive a zero packet\012\000" )
	.space	1
.LC403:
	ASCII(.ascii	"nal_release_err\012\000" )
	.space	3
.LC404:
	ASCII(.ascii	"No frame store available!\012\000" )
	.space	1
.LC405:
	ASCII(.ascii	"%s ReadImgNum = %d, NewImgNum = %d, FrameStore leak" )
	ASCII(.ascii	", ClearAll!\012\000" )
.LC406:
	ASCII(.ascii	"%s: flush dpb failed, return %d\012\000" )
	.space	3
.LC407:
	ASCII(.ascii	"%s: DPB.used_size(%d) >= DPB.size(%d), need to incr" )
	ASCII(.ascii	"ease DPB size.\012\000" )
	.space	1
.LC408:
	ASCII(.ascii	"%s call HEVC_IncreaseDPBSize failed\012\000" )
	.space	3
.LC409:
	ASCII(.ascii	"%s: DPB.used_size(%d) >= DPB.size(%d), output minpo" )
	ASCII(.ascii	"c frame.\012\000" )
	.space	3
.LC410:
	ASCII(.ascii	"%s with invalid pos(%d)\012\000" )
	.space	3
.LC411:
	ASCII(.ascii	"%s get APC error, ret %d\012\000" )
	.space	2
.LC412:
	ASCII(.ascii	"%s %d: InsertFrmInDPB failed, return %d\012\000" )
	.space	3
.LC413:
	ASCII(.ascii	"%s call HEVC_CheckFrameStore failed!\012\000" )
	.space	2
.LC414:
	ASCII(.ascii	"%s call HEVC_RemoveUnUsedFrameStore Failed.\012\000" )
	.space	3
.LC415:
	ASCII(.ascii	"%s call HEVC_InsertFrmInDPB Failed.\012\000" )
	.space	3
.LC416:
	ASCII(.ascii	"%s call HEVC_FirstFrameFastOut Failed\012\000" )
	.space	1
.LC417:
	ASCII(.ascii	"DPB.used_size may bigger than DPB.size\012\000" )
.LC418:
	ASCII(.ascii	"HEVC_InsertFrmInDPB Failed, ret = %d\012\000" )
	.space	2
.LC419:
	ASCII(.ascii	"%s call HEVC_OutputFrmFromDPB failed\012\000" )
	.space	2
.LC420:
	ASCII(.ascii	"%s CurrPic.state = HEVC_PIC_EMPTY, return HEVC_DEC_" )
	ASCII(.ascii	"NORMAL\012\000" )
	.space	1
.LC421:
	ASCII(.ascii	"Discard cur pic, ErrLevel(%d) > OutErrThr(%d)\012\000" )
	.space	1
.LC422:
	ASCII(.ascii	"%s call HEVC_IModeProcess Failed\012\000" )
	.space	2
.LC423:
	ASCII(.ascii	"HEVC_StorePicInDpb return(%d) from L%d\012\000" )
.LC424:
	ASCII(.ascii	"%s call HEVC_SimpleDPBProcess Failed\012\000" )
	.space	2
.LC425:
	ASCII(.ascii	"%s call HEVC_DecOrderProcess Failed\012\000" )
	.space	3
.LC426:
	ASCII(.ascii	"%s call HEVC_DispOrderProcess Failed\012\000" )
	.space	2
.LC427:
	ASCII(.ascii	"%s return HEVC_DEC_ERR\012\000" )
.LC428:
	ASCII(.ascii	"No frame store for lost pic.\012\000" )
	.space	2
.LC429:
	ASCII(.ascii	"DPB ERROR: Can not get ref_fs!\012\000" )
.LC430:
	ASCII(.ascii	"DPB no suited fs for lost pic.\012\000" )
.LC431:
	ASCII(.ascii	"Take poc(%d) to creat lost poc(%d).\012\000" )
	.space	3
.LC432:
	ASCII(.ascii	"DPB ERROR: Ref DecodeFs = NULL, ClearDPB!\012\000" )
	.space	1
.LC433:
	ASCII(.ascii	"ERROR: Can not new logic fs!\012\000" )
	.space	2
.LC434:
	ASCII(.ascii	"Lost pic insert in DPB Failed!\012\000" )
.LC435:
	ASCII(.ascii	"Ref frame(poc %d) lost.\012\000" )
	.space	3
.LC436:
	ASCII(.ascii	"s32OutErrThr = %d,drop poc %d because lost ref fram" )
	ASCII(.ascii	"e(pod %d)\012\000" )
	.space	2
.LC437:
	ASCII(.ascii	"%s call HEVC_FlushOutputFrmFromDPB failed\012\000" )
	.space	1
.LC438:
	ASCII(.ascii	"%s call HEVC_EmptyFrmFromDPB failed\012\000" )
	.space	3
.LC439:
	ASCII(.ascii	"Init pic err, exit\012\000" )
.LC440:
	ASCII(.ascii	"HEVC_WritePicMsg failed!\012\000" )
	.space	2
.LC441:
	ASCII(.ascii	"Start Reason: SliceParaNum= %d\012\000" )
.LC442:
	ASCII(.ascii	"Too many slice , err!\012\000" )
	.space	1
.LC443:
	ASCII(.ascii	"Cur pic not init yet, discard this slice!\012\000" )
	.space	1
.LC444:
	ASCII(.ascii	"Dec List error, ret=%d\012\000" )
.LC445:
	ASCII(.ascii	"Ref List is Wrong!\012\000" )
.LC446:
	ASCII(.ascii	"HEVC_WriteSliceMsg err.\012\000" )
	.space	3
.LC447:
	ASCII(.ascii	"HEVC_StorePicInDPB failed, ret = %d\012\000" )
	.space	3
.LC448:
	ASCII(.ascii	"DecVDM no slice and needn't start VDM, but MaxBytes" )
	ASCII(.ascii	"Received exceed threshold!\012\000" )
	.space	1
.LC449:
	ASCII(.ascii	"DecVDM no slice and needn't start VDM\012\000" )
	.space	1
.LC450:
	ASCII(.ascii	"IPBDecMode Changed (%d -> %d)\012\000" )
	.space	1
.LC451:
	ASCII(.ascii	"Discard this B(poc=%d) before P, is_ref_idc=%d.\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC452:
	ASCII(.ascii	"VDM start, TotalPicNum=%d.\012\000" )
.LC453:
	ASCII(.ascii	"%s no correct slice in the pic\012\000" )
.LC454:
	ASCII(.ascii	"VPS decode error.\012\000" )
	.space	1
.LC455:
	ASCII(.ascii	"SPS decode error.\012\000" )
	.space	1
.LC456:
	ASCII(.ascii	"PPS decode error.\012\000" )
	.space	1
.LC457:
	ASCII(.ascii	"SEI decode error.\012\000" )
	.space	1
.LC458:
	ASCII(.ascii	"***** NAL: UNSUPPORT, nal_unit_type=%d\012\000" )
.LC459:
	ASCII(.ascii	"HEVC_DecSliceSegmentHeader dec err\012\000" )
.LC460:
	ASCII(.ascii	"HEVCDEC_DecodePacket NULL == pPacket!\012\000" )
	.space	1
.LC461:
	ASCII(.ascii	"HevcCtx not init!\012\000" )
	.space	1
.LC462:
	ASCII(.ascii	"HEVCDEC_DecodePacket NULL == pHevcCtx->pCurrNal!\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC463:
	ASCII(.ascii	"%s last frame fs unrelease, fix it!\012\000" )
	.space	3
.LC464:
	ASCII(.ascii	"IsSizeChanged = %d,old w/h = %d/%d,new w/h = %d/%d\012" )
	ASCII(.ascii	"\000" )
.LC465:
	ASCII(.ascii	"HEVC_InquireSliceProperty error.\012\000" )
	.space	2
.LC466:
	ASCII(.ascii	"HEVC_DecOneNal ERR\012\000" )
.LC467:
	ASCII(.ascii	"%s pstLogicFsImage is NULL!\012\000" )
	.space	3
.LC468:
	ASCII(.ascii	"%s InsertVO err:%d, ClearAll\012\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
.LANCHOR2 = . + 458304
.LANCHOR3 = . + 605616
.LANCHOR5 = . + 646536
	.type	s_auiSigLastScan, %object
	.size	s_auiSigLastScan, 458752
s_auiSigLastScan:
	.space	458752
	.type	g_TsToRsMap, %object
	.size	g_TsToRsMap, 147456
g_TsToRsMap:
	.space	147456
	.type	g_IsDeced, %object
	.size	g_IsDeced, 36864
g_IsDeced:
	.space	36864
	.type	s_TmpParam, %object
	.size	s_TmpParam, 26164
s_TmpParam:
	.space	26164
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
