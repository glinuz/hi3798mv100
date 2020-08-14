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
	.file	"pq_mng_ifmd_alg.c"
	.text
	.align	2
	.type	DetectSceneChange, %function
DetectSceneChange:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, [r1, #128]
	mov	ip, #0
	ldr	r3, [r1, #124]
	mov	lr, r0
	ldr	r5, [r1, #132]
	mov	r2, ip
	add	r3, r3, r4, lsl #1
	ldr	r1, [r0]
	add	r5, r3, r5, lsl #2
	mov	r4, r1
	mov	r5, r5, asl #1
.L4:
	ldr	r3, [lr], #4
	cmp	r1, r3
	add	ip, ip, r3
	movge	r1, r3
	cmp	r4, r3
	movlt	r4, r3
	cmp	r2, #4
	add	r2, r2, #1
	strgt	r5, [r0, #20]
	ldrle	r3, [lr]
	strle	r3, [lr, #-4]
	cmp	r2, #6
	bne	.L4
	rsb	ip, r1, ip
	mov	r3, #3392
	rsb	ip, r4, ip
	movt	r3, 3
	add	r2, ip, #3
	cmp	ip, #0
	movlt	ip, r2
	mov	ip, ip, asr #2
	cmp	r5, ip, asl #1
	movgt	r0, #1
	movle	r0, #0
	cmp	r5, r3
	movle	r0, #0
	andgt	r0, r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	DetectSceneChange, .-DetectSceneChange
	.align	2
	.global	calc_MCorMA
	.type	calc_MCorMA, %function
calc_MCorMA:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	lr, [fp, #4]
	rsb	r1, r0, r1
	movw	r4, #34464
	movt	r4, 1
	ldrsb	ip, [lr, #100]
	cmp	ip, #100
	movge	ip, #100
	cmp	r1, r4
	bic	ip, ip, ip, asr #31
	uxtb	ip, ip
	strb	ip, [lr, #100]
	ble	.L8
	add	r3, ip, #10
	uxtb	r3, r3
	strb	r3, [lr, #100]
.L9:
	sxtb	r3, r3
	cmp	r3, #59
	movgt	r3, #0
	movle	r3, #1
	str	r3, [lr, #96]
	ldmfd	sp, {r4, fp, sp, pc}
.L8:
	rsb	r2, r3, r2
	movw	r3, #20000
	cmp	r1, r3
	cmpgt	r2, #1000
	addgt	r3, ip, #6
	suble	r3, ip, #5
	uxtb	r3, r3
	strb	r3, [lr, #100]
	b	.L9
	UNWIND(.fnend)
	.size	calc_MCorMA, .-calc_MCorMA
	.align	2
	.type	Calc22PLD_4FLD, %function
Calc22PLD_4FLD:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	mov	r4, r2
	ldr	r2, [r0, #4]
	clz	r1, r1
	ldr	r5, [r0, #40]
	mov	r1, r1, lsr #5
	ldr	lr, [r0, #32]
	str	r2, [fp, #-48]
	cmp	r1, #1
	ldr	r2, [r0, #8]
	ldr	r7, [r0, #28]
	ldr	r6, [r0, #44]
	str	r2, [fp, #-52]
	ldr	r2, [r0, #12]
	str	r3, [fp, #-64]
	ldr	r3, [r0, #24]
	moveq	r3, r5
	str	r2, [fp, #-60]
	moveq	r5, r6
	ldr	r2, [r0, #16]
	ldr	r10, [r0, #124]
	moveq	r6, r3
	ldreq	lr, [r4, #40]
	moveq	r3, r7
	str	r2, [fp, #-72]
	movne	r2, r7
	ldreq	r2, [r4, #44]
	ldr	ip, [r0, #120]
	ldr	r9, [r0, #128]
	ldr	r8, [r0, #132]
	add	ip, ip, r10, lsl #1
	strne	lr, [r4, #40]
	strne	r7, [r4, #44]
	cmp	r2, lr
	add	r9, ip, r9, lsl #2
	movlt	ip, r2
	movge	ip, lr
	cmp	r2, #80
	cmple	r3, #80
	add	r9, r9, r8, lsl #3
	mov	ip, ip, asl #1
	str	r9, [fp, #-56]
	str	ip, [fp, #-68]
	ble	.L16
	add	r9, r3, r3, lsl #1
	mov	ip, r2, asl #1
	cmp	r9, ip
	bgt	.L78
	add	r9, r2, r2, lsl #2
	add	r9, r9, r9, lsr #31
	cmp	lr, r9, asr #1
	bgt	.L19
	cmp	lr, #1000
	ble	.L21
	add	r9, lr, lr, lsl #2
	rsb	r10, r2, r2, lsl #3
	cmp	r9, r10
	bge	.L19
.L21:
	cmp	r3, #99
	bgt	.L79
	mov	r9, r3, asl #3
	str	r9, [fp, #-60]
	cmp	r2, r9
	blt	.L24
	cmp	r3, #10
	mov	r9, lr, asl #1
	movge	r10, r3
	movlt	r10, #10
	cmp	r9, r10, asl #3
	blt	.L24
.L25:
	ldr	r9, [fp, #-60]
	cmp	r9, ip
	bge	.L26
	cmp	r9, lr, asl #1
	bge	.L80
.L27:
	add	r8, r8, #100
	add	ip, r4, r1
	cmp	r8, r3, asl #2
	ldrltb	lr, [ip, #88]
	movlt	r8, #1
	ldrgeb	lr, [ip, #88]
	movge	r8, #0
	sublt	lr, lr, #15
	addge	lr, lr, #1
	strb	lr, [ip, #88]
	b	.L18
.L78:
	add	ip, r4, r1
	mov	r8, #1
	mov	lr, #0
	strb	lr, [ip, #88]
.L18:
	cmp	r5, #29
	movle	ip, #1
	movgt	ip, #0
	cmp	r6, #400
	movle	ip, #0
	cmp	ip, #0
	bne	.L47
.L58:
	mov	r5, r5, asl #5
	cmp	r5, #127
	ble	.L47
	add	ip, r4, r1
.L63:
	ldrb	r1, [ip, #88]
	sub	r1, r1, #5
	strb	r1, [ip, #88]
.L47:
	cmp	r8, #1
	beq	.L62
.L48:
	mov	lr, #0
.L43:
	ldrsb	ip, [r4, #88]
	ldrsb	r1, [r4, #89]
	cmp	ip, #32
	str	lr, [r4, #36]
	movge	ip, #32
	cmp	r1, #32
	bic	ip, ip, ip, asr #31
	movge	r1, #32
	bic	r1, r1, r1, asr #31
	uxtb	ip, ip
	strb	ip, [r4, #88]
	uxtb	r1, r1
	sxtb	ip, ip
	strb	r1, [r4, #89]
	sxtb	r1, r1
	cmp	ip, #24
	cmpgt	r1, #24
	movgt	r8, #1
	movgt	lr, #1
	movle	lr, #0
	strgt	r8, [r4, #92]
	bgt	.L50
	cmp	r1, #19
	cmpgt	ip, #19
	strle	lr, [r4, #92]
	ldrgt	r8, [r4, #92]
	movle	r8, lr
.L50:
	ldr	r9, [r4, #12]
	movw	r6, #26215
	ldr	r7, [r4, #8]
	movt	r6, 26214
	ldr	r5, [r4, #4]
	movw	ip, #62537
	add	r1, r9, r7
	ldr	lr, [r4]
	ldr	r0, [r0]
	add	r1, r5, r1
	add	r1, lr, r1
	str	r7, [r4, #12]
	add	r1, r0, r1
	str	r9, [r4, #16]
	ldr	r9, [r4, #28]
	movt	ip, 65535
	smull	r6, r7, r1, r6
	mov	r1, r1, asr #31
	ldr	r10, [r4, #20]
	str	r5, [r4, #8]
	stmia	r4, {r0, lr}
	rsb	r1, r1, r7, asr #1
	str	r1, [r4, #24]
	add	r9, r1, r9
	rsb	r10, r10, r9
	str	r10, [r4, #28]
	cmp	r10, ip
	blt	.L52
	movw	r0, #3000
	cmp	r10, r0
	str	r1, [r4, #20]
	movge	r10, r0
	cmp	r8, #0
	str	r10, [r4, #28]
	beq	.L60
	cmp	r10, #1000
	movge	r1, #0
	strgeb	r1, [r4, #88]
	strgeb	r1, [r4, #89]
	strge	r1, [r4, #92]
.L54:
	str	r4, [sp]
	ldr	r1, [fp, #-52]
	ldr	r0, [fp, #-48]
	bl	calc_MCorMA
	ldr	r3, [fp, #-56]
	str	r3, [r4, #32]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L52:
	cmp	r8, #0
	movw	r0, #62536
	movt	r0, 65535
	str	r1, [r4, #20]
	str	r0, [r4, #28]
	bne	.L54
.L60:
	mov	r1, #0
	str	r1, [r4, #28]
	b	.L54
.L16:
	ldr	r7, [fp, #-48]
	movw	ip, #5499
	cmp	r7, ip
	bgt	.L32
	ldr	r7, [fp, #-52]
	ldr	r8, [fp, #-48]
	cmp	r7, #0
	moveq	r7, #20
	cmp	r8, r7
	bgt	.L32
	ldr	r7, [fp, #-60]
	cmp	r7, ip
	bgt	.L33
	ldr	ip, [fp, #-72]
	add	ip, ip, #150
	cmp	r7, ip
	ble	.L34
.L33:
	ldr	ip, [fp, #-64]
	cmp	ip, #0
	beq	.L32
.L34:
	cmp	r3, #39
	ble	.L35
	cmp	r3, r2, asl #1
	bge	.L32
.L35:
	cmp	lr, #80
	bgt	.L81
.L37:
	ldr	ip, [fp, #-52]
	cmp	ip, #1000
	ble	.L39
	mov	lr, ip, asl #1
	ldr	ip, [fp, #-48]
	mov	ip, ip, asl #4
	cmp	lr, ip
	ble	.L40
	ldr	r7, [fp, #-64]
	cmp	r7, #0
	bne	.L41
	ldr	r8, [fp, #-60]
	ldr	r7, [fp, #-72]
	cmp	r7, r8, asl #1
	ble	.L40
.L41:
	ldr	lr, [fp, #-52]
	movw	ip, #9000
	cmp	lr, ip
	ble	.L36
	cmp	r5, #29
	add	ip, r4, r1
	movle	r8, #1
	movgt	r8, #0
	cmp	r6, #400
	ldrb	r1, [ip, #88]
	movle	r8, #0
	cmp	r8, #0
	add	r1, r1, #1
	strb	r1, [ip, #88]
	bne	.L48
	mov	r5, r5, asl #5
	cmp	r5, #127
	bgt	.L63
	b	.L48
.L32:
	add	ip, r4, r1
	mov	lr, #0
	strb	lr, [ip, #88]
.L36:
	cmp	r5, #29
	movle	ip, #1
	movgt	ip, #0
	cmp	r6, #400
	movle	ip, #0
	cmp	ip, #0
	moveq	r8, #1
	beq	.L58
.L62:
	ldr	lr, [r4, #36]
	add	lr, lr, #1
	cmp	lr, #50
	movge	lr, #50
	bic	lr, lr, lr, asr #31
	b	.L43
.L19:
	add	ip, r4, r1
	mov	r8, #1
	ldrb	lr, [ip, #88]
	sub	lr, lr, #1
	strb	lr, [ip, #88]
	b	.L18
.L79:
	cmp	r3, #500
	add	ip, r4, r1
	movlt	lr, r3, asr #31
	movwlt	r8, #34079
	ldrb	r7, [ip, #88]
	movtlt	r8, 20971
	movge	lr, #5
	smulllt	r8, r9, r3, r8
	mov	r8, #1
	rsblt	lr, lr, r9, asr #5
	uxtblt	lr, lr
	rsb	r7, lr, r7
	strb	r7, [ip, #88]
	b	.L18
.L24:
	rsb	r9, lr, r2
	cmp	r9, #0
	rsblt	r9, r9, #0
	mov	r10, r9
	ldr	r9, [fp, #-68]
	cmp	r9, #150
	movlt	r9, #150
	add	r9, r9, #50
	cmp	r10, r9
	ble	.L25
	ldr	r9, [r4, #32]
	cmp	r9, #10
	ble	.L25
	add	ip, r4, r1
	mov	r8, #1
	ldrb	lr, [ip, #88]
	sub	lr, lr, #5
	strb	lr, [ip, #88]
	b	.L18
.L39:
	add	ip, r4, r1
	ldrb	lr, [ip, #88]
	sub	lr, lr, #1
	strb	lr, [ip, #88]
	b	.L36
.L80:
	ldr	ip, [r4, #32]
	cmp	ip, #9
	ble	.L27
.L26:
	add	ip, r2, #150
	cmp	r3, ip
	ble	.L82
.L29:
	ldr	r8, [r4, #92]
	add	ip, r4, r1
	cmp	r8, #1
	ldreqb	lr, [ip, #88]
	movne	r8, #1
	ldrneb	lr, [ip, #88]
	subeq	lr, lr, #1
	subne	lr, lr, #3
	strb	lr, [ip, #88]
	b	.L18
.L81:
	rsb	lr, lr, r2
	ldr	ip, [fp, #-68]
	cmp	lr, #0
	add	r9, ip, #50
	rsblt	lr, lr, #0
	cmp	lr, r9
	ble	.L37
	ldr	ip, [r4, #32]
	cmp	ip, #10
	ble	.L37
	ldr	lr, [r4, #92]
	cmp	lr, #1
	beq	.L45
	movw	lr, #5000
	cmp	ip, lr
	ble	.L36
.L45:
	add	ip, r4, r1
	ldr	lr, [r4, #36]
	ldrb	r7, [ip, #88]
	mvn	lr, lr
	add	lr, lr, r7
	strb	lr, [ip, #88]
	b	.L36
.L82:
	add	r7, r7, #150
	cmp	r3, r7
	movle	r8, #1
	bgt	.L29
	b	.L18
.L40:
	ldr	r7, [r4, #92]
	cmp	r7, #1
	bne	.L39
	cmp	lr, ip
	bge	.L45
	ldr	ip, [fp, #-64]
	cmp	ip, #0
	bne	.L46
	ldr	ip, [fp, #-60]
	ldr	lr, [fp, #-72]
	mov	ip, ip, asl #4
	cmp	ip, lr, asl #1
	ble	.L45
.L46:
	add	ip, r4, r1
	ldr	r7, [r4, #36]
	ldrb	lr, [ip, #88]
	mvn	r7, r7
	add	lr, lr, r7, lsl #1
	strb	lr, [ip, #88]
	b	.L36
	UNWIND(.fnend)
	.size	Calc22PLD_4FLD, .-Calc22PLD_4FLD
	.global	__aeabi_idiv
	.align	2
	.global	PQ_MNG_ALG_FmdSet
	.type	PQ_MNG_ALG_FmdSet, %function
PQ_MNG_ALG_FmdSet:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #268)
	sub	sp, sp, #268
	ldr	r3, [r0]
	mov	r4, r0
	mov	r5, r1
	cmp	r3, #0
	mov	r6, r2
	beq	.L352
	mov	r2, #28
	mov	r1, #0
	mov	r0, r6
	bl	memset
	ldr	r3, [r5, #68]
	cmp	r3, #1
	beq	.L353
	ldrb	r3, [r4, #7]
	cmp	r3, #255
	beq	.L87
	ldrb	r3, [r4, #8]
	cmp	r3, #255
	beq	.L87
	ldrb	r3, [r4, #9]
	cmp	r3, #255
	beq	.L87
	ldrb	r3, [r4, #4]
	cmp	r3, #255
	beq	.L89
.L360:
	ldrb	r3, [r4, #5]
	cmp	r3, #255
	beq	.L89
.L90:
	ldr	r3, [r5, #48]
	cmp	r3, #0
	ldrneb	r1, [r4, #9]
	ldreqb	r1, [r4, #8]
	str	r1, [r4, #20]
	ldr	r3, [r5, #48]
	cmp	r3, #0
	ldrneb	r3, [r4, #6]
	ldreqb	r3, [r4, #5]
	str	r3, [r4, #24]
	ldr	r2, [r5, #48]
	cmp	r2, #0
	movne	r2, #1
	moveq	r2, #2
	str	r2, [r4, #28]
	ldr	r2, [r5, #48]
	cmp	r2, #0
	ldrne	r2, [r5, #64]
	ldrne	r0, [r5, #60]
	ldreqb	r2, [r4, #6]
	ldreqb	r0, [r4, #9]
	orrs	ip, r3, r1
	moveq	r3, ip
	beq	.L104
	cmp	r3, #1
	cmpeq	r1, #1
	moveq	r3, #1
	movne	r3, #0
	eor	r3, r3, #1
.L104:
	orrs	r1, r2, r0
	beq	.L105
	cmp	r2, #1
	cmpeq	r0, #1
	moveq	r2, #1
	movne	r2, #0
	eor	r1, r2, #1
.L105:
	ldr	ip, [r4, #456]
	add	r7, r4, #96
	ldr	r0, [r4, #460]
	mov	r2, r4
	str	r3, [r4, #12]
	mov	r3, r4
	str	r1, [r4, #16]
	str	ip, [r4, #452]
	str	r0, [r4, #456]
	ldr	r1, [r4, #472]
	ldr	r0, [r5, #52]
	ldr	ip, [r4, #468]
	str	r1, [r4, #468]
	str	r0, [r4, #460]
	str	ip, [r4, #464]
	ldr	r1, [r5, #56]
	str	r1, [r4, #472]
	ldr	r0, [r5, #44]
	ldr	r1, [r5, #40]
	mul	r1, r1, r0
	mov	r1, r1, asr #11
	str	r1, [r4, #476]
.L106:
	ldrh	ip, [r3, #150]
	add	r3, r3, #6
	ldrh	r0, [r3, #142]
	strh	ip, [r3, #146]
	strh	r0, [r3, #144]
	cmp	r3, r7
	bne	.L106
	add	r0, r5, #232
	mov	r3, #0
.L108:
	ldrh	ip, [r2, #152]
	cmp	r1, ip
	ble	.L107
	ldrh	ip, [r2, #148]
	cmp	r1, ip
	ble	.L107
	ldrh	ip, [r2, #150]
	cmp	r1, ip
	addgt	r3, r3, #1
.L107:
	ldr	ip, [r0, #4]!
	add	r2, r2, #6
	strh	ip, [r2, #142]
	cmp	r2, r7
	bne	.L108
	ldr	r2, [r5, #20]
	str	r3, [r5, #232]
	cmp	r2, #1
	beq	.L354
	cmp	r2, #2
	ldr	r10, [r4, #12]
	moveq	r3, #0
	streq	r3, [fp, #-248]
	beq	.L111
	cmp	r2, #3
	moveq	r3, #1
	movne	r3, #2
	str	r3, [fp, #-248]
.L111:
	add	r3, r4, #124
	add	r8, r5, #84
	str	r3, [fp, #-252]
	add	r9, r4, r10
	mov	r0, r3
	mov	r1, r8
	bl	DetectSceneChange
	cmp	r10, #0
	ldr	r3, [fp, #-252]
	streq	r10, [fp, #-256]
	streq	r10, [fp, #-252]
	strb	r0, [r9, #111]
	beq	.L161
	ldrsb	r1, [r4, #111]
	cmp	r1, #0
	bne	.L355
.L258:
	str	r1, [fp, #-252]
	str	r1, [fp, #-256]
.L161:
	mov	r0, r3
	mov	r1, r8
	bl	DetectSceneChange
	strb	r0, [r9, #111]
	ldr	r9, [r5, #24]
	cmp	r9, #0
	beq	.L162
	ldr	r3, .L373
	ldr	r3, [r3, #16]
	cmp	r3, #1
	beq	.L356
	ldr	r9, [r5, #76]
	cmp	r9, #1
	beq	.L357
.L170:
	ldr	r3, [r5, #40]
	cmp	r3, #288
	bgt	.L171
	ldr	r9, [r5, #72]
.L349:
	cmp	r9, #1
	beq	.L162
.L171:
	ldr	r3, [r4, #452]
	cmp	r3, #0
	beq	.L358
	ldr	r8, [r5, #28]
	ldr	r9, [r4, #252]
	cmp	r8, #0
	bne	.L359
	ldrsb	ip, [r4, #108]
	ldrsb	r3, [r4, #109]
	cmp	ip, #1
	beq	.L275
	cmp	ip, #2
	bne	.L222
	mov	r2, r8
.L213:
	ldr	r10, [r4, #28]
	b	.L219
.L87:
	ldr	r3, [r5, #60]
	strb	r3, [r4, #7]
	ldr	r3, [r5, #60]
	strb	r3, [r4, #8]
	ldr	r3, [r5, #60]
	strb	r3, [r4, #9]
	ldrb	r3, [r4, #4]
	cmp	r3, #255
	bne	.L360
.L89:
	ldr	r3, [r5, #48]
	cmp	r3, #0
	ldrne	r3, [r5, #60]
	ldreqb	r3, [r5, #60]
	clzne	r3, r3
	movne	r3, r3, lsr #5
	strb	r3, [r4, #4]
	ldr	r3, [r5, #48]
	cmp	r3, #0
	ldreq	r3, [r5, #60]
	ldrneb	r3, [r5, #60]
	clzeq	r3, r3
	moveq	r3, r3, lsr #5
	strb	r3, [r4, #5]
	ldr	r3, [r5, #48]
	cmp	r3, #0
	ldrne	r3, [r5, #60]
	ldreqb	r3, [r5, #60]
	clzne	r3, r3
	movne	r3, r3, lsr #5
	strb	r3, [r4, #6]
	b	.L90
.L356:
	ldr	r3, [r5, #76]
	ldr	r9, [r5, #72]
	cmp	r3, #1
	bne	.L349
	cmp	r9, #0
	bne	.L349
	ldr	r0, .L373+4
	ldr	r1, [r5, #80]
	ldr	ip, [r0]
	cmp	r1, ip
	bcc	.L171
	ldr	r0, [r0, #4]
	cmp	r1, r0
	bcs	.L171
	mov	r9, r3
.L162:
	ldr	r8, [r5, #28]
	cmp	r8, #0
	ldreq	r10, [r4, #28]
	bne	.L361
.L175:
	ldrsb	ip, [r4, #108]
	ldrsb	r3, [r4, #109]
	cmp	ip, #1
	beq	.L215
	cmp	ip, #2
	bne	.L223
	mov	r2, #0
.L219:
	cmp	r9, #1
	beq	.L345
	sub	ip, r2, #1
	mov	r3, #2
	clz	ip, ip
	mov	ip, ip, lsr #5
.L216:
	ldr	r7, .L373+8
	sub	lr, fp, #144
	strb	r3, [r4, #109]
	cmp	ip, #1
	strb	ip, [r4, #108]
	ldmia	r7!, {r0, r1, r2, r3}
	ldr	r9, [r5, #8]
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r7!, {r0, r1, r2, r3}
	str	r9, [fp, #-256]
	ldr	r9, [r4, #16]
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r7, {r0, r1}
	stmia	lr, {r0, r1}
	beq	.L225
	cmp	ip, #2
	beq	.L226
	cmp	ip, #0
	beq	.L227
	mov	r3, #0
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	tst	r10, #1
	ldr	r3, [fp, #-248]
	clzne	r9, r9
	movne	r9, r9, lsr #5
	cmp	ip, #2
	str	r3, [r6, #20]
	ldr	r3, [fp, #-252]
	str	r9, [r6]
	str	r3, [r6, #16]
	mov	r3, #0
	str	r3, [r6, #12]
	beq	.L270
	subs	ip, ip, #1
	mvnne	ip, #0
	mov	r3, ip
.L241:
	cmp	ip, #1
	bls	.L235
.L238:
	str	r3, [r6, #24]
	ldrb	r2, [r4, #8]
	ldrb	r3, [r4, #9]
	strb	r2, [r4, #7]
	strb	r3, [r4, #8]
	ldr	r3, [r5, #20]
	cmp	r3, #1
	beq	.L362
.L239:
	mov	r1, r6
	mov	r2, #28
	add	r0, r4, #480
	bl	memcpy
	ldrb	r3, [r4, #6]
	ldrb	r2, [r4, #5]
	mov	r0, #0
	strb	r3, [r4, #5]
	strb	r2, [r4, #4]
	ldr	r3, [r5, #64]
	strb	r3, [r4, #6]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L352:
	str	r3, [r2]
	mov	r1, r2
	str	r3, [r2, #8]
	str	r3, [r2, #4]
	mov	r2, #28
	str	r3, [r6, #12]
	ldr	r3, [r5, #60]
	str	r3, [r6, #20]
	ldr	r3, [r4]
	add	r3, r3, #1
	str	r3, [r0], #480
	bl	memcpy
.L334:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L222:
	ldr	r10, [r4, #28]
.L223:
	cmp	r9, #1
	beq	.L269
.L345:
	uxtb	r3, r3
	b	.L216
.L355:
	ldrsb	r1, [r4, #112]
	cmp	r1, #0
	beq	.L258
	mov	r2, #1
	str	r2, [fp, #-252]
	str	r2, [fp, #-256]
	b	.L161
.L357:
	ldr	r1, .L373+4
	ldr	r3, [r5, #80]
	ldr	r0, [r1]
	cmp	r3, r0
	bcc	.L170
	ldr	r1, [r1, #4]
	cmp	r3, r1
	bcc	.L162
	b	.L170
.L269:
	mov	r9, #0
.L217:
	strb	r9, [r4, #109]
	mov	r3, #2
	ldr	r9, [r4, #16]
	strb	r3, [r4, #108]
.L226:
	mov	r3, #1
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	tst	r10, #1
	ldr	r3, [fp, #-248]
	clzne	r9, r9
	mov	r2, #1
	str	r2, [r6, #12]
	movne	r9, r9, lsr #5
	str	r9, [r6]
	str	r3, [r6, #20]
	ldr	r3, [fp, #-252]
	str	r3, [r6, #16]
	mov	r3, r2
.L235:
	ldr	r2, .L373+12
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #140]
	b	.L238
.L362:
	ldr	r3, [r6, #20]
	cmp	r3, #2
	strneb	r3, [r4, #9]
	b	.L239
.L354:
	ldr	r8, [r4, #452]
	cmp	r8, #0
	beq	.L110
	mov	r3, #2
	ldr	r10, [r4, #12]
	str	r3, [fp, #-248]
	b	.L111
.L275:
	mov	r2, r8
.L212:
	cmp	r2, #1
	ldr	r10, [r4, #28]
	beq	.L345
.L215:
	cmp	r9, #1
	beq	.L217
	ldr	r9, [r4, #16]
	mov	r2, #0
	mov	r3, #1
	strb	r2, [r4, #108]
	strb	r3, [r4, #109]
.L227:
	tst	r10, #1
	mvn	ip, #0
	clzne	r9, r9
	mov	r3, #0
	str	r3, [r6, #4]
	mov	r2, #0
	str	r3, [r6, #8]
	mov	r3, ip
	movne	r9, r9, lsr #5
	str	r2, [r6, #12]
	str	r9, [r6]
.L230:
	ldr	r2, [fp, #-248]
	str	r2, [r6, #20]
	ldr	r2, [fp, #-252]
	str	r2, [r6, #16]
	b	.L241
.L361:
	ldr	r8, [r4, #452]
	ldr	r10, [r4, #28]
	cmp	r8, #0
	ldr	r3, [r5, #208]
	ldr	r0, [r5, #212]
	ldr	r1, [r5, #216]
	bne	.L173
	ldr	r2, [r4, #464]
	cmp	r2, #1
	bgt	.L363
	ldr	r2, [r4, #104]
	cmp	r2, #0
	beq	.L180
	add	r3, r3, r0, lsl #1
	add	r3, r3, r1, lsl #2
	mov	r3, r3, asl #1
	str	r3, [r2]
	ldr	r3, [r4, #104]
	ldr	r2, [r4, #32]
	add	r3, r3, #4
	cmp	r3, r7
	addhi	r3, r4, #36
	str	r2, [fp, #-268]
	cmp	r3, #0
	str	r3, [r4, #104]
	add	r3, r2, #1
	str	r3, [r4, #32]
	beq	.L180
	cmp	r3, #4
	bgt	.L364
.L180:
	sub	r0, fp, #144
	add	r2, r4, #360
	mov	r3, #0
	mvn	r1, #0
	mov	lr, r0
	strb	r3, [r4, #368]
	str	r3, [r4, #360]
	str	r0, [fp, #-268]
	strb	r3, [r4, #369]
	strb	r1, [r4, #113]
	ldmia	r2, {r0, r1, r2}
	ldr	ip, [r4, #408]
	cmp	ip, #1
	stmia	lr, {r0, r1, r2}
	beq	.L177
.L178:
	add	lr, r4, #384
	add	ip, r4, #396
	add	r7, r4, #372
	add	r8, r4, #408
	ldmia	lr, {r0, r1, r2}
	str	r3, [fp, #-144]
	add	r3, r4, #420
	stmia	r7, {r0, r1, r2}
	ldmia	ip, {r0, r1, r2}
	stmia	lr, {r0, r1, r2}
	ldmia	r8, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	ldmia	r3, {r0, r1, r2}
	stmia	r8, {r0, r1, r2}
	ldr	r2, [fp, #-268]
	ldmia	r2, {r0, r1, r2}
	stmia	r3, {r0, r1, r2}
.L173:
	ldr	r3, [r4, #460]
	cmp	r3, #0
	ldr	r3, [r4, #456]
	beq	.L207
	cmp	r3, #0
	subne	r1, r10, #2
	bne	.L344
.L208:
	sub	r1, r10, #1
.L344:
	mov	r3, r1, asl #4
	add	r2, r4, #12
	sub	r3, r3, r1, asl #2
	add	r3, r3, #360
	add	r1, r2, r3
	ldr	r2, [r2, r3]
	ldrb	r8, [r1, #10]
.L209:
	ldrsb	ip, [r4, #108]
	ldrsb	r3, [r4, #109]
	cmp	ip, #1
	beq	.L212
	cmp	ip, #2
	beq	.L213
	cmp	r2, #1
	bne	.L222
	ldr	lr, .L373+8
	mov	r3, #0
	strb	r2, [r4, #108]
	sub	ip, fp, #144
	strb	r3, [r4, #109]
	ldmia	lr!, {r0, r1, r2, r3}
	ldr	r7, [r5, #8]
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	str	r7, [fp, #-256]
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1}
	stmia	ip, {r0, r1}
.L225:
	ldr	r3, [fp, #-256]
	mov	r2, #1
	cmp	r3, #0
	mov	r3, #0
	stmib	r6, {r2, r3}
	bne	.L231
	sxtb	r3, r8
	movw	r0, #26215
	add	r3, r3, r2
.L346:
	movt	r0, 26214
	mov	r2, r3, asr #31
	smull	r0, r1, r3, r0
	rsb	r2, r2, r1, asr #2
	mov	r1, r2, asl #3
	add	r2, r1, r2, lsl #1
	rsb	r3, r2, r3
	sub	r2, fp, #44
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #-100]
	str	r3, [r6]
.L232:
	mov	ip, #0
	str	ip, [r6, #12]
	mov	r3, ip
	b	.L230
.L231:
	ldr	r3, [fp, #-256]
	cmp	r3, #1
	bne	.L232
	sxtb	r3, r8
	movw	r0, #26215
	add	r3, r3, #2
	b	.L346
.L358:
	ldr	ip, [r4, #248]
	mov	r1, r10
	ldr	lr, [r4, #252]
	mov	r0, r8
	ldr	r3, [r5, #8]
	add	r2, r4, #256
	str	ip, [r4, #244]
	str	lr, [r4, #248]
	bl	Calc22PLD_4FLD
	ldr	r9, [r4, #348]
	str	r9, [r4, #252]
	b	.L162
.L353:
	add	r1, r4, #480
	mov	r0, r6
	mov	r2, #28
	bl	memcpy
	b	.L334
.L207:
	cmp	r3, #0
	bne	.L208
	mov	r3, r10, asl #4
	add	r2, r4, #12
	sub	r3, r3, r10, asl #2
	add	r3, r3, #360
	add	r1, r2, r3
	ldr	r2, [r2, r3]
	ldrb	r8, [r1, #10]
	b	.L209
.L363:
	mov	r1, r8
	mov	r2, #60
	add	r0, r4, #372
	bl	memset
	mov	r1, r8
	mov	r2, #20
	add	r0, r4, #432
	ldrb	r8, [r4, #382]
	bl	memset
	b	.L175
.L110:
	ldr	r2, [r5, #8]
	cmp	r2, #2
	streq	r2, [r4, #120]
	beq	.L113
	cmp	r2, #1
	beq	.L365
.L114:
	ldr	r10, [r4, #12]
	cmp	r10, #0
	beq	.L341
	ldr	r1, [r4, #20]
	rsb	r3, r3, #16
	ldr	r9, [r5, #220]
	cmp	r1, #1
	ldr	r2, [r5, #224]
	ldr	ip, [r5, #40]
	ldrne	r1, [r5, #228]
	moveq	r1, r9
	moveq	r9, r2
	moveq	r2, r1
	ldreq	r0, [r5, #228]
	strne	r1, [fp, #-248]
	ldr	r1, [r5, #44]
	streq	r0, [fp, #-248]
	rsb	r0, r2, r9
	str	r2, [fp, #-252]
	mul	r1, r1, ip
	ldr	r2, .L373
	ldr	ip, .L373
	ldr	r2, [r2]
	mov	r1, r1, asr #9
	str	r2, [ip, #4]
	cmp	r1, #0
	str	r2, [ip, #8]
	str	r2, [ip, #12]
	add	ip, r1, #31
	str	r2, [fp, #-264]
	movlt	r1, ip
	mov	r1, r1, asr #5
	mul	r3, r3, r1
	add	r1, r3, #15
	cmp	r3, #0
	movlt	r3, r1
	mov	r1, r3, asr #4
	add	r1, r1, r1, lsl #1
	add	r1, r1, r1, lsr #31
	mov	r1, r1, asr #1
	cmp	r1, #6
	movlt	r1, #6
	bl	__aeabi_idiv
	ldr	r2, [fp, #-252]
	cmp	r9, r2
	movlt	r3, r9
	movge	r3, r2
	str	r3, [fp, #-256]
	movge	r3, r9
	movlt	r3, r2
	str	r3, [fp, #-252]
	subs	r3, r0, #0
	str	r3, [fp, #-260]
	ble	.L122
	cmp	r2, #254
	movgt	r3, r2, asr #31
	movwgt	r0, #26215
	movle	r3, #50
	movtgt	r0, 26214
	smullgt	r0, r1, r2, r0
	rsbgt	r3, r3, r1, asr #1
	add	r3, r3, r2
	cmp	r3, r9
	bge	.L124
	ldr	r3, .L373
	mov	ip, #1
	str	ip, [r3]
.L125:
	ldr	r3, [fp, #-252]
	cmp	r3, #100
	ble	.L340
	str	r2, [fp, #-268]
	mov	r0, #100
	ldr	r2, [fp, #-256]
	ldr	r1, [fp, #-252]
	str	ip, [fp, #-272]
	rsb	r3, r2, r1
	mul	r0, r0, r3
	bl	__aeabi_idiv
	ldr	r2, [fp, #-268]
	ldr	ip, [fp, #-272]
	cmp	r0, #19
	movle	r3, #0
	ble	.L128
.L340:
	ldr	r1, [fp, #-260]
	movw	r3, #65036
	movt	r3, 65535
	cmp	r1, r3
	movge	r3, r1
	cmp	r3, #500
	movge	r3, #500
.L128:
	ldr	r1, .L373
	ldr	r0, [r1, #8]
	ldr	r1, [fp, #-264]
	cmp	r0, ip
	ldr	r0, .L373
	rsb	r1, r1, ip
	clz	r1, r1
	ldr	r0, [r0, #12]
	mov	r1, r1, lsr #5
	addeq	r1, r1, #1
	cmp	r0, ip
	addeq	r1, r1, #1
	ldr	r0, [fp, #-248]
	cmp	r1, #0
	mov	r1, #0
	str	r1, [fp, #-144]
	moveq	r3, r1
	cmp	r2, r0
	str	r1, [fp, #-140]
	movlt	ip, r0
	movge	ip, r2
	cmp	ip, #254
	str	r1, [fp, #-136]
	str	r1, [fp, #-132]
	movgt	lr, ip, asr #31
	movwgt	r0, #26215
	str	r1, [fp, #-128]
	movtgt	r0, 26214
	str	r1, [fp, #-124]
	smullgt	r0, r1, ip, r0
	movle	lr, #50
	rsbgt	lr, lr, r1, asr #1
	ldr	r1, [fp, #-248]
	add	lr, ip, lr
	cmp	lr, r9
	movlt	ip, #1
	movge	ip, #0
	strlt	ip, [fp, #-144]
	cmp	r9, r1
	movge	lr, r9
	movlt	lr, r1
	cmp	lr, #254
	movwgt	r0, #26215
	movle	r1, #50
	movtgt	r0, 26214
	smullgt	r0, r1, lr, r0
	movgt	r0, lr, asr #31
	rsbgt	r1, r0, r1, asr #1
	add	r1, lr, r1
	cmp	r1, r2
	movlt	r1, #1
	strlt	r1, [fp, #-140]
	ldr	r1, [fp, #-252]
	cmp	r1, #254
	movgt	lr, r1, asr #31
	movwgt	r0, #26215
	movle	lr, #50
	movtgt	r0, 26214
	smullgt	r0, r1, r1, r0
	rsbgt	lr, lr, r1, asr #1
	ldr	r1, [fp, #-252]
	add	lr, r1, lr
	ldr	r1, [fp, #-248]
	cmp	lr, r1
	movlt	r1, #1
	strlt	r1, [fp, #-136]
	ldr	r1, [fp, #-248]
	cmp	r2, r1
	movge	lr, r1
	movlt	lr, r2
	cmp	lr, #254
	movwgt	r0, #26215
	movle	r1, #50
	movtgt	r0, 26214
	smullgt	r0, r1, lr, r0
	movgt	r0, lr, asr #31
	rsbgt	r1, r0, r1, asr #1
	rsb	r1, r1, lr
	cmp	r1, r9
	movgt	r1, #1
	strgt	r1, [fp, #-132]
	ldr	r1, [fp, #-248]
	cmp	r9, r1
	movge	r9, r1
	cmp	r9, #254
	movgt	lr, r9, asr #31
	movwgt	r0, #26215
	movle	lr, #50
	movtgt	r0, 26214
	smullgt	r0, r1, r9, r0
	rsbgt	lr, lr, r1, asr #1
	rsb	r9, lr, r9
	cmp	r9, r2
	ldr	r2, [fp, #-256]
	movgt	r1, #1
	strgt	r1, [fp, #-128]
	cmp	r2, #254
	movgt	lr, r2, asr #31
	movwgt	r0, #26215
	movle	r1, #50
	movtgt	r0, 26214
	smullgt	r0, r1, r2, r0
	ldr	r2, [fp, #-256]
	movgt	r0, lr
	rsbgt	r1, r0, r1, asr #1
	rsb	r1, r1, r2
	ldr	r2, [fp, #-248]
	cmp	r1, r2
	movgt	r8, #1
	strgt	r8, [fp, #-124]
	cmp	ip, #0
	beq	.L144
	ldr	r1, [fp, #-128]
	cmp	r1, #0
	bne	.L144
	eor	r1, r3, r3, asr #31
	sub	r1, r1, r3, asr #31
	cmp	r1, #20
	ble	.L145
	cmp	r3, #50
	movlt	r3, #50
.L145:
	cmp	r8, #0
	bne	.L146
	ldr	r1, [fp, #-136]
	cmp	r1, #0
	movne	r3, r8
	bne	.L146
	ldr	r1, [fp, #-128]
	cmp	ip, #0
	cmpne	r1, #0
	bne	.L146
	ldr	r0, [fp, #-132]
	cmp	r0, #0
	bne	.L366
.L147:
	orrs	r2, r0, r1
	movne	r3, #0
.L146:
	sub	r1, fp, #144
	sub	lr, fp, #124
	mov	r0, #0
	b	.L149
.L367:
	ldr	ip, [r1, #4]!
.L149:
	cmp	r1, lr
	add	r0, r0, ip
	bne	.L367
	cmp	r0, #0
	bne	.L150
	cmp	r3, #50
	mvn	r1, #49
	movge	r3, #50
	cmp	r3, r1
	movlt	r3, r1
.L150:
	add	r0, r3, #49
	ldr	r2, [fp, #-248]
	cmp	r0, #98
	ldr	r1, [r4, #116]
	movls	r3, #0
	cmp	r2, #1
	movle	r3, #0
	add	r3, r3, r1
	cmp	r3, #1000
	str	r3, [r4, #116]
	movgt	r3, #1000
	strgt	r3, [r4, #116]
	bgt	.L154
	cmn	r3, #1000
	movw	r1, #64536
	movt	r1, 65535
	strlt	r1, [r4, #116]
	bge	.L368
.L156:
	mov	r3, #1
	str	r3, [r4, #120]
	str	r3, [fp, #-248]
	b	.L111
.L270:
	mov	r3, #1
	b	.L235
.L144:
	ldr	r1, [fp, #-140]
	cmp	r1, #0
	beq	.L145
	ldr	r1, [fp, #-132]
	cmp	r1, #0
	bne	.L145
	eor	r1, r3, r3, asr #31
	sub	r1, r1, r3, asr #31
	cmp	r1, #20
	ble	.L145
	mvn	r1, #49
	cmp	r3, r1
	movge	r3, r1
	b	.L145
.L124:
	ldr	r3, .L373
	mov	ip, #0
	str	ip, [r3]
	b	.L125
.L368:
	cmp	r3, #200
	bgt	.L154
	cmn	r3, #200
	blt	.L156
.L341:
	ldr	r3, [r4, #120]
	str	r3, [fp, #-248]
	b	.L111
.L154:
	mov	r3, #0
	str	r3, [r4, #120]
	str	r3, [fp, #-248]
	b	.L111
.L364:
	ldr	r3, [r4, #100]
	sub	lr, fp, #44
	mov	r1, #0
	str	r1, [fp, #-260]
	mov	r2, r3
	str	r3, [fp, #-272]
	ldr	r3, [r3]
	str	r3, [lr, #-196]!
	mov	r0, r3
	mov	ip, lr
	str	r3, [fp, #-264]
	str	lr, [fp, #-280]
	str	lr, [fp, #-276]
.L181:
	cmp	r0, r3
	add	r2, r2, #4
	add	r1, r1, #1
	movlt	r0, r3
	cmp	r7, r2
	addcc	r2, r4, #36
	cmp	r1, #5
	beq	.L369
	ldr	r3, [r2]
	ldr	lr, [fp, #-264]
	str	r3, [ip, #4]!
	cmp	r3, lr
	strlt	r3, [fp, #-264]
	strlt	r1, [fp, #-260]
	b	.L181
.L369:
	mov	r2, r0
	ldr	lr, [fp, #-276]
	ldr	r0, [fp, #-280]
	mov	r3, #0
	ldr	ip, [fp, #-260]
	str	r2, [fp, #-288]
.L186:
	ldr	r1, [r0], #4
	cmp	r3, ip
	cmpne	r1, r2
	add	r3, r3, #1
	movlt	r2, r1
	cmp	r3, #5
	bne	.L186
	ldr	r3, [fp, #-272]
	add	r1, r4, #432
	str	r2, [fp, #-276]
	add	r3, r3, #4
	ldr	r2, [fp, #-288]
	cmp	r7, r3
	ldr	r7, [fp, #-264]
	mov	ip, r3
	addcc	r3, r4, #36
	str	r2, [fp, #-212]
	movcc	ip, r3
	ldr	r3, [fp, #-276]
	str	r7, [fp, #-220]
	mov	r7, r7, asl #1
	str	r1, [fp, #-284]
	str	r3, [fp, #-216]
	ldmia	lr!, {r0, r1, r2, r3}
	str	r7, [fp, #-280]
	str	ip, [r4, #100]
	mvn	ip, #0
	str	ip, [fp, #-260]
	str	lr, [fp, #-292]
	sub	lr, fp, #208
	str	lr, [fp, #-272]
	mov	lr, #0
	ldr	r7, [fp, #-272]
	ldr	ip, [fp, #-268]
	stmia	r7!, {r0, r1, r2, r3}
	sub	r0, fp, #144
	ldr	r3, [fp, #-292]
	str	r0, [fp, #-268]
	str	ip, [r4, #32]
	sub	ip, fp, #128
	ldmia	r3, {r0, r1, r2, r3}
	str	ip, [fp, #-296]
	ldr	ip, [fp, #-284]
	stmia	r7, {r0, r1, r2, r3}
	mvn	r1, #0
	str	r6, [fp, #-292]
	ldr	r6, [fp, #-276]
	str	r8, [fp, #-300]
	ldr	r7, [fp, #-296]
	ldr	r8, [fp, #-264]
	str	r10, [fp, #-296]
	mov	r10, ip
	str	r9, [fp, #-304]
	str	r4, [fp, #-264]
	mov	r4, lr
	ldr	r9, [fp, #-288]
	str	r5, [fp, #-288]
	mov	r5, lr
	str	r1, [fp, #-272]
.L197:
	sub	lr, fp, #208
	str	r8, [fp, #-188]
	str	r6, [fp, #-184]
	sub	ip, fp, #176
	str	r9, [fp, #-180]
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	mov	r2, #100
	ldr	r1, .L373+12
	ldr	r0, [fp, #-268]
	bl	memcpy
	ldr	r3, [fp, #-280]
	cmp	r6, r3
	blt	.L192
	sub	r2, r7, #20
	sub	r3, fp, #176
	mov	r1, #0
	b	.L191
.L371:
	cmp	r0, r8
	addeq	r1, r1, #1
.L190:
	cmp	r7, r2
	add	r3, r3, #4
	beq	.L370
.L191:
	ldr	r0, [r2, #4]!
	cmp	r0, #0
	ldr	r0, [r3]
	beq	.L371
	mov	r0, r0, asl #5
	cmp	r0, r9
	addgt	r1, r1, #1
	b	.L190
.L370:
	cmp	r1, #4
	movle	r1, #0
	movgt	r1, #1
	cmp	r1, #0
	beq	.L192
	cmp	r1, #1
	bne	.L265
	ldr	r3, [fp, #-264]
	ldrsb	r3, [r3, #113]
	add	r2, r3, #1
	cmp	r5, r2
	beq	.L193
	add	r2, r5, #4
	cmp	r2, r3
	beq	.L193
.L265:
	str	r5, [fp, #-272]
.L192:
	add	r4, r4, #1
.L196:
	add	r5, r5, #1
	add	r10, r10, #4
	cmp	r5, #5
	add	r7, r7, #20
	bne	.L197
	ldr	r5, [fp, #-288]
	mov	r1, r4
	ldr	r4, [fp, #-264]
	ldr	r8, [fp, #-300]
	ldr	r2, [r5, #124]
	ldr	r3, [r5, #128]
	ldr	r10, [fp, #-296]
	cmp	r3, r2
	ldr	r9, [fp, #-304]
	ldr	r6, [fp, #-292]
	movge	r3, r2
	cmp	r3, #64
	ldr	r3, [fp, #-256]
	movle	r2, #0
	movgt	r2, #1
	cmp	r3, #0
	moveq	r3, r2
	orrne	r3, r2, #1
	str	r2, [r4, #364]
	cmp	r1, #4
	orrgt	r3, r3, #1
	cmp	r3, #0
	bne	.L372
	ldr	r3, [fp, #-260]
	cmp	r3, #4
	ldrhi	ip, [r4, #360]
	bhi	.L202
	ldr	r3, [fp, #-260]
	ldr	r1, [r4, #12]
	add	r2, r4, r3, lsl #2
	and	r3, r3, #1
	ldr	r2, [r2, #432]
	cmp	r2, #3
	strb	r2, [r4, #368]
	movle	ip, #0
	movgt	ip, #1
	cmp	r1, r3
	str	ip, [r4, #360]
	ldrne	r3, [fp, #-260]
	ldreqb	r3, [fp, #-260]
	addne	r3, r3, #5
	uxtbne	r3, r3
	strb	r3, [r4, #370]
.L202:
	ldr	r2, [fp, #-260]
	cmp	r2, #0
	mov	r3, r2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #369]
	blt	.L205
	uxtb	r2, r2
	mov	r3, ip
.L206:
	add	r1, r4, #360
	ldr	lr, [r4, #408]
	strb	r2, [r4, #113]
	ldmia	r1, {r0, r1, r2}
	cmp	lr, #1
	ldr	lr, [fp, #-268]
	stmia	lr, {r0, r1, r2}
	bne	.L178
	cmp	ip, #0
	bne	.L178
.L177:
	mov	r1, #2
	mov	r2, #0
	strb	r1, [r4, #428]
	strb	r1, [r4, #416]
	strb	r1, [r4, #404]
	strb	r1, [r4, #392]
	str	r2, [r4, #420]
	str	r2, [r4, #408]
	str	r2, [r4, #396]
	str	r2, [r4, #384]
	b	.L178
.L365:
	ldr	r2, [r4, #20]
	cmp	r2, #0
	bne	.L115
	ldr	r2, [r4, #24]
	cmp	r2, #0
	bne	.L114
.L116:
	ldr	r2, [r4, #120]
.L113:
	str	r2, [fp, #-248]
	ldr	r10, [r4, #12]
	b	.L111
.L372:
	cmp	r2, #0
	movne	r3, #0
	strneb	r3, [r4, #368]
	bne	.L200
	ldrb	r3, [r4, #368]
	sub	r3, r3, #1
	sxtb	r3, r3
	cmp	r3, #4
	movlt	r8, r3
	movge	r8, #4
	bic	r8, r8, r8, asr #31
	uxtb	r8, r8
	strb	r8, [r4, #368]
	sxtb	r8, r8
.L200:
	mov	r3, #0
	str	r8, [r4, #432]
	mov	ip, r3
	str	r8, [r4, #436]
	str	r8, [r4, #440]
	str	r8, [r4, #444]
	str	r8, [r4, #448]
	str	r3, [r4, #360]
	strb	r3, [r4, #370]
	strb	r3, [r4, #369]
.L205:
	ldrb	r2, [fp, #-272]
	mov	r3, ip
	b	.L206
.L193:
	ldr	r3, [r10]
	add	r3, r3, #1
	cmp	r3, #5
	movge	r3, #5
	bic	r3, r3, r3, asr #31
	str	r3, [r10]
	cmp	r3, #3
	ble	.L343
	ldr	r2, [fp, #-284]
	mov	r3, #0
	mov	r1, #3
.L195:
	cmp	r5, r3
	add	r3, r3, #1
	strne	r1, [r2]
	cmp	r3, #5
	add	r2, r2, #4
	bne	.L195
.L343:
	str	r5, [fp, #-272]
	str	r5, [fp, #-260]
	b	.L196
.L122:
	bge	.L124
	cmp	r9, #254
	movgt	r3, r9, asr #31
	movwgt	r0, #26215
	movle	r3, #50
	movtgt	r0, 26214
	smullgt	r0, r1, r9, r0
	rsbgt	r3, r3, r1, asr #1
	add	r3, r3, r9
	cmp	r3, r2
	bge	.L124
	ldr	r3, .L373
	mov	ip, #2
	str	ip, [r3]
	b	.L125
.L115:
	cmp	r2, #1
	bne	.L114
	ldr	r2, [r4, #24]
	cmp	r2, #1
	bne	.L114
	b	.L116
.L366:
	ldr	lr, [fp, #-140]
	cmp	lr, #0
	bne	.L146
	b	.L147
.L359:
	ldr	r10, [r4, #28]
	b	.L173
.L374:
	.align	2
.L373:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LANCHOR2+100
	.word	.LANCHOR2
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_FmdSet, .-PQ_MNG_ALG_FmdSet
	.global	g_u32FrmRateThr_H
	.global	g_u32FrmRateThr_L
	.global	g_u32IPDetect
	.section	.rodata
	.align	2
.LANCHOR2 = . + 0
.LC0:
	.word	1
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	1
.LC1:
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.type	g_u32PDTypeNum, %object
	.size	g_u32PDTypeNum, 8
g_u32PDTypeNum:
	.word	32
	.word	22
	.data
	.align	2
.LANCHOR1 = . + 0
	.type	g_u32FrmRateThr_L, %object
	.size	g_u32FrmRateThr_L, 4
g_u32FrmRateThr_L:
	.word	23800
	.type	g_u32FrmRateThr_H, %object
	.size	g_u32FrmRateThr_H, 4
g_u32FrmRateThr_H:
	.word	24200
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	stastlasi1432big.4488, %object
	.size	stastlasi1432big.4488, 16
stastlasi1432big.4488:
	.space	16
	.type	g_u32IPDetect, %object
	.size	g_u32IPDetect, 4
g_u32IPDetect:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
