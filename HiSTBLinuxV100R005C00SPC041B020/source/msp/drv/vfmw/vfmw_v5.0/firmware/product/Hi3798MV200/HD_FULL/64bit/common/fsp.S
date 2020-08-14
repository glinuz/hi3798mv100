#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"fsp.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.type	FSP_IsSpecialFrm.part.0, %function
FSP_IsSpecialFrm.part.0:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x0, .LC0
	ldr	x3, [x0, w20, sxtw 3]
	cbz	x3, .L5
	sub	w1, w19, #1
	mov	w0, 0
	cmn	w1, #3
	bhi	.L2
	add	x2, x3, 327680
	ldr	w1, [x2, 2704]
	cbz	w1, .L2
	ldr	w0, [x2, 2196]
	cmp	w19, w0
	beq	.L9
	mov	w2, 0
	b	.L3
	.p2align 3
.L4:
	add	x0, x3, x0, lsl 4
	ldr	w0, [x0, 2196]
	cmp	w19, w0
	beq	.L9
.L3:
	add	w2, w2, 1
	cmp	w2, w1
	sxtw	x0, w2
	add	x0, x0, 20480
	bne	.L4
.L5:
	mov	w0, 0
.L2:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L9:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_IsSpecialFrm.part.0, .-FSP_IsSpecialFrm.part.0
	.align	3
.LC0:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	FSP_IsFsStatusReady.part.5, %function
FSP_IsFsStatusReady.part.5:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	mov	w0, 1
	ble	.L14
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L14:
	ldr	w1, [x20, 16]
	mov	w0, w19
	bl	FSP_IsSpecialFrm.part.0
	cmp	w0, wzr
	cset	w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_IsFsStatusReady.part.5, .-FSP_IsFsStatusReady.part.5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_Init
	.type	FSP_Init, %function
FSP_Init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x0, x30
	bl	_mcount
	ldr	x21, .LC3
	ldr	x19, .LC4
	mov	x2, 128
	mov	w1, 0
	ldr	x3, [x21, 96]
	mov	x0, x19
	add	x20, x19, x2
	blr	x3
	ldr	x3, [x21, 96]
	mov	x2, 4224
	mov	w1, 0
	mov	x0, x20
	blr	x3
	add	x1, x20, 4096
	add	x0, x19, 130
	add	x1, x1, 130
	mov	w2, -1
	.p2align 2
.L16:
	strh	w2, [x0]
	add	x0, x0, 264
	cmp	x0, x1
	bne	.L16
	ldr	x0, .LC5
	mov	x2, 32
	ldr	x3, [x21, 96]
	mov	w1, 0
	str	wzr, [x0], 8
	blr	x3
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	FSP_Init, .-FSP_Init
	.align	3
.LC3:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC4:
	.xword	.LANCHOR0
	.align	3
.LC5:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ConfigAttribute
	.type	FSP_ConfigAttribute, %function
FSP_ConfigAttribute:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, 15
	bhi	.L22
	ldr	x0, .LC11
	ldr	x2, [x0, w19, sxtw 3]
	ldr	w0, [x2, 4]
	cmp	w0, 1
	bne	.L23
	ldrsb	w1, [x20]
	mov	w0, 0
	strb	w1, [x2, 1]
.L20:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L23:
	ldr	x3, .LC13
	mov	w2, 234
	ldr	x1, .LC10
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L20
	.p2align 3
.L22:
	ldr	x3, .LC8
	mov	w2, 233
	ldr	x1, .LC10
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L20
	.size	FSP_ConfigAttribute, .-FSP_ConfigAttribute
	.align	3
.LC8:
	.xword	.LC7
	.align	3
.LC10:
	.xword	.LC9
	.align	3
.LC11:
	.xword	.LANCHOR0
	.align	3
.LC13:
	.xword	.LC12
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ClearContext
	.type	FSP_ClearContext, %function
FSP_ClearContext:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L27
	ldr	x20, .LC17
	add	x21, x19, 28672
	add	x4, x21, 256
	strb	wzr, [x19, 8]
	strb	wzr, [x19, 9]
	mov	x2, 32
	strb	wzr, [x19, 11]
	mov	w1, 0
	strb	wzr, [x19, 12]
	add	x0, x19, 20
	strb	wzr, [x19, 13]
	add	x22, x19, 20480
	strb	wzr, [x19, 14]
	str	wzr, [x19, 16]
	add	x19, x19, 24576
	str	wzr, [x21, 228]
	ldr	x3, [x20, 96]
	stp	wzr, wzr, [x4, 136]
	stp	wzr, wzr, [x4, 144]
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 100
	mov	w1, 0
	add	x0, x22, 1336
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 3200
	mov	w1, 0
	add	x0, x22, 1436
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 3200
	mov	w1, 0
	add	x0, x19, 540
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 200
	mov	w1, 0
	add	x0, x19, 3740
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 240
	mov	w1, 0
	add	x0, x19, 3940
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 160
	mov	w1, 0
	add	x0, x21, 232
	blr	x3
	mov	w0, 0
.L26:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L27:
	ldr	x3, .LC15
	mov	w2, 250
	ldr	x1, .LC16
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L26
	.size	FSP_ClearContext, .-FSP_ClearContext
	.align	3
.LC15:
	.xword	.LC14
	.align	3
.LC16:
	.xword	.LC9
	.align	3
.LC17:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ClearContextAll
	.type	FSP_ClearContextAll, %function
FSP_ClearContextAll:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L31
	ldr	x3, .LC20
	add	x20, x19, 28672
	mov	x2, 29080
	mov	w1, 0
	mov	x0, x19
	ldrsb	w22, [x19, 10]
	ldr	x3, [x3, 96]
	ldrsb	w25, [x19]
	ldrsb	w24, [x19, 1]
	ldr	w23, [x19, 4]
	ldr	w21, [x20, 392]
	blr	x3
	strb	w25, [x19]
	mov	w0, 0
	strb	w24, [x19, 1]
	strb	w22, [x19, 10]
	str	w23, [x19, 4]
	str	w21, [x20, 392]
.L30:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L31:
	ldr	x3, .LC18
	mov	w2, 286
	ldr	x1, .LC19
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L30
	.size	FSP_ClearContextAll, .-FSP_ClearContextAll
	.align	3
.LC18:
	.xword	.LC14
	.align	3
.LC19:
	.xword	.LC9
	.align	3
.LC20:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_EnableInstance
	.type	FSP_EnableInstance, %function
FSP_EnableInstance:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	cmp	w20, 15
	bhi	.L37
	ldr	x0, .LC23
	ldr	x19, [x0, w20, sxtw 3]
	cbz	x19, .L38
	ldr	w0, [x19, 4]
	cbnz	w0, .L36
	ldr	x3, .LC26
	mov	w1, 0
	mov	x2, 29080
	mov	x0, x19
	ldr	x3, [x3, 96]
	blr	x3
	strb	w20, [x19]
	add	x1, x19, 28672
	mov	w0, 1
	str	w0, [x19, 4]
	mov	w0, 0
	str	wzr, [x1, 392]
.L34:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L36:
	mov	w0, -1
	b	.L34
	.p2align 3
.L37:
	ldr	x3, .LC21
	mov	w2, 312
	ldr	x1, .LC22
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L34
.L38:
	ldr	x3, .LC25
	mov	w2, 313
	ldr	x1, .LC22
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L34
	.size	FSP_EnableInstance, .-FSP_EnableInstance
	.align	3
.LC21:
	.xword	.LC7
	.align	3
.LC22:
	.xword	.LC9
	.align	3
.LC23:
	.xword	.LANCHOR0
	.align	3
.LC25:
	.xword	.LC24
	.align	3
.LC26:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_DisableInstance
	.type	FSP_DisableInstance, %function
FSP_DisableInstance:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L43
	ldr	x0, .LC29
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L44
	ldr	w1, [x0, 4]
	cmp	w1, 1
	beq	.L45
.L39:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L45:
	str	wzr, [x0, 4]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L43:
	ldr	x3, .LC27
	mov	w2, 335
	ldr	x1, .LC28
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L44:
	ldr	x3, .LC30
	mov	w2, 336
	ldr	x1, .LC28
	bl	dprint_vfmw
	b	.L39
	.size	FSP_DisableInstance, .-FSP_DisableInstance
	.align	3
.LC27:
	.xword	.LC7
	.align	3
.LC28:
	.xword	.LC9
	.align	3
.LC29:
	.xword	.LANCHOR0
	.align	3
.LC30:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_Exit
	.type	FSP_Exit, %function
FSP_Exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x0, x30
	mov	x19, 0
	bl	_mcount
	ldr	x20, .LC33
	b	.L48
	.p2align 3
.L47:
	add	x19, x19, 1
	cmp	x19, 16
	beq	.L56
.L48:
	ldr	x0, [x20, x19, lsl 3]
	cbz	x0, .L47
	ldr	w0, [x0, 4]
	cbz	w0, .L47
	mov	w0, w19
	add	x19, x19, 1
	bl	FSP_DisableInstance
	cmp	x19, 16
	bne	.L48
.L56:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_Exit, .-FSP_Exit
	.align	3
.LC33:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ResetInstance
	.type	FSP_ResetInstance, %function
FSP_ResetInstance:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w21, w0
	mov	x0, x30
	bl	_mcount
	cmp	w21, 15
	bhi	.L78
	ldr	x0, .LC39
	ldr	x22, [x0, w21, sxtw 3]
	cbz	x22, .L79
	add	x19, x22, 20480
	add	x20, x22, 24576
	ldr	x25, .LC41
	add	x19, x19, 1452
	ldr	x24, .LC45
	add	x20, x20, 556
	ldr	x23, .LC43
	b	.L67
	.p2align 3
.L64:
	ldrsb	w0, [x19, -14]
	cbnz	w0, .L80
	.p2align 2
.L62:
	add	x19, x19, 100
	cmp	x20, x19
	beq	.L81
.L67:
	ldr	w1, [x19]
	cbz	w1, .L62
	ldr	w0, [x25]
	cbz	w0, .L62
	ldrsb	w0, [x19, -16]
	cbz	w0, .L64
	ldr	x3, [x25, 32]
	cbz	x3, .L82
	mov	w2, 0
	mov	w0, w21
	blr	x3
	ldrsb	w0, [x19, -14]
	cbz	w0, .L62
.L80:
	ldr	x2, [x25, 24]
	ldr	w1, [x19]
	cbz	x2, .L83
	mov	w0, w21
	add	x19, x19, 100
	blr	x2
	cmp	x20, x19
	bne	.L67
	.p2align 2
.L81:
	mov	x0, x22
	bl	FSP_ClearContext
	mov	w0, 0
.L59:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L78:
	ldr	x3, .LC37
	mov	w2, 356
	ldr	x1, .LC38
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L59
.L83:
	mov	x1, x24
	mov	w0, 0
	bl	dprint_vfmw
	b	.L62
.L82:
	mov	x1, x23
	mov	w0, 0
	bl	dprint_vfmw
	b	.L64
.L79:
	ldr	x3, .LC40
	mov	w2, 357
	ldr	x1, .LC38
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L59
	.size	FSP_ResetInstance, .-FSP_ResetInstance
	.align	3
.LC37:
	.xword	.LC7
	.align	3
.LC38:
	.xword	.LC9
	.align	3
.LC39:
	.xword	.LANCHOR0
	.align	3
.LC40:
	.xword	.LC24
	.align	3
.LC41:
	.xword	.LANCHOR1
	.align	3
.LC43:
	.xword	.LC42
	.align	3
.LC45:
	.xword	.LC44
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_EmptyInstance
	.type	FSP_EmptyInstance, %function
FSP_EmptyInstance:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	w25, w0
	mov	x0, x30
	bl	_mcount
	cmp	w25, 15
	bhi	.L121
	ldr	x0, .LC48
	ldr	x24, [x0, w25, sxtw 3]
	cbz	x24, .L122
	ldr	x3, .LC50
	mov	x2, 21760
	mov	w1, 0
	add	x0, x24, 56
	mov	w26, 0
	ldr	x3, [x3, 96]
	blr	x3
	ldrsb	w0, [x24, 10]
	cmp	w0, wzr
	ble	.L98
	ldr	x27, .LC51
	mov	w21, 4
	ldr	x23, .LC53
	ldr	x22, .LC52
	b	.L113
	.p2align 3
.L95:
	lsl	x0, x19, 2
	sub	x1, x0, x19
	lsl	x0, x1, 5
	add	x0, x1, x0
	add	x0, x0, x19
	add	x0, x24, x0
	add	x0, x0, 20480
	strb	wzr, [x0, 1438]
.L96:
	lsl	x0, x19, 2
	add	w26, w26, 1
	sub	x0, x0, x19
	lsl	x1, x0, 5
	add	x0, x0, x1
	add	x19, x0, x19
	add	x19, x24, x19
	add	x19, x19, 20480
	strb	wzr, [x19, 1436]
	strb	wzr, [x19, 1528]
	str	wzr, [x19, 1524]
	ldrsb	w0, [x24, 10]
	cmp	w0, w26
	ble	.L98
.L113:
	sxtw	x19, w26
	lsl	x20, x19, 2
	sub	x20, x20, x19
	lsl	x0, x20, 5
	add	x20, x20, x0
	add	x20, x20, x19
	add	x20, x24, x20
	add	x20, x20, 20480
	ldr	w1, [x20, 1452]
	cbz	w1, .L91
	ldr	w0, [x27]
	cbz	w0, .L91
	ldrsb	w0, [x20, 1436]
	cbnz	w0, .L123
.L92:
	lsl	x20, x19, 2
	sub	x20, x20, x19
	lsl	x0, x20, 5
	add	x20, x20, x0
	add	x20, x20, x19
	add	x20, x24, x20
	add	x20, x20, 20480
	ldrsb	w0, [x20, 1438]
	cbnz	w0, .L124
	.p2align 2
.L91:
	cmp	w25, 15
	bgt	.L95
	mov	w0, w25
	bl	FSP_IsSpecialFrm.part.0
	cmp	w0, 1
	bne	.L95
	lsl	x0, x19, 2
	sub	x1, x0, x19
	lsl	x0, x1, 5
	add	x0, x1, x0
	add	x0, x0, x19
	add	x0, x24, x0
	add	x0, x0, 20480
	strb	w21, [x0, 1438]
	b	.L96
	.p2align 3
.L98:
	ldrsb	w0, [x24, 11]
	cmp	w0, wzr
	ble	.L90
	sub	w0, w0, #1
	mov	w20, 100
	mov	x1, 25218
	add	x19, x24, 24576
	add	x19, x19, 542
	mov	w21, 4
	umaddl	x20, w0, w20, x1
	add	x20, x24, x20
	.p2align 2
.L103:
	cmp	w25, 15
	ble	.L125
.L101:
	strb	wzr, [x19]
.L102:
	strb	wzr, [x19, -2]
	add	x19, x19, 100
	strb	wzr, [x19, -10]
	str	wzr, [x19, -14]
	cmp	x19, x20
	bne	.L103
.L90:
	ldrsb	w0, [x24, 13]
	cmp	w0, wzr
	ble	.L100
	sub	w1, w0, #1
	mov	x3, 28916
	add	x0, x24, 28672
	mov	w2, 1
	add	x1, x3, x1, uxtw 3
	add	x0, x0, 236
	add	x1, x24, x1
	.p2align 2
.L104:
	strb	w2, [x0]
	add	x0, x0, 8
	strb	wzr, [x0, -7]
	cmp	x1, x0
	bne	.L104
