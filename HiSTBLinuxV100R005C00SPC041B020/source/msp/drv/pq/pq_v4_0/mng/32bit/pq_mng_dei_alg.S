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
	.file	"pq_mng_dei_alg.c"
	.text
	.align	2
	.global	PQ_MNG_CopyU32ByBitWithSignal
	.type	PQ_MNG_CopyU32ByBitWithSignal, %function
PQ_MNG_CopyU32ByBitWithSignal:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [fp, #4]
	add	lr, r1, ip
	cmp	lr, #32
	ldmhifd	sp, {fp, sp, pc}
	add	r3, ip, r3
	cmp	r3, #32
	ldmhifd	sp, {fp, sp, pc}
	rsb	lr, ip, #32
	sub	r3, ip, #1
	rsb	r1, r1, lr
	mov	r0, r0, asl r1
	mov	r0, r0, lsr lr
	mov	r3, r0, lsr r3
	tst	r3, #1
	mvn	r3, #0
	eorne	r0, r0, r3, asl ip
	biceq	r0, r0, r3, asl ip
	str	r0, [r2]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	PQ_MNG_CopyU32ByBitWithSignal, .-PQ_MNG_CopyU32ByBitWithSignal
	.align	2
	.global	PQ_MNG_CopyU32ByBitWithoutSignal
	.type	PQ_MNG_CopyU32ByBitWithoutSignal, %function
PQ_MNG_CopyU32ByBitWithoutSignal:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [fp, #4]
	add	lr, r1, ip
	cmp	lr, #32
	ldmhifd	sp, {fp, sp, pc}
	add	r3, ip, r3
	cmp	r3, #32
	ldmhifd	sp, {fp, sp, pc}
	rsb	lr, ip, #32
	mvn	r3, #0
	rsb	r1, r1, lr
	mvn	ip, r3, asl ip
	mov	r1, r0, asl r1
	and	r1, ip, r1, lsr lr
	str	r1, [r2]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	PQ_MNG_CopyU32ByBitWithoutSignal, .-PQ_MNG_CopyU32ByBitWithoutSignal
	.global	__aeabi_idiv
	.align	2
	.global	PQ_MNG_ALG_API_GMD
	.type	PQ_MNG_ALG_API_GMD, %function
PQ_MNG_ALG_API_GMD:
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
	cmp	r1, #0
	cmpne	r2, #0
	mov	r5, r0
	mov	r4, r3
	mov	r7, r1
	beq	.L15
	add	r0, r2, r2, lsr #31
	mov	r1, r2
	mov	r0, r0, asr #1
	add	r0, r0, #1048576
	bl	__aeabi_idiv
	mov	r1, r7
	mov	r6, r0
	add	r0, r7, r7, lsr #31
	mov	r0, r0, asr #1
	add	r0, r0, #1048576
	bl	__aeabi_idiv
	ldr	r1, .L20
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bgt	.L19
.L9:
	ldr	r2, [r1, #48]
	ldr	r3, .L20
	cmp	r2, #3
	subgt	r2, r2, #4
	movle	r2, #0
	usatgt	r2, #4, r2
	str	r2, [r3, #48]
.L10:
	str	r2, [r4]
	mov	r3, r0, asr #31
	mov	r2, r0
	strd	r2, [fp, #-84]
	ldr	r3, [r5]
	sub	r7, fp, #60
	ldr	lr, [r1, #56]
	sub	ip, fp, #76
	ldr	r4, [r1, #52]
	sub	r0, fp, #64
	ldr	r2, [r5, #4]
	ldr	r10, [r5, #8]
	ldr	r8, [r1, #60]
	ldr	r5, [r5, #12]
	str	r3, [r1, #52]
	ldr	r3, [r1, #64]
	str	lr, [fp, #-72]
	movw	lr, #65535
	str	r8, [fp, #-68]
	str	r4, [fp, #-76]
	str	r2, [r1, #56]
	str	r10, [r1, #60]
	str	r5, [r1, #64]
	str	r3, [fp, #-64]
.L13:
	smull	r4, r5, r4, r6
	mov	r2, r4, lsr #12
	ldr	r4, [fp, #-80]
	mov	r3, r5, asr #12
	orr	r2, r2, r5, asl #20
	ldr	r5, [fp, #-84]
	mul	r4, r2, r4
	mla	r4, r3, r5, r4
	umull	r2, r3, r2, r5
	add	r3, r4, r3
	mov	r9, r3, asr #11
	mov	r8, r2, lsr #11
	orr	r8, r8, r3, asl #21
	adds	r2, r8, #1
	adc	r3, r9, #0
	movs	r3, r3, asr #1
	mov	r2, r2, rrx
	cmp	lr, r2
	movlt	r3, lr
	movge	r3, r2
	cmp	ip, r0
	str	r3, [r7], #4
	ldrne	r4, [ip, #4]!
	bne	.L13
.L12:
	ldr	r2, [r1, #72]
	ldr	r3, [r1, #76]
	ldr	r0, [r1, #80]
	mov	r2, r2, lsr #2
	add	r3, r2, r3, lsr #1
	add	r3, r3, r0
	cmp	r3, #3072
	movge	r0, #255
	bge	.L14
	cmp	r3, #1024
	movlt	r0, #0
	blt	.L14
	sub	r3, r3, #1024
	add	r0, r3, #7
	cmp	r3, #0
	movlt	r3, r0
	mov	r0, r3, asr #3
.L14:
	ldr	lr, [fp, #-60]
	ldr	ip, [fp, #-56]
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #-48]
	str	lr, [r1, #68]
	str	ip, [r1, #72]
	str	r2, [r1, #76]
	str	r3, [r1, #80]
.L8:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L19:
	ldr	r3, [r1]
	cmp	r3, #0
	bge	.L9
	ldr	r3, [r1, #8]
	cmp	r3, #0
	ble	.L9
	ldr	r2, [r1, #48]
	add	r2, r2, #1
	usat	r2, #4, r2
	str	r2, [r1, #48]
	b	.L10
.L15:
	mvn	r0, #0
	b	.L8
.L21:
	.align	2
.L20:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_API_GMD, .-PQ_MNG_ALG_API_GMD
	.align	2
	.global	PQ_MNG_ALG_API_GMD_P
	.type	PQ_MNG_ALG_API_GMD_P, %function
PQ_MNG_ALG_API_GMD_P:
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
	cmp	r1, #0
	cmpne	r2, #0
	mov	r5, r0
	mov	r4, r3
	mov	r7, r1
	beq	.L30
	add	r0, r2, r2, lsr #31
	mov	r1, r2
	mov	r0, r0, asr #1
	add	r0, r0, #1048576
	bl	__aeabi_idiv
	mov	r1, r7
	mov	r10, r0
	add	r0, r7, r7, lsr #31
	mov	r0, r0, asr #1
	add	r0, r0, #1048576
	bl	__aeabi_idiv
	ldr	ip, .L35
	ldr	r3, [ip, #92]
	ldr	r2, [ip, #84]
	mov	r1, r3
	rsb	r2, r1, r2
	cmp	r2, #0
	str	r3, [fp, #-80]
	ldr	r3, [ip, #16]
	rsblt	r2, r2, #0
	cmp	r2, r3
	ldr	r3, [ip, #88]
	str	r3, [fp, #-84]
	blt	.L34
.L25:
	mov	r3, #0
	str	r3, [ip, #96]
.L26:
	str	r3, [r4]
	sub	lr, fp, #60
	ldr	r4, [r5]
	sub	r1, fp, #76
	ldr	r3, [r5, #4]
	mov	r6, r0
	ldr	r2, [r5, #8]
	mov	r7, r0, asr #31
	ldr	r5, [r5, #12]
	movw	r0, #65535
	str	r4, [fp, #-76]
	str	r3, [fp, #-72]
	str	r2, [fp, #-68]
	str	r5, [fp, #-64]
.L28:
	smull	r4, r5, r4, r10
	mov	r2, r4, lsr #12
	mov	r3, r5, asr #12
	orr	r2, r2, r5, asl #20
	mul	r4, r2, r7
	mla	r4, r3, r6, r4
	umull	r2, r3, r2, r6
	add	r3, r4, r3
	mov	r9, r3, asr #11
	mov	r8, r2, lsr #11
	orr	r8, r8, r3, asl #21
	adds	r2, r8, #1
	adc	r3, r9, #0
	movs	r3, r3, asr #1
	mov	r2, r2, rrx
	cmp	r0, r2
	movge	r3, r2
	sub	r2, fp, #64
	movlt	r3, r0
	cmp	r1, r2
	str	r3, [lr], #4
	ldrne	r4, [r1, #4]!
	bne	.L28
.L27:
	ldr	r2, [fp, #-56]
	ldr	r3, [fp, #-52]
	ldr	r1, [fp, #-48]
	mov	r2, r2, lsr #2
	add	r3, r2, r3, lsr #1
	add	r3, r3, r1
	cmp	r3, #3072
	movge	r3, #255
	bge	.L29
	cmp	r3, #1024
	movlt	r3, #0
	blt	.L29
	sub	r3, r3, #1024
	add	r2, r3, #7
	cmp	r3, #0
	movlt	r3, r2
	mov	r3, r3, asr #3
.L29:
	ldr	r2, [fp, #-84]
	mov	r0, r3
	str	r3, [ip, #92]
	str	r2, [ip, #84]
	ldr	r2, [fp, #-80]
	str	r2, [ip, #88]
.L23:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L34:
	rsb	r3, r1, r3
	ldr	r2, [ip, #12]
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, r2
	ble	.L25
	ldr	r3, [ip, #96]
	add	r3, r3, #1
	usat	r3, #4, r3
	str	r3, [ip, #96]
	b	.L26
.L30:
	mvn	r0, #0
	b	.L23
.L36:
	.align	2
.L35:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_API_GMD_P, .-PQ_MNG_ALG_API_GMD_P
	.global	__aeabi_uidiv
	.align	2
	.global	PQ_MNG_ALG_RGMV_HVCHK
	.type	PQ_MNG_ALG_RGMV_HVCHK, %function
PQ_MNG_ALG_RGMV_HVCHK:
	UNWIND(.fnstart)


	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #28)
	sub	sp, sp, #28
	cmp	r2, #0
	str	r0, [fp, #-60]
	str	r2, [fp, #-56]
	str	r1, [fp, #-64]
	ldr	r2, [fp, #4]
	beq	.L37
	cmp	r3, #0
	cmpne	r2, #0
	beq	.L37
	add	r2, r2, r2, lsr #31
	mov	r2, r2, asr #1
	adds	r1, r2, #9
	addmi	r2, r2, #24
	movpl	r2, r1
	cmp	r3, #960
	addgt	r3, r3, r3, lsr #31
	mov	r2, r2, asr #4
	str	r2, [fp, #-52]
	movgt	r3, r3, asr #1
	adds	r7, r3, #33
	addmi	r7, r3, #96
	ldr	r3, [fp, #-52]
	cmp	r3, #0
	mov	r7, r7, asr #6
	moveq	r0, r3
	moveq	r10, r0
	beq	.L42
	mov	r8, #0
	str	r8, [fp, #-48]
	mov	r9, r8
	mov	r10, r8
.L50:
	cmp	r7, #0
	beq	.L51
	ldr	r3, [fp, #-56]
	bic	r6, r8, #3
	mov	r4, #0
	add	r6, r3, r6
	mov	r5, r6
.L49:
	ldr	r3, [r6, r4, asl #4]
	add	r4, r4, #1
	add	r5, r5, #16
	ubfx	r2, r3, #0, #9
	ubfx	r0, r3, #9, #10
	tst	r2, #256
	ubfx	r3, r3, #19, #10
	movne	ip, #65024
	moveq	ip, r2
	movtne	ip, 65535
	eorne	ip, ip, r2
	ldr	r2, [r5, #-12]
	mov	lr, r3, lsr #4
	rsb	r0, lr, r0
	ubfx	r1, r2, #7, #8
	ubfx	r2, r2, #15, #10
	tst	r1, #128
	rsb	r2, lr, r2
	eorne	r1, r1, #255
	bic	r0, r0, r0, asr #31
	bic	r2, r2, r2, asr #31
	mvnne	r1, r1
	cmp	r3, #127
	movls	r3, #0
	movhi	r3, #1
	cmp	r1, #0
	rsblt	r1, r1, #0
	cmp	r2, #255
	movhi	r2, #0
	andls	r2, r3, #1
	cmp	r1, #3
	movle	r2, #0
	andgt	r2, r2, #1
	cmp	r2, #0
	ldrne	r2, [fp, #-48]
	addne	r2, r2, #1
	strne	r2, [fp, #-48]
	cmp	ip, #0
	rsblt	ip, ip, #0
	cmp	r0, #255
	movhi	r3, #0
	andls	r3, r3, #1
	cmp	ip, #3
	movle	r3, #0
	andgt	r3, r3, #1
	cmp	r3, #0
	addne	r10, r10, #1
	cmp	r4, r7
	bne	.L49
.L51:
	ldr	r3, [fp, #8]
	add	r9, r9, #1
	add	r8, r8, r3
	ldr	r3, [fp, #-52]
	cmp	r9, r3
	bne	.L50
	ldr	r3, [fp, #-48]
	mov	r10, r10, asl #7
	mov	r0, r3, asl #7
.L42:
	ldr	r3, [fp, #-52]
	movw	r4, #511
	mul	r7, r3, r7
	mov	r1, r7
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-60]
	mov	r1, r7
	cmp	r0, r4
	movcc	r3, r0
	movcs	r3, r4
	mov	r0, r10
	str	r3, [r2]
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-64]
	cmp	r0, r4
	movcs	r0, r4
	str	r0, [r3]
.L37:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	PQ_MNG_ALG_RGMV_HVCHK, .-PQ_MNG_ALG_RGMV_HVCHK
	.global	g_pstDeiSwAlgPara
	.global	g_bMcDiEn
	.data
	.align	2
	.type	g_bMcDiEn, %object
	.size	g_bMcDiEn, 4
g_bMcDiEn:
	.word	1
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_pstDeiSwAlgPara, %object
	.size	g_pstDeiSwAlgPara, 48
g_pstDeiSwAlgPara:
	.space	48
	.type	nAdjK_STAT.4294, %object
	.size	nAdjK_STAT.4294, 4
nAdjK_STAT.4294:
	.space	4
	.type	g_mtnsum0, %object
	.size	g_mtnsum0, 4
g_mtnsum0:
	.space	4
	.type	g_mtnsum1, %object
	.size	g_mtnsum1, 4
g_mtnsum1:
	.space	4
	.type	g_mtnsum2, %object
	.size	g_mtnsum2, 4
g_mtnsum2:
	.space	4
	.type	g_mtnsum3, %object
	.size	g_mtnsum3, 4
g_mtnsum3:
	.space	4
	.type	g_MTSum_GMD, %object
	.size	g_MTSum_GMD, 16
g_MTSum_GMD:
	.space	16
	.type	nAdjK_status.4316, %object
	.size	nAdjK_status.4316, 12
nAdjK_status.4316:
	.space	12
	.type	nAdjK_STAT_p.4315, %object
	.size	nAdjK_STAT_p.4315, 4
nAdjK_STAT_p.4315:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
