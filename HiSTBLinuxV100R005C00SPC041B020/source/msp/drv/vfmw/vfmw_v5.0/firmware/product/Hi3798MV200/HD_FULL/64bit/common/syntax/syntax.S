#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"syntax.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	InitVoQueue
	.type	InitVoQueue, %function
InitVoQueue:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC1
	mov	w0, 12
	bl	dprint_vfmw
	strh	wzr, [x19, 2]
	mov	w1, 64
	strh	w1, [x19]
	add	x1, x19, 512
	strh	wzr, [x19, 4]
	strh	wzr, [x19, 6]
	mov	w0, -1
	strh	wzr, [x19, 8]
	stp	w0, w0, [x1, 16]
	stp	w0, w0, [x1, 24]
	str	w0, [x19, 544]
	str	w0, [x19, 548]
	mov	w0, 1
	str	wzr, [x19, 572]
	str	wzr, [x19, 600]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	InitVoQueue, .-InitVoQueue
	.align	3
.LC1:
	.xword	.LC0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PrintVoQueue
	.type	PrintVoQueue, %function
PrintVoQueue:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC3
	mov	w0, 1
	ldrh	w4, [x20, 8]
	ldrh	w3, [x20, 4]
	ldrh	w2, [x20, 6]
	bl	dprint_vfmw
	ldr	x22, .LC5
	ldrh	w0, [x20, 4]
	ldrh	w1, [x20, 8]
	and	w19, w0, 63
	cmp	w19, w1
	beq	.L5
	ldr	x21, .LC7
	b	.L6
	.p2align 3
.L4:
	uxtw	x2, w19
	mov	x1, x21
	add	x2, x2, 2
	mov	w0, 1
	ldr	x2, [x20, x2, lsl 3]
	ldr	w2, [x2, 156]
	bl	dprint_vfmw
	add	w0, w19, 1
	ldrh	w1, [x20, 8]
	and	w19, w0, 63
	cmp	w19, w1
	beq	.L5
.L6:
	and	w0, w0, 7
	cbnz	w0, .L4
	mov	x1, x22
	mov	w0, 1
	bl	dprint_vfmw
	b	.L4
	.p2align 3
.L5:
	mov	x1, x22
	mov	w0, 1
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	PrintVoQueue, .-PrintVoQueue
	.align	3
.LC3:
	.xword	.LC2
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC7:
	.xword	.LC6
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ResetVoQueue
	.type	ResetVoQueue, %function
ResetVoQueue:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x20, .LC8
	mov	w0, 2
	ldr	x1, [x20, 24]
	blr	x1
	ldrh	w2, [x19, 6]
	ldrh	w3, [x19, 4]
	ldrh	w4, [x19, 8]
	cmp	w2, w3
	beq	.L13
.L11:
	ldr	x1, .LC10
	mov	w0, 1
	bl	dprint_vfmw
.L12:
	add	x4, x19, 512
	strh	wzr, [x19, 2]
	strh	wzr, [x19, 4]
	mov	w0, 64
	strh	wzr, [x19, 6]
	mov	x3, -1
	strh	w0, [x19]
	mov	w0, -1
	strh	wzr, [x19, 8]
	ldr	w1, [x19, 600]
	ldr	x2, [x20, 32]
	stp	w0, w0, [x4, 16]
	stp	w0, w0, [x4, 24]
	add	w1, w1, 100
	str	w0, [x19, 544]
	str	w0, [x19, 548]
	mov	w0, 2
	str	x3, [x19, 584]
	str	w1, [x19, 600]
	str	wzr, [x19, 572]
	str	wzr, [x19, 580]
	str	xzr, [x19, 592]
	blr	x2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L13:
	cmp	w2, w4
	bne	.L11
	b	.L12
	.size	ResetVoQueue, .-ResetVoQueue
	.align	3
.LC8:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC10:
	.xword	.LC9
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReturnImageToSyntax
	.type	ReturnImageToSyntax, %function
ReturnImageToSyntax:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	mov	x21, x2
	bl	_mcount
	cmp	w19, 20
	bhi	.L14
	ldr	x0, .LC11
	ldrb	w0, [x0,w19,uxtw]
	adr	x1, .Lrtx17
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx17:
	.section	.rodata
	.align	0
	.align	2
.L17:
	.byte	(.L16 - .Lrtx17) / 4
	.byte	(.L18 - .Lrtx17) / 4
	.byte	(.L19 - .Lrtx17) / 4
	.byte	(.L20 - .Lrtx17) / 4
	.byte	(.L14 - .Lrtx17) / 4
	.byte	(.L21 - .Lrtx17) / 4
	.byte	(.L22 - .Lrtx17) / 4
	.byte	(.L14 - .Lrtx17) / 4
	.byte	(.L23 - .Lrtx17) / 4
	.byte	(.L24 - .Lrtx17) / 4
	.byte	(.L25 - .Lrtx17) / 4
	.byte	(.L25 - .Lrtx17) / 4
	.byte	(.L25 - .Lrtx17) / 4
	.byte	(.L26 - .Lrtx17) / 4
	.byte	(.L14 - .Lrtx17) / 4
	.byte	(.L27 - .Lrtx17) / 4
	.byte	(.L28 - .Lrtx17) / 4
	.byte	(.L29 - .Lrtx17) / 4
	.byte	(.L14 - .Lrtx17) / 4
	.byte	(.L14 - .Lrtx17) / 4
	.byte	(.L30 - .Lrtx17) / 4
	.text
	.p2align 3
.L30:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	USERDEC_RecycleImage
.L14:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L16:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	H264DEC_RecycleImage
	b	.L14
	.p2align 3
.L18:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	VC1DEC_RecycleImage
	b	.L14
	.p2align 3
.L19:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	MPEG4DEC_RecycleImage
	b	.L14
	.p2align 3
.L20:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	MPEG2DEC_RecycleImage
	b	.L14
	.p2align 3
.L21:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	DIVX3DEC_RecycleImage
	b	.L14
	.p2align 3
.L22:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	AVSDEC_RecycleImage
	b	.L14
	.p2align 3
.L23:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	REAL8DEC_RecycleImage
	b	.L14
	.p2align 3
.L24:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	REAL9DEC_RecycleImage
	b	.L14
	.p2align 3
.L25:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	VP6DEC_RecycleImage
	b	.L14
	.p2align 3
.L26:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	VP8DEC_RecycleImage
	b	.L14
	.p2align 3
.L27:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	MVC_DEC_RecycleImage
	b	.L14
	.p2align 3
.L28:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	HEVC_DECRecycleImage
	b	.L14
	.p2align 3
.L29:
	ldr	w1, [x21, 156]
	mov	x0, x20
	bl	VP9DEC_RecycleImage
	b	.L14
	.size	ReturnImageToSyntax, .-ReturnImageToSyntax
	.align	3
.LC11:
	.xword	.L17
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ProduceTimeStamp
	.type	ProduceTimeStamp, %function
ProduceTimeStamp:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	mov	w19, w2
	bl	_mcount
	cmp	w19, 20
	ldr	x0, [x20, 600]
	beq	.L32
	ldr	x2, [x21, 584]
	sub	w19, w19, #8
	sub	x1, x0, x2
	cmp	x1, 5
	ccmn	x2, #1, 4, ls
	beq	.L42
	cmp	w19, 1
	mov	x0, -1
	bls	.L34
.L32:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L42:
	cmp	w19, 1
	bhi	.L32
	ldr	w1, [x20, 108]
	and	w1, w1, 3
	cmp	w1, 2
	bne	.L32
.L34:
	mov	x0, -1
	str	x0, [x20, 592]
	b	.L32
	.size	ProduceTimeStamp, .-ProduceTimeStamp
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InsertImgToVoQueue
	.type	InsertImgToVoQueue, %function
InsertImgToVoQueue:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	w21, w0
	mov	x26, x4
	mov	x0, x30
	mov	w23, w1
	mov	x24, x2
	mov	x27, x3
	bl	_mcount
	cbz	x26, .L117
	ldr	x1, .LC15
	mov	w0, 2
	bl	dprint_vfmw
	ldr	x20, .LC16
	mov	w0, 2
	ldr	x1, [x20, 24]
	blr	x1
	ldrh	w19, [x27, 4]
	ldrh	w3, [x27, 8]
	and	w19, w19, 63
	cmp	w19, w3
	beq	.L50
	ldr	x25, .LC18
	mov	w22, 1000
	b	.L114
	.p2align 3
.L49:
	add	w19, w19, 1
	and	w19, w19, 63
	cmp	w19, w3
	beq	.L50
.L114:
	sxtw	x0, w19
	ldr	w1, [x26, 156]
	add	x0, x0, 2
	ldr	x0, [x27, x0, lsl 3]
	ldr	w2, [x0, 156]
	cmp	w2, w1
	bne	.L49
	mov	x1, x25
	mov	w0, 0
	add	w19, w19, 1
	bl	dprint_vfmw
	str	w22, [x26, 160]
	and	w19, w19, 63
	ldrh	w3, [x27, 8]
	cmp	w19, w3
	bne	.L114
	.p2align 2
.L50:
	ldr	w2, [x26, 160]
	cmp	w2, 100
	bhi	.L118
	ldp	w3, w4, [x26, 112]
	cmp	w3, 31
	bhi	.L119
.L52:
	ldr	x1, .LC20
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x26, 160]
	cmp	w0, 1000
	beq	.L55
	mov	x2, x26
	mov	x1, x24
	mov	w0, w23
	bl	ReturnImageToSyntax
.L55:
	ldr	x1, [x20, 32]
	mov	w0, 2
	blr	x1
	mov	w0, 1
.L45:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L119:
	cmp	w4, 31
	bls	.L52
	ldr	x3, [x26, 600]
	cmp	w23, 20
	str	x3, [x26, 592]
	beq	.L75
	add	x19, x27, 528
	ldr	x0, [x19, 56]
	sub	x1, x3, x0
	cmp	x1, 5
	ccmn	x0, #1, 4, ls
	sub	w0, w23, #8
	beq	.L120
	cmp	w0, 1
	bls	.L58
	mov	x2, x3
	mov	x3, -1
	b	.L56
	.p2align 3
.L118:
	ldp	w3, w4, [x26, 112]
	b	.L52
.L120:
	cmp	w0, 1
	mov	x2, x3
	bls	.L121
.L56:
	ldr	x1, .LC22
	mov	w0, 29
	ldr	x4, [x26, 608]
	str	x3, [x26, 600]
	bl	dprint_vfmw
	ldr	w0, [x26, 120]
	cbz	w0, .L59
	ldr	w0, [x26, 124]
	cbnz	w0, .L122
.L59:
	ldp	w1, w0, [x26, 112]
	stp	w1, w0, [x26, 120]
	lsr	w2, w1, 1
	lsr	w0, w0, 1
	str	w2, [x26, 128]
	str	w0, [x26, 132]
