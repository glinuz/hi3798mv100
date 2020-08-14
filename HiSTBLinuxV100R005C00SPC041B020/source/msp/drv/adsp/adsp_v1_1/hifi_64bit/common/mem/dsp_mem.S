#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"dsp_mem.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_MALLOC
	.type	HI_ADSP_MALLOC, %function
HI_ADSP_MALLOC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	x2, .LC0
	mov	w1, w20
	mov	w0, w19
	ldr	x2, [x2, 64]
	blr	x2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HI_ADSP_MALLOC, .-HI_ADSP_MALLOC
	.align	3
.LC0:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_FREE
	.type	HI_ADSP_FREE, %function
HI_ADSP_FREE:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L2
	ldr	x2, .LC1
	mov	x1, x19
	mov	w0, w20
	ldr	x2, [x2, 72]
	blr	x2
.L2:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HI_ADSP_FREE, .-HI_ADSP_FREE
	.align	3
.LC1:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_V_MALLOC
	.type	HI_ADSP_V_MALLOC, %function
HI_ADSP_V_MALLOC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	x2, .LC2
	mov	w1, w20
	mov	w0, w19
	ldr	x2, [x2, 80]
	blr	x2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HI_ADSP_V_MALLOC, .-HI_ADSP_V_MALLOC
	.align	3
.LC2:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_V_FREE
	.type	HI_ADSP_V_FREE, %function
HI_ADSP_V_FREE:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L9
	ldr	x2, .LC3
	mov	x1, x19
	mov	w0, w20
	ldr	x2, [x2, 88]
	blr	x2
.L9:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HI_ADSP_V_FREE, .-HI_ADSP_V_FREE
	.align	3
.LC3:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_MALLOC_Aligned
	.type	HI_ADSP_MALLOC_Aligned, %function
HI_ADSP_MALLOC_Aligned:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w23, w0
	mov	x0, x30
	mov	w19, w2
	mov	w22, w1
	bl	_mcount
	sub	w0, w19, #2
	cmp	w0, 254
	bhi	.L19
	cmp	w19, 1
	bls	.L17
	mov	w0, 0
	.p2align 2
.L18:
	add	w0, w0, 1
	lsr	w3, w19, w0
	cmp	w3, 1
	bhi	.L18
	mov	x20, -1
	uxtw	x21, w19
	lsl	x20, x20, x0
	bics	xzr, x21, x20
	bne	.L19
.L21:
	cmp	w19, w22
	bcs	.L20
	udiv	w0, w22, w19
	msub	w0, w0, w19, w22
	cbnz	w0, .L19
.L20:
	ldr	x2, .LC4
	add	w1, w19, w22
	mov	w0, w23
	ldr	x2, [x2, 64]
	blr	x2
	cbz	x0, .L19
	add	x1, x0, x21
	and	x1, x1, x20
	sub	x2, x1, x0
	mov	x0, x1
	strb	w2, [x1, -1]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L19:
	mov	x0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L17:
	uxtw	x21, w19
	mov	x20, -1
	b	.L21
	.size	HI_ADSP_MALLOC_Aligned, .-HI_ADSP_MALLOC_Aligned
	.align	3
.LC4:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_FREE_Aligned
	.type	HI_ADSP_FREE_Aligned, %function
HI_ADSP_FREE_Aligned:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L27
	ldrb	w1, [x19, -1]
	subs	x1, x19, x1
	beq	.L27
	ldr	x2, .LC5
	mov	w0, w20
	ldr	x2, [x2, 72]
	blr	x2
.L27:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HI_ADSP_FREE_Aligned, .-HI_ADSP_FREE_Aligned
	.align	3
.LC5:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_VMALLOC_Aligned
	.type	HI_ADSP_VMALLOC_Aligned, %function
HI_ADSP_VMALLOC_Aligned:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w23, w0
	mov	x0, x30
	mov	w19, w2
	mov	w22, w1
	bl	_mcount
	sub	w0, w19, #2
	cmp	w0, 254
	bhi	.L40
	cmp	w19, 1
	bls	.L38
	mov	w0, 0
	.p2align 2
.L39:
	add	w0, w0, 1
	lsr	w3, w19, w0
	cmp	w3, 1
	bhi	.L39
	mov	x20, -1
	uxtw	x21, w19
	lsl	x20, x20, x0
	bics	xzr, x21, x20
	bne	.L40
.L42:
	cmp	w19, w22
	bcs	.L41
	udiv	w0, w22, w19
	msub	w0, w0, w19, w22
	cbnz	w0, .L40
.L41:
	ldr	x2, .LC6
	add	w1, w19, w22
	mov	w0, w23
	ldr	x2, [x2, 80]
	blr	x2
	cbz	x0, .L40
	add	x1, x0, x21
	and	x1, x1, x20
	sub	x2, x1, x0
	mov	x0, x1
	strb	w2, [x1, -1]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L40:
	mov	x0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L38:
	uxtw	x21, w19
	mov	x20, -1
	b	.L42
	.size	HI_ADSP_VMALLOC_Aligned, .-HI_ADSP_VMALLOC_Aligned
	.align	3
.LC6:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HI_ADSP_VFREE_Aligned
	.type	HI_ADSP_VFREE_Aligned, %function
HI_ADSP_VFREE_Aligned:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L48
	ldrb	w1, [x19, -1]
	subs	x1, x19, x1
	beq	.L48
	ldr	x2, .LC7
	mov	w0, w20
	ldr	x2, [x2, 88]
	blr	x2
.L48:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	HI_ADSP_VFREE_Aligned, .-HI_ADSP_VFREE_Aligned
	.align	3
.LC7:
	.xword	g_AdspOsalFunc
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
