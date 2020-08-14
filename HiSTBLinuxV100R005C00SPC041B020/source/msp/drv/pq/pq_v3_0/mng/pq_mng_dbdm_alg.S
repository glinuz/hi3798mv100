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
	.file	"pq_mng_dbdm_alg.c"
	.text
	.align	2
	.type	CheckStartEndBorder, %function
CheckStartEndBorder:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	ble	.L2
	add	r6, r1, r1, lsr #31
	mov	lr, r2
	mov	ip, #0
	mov	r6, r6, asr #1
	b	.L4
.L3:
	add	ip, ip, #1
	cmp	ip, r0
	beq	.L2
.L4:
	mov	r4, lr
	add	lr, lr, #1
	ldrb	r5, [r4]
	cmp	r5, #1
	bne	.L3
	cmp	r6, ip
	blt	.L3
	mov	ip, #0
	strb	ip, [r4]
	ldr	ip, [r3]
	sub	ip, ip, #1
	str	ip, [r3]
.L2:
	cmp	r0, #1
	ldmlefd	sp, {r4, r5, r6, fp, sp, pc}
	add	r1, r1, r1, lsr #31
	sub	ip, r0, #1
	add	r2, r2, ip
	mov	ip, #1
	mov	r4, r1, asr ip
	b	.L7
.L6:
	add	ip, ip, #1
	cmp	ip, r0
	beq	.L11
.L7:
	mov	r1, r2
	sub	r2, r2, #1
	ldrb	lr, [r1]
	cmp	lr, #1
	bne	.L6
	cmp	r4, ip
	blt	.L6
	mov	r2, #0
	strb	r2, [r1]
	ldr	r2, [r3]
	sub	r2, r2, #1
	str	r2, [r3]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L11:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	CheckStartEndBorder, .-CheckStartEndBorder
	.align	2
	.type	DetBorderOffsetRuler.isra.1, %function