.L60:
	str	w1, [x19, 28]
	ldr	w2, [x19, 8]
	ldr	w0, [x26, 116]
	str	w0, [x19, 32]
	ldr	w0, [x26, 108]
	ubfx	x0, x0, 5, 3
	str	w0, [x19, 36]
	ldr	w0, [x26, 108]
	str	w0, [x19, 40]
	ldr	w1, [x26, 164]
	ldp	w23, w0, [x26, 124]
	ldr	w22, [x26, 108]
	cmp	w1, w2
	ldrh	w25, [x26, 132]
	ldrh	w2, [x26, 120]
	ubfx	x28, x22, 14, 3
	orr	w25, w25, w0, lsl 16
	ubfx	x22, x22, 8, 2
	orr	w23, w2, w23, lsl 16
	beq	.L61
	str	w1, [x19, 48]
.L61:
	ldr	w0, [x19, 44]
	ldr	x24, .LC23
	add	w1, w0, 1
	str	w1, [x19, 44]
	str	w0, [x26, 168]
	ldr	w0, [x27, 528]
	cmp	w28, w0
	beq	.L62
	ldr	x4, [x24]
	cbz	x4, .L63
	strb	w0, [x29, 104]
	mov	w3, 2
	strb	w28, [x29, 105]
	add	x2, x29, 104
	mov	w1, 7
	mov	w0, w21
	blr	x4
.L63:
	str	w28, [x27, 528]
.L62:
	ldr	w0, [x19, 4]
	cmp	w22, w0
	beq	.L64
	ldr	x4, [x24]
	cbz	x4, .L65
	add	x2, x29, 112
	mov	w3, 1
	mov	w1, 4
	mov	w0, w21
	strb	w22, [x2, -8]!
	blr	x4
.L65:
	str	w22, [x19, 4]
.L64:
	ldr	w0, [x19, 16]
	cmp	w23, w0
	beq	.L123
.L66:
	ldr	x4, [x24]
	stp	w23, w25, [x19, 16]
	cbz	x4, .L67
	ldp	w2, w0, [x26, 120]
	ldr	w1, [x26, 128]
	mov	w3, 8
	strh	w0, [x29, 106]
	ldr	w0, [x26, 132]
	strh	w2, [x29, 104]
	add	x2, x29, 104
	strh	w1, [x29, 108]
	mov	w1, 1
	strh	w0, [x29, 110]
	mov	w0, w21
	blr	x4
.L67:
	ldp	w1, w0, [x26, 44]
	ldr	w2, [x26, 140]
	sxtw	x22, w21
	stp	w1, w0, [x26, 28]
	add	w1, w1, w2
	add	w0, w2, w0
	stp	w1, w0, [x26, 36]
	ldr	w5, [x26, 156]
	mov	w0, 12
	ldrh	w4, [x27, 8]
	ldrh	w3, [x27, 6]
	ldrh	w2, [x27, 4]
	ldr	x1, .LC25
	bl	dprint_vfmw
	ldr	x23, .LC26
	ldrh	w0, [x27, 8]
	lsl	x1, x22, 4
	add	x1, x1, x22, lsl 2
	add	w2, w0, 1
	add	x0, x0, 2
	add	x25, x23, x1, lsl 4
	and	w2, w2, 63
	sub	x25, x25, x1
	mov	w1, 5
	str	x26, [x27, x0, lsl 3]
	strh	w2, [x27, 8]
	str	wzr, [x26, 348]
	ldr	w0, [x26, 96]
	str	w0, [x25, 20]
	ldr	x27, .LC27
	ldr	w0, [x26, 156]
	ldr	w2, [x27, w21, sxtw 2]
	add	w0, w0, 2
	str	w0, [x19, 24]
	mov	w0, w21
	bl	VDEC_Lowdelay_Event_Time
	ldr	w0, [x27, w21, sxtw 2]
	ldr	x1, [x20, 32]
	add	w0, w0, 1
	str	w0, [x27, w21, sxtw 2]
	mov	w0, 2
	blr	x1
	ldr	x0, .LC28
	ldr	x0, [x0, w21, sxtw 3]
	ldr	w0, [x0, 144]
	cbz	w0, .L69
	ldr	w0, [x25, 8]
	cbnz	w0, .L124
.L69:
	ldr	x4, [x24]
	cbz	x4, .L70
	mov	w3, 624
	mov	x2, x26
	mov	w1, 5
	mov	w0, w21
	blr	x4
.L70:
	mov	w0, 8
	bl	CHECK_REC_POS_ENABLE
	cbnz	w0, .L125
.L72:
	lsl	x0, x22, 4
	ldr	x1, [x20]
	add	x0, x0, x22, lsl 2
	lsl	x19, x0, 4
	sub	x19, x19, x0
	add	x20, x23, x19
	ldr	w0, [x20, 4]
	add	w0, w0, 1
	str	w0, [x20, 4]
	blr	x1
	mov	w2, w0
	ldr	w1, [x23, x19]
	mov	w0, 1
	sub	w1, w2, w1
	cmp	w1, 299
	bls	.L45
	ldr	w0, [x20, 4]
	cbz	w0, .L74
	mov	w3, 10000
	mul	w0, w0, w3
	udiv	w1, w0, w1
	str	w1, [x20, 8]
.L74:
	lsl	x1, x22, 4
	mov	w0, 1
	add	x22, x1, x22, lsl 2
	lsl	x1, x22, 4
	sub	x22, x1, x22
	add	x1, x23, x22
	str	w2, [x23, x22]
	str	wzr, [x1, 4]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L125:
	ldr	x0, [x20]
	blr	x0
	mov	w4, w0
	ldr	x3, .LC29
	ldr	x1, .LC30
	ldr	w0, [x3, w21, sxtw 2]
	ldr	w1, [x1]
	sub	w0, w4, w0
	cmp	w0, w1
	bls	.L72
	lsl	w2, w21, 24
	ldr	x0, .LC31
	add	w2, w2, 524288
	mov	w1, 636
	str	w4, [x3, w21, sxtw 2]
	bl	DBG_AddTrace
	b	.L72
.L123:
	ldr	w0, [x19, 20]
	cmp	w25, w0
	bne	.L66
	b	.L67
.L75:
	mov	x2, x3
	add	x19, x27, 528
	b	.L56
.L124:
	lsl	w1, w0, 2
	sub	w1, w1, w0
	lsl	w2, w1, 5
	add	w1, w1, w2
	add	w0, w1, w0
	str	w0, [x26, 136]
	b	.L69
.L122:
	sub	w0, w23, #15
	cmp	w0, 1
	ccmp	w23, wzr, 4, hi
	bne	.L59
	ldr	w1, [x26, 112]
	b	.L60
.L121:
	ldr	w0, [x26, 108]
	and	w0, w0, 3
	cmp	w0, 2
	bne	.L56
.L58:
	mov	x2, -1
	str	x2, [x26, 592]
	mov	x3, x2
	b	.L56
.L117:
	ldr	x1, .LC13
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -2
	b	.L45
	.size	InsertImgToVoQueue, .-InsertImgToVoQueue
	.align	3
.LC13:
	.xword	.LC12
	.align	3
.LC15:
	.xword	.LC14
	.align	3
.LC16:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC18:
	.xword	.LC17
	.align	3
.LC20:
	.xword	.LC19
	.align	3
.LC22:
	.xword	.LC21
	.align	3
.LC23:
	.xword	g_event_report
	.align	3
.LC25:
	.xword	.LC24
	.align	3
.LC26:
	.xword	g_VfmwChanStat
	.align	3
.LC27:
	.xword	g_LowDelaySeqIndex
	.align	3
.LC28:
	.xword	s_pstVfmwChan
	.align	3
.LC29:
	.xword	.LANCHOR0
	.align	3
.LC30:
	.xword	g_TraceFramePeriod
	.align	3
.LC31:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetQueueImgNum
	.type	GetQueueImgNum, %function
GetQueueImgNum:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	mov	x20, x2
	bl	_mcount
	ldrh	w0, [x19, 6]
	ldrh	w3, [x19, 4]
	cmp	w0, w3
	bcc	.L127
	sub	w3, w0, w3
.L128:
	ldrh	w4, [x19, 8]
	cmp	w0, w4
	bls	.L131
	add	w4, w4, 64
.L131:
	str	w3, [x21]
	sub	w0, w4, w0
	str	w0, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L127:
	add	w1, w0, 64
	sub	w3, w1, w3
	b	.L128
	.size	GetQueueImgNum, .-GetQueueImgNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SYNTAX_ReceiveImage
	.type	SYNTAX_ReceiveImage, %function
SYNTAX_ReceiveImage:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	x20, xzr
	ccmp	x19, xzr, 4, ne
	beq	.L152
	ldr	w0, [x19]
	cmp	w0, 20
	bls	.L154
.L152:
	mov	w0, -2
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L154:
	ldr	x1, .LC32
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx135
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx135:
	.section	.rodata
	.align	0
	.align	2
.L135:
	.byte	(.L148 - .Lrtx135) / 4
	.byte	(.L136 - .Lrtx135) / 4
	.byte	(.L137 - .Lrtx135) / 4
	.byte	(.L138 - .Lrtx135) / 4
	.byte	(.L152 - .Lrtx135) / 4
	.byte	(.L139 - .Lrtx135) / 4
	.byte	(.L140 - .Lrtx135) / 4
	.byte	(.L152 - .Lrtx135) / 4
	.byte	(.L141 - .Lrtx135) / 4
	.byte	(.L142 - .Lrtx135) / 4
	.byte	(.L143 - .Lrtx135) / 4
	.byte	(.L143 - .Lrtx135) / 4
	.byte	(.L143 - .Lrtx135) / 4
	.byte	(.L144 - .Lrtx135) / 4
	.byte	(.L152 - .Lrtx135) / 4
	.byte	(.L145 - .Lrtx135) / 4
	.byte	(.L146 - .Lrtx135) / 4
	.byte	(.L147 - .Lrtx135) / 4
	.byte	(.L152 - .Lrtx135) / 4
	.byte	(.L152 - .Lrtx135) / 4
	.byte	(.L148 - .Lrtx135) / 4
	.text
	.p2align 3
.L148:
	add	x19, x19, 16
	.p2align 2
.L149:
	ldr	x22, .LC33
	mov	w0, 2
	mov	w21, -3
	ldr	x1, [x22, 24]
	blr	x1
	ldrh	w0, [x19, 6]
	ldrh	w1, [x19, 8]
	cmp	w1, w0
	beq	.L150
	add	x1, x0, 2
	ldr	x3, [x22, 104]
	mov	x2, 624
	mov	x0, x20
	mov	w21, 1
	ldr	x1, [x19, x1, lsl 3]
	blr	x3
	ldr	x1, .LC35
	mov	w0, 12
	ldrh	w4, [x19, 8]
	ldrh	w3, [x19, 6]
	ldrh	w2, [x19, 4]
	ldr	w5, [x20, 156]
	bl	dprint_vfmw
	ldr	w1, [x20, 156]
	ldr	w0, [x19, 600]
	add	w0, w1, w0
	str	w0, [x20, 156]
	ldrh	w0, [x19, 6]
	add	w0, w0, w21
	and	w0, w0, 63
	strh	w0, [x19, 6]
