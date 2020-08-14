#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"pq_mng_dm_alg.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	DM_Blend
	.type	DM_Blend, %function
DM_Blend:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w3
	mov	w21, w2
	mov	w22, w1
	bl	_mcount
	sub	w4, w19, w21
	add	w3, w19, w19, lsr 31
	mul	w0, w20, w21
	madd	w0, w4, w22, w0
	ldp	x21, x22, [sp, 32]
	add	w0, w0, w3, asr 1
	sdiv	w0, w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	DM_Blend, .-DM_Blend
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PQ_MNG_ALG_UpdateDMCfg
	.type	PQ_MNG_ALG_UpdateDMCfg, %function
PQ_MNG_ALG_UpdateDMCfg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	w0, [x20, 164]
	add	x2, x20, 184
	cmp	w0, 720
	bgt	.L3
	ldrb	w1, [x20, 169]
	cmp	w1, 1
	beq	.L27
.L3:
	ldrb	w0, [x20, 168]
.L13:
	lsl	x1, x0, 2
	sub	x0, x1, x0
	lsl	x0, x0, 4
	add	x1, x2, x0
	ldrh	w0, [x2, x0]
	strh	w0, [x19, 106]
	ldrh	w0, [x1, 2]
	strh	w0, [x19, 108]
	ldrh	w0, [x1, 4]
	strh	w0, [x19, 116]
	ldrh	w0, [x1, 6]
	strh	w0, [x19, 118]
	ldrb	w0, [x1, 8]
	strb	w0, [x19, 134]
	ldrb	w0, [x1, 9]
	strb	w0, [x19, 135]
	ldrsh	w5, [x1, 10]
	strh	w5, [x19, 124]
	ldrsh	w3, [x1, 12]
	strh	w3, [x19, 126]
	ldrsh	w4, [x1, 14]
	strh	w4, [x19, 128]
	ldrsh	w2, [x1, 16]
	strh	w2, [x19, 130]
	ldrb	w0, [x1, 20]
	strb	w0, [x19, 132]
	cmp	w0, 1
	beq	.L28
.L17:
	ldrb	w0, [x1, 19]
	add	x2, x19, 140
	strb	w0, [x19, 133]
	mov	w0, 0
	ldrb	w3, [x1, 18]
	strb	w3, [x19, 138]
	ldr	x3, [x1, 40]
	ldp	x4, x5, [x3]
	stp	x4, x5, [x2]
	ldp	x4, x5, [x3, 16]
	stp	x4, x5, [x2, 16]
	ldp	x4, x5, [x3, 32]
	stp	x4, x5, [x2, 32]
	ldr	x4, [x3, 48]
	str	x4, [x19, 188]
	ldr	x3, [x3, 54]
	str	x3, [x19, 194]
	ldrh	w2, [x1, 22]
	strh	w2, [x19, 100]
	ldrh	w2, [x1, 24]
	strh	w2, [x19, 102]
	ldrb	w2, [x1, 26]
	strb	w2, [x19, 136]
	ldrb	w2, [x1, 27]
	strb	w2, [x19, 137]
	ldrb	w2, [x1, 28]
	strb	w2, [x19, 110]
	ldrh	w2, [x1, 30]
	strh	w2, [x19, 112]
	ldrh	w2, [x1, 32]
	strh	w2, [x19, 114]
	ldrh	w2, [x1, 34]
	strh	w2, [x19, 120]
	ldrh	w1, [x1, 36]
	strh	w1, [x19, 122]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L27:
	mov	w3, 15
	cmp	w0, wzr
	csel	w3, wzr, w3, le
	cbz	x20, .L19
	ldr	x5, .LC0
	ldr	w4, [x5]
	cmp	w4, 31
	bhi	.L20
	uxtw	x0, w4
	add	w1, w4, 1
	lsl	x7, x0, 2
.L6:
	add	x4, x20, x0, lsl 2
	str	w1, [x5]
	add	x6, x20, 12
	add	x0, x6, x7
	str	w3, [x4, 12]
	str	x0, [x20, 152]
	ldr	w5, [x20, 140]
	cmp	w5, wzr
	beq	.L7
	mov	w1, 0
	mov	w3, 0
	add	x7, x20, 136
	ble	.L9
	.p2align 2
.L23:
	ldr	w4, [x0], -4
	add	w1, w1, 1
	cmp	x0, x6
	add	w3, w3, w4
	csel	x0, x0, x7, cs
	cmp	w5, w1
	bne	.L23
.L9:
	sdiv	w3, w3, w5
	uxth	w3, w3
.L10:
	ldrh	w0, [x20, 170]
	cmp	w0, w3
	bls	.L5
	ldrb	w0, [x20, 178]
	b	.L13
	.p2align 3
.L28:
	cmp	w5, w4
	ccmp	w3, w2, 4, gt
	bgt	.L17
	strb	wzr, [x19, 132]
	b	.L17
	.p2align 3
.L19:
	mov	w3, 65535
.L5:
	ldrh	w0, [x20, 172]
	cmp	w0, w3
	bhi	.L29
	ldrh	w0, [x20, 174]
	cmp	w0, w3
	bls	.L15
	ldrb	w0, [x20, 180]
	b	.L13
	.p2align 3
.L7:
	ldrh	w3, [x6, x7]
	b	.L10
	.p2align 3
.L20:
	mov	x7, 0
	mov	x0, 0
	b	.L6
	.p2align 3
.L29:
	ldrb	w0, [x20, 179]
	b	.L13
	.p2align 3
.L15:
	ldrh	w0, [x20, 176]
	cmp	w0, w3
	bls	.L16
	ldrb	w0, [x20, 181]
	b	.L13
.L16:
	ldrb	w0, [x20, 182]
	b	.L13
	.size	PQ_MNG_ALG_UpdateDMCfg, .-PQ_MNG_ALG_UpdateDMCfg
	.align	3
.LC0:
	.xword	.LANCHOR0
	.global	g_u32DmStr
	.data
	.align	2
	.type	g_u32DmStr, %object
	.size	g_u32DmStr, 4
g_u32DmStr:
	.word	8
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	i.1997, %object
	.size	i.1997, 4
i.1997:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
