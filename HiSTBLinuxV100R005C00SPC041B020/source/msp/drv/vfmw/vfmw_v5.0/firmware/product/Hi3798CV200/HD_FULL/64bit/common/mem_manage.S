#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"mem_manage.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	MEM_InitMemManager
	.type	MEM_InitMemManager, %function
MEM_InitMemManager:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x0, x30
	bl	_mcount
	ldr	x19, .LC0
	mov	w0, 1
	ldr	x1, [x19, 24]
	blr	x1
	ldr	x3, [x19, 96]
	mov	x2, 16384
	ldr	x0, .LC1
	mov	w1, 0
	blr	x3
	ldr	x2, .LC2
	mov	w0, 1
	ldr	x1, [x19, 32]
	str	wzr, [x2, 3328]
	blr	x1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MEM_InitMemManager, .-MEM_InitMemManager
	.align	3
.LC0:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC1:
	.xword	.LANCHOR0
	.align	3
.LC2:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_ManagerWithOperation
	.type	MEM_ManagerWithOperation, %function
MEM_ManagerWithOperation:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC3
	ldp	x2, x1, [x19, 56]
	str	x2, [x0, 3336]
	str	x1, [x0, 3344]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MEM_ManagerWithOperation, .-MEM_ManagerWithOperation
	.align	3
.LC3:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_ManagerClearOperation
	.type	MEM_ManagerClearOperation, %function
MEM_ManagerClearOperation:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC4
	str	xzr, [x0, 3336]
	str	xzr, [x0, 3344]
	ldp	x29, x30, [sp], 16
	ret
	.size	MEM_ManagerClearOperation, .-MEM_ManagerClearOperation
	.align	3
.LC4:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_AddMemRecord
	.type	MEM_AddMemRecord, %function
MEM_AddMemRecord:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w0
	mov	x0, x30
	mov	x20, x1
	mov	w19, w2
	bl	_mcount
	cmp	x20, xzr
	ccmp	w19, wzr, 4, ne
	ccmp	w21, wzr, 4, ne
	bne	.L20
.L17:
	mov	w0, 0
.L16:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L20:
	ldr	x22, .LC7
	mov	w0, 1
	ldr	x1, [x22, 24]
	blr	x1
	ldr	x3, .LC8
	ldr	w0, [x3, 24]
	cbz	w0, .L10
	add	x4, x3, 56
	mov	w0, 1
	b	.L7
	.p2align 3
.L22:
	add	w0, w0, 1
	cmp	w0, 512
	beq	.L21
.L7:
	ldr	w5, [x4]
	add	x4, x4, 32
	cbnz	w5, .L22
.L6:
	sbfiz	x0, x0, 5, 32
	ldr	x1, [x22, 32]
	add	x3, x3, x0
	mov	w0, 1
	str	x20, [x3, 8]
	str	w21, [x3, 16]
	str	w19, [x3, 24]
	blr	x1
	b	.L17
	.p2align 3
.L21:
	ldr	x1, [x22, 32]
	mov	w0, 1
	blr	x1
	mov	w0, -1
	b	.L16
.L10:
	mov	w0, 0
	b	.L6
	.size	MEM_AddMemRecord, .-MEM_AddMemRecord
	.align	3
.LC7:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC8:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_DelMemRecord
	.type	MEM_DelMemRecord, %function
MEM_DelMemRecord:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	mov	w21, w2
	bl	_mcount
	cmp	x20, xzr
	mov	w0, 0
	ccmp	w21, wzr, 4, ne
	ccmp	w19, wzr, 4, ne
	bne	.L35
.L24:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L35:
	ldr	x22, .LC11
	mov	w0, 1
	ldr	x1, [x22, 24]
	blr	x1
	ldr	x1, .LC12
	mov	w3, 0
	add	x0, x1, 24
	b	.L26
	.p2align 3
.L25:
	add	w3, w3, 1
	add	x0, x0, 32
	cmp	w3, 512
	beq	.L36
.L26:
	ldr	w4, [x0]
	cbz	w4, .L25
	ldr	w5, [x0, -8]
	cmp	w19, w5
	bne	.L25
	ldr	x5, [x0, -16]
	cmp	x5, x20
	ccmp	w21, w4, 0, eq
	bne	.L25
	sbfiz	x3, x3, 5, 32
	ldr	x2, [x22, 32]
	add	x1, x1, x3
	mov	w0, 1
	str	xzr, [x1, 8]
	str	wzr, [x1, 16]
	str	wzr, [x1, 24]
	blr	x2
	mov	w0, 0
	b	.L24
	.p2align 3
.L36:
	ldr	x1, [x22, 32]
	mov	w0, 1
	blr	x1
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MEM_DelMemRecord, .-MEM_DelMemRecord
	.align	3