.L150:
	ldr	x1, [x22, 32]
	mov	w0, 2
	blr	x1
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L146:
	add	x19, x19, 61440
	add	x19, x19, 1072
	b	.L149
	.p2align 3
.L145:
	add	x19, x19, 920
	b	.L149
	.p2align 3
.L144:
	add	x19, x19, 8192
	add	x19, x19, 24
	b	.L149
	.p2align 3
.L143:
	add	x19, x19, 12288
	add	x19, x19, 472
	b	.L149
	.p2align 3
.L147:
	add	x19, x19, 270336
	add	x19, x19, 888
	b	.L149
	.p2align 3
.L136:
	add	x19, x19, 53248
	add	x19, x19, 200
	b	.L149
	.p2align 3
.L138:
	add	x19, x19, 86016
	add	x19, x19, 1792
	b	.L149
	.p2align 3
.L137:
	add	x19, x19, 20480
	add	x19, x19, 2256
	b	.L149
	.p2align 3
.L142:
	add	x19, x19, 53248
	add	x19, x19, 672
	b	.L149
	.p2align 3
.L141:
	add	x19, x19, 53248
	add	x19, x19, 704
	b	.L149
	.p2align 3
.L140:
	add	x19, x19, 12288
	add	x19, x19, 3920
	b	.L149
	.p2align 3
.L139:
	add	x19, x19, 40960
	add	x19, x19, 648
	b	.L149
	.size	SYNTAX_ReceiveImage, .-SYNTAX_ReceiveImage
	.align	3
.LC32:
	.xword	.L135
	.align	3
.LC33:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC35:
	.xword	.LC34
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SYNTAX_ReleaseImage
	.type	SYNTAX_ReleaseImage, %function
SYNTAX_ReleaseImage:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x21, x0
	mov	x24, x1
	mov	x0, x30
	bl	_mcount
	cbz	x24, .L261
	ldr	w0, [x21]
	cmp	w0, 20
	bls	.L262
.L200:
	mov	w0, -2
	b	.L157
	.p2align 3
.L262:
	ldr	x1, .LC38
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx159
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx159:
	.section	.rodata
	.align	0
	.align	2
.L159:
	.byte	(.L172 - .Lrtx159) / 4
	.byte	(.L160 - .Lrtx159) / 4
	.byte	(.L161 - .Lrtx159) / 4
	.byte	(.L162 - .Lrtx159) / 4
	.byte	(.L200 - .Lrtx159) / 4
	.byte	(.L163 - .Lrtx159) / 4
	.byte	(.L164 - .Lrtx159) / 4
	.byte	(.L200 - .Lrtx159) / 4
	.byte	(.L165 - .Lrtx159) / 4
	.byte	(.L166 - .Lrtx159) / 4
	.byte	(.L167 - .Lrtx159) / 4
	.byte	(.L167 - .Lrtx159) / 4
	.byte	(.L167 - .Lrtx159) / 4
	.byte	(.L168 - .Lrtx159) / 4
	.byte	(.L200 - .Lrtx159) / 4
	.byte	(.L169 - .Lrtx159) / 4
	.byte	(.L170 - .Lrtx159) / 4
	.byte	(.L171 - .Lrtx159) / 4
	.byte	(.L200 - .Lrtx159) / 4
	.byte	(.L200 - .Lrtx159) / 4
	.byte	(.L172 - .Lrtx159) / 4
	.text
.L172:
	add	x25, x21, 16
	.p2align 2
.L173:
	ldr	x22, .LC39
	mov	w0, 2
	ldr	x1, [x22, 24]
	blr	x1
	ldrh	w20, [x25, 4]
	ldrh	w1, [x25, 6]
	and	w20, w20, 63
	ldr	w26, [x24, 156]
	ldr	w0, [x25, 600]
	cmp	w20, w1
	sub	w26, w26, w0
	beq	.L198
	ldr	x23, .LC43
	b	.L247
	.p2align 3
.L178:
	add	w20, w20, 1
	and	w20, w20, 63
	cmp	w20, w1
	beq	.L198
.L247:
	sxtw	x19, w20
	add	x0, x19, 2
	ldr	x0, [x25, x0, lsl 3]
	cbz	x0, .L176
	ldr	w0, [x0, 156]
	cmp	w26, w0
	bne	.L178
	mov	w5, 1
.L177:
	ldrh	w4, [x25, 4]
	b	.L180
	.p2align 3
.L203:
	mov	w20, w1
.L180:
	cmp	w20, wzr
	sub	w0, w20, #1
	mov	w1, 63
	add	x2, x19, 2
	csel	w1, w0, w1, ne
	cmp	w20, w4
	sxtw	x0, w1
	add	x3, x0, 2
	mov	x19, x0
	ldr	x0, [x25, x3, lsl 3]
	str	x0, [x25, x2, lsl 3]
	bne	.L203
	ldr	w0, [x24, 156]
	cmp	w5, 1
	ldr	w1, [x25, 600]
	sub	w0, w0, w1
	str	w0, [x24, 156]
	ldrh	w2, [x25, 4]
	add	w2, w2, 1
	and	w2, w2, 63
	strh	w2, [x25, 4]
	beq	.L246
	ldrh	w1, [x25, 6]
	add	w20, w20, 1
	and	w20, w20, 63
	cmp	w20, w1
	bne	.L247
.L198:
	ldr	x1, .LC41
	mov	w2, w26
	mov	w0, 1
	mov	w19, -2
	bl	dprint_vfmw
.L175:
	ldr	x1, [x22, 32]
	mov	w0, 2
	blr	x1
	mov	w0, w19
.L157:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L169:
	add	x25, x21, 920
	b	.L173
.L163:
	add	x25, x21, 40960
	add	x25, x25, 648
	b	.L173
.L164:
	add	x25, x21, 12288
	add	x25, x25, 3920
	b	.L173
.L165:
	add	x25, x21, 53248
	add	x25, x25, 704
	b	.L173
.L160:
	add	x25, x21, 53248
	add	x25, x25, 200
	b	.L173
.L162:
	add	x25, x21, 86016
	add	x25, x25, 1792
	b	.L173
.L167:
	add	x25, x21, 12288
	add	x25, x25, 472
	b	.L173
.L168:
	add	x25, x21, 8192
	add	x25, x25, 24
	b	.L173
.L161:
	add	x25, x21, 20480
	add	x25, x25, 2256
	b	.L173
.L170:
	add	x25, x21, 61440
	add	x25, x25, 1072
	b	.L173
.L171:
	add	x25, x21, 270336
	add	x25, x25, 888
	b	.L173
.L166:
	add	x25, x21, 53248
	add	x25, x25, 672
	b	.L173
	.p2align 3
.L176:
	mov	w2, w20
	mov	x1, x23
	mov	w0, 1
	bl	dprint_vfmw
	mov	w5, 2
	b	.L177
.L246:
	ldrh	w4, [x25, 8]
	mov	w0, 12
	ldrh	w3, [x25, 6]
	ldr	x1, .LC45
	ldr	w5, [x24, 156]
	bl	dprint_vfmw
	ldr	w0, [x21]
	cmp	w0, 20
	bls	.L263
.L205:
	mov	w19, -2
	b	.L175
	.p2align 3
.L263:
	ldr	x1, .LC46
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx182
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx182:
	.section	.rodata
	.align	0
	.align	2
.L182:
	.byte	(.L181 - .Lrtx182) / 4
	.byte	(.L183 - .Lrtx182) / 4
	.byte	(.L184 - .Lrtx182) / 4
	.byte	(.L185 - .Lrtx182) / 4
	.byte	(.L205 - .Lrtx182) / 4
	.byte	(.L186 - .Lrtx182) / 4
	.byte	(.L187 - .Lrtx182) / 4
	.byte	(.L205 - .Lrtx182) / 4
	.byte	(.L188 - .Lrtx182) / 4
	.byte	(.L189 - .Lrtx182) / 4
	.byte	(.L190 - .Lrtx182) / 4
	.byte	(.L190 - .Lrtx182) / 4
	.byte	(.L190 - .Lrtx182) / 4
	.byte	(.L191 - .Lrtx182) / 4
	.byte	(.L205 - .Lrtx182) / 4
	.byte	(.L192 - .Lrtx182) / 4
	.byte	(.L193 - .Lrtx182) / 4
	.byte	(.L194 - .Lrtx182) / 4
	.byte	(.L205 - .Lrtx182) / 4
	.byte	(.L205 - .Lrtx182) / 4
	.byte	(.L195 - .Lrtx182) / 4
	.text
.L195:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	USERDEC_RecycleImage
	cmp	w0, 1
	beq	.L197
.L196:
	mov	w19, -20
	b	.L175
.L181:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	H264DEC_RecycleImage
	cbnz	w0, .L196
.L197:
	mov	w19, 1
	b	.L175
.L194:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	VP9DEC_RecycleImage
	cbnz	w0, .L196
	mov	w19, 1
	b	.L175
.L193:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	HEVC_DECRecycleImage
	cbnz	w0, .L196
	mov	w19, 1
	b	.L175
.L192:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	MVC_DEC_RecycleImage
	cbnz	w0, .L196
	mov	w19, 1
	b	.L175
.L191:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	VP8DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L190:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	VP6DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L189:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	REAL9DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L188:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	REAL8DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L187:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	AVSDEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L186:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	DIVX3DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L185:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	MPEG2DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L184:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	MPEG4DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L183:
	ldr	w1, [x24, 156]
	add	x0, x21, 8
	bl	VC1DEC_RecycleImage
	cmp	w0, 1
	beq	.L197
	mov	w19, -20
	b	.L175
.L261:
	ldr	x1, .LC37
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -2
	b	.L157
	.size	SYNTAX_ReleaseImage, .-SYNTAX_ReleaseImage
	.align	3
.LC37:
	.xword	.LC36
	.align	3
.LC38:
	.xword	.L159
	.align	3
.LC39:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC41:
	.xword	.LC40
	.align	3
.LC43:
	.xword	.LC42
	.align	3
.LC45:
	.xword	.LC44
	.align	3
.LC46:
	.xword	.L182
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SYNTAX_ReturnImage
	.type	SYNTAX_ReturnImage, %function
SYNTAX_ReturnImage:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19]
	cmp	w0, 20
	bls	.L283
.L282:
	mov	w0, -2
	b	.L265
	.p2align 3
