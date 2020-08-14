#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"aoe_reg.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	AOE_REG_ReadBitDepth
	.type	AOE_REG_ReadBitDepth, %function
AOE_REG_ReadBitDepth:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 1
	beq	.L3
	cbz	w19, .L4
	cmp	w19, 2
	beq	.L5
	str	wzr, [x20]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L5:
	mov	w1, 24
	str	w1, [x20]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L4:
	mov	w1, 8
	str	w1, [x20]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3:
	mov	w1, 16
	str	w1, [x20]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_REG_ReadBitDepth, .-AOE_REG_ReadBitDepth
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_REG_ReadChannel
	.type	AOE_REG_ReadChannel, %function
AOE_REG_ReadChannel:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 1
	beq	.L12
	cbz	w19, .L13
	cmp	w19, 3
	beq	.L14
	str	wzr, [x20]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L14:
	mov	w1, 8
	str	w1, [x20]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L13:
	mov	w1, 1
	str	w1, [x20]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L12:
	mov	w1, 2
	str	w1, [x20]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_REG_ReadChannel, .-AOE_REG_ReadChannel
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_REG_ReadPcmType
	.type	AOE_REG_ReadPcmType, %function
AOE_REG_ReadPcmType:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 4
	bhi	.L20
	mov	w0, 0
	str	w19, [x20]
.L19:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L20:
	mov	w0, -1
	b	.L19
	.size	AOE_REG_ReadPcmType, .-AOE_REG_ReadPcmType
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_REG_ReadSampelRate
	.type	AOE_REG_ReadSampelRate, %function
AOE_REG_ReadSampelRate:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w19, 12
	bhi	.L23
	ldr	x1, .LC0
	mov	w0, 0
	ldr	w1, [x1, w19, uxtw 2]
	str	w1, [x20]
.L22:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L23:
	mov	w0, -1
	b	.L22
	.size	AOE_REG_ReadSampelRate, .-AOE_REG_ReadSampelRate
	.align	3
.LC0:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_REG_ReadDataFormat
	.type	AOE_REG_ReadDataFormat, %function
AOE_REG_ReadDataFormat:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	str	w20, [x19]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_REG_ReadDataFormat, .-AOE_REG_ReadDataFormat
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_REG_ReadBufLatencyMs
	.type	AOE_REG_ReadBufLatencyMs, %function
AOE_REG_ReadBufLatencyMs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	str	w20, [x19]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_REG_ReadBufLatencyMs, .-AOE_REG_ReadBufLatencyMs
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOE_REG_ReadBufDelayMs
	.type	AOE_REG_ReadBufDelayMs, %function
AOE_REG_ReadBufDelayMs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	str	w20, [x19]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOE_REG_ReadBufDelayMs, .-AOE_REG_ReadBufDelayMs
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	g_u32AoeSampleRateTab, %object
	.size	g_u32AoeSampleRateTab, 52
g_u32AoeSampleRateTab:
	.word	8000
	.word	11025
	.word	12000
	.word	16000
	.word	22050
	.word	24000
	.word	32000
	.word	44100
	.word	48000
	.word	88200
	.word	96000
	.word	176400
	.word	192000
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
