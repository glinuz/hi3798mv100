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
	.global	__aeabi_idiv
	.text
	.align	2
	.type	Detect22Pld, %function
Detect22Pld:
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
	ldr	lr, [fp, #4]
	ldmib	r3, {r5, ip}
	cmp	lr, #1
	stmia	r3, {r5, ip}
	beq	.L149
	ldr	r5, [r1, #128]
	clz	r4, r0
	ldr	ip, [r1, #124]
	mov	r4, r4, lsr #5
	ldr	r0, [r1, #120]
	str	r5, [fp, #-56]
	cmp	r4, #1
	ldr	r5, [r1, #132]
	ldr	r10, [r1]
	str	r5, [fp, #-60]
	ldr	r5, [r2, #92]
	str	r5, [fp, #-48]
	beq	.L150
	cmp	lr, #0
	bne	.L105
	ldr	lr, [r1, #24]
	ldr	r5, [r2, #48]
	ldr	r6, [r2, #52]
	add	lr, r5, lr
	str	lr, [r2, #68]
	ldr	r5, [r1, #28]
	ldr	r7, [r2, #56]
	add	r6, r6, r5
	str	r6, [r2, #72]
	ldr	r5, [r1, #32]
	add	r5, r7, r5
	str	r5, [r2, #76]
	ldr	r7, [r1, #24]
	ldr	r8, [r1, #40]
	str	r7, [r2, #48]
	ldr	r7, [r1, #28]
	str	r8, [fp, #-68]
	str	r7, [r2, #52]
	ldr	r7, [r1, #32]
	str	r7, [r2, #56]
.L39:
	ldmib	r1, {r8, r9}
	cmp	r5, r6
	ldr	r7, [r1, #12]
	add	ip, r0, ip, lsl #1
	ldr	r1, [r1, #16]
	movge	r0, r5
	movlt	r0, r6
	str	r0, [fp, #-52]
	cmp	r9, r8
	mov	r0, r1
	str	r1, [fp, #-80]
	ldr	r1, [fp, #-56]
	add	ip, ip, r1, lsl #2
	movge	r1, r9
	movlt	r1, r8
	str	r1, [fp, #-72]
	ldr	r1, [fp, #-60]
	cmp	r0, r7
	add	r1, ip, r1, lsl #3
	str	r1, [fp, #-64]
	mov	r1, r0
	ldr	r0, [fp, #-52]
	movlt	r1, r7
	str	r1, [fp, #-76]
	cmp	lr, #4
	cmple	r0, #4
	bgt	.L40
	ldr	r0, [fp, #-76]
	movw	ip, #499
	ldr	r1, [fp, #-72]
	cmp	r1, ip
	cmple	r0, ip
	ldr	r1, [fp, #-56]
	movle	ip, #1
	movgt	ip, #0
	cmp	r1, #1
	movgt	ip, #0
	andle	ip, ip, #1
	ldr	r1, [fp, #-60]
	cmp	r1, #1
	movgt	ip, #0
	andle	ip, ip, #1
	cmp	ip, #0
	movne	r0, #1
	strne	r0, [r2, #92]
	bne	.L37
.L40:
	cmp	lr, #800
	ble	.L41
	add	r0, r2, r4
	ldrb	r1, [r0, #88]
	sub	r1, r1, #15
	strb	r1, [r0, #88]
.L42:
	ldr	r1, [fp, #-68]
	mov	r1, r1, asl #5
	cmp	r1, #127
	addgt	r4, r2, r4
	ldrgtb	r1, [r4, #88]
	subgt	r1, r1, #5
	strgtb	r1, [r4, #88]
.L99:
	ldr	ip, [r2, #36]
	mov	r9, #1
	add	ip, ip, r9
.L101:
	ldrsb	r0, [r2, #88]
	ldrsb	r1, [r2, #89]
	cmp	r0, #32
	str	ip, [r2, #36]
	movge	r0, #32
	cmp	r1, #32
	bic	r0, r0, r0, asr #31
	movge	r1, #32
	bic	r1, r1, r1, asr #31
	uxtb	r0, r0
	strb	r0, [r2, #88]
	uxtb	r1, r1
	sxtb	r0, r0
	strb	r1, [r2, #89]
	sxtb	r1, r1
	cmp	r0, #24
	cmpgt	r1, #24
	movgt	r1, #1
	movgt	ip, #1
	movle	ip, #0
	strgt	r1, [r2, #92]
	bgt	.L88
	cmp	r1, #19
	cmpgt	r0, #19
	strle	ip, [r2, #92]
.L88:
	ldr	ip, [r2, #12]
	movw	r4, #26215
	ldr	r7, [r2, #8]
	movt	r4, 26214
	ldr	r6, [r2, #4]
	movw	r0, #62536
	ldr	lr, [r2]
	add	r1, ip, r7
	add	r1, r6, r1
	ldr	r8, [r2, #20]
	add	r1, lr, r1
	movt	r0, 65535
	add	r1, r10, r1
	stmia	r2, {r10, lr}
	ldr	lr, [r2, #28]
	smull	r4, r5, r1, r4
	mov	r1, r1, asr #31
	str	ip, [r2, #16]
	movw	ip, #3000
	str	r7, [r2, #12]
	str	r6, [r2, #8]
	rsb	r1, r1, r5, asr #1
	str	r1, [r2, #24]
	add	r10, r1, lr
	str	r1, [r2, #20]
	rsb	r8, r8, r10
	cmp	r8, r0
	movlt	r8, r0
	cmp	r8, ip
	movge	r8, ip
	cmp	r8, #1000
	str	r8, [r2, #28]
	blt	.L89
	ldr	r1, [fp, #-48]
	cmp	r1, #1
	movne	r9, #0
	andeq	r9, r9, #1
	cmp	r9, #0
	bne	.L151
.L89:
	ldr	r1, [fp, #-48]
	ldr	r0, [r2, #92]
	cmp	r0, #1
	cmpeq	r1, #0
	moveq	r1, #0
	streq	r1, [r2, #28]
.L90:
	ldr	r1, [fp, #-64]
	str	r1, [r2, #32]
	str	r0, [r3, #8]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L150:
	cmp	lr, #0
	beq	.L152
.L105:
	ldr	r0, [fp, #-48]
.L37:
	str	r0, [r3, #8]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L41:
	ldr	r1, [fp, #-52]
	cmp	lr, #120
	cmple	r1, #120
	ble	.L43
	cmp	r5, r6
	movlt	r1, r5
	movge	r1, r6
	cmp	lr, #10
	mov	ip, r1, asl #1
	movlt	lr, #10
	mov	r7, lr, asl #2
	cmp	r7, ip
	bge	.L44
	cmp	r1, #120
	ldr	r0, [fp, #-52]
	ble	.L45
	cmp	r0, #800
	ble	.L46
	add	r5, lr, lr, lsl #1
	cmp	ip, r5
	ble	.L46
	cmp	r0, ip
	bgt	.L131
	ldr	r0, [fp, #-52]
	mov	lr, r1, asl #2
	add	r6, lr, r1
	cmp	r6, r0, asl #2
	ble	.L48
	cmp	r1, r5
	ble	.L42
.L132:
	add	r0, r2, r4
	ldrb	r1, [r0, #88]
	add	r1, r1, #1
	strb	r1, [r0, #88]
.L49:
	ldr	r1, [fp, #-68]
	mov	r1, r1, asl #5
	cmp	r1, #127
	movle	r9, #0
	movle	ip, r9
	ble	.L101
	add	r4, r2, r4
	mov	r9, #0
	mov	ip, r9
	ldrb	r1, [r4, #88]
	sub	r1, r1, #5
	strb	r1, [r4, #88]
	b	.L101
.L43:
	ldr	r1, [fp, #-52]
	cmp	r1, #120
	cmple	lr, #120
	bgt	.L42
	movw	r0, #5499
	cmp	r8, r0
	cmple	r7, r0
	bgt	.L131
	cmp	lr, #39
	ble	.L60
	cmp	lr, r6, asl #1
	bge	.L131
.L60:
	ldr	r1, [fp, #-72]
	add	r0, r8, r8, lsl #2
	mov	r5, r9, asl #2
	movw	ip, #2500
	cmp	r0, r5
	cmpgt	r1, ip
	movgt	lr, #1
	movle	lr, #0
	bgt	.L131
	ldr	r1, [fp, #-80]
	add	r0, r7, r7, lsl #2
	mov	r6, r1, asl #2
	ldr	r1, [fp, #-76]
	cmp	r0, r6
	cmpgt	r1, ip
	addgt	r1, r2, r4
	strgtb	lr, [r1, #88]
	bgt	.L42
	ldr	r1, [fp, #-72]
	movw	ip, #10000
	cmp	r1, ip
	ble	.L63
	mov	r0, r9, asl #1
	cmp	r0, r8, asl #4
	bgt	.L146
	mov	r1, r7, asl #1
	cmp	r9, r1
	bgt	.L49
	add	r1, r1, r7
	cmp	r0, r1
	add	r0, r2, r4
	ldrb	r1, [r0, #88]
	bgt	.L130
.L80:
	sub	r1, r1, #5
	strb	r1, [r0, #88]
	b	.L42
.L151:
	mov	r1, #0
	strb	r1, [r2, #88]
	strb	r1, [r2, #89]
	mov	r0, r1
	str	r1, [r2, #92]
	b	.L90
.L152:
	ldr	r5, [r1, #20]
	ldr	lr, [r2, #68]
	ldr	r6, [r2, #72]
	str	r5, [r2, #48]
	ldr	r7, [r1, #24]
	ldr	r5, [r2, #76]
	str	r7, [r2, #52]
	ldr	r7, [r1, #32]
	str	r7, [r2, #56]
	ldr	r7, [r1, #44]
	str	r7, [fp, #-68]
	b	.L39
.L44:
	add	r1, ip, r1
	cmp	r7, r1
	ble	.L153
.L131:
	add	r1, r2, r4
	mov	r0, #0
	strb	r0, [r1, #88]
	b	.L42
.L149:
	ldr	ip, [r1, #28]
	clz	r0, r0
	ldr	r8, [r1, #132]
	mov	r0, r0, lsr #5
	ldr	r10, [r1, #44]
	cmp	r0, #1
	str	ip, [fp, #-56]
	ldmib	r1, {r9, ip}
	ldr	r7, [r1, #124]
	ldr	r4, [r1, #32]
	str	r8, [fp, #-48]
	ldrne	r8, [fp, #-56]
	ldr	lr, [r1, #40]
	moveq	lr, r10
	ldr	r6, [r1, #128]
	ldreq	r10, [r2, #44]
	movne	r10, r8
	ldreq	r4, [r2, #40]
	str	ip, [fp, #-52]
	ldr	ip, [r1, #120]
	ldr	r5, [r1, #24]
	add	ip, ip, r7, lsl #1
	ldr	r1, [r1]
	ldreq	r5, [fp, #-56]
	add	r6, ip, r6, lsl #2
	strne	r4, [r2, #40]
	strne	r8, [r2, #44]
	cmp	r4, r10
	ldr	ip, [fp, #-48]
	movlt	r7, r4
	movge	r7, r10
	cmp	r5, #80
	cmple	r10, #80
	mov	r7, r7, asl #1
	add	r6, r6, ip, lsl #3
	ble	.L5
	add	ip, r5, r5, lsl #1
	mov	r9, r10, asl #1
	cmp	ip, r9
	ble	.L154
.L21:
	add	ip, r2, r0
	mov	r4, #0
	strb	r4, [ip, #88]
.L7:
	mov	lr, lr, asl #5
	cmp	lr, #127
	addgt	r0, r2, r0
	ldrgtb	ip, [r0, #88]
	subgt	ip, ip, #5
	strgtb	ip, [r0, #88]
.L94:
	ldr	lr, [r2, #36]
	add	lr, lr, #1
	cmp	lr, #50
	movge	lr, #50
	bic	lr, lr, lr, asr #31
.L30:
	ldrsb	ip, [r2, #88]
	ldrsb	r0, [r2, #89]
	cmp	ip, #32
	str	lr, [r2, #36]
	movge	ip, #32
	cmp	r0, #32
	bic	ip, ip, ip, asr #31
	movge	r0, #32
	bic	r0, r0, r0, asr #31
	uxtb	ip, ip
	strb	ip, [r2, #88]
	uxtb	r0, r0
	sxtb	ip, ip
	strb	r0, [r2, #89]
	sxtb	r0, r0
	cmp	ip, #24
	cmpgt	r0, #24
	movgt	r0, #1
	movgt	lr, #1
	movle	lr, #0
	strgt	r0, [r2, #92]
	bgt	.L32
	cmp	r0, #19
	cmpgt	ip, #19
	strle	lr, [r2, #92]
	ldrgt	r0, [r2, #92]
	movle	r0, lr
.L32:
	ldr	r5, [r2, #12]
	movw	r4, #26215
	ldr	r10, [r2, #8]
	movt	r4, 26214
	ldr	r9, [r2, #4]
	movw	r7, #62537
	add	ip, r5, r10
	ldr	r8, [r2]
	add	ip, r9, ip
	str	r5, [r2, #16]
	add	ip, r8, ip
	ldr	lr, [r2, #28]
	add	ip, r1, ip
	movt	r7, 65535
	stmia	r2, {r1, r8, r9, r10}
	smull	r4, r5, ip, r4
	mov	ip, ip, asr #31
	ldr	r1, [r2, #20]
	rsb	ip, ip, r5, asr #1
	str	ip, [r2, #24]
	add	lr, ip, lr
	rsb	lr, r1, lr
	str	lr, [r2, #28]
	cmp	lr, r7
	blt	.L34
	movw	r1, #3000
	cmp	lr, r1
	str	ip, [r2, #20]
	movge	lr, r1
	cmp	r0, #0
	str	lr, [r2, #28]
	beq	.L97
	cmp	lr, #1000
	movge	r1, #0
	strgeb	r1, [r2, #88]
	movge	r0, r1
	strgeb	r1, [r2, #89]
	strge	r1, [r2, #92]
.L36:
	str	r6, [r2, #32]
	str	r0, [r3, #8]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L34:
	cmp	r0, #0
	movw	r1, #62536
	movt	r1, 65535
	str	ip, [r2, #20]
	str	r1, [r2, #28]
	bne	.L36
.L97:
	mov	r1, #0
	str	r1, [r2, #28]
	b	.L36
.L5:
	movw	ip, #5499
	cmp	r9, ip
	bgt	.L21
	ldr	ip, [fp, #-52]
	cmp	ip, #0
	moveq	ip, #20
	cmp	r9, ip
	bgt	.L21
	cmp	r5, #39
	ble	.L23
	cmp	r5, r10, asl #1
	bge	.L21
.L23:
	cmp	r4, #80
	ble	.L24
	rsb	r4, r4, r10
	add	r7, r7, #50
	eor	ip, r4, r4, asr #31
	sub	ip, ip, r4, asr #31
	cmp	ip, r7
	ble	.L24
	ldr	ip, [r2, #32]
	cmp	ip, #10
	ble	.L24
	ldr	r4, [r2, #92]
	cmp	r4, #1
	beq	.L29
	movw	r4, #5000
	cmp	ip, r4
	ble	.L7
.L29:
	add	r4, r2, r0
	ldr	ip, [r2, #36]
	ldrb	r5, [r4, #88]
	mvn	ip, ip
	add	ip, ip, r5
	strb	ip, [r4, #88]
	b	.L7
.L153:
	add	r0, r2, r4
	ldrb	r1, [r0, #88]
	sub	r1, r1, #5
	strb	r1, [r0, #88]
	b	.L42
.L45:
	cmp	r0, #119
	ble	.L131
	add	ip, ip, r1
	cmp	r0, ip
	bgt	.L131
	rsb	ip, r6, r5
	eor	r0, ip, ip, asr #31
	sub	r0, r0, ip, asr #31
	cmp	r0, #40
	ble	.L57
	cmp	r1, #1
	ldr	r0, [fp, #-52]
	add	r5, r2, r4
	str	r3, [fp, #-56]
	movlt	r1, #1
	str	r2, [fp, #-52]
	bl	__aeabi_idiv
	ldrb	ip, [r5, #88]
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-56]
	rsb	r0, r0, ip
	strb	r0, [r5, #88]
	b	.L42
.L154:
	add	ip, r10, r10, lsl #2
	add	ip, ip, ip, lsr #31
	cmp	r4, ip, asr #1
	bgt	.L26
	cmp	r4, #1000
	ble	.L10
	add	ip, r4, r4, lsl #2
	rsb	r8, r10, r10, lsl #3
	cmp	ip, r8
	bge	.L26
.L10:
	cmp	r5, #99
	ble	.L11
	cmp	r5, #500
	add	ip, r2, r0
	movlt	r8, r5, asr #31
	movwlt	r4, #34079
	ldrb	r7, [ip, #88]
	movtlt	r4, 20971
	movge	r4, #5
	smulllt	r4, r5, r5, r4
	rsblt	r4, r8, r5, asr #5
	uxtblt	r4, r4
	rsb	r4, r4, r7
	strb	r4, [ip, #88]
	b	.L7
.L46:
	ldr	r0, [fp, #-52]
	cmp	r0, ip
	bgt	.L131
	ldr	r0, [fp, #-52]
	mov	r5, r0, asl #1
	add	r0, r5, r0
	cmp	r0, r1, asl #2
	bge	.L53
	add	lr, r7, lr
	cmp	ip, lr
	ble	.L42
	b	.L132
.L26:
	add	r4, r2, r0
	ldrb	ip, [r4, #88]
.L127:
	sub	ip, ip, #1
	strb	ip, [r4, #88]
	b	.L7
.L146:
	ldr	r1, [fp, #-76]
	cmp	r1, ip
	bgt	.L139
	ldr	r1, [fp, #-76]
	cmp	r1, #1000
	ble	.L132
	cmp	r6, r7, asl #4
	bgt	.L132
	ldr	r1, [fp, #-80]
	add	r7, r7, r7, lsl #1
	cmp	r7, r1, asl #1
	blt	.L42
.L85:
	ldr	r1, [r2, #92]
	cmp	r1, #1
	beq	.L155
.L81:
	add	r0, r2, r4
	ldrb	r1, [r0, #88]
.L130:
	sub	r1, r1, #1
	strb	r1, [r0, #88]
	b	.L42
.L63:
	ldr	r1, [fp, #-72]
	cmp	r1, #1000
	ble	.L72
	cmp	r5, r8, asl #4
	bgt	.L146
	add	r7, r7, r7, lsl #1
	add	r0, r2, r4
	cmp	r7, r9, asl #1
	ldrb	r1, [r0, #88]
	blt	.L130
	b	.L80
.L24:
	ldr	ip, [fp, #-52]
	cmp	ip, #1000
	ble	.L26
	mov	r8, r9, asl #4
	mov	ip, ip, asl #1
	cmp	ip, r8
	ble	.L27
	ldr	r4, [fp, #-52]
	movw	ip, #9000
	cmp	r4, ip
	ble	.L7
.L128:
	add	ip, r2, r0
	mov	lr, lr, asl #5
	cmp	lr, #127
	ldrb	r0, [ip, #88]
	add	r0, r0, #1
	strb	r0, [ip, #88]
	ldrgtb	r0, [ip, #88]
	subgt	r0, r0, #5
	strgtb	r0, [ip, #88]
.L96:
	mov	lr, #0
	b	.L30
.L53:
	add	r0, r2, r4
	add	r1, ip, r1
	cmp	r1, r5
	ldrb	r1, [r0, #88]
	suble	r1, r1, #2
	strleb	r1, [r0, #88]
	ble	.L42
	b	.L130
.L11:
	mov	ip, r5, asl #3
	str	ip, [fp, #-52]
	cmp	r10, ip
	blt	.L13
	cmp	r5, #10
	mov	ip, r4, asl #1
	movge	r8, r5
	movlt	r8, #10
	cmp	ip, r8, asl #3
	bge	.L14
.L13:
	rsb	ip, r4, r10
	cmp	ip, #0
	rsblt	ip, ip, #0
	cmp	r7, #150
	mov	r8, ip
	movlt	r7, #150
	add	ip, r7, #50
	cmp	r8, ip
	ble	.L14
	ldr	ip, [r2, #32]
	cmp	ip, #10
	ble	.L14
	add	r4, r2, r0
	ldrb	ip, [r4, #88]
	sub	ip, ip, #5
	strb	ip, [r4, #88]
	b	.L7
.L14:
	ldr	ip, [fp, #-52]
	cmp	ip, r9
	bge	.L15
	cmp	ip, r4, asl #1
	blt	.L16
	ldr	ip, [r2, #32]
	cmp	ip, #9
	bgt	.L15
.L16:
	ldr	ip, [fp, #-48]
	add	ip, ip, #100
	cmp	ip, r5, asl #2
	bge	.L128
	add	r4, r2, r0
	ldrb	ip, [r4, #88]
	sub	ip, ip, #15
	strb	ip, [r4, #88]
	b	.L7
.L57:
	cmp	r0, #19
	bgt	.L42
	b	.L132
.L72:
	bge	.L81
	ldr	r1, [fp, #-76]
	cmp	r1, ip
	bgt	.L139
	ldr	r1, [fp, #-76]
	cmp	r1, #1000
	ble	.L42
	cmp	r6, r7, asl #4
	ble	.L85
	b	.L132
.L139:
	ldr	r1, [fp, #-80]
	mov	r7, r7, asl #4
	cmp	r7, r1, asl #1
	bge	.L85
	b	.L132
.L15:
	add	r10, r10, #150
	cmp	r5, r10
	bgt	.L19
	ldr	ip, [fp, #-56]
	add	ip, ip, #150
	cmp	r5, ip
	ble	.L7
.L19:
	ldr	ip, [r2, #92]
	add	r4, r2, r0
	cmp	ip, #1
	ldrb	ip, [r4, #88]
	subne	ip, ip, #3
	strneb	ip, [r4, #88]
	bne	.L7
	b	.L127
.L48:
	ldr	r0, [fp, #-52]
	add	r5, r0, r0, lsl #1
	cmp	lr, r5
	bgt	.L81
	ldr	r0, [fp, #-52]
	add	r1, ip, r1, lsl #3
	rsb	r0, r0, r0, lsl #3
	cmp	r1, r0
	add	r0, r2, r4
	ldrb	r1, [r0, #88]
	suble	r1, r1, #15
	strleb	r1, [r0, #88]
	ble	.L42
	b	.L80
.L27:
	ldr	r4, [r2, #92]
	cmp	r4, #1
	bne	.L26
	cmp	ip, r8
	bge	.L29
	add	ip, r2, r0
	ldr	r5, [r2, #36]
	ldrb	r4, [ip, #88]
	mvn	r5, r5
	add	r4, r4, r5, lsl #1
	strb	r4, [ip, #88]
	b	.L7
.L155:
	add	r1, r2, r4
	ldr	ip, [r2, #36]
	ldrb	r0, [r1, #88]
	mvn	ip, ip
	add	r0, r0, ip, lsl #1
	strb	r0, [r1, #88]
	b	.L42
	UNWIND(.fnend)
	.size	Detect22Pld, .-Detect22Pld
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
	UNWIND(.pad #196)
	sub	sp, sp, #196
	ldr	r3, [r0]
	mov	r4, r0
	mov	r5, r1
	cmp	r3, #0
	mov	r6, r2
	ldr	r7, [r1, #8]
	beq	.L415
	mov	r2, #28
	mov	r1, #0
	mov	r0, r6
	bl	memset
	ldr	r3, [r5, #68]
	cmp	r3, #1
	beq	.L416
	ldrb	r3, [r4, #7]
	cmp	r3, #255
	beq	.L160
	ldrb	r3, [r4, #8]
	cmp	r3, #255
	beq	.L160
	ldrb	r3, [r4, #9]
	cmp	r3, #255
	beq	.L160
	ldrb	r3, [r4, #4]
	cmp	r3, #255
	beq	.L162
.L419:
	ldrb	r3, [r4, #5]
	cmp	r3, #255
	beq	.L162
.L163:
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
	orrs	r8, r3, r1
	beq	.L177
	cmp	r3, #1
	cmpeq	r1, #1
	moveq	r8, #1
	movne	r8, #0
	eor	r8, r8, #1
.L177:
	orrs	r3, r2, r0
	moveq	r2, r3
	beq	.L178
	cmp	r2, #1
	cmpeq	r0, #1
	moveq	r2, #1
	movne	r2, #0
	eor	r2, r2, #1
.L178:
	ldr	r1, [r4, #448]
	cmp	r7, #1
	ldr	r3, [r4, #452]
	str	r8, [r4, #12]
	str	r1, [r4, #444]
	str	r3, [r4, #448]
	str	r2, [r4, #16]
	ldr	r3, [r4, #464]
	ldr	r2, [r5, #52]
	ldr	r1, [r4, #460]
	str	r3, [r4, #460]
	str	r2, [r4, #452]
	str	r1, [r4, #456]
	ldr	r3, [r5, #56]
	str	r3, [r4, #464]
	bls	.L417
.L179:
	ldrb	r2, [r4, #8]
	ldrb	r3, [r4, #9]
	strb	r2, [r4, #7]
	strb	r3, [r4, #8]
	ldr	r3, [r5, #20]
	cmp	r3, #1
	beq	.L418
.L294:
	mov	r2, #28
	mov	r1, r6
	add	r0, r4, #472
	bl	memcpy
	ldrb	r3, [r4, #6]
	ldrb	r2, [r4, #5]
	strb	r3, [r4, #5]
	strb	r2, [r4, #4]
	ldr	r3, [r5, #64]
	strb	r3, [r4, #6]
.L392:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L160:
	ldr	r3, [r5, #60]
	strb	r3, [r4, #7]
	ldr	r3, [r5, #60]
	strb	r3, [r4, #8]
	ldr	r3, [r5, #60]
	strb	r3, [r4, #9]
	ldrb	r3, [r4, #4]
	cmp	r3, #255
	bne	.L419
.L162:
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
	b	.L163
.L415:
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
	str	r3, [r0], #472
	bl	memcpy
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L418:
	ldr	r3, [r6, #20]
	cmp	r3, #2
	strneb	r3, [r4, #9]
	b	.L294
.L417:
	ldr	r0, [r5, #44]
	add	r7, r4, #96
	ldr	r1, [r5, #40]
	mov	r2, r4
	mov	r3, r4
	mul	r0, r1, r0
	mov	r0, r0, asr #11
	str	r0, [r4, #468]
.L180:
	ldrh	ip, [r3, #150]
	add	r3, r3, #6
	ldrh	r1, [r3, #142]
	strh	ip, [r3, #146]
	strh	r1, [r3, #144]
	cmp	r3, r7
	bne	.L180
	add	r3, r5, #232
	mov	r1, #0
.L182:
	ldrh	ip, [r2, #152]
	cmp	r0, ip
	ble	.L181
	ldrh	ip, [r2, #148]
	cmp	r0, ip
	ble	.L181
	ldrh	ip, [r2, #150]
	cmp	r0, ip
	addgt	r1, r1, #1
.L181:
	ldr	ip, [r3, #4]!
	add	r2, r2, #6
	strh	ip, [r2, #142]
	cmp	r2, r7
	bne	.L182
	ldr	r3, [r5, #20]
	str	r1, [r5, #232]
	cmp	r3, #1
	beq	.L420
	cmp	r3, #2
	beq	.L314
	cmp	r3, #3
	moveq	r3, #1
	movne	r3, #2
	str	r3, [fp, #-168]
.L184:
	ldr	lr, [r5, #212]
	mov	r1, r4
	ldr	r3, [r5, #208]
	mov	r0, #0
	ldr	ip, [r5, #216]
	mov	r2, r0
	add	r3, r3, lr, lsl #1
	ldr	r9, [r1, #124]!
	add	ip, r3, ip, lsl #2
	mov	lr, r9
	mov	ip, ip, asl #1
.L216:
	ldr	r3, [r1], #4
	cmp	r9, r3
	add	r0, r0, r3
	movge	r9, r3
	cmp	lr, r3
	movlt	lr, r3
	cmp	r2, #4
	add	r2, r2, #1
	strgt	ip, [r4, #144]
	ldrle	r3, [r1]
	strle	r3, [r1, #-4]
	cmp	r2, #6
	bne	.L216
	rsb	r0, r9, r0
	mov	r2, #3392
	rsb	r0, lr, r0
	movt	r2, 3
	add	r1, r0, #3
	cmp	r0, #0
	add	r3, r4, r8
	movlt	r0, r1
	mov	r0, r0, asr #2
	cmp	ip, r0, asl #1
	movgt	r1, #1
	movle	r1, #0
	cmp	ip, r2
	movle	ip, #0
	andgt	ip, r1, #1
	cmp	r8, #0
	strb	ip, [r3, #111]
	streq	r8, [fp, #-172]
	moveq	ip, r8
	beq	.L217
	ldrsb	r3, [r4, #111]
	cmp	r3, #0
	beq	.L318
	ldrsb	r3, [r4, #112]
	cmp	r3, #0
	beq	.L318
	mov	r3, #1
	str	r3, [fp, #-172]
	mov	ip, r3
.L217:
	ldr	r9, [r5, #24]
	cmp	r9, #0
	beq	.L218
	ldr	r3, .L433
	add	r1, r5, #84
	ldr	r3, [r3]
	cmp	r3, #1
	beq	.L421
	ldr	r9, [r5, #76]
	cmp	r9, #1
	beq	.L422
.L226:
	ldr	r3, [r5, #40]
	cmp	r3, #288
	bgt	.L227
	ldr	r9, [r5, #72]
.L412:
	cmp	r9, #1
	beq	.L218
.L227:
	ldr	r3, [r4, #444]
	cmp	r3, #0
	beq	.L423
	ldr	r8, [r5, #28]
	ldr	r9, [r4, #252]
	cmp	r8, #0
	bne	.L424
	ldrsb	ip, [r4, #108]
	ldrsb	r3, [r4, #109]
	cmp	ip, #1
	beq	.L335
	cmp	ip, #2
	bne	.L277
	mov	r2, r8
.L268:
	ldr	r10, [r4, #28]
.L274:
	cmp	r9, #1
	beq	.L407
	sub	ip, r2, #1
	mov	r3, #2
	clz	ip, ip
	mov	ip, ip, lsr #5
.L271:
	ldr	r7, .L433+4
	sub	lr, fp, #144
	strb	r3, [r4, #109]
	cmp	ip, #1
	strb	ip, [r4, #108]
	ldmia	r7!, {r0, r1, r2, r3}
	ldr	r9, [r5, #8]
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r7!, {r0, r1, r2, r3}
	str	r9, [fp, #-176]
	ldr	r9, [r4, #16]
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	r7, {r0, r1}
	stmia	lr, {r0, r1}
	beq	.L280
	cmp	ip, #2
	beq	.L281
	cmp	ip, #0
	beq	.L282
	mov	r3, #0
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	tst	r10, #1
	ldr	r3, [fp, #-168]
	clzne	r9, r9
	movne	r9, r9, lsr #5
	cmp	ip, #2
	str	r3, [r6, #20]
	ldr	r3, [fp, #-172]
	str	r9, [r6]
	str	r3, [r6, #16]
	mov	r3, #0
	str	r3, [r6, #12]
	beq	.L330
	subs	ip, ip, #1
	mvnne	ip, #0
	mov	r3, ip
.L298:
	cmp	ip, #1
	bls	.L290
.L293:
	str	r3, [r6, #24]
	b	.L179
.L416:
	add	r1, r4, #472
	mov	r0, r6
	mov	r2, #28
	bl	memcpy
	b	.L392
.L420:
	ldr	r3, [r4, #444]
	cmp	r3, #0
	movne	r3, #2
	strne	r3, [fp, #-168]
	bne	.L184
	ldr	r2, [r5, #44]
	ldr	r3, [r5, #8]
	str	r2, [fp, #-168]
	cmp	r3, #2
	ldr	r2, [r5, #40]
	str	r2, [fp, #-172]
	beq	.L403
	cmp	r3, #1
	beq	.L425
	ldr	r3, [r4, #12]
	cmp	r3, #0
	beq	.L402
	ldr	r3, [r4, #20]
	ldr	ip, [r5, #220]
	cmp	r3, #1
	ldr	r0, [r5, #224]
	beq	.L296
.L192:
	ldr	r9, [r5, #228]
.L193:
	cmp	r0, r9
	rsb	r1, r1, #16
	movge	lr, r0
	movlt	lr, r9
	cmp	lr, #254
	movgt	r10, lr, asr #31
	movwgt	r2, #26215
	movle	r10, #50
	movtgt	r2, 26214
	smullgt	r2, r3, lr, r2
	rsbgt	r10, r10, r3, asr #1
	add	lr, lr, r10
	cmp	lr, ip
	movge	r3, #0
	movlt	r3, #1
	cmp	ip, r9
	str	r3, [fp, #-188]
	movge	lr, ip
	movlt	lr, r9
	cmp	lr, #254
	movgt	r10, lr, asr #31
	movwgt	r2, #26215
	movle	r10, #50
	movtgt	r2, 26214
	smullgt	r2, r3, lr, r2
	rsbgt	r10, r10, r3, asr #1
	add	r10, lr, r10
	cmp	r0, r10
	movgt	r3, #1
	movle	r3, #0
	cmp	ip, r0
	str	r3, [fp, #-192]
	movge	r3, ip
	movlt	r3, r0
	cmp	r3, #254
	str	r3, [fp, #-176]
	movgt	lr, r3, asr #31
	movwgt	r2, #26215
	movle	r3, #50
	movtgt	r2, 26214
	smullgt	r2, r3, r3, r2
	rsbgt	r3, lr, r3, asr #1
	cmp	r0, r9
	str	r3, [fp, #-196]
	movlt	lr, r0
	movge	lr, r9
	cmp	lr, #254
	movgt	r10, lr, asr #31
	movwgt	r2, #26215
	movle	r10, #50
	movtgt	r2, 26214
	smullgt	r2, r3, lr, r2
	rsbgt	r10, r10, r3, asr #1
	rsb	r10, r10, lr
	cmp	ip, r10
	movge	r3, #0
	movlt	r3, #1
	cmp	ip, r9
	str	r3, [fp, #-180]
	movlt	lr, ip
	movge	lr, r9
	cmp	lr, #254
	movgt	r10, lr, asr #31
	movwgt	r2, #26215
	movle	r10, #50
	movtgt	r2, 26214
	smullgt	r2, r3, lr, r2
	rsbgt	r10, r10, r3, asr #1
	rsb	r10, r10, lr
	cmp	r0, r10
	movge	r3, #0
	movlt	r3, #1
	cmp	ip, r0
	str	r3, [fp, #-184]
	movlt	r10, ip
	movge	r10, r0
	cmp	r10, #254
	rsb	r0, r0, ip
	movgt	lr, r10, asr #31
	movwgt	r2, #26215
	movle	r2, #50
	movtgt	r2, 26214
	smullgt	r2, r3, r10, r2
	movgt	r2, lr
	ldr	lr, [fp, #-168]
	rsbgt	r2, r2, r3, asr #1
	ldr	r3, [fp, #-172]
	rsb	r2, r2, r10
	str	r2, [fp, #-168]
	mul	r3, r3, lr
	mov	r3, r3, asr #9
	add	r2, r3, #31
	cmp	r3, #0
	movlt	r3, r2
	mov	r3, r3, asr #5
	mul	r1, r1, r3
	add	r3, r1, #15
	cmp	r1, #0
	movlt	r1, r3
	mov	r1, r1, asr #4
	add	r1, r1, r1, lsl #1
	add	r1, r1, r1, lsr #31
	mov	r1, r1, asr #1
	cmp	r1, #6
	movlt	r1, #6
	bl	__aeabi_idiv
	ldr	r2, [fp, #-176]
	cmp	r2, #100
	mov	r3, r0
	ble	.L201
	rsb	r10, r10, r2
	str	r0, [fp, #-172]
	mov	r0, #100
	mov	r1, r2
	mul	r0, r0, r10
	bl	__aeabi_idiv
	ldr	r3, [fp, #-172]
	cmp	r0, #19
	bgt	.L201
.L311:
	mov	r3, #0
.L202:
	ldr	r2, [fp, #-188]
	ldr	r1, [fp, #-184]
	cmp	r2, r1
	bls	.L203
	cmp	r3, #50
	movlt	r3, #50
.L204:
	ldr	r2, [fp, #-168]
	cmp	r9, r2
	blt	.L207
	ldr	r2, [fp, #-176]
	ldr	r1, [fp, #-196]
	add	lr, r2, r1
	cmp	r9, lr
	movgt	r3, #0
	bgt	.L207
	ldr	r2, [fp, #-184]
	ldr	r1, [fp, #-188]
	tst	r2, r1
	bne	.L206
	ldr	r2, [fp, #-180]
	ldr	r1, [fp, #-192]
	ands	r2, r2, r1
	bne	.L206
	ldr	r1, [fp, #-184]
	ldr	r0, [fp, #-180]
	orrs	r1, r1, r0
	movne	r3, r2
	bne	.L207
.L206:
	ldr	r1, [fp, #-188]
	ldr	r2, [fp, #-192]
	orr	r2, r2, r1
	ldr	r1, [fp, #-180]
	orr	r2, r1, r2
	ldr	r1, [fp, #-184]
	orrs	r2, r1, r2
	bne	.L207
	cmp	r3, #50
	mvn	r2, #49
	movge	r3, #50
	cmp	r3, r2
	movlt	r3, r2
.L207:
	ldr	r2, [r4, #116]
	add	r3, r3, r2
	str	r3, [r4, #116]
	cmp	r3, #1000
	movgt	r3, #1000
	strgt	r3, [r4, #116]
	bgt	.L209
	cmn	r3, #1000
	movw	r2, #64536
	movt	r2, 65535
	strlt	r2, [r4, #116]
	bge	.L426
.L211:
	mov	r3, #1
	str	r3, [r4, #120]
	str	r3, [fp, #-168]
	b	.L184
.L314:
	mov	r3, #0
	str	r3, [fp, #-168]
	b	.L184
.L421:
	ldr	r3, [r5, #76]
	ldr	r9, [r5, #72]
	cmp	r3, #1
	bne	.L412
	cmp	r9, #0
	bne	.L412
	ldr	r0, .L433+8
	ldr	r2, [r5, #80]
	ldr	lr, [r0]
	cmp	r2, lr
	bcc	.L227
	ldr	r0, [r0, #4]
	cmp	r2, r0
	bcs	.L227
	mov	r9, r3
.L218:
	ldr	r8, [r5, #28]
	cmp	r8, #0
	ldreq	r10, [r4, #28]
	bne	.L427
.L231:
	ldrsb	ip, [r4, #108]
	ldrsb	r3, [r4, #109]
	cmp	ip, #1
	beq	.L270
	cmp	ip, #2
	bne	.L278
	mov	r2, #0
	b	.L274
.L318:
	str	r3, [fp, #-172]
	mov	ip, r3
	b	.L217
.L277:
	ldr	r10, [r4, #28]
.L278:
	cmp	r9, #1
	bne	.L407
	mov	r9, #0
.L272:
	strb	r9, [r4, #109]
	mov	r3, #2
	ldr	r9, [r4, #16]
	strb	r3, [r4, #108]
.L281:
	mov	r3, #1
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	tst	r10, #1
	ldr	r3, [fp, #-168]
	clzne	r9, r9
	mov	r2, #1
	str	r2, [r6, #12]
	movne	r9, r9, lsr #5
	str	r9, [r6]
	str	r3, [r6, #20]
	ldr	r3, [fp, #-172]
	str	r3, [r6, #16]
	mov	r3, r2
.L290:
	ldr	r2, .L433+12
	add	r3, r2, r3, lsl #2
	ldr	r3, [r3, #140]
	b	.L293
.L427:
	ldr	r8, [r4, #444]
	ldr	r10, [r4, #28]
	cmp	r8, #0
	ldr	r3, [r5, #208]
	ldr	r0, [r5, #212]
	ldr	r1, [r5, #216]
	bne	.L229
	ldr	r2, [r4, #456]
	cmp	r2, #1
	ble	.L230
	mov	r1, r8
	mov	r2, #60
	add	r0, r4, #364
	bl	memset
	mov	r1, r8
	mov	r2, #20
	add	r0, r4, #424
	ldrb	r8, [r4, #374]
	bl	memset
	b	.L231
.L335:
	mov	r2, r8
.L267:
	cmp	r2, #1
	ldr	r10, [r4, #28]
	bne	.L270
.L407:
	uxtb	r3, r3
	b	.L271
.L270:
	cmp	r9, #1
	beq	.L272
	ldr	r9, [r4, #16]
	mov	r2, #0
	mov	r3, #1
	strb	r2, [r4, #108]
	strb	r3, [r4, #109]
.L282:
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
.L285:
	ldr	r2, [fp, #-168]
	str	r2, [r6, #20]
	ldr	r2, [fp, #-172]
	str	r2, [r6, #16]
	b	.L298
.L431:
	cmp	r0, #4
	movle	r0, #0
	movgt	r0, #1
	cmp	r0, #0
	beq	.L248
	cmp	r0, #1
	bne	.L325
	ldr	r3, [fp, #-176]
	ldrsb	r2, [r3, #113]
	add	r1, r2, #1
	cmp	r5, r1
	beq	.L249
	add	r1, r5, #4
	cmp	r2, r1
	beq	.L249
.L325:
	str	r5, [fp, #-196]
.L248:
	add	r6, r6, #1
.L252:
	add	r5, r5, #1
	add	r4, r4, #4
	cmp	r5, #5
	add	r7, r7, #20
	bne	.L253
	ldr	ip, [fp, #-212]
	mov	r1, r6
	ldr	r2, [fp, #-200]
	ldr	r9, [fp, #-216]
	cmp	ip, #0
	moveq	r3, r2
	orrne	r3, r2, #1
	ldr	r10, [fp, #-220]
	cmp	r1, #4
	movle	ip, r3
	orrgt	ip, r3, #1
	ldr	r4, [fp, #-176]
	cmp	ip, #0
	ldr	r5, [fp, #-204]
	ldr	r6, [fp, #-224]
	bne	.L428
	ldr	r3, [fp, #-188]
	cmp	r3, #4
	bhi	.L257
	add	r2, r4, r3, lsl #2
	ldr	r0, [r4, #12]
	mov	ip, r3
	and	r3, r3, #1
	ldr	r2, [r2, #424]
	cmp	r2, #3
	strb	r2, [r4, #360]
	movle	r1, #0
	movgt	r1, #1
	cmp	r0, r3
	str	r1, [r4, #352]
	ldreqb	r3, [fp, #-188]
	movne	r3, ip
	addne	r3, r3, #5
	uxtbne	r3, r3
	strb	r3, [r4, #362]
.L257:
	ldr	r2, [fp, #-188]
	cmp	r2, #0
	mov	r3, r2
	moveq	r3, #1
	movne	r3, #0
	strb	r3, [r4, #361]
	blt	.L260
	uxtb	r3, r2
.L261:
	add	r2, r4, #352
	strb	r3, [r4, #113]
	ldr	ip, [r4, #400]
	ldmia	r2, {r0, r1, r2}
	cmp	ip, #1
	ldr	ip, [fp, #-192]
	mov	r3, r0
	stmia	ip, {r0, r1, r2}
	beq	.L429
.L234:
	add	r7, r4, #376
	add	lr, r4, #388
	add	r8, r4, #364
	add	ip, r4, #400
	ldmia	r7, {r0, r1, r2}
	str	r3, [fp, #-144]
	add	r3, r4, #412
	stmia	r8, {r0, r1, r2}
	ldmia	lr, {r0, r1, r2}
	stmia	r7, {r0, r1, r2}
	ldmia	ip, {r0, r1, r2}
	stmia	lr, {r0, r1, r2}
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	ldr	r2, [fp, #-192]
	ldmia	r2, {r0, r1, r2}
	stmia	r3, {r0, r1, r2}
.L229:
	ldr	r3, [r4, #452]
	cmp	r3, #0
	ldr	r3, [r4, #448]
	bne	.L430
	cmp	r3, #0
	bne	.L263
	mov	r3, r10, asl #4
	add	r2, r4, #12
	sub	r3, r3, r10, asl #2
	add	r3, r3, #352
	add	r1, r2, r3
	ldr	r2, [r2, r3]
	ldrb	r8, [r1, #10]
.L264:
	ldrsb	ip, [r4, #108]
	ldrsb	r3, [r4, #109]
	cmp	ip, #1
	beq	.L267
	cmp	ip, #2
	beq	.L268
	cmp	r2, #1
	bne	.L277
	ldr	lr, .L433+4
	mov	r3, #0
	strb	r2, [r4, #108]
	sub	ip, fp, #144
	strb	r3, [r4, #109]
	ldmia	lr!, {r0, r1, r2, r3}
	ldr	r7, [r5, #8]
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	str	r7, [fp, #-176]
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1}
	stmia	ip, {r0, r1}
.L280:
	ldr	r3, [fp, #-176]
	mov	r2, #1
	cmp	r3, #0
	mov	r3, #0
	stmib	r6, {r2, r3}
	bne	.L286
	sxtb	r3, r8
	movw	r0, #26215
	add	r3, r3, r2
.L408:
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
.L287:
	mov	ip, #0
	str	ip, [r6, #12]
	mov	r3, ip
	b	.L285
.L286:
	ldr	r3, [fp, #-176]
	cmp	r3, #1
	bne	.L287
	sxtb	r3, r8
	movw	r0, #26215
	add	r3, r3, #2
	b	.L408
.L423:
	ldr	lr, [r5, #8]
	mov	r0, r8
	add	r3, r4, #244
	add	r2, r4, #256
	str	ip, [fp, #-176]
	str	lr, [sp]
	bl	Detect22Pld
	ldr	ip, [fp, #-176]
	mov	r9, r0
	b	.L218
.L430:
	cmp	r3, #0
	subne	r1, r10, #2
	bne	.L406
.L263:
	sub	r1, r10, #1
.L406:
	mov	r3, r1, asl #4
	add	r2, r4, #12
	sub	r3, r3, r1, asl #2
	add	r3, r3, #352
	add	r1, r2, r3
	ldr	r2, [r2, r3]
	ldrb	r8, [r1, #10]
	b	.L264
.L422:
	ldr	r2, .L433+8
	ldr	r3, [r5, #80]
	ldr	r0, [r2]
	cmp	r3, r0
	bcc	.L226
	ldr	r2, [r2, #4]
	cmp	r3, r2
	bcc	.L218
	b	.L226
.L230:
	ldr	lr, [r4, #104]
	ldr	r2, [r4, #100]
	cmp	lr, #0
	beq	.L236
	add	r3, r3, r0, lsl #1
	add	r3, r3, r1, lsl #2
	mov	r3, r3, asl #1
	str	r3, [lr]
	ldr	r3, [r4, #104]
	ldr	r1, [r4, #32]
	add	r3, r3, #4
	cmp	r3, r7
	addhi	r3, r4, #36
	str	r1, [fp, #-180]
	cmp	r3, #0
	str	r3, [r4, #104]
	add	r3, r1, #1
	str	r3, [r4, #32]
	beq	.L236
	cmp	r3, #4
	ble	.L236
	ldr	r3, [r2]
	sub	r1, fp, #44
	str	ip, [fp, #-188]
	mov	r0, r1
	mov	r1, #0
	str	r3, [r0, #-120]!
	mov	lr, r1
	str	r3, [fp, #-176]
	mov	r8, r3
	str	r0, [fp, #-184]
	b	.L237
.L240:
	ldr	r3, [r2]
	ldr	ip, [fp, #-176]
	cmp	r3, ip
	str	r3, [r0, #4]!
	strlt	r3, [fp, #-176]
	movlt	lr, r1
.L237:
	cmp	r8, r3
	add	r2, r2, #4
	add	r1, r1, #1
	movlt	r8, r3
	cmp	r7, r2
	addcc	r2, r4, #36
	cmp	r1, #5
	bne	.L240
	ldr	ip, [fp, #-188]
	mov	r3, #0
	ldr	r1, [fp, #-184]
	str	r8, [fp, #-204]
.L242:
	ldr	r2, [r1], #4
	cmp	r3, lr
	cmpne	r2, r8
	add	r3, r3, #1
	movlt	r8, r2
	cmp	r3, #5
	bne	.L242
	ldr	r3, [r4, #100]
	ldr	r2, [fp, #-180]
	add	r3, r3, #4
	ldr	r0, [fp, #-176]
	cmp	r7, r3
	str	ip, [fp, #-212]
	str	r2, [r4, #32]
	mvn	r2, #0
	addcc	r3, r4, #36
	mov	lr, r2
	str	r3, [r4, #100]
	mov	r7, r0, asl #1
	ldr	r1, [r5, #124]
	add	r3, r4, #424
	str	r2, [fp, #-196]
	sub	r0, fp, #144
	ldr	r2, [r5, #128]
	str	r0, [fp, #-192]
	mov	r0, r3
	cmp	r2, r1
	ldr	ip, [fp, #-192]
	str	r7, [fp, #-180]
	sub	r7, fp, #128
	movge	r2, r1
	cmp	r2, #64
	str	lr, [fp, #-188]
	mov	lr, #0
	movle	r2, #0
	movgt	r2, #1
	str	r9, [fp, #-216]
	str	r2, [r4, #356]
	ldr	r9, [fp, #-176]
	str	r10, [fp, #-220]
	str	r4, [fp, #-176]
	mov	r4, r0
	ldr	r10, [fp, #-204]
	str	r6, [fp, #-224]
	mov	r6, lr
	str	r5, [fp, #-204]
	mov	r5, lr
	str	r3, [fp, #-208]
	str	r2, [fp, #-200]
.L253:
	mov	r0, ip
	mov	r2, #100
	ldr	r1, .L433+12
	bl	memcpy
	ldr	r3, [fp, #-180]
	cmp	r8, r3
	mov	ip, r0
	blt	.L248
	sub	r1, r7, #20
	ldr	r2, [fp, #-184]
	mov	r0, #0
	b	.L247
.L432:
	cmp	r3, r9
	addeq	r0, r0, #1
.L246:
	cmp	r7, r1
	add	r2, r2, #4
	beq	.L431
.L247:
	ldr	r3, [r1, #4]!
	cmp	r3, #0
	ldr	r3, [r2]
	beq	.L432
	mov	r3, r3, asl #5
	cmp	r3, r10
	addgt	r0, r0, #1
	b	.L246
.L209:
	mov	r3, #0
.L403:
	str	r3, [r4, #120]
	str	r3, [fp, #-168]
	b	.L184
.L330:
	mov	r3, #1
	b	.L290
.L236:
	sub	r0, fp, #144
	add	r2, r4, #352
	mov	r3, #0
	mvn	r1, #0
	mov	lr, r0
	strb	r3, [r4, #360]
	str	r3, [r4, #352]
	str	r0, [fp, #-192]
	strb	r3, [r4, #361]
	strb	r1, [r4, #113]
	ldmia	r2, {r0, r1, r2}
	ldr	ip, [r4, #400]
	cmp	ip, #1
	stmia	lr, {r0, r1, r2}
	bne	.L234
.L233:
	mov	r1, #2
	mov	r2, #0
	strb	r1, [r4, #420]
	strb	r1, [r4, #408]
	strb	r1, [r4, #396]
	strb	r1, [r4, #384]
	str	r2, [r4, #412]
	str	r2, [r4, #400]
	str	r2, [r4, #388]
	str	r2, [r4, #376]
	b	.L234
.L426:
	cmp	r3, #200
	bgt	.L209
	cmn	r3, #200
	blt	.L211
.L402:
	ldr	r3, [r4, #120]
	str	r3, [fp, #-168]
	b	.L184
.L201:
	add	r2, r3, #49
	cmp	r2, #98
	bls	.L311
	movw	r2, #65036
	movt	r2, 65535
	cmp	r3, r2
	movlt	r3, r2
	cmp	r3, #500
	movge	r3, #500
	b	.L202
.L428:
	cmp	r2, #0
	movne	r3, #0
	strneb	r3, [r4, #360]
	bne	.L256
	ldrb	r3, [r4, #360]
	sub	r3, r3, #1
	sxtb	r3, r3
	cmp	r3, #4
	movge	r3, #4
	bic	r3, r3, r3, asr #31
	strb	r3, [r4, #360]
.L256:
	add	r3, r4, #360
	mov	r2, #0
	ldrsb	r3, [r3]
	str	r2, [r4, #352]
	strb	r2, [r4, #362]
	str	r3, [r4, #424]
	str	r3, [r4, #428]
	str	r3, [r4, #432]
	str	r3, [r4, #436]
	str	r3, [r4, #440]
	strb	r2, [r4, #361]
.L260:
	ldrb	r3, [fp, #-196]
	b	.L261
.L249:
	ldr	r2, [r4]
	add	r2, r2, #1
	cmp	r2, #5
	movge	r2, #5
	bic	r2, r2, r2, asr #31
	str	r2, [r4]
	cmp	r2, #3
	ble	.L405
	ldr	r1, [fp, #-208]
	mov	r2, #0
	mov	r0, #3
.L251:
	cmp	r5, r2
	add	r2, r2, #1
	strne	r0, [r1]
	cmp	r2, #5
	add	r1, r1, #4
	bne	.L251
.L405:
	str	r5, [fp, #-196]
	str	r5, [fp, #-188]
	b	.L252
.L429:
	cmp	r0, #0
	bne	.L234
	b	.L233
.L203:
	ldr	r2, [fp, #-192]
	ldr	r1, [fp, #-180]
	cmp	r2, r1
	bls	.L204
	mvn	r2, #49
	cmp	r3, r2
	movge	r3, r2
	b	.L204
.L425:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	bne	.L187
	ldr	r3, [r4, #24]
	cmp	r3, #0
	beq	.L402
.L393:
	ldr	r3, [r4, #12]
	cmp	r3, #0
	ldrne	ip, [r5, #220]
	ldrne	r0, [r5, #224]
	bne	.L192
	b	.L402
.L187:
	cmp	r3, #1
	bne	.L393
	ldr	r3, [r4, #24]
	cmp	r3, #1
	beq	.L402
	ldr	r3, [r4, #12]
	cmp	r3, #0
	ldrne	ip, [r5, #220]
	ldrne	r0, [r5, #224]
	beq	.L402
.L296:
	mov	r3, r0
	ldr	r9, [r5, #228]
	mov	r0, ip
	mov	ip, r3
	b	.L193
.L424:
	ldr	r10, [r4, #28]
	b	.L229
.L434:
	.align	2
.L433:
	.word	.LANCHOR0
	.word	.LANCHOR2+100
	.word	.LANCHOR1
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
	.type	g_u32IPDetect, %object
	.size	g_u32IPDetect, 4
g_u32IPDetect:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