.L283:
	ldr	x1, .LC47
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx267
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx267:
	.section	.rodata
	.align	0
	.align	2
.L267:
	.byte	(.L280 - .Lrtx267) / 4
	.byte	(.L268 - .Lrtx267) / 4
	.byte	(.L269 - .Lrtx267) / 4
	.byte	(.L270 - .Lrtx267) / 4
	.byte	(.L282 - .Lrtx267) / 4
	.byte	(.L271 - .Lrtx267) / 4
	.byte	(.L272 - .Lrtx267) / 4
	.byte	(.L282 - .Lrtx267) / 4
	.byte	(.L273 - .Lrtx267) / 4
	.byte	(.L274 - .Lrtx267) / 4
	.byte	(.L275 - .Lrtx267) / 4
	.byte	(.L275 - .Lrtx267) / 4
	.byte	(.L275 - .Lrtx267) / 4
	.byte	(.L276 - .Lrtx267) / 4
	.byte	(.L282 - .Lrtx267) / 4
	.byte	(.L277 - .Lrtx267) / 4
	.byte	(.L278 - .Lrtx267) / 4
	.byte	(.L279 - .Lrtx267) / 4
	.byte	(.L282 - .Lrtx267) / 4
	.byte	(.L282 - .Lrtx267) / 4
	.byte	(.L280 - .Lrtx267) / 4
	.text
	.p2align 3
.L280:
	add	x19, x19, 16
	.p2align 2
.L281:
	ldr	x20, .LC48
	mov	w0, 2
	ldr	x1, [x20, 24]
	blr	x1
	ldrh	w1, [x19, 6]
	mov	w0, 2
	sub	w1, w1, #1
	and	w1, w1, 63
	strh	w1, [x19, 6]
	ldr	x1, [x20, 32]
	blr	x1
	mov	w0, 1
.L265:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L272:
	add	x19, x19, 12288
	add	x19, x19, 3920
	b	.L281
	.p2align 3
.L273:
	add	x19, x19, 53248
	add	x19, x19, 704
	b	.L281
	.p2align 3
.L274:
	add	x19, x19, 53248
	add	x19, x19, 672
	b	.L281
	.p2align 3
.L269:
	add	x19, x19, 20480
	add	x19, x19, 2256
	b	.L281
	.p2align 3
.L271:
	add	x19, x19, 40960
	add	x19, x19, 648
	b	.L281
	.p2align 3
.L278:
	add	x19, x19, 61440
	add	x19, x19, 1072
	b	.L281
	.p2align 3
.L276:
	add	x19, x19, 8192
	add	x19, x19, 24
	b	.L281
	.p2align 3
.L277:
	add	x19, x19, 920
	b	.L281
	.p2align 3
.L270:
	add	x19, x19, 86016
	add	x19, x19, 1792
	b	.L281
	.p2align 3
.L268:
	add	x19, x19, 53248
	add	x19, x19, 200
	b	.L281
	.p2align 3
.L279:
	add	x19, x19, 270336
	add	x19, x19, 888
	b	.L281
	.p2align 3
.L275:
	add	x19, x19, 12288
	add	x19, x19, 472
	b	.L281
	.size	SYNTAX_ReturnImage, .-SYNTAX_ReturnImage
	.align	3
.LC47:
	.xword	.L267
	.align	3
.LC48:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SegMan
	.type	SegMan, %function
SegMan:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L285
	ldr	x0, [x19, 8]
	cbz	x0, .L285
	ldr	x0, .LC51
	ldr	x0, [x0, w20, sxtw 3]
	ldr	w1, [x0, 1552]
	cmp	w1, 1
	beq	.L305
.L288:
	ldrb	w0, [x19, 18]
	strb	wzr, [x19, 448]
	cmp	w0, 1
	beq	.L306
	cbnz	w0, .L304
	ldr	x21, .LC52
	add	x23, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x23
	add	x22, x19, 184
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	mov	x0, x22
	blr	x3
	ldrb	w0, [x19, 2]
	add	w0, w0, 1
	uxtb	w0, w0
	strb	w0, [x19, 2]
	cmp	w0, 1
	bls	.L304
	ldr	w1, [x19, 120]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
	ldr	w1, [x19, 208]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	mov	x0, x23
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	mov	x0, x22
	blr	x3
	ldr	x1, .LC54
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC55
	ldr	x4, [x0]
	cbz	x4, .L292
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	mov	w0, w20
	blr	x4
.L292:
	strb	wzr, [x19, 2]
.L304:
	mov	w0, -1
.L303:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L305:
	ldr	w0, [x0, 1592]
	cmp	w0, 1
	bne	.L288
	ldrb	w0, [x19, 448]
	cmp	w0, 1
	bne	.L288
	mov	x2, 176
	add	x1, x19, 272
	add	x0, x19, 96
	bl	memcpy
	strb	wzr, [x19, 448]
	mov	w0, 0
	b	.L303
	.p2align 3
.L306:
	ldrb	w21, [x19, 2]
	cbz	w21, .L307
	cmp	w21, 1
	bne	.L304
	ldr	w0, [x19, 116]
	cmp	w0, 1023
	bhi	.L294
	ldr	x1, .LC57
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x22, .LC52
	ldr	w2, [x19, 116]
	ldr	x0, [x19, 8]
	ldr	x1, [x19, 96]
	ldr	x3, [x22, 104]
	sub	x0, x0, x2
	blr	x3
	strb	w21, [x19, 106]
	ldp	w0, w1, [x19, 116]
	ldp	w2, w4, [x19, 24]
	ldr	x3, [x19, 8]
	sub	w2, w2, w0
	sub	x3, x3, x0, uxtw
	add	w0, w4, w0
	str	x3, [x19, 96]
	stp	w2, w0, [x19, 112]
	mov	w0, w20
	bl	SM_ReleaseStreamSeg
	ldr	w0, [x19, 32]
	mov	x2, 88
	ldr	x3, [x22, 96]
	mov	w1, 0
	str	w0, [x19, 120]
	add	x0, x19, 184
	blr	x3
.L295:
	mov	w0, 1
	add	x1, x19, 96
	strb	w0, [x19, 448]
	mov	x2, 176
	strb	wzr, [x19, 2]
	add	x0, x19, 272
	bl	memcpy
	ldr	x23, [sp, 48]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L294:
	ldr	x1, .LC59
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x3, .LC52
	mov	x2, 88
	add	x1, x19, 8
	add	x0, x19, 184
	ldr	x3, [x3, 104]
	blr	x3
	b	.L295
	.p2align 3
.L307:
	ldr	x20, .LC52
	mov	x2, 88
	add	x1, x19, 8
	add	x0, x19, 96
	ldr	x3, [x20, 104]
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	strb	wzr, [x19, 2]
	mov	w0, 0
	b	.L303
	.p2align 3
.L285:
	ldr	x1, .LC50
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L303
	.size	SegMan, .-SegMan
	.align	3
.LC50:
	.xword	.LC49
	.align	3
.LC51:
	.xword	s_pstVfmwChan
	.align	3
.LC52:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC54:
	.xword	.LC53
	.align	3
.LC55:
	.xword	g_event_report
	.align	3
.LC57:
	.xword	.LC56
	.align	3
.LC59:
	.xword	.LC58
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecodeStream
	.type	DecodeStream, %function
DecodeStream:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w1
	mov	x20, x2
	bl	_mcount
	cmp	w21, 17
	bhi	.L308
	ldr	x0, .LC60
	ldrh	w0, [x0,w21,uxtw #1]
	adr	x1, .Lrtx311
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx311:
	.section	.rodata
	.align	0
	.align	2
.L311:
	.2byte	(.L310 - .Lrtx311) / 4
	.2byte	(.L312 - .Lrtx311) / 4
	.2byte	(.L313 - .Lrtx311) / 4
	.2byte	(.L314 - .Lrtx311) / 4
	.2byte	(.L308 - .Lrtx311) / 4
	.2byte	(.L315 - .Lrtx311) / 4
	.2byte	(.L316 - .Lrtx311) / 4
	.2byte	(.L308 - .Lrtx311) / 4
	.2byte	(.L317 - .Lrtx311) / 4
	.2byte	(.L318 - .Lrtx311) / 4
	.2byte	(.L319 - .Lrtx311) / 4
	.2byte	(.L319 - .Lrtx311) / 4
	.2byte	(.L319 - .Lrtx311) / 4
	.2byte	(.L320 - .Lrtx311) / 4
	.2byte	(.L308 - .Lrtx311) / 4
	.2byte	(.L321 - .Lrtx311) / 4
	.2byte	(.L322 - .Lrtx311) / 4
	.2byte	(.L323 - .Lrtx311) / 4
	.text
	.p2align 3
.L312:
	ldrb	w2, [x19, 18]
	mov	w0, 22
	ldr	x1, .LC64
	bl	dprint_vfmw
	ldr	x1, .LC66
	mov	w0, 22
	ldr	w2, [x19, 24]
	bl	dprint_vfmw
	ldr	x1, .LC68
	mov	w0, 22
	ldr	w2, [x19, 28]
	bl	dprint_vfmw
	ldr	x1, [x20]
	ldr	w0, [x1, 596]
	cmp	w0, 1
	bhi	.L404
	ldr	w2, [x1, 600]
	cmp	w2, 8
	bgt	.L405
	add	x21, x20, 16384
	sxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x21, 1528]
	ldr	w0, [x1, 600]
	strb	w0, [x21, 1530]
	beq	.L406
	ldrb	w0, [x19, 19]
	cmp	w0, 1
	beq	.L407
	ldr	x22, .LC61
	add	x23, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x23
	ldr	x3, [x22, 104]
	blr	x3
	ldr	x3, [x22, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	ldr	w0, [x19, 116]
	cmp	w0, 15
	bhi	.L344
	ldr	x1, .LC75
.L403:
	mov	w0, 0
	add	x20, x20, 53248
	bl	dprint_vfmw
	ldr	w1, [x19, 120]
	ldr	w0, [x20, 800]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC62
	ldr	x4, [x0]
	cbz	x4, .L308
	ldr	w0, [x20, 800]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L308:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L323:
	ldrb	w0, [x19, 19]
	cmp	w0, 1
	beq	.L408
	ldr	x21, .LC61
	add	x23, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x23
	add	x22, x20, 290816
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	mov	x1, x23
	mov	x0, x20
	bl	VP9DEC_DecodePacket
	ldr	w0, [x22, 3292]
	cbnz	w0, .L409
.L332:
	ldr	w0, [x22, 3288]
	cbnz	w0, .L333
	strb	wzr, [x19, 1]
.L334:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	add	x20, x20, 262144
	ldrb	w0, [x20, 1044]
	cbnz	w0, .L308
	ldr	x3, [x21, 96]
	mov	x2, 176
	mov	w1, 0
	mov	x0, x23
	blr	x3
	ldr	w1, [x19, 32]
	ldr	w0, [x22, 3392]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC62
	ldr	x4, [x0]
	cbz	x4, .L308
	ldr	w0, [x22, 3392]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L308
	.p2align 3
.L310:
	ldr	x2, [x19, 8]
	add	x1, x29, 72
	ldr	x3, [x19, 40]
	mov	x0, x20
	ldrb	w5, [x19, 18]
	ldp	w7, w6, [x19, 24]
	ldr	w4, [x19, 32]
	str	x2, [x29, 80]
	str	x3, [x29, 104]
	ldrsb	w2, [x19, 17]
	ldrb	w3, [x19, 20]
	strb	w5, [x29, 72]
	stp	w7, w6, [x29, 88]
	str	w4, [x29, 96]
	strb	w3, [x29, 73]
	str	w2, [x29, 100]
	bl	H264DEC_DecodePacket
	cmn	w0, #2
	beq	.L410
	strb	wzr, [x19, 1]
.L325:
	ldr	x0, [x20, 616]
	cbnz	x0, .L308
	add	x19, x20, 12288
	ldrb	w0, [x19, 2640]
	cmp	w0, 1
	bne	.L308
	add	x20, x20, 65536
	ldr	w0, [x20, 116]
	bl	VCTRL_OutputLastFrame
	strb	wzr, [x19, 2640]
	b	.L308
	.p2align 3
.L313:
	add	x21, x20, 20480
	mov	x0, x20
	ldr	w22, [x21, 2856]
	bl	MP4_GetPacketState
	cmp	w0, 3
	beq	.L411
.L350:
	ldr	w0, [x21, 2856]
	mov	x1, x19
	bl	SegMan
	cbz	w0, .L412
.L352:
	ldr	x0, [x20, 1040]
	cbnz	x0, .L308
	ldrb	w0, [x21, 2892]
	cmp	w0, 1
	bne	.L308
	ldr	w0, [x21, 2856]
	bl	VCTRL_OutputLastFrame
	strb	wzr, [x21, 2892]
	b	.L308
	.p2align 3
.L314:
	add	x21, x20, 86016
	mov	x0, x20
	ldr	w22, [x21, 2392]
	bl	MP2_GetPacketState
	cmp	w0, 2
	beq	.L413
.L346:
	ldr	w0, [x21, 2392]
	mov	x1, x19
	bl	SegMan
	cbz	w0, .L414
.L348:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	ldrb	w0, [x21, 2410]
	cmp	w0, 1
	bne	.L308
	ldr	w0, [x21, 2392]
	bl	VCTRL_OutputLastFrame
	strb	wzr, [x21, 2410]
	b	.L308
	.p2align 3
.L315:
	ldrb	w0, [x19, 19]
	cmp	w0, 1
	beq	.L415
	ldr	x21, .LC61
	add	x22, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x22
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	mov	x1, x22
	mov	x0, x20
	bl	DIVX3DEC_Decode
	cmn	w0, #2
	beq	.L416
	strb	wzr, [x19, 1]
.L364:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	ldr	x3, [x21, 96]
	mov	x2, 176
	mov	w1, 0
	mov	x0, x22
	add	x20, x20, 40960
	blr	x3
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 1248]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC62
	ldr	x4, [x0]
	cbz	x4, .L308
	ldr	w0, [x20, 1248]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L308
	.p2align 3
