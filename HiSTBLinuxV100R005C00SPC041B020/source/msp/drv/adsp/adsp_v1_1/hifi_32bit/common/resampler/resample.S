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
	.file	"resample.c"
	.text
	.align	2
	.type	cvtShortShort, %function
cvtShortShort:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [fp, #4]
	sub	lr, ip, #2
	cmp	r3, #0
	cmpeq	ip, #2
	clz	lr, lr
	mov	lr, lr, lsr #5
	bne	.L2
	add	r2, r1, r2, lsl #1
	cmp	r1, r2
	beq	.L8
	add	r0, r0, #4
	sub	ip, r2, #2
	mov	r3, r1
.L4:
	ldrh	lr, [r3], #2
	add	r0, r0, #4
	cmp	r2, r3
	strh	lr, [r0, #-8]
	ldrh	lr, [ip, #2]!
	strh	lr, [r0, #-6]
	bne	.L4
	add	r0, r1, #2
	rsb	r0, r0, r2
	mov	r0, r0, lsr #1
	add	r0, r0, #1
	mov	r0, r0, asl #2
	mov	r0, r0, asr #1
	ldmfd	sp, {fp, sp, pc}
.L2:
	cmp	lr, #0
	movne	r2, r2, asl #1
	add	r2, r1, r2, lsl #1
	cmp	r1, r2
	beq	.L8
	mov	r3, r1
.L6:
	ldrh	ip, [r3], #2
	cmp	r2, r3
	strh	ip, [r0], #2
	bne	.L6
	add	r0, r1, #2
	rsb	r0, r0, r2
	bic	r0, r0, #1
	add	r0, r0, #2
	mov	r0, r0, asr #1
	ldmfd	sp, {fp, sp, pc}
.L8:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	cvtShortShort, .-cvtShortShort
	.align	2
	.type	cvt2int, %function
cvt2int:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [fp, #8]
	ldr	lr, [fp, #4]
	sub	r4, ip, #2
	clz	r4, r4
	cmp	lr, #0
	cmpeq	ip, #2
	mov	r4, r4, lsr #5
	bne	.L16
	cmp	r3, #16
	beq	.L36
	add	r2, r1, r2, lsl #2
	cmp	r1, r2
	beq	.L28
	add	r3, r0, #8
	sub	lr, r2, #4
	mov	ip, r1
.L20:
	ldr	r0, [ip], #4
	add	r3, r3, #8
	cmp	r2, ip
	str	r0, [r3, #-16]
	ldr	r0, [lr, #4]!
	str	r0, [r3, #-12]
	bne	.L20
	add	r0, r1, #4
	rsb	r0, r0, r2
	mov	r0, r0, lsr #2
	add	r0, r0, #1
	mov	r0, r0, asl #3
	mov	r0, r0, asr #2
	ldmfd	sp, {r4, fp, sp, pc}
.L16:
	cmp	r4, #0
	movne	r2, r2, asl #1
	cmp	r3, #16
	beq	.L37
	add	r2, r1, r2, lsl #2
	cmp	r1, r2
	beq	.L28
	mov	ip, r1
.L24:
	ldr	r3, [ip], #4
	cmp	r2, ip
	str	r3, [r0], #4
	bne	.L24
	add	r0, r1, #4
	rsb	r0, r0, r2
	bic	r0, r0, #3
	add	r0, r0, #4
	mov	r0, r0, asr #2
	ldmfd	sp, {r4, fp, sp, pc}
.L37:
	add	r2, r1, r2, lsl #1
	cmp	r1, r2
	beq	.L28
	mov	lr, r1
.L23:
	ldrsh	ip, [lr], #2
	cmp	r2, lr
	mov	ip, ip, asl #16
	str	ip, [r0], #4
	bne	.L23
	add	r0, r1, #2
	rsb	r2, r0, r2
	mov	r0, r2, lsr #1
	add	r0, r0, #1
	sbfx	r0, r0, #0, #30
	ldmfd	sp, {r4, fp, sp, pc}
.L36:
	add	r2, r1, r2, lsl #1
	cmp	r1, r2
	beq	.L28
	add	r0, r0, #8
	sub	lr, r2, #2
	mov	ip, r1
.L19:
	ldrsh	r3, [ip], #2
	add	r0, r0, #8
	cmp	r2, ip
	mov	r3, r3, asl #16
	str	r3, [r0, #-16]
	ldrsh	r3, [lr, #2]!
	mov	r3, r3, asl #16
	str	r3, [r0, #-12]
	bne	.L19
	add	r0, r1, #2
	rsb	r2, r0, r2
	mov	r0, r2, lsr #1
	add	r0, r0, #1
	mov	r0, r0, asl #3
	mov	r0, r0, asr #2
	ldmfd	sp, {r4, fp, sp, pc}
.L28:
	mov	r0, #0
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	cvt2int, .-cvt2int
	.align	2
	.type	ResamplerMonoProcess24, %function
ResamplerMonoProcess24:
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
	mov	r5, r0
	mov	ip, #1
	str	r3, [fp, #-72]
	mov	r4, r3
	ldr	r3, [r0, #40]
	ldr	r0, [r0, #56]
	str	ip, [sp, #4]
	str	ip, [sp]
	ldr	ip, [r5, #28]
	ldr	r10, [r5, #20]
	str	ip, [fp, #-48]
	ldr	ip, [r5, #72]
	str	ip, [fp, #-60]
	bl	cvt2int
	ldr	r3, [r5, #56]
	mov	r2, r10, asl #2
	ldr	r8, [r5, #32]
	rsb	r2, r2, #4
	add	r3, r3, r2
	ldr	lr, [r5, #64]
	ldr	r6, [r5, #60]
	add	r8, r3, r8, lsl #2
	mov	r2, r0, asl #2
	str	r2, [fp, #-68]
	add	r2, r3, r2
	str	r2, [fp, #-56]
	cmp	r2, r8
	bls	.L56
	sub	r3, r10, #1
	add	r9, r4, #4
	mov	r3, r3, lsr #1
	add	r3, r3, #1
	mov	r3, r3, asl #2
	str	r3, [fp, #-64]
.L54:
	cmp	r10, #0
	ble	.L40
	ldr	r2, [fp, #-64]
	mov	r3, #0
	sub	r0, r8, #12
	add	r1, r8, #8
	add	r7, lr, r2
	mov	r2, r3
	str	r10, [fp, #-52]
.L41:
	ldr	r10, [lr], #4
	ldr	r4, [r6], #4
	ldr	ip, [r0, #8]
#APP
	smlawb r2, ip, r10, r2
	ldr	ip, [r1, #-8]
#APP
	smlawb r3, ip, r4, r3
	ldr	ip, [r0, #4]
#APP
	smlawt r2, ip, r10, r2
	ldr	r10, [r1, #-4]
#APP
	smlawt r3, r10, r4, r3
	cmp	lr, r7
	sub	r0, r0, #8
	add	r1, r1, #8
	bne	.L41
	ldr	r1, [r5, #40]
	add	r3, r3, r2
	ldr	r10, [fp, #-52]
	cmp	r1, #16
	beq	.L63
	cmp	r1, #18
	beq	.L64
.L46:
	cmp	r1, #20
	beq	.L65
	add	r3, r3, #64
	mov	r1, r3, asr #31
	mov	r2, r3, asr #7
	cmp	r1, r3, asr #30
	biceq	r3, r2, #-16777216
	movwne	r3, #65535
	movtne	r3, 127
	eorne	r3, r3, r1
	mov	r1, r9
.L61:
	str	r3, [r9, #-4]
.L45:
	ldr	r2, [fp, #-48]
	add	r9, r9, #4
	ldr	r3, [fp, #-60]
	ldr	ip, [r3, r2, asl #2]
	ldr	r2, [r5, #68]
	ubfx	r6, ip, #8, #12
	ubfx	r0, ip, #1, #7
	mov	r3, ip, lsr #20
	mov	ip, ip, asr #20
	mul	r6, r10, r6
	add	r8, r8, r0, lsl #2
	mul	lr, r10, r3
	ldr	r3, [fp, #-56]
	str	ip, [fp, #-48]
	cmp	r3, r8
	add	r6, r2, r6, lsl #1
	add	lr, r2, lr, lsl #1
	bhi	.L54
	ldr	r3, [fp, #-72]
	rsb	r1, r3, r1
	mov	r4, r1, asr #2
.L39:
	ldr	r1, [fp, #-48]
	ldr	r3, [fp, #-56]
	ldr	ip, .L66
	ldr	r2, [r5, #24]
	rsb	r3, r3, r8
	str	r1, [r5, #28]
	ldr	r0, [r5, #52]
	mov	r3, r3, asr #2
	ldr	r1, [fp, #-68]
	mov	r2, r2, asl #2
	str	r3, [r5, #32]
	add	r1, r0, r1
	str	lr, [r5, #64]
	str	r6, [r5, #60]
	ldr	r3, [ip, #28]
	blx	r3
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L63:
	add	r3, r3, #16384
	mov	r1, r9
	mov	r2, r3, asr #31
	mov	r0, r3, asr #15
	cmp	r2, r3, asr #30
	eorne	r3, r2, #32512
	sxtheq	r3, r0
	eorne	r3, r3, #255
	sxthne	r3, r3
	b	.L61
.L64:
	add	r3, r3, #4096
	mov	r1, r3, asr #31
	mov	r2, r3, asr #13
	cmp	r1, r3, asr #30
	beq	.L47
	movw	r3, #65535
	movt	r3, 1
	eor	r3, r3, r1
.L51:
	str	r3, [r9, #-4]
	mov	r1, r9
	b	.L45
.L65:
	add	r3, r3, #1024
	mov	r1, r3, asr #31
	mov	r2, r3, asr #11
	cmp	r1, r3, asr #30
	ubfxeq	r3, r2, #0, #20
	movwne	r3, #65535
	movtne	r3, 7
	eorne	r3, r3, r1
	b	.L51
.L40:
	ldr	r1, [r5, #40]
	cmp	r1, #16
	beq	.L55
	cmp	r1, #18
	movne	r3, #0
	bne	.L46
	mov	r2, #0
.L47:
	ubfx	r3, r2, #0, #18
	b	.L51
.L56:
	mov	r4, #0
	b	.L39
.L55:
	mov	r1, r9
	mov	r3, #0
	b	.L61
.L67:
	.align	2
.L66:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ResamplerMonoProcess24, .-ResamplerMonoProcess24
	.align	2
	.type	ResamplerStereoProcess24, %function
ResamplerStereoProcess24:
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
	ldr	r4, [fp, #4]
	mov	r5, r0
	ldr	lr, [r0, #40]
	str	r0, [fp, #-72]
	mov	ip, #2
	ldr	r0, [r0, #56]
	mov	r6, r3
	str	r4, [sp]
	str	ip, [sp, #4]
	ldr	ip, [r5, #72]
	str	r3, [fp, #-76]
	mov	r3, lr
	ldr	r4, [r5, #20]
	ldr	r10, [r5, #28]
	str	ip, [fp, #-52]
	bl	cvt2int
	mov	ip, r5
	ldr	r2, [r5, #56]
	ldr	r1, [ip, #68]
	sub	r3, r4, #1
	ldr	r7, [r5, #32]
	sub	r2, r2, r3, asl #3
	ldr	lr, [r5, #64]
	str	r1, [fp, #-60]
	add	r7, r2, r7, lsl #2
	ldr	r5, [r5, #60]
	mov	r1, r0, asl #2
	str	r1, [fp, #-68]
	add	r2, r2, r1
	str	r2, [fp, #-56]
	cmp	r2, r7
	bls	.L77
	mov	r3, r3, lsr #1
	add	r8, r6, #8
	add	r3, r3, #1
	mov	r3, r3, asl #2
	str	r3, [fp, #-64]
.L76:
	cmp	r4, #0
	ble	.L78
	ldr	r3, [fp, #-64]
	mov	r9, #0
	sub	r1, r7, #20
	add	r2, r7, #16
	add	r6, lr, r3
	mov	r3, r9
	str	r4, [fp, #-48]
.L71:
	ldr	ip, [lr], #4
	ldr	r0, [r5], #4
	ldr	r4, [r1, #16]
#APP
	smlawb r3, r4, ip, r3
	ldr	r4, [r2, #-16]
#APP
	smlawb r9, r4, r0, r9
	ldr	r4, [r1, #12]
#APP
	smlawb r9, r4, ip, r9
	ldr	r4, [r2, #-12]
#APP
	smlawb r3, r4, r0, r3
	ldr	r4, [r1, #8]
#APP
	smlawt r3, r4, ip, r3
	ldr	r4, [r2, #-8]
#APP
	smlawt r9, r4, r0, r9
	ldr	r4, [r1, #4]
#APP
	smlawt r9, r4, ip, r9
	ldr	ip, [r2, #-4]
#APP
	smlawt r3, ip, r0, r3
	cmp	lr, r6
	sub	r1, r1, #16
	add	r2, r2, #16
	bne	.L71
	add	r9, r9, #64
	ldr	r4, [fp, #-48]
	mov	r2, r9, asr #31
	mov	r1, r9, asr #7
	cmp	r2, r9, asr #30
	beq	.L81
	movw	r1, #65535
	add	r3, r3, #64
	movt	r1, 127
	eor	r1, r1, r2
	mov	r0, r3, asr #31
	mov	r2, r3, asr #7
	mov	r1, r1, asl #8
	mov	r3, r3, asr #30
.L73:
	cmp	r0, r3
	str	r1, [r8, #-8]
	biceq	r2, r2, #-16777216
	mov	r3, r8
	movwne	r2, #65535
	add	r8, r8, #8
	movtne	r2, 127
	eorne	r2, r2, r0
	mov	r2, r2, asl #8
	str	r2, [r8, #-12]
	ldr	r2, [fp, #-52]
	ldr	r2, [r2, r10, asl #2]
	ubfx	r5, r2, #8, #12
	mov	lr, r2, lsr #20
	uxtb	r1, r2
	mul	r5, r5, r4
	add	r7, r7, r1, lsl #2
	mul	lr, lr, r4
	ldr	r1, [fp, #-56]
	mov	r10, r2, asr #20
	ldr	r2, [fp, #-60]
	cmp	r1, r7
	add	r5, r2, r5, lsl #1
	add	lr, r2, lr, lsl #1
	bhi	.L76
	ldr	r2, [fp, #-76]
	rsb	r3, r2, r3
	mov	r3, r3, asr #2
	add	r3, r3, r3, lsr #31
	mov	r4, r3, asr #1
.L69:
	ldr	r6, [fp, #-72]
	mov	ip, #0
	ldr	r3, .L82
	ldr	r1, [fp, #-68]
	ldr	r2, [r6, #24]
	ldr	r0, [r6, #52]
	str	r10, [r6, #28]
	add	r1, r0, r1
	str	lr, [r6, #64]
	str	r5, [r6, #60]
	mov	r2, r2, asl #2
	ldr	r3, [r3, #28]
	str	ip, [r6, #32]
	blx	r3
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L81:
	add	r3, r3, #64
	mov	r1, r1, asl #8
	mov	r2, r3, asr #7
	mov	r0, r3, asr #31
	mov	r3, r3, asr #30
	b	.L73
.L78:
	mov	r3, #0
	mov	r0, r3
	mov	r2, r3
	mov	r1, r3
	b	.L73
.L77:
	mov	r4, #0
	b	.L69
.L83:
	.align	2
.L82:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ResamplerStereoProcess24, .-ResamplerStereoProcess24
	.global	__aeabi_idiv
	.align	2
	.global	ResampleConfig
	.type	ResampleConfig, %function
ResampleConfig:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r1, r2
	mov	r4, r1
	mov	r8, r2
	mov	r7, r0
	beq	.L97
	mov	r3, r2
	mov	r5, r1
.L88:
	cmp	r5, r3
	rsbgt	r5, r3, r5
	rsble	r3, r5, r3
	cmp	r3, r5
	bne	.L88
.L85:
	mov	r1, r5
	mov	r0, r8
	bl	__aeabi_idiv
	mov	r1, r5
	mov	r9, r0
	mov	r0, r4
	bl	__aeabi_idiv
	cmp	r9, #1280
	mov	r10, r0
	bgt	.L100
	ldr	r3, [r7, #16]
	add	r0, r0, r9
	mov	r1, r9
	sub	r0, r0, #1
	mov	r5, r3
	str	r3, [fp, #-48]
	bl	__aeabi_idiv
	mul	r0, r0, r5
	cmp	r0, #255
	bgt	.L100
	cmp	r4, #0
	add	r5, r4, #3
	add	r2, r8, #3
	add	r6, r4, r4, lsr #31
	movge	r5, r4
	cmp	r8, #0
	add	r0, r8, r8, lsr #31
	ldr	ip, .L109
	movge	r2, r8
	mov	r5, r5, asr #2
	mov	r2, r2, asr #2
	mov	r6, r6, asr #1
	mov	r0, r0, asr #1
	mov	lr, #0
	b	.L94
.L90:
	cmp	r3, r6
	beq	.L107
	cmp	r3, r5
	beq	.L108
.L93:
	add	lr, lr, #1
	add	ip, ip, #24
	cmp	lr, #40
	beq	.L100
.L94:
	ldr	r3, [ip]
	cmp	r3, r4
	bne	.L90
	ldr	r1, [ip, #4]
	cmp	r1, r8
	bne	.L90
.L91:
	cmp	lr, #40
	beq	.L100
	ldr	lr, [ip, #20]
	mov	r0, #0
	ldr	r5, [ip, #12]
	ldr	r3, [r7, #40]
	str	lr, [r7, #68]
	ldr	r1, [ip, #16]
	mov	r2, r5, asl #1
	str	r0, [r7, #36]
	cmp	r3, #16
	ldr	r0, [fp, #-48]
	str	r1, [r7, #72]
	mul	r2, r2, r0
	beq	.L106
	sub	r1, r3, #18
	bics	r1, r1, #2
	moveq	r1, #1
	movne	r1, #0
	cmp	r3, #24
	movne	r3, r1
	orreq	r3, r1, #1
	cmp	r3, #0
	bne	.L106
.L96:
	sub	r3, r9, #1
	mov	r1, #0
	mov	r0, r1
	str	r4, [r7, #8]
	mul	r3, r3, r5
	str	r8, [r7, #12]
	stmia	r7, {r9, r10}
	str	r5, [r7, #20]
	str	r2, [r7, #24]
	str	lr, [r7, #64]
	add	r3, lr, r3, lsl #1
	str	r1, [r7, #28]
	str	r3, [r7, #60]
	str	r1, [r7, #32]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L107:
	ldr	r1, [ip, #4]
	cmp	r1, r0
	beq	.L91
	cmp	r3, r5
	bne	.L93
.L108:
	ldr	r3, [ip, #4]
	cmp	r3, r2
	beq	.L91
	add	lr, lr, #1
	add	ip, ip, #24
	cmp	lr, #40
	bne	.L94
.L100:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L106:
	ldr	r3, [r7, #52]
	add	r3, r3, r2, lsl #2
	str	r3, [r7, #56]
	b	.L96
.L97:
	mov	r5, r1
	b	.L85
.L110:
	.align	2
.L109:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	ResampleConfig, .-ResampleConfig
	.align	2
	.global	ResamplerCreate
	.type	ResamplerCreate, %function
ResamplerCreate:
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
	cmp	r0, #32000
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	beq	.L113
	ble	.L185
	movw	r2, #22664
	movt	r2, 1
	cmp	r0, r2
	beq	.L113
	ble	.L186
	movw	r2, #45328
	movt	r2, 2
	cmp	r0, r2
	beq	.L113
	mov	r2, #60928
	movt	r2, 2
	cmp	r0, r2
	beq	.L113
	mov	r2, #30464
	movt	r2, 1
	cmp	r0, r2
	beq	.L113
.L112:
	sub	r0, fp, #48
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r5
	bl	LSRC_Create
	ldr	r0, [fp, #-48]
	cmp	r0, #0
	beq	.L138
	mov	r1, #88
	mov	r0, #26
	bl	HI_ADSP_MALLOC
	subs	r4, r0, #0
	beq	.L187
	ldr	r3, [fp, #-48]
	mov	r0, r4
	mov	r2, #1
	str	r2, [r4, #80]
	str	r3, [r4, #84]
.L138:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L193:
	cmp	r0, #8000
	beq	.L113
	movw	r2, #11025
	cmp	r0, r2
	bne	.L112
.L113:
	cmp	r6, #32000
	beq	.L117
	ble	.L188
	movw	r2, #22664
	movt	r2, 1
	cmp	r6, r2
	beq	.L117
	ble	.L189
	movw	r2, #45328
	movt	r2, 2
	cmp	r6, r2
	beq	.L117
	mov	r2, #60928
	movt	r2, 2
	cmp	r6, r2
	beq	.L117
	mov	r2, #30464
	movt	r2, 1
	cmp	r6, r2
	bne	.L112
.L117:
	cmp	r5, r6
	bge	.L112
	sub	r2, r7, #1
	cmp	r2, #1
	bhi	.L112
	bic	r2, r3, #2
	cmp	r2, #16
	beq	.L142
	sub	r2, r3, #20
	bics	r2, r2, #4
	bne	.L112
.L142:
	mov	r4, r6
	mov	r2, r5
.L125:
	cmp	r2, r4
	rsbgt	r2, r4, r2
	rsble	r4, r2, r4
	cmp	r4, r2
	bne	.L125
	mov	r1, r4
	mov	r0, r6
	str	r3, [fp, #-56]
	bl	__aeabi_idiv
	mov	r1, r4
	mov	r8, r0
	mov	r0, r5
	bl	__aeabi_idiv
	cmp	r8, #1280
	ldr	r3, [fp, #-56]
	str	r0, [fp, #-60]
	bgt	.L112
	add	r0, r8, r0
	mov	r1, r8
	sub	r0, r0, #1
	bl	__aeabi_idiv
	ldr	r3, [fp, #-56]
	mul	r0, r0, r7
	cmp	r0, #255
	bgt	.L112
	cmp	r5, #0
	add	r2, r5, #3
	add	r9, r6, #3
	add	r1, r5, r5, lsr #31
	movge	r2, r5
	cmp	r6, #0
	add	r10, r6, r6, lsr #31
	ldr	r4, .L196
	movge	r9, r6
	mov	r2, r2, asr #2
	mov	r9, r9, asr #2
	mov	r1, r1, asr #1
	mov	r10, r10, asr #1
	mov	lr, #0
	b	.L130
.L126:
	cmp	r0, r1
	beq	.L190
.L128:
	cmp	r0, r2
	beq	.L191
.L129:
	add	lr, lr, #1
	add	r4, r4, #24
	cmp	lr, #40
	beq	.L112
.L130:
	ldr	r0, [r4]
	cmp	r5, r0
	bne	.L126
	ldr	ip, [r4, #4]
	cmp	r6, ip
	bne	.L126
	ldr	ip, [r4, #8]
	cmp	ip, #2
	bne	.L126
.L127:
	cmp	lr, #40
	beq	.L112
	mov	r1, #88
	mov	r0, #26
	str	r3, [fp, #-56]
	ldr	r10, [r4, #12]
	bl	HI_ADSP_MALLOC
	ldr	r3, [fp, #-56]
	mov	r9, r10, asl #1
	mul	r9, r9, r7
	subs	ip, r0, #0
	beq	.L112
	ldr	r0, [r4, #20]
	cmp	r3, #16
	mov	r2, #0
	str	r0, [ip, #68]
	ldr	r1, [r4, #16]
	str	r3, [ip, #40]
	str	r2, [ip, #36]
	str	r1, [ip, #72]
	beq	.L192
	sub	r1, r3, #18
	bics	r1, r1, #2
	moveq	r1, #1
	movne	r1, #0
	cmp	r3, #24
	orreq	r1, r1, #1
	cmp	r1, #0
	moveq	r2, r0
	bne	.L184
.L135:
	sub	r3, r8, #1
	ldr	r0, [fp, #-60]
	mov	r1, #0
	str	r5, [ip, #8]
	mul	r3, r3, r10
	str	r0, [ip, #4]
	str	r6, [ip, #12]
	mov	r0, ip
	str	r8, [ip]
	str	r7, [ip, #16]
	str	r10, [ip, #20]
	add	r3, r2, r3, lsl #1
	str	r3, [ip, #60]
	ldr	r3, [fp, #4]
	str	r9, [ip, #24]
	str	r2, [ip, #64]
	str	r3, [ip, #76]
	str	r1, [ip, #28]
	str	r1, [ip, #32]
	str	r1, [ip, #80]
	str	r1, [ip, #84]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L185:
	movw	r2, #12000
	cmp	r0, r2
	beq	.L113
	ble	.L193
	movw	r2, #22050
	cmp	r0, r2
	beq	.L113
	movw	r2, #24000
	cmp	r0, r2
	beq	.L113
	cmp	r0, #16000
	bne	.L112
	b	.L113
.L188:
	movw	r2, #12000
	cmp	r6, r2
	beq	.L117
	ble	.L194
	movw	r2, #22050
	cmp	r6, r2
	beq	.L117
	movw	r2, #24000
	cmp	r6, r2
	beq	.L117
	cmp	r6, #16000
	bne	.L112
	b	.L117
.L190:
	ldr	ip, [r4, #4]
	cmp	ip, r10
	bne	.L128
	ldr	ip, [r4, #8]
	cmp	ip, #2
	beq	.L127
	cmp	r0, r2
	bne	.L129
.L191:
	ldr	r0, [r4, #4]
	cmp	r0, r9
	bne	.L129
	ldr	r0, [r4, #8]
	cmp	r0, #2
	bne	.L129
	b	.L127
.L186:
	movw	r2, #44100
	cmp	r0, r2
	beq	.L113
	movw	r2, #48000
	cmp	r0, r2
	beq	.L113
	b	.L112
.L194:
	cmp	r6, #8000
	beq	.L117
	movw	r2, #11025
	cmp	r6, r2
	beq	.L117
	b	.L112
.L189:
	movw	r2, #44100
	cmp	r6, r2
	beq	.L117
	movw	r2, #48000
	cmp	r6, r2
	beq	.L117
	b	.L112
.L192:
	ldr	r1, [fp, #4]
	cmp	r1, r2
	beq	.L195
.L184:
	mov	r1, #16640
	mov	r0, #26
	str	r2, [fp, #-68]
	str	r3, [fp, #-64]
	str	ip, [fp, #-56]
	bl	HI_ADSP_MALLOC
	ldr	ip, [fp, #-56]
	ldr	r3, [fp, #-64]
	ldr	r2, [fp, #-68]
	cmp	r0, #0
	str	r0, [ip, #52]
	beq	.L182
	ldr	r3, .L196+4
	mov	r4, r9, asl #2
	str	ip, [fp, #-56]
	mov	r1, r2
	mov	r2, r4
	ldr	r3, [r3, #20]
	blx	r3
	ldr	ip, [fp, #-56]
	ldr	r3, [ip, #52]
	ldr	r2, [ip, #68]
	add	r4, r3, r4
	str	r4, [ip, #56]
	b	.L135
.L195:
	add	r1, r9, #4096
	mov	r0, #26
	str	r3, [fp, #-64]
	mov	r1, r1, asl #1
	str	ip, [fp, #-56]
	bl	HI_ADSP_MALLOC
	ldr	ip, [fp, #-56]
	ldr	r3, [fp, #-64]
	cmp	r0, #0
	str	r0, [ip, #44]
	beq	.L182
	ldr	r3, .L196+4
	mov	r4, r9, asl #1
	ldr	r1, [fp, #4]
	mov	r2, r4
	str	ip, [fp, #-56]
	ldr	r3, [r3, #20]
	blx	r3
	ldr	ip, [fp, #-56]
	ldr	r3, [ip, #44]
	ldr	r2, [ip, #68]
	add	r4, r3, r4
	str	r4, [ip, #48]
	b	.L135
.L187:
	ldr	r0, [fp, #-48]
	bl	LSRC_Destroy
	mov	r0, r4
	b	.L138
.L182:
	mov	r1, ip
	mov	r0, #26
	str	r3, [fp, #-56]
	bl	HI_ADSP_FREE
	ldr	r3, [fp, #-56]
	b	.L112
.L197:
	.align	2
.L196:
	.word	.LANCHOR0
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ResamplerCreate, .-ResamplerCreate
	.align	2
	.global	ResamplerFree
	.type	ResamplerFree, %function
ResamplerFree:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, [r4, #80]
	cmp	r3, #0
	beq	.L220
	ldr	r0, [r4, #84]
	bl	LSRC_Destroy
.L217:
	mov	r1, r4
	mov	r0, #26
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	HI_ADSP_FREE
.L220:
	ldr	r3, [r4, #40]
	cmp	r3, #16
	beq	.L221
	sub	r2, r3, #18
	bics	r2, r2, #2
	moveq	r2, #1
	movne	r2, #0
	cmp	r3, #24
	movne	r3, r2
	orreq	r3, r2, #1
	cmp	r3, #0
	beq	.L217
.L219:
	ldr	r1, [r4, #52]
	cmp	r1, #0
	beq	.L217
.L216:
	mov	r0, #26
	bl	HI_ADSP_FREE
	b	.L217
.L221:
	ldr	r3, [r4, #76]
	cmp	r3, #0
	bne	.L219
	ldr	r1, [r4, #44]
	cmp	r1, #0
	bne	.L216
	b	.L217
	UNWIND(.fnend)
	.size	ResamplerFree, .-ResamplerFree
	.align	2
	.global	ResampleFlush
	.type	ResampleFlush, %function
ResampleFlush:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, [r4, #80]
	cmp	r1, #0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldr	r2, [r4]
	ldr	r0, [r4, #20]
	sub	r2, r2, #1
	ldr	ip, [r4, #40]
	ldr	r3, [r4, #68]
	mul	r2, r0, r2
	cmp	ip, #16
	str	r3, [r4, #64]
	str	r1, [r4, #28]
	str	r1, [r4, #32]
	str	r1, [r4, #36]
	add	r3, r3, r2, lsl #1
	str	r3, [r4, #60]
	beq	.L232
	sub	r3, ip, #18
	bics	r3, r3, #2
	moveq	r3, #1
	movne	r3, #0
	cmp	ip, #24
	movne	ip, r3
	orreq	ip, r3, #1
	cmp	ip, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r2, [r4, #24]
.L231:
	ldr	r3, .L233
	add	r2, r2, #4096
	ldr	r0, [r4, #52]
	mov	r2, r2, asl #2
	ldr	r3, [r3, #20]
	blx	r3
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #52]
	add	r3, r3, r2, lsl #2
	str	r3, [r4, #56]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L232:
	ldr	r3, [r4, #76]
	ldr	r2, [r4, #24]
	cmp	r3, #0
	bne	.L231
	mov	r1, r3
	ldr	r3, .L233
	add	r2, r2, #4096
	ldr	r0, [r4, #44]
	mov	r2, r2, asl #1
	ldr	r3, [r3, #20]
	blx	r3
	ldr	r2, [r4, #24]
	ldr	r3, [r4, #44]
	add	r3, r3, r2, lsl #1
	str	r3, [r4, #48]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L234:
	.align	2
.L233:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ResampleFlush, .-ResampleFlush
	.align	2
	.global	NEW_ResamplerCheck
	.type	NEW_ResamplerCheck, %function
NEW_ResamplerCheck:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r5, r3
	mov	r4, r1
	mov	r3, r2
	ldr	ip, [fp, #4]
	beq	.L238
	ldr	lr, [r0, #80]
	cmp	lr, #0
	bne	.L237
	ldr	r1, [r0, #8]
	cmp	r4, r1
	beq	.L242
.L241:
	mov	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L242:
	ldr	r2, [r0, #12]
	cmp	r3, r2
	bne	.L241
	ldr	r3, [r0, #16]
	cmp	r5, r3
	bne	.L241
	ldr	r0, [r0, #40]
	subs	r0, ip, r0
	movne	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L237:
	ldr	r0, [r0, #84]
	mov	r3, ip
	str	r5, [fp, #4]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	LSRC_CheckUpdate
.L238:
	mov	r0, #-2147483647
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	NEW_ResamplerCheck, .-NEW_ResamplerCheck
	.align	2
	.global	ResamplerGetMaxOutputNum
	.type	ResamplerGetMaxOutputNum, %function
ResamplerGetMaxOutputNum:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #0
	mov	r3, r1
	beq	.L246
	ldr	r2, [r0, #80]
	cmp	r2, #0
	bne	.L245
	cmp	r1, #2048
	bgt	.L248
	tst	r1, #1
	bne	.L248
	ldr	r2, [r0, #16]
	ldr	ip, [r0, #12]
	sub	r2, r2, #1
	ldr	r1, [r0, #8]
	add	r0, r3, r2
	mul	r0, ip, r0
	bl	__aeabi_idiv
	add	r0, r0, #1
	ldmfd	sp, {fp, sp, pc}
.L248:
	mov	r0, #-2147483645
	ldmfd	sp, {fp, sp, pc}
.L245:
	ldr	r0, [r0, #84]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	LSRC_GetMaxOutputNum
.L246:
	mov	r0, #-2147483647
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	ResamplerGetMaxOutputNum, .-ResamplerGetMaxOutputNum
	.global	__aeabi_idivmod
	.align	2
	.global	ResamplerGetMinInputNum
	.type	ResamplerGetMinInputNum, %function
ResamplerGetMinInputNum:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r5, r0, #0
	mov	r4, r1
	beq	.L254
	ldr	r3, [r5, #80]
	cmp	r3, #0
	bne	.L251
	ldr	r6, [r5, #8]
	ldr	r7, [r5, #12]
	mul	r6, r6, r1
	mov	r1, r7
	mov	r0, r6
	bl	__aeabi_idiv
	mov	r1, r7
	mov	r4, r0
	mov	r0, r6
	bl	__aeabi_idivmod
	ldr	r3, [r5, #16]
	cmp	r1, #0
	addne	r4, r4, #1
	mul	r3, r3, r4
	tst	r3, #1
	addne	r4, r4, #1
	cmp	r4, #2048
	movle	r0, r4
	movgt	r0, #-2147483645
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L251:
	ldr	r0, [r5, #84]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	LSRC_GetMinInputNum
.L254:
	mov	r0, r1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	ResamplerGetMinInputNum, .-ResamplerGetMinInputNum
	.align	2
	.global	ResamplerProcessFrame
	.type	ResamplerProcessFrame, %function
ResamplerProcessFrame:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #52)
	sub	sp, sp, #52
	subs	r5, r0, #0
	mov	r8, r1
	str	r2, [fp, #-48]
	mov	r6, r3
	ldmib	fp, {ip, lr}
	ldr	r7, [fp, #12]
	beq	.L271
	ldr	r0, [r5, #80]
	cmp	r0, #0
	beq	.L306
	ldr	r0, [r5, #84]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	LSRC_ProcessFrame
.L306:
	cmp	r1, #0
	beq	.L291
	cmp	r3, #2048
	bgt	.L267
	tst	r3, #1
	bne	.L267
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L291
	ldr	r3, [r5, #8]
	cmp	ip, r3
	beq	.L307
.L268:
	mov	r2, lr
	mov	r1, ip
	mov	r0, r5
	bl	ResampleConfig
.L269:
	mov	r1, r6
	mov	r0, r5
	bl	ResamplerGetMaxOutputNum
	cmp	r0, #0
	blt	.L308
	ldr	ip, [r5, #16]
	cmp	ip, #2
	beq	.L309
	cmp	ip, #1
	bne	.L293
	ldr	r3, [r5, #40]
	cmp	r3, #16
	beq	.L310
	sub	r2, r3, #18
	bics	r2, r2, #2
	moveq	r2, #1
	movne	r2, #0
	cmp	r3, #24
	movne	r3, r2
	orreq	r3, r2, #1
	cmp	r3, #0
	beq	.L305
.L283:
	ldr	r3, [fp, #-48]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	ResamplerMonoProcess24
.L307:
	ldr	r3, [r5, #12]
	cmp	lr, r3
	bne	.L268
	b	.L269
.L309:
	ldr	r3, [r5, #40]
	cmp	r3, #16
	beq	.L311
	sub	r2, r3, #18
	bics	r2, r2, #2
	moveq	r2, #1
	movne	r2, #0
	cmp	r3, #24
	movne	r3, r2
	orreq	r3, r2, #1
	cmp	r3, #0
	beq	.L305
.L274:
	str	r7, [fp, #4]
	mov	r2, r6
	ldr	r3, [fp, #-48]
	mov	r1, r8
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	ResamplerStereoProcess24
.L308:
	cmp	r0, #-2147483647
	beq	.L271
	cmp	r0, #-2147483645
	movne	r4, #-2147483640
	beq	.L267
.L304:
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L267:
	mov	r4, #-2147483645
	b	.L304
.L305:
	mov	r4, #-2147483642
	b	.L304
.L310:
	ldr	r4, [r5, #76]
	cmp	r4, #0
	bne	.L283
	ldr	lr, [r5, #28]
	mov	r2, r6
	mov	r3, r7
	ldr	r0, [r5, #48]
	mov	r1, r8
	str	lr, [fp, #-52]
	ldr	lr, [r5, #20]
	mov	r6, lr
	ldr	lr, [r5, #72]
	str	ip, [sp]
	str	lr, [fp, #-64]
	bl	cvtShortShort
	mov	r2, r6, asl #1
	ldr	r3, [r5, #48]
	ldr	r8, [r5, #32]
	rsb	r2, r2, #2
	add	r3, r3, r2
	mov	lr, r6
	ldr	r7, [r5, #60]
	add	r8, r3, r8, lsl #1
	ldr	r6, [r5, #64]
	mov	r2, r0, asl #1
	str	r2, [fp, #-72]
	add	r3, r3, r2
	str	r3, [fp, #-56]
	cmp	r3, r8
	bls	.L284
	sub	r3, lr, #1
	ldr	r1, [fp, #-48]
	movw	r2, #65526
	str	r5, [fp, #-60]
	mov	r3, r3, lsr #1
	add	r9, r1, #2
	mov	r5, lr
	movt	r2, 65535
	mov	r3, r3, asl #2
	rsb	r3, r3, r2
	str	r3, [fp, #-68]
.L289:
	cmp	r5, #0
	ble	.L285
	ldr	r2, [fp, #-68]
	mov	r3, #0
	sub	r1, r8, #6
	add	r0, r8, #4
	add	r4, r8, r2
	mov	r2, r3
.L286:
	ldr	ip, [r6], #4
	ldr	lr, [r7], #4
	ldrsh	r10, [r1, #4]
#APP
	smlabb r2, r10, ip, r2
	ldrsh	r10, [r0, #-4]
#APP
	smlabb r3, r10, lr, r3
	ldrsh	r10, [r1, #2]
#APP
	smlabt r2, r10, ip, r2
	ldrsh	ip, [r0, #-2]
#APP
	smlabt r3, ip, lr, r3
	sub	r1, r1, #4
	add	r0, r0, #4
	cmp	r1, r4
	bne	.L286
	add	r3, r3, r2
	mov	r1, r9
	add	r3, r3, #16384
	mov	r2, r3, asr #31
	mov	r0, r3, asr #15
	cmp	r2, r3, asr #30
	eorne	r3, r2, #32512
	uxtheq	r3, r0
	eorne	r3, r3, #255
	uxthne	r3, r3
.L288:
	strh	r3, [r9, #-2]
	add	r9, r9, #2
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-64]
	ldr	r3, [r3, r2, asl #2]
	ldr	r2, [fp, #-60]
	ubfx	r7, r3, #8, #12
	mov	r6, r3, lsr #20
	ldr	r0, [r2, #68]
	and	r2, r3, #254
	add	r8, r8, r2
	ldr	r2, [fp, #-56]
	mul	r7, r7, r5
	cmp	r2, r8
	mul	r2, r6, r5
	mov	r3, r3, asr #20
	str	r3, [fp, #-52]
	add	r7, r0, r7, lsl #1
	add	r6, r0, r2, lsl #1
	bhi	.L289
	ldr	r3, [fp, #-48]
	ldr	r5, [fp, #-60]
	rsb	r1, r3, r1
	mov	r4, r1, asr #1
.L284:
	ldr	r1, [fp, #-52]
	ldr	r3, [fp, #-56]
	ldr	ip, .L312
	ldr	r2, [r5, #24]
	rsb	r3, r3, r8
	str	r1, [r5, #28]
	ldr	r0, [r5, #44]
	mov	r3, r3, asr #1
	ldr	r1, [fp, #-72]
	mov	r2, r2, asl #1
	str	r3, [r5, #32]
	add	r1, r0, r1
	str	r6, [r5, #64]
	str	r7, [r5, #60]
	ldr	r3, [ip, #28]
	blx	r3
	b	.L304
.L285:
	mov	r1, r9
	mov	r3, #0
	b	.L288
.L311:
	ldr	r4, [r5, #76]
	cmp	r4, #0
	bne	.L274
	ldr	lr, [r5, #28]
	mov	r2, r6
	mov	r3, r7
	ldr	r0, [r5, #48]
	mov	r1, r8
	str	lr, [fp, #-52]
	ldr	lr, [r5, #20]
	mov	r6, lr
	str	lr, [fp, #-56]
	ldr	lr, [r5, #72]
	str	ip, [sp]
	str	lr, [fp, #-68]
	bl	cvtShortShort
	sub	r3, r6, #1
	ldr	r2, [r5, #48]
	ldr	r1, [r5, #32]
	sub	r2, r2, r3, asl #2
	ldr	r7, [r5, #64]
	ldr	r6, [r5, #60]
	add	r1, r2, r1, lsl #1
	mov	lr, r1
	mov	r0, r0, asl #1
	str	r0, [fp, #-76]
	add	r2, r2, r0
	str	r2, [fp, #-60]
	cmp	r2, r1
	bls	.L275
	ldr	r2, [fp, #-48]
	mov	r3, r3, lsr #1
	add	r3, r3, #1
	mov	r10, r1
	add	r2, r2, #4
	str	r4, [fp, #-80]
	mov	r3, r3, asl #2
	str	r5, [fp, #-64]
	mov	r9, r2
	str	r3, [fp, #-72]
.L281:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	ble	.L276
	ldr	r3, [fp, #-72]
	mov	r2, #0
	sub	r5, r10, #12
	add	r4, r10, #8
	add	r8, r7, r3
	mov	r1, r2
.L277:
	ldr	ip, [r7], #4
	ldr	r0, [r6], #4
	ldr	r3, [r5, #8]
	ldr	lr, [r4, #-8]
#APP
	smlabb r1, r3, ip, r1
	smlatb r3, r3, ip, r2
	smlabb r1, lr, r0, r1
	smlatb r3, lr, r0, r3
	ldr	r2, [r5, #4]
	ldr	lr, [r4, #-4]
#APP
	smlabt r1, r2, ip, r1
	smlatt r3, r2, ip, r3
	smlabt r1, lr, r0, r1
	smlatt r2, lr, r0, r3
	cmp	r7, r8
	sub	r5, r5, #8
	add	r4, r4, #8
	bne	.L277
	add	r1, r1, #16384
	mov	r3, r1, asr #31
	mov	r0, r1, asr #15
	cmp	r3, r1, asr #30
	addeq	r2, r2, #16384
	eorne	r3, r3, #32512
	addne	r2, r2, #16384
	eorne	r3, r3, #255
	moveq	r1, r2, asr #31
	uxtheq	r3, r0
	movne	r1, r2, asr #31
	moveq	r0, r2, asr #30
	movne	r0, r2, asr #30
	uxthne	r3, r3
	cmp	r1, r0
	mov	r2, r2, asr #15
	eorne	r2, r1, #32512
	strh	r3, [r9, #-4]
	eorne	r2, r2, #255
	movne	r1, r9
	moveq	r1, r9
.L280:
	strh	r2, [r9, #-2]
	add	r9, r9, #4
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-68]
	ldr	ip, [fp, #-56]
	ldr	r3, [r3, r2, asl #2]
	ldr	r2, [fp, #-64]
	ubfx	r6, r3, #8, #12
	mov	r7, r3, lsr #20
	ldr	r0, [r2, #68]
	uxtb	r2, r3
	mul	r6, r6, ip
	add	r10, r10, r2, lsl #1
	mul	r2, r7, ip
	ldr	ip, [fp, #-60]
	mov	r3, r3, asr #20
	str	r3, [fp, #-52]
	cmp	ip, r10
	add	r6, r0, r6, lsl #1
	add	r7, r0, r2, lsl #1
	bhi	.L281
	ldr	r3, [fp, #-48]
	mov	lr, r10
	ldr	r5, [fp, #-64]
	rsb	r1, r3, r1
	mov	r4, r1, asr #1
	add	r4, r4, r4, lsr #31
	mov	r4, r4, asr #1
.L275:
	ldr	r2, [fp, #-60]
	ldr	r1, [fp, #-52]
	rsb	r3, r2, lr
	ldr	ip, .L312
	ldr	r2, [r5, #24]
	str	r1, [r5, #28]
	mov	r3, r3, asr #1
	ldr	r0, [r5, #44]
	ldr	r1, [fp, #-76]
	mov	r2, r2, asl #1
	str	r3, [r5, #32]
	add	r1, r0, r1
	str	r7, [r5, #64]
	str	r6, [r5, #60]
	ldr	r3, [ip, #28]
	blx	r3
	b	.L304
.L293:
	mov	r4, #-2147483643
	b	.L304
.L276:
	ldrh	r3, [fp, #-80]
	mov	r1, r9
	mov	r2, #0
	strh	r3, [r9, #-4]
	b	.L280
.L271:
	mov	r4, #-2147483647
	b	.L304
.L291:
	mov	r4, #-2147483646
	b	.L304
.L313:
	.align	2
.L312:
	.word	g_AdspOsalFunc
	UNWIND(.fnend)
	.size	ResamplerProcessFrame, .-ResamplerProcessFrame
	.data
	.align	2
.LANCHOR0 = . + 0
	.type	resampler_info, %object
	.size	resampler_info, 960
resampler_info:
	.word	32000
	.word	48000
	.word	1
	.word	8
	.word	SRC_res_32_48_mid_nextphasetab
	.word	SRC_res_32_48_mid_filttab
	.word	44100
	.word	48000
	.word	1
	.word	8
	.word	SRC_res_44_48_mid_nextphasetab
	.word	SRC_res_44_48_mid_filttab
	.word	48000
	.word	48000
	.word	1
	.word	8
	.word	res_48_48_nextphasetab
	.word	res_48_48_good_filttab
	.word	8000
	.word	11025
	.word	2
	.word	8
	.word	SRC_res_8_11_good_nextphasetab
	.word	SRC_res_8_11_mid_filttab
	.word	16000
	.word	22050
	.word	2
	.word	8
	.word	SRC_res_8_11_good_nextphasetab
	.word	SRC_res_8_11_mid_filttab
	.word	8000
	.word	22050
	.word	2
	.word	8
	.word	SRC_res_8_22_good_nextphasetab
	.word	SRC_res_8_22_mid_filttab
	.word	16000
	.word	44100
	.word	2
	.word	8
	.word	SRC_res_8_22_good_nextphasetab
	.word	SRC_res_8_22_mid_filttab
	.word	8000
	.word	44100
	.word	2
	.word	8
	.word	SRC_res_8_44_good_nextphasetab
	.word	SRC_res_8_44_mid_filttab
	.word	11025
	.word	12000
	.word	2
	.word	8
	.word	SRC_res_11_12_good_nextphasetab
	.word	SRC_res_11_12_mid_filttab
	.word	22050
	.word	24000
	.word	2
	.word	8
	.word	SRC_res_11_12_good_nextphasetab
	.word	SRC_res_11_12_mid_filttab
	.word	11025
	.word	16000
	.word	2
	.word	8
	.word	SRC_res_11_16_good_nextphasetab
	.word	SRC_res_11_16_mid_filttab
	.word	22050
	.word	32000
	.word	2
	.word	8
	.word	SRC_res_11_16_good_nextphasetab
	.word	SRC_res_11_16_mid_filttab
	.word	11025
	.word	24000
	.word	2
	.word	8
	.word	SRC_res_11_24_good_nextphasetab
	.word	SRC_res_11_24_mid_filttab
	.word	22050
	.word	48000
	.word	2
	.word	8
	.word	SRC_res_11_24_good_nextphasetab
	.word	SRC_res_11_24_mid_filttab
	.word	11025
	.word	32000
	.word	2
	.word	8
	.word	SRC_res_11_32_good_nextphasetab
	.word	SRC_res_11_32_mid_filttab
	.word	11025
	.word	48000
	.word	2
	.word	8
	.word	SRC_res_11_48_good_nextphasetab
	.word	SRC_res_11_48_mid_filttab
	.word	12000
	.word	22050
	.word	2
	.word	8
	.word	SRC_res_12_22_good_nextphasetab
	.word	SRC_res_12_22_mid_filttab
	.word	24000
	.word	44100
	.word	2
	.word	8
	.word	SRC_res_12_22_good_nextphasetab
	.word	SRC_res_12_22_mid_filttab
	.word	12000
	.word	44100
	.word	2
	.word	8
	.word	SRC_res_12_44_good_nextphasetab
	.word	SRC_res_12_44_mid_filttab
	.word	8000
	.word	12000
	.word	2
	.word	16
	.word	SRC_res_8_12_good_nextphasetab
	.word	SRC_res_8_12_good_filttab
	.word	8000
	.word	16000
	.word	2
	.word	16
	.word	SRC_res_8_16_good_nextphasetab
	.word	SRC_res_8_16_good_filttab
	.word	8000
	.word	24000
	.word	2
	.word	16
	.word	SRC_res_8_24_good_nextphasetab
	.word	SRC_res_8_24_good_filttab
	.word	8000
	.word	32000
	.word	2
	.word	16
	.word	SRC_res_8_32_good_nextphasetab
	.word	SRC_res_8_32_good_filttab
	.word	8000
	.word	48000
	.word	2
	.word	16
	.word	SRC_res_8_48_good_nextphasetab
	.word	SRC_res_8_48_good_filttab
	.word	11025
	.word	22050
	.word	2
	.word	16
	.word	SRC_res_8_16_good_nextphasetab
	.word	SRC_res_8_16_good_filttab
	.word	11025
	.word	44100
	.word	2
	.word	16
	.word	SRC_res_8_32_good_nextphasetab
	.word	SRC_res_8_32_good_filttab
	.word	12000
	.word	16000
	.word	2
	.word	16
	.word	SRC_res_12_16_good_nextphasetab
	.word	SRC_res_12_16_good_filttab
	.word	12000
	.word	24000
	.word	2
	.word	16
	.word	SRC_res_8_16_good_nextphasetab
	.word	SRC_res_8_16_good_filttab
	.word	12000
	.word	32000
	.word	2
	.word	16
	.word	SRC_res_12_32_good_nextphasetab
	.word	SRC_res_12_32_good_filttab
	.word	12000
	.word	48000
	.word	2
	.word	16
	.word	SRC_res_8_32_good_nextphasetab
	.word	SRC_res_8_32_good_filttab
	.word	16000
	.word	24000
	.word	2
	.word	16
	.word	SRC_res_8_12_good_nextphasetab
	.word	SRC_res_8_12_good_filttab
	.word	16000
	.word	32000
	.word	2
	.word	16
	.word	SRC_res_8_16_good_nextphasetab
	.word	SRC_res_8_16_good_filttab
	.word	16000
	.word	48000
	.word	2
	.word	16
	.word	SRC_res_8_24_good_nextphasetab
	.word	SRC_res_8_24_good_filttab
	.word	22050
	.word	44100
	.word	2
	.word	16
	.word	SRC_res_8_16_good_nextphasetab
	.word	SRC_res_8_16_good_filttab
	.word	24000
	.word	32000
	.word	2
	.word	16
	.word	SRC_res_12_16_good_nextphasetab
	.word	SRC_res_12_16_good_filttab
	.word	24000
	.word	48000
	.word	2
	.word	16
	.word	SRC_res_8_16_good_nextphasetab
	.word	SRC_res_8_16_good_filttab
	.word	32000
	.word	44100
	.word	2
	.word	16
	.word	SRC_res_8_11_good_nextphasetab
	.word	SRC_res_8_11_good_filttab
	.word	32000
	.word	48000
	.word	2
	.word	16
	.word	SRC_res_8_12_good_nextphasetab
	.word	SRC_res_8_12_good_filttab
	.word	44100
	.word	48000
	.word	2
	.word	36
	.word	SRC_res_11_12_good_nextphasetab
	.word	SRC_res_11_12_good_filttab
	.word	48000
	.word	48000
	.word	2
	.word	8
	.word	res_48_48_nextphasetab
	.word	res_48_48_good_filttab
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
