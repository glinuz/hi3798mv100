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
	.file	"pq_mng_db_alg.c"
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
	.type	FillBorder_Size8, %function
FillBorder_Size8:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	sub	lr, r2, #3
	cmp	lr, #4
	mov	r4, r3
	mov	r5, r2
	mov	r3, #0
	mov	r6, r0
	mov	r7, r1
	str	r3, [r4]
	str	r3, [fp, #-68]
	str	r3, [fp, #-64]
	str	r3, [fp, #-60]
	str	r3, [fp, #-56]
	str	r3, [fp, #-52]
	str	r3, [fp, #-48]
	str	r3, [fp, #-44]
	str	r3, [fp, #-40]
	bls	.L13
	add	r3, r0, #3
	mov	ip, #4
	b	.L15
.L14:
	add	ip, ip, #1
	cmp	ip, lr
	beq	.L29
.L15:
	ldrb	r2, [r3, #1]!
	cmp	r2, #1
	bne	.L14
	and	r2, ip, #7
	sub	r1, fp, #36
	add	ip, ip, #1
	add	r2, r1, r2, lsl #2
	cmp	ip, lr
	ldr	r1, [r2, #-32]
	add	r1, r1, #1
	str	r1, [r2, #-32]
	bne	.L15
.L29:
	ldr	r3, [fp, #-68]
.L13:
	mov	r8, #0
	sub	r1, fp, #68
	mov	r0, r8
	mov	r2, r8
	b	.L22
.L17:
	ldr	r3, [r1, #4]!
.L22:
	cmp	r3, r0
	movhi	r8, r2
	add	r2, r2, #1
	movhi	r0, r3
	cmp	r2, #8
	bne	.L17
	mov	r2, r5
	mov	r1, #0
	mov	r0, r6
	bl	memset
	cmp	r8, #0
	bne	.L30
.L18:
	sub	ip, r7, #1
	add	ip, ip, r8
	cmp	r5, ip
	bls	.L12
	mov	r3, #1
.L20:
	strb	r3, [r6, ip]
	add	ip, ip, r7
	ldr	lr, [r4]
	cmp	r5, ip
	add	lr, lr, #1
	str	lr, [r4]
	bhi	.L20
.L12:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L30:
	add	r3, r6, r8
	mov	r2, #1
	strb	r2, [r3, #-1]
	ldr	r3, [r4]
	add	r3, r3, r2
	str	r3, [r4]
	b	.L18
	UNWIND(.fnend)
	.size	FillBorder_Size8, .-FillBorder_Size8
	.global	__aeabi_uidiv
	.align	2
	.type	FillBorderHchrVy.constprop.12, %function
FillBorderHchrVy.constprop.12:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	cmp	r2, #1
	add	r2, r0, #8192
	str	r1, [fp, #-48]
	add	ip, r0, #4096
	ldr	r3, .L79
	ldrb	r1, [r2, #452]
	add	r4, r0, #8640
	ldr	lr, [r2, #444]
	addeq	r4, r4, #12
	ldreqb	r5, [r3, #-4082]
	addne	r4, r4, #16
	str	r1, [fp, #-60]
	ldr	r1, [r2, #440]
	ldr	r2, [ip, #24]
	ldrneb	r5, [r3, #-4083]
	str	r1, [fp, #-56]
	str	r2, [fp, #-52]
	ldr	r1, [ip, #20]
	sxtb	r5, r5
	ldr	r2, [r3, #24]
	ldr	r9, [r3, #20]
	streq	lr, [fp, #-56]
	streq	r1, [fp, #-52]
	moveq	r9, r2
	cmp	r5, #8
	beq	.L78
	ldr	r3, [fp, #-48]
	sub	r0, r3, #3
	mov	r3, #0
	cmp	r0, #4
	str	r3, [r4]
	bls	.L64
	ldrb	r3, [r9, #4]
	cmp	r3, #1
	beq	.L64
	add	r2, r9, #4
	mov	r8, r0
	mov	r3, #4
	b	.L37
.L38:
	ldrb	r1, [r2, #1]!
	cmp	r1, #1
	beq	.L65
.L37:
	add	r10, r3, #1
	cmp	r10, r0
	mov	r3, r10
	bne	.L38
.L36:
	rsb	lr, r5, r10
	cmp	lr, #4
	ble	.L43
	rsb	r1, r5, lr
	rsb	r7, r5, #0
	add	lr, r9, lr
	mov	r2, #0
	mov	r6, #1
.L42:
	add	r1, r1, r7
	strb	r6, [lr, r2]
	add	ip, r5, r1
	ldr	r3, [r4]
	cmp	ip, #4
	rsb	r2, r5, r2
	add	r3, r3, #1
	str	r3, [r4]
	bgt	.L42
.L43:
	cmp	r0, #0
	mov	r7, r0
	ble	.L75
	ldrb	r2, [r9, r0]
	add	r3, r9, r0
	cmp	r2, #1
	bne	.L46
	b	.L75
.L47:
	ldrb	r2, [r3, #-1]!
	cmp	r2, #1
	beq	.L44
.L46:
	subs	r7, r7, #1
	bne	.L47
.L44:
	add	r3, r7, r5
	cmp	r3, r0
	movcc	r1, #1
	bcs	.L52
.L51:
	strb	r1, [r9, r3]
	add	r3, r3, r5
	ldr	r2, [r4]
	cmp	r3, r0
	add	r2, r2, #1
	str	r2, [r4]
	bcc	.L51
.L52:
	cmp	r7, r10
	bcc	.L50
	mov	r2, r10
	mov	r3, #0
	mov	r10, r7
	mov	ip, #1
	mov	r7, r2
	b	.L57
.L55:
	add	r8, r8, #1
	cmp	r10, r8
	mov	r7, r8
	bcc	.L50
.L57:
	ldrb	r6, [r9, r7]
	cmp	r6, #1
	bne	.L55
	ldr	r1, [r4]
	cmp	r5, #0
	cmpne	r2, r7
	add	r1, r1, #1
	str	r1, [r4]
	beq	.L68
	mov	r1, r5
	rsb	r0, r3, r7
	str	r2, [fp, #-68]
	str	ip, [fp, #-64]
	str	r3, [fp, #-72]
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-64]
	ldr	r2, [fp, #-68]
	cmp	r0, #1
	bls	.L68
	ldr	r3, [fp, #-72]
	mov	lr, r6
	add	r3, r5, r3
.L56:
	add	lr, lr, #1
	strb	ip, [r9, r3]
	cmp	lr, r0
	ldr	r1, [r4]
	add	r3, r3, r5
	add	r1, r1, #1
	str	r1, [r4]
	bne	.L56
.L68:
	add	r8, r8, #1
	mov	r3, r7
	cmp	r10, r8
	mov	r7, r8
	bcs	.L57
.L50:
	ldr	r3, [fp, #-48]
	cmp	r3, #4
	bls	.L35
	ldr	r3, [fp, #-48]
	add	r1, r9, #3
	ldr	r2, [fp, #-56]
	mov	ip, #0
	sub	r0, r3, #1
	ldr	lr, [fp, #-60]
	add	r3, r2, #3
	add	r0, r2, r0
	b	.L62
.L58:
	ldrb	r2, [r3]
	sub	r6, r2, #1
	sxtb	r2, r2
	cmp	r2, #0
	strleb	ip, [r3]
	ble	.L60
.L76:
	strb	r6, [r3]
.L60:
	add	r3, r3, #1
	cmp	r3, r0
	beq	.L35
.L62:
	ldrb	r2, [r1, #1]!
	cmp	r2, #1
	bne	.L58
	strb	ip, [r1]
	strb	r2, [r1, #-1]
	ldrb	r2, [r3]
	add	r6, r2, #1
	sxtb	r2, r2
	cmp	r2, lr
	strgeb	lr, [r3]
	blt	.L76
	add	r3, r3, #1
	cmp	r3, r0
	bne	.L62
.L35:
	mov	r3, r4
	ldr	r4, [fp, #-48]
	mov	r1, r5
	mov	r2, r9
	mov	r0, r4
	bl	CheckStartEndBorder
	mov	r2, r4
	mov	r1, r9
	ldr	r0, [fp, #-52]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	memcpy
.L65:
	mov	r8, r10
	b	.L36
.L78:
	mov	r3, r4
	ldr	r2, [fp, #-48]
	mov	r1, r5
	mov	r0, r9
	bl	FillBorder_Size8
	b	.L35
.L64:
	mov	r8, #4
	mov	r10, r8
	b	.L36
.L75:
	mov	r7, r0
	b	.L44
.L80:
	.align	2
.L79:
	.word	.LANCHOR0+20480
	UNWIND(.fnend)
	.size	FillBorderHchrVy.constprop.12, .-FillBorderHchrVy.constprop.12
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
	UNWIND(.pad #60)
	sub	sp, sp, #60
	mov	r5, r2
	mov	r10, r0
	movw	r2, #20508
	mov	r6, r1
	ldr	r0, .L283
	mov	r1, #0
	ldr	r7, .L283+4
	bl	memset
	mov	r1, r5
	movw	r2, #8808
	mov	r0, r10
	bl	memcpy
	ldr	r2, [r5, #48]
	ldr	r0, .L283
	add	r3, r2, #1
	strb	r2, [r7, #12]
	ldr	r2, [r5, #64]
	uxtb	r3, r3
	cmp	r3, #64
	strb	r2, [r7, #13]
	add	r2, r2, #1
	ldr	r3, [r5, #56]
	uxtb	r2, r2
	strb	r3, [r7, #14]
	add	r3, r3, #1
	ldr	r1, [r5, #44]
	uxtb	r3, r3
	str	r1, [r7]
	ldr	r1, [r5, #60]
	str	r1, [r7, #4]
	ldr	r1, [r5, #52]
	str	r1, [r7, #8]
	mvnhi	r1, #0
	strhib	r1, [r7, #12]
	cmp	r2, #64
	mov	r1, #0
	mvnhi	r2, #0
	strhib	r2, [r7, #13]
	cmp	r3, #64
	mov	r2, #16384
	mvnhi	r3, #0
	strhib	r3, [r7, #14]
	bl	memset
	ldr	r2, .L283+8
	add	r1, r5, #576
	add	ip, r2, #16384
.L85:
	ldr	r3, [r1, #4]!
	ubfx	r0, r3, #0, #11
	ubfx	r3, r3, #16, #11
	str	r0, [r2, #-4]
	str	r3, [r2], #8
	cmp	r2, ip
	bne	.L85
	mov	r2, #4096
	mov	r1, #0
	ldr	r0, .L283+12
	movw	r8, #4097
	ldr	r4, .L283
	bl	memset
	add	ip, r5, #64
	mov	lr, #1
.L86:
	add	r3, lr, #16384
	ldr	r2, [ip, #4]!
	add	r3, r3, #14
	mov	r1, lr
	and	r2, r2, #1
	mov	r0, #1
	strb	r2, [r4, r3]
	b	.L88
.L265:
	mov	r2, r2, lsr r0
	add	r0, r0, #1
	cmp	r0, #32
	and	r2, r2, #1
	add	r1, r1, #1
	strb	r2, [r3]
	beq	.L264
.L88:
	add	r3, r4, r1
	cmp	r1, #4096
	add	r3, r3, #16384
	ldr	r2, [ip]
	add	r3, r3, #15
	bcc	.L265
.L87:
	ldr	r0, .L283+12
	movw	r1, #3836
	ldr	ip, .L283+16
	add	r2, r0, #1920
	add	r3, r0, #2880
	str	r0, [ip, #16]
	str	r2, [ip, #20]
	str	r3, [ip, #24]
	ldr	r3, [r5]
	ldr	r2, [r5, #8]
	sub	r0, r3, #4
	cmp	r0, r1
	str	r3, [fp, #-48]
	ldr	r3, [r5, #4]
	bhi	.L90
	sub	r1, r3, #1
	cmp	r1, #2160
	bcs	.L90
.L91:
	ldr	r0, [fp, #-48]
	movw	r1, #1080
	cmp	r0, #1920
	cmple	r3, r1
	movgt	r3, #0
	strgt	r3, [r5, #36]
	strgt	r3, [r5, #40]
	cmp	r2, #960
	movgt	r3, #0
	strgt	r3, [r5, #36]
	ldr	r3, [r5, #32]
	cmp	r3, #1
	beq	.L266
.L94:
	add	r8, r6, #8192
	mvn	r3, #0
	strb	r3, [r6]
.L108:
	ldr	r3, [r5, #36]
	cmp	r3, #1
	beq	.L267
.L154:
	mvn	r3, #0
	strb	r3, [r6, #1]
.L168:
	ldr	r3, [r5, #40]
	cmp	r3, #1
	beq	.L268
.L179:
	mvn	r3, #0
	strb	r3, [r6, #2]
.L193:
	ldr	r3, [r6, #4]
	mov	r0, #0
	str	r3, [r10, #44]
	ldrsb	r3, [r6]
	str	r3, [r10, #48]
	ldr	r3, [r6, #8]
	str	r3, [r10, #52]
	ldrsb	r3, [r6, #2]
	str	r3, [r10, #56]
	ldr	r3, [r6, #12]
	str	r3, [r10, #60]
	ldrsb	r3, [r6, #1]
	str	r3, [r10, #64]
	ldr	r3, [r8, #456]
	str	r3, [r10, #16]
	ldr	r3, [r8, #460]
	str	r3, [r10, #20]
	ldr	r3, [r8, #464]
	str	r3, [r10, #24]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L90:
	mov	r1, #0
	str	r1, [r5, #28]
	b	.L91
.L264:
	add	lr, lr, #32
	cmp	lr, r8
	bne	.L86
	b	.L87
.L266:
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne	.L94
	add	r9, r6, #4096
	ldr	r1, [r7]
	ldr	r3, .L283+4
	ldrb	r2, [r9, #30]
	cmp	r1, r2
	bcc	.L95
	ldrb	r0, [r3, #12]
	add	r3, r0, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L96
.L97:
	ldrb	r1, [r9, #28]
	add	r3, r6, #4096
	add	r3, r3, #52
	sub	r2, r1, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L98
.L99:
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
	bne	.L99
	ldrb	r0, [r7, #12]
	ldrb	r1, [r9, #28]
.L98:
	ldr	r2, [r9, #36]
	strb	r0, [r9, #52]
	cmp	r2, r1
	ldr	r0, [r7]
	addcc	r2, r2, #1
	strcc	r2, [r9, #36]
	cmp	r1, r2
	str	r0, [r9, #56]
	beq	.L101
.L106:
	ldrb	r2, [r6]
	ldrb	r3, [r7, #12]
.L102:
	cmp	r3, r2
	sxtb	r3, r2
	str	r3, [fp, #-56]
	bne	.L269
	add	r3, r2, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L270
	ldr	r3, [fp, #-56]
	add	r8, r6, #8192
	cmp	r3, #8
	add	r3, r6, #8640
	mov	r2, r3
	ldr	r3, .L283+16
	add	r2, r2, #8
	str	r2, [fp, #-72]
	ldr	r3, [r3, #16]
	str	r3, [fp, #-52]
	mov	r3, #0
	str	r3, [r8, #456]
	ldrb	r3, [r8, #452]
	str	r3, [fp, #-76]
	beq	.L271
	ldr	r3, [fp, #-48]
	sub	lr, r3, #3
	ldr	r3, [r8, #436]
	cmp	lr, #4
	str	r3, [fp, #-80]
	bls	.L206
	ldr	r2, [fp, #-52]
	ldrb	r3, [r2, #4]
	cmp	r3, #1
	beq	.L206
	add	r3, r2, #4
	mov	r2, #4
	b	.L113
.L114:
	ldrb	r1, [r3, #1]!
	cmp	r1, #1
	beq	.L260
.L113:
	add	r2, r2, #1
	str	r2, [fp, #-60]
	cmp	r2, lr
	bne	.L114
.L260:
	str	r2, [fp, #-68]
.L112:
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-56]
	rsb	r3, r2, r3
	cmp	r3, #4
	strgt	lr, [fp, #-64]
	ble	.L121
.L120:
	sub	r2, r3, #-1073741823
	mov	ip, #0
	mov	lr, ip
	mov	r1, ip
	add	r2, r4, r2, lsl #2
.L119:
	ldr	r0, [r2], #4
	cmp	r0, lr
	movhi	ip, r1
	add	r1, r1, #1
	movhi	lr, r0
	cmp	r1, #3
	bne	.L119
	sub	r3, r3, #1
	ldr	r1, [fp, #-52]
	add	ip, r3, ip
	ldr	r3, [fp, #-56]
	mov	r2, #1
	rsb	r3, r3, ip
	strb	r2, [r1, ip]
	cmp	r3, #4
	ldr	r2, [r8, #456]
	add	r2, r2, #1
	str	r2, [r8, #456]
	bgt	.L120
	ldr	lr, [fp, #-64]
.L121:
	cmp	lr, #0
	mov	r3, lr
	ble	.L261
	ldr	r2, [fp, #-52]
	ldrb	r1, [r2, lr]
	add	r2, r2, lr
	cmp	r1, #1
	bne	.L124
	b	.L261
.L125:
	ldrb	r1, [r2, #-1]!
	cmp	r1, #1
	beq	.L207
.L124:
	subs	r3, r3, #1
	bne	.L125
.L207:
	str	r3, [fp, #-64]
.L122:
	ldr	r3, [fp, #-64]
	ldr	r2, [fp, #-56]
	add	r3, r3, r2
	cmp	r3, lr
	strcc	r10, [fp, #-84]
	bcs	.L132
.L131:
	sub	r2, r3, #-1073741823
	mov	ip, #0
	mov	r10, ip
	mov	r1, ip
	add	r2, r4, r2, lsl #2
.L130:
	ldr	r0, [r2], #4
	cmp	r0, r10
	movhi	ip, r1
	add	r1, r1, #1
	movhi	r10, r0
	cmp	r1, #3
	bne	.L130
	sub	r3, r3, #1
	ldr	r1, [fp, #-52]
	add	ip, r3, ip
	ldr	r3, [fp, #-56]
	mov	r2, #1
	add	r3, ip, r3
	strb	r2, [r1, ip]
	cmp	r3, lr
	ldr	r2, [r8, #456]
	add	r2, r2, #1
	str	r2, [r8, #456]
	bcc	.L131
	ldr	r10, [fp, #-84]
.L132:
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-64]
	cmp	r3, r2
	bhi	.L128
	ldr	ip, [fp, #-60]
	mov	r2, #0
	str	r10, [fp, #-84]
	mov	r10, r2
	str	r6, [fp, #-88]
	str	r5, [fp, #-92]
	mov	r6, ip
	b	.L139
.L135:
	ldr	r3, [fp, #-68]
	ldr	r2, [fp, #-64]
	add	r3, r3, #1
	str	r3, [fp, #-68]
	cmp	r3, r2
	mov	r6, r3
	bhi	.L272
.L139:
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3, r6]
	cmp	r3, #1
	bne	.L135
	ldr	r2, [fp, #-60]
	str	r3, [fp, #-96]
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	cmpne	r2, r6
	ldr	r2, [r8, #456]
	add	r2, r2, #1
	str	r2, [r8, #456]
	beq	.L209
	mov	r1, r3
	rsb	r0, r10, r6
	bl	__aeabi_uidiv
	cmp	r0, #1
	mov	r5, r0
	bls	.L209
	ldr	r3, [fp, #-96]
	mov	r0, r3
.L138:
	ldr	r3, [fp, #-56]
	add	r3, r10, r3
	mov	r10, #0
	sub	r1, r3, #-1073741823
	mov	ip, r10
	mov	r2, r10
	add	r1, r4, r1, lsl #2
.L137:
	ldr	lr, [r1], #4
	cmp	lr, ip
	movhi	r10, r2
	add	r2, r2, #1
	movhi	ip, lr
	cmp	r2, #3
	bne	.L137
	sub	r3, r3, #1
	ldr	r2, [fp, #-52]
	add	r10, r3, r10
	add	r0, r0, #1
	mov	r3, #1
	cmp	r0, r5
	strb	r3, [r2, r10]
	ldr	r3, [r8, #456]
	add	r3, r3, #1
	str	r3, [r8, #456]
	bne	.L138
.L209:
	mov	r10, r6
	b	.L135
.L268:
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne	.L179
	add	r9, r6, #4096
	ldr	r1, [r7, #8]
	ldr	r5, [r5, #4]
	ldrb	r2, [r9, #32]
	ldr	r3, .L283+4
	cmp	r1, r2
	bcc	.L180
	ldrb	r1, [r3, #14]
	add	r3, r1, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L181
.L182:
	ldrb	r0, [r9, #28]
	add	r3, r6, #4224
	add	r3, r3, #52
	sub	r2, r0, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L183
.L184:
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
	bne	.L184
	ldrb	r1, [r7, #14]
	ldrb	r0, [r9, #28]
.L183:
	ldr	r2, [r9, #44]
	strb	r1, [r9, #180]
	cmp	r2, r0
	ldr	r1, [r7, #8]
	addcc	r2, r2, #1
	strcc	r2, [r9, #44]
	cmp	r0, r2
	str	r1, [r9, #184]
	beq	.L186
.L191:
	ldrb	r2, [r6, #2]
	ldrb	r3, [r7, #14]
.L187:
	cmp	r3, r2
	bne	.L273
	add	r2, r2, #1
	uxtb	r3, r2
	cmp	r3, #1
	bls	.L274
	mov	r2, #1
	mov	r1, r5
	mov	r0, r6
	bl	FillBorderHchrVy.constprop.12
	ldrsb	r3, [r6, #2]
	cmp	r3, #8
	beq	.L196
	ldrb	r3, [r8, #450]
	ldrb	r2, [r8, #451]
	cmp	r2, r3
	addhi	r3, r3, #1
	strhib	r3, [r8, #450]
	bhi	.L196
	cmp	r5, #4
	strhi	r6, [fp, #-48]
	movhi	r3, #5
	bhi	.L203
	b	.L196
.L219:
	mov	r4, r3
.L200:
	cmp	r5, r4
	add	r3, r3, #1
	bls	.L275
.L203:
	ldr	r0, [r9, #20]
	sub	r2, r3, #1
	ldrb	r6, [r0, r2]
	cmp	r6, #1
	bne	.L219
	ldr	r1, [r8, #444]
	ldrb	lr, [r8, #451]
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L219
	sub	r4, r3, #2
	ldrsb	ip, [r1, r4]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L220
	cmp	ip, r1
	blt	.L220
	strb	r6, [r0, r4]
	mov	r0, #0
	ldr	r1, [r9, #20]
	mov	r4, r3
	strb	r0, [r1, r2]
	b	.L200
.L267:
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne	.L154
	add	r9, r6, #4096
	ldr	r1, [r7, #4]
	ldr	r3, [r5, #8]
	ldrb	r2, [r9, #31]
	cmp	r1, r2
	str	r3, [fp, #-48]
	ldr	r3, .L283+4
	bcc	.L155
	ldrb	r0, [r3, #13]
	add	r3, r0, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L156
.L157:
	ldrb	r1, [r9, #28]
	add	r3, r6, #4352
	add	r3, r3, #52
	sub	r2, r1, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L158
.L159:
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
	bne	.L159
	ldrb	r0, [r7, #13]
	ldrb	r1, [r9, #28]
.L158:
	ldr	r2, [r9, #40]
	strb	r0, [r9, #308]
	cmp	r2, r1
	ldr	r0, [r7, #4]
	addcc	r2, r2, #1
	strcc	r2, [r9, #40]
	cmp	r1, r2
	str	r0, [r9, #312]
	beq	.L161
.L166:
	ldrb	r3, [r6, #1]
	ldrb	r2, [r7, #13]
.L162:
	cmp	r2, r3
	bne	.L276
	add	r3, r3, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L277
	ldr	r4, [fp, #-48]
	mov	r2, #2
	mov	r0, r6
	mov	r1, r4
	bl	FillBorderHchrVy.constprop.12
	ldrsb	r3, [r6, #1]
	cmp	r3, #8
	beq	.L171
	ldrb	r3, [r8, #449]
	ldrb	r2, [r8, #451]
	cmp	r2, r3
	addhi	r3, r3, #1
	strhib	r3, [r8, #449]
	bhi	.L171
	cmp	r4, #4
	mov	r3, r4
	strhi	r5, [fp, #-52]
	movhi	r3, #5
	bhi	.L178
	b	.L171
.L215:
	mov	r4, r3
.L175:
	ldr	r2, [fp, #-48]
	add	r3, r3, #1
	cmp	r2, r4
	bls	.L278
.L178:
	ldr	r0, [r9, #24]
	sub	r2, r3, #1
	ldrb	r4, [r0, r2]
	cmp	r4, #1
	bne	.L215
	ldr	r1, [r8, #440]
	ldrb	lr, [r8, #451]
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L215
	sub	r5, r3, #2
	ldrsb	ip, [r1, r5]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L216
	cmp	ip, r1
	blt	.L216
	strb	r4, [r0, r5]
	mov	r0, #0
	ldr	r1, [r9, #24]
	mov	r4, r3
	strb	r0, [r1, r2]
	b	.L175
.L95:
	mov	r0, #255
	mvn	r2, #0
	strb	r2, [r3, #12]
.L96:
	mov	r3, #100
	str	r3, [r7]
	b	.L97
.L155:
	mov	r0, #255
	mvn	r2, #0
	strb	r2, [r3, #13]
.L156:
	mov	r3, #100
	str	r3, [r7, #4]
	b	.L157
.L216:
	cmp	lr, r1
	cmple	ip, r1
	mov	r4, r3
	bge	.L175
	mov	r1, #1
	strb	r1, [r0, r3]
	ldr	r1, [r9, #24]
	mov	r0, #0
	strb	r0, [r1, r2]
	b	.L175
.L278:
	ldr	r5, [fp, #-52]
.L171:
	ldr	r3, [r7, #4]
	str	r3, [r6, #12]
	b	.L168
.L220:
	cmp	lr, r1
	cmple	ip, r1
	mov	r4, r3
	bge	.L200
	mov	r1, #1
	strb	r1, [r0, r3]
	ldr	r1, [r9, #20]
	mov	r0, #0
	strb	r0, [r1, r2]
	b	.L200
.L276:
	ldr	r2, [fp, #-48]
	mov	r1, #0
	ldr	r0, [r8, #440]
	bl	memset
	mov	r3, #0
	strb	r3, [r8, #449]
	b	.L168
.L161:
	cmp	r1, #1
	ldrb	lr, [r7, #13]
	ble	.L213
	mov	r2, #1
	str	r1, [fp, #-60]
	mov	r1, r10
	sxtb	r0, lr
	mov	r10, lr
	str	r0, [fp, #-56]
	mov	lr, r1
	mov	r0, r2
	b	.L165
.L164:
	ldr	r1, [fp, #-60]
	add	r3, r3, #8
	cmp	ip, r1
	bge	.L279
.L165:
	ldrsb	ip, [r3, #8]
	add	r2, r2, #1
	ldr	r4, [fp, #-56]
	uxtb	r2, r2
	cmp	ip, r4
	sxtb	ip, r2
	bne	.L164
	add	r1, r0, #1
	ldrb	r4, [r9, #50]
	str	r1, [fp, #-52]
	ldr	r1, [r3, #12]
	cmp	r1, r4
	ldrcsb	r0, [fp, #-52]
	b	.L164
.L279:
	mov	r3, lr
	mov	lr, r10
	mov	r10, r3
.L163:
	ldrb	r3, [r9, #48]
	cmp	r3, r0
	bhi	.L166
	strb	lr, [r6, #1]
	mov	r3, lr
	ldrb	r2, [r7, #13]
	b	.L162
.L275:
	ldr	r6, [fp, #-48]
.L196:
	ldr	r3, [r7, #8]
	str	r3, [r6, #8]
	b	.L193
.L273:
	mov	r2, r5
	mov	r1, #0
	ldr	r0, [r8, #444]
	bl	memset
	mov	r3, #0
	strb	r3, [r8, #450]
	b	.L193
.L186:
	cmp	r0, #1
	ldrb	r2, [r7, #14]
	ble	.L217
	mov	r1, #1
	sxtb	lr, r2
	mov	r4, r2
	mov	r2, r10
	mov	ip, r1
	mov	r10, r5
	str	r0, [fp, #-56]
	mov	r5, r2
	b	.L190
.L189:
	ldr	r2, [fp, #-52]
	add	r3, r3, #8
	ldr	r0, [fp, #-56]
	cmp	r2, r0
	bge	.L280
.L190:
	ldrsb	r2, [r3, #8]
	add	r1, r1, #1
	cmp	r2, lr
	uxtb	r1, r1
	sxtb	r2, r1
	str	r2, [fp, #-52]
	bne	.L189
	add	r0, ip, #1
	ldrb	r2, [r9, #51]
	str	r0, [fp, #-48]
	ldr	r0, [r3, #12]
	cmp	r0, r2
	ldrcsb	ip, [fp, #-48]
	b	.L189
.L280:
	mov	r3, r5
	mov	r2, r4
	mov	r5, r10
	mov	r10, r3
.L188:
	ldrb	r3, [r9, #48]
	cmp	r3, ip
	bhi	.L191
	strb	r2, [r6, #2]
	ldrb	r3, [r7, #14]
	b	.L187
.L180:
	mov	r1, #255
	mvn	r2, #0
	strb	r2, [r3, #14]
.L181:
	mov	r3, #100
	str	r3, [r7, #8]
	b	.L182
.L269:
	add	r8, r6, #8192
	ldr	r2, [fp, #-48]
	mov	r1, #0
	ldr	r0, [r8, #436]
	bl	memset
	mov	r3, #0
	strb	r3, [r8, #448]
	b	.L108
.L101:
	cmp	r1, #1
	ldrb	lr, [r7, #12]
	ble	.L204
	mov	r2, #1
	str	r1, [fp, #-60]
	mov	r1, r10
	sxtb	r0, lr
	mov	r10, lr
	str	r0, [fp, #-56]
	mov	lr, r1
	mov	r0, r2
	b	.L105
.L104:
	ldr	r1, [fp, #-60]
	add	r3, r3, #8
	cmp	ip, r1
	bge	.L281
.L105:
	ldrsb	ip, [r3, #8]
	add	r2, r2, #1
	ldr	r8, [fp, #-56]
	uxtb	r2, r2
	cmp	ip, r8
	sxtb	ip, r2
	bne	.L104
	add	r1, r0, #1
	ldrb	r8, [r9, #49]
	str	r1, [fp, #-52]
	ldr	r1, [r3, #12]
	cmp	r1, r8
	ldrcsb	r0, [fp, #-52]
	b	.L104
.L281:
	mov	r3, lr
	mov	lr, r10
	mov	r10, r3
.L103:
	ldrb	r3, [r9, #48]
	cmp	r3, r0
	bhi	.L106
	strb	lr, [r6]
	mov	r2, lr
	ldrb	r3, [r7, #12]
	b	.L102
.L204:
	mov	r0, #1
	b	.L103
.L277:
	ldr	r0, [r9, #24]
	mov	r1, #0
	ldr	r9, [fp, #-48]
	mov	r4, r1
	mov	r2, r9
	bl	memset
	str	r4, [r6, #12]
	mvn	r3, #0
	mov	r2, r9
	strb	r3, [r6, #1]
	mov	r1, r4
	ldr	r0, [r8, #440]
	bl	memset
	strb	r4, [r8, #449]
	str	r4, [r8, #464]
	b	.L168
.L213:
	mov	r0, #1
	b	.L163
.L274:
	mov	r1, #0
	mov	r2, r5
	mov	r4, r1
	ldr	r0, [r9, #20]
	bl	memset
	str	r4, [r6, #8]
	mvn	r3, #0
	mov	r2, r5
	strb	r3, [r6, #2]
	mov	r1, r4
	ldr	r0, [r8, #444]
	bl	memset
	strb	r4, [r8, #450]
	str	r4, [r8, #460]
	b	.L193
.L217:
	mov	ip, #1
	b	.L188
.L271:
	mov	r3, r2
	ldr	r1, [fp, #-56]
	ldr	r2, [fp, #-48]
	ldr	r0, [fp, #-52]
	bl	FillBorder_Size8
.L111:
	sub	r1, fp, #56
	ldr	r3, [fp, #-72]
	ldmia	r1, {r1, r2, r4}
	mov	r0, r4
	bl	CheckStartEndBorder
	ldr	r3, .L283+16
	mov	r2, r4
	ldr	r0, [r9, #16]
	ldr	r1, [r3, #16]
	bl	memcpy
	ldrsb	r3, [r6]
	cmp	r3, #8
	beq	.L146
	ldrb	r3, [r8, #448]
	ldrb	r2, [r8, #451]
	cmp	r2, r3
	addhi	r3, r3, #1
	strhib	r3, [r8, #448]
	bhi	.L146
	cmp	r4, #4
	mov	r3, r4
	strhi	r5, [fp, #-52]
	movhi	r3, #5
	bhi	.L153
	b	.L146
.L211:
	mov	r4, r3
.L150:
	ldr	r2, [fp, #-48]
	add	r3, r3, #1
	cmp	r2, r4
	bls	.L282
.L153:
	ldr	r0, [r9, #16]
	sub	r2, r3, #1
	ldrb	r4, [r0, r2]
	cmp	r4, #1
	bne	.L211
	ldr	r1, [r8, #436]
	ldrb	lr, [r8, #451]
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L211
	sub	r5, r3, #2
	ldrsb	ip, [r1, r5]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L212
	cmp	ip, r1
	blt	.L212
	strb	r4, [r0, r5]
	mov	r0, #0
	ldr	r1, [r9, #16]
	mov	r4, r3
	strb	r0, [r1, r2]
	b	.L150
.L282:
	ldr	r5, [fp, #-52]
.L146:
	ldr	r3, [r7]
	str	r3, [r6, #4]
	b	.L108
.L212:
	cmp	lr, r1
	cmple	ip, r1
	mov	r4, r3
	bge	.L150
	mov	r1, #1
	strb	r1, [r0, r3]
	ldr	r1, [r9, #16]
	mov	r0, #0
	strb	r0, [r1, r2]
	b	.L150
.L270:
	ldr	r0, [r9, #16]
	add	r8, r6, #8192
	ldr	r9, [fp, #-48]
	mov	r1, #0
	mov	r4, r1
	mov	r2, r9
	bl	memset
	str	r4, [r6, #4]
	mvn	r3, #0
	mov	r2, r9
	strb	r3, [r6]
	mov	r1, r4
	ldr	r0, [r8, #436]
	bl	memset
	strb	r4, [r8, #448]
	str	r4, [r8, #456]
	b	.L108
.L206:
	mov	r3, #4
	str	r3, [fp, #-68]
	str	r3, [fp, #-60]
	b	.L112
.L272:
	sub	r5, fp, #92
	ldmia	r5, {r5, r6, r10}
.L128:
	ldr	r3, [fp, #-48]
	cmp	r3, #4
	bls	.L111
	ldr	r3, [fp, #-48]
	mov	ip, #0
	ldr	r2, [fp, #-80]
	sub	lr, r3, #1
	ldr	r0, [fp, #-76]
	add	r3, r2, #3
	add	lr, r2, lr
	ldr	r2, [fp, #-52]
	add	r2, r2, #3
	b	.L144
.L140:
	ldrb	r1, [r3]
	sub	r4, r1, #1
	sxtb	r1, r1
	cmp	r1, #0
	strleb	ip, [r3]
	ble	.L142
.L262:
	strb	r4, [r3]
.L142:
	add	r3, r3, #1
	cmp	r3, lr
	beq	.L111
.L144:
	ldrb	r1, [r2, #1]!
	cmp	r1, #1
	bne	.L140
	strb	ip, [r2]
	strb	r1, [r2, #-1]
	ldrb	r1, [r3]
	add	r4, r1, #1
	sxtb	r1, r1
	cmp	r1, r0
	strgeb	r0, [r3]
	bge	.L142
	b	.L262
.L261:
	str	lr, [fp, #-64]
	b	.L122
.L284:
	.align	2
.L283:
	.word	.LANCHOR0
	.word	.LANCHOR0+16384
	.word	.LANCHOR0+4
	.word	.LANCHOR0+16399
	.word	.LANCHOR0+20480
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_GetDbDetInfo, .-PQ_MNG_ALG_GetDbDetInfo
	.global	__aeabi_idiv
	.align	2
	.global	PQ_MNG_ALG_DBCfgDetector
	.type	PQ_MNG_ALG_DBCfgDetector, %function
PQ_MNG_ALG_DBCfgDetector:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r8, [r1]
	mov	r4, r2
	ldr	r9, [r1, #4]
	mov	r6, r1
	mov	r5, r0
	str	r8, [r0, #668]
	ldr	r3, [r1, #4]
	str	r3, [r0, #672]
	ldr	r3, [r1, #48]
	ldr	r2, [r1]
	cmp	r3, #8
	ldr	r7, [r1, #4]
	ldr	r3, [r1, #8]
	movne	r2, #90
	strneh	r2, [r0, #24]
	beq	.L311
.L287:
	ldr	r2, [r6, #64]
	cmp	r2, #8
	movne	r3, #60
	strneh	r3, [r5, #26]
	beq	.L312
.L289:
	ldr	r1, [r6, #56]
	add	r3, r1, #1
	cmp	r3, #1
	bls	.L290
	mov	r0, r7
	bl	__aeabi_idiv
	cmp	r7, #0
	add	r2, r7, #7
	add	r3, r7, #63
	movge	r2, r7
	mov	r2, r2, asr #3
	uxth	r0, r0
	cmp	r0, r2
	movge	r0, r2
	cmp	r7, #0
	movlt	r7, r3
	mov	r3, r7, asr #6
	cmp	r0, r3
	movlt	r0, r3
	strh	r0, [r5, #28]
.L290:
	movw	r3, #1080
	cmp	r8, #1920
	cmple	r9, r3
	mov	r2, #0
	mov	r3, #1
	strh	r2, [r5, #20]
	strh	r3, [r5, #22]
	strh	r3, [r5, #16]
	strh	r3, [r5, #18]
	movle	r3, #6
	strgth	r2, [r5, #22]
	movgt	r3, #8
	strh	r3, [r5, #30]
	ldmia	r6, {r1, r2}
	ldr	r3, [r6, #8]
	cmp	r1, #720
	movle	r1, #16
	strleh	r1, [r5, #32]
	ble	.L294
	cmp	r1, #1920
	movle	r1, #42
	movgt	r1, #63
	strh	r1, [r5, #32]
.L294:
	cmp	r2, #576
	movle	r2, #16
	strleh	r2, [r5, #36]
	ble	.L297
	movw	r1, #1080
	cmp	r2, r1
	movle	r2, #36
	movgt	r2, #63
	strh	r2, [r5, #36]
.L297:
	cmp	r3, #360
	movle	r3, #16
	strleh	r3, [r5, #34]
	ble	.L300
	cmp	r3, #960
	movle	r3, #42
	movgt	r3, #63
	strh	r3, [r5, #34]
.L300:
	mov	r2, #512
	mov	r1, #0
	add	r0, r5, #40
	add	r4, r4, #16
	bl	memset
	add	ip, r5, #36
	mov	lr, #0
.L302:
	mov	r0, r4
	ldr	r2, [ip, #4]!
	ldrb	r1, [r0, lr]!
	mov	r3, #1
	and	r1, r1, r3
	orr	r1, r1, r2
	str	r1, [ip]
	b	.L304
.L314:
	ldrb	r2, [r0, #1]!
	and	r2, r2, #1
	orr	r1, r1, r2, asl r3
	add	r3, r3, #1
	cmp	r3, #32
	str	r1, [ip]
	beq	.L313
.L304:
	add	r2, r3, lr
	cmp	r2, #4096
	bcc	.L314
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L313:
	add	lr, lr, #32
	cmp	lr, #4096
	bne	.L302
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L311:
	add	r1, r2, #7
	cmp	r2, #0
	movlt	r2, r1
	mov	r2, r2, asr #3
	strh	r2, [r0, #24]
	b	.L287
.L312:
	add	r2, r3, #7
	cmp	r3, #0
	movlt	r3, r2
	mov	r3, r3, asr #3
	strh	r3, [r5, #26]
	b	.L289
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_DBCfgDetector, .-PQ_MNG_ALG_DBCfgDetector
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	stDetStaticInfo, %object
	.size	stDetStaticInfo, 20508
stDetStaticInfo:
	.space	20508
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