.L316:
	mov	x0, x20
	ldr	w21, [x20, 4]
	bl	AVS_GetPacketState
	cbnz	w0, .L353
	ldr	x0, .LC73
	ldr	x0, [x0, w21, sxtw 3]
	ldr	w1, [x0, 1552]
	cmp	w1, 1
	bne	.L354
	ldr	w0, [x0, 1592]
	cbnz	w0, .L353
.L354:
	add	x1, x20, 16384
	mov	x0, x20
	add	x1, x1, 432
	bl	AVSDEC_Decode
.L353:
	ldr	w0, [x20, 4]
	mov	x1, x19
	bl	SegMan
	cbz	w0, .L417
.L355:
	ldr	x0, [x20, 24]
	cbnz	x0, .L308
	ldrb	w0, [x20]
	cmp	w0, 1
	bne	.L308
	ldr	w0, [x20, 4]
	bl	VCTRL_OutputLastFrame
	strb	wzr, [x20]
	b	.L308
	.p2align 3
.L317:
	ldrb	w0, [x19, 19]
	cmp	w0, 1
	beq	.L418
	ldr	x21, .LC61
	add	x22, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x22
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	mov	x1, x22
	mov	x0, x20
	bl	REAL8DEC_Decode
	cmn	w0, #2
	beq	.L419
	strb	wzr, [x19, 1]
.L358:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	ldr	x3, [x21, 96]
	mov	x2, 176
	mov	w1, 0
	mov	x0, x22
	add	x20, x20, 53248
	blr	x3
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 1304]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC62
	ldr	x4, [x0]
	cbz	x4, .L308
	ldr	w0, [x20, 1304]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L308
	.p2align 3
.L318:
	ldrb	w0, [x19, 19]
	cmp	w0, 1
	beq	.L420
	ldr	x21, .LC61
	add	x22, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x22
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	mov	x1, x22
	mov	x0, x20
	bl	REAL9DEC_Decode
	cmn	w0, #2
	beq	.L421
	strb	wzr, [x19, 1]
.L361:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	ldr	x3, [x21, 96]
	mov	x2, 176
	mov	w1, 0
	mov	x0, x22
	add	x20, x20, 53248
	blr	x3
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 1272]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC62
	ldr	x4, [x0]
	cbz	x4, .L308
	ldr	w0, [x20, 1272]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L308
	.p2align 3
.L319:
	ldrb	w0, [x19, 19]
	cmp	w0, 1
	beq	.L422
	ldr	x21, .LC61
	add	x22, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x22
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	mov	x1, x22
	mov	x0, x20
	bl	VP6DEC_Decode
	cmn	w0, #2
	beq	.L423
	strb	wzr, [x19, 1]
.L370:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	ldr	x3, [x21, 96]
	mov	x2, 176
	mov	w1, 0
	mov	x0, x22
	blr	x3
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 13360]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC62
	ldr	x4, [x0]
	cbz	x4, .L308
	ldr	w0, [x20, 13360]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L308
	.p2align 3
.L320:
	ldrb	w0, [x19, 19]
	cmp	w0, 1
	beq	.L424
	ldr	x21, .LC61
	add	x22, x19, 96
	mov	x2, 88
	add	x1, x19, 8
	mov	x0, x22
	ldr	x3, [x21, 104]
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 88
	mov	w1, 0
	add	x0, x19, 184
	blr	x3
	mov	x1, x22
	mov	x0, x20
	bl	VP8DEC_Decode
	cmn	w0, #2
	beq	.L425
	strb	wzr, [x19, 1]
.L367:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	ldr	x3, [x21, 96]
	mov	x2, 176
	mov	w1, 0
	mov	x0, x22
	blr	x3
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 8816]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC62
	ldr	x4, [x0]
	cbz	x4, .L308
	ldr	w0, [x20, 8816]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	b	.L308
	.p2align 3
.L321:
	ldrb	w3, [x19, 18]
	add	x1, x29, 72
	ldp	w5, w4, [x19, 24]
	ldr	w2, [x19, 32]
	mov	x0, x20
	ldr	x6, [x19, 8]
	ldr	x7, [x19, 40]
	strb	w3, [x29, 72]
	str	x6, [x29, 80]
	stp	w5, w4, [x29, 88]
	str	w2, [x29, 96]
	str	x7, [x29, 104]
	bl	MVC_DEC_DecodePacket
	cmn	w0, #2
	beq	.L426
	strb	wzr, [x19, 1]
.L336:
	ldr	x0, [x20, 232]
	cbnz	x0, .L308
	ldrb	w0, [x20, 1]
	cmp	w0, 1
	bne	.L308
	ldr	w0, [x20, 120]
	bl	VCTRL_OutputLastFrame
	strb	wzr, [x20, 1]
	b	.L308
	.p2align 3
.L322:
	ldr	x2, [x19, 8]
	add	x1, x29, 72
	ldr	x3, [x19, 40]
	mov	x0, x20
	ldrb	w5, [x19, 18]
	ldp	w7, w6, [x19, 24]
	ldr	w4, [x19, 32]
	str	x2, [x29, 80]
	str	x3, [x29, 104]
	ldrsb	w2, [x19, 17]
	ldrb	w3, [x19, 20]
	strb	w5, [x29, 72]
	stp	w7, w6, [x29, 88]
	str	w4, [x29, 96]
	strb	w3, [x29, 73]
	str	w2, [x29, 100]
	bl	HEVCDEC_DecodePacket
	cmn	w0, #2
	beq	.L328
	ldrb	w0, [x20, 5]
	cbz	w0, .L329
.L328:
	mov	w0, 1
	strb	w0, [x19, 1]
.L330:
	ldr	x0, [x20, 2384]
	cbnz	x0, .L308
	ldrb	w0, [x20, 7]
	cmp	w0, 1
	bne	.L308
	ldr	w0, [x20, 276]
	bl	VCTRL_OutputLastFrame
	strb	wzr, [x20, 7]
	b	.L308
	.p2align 3
.L404:
	ldr	x1, .LC70
	mov	w0, 0
	bl	dprint_vfmw
	b	.L308
	.p2align 3
.L409:
	ldr	w0, [x22, 3328]
	ldr	w1, [x22, 3332]
	cmp	w1, w0
	beq	.L332
.L333:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L334
	.p2align 3
.L329:
	strb	wzr, [x19, 1]
	b	.L330
	.p2align 3
.L416:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L364
	.p2align 3
.L419:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L358
	.p2align 3
.L425:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L367
	.p2align 3
.L421:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L361
	.p2align 3
.L423:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L370
	.p2align 3
.L417:
	add	x1, x19, 96
	mov	x0, x20
	bl	AVSDEC_Decode
	b	.L355
	.p2align 3
.L414:
	add	x1, x19, 96
	mov	x0, x20
	bl	MPEG2DEC_Decode
	ldrb	w0, [x21, 2409]
	cmp	w0, 1
	csel	w0, w0, wzr, eq
	strb	w0, [x19, 1]
	b	.L348
	.p2align 3
.L412:
	add	x1, x19, 96
	mov	x0, x20
	bl	MPEG4DEC_Decode
	b	.L352
	.p2align 3
.L413:
	ldr	x0, .LC73
	ldr	x0, [x0, w22, sxtw 3]
	ldr	w1, [x0, 1552]
	cmp	w1, 1
	bne	.L347
	ldr	w0, [x0, 1592]
	cbnz	w0, .L346
.L347:
	add	x1, x21, 1216
	mov	x0, x20
	bl	MPEG2DEC_Decode
	b	.L346
	.p2align 3
