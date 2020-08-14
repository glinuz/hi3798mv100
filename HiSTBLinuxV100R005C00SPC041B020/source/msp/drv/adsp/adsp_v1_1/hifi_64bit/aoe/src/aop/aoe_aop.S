#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"aoe_aop.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	AOP_Flush
	.type	AOP_Flush, %function
AOP_Flush:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	AOP_Flush, .-AOP_Flush
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOPFlushChnBuf
	.type	AOPFlushChnBuf, %function
AOPFlushChnBuf:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 144]
	cbz	x0, .L2
	bl	RBUF_Flush
.L2:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOPFlushChnBuf, .-AOPFlushChnBuf
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_ProcDelayCompensation
	.type	AOP_ProcDelayCompensation, %function
AOP_ProcDelayCompensation:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x2, .LC0
	add	x0, x2, x21, uxtw 3
	ldr	x19, [x0, 64]
	cbz	x19, .L8
	ldr	w0, [x19, 152]
	cbz	w0, .L8
	ldr	w0, [x19, 164]
	cmp	w0, 1
	beq	.L38
	ldr	w20, [x20, 32]
	cmp	w20, 9
	bhi	.L39
.L8:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L39:
	ldr	x0, [x19, 144]
	add	x1, x29, 56
	ldr	x21, [x2, w21, uxtw 3]
	bl	RBUF_QueryInfo
	ldp	w1, w0, [x19, 48]
	bl	UTIL_CalcFrameSize
	mov	w3, w0
	ldr	w0, [x19, 56]
	mov	w2, 19923
	movk	w2, 0x1062, lsl 16
	mul	w20, w20, w0
	umull	x2, w20, w2
	lsr	x2, x2, 32
	lsr	w2, w2, 6
	and	w0, w2, 3
	cbz	w0, .L14
	and	w2, w2, -4
	add	w2, w2, 4
.L14:
	ldp	w1, w0, [x19, 48]
	mul	w20, w3, w2
	bl	UTIL_CalcFrameSize
	ldr	w3, [x19, 56]
	mov	w1, 19923
	movk	w1, 0x1062, lsl 16
	lsl	w2, w3, 3
	add	w3, w2, w3, lsl 1
	umull	x1, w3, w1
	lsr	x1, x1, 32
	lsr	w1, w1, 6
	and	w2, w1, 3
	cbz	w2, .L15
	and	w1, w1, -4
	add	w1, w1, 4
.L15:
	ldr	w2, [x29, 80]
	cmp	w20, w2
	bcc	.L8
	sub	w2, w20, w2
	madd	w2, w0, w1, w2
	cbz	w2, .L8
	ldr	w3, [x29, 88]
	mov	x1, 0
	ldr	x0, [x19, 144]
	cmp	w2, w3
	csel	w2, w2, w3, ls
	bl	RBUF_WriteData
	ldr	w0, [x21, 100]
	add	w0, w0, 1
	str	w0, [x21, 100]
	b	.L8
	.p2align 3
.L38:
	ldr	x21, [x2, w21, uxtw 3]
	add	x1, x29, 56
	ldr	w0, [x21, 4]
	orr	w0, w0, 1048576
	str	w0, [x21, 4]
	ldr	x0, [x19, 144]
	ldr	w20, [x20, 32]
	bl	RBUF_QueryInfo
	ldr	x0, [x19, 144]
	cbz	x0, .L11
	bl	RBUF_Flush
.L11:
	ldp	w1, w0, [x19, 48]
	bl	UTIL_CalcFrameSize
	ldr	w2, [x19, 56]
	mov	w1, 19923
	movk	w1, 0x1062, lsl 16
	mul	w20, w20, w2
	umull	x1, w20, w1
	lsr	x1, x1, 32
	lsr	w1, w1, 6
	and	w2, w1, 3
	cbz	w2, .L12
	and	w1, w1, -4
	add	w1, w1, 4