.L100:
	mov	w0, 0
	str	wzr, [x24, 16]
.L86:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L125:
	ldr	w1, [x19, 14]
	mov	w0, w25
	bl	FSP_IsSpecialFrm.part.0
	cmp	w0, 1
	bne	.L101
	strb	w21, [x19]
	b	.L102
	.p2align 3
.L124:
	ldr	x2, [x27, 24]
	cbz	x2, .L126
	mov	w0, w25
	blr	x2
	ldr	w1, [x20, 1452]
	b	.L91
	.p2align 3
.L123:
	ldr	x3, [x27, 32]
	cbz	x3, .L127
	mov	w2, 0
	mov	w0, w25
	blr	x3
	ldr	w1, [x20, 1452]
	b	.L92
.L121:
	ldr	x3, .LC46
	mov	w2, 411
	ldr	x1, .LC47
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L86
.L122:
	ldr	x3, .LC49
	mov	w2, 412
	ldr	x1, .LC47
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L86
.L127:
	mov	x1, x22
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 1452]
	b	.L92
.L126:
	mov	x1, x23
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 1452]
	b	.L91
	.size	FSP_EmptyInstance, .-FSP_EmptyInstance
	.align	3
.LC46:
	.xword	.LC7
	.align	3
.LC47:
	.xword	.LC9
	.align	3
.LC48:
	.xword	.LANCHOR0
	.align	3
.LC49:
	.xword	.LC24
	.align	3
.LC50:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC51:
	.xword	.LANCHOR1
	.align	3
.LC52:
	.xword	.LC42
	.align	3
.LC53:
	.xword	.LC44
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetUsedBufferNum
	.type	FSP_GetUsedBufferNum, %function
FSP_GetUsedBufferNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC54
	ldr	x5, [x0, w19, sxtw 3]
	add	x0, x5, 294912
	ldrsb	w0, [x0, 2234]
	cmp	w0, wzr
	ble	.L132
	mov	x2, 57010
	sub	w4, w0, #1
	mov	w3, 100
	movk	x2, 0x4, lsl 16
	add	x1, x5, 315392
	mov	w0, 0
	umaddl	x2, w4, w3, x2
	add	x1, x1, 3662
	add	x3, x5, x2
	.p2align 2
.L131:
	ldrsb	w2, [x1], 100
	cmp	w2, 3
	csinc	w0, w0, w0, ne
	cmp	x1, x3
	bne	.L131
.L129:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L132:
	mov	w0, 0
	b	.L129
	.size	FSP_GetUsedBufferNum, .-FSP_GetUsedBufferNum
	.align	3
.LC54:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ConfigInstance
	.type	FSP_ConfigInstance, %function
FSP_ConfigInstance:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	cmp	w20, 15
	bhi	.L144
	cbz	x21, .L145
	ldr	x0, .LC59
	ldr	x19, [x0, w20, sxtw 3]
	cbz	x19, .L138
	ldr	w0, [x19, 4]
	cmp	w0, 1
	bne	.L138
	ldr	x0, .LC62
	ldr	x0, [x0, w20, sxtw 3]
	add	x0, x0, 323584
	ldr	w0, [x0, 2632]
	cbz	w0, .L146
.L140:
	ldr	x3, .LC63
	mov	x2, 32
	mov	x1, x21
	add	x0, x19, 20
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 0
.L136:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L146:
	mov	x0, x19
	bl	FSP_ClearContext
	b	.L140
	.p2align 3
.L138:
	ldr	x3, .LC61
	mov	w2, 539
	ldr	x1, .LC56
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L136
	.p2align 3
.L144:
	ldr	x3, .LC55
	mov	w2, 536
	ldr	x1, .LC56
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L136
.L145:
	ldr	x3, .LC58
	mov	w2, 537
	ldr	x1, .LC56
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L136
	.size	FSP_ConfigInstance, .-FSP_ConfigInstance
	.align	3
.LC55:
	.xword	.LC7
	.align	3
.LC56:
	.xword	.LC9
	.align	3
.LC58:
	.xword	.LC57
	.align	3
.LC59:
	.xword	.LANCHOR0
	.align	3
.LC61:
	.xword	.LC60
	.align	3
.LC62:
	.xword	s_pstVfmwChan
	.align	3
.LC63:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_CheckCapLevel
	.type	FSP_CheckCapLevel, %function
FSP_CheckCapLevel:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC64
	ldr	x0, [x0, w19, sxtw 3]
	add	x0, x0, 327680
	ldr	w0, [x0, 2716]
	cbz	w0, .L148
.L152:
	mov	w0, 0
.L156:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L148:
	ldr	x1, .LC65
	mov	w0, w19
	ldr	x19, [x1, w19, sxtw 3]
	bl	VCTRL_GetChanCapLevel
	cmp	w0, 32
	bhi	.L150
	uxtw	x4, w0
	ldr	x3, .LC66
	lsl	x1, x4, 3
	add	x2, x19, 20
	sub	x1, x1, x4
	lsl	x1, x1, 2
	add	x4, x3, x1
	ldp	w5, w6, [x2, 8]
	ldr	w3, [x3, x1]
	ldr	w4, [x4, 4]
	mul	w7, w5, w6
	mul	w1, w3, w4
	cmp	w7, w1
	ccmp	w0, 25, 4, gt
	beq	.L152
	ldrsb	w1, [x2, 2]
	cmp	w1, 1
	beq	.L152
	ldr	x1, .LC70
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	b	.L153
	.p2align 3
.L150:
	ldrsb	w1, [x19, 22]
	cmp	w1, 1
	beq	.L152
	ldr	x1, .LC68
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
.L153:
	strb	wzr, [x19, 9]
	mov	w0, -1
	b	.L156
	.size	FSP_CheckCapLevel, .-FSP_CheckCapLevel
	.align	3
.LC64:
	.xword	s_pstVfmwChan
	.align	3
.LC65:
	.xword	.LANCHOR0
	.align	3
.LC66:
	.xword	CapItem
	.align	3
.LC68:
	.xword	.LC67
	.align	3
.LC70:
	.xword	.LC69
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_RecordSpecialFrame
	.type	FSP_RecordSpecialFrame, %function
FSP_RecordSpecialFrame:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC71
	ldr	x3, [x0, w19, sxtw 3]
	add	x2, x3, 327680
	ldr	w0, [x2, 2708]
	cbnz	w0, .L159
	ldr	w0, [x2, 2704]
	cbz	w0, .L159
	ldr	x0, .LC72
	mov	w1, 0
	mov	w7, 4
	ldr	x4, [x0, w19, sxtw 3]
	add	x0, x4, 28672
	ldr	w0, [x0, 396]
	lsl	x5, x0, 2
	sub	x5, x5, x0
	lsl	x6, x5, 5
	add	x5, x5, x6
	add	x0, x5, x0
	add	x4, x4, x0
	add	x0, x4, 20480
	ldr	w5, [x0, 1452]
	b	.L163
	.p2align 3
.L162:
	ldr	w0, [x2, 2704]
	cmp	w0, w1
	bls	.L159
.L163:
	uxtw	x0, w1
	add	w1, w1, 1
	add	x0, x0, 20480
	add	x0, x3, x0, lsl 4
	ldr	w0, [x0, 2196]
	cmp	w5, w0
	bne	.L162
	add	x0, x4, 20480
	strb	w7, [x0, 1438]
	ldr	w0, [x2, 2704]
	cmp	w0, w1
	bhi	.L163
.L159:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_RecordSpecialFrame, .-FSP_RecordSpecialFrame
	.align	3
.LC71:
	.xword	s_pstVfmwChan
	.align	3
.LC72:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ConfigFrameBuf
	.type	FSP_ConfigFrameBuf, %function
FSP_ConfigFrameBuf:
	sub	sp, sp, #128
	stp	x29, x30, [sp, 16]
	add	x29, sp, 16
	stp	x19, x20, [sp, 32]
	stp	x21, x22, [sp, 48]
	stp	x23, x24, [sp, 64]
	stp	x25, x26, [sp, 80]
	stp	x27, x28, [sp, 96]
	mov	w19, w0
	mov	x0, x30
	mov	w22, w1
	mov	w27, w2
	str	x3, [x29, 104]
	bl	_mcount
	ldr	x23, .LC73
	mov	x2, 1272
	ldr	x20, .LC74
	mov	w1, 0
	ldr	x3, [x23, 96]
	add	x0, x20, 40
	blr	x3
	cmp	w19, 15
	bhi	.L217
	ldr	x0, .LC77
	ldr	x25, [x0, w19, sxtw 3]
	cbz	x25, .L218
	mov	w0, w19
	bl	FSP_CheckCapLevel
	mov	w21, w0
	cbnz	w0, .L174
	ldr	x28, .LC79
	add	x26, x25, 20
	ldr	x1, [x28, w19, sxtw 3]
	ldp	w4, w5, [x26, 8]
	ldp	w2, w3, [x1, 160]
	mul	w7, w4, w5
	mul	w6, w2, w3
	cmp	w7, w6
	bgt	.L219
	ldr	w0, [x1, 1552]
	ldrsb	w24, [x26, 3]
	cmp	w0, 1
	add	x0, x1, 323584
	beq	.L220
.L175:
	ldr	w2, [x0, 2640]
	cmp	w24, w2
	beq	.L177
	mov	w2, 1
	str	w2, [x0, 2656]
	ldr	x2, .LC83
	ldr	x9, [x2, 128]
	ldrsb	w2, [x26, 3]
	str	w2, [x0, 2640]
	cbz	x9, .L179
.L224:
	ldp	w2, w3, [x26, 8]
	ldrsb	w6, [x26, 2]
	mov	w1, w27
	ldrsb	w4, [x26, 6]
	mov	w0, w22
	mov	w7, w19
	add	x22, x20, 40
	mov	w5, 32
	str	x22, [sp]
	blr	x9
	ldr	x1, [x28, w19, sxtw 3]
	mov	w2, 20
	add	x1, x1, 327680
	ldr	w1, [x1, 2120]
	cmp	w1, 20
	csel	w1, w1, w2, le
	cmp	w0, 2
	strb	w1, [x25, 13]
	beq	.L221
	cbnz	w0, .L182
	ldr	w0, [x22, 1080]
	strb	w0, [x25, 9]
	mov	w0, 31
	ldr	x1, .LC87
	ldr	x2, [x28, w19, sxtw 3]
	str	wzr, [x2, 1592]
	bl	dprint_vfmw
	ldr	w4, [x22, 1080]
	cmp	w24, w4
	bgt	.L222
	ldrsb	w0, [x25, 1]
	strb	wzr, [x25, 11]
	strb	wzr, [x25, 12]
	cbnz	w0, .L223
.L185:
	ldr	w0, [x20, 1120]
	mov	w1, 32
	cmp	w0, w1
	csel	w0, w0, w1, le
	strb	w0, [x25, 10]
.L186:
	ldr	x1, [x23, 24]
	mov	w0, 8
	add	x27, x20, 40
	add	x28, x25, 28672
	blr	x1
	ldr	w0, [x27, 1252]
	cbz	w0, .L191
	ldr	w5, [x28, 396]
	mov	w24, 0
	mov	w22, 1
	.p2align 2
.L190:
	uxtw	x0, w5
	uxtw	x2, w24
	lsl	x1, x0, 2
	lsl	x3, x2, 2
	sub	x1, x1, x0
	sub	x3, x3, x2
	lsl	x6, x1, 5
	add	x7, x27, x2, lsl 2
	add	x1, x1, x6
	add	x3, x27, x3, lsl 2
	add	x1, x1, x0
	ldr	w2, [x27, 1032]
	add	x1, x25, x1
	ldr	w12, [x7, 392]
	add	x1, x1, 20480
	ldr	w6, [x27, 1048]
	ldp	w0, w13, [x3, 8]
	ldr	w3, [x7, 904]
	add	w24, w24, 1
	str	w0, [x1, 1452]
	add	x0, x1, 1280
	str	w5, [x1, 1444]
	str	w13, [x1, 1456]
	ldr	w11, [x27]
	ldr	w7, [x27, 1044]
	ldr	w10, [x27, 1052]
	ldr	w9, [x27, 1060]
	ldr	w8, [x27, 1064]
	stp	w3, w2, [x0, 192]
	add	x2, x1, 1280
	stp	w12, w11, [x0, 184]
	stp	w10, w9, [x2, 204]
	stp	w8, w7, [x2, 212]
	str	w6, [x1, 1500]
	ldr	w0, [x27, 1036]
	str	w0, [x1, 1480]
	mov	w0, w19
	ldr	w5, [x27, 1068]
	ldr	w3, [x26, 8]
	str	w3, [x1, 1504]
	ldr	w2, [x27, 1072]
	ldr	w3, [x26, 12]
	str	w3, [x1, 1508]
	str	w5, [x1, 1512]
	str	w2, [x1, 1516]
	ldr	w2, [x27, 1076]
	str	w2, [x1, 1520]
	strb	wzr, [x1, 1436]
	strb	wzr, [x1, 1437]
	strb	wzr, [x1, 1438]
	bl	FSP_RecordSpecialFrame
	ldr	w0, [x28, 396]
	ldr	w1, [x27, 1252]
	add	w5, w0, 1
	uxtw	x0, w0
	cmp	w1, w24
	lsl	x1, x0, 2
	sub	x1, x1, x0
	lsl	x2, x1, 5
	add	x1, x1, x2
	add	x0, x1, x0
	add	x0, x25, x0
	add	x0, x0, 20480
	str	w22, [x0, 1532]
	str	w5, [x28, 396]
	bhi	.L190
.L191:
	ldr	w6, [x20, 1300]
	cbz	w6, .L189
	ldr	w2, [x28, 404]
	add	x4, x20, 560
	add	w6, w2, w6
	.p2align 2
.L194:
	uxtw	x0, w2
	ldr	w5, [x4], 12
	lsl	x1, x0, 2
	add	w2, w2, 1
	sub	x1, x1, x0
	cmp	w6, w2
	lsl	x3, x1, 5
	add	x1, x1, x3
	add	x0, x1, x0
	add	x0, x25, x0
	add	x0, x0, 20480
	str	w5, [x0, 1460]
	bne	.L194
	str	w6, [x28, 404]
.L189:
	add	x0, x25, 24576
	mov	w1, 1
	strb	w1, [x25, 12]
	mov	w3, 31
	add	x1, x20, 40
	ldr	w6, [x20, 40]
	str	wzr, [x0, 3748]
	mov	w5, 1
	ldrsb	w2, [x25, 10]
	cmp	w2, 31
	ldrb	w2, [x25, 10]
	str	w6, [x0, 3772]
	csel	w2, w2, w3, le
	ldr	w3, [x20, 1092]
	str	w3, [x0, 3788]
	sxtb	w2, w2
	uxtw	x2, w2
	lsl	x3, x2, 2
	sub	x2, x3, x2
	ldr	w3, [x20, 1108]
	add	x2, x1, x2, lsl 2
	ldr	w2, [x2, 8]
	str	w2, [x0, 3756]
	ldr	w2, [x20, 1100]
	str	w2, [x0, 3792]
	ldr	w2, [x20, 1104]
	str	w2, [x0, 3796]
	ldr	w2, [x20, 1288]
	ldr	w7, [x26, 8]
	str	w7, [x0, 3808]
	cmp	w2, 20
	ldr	w8, [x26, 12]
	str	w3, [x0, 3816]
	ldr	w3, [x20, 1112]
	str	w3, [x0, 3820]
	ldr	w3, [x20, 1116]
	strb	wzr, [x0, 3740]
	strb	wzr, [x0, 3741]
	strb	wzr, [x0, 3742]
	str	w8, [x0, 3812]
	str	w5, [x0, 3836]
	str	w3, [x0, 3824]
	mov	w0, 20
	csel	w2, w2, w0, le
	strb	w2, [x25, 13]
	ldr	w4, [x20, 1296]
	cbz	w4, .L193
	ldr	w1, [x28, 400]
	add	x2, x20, 1124
	add	w4, w1, w4
	.p2align 2
