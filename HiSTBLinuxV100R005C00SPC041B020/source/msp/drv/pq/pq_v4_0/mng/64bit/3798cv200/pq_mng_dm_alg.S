#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"pq_mng_dm_alg.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_UpdateDMCfg
	.type	PQ_MNG_ALG_UpdateDMCfg, %function
PQ_MNG_ALG_UpdateDMCfg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x21, x2
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cbz	x21, .L20
	ldr	w0, [x21, 8]
	ldr	x6, .LC0
	cmp	w0, 720
	bls	.L40
	add	x8, x6, 424
	add	x7, x6, 832
.L3:
	mov	w0, 6
	str	w0, [x20, 140]
	mov	w0, 1
	str	w0, [x19, 4]
	mov	w4, 0
	ldr	w2, [x21, 8]
	cbz	w2, .L4
	ldp	w1, w0, [x20, 144]
	mul	w1, w1, w0
	lsl	w1, w1, 4
	udiv	w0, w1, w2
	cbz	w0, .L4
	mov	w1, 15
	cmp	w0, w1
	csel	w0, w0, w1, ls
	and	w4, w0, 65535
.L4:
	cbz	x20, .L24
	ldr	x3, .LC1
	ldr	w1, [x3]
	cmp	w1, 31
	bhi	.L25
	uxtw	x2, w1
	add	w1, w1, 1
	lsl	x9, x2, 2
.L6:
	add	x2, x20, x2, lsl 2
	add	x5, x20, 12
	add	x0, x5, x9
	str	w1, [x3]
	str	w4, [x2, 12]
	str	x0, [x20, 152]
	ldr	w4, [x20, 140]
	cmp	w4, wzr
	beq	.L7
	mov	w1, 0
	mov	w2, 0
	add	x20, x20, 136
	ble	.L9
	.p2align 2
.L32:
	ldr	w3, [x0], -4
	add	w1, w1, 1
	cmp	x0, x5
	add	w2, w2, w3
	csel	x0, x0, x20, cs
	cmp	w4, w1
	bne	.L32
.L9:
	sdiv	w2, w2, w4
	uxth	w2, w2
.L10:
	ldrh	w0, [x7]
	cmp	w0, w2
	bls	.L5
	ldrb	w0, [x7, 8]
.L13:
	cmp	w0, 5
	mov	w1, 5
	csel	w0, w0, w1, ls
	str	wzr, [x19, 8]
	uxtb	x0, w0
	lsl	x1, x0, 6
	add	x0, x1, x0, lsl 2
	add	x1, x8, x0
	ldrh	w0, [x8, x0]
	str	w0, [x19, 12]
	ldrh	w0, [x1, 2]
	str	w0, [x19, 16]
	ldrh	w0, [x1, 4]
	str	w0, [x19, 20]
	ldrh	w0, [x1, 6]
	str	w0, [x19, 24]
	ldrb	w0, [x1, 8]
	str	w0, [x19, 28]
	ldrb	w0, [x1, 9]
	str	w0, [x19, 32]
	ldrsh	w5, [x1, 10]
	str	w5, [x19, 36]
	ldrsh	w3, [x1, 12]
	str	w3, [x19, 40]
	ldrsh	w4, [x1, 14]
	str	w4, [x19, 44]
	ldrsh	w2, [x1, 16]
	str	w2, [x19, 48]
	ldrb	w0, [x1, 18]
	str	w0, [x19, 52]
	cmp	w0, 1
	beq	.L41
.L17:
	ldrh	w2, [x1, 20]
	cmp	w0, 1
	str	w2, [x19, 56]
	ldrh	w0, [x1, 22]
	str	w0, [x19, 60]
	ldrh	w2, [x1, 24]
	str	w2, [x19, 64]
	ldrh	w4, [x1, 26]
	str	w4, [x19, 68]
	ldrh	w0, [x1, 28]
	str	w0, [x19, 72]
	ldrh	w3, [x1, 30]
	str	w3, [x19, 76]
	beq	.L42