DetBorderOffsetRuler.isra.1:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #60)
	sub	sp, sp, #60
	cmp	r1, #1
	mov	r5, r3
	str	r0, [fp, #-88]
	mov	r3, #0
	mov	r4, r2
	str	r3, [fp, #-76]
	str	r3, [fp, #-72]
	str	r3, [fp, #-68]
	str	r3, [fp, #-64]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-52]
	str	r3, [fp, #-48]
	str	r3, [fp, #-80]
	strb	r3, [fp, #-81]
	beq	.L78
	cmp	r1, #2
	subne	r7, fp, #80
	subne	r3, fp, #81
	ldreq	r3, [fp, #4]
	ldreq	r2, [fp, #4]
	addeq	r3, r3, #8192
	strne	r3, [fp, #-92]
	addeq	r7, r2, #8640
	ldreq	r3, [r3, #440]
	addeq	r7, r7, #16
	streq	r3, [fp, #-92]
.L14:
	cmp	r4, #8
	ldr	r3, [fp, #-88]
	beq	.L15
	sub	r1, r3, #3
	cmp	r1, #4
	bls	.L53
	ldrb	r3, [r5, #4]
	cmp	r3, #1
	beq	.L53
	add	r2, r5, #4
	mov	r10, r1
	mov	r3, #4
	b	.L17
.L27:
	ldrb	r0, [r2, #1]!
	cmp	r0, #1
	beq	.L55
.L17:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L27
.L16:
	rsb	ip, r4, r10
	cmp	ip, #3
	ble	.L32
	rsb	r2, r4, ip
	rsb	r6, r4, #0
	add	ip, r5, ip
	mov	r3, #0
	mov	lr, #1
.L31:
	add	r2, r2, r6
	strb	lr, [ip, r3]
	add	r0, r2, r4
	rsb	r3, r4, r3
	cmp	r0, #3
	bgt	.L31
.L32:
	cmp	r1, #0
	mov	r2, r1
	ble	.L30
	ldrb	r0, [r5, r1]
	add	r3, r5, r1
	cmp	r0, #1
	bne	.L33
	b	.L30
.L34:
	ldrb	r0, [r3, #-1]!
	cmp	r0, #1
	beq	.L30
.L33:
	subs	r2, r2, #1
	bne	.L34
.L30:
	add	r3, r4, r2
	cmp	r3, r1
	movcc	r0, #1
	bcs	.L39
.L38:
	strb	r0, [r5, r3]
	add	r3, r3, r4
	cmp	r3, r1
	bcc	.L38
.L39:
	cmp	r10, r2
	bgt	.L37
	add	r3, r2, #1
	str	r7, [fp, #-96]
	mov	ip, r10
	mov	r1, r10
	mov	r2, r3
	sub	r6, r4, #1
	rsb	r9, r4, #0
	add	r8, r4, #1
	mov	r3, #1
	mov	r7, r2
	b	.L41
.L42:
	mov	ip, lr
.L41:
	add	lr, ip, #1
	cmp	lr, r7
	beq	.L79
	cmp	lr, r10
	moveq	r1, r10
	beq	.L42
	ldrb	r2, [r5, lr]
	cmp	r2, #1
	bne	.L42
	add	r0, r4, r1
	rsb	r2, r0, lr
	cmp	r2, r6
	blt	.L57
	add	r1, r4, r0
	rsb	r2, r1, ip
.L43:
	add	r2, r2, r9
	strb	r3, [r5, r0]
	add	ip, r8, r2
	mov	r0, r1
	cmp	r6, ip
	add	r1, r4, r1
	ble	.L43
.L57:
	mov	r1, lr
	b	.L42
.L79:
	ldr	r7, [fp, #-96]
.L37:
	ldr	r3, [fp, #-88]
	mov	lr, #0
	str	lr, [r7]
	cmp	r3, #4
	bls	.L26
	ldr	r3, [fp, #-88]
	add	r1, r5, #3
	sub	r0, r3, #1
	ldr	r3, [fp, #4]
	add	ip, r3, #8192
	ldr	r3, [fp, #-92]
	add	r0, r3, r0
	add	r3, r3, #3
	b	.L49
.L45:
	ldrb	r2, [r3]
	sub	r6, r2, #1
	sxtb	r2, r2
	cmp	r2, #0
	strleb	lr, [r3]
	ble	.L47
.L76:
	strb	r6, [r3]
.L47:
	add	r3, r3, #1
	cmp	r3, r0
	beq	.L26
.L49:
	ldrb	r2, [r1, #1]!
	cmp	r2, #1
	bne	.L45
	ldr	r6, [r7]
	add	r6, r6, #1
	str	r6, [r7]
	strb	lr, [r1]
	strb	r2, [r1, #-1]
	ldrb	r2, [r3]
	ldrb	r6, [ip, #452]
	add	r8, r2, #1
	sxtb	r2, r2
	cmp	r2, r6
	strltb	r8, [r3]
	bge	.L76
	add	r3, r3, #1
	cmp	r3, r0
	bne	.L49
.L26:
	mov	r3, r7
	mov	r2, r5
	mov	r1, r4
	ldr	r0, [fp, #-88]
	bl	CheckStartEndBorder
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L55:
	mov	r10, r3
	b	.L16
.L15:
	sub	r8, r3, #3
	cmp	r8, #4
	bls	.L54
	add	r2, r5, #3
	mov	r3, #4
	b	.L20
.L19:
	add	r3, r3, #1
	cmp	r3, r8
	beq	.L80
.L20:
	ldrb	r1, [r2, #1]!
	cmp	r1, #1
	bne	.L19
	and	r1, r3, #7
	sub	r0, fp, #44
	add	r3, r3, #1
	add	r1, r0, r1, lsl #2
	cmp	r3, r8
	ldr	r0, [r1, #-32]
	add	r0, r0, #1
	str	r0, [r1, #-32]
	bne	.L20
.L80:
	ldr	r1, [fp, #-76]
.L18:
	mov	r3, #0
	sub	r0, fp, #76
	mov	ip, r3
	mov	r2, r3
	b	.L50
.L22:
	ldr	r1, [r0, #4]!
.L50:
	cmp	r1, ip
	movhi	r3, r2
	add	r2, r2, #1
	movhi	ip, r1
	cmp	r2, #8
	bne	.L22
	ldr	r2, [fp, #-88]
	mov	r1, #0
	mov	r0, r5
	uxtb	r6, r3
	bl	memset
	cmp	r6, #0
	mov	r3, #0
	str	r3, [r7]
	sxtbne	r3, r6
	moveq	r3, r6
	addne	r1, r5, r3
	add	r3, r3, #7
	movne	r2, #1
	strneb	r2, [r1, #-1]
	strne	r2, [r7]
	cmp	r3, r8
	movcc	r1, #1
	bcs	.L26
.L25:
	strb	r1, [r5, r3]
	add	r3, r3, #8
	ldr	r2, [r7]
	cmp	r3, r8
	add	r2, r2, #1
	str	r2, [r7]
	bcc	.L25
	b	.L26
.L78:
	ldr	r3, [fp, #4]
	ldr	r2, [fp, #4]
	add	r3, r3, #8192
	add	r7, r2, #8640
	ldr	r3, [r3, #444]
	add	r7, r7, #12
	str	r3, [fp, #-92]
	b	.L14
.L53:
	mov	r10, #4
	b	.L16
.L54:
	mov	r1, #0
	b	.L18
	UNWIND(.fnend)
	.size	DetBorderOffsetRuler.isra.1, .-DetBorderOffsetRuler.isra.1
	.global	__aeabi_uidiv
	.align	2
	.global	PQ_MNG_ALG_GetDbDetInfo
	.type	PQ_MNG_ALG_GetDbDetInfo, %function
PQ_MNG_ALG_GetDbDetInfo:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #92)
	sub	sp, sp, #92
	mov	r4, r2
	mov	r10, r0
	movw	r2, #20508
	mov	r7, r1
	ldr	r0, .L291
	mov	r1, #0
	ldr	r8, .L291+4
	bl	memset
	mov	r1, r4
	movw	r2, #8260
	mov	r0, r10
	bl	memcpy
	ldr	r2, [r4, #48]
	ldr	r0, .L291
	add	r3, r2, #1
	strb	r2, [r8, #12]
	ldr	r2, [r4, #64]
	uxtb	r3, r3
	cmp	r3, #64
	strb	r2, [r8, #13]
	add	r2, r2, #1
	ldr	r3, [r4, #56]
	uxtb	r2, r2
	strb	r3, [r8, #14]
	add	r3, r3, #1
	ldr	r1, [r4, #44]
	uxtb	r3, r3
	str	r1, [r8]
	ldr	r1, [r4, #60]
	str	r1, [r8, #4]
	ldr	r1, [r4, #52]
	str	r1, [r8, #8]
	mvnhi	r1, #0
	strhib	r1, [r8, #12]
	cmp	r2, #64
	mov	r1, #0
	mvnhi	r2, #0
	strhib	r2, [r8, #13]
	cmp	r3, #64
	mov	r2, #16384
	mvnhi	r3, #0
	strhib	r3, [r8, #14]
	bl	memset
	ldr	r2, .L291+8
	add	r1, r4, #576
	add	ip, r2, #15360
.L85:
	ldr	r3, [r1, #4]!
	ubfx	r0, r3, #0, #11
	ubfx	r3, r3, #16, #11
	str	r0, [r2, #-4]
	str	r3, [r2], #8
	cmp	r2, ip
	bne	.L85
	movw	r2, #3960
	mov	r1, #0
	ldr	r0, .L291+12
	mov	r6, #1
	bl	memset
	ldr	lr, .L291
	add	ip, r4, #64
	movw	r9, #3969
	movw	r5, #3959
.L86:
	cmp	r6, r9
	ldr	r3, [ip, #4]!
	beq	.L87
	add	r2, r6, #16384
	and	r3, r3, #1
	add	r2, r2, #14
	mov	r1, r6
	mov	r0, #1
	strb	r3, [lr, r2]
	b	.L88
.L276:
	mov	r2, r2, asr r0
	add	r0, r0, #1
	cmp	r0, #32
	and	r2, r2, #1
	add	r1, r1, #1
	strb	r2, [r3]
	beq	.L275
.L88:
	add	r3, lr, r1
	cmp	r1, r5
	add	r3, r3, #16384
	ldr	r2, [ip]
	add	r3, r3, #15
	bls	.L276
.L87:
	ldr	r5, .L291+16
	movw	r3, #3836
	ldr	r0, .L291+12
	add	r1, r0, #1920
	add	r2, r0, #2880
	str	r0, [r5, #16]
	str	r1, [r5, #20]
	str	r2, [r5, #24]
	ldr	r2, [r4, #8]
	ldr	r6, [r4]
	ldr	r1, [r4, #4]
	str	r2, [fp, #-84]
	sub	r2, r6, #4
	cmp	r2, r3
	str	r1, [fp, #-80]
	bhi	.L211
	ldr	r3, [fp, #-80]
	sub	r3, r3, #1
	cmp	r3, #2160
	bcs	.L211
.L90:
	ldr	r2, [fp, #-80]
	movw	r3, #1080
	cmp	r6, #1920
	cmple	r2, r3
	movgt	r3, #0
	strgt	r3, [r4, #36]
	strgt	r3, [r4, #40]
	ldr	r3, [fp, #-84]
	cmp	r3, #960
	movgt	r3, #0
	strgt	r3, [r4, #36]
	ldr	r3, [r4, #32]
	cmp	r3, #1
	beq	.L277
.L93:
	add	r9, r7, #8192
	mvn	r3, #0
	strb	r3, [r7]
.L160:
	ldr	r3, [r4, #36]
	cmp	r3, #1
	beq	.L278
.L161:
	mvn	r3, #0
	strb	r3, [r7, #1]
.L185:
	ldr	r3, [r4, #40]
	cmp	r3, #1
	beq	.L279
.L186:
	mvn	r3, #0
	strb	r3, [r7, #2]
.L210:
	ldr	r3, [r7, #4]
	mov	r0, #0
	str	r3, [r10, #44]
	ldrsb	r3, [r7]
	str	r3, [r10, #48]
	ldr	r3, [r7, #8]
	str	r3, [r10, #52]
	ldrsb	r3, [r7, #2]
	str	r3, [r10, #56]
	ldr	r3, [r7, #12]
	str	r3, [r10, #60]
	ldrsb	r3, [r7, #1]
	str	r3, [r10, #64]
	ldr	r3, [r9, #456]
	str	r3, [r10, #16]
	ldr	r3, [r9, #460]
	str	r3, [r10, #20]
	ldr	r3, [r9, #464]
	str	r3, [r10, #24]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L211:
	mov	r3, #0
	str	r3, [r4, #28]
	b	.L90
.L275:
	add	r6, r6, #32
	b	.L86
.L277:
	ldr	r3, [r4, #28]
	cmp	r3, #1
	bne	.L93
	add	r3, r7, #4096
	ldr	r1, [r8]
	str	r3, [fp, #-88]
	mov	r2, r3
	ldr	r3, .L291+4
	ldrb	r2, [r2, #30]
	cmp	r1, r2
	bcc	.L94
	ldrb	r0, [r3, #12]
	add	r3, r0, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L95
.L96:
	ldr	r3, [fp, #-88]
	ldrb	r1, [r3, #28]
	add	r3, r7, #4096
	add	r3, r3, #52
	sub	r2, r1, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L97
.L98:
	sxtb	ip, r2
	sub	r2, r2, #1
	ands	r2, r2, #255
	sxth	ip, ip
	mov	r0, ip, asl #3
	sub	r1, r0, #8
	add	r0, r3, r0
	add	lr, r3, r1
	ldrb	r1, [r3, r1]
	strb	r1, [r3, ip, asl #3]
	ldr	r1, [lr, #4]
	str	r1, [r0, #4]
	bne	.L98
	ldr	r2, [fp, #-88]
	ldrb	r0, [r8, #12]
	ldrb	r1, [r2, #28]
.L97:
	ldr	ip, [fp, #-88]
	ldr	r2, [ip, #36]
	strb	r0, [ip, #52]
	ldr	r0, [r8]
	cmp	r2, r1
	addcc	r2, r2, #1
	str	r0, [ip, #56]
	movcc	r0, ip
	strcc	r2, [r0, #36]
	cmp	r1, r2
	beq	.L100
.L105:
	ldrb	r3, [r7]
	ldrb	r2, [r8, #12]
.L101:
	cmp	r2, r3
	sxtb	r2, r3
	str	r2, [fp, #-116]
	beq	.L280
	add	r9, r7, #8192
	mov	r2, r6
	mov	r1, #0
	ldr	r0, [r9, #436]
	bl	memset
	mov	r3, #0
	strb	r3, [r9, #448]
	b	.L160
.L279:
	ldr	r3, [r4, #28]
	cmp	r3, #1
	bne	.L186
	add	r3, r7, #4096
	ldr	r1, [r8, #8]
	str	r3, [fp, #-88]
	mov	r2, r3
	ldr	r3, .L291+4
	ldrb	r2, [r2, #32]
	cmp	r1, r2
	bcc	.L187
	ldrb	r0, [r3, #14]
	add	r3, r0, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L188
.L189:
	ldr	r3, [fp, #-88]
	ldrb	r1, [r3, #28]
	add	r3, r7, #4224
	add	r3, r3, #52
	sub	r2, r1, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L190
.L191:
	sxtb	r0, r2
	sub	r2, r2, #1
	ands	r2, r2, #255
	sxth	r0, r0
	mov	r1, r0, asl #3
	sub	ip, r1, #8
	add	r1, r3, r1
	add	lr, r3, ip
	ldrb	ip, [r3, ip]
	strb	ip, [r3, r0, asl #3]
	ldr	r0, [lr, #4]
	str	r0, [r1, #4]
	bne	.L191
	ldr	r2, [fp, #-88]
	ldrb	r0, [r8, #14]
	ldrb	r1, [r2, #28]
.L190:
	ldr	ip, [fp, #-88]
	ldr	r2, [ip, #44]
	strb	r0, [ip, #180]
	ldr	r0, [r8, #8]
	cmp	r2, r1
	addcc	r2, r2, #1
	str	r0, [ip, #184]
	movcc	r0, ip
	strcc	r2, [r0, #44]
	cmp	r1, r2
	beq	.L193
.L198:
	ldrb	r3, [r7, #2]
	ldrb	r2, [r8, #14]
.L194:
	cmp	r2, r3
	sxtb	r2, r3
	bne	.L199
	add	r3, r3, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L200
	ldr	r3, [r5, #24]
	mov	r1, #1
	str	r7, [sp]
	ldr	r0, [fp, #-80]
	bl	DetBorderOffsetRuler.isra.1
	ldrsb	r3, [r7, #2]
	ldr	r4, .L291+16
	cmp	r3, #8
	beq	.L201
	ldrb	r3, [r9, #450]
	ldrb	r2, [r9, #451]
	cmp	r3, r2
	addcc	r3, r3, #1
	uxtbcc	r3, r3
	strccb	r3, [r9, #450]
	cmp	r2, r3
	beq	.L281
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-80]
	ldr	r1, [r5, #24]
	ldr	r0, [r3, #20]
	bl	memcpy
.L209:
	ldr	r3, [r8, #8]
	str	r3, [r7, #8]
	b	.L210
.L278:
	ldr	r3, [r4, #28]
	cmp	r3, #1
	bne	.L161
	add	r3, r7, #4096
	ldr	r1, [r8, #4]
	str	r3, [fp, #-88]
	mov	r2, r3
	ldr	r3, .L291+4
	ldrb	r2, [r2, #31]
	cmp	r1, r2
	bcc	.L162
	ldrb	r0, [r3, #13]
	add	r3, r0, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L163
.L164:
	ldr	r3, [fp, #-88]
	ldrb	r1, [r3, #28]
	add	r3, r7, #4352
	add	r3, r3, #52
	sub	r2, r1, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L165
.L166:
	sxtb	ip, r2
	sub	r2, r2, #1
	ands	r2, r2, #255
	sxth	ip, ip
	mov	r0, ip, asl #3
	sub	r1, r0, #8
	add	r0, r3, r0
	add	lr, r3, r1
	ldrb	r1, [r3, r1]
	strb	r1, [r3, ip, asl #3]
	ldr	r1, [lr, #4]
	str	r1, [r0, #4]
	bne	.L166
	ldr	r2, [fp, #-88]
	ldrb	r0, [r8, #13]
	ldrb	r1, [r2, #28]
.L165:
	ldr	ip, [fp, #-88]
	ldr	r2, [ip, #40]
	strb	r0, [ip, #308]
	ldr	r0, [r8, #4]
	cmp	r2, r1
	addcc	r2, r2, #1
	str	r0, [ip, #312]
	movcc	r0, ip
	strcc	r2, [r0, #40]
	cmp	r1, r2
	beq	.L168
.L173:
	ldrb	r3, [r7, #1]
	ldrb	r2, [r8, #13]
.L169:
	cmp	r2, r3
	sxtb	r2, r3
	beq	.L282
	ldr	r2, [fp, #-84]
	mov	r1, #0
	ldr	r0, [r9, #440]
	bl	memset
	mov	r3, #0
	strb	r3, [r9, #449]
	b	.L185
.L94:
	mov	r0, #255
	mvn	r2, #0
	strb	r2, [r3, #12]
.L95:
	mov	r3, #100
	str	r3, [r8]
	b	.L96
.L187:
	mov	r0, #255
	mvn	r2, #0
	strb	r2, [r3, #14]
.L188:
	mov	r3, #100
	str	r3, [r8, #8]
	b	.L189
.L162:
	mov	r0, #255
	mvn	r2, #0
	strb	r2, [r3, #13]
.L163:
	mov	r3, #100
	str	r3, [r8, #4]
	b	.L164
.L282:
	add	r3, r3, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L175
	ldr	r3, [r5, #20]
	mov	r1, #2
	str	r7, [sp]
	ldr	r0, [fp, #-84]
	bl	DetBorderOffsetRuler.isra.1
	ldrsb	r3, [r7, #1]
	ldr	r6, .L291+16
	cmp	r3, #8
	beq	.L176
	ldrb	r3, [r9, #449]
	ldrb	r2, [r9, #451]
	cmp	r3, r2
	addcc	r3, r3, #1
	uxtbcc	r3, r3
	strccb	r3, [r9, #449]
	cmp	r2, r3
	bne	.L178
	ldr	r3, [fp, #-84]
	cmp	r3, #4
	ble	.L224
	add	r3, r3, #1
	str	r4, [fp, #-100]
	str	r3, [fp, #-84]
	mov	r6, #0
	mov	r3, #5
	str	r10, [fp, #-92]
	str	r7, [fp, #-96]
	ldr	r4, [fp, #-88]
	b	.L183
.L180:
	ldr	r2, [fp, #-84]
	add	r3, r3, #1
	cmp	r3, r2
	beq	.L283
.L183:
	ldr	r0, [r4, #24]
	sub	r2, r3, #1
	ldrb	r7, [r0, r2]
	cmp	r7, #1
	bne	.L180
	ldr	r1, [r9, #440]
	sub	r10, r3, #2
	ldrb	lr, [r9, #451]
	add	r6, r6, #1
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L180
	ldrsb	ip, [r1, r10]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L182
	cmp	ip, r1
	blt	.L182
	strb	r7, [r0, r10]
	mov	r0, #0
	ldr	r1, [r4, #24]
	strb	r0, [r1, r2]
	b	.L180
.L280:
	uxtb	r3, r3
	str	r3, [fp, #-96]
	mov	r2, r3
	add	r3, r3, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L107
	cmp	r2, #8
	ldr	r2, [r5, #16]
	mov	r3, #0
	str	r3, [fp, #-76]
	str	r3, [fp, #-72]
	str	r2, [fp, #-92]
	str	r3, [fp, #-68]
	str	r3, [fp, #-64]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-52]
	str	r3, [fp, #-48]
	beq	.L108
	sub	r9, r6, #3
	cmp	r9, #4
	bls	.L216
	ldrb	r3, [r2, #4]
	cmp	r3, #1
	beq	.L216
	add	r3, r2, #4
	mov	r2, #4
	b	.L110
.L120:
	ldrb	r1, [r3, #1]!
	cmp	r1, #1
	beq	.L270
.L110:
	add	r2, r2, #1
	str	r2, [fp, #-100]
	cmp	r2, r9
	bne	.L120
.L270:
	str	r2, [fp, #-108]
.L109:
	ldr	r2, [fp, #-96]
	ldr	r3, [fp, #-100]
	rsb	r3, r2, r3
	cmp	r3, #4
	ldrgt	r2, .L291
	strgt	r2, [fp, #-112]
	ble	.L127
.L126:
	sub	r1, r3, #-1073741823
	ldr	lr, [fp, #-112]
	mov	ip, #0
	add	r1, lr, r1, lsl #2
	mov	r0, ip
	mov	r2, ip
.L125:
	ldr	lr, [r1], #4
	cmp	lr, r0
	movhi	ip, r2
	add	r2, r2, #1
	movhi	r0, lr
	cmp	r2, #3
	bne	.L125
	sub	r3, r3, #1
	ldr	r1, [fp, #-92]
	add	ip, r3, ip
	ldr	r3, [fp, #-96]
	mov	r2, #1
	rsb	r3, r3, ip
	strb	r2, [r1, ip]
	cmp	r3, #4
	bgt	.L126
.L127:
	cmp	r9, #0
	mov	r3, r9
	ble	.L271
	ldr	r2, [fp, #-92]
	ldrb	r1, [r2, r9]
	add	r2, r2, r9
	cmp	r1, #1
	bne	.L130
	b	.L271
.L131:
	ldrb	r1, [r2, #-1]!
	cmp	r1, #1
	beq	.L219
.L130:
	subs	r3, r3, #1
	bne	.L131
.L219:
	str	r3, [fp, #-104]
.L128:
	ldr	r2, [fp, #-96]
	ldr	r3, [fp, #-104]
	add	r3, r3, r2
	cmp	r3, r9
	ldrcc	r2, .L291
	strcc	r2, [fp, #-112]
	bcs	.L138
.L137:
	sub	r1, r3, #-1073741823
	ldr	lr, [fp, #-112]
	mov	ip, #0
	add	r1, lr, r1, lsl #2
	mov	r0, ip
	mov	r2, ip
.L136:
	ldr	lr, [r1], #4
	cmp	lr, r0
	movhi	ip, r2
	add	r2, r2, #1
	movhi	r0, lr
	cmp	r2, #3
	bne	.L136
	sub	r3, r3, #1
	ldr	r1, [fp, #-92]
	add	ip, r3, ip
	ldr	r3, [fp, #-96]
	mov	r2, #1
	add	r3, ip, r3
	strb	r2, [r1, ip]
	cmp	r3, r9
	bcc	.L137
.L138:
	ldr	r3, [fp, #-100]
	ldr	r2, [fp, #-104]
	cmp	r3, r2
	bhi	.L134
	ldr	r3, [fp, #-100]
	str	r4, [fp, #-120]
	str	r6, [fp, #-112]
	mov	r2, r3
	mov	r3, #0
	mov	r9, r3
	mov	r4, r2
	b	.L145
.L141:
	ldr	r3, [fp, #-108]
	ldr	r2, [fp, #-104]
	add	r3, r3, #1
	str	r3, [fp, #-108]
	cmp	r3, r2
	mov	r4, r3
	bhi	.L284
.L145:
	ldr	r3, [fp, #-92]
	ldrb	r3, [r3, r4]
	cmp	r3, #1
	bne	.L141
	ldr	r2, [fp, #-100]
	cmp	r2, r4
	beq	.L220
	ldr	r2, [fp, #-96]
	cmp	r2, #0
	beq	.L141
	mov	r1, r2
	rsb	r0, r9, r4
	str	r3, [fp, #-124]
	bl	__aeabi_uidiv
	cmp	r0, #1
	mov	r6, r0
	bls	.L221
	ldr	r3, [fp, #-124]
	mov	lr, r3
.L144:
	ldr	r3, [fp, #-96]
	mov	ip, #0
	mov	r0, ip
	add	r1, r9, r3
	ldr	r9, .L291
	sub	r2, r1, #-1073741823
	mov	r3, ip
	add	r2, r9, r2, lsl #2
.L143:
	ldr	r9, [r2], #4
	cmp	r9, r0
	movhi	ip, r3
	add	r3, r3, #1
	movhi	r0, r9
	cmp	r3, #3
	bne	.L143
	sub	r1, r1, #1
	add	lr, lr, #1
	ldr	r2, [fp, #-92]
	add	r9, r1, ip
	cmp	lr, r6
	mov	r3, #1
	strb	r3, [r2, r9]
	bne	.L144
.L221:
	mov	r9, r4
	b	.L141
.L281:
	ldr	r3, [fp, #-80]
	cmp	r3, #4
	ble	.L226
	add	r3, r3, #1
	str	r7, [fp, #-84]
	str	r3, [fp, #-80]
	mov	r2, #0
	mov	r3, #5
	ldr	r7, [fp, #-88]
	b	.L208
.L205:
	ldr	r1, [fp, #-80]
	add	r3, r3, #1
	cmp	r3, r1
	beq	.L285
.L208:
	ldr	ip, [r7, #20]
	sub	r1, r3, #1
	ldrb	r6, [ip, r1]
	cmp	r6, #1
	bne	.L205
	ldr	r0, [r9, #444]
	sub	r5, r3, #2
	ldrb	r4, [r9, #451]
	add	r2, r2, #1
	ldrsb	lr, [r0, r1]
	cmp	lr, r4
	bge	.L205
	ldrsb	lr, [r0, r5]
	ldrsb	r0, [r0, r3]
	cmp	r4, lr
	bgt	.L207
	cmp	lr, r0
	blt	.L207
	strb	r6, [ip, r5]
	mov	ip, #0
	ldr	r0, [r7, #20]
	strb	ip, [r0, r1]
	b	.L205
.L200:
	ldr	r3, [fp, #-88]
	mov	r1, #0
	ldr	r5, [fp, #-80]
	mov	r4, r1
	ldr	r0, [r3, #20]
	mov	r2, r5
	bl	memset
	str	r4, [r7, #8]
	mvn	r3, #0
	mov	r2, r5
	strb	r3, [r7, #2]
	mov	r1, r4
	ldr	r0, [r9, #444]
	bl	memset
	strb	r4, [r9, #450]
	str	r4, [r9, #460]
	b	.L210
.L107:
	ldr	r3, [fp, #-88]
	mov	r1, #0
	mov	r2, r6
	str	r1, [fp, #-88]
	add	r9, r7, #8192
	ldr	r0, [r3, #16]
	bl	memset
	ldr	ip, [fp, #-88]
	mvn	r3, #0
	mov	r2, r6
	strb	r3, [r7]
	str	ip, [r7, #4]
	mov	r1, ip
	ldr	r0, [r9, #436]
	bl	memset
	ldr	ip, [fp, #-88]
	strb	ip, [r9, #448]
	str	ip, [r9, #456]
	b	.L160
.L175:
	ldr	r3, [fp, #-88]
	mov	r1, #0
	mov	r6, r1
	ldr	r2, [fp, #-84]
	ldr	r0, [r3, #24]
	bl	memset
	str	r6, [r7, #12]
	mvn	r3, #0
	ldr	r2, [fp, #-84]
	strb	r3, [r7, #1]
	mov	r1, r6
	ldr	r0, [r9, #440]
	bl	memset
	strb	r6, [r9, #449]
	str	r6, [r9, #464]
	b	.L185
.L199:
	ldr	r2, [fp, #-80]
	mov	r1, #0
	ldr	r0, [r9, #444]
	bl	memset
	mov	r3, #0
	strb	r3, [r9, #450]
	b	.L210
.L283:
	sub	r4, fp, #100
	ldmia	r4, {r4, r7, r10}
.L179:
	str	r6, [r9, #464]
.L184:
	ldr	r3, [r8, #4]
	str	r3, [r7, #12]
	b	.L185
.L207:
	cmp	r4, r0
	cmple	lr, r0
	movlt	r0, #1
	strltb	r0, [ip, r3]
	ldrlt	r0, [r7, #20]
	movlt	ip, #0
	strltb	ip, [r0, r1]
	b	.L205
.L284:
	ldr	r6, [fp, #-112]
	ldr	r4, [fp, #-120]
.L134:
	cmp	r6, #4
	addls	r9, r7, #8192
	movls	r3, #0
	bls	.L140
	ldr	r3, [fp, #-92]
	mov	ip, #0
	add	r9, r7, #8192
	str	r4, [fp, #-100]
	add	lr, r3, #3
	sub	r3, r6, #1
	str	r3, [fp, #-96]
	mov	r3, #3
	b	.L150
.L146:
	ldr	r1, [r9, #436]
	ldrb	r2, [r1, r3]
	sub	r0, r2, #1
	sxtb	r2, r2
	cmp	r2, #0
	ble	.L149
.L272:
	strb	r0, [r1, r3]
.L148:
	ldr	r2, [fp, #-96]
	add	r3, r3, #1
	cmp	r3, r2
	beq	.L286
.L150:
	ldrb	r2, [lr, #1]!
	cmp	r2, #1
	bne	.L146
	strb	r2, [lr, #-1]
	mov	r1, #0
	strb	r1, [lr]
	add	ip, ip, #1
	ldr	r1, [r9, #436]
	ldrb	r0, [r9, #452]
	ldrb	r2, [r1, r3]
	add	r4, r2, #1
	sxtb	r2, r2
	cmp	r2, r0
	movlt	r2, r4
	strltb	r2, [r1, r3]
	blt	.L148
	b	.L272
.L149:
	mov	r2, #0
	strb	r2, [r1, r3]
	b	.L148
.L286:
	ldr	r4, [fp, #-100]
	mov	r3, ip
.L140:
	str	r3, [r9, #456]
.L119:
	add	r3, r7, #8640
	ldr	r2, [fp, #-92]
	add	r3, r3, #8
	ldr	r1, [fp, #-116]
	mov	r0, r6
	bl	CheckStartEndBorder
	ldrsb	r3, [r7]
	cmp	r3, #8
	beq	.L151
	ldrb	r3, [r9, #448]
	ldrb	r2, [r9, #451]
	cmp	r3, r2
	addcc	r3, r3, #1
	uxtbcc	r3, r3
	strccb	r3, [r9, #448]
	cmp	r2, r3
	bne	.L151
	cmp	r6, #4
	ble	.L222
	add	r3, r6, #1
	str	r4, [fp, #-104]
	str	r3, [fp, #-92]
	mov	r6, #0
	mov	r3, #5
	str	r10, [fp, #-96]
	str	r7, [fp, #-100]
	ldr	r4, [fp, #-88]
	b	.L158
.L155:
	ldr	r2, [fp, #-92]
	add	r3, r3, #1
	cmp	r3, r2
	beq	.L287
.L158:
	ldr	r0, [r4, #16]
	sub	r2, r3, #1
	ldrb	r7, [r0, r2]
	cmp	r7, #1
	bne	.L155
	ldr	r1, [r9, #436]
	sub	r10, r3, #2
	ldrb	lr, [r9, #451]
	add	r6, r6, #1
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L155
	ldrsb	ip, [r1, r10]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L157
	cmp	ip, r1
	blt	.L157
	strb	r7, [r0, r10]
	mov	r0, #0
	ldr	r1, [r4, #16]
	strb	r0, [r1, r2]
	b	.L155
.L151:
	ldr	r3, [fp, #-88]
	mov	r2, r6
	ldr	r1, [r5, #16]
	ldr	r0, [r3, #16]
	bl	memcpy
.L159:
	ldr	r3, [r8]
	str	r3, [r7, #4]
	b	.L160
.L157:
	cmp	lr, r1
	cmple	ip, r1
	movlt	r1, #1
	strltb	r1, [r0, r3]
	ldrlt	r1, [r4, #16]
	movlt	r0, #0
	strltb	r0, [r1, r2]
	b	.L155
.L178:
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-84]
	ldr	r1, [r5, #20]
	ldr	r0, [r3, #24]
	bl	memcpy
	b	.L184
.L193:
	cmp	r1, #1
	ldrb	r2, [r8, #14]
	ble	.L225
	mov	ip, #1
	sxtb	lr, r2
	mov	r0, ip
	mov	r6, r2
	b	.L197
.L196:
	cmp	ip, r1
	add	r3, r3, #8
	beq	.L288
.L197:
	ldrsb	r2, [r3, #8]
	add	ip, ip, #1
	cmp	r2, lr
	bne	.L196
	ldr	r2, [fp, #-88]
	ldrb	r4, [r2, #51]
	add	r2, r0, #1
	str	r2, [fp, #-84]
	ldr	r2, [r3, #12]
	cmp	r2, r4
	ldrcsb	r0, [fp, #-84]
	b	.L196
.L182:
	cmp	lr, r1
	cmple	ip, r1
	movlt	r1, #1
	strltb	r1, [r0, r3]
	ldrlt	r1, [r4, #24]
	movlt	r0, #0
	strltb	r0, [r1, r2]
	b	.L180
.L108:
	sub	ip, r6, #3
	cmp	ip, #4
	bls	.L111
	ldr	r3, [fp, #-92]
	add	r0, r3, #3
	mov	r3, #4
.L113:
	ldrb	r2, [r0, #1]!
	cmp	r2, #1
	bne	.L112
	and	r2, r3, #7
	sub	r1, fp, #44
	add	r2, r1, r2, lsl #2
	ldr	r1, [r2, #-32]
	add	r1, r1, #1
	str	r1, [r2, #-32]
.L112:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L113
	ldr	r3, [fp, #-76]
.L111:
	mov	r9, #0
	sub	r1, fp, #76
	mov	r0, r9
	mov	r2, r9
	b	.L213
.L115:
	ldr	r3, [r1, #4]!
.L213:
	cmp	r0, r3
	movcc	r9, r2
	add	r2, r2, #1
	movcc	r0, r3
	cmp	r2, #8
	bne	.L115
	mov	r1, #0
	mov	r2, r6
	ldr	r0, [fp, #-92]
	str	ip, [fp, #-96]
	bl	memset
	cmp	r9, #0
	ldr	ip, [fp, #-96]
	ldrne	r3, [fp, #-92]
	movne	r1, #1
	moveq	r3, r9
	addne	r2, r3, r9
	add	r9, r9, #7
	movne	r3, r1
	strneb	r1, [r2, #-1]
	cmp	r9, ip
	bcs	.L117
	mov	r2, #1
.L118:
	ldr	r1, [fp, #-92]
	add	r3, r3, #1
	strb	r2, [r1, r9]
	add	r9, r9, #8
	cmp	r9, ip
	bcc	.L118
.L117:
	add	r9, r7, #8192
	str	r3, [r9, #456]
	b	.L119
.L220:
	ldr	r9, [fp, #-100]
	b	.L141
.L100:
	cmp	r1, #1
	ldrb	lr, [r8, #12]
	ble	.L214
	mov	r2, #1
	sxtb	r0, lr
	mov	r9, r6
	str	r0, [fp, #-96]
	str	r1, [fp, #-100]
	mov	r0, r2
	b	.L104
.L103:
	ldr	r1, [fp, #-100]
	add	r3, r3, #8
	cmp	ip, r1
	bge	.L289
.L104:
	ldrsb	ip, [r3, #8]
	add	r2, r2, #1
	ldr	r6, [fp, #-96]
	uxtb	r2, r2
	cmp	ip, r6
	sxtb	ip, r2
	bne	.L103
	ldr	r6, [fp, #-88]
	add	r1, r0, #1
	str	r1, [fp, #-92]
	ldr	r1, [r3, #12]
	ldrb	r6, [r6, #49]
	cmp	r1, r6
	ldrcsb	r0, [fp, #-92]
	b	.L103
.L287:
	sub	r4, fp, #104
	ldmia	r4, {r4, r7, r10}
.L154:
	str	r6, [r9, #456]
	b	.L159
.L285:
	ldr	r7, [fp, #-84]
.L204:
	str	r2, [r9, #460]
	b	.L209
.L168:
	cmp	r1, #1
	ldrb	r2, [r8, #13]
	ble	.L223
	mov	ip, #1
	sxtb	r0, r2
	mov	r6, r2
	str	r0, [fp, #-96]
	mov	r0, ip
	b	.L172
.L171:
	cmp	ip, r1
	add	r3, r3, #8
	beq	.L290
.L172:
	ldrsb	r2, [r3, #8]
	add	ip, ip, #1
	ldr	lr, [fp, #-96]
	cmp	r2, lr
	bne	.L171
	ldr	r2, [fp, #-88]
	ldrb	lr, [r2, #50]
	add	r2, r0, #1
	str	r2, [fp, #-92]
	ldr	r2, [r3, #12]
	cmp	r2, lr
	ldrcsb	r0, [fp, #-92]
	b	.L171
.L288:
	mov	r2, r6
.L195:
	ldr	r3, [fp, #-88]
	ldrb	r3, [r3, #48]
	cmp	r3, r0
	bhi	.L198
	strb	r2, [r7, #2]
	mov	r3, r2
	ldrb	r2, [r8, #14]
	b	.L194
.L292:
	.align	2
.L291:
	.word	.LANCHOR0
	.word	.LANCHOR0+16384
	.word	.LANCHOR0+4
	.word	.LANCHOR0+16399
	.word	.LANCHOR0+20480
.L290:
	mov	r2, r6
.L170:
	ldr	r3, [fp, #-88]
	ldrb	r3, [r3, #48]
	cmp	r3, r0
	bhi	.L173
	strb	r2, [r7, #1]
	mov	r3, r2
	ldrb	r2, [r8, #13]
	b	.L169
.L289:
	mov	r6, r9
.L102:
	ldr	r3, [fp, #-88]
	ldrb	r3, [r3, #48]
	cmp	r3, r0
	bhi	.L105
	strb	lr, [r7]
	mov	r3, lr
	ldrb	r2, [r8, #12]
	b	.L101
.L216:
	mov	r3, #4
	str	r3, [fp, #-108]
	str	r3, [fp, #-100]
	b	.L109
.L225:
	mov	r0, #1
	b	.L195
.L226:
	mov	r2, #0
	b	.L204
.L223:
	mov	r0, #1
	b	.L170
.L201:
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-80]
	ldr	r1, [r4, #24]
	ldr	r0, [r3, #20]
	bl	memcpy
	b	.L209
.L214:
	mov	r0, #1
	b	.L102
.L222:
	mov	r6, #0
	b	.L154
.L176:
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-84]
	ldr	r1, [r6, #20]
	ldr	r0, [r3, #24]
	bl	memcpy
	b	.L184
.L271:
	str	r9, [fp, #-104]
	b	.L128
.L224:
	mov	r6, #0
	b	.L179
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_GetDbDetInfo, .-PQ_MNG_ALG_GetDbDetInfo
	.align	2
	.global	PQ_MNG_ALG_DBCfgDetector
	.type	PQ_MNG_ALG_DBCfgDetector, %function
PQ_MNG_ALG_DBCfgDetector:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r5, r0
	ldr	r0, [r1, #48]
	mov	r4, r2
	cmp	r0, #8
	ldmia	r1, {r0, r2, ip}
	strneh	r3, [r5, #20]
	beq	.L308
.L295:
	ldr	r3, [r1, #64]
	cmp	r3, #8
	ldrneh	r3, [fp, #4]
	strneh	r3, [r5, #22]
	beq	.L309
.L297:
	movw	r3, #1080
	cmp	r0, #1920
	cmple	r2, r3
	mov	r1, #0
	mov	r3, #1
	add	r0, r5, #36
	strh	r3, [r5, #16]
	strh	r3, [r5, #18]
	mov	r2, #512
	movgt	r3, #0
	add	r4, r4, #16
	strgth	r3, [r5, #16]
	strgth	r3, [r5, #18]
	bl	memset
	add	ip, r5, #32
	mov	lr, #0
	movw	r5, #3959
.L299:
	cmp	lr, #3968
	ldr	r2, [ip, #4]!
	beq	.L305
	mov	r0, r4
	mov	r3, #1
	ldrb	r1, [r0, lr]!
	and	r1, r1, r3
	orr	r1, r1, r2
	str	r1, [ip]
	b	.L301
.L311:
	ldrb	r2, [r0, #1]!
	and	r2, r2, #1
	orr	r1, r1, r2, asl r3
	add	r3, r3, #1
	cmp	r3, #32
	str	r1, [ip]
	beq	.L310
.L301:
	add	r2, r3, lr
	cmp	r2, r5
	bls	.L311
.L305:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L310:
	add	lr, lr, #32
	b	.L299
.L308:
	cmp	r0, #0
	add	r3, r0, #7
	movge	r3, r0
	mov	r3, r3, asr #3
	strh	r3, [r5, #20]
	b	.L295
.L309:
	add	r3, ip, #7
	cmp	ip, #0
	movlt	ip, r3
	mov	r3, ip, asr #3
	strh	r3, [r5, #22]
	b	.L297
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_DBCfgDetector, .-PQ_MNG_ALG_DBCfgDetector
	.global	__aeabi_idiv
	.align	2
	.global	PQ_MNG_ALG_UpdateDMCfg
	.type	PQ_MNG_ALG_UpdateDMCfg, %function
PQ_MNG_ALG_UpdateDMCfg:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	ldr	lr, .L340
	mov	r5, r0
	mov	r4, r1
	cmp	r0, #0
	sub	ip, fp, #100
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldmia	lr, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	beq	.L325
	ldr	r6, [r5]
	ldr	r0, [r5, #144]
	cmp	r6, #0
	ldr	r3, [r5, #148]
	bne	.L339
	mov	r0, r6
.L314:
	ldr	r6, .L340+4
	mov	r3, #8
	mov	r2, #4
	strb	r3, [r4, #8]
	strb	r2, [r4, #7]
	strb	r3, [r4, #5]
.L316:
	ldr	r2, .L340+8
	sub	r3, fp, #36
	add	r0, r3, r0, lsl #2
	ldr	r3, [r2, #-4044]
	ldrh	ip, [r0, #-64]
	cmp	r3, #31
	movhi	r1, #1
	movhi	r3, #0
	addls	r1, r3, #1
.L317:
	add	r3, r3, #2
	str	r1, [r2, #-4044]
	add	r3, r5, r3, lsl #2
	str	ip, [r3, #4]!
	ldr	r1, [r5, #140]
	str	r3, [r5, #152]
	cmp	r1, #0
	beq	.L323
	ble	.L328
	mov	r2, #0
	add	lr, r5, #12
	mov	r0, r2
.L321:
	ldr	ip, [r3], #-4
	add	r2, r2, #1
	cmp	r3, lr
	add	r0, r0, ip
	addcc	r3, r5, #136
	cmp	r2, r1
	bne	.L321
.L319:
	bl	__aeabi_idiv
	mov	ip, r0
.L323:
	uxth	r2, ip
	mov	r0, #1
	rsb	r3, r2, #8
	mov	r1, r2, asl #2
	mov	lr, r2, lsr r0
	mov	r8, r3, asl #6
	add	r2, r1, r2
	sub	r8, r8, r3, asl #2
	add	r3, r3, r2
	adds	r7, r8, #4
	rsb	r5, lr, #4
	addmi	r7, r8, #11
	adds	r2, r1, #4
	addmi	r1, r1, #11
	movpl	r1, r2
	adds	r2, r3, #4
	addmi	r3, r3, #11
	movpl	r3, r2
	cmp	r5, #2
	mov	r7, r7, asr #3
	mov	r2, r1, asr #3
	mov	r3, r3, asr #3
	mov	r0, r0, asl lr
	strb	r7, [r4]
	movle	r5, #2
	strb	r2, [r4, #2]
	strb	r3, [r4, #3]
	strb	r0, [r4, #1]
	ble	.L322
	cmp	r5, #4
	movge	r5, #4
	uxtb	r5, r5
.L322:
	mov	r0, #0
	strb	r5, [r4, #4]
	strb	ip, [r4, #6]
	str	r6, [r4, #12]
.L335:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L339:
	mul	r0, r3, r0
	mov	r1, r6
	mov	r0, r0, asl #4
	bl	__aeabi_idiv
	cmp	r6, #720
	usat	r0, #4, r0
	bls	.L314
	mov	r3, #12
	mov	r2, #0
	strb	r3, [r4, #8]
	mov	r3, #10
	ldr	r6, .L340+12
	strb	r2, [r4, #7]
	strb	r3, [r4, #5]
	b	.L316
.L328:
	mov	r0, #0
	b	.L319
.L325:
	mvn	r0, #0
	b	.L335
.L341:
	.align	2
.L340:
	.word	.LANCHOR2
	.word	.LANCHOR1
	.word	.LANCHOR3
	.word	.LANCHOR1+16
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_UpdateDMCfg, .-PQ_MNG_ALG_UpdateDMCfg
	.global	DM_DIR_STR_LUT_HD
	.global	DM_DIR_STR_LUT
	.section	.rodata
	.align	2
.LANCHOR2 = . + 0
.LC0:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	5
	.word	6
	.word	6
	.word	7
	.word	8
	.word	8
	.word	8
	.word	8
	.word	8
	.word	8
	.data
	.align	2
.LANCHOR1 = . + 0
	.type	DM_DIR_STR_LUT, %object
	.size	DM_DIR_STR_LUT, 16
DM_DIR_STR_LUT:
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	6
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.type	DM_DIR_STR_LUT_HD, %object
	.size	DM_DIR_STR_LUT_HD, 16
DM_DIR_STR_LUT_HD:
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	6
	.byte	4
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.bss
	.align	2
.LANCHOR0 = . + 0
.LANCHOR3 = . + 24552
	.type	stDetStaticInfo, %object
	.size	stDetStaticInfo, 20508
stDetStaticInfo:
	.space	20508
	.type	i.4623, %object
	.size	i.4623, 4
i.4623:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