.LC11:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC12:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_Phy2Vir
	.type	MEM_Phy2Vir, %function
MEM_Phy2Vir:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x4, .LC13
	mov	w2, 0
	add	x0, x4, 24
	b	.L40
	.p2align 3
.L38:
	add	w2, w2, 1
	add	x0, x0, 32
	cmp	w2, 512
	beq	.L45
.L40:
	ldr	w1, [x0]
	cbz	w1, .L38
	ldr	w3, [x0, -8]
	cmp	w3, w19
	add	w1, w1, w3
	bhi	.L38
	cmp	w19, w1
	bcs	.L38
	ubfiz	x2, x2, 5, 32
	sub	w3, w19, w3
	add	x0, x4, x2
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	x0, [x0, 8]
	add	x0, x0, x3
	ret
.L45:
	mov	x0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MEM_Phy2Vir, .-MEM_Phy2Vir
	.align	3
.LC13:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_Vir2Phy
	.type	MEM_Vir2Phy, %function
MEM_Vir2Phy:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x4, .LC14
	mov	w2, 0
	add	x0, x4, 24
	b	.L49
	.p2align 3
.L47:
	add	w2, w2, 1
	add	x0, x0, 32
	cmp	w2, 512
	beq	.L54
.L49:
	ldr	w1, [x0]
	cbz	w1, .L47
	ldr	x3, [x0, -16]
	cmp	x3, x19
	add	x1, x3, x1, uxtw
	bhi	.L47
	cmp	x19, x1
	bcs	.L47
	ubfiz	x2, x2, 5, 32
	sub	x3, x19, x3
	add	x0, x4, x2
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w0, [x0, 16]
	add	w0, w0, w3
	ret
.L54:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MEM_Vir2Phy, .-MEM_Vir2Phy
	.align	3
.LC14:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_WritePhyWord
	.type	MEM_WritePhyWord, %function
MEM_WritePhyWord:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	x5, .LC15
	mov	w3, 0
	add	x0, x5, 24
	b	.L59
	.p2align 3
.L56:
	add	w3, w3, 1
	add	x0, x0, 32
	cmp	w3, 512
	beq	.L55
.L59:
	ldr	w2, [x0]
	cbz	w2, .L56
	ldr	w4, [x0, -8]
	cmp	w19, w4
	add	w2, w2, w4
	bcc	.L56
	cmp	w19, w2
	bcs	.L56
	ubfiz	x3, x3, 5, 32
	sub	w4, w19, w4
	add	x0, x5, x3
	ldr	x0, [x0, 8]
	cmn	x0, x4
	beq	.L55
	str	w20, [x0, x4]
.L55:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MEM_WritePhyWord, .-MEM_WritePhyWord
	.align	3
.LC15:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_ReadPhyWord
	.type	MEM_ReadPhyWord, %function
MEM_ReadPhyWord:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	ldr	x4, .LC16
	mov	w2, 0
	add	x0, x4, 24
	b	.L68
	.p2align 3
.L65:
	add	w2, w2, 1
	add	x0, x0, 32
	cmp	w2, 512
	beq	.L73
.L68:
	ldr	w1, [x0]
	cbz	w1, .L65
	ldr	w3, [x0, -8]
	cmp	w19, w3
	add	w1, w1, w3
	bcc	.L65
	cmp	w19, w1
	bcs	.L65
	ubfiz	x2, x2, 5, 32
	sub	w3, w19, w3
	add	x0, x4, x2
	ldr	x0, [x0, 8]
	cmn	x0, x3
	beq	.L73
	ldr	w0, [x0, x3]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L73:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MEM_ReadPhyWord, .-MEM_ReadPhyWord
	.align	3
.LC16:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_CopyPhy2Phy
	.type	MEM_CopyPhy2Phy, %function
MEM_CopyPhy2Phy:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	w21, w1
	mov	w19, w2
	bl	_mcount
	ldr	x1, .LC17
	mov	w4, 0
	add	x2, x1, 24
	mov	x0, x2
	b	.L77
	.p2align 3
.L75:
	add	w4, w4, 1
	add	x0, x0, 32
	cmp	w4, 512
	beq	.L90
.L77:
	ldr	w3, [x0]
	cbz	w3, .L75
	ldr	w5, [x0, -8]
	cmp	w20, w5
	add	w3, w3, w5
	bcc	.L75
	cmp	w20, w3
	bcs	.L75
	ubfiz	x0, x4, 5, 32
	sub	w5, w20, w5
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	add	x0, x0, x5
.L76:
	mov	w4, 0
	b	.L81
	.p2align 3
.L78:
	add	w4, w4, 1
	add	x2, x2, 32
	cmp	w4, 512
	beq	.L74