.L18:
	ldrh	w0, [x1, 32]
	str	w0, [x19, 80]
	ldrh	w0, [x1, 34]
	str	w0, [x19, 84]
	ldrb	w0, [x1, 36]
	str	w0, [x19, 88]
	ldrb	w0, [x1, 37]
	str	w0, [x19, 92]
	ldrb	w0, [x1, 38]
	str	w0, [x19, 96]
	ldrh	w0, [x1, 40]
	str	w0, [x19, 100]
	ldrh	w0, [x1, 42]
	str	w0, [x19, 104]
	ldrh	w0, [x1, 44]
	str	w0, [x19, 108]
	ldrh	w0, [x1, 46]
	str	w0, [x19, 112]
	ldrb	w0, [x1, 48]
	str	w0, [x19, 116]
	ldrsh	w3, [x1, 50]
	str	w3, [x19, 120]
	ldrsh	w5, [x1, 52]
	str	w5, [x19, 124]
	ldrsh	w2, [x1, 54]
	str	w2, [x19, 128]
	ldrsh	w4, [x1, 56]
	str	w4, [x19, 132]
	ldrb	w0, [x1, 58]
	str	w0, [x19, 136]
	cmp	w0, 1
	beq	.L43
.L19:
	ldrh	w0, [x1, 60]
	mov	w2, 2
	str	w0, [x19, 140]
	mov	w5, 24
	ldr	w3, [x6, 848]
	mov	w4, 4
	ldrh	w6, [x1, 62]
	mov	w0, 0
	str	w6, [x19, 144]
	ldrh	w6, [x1, 64]
	str	w6, [x19, 148]
	ldrh	w1, [x1, 66]
	stp	w1, w5, [x19, 152]
	stp	w2, w2, [x19, 160]
	str	w4, [x19, 168]
	str	w3, [x19, 192]
.L2:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L40:
	mov	x8, x6
	add	x7, x6, 408
	b	.L3
	.p2align 3
.L24:
	mov	w2, 65535
.L5:
	ldrh	w0, [x7, 2]
	cmp	w0, w2
	bls	.L14
	ldrb	w0, [x7, 9]
	b	.L13
	.p2align 3
.L25:
	mov	w1, 1
	mov	x9, 0
	mov	x2, 0
	b	.L6
	.p2align 3
.L7:
	ldrh	w2, [x5, x9]
	b	.L10
	.p2align 3
.L14:
	ldrh	w0, [x7, 4]
	cmp	w0, w2
	bhi	.L44
	ldrh	w0, [x7, 6]
	cmp	w0, w2
	bls	.L16
	ldrb	w0, [x7, 11]
	b	.L13
	.p2align 3
.L44:
	ldrb	w0, [x7, 10]
	b	.L13
	.p2align 3
.L41:
	cmp	w5, w4
	ccmp	w3, w2, 4, gt
	bgt	.L17
	mov	w0, 0
	str	wzr, [x19, 52]
	b	.L17
	.p2align 3
.L42:
	cmp	w4, w3
	ccmp	w2, w0, 2, ls
	bls	.L18
	str	wzr, [x19, 52]
	b	.L18
	.p2align 3
.L43:
	cmp	w5, w4
	ccmp	w3, w2, 4, gt
	bgt	.L19
	str	wzr, [x19, 136]
	b	.L19
	.p2align 3
.L16:
	ldrb	w0, [x7, 12]
	b	.L13
.L20:
	mov	w0, -1
	b	.L2
	.size	PQ_MNG_ALG_UpdateDMCfg, .-PQ_MNG_ALG_UpdateDMCfg
	.align	3
.LC0:
	.xword	.LANCHOR0
	.align	3
.LC1:
	.xword	.LANCHOR1
	.global	DMParaHD4k
	.global	DMParaSD
	.global	QPLimitFlagLutHD4k
	.global	QPLimitFlagLutSD
	.global	g_u32DmStr
	.data
	.align	3
.LANCHOR0 = . + 0
	.type	DMParaSD, %object
	.size	DMParaSD, 408