.L12:
	ldr	w3, [x29, 88]
	mul	w2, w0, w1
	ldr	x0, [x19, 144]
	mov	x1, 0
	cmp	w2, w3
	csel	w2, w2, w3, ls
	bl	RBUF_WriteData
	ldr	w0, [x21, 100]
	add	w0, w0, 1
	str	w0, [x21, 100]
	str	wzr, [x19, 164]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.size	AOP_ProcDelayCompensation, .-AOP_ProcDelayCompensation
	.align	3
.LC0:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_DestroyChn
	.type	AOP_DestroyChn, %function
AOP_DestroyChn:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC1
	add	x0, x0, x20, uxtw 3
	ldr	x19, [x0, 64]
	cbz	x19, .L40
	ldr	w0, [x19, 136]
	cmp	w0, 1
	beq	.L46
.L42:
	ldr	x0, [x19, 144]
	bl	RBUF_Destroy
	mov	x1, x19
	mov	w0, 26
	bl	HI_ADSP_FREE
.L40:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L46:
	mov	w0, w20
	bl	CAST_Destroy
	b	.L42
	.size	AOP_DestroyChn, .-AOP_DestroyChn
	.align	3
.LC1:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_Init
	.type	AOP_Init, %function
AOP_Init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	mov	x19, x23
	add	x22, x23, 1024
	bl	_mcount
	ldr	x20, .LC4
	ldr	x21, .LC5
	add	x20, x20, 64
	.p2align 2
.L48:
	mov	x2, 128
	ldr	x3, [x21, 40]
	str	x19, [x20, -64]
	mov	x0, x19
	mov	w1, 0
	add	x19, x19, x2
	blr	x3
	ldr	w0, [x19, -124]
	cmp	x19, x22
	orr	w0, w0, 4
	str	w0, [x19, -124]
	str	xzr, [x20], 8
	bne	.L48
	mov	x0, x23
	bl	CAST_Init
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	AOP_Init, .-AOP_Init
	.align	3
.LC4:
	.xword	.LANCHOR0
	.align	3
.LC5:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_DeInit
	.type	AOP_DeInit, %function
AOP_DeInit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x0, x30
	mov	w19, 0
	bl	_mcount
	ldr	x20, .LC6
	add	x20, x20, 64
	.p2align 2
.L51:
	mov	w0, w19
	add	w19, w19, 1
	bl	AOP_DestroyChn
	str	xzr, [x20], 8
	cmp	w19, 8
	bne	.L51
	mov	w0, w19
	bl	CAST_DeInit
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOP_DeInit, .-AOP_DeInit
	.align	3
.LC6:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_Process
	.type	AOP_Process, %function
AOP_Process:
	stp	x29, x30, [sp, -176]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x0, x30
	mov	w21, 0
	add	x23, x29, 100
	bl	_mcount
	ldr	x20, .LC7
	ldr	x22, .LC8
	b	.L76
	.p2align 3
.L70:
	add	w21, w21, 1
	add	x20, x20, 8
	cmp	w21, 8
	beq	.L96
.L76:
	ldr	x19, [x20]
	ldr	w0, [x19, 4]
	tbnz	x0, 2, .L77
	ldr	w0, [x19, 4]
	ands	w0, w0, 3
	beq	.L56
	cmp	w0, 1
	beq	.L57
	ldr	w0, [x19, 4]
	mov	w1, 4
	bfi	w0, w1, 3, 4
	str	w0, [x19, 4]
	ldr	w0, [x19, 4]
	orr	w0, w0, w1
	str	w0, [x19, 4]
	.p2align 2
