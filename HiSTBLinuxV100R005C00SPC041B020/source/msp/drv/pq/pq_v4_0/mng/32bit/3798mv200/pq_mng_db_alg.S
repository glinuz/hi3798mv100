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
	mov	r4, r2
	mov	r9, r0
	movw	r2, #20508
	mov	r7, r1
	ldr	r0, .L281
	mov	r1, #0
	ldr	r10, .L281+4
	bl	memset
	mov	r1, r4
	movw	r2, #8808
	mov	r0, r9
	bl	memcpy
	ldr	r2, [r4, #48]
	ldr	r0, .L281
	add	r3, r2, #1
	strb	r2, [r10, #12]
	ldr	r2, [r4, #64]
	uxtb	r3, r3
	cmp	r3, #64
	strb	r2, [r10, #13]
	add	r2, r2, #1
	ldr	r3, [r4, #56]
	uxtb	r2, r2
	strb	r3, [r10, #14]
	add	r3, r3, #1
	ldr	r1, [r4, #44]
	uxtb	r3, r3
	str	r1, [r10]
	ldr	r1, [r4, #60]
	str	r1, [r10, #4]
	ldr	r1, [r4, #52]
	str	r1, [r10, #8]
	mvnhi	r1, #0
	strhib	r1, [r10, #12]
	cmp	r2, #64
	mov	r1, #0
	mvnhi	r2, #0
	strhib	r2, [r10, #13]
	cmp	r3, #64
	mov	r2, #16384
	mvnhi	r3, #0
	strhib	r3, [r10, #14]
	bl	memset
	ldr	r2, .L281+8
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
	mov	r2, #2048
	mov	r1, #0
	ldr	r0, .L281+12
	mov	r5, #1
	bl	memset
	ldr	lr, .L281
	add	ip, r4, #64
	movw	r6, #2049
.L86:
	cmp	r5, r6
	ldr	r3, [ip, #4]!
	beq	.L87
	add	r2, r5, #16384
	and	r3, r3, #1
	add	r2, r2, #14
	mov	r1, r5
	mov	r0, #1
	strb	r3, [lr, r2]
	b	.L88
.L264:
	mov	r2, r2, lsr r0
	add	r0, r0, #1
	cmp	r0, #32
	and	r2, r2, #1
	add	r1, r1, #1
	strb	r2, [r3]
	beq	.L263
.L88:
	add	r3, lr, r1
	cmp	r1, #2048
	add	r3, r3, #16384
	ldr	r2, [ip]
	add	r3, r3, #15
	bcc	.L264
.L87:
	ldr	r0, .L281+12
	movw	r3, #3836
	ldr	r5, .L281+16
	add	r1, r0, #1920
	add	r2, r0, #2880
	str	r0, [r5, #16]
	str	r2, [r5, #24]
	str	r1, [r5, #20]
	ldr	r6, [r4]
	ldr	r0, [r4, #8]
	sub	r2, r6, #4
	ldr	r1, [r4, #4]
	cmp	r2, r3
	bhi	.L203
	sub	r3, r1, #1
	cmp	r3, #2160
	bcs	.L203
.L90:
	movw	r3, #1080
	cmp	r6, #1920
	cmple	r1, r3
	movgt	r3, #0
	strgt	r3, [r4, #36]
	strgt	r3, [r4, #40]
	cmp	r0, #960
	movgt	r3, #0
	strgt	r3, [r4, #36]
	ldr	r3, [r4, #32]
	cmp	r3, #1
	beq	.L265
.L93:
	add	r8, r7, #8192
	mvn	r3, #0
	strb	r3, [r7]
.L107:
	ldr	r3, [r4, #36]
	cmp	r3, #1
	beq	.L266
.L153:
	mvn	r3, #0
	strb	r3, [r7, #1]
.L167:
	ldr	r3, [r4, #40]
	cmp	r3, #1
	beq	.L267
.L178:
	mvn	r3, #0
	strb	r3, [r7, #2]
.L192:
	ldr	r3, [r7, #4]
	mov	r0, #0
	str	r3, [r9, #44]
	ldrsb	r3, [r7]
	str	r3, [r9, #48]
	ldr	r3, [r7, #8]
	str	r3, [r9, #52]
	ldrsb	r3, [r7, #2]
	str	r3, [r9, #56]
	ldr	r3, [r7, #12]
	str	r3, [r9, #60]
	ldrsb	r3, [r7, #1]
	str	r3, [r9, #64]
	ldr	r3, [r8, #456]
	str	r3, [r9, #16]
	ldr	r3, [r8, #460]
	str	r3, [r9, #20]
	ldr	r3, [r8, #464]
	str	r3, [r9, #24]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L203:
	mov	r3, #0
	str	r3, [r4, #28]
	b	.L90
.L263:
	add	r5, r5, #32
	b	.L86
.L265:
	ldr	r3, [r4, #28]
	cmp	r3, #1
	bne	.L93
	add	r3, r7, #4096
	ldr	r1, [r10]
	str	r3, [fp, #-48]
	mov	r2, r3
	ldr	r3, .L281+4
	ldrb	r2, [r2, #30]
	cmp	r1, r2
	bcc	.L94
	ldrb	r0, [r3, #12]
	add	r3, r0, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L95
.L96:
	ldr	r3, [fp, #-48]
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
	ldr	r2, [fp, #-48]
	ldrb	r0, [r10, #12]
	ldrb	r1, [r2, #28]
.L97:
	ldr	ip, [fp, #-48]
	ldr	r2, [ip, #36]
	strb	r0, [ip, #52]
	ldr	r0, [r10]
	cmp	r2, r1
	addcc	r2, r2, #1
	str	r0, [ip, #56]
	movcc	r0, ip
	strcc	r2, [r0, #36]
	cmp	r1, r2
	beq	.L100
.L105:
	ldrb	r2, [r7]
	ldrb	r3, [r10, #12]
.L101:
	cmp	r3, r2
	sxtb	r3, r2
	str	r3, [fp, #-56]
	bne	.L268
	add	r3, r2, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L269
	ldr	r3, [fp, #-56]
	add	r8, r7, #8192
	cmp	r3, #8
	add	r3, r7, #8640
	ldrb	r1, [r8, #452]
	mov	r2, r3
	ldr	r3, [r5, #16]
	str	r1, [fp, #-80]
	str	r3, [fp, #-52]
	mov	r3, #0
	str	r3, [r8, #456]
	add	r3, r2, #8
	str	r3, [fp, #-76]
	beq	.L270
	sub	lr, r6, #3
	ldr	r3, [r8, #436]
	cmp	lr, #4
	str	r3, [fp, #-84]
	bls	.L207
	ldr	r2, [fp, #-52]
	ldrb	r3, [r2, #4]
	cmp	r3, #1
	beq	.L207
	add	r3, r2, #4
	mov	r2, #4
	b	.L112
.L113:
	ldrb	r1, [r3, #1]!
	cmp	r1, #1
	beq	.L260
.L112:
	add	r2, r2, #1
	str	r2, [fp, #-60]
	cmp	r2, lr
	bne	.L113
.L260:
	str	r2, [fp, #-68]
.L111:
	ldr	r2, [fp, #-56]
	ldr	r3, [fp, #-60]
	rsb	r3, r2, r3
	cmp	r3, #4
	strgt	r6, [fp, #-64]
	ldrgt	r2, .L281
	strgt	r2, [fp, #-72]
	ble	.L120
.L119:
	sub	r2, r3, #-1073741823
	ldr	r0, [fp, #-72]
	mov	ip, #0
	add	r2, r0, r2, lsl #2
	mov	r6, ip
	mov	r1, ip
.L118:
	ldr	r0, [r2], #4
	cmp	r0, r6
	movhi	ip, r1
	add	r1, r1, #1
	movhi	r6, r0
	cmp	r1, #3
	bne	.L118
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
	bgt	.L119
	ldr	r6, [fp, #-64]
.L120:
	cmp	lr, #0
	mov	r3, lr
	ble	.L261
	ldr	r2, [fp, #-52]
	ldrb	r1, [r2, lr]
	add	r2, r2, lr
	cmp	r1, #1
	bne	.L123
	b	.L261
.L124:
	ldrb	r1, [r2, #-1]!
	cmp	r1, #1
	beq	.L208
.L123:
	subs	r3, r3, #1
	bne	.L124
.L208:
	str	r3, [fp, #-64]
.L121:
	ldr	r2, [fp, #-56]
	ldr	r3, [fp, #-64]
	add	r3, r3, r2
	cmp	r3, lr
	strcc	r6, [fp, #-88]
	ldrcc	r2, .L281
	strcc	r2, [fp, #-72]
	bcs	.L131
.L130:
	sub	r2, r3, #-1073741823
	ldr	r0, [fp, #-72]
	mov	ip, #0
	add	r2, r0, r2, lsl #2
	mov	r6, ip
	mov	r1, ip
.L129:
	ldr	r0, [r2], #4
	cmp	r0, r6
	movhi	ip, r1
	add	r1, r1, #1
	movhi	r6, r0
	cmp	r1, #3
	bne	.L129
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
	bcc	.L130
	ldr	r6, [fp, #-88]
.L131:
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-64]
	cmp	r3, r2
	bhi	.L127
	ldr	ip, [fp, #-60]
	mov	r2, #0
	str	r6, [fp, #-72]
	str	r9, [fp, #-88]
	mov	r9, r2
	mov	r6, ip
	str	r4, [fp, #-92]
	b	.L138
.L134:
	ldr	r3, [fp, #-68]
	ldr	r2, [fp, #-64]
	add	r3, r3, #1
	str	r3, [fp, #-68]
	cmp	r3, r2
	mov	r6, r3
	bhi	.L271
.L138:
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3, r6]
	cmp	r3, #1
	bne	.L134
	ldr	r2, [fp, #-60]
	str	r3, [fp, #-96]
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	cmpne	r2, r6
	ldr	r2, [r8, #456]
	add	r2, r2, #1
	str	r2, [r8, #456]
	beq	.L210
	mov	r1, r3
	rsb	r0, r9, r6
	bl	__aeabi_uidiv
	cmp	r0, #1
	mov	r4, r0
	bls	.L210
	ldr	r3, [fp, #-96]
	mov	r0, r3
.L137:
	ldr	r3, [fp, #-56]
	ldr	lr, .L281
	add	r3, r9, r3
	mov	r9, #0
	sub	r1, r3, #-1073741823
	mov	ip, r9
	mov	r2, r9
	add	r1, lr, r1, lsl #2
.L136:
	ldr	lr, [r1], #4
	cmp	lr, ip
	movhi	r9, r2
	add	r2, r2, #1
	movhi	ip, lr
	cmp	r2, #3
	bne	.L136
	sub	r3, r3, #1
	ldr	r2, [fp, #-52]
	add	r9, r3, r9
	add	r0, r0, #1
	mov	r3, #1
	cmp	r0, r4
	strb	r3, [r2, r9]
	ldr	r3, [r8, #456]
	add	r3, r3, #1
	str	r3, [r8, #456]
	bne	.L137
.L210:
	mov	r9, r6
	b	.L134
.L267:
	ldr	r3, [r4, #28]
	cmp	r3, #1
	bne	.L178
	add	r3, r7, #4096
	ldr	r1, [r10, #8]
	str	r3, [fp, #-48]
	ldrb	r2, [r3, #32]
	ldr	r5, [r4, #4]
	cmp	r1, r2
	ldr	r3, .L281+4
	bcc	.L179
	ldrb	r1, [r3, #14]
	add	r3, r1, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L180
.L181:
	ldr	r3, [fp, #-48]
	ldrb	r0, [r3, #28]
	add	r3, r7, #4224
	add	r3, r3, #52
	sub	r2, r0, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L182
.L183:
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
	bne	.L183
	ldr	r2, [fp, #-48]
	ldrb	r1, [r10, #14]
	ldrb	r0, [r2, #28]
.L182:
	ldr	ip, [fp, #-48]
	ldr	r2, [ip, #44]
	strb	r1, [ip, #180]
	ldr	r1, [r10, #8]
	cmp	r2, r0
	addcc	r2, r2, #1
	str	r1, [ip, #184]
	movcc	r1, ip
	strcc	r2, [r1, #44]
	cmp	r0, r2
	beq	.L185
.L190:
	ldrb	r2, [r7, #2]
	ldrb	r3, [r10, #14]
.L186:
	cmp	r3, r2
	bne	.L272
	add	r2, r2, #1
	uxtb	r3, r2
	cmp	r3, #1
	bls	.L273
	mov	r2, #1
	mov	r1, r5
	mov	r0, r7
	bl	FillBorderHchrVy.constprop.12
	ldrsb	r3, [r7, #2]
	cmp	r3, #8
	beq	.L195
	ldrb	r3, [r8, #450]
	ldrb	r2, [r8, #451]
	cmp	r2, r3
	addhi	r3, r3, #1
	strhib	r3, [r8, #450]
	bhi	.L195
	cmp	r5, #4
	bls	.L195
	str	r7, [fp, #-56]
	mov	r3, #5
	mov	r6, #0
	str	r9, [fp, #-52]
	ldr	r7, [fp, #-48]
	b	.L202
.L220:
	mov	r4, r3
.L199:
	cmp	r5, r4
	add	r3, r3, #1
	bls	.L274
.L202:
	ldr	r0, [r7, #20]
	sub	r2, r3, #1
	ldrb	r9, [r0, r2]
	cmp	r9, #1
	bne	.L220
	ldr	r1, [r8, #444]
	ldrb	lr, [r8, #451]
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L220
	sub	r4, r3, #2
	ldrsb	ip, [r1, r4]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L221
	cmp	ip, r1
	blt	.L221
	strb	r9, [r0, r4]
	mov	r4, r3
	ldr	r1, [r7, #20]
	strb	r6, [r1, r2]
	b	.L199
.L266:
	ldr	r3, [r4, #28]
	cmp	r3, #1
	bne	.L153
	add	r3, r7, #4096
	ldr	r1, [r10, #4]
	str	r3, [fp, #-48]
	ldrb	r2, [r3, #31]
	ldr	r6, [r4, #8]
	cmp	r1, r2
	ldr	r3, .L281+4
	bcc	.L154
	ldrb	r0, [r3, #13]
	add	r3, r0, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L155
.L156:
	ldr	r3, [fp, #-48]
	ldrb	r1, [r3, #28]
	add	r3, r7, #4352
	add	r3, r3, #52
	sub	r2, r1, #1
	uxtb	r2, r2
	sxtb	ip, r2
	cmp	ip, #0
	ble	.L157
.L158:
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
	bne	.L158
	ldr	r2, [fp, #-48]
	ldrb	r0, [r10, #13]
	ldrb	r1, [r2, #28]
.L157:
	ldr	ip, [fp, #-48]
	ldr	r2, [ip, #40]
	strb	r0, [ip, #308]
	ldr	r0, [r10, #4]
	cmp	r2, r1
	addcc	r2, r2, #1
	str	r0, [ip, #312]
	movcc	r0, ip
	strcc	r2, [r0, #40]
	cmp	r1, r2
	beq	.L160
.L165:
	ldrb	r2, [r7, #1]
	ldrb	r3, [r10, #13]
.L161:
	cmp	r3, r2
	bne	.L275
	add	r3, r2, #1
	uxtb	r3, r3
	cmp	r3, #1
	bls	.L276
	mov	r2, #2
	mov	r1, r6
	mov	r0, r7
	bl	FillBorderHchrVy.constprop.12
	ldrsb	r3, [r7, #1]
	cmp	r3, #8
	beq	.L170
	ldrb	r3, [r8, #449]
	ldrb	r2, [r8, #451]
	cmp	r2, r3
	addhi	r3, r3, #1
	strhib	r3, [r8, #449]
	bhi	.L170
	cmp	r6, #4
	bls	.L170
	str	r4, [fp, #-56]
	mov	r3, #5
	str	r7, [fp, #-52]
	ldr	r4, [fp, #-48]
	b	.L177
.L216:
	mov	r5, r3
.L174:
	cmp	r6, r5
	add	r3, r3, #1
	bls	.L277
.L177:
	ldr	r0, [r4, #24]
	sub	r2, r3, #1
	ldrb	r5, [r0, r2]
	cmp	r5, #1
	bne	.L216
	ldr	r1, [r8, #440]
	ldrb	lr, [r8, #451]
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L216
	sub	r7, r3, #2
	ldrsb	ip, [r1, r7]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L217
	cmp	ip, r1
	blt	.L217
	strb	r5, [r0, r7]
	mov	r0, #0
	ldr	r1, [r4, #24]
	mov	r5, r3
	strb	r0, [r1, r2]
	b	.L174
.L94:
	mov	r0, #255
	mvn	r2, #0
	strb	r2, [r3, #12]
.L95:
	mov	r3, #100
	str	r3, [r10]
	b	.L96
.L154:
	mov	r0, #255
	mvn	r2, #0
	strb	r2, [r3, #13]
.L155:
	mov	r3, #100
	str	r3, [r10, #4]
	b	.L156
.L217:
	cmp	lr, r1
	cmple	ip, r1
	mov	r5, r3
	bge	.L174
	mov	r1, #1
	strb	r1, [r0, r3]
	ldr	r1, [r4, #24]
	mov	r0, #0
	strb	r0, [r1, r2]
	b	.L174
.L277:
	ldr	r7, [fp, #-52]
	ldr	r4, [fp, #-56]
.L170:
	ldr	r3, [r10, #4]
	str	r3, [r7, #12]
	b	.L167
.L221:
	cmp	lr, r1
	cmple	ip, r1
	mov	r4, r3
	movlt	r1, #1
	strltb	r1, [r0, r3]
	ldrlt	r1, [r7, #20]
	strltb	r6, [r1, r2]
	b	.L199
.L275:
	mov	r2, r6
	mov	r1, #0
	ldr	r0, [r8, #440]
	bl	memset
	mov	r3, #0
	strb	r3, [r8, #449]
	b	.L167
.L160:
	cmp	r1, #1
	ldrb	lr, [r10, #13]
	ble	.L214
	mov	r2, #1
	sxtb	r0, lr
	str	r1, [fp, #-60]
	str	r0, [fp, #-56]
	mov	r0, r2
	b	.L164
.L163:
	ldr	r1, [fp, #-60]
	add	r3, r3, #8
	cmp	ip, r1
	bge	.L162
.L164:
	ldrsb	ip, [r3, #8]
	add	r2, r2, #1
	ldr	r5, [fp, #-56]
	uxtb	r2, r2
	cmp	ip, r5
	sxtb	ip, r2
	bne	.L163
	ldr	r5, [fp, #-48]
	add	r1, r0, #1
	str	r1, [fp, #-52]
	ldr	r1, [r3, #12]
	ldrb	r5, [r5, #50]
	cmp	r1, r5
	ldrcsb	r0, [fp, #-52]
	b	.L163
.L214:
	mov	r0, #1
.L162:
	ldr	r3, [fp, #-48]
	ldrb	r3, [r3, #48]
	cmp	r3, r0
	bhi	.L165
	strb	lr, [r7, #1]
	mov	r2, lr
	ldrb	r3, [r10, #13]
	b	.L161
.L274:
	ldr	r9, [fp, #-52]
	ldr	r7, [fp, #-56]
.L195:
	ldr	r3, [r10, #8]
	str	r3, [r7, #8]
	b	.L192
.L272:
	mov	r2, r5
	mov	r1, #0
	ldr	r0, [r8, #444]
	bl	memset
	mov	r3, #0
	strb	r3, [r8, #450]
	b	.L192
.L185:
	cmp	r0, #1
	ldrb	r2, [r10, #14]
	ble	.L218
	mov	r1, #1
	sxtb	lr, r2
	mov	ip, r1
	mov	r6, r5
	mov	r4, r2
	b	.L189
.L188:
	ldr	r2, [fp, #-56]
	add	r3, r3, #8
	cmp	r2, r0
	bge	.L278
.L189:
	ldrsb	r2, [r3, #8]
	add	r1, r1, #1
	cmp	r2, lr
	uxtb	r1, r1
	sxtb	r2, r1
	str	r2, [fp, #-56]
	bne	.L188
	ldr	r5, [fp, #-48]
	add	r2, ip, #1
	str	r2, [fp, #-52]
	ldr	r2, [r3, #12]
	ldrb	r5, [r5, #51]
	cmp	r2, r5
	ldrcsb	ip, [fp, #-52]
	b	.L188
.L278:
	mov	r5, r6
	mov	r2, r4
.L187:
	ldr	r3, [fp, #-48]
	ldrb	r3, [r3, #48]
	cmp	r3, ip
	bhi	.L190
	strb	r2, [r7, #2]
	ldrb	r3, [r10, #14]
	b	.L186
.L179:
	mov	r1, #255
	mvn	r2, #0
	strb	r2, [r3, #14]
.L180:
	mov	r3, #100
	str	r3, [r10, #8]
	b	.L181
.L268:
	add	r8, r7, #8192
	mov	r2, r6
	mov	r1, #0
	ldr	r0, [r8, #436]
	bl	memset
	mov	r3, #0
	strb	r3, [r8, #448]
	b	.L107
.L100:
	cmp	r1, #1
	ldrb	lr, [r10, #12]
	ble	.L205
	mov	r2, #1
	sxtb	r0, lr
	mov	r8, r6
	str	r0, [fp, #-56]
	str	r1, [fp, #-60]
	mov	r0, r2
	b	.L104
.L103:
	ldr	r1, [fp, #-60]
	add	r3, r3, #8
	cmp	ip, r1
	bge	.L279
.L104:
	ldrsb	ip, [r3, #8]
	add	r2, r2, #1
	ldr	r6, [fp, #-56]
	uxtb	r2, r2
	cmp	ip, r6
	sxtb	ip, r2
	bne	.L103
	ldr	r6, [fp, #-48]
	add	r1, r0, #1
	str	r1, [fp, #-52]
	ldr	r1, [r3, #12]
	ldrb	r6, [r6, #49]
	cmp	r1, r6
	ldrcsb	r0, [fp, #-52]
	b	.L103
.L279:
	mov	r6, r8
.L102:
	ldr	r3, [fp, #-48]
	ldrb	r3, [r3, #48]
	cmp	r3, r0
	bhi	.L105
	strb	lr, [r7]
	mov	r2, lr
	ldrb	r3, [r10, #12]
	b	.L101
.L205:
	mov	r0, #1
	b	.L102
.L276:
	ldr	r3, [fp, #-48]
	mov	r1, #0
	mov	r5, r1
	mov	r2, r6
	ldr	r0, [r3, #24]
	bl	memset
	str	r5, [r7, #12]
	mvn	r3, #0
	mov	r2, r6
	strb	r3, [r7, #1]
	mov	r1, r5
	ldr	r0, [r8, #440]
	bl	memset
	strb	r5, [r8, #449]
	str	r5, [r8, #464]
	b	.L167
.L273:
	ldr	r3, [fp, #-48]
	mov	r1, #0
	mov	r4, r1
	mov	r2, r5
	ldr	r0, [r3, #20]
	bl	memset
	str	r4, [r7, #8]
	mvn	r3, #0
	mov	r2, r5
	strb	r3, [r7, #2]
	mov	r1, r4
	ldr	r0, [r8, #444]
	bl	memset
	strb	r4, [r8, #450]
	str	r4, [r8, #460]
	b	.L192
.L218:
	mov	ip, #1
	b	.L187
.L270:
	mov	r2, r6
	ldr	r1, [fp, #-56]
	ldr	r0, [fp, #-52]
	bl	FillBorder_Size8
.L110:
	ldr	r3, [fp, #-76]
	mov	r0, r6
	ldr	r2, [fp, #-52]
	ldr	r1, [fp, #-56]
	bl	CheckStartEndBorder
	ldr	r3, [fp, #-48]
	mov	r2, r6
	ldr	r1, [r5, #16]
	ldr	r0, [r3, #16]
	bl	memcpy
	ldrsb	r3, [r7]
	cmp	r3, #8
	beq	.L145
	ldrb	r3, [r8, #448]
	ldrb	r2, [r8, #451]
	cmp	r2, r3
	addhi	r3, r3, #1
	strhib	r3, [r8, #448]
	bhi	.L145
	cmp	r6, #4
	bls	.L145
	str	r4, [fp, #-56]
	mov	r3, #5
	str	r7, [fp, #-52]
	ldr	r4, [fp, #-48]
	b	.L152
.L212:
	mov	r5, r3
.L149:
	cmp	r6, r5
	add	r3, r3, #1
	bls	.L280
.L152:
	ldr	r0, [r4, #16]
	sub	r2, r3, #1
	ldrb	r5, [r0, r2]
	cmp	r5, #1
	bne	.L212
	ldr	r1, [r8, #436]
	ldrb	lr, [r8, #451]
	ldrsb	ip, [r1, r2]
	cmp	ip, lr
	bge	.L212
	sub	r7, r3, #2
	ldrsb	ip, [r1, r7]
	ldrsb	r1, [r1, r3]
	cmp	lr, ip
	bgt	.L213
	cmp	ip, r1
	blt	.L213
	strb	r5, [r0, r7]
	mov	r0, #0
	ldr	r1, [r4, #16]
	mov	r5, r3
	strb	r0, [r1, r2]
	b	.L149
.L280:
	ldr	r7, [fp, #-52]
	ldr	r4, [fp, #-56]
.L145:
	ldr	r3, [r10]
	str	r3, [r7, #4]
	b	.L107
.L213:
	cmp	lr, r1
	cmple	ip, r1
	mov	r5, r3
	bge	.L149
	mov	r1, #1
	strb	r1, [r0, r3]
	ldr	r1, [r4, #16]
	mov	r0, #0
	strb	r0, [r1, r2]
	b	.L149
.L269:
	ldr	r3, [fp, #-48]
	add	r8, r7, #8192
	mov	r1, #0
	mov	r2, r6
	mov	r5, r1
	ldr	r0, [r3, #16]
	bl	memset
	str	r5, [r7, #4]
	mvn	r3, #0
	mov	r2, r6
	strb	r3, [r7]
	mov	r1, r5
	ldr	r0, [r8, #436]
	bl	memset
	strb	r5, [r8, #448]
	str	r5, [r8, #456]
	b	.L107
.L207:
	mov	r3, #4
	str	r3, [fp, #-68]
	str	r3, [fp, #-60]
	b	.L111
.L271:
	ldr	r6, [fp, #-72]
	ldr	r9, [fp, #-88]
	ldr	r4, [fp, #-92]
.L127:
	cmp	r6, #4
	bls	.L110
	ldr	r2, [fp, #-84]
	sub	lr, r6, #1
	mov	ip, #0
	add	r3, r2, #3
	add	lr, r2, lr
	ldr	r2, [fp, #-52]
	add	r2, r2, #3
	b	.L143
.L139:
	ldrb	r1, [r3]
	sub	r0, r1, #1
	sxtb	r1, r1
	cmp	r1, #0
	strgtb	r0, [r3]
	strleb	ip, [r3]
.L141:
	add	r3, r3, #1
	cmp	r3, lr
	beq	.L110
.L143:
	ldrb	r1, [r2, #1]!
	cmp	r1, #1
	bne	.L139
	strb	ip, [r2]
	strb	r1, [r2, #-1]
	ldrb	r1, [r3]
	add	r0, r1, #1
	str	r0, [fp, #-60]
	ldr	r0, [fp, #-80]
	sxtb	r1, r1
	cmp	r1, r0
	ldrltb	r1, [fp, #-60]
	ldrgeb	r1, [fp, #-80]
	strb	r1, [r3]
	b	.L141
.L261:
	str	lr, [fp, #-64]
	b	.L121
.L282:
	.align	2
.L281:
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
	str	r8, [r0, #636]
	ldr	r3, [r1, #4]
	str	r3, [r0, #640]
	ldr	r3, [r1, #48]
	ldr	r2, [r1]
	cmp	r3, #8
	ldr	r7, [r1, #4]
	ldr	r3, [r1, #8]
	movne	r2, #90
	strneh	r2, [r0, #24]
	beq	.L306
.L285:
	ldr	r2, [r6, #64]
	cmp	r2, #8
	movne	r3, #60
	strneh	r3, [r5, #26]
	beq	.L307
.L287:
	ldr	r1, [r6, #56]
	add	r3, r1, #1
	cmp	r3, #1
	bls	.L288
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
.L288:
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
	ble	.L292
	cmp	r1, #1920
	movle	r1, #42
	movgt	r1, #63
	strh	r1, [r5, #32]
.L292:
	cmp	r2, #576
	movle	r2, #16
	strleh	r2, [r5, #36]
	ble	.L295
	movw	r1, #1080
	cmp	r2, r1
	movle	r2, #36
	movgt	r2, #63
	strh	r2, [r5, #36]
.L295:
	cmp	r3, #360
	movle	r3, #16
	strleh	r3, [r5, #34]
	ble	.L298
	cmp	r3, #960
	movle	r3, #42
	movgt	r3, #63
	strh	r3, [r5, #34]
.L298:
	mov	r2, #512
	mov	r1, #0
	add	r0, r5, #40
	add	r4, r4, #16
	bl	memset
	add	ip, r5, #36
	mov	lr, #0
.L300:
	cmp	lr, #2048
	ldr	r2, [ip, #4]!
	beq	.L304
	mov	r0, r4
	mov	r3, #1
	ldrb	r1, [r0, lr]!
	and	r1, r1, r3
	orr	r1, r1, r2
	str	r1, [ip]
	b	.L302
.L309:
	ldrb	r2, [r0, #1]!
	and	r2, r2, #1
	orr	r1, r1, r2, asl r3
	add	r3, r3, #1
	cmp	r3, #32
	str	r1, [ip]
	beq	.L308
.L302:
	add	r2, r3, lr
	cmp	r2, #2048
	bcc	.L309
.L304:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L308:
	add	lr, lr, #32
	b	.L300
.L306:
	add	r1, r2, #7
	cmp	r2, #0
	movlt	r2, r1
	mov	r2, r2, asr #3
	strh	r2, [r0, #24]
	b	.L285
.L307:
	add	r2, r3, #7
	cmp	r3, #0
	movlt	r3, r2
	mov	r3, r3, asr #3
	strh	r3, [r5, #26]
	b	.L287
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