DMParaSD:
	.hword	15
	.hword	9
	.hword	35
	.hword	140
	.byte	4
	.byte	5
	.hword	20
	.hword	80
	.hword	1
	.hword	4
	.byte	1
	.zero	1
	.hword	2
	.hword	8
	.hword	3
	.hword	12
	.hword	6
	.hword	24
	.hword	10
	.hword	40
	.byte	6
	.byte	6
	.byte	6
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	20
	.hword	18
	.hword	40
	.hword	160
	.byte	4
	.byte	5
	.hword	25
	.hword	100
	.hword	0
	.hword	0
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	18
	.hword	72
	.byte	7
	.byte	8
	.byte	7
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	20
	.hword	18
	.hword	45
	.hword	180
	.byte	4
	.byte	5
	.hword	25
	.hword	100
	.hword	-1
	.hword	-4
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	20
	.hword	80
	.byte	7
	.byte	8
	.byte	7
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	25
	.hword	18
	.hword	50
	.hword	200
	.byte	4
	.byte	4
	.hword	30
	.hword	120
	.hword	-2
	.hword	-8
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	25
	.hword	100
	.byte	8
	.byte	8
	.byte	8
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	25
	.hword	18
	.hword	50
	.hword	200
	.byte	4
	.byte	4
	.hword	30
	.hword	120
	.hword	-3
	.hword	-12
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	25
	.hword	100
	.byte	8
	.byte	8
	.byte	8
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	18
	.hword	25
	.hword	45
	.hword	180
	.byte	4
	.byte	4
	.hword	25
	.hword	100
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	2
	.hword	8
	.hword	3
	.hword	12
	.hword	6
	.hword	24
	.hword	8
	.hword	32
	.byte	8
	.byte	8
	.byte	6
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.type	QPLimitFlagLutSD, %object
	.size	QPLimitFlagLutSD, 14
QPLimitFlagLutSD:
	.hword	2
	.hword	6
	.hword	10
	.hword	14
	.byte	1
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.zero	1
	.zero	2
	.type	DMParaHD4k, %object
	.size	DMParaHD4k, 408
DMParaHD4k:
	.hword	15
	.hword	9
	.hword	35
	.hword	140
	.byte	4
	.byte	4
	.hword	20
	.hword	80
	.hword	1
	.hword	4
	.byte	1
	.zero	1
	.hword	2
	.hword	8
	.hword	3
	.hword	12
	.hword	6
	.hword	24
	.hword	10
	.hword	40
	.byte	8
	.byte	8
	.byte	6
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	20
	.hword	18
	.hword	40
	.hword	160
	.byte	4
	.byte	4
	.hword	25
	.hword	100
	.hword	0
	.hword	0
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	18
	.hword	72
	.byte	8
	.byte	8
	.byte	7
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	20
	.hword	18
	.hword	45
	.hword	180
	.byte	4
	.byte	4
	.hword	25
	.hword	100
	.hword	-1
	.hword	-4
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	20
	.hword	80
	.byte	8
	.byte	8
	.byte	7
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	25
	.hword	18
	.hword	50
	.hword	200
	.byte	3
	.byte	3
	.hword	30
	.hword	120
	.hword	-2
	.hword	-8
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	25
	.hword	100
	.byte	8
	.byte	8
	.byte	8
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	25
	.hword	18
	.hword	50
	.hword	200
	.byte	3
	.byte	3
	.hword	30
	.hword	120
	.hword	-3
	.hword	-12
	.byte	1
	.zero	1
	.hword	3
	.hword	12
	.hword	2
	.hword	8
	.hword	5
	.hword	20
	.hword	25
	.hword	100
	.byte	8
	.byte	8
	.byte	8
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.hword	18
	.hword	25
	.hword	45
	.hword	180
	.byte	4
	.byte	4
	.hword	25
	.hword	100
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	2
	.hword	8
	.hword	3
	.hword	12
	.hword	6
	.hword	24
	.hword	8
	.hword	32
	.byte	8
	.byte	8
	.byte	6
	.zero	1
	.hword	2
	.hword	8
	.hword	0
	.hword	0
	.byte	9
	.zero	1
	.hword	20
	.hword	80
	.hword	0
	.hword	0
	.byte	0
	.zero	1
	.hword	30
	.hword	120
	.hword	0
	.hword	0
	.type	QPLimitFlagLutHD4k, %object
	.size	QPLimitFlagLutHD4k, 14
QPLimitFlagLutHD4k:
	.hword	1
	.hword	6
	.hword	10
	.hword	14
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.zero	1
	.zero	2
	.type	g_u32DmStr, %object
	.size	g_u32DmStr, 4
g_u32DmStr:
	.word	8
	.bss
	.align	2
.LANCHOR1 = . + 0
	.type	i.1904, %object
	.size	i.1904, 4
i.1904:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