.L195:
	add	x0, x25, x1, uxtw 3
	ldr	w3, [x2], 8
	add	x0, x0, 28672
	add	w1, w1, 1
	cmp	w1, w4
	strb	wzr, [x0, 237]
	strb	w5, [x0, 236]
	str	w3, [x0, 232]
	bne	.L195
	str	w1, [x28, 400]
.L193:
	add	x25, x25, 20480
	ldr	w0, [x20, 1284]
	str	w0, [x28, 228]
	ldr	w2, [x20, 1092]
	ldr	w0, [x25, 1452]
	str	w2, [x25, 1384]
	ldr	w2, [x20, 1108]
	ldr	w1, [x20, 1100]
	str	w0, [x25, 1352]
	str	w2, [x25, 1412]
	ldr	w0, [x20, 1104]
	ldr	x2, [x29, 104]
	str	w1, [x25, 1388]
	str	w0, [x25, 1392]
	ldr	w1, [x20, 1112]
	ldr	w0, [x20, 1116]
	str	w1, [x25, 1416]
	str	w0, [x25, 1420]
	mov	w0, 1
	ldr	w1, [x20, 44]
	str	w0, [x25, 1432]
	mov	w0, 8
	strb	wzr, [x25, 1336]
	strb	wzr, [x25, 1337]
	strb	wzr, [x25, 1338]
	str	wzr, [x25, 1344]
	str	w6, [x25, 1368]
	str	w7, [x25, 1404]
	str	w8, [x25, 1408]
	str	w1, [x2]
	ldr	x1, [x23, 32]
	blr	x1
	b	.L200
	.p2align 3
.L219:
	ldr	x1, .LC81
	bl	dprint_vfmw
	ldr	x0, .LC82
	ldr	x4, [x0]
	cbz	x4, .L174
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	mov	w0, w19
	mov	w21, -1
	blr	x4
	b	.L200
	.p2align 3
.L221:
	ldr	w2, [x22, 1080]
	mov	w21, w0
	strb	w2, [x25, 9]
	mov	w1, 1
	ldr	x0, [x28, w19, sxtw 3]
	str	w1, [x0, 1592]
.L200:
	add	sp, x29, 0
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L220:
	ldr	w2, [x0, 2616]
	tst	w2, -3
	beq	.L175
	cmp	w2, 1
	beq	.L200
	strb	wzr, [x25, 9]
	mov	w0, 31
	ldr	x1, .LC85
	mov	w21, -1
	ldr	x2, [x28, w19, sxtw 3]
	add	x2, x2, 323584
	ldr	w2, [x2, 2616]
	bl	dprint_vfmw
	b	.L200
	.p2align 3
.L177:
	ldr	x2, .LC83
	str	wzr, [x0, 2656]
	ldr	x9, [x2, 128]
	ldrsb	w2, [x26, 3]
	str	w2, [x0, 2640]
	cbnz	x9, .L224
.L179:
	add	x1, x1, 327680
	mov	w2, 20
	mov	w0, -1
	ldr	w1, [x1, 2120]
	cmp	w1, w2
	csel	w1, w1, w2, le
	strb	w1, [x25, 13]
.L182:
	strb	wzr, [x25, 9]
	mov	w2, w0
	ldr	x1, .LC89
	mov	w0, 0
	ldr	x3, [x28, w19, sxtw 3]
	mov	w21, -1
	str	wzr, [x3, 1592]
	bl	dprint_vfmw
	b	.L200
	.p2align 3
.L223:
	ldr	x0, [x28, w19, sxtw 3]
	ldr	w0, [x0, 1552]
	cmp	w0, 1
	beq	.L185
	ldrsb	w1, [x26, 3]
	ldr	w2, [x22, 1080]
	add	w0, w1, 7
	cmp	w0, w2
	csel	w0, w0, w2, le
	strb	w0, [x25, 10]
	b	.L186
	.p2align 3
.L174:
	mov	w21, -1
	b	.L200
.L217:
	ldr	x3, .LC75
	mov	w2, 653
	ldr	x1, .LC76
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L200
.L218:
	ldr	x3, .LC78
	mov	w2, 654
	ldr	x1, .LC76
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	b	.L200
.L222:
	ldr	x1, .LC91
	mov	w3, w24
	mov	w2, 0
	mov	w0, 0
	mov	w21, -1
	bl	dprint_vfmw
	ldr	x1, [x23, 32]
	mov	w0, 8
	blr	x1
	b	.L200
	.size	FSP_ConfigFrameBuf, .-FSP_ConfigFrameBuf
	.align	3
.LC73:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC74:
	.xword	.LANCHOR1
	.align	3
.LC75:
	.xword	.LC7
	.align	3
.LC76:
	.xword	.LC9
	.align	3
.LC77:
	.xword	.LANCHOR0
	.align	3
.LC78:
	.xword	.LC24
	.align	3
.LC79:
	.xword	s_pstVfmwChan
	.align	3
.LC81:
	.xword	.LC80
	.align	3
.LC82:
	.xword	g_event_report
	.align	3
.LC83:
	.xword	g_vdm_hal_fun_ptr
	.align	3
.LC85:
	.xword	.LC84
	.align	3
.LC87:
	.xword	.LC86
	.align	3
.LC89:
	.xword	.LC88
	.align	3
.LC91:
	.xword	.LC90
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetDecodeFrameTable
	.type	FSP_GetDecodeFrameTable, %function
FSP_GetDecodeFrameTable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	mov	x21, x2
	bl	_mcount
	cmp	w20, 15
	bhi	.L236
	cbz	x19, .L237
	cbz	x21, .L238
	ldr	x0, .LC99
	ldr	x5, [x0, w20, sxtw 3]
	cbz	x5, .L239
	ldrsb	w0, [x5, 10]
	mov	w3, 0
	str	w0, [x21]
	ldrsb	w0, [x5, 10]
	cmp	w0, wzr
	ble	.L225
	.p2align 2
.L234:
	sxtw	x0, w3
	lsl	x2, x0, 2
	sub	x2, x2, x0
	lsl	x4, x2, 5
	add	x2, x2, x4
	add	x0, x2, x0
	add	x0, x5, x0
	add	x0, x0, 20480
	ldr	w0, [x0, 1452]
	str	w0, [x19, w3, sxtw 2]
	add	w3, w3, 1
	ldrsb	w0, [x5, 10]
	cmp	w0, w3
	bgt	.L234
.L225:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L236:
	ldr	x3, .LC93
	mov	w2, 880
	ldr	x1, .LC94
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L237:
	ldr	x3, .LC96
	mov	w2, 881
	ldr	x1, .LC94
	mov	w0, 0
	bl	dprint_vfmw
	b	.L225
.L238:
	ldr	x3, .LC98
	mov	w2, 882
	ldr	x1, .LC94
	mov	w0, 0
	bl	dprint_vfmw
	b	.L225
.L239:
	ldr	x3, .LC100
	mov	w2, 883
	ldr	x1, .LC94
	mov	w0, 0
	bl	dprint_vfmw
	b	.L225
	.size	FSP_GetDecodeFrameTable, .-FSP_GetDecodeFrameTable
	.align	3
.LC93:
	.xword	.LC7
	.align	3
.LC94:
	.xword	.LC9
	.align	3
.LC96:
	.xword	.LC95
	.align	3
.LC98:
	.xword	.LC97
	.align	3
.LC99:
	.xword	.LANCHOR0
	.align	3
.LC100:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetDecodePmvTable
	.type	FSP_GetDecodePmvTable, %function
FSP_GetDecodePmvTable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	mov	x21, x2
	bl	_mcount
	cmp	w20, 15
	bhi	.L251
	cbz	x19, .L252
	cbz	x21, .L253
	ldr	x0, .LC105
	ldr	x4, [x0, w20, sxtw 3]
	cbz	x4, .L254
	ldrsb	w0, [x4, 13]
	mov	w3, 0
	str	w0, [x21]
	ldrsb	w0, [x4, 13]
	cmp	w0, wzr
	ble	.L240
	.p2align 2
.L249:
	sxtw	x0, w3
	add	x0, x0, 3612
	add	x0, x4, x0, lsl 3
	ldr	w0, [x0, 8]
	str	w0, [x19, w3, sxtw 2]
	add	w3, w3, 1
	ldrsb	w0, [x4, 13]
	cmp	w0, w3
	bgt	.L249
.L240:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L251:
	ldr	x3, .LC101
	mov	w2, 906
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L252:
	ldr	x3, .LC103
	mov	w2, 907
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	b	.L240
.L253:
	ldr	x3, .LC104
	mov	w2, 908
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	b	.L240
.L254:
	ldr	x3, .LC106
	mov	w2, 909
	ldr	x1, .LC102
	mov	w0, 0
	bl	dprint_vfmw
	b	.L240
	.size	FSP_GetDecodePmvTable, .-FSP_GetDecodePmvTable
	.align	3
.LC101:
	.xword	.LC7
	.align	3
.LC102:
	.xword	.LC9
	.align	3
.LC103:
	.xword	.LC95
	.align	3
.LC104:
	.xword	.LC97
	.align	3
.LC105:
	.xword	.LANCHOR0
	.align	3
.LC106:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_IsFsLeak
	.type	FSP_IsFsLeak, %function
FSP_IsFsLeak:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC107
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L256
	mov	w0, 0
.L257:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L256:
	ldr	x1, .LC109
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L257
	.size	FSP_IsFsLeak, .-FSP_IsFsLeak
	.align	3
.LC107:
	.xword	.LANCHOR0
	.align	3
.LC109:
	.xword	.LC108
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_IsFsStatusReady
	.type	FSP_IsFsStatusReady, %function
FSP_IsFsStatusReady:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	w20, w0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 96]
	cmp	w0, 1
	beq	.L262
	mov	w0, 0
.L259:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L262:
	ldr	w1, [x19]
	mov	w0, 0
	tst	w1, 16711935
	bne	.L259
	mov	x1, x19
	mov	w0, w20
	bl	FSP_IsFsStatusReady.part.5
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_IsFsStatusReady, .-FSP_IsFsStatusReady
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FindLeastTickPhyFs
	.type	FindLeastTickPhyFs, %function
FindLeastTickPhyFs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w20, wzr
	ble	.L267
	mov	w4, -1
	mov	w0, 0
	mov	w2, 0
	.p2align 2
.L266:
	ldr	w3, [x19, 88]
	add	x19, x19, 96
	cmp	w3, w4
	bcs	.L265
	mov	w0, w2
	mov	w4, w3
.L265:
	add	w2, w2, 1
	cmp	w20, w2
	bne	.L266
.L264:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L267:
	mov	w0, 0
	b	.L264
	.size	FindLeastTickPhyFs, .-FindLeastTickPhyFs
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_FindEmptyLogicFs
	.type	FSP_FindEmptyLogicFs, %function
FSP_FindEmptyLogicFs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 56
	mov	w1, 0
	.p2align 2
.L272:
	ldrsb	w2, [x0]
	cbnz	w2, .L270
	ldrsb	w2, [x0, 1]
	cbz	w2, .L274
.L270:
	add	w1, w1, 1
	add	x0, x0, 680
	cmp	w1, 32
	bne	.L272
	mov	x0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L274:
	sxtw	x0, w1
	lsl	x2, x0, 5
	add	x0, x2, x0, lsl 3
	add	x0, x0, x0, lsl 4
	add	x2, x19, x0
	add	x0, x2, 56
	strb	w1, [x2, 59]
	strb	wzr, [x2, 58]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_FindEmptyLogicFs, .-FSP_FindEmptyLogicFs
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_FindEmptyDecodeFs
	.type	FSP_FindEmptyDecodeFs, %function
FSP_FindEmptyDecodeFs:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w22, w0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	ldr	x1, .LC110
	mov	w0, w22
	ldr	x24, [x1, w22, sxtw 3]
	bl	VCTRL_GetVidStd
	cmp	w0, 17
	beq	.L276
	ldrsb	w23, [x24, 10]
	cmp	w23, wzr
	ble	.L275
	add	x19, x24, 20480
	mov	w20, 0
	add	x19, x19, 1436
	b	.L279
	.p2align 3
.L278:
	add	w20, w20, 1
	add	x19, x19, 100
	cmp	w20, w23
	beq	.L275
.L279:
	ldr	w0, [x19, 96]
	cmp	w0, 1
	bne	.L278
	sxtw	x0, w20
	lsl	x1, x0, 2
	sub	x1, x1, x0
	lsl	x2, x1, 5
	add	x1, x1, x2
	add	x0, x1, x0
	add	x0, x24, x0
	add	x0, x0, 20480
	ldr	w0, [x0, 1436]
	tst	w0, 16711935
	bne	.L278
	mov	x1, x19
	mov	w0, w22
	bl	FSP_IsFsStatusReady.part.5
	cbz	w0, .L278
	str	x19, [x21, 640]
.L275:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L276:
	ldr	x2, .LC111
	add	x0, x24, 20480
	add	x0, x0, 1532
	mov	w1, 0
	ldr	x2, [x2, w22, sxtw 3]
	add	x2, x2, 622592
	ldr	w3, [x2, 1912]
	b	.L281
	.p2align 3
.L280:
	add	w1, w1, 1
	add	x0, x0, 100
	cmp	w1, 32
	beq	.L275
.L281:
	ldr	w2, [x0]
	cmp	w2, 1
	bne	.L280
	ldrsb	w2, [x0, -96]
	cbnz	w2, .L280
	ldrsb	w2, [x0, -94]
	cbnz	w2, .L280
	ldr	w2, [x0, -76]
	cmp	w3, w2
	bne	.L280
	sxtw	x1, w1
	lsl	x0, x1, 2
	sub	x0, x0, x1
	lsl	x2, x0, 5
	add	x0, x0, x2
	add	x0, x0, x1
	add	x0, x0, 20480
	add	x0, x24, x0
	add	x0, x0, 1436
	str	x0, [x21, 640]
	b	.L275
	.size	FSP_FindEmptyDecodeFs, .-FSP_FindEmptyDecodeFs
	.align	3
.LC110:
	.xword	.LANCHOR0
	.align	3
.LC111:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_NewLogicFs
	.type	FSP_NewLogicFs, %function
FSP_NewLogicFs:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w24, w0
	mov	x0, x30
	mov	w21, w1
	bl	_mcount
	cmp	w24, 15
	bhi	.L320
	ldr	x19, .LC114
	ldr	x0, [x19, w24, sxtw 3]
	cbz	x0, .L321
	ldr	x23, .LC116
	mov	w0, 3
	ldr	x1, [x23, 24]
	blr	x1
	ldr	x22, [x19, w24, sxtw 3]
	mov	w1, 0
	add	x0, x22, 56
	.p2align 2
.L293:
	ldrsb	w2, [x0]
	cbnz	w2, .L291
	ldrsb	w2, [x0, 1]
	cbz	w2, .L322
