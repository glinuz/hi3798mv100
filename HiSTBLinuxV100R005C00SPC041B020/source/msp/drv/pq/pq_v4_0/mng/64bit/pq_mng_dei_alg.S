#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"pq_mng_dei_alg.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_CopyU32ByBitWithSignal
	.type	PQ_MNG_CopyU32ByBitWithSignal, %function
PQ_MNG_CopyU32ByBitWithSignal:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	w21, w1
	mov	w20, w4
	mov	x23, x2
	mov	w22, w3
	bl	_mcount
	add	w0, w21, w20
	cmp	w0, 32
	bhi	.L1
	add	w22, w20, w22
	cmp	w22, 32
	bhi	.L1
	mov	w1, 32
	sub	w0, w20, #1
	sub	w1, w1, w20
	sub	w21, w1, w21
	lsl	w19, w19, w21
	lsr	w19, w19, w1
	lsr	w0, w19, w0
	tbnz	x0, 0, .L5
	mov	w4, 1
	lsl	w20, w4, w20
	sub	w20, w20, #1
	and	w19, w20, w19
	str	w19, [x23]
.L1:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L5:
	mov	w4, -1
	lsl	w20, w4, w20
	eor	w19, w20, w19
	str	w19, [x23]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	PQ_MNG_CopyU32ByBitWithSignal, .-PQ_MNG_CopyU32ByBitWithSignal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_CopyU32ByBitWithoutSignal
	.type	PQ_MNG_CopyU32ByBitWithoutSignal, %function
PQ_MNG_CopyU32ByBitWithoutSignal:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	w21, w1
	mov	w20, w4
	mov	x23, x2
	mov	w22, w3
	bl	_mcount
	add	w0, w21, w20
	cmp	w0, 32
	bhi	.L6
	add	w22, w20, w22
	cmp	w22, 32
	bhi	.L6
	mov	w0, 32
	mov	w4, 1
	sub	w0, w0, w20
	lsl	w20, w4, w20
	sub	w1, w0, w21
	sub	w20, w20, #1
	lsl	w1, w19, w1
	lsr	w19, w1, w0
	and	w19, w20, w19
	str	w19, [x23]
.L6:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	PQ_MNG_CopyU32ByBitWithoutSignal, .-PQ_MNG_CopyU32ByBitWithoutSignal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_API_GMD
	.type	PQ_MNG_ALG_API_GMD, %function
PQ_MNG_ALG_API_GMD:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	x19, x3
	bl	_mcount
	cmp	w21, wzr
	ccmp	w20, wzr, 4, ne
	beq	.L16
	ldr	x4, .LC2
	add	w5, w21, w21, lsr 31
	add	w0, w20, w20, lsr 31
	asr	w5, w5, 1
	asr	w0, w0, 1
	add	w5, w5, 1048576
	ldr	w1, [x4, 4]
	add	w0, w0, 1048576
	sdiv	w2, w5, w21
	cmp	w1, wzr
	sdiv	w1, w0, w20
	ble	.L10
	ldr	w0, [x4]
	tbz	w0, #31, .L10
	ldr	w0, [x4, 8]
	cmp	w0, wzr
	ble	.L10
	ldr	w0, [x4, 48]
	mov	w3, 15
	add	w0, w0, 1
	b	.L18
	.p2align 3
.L10:
	ldr	w0, [x4, 48]
	cmp	w0, 3
	ble	.L19
	sub	w0, w0, #4
	mov	w3, 15
.L18:
	cmp	w0, w3
	csel	w0, w0, w3, le
	cmp	w0, wzr
	csel	w0, w0, wzr, ge
	str	w0, [x4, 48]
.L11:
	str	w0, [x19]
	sxtw	x1, w1
	ldp	w5, w3, [x4, 56]
	ldr	w8, [x4, 64]
	ldr	w0, [x4, 52]
	ldp	w7, w6, [x22]
	stp	w0, w5, [x29, 48]
	stp	w3, w8, [x29, 56]
	ldp	w5, w3, [x22, 8]
	stp	w5, w3, [x4, 60]
	stp	w7, w6, [x4, 52]
	mov	x3, 0
	add	x7, x29, 64
	mov	w5, 65535
.L14:
	smull	x0, w0, w2
	mov	w6, 65535
	asr	x0, x0, 12
	mul	x0, x0, x1
	asr	x0, x0, 11
	add	x0, x0, 1
	asr	x0, x0, 1
	cmp	w0, w5
	csel	w0, w0, w5, le
	str	w0, [x7, x3]
	add	x3, x3, 4
	cmp	x3, 16
	beq	.L13
	add	x0, x29, 48
	ldr	w0, [x0, x3]
	b	.L14
	.p2align 3