.L77:
	ldr	x19, [x20, 64]
	cbz	x19, .L70
	ldr	w0, [x19, 152]
	cmp	w0, 1
	bne	.L70
	ldr	x24, [x20]
	str	wzr, [x19, 20]
	ldr	w0, [x24, 12]
	uxtb	w0, w0
	str	w0, [x19, 24]
	ldr	w0, [x24, 12]
	ubfx	x0, x0, 8, 8
	str	w0, [x19, 28]
	ldr	w0, [x24, 12]
	tbnz	x0, 19, .L72
	ldr	w0, [x24, 12]
	ubfx	x0, x0, 16, 3
	str	w0, [x19, 32]
	str	w0, [x19, 36]
	ldr	w0, [x24, 12]
	tbz	x0, 30, .L97
.L74:
	ldr	w0, [x24, 12]
	ubfx	x0, x0, 24, 6
	neg	w0, w0
	str	w0, [x19, 40]
.L75:
	ldr	w0, [x24, 12]
	ldr	w1, [x19, 48]
	ubfx	x0, x0, 31, 1
	str	w0, [x19, 44]
	ldr	w0, [x24, 4]
	ubfx	x0, x0, 8, 1
	str	w0, [x19, 128]
	ldr	w0, [x24, 4]
	ubfx	x0, x0, 10, 1
	str	w0, [x19, 132]
	ldr	w0, [x19, 52]
	ldr	w26, [x24]
	ubfx	x26, x26, 20, 12
	str	w26, [x19, 124]
	bl	UTIL_CalcFrameSize
	mov	w1, w0
	ldr	w2, [x19, 56]
	mov	w0, w26
	bl	UTIL_LatencyMs2ByteSize
	str	w0, [x19, 120]
	ldr	x0, [x19, 144]
	add	x1, x19, 72
	bl	RBUF_UpdateAttr
	ldr	w0, [x24, 4]
	ldr	w1, [x19, 160]
	ubfx	x0, x0, 21, 9
	str	w0, [x19, 156]
	cmp	w0, w1
	beq	.L70
	add	w21, w21, 1
	mov	w1, 1
	cmp	w21, 8
	add	x20, x20, 8
	stp	w0, w1, [x19, 160]
	bne	.L76
.L96:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 176
	ret
	.p2align 3
.L57:
	mov	w0, w21
	bl	AOP_DestroyChn
	str	xzr, [x20, 64]
	ldr	x0, [x20]
.L68:
	ldr	w1, [x0, 4]
	tbnz	x1, 2, .L70
.L81:
	ldr	w1, [x0, 4]
	and	w1, w1, -121
	str	w1, [x0, 4]
	ldr	w1, [x0, 4]
	orr	w1, w1, 4
	str	w1, [x0, 4]
	b	.L77
	.p2align 3
.L56:
	str	xzr, [x29, 88]
	add	x0, x29, 96
	ldr	w1, [x19]
	and	w1, w1, 3
	bl	AOE_REG_ReadBitDepth
	cbnz	w0, .L61
	ldr	w1, [x19]
	mov	x0, x23
	ubfx	x1, x1, 2, 2
	bl	AOE_REG_ReadChannel
	cbnz	w0, .L61
	ldr	w1, [x19]
	add	x0, x29, 104
	ubfx	x1, x1, 4, 4
	bl	AOE_REG_ReadSampelRate
	cbnz	w0, .L61
	ldr	w1, [x19]
	add	x0, x29, 108
	ubfx	x1, x1, 8, 2
	bl	AOE_REG_ReadDataFormat
	cbnz	w0, .L61
	ldr	w1, [x19]
	add	x0, x29, 84
	ubfx	x1, x1, 20, 12
	bl	AOE_REG_ReadBufLatencyMs
	cbnz	w0, .L61
	ldr	w2, [x19]
	ldp	w1, w0, [x29, 96]
	ubfx	x2, x2, 10, 1
	str	w2, [x29, 112]
	bl	UTIL_CalcFrameSize
	mov	w1, 1
	str	w1, [x29, 120]
	mov	x2, 0
	ldr	w1, [x19, 24]
	ubfx	x1, x1, 24, 1
	str	w1, [x29, 160]
	ldr	w4, [x19, 16]
	ldr	w3, [x19, 20]
	bfi	x2, x4, 0, 32
	bfi	x2, x3, 32, 32
	str	x2, [x29, 128]
	ldr	w2, [x19, 24]
	ubfx	x2, x2, 0, 24
	str	w2, [x29, 136]
	cbz	w1, .L59
	ldr	w2, [x19, 28]
	mov	x1, 0
	bfi	x1, x2, 0, 32
	ldr	w2, [x19, 32]
	bfi	x1, x2, 32, 32
	str	x1, [x29, 144]
	mov	x1, 0
	ldr	w2, [x19, 36]
	bfi	x1, x2, 0, 32
	ldr	w2, [x19, 40]
	bfi	x1, x2, 32, 32
	str	x1, [x29, 152]