.L291:
	add	w1, w1, 1
	add	x0, x0, 680
	cmp	w1, 32
	bne	.L293
.L294:
	ldr	x1, .LC118
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, [x23, 32]
	mov	w0, 3
	blr	x1
	mov	w0, -1
.L319:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L322:
	sxtw	x25, w1
	lsl	x19, x25, 5
	add	x19, x19, x25, lsl 3
	add	x19, x19, x19, lsl 4
	add	x20, x22, x19
	adds	x19, x20, 56
	strb	w1, [x20, 59]
	strb	wzr, [x20, 58]
	beq	.L294
	str	xzr, [x20, 696]
	str	xzr, [x20, 704]
	str	xzr, [x20, 712]
	str	wzr, [x20, 720]
	cbnz	w21, .L295
	add	x0, x22, 20480
	add	x1, x22, 28672
	add	x0, x0, 1336
	str	x0, [x20, 696]
	str	x0, [x20, 704]
	mov	w2, 1
	str	x0, [x20, 712]
	ldr	w0, [x1, 232]
	str	w0, [x20, 720]
	ldrb	w0, [x22, 13]
	strb	w2, [x20, 58]
	sub	w0, w0, #1
	strb	w0, [x20, 60]
.L296:
	lsl	x0, x25, 5
	mov	w1, 1
	add	x0, x0, x25, lsl 3
	mov	w2, 1
	add	x19, x19, 8
	add	x0, x0, x0, lsl 4
	add	x21, x22, x0
	add	x0, x21, 64
	ldr	x3, [x21, 704]
	strb	w1, [x21, 57]
	strb	w1, [x3, 2]
	str	w2, [x21, 692]
	ldr	x2, [x21, 696]
	ldr	w1, [x22, 16]
	str	w1, [x21, 688]
	str	w1, [x2, 88]
	ldr	x1, [x21, 704]
	ldr	w2, [x21, 688]
	str	w2, [x1, 88]
	mov	x2, 624
	ldr	w1, [x22, 16]
	add	w1, w1, 1
	str	w1, [x22, 16]
	mov	w1, 0
	ldr	x3, [x23, 96]
	ldr	x20, [x21, 696]
	ldr	x22, [x21, 704]
	ldr	x24, [x21, 712]
	blr	x3
	ldrsb	w0, [x21, 59]
	str	w0, [x19, 156]
	ldr	w0, [x20, 68]
	str	w0, [x19, 112]
	ldr	w0, [x20, 72]
	str	w0, [x19, 116]
	ldr	w0, [x22, 32]
	str	w0, [x19, 140]
	ldr	w0, [x22, 60]
	str	w0, [x19, 144]
	ldr	w0, [x22, 64]
	str	w0, [x19, 148]
	ldr	w1, [x20, 16]
	str	w1, [x19, 44]
	mov	w0, w1
	ldr	w2, [x20, 76]
	add	w1, w2, w1
	str	w1, [x19, 48]
	bl	MEM_Phy2Vir
	str	x0, [x19, 504]
	ldr	w0, [x19, 48]
	bl	MEM_Phy2Vir
	str	x0, [x19, 512]
	ldr	w0, [x20, 24]
	str	w0, [x19, 228]
	bl	MEM_Phy2Vir
	str	x0, [x19, 232]
	ldr	w1, [x22, 16]
	str	w1, [x19, 28]
	ldr	w0, [x22, 76]
	add	w0, w1, w0
	str	w0, [x19, 32]
	ldr	w2, [x22, 32]
	add	w1, w2, w1
	str	w1, [x19, 36]
	ldr	w1, [x22, 32]
	add	w0, w1, w0
	str	w0, [x19, 40]
	ldr	w1, [x22, 16]
	str	w1, [x19, 60]
	mov	w0, w1
	ldr	w2, [x22, 76]
	add	w1, w2, w1
	str	w1, [x19, 64]
	bl	MEM_Phy2Vir
	str	x0, [x19, 520]
	ldr	w0, [x19, 64]
	bl	MEM_Phy2Vir
	str	x0, [x19, 528]
	ldr	w1, [x19, 44]
	ldr	w2, [x24, 16]
	str	w2, [x19, 52]
	ldr	x0, [x19, 504]
	ldr	w3, [x20, 76]
	add	w2, w3, w2
	str	w2, [x19, 56]
	ldr	w2, [x20, 48]
	str	w2, [x19, 152]
	ldr	w2, [x20, 52]
	add	w2, w2, w1
	str	w2, [x19, 68]
	ldp	w3, w2, [x20, 52]
	add	w2, w3, w2
	add	w1, w2, w1
	str	w1, [x19, 72]
	ldrsw	x1, [x20, 52]
	add	x1, x1, x0
	str	x1, [x19, 544]
	ldpsw	x2, x1, [x20, 52]
	add	x1, x2, x1
	add	x0, x1, x0
	str	x0, [x19, 552]
	ldr	w0, [x20, 28]
	str	w0, [x19, 220]
	ldr	w0, [x20, 28]
	bl	MEM_Phy2Vir
	str	x0, [x19, 536]
	ldr	w0, [x22, 40]
	str	w0, [x19, 88]
	ldr	w0, [x22, 44]
	str	w0, [x19, 92]
	ldr	w0, [x20, 36]
	str	w0, [x19, 76]
	ldr	w0, [x20, 36]
	bl	MEM_Phy2Vir
	str	x0, [x19, 496]
	ldr	x1, [x23, 32]
	mov	w0, 3
	stp	wzr, wzr, [x19, 80]
	blr	x1
	ldrsb	w0, [x21, 59]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L295:
	mov	w0, w24
	mov	x1, x19
	bl	FSP_FindEmptyDecodeFs
	ldr	x0, [x20, 696]
	cbz	x0, .L323
	add	x0, x22, 24576
	add	x0, x0, 3740
	str	x0, [x20, 712]
	ldrsb	w0, [x22, 13]
	cmp	w0, wzr
	ble	.L299
	mov	w0, w24
	bl	VCTRL_GetVidStd
	cmp	w0, 17
	beq	.L300
	ldrsb	w3, [x22, 13]
	add	x4, x22, 28672
	mov	w1, 0
	add	x0, x4, 236
	cmp	w3, wzr
	bgt	.L306
	b	.L302
	.p2align 3
.L303:
	add	w1, w1, 1
	add	x0, x0, 8
	cmp	w1, w3
	beq	.L302
.L306:
	ldrb	w2, [x0]
	cmp	w2, 1
	bne	.L303
	ldrb	w2, [x0, 1]
	cbnz	w2, .L303
	lsl	x0, x25, 5
	sxtw	x2, w1
	add	x0, x0, x25, lsl 3
	add	x2, x2, 3612
	cmp	w3, w1
	add	x0, x0, x0, lsl 4
	add	x2, x22, x2, lsl 3
	add	x0, x22, x0
	strb	w1, [x0, 60]
	ldr	w1, [x2, 8]
	str	w1, [x0, 720]
	ldr	w5, [x4, 228]
	str	w5, [x0, 724]
	ble	.L304
	.p2align 2
.L299:
	lsl	x0, x25, 5
	ldr	x21, .LC124
	add	x0, x0, x25, lsl 3
	add	x0, x0, x0, lsl 4
	ldr	w1, [x21]
	add	x20, x22, x0
	ldr	x0, [x20, 696]
	str	x0, [x20, 704]
	cbz	w1, .L296
	mov	w0, w24
	bl	VCTRL_GetImageWidth
	mov	w26, w0
	mov	w0, w24
	bl	VCTRL_GetImageHeight
	ldr	x4, [x21, 16]
	cbz	x4, .L324
	mov	w3, w0
	mov	w2, w26
	add	x1, x29, 88
	mov	w0, w24
	blr	x4
	cbnz	w0, .L310
	ldr	w0, [x29, 96]
	cbz	w0, .L325
	ldr	x1, [x20, 696]
	str	w0, [x1, 16]
	ldrsb	x0, [x20, 60]
	ldr	w1, [x29, 120]
	add	x0, x0, 3612
	str	w1, [x20, 720]
	add	x0, x22, x0, lsl 3
	str	w1, [x0, 8]
	b	.L296
.L302:
	cmp	w3, wzr
	ldr	w5, [x4, 228]
	ble	.L308
.L304:
	sub	w3, w3, #1
	mov	x0, 28917
	add	x1, x4, 237
	add	x0, x0, x3, uxtw 3
	add	x0, x22, x0
	.p2align 2
.L307:
	strb	wzr, [x1], 8
	cmp	x1, x0
	bne	.L307
.L308:
	lsl	x2, x25, 5
	ldr	x1, .LC123
	add	x2, x2, x25, lsl 3
	mov	w0, 0
	add	x2, x2, x2, lsl 4
	add	x2, x22, x2
	strb	wzr, [x2, 60]
	ldr	w3, [x4, 232]
	str	w3, [x2, 720]
	str	w5, [x2, 724]
	bl	dprint_vfmw
	b	.L299
.L300:
	add	x0, x22, 28672
	strb	wzr, [x20, 60]
	ldr	w1, [x0, 232]
	str	w1, [x20, 720]
	ldr	w0, [x0, 228]
	str	w0, [x20, 724]
	b	.L299
.L320:
	ldr	x3, .LC112
	mov	w2, 1279
	ldr	x1, .LC113
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L319
.L321:
	ldr	x3, .LC115
	mov	w2, 1280
	ldr	x1, .LC113
	bl	dprint_vfmw
	mov	w0, -1
	b	.L319
.L325:
	ldr	x1, [x23, 32]
	mov	w0, 3
	blr	x1
	ldr	x1, .LC129
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L319
.L324:
	ldr	x1, .LC125
	mov	w0, 0
	bl	dprint_vfmw
.L310:
	ldr	x1, [x23, 32]
	mov	w0, 3
	blr	x1
	ldr	x1, .LC127
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L319
.L323:
	ldr	x2, .LC119
	ldr	x1, .LC121
	bl	dprint_vfmw
	ldr	x1, [x23, 32]
	mov	w0, 3
	blr	x1
	mov	w0, -1
	b	.L319
	.size	FSP_NewLogicFs, .-FSP_NewLogicFs
	.align	3
.LC112:
	.xword	.LC7
	.align	3
.LC113:
	.xword	.LC9
	.align	3
.LC114:
	.xword	.LANCHOR0
	.align	3
.LC115:
	.xword	.LC24
	.align	3
.LC116:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC118:
	.xword	.LC117
	.align	3
.LC119:
	.xword	.LANCHOR2
	.align	3
.LC121:
	.xword	.LC120
	.align	3
.LC123:
	.xword	.LC122
	.align	3
.LC124:
	.xword	.LANCHOR1
	.align	3
.LC125:
	.xword	.LC42
	.align	3
.LC127:
	.xword	.LC126
	.align	3
.LC129:
	.xword	.LC128
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetPhyFsNum
	.type	FSP_GetPhyFsNum, %function
FSP_GetPhyFsNum:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w22, w0
	mov	x0, x30
	mov	w20, w1
	mov	w19, 0
	bl	_mcount
	ldr	x21, .LC130
	mov	w0, 3
	ldr	x1, [x21, 24]
	blr	x1
	ldr	x0, .LC131
	ldr	x2, [x0, w22, sxtw 3]
	add	x0, x2, 20480
	add	x2, x2, 24576
	add	x0, x0, 1532
	add	x3, x2, 636
	b	.L328
	.p2align 3
.L327:
	add	x0, x0, 100
	cmp	x0, x3
	beq	.L332
.L328:
	ldr	w2, [x0]
	cmp	w2, 1
	bne	.L327
	ldr	w1, [x0, -76]
	add	x0, x0, 100
	cmp	w1, w20
	ccmp	w20, wzr, 4, ne
	csinc	w19, w19, w19, ne
	cmp	x0, x3
	bne	.L328
.L332:
	ldr	x1, [x21, 32]
	mov	w0, 3
	blr	x1
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	FSP_GetPhyFsNum, .-FSP_GetPhyFsNum
	.align	3
.LC130:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC131:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_RelsePhyFs
	.type	FSP_RelsePhyFs, %function
FSP_RelsePhyFs:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w21, w0
	mov	x0, x30
	mov	w22, w1
	mov	w19, 0
	bl	_mcount
	ldr	x20, .LC132
	mov	w0, 3
	ldr	x1, [x20, 24]
	blr	x1
	ldr	x0, .LC133
	ldr	x24, [x0, w21, sxtw 3]
	add	x0, x24, 20480
	add	x0, x0, 1532
	b	.L337
	.p2align 3
.L334:
	add	w19, w19, 1
	add	x0, x0, 100
	cmp	w19, 32
	beq	.L357
.L337:
	ldr	w2, [x0]
	cmp	w2, 1
	bne	.L334
	ldrsb	w2, [x0, -96]
	cbnz	w2, .L334
	ldrsb	w2, [x0, -94]
	cbnz	w2, .L334
	ldr	w2, [x0, -76]
	cmp	w2, w22
	beq	.L334
	sxtw	x0, w19
	lsl	x1, x0, 2
	sub	x1, x1, x0
	lsl	x2, x1, 5
	add	x1, x1, x2
	add	x0, x1, x0
	add	x24, x24, x0
	add	x24, x24, 20480
	ldr	w23, [x24, 1452]
	mov	w0, w23
	bl	MEM_Phy2Vir
	mov	x22, x0
	cmp	w19, 31
	ldr	w2, [x24, 1456]
	bgt	.L357
	mov	x1, x0
	str	wzr, [x24, 1532]
	mov	w0, w23
	bl	MEM_DelMemRecord
	ldr	x1, [x20, 32]
	mov	w0, 3
	blr	x1
	ldr	x1, [x20, 24]
	mov	w0, 8
	blr	x1
	ldr	x0, .LC134
	mov	w1, 0
	ldr	x3, [x0, w21, sxtw 3]
	add	x4, x3, 327680
	add	x0, x4, 888
	b	.L339
	.p2align 3
.L341:
	add	w1, w1, 1
	add	x0, x0, 12
	cmp	w1, 32
	beq	.L343
.L339:
	ldrb	w2, [x0]
	cmp	w2, 1
	bne	.L341
	ldr	w2, [x0, -8]
	cmp	w23, w2
	bne	.L341
	cmp	w1, 31
	bgt	.L343
	sxtw	x1, w1
	lsl	x0, x1, 2
	sub	x1, x0, x1
	add	x0, x4, x1, lsl 2
	strb	wzr, [x0, 888]
.L343:
	add	x0, x3, 323584
	mov	w19, 0
	add	x0, x0, 2664
	b	.L346
	.p2align 3
.L344:
	add	w19, w19, 1
	add	x0, x0, 72
	cmp	w19, 32
	beq	.L347
.L346:
	ldr	w1, [x0]
	cmp	w1, 255
	beq	.L344
	ldr	w1, [x0, 32]
	cmp	w23, w1
	bne	.L344
	cmp	w19, 31
	bgt	.L347
	sxtw	x0, w19
	ldr	x2, .LC135
	lsl	x4, x0, 6
	mov	w1, 255
	add	x0, x4, x0, lsl 3
	add	x0, x3, x0
	ldr	x4, [x2]
	add	x0, x0, 323584
	str	w1, [x0, 2664]
	cbz	x4, .L347
	uxtw	x23, w23
	mov	w3, 16
	add	x2, x29, 64
	mov	w1, 123
	mov	w0, w21
	stp	x23, x22, [x29, 64]
	blr	x4
