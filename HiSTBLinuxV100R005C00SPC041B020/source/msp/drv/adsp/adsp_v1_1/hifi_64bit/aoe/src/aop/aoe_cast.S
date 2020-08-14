#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"aoe_cast.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	CAST_TimerHandle, %function
CAST_TimerHandle:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x0, x30
	mov	w20, 0
	bl	_mcount
	ldr	x21, .LC2
	ldr	x24, .LC3
	add	x21, x21, 56
.L10:
	ldr	x19, [x21]
	cbz	x19, .L2
	ldr	x1, [x24, 208]
	mov	w0, w20
	blr	x1
	ldr	w22, [x19, 32]
	ldr	x0, [x19, 40]
	bl	RBUF_QueryBusy
	mov	w23, w0
	ldr	x0, [x19, 48]
	bl	RBUF_QueryFree
	cmp	w22, w23
	bhi	.L3
	cmp	w22, w0
	bhi	.L4
	ldp	x0, x1, [x19, 40]
	mov	w3, 1
	mov	w2, w22
.L15:
	bl	RBUF_CopyData
	mov	x0, x19
	ldr	w2, [x19, 56]
	add	w2, w2, 1
	str	w2, [x0, 56]!
.L5:
	ldr	w1, [x19, 68]
	add	w1, w1, 1
	str	w1, [x19, 68]
	ldr	x1, [x21, -32]
	str	w2, [x1, 28]
	ldr	w2, [x0, 4]
	str	w2, [x1, 32]
	ldr	w2, [x0, 8]
	str	w2, [x1, 36]
	ldr	w2, [x0, 12]
	str	w2, [x1, 40]
	ldr	w2, [x0, 16]
	str	w2, [x1, 24]
	ldr	w2, [x0, 20]
	str	w2, [x1, 16]
	ldr	w0, [x0, 24]
	str	w0, [x1, 20]
.L6:
	ldr	x1, [x24, 216]
	mov	w0, w20
	blr	x1
.L2:
	add	w20, w20, 1
	add	x21, x21, 8
	cmp	w20, 4
	bne	.L10
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3:
	cbnz	w23, .L7
	cmp	w22, w0
	bls	.L19
	ldr	w0, [x19, 64]
	add	w0, w0, 1
	str	w0, [x19, 64]
	b	.L6
	.p2align 3
.L7:
	cmp	w23, w0
	bhi	.L9
	mov	w3, 1
	mov	w2, w23
	ldp	x0, x1, [x19, 40]
	b	.L15
	.p2align 3
.L4:
	ldr	x0, [x19, 40]
	mov	w2, w22
.L16:
	mov	x1, 0
	bl	RBUF_ReadData
	ldr	w0, [x19, 64]
	add	w0, w0, 1
	str	w0, [x19, 64]
	b	.L6
	.p2align 3
.L9:
	mov	w2, w23
	ldr	x0, [x19, 40]
	b	.L16
	.p2align 3
.L19:
	ldr	x0, [x19, 48]
	mov	w2, w22
	mov	x1, 0
	bl	RBUF_WriteDataDma
	ldp	w2, w1, [x19, 56]
	add	x0, x19, 56
	add	w1, w1, 1
	str	w1, [x19, 60]
	b	.L5
	.size	CAST_TimerHandle, .-CAST_TimerHandle
	.align	3
.LC2:
	.xword	.LANCHOR0
	.align	3
.LC3:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CAST_SetAopBuf
	.type	CAST_SetAopBuf, %function
CAST_SetAopBuf:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L20
	ldr	x0, .LC4
	ldr	w1, [x0, 4]
	cmp	w19, w1
	beq	.L24
	ldr	w1, [x0, 8]
	cmp	w19, w1
	beq	.L25
	ldr	w1, [x0, 12]
	cmp	w19, w1
	beq	.L26
	ldr	w1, [x0, 16]
	cmp	w19, w1
	beq	.L32
.L20:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L24:
	mov	w1, 0