.L411:
	ldr	x0, .LC73
	ldr	x0, [x0, w22, sxtw 3]
	ldr	w1, [x0, 1552]
	cmp	w1, 1
	bne	.L351
	ldr	w0, [x0, 1592]
	cbnz	w0, .L350
.L351:
	add	x1, x20, 12288
	mov	x0, x20
	add	x1, x1, 3456
	bl	MPEG4DEC_Decode
	b	.L350
	.p2align 3
.L410:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L325
	.p2align 3
.L426:
	mov	w0, 1
	strb	w0, [x19, 1]
	b	.L336
	.p2align 3
.L418:
	add	x20, x20, 53248
	ldr	w0, [x20, 1304]
	bl	VCTRL_OutputLastFrame
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 1304]
	bl	SM_ReleaseStreamSeg
	b	.L308
	.p2align 3
.L422:
	ldr	w0, [x20, 13360]
	bl	VCTRL_OutputLastFrame
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 13360]
	bl	SM_ReleaseStreamSeg
	b	.L308
	.p2align 3
.L415:
	add	x20, x20, 40960
	ldr	w0, [x20, 1248]
	bl	VCTRL_OutputLastFrame
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 1248]
	bl	SM_ReleaseStreamSeg
	b	.L308
	.p2align 3
.L408:
	add	x20, x20, 290816
	ldr	w0, [x20, 3392]
	bl	VCTRL_OutputLastFrame
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 3392]
	bl	SM_ReleaseStreamSeg
	b	.L308
	.p2align 3
.L424:
	ldr	w0, [x20, 8816]
	bl	VCTRL_OutputLastFrame
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 8816]
	bl	SM_ReleaseStreamSeg
	b	.L308
	.p2align 3
.L420:
	add	x20, x20, 53248
	ldr	w0, [x20, 1272]
	bl	VCTRL_OutputLastFrame
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 1272]
	bl	SM_ReleaseStreamSeg
	b	.L308
	.p2align 3
.L344:
	ldr	x0, [x19, 96]
	add	x2, x21, 1528
	mov	w1, 16
	bl	Vc1SMPSeqHdr
	cbz	w0, .L427
	ldr	x1, .LC77
	b	.L403
.L405:
	ldr	x1, .LC72
	mov	w0, 0
	bl	dprint_vfmw
	b	.L308
.L407:
	add	x20, x20, 53248
	ldr	w0, [x20, 800]
	bl	VCTRL_OutputLastFrame
	ldr	w1, [x19, 32]
	ldr	w0, [x20, 800]
	bl	SM_ReleaseStreamSeg
	b	.L308
.L406:
	add	x21, x20, 53248
	mov	x0, x20
	ldr	w22, [x21, 800]
	bl	VC1_GetPacketState
	cmp	w0, 2
	beq	.L428
.L340:
	ldr	w0, [x21, 800]
	mov	x1, x19
	bl	SegMan
	cbz	w0, .L429
.L342:
	ldr	x0, [x20, 8]
	cbnz	x0, .L308
	add	x20, x20, 12288
	ldrb	w0, [x20, 2527]
	cmp	w0, 1
	bne	.L308
	ldr	w0, [x21, 800]
	bl	VCTRL_OutputLastFrame
	strb	wzr, [x20, 2527]
	b	.L308
	.p2align 3
.L427:
	ldp	w0, w3, [x19, 112]
	ldr	x2, [x19, 96]
	mov	x1, x23
	add	w0, w0, 16
	sub	w3, w3, #16
	add	x2, x2, 16
	str	x2, [x19, 96]
	stp	w0, w3, [x19, 112]
	mov	x0, x20
	bl	VC1DEC_Decode
	ldr	x3, [x22, 96]
	mov	x2, 176
	mov	w1, 0
	mov	x0, x23
	blr	x3
	b	.L308
.L429:
	add	x19, x19, 96
	mov	x0, x20
	mov	x1, x19
	bl	VC1DEC_Decode
	ldr	x3, .LC61
	mov	x2, 176
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	b	.L342
.L428:
	ldr	x0, .LC73
	ldr	x0, [x0, w22, sxtw 3]
	ldr	w1, [x0, 1552]
	cmp	w1, 1
	bne	.L341
	ldr	w0, [x0, 1592]
	cbnz	w0, .L340
.L341:
	add	x1, x20, 12288
	mov	x0, x20
	add	x1, x1, 2344
	bl	VC1DEC_Decode
	b	.L340
	.size	DecodeStream, .-DecodeStream
	.align	3
.LC60:
	.xword	.L311
	.align	3
.LC61:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC62:
	.xword	g_event_report
	.align	3
.LC64:
	.xword	.LC63
	.align	3
.LC66:
	.xword	.LC65
	.align	3
.LC68:
	.xword	.LC67
	.align	3
.LC70:
	.xword	.LC69
	.align	3
.LC72:
	.xword	.LC71
	.align	3
.LC73:
	.xword	s_pstVfmwChan
	.align	3
.LC75:
	.xword	.LC74
	.align	3
.LC77:
	.xword	.LC76
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReleasePacket
	.type	ReleasePacket, %function
ReleasePacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	sub	w0, w20, #5
	cmp	w0, 12
	bls	.L455
.L431:
	ldr	x1, .LC87
	mov	w2, w20
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L455:
	ldr	x1, .LC78
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx433
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx433:
	.section	.rodata
	.align	0
	.align	2
.L433:
	.byte	(.L432 - .Lrtx433) / 4
	.byte	(.L431 - .Lrtx433) / 4
	.byte	(.L431 - .Lrtx433) / 4
	.byte	(.L434 - .Lrtx433) / 4
	.byte	(.L435 - .Lrtx433) / 4
	.byte	(.L436 - .Lrtx433) / 4
	.byte	(.L436 - .Lrtx433) / 4
	.byte	(.L436 - .Lrtx433) / 4
	.byte	(.L437 - .Lrtx433) / 4
	.byte	(.L431 - .Lrtx433) / 4
	.byte	(.L431 - .Lrtx433) / 4
	.byte	(.L431 - .Lrtx433) / 4
	.byte	(.L438 - .Lrtx433) / 4
	.text
	.p2align 3
.L438:
	ldr	x20, [x19]
	add	x19, x19, 290816
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 3392]
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x20, 242]
	cmp	w0, 1
	beq	.L445
	ldr	x1, .LC85
	mov	w0, 1
	bl	dprint_vfmw
.L445:
	ldr	x3, .LC79
	mov	x2, 176
	mov	w1, 0
	add	x0, x20, 232
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L432:
	ldr	x20, [x19]
	ldrb	w1, [x20, 242]
	cmp	w1, 1
	bne	.L445
	add	x19, x19, 40960
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 1248]
	bl	SM_ReleaseStreamSeg
	b	.L445
	.p2align 3
.L434:
	ldr	x20, [x19]
	ldrb	w1, [x20, 242]
	cmp	w1, 1
	bne	.L445
	add	x19, x19, 53248
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 1304]
	bl	SM_ReleaseStreamSeg
	b	.L445
	.p2align 3
.L435:
	ldr	x20, [x19]
	ldrb	w1, [x20, 242]
	cmp	w1, 1
	bne	.L445
	add	x19, x19, 53248
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 1272]
	bl	SM_ReleaseStreamSeg
	b	.L445
	.p2align 3
.L436:
	ldr	x20, [x19]
	ldr	w0, [x19, 13360]
	ldr	w1, [x20, 256]
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x20, 242]
	cmp	w0, 1
	beq	.L445
	ldr	x1, .LC83
	mov	w0, 1
	bl	dprint_vfmw
	b	.L445
	.p2align 3
.L437:
	ldr	x20, [x19]
	ldr	w0, [x19, 8816]
	ldr	w1, [x20, 256]
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x20, 242]
	cmp	w0, 1
	beq	.L445
	ldr	x1, .LC81
	mov	w0, 1
	bl	dprint_vfmw
	b	.L445
	.size	ReleasePacket, .-ReleasePacket
	.align	3
.LC78:
	.xword	.L433
	.align	3
.LC79:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC81:
	.xword	.LC80
	.align	3
.LC83:
	.xword	.LC82
	.align	3
.LC85:
	.xword	.LC84
	.align	3
.LC87:
	.xword	.LC86
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReleaseCurPic
	.type	ReleaseCurPic, %function
ReleaseCurPic:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	w20, 17
	bls	.L488
.L457:
	ldr	x1, .LC93
	mov	w2, w20
	mov	w0, 0
	bl	dprint_vfmw
.L456:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L488:
	ldr	x0, .LC88
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx459
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx459:
	.section	.rodata
	.align	0
	.align	2
.L459:
	.byte	(.L458 - .Lrtx459) / 4
	.byte	(.L460 - .Lrtx459) / 4
	.byte	(.L461 - .Lrtx459) / 4
	.byte	(.L462 - .Lrtx459) / 4
	.byte	(.L457 - .Lrtx459) / 4
	.byte	(.L463 - .Lrtx459) / 4
	.byte	(.L464 - .Lrtx459) / 4
	.byte	(.L457 - .Lrtx459) / 4
	.byte	(.L465 - .Lrtx459) / 4
	.byte	(.L466 - .Lrtx459) / 4
	.byte	(.L467 - .Lrtx459) / 4
	.byte	(.L467 - .Lrtx459) / 4
	.byte	(.L467 - .Lrtx459) / 4
	.byte	(.L468 - .Lrtx459) / 4
	.byte	(.L457 - .Lrtx459) / 4
	.byte	(.L469 - .Lrtx459) / 4
	.byte	(.L470 - .Lrtx459) / 4
	.byte	(.L471 - .Lrtx459) / 4
	.text
	.p2align 3
.L471:
	ldr	x20, [x19]
	add	x19, x19, 290816
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 3392]
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x20, 242]
	cmp	w0, 1
	beq	.L478
	ldr	x1, .LC89
	mov	w0, 1
	bl	dprint_vfmw
	b	.L478
	.p2align 3
.L458:
	mov	x0, x19
	bl	H264_ClearCurrPic
	b	.L456
	.p2align 3
.L461:
	mov	x0, x19
	bl	MP4_ClearCurPic
	mov	x0, x19
	bl	MP4_Clean_HeadPacket
	b	.L456
	.p2align 3
.L462:
	mov	x0, x19
	bl	MP2_ClearCurPic
	b	.L456
	.p2align 3
.L460:
	add	x1, x19, 53248
	add	x0, x19, 12288
	add	x0, x0, 2520
	ldr	w1, [x1, 800]
	bl	VC1_ClearCurPic
	b	.L456
	.p2align 3
.L463:
	ldr	x20, [x19]
	ldrb	w1, [x20, 242]
	cmp	w1, 1
	beq	.L489