.L347:
	ldr	x1, [x20, 32]
	mov	w0, 8
	blr	x1
	cmp	w19, 31
	ldp	x21, x22, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x23, x24, [sp, 48]
	csetm	w0, gt
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L357:
	ldr	x1, [x20, 32]
	mov	w0, 3
	blr	x1
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.size	FSP_RelsePhyFs, .-FSP_RelsePhyFs
	.align	3
.LC132:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC133:
	.xword	.LANCHOR0
	.align	3
.LC134:
	.xword	s_pstVfmwChan
	.align	3
.LC135:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_RemovePmv
	.type	FSP_RemovePmv, %function
FSP_RemovePmv:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w20, 15
	bhi	.L366
	cmp	w19, 31
	bhi	.L367
	ldr	x0, .LC140
	ldr	x2, [x0, w20, sxtw 3]
	cbz	x2, .L368
	lsl	x0, x19, 5
	add	x0, x0, x19, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x2, x0
	ldr	w1, [x0, 692]
	cmp	w1, 2
	beq	.L369
.L360:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L369:
	ldrsb	w0, [x0, 60]
	uxtb	w1, w0
	cmp	w1, 19
	bhi	.L365
	sxtw	x0, w0
	add	x0, x0, 3612
	add	x0, x2, x0, lsl 3
	strb	wzr, [x0, 13]
.L365:
	lsl	x1, x19, 5
	add	x19, x1, x19, lsl 3
	add	x19, x19, x19, lsl 4
	add	x19, x2, x19
	str	wzr, [x19, 692]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L366:
	ldr	x3, .LC136
	mov	w2, 1595
	ldr	x1, .LC137
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L367:
	ldr	x3, .LC139
	mov	w2, 1596
	ldr	x1, .LC137
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L368:
	ldr	x3, .LC141
	mov	w2, 1597
	ldr	x1, .LC137
	mov	w0, 0
	bl	dprint_vfmw
	b	.L360
	.size	FSP_RemovePmv, .-FSP_RemovePmv
	.align	3
.LC136:
	.xword	.LC7
	.align	3
.LC137:
	.xword	.LC9
	.align	3
.LC139:
	.xword	.LC138
	.align	3
.LC140:
	.xword	.LANCHOR0
	.align	3
.LC141:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_SetRef
	.type	FSP_SetRef, %function
FSP_SetRef:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w22, w0
	mov	x0, x30
	sxtw	x20, w1
	mov	w21, w2
	bl	_mcount
	cmp	w22, 15
	bhi	.L404
	cmp	w20, 31
	bhi	.L405
	ldr	x0, .LC145
	ldr	x23, [x0, w22, sxtw 3]
	cbz	x23, .L406
	lsl	x19, x20, 5
	sxtb	w24, w21
	add	x19, x19, x20, lsl 3
	add	x19, x19, x19, lsl 4
	add	x19, x23, x19
	ldrsb	w0, [x19, 58]
	strb	w24, [x19, 56]
	cbnz	w0, .L370
	ldr	x0, [x19, 696]
	cbz	x0, .L376
	ldr	w2, [x0, 88]
	ldr	w1, [x19, 688]
	cmp	w2, w1
	beq	.L407
.L376:
	lsl	x0, x20, 5
	add	x0, x0, x20, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x23, x0
	ldrsb	w3, [x0, 60]
	uxtb	w1, w3
	cmp	w1, 19
	bhi	.L379
	cmp	w21, 1
	beq	.L408
	cbnz	w21, .L382
	ldr	w1, [x0, 692]
	cmp	w1, 2
	beq	.L409
.L383:
	lsl	x0, x20, 5
	add	x0, x0, x20, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x23, x0
	ldr	x0, [x0, 704]
	cbz	x0, .L393
	ldrsb	w1, [x0, 2]
	cmp	w1, 1
	beq	.L410
.L393:
	lsl	x1, x20, 5
	add	x20, x1, x20, lsl 3
	add	x20, x20, x20, lsl 4
	add	x23, x23, x20
	str	xzr, [x23, 696]
.L370:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L404:
	ldr	x3, .LC142
	mov	w2, 1625
	ldr	x1, .LC143
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L408:
	ldr	w1, [x0, 692]
	cmp	w1, 1
	beq	.L411
.L381:
	lsl	x0, x20, 5
	add	x0, x0, x20, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x23, x0
	ldr	x1, [x0, 704]
	cbz	x1, .L370
.L388:
	lsl	x0, x20, 5
	ldr	w3, [x1, 88]
	add	x0, x0, x20, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x23, x0
	ldr	w2, [x0, 688]
	cmp	w3, w2
	ccmp	w21, 1, 0, eq
	beq	.L412
.L384:
	cbz	w21, .L393
.L403:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L405:
	ldr	x3, .LC144
	mov	w2, 1626
	ldr	x1, .LC143
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L379:
	ldr	x1, .LC150
	mov	w2, 1678
	mov	w0, 0
	bl	dprint_vfmw
	cbz	w21, .L383
	b	.L381
	.p2align 3
.L411:
	sxtw	x1, w3
	mov	w2, 2
	add	x1, x1, 3612
	add	x1, x23, x1, lsl 3
	strb	w21, [x1, 13]
	str	w2, [x0, 692]
.L382:
	lsl	x0, x20, 5
	add	x0, x0, x20, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x23, x0
	ldr	x1, [x0, 704]
	cbnz	x1, .L388
	cbz	w21, .L393
	b	.L403
	.p2align 3
.L407:
	ldrsb	w1, [x0]
	cmp	w21, w1
	beq	.L377
	ldr	x1, .LC147
	ldr	w2, [x1]
	cbnz	w2, .L413
.L377:
	strb	w24, [x0]
	b	.L376
	.p2align 3
.L412:
	ldrsb	w2, [x1, 2]
	cmp	w2, 1
	bne	.L384
	mov	w2, 2
	strb	w2, [x1, 2]
	strb	w2, [x0, 57]
	cbz	w21, .L393
	b	.L403
	.p2align 3
.L413:
	ldr	x3, [x1, 32]
	ldr	w1, [x0, 16]
	cbz	x3, .L414
	mov	w2, w21
	mov	w0, w22
	blr	x3
	ldr	x0, [x19, 696]
	strb	w24, [x0]
	b	.L376
	.p2align 3
.L410:
	strb	wzr, [x0, 2]
	b	.L393
	.p2align 3
.L409:
	sxtw	x1, w3
	add	x1, x1, 3612
	add	x1, x23, x1, lsl 3
	strb	wzr, [x1, 13]
	str	wzr, [x0, 692]
	str	wzr, [x0, 720]
	b	.L383
	.p2align 3
.L406:
	ldr	x3, .LC146
	mov	w2, 1627
	ldr	x1, .LC143
	mov	w0, 0
	bl	dprint_vfmw
	b	.L370
.L414:
	ldr	x1, .LC148
	mov	w0, 0
	bl	dprint_vfmw
	mov	x0, 680
	madd	x0, x20, x0, x23
	ldr	x0, [x0, 696]
	strb	w24, [x0]
	b	.L376
	.size	FSP_SetRef, .-FSP_SetRef
	.align	3
.LC142:
	.xword	.LC7
	.align	3
.LC143:
	.xword	.LC9
	.align	3
.LC144:
	.xword	.LC138
	.align	3
.LC145:
	.xword	.LANCHOR0
	.align	3
.LC146:
	.xword	.LC24
	.align	3
.LC147:
	.xword	.LANCHOR1
	.align	3
.LC148:
	.xword	.LC42
	.align	3
.LC150:
	.xword	.LC149
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetRef
	.type	FSP_GetRef, %function
FSP_GetRef:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w20, 15
	bhi	.L420
	cmp	w19, 31
	bhi	.L421
	ldr	x0, .LC156
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L422
	lsl	x1, x19, 5
	add	x19, x1, x19, lsl 3
	add	x19, x19, x19, lsl 4
	add	x19, x0, x19
	ldrsb	w0, [x19, 56]
.L417:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L420:
	ldr	x3, .LC152
	mov	w2, 1717
	ldr	x1, .LC153
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L417
	.p2align 3
.L421:
	ldr	x3, .LC155
	mov	w2, 1718
	ldr	x1, .LC153
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L417
.L422:
	ldr	x3, .LC158
	mov	w2, 1719
	ldr	x1, .LC153
	bl	dprint_vfmw
	mov	w0, -1
	b	.L417
	.size	FSP_GetRef, .-FSP_GetRef
	.align	3
.LC152:
	.xword	.LC151
	.align	3
.LC153:
	.xword	.LC9
	.align	3
.LC155:
	.xword	.LC154
	.align	3
.LC156:
	.xword	.LANCHOR0
	.align	3
.LC158:
	.xword	.LC157
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_SetDisplay
	.type	FSP_SetDisplay, %function
FSP_SetDisplay:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	w22, w0
	mov	x0, x30
	sxtw	x20, w1
	mov	w21, w2
	bl	_mcount
	cmp	w22, 15
	bhi	.L464
	cmp	w20, 31
	bhi	.L465
	ldr	x19, .LC162
	ldr	x0, [x19, w22, sxtw 3]
	cbz	x0, .L466
	ldr	x24, .LC164
	mov	w0, 3
	ldr	x1, [x24, 24]
	blr	x1
	ldr	x23, [x19, w22, sxtw 3]
	lsl	x0, x20, 5
	add	x0, x0, x20, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x23, x0
	ldrsb	w1, [x0, 58]
	cmp	w1, wzr
	ccmp	w21, wzr, 0, eq
	bne	.L428
	ldr	x1, [x0, 704]
	cbz	x1, .L428
	ldrsb	w0, [x0, 57]
	cbz	w0, .L428
	ldr	x0, .LC165
	ldr	w2, [x0]
	cbnz	w2, .L467
	.p2align 2
.L428:
	lsl	x19, x20, 5
	mov	w25, 3
	add	x19, x19, x20, lsl 3
	cmp	w21, wzr
	csel	w1, wzr, w25, eq
	add	x19, x19, x19, lsl 4
	add	x19, x23, x19
	ldrsb	w0, [x19, 58]
	strb	w1, [x19, 57]
	cbnz	w0, .L438
	ldr	x1, [x19, 704]
	cbz	x1, .L438
	ldr	w3, [x1, 88]
	ldr	w2, [x19, 688]
	cmp	w3, w2
	bne	.L468
	cbnz	w21, .L434
	strb	wzr, [x19, 57]
	mov	w0, w22
	strb	wzr, [x1, 2]
	bl	VCTRL_GetVidStd
	cmp	w0, 17
	beq	.L469
.L436:
	lsl	x0, x20, 5
	add	x20, x0, x20, lsl 3
	add	x20, x20, x20, lsl 4
	add	x20, x23, x20
	str	xzr, [x20, 704]
.L438:
	ldr	x1, [x24, 32]
	mov	w0, 3
	blr	x1
.L423:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L464:
	ldr	x3, .LC159
	mov	w2, 1736
	ldr	x1, .LC160
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L434:
	mov	w0, w22
	bl	VCTRL_GetVidStd
	cmp	w0, 17
	beq	.L470
.L437:
	lsl	x0, x20, 5
	mov	w1, 3
	add	x20, x0, x20, lsl 3
	add	x20, x20, x20, lsl 4
	add	x20, x23, x20
	ldr	x0, [x20, 704]
	strb	w1, [x20, 57]
	strb	w1, [x0, 2]
	b	.L438
	.p2align 3
.L465:
	ldr	x3, .LC161
	mov	w2, 1737
	ldr	x1, .LC160
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L468:
	ldr	x1, .LC168
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, [x24, 32]
	mov	w0, w25
	blr	x1
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L466:
	ldr	x3, .LC163
	mov	w2, 1738
	ldr	x1, .LC160
	bl	dprint_vfmw
	b	.L423
	.p2align 3
.L467:
	ldr	x2, [x0, 24]
	ldr	w1, [x1, 16]
	cbz	x2, .L471
	mov	w0, w22
	blr	x2
	b	.L428
	.p2align 3
.L469:
	ldr	x0, [x19, 704]
	cbz	x0, .L436
	ldr	w1, [x0, 4]
	cbz	w1, .L436
	sub	w1, w1, #1
	str	w1, [x0, 4]
	ldr	x0, [x19, 704]
	ldr	w1, [x0, 4]
	cbz	w1, .L436
	strb	w25, [x0, 2]
	b	.L436
	.p2align 3
.L470:
	ldr	x0, [x19, 704]
	cbz	x0, .L437
	ldr	w1, [x0, 4]
	add	w1, w1, 1
	str	w1, [x0, 4]
	b	.L437
.L471:
	ldr	x1, .LC166
	mov	w0, 0
	bl	dprint_vfmw
	b	.L428
	.size	FSP_SetDisplay, .-FSP_SetDisplay
	.align	3
.LC159:
	.xword	.LC7
	.align	3
.LC160:
	.xword	.LC9
	.align	3
.LC161:
	.xword	.LC138
	.align	3
.LC162:
	.xword	.LANCHOR0
	.align	3
.LC163:
	.xword	.LC24
	.align	3
.LC164:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC165:
	.xword	.LANCHOR1
	.align	3
.LC166:
	.xword	.LC44
	.align	3
.LC168:
	.xword	.LC167
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ClearNotInVoQueue
	.type	FSP_ClearNotInVoQueue, %function
FSP_ClearNotInVoQueue:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	cmp	w20, 15
	bhi	.L483
	cbz	x22, .L484
	ldr	x0, .LC173
	ldr	x19, [x0, w20, sxtw 3]
	cbz	x19, .L485
	add	x21, x19, 20480
	add	x19, x19, 64
	add	x21, x21, 1344
	b	.L478
	.p2align 3
.L477:
	add	x19, x19, 680
	cmp	x19, x21
	beq	.L486
.L478:
	ldr	w1, [x19, 156]
	mov	w0, w20
	mov	w2, 0
	bl	FSP_SetRef
	mov	x1, x19
	mov	x0, x22
	bl	IsImageInVoQueue
	cbz	w0, .L477
	ldr	w1, [x19, 156]
	mov	w2, 0
	mov	w0, w20
	add	x19, x19, 680
	bl	FSP_SetDisplay
	cmp	x19, x21
	bne	.L478
.L486:
	mov	w0, 0
.L474:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L483:
	ldr	x3, .LC169
	mov	w2, 508
	ldr	x1, .LC170
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L474
.L485:
	ldr	x3, .LC174
	mov	w2, 510
	ldr	x1, .LC170
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L474
.L484:
	ldr	x3, .LC172
	mov	w2, 509
	ldr	x1, .LC170
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L474
	.size	FSP_ClearNotInVoQueue, .-FSP_ClearNotInVoQueue
	.align	3
.LC169:
	.xword	.LC7
	.align	3
.LC170:
	.xword	.LC9
	.align	3
.LC172:
	.xword	.LC171
	.align	3
.LC173:
	.xword	.LANCHOR0
	.align	3
.LC174:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetDisplay
	.type	FSP_GetDisplay, %function
FSP_GetDisplay:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x0, .LC175
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L490
	sxtw	x1, w19
	lsl	x2, x1, 5
	add	x1, x2, x1, lsl 3
	add	x1, x1, x1, lsl 4
	add	x1, x0, x1
	ldrsb	w0, [x1, 57]
