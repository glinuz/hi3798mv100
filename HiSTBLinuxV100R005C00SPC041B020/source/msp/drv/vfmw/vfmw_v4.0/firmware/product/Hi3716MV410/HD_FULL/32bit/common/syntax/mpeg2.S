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
	add	r3, r0, #77824
	mov	r4, r0
	ldr	r3, [r3, #2680]
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
	mov	r6, r3
	mov	r10, #5
	mov	r5, #2
	mov	r9, #3
.L28:
	ldr	r8, .L43
	mov	r2, #32
	mov	r1, #0
	sub	r0, fp, #76
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r2, [r4]
	str	r6, [fp, #-48]
	mov	r3, #0
	str	r7, [fp, #-68]
	ldr	r2, [r2, #28]
	str	r6, [fp, #-64]
	cmp	r2, #24
	str	r7, [fp, #-52]
	strb	r3, [fp, #-75]
	beq	.L33
	uxtb	r5, r5
	strb	r9, [fp, #-72]
	strb	r10, [fp, #-70]
.L36:
	add	r6, r4, #110592
	sub	r1, fp, #76
	strb	r5, [fp, #-69]
	mov	r5, #0
	ldr	r0, [r6, #4080]
	strb	r5, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r7, r0, #0
	bne	.L34
	ldr	r1, [r4]
	sub	r3, fp, #80
	ldr	r0, [r6, #4080]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, r5
	moveq	r0, #1
	beq	.L39
	ldr	r3, [r8, #68]
	mov	r0, r7
	ldr	r1, .L43+4
	blx	r3
	mov	r0, r7
	b	.L39
.L19:
	ldr	ip, .L43
	mov	r0, #0
	ldr	r1, .L43+8
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
.L39:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L34:
	ldr	r3, [r8, #68]
	mov	r0, r5
	ldr	r1, .L43+12
	blx	r3
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L42:
	ldr	r0, [r1, #776]
	cmp	r0, #0
	bne	.L23
	ldr	r7, [r1, #736]
	ldr	r6, [r1, #740]
	cmp	r2, r7
	bhi	.L25
	cmp	r3, r6
	bhi	.L25
	ldr	r3, [r1, #764]
	ldr	r5, [r1, #760]
	cmp	r3, #0
	add	r9, r5, #2
	bne	.L41
.L38:
	mov	r5, #1
.L32:
	ldrb	r10, [r1, #784]	@ zero_extendqisi2
	b	.L28
.L23:
	ldr	r0, [r1, #736]
	ldr	ip, [r1, #740]
	cmp	r2, r0
	bhi	.L30
	cmp	r3, ip
	bhi	.L30
	ldr	r0, [r1, #764]
	mov	r7, r2
	ldr	r5, [r1, #760]
	mov	r6, r3
	cmp	r0, #0
	add	r9, r5, #2
	beq	.L38
.L41:
	add	r5, r5, #1
	cmp	r5, #17
	movge	r5, #17
	b	.L32
.L33:
	strb	r3, [fp, #-72]
	mov	r5, #1
	strb	r10, [fp, #-70]
	b	.L36
.L30:
	ldr	r1, .L43
	stmia	sp, {r0, ip}
.L40:
	ldr	ip, [r1, #68]
	mov	r0, #0
	ldr	r1, .L43+16
	blx	ip
	mov	r0, #0
	b	.L39
.L25:
	ldr	r1, .L43
	str	r6, [sp, #4]
	str	r7, [sp]
	b	.L40
.L44:
	.align	2
.L43:
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
	add	r4, r0, #77824
	mov	r5, r0
	ldr	r3, [r4, #1356]
	ldr	r7, [r4, #2680]
	tst	r3, #8388608
	mvneq	r6, #0
	bne	.L121
.L46:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L121:
	ldrb	r2, [r7, #9]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L47
	ldr	r2, [r4, #1368]
	cmp	r2, #1
	beq	.L47
	ands	r6, r3, #255
	bne	.L122
.L49:
	ldr	r3, [r4, #1404]
	cmp	r3, #0
	beq	.L123
	ldr	r3, [r4, #1344]
	sub	r3, r3, #2
	cmp	r3, #1
	ldr	r3, [r5]
	bls	.L52
	ldr	r10, .L139
	add	r8, r5, #110592
	ldr	r2, [r3, #8]
.L53:
	sub	r3, r2, #1
	str	r2, [r4, #1344]
	cmp	r3, #3
	ldrls	pc, [pc, r3, asl #2]
	b	.L116
.L59:
	.word	.L58
	.word	.L60
	.word	.L61
	.word	.L62
.L122:
	ldr	r2, .L139+4
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L50
	add	r5, r5, #110592
	mov	r3, #0
	mov	r2, r3
	mov	r1, #103
	ldr	r0, [r5, #4080]
	blx	ip
	ldr	r3, [r4, #1356]
.L50:
	ldr	ip, .L139
	mov	r0, #22
	ldr	r2, [r4, #1364]
	mov	r6, #0
	ldr	r1, .L139+8
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L47:
	add	r8, r5, #110592
	ands	r6, r3, #255
	ldr	r2, [r8, #4084]
	add	r2, r2, #1
	str	r2, [r8, #4084]
	beq	.L49
	b	.L122
.L62:
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L124
	cmp	r3, #3
	beq	.L125
.L68:
	cmp	r3, #2
	beq	.L72
.L116:
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	mov	r2, #1
.L119:
	ldrb	r1, [r7, #9]	@ zero_extendqisi2
	str	r2, [fp, #-64]
.L63:
	ldr	r2, [r4, #1304]
	add	r0, r5, #78848
	ldr	r9, [r4, #1408]
	add	r0, r0, #576
	ldr	ip, [r10, #68]
	add	r9, r9, r2
	ldr	r2, [r4, #1364]
	str	r1, [fp, #-60]
	str	r3, [fp, #-56]
	str	ip, [fp, #-52]
	str	r2, [fp, #-48]
	bl	BsPos
	ldr	r1, [fp, #-60]
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-48]
	str	r1, [sp]
	ldr	ip, [fp, #-52]
	ldr	r1, .L139+12
	add	r0, r9, r0, lsr #3
	str	r0, [sp, #4]
	mov	r0, #18
	blx	ip
	ldr	r3, [r4, #1356]
	bic	r3, r3, #14155776
	str	r3, [r4, #1356]
	ldr	r3, [r5]
	ldr	r0, [r3, #692]
	ldr	r3, [r4, #1396]
	cmp	r0, #1
	beq	.L126
	cmp	r3, #0
	bne	.L76
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	sub	r1, r2, #2
	cmp	r1, #1
	bls	.L127
.L77:
	cmp	r2, #3
	beq	.L128
.L74:
	ldr	r1, [r4, #1432]
	ldr	r0, [r8, #4080]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r4, #1436]
	mov	r9, r0
	ldr	r0, [r8, #4080]
	bl	FSP_GetFsImagePtr
	clz	r3, r9
	mov	r3, r3, lsr #5
	clz	r1, r0
	mov	r1, r1, lsr #5
	orrs	r2, r1, r3
	bne	.L129
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	cmp	r2, #2
	beq	.L130
	cmp	r2, #3
	beq	.L131
.L82:
	ldr	r3, [fp, #-64]
	cmp	r3, #0
	movne	r6, #1
	bne	.L46
	ldrb	r3, [r7, #9]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L88
	ldr	r3, [r4, #1368]
	cmp	r3, #0
	bne	.L46
.L88:
	ldrb	r3, [r7, #27]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L132
	ldrb	r3, [r7, #18]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L133
	ldr	r3, [r4, #1520]
	add	r3, r3, #1
	str	r3, [r4, #1520]
	mov	r3, r3, asl #1
	str	r3, [r4, #1516]
.L90:
	ldr	r2, [r4, #1512]
	ldr	r3, [r4, #1364]
	add	r2, r2, #1
	str	r2, [r4, #1512]
	add	r3, r3, #1
	str	r3, [r4, #1364]
	b	.L46
.L58:
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L134
.L118:
	mov	r2, #1
	ldrb	r1, [r7, #9]	@ zero_extendqisi2
	str	r2, [fp, #-64]
	b	.L63
.L61:
	mov	r2, #0
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	str	r2, [fp, #-64]
	ldrb	r1, [r7, #9]	@ zero_extendqisi2
	b	.L63
.L60:
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	sub	r2, r3, #2
	cmp	r2, #1
	bhi	.L118
	cmp	r3, #2
	beq	.L135
.L66:
	ldrb	r1, [r7, #9]	@ zero_extendqisi2
	cmp	r1, #3
	beq	.L136
.L69:
	ldr	r3, [r4, #1368]
	mov	r2, #0
	str	r2, [fp, #-64]
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r4, #1368]
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	ldrb	r1, [r7, #9]	@ zero_extendqisi2
	b	.L63
.L76:
	cmp	r3, #1
	bhi	.L74
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	b	.L77
.L126:
	cmp	r3, #0
	bne	.L74
	ldrb	r2, [r7, #22]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L137
	cmp	r2, #2
	bne	.L74
	ldr	r3, .L139
	ldr	r1, .L139+16
	ldr	r3, [r3, #68]
	blx	r3
	b	.L74
.L52:
	ldr	r2, [r3, #8]
	ldr	r10, .L139
	cmp	r2, #4
	cmpne	r2, #1
	addhi	r8, r5, #110592
	bhi	.L53
	ldr	r1, .L139+20
	mov	r0, #2
	ldr	r3, [r10, #68]
	add	r8, r5, #110592
	blx	r3
	ldr	r3, [r4, #1356]
	ldr	r1, [r4, #1436]
	orr	r3, r3, #128
	str	r3, [r4, #1356]
	ldr	r0, [r8, #4080]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L55
	ldr	r1, [r9, #212]
	ldr	r0, [r8, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r9, #216]
	ldr	r0, [r8, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r9, #220]
	ldr	r0, [r8, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r9, #224]
	ldr	r0, [r8, #4080]
	bl	FreeUsdByDec
	str	r6, [r9, #212]
	str	r6, [r9, #216]
	mov	r2, #1
	str	r6, [r9, #220]
	str	r6, [r9, #224]
	ldr	r1, [r4, #1436]
	ldr	r0, [r8, #4080]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #1432]
	ldr	r0, [r8, #4080]
	bl	FSP_ClearLogicFs
	str	r6, [r4, #1432]
	str	r6, [r4, #1436]
	ldr	r0, [r8, #4080]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L138
.L55:
	mov	r3, #0
	str	r3, [r4, #1396]
	ldr	r3, [r5]
	ldr	r2, [r3, #8]
	b	.L53
.L123:
	ldr	r3, .L139
	mov	r0, #1
	ldr	r2, [r4, #1364]
	ldr	r1, .L139+24
	ldr	r3, [r3, #68]
	blx	r3
	b	.L46
.L130:
	cmp	r3, #0
	bne	.L80
	ldr	r3, [r5]
	ldr	r2, [r9, #200]
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L82
	ldr	ip, [r10, #68]
	mov	r0, #1
	ldr	r1, .L139+28
	blx	ip
	b	.L46
.L129:
	ldr	ip, [r10, #68]
	movw	r3, #926
	ldr	r2, .L139+32
	mov	r0, #1
	ldr	r1, .L139+36
	blx	ip
	b	.L46
.L131:
	cmp	r3, #0
	ldr	r3, [r5]
	ldr	r2, [r9, #200]
	ldr	r3, [r3]
	bne	.L84
	cmp	r2, r3
	bhi	.L84
	cmp	r1, #0
	ldr	r2, [r0, #200]
	bne	.L87
	cmp	r3, r2
	bcs	.L82
.L87:
	ldr	ip, [r10, #68]
	mov	r0, #1
	ldr	r1, .L139+40
	blx	ip
	b	.L46
.L132:
	ldrb	r2, [r7, #16]	@ zero_extendqisi2
	ldrb	r3, [r7, #10]	@ zero_extendqisi2
	ldr	r1, [r4, #1520]
	add	r3, r2, r3
	add	r3, r3, #1
	add	r3, r3, r1
	str	r3, [r4, #1520]
	mov	r3, r3, asl #1
	str	r3, [r4, #1516]
	b	.L90
.L84:
	ldr	ip, [r10, #68]
	mov	r0, #1
	ldr	r1, .L139+44
	blx	ip
	b	.L46
.L125:
	ldr	r2, [r8, #4092]
	cmp	r2, #0
	bne	.L118
.L71:
	ldrb	r1, [r7, #9]	@ zero_extendqisi2
	cmp	r1, #3
	bne	.L69
	mov	r2, #0
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	str	r2, [fp, #-64]
	b	.L63
.L124:
	mov	r3, #0
	str	r3, [r8, #4092]
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L68
	b	.L71
.L134:
	ldrb	r3, [r7, #9]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L69
	mov	r2, #0
	mov	r1, r3
	str	r2, [fp, #-64]
	b	.L63
.L133:
	ldr	r2, [r4, #1520]
	ldr	r3, [r4, #1516]
	add	r2, r2, #1
	str	r2, [r4, #1520]
	ldrb	r2, [r7, #16]	@ zero_extendqisi2
	add	r2, r2, #2
	add	r3, r3, r2
	str	r3, [r4, #1516]
	b	.L90
.L72:
	mov	r3, #1
	str	r3, [r8, #4092]
	str	r3, [fp, #-64]
	ldrb	r1, [r7, #9]	@ zero_extendqisi2
	ldrb	r3, [r7, #22]	@ zero_extendqisi2
	b	.L63
.L127:
	ldr	r2, .L139
	mov	r0, #1
	ldr	r1, .L139+48
	str	r3, [fp, #-64]
	ldr	r2, [r2, #68]
	blx	r2
	b	.L74
.L128:
	mov	r2, #0
	ldr	r3, [r10, #68]
	ldr	r1, .L139+52
	mov	r0, #1
	str	r2, [fp, #-64]
	blx	r3
	b	.L74
.L80:
	ldr	ip, [r10, #68]
	mov	r3, #936
	ldr	r2, .L139+32
	mov	r0, #1
	ldr	r1, .L139+56
	blx	ip
	b	.L46
.L137:
	ldr	r2, .L139
	ldr	r1, .L139+60
	str	r3, [fp, #-64]
	ldr	r2, [r2, #68]
	blx	r2
	b	.L74
.L138:
	ldr	r0, [r8, #4080]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #1432]
	str	r0, [r4, #1436]
	b	.L55
.L135:
	add	r2, r5, #45056
	ldrb	r1, [r2, #273]	@ zero_extendqisi2
	cmp	r1, #3
	beq	.L66
	ldrb	r2, [r2, #272]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L66
	b	.L119
.L136:
	mov	r2, #0
	str	r2, [fp, #-64]
	b	.L63
.L140:
	.align	2
.L139:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, .L147
	sub	r0, fp, #148
	bl	memcpy
	add	r3, r4, #77824
	ldr	r0, [r4, #300]
	ldr	r2, [r3, #1392]
	cmp	r2, #1
	beq	.L142
	ldr	r3, [r3, #2680]
	ldrb	r3, [r3, #13]	@ zero_extendqisi2
	cmp	r3, #0
	subne	r3, fp, #20
	moveq	r0, r0, asl #1
	addne	r0, r3, r0, lsl #2
	ldrne	r0, [r0, #-128]
.L142:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L148:
	.align	2
.L147:
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
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r4, r0, #77824
	mov	r3, #0
	mov	r7, r0
	ldr	r2, [r4, #1392]
	str	r3, [fp, #-64]
	cmp	r2, #1
	ldr	r5, [r4, #2680]
	ldr	r8, [r4, #2684]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	beq	.L151
	cmp	r2, #2
	bne	.L196
	ldr	r3, [r4, #1356]
	and	r3, r3, #1769472
	cmp	r3, #589824
	beq	.L154
.L153:
	ldr	r9, .L205
	mov	r0, #17
	ldr	r1, .L205+4
	ldr	r3, [r9, #68]
	blx	r3
	ldr	r3, [r4, #1392]
	cmp	r3, #1
	beq	.L197
.L156:
	ldr	r3, [r5, #48]
	add	r3, r3, #15
	mov	r3, r3, lsr #4
	str	r3, [r5, #56]
	ldr	r2, [r4, #1392]
	cmp	r2, #2
	beq	.L198
.L157:
	ldr	r2, [r5, #52]
	add	r2, r2, #15
	mov	r2, r2, lsr #4
.L158:
	mov	r1, #0
	str	r2, [r5, #60]
	str	r1, [r5, #64]
	ldr	r1, [r4, #1392]
	cmp	r1, #2
	beq	.L199
.L159:
	ldr	r1, [r4, #1404]
	cmp	r1, #0
	beq	.L160
	ldr	r0, [r8, #56]
	cmp	r3, r0
	beq	.L200
.L160:
	add	r6, r7, #110592
	ldr	r1, [r4, #1436]
	ldr	r0, [r6, #4080]
	bl	FSP_GetFsImagePtr
	subs	r10, r0, #0
	beq	.L163
	ldr	r1, [r10, #212]
	mov	r3, #0
	ldr	r0, [r6, #4080]
	str	r3, [fp, #-72]
	bl	FreeUsdByDec
	ldr	r1, [r10, #216]
	ldr	r0, [r6, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r10, #220]
	ldr	r0, [r6, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r10, #224]
	ldr	r0, [r6, #4080]
	bl	FreeUsdByDec
	ldr	r3, [fp, #-72]
	mov	r2, #1
	str	r3, [r10, #212]
	str	r3, [r10, #216]
	str	r3, [r10, #220]
	str	r3, [r10, #224]
	ldr	r1, [r4, #1436]
	ldr	r0, [r6, #4080]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #1432]
	ldr	r0, [r6, #4080]
	bl	FSP_ClearLogicFs
	ldr	r3, [fp, #-72]
	str	r3, [r4, #1432]
	str	r3, [r4, #1436]
	ldr	r0, [r6, #4080]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L201
.L163:
	add	r10, r7, #79872
	mov	r3, #0
	add	r10, r10, #640
	str	r3, [r4, #1396]
	ldr	r0, [r6, #4080]
	mov	r1, r10
	bl	FSP_ClearNotInVoQueue
	ldr	r3, [r7]
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L165
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L202
.L165:
	ldr	r3, [r4, #1404]
	cmp	r3, #0
	beq	.L168
	ldr	r3, [r8, #56]
	cmp	r3, #0
	beq	.L168
	ldr	r3, [r8, #60]
	cmp	r3, #0
	bne	.L203
.L168:
	ldr	r0, [r6, #4080]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L170
	sub	r3, fp, #56
	sub	r2, fp, #60
	sub	r1, fp, #64
	ldr	r0, [r6, #4080]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r7]
	ldr	r3, [r3, #776]
	cmp	r3, #1
	beq	.L204
.L171:
	mov	r0, r10
	bl	ResetVoQueue
	add	r3, r7, #114688
	mov	r2, #0
	strb	r2, [r3, #1]
.L170:
	mov	r3, #0
	mov	r0, r7
	mov	r1, r3
	str	r3, [r4, #1404]
	str	r3, [r4, #1420]
	bl	MP2_ArrangeVHBMem
	ldr	ip, [r9, #68]
	ldr	r1, .L205+8
	str	r0, [r4, #1404]
	mov	r0, #1
	ldr	r3, [r5, #60]
	ldr	r2, [r5, #56]
	mov	r3, r3, asl #4
	mov	r2, r2, asl #4
	blx	ip
.L166:
	ldr	r3, .L205+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L194
	ldr	r9, [r8, #56]
	mov	r3, #8
	ldr	r8, [r8, #60]
	sub	r2, fp, #52
	ldr	r7, [r5, #56]
	mov	r1, #2
	ldr	lr, [r5, #60]
	ldr	r0, [r6, #4080]
	strh	r9, [fp, #-52]	@ movhi
	strh	r8, [fp, #-50]	@ movhi
	strh	r7, [fp, #-48]	@ movhi
	strh	lr, [fp, #-46]	@ movhi
	blx	ip
.L194:
	ldr	r1, [r4, #1404]
.L161:
	str	r1, [r4, #1352]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L196:
	ldr	r3, [r4, #1356]
	tst	r3, #3538944
	beq	.L154
	tst	r3, #131072
	mov	r2, #2
	orreq	r3, r3, r2
	streq	r3, [r4, #1356]
	tst	r3, #1048576
	str	r2, [r4, #1392]
	orreq	r3, r3, #16
	streq	r3, [r4, #1356]
	moveq	r2, #2
	movne	r2, #2
	b	.L153
.L154:
	mov	r2, #1
	str	r2, [r4, #1392]
	b	.L153
.L151:
	ldr	r3, [r4, #1356]
	mov	r1, #18
	bfi	r1, r1, #16, #16
	and	r3, r3, r1
	cmp	r3, #1179648
	moveq	r2, #2
	streq	r2, [r4, #1392]
	b	.L153
.L199:
	ldrb	r1, [r5, #27]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L159
	ldr	r1, [r5, #52]
	add	r0, r1, #31
	add	r1, r1, #15
	mov	r0, r0, lsr #5
	mov	r1, r1, lsr #4
	rsb	r1, r1, r0, lsl #1
	str	r1, [r5, #64]
	b	.L159
.L198:
	ldrb	r2, [r5, #27]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L157
	ldr	r2, [r5, #52]
	add	r2, r2, #31
	mov	r2, r2, lsr #5
	mov	r2, r2, asl #1
	b	.L158
.L197:
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
	b	.L156
.L203:
	mov	r3, #0
	ldr	r2, [r9, #68]
	str	r3, [r4, #1404]
	mov	r0, #1
	ldr	r1, .L205+16
	str	r3, [fp, #-72]
	blx	r2
	ldr	r2, .L205+12
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L168
	ldr	r3, [fp, #-72]
	mov	r1, #120
	ldr	r0, [r6, #4080]
	mov	r2, r3
	blx	ip
	b	.L168
.L200:
	ldr	r3, [r8, #60]
	cmp	r2, r3
	bne	.L160
	b	.L161
.L204:
	ldr	r2, [fp, #-60]
	ldr	r1, [fp, #-56]
	add	r2, r2, r1
	cmp	r2, #0
	ble	.L171
	add	r7, r7, #114688
	strb	r3, [r7, #1]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L201:
	ldr	r0, [r6, #4080]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #1432]
	str	r0, [r4, #1436]
	b	.L163
.L202:
	ldr	r9, [r4, #1404]
	cmp	r9, #0
	bne	.L166
	mov	r0, r10
	bl	ResetVoQueue
	mov	r1, r9
	mov	r0, r7
	bl	MP2_ArrangeVHBMem
	str	r9, [r4, #1420]
	str	r0, [r4, #1404]
	b	.L166
.L206:
	.align	2
.L205:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC18
	.word	.LC20
	.word	g_event_report
	.word	.LC19
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
	add	r4, r0, #77824
	ldr	r3, [r4, #2680]
	ldr	r5, [r4, #1496]
	ldr	lr, [r4, #1508]
	ldrb	r2, [r3, #22]	@ zero_extendqisi2
	ldr	ip, [r3, #72]
	cmp	r2, #1
	ldr	r1, [r4, #1504]
	ldr	r2, [r4, #1492]
	ldr	r6, [r4, #1488]
	ldr	r0, [r4, #1500]
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
	streq	r2, [r4, #1524]
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
	str	r0, [r4, #1524]
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
	add	r0, r0, #77824
	ldr	r3, [r0, #2680]
	ldr	r3, [r3, #84]
	sub	r3, r3, #1
	cmp	r3, #2
	ldrls	r2, .L215
	addls	r3, r2, r3, lsl #2
	ldrls	r0, [r3, #148]
	movhi	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L216:
	.align	2
.L215:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MP2_CalcDar, .-MP2_CalcDar
	.align	2
	.global	MP2_SetImgFormat
	.type	MP2_SetImgFormat, %function
MP2_SetImgFormat:
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
	add	r7, r0, #77824
	add	r3, r0, #110592
	mov	r6, r0
	ldr	r1, [r7, #1440]
	ldr	r0, [r3, #4080]
	ldr	r5, [r7, #2680]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldrb	r3, [r5, #22]	@ zero_extendqisi2
	ldrb	r1, [r5, #27]	@ zero_extendqisi2
	cmp	r3, #4
	ldr	r2, [r4, #160]
	subne	r3, r3, #1
	ldrb	lr, [r5, #9]	@ zero_extendqisi2
	andne	r3, r3, #3
	moveq	r3, #0
	cmp	r1, #0
	ubfx	r2, r2, #10, #2
	orr	r2, r2, lr
	ldr	ip, [r5, #128]
	moveq	r0, #1
	movne	r0, #0
	movne	lr, #4096
	beq	.L243
.L220:
	ldr	r1, [r7, #2680]
	and	ip, ip, #3
	and	r9, r2, #3
	mov	ip, ip, asl #5
	ldr	r1, [r1, #84]
	orr	ip, ip, r0, asl #8
	orr	r2, ip, r9, asl #10
	str	r3, [r4, #152]
	sub	r1, r1, #1
	str	r3, [r4, #156]
	cmp	r1, #2
	orr	r3, r2, r3
	orr	r3, r3, lr
	mov	r0, r4
	ldrls	r8, .L246
	movhi	r1, #0
	movhi	r8, r1
	addls	r1, r8, r1, lsl #2
	ldrls	r1, [r1, #148]
	andls	r8, r1, #7
	movls	r8, r8, asl #14
	orr	r8, r3, r8
	str	r8, [r4, #160]
	bl	SetAspectRatio
	mov	r3, #3
	str	r3, [r4, #384]
	mov	r2, #1
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	strb	r2, [r4, #379]
	cmp	r3, #3
	moveq	r3, #0
	movne	r3, #4
	strb	r3, [r4, #372]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	streqb	r3, [r4, #379]
	ldrneb	r3, [r3, #144]	@ zero_extendqisi2
	moveq	r2, #32
	moveq	r3, r2
	streq	r2, [r4, #388]
	mov	r2, #1
	strb	r2, [r4, #377]
	strne	r3, [r4, #388]
	str	r3, [r4, #396]
	str	r3, [r4, #392]
	ldr	r3, [r5, #72]
	str	r3, [r4, #188]
	ldr	r2, [r5, #48]
	str	r2, [r4, #164]
	ldr	r3, [r5, #52]
	str	r3, [r4, #168]
	ldr	r1, [r6]
	ldr	r1, [r1, #28]
	cmp	r1, #25
	beq	.L244
.L227:
	mov	r1, r3, lsr #1
	mov	r0, r2, lsr #1
	str	r1, [r4, #184]
	str	r0, [r4, #180]
	ldr	r1, [r5, #148]
	cmp	r1, #0
	beq	.L228
	ldr	r0, [r5, #152]
	cmp	r0, #0
	beq	.L228
	cmp	r2, r1
	movcs	r2, r1
	str	r2, [r4, #172]
	ldr	r2, [r5, #152]
	cmp	r3, r2
	movcs	r3, r2
	str	r3, [r4, #176]
	b	.L229
.L228:
	str	r2, [r4, #172]
	str	r3, [r4, #176]
.L229:
	ldrb	r3, [r5, #27]	@ zero_extendqisi2
	mov	r0, r6
	str	r3, [r4, #484]
	ldrb	r3, [r5, #18]	@ zero_extendqisi2
	str	r3, [r4, #488]
	ldr	r3, [r5, #144]
	str	r3, [r4, #496]
	bl	MP2_CalcRealEncFrameRate
	ldr	r3, [r7, #1524]
	str	r3, [r4, #492]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L243:
	cmp	lr, #3
	beq	.L245
	ldr	r1, [r7, #1368]
	cmp	r1, #0
	bne	.L222
	sub	lr, lr, #1
	clz	lr, lr
	mov	lr, lr, lsr #5
	mov	lr, lr, asl #12
	b	.L220
.L222:
	subs	lr, lr, #1
	movne	lr, #1
	mov	lr, lr, asl #12
	b	.L220
.L245:
	ldrb	lr, [r5, #10]	@ zero_extendqisi2
	and	lr, lr, #1
	mov	lr, lr, asl #12
	b	.L220
.L244:
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
	b	.L227
.L247:
	.align	2
.L246:
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
	ble	.L249
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MP2_SearchStartCode.part.1
.L249:
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #77824
	ldr	r3, [r4, #1392]
	ldr	r1, [r4, #2680]
	cmp	r3, #1
	beq	.L252
	cmp	r3, #2
	beq	.L253
	ldr	r3, [r4, #1356]
	orr	r3, r3, #63
	str	r3, [r4, #1356]
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
.L259:
	tst	r3, #24
	beq	.L261
.L277:
	cmp	r2, #3
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L262:
	add	r5, r0, #110592
	orr	r3, r3, #64
	ldr	r1, [r4, #1436]
	str	r3, [r4, #1356]
	ldr	r0, [r5, #4080]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L265
	ldr	r1, [r6, #212]
	mov	r7, #0
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r6, #216]
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r6, #220]
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r6, #224]
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	str	r7, [r6, #212]
	str	r7, [r6, #216]
	mov	r2, #1
	str	r7, [r6, #220]
	str	r7, [r6, #224]
	ldr	r1, [r4, #1436]
	ldr	r0, [r5, #4080]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r4, #1432]
	ldr	r0, [r5, #4080]
	bl	FSP_ClearLogicFs
	str	r7, [r4, #1432]
	str	r7, [r4, #1436]
	ldr	r0, [r5, #4080]
	bl	FSP_GetFspType
	cmp	r0, r7
	bne	.L276
.L265:
	mov	r3, #0
	str	r3, [r4, #1396]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L253:
	ldr	r3, [r4, #1356]
	tst	r3, #1048576
	orreq	r3, r3, #16
	streq	r3, [r4, #1356]
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	sub	ip, r2, #1
	cmp	ip, #2
	bls	.L259
.L274:
	orr	r3, r3, #8
	str	r3, [r4, #1356]
	tst	r3, #24
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	bne	.L277
.L261:
	cmp	r2, #1
	biceq	r3, r3, #192
	streq	r3, [r4, #1356]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L252:
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	sub	r3, r2, #1
	cmp	r3, #3
	ldr	r3, [r4, #1356]
	orrhi	r3, r3, #8
	strhi	r3, [r4, #1356]
	ldrhib	r2, [r1, #22]	@ zero_extendqisi2
	sub	ip, r2, #2
	cmp	ip, #1
	bls	.L278
.L256:
	cmp	r2, #3
	beq	.L279
	tst	r3, #24
	bne	.L262
	b	.L261
.L276:
	ldr	r0, [r5, #4080]
	bl	FSP_GetNullLogicFs
	str	r0, [r4, #1432]
	str	r0, [r4, #1436]
	b	.L265
.L279:
	ldrb	r2, [r1, #26]	@ zero_extendqisi2
	cmp	r2, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	b	.L274
.L278:
	ldrb	ip, [r1, #24]	@ zero_extendqisi2
	cmp	ip, #0
	orreq	r3, r3, #8
	streq	r3, [r4, #1356]
	ldreqb	r2, [r1, #22]	@ zero_extendqisi2
	b	.L256
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r5, r0, #110592
	add	r6, r0, #77824
	mov	r4, r0
	mov	r1, #1
	ldr	r0, [r5, #4080]
	bl	FSP_NewLogicFs
	cmp	r0, #0
	str	r0, [r6, #1440]
	blt	.L293
	mov	r1, r0
	ldr	r0, [r5, #4080]
	bl	FSP_GetLogicFs
	subs	r4, r0, #0
	beq	.L294
	ldr	r5, .L296
	mov	r0, #18
	ldr	r2, [r6, #1440]
	ldr	r1, .L296+4
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r2, [r4, #520]
	cmp	r2, #0
	beq	.L289
	ldr	r3, [r4, #524]
	cmp	r3, #0
	beq	.L289
	ldr	r1, [r4, #528]
	cmp	r1, #0
	beq	.L289
	ldr	lr, [r1, #12]
	mov	r0, #18
	ldr	r3, [r3, #12]
	ldr	r2, [r2, #12]
	ldr	ip, [r5, #68]
	ldr	r1, .L296+8
	str	lr, [sp]
	blx	ip
	mov	r0, #1
.L285:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L289:
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L293:
	ldr	r3, .L296
	mov	r0, #0
	ldr	r1, .L296+12
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r6, #1436]
	ldr	r0, [r5, #4080]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L283
	ldr	r1, [r7, #212]
	mov	r8, #0
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r7, #216]
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r7, #220]
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r7, #224]
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	str	r8, [r7, #212]
	str	r8, [r7, #216]
	mov	r2, #1
	str	r8, [r7, #220]
	str	r8, [r7, #224]
	ldr	r1, [r6, #1436]
	ldr	r0, [r5, #4080]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r6, #1432]
	ldr	r0, [r5, #4080]
	bl	FSP_ClearLogicFs
	str	r8, [r6, #1432]
	str	r8, [r6, #1436]
	ldr	r0, [r5, #4080]
	bl	FSP_GetFspType
	cmp	r0, r8
	bne	.L295
.L283:
	add	r1, r4, #79872
	mov	r4, #0
	add	r1, r1, #640
	str	r4, [r6, #1396]
	ldr	r0, [r5, #4080]
	bl	FSP_ClearNotInVoQueue
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L295:
	ldr	r0, [r5, #4080]
	bl	FSP_GetNullLogicFs
	str	r0, [r6, #1432]
	str	r0, [r6, #1436]
	b	.L283
.L294:
	ldr	r3, .L296
	mov	r2, #1904
	ldr	r1, .L296+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	b	.L285
.L297:
	.align	2
.L296:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC23
	.word	.LC24
	.word	.LC21
	.word	.LC22
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
	add	r7, r0, #77824
	mov	r4, r0
	ldr	r3, [r7, #1392]
	ldr	r5, [r7, #2680]
	cmp	r3, #1
	ldr	r6, [r7, #2684]
	beq	.L359
.L299:
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L300
	ldr	r2, [r7, #1368]
	ldr	r1, [r7, #1400]
	clz	r3, r2
	ldr	r8, .L364
	mov	r3, r3, lsr #5
	str	r3, [r7, #1368]
	ldr	r2, [r5, #156]
	cmp	r2, r1
	moveq	r3, #0
	andne	r3, r3, #1
	cmp	r3, #0
	bne	.L360
.L355:
	add	r3, r4, #45056
	str	r3, [fp, #-48]
.L302:
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	ldr	r2, [fp, #-48]
	strb	r3, [r2, #273]
	ldr	r3, [r5, #156]
	str	r3, [r7, #1400]
	ldr	r3, [r5, #56]
	strh	r3, [r4, #160]	@ movhi
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #3
	ldr	r3, [r5, #60]
	ldreq	r2, [r5, #64]
	movne	r3, r3, lsr #1
	rsbeq	r3, r2, r3
	strh	r3, [r4, #156]	@ movhi
	ldr	r3, [r5, #60]
	strh	r3, [r4, #158]	@ movhi
	ldrb	r3, [r5, #11]	@ zero_extendqisi2
	strb	r3, [r4, #13]
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	strb	r3, [r4, #11]
	ldr	r2, [r7, #1368]
	strb	r2, [r4, #12]
	ldrb	r3, [r5, #12]	@ zero_extendqisi2
	strb	r3, [r4, #15]
	ldrb	r3, [r5, #22]	@ zero_extendqisi2
	strb	r3, [r4, #8]
	ldr	r3, [r7, #1392]
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r4, #188]
	cmp	r3, #0
	ldrneb	r3, [r5, #25]	@ zero_extendqisi2
	strneb	r3, [r4, #9]
	ldrneb	r3, [r5, #23]	@ zero_extendqisi2
	strneb	r3, [r4, #10]
	ldrb	r3, [r5, #4]	@ zero_extendqisi2
	strb	r3, [r4, #20]
	ldrb	r3, [r5, #5]	@ zero_extendqisi2
	strb	r3, [r4, #21]
	ldrb	r3, [r5, #6]	@ zero_extendqisi2
	strb	r3, [r4, #22]
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	strb	r3, [r4, #23]
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	strb	r3, [r4, #14]
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	strb	r3, [r4, #19]
	ldrb	r3, [r5, #13]	@ zero_extendqisi2
	strb	r3, [r4, #18]
	ldrb	r3, [r5, #14]	@ zero_extendqisi2
	strb	r3, [r4, #17]
	ldrb	r3, [r5, #15]	@ zero_extendqisi2
	strb	r3, [r4, #16]
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r2, #0
	cmpne	r3, #3
	addne	r10, r4, #110592
	bne	.L311
	mov	r1, #0
	mov	r0, r4
	bl	MP2_GetImageBuffer
	subs	r10, r0, #0
	beq	.L361
	add	r10, r4, #110592
	ldr	r1, [r7, #1440]
	ldr	r0, [r10, #4080]
	bl	FSP_GetFsImagePtr
	subs	r6, r0, #0
	beq	.L314
	ldr	lr, [r4]
	mov	r0, #29
	ldr	r1, .L364+4
	ldrd	r2, [lr, #48]
	strd	r2, [r6, #8]
	ldr	lr, [r4]
	ldrd	r2, [lr, #64]
	strd	r2, [r6, #16]
	ldr	lr, [r4]
	ldrd	r2, [lr, #80]
	strd	r2, [r6, #24]
	mvn	r2, #0
	ldr	lr, [r4]
	mvn	r3, #0
	ldr	lr, [lr, #96]
	str	lr, [r6, #68]
	ldr	lr, [r4]
	ldr	lr, [lr, #100]
	str	lr, [r6, #72]
	ldr	lr, [r4]
	ldr	lr, [lr, #104]
	str	lr, [r6, #76]
	ldr	lr, [r4]
	ldr	lr, [lr, #120]
	str	lr, [r6, #80]
	ldr	lr, [r4]
	strd	r2, [lr, #48]
	ldrd	r2, [r6, #8]
	ldr	r9, [r8, #68]
	strd	r2, [fp, #-60]
	ldrd	r2, [r6, #16]
	strd	r2, [sp]
	ldrd	r2, [fp, #-60]
	blx	r9
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	subs	r3, r3, #3
	movne	r3, #1
	str	r3, [r6, #148]
.L314:
	ldrb	r3, [r5, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L311
	ldr	r3, [r7, #1396]
	ldr	r2, [r7, #1432]
	cmp	r3, #0
	ldr	r3, [r7, #1436]
	movne	r1, #2
	moveq	r1, #1
	cmp	r2, r3
	str	r1, [r7, #1396]
	beq	.L317
	ldr	r3, [r8, #68]
	mov	r0, #13
	ldr	r1, .L364+8
	blx	r3
	mov	r2, #0
	ldr	r1, [r7, #1432]
	ldr	r0, [r10, #4080]
	bl	FSP_SetRef
	ldr	r3, [r7, #1436]
.L317:
	ldr	r1, [r7, #1440]
	mov	r2, #1
	str	r3, [r7, #1432]
	str	r1, [r7, #1436]
	ldr	r0, [r10, #4080]
	bl	FSP_SetRef
	ldr	r1, [r7, #1432]
	ldr	r0, [r10, #4080]
	bl	FSP_RemovePmv
	ldr	r3, [r8, #68]
	ldr	r2, [r7, #1440]
	mov	r0, #13
	ldr	r1, .L364+12
	blx	r3
.L311:
	ldr	r1, [r7, #1440]
	ldr	r0, [r10, #4080]
	bl	FSP_GetLogicFs
	ldr	r1, [r7, #1432]
	mov	r5, r0
	ldr	r0, [r10, #4080]
	bl	FSP_GetLogicFs
	ldr	r1, [r7, #1436]
	mov	r6, r0
	ldr	r0, [r10, #4080]
	bl	FSP_GetLogicFs
	cmp	r5, #0
	cmpne	r6, #0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	mov	r9, r0
	cmp	r3, #0
	bne	.L362
	ldr	r2, [r5, #520]
	cmp	r2, #0
	beq	.L319
	ldr	r3, [r5, #528]
	cmp	r3, #0
	beq	.L319
	ldr	r3, [r5, #524]
	cmp	r3, #0
	beq	.L319
	ldr	r3, [r0, #520]
	ldr	r8, [r8, #68]
	cmp	r3, #0
	ldr	r1, .L364+16
	ldrne	lr, [r3, #12]
	ldreq	lr, [r2, #12]
	str	lr, [r4, #200]
	ldr	r3, [r6, #520]
	cmp	r3, #0
	ldreq	r3, [r5, #520]
	ldr	ip, [r3, #12]
	str	ip, [r4, #204]
	ldr	r3, [r5, #520]
	ldr	r2, [r3, #12]
	str	r2, [r4, #208]
	ldr	r10, [r5, #532]
	str	r10, [r4, #248]
	ldr	r3, [r0, #528]
	mov	r0, #13
	cmp	r3, #0
	ldreq	r3, [r5, #528]
	ldr	r3, [r3, #12]
	str	r3, [r4, #220]
	ldr	r3, [r6, #528]
	cmp	r3, #0
	ldreq	r3, [r5, #528]
	ldr	r3, [r3, #12]
	str	r3, [r4, #224]
	ldr	r3, [r5, #528]
	ldr	r3, [r3, #12]
	str	r3, [r4, #228]
	ldr	r3, [r5, #524]
	ldr	r3, [r3, #12]
	str	r3, [r4, #232]
	ldr	r9, [r9, #156]
	str	r9, [r4, #212]
	ldr	r6, [r6, #156]
	str	r6, [r4, #216]
	ldr	r6, [r5, #528]
	ldr	r6, [r6, #12]
	str	lr, [sp, #4]
	ldr	lr, [fp, #-48]
	str	r10, [sp, #8]
	str	r6, [lr, #264]
	str	ip, [sp]
	blx	r8
	ldr	r3, [r5, #524]
	add	r2, r4, #78848
	add	ip, r4, #87
	add	r0, r2, #604
	add	r1, r4, #23
	ldr	lr, [r3, #20]
	add	r2, r2, #860
	mov	r3, #0
	str	lr, [r4, #240]
.L329:
	ldr	lr, [r0, #4]!
	add	r3, r3, #1
	cmp	r3, #64
	strb	lr, [ip, #1]!
	ldr	lr, [r2, #4]!
	strb	lr, [r1, #1]!
	bne	.L329
	mov	r0, r4
	mov	r10, #1
	bl	MP2_EstimatePicQp
	strb	r0, [r4, #152]
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L361:
	ldr	r3, [r8, #68]
	ldr	r2, [r7, #1364]
	ldr	r1, .L364+20
	blx	r3
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L300:
	add	r3, r4, #45056
	str	r3, [fp, #-48]
	ldrb	r1, [r3, #273]	@ zero_extendqisi2
	cmp	r1, #3
	ldreq	r8, .L364
	beq	.L305
	ldr	r3, [r7, #1368]
	ldr	r8, .L364
	cmp	r3, #0
	beq	.L363
.L305:
	mov	r3, #1
	str	r3, [r7, #1368]
	b	.L302
.L359:
	ldrb	r2, [r5, #26]	@ zero_extendqisi2
	ldrb	r3, [r5, #24]	@ zero_extendqisi2
	strb	r2, [r5, #7]
	strb	r2, [r5, #6]
	strb	r3, [r5, #5]
	strb	r3, [r5, #4]
	b	.L299
.L362:
	ldr	r1, [r7, #1440]
	mov	r2, #1
	ldr	r0, [r10, #4080]
	mov	r10, #0
	bl	FSP_ClearLogicFs
	stmia	sp, {r6, r9}
	mov	r3, r5
	ldr	ip, [r8, #68]
	movw	r2, #1227
	ldr	r1, .L364+24
	mov	r0, r10
	blx	ip
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L363:
	add	r10, r4, #110592
	ldr	r2, [r7, #1364]
	mov	r0, #1
	str	r1, [sp]
	ldr	r1, .L364+28
	ldr	r9, [r8, #68]
	blx	r9
	ldr	r0, [r10, #4080]
	mov	r2, #1
	ldr	r1, [r7, #1440]
	bl	FSP_ClearLogicFs
	ldrb	r3, [r6, #22]	@ zero_extendqisi2
	cmp	r3, #3
	ldrne	r3, [r7, #1356]
	orrne	r3, r3, #192
	strne	r3, [r7, #1356]
	b	.L305
.L319:
	ldr	r0, [r10, #4080]
	mov	r2, #1
	ldr	r1, [r7, #1440]
	mov	r10, #0
	bl	FSP_ClearLogicFs
	ldr	r1, [r5, #524]
	ldr	r3, [r7, #1440]
	mov	r0, r10
	ldr	r2, [r7, #1432]
	str	r1, [sp, #12]
	ldr	r1, [r5, #528]
	ldr	r4, [r7, #1436]
	ldr	ip, [r8, #68]
	str	r1, [sp, #8]
	ldr	lr, [r5, #520]
	ldr	r1, .L364+32
	stmia	sp, {r4, lr}
	blx	ip
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L360:
	add	r10, r4, #110592
	ldr	r2, [r7, #1364]
	ldr	r1, .L364+36
	mov	r0, #1
	ldr	r3, [r8, #68]
	blx	r3
	mov	r3, #0
	mov	r2, #1
	str	r3, [r7, #1368]
	ldr	r1, [r7, #1440]
	ldr	r0, [r10, #4080]
	bl	FSP_ClearLogicFs
	ldrb	r3, [r6, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L355
	ldr	r3, [r7, #1356]
	add	r2, r4, #45056
	str	r2, [fp, #-48]
	orr	r3, r3, #192
	str	r3, [r7, #1356]
	b	.L302
.L365:
	.align	2
.L364:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC33
	.word	.LC27
	.word	.LC31
	.word	.LC26
	.word	.LC32
	.word	.LC25
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
	add	r5, r0, #77824
	mov	r4, r0
	add	r0, r0, #78848
	ldrb	r3, [r5, #1308]	@ zero_extendqisi2
	add	r0, r0, #576
	ldr	lr, [r5, #1320]
	cmp	r3, #1
	ldr	r3, [r5, #1624]
	ldr	ip, [r5, #1304]
	add	r1, r3, #7
	beq	.L369
	cmp	r3, #0
	add	r2, lr, r3, lsr #3
	ldr	r6, [r5, #1324]
	bic	r2, r2, #3
	movge	r1, r3
	add	r3, r3, lr, lsl #3
	and	r3, r3, #31
	add	r1, ip, r1, asr #3
	str	r1, [r5, #1304]
	bic	r6, r6, #3
	str	r3, [r4, #172]
	str	r2, [r4, #164]
	bl	BsResidBits
	str	r6, [r4, #168]
	str	r0, [r4, #180]
	ldr	r3, [r5, #1324]
	and	r3, r3, #3
	mov	r3, r3, asl #3
	str	r3, [r4, #176]
	ldr	r3, [r5, #1332]
	mov	r3, r3, asl #3
	str	r3, [r4, #184]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L369:
	cmp	r3, #0
	add	r2, lr, r3, lsr #3
	bic	r2, r2, #3
	movge	r1, r3
	add	r3, r3, lr, lsl #3
	and	r3, r3, #31
	add	r1, ip, r1, asr #3
	str	r1, [r5, #1304]
	str	r3, [r4, #172]
	str	r2, [r4, #164]
	bl	BsResidBits
	mov	r3, #0
	str	r3, [r4, #168]
	str	r3, [r4, #176]
	str	r3, [r4, #184]
	str	r0, [r4, #180]
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
	ldr	r2, [r4]
	add	r1, r4, #45056
	add	r3, r4, #77824
	ldr	r0, [r2, #676]
	str	r0, [r1, #268]
	ldr	r1, [r3, #2680]
	ldrb	r0, [r1, #22]	@ zero_extendqisi2
	cmp	r0, #3
	beq	.L388
	cmp	r0, #2
	addeq	r0, r4, #110592
	moveq	ip, #1
	streq	ip, [r0, #4092]
.L372:
	add	ip, r4, #8
	str	ip, [r4, #4]
	ldr	r0, [r2, #884]
	cmp	r0, #1
	beq	.L389
	cmp	r0, #2
	beq	.L390
.L375:
	ldr	r2, [r3, #1356]
	bic	r2, r2, #16252928
	str	r2, [r3, #1356]
	ldrb	r5, [r1, #22]	@ zero_extendqisi2
	cmp	r5, #1
	movne	r0, #1
	beq	.L391
.L373:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L391:
	ldr	r3, .L392
	ldr	r6, [r3]
	cmp	r6, #0
	moveq	r0, r5
	beq	.L373
	ldr	r1, [r4, #180]
	add	r3, r4, #110592
	ldr	ip, [r4, #184]
	sub	r2, fp, #28
	ldr	r0, [r3, #4080]
	mov	r3, #4
	add	ip, r1, ip
	mov	r1, #20
	add	lr, ip, #7
	cmp	ip, #0
	movlt	ip, lr
	mov	ip, ip, asr #3
	str	ip, [r2, #-4]!
	blx	r6
	mov	r0, r5
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L388:
	add	r0, r4, #110592
	ldr	r0, [r0, #4092]
	cmp	r0, #0
	bne	.L372
	ldr	ip, [r3, #1396]
	cmp	ip, #2
	beq	.L372
	ldrb	r2, [r1, #9]	@ zero_extendqisi2
	cmp	r2, #3
	ldrne	r2, [r3, #1368]
	clzne	r2, r2
	movne	r2, r2, lsr #5
	strne	r2, [r3, #1368]
	b	.L373
.L389:
	cmp	ip, #0
	beq	.L375
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	cmp	r2, #3
	moveq	r0, #0
	streq	r0, [r4, #4]
	bne	.L375
	b	.L373
.L390:
	ldr	r0, [r2, #888]
	cmp	ip, #0
	cmpne	r0, #0
	ble	.L375
	ldrb	r0, [r1, #22]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L375
	mov	r0, #0
	str	r0, [r4, #4]
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	b	.L373
.L393:
	.align	2
.L392:
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
	add	ip, r0, #77824
	ldr	r3, [r0]
	add	r0, r0, #110592
	ldr	r1, [ip, #2680]
	ldr	r2, [r1, #48]
	str	r2, [r3, #560]
	ldr	r2, [r1, #52]
	str	r2, [r3, #564]
	ldr	r2, [ip, #2680]
	ldr	r2, [r2, #84]
	sub	r2, r2, #1
	cmp	r2, #2
	ldrls	lr, .L399
	movhi	r2, #0
	addls	r2, lr, r2, lsl #2
	ldrls	r2, [r2, #148]
	str	r2, [r3, #568]
	mov	r2, #0
	str	r2, [r3, #572]
	str	r2, [r3, #576]
	ldr	lr, [r1, #72]
	str	r2, [r3, #584]
	str	lr, [r3, #580]
	ldr	ip, [ip, #1392]
	sub	ip, ip, #1
	clz	ip, ip
	mov	ip, ip, lsr #5
	str	ip, [r3, #588]
	ldr	ip, [r1, #40]
	str	ip, [r3, #592]
	ldr	r1, [r1, #44]
	str	r1, [r3, #596]
	ldr	r1, [r0, #4084]
	str	r1, [r3, #600]
	ldr	ip, [r0, #4084]
	ldr	r1, [r0, #4088]
	cmp	ip, r1
	rsbge	r2, r1, ip
	str	r2, [r3, #604]
	ldmfd	sp, {fp, sp, pc}
.L400:
	.align	2
.L399:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MP2_RecordVFMWState, .-MP2_RecordVFMWState
	.align	2
	.global	MP2_GetUsrData
	.type	MP2_GetUsrData, %function
MP2_GetUsrData:
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
	add	r7, r0, #78848
	add	r4, r7, #576
	mov	r6, r0
	mov	r0, r4
	bl	BsToNextByte
	mov	r0, r4
	bl	BsResidBits
	add	r9, r0, #7
	cmp	r0, #0
	movlt	r0, r9
	mov	r9, r0, asr #3
	cmp	r9, #0
	ble	.L421
	mov	r5, #0
	ldr	r10, .L439
	mov	r8, r5
	b	.L406
.L436:
	bl	BsGet
	add	r8, r8, #1
	cmp	r8, r9
	strb	r0, [r10, r5]
	add	r5, r5, #1
	beq	.L402
.L406:
	mov	r1, #24
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	bic	r3, r0, #-16777216
	mov	r0, r4
	cmp	r3, #1
	beq	.L403
	cmp	r5, #1024
	blt	.L436
	mov	r1, #8
	mov	r0, r4
	add	r8, r8, #1
	bl	BsGet
	cmp	r8, r9
	bne	.L406
.L402:
	add	r9, r6, #77824
	movw	r3, #1308
	ldrsb	r8, [r9, r3]
	cmp	r8, #0
	beq	.L418
	mov	r3, #0
	str	r3, [fp, #-52]
	ldr	r3, .L439+4
	str	r3, [fp, #-48]
.L407:
	ldr	r4, [r9, #1388]
	cmp	r4, #3
	addle	r8, r6, #110592
	ble	.L420
.L419:
	add	r10, r7, #344
	add	r8, r6, #110592
	add	r7, r7, #360
	mov	r4, #0
.L411:
	ldr	r1, [r10, #4]!
	ldr	r0, [r8, #4080]
	bl	FreeUsdByDec
	cmp	r10, r7
	str	r4, [r10]
	mov	r2, #0
	bne	.L411
	mov	r4, r2
	str	r2, [r9, #1388]
.L420:
	ldr	r0, [r8, #4080]
	add	r4, r6, r4, lsl #2
	bl	GetUsd
	add	r4, r4, #77824
	str	r0, [r4, #1372]
	ldr	r2, [r9, #1388]
	add	r3, r6, r2, lsl #2
	add	r3, r3, #78848
	add	r3, r3, #72
	ldr	r3, [r3, #276]
	cmp	r3, #0
	beq	.L412
	ldr	r3, [r6]
	add	r0, r0, #1056
	ldr	r1, .L439
	ldrd	r2, [r3, #48]
	strd	r2, [r0]
	ldr	r3, [r4, #1372]
	ldr	r2, [r9, #1360]
	str	r2, [r3, #1040]
	ldr	r3, [r4, #1372]
	str	r5, [r3, #1048]
	ldr	r2, [r9, #2680]
	ldr	r3, [r4, #1372]
	ldrb	r2, [r2, #22]	@ zero_extendqisi2
	strb	r2, [r3, #1024]
	ldr	r3, [r4, #1372]
	ldr	r2, [r8, #4084]
	str	r2, [r3, #1028]
	ldr	r2, [r9, #2680]
	ldr	r3, [r4, #1372]
	ldrb	r2, [r2, #10]	@ zero_extendqisi2
	strb	r2, [r3, #1025]
	ldr	r3, [r9, #1356]
	tst	r3, #524288
	ldr	r3, [r4, #1372]
	movne	r2, #3
	moveq	r2, #1
	str	r2, [r3, #1036]
	ldr	r0, [r4, #1372]
	ldr	r2, [r0, #1048]
	cmp	r2, #1024
	movgt	r3, #1024
	strgt	r3, [r0, #1048]
	ldr	r3, [fp, #-48]
	ldrgt	r0, [r4, #1372]
	ldr	r3, [r3, #52]
	ldrgt	r2, [r0, #1048]
	blx	r3
	ldr	r3, .L439+8
	ldr	r5, [r3]
	cmp	r5, #0
	beq	.L416
	ldr	r2, [r4, #1372]
	mov	r3, #1072
	ldr	r0, [r8, #4080]
	mov	r1, #6
	blx	r5
.L416:
	ldr	r3, [r9, #1388]
	add	r3, r3, #1
	str	r3, [r9, #1388]
.L435:
	ldr	r0, [fp, #-52]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L418:
	movw	r3, #1309
	ldrsb	r10, [r9, r3]
	cmp	r10, #0
	beq	.L422
	ldr	r3, [r9, #1332]
	cmp	r3, #0
	str	r3, [fp, #-52]
	ldr	r3, .L439+4
	str	r3, [fp, #-48]
	beq	.L407
	ldr	r1, .L439+12
	mov	r0, r8
	ldr	r3, [r3, #68]
	mov	r10, #1
	blx	r3
	ldr	r3, [r9, #1328]
	ldr	r1, [r9, #1312]
	mov	r2, #3
	sub	r3, r3, #3
	ldr	r0, [r9, #1316]
	add	r1, r1, r3
	ldr	r3, [fp, #-48]
	sub	r0, r0, #3
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r3, [r9, #1316]
	ldr	r2, [r9, #1332]
	mov	r0, r4
	sub	r3, r3, #3
	ldr	lr, [r9, #1324]
	add	r2, r2, #3
	str	r3, [r9, #1312]
	sub	lr, lr, #3
	str	r2, [r9, #1328]
	mov	r1, r3
	str	lr, [r9, #1320]
	strb	r10, [r9, #1308]
	strb	r8, [r9, #1309]
	str	r8, [r9, #1332]
	str	r8, [r9, #1316]
	str	r8, [r9, #1324]
	bl	BsInit
	strb	r10, [r9, #1309]
	strb	r8, [r9, #1308]
	mov	r0, r4
	bl	BsToNextByte
	mov	r0, r4
	bl	BsResidBits
	add	r10, r0, #7
	cmp	r0, #0
	movlt	r0, r10
	mov	r10, r0, asr #3
	cmp	r10, #3
	ble	.L423
	sub	r10, r10, #3
	b	.L410
.L438:
	bl	BsGet
	ldr	r3, .L439
	strb	r0, [r3, r5]
	add	r5, r5, #1
.L409:
	add	r8, r8, #1
	cmp	r8, r10
	beq	.L437
.L410:
	mov	r1, #24
	mov	r0, r4
	bl	BsShow
	mov	r1, #8
	bic	r3, r0, #-16777216
	mov	r0, r4
	cmp	r3, #1
	beq	.L424
	cmp	r5, #1024
	blt	.L438
	mov	r1, #8
	mov	r0, r4
	bl	BsGet
	b	.L409
.L403:
	add	r9, r6, #77824
	ldr	r4, [r9, #1388]
	cmp	r4, #3
	bgt	.L425
	str	r3, [fp, #-52]
	add	r8, r6, #110592
	ldr	r3, .L439+4
	str	r3, [fp, #-48]
	b	.L420
.L425:
	ldr	r2, .L439+4
	str	r3, [fp, #-52]
	str	r2, [fp, #-48]
	b	.L419
.L422:
	ldr	r3, .L439+4
	str	r10, [fp, #-52]
	str	r3, [fp, #-48]
	b	.L407
.L412:
	ldr	r1, [fp, #-48]
	mov	r0, r3
	ldr	ip, [r1, #68]
	ldr	r1, .L439+16
	blx	ip
	b	.L435
.L421:
	mov	r5, #0
	b	.L402
.L424:
	str	r3, [fp, #-52]
	b	.L407
.L437:
	mov	r3, #0
	str	r3, [fp, #-52]
	b	.L407
.L423:
	str	r8, [fp, #-52]
	b	.L407
.L440:
	.align	2
.L439:
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_event_report
	.word	.LC34
	.word	.LC35
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #78848
	mov	r7, r0
	add	r5, r5, #576
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
	bgt	.L466
.L442:
	mov	r0, r5
	bl	BsLongSkip
	mov	r0, r5
	bl	BsResidBits
	mov	r6, r0
	add	r0, r0, #7
	cmp	r6, #0
	movlt	r6, r0
	mov	r6, r6, asr #3
	cmp	r6, #3
	subgt	r6, r6, #3
	movgt	r4, #0
	bgt	.L448
	b	.L449
.L446:
	bl	BsSkip
	cmp	r4, r6
	beq	.L449
.L448:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	cmp	r0, #1
	mov	r0, r5
	bne	.L446
.L451:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L449:
	add	r7, r7, #77824
	movw	r3, #1308
	ldrsb	r4, [r7, r3]
	cmp	r4, #0
	bne	.L445
	ldrb	r3, [r7, #1309]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L445
	ldr	r3, [r7, #1332]
	cmp	r3, #0
	beq	.L445
	ldr	r8, .L469
	mov	r0, #7
	ldr	r1, .L469+4
	mov	r6, #1
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r3, [r7, #1328]
	ldr	r1, [r7, #1312]
	mov	r2, #3
	ldr	r0, [r7, #1316]
	sub	r3, r3, #3
	add	r1, r1, r3
	ldr	ip, [r8, #52]
	sub	r0, r0, #3
	blx	ip
	ldr	r3, [r7, #1316]
	ldr	ip, [r7, #1324]
	mov	r0, r5
	ldr	r2, [r7, #1332]
	sub	r3, r3, #3
	strb	r6, [r7, #1308]
	mov	r1, r3
	add	r2, r2, #3
	str	r3, [r7, #1312]
	sub	r3, ip, #3
	str	r2, [r7, #1328]
	str	r3, [r7, #1320]
	strb	r4, [r7, #1309]
	str	r4, [r7, #1332]
	str	r4, [r7, #1316]
	str	r4, [r7, #1324]
	bl	BsInit
	strb	r6, [r7, #1309]
	strb	r4, [r7, #1308]
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
	bgt	.L467
.L450:
	mov	r0, r5
	bl	BsLongSkip
	mov	r0, r5
	bl	BsResidBits
	add	r6, r0, #7
	cmp	r0, #0
	movlt	r0, r6
	mov	r6, r0, asr #3
	cmp	r6, #3
	ble	.L445
	sub	r6, r6, #3
	mov	r4, #0
	b	.L452
.L468:
	bl	BsSkip
	cmp	r4, r6
	beq	.L445
.L452:
	mov	r1, #24
	mov	r0, r5
	bl	BsShow
	add	r4, r4, #1
	mov	r1, #8
	cmp	r0, #1
	mov	r0, r5
	bne	.L468
	b	.L451
.L445:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L466:
	mov	r0, r4
	bl	MP2_SearchStartCode.part.1
	mov	r1, r0, asl #3
	b	.L442
.L467:
	mov	r0, r6
	bl	MP2_SearchStartCode.part.1
	mov	r1, r0, asl #3
	b	.L450
.L470:
	.align	2
.L469:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC34
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
	add	r5, r0, #78848
	mov	r6, r0
	add	r5, r5, #576
	add	r7, r0, #77824
	b	.L472
.L479:
	bl	BsShow
	mov	r4, r0
	mov	r0, r5
	bl	BsPos
	sub	r3, r4, #256
	sub	r3, r3, #1
	mov	r1, #32
	cmp	r3, #174
	str	r0, [r7, #1416]
	mov	r0, r5
	bls	.L485
	bl	BsSkip
.L472:
	mov	r0, r6
	bl	next_start_code
	mov	r1, #32
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r5
	beq	.L479
	cmn	r3, #1
	beq	.L474
.L480:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L474:
	ldr	r3, .L486
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L481
	add	r6, r6, #110592
	mov	r3, #0
	mov	r2, r3
	mov	r1, #113
	ldr	r0, [r6, #4080]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L485:
	bl	BsResidBits
	cmp	r0, #40
	ble	.L474
	ldr	r3, [r7, #1352]
	mov	r2, #0
	str	r2, [r7, #1412]
	cmp	r3, r2
	bne	.L478
	mov	r0, r6
	bl	MP2_GetSequenceInfo
	ldr	r3, [r7, #1404]
	cmp	r3, #0
	beq	.L480
.L478:
	mov	r0, r6
	bl	MP2_ProtocolPPSCheck
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L481:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L487:
	.align	2
.L486:
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
	add	r4, r0, #78848
	add	r7, r0, #77824
	add	r6, r4, #576
	mov	r2, #65536
	mov	r3, #0
	str	r2, [r7, #1356]
	str	r3, [r7, #1352]
	mov	r1, #12
	mov	r5, r0
	mov	r0, r6
	ldr	r8, [r7, #2680]
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
	ldreq	r3, [r7, #1356]
	orreq	r3, r3, #1
	streq	r3, [r7, #1356]
.L489:
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
	beq	.L496
	ldr	r2, .L503
	add	r3, r4, #604
	add	r0, r4, #860
.L492:
	ldrb	r1, [r2, #1]!	@ zero_extendqisi2
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L492
.L491:
	mov	r1, #1
	mov	r0, r6
	bl	BsGet
	cmp	r0, #1
	str	r0, [r8, #80]
	beq	.L497
	add	r5, r5, #79872
	add	r3, r4, #860
	add	r5, r5, #92
	mov	r2, #16
.L494:
	str	r2, [r3, #4]!
	cmp	r3, r5
	bne	.L494
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L496:
	ldr	r10, .L503+4
	add	r9, r10, #64
.L490:
	mov	r1, #8
	mov	r0, r6
	ldrb	r7, [r10, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r7, #19712
	cmp	r10, r9
	add	r3, r3, #152
	str	r0, [r5, r3, asl #2]
	bne	.L490
	b	.L491
.L497:
	ldr	r7, .L503+4
	add	r8, r7, #64
.L493:
	mov	r1, #8
	mov	r0, r6
	ldrb	r4, [r7, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r4, #19712
	cmp	r7, r8
	add	r3, r3, #216
	str	r0, [r5, r3, asl #2]
	bne	.L493
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L504:
	.align	2
.L503:
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
	add	r5, r0, #78848
	add	r4, r0, #77824
	add	r5, r5, #576
	mov	r1, #1
	ldr	r6, [r4, #2680]
	mov	r7, #0
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
	add	ip, r4, #1488
	add	r5, r4, #1472
	add	r3, ip, #12
	add	r5, r5, #4
	add	lr, r4, #1504
	add	lr, lr, #8
	str	r0, [r6, #120]
	ldmia	ip, {r0, r1, r2}
	stmia	r5, {r0, r1, r2}
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	ldmia	lr, {r0, r1, r2}
	stmia	r3, {r0, r1, r2}
	str	r7, [r4, #1512]
	str	r7, [r4, #1520]
	str	r7, [r4, #1516]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	group_of_pictures_header, .-group_of_pictures_header
	.align	2
	.global	sequence_extension
	.type	sequence_extension, %function
sequence_extension:
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
	add	r10, r0, #77824
	add	r4, r0, #78848
	mov	r1, #0
	add	r4, r4, #576
	ldr	r2, [r10, #1356]
	str	r1, [r10, #1352]
	mov	r6, r0
	orr	r2, r2, #131072
	mov	r1, #8
	str	r2, [r10, #1356]
	mov	r0, r4
	ldr	r5, [r10, #2680]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #124]
	mov	r0, r4
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #27]
	mov	r0, r4
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #28]
	mov	r0, r4
	bl	BsGet
	mov	r1, #2
	mov	r9, r0
	mov	r0, r4
	bl	BsGet
	mov	r1, #12
	mov	r8, r0
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	mov	r7, r0
	mov	r0, r4
	bl	BsGet
	cmp	r0, #0
	bne	.L507
	ldr	r2, [r10, #1356]
	orr	r2, r2, #2
	str	r2, [r10, #1356]
.L506:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L507:
	mov	r1, #8
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	str	r0, [fp, #-48]
	mov	r0, r4
	bl	BsGet
	mov	r1, #2
	strb	r0, [r5, #29]
	mov	r0, r4
	bl	BsGet
	mov	r1, #5
	strb	r0, [r5, #30]
	mov	r0, r4
	bl	BsGet
	ldrb	r2, [r5, #30]	@ zero_extendqisi2
	uxtb	r1, r0
	strb	r0, [r5, #31]
	add	r1, r1, #1
	add	r0, r2, #1
	bl	__aeabi_idiv
	ldr	ip, [r5, #88]
	ldr	r1, .L531
	ldr	r2, [r5, #124]
	add	r1, r1, ip, lsl #2
	tst	r2, #128
	ldr	r1, [r1, #160]
	mul	r0, r1, r0
	str	r0, [r5, #72]
	bne	.L530
	ubfx	r1, r2, #4, #3
	and	r2, r2, #15
	str	r1, [r5, #40]
	str	r2, [r5, #44]
.L510:
	ldr	r2, [r5, #48]
	ldr	r1, [r5, #52]
	ubfx	r2, r2, #0, #12
	ubfx	r1, r1, #0, #12
	orr	r9, r2, r9, asl #12
	orr	r8, r1, r8, asl #12
	str	r9, [r5, #48]
	cmp	r8, #0
	cmpne	r9, #0
	str	r8, [r5, #52]
	ldreq	r2, [r10, #1356]
	orreq	r2, r2, #1
	streq	r2, [r10, #1356]
	ldreq	r9, [r5, #48]
	cmp	r9, #8192
	bhi	.L512
	ldr	r2, [r5, #52]
	cmp	r2, #8192
	bls	.L513
.L512:
	ldr	r1, .L531+4
	ldr	r2, [r10, #1356]
	ldr	ip, [r1]
	orr	r2, r2, #1
	str	r2, [r10, #1356]
	cmp	ip, #0
	beq	.L513
	add	r0, r6, #110592
	mov	r3, #0
	mov	r2, r3
	mov	r1, #102
	ldr	r0, [r0, #4080]
	blx	ip
.L513:
	ldr	r3, [r5, #92]
	mov	r1, #400
	ldr	r2, [r5, #40]
	add	r7, r3, r7, lsl #18
	str	r7, [r5, #92]
	sub	r0, r2, #4
	ldr	r3, [r5, #96]
	mul	r7, r1, r7
	ldr	r1, [fp, #-48]
	cmp	r0, #1
	add	r10, r3, r1, lsl #10
	str	r10, [r5, #96]
	str	r7, [r5, #68]
	bls	.L515
	ldr	r3, .L531+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L516
	add	r0, r6, #110592
	mov	r3, #0
	mov	r2, r3
	mov	r1, #102
	ldr	r0, [r0, #4080]
	blx	ip
	ldr	r2, [r5, #40]
.L516:
	ldr	r3, .L531+8
	mov	r0, #1
	ldr	r1, .L531+12
	ldr	r3, [r3, #68]
	blx	r3
.L515:
	ldr	r2, [r5, #44]
	sub	r3, r2, #4
	cmp	r3, #6
	bls	.L506
	ldr	r3, .L531+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L517
	add	r6, r6, #110592
	mov	r3, #0
	mov	r2, r3
	mov	r1, #102
	ldr	r0, [r6, #4080]
	blx	ip
	ldr	r2, [r5, #44]
.L517:
	ldr	r3, .L531+8
	mov	r0, #1
	ldr	r1, .L531+16
	ldr	r3, [r3, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L530:
	and	r2, r2, #15
	cmp	r2, #5
	moveq	r1, #133
	moveq	r2, #8
	streq	r1, [r5, #40]
	streq	r2, [r5, #44]
	b	.L510
.L532:
	.align	2
.L531:
	.word	.LANCHOR0
	.word	g_event_report
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC36
	.word	.LC37
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
	add	r6, r0, #77824
	add	r4, r0, #78848
	add	r4, r4, #576
	mov	r1, #3
	ldr	r3, [r6, #1356]
	mov	r0, r4
	ldr	r5, [r6, #2680]
	orr	r3, r3, #262144
	str	r3, [r6, #1356]
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #128]
	mov	r0, r4
	bl	BsGet
	cmp	r0, #0
	str	r0, [r5, #132]
	bne	.L540
.L534:
	mov	r1, #14
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #148]
	mov	r0, r4
	bl	BsGet
	cmp	r0, #0
	bne	.L535
	ldr	r3, [r6, #1356]
	orr	r3, r3, #4
	str	r3, [r6, #1356]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L535:
	mov	r0, r4
	mov	r1, #14
	bl	BsGet
	str	r0, [r5, #152]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L540:
	mov	r1, #8
	mov	r0, r4
	bl	BsGet
	mov	r1, #8
	str	r0, [r5, #136]
	mov	r0, r4
	bl	BsGet
	mov	r1, #8
	str	r0, [r5, #140]
	mov	r0, r4
	bl	BsGet
	str	r0, [r5, #144]
	b	.L534
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
	add	r5, r0, #78848
	add	r3, r0, #77824
	add	r5, r5, #576
	mov	r7, r0
	mov	r1, #1
	ldr	r9, [r3, #2680]
	mov	r0, r5
	bl	BsGet
	cmp	r0, #1
	str	r0, [r9, #76]
	beq	.L566
.L542:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #1
	str	r0, [r9, #80]
	ldreq	r6, .L568
	addeq	r8, r6, #64
	beq	.L544
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	beq	.L546
.L552:
	mov	r4, #64
.L547:
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	subs	r4, r4, #1
	bne	.L547
.L546:
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	beq	.L567
.L548:
	mov	r4, #64
.L549:
	mov	r1, #8
	mov	r0, r5
	bl	BsGet
	subs	r4, r4, #1
	bne	.L549
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L544:
	mov	r1, #8
	mov	r0, r5
	ldrb	r4, [r6, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r4, #19712
	cmp	r6, r8
	add	r3, r3, #216
	str	r0, [r7, r3, asl #2]
	bne	.L544
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	bne	.L552
	mov	r1, #1
	mov	r0, r5
	bl	BsGet
	cmp	r0, #0
	bne	.L548
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L567:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L566:
	ldr	r6, .L568
	add	r8, r6, #64
.L543:
	mov	r1, #8
	mov	r0, r5
	ldrb	r4, [r6, #1]!	@ zero_extendqisi2
	bl	BsGet
	add	r3, r4, #19712
	cmp	r6, r8
	add	r3, r3, #152
	str	r0, [r7, r3, asl #2]
	bne	.L543
	b	.L542
.L569:
	.align	2
.L568:
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
	add	r0, r0, #78848
	mov	r1, #32
	add	r0, r0, #576
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
	add	r8, r0, #77824
	add	r4, r0, #78848
	add	r4, r4, #576
	ldr	r5, [r8, #2680]
	ldr	r3, [r8, #1356]
	orr	r3, r3, #2097152
	str	r3, [r8, #1356]
	ldrb	r3, [r5, #27]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L572
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L584
.L580:
	mov	r7, #1
.L573:
	add	r5, r5, #160
	mov	r6, #0
	b	.L577
.L586:
	bl	BsGet
	mov	r1, #1
	str	r0, [r5, #12]
	mov	r0, r4
	bl	BsGet
	cmp	r0, #0
	beq	.L583
	cmp	r6, r7
	bge	.L585
.L577:
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
	bne	.L586
.L583:
	ldr	r3, [r8, #1356]
	orr	r3, r3, #32
	str	r3, [r8, #1356]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L572:
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L580
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r7, #2
	movne	r7, #3
	b	.L573
.L584:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r7, #2
	movne	r7, #3
	b	.L573
.L585:
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
	add	r8, r0, #77824
	add	r4, r0, #78848
	add	r4, r4, #576
	ldr	r7, [r8, #2680]
	ldr	r3, [r8, #1356]
	add	r10, r7, #4
	add	r9, r7, #8
	orr	r3, r3, #1048576
	str	r3, [r8, #1356]
.L588:
	mov	r6, r10
	mov	r5, #0
.L590:
	mov	r1, #4
	mov	r0, r4
	bl	BsGet
	add	r5, r5, #1
	uxtb	r0, r0
	strb	r0, [r6], #1
	sub	r3, r0, #10
	cmp	r0, #0
	cmpne	r3, #4
	ldrls	r3, [r8, #1356]
	orrls	r3, r3, #16
	strls	r3, [r8, #1356]
	cmp	r5, #2
	bne	.L590
	add	r10, r10, #2
	cmp	r10, r9
	bne	.L588
	mov	r1, r5
	mov	r0, r4
	bl	BsGet
	mov	r1, r5
	strb	r0, [r7, #8]
	mov	r0, r4
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r7, #9]
	cmp	r0, #0
	bne	.L592
	ldr	r3, [r8, #1356]
	orr	r3, r3, #16
	str	r3, [r8, #1356]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L592:
	mov	r1, #1
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #10]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #11]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #12]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #13]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #14]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #15]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #16]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #17]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #18]
	mov	r0, r4
	bl	BsGet
	uxtb	r0, r0
	strb	r0, [r7, #3]
	cmp	r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	mov	r1, #1
	mov	r0, r4
	bl	BsGet
	mov	r1, #3
	strb	r0, [r7, #19]
	mov	r0, r4
	bl	BsGet
	mov	r1, #1
	strb	r0, [r7, #20]
	mov	r0, r4
	bl	BsGet
	mov	r1, #7
	strb	r0, [r7, #21]
	mov	r0, r4
	bl	BsGet
	mov	r1, #8
	str	r0, [r7, #32]
	mov	r0, r4
	bl	BsGet
	str	r0, [r7, #36]
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
	add	r0, r0, #78848
	mov	r1, #32
	add	r0, r0, #576
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
	add	r0, r0, #78848
	mov	r1, #32
	add	r0, r0, #576
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
	add	r4, r0, #78848
	mov	r5, #0
	add	r4, r4, #576
	b	.L604
.L606:
	bl	BsSkip
	mov	r0, r4
	bl	BsResidBits
	add	r5, r5, #1
	cmp	r0, #8
	ble	.L605
.L604:
	mov	r1, #1
	mov	r0, r4
	bl	BsGet
	mov	r1, #8
	cmp	r0, #0
	mov	r0, r4
	bne	.L606
.L605:
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
	add	r2, r0, #77824
	add	r4, r0, #78848
	add	r4, r4, #576
	mov	r1, #10
	ldr	r3, [r2, #1356]
	mov	r6, r0
	mov	r0, r4
	ldr	r5, [r2, #2680]
	bic	r3, r3, #3145728
	bic	r3, r3, #56
	orr	r3, r3, #524288
	str	r3, [r2, #1356]
	bl	BsGet
	mov	r1, #3
	str	r0, [r5, #156]
	mov	r0, r4
	bl	BsGet
	mov	r1, #16
	strb	r0, [r5, #22]
	mov	r0, r4
	bl	BsGet
	ldrb	r7, [r5, #22]	@ zero_extendqisi2
	sub	r3, r7, #2
	cmp	r3, #1
	str	r0, [r5, #160]
	bls	.L610
.L608:
	cmp	r7, #3
	beq	.L611
.L609:
	mov	r0, r6
	bl	extra_bit_information
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L611:
	mov	r1, #1
	mov	r0, r4
	bl	BsGet
	mov	r1, r7
	strb	r0, [r5, #25]
	mov	r0, r4
	bl	BsGet
	strb	r0, [r5, #26]
	b	.L609
.L610:
	mov	r1, #1
	mov	r0, r4
	bl	BsGet
	mov	r1, #3
	strb	r0, [r5, #23]
	mov	r0, r4
	bl	BsGet
	ldrb	r7, [r5, #22]	@ zero_extendqisi2
	strb	r0, [r5, #24]
	b	.L608
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
	add	r0, r0, #78848
	mov	r1, #32
	add	r0, r0, #576
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
	add	r3, r0, #77824
	add	r4, r0, #78848
	add	r4, r4, #576
	mov	r2, #0
	ldr	r3, [r3, #2680]
	mov	r5, r0
	mov	r1, #4
	mov	r0, r4
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
	b	.L614
.L616:
	.word	.L615
	.word	.L617
	.word	.L618
	.word	.L619
	.word	.L619
	.word	.L614
	.word	.L621
	.word	.L622
	.word	.L619
	.word	.L619
.L621:
	mov	r0, r5
	bl	picture_display_extension
.L614:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L619:
	mov	r0, r4
	mov	r1, #32
	bl	BsSkip
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L618:
	mov	r0, r5
	bl	quant_matrix_extension
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L622:
	mov	r0, r5
	bl	picture_coding_extension
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L615:
	mov	r0, r5
	bl	sequence_extension
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L617:
	mov	r0, r5
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
	@ args = 0, pretend = 0, frame = 904
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #908)
	sub	sp, sp, #908
	ldr	r3, [r1, #688]
	mov	r9, r1
	mov	r5, r0
	cmp	r3, #1
	ldr	r6, .L634
	beq	.L631
	add	r4, r0, #77824
	mov	r10, #0
	str	r10, [fp, #-944]
	str	r10, [fp, #-940]
.L626:
	ldr	r7, [r4, #3032]
	movw	r2, #49160
	ldr	ip, [r6, #48]
	movt	r2, 1
	mov	r1, #0
	add	r6, r5, #79872
	mov	r0, r5
	mov	r8, #0
	blx	ip
	add	r0, r6, #640
	str	r7, [r4, #3032]
	mov	r7, #1
	bl	ResetVoQueue
	add	r2, r5, #110592
	add	r1, r5, #45056
	str	r9, [r5]
	add	r3, r6, #256
	mov	lr, #3
	str	r3, [r4, #2684]
	add	r6, r6, #444
	str	r6, [r4, #2680]
	mov	r0, r5
	str	r8, [r2, #4092]
	str	r7, [r4, #1368]
	strb	lr, [r1, #273]
	str	r8, [r4, #1412]
	ldr	r1, [r9, #8]
	str	r8, [r4, #1404]
	str	r2, [fp, #-936]
	str	r1, [r4, #1344]
	bl	VCTRL_GetChanIDByCtx
	ldr	r2, [fp, #-936]
	ldr	r6, .L634
	cmn	r0, #1
	str	r0, [r2, #4080]
	beq	.L632
	ldr	r9, [r9, #688]
	cmp	r9, #1
	movne	r9, r7
	beq	.L633
.L628:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L633:
	ldr	r2, [fp, #-940]
	add	r5, r5, #78848
	ldr	r3, [r4, #1356]
	sub	r1, fp, #556
	add	r0, r5, #608
	str	r10, [r4, #1352]
	str	r2, [r4, #1392]
	orr	r3, r3, #65536
	ldr	r2, [fp, #-944]
	str	r3, [r4, #1356]
	ldr	r3, [r6, #52]
	str	r2, [r4, #1404]
	mov	r2, #256
	blx	r3
	ldr	r3, [r6, #52]
	sub	r1, fp, #300
	add	r0, r5, #864
	mov	r2, #256
	blx	r3
	ldr	r3, [r6, #52]
	sub	r1, fp, #932
	mov	r2, #188
	ldr	r0, [r4, #2680]
	blx	r3
	ldr	r3, [r6, #52]
	ldr	r0, [r4, #2684]
	mov	r2, #188
	sub	r1, fp, #744
	blx	r3
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L631:
	add	r4, r0, #77824
	add	r3, r0, #78848
	add	r1, r3, #608
	ldr	ip, [r6, #52]
	mov	r7, r3
	ldr	r3, [r4, #1392]
	mov	r2, #256
	sub	r0, fp, #556
	ldr	r10, [r4, #1352]
	str	r3, [fp, #-940]
	ldr	r3, [r4, #1404]
	str	r3, [fp, #-944]
	blx	ip
	add	r1, r7, #864
	ldr	r3, [r6, #52]
	mov	r2, #256
	sub	r0, fp, #300
	blx	r3
	ldr	r3, [r6, #52]
	ldr	r1, [r4, #2680]
	mov	r2, #188
	sub	r0, fp, #932
	blx	r3
	ldr	r3, [r6, #52]
	ldr	r1, [r4, #2684]
	mov	r2, #188
	sub	r0, fp, #744
	blx	r3
	b	.L626
.L632:
	ldr	r2, [r6, #68]
	mov	r0, r8
	ldr	r1, .L634+4
	mvn	r9, #19
	blx	r2
	b	.L628
.L635:
	.align	2
.L634:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC38
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
	ldr	r3, .L637
	add	r2, r0, #77824
	ldr	r1, .L637+4
	mov	r0, #2
	ldr	r2, [r2, #1364]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L638:
	.align	2
.L637:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC39
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r4, r1
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, r0, #110592
	mov	r2, #0
	ldr	r0, [r5, #4080]
	bl	FSP_SetDisplay
	mov	r1, r4
	ldr	r0, [r5, #4080]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	addne	r4, r0, #208
	addne	r6, r0, #224
	movne	r7, #0
	beq	.L643
.L642:
	ldr	r1, [r4, #4]!
	ldr	r0, [r5, #4080]
	bl	FreeUsdByDec
	cmp	r4, r6
	str	r7, [r4]
	bne	.L642
.L643:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
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
	beq	.L668
	add	r5, r0, #77824
	add	r6, r0, #110592
	ldr	r1, [r5, #1436]
	ldr	r0, [r6, #4080]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L668
	ldr	r1, [r5, #1436]
	ldr	r0, [r6, #4080]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L666
.L668:
	add	r5, r4, #79872
.L647:
	add	r7, r5, #640
.L653:
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	GetVoLastImageID
.L666:
	mov	r0, r4
	bl	MP2_SetImgFormat
	add	r0, r4, #78848
	add	r3, r0, #344
	add	r2, r9, #212
	add	r0, r0, #360
	mov	ip, #0
.L649:
	ldr	r1, [r3, #4]!
	mov	r8, #0
	add	r2, r2, #4
	cmp	r1, r8
	strne	r1, [r2, #-4]
	cmp	r3, r0
	str	ip, [r3]
	bne	.L649
	str	r8, [r5, #1388]
	add	r5, r4, #79872
	mov	r3, #1
	add	r7, r5, #640
	mov	r2, r3
	str	r3, [r9, #244]
	ldr	r1, [r9, #196]
	ldr	r0, [r6, #4080]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #4080]
	mov	r3, r7
	str	r9, [sp]
	mov	r2, r4
	mov	r1, #3
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L650
	ldr	r1, [r9, #196]
	mov	r0, r4
	bl	MPEG2DEC_RecycleImage
	b	.L653
.L650:
	ldr	r3, [r9, #200]
	cmp	r3, #100
	bhi	.L647
	ldr	r3, [r9, #164]
	cmp	r3, #31
	bls	.L647
	ldr	r3, [r9, #168]
	cmp	r3, #31
	bls	.L653
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
	ldrb	r3, [r3, #273]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L670
	add	r3, r0, #77824
	ldr	r3, [r3, #1368]
	cmp	r3, #0
	beq	.L672
.L670:
	add	r5, r4, #110592
	ldr	r0, [r5, #4080]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L672
	ldr	r0, [r5, #4080]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	beq	.L680
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L672:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L680:
	add	r1, r4, #79872
	ldr	r0, [r5, #4080]
	add	r1, r1, #640
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
	ldr	r1, [r1, #28]
	mov	r5, r0
	bl	SM_ReleaseStreamSeg
	ldrsb	r3, [r4]
	cmp	r3, #1
	beq	.L682
	mov	r0, r5
	ldr	r1, [r4, #32]
	bl	SM_ReleaseStreamSeg
.L682:
	ldr	r3, .L686
	mov	r0, r4
	mov	r2, #36
	mov	r1, #0
	ldr	r3, [r3, #48]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L687:
	.align	2
.L686:
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
	beq	.L693
	add	r3, r0, #32768
	mov	r5, #0
	str	r5, [r3, #1032]
	ldr	r1, [r4, #12]
	str	r1, [r3, #1076]
	ldr	r1, [r4, #8]
	str	r1, [r3, #1048]
	ldr	r1, [r4]
	str	r1, [r3, #1040]
	ldr	r1, [r4, #12]
	str	r1, [r3, #1056]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	strb	r1, [r3, #1036]
	ldr	r1, [r4, #16]
	str	r1, [r3, #1064]
	ldrb	ip, [r4, #5]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L691
	ldr	r1, [r4, #88]
	str	r1, [r3, #1052]
	ldr	r1, [r4, #80]
	str	r1, [r3, #1044]
	ldr	r1, [r4, #92]
	str	r1, [r3, #1060]
	ldrb	r1, [r4, #85]	@ zero_extendqisi2
	strb	r1, [r3, #1037]
	ldr	r1, [r4, #96]
	str	r1, [r3, #1068]
	ldrb	ip, [r4, #85]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L692
	add	r1, r0, #33792
	mov	r0, r2
	add	r1, r1, #12
	bl	MP2_ClearCurPacket
	ldr	r3, .L694
	ldrb	r2, [r4, #85]	@ zero_extendqisi2
	mov	r0, #1
	ldr	r1, .L694+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L692:
	mov	r0, ip
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L691:
	str	r5, [r3, #1052]
	mov	r0, ip
	str	r5, [r3, #1044]
	str	r5, [r3, #1060]
	strb	r5, [r3, #1037]
	str	r5, [r3, #1068]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L693:
	ldr	r3, .L694
	add	r2, r0, #32768
	ldr	r1, .L694+8
	mov	r0, #1
	ldr	r2, [r2, #1092]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L695:
	.align	2
.L694:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC41
	.word	.LC40
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
	ldr	r3, [r6, #260]
	cmp	r3, #0
	movne	r4, r0
	movne	r5, #0
	addne	r7, r4, #110592
	bne	.L699
	b	.L700
.L698:
	ldr	r3, [r6, #260]
	add	r4, r4, #44
	cmp	r5, r3
	bcs	.L700
.L699:
	ldr	r1, [r4, #284]
	add	r5, r5, #1
	ldr	r0, [r7, #4080]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #264]
	cmp	r3, #0
	beq	.L698
	ldr	r3, [r4, #272]
	cmp	r3, #0
	beq	.L698
	ldr	r1, [r4, #288]
	add	r4, r4, #44
	ldr	r0, [r7, #4080]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r6, #260]
	cmp	r5, r3
	bcc	.L699
.L700:
	mov	r3, #0
	str	r3, [r6, #260]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MP2_ClearCurPic, .-MP2_ClearCurPic
	.align	2
	.global	MPEG2DEC_VDMPostProc
	.type	MPEG2DEC_VDMPostProc, %function
MPEG2DEC_VDMPostProc:
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
	ldr	r10, .L775
	ldrb	lr, [r0, #152]	@ zero_extendqisi2
	add	r5, r0, #77824
	mov	r6, r0
	mov	r2, r1
	ldr	r4, [r10, #68]
	mov	r9, r1
	strb	lr, [r5, #1536]
	mov	r0, #22
	ldr	r1, .L775+4
	add	r3, r6, #45056
	ldr	r8, [r5, #2680]
	add	r7, r6, #110592
	str	r3, [fp, #-48]
	blx	r4
	mov	r0, r6
	bl	MP2_RecordVFMWState
	mov	r0, r6
	bl	MP2_ClearCurPic
	movw	r2, #45320
	ldr	r4, [r10, #48]
	mov	r1, #0
	add	r0, r6, #8
	blx	r4
	ldr	r1, [r5, #1440]
	ldr	r0, [r7, #4080]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r5, #1436]
	mov	r4, r0
	ldr	r0, [r7, #4080]
	bl	FSP_GetFsImagePtr
	ldr	r1, [r5, #1432]
	str	r0, [fp, #-52]
	ldr	r0, [r7, #4080]
	bl	FSP_GetFsImagePtr
	cmp	r4, #0
	beq	.L770
	ldrb	r2, [r8, #22]	@ zero_extendqisi2
	cmp	r9, #0
	ldr	r3, [fp, #-48]
	strb	r2, [r3, #272]
	str	r9, [r4, #200]
	blt	.L712
	cmp	r9, #100
	movlt	r3, r9
	movge	r3, #100
	str	r3, [r4, #200]
	ldrb	r2, [r8, #22]	@ zero_extendqisi2
	cmp	r2, #3
	beq	.L713
	ldr	r2, [r6]
	ldr	r2, [r2]
	cmp	r3, r2
	bhi	.L771
.L713:
	ldr	r3, [r10, #68]
	mov	r0, #22
	ldr	r1, .L775+8
	blx	r3
	mov	r0, r6
	bl	MP2_SetImgFormat
	ldrb	r3, [r8, #9]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L719
	ldr	r3, [r5, #1368]
	cmp	r3, #1
	movne	r0, #1
	beq	.L719
.L765:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L719:
	add	r1, r6, #78848
	add	r4, r4, #212
	add	r3, r1, #344
	add	r1, r1, #360
	mov	lr, #0
.L718:
	ldr	r2, [r3, #4]!
	mov	r0, #0
	add	r4, r4, #4
	cmp	r2, r0
	strne	r2, [r4, #-4]
	cmp	r3, r1
	str	lr, [r3]
	bne	.L718
	str	r0, [r5, #1388]
	ldr	r3, [r6]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	beq	.L721
	ldrb	r3, [r8, #22]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L721
	ldr	r1, [r5, #1432]
	str	r1, [r5, #1444]
.L722:
	cmn	r1, #1
	beq	.L724
	ldr	r0, [r7, #4080]
	bl	FSP_GetDisplay
	cmp	r0, #1
	beq	.L728
	ldr	r1, [r5, #1444]
	ldr	r0, [r7, #4080]
	bl	FSP_GetDisplay
	cmp	r0, #2
	beq	.L728
.L724:
	ldrb	r3, [r8, #27]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L772
	ldrb	r3, [r8, #18]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r5, #1520]
	addeq	r3, r3, #1
	addne	r3, r3, #1
	streq	r3, [r5, #1520]
	ldreqb	r3, [r8, #16]	@ zero_extendqisi2
	ldreq	r2, [r5, #1516]
	addeq	r3, r3, #2
	strne	r3, [r5, #1520]
	addeq	r3, r2, r3
	movne	r3, r3, asl #1
	str	r3, [r5, #1516]
.L735:
	ldr	r2, [r5, #1512]
	mov	r0, #1
	ldr	r3, [r5, #1364]
	add	r2, r2, r0
	str	r2, [r5, #1512]
	add	r3, r3, r0
	str	r3, [r5, #1364]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L712:
	mov	r3, #0
	str	r3, [r4, #200]
	b	.L713
.L771:
	ldr	r1, [r5, #1436]
	ldr	r0, [r7, #4080]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L715
	ldr	r1, [r4, #212]
	mov	r6, #0
	ldr	r0, [r7, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r4, #216]
	ldr	r0, [r7, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r4, #220]
	ldr	r0, [r7, #4080]
	bl	FreeUsdByDec
	ldr	r1, [r4, #224]
	ldr	r0, [r7, #4080]
	bl	FreeUsdByDec
	str	r6, [r4, #212]
	str	r6, [r4, #216]
	mov	r2, #1
	str	r6, [r4, #220]
	str	r6, [r4, #224]
	ldr	r1, [r5, #1436]
	ldr	r0, [r7, #4080]
	bl	FSP_ClearLogicFs
	mov	r2, #1
	ldr	r1, [r5, #1432]
	ldr	r0, [r7, #4080]
	bl	FSP_ClearLogicFs
	str	r6, [r5, #1432]
	str	r6, [r5, #1436]
	ldr	r0, [r7, #4080]
	bl	FSP_GetFspType
	cmp	r0, r6
	beq	.L715
	ldr	r0, [r7, #4080]
	bl	FSP_GetNullLogicFs
	str	r0, [r5, #1432]
	str	r0, [r5, #1436]
.L715:
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #1396]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L728:
	ldr	r1, [r5, #1444]
	ldr	r0, [r7, #4080]
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L773
	ldr	r2, [r4, #200]
	cmp	r2, #0
	beq	.L730
	ldr	r3, [r6]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bhi	.L774
.L730:
	ldr	r9, [r5, #1444]
	ldr	r0, [r7, #4080]
	ldr	r10, [r10, #68]
	mov	r1, r9
	bl	FSP_GetRef
	mov	r2, r9
	ldr	r1, .L775+12
	mov	r3, r0
	mov	r0, #12
	blx	r10
	mov	r2, #1
	ldr	r1, [r5, #1444]
	ldr	r0, [r7, #4080]
	bl	FSP_SetDisplay
	add	r3, r6, #79872
	ldr	r0, [r7, #4080]
	add	r3, r3, #640
	str	r4, [sp]
	mov	r2, r6
	mov	r1, #3
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L737
	add	r6, r4, #208
	add	r4, r4, #224
	mov	r8, #0
.L732:
	ldr	r1, [r6, #4]!
	ldr	r0, [r7, #4080]
	bl	FreeUsdByDec
	cmp	r6, r4
	str	r8, [r6]
	bne	.L732
	ldr	r0, [r7, #4080]
	mov	r2, #0
	ldr	r1, [r5, #1444]
	bl	FSP_SetDisplay
	mov	r0, #0
	b	.L765
.L721:
	ldr	r1, [r5, #1440]
	str	r1, [r5, #1444]
	b	.L722
.L772:
	ldrb	r2, [r8, #16]	@ zero_extendqisi2
	ldrb	r3, [r8, #10]	@ zero_extendqisi2
	ldr	r1, [r5, #1520]
	add	r3, r2, r3
	add	r3, r3, #1
	add	r3, r3, r1
	str	r3, [r5, #1520]
	mov	r3, r3, asl #1
	str	r3, [r5, #1516]
	b	.L735
.L774:
	ldr	ip, [r10, #68]
	mov	r0, #1
	ldr	r1, .L775+16
	add	r6, r4, #208
	blx	ip
	add	r4, r4, #224
	mov	r9, #0
.L733:
	ldr	r1, [r6, #4]!
	ldr	r0, [r7, #4080]
	bl	FreeUsdByDec
	cmp	r6, r4
	str	r9, [r6]
	bne	.L733
	ldr	r0, [r7, #4080]
	mov	r2, #0
	ldr	r1, [r5, #1444]
	bl	FSP_SetDisplay
	b	.L724
.L770:
	str	r0, [sp]
	mov	r2, r4
	ldr	r3, [fp, #-52]
	mov	r0, r4
	ldr	ip, [r10, #68]
	ldr	r1, .L775+20
	blx	ip
	mov	r0, r4
	b	.L765
.L737:
	ldr	r3, [r7, #4088]
	add	r3, r3, #1
	str	r3, [r7, #4088]
	b	.L724
.L773:
	ldr	r3, [r10, #68]
	movw	r2, #3545
	ldr	r1, .L775+24
	blx	r3
	mov	r0, r4
	b	.L765
.L776:
	.align	2
.L775:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC42
	.word	.LC44
	.word	.LC46
	.word	.LC45
	.word	.LC43
	.word	.LC22
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
	ldr	ip, .L778
	mov	r2, r5
	mov	r3, r4
	ldr	r1, .L778+4
	mov	r0, #2
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L779:
	.align	2
.L778:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC47
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
	bls	.L781
	ldr	r2, [r0]
	cmp	r2, r3
	beq	.L793
.L781:
	ldr	r1, [r5, #20]
	mov	r0, r4
	ldr	r2, [r5, #8]
	sub	r1, r1, #1
	add	r3, r1, r3
	mov	r1, #5
	mul	r3, r2, r3
	ldr	r2, .L795
	cmp	r3, #0
	subne	r3, r3, #1
	str	r3, [r5, #28]
	bl	MP2_u_v
	mov	r3, #0
	mov	r1, #1
	str	r3, [r5, #32]
	str	r0, [r5, #48]
	mov	r0, r4
	bl	BsShow
	cmp	r0, #0
	bne	.L794
.L786:
	mov	r0, r4
	ldr	r2, .L795+4
	mov	r1, #1
	bl	MP2_u_v
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L794:
	ldr	r2, .L795+8
	mov	r1, #1
	mov	r0, r4
	bl	MP2_u_v
	ldr	r2, .L795+12
	mov	r1, #1
	mov	r0, r4
	bl	MP2_u_v
	ldr	r2, .L795+16
	mov	r1, #7
	str	r0, [r5, #32]
	mov	r0, r4
	bl	MP2_u_v
	b	.L784
.L785:
	bl	MP2_u_v
	ldr	r2, .L795+20
	mov	r1, #8
	mov	r0, r4
	bl	MP2_u_v
.L784:
	mov	r1, #1
	mov	r0, r4
	bl	BsShow
	ldr	r2, .L795+4
	mov	r1, #1
	cmp	r0, #0
	mov	r0, r4
	bne	.L785
	b	.L786
.L793:
	ldr	r2, .L795+24
	mov	r1, #3
	mov	r0, r4
	bl	MP2_u_v
	mov	r3, r0, asl #7
	str	r0, [r5, #24]
	b	.L781
.L796:
	.align	2
.L795:
	.word	.LC49
	.word	.LC50
	.word	.LC51
	.word	.LC52
	.word	.LC53
	.word	.LC54
	.word	.LC48
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
	add	r5, r0, #77824
	mov	r6, r0
	ldr	r2, [r7, #260]
	ldr	r1, [r5, #1576]
	cmp	r2, #1024
	mla	r4, r4, r2, r0
	str	r1, [r4, #296]
	ldr	r1, [r5, #1592]
	str	r1, [r4, #300]
	ldr	r1, [r5, #1588]
	str	r1, [r4, #292]
	bcs	.L804
	ldr	r2, [r5, #1624]
	add	r0, r0, #78848
	ldr	r3, [r5, #1320]
	add	r0, r0, #576
	and	r1, r2, #7
	str	r1, [r4, #276]
	add	r3, r3, r2, lsr #3
	str	r3, [r4, #260]
	ldr	r3, [r5, #1336]
	str	r3, [r4, #284]
	bl	BsResidBits
	movw	r2, #1308
	str	r0, [r4, #268]
	ldrsh	r3, [r5, r2]
	cmp	r3, #1
	beq	.L805
	cmp	r3, #256
	bne	.L803
	ldr	r1, [r5, #1324]
	mov	r2, #0
	str	r2, [r4, #280]
	str	r1, [r4, #264]
	ldr	r3, [r5, #1340]
	str	r3, [r4, #288]
	ldr	r3, [r5, #1332]
	mov	r3, r3, asl #3
	str	r3, [r4, #272]
.L802:
	ldr	r3, [r7, #260]
	mov	r0, #1
	add	r3, r3, r0
	str	r3, [r7, #260]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L804:
	ldr	ip, .L806
	mov	r3, #1024
	ldr	r1, .L806+4
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r6
	bl	MP2_ClearCurPic
	ldr	r3, [r5, #1356]
	mov	r0, #0
	str	r0, [r5, #1412]
	bic	r3, r3, #5767168
	str	r3, [r5, #1356]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L805:
	mov	r3, #0
	add	r4, r4, #268
	str	r3, [r4, #-4]
	str	r3, [r4, #12]
	str	r3, [r4, #20]
	str	r3, [r4, #4]
	b	.L802
.L803:
	ldr	r1, .L806
	movw	r3, #1309
	ldrsb	r2, [r5, r2]
	mov	r0, #1
	ldrsb	r3, [r5, r3]
	ldr	ip, [r1, #68]
	ldr	r1, .L806+8
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L807:
	.align	2
.L806:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC55
	.word	.LC56
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
	ldr	r5, .L814
	add	r0, r5, r0, lsl #3
	ldr	r4, [r0, #224]
	ldr	r1, [r0, #228]
	cmp	r4, #6
	bls	.L812
	cmp	r4, #9
	beq	.L813
	mov	r1, #11
	mov	r0, r6
	bl	BsShow
	ldr	r2, .L814+4
	and	r3, r0, #127
	mov	r0, r6
	sub	r3, r3, #24
	add	r5, r5, r3, lsl #3
	ldr	r4, [r5, #352]
	ldr	r1, [r5, #356]
	bl	MP2_u_v
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L812:
	mov	r0, r6
	ldr	r2, .L814+4
	bl	MP2_u_v
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L813:
	mov	r0, r6
	ldr	r2, .L814+4
	bl	MP2_u_v
	rsb	r0, r0, #9
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L815:
	.align	2
.L814:
	.word	.LANCHOR0
	.word	.LC57
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
	b	.L817
.L821:
	cmp	r0, #15
	beq	.L818
	cmp	r0, #8
	add	r5, r5, #33
	bne	.L829
.L818:
	ldr	r2, .L831
	mov	r1, #11
	mov	r0, r4
	bl	MP2_u_v
.L817:
	mov	r1, #11
	mov	r0, r4
	bl	BsShow
	cmp	r0, #23
	bls	.L821
	mov	r0, r4
	bl	MP2_MbAddrInc
	ldr	r4, [r6, #16]
	cmp	r4, #0
	beq	.L822
	ldr	r3, [r6, #28]
	add	r5, r5, r0
	cmp	r3, #0
	addne	r2, r5, r3
	ldr	r3, [r6, #12]
	subeq	r2, r5, #1
	str	r2, [r6, #44]
	sub	r3, r3, #1
	cmp	r2, r3
	bhi	.L830
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L829:
	ldr	r3, .L831+4
	mov	r0, #1
	ldr	r1, .L831+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L822:
	ldr	r3, .L831+4
	mov	r2, r4
	ldr	r1, .L831+12
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L830:
	ldr	ip, .L831+4
	mov	r0, #1
	ldr	r1, .L831+16
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L832:
	.align	2
.L831:
	.word	.LC59
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC58
	.word	.LC61
	.word	.LC60
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
	add	r4, r0, #77824
	add	ip, r0, #78848
	add	r7, ip, #520
	uxtb	r1, r1
	ldr	r2, [r4, #1392]
	add	ip, ip, #576
	ldr	r3, [r4, #2680]
	mov	r6, r0
	ldr	lr, [r4, #1328]
	sub	r2, r2, #1
	ldr	r8, [r4, #1312]
	clz	r2, r2
	mov	r0, r7
	mov	r5, #0
	str	lr, [r4, #1584]
	mov	r2, r2, lsr #5
	str	r8, [r4, #1580]
	str	r2, [r4, #1544]
	ldr	r2, [r3, #60]
	str	r2, [r4, #1548]
	ldr	r2, [r3, #56]
	str	r2, [r4, #1552]
	ldr	r2, [r3, #52]
	ldr	r3, [r3, #48]
	add	r2, r2, #15
	str	r1, [r4, #1564]
	add	r3, r3, #15
	str	ip, [r4, #1596]
	mov	r2, r2, lsr #4
	mov	r3, r3, lsr #4
	mul	r3, r3, r2
	str	r3, [r4, #1556]
	bl	MP2_DecSliceHeader
	mov	r0, r7
	bl	MP2_InitMb
	str	r5, [r4, #1560]
	cmp	r0, #1
	mov	r7, r0
	beq	.L835
	ldr	r3, .L840
	mov	r0, #1
	ldr	r1, .L840+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L835:
	mov	r0, r6
	bl	MP2_WriteSliceInfo
	cmp	r0, #1
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, .L840
	mov	r0, r7
	ldr	r1, .L840+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L841:
	.align	2
.L840:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC62
	.word	.LC63
	UNWIND(.fnend)
	.size	MP2_DecSlice, .-MP2_DecSlice
	.align	2
	.global	MP2_GetHDR
	.type	MP2_GetHDR, %function
MP2_GetHDR:
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
	UNWIND(.pad #52)
	sub	sp, sp, #52
	add	r9, r0, #77824
	add	r5, r0, #78848
	add	r7, r0, #114688
	ldr	r3, [r9, #2680]
	add	r5, r5, #576
	mov	r1, #32
	ldr	r10, [r9, #2684]
	mov	r6, r0
	mov	r8, #0
	mov	r0, r5
	strb	r8, [r7]
	str	r3, [fp, #-80]
	bl	BsShow
	ldr	r3, [r9, #1356]
	str	r3, [fp, #-84]
	mov	r4, r0
	mov	r0, r5
	bl	BsPos
	mov	r1, #32
	str	r0, [r9, #1416]
	mov	r0, r5
	bl	BsSkip
	movw	r3, #439
	cmp	r4, r3
	beq	.L924
	movw	r3, #442
	cmp	r4, r3
	beq	.L925
	movw	r3, #435
	cmp	r4, r3
	cmpne	r4, #440
	bne	.L844
	ldr	r3, [r9, #1412]
	cmp	r3, #1
	beq	.L926
.L849:
	movw	r3, #435
	cmp	r4, r3
	beq	.L854
	cmp	r4, #436
	bcc	.L927
	movw	r3, #437
	cmp	r4, r3
	beq	.L858
	cmp	r4, #436
	bls	.L875
	movw	r3, #439
	cmp	r4, r3
	beq	.L875
	cmp	r4, #440
	bne	.L853
	ldr	r4, .L932
	mov	r0, r5
	ldr	r3, [r9, #1408]
	ldr	r7, [r9, #1304]
	ldr	r8, [r4, #68]
	add	r7, r3, r7
	bl	BsPos
	ldr	r1, .L932+4
	add	r2, r7, r0, lsr #3
	mov	r0, #22
	blx	r8
	mov	r0, r6
	bl	group_of_pictures_header
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	blt	.L928
.L875:
	mov	r5, #0
.L846:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L844:
	movw	r3, #439
	cmp	r4, #256
	cmpne	r4, r3
	bne	.L849
	ldr	r3, [r9, #1412]
	cmp	r3, #1
	bne	.L849
	b	.L926
.L927:
	cmp	r4, #256
	beq	.L856
	bcc	.L853
	cmp	r4, #432
	bls	.L853
	ldr	r7, .L932
	mov	r0, r5
	ldr	r3, [r9, #1408]
	ldr	r4, [r9, #1304]
	ldr	r5, [r7, #68]
	add	r4, r3, r4
	bl	BsPos
	ldr	r1, .L932+8
	add	r2, r4, r0, lsr #3
	mov	r0, #22
	blx	r5
	mov	r0, r6
	bl	MP2_GetUsrData
	subs	r5, r0, #0
	bne	.L869
	add	r4, r6, #110592
	add	r1, r6, #78848
	add	r1, r1, #284
	ldr	r0, [r4, #4080]
	bl	MP2_ClearCurPacket
	ldr	r3, [r7, #68]
	ldr	r1, .L932+12
	mov	r0, #1
	blx	r3
	ldr	r3, .L932+16
	ldr	ip, [r3]
	cmp	ip, #0
	mvneq	r5, #0
	beq	.L846
	mov	r3, r5
	mov	r2, r5
	ldr	r0, [r4, #4080]
	mov	r1, #113
	blx	ip
	mvn	r5, #0
	b	.L846
.L926:
	ldr	r2, [r9, #1356]
	tst	r2, #4194304
	beq	.L849
	strb	r3, [r7]
	orr	r2, r2, #8388608
	mov	r0, r6
	str	r2, [r9, #1356]
	mov	r2, #0
	str	r2, [r9, #1412]
	mov	r5, r3
	bl	MP2_ProtocolPPSCheck
	b	.L846
.L856:
	ldr	r4, .L932
	mov	r0, r5
	ldr	r3, [r9, #1408]
	ldr	r7, [r9, #1304]
	ldr	r8, [r4, #68]
	add	r7, r3, r7
	bl	BsPos
	ldr	r1, .L932+20
	add	r2, r7, r0, lsr #3
	mov	r0, #22
	blx	r8
	mov	r0, r6
	bl	picture_header
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	bge	.L875
	ldr	r3, .L932+16
	add	r5, r6, #110592
	ldr	r2, [fp, #-84]
	ldr	ip, [r3]
	str	r2, [r9, #1356]
	cmp	ip, #0
	beq	.L868
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r5, #4080]
	blx	ip
.L868:
	add	r1, r6, #78848
	ldr	r0, [r5, #4080]
	add	r1, r1, #284
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	ldr	r3, [r4, #68]
	mov	r2, #256
	ldr	r1, .L932+24
	mov	r0, #1
	blx	r3
	b	.L846
.L924:
	ldr	r3, .L932
	mov	r2, #32
	mov	r1, r5
	sub	r0, fp, #76
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, #32
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #32
	str	r0, [fp, #-88]
	sub	r0, fp, #76
	bl	BsGet
	ldr	r3, [fp, #-88]
	movw	r1, #20036
	movw	r2, #20553
	movt	r1, 17221
	movt	r2, 18515
	cmp	r0, r1
	cmpeq	r3, r2
	bne	.L844
	ldr	r5, [r9, #1412]
	cmp	r5, #1
	beq	.L929
.L845:
	mov	r3, #1
	mov	r5, #0
	strb	r3, [r7, #2]
	b	.L846
.L925:
	ldr	r3, .L932
	mov	r2, #32
	mov	r1, r5
	sub	r0, fp, #76
	ldr	r3, [r3, #52]
	blx	r3
	mov	r1, #32
	sub	r0, fp, #76
	bl	BsGet
	mov	r1, #32
	str	r0, [fp, #-88]
	sub	r0, fp, #76
	bl	BsGet
	ldr	r3, [fp, #-88]
	movw	r1, #20036
	movw	r2, #20553
	movt	r1, 17221
	movt	r2, 18515
	cmp	r0, r1
	cmpeq	r3, r2
	bne	.L849
	ldr	r5, [r9, #1412]
	cmp	r5, #1
	bne	.L875
	ldr	r3, [r9, #1356]
	tst	r3, #4194304
	beq	.L875
	strb	r5, [r7]
	orr	r3, r3, #8388608
	str	r8, [r9, #1412]
	mov	r0, r6
	str	r3, [r9, #1356]
	bl	MP2_ProtocolPPSCheck
	b	.L846
.L853:
	sub	r3, r4, #256
	sub	r3, r3, #1
	cmp	r3, #174
	bhi	.L871
	ldr	r3, [r9, #1352]
	cmp	r3, #0
	beq	.L872
.L874:
	ldr	r3, [r9, #1356]
	tst	r3, #589824
	beq	.L875
	orr	r3, r3, #4194304
	mov	r0, r6
	str	r3, [r9, #1356]
	mov	r6, #1
	mov	r1, r4
	str	r6, [r9, #1412]
	bl	MP2_DecSlice
	cmp	r0, r6
	mov	r5, r0
	beq	.L846
	ldr	r3, .L932
	mov	r2, r4
	mov	r0, r6
	ldr	r1, .L932+28
	mov	r5, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L846
.L858:
	ldr	r2, .L932
	mov	r0, r5
	ldr	r3, [r9, #1408]
	ldr	r4, [r9, #1304]
	ldr	r7, [r2, #68]
	add	r4, r3, r4
	bl	BsPos
	ldr	r1, .L932+32
	add	r2, r4, r0, lsr #3
	mov	r0, #22
	blx	r7
	mov	r0, r6
	bl	extension
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	bge	.L875
	ldr	r3, .L932+16
	add	r5, r6, #110592
	ldr	r2, [fp, #-84]
	ldr	ip, [r3]
	str	r2, [r9, #1356]
	cmp	ip, #0
	beq	.L870
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r5, #4080]
	blx	ip
.L870:
	add	r1, r6, #78848
	ldr	r0, [r5, #4080]
	add	r1, r1, #284
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	b	.L846
.L854:
	ldr	r4, .L932
	mov	r0, r5
	ldr	r3, [r9, #1408]
	ldr	r7, [r9, #1304]
	ldr	r8, [r4, #68]
	add	r7, r3, r7
	bl	BsPos
	ldr	r1, .L932+36
	add	r2, r7, r0, lsr #3
	mov	r0, #22
	blx	r8
	mov	r0, r6
	bl	sequence_header
	mov	r0, r5
	bl	BsResidBits
	cmp	r0, #0
	blt	.L930
	ldr	r5, [r9, #1356]
	ands	r5, r5, #7
	bne	.L863
	ldr	r2, [fp, #-80]
	mov	r3, #1
	mov	r0, r10
	strb	r3, [r2]
	mov	r1, r2
	mov	r2, #188
	bl	memcpy
	ldr	r3, [r9, #1360]
	add	r3, r3, #1
	str	r3, [r9, #1360]
	b	.L846
.L863:
	ldrb	r3, [r10]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L931
.L871:
	ldr	r3, .L932+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L875
	add	r6, r6, #110592
	mov	r3, #0
	mov	r2, r3
	mov	r1, #113
	ldr	r0, [r6, #4080]
	mov	r5, r3
	blx	ip
	b	.L846
.L869:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L932+40
	blx	r3
	b	.L846
.L929:
	ldr	r3, [r9, #1356]
	tst	r3, #4194304
	beq	.L845
	strb	r5, [r7, #2]
	orr	r3, r3, #8388608
	strb	r5, [r7]
	mov	r0, r6
	str	r3, [r9, #1356]
	str	r8, [r9, #1412]
	bl	MP2_ProtocolPPSCheck
	b	.L846
.L872:
	mov	r0, r6
	bl	MP2_GetSequenceInfo
	ldr	r3, [r9, #1404]
	cmp	r3, #0
	beq	.L875
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L874
	b	.L875
.L931:
	mov	r2, #188
	mov	r1, r10
	ldr	r0, [fp, #-80]
	bl	memcpy
	ldr	r2, .L932+16
	ldr	r3, [r9, #1356]
	ldr	r4, [r2]
	bic	r3, r3, #7
	str	r3, [r9, #1356]
	cmp	r4, #0
	beq	.L875
	add	r6, r6, #110592
	mov	r3, #0
	mov	r2, r3
	mov	r1, #113
	ldr	r0, [r6, #4080]
	mov	r5, r3
	blx	r4
	b	.L846
.L928:
	ldr	r3, .L932+16
	add	r5, r6, #110592
	ldr	r2, [fp, #-84]
	ldr	ip, [r3]
	str	r2, [r9, #1356]
	cmp	ip, #0
	beq	.L867
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r5, #4080]
	blx	ip
.L867:
	add	r1, r6, #78848
	ldr	r0, [r5, #4080]
	add	r1, r1, #284
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	ldr	r3, [r4, #68]
	mov	r2, #440
	ldr	r1, .L932+44
	mov	r0, #1
	blx	r3
	b	.L846
.L930:
	ldr	r3, .L932+16
	add	r5, r6, #110592
	ldr	r2, [fp, #-84]
	ldr	ip, [r3]
	str	r2, [r9, #1356]
	cmp	ip, #0
	beq	.L862
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r5, #4080]
	blx	ip
.L862:
	add	r1, r6, #78848
	ldr	r0, [r5, #4080]
	add	r1, r1, #284
	mvn	r5, #0
	bl	MP2_ClearCurPacket
	ldr	r3, [r4, #68]
	movw	r2, #435
	ldr	r1, .L932+48
	mov	r0, #1
	blx	r3
	b	.L846
.L933:
	.align	2
.L932:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC66
	.word	.LC70
	.word	.LC71
	.word	g_event_report
	.word	.LC68
	.word	.LC69
	.word	.LC74
	.word	.LC73
	.word	.LC64
	.word	.LC72
	.word	.LC67
	.word	.LC65
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r7, .L971
	add	r5, r0, #79872
	add	r6, r0, #45312
	add	r5, r5, #96
	add	r8, r0, #110592
	add	r6, r6, #16
	ldr	r3, [r7, #52]
	mov	r2, #160
	mov	r4, r0
	mov	r0, r5
	blx	r3
	mov	r1, r5
	mov	r0, r6
	ldr	r2, [r8, #4080]
	bl	MP2_ReceivePacket
	cmp	r0, #1
	beq	.L935
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L971+4
	mov	r5, #0
	blx	r3
.L936:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L935:
	add	r9, r4, #77824
	add	r0, r4, #78848
	add	r0, r0, #576
	ldr	r2, [r9, #1328]
	ldr	r1, [r9, #1312]
	bl	BsInit
	ldr	r3, [r4]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L966
.L937:
	ldr	r3, [r9, #1356]
	str	r3, [r9, #1540]
.L938:
	mov	r0, r4
	bl	MP2_GetHDR
	add	r0, r0, #1
	cmp	r0, #1
	bls	.L967
	mov	r0, r4
	bl	MP2_DecodingDecide
	subs	r5, r0, #0
	beq	.L968
	cmn	r5, #1
	beq	.L941
	mov	r0, r4
	bl	MP2_GetPictureInfo
	subs	r5, r0, #0
	bne	.L944
	add	r1, r6, #33792
	ldr	r0, [r8, #4080]
	add	r1, r1, #12
	bl	MP2_ClearCurPacket
	mov	r0, r4
	bl	MP2_ClearCurPic
	ldr	r3, .L971+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L945
	ldr	r0, [r8, #4080]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L945:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L971+12
	mov	r5, #0
	blx	r3
	b	.L936
.L967:
	ldr	r3, [r4]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L969
.L940:
	add	r4, r4, #114688
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L941
	add	r1, r6, #33792
	ldr	r0, [r8, #4080]
	add	r1, r1, #12
	mov	r5, #0
	bl	MP2_ClearCurPacket
	b	.L936
.L969:
	ldr	r3, [r3, #892]
	cmp	r3, #0
	beq	.L940
.L941:
	mov	r5, #0
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L966:
	ldr	r3, [r3, #892]
	cmp	r3, #1
	ldreq	r3, [r9, #1540]
	streq	r3, [r9, #1356]
	bne	.L937
	b	.L938
.L968:
	add	r1, r6, #33792
	ldr	r0, [r8, #4080]
	add	r1, r1, #12
	bl	MP2_ClearCurPacket
	mov	r0, r4
	bl	MP2_ClearCurPic
	ldr	r3, .L971+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L943
	ldr	r0, [r8, #4080]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L943:
	ldr	r3, [r7, #68]
	mov	r0, #22
	ldr	r1, .L971+16
	mov	r5, #0
	blx	r3
	b	.L936
.L944:
	mov	r0, r4
	bl	MP2_DecNewPic
	subs	r5, r0, #0
	beq	.L970
	ldr	r5, [r9, #1304]
	ldr	r3, [r9, #1408]
	ldr	r2, [r9, #1328]
	mov	r5, r5, asl #3
	add	r3, r3, r2
	str	r3, [r9, #1408]
	b	.L936
.L970:
	mov	r2, #1
	ldr	r1, [r9, #1440]
	ldr	r0, [r8, #4080]
	bl	FSP_ClearLogicFs
	add	r1, r6, #33792
	add	r1, r1, #12
	ldr	r0, [r8, #4080]
	bl	MP2_ClearCurPacket
	mov	r0, r4
	bl	MP2_ClearCurPic
	ldr	r3, .L971+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L947
	ldr	r0, [r8, #4080]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L947:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L971+20
	blx	r3
	b	.L936
.L972:
	.align	2
.L971:
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
	add	r0, r0, #114688
	ldrb	r3, [r0]	@ zero_extendqisi2
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
	.type	__FUNCTION__.13712, %object
	.size	__FUNCTION__.13712, 19
__FUNCTION__.13712:
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
	.type	CSWTCH.46, %object
	.size	CSWTCH.46, 12
CSWTCH.46:
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
	ASCII(.ascii	"new seq size %dx%d\012\000" )
.LC21:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC24:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"ERROR: frm %d, lost one field\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"ERROR: frm %d, lost one field flg = %d, last_struct" )
	ASCII(.ascii	" = %d\012\000" )
	.space	2
.LC27:
	ASCII(.ascii	"Fatal Error: frm%d, get image buffer failed\012\000" )
	.space	3
.LC28:
	ASCII(.ascii	"dec_pts: %lld Usertag: %lld\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"---------- unref %d\012\000" )
	.space	3
.LC30:
	ASCII(.ascii	"++++++++++ ref %d\012\000" )
	.space	1
.LC31:
	ASCII(.ascii	"line: %d(pCur, pFwd, pBwd)(%p, %p, %p)\012\000" )
.LC32:
	ASCII(.ascii	"CurFsID(fwd%d,%d,bwd%d) abnormal!(%p,%p,%p)\012\000" )
	.space	3
.LC33:
	ASCII(.ascii	"dec addr: cur,cur2d,fwd,bwd,pmv = %p,%p,%p,%p,%p\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC34:
	ASCII(.ascii	"\012mpeg2 copy last 3 bytes of the first packet!\012" )
	ASCII(.ascii	"\012\000" )
.LC35:
	ASCII(.ascii	"pMp2CodecInfo->pUsrDatArray[%d] is null\012\000" )
	.space	3
.LC36:
	ASCII(.ascii	"ERROR: unsupported profile %d\012\000" )
	.space	1
.LC37:
	ASCII(.ascii	"ERROR: unsupported level %d\012\000" )
	.space	3
.LC38:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC39:
	ASCII(.ascii	"Decoding quits at frm %d\012\000" )
	.space	2
.LC40:
	ASCII(.ascii	"ERROR: frm %d, Invalid packet pointer!\012\000" )
.LC41:
	ASCII(.ascii	"line %d, pPacket->StreamPack[1].IsLastSeg = %d\012\000" )
.LC42:
	ASCII(.ascii	"ErrRatio = %d\012\000" )
	.space	1
.LC43:
	ASCII(.ascii	"pImg is null, pCurImg %#x pBwdImg %#x pFwdImg %#x\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC44:
	ASCII(.ascii	"DEBUG: DEC over\012\000" )
	.space	3
.LC45:
	ASCII(.ascii	"err(%d) > out_thr(%d)\012\000" )
	.space	1
.LC46:
	ASCII(.ascii	"insert buf %d ref %d\012\000" )
	.space	2
.LC47:
	ASCII(.ascii	"%-40s%20d\012\000" )
	.space	1
.LC48:
	ASCII(.ascii	"slice_vertical_position\000" )
.LC49:
	ASCII(.ascii	"quantiser_scale_code\000" )
	.space	3
.LC50:
	ASCII(.ascii	"extra_bit_slice\000" )
.LC51:
	ASCII(.ascii	"intra_slice_flag\000" )
	.space	3
.LC52:
	ASCII(.ascii	"intra_slice\000" )
.LC53:
	ASCII(.ascii	"reserved_bits\000" )
	.space	2
.LC54:
	ASCII(.ascii	"extra_infrmation_slice\000" )
	.space	1
.LC55:
	ASCII(.ascii	"Slice num = %d >= %d\012\000" )
	.space	2
.LC56:
	ASCII(.ascii	"seg info is wrong! IsLastSeg[0] = %d, IsLastSeg[1] " )
	ASCII(.ascii	"= %d\012\000" )
	.space	3
.LC57:
	ASCII(.ascii	"macroblock_address_increment\000" )
	.space	3
.LC58:
	ASCII(.ascii	"macroblock_escape ERROR\012\000" )
	.space	3
.LC59:
	ASCII(.ascii	"macroblock_escape\000" )
	.space	2
.LC60:
	ASCII(.ascii	"SlcStartMbNum = %d > PicEndMum = %d\012\000" )
	.space	3
.LC61:
	ASCII(.ascii	"FirstMbInSliceFlag = %d\012\000" )
	.space	3
.LC62:
	ASCII(.ascii	"MP2_GetFirstMbInSlice Failed!\012\000" )
	.space	1
.LC63:
	ASCII(.ascii	"MP2_WriteSliceInfo Failed!\012\000" )
.LC64:
	ASCII(.ascii	"found sh %#x\012\000" )
	.space	2
.LC65:
	ASCII(.ascii	"SEQUENCE_HEADER_CODE Bitstream is not enough!\012\000" )
	.space	1
.LC66:
	ASCII(.ascii	"found gh %#x\012\000" )
	.space	2
.LC67:
	ASCII(.ascii	"GROUP_START_CODE Bitstream is not enough!\012\000" )
	.space	1
.LC68:
	ASCII(.ascii	"found ph %#x\012\000" )
	.space	2
.LC69:
	ASCII(.ascii	"PICTURE_START_CODE Bitstream is not enough!\012\000" )
	.space	3
.LC70:
	ASCII(.ascii	"found ud %#x\012\000" )
	.space	2
.LC71:
	ASCII(.ascii	"get usd err\012\000" )
	.space	3
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