.L60:
	ldr	w3, [x29, 84]
	mov	w1, w0
	ldr	w2, [x29, 104]
	str	w0, [x29, 164]
	mov	w0, w3
	str	w3, [x29, 172]
	bl	UTIL_LatencyMs2ByteSize
	str	w0, [x29, 168]
	add	x1, x29, 120
	add	x0, x29, 88
	bl	RBUF_Create
	ldr	x0, [x29, 88]
	cbz	x0, .L61
	mov	w1, 168
	mov	w0, 26
	bl	HI_ADSP_MALLOC
	mov	x19, x0
	cbz	x0, .L62
	ldr	x3, [x22, 40]
	mov	x2, 168
	mov	w1, 0
	add	x24, x0, 48
	blr	x3
	ldr	x3, [x22, 48]
	add	x1, x29, 96
	mov	x2, 80
	mov	x0, x24
	blr	x3
	ldr	x0, [x29, 88]
	str	x0, [x19, 144]
	ldr	x0, [x20]
	ldr	w1, [x0, 4]
	tbnz	x1, 19, .L98
.L63:
	str	wzr, [x19, 136]
.L79:
	mov	w0, 121
	stp	wzr, wzr, [x19, 16]
	stp	wzr, w0, [x19]
	stp	w0, wzr, [x19, 8]
	stp	w0, w0, [x19, 24]
	stp	wzr, wzr, [x19, 32]
	stp	wzr, wzr, [x19, 156]
	mov	w0, 1
	str	w0, [x19, 152]
	str	x19, [x20, 64]
.L65:
	ldr	x0, [x20]
	ldr	w1, [x0, 4]
	tbz	x1, 2, .L81
	b	.L77
.L99:
.L66:
.L67:
	mov	x1, x19
	mov	w0, 26
	bl	HI_ADSP_FREE
.L62:
	ldr	x0, [x29, 88]
	bl	RBUF_Destroy
	.p2align 2
.L61:
	ldr	x0, [x20]
	str	xzr, [x20, 64]
	ldr	w1, [x0, 4]
	tbnz	x1, 2, .L68
	ldr	w1, [x0, 4]
	mov	w2, 1
	bfi	w1, w2, 3, 4
	str	w1, [x0, 4]
	ldr	w1, [x0, 4]
	orr	w1, w1, 4
	str	w1, [x0, 4]
	b	.L65
	.p2align 3
.L59:
	add	x1, x19, 28
	add	x19, x19, 36
	stp	x1, x19, [x29, 144]
	b	.L60
	.p2align 3
.L72:
	ldr	w0, [x24, 12]
	ubfx	x0, x0, 16, 3
	neg	w0, w0
	str	w0, [x19, 32]
	str	w0, [x19, 36]
	ldr	w0, [x24, 12]
	tbnz	x0, 30, .L74
.L97:
	ldr	w0, [x24, 12]
	ubfx	x0, x0, 24, 6
	str	w0, [x19, 40]
	b	.L75
	.p2align 3