.L478:
	ldr	x3, .LC90
	mov	x2, 176
	mov	w1, 0
	add	x0, x20, 232
	ldr	x3, [x3, 96]
	blr	x3
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L464:
	mov	x0, x19
	bl	AVS_ClearCurPic
	b	.L456
	.p2align 3
.L465:
	ldr	x20, [x19]
	ldrb	w1, [x20, 242]
	cmp	w1, 1
	bne	.L478
	add	x19, x19, 53248
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 1304]
	bl	SM_ReleaseStreamSeg
	b	.L478
	.p2align 3
.L466:
	ldr	x20, [x19]
	ldrb	w1, [x20, 242]
	cmp	w1, 1
	bne	.L478
	add	x19, x19, 53248
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 1272]
	bl	SM_ReleaseStreamSeg
	b	.L478
	.p2align 3
.L467:
	ldr	x20, [x19]
	ldr	w0, [x19, 13360]
	ldr	w1, [x20, 256]
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x20, 242]
	cmp	w0, 1
	beq	.L478
	ldr	x1, .LC92
	mov	w0, 1
	bl	dprint_vfmw
	b	.L478
	.p2align 3
.L468:
	ldr	x20, [x19]
	ldr	w0, [x19, 8816]
	ldr	w1, [x20, 256]
	bl	SM_ReleaseStreamSeg
	ldrb	w0, [x20, 242]
	cmp	w0, 1
	beq	.L478
	ldr	x1, .LC91
	mov	w0, 1
	bl	dprint_vfmw
	b	.L478
	.p2align 3
.L469:
	mov	x0, x19
	bl	MVC_ClearCurrPic
	b	.L456
	.p2align 3
.L470:
	mov	x0, x19
	bl	HEVC_ClearCurrPic
	b	.L456
	.p2align 3
.L489:
	add	x19, x19, 40960
	ldr	w1, [x20, 256]
	ldr	w0, [x19, 1248]
	bl	SM_ReleaseStreamSeg
	b	.L478
	.size	ReleaseCurPic, .-ReleaseCurPic
	.align	3
.LC88:
	.xword	.L459
	.align	3
.LC89:
	.xword	.LC84
	.align	3
.LC90:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC91:
	.xword	.LC80
	.align	3
.LC92:
	.xword	.LC82
	.align	3
.LC93:
	.xword	.LC86
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsNewpicSeg
	.type	IsNewpicSeg, %function
IsNewpicSeg:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w1
	mov	x20, x2
	bl	_mcount
	cmp	w19, 2
	mov	w1, 0
	ldr	x5, [x20]
	beq	.L533
.L491:
	ldrb	w2, [x5]
	ldrb	w3, [x5, 1]
	ldrb	w4, [x5, 2]
	cbnz	w2, .L494
	cbnz	w3, .L494
	cmp	w4, 1
	bne	.L494
	cmp	w19, 16
	bls	.L534
.L512:
	mov	w0, -1
.L497:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L534:
	ldr	x0, .LC96
	ldrb	w0, [x0,w19,uxtw]
	adr	x2, .Lrtx499
	add	x0, x2, w0, sxtb #2
	br	x0
.Lrtx499:
	.section	.rodata
	.align	0
	.align	2
.L499:
	.byte	(.L498 - .Lrtx499) / 4
	.byte	(.L500 - .Lrtx499) / 4
	.byte	(.L492 - .Lrtx499) / 4
	.byte	(.L501 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L502 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L512 - .Lrtx499) / 4
	.byte	(.L503 - .Lrtx499) / 4
	.byte	(.L504 - .Lrtx499) / 4
	.text
	.p2align 3
.L533:
	ldrb	w0, [x20, 80]
	and	w1, w0, 1
	tbz	x0, 0, .L491
.L492:
	ldrb	w2, [x5, 3]
	mov	w4, 176
	mov	w0, 47
	cmp	w2, 179
	add	w3, w2, 75
	ccmp	w2, w4, 4, ne
	cset	w4, eq
	cmp	w2, 254
	ccmp	w2, w0, 0, ne
	uxtb	w3, w3
	cset	w0, ls
	cmp	w3, 1
	orr	w0, w4, w0
	cset	w3, ls
	orr	w0, w3, w0
	cbnz	w0, .L535
.L506:
	mov	w0, -1
	cbnz	w1, .L507
.L508:
	cmp	w2, 182
	bne	.L497
.L532:
	str	wzr, [x21]
.L538:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L500:
	ldrb	w1, [x5, 3]
	mov	w0, -1
	sub	w2, w1, #12
	uxtb	w2, w2
	cmp	w2, 3
	ccmp	w1, 10, 4, hi
	cset	w2, eq
	cbnz	w2, .L536
.L510:
	cmp	w1, 13
	bne	.L497
	b	.L532
	.p2align 3
.L498:
	mov	x1, x5
	mov	x0, x21
	bl	H264_IsNewPicNal
	cmp	w0, wzr
	csetm	w0, ne
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L504:
	mov	x1, x5
	mov	x0, x21
	bl	HEVC_IsNewPicNal
	cmp	w0, wzr
	csetm	w0, ne
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L503:
	mov	x1, x5
	mov	x0, x21
	bl	MVC_IsNewPicNal
	cmp	w0, wzr
	csetm	w0, ne
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L502:
	ldrb	w2, [x5, 3]
	mov	w0, -1
	add	w3, w2, 74
	cmp	w2, 179
	cset	w4, eq
	uxtb	w3, w3
	cmp	w3, 1
	cset	w3, ls
	cmp	w2, 176
	cset	w1, eq
	orr	w1, w4, w1
	orr	w1, w3, w1
	cbnz	w1, .L537
.L509:
	cmp	w2, 182
	cset	w1, eq
	orr	w1, w4, w1
	cbz	w1, .L497
	str	wzr, [x21]
	b	.L538
	.p2align 3
.L501:
	ldrb	w1, [x5, 3]
	mov	w3, 179
	mov	w0, -1
	add	w2, w1, 73
	cmp	w1, wzr
	ccmp	w1, w3, 4, ne
	uxtb	w2, w2
	ccmp	w2, 1, 0, ne
	cset	w2, ls
	cbnz	w2, .L539
.L505:
	cbnz	w1, .L497
.L540:
	str	wzr, [x21]
	b	.L538
	.p2align 3
.L539:
	ldr	w2, [x21]
	cbnz	w2, .L505
	mov	w0, 1
	str	w0, [x21]
	mov	w0, 0
	ldrb	w1, [x5, 3]
	cbnz	w1, .L497
	b	.L540
	.p2align 3
.L535:
	ldr	w0, [x21]
	cbnz	w0, .L506
.L507:
	mov	w0, 1
	str	w0, [x21]
	mov	w0, 0
	ldrb	w2, [x5, 3]
	b	.L508
	.p2align 3
.L494:
	ldr	x1, .LC95
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L536:
	ldr	w2, [x21]
	cbnz	w2, .L510
	mov	w0, 1
	str	w0, [x21]
	mov	w0, 0
	ldrb	w1, [x5, 3]
	b	.L510
	.p2align 3
.L537:
	ldr	w1, [x21]
	cbnz	w1, .L509
	mov	w0, 1
	str	w0, [x21]
	mov	w0, 0
	ldrb	w2, [x5, 3]
	cmp	w2, 179
	cset	w4, eq
	b	.L509
	.size	IsNewpicSeg, .-IsNewpicSeg
	.align	3
.LC95:
	.xword	.LC94
	.align	3
.LC96:
	.xword	.L499
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	OpenUsdPool
	.type	OpenUsdPool, %function
OpenUsdPool:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC99
	add	x19, x19, 64
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L547
.L542:
	add	x1, x0, 204800
	str	wzr, [x0]
	add	x1, x1, 536
	add	x0, x0, 1048
	.p2align 2
.L544:
	stp	wzr, wzr, [x0, -4]
	add	x0, x0, 1064
	cmp	x0, x1
	bne	.L544
	mov	w0, 0
.L543:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L547:
	ldr	x1, .LC100
	mov	w0, 7688
	movk	w0, 0x3, lsl 16
	ldr	x1, [x1, 392]
	blr	x1
	str	x0, [x19, w20, sxtw 3]
	cbnz	x0, .L542
	mov	w0, -1
	b	.L543
	.size	OpenUsdPool, .-OpenUsdPool
	.align	3
.LC99:
	.xword	.LANCHOR0
	.align	3
.LC100:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CloseUsdPool
	.type	CloseUsdPool, %function
CloseUsdPool:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC102
	add	x19, x19, 64
	ldr	x0, [x19, w20, sxtw 3]
	cbz	x0, .L548
	ldr	x1, .LC103
	ldr	x1, [x1, 400]
	blr	x1
	str	xzr, [x19, w20, sxtw 3]
.L548:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CloseUsdPool, .-CloseUsdPool
	.align	3
.LC102:
	.xword	.LANCHOR0
	.align	3
.LC103:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	OpenUsdPool_WithMem
	.type	OpenUsdPool_WithMem, %function
OpenUsdPool_WithMem:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x1, .LC105
	add	x1, x1, 64
	ldr	x0, [x1, w19, sxtw 3]
	cbz	x0, .L559
.L554:
	add	x1, x0, 204800
	str	wzr, [x0]
	add	x1, x1, 536
	add	x0, x0, 1048
	.p2align 2
.L556:
	stp	wzr, wzr, [x0, -4]
	add	x0, x0, 1064
	cmp	x0, x1
	bne	.L556
	mov	w0, 0
.L555:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L559:
	str	x20, [x1, w19, sxtw 3]
	cbz	x20, .L557
	mov	x0, x20
	b	.L554
.L557:
	mov	w0, -1
	b	.L555
	.size	OpenUsdPool_WithMem, .-OpenUsdPool_WithMem
	.align	3
.LC105:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CloseUsdPool_WithMem
	.type	CloseUsdPool_WithMem, %function
CloseUsdPool_WithMem:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC107
	add	x0, x0, 64
	ldr	x1, [x0, w19, sxtw 3]
	cbz	x1, .L560
	str	xzr, [x0, w19, sxtw 3]
.L560:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CloseUsdPool_WithMem, .-CloseUsdPool_WithMem
	.align	3
.LC107:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InitUsdPool
	.type	InitUsdPool, %function
InitUsdPool:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC109
	add	x0, x0, 64
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L568
	add	x1, x0, 204800
	str	wzr, [x0]
	add	x1, x1, 536
	add	x0, x0, 1048
	.p2align 2
.L567:
	stp	wzr, wzr, [x0, -4]
	add	x0, x0, 1064
	cmp	x0, x1
	bne	.L567
	mov	w0, 0
.L566:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L568:
	mov	w0, -1
	b	.L566
	.size	InitUsdPool, .-InitUsdPool
	.align	3
.LC109:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetUsd
	.type	GetUsd, %function
GetUsd:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC111
	add	x0, x0, 64
	ldr	x3, [x0, w19, sxtw 3]
	cbz	x3, .L574
	add	x0, x3, 1048
	mov	w1, 0
	b	.L573
	.p2align 3