.L81:
	ldr	w3, [x2]
	cbz	w3, .L78
	ldr	w5, [x2, -8]
	cmp	w21, w5
	add	w3, w3, w5
	bcc	.L78
	cmp	w21, w3
	bcs	.L78
	ubfiz	x4, x4, 5, 32
	sub	w5, w21, w5
	add	x1, x1, x4
	cmp	x0, xzr
	ldr	x1, [x1, 8]
	add	x1, x1, x5
	ccmp	x1, xzr, 4, ne
	beq	.L74
	ldr	x3, .LC18
	uxtw	x2, w19
	ldr	x3, [x3, 104]
	blr	x3
.L74:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L90:
	mov	x0, 0
	b	.L76
	.size	MEM_CopyPhy2Phy, .-MEM_CopyPhy2Phy
	.align	3
.LC17:
	.xword	.LANCHOR0
	.align	3
.LC18:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_CopyPhy2Vir
	.type	MEM_CopyPhy2Vir, %function
MEM_CopyPhy2Vir:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	mov	w21, w2
	bl	_mcount
	ldr	x1, .LC19
	mov	w3, 0
	add	x0, x1, 24
	b	.L95
	.p2align 3
.L92:
	add	w3, w3, 1
	add	x0, x0, 32
	cmp	w3, 512
	beq	.L91
.L95:
	ldr	w2, [x0]
	cbz	w2, .L92
	ldr	w4, [x0, -8]
	cmp	w19, w4
	add	w2, w2, w4
	bcc	.L92
	cmp	w19, w2
	bcs	.L92
	ubfiz	x3, x3, 5, 32
	sub	w4, w19, w4
	add	x0, x1, x3
	cmp	x20, xzr
	ldr	x1, [x0, 8]
	add	x1, x1, x4
	ccmp	x1, xzr, 4, ne
	beq	.L91
	ldr	x3, .LC20
	uxtw	x2, w21
	mov	x0, x20
	ldr	x3, [x3, 104]
	blr	x3
.L91:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MEM_CopyPhy2Vir, .-MEM_CopyPhy2Vir
	.align	3
.LC19:
	.xword	.LANCHOR0
	.align	3
.LC20:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_CopyVir2Phy
	.type	MEM_CopyVir2Phy, %function
MEM_CopyVir2Phy:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x21, x1
	mov	w20, w2
	bl	_mcount
	ldr	x1, .LC21
	mov	w3, 0
	add	x0, x1, 24
	b	.L104
	.p2align 3
.L101:
	add	w3, w3, 1
	add	x0, x0, 32
	cmp	w3, 512
	beq	.L100
.L104:
	ldr	w2, [x0]
	cbz	w2, .L101
	ldr	w4, [x0, -8]
	cmp	w19, w4
	add	w2, w2, w4
	bcc	.L101
	cmp	w19, w2
	bcs	.L101
	ubfiz	x3, x3, 5, 32
	sub	w4, w19, w4
	add	x0, x1, x3
	ldr	x0, [x0, 8]
	add	x0, x0, x4
	cmp	x0, xzr
	ccmp	x21, xzr, 4, ne
	beq	.L100
	ldr	x3, .LC22
	uxtw	x2, w20
	mov	x1, x21
	ldr	x3, [x3, 104]
	blr	x3
.L100:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MEM_CopyVir2Phy, .-MEM_CopyVir2Phy
	.align	3
.LC21:
	.xword	.LANCHOR0
	.align	3
.LC22:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_AllocMemBlock
	.type	MEM_AllocMemBlock, %function
MEM_AllocMemBlock:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	mov	w21, w2
	mov	x19, x3
	mov	w23, w4
	bl	_mcount
	cmp	x19, xzr
	ccmp	w21, wzr, 4, ne
	beq	.L112
	ldr	x20, .LC23
	mov	x2, 32
	mov	w1, 0
	mov	x0, x19
	ldr	w24, [x19, 28]
	ldr	x3, [x20, 96]
	blr	x3
	ldr	x3, [x20, 96]
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 72
	blr	x3
	str	w24, [x29, 92]
	ldr	x5, [x20, 280]
	mov	w2, 4
	add	x4, x29, 72
	mov	w3, w23
	mov	w1, w21
	mov	x0, x22
	blr	x5
	ldr	x2, [x29, 80]
	cbz	x2, .L112
	ldr	w1, [x29, 92]
	mov	w0, -1
	ldp	w4, w3, [x29, 72]
	cmp	w1, 3
	bls	.L122
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L122:
	ldr	x0, .LC24
	ldrb	w0, [x0,w1,uxtw]
	adr	x1, .Lrtx114
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx114:
	.section	.rodata
	.align	0
	.align	2
.L114:
	.byte	(.L113 - .Lrtx114) / 4
	.byte	(.L115 - .Lrtx114) / 4
	.byte	(.L115 - .Lrtx114) / 4
	.byte	(.L116 - .Lrtx114) / 4
	.text
	.p2align 3