.L98:
	ldr	w0, [x0, 4]
	ubfx	x0, x0, 18, 1
	cbz	w0, .L63
	str	w0, [x19, 136]
	add	x1, x19, 72
	ldr	w2, [x19, 48]
	mov	w0, w21
	ldp	w3, w4, [x24, 4]
	bl	CAST_Create
	mov	x24, x0
	cbz	x0, .L99
	ldr	x1, [x19, 144]
	mov	w0, w21
	bl	CAST_SetAopBuf
	str	x24, [x19, 144]
	b	.L79
	.size	AOP_Process, .-AOP_Process
	.align	3
.LC7:
	.xword	.LANCHOR0
	.align	3
.LC8:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_GetChnStatus
	.type	AOP_GetChnStatus, %function
AOP_GetChnStatus:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x0, .LC9
	add	x20, x0, x20, uxtw 3
	ldr	x0, [x20, 64]
	cbz	x0, .L103
	ldr	w1, [x0, 152]
	str	w1, [x19]
	ldr	w1, [x0, 64]
	str	w1, [x19, 4]
	ldr	w1, [x0, 116]
	str	w1, [x19, 16]
	ldr	w1, [x0, 48]
	str	w1, [x19, 8]
	ldr	w1, [x0, 52]
	str	w1, [x19, 12]
	ldr	w1, [x0, 128]
	str	w1, [x19, 20]
	ldr	w1, [x0, 136]
	str	w1, [x19, 28]
	ldr	w1, [x0, 132]
	str	w1, [x19, 24]
	ldr	w0, [x0, 156]
	str	w0, [x19, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L103:
	str	wzr, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOP_GetChnStatus, .-AOP_GetChnStatus
	.align	3
.LC9:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_QueryRbfFree
	.type	AOP_QueryRbfFree, %function
AOP_QueryRbfFree:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	x0, .LC10
	add	x19, x0, x19, uxtw 3
	ldr	x2, [x19, 64]
	cbz	x2, .L106
	ldr	w3, [x2, 152]
	mov	w0, 0
	cbz	w3, .L105
	ldr	x2, [x2, 144]
	cbz	x2, .L105
	mov	w1, w20
	mov	x0, x2
	bl	RBUF_CheckFree
.L105:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L106:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOP_QueryRbfFree, .-AOP_QueryRbfFree
	.align	3
.LC10:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_WriteBuffProcTryCnt
	.type	AOP_WriteBuffProcTryCnt, %function
AOP_WriteBuffProcTryCnt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC11
	add	x0, x0, x19, uxtw 3
	ldr	x0, [x0, 64]
	cbz	x0, .L110
	ldr	w0, [x0, 136]
	cmp	w0, 1
	beq	.L115
.L110:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L115:
	mov	w0, w19
	bl	CAST_FiFoTryCnt
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOP_WriteBuffProcTryCnt, .-AOP_WriteBuffProcTryCnt
	.align	3
.LC11:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_WriteBuffProcOKCnt
	.type	AOP_WriteBuffProcOKCnt, %function
AOP_WriteBuffProcOKCnt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC12
	add	x0, x0, x19, uxtw 3
	ldr	x0, [x0, 64]
	cbz	x0, .L116
	ldr	w0, [x0, 136]
	cmp	w0, 1
	beq	.L121
.L116:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L121:
	mov	w0, w19
	bl	CAST_FiFoFreeCnt
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	AOP_WriteBuffProcOKCnt, .-AOP_WriteBuffProcOKCnt
	.align	3
.LC12:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_WriteRbfData
	.type	AOP_WriteRbfData, %function
AOP_WriteRbfData:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, .LC13
	add	x19, x0, x19, uxtw 3
	ldr	x19, [x19, 64]
	cbz	x19, .L137
	ldr	w1, [x19, 152]
	mov	w0, 0
	cbz	w1, .L123
	ldr	w0, [x20, 28]
	cbz	w0, .L146
.L125:
	ldr	w0, [x19, 136]
	ldr	w2, [x20, 8]
	cmp	w0, 1
	ldr	x1, [x20]
	ldr	x0, [x19, 144]
	beq	.L147
	bl	RBUF_WriteDataDma
.L123:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L146:
	ldr	x0, [x19, 48]
	ldr	x1, [x20, 16]
	cmp	x1, x0
	beq	.L127
	ldr	w0, [x19, 48]
	ldr	w1, [x20, 16]
	cmp	w1, w0
	beq	.L128
	bl	AOE_GetWKBuf
	mov	x1, x0
	ldr	w0, [x19, 48]
	mov	x2, x20
	bl	UTIL_UnifyBitDepthPcmData
.L128:
	ldr	w0, [x19, 52]
	ldr	w1, [x20, 20]
	cmp	w1, w0
	beq	.L127
	bl	AOE_GetWKBuf
	mov	x1, x0
	ldr	w0, [x19, 52]
	mov	x2, x20
	bl	UTIL_UpmixPcmData
.L127:
	ldr	w0, [x19, 44]
	cbz	w0, .L130
	bl	AOE_GetWKBuf
	mov	x21, x0
	ldr	x3, .LC14
	mov	w1, 0
	ldr	w2, [x20, 8]
	ldr	x3, [x3, 40]
	blr	x3
	str	x21, [x20]
.L130:
	ldr	w0, [x19, 4]
	ldr	w1, [x19, 24]
	cmp	w1, w0
	beq	.L148
.L131:
	ldr	w0, [x19, 136]
	cmp	w0, 1
	beq	.L132
	bl	AOE_GetWKBuf
	ldr	x22, .LC14
	ldr	w2, [x20, 8]
	mov	x23, x0
	ldr	x1, [x20]
	add	x21, x19, 20
	ldr	x3, [x22, 48]
	blr	x3
	str	x23, [x20]
	mov	x2, x19
	mov	x0, x20
	ldr	w1, [x19, 40]
	mov	x3, x21
	bl	UTIL_LRFadeGainProcess
	ldr	x3, [x22, 48]
	mov	x2, 20
	mov	x1, x21
	mov	x0, x19
	blr	x3
	b	.L125
	.p2align 3
.L147:
	bl	RBUF_WriteData
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L137:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L148:
	ldr	w0, [x19, 12]
	ldr	w1, [x19, 32]
	cmp	w1, w0
	bne	.L131
	.p2align 2
.L132:
	mov	x0, 121
	ldr	x1, [x19, 24]
	movk	x0, 0x79, lsl 32
	cmp	x1, x0
	bne	.L134
	ldr	x0, [x19, 32]
	cbnz	x0, .L134
	ldr	w0, [x19, 40]
	cbz	w0, .L125
.L134:
	bl	AOE_GetWKBuf
	mov	x21, x0
	ldr	x3, .LC14
	ldr	w2, [x20, 8]
	ldr	x1, [x20]
	ldr	x3, [x3, 48]
	blr	x3
	str	x21, [x20]
	add	x2, x19, 20
	mov	x0, x20
	ldr	w1, [x19, 40]
	bl	UTIL_LRGainProcess
	b	.L125
	.size	AOP_WriteRbfData, .-AOP_WriteRbfData
	.align	3
.LC13:
	.xword	.LANCHOR0
	.align	3
.LC14:
	.xword	g_AdspOsalFunc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AOP_GetRingOut
	.type	AOP_GetRingOut, %function
AOP_GetRingOut:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC15
	add	x19, x0, x19, uxtw 3
	ldr	x0, [x19, 64]
	cbz	x0, .L151
	ldr	x0, [x0, 144]
.L150:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L151:
	mov	x0, 0
	b	.L150
	.size	AOP_GetRingOut, .-AOP_GetRingOut
	.align	3
.LC15:
	.xword	.LANCHOR0
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	g_stAopRm, %object
	.size	g_stAopRm, 128
g_stAopRm:
	.zero	128
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