.L489:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L490:
	ldr	x3, .LC176
	mov	w2, 1829
	ldr	x1, .LC177
	bl	dprint_vfmw
	mov	w0, -1
	b	.L489
	.size	FSP_GetDisplay, .-FSP_GetDisplay
	.align	3
.LC175:
	.xword	.LANCHOR0
	.align	3
.LC176:
	.xword	.LC24
	.align	3
.LC177:
	.xword	.LC9
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_SetStoreType
	.type	FSP_SetStoreType, %function
FSP_SetStoreType:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	w22, w1
	sxtw	x19, w2
	mov	w20, w3
	bl	_mcount
	cmp	w21, 15
	bhi	.L499
	ldr	x0, .LC180
	ldr	x4, [x0, w21, sxtw 3]
	cbz	x4, .L500
	cbz	w22, .L501
	cmp	w22, 1
	bne	.L498
	ldrsb	w0, [x4, 11]
	cmp	w19, w0
	bge	.L498
	lsl	x1, x19, 2
	mov	w0, 0
	sub	x1, x1, x19
	lsl	x2, x1, 5
	add	x2, x1, x2
	add	x2, x2, x19
	add	x19, x4, x2
	add	x19, x19, 24576
	str	w20, [x19, 552]
.L493:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L501:
	ldrsb	w0, [x4, 10]
	cmp	w0, w19
	ble	.L498
	lsl	x1, x19, 2
	mov	w0, 0
	sub	x1, x1, x19
	lsl	x2, x1, 5
	add	x1, x1, x2
	add	x19, x1, x19
	add	x19, x4, x19
	add	x19, x19, 20480
	str	w20, [x19, 1448]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L498:
	mov	w0, -1
	b	.L493
	.p2align 3
.L499:
	ldr	x3, .LC178
	mov	w2, 1845
	ldr	x1, .LC179
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L493
.L500:
	ldr	x3, .LC181
	mov	w2, 1846
	ldr	x1, .LC179
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L493
	.size	FSP_SetStoreType, .-FSP_SetStoreType
	.align	3
.LC178:
	.xword	.LC7
	.align	3
.LC179:
	.xword	.LC9
	.align	3
.LC180:
	.xword	.LANCHOR0
	.align	3
.LC181:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetStoreType
	.type	FSP_GetStoreType, %function
FSP_GetStoreType:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w21, w1
	sxtw	x20, w2
	bl	_mcount
	cmp	w19, 15
	bhi	.L510
	ldr	x0, .LC184
	ldr	x2, [x0, w19, sxtw 3]
	cbz	x2, .L511
	cbnz	w21, .L506
	ldrsb	w1, [x2, 10]
	mov	w0, 2
	cmp	w1, w20
	bgt	.L512
.L504:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L510:
	ldr	x3, .LC182
	mov	w2, 1878
	ldr	x1, .LC183
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L506:
	cmp	w21, 1
	mov	w0, 2
	bne	.L504
	ldrsb	w1, [x2, 11]
	cmp	w20, w1
	bge	.L504
	sxtw	x1, w20
	lsl	x0, x1, 2
	sub	x0, x0, x1
	lsl	x3, x0, 5
	add	x0, x0, x3
	add	x1, x0, x1
	add	x0, x2, x1
	add	x0, x0, 24576
	ldr	w0, [x0, 552]
	b	.L504
	.p2align 3
.L512:
	lsl	x0, x20, 2
	sub	x0, x0, x20
	lsl	x1, x0, 5
	add	x0, x0, x1
	add	x20, x0, x20
	add	x0, x2, x20
	add	x0, x0, 20480
	ldr	w0, [x0, 1448]
	b	.L504
	.p2align 3
.L511:
	ldr	x3, .LC185
	mov	w2, 1879
	ldr	x1, .LC183
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L504
	.size	FSP_GetStoreType, .-FSP_GetStoreType
	.align	3
.LC182:
	.xword	.LC7
	.align	3
.LC183:
	.xword	.LC9
	.align	3
.LC184:
	.xword	.LANCHOR0
	.align	3
.LC185:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ClearLogicFs
	.type	FSP_ClearLogicFs, %function
FSP_ClearLogicFs:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w19, 15
	ccmp	w20, 31, 2, ls
	bhi	.L513
	ldr	x21, .LC186
	ldr	x0, [x21, w19, sxtw 3]
	cbz	x0, .L513
	mov	w2, 0
	mov	w1, w20
	mov	w0, w19
	bl	FSP_SetRef
	sxtw	x1, w20
	ldr	x2, [x21, w19, sxtw 3]
	lsl	x0, x1, 5
	add	x0, x0, x1, lsl 3
	add	x0, x0, x0, lsl 4
	add	x0, x2, x0
	ldrsb	w0, [x0, 57]
	cmp	w0, 3
	beq	.L513
	mov	w2, 0
	mov	w1, w20
	mov	w0, w19
	bl	FSP_SetDisplay
.L513:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	FSP_ClearLogicFs, .-FSP_ClearLogicFs
	.align	3
.LC186:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetLogicFs
	.type	FSP_GetLogicFs, %function
FSP_GetLogicFs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w20, 15
	ccmp	w19, 31, 2, ls
	bhi	.L526
	ldr	x0, .LC187
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L526
	lsl	x1, x19, 5
	add	x19, x1, x19, lsl 3
	add	x19, x19, x19, lsl 4
	add	x0, x0, x19
	add	x0, x0, 56
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L526:
	mov	x0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_GetLogicFs, .-FSP_GetLogicFs
	.align	3
.LC187:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetFsImagePtr
	.type	FSP_GetFsImagePtr, %function
FSP_GetFsImagePtr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w20, 15
	ccmp	w19, 31, 2, ls
	bhi	.L530
	ldr	x0, .LC188
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L530
	lsl	x1, x19, 5
	add	x19, x1, x19, lsl 3
	add	x19, x19, x19, lsl 4
	add	x0, x0, x19
	add	x0, x0, 64
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L530:
	mov	x0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_GetFsImagePtr, .-FSP_GetFsImagePtr
	.align	3
.LC188:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetDecPhyFs
	.type	FSP_GetDecPhyFs, %function
FSP_GetDecPhyFs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w20, 15
	ccmp	w19, 31, 2, ls
	bhi	.L534
	ldr	x0, .LC189
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L534
	lsl	x1, x19, 5
	add	x19, x1, x19, lsl 3
	add	x19, x19, x19, lsl 4
	add	x19, x0, x19
	ldr	x0, [x19, 696]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L534:
	mov	x0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_GetDecPhyFs, .-FSP_GetDecPhyFs
	.align	3
.LC189:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetDispPhyFs
	.type	FSP_GetDispPhyFs, %function
FSP_GetDispPhyFs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w20, 15
	ccmp	w19, 31, 2, ls
	bhi	.L538
	ldr	x0, .LC190
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L538
	lsl	x1, x19, 5
	add	x19, x1, x19, lsl 3
	add	x19, x19, x19, lsl 4
	add	x19, x0, x19
	ldr	x0, [x19, 704]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L538:
	mov	x0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_GetDispPhyFs, .-FSP_GetDispPhyFs
	.align	3
.LC190:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetPmvAddr
	.type	FSP_GetPmvAddr, %function
FSP_GetPmvAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	sxtw	x19, w1
	bl	_mcount
	cmp	w20, 15
	ccmp	w19, 31, 2, ls
	bhi	.L542
	ldr	x0, .LC191
	ldr	x0, [x0, w20, sxtw 3]
	cbz	x0, .L542
	lsl	x1, x19, 5
	add	x19, x1, x19, lsl 3
	add	x19, x19, x19, lsl 4
	add	x19, x0, x19
	ldr	w0, [x19, 720]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L542:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_GetPmvAddr, .-FSP_GetPmvAddr
	.align	3
.LC191:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetDecFsAddrTab
	.type	FSP_GetDecFsAddrTab, %function
FSP_GetDecFsAddrTab:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w22, w0
	mov	x0, x30
	mov	x19, x1
	mov	x20, x2
	mov	x21, x3
	bl	_mcount
	cmp	w22, 15
	bhi	.L554
	cbz	x20, .L555
	cbz	x19, .L556
	ldr	x2, .LC198
	ldr	x0, [x2, w22, sxtw 3]
	cbz	x0, .L557
	str	wzr, [x19]
	mov	w0, 0
	mov	w1, 100
	ldr	x3, [x2, w22, sxtw 3]
	ldrsb	w3, [x3, 10]
	str	w3, [x19]
	cmp	w3, wzr
	ldr	x6, [x2, w22, sxtw 3]
	add	x6, x6, 20480
	add	x6, x6, 1436
	ble	.L543
	.p2align 2
.L552:
	smaddl	x4, w0, w1, x6
	ldr	w5, [x4, 16]
	str	w5, [x20, w0, sxtw 2]
	ldr	w4, [x4, 28]
	str	w4, [x21, w0, sxtw 2]
	add	w0, w0, 1
	ldr	w4, [x19]
	cmp	w4, w0
	bgt	.L552
.L543:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L554:
	ldr	x3, .LC192
	mov	w2, 2025
	ldr	x1, .LC193
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L555:
	ldr	x3, .LC195
	mov	w2, 2026
	ldr	x1, .LC193
	mov	w0, 0
	bl	dprint_vfmw
	b	.L543
.L556:
	ldr	x3, .LC197
	mov	w2, 2027
	ldr	x1, .LC193
	mov	w0, 0
	bl	dprint_vfmw
	b	.L543
.L557:
	ldr	x3, .LC199
	mov	w2, 2028
	ldr	x1, .LC193
	bl	dprint_vfmw
	b	.L543
	.size	FSP_GetDecFsAddrTab, .-FSP_GetDecFsAddrTab
	.align	3
.LC192:
	.xword	.LC7
	.align	3
.LC193:
	.xword	.LC9
	.align	3
.LC195:
	.xword	.LC194
	.align	3
.LC197:
	.xword	.LC196
	.align	3
.LC198:
	.xword	.LANCHOR0
	.align	3
.LC199:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetPmvAddrTab
	.type	FSP_GetPmvAddrTab, %function
FSP_GetPmvAddrTab:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x21, x1
	mov	x19, x2
	bl	_mcount
	cmp	w20, 15
	bhi	.L569
	cbz	x19, .L570
	cbz	x21, .L571
	ldr	x0, .LC206
	ldr	x1, [x0, w20, sxtw 3]
	cbz	x1, .L572
	str	wzr, [x21]
	mov	w3, 0
	ldr	x4, [x0, w20, sxtw 3]
	ldrsb	w0, [x4, 13]
	str	w0, [x21]
	ldrsb	w0, [x4, 13]
	cmp	w0, wzr
	ble	.L558
	.p2align 2
.L567:
	sxtw	x0, w3
	add	x0, x0, 3612
	add	x0, x4, x0, lsl 3
	ldr	w0, [x0, 8]
	str	w0, [x19, w3, sxtw 2]
	add	w3, w3, 1
	ldrsb	w0, [x4, 13]
	cmp	w0, w3
	bgt	.L567
.L558:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L569:
	ldr	x3, .LC200
	mov	w2, 2058
	ldr	x1, .LC201
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L570:
	ldr	x3, .LC203
	mov	w2, 2059
	ldr	x1, .LC201
	mov	w0, 0
	bl	dprint_vfmw
	b	.L558
.L571:
	ldr	x3, .LC205
	mov	w2, 2060
	ldr	x1, .LC201
	mov	w0, 0
	bl	dprint_vfmw
	b	.L558
.L572:
	ldr	x3, .LC207
	mov	w2, 2061
	ldr	x1, .LC201
	mov	w0, 0
	bl	dprint_vfmw
	b	.L558
	.size	FSP_GetPmvAddrTab, .-FSP_GetPmvAddrTab
	.align	3
.LC200:
	.xword	.LC7
	.align	3
.LC201:
	.xword	.LC9
	.align	3
.LC203:
	.xword	.LC202
	.align	3
.LC205:
	.xword	.LC204
	.align	3
.LC206:
	.xword	.LANCHOR0
	.align	3
.LC207:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetInstInfo
	.type	FSP_GetInstInfo, %function
FSP_GetInstInfo:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x0, .LC208
	ldr	x3, [x0, w20, sxtw 3]
	cbz	x3, .L576
	ldrsb	w0, [x3, 10]
	add	x1, x3, 20480
	strb	w0, [x19, 1]
	mov	x0, x19
	ldr	x5, .LC211
	mov	x2, 3200
	ldr	w6, [x1, 1452]
	add	x1, x1, 1436
	ldr	w4, [x1, 116]
	sub	w4, w4, w6
	str	w4, [x19, 3208]
	ldrsb	w4, [x3, 11]
	strb	w4, [x19, 2]
	ldrsb	w4, [x3, 12]
	strb	w4, [x19, 3]
	ldrsb	w4, [x3]
	strb	w4, [x19, 4]
	ldrsb	w3, [x3, 13]
	strb	w3, [x0], 8
	ldr	x3, [x5, 104]
	blr	x3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L576:
	ldr	x3, .LC209
	mov	w2, 2079
	ldr	x1, .LC210
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_GetInstInfo, .-FSP_GetInstInfo
	.align	3
.LC208:
	.xword	.LANCHOR0
	.align	3
.LC209:
	.xword	.LC24
	.align	3
.LC210:
	.xword	.LC9
	.align	3
.LC211:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetLogicIdByPhyAddr
	.type	FSP_GetLogicIdByPhyAddr, %function
FSP_GetLogicIdByPhyAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cmp	w20, 15
	bhi	.L589
	ldr	x1, .LC214
	mov	w0, 0
	ldr	x1, [x1, w20, sxtw 3]
	add	x2, x1, 696
	cbnz	x1, .L586
	b	.L590
	.p2align 3
.L581:
	add	w0, w0, 1
	cmp	w0, 32
	beq	.L591
.L586:
	ldr	x3, [x2]
	add	x2, x2, 680
	cbz	x3, .L581
	ldr	w3, [x3, 16]
	cmp	w3, w19
	bne	.L581
.L579:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L591:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L589:
	ldr	x3, .LC212
	mov	w2, 2102
	ldr	x1, .LC213
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L579
.L590:
	ldr	x3, .LC215
	mov	w2, 2103
	ldr	x1, .LC213
	bl	dprint_vfmw
	mov	w0, -1
	b	.L579
	.size	FSP_GetLogicIdByPhyAddr, .-FSP_GetLogicIdByPhyAddr
	.align	3
.LC212:
	.xword	.LC7
	.align	3
.LC213:
	.xword	.LC9
	.align	3
.LC214:
	.xword	.LANCHOR0
	.align	3
.LC215:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_PrintContext
	.type	FSP_PrintContext, %function