.L19:
	mov	w0, 0
	str	wzr, [x4, 48]
	b	.L11
	.p2align 3
.L13:
	ldp	w1, w3, [x4, 76]
	ldr	w2, [x4, 84]
	mov	w0, 255
	lsr	w1, w1, 2
	add	w1, w1, w3, lsr 1
	add	w1, w1, w2
	subs	w1, w1, #1024
	csel	w1, w1, wzr, pl
	lsl	w1, w1, 5
	cmp	w1, w6
	bgt	.L15
	add	w0, w1, w0
	cmp	w1, wzr
	csel	w1, w0, w1, lt
	asr	w0, w1, 8
.L15:
	ldp	w6, w5, [x29, 64]
	ldp	w3, w2, [x29, 72]
	stp	w6, w5, [x4, 72]
	stp	w3, w2, [x4, 80]
.L9:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L16:
	mov	w0, -1
	b	.L9
	.size	PQ_MNG_ALG_API_GMD, .-PQ_MNG_ALG_API_GMD
	.align	3
.LC2:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_API_GMD_P
	.type	PQ_MNG_ALG_API_GMD_P, %function
PQ_MNG_ALG_API_GMD_P:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	x19, x3
	bl	_mcount
	cmp	w21, wzr
	ccmp	w20, wzr, 4, ne
	beq	.L28
	ldr	x6, .LC4
	add	w0, w20, w20, lsr 31
	asr	w0, w0, 1
	add	w5, w21, w21, lsr 31
	add	x3, x6, 88
	add	w0, w0, 1048576
	ldr	w4, [x6, 88]
	asr	w5, w5, 1
	sdiv	w1, w0, w20
	add	w5, w5, 1048576
	ldp	w9, w7, [x3, 4]
	sub	w4, w4, w7
	sxtw	x0, w4
	eor	x4, x0, x0, asr 63
	sub	x4, x4, x0, asr 63
	sdiv	w2, w5, w21
	ldr	w5, [x6, 16]
	cmp	w5, w4
	bgt	.L30
.L23:
	mov	w0, 0
	str	wzr, [x6, 100]
.L24:
	str	w0, [x19]
	sxtw	x1, w1
	mov	x3, 0
	add	x8, x29, 64
	ldp	w0, w4, [x22]
	ldr	w5, [x22, 8]
	stp	w0, w4, [x29, 48]
	ldr	w4, [x22, 12]
	str	w4, [x29, 60]
	mov	w4, 65535
	str	w5, [x29, 56]
.L26:
	smull	x0, w0, w2
	mov	w5, 65535
	asr	x0, x0, 12
	mul	x0, x0, x1
	asr	x0, x0, 11
	add	x0, x0, 1
	asr	x0, x0, 1
	cmp	w0, w4
	csel	w0, w0, w4, le
	str	w0, [x8, x3]
	add	x3, x3, 4
	cmp	x3, 16
	beq	.L25
	add	x0, x29, 48
	ldr	w0, [x0, x3]
	b	.L26
	.p2align 3
.L25:
	ldp	w1, w3, [x29, 68]
	ldr	w2, [x29, 76]
	mov	w0, 255
	lsr	w1, w1, 2
	add	w1, w1, w3, lsr 1
	add	w1, w1, w2
	subs	w1, w1, #1024
	csel	w1, w1, wzr, pl
	lsl	w1, w1, 5
	cmp	w1, w5
	ble	.L31
.L27:
	stp	w9, w7, [x6, 88]
	str	w0, [x6, 96]
.L21:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L31:
	add	w0, w1, w0
	cmp	w1, wzr
	csel	w1, w0, w1, lt
	asr	w0, w1, 8
	b	.L27
	.p2align 3
.L30:
	sub	w0, w9, w7
	ldr	w4, [x6, 12]
	sxtw	x0, w0
	eor	x3, x0, x0, asr 63
	sub	x3, x3, x0, asr 63
	cmp	w4, w3
	bge	.L23
	ldr	w0, [x6, 100]
	mov	w3, 15
	add	w0, w0, 1
	cmp	w0, w3
	csel	w0, w0, w3, le
	cmp	w0, wzr
	csel	w0, w0, wzr, ge
	str	w0, [x6, 100]
	b	.L24
	.p2align 3
.L28:
	mov	w0, -1
	b	.L21
	.size	PQ_MNG_ALG_API_GMD_P, .-PQ_MNG_ALG_API_GMD_P
	.align	3
.LC4:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_RGMV_HVCHK
	.type	PQ_MNG_ALG_RGMV_HVCHK, %function