.L22:
	add	x0, x0, x1, uxtw 3
	ldr	x0, [x0, 56]
	str	x20, [x0, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L25:
	mov	w1, 1
	b	.L22
	.p2align 3
.L26:
	mov	w1, 2
	b	.L22
	.p2align 3
.L32:
	mov	w1, 3
	b	.L22
	.size	CAST_SetAopBuf, .-CAST_SetAopBuf
	.align	3
.LC4:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CAST_Destroy
	.type	CAST_Destroy, %function
CAST_Destroy:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC5
	ldr	w0, [x19, 4]
	cmp	w20, w0
	beq	.L37
	ldr	w0, [x19, 8]
	cmp	w20, w0
	beq	.L38
	ldr	w0, [x19, 12]
	cmp	w20, w0
	beq	.L39
	ldr	w0, [x19, 16]
	cmp	w20, w0
	beq	.L45
.L33:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L37:
	mov	w22, 0
.L34:
	uxtw	x20, w22
	add	x20, x20, 6
	add	x20, x19, x20, lsl 3
	ldr	x21, [x20, 8]
	cbz	x21, .L33
	ldr	x23, .LC6
	mov	w0, w22
	ldr	x24, [x21, 48]
	ldr	x1, [x23, 208]
	blr	x1
	mov	x1, x21
	mov	w0, 26
	bl	HI_ADSP_FREE
	str	xzr, [x20, 8]
	ldr	x1, [x23, 216]
	mov	w0, w22
	blr	x1
	mov	x0, x24
	bl	RBUF_Destroy
	ldr	w0, [x19]
	sub	w0, w0, #1
	str	w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L38:
	mov	w22, 1
	b	.L34
	.p2align 3
.L39:
	mov	w22, 2
	b	.L34
	.p2align 3
.L45:
	mov	w22, 3
	b	.L34
	.size	CAST_Destroy, .-CAST_Destroy
	.align	3
.LC5:
	.xword	.LANCHOR0
	.align	3
.LC6:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CAST_Create
	.type	CAST_Create, %function
CAST_Create:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w23, w0
	mov	x0, x30
	mov	x20, x1
	mov	w26, w2
	mov	w27, w3
	mov	w28, w4
	bl	_mcount
	ldr	x25, .LC7
	str	xzr, [x29, 104]
	ldr	x0, [x25, 56]
	cbz	x0, .L54
	ldr	x0, [x25, 64]
	cbz	x0, .L55
	ldr	x0, [x25, 72]
	cbz	x0, .L56
	ldr	x0, [x25, 80]
	cbz	x0, .L57
.L50:
	mov	x20, 0
	mov	x0, x20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L57:
	mov	w24, 3
.L47:
	mov	w1, 88
	mov	w0, 26
	bl	HI_ADSP_MALLOC
	mov	x19, x0
	cbz	x0, .L50
	ldr	x22, .LC8
	mov	x2, 88
	mov	w1, 0
	uxtw	x21, w24
	ldr	x3, [x22, 40]
	blr	x3
	str	w28, [x19, 20]
	add	x0, x25, x21, lsl 3
	stp	w26, w27, [x19, 12]
	ldr	x26, [x0, 24]
	str	wzr, [x20]
	ldp	w1, w0, [x19, 12]
	bl	UTIL_CalcFrameSize
	ldr	w2, [x19, 20]
	mov	w1, 19923
	movk	w1, 0x1062, lsl 16
	lsl	w3, w2, 6
	add	w2, w3, w2
	umull	x1, w2, w1
	lsr	x1, x1, 32
	lsr	w1, w1, 6
	and	w2, w1, 3
	cbz	w2, .L51
	and	w1, w1, -4
	add	w1, w1, 4
.L51:
	mul	w1, w0, w1
	add	x0, x26, 8
	str	w1, [x20, 16]
	str	x0, [x20, 24]
	add	x0, x26, 12
	str	x0, [x20, 32]
	ldp	w1, w0, [x19, 12]
	bl	UTIL_CalcFrameSize
	str	w0, [x20, 44]
	ldr	w2, [x20, 16]
	mov	x1, x20
	str	w2, [x26, 4]
	add	x0, x29, 104
	str	w23, [x26]
	bl	RBUF_Create
	ldr	x20, [x29, 104]
	cbz	x20, .L52
	add	x0, x25, x21, lsl 2
	ldr	w1, [x25]
	str	x20, [x19, 40]
	add	w1, w1, 1
	str	w1, [x25]
	str	w23, [x0, 4]
	ldr	w1, [x19, 12]
	ldr	w0, [x19, 16]
	bl	UTIL_CalcFrameSize
	ldr	w2, [x19, 20]
	mov	w1, 19923
	movk	w1, 0x1062, lsl 16
	add	w2, w2, w2, lsl 2
	umull	x1, w2, w1
	lsr	x1, x1, 32
	lsr	w1, w1, 6
	and	w2, w1, 3
	cbz	w2, .L53
	and	w1, w1, -4
	add	w1, w1, 4
.L53:
	mul	w1, w0, w1
	str	w1, [x19, 32]
	mov	w0, w24
	add	x21, x25, x21, lsl 3
	ldr	x1, [x22, 200]
	blr	x1
	ldr	x3, [x22, 40]
	mov	x2, 28
	mov	w1, 0
	add	x0, x19, 56
	blr	x3
	str	x19, [x21, 56]
	ldr	x20, [x29, 104]
	ldp	x21, x22, [sp, 32]
	mov	x0, x20
	ldp	x19, x20, [sp, 16]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L52:
	mov	x1, x19
	mov	w0, 26
	bl	HI_ADSP_FREE
	mov	x0, x20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L54:
	mov	w24, 0
	b	.L47
	.p2align 3
.L55:
	mov	w24, 1
	b	.L47
	.p2align 3
.L56:
	mov	w24, 2
	b	.L47
	.size	CAST_Create, .-CAST_Create
	.align	3
.LC7:
	.xword	.LANCHOR0
	.align	3
.LC8:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CAST_FiFoTryCnt
	.type	CAST_FiFoTryCnt, %function
CAST_FiFoTryCnt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC9
	ldr	w1, [x0, 4]
	cmp	w19, w1
	beq	.L72
	ldr	w1, [x0, 8]
	cmp	w19, w1
	beq	.L73
	ldr	w1, [x0, 12]
	cmp	w19, w1
	beq	.L74
	ldr	w1, [x0, 16]
	cmp	w19, w1
	beq	.L80
.L68:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L72:
	mov	w1, 0
.L69:
	add	x0, x0, x1, uxtw 3
	ldr	x0, [x0, 56]
	cbz	x0, .L68
	ldr	w1, [x0, 76]
	add	w1, w1, 1
	str	w1, [x0, 76]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L73:
	mov	w1, 1
	b	.L69
	.p2align 3
.L74:
	mov	w1, 2
	b	.L69
	.p2align 3
.L80:
	mov	w1, 3
	b	.L69
	.size	CAST_FiFoTryCnt, .-CAST_FiFoTryCnt
	.align	3
.LC9:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CAST_FiFoFreeCnt
	.type	CAST_FiFoFreeCnt, %function
CAST_FiFoFreeCnt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC10
	ldr	w1, [x0, 4]
	cmp	w19, w1
	beq	.L85
	ldr	w1, [x0, 8]
	cmp	w19, w1
	beq	.L86
	ldr	w1, [x0, 12]
	cmp	w19, w1
	beq	.L87
	ldr	w1, [x0, 16]
	cmp	w19, w1
	beq	.L93
.L81:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L85:
	mov	w1, 0
.L82:
	add	x0, x0, x1, uxtw 3
	ldr	x0, [x0, 56]
	cbz	x0, .L81
	ldr	w1, [x0, 80]
	add	w1, w1, 1
	str	w1, [x0, 80]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L86:
	mov	w1, 1
	b	.L82
	.p2align 3
.L87:
	mov	w1, 2
	b	.L82
	.p2align 3
.L93:
	mov	w1, 3
	b	.L82
	.size	CAST_FiFoFreeCnt, .-CAST_FiFoFreeCnt
	.align	3
.LC10:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CAST_Init
	.type	CAST_Init, %function
CAST_Init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x19, x22, 1024
	mov	w24, 8
	bl	_mcount
	ldr	x23, .LC13
	ldr	x0, .LC14
	mov	x2, 104
	mov	w1, 0
	add	x22, x22, 1216
	ldr	x3, [x23, 40]
	add	x20, x0, 56
	add	x21, x0, 4
	blr	x3
.L95:
	ldr	x3, [x23, 40]
	mov	x0, x19
	str	x19, [x20, -32]
	mov	x2, 48
	mov	w1, 0
	add	x19, x19, x2
	blr	x3
	str	xzr, [x20], 8
	str	w24, [x21], 4
	cmp	x19, x22
	bne	.L95
	ldr	x0, .LC15
	bl	TIMER_RegIsrFunc
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	CAST_Init, .-CAST_Init
	.align	3
.LC13:
	.xword	g_AdspOsalFunc
	.align	3
.LC14:
	.xword	.LANCHOR0
	.align	3
.LC15:
	.xword	CAST_TimerHandle
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CAST_DeInit
	.type	CAST_DeInit, %function
CAST_DeInit:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	bl	TIMER_DelIsrFunc
	ldr	x0, .LC16
	stp	xzr, xzr, [x0, 56]
	stp	xzr, xzr, [x0, 72]
	ldp	x29, x30, [sp], 16
	ret
	.size	CAST_DeInit, .-CAST_DeInit
	.align	3
.LC16:
	.xword	.LANCHOR0
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	g_stCastRm, %object
	.size	g_stCastRm, 104
g_stCastRm:
	.zero	104
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