.L115:
	cmp	w4, wzr
	ccmp	w3, wzr, 4, ne
	ccmp	x2, xzr, 4, ne
	beq	.L112
.L117:
	ldr	x4, .LC25
	mov	w0, 0
	str	x2, [x19, 8]
	ldr	w1, [x29, 88]
	ldr	w2, [x4, 3328]
	str	w1, [x19, 24]
	add	w2, w2, 3
	str	w3, [x19, 16]
	add	w1, w2, w1
	and	w1, w1, -4
	str	w1, [x4, 3328]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L113:
	cmp	w4, wzr
	ccmp	x2, xzr, 4, ne
	beq	.L112
	mov	w3, w4
	b	.L117
	.p2align 3
.L116:
	cmp	w3, wzr
	ccmp	x2, xzr, 4, ne
	bne	.L117
.L112:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, -1
	ldp	x29, x30, [sp], 96
	ret
	.size	MEM_AllocMemBlock, .-MEM_AllocMemBlock
	.align	3
.LC23:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC24:
	.xword	.L114
	.align	3
.LC25:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_MapRegisterAddr
	.type	MEM_MapRegisterAddr, %function
MEM_MapRegisterAddr:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x2
	mov	w22, w1
	bl	_mcount
	cmp	x19, xzr
	ccmp	w20, wzr, 4, ne
	beq	.L126
	ldr	x21, .LC26
	mov	w1, 0
	mov	x2, 32
	mov	x0, x19
	ldr	x3, [x21, 96]
	blr	x3
	ldr	x1, [x21, 296]
	mov	w0, w20
	blr	x1
	cbz	x0, .L126
	str	x0, [x19, 8]
	mov	w0, 0
	str	w20, [x19, 16]
	str	w22, [x19, 24]
.L125:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L126:
	mov	w0, -1
	b	.L125
	.size	MEM_MapRegisterAddr, .-MEM_MapRegisterAddr
	.align	3
.LC26:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_UnmapRegisterAddr
	.type	MEM_UnmapRegisterAddr, %function
MEM_UnmapRegisterAddr:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cmp	w20, wzr
	ccmp	x19, xzr, 4, ne
	beq	.L130
	ldr	x1, .LC27
	mov	x0, x19
	ldr	x1, [x1, 304]
	blr	x1
.L130:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MEM_UnmapRegisterAddr, .-MEM_UnmapRegisterAddr
	.align	3
.LC27:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_ReleaseMemBlock
	.type	MEM_ReleaseMemBlock, %function
MEM_ReleaseMemBlock:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cmp	w19, wzr
	ccmp	x20, xzr, 4, ne
	bne	.L141
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L141:
	ldr	x21, .LC28
	mov	x2, 24
	mov	w1, 0
	add	x0, x29, 56
	ldr	x3, [x21, 96]
	blr	x3
	str	w19, [x29, 56]
	ldr	x0, .LC29
	str	x20, [x29, 64]
	add	x3, x0, 16384
	add	x0, x0, 24
	add	x3, x3, 24
	b	.L136
	.p2align 3
.L134:
	add	x0, x0, 32
	cmp	x0, x3
	beq	.L135
.L136:
	ldr	w1, [x0]
	cbz	w1, .L134
	ldr	w2, [x0, -8]
	cmp	w19, w2
	add	w4, w1, w2
	bcc	.L134
	cmp	w19, w4
	bcs	.L134
	str	w1, [x29, 72]
.L135:
	ldr	x1, [x21, 288]
	add	x0, x29, 56
	blr	x1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 80
	ret
	.size	MEM_ReleaseMemBlock, .-MEM_ReleaseMemBlock
	.align	3
.LC28:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC29:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_NsMapToSec
	.type	MEM_NsMapToSec, %function
MEM_NsMapToSec:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	MEM_NsMapToSec, .-MEM_NsMapToSec
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MEM_NsInSecUnMap
	.type	MEM_NsInSecUnMap, %function
MEM_NsInSecUnMap:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	MEM_NsInSecUnMap, .-MEM_NsInSecUnMap
	.global	mem_free
	.global	mem_malloc
	.global	s_MemRecord
	.bss
	.align	3
.LANCHOR0 = . + 0
.LANCHOR1 = . + 13056
	.type	s_MemRecord, %object
	.size	s_MemRecord, 16384
s_MemRecord:
	.zero	16384
	.type	s_MemOffset, %object
	.size	s_MemOffset, 4
s_MemOffset:
	.zero	4
	.zero	4
	.type	mem_malloc, %object
	.size	mem_malloc, 8
mem_malloc:
	.zero	8
	.type	mem_free, %object
	.size	mem_free, 8
mem_free:
	.zero	8
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