PQ_MNG_ALG_RGMV_HVCHK:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x21, x2
	mov	x0, x30
	mov	x20, x1
	mov	w24, w3
	mov	w23, w4
	mov	w22, w5
	bl	_mcount
	cbz	x21, .L32
	cmp	w23, wzr
	ccmp	w24, wzr, 4, ne
	beq	.L32
	add	w23, w23, w23, lsr 31
	asr	w0, w23, 1
	adds	w12, w0, 9
	add	w0, w0, 24
	csel	w12, w0, w12, mi
	cmp	w24, 960
	asr	w12, w12, 4
	bgt	.L59
	add	w5, w24, 96
	adds	w24, w24, 33
	csel	w24, w5, w24, mi
	asr	w5, w24, 6
.L35:
	mov	w8, 0
	mov	w1, 0
	cbz	w12, .L37
	mov	w11, 0
	mov	w8, 0
	mov	w9, 0
	mov	w10, 0
	.p2align 2
.L45:
	cbz	w5, .L46
	lsr	w7, w11, 2
	mov	w2, 0
	.p2align 2
.L44:
	add	x0, x21, x7, uxtw 2
	ldr	w3, [x21, w7, uxtw 2]
	add	w2, w2, 1
	add	w7, w7, 4
	and	w6, w3, 511
	ubfx	x15, x3, 19, 10
	ldr	w4, [x0, 4]
	and	w0, w6, 256
	cmp	w0, wzr
	eor	w0, w6, -512
	ubfx	x14, x4, 7, 8
	csel	w6, w0, w6, ne
	and	w0, w14, 128
	eor	w1, w14, -256
	cmp	w0, wzr
	lsr	w13, w15, 4
	csel	w1, w1, w14, ne
	ubfx	x4, x4, 15, 10
	cmp	w15, 127
	ubfx	x3, x3, 9, 10
	sxtw	x1, w1
	cset	w14, hi
	subs	w4, w4, w13
	csel	w4, w4, wzr, pl
	eor	x0, x1, x1, asr 63
	cmp	w4, 255
	sub	x0, x0, x1, asr 63
	cset	w1, ls
	cmp	w0, 3
	cset	w15, gt
	subs	w0, w3, w13
	sxtw	x3, w6
	csel	w0, w0, wzr, pl
	cmp	w0, 255
	and	w1, w14, w1
	eor	x0, x3, x3, asr 63
	and	w1, w15, w1
	sub	x0, x0, x3, asr 63
	cset	w3, ls
	cmp	w0, 3
	and	w0, w14, w3
	cset	w4, gt
	cmp	w2, w5
	and	w0, w4, w0
	add	w9, w9, w1
	add	w8, w8, w0
	bne	.L44
.L46:
	add	w10, w10, 1
	add	w11, w11, w22
	cmp	w12, w10
	bne	.L45
	lsl	w1, w9, 7
	lsl	w8, w8, 7
.L37:
	mul	w5, w5, w12
	mov	w2, 511
	udiv	w1, w1, w5
	cmp	w1, w2
	csel	w1, w1, w2, ls
	str	w1, [x19]
	udiv	w0, w8, w5
	cmp	w0, w2
	csel	w0, w0, w2, ls
	str	w0, [x20]
.L32:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L59:
	add	w24, w24, w24, lsr 31
	asr	w0, w24, 1
	add	w5, w0, 96
	adds	w0, w0, 33
	csel	w0, w5, w0, mi
	asr	w5, w0, 6
	b	.L35
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
	.align	3
.LANCHOR0 = . + 0
	.type	g_pstDeiSwAlgPara, %object
	.size	g_pstDeiSwAlgPara, 48
g_pstDeiSwAlgPara:
	.zero	48
	.type	nAdjK_STAT.1625, %object
	.size	nAdjK_STAT.1625, 4
nAdjK_STAT.1625:
	.zero	4
	.type	g_mtnsum0, %object
	.size	g_mtnsum0, 4
g_mtnsum0:
	.zero	4
	.type	g_mtnsum1, %object
	.size	g_mtnsum1, 4
g_mtnsum1:
	.zero	4
	.type	g_mtnsum2, %object
	.size	g_mtnsum2, 4
g_mtnsum2:
	.zero	4
	.type	g_mtnsum3, %object
	.size	g_mtnsum3, 4
g_mtnsum3:
	.zero	4
	.zero	4
	.type	g_MTSum_GMD, %object
	.size	g_MTSum_GMD, 16
g_MTSum_GMD:
	.zero	16
	.type	nAdjK_status.1647, %object
	.size	nAdjK_status.1647, 12
nAdjK_status.1647:
	.zero	12
	.type	nAdjK_STAT_p.1646, %object
	.size	nAdjK_STAT_p.1646, 4
nAdjK_STAT_p.1646:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