.L572:
	add	w1, w1, 1
	add	x0, x0, 1064
	cmp	w1, 192
	beq	.L574
.L573:
	ldr	w2, [x0]
	cbnz	w2, .L572
	ldr	w2, [x0, -4]
	cbnz	w2, .L572
	sxtw	x0, w1
	lsl	x1, x0, 7
	add	x1, x1, x0, lsl 2
	add	x0, x1, x0
	add	x1, x3, x0, lsl 3
	add	x0, x1, 8
	strb	wzr, [x1, 1034]
	strb	wzr, [x1, 1035]
	strb	wzr, [x1, 1036]
	strh	wzr, [x1, 1038]
	str	wzr, [x1, 1044]
	str	wzr, [x1, 1052]
	str	wzr, [x1, 1056]
	str	wzr, [x1, 1060]
	str	xzr, [x1, 1064]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L574:
	mov	x0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	GetUsd, .-GetUsd
	.align	3
.LC111:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FreeUsdByDec
	.type	FreeUsdByDec, %function
FreeUsdByDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L576
	str	wzr, [x19, 1040]
.L576:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FreeUsdByDec, .-FreeUsdByDec
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MarkUsdByDnr
	.type	MarkUsdByDnr, %function
MarkUsdByDnr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L581
	mov	w0, 1
	str	w0, [x19, 1036]
.L581:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MarkUsdByDnr, .-MarkUsdByDnr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UnMarkUsdByDnr
	.type	UnMarkUsdByDnr, %function
UnMarkUsdByDnr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L586
	str	wzr, [x19, 1036]
.L586:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	UnMarkUsdByDnr, .-UnMarkUsdByDnr
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsImageInVoQueue
	.type	IsImageInVoQueue, %function
IsImageInVoQueue:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L603
	ldrh	w0, [x19, 4]
	ldrh	w4, [x19, 8]
	and	w0, w0, 63
	.p2align 2
.L602:
	cmp	w0, w4
	beq	.L596
	sxtw	x2, w0
	add	w0, w0, 1
	add	x2, x2, 2
	and	w0, w0, 63
	ldr	x2, [x19, x2, lsl 3]
	cbz	x2, .L602
	ldr	w3, [x2, 156]
	ldr	w2, [x20, 156]
	cmp	w3, w2
	bne	.L602
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L596:
	mov	w0, -1
.L593:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L603:
	ldr	x1, .LC113
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L593
	.size	IsImageInVoQueue, .-IsImageInVoQueue
	.align	3
.LC113:
	.xword	.LC12
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsVoQueueEmpty
	.type	IsVoQueueEmpty, %function
IsVoQueueEmpty:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L608
	ldrh	w2, [x19, 6]
	mov	w0, 0
	ldrh	w1, [x19, 8]
	cmp	w1, w2
	beq	.L609
.L606:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L609:
	ldrh	w0, [x19, 4]
	ldr	x19, [sp, 16]
	cmp	w0, w1
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L608:
	ldr	x2, .LC114
	mov	w0, 0
	ldr	x1, .LC116
	add	x2, x2, 24
	bl	dprint_vfmw
	mov	w0, 0
	b	.L606
	.size	IsVoQueueEmpty, .-IsVoQueueEmpty
	.align	3
.LC114:
	.xword	.LANCHOR1
	.align	3
.LC116:
	.xword	.LC115
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SetAspectRatio
	.type	SetAspectRatio, %function
SetAspectRatio:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w20, 5
	bls	.L620
	ldr	x1, .LC119
	mov	w2, w20
	mov	w0, 1
	stp	wzr, w0, [x19]
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L620:
	ldr	x0, .LC117
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx613
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx613:
	.section	.rodata
	.align	0
	.align	2
.L613:
	.byte	(.L612 - .Lrtx613) / 4
	.byte	(.L614 - .Lrtx613) / 4
	.byte	(.L615 - .Lrtx613) / 4
	.byte	(.L616 - .Lrtx613) / 4
	.byte	(.L617 - .Lrtx613) / 4
	.byte	(.L618 - .Lrtx613) / 4
	.text
	.p2align 3
.L617:
	mov	w1, 235
	mov	w0, 100
	stp	w1, w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L618:
	mov	w0, 2
	stp	wzr, w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L612:
	mov	w0, 1
	stp	wzr, w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L614:
	mov	w1, 4
	mov	w0, 3
	stp	w1, w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L615:
	mov	w1, 16
	mov	w0, 9
	stp	w1, w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L616:
	mov	w1, 221
	mov	w0, 100
	stp	w1, w0, [x19]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	SetAspectRatio, .-SetAspectRatio
	.align	3
.LC117:
	.xword	.L613
	.align	3
.LC119:
	.xword	.LC118
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetVoLastImageID
	.type	GetVoLastImageID, %function
GetVoLastImageID:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 552]
	ldr	x19, [sp, 16]
	cmp	w0, 1
	csinv	w0, w0, wzr, hi
	ldp	x29, x30, [sp], 32
	ret
	.size	GetVoLastImageID, .-GetVoLastImageID
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsMain10Profile
	.type	IsMain10Profile, %function
IsMain10Profile:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 86016
	ldr	w2, [x1, 936]
	cmp	w2, 1
	beq	.L627
	ldr	w3, [x1, 940]
	mov	w0, 0
	cmp	w3, 1
	beq	.L630
	cmp	w2, 2
	beq	.L629
	ldr	w0, [x1, 944]
	cmp	w0, 1
	cset	w0, eq
.L626:
	ldr	w1, [x1, 944]
	cmp	w1, 1
	cset	w1, eq
	orr	w0, w1, w0
.L630:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L627:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L629:
	mov	w0, 1
	b	.L626
	.size	IsMain10Profile, .-IsMain10Profile
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MetadataMapToSec
	.type	MetadataMapToSec, %function
MetadataMapToSec:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	MetadataMapToSec, .-MetadataMapToSec
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MetadataUnMapToSec
	.type	MetadataUnMapToSec, %function
MetadataUnMapToSec:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	MetadataUnMapToSec, .-MetadataUnMapToSec
	.global	pstVdecUsdPool
	.global	g_DnrEnable
	.section	.rodata
	.align	3
.LANCHOR1 = . + 0
	.type	__FUNCTION__.12698, %object
	.size	__FUNCTION__.12698, 19
__FUNCTION__.12698:
	.string	"InsertImgToVoQueue"
	.zero	5
	.type	__func__.12944, %object
	.size	__func__.12944, 15
__func__.12944:
	.string	"IsVoQueueEmpty"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"Initializing VO queue\n" )
	.zero	1
.LC2:
	ASCII(.string	"VoQueue state: head(%d) history(%d) tail(%d)" )
	.zero	3
.LC4:
	ASCII(.string	"\n" )
	.zero	6
.LC6:
	ASCII(.string	"%d " )
	.zero	4
.LC9:
	ASCII(.string	"ResetVoQueue but queue not empty: head(%d) history(%d) tail(%d)\n" )
	.zero	7
.LC12:
	ASCII(.string	"image point NULL\n" )
	.zero	6
.LC14:
	ASCII(.string	"call InsertVOQueue\n" )
	.zero	4
.LC17:
	ASCII(.string	"FATAL: p_image(id=%d) has already in the VoQueue!\n" )
	.zero	5
.LC19:
	ASCII(.string	"error=%d,W=%d,H=%d,not acceptable!\n" )
	.zero	4
.LC21:
	ASCII(.string	"out_pts: %lld -> %lld Usertag %lld\n" )
	.zero	4
.LC24:
	ASCII(.string	"Queue=(%02d,%02d,%02d) + F%d\n" )
	.zero	2
.LC34:
	ASCII(.string	"Queue=(%02d,%02d,%02d) - F%d\n" )
	.zero	2
.LC36:
	ASCII(.string	"SYNTAX_ReleaseImage p_image is null\n" )
	.zero	3
.LC40:
	ASCII(.string	"Image %d not found in vo queue.\n" )
	.zero	7
.LC42:
	ASCII(.string	"image[%d] null\n" )
.LC44:
	ASCII(.string	"Queue=(%02d,%02d,%02d) x F%d\n" )
	.zero	2
.LC49:
	ASCII(.string	"pSegMan error!\n" )
.LC53:
	ASCII(.string	"More than two packet!!!ERROR!!!\n" )
	.zero	7
.LC56:
	ASCII(.string	"*****************copy packets tobe one*****************\n" )
	.zero	7
.LC58:
	ASCII(.string	"*****************two packet*****************\n" )
	.zero	2
.LC63:
	ASCII(.string	"pSegMan->CurrSeg.IsLastSeg=%d\n" )
	.zero	1
.LC65:
	ASCII(.string	"pSegMan->CurrSeg.PhyAddr=%#x\n" )
	.zero	2
.LC67:
	ASCII(.string	"pSegMan->CurrSeg.LenInByte=%#x\n" )
.LC69:
	ASCII(.string	"ERROR: Wrong PreParseInfo about Vc1 IsAdvProfile!\n" )
	.zero	5
.LC71:
	ASCII(.string	"ERROR: Wrong PreParseInfo about Vc1 CodecVersion!\n" )
	.zero	5
.LC74:
	ASCII(.string	"LENTH ERROR: VC1 S/MP BS is wrong!\n" )
	.zero	4
.LC76:
	ASCII(.string	"SYNTAX ERROR: VC1 S/MP BS is wrong!\n" )
	.zero	3
.LC80:
	ASCII(.string	"ERROR: VFMW_VP8 one packet stream, but IsLastSeg == 0\n" )
	.zero	1
.LC82:
	ASCII(.string	"ERROR: VFMW_VP6 one packet stream, but IsLastSeg == 0\n" )
	.zero	1
.LC84:
	ASCII(.string	"ERROR: VFMW_VP9 one packet stream, but IsLastSeg == 0\n" )
	.zero	1
.LC86:
	ASCII(.string	"VidStd=%d Error!\n" )
	.zero	6
.LC94:
	ASCII(.string	"pu8[0] = %x,pu8[1] = %x, pu8[2] = %x\n" )
	.zero	2
.LC115:
	ASCII(.string	"%s pstVoQue NULL\n" )
	.zero	6
.LC118:
	ASCII(.string	"WARNNING: aspect ration (%d) is not expected!\n" )
	.bss
	.align	3
.LANCHOR0 = . + 0
	.type	last_rec_pos_time.12690, %object
	.size	last_rec_pos_time.12690, 64
last_rec_pos_time.12690:
	.zero	64
	.type	pstVdecUsdPool, %object
	.size	pstVdecUsdPool, 128
pstVdecUsdPool:
	.zero	128
	.type	g_DnrEnable, %object
	.size	g_DnrEnable, 4
g_DnrEnable:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