FSP_PrintContext:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L621
	ldr	x0, .LC218
	ldr	x21, [x0, w19, sxtw 3]
	cbz	x21, .L622
	ldr	x1, .LC221
	mov	w0, 0
	add	x19, x21, 56
	mov	w23, 1
	mov	w20, 0
	bl	dprint_vfmw
	ldr	x22, .LC225
	ldr	x2, .LC223
	mov	w0, 0
	ldr	w3, [x21, 4]
	mov	x1, x22
	bl	dprint_vfmw
	ldr	x2, .LC227
	mov	x1, x22
	ldr	w3, [x21, 28]
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x2, .LC229
	mov	x1, x22
	ldr	w3, [x21, 32]
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x2, .LC231
	mov	x1, x22
	ldr	w3, [x21, 44]
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x2, .LC233
	mov	x1, x22
	ldr	w3, [x21, 48]
	mov	w0, 0
	bl	dprint_vfmw
	ldrsb	w3, [x21, 23]
	mov	x1, x22
	ldr	x2, .LC235
	mov	w0, 0
	bl	dprint_vfmw
	ldrsb	w3, [x21, 25]
	mov	x1, x22
	ldr	x2, .LC237
	mov	w0, 0
	bl	dprint_vfmw
	ldrsb	w3, [x21, 26]
	mov	x1, x22
	ldr	x2, .LC239
	mov	w0, 0
	bl	dprint_vfmw
	ldrsb	w3, [x21, 10]
	mov	x1, x22
	ldr	x2, .LC241
	mov	w0, 0
	bl	dprint_vfmw
	ldrsb	w3, [x21, 11]
	mov	x1, x22
	ldr	x2, .LC243
	mov	w0, 0
	bl	dprint_vfmw
	ldrsb	w3, [x21, 13]
	mov	x1, x22
	ldr	x2, .LC245
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, .LC247
	mov	w2, 32
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x22, .LC251
	ldr	x24, .LC249
	b	.L596
	.p2align 3
.L598:
	ldrsb	w3, [x19, 1]
	mov	x1, x22
	ldrsb	w2, [x19]
	mov	w0, 0
	bl	dprint_vfmw
	cmp	w23, 31
	bgt	.L623
.L599:
	add	w20, w20, 1
	add	w23, w23, 1
	add	x19, x19, 680
.L596:
	and	w0, w20, 7
	cbz	w20, .L597
	cbnz	w0, .L598
	mov	x1, x24
	bl	dprint_vfmw
	ldrsb	w3, [x19, 1]
	mov	x1, x22
	ldrsb	w2, [x19]
	mov	w0, 0
	bl	dprint_vfmw
	cmp	w23, 31
	ble	.L599
.L623:
	ldrsb	w2, [x21, 10]
	mov	w0, 0
	ldr	x1, .LC253
	bl	dprint_vfmw
	ldrsb	w0, [x21, 10]
	cmp	w0, wzr
	ble	.L600
	ldr	x20, .LC249
	mov	w19, 0
	.p2align 2
.L601:
	sxtw	x2, w19
	mov	x1, x22
	lsl	x3, x2, 2
	mov	w0, 0
	sub	x3, x3, x2
	add	w19, w19, 1
	lsl	x4, x3, 5
	add	x3, x3, x4
	add	x2, x3, x2
	add	x2, x21, x2
	add	x2, x2, 20480
	ldrsb	w3, [x2, 1438]
	ldrsb	w2, [x2, 1436]
	bl	dprint_vfmw
	ldrsb	w0, [x21, 10]
	and	w1, w19, 7
	cmp	w0, w19
	ble	.L600
	cbz	w19, .L601
	cbnz	w1, .L601
	mov	x1, x20
	mov	w0, 0
	bl	dprint_vfmw
	b	.L601
	.p2align 3
.L600:
	ldrsb	w2, [x21, 11]
	mov	w0, 0
	ldr	x1, .LC255
	bl	dprint_vfmw
	ldrsb	w0, [x21, 11]
	cmp	w0, wzr
	ble	.L602
	ldr	x20, .LC249
	mov	w19, 0
	.p2align 2
.L603:
	sxtw	x2, w19
	mov	x1, x22
	lsl	x3, x2, 2
	mov	w0, 0
	sub	x3, x3, x2
	add	w19, w19, 1
	lsl	x4, x3, 5
	add	x3, x3, x4
	add	x2, x3, x2
	add	x2, x21, x2
	add	x2, x2, 24576
	ldrsb	w3, [x2, 542]
	ldrsb	w2, [x2, 540]
	bl	dprint_vfmw
	ldrsb	w0, [x21, 11]
	and	w1, w19, 7
	cmp	w0, w19
	ble	.L602
	cbz	w19, .L603
	cbnz	w1, .L603
	mov	x1, x20
	mov	w0, 0
	bl	dprint_vfmw
	b	.L603
	.p2align 3
.L602:
	ldrsb	w2, [x21, 13]
	mov	w0, 0
	ldr	x1, .LC257
	bl	dprint_vfmw
	ldrsb	w0, [x21, 13]
	cmp	w0, wzr
	ble	.L606
	ldr	x20, .LC261
	mov	w19, 0
	ldr	x22, .LC249
	.p2align 2
.L607:
	sxtw	x2, w19
	mov	x1, x20
	add	x2, x2, 3612
	mov	w0, 0
	add	w19, w19, 1
	add	x2, x21, x2, lsl 3
	ldrb	w2, [x2, 13]
	bl	dprint_vfmw
	ldrsb	w0, [x21, 13]
	and	w1, w19, 15
	cmp	w0, w19
	ble	.L606
	cbz	w19, .L607
	cbnz	w1, .L607
	mov	x1, x22
	mov	w0, 0
	bl	dprint_vfmw
	b	.L607
	.p2align 3
.L606:
	ldr	x1, .LC259
	mov	w0, 0
	bl	dprint_vfmw
.L592:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L621:
	ldr	x3, .LC216
	mov	w2, 2134
	ldr	x1, .LC217
	mov	w0, 0
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L622:
	ldr	x3, .LC219
	mov	w2, 2135
	ldr	x1, .LC217
	mov	w0, 0
	bl	dprint_vfmw
	b	.L592
.L597:
	ldrsb	w3, [x19, 1]
	mov	x1, x22
	ldrsb	w2, [x19]
	mov	w0, 0
	bl	dprint_vfmw
	b	.L599
	.size	FSP_PrintContext, .-FSP_PrintContext
	.align	3
.LC216:
	.xword	.LC7
	.align	3
.LC217:
	.xword	.LC9
	.align	3
.LC218:
	.xword	.LANCHOR0
	.align	3
.LC219:
	.xword	.LC24
	.align	3
.LC221:
	.xword	.LC220
	.align	3
.LC223:
	.xword	.LC222
	.align	3
.LC225:
	.xword	.LC224
	.align	3
.LC227:
	.xword	.LC226
	.align	3
.LC229:
	.xword	.LC228
	.align	3
.LC231:
	.xword	.LC230
	.align	3
.LC233:
	.xword	.LC232
	.align	3
.LC235:
	.xword	.LC234
	.align	3
.LC237:
	.xword	.LC236
	.align	3
.LC239:
	.xword	.LC238
	.align	3
.LC241:
	.xword	.LC240
	.align	3
.LC243:
	.xword	.LC242
	.align	3
.LC245:
	.xword	.LC244
	.align	3
.LC247:
	.xword	.LC246
	.align	3
.LC249:
	.xword	.LC248
	.align	3
.LC251:
	.xword	.LC250
	.align	3
.LC253:
	.xword	.LC252
	.align	3
.LC255:
	.xword	.LC254
	.align	3
.LC257:
	.xword	.LC256
	.align	3
.LC259:
	.xword	.LC258
	.align	3
.LC261:
	.xword	.LC260
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_IsRefLeak
	.type	FSP_IsRefLeak, %function
FSP_IsRefLeak:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w1
	mov	w20, 1
	bl	_mcount
	str	wzr, [x29, 76]
	ldr	w0, [x19, 28]
	stp	wzr, wzr, [x29, 68]
	cmp	w0, 1920
	bgt	.L625
	ldr	w0, [x19, 32]
	mov	w20, 4
	cmp	w0, 1088
	csinc	w20, w20, wzr, le
.L625:
	ldrsb	w0, [x19, 10]
	cmp	w0, wzr
	ble	.L634
	sub	w1, w0, #1
	mov	w3, 100
	mov	x2, 22112
	add	x0, x19, 20480
	add	x0, x0, 1532
	mov	w22, 0
	umaddl	x2, w1, w3, x2
	mov	w23, 0
	add	x2, x19, x2
	b	.L629
	.p2align 3
.L627:
	add	x0, x0, 100
	cmp	x0, x2
	beq	.L626
.L629:
	ldr	w1, [x0]
	cmp	w1, 1
	bne	.L627
	ldrsb	w1, [x0, -96]
	cmp	w1, 1
	beq	.L642
	ldrsb	w3, [x0, -94]
	cmp	w3, 2
	bne	.L627
	cmp	w1, wzr
	add	x0, x0, 100
	csinc	w22, w22, w22, ne
	cmp	x0, x2
	bne	.L629
	.p2align 2
.L626:
	add	x2, x29, 76
	add	x1, x29, 68
	mov	w0, w21
	add	x3, x29, 72
	bl	VCTRL_GetChanImgNum
	ldr	w1, [x29, 68]
	mov	w0, 0
	ldrsb	w2, [x19, 10]
	add	w1, w20, w1
	cmp	w1, w2
	bcc	.L638
	add	w22, w23, w22
	add	w20, w22, w20
	cmp	w2, w20
	bhi	.L638
	ldr	x1, .LC262
	ldr	x1, [x1, w21, sxtw 3]
	ldr	w1, [x1, 692]
	cmp	w1, 2
	beq	.L638
	mov	w0, w21
	bl	FSP_PrintContext
	mov	w0, 1
.L638:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L642:
	add	w23, w23, 1
	b	.L627
.L634:
	mov	w22, 0
	mov	w23, 0
	b	.L626
	.size	FSP_IsRefLeak, .-FSP_IsRefLeak
	.align	3
.LC262:
	.xword	s_pstVfmwChan
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_IsNewFsAvalible
	.type	FSP_IsNewFsAvalible, %function
FSP_IsNewFsAvalible:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	w23, w0
	mov	x0, x30
	bl	_mcount
	cmp	w23, 15
	bhi	.L694
	ldr	x22, .LC265
	ldr	x19, [x22, w23, sxtw 3]
	cbz	x19, .L695
	ldr	x0, .LC267
	ldr	w1, [x0]
	cbnz	w1, .L647
.L651:
	ldrsb	w0, [x19, 10]
	cbnz	w0, .L696
.L648:
	mov	w1, 1
.L645:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L696:
	mov	w0, w23
	bl	VCTRL_GetVidStd
	cmp	w0, 17
	beq	.L652
	mov	w0, w23
	bl	VCTRL_GetVidStd
	cmp	w0, 16
	beq	.L653
.L657:
	ldrsb	w24, [x19, 10]
	cmp	w24, wzr
	ble	.L661
	add	x20, x19, 20480
	mov	w21, 0
	add	x20, x20, 1436
	b	.L660
	.p2align 3
.L658:
	add	w21, w21, 1
	add	x20, x20, 100
	cmp	w21, w24
	beq	.L661
.L660:
	ldr	w0, [x20, 96]
	cmp	w0, 1
	bne	.L658
	sxtw	x0, w21
	lsl	x1, x0, 2
	sub	x1, x1, x0
	lsl	x2, x1, 5
	add	x1, x1, x2
	add	x0, x1, x0
	add	x0, x19, x0
	add	x0, x0, 20480
	ldr	w0, [x0, 1436]
	tst	w0, 16711935
	bne	.L658
	mov	x1, x20
	mov	w0, w23
	bl	FSP_IsFsStatusReady.part.5
	cbz	w0, .L658
	cmp	w24, w21
	ble	.L661
.L666:
	mov	w0, w23
	bl	VCTRL_GetVidStd
	cmp	w0, 17
	beq	.L648
	ldrsb	w3, [x19, 13]
	cmp	w3, wzr
	ble	.L674
	add	x0, x19, 28672
	mov	w1, 0
	add	x0, x0, 236
	b	.L670
	.p2align 3
.L668:
	add	w1, w1, 1
	add	x0, x0, 8
	cmp	w1, w3
	beq	.L671
.L670:
	ldrb	w2, [x0]
	cmp	w2, 1
	bne	.L668
	ldrb	w2, [x0, 1]
	cbnz	w2, .L668
	cmp	w3, w1
	bgt	.L648
.L667:
	cmp	w3, w1
	bne	.L661
	.p2align 2
.L671:
	mov	w0, w23
	bl	VCTRL_GetVidStd
	cbz	w0, .L648
	mov	w0, w23
	bl	VCTRL_GetVidStd
	cmp	w0, 16
	beq	.L648
	mov	w0, w23
	bl	VCTRL_GetVidStd
	cmp	w0, 15
	beq	.L648
	.p2align 2
.L661:
	ldr	x0, [x22, w23, sxtw 3]
	cbz	x0, .L697
	mov	w1, 0
.L699:
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L647:
	ldr	x1, [x0, 8]
	cbz	x1, .L698
	mov	w0, w23
	blr	x1
	mov	w1, 0
	cbz	w0, .L651
	mov	w0, w1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L652:
	ldr	x0, .LC269
	add	x20, x19, 20480
	add	x20, x20, 1436
	mov	w21, 0
	ldr	x0, [x0, w23, sxtw 3]
	add	x0, x0, 622592
	ldr	w24, [x0, 1912]
	b	.L665
	.p2align 3
.L663:
	add	w21, w21, 1
	add	x20, x20, 100
	cmp	w21, 32
	beq	.L661
.L665:
	ldr	w0, [x20, 96]
	cmp	w0, 1
	bne	.L663
	sxtw	x0, w21
	lsl	x1, x0, 2
	sub	x1, x1, x0
	lsl	x2, x1, 5
	add	x1, x1, x2
	add	x0, x1, x0
	add	x0, x19, x0
	add	x0, x0, 20480
	ldr	w0, [x0, 1436]
	tst	w0, 16711935
	bne	.L663
	mov	x1, x20
	mov	w0, w23
	bl	FSP_IsFsStatusReady.part.5
	cbz	w0, .L663
	ldr	w0, [x20, 20]
	cmp	w24, w0
	bne	.L663
	cmp	w21, 31
	ble	.L666
	b	.L661
	.p2align 3
.L697:
	ldr	x1, .LC270
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, 0
	b	.L699
.L653:
	mov	w1, w23
	mov	x0, x19
	bl	FSP_IsRefLeak
	cbz	w0, .L657
	mov	w1, -1
	b	.L645
	.p2align 3
.L694:
	ldr	x1, .LC264
	mov	w2, 1066
	ldr	x3, .LC263
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L645
.L674:
	mov	w1, 0
	b	.L667
.L695:
	ldr	x1, .LC264
	mov	w2, 1067
	ldr	x3, .LC266
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	b	.L645
.L698:
	ldr	x1, .LC268
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 0
	b	.L645
	.size	FSP_IsNewFsAvalible, .-FSP_IsNewFsAvalible
	.align	3
.LC263:
	.xword	.LC7
	.align	3
.LC264:
	.xword	.LC9
	.align	3
.LC265:
	.xword	.LANCHOR0
	.align	3
.LC266:
	.xword	.LC24
	.align	3
.LC267:
	.xword	.LANCHOR1
	.align	3
.LC268:
	.xword	.LC42
	.align	3
.LC269:
	.xword	s_pstVfmwChan
	.align	3
.LC270:
	.xword	.LC108
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetQueSwitchFlag
	.type	FSP_GetQueSwitchFlag, %function
FSP_GetQueSwitchFlag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	sxtw	x19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L703
	ldr	x0, .LC273
	lsl	x1, x19, 8
	add	x19, x1, x19, lsl 3
	add	x0, x0, 128
	ldrb	w0, [x0, x19]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L703:
	ldr	x3, .LC271
	mov	w2, 2223
	ldr	x1, .LC272
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	mov	w0, 255
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_GetQueSwitchFlag, .-FSP_GetQueSwitchFlag
	.align	3
