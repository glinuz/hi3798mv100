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
	.file	"mpeg2.c"
	.text
	.align	2
	.type	MP2_SearchStartCode.part.1, %function
MP2_SearchStartCode.part.1:
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
	add	r3, r0, #2
	add	ip, r0, r1
.L12:
	cmp	ip, r3
	bls	.L2
.L15:
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	bls	.L14
.L8:
	add	r3, r3, #3
	cmp	ip, r3
	bhi	.L15
.L2:
	sub	r0, r1, #3
	ldmfd	sp, {fp, sp, pc}
.L14:
	cmp	r2, #0
	bne	.L5
	ldrb	r2, [r3, #-1]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #1
	beq	.L12
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #2
	addne	r3, r3, #4
	b	.L12
.L5:
	cmp	r2, #1
	bne	.L12
	ldrb	r2, [r3, #-2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L8
	ldrb	r2, [r3, #-1]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L8
	rsb	r3, r0, r3
	sub	r0, r3, #2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_SearchStartCode.part.1, .-MP2_SearchStartCode.part.1
	.align	2
	.global	MP2_VerifyMemPara
	.type	MP2_VerifyMemPara, %function
MP2_VerifyMemPara:
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
	.size	MP2_VerifyMemPara, .-MP2_VerifyMemPara
	.align	2
	.global	MP2_ArrangeVHBMem
	.type	MP2_ArrangeVHBMem, %function
MP2_ArrangeVHBMem:
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
	add	r6, r0, #86016
	mov	r5, r0
	ldr	r3, [r6, #1688]
	ldr	r2, [r3, #48]
	ldr	r3, [r3, #52]
	cmp	r2, #31
	bls	.L19
	cmp	r3, #31
	bls	.L19
	ldr	r1, [r0]
	ldr	r0, [r1, #28]
	cmp	r0, #25
	beq	.L42
	mov	r7, r2
	mov	r10, r3
	mov	r8, #5
	mov	r4, #2
	mov	r9, #3
.L28:
	ldr	r3, .L45
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r2, [r5]
	str	r10, [fp, #-48]
	mov	r3, #0
	str	r7, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r10, [fp, #-64]
	cmp	r2, #24
	str	r7, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L33
	uxtb	r3, r4
	strb	r9, [fp, #-73]
	strb	r8, [fp, #-71]
.L38:
	sub	r1, fp, #76
	ldr	r0, [r6, #2048]
	mov	r4, #0
	strb	r3, [fp, #-70]
	strb	r4, [fp, #-74]
	bl	FSP_ConfigInstance
	cmp	r0, r4
	bne	.L34
	ldr	r1, [r5]
	sub	r3, fp, #80
	ldr	r0, [r6, #2048]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L43
	cmp	r0, #0
	moveq	r0, #1
	beq	.L41
.L36:
	ldr	r1, .L45+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
	b	.L41
.L19:
	ldr	r1, .L45+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #0
.L41:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L34:
	ldr	r1, .L45+12
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L42:
	ldr	r0, [r1, #708]
	cmp	r0, #0
	bne	.L23
	ldr	r7, [r1, #668]
	ldr	r10, [r1, #672]
	cmp	r2, r7
	bhi	.L25
	cmp	r3, r10
	bhi	.L25
	ldr	r2, [r1, #696]
	ldr	r3, [r1, #692]
	cmp	r2, #0
	add	r9, r3, #2
	beq	.L40
	add	r3, r3, #1
	cmp	r3, #17
	movlt	r4, r3
	movge	r4, #17
.L32:
	ldrb	r8, [r1, #716]	@ zero_extendqisi2
	b	.L28
.L23:
	ldr	r0, [r1, #668]
	ldr	ip, [r1, #672]
	cmp	r2, r0
	bhi	.L30
	cmp	r3, ip
	bhi	.L30
	ldr	r0, [r1, #696]
	mov	r7, r2
	ldr	r4, [r1, #692]
	mov	r10, r3
	cmp	r0, #0
	add	r9, r4, #2
	bne	.L44
.L40:
	mov	r4, #1
	b	.L32
.L33:
	mov	r3, #1
	strb	r8, [fp, #-71]
	strb	r3, [fp, #-73]
	b	.L38
.L30:
	stmia	sp, {r0, ip}
	mov	r0, #0
	ldr	r1, .L45+16
	bl	dprint_vfmw
	mov	r0, #0
	b	.L41
.L44:
	add	r3, r4, #1
	cmp	r3, #17
	movlt	r4, r3
	movge	r4, #17
	b	.L32
.L25:
	stmia	sp, {r7, r10}
	mov	r0, #0
	ldr	r1, .L45+16
	bl	dprint_vfmw
	mov	r0, #0
	b	.L41
.L43:
	add	r0, r5, #87040
	add	r0, r0, #672
	bl	ResetVoQueue
	b	.L36
.L46:
	.align	2
.L45:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC3
	.word	.LC1
	.word	.LC4
	.word	.LC2
	UNWIND(.fnend)
	.size	MP2_ArrangeVHBMem, .-MP2_ArrangeVHBMem
	.align	2
	.global	MP2_DecodingDecide
	.type	MP2_DecodingDecide, %function
MP2_DecodingDecide:
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
	add	r4, r0, #86016
	mov	r5, r0
	ldr	r3, [r4, #364]
	ldr	r7, [r4, #1688]
	tst	r3, #8388608
	mvneq	r6, #0
	bne	.L124
.L48:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L124:
	ldrb	r2, [r7, #9]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L49
	ldr	r2, [r4, #376]
	cmp	r2, #1
	beq	.L49
	ands	r6, r3, #255
	beq	.L51
.L127:
	ldr	r2, .L143
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L52
	mov	r3, #0
	mov	r1, #103
	mov	r2, r3
	ldr	r0, [r4, #2048]
	blx	ip
	ldr	r3, [r4, #364]
.L52:
	ldr	r2, [r4, #372]
	mov	r0, #22
	ldr	r1, .L143+4
	mov	r6, #0
	bl	dprint_vfmw
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L51:
	ldr	r3, [r4, #412]
	cmp	r3, #0
	beq	.L125
	ldr	r3, [r4, #352]
	sub	r3, r3, #2
	cmp	r3, #1
	ldr	r3, [r5]
	ldr	r2, [r3, #8]
	bls	.L126
.L55:
	sub	r3, r2, #1
	str	r2, [r4, #352]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L120
.L62:
	.word	.L61
	.word	.L63
	.word	.L64
	.word	.L65
.L49:
	ldr	r2, [r4, #2052]
	ands	r6, r3, #255
	add	r2, r2, #1
	str	r2, [r4, #2052]
	beq	.L51
	b	.L127
.L65:
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	cmp	r10, #1
	beq	.L128
	cmp	r10, #3
	beq	.L129
.L71:
	cmp	r10, #2
	beq	.L75
.L120:
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	mov	r3, #1
	ldrb	r9, [r7, #9]	@ zero_extendqisi2
	str	r3, [fp, #-52]
.L66:
	ldr	r2, [r4, #372]
	add	r0, r4, #608
	ldr	r1, [r4, #312]
	ldr	r8, [r4, #416]
	str	r2, [fp, #-48]
	add	r8, r8, r1
	bl	BsPos
	mov	r3, r10
	str	r9, [sp]
	ldr	r2, [fp, #-48]
	ldr	r1, .L143+8
	add	r8, r8, r0, lsr #3
	mov	r0, #18
	str	r8, [sp, #4]
	bl	dprint_vfmw
	ldr	r3, [r4, #364]
	bic	r3, r3, #14155776
	str	r3, [r4, #364]
	ldr	r3, [r5]
	ldr	r0, [r3, #612]
	ldr	r3, [r4, #404]
	cmp	r0, #1
	beq	.L130
	cmp	r3, #0
	bne	.L79
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	sub	r1, r2, #2
	cmp	r1, #1
	bls	.L131
.L80:
	cmp	r2, #3
	beq	.L132
.L77:
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #444]
	mov	r8, r0
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	clz	r3, r8
	mov	r3, r3, lsr #5
	clz	r1, r0
	mov	r1, r1, lsr #5
	orrs	r2, r1, r3
	bne	.L133
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	cmp	r2, #2
	beq	.L134
	cmp	r2, #3
	beq	.L135
.L85:
	ldr	r3, [fp, #-52]
	cmp	r3, #0
	movne	r6, #1
	bne	.L48
	ldrb	r3, [r7, #9]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L91
	ldr	r3, [r4, #376]
	cmp	r3, #0
	bne	.L48
.L91:
	ldrb	r3, [r7, #27]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L136
	ldrb	r3, [r7, #18]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L137
	ldr	r3, [r4, #528]
	add	r3, r3, #1
	str	r3, [r4, #528]
	mov	r3, r3, asl #1
	str	r3, [r4, #524]
.L93:
	ldr	r2, [r4, #520]
	ldr	r3, [r4, #372]
	add	r2, r2, #1
	str	r2, [r4, #520]
	add	r3, r3, #1
	str	r3, [r4, #372]
	b	.L48
.L61:
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	cmp	r10, #3
	beq	.L138
.L122:
	mov	r3, #1
	ldrb	r9, [r7, #9]	@ zero_extendqisi2
	str	r3, [fp, #-52]
	b	.L66
.L64:
	mov	r3, #0
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	str	r3, [fp, #-52]
	ldrb	r9, [r7, #9]	@ zero_extendqisi2
	b	.L66
.L63:
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	sub	r3, r10, #2
	cmp	r3, #1
	bhi	.L122
	cmp	r10, #2
	beq	.L139
.L69:
	ldrb	r9, [r7, #9]	@ zero_extendqisi2
	cmp	r9, #3
	beq	.L140
.L72:
	ldr	r3, [r4, #376]
	mov	r2, #0
	str	r2, [fp, #-52]
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r4, #376]
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	ldrb	r9, [r7, #9]	@ zero_extendqisi2
	b	.L66
.L79:
	cmp	r3, #1
	bhi	.L77
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	b	.L80
.L130:
	cmp	r3, #0
	bne	.L77
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L141
	cmp	r2, #2
	bne	.L77
	ldr	r1, .L143+12
	bl	dprint_vfmw
	b	.L77
.L126:
	cmp	r2, #4
	cmpne	r2, #1
	bhi	.L55
	ldr	r1, .L143+16
	mov	r0, #2
	bl	dprint_vfmw
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L119
	ldr	r3, [r4, #364]
	ldr	r1, [r4, #444]
	orr	r3, r3, #128
	ldr	r0, [r4, #2048]
	str	r3, [r4, #364]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L58
	add	r9, r8, #576
	ldr	r1, [r8, #560]
	ldr	r0, [r4, #2048]
	add	r10, r8, #592
	bl	FreeUsdByDec
	ldr	r1, [r9, #-8]
	ldr	r0, [r4, #2048]
	add	r3, r8, #560
	str	r3, [fp, #-48]
	bl	FreeUsdByDec
	ldr	r1, [r8, #576]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	ldr	r1, [r10, #-8]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-48]
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r3]
	strd	r0, [r9, #-8]
	strd	r0, [r9]
	strd	r0, [r10, #-8]
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	str	r6, [r4, #440]
	str	r6, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L142
.L58:
	mov	r3, #0
	str	r3, [r4, #404]
.L119:
	ldr	r3, [r5]
	ldr	r2, [r3, #8]
	b	.L55
.L125:
	ldr	r2, [r4, #372]
	mov	r0, #1
	ldr	r1, .L143+20
	bl	dprint_vfmw
	b	.L48
.L134:
	cmp	r3, #0
	bne	.L83
	ldr	r3, [r5]
	ldr	r2, [r8, #160]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L85
	ldr	r1, .L143+24
	mov	r0, #1
	bl	dprint_vfmw
	b	.L48
.L133:
	movw	r3, #882
	ldr	r2, .L143+28
	ldr	r1, .L143+32
	mov	r0, #1
	bl	dprint_vfmw
	b	.L48
.L135:
	cmp	r3, #0
	ldr	r3, [r5]
	ldr	r2, [r8, #160]
	ldr	r3, [r3]
	bne	.L87
	cmp	r2, r3
	bhi	.L87
	cmp	r1, #0
	ldr	r2, [r0, #160]
	bne	.L90
	cmp	r3, r2
	bcs	.L85
.L90:
	ldr	r1, .L143+36
	mov	r0, #1
	bl	dprint_vfmw
	b	.L48
.L136:
	ldrb	r2, [r7, #16]	@ zero_extendqisi2
	ldrb	r3, [r7, #10]	@ zero_extendqisi2
	ldr	r1, [r4, #528]
	add	r3, r2, r3
	add	r3, r3, #1
	add	r3, r3, r1
	str	r3, [r4, #528]
	mov	r3, r3, asl #1
	str	r3, [r4, #524]
	b	.L93
.L87:
	ldr	r1, .L143+40
	mov	r0, #1
	bl	dprint_vfmw
	b	.L48
.L129:
	ldr	r3, [r4, #2060]
	cmp	r3, #0
	bne	.L122
.L74:
	ldrb	r9, [r7, #9]	@ zero_extendqisi2
	cmp	r9, #3
	bne	.L72
	mov	r3, #0
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	str	r3, [fp, #-52]
	b	.L66
.L128:
	mov	r3, #0
	str	r3, [r4, #2060]
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	cmp	r10, #3
	bne	.L71
	b	.L74
.L138:
	ldrb	r10, [r7, #9]	@ zero_extendqisi2
	cmp	r10, #3
	bne	.L72
	mov	r3, #0
	mov	r9, r10
	str	r3, [fp, #-52]
	b	.L66
.L137:
	ldr	r2, [r4, #528]
	ldr	r3, [r4, #524]
	add	r2, r2, #1
	str	r2, [r4, #528]
	ldrb	r2, [r7, #16]	@ zero_extendqisi2
	add	r2, r2, #2
	add	r3, r3, r2
	str	r3, [r4, #524]
	b	.L93
.L75:
	mov	r9, #1
	str	r9, [r4, #2060]
	str	r9, [fp, #-52]
	ldrb	r10, [r7, #22]	@ zero_extendqisi2
	ldrb	r9, [r7, #9]	@ zero_extendqisi2
	b	.L66
.L131:
	ldr	r1, .L143+44
	mov	r0, #1
	str	r3, [fp, #-52]
	bl	dprint_vfmw
	b	.L77
.L132:
	ldr	r1, .L143+48
	mov	r0, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [fp, #-52]
	b	.L77
.L83:
	mov	r3, #892
	ldr	r2, .L143+28
	ldr	r1, .L143+52
	mov	r0, #1
	bl	dprint_vfmw
	b	.L48
.L141:
	ldr	r1, .L143+56
	str	r3, [fp, #-52]
	bl	dprint_vfmw
	b	.L77
.L139:
	add	r3, r5, #45056
	ldrb	r2, [r3, #305]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L69
	ldrb	r9, [r3, #304]	@ zero_extendqisi2
	cmp	r9, #1
	streq	r9, [fp, #-52]
	ldreqb	r9, [r7, #9]	@ zero_extendqisi2
	bne	.L69
	b	.L66
.L140:
	mov	r3, #0
	str	r3, [fp, #-52]
	b	.L66
.L142:
	ldr	r0, [r4, #2048]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #440]
	str	r0, [r4, #444]
	b	.L58
.L144:
	.align	2
.L143:
	.word	g_event_report
	.word	.LC5
	.word	.LC8
	.word	.LC10
	.word	.LC7
	.word	.LC6
	.word	.LC15
	.word	.LANCHOR0
	.word	.LC13
	.word	.LC17
	.word	.LC16
	.word	.LC11
	.word	.LC12
	.word	.LC14
	.word	.LC9
	UNWIND(.fnend)
	.size	MP2_DecodingDecide, .-MP2_DecodingDecide
	.align	2
	.global	MP2_EstimatePicQp
	.type	MP2_EstimatePicQp, %function
MP2_EstimatePicQp:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	mov	r2, #128
	mov	r4, r0
	ldr	r1, .L151
	sub	r0, fp, #148
	bl	memcpy
	add	r3, r4, #86016
	ldr	r0, [r4, #320]
	ldr	r2, [r3, #400]
	cmp	r2, #1
	beq	.L146
	ldr	r3, [r3, #1688]
	ldrb	r3, [r3, #13]	@ zero_extendqisi2
	cmp	r3, #0
	subne	r3, fp, #20
	moveq	r0, r0, asl #1
	addne	r0, r3, r0, lsl #2
	ldrne	r0, [r0, #-128]
.L146:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L152:
	.align	2
.L151:
	.word	.LANCHOR0+20
	UNWIND(.fnend)
	.size	MP2_EstimatePicQp, .-MP2_EstimatePicQp
	.align	2
	.global	MP2_GetSequenceInfo
	.type	MP2_GetSequenceInfo, %function
MP2_GetSequenceInfo:
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
	UNWIND(.pad #52)
	sub	sp, sp, #52
	add	r4, r0, #86016
	mov	r3, #0
	mov	r6, r0
	ldr	r2, [r4, #400]
	str	r3, [fp, #-64]
	cmp	r2, #1
	ldr	r5, [r4, #1688]
	ldr	r7, [r4, #1692]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	beq	.L155
	cmp	r2, #2
	bne	.L221
	ldr	r3, [r4, #364]
	and	r3, r3, #1769472
	cmp	r3, #589824
	beq	.L158
.L157:
	ldr	r1, .L231
	mov	r0, #17
	bl	dprint_vfmw
	ldr	r3, [r4, #400]
	cmp	r3, #1
	beq	.L222
.L160:
	ldr	r3, [r5, #48]
	add	r3, r3, #15
	mov	r3, r3, lsr #4
	str	r3, [r5, #56]
	ldr	r2, [r4, #400]
	cmp	r2, #2
	beq	.L223
.L161:
	ldr	r2, [r5, #52]
	add	r2, r2, #15
	mov	r2, r2, lsr #4
.L162:
	mov	r1, #0
	str	r2, [r5, #60]
	str	r1, [r5, #64]
	ldr	r1, [r4, #400]
	cmp	r1, #2
	beq	.L224
.L163:
	ldr	r1, [r4, #412]
	cmp	r1, #0
	beq	.L164
	ldr	r0, [r7, #56]
	cmp	r3, r0
	beq	.L225
.L164:
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	subs	r10, r0, #0
	beq	.L167
	ldr	r1, [r10, #560]
	add	r9, r10, #576
	ldr	r0, [r4, #2048]
	add	r8, r10, #592
	bl	FreeUsdByDec
	ldr	r1, [r10, #568]
	ldr	r0, [r4, #2048]
	add	r3, r10, #560
	str	r3, [fp, #-72]
	bl	FreeUsdByDec
	ldr	r1, [r10, #576]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	ldr	r1, [r10, #584]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-72]
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r3]
	strd	r0, [r9, #-8]
	strd	r0, [r9]
	strd	r0, [r8, #-8]
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2048]
	str	r3, [r4, #440]
	str	r3, [r4, #444]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L226
.L167:
	add	r1, r6, #87040
	mov	r3, #0
	add	r1, r1, #672
	str	r3, [r4, #404]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r6]
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L169
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L227
.L169:
	ldr	r3, [r4, #412]
	cmp	r3, #0
	beq	.L171
	ldr	r3, [r7, #56]
	cmp	r3, #0
	beq	.L171
	ldr	r3, [r7, #60]
	cmp	r3, #0
	bne	.L228
.L171:
	ldr	r0, [r4, #2048]
	bl	FSP_GetFspType
	cmp	r0, #0
	ldrne	r8, .L231+4
	bne	.L172
	sub	r3, fp, #56
	sub	r2, fp, #60
	sub	r1, fp, #64
	ldr	r0, [r4, #2048]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r6]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L229
.L218:
	ldr	r8, .L231+4
.L174:
	mov	r3, #0
	strb	r3, [r4, #2065]
.L172:
	mov	r3, #0
	mov	r0, r6
	mov	r1, r3
	str	r3, [r4, #412]
	str	r3, [r4, #428]
	bl	MP2_ArrangeVHBMem
	ldr	r1, .L231+8
	str	r0, [r4, #412]
	mov	r0, #1
	ldr	r3, [r5, #60]
	ldr	r2, [r5, #56]
	mov	r3, r3, asl #4
	mov	r2, r2, asl #4
	bl	dprint_vfmw
.L170:
	ldr	ip, [r8]
	cmp	ip, #0
	beq	.L219
	ldr	r0, [r5, #60]
	mov	r3, #8
	ldr	r8, [r7, #56]
	sub	r2, fp, #52
	ldr	r6, [r7, #60]
	mov	r1, #2
	ldr	lr, [r5, #56]
	strh	r0, [fp, #-46]	@ movhi
	strh	r8, [fp, #-52]	@ movhi
	strh	r6, [fp, #-50]	@ movhi
	strh	lr, [fp, #-48]	@ movhi
	ldr	r0, [r4, #2048]
	blx	ip
.L219:
	ldr	r1, [r4, #412]
.L165:
	str	r1, [r4, #360]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L221:
	ldr	r3, [r4, #364]
	tst	r3, #3538944
	beq	.L158
	tst	r3, #131072
	mov	r2, #2
	orreq	r3, r3, r2
	streq	r3, [r4, #364]
	tst	r3, #1048576
	str	r2, [r4, #400]
	orreq	r3, r3, #16
	streq	r3, [r4, #364]
	moveq	r2, #2
	movne	r2, #2
	b	.L157
.L158:
	mov	r2, #1
	str	r2, [r4, #400]
	b	.L157
.L155:
	ldr	r3, [r4, #364]
	mov	r1, #18
	bfi	r1, r1, #16, #16
	and	r3, r3, r1
	cmp	r3, #1179648
	moveq	r2, #2
	streq	r2, [r4, #400]
	b	.L157
.L224:
	ldrb	r1, [r5, #27]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L163
	ldr	r1, [r5, #52]
	add	r0, r1, #31
	add	r1, r1, #15
	mov	r0, r0, lsr #5
	mov	r1, r1, lsr #4
	rsb	r1, r1, r0, lsl #1
	str	r1, [r5, #64]
	b	.L163
.L223:
	ldrb	r2, [r5, #27]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L161
	ldr	r2, [r5, #52]
	add	r2, r2, #31
	mov	r2, r2, lsr #5
	mov	r2, r2, asl #1
	b	.L162
.L222:
	mov	r2, #0
	mov	r0, #3
	mov	r1, #5
	strb	r3, [r5, #27]
	strb	r3, [r5, #18]
	strb	r3, [r5, #11]
	strb	r3, [r5, #28]
	strb	r0, [r5, #9]
	str	r1, [r5, #144]
	strb	r2, [r5, #12]
	strb	r2, [r5, #13]
	strb	r2, [r5, #14]
	strb	r2, [r5, #15]
	strb	r2, [r5, #8]
	b	.L160
.L228:
	mov	r3, #0
	ldr	r1, .L231+12
	str	r3, [r4, #412]
	mov	r0, #1
	bl	dprint_vfmw
	b	.L171
.L225:
	ldr	r3, [r7, #60]
	cmp	r2, r3
	bne	.L164
	b	.L165
.L226:
	ldr	r0, [r4, #2048]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #440]
	str	r0, [r4, #444]
	b	.L167
.L227:
	ldr	r9, [r4, #412]
	cmp	r9, #0
	beq	.L211
	ldr	r8, .L231+4
	b	.L170
.L229:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	ble	.L230
.L175:
	mov	r3, #1
	strb	r3, [r4, #2065]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L230:
	ldr	r3, [fp, #-60]
	cmp	r3, #32
	bgt	.L175
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne	.L175
	ldr	r3, [r4, #2052]
	cmp	r3, #0
	beq	.L218
	ldr	r1, [r5, #56]
	cmp	r1, #0
	beq	.L218
	ldr	lr, [r5, #60]
	ldr	r0, [r4, #2072]
	cmp	lr, r0
	beq	.L218
	ldr	ip, [r4, #2068]
	cmp	r1, ip
	beq	.L218
	stmib	sp, {r0, r1, lr}
	mov	r0, r2
	str	ip, [sp]
	ldr	r1, .L231+16
	ldr	r8, .L231+4
	bl	dprint_vfmw
	ldr	r3, [r5, #60]
	ldr	ip, [r8]
	str	r3, [r4, #2072]
	ldr	r3, [r5, #56]
	cmp	ip, #0
	str	r3, [r4, #2068]
	beq	.L174
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r4, #2048]
	blx	ip
	b	.L174
.L211:
	mov	r0, r6
	mov	r1, r9
	bl	MP2_ArrangeVHBMem
	ldr	r8, .L231+4
	str	r9, [r4, #428]
	str	r0, [r4, #412]
	b	.L170
.L232:
	.align	2
.L231:
	.word	.LC18
	.word	g_event_report
	.word	.LC21
	.word	.LC19
	.word	.LC20
	UNWIND(.fnend)
	.size	MP2_GetSequenceInfo, .-MP2_GetSequenceInfo
	.global	__aeabi_idiv
	.align	2
	.global	MP2_CalcRealEncFrameRate
	.type	MP2_CalcRealEncFrameRate, %function
MP2_CalcRealEncFrameRate:
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
	add	r4, r0, #86016
	ldr	r3, [r4, #1688]
	ldr	r5, [r4, #504]
	ldr	lr, [r4, #516]
	ldrb	r2, [r3, #22]	@ zero_extendqisi2
	ldr	ip, [r3, #72]
	cmp	r2, #1
	ldr	r1, [r4, #512]
	ldr	r2, [r4, #500]
	ldr	r6, [r4, #496]
	ldr	r0, [r4, #508]
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	cmp	ip, #0
	add	r0, r6, r0
	add	r1, r2, r1
	movgt	r2, #1
	movle	r2, #0
	cmp	r0, #0
	andgt	r2, r2, #1
	movle	r2, #0
	cmp	r1, #0
	andgt	r2, r2, #1
	movle	r2, #0
	cmp	r2, #0
	streq	r2, [r4, #532]
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldrb	r3, [r3, #27]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r0, r0, asl #1
	addeq	r1, r5, lr
	mul	r0, ip, r0
	bl	__aeabi_idiv
	mov	r3, #100
	mul	r0, r3, r0
	add	r3, r0, #1020
	cmp	r0, #0
	add	r3, r3, #3
	movlt	r0, r3
	mov	r0, r0, asr #10
	str	r0, [r4, #532]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_CalcRealEncFrameRate, .-MP2_CalcRealEncFrameRate
	.align	2
	.global	MP2_CalcDar
	.type	MP2_CalcDar, %function
MP2_CalcDar:
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
	add	r0, r0, #86016
	ldr	r3, [r0, #1688]
	ldr	r3, [r3, #84]
	sub	r3, r3, #1
	cmp	r3, #2
	ldrls	r2, .L241
	addls	r3, r2, r3, lsl #2
	ldrls	r0, [r3, #148]
	movhi	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L242:
	.align	2
.L241:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MP2_CalcDar, .-MP2_CalcDar
	.align	2
	.global	MP2_SetImgFormat
	.type	MP2_SetImgFormat, %function
MP2_SetImgFormat:
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
	add	r3, r0, #86016
	str	r0, [fp, #-48]
	ldr	r1, [r3, #448]
	ldr	r0, [r3, #2048]
	str	r3, [fp, #-52]
	ldr	r5, [r3, #1688]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L243
	ldrb	ip, [r5, #22]	@ zero_extendqisi2
	ldrb	r3, [r5, #27]	@ zero_extendqisi2
	cmp	ip, #4
	ldr	r8, [r4, #108]
	subne	ip, ip, #1
	ldrb	lr, [r5, #9]	@ zero_extendqisi2
	andne	ip, ip, #3
	moveq	ip, #0
	cmp	r3, #0
	ubfx	r8, r8, #10, #2
	orr	r8, r8, lr
	ldr	r2, [r5, #128]
	moveq	r9, #1
	movne	r9, #0
	movne	lr, #4096
	beq	.L274
.L246:
	ldr	r3, [fp, #-52]
	and	r2, r2, #3
	mov	r2, r2, asl #5
	ldr	r3, [r3, #1688]
	orr	r2, r2, r9, asl #8
	ldr	r3, [r3, #84]
	sub	r3, r3, #1
	cmp	r3, #2
	ldrls	r1, .L279
	movhi	r1, #0
	movhi	r0, r1
	addls	r3, r1, r3, lsl #2
	ldrls	r1, [r3, #148]
	and	r3, r8, #3
	andls	r0, r1, #7
	orr	r3, r2, r3, asl #10
	orr	r3, r3, ip
	orr	r3, r3, lr
	movls	r0, r0, asl #14
	orr	r0, r3, r0
	str	r0, [r4, #108]
	ldr	r3, [r5, #56]
	mov	r3, r3, asl #4
	cmp	r3, #1920
	ldr	r3, [fp, #-48]
	orrls	r0, r0, #131072
	bichi	r0, r0, #131072
	str	r0, [r4, #108]
	ldr	r3, [r3]
	ldr	r3, [r3, #592]
	cmp	r3, #1
	movne	lr, #0
	beq	.L275
.L252:
	orr	r0, lr, r0
	str	ip, [r4, #100]
	str	r0, [r4, #108]
	mov	r0, r4
	str	ip, [r4, #104]
	bl	SetAspectRatio
	mov	r3, #3
	str	r3, [r4, #376]
	mov	r2, #1
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	strb	r2, [r4, #371]
	cmp	r3, #3
	moveq	r3, #0
	movne	r3, #4
	strb	r3, [r4, #364]
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	streqb	r3, [r4, #371]
	ldrneb	r3, [r3, #144]	@ zero_extendqisi2
	moveq	r2, #32
	moveq	r3, r2
	streq	r2, [r4, #380]
	mov	r2, #1
	strb	r2, [r4, #369]
	strne	r3, [r4, #380]
	str	r3, [r4, #388]
	str	r3, [r4, #384]
	ldr	r3, [r5, #72]
	ldr	r2, [fp, #-48]
	str	r3, [r4, #136]
	ldr	r3, [r5, #48]
	str	r3, [r4, #112]
	ldr	r1, [r5, #52]
	str	r1, [r4, #116]
	ldr	r2, [r2]
	ldr	r2, [r2, #28]
	cmp	r2, #25
	beq	.L276
.L256:
	mov	r2, r1, lsr #1
	mov	r0, r3, lsr #1
	str	r2, [r4, #132]
	str	r0, [r4, #128]
	ldr	r2, [r5, #148]
	cmp	r2, #0
	bne	.L277
.L257:
	str	r3, [r4, #120]
	str	r1, [r4, #124]
.L258:
	ldrb	r3, [r5, #27]	@ zero_extendqisi2
	ldr	r0, [fp, #-48]
	str	r3, [r4, #476]
	ldrb	r3, [r5, #18]	@ zero_extendqisi2
	str	r3, [r4, #480]
	ldr	r3, [r5, #144]
	str	r3, [r4, #488]
	bl	MP2_CalcRealEncFrameRate
	ldr	r3, [fp, #-52]
	ldr	r3, [r3, #532]
	str	r3, [r4, #484]
.L243:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L277:
	ldr	r0, [r5, #152]
	cmp	r0, #0
	beq	.L257
	cmp	r3, r2
	movcs	r3, r2
	str	r3, [r4, #120]
	ldr	r3, [r5, #152]
	cmp	r1, r3
	movcs	r1, r3
	str	r1, [r4, #124]
	b	.L258
.L274:
	cmp	lr, #3
	beq	.L278
	ldr	r3, [fp, #-52]
	ldr	r3, [r3, #376]
	cmp	r3, #0
	bne	.L248
	sub	lr, lr, #1
	clz	lr, lr
	mov	lr, lr, lsr #5
	mov	lr, lr, asl #12
	b	.L246
.L275:
	tst	r0, #131072
	movne	lr, #0
	moveq	lr, #262144
	b	.L252
.L248:
	subs	lr, lr, #1
	movne	lr, #1
	mov	lr, lr, asl #12
	b	.L246
.L278:
	ldrb	lr, [r5, #10]	@ zero_extendqisi2
	and	lr, lr, #1
	mov	lr, lr, asl #12
	b	.L246
.L276:
	ldr	ip, [r4, #140]
	add	r1, r1, #15
	ldr	lr, [r4, #44]
	add	r9, r4, #512
	mov	r1, r1, lsr #4
	mov	r3, ip, asl #4
	add	r10, lr, ip
	mul	r1, r1, r3
	add	r8, r1, lr
	str	r8, [r4, #48]
	ldrd	r2, [r9, #-8]
	add	ip, r8, ip
	mov	r0, r8
	adds	r6, r2, r1
	adc	r7, r3, r1, asr #31
	strd	r6, [r9]
	str	r8, [r4, #32]
	add	r8, r4, #528
	str	lr, [r4, #28]
	str	ip, [r4, #40]
	str	r10, [r4, #36]
	strd	r2, [r8, #-8]
	str	lr, [r4, #60]
	bl	MEM_Phy2Vir
	ldr	r2, [r4, #32]
	mov	r3, #0
	ldr	r1, [r4, #116]
	str	r3, [r4, #532]
	str	r2, [r4, #64]
	ldr	r3, [r4, #112]
	str	r0, [r4, #528]
	b	.L256
.L280:
	.align	2
.L279:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MP2_SetImgFormat, .-MP2_SetImgFormat
	.align	2
	.global	MP2_SearchStartCode
	.type	MP2_SearchStartCode, %function
MP2_SearchStartCode:
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
	cmp	r1, #4
	ble	.L282
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MP2_SearchStartCode.part.1
.L282:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_SearchStartCode, .-MP2_SearchStartCode
	.align	2
	.global	MP2_ProtocolPPSCheck
	.type	MP2_ProtocolPPSCheck, %function
MP2_ProtocolPPSCheck:
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
	add	r4, r0, #86016
	ldr	r3, [r4, #400]
	ldr	r1, [r4, #1688]
	cmp	r3, #1
	beq	.L285
	cmp	r3, #2
	beq	.L286
	ldr	r3, [r4, #364]
	orr	r3, r3, #63
	str	r3, [r4, #364]
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
.L292:
	tst	r3, #24
	beq	.L294
.L310:
	cmp	r2, #3
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L295:
	orr	r3, r3, #64
	ldr	r1, [r4, #444]
	str	r3, [r4, #364]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L298
	ldr	r1, [r5, #560]
	add	r6, r5, #576
	ldr	r0, [r4, #2048]
	add	r7, r5, #592
	bl	FreeUsdByDec
	ldr	r1, [r5, #568]
	ldr	r0, [r4, #2048]
	add	r8, r5, #560
	bl	FreeUsdByDec
	ldr	r1, [r5, #576]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	ldr	r1, [r5, #584]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r8]
	strd	r0, [r6, #-8]
	strd	r0, [r6]
	strd	r0, [r7, #-8]
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2048]
	str	r3, [r4, #440]
	str	r3, [r4, #444]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L309
.L298:
	mov	r3, #0
	str	r3, [r4, #404]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L286:
	ldr	r3, [r4, #364]
	tst	r3, #1048576
	orreq	r3, r3, #16
	streq	r3, [r4, #364]
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	sub	r0, r2, #1
	cmp	r0, #2
	bls	.L292
.L307:
	orr	r3, r3, #8
	str	r3, [r4, #364]
	tst	r3, #24
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	bne	.L310
.L294:
	cmp	r2, #1
	biceq	r3, r3, #192
	streq	r3, [r4, #364]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L285:
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	sub	r3, r2, #1
	cmp	r3, #3
	ldr	r3, [r4, #364]
	orrhi	r3, r3, #8
	strhi	r3, [r4, #364]
	ldrhib	r2, [r1, #22]	@ zero_extendqisi2
	sub	r0, r2, #2
	cmp	r0, #1
	bls	.L311
.L289:
	cmp	r2, #3
	beq	.L312
	tst	r3, #24
	bne	.L295
	b	.L294
.L309:
	ldr	r0, [r4, #2048]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #440]
	str	r0, [r4, #444]
	b	.L298
.L312:
	ldrb	r2, [r1, #26]	@ zero_extendqisi2
	cmp	r2, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	b	.L307
.L311:
	ldrb	r0, [r1, #24]	@ zero_extendqisi2
	cmp	r0, #0
	orreq	r3, r3, #8
	streq	r3, [r4, #364]
	ldreqb	r2, [r1, #22]	@ zero_extendqisi2
	b	.L289
	UNWIND(.fnend)
	.size	MP2_ProtocolPPSCheck, .-MP2_ProtocolPPSCheck
	.align	2
	.global	MP2_GetImageBuffer
	.type	MP2_GetImageBuffer, %function
MP2_GetImageBuffer:
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
	add	r4, r0, #86016
	mov	r5, r0
	mov	r1, #1
	ldr	r0, [r4, #2048]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r4, #448]
	blt	.L326
	mov	r1, r0
	ldr	r0, [r4, #2048]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L327
	ldr	r2, [r4, #448]
	mov	r0, #18
	ldr	r1, .L329
	bl	dprint_vfmw
	ldr	r2, [r5, #640]
	cmp	r2, #0
	beq	.L322
	ldr	r3, [r5, #644]
	cmp	r3, #0
	beq	.L322
	ldr	r1, [r5, #648]
	cmp	r1, #0
	beq	.L322
	ldr	ip, [r1, #16]
	mov	r0, #18
	ldr	r3, [r3, #16]
	ldr	r2, [r2, #16]
	ldr	r1, .L329+4
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r0, #1
.L318:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L322:
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L326:
	ldr	r1, .L329+8
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L316
	ldr	r1, [r6, #560]
	add	r7, r6, #576
	ldr	r0, [r4, #2048]
	add	r8, r6, #592
	bl	FreeUsdByDec
	ldr	r1, [r6, #568]
	ldr	r0, [r4, #2048]
	add	r9, r6, #560
	bl	FreeUsdByDec
	ldr	r1, [r6, #576]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	ldr	r1, [r6, #584]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r9]
	strd	r0, [r7, #-8]
	strd	r0, [r7]
	strd	r0, [r8, #-8]
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2048]
	str	r3, [r4, #440]
	str	r3, [r4, #444]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L328
.L316:
	add	r1, r5, #87040
	ldr	r0, [r4, #2048]
	mov	r5, #0
	add	r1, r1, #672
	str	r5, [r4, #404]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L328:
	ldr	r0, [r4, #2048]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #440]
	str	r0, [r4, #444]
	b	.L316
.L327:
	movw	r2, #1903
	ldr	r1, .L329+12
	bl	dprint_vfmw
	mov	r0, r5
	b	.L318
.L330:
	.align	2
.L329:
	.word	.LC24
	.word	.LC25
	.word	.LC22
	.word	.LC23
	UNWIND(.fnend)
	.size	MP2_GetImageBuffer, .-MP2_GetImageBuffer
	.align	2
	.global	MP2_GetPictureInfo
	.type	MP2_GetPictureInfo, %function
MP2_GetPictureInfo:
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
	add	r4, r0, #86016
	mov	r5, r0
	ldr	r3, [r4, #400]
	ldr	r6, [r4, #1688]
	cmp	r3, #1
	ldr	r8, [r4, #1692]
	beq	.L390
.L332:
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L333
	ldr	r2, [r4, #376]
	ldr	r1, [r4, #408]
	clz	r3, r2
	mov	r3, r3, lsr #5
	str	r3, [r4, #376]
	ldr	r2, [r6, #156]
	cmp	r2, r1
	moveq	r3, #0
	andne	r3, r3, #1
	cmp	r3, #0
	bne	.L391
.L386:
	add	r7, r5, #45056
.L335:
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	strb	r3, [r7, #305]
	ldr	r3, [r6, #156]
	str	r3, [r4, #408]
	ldr	r3, [r6, #56]
	strh	r3, [r5, #160]	@ movhi
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	cmp	r3, #3
	ldr	r3, [r6, #60]
	ldreq	r2, [r6, #64]
	movne	r3, r3, lsr #1
	rsbeq	r3, r2, r3
	strh	r3, [r5, #156]	@ movhi
	ldr	r3, [r6, #60]
	strh	r3, [r5, #158]	@ movhi
	ldrb	r3, [r6, #11]	@ zero_extendqisi2
	strb	r3, [r5, #13]
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	strb	r3, [r5, #11]
	ldr	r2, [r4, #376]
	strb	r2, [r5, #12]
	ldrb	r3, [r6, #12]	@ zero_extendqisi2
	strb	r3, [r5, #15]
	ldrb	r3, [r6, #22]	@ zero_extendqisi2
	strb	r3, [r5, #8]
	ldr	r3, [r4, #400]
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r5, #188]
	cmp	r3, #0
	ldrneb	r3, [r6, #25]	@ zero_extendqisi2
	strneb	r3, [r5, #9]
	ldrneb	r3, [r6, #23]	@ zero_extendqisi2
	strneb	r3, [r5, #10]
	ldrb	r3, [r6, #4]	@ zero_extendqisi2
	strb	r3, [r5, #20]
	ldrb	r3, [r6, #5]	@ zero_extendqisi2
	strb	r3, [r5, #21]
	ldrb	r3, [r6, #6]	@ zero_extendqisi2
	strb	r3, [r5, #22]
	ldrb	r3, [r6, #7]	@ zero_extendqisi2
	strb	r3, [r5, #23]
	ldrb	r3, [r6, #10]	@ zero_extendqisi2
	strb	r3, [r5, #14]
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	strb	r3, [r5, #19]
	ldrb	r3, [r6, #13]	@ zero_extendqisi2
	strb	r3, [r5, #18]
	ldrb	r3, [r6, #14]	@ zero_extendqisi2
	strb	r3, [r5, #17]
	ldrb	r3, [r6, #15]	@ zero_extendqisi2
	strb	r3, [r5, #16]
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	cmp	r2, #0
	cmpne	r3, #3
	bne	.L344
	mov	r1, #0
	mov	r0, r5
	bl	MP2_GetImageBuffer
	subs	r9, r0, #0
	beq	.L392
	ldr	r1, [r4, #448]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L347
	ldr	r9, [r5]
	add	ip, r8, #608
	add	lr, r8, #624
	ldr	r1, .L395
	mov	r0, #29
	ldrd	r2, [r9, #56]
	strd	r2, [ip, #-8]
	ldr	r9, [r5]
	ldrd	r2, [r9, #72]
	strd	r2, [ip]
	ldr	r9, [r5]
	ldrd	r2, [r9, #88]
	strd	r2, [lr, #-8]
	mvn	r2, #0
	ldr	lr, [r5]
	mvn	r3, #0
	ldr	lr, [lr, #104]
	str	lr, [r8, #8]
	ldr	lr, [r5]
	ldr	lr, [lr, #108]
	str	lr, [r8, #12]
	ldr	lr, [r5]
	ldr	lr, [lr, #112]
	str	lr, [r8, #16]
	ldr	lr, [r5]
	ldr	lr, [lr, #128]
	str	lr, [r8, #20]
	ldr	lr, [r5]
	strd	r2, [lr, #56]
	ldrd	r2, [ip]
	strd	r2, [fp, #-60]
	ldrd	r2, [ip, #-8]
	strd	r2, [fp, #-52]
	ldrd	r2, [fp, #-60]
	strd	r2, [sp]
	ldrd	r2, [fp, #-52]
	bl	dprint_vfmw
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	subs	r3, r3, #3
	movne	r3, #1
	str	r3, [r8, #96]
.L347:
	ldrb	r3, [r6, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L344
	ldr	r3, [r4, #404]
	ldr	r2, [r4, #440]
	cmp	r3, #0
	ldr	r3, [r4, #444]
	movne	r1, #2
	moveq	r1, #1
	cmp	r2, r3
	str	r1, [r4, #404]
	beq	.L350
	ldr	r1, .L395+4
	mov	r0, #13
	bl	dprint_vfmw
	mov	r2, #0
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_SetRef
	ldr	r3, [r4, #444]
.L350:
	ldr	r1, [r4, #448]
	mov	r2, #1
	str	r3, [r4, #440]
	ldr	r0, [r4, #2048]
	str	r1, [r4, #444]
	bl	FSP_SetRef
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_RemovePmv
	ldr	r2, [r4, #448]
	ldr	r1, .L395+8
	mov	r0, #13
	bl	dprint_vfmw
.L344:
	ldr	r1, [r4, #448]
	ldr	r0, [r4, #2048]
	bl	FSP_GetLogicFs
	ldr	r1, [r4, #440]
	mov	r6, r0
	ldr	r0, [r4, #2048]
	bl	FSP_GetLogicFs
	ldr	r1, [r4, #444]
	mov	r8, r0
	ldr	r0, [r4, #2048]
	bl	FSP_GetLogicFs
	cmp	r6, #0
	cmpne	r8, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	mov	r10, r0
	cmp	r3, #0
	bne	.L393
	ldr	r2, [r6, #640]
	cmp	r2, #0
	beq	.L352
	ldr	r3, [r6, #648]
	cmp	r3, #0
	beq	.L352
	ldr	r3, [r6, #644]
	cmp	r3, #0
	beq	.L352
	ldr	r3, [r0, #640]
	ldr	r1, .L395+12
	cmp	r3, #0
	ldrne	lr, [r3, #16]
	ldreq	lr, [r2, #16]
	str	lr, [r5, #200]
	ldr	r3, [r8, #640]
	cmp	r3, #0
	ldreq	r3, [r6, #640]
	ldr	ip, [r3, #16]
	str	ip, [r5, #204]
	ldr	r3, [r6, #640]
	ldr	r2, [r3, #16]
	str	r2, [r5, #208]
	ldr	r9, [r6, #652]
	str	r9, [r5, #268]
	ldr	r3, [r6, #640]
	ldr	r3, [r3, #36]
	str	r3, [r5, #240]
	ldr	r3, [r6, #640]
	ldr	r3, [r3, #40]
	str	r3, [r5, #244]
	ldr	r3, [r0, #648]
	mov	r0, #13
	cmp	r3, #0
	ldreq	r3, [r6, #648]
	ldr	r3, [r3, #16]
	str	r3, [r5, #220]
	ldr	r3, [r8, #648]
	cmp	r3, #0
	ldreq	r3, [r6, #648]
	ldr	r3, [r3, #16]
	str	r3, [r5, #224]
	ldr	r3, [r6, #648]
	ldr	r3, [r3, #16]
	str	r3, [r5, #228]
	ldr	r3, [r6, #644]
	ldr	r3, [r3, #16]
	str	r3, [r5, #232]
	ldr	r10, [r10, #104]
	str	r10, [r5, #212]
	ldr	r8, [r8, #104]
	str	r8, [r5, #216]
	ldr	r8, [r6, #648]
	ldr	r8, [r8, #16]
	str	r9, [sp, #8]
	str	lr, [sp, #4]
	str	r8, [r7, #284]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r2, [r6, #644]
	add	r3, r4, #636
	add	ip, r5, #87
	add	r4, r4, #892
	add	r1, r5, #23
	ldr	r0, [r2, #32]
	mov	r2, #0
	str	r0, [r5, #248]
	ldr	r0, [r6, #644]
	ldr	r0, [r0, #76]
	str	r0, [r5, #252]
	ldr	r0, [r6, #644]
	ldr	r0, [r0, #60]
	str	r0, [r5, #256]
	ldr	r0, [r6, #644]
	ldr	r0, [r0, #64]
	str	r0, [r5, #260]
.L362:
	ldr	r0, [r3, #4]!
	add	r2, r2, #1
	cmp	r2, #64
	strb	r0, [ip, #1]!
	ldr	r0, [r4, #4]!
	strb	r0, [r1, #1]!
	bne	.L362
	mov	r0, r5
	mov	r9, #1
	bl	MP2_EstimatePicQp
	strb	r0, [r5, #152]
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L392:
	ldr	r2, [r4, #372]
	ldr	r1, .L395+16
	bl	dprint_vfmw
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L333:
	add	r7, r5, #45056
	ldrb	r1, [r7, #305]	@ zero_extendqisi2
	cmp	r1, #3
	beq	.L338
	ldr	r3, [r4, #376]
	cmp	r3, #0
	beq	.L394
.L338:
	mov	r3, #1
	str	r3, [r4, #376]
	b	.L335
.L390:
	ldrb	r2, [r6, #26]	@ zero_extendqisi2
	ldrb	r3, [r6, #24]	@ zero_extendqisi2
	strb	r2, [r6, #7]
	strb	r2, [r6, #6]
	strb	r3, [r6, #5]
	strb	r3, [r6, #4]
	b	.L332
.L393:
	ldr	r1, [r4, #448]
	mov	r9, #0
	ldr	r0, [r4, #2048]
	mov	r2, #1
	bl	FSP_ClearLogicFs
	stmia	sp, {r8, r10}
	mov	r3, r6
	movw	r2, #1183
	ldr	r1, .L395+20
	mov	r0, r9
	bl	dprint_vfmw
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L394:
	ldr	r2, [r4, #372]
	mov	r0, #1
	str	r1, [sp]
	ldr	r1, .L395+24
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r4, #448]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	ldrb	r3, [r8, #22]	@ zero_extendqisi2
	cmp	r3, #3
	ldrne	r3, [r4, #364]
	orrne	r3, r3, #192
	strne	r3, [r4, #364]
	b	.L338
.L352:
	ldr	r1, [r4, #448]
	mov	r2, #1
	ldr	r0, [r4, #2048]
	mov	r9, #0
	bl	FSP_ClearLogicFs
	ldr	r1, [r6, #644]
	ldr	r3, [r4, #448]
	mov	r0, r9
	ldr	r2, [r4, #440]
	str	r1, [sp, #12]
	ldr	ip, [r6, #648]
	ldr	lr, [r4, #444]
	ldr	r1, .L395+28
	str	ip, [sp, #8]
	ldr	ip, [r6, #640]
	str	lr, [sp]
	str	ip, [sp, #4]
	bl	dprint_vfmw
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L391:
	ldr	r2, [r4, #372]
	mov	r0, #1
	ldr	r1, .L395+32
	bl	dprint_vfmw
	mov	r3, #0
	mov	r2, #1
	str	r3, [r4, #376]
	ldr	r1, [r4, #448]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	ldrb	r3, [r8, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L386
	ldr	r3, [r4, #364]
	add	r7, r5, #45056
	orr	r3, r3, #192
	str	r3, [r4, #364]
	b	.L335
.L396:
	.align	2
.L395:
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC34
	.word	.LC28
	.word	.LC32
	.word	.LC27
	.word	.LC33
	.word	.LC26
	UNWIND(.fnend)
	.size	MP2_GetPictureInfo, .-MP2_GetPictureInfo
	.align	2
	.global	MP2_SetVDMInf
	.type	MP2_SetVDMInf, %function
MP2_SetVDMInf:
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
	add	r4, r0, #86016
	mov	r5, r0
	add	r0, r4, #608
	ldrb	r3, [r4, #316]	@ zero_extendqisi2
	ldr	r2, [r4, #320]
	cmp	r3, #1
	ldr	r3, [r4, #632]
	beq	.L400
	ldr	ip, [r4, #312]
	mov	r1, r3, lsr #3
	add	r3, r3, r2, lsl #3
	add	r2, r1, r2
	ldr	r6, [r4, #324]
	and	r3, r3, #31
	bic	r2, r2, #3
	add	r1, r1, ip
	str	r1, [r4, #312]
	bic	r6, r6, #3
	str	r3, [r5, #172]
	str	r2, [r5, #164]
	bl	BsResidBits
	str	r6, [r5, #168]
	str	r0, [r5, #180]
	ldr	r3, [r4, #324]
	and	r3, r3, #3
	mov	r3, r3, asl #3
	str	r3, [r5, #176]
	ldr	r3, [r4, #332]
	mov	r3, r3, asl #3
	str	r3, [r5, #184]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L400:
	ldr	r1, [r4, #312]
	mov	ip, r3, lsr #3
	add	r3, r3, r2, lsl #3
	add	r2, ip, r2
	and	r3, r3, #31
	bic	r2, r2, #3
	add	r1, r1, ip
	str	r1, [r4, #312]
	str	r3, [r5, #172]
	str	r2, [r5, #164]
	bl	BsResidBits
	mov	r3, #0
	str	r3, [r5, #168]
	str	r3, [r5, #176]
	str	r3, [r5, #184]
	str	r0, [r5, #180]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_SetVDMInf, .-MP2_SetVDMInf
	.align	2
	.global	MP2_DecNewPic
	.type	MP2_DecNewPic, %function
MP2_DecNewPic:
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
	mov	r4, r0
	bl	MP2_SetVDMInf
	ldr	r3, [r4]
	add	r2, r4, #45056
	add	r1, r4, #86016
	ldr	r0, [r3, #592]
	str	r0, [r2, #288]
	ldr	r0, [r3, #632]
	str	r0, [r2, #292]
	ldr	r0, [r3, #596]
	str	r0, [r2, #296]
	ldr	r2, [r1, #1688]
	ldrb	r0, [r2, #22]	@ zero_extendqisi2
	cmp	r0, #3
	beq	.L419
	cmp	r0, #2
	moveq	r0, #1
	streq	r0, [r1, #2060]
.L403:
	add	ip, r4, #8
	str	ip, [r4, #4]
	ldr	r0, [r3, #864]
	cmp	r0, #1
	beq	.L420
	cmp	r0, #2
	beq	.L421
.L406:
	ldr	r3, [r1, #364]
	bic	r3, r3, #16252928
	str	r3, [r1, #364]
	ldrb	r5, [r2, #22]	@ zero_extendqisi2
	cmp	r5, #1
	movne	r0, #1
	beq	.L422
.L404:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L422:
	ldr	r3, .L423
	ldr	r6, [r3]
	cmp	r6, #0
	moveq	r0, r5
	beq	.L404
	ldr	lr, [r4, #184]
	sub	r2, fp, #28
	ldr	ip, [r4, #180]
	mov	r3, #4
	ldr	r0, [r1, #2048]
	mov	r1, #20
	add	ip, ip, lr
	add	lr, ip, #7
	cmp	ip, #0
	movlt	ip, lr
	mov	ip, ip, asr #3
	str	ip, [r2, #-4]!
	blx	r6
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L419:
	ldr	r0, [r1, #2060]
	cmp	r0, #0
	bne	.L403
	ldr	ip, [r1, #404]
	cmp	ip, #2
	beq	.L403
	ldrb	r3, [r2, #9]	@ zero_extendqisi2
	cmp	r3, #3
	ldrne	r3, [r1, #376]
	clzne	r3, r3
	movne	r3, r3, lsr #5
	strne	r3, [r1, #376]
	b	.L404
.L420:
	cmp	ip, #0
	beq	.L406
	ldrb	r3, [r2, #22]	@ zero_extendqisi2
	cmp	r3, #3
	moveq	r0, #0
	streq	r0, [r4, #4]
	bne	.L406
	b	.L404
.L421:
	ldr	r0, [r3, #868]
	cmp	ip, #0
	cmpne	r0, #0
	ble	.L406
	ldrb	r0, [r2, #22]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L406
	mov	r0, #0
	str	r0, [r4, #4]
	ldr	r2, [r3, #868]
	sub	r2, r2, #1
	str	r2, [r3, #868]
	b	.L404
.L424:
	.align	2
.L423:
	.word	g_event_report
	UNWIND(.fnend)
	.size	MP2_DecNewPic, .-MP2_DecNewPic
	.align	2
	.global	MP2_RecordVFMWState
	.type	MP2_RecordVFMWState, %function
MP2_RecordVFMWState:
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
	add	r2, r0, #86016
	ldr	r1, [r0]
	ldr	r3, [r2, #1688]
	ldr	ip, [r3, #40]
	str	ip, [r1, #568]
	ldr	r1, [r3, #44]
	ldr	r3, [r0]
	str	r1, [r3, #572]
	ldr	r3, [r2, #2052]
	ldr	r2, [r2, #2056]
	ldr	r1, [r0]
	cmp	r3, r2
	rsbge	r3, r2, r3
	movlt	r3, #0
	str	r3, [r1, #576]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_RecordVFMWState, .-MP2_RecordVFMWState
	.align	2
	.global	MP2_GetUsrData
	.type	MP2_GetUsrData, %function
MP2_GetUsrData:
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
	add	r8, r0, #86016
	mov	r10, r0
	add	r5, r8, #608
	mov	r0, r5
	bl	BsToNextByte
	mov	r0, r5
	bl	BsResidBits
	add	r7, r0, #7
	cmp	r0, #0
	movlt	r0, r7
	mov	r7, r0, asr #3
	cmp	r7, #0
	ble	.L448
	mov	r6, #0
	ldr	r9, .L466
	mov	r4, r6
	b	.L433
.L464:
	bl	BsGet
	add	r4, r4, #1
	cmp	r4, r7
	strb	r0, [r9, r6]
	add	r6, r6, #1
	beq	.L429
.L433:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	mov	r1, #8
	bic	r3, r0, #-16777216
	mov	r0, r5
	cmp	r3, #1
	beq	.L430
	cmp	r6, #1024
	blt	.L464
	mov	r1, #8
	mov	r0, r5
	add	r4, r4, #1
	bl	BsGet
	cmp	r4, r7
	bne	.L433
.L429:
	add	r3, r8, #316
	ldrsb	r7, [r3]
	cmp	r7, #0
	beq	.L445
.L463:
	mov	r4, #0
.L434:
	ldr	r5, [r8, #396]
	cmp	r5, #3
	ble	.L447
.L446:
	add	r5, r8, #376
	add	r9, r8, #392
	mov	r7, #0
.L438:
	ldr	r1, [r5, #4]!
	ldr	r0, [r8, #2048]
	bl	FreeUsdByDec
	cmp	r5, r9
	str	r7, [r5]
	mov	r3, #0
	bne	.L438
	mov	r5, r3
	str	r3, [r8, #396]
.L447:
	ldr	r0, [r8, #2048]
	add	r5, r10, r5, lsl #2
	bl	GetUsd
	add	r5, r5, #86016
	str	r0, [r5, #380]
	ldr	r2, [r8, #396]
	add	r3, r10, r2, lsl #2
	add	r3, r3, #86016
	add	r3, r3, #72
	ldr	r3, [r3, #308]
	cmp	r3, #0
	beq	.L439
	ldr	r3, [r10]
	add	r0, r0, #1056
	ldr	r1, .L466
	ldrd	r2, [r3, #56]
	strd	r2, [r0]
	ldr	r3, [r5, #380]
	ldr	r2, [r8, #368]
	str	r2, [r3, #1044]
	ldr	r3, [r5, #380]
	str	r6, [r3, #1052]
	ldr	r2, [r8, #1688]
	ldr	r3, [r5, #380]
	ldrb	r2, [r2, #22]	@ zero_extendqisi2
	strb	r2, [r3, #1024]
	ldr	r3, [r5, #380]
	ldr	r2, [r8, #2052]
	str	r2, [r3, #1032]
	ldr	r2, [r8, #1688]
	ldr	r3, [r5, #380]
	ldrb	r2, [r2, #10]	@ zero_extendqisi2
	strb	r2, [r3, #1025]
	ldr	r3, [r8, #364]
	tst	r3, #524288
	ldr	r3, [r5, #380]
	movne	r2, #3
	moveq	r2, #1
	str	r2, [r3, #1040]
	ldr	r0, [r5, #380]
	ldr	r2, [r0, #1052]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r0, #1052]
	ldr	r3, .L466+4
	ldrgt	r0, [r5, #380]
	ldr	r3, [r3, #52]
	ldrgt	r2, [r0, #1052]
	blx	r3
	ldr	r3, .L466+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L443
	ldr	r2, [r5, #380]
	movw	r3, #1064
	mov	r1, #6
	ldr	r0, [r8, #2048]
	blx	ip
.L443:
	ldr	r3, [r8, #396]
	add	r3, r3, #1
	str	r3, [r8, #396]
.L462:
	mov	r0, r4
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L445:
	movw	r3, #317
	ldrsb	r3, [r8, r3]
	cmp	r3, #0
	beq	.L451
	ldr	r4, [r8, #332]
	cmp	r4, #0
	beq	.L434
	ldr	r1, .L466+12
	mov	r0, r7
	bl	dprint_vfmw
	ldr	ip, .L466+4
	ldr	r3, [r8, #328]
	mov	r2, #3
	ldr	r1, [r8, #344]
	mov	r4, #1
	ldr	r0, [r8, #348]
	sub	r3, r3, #3
	add	r1, r1, r3
	ldr	r3, [ip, #52]
	sub	r0, r0, #3
	blx	r3
	ldr	r3, [r8, #348]
	ldr	ip, [r8, #324]
	mov	r0, r5
	ldr	r2, [r8, #332]
	sub	r3, r3, #3
	strb	r7, [r8, #317]
	add	r2, r2, #3
	mov	r1, r3
	str	r2, [r8, #328]
	str	r3, [r8, #344]
	sub	r3, ip, #3
	str	r7, [r8, #332]
	str	r3, [r8, #320]
	str	r7, [r8, #348]
	str	r7, [r8, #324]
	strb	r4, [r8, #316]
	bl	BsInit
	strb	r4, [r8, #317]
	strb	r7, [r8, #316]
	mov	r0, r5
	bl	BsToNextByte
	mov	r0, r5
	bl	BsResidBits
	add	r9, r0, #7
	cmp	r0, #0
	movlt	r0, r9
	mov	r9, r0, asr #3
	cmp	r9, #3
	ble	.L450
	ldr	r4, .L466
	sub	r9, r9, #3
	b	.L437
.L465:
	bl	BsGet
	strb	r0, [r4, r6]
	add	r6, r6, #1
.L436:
	add	r7, r7, #1
	cmp	r7, r9
	beq	.L463
.L437:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	mov	r1, #8
	bic	r3, r0, #-16777216
	mov	r0, r5
	cmp	r3, #1
	beq	.L451
	cmp	r6, #1024
	blt	.L465
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	b	.L436
.L430:
	ldr	r5, [r8, #396]
	mov	r4, r3
	cmp	r5, #3
	bgt	.L446
	b	.L447
.L451:
	mov	r4, r3
	b	.L434
.L439:
	mov	r0, r3
	ldr	r1, .L466+16
	bl	dprint_vfmw
	b	.L462
.L448:
	mov	r6, #0
	b	.L429
.L450:
	mov	r4, r7
	b	.L434
.L467:
	.align	2
.L466:
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC35
	.word	.LC36
	UNWIND(.fnend)
	.size	MP2_GetUsrData, .-MP2_GetUsrData
	.align	2
	.global	next_start_code
	.type	next_start_code, %function
next_start_code:
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
	add	r7, r0, #86016
	add	r5, r7, #608
	mov	r0, r5
	bl	BsToNextByte
	mov	r0, r5
	bl	BsGetNextBytePtr
	mov	r4, r0
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	add	r3, r0, #7
	movge	r1, r0
	movlt	r1, r3
	mov	r1, r1, asr #3
	cmp	r1, #4
	movle	r1, #0
	bgt	.L493
.L469:
	mov	r0, r5
	bl	BsLongSkip
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	add	r6, r0, #7
	movge	r6, r0
	mov	r6, r6, asr #3
	cmp	r6, #3
	subgt	r6, r6, #3
	movgt	r4, #0
	bgt	.L475
	b	.L476
.L473:
	bl	BsSkip
	cmp	r4, r6
	beq	.L476
.L475:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	cmp	r0, #1
	mov	r0, r5
	bne	.L473
.L478:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L476:
	add	r3, r7, #316
	ldrsb	r4, [r3]
	cmp	r4, #0
	bne	.L472
	ldrb	r3, [r7, #317]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L472
	ldr	r3, [r7, #332]
	cmp	r3, #0
	beq	.L472
	ldr	r1, .L496
	mov	r0, #7
	bl	dprint_vfmw
	ldr	ip, .L496+4
	ldr	r3, [r7, #328]
	mov	r2, #3
	ldr	r1, [r7, #344]
	mov	r6, #1
	ldr	r0, [r7, #348]
	sub	r3, r3, #3
	add	r1, r1, r3
	ldr	r3, [ip, #52]
	sub	r0, r0, #3
	blx	r3
	ldr	r3, [r7, #348]
	ldr	ip, [r7, #324]
	mov	r0, r5
	ldr	r2, [r7, #332]
	sub	r3, r3, #3
	strb	r6, [r7, #316]
	mov	r1, r3
	add	r2, r2, #3
	str	r3, [r7, #344]
	sub	r3, ip, #3
	str	r2, [r7, #328]
	str	r3, [r7, #320]
	strb	r4, [r7, #317]
	str	r4, [r7, #332]
	str	r4, [r7, #348]
	str	r4, [r7, #324]
	bl	BsInit
	strb	r6, [r7, #317]
	strb	r4, [r7, #316]
	mov	r0, r5
	bl	BsToNextByte
	mov	r0, r5
	bl	BsGetNextBytePtr
	mov	r6, r0
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	add	r1, r0, #7
	movge	r3, r0
	movlt	r3, r1
	mov	r1, r3, asr #3
	cmp	r1, #4
	movle	r1, r4
	bgt	.L494
.L477:
	mov	r0, r5
	bl	BsLongSkip
	mov	r0, r5
	bl	BsResidBits
	add	r6, r0, #7
	cmp	r0, #0
	movlt	r0, r6
	mov	r6, r0, asr #3
	cmp	r6, #3
	ble	.L472
	sub	r6, r6, #3
	mov	r4, #0
	b	.L479
.L495:
	bl	BsSkip
	cmp	r4, r6
	beq	.L472
.L479:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	cmp	r0, #1
	mov	r0, r5
	bne	.L495
	b	.L478
.L472:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L493:
	mov	r0, r4
	bl	MP2_SearchStartCode.part.1
	mov	r1, r0, asl #3
	b	.L469
.L494:
	mov	r0, r6
	bl	MP2_SearchStartCode.part.1
	mov	r1, r0, asl #3
	b	.L477
.L497:
	.align	2
.L496:
	.word	.LC35
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	next_start_code, .-next_start_code
	.align	2
	.global	GetSliceHdr
	.type	GetSliceHdr, %function
GetSliceHdr:
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
	add	r7, r0, #86016
	mov	r6, r0
	add	r5, r7, #608
	b	.L499
.L506:
	bl	BsShow
	mov	r4, r0
	mov	r0, r5
	bl	BsPos
	sub	r3, r4, #256
	sub	r3, r3, #1
	mov	r1, #32
	cmp	r3, #174
	str	r0, [r7, #424]
	mov	r0, r5
	bls	.L512
	bl	BsSkip
.L499:
	mov	r0, r6
	bl	next_start_code
	mov	r1, #32
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r5
	beq	.L506
	cmn	r3, #1
	beq	.L501
.L507:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L501:
	ldr	r3, .L513
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L508
	mov	r3, #0
	ldr	r0, [r7, #2048]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L512:
	bl	BsResidBits
	cmp	r0, #40
	ble	.L501
	ldr	r3, [r7, #360]
	mov	r2, #0
	str	r2, [r7, #420]
	cmp	r3, r2
	bne	.L505
	mov	r0, r6
	bl	MP2_GetSequenceInfo
	ldr	r3, [r7, #412]
	cmp	r3, #0
	beq	.L507
.L505:
	mov	r0, r6
	bl	MP2_ProtocolPPSCheck
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L508:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L514:
	.align	2
.L513:
	.word	g_event_report
	UNWIND(.fnend)
	.size	GetSliceHdr, .-GetSliceHdr
	.align	2
	.global	sequence_header
	.type	sequence_header, %function
sequence_header:
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
	add	r4, r0, #86016
	mov	r2, #65536
	add	r6, r4, #608
	mov	r3, #0
	str	r2, [r4, #364]
	mov	r1, #12
	str	r3, [r4, #360]
	mov	r5, r0
	mov	r0, r6
	ldr	r8, [r4, #1688]
	bl	BsGet
	mov	r1, #12
	str	r0, [r8, #48]
	mov	r0, r6
	bl	BsGet
	mov	r1, #4
	str	r0, [r8, #52]
	mov	r0, r6
	bl	BsGet
	mov	r1, #4
	str	r0, [r8, #84]
	mov	r0, r6
	bl	BsGet
	mov	r1, #18
	str	r0, [r8, #88]
	mov	r0, r6
	bl	BsGet
	mov	r1, #1
	str	r0, [r8, #92]
	mov	r0, r6
	bl	BsGet
	cmp	r0, #0
	ldreq	r3, [r4, #364]
	orreq	r3, r3, #1
	streq	r3, [r4, #364]
.L516:
	mov	r1, #10
	mov	r0, r6
	bl	BsGet
	mov	r1, #1
	str	r0, [r8, #96]
	mov	r0, r6
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #1]
	mov	r0, r6
	bl	BsGet
	cmp	r0, #1
	str	r0, [r8, #76]
	beq	.L523
	ldr	r2, .L530
	add	r3, r4, #636
	add	r0, r4, #892
.L519:
	ldrb	r1, [r2, #1]!	@ zero_extendqisi2
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L519
.L518:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	cmp	r0, #1
	str	r0, [r8, #80]
	beq	.L524
	add	r5, r5, #87040
	add	r3, r4, #892
	add	r5, r5, #124
	mov	r2, #16
.L521:
	str	r2, [r3, #4]!
	cmp	r3, r5
	bne	.L521
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L523:
	ldr	r10, .L530+4
	add	r9, r10, #64
.L517:
	mov	r1, #8
	mov	r0, r6
	ldrb	r7, [r10, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r7, #21504
	cmp	r10, r9
	add	r3, r3, #160
	str	r0, [r5, r3, asl #2]
	bne	.L517
	b	.L518
.L524:
	ldr	r7, .L530+4
	add	r8, r7, #64
.L520:
	mov	r1, #8
	mov	r0, r6
	ldrb	r4, [r7, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r4, #21504
	cmp	r7, r8
	add	r3, r3, #224
	str	r0, [r5, r3, asl #2]
	bne	.L520
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L531:
	.align	2
.L530:
	.word	.LANCHOR2-1
	.word	.LANCHOR2+63
	UNWIND(.fnend)
	.size	sequence_header, .-sequence_header
	.align	2
	.global	group_of_pictures_header
	.type	group_of_pictures_header, %function
group_of_pictures_header:
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
	add	r4, r0, #86016
	mov	r1, #1
	add	r5, r4, #608
	add	r7, r4, #484
	ldr	r6, [r4, #1688]
	mov	r0, r5
	bl	BsGet
	mov	r1, #5
	strb	r0, [r6, #2]
	mov	r0, r5
	bl	BsGet
	mov	r1, #6
	str	r0, [r6, #100]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #104]
	mov	r0, r5
	bl	BsSkip
	mov	r0, r5
	mov	r1, #6
	bl	BsGet
	mov	r1, #6
	str	r0, [r6, #108]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #112]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #116]
	mov	r0, r5
	bl	BsGet
	add	ip, r4, #496
	add	r3, r4, #508
	add	r5, r4, #520
	mov	lr, #0
	str	r0, [r6, #120]
	ldmia	ip, {r0, r1, r2}
	stmia	r7, {r0, r1, r2}
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	ldmia	r5, {r0, r1, r2}
	stmia	r3, {r0, r1, r2}
	str	lr, [r4, #520]
	str	lr, [r4, #528]
	str	lr, [r4, #524]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	group_of_pictures_header, .-group_of_pictures_header
	.align	2
	.global	sequence_extension
	.type	sequence_extension, %function
sequence_extension:
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
	add	r6, r0, #86016
	mov	r2, #0
	add	r5, r6, #608
	mov	r1, #8
	ldr	r3, [r6, #364]
	str	r2, [r6, #360]
	mov	r0, r5
	orr	r3, r3, #131072
	str	r3, [r6, #364]
	ldr	r4, [r6, #1688]
	bl	BsGet
	mov	r1, #1
	str	r0, [r4, #124]
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #27]
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #28]
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	mov	r10, r0
	mov	r0, r5
	bl	BsGet
	mov	r1, #12
	mov	r8, r0
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	mov	r7, r0
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	bne	.L534
	ldr	r3, [r6, #364]
	orr	r3, r3, #2
	str	r3, [r6, #364]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L534:
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	mov	r9, r0
	mov	r0, r5
	bl	BsGet
	mov	r1, #2
	strb	r0, [r4, #29]
	mov	r0, r5
	bl	BsGet
	mov	r1, #5
	strb	r0, [r4, #30]
	mov	r0, r5
	bl	BsGet
	ldrb	r3, [r4, #30]	@ zero_extendqisi2
	uxtb	r1, r0
	strb	r0, [r4, #31]
	add	r1, r1, #1
	add	r0, r3, #1
	bl	__aeabi_idiv
	ldr	r1, [r4, #88]
	ldr	r2, .L558
	ldr	r3, [r4, #124]
	add	r2, r2, r1, lsl #2
	tst	r3, #128
	ldr	r2, [r2, #160]
	mul	r0, r2, r0
	str	r0, [r4, #72]
	bne	.L557
	ubfx	r2, r3, #4, #3
	and	r3, r3, #15
	str	r2, [r4, #40]
	str	r3, [r4, #44]
.L537:
	ldr	r3, [r4, #48]
	ldr	r2, [r4, #52]
	ubfx	r3, r3, #0, #12
	ubfx	r2, r2, #0, #12
	orr	r3, r3, r10, asl #12
	orr	r8, r2, r8, asl #12
	str	r3, [r4, #48]
	cmp	r8, #0
	cmpne	r3, #0
	str	r8, [r4, #52]
	ldreq	r3, [r6, #364]
	orreq	r3, r3, #1
	streq	r3, [r6, #364]
	ldreq	r3, [r4, #48]
	cmp	r3, #8192
	bhi	.L539
	ldr	r3, [r4, #52]
	cmp	r3, #8192
	bls	.L540
.L539:
	ldr	r2, .L558+4
	ldr	r3, [r6, #364]
	ldr	ip, [r2]
	orr	r3, r3, #1
	str	r3, [r6, #364]
	cmp	ip, #0
	beq	.L540
	mov	r3, #0
	mov	r1, #102
	mov	r2, r3
	ldr	r0, [r6, #2048]
	blx	ip
.L540:
	ldr	r3, [r4, #92]
	mov	r1, #400
	ldr	r2, [r4, #40]
	add	r7, r3, r7, lsl #18
	str	r7, [r4, #92]
	sub	r0, r2, #4
	ldr	r3, [r4, #96]
	mul	r7, r1, r7
	cmp	r0, #1
	add	r9, r3, r9, lsl #10
	str	r9, [r4, #96]
	str	r7, [r4, #68]
	bls	.L542
	ldr	r3, .L558+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L543
	mov	r3, #0
	mov	r1, #102
	mov	r2, r3
	ldr	r0, [r6, #2048]
	blx	ip
	ldr	r2, [r4, #40]
.L543:
	ldr	r1, .L558+8
	mov	r0, #1
	bl	dprint_vfmw
.L542:
	ldr	r2, [r4, #44]
	sub	r3, r2, #4
	cmp	r3, #6
	ldmlsfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L558+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L544
	mov	r3, #0
	ldr	r0, [r6, #2048]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	ldr	r2, [r4, #44]
.L544:
	ldr	r1, .L558+12
	mov	r0, #1
	sub	sp, fp, #44
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L557:
	and	r3, r3, #15
	cmp	r3, #5
	moveq	r2, #133
	moveq	r3, #8
	streq	r2, [r4, #40]
	streq	r3, [r4, #44]
	b	.L537
.L559:
	.align	2
.L558:
	.word	.LANCHOR0
	.word	g_event_report
	.word	.LC37
	.word	.LC38
	UNWIND(.fnend)
	.size	sequence_extension, .-sequence_extension
	.align	2
	.global	sequence_display_extension
	.type	sequence_display_extension, %function
sequence_display_extension:
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
	add	r5, r0, #86016
	mov	r1, #3
	add	r4, r5, #608
	ldr	r3, [r5, #364]
	mov	r0, r4
	ldr	r6, [r5, #1688]
	orr	r3, r3, #262144
	str	r3, [r5, #364]
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #128]
	mov	r0, r4
	bl	BsGet
	cmp	r0, #0
	str	r0, [r6, #132]
	bne	.L567
.L561:
	mov	r1, #14
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	str	r0, [r6, #148]
	mov	r0, r4
	bl	BsGet
	cmp	r0, #0
	bne	.L562
	ldr	r3, [r5, #364]
	orr	r3, r3, #4
	str	r3, [r5, #364]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L562:
	mov	r0, r4
	mov	r1, #14
	bl	BsGet
	str	r0, [r6, #152]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L567:
	mov	r1, #8
	mov	r0, r4
	bl	BsGet
	mov	r1, #8
	str	r0, [r6, #136]
	mov	r0, r4
	bl	BsGet
	mov	r1, #8
	str	r0, [r6, #140]
	mov	r0, r4
	bl	BsGet
	str	r0, [r6, #144]
	b	.L561
	UNWIND(.fnend)
	.size	sequence_display_extension, .-sequence_display_extension
	.align	2
	.global	quant_matrix_extension
	.type	quant_matrix_extension, %function
quant_matrix_extension:
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
	add	r3, r0, #86016
	mov	r7, r0
	add	r5, r3, #608
	mov	r1, #1
	ldr	r9, [r3, #1688]
	mov	r0, r5
	bl	BsGet
	cmp	r0, #1
	str	r0, [r9, #76]
	beq	.L593
.L569:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #1
	str	r0, [r9, #80]
	ldreq	r6, .L595
	addeq	r8, r6, #64
	beq	.L571
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	beq	.L573
.L579:
	mov	r4, #64
.L574:
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	subs	r4, r4, #1
	bne	.L574
.L573:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	beq	.L594
.L575:
	mov	r4, #64
.L576:
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	subs	r4, r4, #1
	bne	.L576
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L571:
	mov	r1, #8
	mov	r0, r5
	ldrb	r4, [r6, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r4, #21504
	cmp	r6, r8
	add	r3, r3, #224
	str	r0, [r7, r3, asl #2]
	bne	.L571
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	bne	.L579
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	bne	.L575
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L594:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L593:
	ldr	r6, .L595
	add	r8, r6, #64
.L570:
	mov	r1, #8
	mov	r0, r5
	ldrb	r4, [r6, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r4, #21504
	cmp	r6, r8
	add	r3, r3, #160
	str	r0, [r7, r3, asl #2]
	bne	.L570
	b	.L569
.L596:
	.align	2
.L595:
	.word	.LANCHOR2+63
	UNWIND(.fnend)
	.size	quant_matrix_extension, .-quant_matrix_extension
	.align	2
	.global	sequence_scalable_extension
	.type	sequence_scalable_extension, %function
sequence_scalable_extension:
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
	add	r0, r0, #86016
	mov	r1, #32
	add	r0, r0, #608
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	BsSkip
	UNWIND(.fnend)
	.size	sequence_scalable_extension, .-sequence_scalable_extension
	.align	2
	.global	picture_display_extension
	.type	picture_display_extension, %function
picture_display_extension:
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
	add	r8, r0, #86016
	add	r4, r8, #608
	ldr	r5, [r8, #1688]
	ldr	r3, [r8, #364]
	orr	r3, r3, #2097152
	str	r3, [r8, #364]
	ldrb	r3, [r5, #27]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L599
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L611
.L607:
	mov	r7, #1
.L600:
	add	r5, r5, #160
	mov	r6, #0
	b	.L604
.L613:
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #12]
	mov	r0, r4
	bl	BsGet
	cmp	r0, #0
	beq	.L610
	cmp	r6, r7
	bge	.L612
.L604:
	mov	r1, #16
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	add	r6, r6, r1
	str	r0, [r5, #4]!
	mov	r0, r4
	bl	BsGet
	mov	r1, #16
	cmp	r0, #0
	mov	r0, r4
	bne	.L613
.L610:
	ldr	r3, [r8, #364]
	orr	r3, r3, #32
	str	r3, [r8, #364]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L599:
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L607
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r7, #2
	movne	r7, #3
	b	.L600
.L611:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r7, #2
	movne	r7, #3
	b	.L600
.L612:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	picture_display_extension, .-picture_display_extension
	.align	2
	.global	picture_coding_extension
	.type	picture_coding_extension, %function
picture_coding_extension:
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
	add	r6, r0, #86016
	add	r5, r6, #608
	ldr	r8, [r6, #1688]
	ldr	r3, [r6, #364]
	add	r10, r8, #4
	add	r9, r8, #8
	orr	r3, r3, #1048576
	str	r3, [r6, #364]
.L615:
	mov	r7, r10
	mov	r4, #0
.L617:
	mov	r1, #4
	mov	r0, r5
	bl	BsGet
	add	r4, r4, #1
	uxtb	r0, r0
	strb	r0, [r7], #1
	sub	r2, r0, #10
	cmp	r0, #0
	cmpne	r2, #4
	ldrls	r3, [r6, #364]
	orrls	r3, r3, #16
	strls	r3, [r6, #364]
	cmp	r4, #2
	bne	.L617
	add	r10, r10, #2
	cmp	r10, r9
	bne	.L615
	mov	r1, r4
	mov	r0, r5
	bl	BsGet
	mov	r1, r4
	strb	r0, [r8, #8]
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r8, #9]
	cmp	r0, #0
	bne	.L619
	ldr	r3, [r6, #364]
	orr	r3, r3, #16
	str	r3, [r6, #364]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L619:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #10]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #11]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #12]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #13]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #14]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #15]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #16]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #17]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #18]
	mov	r0, r5
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r8, #3]
	cmp	r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, #3
	strb	r0, [r8, #19]
	mov	r0, r5
	bl	BsGet
	mov	r1, #1
	strb	r0, [r8, #20]
	mov	r0, r5
	bl	BsGet
	mov	r1, #7
	strb	r0, [r8, #21]
	mov	r0, r5
	bl	BsGet
	mov	r1, #8
	str	r0, [r8, #32]
	mov	r0, r5
	bl	BsGet
	str	r0, [r8, #36]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	picture_coding_extension, .-picture_coding_extension
	.align	2
	.global	picture_spatial_scalable_extension
	.type	picture_spatial_scalable_extension, %function
picture_spatial_scalable_extension:
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
	add	r0, r0, #86016
	mov	r1, #32
	add	r0, r0, #608
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	BsSkip
	UNWIND(.fnend)
	.size	picture_spatial_scalable_extension, .-picture_spatial_scalable_extension
	.align	2
	.global	picture_temporal_scalable_extension
	.type	picture_temporal_scalable_extension, %function
picture_temporal_scalable_extension:
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
	add	r0, r0, #86016
	mov	r1, #32
	add	r0, r0, #608
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	BsSkip
	UNWIND(.fnend)
	.size	picture_temporal_scalable_extension, .-picture_temporal_scalable_extension
	.align	2
	.global	extra_bit_information
	.type	extra_bit_information, %function
extra_bit_information:
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
	add	r4, r0, #86016
	mov	r5, #0
	add	r4, r4, #608
	b	.L631
.L633:
	bl	BsSkip
	mov	r0, r4
	bl	BsResidBits
	add	r5, r5, #1
	cmp	r0, #8
	ble	.L632
.L631:
	mov	r1, #1
	mov	r0, r4
	bl	BsGet
	mov	r1, #8
	cmp	r0, #0
	mov	r0, r4
	bne	.L633
.L632:
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	extra_bit_information, .-extra_bit_information
	.align	2
	.global	picture_header
	.type	picture_header, %function
picture_header:
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
	add	r2, r0, #86016
	mov	r1, #10
	add	r5, r2, #608
	mov	r7, r0
	ldr	r3, [r2, #364]
	mov	r0, r5
	ldr	r4, [r2, #1688]
	bic	r3, r3, #3145728
	bic	r3, r3, #56
	orr	r3, r3, #524288
	str	r3, [r2, #364]
	bl	BsGet
	mov	r1, #3
	str	r0, [r4, #156]
	mov	r0, r5
	bl	BsGet
	mov	r1, #16
	strb	r0, [r4, #22]
	mov	r0, r5
	bl	BsGet
	ldrb	r6, [r4, #22]	@ zero_extendqisi2
	sub	r3, r6, #2
	cmp	r3, #1
	str	r0, [r4, #160]
	bls	.L637
.L635:
	cmp	r6, #3
	beq	.L638
.L636:
	mov	r0, r7
	bl	extra_bit_information
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L638:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, r6
	strb	r0, [r4, #25]
	mov	r0, r5
	bl	BsGet
	strb	r0, [r4, #26]
	b	.L636
.L637:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	mov	r1, #3
	strb	r0, [r4, #23]
	mov	r0, r5
	bl	BsGet
	ldrb	r6, [r4, #22]	@ zero_extendqisi2
	strb	r0, [r4, #24]
	b	.L635
	UNWIND(.fnend)
	.size	picture_header, .-picture_header
	.align	2
	.global	copyright_extension
	.type	copyright_extension, %function
copyright_extension:
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
	add	r0, r0, #86016
	mov	r1, #32
	add	r0, r0, #608
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	BsSkip
	UNWIND(.fnend)
	.size	copyright_extension, .-copyright_extension
	.align	2
	.global	extension
	.type	extension, %function
extension:
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
	add	r3, r0, #86016
	mov	r2, #0
	add	r5, r3, #608
	mov	r4, r0
	ldr	r3, [r3, #1688]
	mov	r1, #4
	mov	r0, r5
	str	r2, [r3, #148]
	str	r2, [r3, #152]
	str	r2, [r3, #128]
	str	r2, [r3, #132]
	str	r2, [r3, #136]
	str	r2, [r3, #140]
	str	r2, [r3, #144]
	bl	BsGet
	sub	r0, r0, #1
	cmp	r0, #9
	ldrls	pc, [pc, r0, asl #2]
	b	.L641
.L643:
	.word	.L642
	.word	.L644
	.word	.L645
	.word	.L646
	.word	.L646
	.word	.L641
	.word	.L648
	.word	.L649
	.word	.L646
	.word	.L646
.L648:
	mov	r0, r4
	bl	picture_display_extension
.L641:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L646:
	mov	r0, r5
	mov	r1, #32
	bl	BsSkip
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L645:
	mov	r0, r4
	bl	quant_matrix_extension
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L649:
	mov	r0, r4
	bl	picture_coding_extension
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L642:
	mov	r0, r4
	bl	sequence_extension
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L644:
	mov	r0, r4
	bl	sequence_display_extension
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	extension, .-extension
	.align	2
	.global	MPEG2DEC_Init
	.type	MPEG2DEC_Init, %function
MPEG2DEC_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 888
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #888)
	sub	sp, sp, #888
	ldr	r3, [r1, #608]
	mov	r7, r1
	mov	r5, r0
	cmp	r3, #1
	ldr	r6, .L662
	add	r4, r0, #86016
	beq	.L659
.L653:
	ldr	r8, [r4, #2040]
	movw	r2, #22560
	ldr	r3, [r6, #48]
	movt	r2, 1
	mov	r1, #0
	add	r6, r5, #87040
	mov	r0, r5
	ldr	r9, .L662
	blx	r3
	add	r0, r6, #672
	str	r8, [r4, #2040]
	mov	r8, #1
	bl	ResetVoQueue
	add	r3, r5, #45056
	str	r7, [r5]
	add	r2, r6, #288
	str	r8, [r4, #376]
	add	r6, r6, #476
	str	r2, [r4, #1692]
	mov	r2, #3
	str	r6, [r4, #1688]
	mov	r6, #0
	str	r6, [r4, #2060]
	mov	r0, r5
	strb	r2, [r3, #305]
	str	r6, [r4, #420]
	ldr	r3, [r7, #8]
	str	r6, [r4, #412]
	str	r3, [r4, #352]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #2048]
	beq	.L660
	ldr	r5, [r7, #608]
	cmp	r5, #1
	movne	r5, r8
	beq	.L661
.L655:
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L661:
	ldr	r3, [r9, #52]
	sub	r1, fp, #548
	mov	r2, #256
	add	r0, r4, #640
	blx	r3
	ldr	r3, [r9, #52]
	sub	r1, fp, #292
	mov	r2, #256
	add	r0, r4, #896
	blx	r3
	ldr	r3, [r9, #52]
	sub	r1, fp, #924
	mov	r2, #188
	ldr	r0, [r4, #1688]
	blx	r3
	ldr	r3, [r9, #52]
	ldr	r0, [r4, #1692]
	mov	r2, #188
	sub	r1, fp, #736
	blx	r3
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L659:
	add	r1, r4, #640
	ldr	r3, [r6, #52]
	mov	r2, #256
	sub	r0, fp, #548
	blx	r3
	ldr	r3, [r6, #52]
	add	r1, r4, #896
	mov	r2, #256
	sub	r0, fp, #292
	blx	r3
	ldr	r3, [r6, #52]
	ldr	r1, [r4, #1688]
	mov	r2, #188
	sub	r0, fp, #924
	blx	r3
	ldr	r3, [r6, #52]
	ldr	r1, [r4, #1692]
	mov	r2, #188
	sub	r0, fp, #736
	blx	r3
	b	.L653
.L660:
	mov	r0, r6
	ldr	r1, .L662+4
	bl	dprint_vfmw
	mvn	r5, #19
	b	.L655
.L663:
	.align	2
.L662:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC39
	UNWIND(.fnend)
	.size	MPEG2DEC_Init, .-MPEG2DEC_Init
	.align	2
	.global	MPEG2DEC_Destroy
	.type	MPEG2DEC_Destroy, %function
MPEG2DEC_Destroy:
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
	add	r3, r0, #86016
	ldr	r1, .L665
	mov	r0, #2
	ldr	r2, [r3, #372]
	bl	dprint_vfmw
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L666:
	.align	2
.L665:
	.word	.LC40
	UNWIND(.fnend)
	.size	MPEG2DEC_Destroy, .-MPEG2DEC_Destroy
	.align	2
	.global	MPEG2DEC_RecycleImage
	.type	MPEG2DEC_RecycleImage, %function
MPEG2DEC_RecycleImage:
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
	mov	r4, r1
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r5, r0, #86016
	mov	r2, #0
	ldr	r0, [r5, #2048]
	bl	FSP_SetDisplay
	mov	r1, r4
	ldr	r0, [r5, #2048]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	addne	r4, r0, #552
	addne	r6, r0, #584
	movne	r8, #0
	movne	r9, #0
	beq	.L671
.L670:
	ldr	r1, [r4, #8]!
	ldr	r0, [r5, #2048]
	bl	FreeUsdByDec
	cmp	r4, r6
	strd	r8, [r4]
	bne	.L670
.L671:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG2DEC_RecycleImage, .-MPEG2DEC_RecycleImage
	.align	2
	.global	MPEG2DEC_GetRemainImg
	.type	MPEG2DEC_GetRemainImg, %function
MPEG2DEC_GetRemainImg:
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
	ldr	r3, [r0]
	mov	r4, r0
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L696
	add	r5, r0, #86016
	ldr	r1, [r5, #444]
	ldr	r0, [r5, #2048]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L696
	ldr	r1, [r5, #444]
	ldr	r0, [r5, #2048]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L694
.L696:
	add	r6, r4, #87040
.L675:
	add	r7, r6, #672
.L681:
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	GetVoLastImageID
.L694:
	mov	r0, r4
	bl	MP2_SetImgFormat
	add	r3, r5, #376
	add	r2, r9, #560
	add	ip, r5, #392
	mov	r0, #0
.L677:
	ldr	r1, [r3, #4]!
	mov	r8, #0
	add	r2, r2, #8
	cmp	r1, r8
	strne	r0, [r2, #-4]
	strne	r1, [r2, #-8]
	cmp	r3, ip
	str	r0, [r3]
	bne	.L677
	add	r6, r4, #87040
	mov	r3, #1
	add	r7, r6, #672
	str	r8, [r5, #396]
	mov	r2, r3
	str	r3, [r9, #188]
	ldr	r1, [r9, #156]
	ldr	r0, [r5, #2048]
	bl	FSP_SetDisplay
	ldr	r0, [r5, #2048]
	mov	r3, r7
	str	r9, [sp]
	mov	r2, r4
	mov	r1, #3
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L678
	ldr	r1, [r9, #156]
	mov	r0, r4
	bl	MPEG2DEC_RecycleImage
	b	.L681
.L678:
	ldr	r3, [r9, #160]
	cmp	r3, #100
	bhi	.L675
	ldr	r3, [r9, #112]
	cmp	r3, #31
	bls	.L675
	ldr	r3, [r9, #116]
	cmp	r3, #31
	bls	.L681
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG2DEC_GetRemainImg, .-MPEG2DEC_GetRemainImg
	.align	2
	.global	MPEG2DEC_GetImageBuffer
	.type	MPEG2DEC_GetImageBuffer, %function
MPEG2DEC_GetImageBuffer:
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
	add	r3, r0, #45056
	mov	r4, r0
	add	r5, r0, #86016
	ldrb	r3, [r3, #305]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L698
	ldr	r3, [r5, #376]
	cmp	r3, #0
	beq	.L700
.L698:
	ldr	r0, [r5, #2048]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L700
	ldr	r0, [r5, #2048]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L708
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L700:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L708:
	add	r1, r4, #87040
	ldr	r0, [r5, #2048]
	add	r1, r1, #672
	bl	FSP_ClearNotInVoQueue
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MPEG2DEC_GetImageBuffer, .-MPEG2DEC_GetImageBuffer
	.align	2
	.global	MP2_ClearCurPacket
	.type	MP2_ClearCurPacket, %function
MP2_ClearCurPacket:
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
	ldr	r1, [r1, #20]
	mov	r5, r0
	bl	SM_ReleaseStreamSeg
	ldrsb	r3, [r4]
	cmp	r3, #1
	beq	.L710
	mov	r0, r5
	ldr	r1, [r4, #24]
	bl	SM_ReleaseStreamSeg
.L710:
	ldr	r3, .L714
	mov	r0, r4
	mov	r2, #36
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L715:
	.align	2
.L714:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MP2_ClearCurPacket, .-MP2_ClearCurPacket
	.align	2
	.global	MP2_ReceivePacket
	.type	MP2_ReceivePacket, %function
MP2_ReceivePacket:
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
	add	r3, r0, #40960
	beq	.L721
	mov	r5, #0
	str	r5, [r3, #8]
	ldr	r1, [r4, #16]
	str	r1, [r3, #52]
	ldr	r1, [r4, #12]
	str	r1, [r3, #16]
	ldr	r1, [r4]
	str	r1, [r3, #40]
	ldr	r1, [r4, #16]
	str	r1, [r3, #24]
	ldrb	r1, [r4, #6]	@ zero_extendqisi2
	strb	r1, [r3, #12]
	ldr	r1, [r4, #20]
	str	r1, [r3, #32]
	ldrb	ip, [r4, #6]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L719
	ldr	r1, [r4, #92]
	str	r1, [r3, #20]
	ldr	r1, [r4, #80]
	str	r1, [r3, #44]
	ldr	r1, [r4, #96]
	str	r1, [r3, #28]
	ldrb	r1, [r4, #86]	@ zero_extendqisi2
	strb	r1, [r3, #13]
	ldr	r1, [r4, #100]
	str	r1, [r3, #36]
	ldrb	ip, [r4, #86]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L720
	add	r1, r3, #12
	mov	r0, r2
	bl	MP2_ClearCurPacket
	ldrb	r2, [r4, #86]	@ zero_extendqisi2
	ldr	r1, .L722
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L720:
	mov	r0, ip
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L719:
	str	r5, [r3, #20]
	mov	r0, ip
	str	r5, [r3, #44]
	str	r5, [r3, #28]
	strb	r5, [r3, #13]
	str	r5, [r3, #36]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L721:
	ldr	r1, .L722+4
	mov	r0, #1
	ldr	r2, [r3, #68]
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L723:
	.align	2
.L722:
	.word	.LC42
	.word	.LC41
	UNWIND(.fnend)
	.size	MP2_ReceivePacket, .-MP2_ReceivePacket
	.align	2
	.global	MP2_ClearCurPic
	.type	MP2_ClearCurPic, %function
MP2_ClearCurPic:
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
	add	r6, r0, #45056
	ldr	r3, [r6, #280]
	cmp	r3, #0
	movne	r4, r0
	movne	r5, #0
	addne	r7, r4, #86016
	bne	.L727
	b	.L728
.L726:
	ldr	r3, [r6, #280]
	add	r4, r4, #44
	cmp	r5, r3
	bcs	.L728
.L727:
	ldr	r1, [r4, #304]
	add	r5, r5, #1
	ldr	r0, [r7, #2048]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #284]
	cmp	r3, #0
	beq	.L726
	ldr	r3, [r4, #292]
	cmp	r3, #0
	beq	.L726
	ldr	r1, [r4, #308]
	add	r4, r4, #44
	ldr	r0, [r7, #2048]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r6, #280]
	cmp	r5, r3
	bcc	.L727
.L728:
	mov	r3, #0
	str	r3, [r6, #280]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_ClearCurPic, .-MP2_ClearCurPic
	.global	__aeabi_uidiv
	.align	2
	.global	MPEG2DEC_VDMPostProc
	.type	MPEG2DEC_VDMPostProc, %function
MPEG2DEC_VDMPostProc:
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
	ldrb	r3, [r0, #152]	@ zero_extendqisi2
	add	r4, r0, #86016
	mov	r7, r0
	mov	r2, r1
	mov	r8, r1
	strb	r3, [r4, #544]
	mov	r0, #22
	ldr	r1, .L809
	add	r5, r7, #8
	ldr	r6, [r4, #1688]
	bl	dprint_vfmw
	ldr	r3, [r4, #1688]
	mov	r0, r7
	ldr	r2, [r7]
	add	r9, r7, #45056
	ldr	r1, [r3, #40]
	str	r1, [r2, #568]
	ldr	r2, [r3, #44]
	ldr	r3, [r7]
	str	r2, [r3, #572]
	ldr	r3, [r4, #2052]
	ldr	r2, [r4, #2056]
	ldr	r1, [r7]
	cmp	r3, r2
	rsbge	r3, r2, r3
	movlt	r3, #0
	str	r3, [r1, #576]
	bl	MP2_ClearCurPic
	ldr	r3, .L809+4
	movw	r2, #45348
	mov	r0, r5
	mov	r1, #0
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r1, [r4, #448]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #444]
	mov	r5, r0
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #440]
	mov	r10, r0
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	cmp	r5, #0
	beq	.L803
	ldrb	r3, [r6, #22]	@ zero_extendqisi2
	cmp	r8, #0
	strb	r3, [r9, #304]
	str	r8, [r5, #160]
	blt	.L741
	cmp	r8, #100
	movlt	r1, r8
	movge	r1, #100
	str	r1, [r5, #160]
	ldrb	r3, [r6, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L742
	ldr	r3, [r7]
	ldr	r3, [r3]
	cmp	r1, r3
	bhi	.L804
.L742:
	ldr	r1, .L809+8
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r7
	bl	MP2_SetImgFormat
	ldr	r3, [r7]
	ldr	r3, [r3, #596]
	cmp	r3, #1
	movne	r3, #0
	strne	r3, [r5, #80]
	strne	r3, [r5, #84]
	beq	.L805
.L748:
	ldrb	r3, [r6, #9]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L751
	ldr	r3, [r4, #376]
	cmp	r3, #1
	movne	r0, #1
	beq	.L751
.L798:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L751:
	add	r1, r5, #560
	add	r3, r4, #376
	add	lr, r4, #392
	mov	r0, #0
.L750:
	ldr	r2, [r3, #4]!
	mov	ip, #0
	add	r1, r1, #8
	cmp	r2, ip
	strne	r0, [r1, #-4]
	strne	r2, [r1, #-8]
	cmp	r3, lr
	str	r0, [r3]
	bne	.L750
	str	ip, [r4, #396]
	ldr	r3, [r7]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L753
	ldrb	r3, [r6, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L753
	ldr	r1, [r4, #440]
	str	r1, [r4, #452]
.L754:
	cmn	r1, #1
	beq	.L756
	ldr	r0, [r4, #2048]
	bl	FSP_GetDisplay
	cmp	r0, #1
	beq	.L760
	ldr	r1, [r4, #452]
	ldr	r0, [r4, #2048]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L760
.L756:
	ldrb	r3, [r6, #27]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L806
	ldrb	r3, [r6, #18]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #528]
	addeq	r3, r3, #1
	addne	r3, r3, #1
	streq	r3, [r4, #528]
	ldreqb	r3, [r6, #16]	@ zero_extendqisi2
	ldreq	r2, [r4, #524]
	addeq	r3, r3, #2
	strne	r3, [r4, #528]
	addeq	r3, r2, r3
	movne	r3, r3, asl #1
	str	r3, [r4, #524]
.L767:
	ldr	r2, [r4, #520]
	mov	r0, #1
	ldr	r3, [r4, #372]
	add	r2, r2, r0
	str	r2, [r4, #520]
	add	r3, r3, r0
	str	r3, [r4, #372]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L741:
	mov	r3, #0
	str	r3, [r5, #160]
	b	.L742
.L804:
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L744
	ldr	r1, [r5, #560]
	add	r6, r5, #576
	ldr	r0, [r4, #2048]
	add	r7, r5, #592
	bl	FreeUsdByDec
	ldr	r1, [r5, #568]
	ldr	r0, [r4, #2048]
	add	r8, r5, #560
	bl	FreeUsdByDec
	ldr	r1, [r5, #576]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	ldr	r1, [r5, #584]
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	mov	r0, #0
	mov	r1, #0
	mov	r2, #1
	strd	r0, [r8]
	strd	r0, [r6, #-8]
	strd	r0, [r6]
	strd	r0, [r7, #-8]
	ldr	r1, [r4, #444]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #440]
	ldr	r0, [r4, #2048]
	bl	FSP_ClearLogicFs
	mov	r3, #0
	ldr	r0, [r4, #2048]
	str	r3, [r4, #440]
	str	r3, [r4, #444]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L744
	ldr	r0, [r4, #2048]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #440]
	str	r0, [r4, #444]
.L744:
	mov	r3, #0
	mov	r0, #1
	str	r3, [r4, #404]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L805:
	ldr	r2, .L809+12
	ldr	r2, [r2]
	add	r2, r2, #4096
	ldr	r0, [r2, #84]
	ldr	r1, [r2, #88]
	str	r3, [r5, #80]
	cmp	r1, #0
	moveq	r1, #1
	bl	__aeabi_uidiv
	str	r0, [r5, #84]
	b	.L748
.L760:
	ldr	r1, [r4, #452]
	ldr	r0, [r4, #2048]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L807
	ldr	r2, [r5, #160]
	cmp	r2, #0
	beq	.L762
	ldr	r3, [r7]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bhi	.L808
.L762:
	ldr	r8, [r4, #452]
	ldr	r0, [r4, #2048]
	mov	r1, r8
	bl	FSP_GetRef
	mov	r2, r8
	ldr	r1, .L809+16
	mov	r3, r0
	mov	r0, #12
	bl	dprint_vfmw
	mov	r2, #1
	ldr	r1, [r4, #452]
	ldr	r0, [r4, #2048]
	bl	FSP_SetDisplay
	add	r3, r7, #87040
	ldr	r0, [r4, #2048]
	add	r3, r3, #672
	str	r5, [sp]
	mov	r2, r7
	mov	r1, #3
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L769
	add	r6, r5, #552
	add	r5, r5, #584
	mov	r8, #0
	mov	r9, #0
.L764:
	ldr	r1, [r6, #8]!
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	cmp	r6, r5
	strd	r8, [r6]
	bne	.L764
	ldr	r0, [r4, #2048]
	mov	r2, #0
	ldr	r1, [r4, #452]
	bl	FSP_SetDisplay
	mov	r0, #0
	b	.L798
.L753:
	ldr	r1, [r4, #448]
	str	r1, [r4, #452]
	b	.L754
.L806:
	ldrb	r2, [r6, #16]	@ zero_extendqisi2
	ldrb	r3, [r6, #10]	@ zero_extendqisi2
	ldr	r1, [r4, #528]
	add	r3, r2, r3
	add	r3, r3, #1
	add	r3, r3, r1
	str	r3, [r4, #528]
	mov	r3, r3, asl #1
	str	r3, [r4, #524]
	b	.L767
.L808:
	ldr	r1, .L809+20
	mov	r0, #1
	bl	dprint_vfmw
	add	r7, r5, #552
	add	r5, r5, #584
	mov	r8, #0
	mov	r9, #0
.L765:
	ldr	r1, [r7, #8]!
	ldr	r0, [r4, #2048]
	bl	FreeUsdByDec
	cmp	r7, r5
	strd	r8, [r7]
	bne	.L765
	mov	r2, #0
	ldr	r1, [r4, #452]
	ldr	r0, [r4, #2048]
	bl	FSP_SetDisplay
	b	.L756
.L803:
	str	r0, [sp]
	mov	r3, r10
	mov	r2, r5
	ldr	r1, .L809+24
	mov	r0, r5
	bl	dprint_vfmw
	mov	r0, r5
	b	.L798
.L769:
	ldr	r3, [r4, #2056]
	add	r3, r3, #1
	str	r3, [r4, #2056]
	b	.L756
.L807:
	mov	r2, #3552
	ldr	r1, .L809+28
	bl	dprint_vfmw
	mov	r0, r5
	b	.L798
.L810:
	.align	2
.L809:
	.word	.LC43
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC45
	.word	g_HwMem
	.word	.LC47
	.word	.LC46
	.word	.LC44
	.word	.LC23
	UNWIND(.fnend)
	.size	MPEG2DEC_VDMPostProc, .-MPEG2DEC_VDMPostProc
	.align	2
	.global	MP2_u_v
	.type	MP2_u_v, %function
MP2_u_v:
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
	mov	r5, r2
	mov	r6, r0
	mov	r7, r1
	bl	BsShow
	mov	r1, r7
	mov	r4, r0
	mov	r0, r6
	bl	BsSkip
	mov	r3, r4
	mov	r2, r5
	ldr	r1, .L812
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L813:
	.align	2
.L812:
	.word	.LC48
	UNWIND(.fnend)
	.size	MP2_u_v, .-MP2_u_v
	.align	2
	.global	MP2_DecSliceHeader
	.type	MP2_DecSliceHeader, %function
MP2_DecSliceHeader:
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
	ldr	r2, [r0, #4]
	mov	r1, #1
	mov	r3, #0
	mov	r5, r0
	cmp	r2, #175
	str	r1, [r0, #16]
	ldr	r4, [r0, #52]
	str	r3, [r0, #24]
	bls	.L815
	ldr	r2, [r0]
	cmp	r2, r3
	beq	.L827
.L815:
	ldr	r2, [r5, #20]
	mov	r0, r4
	ldr	r1, [r5, #8]
	sub	r2, r2, #1
	add	r3, r2, r3
	ldr	r2, .L830
	mul	r3, r1, r3
	mov	r1, #5
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r5, #28]
	bl	MP2_u_v
	mov	r3, #0
	mov	r1, #1
	str	r3, [r5, #32]
	str	r0, [r5, #44]
	mov	r0, r4
	bl	BsShow
	cmp	r0, #0
	bne	.L828
.L819:
	mov	r0, r4
	ldr	r2, .L830+4
	mov	r1, #1
	bl	MP2_u_v
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L828:
	ldr	r2, .L830+8
	mov	r1, #1
	mov	r0, r4
	bl	MP2_u_v
	ldr	r2, .L830+12
	mov	r1, #1
	mov	r0, r4
	bl	MP2_u_v
	ldr	r2, .L830+16
	mov	r1, #7
	str	r0, [r5, #32]
	mov	r0, r4
	bl	MP2_u_v
	b	.L818
.L829:
	bl	BsShow
	ldr	r2, .L830+4
	mov	r1, #1
	cmp	r0, #0
	mov	r0, r4
	beq	.L819
	bl	MP2_u_v
	ldr	r2, .L830+20
	mov	r1, #8
	mov	r0, r4
	bl	MP2_u_v
.L818:
	mov	r0, r4
	bl	BsResidBits
	mov	r1, #1
	cmp	r0, #0
	mov	r0, r4
	bgt	.L829
	b	.L819
.L827:
	ldr	r2, .L830+24
	mov	r1, #3
	mov	r0, r4
	bl	MP2_u_v
	mov	r3, r0, asl #7
	str	r0, [r5, #24]
	b	.L815
.L831:
	.align	2
.L830:
	.word	.LC50
	.word	.LC51
	.word	.LC52
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC49
	UNWIND(.fnend)
	.size	MP2_DecSliceHeader, .-MP2_DecSliceHeader
	.align	2
	.global	MP2_ExitMb
	.type	MP2_ExitMb, %function
MP2_ExitMb:
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
	str	r3, [r0, #16]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_ExitMb, .-MP2_ExitMb
	.align	2
	.global	MP2_WriteSliceInfo
	.type	MP2_WriteSliceInfo, %function
MP2_WriteSliceInfo:
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
	add	r7, r0, #45056
	mov	r4, #44
	add	r5, r0, #86016
	mov	r6, r0
	ldr	r2, [r7, #280]
	ldr	r3, [r5, #584]
	cmp	r2, #1024
	mla	r4, r4, r2, r0
	str	r3, [r4, #316]
	ldr	r3, [r5, #596]
	str	r3, [r4, #320]
	ldr	r3, [r5, #592]
	str	r3, [r4, #312]
	bcs	.L839
	ldr	r2, [r5, #632]
	add	r0, r5, #608
	ldr	r3, [r5, #320]
	and	r1, r2, #7
	str	r1, [r4, #296]
	add	r3, r3, r2, lsr #3
	str	r3, [r4, #280]
	ldr	r3, [r5, #336]
	str	r3, [r4, #304]
	bl	BsResidBits
	add	r2, r5, #316
	str	r0, [r4, #288]
	ldrsh	r3, [r2]
	cmp	r3, #1
	beq	.L840
	cmp	r3, #256
	bne	.L838
	ldr	r3, [r5, #324]
	mov	r2, #0
	str	r2, [r4, #300]
	str	r3, [r4, #284]
	ldr	r3, [r5, #340]
	str	r3, [r4, #308]
	ldr	r3, [r5, #332]
	mov	r3, r3, asl #3
	str	r3, [r4, #292]
.L837:
	ldr	r3, [r7, #280]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r7, #280]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L839:
	mov	r3, #1024
	ldr	r1, .L841
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	bl	MP2_ClearCurPic
	ldr	r3, [r5, #364]
	mov	r0, #0
	str	r0, [r5, #420]
	bic	r3, r3, #5767168
	str	r3, [r5, #364]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L840:
	mov	r3, #0
	str	r3, [r4, #284]
	str	r3, [r4, #300]
	str	r3, [r4, #308]
	str	r3, [r4, #292]
	b	.L837
.L838:
	movw	r3, #317
	ldrsb	r2, [r2]
	ldrsb	r3, [r5, r3]
	mov	r0, #1
	ldr	r1, .L841+4
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L842:
	.align	2
.L841:
	.word	.LC56
	.word	.LC57
	UNWIND(.fnend)
	.size	MP2_WriteSliceInfo, .-MP2_WriteSliceInfo
	.align	2
	.global	MP2_MbAddrInc
	.type	MP2_MbAddrInc, %function
MP2_MbAddrInc:
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
	mov	r1, #4
	mov	r6, r0
	bl	BsShow
	ldr	r5, .L849
	add	r0, r5, r0, lsl #3
	ldr	r4, [r0, #224]
	ldr	r1, [r0, #228]
	cmp	r4, #6
	bls	.L847
	cmp	r4, #9
	beq	.L848
	mov	r1, #11
	mov	r0, r6
	bl	BsShow
	ldr	r2, .L849+4
	and	r3, r0, #127
	mov	r0, r6
	sub	r3, r3, #24
	add	r5, r5, r3, lsl #3
	ldr	r4, [r5, #352]
	ldr	r1, [r5, #356]
	bl	MP2_u_v
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L847:
	mov	r0, r6
	ldr	r2, .L849+4
	bl	MP2_u_v
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L848:
	mov	r0, r6
	ldr	r2, .L849+4
	bl	MP2_u_v
	rsb	r0, r0, #9
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L850:
	.align	2
.L849:
	.word	.LANCHOR0
	.word	.LC58
	UNWIND(.fnend)
	.size	MP2_MbAddrInc, .-MP2_MbAddrInc
	.align	2
	.global	MP2_InitMb
	.type	MP2_InitMb, %function
MP2_InitMb:
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
	mov	r5, #0
	mov	r6, r0
	ldr	r4, [r0, #52]
	b	.L852
.L856:
	cmp	r0, #15
	beq	.L853
	cmp	r0, #8
	add	r5, r5, #33
	bne	.L864
.L853:
	ldr	r2, .L866
	mov	r1, #11
	mov	r0, r4
	bl	MP2_u_v
.L852:
	mov	r1, #11
	mov	r0, r4
	bl	BsShow
	cmp	r0, #23
	bls	.L856
	mov	r0, r4
	bl	MP2_MbAddrInc
	ldr	r4, [r6, #16]
	cmp	r4, #0
	beq	.L857
	ldr	r3, [r6, #28]
	add	r5, r5, r0
	cmp	r3, #0
	addne	r2, r5, r3
	ldr	r3, [r6, #12]
	subeq	r2, r5, #1
	str	r2, [r6, #40]
	sub	r3, r3, #1
	cmp	r2, r3
	bhi	.L865
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L864:
	ldr	r1, .L866+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L857:
	mov	r2, r4
	ldr	r1, .L866+8
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L865:
	ldr	r1, .L866+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L867:
	.align	2
.L866:
	.word	.LC60
	.word	.LC59
	.word	.LC62
	.word	.LC61
	UNWIND(.fnend)
	.size	MP2_InitMb, .-MP2_InitMb
	.align	2
	.global	MP2_GetFirstMbInSlice
	.type	MP2_GetFirstMbInSlice, %function
MP2_GetFirstMbInSlice:
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
	bl	MP2_DecSliceHeader
	mov	r0, r4
	bl	MP2_InitMb
	mov	r3, #0
	str	r3, [r4, #16]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_GetFirstMbInSlice, .-MP2_GetFirstMbInSlice
	.align	2
	.global	MP2_DecSlice
	.type	MP2_DecSlice, %function
MP2_DecSlice:
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
	add	r4, r0, #86016
	uxtb	r8, r1
	add	r6, r4, #552
	add	lr, r4, #608
	ldr	r2, [r4, #400]
	mov	r7, r0
	ldr	r3, [r4, #1688]
	mov	r0, r6
	ldr	ip, [r4, #344]
	sub	r2, r2, #1
	ldr	r1, [r4, #328]
	clz	r2, r2
	mov	r5, #0
	str	ip, [r4, #600]
	mov	r2, r2, lsr #5
	str	r1, [r4, #588]
	str	r2, [r4, #552]
	ldr	r2, [r3, #60]
	str	r2, [r4, #556]
	ldr	r2, [r3, #56]
	str	r2, [r4, #560]
	ldr	ip, [r3, #52]
	ldr	r1, [r3, #48]
	add	ip, ip, #15
	str	lr, [r4, #604]
	add	r1, r1, #15
	str	r8, [r4, #572]
	mov	ip, ip, lsr #4
	mov	r1, r1, lsr #4
	mul	r1, r1, ip
	str	r1, [r4, #564]
	bl	MP2_DecSliceHeader
	mov	r0, r6
	bl	MP2_InitMb
	str	r5, [r4, #568]
	cmp	r0, #1
	mov	r6, r0
	beq	.L870
	ldr	r1, .L881
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L881+4
	ldr	ip, [r3]
	cmp	ip, r5
	beq	.L873
.L880:
	ldr	r0, [r4, #2048]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #127
	blx	ip
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L870:
	mov	r0, r7
	bl	MP2_WriteSliceInfo
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r0, r6
	ldr	r1, .L881+8
	bl	dprint_vfmw
	ldr	r3, .L881+4
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L880
.L873:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L882:
	.align	2
.L881:
	.word	.LC63
	.word	g_event_report
	.word	.LC64
	UNWIND(.fnend)
	.size	MP2_DecSlice, .-MP2_DecSlice
	.align	2
	.global	MP2_GetHDR
	.type	MP2_GetHDR, %function
MP2_GetHDR:
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
	add	r4, r0, #86016
	add	r5, r4, #608
	mov	r1, #32
	mov	r6, r0
	mov	r7, #0
	mov	r0, r5
	strb	r7, [r4, #2064]
	ldr	r8, [r4, #1688]
	ldr	r9, [r4, #1692]
	bl	BsShow
	ldr	r3, [r4, #364]
	str	r3, [fp, #-80]
	mov	r10, r0
	mov	r0, r5
	bl	BsPos
	mov	r1, #32
	str	r0, [r4, #424]
	mov	r0, r5
	bl	BsSkip
	movw	r3, #439
	cmp	r10, r3
	beq	.L951
	movw	r3, #435
	cmp	r10, r3
	cmpne	r10, #440
	bne	.L890
.L886:
	ldr	r3, [r4, #420]
	cmp	r3, #1
	beq	.L952
.L887:
	movw	r3, #435
	cmp	r10, r3
	beq	.L892
.L955:
	cmp	r10, #436
	bcc	.L953
	movw	r3, #437
	cmp	r10, r3
	beq	.L896
	cmp	r10, #436
	bls	.L946
	movw	r3, #439
	cmp	r10, r3
	beq	.L946
	cmp	r10, #440
	bne	.L891
	ldr	r3, [r4, #416]
	mov	r0, r5
	ldr	r7, [r4, #312]
	add	r7, r3, r7
	bl	BsPos
	ldr	r1, .L959
	add	r2, r7, r0, lsr #3
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	group_of_pictures_header
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	blt	.L954
.L946:
	mov	r5, #0
.L889:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L890:
	cmp	r10, #256
	beq	.L886
	movw	r3, #435
	cmp	r10, r3
	bne	.L955
.L892:
	ldr	r3, [r4, #416]
	mov	r0, r5
	ldr	r7, [r4, #312]
	add	r7, r3, r7
	bl	BsPos
	ldr	r1, .L959+4
	add	r2, r7, r0, lsr #3
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	sequence_header
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	blt	.L956
	ldr	r5, [r4, #364]
	ands	r5, r5, #7
	bne	.L901
	mov	r3, #1
	mov	r0, r9
	strb	r3, [r8]
	mov	r1, r8
	mov	r2, #188
	bl	memcpy
	ldr	r3, [r4, #368]
	add	r3, r3, #1
	str	r3, [r4, #368]
	b	.L889
.L953:
	cmp	r10, #256
	beq	.L894
	bcc	.L891
	cmp	r10, #432
	bls	.L891
	ldr	r3, [r4, #416]
	mov	r0, r5
	ldr	r5, [r4, #312]
	add	r5, r3, r5
	bl	BsPos
	ldr	r1, .L959+8
	add	r2, r5, r0, lsr #3
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	MP2_GetUsrData
	subs	r5, r0, #0
	bne	.L907
	add	r1, r6, #86016
	ldr	r0, [r4, #2048]
	add	r1, r1, #316
	bl	MP2_ClearCurPacket
	ldr	r3, .L959+12
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r5, #0
	beq	.L889
	mov	r3, r5
	mov	r2, r5
	ldr	r0, [r4, #2048]
	mov	r1, #113
	blx	ip
	mvn	r5, #0
	b	.L889
.L894:
	ldr	r3, [r4, #416]
	mov	r0, r5
	ldr	r7, [r4, #312]
	add	r7, r3, r7
	bl	BsPos
	ldr	r1, .L959+16
	add	r2, r7, r0, lsr #3
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	picture_header
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	bge	.L946
	ldr	r3, .L959+12
	ldr	r2, [fp, #-80]
	ldr	ip, [r3]
	str	r2, [r4, #364]
	cmp	ip, #0
	beq	.L906
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r4, #2048]
	blx	ip
.L906:
	add	r1, r6, #86016
	ldr	r0, [r4, #2048]
	add	r1, r1, #316
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	mov	r2, #256
	ldr	r1, .L959+20
	mov	r0, #1
	bl	dprint_vfmw
	b	.L889
.L952:
	ldr	r2, [r4, #364]
	tst	r2, #4194304
	beq	.L887
	orr	r2, r2, #8388608
	strb	r3, [r4, #2064]
	str	r2, [r4, #364]
	mov	r0, r6
	mov	r2, #0
	str	r2, [r4, #420]
	mov	r5, r3
	bl	MP2_ProtocolPPSCheck
	b	.L889
.L951:
	ldr	r3, .L959+24
	mov	r2, #32
	mov	r1, r5
	sub	r0, fp, #76
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, #32
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #32
	str	r0, [fp, #-84]
	sub	r0, fp, #76
	bl	BsGet
	ldr	r2, [fp, #-84]
	movw	r1, #20036
	movw	r3, #20553
	movt	r1, 17221
	movt	r3, 18515
	cmp	r0, r1
	cmpeq	r2, r3
	bne	.L886
	ldr	r5, [r4, #420]
	cmp	r5, #1
	beq	.L957
.L888:
	mov	r3, #1
	mov	r5, #0
	strb	r3, [r4, #2066]
	b	.L889
.L891:
	sub	r3, r10, #256
	sub	r3, r3, #1
	cmp	r3, #174
	bhi	.L909
	ldr	r7, [r4, #360]
	cmp	r7, #0
	beq	.L910
.L912:
	ldr	r3, [r4, #364]
	tst	r3, #589824
	beq	.L946
	orr	r3, r3, #4194304
	mov	r0, r6
	str	r3, [r4, #364]
	mov	r6, #1
	mov	r1, r10
	str	r6, [r4, #420]
	bl	MP2_DecSlice
	cmp	r0, r6
	mov	r5, r0
	beq	.L889
	mov	r2, r10
	mov	r0, r6
	ldr	r1, .L959+28
	mov	r5, #0
	bl	dprint_vfmw
	b	.L889
.L896:
	ldr	r3, [r4, #416]
	mov	r0, r5
	ldr	r7, [r4, #312]
	add	r7, r3, r7
	bl	BsPos
	ldr	r1, .L959+32
	add	r2, r7, r0, lsr #3
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	extension
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	bge	.L946
	ldr	r3, .L959+12
	ldr	r2, [fp, #-80]
	ldr	ip, [r3]
	str	r2, [r4, #364]
	cmp	ip, #0
	beq	.L908
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r4, #2048]
	blx	ip
.L908:
	add	r1, r6, #86016
	ldr	r0, [r4, #2048]
	add	r1, r1, #316
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	b	.L889
.L901:
	ldrb	r3, [r9]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L958
.L909:
	ldr	r3, .L959+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L946
.L945:
	mov	r3, #0
	ldr	r0, [r4, #2048]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L946
.L907:
	ldr	r1, .L959+36
	mov	r0, #1
	bl	dprint_vfmw
	b	.L889
.L957:
	ldr	r3, [r4, #364]
	tst	r3, #4194304
	beq	.L888
	orr	r3, r3, #8388608
	strb	r5, [r4, #2066]
	str	r3, [r4, #364]
	mov	r0, r6
	strb	r5, [r4, #2064]
	str	r7, [r4, #420]
	bl	MP2_ProtocolPPSCheck
	b	.L889
.L910:
	mov	r0, r6
	bl	MP2_GetSequenceInfo
	ldr	r5, [r4, #412]
	cmp	r5, #0
	beq	.L889
	ldrb	r3, [r4, #2065]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L912
	mov	r5, r7
	b	.L889
.L958:
	mov	r2, #188
	mov	r1, r9
	mov	r0, r8
	bl	memcpy
	ldr	r2, .L959+12
	ldr	r3, [r4, #364]
	ldr	ip, [r2]
	bic	r3, r3, #7
	str	r3, [r4, #364]
	cmp	ip, #0
	bne	.L945
	b	.L946
.L954:
	ldr	r3, .L959+12
	ldr	r2, [fp, #-80]
	ldr	ip, [r3]
	str	r2, [r4, #364]
	cmp	ip, #0
	beq	.L905
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r4, #2048]
	blx	ip
.L905:
	add	r1, r6, #86016
	ldr	r0, [r4, #2048]
	add	r1, r1, #316
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	mov	r2, #440
	ldr	r1, .L959+40
	mov	r0, #1
	bl	dprint_vfmw
	b	.L889
.L956:
	ldr	r3, .L959+12
	ldr	r2, [fp, #-80]
	ldr	ip, [r3]
	str	r2, [r4, #364]
	cmp	ip, #0
	beq	.L900
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r4, #2048]
	blx	ip
.L900:
	add	r1, r6, #86016
	ldr	r0, [r4, #2048]
	add	r1, r1, #316
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	movw	r2, #435
	ldr	r1, .L959+44
	mov	r0, #1
	bl	dprint_vfmw
	b	.L889
.L960:
	.align	2
.L959:
	.word	.LC67
	.word	.LC65
	.word	.LC71
	.word	g_event_report
	.word	.LC69
	.word	.LC70
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC74
	.word	.LC73
	.word	.LC72
	.word	.LC68
	.word	.LC66
	UNWIND(.fnend)
	.size	MP2_GetHDR, .-MP2_GetHDR
	.align	2
	.global	MPEG2DEC_Decode
	.type	MPEG2DEC_Decode, %function
MPEG2DEC_Decode:
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
	ldr	r3, .L999
	add	r5, r0, #87040
	add	r7, r0, #45312
	add	r5, r5, #128
	add	r6, r0, #86016
	add	r7, r7, #48
	ldr	r3, [r3, #52]
	mov	r2, #160
	mov	r4, r0
	mov	r0, r5
	blx	r3
	mov	r1, r5
	mov	r0, r7
	ldr	r2, [r6, #2048]
	bl	MP2_ReceivePacket
	cmp	r0, #1
	beq	.L962
	ldr	r1, .L999+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r5, #0
.L963:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L962:
	ldr	r2, [r6, #328]
	add	r0, r6, #608
	ldr	r1, [r6, #344]
	bl	BsInit
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L994
.L964:
	ldr	r3, [r6, #364]
	str	r3, [r6, #548]
.L965:
	mov	r0, r4
	bl	MP2_GetHDR
	add	r0, r0, #1
	cmp	r0, #1
	bls	.L995
	mov	r0, r4
	bl	MP2_DecodingDecide
	subs	r5, r0, #0
	beq	.L996
	cmn	r5, #1
	beq	.L993
	mov	r0, r4
	bl	MP2_GetPictureInfo
	subs	r5, r0, #0
	bne	.L971
	add	r1, r7, #40960
	ldr	r0, [r6, #2048]
	add	r1, r1, #12
	bl	MP2_ClearCurPacket
	mov	r0, r4
	bl	MP2_ClearCurPic
	ldr	r3, .L999+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L972
	ldr	r0, [r6, #2048]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L972:
	ldr	r1, .L999+12
	mov	r0, #1
	bl	dprint_vfmw
	mov	r5, #0
	b	.L963
.L995:
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L997
.L967:
	ldrb	r3, [r6, #2065]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L993
	add	r1, r7, #40960
	ldr	r0, [r6, #2048]
	add	r1, r1, #12
	mov	r5, #0
	bl	MP2_ClearCurPacket
	b	.L963
.L996:
	add	r1, r7, #40960
	ldr	r0, [r6, #2048]
	add	r1, r1, #12
	bl	MP2_ClearCurPacket
	mov	r0, r4
	bl	MP2_ClearCurPic
	ldr	r3, .L999+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L970
	ldr	r0, [r6, #2048]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L970:
	ldr	r1, .L999+16
	mov	r0, #22
	bl	dprint_vfmw
.L993:
	mov	r5, #0
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L994:
	ldr	r3, [r3, #872]
	cmp	r3, #1
	ldreq	r3, [r6, #548]
	streq	r3, [r6, #364]
	bne	.L964
	b	.L965
.L997:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	beq	.L967
	b	.L993
.L971:
	mov	r0, r4
	bl	MP2_DecNewPic
	subs	r5, r0, #0
	beq	.L998
	ldr	r5, [r6, #312]
	ldr	r3, [r6, #416]
	ldr	r2, [r6, #328]
	mov	r5, r5, asl #3
	add	r3, r3, r2
	str	r3, [r6, #416]
	b	.L963
.L998:
	mov	r2, #1
	ldr	r1, [r6, #448]
	ldr	r0, [r6, #2048]
	bl	FSP_ClearLogicFs
	add	r1, r7, #40960
	add	r1, r1, #12
	ldr	r0, [r6, #2048]
	bl	MP2_ClearCurPacket
	mov	r0, r4
	bl	MP2_ClearCurPic
	ldr	r3, .L999+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L974
	ldr	r0, [r6, #2048]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L974:
	ldr	r1, .L999+20
	mov	r0, #1
	bl	dprint_vfmw
	b	.L963
.L1000:
	.align	2
.L999:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC75
	.word	g_event_report
	.word	.LC77
	.word	.LC76
	.word	.LC78
	UNWIND(.fnend)
	.size	MPEG2DEC_Decode, .-MPEG2DEC_Decode
	.align	2
	.global	MP2_GetPacketState
	.type	MP2_GetPacketState, %function
MP2_GetPacketState:
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
	add	r0, r0, #86016
	ldrb	r3, [r0, #2064]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r0, #2
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_GetPacketState, .-MP2_GetPacketState
	.global	g_tmpusd
	.global	zigzag
	.global	default_intra_quantizer_matrix
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__FUNCTION__.14400, %object
	.size	__FUNCTION__.14400, 19
__FUNCTION__.14400:
	.ascii	"MP2_DecodingDecide\000"
	.space	1
.LC0:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	7
	.word	8
	.word	10
	.word	12
	.word	14
	.word	16
	.word	18
	.word	20
	.word	22
	.word	24
	.word	28
	.word	32
	.word	36
	.word	40
	.word	44
	.word	48
	.word	52
	.word	56
	.word	64
	.word	72
	.word	80
	.word	88
	.word	96
	.word	104
	.word	112
	.type	CSWTCH.47, %object
	.size	CSWTCH.47, 12
CSWTCH.47:
	.word	5
	.word	1
	.word	2
	.type	frame_rate_Table, %object
	.size	frame_rate_Table, 64
frame_rate_Table:
	.word	0
	.word	24551
	.word	24576
	.word	25600
	.word	30689
	.word	30720
	.word	51200
	.word	61378
	.word	61440
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.type	tab_for_first_4bit, %object
	.size	tab_for_first_4bit, 128
tab_for_first_4bit:
	.word	8
	.word	4
	.word	9
	.word	5
	.word	5
	.word	4
	.word	4
	.word	4
	.word	3
	.word	3
	.word	3
	.word	3
	.word	2
	.word	3
	.word	2
	.word	3
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.type	tab_for_last_7bit, %object
	.size	tab_for_last_7bit, 832
tab_for_last_7bit:
	.word	33
	.word	11
	.word	32
	.word	11
	.word	31
	.word	11
	.word	30
	.word	11
	.word	29
	.word	11
	.word	28
	.word	11
	.word	27
	.word	11
	.word	26
	.word	11
	.word	25
	.word	11
	.word	24
	.word	11
	.word	23
	.word	11
	.word	22
	.word	11
	.word	21
	.word	10
	.word	21
	.word	10
	.word	20
	.word	10
	.word	20
	.word	10
	.word	19
	.word	10
	.word	19
	.word	10
	.word	18
	.word	10
	.word	18
	.word	10
	.word	17
	.word	10
	.word	17
	.word	10
	.word	16
	.word	10
	.word	16
	.word	10
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	15
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	14
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	13
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	12
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	11
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	10
	.word	8
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	9
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.word	8
	.word	7
	.data
	.align	2
.LANCHOR2 = . + 0
	.type	default_intra_quantizer_matrix, %object
	.size	default_intra_quantizer_matrix, 64
default_intra_quantizer_matrix:
	.byte	8
	.byte	16
	.byte	19
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	16
	.byte	16
	.byte	22
	.byte	24
	.byte	27
	.byte	29
	.byte	34
	.byte	37
	.byte	19
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	34
	.byte	38
	.byte	22
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	37
	.byte	40
	.byte	22
	.byte	26
	.byte	27
	.byte	29
	.byte	32
	.byte	35
	.byte	40
	.byte	48
	.byte	26
	.byte	27
	.byte	29
	.byte	32
	.byte	35
	.byte	40
	.byte	48
	.byte	58
	.byte	26
	.byte	27
	.byte	29
	.byte	34
	.byte	38
	.byte	46
	.byte	56
	.byte	69
	.byte	27
	.byte	29
	.byte	35
	.byte	38
	.byte	46
	.byte	56
	.byte	69
	.byte	83
	.type	zigzag, %object
	.size	zigzag, 64
zigzag:
	.byte	0
	.byte	1
	.byte	8
	.byte	16
	.byte	9
	.byte	2
	.byte	3
	.byte	10
	.byte	17
	.byte	24
	.byte	32
	.byte	25
	.byte	18
	.byte	11
	.byte	4
	.byte	5
	.byte	12
	.byte	19
	.byte	26
	.byte	33
	.byte	40
	.byte	48
	.byte	41
	.byte	34
	.byte	27
	.byte	20
	.byte	13
	.byte	6
	.byte	7
	.byte	14
	.byte	21
	.byte	28
	.byte	35
	.byte	42
	.byte	49
	.byte	56
	.byte	57
	.byte	50
	.byte	43
	.byte	36
	.byte	29
	.byte	22
	.byte	15
	.byte	23
	.byte	30
	.byte	37
	.byte	44
	.byte	51
	.byte	58
	.byte	59
	.byte	52
	.byte	45
	.byte	38
	.byte	31
	.byte	39
	.byte	46
	.byte	53
	.byte	60
	.byte	61
	.byte	54
	.byte	47
	.byte	55
	.byte	62
	.byte	63
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC1:
	ASCII(.ascii	"image size abnormal(%dx%d)\012\000" )
.LC2:
	ASCII(.ascii	"mp2 actual frame size(%dx%d) exeed max config(%dx%d" )
	ASCII(.ascii	")\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"FSP_ConfigInstance fail!\012\000" )
	.space	2
.LC5:
	ASCII(.ascii	"ERROR: frm %d, integrality: %#x\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"ERROR: frm %d, No Img buffer is allocated\012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"----------------- mp2 Imode -> %d -----------------" )
	ASCII(.ascii	"-\012\000" )
	.space	2
.LC8:
	ASCII(.ascii	"frm %d pic type %d struct is %d total pos %#x\012\000" )
	.space	1
.LC9:
	ASCII(.ascii	"NOTE: Drop non-I, cur is B\012\000" )
.LC10:
	ASCII(.ascii	"NOTE: Drop non-I, cur is P\012\000" )
.LC11:
	ASCII(.ascii	"NOTE: Drop non-I\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"NOTE: Drop B\012\000" )
	.space	2
.LC13:
	ASCII(.ascii	"%s %d pFwdImg = NULL,  pBwdImg = NULL!!\012\000" )
	.space	3
.LC14:
	ASCII(.ascii	"fuction return value is null,%s %d !!\012\000" )
	.space	1
.LC15:
	ASCII(.ascii	"dec P frame, fwd ref err(%d) > ref_thr(%d)\012\000" )
.LC16:
	ASCII(.ascii	"dec B frame, bwd ref err(%d) > ref_thr(%d)\012\000" )
.LC17:
	ASCII(.ascii	"dec B frame, fwd ref err(%d) > ref_thr(%d)\012\000" )
.LC18:
	ASCII(.ascii	"Protocol %d\012\000" )
	.space	3
.LC19:
	ASCII(.ascii	"seq size change, call vpss to release all frames.\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC20:
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC21:
	ASCII(.ascii	"new seq size %dx%d\012\000" )
.LC22:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC25:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"ERROR: frm %d, lost one field\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"ERROR: frm %d, lost one field flg = %d, last_struct" )
	ASCII(.ascii	" = %d\012\000" )
	.space	2
.LC28:
	ASCII(.ascii	"Fatal Error: frm%d, get image buffer failed\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"dec_pts: %lld Usertag: %lld\012\000" )
	.space	3
.LC30:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC31:
	ASCII(.ascii	"++++++++++ ref %d\012\000" )
	.space	1
.LC32:
	ASCII(.ascii	"line: %d(pCur, pFwd, pBwd)(%p, %p, %p)\012\000" )
.LC33:
	ASCII(.ascii	"CurFsID(fwd%d,%d,bwd%d) abnormal!(%p,%p,%p)\012\000" )
	.space	3
.LC34:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,bwd,pmv = %p,%p,%p,%p,%p\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC35:
	ASCII(.ascii	"\012mpeg2 copy last 3 bytes of the first packet!\012" )
	ASCII(.ascii	"\012\000" )
.LC36:
	ASCII(.ascii	"pMp2CodecInfo->pUsrDatArray[%d] is null\012\000" )
	.space	3
.LC37:
	ASCII(.ascii	"ERROR: unsupported profile %d\012\000" )
	.space	1
.LC38:
	ASCII(.ascii	"ERROR: unsupported level %d\012\000" )
	.space	3
.LC39:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC40:
	ASCII(.ascii	"Decoding quits at frm %d\012\000" )
	.space	2
.LC41:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet pointer!\012\000" )
.LC42:
	ASCII(.ascii	"line %d, pPacket->StreamPack[1].IsLastSeg = %d\012\000" )
.LC43:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC44:
	ASCII(.ascii	"pImg is null, pCurImg %#x pBwdImg %#x pFwdImg %#x\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC45:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC46:
	ASCII(.ascii	"err(%d) > out_thr(%d)\012\000" )
	.space	1
.LC47:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.space	2
.LC48:
	ASCII(.ascii	"%-40s%20d\012\000" )
	.space	1
.LC49:
	ASCII(.ascii	"slice_vertical_position\000" )
.LC50:
	ASCII(.ascii	"quantiser_scale_code\000" )
	.space	3
.LC51:
	ASCII(.ascii	"extra_bit_slice\000" )
.LC52:
	ASCII(.ascii	"intra_slice_flag\000" )
	.space	3
.LC53:
	ASCII(.ascii	"intra_slice\000" )
.LC54:
	ASCII(.ascii	"reserved_bits\000" )
	.space	2
.LC55:
	ASCII(.ascii	"extra_infrmation_slice\000" )
	.space	1
.LC56:
	ASCII(.ascii	"Slice num = %d >= %d\012\000" )
	.space	2
.LC57:
	ASCII(.ascii	"seg info is wrong! IsLastSeg[0] = %d, IsLastSeg[1] " )
	ASCII(.ascii	"= %d\012\000" )
	.space	3
.LC58:
	ASCII(.ascii	"macroblock_address_increment\000" )
	.space	3
.LC59:
	ASCII(.ascii	"macroblock_escape ERROR\012\000" )
	.space	3
.LC60:
	ASCII(.ascii	"macroblock_escape\000" )
	.space	2
.LC61:
	ASCII(.ascii	"SlcStartMbNum = %d > PicEndMum = %d\012\000" )
	.space	3
.LC62:
	ASCII(.ascii	"FirstMbInSliceFlag = %d\012\000" )
	.space	3
.LC63:
	ASCII(.ascii	"MP2_GetFirstMbInSlice Failed!\012\000" )
	.space	1
.LC64:
	ASCII(.ascii	"MP2_WriteSliceInfo Failed!\012\000" )
.LC65:
	ASCII(.ascii	"found sh %#x\012\000" )
	.space	2
.LC66:
	ASCII(.ascii	"SEQUENCE_HEADER_CODE Bitstream is not enough!\012\000" )
	.space	1
.LC67:
	ASCII(.ascii	"found gh %#x\012\000" )
	.space	2
.LC68:
	ASCII(.ascii	"GROUP_START_CODE Bitstream is not enough!\012\000" )
	.space	1
.LC69:
	ASCII(.ascii	"found ph %#x\012\000" )
	.space	2
.LC70:
	ASCII(.ascii	"PICTURE_START_CODE Bitstream is not enough!\012\000" )
	.space	3
.LC71:
	ASCII(.ascii	"found ud %#x\012\000" )
	.space	2
.LC72:
	ASCII(.ascii	"cann't get ud pointer\012\000" )
	.space	1
.LC73:
	ASCII(.ascii	"found ext %#x\012\000" )
	.space	1
.LC74:
	ASCII(.ascii	"MP2_DecSlice is wrong!\012\000" )
.LC75:
	ASCII(.ascii	"line %d, MP2_ReceivePacket receive packet failed!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC76:
	ASCII(.ascii	"MP2 Decode Decide is wrong!\012\000" )
	.space	3
.LC77:
	ASCII(.ascii	"MP2 get picture info is wrong!\012\000" )
.LC78:
	ASCII(.ascii	"MP2 dec new pic failed!\012\000" )
	.bss
	.align	2
.LANCHOR1 = . + 0
	.type	g_tmpusd, %object
	.size	g_tmpusd, 1024
g_tmpusd:
	.space	1024
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
