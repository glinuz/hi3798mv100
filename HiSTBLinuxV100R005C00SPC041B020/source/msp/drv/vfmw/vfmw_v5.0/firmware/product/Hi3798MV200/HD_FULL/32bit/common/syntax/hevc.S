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
	.type	HEVC_SetFrmRepeatCount.part.4, %function
HEVC_SetFrmRepeatCount.part.4:
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
	ldr	ip, [r0, #308]
	ldr	r2, [r0, #2536]
	ldr	r3, [r0, #304]
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
	ldrhi	r3, [r0, #308]
	ldrls	r3, [r0, #308]
	addhi	r3, r3, #2
	addls	r3, r3, #1
	str	r3, [r0, #308]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_SetFrmRepeatCount.part.4, .-HEVC_SetFrmRepeatCount.part.4
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
	add	r3, r1, #356
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
	add	r3, r0, #356
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
	ldr	r3, [r7, #1876]
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
	ldr	r2, [r6, #1208]
	ldr	r1, .L107+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #1208]
	cmp	r3, #0
	addne	r5, r7, #2352
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
	ldr	r0, [r7, #276]
	bl	FSP_GetLogicFs
	ldr	r1, .L107+16
	mov	r2, r0
	mov	r0, #1
	bl	dprint_vfmw
.L91:
	ldr	r1, .L107+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #1208]
	cmp	r3, r4
	bhi	.L93
.L94:
	ldr	r2, [r6, #1212]
	mov	r0, #1
	ldr	r1, .L107+24
	bl	dprint_vfmw
	ldr	r3, [r6, #1212]
	cmp	r3, #0
	addne	r5, r7, #2432
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
	ldr	r0, [r7, #276]
	bl	FSP_GetLogicFs
	ldr	r1, .L107+16
	mov	r2, r0
	mov	r0, #1
	bl	dprint_vfmw
.L96:
	ldr	r1, .L107+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #1212]
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
	addne	r8, r8, #640
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
	add	r6, r5, #2896
	bl	dprint_vfmw
	ldr	r3, [r5, #2832]
	ldr	r2, [r7, #2836]!
	mov	r0, #1
	ldr	r1, .L121+4
	add	r5, r5, #2960
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
	ldr	r1, [r9, #2552]
	ldr	lr, [r9, #2548]
	mov	r0, #1
	ldr	ip, [r9, #2556]
	ldr	r3, [r9, #2540]
	ldr	r2, [r9, #2536]
	stmia	sp, {r1, ip, lr}
	ldr	r1, .L139+4
	bl	dprint_vfmw
	ldr	r3, [r9, #2540]
	cmp	r3, #0
	addne	r4, r9, #2544
	movne	r5, #0
	addne	r4, r4, #12
	beq	.L128
.L127:
	ldr	r3, [r4, #4]!
	ldr	r0, [r9, #276]
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
	ldr	r0, [r9, #276]
	bl	FSP_GetRef
	ldr	r3, [r4]
	ldr	r1, [r3, #32]
	mov	r7, r0
	ldr	r0, [r9, #276]
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
	ldr	r3, [r9, #2540]
	cmp	r3, r5
	bhi	.L127
.L128:
	ldr	r1, .L139+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r9, #2540]
	cmp	r3, #0
	addne	r5, r9, #2544
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
	ldr	r3, [r9, #2540]
	add	r6, r6, #1
	cmp	r3, r6
	bls	.L126
.L135:
	ldr	r3, [r5, #4]!
	ldr	r0, [r9, #276]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	ldr	r3, [r5]
	ldr	r1, [r3, #32]
	mov	r4, r0
	ldr	r0, [r9, #276]
	bl	FSP_GetDecPhyFs
	ldr	r3, [r5]
	ldr	r1, [r3, #32]
	mov	r8, r0
	ldr	r0, [r9, #276]
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
	ldr	r3, [r3, #380]
	cmp	r3, #0
	bne	.L151
	ldr	r2, [r0, #2356]
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
	ldr	r0, [r0, #2340]
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	ldr	r0, [r4, #2340]
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
	.global	HEVC_IsFrameEndNal
	.type	HEVC_IsFrameEndNal, %function
HEVC_IsFrameEndNal:
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
	sub	r3, r0, #48
	sub	r0, r0, #32
	cmp	r0, #2
	cmphi	r3, #1
	movls	r0, #1
	movhi	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVC_IsFrameEndNal, .-HEVC_IsFrameEndNal
	.align	2
	.global	HEVCDEC_IsNeedSedOnly
	.type	HEVCDEC_IsNeedSedOnly, %function
HEVCDEC_IsNeedSedOnly:
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
	ldr	r3, [r0, #208]
	mov	r4, r0
	cmp	r3, #0
	beq	.L161
	ldr	r2, [r0, #2360]
	cmp	r2, #0
	beq	.L166
.L161:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L166:
	add	r2, r0, #61440
	ldr	r2, [r2, #696]
	cmp	r2, #1
	bls	.L161
	ldr	r2, [r0, #236]
	cmp	r3, r2
	beq	.L161
	ldr	r3, [r0, #220]
	cmp	r3, #1
	beq	.L167
	cmp	r3, #0
	bne	.L161
	ldr	r0, [r0, #276]
	bl	VCTRL_CheckFreshRaw
	cmp	r0, #0
	beq	.L161
	ldr	r0, [r4, #276]
	bl	VCTRL_CheckFreshSeg
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L167:
	ldr	r0, [r0, #2364]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVCDEC_IsNeedSedOnly, .-HEVCDEC_IsNeedSedOnly
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
	ldr	r3, [r5, #2872]
	mov	r8, r1
	ldr	r2, [r5, #2868]
	ldr	r7, [r3, #68]
	ldr	r9, [r3, #72]
	ldr	r2, [r2, #148]
	sub	r3, r7, #1
	cmp	r3, #9
	str	r2, [fp, #-52]
	bhi	.L196
	sub	r3, r9, #1
	cmp	r3, #10
	bhi	.L197
	ldr	r4, .L198
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
	ble	.L177
	ldr	r3, [fp, #-52]
	mov	r2, #0
	mov	r1, r2
	mov	r0, r6
	sub	r10, r3, #4
.L176:
	str	r2, [r6, r1, asl #3]
	mov	r3, r2, asl r10
	ldr	ip, [r5, #2872]
	add	ip, ip, r1, lsl #1
	ldrsh	ip, [ip, #176]
	add	r2, ip, r2
	sub	ip, r2, #1
	str	ip, [r0, #4]
	mov	lr, r2, asl r10
	cmp	r3, lr
	bge	.L182
	cmp	r3, #255
	bhi	.L178
	uxtb	r4, r1
	add	ip, r8, r3
	b	.L180
.L181:
	cmp	r3, #256
	beq	.L178
.L180:
	add	r3, r3, #1
	strb	r4, [ip], #1
	cmp	r3, lr
	bne	.L181
.L182:
	add	r1, r1, #1
	add	r0, r0, #8
	cmp	r1, r7
	bne	.L176
.L177:
	cmp	r9, #0
	ble	.L174
	ldr	r3, [fp, #-52]
	mov	ip, #0
	ldr	r7, [fp, #-48]
	mov	lr, ip
	sub	r4, r3, #4
	mov	r6, r7
.L184:
	str	ip, [r7, lr, asl #3]
	mov	r3, ip, asl r4
	ldr	r2, [r5, #2872]
	add	r2, r2, lr, lsl #1
	ldrsh	r2, [r2, #216]
	add	ip, r2, ip
	sub	r2, ip, #1
	str	r2, [r6, #4]
	mov	r1, ip, asl r4
	cmp	r3, r1
	bge	.L189
	cmp	r3, #255
	bhi	.L185
	add	r2, r3, #255
	uxtb	r0, lr
	add	r2, r8, r2
	b	.L187
.L188:
	cmp	r3, #256
	beq	.L185
.L187:
	add	r3, r3, #1
	strb	r0, [r2, #1]!
	cmp	r3, r1
	bne	.L188
.L189:
	add	lr, lr, #1
	add	r6, r6, #8
	cmp	lr, r9
	bne	.L184
.L174:
	mov	r0, #0
	b	.L170
.L178:
	ldr	r2, .L198+4
	mov	r0, #1
	ldr	r1, .L198+8
	bl	dprint_vfmw
	mvn	r0, #0
.L170:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L185:
	ldr	r2, .L198+4
	mov	r0, #1
	ldr	r1, .L198+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L170
.L197:
	mov	r1, #11
	mov	r3, r9
	str	r1, [sp]
	mov	r0, #1
	ldr	r2, .L198+4
	ldr	r1, .L198+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L170
.L196:
	mov	r1, #10
	mov	r3, r7
	str	r1, [sp]
	mov	r0, #1
	ldr	r2, .L198+4
	ldr	r1, .L198+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L170
.L199:
	.align	2
.L198:
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
	beq	.L224
	cmp	r1, #0
	beq	.L225
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
.L204:
	ldrb	r5, [r2, #1]!	@ zero_extendqisi2
	ldrb	r4, [r0, #1]!	@ zero_extendqisi2
	cmp	r5, r4
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r7
	bne	.L204
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
.L205:
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
	bls	.L205
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
	beq	.L206
	add	r7, r10, #148
	add	r5, r1, #148
	mov	r9, #1
.L207:
	mov	r0, r5
	mov	r2, r7
	mov	r3, #0
.L208:
	ldr	lr, [r2, #4]!
	add	r3, r3, #1
	ldr	ip, [r0, #4]!
	cmp	lr, ip
	movne	r6, #0
	andeq	r6, r6, #1
	cmp	r3, r4
	bls	.L208
	add	r9, r9, #1
	add	r7, r7, #4
	cmp	r9, r8
	add	r5, r5, #4
	bls	.L207
.L206:
	ldrb	r3, [r10, #6]	@ zero_extendqisi2
	ldrb	r2, [r1, #6]	@ zero_extendqisi2
	cmp	r2, r3
	movne	r6, #0
	andeq	r6, r6, #1
	cmp	r3, #0
	beq	.L209
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
	bne	.L226
.L210:
	ldr	r2, [r10, #48]
	ldr	r3, [r1, #48]
	cmp	r2, r3
	movne	r6, #0
	andeq	r6, r6, #1
.L209:
	eor	r6, r6, #1
	rsb	r0, r6, #0
.L202:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L226:
	ldr	r2, [r10, #44]
	ldr	r3, [r1, #44]
	cmp	r2, r3
	movne	r6, #0
	andeq	r6, r6, #1
	b	.L210
.L225:
	mov	r0, r1
	movw	r3, #1884
	ldr	r2, .L227
	ldr	r1, .L227+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L202
.L224:
	movw	r3, #1883
	ldr	r2, .L227
	ldr	r1, .L227+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L202
.L228:
	.align	2
.L227:
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
	ldr	r2, [r0, #2340]
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
	ldr	r3, .L237
	mov	r5, r1
	mov	r2, #32
	ldr	r1, [r0, #2340]
	sub	r0, fp, #52
	ldr	r3, [r3, #52]
	mov	r5, r5, asl #3
	blx	r3
	ldr	r3, [fp, #-28]
	ldr	r2, [fp, #-24]
	rsb	r3, r2, r3
	cmp	r3, r5
	movcs	r0, #0
	bcc	.L236
.L231:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L236:
	mov	r1, #1
	sub	r0, fp, #52
	bl	BsShow
	cmp	r0, #0
	moveq	r0, #1
	beq	.L231
	mov	r1, #1
	sub	r0, fp, #52
	bl	BsSkip
	ldr	r3, [fp, #-28]
	ldr	r4, [fp, #-24]
	rsb	r4, r4, r3
	b	.L232
.L233:
	bl	BsSkip
	ldr	r3, [fp, #-28]
	ldr	r4, [fp, #-24]
	rsb	r4, r4, r3
.L232:
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
	bne	.L233
	mov	r0, r3
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L238:
	.align	2
.L237:
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
	beq	.L247
	mov	r1, #8
	ldr	r0, [r0, #2340]
	bl	BsGet
	sub	r4, r7, #1
	add	r8, r7, #47
	mov	r3, r0
	cmp	r3, #0
	str	r0, [r7, #48]
	str	r0, [r7, #52]
	bne	.L241
.L252:
	add	r5, r4, #16
.L242:
	mov	r1, #8
	ldr	r0, [r6, #2340]
	bl	BsGet
	strb	r0, [r4, #1]!
	cmp	r4, r5
	bne	.L242
.L243:
	cmp	r5, r8
	mov	r4, r5
	beq	.L248
.L254:
	ldr	r3, [r7, #52]
	cmp	r3, #0
	beq	.L252
.L241:
	cmp	r3, #1
	beq	.L253
	cmp	r3, #2
	addne	r5, r4, #16
	bne	.L243
	mov	r1, #32
	ldr	r0, [r6, #2340]
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
	bne	.L254
.L248:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L253:
	mov	r1, #16
	ldr	r0, [r6, #2340]
	bl	BsGet
	add	r5, r4, #16
	mov	r3, r0, lsr #8
	strb	r0, [r4, #2]
	strb	r3, [r4, #1]
	b	.L243
.L247:
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
	beq	.L257
	ldr	r0, [r0, #2340]
	mov	r1, #1
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
	beq	.L261
	add	r7, r1, #12
	mov	r4, r1
	mov	r3, #1
	strb	r3, [r1]
.L260:
	mov	r1, #16
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #4]!
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r4, r7
	str	r0, [r4, #12]
	bne	.L260
	mov	r1, #16
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r6, #28]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r6, #32]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r6, #36]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r3, #0
	str	r0, [r6, #40]
.L259:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L261:
	mvn	r3, #0
	b	.L259
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
	beq	.L265
	mov	r3, #1
	mov	r1, #16
	strb	r3, [r5]
	ldr	r0, [r0, #2340]
	bl	BsGet
	mov	r1, #16
	strh	r0, [r5, #2]	@ movhi
	ldr	r0, [r6, #2340]
	bl	BsGet
	mov	r3, r4
	strh	r0, [r5, #4]	@ movhi
.L264:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L265:
	mvn	r3, #0
	b	.L264
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
	beq	.L268
	ldr	r0, [r0, #2340]
	mov	r1, #8
	bl	BsGet
	mov	r3, r4
	strb	r0, [r5]
.L267:
	mov	r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L268:
	mvn	r3, #0
	b	.L267
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
	beq	.L271
	mov	r1, #1
	ldr	r0, [r0, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r6]
	ldr	r0, [r5, #2340]
	bl	BsGet
	str	r0, [r6, #4]
	ldr	r0, [r5, #2340]
	bl	BsToNextByte
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L271:
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
	beq	.L275
	mov	r1, #1
	ldr	r0, [r0, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5, #2]
	cmp	r0, #0
	beq	.L276
.L274:
	ldr	r0, [r4, #2340]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L276:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r5, #1]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #4]
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r5, #3]
	b	.L274
.L275:
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
	ldr	r3, .L278
	mov	r4, r0
	add	r0, r0, #27136
	mov	r2, #80
	mov	r1, #0
	add	r0, r0, #224
	ldr	r3, [r3, #48]
	blx	r3
	add	r3, r4, #4096
	add	ip, r3, #2576
	add	r4, r4, #6656
	mov	r0, #0
	mov	r1, #0
	mov	r2, #0
	strd	r0, [ip]
	strd	r0, [r4, #-8]
	strd	r0, [r4]
	strd	r0, [ip, #-8]
	str	r2, [r3, #460]
	str	r2, [r3, #464]
	str	r2, [r3, #452]
	str	r2, [r3, #456]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L279:
	.align	2
.L278:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	HEVC_Sei_ParsePts_Init, .-HEVC_Sei_ParsePts_Init
	.align	2
	.global	HEVC_Sei_Get_Dpb_Output_Time
	.type	HEVC_Sei_Get_Dpb_Output_Time, %function
HEVC_Sei_Get_Dpb_Output_Time:
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
	add	r3, r0, #53248
	ldr	r5, [r0, #2352]
	movw	r6, #2024
	ldr	ip, [r3, #1124]
	add	r3, r0, #24576
	ldr	lr, [r1, #12]
	ldr	r4, [r0, #2348]
	mla	ip, r6, ip, r5
	movw	r5, #26164
	ldr	r6, [r3, #2788]
	cmp	r6, #1
	ldr	ip, [ip, #32]
	str	lr, [r3, #2808]
	ldr	lr, [r1, #16]
	mla	r1, r5, ip, r4
	str	lr, [r3, #2812]
	ldr	lr, [r2, #76]
	str	lr, [r3, #2832]
	ldr	ip, [r2, #80]
	str	ip, [r3, #2836]
	ldr	ip, [r1, #56]
	add	r1, r1, ip, lsl #2
	ldrb	r8, [r1, #412]	@ zero_extendqisi2
	beq	.L288
	ldrb	r1, [r3, #2857]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L289
	add	ip, r0, #4096
	add	r0, r0, #6656
	add	r1, ip, #2576
	ldrd	r4, [r0, #-8]
	ldr	r6, [ip, #484]
	adds	r4, r4, r6
	adc	r5, r5, #0
	strd	r4, [r1]
.L282:
	add	r1, ip, #2576
	mov	r0, #0
	strd	r4, [r1, #-8]
	strb	r0, [r3, #2857]
	ldrb	r2, [r2, #9]	@ zero_extendqisi2
	cmp	r2, r0
	ldr	r2, [ip, #488]
	bne	.L290
	adds	r4, r4, r2
	add	r3, r3, #2784
	adc	r5, r5, #0
	subs	r4, r4, r8
	sbc	r5, r5, #0
	strd	r4, [r3, #40]
	umull	r0, r1, r4, lr
	mla	r1, lr, r5, r1
	strd	r0, [r3, #32]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L289:
	ldrb	r1, [r0, #3505]	@ zero_extendqisi2
	add	ip, r0, #4096
	cmp	r1, #0
	add	r1, r0, #6656
	bne	.L291
	ldrd	r4, [r1, #-8]
	ldrd	r6, [r1]
	strd	r4, [fp, #-52]
.L285:
	mov	r5, #0
	ldr	r0, [ip, #484]
	mov	r1, r5
	ldrd	r4, [fp, #-52]
	add	r9, ip, #2576
	add	r10, ip, #2560
	adds	r4, r4, r0
	strd	r0, [fp, #-60]
	adc	r5, r5, r1
	cmp	r4, r6
	sbcs	r1, r5, r7
	strd	r4, [r9]
	strd	r4, [r10, #-8]
	bge	.L282
	ldrd	r4, [fp, #-60]
	adds	r4, r4, r6
	adc	r5, r5, r7
	strd	r4, [r9]
	strd	r4, [r10, #-8]
	b	.L282
.L290:
	adds	r4, r4, r2
	add	r1, r3, #2784
	adc	r5, r5, #0
	movs	r5, r5, asr #1
	mov	r4, r4, rrx
	subs	r4, r4, r8
	sbc	r5, r5, #0
	strd	r4, [r1, #40]
	umull	r2, r3, r4, lr
	mla	r3, lr, r5, r3
	strd	r2, [r1, #32]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L288:
	add	ip, r0, #4096
	add	r0, r0, #6656
	add	r1, ip, #2576
	ldr	r4, [ip, #452]
	mov	r5, r4, asr #31
	strd	r4, [r1]
	strd	r4, [r0, #-8]
	b	.L282
.L291:
	ldrd	r0, [r1]
	strd	r0, [fp, #-52]
	mov	r6, r0
	mov	r7, r1
	b	.L285
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
	bls	.L296
.L293:
	sub	r1, r1, #6
	cmp	r1, #3
	ldmlsfd	sp, {r4, r5, fp, sp, pc}
	add	r3, r0, #6656
	mov	r0, r3
	add	r3, r3, #16
	ldrd	r2, [r3]
	strd	r2, [r0]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L296:
	ldr	ip, [r0, #2356]
	mvn	r3, #0
	mvn	r2, #0
	ldrd	r4, [ip, #56]
	cmp	r5, r3
	cmpeq	r4, r2
	beq	.L294
	add	r2, r0, #27392
	movw	ip, #27424
	add	r3, r2, #16
	strd	r4, [r2, #24]
	ldrd	r2, [r3, #-8]
	strd	r2, [r0, ip]
.L294:
	add	r3, r0, #24576
	mov	r2, #1
	strb	r2, [r3, #2856]
	b	.L293
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
	add	r2, r0, #27392
	add	r1, r0, #24576
	mov	r4, r0
	ldrd	r6, [r2, #24]
	mov	r3, r2
	ldr	r5, [r1, #2832]
	movw	r0, #27424
	ldr	r10, [r1, #2836]
	strd	r6, [fp, #-52]
	cmp	r5, #0
	ldrd	r6, [r3, #8]
	ldrd	r2, [r4, r0]
	strd	r2, [fp, #-60]
	beq	.L300
	mov	r9, r5
	mov	r0, r10
	b	.L299
.L301:
	mov	r9, r1
.L299:
	mov	r1, r9
	bl	__aeabi_uidivmod
	mov	r0, r9
	cmp	r1, #0
	bne	.L301
.L298:
	ldrd	r2, [fp, #-60]
	mov	r1, r9
	mov	r0, r5
	mov	r8, #1000
	subs	r2, r6, r2
	add	r4, r4, #54272
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
	strd	r0, [r4]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L300:
	mov	r9, r10
	b	.L298
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
	ldr	r6, .L334
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
	bls	.L320
	add	r4, r6, #39
	add	r9, r5, #3
	add	r6, r6, #55
	sub	r10, r8, #1
	mov	r8, #0
.L305:
	mov	r1, #8
	ldr	r0, [r7, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r9, #1]!
	ldrb	r1, [r4, #1]!	@ zero_extendqisi2
	cmp	r1, r0
	strne	r10, [r5, #20]
	strneb	r8, [r5, #24]
	cmp	r4, r6
	sub	r10, r10, #1
	bne	.L305
	ldrb	r4, [r5, #24]	@ zero_extendqisi2
	cmp	r4, #1
	beq	.L330
	ldr	r3, [fp, #-104]
	sub	r9, r3, #16
	str	r9, [r5, #20]
	cmp	r9, #0
	beq	.L315
	mov	r4, #0
.L319:
	mov	r1, #8
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldr	r3, [r5, #20]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L319
.L318:
	mov	r0, #0
.L303:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L315:
	mov	r0, #0
	str	r0, [r5]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L330:
	mov	r1, #5
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #3
	uxtb	r6, r0
	ldr	r0, [r7, #2340]
	bl	BsGet
	uxtb	r0, r0
	cmp	r6, #6
	cmpeq	r0, #3
	beq	.L331
	sub	r3, r6, #2
	cmp	r0, #0
	cmpeq	r6, #2
	clz	r3, r3
	mov	r3, r3, lsr #5
	beq	.L332
	cmp	r0, #1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r3, #0
	bne	.L333
	ldr	r3, [fp, #-104]
	sub	r9, r3, #18
	str	r9, [r5, #20]
.L310:
	cmp	r9, #0
	movne	r4, #0
	beq	.L315
.L317:
	mov	r1, #8
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldr	r3, [r5, #20]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L317
	b	.L318
.L333:
	mov	r1, #8
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #8
	mov	r4, r0
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldr	r3, [fp, #-104]
	uxtb	r4, r4
	sub	r9, r3, #19
	str	r9, [r5, #20]
	uxtb	r0, r0
	add	r4, r4, r0, lsl #8
	str	r4, [r5, #64]
	b	.L310
.L320:
	mvn	r0, #0
	b	.L303
.L331:
	mov	r1, #2
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #31]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #30]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #29]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #28]
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldrb	r3, [r5, #29]	@ zero_extendqisi2
	mov	r2, #28
	add	r0, r5, #36
	cmp	r3, #0
	ldr	r3, .L334+4
	subeq	r1, fp, #100
	subne	r1, fp, #72
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, #8
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r3, #50
	mov	r1, #8
	uxtb	r0, r0
	mul	r3, r3, r0
	str	r3, [r5, #32]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #8
	mov	r4, r0
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldr	r3, [fp, #-104]
	uxtb	r4, r4
	sub	r9, r3, #21
	str	r9, [r5, #20]
	uxtb	r0, r0
	add	r4, r4, r0, lsl #8
	str	r4, [r5, #64]
	b	.L310
.L332:
	mov	r1, #2
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #31]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #30]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, r4
	strb	r0, [r5, #29]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #28]
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldrb	r3, [r5, #29]	@ zero_extendqisi2
	mov	r2, #28
	add	r0, r5, #36
	cmp	r3, #0
	ldr	r3, .L334+4
	subeq	r1, fp, #100
	subne	r1, fp, #72
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, #8
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r3, #50
	ldr	r2, [fp, #-104]
	sub	r9, r2, #19
	str	r9, [r5, #20]
	uxtb	r0, r0
	mul	r3, r3, r0
	str	r3, [r5, #32]
	b	.L310
.L335:
	.align	2
.L334:
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
	ldr	r3, [r7, #1208]
	cmp	r3, #0
	beq	.L337
	ldr	r4, [r0, #2368]
	cmp	r4, #0
	beq	.L338
	ldr	r2, [r4, #32]
	cmp	r2, #0
	addne	r6, r0, #2368
	movne	r4, #0
	bne	.L343
	b	.L381
.L344:
	ldr	r3, [ip, #640]
	cmp	r3, #0
	beq	.L345
	ldr	r3, [r7, #1208]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L382
	ldr	r2, [r6, #4]!
	cmp	r2, #0
	beq	.L338
	ldr	r2, [r2, #32]
	cmp	r2, #0
	beq	.L339
.L343:
	ldr	r1, [r2, #32]
	ldr	r0, [r5, #276]
	bl	FSP_GetLogicFs
	subs	ip, r0, #0
	bne	.L344
.L345:
	add	r3, r4, #592
	mov	r2, r4
	ldr	r1, .L386
	mov	r0, #1
	ldr	r3, [r5, r3, asl #2]
	ldr	r3, [r3, #32]
	str	ip, [sp]
	bl	dprint_vfmw
.L342:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L382:
	cmp	r3, #0
	beq	.L337
	ldr	r2, [r7, #1212]
	cmp	r2, #0
	beq	.L337
	ldr	r4, [r5, #2436]
	cmp	r4, #0
	beq	.L350
	ldr	r2, [r4, #32]
	cmp	r2, #0
	addne	r6, r5, #2432
	movne	r4, #0
	addne	r6, r6, #4
	bne	.L354
	b	.L383
.L355:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L356
	ldr	r3, [r7, #1212]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L337
	ldr	r3, [r6, #4]!
	cmp	r3, #0
	beq	.L384
	ldr	r2, [r3, #32]
	cmp	r2, #0
	beq	.L385
.L354:
	ldr	r1, [r2, #32]
	ldr	r0, [r5, #276]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L355
.L356:
	mov	r3, r0
	mov	r2, r4
	ldr	r1, .L386+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L376
.L337:
	mov	r0, #0
.L376:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L338:
	mov	r2, r4
	ldr	r1, .L386+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L342
.L381:
	mov	r4, r2
.L339:
	mov	r2, r4
	ldr	r1, .L386+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L342
.L384:
	ldr	r3, [r7, #1208]
.L350:
	mov	r2, r4
	ldr	r1, .L386+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L376
.L385:
	ldr	r3, [r7, #1208]
.L351:
	mov	r2, r4
	ldr	r1, .L386+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L376
.L383:
	mov	r4, r2
	b	.L351
.L387:
	.align	2
.L386:
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
	ldr	r3, [r0, #2540]
	cmp	r3, #16
	bhi	.L389
	cmp	r3, #0
	addne	ip, r0, #2544
	mvnne	r4, #-2147483648
	addne	ip, ip, #12
	movne	r3, #0
	beq	.L391
.L394:
	ldr	r2, [ip, #4]!
	ldr	lr, [r2, #12]
	cmp	lr, r4
	bge	.L393
	ldr	r2, [r2]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	streq	r3, [r1]
	moveq	r4, lr
.L393:
	ldr	r2, [r0, #2540]
	add	r3, r3, #1
	cmp	r2, r3
	bhi	.L394
	ldr	r3, [r1]
	cmn	r3, #1
	beq	.L391
	str	r4, [sp]
	mov	r0, #1
	ldr	r2, .L397
	ldr	r1, .L397+4
	bl	dprint_vfmw
	mov	r0, #0
.L392:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L389:
	ldr	r2, .L397
	mov	r0, #1
	ldr	r1, .L397+8
	bl	dprint_vfmw
	mov	r0, r4
	b	.L392
.L391:
	ldr	r2, .L397
	mov	r0, #1
	ldr	r1, .L397+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L392
.L398:
	.align	2
.L397:
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
	ldr	r3, [r0, #2536]
	mov	r4, r0
	cmp	r3, #16
	bhi	.L406
	ldr	r0, [r0, #276]
	bl	FSP_GetTotalValidFsNum
	add	r2, r4, #53248
	ldr	r1, [r2, #384]
	cmp	r1, #1920
	movhi	r2, #1
	bls	.L407
.L402:
	rsb	r2, r2, r0
	bic	r2, r2, r2, asr #31
	cmp	r2, #0
	ble	.L403
	ldr	r3, [r4, #2536]
	add	r3, r3, #1
	cmp	r2, r3
	bcc	.L403
	str	r3, [r4, #2536]
	mov	r0, #13
	ldr	r2, .L408
	ldr	r1, .L408+4
	bl	dprint_vfmw
	mov	r0, #0
.L401:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L407:
	ldr	r2, [r2, #388]
	cmp	r2, #1088
	movhi	r2, #1
	movls	r2, #4
	b	.L402
.L403:
	mov	r3, r2
	ldr	r1, .L408+8
	ldr	r2, .L408
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L401
.L406:
	mov	r5, #17
	ldr	r2, .L408
	mov	r0, #1
	str	r5, [sp]
	ldr	r1, .L408+12
	bl	dprint_vfmw
	str	r5, [r4, #2536]
	mvn	r0, #0
	b	.L401
.L409:
	.align	2
.L408:
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
	ldr	r3, [r0, #1120]
	sub	r2, r3, #16
	cmp	r2, #5
	bls	.L411
	ldr	r3, [r0, #372]
	sub	r3, r3, #16
	cmp	r3, #5
	bls	.L422
	ldr	r3, [r0, #1108]
	cmp	r3, #1
	beq	.L414
	cmp	r3, #2
	beq	.L415
	cmp	r3, #0
	moveq	r3, #2
	streq	r3, [r0, #380]
	ldmfd	sp, {fp, sp, pc}
.L411:
	str	r3, [r0, #372]
	mov	r3, #0
	str	r3, [r0, #380]
	ldmfd	sp, {fp, sp, pc}
.L414:
	ldr	r2, [r0, #380]
	cmp	r2, #2
	beq	.L423
	str	r3, [r0, #380]
	ldmfd	sp, {fp, sp, pc}
.L415:
	ldr	r3, [r0, #380]
	sub	r3, r3, #1
	cmp	r3, #1
	movhi	r3, #0
	strhi	r3, [r0, #380]
	ldmfd	sp, {fp, sp, pc}
.L423:
	ldmfd	sp, {fp, sp, pc}
.L422:
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
	ldr	r0, [r0, #1120]
	cmp	r0, #15
	bhi	.L425
	tst	r0, #1
	bne	.L427
.L425:
	sub	r0, r0, #16
	cmp	r0, #7
	movhi	r0, #0
	movls	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L427:
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
	ldr	r0, [r1, #620]
	ldr	r3, [r1, #624]
	ldrb	ip, [r1, #612]	@ zero_extendqisi2
	ldrb	lr, [r1, #616]	@ zero_extendqisi2
	mov	r0, r0, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r0, r0
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r0
	str	r3, [r2]
	ldr	r0, [r1, #620]
	ldrb	lr, [r1, #613]	@ zero_extendqisi2
	ldrb	r3, [r1, #625]	@ zero_extendqisi2
	and	r0, r0, #65280
	ldrb	ip, [r1, #617]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #4]
	ldr	r0, [r1, #616]
	ldrb	lr, [r1, #614]	@ zero_extendqisi2
	ldrb	r3, [r1, #626]	@ zero_extendqisi2
	and	r0, r0, #16711680
	ldrb	ip, [r1, #622]	@ zero_extendqisi2
	orr	r0, lr, r0
	orr	r3, r0, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #8]
	ldr	r0, [r1, #624]
	ldrb	r3, [r1, #619]	@ zero_extendqisi2
	ldrb	lr, [r1, #615]	@ zero_extendqisi2
	and	r0, r0, #-16777216
	ldrb	ip, [r1, #623]	@ zero_extendqisi2
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
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	lr, r0, #18
	add	r0, r1, r0, lsl #6
	add	lr, r1, lr, lsl #6
	ldr	r1, [r0, #1772]
	ldr	r3, [r0, #1788]
	ldrb	ip, [lr, #612]	@ zero_extendqisi2
	ldrb	r4, [r0, #1780]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, r4, asl #16
	orr	r3, r3, r1
	str	r3, [r2]
	ldr	r1, [r0, #1804]
	ldr	r3, [r0, #1820]
	ldrb	ip, [r0, #1796]	@ zero_extendqisi2
	ldrb	r4, [r0, #1812]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, r4, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #4]
	ldr	r1, [r0, #1772]
	ldrb	r4, [lr, #613]	@ zero_extendqisi2
	ldrb	r3, [r0, #1789]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1781]	@ zero_extendqisi2
	orr	r1, r4, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #8]
	ldr	r1, [r0, #1804]
	ldrb	r4, [r0, #1797]	@ zero_extendqisi2
	ldrb	r3, [r0, #1821]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1813]	@ zero_extendqisi2
	orr	r1, r4, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #12]
	ldr	ip, [r0, #1780]
	ldrb	r4, [lr, #614]	@ zero_extendqisi2
	ldrb	r3, [r0, #1790]	@ zero_extendqisi2
	and	ip, ip, #16711680
	ldrb	r1, [r0, #1774]	@ zero_extendqisi2
	orr	ip, r4, ip
	orr	r3, ip, r3, asl #24
	orr	r3, r3, r1, asl #8
	str	r3, [r2, #16]
	ldr	ip, [r0, #1812]
	ldrb	r4, [r0, #1798]	@ zero_extendqisi2
	ldrb	r3, [r0, #1822]	@ zero_extendqisi2
	and	ip, ip, #16711680
	ldrb	r1, [r0, #1806]	@ zero_extendqisi2
	orr	ip, r4, ip
	orr	r3, ip, r3, asl #24
	orr	r3, r3, r1, asl #8
	str	r3, [r2, #20]
	ldr	r1, [r0, #1788]
	ldrb	lr, [lr, #615]	@ zero_extendqisi2
	ldrb	r3, [r0, #1783]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1775]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #24]
	ldr	r1, [r0, #1820]
	ldrb	lr, [r0, #1799]	@ zero_extendqisi2
	ldrb	r3, [r0, #1815]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1807]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #28]
	ldr	r1, [r0, #1776]
	ldr	r3, [r0, #1792]
	ldrb	ip, [r0, #1768]	@ zero_extendqisi2
	ldrb	lr, [r0, #1784]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #32]
	ldr	r1, [r0, #1808]
	ldr	r3, [r0, #1824]
	ldrb	ip, [r0, #1800]	@ zero_extendqisi2
	ldrb	lr, [r0, #1816]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #36]
	ldr	r1, [r0, #1776]
	ldrb	lr, [r0, #1769]	@ zero_extendqisi2
	ldrb	r3, [r0, #1793]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1785]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #40]
	ldr	r1, [r0, #1808]
	ldrb	lr, [r0, #1801]	@ zero_extendqisi2
	ldrb	r3, [r0, #1825]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1817]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #44]
	ldr	r1, [r0, #1784]
	ldrb	lr, [r0, #1770]	@ zero_extendqisi2
	ldrb	r3, [r0, #1794]	@ zero_extendqisi2
	and	r1, r1, #16711680
	ldrb	ip, [r0, #1778]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #48]
	ldrb	lr, [r0, #1802]	@ zero_extendqisi2
	ldr	r1, [r0, #1816]
	ldrb	r3, [r0, #1826]	@ zero_extendqisi2
	ldrb	ip, [r0, #1810]	@ zero_extendqisi2
	and	r1, r1, #16711680
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #52]
	ldr	r1, [r0, #1792]
	ldrb	lr, [r0, #1771]	@ zero_extendqisi2
	ldrb	r3, [r0, #1787]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1779]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #56]
	ldr	r1, [r0, #1824]
	ldrb	ip, [r0, #1803]	@ zero_extendqisi2
	ldrb	r3, [r0, #1819]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	r0, [r0, #1811]	@ zero_extendqisi2
	orr	r1, ip, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, r0, asl #8
	str	r3, [r2, #60]
	ldmfd	sp, {r4, fp, sp, pc}
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
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	lr, r0, #6
	add	r0, r1, r0, lsl #6
	add	lr, r1, lr, lsl #6
	ldr	r1, [r0, #1012]
	ldr	r3, [r0, #1044]
	ldrb	ip, [lr, #612]	@ zero_extendqisi2
	ldrb	r4, [r0, #1028]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, r4, asl #16
	orr	r3, r3, r1
	str	r3, [r2]
	ldr	r1, [r0, #1020]
	ldr	r3, [r0, #1052]
	ldrb	ip, [r0, #1004]	@ zero_extendqisi2
	ldrb	r4, [r0, #1036]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, r4, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #4]
	ldr	r1, [r0, #1012]
	ldrb	r4, [lr, #613]	@ zero_extendqisi2
	ldrb	r3, [r0, #1045]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1029]	@ zero_extendqisi2
	orr	r1, r4, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #8]
	ldr	r1, [r0, #1020]
	ldrb	r4, [r0, #1005]	@ zero_extendqisi2
	ldrb	r3, [r0, #1053]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1037]	@ zero_extendqisi2
	orr	r1, r4, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #12]
	ldr	ip, [r0, #1028]
	ldrb	r4, [lr, #614]	@ zero_extendqisi2
	ldrb	r3, [r0, #1046]	@ zero_extendqisi2
	and	ip, ip, #16711680
	ldrb	r1, [r0, #1014]	@ zero_extendqisi2
	orr	ip, r4, ip
	orr	r3, ip, r3, asl #24
	orr	r3, r3, r1, asl #8
	str	r3, [r2, #16]
	ldr	ip, [r0, #1036]
	ldrb	r4, [r0, #1006]	@ zero_extendqisi2
	ldrb	r3, [r0, #1054]	@ zero_extendqisi2
	and	ip, ip, #16711680
	ldrb	r1, [r0, #1022]	@ zero_extendqisi2
	orr	ip, r4, ip
	orr	r3, ip, r3, asl #24
	orr	r3, r3, r1, asl #8
	str	r3, [r2, #20]
	ldr	r1, [r0, #1044]
	ldrb	lr, [lr, #615]	@ zero_extendqisi2
	ldrb	r3, [r0, #1031]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1015]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #24]
	ldr	r1, [r0, #1052]
	ldrb	lr, [r0, #1007]	@ zero_extendqisi2
	ldrb	r3, [r0, #1039]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1023]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #28]
	ldr	r1, [r0, #1016]
	ldr	r3, [r0, #1048]
	ldrb	ip, [r0, #1000]	@ zero_extendqisi2
	ldrb	lr, [r0, #1032]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #32]
	ldr	r1, [r0, #1024]
	ldr	r3, [r0, #1056]
	ldrb	ip, [r0, #1008]	@ zero_extendqisi2
	ldrb	lr, [r0, #1040]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #36]
	ldr	r1, [r0, #1016]
	ldrb	lr, [r0, #1001]	@ zero_extendqisi2
	ldrb	r3, [r0, #1049]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1033]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #40]
	ldr	r1, [r0, #1024]
	ldrb	lr, [r0, #1009]	@ zero_extendqisi2
	ldrb	r3, [r0, #1057]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1041]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #44]
	ldr	r1, [r0, #1032]
	ldrb	lr, [r0, #1002]	@ zero_extendqisi2
	ldrb	r3, [r0, #1050]	@ zero_extendqisi2
	and	r1, r1, #16711680
	ldrb	ip, [r0, #1018]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #48]
	ldrb	lr, [r0, #1010]	@ zero_extendqisi2
	ldr	r1, [r0, #1040]
	ldrb	r3, [r0, #1058]	@ zero_extendqisi2
	ldrb	ip, [r0, #1026]	@ zero_extendqisi2
	and	r1, r1, #16711680
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #52]
	ldr	r1, [r0, #1048]
	ldrb	lr, [r0, #1003]	@ zero_extendqisi2
	ldrb	r3, [r0, #1035]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1019]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #56]
	ldr	r1, [r0, #1056]
	ldrb	ip, [r0, #1011]	@ zero_extendqisi2
	ldrb	r3, [r0, #1043]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	r0, [r0, #1027]	@ zero_extendqisi2
	orr	r1, ip, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, r0, asl #8
	str	r3, [r2, #60]
	ldmfd	sp, {r4, fp, sp, pc}
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
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	lr, r0, #12
	add	r0, r1, r0, lsl #6
	add	lr, r1, lr, lsl #6
	ldr	r1, [r0, #1388]
	ldr	r3, [r0, #1404]
	ldrb	ip, [lr, #612]	@ zero_extendqisi2
	ldrb	r4, [r0, #1396]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, r4, asl #16
	orr	r3, r3, r1
	str	r3, [r2]
	ldr	r1, [r0, #1420]
	ldr	r3, [r0, #1436]
	ldrb	ip, [r0, #1412]	@ zero_extendqisi2
	ldrb	r4, [r0, #1428]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, r4, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #4]
	ldr	r1, [r0, #1388]
	ldrb	r4, [lr, #613]	@ zero_extendqisi2
	ldrb	r3, [r0, #1405]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1397]	@ zero_extendqisi2
	orr	r1, r4, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #8]
	ldr	r1, [r0, #1420]
	ldrb	r4, [r0, #1413]	@ zero_extendqisi2
	ldrb	r3, [r0, #1437]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1429]	@ zero_extendqisi2
	orr	r1, r4, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #12]
	ldr	ip, [r0, #1396]
	ldrb	r4, [lr, #614]	@ zero_extendqisi2
	ldrb	r3, [r0, #1406]	@ zero_extendqisi2
	and	ip, ip, #16711680
	ldrb	r1, [r0, #1390]	@ zero_extendqisi2
	orr	ip, r4, ip
	orr	r3, ip, r3, asl #24
	orr	r3, r3, r1, asl #8
	str	r3, [r2, #16]
	ldr	ip, [r0, #1428]
	ldrb	r4, [r0, #1414]	@ zero_extendqisi2
	ldrb	r3, [r0, #1438]	@ zero_extendqisi2
	and	ip, ip, #16711680
	ldrb	r1, [r0, #1422]	@ zero_extendqisi2
	orr	ip, r4, ip
	orr	r3, ip, r3, asl #24
	orr	r3, r3, r1, asl #8
	str	r3, [r2, #20]
	ldr	r1, [r0, #1404]
	ldrb	lr, [lr, #615]	@ zero_extendqisi2
	ldrb	r3, [r0, #1399]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1391]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #24]
	ldr	r1, [r0, #1436]
	ldrb	lr, [r0, #1415]	@ zero_extendqisi2
	ldrb	r3, [r0, #1431]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1423]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #28]
	ldr	r1, [r0, #1392]
	ldr	r3, [r0, #1408]
	ldrb	ip, [r0, #1384]	@ zero_extendqisi2
	ldrb	lr, [r0, #1400]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #32]
	ldr	r1, [r0, #1424]
	ldr	r3, [r0, #1440]
	ldrb	ip, [r0, #1416]	@ zero_extendqisi2
	ldrb	lr, [r0, #1432]	@ zero_extendqisi2
	mov	r1, r1, asl #8
	orr	r3, ip, r3, asl #24
	uxth	r1, r1
	orr	r3, r3, lr, asl #16
	orr	r3, r3, r1
	str	r3, [r2, #36]
	ldr	r1, [r0, #1392]
	ldrb	lr, [r0, #1385]	@ zero_extendqisi2
	ldrb	r3, [r0, #1409]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1401]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #40]
	ldr	r1, [r0, #1424]
	ldrb	lr, [r0, #1417]	@ zero_extendqisi2
	ldrb	r3, [r0, #1441]	@ zero_extendqisi2
	and	r1, r1, #65280
	ldrb	ip, [r0, #1433]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #16
	str	r3, [r2, #44]
	ldr	r1, [r0, #1400]
	ldrb	lr, [r0, #1386]	@ zero_extendqisi2
	ldrb	r3, [r0, #1410]	@ zero_extendqisi2
	and	r1, r1, #16711680
	ldrb	ip, [r0, #1394]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #48]
	ldrb	lr, [r0, #1418]	@ zero_extendqisi2
	ldr	r1, [r0, #1432]
	ldrb	r3, [r0, #1442]	@ zero_extendqisi2
	ldrb	ip, [r0, #1426]	@ zero_extendqisi2
	and	r1, r1, #16711680
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #24
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #52]
	ldr	r1, [r0, #1408]
	ldrb	lr, [r0, #1387]	@ zero_extendqisi2
	ldrb	r3, [r0, #1403]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	ip, [r0, #1395]	@ zero_extendqisi2
	orr	r1, lr, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, ip, asl #8
	str	r3, [r2, #56]
	ldr	r1, [r0, #1440]
	ldrb	ip, [r0, #1419]	@ zero_extendqisi2
	ldrb	r3, [r0, #1435]	@ zero_extendqisi2
	and	r1, r1, #-16777216
	ldrb	r0, [r0, #1427]	@ zero_extendqisi2
	orr	r1, ip, r1
	orr	r3, r1, r3, asl #16
	orr	r3, r3, r0, asl #8
	str	r3, [r2, #60]
	ldmfd	sp, {r4, fp, sp, pc}
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
	ldr	ip, [r0, #2872]
	ldrb	r3, [ip, #13]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L467
	ldr	r3, [ip, #72]
	ldr	r7, [ip, #68]
	ldrb	r4, [ip, #15]	@ zero_extendqisi2
	mov	r2, r3
	str	r3, [fp, #-236]
	ldr	r3, [r0, #2868]
	cmp	r2, #0
	sub	r8, r2, #1
	sub	r2, r7, #1
	str	r2, [fp, #-240]
	ldr	r5, [r3, #72]
	ldr	r1, [r3, #148]
	ble	.L438
	ldr	r3, [fp, #-236]
	add	r2, ip, #214
	sub	r0, fp, #216
	add	lr, r3, #107
	add	lr, ip, lr, lsl #1
.L437:
	ldrsh	r3, [r2, #2]!
	cmp	r2, lr
	sub	r3, r3, #1
	str	r3, [r0, #4]!
	bne	.L437
.L438:
	cmp	r7, #0
	ble	.L436
	add	r3, r7, #87
	add	r2, ip, #174
	sub	r0, fp, #128
	add	ip, ip, r3, lsl #1
.L441:
	ldrsh	r3, [r2, #2]!
	cmp	r2, ip
	sub	r3, r3, #1
	str	r3, [r0, #4]!
	bne	.L441
.L436:
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
	beq	.L479
	cmp	r8, #0
	mov	r3, #0
	blt	.L458
	sub	r10, fp, #212
	mov	r4, r3
	mov	r6, r3
	mov	r8, r3
	str	r3, [fp, #-232]
	str	r3, [fp, #-244]
.L444:
	ldr	r2, [fp, #-240]
	cmp	r2, #0
	blt	.L461
	ldr	r8, [r10]
	mov	r2, #0
	ldr	r3, [fp, #-232]
	mov	r1, r2
	add	r8, r8, #1
	sub	lr, fp, #128
	mov	r6, r4
	add	r9, r3, r8
	b	.L447
.L459:
	mov	r6, r4
.L447:
	ldr	r3, [lr, #4]!
	add	r1, r1, #1
	add	r3, r3, #1
	add	ip, r2, r3
	cmp	r5, ip
	mla	r4, r8, r3, r4
	bge	.L445
	cmp	r0, r9
	blt	.L477
.L445:
	cmp	r1, r7
	mov	r2, ip
	bne	.L459
.L450:
	add	r2, ip, r3
	cmp	r5, r2
	bge	.L460
	cmp	r0, r9
	blt	.L480
.L460:
	ldr	r2, [fp, #-244]
	add	r10, r10, #4
	ldr	r1, [fp, #-236]
	add	r2, r2, #1
	str	r9, [fp, #-232]
	cmp	r2, r1
	str	r2, [fp, #-244]
	bne	.L444
	rsb	r0, r9, r0
	mov	r2, ip
	rsb	r2, r2, r5
	mul	r3, r0, r3
	add	r3, r2, r3
	add	r6, r3, r6
.L467:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L479:
	cmp	r8, #0
	mov	r3, r4
	blt	.L458
	sub	r10, fp, #212
	mov	lr, r4
	mov	r6, r4
	mov	r8, r4
	str	r4, [fp, #-232]
	str	r4, [fp, #-244]
.L443:
	ldr	r2, [fp, #-240]
	cmp	r2, #0
	blt	.L464
	ldr	r8, [r10]
	mov	r2, #0
	ldr	r3, [fp, #-232]
	mov	ip, r2
	add	r8, r8, #1
	sub	r4, fp, #128
	mov	r6, lr
	add	r9, r3, r8
	b	.L453
.L462:
	mov	r6, lr
.L453:
	ldr	r3, [r4, #4]!
	add	ip, ip, #1
	add	r3, r3, #1
	add	r1, r2, r3
	cmp	r5, r1
	mla	lr, r8, r3, lr
	bge	.L451
	cmp	r0, r9
	blt	.L452
.L451:
	cmp	ip, r7
	mov	r2, r1
	bne	.L462
.L456:
	add	r2, r1, r3
	cmp	r5, r2
	bge	.L476
	cmp	r0, r9
	blt	.L481
.L476:
	ldr	r2, [fp, #-244]
	add	r10, r10, #4
	ldr	ip, [fp, #-236]
	add	r2, r2, #1
	str	r9, [fp, #-232]
	cmp	r2, ip
	str	r2, [fp, #-244]
	bne	.L443
	rsb	r0, r9, r0
	mov	r2, r1
	mul	r3, r0, r3
	b	.L442
.L477:
	ldr	r1, [fp, #-232]
	rsb	r0, r1, r0
	mul	r3, r0, r3
.L442:
	rsb	r2, r2, r5
	add	r3, r2, r3
	add	r6, r3, r6
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L452:
	cmp	r1, r5
	bgt	.L477
	mov	r1, r2
	b	.L476
.L461:
	ldr	r2, [fp, #-232]
	mov	ip, #0
	add	r9, r2, r8
	b	.L450
.L464:
	ldr	r2, [fp, #-232]
	mov	r1, #0
	add	r9, r2, r8
	b	.L456
.L480:
	ldr	r2, [fp, #-232]
	rsb	r0, r2, r0
	mov	r2, ip
	mul	r3, r0, r3
	b	.L442
.L481:
	ldr	r2, [fp, #-232]
	rsb	r0, r2, r0
	mov	r2, r1
	mul	r3, r0, r3
	b	.L442
.L458:
	mov	r2, r3
	mov	r6, r3
	b	.L442
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
	ldr	lr, [r0, #2872]
	ldrb	r3, [lr, #13]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r0, r1
	beq	.L506
	ldr	r3, [lr, #72]
	ldr	r8, [lr, #68]
	mov	r2, r3
	str	r3, [fp, #-232]
	ldr	r3, [r0, #2868]
	cmp	r2, #0
	ldr	r4, [r3, #72]
	ldr	ip, [r3, #148]
	ble	.L488
	ldr	r3, [fp, #-232]
	add	r2, lr, #214
	sub	r1, fp, #216
	add	r0, r3, #107
	add	r0, lr, r0, lsl #1
.L487:
	ldrsh	r3, [r2, #2]!
	cmp	r2, r0
	sub	r3, r3, #1
	str	r3, [r1, #4]!
	bne	.L487
.L488:
	cmp	r8, #0
	ble	.L486
	add	r0, r8, #87
	add	r2, lr, #174
	sub	r1, fp, #128
	add	r0, lr, r0, lsl #1
.L491:
	ldrsh	r3, [r2, #2]!
	cmp	r2, r0
	sub	r3, r3, #1
	str	r3, [r1, #4]!
	bne	.L491
.L486:
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
	ble	.L490
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
.L492:
	cmp	r8, #0
	ble	.L502
	ldr	r6, [r0]
	ldr	r4, [fp, #-236]
	add	r6, r6, #1
	mla	ip, r6, r4, r3
	cmp	r7, ip
	blt	.L503
	mov	r5, #0
	mov	r1, r3
	mov	r2, r5
	mov	r3, ip
	sub	lr, fp, #124
	b	.L497
.L494:
	ldr	r4, [lr, #4]!
	mov	r1, r3
	add	r4, r4, #1
	mla	ip, r6, r4, r3
	cmp	ip, r7
	bgt	.L495
	mov	r3, ip
.L497:
	add	r2, r2, #1
	add	r5, r5, r4
	cmp	r2, r8
	bne	.L494
.L496:
	cmp	r3, r7
	bgt	.L501
	ldr	r2, [fp, #-232]
	add	r10, r10, #1
	add	r9, r9, r6
	add	r0, r0, #4
	cmp	r10, r2
	bne	.L492
.L501:
	mov	r3, r1
.L495:
	cmp	r4, #0
	beq	.L490
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
.L506:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L502:
	mov	r5, #0
	b	.L496
.L503:
	ldr	r4, [fp, #-236]
	mov	r5, #0
	b	.L495
.L490:
	ldr	r1, .L508
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L506
.L509:
	.align	2
.L508:
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
	ldr	r4, .L533
	mov	r5, r0
	str	r0, [fp, #-60]
	mov	r2, #147456
	mov	r1, #0
	ldr	r3, [r4, #48]
	add	r10, r5, #24576
	ldr	r0, .L533+4
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #36864
	mov	r1, #0
	ldr	r0, .L533+8
	blx	r3
	add	r0, r5, #64768
	ldr	r3, [r4, #48]
	mov	r2, #880
	mov	r1, #0
	add	r0, r0, #148
	blx	r3
	add	r2, r5, #61440
	mov	r3, #0
	str	r2, [fp, #-56]
	str	r3, [r2, #3472]
	ldr	r9, [r10, #2872]
	ldr	r1, [r10, #2868]
	ldr	r2, [r9, #72]
	ldr	r8, [r1, #240]
	cmp	r2, r3
	strne	r3, [fp, #-72]
	strne	r3, [fp, #-76]
	beq	.L510
.L518:
	ldr	r2, [r9, #68]
	cmp	r2, #0
	ldr	r2, [fp, #-72]
	add	r2, r2, #108
	streq	r2, [fp, #-80]
	beq	.L519
	mov	r1, r2
	str	r2, [fp, #-80]
	mov	r7, r1, asl #1
	ldr	r1, [fp, #-76]
	mov	r2, #0
	str	r2, [fp, #-48]
	str	r2, [fp, #-52]
	mul	r1, r8, r1
	ldr	r2, [fp, #-56]
	ldr	r2, [r2, #3472]
	str	r1, [fp, #-68]
.L517:
	add	r1, r2, #8064
	ldr	r0, [fp, #-60]
	add	r1, r1, #50
	add	r1, r0, r1, lsl #3
	str	r3, [r1, #4]
	ldrsh	r1, [r9, r7]
	cmp	r1, #0
	ldreq	r1, [fp, #-48]
	addeq	r1, r1, #88
	streq	r1, [fp, #-64]
	beq	.L513
	ldr	r2, [fp, #-48]
	mov	r5, #0
	ldr	r1, [fp, #-52]
	add	r2, r2, #88
	ldr	r0, [fp, #-68]
	str	r2, [fp, #-64]
	add	r6, r1, r0
	mov	ip, r2, asl #1
.L514:
	ldrsh	r2, [r9, ip]
	cmp	r2, #0
	beq	.L516
	ldr	r2, .L533+4
	rsb	lr, r3, r6
	mov	r4, r3
	add	r2, r2, r3, lsl #2
.L515:
	add	r1, lr, r3
	str	r1, [r2], #4
	ldr	r9, [r10, #2872]
	add	r3, r3, #1
	rsb	r1, r4, r3
	ldrsh	r0, [r9, ip]
	cmp	r0, r1
	bhi	.L515
.L516:
	ldrsh	r2, [r9, r7]
	add	r5, r5, #1
	add	r6, r6, r8
	cmp	r2, r5
	bhi	.L514
	ldr	r2, [fp, #-56]
	ldr	r2, [r2, #3472]
.L513:
	ldr	r1, [fp, #-60]
	sub	ip, r3, #1
	ldr	r0, [fp, #-64]
	add	r1, r1, r2, lsl #3
	ldr	lr, [fp, #-48]
	add	r1, r1, #64768
	add	r2, r2, #1
	mov	r0, r0, asl #1
	add	lr, lr, #1
	str	ip, [r1, #152]
	ldr	r1, [fp, #-56]
	str	lr, [fp, #-48]
	str	r2, [r1, #3472]
	ldr	r1, [r9, #68]
	ldrsh	r0, [r9, r0]
	cmp	r1, lr
	ldr	r1, [fp, #-52]
	add	r1, r1, r0
	str	r1, [fp, #-52]
	bhi	.L517
.L519:
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
	bhi	.L518
.L510:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L534:
	.align	2
.L533:
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
	ldr	r1, .L558
	mov	r0, #2
	ldr	r3, [r7, #404]
	add	r4, r5, #61440
	ldr	r2, [r7, #580]
	add	r8, r5, #24576
	bl	dprint_vfmw
	ldr	r1, .L558+4
	mov	r0, #4
	ldr	r6, .L558+8
	bl	dprint_vfmw
	ldr	r3, [r7, #564]
	add	ip, r5, #65536
	mov	lr, #0
	add	r1, r5, #2960
	add	r0, r5, #62208
	str	r3, [r4, #1964]
	add	r1, r1, #8
	ldr	r3, [r7, #468]
	mov	r2, #64
	ldr	r9, [r7, #472]
	add	r0, r0, #28
	rsb	r9, r3, r9
	str	r9, [r4, #1968]
	ldr	r9, [r7, #580]
	str	r9, [r4, #1956]
	ldr	r9, [r7, #404]
	str	r9, [r4, #1960]
	ldr	r9, [r7, #568]
	str	r9, [r4, #1972]
	ldr	r9, [r7, #572]
	str	r9, [r4, #1976]
	ldr	r9, [r7, #576]
	str	r9, [r4, #1980]
	ldr	r9, [r7, #492]
	rsb	r3, r3, r9
	str	r3, [r4, #1984]
	ldr	r3, [r7, #496]
	rsb	r3, r9, r3
	str	r3, [r4, #1988]
	str	lr, [r5, #208]
	ldr	r3, [ip, #1872]
	str	r3, [r4, #3244]
	ldr	r3, [r8, #2868]
	ldrb	ip, [r3, #12]	@ zero_extendqisi2
	strb	ip, [r4, #676]
	ldrb	ip, [r3, #5]	@ zero_extendqisi2
	strb	ip, [r4, #677]
	ldrb	ip, [r3, #9]	@ zero_extendqisi2
	strb	ip, [r4, #678]
	ldrb	ip, [r3, #8]	@ zero_extendqisi2
	strb	ip, [r4, #679]
	ldrb	ip, [r3, #7]	@ zero_extendqisi2
	strb	ip, [r4, #680]
	ldr	ip, [r3, #64]
	str	ip, [r4, #684]
	ldr	ip, [r3, #116]
	strb	ip, [r4, #688]
	ldr	ip, [r3, #244]
	sub	ip, ip, #1
	str	ip, [r4, #692]
	ldr	ip, [r3, #240]
	sub	ip, ip, #1
	str	ip, [r4, #696]
	ldr	ip, [r7, #376]
	str	ip, [r4, #700]
	ldr	ip, [r8, #2872]
	ldr	lr, [ip, #84]
	sub	lr, lr, #2
	str	lr, [r4, #704]
	ldrb	lr, [ip, #11]	@ zero_extendqisi2
	strb	lr, [r4, #708]
	ldrb	ip, [ip, #10]	@ zero_extendqisi2
	strb	ip, [r4, #709]
	ldr	ip, [r3, #200]
	str	ip, [r4, #712]
	ldr	ip, [r3, #208]
	str	ip, [r4, #716]
	ldr	ip, [r3, #188]
	str	ip, [r4, #720]
	ldr	ip, [r3, #196]
	str	ip, [r4, #724]
	ldr	ip, [r3, #172]
	str	ip, [r4, #728]
	ldr	ip, [r3, #180]
	str	ip, [r4, #732]
	ldr	ip, [r3, #216]
	str	ip, [r4, #736]
	ldr	ip, [r3, #140]
	str	ip, [r4, #740]
	ldr	lr, [r3, #148]
	ldr	ip, [r6, #52]
	str	lr, [r4, #744]
	ldr	lr, [r3, #112]
	str	lr, [r4, #748]
	ldr	lr, [r3, #104]
	str	lr, [r4, #752]
	ldr	lr, [r3, #124]
	str	lr, [r4, #756]
	ldr	lr, [r3, #120]
	str	lr, [r4, #760]
	ldr	lr, [r3, #108]
	str	lr, [r4, #764]
	ldr	r3, [r3, #100]
	str	r3, [r4, #768]
	blx	ip
	ldr	ip, [r8, #2868]
	add	r3, r5, #64512
	add	r2, r5, #64256
	add	r1, r5, #63744
	add	r3, r3, #12
	ldr	lr, [ip, #76]
	add	r2, r2, #188
	add	r1, r1, #188
	mov	r0, r5
	str	lr, [r4, #860]
	ldr	ip, [ip, #72]
	str	ip, [r4, #864]
	bl	HEVC_CalcTileInfo
	subs	r6, r0, #0
	bne	.L556
	ldr	r3, [r8, #2872]
	ldrb	r2, [r3, #16]	@ zero_extendqisi2
	strb	r2, [r4, #888]
	ldrb	r2, [r3, #7]	@ zero_extendqisi2
	strb	r2, [r4, #889]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	strb	r2, [r4, #890]
	ldrb	r2, [r3, #14]	@ zero_extendqisi2
	strb	r2, [r4, #891]
	ldrb	r2, [r3, #15]	@ zero_extendqisi2
	strb	r2, [r4, #892]
	ldrb	r2, [r3, #13]	@ zero_extendqisi2
	strb	r2, [r4, #893]
	ldr	r2, [r8, #2868]
	ldr	r1, [r3, #56]
	ldr	r2, [r2, #148]
	rsb	r2, r1, r2
	str	r2, [r3, #88]
	ldr	r3, [r8, #2872]
	ldr	r2, [r3, #88]
	str	r2, [r4, #896]
	ldr	r2, [r3, #52]
	str	r2, [r4, #900]
	ldrb	r2, [r3, #8]	@ zero_extendqisi2
	strb	r2, [r4, #904]
	ldrb	r2, [r3, #12]	@ zero_extendqisi2
	strb	r2, [r4, #905]
	ldrb	r2, [r3, #6]	@ zero_extendqisi2
	strb	r2, [r4, #906]
	ldr	r2, [r3, #60]
	str	r2, [r4, #908]
	ldr	r2, [r3, #64]
	str	r2, [r4, #912]
	ldr	r2, [r3, #72]
	sub	r2, r2, #1
	str	r2, [r4, #916]
	ldr	r2, [r3, #68]
	sub	r2, r2, #1
	str	r2, [r4, #920]
	ldrb	r2, [r3, #10]	@ zero_extendqisi2
	strb	r2, [r4, #709]
	ldrb	r3, [r3, #11]	@ zero_extendqisi2
	strb	r3, [r4, #708]
	ldr	r3, [r5, #220]
	str	r6, [r5, #236]
	cmp	r3, #1
	beq	.L547
	ldr	r3, [r7, #420]
	ldr	r0, [r5, #276]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	subs	r8, r0, #0
	beq	.L557
	ldr	r2, [r8, #640]
	ldr	r3, [r8, #644]
	cmp	r2, #0
	beq	.L540
	cmp	r3, #0
	beq	.L540
	add	r3, r5, #63488
	add	r2, r5, #63232
	add	r2, r2, #220
	ldr	r0, [r5, #276]
	mov	r1, r3
	add	r3, r3, #184
	add	r1, r1, #92
	bl	FSP_GetDecFsAddrTab
	ldr	r3, [r8, #640]
	add	r2, r5, #63488
	ldr	r3, [r3, #8]
	mov	r1, r2
	add	r2, r2, #96
	add	r1, r1, #176
	str	r3, [r4, #1956]
	ldr	r0, [r5, #276]
	bl	FSP_GetPmvAddrTab
	ldr	r3, [r7, #404]
	str	r3, [r4, #1960]
	ldr	r3, [r8, #656]
	str	r3, [r4, #2228]
	ldr	r3, [r8, #644]
	ldr	r3, [r3, #16]
	str	r3, [r4, #3464]
	ldr	r3, [r8, #644]
	ldr	r3, [r3, #76]
	str	r3, [r4, #3468]
	ldr	ip, [r5, #2832]
	cmp	ip, #0
	str	ip, [r4, #3236]
	beq	.L542
	add	r2, r5, #2896
	add	r3, r5, #64512
	add	r2, r2, #4
	add	r3, r3, #96
	mov	r0, r6
.L543:
	add	r0, r0, #1
	ldr	r1, [r2, #4]!
	cmp	r0, ip
	str	r1, [r3, #4]!
	bne	.L543
.L542:
	add	r8, r5, #62208
	mov	r6, #0
	add	r8, r8, #160
.L544:
	mov	r2, r8
	mov	r0, r6
	mov	r1, r5
	add	r6, r6, #1
	bl	HEVC_WriteQmatrix_8x8
	cmp	r6, #4
	add	r8, r8, #64
	bne	.L544
	add	r2, r5, #62464
	mov	r0, r6
	add	r2, r2, #160
	mov	r1, r5
	bl	HEVC_WriteQmatrix_8x8
	add	r2, r5, #62464
	add	r2, r2, #224
	mov	r1, r5
	mov	r0, #5
	add	r8, r5, #62720
	bl	HEVC_WriteQmatrix_8x8
	add	r2, r8, #32
	mov	r1, r5
	mov	r0, #0
	bl	HEVC_WriteQmatrix_16x16
	add	r2, r5, #62720
	add	r2, r2, #96
	mov	r1, r5
	mov	r0, #1
	add	r8, r8, #160
	mov	r6, #2
	bl	HEVC_WriteQmatrix_16x16
.L545:
	mov	r2, r8
	mov	r0, r6
	mov	r1, r5
	add	r6, r6, #1
	bl	HEVC_WriteQmatrix_16x16
	cmp	r6, #6
	add	r8, r8, #64
	bne	.L545
	add	r2, r5, #62976
	mov	r1, r5
	add	r2, r2, #160
	mov	r0, #0
	bl	HEVC_WriteQmatrix_32x32
	add	r2, r5, #62976
	add	r2, r2, #224
	mov	r1, r5
	mov	r0, #1
	add	r8, r5, #63232
	bl	HEVC_WriteQmatrix_32x32
	add	r8, r8, #32
	mov	r6, #0
.L546:
	mov	r2, r8
	mov	r0, r6
	mov	r1, r5
	add	r6, r6, #1
	bl	HEVC_WriteQmatrix_4x4
	cmp	r6, #6
	add	r8, r8, #16
	bne	.L546
	ldr	r1, [r5, #2300]
	mov	r0, #0
	ldr	r3, [r5, #2304]
	ldr	r2, [r5, #2292]
	ldr	ip, [r5, #2296]
	mov	r1, r1, asl #16
	orr	r3, r1, r3, asl #24
	orr	r3, r3, r2
	orr	r3, r3, ip, asl #8
	str	r3, [r4, #1920]
	ldr	r1, [r5, #2316]
	ldr	r3, [r5, #2320]
	ldr	r2, [r5, #2308]
	ldr	ip, [r5, #2312]
	mov	r1, r1, asl #16
	orr	r3, r1, r3, asl #24
	orr	r3, r3, r2
	orr	r3, r3, ip, asl #8
	str	r3, [r4, #1924]
	ldr	r3, [r7, #404]
	str	r3, [r4, #1952]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L547:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L556:
	ldr	r2, .L558+12
	mov	r0, #1
	ldr	r1, .L558+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L540:
	ldr	r1, .L558+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L557:
	ldr	r3, [r7, #420]
	mov	r0, #1
	ldr	r1, .L558+24
	ldr	r2, [r3, #32]
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L559:
	.align	2
.L558:
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
	ldrb	r3, [r3, #1051]	@ zero_extendqisi2
	cmp	r3, #0
	addeq	r3, r0, #54272
	addeq	r3, r3, #24
	beq	.L562
	ldrsb	r3, [r0, #9]
	cmp	r3, #0
	addne	r3, r0, #57856
	addne	r3, r3, #172
	beq	.L595
.L562:
	ldr	ip, [r3, #160]
	ldr	lr, [r3, #164]
	cmp	ip, #16
	bhi	.L596
	cmp	lr, #16
	bhi	.L566
	cmp	ip, #0
	beq	.L567
	ldr	r3, [r0, #2368]
	cmp	r3, #0
	beq	.L580
	ldr	r3, [r3, #32]
	cmp	r3, #0
	addne	r1, r0, #2368
	movne	r2, #0
	bne	.L572
	b	.L597
.L573:
	ldr	r3, [r1, #4]!
	cmp	r3, #0
	beq	.L568
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L569
.L572:
	add	r2, r2, #1
	cmp	r2, ip
	bne	.L573
.L567:
	cmp	lr, #0
	beq	.L592
	ldr	r3, [r0, #2436]
	cmp	r3, #0
	beq	.L583
	ldr	r3, [r3, #32]
	cmp	r3, #0
	addne	r0, r0, #2432
	movne	r2, #0
	addne	r0, r0, #4
	bne	.L578
	b	.L598
.L579:
	ldr	r3, [r0, #4]!
	cmp	r3, #0
	beq	.L574
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L575
.L578:
	add	r2, r2, #1
	cmp	r2, lr
	bne	.L579
.L592:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L580:
	mov	r2, r3
.L568:
	ldr	r1, .L599
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L597:
	mov	r2, r3
.L569:
	ldr	r1, .L599+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L583:
	mov	r2, r3
.L574:
	ldr	r1, .L599+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L598:
	mov	r2, r3
.L575:
	ldr	r1, .L599+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L595:
	mov	r0, r3
	ldr	r2, .L599+16
	ldr	r1, .L599+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L566:
	mov	r2, lr
	mov	r3, #17
	ldr	r1, .L599+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L596:
	mov	r2, ip
	mov	r3, #17
	ldr	r1, .L599+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L600:
	.align	2
.L599:
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
	bne	.L640
	ldr	r0, [r4, #208]
	ldr	r3, [r4, #144]
	cmp	r0, r3
	bcs	.L676
	add	r9, r4, #65536
	mov	r5, #1360
	ldr	r7, [fp, #-112]
	mov	r2, r5
	ldr	r6, [r9, #1872]
	add	r8, r4, #53248
	ldr	r3, .L680
	mov	r1, r7
	mla	r6, r5, r0, r6
	ldr	r3, [r3, #48]
	mov	r0, r6
	blx	r3
	ldr	r3, .L680
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
	ldrcc	r2, [r9, #1872]
	ldrcs	r3, [fp, #-112]
	mlacc	r3, r5, r3, r2
	str	r3, [r6, #1356]
	ldr	r3, [r4, #248]
	ldr	r0, [r8, #1124]
	add	r1, r3, #16640
	add	r2, r3, #1
	add	r1, r1, #214
	str	r0, [r4, r1, asl #2]
	ldr	r3, [r4, #172]
	str	r2, [r4, #248]
	add	r3, r3, #1
	str	r3, [r4, #172]
	ldr	r2, [r9, #2680]
	mov	r0, r3, asl #5
	ldr	r1, [r8, #1108]
	sub	r3, r0, r3, asl #2
	str	r3, [fp, #-116]
	add	r3, r2, r3
	str	r1, [r3, #8]
	ldr	r2, [r4, #2364]
	ldrb	r1, [r2, #1]	@ zero_extendqisi2
	ldr	r3, [r2, #8]
	cmp	r1, #1
	cmpls	r1, r3
	bcs	.L606
	add	r0, r6, r1, lsl #2
.L607:
	mov	r3, r1, asl #5
	sub	r3, r3, r1, asl #2
	add	r1, r1, #1
	add	r2, r2, r3
	ldr	r2, [r2, #60]
	str	r2, [r0, #8]
	ldr	r2, [r4, #2364]
	add	r2, r2, r3
	ldr	r2, [r2, #56]
	str	r2, [r0], #4
	ldr	r2, [r4, #2364]
	add	r3, r2, r3
	ldr	r3, [r3, #64]
	str	r3, [r0, #12]
	ldr	r2, [r4, #2364]
	ldr	r3, [r2, #8]
	cmp	r3, r1
	movhi	ip, #1
	movls	ip, #0
	cmp	r1, #1
	movhi	ip, #0
	cmp	ip, #0
	bne	.L607
.L606:
	cmp	r3, #1
	addls	r3, r6, r3, lsl #2
	addls	r1, r6, #8
	movls	r2, #0
	bhi	.L612
.L611:
	str	r2, [r3, #8]
	str	r2, [r3], #4
	cmp	r3, r1
	str	r2, [r3, #12]
	bne	.L611
.L612:
	ldrb	r3, [r8, #1051]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L677
.L610:
	add	r5, r4, #57344
	ldr	r3, [r5, #844]
	cmp	r3, #0
	beq	.L613
	add	r0, r4, #2368
	add	r10, r6, #112
	add	lr, r6, #368
	mov	r1, #0
.L614:
	ldr	ip, [r0]
	sub	r7, fp, #44
	add	r1, r1, #1
	ldr	r2, [ip, #32]
	ldr	r3, [r2, #24]
	ldr	r2, [r2, #28]
	add	r3, r3, #15872
	add	r3, r3, #22
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
	ldr	r3, [r5, #844]
	cmp	r3, r1
	bhi	.L614
	ldr	r2, [r5, #848]
	cmp	r2, #0
	beq	.L615
.L639:
	add	r1, r4, #2432
	add	r10, r6, #176
	add	r1, r1, #4
	add	lr, r6, #432
	mov	r0, #0
.L616:
	ldr	ip, [r1]
	sub	r7, fp, #44
	add	r0, r0, #1
	ldr	r2, [ip, #32]
	ldr	r3, [r2, #24]
	ldr	r2, [r2, #28]
	add	r3, r3, #15872
	add	r3, r3, #22
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
	ldr	r2, [r5, #848]
	cmp	r2, r0
	bhi	.L616
	ldr	r3, [r5, #844]
.L615:
	cmp	r3, #0
	beq	.L617
	ldr	r1, [r6, #372]
	ldr	lr, [r8, #376]
	add	r1, r1, #740
	add	r1, r1, #2
	ldr	r1, [r4, r1, asl #2]
	cmp	lr, r1
	blt	.L646
	add	ip, r6, #372
	mov	r0, #0
	b	.L619
.L620:
	ldr	r1, [ip, #4]!
	add	r1, r1, #740
	add	r1, r1, #2
	ldr	r1, [r4, r1, asl #2]
	cmp	r1, lr
	bgt	.L646
.L619:
	add	r0, r0, #1
	cmp	r0, r3
	bne	.L620
.L617:
	ldr	r1, [r5, #744]
	cmp	r1, #0
	bne	.L674
	cmp	r2, #0
	beq	.L674
	ldr	r3, [r6, #436]
	ldr	ip, [r8, #376]
	add	r3, r3, #740
	add	r3, r3, #2
	ldr	r3, [r4, r3, asl #2]
	cmp	ip, r3
	blt	.L618
	add	r0, r6, #436
	b	.L621
.L622:
	ldr	r3, [r0, #4]!
	add	r3, r3, #740
	add	r3, r3, #2
	ldr	r3, [r4, r3, asl #2]
	cmp	r3, ip
	bgt	.L646
.L621:
	add	r1, r1, #1
	cmp	r1, r2
	bne	.L622
.L674:
	mov	r1, #1
.L618:
	strb	r1, [r6, #24]
	ldrb	r3, [r8, #1051]	@ zero_extendqisi2
	strb	r3, [r6, #25]
	ldr	r3, [r5, #740]
	str	r3, [r6, #28]
	ldrb	r3, [r5, #694]	@ zero_extendqisi2
	strb	r3, [r6, #32]
	ldr	r3, [r5, #764]
	str	r3, [r6, #36]
	ldr	r3, [r5, #744]
	cmp	r3, #0
	bne	.L623
	ldrb	r3, [r5, #695]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L624
	ldr	r3, [r5, #796]
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #436]
	mov	r7, r3
.L625:
	str	r7, [r6, #40]
	add	r10, r4, #24576
	ldr	r3, [r5, #796]
	str	r3, [r6, #48]
	ldrb	r3, [r5, #695]	@ zero_extendqisi2
	strb	r3, [r6, #52]
	ldr	r3, [r5, #800]
	str	r3, [r6, #44]
	ldr	r3, [r5, #852]
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r6, #60]
	ldr	r3, [r5, #856]
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r6, #56]
	ldrb	r2, [r5, #691]	@ zero_extendqisi2
	strb	r2, [r6, #64]
	ldrb	r2, [r5, #693]	@ zero_extendqisi2
	strb	r2, [r6, #65]
	ldr	r2, [r5, #844]
	str	r2, [r6, #68]
	ldr	r2, [r5, #848]
	str	r2, [r6, #72]
	ldr	r2, [r5, #744]
	str	r2, [r6, #76]
	ldr	r2, [r8, #1128]
	str	r2, [r6, #80]
	ldr	r2, [r10, #2868]
	ldr	r0, [r8, #1128]
	ldr	r1, [r2, #240]
	bl	__aeabi_uidivmod
	str	r1, [r6, #84]
	ldr	r2, [r10, #2868]
	ldr	r0, [r8, #1128]
	ldr	r1, [r2, #240]
	bl	__aeabi_uidiv
	str	r0, [r6, #88]
	mov	r0, r4
	ldr	r1, [r8, #1128]
	bl	HEVC_CtbRsAddrToTsAddr
	str	r0, [r6, #92]
	ldr	r2, [r5, #808]
	str	r2, [r6, #96]
	ldr	r2, [r5, #812]
	str	r2, [r6, #100]
	ldr	r2, [r5, #820]
	str	r2, [r6, #104]
	ldr	r2, [r5, #816]
	str	r2, [r6, #108]
	ldrb	r2, [r5, #690]	@ zero_extendqisi2
	strb	r2, [r6, #112]
	ldrb	r2, [r5, #689]	@ zero_extendqisi2
	strb	r2, [r6, #113]
	ldrb	r2, [r5, #697]	@ zero_extendqisi2
	strb	r2, [r6, #114]
	ldrb	r2, [r5, #698]	@ zero_extendqisi2
	strb	r2, [r6, #115]
	ldr	r3, [r10, #2872]
	ldrb	r2, [r3, #10]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L630
	ldr	r2, [r5, #744]
	cmp	r2, #1
	beq	.L631
.L630:
	ldrb	r3, [r3, #11]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L678
.L633:
	ldrb	r3, [r5, #691]	@ zero_extendqisi2
	cmp	r3, #0
	subne	r3, fp, #44
	movne	r2, r7
	addne	r10, r3, r2, lsl #2
	ldrne	r3, [r10, #-64]
	strne	r3, [r6, #564]
	ldr	r3, [r4, #220]
	cmp	r3, #0
	bne	.L636
	ldr	r2, [r9, #2680]
	ldr	r0, [fp, #-116]
	ldr	r1, [r4, #2364]
	add	r2, r2, r0
	str	r1, [r2, #24]
	str	r3, [r4, #2364]
.L636:
	add	r1, r4, #61440
	ldrb	r3, [r1, #893]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L655
	ldr	r3, [r4, #248]
	cmp	r3, #1
	beq	.L679
	add	r0, r4, r3, lsl #3
	ldr	ip, [r6, #92]
	add	r0, r0, #65536
	add	r2, r3, #8192
	add	r2, r2, #31
	sub	ip, ip, #1
	str	ip, [r0, #248]
	sub	r3, r3, #1
	ldr	r0, [r6, #92]
	add	r2, r4, r2, lsl #3
	str	r0, [r2, #4]
.L638:
	ldr	r2, [r1, #692]
	add	r3, r4, r3, lsl #3
	ldr	r1, [r1, #696]
	add	r3, r3, #65536
	mla	r2, r1, r2, r2
	add	r2, r2, r1
	str	r2, [r3, #264]
	ldr	r1, [r6, #92]
	sub	r1, r1, #1
	cmp	r1, #36864
	bcs	.L655
	mov	r0, r4
	bl	HEVC_CtbTsAddrToRsAddr
	str	r0, [r6, #568]
.L655:
	ldr	r0, [fp, #-112]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L646:
	mov	r1, #0
	b	.L618
.L623:
	cmp	r3, #1
	movne	r3, #0
	movne	r7, r3
	bne	.L625
.L624:
	ldr	r3, [r5, #796]
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #372]
	mov	r7, r3
	b	.L625
.L677:
	ldr	r3, .L680
	add	r1, r4, #54272
	add	r0, r4, #57856
	add	r1, r1, #24
	add	r0, r0, #172
	movw	r2, #3732
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #1
	strb	r3, [r4, #9]
	b	.L610
.L678:
	ldr	r3, [r5, #744]
	cmp	r3, #0
	bne	.L633
.L631:
	ldr	ip, [r5, #2276]
	add	r1, r4, #60416
	ldr	r3, .L680
	add	r1, r1, #240
	ldr	r8, .L680
	mov	r2, #64
	str	ip, [r6, #580]
	add	r0, r6, #588
	ldr	ip, [r5, #3308]
	ldr	r3, [r3, #52]
	str	ip, [r6, #584]
	blx	r3
	add	r1, r4, #59648
	add	r1, r1, #172
	ldr	r3, [r8, #52]
	mov	r2, #64
	add	r0, r6, #716
	blx	r3
	add	r1, r4, #60672
	add	r1, r1, #48
	ldr	r3, [r8, #52]
	mov	r2, #128
	add	r0, r6, #844
	blx	r3
	add	r1, r4, #60672
	add	r0, r6, #1088
	ldr	r3, [r8, #52]
	add	r1, r1, #176
	add	r0, r0, #12
	mov	r2, #128
	blx	r3
	ldr	r3, [r5, #744]
	cmp	r3, #0
	bne	.L633
	add	r1, r4, #60928
	ldr	r3, [r8, #52]
	add	r1, r1, #48
	mov	r2, #64
	add	r0, r6, #652
	blx	r3
	add	r1, r4, #60160
	ldr	r3, [r8, #52]
	add	r1, r1, #172
	mov	r2, #64
	add	r0, r6, #780
	blx	r3
	add	r1, r4, #60928
	ldr	r3, [r8, #52]
	add	r1, r1, #112
	mov	r2, #128
	add	r0, r6, #972
	blx	r3
	add	r1, r4, #60928
	add	r0, r6, #1216
	ldr	r3, [r8, #52]
	add	r1, r1, #240
	mov	r2, #128
	add	r0, r0, #12
	blx	r3
	b	.L633
.L679:
	ldr	r2, [r6, #92]
	mov	r3, #0
	str	r2, [r9, #260]
	b	.L638
.L613:
	ldr	r3, [r5, #848]
	cmp	r3, #0
	bne	.L639
	b	.L674
.L640:
	mvn	r3, #0
	str	r3, [fp, #-112]
	b	.L655
.L676:
	mov	r2, r0
	ldr	r1, .L680+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	str	r3, [fp, #-112]
	b	.L655
.L681:
	.align	2
.L680:
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
	ldr	r3, [ip, #396]
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r2, #2008]
	ldr	r3, [ip, #400]
	str	r3, [r2, #2000]
	ldr	r3, [r1, #208]
	str	r3, [r2, #3240]
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
	ldr	r3, [r3, #1124]
	ldr	r2, [r0, #156]
	cmp	r3, #0
	blt	.L687
	cmp	r3, r2
	bge	.L687
	ldr	r2, [r0, #2352]
	movw	r1, #2024
	mla	r3, r1, r3, r2
	ldr	r2, [r0, #152]
	ldr	r3, [r3, #32]
	cmp	r3, #0
	blt	.L691
	cmp	r3, r2
	bge	.L691
	ldr	r2, [r0, #2348]
	movw	r1, #26164
	add	r0, r0, #57344
	mov	r9, #1
	str	r0, [fp, #-52]
	mla	r3, r1, r3, r2
	ldr	r2, [r0, #236]
	ldr	r1, [r0, #240]
	ldr	ip, [r0, #244]
	add	r10, r1, r2
	str	r2, [fp, #-60]
	cmp	r10, ip
	str	r1, [fp, #-64]
	ldr	r3, [r3, #128]
	mov	r4, r10
	mov	r9, r9, asl r3
	bcs	.L702
	cmp	r10, #15
	bgt	.L694
	ldr	r3, [fp, #-48]
	add	r5, r10, #14400
	add	r5, r5, #25
	str	r10, [fp, #-68]
	str	r6, [fp, #-56]
	ldr	r3, [r3, #376]
	add	r5, r6, r5, lsl #2
	mov	r6, ip
	mov	r10, r3
	b	.L696
.L714:
	cmp	r6, r4
	bls	.L712
	cmp	r4, #16
	beq	.L713
.L696:
	ldr	r7, [r5, #4]!
	mov	r1, r9
	mov	r0, r10
	mov	r8, r10
	ldr	r2, [r5, #-1884]
	cmp	r2, #0
	bne	.L697
	bl	__aeabi_idivmod
	mov	r0, r7
	mov	r8, r1
	mov	r1, r9
	bl	__aeabi_idivmod
	mov	r7, r1
.L697:
	cmp	r8, r7
	add	r4, r4, #1
	bne	.L714
	ldr	r10, [fp, #-68]
	mov	r4, #1
	ldr	r6, [fp, #-56]
.L693:
	cmp	r10, #0
	beq	.L689
	ldr	r2, [fp, #-48]
	ldr	r3, [fp, #-52]
	ldr	ip, [r2, #1096]
	ldr	r3, [r3, #292]
	ldr	r0, [r2, #376]
	add	r3, ip, r3
	cmp	r0, r3
	beq	.L706
	add	r6, r6, #57600
	mov	r1, #0
	add	r6, r6, #36
	b	.L699
.L700:
	ldr	r3, [r6, #4]!
	add	r3, ip, r3
	cmp	r3, r0
	beq	.L706
.L699:
	add	r1, r1, #1
	cmp	r1, r10
	beq	.L689
	cmp	r1, #17
	bne	.L700
	ldr	r3, [fp, #-64]
	mov	r0, #0
	ldr	r2, [fp, #-60]
	str	r1, [sp]
	str	r3, [sp, #8]
	mov	r3, r1
	str	r2, [sp, #4]
	movw	r2, #6983
	ldr	r1, .L715
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L691:
	str	r2, [sp]
	mov	r0, #0
	movw	r2, #6934
	ldr	r1, .L715+4
	mov	r4, r0
	bl	dprint_vfmw
.L689:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L687:
	str	r2, [sp]
	mov	r0, #0
	movw	r2, #6922
	ldr	r1, .L715+8
	mov	r4, r0
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L713:
	mov	ip, r6
	ldr	r6, [fp, #-56]
.L694:
	mov	r3, r4
	mov	r1, #16
	movw	r2, #6954
	str	r1, [sp]
	mov	r0, #0
	str	ip, [sp, #4]
	ldr	r1, .L715+12
	mov	r4, r0
	bl	dprint_vfmw
	ldr	r3, [fp, #-52]
	ldr	r2, [r3, #236]
	ldr	r3, [r3, #240]
	str	r2, [fp, #-60]
	add	r10, r3, r2
	str	r3, [fp, #-64]
	b	.L693
.L712:
	ldr	r10, [fp, #-68]
	mov	r4, #0
	ldr	r6, [fp, #-56]
	b	.L693
.L706:
	mov	r4, #1
	b	.L689
.L702:
	mov	r4, #0
	b	.L693
.L716:
	.align	2
.L715:
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
	ldr	r3, [r3, #1108]
	cmp	r3, #1
	beq	.L734
	cmp	r3, #2
	beq	.L735
	cmp	r3, #0
	moveq	r3, #2
	streq	r3, [fp, #-48]
	beq	.L719
	ldr	r2, .L744
	mov	r0, #1
	ldr	r1, .L744+4
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L734:
	str	r3, [fp, #-48]
.L719:
	add	r8, ip, #54272
	add	r9, ip, #2368
	add	r8, r8, #180
	mov	r10, #0
.L732:
	ldr	r1, [r8, #4]!
	cmp	r1, #0
	beq	.L722
	ldr	r3, [r9]
	cmp	r3, #0
	bne	.L736
	mov	r2, r9
	mov	r4, r3
	b	.L724
.L726:
	ldr	r6, [r2, #4]!
	cmp	r6, #0
	bne	.L742
.L724:
	add	r4, r4, #1
	cmp	r4, r1
	bne	.L726
	mov	r4, #255
.L725:
	ldr	r2, [ip, #2356]
	ldr	r2, [r2, #12]
	cmp	r2, #2
	bne	.L727
	ldr	r2, [fp, #-56]
	ldr	r6, [r2, #420]
	add	r6, r6, #36
.L723:
	mov	r7, r9
	mov	r5, #0
	b	.L733
.L729:
	ldr	r3, [r7, #4]!
.L733:
	cmp	r3, #0
	beq	.L743
.L728:
	ldr	r3, [r8]
	add	r5, r5, #1
	cmp	r3, r5
	bhi	.L729
.L730:
	ldr	r3, [fp, #-48]
	add	r10, r10, #1
	add	r9, r9, #68
	cmp	r10, r3
	bcc	.L732
.L735:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L722:
	ldr	r3, [ip, #2356]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L730
	mov	r3, #1
	str	r3, [r8]
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #420]
	add	r3, r3, #36
	str	r3, [r9]
	b	.L730
.L743:
	sxtb	r2, r4
	str	r5, [sp]
	str	r2, [sp, #4]
	mov	r3, r10
	ldr	r2, .L744
	mov	r0, #1
	ldr	r1, .L744+8
	str	ip, [fp, #-52]
	bl	dprint_vfmw
	str	r6, [r7]
	ldr	ip, [fp, #-52]
	b	.L728
.L742:
	uxtb	r4, r4
	cmp	r4, #255
	bne	.L723
	b	.L725
.L736:
	mov	r6, r3
	mov	r4, #0
	b	.L723
.L727:
	mov	r3, r10
	ldr	r2, .L744
	ldr	r1, .L744+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L745:
	.align	2
.L744:
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
	beq	.L782
	ldr	r3, .L814
	mov	r2, #136
	mov	r1, #0
	sub	r0, fp, #180
	ldr	r3, [r3, #48]
	blx	r3
	ldr	lr, [r7, #2552]
	cmp	lr, #0
	beq	.L783
	mov	r0, #0
	add	r1, r7, #2624
	mov	r3, r0
	b	.L750
.L749:
	cmp	r3, lr
	beq	.L748
.L750:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #1]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L749
	ldr	ip, [r2, #52]
	cmp	ip, #0
	bne	.L749
	ldr	ip, [r2, #56]
	cmp	ip, #1
	subeq	ip, fp, #44
	addeq	r2, r2, #36
	addeq	ip, ip, r0, lsl #2
	addeq	r0, r0, #1
	streq	r2, [ip, #-136]
	cmp	r3, lr
	bne	.L750
.L748:
	ldr	r4, [r7, #2556]
	cmp	r4, #0
	beq	.L784
	add	r1, r7, #2688
	mov	ip, r0
	add	r1, r1, #4
	mov	r3, #0
	b	.L753
.L752:
	cmp	r3, r4
	beq	.L810
.L753:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	lr, [r2, #1]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L752
	ldr	lr, [r2, #52]
	cmp	lr, #0
	bne	.L752
	ldr	lr, [r2, #56]
	cmp	lr, #1
	subeq	lr, fp, #44
	addeq	r2, r2, #36
	addeq	lr, lr, ip, lsl #2
	addeq	ip, ip, #1
	streq	r2, [lr, #-136]
	cmp	r3, r4
	bne	.L753
.L810:
	rsb	r4, r0, ip
.L751:
	ldr	r5, [r7, #2548]
	cmp	r5, #0
	beq	.L785
	add	lr, r7, #2752
	mov	r9, ip
	add	lr, lr, #8
	mov	r2, #0
	b	.L756
.L755:
	cmp	r2, r5
	beq	.L811
.L756:
	ldr	r3, [lr, #4]!
	add	r2, r2, #1
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L755
	ldr	r1, [r3, #52]
	cmp	r1, #1
	bne	.L755
	ldr	r1, [r3, #56]
	sub	r6, fp, #44
	add	r6, r6, r9, lsl #2
	add	r3, r3, #36
	cmp	r1, #0
	addeq	r9, r9, #1
	streq	r3, [r6, #-136]
	cmp	r2, r5
	bne	.L756
.L811:
	rsb	r5, r0, r9
	rsb	r5, r4, r5
.L754:
	cmp	r9, #0
	beq	.L812
	add	r3, r7, #53248
	str	r3, [fp, #-184]
	ldr	r3, [r3, #1108]
	cmp	r3, #0
	str	r3, [fp, #-188]
	beq	.L813
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1216]
	ldr	r5, [r3, #1220]
	cmp	r4, #0
	str	r4, [r3, #1208]
	str	r5, [r3, #1212]
	beq	.L775
.L781:
	add	r1, r7, #57344
	add	r2, r7, #2352
	add	r8, r7, #57600
	add	r2, r2, #12
	ldrb	r10, [r1, #424]	@ zero_extendqisi2
	add	r8, r8, #172
	str	r5, [fp, #-192]
	mov	r6, #0
	mov	r5, r10
	mov	r10, r4
	mov	r4, r2
.L768:
	cmp	r5, #0
	mov	r1, r9
	mov	r0, r6
	ldrne	r1, [r8]
	bne	.L767
	bl	__aeabi_uidivmod
.L767:
	sub	r3, fp, #44
	add	r6, r6, #1
	add	r3, r3, r1, lsl #2
	cmp	r10, r6
	add	r8, r8, #4
	ldr	r3, [r3, #-136]
	str	r3, [r4, #4]!
	bhi	.L768
	ldr	r3, [fp, #-188]
	ldr	r5, [fp, #-192]
	cmp	r3, #0
	bne	.L809
.L769:
	cmp	r5, #0
	beq	.L771
	add	r2, r7, #57344
	add	r6, r7, #57856
	add	r6, r6, #44
	add	r8, r7, #2432
	ldrb	r10, [r2, #425]	@ zero_extendqisi2
	mov	r4, #0
.L774:
	cmp	r10, #0
	mov	r1, r9
	mov	r0, r4
	ldrne	r1, [r6]
	bne	.L773
	bl	__aeabi_uidivmod
.L773:
	sub	r3, fp, #44
	add	r4, r4, #1
	add	r3, r3, r1, lsl #2
	cmp	r4, r5
	add	r6, r6, #4
	ldr	r3, [r3, #-68]
	str	r3, [r8, #4]!
	bne	.L774
.L809:
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1208]
	ldr	r5, [r3, #1212]
.L770:
	cmp	r4, #16
	bhi	.L779
.L775:
	add	r4, r4, #588
	add	r2, r7, #2432
	add	r4, r4, #3
	add	r4, r7, r4, lsl #2
.L778:
	ldr	r3, [r7, #2368]
	str	r3, [r4, #4]!
	cmp	r4, r2
	bne	.L778
.L779:
	cmp	r5, #16
	bhi	.L777
	add	r5, r5, #608
	add	r2, r7, #2496
	add	r2, r2, #4
	add	r5, r7, r5, lsl #2
.L780:
	ldr	r3, [r7, #2436]
	str	r3, [r5, #4]!
	cmp	r5, r2
	bne	.L780
.L777:
	mov	r0, #0
.L808:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L813:
	cmp	r4, #0
	beq	.L760
	sub	r3, fp, #180
	ldr	r2, [fp, #-188]
	add	lr, r3, r0, lsl #2
	sub	r1, fp, #116
.L761:
	add	r2, r2, #1
	ldr	r3, [lr], #4
	cmp	r2, r4
	str	r3, [r1, #4]!
	bne	.L761
.L760:
	cmp	r0, #0
	beq	.L762
	add	r1, r4, #16
	sub	lr, fp, #180
	mov	r2, #0
	add	r1, lr, r1, lsl #2
.L763:
	add	r2, r2, #1
	ldr	r3, [lr], #4
	cmp	r2, r0
	str	r3, [r1, #4]!
	bne	.L763
	add	r4, r4, r2
.L762:
	cmp	r5, #0
	beq	.L764
	add	r4, r4, #16
	sub	r3, fp, #180
	add	ip, r3, ip, lsl #2
	mov	r2, #0
	add	r4, r3, r4, lsl #2
.L765:
	add	r2, r2, #1
	ldr	r3, [ip], #4
	cmp	r2, r5
	str	r3, [r4, #4]!
	bne	.L765
.L764:
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1216]
	ldr	r5, [r3, #1220]
	cmp	r4, #0
	str	r4, [r3, #1208]
	str	r5, [r3, #1212]
	bne	.L781
	b	.L769
.L812:
	ldr	r3, .L814
	add	r4, r7, #53248
	mov	r2, #136
	mov	r1, r9
	str	r9, [r4, #1208]
	add	r0, r7, #2368
	ldr	ip, [r3, #48]
	str	r9, [r4, #1212]
	blx	ip
	ldr	r2, [r7, #2356]
	ldr	r2, [r2, #12]
	cmp	r2, #2
	bne	.L777
	ldr	r2, [r4, #420]
	mov	r3, #1
	mov	r0, r9
	str	r3, [r4, #1208]
	add	r3, r2, #36
	str	r3, [r7, #2368]
	b	.L808
.L783:
	mov	r0, lr
	b	.L748
.L784:
	mov	ip, r0
	b	.L751
.L785:
	mov	r9, ip
	b	.L754
.L771:
	ldr	r3, .L814
	add	r0, r7, #2432
	add	r0, r0, #4
	mov	r2, #68
	add	r1, r7, #2368
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r3, [fp, #-184]
	ldr	r4, [r3, #1208]
	str	r4, [r3, #1212]
	mov	r5, r4
	b	.L770
.L782:
	mvn	r0, #0
	b	.L808
.L815:
	.align	2
.L814:
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
	ldr	r2, [r3, #1108]
	cmp	r2, #2
	beq	.L821
	bl	HEVC_InitListX
	cmp	r0, #0
	bne	.L822
	mov	r0, r4
	bl	HEVC_RepairList
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, .L823
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L821:
	mov	r0, #0
	str	r0, [r3, #1208]
	str	r0, [r3, #1212]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L822:
	ldr	r1, .L823+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L824:
	.align	2
.L823:
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
	ldr	r2, [r5, #2560]
	ldr	r3, [r2, #28]
	cmp	r3, #16
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r3, r3, #708
	mov	ip, #0
	add	r3, r3, #2
	ldr	r1, .L830
	mov	r0, #13
	str	ip, [r4, r3, asl #2]
	ldr	r3, [r2, #28]
	add	r3, r3, #724
	add	r3, r3, #2
	str	ip, [r4, r3, asl #2]
	ldr	r3, [r2, #28]
	add	r3, r3, #740
	add	r3, r3, #2
	str	ip, [r4, r3, asl #2]
	ldr	r3, [r2, #24]
	add	r3, r3, #536
	add	r3, r4, r3, lsl #2
	str	ip, [r3, #4]
	ldr	r3, [r2, #24]
	ldr	r2, [r2, #28]
	bl	dprint_vfmw
	ldr	r3, [r5, #2560]
	mov	r2, #16
	str	r2, [r3, #28]
	ldr	r3, [r4, #2836]
	sub	r3, r3, #1
	str	r3, [r4, #2836]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L831:
	.align	2
.L830:
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
	ldr	r3, [r0, #2540]
	mov	r6, r0
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, r0, #2544
	mov	r4, #0
	add	r5, r5, #12
	b	.L835
.L834:
	ldr	r3, [r6, #2540]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L838
.L835:
	ldr	r3, [r5, #4]!
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L834
	mov	r1, r4
	mov	r0, r6
	bl	HEVC_RemoveApcStore
	ldr	r3, [r6, #2540]
	add	r4, r4, #1
	cmp	r4, r3
	bcc	.L835
.L838:
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
	ldr	r2, [r0, #2352]
	movw	r4, #2024
	ldr	r1, [r3, #1124]
	movw	ip, #26164
	str	r3, [fp, #-68]
	mov	r9, r0
	ldr	r3, [r0, #2348]
	mov	r0, #1
	mla	r2, r4, r1, r2
	ldr	lr, [r9, #2540]
	cmp	lr, #0
	ldr	r2, [r2, #32]
	mla	r3, ip, r2, r3
	ldr	r8, [r3, #128]
	mov	r3, r0, asl r8
	str	r3, [fp, #-48]
	beq	.L839
	add	r8, r9, #2560
	add	r3, r9, #57600
	add	r7, r9, #57344
	str	r9, [fp, #-56]
	mov	r9, r8
	mov	r2, #0
	add	r3, r3, #36
	str	r2, [fp, #-60]
	str	r3, [fp, #-72]
	str	r0, [fp, #-64]
.L856:
	ldr	r0, [r7, #236]
	ldr	r2, [r7, #240]
	ldr	r1, [r7, #244]
	add	r2, r2, r0
	ldr	r5, [r9]
	cmp	r2, r1
	mov	r8, r2
	movcs	r4, #0
	ldrcs	r0, [r5, #12]
	bcs	.L842
	add	r6, r2, #14400
	ldr	r3, [fp, #-56]
	add	r6, r6, #26
	ldr	r10, [r5, #12]
	mov	r4, #0
	add	r6, r3, r6, lsl #2
.L848:
	ldr	r3, [r6, #-1884]
	cmp	r3, #0
	ldr	r3, [r5, #52]
	beq	.L843
	cmp	r3, #0
	beq	.L844
	ldr	r3, [r6]
	cmp	r3, r10
	beq	.L874
.L844:
	cmp	r4, #0
	bne	.L861
	ldr	r1, [r6]
	mov	r4, r10
	b	.L858
.L843:
	cmp	r3, #0
	beq	.L846
	ldr	r1, [fp, #-48]
	mov	r0, r10
	bl	__aeabi_idivmod
	ldr	r0, [r6]
	str	r1, [fp, #-52]
	ldr	r1, [fp, #-48]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-52]
	cmp	r3, r1
	beq	.L874
.L846:
	cmp	r4, #0
	bne	.L861
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
.L858:
	ldrb	r0, [r5, #1]	@ zero_extendqisi2
	rsb	r4, r1, r4
	clz	r4, r4
	cmp	r0, #0
	mov	r4, r4, lsr #5
	moveq	r4, #0
	cmp	r4, #0
	bne	.L875
.L861:
	mov	r0, r10
.L847:
	ldr	r3, [r7, #244]
	add	r8, r8, #1
	add	r6, r6, #4
	mov	r10, r0
	cmp	r8, r3
	bcc	.L848
	ldr	r2, [r7, #240]
	ldr	r1, [r7, #236]
	add	r2, r2, r1
.L842:
	ldr	r3, [fp, #-68]
	cmp	r2, #0
	ldrne	r8, [fp, #-72]
	movne	r6, #0
	ldr	ip, [r3, #1096]
	beq	.L850
.L852:
	ldr	r3, [r5, #52]
	add	r6, r6, #1
	cmp	r3, #0
	bne	.L851
	ldr	r3, [r8]
	add	r3, ip, r3
	cmp	r3, r0
	beq	.L876
.L851:
	ldr	r3, [r7, #240]
	add	r8, r8, #4
	ldr	r2, [r7, #236]
	add	r3, r3, r2
	cmp	r6, r3
	bcc	.L852
.L850:
	eor	r4, r4, #1
	cmp	r0, ip
	moveq	r0, #0
	andne	r0, r4, #1
	cmp	r0, #0
	beq	.L854
	mov	r3, #0
	strb	r3, [r5, #1]
	ldr	r2, [r9]
	str	r3, [r2, #52]
	ldr	r2, [r9]
	ldr	r1, [r2, #8]
	cmp	r1, #1
	beq	.L854
	ldr	r1, [r2, #32]
	mov	r2, r3
	ldr	r3, [fp, #-56]
	ldr	r0, [r3, #276]
	bl	FSP_SetRef
.L854:
	ldr	r3, [fp, #-56]
	add	r9, r9, #4
	ldr	r2, [r3, #2540]
	ldr	r3, [fp, #-60]
	add	r3, r3, #1
	str	r3, [fp, #-60]
	cmp	r3, r2
	bcc	.L856
.L839:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L875:
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
	ldr	r0, [r1, #276]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r5, [r9]
	ldr	r0, [r5, #12]
	b	.L847
.L874:
	ldr	r1, [fp, #-64]
	mov	r2, #1
	mov	r4, #1
	strb	r1, [r5, #1]
	ldr	r3, [r9]
	str	r1, [r3, #4]
	ldr	r1, [fp, #-56]
	ldr	r3, [r9]
	ldr	r0, [r1, #276]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r5, [r9]
	ldr	r0, [r5, #12]
	b	.L847
.L876:
	ldr	r1, [fp, #-64]
	mov	r2, #1
	mov	r4, r2
	strb	r1, [r5, #1]
	ldr	r3, [r9]
	str	r1, [r3, #4]
	ldr	r1, [fp, #-56]
	ldr	r3, [r9]
	ldr	r0, [r1, #276]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r5, [r9]
	ldr	r3, [fp, #-68]
	ldr	r0, [r5, #12]
	ldr	ip, [r3, #1096]
	b	.L851
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
	ldr	ip, [r0, #2352]
	movw	r6, #2024
	ldr	r2, [r3, #1124]
	add	lr, r0, #57344
	str	r1, [fp, #-84]
	movw	r5, #26164
	ldr	r1, [lr, #236]
	mov	r7, r0
	mla	ip, r6, r2, ip
	ldr	r6, [lr, #240]
	ldr	r4, [r0, #2348]
	add	r1, r6, r1
	ldr	r2, [lr, #244]
	str	r1, [fp, #-80]
	mov	lr, r1
	cmp	r1, r2
	ldr	ip, [ip, #32]
	str	r0, [fp, #-60]
	mov	r0, #1
	mla	r1, r5, ip, r4
	ldr	r10, [r1, #128]
	mov	r10, r0, asl r10
	bcs	.L878
	add	r9, lr, #14400
	ldr	r6, [r7, #2540]
	add	r9, r9, #26
	mov	r0, #0
	add	r2, r2, #57600
	add	r1, lr, #57600
	add	r9, r7, r9, lsl #2
	mov	ip, r0
	add	r2, r2, #17
	add	r1, r1, #17
	str	r0, [fp, #-64]
	mov	r0, r7
	add	r2, r7, r2
	str	ip, [fp, #-72]
	str	r2, [fp, #-68]
	add	r2, r7, r1
	str	r3, [fp, #-76]
	str	r2, [fp, #-56]
.L879:
	cmp	r6, #0
	beq	.L885
	ldr	r3, [fp, #-60]
	mov	r2, #0
	ldr	r8, [r9, #-1884]
	mov	r5, r2
	add	r4, r3, #2560
	str	r2, [fp, #-52]
	b	.L882
.L922:
	ldr	r3, [r4]
	ldr	r1, [r3, #52]
	cmp	r1, #0
	beq	.L881
	ldr	r0, [r3, #12]
	ldr	r1, [r9]
	cmp	r0, r1
	ldreqb	r3, [r3, #1]	@ zero_extendqisi2
	beq	.L920
.L881:
	add	r5, r5, #1
	add	r4, r4, #4
	cmp	r5, r6
	beq	.L921
.L882:
	cmp	r8, #0
	bne	.L922
	ldr	r7, [r4]
	mov	r1, r10
	ldr	r3, [r7, #52]
	cmp	r3, #0
	beq	.L881
	ldr	r0, [r7, #12]
	bl	__aeabi_idivmod
	ldr	r0, [r9]
	str	r1, [fp, #-48]
	mov	r1, r10
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-48]
	cmp	r3, r1
	bne	.L881
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
.L920:
	cmp	r3, #0
	add	r5, r5, #1
	ldr	r3, [fp, #-52]
	add	r4, r4, #4
	movne	r3, #1
	cmp	r5, r6
	str	r3, [fp, #-52]
	bne	.L882
.L921:
	ldr	r2, [fp, #-52]
	cmp	r2, #0
	beq	.L885
.L883:
	ldr	r3, [fp, #-56]
	add	r9, r9, #4
	ldr	r2, [fp, #-68]
	add	r3, r3, #1
	str	r3, [fp, #-56]
	cmp	r3, r2
	bne	.L879
	ldr	r2, [fp, #-80]
	ldr	r3, [fp, #-76]
	cmp	r2, #0
	beq	.L887
.L886:
	ldr	r2, [fp, #-60]
	mov	r7, #0
	ldr	r0, [fp, #-72]
	add	r4, r2, #57600
	ldr	r10, [fp, #-80]
	mov	r9, r2
	mov	r8, r4
	add	r4, r4, #36
	add	r8, r8, #17
.L891:
	cmp	r6, #0
	beq	.L892
	add	lr, r9, #2544
	mov	r5, #0
	add	lr, lr, #12
	mov	r2, r5
	str	r0, [fp, #-48]
	b	.L889
.L888:
	cmp	r2, r6
	beq	.L923
.L889:
	ldr	r1, [lr, #4]!
	add	r2, r2, #1
	ldr	r0, [r1, #52]
	cmp	r0, #0
	bne	.L888
	ldr	ip, [r4]
	ldr	r0, [r3, #1096]
	add	r0, r0, ip
	ldr	ip, [r1, #12]
	cmp	ip, r0
	bne	.L888
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	cmp	r1, #0
	movne	r5, #1
	cmp	r2, r6
	bne	.L889
.L923:
	cmp	r5, #0
	ldr	r0, [fp, #-48]
	beq	.L892
.L890:
	add	r7, r7, #1
	add	r4, r4, #4
	cmp	r7, r10
	add	r8, r8, #1
	bcc	.L891
	str	r0, [fp, #-72]
.L887:
	ldr	r3, [fp, #-72]
	cmp	r3, #0
	ldrne	r3, [fp, #-84]
	movne	r0, #0
	ldrne	r2, [fp, #-64]
	moveq	r0, #1
	strne	r2, [r3]
.L919:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L892:
	ldr	r2, [r3, #1096]
	ldr	ip, [r4]
	ldr	r1, [r9, #200]
	add	r2, r2, ip
	cmp	r2, r1
	blt	.L890
	ldrb	r1, [r8]	@ zero_extendqisi2
	cmp	r1, #0
	ldr	r1, [fp, #-64]
	movne	r0, #1
	movne	r1, r2
	str	r1, [fp, #-64]
	b	.L890
.L885:
	ldr	r3, [fp, #-76]
	ldr	r0, [r9, #-68]
	ldr	r2, [r3, #1096]
	ldr	r3, [fp, #-60]
	add	r2, r2, r0
	ldr	r1, [r3, #200]
	cmp	r2, r1
	blt	.L883
	ldr	r3, [fp, #-56]
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, [fp, #-64]
	cmp	r1, #0
	movne	r3, r2
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-72]
	movne	r3, #1
	str	r3, [fp, #-72]
	b	.L883
.L878:
	ldr	r2, [fp, #-80]
	cmp	r2, #0
	beq	.L919
	ldr	r1, [fp, #-60]
	mov	r2, #0
	str	r2, [fp, #-64]
	str	r2, [fp, #-72]
	ldr	r6, [r1, #2540]
	b	.L886
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
	ldr	r3, [r0, #2356]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r1, #24]
	ldmccfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVC_SetFrmRepeatCount.part.4
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
	add	r2, r2, #640
	add	r4, r4, #384
	mov	r5, #0
.L931:
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne	.L927
	cmp	r2, #0
	beq	.L928
	ldr	lr, [r0, #2540]
	cmp	lr, #0
	beq	.L928
	ldr	r1, [r0, #2560]
	cmp	r1, r2
	beq	.L927
	add	r1, r0, #2560
	b	.L929
.L930:
	ldr	ip, [r1, #4]!
	cmp	ip, r2
	beq	.L927
.L929:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L930
.L928:
	add	r5, r5, #1
.L927:
	add	r2, r2, #704
	cmp	r2, r4
	bne	.L931
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
	ldr	r1, [r10, #236]
	cmp	r1, #0
	beq	.L965
	add	r9, r0, #57600
	mov	r7, #0
	add	ip, r0, #53248
	mov	r6, r7
	mov	r8, r9
	add	r9, r9, #16
	add	r8, r8, #36
.L949:
	ldrb	r3, [r9, #1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L943
	ldr	r4, [r5, #2540]
	ldr	lr, [ip, #1096]
	ldr	r3, [r8]
	cmp	r4, #0
	add	lr, lr, r3
	beq	.L943
	ldr	r1, [r5, #2560]
	ldr	r3, [r1, #12]
	cmp	r3, lr
	addne	r2, r5, #2560
	movne	r3, #0
	bne	.L946
	b	.L997
.L948:
	ldr	r1, [r2, #4]!
	ldr	r0, [r1, #12]
	cmp	r0, lr
	beq	.L944
.L946:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L948
.L943:
	ldr	r1, [r10, #236]
	add	r6, r6, #1
	add	r8, r8, #4
	cmp	r1, r6
	bhi	.L949
.L942:
	ldr	r2, [r5, #2536]
	str	r7, [r5, #2552]
	cmp	r2, r7
	addhi	r3, r7, #656
	movhi	r0, #0
	addhi	r3, r5, r3, lsl #2
	bls	.L954
.L953:
	add	r7, r7, #1
	str	r0, [r3, #4]!
	cmp	r7, r2
	bne	.L953
.L954:
	ldr	r8, [r10, #240]
	add	r8, r1, r8
	cmp	r8, r6
	movls	r7, #0
	bls	.L952
	add	r9, r6, #14400
	add	r10, r6, #57600
	add	r9, r9, #9
	add	r10, r10, #16
	add	r10, r5, r10
	add	ip, r5, #53248
	add	r9, r5, r9, lsl #2
	mov	r7, #0
.L961:
	ldrb	r3, [r10, #1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L955
	ldr	r4, [r5, #2540]
	ldr	lr, [ip, #1096]
	ldr	r3, [r9]
	cmp	r4, #0
	add	lr, lr, r3
	beq	.L955
	ldr	r1, [r5, #2560]
	ldr	r3, [r1, #12]
	cmp	r3, lr
	addne	r2, r5, #2560
	movne	r3, #0
	bne	.L958
	b	.L998
.L960:
	ldr	r1, [r2, #4]!
	ldr	r0, [r1, #12]
	cmp	r0, lr
	beq	.L956
.L958:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L960
.L955:
	add	r6, r6, #1
	add	r9, r9, #4
	cmp	r8, r6
	bhi	.L961
.L1001:
	ldr	r2, [r5, #2536]
.L952:
	cmp	r2, r7
	str	r7, [r5, #2556]
	bls	.L941
	add	r1, r7, #672
	mov	r3, #0
	add	r1, r1, #1
	add	r5, r5, r1, lsl #2
.L963:
	add	r7, r7, #1
	str	r3, [r5, #4]!
	cmp	r7, r2
	bne	.L963
.L941:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L997:
	mov	r3, #0
.L944:
	add	r4, r5, r3, lsl #2
	mov	r2, #1
	strb	r2, [r1, #1]
	ldr	r3, [r4, #2560]
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L999
.L947:
	mov	r2, #1
	str	r2, [r3, #56]
	ldr	r2, [r4, #2560]
	mov	r1, #0
	add	r3, r5, r7, lsl #2
	add	r7, r7, #1
	str	r1, [r2, #52]
	ldr	r2, [r4, #2560]
	str	r2, [r3, #2628]
	b	.L943
.L998:
	mov	r3, #0
.L956:
	add	r4, r5, r3, lsl #2
	mov	r2, #1
	strb	r2, [r1, #1]
	ldr	r3, [r4, #2560]
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L1000
.L959:
	mov	r2, #1
	str	r2, [r3, #56]
	ldr	r2, [r4, #2560]
	add	r3, r7, #672
	mov	r1, #0
	add	r6, r6, #1
	add	r3, r3, #2
	cmp	r8, r6
	str	r1, [r2, #52]
	add	r7, r7, #1
	ldr	r2, [r4, #2560]
	add	r9, r9, #4
	str	r2, [r5, r3, asl #2]
	bhi	.L961
	b	.L1001
.L999:
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #276]
	str	ip, [fp, #-48]
	bl	FSP_SetRef
	ldr	r3, [r4, #2560]
	ldr	ip, [fp, #-48]
	b	.L947
.L1000:
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #276]
	str	ip, [fp, #-48]
	bl	FSP_SetRef
	ldr	r3, [r4, #2560]
	ldr	ip, [fp, #-48]
	b	.L959
.L965:
	mov	r7, r1
	mov	r6, r1
	b	.L942
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
	ldr	r2, [r0, #2352]
	movw	ip, #2024
	ldr	r3, [r3, #1124]
	add	r10, r0, #57344
	ldr	lr, [r0, #2348]
	movw	r4, #26164
	ldr	r5, [r10, #240]
	mov	r1, #1
	mla	ip, ip, r3, r2
	ldr	r2, [r10, #236]
	ldr	r3, [r10, #244]
	add	r2, r2, r5
	sub	r2, r2, #1
	sub	r9, r3, #1
	cmp	r9, r2
	ldr	ip, [ip, #32]
	mla	r2, r4, ip, lr
	ldr	r8, [r2, #128]
	mov	r8, r1, asl r8
	ble	.L1013
	add	r7, r3, #14400
	add	r3, r3, #57600
	add	r7, r7, #25
	add	r3, r3, #17
	add	r3, r0, r3
	sub	r8, r8, #1
	add	r7, r0, r7, lsl #2
	mov	r6, #0
	str	r6, [fp, #-48]
	mov	r6, r3
	str	r1, [fp, #-52]
.L1009:
	ldrb	r3, [r6, #-1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1004
	ldrb	r4, [r7, #-1884]	@ zero_extendqisi2
	ldr	lr, [r7]
	ldr	r5, [r0, #2540]
	cmp	r4, #0
	andeq	lr, lr, r8
	cmp	r5, #0
	ble	.L1004
	add	r1, r0, #2544
	mov	r3, #0
	add	r1, r1, #12
	b	.L1008
.L1007:
	add	r3, r3, #1
	cmp	r3, r5
	beq	.L1004
.L1008:
	ldr	ip, [r1, #4]!
	cmp	r4, #0
	ldr	r2, [ip, #12]
	andeq	r2, r2, r8
	cmp	lr, r2
	bne	.L1007
	ldr	r1, [fp, #-48]
	add	r3, r0, r3, lsl #2
	ldr	lr, [fp, #-52]
	add	r2, r0, r1, lsl #2
	add	r1, r1, #1
	strb	lr, [ip, #1]
	mov	ip, #0
	str	r1, [fp, #-48]
	ldr	r1, [r3, #2560]
	str	ip, [r1, #56]
	ldr	r1, [r3, #2560]
	str	lr, [r1, #52]
	ldr	r3, [r3, #2560]
	str	r3, [r2, #2764]
.L1004:
	ldr	r3, [r10, #236]
	sub	r9, r9, #1
	ldr	r2, [r10, #240]
	sub	r7, r7, #4
	add	r3, r3, r2
	sub	r3, r3, #1
	cmp	r3, r9
	blt	.L1009
	ldr	r6, [fp, #-48]
	mov	r2, r6
.L1003:
	ldr	r3, [r0, #2536]
	str	r2, [r0, #2548]
	cmp	r3, r6
	ble	.L1002
	add	r1, r6, #688
	mov	r2, #0
	add	r1, r1, #2
	add	r0, r0, r1, lsl #2
.L1011:
	add	r6, r6, #1
	str	r2, [r0, #4]!
	cmp	r6, r3
	bne	.L1011
.L1002:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1013:
	mov	r2, #0
	mov	r6, r2
	b	.L1003
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
	beq	.L1021
	ldr	ip, [r0, #2832]
	cmp	ip, #0
	beq	.L1030
	ldr	r4, [r0, #2840]
	cmp	r4, #0
	addne	r3, r0, #2832
	movne	r4, #0
	addne	r3, r3, #8
	bne	.L1025
	b	.L1023
.L1028:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L1023
.L1025:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L1028
.L1030:
	mvn	r0, #6
.L1034:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1023:
	ldr	r1, [r6, #32]
	ldr	r0, [r5, #276]
	bl	FSP_GetLogicFs
	subs	ip, r0, #0
	beq	.L1026
	ldr	r3, [ip, #640]
	cmp	r3, #0
	beq	.L1026
	add	r2, r5, r4, lsl #2
	mov	r0, #1
	mov	r3, r4
	ldr	r1, .L1035
	str	r0, [r2, #2840]
	mov	r0, #13
	ldr	ip, [ip, #640]
	ldr	ip, [ip, #8]
	str	ip, [r2, #2904]
	ldr	ip, [r6, #12]
	str	ip, [r2, #2968]
	ldr	r2, [r5, #2836]
	add	r2, r2, #1
	str	r2, [r5, #2836]
	ldr	r2, [r6, #228]
	bl	dprint_vfmw
	mov	r0, #0
	str	r4, [r6, #28]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1026:
	ldrb	r1, [r6, #1]	@ zero_extendqisi2
	mov	r0, #0
	ldr	r2, [r6, #32]
	ldr	r3, [r6, #12]
	stmia	sp, {r1, ip}
	str	r0, [sp, #8]
	ldr	r1, .L1035+4
	bl	dprint_vfmw
	mvn	r0, #6
	b	.L1034
.L1021:
	mov	r0, r6
	movw	r3, #9276
	ldr	r2, .L1035+8
	ldr	r1, .L1035+12
	bl	dprint_vfmw
	mvn	r0, #6
	b	.L1034
.L1036:
	.align	2
.L1035:
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
	ldr	r0, [r0, #276]
	ldr	r1, [r1, #32]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1038
	ldrsb	r2, [r0, #1]
	cmp	r2, #3
	cmpne	r2, #0
	moveq	r0, #1
	movne	r0, #0
	beq	.L1057
	ldr	r3, [r4, #8]
	cmp	r3, #1
	beq	.L1044
	ldr	r2, [r4, #4]
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1043
	cmp	r3, #1
	beq	.L1043
.L1054:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L1057:
	ldr	r3, [r4, #32]
	mov	r0, #13
	str	r2, [sp]
	ldr	r1, .L1058
	ldr	r2, .L1058+4
	bl	dprint_vfmw
.L1040:
	ldr	r3, [r4, #8]
	cmp	r3, #1
	beq	.L1044
	ldr	r3, [r4, #12]
	mov	r0, #1
	ldr	r2, .L1058+4
	ldr	r1, .L1058+8
	bl	dprint_vfmw
	mvn	r0, #2
	b	.L1054
.L1038:
	ldr	lr, [r4, #32]
	mov	r2, r4
	ldr	ip, [r4, #8]
	mov	r0, #1
	ldr	r3, [r4, #12]
	ldr	r1, .L1058+12
	stmia	sp, {ip, lr}
	bl	dprint_vfmw
	b	.L1040
.L1043:
	str	r3, [sp, #4]
	mov	r0, #1
	str	r2, [sp]
	mov	r3, r4
	ldr	r2, .L1058+4
	ldr	r1, .L1058+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1054
.L1044:
	mov	r2, #1
	mov	r3, #2
	strb	r2, [r4, #2]
	mvn	r0, #2
	str	r3, [r4, #8]
	b	.L1054
.L1059:
	.align	2
.L1058:
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
	ldr	r3, [r5, #2864]
	ldr	lr, [r4, #16]
	add	r3, r3, #12288
	ldr	r2, [r3, #180]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #256
	moveq	ip, #0
	beq	.L1061
	mov	r3, #256
	movt	r3, 1
	cmp	r2, r3
	movne	ip, #1
	bne	.L1061
	add	r3, r0, #4096
	ldr	ip, [r3, #480]
	subs	ip, ip, #1
	movne	ip, #1
.L1061:
	ldr	r3, [r5, #2868]
	ldrb	r3, [r3, #2633]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r2, #3072
	beq	.L1062
	ldr	r3, [r4, #696]
	cmp	r3, #1
	cmpne	r3, #11
	moveq	r2, #1024
	bne	.L1084
.L1063:
	ldr	r3, [r4, #700]
	clz	ip, r3
	mov	ip, ip, lsr #5
.L1062:
	and	r3, lr, #3
	add	lr, r0, #53248
	orr	r1, r3, r1
	orr	r3, r1, r2
	orr	r3, r3, ip, asl #8
	orr	r2, r3, #8192
	str	r2, [r4, #180]
	ldr	r2, [lr, #384]
	cmp	r2, #1920
	bichi	r3, r3, #131072
	orrls	r3, r3, #139264
	orrhi	r3, r3, #8192
	str	r3, [r4, #180]
	ldr	r2, [r0, #2356]
	ldr	r2, [r2, #592]
	cmp	r2, #1
	movne	r1, #0
	beq	.L1085
.L1066:
	ldr	r2, [r4, #4]
	orr	r3, r1, r3
	str	r3, [r4, #180]
	cmp	r2, #0
	ldr	r1, [r4, #32]
	ldrne	r3, [r4, #20]
	strne	r3, [r4, #232]
	ldr	r0, [r0, #276]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L1086
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
.L1084:
	bic	r3, r3, #8
	cmp	r3, #2
	moveq	r2, #2048
	movne	r2, #3072
	b	.L1063
.L1085:
	tst	r3, #131072
	movne	r1, #0
	moveq	r1, #262144
	b	.L1066
.L1087:
	.align	2
.L1086:
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
	ldr	r5, [r6, #420]
	add	r1, r6, #424
	ldr	r3, [r6, #408]
	add	r0, r5, #72
	str	r3, [r5, #20]
	bl	memcpy
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_GetImagePara
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L1110
	ldr	r2, [r5, #20]
	cmp	r2, #0
	str	r2, [r7, #160]
	bne	.L1091
.L1096:
	ldrb	r3, [r6, #368]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1111
.L1093:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_SetDisplay
	add	r3, r4, #61696
	ldr	r0, [r4, #276]
	add	r3, r3, #64
	str	r7, [sp]
	mov	r2, r4
	mov	r1, #16
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L1112
	ldr	r3, [r4, #2356]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r7, #24]
	bcs	.L1113
.L1100:
	ldr	r2, [r4, #316]
	mov	r0, #0
	ldr	r3, [r4, #308]
	add	r2, r2, #1
	str	r2, [r4, #316]
	add	r3, r3, #1
	str	r3, [r4, #308]
.L1090:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1091:
	ldr	r8, [r6, #380]
	cmp	r8, #0
	beq	.L1114
.L1094:
	ldr	r3, [r4, #2356]
	ldr	r3, [r3, #4]
	cmp	r3, r2
	bcs	.L1096
	ldr	r1, [r5, #32]
	mov	r2, #0
	ldr	r0, [r4, #276]
	bl	FSP_SetDisplay
	ldr	r1, [r4, #2356]
	ldr	r3, [r7, #160]
	mov	r0, #1
	ldr	r2, .L1115
	ldr	ip, [r1, #4]
	ldr	r1, .L1115+4
	str	ip, [sp]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1090
.L1111:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_SetRef
	b	.L1093
.L1113:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_SetFrmRepeatCount.part.4
	b	.L1100
.L1114:
	ldr	r2, .L1115
	mov	r0, #1
	ldr	r1, .L1115+8
	bl	dprint_vfmw
	ldr	r3, .L1115+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1095
	mov	r3, r8
	mov	r2, r8
	mov	r1, #111
	ldr	r0, [r4, #276]
	blx	ip
.L1095:
	ldr	r2, [r7, #160]
	cmp	r2, #0
	bne	.L1094
	b	.L1096
.L1112:
	ldr	r2, .L1115
	mov	r0, #1
	ldr	r1, .L1115+16
	bl	dprint_vfmw
	ldr	r0, [r4, #276]
	ldr	r1, [r5, #32]
	mov	r2, #0
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L1090
.L1110:
	ldr	r2, .L1115
	ldr	r1, .L1115+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1090
.L1116:
	.align	2
.L1115:
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
	bne	.L1120
	add	r3, r4, #53248
	add	r1, r4, #61440
	mov	r2, #1
	mov	ip, #2
	ldr	r0, [r3, #420]
	str	ip, [r0, #8]
	mov	r0, r5
	str	r2, [r1, #672]
	ldr	r1, [r3, #376]
	str	r1, [r4, #244]
	ldr	r3, [r3, #420]
	strb	r2, [r3, #2]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1120:
	ldr	r2, .L1121
	mov	r0, #1
	ldr	r1, .L1121+4
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1122:
	.align	2
.L1121:
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
	bhi	.L1125
	mov	r3, #704
	mla	r1, r3, r1, r0
	add	r1, r1, #67584
	ldrb	r0, [r1, #641]	@ zero_extendqisi2
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1125:
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
	ldr	r0, [r0, #276]
	mov	r8, r1
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1131
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	addne	r4, r0, #560
	addne	r9, r0, #592
	movne	r6, #0
	movne	r7, #0
	beq	.L1136
.L1130:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1137
.L1129:
	cmp	r4, r9
	bne	.L1130
	ldr	r0, [r5, #276]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1137:
	mov	r1, r2
	ldr	r0, [r5, #276]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L1129
.L1136:
	ldr	r1, .L1138
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1131:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1139:
	.align	2
.L1138:
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
	ldr	r3, .L1150
	mov	r5, r1
	mov	r6, r2
	mov	r4, r0
	mov	r2, #32
	ldr	r3, [r3, #48]
	sub	r0, fp, #60
	mov	r1, #0
	blx	r3
	ldr	r3, [r4, #2356]
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
	beq	.L1141
	ldr	r2, [r4, #2536]
	mov	r1, #5
	ldrb	r3, [fp, #12]	@ zero_extendqisi2
	strb	r0, [fp, #-56]
	strb	r1, [fp, #-55]
	strb	r2, [fp, #-57]
.L1146:
	sub	r1, fp, #60
	ldr	r0, [r4, #276]
	strb	r3, [fp, #-54]
	mov	r3, #0
	strb	r3, [fp, #-58]
	bl	FSP_ConfigInstance
	cmp	r0, #0
	bne	.L1142
	ldr	r1, [r4, #2356]
	sub	r3, fp, #64
	ldr	r0, [r4, #276]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L1149
	cmp	r0, #0
	moveq	r0, #1
	bne	.L1144
.L1148:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1141:
	mov	r3, #1
	mov	r2, #5
	strb	r0, [fp, #-56]
	strb	r3, [fp, #-57]
	strb	r2, [fp, #-55]
	b	.L1146
.L1149:
	add	r0, r4, #61696
	add	r0, r0, #64
	bl	ResetVoQueue
.L1144:
	ldr	r1, .L1150+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L1148
.L1142:
	ldr	r1, .L1150+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L1148
.L1151:
	.align	2
.L1150:
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
.L1153:
	ldr	r1, [r0, #3436]
	add	ip, ip, #1
	cmp	ip, #3
	add	r0, r0, #4
	strh	r1, [lr, #2]!	@ movhi
	ldr	r1, [r0, #3444]
	strh	r1, [r4, #2]!	@ movhi
	bne	.L1153
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
	ldr	ip, .L1156
	add	r1, r1, #12
	mov	r2, #36
	str	lr, [r3, #336]
	ldr	r3, [ip, #52]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L1157:
	.align	2
.L1156:
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
	ldr	r3, [r0, #2352]
	mov	r7, r0
	movw	r0, #2024
	ldr	r2, [r6, #1124]
	movw	r5, #26164
	ldr	ip, [r7, #2348]
	add	lr, r7, #67584
	mov	r1, #0
	add	lr, lr, #640
	mla	r3, r0, r2, r3
	mov	r0, r1
	ldr	r3, [r3, #32]
	strb	r1, [r6, #369]
	mla	r5, r5, r3, ip
.L1163:
	ldr	r3, [lr, #4]
	cmp	r3, #0
	bne	.L1159
	cmp	lr, #0
	beq	.L1170
	ldr	r1, [r7, #2540]
	cmp	r1, #0
	beq	.L1170
	ldr	r2, [r7, #2560]
	cmp	r2, lr
	beq	.L1159
	add	r2, r7, #2560
	b	.L1161
.L1162:
	ldr	ip, [r2, #4]!
	cmp	ip, lr
	beq	.L1159
.L1161:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L1162
.L1170:
	mov	r8, #704
	mov	r4, lr
	mla	r8, r8, r0, r7
	mov	r3, #101
	mov	r2, #624
	str	r3, [r6, #408]
	add	r0, r6, #424
	mov	r3, #1
	add	r8, r8, #67584
	strb	r3, [r6, #369]
	add	r1, r8, #712
	mov	r9, #0
	bl	memcpy
	str	r4, [r6, #420]
	str	r9, [r6, #584]
	str	r9, [r8, #660]
	strb	r9, [r8, #642]
	strb	r9, [r4]
	ldr	r3, [r6, #420]
	str	r9, [r3, #52]
	ldr	r3, [r6, #420]
	str	r9, [r3, #56]
	ldr	r3, [r6, #420]
	str	r9, [r3, #8]
	ldrb	r1, [r6, #369]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L1167
	ldr	r0, [r7, #276]
	bl	FSP_NewLogicFs
	mov	r8, r0
	mov	r1, r0
	ldr	r0, [r7, #276]
	bl	FSP_GetLogicFs
	mvn	r3, r8
	mov	r3, r3, lsr #31
	cmp	r0, #0
	mov	r4, r0
	moveq	r3, #0
	cmp	r3, #0
	beq	.L1165
	add	r2, r7, #3472
	add	r1, r5, #2624
	add	r2, r2, #8
	bl	HEVC_ConvertHDRInfo
	ldr	r3, [r7, #2356]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L1174
.L1166:
	ldr	r5, .L1175
	add	r1, r4, #8
	ldr	r0, [r6, #420]
	mov	r2, #624
	ldr	r3, [r5, #52]
	add	r0, r0, #72
	blx	r3
	ldr	r1, [r6, #420]
	add	r0, r7, #53504
	ldr	r3, [r5, #52]
	add	r1, r1, #72
	mov	r2, #624
	add	r0, r0, #168
	blx	r3
	ldr	r3, [r6, #420]
	mov	r0, #0
	str	r8, [r3, #32]
	ldrsb	r3, [r4, #4]
	str	r3, [r6, #404]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1159:
	add	r0, r0, #1
	add	lr, lr, #704
	cmp	r0, #20
	bne	.L1163
.L1167:
	ldr	r1, .L1175+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1174:
	add	r3, r7, #24576
	ldr	r5, [r4, #148]
	ldr	r0, [r4, #52]
	ldr	r2, [r3, #2868]
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
	b	.L1166
.L1165:
	strb	r3, [r6, #369]
	mov	r0, r3
	ldr	r1, .L1175+8
	bl	dprint_vfmw
	mvn	r0, #3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1176:
	.align	2
.L1175:
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
	ldr	r0, [r0, #276]
	bl	FSP_GetFspType
	cmn	r0, #1
	mov	r4, r0
	movne	r4, #0
	bne	.L1181
	b	.L1184
.L1180:
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L1182
	cmp	r4, #32
	beq	.L1185
.L1181:
	mov	r1, r4
	ldr	r0, [r5, #276]
	bl	FSP_GetLogicFs
	add	r4, r4, #1
	cmp	r0, #0
	bne	.L1180
	ldr	r1, .L1186
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1184:
	ldr	r1, .L1186+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1182:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1185:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1187:
	.align	2
.L1186:
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
	ldr	r0, [r0, #276]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1202
.L1189:
	add	r9, r5, #68608
	add	r8, r5, #82944
	add	r9, r9, #272
	add	r8, r8, #16
	mov	r6, #0
	mov	r7, #0
.L1192:
	sub	r4, r9, #32
	mov	r3, #0
	strb	r3, [r9, #-655]
	str	r3, [r9, #-652]
.L1191:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1203
.L1190:
	cmp	r4, r9
	bne	.L1191
	add	r9, r4, #704
	cmp	r9, r8
	bne	.L1192
	ldr	r2, [r5, #180]
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r3, #0
	add	r5, r5, #2144
	mov	r1, r3
.L1194:
	add	r3, r3, #1
	str	r1, [r5, #4]!
	cmp	r3, r2
	bne	.L1194
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1203:
	mov	r1, r2
	ldr	r0, [r5, #276]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L1190
.L1202:
	add	r0, r5, #61696
	add	r0, r0, #64
	bl	ResetVoQueue
	b	.L1189
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
	ldr	r3, [r6, #2560]
	str	r7, [r3, #4]
	ldr	r3, [r6, #2560]
	ldr	r0, [r0, #276]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1205
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L1206
	ldr	r3, [r6, #2560]
	mov	r2, r7
	ldr	r0, [r5, #276]
	ldr	r1, [r3, #32]
	bl	FSP_SetDisplay
.L1206:
	ldr	r3, [r6, #2560]
	mov	r2, #0
	ldr	r0, [r5, #276]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
.L1205:
	ldr	r1, [r5, #2540]
	sub	r1, r1, #1
	cmp	r4, r1
	addls	r3, r4, #640
	addls	r3, r5, r3, lsl #2
	bhi	.L1209
.L1208:
	add	r4, r4, #1
	ldr	r2, [r3, #4]!
	cmp	r4, r1
	str	r2, [r3, #-4]
	bls	.L1208
.L1209:
	add	r3, r1, #640
	mov	r2, #0
	str	r2, [r5, r3, asl #2]
	str	r1, [r5, #2540]
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
	ldr	r3, [r0, #2540]
	mov	r5, r0
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r7, #0
	mov	r4, r7
	mov	r6, r7
	b	.L1221
.L1220:
	ldr	r3, [r5, #2540]
	add	r4, r4, #1
	cmp	r4, r3
	mov	r7, r4
	bcs	.L1227
.L1221:
	add	r3, r4, #640
	ldr	r3, [r5, r3, asl #2]
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1220
	mov	r1, r7
	mov	r0, r5
	strb	r6, [r3]
	sub	r4, r4, #1
	bl	HEVC_RemoveApcStore
	mov	r1, r7
	mov	r0, r5
	bl	HEVC_RemoveFrameStoreOutDPB
	b	.L1220
.L1227:
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
	ldr	r5, [r0, #2540]
	mov	r7, r0
	cmp	r5, #0
	beq	.L1237
	mov	r6, #0
	mvn	r8, #0
	mov	r10, r6
	mov	r9, r6
	b	.L1236
.L1247:
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	cmpne	r3, #3
	ldr	r3, [r4, #2560]
	bne	.L1232
	ldr	r0, [r3, #8]
	cmp	r0, #1
	beq	.L1232
	str	r8, [r3, #32]
	mov	r2, #1
	ldr	r3, [r4, #2560]
	strb	r1, [r3, #1]
	ldr	r3, [r4, #2560]
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2560]
.L1232:
	ldr	r3, [r3]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	addne	r10, r10, #1
	beq	.L1246
	add	r9, r9, #1
	cmp	r9, r5
	beq	.L1229
.L1236:
	add	r4, r7, r10, lsl #2
	ldr	r0, [r7, #276]
	ldr	r3, [r4, #2560]
	ldr	r1, [r3, #32]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L1230
	ldrsb	r1, [r0]
	cmp	r1, #0
	beq	.L1247
	ldr	r3, [r4, #2560]
	b	.L1232
.L1230:
	ldr	r3, [r4, #2560]
	ldr	r1, [r3, #8]
	cmp	r1, #1
	beq	.L1232
	str	r8, [r3, #32]
	mov	r2, #1
	ldr	r3, [r4, #2560]
	strb	r0, [r3, #1]
	ldr	r3, [r4, #2560]
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2560]
	b	.L1232
.L1246:
	mov	r1, r10
	mov	r0, r7
	add	r9, r9, #1
	bl	HEVC_RemoveFrameStoreOutDPB
	cmp	r9, r5
	mov	r6, #1
	bne	.L1236
.L1229:
	mov	r0, r6
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1237:
	mov	r6, r5
	b	.L1229
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
	ldr	r3, [r0, #2536]
	mov	r4, r0
	ldr	r2, [r0, #2540]
	cmp	r2, r3
	ldmccfd	sp, {r4, r5, fp, sp, pc}
	cmp	r3, #15
	bls	.L1251
.L1252:
	ldr	r3, [r4, #2560]
	mov	r5, #0
	mov	r2, r5
	strb	r5, [r3, #1]
	ldr	r1, [r3, #32]
	ldr	r0, [r4, #276]
	bl	FSP_SetRef
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_RemoveApcStore
	mov	r1, r5
	mov	r0, r4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	HEVC_RemoveFrameStoreOutDPB
.L1251:
	bl	HEVC_IncreaseDPBSize
	cmp	r0, #0
	bne	.L1252
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
	ldr	r2, [r0, #2540]
	mov	r6, r0
	ldr	r3, [r0, #2536]
	cmp	r2, r3
	bhi	.L1261
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1262:
	add	lr, r6, #2544
	mov	r3, #0
	add	lr, lr, #12
.L1258:
	ldr	r2, [lr, #4]!
	ldr	r4, [r5]
	ldr	ip, [r2, #12]
	cmp	r4, ip
	strgt	ip, [r5]
	strgt	r3, [r7]
	add	r3, r3, #1
	ldr	ip, [r6, #2540]
	cmp	ip, r3
	bhi	.L1258
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1261:
	ldr	r1, .L1263
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r6, #2536]
	cmp	r2, #0
	str	r2, [r6, #2540]
	bne	.L1262
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1264:
	.align	2
.L1263:
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
	ldr	r5, [r0, #2352]
	movw	r6, #2024
	movw	r4, #26164
	ldr	ip, [r3, #1124]
	ldr	r3, [r0, #2348]
	ldr	lr, [fp, #4]
	mla	ip, r6, ip, r5
	ldr	ip, [ip, #32]
	mla	r3, r4, ip, r3
	ldr	ip, [r3, #72]
	ldr	r3, [r3, #76]
	cmp	r1, ip
	cmpeq	lr, #0
	bne	.L1269
	cmp	r2, r3
	beq	.L1270
.L1269:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1270:
	ldr	r3, [r0, #168]
	cmp	r3, #0
	beq	.L1269
	ldr	r3, [r0, #2356]
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
	ldr	r4, [r0, #2364]
	ldrb	lr, [r4, #1]	@ zero_extendqisi2
	cmp	lr, #0
	beq	.L1282
	mov	r3, #0
	mov	r2, r4
	mov	r1, r3
.L1273:
	add	r3, r3, #1
	ldr	ip, [r2, #48]
	cmp	r3, lr
	add	r2, r2, #28
	add	r1, r1, ip
	bne	.L1273
.L1272:
	ldr	r2, [r0, #2340]
	mov	r3, lr, asl #5
	sub	lr, r3, lr, asl #2
	add	r4, r4, lr
	ldr	r3, [r2, #24]
	and	r3, r3, #7
	str	r3, [r4, #60]
	ldr	r2, [r0, #2364]
	ldr	r3, [r0, #2340]
	ldrb	ip, [r2, #1]	@ zero_extendqisi2
	ldr	lr, [r3, #24]
	mov	r3, ip, asl #5
	sub	r3, r3, ip, asl #2
	add	r3, r2, r3
	ldr	r2, [r3, #52]
	add	r2, r2, lr, lsr #3
	rsb	r2, r1, r2
	str	r2, [r3, #64]
	ldr	ip, [r0, #2364]
	ldr	r3, [r0, #2340]
	ldrb	r2, [ip, #1]	@ zero_extendqisi2
	ldr	lr, [r3, #24]
	mov	r3, r2, asl #5
	sub	r2, r3, r2, asl #2
	add	r2, ip, r2
	ldr	r3, [r2, #48]
	add	r3, r1, r3
	rsb	r3, lr, r3, lsl #3
	str	r3, [r2, #56]
	ldr	r2, [r0, #2364]
	ldrb	r1, [r2, #1]	@ zero_extendqisi2
	mov	r3, r1, asl #5
	sub	r3, r3, r1, asl #2
	add	r3, r2, r3
	ldr	r2, [r3, #64]
	ldr	r1, [r3, #60]
	and	r2, r2, #3
	add	r2, r1, r2, lsl #3
	str	r2, [r3, #60]
	ldr	r2, [r0, #2364]
	ldrb	r1, [r2, #1]	@ zero_extendqisi2
	mov	r3, r1, asl #5
	sub	r3, r3, r1, asl #2
	add	r3, r2, r3
	ldr	r2, [r3, #64]
	bic	r2, r2, #3
	str	r2, [r3, #64]
	ldr	r3, [r0, #2364]
	ldr	r2, [r3, #8]
	sub	ip, r2, #1
	cmp	ip, #1
	bhi	.L1289
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	add	lr, r1, #1
	cmp	r2, lr
	movhi	r2, #1
	movls	r2, #0
	cmp	lr, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L1275
	add	r1, r1, #2
	mov	ip, #0
.L1276:
	sub	r2, r1, #1
	mov	lr, r2, asl #5
	sub	lr, lr, r2, asl #2
	add	r3, r3, lr
	str	ip, [r3, #60]
	ldr	r2, [r0, #2364]
	add	r2, r2, lr
	ldr	r3, [r2, #52]
	str	r3, [r2, #64]
	ldr	r2, [r0, #2364]
	add	r2, r2, lr
	ldr	r3, [r2, #48]
	mov	r3, r3, asl #3
	str	r3, [r2, #56]
	ldr	r3, [r0, #2364]
	add	r3, r3, lr
	ldr	r2, [r3, #64]
	ldr	r4, [r3, #60]
	and	r2, r2, #15
	add	r2, r4, r2, lsl #3
	str	r2, [r3, #60]
	ldr	r2, [r0, #2364]
	add	r3, r2, lr
	ldr	r2, [r3, #64]
	bic	r2, r2, #15
	str	r2, [r3, #64]
	ldr	r3, [r0, #2364]
	ldr	r2, [r3, #8]
	cmp	r1, #1
	cmpls	r1, r2
	add	r1, r1, #1
	movcc	lr, #1
	movcs	lr, #0
	cmp	lr, #0
	bne	.L1276
	sub	ip, r2, #1
.L1275:
	mov	r1, ip, asl #5
	ldr	lr, [r3, #12]
	sub	ip, r1, ip, asl #2
	add	r3, r3, ip
	ldr	r2, [r3, #56]
	rsb	r2, lr, r2
	str	r2, [r3, #56]
	ldr	lr, [r0, #2364]
	ldrb	r1, [lr, #1]	@ zero_extendqisi2
	ldr	r3, [lr, #8]
	cmp	r1, r3
	ldmcsfd	sp, {r4, fp, sp, pc}
	sxth	r3, r1
	add	r0, r0, #53248
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, lr, r3
.L1280:
	ldr	r2, [r3, #64]
	add	r1, r1, #1
	ldr	ip, [r0, #400]
	add	r3, r3, #28
	cmp	r2, ip
	strcc	r2, [r0, #400]
	ldr	r2, [lr, #8]
	cmp	r2, r1
	bhi	.L1280
	ldmfd	sp, {r4, fp, sp, pc}
.L1289:
	ldr	r1, .L1290
	mov	r0, #0
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	dprint_vfmw
.L1282:
	mov	r1, lr
	b	.L1272
.L1291:
	.align	2
.L1290:
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
	ldr	r2, [r2, #1124]
	cmp	r2, r3
	bge	.L1300
	movw	r3, #2024
	ldr	r1, [r0, #2352]
	mul	r3, r3, r2
	add	ip, r1, r3
	ldrb	r3, [r1, r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1301
	ldr	r2, [ip, #32]
	ldr	r3, [r0, #152]
	cmp	r2, r3
	bge	.L1302
	ldr	r1, [r0, #2348]
	movw	r3, #26164
	mla	r3, r3, r2, r1
	ldrb	r1, [r3, #1]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1303
	ldr	r2, [r3, #52]
	ldr	r3, [r0, #148]
	cmp	r2, r3
	bcs	.L1304
	movw	r3, #23196
	ldr	r1, [r0, #2344]
	mul	r3, r3, r2
	ldrb	r3, [r1, r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1305
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1300:
	ldr	r1, .L1306
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1301:
	ldr	r1, .L1306+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1302:
	ldr	r1, .L1306+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1303:
	ldr	r1, .L1306+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1304:
	ldr	r1, .L1306+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1305:
	ldr	r1, .L1306+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1307:
	.align	2
.L1306:
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
	ldr	r2, [r0, #2364]
	ldr	r1, [r0, #200]
	ldr	r3, [r2, #16]
	cmn	r1, #-2147483647
	sub	ip, r3, #16
	beq	.L1340
	cmp	r3, #21
	cmpne	ip, #2
	addls	ip, r0, #53248
	ldrls	lr, [ip, #1096]
	bls	.L1314
.L1322:
	sub	r3, r3, #19
	add	ip, r0, #53248
	cmp	r3, #1
	movls	r1, #-2147483647
	strls	r1, [r0, #200]
	ldr	lr, [ip, #1096]
.L1314:
	cmp	lr, r1
	bge	.L1312
	ldr	r3, [r2, #16]
	sub	r4, r3, #8
	cmp	r4, #1
	bls	.L1341
.L1315:
	ldrb	r1, [r0, #8]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1316
	ldr	r4, [r0, #204]
	cmp	lr, r4
	bge	.L1316
	ldr	r3, [r2, #16]
	sub	r5, r3, #8
	cmp	r5, #1
	bls	.L1342
.L1316:
	ldrb	r3, [ip, #1051]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1317
	ldr	r2, [r2, #16]
	sub	r1, r2, #19
	cmp	r1, #1
	bls	.L1337
	cmp	r2, #21
	beq	.L1337
	sub	r2, r2, #16
	cmp	r2, #2
	strls	lr, [r0, #204]
	movls	r3, #1
	strlsb	r3, [r0, #8]
.L1317:
	ldr	r2, [r0, #248]
	ldrb	r3, [ip, #1049]	@ zero_extendqisi2
	cmp	r2, #0
	ldreq	r1, [ip, #1124]
	bne	.L1343
	ldr	r2, [r0, #320]
	cmp	r2, r1
	movne	r0, #1
	beq	.L1344
.L1335:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1340:
	cmp	ip, #5
	bhi	.L1338
	cmp	r3, #21
	cmpne	ip, #2
	bhi	.L1322
	add	ip, r0, #53248
	ldr	lr, [ip, #1096]
	str	lr, [r0, #200]
.L1312:
	ldrb	r1, [r0, #3]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1315
	ldr	r3, [r2, #16]
	sub	r4, r3, #8
	cmp	r4, #1
	bhi	.L1315
	str	r3, [sp, #8]
	mov	r0, #1
	str	r1, [sp, #4]
	movw	r3, #10448
	str	lr, [sp]
	ldr	r2, .L1345
	ldr	r1, .L1345+4
	bl	dprint_vfmw
	mov	r0, #2
	b	.L1335
.L1343:
	add	r2, r2, #16640
	ldr	r1, [ip, #1124]
	add	r2, r2, #213
	ldr	r2, [r0, r2, asl #2]
	cmp	r1, r2
	ldr	r2, [r0, #320]
	movne	r3, #1
	cmp	r2, r1
	movne	r0, #1
	bne	.L1335
	b	.L1344
.L1342:
	str	r3, [sp, #12]
	mov	r0, #1
	stmia	sp, {r1, lr}
	movw	r3, #10458
	str	r4, [sp, #8]
	ldr	r2, .L1345
	ldr	r1, .L1345+8
	bl	dprint_vfmw
.L1338:
	mov	r0, #2
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1344:
	ldr	r0, [r0, #284]
	cmp	r0, lr
	moveq	r0, r3
	movne	r0, #1
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1341:
	str	r3, [sp, #8]
	mov	r0, #1
	str	r1, [sp, #4]
	movw	r3, #10440
	str	lr, [sp]
	ldr	r2, .L1345
	ldr	r1, .L1345+12
	bl	dprint_vfmw
	mov	r0, #2
	b	.L1335
.L1337:
	str	lr, [r0, #204]
	strb	r3, [r0, #8]
	b	.L1317
.L1346:
	.align	2
.L1345:
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
	ldr	r0, [r0, #2340]
	bl	BsGet
	cmp	r0, #0
	beq	.L1352
	ldr	r0, [r4, #2340]
	bl	BsBitsToNextByte
	subs	r1, r0, #0
	beq	.L1351
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r1, .L1353
	mov	r2, r0
	mov	r0, #19
	bl	dprint_vfmw
.L1351:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1352:
	ldr	r1, .L1353+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1354:
	.align	2
.L1353:
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
	ldr	r0, [r0, #1120]
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
	ldr	r0, [r0, #1120]
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
	bne	.L1396
.L1393:
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r3, [fp, #-152]
	cmp	r3, #0
	str	r0, [r5, #284]
	ble	.L1360
	ldr	r3, [fp, #-152]
	add	r6, r5, #42
	add	r9, r3, #42
	mov	r7, r6
	add	r9, r5, r9
	b	.L1362
.L1361:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r7, #1]!
	cmp	r7, r9
	beq	.L1397
.L1362:
	cmp	r8, #0
	beq	.L1361
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r7, #-5]
	b	.L1361
.L1397:
	ldr	r3, [fp, #-152]
	cmp	r3, #7
	ldrle	r7, [fp, #-152]
	ble	.L1366
	b	.L1372
.L1365:
	cmp	r7, #8
	beq	.L1372
.L1366:
	mov	r1, #2
	ldr	r0, [r4, #2340]
	bl	BsGet
	add	r7, r7, #1
	cmp	r0, #0
	beq	.L1365
	ldr	r1, .L1398
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1360:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1372:
	add	r9, r5, #360
	add	r3, r5, #48
	mov	r7, #0
	b	.L1370
.L1369:
	ldr	r3, [fp, #-152]
	add	r7, r7, #1
	add	r9, r9, #4
	cmp	r3, r7
	mov	r3, r5
	ble	.L1360
.L1370:
	cmp	r8, #0
	beq	.L1394
	ldrb	r2, [r6, #-5]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1392
.L1394:
	add	r5, r3, #32
.L1367:
	ldrb	r3, [r6, #1]!	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1369
	mov	r1, #8
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r9]
	b	.L1369
.L1392:
	mov	r1, #2
	ldr	r0, [r4, #2340]
	str	r3, [fp, #-156]
	bl	BsGet
	ldr	r3, [fp, #-156]
	mov	r1, #1
	add	r5, r3, #32
	mov	r10, r3
	str	r0, [r9, #-72]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #5
	strb	r0, [r6, #199]
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r9, #-48]
.L1368:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1]!
	cmp	r10, r5
	bne	.L1368
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6, #205]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6, #211]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6, #217]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r6, #223]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #16
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #12
	ldr	r0, [r4, #2340]
	bl	BsGet
	b	.L1367
.L1396:
	mov	r1, #2
	ldr	r0, [r0, #2340]
	bl	BsGet
	mov	r1, #1
	ldr	r9, .L1398+4
	mov	r7, r5
	mov	r6, #0
	str	r0, [r5, #276]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #5
	strb	r0, [r5]
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r5, #272]
.L1359:
	mov	r3, r6
	ldr	ip, [r9, #68]
	ldr	r2, .L1398+8
	mov	r1, #100
	sub	r0, fp, #144
	add	r6, r6, #1
	blx	ip
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	cmp	r6, #32
	strb	r0, [r7, #1]!
	bne	.L1359
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #33]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #34]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #35]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r5, #36]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #16
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #12
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r2, [r5, #272]
	add	r3, r4, #81920
	mov	r1, #8
	str	r2, [r3, #3848]
	ldrb	r2, [r5, #2]	@ zero_extendqisi2
	str	r2, [r3, #3852]
	ldrb	r2, [r5, #3]	@ zero_extendqisi2
	str	r2, [r3, #3856]
	b	.L1393
.L1399:
	.align	2
.L1398:
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
	ldr	r9, .L1418
	add	r10, r0, #2224
	mov	r6, #0
	add	r3, r0, #2240
	str	r0, [fp, #-52]
	str	r3, [fp, #-48]
.L1401:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	beq	.L1409
	ldr	r3, [fp, #-52]
	mov	r8, r6, asl #9
	ldr	r7, [fp, #-48]
	sub	r8, r8, r6, asl #7
	add	r5, r3, #612
	mov	r4, #0
.L1407:
	cmp	r6, #0
	add	r0, r5, r8
	mov	r2, #64
	ldr	r3, [r9, #52]
	beq	.L1416
	cmp	r6, #3
	beq	.L1417
	ldr	ip, .L1418+4
	cmp	r4, #2
	ldr	r1, .L1418+8
	movhi	r1, ip
	blx	r3
.L1403:
	mov	r3, #16
	str	r3, [r7, #4]!
	ldr	r3, [r10]
	add	r4, r4, #1
	add	r5, r5, #64
	cmp	r3, r4
	bhi	.L1407
.L1409:
	add	r6, r6, #1
	ldr	r3, [fp, #-48]
	cmp	r6, #4
	add	r3, r3, #24
	str	r3, [fp, #-48]
	bne	.L1401
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1416:
	mov	r2, #16
	ldr	r1, .L1418+12
	mov	r0, r5
	blx	r3
	b	.L1403
.L1417:
	ldr	r0, .L1418+4
	cmp	r4, #0
	ldr	r1, .L1418+8
	mov	r2, #64
	movne	r1, r0
	add	r0, r5, #1152
	blx	r3
	b	.L1403
.L1419:
	.align	2
.L1418:
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
	ldr	r7, [lr, #324]
	add	r3, r3, r2
	cmp	r7, #64
	mov	r6, r3, asl #4
	movcs	r7, #64
	mov	r8, r7, lsr #2
.L1421:
	cmp	r5, r8
	ldmcsfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r7, #0
	beq	.L1421
	add	r3, r5, r6
	ldrb	ip, [r1]	@ zero_extendqisi2
	mov	r4, r1
	mov	r2, #0
	add	r3, r0, r3, lsl #2
	ldr	lr, [r3, #612]
	orr	ip, lr, ip
	str	ip, [r3, #612]
.L1427:
	add	r2, r2, #1
	add	r3, r5, r6
	and	lr, r2, #3
	cmp	r2, r7
	add	r3, r0, r3, lsl #2
	mov	ip, lr, asl #3
	beq	.L1421
	ldrb	r9, [r4, #1]!	@ zero_extendqisi2
	cmp	lr, #3
	ldr	lr, [r3, #612]
	addeq	r5, r5, #1
	orr	ip, lr, r9, asl ip
	str	ip, [r3, #612]
	b	.L1427
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
	ldr	r3, [r6, #2868]
	ldrb	r2, [r3, #5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1429
	ldrb	r3, [r3, #6]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r6, #2872]
	bne	.L1430
	ldrb	r3, [r3, #21]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1448
.L1431:
	ldr	r8, .L1449
	mov	r1, #0
	mov	r2, #1536
	add	r0, r5, #612
	add	r10, r5, #2224
	mov	r7, r1
	ldr	r3, [r8, #48]
	blx	r3
.L1433:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	movne	r9, r7, asl #3
	movne	r4, #0
	subne	r9, r9, r7, asl #1
	beq	.L1437
.L1434:
	add	r0, r9, r4
	ldr	r1, [r6, #2872]
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
	bcc	.L1434
.L1437:
	add	r7, r7, #1
	cmp	r7, #4
	bne	.L1433
	ldr	r1, [r6, #2872]
	add	r0, r5, #2240
	add	r0, r0, #4
	ldr	r3, [r8, #52]
	add	r1, r1, #1920
	mov	r2, #96
	add	r1, r1, #8
	blx	r3
.L1429:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1430:
	ldrb	r1, [r3, #21]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L1431
	ldr	r8, .L1449
	mov	r2, #1536
	add	r0, r0, #612
	mov	r7, r1
	add	r10, r5, #2224
	ldr	r3, [r8, #48]
	blx	r3
.L1438:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	movne	r9, r7, asl #3
	movne	r4, #0
	subne	r9, r9, r7, asl #1
	beq	.L1442
.L1439:
	add	r0, r9, r4
	ldr	r1, [r6, #2868]
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
	bcc	.L1439
.L1442:
	add	r7, r7, #1
	cmp	r7, #4
	bne	.L1438
	ldr	r1, [r6, #2868]
	add	r0, r5, #2240
	ldr	r3, [r8, #52]
	mov	r2, #96
	add	r1, r1, #2528
	add	r0, r0, #4
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1448:
	bl	HEVC_SetDefaultScalingList
	b	.L1429
.L1450:
	.align	2
.L1449:
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
	b	.L1452
.L1454:
	.word	.L1458
	.word	.L1456
	.word	.L1456
	.word	.L1457
.L1456:
	ldr	r3, .L1462
	cmp	r1, #2
	add	r0, r3, #64
	movhi	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L1458:
	ldr	r0, .L1462+4
	ldmfd	sp, {fp, sp, pc}
.L1457:
	ldr	r3, .L1462
	cmp	r1, #0
	add	r0, r3, #64
	movne	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L1452:
	ldr	r1, .L1462+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1463:
	.align	2
.L1462:
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
	bne	.L1474
	sub	ip, r3, #1
	mov	r4, #1
	movs	ip, ip, asr #1
	beq	.L1466
.L1467:
	movs	ip, ip, asr #1
	add	r4, r4, #1
	bne	.L1467
.L1466:
	mov	r1, #1
	ldr	r0, [r6, #2340]
	bl	BsGet
	cmp	r5, #1
	movls	r10, #0
	movhi	r10, #1
	uxtb	r0, r0
	strb	r0, [r8]
	cmp	r0, #0
	cmpne	r5, #1
	bls	.L1471
	ldr	r3, [r7, #168]
	cmp	r3, #0
	beq	.L1471
	mov	r9, r8
	mov	r5, #0
.L1472:
	mov	r1, r4
	ldr	r0, [r6, #2340]
	bl	BsGet
	add	r5, r5, #1
	str	r0, [r9, #4]!
	ldr	r3, [r7, #168]
	cmp	r3, r5
	bhi	.L1472
.L1471:
	ldr	r5, [r7, #60]
	cmp	r5, #0
	beq	.L1489
.L1470:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1489:
	mov	r1, #1
	ldr	r0, [r6, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r8, #1]
	cmp	r0, #0
	moveq	r10, #0
	andne	r10, r10, #1
	cmp	r10, #0
	beq	.L1470
	ldr	r3, [r7, #172]
	cmp	r3, #0
	beq	.L1470
	add	r8, r8, #128
.L1473:
	mov	r1, r4
	ldr	r0, [r6, #2340]
	bl	BsGet
	add	r5, r5, #1
	str	r0, [r8, #4]!
	ldr	r3, [r7, #172]
	cmp	r3, r5
	bhi	.L1473
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1474:
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
	ldr	r0, [r0, #2340]
	bl	BsResidBits
	subs	r3, r0, #0
	ble	.L1492
	ldr	r2, [r4, #2364]
	ldr	r0, [r2, #12]
	add	r0, r0, #3
	cmp	r3, r0
	movls	r0, #0
	movhi	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1492:
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
	ldr	r0, [r0, #2340]
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #24]
	mov	r2, r2, asl #3
	cmp	r2, r3
	bls	.L1496
	mov	r1, #8
	bl	BsShow
	subs	r0, r0, #128
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1496:
	ldr	r1, .L1497
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1498:
	.align	2
.L1497:
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
.L1511:
	ldr	r3, [r6, #2364]
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
.L1500:
	ldrb	r4, [r3]	@ zero_extendqisi2
	rsb	r5, r0, ip
	cmp	r4, #0
	bne	.L1512
	ldr	r2, [r6, #2364]
	add	r1, r1, #1
	add	lr, r2, r5
	ldr	r2, [r2, #44]
	cmp	r2, r3
	beq	.L1513
	ldr	r2, [lr, #44]
	cmp	r2, r3
	sub	r3, r3, #1
	bne	.L1500
	ldr	r1, [lr, #68]
	ldr	r0, [r6, #276]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r6, #2364]
	mov	r1, r4
	add	r5, r3, r5
	str	r4, [r5, #44]
	ldr	r2, [r6, #2364]
	ldr	r3, [r2, #8]
	sub	r3, r3, #1
	str	r3, [r2, #8]
	b	.L1511
.L1512:
	tst	r4, #1
	mov	r3, #1
	moveq	r2, r3
	bne	.L1507
.L1508:
	ands	r0, r4, r2, asl r3
	add	r3, r3, #1
	bne	.L1507
	cmp	r3, #8
	bne	.L1508
	ldr	r1, .L1514
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1513:
	ldr	r1, .L1514+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1507:
	ldr	ip, [r6, #2364]
	mov	r2, #0
	add	r1, r3, r1, lsl #3
	mov	r0, r2
	str	r2, [ip, #4]
	ldr	r3, [r6, #2364]
	str	r1, [r3, #12]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1515:
	.align	2
.L1514:
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
	blt	.L1517
	add	r7, r6, #1
	mov	r0, r3
	mov	r5, #100
	mov	r1, r7
	bl	__aeabi_uidivmod
	add	r2, r9, #65536
	ldr	r8, [r2, #1876]
	mla	r2, r5, r1, r8
	mov	r3, r1
	ldr	r2, [r2, #36]
	cmp	r2, #0
	beq	.L1518
	mov	r4, #0
	b	.L1520
.L1521:
	bl	__aeabi_uidivmod
	mla	r2, r5, r1, r8
	mov	r3, r1
	ldr	r2, [r2, #36]
	cmp	r2, #0
	beq	.L1518
.L1520:
	add	r4, r4, #1
	mov	r1, r7
	cmp	r4, r6
	add	r0, r3, r4
	ble	.L1521
.L1517:
	mvn	r0, #0
	str	r3, [r9, #140]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1518:
	cmp	r3, r7
	mov	r0, r3
	str	r3, [r9, #140]
	bcs	.L1531
	mov	r3, #100
	mov	r2, #1
	mla	r8, r3, r0, r8
	str	r2, [r8, #36]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1531:
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
	beq	.L1540
	add	r7, r5, #56
	mov	r4, r5
	mov	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #40]
.L1535:
	ldr	r3, [r4, #44]
	mov	r0, r6
	cmp	r3, #0
	beq	.L1534
	ldr	r1, [r4, #68]
	bl	SM_ReleaseStreamSeg
	ldr	ip, [r4, #48]
	ldr	r3, [r4, #44]
	mov	r0, #7
	ldr	r2, .L1541
	ldr	r1, .L1541+4
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #44]
	str	r3, [r4, #60]
	str	r3, [r4, #48]
.L1534:
	add	r4, r4, #28
	cmp	r4, r7
	bne	.L1535
	ldr	r3, .L1541+8
	mov	r0, r5
	mov	r2, #100
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L1540:
	mov	r0, r5
	movw	r3, #11740
	ldr	r2, .L1541+12
	ldr	r1, .L1541+16
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1542:
	.align	2
.L1541:
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
	add	r0, r0, #2752
	add	r1, r7, #2688
	ldr	r3, [r3, #2868]
	add	r2, r7, #2544
	mov	lr, #0
	add	r1, r1, #4
	add	r2, r2, #12
	add	ip, r7, #2624
	ldr	r3, [r3, #236]
	mov	r6, #17
	add	r3, r3, #1
	cmp	r3, #17
	movhi	r3, #17
	str	r3, [r0, #-216]
	add	r0, r0, #8
	mov	r3, lr
	str	lr, [r7, #2540]
	str	lr, [r7, #2552]
	str	lr, [r7, #2556]
	str	lr, [r7, #2548]
	str	lr, [r7, #2544]
.L1546:
	subs	r6, r6, #1
	str	r3, [r0, #4]!
	str	r3, [ip, #4]!
	str	r3, [r1, #4]!
	str	r3, [r2, #4]!
	bne	.L1546
	add	r4, r7, #67584
	ldr	r10, .L1579
	add	r4, r4, #712
	mov	r8, r6
.L1547:
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
	ldr	r3, .L1579
	bne	.L1547
	ldr	r3, [r3, #48]
	mov	r2, #200
	mov	r1, r5
	add	r0, r7, #2832
	blx	r3
	add	r2, r7, #2352
	add	r2, r2, #12
	add	r3, r7, #2432
	mov	r1, r5
	str	r9, [r7, #2832]
	str	r5, [r7, #2836]
.L1548:
	str	r1, [r2, #4]!
	cmp	r2, r3
	mov	r0, #0
	bne	.L1548
	add	r1, r7, #2496
	add	lr, r7, #53248
	add	r1, r1, #4
	mov	ip, r0
	str	r0, [lr, #1208]
.L1549:
	str	ip, [r3, #4]!
	cmp	r3, r1
	mov	r2, #0
	bne	.L1549
	str	r2, [lr, #1212]
	ldr	r1, [r7, #180]
	cmp	r1, r2
	movne	r3, r2
	addne	r2, r7, #2144
	movne	r0, r3
	beq	.L1554
.L1553:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L1553
.L1554:
	ldr	r3, [r7, #160]
	cmp	r3, #0
	beq	.L1578
	mov	r4, #0
	add	r8, r7, #65536
	mov	r5, r4
.L1559:
	ldr	r3, [r8, #2680]
	add	r3, r3, r6
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq	.L1555
	ldr	r3, [r7, #2364]
	cmp	r1, r3
	beq	.L1555
	ldr	r0, [r7, #276]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r8, #2680]
	add	r3, r3, r6
	str	r5, [r3, #24]
	ldr	r3, [r7, #208]
	ldr	r2, [r7, #248]
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r7, #208]
	ldr	r3, [r7, #172]
	cmp	r2, #0
	subne	r2, r2, #1
	str	r2, [r7, #248]
	cmp	r3, #2
	subhi	r3, r3, #1
	movls	r3, #2
	str	r3, [r7, #172]
.L1555:
	ldr	r3, [r7, #160]
	add	r4, r4, #1
	add	r6, r6, #28
	cmp	r3, r4
	bhi	.L1559
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1578:
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1580:
	.align	2
.L1579:
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
	beq	.L1592
	ldr	r3, [r6, #2540]
	add	r8, r6, #2544
	cmp	r3, #0
	addne	r8, r8, #12
	movne	r3, #0
	movne	ip, #1
	movne	r1, r8
	beq	.L1607
.L1586:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	streqb	ip, [r2, #2]
	ldr	r2, [r6, #2540]
	cmp	r2, r3
	bhi	.L1586
.L1587:
	mov	r4, r6
	mov	r7, #17
	mov	r5, #0
	mov	r3, #-2147483647
	str	r3, [r6, #244]
.L1584:
	ldr	r3, [r8, #4]!
	mov	r2, #1
	cmp	r3, #0
	beq	.L1588
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #276]
	bl	FSP_ClearLogicFs
.L1588:
	ldr	r3, [r4, #2628]
	mov	r2, #1
	cmp	r3, #0
	beq	.L1589
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #276]
	bl	FSP_ClearLogicFs
.L1589:
	ldr	r3, [r4, #2696]
	mov	r2, #1
	cmp	r3, #0
	beq	.L1590
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #276]
	bl	FSP_ClearLogicFs
.L1590:
	ldr	r3, [r4, #2764]
	mov	r2, #1
	cmp	r3, #0
	beq	.L1591
	ldr	r1, [r3, #32]
	ldr	r0, [r6, #276]
	bl	FSP_ClearLogicFs
.L1591:
	subs	r7, r7, #1
	str	r5, [r4, #2764]
	str	r5, [r4, #2696]
	add	r4, r4, #4
	str	r5, [r4, #2624]
	str	r5, [r4, #2556]
	bne	.L1584
	mov	r0, r6
	bl	Hevc_InitDecBuffers
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1607:
	add	r8, r8, #12
	b	.L1587
.L1592:
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
	ldr	r0, [r0, #2364]
	cmp	r0, #0
	beq	.L1611
	ldr	r3, [r0, #48]
	cmp	r3, #4096
	ldmcsfd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, .L1612
	mov	r0, #7
	bl	dprint_vfmw
	ldr	r3, [r4, #2364]
	ldr	ip, .L1612+4
	mov	r5, #0
	ldr	r2, [r3, #48]
	ldr	r0, [r3, #72]
	ldr	r1, [r3, #44]
	rsb	r0, r2, r0
	ldr	r3, [ip, #52]
	blx	r3
	ldr	r3, [r4, #2364]
	mov	r0, #1
	ldr	r2, [r3, #96]
	ldr	r1, [r3, #68]
	str	r2, [r3, #68]
	ldr	r3, [r4, #2364]
	str	r5, [r3, #60]
	ldr	r3, [r4, #2364]
	ldr	ip, [r3, #48]
	ldr	r2, [r3, #72]
	rsb	r2, ip, r2
	str	r2, [r3, #44]
	ldr	r3, [r4, #2364]
	ldr	ip, [r3, #48]
	ldr	r2, [r3, #80]
	rsb	r2, ip, r2
	str	r2, [r3, #52]
	ldr	r3, [r4, #2364]
	ldr	r2, [r3, #48]
	ldr	ip, [r3, #76]
	add	r2, r2, ip
	str	r2, [r3, #48]
	ldr	r3, [r4, #2364]
	str	r0, [r3, #8]
	ldr	r0, [r4, #276]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #2364]
	str	r5, [r3, #72]
	ldr	r3, [r4, #2364]
	str	r5, [r3, #88]
	ldr	r3, [r4, #2364]
	str	r5, [r3, #76]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1611:
	ldr	r1, .L1612+8
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L1613:
	.align	2
.L1612:
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
	beq	.L1619
	ldr	ip, [r0, #2540]
	cmp	ip, #0
	beq	.L1619
	ldr	r3, [r0, #2560]
	cmp	r3, r1
	beq	.L1621
	add	r0, r0, #2560
	mov	r3, #0
	b	.L1616
.L1617:
	ldr	r2, [r0, #4]!
	cmp	r2, r1
	beq	.L1621
.L1616:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1617
.L1619:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1621:
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
	bls	.L1628
	movw	r3, #61166
	mov	r2, r0
	movt	r3, 65535
	ldr	r1, .L1629
	mov	r0, #1
	bl	dprint_vfmw
	movw	r0, #61166
	movt	r0, 65535
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1628:
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
.L1630:
	.align	2
.L1629:
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
	ldr	r1, .L1655
	ldr	r0, [r0, #2340]
	bl	HEVC_ue_v
	mov	r1, #1
	str	r0, [r4, #4]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4]
	cmp	r0, #0
	beq	.L1632
	ldr	r3, [r4, #16]
.L1633:
	cmp	r3, #4
	ldrls	pc, [pc, r3, asl #2]
	b	.L1650
.L1637:
	.word	.L1636
	.word	.L1638
	.word	.L1639
	.word	.L1640
	.word	.L1641
.L1632:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #8
	strb	r0, [r4, #1]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #8
	str	r0, [r4, #8]
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r0, #8
	str	r0, [r4, #12]
	bhi	.L1653
	mov	r1, #8
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r3, r0
	str	r0, [r4, #16]
	b	.L1633
.L1641:
	mov	r1, #8
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r0, #255
	str	r0, [r4, #36]
	beq	.L1654
.L1649:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r4, #2]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #52]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #56]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #60]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #64]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #68]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #72]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #52]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r3, #0
	str	r0, [r4, #76]
.L1635:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1636:
	mov	r1, #32
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #20]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r3, #0
	str	r0, [r4, #24]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1638:
	mov	r1, #32
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #28]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r3, #0
	str	r0, [r4, #32]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1639:
	ldr	r8, [r4, #12]
	mov	r3, #1
	movs	r8, r3, asl r8
	addne	r7, r4, #2128
	movne	r6, #0
	beq	.L1644
.L1643:
	ldr	r1, [r4, #8]
	add	r6, r6, #1
	ldr	r0, [r5, #2340]
	add	r1, r1, #7
	bic	r1, r1, #7
	bl	BsGet
	cmp	r8, r6
	str	r0, [r7, #4]!
	bne	.L1643
.L1644:
	ldr	r1, [r4, #8]
	add	r8, r4, r8, lsl #2
	mov	r3, #0
	mov	r2, #1
	mov	r2, r2, asl r1
	mov	r0, r3
	str	r2, [r8, #2132]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1640:
	mov	r1, #16
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r0, #256
	str	r0, [r4, #80]
	bhi	.L1645
	cmp	r0, #0
	addne	r6, r4, #80
	movne	r7, #0
	beq	.L1650
.L1647:
	ldr	r1, [r4, #8]
	add	r7, r7, #1
	ldr	r0, [r5, #2340]
	add	r1, r1, #7
	bic	r1, r1, #7
	bl	BsGet
	str	r0, [r6, #4]!
	ldr	r1, [r4, #12]
	ldr	r0, [r5, #2340]
	add	r1, r1, #7
	bic	r1, r1, #7
	bl	BsGet
	str	r0, [r6, #1024]
	ldr	r3, [r4, #80]
	cmp	r3, r7
	bhi	.L1647
.L1650:
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1654:
	mov	r1, #32
	ldr	r0, [r5, #2340]
	bl	BsGet
	str	r0, [r4, #40]
	b	.L1649
.L1653:
	mov	r2, r0
	ldr	r1, .L1655+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1635
.L1645:
	ldr	r1, .L1655+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1635
.L1656:
	.align	2
.L1655:
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
	ldr	r1, .L1666
	ldr	r0, [r0, #2340]
	bl	HEVC_ue_v
	mov	r1, #1
	str	r0, [r5, #12]
	ldr	r0, [r4, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	bne	.L1658
	mov	r1, #7
	ldr	r0, [r4, #2340]
	bl	BsGet
	sub	r3, r0, #3
	str	r0, [r5, #16]
	cmp	r3, #2
	bhi	.L1665
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #6
	strb	r0, [r5, #1]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #20]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #2]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #3]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #4]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #5]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r5, #6]
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r5, #7]
	bne	.L1661
	ldr	r3, [r5, #16]
	cmp	r3, #5
	beq	.L1661
	mov	r1, #4
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #4
	str	r0, [r5, #24]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #4
	str	r0, [r5, #28]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #4
	str	r0, [r5, #32]
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r5, #36]
.L1661:
	mov	r1, #8
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #40]
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r5, #8]
.L1658:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r5, #9]
	ldr	r0, [r4, #2340]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1665:
	mov	r2, r0
	ldr	r1, .L1666+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1667:
	.align	2
.L1666:
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
	bne	.L1673
	ldr	r1, .L1679
	ldr	r0, [r0, #2340]
	bl	HEVC_ue_v
	str	r0, [r5, #4]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	streq	r3, [r5, #8]
	bne	.L1677
.L1671:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	bne	.L1678
.L1672:
	ldr	r0, [r4, #2340]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1677:
	ldr	r1, [r6, #8]
	ldr	r0, [r4, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #8]
	b	.L1671
.L1678:
	ldr	r1, [r6, #12]
	ldr	r0, [r4, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #12]
	b	.L1672
.L1673:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1680:
	.align	2
.L1679:
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
	ldr	r0, [r0, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r7, #4]
	ldr	r0, [r6, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7]
	ldr	r0, [r6, #2340]
	bl	BsGet
	ldr	r1, .L1688
	strb	r0, [r7, #1]
	ldr	r0, [r6, #2340]
	bl	HEVC_ue_v
	cmp	r0, #15
	str	r0, [r7, #8]
	bgt	.L1682
	cmp	r0, #0
	addge	r5, r7, #8
	movge	r4, #0
	blt	.L1686
.L1685:
	ldr	r1, .L1688+4
	add	r4, r4, #1
	ldr	r0, [r6, #2340]
	bl	HEVC_ue_v
	str	r0, [r5, #4]!
	ldr	r3, [r7, #8]
	cmp	r3, r4
	bge	.L1685
.L1686:
	ldr	r0, [r6, #2340]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1682:
	ldr	r1, .L1688+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1689:
	.align	2
.L1688:
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
	ldr	r1, [r4, #2792]
	mov	r0, #0
	ldr	r3, [r4, #2784]
	mov	r8, r2
	str	r0, [r4, #2792]
	str	r1, [r4, #2804]
	add	r3, r3, #1
	ldr	r1, .L1723
	str	r3, [r4, #2784]
	ldr	r0, [r6, #2340]
	bl	HEVC_ue_v
	cmp	r0, #15
	str	r0, [r5, #4]
	bhi	.L1718
	mov	r1, #1
	strb	r1, [r4, #2857]
	ldrb	r3, [r8, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1693
	ldrb	r0, [r5]	@ zero_extendqisi2
.L1694:
	cmp	r0, #0
	bne	.L1719
.L1695:
	mov	r1, #1
	ldr	r0, [r6, #2340]
	bl	BsGet
	strb	r0, [r5, #1]
	ldr	r1, [r8, #32]
	ldr	r0, [r6, #2340]
	add	r1, r1, #1
	bl	BsGet
	add	r0, r0, #1
	str	r0, [r5, #16]
	ldr	r2, [r8, #48]
	cmp	r2, #31
	bhi	.L1720
.L1696:
	mov	r4, #0
	cmp	r4, #0
	mov	r9, r5
	mov	r7, #1
	beq	.L1721
.L1697:
	cmp	r4, #1
	beq	.L1722
.L1701:
	cmp	r7, #1
	bhi	.L1700
.L1699:
	add	r4, r4, #1
	add	r7, r7, #1
	cmp	r4, #0
	add	r9, r9, #16
	bne	.L1697
.L1721:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1701
	ldr	r3, [r8, #48]
	cmn	r3, #1
	movne	r10, r9
	movne	r3, r4
	beq	.L1699
.L1713:
	ldr	r1, [r8, #28]
	add	r3, r3, #1
	ldr	r0, [r6, #2340]
	add	r1, r1, #1
	str	r3, [fp, #-48]
	str	r3, [fp, #-52]
	bl	BsGet
	str	r0, [r10, #20]
	ldr	r1, [r8, #28]
	ldr	r0, [r6, #2340]
	add	r1, r1, #1
	bl	BsGet
	ldr	r3, [fp, #-48]
	str	r0, [r10, #24]
	ldrb	r2, [r8, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1703
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1704
.L1703:
	ldr	r1, [r8, #28]
	ldr	r0, [r6, #2340]
	add	r1, r1, #1
	str	r3, [fp, #-48]
	bl	BsGet
	str	r0, [r10, #28]
	ldr	r1, [r8, #28]
	ldr	r0, [r6, #2340]
	add	r1, r1, #1
	bl	BsGet
	ldr	r3, [fp, #-48]
	str	r0, [r10, #32]
.L1704:
	ldr	r2, [r8, #48]
	add	r10, r10, #32
	add	r2, r2, #1
	cmp	r2, r3
	bhi	.L1713
	b	.L1701
.L1700:
	ldr	r0, [r6, #2340]
	bl	BsToNextByte
	mov	r0, #0
.L1717:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1720:
	ldr	r1, .L1723+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r8, #48]
	b	.L1696
.L1719:
	ldr	r1, [r8, #32]
	ldr	r0, [r6, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #8]
	ldr	r1, [r8, #36]
	ldr	r0, [r6, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r5, #12]
	b	.L1695
.L1693:
	ldr	r0, [r6, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r5]
	b	.L1694
.L1718:
	mov	r2, r0
	ldr	r1, .L1723+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1717
.L1722:
	ldrb	r3, [r8, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1700
	ldr	r3, [r8, #48]
	cmn	r3, #1
	beq	.L1700
	mov	r10, r9
	mov	r3, #0
	b	.L1713
.L1724:
	.align	2
.L1723:
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
	ldr	r1, [r3, #2788]
	ldr	r2, [r3, #2792]
	add	r1, r1, #1
	str	r1, [r3, #2788]
	add	r2, r2, #1
	str	r2, [r3, #2792]
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1751
.L1726:
	ldrh	r3, [r5, #108]
	cmp	r3, #0
	beq	.L1729
	ldr	r1, [r5, #140]
	ldr	r0, [r4, #2340]
	add	r1, r1, #1
	bl	BsGet
	add	r0, r0, #1
	str	r0, [r6, #12]
	ldr	r1, [r5, #144]
	ldr	r0, [r4, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r6, #16]
	ldrb	r3, [r5, #110]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1752
.L1729:
	ldr	r0, [r4, #2340]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1751:
	mov	r1, #4
	ldr	r0, [r0, #2340]
	bl	BsGet
	mov	r1, #2
	str	r0, [r6, #4]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #8]
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r6]
	b	.L1726
.L1752:
	ldr	r1, [r5, #120]
	ldr	r0, [r4, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r6, #20]
	ldrb	r3, [r5, #110]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1729
	ldrb	r3, [r5, #111]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1729
	ldr	r1, .L1754
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	mov	r1, #1
	str	r0, [r6, #24]
	ldr	r0, [r4, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #1]
	cmp	r0, #0
	bne	.L1753
.L1730:
	ldr	r3, [r6, #24]
	cmp	r3, #255
	bhi	.L1733
	add	r8, r6, #28
	mov	r7, #0
	b	.L1732
.L1731:
	ldr	r3, [r6, #24]
	cmp	r3, r7
	bcc	.L1729
.L1732:
	ldr	r1, .L1754+4
	add	r7, r7, #1
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	str	r0, [r8, #4]!
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1731
	ldr	r1, [r5, #116]
	ldr	r0, [r4, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r8, #1024]
	b	.L1731
.L1753:
	ldr	r1, [r5, #116]
	ldr	r0, [r4, #2340]
	add	r1, r1, #1
	bl	BsGet
	str	r0, [r6, #28]
	b	.L1730
.L1733:
	ldr	r1, .L1754+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1755:
	.align	2
.L1754:
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
	bne	.L1798
.L1759:
	ldr	r1, .L1807
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #16
	str	r0, [r4, #16]
	bhi	.L1799
	ldr	r1, .L1807+4
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #16
	str	r0, [r4, #20]
	bhi	.L1800
	ldr	r6, [r4, #16]
	add	r2, r0, r6
	str	r2, [r4, #24]
	cmp	r2, #16
	bhi	.L1784
	cmp	r6, #0
	addne	r10, r4, #68
	addne	r9, r4, #52
	movne	r7, #0
	movne	r8, r7
	bne	.L1788
	b	.L1786
.L1787:
	str	r8, [r10, #4]!
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r9, #1]!
	ldr	r6, [r4, #16]
	cmp	r6, r7
	bls	.L1801
.L1788:
	ldr	r1, .L1807+8
	add	r7, r7, #1
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	mov	r1, #1
	cmp	r0, #32768
	mvn	r3, r0
	add	r8, r8, r3
	bcc	.L1787
	mov	r2, r0
	ldr	r1, .L1807+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1798:
	mov	r1, #1
	ldr	r0, [r0, #2340]
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4]
	beq	.L1759
	ldr	r8, [r6, #228]
	cmp	r8, r7
	movne	r0, #0
	strne	r0, [r4, #4]
	beq	.L1802
.L1762:
	sub	r3, r7, #1
	rsb	r7, r0, r3
	cmp	r7, r3
	bhi	.L1803
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldr	r1, .L1807+16
	str	r0, [r4, #8]
	ldr	r0, [r5, #2340]
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
	bhi	.L1804
	add	r2, r3, #13120
	mov	r10, #0
	add	r2, r2, #60
	add	r8, r4, #35
	add	r6, r6, r2
	str	r10, [fp, #-48]
	str	r10, [fp, #-52]
	mov	r7, r10
	b	.L1771
.L1766:
	sub	r2, r0, #1
	cmp	r2, #1
	bls	.L1805
.L1767:
	strb	r0, [r8, #1]!
	add	r10, r10, #1
	ldr	r2, [r9, #4]
	add	r6, r6, #4
	cmp	r2, r10
	bcc	.L1806
.L1771:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r0, #0
	bne	.L1766
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r0, r0, asl #1
	sub	r2, r0, #1
	cmp	r2, #1
	bhi	.L1767
.L1805:
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
	b	.L1767
.L1806:
	add	r2, r2, #1
	ldr	r3, [fp, #-52]
	str	r2, [r4, #28]
	cmp	r7, #16
	ldr	r2, [fp, #-48]
	str	r7, [r4, #24]
	str	r3, [r4, #16]
	str	r2, [r4, #20]
	bhi	.L1772
	cmp	r3, #16
	bhi	.L1772
	cmp	r2, #16
	bhi	.L1772
	cmp	r7, #1
	subhi	r7, r7, #1
	addhi	r8, r4, #72
	addhi	r6, r4, #53
	movhi	r5, #0
	bls	.L1778
.L1777:
	ldr	ip, [r8, #4]!
	cmp	r5, #0
	ldrb	lr, [r6, #1]!	@ zero_extendqisi2
	mov	r2, r5
	movge	r1, r8
	movge	r3, r6
	blt	.L1780
.L1776:
	ldr	r0, [r1, #-4]!
	sub	r3, r3, #1
	cmp	ip, r0
	strlt	r0, [r1, #4]
	ldrltb	r0, [r3]	@ zero_extendqisi2
	strltb	r0, [r3, #1]
	strlt	ip, [r1]
	strltb	lr, [r3]
	subs	r2, r2, #1
	bpl	.L1776
.L1780:
	add	r5, r5, #1
	cmp	r5, r7
	bne	.L1777
.L1778:
	ldr	r3, [fp, #-52]
	movs	r6, r3, lsr #1
	beq	.L1775
	ldr	r3, [fp, #-52]
	add	r0, r4, #72
	mov	r1, #0
	add	r2, r3, #17
	add	r3, r3, #52
	add	r3, r4, r3
	add	r2, r4, r2, lsl #2
	add	r4, r4, #53
.L1781:
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
	bne	.L1781
.L1775:
	mov	r0, #0
.L1763:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1801:
	ldr	r2, [r4, #24]
.L1786:
	cmp	r2, r6
	bls	.L1775
	add	r8, r6, #17
	add	r9, r6, #52
	add	r9, r4, r9
	mov	r7, #0
	add	r8, r4, r8, lsl #2
.L1789:
	ldr	r1, .L1807+20
	add	r6, r6, #1
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	mov	r1, #1
	add	r0, r0, r1
	add	r7, r7, r0
	str	r7, [r8, #4]!
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r9, #1]!
	ldr	r3, [r4, #24]
	cmp	r3, r6
	bhi	.L1789
	b	.L1775
.L1802:
	ldr	r1, .L1807+24
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	add	r2, r0, #1
	str	r2, [r4, #4]
	cmp	r8, r2
	bcs	.L1762
	mov	r3, r8
	ldr	r1, .L1807+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1763
.L1772:
	ldr	r1, .L1807+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1763
.L1799:
	mov	r2, r0
	ldr	r1, .L1807+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1763
.L1803:
	mov	r2, r7
	ldr	r1, .L1807+40
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1763
.L1804:
	ldr	r1, .L1807+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1763
.L1800:
	ldr	r2, [r4, #16]
	mov	r0, #1
	ldr	r1, .L1807+48
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1763
.L1784:
	mov	r3, #16
	ldr	r1, .L1807+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1763
.L1808:
	.align	2
.L1807:
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
	beq	.L1831
	cmp	r2, #0
	bne	.L1856
.L1814:
	mov	r4, r8
	mov	r6, #0
.L1828:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #40]
	cmp	r0, #0
	movne	r3, #1
	strneb	r3, [r4, #41]
	beq	.L1857
.L1816:
	ldrb	r2, [r4, #41]	@ zero_extendqisi2
	mov	r3, #0
	strb	r3, [r4, #42]
	cmp	r2, r3
	str	r3, [r4, #48]
	beq	.L1817
	ldr	r1, .L1864
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldrb	r3, [r4, #42]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r4, #44]
	beq	.L1819
.L1860:
	ldr	r2, [r4, #48]
	cmp	r2, #31
	bhi	.L1858
.L1821:
	mov	r2, r4
	mov	r10, #0
.L1830:
	cmp	r10, #0
	beq	.L1859
	cmp	r10, #1
	beq	.L1854
.L1827:
	add	r10, r10, #1
	add	r2, r2, #4
	cmp	r10, #2
	bne	.L1830
.L1826:
	add	r6, r6, #1
	add	r4, r4, #1280
	cmp	r7, r6
	add	r4, r4, #12
	bcs	.L1828
	mov	r0, #0
.L1853:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1817:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r3, r0
	strb	r3, [r4, #42]
	cmp	r3, #0
	bne	.L1860
.L1819:
	ldr	r1, .L1864+4
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	mov	r2, r0
	cmp	r2, #31
	str	r0, [r4, #48]
	bls	.L1821
.L1858:
	ldr	r1, .L1864+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #48]
	b	.L1821
.L1857:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r4, #41]
	b	.L1816
.L1856:
	mov	r1, #1
	ldr	r0, [r0, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8]
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r8, #1]
	ldrh	r3, [r8]
	cmp	r3, #0
	beq	.L1814
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r8, #2]
	cmp	r0, #0
	bne	.L1861
.L1812:
	mov	r1, #4
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #4
	str	r0, [r8, #16]
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldrb	r3, [r8, #2]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r8, #20]
	bne	.L1862
.L1813:
	mov	r1, #5
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #5
	str	r0, [r8, #28]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #5
	str	r0, [r8, #32]
	ldr	r0, [r5, #2340]
	bl	BsGet
	str	r0, [r8, #36]
	b	.L1814
.L1831:
	mvn	r0, #0
	b	.L1853
.L1862:
	mov	r1, #4
	ldr	r0, [r5, #2340]
	bl	BsGet
	str	r0, [r8, #24]
	b	.L1813
.L1861:
	mov	r1, #8
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #5
	str	r0, [r8, #4]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r8, #8]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #5
	strb	r0, [r8, #3]
	ldr	r0, [r5, #2340]
	bl	BsGet
	str	r0, [r8, #36]
	b	.L1812
.L1859:
	ldrb	r1, [r8]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1863
	mov	r9, r2
	str	r10, [fp, #-48]
	b	.L1844
.L1829:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	str	r2, [fp, #-52]
	bl	BsGet
	ldr	r3, [fp, #-48]
	add	r9, r9, #8
	ldr	r2, [fp, #-52]
	str	r0, [r9, #1068]
	ldr	r1, [r4, #48]
	cmp	r1, r3
	bcc	.L1827
.L1844:
	ldr	r3, [fp, #-48]
	ldr	r1, .L1864+12
	ldr	r0, [r5, #2340]
	add	r3, r3, #1
	str	r2, [fp, #-52]
	str	r3, [fp, #-48]
	bl	HEVC_ue_v
	ldr	r1, .L1864+16
	str	r0, [r9, #52]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r2, [fp, #-52]
	str	r0, [r9, #308]
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1829
	ldr	r1, .L1864+20
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L1864+24
	str	r0, [r9, #564]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r2, [fp, #-52]
	str	r0, [r9, #820]
	b	.L1829
.L1863:
	add	r2, r2, #4
	mov	r10, #1
.L1854:
	ldrb	r1, [r8, #1]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1826
	mov	r3, #0
	mov	r9, r2
	str	r3, [fp, #-48]
	b	.L1844
.L1865:
	.align	2
.L1864:
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
	ldr	r0, [r0, #2340]
	bl	BsGet
	cmp	r0, #3
	str	r0, [r6, #12]
	beq	.L1867
	mov	r2, r0
	ldr	r1, .L1919
	mov	r0, #1
	bl	dprint_vfmw
.L1867:
	mov	r1, #6
	ldr	r0, [r7, #2340]
	bl	BsGet
	cmp	r0, #63
	str	r0, [r6, #16]
	bhi	.L1911
	mov	r1, #3
	ldr	r0, [r7, #2340]
	bl	BsGet
	cmp	r0, #7
	str	r0, [fp, #-152]
	bhi	.L1912
	ldr	r3, [fp, #-152]
	mov	r1, #1
	add	r5, r3, #1
	str	r5, [r6, #20]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #16
	strb	r0, [r6, #1]
	ldr	r0, [r7, #2340]
	bl	BsGet
	movw	r3, #65535
	cmp	r0, r3
	str	r0, [r6, #24]
	bne	.L1913
	add	r1, r6, #12416
	ldr	r3, [fp, #-152]
	add	r1, r1, #20
	mov	r2, #1
	mov	r0, r7
	bl	HEVC_DecPTL
	subs	r4, r0, #0
	bne	.L1914
	mov	r1, #1
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldr	r3, [fp, #-152]
	cmp	r3, #0
	addge	r8, r6, #48
	strb	r0, [r6, #7]
	bge	.L1879
	b	.L1877
.L1876:
	add	r4, r4, #1
	cmp	r4, r5
	beq	.L1877
.L1879:
	ldr	r1, .L1919+4
	ldr	r0, [r7, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L1919+8
	add	r0, r0, #1
	str	r0, [r8, #4]!
	ldr	r0, [r7, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L1919+12
	str	r0, [r8, #32]
	ldr	r0, [r7, #2340]
	bl	HEVC_ue_v
	str	r0, [r8, #64]
	ldrb	r3, [r6, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1876
	ldr	r3, [fp, #-152]
	add	r2, r4, #1
	cmp	r3, r2
	blt	.L1877
	add	r3, r4, #13
	add	r3, r6, r3, lsl #2
.L1878:
	ldr	r1, [r6, #52]
	add	r2, r2, #1
	cmp	r2, r5
	str	r1, [r3, #4]!
	ldr	r1, [r6, #84]
	str	r1, [r3, #32]
	ldr	r1, [r6, #116]
	str	r1, [r3, #64]
	bne	.L1878
.L1877:
	mov	r1, #6
	ldr	r0, [r7, #2340]
	bl	BsGet
	ldr	r1, .L1919+16
	str	r0, [r6, #28]
	ldr	r0, [r7, #2340]
	bl	HEVC_ue_v
	cmp	r0, #1024
	str	r0, [r6, #32]
	bhi	.L1915
	cmp	r0, #1
	ble	.L1880
	ldr	r10, .L1919+20
	add	r9, r6, #148
	mov	r8, #1
.L1881:
	ldr	r3, [r6, #28]
	cmp	r3, #0
	movgt	r5, r9
	movgt	r4, #0
	ble	.L1883
.L1882:
	ldr	r2, .L1919+24
	mov	r3, r8
	str	r4, [sp]
	mov	r1, #100
	ldr	ip, [r10, #68]
	sub	r0, fp, #144
	blx	ip
	mov	r1, #1
	ldr	r0, [r7, #2340]
	add	r4, r4, r1
	bl	BsGet
	str	r0, [r5, #4]!
	ldr	r2, [r6, #28]
	cmp	r2, r4
	bgt	.L1882
.L1883:
	ldr	r3, [r6, #32]
	add	r8, r8, #1
	add	r9, r9, #4
	cmp	r3, r8
	bgt	.L1881
.L1880:
	mov	r1, #1
	ldr	r0, [r7, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #6]
	cmp	r0, #0
	beq	.L1889
	mov	r1, #32
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r6, #36]
	ldr	r0, [r7, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #40]
	ldr	r0, [r7, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #5]
	cmp	r0, #0
	bne	.L1916
.L1887:
	ldr	r1, .L1919+28
	ldr	r0, [r7, #2340]
	bl	HEVC_ue_v
	cmp	r0, #1024
	str	r0, [r6, #48]
	bcs	.L1917
	cmp	r0, #0
	ble	.L1889
	add	r9, r6, #8192
	mov	r3, #1
	ldr	r1, .L1919+32
	add	r8, r6, #12800
	str	r3, [r9, #148]
	add	r5, r6, #4224
	ldr	r0, [r7, #2340]
	add	r8, r8, #20
	bl	HEVC_ue_v
	add	r3, r6, #4096
	add	r5, r5, #24
	mov	r4, #0
	str	r0, [r3, #148]
	ldr	r0, [r9, #148]
	ldr	r9, [fp, #-152]
.L1890:
	uxtb	r2, r0
	mov	r1, r8
	mov	r3, r9
	mov	r0, r7
	bl	HEVC_DecHrdParam
	add	r4, r4, #1
	ldr	r1, .L1919+32
	cmp	r0, #0
	bne	.L1918
	ldr	r3, [r6, #48]
	cmp	r3, r4
	ble	.L1889
	ldr	r0, [r7, #2340]
	bl	HEVC_ue_v
	cmp	r4, #0
	mov	r1, #1
	str	r0, [r5], #4
	ldrle	r0, [r5, #4092]
	ble	.L1890
	ldr	r0, [r7, #2340]
	bl	BsGet
	str	r0, [r5, #4092]
	b	.L1890
.L1889:
	mov	r1, #1
	ldr	r0, [r7, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r6, #3]
	cmp	r0, #0
	bne	.L1906
.L1886:
	mov	r0, #0
.L1869:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1895:
	ldr	r0, [r7, #2340]
	bl	BsGet
	strb	r0, [r6, #4]
.L1906:
	mov	r0, r7
	bl	HEVC_MoreRbspData
	mov	r1, #1
	cmp	r0, #0
	bne	.L1895
	b	.L1886
.L1918:
	ldr	r1, .L1919+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1916:
	ldr	r1, .L1919+40
	ldr	r0, [r7, #2340]
	bl	HEVC_ue_v
	str	r0, [r6, #44]
	b	.L1887
.L1913:
	ldr	r1, .L1919+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1869
.L1911:
	ldr	r1, .L1919+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1869
.L1914:
	ldr	r1, .L1919+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1869
.L1912:
	mov	r2, r0
	ldr	r1, .L1919+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1869
.L1915:
	mov	r2, r0
	mov	r3, #1024
	ldr	r1, .L1919+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1869
.L1917:
	mov	r2, r0
	mov	r3, #1024
	ldr	r1, .L1919+64
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1869
.L1920:
	.align	2
.L1919:
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
	ldr	r0, [r0, #2340]
	bl	BsGet
	cmp	r0, #15
	mov	r6, r0
	bhi	.L1927
	movw	r7, #23196
	ldr	r3, [r4, #2344]
	mul	r5, r7, r0
	add	r2, r3, r5
	ldrb	r9, [r3, r5]	@ zero_extendqisi2
	cmp	r9, #0
	bne	.L1928
	str	r0, [r2, #8]
	mov	r0, r4
	ldr	r1, [r4, #2344]
	add	r1, r1, r5
	bl	HEVC_ProcessVPS
	subs	r8, r0, #0
	bne	.L1929
	ldr	r3, [r4, #2344]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2344]
	strb	r2, [r3, r5]
.L1923:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1928:
	ldr	r9, .L1931
	mov	r0, r4
	ldr	r1, .L1931+4
	str	r6, [r9, #-3456]
	bl	HEVC_ProcessVPS
	subs	r8, r0, #0
	bne	.L1930
	ldr	ip, .L1931+8
	mov	r3, #1
	ldr	r0, [r4, #2344]
	mov	r2, r7
	ldr	r1, .L1931+4
	add	r0, r0, r5
	ldr	ip, [ip, #56]
	strb	r3, [r9, #-3462]
	strb	r3, [r9, #-3464]
	blx	ip
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1930:
	mov	r2, r6
	ldr	r1, .L1931+12
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L1923
.L1927:
	ldr	r1, .L1931+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1923
.L1929:
	mov	r2, r6
	ldr	r1, .L1931+20
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #2344]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #2]
	ldr	r3, [r4, #2344]
	strb	r9, [r3, r5]
	b	.L1923
.L1932:
	.align	2
.L1931:
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
	ldr	r3, .L1982
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
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4]
	cmp	r0, #0
	bne	.L1973
.L1935:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #1]
	cmp	r0, #0
	bne	.L1974
.L1937:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #3]
	cmp	r0, #0
	bne	.L1975
.L1939:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #6]
	cmp	r0, #0
	bne	.L1976
.L1941:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #7]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #8]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #9]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #10]
	cmp	r0, #0
	beq	.L1942
	ldr	r1, .L1982+4
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	str	r0, [r4, #60]
	ldr	r3, [r6, #64]
	cmp	r3, #3
	bhi	.L1977
	sub	r2, fp, #28
	ldr	r1, .L1982+8
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-32]
	mul	r0, r3, r0
	str	r0, [r4, #60]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	sub	r2, fp, #28
	ldr	r1, .L1982+12
	str	r0, [r4, #64]
	ldr	r3, [r6, #64]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-32]
	mul	r0, r3, r0
	str	r0, [r4, #64]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	sub	r2, fp, #28
	ldr	r1, .L1982+16
	str	r0, [r4, #68]
	ldr	r3, [r6, #64]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-16]
	mul	r0, r3, r0
	str	r0, [r4, #68]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	sub	r2, fp, #28
	str	r0, [r4, #72]
	ldr	r3, [r6, #64]
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-16]
	mul	r0, r3, r0
	str	r0, [r4, #72]
.L1942:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #11]
	cmp	r0, #0
	beq	.L1945
	mov	r1, #32
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #32
	str	r0, [r4, #76]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #80]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #12]
	cmp	r0, #0
	bne	.L1978
.L1946:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #13]
	cmp	r0, #0
	bne	.L1979
.L1945:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #14]
	cmp	r0, #0
	moveq	r3, r0
	bne	.L1980
.L1944:
	mov	r0, r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1980:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #15]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #16]
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldr	r1, .L1982+20
	strb	r0, [r4, #17]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #4096
	str	r0, [r4, #88]
	bcs	.L1981
.L1947:
	ldr	r1, .L1982+24
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L1982+28
	str	r0, [r4, #92]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L1982+32
	str	r0, [r4, #96]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L1982+36
	str	r0, [r4, #100]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	mov	r3, #0
	str	r0, [r4, #104]
	mov	r0, r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1976:
	ldr	r1, .L1982+40
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L1982+44
	str	r0, [r4, #52]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	str	r0, [r4, #56]
	b	.L1941
.L1973:
	mov	r1, #8
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r0, #255
	str	r0, [r4, #24]
	bne	.L1935
	mov	r1, #16
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #16
	str	r0, [r4, #28]
	ldr	r0, [r5, #2340]
	bl	BsGet
	str	r0, [r4, #32]
	b	.L1935
.L1975:
	mov	r1, #3
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #36]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #4]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #5]
	cmp	r0, #0
	beq	.L1939
	mov	r1, #8
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #8
	str	r0, [r4, #40]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #8
	str	r0, [r4, #44]
	ldr	r0, [r5, #2340]
	bl	BsGet
	str	r0, [r4, #48]
	b	.L1939
.L1974:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r4, #2]
	b	.L1937
.L1979:
	ldr	r3, [r6, #56]
	mov	r2, #1
	add	r1, r4, #108
	mov	r0, r5
	bl	HEVC_DecHrdParam
	cmp	r0, #0
	beq	.L1945
	ldr	r1, .L1982+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1944
.L1978:
	ldr	r1, .L1982+52
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	str	r0, [r4, #84]
	b	.L1946
.L1981:
	ldr	r1, .L1982+56
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1947
.L1977:
	ldr	r1, .L1982+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
	b	.L1944
.L1983:
	.align	2
.L1982:
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
	bls	.L1989
	mvn	r0, #-2147483648
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1989:
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
	ldr	r1, .L2002
	ldr	r0, [r0, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L2002+4
	str	r0, [r9]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #1024
	str	r0, [r9, #4]
	bhi	.L1991
	cmp	r0, #0
	beq	.L1997
	add	r7, r9, #4096
	add	r8, r9, #12288
	add	r7, r7, #8
	add	r8, r8, #8
	add	r6, r9, #4
	mov	r4, #0
	b	.L1996
.L1995:
	ldr	r3, [r9, #4]
	add	r8, r8, #4
	cmp	r3, r4
	bls	.L1997
.L1996:
	mov	r1, #6
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #3
	str	r0, [r6, #4]!
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldr	r1, .L2002+8
	str	r0, [r7], #4
	ldr	r3, [r6]
	sub	r3, r3, #19
	cmp	r3, #1
	bls	.L1994
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	str	r0, [r7, #4092]
.L1994:
	cmp	r4, #0
	add	r4, r4, #1
	beq	.L1995
	ldr	r1, .L2002+12
	add	r8, r8, #4
	ldr	r0, [r5, #2340]
	bl	HEVC_se_v
	str	r0, [r8, #-4]
	ldr	r3, [r9, #4]
	cmp	r3, r4
	bhi	.L1996
.L1997:
	ldr	r0, [r5, #2340]
	bl	BsToNextByte
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1991:
	ldr	r1, .L2002+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2003:
	.align	2
.L2002:
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
	beq	.L2006
	ldr	r1, .L2007
	ldr	r0, [r0, #2340]
	bl	HEVC_se_v
	mov	r1, #1
	str	r0, [r6, #4]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r6]
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r6, #1]
	ldr	r0, [r4, #2340]
	bl	BsToNextByte
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2006:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2008:
	.align	2
.L2007:
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
	ldr	r1, [r0, #2364]
	mov	r4, r0
	ldr	r7, [fp, #4]
	ldr	r1, [r1, #16]
	cmp	r1, #39
	beq	.L2053
	cmp	r3, #5
	beq	.L2037
	cmp	r3, #132
	beq	.L2038
	cmp	r7, #0
	movne	r5, #0
	beq	.L2032
.L2040:
	add	r5, r5, #1
	mov	r1, #8
	ldr	r0, [r4, #2340]
	bl	BsGet
	cmp	r5, r7
	bne	.L2040
	b	.L2032
.L2037:
	mov	r2, r7
	add	r1, r6, #3200
	bl	HEVC_Sei_User_Data_Unregistered
	cmp	r0, #0
	bne	.L2034
.L2032:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_Sei_More_Data_In_Payload
	cmp	r0, #0
	bne	.L2054
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2054:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_Sei_Payload_Extension_Present
	cmp	r0, #0
	bne	.L2055
.L2041:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r6, #12]
	b	.L2042
.L2043:
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r6, #16]
.L2042:
	ldr	r0, [r4, #2340]
	bl	BsIsByteAligned
	mov	r1, #1
	cmp	r0, #0
	beq	.L2043
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2053:
	cmp	r3, #147
	ldrls	pc, [pc, r3, asl #2]
	b	.L2011
.L2013:
	.word	.L2012
	.word	.L2014
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2015
	.word	.L2016
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2017
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2018
	.word	.L2011
	.word	.L2019
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2020
	.word	.L2021
	.word	.L2022
	.word	.L2023
	.word	.L2011
	.word	.L2024
	.word	.L2025
	.word	.L2011
	.word	.L2011
	.word	.L2026
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2011
	.word	.L2027
	.word	.L2011
	.word	.L2011
	.word	.L2028
.L2038:
	add	r1, r6, #23808
	add	r1, r1, #16
	bl	HEVC_Sei_Decoded_Picture_Hash
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2055:
	ldr	r1, .L2056
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	str	r0, [r6, #8]
	b	.L2041
.L2028:
	add	r1, r6, #3472
	add	r1, r1, #10
	bl	HEVC_Sei_HLGHDR_Comapitibility_Info
	cmp	r0, #0
	beq	.L2032
.L2034:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2012:
	add	r2, r2, #2720
	add	r1, r6, #24
	add	r2, r2, #12
	bl	HEVC_Sei_Buf_Period
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2014:
	add	r2, r2, #2624
	add	r1, r6, #1088
	bl	HEVC_Sei_Pic_Timming
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2015:
	mov	r2, r7
	add	r1, r6, #3200
	bl	HEVC_Sei_User_Data_Unregistered
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2016:
	add	r1, r6, #3360
	bl	HEVC_Sei_Recovery_Point
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2017:
	add	r1, r6, #3472
	add	r1, r1, #12
	bl	HEVC_Sei_Tone_Mapping_Info
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2018:
	add	r1, r6, #3360
	add	r1, r1, #8
	bl	HEVC_Sei_Frame_Packing_Arrangement
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2019:
	add	r1, r6, #3408
	add	r1, r1, #4
	bl	HEVC_Sei_Display_Orientation
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2020:
	add	r1, r6, #6592
	mov	r2, r7
	add	r1, r1, #52
	bl	HEVC_Sei_Structure_Of_Pictures_Info
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2021:
	add	r1, r6, #3264
	add	r1, r1, #4
	bl	HEVC_Sei_Active_Param_sets
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2022:
	add	r2, r2, #2720
	add	r1, r6, #3344
	add	r2, r2, #12
	bl	HEVC_Sei_Dec_Unit_Info
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2023:
	add	r1, r6, #3408
	add	r1, r1, #12
	bl	HEVC_Sei_Temporal_Sub_Layer_Zero_Index
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2024:
	mov	r3, r7
	mov	r1, r6
	bl	HEVC_Sei_Scalable_Nesting
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2025:
	add	r1, r6, #3424
	add	r1, r1, #4
	bl	HEVC_Sei_Region_Refresh_Info
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2026:
	add	r1, r6, #3424
	add	r1, r1, #8
	bl	HEVC_Sei_Mastering_Display_Colour_Volume
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2027:
	add	r1, r6, #3472
	add	r1, r1, #4
	bl	HEVC_Sei_Content_Light_Level_Information
	cmp	r0, #0
	beq	.L2032
	b	.L2034
.L2011:
	cmp	r7, #0
	movne	r5, #0
	beq	.L2032
.L2035:
	add	r5, r5, #1
	mov	r1, #8
	ldr	r0, [r4, #2340]
	bl	BsGet
	cmp	r5, r7
	bne	.L2035
	b	.L2032
.L2057:
	.align	2
.L2056:
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
	ldr	r0, [r0, #2340]
	b	.L2059
.L2061:
	bl	BsSkip
	ldr	r0, [r4, #2340]
	add	r5, r5, #255
	ldr	r3, [r0, #24]
	ldr	r2, [r0, #16]
	add	r1, r3, #8
	cmp	r1, r2
	bhi	.L2067
.L2059:
	mov	r1, #8
	bl	BsShow
	mov	r1, #8
	cmp	r0, #255
	ldr	r0, [r4, #2340]
	beq	.L2061
	bl	BsGet
	mov	r8, #0
	mov	r9, r0
	str	r0, [r6]
	ldr	r0, [r4, #2340]
	b	.L2062
.L2063:
	bl	BsSkip
	ldr	r0, [r4, #2340]
	add	r8, r8, #255
	ldr	r3, [r0, #24]
	ldr	r2, [r0, #16]
	add	r1, r3, #8
	cmp	r1, r2
	bhi	.L2068
.L2062:
	mov	r1, #8
	bl	BsShow
	mov	r1, #8
	cmp	r0, #255
	ldr	r0, [r4, #2340]
	beq	.L2063
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
	bne	.L2069
.L2060:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2067:
	str	r2, [sp]
	mov	r0, #1
	movw	r2, #3041
	ldr	r1, .L2070
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2068:
	str	r2, [sp]
	mov	r0, #1
	movw	r2, #3063
	ldr	r1, .L2070
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2069:
	ldr	r1, .L2070+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2060
.L2071:
	.align	2
.L2070:
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
	add	r6, r0, #3472
	add	r5, r0, #27392
	mov	r4, r0
	add	r6, r6, #8
	add	r5, r5, #60
	b	.L2076
.L2073:
	ldr	r0, [r4, #2340]
	bl	BsIsByteAligned
	cmp	r0, #0
	mov	r0, r4
	beq	.L2081
	bl	HEVC_More_Rbsp_Data
	subs	r7, r0, #0
	beq	.L2082
.L2076:
	mov	r2, r5
	mov	r1, r6
	mov	r0, r4
	bl	HEVC_SeiMessage
	cmp	r0, #0
	beq	.L2073
	ldr	r1, .L2083
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
.L2074:
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2081:
	ldr	r1, .L2083+4
	mvn	r7, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2082:
	ldr	r0, [r4, #2340]
	mov	r1, #8
	bl	BsGet
	cmp	r0, #128
	beq	.L2074
	mov	r2, r0
	ldr	r1, .L2083+8
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	b	.L2074
.L2084:
	.align	2
.L2083:
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
	ldr	r0, [r0, #2340]
	mov	r1, #1
	mov	r9, r2
	bl	BsGet
	add	r8, r7, #20480
	mov	r1, #1
	strb	r0, [r8, #2556]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	uxtb	r0, r0
	strb	r0, [r8, #2557]
	cmp	r0, #0
	ldr	r0, [r4, #2340]
	beq	.L2086
	bl	BsGet
	ldr	r1, .L2102
	strb	r0, [r8, #2558]
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	cmp	r0, #63
	str	r0, [r8, #2564]
	bhi	.L2099
	ldrb	r6, [r8, #2558]	@ zero_extendqisi2
	cmp	r0, r6
	addcs	r5, r6, #5760
	addcs	r5, r5, #1
	addcs	r5, r7, r5, lsl #2
	bcc	.L2092
.L2091:
	mov	r1, #3
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r1, .L2102+4
	add	r6, r6, #1
	str	r0, [r5, #4]!
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	str	r0, [r5, #256]
	ldr	r3, [r8, #2564]
	cmp	r3, r6
	bcs	.L2091
.L2092:
	ldr	r0, [r4, #2340]
	bl	BsToNextByte
	mov	r3, #0
	strb	r3, [r8, #2560]
	b	.L2090
.L2095:
	bl	HEVC_MoreRbspData
	cmp	r0, #0
	beq	.L2100
.L2090:
	mov	r2, r9
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_SeiMessage
	cmp	r0, #0
	mov	r0, r4
	beq	.L2095
	ldr	r1, .L2102+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2086:
	bl	BsGet
	uxtb	r5, r0
	strb	r5, [r8, #2559]
	cmp	r5, #0
	bne	.L2092
	mov	r1, #3
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r1, .L2102+12
	str	r0, [r8, #3080]
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	cmp	r0, #63
	str	r0, [r8, #3084]
	addls	r6, r7, #23552
	addls	r6, r6, #12
	bhi	.L2101
.L2094:
	mov	r1, #6
	ldr	r0, [r4, #2340]
	bl	BsGet
	add	r5, r5, #1
	str	r0, [r6, #4]!
	ldr	r3, [r8, #3084]
	cmp	r3, r5
	bcs	.L2094
	b	.L2092
.L2100:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2099:
	mov	r2, #64
	ldr	r1, .L2102+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2101:
	ldr	r1, .L2102+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2103:
	.align	2
.L2102:
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
	ldr	r3, .L2141
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
	add	r3, r6, #2224
	str	r3, [fp, #-48]
	add	r3, r9, #24
	str	r3, [fp, #-68]
	add	r3, r9, #49
	str	r3, [fp, #-72]
	mov	r3, #0
	str	r3, [fp, #-52]
.L2105:
	ldr	r2, [fp, #-48]
	ldr	r3, [r2, #4]!
	cmp	r3, #0
	str	r2, [fp, #-48]
	beq	.L2124
	ldr	r3, [fp, #-76]
	ldr	r7, [fp, #-72]
	str	r3, [fp, #-64]
	ldr	r3, [fp, #-68]
	str	r3, [fp, #-56]
	mov	r3, #0
	str	r3, [fp, #-60]
.L2123:
	mov	r1, #1
	ldr	r0, [r6, #2340]
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
	beq	.L2106
	cmp	r0, #0
	add	r4, r6, #356
	beq	.L2137
	ldr	r3, [fp, #-52]
	cmp	r3, #1
	bls	.L2130
	ldr	r1, .L2141+4
	ldr	r0, [r6, #2340]
	bl	HEVC_se_v
	add	r3, r0, #7
	str	r0, [r9, #1660]
	cmp	r3, #254
	bhi	.L2138
	ldr	r3, [fp, #-64]
	add	r8, r0, #8
	str	r8, [r3]
.L2117:
	cmp	r5, #0
	ble	.L2116
	sub	r4, r4, #4
	mov	r10, #0
	b	.L2121
.L2120:
	mov	r8, r8, lsr #24
	ldr	r2, [r4, #4]!
	add	r3, r3, r8
	cmp	r10, r5
	uxtb	r3, r3
	rsb	r8, r8, r3
	strb	r8, [r7, r2]
	beq	.L2116
.L2121:
	ldr	r1, .L2141+8
	add	r10, r10, #1
	ldr	r0, [r6, #2340]
	bl	HEVC_se_v
	add	r3, r8, r0
	add	r2, r0, #128
	add	r3, r3, #256
	cmp	r2, #255
	str	r0, [r9, #1664]
	mov	r8, r3, asr #31
	bls	.L2120
	ldr	r1, .L2141+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2135:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2125:
	ldr	r1, .L2141+16
	ldr	r0, [r6, #2340]
	bl	HEVC_ue_v
	ldr	r3, [fp, #-60]
	cmp	r0, r3
	bhi	.L2127
	ldr	r2, [fp, #-60]
	uxtb	r0, r0
	ldr	r3, [fp, #-56]
	rsb	r1, r0, r2
	strb	r0, [r3, #1560]
	uxtb	r1, r1
	strb	r1, [r3, #1584]
.L2111:
	ldr	r3, [fp, #-60]
	cmp	r1, r3
	beq	.L2139
	ldr	r2, [fp, #-52]
	mov	r3, r2, asl #3
	sub	r3, r3, r2, asl #1
	add	r1, r3, r1
	add	r1, r9, r1, lsl #6
	add	r1, r1, #49
.L2114:
	cmp	r1, #0
	beq	.L2140
	ldr	r3, .L2141
	mov	r2, r5
	mov	r0, r7
	ldr	r3, [r3, #52]
	blx	r3
.L2116:
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
	bhi	.L2123
.L2124:
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
	bne	.L2105
	mov	r0, #0
	b	.L2135
.L2130:
	mov	r8, #8
	b	.L2117
.L2137:
	ldr	r1, .L2141+16
	ldr	r0, [r6, #2340]
	bl	HEVC_ue_v
	ldr	r3, [fp, #-60]
	cmp	r0, r3
	bhi	.L2127
	ldr	r3, [fp, #-52]
	uxtb	r0, r0
	ldr	ip, [fp, #-60]
	ldr	r2, [fp, #-56]
	cmp	r3, #1
	rsb	r1, r0, ip
	strb	r0, [r2, #1560]
	uxtb	r1, r1
	strb	r1, [r2, #1584]
	bls	.L2111
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
	b	.L2111
.L2139:
	mov	r1, r3
	ldr	r0, [fp, #-52]
	bl	HEVC_GetScalingListDefaultAddress
	mov	r1, r0
	b	.L2114
.L2138:
	ldr	r1, .L2141+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2135
.L2140:
	ldr	r1, .L2141+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2135
.L2127:
	ldr	r1, .L2141+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2135
.L2106:
	cmp	r0, #0
	beq	.L2125
	ldr	r4, .L2141+32
	mov	r8, #8
	b	.L2117
.L2142:
	.align	2
.L2141:
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
	ldr	r1, .L2297
	ldr	r0, [r0, #2340]
	bl	HEVC_ue_v
	cmp	r0, #1
	mov	r8, r0
	str	r0, [r4, #64]
	beq	.L2144
	cmp	r0, #3
	bhi	.L2274
	mov	r2, r0
	ldr	r1, .L2297+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2146:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2274:
	ldr	r1, .L2297+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2144:
	ldr	r1, .L2297+12
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #8192
	str	r0, [r4, #72]
	bhi	.L2275
	ldr	r1, .L2297+16
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #4096
	str	r0, [r4, #76]
	bhi	.L2276
	ldr	r2, [r4, #760]
	ldr	r3, [r4, #72]
	cmp	r2, #120
	mul	r0, r3, r0
	beq	.L2150
	bgt	.L2151
	cmp	r2, #60
	beq	.L2152
	ble	.L2277
	cmp	r2, #90
	beq	.L2155
	cmp	r2, #93
	beq	.L2156
	cmp	r2, #63
	bne	.L2149
	mov	r2, #61440
	mov	r3, #245760
	b	.L2157
.L2151:
	cmp	r2, #156
	beq	.L2158
	bgt	.L2159
	cmp	r2, #150
	beq	.L2158
	cmp	r2, #153
	beq	.L2158
	cmp	r2, #123
	beq	.L2150
.L2149:
	ldr	r1, .L2297+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2159:
	cmp	r2, #183
	beq	.L2160
	cmp	r2, #186
	beq	.L2160
	cmp	r2, #180
	bne	.L2149
.L2160:
	mov	r2, #8912896
	mov	r3, #35651584
.L2157:
	cmp	r0, r2
	movls	r3, #16
	strls	r3, [r4, #236]
	bls	.L2162
	cmp	r0, r3, lsr #1
	movls	r3, #12
	strls	r3, [r4, #236]
	bls	.L2162
	add	r3, r3, r3, lsl #1
	cmp	r0, r3, lsr #2
	movls	r3, #8
	movhi	r3, #6
	str	r3, [r4, #236]
.L2162:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4, #80]
	beq	.L2165
	ldr	r1, .L2297+24
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r3, [r4, #64]
	ldr	r6, .L2297+28
	ldr	r1, .L2297+32
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #544]
	mul	r0, r3, r0
	str	r0, [r4, #84]
	ldr	r0, [r5, #2340]
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
	blt	.L2278
	ldr	r1, .L2297+36
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r3, [r4, #64]
	ldr	r1, .L2297+40
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #560]
	mul	r0, r3, r0
	str	r0, [r4, #92]
	ldr	r0, [r5, #2340]
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
	blt	.L2279
.L2165:
	ldr	r1, .L2297+44
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #6
	add	r2, r0, #8
	str	r2, [r4, #100]
	bhi	.L2280
	ldr	r1, .L2297+48
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #6
	add	r2, r0, #8
	str	r2, [r4, #108]
	bhi	.L2281
	ldr	r3, [r4, #100]
	ldr	r1, .L2297+52
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
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #12
	bhi	.L2282
	add	r0, r0, #4
	mov	r1, #1
	str	r0, [r4, #128]
	mov	r6, r4
	str	r0, [r4, #132]
	mov	r7, #0
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r4, #3]
	b	.L2176
.L2170:
	ldr	r3, [r4, #56]
	add	r7, r7, #1
	cmp	r7, r3
	bhi	.L2175
.L2176:
	ldr	r1, .L2297+56
	add	r6, r6, #4
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L2297+60
	add	r0, r0, #1
	str	r0, [r6, #376]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r1, .L2297+64
	ldr	r9, [r6, #376]
	str	r0, [r6, #408]
	ldr	r0, [r5, #2340]
	ldr	r10, [r4, #380]
	bl	HEVC_ue_v
	str	r0, [r6, #440]
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2170
	ldr	r1, [r4, #56]
	add	r3, r7, #1
	cmp	r3, r1
	addls	r7, r7, #95
	addls	r7, r4, r7, lsl #2
	bhi	.L2175
.L2174:
	ldr	r2, [r4, #380]
	add	r3, r3, #1
	cmp	r3, r1
	str	r2, [r7, #4]!
	ldr	r2, [r4, #412]
	str	r2, [r7, #32]
	ldr	r2, [r4, #444]
	str	r2, [r7, #64]
	bls	.L2174
.L2175:
	ldr	r3, [r4, #236]
	cmp	r9, r10
	ldr	r1, .L2297+68
	movcc	r9, r10
	cmp	r3, r9
	movcs	r3, r9
	str	r3, [r4, #236]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #136]
	bhi	.L2283
	ldr	r1, .L2297+72
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #144]
	bhi	.L2284
	ldr	r3, [r4, #136]
	add	r3, r3, #3
	str	r3, [r4, #140]
	add	r6, r0, r3
	str	r6, [r4, #148]
	sub	r2, r6, #4
	cmp	r2, #2
	bhi	.L2285
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
	beq	.L2179
	bl	__aeabi_uidiv
	add	r0, r0, #1
.L2180:
	ldr	r7, [r4, #76]
	mov	r1, r6
	str	r0, [r4, #240]
	mov	r0, r7
	bl	__aeabi_uidivmod
	mov	r0, r7
	cmp	r1, #0
	mov	r1, r6
	beq	.L2181
	bl	__aeabi_uidiv
	add	r0, r0, #1
.L2182:
	str	r0, [r4, #244]
	ldr	r1, .L2297+76
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #168]
	bhi	.L2286
	ldr	r1, .L2297+80
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #176]
	bhi	.L2287
	ldr	r3, [r4, #168]
	ldr	r2, [r4, #140]
	add	r3, r3, #2
	str	r3, [r4, #172]
	cmp	r3, r2
	bcs	.L2288
	add	r0, r0, r3
	str	r0, [r4, #180]
	cmp	r0, #5
	bhi	.L2186
	ldr	r3, [r4, #148]
	cmp	r0, r3
	bhi	.L2186
	ldr	r1, .L2297+84
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r3, [r4, #148]
	ldr	r2, [r4, #172]
	rsb	r3, r2, r3
	cmp	r0, r3
	str	r0, [r4, #200]
	bhi	.L2289
	ldr	r1, .L2297+88
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	ldr	r3, [r4, #148]
	ldr	r2, [r4, #172]
	rsb	r3, r2, r3
	cmp	r0, r3
	str	r0, [r4, #208]
	bhi	.L2290
	ldr	r3, [r4, #200]
	add	r0, r0, #1
	mov	r1, #0
	str	r0, [r4, #212]
	add	r3, r3, #1
	str	r3, [r4, #204]
	str	r1, [r5, #260]
	mov	ip, #1
	ldr	r3, [r4, #172]
	ldr	r0, [r4, #160]
	ldr	r2, [r4, #144]
	mov	r3, ip, asl r3
	cmp	r3, r0, lsr r2
	bcc	.L2191
	b	.L2291
.L2216:
	mov	r8, r0
.L2191:
	str	r8, [r5, #260]
	add	r0, r8, #1
	ldr	r3, [r4, #172]
	ldr	r1, [r4, #160]
	add	r3, r8, r3
	ldr	r2, [r4, #144]
	mov	r3, ip, asl r3
	cmp	r3, r1, lsr r2
	bcc	.L2216
.L2190:
	mov	r6, #1
	add	r8, r2, r8
	str	r6, [r4, #224]
	mov	r3, #0
	str	r8, [r4, #216]
	mov	r1, r6
	str	r3, [r4, #220]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #5]
	cmp	r0, #0
	beq	.L2192
	mov	r1, r6
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #6]
	cmp	r0, #0
	bne	.L2292
.L2192:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #7]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r6, r0
	strb	r6, [r4, #8]
	cmp	r6, #0
	bne	.L2193
	ldr	r2, [r4, #76]
	cmp	r2, #64
	bls	.L2293
.L2193:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	cmp	r0, #0
	str	r0, [r4, #116]
	beq	.L2194
	mov	r1, #4
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #4
	add	r0, r0, #1
	str	r0, [r4, #120]
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldr	r1, .L2297+92
	add	r0, r0, #1
	str	r0, [r4, #124]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #2
	str	r0, [r4, #184]
	bhi	.L2294
.L2195:
	ldr	r1, .L2297+96
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #2
	str	r0, [r4, #192]
	bhi	.L2295
.L2196:
	ldr	r2, [r4, #184]
	add	r2, r2, #3
	str	r2, [r4, #188]
	add	r0, r0, r2
	str	r0, [r4, #196]
	cmp	r0, #5
	bhi	.L2197
	ldr	r3, [r4, #148]
	cmp	r0, r3
	bhi	.L2197
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r4, #9]
.L2194:
	ldr	r1, .L2297+100
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #64
	str	r0, [r4, #228]
	bhi	.L2199
	cmp	r0, #0
	addne	r7, r4, #13056
	movne	r6, #0
	addne	r7, r7, #52
	bne	.L2204
	b	.L2205
.L2203:
	ldr	r3, [r4, #228]
	cmp	r6, r3
	bcs	.L2205
.L2204:
	mov	r3, r6
	mov	r2, r7
	mov	r1, r4
	mov	r0, r5
	bl	HEVC_DecShortTermRefPicSet
	add	r6, r6, #1
	add	r7, r7, #204
	cmp	r0, #0
	beq	.L2203
	ldr	r1, .L2297+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2156:
	mov	r2, #245760
	mov	r3, #983040
	b	.L2157
.L2181:
	bl	__aeabi_uidiv
	b	.L2182
.L2179:
	bl	__aeabi_uidiv
	b	.L2180
.L2205:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #10]
	cmp	r0, #0
	beq	.L2202
	ldr	r1, .L2297+108
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #32
	str	r0, [r4, #232]
	bhi	.L2208
	cmp	r0, #0
	addne	r8, r4, #244
	addne	r7, r4, #15
	movne	r6, #0
	beq	.L2202
.L2210:
	ldr	r1, [r4, #128]
	add	r6, r6, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	str	r0, [r8, #4]!
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r7, #1]!
	ldr	r3, [r4, #232]
	cmp	r6, r3
	bcc	.L2210
.L2202:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #11]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #12]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #13]
	cmp	r0, #0
	beq	.L2207
	mov	r2, r4
	add	r1, r4, #2624
	mov	r0, r5
	bl	HEVC_DecVuiParam
	cmp	r0, #0
	bne	.L2296
.L2207:
	ldr	ip, [r4, #76]
	ldr	r3, [r4, #72]
	ldr	r2, [r4, #2656]
	ldr	r1, [r4, #2652]
	ldr	r0, [r4, #2648]
	str	ip, [sp]
	bl	HEVCGetDar
	mov	r1, #1
	str	r0, [r4, #2644]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #14]
	cmp	r0, #0
	bne	.L2270
.L2213:
	mov	r3, #1
	mov	r0, #0
	strb	r3, [r4]
	b	.L2146
.L2212:
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r4, #15]
.L2270:
	mov	r0, r5
	bl	HEVC_MoreRbspData
	mov	r1, #1
	cmp	r0, #0
	bne	.L2212
	b	.L2213
.L2277:
	cmp	r2, #0
	beq	.L2154
	cmp	r2, #30
	bne	.L2149
.L2154:
	mov	r2, #9216
	mov	r3, #36864
	b	.L2157
.L2293:
	ldr	r1, .L2297+112
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2297+116
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r0, #0
	beq	.L2146
	ldr	r0, [r5, #276]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #102
	blx	ip
	mvn	r0, #0
	b	.L2146
.L2152:
	mov	r2, #30720
	mov	r3, #122880
	b	.L2157
.L2150:
	mov	r2, #557056
	mov	r3, #2228224
	b	.L2157
.L2158:
	mov	r2, #2228224
	mov	r3, #8912896
	b	.L2157
.L2155:
	mov	r2, #138240
	mov	r3, #552960
	b	.L2157
.L2292:
	add	r1, r4, #860
	mov	r0, r5
	bl	HEVC_DecScalingListData
	cmp	r0, #0
	beq	.L2192
	mov	r0, r6
	ldr	r1, .L2297+120
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2186:
	ldr	r1, .L2297+124
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2295:
	mov	r2, r0
	ldr	r1, .L2297+128
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r0, [r4, #192]
	b	.L2196
.L2294:
	mov	r2, r0
	ldr	r1, .L2297+132
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2195
.L2291:
	mov	r8, r1
	b	.L2190
.L2275:
	mov	r0, r8
	ldr	r1, .L2297+136
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2282:
	ldr	r1, .L2297+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2283:
	ldr	r1, .L2297+144
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2286:
	ldr	r1, .L2297+148
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2197:
	ldr	r1, .L2297+152
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2289:
	ldr	r1, .L2297+156
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2276:
	mov	r0, r8
	ldr	r1, .L2297+160
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2285:
	ldr	r1, .L2297+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2284:
	ldr	r1, .L2297+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2280:
	ldr	r1, .L2297+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2281:
	ldr	r1, .L2297+176
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2288:
	ldr	r1, .L2297+180
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2287:
	ldr	r1, .L2297+184
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2290:
	ldr	r1, .L2297+188
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2199:
	mov	r2, #64
	ldr	r1, .L2297+192
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2278:
	ldr	r1, .L2297+196
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2279:
	ldr	r1, .L2297+200
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2208:
	ldr	r1, .L2297+204
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2296:
	ldr	r1, .L2297+208
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2146
.L2298:
	.align	2
.L2297:
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
	ldr	r5, .L2317
	mov	r4, r0
	movw	r2, #26164
	mov	r1, #0
	ldr	r0, .L2317+4
	ldr	r3, [r5, #48]
	blx	r3
	mov	r2, #384
	ldr	r3, [r5, #48]
	mov	r1, #0
	sub	r0, fp, #428
	ldr	r8, .L2317+8
	blx	r3
	ldr	r0, [r4, #2340]
	mov	r1, #4
	bl	BsGet
	ldr	r2, [r4, #148]
	cmp	r2, r0
	str	r0, [fp, #-440]
	bls	.L2310
	mov	r1, #3
	ldr	r0, [r4, #2340]
	bl	BsGet
	cmp	r0, #8
	mov	r7, r0
	bhi	.L2311
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r3, r7
	mov	r2, #1
	sub	r1, fp, #428
	mov	r9, r0
	mov	r0, r4
	bl	HEVC_DecPTL
	cmp	r0, #0
	bne	.L2312
	ldr	r1, .L2317+12
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	ldr	r2, [r4, #152]
	cmp	r0, r2
	mov	r10, r0
	bcs	.L2313
	movw	ip, #26164
	ldr	r2, [r4, #2348]
	mul	r6, ip, r0
	uxtb	r0, r9
	str	ip, [fp, #-444]
	ldr	r3, [fp, #-440]
	add	r2, r2, r6
	ldrb	r9, [r2, #1]	@ zero_extendqisi2
	cmp	r9, #0
	bne	.L2314
	str	r3, [r2, #52]
	sub	r1, fp, #428
	ldr	lr, [r4, #2348]
	mov	r2, #384
	add	lr, lr, r6
	str	r7, [lr, #56]
	ldr	lr, [r4, #2348]
	add	lr, lr, r6
	strb	r0, [lr, #2]
	ldr	r0, [r4, #2348]
	ldr	r7, [r5, #52]
	add	r0, r0, r6
	add	r0, r0, #476
	blx	r7
	ldr	r2, [r4, #2348]
	mov	r0, r4
	add	r2, r2, r6
	str	r10, [r2, #60]
	ldr	r1, [r4, #2348]
	add	r1, r1, r6
	bl	HEVC_ProcessSPS
	ldr	ip, [fp, #-444]
	subs	r7, r0, #0
	bne	.L2315
	ldr	r3, [r4, #2348]
	mov	r1, #1
	add	r0, r4, #27392
	mov	r2, ip
	add	r0, r0, #60
	strb	r1, [r3, r6]
	ldr	r3, [r4, #2348]
	add	r3, r3, r6
	strb	r1, [r3, #1]
	ldr	r1, [r4, #2348]
	ldr	r3, [r5, #52]
	add	r1, r1, r6
	blx	r3
.L2301:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2314:
	str	r3, [r8, #-3412]
	sub	r1, fp, #428
	str	r7, [r8, #-3408]
	mov	r2, #384
	strb	r0, [r8, #-3462]
	ldr	r7, [r5, #52]
	ldr	r0, .L2317+16
	blx	r7
	ldr	r1, .L2317+4
	mov	r0, r4
	str	r10, [r8, #-3404]
	bl	HEVC_ProcessSPS
	ldr	ip, [fp, #-444]
	subs	r7, r0, #0
	bne	.L2316
	ldr	r0, [r4, #2348]
	mov	r3, #1
	mov	r2, ip
	strb	r3, [r8, #-3464]
	strb	r3, [r8, #-3463]
	add	r0, r0, r6
	ldr	r9, [r5, #56]
	ldr	r1, .L2317+4
	str	ip, [fp, #-440]
	blx	r9
	ldr	ip, [fp, #-440]
	add	r0, r4, #27392
	ldr	r1, [r4, #2348]
	add	r0, r0, #60
	ldr	r3, [r5, #52]
	mov	r2, ip
	add	r1, r1, r6
	blx	r3
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2313:
	ldr	r1, .L2317+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r4, #152]
	cmp	r10, r2
	bcc	.L2309
	ldr	r1, .L2317+24
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L2309
	str	r2, [fp, #-432]
	mov	r3, #8
	str	r10, [fp, #-436]
	sub	r2, fp, #436
	ldr	r0, [r4, #276]
	mov	r1, #109
	blx	ip
	mvn	r7, #0
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2309:
	mvn	r7, #0
	b	.L2301
.L2310:
	ldr	r1, .L2317+28
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	b	.L2301
.L2311:
	mov	r2, #8
	ldr	r1, .L2317+32
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	b	.L2301
.L2312:
	ldr	r1, .L2317+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L2301
.L2315:
	mov	r2, r10
	ldr	r1, .L2317+40
	mov	r0, #1
	mvn	r7, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #2348]
	mov	r2, #1
	strb	r2, [r3, r6]
	ldr	r3, [r4, #2348]
	add	r6, r3, r6
	strb	r9, [r6, #1]
	b	.L2301
.L2316:
	ldr	r1, [r4, #2348]
	add	r0, r4, #27392
	mov	r2, ip
	ldr	r4, [r5, #52]
	add	r1, r1, r6
	add	r0, r0, #60
	blx	r4
	mov	r2, r10
	ldr	r1, .L2317+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L2301
.L2318:
	.align	2
.L2317:
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
	ldr	r1, .L2447
	ldr	r0, [r0, #2340]
	bl	HEVC_ue_v
	cmp	r0, #0
	str	r0, [r4, #32]
	blt	.L2320
	ldr	r3, [r5, #152]
	cmp	r0, r3
	bge	.L2320
	ldr	r3, [r5, #2348]
	movw	r6, #26164
	mla	r6, r6, r0, r3
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2427
	mov	r1, r7
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #2]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #4]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, r7
	str	r0, [r4, #36]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #3]
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldr	r1, .L2447+4
	strb	r0, [r4, #5]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	add	r0, r0, #1
	str	r0, [r4, #40]
	cmp	r0, #15
	bhi	.L2428
	ldr	r1, .L2447+8
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	add	r0, r0, #1
	str	r0, [r4, #44]
	cmp	r0, #15
	bhi	.L2429
	ldr	r1, .L2447+12
	ldr	r0, [r5, #2340]
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
	bne	.L2430
	mov	r1, r7
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #6]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, r7
	strb	r0, [r4, #7]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #8]
	cmp	r0, #0
	streq	r0, [r4, #52]
	bne	.L2431
.L2329:
	str	r0, [r4, #56]
	ldr	r1, .L2447+16
	ldr	r0, [r5, #2340]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r4, #60]
	cmp	r3, #24
	bhi	.L2432
	ldr	r1, .L2447+20
	ldr	r0, [r5, #2340]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r4, #64]
	cmp	r3, #24
	bhi	.L2433
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #9]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #10]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #11]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #12]
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #13]
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldrb	r3, [r4, #13]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #14]
	bne	.L2434
	ldr	r3, [r6, #240]
	strh	r3, [r4, #176]	@ movhi
	ldr	r3, [r6, #244]
	strh	r3, [r4, #216]	@ movhi
.L2368:
	ldr	r0, [r4, #68]
	cmp	r0, #0
	ble	.L2373
.L2367:
	ldr	ip, [r6, #160]
	add	r1, r4, #174
	mov	r3, #0
.L2372:
	ldrsh	r2, [r1, #2]!
	add	r3, r3, #1
	mul	r2, ip, r2
	cmp	r2, #64
	bhi	.L2371
	ldr	lr, [r6, #72]
	cmp	lr, #64
	bhi	.L2435
.L2371:
	cmp	r3, r0
	blt	.L2372
.L2373:
	ldr	r1, [r4, #72]
	cmp	r1, #0
	ble	.L2370
	ldr	r0, [r6, #160]
	ldrsh	r2, [r4, #216]
	mul	r2, r0, r2
	cmp	r2, #63
	addhi	r1, r1, #107
	addhi	r3, r4, #216
	addhi	r1, r4, r1, lsl #1
	bhi	.L2376
	b	.L2374
.L2377:
	ldrsh	r2, [r3, #2]!
	mul	r2, r0, r2
	cmp	r2, #63
	bls	.L2374
.L2376:
	cmp	r3, r1
	bne	.L2377
.L2370:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #17]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #18]
	cmp	r0, #0
	bne	.L2436
.L2379:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #21]
	cmp	r0, #0
	beq	.L2382
	add	r1, r4, #260
	mov	r0, r5
	bl	HEVC_DecScalingListData
	cmp	r0, #0
	bne	.L2437
.L2382:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	ldr	r1, .L2447+24
	strb	r0, [r4, #22]
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	add	r2, r0, #2
	str	r2, [r4, #84]
	cmp	r2, #1
	ble	.L2383
	ldr	r3, [r6, #148]
	cmp	r2, r3
	bhi	.L2383
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #23]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #24]
	cmp	r0, #0
	bne	.L2414
	b	.L2387
.L2386:
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r4, #25]
.L2414:
	mov	r0, r5
	bl	HEVC_MoreRbspData
	mov	r1, #1
	cmp	r0, #0
	bne	.L2386
.L2387:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2431:
	ldr	r1, .L2447+28
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #3
	str	r0, [r4, #52]
	bls	.L2329
	mov	r0, r7
	ldr	r1, .L2447+32
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2434:
	ldr	r1, .L2447+36
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #19
	add	r2, r0, #1
	str	r2, [r4, #68]
	bhi	.L2438
	ldr	r1, .L2447+40
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #21
	add	r3, r0, #1
	str	r3, [r4, #72]
	bhi	.L2439
	ldr	r2, [r4, #68]
	cmp	r2, #10
	bgt	.L2440
	cmp	r3, #11
	ble	.L2337
	ldr	r1, .L2447+44
	mov	r2, #11
.L2424:
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2447+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2419
.L2421:
	mov	r3, #0
	ldr	r0, [r5, #276]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L2415:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2374:
	ldr	r1, .L2447+52
.L2423:
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2447+48
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L2421
.L2419:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2435:
	ldr	r1, .L2447+56
	b	.L2423
.L2430:
	mov	r0, r7
	ldr	r1, .L2447+60
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2337:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r4, #15]
	cmp	r0, #0
	bne	.L2351
	ldr	r3, [r4, #68]
	cmp	r3, #1
	addgt	r8, r4, #90
	movgt	r7, r0
	bgt	.L2349
	b	.L2441
.L2443:
	cmp	r3, r2
	bhi	.L2347
	ldr	r3, [r4, #68]
	add	r7, r7, #1
	sub	r3, r3, #1
	cmp	r3, r7
	ble	.L2442
.L2349:
	ldr	r1, .L2447+64
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	add	r0, r0, #1
	uxth	r3, r0
	strh	r3, [r8, #2]!	@ movhi
	ldr	r2, [r6, #240]
	sxth	r3, r3
	cmp	r3, #0
	bge	.L2443
.L2347:
	str	r2, [sp]
	mov	r0, #1
	mov	r2, r7
	ldr	r1, .L2447+68
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2440:
	mov	r3, r2
	ldr	r1, .L2447+72
	mov	r2, #10
	b	.L2424
.L2436:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r4, #19]
	ldr	r0, [r5, #2340]
	bl	BsGet
	uxtb	r7, r0
	strb	r7, [r4, #20]
	cmp	r7, #0
	bne	.L2379
	ldr	r1, .L2447+76
	ldr	r0, [r5, #2340]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r4, #76]
	bhi	.L2444
.L2381:
	ldr	r1, .L2447+80
	ldr	r0, [r5, #2340]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r4, #80]
	bls	.L2379
	ldr	r1, .L2447+84
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #80]
	b	.L2379
.L2351:
	ldr	r7, [r4, #68]
	cmp	r7, #0
	addgt	r3, r4, #174
	movgt	r8, #0
	ble	.L2341
.L2359:
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
	bne	.L2359
.L2341:
	ldr	r10, [r4, #72]
	cmp	r10, #0
	addgt	r2, r4, #214
	movgt	r8, #0
	ble	.L2358
.L2360:
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
	bne	.L2360
.L2358:
	cmp	r7, #1
	bne	.L2366
	ldr	r0, [r4, #72]
	cmp	r0, #1
	beq	.L2367
.L2366:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	strb	r0, [r4, #16]
	b	.L2368
.L2320:
	mov	r2, r0
	ldr	r1, .L2447+88
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2427:
	mov	r2, r0
	ldr	r1, .L2447+92
	mov	r0, r7
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2428:
	mov	r0, r7
	ldr	r1, .L2447+96
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2429:
	mov	r0, r7
	ldr	r1, .L2447+100
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2432:
	ldr	r1, .L2447+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2433:
	ldr	r1, .L2447+108
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2383:
	ldr	r1, .L2447+112
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2419
.L2442:
	ldr	r3, [r4, #72]
	cmp	r3, #1
	ble	.L2345
.L2344:
	add	r8, r4, #130
	mov	r7, #0
	b	.L2356
.L2445:
	cmp	r3, r2
	bhi	.L2354
	ldr	r3, [r4, #72]
	add	r7, r7, #1
	sub	r3, r3, #1
	cmp	r3, r7
	ble	.L2345
.L2356:
	ldr	r1, .L2447+116
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	add	r0, r0, #1
	uxth	r3, r0
	strh	r3, [r8, #2]!	@ movhi
	ldr	r2, [r6, #244]
	sxth	r3, r3
	cmp	r3, #0
	bge	.L2445
.L2354:
	str	r2, [sp]
	mov	r0, #1
	mov	r2, r7
	ldr	r1, .L2447+120
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2438:
	mov	r3, #20
	ldr	r1, .L2447+124
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2444:
	ldr	r1, .L2447+128
	mov	r0, #1
	bl	dprint_vfmw
	str	r7, [r4, #76]
	b	.L2381
.L2345:
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2351
.L2352:
	ldr	r7, [r4, #68]
	ldrh	r3, [r6, #240]
	sub	r2, r7, #1
	add	ip, r4, r7, lsl #1
	cmp	r2, #0
	strh	r3, [ip, #174]	@ movhi
	ble	.L2361
	add	r8, r7, #44
	add	r1, r4, #90
	add	lr, r4, #174
	add	r8, r4, r8, lsl #1
.L2362:
	ldrh	r0, [r1, #2]!
	cmp	r1, r8
	strh	r0, [lr, #2]!	@ movhi
	ldrh	r3, [ip, #174]
	rsb	r3, r0, r3
	uxth	r3, r3
	strh	r3, [ip, #174]	@ movhi
	bne	.L2362
.L2361:
	sxth	r3, r3
	cmp	r3, #0
	ble	.L2446
	ldr	r1, [r4, #72]
	ldrh	r3, [r6, #244]
	sub	r2, r1, #1
	add	ip, r4, r1, lsl #1
	cmp	r2, #0
	strh	r3, [ip, #214]	@ movhi
	ble	.L2364
	add	r8, r1, #64
	add	lr, r4, #214
	add	r1, r4, #130
	add	r8, r4, r8, lsl #1
.L2365:
	ldrh	r0, [r1, #2]!
	cmp	r1, r8
	strh	r0, [lr, #2]!	@ movhi
	ldrh	r3, [ip, #214]
	rsb	r3, r0, r3
	uxth	r3, r3
	strh	r3, [ip, #214]	@ movhi
	bne	.L2365
.L2364:
	sxth	r3, r3
	cmp	r3, #0
	bgt	.L2358
	ldr	r1, .L2447+132
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2439:
	mov	r2, r3
	ldr	r1, .L2447+136
	mov	r3, #22
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2437:
	ldr	r1, .L2447+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2441:
	ldr	r3, [r4, #72]
	cmp	r3, #1
	bgt	.L2344
	b	.L2352
.L2446:
	ldr	r1, .L2447+144
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2415
.L2448:
	.align	2
.L2447:
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
	ldr	r1, .L2459
	mov	r4, r0
	ldr	r0, [r0, #2340]
	bl	HEVC_ue_v
	ldr	r3, [r4, #156]
	cmp	r0, r3
	mov	r6, r0
	bcs	.L2455
	movw	r7, #2024
	ldr	r3, [r4, #2352]
	mul	r5, r7, r0
	add	r2, r3, r5
	ldrb	r9, [r3, r5]	@ zero_extendqisi2
	cmp	r9, #0
	bne	.L2456
	str	r0, [r2, #28]
	mov	r0, r4
	ldr	r1, [r4, #2352]
	add	r1, r1, r5
	bl	HEVC_ProcessPPS
	subs	r8, r0, #0
	bne	.L2457
	ldr	r3, [r4, #2352]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #1]
	ldr	r3, [r4, #2352]
	strb	r2, [r3, r5]
.L2451:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2456:
	ldr	r9, .L2459+4
	mov	r0, r4
	ldr	r1, .L2459+8
	str	r6, [r9, #-3436]
	bl	HEVC_ProcessPPS
	subs	r8, r0, #0
	bne	.L2458
	ldr	ip, .L2459+12
	mov	r3, #1
	ldr	r0, [r4, #2352]
	mov	r2, r7
	ldr	r1, .L2459+8
	add	r0, r0, r5
	ldr	ip, [ip, #56]
	strb	r3, [r9, #-3463]
	strb	r3, [r9, #-3464]
	blx	ip
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2458:
	mov	r2, r6
	ldr	r1, .L2459+16
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L2451
.L2455:
	mov	r2, r0
	ldr	r1, .L2459+20
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L2451
.L2457:
	mov	r2, r6
	ldr	r1, .L2459+16
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #2352]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #1]
	ldr	r3, [r4, #2352]
	strb	r9, [r3, r5]
	b	.L2451
.L2460:
	.align	2
.L2459:
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
	ldr	r2, [r0, #2352]
	movw	ip, #2024
	ldr	r1, [r9, #1124]
	mov	r5, r0
	ldr	r0, [r9, #1108]
	ldr	r4, [r5, #2348]
	mla	r2, ip, r1, r2
	cmp	r0, #1
	ldr	r6, [r2, #32]
	beq	.L2526
	cmp	r0, #0
	bne	.L2465
	ldrb	r2, [r2, #11]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2465
.L2463:
	ldr	r1, .L2537
	ldr	r0, [r5, #2340]
	bl	HEVC_ue_v
	cmp	r0, #7
	str	r0, [r9, #2640]
	bhi	.L2527
	movw	r2, #26164
	mla	r8, r2, r6, r4
	ldr	r2, [r8, #64]
	cmp	r2, #0
	ldreq	r2, [r9, #2644]
	bne	.L2528
.L2469:
	add	r0, r0, r2
	str	r0, [r9, #3672]
	cmp	r0, #7
	bhi	.L2470
	ldr	r2, [r9, #1216]
	cmp	r2, #0
	addne	r4, r5, #55808
	addne	r4, r4, #84
	movne	r6, #0
	movne	r7, r4
	beq	.L2472
.L2473:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldr	r2, [r9, #1216]
	cmp	r2, r6
	bhi	.L2473
	ldr	r1, [r8, #64]
	cmp	r1, #0
	beq	.L2474
	cmp	r2, #0
	addne	r7, r5, #55808
	movne	r6, #0
	addne	r7, r7, #148
	beq	.L2472
.L2478:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldr	r2, [r9, #1216]
	cmp	r2, r6
	bhi	.L2478
.L2474:
	cmp	r2, #0
	beq	.L2472
	add	r6, r5, #56064
	mov	r7, #0
	add	r6, r6, #88
	mov	r3, r9
	str	r5, [fp, #-48]
	mov	r9, r8
	mov	r10, #1
	mov	r8, r7
	mov	r5, r6
	mov	r7, r3
	b	.L2489
.L2479:
	ldr	r1, [r7, #2640]
	str	r0, [r4, #192]
	mov	r1, r10, asl r1
	str	r1, [r4, #1028]
.L2481:
	ldr	r1, [r9, #64]
	cmp	r1, #0
	beq	.L2483
	ldr	r1, [r4, #64]
	cmp	r1, #0
	bne	.L2484
	ldr	r0, [r7, #3672]
	str	r1, [r5, #964]
	mov	r0, r10, asl r0
	str	r1, [r5, #968]
	str	r0, [r5, #836]
	str	r0, [r5, #840]
.L2483:
	ldr	r1, [r7, #1216]
	add	r8, r8, #1
	add	r5, r5, #8
	cmp	r1, r8
	bls	.L2529
.L2489:
	ldr	r0, [r4, #4]!
	cmp	r0, #0
	beq	.L2479
	ldr	r3, [fp, #-48]
	ldr	r1, .L2537+4
	ldr	r0, [r3, #2340]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r4, #128]
	cmp	r1, #255
	bhi	.L2530
	ldr	r3, [fp, #-48]
	ldr	lr, [r7, #2640]
	ldr	r1, .L2537+8
	add	r0, r0, r10, asl lr
	str	r0, [r4, #1028]
	ldr	r0, [r3, #2340]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r4, #192]
	cmp	r1, #255
	bls	.L2481
	mov	r2, r0
	ldr	r1, .L2537+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2526:
	ldrb	r2, [r2, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2463
.L2465:
	mov	r0, #0
.L2464:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2484:
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
.L2487:
	ldr	r3, [fp, #-48]
	add	r6, r6, #1
	ldr	r1, .L2537+16
	ldr	r0, [r3, #2340]
	bl	HEVC_se_v
	ldr	r1, .L2537+20
	add	r3, r0, #128
	str	r0, [r9]
	cmp	r3, #255
	bhi	.L2531
	ldr	r3, [fp, #-48]
	ldr	r0, [r3, #2340]
	bl	HEVC_se_v
	add	r3, r0, #512
	str	r0, [r8], #4
	cmp	r3, #1024
	bcs	.L2532
	ldr	r1, [r7, #3672]
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
	bne	.L2487
	ldr	r9, [fp, #-52]
	ldr	r4, [fp, #-56]
	ldr	r8, [fp, #-60]
	ldr	r5, [fp, #-64]
	b	.L2483
.L2529:
	ldr	r5, [fp, #-48]
	mov	r8, r9
	mov	r9, r7
.L2472:
	ldr	r4, [r9, #1108]
	cmp	r4, #0
	bne	.L2465
	ldr	r2, [r9, #1220]
	cmp	r2, #0
	beq	.L2465
	add	r10, r5, #56320
	add	r10, r10, #84
	mov	r6, r10
.L2490:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	add	r4, r4, #1
	str	r0, [r6, #4]!
	ldr	r2, [r9, #1220]
	cmp	r2, r4
	bhi	.L2490
	ldr	r1, [r8, #64]
	cmp	r1, #0
	bne	.L2533
.L2491:
	cmp	r2, #0
	beq	.L2465
	add	r7, r5, #56576
	mov	r3, #0
	add	r7, r7, #88
	str	r5, [fp, #-48]
	mov	r5, r9
	mov	r9, r8
	mov	r8, r7
	mov	r7, r3
	b	.L2505
.L2495:
	ldr	r1, [r5, #2640]
	mov	r3, #1
	str	r0, [r10, #192]
	mov	r1, r3, asl r1
	str	r1, [r10, #836]
.L2497:
	ldr	r1, [r9, #64]
	cmp	r1, #0
	beq	.L2499
	ldr	r1, [r10, #64]
	cmp	r1, #0
	bne	.L2500
	ldr	r0, [r5, #3672]
	mov	r3, #1
	str	r1, [r8, #772]
	mov	r0, r3, asl r0
	str	r1, [r8, #776]
	str	r0, [r8, #644]
	str	r0, [r8, #648]
.L2499:
	ldr	r1, [r5, #1220]
	add	r7, r7, #1
	add	r8, r8, #8
	cmp	r1, r7
	bls	.L2465
.L2505:
	ldr	r0, [r10, #4]!
	cmp	r0, #0
	beq	.L2495
	ldr	r3, [fp, #-48]
	ldr	r1, .L2537+24
	ldr	r0, [r3, #2340]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r10, #128]
	cmp	r1, #255
	bhi	.L2534
	ldr	lr, [r5, #2640]
	mov	r3, #1
	ldr	r1, .L2537+28
	add	r0, r0, r3, asl lr
	ldr	r3, [fp, #-48]
	str	r0, [r10, #836]
	ldr	r0, [r3, #2340]
	bl	HEVC_se_v
	add	r1, r0, #128
	str	r0, [r10, #192]
	cmp	r1, #255
	bls	.L2497
	ldr	r1, .L2537+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2500:
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
.L2503:
	ldr	r1, .L2537+36
	add	r4, r4, #1
	ldr	r0, [r10, #2340]
	bl	HEVC_se_v
	ldr	r1, .L2537+40
	add	r3, r0, #128
	str	r0, [r6]
	cmp	r3, #255
	bhi	.L2535
	ldr	r0, [r10, #2340]
	bl	HEVC_se_v
	add	r3, r0, #512
	str	r0, [r9], #4
	cmp	r3, #1024
	bcs	.L2536
	ldr	r1, [r5, #3672]
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
	bne	.L2503
	ldr	r8, [fp, #-52]
	ldr	r7, [fp, #-56]
	ldr	r9, [fp, #-60]
	ldr	r10, [fp, #-64]
	b	.L2499
.L2528:
	ldr	r1, .L2537+44
	ldr	r0, [r5, #2340]
	bl	HEVC_se_v
	add	r2, r0, #7
	str	r0, [r9, #2644]
	cmp	r2, #14
	bhi	.L2468
	mov	r2, r0
	ldr	r0, [r9, #2640]
	b	.L2469
.L2531:
	ldr	r1, .L2537+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2532:
	ldr	r1, .L2537+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2533:
	cmp	r2, #0
	addne	r6, r5, #56320
	movne	r4, #0
	addne	r6, r6, #148
	beq	.L2465
.L2494:
	mov	r1, #1
	ldr	r0, [r5, #2340]
	bl	BsGet
	add	r4, r4, #1
	str	r0, [r6, #4]!
	ldr	r2, [r9, #1220]
	cmp	r2, r4
	bhi	.L2494
	b	.L2491
.L2530:
	mov	r2, r0
	ldr	r1, .L2537+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2535:
	ldr	r1, .L2537+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2536:
	ldr	r1, .L2537+64
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2534:
	ldr	r1, .L2537+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2470:
	ldr	r1, .L2537+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2527:
	ldr	r1, .L2537+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2468:
	ldr	r1, .L2537+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2464
.L2538:
	.align	2
.L2537:
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
	ldr	r0, [r0, #2340]
	bl	BsGet
	add	r10, r4, #53248
	ldr	r7, [r10, #1120]
	sub	r3, r7, #16
	cmp	r3, #5
	movhi	r3, #0
	movls	r3, #1
	cmp	r3, #0
	str	r3, [fp, #-72]
	strb	r0, [r10, #1049]
	bne	.L2806
.L2540:
	ldr	r1, .L2849
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	cmp	r0, #0
	str	r0, [r10, #1124]
	ldr	r3, [r4, #156]
	blt	.L2542
	cmp	r0, r3
	bge	.L2542
	mov	r0, r4
	bl	HEVC_VpsSpsPpsCheck
	subs	r3, r0, #0
	str	r3, [fp, #-88]
	bne	.L2807
	ldr	r1, [r10, #1124]
	movw	r5, #2024
	ldr	r2, [r4, #2352]
	movw	r6, #26164
	ldr	r3, [r4, #2348]
	mla	r5, r5, r1, r2
	ldrb	r1, [r5, #2]	@ zero_extendqisi2
	ldr	r2, [r5, #32]
	cmp	r1, #0
	mla	r6, r6, r2, r3
	bne	.L2808
.L2548:
	mov	r3, #0
	strb	r3, [r10, #1051]
.L2549:
	ldr	r9, [r6, #160]
	ldr	r3, [r6, #164]
	cmp	r9, #0
	beq	.L2551
	cmp	r3, #0
	beq	.L2551
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
	ble	.L2553
.L2554:
	add	r1, r1, #1
	cmp	r8, r3, asl r1
	bgt	.L2554
.L2553:
	ldrb	r3, [r10, #1049]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r2, #0
	beq	.L2809
.L2555:
	mul	r9, r9, r8
	ldrb	r8, [r10, #1051]	@ zero_extendqisi2
	str	r2, [r10, #1200]
	cmp	r8, #0
	str	r9, [r10, #1204]
	ldr	r3, [r4, #188]
	str	r3, [r10, #1096]
	beq	.L2810
.L2558:
	ldr	r3, [r5, #12]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #0
	streq	r3, [r10, #1188]
	bne	.L2811
.L2667:
	ldrb	r3, [r5, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2672
.L2678:
	ldr	r3, [r10, #1200]
	cmp	r3, #0
	bne	.L2812
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2680
	ldr	r3, [fp, #-72]
	cmp	r3, #0
	movne	r3, #1
	strneb	r3, [r4, #3]
	bne	.L2682
.L2683:
	mov	r3, #0
	strb	r3, [r4, #2]
.L2684:
	mov	r0, r4
	bl	HEVC_IsNewPic
	str	r0, [r10, #2636]
	mov	r0, r4
	bl	HEVC_ReadByteAlignment
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
.L2786:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2812:
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2683
.L2682:
	ldr	r3, [r10, #1096]
	ldr	r2, [fp, #-72]
	cmp	r3, #0
	moveq	r2, #0
	andne	r2, r2, #1
	cmp	r2, #0
	beq	.L2683
	ldr	r3, [r10, #1120]
	cmp	r3, #21
	moveq	r3, #1
	streqb	r3, [r4, #2]
	bne	.L2683
	b	.L2684
.L2808:
	ldrb	r3, [r10, #1049]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2548
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1051]
	b	.L2549
.L2810:
	ldr	r3, [r5, #36]
	cmp	r3, #0
	beq	.L2563
.L2562:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r1, .L2849+4
	add	r8, r8, #1
	mov	r2, r0
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r3, [r5, #36]
	cmp	r3, r8
	bhi	.L2562
.L2563:
	ldr	r1, .L2849+8
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	cmp	r0, #2
	str	r0, [r10, #1108]
	bhi	.L2813
	ldrb	r3, [r5, #4]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r3, #1
	streqb	r3, [r10, #1048]
	bne	.L2814
.L2565:
	sub	r7, r7, #19
	cmp	r7, #1
	bhi	.L2815
	mov	r3, #0
	str	r3, [r10, #1096]
	ldr	r3, [r4, #220]
	cmp	r3, #0
	streq	r3, [r4, #188]
	ldr	r3, [r10, #1116]
	cmp	r3, #0
	streq	r3, [r10, #1100]
	streq	r3, [r4, #184]
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2797
.L2686:
	mov	r3, #0
	strb	r3, [r10, #1055]
	str	r3, [fp, #-92]
.L2688:
	ldr	r3, [r10, #1108]
	cmp	r3, #2
	beq	.L2611
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1056]
	cmp	r0, #0
	bne	.L2816
	ldr	r3, [r10, #1108]
	ldr	r2, [r5, #40]
	cmp	r3, #0
	str	r2, [r10, #1216]
	bne	.L2617
	ldr	r3, [r5, #44]
	str	r3, [r10, #1220]
.L2618:
	ldr	r3, [fp, #-92]
	cmp	r3, #0
	beq	.L2817
	ldr	r3, [fp, #-92]
	ldr	r2, [r3, #24]
	cmp	r2, #16
	bhi	.L2621
	cmp	r2, #0
	moveq	r3, r2
	beq	.L2623
	ldr	r3, [fp, #-92]
	add	r2, r2, #52
	add	r1, r3, #52
	add	r2, r3, r2
	mov	r3, #0
.L2625:
	ldrb	r0, [r1, #1]!	@ zero_extendqisi2
	cmp	r0, #0
	addne	r3, r3, #1
	cmp	r1, r2
	bne	.L2625
	cmp	r3, #1
	movle	r2, #0
	movgt	r2, #1
.L2623:
	ldrb	r1, [r5, #22]	@ zero_extendqisi2
	cmp	r1, #0
	moveq	r2, #0
	andne	r2, r2, #1
	cmp	r2, #0
	beq	.L2818
	add	r2, r4, #57600
	add	r1, r4, #54272
	add	r2, r2, #168
	add	r1, r1, #24
	mov	r0, r4
	bl	HEVC_DecRefPicListsModification
	cmp	r0, #0
	bne	.L2819
.L2619:
	ldr	r3, [r10, #1108]
	cmp	r3, #0
	beq	.L2690
.L2626:
	cmp	r3, #2
	beq	.L2611
.L2691:
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2820
.L2627:
	ldrb	r3, [r10, #1055]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r10, #1108]
	beq	.L2637
	cmp	r3, #0
	beq	.L2821
	cmp	r3, #2
	mov	r3, #1
	strb	r3, [r10, #1059]
	beq	.L2632
.L2635:
	ldr	r3, [r10, #1216]
	cmp	r3, #1
	bls	.L2794
.L2633:
	ldr	r1, .L2849+12
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	ldrb	r2, [r10, #1059]	@ zero_extendqisi2
	cmp	r2, #0
	str	r0, [r10, #1160]
	bne	.L2638
	ldr	r3, [r10, #1156]
	sub	r3, r3, #1
.L2640:
	cmp	r3, r0
	movcs	r1, #0
	movcc	r1, #1
	orrs	r1, r1, r0, lsr #31
	beq	.L2794
	str	r3, [sp]
	mov	r3, r0
	ldr	r1, .L2849+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2806:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1050]
	b	.L2540
.L2809:
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r10, #1128]
	cmp	r0, #0
	ldr	r2, [r6, #244]
	ldr	r3, [r6, #240]
	mul	r3, r3, r2
	blt	.L2557
	cmp	r3, r0
	bls	.L2557
	mul	r2, r9, r0
	b	.L2555
.L2811:
	ldr	r1, .L2849+20
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	str	r0, [r10, #1188]
	ldr	r3, [r5, #12]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	ldreq	r3, [r6, #244]
	subeq	r3, r3, #1
	beq	.L2663
	cmp	r3, #256
	ldreq	r2, [r5, #68]
	ldreq	r3, [r5, #72]
	ldrne	r3, [r6, #244]
	ldrne	r2, [r5, #68]
	mul	r3, r3, r2
	sub	r3, r3, #1
.L2663:
	cmp	r3, #255
	movcs	r3, #255
	cmp	r0, r3
	bhi	.L2822
	cmp	r0, #0
	beq	.L2667
	ldr	r1, .L2849+24
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	cmp	r0, #31
	add	r1, r0, #1
	str	r1, [r10, #1112]
	bhi	.L2823
	ldr	r3, [r10, #1188]
	cmp	r3, #0
	addne	r7, r4, #54272
	movne	r6, #0
	addne	r7, r7, #200
	bne	.L2671
	b	.L2667
.L2824:
	ldr	r1, [r10, #1112]
.L2671:
	ldr	r0, [r4, #2340]
	add	r6, r6, #1
	bl	BsGet
	str	r0, [r7, #4]!
	ldr	r3, [r10, #1188]
	cmp	r3, r6
	bhi	.L2824
	ldrb	r3, [r5, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2678
.L2672:
	ldr	r1, .L2849+28
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	cmp	r0, #256
	str	r0, [r10, #1192]
	bhi	.L2825
	ldr	r0, [r4, #2340]
	bl	BsResidBits
	ldr	r3, [r10, #1192]
	cmp	r0, r3
	bcc	.L2676
	cmp	r3, #0
	movne	r5, #0
	beq	.L2678
.L2679:
	mov	r1, #8
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r3, [r10, #1192]
	add	r5, r5, #1
	cmp	r3, r5
	str	r0, [r10, #1196]
	bhi	.L2679
	b	.L2678
.L2611:
	ldrb	r3, [r10, #1055]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2632
	mov	r3, #2
	mov	r2, #1
	strb	r2, [r10, #1059]
.L2637:
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2642
	cmp	r3, #1
	beq	.L2645
.L2642:
	ldrb	r2, [r5, #11]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2631
	cmp	r3, #0
	bne	.L2631
.L2645:
	mov	r0, r4
	bl	HEVC_DecPredWeightTable
	cmp	r0, #0
	ldreq	r3, [r10, #1108]
	bne	.L2826
.L2631:
	cmp	r3, #2
	beq	.L2646
	ldr	r1, .L2849+32
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	rsb	r3, r0, #4
	rsb	r2, r0, #5
	cmp	r3, #4
	str	r2, [r10, #1164]
	bhi	.L2827
.L2646:
	ldr	r1, .L2849+36
	ldr	r0, [r4, #2340]
	bl	HEVC_se_v
	str	r0, [r10, #1168]
	ldr	r2, [r5, #48]
	add	r2, r0, r2
	str	r2, [r10, #1104]
	ldr	r3, [r6, #104]
	cmp	r2, #51
	rsb	r0, r3, #0
	movle	r1, #0
	movgt	r1, #1
	cmp	r2, r0
	orrlt	r1, r1, #1
	cmp	r1, #0
	bne	.L2828
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2829
.L2648:
	ldrb	r3, [r5, #18]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2795
	ldrb	r3, [r5, #19]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r3, [r10, #1060]
	bne	.L2830
.L2656:
	ldrb	r0, [r5, #20]	@ zero_extendqisi2
	strb	r0, [r10, #1061]
	ldr	r3, [r5, #76]
	str	r3, [r10, #1180]
	ldr	r3, [r5, #80]
	str	r3, [r10, #1184]
.L2653:
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2659
	ldr	r2, [r10, #1052]
	bic	r3, r2, #-16777216
	bic	r3, r3, #255
	adds	r3, r3, #0
	movne	r3, #1
.L2659:
	ldrb	r2, [r5, #17]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2660
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	bne	.L2831
.L2660:
	strb	r2, [r10, #1062]
	b	.L2558
.L2680:
	ldr	r2, [fp, #-72]
	cmp	r2, #0
	strneb	r3, [r4, #3]
	b	.L2683
.L2818:
	ldr	r3, [r10, #1108]
	cmp	r3, #0
	bne	.L2691
.L2690:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r3, [r10, #1108]
	strb	r0, [r10, #1057]
	b	.L2626
.L2795:
	ldrb	r0, [r10, #1061]	@ zero_extendqisi2
	b	.L2653
.L2815:
	ldr	r1, [r6, #132]
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r2, #1
	str	r0, [r10, #1136]
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
	bls	.L2567
	ldr	r0, [fp, #-104]
	ldr	r2, [fp, #-100]
	rsb	r2, r2, r1
	cmp	r2, r0, lsr #1
	movcs	r2, r0
	addcs	r3, r3, r2
	bcc	.L2567
.L2568:
	ldr	r2, [r10, #1120]
	sub	r2, r2, #16
	cmp	r2, #2
	ldr	r2, [fp, #-100]
	movls	r3, #0
	add	r3, r3, r2
	str	r3, [r10, #1096]
	ldr	r2, [r4, #244]
	cmp	r3, r2
	bge	.L2570
	ldrb	r0, [r10, #1049]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L2832
.L2570:
	ldr	r3, [r4, #220]
	cmp	r3, #0
	ldreq	r3, [r10, #1096]
	streq	r3, [r4, #188]
	ldr	r3, [r10, #1116]
	cmp	r3, #0
	bne	.L2572
	ldr	r3, [r10, #1120]
	cmp	r3, #15
	bls	.L2833
.L2573:
	sub	r2, r3, #16
	cmp	r2, #7
	movhi	r2, #0
	movls	r2, #1
.L2574:
	sub	r3, r3, #7
	bics	r3, r3, #2
	andne	r3, r2, #1
	moveq	r3, #0
	cmp	r3, #0
	ldrne	r3, [r10, #1096]
	strne	r3, [r10, #1100]
	strne	r3, [r4, #184]
.L2572:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1052]
	cmp	r0, #0
	beq	.L2575
	ldr	r3, [r6, #228]
	cmp	r3, #1
	movhi	r1, #0
	movhi	r2, #1
	bls	.L2577
.L2579:
	add	r1, r1, #1
	cmp	r3, r2, asl r1
	bhi	.L2579
	cmp	r1, #0
	bne	.L2834
.L2577:
	mov	r1, #0
	str	r1, [r10, #1140]
.L2582:
	mov	r2, #204
	add	r0, r4, #57344
	mla	r1, r2, r1, r6
	add	r0, r0, #220
	str	r0, [fp, #-92]
	add	r1, r1, #13056
	add	r1, r1, #52
	bl	memcpy
.L2578:
	ldrb	r3, [r6, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2583
	add	r3, r4, #57344
	str	r3, [fp, #-84]
	mov	r1, r3
	ldr	r3, [r6, #232]
	ldr	r2, [r1, #240]
	cmp	r3, #0
	ldr	r3, [r1, #236]
	add	r3, r2, r3
	str	r3, [fp, #-108]
	bne	.L2584
.L2587:
	mov	r9, #0
.L2585:
	ldr	r1, .L2849+40
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	ldr	ip, [fp, #-84]
	ldr	r1, [r10, #1144]
	str	r0, [r10, #1148]
	add	r0, r0, r1
	ldr	r2, [ip, #236]
	ldr	r3, [ip, #240]
	str	r0, [ip, #252]
	add	lr, r3, r2
	add	lr, lr, r0
	cmp	lr, #16
	bhi	.L2835
	ldr	r3, [fp, #-108]
	cmp	r0, #0
	sub	r7, r3, #1
	add	r7, r0, r7
	beq	.L2603
	add	r3, r7, #13888
	add	r7, r7, #57600
	add	r3, r3, #51
	add	r7, r7, #17
	add	r7, r4, r7
	str	r7, [fp, #-64]
	mov	r3, r3, asl #2
	add	r8, r4, #55296
	add	ip, r3, #64
	str	ip, [fp, #-56]
	ldr	r7, [fp, #-56]
	add	r0, r4, #55552
	str	r8, [fp, #-48]
	add	lr, r4, #54272
	add	r8, r4, r7
	add	r7, r4, r3
	ldr	r3, [fp, #-48]
	mov	ip, r0
	str	r0, [fp, #-52]
	add	r0, r0, #136
	str	r0, [fp, #-60]
	add	r0, ip, #12
	add	ip, lr, #39
	add	lr, r3, #204
	ldr	r3, [fp, #-52]
	add	r2, r4, #54272
	str	r7, [fp, #-96]
	add	r2, r2, #54
	add	r3, r3, #76
	str	lr, [fp, #-48]
	str	r5, [fp, #-112]
	mov	lr, r3
	mov	r3, #0
	str	r6, [fp, #-76]
	mov	r7, lr
	str	r9, [fp, #-80]
	mov	r9, ip
	ldr	r5, [fp, #-64]
	str	r4, [fp, #-52]
	mov	r4, r3
	str	r10, [fp, #-64]
	mov	r10, r0
	str	r2, [fp, #-116]
	str	r3, [fp, #-68]
	str	r2, [fp, #-56]
	ldr	r6, [fp, #-96]
	b	.L2602
.L2838:
	ldr	r3, [fp, #-80]
	cmp	r3, #0
	ldreq	r0, [fp, #-80]
	bne	.L2836
.L2593:
	ldr	r2, [fp, #-76]
	add	r3, r2, r0
	add	r0, r0, #62
	ldrb	r1, [r3, #16]	@ zero_extendqisi2
	ldr	r3, [r2, r0, asl #2]
	strb	r1, [r5]
.L2596:
	ldr	r2, [fp, #-60]
	mov	r1, #1
	str	r3, [r2, #4]!
	ldr	r3, [fp, #-52]
	str	r2, [fp, #-60]
	ldr	r0, [r3, #2340]
	bl	BsGet
	ldr	r3, [fp, #-56]
	uxtb	r0, r0
	strb	r0, [r3, #1]!
	cmp	r0, #0
	str	r3, [fp, #-56]
	bne	.L2837
	cmp	r4, #0
	streq	r4, [fp, #-68]
	beq	.L2600
	ldr	r3, [fp, #-64]
	ldr	r2, [fp, #-68]
	ldr	r3, [r3, #1144]
	cmp	r3, r4
	moveq	r2, #0
	str	r2, [fp, #-68]
.L2600:
	mov	r3, #0
	str	r3, [r8]
.L2599:
	ldr	r3, [fp, #-84]
	add	r4, r4, #1
	ldr	r2, [fp, #-48]
	sub	r6, r6, #4
	sub	r8, r8, #4
	sub	r5, r5, #1
	ldr	r3, [r3, #252]
	add	r2, r2, #4
	add	r10, r10, #4
	add	r9, r9, #1
	cmp	r3, r4
	str	r2, [fp, #-48]
	add	r7, r7, #4
	bls	.L2601
	ldr	r3, [fp, #-64]
	ldr	r1, [r3, #1144]
.L2602:
	cmp	r1, r4
	bhi	.L2838
	ldr	r3, [fp, #-76]
	ldr	r1, [r3, #132]
	ldr	r3, [fp, #-52]
	ldr	r0, [r3, #2340]
	bl	BsGet
	ldr	r3, [fp, #-52]
	mov	r1, #1
	str	r0, [r10]
	str	r0, [fp, #-96]
	ldr	r0, [r3, #2340]
	bl	BsGet
	ldr	r3, [fp, #-96]
	uxtb	r0, r0
	strb	r0, [r9]
	strb	r0, [r5]
	b	.L2596
.L2567:
	ldr	r2, [fp, #-100]
	cmp	r1, r2
	bcs	.L2568
	rsb	r1, r1, r2
	ldr	r2, [fp, #-104]
	cmp	r1, r2, lsr #1
	rsbhi	r3, r2, r3
	b	.L2568
.L2814:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1048]
	b	.L2565
.L2829:
	ldr	r1, .L2849+44
	ldr	r0, [r4, #2340]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r10, #1172]
	cmp	r3, #24
	bhi	.L2839
	ldr	r3, [r5, #60]
	add	r2, r0, r3
	add	r2, r2, #12
	cmp	r2, #24
	bhi	.L2840
	ldr	r1, .L2849+48
	ldr	r0, [r4, #2340]
	bl	HEVC_se_v
	add	r3, r0, #12
	str	r0, [r10, #1176]
	cmp	r3, #24
	bhi	.L2841
	ldr	r2, [r5, #64]
	add	r2, r0, r2
	add	r3, r2, #12
	cmp	r3, #24
	bls	.L2648
	ldr	r1, .L2849+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2542:
	str	r3, [sp]
	mov	r3, r0
	ldr	r2, .L2849+56
	mov	r0, #1
	ldr	r1, .L2849+60
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2794:
	ldr	r3, [r10, #1108]
	b	.L2637
.L2583:
	ldrb	r3, [r6, #11]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r3, [r10, #1055]
	bne	.L2842
.L2608:
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2688
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r10, #1053]
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1054]
	b	.L2688
.L2797:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #1
	strb	r0, [r10, #1053]
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1054]
	b	.L2686
.L2820:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1058]
	b	.L2627
.L2617:
	str	r0, [r10, #1220]
.L2616:
	cmp	r3, #2
	bne	.L2618
	b	.L2619
.L2816:
	ldr	r1, .L2849+64
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	add	r2, r0, #1
	str	r2, [r10, #1152]
	cmp	r2, #16
	bhi	.L2843
	ldr	r3, [r10, #1108]
	str	r2, [r10, #1216]
	cmp	r3, #0
	movne	r2, #0
	strne	r2, [r10, #1156]
	strne	r2, [r10, #1220]
	bne	.L2616
	ldr	r1, .L2849+68
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	add	r2, r0, #1
	str	r2, [r10, #1156]
	cmp	r2, #16
	strls	r2, [r10, #1220]
	ldrls	r3, [r10, #1108]
	bls	.L2616
	mov	r3, #16
	ldr	r1, .L2849+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2821:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	ldr	r3, [r10, #1108]
	cmp	r3, #2
	uxtb	r0, r0
	strb	r0, [r10, #1059]
	beq	.L2646
	cmp	r0, #0
	bne	.L2635
	ldr	r2, [r10, #1220]
	cmp	r2, #1
	bhi	.L2633
	b	.L2637
.L2632:
	mov	r3, #2
	b	.L2642
.L2638:
	ldr	r3, [r10, #1152]
	sub	r3, r3, #1
	b	.L2640
.L2833:
	tst	r3, #1
	movne	r2, #1
	bne	.L2574
	b	.L2573
.L2584:
	ldr	r1, .L2849+76
	ldr	r0, [r4, #2340]
	bl	HEVC_ue_v
	str	r0, [r10, #1144]
	mov	r2, r0
	ldr	r3, [r6, #232]
	cmp	r0, r3
	bhi	.L2844
	cmp	r3, #1
	bls	.L2587
	mov	r9, #0
	mov	r2, #1
.L2588:
	add	r9, r9, #1
	cmp	r3, r2, asl r9
	bhi	.L2588
	str	r9, [fp, #-88]
	b	.L2585
.L2832:
	ldr	r1, .L2849+80
	bl	dprint_vfmw
	b	.L2570
.L2676:
	mov	r2, r0
	ldr	r1, .L2849+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2837:
	ldr	r3, [fp, #-52]
	ldr	r1, .L2849+88
	ldr	r0, [r3, #2340]
	bl	HEVC_ue_v
	cmp	r4, #0
	str	r0, [r7]
	beq	.L2598
	ldr	r3, [fp, #-64]
	ldr	r3, [r3, #1144]
	cmp	r3, r4
	ldrne	r3, [fp, #-68]
	addne	r0, r0, r3
.L2598:
	str	r0, [r6]
	mov	r3, #1
	str	r0, [fp, #-68]
	str	r3, [r8]
	b	.L2599
.L2836:
	ldr	r3, [fp, #-52]
	ldr	r1, [fp, #-88]
	ldr	r0, [r3, #2340]
	bl	BsGet
	ldr	r3, [fp, #-48]
	cmp	r0, #0
	str	r0, [r3]
	blt	.L2845
	ldr	r3, [fp, #-76]
	ldr	r3, [r3, #232]
	sub	r3, r3, #1
	cmp	r3, r0
	bcs	.L2593
	mov	r7, r4
.L2595:
	str	r3, [sp]
	mov	r2, r7
	mov	r3, r0
	ldr	r1, .L2849+92
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2551:
	mov	r2, r9
	ldr	r1, .L2849+96
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2831:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1062]
	b	.L2558
.L2830:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1060]
	cmp	r0, #0
	beq	.L2656
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r10, #1061]
	cmp	r0, #0
	bne	.L2653
	ldr	r1, .L2849+100
	ldr	r0, [r4, #2340]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r10, #1180]
	bhi	.L2846
.L2657:
	ldr	r1, .L2849+104
	ldr	r0, [r4, #2340]
	bl	HEVC_se_v
	add	r3, r0, #6
	mov	r2, r0
	cmp	r3, #12
	str	r0, [r10, #1184]
	bls	.L2795
	ldr	r1, .L2849+108
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #80]
	ldrb	r0, [r10, #1061]	@ zero_extendqisi2
	str	r3, [r10, #1184]
	b	.L2653
.L2601:
	ldr	r2, [fp, #-108]
	cmp	r3, #0
	ldr	r5, [fp, #-112]
	add	r2, r2, #14400
	ldr	r6, [fp, #-76]
	ldr	r4, [fp, #-52]
	add	r2, r2, #25
	ldr	r10, [fp, #-64]
	beq	.L2603
	add	r2, r2, r3
	add	lr, r4, #55552
	add	lr, lr, #140
	mov	r7, #0
	add	r2, r4, r2, lsl #2
	ldr	r8, [fp, #-104]
	ldr	ip, [fp, #-116]
	ldr	r9, [fp, #-100]
	str	r4, [fp, #-48]
	b	.L2606
.L2848:
	ldr	r0, [r10, #1096]
	ldr	r4, [lr]
	rsb	r1, r9, r0
	add	r1, r1, r4
	ldr	r4, [r2, #-1948]
	mls	r1, r4, r8, r1
	rsb	r0, r0, r1
	str	r0, [r2, #-68]
	str	r1, [r2]
.L2605:
	add	r7, r7, #1
	add	lr, lr, #4
	cmp	r3, r7
	sub	r2, r2, #4
	bls	.L2847
.L2606:
	ldrb	r1, [ip, #1]!	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2848
	ldr	r1, [lr]
	str	r1, [r2]
	ldr	r0, [r10, #1096]
	rsb	r1, r0, r1
	str	r1, [r2, #-68]
	b	.L2605
.L2807:
	ldr	r3, [r4, #248]
	mov	r0, r4
	cmp	r3, #0
	ldrne	r3, [r4, #188]
	str	r3, [r10, #1096]
	bl	HEVC_IsNewPic
	ldr	r1, .L2849+112
	str	r0, [r10, #2636]
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2847:
	ldr	r4, [fp, #-48]
.L2591:
	ldr	r2, [fp, #-108]
	add	r2, r3, r2
	ldr	r3, [fp, #-84]
	cmp	r2, #16
	str	r2, [r3, #244]
	bls	.L2583
	mov	r3, #16
	ldr	r1, .L2849+116
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2575:
	add	r7, r4, #57344
	ldr	r3, [r6, #228]
	add	r7, r7, #220
	mov	r1, r6
	mov	r0, r4
	mov	r2, r7
	bl	HEVC_DecShortTermRefPicSet
	cmp	r0, #0
	streq	r7, [fp, #-92]
	beq	.L2578
	ldr	r1, .L2849+120
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2842:
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsGet
	strb	r0, [r10, #1055]
	b	.L2608
.L2850:
	.align	2
.L2849:
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
.L2834:
	ldr	r0, [r4, #2340]
	bl	BsGet
	str	r0, [r10, #1140]
	cmp	r0, #0
	ldr	r3, [r6, #228]
	mov	r1, r0
	sub	r3, r3, #1
	blt	.L2581
	cmp	r3, r0
	bcs	.L2582
.L2581:
	mov	r2, r1
	mov	r0, #1
	ldr	r1, .L2849+124
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2828:
	ldr	r1, .L2849+128
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2557:
	mov	r2, r0
	ldr	r1, .L2849+132
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2825:
	mov	r2, r0
	ldr	r1, .L2849+136
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2845:
	ldr	r6, [fp, #-76]
	mov	r7, r4
	ldr	r3, [r6, #232]
	sub	r3, r3, #1
	b	.L2595
.L2813:
	mov	r2, r0
	ldr	r1, .L2849+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2621:
	mov	r3, #16
	ldr	r1, .L2849+144
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2827:
	ldr	r1, .L2849+148
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2822:
	mov	r2, r0
	ldr	r1, .L2849+152
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2846:
	ldr	r1, .L2849+156
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #76]
	str	r3, [r10, #1180]
	b	.L2657
.L2840:
	mov	r2, r0
	ldr	r1, .L2849+160
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2603:
	mov	r3, #0
	b	.L2591
.L2826:
	ldr	r1, .L2849+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2841:
	mov	r2, r0
	ldr	r1, .L2849+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2839:
	mov	r2, r0
	ldr	r1, .L2849+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2835:
	str	r0, [sp]
	mov	ip, #16
	ldr	r1, .L2849+176
	mov	r0, #1
	str	ip, [sp, #4]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2819:
	ldr	r1, .L2849+180
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2843:
	mov	r3, #16
	ldr	r1, .L2849+184
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2817:
	ldr	r1, .L2849+188
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2823:
	mov	r2, r1
	mov	r0, #1
	ldr	r1, .L2849+192
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
.L2844:
	ldr	r1, .L2849+196
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2786
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
	ldr	r1, [r0, #2364]
	mov	r4, r0
	cmp	r1, #0
	beq	.L2852
	ldr	r0, [r0, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L2852:
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
	ldr	r1, [r0, #2364]
	mov	r4, r0
	cmp	r1, #0
	beq	.L2858
	ldr	r0, [r0, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L2858:
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
	beq	.L2866
	mov	r4, #0
	add	r8, r0, #65536
	mov	r5, r4
	mov	r6, r4
.L2865:
	ldr	r3, [r8, #2680]
	add	r5, r5, #1
	add	r3, r3, r4
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq	.L2864
	ldr	r0, [r7, #276]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r8, #2680]
	add	r3, r3, r4
	str	r6, [r3, #24]
.L2864:
	ldr	r3, [r7, #160]
	add	r4, r4, #28
	cmp	r3, r5
	bhi	.L2865
.L2866:
	mov	r0, #0
	mov	r3, #2
	str	r0, [r7, #208]
	str	r0, [r7, #256]
	str	r0, [r7, #248]
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
	add	r9, r5, #2544
	mov	r4, r5
	add	r9, r9, #12
	mov	r6, #17
	mov	r7, #0
.L2877:
	ldr	r3, [r9, #4]!
	mov	r2, #1
	cmp	r3, #0
	beq	.L2873
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #276]
	bl	FSP_ClearLogicFs
.L2873:
	ldr	r3, [r4, #2628]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2874
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #276]
	bl	FSP_ClearLogicFs
.L2874:
	ldr	r3, [r4, #2696]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2875
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #276]
	bl	FSP_ClearLogicFs
.L2875:
	ldr	r3, [r4, #2764]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2876
	ldr	r1, [r3, #32]
	ldr	r0, [r5, #276]
	bl	FSP_ClearLogicFs
.L2876:
	subs	r6, r6, #1
	str	r7, [r4, #2764]
	str	r7, [r4, #2696]
	add	r4, r4, #4
	str	r7, [r4, #2624]
	str	r7, [r4, #2556]
	bne	.L2877
	add	r4, r5, #24576
	mov	r7, #0
	mov	r1, r7
	mov	r2, #200
	ldr	r3, [r4, #2868]
	add	r0, r5, #2832
	ldr	r3, [r3, #236]
	str	r7, [r5, #2540]
	add	r3, r3, #1
	str	r7, [r5, #2552]
	cmp	r3, #17
	str	r7, [r5, #2556]
	str	r7, [r5, #2548]
	movhi	r3, #17
	str	r3, [r5, #2536]
	ldr	r3, .L2919
	str	r7, [r5, #2544]
	ldr	r3, [r3, #48]
	blx	r3
	add	r2, r5, #2352
	add	r2, r2, #12
	add	r3, r5, #2432
	mov	r0, r7
	mov	r1, #16
	str	r7, [r5, #2836]
	str	r1, [r5, #2832]
.L2880:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L2880
	add	r2, r5, #2496
	add	ip, r5, #53248
	add	r2, r2, #4
	mov	r0, r1
	str	r1, [ip, #1208]
.L2881:
	str	r0, [r3, #4]!
	cmp	r3, r2
	mov	r1, #0
	bne	.L2881
	cmp	r8, r1
	str	r1, [ip, #1212]
	bne	.L2882
	ldr	r3, [r5, #2356]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L2882
.L2883:
	ldr	r1, [r5, #180]
	cmp	r1, #0
	addne	r2, r5, #2144
	movne	r3, #0
	movne	r0, r3
	beq	.L2891
.L2890:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L2890
.L2891:
	ldr	r3, [r5, #160]
	cmp	r3, #0
	addne	r8, r5, #65536
	movne	r4, #0
	movne	r7, r4
	beq	.L2889
.L2893:
	ldr	r3, [r8, #2680]
	add	r4, r4, #1
	add	r3, r3, r6
	ldr	r3, [r3, #24]
	cmp	r3, #0
	mov	r1, r3
	beq	.L2892
	ldr	r2, [r5, #2364]
	cmp	r3, r2
	beq	.L2892
	ldr	r0, [r5, #276]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r8, #2680]
	add	r3, r3, r6
	str	r7, [r3, #24]
.L2892:
	ldr	r3, [r5, #160]
	add	r6, r6, #28
	cmp	r3, r4
	bhi	.L2893
.L2889:
	mov	r0, #0
.L2884:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2882:
	mov	r7, #20
	str	r7, [r5, #176]
	str	r7, [r5, #180]
	mov	r0, r5
	ldr	r1, [r4, #2868]
	ldr	r3, [r1, #240]
	ldr	ip, [r1, #244]
	ldr	r2, [r1, #148]
	str	r7, [sp, #8]
	mov	r1, r3, asl r2
	str	ip, [sp]
	mov	r2, ip, asl r2
	str	r7, [sp, #4]
	bl	HEVC_ArrangeVahbMem
	ldr	r3, [r5, #2356]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L2894
	cmp	r0, #1
	bne	.L2917
	ldr	r0, [r5, #276]
	bl	FSP_GetTotalValidFsNum
	cmp	r0, #20
	movgt	r0, r7
	ble	.L2918
.L2886:
	str	r0, [r5, #176]
	b	.L2883
.L2918:
	ldr	r0, [r5, #276]
	bl	FSP_GetTotalValidFsNum
	b	.L2886
.L2917:
	ldr	r2, .L2919+4
	mov	r0, #0
	ldr	r1, .L2919+8
	bl	dprint_vfmw
	mvn	r0, #4
	b	.L2884
.L2894:
	mvn	r0, #1
	b	.L2884
.L2920:
	.align	2
.L2919:
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
	beq	.L2925
	mov	r4, #0
	add	r7, r0, #65536
	mov	r5, r4
	b	.L2924
.L2923:
	ldr	r3, [r6, #160]
	add	r3, r3, #1
	cmp	r3, r5
	bls	.L2925
.L2924:
	ldr	r1, [r7, #1876]
	add	r5, r5, #1
	add	r1, r1, r4
	add	r4, r4, #100
	ldr	r3, [r1, #36]
	cmp	r3, #1
	bne	.L2923
	ldr	r0, [r6, #276]
	bl	HEVC_ReleaseNAL
	ldr	r3, [r6, #160]
	add	r3, r3, #1
	cmp	r3, r5
	bhi	.L2924
.L2925:
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
	ldr	r1, .L2941
	mov	r4, r0
	mov	r0, #22
	add	r5, r4, #67584
	bl	dprint_vfmw
	mov	r3, #-2147483647
	mov	r0, r4
	str	r3, [r4, #244]
	bl	HEVC_ClearAllNal
	add	r0, r4, #61696
	add	r0, r0, #64
	ldr	r6, .L2941+4
	bl	ResetVoQueue
	mov	r8, #0
	add	r5, r5, #712
	mov	r9, #20
	mov	r7, r8
.L2929:
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
	bne	.L2929
	add	r3, r4, #2144
	add	r1, r4, #2224
	mov	r2, #0
.L2930:
	str	r2, [r3, #4]!
	cmp	r3, r1
	bne	.L2930
	ldr	r3, [r4, #160]
	cmp	r3, #0
	addne	ip, r4, #65536
	movne	r3, #0
	movne	r0, r3
	movne	r1, r3
	beq	.L2934
.L2933:
	ldr	r2, [ip, #2680]
	add	r0, r0, #1
	str	r1, [r2, r3]
	ldr	r2, [ip, #2680]
	add	r2, r2, r3
	str	r1, [r2, #20]
	ldr	r2, [ip, #2680]
	add	r2, r2, r3
	add	r3, r3, #28
	str	r1, [r2, #24]
	ldr	r2, [r4, #160]
	cmp	r2, r0
	bhi	.L2933
.L2934:
	ldr	r3, [r6, #48]
	mov	r2, #200
	mov	r1, #0
	add	r0, r4, #2832
	blx	r3
	add	ip, r4, #2752
	add	r0, r4, #2688
	add	r1, r4, #2544
	add	ip, ip, #8
	add	r0, r0, #4
	add	r1, r1, #12
	add	lr, r4, #2624
	mov	r3, #16
	mov	r2, #17
	str	r3, [r4, #2832]
	mov	r3, #0
.L2932:
	subs	r2, r2, #1
	str	r3, [ip, #4]!
	str	r3, [r0, #4]!
	str	r3, [lr, #4]!
	str	r3, [r1, #4]!
	bne	.L2932
	add	r1, r4, #2352
	add	ip, r4, #2496
	add	r1, r1, #12
	add	ip, ip, #4
	add	r3, r4, #2432
	mov	r0, r2
	mov	lr, #17
	str	r2, [r4, #2540]
	str	r2, [r4, #2544]
	str	r2, [r4, #2548]
	str	r2, [r4, #2552]
	str	r2, [r4, #2556]
	str	lr, [r4, #2536]
.L2935:
	str	r0, [r3, #4]!
	cmp	r3, ip
	str	r0, [r1, #4]!
	mov	r5, #0
	bne	.L2935
	add	r0, r4, #54272
	mov	r9, #2
	mov	r1, r5
	ldr	r3, [r6, #48]
	str	r5, [r4, #2364]
	movw	r2, #3732
	str	r5, [r4, #208]
	add	r0, r0, #24
	str	r5, [r4, #256]
	add	r7, r4, #2496
	str	r5, [r4, #248]
	add	r7, r7, #8
	str	r9, [r4, #172]
	mov	r8, #32
	blx	r3
	add	r2, r4, #53248
	mov	r3, #1
	mov	lr, #20
	str	r9, [r2, #1108]
	mov	ip, #16
	str	r3, [r2, #2636]
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
	str	r9, [r4, #2240]
	mvn	r9, #-2147483648
	str	r5, [r4, #168]
	movw	r5, #1025
	str	r8, [r4, #352]
	mov	r8, #4
	str	lr, [r4, #176]
	str	lr, [r4, #180]
	mov	lr, #6
	str	r3, [r4, #192]
	str	r3, [r4, #196]
	str	r3, [r4, #344]
	mov	r3, #256
	ldr	r6, [r6, #48]
	str	r9, [r4, #200]
	str	ip, [r4, #324]
	str	ip, [r4, #348]
	str	r10, [r4, #328]
	str	r3, [r4, #332]
	str	r5, [r4, #336]
	str	r8, [r4, #340]
	str	lr, [r4, #2228]
	str	lr, [r4, #2232]
	str	lr, [r4, #2236]
	blx	r6
	str	r7, [r4, #2340]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2942:
	.align	2
.L2941:
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
	ldr	r7, .L2956
	movw	r2, #20256
	ldr	r5, [r8, #664]
	mov	r4, r0
	mov	r9, r1
	movt	r2, 1
	ldr	r3, [r7, #48]
	mov	r1, #0
	blx	r3
	str	r5, [r8, #664]
	mov	r3, #0
	str	r9, [r4, #2356]
	str	r3, [r4, #2360]
	ldr	r3, [r9, #28]
	cmp	r3, #25
	beq	.L2951
	mov	r3, #17
	mov	r1, #65
	str	r3, [r4, #148]
	mov	r2, #200
	str	r3, [r4, #152]
	mov	r3, #210
	str	r1, [r4, #156]
	str	r2, [r4, #144]
	str	r3, [r4, #160]
.L2945:
	ldr	r0, [r9, #584]
	bl	MEM_Phy2Vir
	subs	r5, r0, #0
	beq	.L2952
	ldr	r3, [r4, #148]
	movw	r2, #23196
	ldr	r10, [r4, #2356]
	add	r6, r4, #65536
	ldr	r0, [r4, #152]
	mov	lr, #100
	mul	r3, r2, r3
	ldr	ip, [r10, #580]
	ldr	r2, [r4, #156]
	add	r1, r5, ip
	str	r1, [r4, #2344]
	add	ip, r3, ip
	str	r3, [fp, #-52]
	add	r3, r5, ip
	str	r3, [r4, #2348]
	movw	r3, #26164
	mla	ip, r3, r0, ip
	add	r3, r5, ip
	str	r3, [r4, #2352]
	movw	r3, #2024
	mla	ip, r3, r2, ip
	mov	r3, #1360
	add	r0, r5, ip
	str	r0, [r6, #1872]
	ldr	r2, [r4, #144]
	mul	r2, r3, r2
	add	ip, ip, r2
	add	r3, r5, ip
	str	r3, [r6, #1876]
	ldr	r3, [r4, #160]
	mla	lr, r3, lr, lr
	mov	r3, r3, asl #5
	str	lr, [fp, #-48]
	ldr	lr, [r4, #160]
	sub	r3, r3, lr, asl #2
	ldr	lr, [fp, #-48]
	add	ip, ip, lr
	add	r5, r5, ip
	str	r5, [r6, #2680]
	ldr	r5, [r10, #588]
	add	r3, ip, r3
	cmp	r3, r5
	bhi	.L2953
	ldr	r5, [r9, #608]
	cmp	r5, #0
	beq	.L2954
.L2949:
	ldr	r3, [r7, #48]
	mov	r1, #0
	blx	r3
	ldr	ip, [r4, #160]
	mov	r2, #100
	ldr	r3, [r7, #48]
	mov	r1, #0
	ldr	r0, [r6, #1876]
	mla	r2, ip, r2, r2
	blx	r3
	ldr	r3, [r4, #160]
	ldr	ip, [r7, #48]
	mov	r1, #0
	ldr	r0, [r6, #2680]
	mov	r5, #1
	mov	r2, r3, asl #5
	sub	r2, r2, r3, asl #2
	blx	ip
	ldr	lr, [r4, #148]
	movw	r3, #23196
	ldr	r6, [r4, #2344]
	add	ip, r4, #24576
	movw	r1, #2024
	mul	r3, r3, lr
	movw	r2, #26164
	mov	r0, r4
	sub	r3, r3, #23040
	sub	r3, r3, #156
	add	r6, r6, r3
	str	r6, [ip, #2864]
	ldr	r3, [r4, #156]
	ldr	r7, [r4, #2352]
	mul	r3, r1, r3
	sub	r3, r3, #2016
	sub	r3, r3, #8
	add	r3, r7, r3
	str	r3, [ip, #2872]
	ldr	r3, [r4, #152]
	ldr	r1, [r4, #2348]
	mul	r3, r2, r3
	sub	r3, r3, #26112
	sub	r3, r3, #52
	add	r3, r1, r3
	str	r3, [ip, #2868]
	str	lr, [r6, #8]
	ldr	r3, [ip, #2872]
	ldr	r2, [r4, #152]
	str	r2, [r3, #28]
	ldr	r3, [ip, #2868]
	ldr	r2, [r4, #156]
	str	r2, [r3, #60]
	bl	HEVC_InitScalingOrderTable
	add	r3, r4, #81920
	mov	r0, r4
	str	r5, [r3, #3860]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #276]
	beq	.L2955
	mov	r3, #-2147483647
	mov	r0, r4
	str	r3, [r4, #244]
	bl	HEVC_InitDecPara
	mov	r3, #0
	str	r3, [r8, #672]
	movw	r1, #9999
	ldr	ip, [r4, #156]
	mov	r0, r3
	ldr	r2, [r4, #2356]
	str	r3, [r4, #240]
	strb	r5, [r4]
	str	ip, [r4, #320]
	str	r1, [r4, #284]
	ldr	r3, [r2, #624]
	str	r3, [r4, #212]
.L2947:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2954:
	ldr	r2, [fp, #-52]
	mov	r0, r1
	ldr	r3, [r7, #48]
	mov	r1, r5
	blx	r3
	ldr	r2, [r4, #152]
	movw	ip, #26164
	ldr	r3, [r7, #48]
	mov	r1, r5
	ldr	r0, [r4, #2348]
	mul	r2, ip, r2
	blx	r3
	ldr	r2, [r4, #156]
	movw	ip, #2024
	ldr	r3, [r7, #48]
	mov	r1, r5
	ldr	r0, [r4, #2352]
	mul	r2, ip, r2
	blx	r3
	ldr	r2, [r4, #144]
	mov	r3, #1360
	ldr	r0, [r6, #1872]
	mul	r2, r3, r2
	b	.L2949
.L2951:
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
	b	.L2945
.L2953:
	str	r5, [sp]
	mov	r0, #0
	ldr	r2, .L2956+4
	ldr	r1, .L2956+8
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L2947
.L2955:
	ldr	r2, .L2956+4
	mov	r0, #0
	ldr	r1, .L2956+12
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L2947
.L2952:
	ldr	r2, .L2956+4
	ldr	r1, .L2956+16
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L2947
.L2957:
	.align	2
.L2956:
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
	ldr	r3, [r4, #420]
	cmp	r3, #0
	beq	.L2959
	ldr	r1, [r3, #32]
	mov	r2, #1
	ldr	r0, [r5, #276]
	bl	FSP_ClearLogicFs
	ldr	r2, [r4, #420]
	mov	r3, #0
	str	r3, [r2, #4]
	str	r3, [r4, #420]
.L2959:
	mov	r0, #0
	strb	r0, [r4, #369]
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
	bne	.L2966
	add	r0, r4, #61696
	add	r0, r0, #64
	bl	ResetVoQueue
	ldr	r0, [r4, #276]
	bl	FSP_EmptyInstance
.L2965:
	mov	r0, r4
	bl	HEVC_InitDecPara
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2966:
	add	r1, r4, #61696
	ldr	r0, [r4, #276]
	add	r1, r1, #64
	bl	FSP_ClearNotInVoQueue
	b	.L2965
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
	ldr	r3, [r0, #288]
	mov	r4, r0
	add	r8, r0, #24576
	add	r5, r0, #53248
	str	r3, [fp, #-60]
	ldr	r3, [r4, #292]
	ldr	r2, [r8, #2868]
	ldr	r0, [r5, #1124]
	str	r3, [fp, #-48]
	ldr	r3, [r4, #296]
	cmp	r0, #0
	str	r3, [fp, #-52]
	ldr	r3, [r2, #100]
	str	r3, [fp, #-68]
	ldr	r3, [r2, #108]
	str	r3, [fp, #-72]
	ldr	r3, [r4, #300]
	str	r3, [fp, #-56]
	blt	.L2970
	ldr	r3, [r4, #156]
	cmp	r0, r3
	bge	.L2970
	ldr	lr, [r4, #2352]
	movw	r6, #2024
	movw	ip, #26164
	ldr	r3, [r4, #2348]
	ldr	r7, [r4, #2344]
	movw	r1, #23196
	mla	r6, r6, r0, lr
	ldr	r0, [r8, #2864]
	ldr	r0, [r0, #8]
	ldr	r10, [r6, #32]
	mul	r10, ip, r10
	add	r9, r3, r10
	ldr	ip, [r9, #52]
	mla	r1, r1, ip, r7
	ldr	ip, [r1, #8]
	cmp	r0, ip
	bne	.L2971
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L3028
.L2971:
	mov	r2, #0
	strb	r2, [r1, #2]
	ldr	r0, [r4, #148]
	movw	r7, #23196
	ldr	ip, .L3034
	mov	r2, r7
	ldr	lr, [r4, #2344]
	mul	r0, r7, r0
	ldr	ip, [ip, #52]
	str	r3, [fp, #-64]
	sub	r0, r0, #23040
	sub	r0, r0, #156
	add	r0, lr, r0
	blx	ip
	ldr	r0, [r4, #148]
	ldr	r2, [r8, #2868]
	ldr	r3, [fp, #-64]
	mul	r7, r7, r0
	ldr	r1, [r4, #2344]
	sub	r7, r7, #23040
	sub	r7, r7, #156
	add	r7, r1, r7
	str	r7, [r8, #2864]
.L2972:
	ldr	r0, [r2, #60]
	ldr	r1, [r9, #60]
	cmp	r0, r1
	beq	.L3029
.L2973:
	mov	r2, #0
	strb	r2, [r3, r10]
	ldr	r0, [r4, #152]
	movw	r7, #26164
	ldr	ip, [r8, #2868]
	mov	r2, r7
	ldr	lr, [r9, #236]
	mov	r1, r9
	mul	r0, r7, r0
	ldr	ip, [ip, #236]
	ldr	r3, .L3034
	ldr	r10, [r4, #2348]
	subs	ip, lr, ip
	sub	r0, r0, #26112
	sub	r0, r0, #52
	ldr	r3, [r3, #52]
	add	r0, r10, r0
	movne	ip, #1
	str	ip, [fp, #-64]
	blx	r3
	ldr	r3, [r4, #152]
	ldr	r2, [r4, #2348]
	mul	r7, r7, r3
	sub	r7, r7, #26112
	sub	r7, r7, #52
	add	r2, r2, r7
	str	r2, [r8, #2868]
	ldr	r3, [r2, #60]
	str	r3, [r4, #288]
	ldr	r3, [r2, #72]
	str	r3, [r4, #292]
	ldr	r3, [r2, #76]
	str	r3, [r4, #296]
	ldr	r3, [r2, #148]
	str	r3, [r4, #300]
.L2974:
	ldr	r3, [r2, #108]
	ldr	r2, [r2, #100]
	ldr	r1, [r4, #2356]
	cmp	r3, r2
	movcc	r3, r2
	str	r3, [r1, #884]
	ldr	r2, [r8, #2872]
	ldr	r3, [r6, #28]
	ldr	r2, [r2, #28]
	cmp	r2, r3
	beq	.L3030
.L2975:
	ldrb	r3, [r6, #13]	@ zero_extendqisi2
	mov	r7, #0
	strb	r7, [r6, #1]
	cmp	r3, r7
	beq	.L2977
	ldrb	r3, [r6, #15]	@ zero_extendqisi2
	cmp	r3, r7
	beq	.L2978
	ldr	r10, [r6, #68]
	cmp	r10, r7
	ble	.L2983
	add	ip, r6, #174
	str	r4, [fp, #-80]
	str	r6, [fp, #-76]
	mov	r4, ip
	str	r5, [fp, #-84]
.L2982:
	ldr	r6, [r9, #240]
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
	bne	.L2982
	ldr	r6, [fp, #-76]
	ldr	r4, [fp, #-80]
	ldr	r5, [fp, #-84]
.L2983:
	ldr	r10, [r6, #72]
	cmp	r10, #0
	ble	.L2984
	add	ip, r6, #214
	str	r4, [fp, #-80]
	mov	r7, #0
	str	r6, [fp, #-76]
	mov	r4, ip
	str	r5, [fp, #-84]
.L2985:
	ldr	r6, [r9, #244]
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
	bne	.L2985
	ldr	r6, [fp, #-76]
	ldr	r4, [fp, #-80]
	ldr	r5, [fp, #-84]
	b	.L2984
.L2970:
	mov	r0, #0
.L2969:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2977:
	ldr	r3, [r9, #240]
	strh	r3, [r6, #176]	@ movhi
	ldr	r3, [r9, #244]
	strh	r3, [r6, #216]	@ movhi
.L2984:
	ldr	r0, [r4, #156]
	movw	r7, #2024
	ldr	r3, .L3034
	mov	r2, r7
	ldr	ip, [r4, #2352]
	mov	r1, r6
	mul	r0, r7, r0
	ldr	r3, [r3, #52]
	sub	r0, r0, #2016
	sub	r0, r0, #8
	add	r0, ip, r0
	blx	r3
	ldr	r2, [r4, #156]
	ldr	r3, [r4, #2352]
	mul	r7, r7, r2
	sub	r7, r7, #2016
	sub	r7, r7, #8
	add	r7, r3, r7
	str	r7, [r8, #2872]
.L2976:
	ldr	r3, [r4, #220]
	cmp	r3, #0
	bne	.L2991
	ldr	r3, [r9, #60]
	ldr	r2, [fp, #-64]
	ldr	r1, [fp, #-60]
	cmp	r1, r3
	cmpeq	r2, #0
	bne	.L2992
	ldr	r3, [r4, #168]
	cmp	r3, #0
	beq	.L2992
	ldr	r3, [r9, #72]
	ldr	r2, [fp, #-48]
	cmp	r2, r3
	bne	.L2992
	ldr	r3, [r9, #76]
	ldr	r2, [fp, #-52]
	cmp	r2, r3
	bne	.L2992
	ldr	r3, [r4, #2356]
	ldr	r2, [r3, #872]
	cmp	r2, #1
	bne	.L2994
.L2993:
	ldr	r3, [r3, #888]
	cmp	r3, #1
	beq	.L3031
.L2997:
	add	r3, r4, #81920
	ldr	r3, [r3, #3860]
	cmp	r3, #1
	beq	.L3032
.L2998:
	mov	r1, #1
	mov	r0, r4
	bl	HEVC_InitDPB
	cmn	r0, #2
	beq	.L2969
	cmp	r0, #0
	bne	.L2999
	ldr	r3, [r4, #2356]
.L2996:
	mov	r2, #0
	str	r2, [r3, #888]
.L2994:
	mov	r0, r4
	bl	HEVC_AllocFrameStore
	subs	r6, r0, #0
	bne	.L3033
.L2991:
	ldr	r1, [r5, #1120]
	mov	r0, #1
	ldr	r2, [r5, #1096]
	mvn	r3, #0
	strb	r0, [r5, #369]
	mov	r9, #0
	str	r1, [r5, #372]
	add	r10, r4, #54272
	str	r2, [r5, #376]
	movw	r1, #54288
	str	r3, [r5, #400]
	mvn	r6, #0
	ldr	r3, [r8, #2868]
	mvn	r7, #0
	add	r0, r4, #53504
	add	r0, r0, #168
	ldr	r2, [r3, #72]
	str	r2, [r5, #384]
	ldr	r2, [r3, #76]
	str	r2, [r5, #388]
	ldr	r2, [r3, #240]
	ldr	r3, [r3, #244]
	str	r9, [r5, #396]
	mul	r3, r3, r2
	str	r3, [r5, #392]
	ldr	ip, [r4, #2356]
	ldrd	r2, [ip, #56]
	strd	r2, [r10]
	ldrd	r2, [ip, #72]
	strd	r2, [r10, #8]
	ldrd	r2, [ip, #88]
	strd	r2, [r4, r1]
	ldr	r3, [ip, #104]
	str	r3, [r5, #432]
	ldr	r3, [ip, #108]
	str	r3, [r5, #436]
	ldr	r3, [ip, #112]
	str	r3, [r5, #440]
	ldr	r3, [ip, #128]
	str	r3, [r5, #444]
	strd	r6, [ip, #56]
	ldr	r2, [r8, #2868]
	ldr	r3, [r2, #72]
	str	r3, [r5, #536]
	ldr	r3, [r2, #76]
	str	r3, [r5, #540]
	ldr	r3, [r2, #2660]
	ldr	r1, [r2, #64]
	mov	r3, r3, asl #5
	subs	r1, r1, #1
	uxtb	r3, r3
	movne	r1, #1
	orr	r3, r3, r1, asl #2
	str	r3, [r5, #532]
	ldr	r1, [r2, #2644]
	and	r1, r1, #7
	bl	SetAspectRatio
	str	r9, [r5, #520]
	ldr	r3, [r4, #220]
	cmp	r3, #1
	beq	.L2970
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #1096]
	str	r2, [r3, #12]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #532]
	str	r2, [r3, #180]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #520]
	str	r2, [r3, #168]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #428]
	str	r2, [r3, #76]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #424]
	str	r2, [r3, #72]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #536]
	str	r2, [r3, #184]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #540]
	str	r2, [r3, #188]
	ldr	r3, [r5, #420]
	ldr	r0, [r4, #276]
	ldr	r1, [r3, #32]
	bl	FSP_GetDispPhyFs
	cmp	r0, #0
	beq	.L3000
	ldr	r3, [r5, #520]
	mov	r1, r9
	ldr	r2, [r0, #8]
	subs	r3, r3, r9
	ldr	r0, [r4, #276]
	movne	r3, #1
	bl	FSP_SetStoreType
.L3000:
	ldr	r3, [r8, #2868]
	mov	r4, #0
	ldr	lr, [r5, #420]
	mov	r0, r4
	ldr	r2, [r3, #84]
	str	r2, [r5, #756]
	ldr	r7, [r3, #88]
	str	r7, [r5, #760]
	ldr	r1, [r3, #92]
	str	r1, [r5, #764]
	ldr	r6, [r3, #96]
	str	r6, [r5, #768]
	ldr	ip, [r3, #72]
	rsb	r2, r2, ip
	rsb	r2, r7, r2
	str	r2, [r5, #544]
	ldr	ip, [r3, #76]
	rsb	r1, r1, ip
	rsb	r1, r6, r1
	str	r1, [r5, #548]
	ldr	r1, [r3, #64]
	str	r1, [r5, #596]
	ldr	r1, [r3, #100]
	str	r1, [r5, #600]
	ldr	r3, [r3, #108]
	str	r3, [r5, #604]
	ldr	r3, [lr, #200]
	str	r3, [r5, #552]
	ldr	r3, [lr, #204]
	str	r3, [r5, #556]
	str	r2, [lr, #192]
	ldr	r2, [r5, #548]
	ldr	r3, [r5, #420]
	str	r2, [r3, #196]
	ldr	r2, [r5, #552]
	ldr	r3, [r5, #420]
	str	r2, [r3, #200]
	ldr	r2, [r5, #556]
	ldr	r3, [r5, #420]
	str	r2, [r3, #204]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #600]
	str	r2, [r3, #248]
	ldr	r3, [r5, #420]
	ldr	r2, [r5, #604]
	str	r2, [r3, #252]
	str	r4, [r5, #380]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2992:
	ldr	r3, [fp, #-64]
	mov	r0, r4
	ldr	r2, [fp, #-52]
	ldr	r1, [fp, #-48]
	str	r3, [sp]
	ldr	r3, [fp, #-56]
	bl	HEVC_GetReRangeFlag
	ldr	r3, [r4, #2356]
	cmp	r0, #0
	bne	.L2993
	ldr	r2, [r3, #872]
	cmp	r2, #1
	bne	.L2996
	b	.L2993
.L2978:
	ldr	lr, [r6, #68]
	ldr	r3, [r9, #240]
	cmp	lr, #1
	add	r0, r6, lr, lsl #1
	strh	r3, [r0, #174]	@ movhi
	ble	.L2989
	add	lr, lr, #44
	add	r1, r6, #90
	add	ip, r6, #174
	add	lr, r6, lr, lsl #1
.L2988:
	ldrh	r2, [r1, #2]!
	cmp	r1, lr
	strh	r2, [ip, #2]!	@ movhi
	ldrh	r3, [r0, #174]
	rsb	r3, r2, r3
	strh	r3, [r0, #174]	@ movhi
	bne	.L2988
.L2989:
	ldr	lr, [r6, #72]
	ldr	r3, [r9, #244]
	cmp	lr, #1
	add	r0, r6, lr, lsl #1
	strh	r3, [r0, #214]	@ movhi
	ble	.L2984
	add	lr, lr, #64
	add	r1, r6, #130
	add	ip, r6, #214
	add	lr, r6, lr, lsl #1
.L2990:
	ldrh	r2, [r1, #2]!
	cmp	r1, lr
	strh	r2, [ip, #2]!	@ movhi
	ldrh	r3, [r0, #214]
	rsb	r3, r2, r3
	strh	r3, [r0, #214]	@ movhi
	bne	.L2990
	b	.L2984
.L3028:
	ldr	r0, [r4, #168]
	cmp	r0, #0
	bne	.L2972
	b	.L2971
.L3029:
	ldrb	r1, [r3, r10]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2973
	ldr	r0, [r4, #168]
	cmp	r0, #0
	beq	.L2973
	ldr	r0, [r9, #72]
	ldr	ip, [fp, #-48]
	cmp	ip, r0
	bne	.L2973
	ldr	r0, [r9, #76]
	ldr	ip, [fp, #-52]
	cmp	ip, r0
	bne	.L2973
	ldr	r0, [r9, #148]
	ldr	ip, [fp, #-56]
	cmp	ip, r0
	streq	r1, [fp, #-64]
	bne	.L2973
	b	.L2974
.L3030:
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2975
	ldr	r3, [r4, #168]
	cmp	r3, #0
	beq	.L2975
	ldr	r3, [r9, #72]
	ldr	r2, [fp, #-48]
	cmp	r2, r3
	bne	.L2975
	ldr	r3, [r9, #76]
	ldr	r2, [fp, #-52]
	cmp	r2, r3
	bne	.L2975
	ldr	r3, [r9, #148]
	ldr	r2, [fp, #-56]
	cmp	r2, r3
	bne	.L2975
	b	.L2976
.L3033:
	ldr	r1, .L3034+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	b	.L2969
.L3031:
	ldr	ip, [r9, #108]
	mov	r0, #31
	ldr	r1, [fp, #-72]
	ldr	r3, [r9, #100]
	ldr	r2, [fp, #-68]
	str	r1, [sp]
	str	ip, [sp, #4]
	ldr	r1, .L3034+8
	bl	dprint_vfmw
	b	.L2997
.L3032:
	mov	r0, r4
	bl	HEVC_GetBackPicFromVOQueue
	b	.L2998
.L2999:
	mov	r2, r0
	ldr	r1, .L3034+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	mov	r1, #0
	bl	HEVC_ClearAll
	mvn	r0, #0
	b	.L2969
.L3035:
	.align	2
.L3034:
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
	beq	.L3044
	cmn	r6, #3
	moveq	r0, #0
	bne	.L3045
.L3039:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3044:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_SetDisplay
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L3046
	add	r3, r4, #61696
	ldr	r0, [r4, #276]
	add	r3, r3, #64
	str	r7, [sp]
	mov	r2, r4
	mov	r1, #16
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L3047
	ldr	r3, [r4, #2356]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	strcc	r6, [r7, #24]
	bcs	.L3048
.L3042:
	ldr	r2, [r4, #316]
	mov	r0, #0
	ldr	r3, [r4, #308]
	add	r2, r2, #1
	str	r2, [r4, #316]
	add	r3, r3, #1
	str	r3, [r4, #308]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3048:
	mov	r1, r7
	mov	r0, r4
	bl	HEVC_SetFrmRepeatCount.part.4
	b	.L3042
.L3045:
	mov	r3, r6
	movw	r2, #9561
	ldr	r1, .L3049
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r5, #228]
	mov	r0, r4
	bl	HEVC_DECRecycleImage
	mvn	r0, #0
	b	.L3039
.L3047:
	mov	r2, r0
	ldr	r1, .L3049+4
	mov	r0, r6
	bl	dprint_vfmw
	mov	r2, r6
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_SetDisplay
	mov	r0, r4
	mov	r1, #1
	bl	HEVC_ClearAll
	mvn	r0, #0
	b	.L3039
.L3046:
	ldr	r2, .L3049+8
	ldr	r1, .L3049+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3039
.L3050:
	.align	2
.L3049:
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
	ldr	r4, [r6, #420]
	mov	r7, r0
	add	r1, r6, #424
	mov	r2, #624
	add	r0, r4, #72
	str	ip, [r4, #4]
	strb	r3, [r4, #2]
	strb	r5, [r4, #1]
	ldr	r3, [r6, #380]
	str	r3, [r4, #16]
	ldr	r3, [r6, #408]
	str	r3, [r4, #20]
	bl	memcpy
	str	r5, [r4, #48]
	ldr	r3, [r6, #408]
	mov	r2, r5
	ldr	r1, [r4, #32]
	str	r3, [r4, #40]
	ldr	r0, [r7, #276]
	bl	FSP_SetRef
	mov	r1, r4
	mov	r0, r7
	bl	HEVC_OutputFrmToVO
	ldr	r3, .L3052
	str	r5, [r4, #4]
	mov	r1, r5
	mov	r2, #680
	ldr	r3, [r3, #48]
	mov	r6, r0
	add	r0, r7, #53504
	add	r0, r0, #112
	blx	r3
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3053:
	.align	2
.L3052:
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
	ldr	r3, [r2, #404]
	cmp	r3, r1
	addne	r3, r3, #536
	movne	ip, #0
	addne	r4, r4, r3, lsl #2
	strne	ip, [r4, #4]
	strne	r1, [r2, #404]
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
	ldr	ip, [r0, #2540]
	mov	r4, r0
	cmp	ip, #0
	beq	.L3070
	add	r1, r0, #2544
	mov	r7, #0
	add	r1, r1, #12
	mov	r3, r7
.L3059:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L3058
	ldr	r0, [r2, #12]
	ldr	r2, [r4, #244]
	cmp	r0, r2
	addgt	r7, r7, #1
.L3058:
	cmp	r3, ip
	bne	.L3059
.L3057:
	ldr	r3, [r4, #228]
	cmp	r3, #1
	beq	.L3082
	add	r9, r4, #53248
	add	r2, r4, #24576
	ldr	r1, [r9, #1116]
	ldr	r3, [r2, #2868]
	add	r3, r3, r1, lsl #2
	ldr	r8, [r3, #412]
.L3061:
	cmp	ip, #0
	beq	.L3071
	add	r6, r4, #2544
	add	r9, r4, #53248
	add	r6, r6, #12
	mov	r5, #0
	str	r5, [fp, #-48]
.L3068:
	ldr	r1, [r6, #4]!
	mov	r2, r5
	ldr	ip, [r4, #244]
	mov	r0, #22
	ldrb	r10, [r4, #2]	@ zero_extendqisi2
	ldr	lr, [r9, #1096]
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	str	r8, [sp, #12]
	str	r7, [sp, #8]
	str	r10, [sp, #20]
	str	lr, [sp, #16]
	str	ip, [sp, #4]
	ldr	ip, [r1, #12]
	ldr	r1, .L3084
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r1, [r6]
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3063
	cmp	r7, r8
	ldr	r0, [r1, #12]
	ldr	r3, [r4, #244]
	movls	ip, #0
	movhi	ip, #1
	cmp	r0, r3
	movle	ip, #0
	cmp	ip, #0
	beq	.L3064
	ldr	ip, [r9, #1096]
	cmp	r0, ip
	blt	.L3065
	ldrb	r0, [r4, #2]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L3065
.L3064:
	cmp	r3, #0
	bne	.L3066
	ldr	r0, [r1, #12]
	cmp	r0, #0
	bge	.L3066
	cmp	r2, #0
	beq	.L3083
.L3067:
	str	r0, [sp]
	mov	r0, #31
	ldr	r2, .L3084+4
	ldr	r1, .L3084+8
	bl	dprint_vfmw
.L3066:
	ldr	r3, [r4, #2540]
	add	r5, r5, #1
	cmp	r3, r5
	bhi	.L3068
.L3081:
	ldr	r0, [fp, #-48]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3065:
	mov	r3, #1
	mov	r0, r4
	strb	r3, [r1, #2]
	sub	r7, r7, #1
	ldr	r1, [r6]
	bl	HEVC_OutputFrmToVO
	ldr	r3, [r6]
	ldr	r3, [r3, #12]
	str	r3, [r4, #244]
	str	r0, [fp, #-48]
	b	.L3066
.L3063:
	ldr	r3, [r4, #244]
	b	.L3064
.L3083:
	mov	r2, r3
	ldr	r0, [r4, #276]
	ldr	r1, [r1, #32]
	bl	FSP_SetDisplay
	ldr	r2, [r6]
	ldr	r3, [r4, #244]
	ldr	r0, [r2, #12]
	b	.L3067
.L3082:
	add	r3, r4, #24576
	mov	r8, #0
	ldr	r1, [r3, #2868]
	add	r3, r1, #408
	add	r1, r1, #440
.L3060:
	ldr	r2, [r3, #4]!
	cmp	r8, r2
	movcc	r8, r2
	cmp	r3, r1
	bne	.L3060
	b	.L3061
.L3070:
	mov	r7, ip
	b	.L3057
.L3071:
	str	ip, [fp, #-48]
	b	.L3081
.L3085:
	.align	2
.L3084:
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
	ldr	r3, [r4, #2560]
	strb	r2, [r3, #2]
	ldr	r1, [r4, #2560]
	bl	HEVC_OutputFrmToVO
	ldr	r3, [r4, #2560]
	ldr	r2, [r3, #12]
	str	r2, [r5, #244]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r7, r0
	bne	.L3087
	mov	r1, r6
	mov	r0, r5
	bl	HEVC_RemoveApcStore
	mov	r1, r6
	mov	r0, r5
	bl	HEVC_RemoveFrameStoreOutDPB
.L3087:
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
	bne	.L3092
	mov	r0, r4
	ldr	r1, [fp, #-24]
	bl	HEVC_OutputOneFrmFromDPB
	cmp	r0, #0
	bne	.L3093
.L3090:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3092:
	ldr	r2, .L3094
	mov	r0, #1
	ldr	r1, .L3094+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3090
.L3093:
	ldr	r2, .L3094
	mov	r0, #1
	ldr	r1, .L3094+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3090
.L3095:
	.align	2
.L3094:
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
	beq	.L3105
	ldr	r3, [r4, #2540]
	cmp	r3, #0
	addne	r5, r4, #2544
	movne	r6, #0
	addne	r5, r5, #12
	movne	r8, #1
	bne	.L3103
	b	.L3104
.L3111:
	strb	r8, [r3, #2]
	ldr	r1, [r5]
	bl	HEVC_OutputFrmToVO
	cmn	r0, #1
	mov	r9, r0
	beq	.L3110
.L3101:
	ldr	r3, [r4, #2540]
	cmp	r3, r6
	bls	.L3104
.L3103:
	ldr	r3, [r5, #4]!
	mov	r2, #0
	ldr	r0, [r4, #276]
	add	r6, r6, #1
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldr	r3, [r5]
	mov	r0, r4
	ldrb	r7, [r3, #2]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L3111
	ldr	r1, [r3, #32]
	ldr	r0, [r4, #276]
	bl	FSP_GetDisplay
	mov	r2, #0
	cmp	r0, #3
	beq	.L3101
	ldr	r3, [r5]
	ldr	r0, [r4, #276]
	ldr	r1, [r3, #32]
	bl	FSP_SetDisplay
	ldr	r3, [r4, #2540]
	cmp	r3, r6
	bhi	.L3103
.L3104:
	mov	r3, #-2147483647
	mov	r0, r4
	str	r3, [r4, #244]
	mov	r9, #0
	bl	Hevc_InitDecBuffers
.L3097:
	mov	r0, r9
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3110:
	mov	r0, r7
	ldr	r2, .L3112
	ldr	r1, .L3112+4
	bl	dprint_vfmw
	mov	r0, r9
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3105:
	mvn	r9, #0
	b	.L3097
.L3113:
	.align	2
.L3112:
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
	str	r3, [r4, #3840]
	ldr	r6, [r0, #2364]
	cmp	r6, r3
	beq	.L3136
	ldrb	r3, [r1]	@ zero_extendqisi2
	str	r3, [r6, #40]
	ldr	r3, [r0, #2356]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3137
.L3120:
	ldrb	r3, [r1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3121
	ldr	r3, [r5, #2364]
	ldr	r2, [r3, #8]
	cmp	r2, #1
	bls	.L3122
	ldr	r0, [r3, #40]
	cmp	r0, #1
	beq	.L3121
.L3122:
	ldr	r0, [r1, #12]
	cmp	r0, #0
	ble	.L3123
	ldr	ip, [r1, #4]
	cmp	ip, #0
	beq	.L3123
	ldr	r0, [r1, #8]
	cmp	r0, #0
	bne	.L3138
.L3123:
	mov	r2, #1
	str	r2, [r3, #40]
	ldr	r1, [r1, #16]
	ldr	r0, [r5, #276]
	bl	SM_ReleaseStreamSeg
.L3119:
	ldr	r3, [r5, #2364]
	ldr	r0, [r3, #40]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
.L3118:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3137:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	bne	.L3119
	b	.L3120
.L3138:
	mov	r0, r2, asl #5
	mov	lr, #0
	sub	r2, r0, r2, asl #2
	mov	r0, r5
	add	r3, r3, r2
	str	ip, [r3, #44]
	ldr	r3, [r5, #2364]
	ldr	r6, [r1, #12]
	ldr	ip, [r3, #8]
	mov	r2, ip, asl #5
	sub	r2, r2, ip, asl #2
	add	r3, r3, r2
	str	r6, [r3, #48]
	ldr	r3, [r5, #2364]
	ldr	r6, [r1, #8]
	ldr	ip, [r3, #8]
	mov	r2, ip, asl #5
	sub	r2, r2, ip, asl #2
	add	r3, r3, r2
	str	r6, [r3, #52]
	ldr	r3, [r5, #2364]
	ldr	ip, [r1, #16]
	ldr	r1, [r3, #8]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	ip, [r3, #68]
	ldr	r3, [r5, #2364]
	ldr	r1, [r3, #8]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	lr, [r3, #60]
	ldr	r2, [r5, #2364]
	ldr	r3, [r2, #8]
	add	r3, r3, #1
	str	r3, [r2, #8]
	bl	HEVC_CombinePacket
	mov	r3, #1
	str	r3, [r4, #3840]
	b	.L3119
.L3136:
	ldr	r3, [r1, #12]
	cmp	r3, #0
	ble	.L3116
	ldr	r3, [r1, #4]
	cmp	r3, #0
	beq	.L3116
	ldr	r3, [r1, #8]
	str	r1, [fp, #-32]
	cmp	r3, #0
	beq	.L3116
	bl	HEVC_FindNaluArraySlot
	ldr	r1, [fp, #-32]
	cmn	r0, #1
	mov	r4, r0
	beq	.L3139
	add	r3, r5, #65536
	mov	r0, #100
	mov	r2, #1
	ldr	r3, [r3, #1876]
	mla	r4, r0, r4, r3
	str	r4, [r5, #2364]
	ldr	r3, [r1, #4]
	str	r3, [r4, #44]
	ldr	r0, [r1, #12]
	ldr	r3, [r5, #2364]
	str	r0, [r3, #48]
	ldr	r0, [r1, #16]
	ldr	r3, [r5, #2364]
	str	r0, [r3, #68]
	ldr	r3, [r5, #2364]
	ldr	r0, [r1, #8]
	str	r0, [r3, #52]
	ldr	r3, [r5, #2364]
	str	r6, [r3, #60]
	ldr	r3, [r5, #2364]
	str	r2, [r3, #8]
	ldr	r3, [r5, #2364]
	strb	r6, [r3, #1]
	ldr	r3, [r5, #2364]
	ldrb	r2, [r1]	@ zero_extendqisi2
	str	r2, [r3, #40]
	ldr	r3, [r1, #20]
	str	r3, [r5, #216]
	b	.L3119
.L3121:
	ldr	r1, .L3140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3118
.L3116:
	ldr	r1, .L3140+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3118
.L3139:
	ldr	r1, .L3140+8
	mov	r0, r6
	bl	dprint_vfmw
	mov	r1, r6
	mov	r0, r5
	bl	HEVC_ClearAll
	mov	r0, r4
	b	.L3118
.L3141:
	.align	2
.L3140:
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
	beq	.L3145
	add	r3, r0, #53248
	ldrb	r3, [r3, #369]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3145
	add	r2, r0, #67584
	add	r0, r0, #81920
	add	r2, r2, #640
	add	r0, r0, #384
.L3151:
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne	.L3146
	cmp	r2, #0
	beq	.L3147
	ldr	lr, [r4, #2540]
	cmp	lr, #0
	beq	.L3147
	ldr	r1, [r4, #2560]
	cmp	r1, r2
	beq	.L3146
	add	r1, r4, #2560
	b	.L3148
.L3149:
	ldr	ip, [r1, #4]!
	cmp	ip, r2
	beq	.L3146
.L3148:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L3149
.L3147:
	ldr	r0, [r4, #276]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L3145
	cmn	r0, #1
	beq	.L3170
.L3169:
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3145:
	mov	r0, #1
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3146:
	add	r2, r2, #704
	cmp	r2, r0
	bne	.L3151
	add	r0, r4, #61696
	sub	r2, fp, #24
	sub	r1, fp, #28
	add	r0, r0, #64
	bl	GetQueueImgNum
	mov	r0, r4
	bl	HEVC_PrintFrameStoreState
	ldr	r1, .L3171
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_PrintDPBState
	ldr	r2, [r4, #176]
	ldr	r0, [r4, #2540]
	ldr	r3, [fp, #-28]
	sub	r2, r2, #2
	ldr	r1, [fp, #-24]
	rsb	r2, r0, r2
	add	r0, r3, r1
	cmp	r0, r2
	bge	.L3169
	str	r1, [sp]
	mov	r0, #0
	ldr	r2, .L3171+4
	ldr	r1, .L3171+8
	bl	dprint_vfmw
	mov	r0, r4
	mov	r1, #1
	bl	HEVC_ClearAll
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3170:
	add	r1, r4, #61696
	ldr	r0, [r4, #276]
	add	r1, r1, #64
	bl	FSP_ClearNotInVoQueue
	b	.L3169
.L3172:
	.align	2
.L3171:
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
	ldr	r3, [r0, #2540]
	mov	r4, r0
	cmp	r3, #0
	beq	.L3175
	add	r6, r0, #2544
	mov	r5, #0
	add	r6, r6, #12
	mov	r7, r5
	b	.L3177
.L3176:
	ldr	r3, [r4, #2540]
	cmp	r3, r5
	bls	.L3175
.L3177:
	ldr	r3, [r6, #4]!
	add	r5, r5, #1
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3176
	strb	r7, [r3, #1]
	mov	r2, #0
	ldr	r1, [r3, #32]
	ldr	r0, [r4, #276]
	bl	FSP_SetRef
	ldr	r3, [r4, #2540]
	cmp	r3, r5
	bhi	.L3177
.L3175:
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L3179
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L3175
	b	.L3179
.L3181:
	bl	HEVC_GetMinPOC
	ldr	r1, [fp, #-36]
	mov	r0, r4
	bl	HEVC_OutputOneFrmFromDPB
	cmn	r0, #1
	beq	.L3180
.L3179:
	ldr	r3, [r4, #2540]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	bne	.L3181
	mov	r0, r3
.L3180:
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
	beq	.L3190
	ldr	r2, .L3194
	mov	r0, #0
	ldr	r1, .L3194+4
	bl	dprint_vfmw
.L3190:
	mov	r0, r4
	mov	r1, #0
	bl	HEVC_InitDPB
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3195:
	.align	2
.L3194:
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
	b	.L3197
.L3199:
	str	lr, [sp]
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_IncreaseDPBSize
	cmp	r0, #0
	bne	.L3205
.L3197:
	ldr	ip, [r4, #2540]
	mov	r0, #13
	ldr	lr, [r4, #2536]
	ldr	r2, .L3206
	cmp	ip, lr
	ldr	r1, .L3206+4
	mov	r3, ip
	bcs	.L3199
	b	.L3201
.L3202:
	str	lr, [sp]
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_OutputUnRefPicWithMinPoc
	cmp	r0, #0
	bne	.L3200
	ldr	ip, [r4, #2540]
	ldr	lr, [r4, #2536]
.L3201:
	cmp	ip, lr
	mov	r3, ip
	ldr	r2, .L3206
	mov	r0, #1
	ldr	r1, .L3206+8
	bcs	.L3202
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3205:
	ldr	r2, .L3206
	mov	r0, #1
	ldr	r1, .L3206+12
	bl	dprint_vfmw
	ldr	ip, [r4, #2540]
	ldr	lr, [r4, #2536]
	b	.L3201
.L3200:
	ldr	r2, .L3206
	mov	r0, #1
	ldr	r1, .L3206+16
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_PrintDPBState
	mov	r0, r4
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	HEVC_ClearDPB
.L3207:
	.align	2
.L3206:
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
	bhi	.L3212
	ldr	r3, [r2, #52]
	add	r5, r0, r1, lsl #2
	mov	r1, #1
	mov	r2, r1
	str	r3, [r5, #2560]
	str	r1, [r3, #4]
	ldr	r3, [r5, #2560]
	ldr	r0, [r0, #276]
	ldr	r1, [r3, #32]
	bl	FSP_SetRef
	ldrb	r3, [r4]	@ zero_extendqisi2
	ldr	r0, [r5, #2560]
	mov	r2, #624
	adds	r3, r3, #0
	add	r1, r4, #56
	movne	r3, #1
	strb	r3, [r0, #1]
	ldr	r3, [r5, #2560]
	ldr	r0, [r4, #36]
	str	r0, [r3, #24]
	ldr	r0, [r5, #2560]
	add	r0, r0, #72
	bl	memcpy
	ldr	ip, [r5, #2560]
	ldr	lr, [r4, #40]
	add	r1, r7, #1
	ldr	r3, .L3214
	mov	r2, #4
	add	r0, r6, #2560
	str	lr, [ip, #40]
	str	lr, [ip, #20]
	ldr	ip, [r5, #2560]
	ldr	lr, [r4, #52]
	str	lr, [ip, #68]
	ldr	ip, [r5, #2560]
	ldr	lr, [r4, #12]
	str	lr, [ip, #44]
	str	lr, [ip, #16]
	ldr	ip, [r5, #2560]
	ldr	lr, [r4, #8]
	str	lr, [ip, #12]
	ldr	ip, [r5, #2560]
	ldr	lr, [r4, #8]
	str	lr, [ip, #36]
	bl	qsort
	ldr	r3, [r6, #2540]
	mov	r0, r6
	add	r3, r3, #1
	str	r3, [r6, #2540]
	ldr	r1, [r4, #52]
	bl	HEVC_GetAPC
	subs	r3, r0, #0
	bne	.L3213
	mov	r0, r3
	strb	r3, [r4, #1]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3212:
	mov	r3, r1
	ldr	r2, .L3214+4
	ldr	r1, .L3214+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	bl	HEVC_ClearDPB
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3213:
	ldr	r2, .L3214+4
	mov	r0, #0
	ldr	r1, .L3214+12
	bl	dprint_vfmw
	mov	r0, r6
	bl	HEVC_PrintDPBState
	mov	r0, r6
	bl	HEVC_ClearDPB
	mvn	r0, #6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3215:
	.align	2
.L3214:
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
	ldrb	r3, [r4, #368]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3229
	ldr	r3, [r4, #420]
	mov	r2, #2
	str	r2, [r3, #4]
.L3218:
	ldr	r3, [r4, #420]
	mov	r2, #1
	mov	r0, r5
	strb	r2, [r3, #2]
	ldr	r1, [r4, #420]
	bl	HEVC_CheckFrameStore
	cmn	r0, #1
	mov	r4, r0
	beq	.L3230
	cmn	r0, #3
	beq	.L3222
	mov	r0, r5
	bl	HEVC_OutputCurrPic
	cmp	r0, #0
	bne	.L3231
.L3222:
	mov	r0, #0
.L3219:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3229:
	bl	HEVC_SimpleSlideDPB
	add	r2, r5, #53504
	add	r2, r2, #112
	ldr	r1, [r5, #2540]
	mov	r0, r5
	bl	HEVC_InsertFrmInDPB
	cmp	r0, #0
	beq	.L3218
	str	r0, [sp]
	movw	r3, #8674
	ldr	r2, .L3232
	mov	r0, #0
	ldr	r1, .L3232+4
	bl	dprint_vfmw
	mvn	r0, #6
	b	.L3219
.L3231:
	ldr	r2, .L3232
	mov	r0, #0
	ldr	r1, .L3232+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3219
.L3230:
	ldr	r2, .L3232
	mov	r0, #0
	ldr	r1, .L3232+12
	bl	dprint_vfmw
	mov	r0, r4
	b	.L3219
.L3233:
	.align	2
.L3232:
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
	ldr	r1, [r0, #2540]
	mov	r4, r0
	ldr	r3, [r0, #2536]
	cmp	r1, r3
	bcs	.L3246
.L3235:
	add	r2, r4, #53504
	mov	r0, r4
	add	r2, r2, #112
	bl	HEVC_InsertFrmInDPB
	subs	r6, r0, #0
	bne	.L3247
	add	r3, r4, #53248
	mov	r0, r4
	ldr	r1, [r3, #420]
	bl	HEVC_CheckFrameStore
	cmn	r0, #1
	mov	r5, r0
	beq	.L3248
	cmn	r0, #3
	beq	.L3241
	mov	r0, r4
	bl	HEVC_OutputCurrPic
	cmp	r0, #0
	bne	.L3249
.L3241:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3246:
	bl	HEVC_RemoveUnusedApcStore
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #1
	beq	.L3245
	ldr	r2, .L3250
	mov	r0, #0
	ldr	r1, .L3250+4
	bl	dprint_vfmw
.L3245:
	ldr	r1, [r4, #2540]
	b	.L3235
.L3247:
	ldr	r2, .L3250
	mov	r0, #0
	ldr	r1, .L3250+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3249:
	mov	r0, r6
	ldr	r2, .L3250
	ldr	r1, .L3250+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3248:
	mov	r0, r6
	ldr	r2, .L3250
	ldr	r1, .L3250+16
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3251:
	.align	2
.L3250:
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
	ldr	r3, [r3, #672]
	cmp	r3, #1
	beq	.L3253
	bl	HEVC_FirstFrameFastOut
	cmp	r0, #0
	bne	.L3270
.L3253:
	ldr	r1, [r4, #2540]
	ldr	r3, [r4, #2536]
	cmp	r1, r3
	bcs	.L3271
.L3255:
	add	r2, r4, #53504
	mov	r0, r4
	add	r2, r2, #112
	bl	HEVC_InsertFrmInDPB
	cmp	r0, #0
	bne	.L3272
	ldr	r3, [r4, #2540]
	cmp	r3, #0
	bne	.L3258
.L3259:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3258:
	mov	r0, r4
	bl	HEVC_OutputFrmFromDPB
	cmp	r0, #0
	beq	.L3259
	ldr	r2, .L3273
	mov	r0, #1
	ldr	r1, .L3273+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3271:
	mov	r0, r4
	bl	HEVC_RemoveUnusedApcStore
	mov	r0, r4
	bl	HEVC_RemoveUnUsedFrameStore
	cmp	r0, #1
	beq	.L3269
	ldr	r1, .L3273+8
	mov	r0, #0
	bl	dprint_vfmw
.L3269:
	ldr	r1, [r4, #2540]
	b	.L3255
.L3272:
	mov	r2, r0
	ldr	r1, .L3273+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3270:
	ldr	r2, .L3273
	mov	r0, #0
	ldr	r1, .L3273+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3274:
	.align	2
.L3273:
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
	ldr	r3, [r0, #2356]
	mov	r4, r0
	ldr	r3, [r3, #596]
	cmp	r3, #1
	beq	.L3308
	add	r5, r0, #53248
	mov	r3, #0
	str	r3, [r5, #504]
	str	r3, [r5, #508]
.L3278:
	mov	r0, r4
	bl	HEVC_RemoveDummyFrame
	ldrb	r6, [r5, #369]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L3309
	ldr	r3, [r4, #2356]
	ldr	r2, [r5, #408]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bhi	.L3310
	ldr	r3, [r4, #2360]
	ldr	r2, [r5, #420]
	add	r3, r3, #4096
	ldr	r3, [r3, #1188]
	str	r3, [r2, #696]
	ldr	r3, [r4, #2360]
	ldr	r2, [r5, #420]
	add	r3, r3, #4096
	ldr	r3, [r3, #1192]
	str	r3, [r2, #700]
	ldr	r3, [r4, #280]
	cmp	r3, #2
	beq	.L3311
	ldr	r3, [r4, #2356]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	beq	.L3312
	cmp	r3, #1
	beq	.L3313
	cmp	r3, #0
	beq	.L3292
.L3293:
	mov	r0, r4
	mvn	r6, #0
	bl	HEVC_ClearCurrPic
	ldr	r2, .L3316
	ldr	r1, .L3316+4
	mov	r0, #1
	bl	dprint_vfmw
.L3291:
	mov	r3, #0
	mov	r0, r6
	strb	r3, [r5, #369]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3308:
	ldr	r2, .L3316+8
	add	r5, r0, #53248
	ldr	r2, [r2]
	add	r2, r2, #4096
	ldr	r0, [r2, #84]
	ldr	r1, [r2, #88]
	str	r3, [r5, #504]
	cmp	r1, #0
	moveq	r1, #1
	bl	__aeabi_uidiv
	str	r0, [r5, #508]
	b	.L3278
.L3309:
	ldr	r2, .L3316
	mov	r0, #1
	ldr	r1, .L3316+12
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3312:
	mov	r0, r4
	bl	HEVC_SimpleDPBProcess
	subs	r6, r0, #0
	bne	.L3314
.L3287:
	movw	r3, #9003
	mov	r2, r6
	ldr	r1, .L3316+16
	mov	r0, #14
	bl	dprint_vfmw
.L3285:
	cmn	r6, #1
	bne	.L3291
	b	.L3293
.L3310:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	ldr	r3, [r4, #2356]
	ldr	r2, [r5, #408]
	mov	r0, #1
	ldr	r1, .L3316+20
	ldr	r3, [r3, #4]
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3313:
	mov	r0, r4
	bl	HEVC_DecOrderProcess
	subs	r6, r0, #0
	beq	.L3289
	ldr	r2, .L3316
	mov	r0, #0
	ldr	r1, .L3316+24
	bl	dprint_vfmw
	ldr	r3, [r4, #2356]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L3285
.L3292:
	mov	r0, r4
	bl	HEVC_DispOrderProcess
	subs	r6, r0, #0
	bne	.L3290
.L3294:
	mov	r6, #0
	b	.L3291
.L3311:
	mov	r0, r4
	bl	HEVC_DirectOutput
	ldr	r3, [r5, #404]
	ldr	r2, [r4, #180]
	cmp	r3, r2
	addne	r3, r3, #536
	movne	r1, #0
	addne	r3, r4, r3, lsl #2
	strne	r1, [r3, #4]
	mov	r6, r0
	strne	r2, [r5, #404]
	cmp	r0, #0
	bne	.L3315
.L3284:
	movw	r3, #8988
	mov	r2, r6
	ldr	r1, .L3316+16
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3285
.L3289:
	ldr	r3, [r4, #2356]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L3294
	b	.L3292
.L3290:
	ldr	r2, .L3316
	mov	r0, #0
	ldr	r1, .L3316+28
	bl	dprint_vfmw
	b	.L3285
.L3315:
	ldr	r2, .L3316
	mov	r0, #0
	ldr	r1, .L3316+32
	bl	dprint_vfmw
	b	.L3284
.L3314:
	ldr	r2, .L3316
	mov	r0, #0
	ldr	r1, .L3316+36
	bl	dprint_vfmw
	b	.L3287
.L3317:
	.align	2
.L3316:
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
	add	r4, r4, #640
	mov	r5, r0
	mov	r6, r1
	mov	r7, #0
.L3323:
	ldr	r2, [r4, #4]
	cmp	r2, #0
	bne	.L3319
	cmp	r4, #0
	beq	.L3320
	ldr	r3, [r5, #2540]
	cmp	r3, #0
	beq	.L3320
	ldr	r1, [r5, #2560]
	cmp	r4, r1
	beq	.L3319
	add	ip, r5, #2560
	b	.L3321
.L3322:
	ldr	lr, [ip, #4]!
	cmp	lr, r4
	beq	.L3319
.L3321:
	add	r2, r2, #1
	cmp	r2, r3
	bne	.L3322
.L3320:
	mov	r8, #704
	mov	r2, #624
	mla	r8, r8, r7, r5
	sub	r0, fp, #660
	mov	r9, #1
	mov	r3, #101
	str	r6, [fp, #-708]
	add	r8, r8, #67584
	str	r3, [fp, #-676]
	add	r1, r8, #712
	strb	r9, [fp, #-715]
	strb	r9, [fp, #-716]
	bl	memcpy
	cmp	r7, #19
	strb	r9, [r8, #642]
	mov	r7, #0
	strb	r9, [r8, #640]
	str	r4, [fp, #-664]
	str	r7, [r8, #648]
	str	r7, [fp, #-500]
	bgt	.L3324
	ldr	r0, [r5, #2540]
	cmp	r0, #0
	beq	.L3326
	add	r1, r5, #2544
	mov	ip, r7
	add	r1, r1, #12
	mvn	r2, #-2147483648
	mov	r8, r1
.L3329:
	ldr	lr, [r8, #4]!
	add	ip, ip, #1
	ldr	r3, [lr, #12]
	rsb	r3, r6, r3
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, r2
	bge	.L3328
	ldrb	lr, [lr, #1]	@ zero_extendqisi2
	cmp	lr, #0
	movne	r2, r3
	movne	r7, #1
.L3328:
	cmp	ip, r0
	bne	.L3329
	cmp	r7, #0
	beq	.L3326
	mov	r4, #0
	b	.L3330
.L3331:
	add	r4, r4, #1
	cmp	r4, r0
	beq	.L3332
.L3330:
	ldr	ip, [r1, #4]!
	ldr	r3, [ip, #12]
	rsb	r3, r6, r3
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, r2
	bne	.L3331
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3331
	ldr	r1, [ip, #32]
	add	r4, r4, #640
	ldr	r0, [r5, #276]
	bl	FSP_GetLogicFs
	ldr	r2, [r5, r4, asl #2]
	mov	r3, r6
	ldr	r1, .L3363
	ldr	r2, [r2, #12]
	mov	r7, r0
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r7, #0
	beq	.L3332
	ldr	r0, [r7, #640]
	cmp	r0, #0
	beq	.L3361
	mov	r1, #0
	ldr	r0, [r5, #276]
	bl	FSP_NewLogicFs
	mov	r8, r0
	mov	r1, r0
	ldr	r0, [r5, #276]
	bl	FSP_GetLogicFs
	mvn	r2, r8
	mov	r2, r2, lsr #31
	cmp	r0, #0
	mov	r3, r0
	moveq	r2, #0
	cmp	r2, #0
	beq	.L3335
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
	ldr	r1, [r5, #2540]
	str	r3, [fp, #-680]
	bl	HEVC_InsertFrmInDPB
	subs	r4, r0, #0
	bne	.L3362
	ldr	r0, [r5, #276]
	mov	r1, r8
	mov	r2, r4
	bl	FSP_SetDisplay
	ldr	r3, [r5, #200]
	mov	r0, r4
	cmn	r3, #-2147483647
	streq	r6, [r5, #200]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3319:
	add	r7, r7, #1
	add	r4, r4, #704
	cmp	r7, #20
	bne	.L3323
.L3324:
	ldr	r1, .L3363+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
.L3356:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3332:
	ldr	r1, .L3363+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #3
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3335:
	ldr	r3, [fp, #-664]
	mov	r0, r2
	ldr	r1, .L3363+12
	strb	r2, [r3]
	bl	dprint_vfmw
	mvn	r0, #3
	b	.L3356
.L3362:
	ldr	r3, [fp, #-664]
	mov	r1, r8
	mov	r4, #0
	mov	r2, #1
	strb	r4, [r3]
	ldr	r0, [r5, #276]
	bl	FSP_ClearLogicFs
	mov	r0, r4
	ldr	r1, .L3363+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3356
.L3326:
	mov	r0, #0
	ldr	r1, .L3363+20
	strb	r0, [r4]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3356
.L3361:
	ldr	r1, .L3363+24
	bl	dprint_vfmw
	mov	r0, r5
	bl	HEVC_ClearDPB
	mvn	r0, #3
	b	.L3356
.L3364:
	.align	2
.L3363:
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
	b	.L3366
.L3369:
	ldr	r2, [fp, #-24]
	bl	dprint_vfmw
	ldr	r3, [r4, #2356]
	mov	r0, r4
	ldr	r2, [r3, #4]
	cmp	r2, #0
	beq	.L3367
	ldr	r1, [fp, #-24]
	bl	HEVC_CreateLostPicture
	cmp	r0, #0
	bne	.L3372
.L3366:
	sub	r1, fp, #24
	mov	r0, r4
	bl	HEVC_CheckThatAllRefPicsAreAvailable
	ldr	r1, .L3373
	cmp	r0, #0
	mov	r0, #1
	beq	.L3369
	mov	r0, r4
	bl	HEVC_UpdateReflist
	mov	r0, r4
	bl	HEVC_UpdateLTReflist
	mov	r0, r4
	bl	HEVC_RecoverDPBProcess
	mov	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3367:
	ldr	ip, [fp, #-24]
	add	r4, r4, #53248
	ldr	r1, .L3373+4
	mov	r0, #1
	ldr	r3, [r4, #1096]
	str	ip, [sp]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3372:
	mov	r0, r4
	bl	HEVC_RemoveDummyFrame
	mvn	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L3374:
	.align	2
.L3373:
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
	ldr	r3, [r0, #2364]
	add	r5, r0, #53248
	mov	r1, #0
	mov	r4, r0
	ldr	r2, [r3, #16]
	str	r2, [r5, #1120]
	ldr	r3, [r3, #32]
	str	r3, [r5, #1116]
	strb	r1, [r0, #1]
	ldr	r3, [r5, #2636]
	cmp	r3, #1
	beq	.L3425
.L3376:
	ldr	r2, [r4, #208]
	ldr	r3, [r4, #144]
	cmp	r2, r3
	bcs	.L3426
	ldrb	r3, [r5, #369]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3427
	mov	r0, r4
	bl	HEVC_PicTypeStatistic
	mov	r0, r4
	bl	HEVC_CalcStreamBits
	ldr	r3, [r5, #2636]
	cmp	r3, #2
	beq	.L3389
	ldr	r3, [r4, #220]
	cmp	r3, #0
	bne	.L3390
	ldrb	r3, [r5, #1051]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3428
.L3390:
	mov	r0, r4
	bl	HEVC_WriteSliceMsg
	cmp	r0, #0
	bne	.L3429
.L3393:
	ldr	r3, [r4, #220]
	cmp	r3, #0
	beq	.L3430
.L3395:
	add	r4, r4, #4096
	mov	r0, #0
	ldr	r3, [r4, #476]
	str	r3, [r5, #412]
	ldr	r3, [r4, #480]
	str	r3, [r5, #416]
.L3381:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3430:
	ldr	r3, [r5, #1124]
	mov	r2, #1
	str	r3, [r4, #320]
	ldr	r3, [r5, #1096]
	str	r2, [r4, #168]
	str	r3, [r4, #284]
	b	.L3395
.L3425:
	ldr	r3, [r0, #220]
	cmp	r3, r1
	bne	.L3378
	sub	r2, r2, #16
	cmp	r2, #4
	bls	.L3379
	bl	HEVC_IsPVRFlushPic
	cmp	r0, #0
	beq	.L3380
.L3379:
	mov	r0, r4
	bl	HEVC_FlushOutputFrmFromDPB
	cmp	r0, #0
	bne	.L3431
.L3380:
	ldrb	r6, [r4, #2]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L3432
.L3382:
	ldr	r3, [r4, #2356]
	ldr	r2, [r3, #8]
	cmp	r2, #2
	beq	.L3378
	ldr	r3, [r3, #12]
	mov	r0, r4
	cmp	r3, #2
	beq	.L3433
	bl	HEVC_RefPicProcess
	cmp	r0, #0
	bne	.L3396
.L3378:
	mov	r0, r4
	bl	HEVC_InitPic
	cmn	r0, #2
	beq	.L3381
	cmp	r0, #0
	bne	.L3434
	ldr	r3, [r4, #252]
	mov	r0, r4
	add	r3, r3, #1
	str	r3, [r4, #252]
	bl	HEVC_SetScalingList
	mov	r0, r4
	bl	HEVC_WritePicMsg
	cmp	r0, #0
	beq	.L3376
	ldr	r1, .L3436
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3389:
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	beq	.L3393
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
	b	.L3393
.L3428:
	mov	r0, r4
	bl	HEVC_DecList
	cmp	r0, #0
	bne	.L3435
	mov	r0, r4
	bl	HEVC_IsRefListWrong
	cmp	r0, #0
	beq	.L3390
	mov	r0, r4
	bl	HEVC_PrintDPBState
	ldr	r1, .L3436+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3432:
	ldr	r3, [r4, #2540]
	cmp	r3, #0
	beq	.L3382
	mov	r0, r4
	bl	HEVC_EmptyFrmFromDPB
	cmp	r0, #0
	beq	.L3382
	mov	r0, r6
	ldr	r2, .L3436+8
	ldr	r1, .L3436+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3396:
	mvn	r0, #0
	b	.L3381
.L3433:
	bl	HEVC_UpdateReflist
	mov	r0, r4
	bl	HEVC_UpdateLTReflist
	b	.L3378
.L3427:
	ldr	r1, .L3436+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3426:
	ldr	r1, .L3436+20
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #208]
	ldr	r2, [r4, #144]
	cmp	r3, r2
	bcc	.L3387
	ldr	r1, .L3436+24
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L3387
	str	r3, [fp, #-36]
	mov	r1, #108
	str	r2, [fp, #-32]
	mov	r3, #8
	sub	r2, fp, #36
	ldr	r0, [r4, #276]
	blx	ip
.L3387:
	ldr	r1, .L3436+28
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mvn	r0, #0
	b	.L3381
.L3429:
	ldr	r1, .L3436+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3434:
	ldr	r1, .L3436+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3435:
	mov	r2, r0
	ldr	r1, .L3436+40
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3431:
	ldr	r2, .L3436+8
	mov	r0, #1
	ldr	r1, .L3436+44
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3381
.L3437:
	.align	2
.L3436:
	.word	.LC440
	.word	.LC445
	.word	.LANCHOR1+916
	.word	.LC438
	.word	.LC443
	.word	.LC441
	.word	g_event_report
	.word	.LC442
	.word	.LC446
	.word	.LC439
	.word	.LC444
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
	add	ip, r0, #53248
	mov	r4, r0
	mov	r5, r3
	ldr	r2, [fp, #8]
	str	r1, [ip, #408]
	ldr	r0, [r0, #240]
	ldr	r3, [fp, #4]
	cmp	r0, #1
	bls	.L3439
.L3442:
	ldr	r1, [ip, #384]
	cmp	r1, #1920
	bhi	.L3457
.L3440:
	ldr	r0, [r4, #2356]
.L3443:
	ldr	r1, [r0, #624]
	cmp	r3, #0
	str	r1, [r4, #212]
	beq	.L3446
.L3460:
	ldr	r3, .L3461
	mov	r0, r2, asl #6
	sub	r2, r0, r2, asl #3
	add	r2, r2, r3
	ldr	r3, [r2, #4]
	cmp	r3, #3
	beq	.L3458
	cmp	r3, #1
	beq	.L3446
.L3449:
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_StorePicInDPB
	subs	r2, r0, #0
	bne	.L3459
	mov	r3, #1
	mov	r0, r2
	str	r3, [r4, #168]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3457:
	ldr	r0, [r4, #2356]
	ldr	r1, [r0, #8]
	cmp	r1, #2
	movne	r1, #0
	strne	r1, [r4, #212]
	beq	.L3443
	cmp	r3, #0
	bne	.L3460
.L3446:
	mov	r0, r4
	bl	HEVC_ClearAllSlice
	b	.L3449
.L3459:
	ldr	r1, .L3461+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mov	r0, r4
	bl	HEVC_ClearDPB
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3439:
	add	r0, r0, #1
	str	r0, [r4, #240]
	cmp	r0, #1
	bls	.L3440
	b	.L3442
.L3458:
	mov	r0, r4
	bl	HEVC_ClearAllSlice
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3462:
	.align	2
.L3461:
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
	bhi	.L3464
	bl	HEVC_ClearCurrPic
	ldr	r3, .L3532
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3465
	mov	r3, #0
	mov	r1, #127
	mov	r2, r3
	ldr	r0, [r4, #276]
	blx	ip
.L3465:
	ldr	r3, [r4, #256]
	cmp	r3, #524288
	bhi	.L3526
	ldr	r1, .L3532+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
.L3521:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3464:
	ldr	r3, [r0, #312]
	add	r6, r0, #53248
	ldr	r2, [r0, #316]
	add	r3, r3, #1
	ldr	r1, [r0, #2356]
	cmp	r3, r2
	movlt	r3, r2
	str	r3, [r0, #312]
	rsb	r3, r2, r3
	str	r3, [r1, #576]
	bl	HEVC_IsRefForCurrPic
	strb	r0, [r6, #368]
	ldr	r5, [r4, #264]
	ldr	r2, [r4, #280]
	ubfx	r5, r5, #16, #3
	cmp	r5, r2
	beq	.L3468
	sub	r1, r2, #2
	cmp	r1, #1
	movhi	r1, #0
	movls	r1, #1
	cmp	r5, #2
	moveq	r1, #0
	cmp	r1, #0
	beq	.L3468
	cmp	r5, #4
	cmpne	r5, #1
	bls	.L3527
.L3468:
	cmp	r5, #2
	str	r5, [r4, #280]
	beq	.L3470
	cmp	r5, #3
	beq	.L3471
	cmp	r5, #1
	beq	.L3528
	ldr	r3, [r4, #248]
	cmp	r3, #0
	beq	.L3491
	cmp	r5, #4
	beq	.L3529
.L3477:
	ldr	r2, [r6, #380]
.L3478:
	cmp	r2, #2
	beq	.L3530
	cmp	r2, #1
	streqb	r2, [r4, #4]
.L3483:
	ldr	r2, [r4, #252]
	mov	r0, #2
	ldr	r1, .L3532+8
	sub	r2, r2, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #208]
	cmp	r3, #0
	beq	.L3531
	ldr	r0, [r6, #396]
	add	r2, r4, #61440
	clz	r1, r0
	mov	r1, r1, lsr #5
	str	r1, [r2, #2008]
	ldr	r1, [r6, #400]
	str	r3, [r2, #3240]
	str	r1, [r2, #2000]
	ldr	r3, [r4, #2356]
	ldr	r1, [r3, #592]
	str	r1, [r2, #1992]
	ldr	r3, [r3, #632]
	str	r3, [r2, #1996]
	ldr	r3, [r4, #252]
	cmp	r3, #0
	beq	.L3523
	add	r1, r4, #61952
	add	r3, r4, #65536
	add	r1, r1, #164
	str	r1, [r4, #2360]
	ldr	ip, [r6, #412]
	mov	r1, #0
	add	r0, r0, #1
	str	ip, [r3, #1864]
	ldr	ip, [r6, #416]
	str	ip, [r3, #1868]
	str	r0, [r6, #396]
	str	r1, [r3, #1860]
	ldr	r1, [r6, #380]
	cmp	r1, #0
	bne	.L3523
	ldr	r2, [r2, #3244]
	cmp	r2, #0
	beq	.L3492
.L3490:
	ldr	r3, [r2]
	ldr	r0, [r2, #4]
	ldr	r2, [r2, #1356]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3490
.L3489:
	ldr	r3, .L3532
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3523
	sub	r2, fp, #28
	ldr	r0, [r4, #276]
	mov	r3, #4
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
.L3523:
	mov	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3526:
	ldr	r1, .L3532+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #7
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3528:
	ldr	r3, [r6, #380]
	cmp	r0, #0
	cmpeq	r3, #2
	beq	.L3474
.L3473:
	ldr	r3, [r4, #248]
	cmp	r3, #0
	bne	.L3477
.L3491:
	ldr	r3, .L3532
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3474
	mov	r3, #0
	mov	r1, #127
	mov	r2, r3
	ldr	r0, [r4, #276]
	blx	ip
	b	.L3474
.L3527:
	mov	r3, r5
	ldr	r1, .L3532+16
	mov	r0, #1
	bl	dprint_vfmw
	str	r5, [r4, #280]
.L3471:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mov	r0, r4
	bl	HEVC_ClearDPB
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3470:
	ldr	r3, [r6, #380]
	cmp	r3, #0
	beq	.L3473
	b	.L3474
.L3530:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3483
	ldr	r3, [r4, #2356]
	ldr	r5, [r3, #12]
	cmp	r5, #0
	bne	.L3483
	ldr	r3, .L3532+20
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3483
	mov	r3, r0
	ldr	r2, [r6, #376]
	ldr	r1, .L3532+24
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3532
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3474
	mov	r3, r5
	mov	r2, r5
	mov	r1, #127
	ldr	r0, [r4, #276]
	blx	ip
.L3474:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mvn	r0, #0
	b	.L3521
.L3529:
	ldr	r2, [r6, #380]
	cmp	r2, #0
	moveq	r3, #2
	streqb	r3, [r4, #4]
	beq	.L3483
	b	.L3478
.L3531:
	ldr	r2, .L3532
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3486
	mov	r2, r3
	mov	r1, #127
	ldr	r0, [r4, #276]
	blx	ip
.L3486:
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	ldr	r2, .L3532+28
	ldr	r1, .L3532+32
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3521
.L3492:
	mov	r1, r2
	b	.L3489
.L3533:
	.align	2
.L3532:
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
	.global	HEVCDEC_CheckSedOnly
	.type	HEVCDEC_CheckSedOnly, %function
HEVCDEC_CheckSedOnly:
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
	ldr	r3, [r0, #216]
	mov	r5, #0
	mov	r4, r0
	str	r5, [r0, #220]
	cmp	r3, #2
	str	r5, [r0, #228]
	beq	.L3562
	cmp	r3, #1
	beq	.L3563
.L3547:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3563:
	ldr	r3, [r0, #224]
	cmp	r3, #1
	bne	.L3547
	str	r5, [r0, #224]
.L3561:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3562:
	ldr	r3, [r0, #212]
	cmp	r3, r5
	beq	.L3561
	ldr	r3, [r0, #224]
	cmp	r3, #1
	beq	.L3547
	ldr	r0, [r0, #276]
	bl	VCTRL_CheckFreshSeg
	cmp	r0, #0
	beq	.L3561
	ldr	r0, [r4, #276]
	bl	VCTRL_CheckFreshRaw
	cmp	r0, #0
	beq	.L3561
	ldr	r3, [r4, #2364]
	ldr	r0, [r4, #2340]
	ldr	r1, [r3, #44]
	ldr	r2, [r3, #48]
	bl	BsInit
	mov	r1, #24
	ldr	r0, [r4, #2340]
	bl	BsSkip
	ldr	r0, [r4, #2340]
	bl	BsResidBits
	cmp	r0, #15
	ble	.L3561
	mov	r1, #1
	ldr	r0, [r4, #2340]
	bl	BsSkip
	mov	r1, #6
	ldr	r0, [r4, #2340]
	bl	BsGet
	mov	r1, #9
	mov	r6, r0
	ldr	r0, [r4, #2340]
	bl	BsSkip
	mov	r0, r6
	bl	HEVC_IsSliceUnit
	cmp	r0, #1
	mov	r7, r0
	beq	.L3564
	ldr	r3, [r4, #208]
	cmp	r3, #0
	beq	.L3561
	sub	r3, r6, #48
	sub	r6, r6, #32
	cmp	r6, #2
	cmphi	r3, #1
	bhi	.L3561
.L3543:
	mov	r0, r4
	bl	HEVC_DecVDM
	cmp	r0, #0
	bne	.L3561
	add	r2, r4, #53248
	mov	r3, #1
	mvn	r0, #0
	str	r3, [r4, #228]
	strb	r3, [r2, #368]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3564:
	ldr	r0, [r4, #2340]
	bl	BsResidBits
	cmp	r0, #0
	movle	r0, r5
	ble	.L3540
	mov	r1, r7
	ldr	r0, [r4, #2340]
	bl	BsGet
.L3540:
	ldr	r3, [r4, #208]
	cmp	r3, #0
	beq	.L3541
	sub	r3, r6, #48
	sub	r6, r6, #32
	cmp	r6, #2
	cmphi	r3, #1
	movls	r3, #1
	movhi	r3, #0
	cmp	r0, #0
	moveq	r5, r3
	orrne	r5, r3, #1
	cmp	r5, #0
	bne	.L3543
.L3541:
	add	r3, r4, #53248
	mov	r5, #0
	mov	r2, #1
	str	r5, [r4, #224]
	str	r2, [r4, #220]
	mov	r0, r5
	ldrb	r3, [r3, #369]	@ zero_extendqisi2
	str	r3, [r4, #232]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVCDEC_CheckSedOnly, .-HEVCDEC_CheckSedOnly
	.align	2
	.global	HEVCDEC_SetDecParam
	.type	HEVCDEC_SetDecParam, %function
HEVCDEC_SetDecParam:
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
	ldr	r5, [r0, #224]
	mov	r4, r0
	cmp	r5, #1
	beq	.L3574
.L3566:
	mov	r0, r4
	bl	HEVCDEC_IsNeedSedOnly
	cmp	r0, #1
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r1, r4, #65536
	add	r2, r4, #61440
	add	r3, r4, #61952
	mov	ip, #0
	str	r0, [r1, #1860]
	add	r3, r3, #164
	ldr	r1, [r4, #208]
	str	r1, [r4, #236]
	str	r1, [r2, #3240]
	ldr	r1, [r4, #2356]
	ldr	r1, [r1, #632]
	str	r1, [r2, #1996]
	str	r3, [r4, #2360]
	str	ip, [r4, #224]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3574:
	ldr	r3, [r0, #208]
	cmp	r3, #0
	beq	.L3566
	ldr	r6, [r0, #2360]
	cmp	r6, #0
	bne	.L3566
	bl	HEVC_DecVDM
	cmp	r0, #0
	beq	.L3575
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	str	r6, [r4, #2364]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3575:
	add	r3, r4, #53248
	str	r5, [r4, #228]
	strb	r5, [r3, #368]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	HEVCDEC_SetDecParam, .-HEVCDEC_SetDecParam
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
	ldr	r0, [r0, #2364]
	ldr	r3, [r0, #8]
	cmp	r3, #0
	ldrne	r1, [r4, #256]
	movne	r2, r0
	movne	r3, #0
	beq	.L3581
.L3580:
	ldr	ip, [r2, #48]
	add	r3, r3, #1
	add	r2, r2, #28
	add	r1, r1, ip
	str	r1, [r4, #256]
	ldr	ip, [r0, #8]
	cmp	ip, r3
	bhi	.L3580
.L3581:
	mov	r3, #0
	strb	r3, [r0, #1]
	ldr	r3, [r4, #2364]
	ldr	r2, [r3, #16]
	sub	r2, r2, #36
	cmp	r2, #1
	bls	.L3715
.L3579:
	ldr	r2, [r3, #4]
	add	r2, r2, #16
	str	r2, [r3, #4]
	ldr	r1, [r4, #2364]
	ldr	r2, [r1, #16]
	cmp	r2, #49
	ldrls	pc, [pc, r2, asl #2]
	b	.L3582
.L3584:
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3583
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3585
	.word	.L3586
	.word	.L3587
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3588
	.word	.L3588
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3582
	.word	.L3589
	.word	.L3590
.L3715:
	mov	r2, #1
	strb	r2, [r4, #1]
	b	.L3579
.L3590:
	mov	r0, r4
	bl	HEVC_IsEosNal
	cmp	r0, #1
	beq	.L3716
.L3606:
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	beq	.L3703
.L3705:
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #2364]
.L3702:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3583:
	mov	r0, r4
	bl	HEVC_DecSlice
	cmn	r0, #2
	beq	.L3702
	cmp	r0, #0
	beq	.L3703
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	beq	.L3713
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3713:
	ldr	r3, .L3718
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3704
.L3706:
	mov	r3, #0
	ldr	r0, [r4, #276]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3704:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3585:
	mov	r0, r4
	bl	HEVC_DecVPS
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3591
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3591:
	cmp	r5, #0
	bne	.L3717
.L3703:
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3586:
	mov	r0, r4
	bl	HEVC_DecSPS
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3596
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3596:
	cmp	r5, #0
	beq	.L3703
	ldr	r6, .L3718
	mov	r0, #1
	ldr	r1, .L3718+4
	bl	dprint_vfmw
	ldr	ip, [r6]
	cmp	ip, #0
	bne	.L3708
	b	.L3704
.L3587:
	mov	r0, r4
	bl	HEVC_DecPPS
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3598
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3598:
	cmp	r5, #0
	beq	.L3703
	ldr	r6, .L3718
	mov	r0, #1
	ldr	r1, .L3718+8
	bl	dprint_vfmw
	ldr	ip, [r6]
	cmp	ip, #0
	beq	.L3704
.L3708:
	sub	r2, fp, #28
	mvn	r5, #0
	mov	r3, #4
	mov	r1, #100
	str	r5, [r2, #-4]!
	ldr	r0, [r4, #276]
	blx	ip
	ldr	ip, [r6]
	cmp	ip, #0
	bne	.L3706
	b	.L3704
.L3588:
	mov	r0, r4
	bl	HEVC_DecSEI
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3599
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3599:
	cmp	r5, #0
	beq	.L3703
	ldr	r1, .L3718+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3713
.L3589:
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3604
	cmp	r1, #0
	beq	.L3603
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3603:
	add	r3, r4, #53248
	ldrb	r3, [r3, #369]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3703
.L3604:
	add	r5, r4, #2496
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
	bne	.L3703
	ldr	r5, .L3718
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3605
	mov	r3, r6
	mov	r2, r6
	mov	r1, #112
	ldr	r0, [r4, #276]
	blx	ip
.L3605:
	mov	r0, r4
	bl	HEVC_DecVDM
	cmp	r0, #0
	beq	.L3703
.L3711:
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L3706
	b	.L3704
.L3582:
	ldr	r1, .L3718+16
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r1, [r4, #2364]
	ldr	r3, [r1, #16]
	cmp	r3, #63
	bhi	.L3609
	cmp	r1, #0
	bne	.L3705
	b	.L3703
.L3609:
	ldr	r5, .L3718
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3610
	mov	r3, #0
	mov	r1, #102
	mov	r2, r3
	ldr	r0, [r4, #276]
	blx	ip
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	beq	.L3611
.L3612:
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3611:
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3703
	mov	r3, #0
	ldr	r0, [r4, #276]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L3703
.L3610:
	cmp	r1, #0
	bne	.L3612
	b	.L3703
.L3717:
	ldr	r6, .L3718
	mov	r0, #1
	ldr	r1, .L3718+20
	bl	dprint_vfmw
	ldr	ip, [r6]
	cmp	ip, #0
	bne	.L3708
	b	.L3704
.L3716:
	ldr	r5, .L3718
	strb	r0, [r4, #7]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3607
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #276]
	blx	ip
.L3607:
	ldr	r3, [r4, #212]
	cmp	r3, #1
	bne	.L3608
	add	r3, r4, #53248
	ldrb	r3, [r3, #369]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3703
.L3608:
	mov	r0, r4
	bl	HEVC_DecVDM
	cmp	r0, #0
	beq	.L3606
	b	.L3711
.L3719:
	.align	2
.L3718:
	.word	g_event_report
	.word	.LC455
	.word	.LC456
	.word	.LC457
	.word	.LC458
	.word	.LC454
	UNWIND(.fnend)
	.size	HEVC_DecOneNal, .-HEVC_DecOneNal
	.align	2
	.global	HEVCDEC_VDMProcSedOnly
	.type	HEVCDEC_VDMProcSedOnly, %function
HEVCDEC_VDMProcSedOnly:
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
	ldr	r6, .L3732
	mov	r3, #180
	ldr	r2, [r0, #2360]
	mov	r4, r0
	ldr	lr, [r0, #276]
	mov	r0, #22
	mla	r1, r3, r1, r6
	ldr	r3, [r2, #20]
	ldr	r2, [r2, #16]
	ldr	ip, .L3732+4
	add	r6, r1, #16
	ldr	r1, .L3732+8
	mla	r5, r2, r3, r3
	ldr	r3, [r6, #4]
	ldr	r7, [ip, lr, asl #2]
	add	r5, r5, r2
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, [r6, #4]
	cmp	r3, r5
	ldr	r3, [r4, #220]
	beq	.L3730
	cmp	r3, #1
	beq	.L3731
.L3725:
	mov	r3, #0
	str	r3, [r4, #2360]
.L3723:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3730:
	cmp	r3, #1
	beq	.L3722
	mov	r0, r4
	bl	HEVC_DecVDM
	cmp	r0, #0
	addeq	r2, r4, #53248
	moveq	r3, #1
	streq	r3, [r4, #228]
	streqb	r3, [r2, #368]
	b	.L3723
.L3731:
	ldr	ip, [r4, #232]
	add	r3, r4, #53248
	ldr	r0, [r4, #208]
	mov	r5, #0
	ldr	r1, [r4, #172]
	ldr	r2, [r4, #248]
	sub	r0, r0, #1
	sub	r1, r1, #1
	str	r0, [r4, #208]
	str	r1, [r4, #172]
	sub	r2, r2, #1
	str	r5, [r4, #224]
	str	r2, [r4, #248]
	strb	ip, [r3, #369]
	ldr	r1, [r4, #2364]
	cmp	r1, r5
	beq	.L3725
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	str	r5, [r4, #2364]
	b	.L3725
.L3722:
	ldr	r5, [r4, #232]
	add	lr, r4, #53248
	ldr	r0, [r4, #208]
	mov	ip, #0
	ldr	r1, [r4, #172]
	ldr	r2, [r4, #248]
	sub	r0, r0, #1
	str	r3, [r4, #224]
	sub	r1, r1, #1
	str	r0, [r4, #208]
	sub	r2, r2, #1
	str	r1, [r4, #172]
	mov	r0, #0
	str	r2, [r4, #248]
	strb	r5, [lr, #369]
	strb	r3, [r7, #745]
	strb	r3, [r4, #5]
	str	ip, [r4, #2360]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3733:
	.align	2
.L3732:
	.word	g_BackUp
	.word	s_pstVfmwChan
	.word	.LC459
	UNWIND(.fnend)
	.size	HEVCDEC_VDMProcSedOnly, .-HEVCDEC_VDMProcSedOnly
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
	ldr	r0, [r0, #276]
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	ip, .L3756
	mov	r4, r0
	mov	r3, #0
	mov	r7, r1
	add	r0, r0, #54272
	str	r3, [r2]
	mov	r1, r3
	str	r3, [r7]
	mov	r6, r2
	ldr	r3, [ip, #48]
	movw	r2, #3732
	add	r0, r0, #24
	add	r5, r4, #53248
	blx	r3
	ldr	r3, [r4, #2364]
	mov	r0, r4
	ldr	r2, [r3, #16]
	str	r2, [r5, #1120]
	ldr	r3, [r3, #32]
	str	r3, [r5, #1116]
	bl	HEVC_DecSliceSegmentHeader
	cmp	r0, #0
	bne	.L3752
	ldr	r2, [r5, #2636]
	movw	ip, #2024
	movw	r0, #26164
	sub	r2, r2, #1
	clz	r2, r2
	mov	r2, r2, lsr #5
	str	r2, [r7]
	ldr	r2, [r5, #1124]
	ldr	r3, [r4, #2352]
	ldr	r7, [r4, #220]
	ldr	r1, [r4, #2348]
	mla	r3, ip, r2, r3
	cmp	r7, #1
	ldr	r3, [r3, #32]
	mla	r5, r0, r3, r1
	beq	.L3753
	add	ip, r4, #24576
	ldr	lr, [r4, #2356]
	ldr	r0, [r5, #236]
	ldr	r2, [ip, #2868]
	ldr	r7, [lr, #832]
	ldr	r3, [r5, #72]
	ldr	r1, [r2, #236]
	ldr	r8, [r2, #72]
	subs	r0, r0, r1
	ldr	r2, [r2, #76]
	movne	r0, #1
	cmp	r7, #1
	ldr	r7, [r5, #76]
	beq	.L3754
.L3741:
	cmp	r3, r8
	moveq	r1, r0
	orrne	r1, r0, #1
	cmp	r7, r2
	moveq	r3, r1
	orrne	r3, r1, #1
	cmp	r3, #0
	beq	.L3744
	mov	r0, #0
	mov	r3, #1
	str	r3, [r6]
.L3737:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3753:
	ldr	r0, [r4, #2340]
	bl	BsResidBits
	ldr	r3, [r5, #240]
	ldr	r1, [r4, #208]
	add	r1, r1, #1
	mov	r6, r0
	ldr	r0, [r5, #244]
	mul	r0, r0, r3
	bl	__aeabi_uidiv
	cmp	r6, r0
	movge	r3, #0
	movlt	r3, #1
	orrs	r3, r3, r6, lsr #31
	bne	.L3739
.L3744:
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3752:
	ldr	r1, .L3756+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3756+8
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r0, #0
	beq	.L3737
	sub	r2, fp, #36
	ldr	r0, [r4, #276]
	mov	r3, #4
	mvn	r4, #0
	mov	r1, #100
	str	r4, [r2, #-4]!
	blx	ip
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3754:
	ldr	r1, [r5, #108]
	ldr	r5, [r5, #100]
	cmp	r1, r5
	movcc	r1, r5
	str	r1, [lr, #884]
	ldr	ip, [ip, #2868]
	ldr	r4, [r4, #2356]
	ldr	r1, [ip, #108]
	ldr	ip, [ip, #100]
	ldr	lr, [r4, #884]
	cmp	r1, ip
	movcc	r1, ip
	cmp	r1, #8
	movls	ip, #0
	movhi	ip, #1
	cmp	lr, #8
	movne	ip, #0
	cmp	ip, #0
	beq	.L3755
.L3742:
	mov	r1, #1
	str	r1, [r4, #888]
	str	r1, [r6]
	b	.L3741
.L3755:
	sub	r1, r1, #8
	cmp	lr, #8
	clz	r1, r1
	mov	r1, r1, lsr #5
	movle	r1, #0
	cmp	r1, #0
	beq	.L3741
	b	.L3742
.L3739:
	mov	r3, r0
	mov	r2, r6
	mov	r0, r7
	ldr	r1, .L3756+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3737
.L3757:
	.align	2
.L3756:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC460
	.word	g_event_report
	.word	.LC461
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	cmp	r1, #0
	mov	r4, r0
	mov	r0, #0
	str	r0, [fp, #-64]
	str	r0, [fp, #-60]
	str	r0, [fp, #-56]
	beq	.L3829
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3830
	ldr	r2, [r4, #2356]
	ldrb	lr, [r4, #5]	@ zero_extendqisi2
	ldr	r3, [r2, #8]
	cmp	lr, #1
	mov	r3, r3, asl #16
	and	r3, r3, #458752
	str	r3, [r4, #264]
	ldr	r3, [r2]
	str	r3, [r4, #268]
	ldr	r3, [r2, #4]
	str	r3, [r4, #272]
	beq	.L3762
	ldr	r3, [r2, #832]
	cmp	r3, #1
	addne	r7, r4, #81920
	beq	.L3831
.L3763:
	mov	r0, r4
	bl	HEVC_ReceivePacket
	ldr	r3, [r4, #2364]
	str	r3, [r7, #3844]
	mov	r6, r0
.L3764:
	cmp	r6, #0
	mov	r3, #0
	strb	r3, [r4, #5]
	beq	.L3765
.L3788:
	mov	r5, #0
.L3825:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3831:
	ldr	r3, [r2, #872]
	add	r7, r4, #81920
	cmp	r3, #1
	bne	.L3763
	ldr	r3, [r7, #3840]
	cmp	r3, #1
	bne	.L3763
	ldr	r3, [r7, #3844]
	mov	r5, r0
	str	r3, [r4, #2364]
	str	r0, [r7, #3840]
	strb	r0, [r4, #5]
	b	.L3825
.L3765:
	mov	r0, r4
	bl	HEVCDEC_CheckSedOnly
	subs	r5, r0, #0
	beq	.L3766
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	beq	.L3828
	ldr	r0, [r4, #276]
	mvn	r5, #0
	bl	HEVC_ReleaseNAL
	mov	r0, r5
	str	r6, [r4, #2364]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3762:
	ldr	r3, [r4, #2364]
	cmp	r3, #0
	movne	r6, #0
	mvneq	r6, #0
	cmp	r3, #0
	bne	.L3764
	ldr	r1, .L3838
	mov	r5, r6
	bl	dprint_vfmw
	b	.L3825
.L3770:
	ldr	r1, .L3838+4
	mov	r0, r7
	bl	dprint_vfmw
	ldr	r1, [r4, #2364]
	cmp	r1, #0
	beq	.L3784
	ldr	r0, [r4, #276]
	bl	HEVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #2364]
.L3784:
	ldr	r3, .L3838+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3828
	mov	r3, #0
	ldr	r0, [r4, #276]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3828:
	mvn	r5, #0
	b	.L3825
.L3766:
	ldr	r3, [r4, #2364]
	str	r6, [r3, #4]
	ldr	r3, [r4, #2364]
	ldr	r0, [r4, #2340]
	ldr	r2, [r3, #48]
	ldr	r1, [r3, #44]
	bl	BsInit
	mov	r1, #24
	ldr	r0, [r4, #2340]
	bl	BsSkip
	mov	r1, #1
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	mov	r1, #6
	str	r0, [r6, #24]
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	mov	r1, #6
	str	r0, [r6, #16]
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	mov	r1, #3
	str	r0, [r6, #28]
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	str	r0, [r6, #32]
	ldr	r6, [r4, #2364]
	ldr	r3, [r6, #32]
	cmp	r3, #0
	subne	r3, r3, #1
	strne	r3, [r6, #32]
	ldrne	r6, [r4, #2364]
	ldr	r0, [r6, #16]
	bl	HEVC_IsSliceUnit
	cmp	r0, #1
	mov	r7, r0
	beq	.L3832
.L3769:
	mov	r3, #0
	str	r3, [r6, #4]
	ldr	r3, [r4, #2364]
	add	r0, r4, #2496
	add	r0, r0, #8
	ldr	r1, [r3, #44]
	ldr	r2, [r3, #48]
	bl	BsInit
	mov	r1, #24
	ldr	r0, [r4, #2340]
	bl	BsSkip
	mov	r1, #1
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	mov	r1, #6
	str	r0, [r6, #24]
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	mov	r1, #6
	str	r0, [r6, #16]
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	mov	r1, #3
	str	r0, [r6, #28]
	ldr	r0, [r4, #2340]
	ldr	r6, [r4, #2364]
	bl	BsGet
	sub	r0, r0, #1
	str	r0, [r6, #32]
.L3786:
	mov	r0, r4
	bl	HEVC_DecOneNal
	cmn	r0, #2
	beq	.L3789
	cmp	r0, #0
	bne	.L3833
	ldr	r3, [r4, #212]
	cmp	r3, #1
	bne	.L3788
	mov	r0, r4
	bl	HEVCDEC_SetDecParam
	b	.L3825
.L3830:
	ldr	r1, .L3838+12
	mvn	r5, #0
	bl	dprint_vfmw
	b	.L3825
.L3832:
	sub	r2, fp, #48
	sub	r1, fp, #52
	mov	r0, r4
	bl	HEVC_InquireSliceProperty
	subs	r3, r0, #0
	bne	.L3770
	ldr	r6, [fp, #-52]
	cmp	r6, #1
	beq	.L3834
.L3771:
	ldr	r6, [fp, #-48]
	cmp	r6, #1
	beq	.L3835
.L3783:
	ldr	r6, [r4, #2364]
	ldr	r0, [r6, #16]
	bl	HEVC_IsSliceUnit
	cmp	r0, #1
	beq	.L3786
	b	.L3769
.L3829:
	mov	r0, r1
	ldr	r1, .L3838+16
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L3825
.L3835:
	add	r3, r4, #53248
	ldr	r3, [r3, #1120]
	sub	r3, r3, #16
	cmp	r3, #4
	bls	.L3836
.L3776:
	ldrb	r6, [r4, #2]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L3837
.L3779:
	ldr	r0, [r4, #276]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L3783
	sub	r3, fp, #56
	ldr	r0, [r4, #276]
	sub	r2, fp, #60
	sub	r1, fp, #64
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r4, #2356]
	ldr	r0, [r3, #708]
	cmp	r0, #1
	bne	.L3783
	ldr	r9, [fp, #-56]
	cmp	r9, #0
	bgt	.L3781
	ldr	r3, [fp, #-60]
	cmp	r3, #32
	bgt	.L3781
	ldr	r8, [r4, #2360]
	cmp	r8, #0
	bne	.L3781
	ldr	r2, [fp, #-48]
	cmp	r2, #0
	beq	.L3783
	ldr	r3, [r4, #252]
	cmp	r3, #0
	beq	.L3783
	add	r7, r4, #24576
	add	r6, r4, #81920
	ldr	r3, [r7, #2868]
	ldr	r1, [r6, #3864]
	ldr	lr, [r3, #76]
	cmp	lr, r1
	beq	.L3783
	ldr	ip, [r3, #72]
	ldr	r3, [r6, #3868]
	subs	r10, ip, r3
	movne	r10, #1
	cmp	r9, #0
	movne	r10, #0
	cmp	r10, #0
	beq	.L3783
	stmia	sp, {r1, ip, lr}
	ldr	r1, .L3838+20
	bl	dprint_vfmw
	str	r8, [r6, #3860]
	ldr	r3, [r7, #2868]
	ldr	r1, .L3838+8
	ldr	r2, [r3, #76]
	ldr	ip, [r1]
	str	r2, [r6, #3864]
	cmp	ip, #0
	ldr	r3, [r3, #72]
	str	r3, [r6, #3868]
	beq	.L3783
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r4, #276]
	blx	ip
	b	.L3783
.L3834:
	ldr	r2, [r4, #208]
	cmp	r2, #0
	bne	.L3772
	add	r3, r4, #53248
	ldrb	r3, [r3, #369]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3771
	ldr	r2, .L3838+24
	ldr	r1, .L3838+28
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	b	.L3771
.L3833:
	ldr	r1, .L3838+32
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L3825
.L3789:
	mov	r5, r0
	b	.L3825
.L3772:
	ldr	ip, [r4, #2364]
	add	r0, r4, #2496
	mov	r2, #13
	add	r0, r0, #8
	ldr	r1, .L3838+36
	mvn	r5, #1
	str	r3, [ip, #4]
	bl	BsInit
	mov	r1, #40
	ldr	r0, [r4, #2340]
	bl	BsSkip
	ldr	r3, [r4, #2364]
	mov	r2, #48
	mov	r0, r4
	str	r2, [r3, #16]
	strb	r6, [r4, #6]
	bl	HEVC_DecOneNal
	ldr	r3, [r4, #304]
	strb	r6, [r4, #5]
	add	r3, r3, #1
	str	r3, [r4, #304]
	b	.L3825
.L3836:
	mov	r0, r4
	bl	HEVC_FlushOutputFrmFromDPB
	cmp	r0, #0
	beq	.L3776
	mov	r0, r6
	ldr	r2, .L3838+40
	ldr	r1, .L3838+44
	bl	dprint_vfmw
	b	.L3776
.L3837:
	mov	r0, r4
	bl	HEVC_EmptyFrmFromDPB
	cmp	r0, #0
	beq	.L3779
	mov	r0, r6
	ldr	r2, .L3838+40
	ldr	r1, .L3838+48
	bl	dprint_vfmw
	b	.L3779
.L3781:
	add	r2, r4, #81920
	mov	r1, #0
	mov	r3, #1
	mvn	r5, #1
	str	r1, [r2, #3860]
	strb	r3, [r4, #5]
	b	.L3825
.L3839:
	.align	2
.L3838:
	.word	.LC464
	.word	.LC467
	.word	g_event_report
	.word	.LC463
	.word	.LC462
	.word	.LC466
	.word	.LANCHOR1+944
	.word	.LC465
	.word	.LC468
	.word	.LANCHOR1+988
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
	bne	.L3842
	add	r3, r4, #53248
	ldrb	r3, [r3, #369]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3844
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3842:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3844:
	ldr	r2, .L3845
	ldr	r1, .L3845+4
	bl	dprint_vfmw
	mov	r0, r4
	bl	HEVC_ClearCurrPic
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3846:
	.align	2
.L3845:
	.word	.LANCHOR1+944
	.word	.LC465
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
	ldr	r2, [r0, #3244]
	cmp	r2, #0
	beq	.L3850
	mov	r0, #0
.L3849:
	ldr	r3, [r2]
	ldr	r1, [r2, #4]
	ldr	r2, [r2, #1356]
	add	r3, r3, r1
	add	r3, r3, #7
	cmp	r2, #0
	add	r0, r0, r3, lsr #3
	bne	.L3849
	ldmfd	sp, {fp, sp, pc}
.L3850:
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
	ldr	r3, [r0, #2540]
	mov	r4, r0
	cmp	r3, #0
	addne	r8, r0, #61696
	addne	r8, r8, #64
	mvnne	r6, #0
	movne	r7, #1
	bne	.L3863
	b	.L3864
.L3855:
	mov	r0, r4
	bl	HEVC_RemoveApcStore
	ldr	r1, [fp, #-52]
	mov	r0, r4
	bl	HEVC_RemoveFrameStoreOutDPB
	ldr	r2, [r4, #2540]
	cmp	r2, #0
	beq	.L3872
.L3863:
	sub	r1, fp, #48
	sub	r2, fp, #52
	mov	r0, r4
	bl	HEVC_GetMinPOC
	ldr	r1, [fp, #-52]
	add	r3, r1, #640
	ldr	r5, [r4, r3, asl #2]
	cmp	r5, #0
	beq	.L3855
	ldrb	r3, [r5, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3855
	mov	r1, r5
	mov	r0, r4
	strb	r7, [r5, #2]
	bl	HEVC_GetImagePara
	mov	r1, r5
	mov	r0, r4
	bl	HEVC_CheckFrameStore
	subs	r6, r0, #0
	beq	.L3873
.L3862:
	ldr	r1, [fp, #-52]
	b	.L3855
.L3872:
	cmp	r6, #0
	bne	.L3864
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3873:
	mov	r2, #1
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_SetDisplay
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L3874
	ldr	r3, [r4, #2540]
	mov	r2, r4
	mov	r1, #16
	cmp	r3, #1
	streq	r3, [r9, #188]
	mov	r3, r8
	ldr	r0, [r4, #276]
	str	r9, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	mov	r10, r0
	beq	.L3859
	mov	r3, r0
	ldr	r2, .L3876
	ldr	r1, .L3876+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r5, #32]
	ldr	r0, [r4, #276]
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r1, #1
	mov	r0, r4
	mvn	r10, #0
	bl	HEVC_ClearAll
.L3859:
	ldr	r3, [r4, #2356]
	ldr	r2, [r3, #604]
	add	r2, r2, #2032
	add	r2, r2, #15
	cmp	r2, #4096
	movcc	r3, #0
	strcc	r3, [r9, #24]
	bcs	.L3875
.L3861:
	ldr	r1, [r4, #2540]
	ldr	r3, [r4, #316]
	ldr	r2, [r4, #308]
	cmp	r1, #1
	add	r3, r3, #1
	str	r3, [r4, #316]
	add	r2, r2, #1
	str	r2, [r4, #308]
	beq	.L3862
	mov	r6, r10
	ldr	r1, [fp, #-52]
	b	.L3855
.L3864:
	add	r0, r4, #61696
	add	r0, r0, #64
	bl	GetVoLastImageID
	mov	r6, r0
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3875:
	mov	r1, r9
	mov	r0, r4
	bl	HEVC_SetFrmRepeatCount.part.4
	b	.L3861
.L3874:
	ldr	r1, .L3876+8
	mvn	r6, #0
	ldr	r2, .L3876
	bl	dprint_vfmw
	ldr	r1, [fp, #-52]
	b	.L3855
.L3877:
	.align	2
.L3876:
	.word	.LANCHOR1+1004
	.word	.LC470
	.word	.LC469
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
	ldr	r2, [r0, #2868]
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
	ldr	r2, [r0, #2868]
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
	.type	SarTable.14315, %object
	.size	SarTable.14315, 136
SarTable.14315:
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
	.type	__func__.14484, %object
	.size	__func__.14484, 18
__func__.14484:
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
	.type	__func__.14936, %object
	.size	__func__.14936, 27
__func__.14936:
	.ascii	"HEVC_GetUnRefPicWithMinPoc\000"
	.space	1
	.type	__func__.14945, %object
	.size	__func__.14945, 21
__func__.14945:
	.ascii	"HEVC_IncreaseDPBSize\000"
	.space	3
	.type	__func__.15176, %object
	.size	__func__.15176, 17
__func__.15176:
	.ascii	"HEVC_WritePicMsg\000"
	.space	3
	.type	__func__.15205, %object
	.size	__func__.15205, 16
__func__.15205:
	.ascii	"HEVC_CheckListX\000"
	.type	__func__.15329, %object
	.size	__func__.15329, 16
__func__.15329:
	.ascii	"HEVC_RepairList\000"
	.type	__func__.15702, %object
	.size	__func__.15702, 21
__func__.15702:
	.ascii	"HEVC_CheckFrameStore\000"
	.space	3
	.type	__func__.15545, %object
	.size	__func__.15545, 19
__func__.15545:
	.ascii	"HEVC_OutputCurrPic\000"
	.space	1
	.type	__func__.15550, %object
	.size	__func__.15550, 23
__func__.15550:
	.ascii	"HEVC_FirstFrameFastOut\000"
	.space	1
	.type	__func__.15848, %object
	.size	__func__.15848, 14
__func__.15848:
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
	.type	__func__.16129, %object
	.size	__func__.16129, 16
__func__.16129:
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
	.type	__func__.15027, %object
	.size	__func__.15027, 27
__func__.15027:
	.ascii	"HEVC_DecSliceSegmentHeader\000"
	.space	1
	.type	__func__.15408, %object
	.size	__func__.15408, 13
__func__.15408:
	.ascii	"HEVC_InitDPB\000"
	.space	3
	.type	__func__.16189, %object
	.size	__func__.16189, 13
__func__.16189:
	.ascii	"HEVCDEC_Init\000"
	.space	3
	.type	__func__.15694, %object
	.size	__func__.15694, 19
__func__.15694:
	.ascii	"HEVC_OutputFrmToVO\000"
	.space	1
	.type	__func__.15661, %object
	.size	__func__.15661, 22
__func__.15661:
	.ascii	"HEVC_OutputFrmFromDPB\000"
	.space	2
	.type	__func__.14951, %object
	.size	__func__.14951, 30
__func__.14951:
	.ascii	"HEVC_OutputUnRefPicWithMinPoc\000"
	.space	2
	.type	__func__.15674, %object
	.size	__func__.15674, 27
__func__.15674:
	.ascii	"HEVC_FlushOutputFrmFromDPB\000"
	.space	1
	.type	__func__.16177, %object
	.size	__func__.16177, 23
__func__.16177:
	.ascii	"HEVCDEC_GetImageBuffer\000"
	.space	1
	.type	__func__.16228, %object
	.size	__func__.16228, 14
__func__.16228:
	.ascii	"HEVC_ClearDPB\000"
	.space	2
	.type	__func__.14956, %object
	.size	__func__.14956, 23
__func__.14956:
	.ascii	"HEVC_RecoverDPBProcess\000"
	.space	1
	.type	__func__.15638, %object
	.size	__func__.15638, 20
__func__.15638:
	.ascii	"HEVC_InsertFrmInDPB\000"
	.type	__func__.15555, %object
	.size	__func__.15555, 22
__func__.15555:
	.ascii	"HEVC_SimpleDPBProcess\000"
	.space	2
	.type	__func__.15560, %object
	.size	__func__.15560, 21
__func__.15560:
	.ascii	"HEVC_DecOrderProcess\000"
	.space	3
	.type	__func__.15565, %object
	.size	__func__.15565, 22
__func__.15565:
	.ascii	"HEVC_DispOrderProcess\000"
	.space	2
	.type	__func__.15581, %object
	.size	__func__.15581, 19
__func__.15581:
	.ascii	"HEVC_StorePicInDPB\000"
	.space	1
	.type	__func__.14983, %object
	.size	__func__.14983, 14
__func__.14983:
	.ascii	"HEVC_DecSlice\000"
	.space	2
	.type	__func__.15282, %object
	.size	__func__.15282, 12
__func__.15282:
	.ascii	"HEVC_DecVDM\000"
	.type	__func__.16292, %object
	.size	__func__.16292, 20
__func__.16292:
	.ascii	"HEVC_HaveSliceToDec\000"
	.type	__func__.14431, %object
	.size	__func__.14431, 21
__func__.14431:
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
	.type	__func__.16309, %object
	.size	__func__.16309, 21
__func__.16309:
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
	ASCII(.ascii	"CtbNumOneFrame=%d, SedEnd0=%d\012\000" )
	.space	1
.LC460:
	ASCII(.ascii	"HEVC_DecSliceSegmentHeader dec err\012\000" )
.LC461:
	ASCII(.ascii	"CurBitsLen = %d, MinBitsLen = %d\012\000" )
	.space	2
.LC462:
	ASCII(.ascii	"HEVCDEC_DecodePacket NULL == pPacket!\012\000" )
	.space	1
.LC463:
	ASCII(.ascii	"HevcCtx not init!\012\000" )
	.space	1
.LC464:
	ASCII(.ascii	"HEVCDEC_DecodePacket NULL == pHevcCtx->pCurrNal!\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC465:
	ASCII(.ascii	"%s last frame fs unrelease, fix it!\012\000" )
	.space	3
.LC466:
	ASCII(.ascii	"IsSizeChanged = %d,old w/h = %d/%d,new w/h = %d/%d\012" )
	ASCII(.ascii	"\000" )
.LC467:
	ASCII(.ascii	"HEVC_InquireSliceProperty error.\012\000" )
	.space	2
.LC468:
	ASCII(.ascii	"HEVC_DecOneNal ERR\012\000" )
.LC469:
	ASCII(.ascii	"%s pstLogicFsImage is NULL!\012\000" )
	.space	3
.LC470:
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