.LC271:
	.xword	.LC7
	.align	3
.LC272:
	.xword	.LC9
	.align	3
.LC273:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_SetQueSwitchFlag
	.type	FSP_SetQueSwitchFlag, %function
FSP_SetQueSwitchFlag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	sxtw	x19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L707
	ldr	x0, .LC277
	lsl	x2, x19, 8
	add	x19, x2, x19, lsl 3
	mov	w1, 1
	add	x0, x0, 128
	strb	w1, [x0, x19]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L707:
	ldr	x3, .LC275
	mov	w2, 2233
	ldr	x1, .LC276
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_SetQueSwitchFlag, .-FSP_SetQueSwitchFlag
	.align	3
.LC275:
	.xword	.LC7
	.align	3
.LC276:
	.xword	.LC9
	.align	3
.LC277:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_IsFsInVO
	.type	FSP_IsFsInVO, %function
FSP_IsFsInVO:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x2, .LC279
	sxtw	x4, w20
	ldr	x1, [x2, w20, sxtw 3]
	cbz	x1, .L715
	lsl	x0, x4, 8
	add	x3, x2, 128
	add	x0, x0, x4, lsl 3
	add	x0, x3, x0
	ldrsh	w5, [x0, 2]
	cmn	w5, #1
	beq	.L715
	ldrh	w2, [x0, 4]
	cmp	w2, w5
	beq	.L715
	ldr	w1, [x1, 4]
	mov	w0, 0
	cmp	w1, 1
	beq	.L722
.L709:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L712:
	add	x0, x3, x0, lsl 2
	ldr	w0, [x0, 8]
	cmp	w0, w19
	beq	.L719
.L711:
	add	w2, w2, 1
	and	w2, w2, 63
	cmp	w5, w2
	add	x0, x4, x2, sxtw
	bne	.L712
.L715:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L722:
	and	w2, w2, 63
	cmp	w5, w2
	beq	.L709
	lsl	x0, x4, 6
	add	x4, x0, x4, lsl 1
	add	x0, x4, x2, sxtw
	add	x0, x3, x0, lsl 2
	ldr	w0, [x0, 8]
	cmp	w19, w0
	bne	.L711
	mov	w0, w1
	b	.L709
	.p2align 3
.L719:
	mov	w0, 1
	b	.L709
	.size	FSP_IsFsInVO, .-FSP_IsFsInVO
	.align	3
.LC279:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_ProcFspQue
	.type	FSP_ProcFspQue, %function
FSP_ProcFspQue:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	mov	w21, w2
	bl	_mcount
	cbz	x19, .L742
	cmp	w21, 1
	bhi	.L743
	sxtw	x10, w20
	beq	.L744
	ldr	x11, .LC286
	lsl	x9, x10, 8
	add	x9, x9, x10, lsl 3
	add	x5, x11, 128
	add	x9, x5, x9
	ldrh	w7, [x9, 4]
	ldrh	w1, [x9, 6]
	and	w3, w7, 63
	cmp	w3, w1
	beq	.L728
	lsl	x4, x10, 6
	add	x4, x4, x10, lsl 1
	.p2align 2
.L733:
	uxtw	x0, w3
	ldr	w6, [x19, 28]
	add	x2, x4, x0
	add	x2, x5, x2, lsl 2
	ldr	w2, [x2, 8]
	cmp	w6, w2
	beq	.L734
	mov	w8, w3
	cbnz	w2, .L730
	mov	w12, 2
.L729:
	mov	w8, w7
	.p2align 2
.L732:
	cmp	w3, wzr
	sub	w6, w3, #1
	mov	w2, 63
	add	x0, x4, x0
	csel	w2, w6, w2, ne
	cmp	w3, w7
	add	x6, x5, x0, lsl 2
	mov	w3, w2
	uxtw	x0, w2
	add	x2, x4, x0
	add	x2, x5, x2, lsl 2
	ldr	w2, [x2, 8]
	str	w2, [x6, 8]
	bne	.L732
	add	w7, w7, 1
	cmp	w12, 1
	and	w7, w7, 63
	strh	w7, [x9, 4]
	beq	.L728
.L730:
	add	w3, w8, 1
	and	w3, w3, 63
	cmp	w3, w1
	bne	.L733
.L728:
	lsl	x0, x10, 8
	add	x11, x11, 128
	add	x10, x0, x10, lsl 3
	add	x0, x11, x10
	ldrsh	w1, [x0, 2]
	cmp	w7, w1
	bne	.L723
	mov	w1, -1
	strb	wzr, [x11, x10]
	strh	w1, [x0, 2]
.L723:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L734:
	mov	w12, 1
	b	.L729
	.p2align 3
.L743:
	ldr	x3, .LC285
	mov	w2, 2294
	ldr	x1, .LC283
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L744:
	ldr	x0, .LC286
	lsl	x1, x10, 8
	add	x1, x1, x10, lsl 3
	lsl	x3, x10, 6
	add	x2, x0, 128
	add	x10, x3, x10, lsl 1
	add	x1, x2, x1
	ldr	w4, [x19, 28]
	ldrh	w0, [x1, 6]
	add	w3, w0, 1
	add	x10, x10, x0, sxtw
	and	w0, w3, 63
	strh	w0, [x1, 6]
	add	x0, x2, x10, lsl 2
	str	w4, [x0, 8]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L742:
	ldr	x3, .LC282
	mov	w2, 2293
	ldr	x1, .LC283
	mov	w0, 0
	bl	dprint_vfmw
	b	.L723
	.size	FSP_ProcFspQue, .-FSP_ProcFspQue
	.align	3
.LC282:
	.xword	.LC281
	.align	3
.LC283:
	.xword	.LC9
	.align	3
.LC285:
	.xword	.LC284
	.align	3
.LC286:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetNullLogicFs
	.type	FSP_GetNullLogicFs, %function
FSP_GetNullLogicFs:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC288
	ldr	x3, [x0, w19, sxtw 3]
	add	x0, x3, 20480
	ldrsb	w0, [x0, 657]
	cbz	w0, .L748
	add	x1, x3, 16384
	mov	w0, 30
	add	x1, x1, 4073
	b	.L747
	.p2align 3
.L754:
	sub	w0, w0, #1
	cmn	w0, #1
	beq	.L753
.L747:
	ldrsb	w2, [x1]
	sub	x1, x1, #680
	cbnz	w2, .L754
.L746:
	sxtw	x2, w0
	lsl	x1, x2, 5
	add	x2, x1, x2, lsl 3
	add	x2, x2, x2, lsl 4
	add	x1, x3, x2
	str	xzr, [x1, 696]
	str	xzr, [x1, 704]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L753:
	mov	w0, 0
	b	.L746
.L748:
	mov	w0, 31
	b	.L746
	.size	FSP_GetNullLogicFs, .-FSP_GetNullLogicFs
	.align	3
.LC288:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_SwitchNotify
	.type	FSP_SwitchNotify, %function
FSP_SwitchNotify:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bls	.L755
	ldr	x3, .LC289
	mov	w2, 2405
	ldr	x1, .LC290
	mov	w0, 0
	bl	dprint_vfmw
.L755:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	FSP_SwitchNotify, .-FSP_SwitchNotify
	.align	3
.LC289:
	.xword	.LC7
	.align	3
.LC290:
	.xword	.LC9
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetFspType
	.type	FSP_GetFspType, %function
FSP_GetFspType:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L761
	ldr	x0, .LC293
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L762
	ldr	w0, [x0, 4]
	cmp	w0, 1
	csetm	w0, ne
.L759:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L761:
	ldr	x3, .LC291
	mov	w2, 2436
	ldr	x1, .LC292
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L759
.L762:
	ldr	x3, .LC294
	mov	w2, 2437
	ldr	x1, .LC292
	bl	dprint_vfmw
	mov	w0, -1
	b	.L759
	.size	FSP_GetFspType, .-FSP_GetFspType
	.align	3
.LC291:
	.xword	.LC7
	.align	3
.LC292:
	.xword	.LC9
	.align	3
.LC293:
	.xword	.LANCHOR0
	.align	3
.LC294:
	.xword	.LC24
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FSP_GetTotalValidFsNum
	.type	FSP_GetTotalValidFsNum, %function
FSP_GetTotalValidFsNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	cmp	w19, 15
	bhi	.L767
	ldr	x0, .LC297
	ldr	x0, [x0, w19, sxtw 3]
	cbz	x0, .L768
	ldrsb	w0, [x0, 9]
.L765:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L767:
	ldr	x3, .LC295
	mov	w2, 2454
	ldr	x1, .LC296
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L765
.L768:
	ldr	x3, .LC298
	mov	w2, 2455
	ldr	x1, .LC296
	bl	dprint_vfmw
	mov	w0, -1
	b	.L765
	.size	FSP_GetTotalValidFsNum, .-FSP_GetTotalValidFsNum
	.align	3
.LC295:
	.xword	.LC7
	.align	3
.LC296:
	.xword	.LC9
	.align	3
.LC297:
	.xword	.LANCHOR0
	.align	3
.LC298:
	.xword	.LC24
	.global	s_FspVoQue
	.global	s_pFspInst
	.section	.rodata
	.align	3
.LANCHOR2 = . + 0
	.type	__func__.11813, %object
	.size	__func__.11813, 15
__func__.11813:
	.string	"FSP_NewLogicFs"
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC7:
	ASCII(.string	"InstID out of range" )
	.zero	4
.LC9:
	ASCII(.string	"fsp.c,L%d: %s\n" )
	.zero	1
.LC12:
	ASCII(.string	"fsp instance not in used" )
	.zero	7
.LC14:
	ASCII(.string	"pFspInst = NULL" )
.LC24:
	ASCII(.string	"fsp is not inited" )
	.zero	6
.LC42:
	ASCII(.string	"%s : AllocFrame func null\n" )
	.zero	5
.LC44:
	ASCII(.string	"%s : RleaseFrame func null\n" )
	.zero	4
.LC57:
	ASCII(.string	"pstFspInstCfg = NULL" )
	.zero	3
.LC60:
	ASCII(.string	"This inst is not active" )
.LC67:
	ASCII(.string	"eChanCapLevel = %d is not expected!\n" )
	.zero	3
.LC69:
	ASCII(.string	"decode channel CapLevel is %d(w=%d,h=%d), this stream (w=%d,h=%d) can NOT be decoded!\n" )
	.zero	1
.LC80:
	ASCII(.string	"user set decode channel max resolution is (w=%d,h=%d), this stream (w=%d,h=%d) can NOT be decoded!\n" )
	.zero	4
.LC84:
	ASCII(.string	"DFS, Wrong Partition State. State: 0x%x\n" )
	.zero	7
.LC86:
	ASCII(.string	"DFS, Mem Element Fill Successed! Ready to Decode!\n" )
	.zero	5
.LC88:
	ASCII(.string	"Fsp arrange failed, return %d\n" )
	.zero	1
.LC90:
	ASCII(.string	"arrange ret %d, min = %d, actual = %d\n" )
	.zero	1
.LC95:
	ASCII(.string	"PhyAddr is NULL\n" )
	.zero	7
.LC97:
	ASCII(.string	"pFsNum is NULL\n" )
.LC108:
	ASCII(.string	"FSP_IsFsLeak: fsp is not inited \n" )
	.zero	6
.LC117:
	ASCII(.string	"no logic frame store!!!\n" )
	.zero	7
.LC120:
	ASCII(.string	"%s no decode phy fs\n" )
	.zero	3
.LC122:
	ASCII(.string	"no pmv memory\n" )
	.zero	1
.LC126:
	ASCII(.string	"FSP_AllocFrame failed\n" )
	.zero	1
.LC128:
	ASCII(.string	"invalid stFrameInfo PhyAddr\n" )
	.zero	3
.LC138:
	ASCII(.string	"LogicFrameID out of range" )
	.zero	6
.LC149:
	ASCII(.string	"fsp.c, line %d: pstLogicFs->PmvIdc out of range(=%d)\n" )
	.zero	2
.LC151:
	ASCII(.string	"FSP_GetRef: InstID out of range" )
.LC154:
	ASCII(.string	"FSP_GetRef: LogicFrameID out of range" )
	.zero	2
.LC157:
	ASCII(.string	"FSP_GetRef: Fsp is not inited" )
	.zero	2
.LC167:
	ASCII(.string	"FSP_SetDisplay: LogicTick=%d, DispTick=%d!\n" )
	.zero	4
.LC171:
	ASCII(.string	"pstVoQue is NULL" )
	.zero	7
.LC194:
	ASCII(.string	"DecFsAddrTab is NULL\n" )
	.zero	2
.LC196:
	ASCII(.string	"DecFsNum is NULL\n" )
	.zero	6
.LC202:
	ASCII(.string	"PmvAddrTab is NULL\n" )
	.zero	4
.LC204:
	ASCII(.string	"PmvNum is NULL\n" )
.LC220:
	ASCII(.string	"FSP CONTEXT:\n" )
	.zero	2
.LC222:
	ASCII(.string	"FspInstState" )
	.zero	3
.LC224:
	ASCII(.string	"%-35s :%d\n" )
	.zero	5
.LC226:
	ASCII(.string	"DecFsWidth" )
	.zero	5
.LC228:
	ASCII(.string	"DecFsHeight" )
	.zero	4
.LC230:
	ASCII(.string	"DispFsWidth" )
	.zero	4
.LC232:
	ASCII(.string	"DispFsHeight" )
	.zero	3
.LC234:
	ASCII(.string	"ExpectedDecFsNum" )
	.zero	7
.LC236:
	ASCII(.string	"ExpectedDispFsNum" )
	.zero	6
.LC238:
	ASCII(.string	"ExpectedPmvNum" )
	.zero	1
.LC240:
	ASCII(.string	"DecFsNum" )
	.zero	7
.LC242:
	ASCII(.string	"DispFsNum" )
	.zero	6
.LC244:
	ASCII(.string	"PmvNum" )
	.zero	1
.LC246:
	ASCII(.string	"logic fs: %d\n" )
	.zero	2
.LC248:
	ASCII(.string	"\n" )
	.zero	6
.LC250:
	ASCII(.string	"%d%d " )
	.zero	2
.LC252:
	ASCII(.string	"\ndec fs: %d\n" )
	.zero	3
.LC254:
	ASCII(.string	"\ndisp fs: %d\n" )
	.zero	2
.LC256:
	ASCII(.string	"\npmv: %d\n" )
	.zero	6
.LC258:
	ASCII(.string	"\n\n" )
	.zero	5
.LC260:
	ASCII(.string	"%d " )
	.zero	4
.LC281:
	ASCII(.string	"p_image NULL" )
	.zero	3
.LC284:
	ASCII(.string	"Flag Invalid" )
	.bss
	.align	3
.LANCHOR0 = . + 0
.LANCHOR1 = . + 4352
	.type	s_pFspInst, %object
	.size	s_pFspInst, 128
s_pFspInst:
	.zero	128
	.type	s_FspVoQue, %object
	.size	s_FspVoQue, 4224
s_FspVoQue:
	.zero	4224
	.type	g_IsFrameIntfValid, %object
	.size	g_IsFrameIntfValid, 4
g_IsFrameIntfValid:
	.zero	4
	.zero	4
	.type	g_FspFrameIntf, %object
	.size	g_FspFrameIntf, 32
g_FspFrameIntf:
	.zero	32
	.type	MemArrange.11693, %object
	.size	MemArrange.11693, 1272
MemArrange.11693:
	.zero	1272
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
