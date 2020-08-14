#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"mvc.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	MVC_IsSTRefFlg
	.type	MVC_IsSTRefFlg, %function
MVC_IsSTRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19]
	ldr	x19, [sp, 16]
	and	w0, w0, 16776960
	cmp	w0, 65536
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_IsSTRefFlg, .-MVC_IsSTRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_IsLTRefFlg
	.type	MVC_IsLTRefFlg, %function
MVC_IsLTRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19]
	ldr	x19, [sp, 16]
	and	w0, w0, 16776960
	cmp	w0, 256
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_IsLTRefFlg, .-MVC_IsLTRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_pic_by_pic_num_desc
	.type	MVC_compare_pic_by_pic_num_desc, %function
MVC_compare_pic_by_pic_num_desc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, 1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 20]
	ldr	w2, [x2, 20]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	MVC_compare_pic_by_pic_num_desc, .-MVC_compare_pic_by_pic_num_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_pic_by_lt_pic_num_asc
	.type	MVC_compare_pic_by_lt_pic_num_asc, %function
MVC_compare_pic_by_lt_pic_num_asc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, -1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 16]
	ldr	w2, [x2, 16]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	MVC_compare_pic_by_lt_pic_num_asc, .-MVC_compare_pic_by_lt_pic_num_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_fs_by_frame_num_desc
	.type	MVC_compare_fs_by_frame_num_desc, %function
MVC_compare_fs_by_frame_num_desc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, 1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 24]
	ldr	w2, [x2, 24]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	MVC_compare_fs_by_frame_num_desc, .-MVC_compare_fs_by_frame_num_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_fs_by_lt_pic_idx_asc
	.type	MVC_compare_fs_by_lt_pic_idx_asc, %function
MVC_compare_fs_by_lt_pic_idx_asc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, -1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 28]
	ldr	w2, [x2, 28]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, hi
	csel	w0, w0, w3, cs
	ret
	.size	MVC_compare_fs_by_lt_pic_idx_asc, .-MVC_compare_fs_by_lt_pic_idx_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_pic_by_poc_asc
	.type	MVC_compare_pic_by_poc_asc, %function
MVC_compare_pic_by_poc_asc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, -1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 24]
	ldr	w2, [x2, 24]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	MVC_compare_pic_by_poc_asc, .-MVC_compare_pic_by_poc_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_pic_by_poc_desc
	.type	MVC_compare_pic_by_poc_desc, %function
MVC_compare_pic_by_poc_desc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, 1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 24]
	ldr	w2, [x2, 24]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	MVC_compare_pic_by_poc_desc, .-MVC_compare_pic_by_poc_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_fs_by_poc_asc
	.type	MVC_compare_fs_by_poc_asc, %function
MVC_compare_fs_by_poc_asc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, -1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 32]
	ldr	w2, [x2, 32]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	MVC_compare_fs_by_poc_asc, .-MVC_compare_fs_by_poc_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_compare_fs_by_poc_desc
	.type	MVC_compare_fs_by_poc_desc, %function
MVC_compare_fs_by_poc_desc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19]
	mov	w3, 1
	ldr	x2, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	w1, [x0, 32]
	ldr	w2, [x2, 32]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	MVC_compare_fs_by_poc_desc, .-MVC_compare_fs_by_poc_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	MVC_SetFrmRepeatCount.part.1, %function
MVC_SetFrmRepeatCount.part.1:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	add	x3, x19, 11124736
	ldp	w0, w2, [x19, 132]
	ldr	w3, [x3, 760]
	lsr	w0, w0, 1
	add	w2, w3, w2, lsr 1
	add	w3, w2, 1
	cmp	w0, w3
	bcc	.L28
	sub	w0, w0, #1
	sub	w0, w0, w2
	cmp	w0, 1
	bls	.L29
	mov	w0, 2
	str	w0, [x20, 24]
	ldr	w0, [x19, 136]
	add	w0, w0, 4
	str	w0, [x19, 136]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L28:
	str	wzr, [x20, 24]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L29:
	mov	w0, 1
	str	w0, [x20, 24]
	ldr	w0, [x19, 136]
	add	w0, w0, 2
	str	w0, [x19, 136]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_SetFrmRepeatCount.part.1, .-MVC_SetFrmRepeatCount.part.1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	MVC_CombinePacket.part.12, %function
MVC_CombinePacket.part.12:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC1
	mov	w0, 7
	bl	dprint_vfmw
	ldr	x2, [x19, 240]
	mov	w0, 7
	ldr	x1, .LC3
	ldp	w5, w3, [x2, 48]
	ldr	w4, [x2, 16]
	ldr	w2, [x2, 20]
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	ldr	x3, .LC4
	ldr	w2, [x1, 16]
	ldr	x0, [x1, 40]
	ldr	x3, [x3, 104]
	ldr	x1, [x1, 8]
	sub	x0, x0, x2
	blr	x3
	ldr	x2, [x19, 240]
	mov	w4, 1
	ldr	x1, .LC6
	mov	w0, 7
	ldr	w3, [x2, 68]
	ldr	w20, [x2, 36]
	str	w3, [x2, 36]
	ldr	x2, [x19, 240]
	str	wzr, [x2, 28]
	ldr	x2, [x19, 240]
	ldr	w5, [x2, 16]
	ldr	x3, [x2, 40]
	sub	x3, x3, x5
	str	x3, [x2, 8]
	ldr	x2, [x19, 240]
	ldr	w5, [x2, 16]
	ldr	w3, [x2, 52]
	sub	w3, w3, w5
	str	w3, [x2, 20]
	ldr	x2, [x19, 240]
	ldr	w5, [x2, 16]
	ldr	w3, [x2, 48]
	add	w3, w5, w3
	str	w3, [x2, 16]
	ldr	x2, [x19, 240]
	str	w4, [x2, 76]
	ldr	x2, [x19, 240]
	ldp	w3, w2, [x2, 16]
	bl	dprint_vfmw
	ldr	w0, [x19, 120]
	mov	w1, w20
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 240]
	str	xzr, [x0, 40]
	ldr	x0, [x19, 240]
	str	wzr, [x0, 60]
	ldr	x0, [x19, 240]
	str	wzr, [x0, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_CombinePacket.part.12, .-MVC_CombinePacket.part.12
	.align	3
.LC1:
	.xword	.LC0
	.align	3
.LC3:
	.xword	.LC2
	.align	3
.LC4:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC6:
	.xword	.LC5
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_ue_v
	.type	mvc_ue_v, %function
mvc_ue_v:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x1
	add	x23, x21, 872
	bl	_mcount
	mov	w1, 32
	mov	x0, x23
	bl	BsShow
	mov	w20, w0
	bl	ZerosMS_32
	cmp	w0, 15
	mov	w19, w0
	bls	.L37
	cmp	w0, 31
	bls	.L38
	mov	w0, 1
	mov	w20, -1
	strb	w0, [x21, 10]
	mov	w19, 32
.L35:
	ldr	x4, [x21, 240]
	mov	w3, w20
	ldr	x1, .LC8
	mov	x2, x22
	mov	w0, 21
	ldr	w5, [x4, 72]
	add	w19, w5, w19
	str	w19, [x4, 72]
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L37:
	lsl	w19, w0, 1
	mov	w2, 32
	add	w19, w19, 1
	mov	x0, x23
	sub	w2, w2, w19
	mov	w1, w19
	lsr	w20, w20, w2
	sub	w20, w20, #1
	bl	BsSkip
	b	.L35
	.p2align 3
.L38:
	mov	w1, w0
	mov	x0, x23
	add	w24, w19, 1
	lsl	w19, w19, 1
	bl	BsSkip
	add	w19, w19, 1
	mov	w1, w24
	mov	x0, x23
	bl	BsShow
	sub	w20, w0, #1
	mov	w1, w24
	mov	x0, x23
	bl	BsSkip
	b	.L35
	.size	mvc_ue_v, .-mvc_ue_v
	.align	3
.LC8:
	.xword	.LC7
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_se_v
	.type	mvc_se_v, %function
mvc_se_v:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x1
	add	x23, x21, 872
	bl	_mcount
	mov	w1, 32
	mov	x0, x23
	bl	BsShow
	mov	w20, w0
	bl	ZerosMS_32
	cmp	w0, 15
	mov	w19, w0
	bhi	.L40
	lsl	w19, w0, 1
	mov	w2, 32
	add	w19, w19, 1
	mov	x0, x23
	sub	w2, w2, w19
	mov	w1, w19
	lsr	w20, w20, w2
	and	w2, w20, 1
	lsr	w20, w20, 1
	cmp	w2, wzr
	csneg	w20, w20, w20, eq
	bl	BsSkip
.L42:
	ldr	x4, [x21, 240]
	mov	w3, w20
	ldr	x1, .LC9
	mov	x2, x22
	mov	w0, 21
	ldr	w5, [x4, 72]
	add	w19, w5, w19
	str	w19, [x4, 72]
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L40:
	cmp	w0, 31
	bls	.L49
	mov	w0, 1
	mov	w20, 2147483647
	strb	w0, [x21, 10]
	mov	w19, 32
	b	.L42
	.p2align 3
.L49:
	mov	w1, w0
	mov	x0, x23
	bl	BsSkip
	mov	w1, w19
	mov	x0, x23
	lsl	w19, w19, 1
	bl	BsGet
	mov	w20, w0
	mov	w1, 1
	mov	x0, x23
	add	w19, w19, 1
	bl	BsGet
	and	w0, w0, 1
	cmp	w0, wzr
	csneg	w20, w20, w20, eq
	b	.L42
	.size	mvc_se_v, .-mvc_se_v
	.align	3
.LC9:
	.xword	.LC7
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_u_v
	.type	mvc_u_v, %function
mvc_u_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x2
	mov	w19, w1
	bl	_mcount
	mov	w1, w19
	add	x0, x21, 872
	bl	BsGet
	mov	w20, w0
	ldr	x4, [x21, 240]
	mov	w3, w0
	ldr	x1, .LC10
	mov	x2, x22
	mov	w0, 21
	ldr	w5, [x4, 72]
	add	w19, w5, w19
	str	w19, [x4, 72]
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	mvc_u_v, .-mvc_u_v
	.align	3
.LC10:
	.xword	.LC7
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_u_1
	.type	mvc_u_1, %function
mvc_u_1:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	mov	w1, 1
	add	x0, x20, 872
	bl	BsGet
	mov	w19, w0
	ldr	x5, [x20, 240]
	mov	w3, w0
	ldr	x1, .LC11
	mov	x2, x21
	mov	w0, 21
	ldr	w4, [x5, 72]
	add	w4, w4, 1
	str	w4, [x5, 72]
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	mvc_u_1, .-mvc_u_1
	.align	3
.LC11:
	.xword	.LC7
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetMinPOC
	.type	MVC_GetMinPOC, %function
MVC_GetMinPOC:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x24, x3
	mov	x21, x0
	mov	x0, x30
	mov	x23, x2
	mov	w22, w1
	add	x26, x21, 11124736
	bl	_mcount
	mov	w0, -1
	str	w0, [x24]
	mov	w0, 2147483647
	str	w0, [x23]
	ldr	w0, [x26, 760]
	ldr	w1, [x26, 764]
	cmp	w1, w0
	bls	.L53
	str	w0, [x26, 764]
.L53:
	mov	w25, -1
	mov	w20, 0
	cbnz	w0, .L68
	b	.L59
	.p2align 3
.L56:
	ldr	w2, [x1, 32]
	ldr	w3, [x23]
	cmp	w3, w2
	ble	.L55
	ldrb	w3, [x1, 5]
	cbz	w3, .L55
	ldrb	w0, [x0, 1]
	sub	w0, w0, #1
	uxtb	w0, w0
	cmp	w0, 1
	bls	.L75
	.p2align 2
.L55:
	ldr	w0, [x26, 760]
	add	w20, w20, 1
	cmp	w0, w20
	bls	.L59
.L68:
	add	x19, x21, x20, uxtw 3
	add	x19, x19, 11124736
	ldr	x0, [x19, 376]
	cbz	x0, .L55
	ldrsb	w1, [x0, 6]
	ldr	w0, [x21, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L55
	cmn	w22, #1
	ldr	x1, [x19, 376]
	bne	.L56
	ldrb	w2, [x1, 5]
	cbz	w2, .L55
	ldrb	w0, [x0, 1]
	sub	w0, w0, #1
	uxtb	w0, w0
	cmp	w0, 1
	bhi	.L55
	ldr	w0, [x1, 32]
	ldr	w2, [x23]
	cmp	w2, w0
	ble	.L57
	str	w0, [x23]
	str	w20, [x24]
	add	w20, w20, 1
	ldr	x0, [x19, 376]
	ldr	w25, [x0, 56]
	ldr	w0, [x26, 760]
	cmp	w0, w20
	bhi	.L68
.L59:
	ldr	w0, [x24]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x25, x26, [sp, 64]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L75:
	ldr	w0, [x1, 56]
	cmp	w22, w0
	bne	.L55
	str	w2, [x23]
	str	w20, [x24]
	b	.L55
	.p2align 3
.L57:
	bne	.L55
	ldr	w0, [x1, 56]
	cmp	w0, w25
	bge	.L55
	str	w20, [x24]
	ldr	x0, [x19, 376]
	ldr	w25, [x0, 56]
	b	.L55
	.size	MVC_GetMinPOC, .-MVC_GetMinPOC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FrameStoreRefFlg
	.type	MVC_FrameStoreRefFlg, %function
MVC_FrameStoreRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 3]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_FrameStoreRefFlg, .-MVC_FrameStoreRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_NonLongTermRefFlg
	.type	MVC_NonLongTermRefFlg, %function
MVC_NonLongTermRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w1, [x19, 3]
	cmp	w1, 3
	beq	.L96
	tbnz	x1, 0, .L97
	mov	w0, 0
	tbnz	x1, 1, .L82
.L89:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L97:
	ldrb	w2, [x19, 745]
	mov	w0, 1
	cbz	w2, .L89
	mov	w0, 0
	tbz	x1, 1, .L89
	b	.L82
	.p2align 3
.L96:
	ldrb	w1, [x19, 697]
	mov	w0, 1
	cbz	w1, .L89
	ldrb	w1, [x19, 745]
	cbz	w1, .L89
.L82:
	ldrb	w0, [x19, 793]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_NonLongTermRefFlg, .-MVC_NonLongTermRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ShortTermRefFlg
	.type	MVC_ShortTermRefFlg, %function
MVC_ShortTermRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w1, [x19, 3]
	cmp	w1, 3
	beq	.L114
	tbnz	x1, 0, .L115
.L102:
	mov	w0, 0
	tbnz	x1, 1, .L103
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L115:
	ldr	w0, [x19, 744]
	and	w0, w0, 16776960
	cmp	w0, 65536
	bne	.L102
.L107:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L114:
	ldr	w0, [x19, 696]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L107
	ldr	w0, [x19, 744]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L107
.L103:
	ldr	w0, [x19, 792]
	ldr	x19, [sp, 16]
	and	w0, w0, 16776960
	cmp	w0, 65536
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_ShortTermRefFlg, .-MVC_ShortTermRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_LongTermRefFlg
	.type	MVC_LongTermRefFlg, %function
MVC_LongTermRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w1, [x19, 3]
	cmp	w1, 3
	beq	.L132
	tbnz	x1, 0, .L133
.L120:
	mov	w0, 0
	tbnz	x1, 1, .L121
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L133:
	ldr	w0, [x19, 744]
	and	w0, w0, 16776960
	cmp	w0, 256
	bne	.L120
.L125:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L132:
	ldr	w0, [x19, 696]
	and	w0, w0, 16776960
	cmp	w0, 256
	beq	.L125
	ldr	w0, [x19, 744]
	and	w0, w0, 16776960
	cmp	w0, 256
	beq	.L125
.L121:
	ldr	w0, [x19, 792]
	ldr	x19, [sp, 16]
	and	w0, w0, 16776960
	cmp	w0, 256
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_LongTermRefFlg, .-MVC_LongTermRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UpdateLTReflist
	.type	MVC_UpdateLTReflist, %function
MVC_UpdateLTReflist:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x23, x0
	mov	x0, x30
	add	x26, x23, 11124736
	bl	_mcount
	ldr	w24, [x26, 760]
	cbz	w24, .L135
	sub	w22, w24, #1
	add	x20, x26, 376
	add	x22, x22, 1388544
	mov	w19, 0
	add	x22, x22, 2096
	add	x25, x23, 11157504
	add	x22, x23, x22, lsl 3
	b	.L137
	.p2align 3
.L136:
	cmp	x22, x20
	beq	.L151
.L137:
	ldr	x21, [x20]
	add	x20, x20, 8
	mov	x0, x21
	cbz	x21, .L136
	bl	MVC_LongTermRefFlg
	cbz	w0, .L136
	ldr	w1, [x21, 56]
	ldr	w0, [x25, 2688]
	cmp	w1, w0
	bne	.L136
	uxtw	x0, w19
	cmp	x22, x20
	add	x0, x0, 1388544
	add	w19, w19, 1
	add	x0, x23, x0, lsl 3
	str	x21, [x0, 17016]
	bne	.L137
	.p2align 2
.L151:
	cmp	w19, w24
	str	w19, [x26, 772]
	bcs	.L134
	.p2align 2
.L146:
	uxtw	x0, w19
	add	w19, w19, 1
	add	x0, x0, 1388544
	cmp	w19, w24
	add	x0, x23, x0, lsl 3
	str	xzr, [x0, 17016]
	bne	.L146
.L134:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L135:
	str	wzr, [x26, 772]
	b	.L134
	.size	MVC_UpdateLTReflist, .-MVC_UpdateLTReflist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UpdateReflist
	.type	MVC_UpdateReflist, %function
MVC_UpdateReflist:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x23, x0
	mov	x0, x30
	add	x26, x23, 11124736
	bl	_mcount
	ldr	w24, [x26, 760]
	cbz	w24, .L153
	sub	w22, w24, #1
	add	x20, x26, 376
	add	x22, x22, 1388544
	mov	w19, 0
	add	x22, x22, 2096
	add	x25, x23, 11157504
	add	x22, x23, x22, lsl 3
	b	.L155
	.p2align 3
.L154:
	cmp	x22, x20
	beq	.L169
.L155:
	ldr	x21, [x20]
	add	x20, x20, 8
	mov	x0, x21
	cbz	x21, .L154
	bl	MVC_ShortTermRefFlg
	cbz	w0, .L154
	ldr	w1, [x21, 56]
	ldr	w0, [x25, 2688]
	cmp	w1, w0
	bne	.L154
	uxtw	x0, w19
	cmp	x22, x20
	add	x0, x0, 1388544
	add	w19, w19, 1
	add	x0, x23, x0, lsl 3
	str	x21, [x0, 16888]
	bne	.L155
	.p2align 2
.L169:
	cmp	w19, w24
	str	w19, [x26, 768]
	bcs	.L152
	.p2align 2
.L164:
	uxtw	x0, w19
	add	w19, w19, 1
	add	x0, x0, 1388544
	cmp	w19, w24
	add	x0, x23, x0, lsl 3
	str	xzr, [x0, 16888]
	bne	.L164
.L152:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L153:
	str	wzr, [x26, 768]
	b	.L152
	.size	MVC_UpdateReflist, .-MVC_UpdateReflist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetPicNumX
	.type	MVC_GetPicNumX, %function
MVC_GetPicNumX:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldrb	w0, [x20, 3]
	ldr	w2, [x20, 656]
	cbz	w0, .L172
	lsl	w2, w2, 1
	add	w2, w2, 1
.L172:
	mvn	w0, w19
	add	w0, w0, w2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_GetPicNumX, .-MVC_GetPicNumX
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UnMarkFrameStoreRef
	.type	MVC_UnMarkFrameStoreRef, %function
MVC_UnMarkFrameStoreRef:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 2]
	tbz	x0, 0, .L174
	ldrb	w1, [x19, 3]
	strb	wzr, [x19, 745]
	and	w1, w1, 2
	strb	wzr, [x19, 746]
	strb	w1, [x19, 3]
.L174:
	tbz	x0, 1, .L175
	ldrb	w1, [x19, 3]
	strb	wzr, [x19, 793]
	and	w1, w1, 1
	strb	wzr, [x19, 794]
	strb	w1, [x19, 3]
.L175:
	cmp	w0, 3
	bne	.L176
	strb	wzr, [x19, 698]
	strb	wzr, [x19, 697]
.L176:
	strb	wzr, [x19, 3]
	mov	w2, 0
	ldrsb	w1, [x19, 6]
	ldr	w0, [x20, 120]
	bl	FSP_SetRef
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_UnMarkFrameStoreRef, .-MVC_UnMarkFrameStoreRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	MVC_SlidingWinMark.part.2, %function
MVC_SlidingWinMark.part.2:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 11124736
	ldr	w5, [x1, 760]
	cbz	w5, .L200
	add	x1, x1, 376
	mov	w7, 0
	mov	w6, 2147483647
	mov	w2, 0
	add	x8, x19, 11157504
	.p2align 2
.L190:
	ldr	x0, [x1]
	cbz	x0, .L185
	ldr	w3, [x0, 64]
	cmp	w3, w6
	bcs	.L185
	ldrb	w4, [x0, 3]
	cbz	w4, .L185
	cmp	w4, 3
	beq	.L256
	tbnz	x4, 0, .L257
.L189:
	tbnz	x4, 1, .L198
	.p2align 2
.L185:
	add	w2, w2, 1
	add	x1, x1, 8
	cmp	w2, w5
	bne	.L190
.L184:
	sxtw	x0, w7
	add	x0, x0, 1388544
	add	x0, x19, x0, lsl 3
	ldr	x1, [x0, 16760]
	cbz	x1, .L183
	ldrb	w0, [x1, 3]
	cbz	w0, .L183
	cmp	w0, 3
	beq	.L258
	tbnz	x0, 0, .L259
.L195:
	tbnz	x0, 1, .L199
.L183:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L257:
	ldrb	w9, [x0, 745]
	cbnz	w9, .L189
.L187:
	ldr	w4, [x0, 56]
	ldr	w0, [x8, 2688]
	cmp	w4, w0
	csel	w6, w6, w3, ne
	csel	w7, w7, w2, ne
	b	.L185
	.p2align 3
.L256:
	ldrb	w4, [x0, 697]
	cbz	w4, .L187
	ldrb	w4, [x0, 745]
	cbz	w4, .L187
.L198:
	ldrb	w4, [x0, 793]
	cbnz	w4, .L185
	ldr	w4, [x0, 56]
	ldr	w0, [x8, 2688]
	cmp	w4, w0
	csel	w6, w6, w3, ne
	csel	w7, w7, w2, ne
	b	.L185
	.p2align 3
.L259:
	ldrb	w2, [x1, 745]
	cbnz	w2, .L195
.L193:
	add	x0, x19, 11157504
	ldr	w2, [x1, 56]
	ldr	w0, [x0, 2688]
	cmp	w2, w0
	bne	.L183
	mov	x0, x19
	bl	MVC_UnMarkFrameStoreRef
	mov	x0, x19
	bl	MVC_UpdateReflist
	b	.L183
	.p2align 3
.L258:
	ldrb	w0, [x1, 697]
	cbz	w0, .L193
	ldrb	w0, [x1, 745]
	cbz	w0, .L193
.L199:
	ldrb	w0, [x1, 793]
	cbz	w0, .L193
	b	.L183
.L200:
	mov	w7, 0
	b	.L184
	.size	MVC_SlidingWinMark.part.2, .-MVC_SlidingWinMark.part.2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UnMarkLTFrmByFrmIdx
	.type	MVC_UnMarkLTFrmByFrmIdx, %function
MVC_UnMarkLTFrmByFrmIdx:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	x0, x20, 11124736
	ldr	w2, [x0, 772]
	cbz	w2, .L260
	sub	w4, w2, #1
	add	x0, x0, 632
	add	x4, x4, 1388544
	add	x5, x20, 11157504
	add	x4, x4, 2128
	add	x4, x20, x4, lsl 3
	b	.L263
	.p2align 3
.L262:
	cmp	x0, x4
	beq	.L260
.L263:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldr	w3, [x1, 28]
	cmp	w3, w19
	bne	.L262
	ldr	w3, [x1, 56]
	ldr	w2, [x5, 2688]
	cmp	w3, w2
	bne	.L262
	mov	x0, x20
	bl	MVC_UnMarkFrameStoreRef
.L260:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_UnMarkLTFrmByFrmIdx, .-MVC_UnMarkLTFrmByFrmIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UnMarkLTFldByFrmIdx
	.type	MVC_UnMarkLTFldByFrmIdx, %function
MVC_UnMarkLTFldByFrmIdx:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x1
	mov	w20, w2
	mov	w19, w3
	bl	_mcount
	add	x0, x21, 11124736
	ldr	w4, [x0, 772]
	cbz	w4, .L268
	sub	w6, w4, #1
	add	x0, x0, 632
	add	x6, x6, 1388544
	add	x7, x21, 11157504
	add	x6, x6, 2128
	add	x6, x21, x6, lsl 3
	b	.L272
	.p2align 3
.L270:
	cmp	x0, x6
	beq	.L268
.L272:
	ldr	x4, [x0]
	add	x0, x0, 8
	ldr	w5, [x4, 28]
	cmp	w5, w19
	bne	.L270
	ldr	w5, [x4, 56]
	ldr	w3, [x7, 2688]
	cmp	w5, w3
	bne	.L270
	cmp	w20, 1
	beq	.L284
	cmp	w20, 2
	bne	.L270
	ldrb	w1, [x4, 745]
	cmp	w1, wzr
	ccmp	x4, x22, 0, ne
	beq	.L270
.L283:
	mov	x1, x4
	mov	x0, x21
	bl	MVC_UnMarkFrameStoreRef
.L268:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L284:
	ldrb	w1, [x4, 793]
	cmp	w1, wzr
	ccmp	x4, x22, 0, ne
	beq	.L270
	b	.L283
	.size	MVC_UnMarkLTFldByFrmIdx, .-MVC_UnMarkLTFldByFrmIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UnMarkSTRef
	.type	MVC_UnMarkSTRef, %function
MVC_UnMarkSTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	mov	w21, w2
	bl	_mcount
	ldrb	w0, [x20, 3]
	cbnz	w0, .L286
	add	x0, x19, 11124736
	ldr	w3, [x20, 656]
	sub	w3, w3, #1
	ldr	w1, [x0, 768]
	sub	w2, w3, w21
	cbz	w1, .L285
	sub	w4, w1, #1
	add	x0, x0, 504
	add	x4, x4, 1388544
	add	x5, x19, 11157504
	add	x4, x4, 2112
	add	x4, x19, x4, lsl 3
	b	.L292
	.p2align 3
.L291:
	cmp	x0, x4
	beq	.L285
.L292:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldr	w3, [x1, 716]
	cmp	w3, w2
	bne	.L291
	ldrb	w3, [x1, 3]
	cmp	w3, 3
	bne	.L291
	ldr	w3, [x1, 696]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L291
	ldr	w6, [x1, 56]
	ldr	w3, [x5, 2688]
	cmp	w6, w3
	bne	.L291
	mov	x0, x19
	bl	MVC_UnMarkFrameStoreRef
.L285:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L286:
	add	x1, x19, 11124736
	ldr	w0, [x20, 656]
	add	x4, x1, 504
	mov	w3, 0
	lsl	w0, w0, 1
	add	x8, x19, 11157504
	ldr	w6, [x1, 768]
	sub	w2, w0, w21
	cbz	w6, .L285
	.p2align 2
.L306:
	ldr	x1, [x4]
	add	x4, x4, 8
	ldrb	w0, [x1, 3]
	and	w7, w0, 1
	tbz	x0, 0, .L293
	ldr	w5, [x1, 744]
	and	w5, w5, 16776960
	cmp	w5, 65536
	beq	.L313
.L293:
	tbz	x0, 1, .L295
	ldr	w0, [x1, 792]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L314
.L295:
	add	w3, w3, 1
	cmp	w3, w6
	bne	.L306
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L313:
	ldr	w9, [x1, 56]
	ldr	w5, [x8, 2688]
	cmp	w9, w5
	bne	.L293
	ldr	w5, [x1, 764]
	cmp	w5, w2
	bne	.L293
	add	x2, x19, x3, uxtw 3
	and	w0, w0, 2
	add	x2, x2, 11124736
	strb	w0, [x1, 3]
	ldr	x0, [x2, 504]
	strb	wzr, [x0, 746]
	ldr	x0, [x2, 504]
	ldrb	w1, [x0, 696]
	cmp	w1, 3
	ccmp	w1, wzr, 4, ne
	bne	.L294
	strb	wzr, [x0, 698]
	ldr	x0, [x2, 504]
	strb	wzr, [x0, 697]
	ldr	x0, [x2, 504]
.L294:
	ldrb	w1, [x0, 3]
	cbnz	w1, .L285
	ldr	w1, [x0, 48]
	mov	w2, 0
	add	x1, x19, x1, lsl 2
	str	wzr, [x1, 148]
	ldrsb	w1, [x0, 6]
	ldr	w0, [x19, 120]
	bl	FSP_SetRef
	b	.L285
	.p2align 3
.L314:
	ldr	w5, [x1, 56]
	ldr	w0, [x8, 2688]
	cmp	w5, w0
	bne	.L295
	ldr	w0, [x1, 812]
	cmp	w0, w2
	bne	.L295
	add	x0, x19, x3, uxtw 3
	strb	w7, [x1, 3]
	add	x0, x0, 11124736
	ldr	x1, [x0, 504]
	strb	wzr, [x1, 794]
	ldr	x1, [x0, 504]
	ldrb	w2, [x1, 696]
	cmp	w2, 3
	ccmp	w2, wzr, 4, ne
	bne	.L296
	strb	wzr, [x1, 698]
	ldr	x1, [x0, 504]
	strb	wzr, [x1, 697]
	ldr	x1, [x0, 504]
.L296:
	ldrb	w0, [x1, 3]
	cbnz	w0, .L285
	ldr	w0, [x1, 48]
	mov	w2, 0
	add	x0, x19, x0, lsl 2
	str	wzr, [x0, 148]
	ldrsb	w1, [x1, 6]
	ldr	w0, [x19, 120]
	bl	FSP_SetRef
	b	.L285
	.size	MVC_UnMarkSTRef, .-MVC_UnMarkSTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UnMarkLTRef
	.type	MVC_UnMarkLTRef, %function
MVC_UnMarkLTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x1
	mov	x19, x0
	mov	x0, x30
	mov	w20, w2
	bl	_mcount
	ldrb	w0, [x21, 3]
	cbz	w0, .L316
	add	x0, x19, 11124736
	mov	w3, 0
	add	x4, x0, 632
	add	x2, x19, 11157504
	ldr	w6, [x0, 772]
	cbz	w6, .L315
	.p2align 2
.L336:
	ldr	x1, [x4]
	add	x4, x4, 8
	ldrb	w0, [x1, 3]
	and	w7, w0, 1
	tbz	x0, 0, .L321
	ldr	w5, [x1, 744]
	and	w5, w5, 16776960
	cmp	w5, 256
	beq	.L343
.L321:
	tbz	x0, 1, .L323
	ldr	w0, [x1, 792]
	and	w0, w0, 16776960
	cmp	w0, 256
	beq	.L344
.L323:
	add	w3, w3, 1
	cmp	w3, w6
	bne	.L336
.L315:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L343:
	ldr	w8, [x1, 56]
	ldr	w5, [x2, 2688]
	cmp	w8, w5
	bne	.L321
	ldr	w5, [x1, 760]
	cmp	w20, w5
	bne	.L321
	add	x2, x19, x3, uxtw 3
	and	w0, w0, 2
	add	x2, x2, 11124736
	strb	w0, [x1, 3]
	ldr	x0, [x2, 632]
	strb	wzr, [x0, 745]
	ldr	x0, [x2, 632]
	ldrb	w1, [x0, 696]
	cmp	w1, 3
	ccmp	w1, wzr, 4, ne
	bne	.L322
	strb	wzr, [x0, 698]
	ldr	x0, [x2, 632]
	strb	wzr, [x0, 697]
	ldr	x0, [x2, 632]
.L322:
	ldrb	w1, [x0, 3]
	cbnz	w1, .L315
	ldr	w1, [x0, 48]
	mov	w2, 0
	add	x1, x19, x1, lsl 2
	str	wzr, [x1, 148]
	ldrsb	w1, [x0, 6]
	ldr	w0, [x19, 120]
	bl	FSP_SetRef
	b	.L315
	.p2align 3
.L344:
	ldr	w5, [x1, 56]
	ldr	w0, [x2, 2688]
	cmp	w5, w0
	bne	.L323
	ldr	w0, [x1, 808]
	cmp	w20, w0
	bne	.L323
	add	x0, x19, x3, uxtw 3
	strb	w7, [x1, 3]
	add	x0, x0, 11124736
	ldr	x1, [x0, 632]
	strb	wzr, [x1, 793]
	ldr	x1, [x0, 632]
	ldrb	w2, [x1, 696]
	cmp	w2, 3
	ccmp	w2, wzr, 4, ne
	bne	.L324
	strb	wzr, [x1, 698]
	ldr	x1, [x0, 632]
	strb	wzr, [x1, 697]
	ldr	x1, [x0, 632]
.L324:
	ldrb	w0, [x1, 3]
	cbnz	w0, .L315
	ldr	w0, [x1, 48]
	mov	w2, 0
	add	x0, x19, x0, lsl 2
	str	wzr, [x0, 148]
	ldrsb	w1, [x1, 6]
	ldr	w0, [x19, 120]
	bl	FSP_SetRef
	b	.L315
	.p2align 3
.L316:
	add	x0, x19, 11124736
	ldr	w1, [x0, 772]
	cbz	w1, .L315
	sub	w4, w1, #1
	add	x0, x0, 632
	add	x4, x4, 1388544
	add	x5, x19, 11157504
	add	x4, x4, 2128
	add	x4, x19, x4, lsl 3
	b	.L327
	.p2align 3
.L326:
	cmp	x0, x4
	beq	.L315
.L327:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldr	w3, [x1, 712]
	cmp	w3, w20
	bne	.L326
	ldrb	w2, [x1, 3]
	cmp	w2, 3
	bne	.L326
	ldr	w2, [x1, 696]
	and	w2, w2, 16776960
	cmp	w2, 256
	bne	.L326
	ldr	w3, [x1, 56]
	ldr	w2, [x5, 2688]
	cmp	w3, w2
	bne	.L326
	mov	x0, x19
	bl	MVC_UnMarkFrameStoreRef
	b	.L315
	.size	MVC_UnMarkLTRef, .-MVC_UnMarkLTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_MarkPicLTRef
	.type	MVC_MarkPicLTRef, %function
MVC_MarkPicLTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x3
	mov	x21, x0
	mov	x0, x30
	mov	w20, w2
	mov	w19, w1
	bl	_mcount
	ldrb	w2, [x22, 3]
	add	x0, x21, 11124736
	cbz	w2, .L346
	ldr	w7, [x0, 768]
	mov	w3, 0
	add	x4, x0, 504
	add	x8, x21, 11157504
	cbz	w7, .L345
	.p2align 2
.L367:
	ldr	x0, [x4]
	ldrb	w5, [x0, 3]
	tbz	x5, 0, .L352
	ldr	w6, [x0, 744]
	and	w6, w6, 16776960
	cmp	w6, 65536
	beq	.L373
.L352:
	tbz	x5, 1, .L355
	ldr	w1, [x0, 792]
	and	w1, w1, 16776960
	cmp	w1, 65536
	beq	.L374
.L355:
	add	w3, w3, 1
	add	x4, x4, 8
	cmp	w3, w7
	bne	.L367
.L345:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L373:
	ldr	w1, [x0, 764]
	cmp	w20, w1
	bne	.L352
	ldr	w6, [x0, 56]
	ldr	w1, [x8, 2688]
	cmp	w6, w1
	bne	.L352
	ldrb	w1, [x0, 793]
	cmp	w1, 1
	bne	.L356
	ldr	w1, [x0, 28]
	cmp	w19, w1
	bne	.L355
.L356:
	add	x3, x21, x3, uxtw 3
	str	w19, [x0, 28]
	add	x0, x3, 11124736
	cmp	w2, 1
	lsl	w1, w19, 1
	mov	w2, 1
	csinc	w1, w1, w1, ne
	ldr	x3, [x0, 504]
	str	w1, [x3, 760]
	ldr	x1, [x0, 504]
	strb	w2, [x1, 745]
	ldr	x1, [x0, 504]
	strb	wzr, [x1, 746]
	ldr	x1, [x0, 504]
	ldrb	w2, [x1, 696]
	cmp	w2, 3
	ccmp	w2, wzr, 4, ne
	bne	.L345
	ldrb	w3, [x1, 745]
	ldrb	w2, [x1, 793]
	cmp	w2, w3
	bne	.L345
	strb	w2, [x1, 697]
	ldr	x0, [x0, 504]
	ldrb	w1, [x0, 746]
	strb	w1, [x0, 698]
	b	.L345
	.p2align 3
.L374:
	ldr	w1, [x0, 812]
	cmp	w1, w20
	bne	.L355
	ldr	w5, [x0, 56]
	ldr	w1, [x8, 2688]
	cmp	w5, w1
	bne	.L355
	ldrb	w1, [x0, 745]
	cmp	w1, 1
	bne	.L359
	ldr	w1, [x0, 28]
	cmp	w1, w19
	bne	.L355
.L359:
	add	x3, x21, x3, uxtw 3
	str	w19, [x0, 28]
	add	x0, x3, 11124736
	cmp	w2, 2
	lsl	w1, w19, 1
	mov	w2, 1
	csinc	w1, w1, w1, ne
	ldr	x3, [x0, 504]
	str	w1, [x3, 808]
	ldr	x1, [x0, 504]
	strb	w2, [x1, 793]
	ldr	x1, [x0, 504]
	strb	wzr, [x1, 794]
	ldr	x1, [x0, 504]
	ldrb	w2, [x1, 696]
	cmp	w2, 3
	ccmp	w2, wzr, 4, ne
	bne	.L345
	ldrb	w3, [x1, 745]
	ldrb	w2, [x1, 793]
	cmp	w2, w3
	bne	.L345
	strb	w2, [x1, 697]
	ldr	x0, [x0, 504]
	ldrb	w1, [x0, 794]
	strb	w1, [x0, 698]
	b	.L345
	.p2align 3
.L346:
	ldr	w6, [x0, 768]
	cbz	w6, .L345
	add	x0, x0, 504
	mov	w1, 0
	add	x3, x21, 11157504
	b	.L351
	.p2align 3
.L350:
	add	w1, w1, 1
	cmp	w1, w6
	beq	.L345
.L351:
	ldr	x4, [x0]
	add	x0, x0, 8
	ldrb	w5, [x4, 3]
	cmp	w5, 3
	bne	.L350
	ldr	w5, [x4, 696]
	and	w5, w5, 16776960
	cmp	w5, 65536
	bne	.L350
	ldr	w2, [x4, 716]
	cmp	w20, w2
	bne	.L350
	ldr	w2, [x3, 2688]
	ldr	w5, [x4, 56]
	cmp	w5, w2
	bne	.L350
	add	x0, x21, x1, uxtw 3
	str	w19, [x4, 28]
	add	x0, x0, 11124736
	mov	w1, 1
	ldr	x2, [x0, 504]
	str	w19, [x2, 712]
	ldr	x2, [x0, 504]
	strb	w1, [x2, 697]
	ldr	x2, [x0, 504]
	strb	w1, [x2, 745]
	ldr	x2, [x0, 504]
	strb	w1, [x2, 793]
	ldr	x1, [x0, 504]
	strb	wzr, [x1, 698]
	ldr	x1, [x0, 504]
	strb	wzr, [x1, 746]
	ldr	x0, [x0, 504]
	strb	wzr, [x0, 794]
	b	.L345
	.size	MVC_MarkPicLTRef, .-MVC_MarkPicLTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_MarkSTToLTRef
	.type	MVC_MarkSTToLTRef, %function
MVC_MarkSTToLTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x1
	mov	x21, x0
	mov	x0, x30
	mov	w20, w2
	mov	w19, w3
	bl	_mcount
	ldrb	w0, [x22, 3]
	cbnz	w0, .L376
	ldr	w2, [x22, 656]
	mov	w1, w19
	mov	x0, x21
	sub	w2, w2, #1
	sub	w20, w2, w20
	bl	MVC_UnMarkLTFrmByFrmIdx
.L377:
	mov	x3, x22
	mov	w2, w20
	mov	w1, w19
	mov	x0, x21
	bl	MVC_MarkPicLTRef
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L376:
	add	x0, x21, 11124736
	ldr	w3, [x22, 656]
	lsl	w3, w3, 1
	ldr	w1, [x0, 768]
	sub	w20, w3, w20
	cbz	w1, .L377
	sub	w5, w1, #1
	add	x0, x0, 504
	add	x5, x5, 1388544
	add	x6, x21, 11157504
	add	x5, x5, 2112
	add	x5, x21, x5, lsl 3
	.p2align 2
.L380:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w3, [x1, 3]
	tbz	x3, 0, .L378
	ldr	w4, [x1, 744]
	and	w4, w4, 16776960
	cmp	w4, 65536
	beq	.L391
.L378:
	tbz	x3, 1, .L379
	ldr	w3, [x1, 792]
	and	w3, w3, 16776960
	cmp	w3, 65536
	beq	.L392
.L379:
	cmp	x0, x5
	bne	.L380
	b	.L377
	.p2align 3
.L391:
	ldr	w2, [x1, 764]
	cmp	w2, w20
	bne	.L378
	ldr	w4, [x1, 56]
	ldr	w2, [x6, 2688]
	cmp	w4, w2
	bne	.L378
	mov	w3, w19
	mov	w2, 1
	mov	x0, x21
	bl	MVC_UnMarkLTFldByFrmIdx
	b	.L377
	.p2align 3
.L392:
	ldr	w2, [x1, 812]
	cmp	w2, w20
	bne	.L379
	ldr	w3, [x1, 56]
	ldr	w2, [x6, 2688]
	cmp	w3, w2
	bne	.L379
	mov	w3, w19
	mov	w2, 2
	mov	x0, x21
	bl	MVC_UnMarkLTFldByFrmIdx
	b	.L377
	.size	MVC_MarkSTToLTRef, .-MVC_MarkSTToLTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UpdateMaxLTFrmIdx
	.type	MVC_UpdateMaxLTFrmIdx, %function
MVC_UpdateMaxLTFrmIdx:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	add	x21, x22, 11124736
	mov	w20, w1
	bl	_mcount
	ldr	w0, [x21, 772]
	str	w20, [x21, 776]
	cbz	w0, .L393
	mov	w2, w20
	mov	w19, 0
	add	x20, x22, 11157504
	b	.L395
	.p2align 3
.L401:
	ldr	w2, [x21, 776]
.L395:
	uxtw	x0, w19
	add	w19, w19, 1
	add	x0, x0, 1388544
	add	x0, x22, x0, lsl 3
	ldr	x1, [x0, 17016]
	ldr	w0, [x1, 28]
	cmp	w0, w2
	bcc	.L396
	ldr	w2, [x1, 56]
	ldr	w0, [x20, 2688]
	cmp	w2, w0
	beq	.L400
.L396:
	ldr	w0, [x21, 772]
	cmp	w0, w19
	bhi	.L401
.L393:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L400:
	mov	x0, x22
	bl	MVC_UnMarkFrameStoreRef
	b	.L396
	.size	MVC_UpdateMaxLTFrmIdx, .-MVC_UpdateMaxLTFrmIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UnMarkAllSTRef
	.type	MVC_UnMarkAllSTRef, %function
MVC_UnMarkAllSTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 11124736
	mov	w19, 0
	bl	_mcount
	add	x22, x20, 11157504
	ldr	w0, [x21, 768]
	cbnz	w0, .L407
	b	.L402
	.p2align 3
.L404:
	ldr	w0, [x21, 768]
	cmp	w0, w19
	bls	.L402
.L407:
	uxtw	x0, w19
	ldr	w2, [x22, 2688]
	add	x0, x0, 1388544
	add	w19, w19, 1
	add	x0, x20, x0, lsl 3
	ldr	x1, [x0, 16888]
	ldr	w0, [x1, 56]
	cmp	w0, w2
	bne	.L404
	mov	x0, x20
	bl	MVC_UnMarkFrameStoreRef
	ldr	w0, [x21, 768]
	cmp	w0, w19
	bhi	.L407
.L402:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MVC_UnMarkAllSTRef, .-MVC_UnMarkAllSTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_MarkCurrPicLT
	.type	MVC_MarkCurrPicLT, %function
MVC_MarkCurrPicLT:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x1
	mov	x21, x0
	mov	x0, x30
	mov	w20, w2
	bl	_mcount
	ldrb	w2, [x19, 3]
	cbz	w2, .L412
	ldr	x1, [x19, 640]
	mov	w3, w20
	mov	x0, x21
	bl	MVC_UnMarkLTFldByFrmIdx
	strb	wzr, [x19, 5]
	str	w20, [x19, 660]
	mov	w0, 1
	strb	w0, [x19, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L412:
	mov	w1, w20
	mov	x0, x21
	bl	MVC_UnMarkLTFrmByFrmIdx
	strb	wzr, [x19, 5]
	str	w20, [x19, 660]
	mov	w0, 1
	strb	w0, [x19, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MVC_MarkCurrPicLT, .-MVC_MarkCurrPicLT
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_RemoveFrameStoreOutDPB
	.type	MVC_RemoveFrameStoreOutDPB, %function
MVC_RemoveFrameStoreOutDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w19, w1
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, x19, uxtw 3
	add	x19, x19, 11124736
	bl	_mcount
	ldr	x1, [x19, 376]
	cbz	x1, .L413
	ldr	w0, [x1, 52]
	add	x21, x20, 11124736
	add	x0, x20, x0
	add	x0, x0, 11124736
	strb	wzr, [x0, 784]
	ldr	w0, [x21, 996]
	cmp	w0, wzr
	cset	w2, ne
	sub	w0, w0, w2
	str	w0, [x21, 996]
	ldrsb	w1, [x1, 6]
	ldr	w0, [x20, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L416
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	beq	.L417
	ldr	x1, [x19, 376]
	mov	w2, 0
	ldr	w0, [x20, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetDisplay
.L417:
	ldr	x1, [x19, 376]
	mov	w2, 0
	ldr	w0, [x20, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetRef
.L416:
	ldr	x0, [x19, 376]
	strb	wzr, [x0, 2]
	ldr	x0, [x19, 376]
	strb	wzr, [x0, 5]
	ldr	x0, [x19, 376]
	strb	wzr, [x0, 3]
	str	xzr, [x19, 376]
	ldr	w0, [x21, 764]
	sub	w0, w0, #1
	str	w0, [x21, 764]
.L413:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MVC_RemoveFrameStoreOutDPB, .-MVC_RemoveFrameStoreOutDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_RemoveUnUsedFrameStore
	.type	MVC_RemoveUnUsedFrameStore, %function
MVC_RemoveUnUsedFrameStore:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	mov	w22, w1
	mov	w19, 0
	bl	_mcount
	add	x0, x23, 11124736
	add	x20, x0, 376
	ldr	w21, [x0, 760]
	cbz	w21, .L445
	.p2align 2
.L440:
	ldr	x2, [x20]
	add	x20, x20, 8
	cbz	x2, .L431
	ldrb	w0, [x2, 3]
	cbnz	w0, .L431
	ldrb	w0, [x2, 5]
	cbnz	w0, .L431
	ldr	w0, [x2, 56]
	cmp	w0, w22
	ccmn	w22, #1, 4, ne
	beq	.L446
	.p2align 2
.L431:
	add	w19, w19, 1
	cmp	w21, w19
	bne	.L440
.L445:
	mov	w0, 0
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L446:
	ldr	w3, [x2, 16]
	mov	w1, w19
	mov	x0, x23
	cmp	w3, 1
	beq	.L431
	strb	wzr, [x2, 2]
	bl	MVC_RemoveFrameStoreOutDPB
	b	.L431
	.size	MVC_RemoveUnUsedFrameStore, .-MVC_RemoveUnUsedFrameStore
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_CheckFrameStore
	.type	MVC_CheckFrameStore, %function
MVC_CheckFrameStore:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldrb	w0, [x20, 6]
	cbnz	w0, .L448
	ldrsb	w0, [x19, 8]
	cmp	w0, 1
	bne	.L460
	strb	w0, [x20, 6]
.L448:
	ldrsb	w1, [x19, 6]
	ldr	w0, [x20, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L450
	ldrsb	w3, [x0, 1]
	cmp	w3, 3
	ccmp	w3, wzr, 4, ne
	beq	.L469
	ldrb	w2, [x19, 2]
	ldrb	w3, [x19, 1]
	cbz	w2, .L459
	cmp	w3, 1
	beq	.L459
	mov	x1, 16711680
	ldr	x4, [x19]
	movk	x1, 0xff, lsl 32
	mov	x3, 196608
	movk	x3, 0x3, lsl 32
	and	x1, x4, x1
	cmp	x1, x3
	beq	.L470
	cmp	w2, 1
	beq	.L471
	cmp	w2, 2
	beq	.L472
.L456:
	ldr	x1, [x20, 224]
	ldr	w2, [x0, 168]
	mov	w0, 0
	ldr	w3, [x1, 4]
	cmp	w2, w3
	bhi	.L473
.L466:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L469:
	ldrsb	w2, [x19, 6]
	mov	w0, 1
	ldr	x1, .LC15
	bl	dprint_vfmw
.L452:
	ldr	w0, [x19, 16]
	cmp	w0, 1
	beq	.L457
	mov	w0, -3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L471:
	ldrb	w3, [x19, 4]
	tbz	x3, 0, .L456
	ldr	x1, .LC23
	mov	w0, w2
	bl	dprint_vfmw
	mov	w0, -1
	b	.L466
	.p2align 3
.L472:
	ldrb	w3, [x19, 4]
	tbz	x3, 1, .L456
	ldr	x1, .LC25
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L466
	.p2align 3
.L457:
	mov	w1, 2
	mov	w0, -3
	str	w1, [x19, 16]
	b	.L466
	.p2align 3
.L460:
	mov	w0, -1
	b	.L466
	.p2align 3
.L450:
	ldrsb	w4, [x19, 6]
	mov	x2, x19
	ldr	x1, .LC17
	mov	w0, 1
	ldr	w3, [x19, 16]
	bl	dprint_vfmw
	b	.L452
	.p2align 3
.L459:
	ldr	x1, .LC19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L466
	.p2align 3
.L473:
	ldr	x1, .LC27
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L466
.L470:
	ldrb	w3, [x19, 4]
	mov	w0, 1
	ldr	x1, .LC21
	bl	dprint_vfmw
	mov	w0, -1
	b	.L466
	.size	MVC_CheckFrameStore, .-MVC_CheckFrameStore
	.align	3
.LC15:
	.xword	.LC14
	.align	3
.LC17:
	.xword	.LC16
	.align	3
.LC19:
	.xword	.LC18
	.align	3
.LC21:
	.xword	.LC20
	.align	3
.LC23:
	.xword	.LC22
	.align	3
.LC25:
	.xword	.LC24
	.align	3
.LC27:
	.xword	.LC26
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ExchangePts
	.type	MVC_ExchangePts, %function
MVC_ExchangePts:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x24, x1
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x24, 672]
	cmn	x0, #1
	beq	.L474
	add	x25, x21, 11124736
	ldr	w0, [x25, 760]
	cbz	w0, .L474
	mov	w23, -1
	mov	w20, 0
	mov	x22, -2
	.p2align 2
.L479:
	add	x19, x21, x20, sxtw 3
	add	x19, x19, 11124736
	ldr	x0, [x19, 376]
	cbz	x0, .L478
	ldrsb	w1, [x0, 6]
	ldr	w0, [x21, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L478
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	ccmp	w0, wzr, 4, ne
	beq	.L478
	ldr	x0, [x19, 376]
	ldr	x0, [x0, 672]
	cmp	x0, x22
	bcs	.L478
	mov	w23, w20
	mov	x22, x0
	.p2align 2
.L478:
	ldr	w0, [x25, 760]
	add	w20, w20, 1
	cmp	w0, w20
	bhi	.L479
	cmn	w23, #1
	beq	.L474
	ldr	x2, [x24, 672]
	cmp	x22, x2
	bcc	.L493
.L474:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L493:
	ldr	x1, .LC29
	mov	x3, x22
	mov	w0, 29
	bl	dprint_vfmw
	sxtw	x0, w23
	ldr	x1, [x24, 672]
	add	x0, x0, 1388544
	add	x21, x21, x0, lsl 3
	ldr	x0, [x21, 16760]
	str	x1, [x0, 672]
	str	x22, [x24, 672]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	MVC_ExchangePts, .-MVC_ExchangePts
	.align	3
.LC29:
	.xword	.LC28
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetImagePara
	.type	MVC_GetImagePara, %function
MVC_GetImagePara:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	add	x5, x20, 11116544
	mov	w0, 2240
	ldp	x7, x4, [x20, 272]
	ldr	w3, [x19, 180]
	mov	w2, 64764
	ldr	w1, [x5, 2824]
	movk	w2, 0xfffd, lsl 16
	and	w2, w3, w2
	mov	w8, 3992
	mov	w3, 262144
	ldrb	w6, [x19, 2]
	umull	x0, w1, w0
	add	x0, x4, x0
	ldr	w1, [x0, 28]
	str	w2, [x19, 180]
	ldr	x4, [x20, 224]
	umull	x0, w1, w8
	ldr	w4, [x4, 592]
	add	x1, x7, x0
	cmp	w4, 1
	add	x4, x20, 11124736
	csel	w0, wzr, w3, ne
	cmp	w6, 3
	orr	w2, w0, w2
	str	w2, [x19, 180]
	ldr	w0, [x20, 56]
	lsl	w0, w0, 10
	str	w0, [x19, 208]
	bhi	.L496
	ldr	x0, .LC30
	ldrb	w0, [x0,w6,uxtw]
	adr	x3, .Lrtx498
	add	x0, x3, w0, sxtb #2
	br	x0
.Lrtx498:
	.section	.rodata
	.align	0
	.align	2
.L498:
	.byte	(.L497 - .Lrtx498) / 4
	.byte	(.L499 - .Lrtx498) / 4
	.byte	(.L500 - .Lrtx498) / 4
	.byte	(.L497 - .Lrtx498) / 4
	.text
	.p2align 3
.L497:
	ldrb	w0, [x19, 696]
	cbnz	w0, .L501
	ldp	w7, w4, [x19, 40]
	mov	w0, 3840
	mov	w3, 3584
	cmp	w7, wzr
	ldrb	w8, [x19]
	csel	w0, w0, w3, ne
	str	w4, [x19, 232]
.L502:
	ldr	w4, [x19, 768]
	ldr	w3, [x19, 816]
	cmp	w4, w3
	beq	.L540
	cset	w7, le
.L512:
	add	x4, x20, 11124736
	ldrb	w3, [x4, 364]
	cmp	w3, 3
	beq	.L526
	cmp	w3, 4
	lsl	w7, w7, 12
	mov	w3, 0
	csel	w7, w7, w3, ne
.L513:
	and	w3, w8, 3
	and	w2, w2, -12289
	orr	w3, w3, w7
	orr	w2, w2, w0
	orr	w2, w3, w2
	str	w2, [x19, 180]
.L496:
	ldrb	w0, [x1, 20]
	mov	w2, 0
	str	w0, [x19, 548]
	str	w0, [x19, 552]
	ldrb	w0, [x1, 24]
	cbz	w0, .L514
	ldr	w2, [x1, 68]
.L514:
	str	w2, [x19, 560]
	mov	w0, 1
	str	w0, [x19, 556]
	ldrb	w0, [x5, 2819]
	cmp	w0, 1
	csel	w0, w0, wzr, eq
	str	w0, [x19, 272]
	ldr	x0, [x20, 224]
	ldr	w0, [x0, 12]
	cmp	w0, wzr
	ble	.L517
	ldrb	w0, [x19, 696]
	cbz	w0, .L541
.L517:
	ldrsb	w0, [x4, 328]
	cbz	w0, .L518
	ldr	w0, [x4, 344]
	cmp	w0, 3
	beq	.L542
	cmp	w0, 4
	beq	.L543
	cmp	w0, 5
	beq	.L544
	str	wzr, [x19, 428]
	mov	w0, 0
	ldr	x1, .LC32
	ldr	w2, [x4, 344]
	bl	dprint_vfmw
	.p2align 2
.L518:
	ldr	w0, [x20, 120]
	ldrsb	w1, [x19, 6]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L494
	ldr	x3, .LC33
	mov	x2, 624
	add	x1, x19, 72
	ldr	x3, [x3, 104]
	blr	x3
	mov	w0, 15
	str	w0, [x20, 376]
	mov	w2, 4
	mov	w1, 1
	ldrb	w0, [x19, 696]
	strb	w1, [x20, 371]
	cmp	w0, wzr
	csel	w0, w0, w2, eq
	strb	w0, [x20, 364]
	ldr	w0, [x19, 732]
	str	w0, [x20, 380]
	strb	w1, [x20, 369]
	str	w0, [x20, 384]
	str	w0, [x20, 388]
.L494:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L541:
	cmp	w6, 3
	bne	.L517
	ldr	w0, [x19, 180]
	and	w0, w0, -769
	str	w0, [x19, 180]
	b	.L517
	.p2align 3
.L499:
	ldrb	w0, [x19]
	add	x4, x20, 11124736
	ldr	w3, [x19, 44]
	and	w0, w0, 3
	str	w3, [x19, 232]
	orr	w0, w0, 1792
	orr	w2, w0, w2
	str	w2, [x19, 180]
	b	.L496
	.p2align 3
.L500:
	ldrb	w0, [x19]
	mov	w3, 2816
	ldr	w7, [x19, 44]
	add	x4, x20, 11124736
	and	w0, w0, 3
	str	w7, [x19, 232]
	orr	w0, w0, w3
	orr	w2, w0, w2
	str	w2, [x19, 180]
	b	.L496
	.p2align 3
.L542:
	mov	w0, 1
	str	w0, [x19, 428]
	b	.L518
	.p2align 3
.L544:
	mov	w0, 3
	str	w0, [x19, 428]
	b	.L518
	.p2align 3
.L501:
	ldr	w4, [x19, 776]
	ldr	w0, [x20, 116]
	ldr	w10, [x19, 824]
	cmp	w4, w0
	bhi	.L545
	cmp	w0, w10
	bcc	.L506
.L504:
	ldr	w0, [x19, 44]
	ldrb	w8, [x19]
	ldrb	w7, [x19, 795]
	ldrb	w9, [x19, 747]
	str	w0, [x19, 232]
	mov	w0, 3
.L507:
	ldrb	w3, [x19, 4]
	cmp	w3, 1
	beq	.L546
	cmp	w3, 2
	beq	.L510
	lsl	w0, w0, 10
	orr	w0, w0, 768
	stp	w9, w7, [x19, 172]
	b	.L502
	.p2align 3
.L506:
	ldrb	w8, [x19, 747]
	mov	w0, 1
	ldrb	w7, [x19, 795]
	mov	w9, w8
	str	w4, [x19, 232]
	b	.L507
	.p2align 3
.L543:
	mov	w0, 2
	str	w0, [x19, 428]
	b	.L518
	.p2align 3
.L526:
	mov	w7, 4096
	b	.L513
	.p2align 3
.L540:
	mov	x3, 720
	ldr	x4, [x19, 184]
	movk	x3, 0x1e0, lsl 32
	cmp	x4, x3
	cset	w7, ne
	b	.L512
	.p2align 3
.L545:
	cmp	w0, w10
	bcc	.L504
	ldrb	w8, [x19, 795]
	mov	w0, 2
	ldrb	w9, [x19, 747]
	mov	w7, w8
	str	w10, [x19, 232]
	b	.L507
	.p2align 3
.L546:
	mov	w8, w7
	mov	w0, 2816
	str	w10, [x19, 232]
	stp	w9, w7, [x19, 172]
	b	.L502
	.p2align 3
.L510:
	mov	w8, w9
	mov	w0, 1792
	str	w4, [x19, 232]
	stp	w9, w7, [x19, 172]
	b	.L502
	.size	MVC_GetImagePara, .-MVC_GetImagePara
	.align	3
.LC30:
	.xword	.L498
	.align	3
.LC32:
	.xword	.LC31
	.align	3
.LC33:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_SetFrmRepeatCount
	.type	MVC_SetFrmRepeatCount, %function
MVC_SetFrmRepeatCount:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bls	.L548
	mov	x1, x20
	mov	x0, x19
	bl	MVC_SetFrmRepeatCount.part.1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L548:
	str	wzr, [x20, 24]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_SetFrmRepeatCount, .-MVC_SetFrmRepeatCount
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_SplitFrmToFlds
	.type	MVC_SplitFrmToFlds, %function
MVC_SplitFrmToFlds:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w4, [x19, 697]
	mov	w6, 1
	ldrb	w3, [x19, 698]
	mov	w5, 2
	ldrb	w1, [x19, 699]
	ldr	w2, [x19, 728]
	ldr	x0, [x19, 704]
	strb	w4, [x19, 745]
	strb	w3, [x19, 746]
	strb	w6, [x19, 744]
	strb	w1, [x19, 747]
	strb	w4, [x19, 793]
	strb	w3, [x19, 794]
	strb	w5, [x19, 792]
	strb	w1, [x19, 795]
	str	x0, [x19, 752]
	str	w2, [x19, 776]
	str	x0, [x19, 800]
	str	w2, [x19, 824]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_SplitFrmToFlds, .-MVC_SplitFrmToFlds
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_CombineFldsToFrm
	.type	MVC_CombineFldsToFrm, %function
MVC_CombineFldsToFrm:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 768]
	mov	w5, 3
	ldr	w3, [x19, 776]
	ldr	w2, [x19, 816]
	ldr	w0, [x19, 824]
	cmp	w1, w2
	ldr	x4, [x19, 752]
	add	w0, w3, w0
	ldrb	w3, [x19, 793]
	csel	w1, w1, w2, le
	lsr	w0, w0, 1
	strb	w5, [x19, 696]
	str	w1, [x19, 32]
	str	x4, [x19, 704]
	str	w1, [x19, 720]
	str	w0, [x19, 728]
	cbz	w3, .L552
	ldrb	w1, [x19, 745]
	cbz	w1, .L552
	mov	w1, 1
	strb	w1, [x19, 697]
.L554:
	mov	w3, 0
.L555:
	ldrb	w2, [x19, 795]
	ldrb	w1, [x19, 747]
	str	w0, [x19, 44]
	cmp	w1, w2, uxtb
	strb	w3, [x19, 698]
	csel	w0, w1, w2, cs
	strb	w0, [x19, 699]
	strb	w0, [x19]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L552:
	ldr	w1, [x19, 792]
	strb	wzr, [x19, 697]
	tst	w1, 16776960
	beq	.L554
	ldr	w1, [x19, 744]
	mov	w3, 1
	tst	w1, 16776960
	bne	.L555
	b	.L554
	.size	MVC_CombineFldsToFrm, .-MVC_CombineFldsToFrm
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetAPC
	.type	MVC_GetAPC, %function
MVC_GetAPC:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x21, x1
	mov	x0, x30
	mov	w19, w2
	bl	_mcount
	cbz	x21, .L588
	cmp	w19, 1
	beq	.L568
	cmp	w19, 2
	beq	.L569
	ldr	w23, [x21, 768]
	ldr	w24, [x21, 816]
.L570:
	add	x20, x22, 11124736
	ldr	w2, [x20, 992]
	cbz	w2, .L579
	ldrb	w0, [x20, 784]
	cbz	w0, .L589
	add	x0, x20, 785
	mov	w19, 0
	b	.L573
	.p2align 3
.L577:
	ldrb	w1, [x0], 1
	cbz	w1, .L571
.L573:
	add	w19, w19, 1
	cmp	w19, w2
	bne	.L577
.L579:
	mov	w0, -1
.L583:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L569:
	ldr	w23, [x21, 816]
	mov	w24, w23
	b	.L570
.L589:
	mov	w19, 0
	.p2align 2
.L571:
	ldrsb	w1, [x21, 6]
	ldr	w0, [x22, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L574
	ldr	x1, [x0, 640]
	cbz	x1, .L574
	uxtw	x1, w19
	mov	w2, 1
	add	x3, x22, x1
	mov	w4, 16
	add	x3, x3, 11124736
	add	x1, x22, x1, lsl 2
	add	x1, x1, 11124736
	strb	w2, [x3, 784]
	mov	w3, w19
	ldr	x0, [x0, 640]
	ldr	w0, [x0, 8]
	str	w0, [x1, 800]
	mov	w0, 13
	str	w23, [x1, 864]
	str	w24, [x1, 928]
	ldr	x1, .LC41
	ldr	w2, [x20, 996]
	cmp	w2, 15
	csinc	w2, w4, w2, hi
	str	w2, [x20, 996]
	ldr	w2, [x21, 228]
	bl	dprint_vfmw
	str	w19, [x21, 52]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L568:
	ldr	w23, [x21, 768]
	mov	w24, w23
	b	.L570
.L574:
	ldrsb	w2, [x21, 6]
	mov	x4, x0
	ldrb	w3, [x21, 3]
	mov	x5, 0
	ldr	x1, .LC43
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L583
.L588:
	ldr	x2, .LC37
	mov	w3, 2085
	ldr	x1, .LC39
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L583
	.size	MVC_GetAPC, .-MVC_GetAPC
	.align	3
.LC37:
	.xword	.LC36
	.align	3
.LC39:
	.xword	.LC38
	.align	3
.LC41:
	.xword	.LC40
	.align	3
.LC43:
	.xword	.LC42
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_SlidingWinMark
	.type	MVC_SlidingWinMark, %function
MVC_SlidingWinMark:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 11124736
	ldr	x0, [x19, 248]
	ldr	w2, [x1, 772]
	ldr	w0, [x0, 3944]
	ldr	w1, [x1, 768]
	sub	w0, w0, w2
	cmp	w1, w0
	bne	.L590
	mov	x0, x19
	bl	MVC_SlidingWinMark.part.2
.L590:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_SlidingWinMark, .-MVC_SlidingWinMark
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DumpDPB
	.type	MVC_DumpDPB, %function
MVC_DumpDPB:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC44
	ldr	w0, [x0]
	tbz	x0, 14, .L592
	add	x22, x21, 11124736
	ldr	w0, [x22, 764]
	cbz	w0, .L598
	ldr	x20, .LC48
	mov	w19, 0
	.p2align 2
.L597:
	uxtw	x3, w19
	mov	w2, w19
	add	x3, x3, 1388544
	mov	x1, x20
	mov	w0, 14
	add	w19, w19, 1
	add	x3, x21, x3, lsl 3
	ldr	x3, [x3, 16760]
	ldrb	w5, [x3, 3]
	ldr	w4, [x3, 32]
	ldr	w3, [x3, 20]
	bl	dprint_vfmw
	ldr	w0, [x22, 764]
	cmp	w0, w19
	bhi	.L597
.L598:
	ldr	x25, .LC46
	mov	w0, 14
	mov	x1, x25
	bl	dprint_vfmw
	ldr	w0, [x22, 768]
	cbz	w0, .L596
	ldr	x24, .LC50
	mov	w19, 0
	ldr	x23, .LC52
	.p2align 2
.L599:
	add	x20, x21, x19, uxtw 3
	mov	w2, w19
	add	x20, x20, 11124736
	mov	x1, x24
	mov	w0, 14
	ldr	x3, [x20, 504]
	ldrb	w5, [x3, 3]
	ldr	w4, [x3, 32]
	ldr	w3, [x3, 20]
	bl	dprint_vfmw
	ldr	x3, [x20, 504]
	mov	w2, w19
	mov	x1, x23
	mov	w0, 14
	add	w19, w19, 1
	ldr	w3, [x3, 720]
	bl	dprint_vfmw
	ldr	w0, [x22, 768]
	cmp	w0, w19
	bhi	.L599
.L596:
	mov	x1, x25
	mov	w0, 14
	bl	dprint_vfmw
.L592:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	MVC_DumpDPB, .-MVC_DumpDPB
	.align	3
.LC44:
	.xword	g_PrintEnable
	.align	3
.LC46:
	.xword	.LC45
	.align	3
.LC48:
	.xword	.LC47
	.align	3
.LC50:
	.xword	.LC49
	.align	3
.LC52:
	.xword	.LC51
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UpdateCurrFrameInfo
	.type	MVC_UpdateCurrFrameInfo, %function
MVC_UpdateCurrFrameInfo:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 11157504
	mov	w2, 3
	ldr	x19, [x1, 2600]
	ldrb	w3, [x1, 1962]
	strb	w3, [x19, 1]
	ldrb	w3, [x1, 1964]
	cbnz	w3, .L606
	ldrb	w2, [x1, 1965]
	mov	w0, 3
	cmp	w2, wzr
	csel	w2, w2, w0, eq
.L606:
	add	x3, x1, 1960
	strb	w2, [x19, 3]
	mov	w2, 3
	ldr	w0, [x3, 656]
	str	w0, [x19, 20]
	ldr	w0, [x3, 700]
	str	w0, [x19, 44]
	ldrb	w0, [x3, 12]
	strb	w0, [x19]
	ldr	w0, [x3, 668]
	str	w0, [x19, 32]
	ldr	w0, [x3, 724]
	str	w0, [x19, 48]
	ldr	w0, [x3, 660]
	str	w0, [x19, 28]
	ldrb	w0, [x3, 9]
	str	w0, [x19, 40]
	ldrb	w0, [x3, 7]
	cmp	w0, 1
	csel	w0, w2, wzr, eq
	strb	w0, [x19, 4]
	ldrb	w4, [x3, 3]
	cmp	w4, 1
	beq	.L609
	cbz	w4, .L610
	cmp	w4, 2
	bne	.L605
	str	x19, [x19, 800]
	mov	w5, 1
	strb	w5, [x19, 792]
	ldrb	w5, [x3, 4]
	strb	w5, [x19, 793]
	ldrb	w5, [x3, 5]
	strb	w5, [x19, 794]
	ldr	w5, [x3, 676]
	str	w5, [x19, 816]
	ldrb	w6, [x3, 12]
	strb	w6, [x19, 795]
	ldr	w5, [x3, 700]
	str	w5, [x19, 824]
	str	wzr, [x19, 40]
	ldrb	w7, [x3, 1]
	cbz	w7, .L632
	ldr	w4, [x19, 776]
	orr	w0, w0, 2
	strb	w2, [x19, 2]
	mov	w2, 0
	add	w5, w5, w4
	str	w5, [x19, 44]
	str	w5, [x19, 728]
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	csel	w0, w0, w2, eq
	strb	w0, [x19, 4]
	ldr	x0, [x1, 2576]
	cmn	x0, #1
	beq	.L618
	ldr	x2, [x19, 672]
	cmn	x2, #1
	beq	.L633
.L618:
	ldrb	w2, [x19, 747]
.L636:
	cmp	w2, w6
	mov	x0, x19
	csel	w6, w2, w6, cs
	strb	w6, [x19]
	ldr	w1, [x1, 2628]
	str	w1, [x19, 32]
	bl	MVC_CombineFldsToFrm
.L605:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L610:
	strb	w2, [x19, 2]
	add	x1, x1, 1976
	strb	wzr, [x19, 696]
	mov	x2, 624
	str	x19, [x19, 704]
	add	x0, x19, 72
	ldrb	w23, [x3, 4]
	strb	w23, [x19, 697]
	ldrb	w22, [x3, 5]
	strb	w22, [x19, 698]
	ldr	w4, [x3, 672]
	str	w4, [x19, 720]
	ldrb	w20, [x3, 12]
	strb	w20, [x19, 699]
	ldr	w21, [x3, 700]
	str	w21, [x19, 728]
	ldr	w4, [x3, 676]
	str	w4, [x19, 768]
	ldr	w3, [x3, 680]
	str	w3, [x19, 816]
	bl	memcpy
	strb	w23, [x19, 745]
	strb	w22, [x19, 746]
	mov	w1, 1
	strb	w20, [x19, 747]
	mov	w0, 2
	strb	w1, [x19, 744]
	strb	w23, [x19, 793]
	strb	w22, [x19, 794]
	strb	w0, [x19, 792]
	strb	w20, [x19, 795]
	str	x19, [x19, 752]
	str	w21, [x19, 776]
	str	x19, [x19, 800]
	str	w21, [x19, 824]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L609:
	strb	w4, [x19, 744]
	str	x19, [x19, 752]
	ldrb	w5, [x3, 4]
	strb	w5, [x19, 745]
	ldrb	w5, [x3, 5]
	strb	w5, [x19, 746]
	ldr	w5, [x3, 676]
	str	w5, [x19, 768]
	ldrb	w6, [x3, 12]
	strb	w6, [x19, 747]
	ldr	w5, [x3, 700]
	str	w5, [x19, 776]
	str	wzr, [x19, 40]
	ldrb	w7, [x3, 1]
	cbz	w7, .L634
	ldr	w4, [x19, 824]
	orr	w0, w0, 1
	strb	w2, [x19, 2]
	mov	w2, 0
	add	w5, w5, w4
	str	w5, [x19, 44]
	str	w5, [x19, 728]
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	csel	w0, w0, w2, eq
	strb	w0, [x19, 4]
	ldr	x0, [x1, 2576]
	cmn	x0, #1
	beq	.L614
	ldr	x2, [x19, 672]
	cmn	x2, #1
	beq	.L635
.L614:
	ldrb	w2, [x19, 795]
	mov	x0, x19
	cmp	w2, w6
	csel	w6, w2, w6, cs
	strb	w6, [x19]
	ldr	w1, [x1, 2628]
	str	w1, [x19, 32]
	bl	MVC_CombineFldsToFrm
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L632:
	strb	w4, [x19, 2]
	mov	x2, 624
	str	w5, [x19, 44]
	add	x1, x1, 1976
	str	w5, [x19, 728]
	add	x0, x19, 72
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	csel	w7, wzr, w4, ne
	strb	w7, [x19, 4]
	bl	memcpy
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L634:
	strb	w4, [x19, 2]
	mov	x2, 624
	str	w5, [x19, 44]
	add	x1, x1, 1976
	str	w5, [x19, 728]
	add	x0, x19, 72
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	cset	w3, eq
	strb	w3, [x19, 4]
	bl	memcpy
	b	.L605
	.p2align 3
.L635:
	str	x0, [x19, 672]
	b	.L614
	.p2align 3
.L633:
	str	x0, [x19, 672]
	ldrb	w2, [x19, 747]
	b	.L636
	.size	MVC_UpdateCurrFrameInfo, .-MVC_UpdateCurrFrameInfo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_SimpleSlideDPB
	.type	MVC_SimpleSlideDPB, %function
MVC_SimpleSlideDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 11157504
	bl	_mcount
	add	x0, x21, 11124736
	ldr	w19, [x0, 760]
	ldr	w1, [x0, 764]
	cmp	w1, w19
	bcc	.L638
	cbz	w19, .L643
	add	x0, x0, 376
	mov	w5, 0
	mov	w4, 2147483647
	mov	w1, 0
	.p2align 2
.L641:
	ldr	x2, [x0]
	add	x0, x0, 8
	cbz	x2, .L640
	ldr	w3, [x2, 64]
	cmp	w3, w4
	bcs	.L640
	ldr	w6, [x2, 56]
	ldr	w2, [x20, 2688]
	cmp	w6, w2
	csel	w4, w4, w3, ne
	csel	w5, w5, w1, ne
.L640:
	add	w1, w1, 1
	cmp	w19, w1
	bne	.L641
	mov	w19, w5
.L639:
	sxtw	x1, w5
	mov	x0, x21
	add	x1, x1, 1388544
	add	x1, x21, x1, lsl 3
	ldr	x1, [x1, 16760]
	bl	MVC_UnMarkFrameStoreRef
	mov	w1, w19
	mov	x0, x21
	bl	MVC_RemoveFrameStoreOutDPB
.L638:
	ldr	w0, [x20, 2624]
	strb	wzr, [x20, 1964]
	cbz	w0, .L637
	mov	w0, 1
	strb	w0, [x20, 1965]
.L637:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L643:
	mov	w5, 0
	b	.L639
	.size	MVC_SimpleSlideDPB, .-MVC_SimpleSlideDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ReleaseNAL
	.type	MVC_ReleaseNAL, %function
MVC_ReleaseNAL:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w21, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L661
	ldr	x23, .LC58
	add	x19, x20, 8
	strb	wzr, [x20, 1]
	add	x22, x20, 72
	strb	wzr, [x20, 3]
.L656:
	ldr	x1, [x19]
	mov	w0, w21
	cbz	x1, .L655
	ldr	w1, [x19, 28]
	bl	SM_ReleaseStreamSeg
	ldr	w3, [x19, 8]
	mov	x1, x23
	ldr	x2, [x19]
	mov	w0, 7
	bl	dprint_vfmw
	str	xzr, [x19]
	str	wzr, [x19, 8]
	str	wzr, [x19, 20]
.L655:
	add	x19, x19, 32
	cmp	x19, x22
	bne	.L656
	strb	wzr, [x20]
	str	wzr, [x20, 76]
.L652:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L661:
	ldr	x2, .LC55
	mov	w3, 3998
	ldr	x1, .LC56
	mov	w0, 0
	bl	dprint_vfmw
	b	.L652
	.size	MVC_ReleaseNAL, .-MVC_ReleaseNAL
	.align	3
.LC55:
	.xword	.LC36
	.align	3
.LC56:
	.xword	.LC38
	.align	3
.LC58:
	.xword	.LC57
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ClearCurrNal
	.type	MVC_ClearCurrNal, %function
MVC_ClearCurrNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 240]
	cbz	x1, .L663
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L663:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_ClearCurrNal, .-MVC_ClearCurrNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ClearCurrSlice
	.type	MVC_ClearCurrSlice, %function
MVC_ClearCurrSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC60
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	cbz	x1, .L668
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L668:
	ldr	w1, [x19, 80]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 80]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_ClearCurrSlice, .-MVC_ClearCurrSlice
	.align	3
.LC60:
	.xword	.LC59
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ClearAllNal
	.type	MVC_ClearAllNal, %function
MVC_ClearAllNal:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 12288
	add	x19, x21, 1520
	bl	_mcount
	add	x20, x20, 2384
	b	.L674
	.p2align 3
.L673:
	add	x19, x19, 96
	cmp	x19, x20
	beq	.L676
.L674:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L673
	ldr	w0, [x21, 120]
	mov	x1, x19
	add	x19, x19, 96
	bl	MVC_ReleaseNAL
	cmp	x19, x20
	bne	.L674
.L676:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	MVC_ClearAllNal, .-MVC_ClearAllNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ClearAllSlice
	.type	MVC_ClearAllSlice, %function
MVC_ClearAllSlice:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x19, x21, 11157504
	add	x20, x21, 11161600
	add	x19, x19, 3464
	add	x20, x20, 456
	bl	_mcount
	.p2align 2
.L679:
	ldr	x0, [x19]
	mov	x1, x0
	cbz	x0, .L678
	ldr	w0, [x21, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19]
.L678:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L679
	str	wzr, [x21, 64]
	mov	w0, 0
	str	wzr, [x21, 104]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MVC_ClearAllSlice, .-MVC_ClearAllSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ClearCurrPic
	.type	MVC_ClearCurrPic, %function
MVC_ClearCurrPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	MVC_ClearAllSlice
	add	x0, x19, 11120640
	mov	w4, 32
	ldr	w8, [x19, 40]
	mov	w7, 23352
	movk	w7, 0x1, lsl 16
	mov	w6, 2
	strb	w4, [x0, 370]
	add	x4, x0, 256
	strb	wzr, [x0, 368]
	mov	w5, 7
	strb	w6, [x0, 369]
	mov	w3, 3
	strb	w5, [x0, 371]
	mov	w2, -1
	str	w8, [x0, 380]
	mov	w1, 262144
	str	w7, [x0, 384]
	stp	w3, w2, [x4, 136]
	stp	wzr, wzr, [x4, 144]
	str	w1, [x0, 408]
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_ClearCurrPic, .-MVC_ClearCurrPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ArrangeVahbMem
	.type	MVC_ArrangeVahbMem, %function
MVC_ArrangeVahbMem:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	w22, w4
	bl	_mcount
	ldr	x3, .LC61
	mov	x2, 32
	mov	w1, 0
	add	x0, x29, 64
	ldr	x3, [x3, 96]
	blr	x3
	strb	wzr, [x29, 64]
	ldr	x0, [x19, 224]
	stp	w20, w21, [x29, 88]
	strb	wzr, [x29, 65]
	ldr	w0, [x0, 28]
	stp	w20, w21, [x29, 72]
	stp	w20, w21, [x29, 80]
	cmp	w0, 24
	beq	.L686
	add	x0, x19, 11124736
	mov	w1, 5
	strb	wzr, [x29, 68]
	sxtb	w4, w22
	strb	w1, [x29, 69]
	ldr	w0, [x0, 760]
	add	w0, w0, 3
	strb	w0, [x29, 67]
.L692:
	ldr	w0, [x19, 120]
	add	x1, x29, 64
	strb	w4, [x29, 70]
	strb	wzr, [x29, 66]
	bl	FSP_ConfigInstance
	cbnz	w0, .L687
	ldr	x1, [x19, 224]
	add	x3, x29, 60
	ldr	w0, [x19, 120]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	mov	w1, w0
	beq	.L695
	mov	w0, 1
	cbnz	w1, .L689
.L693:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L686:
	mov	w1, 1
	mov	w0, 5
	strb	w1, [x29, 67]
	mov	w4, 1
	strb	wzr, [x29, 68]
	strb	w0, [x29, 69]
	b	.L692
	.p2align 3
.L695:
	add	x0, x19, 912
	bl	ResetVoQueue
.L689:
	ldr	x1, .LC63
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L693
	.p2align 3
.L687:
	ldr	x1, .LC65
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L693
	.size	MVC_ArrangeVahbMem, .-MVC_ArrangeVahbMem
	.align	3
.LC61:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC63:
	.xword	.LC62
	.align	3
.LC65:
	.xword	.LC64
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_RepairList
	.type	MVC_RepairList, %function
MVC_RepairList:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x6, x19, 11116544
	ldrb	w0, [x6, 2808]
	cmp	w0, 1
	beq	.L714
	cbz	w0, .L699
	cmp	w0, 2
	mov	w0, 0
	bne	.L710
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L710:
	mov	x0, x19
	bl	MVC_ClearCurrSlice
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L714:
	mov	w10, 2
.L698:
	add	x6, x6, 2856
	add	x5, x19, 288
	mov	x8, 0
	mov	w7, 0
	mov	w12, 1
	add	x11, x19, 11157504
	.p2align 2
.L711:
	ldr	w0, [x6]
	cbz	w0, .L701
	ldr	x1, [x5]
	cbnz	x1, .L725
	sub	w3, w0, #1
	add	x9, x8, 37
	add	x0, x5, 8
	add	x4, x9, x3, uxtw
	add	x4, x19, x4, lsl 3
	b	.L704
	.p2align 3
.L705:
	ldr	x2, [x0], 8
	cbnz	x2, .L712
.L704:
	cmp	x0, x4
	bne	.L705
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L710
	ldr	x2, [x11, 2600]
	add	x2, x2, 696
.L712:
	add	x3, x9, x3, uxtw
	mov	x0, x5
	add	x3, x19, x3, lsl 3
	cbz	x1, .L726
	.p2align 2
.L707:
	add	x0, x0, 8
	cmp	x3, x0
	beq	.L708
.L727:
	ldr	x1, [x0]
	cbnz	x1, .L707
.L726:
	str	x2, [x0]
	add	x0, x0, 8
	cmp	x3, x0
	bne	.L727
	.p2align 2
.L708:
	add	w7, w7, 1
	add	x6, x6, 4
	cmp	w7, w10
	add	x5, x5, 264
	add	x8, x8, 33
	bcc	.L711
.L728:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L701:
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L710
	str	w12, [x6]
	add	w7, w7, 1
	cmp	w7, w10
	add	x6, x6, 4
	ldr	x0, [x11, 2600]
	add	x8, x8, 33
	add	x0, x0, 696
	str	x0, [x5]
	add	x5, x5, 264
	bcc	.L711
	b	.L728
.L725:
	sub	w3, w0, #1
	mov	x2, x1
	add	x9, x8, 37
	b	.L712
.L699:
	mov	w10, 1
	b	.L698
	.size	MVC_RepairList, .-MVC_RepairList
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetShortTermPicPoint
	.type	MVC_GetShortTermPicPoint, %function
MVC_GetShortTermPicPoint:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	x0, x20, 11116544
	mov	w5, 0
	ldrb	w1, [x0, 2809]
	cbz	w1, .L730
	ldrb	w0, [x0, 2810]
	mov	w5, 2
	cmp	w0, wzr
	csinc	w5, w5, wzr, ne
.L730:
	add	x0, x20, 11124736
	ldr	w2, [x0, 768]
	cbz	w2, .L738
	sub	w2, w2, #1
	add	x0, x0, 504
	add	x2, x2, 1388544
	add	x2, x2, 2112
	add	x4, x20, x2, lsl 3
	b	.L735
	.p2align 3
.L748:
	cmp	w3, 3
	beq	.L747
.L733:
	add	x0, x0, 8
	cmp	x0, x4
	beq	.L738
.L735:
	ldr	x2, [x0]
	ldrb	w3, [x2, 3]
	cbz	w5, .L748
	tbz	x3, 0, .L734
	ldr	w6, [x2, 744]
	and	w6, w6, 16776960
	cmp	w6, 65536
	beq	.L749
.L734:
	tbz	x3, 1, .L733
	ldr	w3, [x2, 792]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L733
	ldr	w1, [x2, 812]
	cmp	w19, w1
	bne	.L733
	add	x0, x2, 792
	b	.L731
	.p2align 3
.L749:
	ldr	w1, [x2, 764]
	cmp	w1, w19
	bne	.L734
	add	x0, x2, 744
	b	.L731
	.p2align 3
.L747:
	ldr	w3, [x2, 696]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L733
	ldr	w1, [x2, 716]
	cmp	w19, w1
	bne	.L733
	add	x0, x2, 696
	b	.L731
	.p2align 3
.L738:
	mov	x0, 0
.L731:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_GetShortTermPicPoint, .-MVC_GetShortTermPicPoint
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetLongTermPicPoint
	.type	MVC_GetLongTermPicPoint, %function
MVC_GetLongTermPicPoint:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	x0, x20, 11116544
	mov	w5, 0
	ldrb	w1, [x0, 2809]
	cbz	w1, .L751
	ldrb	w0, [x0, 2810]
	mov	w5, 2
	cmp	w0, wzr
	csinc	w5, w5, wzr, ne
.L751:
	add	x0, x20, 11124736
	ldr	w2, [x0, 772]
	cbz	w2, .L759
	sub	w2, w2, #1
	add	x0, x0, 632
	add	x2, x2, 1388544
	add	x2, x2, 2128
	add	x4, x20, x2, lsl 3
	b	.L756
	.p2align 3
.L769:
	cmp	w3, 3
	beq	.L768
.L754:
	add	x0, x0, 8
	cmp	x0, x4
	beq	.L759
.L756:
	ldr	x2, [x0]
	ldrb	w3, [x2, 3]
	cbz	w5, .L769
	tbz	x3, 0, .L755
	ldr	w6, [x2, 744]
	and	w6, w6, 16776960
	cmp	w6, 256
	beq	.L770
.L755:
	tbz	x3, 1, .L754
	ldr	w3, [x2, 792]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L754
	ldr	w1, [x2, 808]
	cmp	w19, w1
	bne	.L754
	add	x0, x2, 792
	b	.L752
	.p2align 3
.L770:
	ldr	w1, [x2, 760]
	cmp	w1, w19
	bne	.L755
	add	x0, x2, 744
	b	.L752
	.p2align 3
.L768:
	ldr	w3, [x2, 696]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L754
	ldr	w1, [x2, 712]
	cmp	w19, w1
	bne	.L754
	add	x0, x2, 696
	b	.L752
	.p2align 3
.L759:
	mov	x0, 0
.L752:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_GetLongTermPicPoint, .-MVC_GetLongTermPicPoint
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ReorderSTList
	.type	MVC_ReorderSTList, %function
MVC_ReorderSTList:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	w23, w3
	mov	x22, x4
	mov	w24, w5
	bl	_mcount
	uxtw	x6, w20
	mov	x0, x19
	lsl	x2, x6, 8
	mov	w1, w23
	add	x6, x2, x6, lsl 3
	add	x6, x6, 288
	add	x19, x19, x6
	bl	MVC_GetShortTermPicPoint
	ldr	w6, [x22]
	add	w2, w21, 1
	cmp	w2, w6
	ble	.L772
	mov	w1, w2
	.p2align 2
.L773:
	add	x6, x19, x1, sxtw 3
	ldr	x6, [x6, -8]
	str	x6, [x19, w1, sxtw 3]
	sub	w1, w1, #1
	ldr	w6, [x22]
	cmp	w6, w1
	blt	.L773
.L772:
	add	w1, w6, 1
	str	w1, [x22]
	str	x0, [x19, w6, sxtw 3]
	ldr	w0, [x22]
	cmp	w2, w0
	mov	w4, w0
	bge	.L781
	b	.L771
	.p2align 3
.L776:
	str	x1, [x19, w4, sxtw 3]
	add	w4, w4, 1
.L775:
	cmp	w2, w0
	blt	.L771
.L781:
	ldr	x1, [x19, w0, sxtw 3]
	add	w0, w0, 1
	cbz	x1, .L775
	ldrb	w6, [x1, 1]
	cbnz	w6, .L776
	ldr	w6, [x1, 20]
	cmp	w23, w6
	bne	.L776
	ldr	x3, [x1, 8]
	ldr	w3, [x3, 56]
	cmp	w3, w24
	bne	.L776
	cmp	w2, w0
	bge	.L781
.L771:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	MVC_ReorderSTList, .-MVC_ReorderSTList
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ReorderLTList
	.type	MVC_ReorderLTList, %function
MVC_ReorderLTList:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	mov	w21, w2
	mov	w23, w3
	mov	x22, x4
	mov	w24, w5
	bl	_mcount
	uxtw	x6, w20
	mov	x0, x19
	lsl	x2, x6, 8
	mov	w1, w23
	add	x6, x2, x6, lsl 3
	add	x6, x6, 288
	add	x19, x19, x6
	bl	MVC_GetLongTermPicPoint
	ldr	w6, [x22]
	add	w2, w21, 1
	cmp	w2, w6
	ble	.L787
	mov	w1, w2
	.p2align 2
.L788:
	add	x6, x19, x1, sxtw 3
	ldr	x6, [x6, -8]
	str	x6, [x19, w1, sxtw 3]
	sub	w1, w1, #1
	ldr	w6, [x22]
	cmp	w6, w1
	blt	.L788
.L787:
	add	w1, w6, 1
	str	w1, [x22]
	str	x0, [x19, w6, sxtw 3]
	ldr	w0, [x22]
	cmp	w2, w0
	mov	w4, w0
	bge	.L798
	b	.L786
	.p2align 3
.L791:
	str	x1, [x19, w4, sxtw 3]
	add	w4, w4, 1
.L790:
	cmp	w2, w0
	blt	.L786
.L798:
	ldr	x1, [x19, w0, sxtw 3]
	add	w0, w0, 1
	cbz	x1, .L790
	ldrb	w6, [x1, 1]
	cbz	w6, .L791
	ldr	w6, [x1, 16]
	cmp	w23, w6
	bne	.L791
	ldr	x3, [x1, 8]
	ldr	w3, [x3, 56]
	cmp	w3, w24
	bne	.L791
	cmp	w2, w0
	bge	.L798
.L786:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	MVC_ReorderLTList, .-MVC_ReorderLTList
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetMaxViewIdx
	.type	MVC_GetMaxViewIdx, %function
MVC_GetMaxViewIdx:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w1
	mov	w22, w2
	mov	w20, w3
	bl	_mcount
	add	x0, x21, 10768384
	add	x1, x0, 4068
	ldr	w5, [x0, 4064]
	adds	w5, w5, 1
	beq	.L812
	ldr	w0, [x0, 4068]
	cmp	w19, w0
	beq	.L812
	mov	w6, w5
	mov	w0, 0
	b	.L806
	.p2align 3
.L808:
	ldr	w4, [x1, w0, sxtw 2]
	cmp	w19, w4
	beq	.L814
.L806:
	add	w0, w0, 1
	cmp	w0, w5
	bne	.L808
.L809:
	mov	w0, 0
	tbnz	w6, #31, .L807
.L805:
	sbfiz	x0, x20, 1, 32
	add	x0, x0, x6, sxtw
	cbnz	w22, .L815
	add	x0, x21, x0, lsl 2
	add	x0, x0, 10772480
	ldr	w0, [x0, 12]
.L807:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L815:
	add	x0, x0, 2691072
	ldp	x19, x20, [sp, 16]
	add	x0, x21, x0, lsl 2
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ldr	w0, [x0, 8172]
	ret
	.p2align 3
.L814:
	mov	w6, w0
	b	.L809
.L812:
	mov	w6, 0
	b	.L805
	.size	MVC_GetMaxViewIdx, .-MVC_GetMaxViewIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_get_inter_view_pic
	.type	mvc_get_inter_view_pic, %function
mvc_get_inter_view_pic:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w22, w3
	mov	w19, w1
	mov	w21, w2
	bl	_mcount
	uxtw	x0, w22
	add	x2, x0, 2777088
	lsl	x1, x0, 3
	sub	x1, x1, x0
	add	x2, x20, x2, lsl 2
	lsl	x0, x1, 4
	sub	x0, x0, x1
	lsl	x0, x0, 4
	ldr	w1, [x2, 12648]
	add	x0, x0, 11108352
	add	x0, x0, 272
	add	x0, x20, x0
	cbz	w1, .L822
	sub	w1, w1, #1
	add	x5, x20, 11157504
	add	x1, x1, 1
	lsl	x4, x1, 3
	sub	x4, x4, x1
	lsl	x3, x4, 4
	sub	x4, x3, x4
	add	x4, x0, x4, lsl 3
	b	.L821
	.p2align 3
.L818:
	add	x0, x0, 840
	cmp	x0, x4
	beq	.L822
.L821:
	ldr	w3, [x0, 56]
	cmp	w3, w19
	bne	.L818
	ldrb	w1, [x5, 1963]
	cmp	w1, 1
	cbnz	w1, .L819
	ldr	w1, [x0, 724]
	cmp	w21, w1
	bne	.L818
	add	x0, x0, 696
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L819:
	bne	.L820
	ldr	w1, [x0, 772]
	cmp	w1, w21
	bne	.L818
	add	x0, x0, 744
	b	.L817
	.p2align 3
.L820:
	cmp	w1, 2
	bne	.L818
	ldr	w1, [x0, 820]
	cmp	w21, w1
	bne	.L818
	add	x0, x0, 792
	b	.L817
	.p2align 3
.L822:
	mov	x0, 0
.L817:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	mvc_get_inter_view_pic, .-mvc_get_inter_view_pic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_reorder_interview
	.type	mvc_reorder_interview, %function
mvc_reorder_interview:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	w24, w5
	mov	w22, w3
	mov	w23, w4
	mov	w20, w1
	mov	x21, x2
	bl	_mcount
	uxtw	x0, w24
	mov	w3, w24
	lsl	x5, x0, 8
	mov	w2, w23
	add	x5, x5, x0, lsl 3
	mov	w1, w22
	mov	x0, x19
	add	x5, x5, 288
	add	x19, x19, x5
	bl	mvc_get_inter_view_pic
	cbz	x0, .L824
	ldr	w3, [x21]
	add	w1, w20, 1
	cmp	w1, w3
	bls	.L826
	mov	w2, w1
	uxtw	x5, w1
	.p2align 2
.L827:
	sub	w2, w2, #1
	lsl	x3, x5, 3
	uxtw	x5, w2
	ldr	x6, [x19, w2, uxtw 3]
	str	x6, [x19, x3]
	ldr	w3, [x21]
	cmp	w3, w2
	bcc	.L827
.L826:
	add	w2, w3, 1
	str	w2, [x21]
	str	x0, [x19, w3, uxtw 3]
	ldr	w0, [x21]
	cmp	w1, w0
	bcc	.L824
	mov	w3, w0
	b	.L831
	.p2align 3
.L829:
	str	x2, [x19, w3, uxtw 3]
	cmp	w1, w0
	add	w3, w3, 1
	bcc	.L824
.L831:
	ldr	x2, [x19, w0, uxtw 3]
	add	w0, w0, 1
	cbz	x2, .L829
	ldr	x5, [x2, 8]
	ldr	w5, [x5, 56]
	cmp	w22, w5
	bne	.L829
	ldr	w5, [x2, 28]
	cmp	w23, w5
	bne	.L829
	cmp	w1, w0
	bcs	.L831
.L824:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	mvc_reorder_interview, .-mvc_reorder_interview
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ReorderRefPiclist
	.type	MVC_ReorderRefPiclist, %function
MVC_ReorderRefPiclist:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x27, x0
	mov	x0, x30
	add	x20, x27, 11116544
	mov	w19, w1
	mov	w21, w2
	bl	_mcount
	str	wzr, [x29, 188]
	mov	w0, 0
	ldrb	w1, [x20, 2809]
	cbz	w1, .L839
	ldrb	w1, [x20, 2810]
	mov	w0, 2
	cmp	w1, wzr
	csinc	w0, w0, wzr, ne
.L839:
	cmp	w19, 1
	bls	.L895
.L838:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 192
	ret
	.p2align 3
.L895:
	cbz	w19, .L896
	add	x1, x20, 1892
	str	x1, [x29, 168]
	add	x1, x20, 2156
	str	x1, [x29, 120]
	add	x1, x20, 2416
	str	x1, [x29, 104]
	ldr	x1, [x27, 248]
	add	x28, x20, 2676
	ldr	w25, [x1, 2896]
	add	w1, w25, 4
	mov	w25, 1
	lsl	w1, w25, w1
	str	w1, [x29, 148]
	cbnz	w0, .L843
.L901:
	ldr	w0, [x20, 2836]
	str	w0, [x29, 144]
.L844:
	cmn	w21, #2
	beq	.L838
	ldr	x0, [x29, 168]
	ldr	w1, [x0]
	cmp	w1, 3
	beq	.L838
	add	w23, w21, 1
	mov	w2, 1
	lsl	x0, x23, 2
	str	x0, [x29, 160]
	stp	w2, wzr, [x29, 152]
	add	x0, x27, 10768384
	add	x2, x27, 11157504
	mov	x23, 0
	str	x2, [x29, 136]
	mov	w22, 0
	ubfiz	x2, x19, 1, 32
	mov	w26, 0
	ldr	w24, [x29, 144]
	add	x25, x0, 4068
	str	x0, [x29, 112]
	str	x2, [x29, 128]
	b	.L847
	.p2align 3
.L900:
	ldr	w3, [x0, 8188]
.L870:
	ldr	x0, [x29, 136]
	mov	w5, w19
	add	x2, x29, 188
	mov	w1, w21
	ldr	w4, [x0, 2628]
	mov	x0, x27
	bl	mvc_reorder_interview
.L852:
	ldr	x0, [x29, 160]
	cmp	x0, x23
	beq	.L838
	add	x23, x23, 4
	ldr	x0, [x29, 168]
	ldr	w1, [x0, x23]
	cmp	w1, 3
	beq	.L838
.L847:
	cmp	w1, 1
	bls	.L897
	cmp	w1, 2
	beq	.L898
	ldr	w0, [x29, 152]
	cbnz	w0, .L854
	ldrb	w5, [x20, 2817]
	mov	w6, w26
.L855:
	cmp	w1, 4
	ldr	w0, [x28, x23]
	beq	.L899
	add	w0, w0, 1
	add	w26, w0, w6
	cmp	w22, w26
	sub	w0, w26, w22
	csel	w26, w0, w26, le
.L868:
	ldr	w1, [x29, 156]
	ldr	x0, [x29, 128]
	add	x0, x0, x26, sxtw
	add	x0, x0, x1, sxtw
	add	x0, x0, 2691072
	add	x0, x27, x0, lsl 2
	cbnz	w5, .L900
	ldr	w3, [x0, 8220]
	b	.L870
	.p2align 3
.L896:
	add	x1, x20, 1760
	str	x1, [x29, 168]
	add	x1, x20, 2024
	str	x1, [x29, 120]
	add	x1, x20, 2288
	str	x1, [x29, 104]
	ldr	x1, [x27, 248]
	add	x28, x20, 2544
	ldr	w25, [x1, 2896]
	add	w1, w25, 4
	mov	w25, 1
	lsl	w1, w25, w1
	str	w1, [x29, 148]
	cbz	w0, .L901
.L843:
	ldr	w0, [x20, 2836]
	ldr	w1, [x29, 148]
	lsl	w0, w0, 1
	add	w0, w0, 1
	lsl	w1, w1, 1
	stp	w0, w1, [x29, 144]
	b	.L844
	.p2align 3
.L897:
	ldr	x0, [x29, 120]
	ldr	w0, [x0, x23]
	cbnz	w1, .L849
	mvn	w0, w0
	add	w24, w24, w0
	ldr	w0, [x29, 148]
	cmp	w24, wzr
	add	w0, w24, w0
	csel	w24, w0, w24, lt
.L850:
	ldr	w0, [x29, 144]
	add	x4, x29, 188
	mov	w2, w21
	mov	w1, w19
	cmp	w0, w24
	add	x0, x27, 11120640
	ldr	w5, [x0, 352]
	ldr	w0, [x29, 148]
	sub	w3, w24, w0
	mov	x0, x27
	csel	w3, w3, w24, lt
	bl	MVC_ReorderSTList
	b	.L852
	.p2align 3
.L899:
	mvn	w0, w0
	add	w26, w0, w6
	cmp	w26, wzr
	add	w0, w26, w22
	csel	w26, w0, w26, lt
	b	.L868
	.p2align 3
.L854:
	ldr	x4, [x29, 112]
	add	x0, x27, 11120640
	ldr	w2, [x4, 4064]
	ldr	w3, [x0, 352]
	adds	w2, w2, 1
	beq	.L857
	ldr	w0, [x4, 4068]
	cmp	w3, w0
	beq	.L857
	mov	w0, 0
	str	w2, [x29, 156]
	b	.L860
	.p2align 3
.L861:
	ldr	w4, [x25, w0, sxtw 2]
	cmp	w3, w4
	beq	.L902
.L860:
	add	w0, w0, 1
	cmp	w2, w0
	bne	.L861
.L892:
	ldrb	w5, [x20, 2817]
	mov	w6, w2
	mov	w0, 0
	b	.L864
	.p2align 3
.L865:
	ldr	w4, [x25, w0, sxtw 2]
	cmp	w3, w4
	beq	.L894
.L864:
	add	w0, w0, 1
	cmp	w0, w2
	bne	.L865
	tbnz	w6, #31, .L878
.L872:
	sbfiz	x0, x19, 1, 32
	add	x0, x0, x6, sxtw
	cbz	w5, .L866
	add	x0, x0, 2691072
	mov	w6, -1
	str	wzr, [x29, 152]
	add	x0, x27, x0, lsl 2
	ldr	w22, [x0, 8172]
	b	.L855
	.p2align 3
.L849:
	add	w0, w0, 1
	add	w24, w24, w0
	ldr	w0, [x29, 148]
	cmp	w0, w24
	sub	w0, w24, w0
	csel	w24, w0, w24, le
	b	.L850
	.p2align 3
.L898:
	add	x0, x27, 11120640
	add	x4, x29, 188
	mov	w2, w21
	mov	w1, w19
	ldr	w5, [x0, 352]
	ldr	x0, [x29, 104]
	ldr	w3, [x0, x23]
	mov	x0, x27
	bl	MVC_ReorderLTList
	b	.L852
	.p2align 3
.L894:
	mov	w6, w0
	tbz	w6, #31, .L872
.L878:
	mov	w6, -1
	mov	w22, 0
	str	wzr, [x29, 152]
	b	.L855
	.p2align 3
.L902:
	str	w0, [x29, 156]
	b	.L892
	.p2align 3
.L866:
	add	x0, x27, x0, lsl 2
	mov	w6, -1
	add	x0, x0, 10772480
	str	wzr, [x29, 152]
	ldr	w22, [x0, 12]
	b	.L855
.L857:
	ldrb	w5, [x20, 2817]
	mov	w6, 0
	str	wzr, [x29, 156]
	b	.L872
	.size	MVC_ReorderRefPiclist, .-MVC_ReorderRefPiclist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ReorderListX
	.type	MVC_ReorderListX, %function
MVC_ReorderListX:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 11116544
	bl	_mcount
	ldrb	w1, [x19, 2808]
	cmp	w1, 2
	beq	.L903
	ldrb	w0, [x19, 1756]
	cbnz	w0, .L915
.L906:
	ldr	w0, [x19, 2868]
	cmp	w1, 1
	add	w0, w0, 1
	str	w0, [x19, 2856]
	bne	.L903
	ldrb	w0, [x19, 1757]
	cbnz	w0, .L916
.L908:
	ldr	w0, [x19, 2872]
	add	w0, w0, 1
	str	w0, [x19, 2860]
.L903:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L915:
	ldr	w2, [x19, 2868]
	mov	w1, 0
	mov	x0, x20
	bl	MVC_ReorderRefPiclist
	ldrb	w1, [x19, 2808]
	b	.L906
	.p2align 3
.L916:
	ldr	w2, [x19, 2872]
	mov	x0, x20
	bl	MVC_ReorderRefPiclist
	b	.L908
	.size	MVC_ReorderListX, .-MVC_ReorderListX
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GenPiclistfromFrmlist
	.type	MVC_GenPiclistfromFrmlist, %function
MVC_GenPiclistfromFrmlist:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	w21, w0
	mov	x0, x30
	mov	w20, w2
	mov	x19, x1
	mov	x25, x3
	mov	x24, x4
	mov	w23, w5
	bl	_mcount
	cmp	w20, 31
	mov	w22, 31
	csel	w22, w20, w22, ls
	cbnz	w23, .L939
	cmp	w21, 1
	ldr	x23, .LC67
	beq	.L979
.L919:
	cmp	w21, 2
	bne	.L928
	mov	w21, 0
	mov	w20, 0
	.p2align 2
.L930:
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	bcs	.L928
.L980:
	cmp	w21, w22
	bcc	.L963
	b	.L935
	.p2align 3
.L931:
	add	w21, w21, 1
	cmp	w22, w21
	bls	.L935
.L963:
	ldr	x0, [x19, w21, uxtw 3]
	add	x26, x19, x21, uxtw 3
	ldrb	w1, [x0, 2]
	tbz	x1, 1, .L931
	add	x0, x0, 792
	blr	x23
	cbz	w0, .L931
	ldr	w1, [x24]
	add	w21, w21, 1
	ldr	x0, [x26]
	add	x0, x0, 792
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
.L935:
	cmp	w20, w22
	bcc	.L962
	b	.L930
	.p2align 3
.L936:
	add	w20, w20, 1
	cmp	w22, w20
	bls	.L930
.L962:
	ldr	x0, [x19, w20, uxtw 3]
	add	x26, x19, x20, uxtw 3
	ldrb	w1, [x0, 2]
	tbz	x1, 0, .L936
	add	x0, x0, 744
	blr	x23
	cbz	w0, .L936
	ldr	w1, [x24]
	add	w20, w20, 1
	ldr	x0, [x26]
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	add	x0, x0, 744
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	bcc	.L980
.L928:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L939:
	cmp	w21, 1
	ldr	x23, .LC66
	bne	.L919
.L979:
	mov	w21, 0
	mov	w20, 0
	.p2align 2
.L920:
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	bcs	.L928
	cmp	w20, w22
	bcc	.L961
	b	.L978
	.p2align 3
.L921:
	add	w20, w20, 1
	cmp	w20, w22
	bcs	.L978
.L961:
	ldr	x0, [x19, w20, uxtw 3]
	add	x26, x19, x20, uxtw 3
	ldrb	w1, [x0, 2]
	tbz	x1, 0, .L921
	add	x0, x0, 744
	blr	x23
	cbz	w0, .L921
	ldr	w1, [x24]
	add	w20, w20, 1
	ldr	x0, [x26]
	add	x0, x0, 744
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	.p2align 2
.L978:
	cmp	w21, w22
	bcs	.L920
	ldr	x0, [x19, w21, uxtw 3]
	add	x26, x19, x21, uxtw 3
	ldrb	w1, [x0, 2]
	tbnz	x1, 1, .L981
.L926:
	add	w21, w21, 1
	b	.L978
	.p2align 3
.L981:
	add	x0, x0, 792
	blr	x23
	cbz	w0, .L926
	ldr	w1, [x24]
	add	w21, w21, 1
	ldr	x0, [x26]
	add	x0, x0, 792
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	b	.L920
	.size	MVC_GenPiclistfromFrmlist, .-MVC_GenPiclistfromFrmlist
	.align	3
.LC66:
	.xword	MVC_IsLTRefFlg
	.align	3
.LC67:
	.xword	MVC_IsSTRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetBaseViewId
	.type	MVC_GetBaseViewId, %function
MVC_GetBaseViewId:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 12288
	mov	x1, 0
	add	x0, x0, 2400
	.p2align 2
.L985:
	ldr	w2, [x0]
	add	x3, x0, 335872
	cbz	w2, .L983
	ldrb	w0, [x0, -4]
	cbnz	w0, .L993
.L983:
	add	w1, w1, 1
	add	x0, x3, 308
	cmp	w1, 32
	bne	.L985
	mov	w0, -1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L993:
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 2
	sub	x0, x0, x1
	lsl	x0, x0, 4
	sub	x1, x0, x1
	lsl	x0, x1, 2
	sub	x1, x0, x1
	lsl	x0, x1, 6
	add	x0, x1, x0
	add	x0, x19, x0, lsl 2
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ldr	w0, [x0, 14692]
	ret
	.size	MVC_GetBaseViewId, .-MVC_GetBaseViewId
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetVOIdx
	.type	MVC_GetVOIdx, %function
MVC_GetVOIdx:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	w19, w1
	mov	x0, x30
	mov	w21, w2
	bl	_mcount
	cbz	w19, .L999
	ldr	w0, [x20]
	cmp	w21, w0
	beq	.L999
	mov	w0, w19
	mov	w3, 0
	b	.L996
	.p2align 3
.L997:
	ldr	w4, [x20, w3, sxtw 2]
	cmp	w4, w21
	beq	.L1000
.L996:
	add	w3, w3, 1
	cmp	w3, w19
	bne	.L997
.L995:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1000:
	mov	w0, w3
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L999:
	mov	w0, 0
	b	.L995
	.size	MVC_GetVOIdx, .-MVC_GetVOIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_is_view_id_in_ref_view_list
	.type	MVC_is_view_id_in_ref_view_list, %function
MVC_is_view_id_in_ref_view_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w20, w0
	mov	w19, w2
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	cbz	w19, .L1006
	ldr	w0, [x21]
	cmp	w20, w0
	beq	.L1006
	mov	w0, 0
	b	.L1003
	.p2align 3
.L1004:
	ldr	w3, [x21, w0, sxtw 2]
	cmp	w3, w20
	beq	.L1002
.L1003:
	add	w0, w0, 1
	cmp	w0, w19
	mov	w1, w0
	bne	.L1004
.L1002:
	cmp	w19, wzr
	ldr	x21, [sp, 32]
	ccmp	w19, w1, 0, ne
	cset	w0, hi
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L1006:
	mov	w1, 0
	b	.L1002
	.size	MVC_is_view_id_in_ref_view_list, .-MVC_is_view_id_in_ref_view_list
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GenPiclistfromFrmlist_Interview
	.type	MVC_GenPiclistfromFrmlist_Interview, %function
MVC_GenPiclistfromFrmlist_Interview:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w22, w0
	mov	x0, x30
	mov	x23, x1
	mov	w20, w2
	mov	x21, x3
	mov	x19, x4
	bl	_mcount
	cmp	w22, 1
	beq	.L1021
	cmp	w22, 2
	bne	.L1007
	cbz	w20, .L1007
	sub	w2, w20, #1
	mov	x1, 1632
	mov	w20, 840
	ldr	w0, [x19]
	add	x5, x23, 792
	umaddl	x20, w2, w20, x1
	add	x1, x23, x20
	.p2align 2
.L1012:
	str	x5, [x21, w0, uxtw 3]
	add	x5, x5, 840
	cmp	x5, x1
	ldr	w0, [x19]
	add	w0, w0, 1
	str	w0, [x19]
	bne	.L1012
.L1007:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1021:
	cbz	w20, .L1007
	sub	w2, w20, #1
	mov	x1, 1584
	mov	w20, 840
	ldr	w0, [x19]
	add	x4, x23, 744
	umaddl	x20, w2, w20, x1
	add	x1, x23, x20
	.p2align 2
.L1010:
	str	x4, [x21, w0, uxtw 3]
	add	x4, x4, 840
	cmp	x1, x4
	ldr	w0, [x19]
	add	w0, w0, 1
	str	w0, [x19]
	bne	.L1010
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	MVC_GenPiclistfromFrmlist_Interview, .-MVC_GenPiclistfromFrmlist_Interview
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_append_interview_list
	.type	mvc_append_interview_list, %function
mvc_append_interview_list:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x22, x0
	mov	x0, x30
	mov	w19, w1
	mov	x21, x2
	mov	x24, x3
	bl	_mcount
	add	x0, x22, 10768384
	add	x1, x22, 11120640
	add	x6, x0, 4068
	ldr	w2, [x0, 4064]
	ldr	w3, [x1, 352]
	adds	w2, w2, 1
	beq	.L1023
	ldr	w0, [x0, 4068]
	cmp	w3, w0
	beq	.L1041
	mov	w5, w2
	mov	w0, 0
	b	.L1024
	.p2align 3
.L1025:
	ldr	w4, [x6, w0, sxtw 2]
	cmp	w3, w4
	beq	.L1023
.L1024:
	add	w0, w0, 1
	cmp	w0, w5
	mov	w2, w0
	bne	.L1025
.L1023:
	add	x0, x22, 11116544
	ldrb	w0, [x0, 2817]
	cbz	w0, .L1026
	ubfiz	x0, x19, 1, 32
	add	x0, x0, x2, uxtw
	add	x0, x0, 2691072
	add	x23, x0, 2044
	add	x0, x22, x0, lsl 2
	add	x23, x22, x23, lsl 2
	add	x23, x23, 12
	ldr	w25, [x0, 8172]
.L1027:
	add	x20, x22, 11157504
	add	x0, x22, 11124736
	ldrb	w1, [x20, 1963]
	ldr	w19, [x0, 760]
	cmp	w1, 2
	cset	w26, eq
	subs	w19, w19, #1
	bmi	.L1022
	ldr	x28, .LC68
	mov	w27, 840
	b	.L1058
	.p2align 3
.L1069:
	ldrb	w0, [x1, 2]
	cmp	w0, 3
	beq	.L1068
.L1029:
	subs	w19, w19, #1
	bmi	.L1022
.L1058:
	sxtw	x0, w19
	add	x0, x0, 1388544
	add	x0, x22, x0, lsl 3
	ldr	x1, [x0, 16760]
	cbz	x1, .L1029
	ldrb	w0, [x20, 1963]
	cbz	w0, .L1069
	cmp	w0, 1
	beq	.L1070
	cmp	w0, 2
	bne	.L1029
	ldrb	w2, [x20, 1961]
	ldrb	w0, [x1, 2]
	ubfx	x0, x0, 1, 1
	cbz	w2, .L1035
	ldr	x2, [x20, 2600]
	ldr	w3, [x2, 772]
	ldr	w2, [x1, 772]
	cmp	w3, w2
	cset	w2, eq
	and	w0, w0, w2
.L1035:
	cbz	w0, .L1029
	ldr	w2, [x1, 820]
	.p2align 2
.L1031:
	add	x0, x1, x26
	ldrb	w0, [x0, 12]
	cbz	w0, .L1029
	ldr	w0, [x20, 2628]
	cmp	w0, w2
	bne	.L1029
	ldr	w3, [x1, 56]
	cbz	w25, .L1029
	ldr	w0, [x23]
	cmp	w3, w0
	beq	.L1036
	mov	w0, 0
	.p2align 2
.L1037:
	add	w0, w0, 1
	cmp	w25, w0
	beq	.L1029
	ldr	w2, [x23, w0, sxtw 2]
	cmp	w3, w2
	bne	.L1037
.L1036:
	ldr	w0, [x24]
	mov	x2, 840
	ldr	x3, [x28, 104]
	umull	x0, w0, w27
	add	x0, x21, x0
	blr	x3
	ldr	w0, [x24]
	umull	x0, w0, w27
	add	x0, x21, x0
	str	x0, [x0, 704]
	str	x0, [x0, 752]
	str	x0, [x0, 800]
	ldr	w0, [x24]
	ldr	w1, [x22, 52]
	umull	x0, w0, w27
	sub	w1, w1, #1
	add	x0, x21, x0
	str	w1, [x0, 48]
	ldr	w0, [x24]
	cmp	w25, w0
	beq	.L1022
	add	w0, w0, 1
	subs	w19, w19, #1
	str	w0, [x24]
	bpl	.L1058
.L1022:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1070:
	ldrb	w2, [x20, 1961]
	ldrb	w0, [x1, 2]
	and	w0, w0, 1
	cbz	w2, .L1033
	ldr	x3, [x20, 2600]
	ldr	w2, [x1, 820]
	ldr	w3, [x3, 820]
	cmp	w3, w2
	cset	w2, eq
	and	w0, w0, w2
.L1033:
	cbz	w0, .L1029
	ldr	w2, [x1, 772]
	b	.L1031
	.p2align 3
.L1068:
	ldr	w2, [x1, 724]
	b	.L1031
	.p2align 3
.L1026:
	ubfiz	x0, x19, 1, 32
	add	x0, x0, x2, uxtw
	add	x0, x0, 2691072
	add	x23, x0, 2052
	add	x0, x22, x0, lsl 2
	add	x23, x22, x23, lsl 2
	add	x23, x23, 12
	ldr	w25, [x0, 8204]
	b	.L1027
.L1041:
	mov	w2, 0
	b	.L1023
	.size	mvc_append_interview_list, .-mvc_append_interview_list
	.align	3
.LC68:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_InitListX
	.type	MVC_InitListX, %function
MVC_InitListX:
	stp	x29, x30, [sp, -480]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 11120640
	add	x20, x19, 11116544
	bl	_mcount
	add	x26, x19, 11157504
	str	wzr, [x21, 360]
	str	wzr, [x21, 364]
	ldrb	w0, [x20, 2808]
	cbnz	w0, .L1072
	ldrb	w0, [x26, 1963]
	add	x27, x19, 11124736
	cbnz	w0, .L1215
	ldr	w0, [x27, 768]
	cbz	w0, .L1152
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 1388544
	add	x0, x27, 504
	add	x3, x3, 2112
	add	x3, x19, x3, lsl 3
	b	.L1078
	.p2align 3
.L1077:
	cmp	x0, x3
	beq	.L1076
.L1078:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 3]
	cmp	w2, 3
	bne	.L1077
	ldr	w2, [x1, 696]
	and	w2, w2, 16776960
	cmp	w2, 65536
	bne	.L1077
	add	x2, x19, x23, uxtw 3
	add	x1, x1, 696
	cmp	x0, x3
	add	w23, w23, 1
	str	x1, [x2, 288]
	bne	.L1078
	.p2align 2
.L1076:
	add	x22, x19, 288
	ldr	x3, .LC69
	mov	w1, w23
	mov	w2, 8
	mov	x0, x22
	bl	qsort
	str	w23, [x20, 2856]
	ldr	w1, [x27, 772]
	cbz	w1, .L1153
	sub	w2, w1, #1
	add	x0, x27, 632
	add	x2, x2, 1388544
	mov	w24, w23
	add	x2, x2, 2128
	add	x2, x19, x2, lsl 3
	b	.L1081
	.p2align 3
.L1080:
	cmp	x2, x0
	beq	.L1216
.L1081:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w3, [x1, 3]
	cmp	w3, 3
	bne	.L1080
	ldr	w3, [x1, 696]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L1080
	add	x3, x19, x24, uxtw 3
	add	x1, x1, 696
	cmp	x2, x0
	add	w24, w24, 1
	str	x1, [x3, 288]
	bne	.L1081
	.p2align 2
.L1216:
	sub	w1, w24, w23
.L1079:
	uxtw	x0, w23
	ldr	x3, .LC70
	add	x0, x0, 36
	mov	w2, 8
	add	x0, x19, x0, lsl 3
	bl	qsort
	str	w24, [x20, 2856]
	ldrsb	w0, [x20, 2820]
	str	wzr, [x20, 2860]
	cbnz	w0, .L1087
.L1131:
	add	x23, x19, 11108352
	mov	x0, x19
	add	x24, x23, 272
	add	x3, x21, 360
	mov	w1, 0
	mov	x2, x24
	bl	mvc_append_interview_list
	ldrb	w0, [x26, 1963]
	cbnz	w0, .L1217
	ldr	w3, [x21, 360]
	ldr	w0, [x20, 2856]
	cbz	w3, .L1156
	add	x1, x23, 968
	add	w3, w0, w3
	.p2align 2
.L1134:
	add	x2, x19, x0, uxtw 3
	add	w0, w0, 1
	cmp	w3, w0
	str	x1, [x2, 288]
	add	x1, x1, 840
	bne	.L1134
.L1133:
	str	w3, [x20, 2856]
.L1135:
	ldrb	w1, [x20, 2808]
	cmp	w1, 1
	beq	.L1218
.L1132:
	cbnz	w1, .L1139
.L1221:
	ldr	w2, [x20, 2856]
	cbz	w2, .L1219
.L1140:
	ldr	w1, [x20, 2868]
	ldr	w0, [x20, 2872]
	ldr	w3, [x20, 2860]
	add	w1, w1, 1
	add	w0, w0, 1
	cmp	w1, w2
	csel	w1, w1, w2, ls
	cmp	w0, w3
	csel	w0, w0, w3, ls
	str	w1, [x20, 2856]
	cmp	w1, 32
	str	w0, [x20, 2860]
	bhi	.L1146
	.p2align 2
.L1178:
	add	x2, x19, x1, uxtw 3
	add	w1, w1, 1
	cmp	w1, 33
	str	xzr, [x2, 288]
	bne	.L1178
.L1146:
	cmp	w0, 32
	bhi	.L1144
	.p2align 2
.L1177:
	add	x1, x19, x0, uxtw 3
	add	w0, w0, 1
	cmp	w0, 33
	str	xzr, [x1, 552]
	bne	.L1177
.L1144:
	mov	w0, 0
.L1205:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 480
	ret
	.p2align 3
.L1072:
	ldrb	w0, [x26, 1963]
	add	x27, x19, 11124736
	cbz	w0, .L1096
	ldr	w0, [x27, 768]
	cbz	w0, .L1220
	sub	w3, w0, #1
	mov	w22, 0
	add	x3, x3, 1388544
	add	x0, x27, 504
	add	x3, x3, 2112
	add	x3, x19, x3, lsl 3
	.p2align 2
.L1115:
	ldr	x2, [x0]
	add	x0, x0, 8
	ldrb	w1, [x2, 2]
	cbz	w1, .L1114
	ldr	w4, [x2, 32]
	add	x1, x29, 480
	ldr	w5, [x26, 2628]
	add	x1, x1, x22, uxtw 3
	sub	x1, x1, #4096
	cmp	w5, w4
	blt	.L1114
	add	w22, w22, 1
	str	x2, [x1, 3712]
.L1114:
	cmp	x0, x3
	bne	.L1115
	add	x24, x29, 96
	ldr	x3, .LC77
	mov	w2, 8
	mov	w1, w22
	mov	x0, x24
	bl	qsort
	ldr	w23, [x27, 768]
	cbz	w23, .L1116
.L1150:
	sub	w1, w23, #1
	add	x0, x27, 504
	add	x1, x1, 1388544
	mov	w28, w22
	add	x1, x1, 2112
	add	x1, x19, x1, lsl 3
	.p2align 2
.L1118:
	ldr	x3, [x0]
	add	x0, x0, 8
	ldrb	w2, [x3, 2]
	cbz	w2, .L1117
	ldr	w4, [x3, 32]
	add	x2, x29, 480
	ldr	w5, [x26, 2628]
	add	x2, x2, x28, uxtw 3
	sub	x2, x2, #4096
	cmp	w5, w4
	bge	.L1117
	add	w28, w28, 1
	str	x3, [x2, 3712]
.L1117:
	cmp	x0, x1
	bne	.L1118
	sub	w23, w28, w22
	ldr	x3, .LC78
	mov	w2, 8
	mov	w1, w23
	add	x0, x24, x22, uxtw 3
	bl	qsort
	cbz	w22, .L1119
.L1149:
	mov	x0, 0
	.p2align 2
.L1120:
	add	w1, w23, w0
	add	x3, x29, 480
	ldr	x2, [x24, x0, lsl 3]
	add	x0, x0, 1
	add	x1, x3, x1, lsl 3
	cmp	w22, w0
	str	x2, [x1, -256]
	bhi	.L1120
.L1119:
	cmp	w28, w22
	bls	.L1121
	mov	w0, w22
	.p2align 2
.L1122:
	add	x2, x29, 480
	sub	w1, w0, w22
	add	x2, x2, x0, uxtw 3
	add	x3, x29, 480
	sub	x2, x2, #4096
	add	x1, x3, x1, lsl 3
	add	w0, w0, 1
	cmp	w28, w0
	ldr	x2, [x2, 3712]
	str	x2, [x1, -256]
	bne	.L1122
.L1121:
	str	wzr, [x20, 2856]
	add	x22, x19, 288
	str	wzr, [x20, 2860]
	add	x25, x20, 2856
	mov	x1, x24
	mov	w2, w28
	ldrb	w0, [x26, 1963]
	mov	w5, 0
	mov	x4, x25
	mov	x3, x22
	add	x23, x19, 552
	add	x24, x20, 2860
	bl	MVC_GenPiclistfromFrmlist
	ldrb	w0, [x26, 1963]
	mov	w2, w28
	mov	w5, 0
	mov	x4, x24
	mov	x3, x23
	add	x1, x29, 224
	bl	MVC_GenPiclistfromFrmlist
	ldr	w28, [x27, 772]
	add	x27, x29, 352
	cbz	w28, .L1123
	mov	x1, 0
	add	x27, x29, 352
	.p2align 2
.L1124:
	lsl	x0, x1, 3
	add	x0, x0, 11124736
	add	x0, x0, 632
	ldr	x0, [x19, x0]
	str	x0, [x27, x1, lsl 3]
	add	x1, x1, 1
	cmp	w28, w1
	bhi	.L1124
.L1123:
	ldr	x3, .LC72
	mov	w2, 8
	mov	w1, w28
	mov	x0, x27
	bl	qsort
	ldrb	w0, [x26, 1963]
	mov	x4, x25
	mov	w5, 1
	mov	x3, x22
	mov	w2, w28
	mov	x1, x27
	bl	MVC_GenPiclistfromFrmlist
	ldrb	w0, [x26, 1963]
	mov	x3, x23
	mov	x1, x27
	mov	w5, 1
	mov	x4, x24
	mov	w2, w28
	bl	MVC_GenPiclistfromFrmlist
	ldr	w23, [x20, 2856]
	ldr	w0, [x20, 2860]
	cmp	w0, w23
	cset	w1, eq
.L1113:
	cmp	w23, 1
	cset	w0, hi
	tst	w1, w0
	beq	.L1125
	cbz	w23, .L1126
	ldr	x4, [x19, 288]
	ldr	x0, [x19, 552]
	cmp	x4, x0
	bne	.L1125
	sub	w23, w23, #1
	mov	x1, x19
	add	x0, x19, x23, lsl 3
.L1128:
	cmp	x0, x1
	beq	.L1130
	ldr	x3, [x1, 296]
	add	x1, x1, 8
	ldr	x2, [x1, 552]
	cmp	x3, x2
	beq	.L1128
	.p2align 2
.L1125:
	ldrsb	w0, [x20, 2820]
	cbz	w0, .L1131
.L1210:
	ldrb	w1, [x20, 2808]
.L1224:
	cbz	w1, .L1221
.L1139:
	ldr	x0, [x20, 2856]
	cbz	x0, .L1141
	ldr	w2, [x20, 2856]
	b	.L1140
	.p2align 3
.L1096:
	ldr	w0, [x27, 768]
	cbz	w0, .L1154
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 1388544
	add	x0, x27, 504
	add	x3, x3, 2112
	add	x3, x19, x3, lsl 3
	b	.L1101
	.p2align 3
.L1100:
	cmp	x0, x3
	beq	.L1099
.L1101:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 3]
	cmp	w2, 3
	bne	.L1100
	ldr	w2, [x1, 720]
	add	x5, x19, x23, uxtw 3
	ldr	w4, [x26, 2632]
	add	x1, x1, 696
	cmp	w4, w2
	blt	.L1100
	str	x1, [x5, 288]
	cmp	x0, x3
	add	w23, w23, 1
	bne	.L1101
	.p2align 2
.L1099:
	add	x22, x19, 288
	ldr	x3, .LC75
	mov	w2, 8
	mov	w1, w23
	mov	x0, x22
	bl	qsort
	ldr	w24, [x27, 768]
	cbz	w24, .L1102
	sub	w1, w24, #1
	add	x0, x27, 504
	add	x1, x1, 1388544
	mov	w25, w23
	add	x1, x1, 2112
	add	x1, x19, x1, lsl 3
	b	.L1104
	.p2align 3
.L1103:
	cmp	x0, x1
	beq	.L1222
.L1104:
	ldr	x2, [x0]
	add	x0, x0, 8
	ldrb	w3, [x2, 3]
	cmp	w3, 3
	bne	.L1103
	ldr	w3, [x2, 720]
	add	x5, x19, x25, uxtw 3
	ldr	w4, [x26, 2632]
	add	x2, x2, 696
	cmp	w4, w3
	bge	.L1103
	str	x2, [x5, 288]
	cmp	x0, x1
	add	w25, w25, 1
	bne	.L1104
	.p2align 2
.L1222:
	uxtw	x0, w23
	sub	w24, w25, w23
	add	x0, x0, 36
	ldr	x3, .LC76
	uxtw	x28, w25
	mov	w2, 8
	add	x0, x19, x0, lsl 3
	mov	w1, w24
	add	x28, x28, 36
	bl	qsort
	add	x28, x19, x28, lsl 3
	cbz	w23, .L1106
.L1148:
	mov	x2, x22
	mov	w0, 0
	.p2align 2
.L1107:
	add	w1, w24, w0
	ldr	x3, [x2], 8
	add	x1, x1, 69
	add	w0, w0, 1
	cmp	w0, w23
	str	x3, [x19, x1, lsl 3]
	bcc	.L1107
.L1106:
	cmp	w25, w23
	bls	.L1108
	mov	w0, w23
	.p2align 2
.L1109:
	sub	w1, w0, w23
	add	x2, x19, x0, uxtw 3
	add	x1, x1, 69
	add	w0, w0, 1
	cmp	w0, w25
	ldr	x2, [x2, 288]
	str	x2, [x19, x1, lsl 3]
	bne	.L1109
.L1108:
	str	w25, [x20, 2856]
	str	w25, [x20, 2860]
	ldr	w1, [x27, 772]
	cbz	w1, .L1155
	sub	w2, w1, #1
	add	x0, x27, 632
	add	x2, x2, 1388544
	mov	w23, w25
	add	x2, x2, 2128
	add	x2, x19, x2, lsl 3
	b	.L1112
	.p2align 3
.L1111:
	cmp	x0, x2
	beq	.L1223
.L1112:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w3, [x1, 2]
	cmp	w3, 3
	bne	.L1111
	ldrb	w3, [x1, 697]
	cmp	w3, 1
	bne	.L1111
	add	x3, x19, x23, uxtw 3
	add	x1, x1, 696
	cmp	x0, x2
	add	w23, w23, 1
	str	x1, [x3, 288]
	str	x1, [x3, 552]
	bne	.L1112
	.p2align 2
.L1223:
	sub	w1, w23, w25
.L1110:
	ldr	x24, .LC70
	mov	x0, x28
	mov	w2, 8
	mov	x3, x24
	bl	qsort
	ldr	w0, [x20, 2856]
	mov	x3, x24
	mov	w2, 8
	sub	w1, w23, w0
	uxtw	x0, w0
	add	x0, x0, 69
	add	x0, x19, x0, lsl 3
	bl	qsort
	str	w23, [x20, 2856]
	mov	w1, 1
	str	w23, [x20, 2860]
	b	.L1113
	.p2align 3
.L1215:
	ldr	w0, [x27, 768]
	mov	w24, 0
	cbz	w0, .L1075
	sub	w4, w0, #1
	mov	w24, 0
	add	x4, x4, 1388544
	add	x0, x27, 504
	add	x4, x4, 2112
	add	x4, x19, x4, lsl 3
	.p2align 2
.L1084:
	ldr	x2, [x0]
	add	x1, x29, 480
	add	x1, x1, x24, uxtw 3
	add	x0, x0, 8
	sub	x1, x1, #4096
	cmp	x4, x0
	ldrb	w3, [x2, 3]
	cbz	w3, .L1083
	add	w24, w24, 1
	str	x2, [x1, 3712]
.L1083:
	bne	.L1084
.L1075:
	ldr	x3, .LC71
	mov	w1, w24
	mov	w2, 8
	add	x0, x29, 96
	add	x22, x19, 288
	add	x23, x20, 2856
	bl	qsort
	str	wzr, [x20, 2856]
	mov	w2, w24
	mov	w5, 0
	ldrb	w0, [x26, 1963]
	add	x1, x29, 96
	mov	x4, x23
	mov	x3, x22
	bl	MVC_GenPiclistfromFrmlist
	ldr	w24, [x27, 772]
	add	x27, x29, 352
	cbz	w24, .L1085
	mov	x1, 0
	add	x27, x29, 352
	.p2align 2
.L1086:
	lsl	x0, x1, 3
	add	x0, x0, 11124736
	add	x0, x0, 632
	ldr	x0, [x19, x0]
	str	x0, [x27, x1, lsl 3]
	add	x1, x1, 1
	cmp	w24, w1
	bhi	.L1086
.L1085:
	ldr	x3, .LC72
	mov	w2, 8
	mov	w1, w24
	mov	x0, x27
	bl	qsort
	ldrb	w0, [x26, 1963]
	mov	w5, 1
	mov	x4, x23
	mov	x3, x22
	mov	w2, w24
	mov	x1, x27
	bl	MVC_GenPiclistfromFrmlist
	str	wzr, [x20, 2860]
	ldrsb	w0, [x20, 2820]
	cbz	w0, .L1131
.L1087:
	ldr	w0, [x20, 2856]
	cbnz	w0, .L1210
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L1091
	ldrb	w0, [x26, 1963]
	cbnz	w0, .L1092
	ldr	x0, [x26, 2600]
	add	x0, x0, 696
	str	x0, [x19, 288]
.L1093:
	mov	w0, 1
	ldrb	w1, [x20, 2808]
	str	w0, [x20, 2856]
	b	.L1224
.L1217:
	ldr	w2, [x21, 360]
	add	x4, x20, 2856
	mov	x3, x22
	mov	x1, x24
	bl	MVC_GenPiclistfromFrmlist_Interview
	b	.L1135
.L1126:
	ldr	x4, [x19, 552]
.L1130:
	ldr	x0, [x19, 560]
	str	x0, [x19, 552]
	str	x4, [x19, 560]
	b	.L1125
.L1092:
	cmp	w0, 1
	ldr	x0, [x26, 2600]
	beq	.L1225
	add	x0, x0, 792
	str	x0, [x19, 288]
	b	.L1093
.L1218:
	add	x22, x23, 1952
	mov	x0, x19
	add	x3, x21, 364
	mov	x2, x22
	bl	mvc_append_interview_list
	ldrb	w0, [x26, 1963]
	cbnz	w0, .L1136
	ldr	w3, [x21, 364]
	ldr	w1, [x20, 2860]
	cbz	w3, .L1157
	add	x0, x23, 2648
	add	w3, w3, w1
	.p2align 2
.L1138:
	add	x2, x19, x1, uxtw 3
	add	w1, w1, 1
	cmp	w3, w1
	str	x0, [x2, 552]
	add	x0, x0, 840
	bne	.L1138
.L1137:
	ldrb	w1, [x20, 2808]
	str	w3, [x20, 2860]
	b	.L1132
.L1156:
	mov	w3, w0
	b	.L1133
.L1225:
	add	x0, x0, 744
	str	x0, [x19, 288]
	b	.L1093
.L1153:
	mov	w24, w23
	b	.L1079
.L1102:
	uxtw	x28, w23
	ldr	x3, .LC76
	add	x28, x28, 36
	mov	w2, 8
	mov	w1, 0
	mov	w25, w23
	add	x28, x19, x28, lsl 3
	mov	x0, x28
	bl	qsort
	cbnz	w23, .L1148
	b	.L1108
	.p2align 3
.L1155:
	mov	w23, w25
	b	.L1110
.L1152:
	mov	w23, 0
	b	.L1076
.L1220:
	add	x24, x29, 96
	ldr	x3, .LC77
	mov	w2, 8
	mov	w1, 0
	mov	x0, x24
	mov	w22, 0
	bl	qsort
	ldr	w23, [x27, 768]
	cbnz	w23, .L1150
	ldr	x3, .LC78
	mov	w2, 8
	mov	w1, 0
	mov	x0, x24
	mov	w28, 0
	bl	qsort
	b	.L1121
	.p2align 3
.L1154:
	mov	w23, 0
	b	.L1099
.L1116:
	ldr	x3, .LC78
	mov	w2, 8
	mov	w1, 0
	add	x0, x24, x22, uxtw 3
	mov	w28, w22
	bl	qsort
	cbnz	w22, .L1149
	b	.L1121
	.p2align 3
.L1136:
	ldr	w2, [x21, 364]
	mov	x1, x22
	add	x4, x20, 2860
	add	x3, x19, 552
	bl	MVC_GenPiclistfromFrmlist_Interview
	ldrb	w1, [x20, 2808]
	b	.L1224
.L1141:
	ldr	x1, .LC82
.L1211:
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_ClearCurrSlice
	mov	w0, -1
	b	.L1205
.L1219:
	ldr	x1, .LC80
	b	.L1211
.L1091:
	ldr	x1, .LC74
	b	.L1211
.L1157:
	mov	w3, w1
	b	.L1137
	.size	MVC_InitListX, .-MVC_InitListX
	.align	3
.LC69:
	.xword	MVC_compare_pic_by_pic_num_desc
	.align	3
.LC70:
	.xword	MVC_compare_pic_by_lt_pic_num_asc
	.align	3
.LC71:
	.xword	MVC_compare_fs_by_frame_num_desc
	.align	3
.LC72:
	.xword	MVC_compare_fs_by_lt_pic_idx_asc
	.align	3
.LC74:
	.xword	.LC73
	.align	3
.LC75:
	.xword	MVC_compare_pic_by_poc_desc
	.align	3
.LC76:
	.xword	MVC_compare_pic_by_poc_asc
	.align	3
.LC77:
	.xword	MVC_compare_fs_by_poc_desc
	.align	3
.LC78:
	.xword	MVC_compare_fs_by_poc_asc
	.align	3
.LC80:
	.xword	.LC79
	.align	3
.LC82:
	.xword	.LC81
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DumpList
	.type	MVC_DumpList, %function
MVC_DumpList:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC83
	ldr	w0, [x0]
	tbz	x0, 13, .L1226
	add	x24, x22, 11116544
	ldr	x23, .LC85
	mov	w20, 0
	add	x24, x24, 2856
.L1228:
	ldr	w0, [x24]
	cbz	w0, .L1231
	uxtw	x0, w20
	mov	w19, 0
	lsl	x21, x0, 5
	add	x21, x21, x0
	.p2align 2
.L1229:
	add	x4, x21, x19, uxtw
	mov	w3, w19
	add	x4, x4, 36
	mov	w2, w20
	mov	x1, x23
	mov	w0, 13
	add	w19, w19, 1
	ldr	x4, [x22, x4, lsl 3]
	ldr	x4, [x4, 8]
	ldr	w5, [x4, 32]
	ldr	w4, [x4, 20]
	bl	dprint_vfmw
	ldr	w0, [x24]
	cmp	w0, w19
	bhi	.L1229
.L1231:
	add	w20, w20, 1
	add	x24, x24, 4
	cmp	w20, 2
	bne	.L1228
.L1226:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	MVC_DumpList, .-MVC_DumpList
	.align	3
.LC83:
	.xword	g_PrintEnable
	.align	3
.LC85:
	.xword	.LC84
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FindNearestPOCPicId
	.type	MVC_FindNearestPOCPicId, %function
MVC_FindNearestPOCPicId:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 11124736
	ldr	w0, [x2, 764]
	cbz	w0, .L1243
	add	x1, x19, 11157504
	ldr	w0, [x2, 760]
	ldr	w7, [x1, 2628]
	cbz	w0, .L1244
	sub	w1, w0, #1
	add	x2, x2, 376
	add	x1, x1, 1388544
	mov	w6, 2147483647
	add	x1, x1, 2096
	mov	w0, 0
	add	x5, x19, x1, lsl 3
	.p2align 2
.L1242:
	ldr	x3, [x2]
	add	x2, x2, 8
	cbz	x3, .L1241
	ldr	w1, [x3, 32]
	sub	w1, w7, w1
	sxtw	x1, w1
	eor	x4, x1, x1, asr 63
	sub	x4, x4, x1, asr 63
	cmp	w4, w6
	bge	.L1241
	ldr	w0, [x3, 228]
	mov	w6, w4
.L1241:
	cmp	x2, x5
	bne	.L1242
.L1240:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1244:
	mov	w0, 0
	b	.L1240
.L1243:
	mov	w0, -1
	b	.L1240
	.size	MVC_FindNearestPOCPicId, .-MVC_FindNearestPOCPicId
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FindMinRefIdx
	.type	MVC_FindMinRefIdx, %function
MVC_FindMinRefIdx:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 11116544
	ldrb	w1, [x0, 2808]
	cmp	w1, 2
	beq	.L1249
	add	x1, x19, 11157504
	ldrb	w1, [x1, 1963]
	cbnz	w1, .L1279
	add	x6, x19, 11124736
	ldr	w5, [x0, 2856]
	add	x6, x6, 1000
	mov	w4, 0
	.p2align 2
.L1253:
	cbz	w5, .L1257
	add	x1, x19, 288
	mov	w3, 32
	mov	w0, 0
	b	.L1255
	.p2align 3
.L1254:
	add	w0, w0, 1
	cmp	w0, w5
	beq	.L1280
.L1255:
	ldr	x2, [x1]
	add	x1, x1, 8
	ldr	x2, [x2, 8]
	ldr	w2, [x2, 52]
	cmp	w2, w4
	bne	.L1254
	cmp	w3, w0
	csel	w3, w3, w0, ls
	add	w0, w0, 1
	stp	w3, w3, [x6]
	cmp	w0, w5
	bne	.L1255
.L1280:
	cmp	w3, 31
	bhi	.L1257
.L1256:
	add	w4, w4, 1
	add	x6, x6, 8
	cmp	w4, 16
	bne	.L1253
.L1249:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1279:
	ldr	w6, [x0, 2856]
	add	x7, x19, 11124736
	add	x7, x7, 1000
	mov	w4, 0
	cbz	w6, .L1261
	.p2align 2
.L1282:
	add	x2, x19, 288
	mov	w5, 32
	mov	w1, 0
	b	.L1259
	.p2align 3
.L1258:
	add	w1, w1, 1
	cmp	w1, w6
	beq	.L1281
.L1259:
	ldr	x0, [x2]
	add	x2, x2, 8
	ldr	x3, [x0, 8]
	ldrb	w0, [x0]
	cmp	w0, 2
	ldr	w0, [x3, 52]
	cset	w3, eq
	orr	w0, w3, w0, lsl 1
	cmp	w0, w4
	bne	.L1258
	cmp	w5, w1
	csel	w5, w5, w1, ls
	add	w1, w1, 1
	str	w5, [x7]
	cmp	w1, w6
	bne	.L1259
.L1281:
	cmp	w5, 31
	bhi	.L1261
.L1260:
	add	w4, w4, 1
	add	x7, x7, 4
	cmp	w4, 32
	beq	.L1249
	cbnz	w6, .L1282
	.p2align 2
.L1261:
	str	wzr, [x7]
	b	.L1260
	.p2align 3
.L1257:
	stp	wzr, wzr, [x6]
	b	.L1256
	.size	MVC_FindMinRefIdx, .-MVC_FindMinRefIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecList
	.type	MVC_DecList, %function
MVC_DecList:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 11124736
	mov	w6, -1
	ldr	w0, [x1, 764]
	cbz	w0, .L1284
	add	x2, x19, 11157504
	ldr	w0, [x1, 760]
	ldr	w7, [x2, 2628]
	cbz	w0, .L1292
	add	w4, w0, w6
	add	x1, x1, 376
	add	x4, x4, 1388544
	mov	w6, 0
	add	x4, x4, 2096
	mov	w5, 2147483647
	add	x4, x19, x4, lsl 3
	.p2align 2
.L1286:
	ldr	x2, [x1]
	add	x1, x1, 8
	cbz	x2, .L1285
	ldr	w0, [x2, 32]
	sub	w0, w7, w0
	sxtw	x0, w0
	eor	x3, x0, x0, asr 63
	sub	x3, x3, x0, asr 63
	cmp	w3, w5
	bge	.L1285
	ldr	w6, [x2, 228]
	mov	w5, w3
.L1285:
	cmp	x1, x4
	bne	.L1286
.L1284:
	add	x21, x19, 11120640
	add	x0, x19, 11116544
	mov	w20, -1
	str	w6, [x21, 348]
	str	w20, [x21, 344]
	ldrb	w1, [x0, 2808]
	cmp	w1, 2
	beq	.L1300
	mov	x0, x19
	bl	MVC_InitListX
	cbnz	w0, .L1301
	mov	x0, x19
	bl	MVC_ReorderListX
	mov	x0, x19
	bl	MVC_RepairList
	cbnz	w0, .L1293
	mov	x0, x19
	bl	MVC_FindMinRefIdx
	ldr	x0, [x19, 288]
	cbz	x0, .L1290
	ldr	x0, [x0, 8]
	ldr	w20, [x0, 228]
.L1290:
	mov	w0, 0
	str	w20, [x21, 344]
.L1288:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1300:
	str	wzr, [x0, 2856]
	str	wzr, [x0, 2860]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1293:
	mov	w0, w20
	b	.L1288
.L1292:
	mov	w6, 0
	b	.L1284
.L1301:
	ldr	x1, .LC87
	mov	w0, 13
	bl	dprint_vfmw
	mov	w0, w20
	b	.L1288
	.size	MVC_DecList, .-MVC_DecList
	.align	3
.LC87:
	.xword	.LC86
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_NoPicOut
	.type	MVC_NoPicOut, %function
MVC_NoPicOut:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 48]
	cbz	w0, .L1309
	mov	x1, 51127
	sub	w3, w0, #1
	mov	w2, 840
	movk	x1, 0xa9, lsl 16
	add	x0, x19, 11124736
	umaddl	x2, w3, w2, x1
	add	x0, x0, 1135
	add	x2, x19, x2
	b	.L1308
	.p2align 3
.L1316:
	ldrb	w3, [x0, -2]
	cmp	w3, 1
	beq	.L1306
	add	x0, x0, 840
	cmp	x0, x2
	beq	.L1309
.L1308:
	ldrb	w1, [x0]
	cmp	w1, 1
	bne	.L1316
.L1306:
	strb	wzr, [x0, -5]
	strb	wzr, [x0, -4]
	add	x0, x0, 840
	cmp	x0, x2
	bne	.L1308
.L1309:
	ldr	w0, [x19, 52]
	cbz	w0, .L1302
	sub	w1, w0, #1
	add	x0, x19, 148
	add	x1, x1, 38
	add	x1, x19, x1, lsl 2
	.p2align 2
.L1310:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L1310
.L1302:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_NoPicOut, .-MVC_NoPicOut
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetBackPicFromVOQueue
	.type	MVC_GetBackPicFromVOQueue, %function
MVC_GetBackPicFromVOQueue:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x22, 120]
	bl	FSP_GetFspType
	cbz	w0, .L1334
.L1318:
	ldr	w0, [x22, 48]
	mov	w23, 0
	mov	w21, 840
	cbz	w0, .L1325
	.p2align 2
.L1329:
	uxtw	x1, w23
	lsl	x0, x1, 3
	sub	x1, x0, x1
	lsl	x0, x1, 4
	sub	x0, x0, x1
	umull	x20, w23, w21
	add	x0, x22, x0, lsl 3
	add	x0, x0, 11124736
	add	x20, x20, 11124736
	add	x19, x20, 1760
	add	x20, x20, 1792
	add	x19, x22, x19
	add	x20, x22, x20
	strb	wzr, [x0, 1130]
	strb	wzr, [x0, 1131]
.L1323:
	ldr	x1, [x19]
	cbnz	x1, .L1335
.L1322:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L1323
	ldr	w0, [x22, 48]
	add	w23, w23, 1
	cmp	w0, w23
	bhi	.L1329
.L1325:
	ldr	w0, [x22, 52]
	cbz	w0, .L1317
	sub	w1, w0, #1
	add	x0, x22, 148
	add	x1, x1, 38
	add	x22, x22, x1, lsl 2
	.p2align 2
.L1326:
	str	wzr, [x0], 4
	cmp	x0, x22
	bne	.L1326
.L1317:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1335:
	ldr	w0, [x22, 120]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L1322
.L1334:
	add	x0, x22, 912
	bl	ResetVoQueue
	b	.L1318
	.size	MVC_GetBackPicFromVOQueue, .-MVC_GetBackPicFromVOQueue
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_wait_vo
	.type	mvc_wait_vo, %function
mvc_wait_vo:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	mvc_wait_vo, .-mvc_wait_vo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_RoundLog2
	.type	MVC_RoundLog2, %function
MVC_RoundLog2:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	mul	w2, w19, w19
	mov	w0, 0
	mov	w3, 1
	.p2align 2
.L1338:
	add	w0, w0, 1
	lsl	w1, w3, w0
	cmp	w2, w1
	bge	.L1338
	asr	w0, w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_RoundLog2, .-MVC_RoundLog2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetReRangeFlag
	.type	MVC_GetReRangeFlag, %function
MVC_GetReRangeFlag:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x22, x20, 11116544
	mov	x19, x1
	bl	_mcount
	mov	w0, 2240
	ldr	x3, [x20, 280]
	ldr	w1, [x22, 2824]
	ldrsb	w2, [x22, 2820]
	umull	x0, w1, w0
	cmn	w2, #1
	add	x0, x3, x0
	beq	.L1357
	cbnz	w2, .L1343
	ldrb	w1, [x20, 2]
	cmp	w1, 1
	beq	.L1358
	ldr	w0, [x0, 28]
	ldr	w1, [x20, 28]
	cmp	w1, w0
	beq	.L1345
	ldr	x1, .LC89
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1343:
	ldr	x1, .LC91
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1357:
	ldr	w0, [x0, 28]
	mov	w21, 3992
	ldr	x1, [x20, 272]
	umull	x0, w0, w21
	add	x21, x1, x0
.L1342:
	ldrb	w0, [x21, 20]
	mov	w2, 2
	ldr	w1, [x21, 3952]
	sub	w0, w2, w0
	ldr	w3, [x21, 3948]
	ldp	w6, w5, [x20, 12]
	madd	w1, w1, w0, w0
	add	w2, w3, 1
	cmp	w6, w2
	ccmp	w5, w1, 0, eq
	bne	.L1347
	add	x0, x20, 11124736
	ldr	w3, [x21, 3972]
	mov	w4, 0
	ldr	w0, [x0, 760]
	add	w0, w0, 1
	cmp	w3, w0
	bhi	.L1347
.L1348:
	ldrb	w0, [x22, 2819]
	cmp	w0, 1
	beq	.L1359
.L1352:
	ldr	w1, [x21, 3972]
	sub	w1, w1, #1
.L1351:
	ldr	w2, [x20, 44]
	mov	w0, w4
	cmp	w2, w1
	cset	w2, ne
	orr	w4, w2, w4
	str	w4, [x19]
	str	w1, [x20, 44]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1347:
	ldr	x0, .LC92
	ldr	x4, [x0]
	cbz	x4, .L1354
	ubfiz	w2, w2, 4, 12
	ubfiz	w1, w1, 4, 12
	ldr	w0, [x20, 120]
	ubfiz	w6, w6, 4, 12
	ubfiz	w5, w5, 4, 12
	strh	w2, [x29, 60]
	strh	w1, [x29, 62]
	mov	w3, 8
	strh	w6, [x29, 56]
	add	x2, x29, 56
	strh	w5, [x29, 58]
	mov	w1, 2
	blr	x4
	ldrb	w0, [x22, 2819]
	mov	w4, 1
	cmp	w0, 1
	bne	.L1352
.L1359:
	add	x0, x20, 290816
	add	x3, x20, 11051008
	add	x0, x0, 2488
	add	x3, x3, 56
	mov	w2, 0
	.p2align 2
.L1350:
	ldr	w1, [x0]
	add	x0, x0, 335872
	add	x0, x0, 308
	cmp	w1, w2
	sub	w1, w1, #1
	csel	w2, w1, w2, hi
	cmp	x0, x3
	bne	.L1350
	add	w0, w2, 1
	mov	w1, 16
	lsl	w0, w0, 1
	cmp	w0, w1
	csel	w1, w0, w1, cc
	b	.L1351
	.p2align 3
.L1345:
	add	x21, x20, 11046912
	add	x21, x21, 180
	b	.L1342
	.p2align 3
.L1358:
	ldr	w1, [x0, 28]
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 2
	sub	x0, x0, x1
	lsl	x0, x0, 4
	sub	x0, x0, x1
	lsl	x21, x0, 2
	sub	x21, x21, x0
	lsl	x1, x21, 6
	add	x21, x21, x1
	add	x21, x20, x21, lsl 2
	add	x21, x21, 286720
	add	x21, x21, 2612
	b	.L1342
	.p2align 3
.L1354:
	mov	w4, 1
	b	.L1348
	.size	MVC_GetReRangeFlag, .-MVC_GetReRangeFlag
	.align	3
.LC89:
	.xword	.LC88
	.align	3
.LC91:
	.xword	.LC90
	.align	3
.LC92:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecPOC
	.type	MVC_DecPOC, %function
MVC_DecPOC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x2, [x19, 248]
	mov	w4, 1
	add	x0, x19, 11116544
	ldr	w3, [x2, 2900]
	ldr	w1, [x2, 2896]
	cmp	w3, w4
	ldrb	w5, [x0, 2811]
	add	w1, w1, 4
	lsl	w1, w4, w1
	beq	.L1362
	cbz	w3, .L1363
	cmp	w3, 2
	bne	.L1360
	cmp	w5, 5
	beq	.L1417
	ldrsb	w2, [x0, 1672]
	cbz	w2, .L1396
	ldr	w3, [x0, 1728]
	mov	w2, 0
	mov	w1, 0
	str	wzr, [x0, 1724]
	str	wzr, [x0, 1736]
.L1397:
	str	w1, [x0, 1732]
.L1399:
	ldrb	w4, [x0, 2816]
	add	w1, w3, w1
	str	w1, [x0, 1720]
	lsl	w1, w1, 1
	cbnz	w4, .L1400
	sub	w1, w1, #1
	str	w1, [x0, 1708]
.L1401:
	ldrb	w3, [x0, 2809]
	cbnz	w3, .L1402
	str	w1, [x0, 1696]
	str	w1, [x0, 1700]
	str	w1, [x0, 1704]
.L1395:
	ldr	w1, [x0, 1728]
	str	w1, [x0, 1724]
	str	w2, [x0, 1736]
.L1360:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1363:
	ldr	w1, [x2, 2904]
	cmp	w5, 5
	add	w1, w1, 4
	lsl	w4, w4, w1
	beq	.L1369
	ldrsb	w1, [x0, 1672]
	cbnz	w1, .L1367
	ldr	w5, [x0, 1684]
	ldr	w3, [x0, 1688]
	mov	w2, w5
.L1368:
	ldr	w1, [x0, 1676]
	cmp	w1, w3
	bcs	.L1366
	sub	w6, w3, w1
	cmp	w6, w4, lsr 1
	bcc	.L1366
	add	w2, w2, w4
	str	w2, [x0, 1692]
.L1370:
	ldrb	w3, [x0, 2809]
	cbnz	w3, .L1372
.L1420:
	ldr	w3, [x0, 1680]
	add	w1, w1, w2
	str	w1, [x0, 1696]
	add	w2, w1, w3
	str	w2, [x0, 1700]
	cmp	w1, w2
	csel	w1, w1, w2, le
	str	w1, [x0, 1708]
.L1373:
	ldr	w3, [x0, 1724]
	ldr	w2, [x0, 1728]
	str	w1, [x0, 1704]
	cmp	w2, w3
	beq	.L1375
	str	w2, [x0, 1724]
.L1375:
	ldrb	w1, [x0, 2816]
	cbz	w1, .L1360
	ldr	w2, [x0, 1676]
	ldr	w1, [x0, 1692]
	str	w1, [x0, 1684]
	str	w2, [x0, 1688]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1362:
	cmp	w5, 5
	beq	.L1418
	ldrsb	w3, [x0, 1672]
	cbz	w3, .L1378
	mov	w1, 0
	str	wzr, [x0, 1724]
	str	wzr, [x0, 1736]
.L1379:
	str	w1, [x0, 1732]
.L1377:
	ldr	w1, [x2, 2916]
	cbz	w1, .L1381
	ldr	w1, [x0, 1728]
	ldr	w5, [x0, 1732]
	ldrb	w6, [x0, 2816]
	add	w5, w5, w1
	str	w5, [x0, 1720]
	cbnz	w6, .L1383
	cbz	w5, .L1384
	sub	w5, w5, #1
	str	w5, [x0, 1720]
.L1383:
	str	wzr, [x0, 1752]
	ldr	w1, [x2, 2916]
	cmp	w1, wzr
	ble	.L1385
.L1404:
	mov	w4, 0
	mov	w3, 0
	.p2align 2
.L1386:
	add	x1, x2, x3, sxtw 2
	add	w3, w3, 1
	ldr	w1, [x1, 2920]
	add	w4, w4, w1
	str	w4, [x0, 1752]
	ldr	w1, [x2, 2916]
	cmp	w1, w3
	bgt	.L1386
.L1385:
	cbz	w5, .L1387
	sub	w5, w5, #1
	ldr	w7, [x0, 1752]
	udiv	w1, w5, w1
	str	w1, [x0, 1744]
	ldr	w4, [x2, 2916]
	udiv	w3, w5, w4
	mul	w1, w1, w7
	str	w1, [x0, 1748]
	msub	w5, w3, w4, w5
	str	w5, [x0, 1740]
	tbnz	w5, #31, .L1388
	mov	w3, 0
	.p2align 2
.L1389:
	add	x4, x2, x3, sxtw 2
	add	w3, w3, 1
	cmp	w5, w3
	ldr	w4, [x4, 2920]
	add	w1, w1, w4
	str	w1, [x0, 1748]
	bge	.L1389
.L1388:
	cbnz	w6, .L1390
	ldr	w3, [x2, 2908]
	add	w1, w1, w3
	str	w1, [x0, 1748]
.L1390:
	ldrb	w3, [x0, 2809]
	cbnz	w3, .L1391
	ldr	w3, [x0, 1712]
	ldr	w4, [x0, 1716]
	add	w1, w1, w3
	str	w1, [x0, 1696]
	ldr	w2, [x2, 2912]
	add	w2, w1, w2
	add	w2, w2, w4
	str	w2, [x0, 1700]
	cmp	w1, w2
	csel	w1, w1, w2, le
	str	w1, [x0, 1708]
.L1392:
	ldr	w3, [x0, 1728]
	ldr	w2, [x0, 1732]
	str	w1, [x0, 1704]
	str	w3, [x0, 1724]
	str	w2, [x0, 1736]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1367:
	ldrsb	w1, [x0, 1673]
	cbz	w1, .L1419
.L1369:
	ldr	w1, [x0, 1676]
	mov	w2, 0
	mov	w5, 0
	str	wzr, [x0, 1684]
	str	wzr, [x0, 1688]
.L1366:
	cmp	w1, w3
	bls	.L1371
	sub	w3, w1, w3
	cmp	w3, w4, lsr 1
	bls	.L1371
	sub	w2, w2, w4
	str	w2, [x0, 1692]
	b	.L1370
	.p2align 3
.L1417:
	add	x1, x0, 1536
	mov	w2, 0
	stp	wzr, wzr, [x1, 160]
	stp	wzr, wzr, [x1, 168]
	str	wzr, [x0, 1732]
	b	.L1395
	.p2align 3
.L1418:
	str	wzr, [x0, 1732]
	b	.L1377
	.p2align 3
.L1371:
	ldrb	w3, [x0, 2809]
	str	w5, [x0, 1692]
	cbz	w3, .L1420
.L1372:
	ldrb	w3, [x0, 2810]
	add	w1, w1, w2
	cbnz	w3, .L1374
	str	w1, [x0, 1696]
	str	w1, [x0, 1708]
	b	.L1373
	.p2align 3
.L1381:
	ldrb	w6, [x0, 2816]
	str	wzr, [x0, 1720]
.L1384:
	str	wzr, [x0, 1752]
	mov	w5, 0
	ldr	w1, [x2, 2916]
	cmp	w1, wzr
	bgt	.L1404
	.p2align 2
.L1387:
	mov	w1, 0
	str	wzr, [x0, 1748]
	b	.L1388
	.p2align 3
.L1391:
	ldrb	w3, [x0, 2810]
	cbnz	w3, .L1393
	ldr	w2, [x0, 1712]
	add	w1, w1, w2
	str	w1, [x0, 1696]
	str	w1, [x0, 1708]
	b	.L1392
	.p2align 3
.L1396:
	ldr	w2, [x0, 1724]
	ldr	w3, [x0, 1728]
	cmp	w3, w2
	ldr	w2, [x0, 1736]
	bcs	.L1398
	add	w1, w1, w2
	str	w1, [x0, 1732]
	mov	w2, w1
	b	.L1399
	.p2align 3
.L1378:
	ldr	w3, [x0, 1724]
	ldr	w4, [x0, 1728]
	cmp	w4, w3
	bcs	.L1380
	ldr	w3, [x0, 1736]
	add	w1, w3, w1
	str	w1, [x0, 1732]
	b	.L1377
	.p2align 3
.L1402:
	ldrb	w3, [x0, 2810]
	str	w1, [x0, 1704]
	cbnz	w3, .L1403
	str	w1, [x0, 1696]
	b	.L1395
	.p2align 3
.L1400:
	str	w1, [x0, 1708]
	b	.L1401
	.p2align 3
.L1374:
	str	w1, [x0, 1700]
	str	w1, [x0, 1708]
	b	.L1373
	.p2align 3
.L1393:
	ldr	w3, [x2, 2912]
	ldr	w2, [x0, 1712]
	add	w1, w1, w3
	add	w1, w1, w2
	str	w1, [x0, 1700]
	str	w1, [x0, 1708]
	b	.L1392
	.p2align 3
.L1419:
	ldr	w3, [x0, 1696]
	mov	w2, 0
	mov	w5, 0
	str	wzr, [x0, 1684]
	str	w3, [x0, 1688]
	b	.L1368
	.p2align 3
.L1403:
	str	w1, [x0, 1700]
	b	.L1395
.L1380:
	ldr	w1, [x0, 1736]
	b	.L1379
.L1398:
	mov	w1, w2
	b	.L1397
	.size	MVC_DecPOC, .-MVC_DecPOC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_CalcPicNum
	.type	MVC_CalcPicNum, %function
MVC_CalcPicNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x8, x19, 11157504
	ldr	x0, [x19, 248]
	mov	w9, 1
	add	x1, x19, 11124736
	ldr	w0, [x0, 2896]
	ldrb	w2, [x8, 1963]
	add	w0, w0, 4
	lsl	w9, w9, w0
	cbz	w2, .L1422
	ldr	w0, [x1, 768]
	cmp	w2, 1
	cset	w6, eq
	cmp	w2, 2
	cset	w7, eq
	mov	w2, 0
	cbz	w0, .L1442
	.p2align 2
.L1459:
	add	x0, x19, x2, uxtw 3
	add	x0, x0, 11124736
	ldr	x3, [x0, 504]
	ldrb	w4, [x3, 3]
	cbz	w4, .L1436
	ldr	w4, [x3, 20]
	ldr	w5, [x8, 2616]
	cmp	w4, w5
	bls	.L1437
	sub	w4, w4, w9
	str	w4, [x3, 24]
.L1438:
	ldr	x3, [x0, 504]
	ldrb	w4, [x3, 3]
	tbz	x4, 0, .L1439
	ldr	w5, [x3, 744]
	and	w5, w5, 16776960
	cmp	w5, 65536
	beq	.L1469
.L1439:
	tbz	x4, 1, .L1436
	ldr	w0, [x3, 792]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L1470
	.p2align 2
.L1436:
	ldr	w0, [x1, 768]
	add	w2, w2, 1
	cmp	w0, w2
	bhi	.L1459
.L1442:
	ldr	w0, [x1, 772]
	mov	w2, 0
	cbz	w0, .L1421
	.p2align 2
.L1458:
	add	x0, x19, x2, uxtw 3
	add	w2, w2, 1
	add	x0, x0, 11124736
	ldr	x4, [x0, 632]
	ldrb	w3, [x4, 3]
	cbz	w3, .L1444
	tbz	x3, 0, .L1445
	ldr	w5, [x4, 744]
	and	w5, w5, 16776960
	cmp	w5, 256
	beq	.L1471
.L1445:
	tbz	x3, 1, .L1444
	ldr	w0, [x4, 792]
	and	w0, w0, 16776960
	cmp	w0, 256
	beq	.L1472
	.p2align 2
.L1444:
	ldr	w0, [x1, 772]
	cmp	w0, w2
	bhi	.L1458
.L1421:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1437:
	str	w4, [x3, 24]
	b	.L1438
	.p2align 3
.L1422:
	ldr	w2, [x1, 768]
	mov	w0, 0
	cbnz	w2, .L1457
	.p2align 2
.L1432:
	ldr	w0, [x1, 772]
	mov	w2, 0
	cbz	w0, .L1421
	uxtw	x0, w2
	add	w2, w2, 1
	add	x0, x0, 1388544
	add	x0, x19, x0, lsl 3
	ldr	x0, [x0, 17016]
	ldrb	w3, [x0, 3]
	cmp	w3, 3
	beq	.L1473
	.p2align 2
.L1433:
	ldr	w0, [x1, 772]
	cmp	w0, w2
	bls	.L1421
	uxtw	x0, w2
	add	w2, w2, 1
	add	x0, x0, 1388544
	add	x0, x19, x0, lsl 3
	ldr	x0, [x0, 17016]
	ldrb	w3, [x0, 3]
	cmp	w3, 3
	bne	.L1433
.L1473:
	ldr	w3, [x0, 696]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L1433
	ldr	w3, [x0, 28]
	str	w3, [x0, 712]
	b	.L1433
	.p2align 3
.L1475:
	ldr	w4, [x3, 696]
	and	w4, w4, 16776960
	cmp	w4, 65536
	bne	.L1428
	ldr	w4, [x3, 20]
	ldr	w5, [x8, 2616]
	cmp	w4, w5
	sub	w5, w4, w9
	bhi	.L1474
	str	w4, [x3, 24]
.L1430:
	ldr	x2, [x2, 504]
	ldr	w3, [x2, 24]
	str	w3, [x2, 716]
	.p2align 2
.L1428:
	ldr	w2, [x1, 768]
	add	w0, w0, 1
	cmp	w2, w0
	bls	.L1432
.L1457:
	add	x2, x19, x0, uxtw 3
	add	x2, x2, 11124736
	ldr	x3, [x2, 504]
	ldrb	w4, [x3, 3]
	cmp	w4, 3
	bne	.L1428
	b	.L1475
	.p2align 3
.L1471:
	ldr	w3, [x4, 28]
	add	w3, w6, w3, lsl 1
	str	w3, [x4, 760]
	ldr	x4, [x0, 632]
	ldrb	w3, [x4, 3]
	b	.L1445
	.p2align 3
.L1469:
	ldr	w4, [x3, 24]
	add	w4, w6, w4, lsl 1
	str	w4, [x3, 764]
	ldr	x3, [x0, 504]
	ldrb	w4, [x3, 3]
	b	.L1439
	.p2align 3
.L1472:
	ldr	w0, [x4, 28]
	add	w0, w7, w0, lsl 1
	str	w0, [x4, 808]
	b	.L1444
	.p2align 3
.L1470:
	ldr	w0, [x3, 24]
	add	w0, w7, w0, lsl 1
	str	w0, [x3, 812]
	b	.L1436
	.p2align 3
.L1474:
	str	w5, [x3, 24]
	b	.L1430
	.size	MVC_CalcPicNum, .-MVC_CalcPicNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_IsOutDPB
	.type	MVC_IsOutDPB, %function
MVC_IsOutDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L1481
	add	x2, x20, 11124736
	ldr	w0, [x2, 760]
	cbz	w0, .L1481
	ldr	x1, [x2, 376]
	cmp	x19, x1
	ccmp	x1, xzr, 4, eq
	bne	.L1483
	sub	w0, w0, #1
	add	x2, x2, 384
	add	x0, x0, 1388544
	add	x0, x0, 2096
	add	x0, x20, x0, lsl 3
	b	.L1478
	.p2align 3
.L1479:
	ldr	x3, [x2], 8
	cmp	x19, x3
	ccmp	x3, xzr, 4, eq
	bne	.L1483
.L1478:
	cmp	x2, x0
	bne	.L1479
.L1481:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1483:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_IsOutDPB, .-MVC_IsOutDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_combine_scalinglist
	.type	mvc_combine_scalinglist, %function
mvc_combine_scalinglist:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w19, w2
	mov	x20, x1
	bl	_mcount
	cmp	w19, wzr
	ble	.L1484
	ldr	x1, .LC95
	mov	x5, 0
	mov	w2, 255
	b	.L1488
	.p2align 3
.L1486:
	ldrb	w3, [x5, x0]
.L1487:
	lsr	w6, w3, 2
	ldr	w4, [x21, w3, uxtw 2]
	ubfiz	w7, w3, 3, 2
	add	x5, x5, 1
	lsl	w3, w2, w7
	and	w4, w4, 255
	ldr	w0, [x20, w6, uxtw 2]
	lsl	w4, w4, w7
	cmp	w19, w5
	bic	w0, w0, w3
	orr	w0, w4, w0
	str	w0, [x20, w6, uxtw 2]
	ble	.L1484
.L1488:
	cmp	w19, 16
	add	x0, x1, 16
	bne	.L1486
	ldrb	w3, [x1, x5]
	b	.L1487
	.p2align 3
.L1484:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	mvc_combine_scalinglist, .-mvc_combine_scalinglist
	.align	3
.LC95:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_assign_quant_params
	.type	mvc_assign_quant_params, %function
mvc_assign_quant_params:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	cbz	x20, .L1522
	cbz	x19, .L1523
	ldrb	w0, [x19, 18]
	cbz	w0, .L1511
	ldr	x21, .LC100
	add	x0, x19, 2016
	ldr	x22, .LC97
	add	x24, x19, 5
	mov	x26, x0
	add	x25, x19, 1984
	mov	w27, 1
	mov	w28, 0
	mov	x23, x21
	str	x0, [x29, 104]
	b	.L1496
	.p2align 3
.L1526:
	cbz	w28, .L1524
	cmp	w28, 3
	beq	.L1525
	ldr	x8, .LC100
	sub	w1, w28, #1
	add	x1, x1, 126
	mov	x2, 16
	mov	x0, x26
	ldr	x8, [x8, 104]
	add	x1, x19, x1, lsl 4
	blr	x8
.L1504:
	cmp	w27, 7
	bhi	.L1511
.L1510:
	add	w28, w28, 1
	add	w27, w27, 1
	add	x24, x24, 1
	add	x26, x26, 16
	add	x25, x25, 4
.L1496:
	cmp	w28, 5
	ldrsb	w0, [x24]
	bhi	.L1497
	cbz	w0, .L1526
	ldr	w0, [x25]
	cbz	w0, .L1504
	cmp	w28, 2
	ldr	x1, .LC97
	ldr	x8, [x23, 104]
	bls	.L1505
	add	x1, x1, 16
.L1505:
	mov	x2, 16
	mov	x0, x26
	blr	x8
	cmp	w27, 7
	bls	.L1510
	.p2align 2
.L1511:
	mov	w0, 0
.L1493:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1497:
	cbnz	w0, .L1506
	ldrb	w0, [x20, 27]
	cbnz	w0, .L1507
	ldr	x1, .LC100
	sub	w0, w28, #6
	add	x0, x0, 33
	cmp	w28, 6
	ldr	x8, [x1, 104]
	add	x0, x19, x0, lsl 6
	ldr	x1, .LC97
	beq	.L1521
	add	x1, x1, 96
.L1509:
	mov	x2, 64
	blr	x8
	b	.L1504
	.p2align 3
.L1506:
	sub	w0, w28, #6
	add	x1, x19, x0, lsl 2
	ldr	w1, [x1, 2008]
	cbz	w1, .L1504
	add	x0, x0, 33
	cmp	w28, 6
	ldr	x8, [x21, 104]
	add	x0, x19, x0, lsl 6
	beq	.L1514
	add	x1, x22, 96
	b	.L1509
	.p2align 3
.L1507:
	ldr	x0, .LC100
	sub	w1, w28, #6
	mov	x2, 64
	ldr	x8, [x0, 104]
	add	x0, x1, 33
	add	x1, x20, x1, lsl 6
	add	x1, x1, 2768
	add	x0, x19, x0, lsl 6
	blr	x8
	b	.L1504
.L1522:
	ldr	x2, .LC98
	mov	w3, 6950
	ldr	x1, .LC99
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1493
.L1523:
	ldr	x2, .LC98
	mov	w3, 6951
	ldr	x1, .LC99
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1493
.L1524:
	ldrb	w0, [x20, 27]
	cbnz	w0, .L1500
	ldr	x0, .LC100
	mov	x2, 16
	ldr	x1, .LC97
	ldr	x8, [x0, 104]
	ldr	x0, [x29, 104]
	blr	x8
	b	.L1510
.L1525:
	ldrb	w0, [x20, 27]
	cbnz	w0, .L1503
	ldr	x0, .LC100
	mov	x2, 16
	ldr	x1, .LC101
	ldr	x8, [x0, 104]
	add	x0, x19, 2064
	blr	x8
	b	.L1510
.L1500:
	ldr	x0, .LC100
	mov	x2, 16
	add	x1, x20, 2672
	ldr	x8, [x0, 104]
	ldr	x0, [x29, 104]
	blr	x8
	b	.L1510
.L1503:
	ldr	x0, .LC100
	mov	x2, 16
	add	x1, x20, 2720
	ldr	x8, [x0, 104]
	add	x0, x19, 2064
	blr	x8
	b	.L1510
.L1514:
	ldr	x1, .LC97
.L1521:
	add	x1, x1, 32
	b	.L1509
	.size	mvc_assign_quant_params, .-mvc_assign_quant_params
	.align	3
.LC97:
	.xword	.LANCHOR1
	.align	3
.LC98:
	.xword	.LC36
	.align	3
.LC99:
	.xword	.LC38
	.align	3
.LC100:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC101:
	.xword	.LANCHOR1+16
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_WriteCurrPicYUV
	.type	MVC_WriteCurrPicYUV, %function
MVC_WriteCurrPicYUV:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 11157504
	bl	_mcount
	ldr	x1, .LC103
	mov	w0, 2
	ldr	x3, [x19, 2600]
	ldrb	w20, [x19, 1963]
	ldr	w3, [x3, 228]
	mov	w2, w20
	bl	dprint_vfmw
	cmp	w20, 3
	ldr	w3, [x19, 2664]
	ccmp	w20, wzr, 4, ne
	ldr	w4, [x19, 2668]
	cset	w0, eq
	ldr	x1, .LC105
	cmp	w0, wzr
	mov	w2, 1
	csinc	w2, w2, w20, ne
	mov	w0, 22
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_WriteCurrPicYUV, .-MVC_WriteCurrPicYUV
	.align	3
.LC103:
	.xword	.LC102
	.align	3
.LC105:
	.xword	.LC104
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_WritePicMsg
	.type	MVC_WritePicMsg, %function
MVC_WritePicMsg:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x23, x20, 11157504
	add	x19, x20, 11161600
	bl	_mcount
	add	x22, x20, 11108352
	ldr	x1, .LC107
	mov	w0, 2
	ldrb	w3, [x23, 1963]
	add	x25, x20, 11112448
	ldr	w4, [x23, 2132]
	ldr	w2, [x23, 2624]
	ldr	w5, [x23, 2684]
	bl	dprint_vfmw
	str	wzr, [x20, 64]
	add	x0, x20, 11116544
	ldr	x1, [x23, 2576]
	str	x1, [x19, 464]
	ldr	x1, [x20, 864]
	str	x1, [x19, 1464]
	ldr	w1, [x23, 2116]
	str	w1, [x19, 1244]
	ldr	w1, [x23, 2120]
	str	w1, [x19, 1248]
	ldr	w1, [x23, 2124]
	str	w1, [x19, 1252]
	ldr	w1, [x23, 2624]
	str	w1, [x19, 472]
	ldrb	w1, [x23, 1963]
	strb	w1, [x19, 456]
	ldr	x1, [x20, 248]
	ldrb	w2, [x1, 21]
	str	w2, [x19, 488]
	ldr	w2, [x23, 2668]
	str	w2, [x19, 476]
	ldr	w2, [x23, 2672]
	str	w2, [x19, 480]
	ldr	w2, [x1, 3948]
	add	w2, w2, 1
	str	w2, [x19, 484]
	ldrb	w2, [x22, 3652]
	strb	w2, [x19, 459]
	ldr	w2, [x25, 3588]
	str	w2, [x19, 492]
	ldrb	w2, [x25, 3528]
	str	w2, [x19, 496]
	ldr	w1, [x1, 748]
	strb	w1, [x19, 457]
	ldrb	w1, [x25, 3532]
	str	w1, [x19, 500]
	ldr	w1, [x0, 1708]
	str	w1, [x19, 504]
	ldr	w1, [x0, 1696]
	str	w1, [x19, 508]
	ldr	w0, [x0, 1700]
	str	w0, [x19, 512]
	ldr	x1, [x23, 2600]
	ldr	w0, [x20, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetLogicFs
	mov	x24, x0
	cbz	x0, .L1564
	ldr	x2, [x0, 640]
	ldr	x3, [x0, 648]
	cbz	x2, .L1534
	cbz	x3, .L1534
	ldr	w0, [x20, 120]
	add	x3, x19, 892
	add	x2, x19, 764
	add	x1, x19, 1152
	add	x21, x20, 11124736
	bl	FSP_GetDecFsAddrTab
	ldr	x0, [x24, 640]
	add	x2, x19, 1156
	add	x1, x19, 1236
	ldr	w0, [x0, 8]
	str	w0, [x19, 748]
	ldr	w0, [x20, 120]
	bl	FSP_GetPmvAddrTab
	ldr	w0, [x23, 2684]
	str	w0, [x19, 756]
	ldr	w0, [x24, 668]
	str	w0, [x19, 1240]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 16]
	str	w0, [x19, 752]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 76]
	str	w0, [x19, 760]
	ldr	w4, [x21, 992]
	str	w4, [x19, 1456]
	cbz	w4, .L1540
	sub	w3, w4, #1
	add	x0, x21, 800
	add	x3, x3, 2781184
	add	x1, x19, 1264
	add	x3, x3, 201
	add	x3, x20, x3, lsl 2
	.p2align 2
.L1539:
	ldr	w2, [x0]
	add	x0, x0, 4
	str	w2, [x1]
	add	x1, x1, 4
	cmp	x3, x0
	ldr	w2, [x0, 60]
	str	w2, [x1, 60]
	ldr	w2, [x0, 124]
	str	w2, [x1, 124]
	bne	.L1539
.L1540:
	ldrb	w0, [x22, 3659]
	cbnz	w0, .L1565
	ldrb	w0, [x25, 3546]
	cbnz	w0, .L1541
	ldr	x9, .LC112
	add	x5, x19, 516
	mov	w1, 0
	.p2align 2
.L1542:
	and	w2, w1, 3
	add	x0, x9, 352
	add	w1, w1, 1
	cmp	w1, 24
	ldr	w0, [x0, w2, uxtw 2]
	ubfx	x3, x0, 8, 8
	lsr	w2, w0, 16
	ubfiz	w2, w2, 8, 8
	and	w0, w0, -16776961
	orr	w2, w2, w3, lsl 16
	orr	w0, w2, w0
	str	w0, [x5], 4
	bne	.L1542
	add	x19, x19, 612
	mov	w5, 0
	.p2align 2
.L1543:
	add	w0, w5, 1
	and	w2, w5, 14
	add	x1, x9, 368
	and	w0, w0, 15
	add	x19, x19, 8
	add	w5, w5, 2
	cmp	w5, 32
	ldr	w0, [x1, w0, uxtw 2]
	ldr	w1, [x1, w2, uxtw 2]
	lsr	w7, w0, 16
	ubfx	x2, x0, 8, 8
	lsr	w3, w1, 16
	lsr	w6, w1, 24
	ubfiz	w10, w0, 16, 8
	ubfiz	w8, w3, 8, 8
	and	w0, w0, -16777216
	and	w3, w1, 255
	ubfx	x1, x1, 8, 8
	orr	w0, w0, w2, lsl 16
	orr	w7, w10, w7, lsl 24
	orr	w3, w8, w3
	orr	w1, w1, w6, lsl 8
	orr	w2, w7, w3
	orr	w0, w1, w0
	stp	w2, w0, [x19, -8]
	bne	.L1543
.L1549:
	mov	w22, 0
	cbz	w4, .L1545
	.p2align 2
.L1553:
	add	x19, x20, x22, uxtw 2
	ldr	w0, [x20, 120]
	add	x23, x19, 11124736
	mov	w1, 0
	add	x19, x19, 11161600
	add	w22, w22, 1
	ldr	w2, [x23, 800]
	bl	FSP_GetStoreType
	cmp	w0, wzr
	cset	w0, ne
	str	w0, [x19, 1600]
	ldr	w0, [x23, 800]
	str	w0, [x19, 1664]
	ldr	w0, [x21, 992]
	cmp	w0, w22
	bhi	.L1553
.L1545:
	mov	w0, 0
.L1532:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1565:
	ldrb	w0, [x25, 3546]
.L1541:
	cmp	w0, 1
	add	x5, x19, 516
	mov	w1, 0
	beq	.L1547
	.p2align 2
.L1550:
	uxtw	x0, w1
	add	w1, w1, 1
	add	x0, x0, 2777088
	cmp	w1, 24
	add	x0, x0, 1576
	ldr	w0, [x20, x0, lsl 2]
	ubfx	x3, x0, 8, 8
	lsr	w2, w0, 16
	ubfiz	w2, w2, 8, 8
	and	w0, w0, -16776961
	orr	w2, w2, w3, lsl 16
	orr	w0, w2, w0
	str	w0, [x5], 4
	bne	.L1550
	add	x19, x19, 612
	mov	w9, 1
	mov	w5, 0
	.p2align 2
.L1551:
	ubfx	x1, x5, 3, 29
	ubfiz	x0, x5, 1, 3
	and	x2, x9, 15
	lsl	x1, x1, 4
	add	x2, x1, x2
	add	x1, x1, x0
	add	x1, x1, 2777088
	add	x0, x2, 2777088
	add	x0, x0, 1600
	add	x1, x1, 1600
	add	x19, x19, 8
	add	w5, w5, 1
	cmp	w5, 16
	add	w9, w9, 2
	ldr	w0, [x20, x0, lsl 2]
	ldr	w1, [x20, x1, lsl 2]
	lsr	w3, w0, 16
	ubfx	x7, x0, 8, 8
	lsr	w8, w1, 16
	lsr	w6, w1, 24
	ubfiz	w10, w8, 8, 8
	ubfiz	w11, w0, 16, 8
	and	w8, w1, 255
	ubfx	x2, x1, 8, 8
	lsl	w6, w6, 8
	and	w1, w0, -16777216
	orr	w0, w2, w1
	orr	w8, w10, w8
	orr	w3, w11, w3, lsl 24
	orr	w6, w6, w7, lsl 16
	orr	w1, w8, w3
	orr	w0, w6, w0
	stp	w1, w0, [x19, -8]
	bne	.L1551
	b	.L1549
	.p2align 3
.L1547:
	uxtw	x0, w1
	add	w1, w1, 1
	add	x0, x0, 2777088
	cmp	w1, 24
	add	x0, x20, x0, lsl 2
	ldr	w0, [x0, 9640]
	ubfx	x3, x0, 8, 8
	lsr	w2, w0, 16
	ubfiz	w2, w2, 8, 8
	and	w0, w0, -16776961
	orr	w2, w2, w3, lsl 16
	orr	w0, w2, w0
	str	w0, [x5], 4
	bne	.L1547
	add	x19, x19, 612
	mov	w9, 1
	mov	w5, 0
	.p2align 2
.L1548:
	ubfx	x1, x5, 3, 29
	ubfiz	x0, x5, 1, 3
	and	x2, x9, 15
	lsl	x1, x1, 4
	add	x2, x1, x2
	add	x1, x1, x0
	add	x1, x1, 2777088
	add	x0, x2, 2777088
	add	x19, x19, 8
	add	w5, w5, 1
	add	x0, x20, x0, lsl 2
	add	x1, x20, x1, lsl 2
	cmp	w5, 16
	add	w9, w9, 2
	ldr	w0, [x0, 9736]
	ldr	w1, [x1, 9736]
	lsr	w8, w0, 16
	ubfx	x7, x0, 8, 8
	lsr	w3, w1, 16
	lsr	w6, w1, 24
	ubfiz	w11, w0, 16, 8
	ubfiz	w10, w3, 8, 8
	ubfx	x2, x1, 8, 8
	and	w3, w1, 255
	lsl	w6, w6, 8
	and	w1, w0, -16777216
	orr	w0, w2, w1
	orr	w8, w11, w8, lsl 24
	orr	w3, w10, w3
	orr	w6, w6, w7, lsl 16
	orr	w1, w8, w3
	orr	w0, w6, w0
	stp	w1, w0, [x19, -8]
	bne	.L1548
	b	.L1549
.L1534:
	ldr	x1, .LC111
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1532
.L1564:
	ldr	x2, [x23, 2600]
	mov	w0, 0
	ldr	x1, .LC109
	ldrsb	w2, [x2, 6]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1532
	.size	MVC_WritePicMsg, .-MVC_WritePicMsg
	.align	3
.LC107:
	.xword	.LC106
	.align	3
.LC109:
	.xword	.LC108
	.align	3
.LC111:
	.xword	.LC110
	.align	3
.LC112:
	.xword	.LANCHOR1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_UpdatePicQpInf
	.type	MVC_UpdatePicQpInf, %function
MVC_UpdatePicQpInf:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	ldp	w1, w0, [x19, 36]
	cmp	w1, w20
	csel	w1, w1, w20, ge
	cmp	w0, w20
	csel	w0, w0, w20, le
	stp	w1, w0, [x19, 36]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_UpdatePicQpInf, .-MVC_UpdatePicQpInf
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_WriteSliceMsg
	.type	MVC_WriteSliceMsg, %function
MVC_WriteSliceMsg:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w5, [x19, 64]
	ldr	w1, [x19, 32]
	add	w0, w5, 1
	ldr	x6, [x19, 864]
	uxtw	x21, w5
	sub	w1, w1, #1
	lsl	x4, x21, 7
	cmp	w5, w1
	sub	x4, x4, x21
	str	w0, [x19, 64]
	lsl	x4, x4, 5
	add	x22, x6, x4
	bcs	.L1568
	lsl	x1, x0, 7
	sub	x0, x1, x0
	add	x0, x6, x0, lsl 5
	str	x0, [x22, 4056]
.L1569:
	ldr	x3, [x19, 240]
	ldrb	w2, [x3]
	ldr	w0, [x3, 76]
	cmp	w2, 1
	ccmp	w2, w0, 2, ls
	bcs	.L1570
	.p2align 2
.L1571:
	uxtw	x0, w2
	add	w2, w2, 1
	lsl	x1, x0, 5
	add	x3, x3, x1
	add	x0, x22, x0, lsl 2
	ldr	w3, [x3, 28]
	str	w3, [x0, 16]
	ldr	x3, [x19, 240]
	add	x3, x3, x1
	ldr	w3, [x3, 24]
	str	w3, [x0, 8]
	ldr	x3, [x19, 240]
	add	x1, x3, x1
	ldr	w1, [x1, 32]
	str	w1, [x0, 24]
	ldr	x3, [x19, 240]
	ldr	w0, [x3, 76]
	cmp	w0, w2
	ccmp	w2, 1, 2, hi
	bls	.L1571
.L1570:
	cmp	w0, 1
	bhi	.L1576
	.p2align 2
.L1647:
	add	x1, x22, x0, uxtw 2
	add	w0, w0, 1
	cmp	w0, 2
	str	wzr, [x1, 8]
	str	wzr, [x1, 16]
	str	wzr, [x1, 24]
	bne	.L1647
.L1576:
	add	x20, x19, 11157504
	add	x23, x19, 11112448
	add	x1, x19, 11120640
	ldrb	w0, [x20, 1969]
	strb	w0, [x22, 1]
	ldr	w0, [x23, 3576]
	add	w2, w0, 26
	ldr	w0, [x1, 328]
	add	w0, w2, w0
	str	w0, [x22, 32]
	cbnz	w5, .L1672
	ldr	x2, [x20, 2600]
	str	w0, [x2, 732]
	ldr	x0, [x20, 2600]
	ldr	w2, [x22, 32]
	str	w2, [x0, 736]
.L1577:
	ldr	w0, [x1, 324]
	add	x27, x19, 11116544
	str	w0, [x22, 36]
	ldr	w0, [x27, 2872]
	str	w0, [x22, 40]
	ldr	w0, [x27, 2868]
	str	w0, [x22, 44]
	ldrb	w0, [x27, 2808]
	strb	w0, [x6, x4]
	ldr	w0, [x27, 2864]
	str	w0, [x22, 48]
	ldrb	w0, [x27, 2814]
	strb	w0, [x22, 2]
	ldr	x0, [x19, 248]
	ldrb	w0, [x0, 22]
	strb	w0, [x22, 3]
	ldr	w0, [x27, 2856]
	str	w0, [x22, 52]
	ldr	w0, [x27, 2860]
	str	w0, [x22, 56]
	ldrb	w0, [x27, 2808]
	cmp	w0, 1
	beq	.L1673
	cbz	w0, .L1674
	strb	wzr, [x22, 4]
.L1579:
	ldr	w0, [x23, 3580]
	str	w0, [x22, 60]
	ldr	w0, [x23, 3584]
	str	w0, [x22, 64]
	ldr	w0, [x1, 336]
	str	w0, [x22, 68]
	ldr	w0, [x1, 340]
	str	w0, [x22, 72]
	ldr	w0, [x1, 332]
	str	w0, [x22, 76]
	ldrb	w0, [x27, 2808]
	cmp	w0, 2
	beq	.L1618
	ldr	w0, [x27, 2856]
	cbz	w0, .L1621
	ldr	x1, [x19, 288]
	ldr	x1, [x1, 8]
	ldrb	w2, [x1, 1]
	cbz	w2, .L1584
	sub	w3, w0, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L1586
	.p2align 3
.L1588:
	ldr	x1, [x0, 296]
	add	x0, x0, 8
	ldr	x1, [x1, 8]
	ldrb	w2, [x1, 1]
	cbz	w2, .L1584
.L1586:
	cmp	x3, x0
	bne	.L1588
.L1621:
	mov	w24, 0
.L1583:
	ldr	w0, [x27, 2860]
	cbz	w0, .L1622
	ldr	x1, [x19, 552]
	ldr	x1, [x1, 8]
	ldrb	w2, [x1, 1]
	cbz	w2, .L1590
	sub	w3, w0, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L1592
	.p2align 3
.L1594:
	ldr	x1, [x0, 560]
	add	x0, x0, 8
	ldr	x1, [x1, 8]
	ldrb	w2, [x1, 1]
	cbz	w2, .L1590
.L1592:
	cmp	x3, x0
	bne	.L1594
.L1622:
	mov	w25, 0
.L1589:
	ldrb	w0, [x20, 1963]
	cbz	w0, .L1595
	ldr	w0, [x27, 2856]
	mov	w28, 0
	cbnz	w0, .L1646
	b	.L1611
	.p2align 3
.L1676:
	cbz	x0, .L1628
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1607:
	lsl	x0, x20, 5
	add	x0, x0, x20, lsl 2
	add	x0, x22, x0
	str	w1, [x0, 1640]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 52]
	str	w1, [x0, 1644]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 32]
	str	w1, [x0, 1632]
	ldr	x1, [x26, 288]
	ldrb	w1, [x1]
	strb	w1, [x0, 1624]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 696]
	strb	w1, [x0, 1626]
	ldr	x1, [x26, 288]
	ldrb	w1, [x1]
	cmp	w1, 1
	beq	.L1675
	strb	w1, [x0, 1630]
	add	w28, w28, 1
	ldr	x1, [x26, 288]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1631]
	ldr	x1, [x26, 288]
	ldr	w1, [x1, 24]
	str	w1, [x0, 1656]
	ldr	w0, [x27, 2856]
	cmp	w0, w28
	bls	.L1611
.L1646:
	uxtw	x20, w28
	ldr	w0, [x19, 120]
	add	x26, x19, x20, lsl 3
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetLogicFs
	ldr	x2, [x26, 288]
	lsl	x1, x20, 5
	add	x1, x1, x20, lsl 2
	add	x1, x22, x1
	ldr	x2, [x2, 8]
	ldr	w2, [x2, 48]
	str	w2, [x1, 1636]
	ldr	x2, [x26, 288]
	ldr	x2, [x2, 8]
	ldrb	w2, [x2, 2]
	strb	w2, [x1, 1625]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 1]
	cmp	w1, 1
	bne	.L1676
	mov	w1, w24
	b	.L1607
	.p2align 3
.L1568:
	str	xzr, [x22, 4056]
	b	.L1569
	.p2align 3
.L1672:
	ldr	x2, [x20, 2600]
	ldr	w3, [x2, 732]
	cmp	w3, w0
	csel	w3, w3, w0, ge
	str	w3, [x2, 732]
	ldr	w3, [x2, 736]
	cmp	w3, w0
	csel	w0, w3, w0, le
	str	w0, [x2, 736]
	b	.L1577
	.p2align 3
.L1674:
	ldrb	w0, [x23, 3530]
	strb	w0, [x22, 4]
	b	.L1579
	.p2align 3
.L1675:
	strb	w1, [x0, 1628]
	add	w28, w28, 1
	ldr	x1, [x26, 288]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1629]
	ldr	x1, [x26, 288]
	ldr	w1, [x1, 24]
	str	w1, [x0, 1652]
	ldr	w0, [x27, 2856]
	cmp	w0, w28
	bhi	.L1646
.L1611:
	ldrb	w0, [x27, 2808]
	cmp	w0, 1
	beq	.L1677
.L1598:
	add	x1, x19, 11124736
	add	x0, x22, 3928
	add	x1, x1, 1000
	add	x3, x22, 4056
	.p2align 2
.L1604:
	ldr	w2, [x1], 4
	str	w2, [x0], 4
	cmp	x3, x0
	bne	.L1604
	add	x0, x19, 11161600
	ldrb	w1, [x23, 3530]
	str	w1, [x0, 740]
	ldr	w2, [x23, 3572]
	str	w2, [x0, 744]
	cbz	w1, .L1616
	ldrb	w0, [x27, 2808]
	cbnz	w0, .L1616
.L1617:
	ldr	w0, [x27, 2876]
	mov	w2, 0
	str	w0, [x22, 80]
	ldr	w0, [x27, 2880]
	str	w0, [x22, 84]
	.p2align 2
.L1619:
	ubfiz	x0, x2, 2, 32
	add	w2, w2, 1
	add	x1, x19, x0
	add	x0, x22, x0
	add	x1, x1, 11116544
	ldr	w3, [x1, 2884]
	str	w3, [x0, 88]
	ldr	w3, [x1, 3012]
	str	w3, [x0, 216]
	ldr	w3, [x1, 3140]
	str	w3, [x0, 344]
	ldr	w3, [x1, 3652]
	str	w3, [x0, 856]
	ldr	w3, [x1, 3780]
	str	w3, [x0, 984]
	ldr	w1, [x1, 3908]
	str	w1, [x0, 1112]
	ldr	w0, [x27, 2868]
	cmp	w0, w2
	bcs	.L1619
	ldrb	w0, [x27, 2808]
	cmp	w0, 1
	beq	.L1678
	.p2align 2
.L1618:
	add	x21, x21, 1392640
	ldr	x0, [x19, 240]
	add	x21, x19, x21, lsl 3
	str	x0, [x21, 19848]
	str	xzr, [x19, 240]
.L1567:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1628:
	mov	w1, 0
	b	.L1607
	.p2align 3
.L1584:
	ldrsb	w1, [x1, 6]
	ldr	w0, [x19, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L1679
	ldr	x0, [x0, 640]
	ldr	w24, [x0, 8]
	b	.L1583
	.p2align 3
.L1616:
	cmp	w2, 1
	bne	.L1618
	ldrb	w0, [x27, 2808]
	cmp	w0, 1
	bne	.L1618
	b	.L1617
	.p2align 3
.L1595:
	ldr	w0, [x27, 2856]
	mov	w28, 0
	cbnz	w0, .L1645
	b	.L1603
	.p2align 3
.L1680:
	cbz	x0, .L1624
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1601:
	lsl	x0, x20, 5
	add	w28, w28, 1
	add	x0, x0, x20, lsl 2
	add	x0, x22, x0
	str	w1, [x0, 1640]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 52]
	str	w1, [x0, 1644]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 32]
	str	w1, [x0, 1632]
	ldr	x1, [x26, 288]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1627]
	ldr	x1, [x26, 288]
	ldrb	w1, [x1]
	strb	w1, [x0, 1626]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 720]
	str	w1, [x0, 1648]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 768]
	str	w1, [x0, 1652]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 816]
	str	w1, [x0, 1656]
	strb	wzr, [x0, 1624]
	ldr	w0, [x27, 2856]
	cmp	w0, w28
	bls	.L1603
.L1645:
	uxtw	x20, w28
	ldr	w0, [x19, 120]
	add	x26, x19, x20, lsl 3
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetLogicFs
	ldr	x2, [x26, 288]
	lsl	x1, x20, 5
	add	x1, x1, x20, lsl 2
	add	x1, x22, x1
	ldr	x2, [x2, 8]
	ldr	w2, [x2, 48]
	str	w2, [x1, 1636]
	ldr	x2, [x26, 288]
	ldr	x2, [x2, 8]
	ldrb	w2, [x2, 2]
	strb	w2, [x1, 1625]
	ldr	x1, [x26, 288]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 1]
	cmp	w1, 1
	bne	.L1680
	mov	w1, w24
	b	.L1601
	.p2align 3
.L1624:
	mov	w1, 0
	b	.L1601
	.p2align 3
.L1603:
	ldrb	w0, [x27, 2808]
	cmp	w0, 1
	bne	.L1598
	ldr	w0, [x27, 2860]
	cbz	w0, .L1598
	mov	w26, 0
	b	.L1606
	.p2align 3
.L1681:
	cbz	x0, .L1626
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1605:
	lsl	x0, x20, 5
	add	w26, w26, 1
	add	x0, x0, x20, lsl 2
	add	x0, x22, x0
	str	w1, [x0, 1640]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 52]
	str	w1, [x0, 2796]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 32]
	str	w1, [x0, 2784]
	ldr	x1, [x24, 552]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2779]
	ldr	x1, [x24, 552]
	ldrb	w1, [x1]
	strb	w1, [x0, 2778]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 720]
	str	w1, [x0, 2800]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 768]
	str	w1, [x0, 2804]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 816]
	str	w1, [x0, 2808]
	strb	wzr, [x0, 2776]
	ldr	w0, [x27, 2860]
	cmp	w0, w26
	bls	.L1598
.L1606:
	uxtw	x20, w26
	ldr	w0, [x19, 120]
	add	x24, x19, x20, lsl 3
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetLogicFs
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w2, [x1, 48]
	lsl	x1, x20, 5
	add	x1, x1, x20, lsl 2
	add	x1, x22, x1
	str	w2, [x1, 2788]
	ldr	x2, [x24, 552]
	ldr	x2, [x2, 8]
	ldrb	w2, [x2, 2]
	strb	w2, [x1, 2777]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 1]
	cmp	w1, 1
	bne	.L1681
	mov	w1, w25
	b	.L1605
	.p2align 3
.L1590:
	ldrsb	w1, [x1, 6]
	ldr	w0, [x19, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L1682
	ldr	x0, [x0, 640]
	ldr	w25, [x0, 8]
	b	.L1589
	.p2align 3
.L1673:
	ldr	w0, [x23, 3572]
	strb	w0, [x22, 4]
	b	.L1579
.L1678:
	mov	w3, 0
	.p2align 2
.L1620:
	ubfiz	x0, x3, 2, 32
	add	w3, w3, 1
	add	x1, x19, x0
	add	x0, x22, x0
	add	x2, x1, 11116544
	add	x1, x1, 11120640
	ldr	w4, [x2, 3268]
	str	w4, [x0, 472]
	ldr	w4, [x2, 3396]
	str	w4, [x0, 600]
	ldr	w4, [x2, 3524]
	str	w4, [x0, 728]
	ldr	w2, [x2, 4036]
	str	w2, [x0, 1240]
	ldr	w2, [x1, 68]
	str	w2, [x0, 1368]
	ldr	w1, [x1, 196]
	str	w1, [x0, 1496]
	ldr	w0, [x27, 2872]
	cmp	w0, w3
	bcs	.L1620
	b	.L1618
.L1626:
	mov	w1, 0
	b	.L1605
.L1677:
	ldr	w0, [x27, 2860]
	cbz	w0, .L1598
	mov	w26, 0
	b	.L1615
	.p2align 3
.L1684:
	cbz	x0, .L1630
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1612:
	lsl	x0, x20, 5
	add	x0, x0, x20, lsl 2
	add	x0, x22, x0
	str	w1, [x0, 2792]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 52]
	str	w1, [x0, 2796]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldr	w1, [x1, 32]
	str	w1, [x0, 2784]
	ldr	x1, [x24, 552]
	ldrb	w1, [x1]
	strb	w1, [x0, 2776]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 696]
	strb	w1, [x0, 2778]
	ldr	x1, [x24, 552]
	ldrb	w1, [x1]
	cmp	w1, 1
	beq	.L1683
	strb	w1, [x0, 2782]
	ldr	x1, [x24, 552]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2783]
	ldr	x1, [x24, 552]
	ldr	w1, [x1, 24]
	str	w1, [x0, 2808]
.L1614:
	ldr	w0, [x27, 2860]
	add	w26, w26, 1
	cmp	w0, w26
	bls	.L1598
.L1615:
	uxtw	x20, w26
	ldr	w0, [x19, 120]
	add	x24, x19, x20, lsl 3
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetLogicFs
	ldr	x2, [x24, 552]
	lsl	x1, x20, 5
	add	x1, x1, x20, lsl 2
	add	x1, x22, x1
	ldr	x2, [x2, 8]
	ldr	w2, [x2, 48]
	str	w2, [x1, 2788]
	ldr	x2, [x24, 552]
	ldr	x2, [x2, 8]
	ldrb	w2, [x2, 2]
	strb	w2, [x1, 2777]
	ldr	x1, [x24, 552]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 1]
	cmp	w1, 1
	bne	.L1684
	mov	w1, w25
	b	.L1612
	.p2align 3
.L1683:
	strb	w1, [x0, 2780]
	ldr	x1, [x24, 552]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2781]
	ldr	x1, [x24, 552]
	ldr	w1, [x1, 24]
	str	w1, [x0, 2804]
	b	.L1614
.L1630:
	mov	w1, 0
	b	.L1612
.L1679:
	ldr	x2, .LC115
	mov	w3, 8012
	ldr	x1, .LC117
	add	x2, x2, 80
	bl	dprint_vfmw
	b	.L1567
.L1682:
	ldr	x2, .LC115
	mov	w3, 8033
	ldr	x1, .LC117
	add	x2, x2, 80
	bl	dprint_vfmw
	b	.L1567
	.size	MVC_WriteSliceMsg, .-MVC_WriteSliceMsg
	.align	3
.LC115:
	.xword	.LANCHOR0
	.align	3
.LC117:
	.xword	.LC116
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_StopPicNum
	.type	MVC_StopPicNum, %function
MVC_StopPicNum:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	MVC_StopPicNum, .-MVC_StopPicNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetPicStreamSize
	.type	MVC_GetPicStreamSize, %function
MVC_GetPicStreamSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 11161600
	bl	_mcount
	mov	w0, 0
	ldr	x2, [x19, 1464]
	cbz	x2, .L1687
	.p2align 2
.L1688:
	ldp	w3, w1, [x2, 8]
	ldr	x2, [x2, 4056]
	add	w1, w3, w1
	add	w1, w1, 7
	add	w0, w0, w1, lsr 3
	cbnz	x2, .L1688
.L1687:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_GetPicStreamSize, .-MVC_GetPicStreamSize
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_SliceCheck
	.type	MVC_SliceCheck, %function
MVC_SliceCheck:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 11116544
	bl	_mcount
	ldr	x1, .LC119
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x21, 2864]
	ldrb	w1, [x19, 10]
	cbnz	w1, .L1701
	mov	w1, 262143
	cmp	w0, w1
	bhi	.L1702
	ldp	w1, w2, [x19, 12]
	mul	w1, w1, w2
	sub	w1, w1, #1
	cmp	w0, w1
	bhi	.L1701
	ldr	x1, .LC123
	mov	x0, x19
	bl	mvc_ue_v
	mov	w2, w0
	ldr	x1, .LC125
	mov	w20, w0
	mov	w0, 19
	bl	dprint_vfmw
	ldrb	w0, [x19, 10]
	cbnz	w0, .L1701
	cmp	w20, 9
	bls	.L1703
	ldr	x1, .LC128
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
.L1701:
	mov	w0, -1
.L1693:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1703:
	ldr	x0, .LC126
	ldrb	w0, [x0,w20,uxtw]
	adr	x1, .Lrtx1698
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1698:
	.section	.rodata
	.align	0
	.align	2
.L1698:
	.byte	(.L1697 - .Lrtx1698) / 4
	.byte	(.L1699 - .Lrtx1698) / 4
	.byte	(.L1700 - .Lrtx1698) / 4
	.byte	(.L1697 - .Lrtx1698) / 4
	.byte	(.L1700 - .Lrtx1698) / 4
	.byte	(.L1697 - .Lrtx1698) / 4
	.byte	(.L1699 - .Lrtx1698) / 4
	.byte	(.L1700 - .Lrtx1698) / 4
	.byte	(.L1697 - .Lrtx1698) / 4
	.byte	(.L1700 - .Lrtx1698) / 4
	.text
	.p2align 3
.L1697:
	strb	wzr, [x21, 2808]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1700:
	mov	w1, 2
	strb	w1, [x21, 2808]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1699:
	mov	w1, 1
	strb	w1, [x21, 2808]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1702:
	ldr	x1, .LC121
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1693
	.size	MVC_SliceCheck, .-MVC_SliceCheck
	.align	3
.LC119:
	.xword	.LC118
	.align	3
.LC121:
	.xword	.LC120
	.align	3
.LC123:
	.xword	.LC122
	.align	3
.LC125:
	.xword	.LC124
	.align	3
.LC126:
	.xword	.L1698
	.align	3
.LC128:
	.xword	.LC127
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_PPSSPSCheck
	.type	MVC_PPSSPSCheck, %function
MVC_PPSSPSCheck:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 11116544
	bl	_mcount
	ldr	x1, .LC130
	mov	w0, 19
	ldr	w2, [x19, 2824]
	bl	dprint_vfmw
	ldr	w2, [x19, 2824]
	mov	w0, 2240
	ldr	x3, [x20, 280]
	umull	x0, w2, w0
	add	x3, x3, x0
	ldrb	w0, [x3, 19]
	cbz	w0, .L1714
	ldrsb	w19, [x19, 2820]
	cmn	w19, #1
	beq	.L1715
	cbnz	w19, .L1709
	ldr	w2, [x3, 28]
	uxtw	x1, w2
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 2
	sub	x0, x0, x1
	lsl	x0, x0, 4
	sub	x0, x0, x1
	lsl	x1, x0, 2
	sub	x1, x1, x0
	lsl	x0, x1, 6
	add	x1, x1, x0
	lsl	x1, x1, 2
	add	x0, x20, x1
	add	x1, x1, 286720
	add	x4, x0, 12288
	add	x1, x20, x1
	add	x1, x1, 2612
	ldrb	w4, [x4, 2396]
	cbz	w4, .L1710
	add	x0, x0, 286720
	ldrb	w0, [x0, 2637]
	cbz	w0, .L1710
.L1708:
	mov	w0, 0
	stp	x1, x3, [x20, 256]
.L1706:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1715:
	ldr	w2, [x3, 28]
	mov	w1, 3992
	ldr	x0, [x20, 272]
	umull	x1, w2, w1
	add	x1, x0, x1
	ldrb	w0, [x1, 25]
	cbnz	w0, .L1708
	ldr	x1, .LC134
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w19
	b	.L1706
	.p2align 3
.L1710:
	ldr	x1, .LC136
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1706
	.p2align 3
.L1714:
	ldr	x1, .LC132
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1706
	.p2align 3
.L1709:
	ldr	x1, .LC137
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1706
	.size	MVC_PPSSPSCheck, .-MVC_PPSSPSCheck
	.align	3
.LC130:
	.xword	.LC129
	.align	3
.LC132:
	.xword	.LC131
	.align	3
.LC134:
	.xword	.LC133
	.align	3
.LC136:
	.xword	.LC135
	.align	3
.LC137:
	.xword	.LC90
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_PPSSPSCheckTmpId
	.type	MVC_PPSSPSCheckTmpId, %function
MVC_PPSSPSCheckTmpId:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	x1, .LC139
	mov	w2, w20
	mov	w0, 19
	bl	dprint_vfmw
	mov	w0, 2240
	ldr	x1, [x19, 280]
	umull	x0, w20, w0
	add	x0, x1, x0
	ldrb	w1, [x0, 19]
	cbz	w1, .L1722
	ldr	w1, [x19, 36]
	ldr	w2, [x0, 28]
	sub	w0, w1, #1
	cmp	w2, w0
	bhi	.L1723
	add	x1, x19, 11116544
	mov	w0, 0
	ldrsb	w20, [x1, 2820]
	cmn	w20, #1
	beq	.L1724
.L1718:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1724:
	mov	w1, 3992
	ldr	x3, [x19, 272]
	umull	x1, w2, w1
	add	x1, x3, x1
	ldrb	w1, [x1, 25]
	cbnz	w1, .L1718
	ldr	x1, .LC145
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, w20
	b	.L1718
	.p2align 3
.L1722:
	ldr	x1, .LC141
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1718
	.p2align 3
.L1723:
	ldr	x1, .LC143
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1718
	.size	MVC_PPSSPSCheckTmpId, .-MVC_PPSSPSCheckTmpId
	.align	3
.LC139:
	.xword	.LC138
	.align	3
.LC141:
	.xword	.LC140
	.align	3
.LC143:
	.xword	.LC142
	.align	3
.LC145:
	.xword	.LC144
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_IsNewPic
	.type	MVC_IsNewPic, %function
MVC_IsNewPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 11120640
	add	x3, x19, 11116544
	ldr	x4, [x19, 248]
	ldr	w5, [x3, 2824]
	ldr	w8, [x1, 380]
	ldr	w6, [x1, 376]
	ldr	w2, [x4, 744]
	cmp	w8, w5
	ldr	w0, [x3, 2836]
	ldr	w7, [x1, 384]
	ccmp	w6, w2, 0, eq
	ldrb	w5, [x1, 368]
	cset	w2, ne
	ldrb	w6, [x3, 2809]
	cmp	w7, w0
	ccmp	w5, w6, 0, eq
	cset	w0, ne
	cmp	w6, wzr
	ccmp	w5, wzr, 4, ne
	orr	w0, w2, w0
	beq	.L1727
	ldrb	w2, [x3, 2810]
	ldrb	w5, [x1, 369]
	cmp	w5, w2
	cset	w2, ne
	orr	w0, w0, w2
.L1727:
	ldrb	w2, [x1, 371]
	ldrb	w5, [x3, 2816]
	cmp	w2, w5
	beq	.L1733
	cmp	w5, wzr
	ccmp	w2, wzr, 4, ne
	cset	w7, eq
.L1728:
	ldrb	w6, [x1, 370]
	ldrb	w5, [x3, 2811]
	cmp	w6, w5
	cset	w2, ne
	cmp	w6, 5
	ccmp	w5, 5, 0, eq
	orr	w0, w2, w0
	orr	w2, w0, w7
	beq	.L1740
.L1729:
	ldr	w0, [x4, 2900]
	cbnz	w0, .L1730
	ldr	w6, [x1, 392]
	ldr	w5, [x3, 1676]
	ldr	w0, [x3, 1680]
	ldr	w4, [x1, 396]
	cmp	w6, w5
	ccmp	w4, w0, 0, eq
	cset	w0, ne
	orr	w2, w2, w0
.L1731:
	ldrb	w0, [x3, 2819]
	cbz	w0, .L1732
	ldr	w4, [x1, 352]
	ldr	w5, [x1, 412]
	ldrb	w6, [x1, 372]
	ldrb	w0, [x3, 2817]
	cmp	w5, w4
	ldrb	w5, [x1, 373]
	cset	w1, ne
	ldrb	w4, [x3, 2818]
	cmp	w6, w0
	cset	w0, ne
	cmp	w5, w4
	orr	w0, w1, w0
	cset	w1, ne
	orr	w0, w1, w0
	orr	w2, w2, w0
.L1732:
	ldr	w3, [x3, 2864]
	mov	w0, 19
	ldr	x1, .LC147
	cmp	w3, wzr
	cset	w19, eq
	orr	w19, w19, w2
	mov	w2, w19
	bl	dprint_vfmw
	mov	w0, w19
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1730:
	cmp	w0, 1
	bne	.L1731
	ldr	w6, [x1, 400]
	ldr	w5, [x3, 1712]
	ldr	w0, [x3, 1716]
	ldr	w4, [x1, 404]
	cmp	w6, w5
	ccmp	w4, w0, 0, eq
	cset	w0, ne
	orr	w2, w2, w0
	b	.L1731
	.p2align 3
.L1733:
	mov	w7, 0
	b	.L1728
	.p2align 3
.L1740:
	ldr	w0, [x3, 2852]
	ldr	w5, [x1, 388]
	cmp	w5, w0
	cset	w0, ne
	orr	w2, w2, w0
	b	.L1729
	.size	MVC_IsNewPic, .-MVC_IsNewPic
	.align	3
.LC147:
	.xword	.LC146
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_ref_pic_list_reordering
	.type	mvc_ref_pic_list_reordering, %function
mvc_ref_pic_list_reordering:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 11116544
	mov	w21, 3
	bl	_mcount
	mov	w22, 1
	ldr	x0, [x19, 256]
	mov	w2, 5
	ldrb	w3, [x20, 2819]
	ldrb	w1, [x20, 2809]
	strb	wzr, [x20, 1756]
	cmp	w3, wzr
	strb	wzr, [x20, 1757]
	csel	w21, w21, w2, eq
	ldr	w0, [x0, 2896]
	cbz	w1, .L1743
	add	w0, w0, w2
	lsl	w22, w22, w0
	ldrb	w0, [x20, 2808]
	cmp	w0, 1
	bls	.L1796
.L1795:
	ldrb	w3, [x19, 10]
.L1762:
	cmp	w3, wzr
	csetm	w0, ne
.L1794:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1743:
	add	w0, w0, 4
	lsl	w22, w22, w0
	ldrb	w0, [x20, 2808]
	cmp	w0, 1
	bhi	.L1795
.L1796:
	ldr	x1, .LC149
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 1756]
	cbz	w0, .L1752
	ldr	w0, [x20, 2868]
	cmn	w0, #2
	beq	.L1778
	ldr	x23, .LC151
	mov	w28, 0
	ldr	x26, .LC161
	ldr	x25, .LC159
	ldr	x24, .LC155
	b	.L1759
	.p2align 3
.L1753:
	cmp	w0, 2
	beq	.L1797
	sub	w0, w0, #4
	cmp	w0, 1
	bls	.L1798
.L1756:
	ldr	w0, [x20, 2868]
	add	w28, w28, 1
	add	w0, w0, 2
	cmp	w0, w28
	bls	.L1799
.L1759:
	add	x27, x19, x28, uxtw 2
	mov	x1, x23
	add	x27, x27, 11116544
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w21, w0
	str	w0, [x27, 1760]
	ldrb	w3, [x19, 10]
	bcc	.L1749
	cbnz	w3, .L1749
	cmp	w0, 3
	beq	.L1752
	cmp	w0, 1
	bhi	.L1753
	mov	x1, x24
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x27, 2024]
	cmp	w22, w0
	ldrb	w3, [x19, 10]
	bls	.L1755
	cbz	w3, .L1756
.L1755:
	ldr	x1, .LC157
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1794
	.p2align 3
.L1752:
	ldrb	w0, [x20, 2808]
	cmp	w0, 1
	bne	.L1795
	ldr	x1, .LC165
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 1757]
	cbz	w0, .L1795
	ldr	w0, [x20, 2872]
	cmn	w0, #2
	beq	.L1795
	ldr	x23, .LC167
	mov	w27, 0
	ldr	x26, .LC177
	ldr	x25, .LC175
	ldr	x24, .LC171
	b	.L1775
	.p2align 3
.L1768:
	cmp	w0, 2
	beq	.L1800
	sub	w0, w0, #4
	cmp	w0, 1
	bls	.L1801
	ldr	w0, [x20, 2872]
	add	w0, w0, 2
	cmp	w27, w0
	beq	.L1776
.L1774:
	add	w27, w27, 1
	cmp	w27, w0
	bcs	.L1795
.L1775:
	add	x28, x19, x27, uxtw 2
	mov	x1, x23
	add	x28, x28, 11116544
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w21, w0
	str	w0, [x28, 1892]
	ldrb	w3, [x19, 10]
	bcc	.L1766
	cbnz	w3, .L1766
	cmp	w0, 3
	beq	.L1762
	cmp	w0, 1
	bhi	.L1768
	mov	x1, x24
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x28, 2156]
	cmp	w22, w0
	ldrb	w3, [x19, 10]
	bls	.L1770
	cbnz	w3, .L1770
.L1771:
	ldr	w0, [x20, 2872]
	add	w0, w0, 2
	cmp	w0, w27
	bne	.L1774
	ldr	w1, [x28, 1892]
	cmp	w1, 3
	beq	.L1774
.L1776:
	ldr	x1, .LC179
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1794
	.p2align 3
.L1797:
	mov	x1, x25
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x27, 2288]
	b	.L1756
	.p2align 3
.L1800:
	mov	x1, x25
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x28, 2416]
	b	.L1771
	.p2align 3
.L1798:
	mov	x1, x26
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x27, 2544]
	b	.L1756
	.p2align 3
.L1801:
	mov	x1, x26
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x28, 2676]
	b	.L1771
	.p2align 3
.L1749:
	ldr	x1, .LC153
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1794
	.p2align 3
.L1766:
	ldr	x1, .LC169
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1794
	.p2align 3
.L1799:
	bne	.L1752
.L1747:
	uxtw	x0, w28
	add	x0, x0, 2777088
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 9952]
	cmp	w0, 3
	beq	.L1752
	ldr	x1, .LC163
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1794
.L1770:
	ldr	x1, .LC173
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1794
.L1778:
	mov	w28, 0
	b	.L1747
	.size	mvc_ref_pic_list_reordering, .-mvc_ref_pic_list_reordering
	.align	3
.LC149:
	.xword	.LC148
	.align	3
.LC151:
	.xword	.LC150
	.align	3
.LC153:
	.xword	.LC152
	.align	3
.LC155:
	.xword	.LC154
	.align	3
.LC157:
	.xword	.LC156
	.align	3
.LC159:
	.xword	.LC158
	.align	3
.LC161:
	.xword	.LC160
	.align	3
.LC163:
	.xword	.LC162
	.align	3
.LC165:
	.xword	.LC164
	.align	3
.LC167:
	.xword	.LC166
	.align	3
.LC169:
	.xword	.LC168
	.align	3
.LC171:
	.xword	.LC170
	.align	3
.LC173:
	.xword	.LC172
	.align	3
.LC175:
	.xword	.LC174
	.align	3
.LC177:
	.xword	.LC176
	.align	3
.LC179:
	.xword	.LC178
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_pred_weight_table
	.type	mvc_pred_weight_table, %function
mvc_pred_weight_table:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x28, x0
	mov	x0, x30
	add	x20, x28, 11116544
	mov	w19, 1
	bl	_mcount
	mov	w24, 0
	ldp	x21, x2, [x28, 256]
	ldr	x1, .LC181
	mov	x0, x28
	str	x2, [x29, 104]
	bl	mvc_ue_v
	str	w0, [x20, 2876]
	lsl	w23, w19, w0
	ldr	w1, [x21, 748]
	cbnz	w1, .L1827
.L1803:
	cmp	w0, 7
	bhi	.L1806
	ldr	w0, [x20, 2880]
	cmp	w0, 7
	bhi	.L1806
	ldrb	w2, [x28, 10]
	cbnz	w2, .L1805
	ldr	x22, .LC187
	mov	w25, 0
	ldr	x27, .LC189
	b	.L1814
	.p2align 3
.L1809:
	str	w23, [x1, 2884]
	add	x0, x28, x19, lsl 2
	str	wzr, [x1, 3652]
	add	x0, x0, 11116544
	ldr	w1, [x21, 748]
	cbnz	w1, .L1828
.L1811:
	str	wzr, [x0, 3012]
	str	wzr, [x0, 3140]
	str	wzr, [x0, 3780]
	str	wzr, [x0, 3908]
.L1813:
	ldr	w0, [x20, 2868]
	add	w25, w25, 1
	cmp	w0, w25
	bcc	.L1829
.L1814:
	mov	x1, x22
	mov	x0, x28
	uxtw	x19, w25
	bl	mvc_u_1
	add	x1, x28, x19, lsl 2
	add	x1, x1, 11116544
	cbz	w0, .L1809
	mov	x1, x27
	mov	x0, x28
	bl	mvc_se_v
	add	x4, x28, x19, lsl 2
	ldr	x1, .LC191
	add	x26, x4, 11116544
	str	w0, [x26, 2884]
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x26, 3652]
	add	x0, x28, x19, lsl 2
	ldr	w1, [x21, 748]
	add	x0, x0, 11116544
	cbz	w1, .L1811
.L1828:
	ldr	x1, .LC193
	mov	x0, x28
	bl	mvc_u_1
	add	x1, x28, x19, lsl 2
	add	x1, x1, 11116544
	cbnz	w0, .L1830
	str	w24, [x1, 3012]
	add	w25, w25, 1
	str	w24, [x1, 3140]
	str	wzr, [x1, 3780]
	str	wzr, [x1, 3908]
	ldr	w0, [x20, 2868]
	cmp	w0, w25
	bcs	.L1814
.L1829:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 44]
	cmp	w0, 1
	beq	.L1815
.L1816:
	ldrb	w0, [x28, 10]
	cmp	w0, wzr
	csetm	w0, ne
.L1808:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1830:
	ldr	x1, .LC195
	mov	x0, x28
	add	x19, x28, x19, lsl 2
	add	x19, x19, 11116544
	bl	mvc_se_v
	ldr	x1, .LC197
	str	w0, [x19, 3012]
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x19, 3780]
	ldr	x1, .LC195
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x19, 3140]
	ldr	x1, .LC197
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x19, 3908]
	b	.L1813
	.p2align 3
.L1827:
	ldr	x1, .LC183
	mov	x0, x28
	bl	mvc_ue_v
	mov	w24, w0
	ldr	w0, [x20, 2876]
	str	w24, [x20, 2880]
	lsl	w24, w19, w24
	b	.L1803
	.p2align 3
.L1815:
	ldrb	w0, [x20, 2808]
	cmp	w0, 1
	bne	.L1816
	ldr	x22, .LC199
	mov	w25, 0
	ldr	x27, .LC201
	b	.L1822
	.p2align 3
.L1817:
	add	x0, x28, x19, lsl 2
	add	x0, x0, 11116544
	str	w23, [x0, 3268]
	str	wzr, [x0, 4036]
	add	x0, x28, x19, lsl 2
	ldr	w1, [x21, 748]
	cbnz	w1, .L1831
.L1819:
	add	x1, x0, 11116544
	add	x0, x0, 11120640
	str	wzr, [x1, 3396]
	str	wzr, [x0, 68]
	str	wzr, [x1, 3524]
	str	wzr, [x0, 196]
.L1821:
	ldr	w0, [x20, 2872]
	add	w25, w25, 1
	cmp	w0, w25
	bcc	.L1816
.L1822:
	mov	x1, x22
	mov	x0, x28
	uxtw	x19, w25
	bl	mvc_u_1
	cbz	w0, .L1817
	mov	x1, x27
	mov	x0, x28
	bl	mvc_se_v
	add	x4, x28, x19, lsl 2
	ldr	x1, .LC203
	add	x26, x4, 11116544
	str	w0, [x26, 3268]
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x26, 4036]
	add	x0, x28, x19, lsl 2
	ldr	w1, [x21, 748]
	cbz	w1, .L1819
.L1831:
	ldr	x1, .LC205
	mov	x0, x28
	bl	mvc_u_1
	cbnz	w0, .L1832
	add	x0, x28, x19, lsl 2
	add	x1, x0, 11116544
	add	x0, x0, 11120640
	str	w24, [x1, 3396]
	str	wzr, [x0, 68]
	str	w24, [x1, 3524]
	str	wzr, [x0, 196]
	b	.L1821
.L1806:
	ldrb	w2, [x28, 10]
.L1805:
	ldr	x1, .LC185
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1808
.L1832:
	add	x19, x28, x19, lsl 2
	ldr	x1, .LC207
	add	x0, x19, 11116544
	add	x19, x19, 11120640
	mov	x26, x0
	mov	x0, x28
	bl	mvc_se_v
	ldr	x1, .LC209
	str	w0, [x26, 3396]
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x19, 68]
	ldr	x1, .LC207
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x26, 3524]
	ldr	x1, .LC209
	mov	x0, x28
	bl	mvc_se_v
	str	w0, [x19, 196]
	b	.L1821
	.size	mvc_pred_weight_table, .-mvc_pred_weight_table
	.align	3
.LC181:
	.xword	.LC180
	.align	3
.LC183:
	.xword	.LC182
	.align	3
.LC185:
	.xword	.LC184
	.align	3
.LC187:
	.xword	.LC186
	.align	3
.LC189:
	.xword	.LC188
	.align	3
.LC191:
	.xword	.LC190
	.align	3
.LC193:
	.xword	.LC192
	.align	3
.LC195:
	.xword	.LC194
	.align	3
.LC197:
	.xword	.LC196
	.align	3
.LC199:
	.xword	.LC198
	.align	3
.LC201:
	.xword	.LC200
	.align	3
.LC203:
	.xword	.LC202
	.align	3
.LC205:
	.xword	.LC204
	.align	3
.LC207:
	.xword	.LC206
	.align	3
.LC209:
	.xword	.LC208
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecMMCO
	.type	MVC_DecMMCO, %function
MVC_DecMMCO:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x26, x0
	mov	w19, w1
	mov	x0, x30
	bl	_mcount
	cbnz	w19, .L1834
	mov	w21, 100
	ldr	x20, .LC213
	ldr	x23, .LC215
	ldr	x25, .LC221
	ldr	x22, .LC219
	ldr	x24, .LC217
	b	.L1845
	.p2align 3
.L1846:
	cmp	w19, 2
	beq	.L1859
.L1847:
	cmp	w19, 6
	cset	w0, eq
	orr	w0, w27, w0
	cbnz	w0, .L1850
	cmp	w19, 4
	beq	.L1860
.L1851:
	cmp	w19, 6
	bhi	.L1843
	ldrb	w0, [x26, 10]
	cbnz	w0, .L1843
.L1861:
	cbz	w19, .L1853
.L1852:
	subs	w21, w21, #1
	beq	.L1854
.L1845:
	mov	x1, x20
	mov	x0, x26
	bl	mvc_ue_v
	cmp	w0, 3
	mov	w19, w0
	and	w0, w0, -3
	cset	w27, eq
	cmp	w0, 1
	bne	.L1846
	mov	x1, x23
	mov	x0, x26
	bl	mvc_ue_v
	cmp	w19, 2
	bne	.L1847
.L1859:
	mov	x1, x24
	mov	x0, x26
	bl	mvc_ue_v
	ldrb	w0, [x26, 10]
	cbz	w0, .L1861
	.p2align 2
.L1843:
	ldr	x1, .LC223
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L1837:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1850:
	mov	x1, x22
	mov	x0, x26
	bl	mvc_ue_v
	cmp	w19, 4
	bne	.L1851
.L1860:
	mov	x1, x25
	mov	x0, x26
	bl	mvc_ue_v
	ldrb	w0, [x26, 10]
	cbz	w0, .L1852
	b	.L1843
	.p2align 3
.L1834:
	ldr	x20, .LC213
	mov	w28, 0
	ldr	x22, .LC215
	ldr	x24, .LC217
	ldr	x21, .LC219
	ldr	x23, .LC221
	b	.L1836
	.p2align 3
.L1838:
	cmp	w1, 2
	beq	.L1862
.L1839:
	cmp	w1, 3
	ccmp	w1, 6, 4, ne
	beq	.L1863
.L1840:
	cmp	w1, 4
	beq	.L1864
.L1841:
	cmp	w1, 6
	bhi	.L1843
	ldrb	w0, [x26, 10]
	cbnz	w0, .L1843
	add	w28, w28, 1
	cbz	w1, .L1853
	cmp	w28, 100
	beq	.L1865
.L1836:
	mov	x1, x20
	mov	x0, x26
	ldrb	w25, [x26, 3]
	uxtw	x27, w28
	bl	mvc_ue_v
	mov	w1, w0
	lsl	x0, x25, 6
	lsl	x19, x27, 4
	sub	x0, x0, x25
	add	x19, x19, x27, lsl 2
	lsl	x0, x0, 2
	and	w2, w1, -3
	sub	x0, x0, x25
	cmp	w2, 1
	lsl	x0, x0, 1
	sub	x0, x0, x25
	add	x19, x19, x0, lsl 2
	add	x19, x26, x19
	add	x19, x19, 11120640
	str	w1, [x19, 420]
	bne	.L1838
	mov	x1, x22
	mov	x0, x26
	bl	mvc_ue_v
	str	w0, [x19, 424]
	ldr	w1, [x19, 420]
	cmp	w1, 2
	bne	.L1839
.L1862:
	mov	x1, x24
	mov	x0, x26
	bl	mvc_ue_v
	lsl	x1, x25, 6
	sub	x1, x1, x25
	lsl	x1, x1, 2
	sub	x1, x1, x25
	lsl	x1, x1, 1
	sub	x2, x1, x25
	lsl	x1, x27, 4
	add	x1, x1, x27, lsl 2
	add	x1, x1, x2, lsl 2
	add	x1, x26, x1
	add	x1, x1, 11120640
	str	w0, [x1, 428]
	ldr	w1, [x1, 420]
	cmp	w1, 3
	ccmp	w1, 6, 4, ne
	bne	.L1840
	.p2align 2
.L1863:
	mov	x1, x21
	mov	x0, x26
	bl	mvc_ue_v
	lsl	x1, x25, 6
	lsl	x2, x27, 4
	sub	x1, x1, x25
	add	x2, x2, x27, lsl 2
	lsl	x1, x1, 2
	sub	x1, x1, x25
	lsl	x1, x1, 1
	sub	x1, x1, x25
	add	x2, x2, x1, lsl 2
	add	x2, x26, x2
	add	x2, x2, 11120640
	ldr	w1, [x2, 420]
	str	w0, [x2, 432]
	cmp	w1, 4
	bne	.L1841
.L1864:
	mov	x1, x23
	mov	x0, x26
	bl	mvc_ue_v
	lsl	x1, x25, 6
	sub	x1, x1, x25
	lsl	x1, x1, 2
	sub	x1, x1, x25
	lsl	x1, x1, 1
	sub	x25, x1, x25
	lsl	x1, x27, 4
	add	x27, x1, x27, lsl 2
	add	x27, x27, x25, lsl 2
	add	x27, x26, x27
	add	x27, x27, 11120640
	ldr	w1, [x27, 420]
	str	w0, [x27, 436]
	b	.L1841
	.p2align 3
.L1853:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1865:
	ldr	x1, .LC211
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1837
.L1854:
	mov	w0, -1
	b	.L1837
	.size	MVC_DecMMCO, .-MVC_DecMMCO
	.align	3
.LC211:
	.xword	.LC210
	.align	3
.LC213:
	.xword	.LC212
	.align	3
.LC215:
	.xword	.LC214
	.align	3
.LC217:
	.xword	.LC216
	.align	3
.LC219:
	.xword	.LC218
	.align	3
.LC221:
	.xword	.LC220
	.align	3
.LC223:
	.xword	.LC222
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_dec_ref_pic_marking
	.type	mvc_dec_ref_pic_marking, %function
mvc_dec_ref_pic_marking:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x21, 11116544
	ldrb	w22, [x21, 3]
	ldrb	w0, [x1, 2813]
	cbz	w0, .L1867
	cmp	w22, wzr
	cset	w19, eq
	strb	w19, [x21, 3]
.L1876:
	ldrb	w0, [x1, 2811]
	cmp	w0, 5
	beq	.L1887
	ldr	x1, .LC231
	mov	x0, x21
	sxtw	x19, w19
	bl	mvc_u_1
	mov	w4, w0
	lsl	x3, x19, 6
	ldr	x1, .LC233
	sub	x3, x3, x19
	uxtb	w2, w0
	lsl	x3, x3, 2
	mov	w0, 16
	sub	x3, x3, x19
	lsl	x3, x3, 1
	sub	x19, x3, x19
	add	x19, x21, x19, lsl 2
	add	x19, x19, 11120640
	strb	w4, [x19, 419]
	bl	dprint_vfmw
	ldrb	w0, [x19, 419]
	cbnz	w0, .L1870
.L1874:
	mov	w20, 0
.L1884:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1867:
	sxtw	x20, w22
	mov	w19, w22
	lsl	x0, x20, 6
	sub	x0, x0, x20
	lsl	x0, x0, 2
	sub	x0, x0, x20
	lsl	x0, x0, 1
	sub	x20, x0, x20
	add	x20, x21, x20, lsl 2
	add	x20, x20, 11120640
	ldrb	w0, [x20, 416]
	cbz	w0, .L1876
	ldrb	w0, [x1, 2811]
	cmp	w0, 5
	beq	.L1888
	ldr	x1, .LC231
	mov	x0, x21
	ldrb	w19, [x20, 419]
	bl	mvc_u_1
	cmp	w19, w0
	bne	.L1889
	ldrb	w0, [x20, 419]
	cbz	w0, .L1874
	mov	w1, 0
	mov	x0, x21
	bl	MVC_DecMMCO
	mov	w20, w0
	b	.L1884
	.p2align 3
.L1870:
	mov	w1, 1
	mov	x0, x21
	bl	MVC_DecMMCO
	cmn	w0, #1
	mov	w20, w0
	beq	.L1890
	mov	w0, 1
	strb	w0, [x19, 416]
	mov	w20, 0
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1888:
	ldr	x1, .LC225
	mov	x0, x21
	ldrb	w19, [x20, 417]
	bl	mvc_u_1
	cmp	w19, w0
	bne	.L1891
	ldr	x1, .LC227
	mov	x0, x21
	ldrb	w19, [x20, 418]
	bl	mvc_u_1
	cmp	w19, w0
	beq	.L1874
	ldr	x1, .LC239
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	b	.L1884
	.p2align 3
.L1887:
	ldr	x1, .LC225
	mov	x0, x21
	sxtw	x19, w19
	mov	w20, 0
	bl	mvc_u_1
	lsl	x2, x19, 6
	ldr	x1, .LC227
	sub	x2, x2, x19
	lsl	x2, x2, 2
	sub	x2, x2, x19
	lsl	x2, x2, 1
	sub	x19, x2, x19
	add	x19, x21, x19, lsl 2
	add	x19, x19, 11120640
	strb	w0, [x19, 417]
	mov	x0, x21
	bl	mvc_u_1
	strb	w0, [x19, 418]
	ldrb	w2, [x19, 417]
	mov	w0, 16
	ldr	x1, .LC229
	bl	dprint_vfmw
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L1889:
	ldr	x1, .LC241
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	ldr	x1, .LC243
	mov	w0, 1
	ldr	w4, [x21, 68]
	ldr	w2, [x21, 88]
	ldr	w3, [x21, 100]
	bl	dprint_vfmw
	b	.L1884
.L1891:
	ldr	x1, .LC237
	mov	w0, 1
	mov	w20, -1
	bl	dprint_vfmw
	b	.L1884
.L1890:
	strb	wzr, [x19, 416]
	mov	w0, 1
	ldr	x1, .LC235
	strb	w22, [x21, 3]
	bl	dprint_vfmw
	b	.L1884
	.size	mvc_dec_ref_pic_marking, .-mvc_dec_ref_pic_marking
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ProcessSliceHeaderFirstPart
	.type	MVC_ProcessSliceHeaderFirstPart, %function
MVC_ProcessSliceHeaderFirstPart:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC245
	mov	x0, x19
	bl	mvc_ue_v
	mov	w21, w0
	ldr	w0, [x19, 40]
	sub	w0, w0, #1
	cmp	w21, w0
	bhi	.L1893
	ldrb	w0, [x19, 10]
	cbnz	w0, .L1893
	mov	w1, w21
	mov	x0, x19
	bl	MVC_PPSSPSCheckTmpId
	cbnz	w0, .L1947
	add	x20, x19, 11116544
	mov	x0, x19
	str	w21, [x20, 2824]
	bl	MVC_PPSSPSCheck
	mov	w22, w0
	cbnz	w0, .L1948
	ldp	x21, x23, [x19, 256]
	ldr	x2, .LC253
	mov	x0, x19
	ldr	w1, [x21, 2896]
	add	w1, w1, 4
	bl	mvc_u_v
	str	w0, [x20, 2836]
	ldrb	w1, [x19, 10]
	cbnz	w1, .L1919
	ldrb	w1, [x20, 2811]
	cmp	w1, 5
	ccmp	w0, wzr, 4, eq
	bne	.L1949
.L1899:
	strb	wzr, [x20, 2809]
	strb	wzr, [x20, 2810]
	ldrb	w0, [x21, 20]
	cbz	w0, .L1950
	ldr	w1, [x21, 3952]
	ldr	w2, [x21, 3948]
	add	w0, w1, 1
	madd	w0, w2, w0, w0
.L1922:
	ldrb	w1, [x21, 21]
	cbnz	w1, .L1905
	strb	wzr, [x20, 2812]
.L1907:
	ldr	w1, [x20, 2864]
	cmp	w1, w0
	bcs	.L1951
.L1908:
	ldrb	w0, [x20, 2811]
	cmp	w0, 5
	beq	.L1952
.L1910:
	str	wzr, [x20, 1676]
	str	wzr, [x20, 1680]
	ldr	w0, [x21, 2900]
	cbz	w0, .L1953
.L1914:
	str	wzr, [x20, 1712]
	str	wzr, [x20, 1716]
	ldr	w0, [x21, 2900]
	cmp	w0, 1
	beq	.L1954
.L1917:
	ldrb	w0, [x19, 10]
	cbnz	w0, .L1919
	ldrb	w0, [x23, 17]
	cbz	w0, .L1920
	ldr	x1, .LC277
	mov	x0, x19
	bl	mvc_ue_v
	cbnz	w0, .L1921
	ldrb	w0, [x19, 10]
	cbz	w0, .L1920
.L1921:
	ldr	x1, .LC279
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC280
	ldr	x4, [x0]
	cbz	x4, .L1919
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	mov	w22, -1
	blr	x4
.L1931:
	mov	w0, w22
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1950:
	ldr	x1, .LC257
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 2809]
	cbnz	w0, .L1955
	ldr	w2, [x21, 3952]
	ldr	w1, [x21, 3948]
	add	w0, w2, 1
	ldrb	w3, [x21, 20]
	madd	w1, w1, w0, w0
	lsl	w0, w1, 1
	cbz	w3, .L1922
.L1902:
	ldrb	w2, [x20, 2809]
	mov	w0, 1
	b	.L1903
	.p2align 3
.L1905:
	mov	w1, 1
	strb	w1, [x20, 2812]
.L1924:
	ldr	w1, [x20, 2864]
	cmp	w1, w0, lsr 1
	bcc	.L1908
	ldr	x1, .LC261
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1931
	.p2align 3
.L1955:
	ldr	x1, .LC259
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 2810]
	ldr	w0, [x21, 3952]
	ldr	w1, [x21, 3948]
	ldrb	w2, [x21, 20]
	add	w0, w0, 1
	madd	w1, w1, w0, w0
	cbnz	w2, .L1902
	ldrb	w2, [x20, 2809]
	mov	w0, 2
.L1903:
	mul	w0, w0, w1
	cbz	w2, .L1922
	strb	wzr, [x20, 2812]
	cmp	w2, 1
	bne	.L1907
	b	.L1924
	.p2align 3
.L1920:
	mov	x0, x19
	bl	MVC_IsNewPic
	uxtb	w0, w0
	strb	w0, [x20, 2813]
	cbnz	w0, .L1931
	add	x19, x19, 11120640
	ldr	w1, [x20, 2864]
	ldr	w0, [x19, 408]
	cmp	w1, w0
	bhi	.L1931
	ldr	x1, .LC282
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1931
	.p2align 3
.L1953:
	ldr	w1, [x21, 2904]
	mov	x0, x19
	ldr	x2, .LC269
	add	w1, w1, 4
	bl	mvc_u_v
	str	w0, [x20, 1676]
	ldrb	w0, [x23, 1]
	cbz	w0, .L1914
	ldrb	w0, [x20, 2809]
	cbnz	w0, .L1914
	ldr	x1, .LC271
	mov	x0, x19
	bl	mvc_se_v
	str	w0, [x20, 1680]
	b	.L1914
	.p2align 3
.L1954:
	ldrb	w0, [x21, 18]
	cbnz	w0, .L1917
	ldr	x1, .LC273
	mov	x0, x19
	bl	mvc_se_v
	str	w0, [x20, 1712]
	ldrb	w0, [x23, 1]
	cmp	w0, 1
	bne	.L1917
	ldrb	w0, [x20, 2809]
	cbnz	w0, .L1917
	ldr	x1, .LC275
	mov	x0, x19
	bl	mvc_se_v
	str	w0, [x20, 1716]
	b	.L1917
	.p2align 3
.L1919:
	mov	w22, -1
	ldr	x23, [sp, 48]
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1893:
	ldr	x1, .LC247
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1952:
	ldr	x1, .LC265
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 2852]
	mov	w1, 65535
	cmp	w0, w1
	bhi	.L1911
	ldrb	w0, [x19, 10]
	cbz	w0, .L1910
.L1911:
	ldr	x1, .LC267
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1910
	.p2align 3
.L1947:
	ldr	x1, .LC249
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1931
	.p2align 3
.L1948:
	ldr	x1, .LC251
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1931
	.p2align 3
.L1949:
	add	x0, x19, 11120640
	ldr	w0, [x0, 352]
	cmp	w0, wzr
	bgt	.L1899
	ldr	x1, .LC255
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1899
.L1951:
	ldr	x1, .LC263
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1931
	.size	MVC_ProcessSliceHeaderFirstPart, .-MVC_ProcessSliceHeaderFirstPart
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
	.align	3
.LC263:
	.xword	.LC262
	.align	3
.LC265:
	.xword	.LC264
	.align	3
.LC267:
	.xword	.LC266
	.align	3
.LC269:
	.xword	.LC268
	.align	3
.LC271:
	.xword	.LC270
	.align	3
.LC273:
	.xword	.LC272
	.align	3
.LC275:
	.xword	.LC274
	.align	3
.LC277:
	.xword	.LC276
	.align	3
.LC279:
	.xword	.LC278
	.align	3
.LC280:
	.xword	g_event_report
	.align	3
.LC282:
	.xword	.LC281
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ProcessSliceHeaderSecondPart
	.type	MVC_ProcessSliceHeaderSecondPart, %function
MVC_ProcessSliceHeaderSecondPart:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 11116544
	bl	_mcount
	ldr	w0, [x20, 40]
	ldr	w2, [x19, 2824]
	sub	w0, w0, #1
	ldr	x21, [x20, 264]
	cmp	w0, w2
	bcc	.L1957
	ldrb	w22, [x20, 10]
	cbnz	w22, .L1957
	mov	x0, x20
	bl	MVC_PPSSPSCheck
	cbnz	w0, .L2039
	ldrb	w1, [x19, 2808]
	cmp	w1, 1
	beq	.L2040
	ldr	w0, [x21, 36]
	cmp	w1, 1
	str	w0, [x19, 2868]
	ldr	w0, [x21, 40]
	str	w0, [x19, 2872]
	bls	.L2041
.L1962:
	cmp	w1, 2
	beq	.L1988
.L1972:
	mov	w0, 32
	str	w0, [x19, 2872]
.L1973:
	mov	x0, x20
	bl	mvc_ref_pic_list_reordering
	mov	w23, w0
	cbnz	w0, .L2042
	ldrb	w0, [x21, 2]
	cbz	w0, .L1975
	ldrb	w0, [x19, 2808]
	cbnz	w0, .L1975
	mov	w22, 1
	mov	w2, w22
.L1976:
	ldr	x1, .LC300
	mov	w0, 19
	strb	w22, [x19, 2815]
	bl	dprint_vfmw
	str	wzr, [x19, 2876]
	ldrb	w0, [x19, 2815]
	str	wzr, [x19, 2880]
	cbnz	w0, .L2043
.L1977:
	ldrb	w0, [x19, 2816]
	cbnz	w0, .L2044
.L1978:
	add	x22, x20, 11120640
	str	wzr, [x22, 324]
	ldrb	w0, [x21]
	cbz	w0, .L1979
	ldrb	w0, [x19, 2808]
	cmp	w0, 1
	bls	.L2045
.L1979:
	ldr	x1, .LC310
	mov	x0, x20
	bl	mvc_se_v
	add	x1, x22, 256
	stp	w0, wzr, [x1, 72]
	stp	wzr, wzr, [x1, 80]
	ldrb	w0, [x21, 3]
	cbnz	w0, .L2046
.L2008:
	mov	w0, w23
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1975:
	ldr	w0, [x21, 44]
	mov	w2, 0
	cmp	w0, 1
	bne	.L1976
	ldrb	w0, [x19, 2808]
	cmp	w0, 1
	bne	.L1976
	mov	w22, w0
	mov	w2, w0
	b	.L1976
	.p2align 3
.L2046:
	ldr	x1, .LC312
	mov	x0, x20
	bl	mvc_ue_v
	cmp	w0, 2
	str	w0, [x22, 332]
	bhi	.L1982
	ldrb	w1, [x20, 10]
	cbnz	w1, .L1982
	cmp	w0, 1
	beq	.L2008
	ldr	x1, .LC316
	mov	x0, x20
	bl	mvc_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x22, 336]
	bhi	.L1985
	ldrb	w0, [x20, 10]
	cbnz	w0, .L1985
	ldr	x1, .LC320
	mov	x0, x20
	bl	mvc_se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x22, 340]
	bhi	.L1987
	ldrb	w0, [x20, 10]
	cbz	w0, .L2008
.L1987:
	ldr	x1, .LC322
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L2043:
	mov	x0, x20
	bl	mvc_pred_weight_table
	cbz	w0, .L1977
	ldr	x1, .LC302
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L2044:
	mov	x0, x20
	bl	mvc_dec_ref_pic_marking
	cbz	w0, .L1978
	ldr	x1, .LC304
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L1988:
	mov	w0, 32
	str	w0, [x19, 2868]
	b	.L1972
	.p2align 3
.L2040:
	ldr	x1, .LC286
	mov	x0, x20
	bl	mvc_u_1
	strb	w0, [x19, 2814]
	ldrb	w1, [x19, 2808]
	ldr	w0, [x21, 36]
	str	w0, [x19, 2868]
	cmp	w1, 1
	ldr	w0, [x21, 40]
	str	w0, [x19, 2872]
	bhi	.L1962
	.p2align 2
.L2041:
	ldr	x1, .LC288
	mov	x0, x20
	bl	mvc_u_1
	cbnz	w0, .L1963
	ldr	w1, [x19, 2868]
.L1964:
	ldrb	w0, [x19, 2809]
	cbnz	w0, .L1965
	cmp	w1, 15
	bhi	.L1966
	ldr	w0, [x19, 2872]
	cmp	w0, 15
	bhi	.L2047
.L1967:
	ldrb	w0, [x20, 10]
	cbnz	w0, .L1966
.L1968:
	ldrb	w0, [x19, 2808]
	cmp	w0, 2
	beq	.L1988
	cmp	w0, 1
	beq	.L1973
	b	.L1972
	.p2align 3
.L1957:
	ldr	x1, .LC283
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	mov	w0, w23
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2045:
	ldr	x1, .LC306
	mov	x0, x20
	bl	mvc_ue_v
	cmp	w0, 2
	str	w0, [x22, 324]
	bhi	.L1980
	ldrb	w0, [x20, 10]
	cbz	w0, .L1979
.L1980:
	ldr	x1, .LC308
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L1965:
	cmp	w1, 31
	bhi	.L1969
	ldr	w0, [x19, 2872]
	cmp	w0, 31
	bls	.L1970
	ldrb	w0, [x19, 2808]
	cmp	w0, 1
	bne	.L1970
.L1969:
	ldr	x1, .LC296
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L1970:
	ldrb	w0, [x20, 10]
	cbz	w0, .L1968
	b	.L1969
	.p2align 3
.L2047:
	ldrb	w0, [x19, 2808]
	cmp	w0, 1
	bne	.L1967
.L1966:
	ldr	x1, .LC294
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L1963:
	ldr	x1, .LC290
	mov	x0, x20
	bl	mvc_ue_v
	mov	w1, w0
	ldrb	w0, [x19, 2808]
	str	w1, [x19, 2868]
	cmp	w0, 1
	bne	.L1964
	ldr	x1, .LC292
	mov	x0, x20
	bl	mvc_ue_v
	str	w0, [x19, 2872]
	ldr	w1, [x19, 2868]
	b	.L1964
	.p2align 3
.L1985:
	ldr	x1, .LC318
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L1982:
	ldr	x1, .LC314
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L2039:
	ldr	x1, .LC284
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.p2align 3
.L2042:
	ldr	x1, .LC298
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L2008
	.size	MVC_ProcessSliceHeaderSecondPart, .-MVC_ProcessSliceHeaderSecondPart
	.align	3
.LC283:
	.xword	.LC246
	.align	3
.LC284:
	.xword	.LC248
	.align	3
.LC286:
	.xword	.LC285
	.align	3
.LC288:
	.xword	.LC287
	.align	3
.LC290:
	.xword	.LC289
	.align	3
.LC292:
	.xword	.LC291
	.align	3
.LC294:
	.xword	.LC293
	.align	3
.LC296:
	.xword	.LC295
	.align	3
.LC298:
	.xword	.LC297
	.align	3
.LC300:
	.xword	.LC299
	.align	3
.LC302:
	.xword	.LC301
	.align	3
.LC304:
	.xword	.LC303
	.align	3
.LC306:
	.xword	.LC305
	.align	3
.LC308:
	.xword	.LC307
	.align	3
.LC310:
	.xword	.LC309
	.align	3
.LC312:
	.xword	.LC311
	.align	3
.LC314:
	.xword	.LC313
	.align	3
.LC316:
	.xword	.LC315
	.align	3
.LC318:
	.xword	.LC317
	.align	3
.LC320:
	.xword	.LC319
	.align	3
.LC322:
	.xword	.LC321
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ExitSlice
	.type	MVC_ExitSlice, %function
MVC_ExitSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 11116544
	add	x0, x19, 11120640
	ldr	x3, [x19, 248]
	ldr	w2, [x1, 2824]
	str	w2, [x0, 380]
	ldr	w2, [x3, 744]
	str	w2, [x0, 376]
	ldr	w2, [x1, 2864]
	str	w2, [x0, 408]
	ldr	w2, [x1, 2836]
	str	w2, [x0, 384]
	ldrb	w2, [x1, 2809]
	strb	w2, [x0, 368]
	cbz	w2, .L2049
	ldrb	w2, [x1, 2810]
	strb	w2, [x0, 369]
.L2049:
	ldrb	w2, [x1, 2816]
	strb	w2, [x0, 371]
	ldrb	w2, [x1, 2811]
	strb	w2, [x0, 370]
	cmp	w2, 5
	beq	.L2057
.L2050:
	ldr	w2, [x3, 2900]
	cbnz	w2, .L2051
	ldr	w2, [x1, 1676]
	str	w2, [x0, 392]
	ldr	w2, [x1, 1680]
	str	w2, [x0, 396]
	ldr	w2, [x3, 2900]
.L2051:
	cmp	w2, 1
	beq	.L2058
.L2052:
	ldrb	w2, [x1, 2819]
	cmp	w2, 1
	bne	.L2053
	ldr	w2, [x0, 352]
	str	w2, [x0, 412]
	ldrb	w2, [x1, 2817]
	strb	w2, [x0, 372]
	ldrb	w1, [x1, 2818]
	strb	w1, [x0, 373]
.L2053:
	ldr	w0, [x19, 80]
	add	w0, w0, 1
	str	w0, [x19, 80]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2058:
	ldr	w2, [x1, 1712]
	str	w2, [x0, 400]
	ldr	w2, [x1, 1716]
	str	w2, [x0, 404]
	b	.L2052
	.p2align 3
.L2057:
	ldr	w2, [x1, 2852]
	str	w2, [x0, 388]
	b	.L2050
	.size	MVC_ExitSlice, .-MVC_ExitSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_PicTypeStatistic
	.type	MVC_PicTypeStatistic, %function
MVC_PicTypeStatistic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 11116544
	ldrb	w0, [x1, 2808]
	cmp	w0, 1
	beq	.L2061
	cbz	w0, .L2062
	cmp	w0, 2
	bne	.L2059
	ldrb	w0, [x1, 2811]
	add	x19, x19, 11157504
	cmp	w0, 5
	beq	.L2069
.L2064:
	strb	wzr, [x19, 1973]
.L2059:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2062:
	add	x19, x19, 11157504
	ldrb	w0, [x19, 1972]
	cmp	w0, 2
	beq	.L2064
	mov	w0, 1
	strb	wzr, [x19, 1973]
	strb	w0, [x19, 1972]
	b	.L2059
	.p2align 3
.L2061:
	add	x19, x19, 11157504
	mov	w0, 2
	strb	wzr, [x19, 1973]
	strb	w0, [x19, 1972]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2069:
	mov	w0, 1
	strb	w0, [x19, 1973]
	b	.L2059
	.size	MVC_PicTypeStatistic, .-MVC_PicTypeStatistic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_CalcStreamBits
	.type	MVC_CalcStreamBits, %function
MVC_CalcStreamBits:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 11112448
	ldrb	w0, [x0, 3528]
	cbz	w0, .L2071
	ldr	x1, [x19, 240]
	ldr	w0, [x1, 72]
	add	w0, w0, 7
	and	w0, w0, -8
	str	w0, [x1, 72]
	ldr	x1, [x19, 240]
	ldr	w0, [x1, 80]
	sub	w0, w0, #1
	str	w0, [x1, 80]
.L2071:
	ldr	x3, [x19, 240]
	ldrb	w21, [x3]
	cbz	w21, .L2082
	sub	w1, w21, #1
	mov	w4, 32
	mov	x2, 48
	add	x0, x3, 16
	mov	w20, 0
	umaddl	x2, w1, w4, x2
	add	x2, x3, x2
	.p2align 2
.L2073:
	ldr	w1, [x0], 32
	cmp	x0, x2
	add	w20, w20, w1
	bne	.L2073
.L2072:
	sbfiz	x4, x21, 5, 32
	ldr	w2, [x3, 72]
	add	x4, x3, x4
	ldr	w5, [x3, 80]
	ldr	x1, .LC324
	mov	w3, w20
	mov	w0, 7
	ldr	w4, [x4, 16]
	bl	dprint_vfmw
	ldr	x2, [x19, 240]
	ubfiz	x3, x21, 5, 8
	ldrb	w0, [x2]
	ldr	w1, [x2, 72]
	add	x0, x2, x0, lsl 5
	and	w1, w1, 7
	str	w1, [x0, 28]
	ldr	x2, [x19, 240]
	ldrb	w1, [x2]
	ldr	w0, [x2, 72]
	add	x1, x2, x1, lsl 5
	ldr	w2, [x1, 20]
	add	w0, w2, w0, lsr 3
	mov	w2, w21
	sub	w0, w0, w20
	str	w0, [x1, 32]
	ldr	x4, [x19, 240]
	ldrb	w1, [x4]
	ldr	w0, [x4, 72]
	add	x4, x4, x1, lsl 5
	ldr	w1, [x4, 16]
	add	w20, w20, w1
	lsl	w20, w20, 3
	sub	w0, w20, w0
	str	w0, [x4, 24]
	ldr	x1, [x19, 240]
	ldrb	w0, [x1]
	add	x0, x1, x0, lsl 5
	ldp	w4, w1, [x0, 28]
	ubfiz	w1, w1, 3, 2
	add	w1, w1, w4
	str	w1, [x0, 28]
	ldr	x1, .LC326
	ldr	x4, [x19, 240]
	ldrb	w0, [x4]
	add	x4, x4, x0, lsl 5
	mov	w0, 7
	ldr	w5, [x4, 32]
	and	w5, w5, -4
	str	w5, [x4, 32]
	ldr	x6, [x19, 240]
	add	x3, x6, x3
	ldrb	w5, [x6]
	ldp	w4, w3, [x3, 24]
	add	x5, x6, x5, lsl 5
	ldr	w5, [x5, 32]
	bl	dprint_vfmw
	ldr	x0, [x19, 240]
	ldr	w2, [x0, 76]
	sub	w1, w2, #1
	cmp	w1, 1
	bhi	.L2093
	ldrb	w20, [x0]
	add	w3, w20, 1
	cmp	w3, 1
	ccmp	w2, w3, 0, ls
	bls	.L2076
	ldr	x21, .LC330
	add	w20, w20, 2
	.p2align 2
.L2077:
	sub	w2, w20, #1
	mov	x1, x21
	ubfiz	x3, x2, 5, 32
	add	x4, x0, x3
	mov	w0, 7
	str	wzr, [x4, 28]
	ldr	x4, [x19, 240]
	add	x4, x4, x3
	ldr	w5, [x4, 20]
	str	w5, [x4, 32]
	ldr	x4, [x19, 240]
	add	x4, x4, x3
	ldr	w5, [x4, 16]
	lsl	w5, w5, 3
	str	w5, [x4, 24]
	ldr	x4, [x19, 240]
	add	x4, x4, x3
	ldp	w6, w5, [x4, 28]
	ubfiz	w5, w5, 3, 2
	add	w5, w5, w6
	str	w5, [x4, 28]
	ldr	x4, [x19, 240]
	add	x4, x4, x3
	ldr	w5, [x4, 32]
	and	w5, w5, -4
	str	w5, [x4, 32]
	ldr	x4, [x19, 240]
	add	x3, x4, x3
	ldp	w4, w3, [x3, 24]
	bl	dprint_vfmw
	ldr	x0, [x19, 240]
	cmp	w20, 1
	cset	w3, ls
	ldr	w1, [x0, 76]
	cmp	w1, w20
	add	w20, w20, 1
	cset	w2, hi
	tst	w3, w2
	bne	.L2077
	sub	w1, w1, #1
.L2076:
	ubfiz	x1, x1, 5, 32
	ldr	w2, [x0, 80]
	add	x0, x0, x1
	add	x5, x19, 11157504
	ldr	w1, [x0, 24]
	sub	w1, w1, w2
	str	w1, [x0, 24]
	ldr	x3, [x19, 240]
	ldrb	w0, [x3]
	ldr	w4, [x3, 76]
	cmp	w0, w4
	bcs	.L2070
	.p2align 2
.L2087:
	ubfiz	x1, x0, 5, 32
	ldr	w2, [x5, 2680]
	add	x1, x3, x1
	add	w0, w0, 1
	ldr	w1, [x1, 32]
	cmp	w1, w2
	bcs	.L2079
	str	w1, [x5, 2680]
	ldr	w4, [x3, 76]
.L2079:
	cmp	w4, w0
	bhi	.L2087
.L2070:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2093:
	ldr	x1, .LC328
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L2082:
	mov	w20, 0
	b	.L2072
	.size	MVC_CalcStreamBits, .-MVC_CalcStreamBits
	.align	3
.LC324:
	.xword	.LC323
	.align	3
.LC326:
	.xword	.LC325
	.align	3
.LC328:
	.xword	.LC327
	.align	3
.LC330:
	.xword	.LC329
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_Scaling_List
	.type	MVC_Scaling_List, %function
MVC_Scaling_List:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	str	x0, [x29, 104]
	mov	x0, x30
	mov	w27, w3
	mov	x28, x1
	mov	x25, x2
	mov	x22, x4
	bl	_mcount
	cmp	w27, wzr
	ble	.L2094
	sub	w26, w27, #1
	mov	w0, 8
	ldr	x21, .LC331
	add	x26, x26, 1
	mov	x20, 0
	mov	w24, w0
	mov	w23, 255
	b	.L2099
	.p2align 3
.L2098:
	lsr	w6, w19, 2
	str	w24, [x28, w19, uxtw 2]
	ubfiz	w1, w19, 3, 2
	and	w4, w24, 255
	lsl	w5, w23, w1
	lsl	w4, w4, w1
	ldr	w1, [x25, w6, sxtw 2]
	cmp	x26, x20
	bic	w1, w1, w5
	orr	w1, w1, w4
	str	w1, [x25, w6, sxtw 2]
	beq	.L2094
.L2099:
	cmp	w27, 16
	add	x1, x21, 16
	beq	.L2107
	ldrb	w19, [x20, x1]
.L2097:
	add	x20, x20, 1
	cbz	w0, .L2098
	ldr	x1, .LC333
	ldr	x0, [x29, 104]
	bl	mvc_se_v
	add	w0, w0, w24
	and	w0, w0, 255
	cmp	w19, wzr
	ccmp	w0, wzr, 0, eq
	cset	w1, eq
	cmp	w0, wzr
	csel	w24, w24, w0, eq
	str	w1, [x22]
	b	.L2098
	.p2align 3
.L2107:
	ldrb	w19, [x21, x20]
	b	.L2097
	.p2align 3
.L2094:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.size	MVC_Scaling_List, .-MVC_Scaling_List
	.align	3
.LC331:
	.xword	.LANCHOR0
	.align	3
.LC333:
	.xword	.LC332
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ProcessPPS
	.type	MVC_ProcessPPS, %function
MVC_ProcessPPS:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x1, .LC337
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2157
	ldr	x1, .LC339
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 1]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2157
	ldr	x1, .LC341
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 32]
	cbnz	w0, .L2112
	ldrb	w0, [x19, 10]
	cbz	w0, .L2113
.L2112:
	ldr	x1, .LC343
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC344
	ldr	x4, [x0]
	cbz	x4, .L2157
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, -1
.L2110:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2113:
	ldr	x1, .LC346
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w0, 31
	str	w0, [x20, 36]
	bhi	.L2114
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2114
	ldr	x1, .LC350
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w0, 31
	str	w0, [x20, 40]
	bhi	.L2116
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2116
	ldr	x1, .LC354
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 2]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2157
	ldr	x2, .LC356
	mov	w1, 2
	mov	x0, x19
	bl	mvc_u_v
	cmp	w0, 2
	str	w0, [x20, 44]
	bhi	.L2158
	ldr	x1, .LC360
	mov	x0, x19
	bl	mvc_se_v
	str	w0, [x20, 48]
	ldr	x1, .LC362
	mov	x0, x19
	bl	mvc_se_v
	ldr	x1, .LC364
	mov	x0, x19
	bl	mvc_se_v
	str	w0, [x20, 52]
	ldr	x1, .LC366
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 3]
	ldr	x1, .LC368
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 4]
	ldr	x1, .LC370
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 17]
	cbnz	w0, .L2159
.L2119:
	ldr	w0, [x20, 52]
	stp	w0, wzr, [x20, 56]
	strb	wzr, [x20, 18]
	add	x0, x19, 872
	bl	BsResidBits
	ldr	x1, [x19, 240]
	ldr	w1, [x1, 80]
	add	w1, w1, 3
	cmp	w0, w1
	bcc	.L2140
	ldr	x1, .LC374
	mov	x0, x19
	bl	mvc_u_1
	str	w0, [x20, 60]
	ldr	x1, .LC376
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 18]
	beq	.L2121
.L2139:
	ldr	x1, .LC378
	mov	x0, x19
	bl	mvc_se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x20, 56]
	bhi	.L2122
	ldrb	w0, [x19, 10]
	cbz	w0, .L2140
.L2122:
	ldr	x1, .LC385
	mov	w0, 1
	bl	dprint_vfmw
.L2157:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, -1
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2140:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2159:
	ldr	x1, .LC372
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2119
	.p2align 3
.L2114:
	ldr	x1, .LC348
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2110
	.p2align 3
.L2116:
	ldr	x1, .LC352
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2110
	.p2align 3
.L2121:
	ldr	x27, .LC381
	add	x21, x29, 120
	ldr	x25, .LC380
	add	x23, x20, 5
	add	x22, x20, 2016
	add	x24, x20, 64
	mov	w26, 0
	mov	x28, x27
	b	.L2130
	.p2align 3
.L2163:
	ldr	w0, [x20, 28]
	mov	x2, 16
	umull	x0, w0, w3
	ldr	x3, [x19, 272]
	add	x3, x3, x0
	ldr	x0, .LC335
	add	x1, x0, 16
	mov	x0, x22
	ldrb	w4, [x3, 27]
	cmp	w4, 1
	beq	.L2160
	ldr	x4, .LC335
	cmp	w26, wzr
	ldr	x3, [x28, 104]
	csel	x1, x1, x4, ne
	blr	x3
.L2125:
	add	w26, w26, 1
	add	x21, x21, 4
	cmp	w26, 6
	add	x23, x23, 1
	add	x22, x22, 16
	add	x24, x24, 64
	beq	.L2161
.L2130:
	mov	x1, x25
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x23]
	cmp	w0, 1
	str	w0, [x21]
	mov	w3, 3992
	beq	.L2162
	cmp	w26, wzr
	ccmp	w26, 3, 4, ne
	beq	.L2163
	sub	w1, w26, #1
	ldr	x3, [x27, 104]
	add	x1, x1, 126
	mov	x0, x22
	mov	x2, 16
	add	x1, x20, x1, lsl 4
	blr	x3
	b	.L2125
	.p2align 3
.L2161:
	ldr	x21, .LC381
	add	x26, x20, 2112
	ldr	x23, .LC335
	add	x28, x20, 448
	mov	x27, 0
	add	x22, x29, 112
	mov	w24, 3992
	mov	x25, x21
.L2138:
	ldr	w0, [x20, 60]
	mov	w8, w27
	cmp	w0, 1
	beq	.L2164
	add	x0, x20, x27
	str	wzr, [x22, x27, lsl 2]
	strb	wzr, [x0, 11]
.L2133:
	ldr	w0, [x20, 28]
	ldr	x1, [x19, 272]
	umull	x0, w0, w24
	add	x0, x1, x0
	ldrb	w1, [x0, 27]
	cmp	w1, 1
	beq	.L2165
	add	x1, x23, 96
	ldr	x3, [x21, 104]
	cbz	w8, .L2166
.L2137:
	mov	x2, 64
	mov	x0, x26
	blr	x3
.L2134:
	add	x27, x27, 1
	add	x26, x26, 64
	cmp	x27, 2
	add	x28, x28, 256
	bne	.L2138
	b	.L2139
	.p2align 3
.L2162:
	mov	x0, x19
	mov	x4, x21
	mov	x2, x22
	mov	x1, x24
	mov	w3, 16
	bl	MVC_Scaling_List
	ldr	w0, [x21]
	cmp	w0, 1
	bne	.L2125
	ldr	x0, .LC335
	cmp	w26, 2
	mov	x2, 16
	add	x1, x0, 16
	csel	x1, x0, x1, ls
	ldr	x0, .LC381
	ldr	x3, [x0, 104]
	mov	x0, x22
	blr	x3
	b	.L2125
.L2160:
	uxtw	x1, w26
	ldr	x0, .LC381
	add	x1, x1, 167
	add	x1, x3, x1, lsl 4
	ldr	x3, [x0, 104]
	mov	x0, x22
	blr	x3
	b	.L2125
.L2158:
	ldr	x1, .LC358
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2110
.L2164:
	ldr	x1, .LC383
	mov	x0, x19
	str	w27, [x29, 108]
	bl	mvc_u_1
	cmp	w0, 1
	add	x1, x20, x27
	str	w0, [x22, x27, lsl 2]
	ldr	w8, [x29, 108]
	strb	w0, [x1, 11]
	bne	.L2133
	mov	x0, x19
	add	x4, x22, x27, lsl 2
	mov	w3, 64
	mov	x2, x26
	mov	x1, x28
	str	w8, [x29, 108]
	bl	MVC_Scaling_List
	ldr	w0, [x22, x27, lsl 2]
	ldr	w8, [x29, 108]
	cmp	w0, 1
	bne	.L2134
	ldr	x0, .LC381
	ldr	x3, [x0, 104]
	ldr	x0, .LC335
	add	x1, x0, 96
	cbnz	w8, .L2137
	add	x1, x0, 32
	b	.L2137
	.p2align 3
.L2165:
	ubfiz	x1, x8, 6, 32
	ldr	x3, [x25, 104]
	add	x1, x1, 2768
	mov	x2, 64
	add	x1, x0, x1
	mov	x0, x26
	blr	x3
	b	.L2134
.L2166:
	ldr	x1, .LC335
	add	x1, x1, 32
	b	.L2137
	.size	MVC_ProcessPPS, .-MVC_ProcessPPS
	.align	3
.LC335:
	.xword	.LANCHOR1
	.align	3
.LC337:
	.xword	.LC336
	.align	3
.LC339:
	.xword	.LC338
	.align	3
.LC341:
	.xword	.LC340
	.align	3
.LC343:
	.xword	.LC342
	.align	3
.LC344:
	.xword	g_event_report
	.align	3
.LC346:
	.xword	.LC345
	.align	3
.LC348:
	.xword	.LC347
	.align	3
.LC350:
	.xword	.LC349
	.align	3
.LC352:
	.xword	.LC351
	.align	3
.LC354:
	.xword	.LC353
	.align	3
.LC356:
	.xword	.LC355
	.align	3
.LC358:
	.xword	.LC357
	.align	3
.LC360:
	.xword	.LC359
	.align	3
.LC362:
	.xword	.LC361
	.align	3
.LC364:
	.xword	.LC363
	.align	3
.LC366:
	.xword	.LC365
	.align	3
.LC368:
	.xword	.LC367
	.align	3
.LC370:
	.xword	.LC369
	.align	3
.LC372:
	.xword	.LC371
	.align	3
.LC374:
	.xword	.LC373
	.align	3
.LC376:
	.xword	.LC375
	.align	3
.LC378:
	.xword	.LC377
	.align	3
.LC380:
	.xword	.LC379
	.align	3
.LC381:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC383:
	.xword	.LC382
	.align	3
.LC385:
	.xword	.LC384
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_PPSEqual
	.type	MVC_PPSEqual, %function
MVC_PPSEqual:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cbz	x19, .L2189
	cbz	x20, .L2190
	ldrb	w3, [x19]
	ldrb	w2, [x20]
	ldrb	w1, [x19, 1]
	ldrb	w0, [x20, 1]
	cmp	w3, w2
	ldp	w4, w2, [x19, 24]
	ccmp	w1, w0, 0, eq
	ldr	w5, [x19, 36]
	ldp	w3, w0, [x20, 24]
	cset	w1, eq
	ldrb	w7, [x19, 2]
	cmp	w4, w3
	ldr	w3, [x19, 40]
	ccmp	w2, w0, 0, eq
	ldr	w6, [x19, 48]
	ldp	w4, w0, [x20, 36]
	cset	w2, eq
	ldr	w12, [x19, 60]
	cmp	w5, w4
	ldrb	w5, [x20, 2]
	ccmp	w3, w0, 0, eq
	ldr	w3, [x19, 44]
	cset	w4, eq
	cmp	w7, w5
	ldp	w0, w5, [x20, 44]
	ldr	w7, [x19, 52]
	and	w1, w1, w2
	ccmp	w3, w0, 0, eq
	ldr	w3, [x20, 52]
	cset	w0, eq
	cmp	w6, w5
	ldrb	w5, [x20, 3]
	and	w1, w4, w1
	ldrb	w6, [x19, 3]
	ccmp	w7, w3, 0, eq
	cset	w3, eq
	and	w0, w0, w1
	cmp	w6, w5
	ldrb	w1, [x20, 4]
	ldrb	w5, [x19, 4]
	and	w0, w3, w0
	ldr	w4, [x20, 60]
	ccmp	w5, w1, 0, eq
	ldrb	w2, [x19, 18]
	ldrb	w3, [x20, 18]
	cset	w1, eq
	cmp	w4, w12
	and	w0, w1, w0
	ccmp	w3, w2, 0, eq
	cset	w1, eq
	and	w0, w1, w0
	cbz	w2, .L2171
	lsl	w12, w12, 1
	cmn	w12, #6
	beq	.L2171
	add	x10, x19, 1984
	add	x9, x20, 1984
	mov	x11, x20
	add	x7, x19, 64
	mov	w8, -6
	mov	x1, 0
	.p2align 2
.L2176:
	add	x3, x19, x1
	add	x2, x20, x1
	ldrsb	w3, [x3, 5]
	ldrsb	w2, [x2, 5]
	cmp	w2, w3
	cset	w2, eq
	and	w0, w0, w2
	cbz	w3, .L2172
	cmp	w1, 5
	bhi	.L2173
	ldr	w3, [x9]
	ldr	w2, [x10]
	cmp	w3, w2
	cset	w3, eq
	and	w0, w0, w3
	cbnz	w2, .L2172
	sub	x2, x7, #64
	mov	x3, x11
	.p2align 2
.L2174:
	ldr	w5, [x2, 64]
	add	x2, x2, 4
	ldr	w4, [x3, 64]
	add	x3, x3, 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x7, x2
	and	w0, w0, w4
	bne	.L2174
	.p2align 2
.L2172:
	add	w8, w8, 1
	add	x1, x1, 1
	cmp	w8, w12
	add	x10, x10, 4
	add	x9, x9, 4
	add	x11, x11, 64
	add	x7, x7, 64
	bne	.L2176
.L2171:
	ldr	w1, [x20, 56]
	ldr	w2, [x19, 56]
	cmp	w2, w1
	cset	w1, eq
	tst	w1, w0
	csetm	w0, eq
.L2169:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2173:
	uxtw	x2, w8
	add	x3, x2, 500
	lsl	x3, x3, 2
	add	x4, x19, x3
	add	x3, x20, x3
	ldr	w4, [x4, 8]
	ldr	w3, [x3, 8]
	cmp	w3, w4
	cset	w3, eq
	and	w0, w0, w3
	cbnz	w4, .L2172
	lsl	x2, x2, 8
	add	x3, x2, 448
	add	x2, x2, 704
	add	x6, x19, x2
	add	x2, x19, x3
	add	x3, x20, x3
	.p2align 2
.L2175:
	ldr	w5, [x2], 4
	ldr	w4, [x3], 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x6, x2
	and	w0, w0, w4
	bne	.L2175
	add	w8, w8, 1
	add	x1, x1, 1
	cmp	w8, w12
	add	x10, x10, 4
	add	x9, x9, 4
	add	x11, x11, 64
	add	x7, x7, 64
	bne	.L2176
	b	.L2171
.L2190:
	ldr	x2, .LC386
	mov	w3, 10242
	ldr	x1, .LC387
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2169
.L2189:
	ldr	x2, .LC386
	mov	w3, 10241
	ldr	x1, .LC387
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2169
	.size	MVC_PPSEqual, .-MVC_PPSEqual
	.align	3
.LC386:
	.xword	.LC36
	.align	3
.LC387:
	.xword	.LC38
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecPPS
	.type	MVC_DecPPS, %function
MVC_DecPPS:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC389
	mov	x0, x19
	bl	mvc_ue_v
	mov	w20, w0
	ldr	w0, [x19, 40]
	sub	w0, w0, #1
	cmp	w20, w0
	bls	.L2192
	ldr	x1, .LC391
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 40]
	sub	w0, w1, #1
	cmp	w20, w0
	bls	.L2199
	ldr	x0, .LC392
	ldr	x4, [x0]
	cbz	x4, .L2199
	ldr	w0, [x19, 120]
	mov	w3, 8
	stp	w20, w1, [x29, 56]
	add	x2, x29, 56
	mov	w1, 110
	blr	x4
	mov	w0, -1
.L2193:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2192:
	ldr	x1, .LC394
	mov	x0, x19
	bl	mvc_ue_v
	mov	w2, w0
	ldr	w0, [x19, 36]
	sub	w0, w0, #1
	cmp	w2, w0
	bhi	.L2203
	mov	w21, 2240
	ldr	x0, [x19, 280]
	umull	x21, w20, w21
	add	x0, x0, x21
	ldrb	w1, [x0, 19]
	cbz	w1, .L2195
	ldr	x22, .LC397
	mov	x0, x19
	mov	x1, x22
	stp	w20, w2, [x22, 24]
	bl	MVC_ProcessPPS
	cbnz	w0, .L2204
	ldr	x1, [x19, 280]
	mov	x0, x22
	add	x1, x1, x21
	bl	MVC_PPSEqual
	cbz	w0, .L2193
	ldr	x3, .LC400
	mov	w1, 1
	ldr	x0, [x19, 280]
	mov	x2, 2240
	strb	w1, [x22, 20]
	ldr	x3, [x3, 112]
	add	x0, x0, x21
	strb	w1, [x22, 19]
	mov	x1, x22
	blr	x3
	mov	w0, 0
	b	.L2193
	.p2align 3
.L2195:
	str	w20, [x0, 24]
	mov	x0, x19
	ldr	x1, [x19, 280]
	add	x1, x1, x21
	str	w2, [x1, 28]
	ldr	x1, [x19, 280]
	add	x1, x1, x21
	bl	MVC_ProcessPPS
	cbnz	w0, .L2205
	ldr	x1, [x19, 280]
	mov	w2, 1
	add	x1, x1, x21
	strb	w2, [x1, 20]
	ldr	x1, [x19, 280]
	add	x21, x1, x21
	strb	w2, [x21, 19]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2199:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, -1
	ldp	x29, x30, [sp], 64
	ret
.L2203:
	ldr	x1, .LC396
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2193
.L2204:
	ldr	x1, .LC399
	mov	w2, 10005
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2193
.L2205:
	ldr	x1, .LC399
	mov	w2, 10025
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 280]
	mov	w2, 1
	mov	w0, -1
	add	x1, x1, x21
	strb	w2, [x1, 20]
	ldr	x1, [x19, 280]
	add	x21, x1, x21
	strb	wzr, [x21, 19]
	b	.L2193
	.size	MVC_DecPPS, .-MVC_DecPPS
	.align	3
.LC389:
	.xword	.LC388
	.align	3
.LC391:
	.xword	.LC390
	.align	3
.LC392:
	.xword	g_event_report
	.align	3
.LC394:
	.xword	.LC393
	.align	3
.LC396:
	.xword	.LC395
	.align	3
.LC397:
	.xword	.LANCHOR2
	.align	3
.LC399:
	.xword	.LC398
	.align	3
.LC400:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	mvc_vui_parameters
	.type	mvc_vui_parameters, %function
mvc_vui_parameters:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x1, .LC402
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20]
	cbnz	w0, .L2248
	strb	wzr, [x20, 15]
.L2208:
	ldr	x1, .LC410
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 1]
	cbnz	w0, .L2249
.L2209:
	ldr	x1, .LC414
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 3]
	cbnz	w0, .L2250
	mov	w0, 5
	str	w0, [x20, 28]
.L2211:
	ldr	x1, .LC428
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 6]
	cbnz	w0, .L2251
.L2212:
	ldr	x1, .LC434
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 8]
	cbnz	w0, .L2252
.L2214:
	ldr	x1, .LC442
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 7]
	cbz	w0, .L2216
	ldr	x1, .LC444
	mov	x0, x19
	add	x24, x20, 60
	bl	mvc_ue_v
	ldr	x2, .LC446
	mov	w1, 4
	str	w0, [x24, 36]
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x24, 32]
	ldr	x2, .LC448
	mov	w1, 4
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x24, 33]
	ldr	w1, [x24, 36]
	cmp	w1, 31
	bhi	.L2219
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2219
	ldr	x27, .LC452
	mov	w21, 0
	ldr	x26, .LC454
	ldr	x25, .LC456
	.p2align 2
.L2220:
	uxtw	x22, w21
	mov	x1, x27
	mov	x0, x19
	add	w21, w21, 1
	add	x23, x20, x22, lsl 2
	bl	mvc_ue_v
	mov	x1, x26
	str	w0, [x23, 100]
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x23, 228]
	mov	x1, x25
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x24, x22]
	ldr	w0, [x24, 36]
	cmp	w0, w21
	bcs	.L2220
	ldr	x2, .LC458
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 296]
	ldr	x2, .LC460
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 300]
	ldr	x2, .LC462
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 304]
	ldr	x2, .LC464
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 308]
.L2216:
	ldr	x1, .LC466
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 10]
	cbnz	w0, .L2253
	ldrb	w0, [x20, 7]
	cbz	w0, .L2224
.L2225:
	ldr	x1, .LC468
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 14]
.L2224:
	ldr	x1, .LC470
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 11]
	ldr	x1, .LC472
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 12]
	cbnz	w0, .L2226
	ldrb	w0, [x19, 10]
.L2227:
	cmp	w0, wzr
	csetm	w0, ne
.L2247:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2252:
	ldr	x2, .LC436
	mov	w1, 32
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 52]
	ldr	x2, .LC438
	mov	w1, 32
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 56]
	ldr	x1, .LC440
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 9]
	bne	.L2214
	ldr	w1, [x20, 52]
	cbz	w1, .L2214
	ldr	w0, [x20, 56]
	udiv	w0, w0, w1
	lsr	w0, w0, 1
	str	w0, [x19, 56]
	b	.L2214
	.p2align 3
.L2251:
	ldr	x1, .LC430
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 44]
	ldr	x1, .LC432
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 48]
	b	.L2212
	.p2align 3
.L2250:
	ldr	x2, .LC416
	mov	w1, 3
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 28]
	ldr	x1, .LC418
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 4]
	ldr	x1, .LC420
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 5]
	cbz	w0, .L2211
	ldr	x2, .LC422
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 32]
	ldr	x2, .LC424
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 36]
	ldr	x2, .LC426
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 40]
	b	.L2211
	.p2align 3
.L2249:
	ldr	x1, .LC412
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 2]
	b	.L2209
	.p2align 3
.L2248:
	ldr	x2, .LC404
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	uxtb	w0, w0
	cmp	w0, 255
	strb	w0, [x20, 15]
	bne	.L2208
	ldr	x2, .LC406
	mov	w1, 16
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 16]
	ldr	x2, .LC408
	mov	w1, 16
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 20]
	b	.L2208
	.p2align 3
.L2253:
	ldr	x1, .LC444
	mov	x0, x19
	add	x24, x20, 372
	bl	mvc_ue_v
	ldr	x2, .LC446
	mov	w1, 4
	str	w0, [x24, 36]
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x24, 32]
	ldr	x2, .LC448
	mov	w1, 4
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x24, 33]
	ldr	w1, [x24, 36]
	cmp	w1, 31
	bhi	.L2219
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2219
	ldr	x27, .LC452
	mov	w21, 0
	ldr	x26, .LC454
	ldr	x25, .LC456
	.p2align 2
.L2222:
	uxtw	x22, w21
	mov	x1, x27
	mov	x0, x19
	add	w21, w21, 1
	add	x23, x20, x22, lsl 2
	bl	mvc_ue_v
	mov	x1, x26
	str	w0, [x23, 412]
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x23, 540]
	mov	x1, x25
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x24, x22]
	ldr	w0, [x24, 36]
	cmp	w0, w21
	bcs	.L2222
	ldr	x2, .LC458
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 296]
	ldr	x2, .LC460
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 300]
	ldr	x2, .LC462
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 304]
	ldr	x2, .LC464
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x24, 308]
	ldrb	w0, [x20, 7]
	cbnz	w0, .L2225
	ldrb	w0, [x20, 10]
	cbnz	w0, .L2225
	b	.L2224
	.p2align 3
.L2226:
	ldr	x1, .LC474
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 13]
	ldr	x1, .LC476
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 684]
	ldr	x1, .LC478
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 688]
	ldr	x1, .LC480
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 696]
	ldr	x1, .LC482
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 692]
	ldr	x1, .LC484
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 700]
	ldr	x1, .LC486
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 704]
	ldrb	w0, [x19, 10]
	cbz	w0, .L2227
	mov	w0, -1
	b	.L2247
.L2219:
	ldr	x1, .LC450
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2247
	.size	mvc_vui_parameters, .-mvc_vui_parameters
	.align	3
.LC402:
	.xword	.LC401
	.align	3
.LC404:
	.xword	.LC403
	.align	3
.LC406:
	.xword	.LC405
	.align	3
.LC408:
	.xword	.LC407
	.align	3
.LC410:
	.xword	.LC409
	.align	3
.LC412:
	.xword	.LC411
	.align	3
.LC414:
	.xword	.LC413
	.align	3
.LC416:
	.xword	.LC415
	.align	3
.LC418:
	.xword	.LC417
	.align	3
.LC420:
	.xword	.LC419
	.align	3
.LC422:
	.xword	.LC421
	.align	3
.LC424:
	.xword	.LC423
	.align	3
.LC426:
	.xword	.LC425
	.align	3
.LC428:
	.xword	.LC427
	.align	3
.LC430:
	.xword	.LC429
	.align	3
.LC432:
	.xword	.LC431
	.align	3
.LC434:
	.xword	.LC433
	.align	3
.LC436:
	.xword	.LC435
	.align	3
.LC438:
	.xword	.LC437
	.align	3
.LC440:
	.xword	.LC439
	.align	3
.LC442:
	.xword	.LC441
	.align	3
.LC444:
	.xword	.LC443
	.align	3
.LC446:
	.xword	.LC445
	.align	3
.LC448:
	.xword	.LC447
	.align	3
.LC450:
	.xword	.LC449
	.align	3
.LC452:
	.xword	.LC451
	.align	3
.LC454:
	.xword	.LC453
	.align	3
.LC456:
	.xword	.LC455
	.align	3
.LC458:
	.xword	.LC457
	.align	3
.LC460:
	.xword	.LC459
	.align	3
.LC462:
	.xword	.LC461
	.align	3
.LC464:
	.xword	.LC463
	.align	3
.LC466:
	.xword	.LC465
	.align	3
.LC468:
	.xword	.LC467
	.align	3
.LC470:
	.xword	.LC469
	.align	3
.LC472:
	.xword	.LC471
	.align	3
.LC474:
	.xword	.LC473
	.align	3
.LC476:
	.xword	.LC475
	.align	3
.LC478:
	.xword	.LC477
	.align	3
.LC480:
	.xword	.LC479
	.align	3
.LC482:
	.xword	.LC481
	.align	3
.LC484:
	.xword	.LC483
	.align	3
.LC486:
	.xword	.LC485
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_SPSEqual
	.type	MVC_SPSEqual, %function
MVC_SPSEqual:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cbz	x19, .L2286
	cbz	x20, .L2287
	ldr	w3, [x19, 736]
	ldr	w2, [x20, 736]
	ldrb	w1, [x19]
	ldrb	w0, [x20]
	cmp	w3, w2
	ldrb	w4, [x19, 1]
	ldrb	w2, [x20, 1]
	ccmp	w1, w0, 0, eq
	ldrb	w1, [x19, 2]
	cset	w3, eq
	ldrb	w0, [x20, 2]
	cmp	w4, w2
	ldr	w6, [x19, 740]
	ccmp	w1, w0, 0, eq
	ldr	w5, [x20, 740]
	cset	w0, eq
	ldr	w2, [x20, 744]
	ldr	w4, [x19, 744]
	and	w0, w3, w0
	ldrb	w1, [x19, 27]
	cmp	w6, w5
	ldrb	w3, [x20, 27]
	ccmp	w4, w2, 0, eq
	cset	w2, eq
	cmp	w3, w1
	and	w0, w2, w0
	cset	w2, eq
	and	w0, w2, w0
	cbz	w1, .L2258
	mov	x2, x19
	mov	x8, x20
	mov	w9, -6
	mov	x1, 0
	.p2align 2
.L2263:
	add	x4, x19, x1
	add	x3, x20, x1
	add	x6, x2, 64
	ldrsb	w3, [x3, 6]
	ldrsb	w4, [x4, 6]
	cmp	w3, w4
	cset	w3, eq
	and	w0, w0, w3
	cbz	w4, .L2259
	cmp	w1, 5
	bhi	.L2260
	mov	x3, x8
	.p2align 2
.L2261:
	ldr	w5, [x2, 752]
	add	x2, x2, 4
	ldr	w4, [x3, 752]
	add	x3, x3, 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x6, x2
	and	w0, w0, w4
	bne	.L2261
.L2259:
	add	x1, x1, 1
	add	w9, w9, 1
	cmp	x1, 8
	mov	x2, x6
	add	x8, x8, 64
	bne	.L2263
.L2258:
	ldr	w4, [x19, 2896]
	ldr	w3, [x20, 2896]
	ldr	w2, [x20, 2900]
	ldr	w1, [x19, 2900]
	cmp	w4, w3
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w0, w2, w0
	cbz	w1, .L2265
	cmp	w1, 1
	bne	.L2264
	ldrb	w4, [x19, 18]
	ldrb	w3, [x20, 18]
	ldr	w2, [x19, 2908]
	ldr	w1, [x20, 2908]
	cmp	w4, w3
	ldr	w5, [x19, 2912]
	ldr	w4, [x20, 2912]
	ccmp	w2, w1, 0, eq
	ldr	w2, [x20, 2916]
	cset	w3, eq
	ldr	w1, [x19, 2916]
	cmp	w5, w4
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w2, w3, w2
	and	w0, w0, w2
	cbz	w1, .L2264
	sub	w5, w1, #1
	add	x2, x20, 2920
	add	x5, x5, 731
	add	x1, x19, 2920
	add	x5, x19, x5, lsl 2
	.p2align 2
.L2267:
	ldr	w4, [x1], 4
	ldr	w3, [x2], 4
	cmp	w4, w3
	cset	w3, eq
	cmp	x1, x5
	and	w0, w0, w3
	bne	.L2267
.L2264:
	ldr	w4, [x19, 3944]
	ldr	w3, [x20, 3944]
	ldrb	w2, [x19, 19]
	ldrb	w1, [x20, 19]
	cmp	w4, w3
	ldr	w6, [x19, 3948]
	ldr	w4, [x20, 3948]
	ccmp	w2, w1, 0, eq
	ldr	w5, [x19, 3952]
	cset	w2, eq
	ldr	w1, [x20, 3952]
	cmp	w6, w4
	ldrb	w3, [x19, 20]
	ldrb	w4, [x20, 20]
	ccmp	w5, w1, 0, eq
	cset	w1, eq
	cmp	w4, w3
	and	w2, w2, w1
	cset	w1, eq
	and	w1, w2, w1
	and	w0, w1, w0
	cbnz	w3, .L2268
	ldrb	w1, [x20, 21]
	ldrb	w2, [x19, 21]
	cmp	w2, w1
	cset	w1, eq
	and	w0, w0, w1
.L2268:
	ldrb	w4, [x19, 22]
	ldrb	w3, [x20, 22]
	ldrb	w2, [x20, 23]
	ldrb	w1, [x19, 23]
	cmp	w4, w3
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w0, w2, w0
	cbnz	w1, .L2288
.L2269:
	ldrb	w1, [x20, 24]
	ldrb	w2, [x19, 24]
	cmp	w2, w1
	cset	w1, eq
	tst	w1, w0
	csetm	w0, eq
.L2256:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2288:
	ldr	w4, [x19, 3956]
	ldr	w3, [x20, 3956]
	ldr	w2, [x19, 3960]
	ldr	w1, [x20, 3960]
	cmp	w4, w3
	ldr	w5, [x19, 3964]
	ldr	w4, [x20, 3964]
	ccmp	w2, w1, 0, eq
	ldr	w3, [x19, 3968]
	cset	w2, eq
	ldr	w1, [x20, 3968]
	cmp	w5, w4
	ccmp	w3, w1, 0, eq
	cset	w1, eq
	and	w1, w2, w1
	and	w0, w0, w1
	b	.L2269
	.p2align 3
.L2260:
	ubfiz	x3, x9, 8, 32
	add	x4, x3, 1136
	add	x7, x3, 1392
	add	x7, x19, x7
	add	x3, x19, x4
	add	x4, x20, x4
	.p2align 2
.L2262:
	ldr	w6, [x3], 4
	ldr	w5, [x4], 4
	cmp	w6, w5
	cset	w5, eq
	cmp	x3, x7
	and	w0, w0, w5
	bne	.L2262
	add	x1, x1, 1
	add	x6, x2, 64
	cmp	x1, 8
	add	w9, w9, 1
	mov	x2, x6
	add	x8, x8, 64
	bne	.L2263
	b	.L2258
	.p2align 3
.L2265:
	ldr	w1, [x20, 2904]
	ldr	w2, [x19, 2904]
	cmp	w2, w1
	cset	w1, eq
	and	w0, w0, w1
	b	.L2264
.L2287:
	ldr	x2, .LC487
	mov	w3, 10486
	ldr	x1, .LC488
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2256
.L2286:
	ldr	x2, .LC487
	mov	w3, 10485
	ldr	x1, .LC488
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2256
	.size	MVC_SPSEqual, .-MVC_SPSEqual
	.align	3
.LC487:
	.xword	.LC36
	.align	3
.LC488:
	.xword	.LC38
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetDar
	.type	MVC_GetDar, %function
MVC_GetDar:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w20, w0
	mov	x0, x30
	mov	w19, w1
	mov	w21, w2
	mov	w23, w3
	mov	w22, w4
	bl	_mcount
	cbz	w20, .L2300
	mov	w0, 255
	cmp	w20, 16
	ccmp	w20, w0, 4, gt
	bne	.L2300
	cmp	w19, wzr
	ccmp	w21, wzr, 4, ne
	ccmp	w20, w0, 0, eq
	beq	.L2300
	cmp	w20, 1
	beq	.L2301
	cmp	w20, w0
	beq	.L2291
	ldr	x0, .LC489
	sbfiz	x20, x20, 3, 32
	add	x0, x0, 104
	add	x1, x0, x20
	ldr	w19, [x0, x20]
	ldr	w21, [x1, 4]
.L2291:
	mul	w19, w19, w23
	mul	w2, w21, w22
	lsl	w19, w19, 10
	sdiv	w2, w19, w2
	subs	w1, w2, #2406
	bmi	.L2311
.L2292:
	subs	w0, w2, #2263
	bmi	.L2312
	cmp	w1, w0
	mov	w19, 4
	blt	.L2290
.L2296:
	subs	w1, w2, #1820
	bmi	.L2313
	cmp	w0, w1
	mov	w19, 3
	blt	.L2290
.L2297:
	subs	w0, w2, #1365
	bmi	.L2314
.L2295:
	mov	w19, 2
	cmp	w1, w0
	csinc	w19, w19, wzr, lt
	b	.L2290
	.p2align 3
.L2300:
	mov	w19, 0
	mov	w2, 0
.L2290:
	ldr	x1, .LC491
	mov	w3, w19
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2301:
	mov	w19, 5
	mov	w2, 0
	b	.L2290
	.p2align 3
.L2312:
	mov	w0, 2263
	mov	w19, 4
	sub	w0, w0, w2
	cmp	w1, w0
	blt	.L2290
	b	.L2296
	.p2align 3
.L2311:
	mov	w1, 2406
	sub	w1, w1, w2
	b	.L2292
	.p2align 3
.L2313:
	mov	w1, 1820
	mov	w19, 3
	sub	w1, w1, w2
	cmp	w0, w1
	blt	.L2290
	b	.L2297
.L2314:
	mov	w0, 1365
	sub	w0, w0, w2
	b	.L2295
	.size	MVC_GetDar, .-MVC_GetDar
	.align	3
.LC489:
	.xword	.LANCHOR0
	.align	3
.LC491:
	.xword	.LC490
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ProcessSPS
	.type	MVC_ProcessSPS, %function
MVC_ProcessSPS:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L2434
	ldr	w0, [x20, 736]
	mov	w3, 128
	mov	w2, 100
	sub	w1, w0, #110
	and	w4, w0, -17
	and	w1, w1, -9
	cmp	w1, wzr
	ccmp	w4, w3, 4, ne
	cset	w1, eq
	cmp	w0, 122
	ccmp	w0, w2, 4, ne
	cset	w0, eq
	orr	w0, w1, w0
	cbnz	w0, .L2435
	mov	w0, 1
	strb	wzr, [x20, 27]
	str	w0, [x20, 748]
.L2328:
	ldr	x1, .LC517
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w0, 12
	str	w0, [x20, 2896]
	bhi	.L2341
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2341
	ldr	x1, .LC521
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w0, 2
	str	w0, [x20, 2900]
	bhi	.L2343
	ldrb	w1, [x19, 10]
	cbnz	w1, .L2343
	cbz	w0, .L2436
	cmp	w0, 1
	beq	.L2437
.L2347:
	ldr	x1, .LC579
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 3944]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2432
	ldr	x1, .LC541
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 19]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2432
	ldr	x1, .LC543
	mov	x0, x19
	bl	mvc_ue_v
	sub	w1, w0, #1
	cmp	w1, 509
	str	w0, [x20, 3948]
	bhi	.L2354
	ldrb	w1, [x19, 10]
	cbnz	w1, .L2354
	ldr	x1, .LC547
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 3952]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2432
	ldr	x1, .LC549
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 20]
	ldrb	w1, [x19, 10]
	cbnz	w1, .L2432
	cbnz	w0, .L2438
	ldr	x1, .LC581
	mov	x0, x19
	bl	mvc_u_1
	ldr	w22, [x20, 3952]
	strb	w0, [x20, 21]
	add	w22, w22, 1
	cmp	w22, 255
	bhi	.L2388
	ldr	w3, [x20, 3948]
	cmp	w22, 1
	mov	w23, 2
	add	w3, w3, 1
	ccmp	w3, 3, 0, hi
	cset	w0, ls
	cbz	w0, .L2358
.L2388:
	ldr	x1, .LC551
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2435:
	ldr	x1, .LC497
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w0, 1
	str	w0, [x20, 748]
	bhi	.L2319
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2319
	ldr	x1, .LC501
	mov	x0, x19
	bl	mvc_ue_v
	cbnz	w0, .L2321
	ldrb	w0, [x19, 10]
	cbz	w0, .L2322
.L2321:
	ldr	x1, .LC503
	mov	w0, 1
	bl	dprint_vfmw
.L2322:
	ldr	x1, .LC505
	mov	x0, x19
	bl	mvc_ue_v
	cbnz	w0, .L2323
	ldrb	w0, [x19, 10]
	cbz	w0, .L2324
.L2323:
	ldr	x1, .LC507
	mov	w0, 1
	bl	dprint_vfmw
.L2324:
	ldr	x1, .LC509
	mov	x0, x19
	bl	mvc_u_1
	cbnz	w0, .L2325
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2325
	ldr	x1, .LC513
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 27]
	bne	.L2328
	ldr	x25, .LC493
	add	x21, x29, 120
	ldr	x26, .LC514
	add	x23, x20, 6
	add	x22, x20, 2672
	add	x24, x20, 752
	mov	w28, 0
	ldr	x27, .LC515
	b	.L2334
	.p2align 3
.L2441:
	ldr	x0, .LC493
	cmp	w28, wzr
	ldr	x3, [x27, 104]
	mov	x2, 16
	csel	x1, x0, x1, eq
	mov	x0, x22
	blr	x3
.L2330:
	add	w28, w28, 1
	add	x21, x21, 4
	cmp	w28, 6
	add	x23, x23, 1
	add	x22, x22, 16
	add	x24, x24, 64
	beq	.L2439
.L2334:
	mov	x1, x26
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x23]
	cmp	w0, 1
	str	w0, [x21]
	add	x1, x25, 16
	beq	.L2440
	cmp	w28, wzr
	ccmp	w28, 3, 4, ne
	beq	.L2441
	ldr	x0, .LC515
	sub	w1, w28, #1
	add	x1, x1, 167
	mov	x2, 16
	add	w28, w28, 1
	add	x21, x21, 4
	ldr	x3, [x0, 104]
	add	x1, x20, x1, lsl 4
	mov	x0, x22
	add	x23, x23, 1
	add	x22, x22, 16
	add	x24, x24, 64
	blr	x3
	cmp	w28, 6
	bne	.L2334
.L2439:
	ldr	x24, .LC515
	add	x21, x20, 2768
	ldr	x23, .LC493
	add	x22, x20, 1136
	mov	x27, 0
	add	x25, x29, 104
	mov	x28, x24
.L2339:
	mov	x1, x26
	mov	x0, x19
	bl	mvc_u_1
	cmp	w0, 1
	add	x2, x20, x27
	str	w0, [x25, x27, lsl 2]
	add	x1, x23, 96
	strb	w0, [x2, 12]
	beq	.L2442
	ldr	x3, [x24, 104]
	cbz	w27, .L2443
.L2338:
	mov	x0, x21
	mov	x2, 64
.L2428:
	blr	x3
.L2336:
	add	x27, x27, 1
	add	x21, x21, 64
	cmp	x27, 2
	add	x22, x22, 256
	bne	.L2339
	b	.L2328
	.p2align 3
.L2385:
	ldr	w2, [x0, 668]
	ldr	w1, [x0, 672]
	cmp	w24, w2
	ccmp	w22, w1, 2, ls
	bls	.L2384
	ldr	x0, .LC577
	ldr	x4, [x0]
	cbz	x4, .L2432
	ldr	w0, [x19, 120]
	mov	w3, 8
	strh	w2, [x29, 116]
	add	x2, x29, 112
	strh	w1, [x29, 118]
	mov	w1, 107
	strh	w24, [x29, 112]
	strh	w22, [x29, 114]
	blr	x4
	.p2align 2
.L2432:
	mov	w0, -1
.L2424:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2438:
	ldr	w22, [x20, 3952]
	strb	wzr, [x20, 21]
	sub	w0, w22, #1
	cmp	w0, 509
	bhi	.L2388
	ldr	w3, [x20, 3948]
	mov	w23, 1
	add	w22, w22, 1
	add	w3, w3, w23
.L2358:
	mul	w22, w23, w22
	lsl	w24, w3, 4
	mov	w1, 100663296
	lsl	w22, w22, 4
	mul	w0, w24, w22
	lsr	w0, w0, 1
	lsl	w21, w0, 2
	sub	w0, w21, w0
	cmp	w0, w1
	bhi	.L2444
	ldr	w2, [x20, 740]
	sub	w1, w2, #10
	cmp	w1, 31
	bls	.L2445
.L2360:
	ldr	x1, .LC556
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2436:
	ldr	x1, .LC525
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w0, 12
	str	w0, [x20, 2904]
	bhi	.L2346
	ldrb	w0, [x19, 10]
	cbz	w0, .L2347
.L2346:
	ldr	x1, .LC527
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2445:
	ldr	x3, .LC554
	ldrh	w1, [x3,w1,uxtw #1]
	adr	x3, .Lrtx2362
	add	x1, x3, w1, sxth #2
	br	x1
.Lrtx2362:
	.section	.rodata
	.align	0
	.align	2
.L2362:
	.2byte	(.L2361 - .Lrtx2362) / 4
	.2byte	(.L2395 - .Lrtx2362) / 4
	.2byte	(.L2364 - .Lrtx2362) / 4
	.2byte	(.L2364 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2364 - .Lrtx2362) / 4
	.2byte	(.L2365 - .Lrtx2362) / 4
	.2byte	(.L2367 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2367 - .Lrtx2362) / 4
	.2byte	(.L2368 - .Lrtx2362) / 4
	.2byte	(.L2369 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2360 - .Lrtx2362) / 4
	.2byte	(.L2371 - .Lrtx2362) / 4
	.2byte	(.L2371 - .Lrtx2362) / 4
	.text
	.p2align 3
.L2364:
	mov	w21, 60416
	movk	w21, 0xd, lsl 16
.L2363:
	udiv	w21, w21, w0
	mov	w2, 16
	ldr	x1, .LC558
	mov	w0, 21
	cmp	w21, w2
	csel	w21, w21, w2, ls
	mov	w2, w21
	bl	dprint_vfmw
	ldr	w2, [x20, 3944]
	cmp	w2, w21
	bhi	.L2372
	ldrb	w0, [x19, 10]
	cbz	w0, .L2373
.L2372:
	ldr	x1, .LC560
	mov	w3, w21
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x20, 3944]
	cmp	w0, 16
	bhi	.L2429
	cmp	w21, w0
	csel	w21, w21, w0, cs
.L2429:
	str	w21, [x20, 3944]
.L2373:
	ldr	x1, .LC562
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 22]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2432
	ldr	x1, .LC564
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 23]
	ldrb	w1, [x19, 10]
	cbnz	w1, .L2432
	cbnz	w0, .L2446
.L2375:
	add	x2, x20, 3840
	lsr	w1, w24, 1
	lsr	w0, w22, 1
	stp	w24, w22, [x2, 136]
	stp	w1, w0, [x2, 144]
.L2377:
	ldr	x1, .LC574
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x20, 24]
	ldrb	w1, [x19, 10]
	cbnz	w1, .L2432
	mov	w1, 2
	str	w1, [x20, 68]
	cbnz	w0, .L2447
	mov	w0, 5
	strb	wzr, [x20, 43]
	str	w0, [x20, 56]
	mov	w0, 0
.L2380:
	ldp	w1, w2, [x20, 44]
	mov	w4, w22
	mov	w3, w24
	bl	MVC_GetDar
	str	w0, [x20, 52]
	ldr	w1, [x20, 3944]
	cbz	w1, .L2383
	cmp	w21, w1
	csel	w21, w21, w1, ls
.L2383:
	ldr	x0, [x19, 224]
	cmp	w21, 16
	mov	w1, 16
	csel	w21, w21, w1, ls
	ldr	w1, [x0, 28]
	cmp	w1, 25
	beq	.L2448
.L2384:
	cmp	w22, 1919
	bls	.L2386
	mov	w0, 4
	cmp	w21, w0
	csel	w21, w21, w0, ls
.L2386:
	add	w21, w21, 1
	str	w21, [x20, 3972]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2371:
	mov	w21, 12582912
	b	.L2363
	.p2align 3
.L2367:
	mov	w21, 30208
	movk	w21, 0x2f, lsl 16
	b	.L2363
	.p2align 3
.L2440:
	mov	x0, x19
	mov	x4, x21
	mov	x2, x22
	mov	x1, x24
	mov	w3, 16
	bl	MVC_Scaling_List
	ldr	w0, [x21]
	cmp	w0, 1
	bne	.L2330
	ldr	x0, .LC493
	cmp	w28, 2
	mov	x2, 16
	add	x1, x0, 16
	csel	x1, x0, x1, ls
	ldr	x0, .LC515
	ldr	x3, [x0, 104]
	mov	x0, x22
	blr	x3
	b	.L2330
	.p2align 3
.L2319:
	ldr	x1, .LC499
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2341:
	ldr	x1, .LC519
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2446:
	ldr	x1, .LC566
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 3956]
	ldr	x1, .LC568
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 3960]
	ldr	x1, .LC570
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 3964]
	ldr	x1, .LC572
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 3968]
	ldr	w1, [x20, 3964]
	ldr	w2, [x20, 3956]
	lsl	w1, w1, 1
	ldr	w3, [x20, 3960]
	lsl	w2, w2, 1
	mul	w1, w1, w23
	sub	w4, w24, w2
	lsl	w23, w23, 1
	sub	w3, w4, w3, lsl 1
	sub	w4, w22, w1
	cmp	w3, wzr
	msub	w0, w23, w0, w4
	ccmp	w0, wzr, 4, gt
	ble	.L2375
	add	x4, x20, 3840
	add	w2, w2, w3, lsr 1
	add	w1, w1, w0, lsr 1
	stp	w3, w0, [x4, 136]
	stp	w2, w1, [x4, 144]
	b	.L2377
	.p2align 3
.L2447:
	add	x1, x20, 28
	mov	x0, x19
	bl	mvc_vui_parameters
	cbnz	w0, .L2431
	ldrb	w0, [x20, 40]
	cbz	w0, .L2431
	ldr	w2, [x20, 732]
	cmp	w2, wzr
	ccmp	w21, w2, 0, ne
	bcc	.L2449
	ldr	w21, [x20, 3944]
	ldrb	w0, [x20, 43]
	cmp	w21, w2
	csel	w21, w21, w2, cs
	b	.L2380
.L2449:
	ldr	x1, .LC576
	mov	w3, w21
	mov	w0, 1
	bl	dprint_vfmw
	.p2align 2
.L2431:
	ldrb	w0, [x20, 43]
	b	.L2380
	.p2align 3
.L2448:
	ldr	w1, [x0, 708]
	cbnz	w1, .L2384
	ldr	w1, [x0, 692]
	cmp	w21, w1
	bls	.L2385
	ldr	x0, .LC577
	ldr	x4, [x0]
	cbz	x4, .L2432
	ldr	w0, [x19, 120]
	mov	w3, 8
	stp	w21, w1, [x29, 112]
	add	x2, x29, 112
	mov	w1, 106
	blr	x4
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2325:
	ldr	x1, .LC511
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2437:
	ldr	x1, .LC529
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x20, 18]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2432
	ldr	x1, .LC531
	mov	x0, x19
	bl	mvc_se_v
	str	w0, [x20, 2908]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2432
	ldr	x1, .LC533
	mov	x0, x19
	bl	mvc_se_v
	str	w0, [x20, 2912]
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2432
	ldr	x1, .LC535
	mov	x0, x19
	bl	mvc_ue_v
	cmp	w0, 255
	str	w0, [x20, 2916]
	bhi	.L2350
	ldrb	w1, [x19, 10]
	cbnz	w1, .L2350
	cbz	w0, .L2347
	ldr	x22, .LC539
	mov	w21, 0
	b	.L2353
	.p2align 3
.L2450:
	ldr	w0, [x20, 2916]
	cmp	w0, w21
	bls	.L2347
.L2353:
	mov	x1, x22
	mov	x0, x19
	bl	mvc_se_v
	add	x1, x20, x21, uxtw 2
	add	w21, w21, 1
	str	w0, [x1, 2920]
	ldrb	w0, [x19, 10]
	cbz	w0, .L2450
	b	.L2432
	.p2align 3
.L2354:
	ldr	x1, .LC545
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2395:
	mov	w21, 17920
	movk	w21, 0x5, lsl 16
	b	.L2363
	.p2align 3
.L2368:
	mov	w21, 30720
	movk	w21, 0x69, lsl 16
	b	.L2363
	.p2align 3
.L2365:
	mov	w21, 55296
	movk	w21, 0x1b, lsl 16
	b	.L2363
	.p2align 3
.L2361:
	mov	w21, 20992
	movk	w21, 0x2, lsl 16
	b	.L2363
	.p2align 3
.L2369:
	mov	w21, 7864320
	b	.L2363
.L2350:
	ldr	x1, .LC537
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
	.p2align 3
.L2442:
	mov	x0, x19
	add	x4, x25, x27, lsl 2
	mov	x2, x21
	mov	x1, x22
	mov	w3, 64
	bl	MVC_Scaling_List
	ldr	w0, [x25, x27, lsl 2]
	cmp	w0, 1
	bne	.L2336
	ldr	x1, .LC493
	mov	x2, 64
	mov	x0, x21
	ldr	x3, [x28, 104]
	add	x1, x1, 96
	cbnz	w27, .L2428
	ldr	x1, .LC493
	add	x1, x1, 32
	b	.L2428
	.p2align 3
.L2343:
	ldr	x1, .LC523
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
.L2444:
	ldr	x1, .LC553
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
.L2434:
	ldr	x2, .LC494
	mov	w3, 10675
	ldr	x1, .LC495
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2424
.L2443:
	ldr	x1, .LC493
	add	x1, x1, 32
	b	.L2338
	.size	MVC_ProcessSPS, .-MVC_ProcessSPS
	.align	3
.LC493:
	.xword	.LANCHOR1
	.align	3
.LC494:
	.xword	.LC36
	.align	3
.LC495:
	.xword	.LC38
	.align	3
.LC497:
	.xword	.LC496
	.align	3
.LC499:
	.xword	.LC498
	.align	3
.LC501:
	.xword	.LC500
	.align	3
.LC503:
	.xword	.LC502
	.align	3
.LC505:
	.xword	.LC504
	.align	3
.LC507:
	.xword	.LC506
	.align	3
.LC509:
	.xword	.LC508
	.align	3
.LC511:
	.xword	.LC510
	.align	3
.LC513:
	.xword	.LC512
	.align	3
.LC514:
	.xword	.LC379
	.align	3
.LC515:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC517:
	.xword	.LC516
	.align	3
.LC519:
	.xword	.LC518
	.align	3
.LC521:
	.xword	.LC520
	.align	3
.LC523:
	.xword	.LC522
	.align	3
.LC525:
	.xword	.LC524
	.align	3
.LC527:
	.xword	.LC526
	.align	3
.LC529:
	.xword	.LC528
	.align	3
.LC531:
	.xword	.LC530
	.align	3
.LC533:
	.xword	.LC532
	.align	3
.LC535:
	.xword	.LC534
	.align	3
.LC537:
	.xword	.LC536
	.align	3
.LC539:
	.xword	.LC538
	.align	3
.LC541:
	.xword	.LC540
	.align	3
.LC543:
	.xword	.LC542
	.align	3
.LC545:
	.xword	.LC544
	.align	3
.LC547:
	.xword	.LC546
	.align	3
.LC549:
	.xword	.LC548
	.align	3
.LC551:
	.xword	.LC550
	.align	3
.LC553:
	.xword	.LC552
	.align	3
.LC554:
	.xword	.L2362
	.align	3
.LC556:
	.xword	.LC555
	.align	3
.LC558:
	.xword	.LC557
	.align	3
.LC560:
	.xword	.LC559
	.align	3
.LC562:
	.xword	.LC561
	.align	3
.LC564:
	.xword	.LC563
	.align	3
.LC566:
	.xword	.LC565
	.align	3
.LC568:
	.xword	.LC567
	.align	3
.LC570:
	.xword	.LC569
	.align	3
.LC572:
	.xword	.LC571
	.align	3
.LC574:
	.xword	.LC573
	.align	3
.LC576:
	.xword	.LC575
	.align	3
.LC577:
	.xword	g_event_report
	.align	3
.LC579:
	.xword	.LC578
	.align	3
.LC581:
	.xword	.LC580
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecSPS
	.type	MVC_DecSPS, %function
MVC_DecSPS:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC583
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	mov	w21, w0
	ldr	x1, .LC585
	mov	x0, x19
	bl	mvc_u_1
	mov	w27, w0
	ldr	x1, .LC587
	mov	x0, x19
	bl	mvc_u_1
	mov	w26, w0
	ldr	x1, .LC589
	mov	x0, x19
	bl	mvc_u_1
	mov	w25, w0
	ldr	x1, .LC591
	mov	x0, x19
	bl	mvc_u_1
	mov	w24, w0
	ldr	x1, .LC593
	mov	x0, x19
	bl	mvc_u_1
	mov	w23, w0
	ldr	x1, .LC595
	mov	x0, x19
	bl	mvc_u_1
	str	w0, [x29, 108]
	ldr	x2, .LC597
	mov	w1, 2
	mov	x0, x19
	bl	mvc_u_v
	ldr	x2, .LC599
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	mov	w20, w0
	ldr	x1, .LC601
	mov	x0, x19
	bl	mvc_ue_v
	mov	w22, w0
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2489
	ldr	w0, [x19, 36]
	sub	w0, w0, #1
	cmp	w22, w0
	bhi	.L2491
	cmp	w21, 100
	mov	w28, w21
	beq	.L2457
	bgt	.L2458
	cmp	w21, 77
	beq	.L2457
	cmp	w21, 88
	beq	.L2459
	cmp	w21, 66
	bne	.L2456
	ldr	x1, .LC605
	mov	w0, 1
	bl	dprint_vfmw
.L2457:
	cmp	w20, 9
	mov	w2, w20
	bls	.L2488
.L2494:
	cmp	w20, 41
	bhi	.L2488
	mov	w20, 3992
	ldr	x0, [x19, 272]
	umull	x20, w22, w20
	add	x0, x0, x20
	ldrb	w1, [x0, 25]
	cbz	w1, .L2464
.L2495:
	ldr	x21, .LC614
	mov	x0, x19
	ldrb	w3, [x29, 108]
	add	x21, x21, 2240
	strb	w27, [x21]
	str	w28, [x21, 736]
	mov	x1, x21
	strb	w26, [x21, 1]
	strb	w25, [x21, 2]
	strb	w24, [x21, 3]
	strb	w23, [x21, 4]
	strb	w3, [x21, 5]
	str	w2, [x21, 740]
	str	w22, [x21, 744]
	bl	MVC_ProcessSPS
	cbnz	w0, .L2492
	ldr	x1, [x19, 272]
	mov	x0, x21
	add	x1, x1, x20
	bl	MVC_SPSEqual
	cbnz	w0, .L2493
.L2453:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2458:
	cmp	w21, 122
	beq	.L2461
	cmp	w21, 144
	beq	.L2461
	cmp	w21, 110
	beq	.L2461
.L2456:
	ldr	x1, .LC611
	mov	w2, w21
	mov	w0, 1
	mov	w28, 100
	bl	dprint_vfmw
	cmp	w20, 9
	mov	w2, w20
	bhi	.L2494
	.p2align 2
.L2488:
	ldr	x1, .LC613
	mov	w3, 41
	mov	w0, 1
	mov	w20, 3992
	bl	dprint_vfmw
	umull	x20, w22, w20
	ldr	x0, [x19, 272]
	mov	w2, 41
	add	x0, x0, x20
	ldrb	w1, [x0, 25]
	cbnz	w1, .L2495
.L2464:
	str	w28, [x0, 736]
	mov	x0, x19
	ldrb	w3, [x29, 108]
	ldr	x1, [x19, 272]
	strb	w27, [x1, x20]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	strb	w26, [x1, 1]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	strb	w25, [x1, 2]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	strb	w24, [x1, 3]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	strb	w23, [x1, 4]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	strb	w3, [x1, 5]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	str	w2, [x1, 740]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	str	w22, [x1, 744]
	ldr	x1, [x19, 272]
	add	x1, x1, x20
	bl	MVC_ProcessSPS
	cbnz	w0, .L2496
	ldr	x1, [x19, 272]
	mov	w2, 1
	add	x1, x1, x20
	strb	w2, [x1, 26]
	ldr	x1, [x19, 272]
	add	x20, x1, x20
	strb	w2, [x20, 25]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2491:
	ldr	x1, .LC602
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 36]
	sub	w0, w1, #1
	cmp	w22, w0
	bls	.L2489
	ldr	x0, .LC603
	ldr	x5, [x0]
	cbz	x5, .L2489
	ldr	w0, [x19, 120]
	mov	w3, 8
	stp	w22, w1, [x29, 120]
	add	x2, x29, 120
	mov	w1, 109
	blr	x5
.L2489:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, -1
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2461:
	ldr	x1, .LC609
	mov	w0, 1
	bl	dprint_vfmw
	cmp	w20, 9
	mov	w2, w20
	bhi	.L2494
	b	.L2488
	.p2align 3
.L2459:
	ldr	x1, .LC607
	mov	w0, 1
	bl	dprint_vfmw
	cmp	w20, 9
	mov	w2, w20
	bhi	.L2494
	b	.L2488
	.p2align 3
.L2493:
	ldr	x3, .LC617
	mov	w1, 1
	ldr	x0, [x19, 272]
	mov	x2, 3992
	strb	w1, [x21, 26]
	ldr	x3, [x3, 112]
	add	x0, x0, x20
	strb	w1, [x21, 25]
	mov	x1, x21
	blr	x3
	mov	w0, 0
	b	.L2453
	.p2align 3
.L2492:
	ldr	x1, .LC616
	mov	w3, w22
	mov	w2, 11219
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2453
.L2496:
	ldr	x1, .LC616
	mov	w2, 11246
	mov	w3, w22
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 272]
	mov	w2, 1
	mov	w0, -1
	add	x1, x1, x20
	strb	w2, [x1, 26]
	ldr	x1, [x19, 272]
	add	x20, x1, x20
	strb	wzr, [x20, 25]
	b	.L2453
	.size	MVC_DecSPS, .-MVC_DecSPS
	.align	3
.LC583:
	.xword	.LC582
	.align	3
.LC585:
	.xword	.LC584
	.align	3
.LC587:
	.xword	.LC586
	.align	3
.LC589:
	.xword	.LC588
	.align	3
.LC591:
	.xword	.LC590
	.align	3
.LC593:
	.xword	.LC592
	.align	3
.LC595:
	.xword	.LC594
	.align	3
.LC597:
	.xword	.LC596
	.align	3
.LC599:
	.xword	.LC598
	.align	3
.LC601:
	.xword	.LC600
	.align	3
.LC602:
	.xword	.LC395
	.align	3
.LC603:
	.xword	g_event_report
	.align	3
.LC605:
	.xword	.LC604
	.align	3
.LC607:
	.xword	.LC606
	.align	3
.LC609:
	.xword	.LC608
	.align	3
.LC611:
	.xword	.LC610
	.align	3
.LC613:
	.xword	.LC612
	.align	3
.LC614:
	.xword	.LANCHOR2
	.align	3
.LC616:
	.xword	.LC615
	.align	3
.LC617:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FreeMvcExtMem
	.type	MVC_FreeMvcExtMem, %function
MVC_FreeMvcExtMem:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	MVC_FreeMvcExtMem, .-MVC_FreeMvcExtMem
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FreeMvcVuiExtMem
	.type	MVC_FreeMvcVuiExtMem, %function
MVC_FreeMvcVuiExtMem:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	MVC_FreeMvcVuiExtMem, .-MVC_FreeMvcVuiExtMem
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ProcessSUBSPSMvcExt
	.type	MVC_ProcessSUBSPSMvcExt, %function
MVC_ProcessSUBSPSMvcExt:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L2559
	ldr	x1, .LC622
	mov	x0, x20
	bl	mvc_ue_v
	cmp	w0, 1
	str	w0, [x19, 4]
	bhi	.L2502
	ldrb	w1, [x20, 10]
	cbnz	w1, .L2502
	ldr	x22, .LC626
	mov	w21, 0
	.p2align 2
.L2504:
	mov	x1, x22
	mov	x0, x20
	bl	mvc_ue_v
	add	x1, x19, x21, uxtw 2
	add	w21, w21, 1
	str	w0, [x1, 8]
	ldr	w0, [x19, 4]
	cmp	w0, w21
	bcs	.L2504
	str	wzr, [x19, 16]
	str	wzr, [x19, 24]
	cbz	w0, .L2505
	ldr	x27, .LC628
	mov	w25, 1
	ldr	x23, .LC634
	mov	w26, 15
	ldr	x24, .LC638
	.p2align 2
.L2515:
	uxtw	x22, w25
	mov	x1, x27
	mov	x0, x20
	add	x21, x19, x22, lsl 2
	bl	mvc_ue_v
	str	w0, [x21, 16]
	ldr	w1, [x19, 4]
	cmp	w1, 15
	csel	w1, w1, w26, ls
	cmp	w0, w1
	bhi	.L2506
	ldrb	w1, [x20, 10]
	cbnz	w1, .L2506
	mov	w28, 0
	cbz	w0, .L2511
	.p2align 2
.L2510:
	mov	x1, x23
	mov	x0, x20
	bl	mvc_ue_v
	add	x1, x22, x28, uxtw
	add	w28, w28, 1
	add	x1, x1, 8
	str	w0, [x19, x1, lsl 2]
	ldr	w0, [x21, 16]
	cmp	w28, w0
	bcc	.L2510
.L2511:
	ldr	x1, .LC630
	mov	x0, x20
	bl	mvc_ue_v
	str	w0, [x21, 24]
	ldr	w1, [x19, 4]
	cmp	w1, 15
	csel	w2, w1, w26, ls
	cmp	w0, w2
	bhi	.L2508
	ldrb	w2, [x20, 10]
	cbnz	w2, .L2508
	mov	w28, 0
	cbz	w0, .L2513
	.p2align 2
.L2514:
	mov	x1, x24
	mov	x0, x20
	bl	mvc_ue_v
	add	x1, x22, x28, uxtw
	add	w28, w28, 1
	add	x1, x1, 10
	str	w0, [x19, x1, lsl 2]
	ldr	w0, [x21, 24]
	cmp	w28, w0
	bcc	.L2514
	ldr	w1, [x19, 4]
.L2513:
	add	w25, w25, 1
	cmp	w1, w25
	bcs	.L2515
	str	wzr, [x19, 48]
	str	wzr, [x19, 56]
	cbz	w1, .L2529
	ldr	x27, .LC642
	mov	w25, 1
	ldr	x23, .LC648
	mov	w26, 15
	ldr	x24, .LC652
	.p2align 2
.L2528:
	uxtw	x22, w25
	mov	x1, x27
	mov	x0, x20
	add	x21, x19, x22, lsl 2
	bl	mvc_ue_v
	str	w0, [x21, 48]
	ldr	w1, [x19, 4]
	cmp	w1, 15
	csel	w1, w1, w26, ls
	cmp	w0, w1
	bhi	.L2519
	ldrb	w1, [x20, 10]
	cbnz	w1, .L2519
	mov	w28, 0
	cbz	w0, .L2524
	.p2align 2
.L2523:
	mov	x1, x23
	mov	x0, x20
	bl	mvc_ue_v
	add	x1, x22, x28, uxtw
	add	w28, w28, 1
	add	x1, x1, 16
	str	w0, [x19, x1, lsl 2]
	ldr	w0, [x21, 48]
	cmp	w28, w0
	bcc	.L2523
.L2524:
	ldr	x1, .LC644
	mov	x0, x20
	bl	mvc_ue_v
	str	w0, [x21, 56]
	ldr	w1, [x19, 4]
	cmp	w1, 15
	csel	w2, w1, w26, ls
	cmp	w0, w2
	bhi	.L2521
	ldrb	w2, [x20, 10]
	cbnz	w2, .L2521
	mov	w28, 0
	cbz	w0, .L2526
	.p2align 2
.L2527:
	mov	x1, x24
	mov	x0, x20
	bl	mvc_ue_v
	add	x1, x22, x28, uxtw
	add	w28, w28, 1
	add	x1, x1, 18
	str	w0, [x19, x1, lsl 2]
	ldr	w0, [x21, 56]
	cmp	w28, w0
	bcc	.L2527
	ldr	w1, [x19, 4]
.L2526:
	add	w25, w25, 1
	cmp	w25, w1
	bls	.L2528
.L2529:
	ldr	x1, .LC640
	mov	x0, x20
	bl	mvc_ue_v
	add	w1, w0, 1
	cmp	w1, 16
	str	w0, [x19, 80]
	str	w1, [x29, 140]
	bhi	.L2517
	ldrb	w0, [x20, 10]
	cbnz	w0, .L2517
	ldr	w0, [x29, 140]
	cbz	w0, .L2534
	add	x0, x19, 8192
	ldr	x22, .LC670
	add	x1, x19, 84
	add	x0, x0, 212
	stp	x1, x19, [x29, 120]
	stp	xzr, x0, [x29, 104]
	str	wzr, [x29, 144]
.L2533:
	ldr	x2, .LC656
	mov	w1, 8
	mov	x0, x20
	bl	mvc_u_v
	ldr	x21, [x29, 120]
	ldr	x1, .LC658
	str	w0, [x21]
	mov	x0, x20
	bl	mvc_ue_v
	add	w27, w0, 1
	cmp	w27, 64
	str	w0, [x21, 64]
	bhi	.L2531
	ldrb	w0, [x20, 10]
	cbnz	w0, .L2531
	cbz	w27, .L2542
	ldp	x26, x25, [x29, 104]
	ldr	x23, [x29, 128]
	mov	w24, 0
.L2532:
	ldr	x2, .LC662
	mov	w1, 3
	mov	x0, x20
	bl	mvc_u_v
	str	w0, [x23, 212]
	ldr	x1, .LC664
	mov	x0, x20
	bl	mvc_ue_v
	add	w2, w0, 1
	cmp	w2, 64
	str	w0, [x23, 4308]
	bhi	.L2535
	ldrb	w1, [x20, 10]
	cbnz	w1, .L2535
	cbz	w2, .L2540
	add	x21, x26, x0, uxtw
	mov	x28, x25
	add	x21, x21, 2102
	add	x21, x19, x21, lsl 2
	.p2align 2
.L2539:
	mov	x1, x22
	mov	x0, x20
	bl	mvc_ue_v
	str	w0, [x28], 4
	cmp	x28, x21
	bne	.L2539
.L2540:
	ldr	x1, .LC666
	mov	x0, x20
	bl	mvc_ue_v
	cmp	w0, 1023
	add	x1, x23, 270336
	str	w0, [x1, 212]
	bhi	.L2537
	ldrb	w1, [x20, 10]
	cbnz	w1, .L2537
	add	w24, w24, 1
	add	x23, x23, 4
	cmp	w27, w24
	add	x25, x25, 256
	add	x26, x26, 64
	bne	.L2532
.L2542:
	ldr	x1, [x29, 120]
	ldr	w0, [x29, 144]
	add	x1, x1, 4
	str	x1, [x29, 120]
	ldr	w1, [x29, 140]
	add	w0, w0, 1
	str	w0, [x29, 144]
	cmp	w1, w0
	ldr	x0, [x29, 128]
	add	x0, x0, 256
	str	x0, [x29, 128]
	ldr	x0, [x29, 112]
	add	x0, x0, 16384
	str	x0, [x29, 112]
	ldr	x0, [x29, 104]
	add	x0, x0, 4096
	str	x0, [x29, 104]
	bne	.L2533
.L2534:
	mov	w0, 0
	b	.L2554
	.p2align 3
.L2506:
	ldr	x1, .LC632
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L2554:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 160
	ret
.L2508:
	ldr	x1, .LC636
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 160
	ret
.L2519:
	ldr	x1, .LC646
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
.L2502:
	ldr	x1, .LC624
	mov	w3, 65534
	mov	w2, w0
	str	w3, [x19, 80]
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
.L2521:
	ldr	x1, .LC650
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
.L2505:
	str	wzr, [x19, 48]
	str	wzr, [x19, 56]
	b	.L2529
.L2537:
	ldr	x1, .LC672
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
.L2535:
	ldr	x0, [x29, 144]
	mov	w3, 65534
	ldr	x1, .LC668
	ubfiz	x28, x0, 6, 32
	mov	w0, 1
	add	x24, x28, x24, uxtw
	add	x19, x19, x24, lsl 2
	str	w3, [x19, 4308]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
.L2559:
	ldr	x2, .LC619
	mov	w3, 11366
	ldr	x1, .LC620
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
.L2517:
	mov	w0, 65534
	ldr	x1, .LC654
	ldr	w2, [x29, 140]
	str	w0, [x19, 80]
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
.L2531:
	ldr	x0, [x29, 144]
	mov	w3, 65534
	ldr	x1, .LC660
	mov	w2, w27
	add	x19, x19, x0, uxtw 2
	mov	w0, 1
	str	w3, [x19, 148]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2554
	.size	MVC_ProcessSUBSPSMvcExt, .-MVC_ProcessSUBSPSMvcExt
	.align	3
.LC619:
	.xword	.LC36
	.align	3
.LC620:
	.xword	.LC38
	.align	3
.LC622:
	.xword	.LC621
	.align	3
.LC624:
	.xword	.LC623
	.align	3
.LC626:
	.xword	.LC625
	.align	3
.LC628:
	.xword	.LC627
	.align	3
.LC630:
	.xword	.LC629
	.align	3
.LC632:
	.xword	.LC631
	.align	3
.LC634:
	.xword	.LC633
	.align	3
.LC636:
	.xword	.LC635
	.align	3
.LC638:
	.xword	.LC637
	.align	3
.LC640:
	.xword	.LC639
	.align	3
.LC642:
	.xword	.LC641
	.align	3
.LC644:
	.xword	.LC643
	.align	3
.LC646:
	.xword	.LC645
	.align	3
.LC648:
	.xword	.LC647
	.align	3
.LC650:
	.xword	.LC649
	.align	3
.LC652:
	.xword	.LC651
	.align	3
.LC654:
	.xword	.LC653
	.align	3
.LC656:
	.xword	.LC655
	.align	3
.LC658:
	.xword	.LC657
	.align	3
.LC660:
	.xword	.LC659
	.align	3
.LC662:
	.xword	.LC661
	.align	3
.LC664:
	.xword	.LC663
	.align	3
.LC666:
	.xword	.LC665
	.align	3
.LC668:
	.xword	.LC667
	.align	3
.LC670:
	.xword	.LC669
	.align	3
.LC672:
	.xword	.LC671
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ProcessSUBSPSMvcVuiExt
	.type	MVC_ProcessSUBSPSMvcVuiExt, %function
MVC_ProcessSUBSPSMvcVuiExt:
	stp	x29, x30, [sp, -208]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	mov	x26, x1
	bl	_mcount
	add	x0, x26, 278528
	ldr	x1, .LC674
	mov	x20, x0
	mov	x0, x19
	str	x20, [x29, 112]
	bl	mvc_ue_v
	cmp	w0, 1023
	str	w0, [x20, 560]
	bhi	.L2592
	ldr	x2, [x29, 112]
	uxtw	x0, w0
	add	x0, x0, 1
	add	x1, x26, 294912
	str	x0, [x29, 128]
	add	x0, x2, 432
	ldr	x25, .LC684
	add	x24, x1, 1368
	str	x0, [x29, 184]
	add	x0, x2, 820
	str	x26, [x29, 152]
	stp	x26, x26, [x29, 168]
	str	xzr, [x29, 192]
	str	x0, [x29, 160]
	.p2align 2
.L2578:
	ldr	x3, [x29, 168]
	mov	w1, 3
	ldr	x2, .LC678
	mov	x0, x19
	add	x22, x3, 278528
	ldr	w3, [x29, 192]
	str	w3, [x29, 148]
	bl	mvc_u_v
	strb	w0, [x22, 112]
	ldr	x1, .LC680
	mov	x0, x19
	bl	mvc_ue_v
	cmn	w0, #1
	ldr	x1, [x29, 176]
	add	x1, x1, 278528
	str	w0, [x1, 564]
	beq	.L2568
	uxtw	x20, w0
	ldr	x0, [x29, 192]
	ldr	x21, [x29, 160]
	add	x20, x20, x0, lsl 6
	add	x20, x20, 69632
	add	x20, x20, 206
	add	x20, x26, x20, lsl 2
	.p2align 2
.L2567:
	mov	x1, x25
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x21], 4
	cmp	x21, x20
	bne	.L2567
.L2568:
	ldr	x1, .LC682
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x22, 368]
	cbnz	w0, .L2593
.L2566:
	ldr	x1, .LC692
	mov	x0, x19
	bl	mvc_u_1
	uxtb	w0, w0
	ldr	x1, [x29, 184]
	strb	w0, [x1]
	cbz	w0, .L2569
	ldr	x1, .LC694
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x24]
	ldr	x2, .LC696
	mov	w1, 4
	mov	x0, x19
	bl	mvc_u_v
	ldr	x1, [x29, 152]
	ldr	x2, .LC698
	add	x1, x1, 294912
	mov	x20, x1
	str	x20, [x29, 136]
	strb	w0, [x1, 1364]
	mov	w1, 4
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 1365]
	ldr	w0, [x24]
	cmp	w0, 31
	bhi	.L2571
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2571
	ldr	x0, [x29, 192]
	mov	w28, 0
	ldr	w23, [x29, 148]
	and	x27, x0, 4294967295
	ldr	x21, .LC703
	lsl	x0, x27, 3
	lsl	x1, x23, 3
	sub	x0, x0, x27
	sub	x1, x1, x23
	lsl	x0, x0, 1
	lsl	x1, x1, 1
	sub	x0, x0, x27
	sub	x1, x1, x23
	lsl	x27, x0, 2
	lsl	x23, x1, 2
	sub	x27, x27, x0
	sub	x1, x23, x1
	lsl	x0, x27, 1
	str	x0, [x29, 200]
	ldr	x0, [x29, 112]
	mov	x27, x24
	ldr	x20, .LC705
	add	x23, x0, 112
	str	x22, [x29, 120]
	add	x0, x23, x1, lsl 3
	mov	w22, w28
	mov	x28, x0
	.p2align 2
.L2572:
	ldr	x1, .LC701
	mov	x0, x19
	uxtw	x23, w22
	add	w22, w22, 1
	bl	mvc_ue_v
	ldr	x1, [x29, 200]
	add	x1, x1, x23
	add	x23, x28, x23
	add	x23, x23, 16384
	add	x1, x26, x1, lsl 2
	add	x24, x1, 294912
	mov	x1, x21
	str	w0, [x24, 1372]
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x24, 1500]
	mov	x1, x20
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x23, 1220]
	ldr	w0, [x27]
	cmp	w0, w22
	bcs	.L2572
	ldr	x2, .LC707
	mov	w1, 5
	mov	x0, x19
	ldr	x22, [x29, 120]
	mov	x24, x27
	bl	mvc_u_v
	ldr	x20, [x29, 136]
	ldr	x2, .LC709
	mov	w1, 5
	str	w0, [x20, 1628]
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 1632]
	ldr	x2, .LC711
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 1636]
	ldr	x2, .LC713
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 1640]
.L2569:
	ldr	x1, .LC715
	add	x2, x22, 496
	mov	x0, x19
	str	x2, [x29, 136]
	bl	mvc_u_1
	uxtb	w0, w0
	strb	w0, [x22, 496]
	cbnz	w0, .L2594
	ldr	x0, [x29, 184]
	ldrb	w0, [x0]
	cbz	w0, .L2576
.L2577:
	ldr	x1, .LC717
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x22, 176]
.L2576:
	ldr	x2, [x29, 192]
	mov	x0, x19
	ldr	x1, .LC719
	add	x24, x24, 312
	add	x2, x2, 1
	mov	x20, x2
	ldr	x2, [x29, 168]
	str	x20, [x29, 192]
	add	x2, x2, 1
	str	x2, [x29, 168]
	ldr	x2, [x29, 176]
	add	x2, x2, 4
	str	x2, [x29, 176]
	ldr	x2, [x29, 152]
	add	x2, x2, 312
	str	x2, [x29, 152]
	bl	mvc_u_1
	strb	w0, [x22, 304]
	ldr	w1, [x29, 148]
	ldr	x0, [x29, 128]
	add	w1, w1, 1
	cmp	w1, 63
	ccmp	x0, x20, 0, ls
	ldr	x0, [x29, 184]
	add	x0, x0, 1
	str	x0, [x29, 184]
	ldr	x0, [x29, 160]
	add	x0, x0, 256
	str	x0, [x29, 160]
	bhi	.L2578
	mov	w0, 0
.L2588:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
	.p2align 3
.L2593:
	ldr	x0, [x29, 176]
	mov	w1, 32
	ldr	x2, .LC686
	add	x20, x0, 294912
	mov	x0, x19
	bl	mvc_u_v
	ldr	x2, .LC688
	mov	w1, 32
	str	w0, [x20, 820]
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 1076]
	ldr	x1, .LC690
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x22, 240]
	b	.L2566
	.p2align 3
.L2594:
	ldr	x1, .LC694
	mov	x0, x19
	bl	mvc_ue_v
	ldr	x1, [x29, 152]
	ldr	x2, .LC696
	add	x1, x1, 315392
	mov	x20, x1
	str	x20, [x29, 120]
	add	x28, x20, 856
	str	w0, [x1, 856]
	mov	w1, 4
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 852]
	ldr	x2, .LC698
	mov	w1, 4
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 853]
	ldr	w0, [x20, 856]
	cmp	w0, 31
	bhi	.L2571
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2571
	ldr	x0, [x29, 192]
	ldr	w23, [x29, 148]
	and	x27, x0, 4294967295
	ldr	x21, .LC703
	lsl	x0, x27, 3
	lsl	x1, x23, 3
	sub	x0, x0, x27
	sub	x1, x1, x23
	lsl	x0, x0, 1
	lsl	x1, x1, 1
	sub	x0, x0, x27
	sub	x1, x1, x23
	lsl	x27, x0, 2
	lsl	x23, x1, 2
	sub	x27, x27, x0
	sub	x1, x23, x1
	lsl	x0, x27, 1
	str	x0, [x29, 200]
	ldr	x0, [x29, 112]
	ldr	x20, .LC705
	add	x23, x0, 112
	add	x0, x23, x1, lsl 3
	mov	w23, 0
	stp	x22, x24, [x29, 96]
	mov	x24, x28
	mov	w22, w23
	mov	x28, x0
	.p2align 2
.L2574:
	ldr	x1, .LC701
	mov	x0, x19
	uxtw	x27, w22
	add	w22, w22, 1
	bl	mvc_ue_v
	ldr	x1, [x29, 200]
	add	x2, x1, x27
	mov	x1, x21
	add	x27, x28, x27
	add	x2, x26, x2, lsl 2
	add	x27, x27, 36864
	add	x23, x2, 315392
	str	w0, [x23, 860]
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x23, 988]
	mov	x1, x20
	mov	x0, x19
	bl	mvc_u_1
	strb	w0, [x27, 708]
	ldr	w0, [x24]
	cmp	w0, w22
	bcs	.L2574
	ldr	x2, .LC707
	mov	w1, 5
	mov	x0, x19
	ldp	x22, x24, [x29, 96]
	bl	mvc_u_v
	ldr	x20, [x29, 120]
	ldr	x2, .LC709
	mov	w1, 5
	str	w0, [x20, 1116]
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 1120]
	ldr	x2, .LC711
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 1124]
	ldr	x2, .LC713
	mov	w1, 5
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 1128]
	ldr	x0, [x29, 184]
	ldrb	w0, [x0]
	cbnz	w0, .L2577
	ldr	x0, [x29, 136]
	ldrb	w0, [x0]
	cbnz	w0, .L2577
	b	.L2576
	.p2align 3
.L2571:
	ldr	x1, .LC699
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 208
	ret
.L2592:
	ldr	x1, .LC676
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2588
	.size	MVC_ProcessSUBSPSMvcVuiExt, .-MVC_ProcessSUBSPSMvcVuiExt
	.align	3
.LC674:
	.xword	.LC673
	.align	3
.LC676:
	.xword	.LC675
	.align	3
.LC678:
	.xword	.LC677
	.align	3
.LC680:
	.xword	.LC679
	.align	3
.LC682:
	.xword	.LC681
	.align	3
.LC684:
	.xword	.LC683
	.align	3
.LC686:
	.xword	.LC685
	.align	3
.LC688:
	.xword	.LC687
	.align	3
.LC690:
	.xword	.LC689
	.align	3
.LC692:
	.xword	.LC691
	.align	3
.LC694:
	.xword	.LC693
	.align	3
.LC696:
	.xword	.LC695
	.align	3
.LC698:
	.xword	.LC697
	.align	3
.LC699:
	.xword	.LC449
	.align	3
.LC701:
	.xword	.LC700
	.align	3
.LC703:
	.xword	.LC702
	.align	3
.LC705:
	.xword	.LC704
	.align	3
.LC707:
	.xword	.LC706
	.align	3
.LC709:
	.xword	.LC708
	.align	3
.LC711:
	.xword	.LC710
	.align	3
.LC713:
	.xword	.LC712
	.align	3
.LC715:
	.xword	.LC714
	.align	3
.LC717:
	.xword	.LC716
	.align	3
.LC719:
	.xword	.LC718
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecSubSPS
	.type	MVC_DecSubSPS, %function
MVC_DecSubSPS:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x2, .LC721
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	mov	w20, w0
	ldr	x1, .LC723
	mov	x0, x19
	bl	mvc_u_1
	mov	w27, w0
	ldr	x1, .LC725
	mov	x0, x19
	bl	mvc_u_1
	mov	w26, w0
	ldr	x1, .LC727
	mov	x0, x19
	bl	mvc_u_1
	mov	w25, w0
	ldr	x1, .LC729
	mov	x0, x19
	bl	mvc_u_1
	str	w0, [x29, 120]
	ldr	x1, .LC731
	mov	x0, x19
	bl	mvc_u_1
	str	w0, [x29, 116]
	ldr	x1, .LC733
	mov	x0, x19
	bl	mvc_u_1
	mov	w22, w0
	ldr	x2, .LC735
	mov	w1, 2
	mov	x0, x19
	bl	mvc_u_v
	ldr	x2, .LC737
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	mov	w21, w0
	ldr	x1, .LC739
	mov	x0, x19
	bl	mvc_ue_v
	mov	w4, w0
	ldrb	w0, [x19, 10]
	cbnz	w0, .L2608
	cmp	w4, 31
	bhi	.L2618
	sub	w0, w20, #66
	cmp	w0, 78
	bls	.L2619
.L2598:
	ldr	x1, .LC746
	mov	w2, w20
	mov	w0, 1
	mov	w21, -1
	bl	dprint_vfmw
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2619:
	ldr	x1, .LC741
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx2600
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2600:
	.section	.rodata
	.align	0
	.align	2
.L2600:
	.byte	(.L2599 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2601 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2602 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2601 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2603 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2601 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2603 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2601 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2598 - .Lrtx2600) / 4
	.byte	(.L2603 - .Lrtx2600) / 4
	.text
	.p2align 3
.L2599:
	ldr	x1, .LC742
	mov	w0, 1
	str	w4, [x29, 104]
	bl	dprint_vfmw
	ldr	w4, [x29, 104]
.L2601:
	cmp	w21, 41
	bhi	.L2620
.L2604:
	uxtw	x23, w4
	mov	x0, x19
	lsl	x1, x23, 3
	sub	x1, x1, x23
	lsl	x1, x1, 2
	sub	x1, x1, x23
	lsl	x1, x1, 4
	sub	x1, x1, x23
	lsl	x2, x1, 2
	sub	x1, x2, x1
	lsl	x2, x1, 6
	add	x2, x1, x2
	lsl	x24, x2, 2
	ldrb	w2, [x29, 120]
	add	x5, x19, x24
	add	x1, x24, 286720
	add	x28, x5, 286720
	add	x1, x19, x1
	add	x1, x1, 2612
	str	x5, [x29, 104]
	strb	w2, [x28, 2615]
	ldrb	w2, [x29, 116]
	str	w21, [x28, 3352]
	strb	w27, [x28, 2612]
	strb	w26, [x28, 2613]
	strb	w25, [x28, 2614]
	strb	w2, [x28, 2616]
	strb	w22, [x28, 2617]
	str	w20, [x28, 3348]
	str	w4, [x28, 3356]
	bl	MVC_ProcessSPS
	mov	w21, w0
	ldr	x5, [x29, 104]
	cbnz	w0, .L2621
	mov	w1, 128
	cmp	w20, 118
	mov	w0, 1
	ccmp	w20, w1, 4, ne
	strb	w0, [x28, 2637]
	beq	.L2622
.L2606:
	lsl	x0, x23, 3
	mov	w2, 1
	sub	x0, x0, x23
	lsl	x0, x0, 2
	sub	x0, x0, x23
	lsl	x0, x0, 4
	sub	x0, x0, x23
	lsl	x1, x0, 2
	sub	x0, x1, x0
	lsl	x1, x0, 6
	add	x0, x0, x1
	add	x19, x19, x0, lsl 2
	add	x19, x19, 12288
	strb	w2, [x19, 2396]
.L2596:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2620:
	ldr	x1, .LC748
	mov	w2, w21
	mov	w0, 1
	str	w4, [x29, 104]
	mov	w21, 41
	bl	dprint_vfmw
	ldr	w4, [x29, 104]
	b	.L2604
	.p2align 3
.L2622:
	ldr	x1, .LC752
	add	x22, x5, 12288
	mov	x0, x19
	add	x20, x24, 12288
	add	x20, x19, x20
	bl	mvc_u_1
	add	x20, x20, 2396
	strb	w0, [x22, 2397]
	mov	x1, x20
	mov	x0, x19
	bl	MVC_ProcessSUBSPSMvcExt
	cbnz	w0, .L2623
	ldr	x1, .LC756
	mov	x0, x19
	bl	mvc_u_1
	str	w0, [x28, 2608]
	cbz	w0, .L2606
	mov	x1, x20
	mov	x0, x19
	bl	MVC_ProcessSUBSPSMvcVuiExt
	cbz	w0, .L2606
	ldr	x1, .LC758
	mov	w0, 1
	mov	w21, -1
	bl	dprint_vfmw
	strb	wzr, [x22, 2396]
	b	.L2596
	.p2align 3
.L2603:
	ldr	x1, .LC744
	mov	w0, 1
	str	w4, [x29, 104]
	bl	dprint_vfmw
	ldr	w4, [x29, 104]
	b	.L2601
	.p2align 3
.L2602:
	ldr	x1, .LC743
	mov	w0, 1
	str	w4, [x29, 104]
	bl	dprint_vfmw
	ldr	w4, [x29, 104]
	b	.L2601
	.p2align 3
.L2608:
	mov	w21, -1
	b	.L2596
	.p2align 3
.L2618:
	ldr	x1, .LC740
	mov	w0, 1
	mov	w21, -1
	bl	dprint_vfmw
	b	.L2596
	.p2align 3
.L2621:
	ldr	x1, .LC750
	mov	w0, 1
	str	x5, [x29, 120]
	mov	w21, -1
	bl	dprint_vfmw
	ldr	x5, [x29, 120]
	add	x5, x5, 12288
	strb	wzr, [x5, 2396]
	strb	wzr, [x28, 2637]
	b	.L2596
.L2623:
	ldr	x1, .LC754
	mov	w0, 1
	mov	w21, -1
	bl	dprint_vfmw
	strb	wzr, [x22, 2396]
	b	.L2596
	.size	MVC_DecSubSPS, .-MVC_DecSubSPS
	.align	3
.LC721:
	.xword	.LC720
	.align	3
.LC723:
	.xword	.LC722
	.align	3
.LC725:
	.xword	.LC724
	.align	3
.LC727:
	.xword	.LC726
	.align	3
.LC729:
	.xword	.LC728
	.align	3
.LC731:
	.xword	.LC730
	.align	3
.LC733:
	.xword	.LC732
	.align	3
.LC735:
	.xword	.LC734
	.align	3
.LC737:
	.xword	.LC736
	.align	3
.LC739:
	.xword	.LC738
	.align	3
.LC740:
	.xword	.LC395
	.align	3
.LC741:
	.xword	.L2600
	.align	3
.LC742:
	.xword	.LC604
	.align	3
.LC743:
	.xword	.LC606
	.align	3
.LC744:
	.xword	.LC608
	.align	3
.LC746:
	.xword	.LC745
	.align	3
.LC748:
	.xword	.LC747
	.align	3
.LC750:
	.xword	.LC749
	.align	3
.LC752:
	.xword	.LC751
	.align	3
.LC754:
	.xword	.LC753
	.align	3
.LC756:
	.xword	.LC755
	.align	3
.LC758:
	.xword	.LC757
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_PassBytes
	.type	MVC_PassBytes, %function
MVC_PassBytes:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x0, [x20, 240]
	cmp	w19, wzr
	ccmp	x0, xzr, 4, ne
	beq	.L2633
	ldrb	w2, [x0]
	sbfiz	x1, x2, 5, 32
	add	x1, x0, x1
	ldr	x1, [x1, 8]
	cbz	x1, .L2633
	ldr	w1, [x0, 76]
	cmp	w2, 1
	ccmp	w1, 2, 2, ls
	bhi	.L2633
	cmp	w1, w2
	bls	.L2633
	uxtw	x5, w2
	add	x3, x0, x5, lsl 5
	ldr	w1, [x3, 28]
	ldr	w3, [x3, 16]
	add	w1, w1, w19, lsl 3
	lsl	w3, w3, 3
	cmp	w1, w3
	bhi	.L2628
	b	.L2626
	.p2align 3
.L2629:
	ldr	w4, [x6, 28]
	ldr	w6, [x6, 16]
	sub	w4, w4, w3
	add	w1, w1, w4
	lsl	w3, w6, 3
	cmp	w1, w3
	bls	.L2626
.L2628:
	add	x0, x0, x5, lsl 5
	add	w5, w2, 1
	mov	x2, x5
	str	w3, [x0, 28]
	ldr	x0, [x20, 240]
	ldrb	w4, [x0]
	add	w4, w4, 1
	strb	w4, [x0]
	ldr	x0, [x20, 240]
	add	x6, x0, x5, lsl 5
	ldr	w4, [x0, 76]
	cmp	w4, w5
	bhi	.L2629
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2626:
	add	x5, x0, x5, lsl 5
	mov	w0, 1
	str	w1, [x5, 28]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2633:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_PassBytes, .-MVC_PassBytes
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_GetBytes
	.type	MVC_GetBytes, %function
MVC_GetBytes:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x19, x1
	mov	x0, x30
	mov	w20, w2
	bl	_mcount
	cbz	x19, .L2649
	ldr	x1, [x21, 240]
	cmp	x1, xzr
	ccmp	w20, wzr, 4, ne
	beq	.L2649
	ldrb	w2, [x1]
	sbfiz	x0, x2, 5, 32
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cbz	x0, .L2649
	ldr	w0, [x1, 76]
	cmp	w2, 1
	ccmp	w0, 2, 2, ls
	bhi	.L2649
	cmp	w0, w2
	bls	.L2649
	mov	w6, 0
.L2644:
	ubfiz	x7, x2, 5, 32
	add	x3, x1, x7
	ldr	w0, [x3, 28]
	ldr	x4, [x3, 8]
	ldr	w8, [x3, 16]
	add	w0, w0, 7
	ubfx	x0, x0, 3, 29
	add	x8, x4, x8
	add	x5, x4, x0
	cmp	x5, x8
	bcs	.L2639
	ldrb	w0, [x4, x0]
	add	w6, w6, 1
	strb	w0, [x19]
	cmp	w20, w6
	add	x19, x19, 1
	add	x5, x5, 1
	ldr	x0, [x21, 240]
	add	x0, x0, x7
	ldr	w3, [x0, 28]
	add	w3, w3, 8
	str	w3, [x0, 28]
	bhi	.L2642
	b	.L2641
	.p2align 3
.L2643:
	ldrb	w1, [x5], 1
	add	w6, w6, 1
	strb	w1, [x19], 1
	cmp	w20, w6
	ldr	x3, [x21, 240]
	add	x3, x3, x7
	ldr	w4, [x3, 28]
	add	w4, w4, 8
	str	w4, [x3, 28]
	bls	.L2641
.L2642:
	cmp	x8, x5
	bne	.L2643
	ldr	x1, [x21, 240]
.L2639:
	ldrb	w0, [x1]
	add	w2, w2, 1
	add	w0, w0, 1
	strb	w0, [x1]
	ldr	x1, [x21, 240]
	ldr	w0, [x1, 76]
	cmp	w0, w2
	bhi	.L2644
	mov	w0, w6
	b	.L2638
	.p2align 3
.L2641:
	mov	w0, w20
.L2638:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2649:
	mov	w0, -1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	MVC_GetBytes, .-MVC_GetBytes
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecFramePackingSEI
	.type	MVC_DecFramePackingSEI, %function
MVC_DecFramePackingSEI:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 11124736
	bl	_mcount
	add	x20, x21, 328
	ldr	x1, .LC760
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x20, 24]
	ldr	x2, .LC762
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	sxtb	w0, w0
	strb	w0, [x20, 1]
	cbz	w0, .L2657
.L2652:
	ldr	x2, .LC794
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x21, 330]
	mov	w1, 1
	strb	w1, [x21, 328]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2657:
	ldr	x2, .LC764
	mov	w1, 7
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 16]
	ldr	x2, .LC766
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 3]
	ldr	x2, .LC768
	mov	w1, 6
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x20, 20]
	ldr	x2, .LC770
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 4]
	ldr	x2, .LC772
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 5]
	ldr	x2, .LC774
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 6]
	ldr	x2, .LC776
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 7]
	ldr	x2, .LC778
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 8]
	ldr	x2, .LC780
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 9]
	ldrsb	w1, [x20, 3]
	cbnz	w1, .L2653
	ldr	w0, [x20, 16]
	cmp	w0, 5
	beq	.L2653
	ldr	x2, .LC782
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 10]
	ldr	x2, .LC784
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 11]
	ldr	x2, .LC786
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 12]
	ldr	x2, .LC788
	mov	w1, 1
	mov	x0, x19
	bl	mvc_u_v
	strb	w0, [x20, 13]
.L2653:
	ldr	x2, .LC790
	mov	w1, 8
	mov	x0, x19
	bl	mvc_u_v
	str	w0, [x21, 356]
	ldr	x1, .LC792
	mov	x0, x19
	bl	mvc_ue_v
	str	w0, [x21, 360]
	b	.L2652
	.size	MVC_DecFramePackingSEI, .-MVC_DecFramePackingSEI
	.align	3
.LC760:
	.xword	.LC759
	.align	3
.LC762:
	.xword	.LC761
	.align	3
.LC764:
	.xword	.LC763
	.align	3
.LC766:
	.xword	.LC765
	.align	3
.LC768:
	.xword	.LC767
	.align	3
.LC770:
	.xword	.LC769
	.align	3
.LC772:
	.xword	.LC771
	.align	3
.LC774:
	.xword	.LC773
	.align	3
.LC776:
	.xword	.LC775
	.align	3
.LC778:
	.xword	.LC777
	.align	3
.LC780:
	.xword	.LC779
	.align	3
.LC782:
	.xword	.LC781
	.align	3
.LC784:
	.xword	.LC783
	.align	3
.LC786:
	.xword	.LC785
	.align	3
.LC788:
	.xword	.LC787
	.align	3
.LC790:
	.xword	.LC789
	.align	3
.LC792:
	.xword	.LC791
	.align	3
.LC794:
	.xword	.LC793
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecPicTimingSEI
	.type	MVC_DecPicTimingSEI, %function
MVC_DecPicTimingSEI:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, 3992
	bl	_mcount
	add	x0, x20, 11112448
	ldr	x3, [x20, 272]
	ldr	w0, [x0, 280]
	umull	x19, w0, w19
	add	x19, x3, x19
	ldrb	w0, [x19, 25]
	cbnz	w0, .L2659
	ldr	w0, [x20, 36]
	cmp	w0, wzr
	ble	.L2660
	ldrb	w1, [x3, 25]
	cbnz	w1, .L2678
	sub	w1, w0, #1
	add	x0, x3, 3992
	add	x1, x1, 1
	lsl	x2, x1, 6
	sub	x2, x2, x1
	lsl	x2, x2, 1
	sub	x2, x2, x1
	lsl	x2, x2, 2
	sub	x1, x2, x1
	add	x1, x3, x1, lsl 3
	b	.L2663
	.p2align 3
.L2664:
	ldrb	w2, [x2]
	cbnz	w2, .L2661
.L2663:
	cmp	x0, x1
	mov	x19, x0
	add	x0, x0, 3992
	sub	x2, x0, #3967
	bne	.L2664
.L2660:
	ldr	x1, .LC796
	mov	w0, 1
	mov	w19, -1
	bl	dprint_vfmw
.L2665:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L2678:
	mov	x19, x3
	.p2align 2
.L2661:
	cbz	x19, .L2660
.L2659:
	ldrb	w0, [x19, 24]
	cbz	w0, .L2666
	ldrb	w0, [x19, 35]
	cbz	w0, .L2679
	ldr	w1, [x19, 388]
	ldr	w22, [x19, 392]
	add	w1, w1, 1
	add	w22, w22, 1
.L2669:
	ldr	x2, .LC798
	mov	x0, x20
	add	x21, x20, 11124736
	bl	mvc_u_v
	ldr	x2, .LC800
	mov	w1, w22
	str	w0, [x21, 368]
	mov	x0, x20
	bl	mvc_u_v
	str	w0, [x21, 372]
.L2666:
	ldrb	w0, [x19, 39]
	mov	w19, 0
	cbz	w0, .L2665
	ldr	x2, .LC802
	mov	x0, x20
	add	x20, x20, 11124736
	mov	w1, 4
	bl	mvc_u_v
	strb	w0, [x20, 364]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2679:
	ldrb	w0, [x19, 38]
	cbz	w0, .L2666
	ldr	w1, [x19, 700]
	ldr	w22, [x19, 704]
	add	w1, w1, 1
	add	w22, w22, 1
	b	.L2669
	.size	MVC_DecPicTimingSEI, .-MVC_DecPicTimingSEI
	.align	3
.LC796:
	.xword	.LC795
	.align	3
.LC798:
	.xword	.LC797
	.align	3
.LC800:
	.xword	.LC799
	.align	3
.LC802:
	.xword	.LC801
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecSEI
	.type	MVC_DecSEI, %function
MVC_DecSEI:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x26, x0
	mov	x0, x30
	add	x21, x29, 128
	mov	w23, 0
	bl	_mcount
	add	x20, x26, 872
	ldr	x0, [x26, 240]
	mov	w1, 32
	strb	wzr, [x21, -1]!
	str	w1, [x0, 28]
	b	.L2683
	.p2align 3
.L2842:
	mov	w1, 8
	mov	x0, x20
	bl	BsSkip
	cmp	w19, wzr
	ble	.L2682
	ldrb	w0, [x29, 127]
	cmp	w0, 255
	bne	.L2841
.L2683:
	mov	w2, 1
	mov	x1, x21
	mov	x0, x26
	bl	MVC_GetBytes
	mov	w19, w0
	ldr	w2, [x26, 896]
	ldr	w0, [x26, 904]
	ldrb	w1, [x29, 127]
	add	w0, w0, 8
	cmp	w0, w2, lsl 3
	add	w23, w23, w1
	bls	.L2842
.L2682:
	ldr	x1, .LC804
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2685
	.p2align 3
.L2841:
	ldr	x25, .LC810
	mov	w24, 0
	ldr	x22, .LC807
	add	x0, x25, 240
	str	x0, [x29, 104]
	.p2align 2
.L2819:
	mov	w19, 0
	b	.L2689
	.p2align 3
.L2844:
	mov	w1, 8
	mov	x0, x20
	bl	BsSkip
	cmp	w28, wzr
	ble	.L2687
	ldrb	w0, [x29, 127]
	cmp	w0, 255
	bne	.L2843
.L2689:
	mov	w2, 1
	mov	x1, x21
	mov	x0, x26
	bl	MVC_GetBytes
	mov	w28, w0
	ldr	w2, [x26, 896]
	ldr	w0, [x26, 904]
	ldrb	w1, [x29, 127]
	add	w0, w0, 8
	cmp	w0, w2, lsl 3
	add	w19, w19, w1
	bls	.L2844
.L2687:
	ldr	x1, .LC806
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2843:
	cmp	w19, 409600
	bgt	.L2687
	ldr	x0, [x26, 240]
	ldr	w1, [x0, 76]
	ldr	w2, [x0, 28]
	cmp	w1, 2
	beq	.L2845
.L2691:
	cmp	w23, 45
	bls	.L2846
.L2692:
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2847
.L2811:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2812
	.p2align 3
.L2813:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2812:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2813
	.p2align 2
.L2718:
	ldr	w0, [x26, 60]
	cmp	w0, 3
	bhi	.L2848
.L2814:
	mov	w23, 0
	b	.L2818
	.p2align 3
.L2816:
	mov	w1, 8
	mov	x0, x20
	bl	BsSkip
	cmp	w19, wzr
	ble	.L2817
	ldrb	w0, [x29, 127]
	cmp	w0, 255
	bne	.L2849
.L2818:
	mov	w2, 1
	mov	x1, x21
	mov	x0, x26
	bl	MVC_GetBytes
	mov	w19, w0
	ldr	w2, [x26, 896]
	ldr	w0, [x26, 904]
	ldrb	w1, [x29, 127]
	add	w0, w0, 8
	cmp	w0, w2, lsl 3
	add	w23, w23, w1
	bls	.L2816
.L2817:
	mov	w0, 0
.L2685:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L2849:
	cmp	w0, 128
	bne	.L2819
	b	.L2817
	.p2align 3
.L2846:
	ldrh	w0, [x22,w23,uxtw #1]
	adr	x1, .Lrtx2694
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx2694:
	.section	.rodata
	.align	0
	.align	2
.L2694:
	.2byte	(.L2693 - .Lrtx2694) / 4
	.2byte	(.L2695 - .Lrtx2694) / 4
	.2byte	(.L2696 - .Lrtx2694) / 4
	.2byte	(.L2697 - .Lrtx2694) / 4
	.2byte	(.L2698 - .Lrtx2694) / 4
	.2byte	(.L2699 - .Lrtx2694) / 4
	.2byte	(.L2700 - .Lrtx2694) / 4
	.2byte	(.L2701 - .Lrtx2694) / 4
	.2byte	(.L2702 - .Lrtx2694) / 4
	.2byte	(.L2703 - .Lrtx2694) / 4
	.2byte	(.L2704 - .Lrtx2694) / 4
	.2byte	(.L2705 - .Lrtx2694) / 4
	.2byte	(.L2706 - .Lrtx2694) / 4
	.2byte	(.L2707 - .Lrtx2694) / 4
	.2byte	(.L2708 - .Lrtx2694) / 4
	.2byte	(.L2709 - .Lrtx2694) / 4
	.2byte	(.L2710 - .Lrtx2694) / 4
	.2byte	(.L2711 - .Lrtx2694) / 4
	.2byte	(.L2712 - .Lrtx2694) / 4
	.2byte	(.L2713 - .Lrtx2694) / 4
	.2byte	(.L2714 - .Lrtx2694) / 4
	.2byte	(.L2715 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2692 - .Lrtx2694) / 4
	.2byte	(.L2716 - .Lrtx2694) / 4
	.text
.L2716:
	mov	w1, w19
	mov	x0, x26
	ldr	w28, [x26, 904]
	bl	MVC_DecFramePackingSEI
	ldr	w3, [x26, 904]
	sub	w0, w3, w28
	lsl	w28, w19, 3
	cmp	w0, w28
	bge	.L2805
	sub	w28, w28, w0
	ldr	w0, [x26, 896]
	cmp	w28, wzr
	add	w23, w28, 7
	csel	w23, w23, w28, lt
	lsl	w0, w0, 3
	asr	w23, w23, 3
	cmp	w23, wzr
	ble	.L2807
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2807
	mov	w27, 0
	b	.L2808
	.p2align 3
.L2809:
	ldr	w0, [x26, 896]
	ldr	w3, [x26, 904]
	lsl	w0, w0, 3
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2807
.L2808:
	mov	w1, 8
	mov	x0, x20
	add	w27, w27, 1
	bl	BsSkip
	cmp	w27, w23
	bne	.L2809
	ldr	w0, [x26, 896]
	ldr	w3, [x26, 904]
	lsl	w0, w0, 3
.L2807:
	asr	w1, w28, 31
	lsr	w2, w1, 29
	add	w1, w28, w2
	and	w1, w1, 7
	sub	w1, w1, w2
	add	w3, w1, w3
	cmp	w3, w0
	bls	.L2850
.L2805:
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbz	w0, #31, .L2718
	ldr	x1, .LC812
	mov	w3, 12844
	ldr	x2, .LC866
	mov	w0, 20
	bl	dprint_vfmw
	b	.L2718
.L2715:
	ldr	x1, .LC863
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2851
.L2801:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2802
	.p2align 3
.L2803:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2802:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2803
	ldr	w0, [x26, 60]
	cmp	w0, 3
	bls	.L2814
.L2848:
	ldr	x1, .LC865
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
.L2714:
	ldr	x1, .LC861
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2852
.L2798:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2799
	.p2align 3
.L2800:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2799:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2800
	b	.L2718
.L2713:
	ldr	x1, .LC859
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2853
.L2795:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2796
	.p2align 3
.L2797:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2796:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2797
	b	.L2718
.L2712:
	ldr	x1, .LC857
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2854
.L2792:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2793
	.p2align 3
.L2794:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2793:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2794
	b	.L2718
.L2711:
	ldr	x1, .LC855
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2855
.L2789:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2790
	.p2align 3
.L2791:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2790:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2791
	b	.L2718
.L2710:
	ldr	x1, .LC853
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2856
.L2786:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2787
	.p2align 3
.L2788:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2787:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2788
	b	.L2718
.L2709:
	ldr	x1, .LC851
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2857
.L2783:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2784
	.p2align 3
.L2785:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2784:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2785
	b	.L2718
.L2708:
	ldr	x1, .LC849
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2858
.L2780:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2781
	.p2align 3
.L2782:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2781:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2782
	b	.L2718
.L2707:
	ldr	x1, .LC847
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2859
.L2777:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2778
	.p2align 3
.L2779:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2778:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2779
	b	.L2718
.L2706:
	ldr	x1, .LC845
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2860
.L2774:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2775
	.p2align 3
.L2776:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2775:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2776
	b	.L2718
.L2705:
	ldr	x1, .LC843
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2861
.L2771:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2772
	.p2align 3
.L2773:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2772:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2773
	b	.L2718
.L2704:
	ldr	x1, .LC841
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2862
.L2768:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2769
	.p2align 3
.L2770:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2769:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2770
	b	.L2718
.L2703:
	ldr	x1, .LC839
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2863
.L2765:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2766
	.p2align 3
.L2767:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2766:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2767
	b	.L2718
.L2702:
	ldr	x1, .LC837
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2864
.L2762:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2763
	.p2align 3
.L2764:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2763:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2764
	b	.L2718
.L2701:
	ldr	x1, .LC835
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2865
.L2759:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2760
	.p2align 3
.L2761:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2760:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2761
	b	.L2718
.L2700:
	ldr	x1, .LC833
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	cmp	w19, wzr
	ble	.L2756
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2756
	mov	w23, 0
	b	.L2757
	.p2align 3
.L2758:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2756
.L2757:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2758
.L2756:
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbz	w0, #31, .L2718
	ldr	x2, .LC810
	mov	w3, 12416
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2718
.L2699:
	cbnz	w24, .L2698
	ldr	x1, .LC822
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
.L2735:
	ldr	w23, [x26, 60]
	ldr	w0, [x26, 120]
	add	x23, x26, x23, uxtw 3
	bl	GetUsd
	str	x0, [x23, 832]
	ldr	w0, [x26, 60]
	add	x0, x0, 104
	ldr	x1, [x26, x0, lsl 3]
	cbz	x1, .L2822
	ldr	x5, [x26, 240]
	cmp	w19, 1024
	mov	w7, 1024
	csel	w27, w19, w7, le
	ldrb	w3, [x5]
	ldr	w6, [x5, 76]
	cmp	w3, w6
	bcs	.L2823
	mov	w28, 0
	.p2align 2
.L2737:
	ubfiz	x2, x3, 5, 32
	add	w3, w3, 1
	add	x2, x5, x2
	cmp	w3, w6
	ldr	w0, [x2, 28]
	ldr	w2, [x2, 16]
	add	w0, w0, 7
	sub	w0, w2, w0, lsr 3
	add	w28, w28, w0
	bne	.L2737
	mov	w0, w28
.L2736:
	cmp	w27, w0
	csel	w28, w28, w27, gt
	cbnz	w24, .L2866
	mov	w23, 0
	mov	w25, 0
.L2739:
	sub	w2, w28, w23
	mov	x0, x26
	bl	MVC_GetBytes
	cmp	w0, wzr
	mov	w28, w0
	ble	.L2867
	sub	w0, w27, w25
	cmp	w28, w0
	blt	.L2868
	ldr	w0, [x26, 60]
	sub	w23, w19, w25
	cmp	w27, w19
	add	x0, x0, 104
	ldr	x0, [x26, x0, lsl 3]
	str	w23, [x0, 1052]
	bcc	.L2869
.L2750:
	cmp	w23, wzr
	ble	.L2745
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2745
	mov	w19, 0
	b	.L2752
	.p2align 3
.L2753:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2745
.L2752:
	mov	w1, 8
	mov	x0, x20
	add	w19, w19, 1
	bl	BsSkip
	cmp	w23, w19
	bne	.L2753
.L2745:
	ldr	w0, [x26, 60]
	add	x0, x0, 104
.L2839:
	ldr	x1, [x26, 224]
	ldr	x0, [x26, x0, lsl 3]
	ldr	x1, [x1, 56]
	str	x1, [x0, 1056]
	mov	w1, 8
	ldr	w0, [x26, 60]
	add	x0, x0, 104
	ldr	x0, [x26, x0, lsl 3]
	str	w1, [x0, 1040]
	ldr	x1, .LC829
	ldr	w0, [x26, 60]
	add	x0, x0, 104
	ldr	x5, [x1]
	ldr	w1, [x26, 84]
	ldr	x0, [x26, x0, lsl 3]
	str	w1, [x0, 1044]
	cbz	x5, .L2718
	ldr	w0, [x26, 60]
	mov	w3, 1064
	mov	w1, 6
	add	x0, x0, 104
	ldr	x2, [x26, x0, lsl 3]
	ldr	w0, [x26, 120]
	blr	x5
	b	.L2718
.L2697:
	ldr	x1, .LC818
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2870
.L2731:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2732
	.p2align 3
.L2733:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2732:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2733
	b	.L2718
.L2696:
	ldr	x1, .LC816
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2871
.L2728:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2729
	.p2align 3
.L2730:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2729:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2730
	b	.L2718
.L2695:
	ldr	x1, .LC814
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	ldr	w28, [x26, 904]
	mov	w1, w19
	mov	x0, x26
	bl	MVC_DecPicTimingSEI
	ldr	w3, [x26, 904]
	sub	w0, w3, w28
	lsl	w28, w19, 3
	cmp	w0, w28
	bge	.L2722
	sub	w28, w28, w0
	ldr	w0, [x26, 896]
	cmp	w28, wzr
	add	w23, w28, 7
	csel	w23, w23, w28, lt
	lsl	w0, w0, 3
	asr	w23, w23, 3
	cmp	w23, wzr
	ble	.L2724
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2724
	mov	w25, 0
	b	.L2725
	.p2align 3
.L2726:
	ldr	w0, [x26, 896]
	ldr	w3, [x26, 904]
	lsl	w0, w0, 3
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2724
.L2725:
	mov	w1, 8
	mov	x0, x20
	add	w25, w25, 1
	bl	BsSkip
	cmp	w25, w23
	bne	.L2726
	ldr	w0, [x26, 896]
	ldr	w3, [x26, 904]
	lsl	w0, w0, 3
.L2724:
	asr	w1, w28, 31
	lsr	w2, w1, 29
	add	w1, w28, w2
	and	w1, w1, 7
	sub	w1, w1, w2
	add	w3, w1, w3
	cmp	w3, w0
	bls	.L2872
.L2722:
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbz	w0, #31, .L2718
	ldr	x2, .LC810
	mov	w3, 12190
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2718
.L2693:
	ldr	x1, .LC809
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	MVC_PassBytes
	tbnz	w0, #31, .L2873
.L2717:
	cmp	w19, wzr
	ble	.L2718
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
	mov	w23, 0
	b	.L2719
	.p2align 3
.L2720:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2718
.L2719:
	mov	w1, 8
	mov	x0, x20
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2720
	b	.L2718
	.p2align 3
.L2845:
	ldr	w0, [x0, 60]
	add	w2, w2, w0
	lsr	w2, w2, 3
	b	.L2691
.L2868:
	ldr	w0, [x26, 60]
	add	x0, x0, 104
	ldr	x0, [x26, x0, lsl 3]
	str	w28, [x0, 1052]
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2745
	mov	w19, 0
	b	.L2746
	.p2align 3
.L2748:
	ldr	w1, [x26, 896]
	ldr	w0, [x26, 904]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2745
.L2746:
	mov	w1, 8
	mov	x0, x20
	add	w19, w19, 1
	bl	BsSkip
	cmp	w28, w19
	bne	.L2748
	b	.L2745
.L2698:
	ldr	x1, .LC820
	mov	w3, w19
	mov	w0, 20
	mov	w24, 1
	bl	dprint_vfmw
	b	.L2735
.L2866:
	strb	w24, [x1, 1026]
	mov	x0, x26
	ldr	x2, .LC824
	mov	w1, 8
	ldr	w3, [x26, 60]
	add	x3, x3, 104
	ldr	x23, [x26, x3, lsl 3]
	bl	mvc_u_v
	strb	w0, [x23, 1027]
	ldr	w0, [x26, 60]
	add	x0, x0, 104
	ldr	x23, [x26, x0, lsl 3]
	ldrb	w0, [x23, 1027]
	cmp	w0, 255
	beq	.L2740
	strb	wzr, [x23, 1028]
	mov	w23, 3
	mov	w25, w23
.L2741:
	ldr	w0, [x26, 60]
	mov	w1, 16
	ldr	x2, .LC828
	add	x0, x0, 104
	ldr	x5, [x26, x0, lsl 3]
	mov	x0, x26
	str	x5, [x29, 96]
	bl	mvc_u_v
	ldr	x5, [x29, 96]
	mov	w1, w23
	strh	w0, [x5, 1030]
	mov	x0, x26
	bl	MVC_PassBytes
	ldr	w0, [x26, 60]
	cmp	w28, w23
	add	x0, x0, 104
	beq	.L2839
	ldr	x1, [x26, x0, lsl 3]
	b	.L2739
.L2850:
	mov	x0, x20
	bl	BsSkip
	b	.L2805
.L2872:
	mov	x0, x20
	bl	BsSkip
	b	.L2722
.L2852:
	ldr	x2, .LC810
	mov	w3, 12766
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2798
.L2853:
	ldr	x2, .LC810
	mov	w3, 12740
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2795
.L2847:
	ldr	x1, .LC812
	mov	w3, 12856
	ldr	x2, [x29, 104]
	mov	w0, 20
	bl	dprint_vfmw
	b	.L2811
.L2851:
	ldr	x1, .LC812
	mov	w3, 12792
	ldr	x2, .LC866
	mov	w0, 20
	bl	dprint_vfmw
	b	.L2801
.L2870:
	ldr	x2, .LC810
	mov	w3, 12228
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2731
.L2856:
	ldr	x2, .LC810
	mov	w3, 12662
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2786
.L2857:
	ldr	x2, .LC810
	mov	w3, 12636
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2783
.L2854:
	ldr	x2, .LC810
	mov	w3, 12714
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2792
.L2863:
	ldr	x2, .LC810
	mov	w3, 12480
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2765
.L2871:
	ldr	x2, .LC810
	mov	w3, 12202
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2728
.L2873:
	ldr	x2, .LC810
	mov	w3, 12137
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2717
.L2855:
	ldr	x2, .LC810
	mov	w3, 12688
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2789
.L2860:
	ldr	x2, .LC810
	mov	w3, 12558
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2774
.L2861:
	ldr	x2, .LC810
	mov	w3, 12532
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2771
.L2858:
	ldr	x2, .LC810
	mov	w3, 12610
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2780
.L2859:
	ldr	x2, .LC810
	mov	w3, 12584
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2777
.L2864:
	ldr	x2, .LC810
	mov	w3, 12454
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2762
.L2865:
	ldr	x2, .LC810
	mov	w3, 12428
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2759
.L2862:
	ldr	x2, .LC810
	mov	w3, 12506
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2768
.L2869:
	sub	w1, w19, w27
	mov	x0, x26
	bl	MVC_PassBytes
	tbz	w0, #31, .L2750
	ldr	x2, .LC810
	mov	w3, 12364
	ldr	x1, .LC812
	mov	w0, 20
	add	x2, x2, 240
	bl	dprint_vfmw
	b	.L2750
.L2823:
	mov	w0, 0
	mov	w28, 0
	b	.L2736
.L2740:
	ldr	x2, .LC826
	mov	w1, 8
	mov	x0, x26
	bl	mvc_u_v
	strb	w0, [x23, 1028]
	mov	w23, 4
	mov	w25, w23
	b	.L2741
.L2867:
	ldr	x1, .LC831
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x26, 60]
	ldr	w0, [x26, 120]
	add	x1, x1, 104
	ldr	x1, [x26, x1, lsl 3]
	bl	FreeUsdByDec
	ldr	w1, [x26, 60]
	mov	w0, -1
	add	x1, x1, 104
	str	xzr, [x26, x1, lsl 3]
	b	.L2685
.L2822:
	mov	w0, -1
	b	.L2685
	.size	MVC_DecSEI, .-MVC_DecSEI
	.align	3
.LC804:
	.xword	.LC803
	.align	3
.LC806:
	.xword	.LC805
	.align	3
.LC807:
	.xword	.L2694
	.align	3
.LC809:
	.xword	.LC808
	.align	3
.LC810:
	.xword	.LANCHOR0
	.align	3
.LC812:
	.xword	.LC811
	.align	3
.LC814:
	.xword	.LC813
	.align	3
.LC816:
	.xword	.LC815
	.align	3
.LC818:
	.xword	.LC817
	.align	3
.LC820:
	.xword	.LC819
	.align	3
.LC822:
	.xword	.LC821
	.align	3
.LC824:
	.xword	.LC823
	.align	3
.LC826:
	.xword	.LC825
	.align	3
.LC828:
	.xword	.LC827
	.align	3
.LC829:
	.xword	g_event_report
	.align	3
.LC831:
	.xword	.LC830
	.align	3
.LC833:
	.xword	.LC832
	.align	3
.LC835:
	.xword	.LC834
	.align	3
.LC837:
	.xword	.LC836
	.align	3
.LC839:
	.xword	.LC838
	.align	3
.LC841:
	.xword	.LC840
	.align	3
.LC843:
	.xword	.LC842
	.align	3
.LC845:
	.xword	.LC844
	.align	3
.LC847:
	.xword	.LC846
	.align	3
.LC849:
	.xword	.LC848
	.align	3
.LC851:
	.xword	.LC850
	.align	3
.LC853:
	.xword	.LC852
	.align	3
.LC855:
	.xword	.LC854
	.align	3
.LC857:
	.xword	.LC856
	.align	3
.LC859:
	.xword	.LC858
	.align	3
.LC861:
	.xword	.LC860
	.align	3
.LC863:
	.xword	.LC862
	.align	3
.LC865:
	.xword	.LC864
	.align	3
.LC866:
	.xword	.LANCHOR0+240
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_InitOldSlice
	.type	MVC_InitOldSlice, %function
MVC_InitOldSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 11120640
	mov	w4, 32
	ldr	w8, [x19, 40]
	mov	w7, 23352
	movk	w7, 0x1, lsl 16
	mov	w6, 2
	strb	w4, [x0, 370]
	add	x4, x0, 256
	strb	wzr, [x0, 368]
	mov	w5, 7
	strb	w6, [x0, 369]
	mov	w3, 3
	strb	w5, [x0, 371]
	mov	w2, -1
	str	w8, [x0, 380]
	mov	w1, 262144
	str	w7, [x0, 384]
	stp	w3, w2, [x4, 136]
	stp	wzr, wzr, [x4, 144]
	str	w1, [x0, 408]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_InitOldSlice, .-MVC_InitOldSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_IsNewPicNal
	.type	MVC_IsNewPicNal, %function
MVC_IsNewPicNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x20, 3]
	and	w0, w0, 31
	sub	w0, w0, #1
	cmp	w0, 7
	bls	.L2884
.L2883:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2884:
	ldr	x1, .LC867
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx2878
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2878:
	.section	.rodata
	.align	0
	.align	2
.L2878:
	.byte	(.L2877 - .Lrtx2878) / 4
	.byte	(.L2883 - .Lrtx2878) / 4
	.byte	(.L2883 - .Lrtx2878) / 4
	.byte	(.L2883 - .Lrtx2878) / 4
	.byte	(.L2877 - .Lrtx2878) / 4
	.byte	(.L2883 - .Lrtx2878) / 4
	.byte	(.L2879 - .Lrtx2878) / 4
	.byte	(.L2879 - .Lrtx2878) / 4
	.text
	.p2align 3
.L2879:
	ldr	w0, [x19]
	mov	w1, 1
	str	w1, [x19]
	cmp	w0, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2877:
	ldr	w0, [x19]
	cbz	w0, .L2880
.L2882:
	mov	w0, -1
	str	wzr, [x19]
.L2885:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2880:
	ldr	w20, [x20, 4]
	rev	w20, w20
	mov	w0, w20
	bl	ZerosMS_32
	cmp	w0, 15
	bhi	.L2882
	mov	w1, 31
	str	wzr, [x19]
	sub	w0, w1, w0, lsl 1
	lsr	w20, w20, w0
	cmp	w20, 1
	csetm	w0, ne
	b	.L2885
	.size	MVC_IsNewPicNal, .-MVC_IsNewPicNal
	.align	3
.LC867:
	.xword	.L2878
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FindZeroBitsInSeg
	.type	MVC_FindZeroBitsInSeg, %function
MVC_FindZeroBitsInSeg:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x22, x0
	mov	x0, x30
	mov	w23, w1
	bl	_mcount
	cmp	x22, xzr
	ccmp	w23, wzr, 4, ne
	ble	.L2906
	and	w1, w23, 63
	add	w0, w23, 63
	cmp	w1, wzr
	cset	w26, ne
	cmp	w23, wzr
	csel	w0, w0, w23, lt
	add	w26, w26, w0, asr 6
	cmp	w26, wzr
	ble	.L2899
	ldr	x20, .LC871
	mov	w25, w23
	ldr	x27, .LC870
	mov	w19, 0
	mov	w24, 64
	add	x20, x20, 1880
	.p2align 2
.L2898:
	cmp	w25, 64
	uxtw	x1, w25
	csel	w21, w25, w24, ls
	ldr	x3, [x27, 104]
	sub	x1, x1, x21
	mov	x2, x21
	add	x1, x22, x1
	sub	x21, x21, #1
	mov	x0, x20
	blr	x3
	ldrb	w1, [x20, x21]
	add	x0, x20, x21
	cbnz	w1, .L2890
	cmp	x0, x20
	add	w19, w19, 1
	bne	.L2893
	b	.L2891
	.p2align 3
.L2894:
	add	w19, w19, 1
	beq	.L2891
.L2893:
	ldrb	w1, [x0, -1]!
	cmp	x0, x20
	cbz	w1, .L2894
.L2890:
	lsl	w0, w19, 3
.L2889:
	cmp	w19, w23
	mov	w1, 0
	bge	.L2895
	sxtw	x2, w23
	sub	x2, x2, #1
	sub	x19, x2, x19, sxtw
	ldrb	w2, [x22, x19]
	tbnz	x2, 0, .L2895
	lsr	w2, w2, 1
	mov	w1, 1
	.p2align 2
.L2897:
	tbnz	x2, 0, .L2895
	add	w1, w1, 1
	lsr	w2, w2, 1
	cmp	w1, 8
	bne	.L2897
.L2895:
	add	w0, w1, w0
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2891:
	subs	w26, w26, #1
	sub	w25, w25, #64
	bne	.L2898
	b	.L2890
.L2906:
	ldr	x1, .LC869
	mov	w2, w23
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x27, [sp, 80]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
.L2899:
	mov	w0, 0
	mov	w19, 0
	b	.L2889
	.size	MVC_FindZeroBitsInSeg, .-MVC_FindZeroBitsInSeg
	.align	3
.LC869:
	.xword	.LC868
	.align	3
.LC870:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC871:
	.xword	.LANCHOR3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FindTrailZeros
	.type	MVC_FindTrailZeros, %function
MVC_FindTrailZeros:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L2913
	ldr	x2, [x19, 240]
	ldr	w0, [x2, 76]
	cmp	w0, 1
	bls	.L2912
	ldr	w1, [x2, 48]
	ldr	x0, [x2, 40]
	bl	MVC_FindZeroBitsInSeg
	ldr	x2, [x19, 240]
	ldr	w1, [x2, 48]
	cmp	w0, w1, lsl 3
	bcs	.L2916
	cmn	w0, #1
	beq	.L2912
	add	w0, w0, 1
	str	w0, [x2, 80]
	ldr	x0, [x19, 240]
	ldr	w1, [x0, 76]
	cmp	w1, 1
	bls	.L2914
.L2917:
	ldr	w2, [x0, 48]
	mov	w20, 0
	ldr	w1, [x0, 80]
	cmp	w1, w2, lsl 3
	bcc	.L2908
	ldr	w1, [x0, 68]
	ldr	w0, [x19, 120]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 240]
	mov	w1, 1
	str	xzr, [x0, 40]
	ldr	x0, [x19, 240]
	str	w1, [x0, 76]
	ldr	x0, [x19, 240]
	str	wzr, [x0, 80]
.L2908:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2916:
	ldr	w1, [x2, 68]
	ldr	w0, [x19, 120]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 240]
	mov	w1, 1
	str	xzr, [x0, 40]
	ldr	x0, [x19, 240]
	str	w1, [x0, 76]
	ldr	x2, [x19, 240]
.L2912:
	ldr	w1, [x2, 16]
	ldr	x0, [x2, 8]
	bl	MVC_FindZeroBitsInSeg
	add	w0, w0, 1
	ldr	x2, [x19, 240]
	str	w0, [x2, 80]
	ldr	x0, [x19, 240]
	ldr	w1, [x0, 76]
	cmp	w1, 1
	bhi	.L2917
.L2914:
	mov	w20, 0
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L2913:
	mov	w20, -1
	b	.L2908
	.size	MVC_FindTrailZeros, .-MVC_FindTrailZeros
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_CombinePacket
	.type	MVC_CombinePacket, %function
MVC_CombinePacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 240]
	ldr	w0, [x0, 16]
	cmp	w0, 4095
	bhi	.L2918
	mov	x0, x19
	bl	MVC_CombinePacket.part.12
.L2918:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_CombinePacket, .-MVC_CombinePacket
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FindNaluArraySlot
	.type	MVC_FindNaluArraySlot, %function
MVC_FindNaluArraySlot:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 1521]
	cbz	w0, .L2925
	add	x0, x19, 1617
	mov	x1, 1
	b	.L2923
	.p2align 3
.L2922:
	add	w1, w1, 1
	cmp	w1, 137
	beq	.L2928
.L2923:
	ldrb	w2, [x0]
	add	x0, x0, 96
	cbnz	w2, .L2922
	mov	w0, w1
.L2921:
	lsl	x2, x1, 2
	mov	w3, 1
	sub	x1, x2, x1
	add	x1, x19, x1, lsl 5
	strb	w3, [x1, 1521]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2928:
	mov	w0, -1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L2925:
	mov	w0, 0
	mov	x1, 0
	b	.L2921
	.size	MVC_FindNaluArraySlot, .-MVC_FindNaluArraySlot
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_InquireSliceProperty
	.type	MVC_InquireSliceProperty, %function
MVC_InquireSliceProperty:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x23, x1
	mov	x21, x2
	bl	_mcount
	add	x20, x19, 11116544
	ldr	w2, [x19, 68]
	mov	w0, 22
	ldr	x1, .LC874
	sub	w2, w2, #1
	bl	dprint_vfmw
	str	wzr, [x21]
	str	wzr, [x23]
	add	x2, x19, 11120640
	add	x0, x19, 12288
	mov	w3, -1
	ldr	x1, [x19, 240]
	ldrb	w4, [x1, 2]
	strb	w4, [x20, 2811]
	ldrb	w5, [x1, 4]
	strb	w5, [x20, 2816]
	ldrsb	w1, [x1, 5]
	strb	w1, [x20, 2820]
	strb	wzr, [x20, 2819]
	str	w3, [x2, 352]
	ldrb	w3, [x0, 2384]
	cmp	w3, 1
	beq	.L2965
	cmn	w1, #1
	beq	.L2966
.L2931:
	mov	x0, x19
	bl	MVC_SliceCheck
	mov	w22, w0
	cbnz	w0, .L2947
	mov	x0, x19
	bl	MVC_ProcessSliceHeaderFirstPart
	cbnz	w0, .L2967
	ldrb	w1, [x20, 2813]
	mov	w0, 2240
	cmp	w1, 1
	cset	w1, eq
	str	w1, [x23]
	ldr	w1, [x20, 2824]
	ldrsb	w2, [x20, 2820]
	ldr	x3, [x19, 280]
	umull	x0, w1, w0
	cmn	w2, #1
	add	x0, x3, x0
	beq	.L2968
	cbnz	w2, .L2943
	ldrb	w1, [x19, 2]
	cmp	w1, 1
	beq	.L2969
	ldr	w1, [x0, 28]
	add	x0, x19, 11046912
	ldr	w2, [x19, 28]
	add	x0, x0, 180
	cmp	w2, w1
	bne	.L2970
.L2942:
	ldrb	w1, [x0, 20]
	mov	w2, 2
	ldr	w5, [x0, 3952]
	sub	w1, w2, w1
	ldr	w2, [x0, 3948]
	ldp	w3, w4, [x19, 12]
	madd	w1, w5, w1, w1
	add	w2, w2, 1
	cmp	w1, w4
	ccmp	w2, w3, 0, eq
	beq	.L2946
	mov	w1, 1
	str	w1, [x21]
.L2946:
	add	x19, x19, 11124736
	ldr	w1, [x0, 3972]
	ldr	w0, [x19, 760]
	add	w0, w0, 1
	cmp	w1, w0
	bls	.L2939
	mov	w0, 1
	str	w0, [x21]
.L2939:
	mov	w0, w22
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2965:
	strb	w3, [x20, 2819]
	ldr	w1, [x19, 14680]
	str	w1, [x2, 352]
	ldrb	w1, [x0, 2388]
	strb	w1, [x20, 2817]
	ldrb	w0, [x0, 2389]
	strb	w0, [x20, 2818]
	b	.L2931
	.p2align 3
.L2968:
	ldr	w1, [x0, 28]
	mov	w0, 3992
	ldr	x2, [x19, 272]
	umull	x0, w1, w0
	add	x0, x2, x0
	b	.L2942
	.p2align 3
.L2966:
	ldr	w1, [x19, 20]
	cbz	w1, .L2931
	add	x1, x19, 10768384
	ldr	w3, [x1, 4064]
	cbnz	w3, .L2934
	ldrb	w3, [x1, 4060]
	cbnz	w3, .L2934
	add	x0, x0, 2400
	mov	x1, 0
	.p2align 2
.L2937:
	ldr	w3, [x0]
	add	x5, x0, 335872
	cbz	w3, .L2935
	ldrb	w0, [x0, -4]
	cbnz	w0, .L2971
.L2935:
	add	w1, w1, 1
	add	x0, x5, 308
	cmp	w1, 32
	bne	.L2937
	mov	w0, -1
	b	.L2964
	.p2align 3
.L2969:
	ldr	w1, [x0, 28]
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 2
	sub	x0, x0, x1
	lsl	x0, x0, 4
	sub	x0, x0, x1
	lsl	x1, x0, 2
	sub	x1, x1, x0
	lsl	x0, x1, 6
	add	x0, x1, x0
	add	x0, x19, x0, lsl 2
	add	x0, x0, 286720
	add	x0, x0, 2612
	b	.L2942
	.p2align 3
.L2934:
	ldr	w0, [x1, 4068]
.L2964:
	cmn	w0, #1
	str	w0, [x2, 352]
	beq	.L2931
	cmp	w4, 5
	mov	w0, 1
	cset	w1, eq
	strb	w0, [x20, 2819]
	strb	w0, [x20, 2818]
	strb	w1, [x20, 2817]
	b	.L2931
	.p2align 3
.L2947:
	mov	w22, -1
	b	.L2939
	.p2align 3
.L2970:
	ldr	x1, .LC877
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2939
	.p2align 3
.L2967:
	ldr	x1, .LC876
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2939
	.p2align 3
.L2943:
	ldr	x1, .LC879
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2939
	.p2align 3
.L2971:
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 2
	sub	x0, x0, x1
	lsl	x0, x0, 4
	sub	x1, x0, x1
	lsl	x0, x1, 2
	sub	x1, x0, x1
	lsl	x0, x1, 6
	add	x0, x1, x0
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 14692]
	b	.L2964
	.size	MVC_InquireSliceProperty, .-MVC_InquireSliceProperty
	.align	3
.LC874:
	.xword	.LC873
	.align	3
.LC876:
	.xword	.LC875
	.align	3
.LC877:
	.xword	.LC88
	.align	3
.LC879:
	.xword	.LC878
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_HaveSliceToDec
	.type	MVC_HaveSliceToDec, %function
MVC_HaveSliceToDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x19, 64]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	csetm	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_HaveSliceToDec, .-MVC_HaveSliceToDec
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_IsRefListWrong
	.type	MVC_IsRefListWrong, %function
MVC_IsRefListWrong:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 11116544
	bl	_mcount
	ldr	w0, [x21, 2856]
	cbz	w0, .L2974
	mov	w19, 0
	b	.L2978
	.p2align 3
.L2975:
	ldr	x0, [x0, 640]
	cbz	x0, .L2977
	ldr	w0, [x21, 2856]
	cmp	w0, w19
	bls	.L3002
.L2978:
	add	x1, x20, x19, sxtw 3
	ldr	w0, [x20, 120]
	add	w19, w19, 1
	ldr	x1, [x1, 288]
	ldr	x1, [x1, 8]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetLogicFs
	cbnz	x0, .L2975
.L2977:
	mov	w0, -1
.L3004:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2974:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3002:
	cbz	w0, .L2974
	ldr	w0, [x21, 2860]
	mov	w19, 0
	cbnz	w0, .L2991
	b	.L2974
	.p2align 3
.L3003:
	ldr	x0, [x0, 640]
	cbz	x0, .L2977
	ldr	w0, [x21, 2860]
	cmp	w0, w19
	bls	.L2974
.L2991:
	add	x1, x20, x19, sxtw 3
	ldr	w0, [x20, 120]
	add	w19, w19, 1
	ldr	x1, [x1, 552]
	ldr	x1, [x1, 8]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetLogicFs
	cbnz	x0, .L3003
	mov	w0, -1
	b	.L3004
	.size	MVC_IsRefListWrong, .-MVC_IsRefListWrong
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DEC_Destroy
	.type	MVC_DEC_Destroy, %function
MVC_DEC_Destroy:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 12288
	add	x19, x21, 1520
	bl	_mcount
	add	x20, x20, 2384
	mov	x0, x21
	bl	MVC_ClearAllSlice
	add	x0, x21, 11120640
	mov	w1, 23352
	ldr	w2, [x21, 40]
	movk	w1, 0x1, lsl 16
	mov	w6, 2
	mov	w5, 7
	mov	w4, 32
	mov	w3, 3
	str	w2, [x0, 380]
	mov	w2, -1
	str	w1, [x0, 384]
	mov	w1, 262144
	strb	wzr, [x0, 368]
	strb	w6, [x0, 369]
	strb	w5, [x0, 371]
	strb	w4, [x0, 370]
	str	w3, [x0, 392]
	str	wzr, [x0, 400]
	str	wzr, [x0, 404]
	str	w2, [x0, 396]
	str	w1, [x0, 408]
	b	.L3007
	.p2align 3
.L3006:
	add	x19, x19, 96
	cmp	x20, x19
	beq	.L3026
.L3007:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L3006
	ldr	w0, [x21, 120]
	mov	x1, x19
	add	x19, x19, 96
	bl	MVC_ReleaseNAL
	cmp	x20, x19
	bne	.L3007
.L3026:
	ldr	w2, [x21, 60]
	mov	w19, 0
	cbz	w2, .L3012
	.p2align 2
.L3020:
	add	x20, x21, x19, uxtw 3
	add	w19, w19, 1
	ldr	x0, [x20, 832]
	mov	x1, x0
	cbz	x0, .L3010
	ldr	w0, [x21, 120]
	bl	FreeUsdByDec
	str	xzr, [x20, 832]
	ldr	w2, [x21, 60]
.L3010:
	cmp	w2, w19
	bhi	.L3020
.L3012:
	add	x20, x21, 11124736
	add	x22, x21, 11157504
	add	x20, x20, 1792
	add	x22, x22, 2624
	.p2align 2
.L3009:
	sub	x19, x20, #32
.L3014:
	ldr	x1, [x19]
	cbnz	x1, .L3027
.L3013:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L3014
	add	x20, x19, 840
	cmp	x20, x22
	bne	.L3009
	ldr	x1, .LC881
	mov	w0, 2
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3027:
	ldr	w0, [x21, 120]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L3013
	.size	MVC_DEC_Destroy, .-MVC_DEC_Destroy
	.align	3
.LC881:
	.xword	.LC880
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DEC_RecycleImage
	.type	MVC_DEC_RecycleImage, %function
MVC_DEC_RecycleImage:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w22, w1
	bl	_mcount
	ldr	x1, .LC883
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w0, [x21, 120]
	mov	w1, w22
	bl	FSP_GetLogicFs
	cbz	x0, .L3033
	ldrsb	w1, [x0, 1]
	cbz	w1, .L3033
	add	x19, x0, 568
	add	x20, x0, 600
.L3031:
	ldr	x1, [x19]
	cbnz	x1, .L3038
.L3030:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L3031
	ldr	w0, [x21, 120]
	mov	w2, 0
	mov	w1, w22
	bl	FSP_SetDisplay
	mov	w0, 0
.L3029:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3038:
	ldr	w0, [x21, 120]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L3030
	.p2align 3
.L3033:
	mov	w0, -1
	b	.L3029
	.size	MVC_DEC_RecycleImage, .-MVC_DEC_RecycleImage
	.align	3
.LC883:
	.xword	.LC882
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_OutputFrmToVO
	.type	MVC_OutputFrmToVO, %function
MVC_OutputFrmToVO:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	mov	w22, w2
	bl	_mcount
	cmp	x20, xzr
	cset	w21, eq
	cmp	x19, xzr
	cset	w0, eq
	orr	w0, w21, w0
	cbnz	w0, .L3073
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 12]
	cmp	w0, wzr
	ble	.L3042
	ldrb	w0, [x19, 8]
	cmp	w0, 2
	beq	.L3042
	ldr	x0, [x19, 816]
	cbz	x0, .L3044
	strb	wzr, [x0, 2]
	mov	w0, 1
	str	xzr, [x19, 816]
.L3072:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3042:
	mov	x1, x20
	mov	x0, x19
	bl	MVC_CheckFrameStore
	cmn	w0, #3
	beq	.L3044
	ldr	x1, [x19, 816]
	cbz	x1, .L3046
	mov	x0, x19
	bl	MVC_GetImagePara
	ldr	x1, [x19, 816]
	mov	x0, x19
	bl	MVC_CheckFrameStore
	mov	w23, w0
	cbz	w0, .L3074
	cmn	w0, #3
	beq	.L3054
	ldr	x1, .LC891
	mov	w3, w0
	mov	w2, 1910
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 816]
	mov	x0, x19
	ldr	w1, [x1, 228]
	bl	MVC_DEC_RecycleImage
	cmp	w23, 1
	cset	w1, eq
.L3053:
	ldr	x0, [x19, 816]
	tst	w21, w1
	strb	wzr, [x0, 2]
	str	xzr, [x19, 816]
	bne	.L3062
.L3046:
	mov	x1, x20
	mov	x0, x19
	bl	MVC_GetImagePara
	mov	x1, x20
	mov	x0, x19
	bl	MVC_CheckFrameStore
	cbz	w0, .L3075
	ldr	x1, .LC891
	mov	w3, w0
	mov	w2, 1959
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x20, 228]
	mov	x0, x19
	bl	MVC_DEC_RecycleImage
	ldr	x23, [sp, 48]
	mov	w0, 2
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3073:
	ldr	x1, .LC885
	mov	x3, x20
	mov	x2, x19
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, 2
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3044:
	mov	w0, 1
	b	.L3072
	.p2align 3
.L3075:
	ldrsb	w1, [x20, 6]
	mov	w2, 1
	ldr	w0, [x19, 120]
	bl	FSP_SetDisplay
	ldrsb	w1, [x20, 6]
	ldr	w0, [x19, 120]
	bl	FSP_GetFsImagePtr
	mov	x21, x0
	cbz	x0, .L3076
	cmp	w22, 1
	beq	.L3077
.L3058:
	ldr	w0, [x19, 120]
	mov	x4, x21
	add	x3, x19, 912
	mov	x2, x19
	mov	w1, 15
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L3078
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L3079
	str	wzr, [x21, 24]
.L3061:
	ldr	w1, [x19, 136]
	mov	w0, 0
	ldr	w2, [x19, 144]
	add	w1, w1, 2
	str	w1, [x19, 136]
	add	w2, w2, 1
	str	w2, [x19, 144]
	b	.L3072
	.p2align 3
.L3077:
	str	w22, [x0, 188]
	b	.L3058
.L3078:
	ldr	x1, .LC889
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	ldrsb	w1, [x20, 6]
	mov	w2, 0
	ldr	w0, [x19, 120]
	bl	FSP_SetDisplay
	mov	w1, 1
	mov	x0, x19
	bl	MVC_ClearAll
	mov	w0, -1
	b	.L3072
	.p2align 3
.L3054:
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 2]
	str	xzr, [x19, 816]
	b	.L3046
	.p2align 3
.L3074:
	ldr	x1, [x19, 816]
	mov	w2, 1
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetDisplay
	ldr	x1, [x19, 816]
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetFsImagePtr
	mov	x23, x0
	cbz	x0, .L3080
	ldr	x0, [x19, 816]
	cmp	x0, x20
	ccmp	w22, 1, 0, eq
	beq	.L3081
.L3049:
	ldr	w0, [x19, 120]
	mov	x4, x23
	add	x3, x19, 912
	mov	x2, x19
	mov	w1, 15
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L3082
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L3083
	str	wzr, [x23, 24]
.L3052:
	ldr	w0, [x19, 136]
	mov	w1, 1
	ldr	w2, [x19, 144]
	add	w0, w0, 2
	str	w0, [x19, 136]
	add	w2, w2, w1
	str	w2, [x19, 144]
	b	.L3053
	.p2align 3
.L3062:
	mov	w0, 0
	b	.L3072
	.p2align 3
.L3079:
	mov	x1, x21
	mov	x0, x19
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3061
	.p2align 3
.L3083:
	mov	x1, x23
	mov	x0, x19
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3052
.L3082:
	ldr	x1, .LC889
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x0, [x19, 816]
	mov	w2, 0
	strb	wzr, [x0, 2]
	ldr	x1, [x19, 816]
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetDisplay
	str	xzr, [x19, 816]
	mov	w1, 1
	mov	x0, x19
	bl	MVC_ClearAll
	mov	w0, -1
	b	.L3072
	.p2align 3
.L3081:
	mov	w0, 1
	str	w0, [x23, 188]
	b	.L3049
.L3076:
	ldr	x1, .LC887
	mov	w2, 1933
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3072
.L3080:
	ldr	x1, .LC887
	mov	w2, 1881
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3072
	.size	MVC_OutputFrmToVO, .-MVC_OutputFrmToVO
	.align	3
.LC885:
	.xword	.LC884
	.align	3
.LC887:
	.xword	.LC886
	.align	3
.LC889:
	.xword	.LC888
	.align	3
.LC891:
	.xword	.LC890
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_OutputFrmFromDPB
	.type	MVC_OutputFrmFromDPB, %function
MVC_OutputFrmFromDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	w21, w1
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, x21, uxtw 3
	add	x19, x19, 11124736
	bl	_mcount
	ldr	x0, [x19, 376]
	cbz	x0, .L3086
	strb	wzr, [x0, 5]
	mov	w2, 0
	mov	x0, x20
	ldr	x1, [x19, 376]
	bl	MVC_OutputFrmToVO
	cmn	w0, #1
	ldr	x1, [x19, 376]
	csetm	w19, eq
	ldrb	w0, [x1, 3]
	cbz	w0, .L3087
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3087:
	mov	w1, w21
	mov	x0, x20
	bl	MVC_RemoveFrameStoreOutDPB
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3086:
	mov	w19, 0
	ldr	x21, [sp, 32]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	MVC_OutputFrmFromDPB, .-MVC_OutputFrmFromDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FlushDPB
	.type	MVC_FlushDPB, %function
MVC_FlushDPB:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	cbz	x20, .L3107
	add	x21, x20, 11124736
	mov	w22, 0
	ldr	w0, [x21, 760]
	cbz	w0, .L3095
	.p2align 2
.L3129:
	uxtw	x0, w22
	add	w22, w22, 1
	add	x0, x0, 1388544
	add	x0, x20, x0, lsl 3
	ldr	x1, [x0, 16760]
	cbz	x1, .L3093
	ldrb	w0, [x1, 3]
	cbz	w0, .L3093
	ldr	w0, [x1, 56]
	cmp	w0, w19
	ccmn	w19, #1, 4, ne
	bne	.L3093
	mov	x0, x20
	bl	MVC_UnMarkFrameStoreRef
	.p2align 2
.L3093:
	ldr	w0, [x21, 760]
	cmp	w0, w22
	bhi	.L3129
.L3095:
	mov	w1, w19
	mov	x0, x20
	bl	MVC_RemoveUnUsedFrameStore
	ldr	w0, [x21, 760]
	cbz	w0, .L3092
	sub	w2, w0, #1
	mov	w22, -1
	add	x2, x2, 1388544
	add	x0, x21, 376
	add	x2, x2, 2096
	mov	w21, 0
	add	x2, x20, x2, lsl 3
	.p2align 2
.L3099:
	ldr	x1, [x0]
	cmn	w19, #1
	add	x0, x0, 8
	cbz	x1, .L3097
	ldr	w1, [x1, 56]
	ccmp	w19, w1, 4, ne
	csinc	w21, w21, w21, ne
	cmp	w19, w1
	ccmn	w19, #1, 4, ne
	csel	w22, w22, w1, eq
.L3097:
	cmp	x2, x0
	bne	.L3099
	cbz	w21, .L3092
	cmn	w22, #1
	cset	w24, ne
	cmp	w19, w22
	cset	w23, gt
	and	w23, w24, w23
	b	.L3105
	.p2align 3
.L3106:
	subs	w21, w21, #1
	beq	.L3092
.L3105:
	add	x3, x29, 72
	add	x2, x29, 76
	mov	w1, w22
	mov	x0, x20
	cbz	w23, .L3102
	bl	MVC_GetMinPOC
	ldr	w1, [x29, 72]
	mov	x0, x20
	cmn	w1, #1
	beq	.L3092
	bl	MVC_OutputFrmFromDPB
	cbnz	w0, .L3107
.L3102:
	mov	w1, w19
	add	x3, x29, 72
	add	x2, x29, 76
	mov	x0, x20
	bl	MVC_GetMinPOC
	ldr	w1, [x29, 72]
	mov	x0, x20
	cmn	w1, #1
	beq	.L3092
	bl	MVC_OutputFrmFromDPB
	cbnz	w0, .L3107
	cmp	w19, w22
	cset	w0, lt
	tst	w24, w0
	beq	.L3106
	mov	w1, w22
	add	x3, x29, 72
	add	x2, x29, 76
	mov	x0, x20
	bl	MVC_GetMinPOC
	ldr	w1, [x29, 72]
	mov	x0, x20
	cmn	w1, #1
	beq	.L3092
	bl	MVC_OutputFrmFromDPB
	cbz	w0, .L3106
.L3107:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, -1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3092:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.size	MVC_FlushDPB, .-MVC_FlushDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_AdaptiveMemMark
	.type	MVC_AdaptiveMemMark, %function
MVC_AdaptiveMemMark:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x24, x20, 11157504
	add	x25, x20, 11116544
	bl	_mcount
	ldr	x22, .LC892
	add	x23, x24, 1960
	mov	w26, 1
	ldr	x21, [x24, 2608]
	add	x19, x21, 8
	add	x21, x21, 2008
	.p2align 2
.L3153:
	ldr	w0, [x19, -4]
	cmp	w0, 6
	bhi	.L3144
	ldrb	w0, [x22,w0,uxtw]
	adr	x1, .Lrtx3146
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx3146:
	.section	.rodata
	.align	0
	.align	2
.L3146:
	.byte	(.L3145 - .Lrtx3146) / 4
	.byte	(.L3147 - .Lrtx3146) / 4
	.byte	(.L3148 - .Lrtx3146) / 4
	.byte	(.L3149 - .Lrtx3146) / 4
	.byte	(.L3150 - .Lrtx3146) / 4
	.byte	(.L3151 - .Lrtx3146) / 4
	.byte	(.L3152 - .Lrtx3146) / 4
	.text
	.p2align 3
.L3151:
	mov	x0, x20
	bl	MVC_UnMarkAllSTRef
	mov	x0, x20
	bl	MVC_UpdateReflist
	mov	w1, 0
	mov	x0, x20
	bl	MVC_UpdateMaxLTFrmIdx
	mov	x0, x20
	bl	MVC_UpdateLTReflist
	strb	w26, [x25, 1672]
	.p2align 2
.L3144:
	add	x19, x19, 20
	cmp	x19, x21
	bne	.L3153
	.p2align 2
.L3145:
	ldrsb	w1, [x25, 1672]
	mov	w0, 0
	cmp	w1, 1
	beq	.L3161
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3150:
	ldr	w1, [x19, 12]
	mov	x0, x20
	bl	MVC_UpdateMaxLTFrmIdx
	mov	x0, x20
	bl	MVC_UpdateLTReflist
	b	.L3144
	.p2align 3
.L3149:
	ldr	w2, [x19]
	mov	x1, x23
	ldr	w3, [x19, 8]
	mov	x0, x20
	bl	MVC_MarkSTToLTRef
	mov	x0, x20
	bl	MVC_UpdateReflist
	mov	x0, x20
	bl	MVC_UpdateLTReflist
	b	.L3144
	.p2align 3
.L3148:
	ldr	w2, [x19, 4]
	mov	x0, x20
	mov	x1, x23
	bl	MVC_UnMarkLTRef
	mov	x0, x20
	bl	MVC_UpdateLTReflist
	b	.L3144
	.p2align 3
.L3147:
	ldr	w2, [x19]
	mov	x0, x20
	mov	x1, x23
	bl	MVC_UnMarkSTRef
	mov	x0, x20
	bl	MVC_UpdateReflist
	b	.L3144
	.p2align 3
.L3152:
	ldr	w2, [x19, 8]
	mov	x1, x23
	mov	x0, x20
	bl	MVC_MarkCurrPicLT
	b	.L3144
	.p2align 3
.L3161:
	add	x0, x20, 11120640
	str	wzr, [x24, 2616]
	ldr	w0, [x0, 356]
	add	x0, x0, 2777088
	add	x0, x20, x0, lsl 2
	str	wzr, [x0, 11020]
	str	wzr, [x24, 3440]
	ldrb	w0, [x24, 1963]
	cmp	w0, 1
	beq	.L3156
	cbz	w0, .L3157
	cmp	w0, 2
	bne	.L3155
	str	wzr, [x24, 2628]
	str	wzr, [x24, 2640]
	str	wzr, [x25, 1700]
	str	wzr, [x25, 1708]
.L3155:
	ldr	w1, [x24, 2688]
	mov	x0, x20
	bl	MVC_FlushDPB
	cmp	w0, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L3157:
	ldr	w2, [x24, 2628]
	ldr	w0, [x24, 2636]
	ldr	w1, [x24, 2640]
	sub	w0, w0, w2
	str	w0, [x24, 2636]
	sub	w1, w1, w2
	str	w1, [x24, 2640]
	cmp	w0, w1
	csel	w0, w0, w1, le
	str	w0, [x24, 2628]
	str	w0, [x24, 2632]
	ldr	w2, [x25, 1708]
	ldr	w0, [x25, 1696]
	ldr	w1, [x25, 1700]
	sub	w0, w0, w2
	str	w0, [x25, 1696]
	sub	w1, w1, w2
	str	w1, [x25, 1700]
	cmp	w0, w1
	csel	w0, w0, w1, le
	str	w0, [x25, 1704]
	str	w0, [x25, 1708]
	b	.L3155
.L3156:
	str	wzr, [x24, 2628]
	str	wzr, [x24, 2636]
	str	wzr, [x25, 1696]
	str	wzr, [x25, 1708]
	b	.L3155
	.size	MVC_AdaptiveMemMark, .-MVC_AdaptiveMemMark
	.align	3
.LC892:
	.xword	.L3146
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_IDRMemMarking
	.type	MVC_IDRMemMarking, %function
MVC_IDRMemMarking:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x20, x22, 11157504
	bl	_mcount
	ldr	x0, [x20, 2608]
	ldrb	w0, [x0, 1]
	cbz	w0, .L3163
	ldr	w0, [x22, 52]
	cbz	w0, .L3190
	sub	w4, w0, #1
	add	x24, x22, 11124736
	add	x4, x4, 38
	add	x2, x24, 376
	add	x1, x22, 148
	add	x4, x22, x4, lsl 2
	b	.L3171
	.p2align 3
.L3169:
	ldr	w3, [x0, 56]
	ldr	w0, [x20, 2688]
	cmp	w3, w0
	beq	.L3191
.L3168:
	add	x1, x1, 4
	cmp	x1, x4
	beq	.L3172
.L3171:
	ldr	x0, [x2]
	add	x2, x2, 8
	cbz	x0, .L3168
	ldr	w3, [x0, 16]
	cmp	w3, 1
	bne	.L3169
	strb	w3, [x22, 6]
	ldr	w3, [x0, 56]
	ldr	w0, [x20, 2688]
	cmp	w3, w0
	bne	.L3168
.L3191:
	str	wzr, [x1]
	add	x1, x1, 4
	cmp	x1, x4
	bne	.L3171
.L3172:
	ldr	w0, [x24, 760]
	cbz	w0, .L3166
	sub	w21, w0, #1
	add	x19, x24, 376
	add	x21, x21, 1388544
	mov	w23, 1
	add	x21, x21, 2096
	add	x21, x22, x21, lsl 3
	b	.L3177
	.p2align 3
.L3175:
	add	x19, x19, 8
	cmp	x19, x21
	beq	.L3166
.L3177:
	ldr	x0, [x19]
	cbz	x0, .L3175
	ldr	w2, [x0, 56]
	ldr	w1, [x20, 2688]
	cmp	w2, w1
	bne	.L3175
	strb	wzr, [x0, 698]
	mov	w2, 0
	strb	wzr, [x0, 697]
	ldr	x0, [x19]
	strb	wzr, [x0, 746]
	strb	wzr, [x0, 745]
	ldr	x0, [x19]
	strb	wzr, [x0, 794]
	strb	wzr, [x0, 793]
	ldr	x0, [x19]
	strb	wzr, [x0, 3]
	ldr	x1, [x19]
	ldr	w0, [x22, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetRef
	ldr	x0, [x19]
	strb	wzr, [x0, 2]
	ldr	x0, [x19]
	strb	wzr, [x0, 5]
	ldr	x0, [x19]
	strb	w23, [x0, 7]
	ldr	x1, [x19]
	ldr	w0, [x1, 52]
	add	x0, x22, x0
	add	x0, x0, 11124736
	strb	wzr, [x0, 784]
	ldrsb	w1, [x1, 6]
	ldr	w0, [x22, 120]
	bl	FSP_GetDisplay
	cmp	w0, 3
	mov	w2, 0
	beq	.L3176
	ldr	x1, [x19]
	ldr	w0, [x22, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetDisplay
.L3176:
	str	xzr, [x19]
	add	x19, x19, 8
	str	xzr, [x19, 120]
	str	xzr, [x19, 248]
	cmp	x19, x21
	ldr	w0, [x24, 764]
	sub	w0, w0, #1
	str	w0, [x24, 764]
	bne	.L3177
	.p2align 2
.L3166:
	mov	x0, x22
	bl	MVC_UpdateReflist
	mov	x0, x22
	bl	MVC_UpdateLTReflist
	ldr	x0, [x20, 2608]
	ldrb	w0, [x0, 2]
	cbnz	w0, .L3192
	mov	w0, 1
	str	wzr, [x24, 776]
	strb	w0, [x20, 1965]
	mov	w0, 0
	strb	wzr, [x20, 1964]
.L3178:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3192:
	mov	w0, 1
	str	w0, [x24, 776]
	strb	w0, [x20, 1964]
	mov	w0, 0
	strb	wzr, [x20, 1965]
	str	wzr, [x20, 2620]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3163:
	ldr	w1, [x20, 2688]
	mov	x0, x22
	bl	MVC_FlushDPB
	cbnz	w0, .L3179
	add	x24, x22, 11124736
	b	.L3166
.L3190:
	add	x24, x22, 11124736
	b	.L3172
.L3179:
	mov	w0, -1
	b	.L3178
	.size	MVC_IDRMemMarking, .-MVC_IDRMemMarking
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_Marking
	.type	MVC_Marking, %function
MVC_Marking:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 11157504
	bl	_mcount
	add	x0, x20, 11116544
	strb	wzr, [x0, 1672]
	ldrb	w1, [x19, 1963]
	cmp	w1, 2
	cset	w1, eq
	strb	w1, [x0, 1673]
	ldrb	w0, [x19, 1966]
	cmp	w0, 5
	beq	.L3194
	ldrb	w0, [x19, 1972]
	cbnz	w0, .L3195
	ldr	x0, [x20, 224]
	ldr	w1, [x0, 640]
	cmp	w1, 1
	beq	.L3219
.L3195:
	ldr	x0, [x19, 2608]
	ldrb	w1, [x0, 3]
	cbz	w1, .L3220
	ldr	w0, [x19, 2624]
	cbnz	w0, .L3221
.L3202:
	mov	w0, 0
.L3217:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3220:
	ldr	w1, [x19, 2624]
	cbz	w1, .L3202
.L3205:
	ldrb	w1, [x19, 1961]
	cbnz	w1, .L3206
	add	x1, x20, 11124736
	ldr	x2, [x20, 248]
	ldr	w4, [x1, 772]
	ldr	w2, [x2, 3944]
	ldr	w3, [x1, 768]
	sub	w1, w2, w4
	cmp	w3, w1
	beq	.L3222
.L3207:
	strb	wzr, [x0]
	strb	wzr, [x19, 1964]
.L3198:
	ldr	w0, [x19, 2624]
	cbz	w0, .L3202
.L3206:
	ldrb	w1, [x19, 1964]
	mov	w0, 0
	cmp	w1, wzr
	cset	w1, eq
	strb	w1, [x19, 1965]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3221:
	mov	x0, x20
	bl	MVC_AdaptiveMemMark
	ldr	x1, [x19, 2608]
	strb	wzr, [x1]
	cbnz	w0, .L3196
.L3203:
	ldrb	w0, [x19, 1966]
	cmp	w0, 5
	beq	.L3198
	ldr	w0, [x19, 2624]
	cbz	w0, .L3202
	ldr	x0, [x19, 2608]
	ldrb	w1, [x0, 3]
	cbnz	w1, .L3206
	b	.L3205
	.p2align 3
.L3219:
	ldr	w1, [x0, 636]
	cbnz	w1, .L3195
	ldr	w1, [x0, 644]
	cbnz	w1, .L3195
	ldr	w0, [x0, 604]
	add	w0, w0, 1024
	cmp	w0, 2048
	bls	.L3195
	.p2align 2
.L3194:
	mov	x0, x20
	bl	MVC_IDRMemMarking
	cbz	w0, .L3203
.L3196:
	mov	w0, -1
	b	.L3217
	.p2align 3
.L3222:
	mov	x0, x20
	bl	MVC_SlidingWinMark.part.2
	ldr	x0, [x19, 2608]
	b	.L3207
	.size	MVC_Marking, .-MVC_Marking
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DirectOutput
	.type	MVC_DirectOutput, %function
MVC_DirectOutput:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 11157504
	bl	_mcount
	ldrb	w0, [x20, 1963]
	cmp	w0, 1
	beq	.L3225
	cbz	w0, .L3226
	cmp	w0, 2
	beq	.L3227
	mov	w21, 0
.L3224:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3227:
	ldr	x0, [x20, 2600]
	mov	w1, 3
	strb	w1, [x0, 696]
	ldrb	w0, [x20, 1961]
	cbnz	w0, .L3242
	ldr	x1, [x19, 816]
	cbz	x1, .L3243
	mov	w2, 0
	mov	x0, x19
	bl	MVC_OutputFrmToVO
	mov	w21, w0
	ldr	x0, [x20, 2600]
	str	x0, [x19, 816]
	str	x0, [x0, 800]
.L3244:
	ldr	x0, [x19, 816]
	mov	w22, 2
	ldrb	w3, [x20, 1962]
	mov	x2, 624
	add	x1, x20, 1976
	strb	w3, [x0, 1]
	ldr	x0, [x19, 816]
	strb	w22, [x0, 2]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 5]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 7]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 3]
	ldr	x0, [x19, 816]
	ldrb	w3, [x20, 1972]
	strb	w3, [x0]
	ldr	x0, [x19, 816]
	str	wzr, [x0, 40]
	ldr	x0, [x19, 816]
	add	x0, x0, 72
	bl	memcpy
	ldr	x1, [x19, 816]
	mov	w0, 2
	ldr	w2, [x20, 2660]
	str	w2, [x1, 824]
	ldr	x1, [x19, 816]
	ldr	w2, [x1, 824]
	str	w2, [x1, 44]
	str	w2, [x1, 728]
	ldr	x1, [x19, 816]
	strb	w22, [x1, 792]
	ldrb	w2, [x20, 1967]
	ldr	x1, [x19, 816]
	cmp	w2, 1
	csel	w0, wzr, w0, ne
	strb	w0, [x1, 4]
.L3299:
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2628]
	str	w1, [x0, 32]
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2688]
	str	w1, [x0, 56]
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2692]
	str	w1, [x0, 60]
	ldrb	w22, [x19, 9]
	cmp	w22, 1
	bne	.L3224
	ldr	x1, [x19, 816]
	mov	w2, 0
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetRef
	ldr	x1, [x19, 816]
	mov	w2, 0
	mov	x0, x19
	bl	MVC_OutputFrmToVO
	mov	w21, w0
	ldr	x0, [x19, 816]
	cbz	x0, .L3252
	ldr	x1, [x20, 2600]
	mov	w2, w22
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_ClearLogicFs
.L3252:
	str	xzr, [x19, 816]
.L3302:
	mov	w0, w21
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3226:
	ldr	x0, [x20, 2600]
	strb	wzr, [x0, 696]
	ldr	x1, [x19, 816]
	cbz	x1, .L3228
	mov	w2, 0
	mov	x0, x19
	bl	MVC_OutputFrmToVO
	cbnz	w0, .L3301
.L3229:
	ldr	x0, [x19, 816]
	cbz	x0, .L3228
	ldr	x1, [x20, 2600]
	mov	w2, 1
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_ClearLogicFs
.L3228:
	ldr	x0, [x20, 2600]
	mov	w3, 3
	str	x0, [x19, 816]
	mov	x2, 624
	add	x1, x20, 1976
	str	x0, [x0, 704]
	ldr	x0, [x19, 816]
	ldr	x4, [x20, 2600]
	str	x4, [x0, 752]
	ldr	x0, [x19, 816]
	ldr	x4, [x20, 2600]
	str	x4, [x0, 800]
	ldr	x0, [x19, 816]
	ldrb	w4, [x20, 1962]
	strb	w4, [x0, 1]
	ldr	x0, [x19, 816]
	strb	w3, [x0, 2]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 3]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 5]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 7]
	ldr	x0, [x19, 816]
	add	x0, x0, 72
	bl	memcpy
	ldr	x1, [x19, 816]
	mov	w0, 3
	ldr	w3, [x20, 2660]
	mov	w2, 0
	str	w3, [x1, 44]
	str	w3, [x1, 728]
	ldr	x1, [x19, 816]
	strb	wzr, [x1, 696]
	ldr	x1, [x19, 816]
	ldrb	w3, [x20, 1972]
	strb	w3, [x1]
	ldr	x1, [x19, 816]
	ldrb	w3, [x20, 1969]
	str	w3, [x1, 40]
	ldrb	w3, [x20, 1967]
	ldr	x1, [x19, 816]
	cmp	w3, 1
	csel	w0, wzr, w0, ne
	strb	w0, [x1, 4]
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2628]
	str	w1, [x0, 32]
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2688]
	str	w1, [x0, 56]
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2692]
	str	w1, [x0, 60]
.L3297:
	ldr	x1, [x19, 816]
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetRef
	ldr	x1, [x19, 816]
	mov	w2, 0
	mov	x0, x19
	bl	MVC_OutputFrmToVO
	mov	w21, w0
	ldr	x0, [x19, 816]
	cbz	x0, .L3252
	ldr	x1, [x20, 2600]
	mov	w2, 1
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_ClearLogicFs
	str	xzr, [x19, 816]
	b	.L3302
	.p2align 3
.L3225:
	ldr	x0, [x20, 2600]
	mov	w1, 3
	strb	w1, [x0, 696]
	ldrb	w0, [x20, 1961]
	cbnz	w0, .L3232
	ldr	x1, [x19, 816]
	cbz	x1, .L3233
	mov	w2, 0
	mov	x0, x19
	bl	MVC_OutputFrmToVO
	mov	w21, w0
	ldr	x0, [x20, 2600]
	str	x0, [x19, 816]
	str	x0, [x0, 752]
.L3234:
	ldr	x0, [x19, 816]
	mov	w22, 1
	ldrb	w3, [x20, 1962]
	add	x1, x20, 1976
	mov	x2, 624
	strb	w3, [x0, 1]
	ldr	x0, [x19, 816]
	strb	w22, [x0, 2]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 5]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 7]
	ldr	x0, [x19, 816]
	strb	wzr, [x0, 3]
	ldr	x0, [x19, 816]
	ldrb	w3, [x20, 1972]
	strb	w3, [x0]
	ldr	x0, [x19, 816]
	str	wzr, [x0, 40]
	ldr	x0, [x19, 816]
	add	x0, x0, 72
	bl	memcpy
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2660]
	str	w1, [x0, 776]
	ldr	x0, [x19, 816]
	ldr	w1, [x0, 776]
	str	w1, [x0, 44]
	str	w1, [x0, 728]
	ldr	x0, [x19, 816]
	strb	w22, [x0, 744]
	ldrb	w1, [x20, 1967]
	ldr	x0, [x19, 816]
	cmp	w1, 1
	cset	w1, eq
	strb	w1, [x0, 4]
	b	.L3299
	.p2align 3
.L3242:
	ldr	x3, [x19, 816]
	cbz	x3, .L3303
.L3247:
	mov	x2, 624
	add	x1, x20, 1976
	add	x0, x3, 72
	ldr	x23, [x3, 672]
	ldr	x22, [x3, 680]
	ldr	x21, [x3, 688]
	bl	memcpy
	ldr	x0, [x20, 2576]
	cmn	x0, #1
	ccmn	x23, #1, 4, eq
	beq	.L3248
	ldr	x0, [x19, 816]
	str	x23, [x0, 672]
.L3248:
	ldr	x0, [x20, 2584]
	ldr	x1, [x19, 816]
	cmp	x0, x22
	csel	x0, x0, x22, ls
	str	x0, [x1, 680]
	ldr	x0, [x20, 2592]
	ldr	x1, [x19, 816]
	cmp	x0, x21
	csel	x0, x0, x21, ls
	str	x0, [x1, 688]
	ldr	x1, [x19, 816]
	ldrb	w0, [x1, 1]
	cbz	w0, .L3249
	ldrb	w0, [x20, 1962]
.L3249:
	strb	w0, [x1, 1]
	mov	w4, 2
	mov	w3, 3
	ldr	x1, [x19, 816]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 2
	strb	w0, [x1, 2]
	ldr	x1, [x19, 816]
	ldrb	w0, [x20, 1972]
	ldrb	w2, [x1]
	cmp	w0, w2, uxtb
	csel	w0, w0, w2, cs
	strb	w0, [x1]
	ldr	x0, [x19, 816]
	strb	w4, [x0, 792]
	ldr	x0, [x19, 816]
	strb	w3, [x0, 696]
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2660]
	str	w1, [x0, 824]
	ldr	x1, [x19, 816]
	ldr	w2, [x1, 776]
	ldr	w0, [x1, 824]
	add	w0, w2, w0
	lsr	w0, w0, 1
	str	w0, [x1, 728]
	ldr	x0, [x19, 816]
	ldr	w1, [x0, 728]
	str	w1, [x0, 44]
	ldr	x0, [x19, 816]
	ldr	x1, [x0, 800]
	str	x1, [x0, 704]
	ldrb	w0, [x20, 1967]
	ldr	x1, [x19, 816]
	cmp	w0, 1
	beq	.L3304
.L3250:
	ldrb	w0, [x1, 4]
.L3251:
	strb	w0, [x1, 4]
	mov	w2, 0
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2628]
	str	w1, [x0, 32]
	b	.L3297
	.p2align 3
.L3232:
	ldr	x3, [x19, 816]
	cbz	x3, .L3305
.L3236:
	mov	x2, 624
	add	x1, x20, 1976
	add	x0, x3, 72
	ldr	x23, [x3, 672]
	ldr	x22, [x3, 680]
	ldr	x21, [x3, 688]
	bl	memcpy
	ldr	x0, [x20, 2576]
	cmn	x0, #1
	ccmn	x23, #1, 4, eq
	beq	.L3237
	ldr	x0, [x19, 816]
	str	x23, [x0, 672]
.L3237:
	ldr	x0, [x20, 2584]
	ldr	x1, [x19, 816]
	cmp	x0, x22
	csel	x0, x0, x22, ls
	str	x0, [x1, 680]
	ldr	x0, [x20, 2592]
	ldr	x1, [x19, 816]
	cmp	x0, x21
	csel	x0, x0, x21, ls
	str	x0, [x1, 688]
	ldr	x1, [x19, 816]
	ldrb	w0, [x1, 1]
	cbz	w0, .L3238
	ldrb	w0, [x20, 1962]
.L3238:
	strb	w0, [x1, 1]
	mov	w4, 1
	mov	w3, 3
	ldr	x1, [x19, 816]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 1
	strb	w0, [x1, 2]
	ldr	x1, [x19, 816]
	ldrb	w0, [x20, 1972]
	ldrb	w2, [x1]
	cmp	w0, w2, uxtb
	csel	w0, w0, w2, cs
	strb	w0, [x1]
	ldr	x0, [x19, 816]
	strb	w4, [x0, 744]
	ldr	x0, [x19, 816]
	strb	w3, [x0, 696]
	ldr	x0, [x19, 816]
	ldr	w1, [x20, 2660]
	str	w1, [x0, 776]
	ldr	x1, [x19, 816]
	ldr	w2, [x1, 776]
	ldr	w0, [x1, 824]
	add	w0, w2, w0
	lsr	w0, w0, w4
	str	w0, [x1, 728]
	ldr	x0, [x19, 816]
	ldr	w1, [x0, 728]
	str	w1, [x0, 44]
	ldr	x0, [x19, 816]
	ldr	x1, [x0, 752]
	str	x1, [x0, 704]
	ldrb	w0, [x20, 1967]
	ldr	x1, [x19, 816]
	cmp	w0, 1
	bne	.L3250
	ldrb	w0, [x1, 4]
	orr	w0, w0, 1
	uxtb	w0, w0
	b	.L3251
	.p2align 3
.L3301:
	ldr	x2, .LC893
	mov	w4, w0
	ldr	x1, .LC895
	mov	w3, 2510
	add	x2, x2, 256
	mov	w0, 22
	bl	dprint_vfmw
	b	.L3229
	.p2align 3
.L3304:
	ldrb	w0, [x1, 4]
	orr	w0, w0, 2
	uxtb	w0, w0
	b	.L3251
	.p2align 3
.L3233:
	ldr	x0, [x20, 2600]
	mov	w21, 0
	str	x0, [x19, 816]
	str	x0, [x0, 752]
	b	.L3234
	.p2align 3
.L3243:
	ldr	x0, [x20, 2600]
	mov	w21, 0
	str	x0, [x19, 816]
	str	x0, [x0, 800]
	b	.L3244
	.p2align 3
.L3303:
	ldr	x0, [x20, 2600]
	str	x0, [x19, 816]
	str	x0, [x0, 800]
	ldr	x3, [x19, 816]
	b	.L3247
	.p2align 3
.L3305:
	ldr	x0, [x20, 2600]
	str	x0, [x19, 816]
	str	x0, [x0, 752]
	ldr	x3, [x19, 816]
	b	.L3236
	.size	MVC_DirectOutput, .-MVC_DirectOutput
	.align	3
.LC893:
	.xword	.LANCHOR0
	.align	3
.LC895:
	.xword	.LC894
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DEC_GetRemainImg
	.type	MVC_DEC_GetRemainImg, %function
MVC_DEC_GetRemainImg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	add	x19, x22, 11124736
	bl	_mcount
	ldr	w0, [x19, 760]
	cbz	w0, .L3320
	mov	w20, 0
	mov	w21, 0
	b	.L3309
	.p2align 3
.L3308:
	ldr	w0, [x19, 760]
	cmp	w0, w21
	bls	.L3347
.L3309:
	uxtw	x0, w21
	add	w21, w21, 1
	add	x0, x0, 1388544
	add	x0, x22, x0, lsl 3
	ldr	x0, [x0, 16760]
	cbz	x0, .L3308
	ldrb	w1, [x0, 5]
	cbz	w1, .L3308
	ldrsb	w1, [x0, 6]
	ldr	w0, [x22, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L3308
	ldrb	w0, [x0, 1]
	sub	w0, w0, #1
	uxtb	w0, w0
	cmp	w0, 1
	ldr	w0, [x19, 760]
	csinc	w20, w20, w20, hi
	cmp	w0, w21
	bhi	.L3309
.L3347:
	mov	w21, -1
	cbnz	w20, .L3313
	b	.L3310
	.p2align 3
.L3311:
	add	x1, x22, x1, lsl 3
	ldr	x1, [x1, 16760]
	bl	MVC_OutputFrmToVO
	mov	w21, w0
	subs	w20, w20, #1
	beq	.L3345
.L3313:
	add	x2, x29, 60
	mov	w1, -1
	add	x3, x29, 56
	mov	x0, x22
	bl	MVC_GetMinPOC
	ldr	w1, [x29, 56]
	mov	w2, w20
	mov	x0, x22
	cmn	w1, #1
	uxtw	x1, w1
	add	x1, x1, 1388544
	bne	.L3311
.L3345:
	ldr	w0, [x19, 760]
	cbz	w0, .L3307
.L3312:
	mov	w20, 0
	b	.L3316
	.p2align 3
.L3315:
	ldr	w0, [x19, 760]
	cmp	w0, w20
	bls	.L3307
.L3316:
	uxtw	x0, w20
	add	w20, w20, 1
	add	x0, x0, 1388544
	add	x0, x22, x0, lsl 3
	ldr	x1, [x0, 16760]
	cbz	x1, .L3315
	ldrb	w0, [x1, 3]
	cbz	w0, .L3315
	mov	x0, x22
	bl	MVC_UnMarkFrameStoreRef
	ldr	w0, [x19, 760]
	cmp	w0, w20
	bhi	.L3316
.L3307:
	mov	w1, -1
	mov	x0, x22
	bl	MVC_RemoveUnUsedFrameStore
	cbnz	w21, .L3318
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L3310:
	cbnz	w0, .L3312
	mov	w1, w21
	mov	x0, x22
	bl	MVC_RemoveUnUsedFrameStore
.L3318:
	add	x0, x22, 912
	bl	GetVoLastImageID
	mov	w21, w0
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L3320:
	mov	w21, -1
	b	.L3307
	.size	MVC_DEC_GetRemainImg, .-MVC_DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	MVC_InitDPB.part.15, %function
MVC_InitDPB.part.15:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x22, x20, 11124736
	add	x19, x22, 376
	add	x21, x22, 504
	bl	_mcount
	.p2align 2
.L3352:
	ldr	x0, [x19]
	mov	w2, 1
	cbz	x0, .L3349
	ldrsb	w1, [x0, 6]
	ldr	w0, [x20, 120]
	bl	FSP_ClearLogicFs
.L3349:
	ldr	x0, [x19, 128]
	mov	w2, 1
	cbz	x0, .L3350
	ldrsb	w1, [x0, 6]
	ldr	w0, [x20, 120]
	bl	FSP_ClearLogicFs
.L3350:
	ldr	x0, [x19, 256]
	mov	w2, 1
	cbz	x0, .L3351
	ldrsb	w1, [x0, 6]
	ldr	w0, [x20, 120]
	bl	FSP_ClearLogicFs
.L3351:
	str	xzr, [x19, 128]
	str	xzr, [x19, 256]
	str	xzr, [x19], 8
	cmp	x19, x21
	bne	.L3352
	add	x1, x22, 768
	ldr	w0, [x20, 44]
	stp	w0, wzr, [x1, -8]
	stp	wzr, wzr, [x1]
	str	wzr, [x22, 776]
	ldr	x1, [x20, 816]
	cbz	x1, .L3354
	ldrb	w0, [x20, 8]
	cmp	w0, 2
	beq	.L3355
.L3359:
	add	x0, x20, 11157504
	ldr	x0, [x0, 2600]
	cbz	x0, .L3357
	ldrsb	w1, [x0, 6]
	mov	w2, 1
	ldr	w0, [x20, 120]
	bl	FSP_ClearLogicFs
.L3357:
	str	xzr, [x20, 816]
.L3354:
	ldr	x3, .LC899
	mov	w1, 0
	mov	x2, 344
	add	x0, x22, 784
	ldr	x3, [x3, 96]
	blr	x3
	str	wzr, [x22, 996]
	ldr	w0, [x22, 760]
	add	x1, x20, 220
	str	w0, [x22, 992]
	add	x0, x20, 148
	.p2align 2
.L3361:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L3361
	add	x1, x20, 288
	add	x0, x20, 552
	.p2align 2
.L3362:
	str	xzr, [x1], 8
	cmp	x1, x0
	bne	.L3362
	add	x1, x20, 11116544
	add	x20, x20, 816
	str	wzr, [x1, 2856]
	.p2align 2
.L3363:
	str	xzr, [x0], 8
	cmp	x20, x0
	bne	.L3363
	str	wzr, [x1, 2860]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L3355:
	mov	w2, 0
	mov	x0, x20
	bl	MVC_OutputFrmToVO
	cbnz	w0, .L3380
.L3358:
	ldr	x0, [x20, 816]
	cbnz	x0, .L3359
	b	.L3354
	.p2align 3
.L3380:
	ldr	x2, .LC896
	mov	w4, w0
	ldr	x1, .LC898
	mov	w3, 5922
	add	x2, x2, 280
	mov	w0, 22
	bl	dprint_vfmw
	b	.L3358
	.size	MVC_InitDPB.part.15, .-MVC_InitDPB.part.15
	.align	3
.LC896:
	.xword	.LANCHOR0
	.align	3
.LC898:
	.xword	.LC897
	.align	3
.LC899:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_InitDPB
	.type	MVC_InitDPB, %function
MVC_InitDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L3383
	mov	x0, x19
	bl	MVC_ClearAllSlice
	mov	x0, x19
	bl	MVC_InitDPB.part.15
.L3382:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3383:
	mov	w0, -1
	b	.L3382
	.size	MVC_InitDPB, .-MVC_InitDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ClearDPB
	.type	MVC_ClearDPB, %function
MVC_ClearDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	mov	w1, w20
	mov	x0, x19
	bl	MVC_FlushDPB
	cbnz	w0, .L3392
	cbz	x19, .L3388
.L3386:
	mov	x0, x19
	bl	MVC_ClearAllSlice
	mov	x0, x19
	bl	MVC_InitDPB.part.15
	cbnz	w0, .L3388
	strb	wzr, [x19, 6]
	mov	w0, 0
	str	wzr, [x19, 220]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3392:
	ldr	x2, .LC900
	mov	w3, 314
	ldr	x1, .LC902
	mov	w0, 22
	add	x2, x2, 296
	bl	dprint_vfmw
	cbnz	x19, .L3386
.L3388:
	ldr	x2, .LC900
	mov	w3, 319
	ldr	x1, .LC904
	mov	w0, 22
	add	x2, x2, 296
	bl	dprint_vfmw
	strb	wzr, [x19, 6]
	str	wzr, [x19, 220]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_ClearDPB, .-MVC_ClearDPB
	.align	3
.LC900:
	.xword	.LANCHOR0
	.align	3
.LC902:
	.xword	.LC901
	.align	3
.LC904:
	.xword	.LC903
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecVDM
	.type	MVC_DecVDM, %function
MVC_DecVDM:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 11157504
	bl	_mcount
	ldrb	w0, [x21, 1963]
	cbz	w0, .L3394
	ldrb	w0, [x21, 1961]
	cmp	w0, 1
	beq	.L3394
.L3395:
	ldr	w20, [x19, 108]
	ldrb	w0, [x19, 8]
	ubfx	x20, x20, 16, 2
	cmp	w20, w0
	beq	.L3396
	sub	w0, w0, #2
	uxtb	w0, w0
	cmp	w0, 1
	ccmp	w20, 1, 2, ls
	bls	.L3461
.L3396:
	uxtb	w20, w20
	strb	w20, [x19, 8]
	cmp	w20, 2
	beq	.L3399
	cmp	w20, 3
	beq	.L3400
	cmp	w20, 1
	beq	.L3462
	ldrb	w0, [x19, 11]
	cmp	w0, 1
	beq	.L3463
.L3407:
	ldr	w0, [x19, 64]
.L3419:
	ldrb	w1, [x21, 1973]
	strb	w1, [x19, 11]
.L3408:
	cbz	w0, .L3459
	ldrb	w0, [x21, 1972]
	cmp	w0, 2
	beq	.L3464
	cmp	w0, 1
	beq	.L3465
.L3411:
	ldr	w2, [x19, 88]
	mov	w0, 2
	ldr	x1, .LC910
	add	x20, x19, 11161600
	ldrb	w3, [x21, 1963]
	sub	w2, w2, #1
	bl	dprint_vfmw
	ldr	w0, [x21, 2676]
	str	w0, [x20, 1260]
	mov	w0, 1
	strb	w0, [x20, 458]
	mov	w0, 0
	ldr	w1, [x21, 2680]
	str	w1, [x20, 1256]
	ldr	w1, [x19, 64]
	str	w1, [x20, 1460]
	ldr	w1, [x19, 88]
	cbz	w1, .L3441
	ldrb	w0, [x21, 1972]
	cbnz	w0, .L3412
	ldr	x0, .LC911
	ldr	x5, [x0]
	cbz	x5, .L3413
	ldr	x1, [x20, 1464]
	cbz	x1, .L3422
	mov	w2, 0
	.p2align 2
.L3415:
	ldp	w3, w0, [x1, 8]
	ldr	x1, [x1, 4056]
	add	w0, w3, w0
	add	w0, w0, 7
	add	w2, w2, w0, lsr 3
	mov	w4, w2
	cbnz	x1, .L3415
.L3414:
	add	x2, x29, 64
	ldr	w0, [x19, 120]
	mov	w1, 20
	mov	w3, 4
	str	w4, [x2, -8]!
	blr	x5
	ldr	x1, [x19, 224]
.L3416:
	add	x2, x20, 456
	str	x2, [x19, 232]
	ldr	w0, [x1, 864]
	cmp	w0, 1
	beq	.L3466
.L3417:
	cmp	w0, 2
	beq	.L3467
.L3418:
	ldr	w2, [x1, 592]
	mov	w0, 0
	str	w2, [x20, 1728]
	ldr	w1, [x1, 632]
	str	w1, [x20, 1732]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3394:
	ldp	w0, w1, [x19, 140]
	ldr	x2, [x19, 224]
	add	w0, w0, 1
	cmp	w0, w1
	csel	w0, w0, w1, ge
	str	w0, [x19, 140]
	sub	w0, w0, w1
	str	w0, [x2, 576]
	b	.L3395
	.p2align 3
.L3400:
	mov	x0, x19
	mov	w21, -1
	bl	MVC_ClearAllSlice
	add	x0, x19, 11120640
	ldr	w4, [x19, 40]
	mov	w1, 7
	mov	w3, 23352
	movk	w3, 0x1, lsl 16
	mov	w2, 2
	strb	w1, [x0, 371]
	mov	w1, 32
	strb	wzr, [x0, 368]
	strb	w2, [x0, 369]
	strb	w1, [x0, 370]
	str	w4, [x0, 380]
	str	w3, [x0, 384]
	str	w20, [x0, 392]
.L3455:
	add	x6, x0, 256
	mov	w2, 262144
	mov	w1, w21
	mov	x0, x19
	stp	w21, wzr, [x6, 140]
	stp	wzr, w2, [x6, 148]
	bl	MVC_ClearDPB
	mov	w0, w21
.L3441:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3462:
	ldrb	w0, [x21, 1972]
	cmp	w0, 2
	bne	.L3407
	ldr	w0, [x21, 2624]
	cbz	w0, .L3404
	ldrb	w0, [x19, 11]
	cmp	w0, 1
	bne	.L3407
.L3420:
	ldrb	w1, [x21, 1973]
	strb	w1, [x19, 11]
	ldr	w0, [x19, 64]
	b	.L3408
	.p2align 3
.L3399:
	ldrb	w0, [x21, 1972]
	cbnz	w0, .L3404
	ldr	w0, [x19, 64]
	cbz	w0, .L3419
	ldr	x1, [x19, 864]
	ldr	w1, [x1, 48]
	cbz	w1, .L3419
.L3404:
	mov	w1, 1
	mov	x0, x19
	strb	w1, [x21, 1967]
	b	.L3458
	.p2align 3
.L3412:
	add	x0, x19, 11124736
	ldr	w0, [x0, 764]
	cbnz	w0, .L3413
	ldr	x1, [x19, 224]
	ldr	w0, [x1, 12]
	cmp	w0, 2
	beq	.L3416
.L3459:
	mov	x0, x19
.L3458:
	bl	MVC_ClearAllSlice
	add	x1, x19, 11120640
	ldr	w8, [x19, 40]
	mov	w7, 23352
	mov	w6, 2
	movk	w7, 0x1, lsl 16
	mov	w0, -1
	mov	w5, 7
	mov	w4, 32
	mov	w3, 3
	mov	w2, 262144
	strb	wzr, [x1, 368]
	strb	w6, [x1, 369]
	str	w8, [x1, 380]
	str	w7, [x1, 384]
.L3456:
	strb	w4, [x1, 370]
	add	x4, x1, 256
	strb	w5, [x1, 371]
	stp	w3, w0, [x4, 136]
	stp	wzr, wzr, [x4, 144]
	str	w2, [x1, 408]
	b	.L3441
	.p2align 3
.L3464:
	ldrb	w22, [x19, 7]
	cmp	w22, 2
	bne	.L3411
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 12]
	cbnz	w0, .L3411
	ldr	x1, .LC908
	mov	w0, 1
	ldr	w3, [x21, 2624]
	mov	w20, -1
	ldr	w2, [x21, 2628]
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_ClearAllSlice
	add	x0, x19, 11120640
	ldr	w4, [x19, 40]
	add	x5, x0, 256
	mov	w3, 23352
	movk	w3, 0x1, lsl 16
	mov	w2, 7
	strb	wzr, [x0, 368]
	mov	w1, 32
	strb	w2, [x0, 371]
	mov	w2, 3
	strb	w1, [x0, 370]
	mov	w1, 262144
	strb	w22, [x0, 369]
	str	w4, [x0, 380]
	str	w3, [x0, 384]
	stp	w2, w20, [x5, 136]
	stp	wzr, wzr, [x5, 144]
	str	w1, [x0, 408]
	mov	w2, 0
	ldr	x1, [x21, 2600]
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetDisplay
	mov	w0, w20
	b	.L3441
	.p2align 3
.L3465:
	ldr	w1, [x21, 2688]
	cbnz	w1, .L3411
	strb	w0, [x19, 7]
	b	.L3411
	.p2align 3
.L3413:
	ldr	x1, [x19, 224]
	add	x2, x20, 456
	str	x2, [x19, 232]
	ldr	w0, [x1, 864]
	cmp	w0, 1
	bne	.L3417
.L3466:
	cbz	x2, .L3418
	ldrb	w22, [x21, 1972]
	cmp	w22, 2
	bne	.L3418
	str	xzr, [x19, 232]
	mov	w2, 0
	ldr	w0, [x19, 120]
	ldr	x1, [x21, 2600]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetDisplay
	mov	x0, x19
.L3457:
	bl	MVC_ClearAllSlice
	add	x1, x19, 11120640
	ldr	w7, [x19, 40]
	mov	w6, 23352
	mov	w0, -1
	movk	w6, 0x1, lsl 16
	mov	w5, 7
	mov	w4, 32
	mov	w3, 3
	mov	w2, 262144
	strb	wzr, [x1, 368]
	strb	w22, [x1, 369]
	str	w7, [x1, 380]
	str	w6, [x1, 384]
	b	.L3456
	.p2align 3
.L3463:
	ldrb	w0, [x21, 1972]
	cmp	w0, 2
	beq	.L3420
	b	.L3407
	.p2align 3
.L3467:
	ldr	w0, [x1, 868]
	cmp	w0, wzr
	ccmp	x2, xzr, 4, gt
	beq	.L3418
	ldrb	w22, [x21, 1972]
	cmp	w22, 2
	bne	.L3418
	str	xzr, [x19, 232]
	mov	w2, 0
	ldr	w0, [x19, 120]
	ldr	x1, [x21, 2600]
	ldrsb	w1, [x1, 6]
	bl	FSP_SetDisplay
	ldr	x2, [x19, 224]
	mov	x0, x19
	ldr	w1, [x2, 868]
	sub	w1, w1, #1
	str	w1, [x2, 868]
	b	.L3457
	.p2align 3
.L3422:
	mov	w4, 0
	b	.L3414
.L3461:
	ldr	x1, .LC906
	mov	w2, w20
	mov	w0, 2
	mov	w21, -1
	bl	dprint_vfmw
	strb	w20, [x19, 8]
	mov	x0, x19
	bl	MVC_ClearAllSlice
	add	x0, x19, 11120640
	ldr	w4, [x19, 40]
	mov	w2, 2
	mov	w1, 7
	mov	w3, 23352
	movk	w3, 0x1, lsl 16
	strb	w2, [x0, 369]
	strb	w1, [x0, 371]
	mov	w2, 32
	mov	w1, 3
	strb	wzr, [x0, 368]
	strb	w2, [x0, 370]
	str	w4, [x0, 380]
	str	w3, [x0, 384]
	str	w1, [x0, 392]
	b	.L3455
	.size	MVC_DecVDM, .-MVC_DecVDM
	.align	3
.LC906:
	.xword	.LC905
	.align	3
.LC908:
	.xword	.LC907
	.align	3
.LC910:
	.xword	.LC909
	.align	3
.LC911:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_FlushDecoder
	.type	MVC_FlushDecoder, %function
MVC_FlushDecoder:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 11116544
	mov	w1, 1
	mov	x0, x19
	strb	w1, [x2, 2813]
	str	wzr, [x2, 2864]
	strb	w1, [x19]
	bl	MVC_DecVDM
	cbz	w0, .L3468
	ldr	x2, .LC912
	mov	w3, 12955
	ldr	x1, .LC914
	mov	w0, 22
	add	x2, x2, 312
	bl	dprint_vfmw
.L3468:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	MVC_FlushDecoder, .-MVC_FlushDecoder
	.align	3
.LC912:
	.xword	.LANCHOR0
	.align	3
.LC914:
	.xword	.LC913
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ReceivePacket
	.type	MVC_ReceivePacket, %function
MVC_ReceivePacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x1
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 11161600
	bl	_mcount
	ldr	x1, .LC916
	mov	w0, 7
	ldp	w3, w4, [x20, 16]
	ldr	x2, [x20, 8]
	bl	dprint_vfmw
	str	wzr, [x21, 1744]
	ldr	x0, [x19, 224]
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L3538
.L3474:
	ldr	x0, [x19, 240]
	cbz	x0, .L3479
	ldrb	w1, [x20]
	cmp	w1, 1
	beq	.L3539
.L3477:
	ldr	x1, .LC918
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	cbz	x1, .L3479
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L3479:
	ldr	w0, [x20, 20]
	cmp	w0, wzr
	ble	.L3481
	ldr	x0, [x20, 8]
	cbz	x0, .L3481
	ldr	w0, [x20, 16]
	cbz	w0, .L3481
	ldrb	w0, [x19, 1521]
	cbz	w0, .L3499
	add	x1, x19, 1617
	mov	x0, 1
	b	.L3484
	.p2align 3
.L3483:
	add	w0, w0, 1
	cmp	w0, 137
	beq	.L3497
.L3484:
	ldrb	w2, [x1]
	add	x1, x1, 96
	cbnz	w2, .L3483
	sxtw	x1, w0
.L3482:
	lsl	x2, x0, 2
	mov	w3, 1
	sub	x0, x2, x0
	cmn	w1, #1
	add	x0, x19, x0, lsl 5
	strb	w3, [x0, 1521]
	beq	.L3497
	lsl	x0, x1, 2
	mov	w2, 1
	sub	x0, x0, x1
	lsl	x0, x0, 5
	add	x1, x0, 1520
	add	x0, x19, x0
	add	x1, x19, x1
	str	x1, [x19, 240]
	ldr	x1, [x20, 8]
	str	x1, [x0, 1528]
	ldr	w1, [x20, 20]
	str	w1, [x0, 1536]
	ldr	w1, [x20, 16]
	str	w1, [x0, 1540]
	ldr	x0, [x19, 240]
	str	wzr, [x0, 28]
	ldr	x0, [x19, 240]
	ldr	w1, [x20, 24]
	str	w1, [x0, 36]
	ldr	x0, [x19, 240]
	ldr	x1, [x20, 32]
	str	x1, [x0, 88]
	ldr	x0, [x19, 240]
	str	w2, [x0, 76]
	ldr	x0, [x19, 240]
	strb	wzr, [x0]
	ldr	x0, [x19, 240]
	ldrb	w1, [x20]
	strb	w1, [x0, 3]
	ldr	x0, [x19, 240]
	b	.L3489
	.p2align 3
.L3539:
	ldr	w2, [x0, 76]
	cmp	w2, 1
	bhi	.L3477
	ldrb	w2, [x0, 3]
	cmp	w2, 1
	bne	.L3478
	b	.L3477
	.p2align 3
.L3538:
	ldr	w0, [x0, 872]
	cbz	w0, .L3474
	ldr	x0, [x19, 240]
	cbz	x0, .L3479
	ldrb	w1, [x20]
.L3478:
	strb	w1, [x0, 3]
	ldr	x0, [x19, 224]
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L3540
.L3491:
	ldr	w0, [x20, 20]
	cmp	w0, wzr
	ble	.L3492
	ldr	x1, [x20, 8]
	cbz	x1, .L3492
	ldr	w0, [x20, 16]
	cbz	w0, .L3492
	ldr	x0, [x19, 240]
	ldr	w2, [x0, 76]
	cmp	w2, 1
	bls	.L3541
.L3493:
	mov	w1, 1
	strb	w1, [x0, 3]
	ldr	w0, [x19, 120]
	ldr	w1, [x20, 24]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC921
	ldr	x4, [x0]
	cbz	x4, .L3535
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3535:
	ldr	x0, [x19, 240]
.L3489:
	ldrb	w0, [x0, 3]
	ldp	x19, x20, [sp, 16]
	cmp	w0, wzr
	ldr	x21, [sp, 32]
	csetm	w0, eq
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3481:
	ldr	x1, .LC923
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 120]
	ldr	w1, [x20, 24]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC921
	ldr	x4, [x0]
	cbz	x4, .L3537
.L3536:
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3537:
	mov	w0, -1
.L3544:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3492:
	ldr	x0, [x19, 240]
	b	.L3493
	.p2align 3
.L3540:
	ldr	w0, [x0, 872]
	cbnz	w0, .L3535
	b	.L3491
	.p2align 3
.L3541:
	ubfiz	x2, x2, 5, 32
	add	x0, x0, x2
	str	x1, [x0, 8]
	ldr	x1, [x19, 240]
	ldr	w2, [x20, 20]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	w2, [x0, 16]
	ldr	x1, [x19, 240]
	ldr	w2, [x20, 16]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	w2, [x0, 20]
	ldr	x1, [x19, 240]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	wzr, [x0, 28]
	ldr	x1, [x19, 240]
	ldr	w2, [x20, 24]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	w2, [x0, 36]
	ldr	x1, [x19, 240]
	ldr	w0, [x1, 76]
	add	w0, w0, 1
	str	w0, [x1, 76]
	ldr	x0, [x19, 240]
	ldr	w1, [x0, 16]
	cmp	w1, 4095
	bls	.L3542
.L3494:
	mov	w1, 1
	str	w1, [x21, 1744]
	b	.L3489
	.p2align 3
.L3542:
	mov	x0, x19
	bl	MVC_CombinePacket.part.12
	ldr	x0, [x19, 240]
	b	.L3494
.L3497:
	ldr	x1, .LC920
	mov	w0, 0
	add	x21, x19, 12288
	add	x20, x19, 1520
	add	x21, x21, 2384
	bl	dprint_vfmw
	b	.L3486
	.p2align 3
.L3485:
	add	x20, x20, 96
	cmp	x21, x20
	beq	.L3543
.L3486:
	ldrb	w0, [x20, 1]
	cmp	w0, 1
	bne	.L3485
	ldr	w0, [x19, 120]
	mov	x1, x20
	bl	MVC_ReleaseNAL
	b	.L3485
.L3543:
	mov	x0, x19
	mov	w20, -1
	bl	MVC_ClearAllSlice
	add	x0, x19, 11120640
	mov	w3, 23352
	movk	w3, 0x1, lsl 16
	ldr	w4, [x19, 40]
	mov	w2, 2
	mov	w1, 7
	str	w3, [x0, 384]
	add	x3, x0, 256
	strb	w2, [x0, 369]
	mov	w2, 32
	strb	w1, [x0, 371]
	mov	w1, 3
	strb	w2, [x0, 370]
	mov	w2, 262144
	str	w4, [x0, 380]
	strb	wzr, [x0, 368]
	stp	w1, w20, [x3, 136]
	stp	wzr, wzr, [x3, 144]
	str	w2, [x0, 408]
	mov	w1, w20
	mov	x0, x19
	bl	MVC_ClearDPB
	ldr	x0, .LC921
	ldr	x4, [x0]
	cbnz	x4, .L3536
	mov	w0, -1
	b	.L3544
.L3499:
	mov	x1, 0
	mov	x0, 0
	b	.L3482
	.size	MVC_ReceivePacket, .-MVC_ReceivePacket
	.align	3
.LC916:
	.xword	.LC915
	.align	3
.LC918:
	.xword	.LC917
	.align	3
.LC920:
	.xword	.LC919
	.align	3
.LC921:
	.xword	g_event_report
	.align	3
.LC923:
	.xword	.LC922
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_ClearAll
	.type	MVC_ClearAll, %function
MVC_ClearAll:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	w22, w1
	mov	x0, x30
	add	x1, x20, 11120640
	mov	x23, x1
	str	x1, [x29, 104]
	bl	_mcount
	add	x21, x20, 12288
	ldr	x1, .LC925
	mov	w0, 22
	add	x19, x20, 1520
	add	x21, x21, 2384
	bl	dprint_vfmw
	mov	x0, x20
	bl	MVC_ClearAllSlice
	mov	w1, 23352
	ldr	w2, [x20, 40]
	movk	w1, 0x1, lsl 16
	mov	w0, 7
	mov	w4, 2
	mov	w3, 32
	strb	w0, [x23, 371]
	mov	w0, 262144
	str	w2, [x23, 380]
	mov	w2, 3
	str	w1, [x23, 384]
	mov	w1, -1
	strb	wzr, [x23, 368]
	strb	w4, [x23, 369]
	strb	w3, [x23, 370]
	str	wzr, [x23, 400]
	str	wzr, [x23, 404]
	str	w0, [x23, 408]
	str	w2, [x23, 392]
	str	w1, [x23, 396]
	b	.L3547
	.p2align 3
.L3546:
	add	x19, x19, 96
	cmp	x19, x21
	beq	.L3578
.L3547:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L3546
	ldr	w0, [x20, 120]
	mov	x1, x19
	add	x19, x19, 96
	bl	MVC_ReleaseNAL
	cmp	x19, x21
	bne	.L3547
.L3578:
	cbz	x20, .L3550
	mov	x0, x20
	bl	MVC_ClearAllSlice
	mov	x0, x20
	bl	MVC_InitDPB.part.15
	cbnz	w0, .L3550
.L3549:
	cbz	w22, .L3551
	ldr	w0, [x20, 120]
	add	x1, x20, 912
	bl	FSP_ClearNotInVoQueue
.L3552:
	mov	x25, x20
	mov	w28, 0
	mov	w26, -1
	mov	w21, 1
	mov	w24, 18
	mov	w23, 16
	mov	w22, 2
	.p2align 2
.L3555:
	add	x0, x25, 11124736
	add	x2, x0, 1024
	add	x1, x0, 1128
	add	x19, x0, 1760
	add	x27, x0, 1792
	strb	wzr, [x0, 1129]
	strb	wzr, [x0, 1130]
	strb	wzr, [x0, 1131]
	strb	wzr, [x0, 1133]
	strb	w21, [x0, 1135]
	strb	wzr, [x0, 1824]
	strb	w21, [x0, 1872]
	strb	w22, [x0, 1920]
	stp	wzr, w26, [x2, 124]
	stp	wzr, wzr, [x2, 132]
	str	w24, [x0, 1176]
	str	w23, [x0, 1180]
	str	w28, [x0, 1356]
	str	x1, [x0, 1832]
	str	x1, [x0, 1880]
	str	x1, [x0, 1928]
.L3554:
	ldr	x1, [x19]
	cbnz	x1, .L3579
.L3553:
	add	x19, x19, 8
	cmp	x19, x27
	bne	.L3554
	add	w28, w28, 1
	add	x25, x25, 840
	cmp	w28, 40
	bne	.L3555
	add	x22, x20, 11157504
	add	x19, x22, 2536
	add	x21, x22, 2568
.L3557:
	ldr	x1, [x19]
	cbnz	x1, .L3580
.L3556:
	add	x19, x19, 8
	cmp	x21, x19
	bne	.L3557
	ldr	w2, [x20, 60]
	mov	w19, 0
	cbz	w2, .L3561
	.p2align 2
.L3568:
	add	x21, x20, x19, uxtw 3
	add	w19, w19, 1
	ldr	x0, [x21, 832]
	mov	x1, x0
	cbz	x0, .L3559
	ldr	w0, [x20, 120]
	bl	FreeUsdByDec
	str	xzr, [x21, 832]
	ldr	w2, [x20, 60]
.L3559:
	cmp	w2, w19
	bhi	.L3568
.L3561:
	ldr	x0, .LC928
	mov	x2, 1656
	strb	wzr, [x20, 4]
	mov	w1, 0
	strb	wzr, [x20, 9]
	strb	wzr, [x20, 6]
	strb	wzr, [x20, 7]
	strb	wzr, [x20, 3]
	stp	wzr, wzr, [x20, 60]
	str	wzr, [x20, 104]
	ldr	x3, [x0, 96]
	ldr	x0, [x29, 104]
	str	wzr, [x20, 220]
	str	xzr, [x20, 240]
	add	x20, x20, 11116544
	strb	wzr, [x0, 2420]
	strb	wzr, [x0, 416]
	strb	wzr, [x0, 2421]
	strb	wzr, [x0, 417]
	add	x0, x20, 2808
	blr	x3
	mov	w1, 2
	mov	w0, 1
	strb	w1, [x20, 2808]
	strb	w0, [x20, 2813]
	mov	w0, 256
	strb	wzr, [x22, 1960]
	str	w0, [x22, 3440]
	mov	w0, 0
	strb	wzr, [x22, 3432]
	str	wzr, [x22, 3436]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3579:
	ldr	w0, [x20, 120]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L3553
.L3580:
	ldr	w0, [x20, 120]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L3556
.L3551:
	add	x0, x20, 912
	bl	ResetVoQueue
	ldr	w0, [x20, 120]
	bl	FSP_EmptyInstance
	b	.L3552
.L3550:
	ldr	x2, .LC926
	mov	w3, 344
	ldr	x1, .LC927
	mov	w0, 22
	add	x2, x2, 336
	bl	dprint_vfmw
	b	.L3549
	.size	MVC_ClearAll, .-MVC_ClearAll
	.align	3
.LC925:
	.xword	.LC924
	.align	3
.LC926:
	.xword	.LANCHOR0
	.align	3
.LC927:
	.xword	.LC903
	.align	3
.LC928:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_InsertFrmInDPB
	.type	MVC_InsertFrmInDPB, %function
MVC_InsertFrmInDPB:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x23, x0
	mov	x0, x30
	uxtw	x21, w1
	mov	x19, x2
	bl	_mcount
	add	x0, x23, 11124736
	ldr	w0, [x0, 764]
	cbnz	w0, .L3582
	ldr	x0, .LC929
	str	wzr, [x0, 1948]
.L3582:
	ldrb	w22, [x19, 3]
	cmp	w22, 1
	beq	.L3584
	cbz	w22, .L3585
	cmp	w22, 2
	beq	.L3586
	ldr	x1, .LC933
	mov	w2, w22
	mov	w0, 0
	mov	w19, 0
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x23
	bl	MVC_ClearAll
.L3590:
	mov	w0, w19
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3586:
	ldr	x0, [x19, 640]
	mov	w1, 3
	strb	w1, [x0, 696]
	ldrb	w0, [x19, 1]
	cbz	w0, .L3604
	add	x20, x23, x21, lsl 3
	add	x20, x20, 11124736
	ldr	x0, [x20, 376]
	cbz	x0, .L3604
	strb	w22, [x0, 792]
	ldr	x0, [x20, 376]
	ldr	x1, [x19, 640]
	str	x1, [x0, 800]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 4]
	strb	w1, [x0, 793]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 5]
	strb	w1, [x0, 794]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 680]
	str	w1, [x0, 816]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 12]
	strb	w1, [x0, 795]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 700]
	str	w1, [x0, 824]
	ldr	x0, [x20, 376]
	ldr	w2, [x0, 776]
	ldr	w1, [x0, 824]
	add	w1, w2, w1
	str	w1, [x0, 44]
	str	w1, [x0, 728]
	ldr	x1, [x20, 376]
	ldrb	w0, [x1, 1]
	cbz	w0, .L3610
	ldrb	w0, [x19, 2]
.L3610:
	strb	w0, [x1, 1]
	ldr	x1, [x20, 376]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 2
	strb	w0, [x1, 2]
	ldrb	w0, [x19, 7]
	ldr	x1, [x20, 376]
	cmp	w0, 1
	beq	.L3678
	ldrb	w0, [x1, 4]
.L3612:
	strb	w0, [x1, 4]
	mov	x2, 624
	add	x1, x19, 16
	ldr	x0, [x20, 376]
	add	x0, x0, 72
	ldr	x25, [x0, 600]
	ldr	x24, [x0, 608]
	ldr	x22, [x0, 616]
	bl	memcpy
	ldr	x0, [x19, 616]
	cmn	x0, #1
	ccmn	x25, #1, 4, eq
	beq	.L3613
	ldr	x0, [x20, 376]
	str	x25, [x0, 672]
.L3613:
	ldr	x0, [x19, 624]
	ldr	x1, [x20, 376]
	cmp	x0, x24
	csel	x0, x0, x24, ls
	str	x0, [x1, 680]
	ldr	x0, [x19, 632]
	ldr	x1, [x20, 376]
	cmp	x0, x22
	csel	x0, x0, x22, ls
	str	x0, [x1, 688]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3614
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3614
.L3615:
	ldr	x1, [x20, 376]
	ldrb	w2, [x1, 795]
	ldrb	w0, [x1, 747]
	cmp	w0, w2, uxtb
	csel	w0, w0, w2, cs
	strb	w0, [x1]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 696]
	str	w1, [x0, 820]
	ldr	x1, [x20, 376]
	ldr	w0, [x1, 772]
	ldr	w2, [x1, 820]
	cmp	w0, w2
	csel	w0, w0, w2, le
	str	w0, [x1, 36]
	ldr	x0, [x20, 376]
	bl	MVC_CombineFldsToFrm
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 11]
	strb	w1, [x0, 13]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 10]
	strb	w1, [x0, 11]
	ldr	x1, [x20, 376]
	ldr	w0, [x1, 52]
	ldr	w1, [x1, 816]
	add	x0, x0, 2781184
	add	x0, x0, 232
	str	w1, [x23, x0, lsl 2]
	b	.L3603
	.p2align 3
.L3585:
	add	x20, x23, x21, lsl 3
	ldr	x1, [x19, 640]
	add	x20, x20, 11124736
	mov	w2, 3
	mov	w0, 3
	str	x1, [x20, 376]
	ldrb	w3, [x19, 2]
	strb	w3, [x1, 1]
	ldr	x1, [x20, 376]
	strb	w2, [x1, 2]
	ldr	x2, [x19, 640]
	ldr	x1, [x20, 376]
	ldrb	w2, [x2, 5]
	strb	w2, [x1, 5]
	ldr	x2, [x19, 640]
	ldr	x1, [x20, 376]
	ldrb	w2, [x2, 7]
	strb	w2, [x1, 7]
	ldrb	w2, [x19, 4]
	ldr	x1, [x20, 376]
	cbz	w2, .L3679
.L3587:
	strb	w0, [x1, 3]
	mov	x2, 624
	add	x1, x19, 16
	mov	w22, 3
	ldr	x0, [x20, 376]
	ldr	w3, [x19, 724]
	str	w3, [x0, 48]
	ldr	x0, [x20, 376]
	add	x0, x0, 72
	bl	memcpy
	ldr	x0, [x20, 376]
	mov	w6, 1
	ldr	w1, [x19, 700]
	str	w1, [x0, 44]
	str	w1, [x0, 728]
	ldr	x0, [x20, 376]
	ldr	x1, [x19, 640]
	str	x1, [x0, 704]
	str	x1, [x0, 752]
	str	x1, [x0, 800]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 660]
	str	w1, [x0, 28]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 656]
	str	w1, [x0, 20]
	ldr	x0, [x20, 376]
	strb	wzr, [x0, 696]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 4]
	strb	w1, [x0, 697]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 5]
	strb	w1, [x0, 698]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 12]
	strb	w1, [x0, 699]
	strb	w1, [x0]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 9]
	str	w1, [x0, 40]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 668]
	str	w1, [x0, 32]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 672]
	str	w1, [x0, 720]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 676]
	str	w1, [x0, 768]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 680]
	str	w1, [x0, 816]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 684]
	str	w1, [x0, 36]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 688]
	str	w1, [x0, 724]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 692]
	str	w1, [x0, 772]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 696]
	str	w1, [x0, 820]
	ldrb	w1, [x19, 7]
	ldr	x0, [x20, 376]
	cmp	w1, 1
	csel	w22, wzr, w22, ne
	strb	w22, [x0, 4]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 728]
	str	w1, [x0, 56]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 732]
	str	w1, [x0, 60]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 11]
	strb	w1, [x0, 13]
	strb	w1, [x0, 12]
	ldr	x1, .LC929
	ldr	x2, [x20, 376]
	ldrb	w3, [x19, 10]
	ldr	w0, [x1, 1948]
	strb	w3, [x2, 11]
	strb	w3, [x2, 10]
	add	w2, w0, 1
	str	w2, [x1, 1948]
	ldr	x1, [x20, 376]
	str	w0, [x1, 64]
	ldr	x0, [x20, 376]
	ldrb	w2, [x0, 699]
	ldr	x1, [x0, 704]
	ldrb	w4, [x0, 697]
	ldrb	w3, [x0, 698]
	ldr	w5, [x0, 728]
	strb	w2, [x0, 747]
	strb	w2, [x0, 795]
	mov	w2, 0
	str	x1, [x0, 752]
	str	x1, [x0, 800]
	mov	w1, 2
	strb	w4, [x0, 745]
	strb	w3, [x0, 746]
	strb	w6, [x0, 744]
	strb	w4, [x0, 793]
	strb	w3, [x0, 794]
	str	w5, [x0, 776]
	str	w5, [x0, 824]
	strb	w1, [x0, 792]
	mov	x0, x23
	ldr	x1, [x19, 640]
	bl	MVC_GetAPC
	cbnz	w0, .L3680
.L3603:
	add	x21, x21, 1388544
	mov	w19, 0
	add	x21, x23, x21, lsl 3
	ldr	x0, [x21, 16760]
	ldrb	w1, [x0, 3]
	cbz	w1, .L3590
	ldrsb	w1, [x0, 6]
	mov	w2, 1
	ldr	w0, [x23, 120]
	bl	FSP_SetRef
	ldr	x25, [sp, 64]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3584:
	ldr	x0, [x19, 640]
	mov	w1, 3
	strb	w1, [x0, 696]
	ldrb	w0, [x19, 1]
	cbz	w0, .L3591
	add	x20, x23, x21, lsl 3
	add	x20, x20, 11124736
	ldr	x0, [x20, 376]
	cbz	x0, .L3591
	strb	w22, [x0, 744]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 12]
	strb	w1, [x0, 747]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 4]
	strb	w1, [x0, 745]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 5]
	strb	w1, [x0, 746]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 676]
	str	w1, [x0, 768]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 12]
	strb	w1, [x0, 747]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 700]
	str	w1, [x0, 776]
	ldr	x0, [x20, 376]
	ldr	w2, [x0, 776]
	ldr	w1, [x0, 824]
	add	w1, w2, w1
	str	w1, [x0, 44]
	str	w1, [x0, 728]
	ldr	x1, [x20, 376]
	ldrb	w0, [x1, 1]
	cbz	w0, .L3596
	ldrb	w0, [x19, 2]
.L3596:
	strb	w0, [x1, 1]
	ldr	x1, [x20, 376]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 1
	strb	w0, [x1, 2]
	ldrb	w0, [x19, 7]
	ldr	x1, [x20, 376]
	cmp	w0, 1
	beq	.L3681
	ldrb	w0, [x1, 4]
.L3598:
	strb	w0, [x1, 4]
	mov	x2, 624
	add	x1, x19, 16
	ldr	x0, [x20, 376]
	add	x0, x0, 72
	ldr	x25, [x0, 600]
	ldr	x24, [x0, 608]
	ldr	x22, [x0, 616]
	bl	memcpy
	ldr	x0, [x19, 616]
	cmn	x0, #1
	ccmn	x25, #1, 4, eq
	beq	.L3599
	ldr	x0, [x20, 376]
	str	x25, [x0, 672]
.L3599:
	ldr	x0, [x19, 624]
	ldr	x1, [x20, 376]
	cmp	x0, x24
	csel	x0, x0, x24, ls
	str	x0, [x1, 680]
	ldr	x0, [x19, 632]
	ldr	x1, [x20, 376]
	cmp	x0, x22
	csel	x0, x0, x22, ls
	str	x0, [x1, 688]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3600
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3600
.L3601:
	ldr	x1, [x20, 376]
	ldrb	w2, [x1, 795]
	ldrb	w0, [x1, 747]
	cmp	w0, w2, uxtb
	csel	w0, w0, w2, cs
	strb	w0, [x1]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 692]
	str	w1, [x0, 772]
	ldr	x1, [x20, 376]
	ldr	w0, [x1, 772]
	ldr	w2, [x1, 820]
	cmp	w0, w2
	csel	w0, w0, w2, le
	str	w0, [x1, 36]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 11]
	strb	w1, [x0, 12]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 10]
	strb	w1, [x0, 10]
	ldr	x0, [x20, 376]
	bl	MVC_CombineFldsToFrm
	ldr	x1, [x20, 376]
	ldr	w0, [x1, 52]
	ldr	w1, [x1, 768]
	add	x0, x0, 2781184
	add	x0, x0, 216
	str	w1, [x23, x0, lsl 2]
	b	.L3603
	.p2align 3
.L3591:
	add	x20, x23, x21, lsl 3
	ldr	x0, [x19, 640]
	add	x20, x20, 11124736
	mov	w1, 1
	str	x0, [x20, 376]
	strb	w1, [x0, 744]
	ldr	x0, [x20, 376]
	ldr	x2, [x19, 640]
	str	x2, [x0, 752]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 4]
	strb	w2, [x0, 745]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 5]
	strb	w2, [x0, 746]
	ldr	x0, [x20, 376]
	ldr	w2, [x19, 676]
	str	w2, [x0, 768]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 12]
	strb	w2, [x0, 747]
	ldr	x0, [x20, 376]
	ldr	w2, [x19, 700]
	str	w2, [x0, 776]
	ldr	x0, [x20, 376]
	ldr	w2, [x0, 776]
	str	w2, [x0, 44]
	str	w2, [x0, 728]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 2]
	strb	w2, [x0, 1]
	ldr	x0, [x20, 376]
	strb	w1, [x0, 2]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 376]
	ldrb	w1, [x1, 5]
	strb	w1, [x0, 5]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 376]
	ldrb	w1, [x1, 7]
	strb	w1, [x0, 7]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3593
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3593
.L3594:
	ldr	x0, [x20, 376]
	mov	x2, 624
	add	x1, x19, 16
	str	wzr, [x0, 40]
	ldr	x0, [x20, 376]
	add	x0, x0, 72
	bl	memcpy
	ldr	x0, [x20, 376]
	mov	w2, 1
	ldr	w1, [x19, 724]
	ldr	x3, .LC929
	str	w1, [x0, 48]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 656]
	str	w1, [x0, 20]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 668]
	str	w1, [x0, 32]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 684]
	str	w1, [x0, 36]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 12]
	strb	w1, [x0]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 692]
	str	w1, [x0, 772]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 728]
	str	w1, [x0, 56]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 732]
	str	w1, [x0, 60]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 11]
	strb	w1, [x0, 12]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 10]
	strb	w1, [x0, 10]
	mov	x0, x23
	ldr	x4, [x20, 376]
	ldr	w1, [x3, 1948]
	str	w1, [x4, 64]
	add	w5, w1, w2
	str	w5, [x3, 1948]
	ldr	x1, [x20, 376]
	ldrb	w3, [x19, 7]
	cmp	w3, w2
	cset	w3, eq
	strb	w3, [x1, 4]
	ldr	x1, [x19, 640]
	bl	MVC_GetAPC
	cbz	w0, .L3603
	ldr	x1, .LC931
	mov	w3, w0
	mov	w2, 2268
	b	.L3676
	.p2align 3
.L3604:
	add	x20, x23, x21, lsl 3
	ldr	x0, [x19, 640]
	add	x20, x20, 11124736
	mov	w1, 2
	str	x0, [x20, 376]
	strb	w1, [x0, 792]
	ldr	x0, [x20, 376]
	ldr	x2, [x19, 640]
	str	x2, [x0, 800]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 4]
	strb	w2, [x0, 793]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 5]
	strb	w2, [x0, 794]
	ldr	x0, [x20, 376]
	ldr	w2, [x19, 680]
	str	w2, [x0, 816]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 12]
	strb	w2, [x0, 795]
	ldr	x0, [x20, 376]
	ldr	w2, [x19, 700]
	str	w2, [x0, 824]
	ldr	x0, [x20, 376]
	ldr	w2, [x0, 776]
	str	w2, [x0, 44]
	str	w2, [x0, 728]
	ldr	x0, [x20, 376]
	ldrb	w2, [x19, 2]
	strb	w2, [x0, 1]
	ldr	x0, [x20, 376]
	strb	w1, [x0, 2]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 376]
	ldrb	w1, [x1, 5]
	strb	w1, [x0, 5]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 376]
	ldrb	w1, [x1, 7]
	strb	w1, [x0, 7]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3606
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3606
.L3607:
	ldr	x0, [x20, 376]
	mov	x2, 624
	add	x1, x19, 16
	str	wzr, [x0, 40]
	ldr	x0, [x20, 376]
	add	x0, x0, 72
	bl	memcpy
	ldr	x0, [x20, 376]
	mov	w2, 2
	ldr	w1, [x19, 724]
	str	w1, [x0, 48]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 656]
	str	w1, [x0, 20]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 668]
	str	w1, [x0, 32]
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 684]
	str	w1, [x0, 36]
	ldr	x0, [x20, 376]
	ldrb	w1, [x19, 12]
	strb	w1, [x0, 795]
	strb	w1, [x0]
	mov	x0, x23
	ldr	x1, [x20, 376]
	ldr	w3, [x19, 696]
	str	w3, [x1, 820]
	ldr	x1, [x20, 376]
	ldr	w3, [x19, 728]
	str	w3, [x1, 56]
	ldr	x1, [x20, 376]
	ldr	w3, [x19, 732]
	str	w3, [x1, 60]
	ldr	x1, [x20, 376]
	ldrb	w3, [x19, 11]
	strb	w3, [x1, 13]
	ldr	x1, [x20, 376]
	ldrb	w3, [x19, 10]
	strb	w3, [x1, 11]
	ldr	x4, [x20, 376]
	ldr	x3, .LC929
	ldr	w1, [x3, 1948]
	str	w1, [x4, 64]
	add	w1, w1, 1
	str	w1, [x3, 1948]
	ldrb	w3, [x19, 7]
	ldr	x1, [x20, 376]
	cmp	w3, 1
	csel	w22, w22, wzr, eq
	strb	w22, [x1, 4]
	ldr	x1, [x19, 640]
	bl	MVC_GetAPC
	cbz	w0, .L3603
	ldr	x1, .LC931
	mov	w3, w0
	mov	w2, 2393
.L3676:
	mov	w0, 0
	mov	w19, -1
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x23
	bl	MVC_ClearDPB
	b	.L3590
	.p2align 3
.L3679:
	ldrb	w0, [x19, 5]
	mov	w2, 3
	cmp	w0, wzr
	csel	w0, w0, w2, eq
	b	.L3587
	.p2align 3
.L3593:
	ldr	x0, [x20, 376]
	mov	w1, 1
	strb	w1, [x0, 3]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3594
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 660]
	str	w1, [x0, 28]
	b	.L3594
	.p2align 3
.L3606:
	ldr	x0, [x20, 376]
	mov	w1, 2
	strb	w1, [x0, 3]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3607
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 660]
	str	w1, [x0, 28]
	b	.L3607
	.p2align 3
.L3614:
	ldr	x1, [x20, 376]
	ldrb	w0, [x1, 3]
	orr	w0, w0, 2
	strb	w0, [x1, 3]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3615
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 660]
	str	w1, [x0, 28]
	b	.L3615
	.p2align 3
.L3600:
	ldr	x1, [x20, 376]
	ldrb	w0, [x1, 3]
	orr	w0, w0, 1
	strb	w0, [x1, 3]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3601
	ldr	x0, [x20, 376]
	ldr	w1, [x19, 660]
	str	w1, [x0, 28]
	b	.L3601
	.p2align 3
.L3678:
	ldrb	w0, [x1, 4]
	orr	w0, w0, 2
	uxtb	w0, w0
	b	.L3612
	.p2align 3
.L3681:
	ldrb	w0, [x1, 4]
	orr	w0, w0, 1
	uxtb	w0, w0
	b	.L3598
.L3680:
	ldr	x1, .LC931
	mov	w3, w0
	mov	w2, 2205
	b	.L3676
	.size	MVC_InsertFrmInDPB, .-MVC_InsertFrmInDPB
	.align	3
.LC929:
	.xword	.LANCHOR3
	.align	3
.LC931:
	.xword	.LC930
	.align	3
.LC933:
	.xword	.LC932
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_AllocFrameStore
	.type	MVC_AllocFrameStore, %function
MVC_AllocFrameStore:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x24, x0
	mov	x0, x30
	add	x20, x24, 11116544
	mov	w21, w1
	bl	_mcount
	mov	w25, 0
	ldrb	w0, [x20, 2809]
	cbz	w0, .L3683
	ldrb	w0, [x20, 2810]
	mov	w25, 2
	cmp	w0, wzr
	csinc	w25, w25, wzr, ne
.L3683:
	cmp	w21, 1
	ldrb	w0, [x24, 8]
	beq	.L3684
	cmp	w0, 2
	add	x22, x24, 11157504
	beq	.L3774
	sub	w0, w25, #1
	strb	wzr, [x22, 1961]
	cmp	w0, 1
	bls	.L3778
.L3686:
	strb	wzr, [x22, 1960]
	ldr	w5, [x24, 48]
	cbz	w5, .L3710
	add	x4, x24, 11124736
	mov	x3, 0
	add	x19, x4, 1128
	.p2align 2
.L3709:
	ldrb	w0, [x19, 2]
	cbnz	w0, .L3704
	cbz	x19, .L3705
	ldr	w0, [x4, 760]
	cbz	w0, .L3705
	ldr	x1, [x4, 376]
	cmp	x1, x19
	ccmp	x1, xzr, 4, eq
	bne	.L3704
	sub	w2, w0, #1
	add	x0, x4, 384
	add	x2, x2, 1388544
	add	x2, x2, 2096
	add	x2, x24, x2, lsl 3
	b	.L3707
	.p2align 3
.L3708:
	ldr	x1, [x0], 8
	cmp	x1, x19
	ccmp	x1, xzr, 4, eq
	bne	.L3704
.L3707:
	cmp	x2, x0
	bne	.L3708
.L3705:
	lsl	x23, x3, 3
	str	x19, [x22, 2600]
	sub	x23, x23, x3
	mov	w26, 1
	lsl	x0, x23, 4
	strb	w26, [x22, 1960]
	sub	x23, x0, x23
	mov	w3, 101
	mov	x2, 624
	add	x0, x22, 1976
	add	x23, x24, x23, lsl 3
	add	x23, x23, 11124736
	add	x1, x23, 1200
	str	wzr, [x23, 1172]
	str	w3, [x22, 2660]
	bl	memcpy
	str	wzr, [x22, 2136]
	strb	wzr, [x23, 1132]
	strb	wzr, [x19, 7]
	ldr	x0, [x22, 2600]
	strb	wzr, [x0, 3]
	ldr	x0, [x22, 2600]
	strb	w26, [x0, 5]
	ldr	x0, [x22, 2600]
	str	wzr, [x0, 16]
	ldrb	w19, [x22, 1960]
	cmp	w19, 1
	bne	.L3710
	cmp	w21, wzr
	ldr	w0, [x24, 120]
	cset	w1, eq
	bl	FSP_NewLogicFs
	mov	w23, w0
	mov	w1, w0
	ldr	w0, [x24, 120]
	bl	FSP_GetLogicFs
	mov	x26, x0
	cmp	w23, wzr
	ccmp	x0, xzr, 4, ge
	beq	.L3711
	add	x1, x0, 8
	ldr	x0, [x24, 224]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L3779
.L3712:
	ldr	x19, .LC938
	mov	x2, 624
	ldr	x0, [x22, 2600]
	ldr	x3, [x19, 104]
	add	x0, x0, 72
	blr	x3
	ldr	x1, [x22, 2600]
	mov	x2, 624
	ldr	x3, [x19, 104]
	add	x0, x22, 1976
	add	x1, x1, 72
	blr	x3
	ldr	x0, [x22, 2600]
	strb	w23, [x0, 6]
	ldrsb	w0, [x26, 4]
	str	w0, [x22, 2684]
.L3703:
	uxtb	w1, w25
	cmp	w21, 1
	strb	w1, [x22, 1963]
	beq	.L3722
.L3786:
	ldrb	w0, [x20, 2809]
	mov	w3, 3
	ldr	x2, [x22, 2600]
	cmp	w0, wzr
	csel	w0, w0, w3, eq
	strb	w0, [x2, 696]
	ldrb	w0, [x22, 1961]
	cmp	w0, 1
	beq	.L3780
	strb	w1, [x22, 3432]
	add	x0, x24, 11120640
	ldr	w1, [x20, 2836]
	str	w1, [x22, 3440]
	ldrb	w1, [x20, 2816]
	str	w1, [x22, 3436]
	ldr	w1, [x24, 128]
	str	w1, [x22, 3444]
	ldr	w0, [x0, 352]
	str	w0, [x22, 3456]
	str	w23, [x22, 3448]
	cbnz	w25, .L3781
	strb	wzr, [x22, 3433]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L3704:
	add	w3, w3, 1
	add	x19, x19, 840
	cmp	w3, w5
	bne	.L3709
.L3710:
	ldr	x1, .LC937
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x24
	bl	MVC_ClearAll
	mov	w0, -1
.L3769:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
.L3781:
	mov	w1, 1
	strb	w1, [x22, 3433]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
.L3684:
	cmp	w0, 2
	beq	.L3782
	add	x22, x24, 11157504
	mov	w25, 0
.L3774:
	strb	wzr, [x22, 1961]
	b	.L3686
.L3778:
	ldrb	w0, [x22, 3432]
	cbz	w0, .L3687
	add	x1, x24, 11120640
	ldr	w2, [x22, 3456]
	ldr	w1, [x1, 352]
	cmp	w2, w1
	beq	.L3688
.L3687:
	add	x0, x24, 11124736
	ldr	w1, [x0, 760]
	subs	w1, w1, #1
	bmi	.L3686
	add	x3, x24, 11120640
	b	.L3697
	.p2align 3
.L3690:
	cmp	w25, 2
	beq	.L3783
.L3689:
	subs	w1, w1, #1
	bmi	.L3686
.L3697:
	sxtw	x0, w1
	add	x0, x0, 1388544
	add	x0, x24, x0, lsl 3
	ldr	x19, [x0, 16760]
	cbz	x19, .L3689
	cmp	w25, 1
	bne	.L3690
	ldrb	w0, [x19, 2]
	cmp	w0, 2
	bne	.L3689
.L3691:
	ldr	w0, [x19, 20]
	ldr	w2, [x20, 2836]
	cmp	w2, w0
	bne	.L3689
	ldr	w2, [x3, 352]
	ldr	w0, [x19, 56]
	cmp	w2, w0
	bne	.L3689
	ldrb	w0, [x20, 2816]
	cbz	w0, .L3784
	ldrb	w0, [x19, 3]
	cbz	w0, .L3689
.L3693:
	mov	w0, 1
	add	x1, x19, 72
	strb	w0, [x22, 1961]
	mov	x2, 624
	add	x0, x22, 1976
	str	x19, [x22, 2600]
	bl	memcpy
	ldrsb	w1, [x19, 6]
	ldr	w0, [x24, 120]
	bl	FSP_GetLogicFs
	cbz	x0, .L3785
	ldrsb	w1, [x0, 4]
	ldrb	w0, [x22, 1961]
	str	w1, [x22, 2684]
	cbz	w0, .L3686
	ldr	x0, [x22, 2600]
	cbz	x0, .L3686
.L3773:
	uxtb	w1, w25
	cmp	w21, 1
	strb	w1, [x22, 1963]
	mov	w23, 0
	bne	.L3786
.L3722:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	mov	w0, 0
	ldp	x29, x30, [sp], 96
	ret
.L3779:
	ldp	w3, w2, [x24, 12]
	ldr	w19, [x1, 140]
	lsl	w2, w2, 4
	ldr	w0, [x1, 44]
	str	w2, [x1, 116]
	mul	w19, w2, w19
	lsl	w2, w3, 4
	str	w2, [x1, 112]
	add	w0, w19, w0
	str	w0, [x1, 48]
	str	x1, [x29, 88]
	bl	MEM_Phy2Vir
	ldr	x1, [x29, 88]
	ldr	w2, [x1, 28]
	ldr	w3, [x1, 140]
	str	x0, [x1, 512]
	add	w2, w19, w2
	ldr	w0, [x1, 60]
	str	w2, [x1, 32]
	add	w2, w3, w2
	add	w0, w19, w0
	str	w2, [x1, 40]
	str	w0, [x1, 64]
	bl	MEM_Phy2Vir
	ldr	x1, [x29, 88]
	str	x0, [x1, 528]
	b	.L3712
	.p2align 3
.L3783:
	ldrb	w0, [x19, 2]
	cmp	w0, 1
	bne	.L3689
	b	.L3691
.L3780:
	strb	wzr, [x22, 3432]
	add	x1, x24, 11120640
	mov	w0, 0
	ldr	w2, [x20, 2836]
	str	w2, [x22, 3440]
	ldrb	w2, [x20, 2816]
	str	w2, [x22, 3436]
	ldr	w2, [x24, 128]
	str	w2, [x22, 3444]
	strb	wzr, [x22, 3433]
	str	w23, [x22, 3448]
	ldr	w1, [x1, 352]
	str	w1, [x22, 3456]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
.L3782:
	add	x22, x24, 11157504
	mov	w25, 0
	strb	wzr, [x22, 1961]
	b	.L3686
.L3784:
	ldrb	w0, [x19, 3]
	cbnz	w0, .L3689
	b	.L3693
	.p2align 3
.L3688:
	cmp	w0, 1
	ccmp	w25, 2, 0, eq
	beq	.L3698
	cmp	w0, 2
	ccmp	w25, 1, 0, eq
	bne	.L3686
.L3698:
	ldr	w0, [x20, 2836]
	ldr	w1, [x22, 3440]
	cmp	w1, w0
	bne	.L3686
	ldrb	w0, [x20, 2816]
	cbnz	w0, .L3699
	ldr	w0, [x22, 3436]
	cbnz	w0, .L3686
.L3700:
	ldp	w1, w0, [x24, 124]
	sub	w0, w0, w1
	cmp	w0, 2
	bhi	.L3686
	ldr	x0, [x22, 2600]
	mov	w1, 1
	strb	w1, [x22, 1961]
	cbz	x0, .L3774
	cmp	w25, 2
	beq	.L3787
	cmp	w25, 1
	mov	w23, 0
	bne	.L3703
	ldrb	w0, [x0, 2]
	cmp	w0, 2
	beq	.L3703
	strb	wzr, [x22, 1961]
	b	.L3686
	.p2align 3
.L3711:
	ldr	x2, [x22, 2600]
	mov	w0, 0
	strb	wzr, [x22, 1960]
	ldr	x1, .LC940
	strb	wzr, [x2, 5]
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x24
	bl	MVC_ClearAll
	mov	w0, -1
	b	.L3769
.L3785:
	ldr	x1, .LC935
	mov	w2, 6591
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3769
.L3787:
	ldrb	w0, [x0, 2]
	cmp	w0, 1
	beq	.L3773
	strb	wzr, [x22, 1961]
	b	.L3686
.L3699:
	ldr	w0, [x22, 3436]
	cbz	w0, .L3686
	b	.L3700
	.size	MVC_AllocFrameStore, .-MVC_AllocFrameStore
	.align	3
.LC935:
	.xword	.LC934
	.align	3
.LC937:
	.xword	.LC936
	.align	3
.LC938:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC940:
	.xword	.LC939
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DEC_Init
	.type	MVC_DEC_Init, %function
MVC_DEC_Init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	ldr	x22, .LC941
	mov	x2, 22248
	mov	w1, 0
	movk	x2, 0xaa, lsl 16
	mov	x0, x20
	ldr	x3, [x22, 96]
	ldr	w21, [x20, 1512]
	blr	x3
	str	w21, [x20, 1512]
	add	x0, x20, 912
	bl	ResetVoQueue
	strb	wzr, [x20]
	stp	x19, xzr, [x20, 224]
	ldr	w0, [x19, 28]
	cmp	w0, 25
	beq	.L3797
	mov	w2, 32
	mov	w1, 256
	mov	w0, 136
	str	w1, [x20, 40]
	stp	w0, w2, [x20, 32]
.L3790:
	ldr	w0, [x19, 584]
	mov	w21, 2240
	bl	MEM_Phy2Vir
	ldr	x1, [x20, 224]
	mov	w2, 3992
	ldp	w4, w5, [x20, 36]
	ldrsw	x3, [x1, 580]
	smull	x2, w4, w2
	add	x3, x0, x3
	add	x1, x3, x2
	cmp	x1, xzr
	smaddl	x0, w5, w21, x1
	ccmp	x3, xzr, 4, ne
	str	x0, [x20, 864]
	ccmp	x0, xzr, 4, ne
	stp	x3, x1, [x20, 272]
	beq	.L3798
	ldr	w1, [x19, 608]
	cbz	w1, .L3799
.L3793:
	add	x2, x20, 11112448
	ldr	x6, [x22, 96]
	mov	w1, 0
	add	x23, x20, 12288
	add	x19, x23, 2396
	add	x21, x20, 10768384
	str	w4, [x2, 280]
	add	x21, x21, 4060
	str	w5, [x2, 3552]
	ldrsw	x3, [x20, 32]
	lsl	x2, x3, 7
	sub	x2, x2, x3
	lsl	x2, x2, 5
	blr	x6
	strb	wzr, [x20, 4]
	ldr	w0, [x20, 108]
	mov	w1, 40
	strb	wzr, [x20, 6]
	mov	x2, 9856
	ubfx	x0, x0, 16, 2
	strb	w0, [x20, 8]
	mov	w0, 18
	strb	wzr, [x20, 7]
	stp	w1, w0, [x20, 48]
	add	x0, x20, 512
	str	wzr, [x20, 60]
	stp	wzr, wzr, [x20, 68]
	stp	wzr, wzr, [x20, 76]
	stp	wzr, wzr, [x20, 84]
	stp	wzr, wzr, [x20, 92]
	str	wzr, [x20, 220]
	movk	x2, 0xa4, lsl 16
	stp	xzr, xzr, [x0, 320]
	stp	xzr, xzr, [x0, 336]
	mov	w0, 25
	str	w0, [x20, 56]
	mov	w1, 0
	mov	x0, x19
	ldr	x3, [x22, 96]
	blr	x3
	mov	x2, 8500
	ldr	x3, [x22, 96]
	mov	w1, 0
	movk	x2, 0x5, lsl 16
	mov	x0, x21
	blr	x3
	mov	x0, x19
	.p2align 2
.L3794:
	add	x1, x0, 274432
	add	x2, x0, 335872
	strb	wzr, [x0]
	add	x0, x2, 308
	cmp	x21, x0
	strb	wzr, [x1, 241]
	bne	.L3794
	add	x0, x20, 11108352
	strb	wzr, [x23, 2384]
	add	x0, x0, 3632
	mov	w1, 32
	str	x0, [x20, 248]
	mov	x0, x20
	strb	wzr, [x20, 2]
	stp	wzr, w1, [x20, 20]
	str	w1, [x20, 28]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x20, 120]
	beq	.L3800
	mov	w1, 0
	mov	x0, x20
	bl	MVC_ClearAll
	mov	w0, 1
.L3792:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L3799:
	ldr	x4, [x22, 96]
	mov	x0, x3
	blr	x4
	ldr	w2, [x20, 40]
	mov	w1, 0
	ldr	x0, [x20, 280]
	ldr	x3, [x22, 96]
	smull	x2, w2, w21
	blr	x3
	ldp	w4, w5, [x20, 36]
	ldr	x0, [x20, 864]
	b	.L3793
.L3797:
	ldr	w0, [x19, 684]
	str	w0, [x20, 36]
	ldr	w0, [x19, 688]
	str	w0, [x20, 40]
	ldr	w0, [x19, 676]
	str	w0, [x20, 32]
	b	.L3790
.L3798:
	ldr	x2, .LC942
	mov	w3, 13884
	ldr	x1, .LC944
	mov	w0, 22
	add	x2, x2, 352
	bl	dprint_vfmw
	mov	w0, -20
	b	.L3792
.L3800:
	ldr	x1, .LC946
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L3792
	.size	MVC_DEC_Init, .-MVC_DEC_Init
	.align	3
.LC941:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC942:
	.xword	.LANCHOR0
	.align	3
.LC944:
	.xword	.LC943
	.align	3
.LC946:
	.xword	.LC945
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_StorePicInDpb
	.type	MVC_StorePicInDpb, %function
MVC_StorePicInDpb:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 11157504
	bl	_mcount
	add	x0, x19, 11116544
	strb	wzr, [x0, 1672]
	ldrb	w1, [x21, 1963]
	cmp	w1, 2
	cset	w1, eq
	strb	w1, [x0, 1673]
	ldrb	w0, [x21, 1960]
	cbz	w0, .L3971
	ldr	w0, [x19, 128]
	str	w0, [x19, 124]
	ldrb	w0, [x21, 1973]
	cbz	w0, .L3804
	ldr	x0, [x21, 2608]
	ldrb	w0, [x0, 1]
	cbz	w0, .L3805
	cbz	x19, .L3807
	mov	x0, x19
	bl	MVC_ClearAllSlice
	mov	x0, x19
	bl	MVC_InitDPB.part.15
	cbnz	w0, .L3807
.L3804:
	ldrb	w0, [x19, 8]
	cmp	w0, 2
	beq	.L3972
	ldr	x20, [x19, 224]
	ldr	w0, [x20, 12]
	cmp	w0, wzr
	ble	.L3973
.L3811:
	ldrb	w1, [x21, 1962]
	cbz	w1, .L3974
.L3812:
	cmp	w0, 2
	ldr	w0, [x21, 2624]
	beq	.L3975
.L3836:
	cbz	w0, .L3844
	ldr	w3, [x19, 112]
	ldr	w2, [x21, 2660]
	cmp	w3, w2
	bcs	.L3845
	ldrb	w0, [x21, 1962]
	cbz	w0, .L3976
.L3845:
	mov	x0, x19
	bl	MVC_Marking
	cbnz	w0, .L3977
	mov	x0, x19
	bl	MVC_UpdateReflist
	mov	x0, x19
	bl	MVC_UpdateLTReflist
	ldr	w1, [x21, 2688]
	mov	x0, x19
	bl	MVC_RemoveUnUsedFrameStore
.L3837:
	ldrb	w0, [x21, 1961]
	cmp	w0, 1
	beq	.L3848
	add	x20, x19, 11124736
	ldr	w0, [x20, 760]
.L3849:
	ldr	w1, [x21, 2624]
	cbz	w1, .L3858
	ldr	w4, [x20, 768]
	ldr	w5, [x20, 772]
	add	w1, w4, w5
	cmp	w1, w0
	bcs	.L3884
.L3858:
	mov	w23, -1
	mov	w22, w23
	b	.L3867
	.p2align 3
.L3861:
	ldr	w22, [x29, 72]
.L3862:
	ldr	w3, [x21, 2624]
	mov	w1, w22
	mov	x0, x19
	mov	w23, w2
	cbnz	w3, .L3863
	ldr	w3, [x21, 2628]
	ldr	w2, [x29, 68]
	cmp	w3, w2
	blt	.L3978
.L3863:
	bl	MVC_OutputFrmFromDPB
	cmn	w0, #1
	mov	w24, w0
	beq	.L3866
	ldr	w0, [x20, 760]
.L3867:
	ldr	w4, [x20, 764]
	add	x3, x29, 72
	add	x2, x29, 68
	mov	w1, -1
	cmp	w4, w0
	mov	x0, x19
	bcc	.L3860
	bl	MVC_GetMinPOC
	tbnz	w0, #31, .L3979
	ldr	w2, [x20, 764]
	cmp	w23, w2
	bne	.L3861
	ldr	w0, [x29, 72]
	cmp	w22, w0
	beq	.L3980
	mov	w22, w0
	b	.L3862
	.p2align 3
.L3974:
	ldrb	w2, [x21, 1963]
	ldr	x23, [x21, 2600]
	sub	w1, w2, #1
	uxtb	w1, w1
	cbnz	w2, .L3981
.L3813:
	cmp	w1, 1
	bls	.L3982
.L3955:
	cbnz	w2, .L3983
.L3818:
	cmp	w0, wzr
	ble	.L3888
	add	x24, x21, 1960
	mov	x22, x23
.L3821:
	cbz	w2, .L3823
	ldrb	w0, [x24, 1]
	cmp	w0, 1
	beq	.L3984
.L3823:
	ldr	w0, [x24, 700]
	str	w0, [x22, 44]
.L3824:
	mov	x1, x22
	mov	x0, x19
	bl	MVC_GetImagePara
	ldrsb	w1, [x22, 6]
	ldr	w0, [x19, 120]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L3985
	ldr	x0, [x24, 616]
	str	x0, [x20, 600]
	ldr	x0, [x24, 624]
	str	x0, [x20, 608]
	ldr	x0, [x24, 632]
	str	x0, [x20, 616]
	ldr	w0, [x22, 44]
	str	w0, [x20, 160]
	cbz	w0, .L3831
	ldr	w1, [x20, 108]
	and	w1, w1, 3
	cbz	w1, .L3986
.L3829:
	ldr	x1, [x19, 224]
	ldr	w1, [x1, 4]
	cmp	w1, w0
	bcc	.L3987
.L3831:
	ldrsb	w1, [x22, 6]
	mov	w2, 1
	ldr	w0, [x19, 120]
	bl	FSP_SetDisplay
	ldr	w0, [x19, 120]
	mov	x4, x20
	add	x3, x19, 912
	mov	x2, x19
	mov	w1, 15
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L3828
	ldrsb	w1, [x22, 6]
	mov	w2, 0
	ldr	w0, [x19, 120]
	bl	FSP_SetDisplay
.L3828:
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L3988
	str	wzr, [x20, 24]
.L3834:
	ldr	w0, [x19, 136]
	add	w0, w0, 2
	str	w0, [x19, 136]
.L3822:
	ldr	w0, [x19, 220]
	cmp	w0, 2
	beq	.L3835
.L3820:
	mov	w0, 1
	str	w0, [x22, 16]
	strb	w0, [x23, 8]
.L3835:
	ldr	x0, [x19, 224]
	mov	w1, 2
	str	w1, [x19, 220]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	ldr	w0, [x21, 2624]
	bne	.L3836
.L3975:
	cbz	w0, .L3837
	add	x20, x19, 11124736
	mov	x0, x19
	bl	MVC_SimpleSlideDPB
	ldr	w1, [x20, 760]
	cbz	w1, .L3838
	ldr	x0, [x20, 376]
	cbz	x0, .L3989
	add	x2, x20, 384
	mov	w0, 0
	b	.L3840
	.p2align 3
.L3841:
	ldr	x3, [x2], 8
	cbz	x3, .L3990
.L3840:
	add	w0, w0, 1
	cmp	w0, w1
	bne	.L3841
.L3838:
	add	x2, x21, 1960
	mov	x0, x19
	bl	MVC_InsertFrmInDPB
	cbnz	w0, .L3991
	ldr	w1, [x20, 764]
	mov	x0, x19
	add	w1, w1, 1
	str	w1, [x20, 764]
	bl	MVC_UpdateReflist
	mov	x0, x19
	bl	MVC_UpdateLTReflist
	ldr	x1, .LC950
	mov	w3, 3773
	mov	w2, 0
	mov	w0, 14
	bl	dprint_vfmw
	.p2align 2
.L3843:
	mov	w20, 0
.L3882:
	strb	wzr, [x21, 1960]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3844:
	ldrb	w0, [x21, 1961]
	cmp	w0, 1
	beq	.L3848
	add	x20, x19, 11124736
	ldr	w0, [x20, 760]
	b	.L3858
	.p2align 3
.L3848:
	ldr	x0, [x19, 816]
	ldr	x4, [x21, 2600]
	cmp	x0, xzr
	ccmp	x4, x0, 0, ne
	bne	.L3992
	mov	x0, x19
	bl	MVC_DirectOutput
	cmn	w0, #1
	mov	w20, w0
	beq	.L3993
.L3854:
	ldr	x1, .LC950
	mov	w3, 3825
	mov	w2, w20
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3810
	.p2align 3
.L3979:
	ldr	x1, .LC969
	mov	w2, 3875
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, -1
	mov	x0, x19
	bl	MVC_ClearDPB
.L3860:
	ldr	w0, [x21, 2624]
	cbz	w0, .L3872
	ldrb	w0, [x21, 1964]
	cbnz	w0, .L3872
	ldr	w0, [x20, 768]
	cbz	w0, .L3872
	mov	w1, 0
	b	.L3877
	.p2align 3
.L3874:
	ldr	w0, [x20, 768]
	cmp	w0, w1
	bls	.L3872
.L3877:
	add	x0, x19, x1, uxtw 3
	ldr	w3, [x21, 2616]
	add	x0, x0, 11124736
	add	w1, w1, 1
	ldr	x2, [x0, 504]
	ldr	w4, [x2, 20]
	cmp	w4, w3
	bne	.L3874
	ldr	w4, [x2, 56]
	ldr	w3, [x21, 2688]
	cmp	w4, w3
	bne	.L3874
	strb	wzr, [x2, 3]
	ldr	x2, [x0, 504]
	ldrb	w3, [x2, 5]
	cmp	w3, 1
	beq	.L3994
.L3876:
	ldr	w0, [x2, 48]
	ldr	w3, [x19, 52]
	cmp	w0, w3
	add	x0, x19, x0, uxtw 2
	beq	.L3874
	str	wzr, [x0, 148]
	str	w3, [x2, 48]
	b	.L3874
	.p2align 3
.L3872:
	ldr	w1, [x20, 760]
	cbz	w1, .L3871
	ldr	x0, [x20, 376]
	cbz	x0, .L3995
	add	x2, x20, 384
	mov	w0, 0
	b	.L3879
	.p2align 3
.L3880:
	ldr	x3, [x2], 8
	cbz	x3, .L3996
.L3879:
	add	w0, w0, 1
	cmp	w0, w1
	bne	.L3880
.L3871:
	add	x2, x21, 1960
	mov	x0, x19
	bl	MVC_InsertFrmInDPB
	cbnz	w0, .L3997
	ldr	w1, [x20, 764]
	mov	x0, x19
	add	w1, w1, 1
	str	w1, [x20, 764]
	bl	MVC_UpdateReflist
	mov	x0, x19
	bl	MVC_UpdateLTReflist
	b	.L3843
	.p2align 3
.L3973:
	ldr	w1, [x19, 220]
	cmp	w1, 2
	bne	.L3811
	b	.L3812
	.p2align 3
.L3981:
	ldrb	w3, [x23, 2]
	cmp	w3, wzr
	ccmp	w1, 1, 0, eq
	bhi	.L3812
	b	.L3813
	.p2align 3
.L3983:
	ldrb	w1, [x23, 2]
	cbz	w1, .L3812
	b	.L3818
	.p2align 3
.L3982:
	ldr	w1, [x19, 220]
	cbnz	w1, .L3815
	mov	w0, 1
	mov	x2, 736
	str	w0, [x19, 220]
	add	x1, x21, 1960
	add	x0, x21, 2696
	bl	memcpy
	ldr	w0, [x20, 12]
	b	.L3812
.L3992:
	add	x20, x19, 11124736
	ldr	w0, [x20, 760]
	cbz	w0, .L3851
	ldr	x1, [x20, 376]
	add	x3, x20, 384
	cmp	x4, x1
	ccmp	x1, xzr, 4, eq
	mov	w1, 0
	bne	.L3998
	.p2align 2
.L3855:
	add	w1, w1, 1
	cmp	w1, w0
	beq	.L3849
	ldr	x2, [x3], 8
	cmp	x4, x2
	ccmp	x2, xzr, 4, eq
	beq	.L3855
.L3852:
	add	x2, x21, 1960
	mov	x0, x19
	bl	MVC_InsertFrmInDPB
	cmn	w0, #1
	mov	w20, w0
	beq	.L3999
	mov	x0, x19
	bl	MVC_UpdateReflist
	mov	x0, x19
	bl	MVC_UpdateLTReflist
	ldr	x1, .LC950
	mov	w3, 3851
	mov	w2, 0
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3843
.L3815:
	cmp	w1, 1
	bne	.L3955
	cmp	w0, wzr
	ldr	x22, [x21, 3336]
	ble	.L3820
	add	x24, x21, 2696
	ldrb	w2, [x24, 3]
	b	.L3821
	.p2align 3
.L3866:
	ldr	x22, .LC961
	mov	w3, w0
	mov	w2, 3915
	mov	w0, 1
	mov	x1, x22
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3916
	mov	w2, w24
	mov	w0, 14
	bl	dprint_vfmw
.L3886:
	ldr	x1, [x21, 2600]
	mov	w2, 1
	ldr	w0, [x19, 120]
	mov	w20, -1
	ldrsb	w1, [x1, 6]
	bl	FSP_ClearLogicFs
	mov	w3, w20
	mov	x1, x22
	mov	w2, 3974
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, [x21, 2600]
	strb	wzr, [x0, 5]
	ldr	x0, [x21, 2600]
	strb	wzr, [x0, 2]
	mov	w0, w20
	strb	wzr, [x21, 1960]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L3996:
	mov	w1, w0
	b	.L3871
	.p2align 3
.L3805:
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbz	w0, .L3804
	ldr	x1, .LC954
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3602
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
	.p2align 3
.L3972:
	ldr	x1, .LC956
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_DirectOutput
	mov	w20, w0
	ldr	w1, [x19, 52]
	ldr	w0, [x21, 2684]
	cmp	w0, w1
	beq	.L3809
	add	x0, x19, x0, uxtw 2
	str	wzr, [x0, 148]
	str	w1, [x21, 2684]
.L3809:
	ldr	x1, .LC950
	mov	w3, 3621
	mov	w2, w20
	mov	w0, 14
	bl	dprint_vfmw
.L3810:
	cmn	w20, #1
	bne	.L3882
	ldr	x22, .LC961
	b	.L3886
	.p2align 3
.L3978:
	ldrb	w0, [x19, 8]
	cmp	w0, 2
	beq	.L4000
.L3864:
	mov	x0, x19
	bl	MVC_DirectOutput
	cmn	w0, #1
	mov	w20, w0
	beq	.L4001
.L3865:
	ldr	x1, .LC950
	mov	w3, 3907
	mov	w2, w20
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3810
	.p2align 3
.L3807:
	ldr	x1, .LC952
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3592
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
.L3888:
	mov	x22, x23
	b	.L3822
.L3994:
	strb	w3, [x2, 7]
	ldr	x2, [x0, 504]
	strb	wzr, [x2, 5]
	ldr	x2, [x0, 504]
	b	.L3876
.L3980:
	ldr	x2, .LC970
	mov	w3, w22
	ldr	x1, .LC972
	mov	w0, 1
	add	x2, x2, 368
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3884
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
.L3988:
	mov	x1, x20
	mov	x0, x19
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3834
.L4000:
	ldrb	w0, [x21, 1963]
	cbz	w0, .L3864
	ldr	x0, [x21, 2600]
	str	x0, [x19, 824]
	b	.L3864
.L3990:
	mov	w1, w0
	b	.L3838
.L3986:
	ldr	x1, .LC958
	mov	w2, 3702
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC959
	ldr	x4, [x0]
	cbz	x4, .L3830
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 111
	blr	x4
.L3830:
	ldr	w0, [x20, 160]
	cbnz	w0, .L3829
	b	.L3831
	.p2align 3
.L3984:
	mov	w0, 3
	strb	w0, [x22, 2]
	ldr	w1, [x22, 44]
	ldr	w0, [x24, 700]
	add	w0, w1, w0
	str	w0, [x22, 44]
	b	.L3824
.L3971:
	ldr	x1, .LC948
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3578
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
.L4001:
	ldr	x1, .LC961
	mov	w3, w0
	mov	w2, 3904
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3865
.L3977:
	ldr	x1, .LC965
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3798
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
.L3995:
	mov	w1, 0
	b	.L3871
.L3997:
	ldr	x22, .LC961
	mov	w3, w0
	mov	w2, 3960
	mov	w0, 1
	mov	x1, x22
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3961
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3886
.L3851:
	ldr	w1, [x21, 2624]
	cbz	w1, .L3858
	ldr	w4, [x20, 768]
	ldr	w5, [x20, 772]
.L3884:
	ldr	x1, .LC967
	mov	w3, w0
	mov	w2, 3864
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	mov	x0, x19
	bl	MVC_ClearDPB
	ldr	x1, .LC950
	mov	w3, 3867
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
.L3976:
	ldr	x1, .LC963
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC959
	ldr	x4, [x0]
	cbz	x4, .L3846
	ldr	w1, [x19, 112]
	mov	w3, 8
	ldr	w2, [x21, 2660]
	ldr	w0, [x19, 120]
	stp	w2, w1, [x29, 72]
	add	x2, x29, 72
	mov	w1, 104
	blr	x4
.L3846:
	mov	x0, x19
	mov	w20, -1
	bl	MVC_ClearAllSlice
	add	x3, x19, 11120640
	mov	w2, 23352
	movk	w2, 0x1, lsl 16
	ldr	w4, [x19, 40]
	mov	w0, 2
	mov	w1, w20
	str	w2, [x3, 384]
	add	x2, x3, 256
	strb	w0, [x3, 369]
	mov	w0, 32
	str	w4, [x3, 380]
	strb	w0, [x3, 370]
	mov	w0, 3
	strb	wzr, [x3, 368]
	stp	w0, w20, [x2, 136]
	stp	wzr, wzr, [x2, 144]
	mov	w2, 7
	mov	x0, x19
	strb	w2, [x3, 371]
	mov	w2, 262144
	str	w2, [x3, 408]
	bl	MVC_ClearDPB
	ldr	x1, .LC950
	mov	w3, 3789
	mov	w2, w20
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
.L3993:
	ldr	x1, .LC961
	mov	w3, w0
	mov	w2, 3821
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3854
.L3989:
	mov	w1, 0
	b	.L3838
.L3998:
	mov	w1, 0
	b	.L3852
.L3991:
	ldr	x22, .LC961
	mov	w3, w0
	mov	w2, 3764
	mov	w0, 0
	mov	x1, x22
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3765
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3886
.L3999:
	ldr	x22, .LC961
	mov	w3, w0
	mov	w2, 3844
	mov	w0, 1
	mov	x1, x22
	bl	dprint_vfmw
	ldr	x1, .LC950
	mov	w3, 3845
	mov	w2, w20
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3886
.L3987:
	ldrsb	w1, [x22, 6]
	mov	w2, 0
	ldr	w0, [x19, 120]
	bl	FSP_SetDisplay
	ldr	x1, .LC950
	mov	w3, 3721
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
.L3985:
	ldr	x1, .LC950
	mov	w3, 3691
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x22, .LC961
	b	.L3886
	.size	MVC_StorePicInDpb, .-MVC_StorePicInDpb
	.align	3
.LC948:
	.xword	.LC947
	.align	3
.LC950:
	.xword	.LC949
	.align	3
.LC952:
	.xword	.LC951
	.align	3
.LC954:
	.xword	.LC953
	.align	3
.LC956:
	.xword	.LC955
	.align	3
.LC958:
	.xword	.LC957
	.align	3
.LC959:
	.xword	g_event_report
	.align	3
.LC961:
	.xword	.LC960
	.align	3
.LC963:
	.xword	.LC962
	.align	3
.LC965:
	.xword	.LC964
	.align	3
.LC967:
	.xword	.LC966
	.align	3
.LC969:
	.xword	.LC968
	.align	3
.LC970:
	.xword	.LANCHOR0
	.align	3
.LC972:
	.xword	.LC971
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecGap
	.type	MVC_DecGap, %function
MVC_DecGap:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x22, x19, 11116544
	add	x23, x19, 11120640
	bl	_mcount
	add	x27, x19, 11157504
	ldrb	w2, [x19, 3]
	mov	w26, 2
	ldr	w0, [x22, 1712]
	str	w0, [x29, 108]
	lsl	x0, x2, 6
	sub	x0, x0, x2
	ldr	x3, [x19, 248]
	lsl	x0, x0, 2
	ldr	w1, [x23, 356]
	sub	x0, x0, x2
	ldr	w24, [x22, 2836]
	lsl	x0, x0, 1
	add	x1, x1, 2777088
	sub	x0, x0, x2
	ldrb	w2, [x3, 19]
	lsl	x0, x0, 2
	add	x1, x19, x1, lsl 2
	cmp	w2, 1
	add	x2, x19, x0
	add	x2, x2, 11120640
	add	x0, x0, 11120640
	ldr	w5, [x1, 11020]
	add	x0, x0, 416
	add	x0, x19, x0
	ldr	w21, [x3, 2896]
	ldrb	w1, [x2, 419]
	csel	w25, w26, wzr, eq
	ldr	w2, [x22, 1716]
	stp	w1, w2, [x29, 100]
	add	x1, x19, 11124736
	str	x0, [x27, 2608]
	ldr	w0, [x1, 764]
	cbz	w0, .L4018
	add	w21, w21, 4
	mov	w20, 1
	add	w2, w5, 1
	lsl	w21, w20, w21
	ldr	x1, .LC976
	mov	w0, w26
	udiv	w5, w2, w21
	str	wzr, [x22, 1712]
	str	wzr, [x22, 1716]
	msub	w28, w5, w21, w2
	bl	dprint_vfmw
	ldr	x1, .LC978
	mov	w3, w28
	mov	w2, w24
	mov	w0, w26
	bl	dprint_vfmw
	cmp	w24, w28
	ble	.L4011
	ldr	x26, .LC982
	b	.L4015
	.p2align 3
.L4008:
	strb	wzr, [x27, 1964]
	mov	x0, x19
	bl	MVC_StorePicInDpb
	add	w1, w28, 1
	cbnz	w0, .L4019
.L4009:
	sdiv	w2, w1, w21
	ldr	w0, [x23, 356]
	add	x0, x0, 2777088
	add	x0, x19, x0, lsl 2
	str	w28, [x0, 11020]
	msub	w28, w2, w21, w1
	cmp	w24, w28
	ble	.L4011
.L4015:
	mov	w1, 1
	mov	x0, x19
	bl	MVC_AllocFrameStore
	cbnz	w0, .L4020
	mov	x1, x26
	mov	w0, 2
	bl	dprint_vfmw
	strb	w20, [x27, 1966]
	ldr	x0, [x27, 2600]
	str	w28, [x0, 20]
	str	w28, [x27, 2616]
	ldr	x0, [x27, 2600]
	str	w28, [x0, 716]
	str	w25, [x27, 2624]
	ldr	x0, [x27, 2600]
	str	wzr, [x0, 712]
	ldr	x0, [x27, 2600]
	strb	w20, [x0, 7]
	ldr	x0, [x27, 2600]
	strb	wzr, [x0, 5]
	ldr	x0, [x27, 2600]
	strb	w20, [x0, 1]
	strb	w20, [x27, 1962]
	ldr	x0, [x27, 2608]
	ldr	w1, [x23, 352]
	str	w1, [x27, 2688]
	strb	wzr, [x0, 3]
	ldr	x0, [x19, 248]
	ldr	w0, [x0, 2900]
	cbz	w0, .L4008
	mov	x0, x19
	str	w28, [x22, 1728]
	bl	MVC_DecPOC
	ldr	w0, [x22, 1708]
	str	w0, [x27, 2628]
	ldr	w0, [x22, 1704]
	str	w0, [x27, 2632]
	ldr	w0, [x22, 1696]
	str	w0, [x27, 2636]
	ldr	w0, [x22, 1700]
	str	w0, [x27, 2640]
	mov	x0, x19
	strb	wzr, [x27, 1964]
	bl	MVC_StorePicInDpb
	add	w1, w28, 1
	cbz	w0, .L4009
.L4019:
	ldr	w1, [x29, 108]
	mov	w3, w0
	str	w1, [x22, 1712]
	mov	w2, 6891
	ldr	w1, [x29, 104]
	mov	w0, 0
	str	w1, [x22, 1716]
	ldrb	w5, [x29, 100]
	ldr	x4, [x27, 2608]
	ldr	x1, .LC984
	strb	w5, [x4, 3]
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L4018:
	ldr	x1, .LC974
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L4011:
	ldr	w0, [x29, 108]
	str	w0, [x22, 1712]
	ldr	w0, [x29, 104]
	str	w0, [x22, 1716]
	mov	w0, 0
	ldrb	w2, [x29, 100]
	ldr	x1, [x27, 2608]
	strb	w2, [x1, 3]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L4020:
	ldr	w0, [x29, 108]
	str	w0, [x22, 1712]
	ldr	w0, [x29, 104]
	str	w0, [x22, 1716]
	mov	w0, 0
	ldrb	w3, [x29, 100]
	ldr	x2, [x27, 2608]
	ldr	x1, .LC980
	strb	w3, [x2, 3]
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.size	MVC_DecGap, .-MVC_DecGap
	.align	3
.LC974:
	.xword	.LC973
	.align	3
.LC976:
	.xword	.LC975
	.align	3
.LC978:
	.xword	.LC977
	.align	3
.LC980:
	.xword	.LC979
	.align	3
.LC982:
	.xword	.LC981
	.align	3
.LC984:
	.xword	.LC983
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_InitPic
	.type	MVC_InitPic, %function
MVC_InitPic:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 11116544
	add	x20, x19, 11157504
	bl	_mcount
	mov	w22, 2240
	ldr	x2, [x19, 280]
	ldr	w0, [x21, 2824]
	ldrb	w1, [x20, 1960]
	str	wzr, [x29, 92]
	umull	x0, w0, w22
	add	x22, x2, x0
	cbz	w1, .L4022
	ldr	x0, [x20, 2600]
	cbz	x0, .L4023
	ldrb	w1, [x0, 2]
	cbz	w1, .L4184
.L4023:
	strb	wzr, [x20, 1960]
.L4022:
	add	x1, x29, 92
	mov	x0, x19
	bl	MVC_GetReRangeFlag
	mov	w23, w0
	ldrsb	w0, [x21, 2820]
	cmn	w0, #1
	beq	.L4185
.L4024:
	cbnz	w0, .L4027
	ldrb	w0, [x19, 2]
	cmp	w0, 1
	beq	.L4186
	ldr	w1, [x19, 28]
	ldr	w0, [x22, 28]
	cmp	w1, w0
	bne	.L4187
	add	x0, x19, 11046912
	add	x0, x0, 180
	str	x0, [x19, 248]
.L4027:
	add	x24, x19, 11112448
	ldr	w0, [x22, 24]
	ldr	w1, [x24, 3552]
	cmp	w1, w0
	beq	.L4188
.L4031:
	strb	wzr, [x22, 20]
	mov	x1, x22
	ldr	x0, [x19, 248]
	bl	mvc_assign_quant_params
	ldr	x3, .LC985
	mov	x2, 2240
	mov	x1, x22
	add	x0, x24, 3528
	ldr	x3, [x3, 112]
	blr	x3
.L4032:
	ldr	w0, [x29, 92]
	cbnz	w0, .L4189
	ldr	x0, [x19, 224]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L4034
.L4044:
	ldr	w0, [x22, 24]
	ldr	w1, [x24, 3552]
	cmp	w1, w0
	bne	.L4054
	ldrb	w0, [x22, 20]
	cbnz	w0, .L4054
	ldrsb	w0, [x21, 2820]
	cmn	w0, #1
	beq	.L4190
.L4056:
	cbnz	w0, .L4057
	add	x0, x19, 10768384
	add	x23, x19, 11120640
	add	x5, x0, 4068
	ldr	w2, [x0, 4064]
	ldr	w3, [x23, 352]
	adds	w2, w2, 1
	beq	.L4058
	ldr	w0, [x0, 4068]
	cmp	w3, w0
	beq	.L4191
	mov	w4, w2
	mov	w0, 0
	b	.L4060
	.p2align 3
.L4061:
	ldr	w1, [x5, w0, sxtw 2]
	cmp	w3, w1
	beq	.L4058
.L4060:
	add	w0, w0, 1
	cmp	w0, w4
	mov	w2, w0
	bne	.L4061
.L4058:
	str	w2, [x23, 356]
.L4057:
	ldrb	w1, [x21, 2811]
	ldr	x0, [x19, 248]
	cmp	w1, 5
	ldr	w2, [x0, 2896]
	beq	.L4062
.L4199:
	ldrb	w0, [x19, 8]
	ldr	w1, [x21, 2836]
	cmp	w0, 2
	beq	.L4064
.L4200:
	ldr	x3, [x19, 224]
	ldr	w0, [x3, 12]
	cmp	w0, 2
	beq	.L4064
	add	x23, x19, 11120640
	ldr	w0, [x23, 356]
	add	x0, x0, 2777088
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 11020]
	cmp	w0, w1
	beq	.L4064
	add	w2, w2, 4
	mov	w4, 1
	lsl	w25, w4, w2
	add	w0, w0, w4
	udiv	w2, w0, w25
	msub	w0, w2, w25, w0
	cmp	w0, w1
	beq	.L4064
	str	w4, [x3, 600]
	ldr	x0, [x19, 224]
	ldr	w1, [x0, 600]
	cbnz	w1, .L4065
	ldr	w1, [x0, 640]
	cmp	w1, w4
	beq	.L4192
.L4066:
	ldr	w0, [x23, 356]
	ldr	w2, [x21, 2836]
	add	x0, x19, x0, lsl 2
	add	x0, x0, 11116544
	ldr	w1, [x0, 2828]
	cmp	w2, w1
	sub	w3, w2, w1
	bls	.L4067
	ldr	w4, [x24, 3508]
	cmp	w4, w3
	ccmp	w1, wzr, 4, cc
	beq	.L4065
	ldrb	w1, [x21, 2808]
	cmp	w1, 2
	bne	.L4193
	cbnz	w2, .L4069
	sub	w2, w25, #1
.L4070:
	str	w2, [x0, 2828]
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbz	w0, .L4065
	ldr	x1, .LC988
	mov	w3, w0
	mov	w2, 7363
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
	.p2align 3
.L4189:
	ldr	x0, [x19, 224]
	ldr	w1, [x0, 708]
	cbnz	w1, .L4034
	ldr	w1, [x0, 28]
	cmp	w1, 25
	beq	.L4194
.L4034:
	ldr	w1, [x19, 84]
	add	w1, w1, 1
	str	w1, [x19, 84]
	cbnz	w23, .L4045
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L4045
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbnz	w0, .L4195
	cbz	x19, .L4095
.L4203:
	mov	x0, x19
	bl	MVC_ClearAllSlice
	mov	x0, x19
	bl	MVC_InitDPB.part.15
	cbnz	w0, .L4048
	cbnz	w23, .L4050
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 872]
	cmp	w0, 1
	bne	.L4044
.L4050:
	ldr	w4, [x24, 3508]
	add	x2, x19, 11108352
	mov	w1, 2
	add	x3, x19, 48
	lsl	w4, w4, 1
	str	w4, [x19, 52]
	ldr	w0, [x24, 3488]
	ldrb	w23, [x2, 3652]
	add	w0, w0, 1
	ldr	w25, [x24, 3484]
	sub	w23, w1, w23
	lsl	w0, w0, 4
	add	w25, w25, 1
	mul	w23, w0, w23
	lsl	w25, w25, 4
	mov	x0, x19
	mov	w2, w23
	mov	w1, w25
	bl	MVC_ArrangeVahbMem
	cmp	w0, 1
	beq	.L4196
.L4180:
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L4197
	ldr	x1, .LC996
	mov	w0, 0
	stp	wzr, wzr, [x19, 12]
	bl	dprint_vfmw
	mov	w1, 0
	mov	x0, x19
	bl	MVC_ClearAll
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L4185:
	mov	w1, 1
	strb	w1, [x19, 2]
	mov	w1, 3992
	ldr	x4, [x19, 272]
	ldr	w2, [x22, 28]
	add	x3, x19, 11112448
	str	w2, [x19, 24]
	ldr	w2, [x22, 28]
	ldr	w3, [x3, 280]
	umull	x1, w2, w1
	add	x1, x4, x1
	ldr	w2, [x1, 744]
	cmp	w3, w2
	beq	.L4198
.L4025:
	ldr	w0, [x19, 84]
	add	x24, x19, 11108352
	ldr	x3, .LC985
	add	x24, x24, 3632
	add	w0, w0, 1
	str	w0, [x19, 84]
	strb	wzr, [x1, 26]
	mov	x2, 3992
	mov	x0, x24
	ldr	x3, [x3, 112]
	blr	x3
	ldrsb	w0, [x21, 2820]
	str	x24, [x19, 248]
	b	.L4024
	.p2align 3
.L4054:
	ldr	x3, .LC985
	mov	x2, 2240
	strb	wzr, [x22, 20]
	mov	x1, x22
	add	x0, x24, 3528
	ldr	x3, [x3, 112]
	blr	x3
	ldrsb	w0, [x21, 2820]
	cmn	w0, #1
	bne	.L4056
.L4190:
	add	x23, x19, 11120640
	str	wzr, [x23, 356]
	ldrb	w1, [x21, 2811]
	ldr	x0, [x19, 248]
	cmp	w1, 5
	ldr	w2, [x0, 2896]
	bne	.L4199
.L4062:
	add	x23, x19, 11120640
	ldr	w1, [x21, 2836]
	ldr	w0, [x23, 356]
	add	x0, x0, 2777088
	add	x0, x19, x0, lsl 2
	str	w1, [x0, 11020]
	ldrb	w0, [x19, 8]
	cmp	w0, 2
	bne	.L4200
	.p2align 2
.L4064:
	ldrb	w0, [x21, 2816]
	cbz	w0, .L4078
	add	x23, x19, 11120640
	ldr	w0, [x23, 356]
	add	x0, x0, 2777088
	add	x0, x19, x0, lsl 2
	str	w1, [x0, 11020]
.L4078:
	mov	x0, x19
	str	w1, [x21, 1728]
	bl	MVC_DecPOC
	mov	w1, 0
	mov	x0, x19
	bl	MVC_AllocFrameStore
	cbnz	w0, .L4201
	strb	wzr, [x20, 1967]
	mov	w0, 1
	strb	w0, [x20, 1960]
	add	x4, x20, 2560
	mov	w6, 1
	ldrb	w0, [x21, 2811]
	strb	w0, [x20, 1966]
	ldr	w8, [x21, 2836]
	str	w8, [x20, 2616]
	ldrb	w7, [x21, 2816]
	str	w7, [x20, 2624]
	ldr	w3, [x21, 1708]
	str	w3, [x20, 2628]
	ldr	w2, [x21, 1704]
	str	w2, [x20, 2632]
	ldr	w1, [x21, 1696]
	str	w1, [x20, 2636]
	ldr	w0, [x21, 1700]
	strb	wzr, [x20, 1962]
	strb	wzr, [x20, 1965]
	strb	wzr, [x20, 1964]
	stp	w0, w3, [x4, 80]
	stp	w2, w1, [x4, 88]
	str	w0, [x20, 2656]
	mov	w0, -1
	str	w0, [x20, 2680]
	ldrb	w5, [x21, 2809]
	cbz	w5, .L4080
	strb	w6, [x21, 2809]
	mov	w5, w6
	mov	w6, 2
.L4080:
	ldr	x4, [x19, 248]
	mov	w3, 2
	ldrb	w2, [x4, 20]
	ldr	w0, [x4, 3952]
	sub	w1, w3, w2
	mov	w2, 0
	madd	w1, w0, w1, w1
	udiv	w0, w1, w6
	str	w0, [x20, 2668]
	ldrb	w1, [x4, 20]
	sub	w1, w3, w1
	ldr	w3, [x4, 3952]
	madd	w1, w3, w1, w1
	str	w1, [x20, 2672]
	ldr	w1, [x4, 3948]
	add	w1, w1, 1
	str	w1, [x20, 2664]
	mul	w0, w0, w1
	str	w0, [x20, 2676]
	ldrb	w0, [x24, 3528]
	strb	w0, [x20, 1968]
	cbnz	w5, .L4081
	ldrb	w0, [x4, 21]
	cmp	w0, wzr
	cset	w2, ne
.L4081:
	strb	w2, [x20, 1969]
	add	x23, x19, 11120640
	cbz	w7, .L4082
	ldr	w0, [x23, 356]
	add	x0, x0, 2777088
	add	x0, x19, x0, lsl 2
	str	w8, [x0, 11020]
.L4082:
	ldr	x0, [x20, 2600]
	cbz	x0, .L4083
	ldrsb	w1, [x21, 2820]
	strb	w1, [x0, 9]
.L4083:
	mov	w0, -1
	str	w0, [x20, 2688]
	ldrb	w0, [x21, 2819]
	cbz	w0, .L4084
	ldr	w0, [x23, 352]
	str	w0, [x20, 2688]
	ldrb	w0, [x21, 2817]
	strb	w0, [x20, 1970]
	ldrb	w0, [x21, 2818]
	strb	w0, [x20, 1971]
.L4084:
	ldr	w0, [x23, 356]
	mov	x22, -1
	str	w0, [x20, 2692]
	mov	w0, 29
	ldr	x1, .LC1010
	ldrb	w3, [x19, 3]
	lsl	x2, x3, 6
	sub	x2, x2, x3
	lsl	x2, x2, 2
	sub	x2, x2, x3
	lsl	x2, x2, 1
	sub	x2, x2, x3
	lsl	x2, x2, 2
	add	x2, x2, 11120640
	add	x2, x2, 416
	add	x2, x19, x2
	str	x2, [x20, 2608]
	ldr	x2, [x19, 224]
	ldr	x3, [x2, 56]
	str	x3, [x20, 2576]
	ldr	x3, [x2, 72]
	str	x3, [x20, 2584]
	ldr	x3, [x2, 88]
	str	x3, [x20, 2592]
	str	x22, [x2, 56]
	ldr	x2, [x20, 2576]
	bl	dprint_vfmw
	ldr	x1, .LC1012
	mov	w0, 29
	ldr	x2, [x20, 2584]
	bl	dprint_vfmw
	ldr	x3, [x19, 248]
	mov	w2, 2
	add	x4, x19, 11108352
	mov	w5, 8864
	ldr	w0, [x3, 3948]
	add	w0, w0, 1
	lsl	w0, w0, 4
	str	w0, [x20, 2088]
	ldrb	w0, [x3, 20]
	ldr	w1, [x3, 3952]
	sub	w0, w2, w0
	madd	w1, w1, w0, w0
	add	x0, x20, 1976
	lsl	w1, w1, 4
	str	w1, [x20, 2092]
	ldr	w1, [x4, 3684]
	str	w5, [x20, 2084]
	and	w1, w1, 7
	ldr	w2, [x3, 748]
	ldrb	w4, [x3, 52]
	cmp	w2, 1
	ldr	w2, [x3, 56]
	cset	w3, ne
	ubfiz	w2, w2, 5, 3
	orr	w2, w2, w4, lsl 14
	orr	w2, w2, w3, lsl 2
	str	w2, [x20, 2084]
	bl	SetAspectRatio
	ldr	w0, [x19, 84]
	ldr	x1, [x20, 2600]
	str	w0, [x20, 2140]
	cbz	x1, .L4202
	ldr	x0, [x19, 248]
	ldr	w0, [x0, 3984]
	str	w0, [x1, 200]
	str	w0, [x20, 2104]
	ldr	x1, [x20, 2600]
	ldr	x0, [x19, 248]
	ldr	w0, [x0, 3988]
	str	w0, [x1, 204]
	str	w0, [x20, 2108]
	ldr	x1, [x20, 2600]
	ldr	x0, [x19, 248]
	ldr	w0, [x0, 3976]
	str	w0, [x1, 192]
	str	w0, [x20, 2096]
	ldr	x1, [x20, 2600]
	ldr	x0, [x19, 248]
	ldr	w0, [x0, 3980]
	str	w0, [x1, 196]
	str	w0, [x20, 2100]
	ldrb	w2, [x20, 1963]
	ldr	x0, [x20, 2600]
	cmp	w2, wzr
	ldr	w1, [x20, 2084]
	cset	w2, ne
	str	w2, [x20, 2072]
	str	w1, [x0, 180]
	ldr	x0, [x20, 2600]
	ldr	w1, [x20, 2072]
	str	w1, [x0, 168]
	ldr	w0, [x21, 2836]
	str	w0, [x20, 2160]
	ldr	x1, [x20, 2600]
	ldr	w2, [x23, 352]
	str	w2, [x20, 2168]
	str	w0, [x1, 256]
	ldr	x0, [x20, 2600]
	ldr	w1, [x23, 352]
	str	w1, [x0, 264]
	ldr	x1, [x20, 2600]
	ldr	w0, [x19, 120]
	ldrsb	w1, [x1, 6]
	bl	FSP_GetDispPhyFs
	cbz	x0, .L4086
	ldr	w3, [x20, 2072]
	mov	w1, 0
	ldr	w2, [x0, 8]
	cmp	w3, wzr
	ldr	w0, [x19, 120]
	cset	w3, ne
	bl	FSP_SetStoreType
.L4086:
	ldr	w1, [x19, 60]
	add	x4, x20, 2536
	add	x5, x20, 2568
.L4090:
	ldr	x0, [x4]
	cbnz	x0, .L4087
	cbz	w1, .L4087
	ldr	x0, [x19, 832]
	subs	w1, w1, #1
	str	x0, [x4]
	beq	.L4088
	add	x2, x19, 832
	mov	w0, 0
	.p2align 2
.L4089:
	add	w0, w0, 1
	cmp	w0, w1
	add	x3, x19, x0, uxtw 3
	ldr	x3, [x3, 832]
	str	x3, [x2], 8
	bne	.L4089
.L4088:
	add	x0, x19, x1, uxtw 3
	str	xzr, [x0, 832]
	str	w1, [x19, 60]
.L4087:
	add	x4, x4, 8
	cmp	x5, x4
	bne	.L4090
	cbz	w1, .L4092
	cmp	w1, wzr
	ble	.L4092
	.p2align 2
.L4093:
	add	x22, x19, x1, sxtw 3
	sub	w21, w1, #1
	ldr	w0, [x19, 120]
	ldr	x1, [x22, 824]
	bl	FreeUsdByDec
	str	xzr, [x22, 824]
	mov	w1, w21
	cbnz	w21, .L4093
.L4092:
	str	wzr, [x19, 60]
	mov	x0, x19
	strb	wzr, [x20, 1972]
	bl	MVC_UpdateReflist
	mov	x0, x19
	bl	MVC_UpdateLTReflist
	mov	x0, x19
	bl	MVC_CalcPicNum
	mov	w0, 0
.L4030:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
.L4192:
	ldr	w1, [x0, 636]
	cbnz	w1, .L4066
	ldr	w1, [x0, 644]
	cbnz	w1, .L4066
	ldr	w0, [x0, 604]
	add	w0, w0, 1024
	cmp	w0, 2048
	bls	.L4066
	.p2align 2
.L4065:
	add	x0, x19, 11108352
	ldrb	w0, [x0, 3651]
	cbnz	w0, .L4074
	ldr	x1, .LC1002
	mov	w0, 2
	bl	dprint_vfmw
	ldr	x0, [x19, 224]
	ldr	w0, [x0, 604]
	tst	w0, -1025
	bne	.L4181
	ldr	w0, [x23, 352]
	cbnz	w0, .L4181
	ldr	w0, [x23, 356]
	ldr	w1, [x21, 2836]
	add	x0, x0, 2777088
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 11020]
	add	w0, w0, 1
	udiv	w2, w0, w25
	msub	w0, w2, w25, w0
	cmp	w1, w0
	bcs	.L4064
	ldrb	w0, [x21, 2808]
	cmp	w0, 2
	beq	.L4064
	mov	x0, x19
	mov	w22, -1
	bl	MVC_ClearAllSlice
	add	x5, x23, 256
	ldr	w0, [x19, 40]
	mov	w2, 2
	str	w0, [x23, 380]
	strb	w2, [x23, 369]
	mov	w0, 23352
	strb	wzr, [x23, 368]
	mov	w2, 32
	strb	w2, [x23, 370]
	movk	w0, 0x1, lsl 16
	stp	w22, wzr, [x5, 140]
	mov	w2, 262144
	mov	w1, w22
	stp	wzr, w2, [x5, 148]
	str	w0, [x23, 384]
	mov	w0, 7
	strb	w0, [x23, 371]
	mov	w0, 3
	str	w0, [x23, 392]
	mov	x0, x19
	bl	MVC_ClearDPB
	ldr	w4, [x23, 356]
	mov	w2, 7417
	ldr	x1, .LC1004
	mov	w0, 0
	add	x4, x4, 2777088
	ldr	w3, [x21, 2836]
	add	x19, x19, x4, lsl 2
	ldr	w4, [x19, 11020]
	add	w4, w4, 1
	udiv	w5, w4, w25
	msub	w4, w5, w25, w4
	bl	dprint_vfmw
	mov	w0, w22
	b	.L4030
	.p2align 3
.L4188:
	ldrb	w0, [x22, 20]
	cbz	w0, .L4032
	b	.L4031
	.p2align 3
.L4045:
	ldr	x1, .LC998
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_GetBackPicFromVOQueue
	cbnz	x19, .L4203
.L4095:
	mov	w0, -1
.L4048:
	ldr	x1, .LC990
	mov	w3, w0
	mov	w2, 7252
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
	.p2align 3
.L4198:
	ldrb	w2, [x1, 26]
	cbnz	w2, .L4025
	add	x24, x19, 11108352
	add	x24, x24, 3632
	str	x24, [x19, 248]
	b	.L4024
	.p2align 3
.L4184:
	ldrsb	w1, [x0, 6]
	mov	w2, 1
	ldr	w0, [x19, 120]
	bl	FSP_ClearLogicFs
	ldr	x0, [x20, 2600]
	strb	wzr, [x0, 2]
	ldr	x0, [x20, 2600]
	strb	wzr, [x0, 5]
	b	.L4023
	.p2align 3
.L4197:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	mov	w0, -2
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L4074:
	mov	x0, x19
	bl	MVC_DecGap
	cbnz	w0, .L4077
.L4181:
	ldr	w1, [x21, 2836]
	b	.L4064
.L4194:
	ldr	w0, [x19, 84]
	add	w0, w0, 1
	str	w0, [x19, 84]
	cbnz	w23, .L4204
.L4035:
	cbz	x19, .L4094
	mov	x0, x19
	bl	MVC_ClearAllSlice
	mov	x0, x19
	bl	MVC_InitDPB.part.15
	mov	w3, w0
	cbnz	w0, .L4036
	add	x2, x19, 11108352
	ldr	w1, [x24, 3488]
	ldr	w26, [x24, 3484]
	add	w1, w1, 1
	ldrb	w25, [x2, 3652]
	mov	w2, 2
	lsl	w1, w1, 4
	add	w26, w26, 1
	sub	w25, w2, w25
	lsl	w26, w26, 4
	mul	w25, w1, w25
	cbz	w23, .L4038
	ldr	w1, [x19, 12]
	cbnz	w1, .L4038
	ldr	w4, [x24, 3508]
	ldr	x3, [x19, 224]
	lsl	w4, w4, 1
	str	w4, [x19, 52]
	ldr	w1, [x3, 668]
	cmp	w26, w1
	bhi	.L4039
	ldr	w2, [x3, 672]
	cmp	w25, w2
	bhi	.L4039
	add	x5, x19, 11124736
	ldr	w6, [x3, 692]
	ldr	w5, [x5, 760]
	cmp	w6, w5
	bcc	.L4205
	ldr	w0, [x3, 696]
	cbnz	w0, .L4042
	mov	w4, 1
	str	w4, [x19, 52]
	ldr	w1, [x3, 668]
	ldr	w2, [x3, 672]
.L4042:
	add	x3, x19, 48
	mov	x0, x19
	bl	MVC_ArrangeVahbMem
	cmp	w0, 1
	bne	.L4180
.L4038:
	lsr	w26, w26, 4
	lsr	w25, w25, 4
	stp	w26, w25, [x19, 12]
	b	.L4044
.L4186:
	strb	wzr, [x19, 2]
	ldr	x0, .LC985
	ldr	w1, [x22, 28]
	ldr	x3, [x0, 112]
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 2
	sub	x0, x0, x1
	lsl	x0, x0, 4
	sub	x1, x0, x1
	add	x0, x19, 10768384
	lsl	x2, x1, 2
	add	x0, x0, 4060
	sub	x1, x2, x1
	lsl	x2, x1, 6
	add	x1, x1, x2
	mov	x2, 8500
	movk	x2, 0x5, lsl 16
	add	x1, x19, x1, lsl 2
	add	x1, x1, 12288
	add	x1, x1, 2396
	blr	x3
	add	x0, x19, 11046912
	add	x0, x0, 180
	str	x0, [x19, 248]
	ldr	w0, [x0, 744]
	str	w0, [x19, 28]
	b	.L4027
	.p2align 3
.L4196:
	lsr	w25, w25, 4
	lsr	w23, w23, 4
	stp	w25, w23, [x19, 12]
	b	.L4044
.L4067:
	ldr	w4, [x24, 3508]
	add	w3, w3, w25
	cmp	w4, w3
	ccmp	w1, wzr, 4, cc
	beq	.L4065
	ldrb	w1, [x21, 2808]
	cmp	w1, 2
	bne	.L4206
	cbnz	w2, .L4072
	sub	w2, w25, #1
.L4073:
	str	w2, [x0, 2828]
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbz	w0, .L4065
	ldr	x1, .LC988
	mov	w3, w0
	mov	w2, 7390
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
	.p2align 3
.L4094:
	mov	w3, -1
.L4036:
	ldr	x1, .LC990
	mov	w2, 7168
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
	.p2align 3
.L4204:
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbz	w0, .L4035
	ldr	x1, .LC988
	mov	w3, w0
	mov	w2, 7159
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
	.p2align 3
.L4195:
	ldr	x1, .LC988
	mov	w3, w0
	mov	w2, 7243
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
.L4077:
	ldr	x1, .LC1006
	mov	w2, 7444
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
	.p2align 3
.L4187:
	ldr	x1, .LC986
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
.L4191:
	mov	w2, 0
	b	.L4058
.L4201:
	ldr	x1, .LC1008
	mov	w2, 7464
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
.L4202:
	ldr	x1, .LC1014
	mov	w2, 7572
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, w22
	b	.L4030
.L4072:
	sub	w2, w2, #1
	b	.L4073
.L4069:
	sub	w2, w2, #1
	b	.L4070
.L4206:
	mov	x0, x19
	mov	w22, -1
	bl	MVC_ClearAllSlice
	add	x6, x23, 256
	ldr	w0, [x19, 40]
	mov	w2, 2
	str	w0, [x23, 380]
	strb	w2, [x23, 369]
	mov	w0, 23352
	strb	wzr, [x23, 368]
	mov	w2, 32
	strb	w2, [x23, 370]
	movk	w0, 0x1, lsl 16
	stp	w22, wzr, [x6, 140]
	mov	w2, 262144
	mov	w1, w22
	stp	wzr, w2, [x6, 148]
	str	w0, [x23, 384]
	mov	w0, 7
	strb	w0, [x23, 371]
	mov	w0, 3
	str	w0, [x23, 392]
	mov	x0, x19
	bl	MVC_ClearDPB
	ldr	w5, [x23, 356]
	mov	w2, 7379
	ldr	x1, .LC1000
	ldr	w6, [x21, 2836]
	add	x5, x5, 2777088
	ldr	w4, [x24, 3508]
.L4182:
	add	x19, x19, x5, lsl 2
	mov	w0, 0
	ldr	w3, [x19, 11020]
	sub	w3, w6, w3
	bl	dprint_vfmw
	mov	w0, w22
	b	.L4030
.L4193:
	mov	x0, x19
	mov	w22, -1
	bl	MVC_ClearAllSlice
	add	x7, x23, 256
	ldr	w0, [x19, 40]
	mov	w2, 2
	str	w0, [x23, 380]
	strb	w2, [x23, 369]
	mov	w0, 23352
	strb	wzr, [x23, 368]
	mov	w2, 32
	strb	w2, [x23, 370]
	movk	w0, 0x1, lsl 16
	stp	w22, wzr, [x7, 140]
	mov	w2, 262144
	mov	w1, w22
	stp	wzr, w2, [x7, 148]
	str	w0, [x23, 384]
	mov	w0, 7
	strb	w0, [x23, 371]
	mov	w0, 3
	str	w0, [x23, 392]
	mov	x0, x19
	bl	MVC_ClearDPB
	ldr	w5, [x23, 356]
	mov	w2, 7352
	ldr	x1, .LC1000
	ldr	w6, [x21, 2836]
	add	x5, x5, 2777088
	ldr	w4, [x24, 3508]
	b	.L4182
.L4039:
	ldr	x1, .LC992
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
.L4205:
	ldr	x1, .LC994
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4030
	.size	MVC_InitPic, .-MVC_InitPic
	.align	3
.LC985:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC986:
	.xword	.LC88
	.align	3
.LC988:
	.xword	.LC987
	.align	3
.LC990:
	.xword	.LC989
	.align	3
.LC992:
	.xword	.LC991
	.align	3
.LC994:
	.xword	.LC993
	.align	3
.LC996:
	.xword	.LC995
	.align	3
.LC998:
	.xword	.LC997
	.align	3
.LC1000:
	.xword	.LC999
	.align	3
.LC1002:
	.xword	.LC1001
	.align	3
.LC1004:
	.xword	.LC1003
	.align	3
.LC1006:
	.xword	.LC1005
	.align	3
.LC1008:
	.xword	.LC1007
	.align	3
.LC1010:
	.xword	.LC1009
	.align	3
.LC1012:
	.xword	.LC1011
	.align	3
.LC1014:
	.xword	.LC1013
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecSlice
	.type	MVC_DecSlice, %function
MVC_DecSlice:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 11116544
	bl	_mcount
	ldr	w2, [x20, 68]
	mov	w0, 22
	ldr	x1, .LC1015
	sub	w2, w2, #1
	bl	dprint_vfmw
	ldr	x1, [x20, 240]
	mov	x0, x20
	ldrb	w2, [x1, 2]
	strb	w2, [x19, 2811]
	ldrb	w1, [x1, 4]
	strb	w1, [x19, 2816]
	bl	MVC_ProcessSliceHeaderFirstPart
	cbnz	w0, .L4253
	mov	x0, x20
	bl	MVC_ProcessSliceHeaderSecondPart
	cbnz	w0, .L4253
	ldr	w2, [x20, 64]
	mov	w3, 0
	add	x21, x20, 11157504
	cbz	w2, .L4214
	sub	w9, w2, #1
	add	x21, x20, 11157504
	add	x9, x9, 1392640
	add	x4, x21, 3464
	add	x9, x9, 2482
	mov	x0, 0
	mov	w10, 0
	mov	w3, 0
	add	x9, x20, x9, lsl 3
	.p2align 2
.L4222:
	ldr	x1, [x4]
	ldr	x6, [x1, 8]
	cbz	x6, .L4234
	ldr	x5, [x1, 40]
	ldr	w7, [x1, 16]
	cbz	x5, .L4235
.L4258:
	ldr	w1, [x1, 48]
.L4217:
	cbz	x0, .L4218
	cmp	x6, x0
	mov	w8, w7
	bls	.L4219
	sub	x0, x6, x0
	sub	w0, w0, w10
	add	w8, w0, w7
.L4219:
	add	w3, w1, w3
	add	w3, w8, w3
.L4220:
	cbz	x5, .L4237
	mov	x0, x5
	mov	w10, w1
.L4221:
	add	x4, x4, 8
	cmp	x9, x4
	bne	.L4222
.L4214:
	str	w3, [x20, 104]
	ldr	w0, [x21, 2664]
	ldr	w4, [x21, 2668]
	mul	w4, w0, w4
	cmp	w4, 1620
	bgt	.L4254
	lsl	w1, w4, 2
	mov	w5, 32769
	sub	w1, w1, w4
	mov	w4, 16384
	lsl	w1, w1, 8
	asr	w0, w1, 1
	cmp	w1, w5
	csel	w4, w0, w4, gt
.L4224:
	ldr	w0, [x20, 32]
	cmp	w0, w2
	bls	.L4225
	cmp	w4, w3
	bcc	.L4225
	ldrb	w0, [x19, 2813]
	cbz	w0, .L4228
	ldr	x1, .LC1023
	mov	w0, 2
	bl	dprint_vfmw
	str	wzr, [x20, 100]
	ldr	w0, [x20, 88]
	add	w0, w0, 1
	str	w0, [x20, 88]
	ldrb	w0, [x21, 1963]
	cbnz	w0, .L4229
	ldr	w0, [x20, 92]
	add	w0, w0, 1
	str	w0, [x20, 92]
.L4230:
	mov	x0, x20
	bl	MVC_InitPic
	cmn	w0, #2
	beq	.L4211
	cbnz	w0, .L4255
	mov	x0, x20
	bl	MVC_WritePicMsg
.L4228:
	mov	x0, x20
	bl	MVC_PicTypeStatistic
	mov	x0, x20
	bl	MVC_DecList
	cbnz	w0, .L4256
	mov	x0, x20
	bl	MVC_IsRefListWrong
	cbnz	w0, .L4257
	mov	x0, x20
	bl	MVC_CalcStreamBits
	mov	x0, x20
	bl	MVC_WriteSliceMsg
	mov	x0, x20
	bl	MVC_ExitSlice
	ldr	w1, [x20, 100]
	mov	w2, 1
	mov	w0, 0
	strb	w2, [x20, 4]
	add	w1, w1, 1
	str	w1, [x20, 100]
.L4211:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4237:
	mov	x0, x6
	mov	w10, w7
	b	.L4221
	.p2align 3
.L4218:
	add	w3, w7, w3
	add	w3, w3, w1
	b	.L4220
	.p2align 3
.L4234:
	ldr	x5, [x1, 40]
	mov	w7, 0
	cbnz	x5, .L4258
	.p2align 2
.L4235:
	mov	w1, 0
	b	.L4217
	.p2align 3
.L4254:
	lsl	w4, w4, 7
	b	.L4224
	.p2align 3
.L4229:
	ldr	w0, [x20, 96]
	add	w0, w0, 1
	str	w0, [x20, 96]
	b	.L4230
.L4253:
	ldr	x1, .LC1016
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC1017
	ldr	x4, [x0]
	cbz	x4, .L4251
	add	x2, x29, 64
	ldr	w0, [x20, 120]
	mov	w5, -1
	mov	w3, 4
	mov	w1, 100
	str	w5, [x2, -8]!
	blr	x4
.L4251:
	mov	x0, x20
	bl	MVC_ClearCurrSlice
	mov	w0, -1
	b	.L4211
.L4225:
	ldr	x1, .LC1019
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x20, 32]
	ldr	w2, [x20, 64]
	cmp	w2, w1
	bcc	.L4227
	ldr	x0, .LC1017
	ldr	x4, [x0]
	cbz	x4, .L4227
	ldr	w0, [x20, 120]
	mov	w3, 8
	stp	w2, w1, [x29, 56]
	add	x2, x29, 56
	mov	w1, 108
	blr	x4
.L4227:
	ldr	x1, .LC1021
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x20
	bl	MVC_ClearAllSlice
	add	x1, x20, 11120640
	mov	w4, 32
	ldr	w8, [x20, 40]
	mov	w7, 23352
	movk	w7, 0x1, lsl 16
	mov	w6, 2
	strb	w4, [x1, 370]
	add	x4, x1, 256
	strb	wzr, [x1, 368]
	mov	w5, 7
	strb	w6, [x1, 369]
	mov	w0, -1
	strb	w5, [x1, 371]
	mov	w3, 3
	str	w8, [x1, 380]
	mov	w2, 262144
	str	w7, [x1, 384]
	stp	w3, w0, [x4, 136]
	stp	wzr, wzr, [x4, 144]
	str	w2, [x1, 408]
	b	.L4211
.L4256:
	ldr	x1, .LC1027
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4211
.L4257:
	ldr	x1, .LC1029
	mov	w0, 1
	bl	dprint_vfmw
	b	.L4251
.L4255:
	ldr	x1, .LC1025
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4211
	.size	MVC_DecSlice, .-MVC_DecSlice
	.align	3
.LC1015:
	.xword	.LC873
	.align	3
.LC1016:
	.xword	.LC875
	.align	3
.LC1017:
	.xword	g_event_report
	.align	3
.LC1019:
	.xword	.LC1018
	.align	3
.LC1021:
	.xword	.LC1020
	.align	3
.LC1023:
	.xword	.LC1022
	.align	3
.LC1025:
	.xword	.LC1024
	.align	3
.LC1027:
	.xword	.LC1026
	.align	3
.LC1029:
	.xword	.LC1028
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DecOneNal
	.type	MVC_DecOneNal, %function
MVC_DecOneNal:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w21, w1
	bl	_mcount
	ldr	x3, [x19, 240]
	ldr	w0, [x3, 76]
	cbz	w0, .L4264
	ldr	w2, [x19, 104]
	mov	w0, 0
	.p2align 2
.L4263:
	ubfiz	x1, x0, 5, 32
	add	w0, w0, 1
	add	x1, x3, x1
	ldr	w1, [x1, 16]
	add	w2, w2, w1
	str	w2, [x19, 104]
	ldr	w1, [x3, 76]
	cmp	w1, w0
	bhi	.L4263
.L4264:
	add	x22, x19, 872
	strb	wzr, [x3]
	mov	w1, 32
	mov	x0, x22
	bl	BsGet
	mov	w20, w0
	and	w4, w0, 31
	ldr	x0, [x19, 240]
	ubfx	x3, x20, 5, 2
	ubfx	x1, x20, 7, 1
	mov	w2, -1
	strb	w4, [x0, 2]
	ldr	x0, [x19, 240]
	strb	w3, [x0, 4]
	ldr	x0, [x19, 240]
	str	w1, [x0, 84]
	ldr	x1, [x19, 240]
	ldr	w0, [x1, 72]
	add	w0, w0, 32
	str	w0, [x1, 72]
	strb	wzr, [x19, 10]
	ldr	x0, [x19, 240]
	strb	w2, [x0, 5]
	ldr	x0, [x19, 240]
	ldrb	w3, [x0, 2]
	cmp	w3, 14
	ccmp	w3, 20, 4, ne
	beq	.L4441
	ldr	x1, .LC1031
	mov	w0, 22
	ldr	w2, [x19, 68]
	bl	dprint_vfmw
	ldr	w0, [x19, 68]
	cmp	w0, 371
	beq	.L4442
.L4267:
	ldr	x1, [x19, 240]
	add	w0, w0, 1
	str	w0, [x19, 68]
	ldrb	w2, [x1, 2]
	sub	w0, w2, #1
	cmp	w0, 29
	bls	.L4443
.L4268:
	ldr	x1, .LC1073
	mov	w3, w20
	mov	w0, 22
	bl	dprint_vfmw
	cmp	w20, 256
	beq	.L4310
	ldr	x1, .LC1075
	mov	w0, 1
	bl	dprint_vfmw
.L4310:
	ldr	x1, [x19, 240]
	cbz	x1, .L4311
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L4311:
	ldr	x0, .LC1039
	ldr	x4, [x0]
	cbz	x4, .L4426
.L4430:
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L4426:
	mov	w0, 0
.L4295:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4443:
	ldr	x1, .LC1034
	ldrh	w0, [x1,w0,uxtw #1]
	adr	x1, .Lrtx4270
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx4270:
	.section	.rodata
	.align	0
	.align	2
.L4270:
	.2byte	(.L4269 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4269 - .Lrtx4270) / 4
	.2byte	(.L4271 - .Lrtx4270) / 4
	.2byte	(.L4272 - .Lrtx4270) / 4
	.2byte	(.L4273 - .Lrtx4270) / 4
	.2byte	(.L4274 - .Lrtx4270) / 4
	.2byte	(.L4275 - .Lrtx4270) / 4
	.2byte	(.L4276 - .Lrtx4270) / 4
	.2byte	(.L4277 - .Lrtx4270) / 4
	.2byte	(.L4278 - .Lrtx4270) / 4
	.2byte	(.L4279 - .Lrtx4270) / 4
	.2byte	(.L4280 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4281 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4268 - .Lrtx4270) / 4
	.2byte	(.L4282 - .Lrtx4270) / 4
	.text
	.p2align 3
.L4441:
	mov	w1, 24
	mov	x0, x22
	bl	BsGet
	mov	w20, w0
	ldr	x0, [x19, 240]
	ubfx	x1, x20, 23, 1
	strb	w1, [x0, 5]
	ldr	x1, [x19, 240]
	ldrsb	w0, [x1, 5]
	cbnz	w0, .L4265
	add	x0, x19, 12288
	ubfx	x4, x20, 16, 6
	ubfx	x2, x20, 6, 10
	mov	w5, 1
	ubfx	x3, x20, 22, 1
	strb	w4, [x0, 2386]
	ubfx	x4, x20, 3, 3
	strb	w5, [x0, 2384]
	strb	w3, [x0, 2385]
	str	w2, [x19, 14680]
	ubfx	x2, x20, 2, 1
	strb	w4, [x0, 2387]
	ubfx	x4, x20, 1, 1
	strb	w2, [x0, 2388]
	and	w2, w20, 1
	strb	w4, [x0, 2389]
	strb	w2, [x0, 2390]
	ldrb	w0, [x1, 2]
	cmp	w0, 20
	beq	.L4444
.L4265:
	ldr	w0, [x1, 72]
	add	w0, w0, 24
	str	w0, [x1, 72]
	ldr	x1, .LC1031
	ldr	x0, [x19, 240]
	ldr	w2, [x19, 68]
	ldrb	w3, [x0, 2]
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x19, 68]
	cmp	w0, 371
	bne	.L4267
.L4442:
	ldr	x1, .LC1033
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x19, 68]
	b	.L4267
.L4269:
	ldr	x1, .LC1036
	mov	w0, 22
	ldr	w3, [x19, 80]
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	add	x2, x19, 11116544
	add	x5, x19, 11120640
	add	x0, x19, 12288
	mov	w3, -1
	ldrb	w6, [x1, 2]
	strb	w6, [x2, 2811]
	ldrb	w4, [x1, 4]
	strb	w4, [x2, 2816]
	ldrsb	w1, [x1, 5]
	strb	w1, [x2, 2820]
	strb	wzr, [x2, 2819]
	str	w3, [x5, 352]
	ldrb	w3, [x0, 2384]
	cmp	w3, 1
	beq	.L4445
	cmn	w1, #1
	beq	.L4446
.L4284:
	ldr	x1, .LC1038
	mov	w0, 21
	ldr	w2, [x19, 88]
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_SliceCheck
	cbnz	w0, .L4447
	mov	x0, x19
	bl	MVC_DecSlice
	cmn	w0, #2
	beq	.L4295
	cbz	w0, .L4426
	ldr	x0, .LC1039
	ldr	x4, [x0]
	cbnz	x4, .L4427
.L4425:
	mov	w0, -1
.L4451:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L4271:
	ldr	x1, .LC1049
	mov	w2, 6
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_DecSEI
	mov	w20, w0
	ldr	x1, [x19, 240]
	cbz	x1, .L4302
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L4302:
	cbz	w20, .L4426
	ldr	x1, .LC1051
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC1039
	ldr	x4, [x0]
	cbnz	x4, .L4430
	b	.L4426
.L4272:
	ldr	x1, .LC1045
	mov	w2, 7
	ldr	w3, [x19, 72]
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_DecSPS
	mov	w20, w0
	ldr	x1, [x19, 240]
	cbz	x1, .L4300
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L4300:
	cbnz	w20, .L4448
	ldr	w1, [x19, 72]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 72]
	b	.L4295
.L4273:
	ldr	x1, .LC1041
	mov	w2, 8
	ldr	w3, [x19, 76]
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_DecPPS
	mov	w20, w0
	ldr	x1, [x19, 240]
	cbz	x1, .L4298
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L4298:
	cbnz	w20, .L4449
	ldr	w1, [x19, 76]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 76]
	b	.L4295
.L4274:
	ldr	x1, .LC1053
	mov	w2, 9
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	cbz	x1, .L4426
.L4429:
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L4275:
	ldr	x1, .LC1055
	mov	w2, 10
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	cbnz	x1, .L4429
	b	.L4426
.L4276:
	ldr	x1, .LC1059
	mov	w2, 11
	mov	w0, 22
	bl	dprint_vfmw
	mov	w1, 32
	mov	x0, x22
	bl	BsGet
	mov	w20, w0
	mov	w1, 32
	mov	x0, x22
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w20, w2
	ccmp	w0, w1, 0, eq
	bne	.L4435
	ldr	x20, .LC1039
	mov	w0, 1
	strb	w0, [x19, 1]
	ldr	x4, [x20]
	cbz	x4, .L4305
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L4305:
	strb	wzr, [x19]
	mov	x0, x19
	bl	MVC_DecVDM
	cbz	w0, .L4435
	b	.L4437
.L4277:
	ldr	x1, .LC1057
	mov	w2, 12
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	cbnz	x1, .L4429
	b	.L4426
.L4278:
	ldr	x1, .LC1061
	mov	w2, 13
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	cbnz	x1, .L4429
	b	.L4426
.L4279:
	ldr	x1, .LC1063
	mov	w2, 14
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x19, 240]
	cbnz	x1, .L4429
	b	.L4426
.L4280:
	ldr	x1, .LC1065
	mov	w2, 15
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_DecSubSPS
	mov	w20, w0
	ldr	x1, [x19, 240]
	cbz	x1, .L4306
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L4306:
	cbnz	w20, .L4450
	ldr	w1, [x19, 20]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 20]
	b	.L4295
.L4281:
	ldr	x1, .LC1068
	mov	w2, 19
	mov	w0, 22
	bl	dprint_vfmw
.L4435:
	ldr	x1, [x19, 240]
	cbnz	x1, .L4429
	b	.L4426
.L4282:
	ldr	x1, .LC1070
	mov	w2, 30
	mov	w0, 22
	bl	dprint_vfmw
	cbnz	w21, .L4308
	ldr	x1, [x19, 240]
	cbz	x1, .L4308
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
.L4308:
	mov	w1, 32
	mov	x0, x22
	bl	BsGet
	mov	w20, w0
	mov	w1, 32
	mov	x0, x22
	rev	w20, w20
	bl	BsGet
	rev	w0, w0
	ldr	x1, .LC1071
	add	x1, x1, 432
	ldr	w2, [x1, 4]
	ldr	w1, [x1, 8]
	cmp	w2, w20
	ccmp	w1, w0, 0, eq
	bne	.L4426
	ldr	x20, .LC1039
	ldr	x4, [x20]
	cbz	x4, .L4309
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L4309:
	strb	wzr, [x19]
	mov	x0, x19
	bl	MVC_DecVDM
	cbz	w0, .L4426
.L4437:
	ldr	x4, [x20]
	cbz	x4, .L4425
.L4427:
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, -1
	b	.L4451
.L4444:
	mov	w0, 5
	cmp	w3, wzr
	csinc	w0, w0, wzr, eq
	strb	w0, [x1, 2]
	ldr	x1, [x19, 240]
	b	.L4265
.L4447:
	mov	x0, x19
	bl	MVC_ClearCurrSlice
	ldr	x0, .LC1039
	ldr	x4, [x0]
	cbnz	x4, .L4427
	mov	w0, -1
	b	.L4451
.L4450:
	ldr	x1, .LC1066
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC1039
	ldr	x4, [x0]
	cbz	x4, .L4425
	add	x2, x29, 64
	ldr	w0, [x19, 120]
	mov	w20, -1
	mov	w3, 4
	mov	w1, 100
	str	w20, [x2, -8]!
	blr	x4
	mov	w0, w20
	b	.L4295
.L4449:
	ldr	x1, .LC1043
.L4439:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, .LC1039
	ldr	x4, [x21]
	cbz	x4, .L4425
	add	x2, x29, 64
	ldr	w0, [x19, 120]
	mov	w20, -1
	mov	w3, 4
	mov	w1, 100
	str	w20, [x2, -8]!
	blr	x4
	ldr	x4, [x21]
	cbz	x4, .L4425
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, w20
	b	.L4295
.L4448:
	ldr	x1, .LC1047
	b	.L4439
.L4446:
	ldr	w1, [x19, 20]
	cbz	w1, .L4284
	add	x1, x19, 10768384
	ldr	w3, [x1, 4064]
	cbz	w3, .L4287
	ldrb	w3, [x1, 4060]
	cmp	w3, 1
	beq	.L4288
.L4287:
	add	x0, x0, 2400
	mov	x1, 0
	.p2align 2
.L4291:
	ldr	w4, [x0]
	add	x3, x0, 335872
	cbz	w4, .L4289
	ldrb	w0, [x0, -4]
	cbnz	w0, .L4452
.L4289:
	add	w1, w1, 1
	add	x0, x3, 308
	cmp	w1, 32
	bne	.L4291
	mov	w0, -1
.L4424:
	cmn	w0, #1
	str	w0, [x5, 352]
	beq	.L4284
	cmp	w6, 5
	mov	w0, 1
	cset	w1, eq
	strb	w0, [x2, 2819]
	strb	w0, [x2, 2818]
	strb	w1, [x2, 2817]
	b	.L4284
.L4445:
	strb	w3, [x2, 2819]
	ldr	w1, [x19, 14680]
	str	w1, [x5, 352]
	ldrb	w1, [x0, 2388]
	strb	w1, [x2, 2817]
	ldrb	w1, [x0, 2389]
	strb	w1, [x2, 2818]
	strb	wzr, [x0, 2384]
	b	.L4284
.L4452:
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 2
	sub	x0, x0, x1
	lsl	x0, x0, 4
	sub	x1, x0, x1
	lsl	x0, x1, 2
	sub	x1, x0, x1
	lsl	x0, x1, 6
	add	x0, x1, x0
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 14692]
	b	.L4424
.L4288:
	ldr	w0, [x1, 4068]
	b	.L4424
	.size	MVC_DecOneNal, .-MVC_DecOneNal
	.align	3
.LC1031:
	.xword	.LC1030
	.align	3
.LC1033:
	.xword	.LC1032
	.align	3
.LC1034:
	.xword	.L4270
	.align	3
.LC1036:
	.xword	.LC1035
	.align	3
.LC1038:
	.xword	.LC1037
	.align	3
.LC1039:
	.xword	g_event_report
	.align	3
.LC1041:
	.xword	.LC1040
	.align	3
.LC1043:
	.xword	.LC1042
	.align	3
.LC1045:
	.xword	.LC1044
	.align	3
.LC1047:
	.xword	.LC1046
	.align	3
.LC1049:
	.xword	.LC1048
	.align	3
.LC1051:
	.xword	.LC1050
	.align	3
.LC1053:
	.xword	.LC1052
	.align	3
.LC1055:
	.xword	.LC1054
	.align	3
.LC1057:
	.xword	.LC1056
	.align	3
.LC1059:
	.xword	.LC1058
	.align	3
.LC1061:
	.xword	.LC1060
	.align	3
.LC1063:
	.xword	.LC1062
	.align	3
.LC1065:
	.xword	.LC1064
	.align	3
.LC1066:
	.xword	.LC749
	.align	3
.LC1068:
	.xword	.LC1067
	.align	3
.LC1070:
	.xword	.LC1069
	.align	3
.LC1071:
	.xword	.LANCHOR1
	.align	3
.LC1073:
	.xword	.LC1072
	.align	3
.LC1075:
	.xword	.LC1074
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DEC_DecodePacket
	.type	MVC_DEC_DecodePacket, %function
MVC_DEC_DecodePacket:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	str	wzr, [x29, 68]
	stp	wzr, wzr, [x29, 60]
	cbz	x20, .L4515
	ldr	x0, [x19, 224]
	ldrb	w1, [x19, 5]
	ldr	w2, [x0, 8]
	cmp	w1, 1
	ubfiz	w2, w2, 16, 2
	str	w2, [x19, 108]
	ldr	w1, [x0]
	str	w1, [x19, 112]
	ldr	w1, [x0, 4]
	str	w1, [x19, 116]
	strb	wzr, [x19, 10]
	beq	.L4456
	ldr	w1, [x0, 832]
	add	x21, x19, 11161600
	cmp	w1, 1
	beq	.L4516
.L4457:
	mov	x1, x20
	mov	x0, x19
	bl	MVC_ReceivePacket
	ldr	x1, [x19, 240]
	str	x1, [x21, 1752]
.L4459:
	strb	wzr, [x19, 5]
	cbz	w0, .L4460
.L4479:
	mov	w0, 0
.L4508:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4460:
	ldr	x0, [x19, 240]
.L4458:
	str	wzr, [x0, 72]
	add	x21, x19, 872
	mov	x0, x21
	ldr	x1, [x19, 240]
	ldr	w2, [x1, 16]
	ldr	x1, [x1, 8]
	bl	BsInit
	mov	w1, 32
	mov	x0, x21
	bl	BsGet
	mov	w20, w0
	and	w3, w0, 31
	ldr	x0, [x19, 240]
	ubfx	x2, x20, 5, 2
	mov	w1, -1
	strb	w3, [x0, 2]
	ldr	x0, [x19, 240]
	strb	w2, [x0, 4]
	ldr	x0, [x19, 240]
	strb	w1, [x0, 5]
	ldr	x0, [x19, 240]
	ldrb	w0, [x0, 2]
	cmp	w0, 20
	beq	.L4517
.L4509:
	mov	w0, 27
	strb	wzr, [x19, 10]
	and	w20, w20, w0
	cmp	w20, 1
	beq	.L4465
.L4466:
	mov	x0, x19
	bl	MVC_FindTrailZeros
	cbnz	w0, .L4518
.L4478:
	ldr	x1, [x19, 240]
	mov	x0, x21
	str	wzr, [x1, 72]
	ldr	x1, [x19, 240]
	ldr	w2, [x1, 16]
	ldr	x1, [x1, 8]
	bl	BsInit
	mov	w1, 0
	mov	x0, x19
	bl	MVC_DecOneNal
	cmn	w0, #2
	beq	.L4508
	cbz	w0, .L4479
.L4513:
	ldr	x0, .LC1083
	ldr	x4, [x0]
	cbz	x4, .L4511
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L4511:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	mov	w0, -1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4516:
	ldr	w0, [x0, 872]
	cmp	w0, 1
	bne	.L4457
	ldr	w0, [x21, 1744]
	cmp	w0, 1
	bne	.L4457
	ldr	x0, [x21, 1752]
	str	x0, [x19, 240]
	str	wzr, [x21, 1744]
	strb	wzr, [x19, 5]
	b	.L4458
	.p2align 3
.L4456:
	ldr	x0, [x19, 240]
	cmp	x0, xzr
	csetm	w0, eq
	b	.L4459
	.p2align 3
.L4517:
	mov	w1, 24
	mov	x0, x21
	bl	BsGet
	ubfx	x2, x0, 23, 1
	ldr	x1, [x19, 240]
	strb	w2, [x1, 5]
	ldr	x2, [x19, 240]
	ldrsb	w1, [x2, 5]
	cbnz	w1, .L4509
	add	x1, x19, 12288
	ubfx	x4, x0, 22, 1
	mov	w6, 1
	ubfx	x3, x0, 6, 10
	ubfx	x5, x0, 2, 1
	cmp	w4, wzr
	strb	w6, [x1, 2384]
	ubfx	x0, x0, 1, 1
	strb	w4, [x1, 2385]
	str	w3, [x19, 14680]
	mov	w3, 5
	strb	w0, [x1, 2389]
	csinc	w0, w3, wzr, eq
	strb	w5, [x1, 2388]
	strb	w0, [x2, 2]
	strb	wzr, [x19, 10]
	.p2align 2
.L4465:
	add	x2, x29, 76
	add	x1, x29, 72
	mov	x0, x19
	bl	MVC_InquireSliceProperty
	cbnz	w0, .L4467
	ldr	w1, [x29, 72]
	ldr	w0, [x19, 128]
	cmp	w1, 1
	csinc	w0, w0, w0, ne
	str	w0, [x19, 128]
	beq	.L4519
.L4468:
	ldr	w0, [x29, 76]
	cmp	w0, 1
	bne	.L4466
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbnz	w0, .L4520
.L4475:
	ldr	w0, [x19, 120]
	bl	FSP_GetFspType
	cbnz	w0, .L4466
	ldr	w0, [x19, 120]
	add	x2, x29, 64
	add	x3, x29, 68
	add	x1, x29, 60
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19, 224]
	ldr	w2, [x0, 708]
	cmp	w2, 1
	bne	.L4466
	ldr	w0, [x29, 68]
	cmp	w0, wzr
	ble	.L4521
.L4472:
	mov	w1, 1
	mov	w0, -2
	strb	w1, [x19, 5]
	b	.L4508
	.p2align 3
.L4467:
	ldr	x1, [x19, 240]
	cbz	x1, .L4513
	ldr	w0, [x19, 120]
	bl	MVC_ReleaseNAL
	str	xzr, [x19, 240]
	b	.L4513
	.p2align 3
.L4518:
	ldr	x1, .LC1085
	mov	w0, 22
	bl	dprint_vfmw
	b	.L4478
	.p2align 3
.L4519:
	add	x20, x19, 11116544
	ldrb	w0, [x20, 2809]
	cmp	w0, 1
	ldr	w0, [x19, 132]
	beq	.L4522
	add	w0, w0, 2
	str	w0, [x19, 132]
.L4470:
	ldr	w0, [x19, 64]
	cbz	w0, .L4468
	ldr	x3, [x19, 240]
	mov	w2, 12
	ldr	x1, .LC1079
	mov	x0, x21
	add	x1, x1, 432
	str	wzr, [x3, 72]
	bl	BsInit
	mov	w1, 1
	mov	x0, x19
	bl	MVC_DecOneNal
	ldrb	w0, [x20, 2809]
	cmp	w0, 1
	ldr	w0, [x19, 132]
	beq	.L4523
	sub	w0, w0, #2
	str	w0, [x19, 132]
	b	.L4472
.L4522:
	add	w0, w0, 1
	str	w0, [x19, 132]
	b	.L4470
.L4520:
	ldr	x1, .LC1080
	mov	w3, 0
	mov	w2, 14134
	mov	w0, 22
	bl	dprint_vfmw
	b	.L4475
.L4515:
	ldr	x2, .LC1077
	mov	w3, 14017
	ldr	x1, .LC1078
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4508
.L4523:
	sub	w0, w0, #1
	str	w0, [x19, 132]
	b	.L4472
.L4521:
	ldr	w0, [x29, 64]
	cmp	w0, 32
	bgt	.L4472
	ldr	x0, [x19, 232]
	cbnz	x0, .L4472
	ldr	w3, [x19, 88]
	cbz	w3, .L4466
	ldr	w6, [x19, 12]
	cbz	w6, .L4466
	add	x20, x19, 11161600
	ldr	w7, [x19, 16]
	ldr	w5, [x20, 1760]
	cmp	w7, w5
	beq	.L4466
	ldr	w4, [x20, 1764]
	cmp	w6, w4
	beq	.L4466
	ldr	x1, .LC1082
	mov	w0, w2
	bl	dprint_vfmw
	ldr	w0, [x19, 16]
	ldr	x1, .LC1083
	str	w0, [x20, 1760]
	ldr	x4, [x1]
	ldr	w0, [x19, 12]
	str	w0, [x20, 1764]
	cbz	x4, .L4466
	ldr	w0, [x19, 120]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L4466
	.size	MVC_DEC_DecodePacket, .-MVC_DEC_DecodePacket
	.align	3
.LC1077:
	.xword	.LC36
	.align	3
.LC1078:
	.xword	.LC38
	.align	3
.LC1079:
	.xword	.LANCHOR1
	.align	3
.LC1080:
	.xword	.LC987
	.align	3
.LC1082:
	.xword	.LC1081
	.align	3
.LC1083:
	.xword	g_event_report
	.align	3
.LC1085:
	.xword	.LC1084
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DEC_VDMPostProc
	.type	MVC_DEC_VDMPostProc, %function
MVC_DEC_VDMPostProc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldrb	w0, [x19]
	cbnz	w0, .L4525
	add	x21, x19, 11157504
	mov	x0, x19
	str	w20, [x21, 2660]
	bl	MVC_ClearAllSlice
	mov	x0, x19
	bl	MVC_StorePicInDpb
	mov	w22, w0
	cbnz	w0, .L4536
	mov	w1, 1
	strb	w1, [x19, 4]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L4525:
	add	x2, x19, 11157504
	mov	x0, x19
	str	w20, [x2, 2660]
	bl	MVC_ClearAllSlice
	mov	x0, x19
	bl	MVC_StorePicInDpb
	cbnz	w0, .L4537
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbnz	w0, .L4538
.L4529:
	add	x0, x19, 11120640
	strb	wzr, [x19, 4]
	mov	w4, 32
	ldr	w8, [x19, 40]
	mov	w7, 23352
	mov	w6, 2
	strb	w4, [x0, 370]
	add	x4, x0, 256
	strb	wzr, [x0, 368]
	movk	w7, 0x1, lsl 16
	strb	w6, [x0, 369]
	mov	w5, 7
	str	w8, [x0, 380]
	mov	w3, 3
	strb	w5, [x0, 371]
	mov	w2, -1
	str	w7, [x0, 384]
	mov	w1, 262144
	stp	w3, w2, [x4, 136]
	stp	wzr, wzr, [x4, 144]
	str	w1, [x0, 408]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L4536:
	ldr	x1, [x21, 2600]
	mov	w2, 1
	ldr	w0, [x19, 120]
	mov	w20, -1
	ldrsb	w1, [x1, 6]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC1087
	mov	w2, w22
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	MVC_ClearAllSlice
	add	x0, x19, 11120640
	ldr	w4, [x19, 40]
	add	x5, x0, 256
	mov	w2, 2
	mov	w3, 23352
	mov	w1, 7
	strb	w2, [x0, 369]
	movk	w3, 0x1, lsl 16
	strb	w1, [x0, 371]
	mov	w2, 32
	str	w4, [x0, 380]
	mov	w1, 3
	strb	w2, [x0, 370]
	mov	w2, 262144
	str	w3, [x0, 384]
	strb	wzr, [x0, 368]
	stp	w1, w20, [x5, 136]
	stp	wzr, wzr, [x5, 144]
	str	w2, [x0, 408]
	mov	w1, w20
	mov	x0, x19
	bl	MVC_ClearDPB
	str	xzr, [x21, 2600]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L4538:
	ldr	x1, .LC1091
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	b	.L4529
	.p2align 3
.L4537:
	ldr	x1, .LC1089
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w1, -1
	mov	x0, x19
	bl	MVC_FlushDPB
	cbz	w0, .L4529
	b	.L4538
	.size	MVC_DEC_VDMPostProc, .-MVC_DEC_VDMPostProc
	.align	3
.LC1087:
	.xword	.LC1086
	.align	3
.LC1089:
	.xword	.LC1088
	.align	3
.LC1091:
	.xword	.LC1090
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MVC_DEC_GetImageBuffer
	.type	MVC_DEC_GetImageBuffer, %function
MVC_DEC_GetImageBuffer:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldr	w0, [x20, 48]
	cbz	w0, .L4556
	mov	x1, 51120
	sub	w0, w0, #1
	mov	w4, 840
	movk	x1, 0xa9, lsl 16
	add	x21, x20, 11124736
	umaddl	x4, w0, w4, x1
	add	x2, x21, 1128
	add	x4, x20, x4
	.p2align 2
.L4547:
	ldrb	w0, [x2, 2]
	cbnz	w0, .L4542
	cbz	x2, .L4543
	ldr	w0, [x21, 760]
	cbz	w0, .L4543
	ldr	x1, [x21, 376]
	cmp	x1, x2
	ccmp	x1, xzr, 4, eq
	bne	.L4542
	sub	w3, w0, #1
	add	x0, x21, 384
	add	x3, x3, 1388544
	add	x3, x3, 2096
	add	x3, x20, x3, lsl 3
	b	.L4544
	.p2align 3
.L4545:
	ldr	x1, [x0], 8
	cmp	x1, xzr
	ccmp	x1, x2, 0, ne
	beq	.L4542
.L4544:
	cmp	x0, x3
	bne	.L4545
.L4543:
	ldr	w0, [x20, 120]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L4556
	ldr	w0, [x20, 120]
	bl	FSP_IsNewFsAvalible
	cmn	w0, #1
	beq	.L4577
.L4575:
	mov	w0, 0
.L4578:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4542:
	add	x2, x2, 840
	cmp	x2, x4
	bne	.L4547
	add	x2, x29, 60
	add	x1, x29, 56
	add	x0, x20, 912
	bl	GetQueueImgNum
	ldr	x1, .LC1093
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, .LC1095
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x20, 48]
	cbz	w0, .L4555
	ldr	x19, .LC1099
	mov	w22, 0
	.p2align 2
.L4554:
	uxtw	x2, w22
	lsl	x0, x2, 3
	sub	x2, x0, x2
	lsl	x0, x2, 4
	sub	x2, x0, x2
	lsl	x2, x2, 3
	add	x0, x20, x2
	add	x2, x2, 11124736
	add	x0, x0, 11124736
	add	x2, x20, x2
	adds	x2, x2, 1128
	ldrb	w3, [x0, 1130]
	ldrb	w4, [x0, 1133]
	beq	.L4559
	ldr	w0, [x21, 760]
	cbz	w0, .L4559
	ldr	x1, [x21, 376]
	cmp	x2, x1
	ccmp	x1, xzr, 4, eq
	bne	.L4561
	sub	w5, w0, #1
	add	x0, x21, 384
	add	x5, x5, 1388544
	add	x5, x5, 2096
	add	x5, x20, x5, lsl 3
	b	.L4552
	.p2align 3
.L4553:
	ldr	x1, [x0], 8
	cmp	x2, x1
	ccmp	x1, xzr, 4, eq
	bne	.L4561
.L4552:
	cmp	x5, x0
	bne	.L4553
.L4559:
	mov	w5, 1
.L4551:
	mov	w2, w22
	mov	x1, x19
	mov	w0, 0
	add	w22, w22, 1
	bl	dprint_vfmw
	ldr	w0, [x20, 48]
	cmp	w0, w22
	bhi	.L4554
.L4555:
	ldp	w2, w3, [x29, 56]
	ldr	x1, .LC1097
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x20, 48]
	ldr	w2, [x21, 764]
	ldp	w3, w1, [x29, 56]
	sub	w0, w0, #2
	sub	w0, w0, w2
	add	w1, w3, w1
	cmp	w1, w0
	bge	.L4575
	ldr	x1, .LC1101
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x20
	bl	MVC_ClearAll
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4556:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, 1
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4561:
	mov	w5, 0
	b	.L4551
.L4577:
	ldr	w0, [x20, 120]
	add	x1, x20, 912
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	b	.L4578
	.size	MVC_DEC_GetImageBuffer, .-MVC_DEC_GetImageBuffer
	.align	3
.LC1093:
	.xword	.LC1092
	.align	3
.LC1095:
	.xword	.LC1094
	.align	3
.LC1097:
	.xword	.LC1096
	.align	3
.LC1099:
	.xword	.LC1098
	.align	3
.LC1101:
	.xword	.LC1100
	.global	MvcTmpBuf
	.global	MVC_quant8_org
	.global	MVC_quant_org
	.global	MVC_quant8_inter_default
	.global	MVC_quant8_intra_default
	.global	MVC_quant_inter_default
	.global	MVC_quant_intra_default
	.global	MVC_g_ZZ_SCAN8
	.global	MVC_g_ZZ_SCAN
	.global	MVC_CalcZeroNum
	.global	MVC_g_AspecRatioIdc
	.global	MVC_g_NalTypeEOPIC
	.section	.rodata
	.align	3
.LANCHOR0 = . + 0
	.type	MVC_g_ZZ_SCAN, %object
	.size	MVC_g_ZZ_SCAN, 16
MVC_g_ZZ_SCAN:
	.byte	0
	.byte	1
	.byte	4
	.byte	8
	.byte	5
	.byte	2
	.byte	3
	.byte	6
	.byte	9
	.byte	12
	.byte	13
	.byte	10
	.byte	7
	.byte	11
	.byte	14
	.byte	15
	.type	MVC_g_ZZ_SCAN8, %object
	.size	MVC_g_ZZ_SCAN8, 64
MVC_g_ZZ_SCAN8:
	.byte	0
	.byte	1
	.byte	8
	.byte	16
	.byte	9
	.byte	2
	.byte	3
	.byte	10
	.byte	17
	.byte	24
	.byte	32
	.byte	25
	.byte	18
	.byte	11
	.byte	4
	.byte	5
	.byte	12
	.byte	19
	.byte	26
	.byte	33
	.byte	40
	.byte	48
	.byte	41
	.byte	34
	.byte	27
	.byte	20
	.byte	13
	.byte	6
	.byte	7
	.byte	14
	.byte	21
	.byte	28
	.byte	35
	.byte	42
	.byte	49
	.byte	56
	.byte	57
	.byte	50
	.byte	43
	.byte	36
	.byte	29
	.byte	22
	.byte	15
	.byte	23
	.byte	30
	.byte	37
	.byte	44
	.byte	51
	.byte	58
	.byte	59
	.byte	52
	.byte	45
	.byte	38
	.byte	31
	.byte	39
	.byte	46
	.byte	53
	.byte	60
	.byte	61
	.byte	54
	.byte	47
	.byte	55
	.byte	62
	.byte	63
	.type	__FUNCTION__.12683, %object
	.size	__FUNCTION__.12683, 18
__FUNCTION__.12683:
	.string	"MVC_WriteSliceMsg"
	.zero	6
	.type	MVC_SarTable.12976, %object
	.size	MVC_SarTable.12976, 136
MVC_SarTable.12976:
	.word	1
	.word	1
	.word	1
	.word	1
	.word	12
	.word	11
	.word	10
	.word	11
	.word	16
	.word	11
	.word	40
	.word	33
	.word	24
	.word	11
	.word	20
	.word	11
	.word	32
	.word	11
	.word	80
	.word	33
	.word	18
	.word	11
	.word	15
	.word	11
	.word	64
	.word	33
	.word	160
	.word	99
	.word	4
	.word	3
	.word	3
	.word	2
	.word	2
	.word	1
	.type	__func__.13210, %object
	.size	__func__.13210, 11
__func__.13210:
	.string	"MVC_DecSEI"
	.zero	5
	.type	__func__.11863, %object
	.size	__func__.11863, 17
__func__.11863:
	.string	"MVC_DirectOutput"
	.zero	7
	.type	__func__.12443, %object
	.size	__func__.12443, 12
__func__.12443:
	.string	"MVC_InitDPB"
	.zero	4
	.type	__FUNCTION__.11583, %object
	.size	__FUNCTION__.11583, 13
__FUNCTION__.11583:
	.string	"MVC_ClearDPB"
	.zero	3
	.type	__func__.13321, %object
	.size	__func__.13321, 17
__func__.13321:
	.string	"MVC_FlushDecoder"
	.zero	7
	.type	__FUNCTION__.11591, %object
	.size	__FUNCTION__.11591, 13
__FUNCTION__.11591:
	.string	"MVC_ClearAll"
	.zero	3
	.type	__FUNCTION__.13448, %object
	.size	__FUNCTION__.13448, 13
__FUNCTION__.13448:
	.string	"MVC_DEC_Init"
	.zero	3
	.type	__func__.12013, %object
	.size	__func__.12013, 18
__func__.12013:
	.string	"MVC_StorePicInDpb"
	.data
	.align	3
.LANCHOR1 = . + 0
	.type	MVC_quant_intra_default, %object
	.size	MVC_quant_intra_default, 16
MVC_quant_intra_default:
	.word	471076102
	.word	538711053
	.word	622861332
	.word	707076124
	.type	MVC_quant_inter_default, %object
	.size	MVC_quant_inter_default, 16
MVC_quant_inter_default:
	.word	403967498
	.word	454562830
	.word	505092116
	.word	572398360
	.type	MVC_quant8_intra_default, %object
	.size	MVC_quant8_intra_default, 64
MVC_quant8_intra_default:
	.word	269289990
	.word	454629138
	.word	303041290
	.word	488315159
	.word	387059725
	.word	522001177
	.word	420942352
	.word	555687195
	.word	454629138
	.word	606150429
	.word	488315159
	.word	639901983
	.word	522001177
	.word	673588257
	.word	555687195
	.word	707274276
	.type	MVC_quant8_inter_default, %object
	.size	MVC_quant8_inter_default, 256
MVC_quant8_inter_default:
	.word	286199049
	.word	404100371
	.word	319884557
	.word	421008917
	.word	353571087
	.word	454629398
	.word	370479889
	.word	471537944
	.word	404100371
	.word	505158425
	.word	421008917
	.word	538844187
	.word	454629398
	.word	555752988
	.word	471537944
	.word	589373470
	.zero	192
	.type	MVC_quant_org, %object
	.size	MVC_quant_org, 16
MVC_quant_org:
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.type	MVC_quant8_org, %object
	.size	MVC_quant8_org, 64
MVC_quant8_org:
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.type	MVC_g_NalTypeEOPIC, %object
	.size	MVC_g_NalTypeEOPIC, 12
MVC_g_NalTypeEOPIC:
	.byte	0
	.byte	0
	.byte	1
	.byte	30
	.byte	72
	.byte	83
	.byte	80
	.byte	73
	.byte	67
	.byte	69
	.byte	78
	.byte	68
	.zero	4
	.type	MVC_CalcZeroNum, %object
	.size	MVC_CalcZeroNum, 256
MVC_CalcZeroNum:
	.byte	8
	.byte	7
	.byte	6
	.byte	6
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.type	MVC_g_AspecRatioIdc, %object
	.size	MVC_g_AspecRatioIdc, 112
MVC_g_AspecRatioIdc:
	.word	1
	.word	1
	.word	1
	.word	1
	.word	12
	.word	11
	.word	10
	.word	11
	.word	16
	.word	11
	.word	40
	.word	33
	.word	24
	.word	11
	.word	20
	.word	11
	.word	32
	.word	11
	.word	80
	.word	33
	.word	18
	.word	11
	.word	15
	.word	11
	.word	64
	.word	33
	.word	160
	.word	99
	.section	.rodata.str1.8,"aMS",%progbits,1
	.align	3
.LC0:
	ASCII(.string	"\n\nEND of the bit buffer, copy the first packet!\n\n" )
	.zero	6
.LC2:
	ASCII(.string	"0:phy:0x%x, 1:phy:0x%x; len0:%d len1:%d\n" )
	.zero	7
.LC5:
	ASCII(.string	"0:phy:0x%x, len0:%d\n" )
	.zero	3
.LC7:
	ASCII(.string	"%-50s%50d\n" )
	.zero	5
.LC14:
	ASCII(.string	"MVC_FRAME %d, state=%d, is already output\n" )
	.zero	5
.LC16:
	ASCII(.string	"plfs(fs=%p, eFoState=%d) is null, logic_fs_id = %d\n" )
	.zero	4
.LC18:
	ASCII(.string	"not used MVC_FRAME: (%d,%d)\n" )
	.zero	3
.LC20:
	ASCII(.string	"MVC_FRAME skip: is_used=%d,skip=%d\n" )
	.zero	4
.LC22:
	ASCII(.string	"topfield skip\n" )
	.zero	1
.LC24:
	ASCII(.string	"bottom field skip\n" )
	.zero	5
.LC26:
	ASCII(.string	"err(%d) > out_thr(%d)\n" )
	.zero	1
.LC28:
	ASCII(.string	"exg_pts: %lld <-> %lld\n" )
.LC31:
	ASCII(.string	"stream MVC_FRAME packing type is %d! what can we do?\n" )
	.zero	2
.LC36:
	ASCII(.string	"firmware/common/syntax/mvc.c" )
	.zero	3
.LC38:
	ASCII(.string	"NULL pointer: %s, L%d\n" )
	.zero	1
.LC40:
	ASCII(.string	"framestore %d allocate apc %d\n" )
	.zero	1
.LC42:
	ASCII(.string	"find APC, but logic_fs_id %d abnormal(ref=%d,plfs=%p, pdfs=%p)\n" )
.LC45:
	ASCII(.string	"\n" )
	.zero	6
.LC47:
	ASCII(.string	"pMvcCtx->DPB.fs[%d]: frame_num=%d poc=%d is_reference=%d\n" )
	.zero	6
.LC49:
	ASCII(.string	"pMvcCtx->DPB.fs_ref[%d]: frame_num=%d poc=%d is_reference=%d\n" )
	.zero	2
.LC51:
	ASCII(.string	"pMvcCtx->DPB.fs_ref[%d]: MVC_FRAME.poc=%d \n" )
	.zero	4
.LC57:
	ASCII(.string	"\n VFMW ** release streambuff=%p   bitstream_length=%d\n" )
	.zero	1
.LC59:
	ASCII(.string	"clear curr slice.\n" )
	.zero	5
.LC62:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC64:
	ASCII(.string	"ERROR: FSP_ConfigInstance fail!\n" )
	.zero	7
.LC73:
	ASCII(.string	"for P slice size of list equal 0(1st).\n" )
.LC79:
	ASCII(.string	"for P slice size of list equal 0(2nd).\n" )
.LC81:
	ASCII(.string	"for B slice size of two list all equal 0.\n" )
	.zero	5
.LC84:
	ASCII(.string	"list[%d][%d]: frame_num=%d, poc=%d\n" )
	.zero	4
.LC86:
	ASCII(.string	"init list error.\n" )
	.zero	6
.LC88:
	ASCII(.string	"In one access unit,all non-base view should have the same subsps!\n" )
	.zero	5
.LC90:
	ASCII(.string	"sps but mvc flag %d is wrong\n" )
	.zero	2
.LC102:
	ASCII(.string	"save pic yuv :  structure = %d;  idc = %d\n" )
	.zero	5
.LC104:
	ASCII(.string	"structure = %d pMvcCtx->CurrPic.pic_width_in_mb = %d pMvcCtx->CurrPic.pic_height_in_mb = %d\n" )
	.zero	3
.LC106:
	ASCII(.string	"nal_ref_idc=%d, structure=%d, image_id=%d, pmv_idc=%d\n" )
	.zero	1
.LC108:
	ASCII(.string	"logic MVC_FRAME id(=%d): get LogicFs error!\n" )
	.zero	3
.LC110:
	ASCII(.string	"phy fs is null: pstDecodeFs = %p, pstDispOutFs = %p\n" )
	.zero	3
.LC116:
	ASCII(.string	"fuction return value is null,%s %d unknow error!!\n" )
	.zero	5
.LC118:
	ASCII(.string	"SH: first_mb_in_slice" )
	.zero	2
.LC120:
	ASCII(.string	"MVC_SliceCheck first_mb_in_slice >= MAX_MB_NUM_IN_PIC error.\n" )
	.zero	2
.LC122:
	ASCII(.string	"SH: slice_type" )
	.zero	1
.LC124:
	ASCII(.string	"slice type = %d\n" )
	.zero	7
.LC127:
	ASCII(.string	"slice type = %d, err\n" )
	.zero	2
.LC129:
	ASCII(.string	"slice  pps id = %d\n" )
	.zero	4
.LC131:
	ASCII(.string	"pps with this pic_parameter_set_id = %d havn't decode\n" )
	.zero	1
.LC133:
	ASCII(.string	"sps with this pic_parameter_set_id havn't decode\n" )
	.zero	6
.LC135:
	ASCII(.string	"subsps with this seq_parameter_set_id = %d havn't decoded\n" )
	.zero	5
.LC138:
	ASCII(.string	"tmp slice  pps id = %d\n" )
.LC140:
	ASCII(.string	"MVC_PPSSPSCheckTmpId: pps with this pic_parameter_set_id = %d havn't decode\n" )
	.zero	3
.LC142:
	ASCII(.string	"MVC_PPSSPSCheckTmpId: seq_parameter_set_id out of range.\n" )
	.zero	6
.LC144:
	ASCII(.string	"MVC_PPSSPSCheckTmpId: sps with this pic_parameter_set_id = %d havn't decode\n" )
	.zero	3
.LC146:
	ASCII(.string	"new pic flag = %d\n" )
	.zero	5
.LC148:
	ASCII(.string	"SH: ref_pic_list_reordering_flag_l0" )
	.zero	4
.LC150:
	ASCII(.string	"SH: reordering_of_pic_nums_idc_l0" )
	.zero	6
.LC152:
	ASCII(.string	"reorder idc l0 = %d, g_SeErrFlag=%d\n" )
	.zero	3
.LC154:
	ASCII(.string	"SH: abs_diff_pic_num_minus1_l0" )
	.zero	1
.LC156:
	ASCII(.string	"abs_diff_pic_num_minus1_l0 = %d, g_SeErrFlag=%d\n" )
	.zero	7
.LC158:
	ASCII(.string	"SH: long_term_pic_idx_l0" )
	.zero	7
.LC160:
	ASCII(.string	"SH: abs_diff_view_idx_minus1_l0" )
.LC162:
	ASCII(.string	"num of idc l0 exceed\n" )
	.zero	2
.LC164:
	ASCII(.string	"SH: ref_pic_list_reordering_flag_l1" )
	.zero	4
.LC166:
	ASCII(.string	"SH: reordering_of_pic_nums_idc_l1" )
	.zero	6
.LC168:
	ASCII(.string	"reorder idc l1 = %d, g_SeErrFlag=%d\n" )
	.zero	3
.LC170:
	ASCII(.string	"SH: abs_diff_pic_num_minus1_l1" )
	.zero	1
.LC172:
	ASCII(.string	"abs_diff_pic_num_minus1_l1 = %d, g_SeErrFlag=%d\n" )
	.zero	7
.LC174:
	ASCII(.string	"SH: long_term_pic_idx_l1" )
	.zero	7
.LC176:
	ASCII(.string	"SH: abs_diff_view_idx_minus1_l1" )
.LC178:
	ASCII(.string	"num of idc l1 exceed\n" )
	.zero	2
.LC180:
	ASCII(.string	"SH: luma_log2_weight_denom" )
	.zero	5
.LC182:
	ASCII(.string	"SH: chroma_log2_weight_denom" )
	.zero	3
.LC184:
	ASCII(.string	"WP log2 exceed  and err flag = %d\n" )
	.zero	5
.LC186:
	ASCII(.string	"SH: luma_weight_flag_l0" )
.LC188:
	ASCII(.string	"SH: luma_weight_l0" )
	.zero	5
.LC190:
	ASCII(.string	"SH: luma_offset_l0" )
	.zero	5
.LC192:
	ASCII(.string	"SH: chroma_weight_flag_l0" )
	.zero	6
.LC194:
	ASCII(.string	"SH: chroma_weight_l0" )
	.zero	3
.LC196:
	ASCII(.string	"SH: chroma_offset_l0" )
	.zero	3
.LC198:
	ASCII(.string	"SH: luma_weight_flag_l1" )
.LC200:
	ASCII(.string	"SH: luma_weight_l1" )
	.zero	5
.LC202:
	ASCII(.string	"SH: luma_offset_l1" )
	.zero	5
.LC204:
	ASCII(.string	"SH: chroma_weight_flag_l1" )
	.zero	6
.LC206:
	ASCII(.string	"SH: chroma_weight_l1" )
	.zero	3
.LC208:
	ASCII(.string	"SH: chroma_offset_l1" )
	.zero	3
.LC210:
	ASCII(.string	"overflow MVC_MAX_MMCO_LEN\n" )
	.zero	5
.LC212:
	ASCII(.string	"SH: memory_management_control_operation" )
.LC214:
	ASCII(.string	"SH: difference_of_pic_nums_minus1" )
	.zero	6
.LC216:
	ASCII(.string	"SH: long_term_pic_num" )
	.zero	2
.LC218:
	ASCII(.string	"SH: long_term_frame_idx" )
.LC220:
	ASCII(.string	"SH: max_long_term_frame_idx_plus1" )
	.zero	6
.LC222:
	ASCII(.string	"mmco exceed 6\n" )
	.zero	1
.LC224:
	ASCII(.string	"SH: no_output_of_prior_pics_flag" )
	.zero	7
.LC226:
	ASCII(.string	"SH: long_term_reference_flag" )
	.zero	3
.LC228:
	ASCII(.string	"IDR and no_output_of_prior_pics_flag = %d\n" )
	.zero	5
.LC230:
	ASCII(.string	"SH: adaptive_ref_pic_marking_mode_flag" )
	.zero	1
.LC232:
	ASCII(.string	"adaptive_ref_pic_marking_mode_flag = %d\n" )
	.zero	7
.LC234:
	ASCII(.string	"MMCO para would save dec err\n" )
	.zero	2
.LC236:
	ASCII(.string	"mulitislice diff no_out_of_prior_pics_flag, used first.\n" )
	.zero	7
.LC238:
	ASCII(.string	"mulitislice diff long_term_reference_flag, used first.\n" )
.LC240:
	ASCII(.string	"mulitislice diff adaptive_ref_pic_marking_mode_flag, used first.\n" )
	.zero	6
.LC242:
	ASCII(.string	"TotalPicNum=%d, SliceNumInPic=%d, TotalNal=%d\n" )
	.zero	1
.LC244:
	ASCII(.string	"SH: pic_parameter_set_id" )
	.zero	7
.LC246:
	ASCII(.string	"pps id:%d in sliceheader err\n" )
	.zero	2
.LC248:
	ASCII(.string	"PPS or SPS of this slice not valid\n" )
	.zero	4
.LC250:
	ASCII(.string	"MVC_PPSSPSCheck failed, PPS or SPS of this slice not valid\n" )
	.zero	4
.LC252:
	ASCII(.string	"SH: frame_num" )
	.zero	2
.LC254:
	ASCII(.string	"IDR NAL but frame_num!=0.\n" )
	.zero	5
.LC256:
	ASCII(.string	"SH: field_pic_flag" )
	.zero	5
.LC258:
	ASCII(.string	"SH: bottom_field_flag" )
	.zero	2
.LC260:
	ASCII(.string	"field first_mb_in_slice bigger than pic size\n" )
	.zero	2
.LC262:
	ASCII(.string	"MVC_FRAME first_mb_in_slice bigger than pic size\n" )
	.zero	6
.LC264:
	ASCII(.string	"SH: idr_pic_id" )
	.zero	1
.LC266:
	ASCII(.string	"idr_pic_id bigger than 65535\n" )
	.zero	2
.LC268:
	ASCII(.string	"SH: pic_order_cnt_lsb" )
	.zero	2
.LC270:
	ASCII(.string	"SH: delta_pic_order_cnt_bottom" )
	.zero	1
.LC272:
	ASCII(.string	"SH: delta_pic_order_cnt[0]" )
	.zero	5
.LC274:
	ASCII(.string	"SH: delta_pic_order_cnt[1]" )
	.zero	5
.LC276:
	ASCII(.string	"SH: redundant_pic_cnt" )
	.zero	2
.LC278:
	ASCII(.string	"redundant pic not support.\n" )
	.zero	4
.LC281:
	ASCII(.string	"first_mb_in_slice in currslice small than prevslice in same pic\n" )
	.zero	7
.LC285:
	ASCII(.string	"SH: direct_spatial_mv_pred_flag" )
.LC287:
	ASCII(.string	"SH: num_ref_idx_override_flag" )
	.zero	2
.LC289:
	ASCII(.string	"SH: num_ref_idx_l0_active_minus1" )
	.zero	7
.LC291:
	ASCII(.string	"SH: num_ref_idx_l1_active_minus1" )
	.zero	7
.LC293:
	ASCII(.string	"MVC_FRAME num_ref_idx_lx_active_minus1 exceed\n" )
	.zero	1
.LC295:
	ASCII(.string	"field num_ref_idx_lx_active_minus1 exceed\n" )
	.zero	5
.LC297:
	ASCII(.string	"reordering para dec err\n" )
	.zero	7
.LC299:
	ASCII(.string	"apply_weights_flag=%d\n" )
	.zero	1
.LC301:
	ASCII(.string	"wpt dec err\n" )
	.zero	3
.LC303:
	ASCII(.string	"mark para dec err\n" )
	.zero	5
.LC305:
	ASCII(.string	"SH: cabac_init_idc" )
	.zero	5
.LC307:
	ASCII(.string	"cabac_init_idc bigger than 3\n" )
	.zero	2
.LC309:
	ASCII(.string	"SH: slice_qp_delta" )
	.zero	5
.LC311:
	ASCII(.string	"SH: disable_deblocking_filter_idc" )
	.zero	6
.LC313:
	ASCII(.string	"disable_deblocking_filter_idc dec err\n" )
	.zero	1
.LC315:
	ASCII(.string	"SH: slice_alpha_c0_offset_div2" )
	.zero	1
.LC317:
	ASCII(.string	"slice_alpha_c0_offset_div2 dec err\n" )
	.zero	4
.LC319:
	ASCII(.string	"SH: slice_beta_offset_div2" )
	.zero	5
.LC321:
	ASCII(.string	"slice_beta_offset_div2 dec err\n" )
.LC323:
	ASCII(.string	"pMvcCtx->pCurrNal->nal_bitoffset = %d; code_len_byte = %d; bitstream_length = %d; pMvcCtx->pCurrNal->nal_trail_zero_bit_num = %d\n" )
	.zero	6
.LC325:
	ASCII(.string	"stream[%d]: bitsoffset = %d; valid_bitlen = %d; bytespos = %x\n" )
	.zero	1
.LC327:
	ASCII(.string	"nal_segment = %d is not expected value\n" )
.LC329:
	ASCII(.string	"stream[%d]: bitsoffset = %d; valid_bitlen = %d\n" )
.LC332:
	ASCII(.string	"   : delta_sl   " )
	.zero	7
.LC336:
	ASCII(.string	"PPS: entropy_coding_mode_flag" )
	.zero	2
.LC338:
	ASCII(.string	"PPS: pic_order_present_flag" )
	.zero	4
.LC340:
	ASCII(.string	"PPS: num_slice_groups_minus1" )
	.zero	3
.LC342:
	ASCII(.string	"MVC_BASELINE stream with FMO, not support.\n" )
	.zero	4
.LC345:
	ASCII(.string	"PPS: num_ref_idx_l0_active_minus1" )
	.zero	6
.LC347:
	ASCII(.string	"num_ref_idx_l0_active_minus1 out of range.\n" )
	.zero	4
.LC349:
	ASCII(.string	"PPS: num_ref_idx_l1_active_minus1" )
	.zero	6
.LC351:
	ASCII(.string	"num_ref_idx_l1_active_minus1 out of range.\n" )
	.zero	4
.LC353:
	ASCII(.string	"PPS: weighted prediction flag" )
	.zero	2
.LC355:
	ASCII(.string	"PPS: weighted_bipred_idc" )
	.zero	7
.LC357:
	ASCII(.string	"weighted_bipred_idc out of range\n" )
	.zero	6
.LC359:
	ASCII(.string	"PPS: pic_init_qp_minus26" )
	.zero	7
.LC361:
	ASCII(.string	"PPS: pic_init_qs_minus26" )
	.zero	7
.LC363:
	ASCII(.string	"PPS: chroma_qp_index_offset" )
	.zero	4
.LC365:
	ASCII(.string	"PPS: deblocking_filter_control_present_flag" )
	.zero	4
.LC367:
	ASCII(.string	"PPS: constrained_intra_pred_flag" )
	.zero	7
.LC369:
	ASCII(.string	"PPS: redundant_pic_cnt_present_flag" )
	.zero	4
.LC371:
	ASCII(.string	"redundant pic not support when find redundant slice later, exit\n" )
	.zero	7
.LC373:
	ASCII(.string	"PPS: transform_8x8_mode_flag" )
	.zero	3
.LC375:
	ASCII(.string	"PPS: pic_scaling_matrix_present_flag" )
	.zero	3
.LC377:
	ASCII(.string	"PPS: second_chroma_qp_index_offset" )
	.zero	5
.LC379:
	ASCII(.string	"SPS: seq_scaling_list_present_flag" )
	.zero	5
.LC382:
	ASCII(.string	"PPS: pic_scaling_list_present_flag" )
	.zero	5
.LC384:
	ASCII(.string	"second_chroma_qp_index_offset out of range.\n" )
	.zero	3
.LC388:
	ASCII(.string	"PPS: pic_parameter_set_id" )
	.zero	6
.LC390:
	ASCII(.string	"pic_parameter_set_id(%d) out of range.\n" )
.LC393:
	ASCII(.string	"PPS: seq_parameter_set_id" )
	.zero	6
.LC395:
	ASCII(.string	"seq_parameter_set_id out of range.\n" )
	.zero	4
.LC398:
	ASCII(.string	"PPS decode error line: %d.\n" )
	.zero	4
.LC401:
	ASCII(.string	"VUI: aspect_ratio_info_present_flag" )
	.zero	4
.LC403:
	ASCII(.string	"VUI: aspect_ratio_idc" )
	.zero	2
.LC405:
	ASCII(.string	"VUI: sar_width" )
	.zero	1
.LC407:
	ASCII(.string	"VUI: sar_height" )
.LC409:
	ASCII(.string	"VUI: overscan_info_present_flag" )
.LC411:
	ASCII(.string	"VUI: overscan_appropriate_flag" )
	.zero	1
.LC413:
	ASCII(.string	"VUI: video_signal_type_present_flag" )
	.zero	4
.LC415:
	ASCII(.string	"VUI: video_format" )
	.zero	6
.LC417:
	ASCII(.string	"VUI: video_full_range_flag" )
	.zero	5
.LC419:
	ASCII(.string	"VUI: color_description_present_flag" )
	.zero	4
.LC421:
	ASCII(.string	"VUI: colour_primaries" )
	.zero	2
.LC423:
	ASCII(.string	"VUI: transfer_characteristics" )
	.zero	2
.LC425:
	ASCII(.string	"VUI: matrix_coefficients" )
	.zero	7
.LC427:
	ASCII(.string	"VUI: chroma_loc_info_present_flag" )
	.zero	6
.LC429:
	ASCII(.string	"VUI: chroma_sample_loc_type_top_field" )
	.zero	2
.LC431:
	ASCII(.string	"VUI: chroma_sample_loc_type_bottom_field" )
	.zero	7
.LC433:
	ASCII(.string	"VUI: timing_info_present_flag" )
	.zero	2
.LC435:
	ASCII(.string	"VUI: num_units_in_tick" )
	.zero	1
.LC437:
	ASCII(.string	"VUI: time_scale" )
.LC439:
	ASCII(.string	"VUI: fixed_frame_rate_flag" )
	.zero	5
.LC441:
	ASCII(.string	"VUI: nal_hrd_parameters_present_flag" )
	.zero	3
.LC443:
	ASCII(.string	"VUI: cpb_cnt_minus1" )
	.zero	4
.LC445:
	ASCII(.string	"VUI: bit_rate_scale" )
	.zero	4
.LC447:
	ASCII(.string	"VUI: cpb_size_scale" )
	.zero	4
.LC449:
	ASCII(.string	"hrd->cpb_cnt_minus1 out of range\n" )
	.zero	6
.LC451:
	ASCII(.string	"VUI: bit_rate_value_minus1" )
	.zero	5
.LC453:
	ASCII(.string	"VUI: cpb_size_value_minus1" )
	.zero	5
.LC455:
	ASCII(.string	"VUI: cbr_flag" )
	.zero	2
.LC457:
	ASCII(.string	"VUI: initial_cpb_removal_delay_length_minus1" )
	.zero	3
.LC459:
	ASCII(.string	"VUI: cpb_removal_delay_length_minus1" )
	.zero	3
.LC461:
	ASCII(.string	"VUI: dpb_output_delay_length_minus1" )
	.zero	4
.LC463:
	ASCII(.string	"VUI: time_offset_length" )
.LC465:
	ASCII(.string	"VUI: vcl_hrd_parameters_present_flag" )
	.zero	3
.LC467:
	ASCII(.string	"VUI: low_delay_hrd_flag" )
.LC469:
	ASCII(.string	"VUI: pic_struct_present_flag" )
	.zero	3
.LC471:
	ASCII(.string	"VUI: bitstream_restriction_flag" )
.LC473:
	ASCII(.string	"VUI: motion_vectors_over_pic_boundaries_flag" )
	.zero	3
.LC475:
	ASCII(.string	"VUI: max_bytes_per_pic_denom" )
	.zero	3
.LC477:
	ASCII(.string	"VUI: max_bits_per_mb_denom" )
	.zero	5
.LC479:
	ASCII(.string	"VUI: log2_max_mv_length_horizontal" )
	.zero	5
.LC481:
	ASCII(.string	"VUI: log2_max_mv_length_vertical" )
	.zero	7
.LC483:
	ASCII(.string	"VUI: num_reorder_frames" )
.LC485:
	ASCII(.string	"VUI: max_dec_frame_buffering" )
	.zero	3
.LC490:
	ASCII(.string	"dar=%d, final_dar=%d\n" )
	.zero	2
.LC496:
	ASCII(.string	"SPS: chroma_format_idc" )
	.zero	1
.LC498:
	ASCII(.string	"pSPS->chroma_format_idc out of range.\n" )
	.zero	1
.LC500:
	ASCII(.string	"SPS: bit_depth_luma_minus8" )
	.zero	5
.LC502:
	ASCII(.string	"bit_depth_luma_minus8 not equal 0.\n" )
	.zero	4
.LC504:
	ASCII(.string	"SPS: bit_depth_chroma_minus8" )
	.zero	3
.LC506:
	ASCII(.string	"bit_depth_chroma_minus8 not equal 0.\n" )
	.zero	2
.LC508:
	ASCII(.string	"SPS: qpprime_y_zero_trans_bypass_flag" )
	.zero	2
.LC510:
	ASCII(.string	"qpprime_y_zero_trans_bypass_flag not equal 0.\n" )
	.zero	1
.LC512:
	ASCII(.string	"SPS: seq_scaling_matrix_present_flag" )
	.zero	3
.LC516:
	ASCII(.string	"SPS: log2_max_frame_num_minus4" )
	.zero	1
.LC518:
	ASCII(.string	"pSPS->log2_max_frame_num_minus4 out of range.\n" )
	.zero	1
.LC520:
	ASCII(.string	"SPS: pic_order_cnt_type" )
.LC522:
	ASCII(.string	"pSPS->pic_order_cnt_type out of range.\n" )
.LC524:
	ASCII(.string	"SPS: log2_max_pic_order_cnt_lsb_minus4" )
	.zero	1
.LC526:
	ASCII(.string	"pSPS->log2_max_pic_order_cnt_lsb_minus4 out of range.\n" )
	.zero	1
.LC528:
	ASCII(.string	"SPS: delta_pic_order_always_zero_flag" )
	.zero	2
.LC530:
	ASCII(.string	"SPS: offset_for_non_ref_pic" )
	.zero	4
.LC532:
	ASCII(.string	"SPS: offset_for_top_to_bottom_field" )
	.zero	4
.LC534:
	ASCII(.string	"SPS: num_ref_frames_in_pic_order_cnt_cycle" )
	.zero	5
.LC536:
	ASCII(.string	"pSPS->num_ref_frames_in_pic_order_cnt_cycle out of range.\n" )
	.zero	5
.LC538:
	ASCII(.string	"SPS: offset_for_ref_frame[i]" )
	.zero	3
.LC540:
	ASCII(.string	"SPS: gaps_in_frame_num_value_allowed_flag" )
	.zero	6
.LC542:
	ASCII(.string	"SPS: pic_width_in_mbs_minus1" )
	.zero	3
.LC544:
	ASCII(.string	"pSPS->pic_width_in_mbs_minus1 out of range(=%d).\n" )
	.zero	6
.LC546:
	ASCII(.string	"SPS: pic_height_in_map_units_minus1" )
	.zero	4
.LC548:
	ASCII(.string	"SPS: frame_mbs_only_flag" )
	.zero	7
.LC550:
	ASCII(.string	"pSPS->pic_height_in_map_units_minus1 out of range.\n" )
	.zero	4
.LC552:
	ASCII(.string	"pic size too large.\n" )
	.zero	3
.LC555:
	ASCII(.string	"level_idc %d not support.\n" )
	.zero	5
.LC557:
	ASCII(.string	"dpb size according level : %d\n" )
	.zero	1
.LC559:
	ASCII(.string	"pSPS->num_ref_frames(%d) > DPB size(%d), try to select the reasonable one.\n" )
	.zero	4
.LC561:
	ASCII(.string	"SPS: direct_8x8_inference_flag" )
	.zero	1
.LC563:
	ASCII(.string	"SPS: frame_cropping_flag" )
	.zero	7
.LC565:
	ASCII(.string	"SPS: frame_cropping_rect_left_offset" )
	.zero	3
.LC567:
	ASCII(.string	"SPS: frame_cropping_rect_right_offset" )
	.zero	2
.LC569:
	ASCII(.string	"SPS: frame_cropping_rect_top_offset" )
	.zero	4
.LC571:
	ASCII(.string	"SPS: frame_cropping_rect_bottom_offset" )
	.zero	1
.LC573:
	ASCII(.string	"SPS: vui_parameters_present_flag" )
	.zero	7
.LC575:
	ASCII(.string	"max_dec_frame_buffering(%d) > MaxDpbSize(%d)\n" )
	.zero	2
.LC578:
	ASCII(.string	"SPS: num_ref_frames" )
	.zero	4
.LC580:
	ASCII(.string	"SPS: mb_adaptive_frame_field_flag" )
	.zero	6
.LC582:
	ASCII(.string	"SPS: profile_idc" )
	.zero	7
.LC584:
	ASCII(.string	"SPS: constrained_set0_flag" )
	.zero	5
.LC586:
	ASCII(.string	"SPS: constrained_set1_flag" )
	.zero	5
.LC588:
	ASCII(.string	"SPS: constrained_set2_flag" )
	.zero	5
.LC590:
	ASCII(.string	"SPS: constrained_set3_flag" )
	.zero	5
.LC592:
	ASCII(.string	"SPS: constrained_set4_flag" )
	.zero	5
.LC594:
	ASCII(.string	"SPS: constrained_set5_flag" )
	.zero	5
.LC596:
	ASCII(.string	"SPS: reserved_zero_2bits" )
	.zero	7
.LC598:
	ASCII(.string	"SPS: level_idc" )
	.zero	1
.LC600:
	ASCII(.string	"SPS: seq_parameter_set_id" )
	.zero	6
.LC604:
	ASCII(.string	"MVC_BASELINE stream, try to decode, exit when FMO occurred.\n" )
	.zero	3
.LC606:
	ASCII(.string	"MVC_EXTENDED stream, try to decode, exit when datapartition occurred.\n" )
	.zero	1
.LC608:
	ASCII(.string	"others High profile stream, try to decode, exit when high profile not support occurred.\n" )
	.zero	7
.LC610:
	ASCII(.string	"profile_idc = %5d error, try to decode as main profile.\n" )
	.zero	7
.LC612:
	ASCII(.string	"level_idc = %5d error, try to decode as level_idc %d.\n" )
	.zero	1
.LC615:
	ASCII(.string	"Line %d: SPS[%d] decode error.\n" )
.LC621:
	ASCII(.string	"SUBSPS: num_views_minus1" )
	.zero	7
.LC623:
	ASCII(.string	"pSubsps->num_views_minus1(%d) out of range.\n" )
	.zero	3
.LC625:
	ASCII(.string	"SUBSPS: view_id[]" )
	.zero	6
.LC627:
	ASCII(.string	"SUBSPS: num_anchor_refs_l0[]" )
	.zero	3
.LC629:
	ASCII(.string	"SUBSPS: num_anchor_refs_l1[]" )
	.zero	3
.LC631:
	ASCII(.string	"pSubsps->num_anchor_refs_l0(%d) out of range.\n" )
	.zero	1
.LC633:
	ASCII(.string	"SUBSPS: anchor_ref_l0[][]" )
	.zero	6
.LC635:
	ASCII(.string	"pSubsps->num_anchor_refs_l1(%d) out of range.\n" )
	.zero	1
.LC637:
	ASCII(.string	"SUBSPS: anchor_ref_l1[][]" )
	.zero	6
.LC639:
	ASCII(.string	"SUBSPS: num_level_values_signalled_minus1" )
	.zero	6
.LC641:
	ASCII(.string	"SUBSPS: num_non_anchor_refs_l0[]" )
	.zero	7
.LC643:
	ASCII(.string	"SUBSPS: num_non_anchor_refs_l1[]" )
	.zero	7
.LC645:
	ASCII(.string	"pSubsps->num_non_anchor_refs_l0(%d) out of range.\n" )
	.zero	5
.LC647:
	ASCII(.string	"SUBSPS: non_anchor_ref_l0[][]" )
	.zero	2
.LC649:
	ASCII(.string	"pSubsps->num_non_anchor_refs_l1(%d) out of range.\n" )
	.zero	5
.LC651:
	ASCII(.string	"SUBSPS: non_anchor_ref_l1[][]" )
	.zero	2
.LC653:
	ASCII(.string	"num_level(%d) out of range.\n" )
	.zero	3
.LC655:
	ASCII(.string	"SUBSPS: level_idc[]" )
	.zero	4
.LC657:
	ASCII(.string	"SUBSPS: num_applicable_ops_minus1" )
	.zero	6
.LC659:
	ASCII(.string	"num_ops(%d) out of range.\n" )
	.zero	5
.LC661:
	ASCII(.string	"SUBSPS: applicable_op_temporal_id[][]" )
	.zero	2
.LC663:
	ASCII(.string	"SUBSPS: applicable_op_num_target_views_minus1[][]" )
	.zero	6
.LC665:
	ASCII(.string	"SUBSPS: applicable_op_num_views_minus1" )
	.zero	1
.LC667:
	ASCII(.string	"applicable_op_num_target_views(%d) out of range.\n" )
	.zero	6
.LC669:
	ASCII(.string	"SUBSPS: applicable_op_target_view_id[][][]" )
	.zero	5
.LC671:
	ASCII(.string	"pSubsps->applicable_op_num_views_minus1(%d) out of range.\n" )
	.zero	5
.LC673:
	ASCII(.string	"MVC VUI: vui_mvc_num_ops_minus1" )
.LC675:
	ASCII(.string	"vui_mvc_num_ops_minus1 %d out of range\n" )
.LC677:
	ASCII(.string	"MVC VUI: vui_mvc_temporal_id[]" )
	.zero	1
.LC679:
	ASCII(.string	"SUBSPS VUI: vui_mvc_num_target_output_views_minus1[]" )
	.zero	3
.LC681:
	ASCII(.string	"MVC VUI: vui_mvc_timing_info_present_flag" )
	.zero	6
.LC683:
	ASCII(.string	"MVC VUI: vui_mvc_view_id[][]" )
	.zero	3
.LC685:
	ASCII(.string	"MVC VUI: vui_mvc_num_units_in_tick" )
	.zero	5
.LC687:
	ASCII(.string	"MVC VUI: vui_mvc_time_scale[]" )
	.zero	2
.LC689:
	ASCII(.string	"MVC VUI: vui_mvc_fixed_frame_rate_flag" )
	.zero	1
.LC691:
	ASCII(.string	"MVC VUI: vui_mvc_nal_hrd_parameters_present_flag[]" )
	.zero	5
.LC693:
	ASCII(.string	"MVC VUI: cpb_cnt_minus1" )
.LC695:
	ASCII(.string	"MVC VUI: bit_rate_scale" )
.LC697:
	ASCII(.string	"MVC VUI: cpb_size_scale" )
.LC700:
	ASCII(.string	"MVC VUI: bit_rate_value_minus1" )
	.zero	1
.LC702:
	ASCII(.string	"MVC VUI: cpb_size_value_minus1" )
	.zero	1
.LC704:
	ASCII(.string	"MVC VUI: cbr_flag" )
	.zero	6
.LC706:
	ASCII(.string	"MVC VUI: initial_cpb_removal_delay_length_minus1" )
	.zero	7
.LC708:
	ASCII(.string	"MVC VUI: cpb_removal_delay_length_minus1" )
	.zero	7
.LC710:
	ASCII(.string	"MVC VUI: dpb_output_delay_length_minus1" )
.LC712:
	ASCII(.string	"MVC VUI: time_offset_length" )
	.zero	4
.LC714:
	ASCII(.string	"MVC VUI: vui_mvc_vcl_hrd_parameters_present_flag[]" )
	.zero	5
.LC716:
	ASCII(.string	"MVC VUI: vui_mvc_low_delay_hrd_flag[]" )
	.zero	2
.LC718:
	ASCII(.string	"MVC VUI: vui_mvc_pic_struct_present_flag[]" )
	.zero	5
.LC720:
	ASCII(.string	"SUBSPS: profile_idc" )
	.zero	4
.LC722:
	ASCII(.string	"SUBSPS: constrained_set0_flag" )
	.zero	2
.LC724:
	ASCII(.string	"SUBSPS: constrained_set1_flag" )
	.zero	2
.LC726:
	ASCII(.string	"SUBSPS: constrained_set2_flag" )
	.zero	2
.LC728:
	ASCII(.string	"SUBSPS: constrained_set3_flag" )
	.zero	2
.LC730:
	ASCII(.string	"SUBSPS: constrained_set4_flag" )
	.zero	2
.LC732:
	ASCII(.string	"SUBSPS: constrained_set5_flag" )
	.zero	2
.LC734:
	ASCII(.string	"SUBSPS: reserved_zero_2bits" )
	.zero	4
.LC736:
	ASCII(.string	"SUBSPS: level_idc" )
	.zero	6
.LC738:
	ASCII(.string	"SUBSPS: seq_parameter_set_id" )
	.zero	3
.LC745:
	ASCII(.string	"profile_idc = %5d error\n" )
	.zero	7
.LC747:
	ASCII(.string	"level_idc = %5d error, try to decode as level_idc 41.\n" )
	.zero	1
.LC749:
	ASCII(.string	"SUBSPS decode error.\n" )
	.zero	2
.LC751:
	ASCII(.string	"SUBSPS: bit_equal_to_one" )
	.zero	7
.LC753:
	ASCII(.string	"SUBSPS Mvc Ext decode error.\n" )
	.zero	2
.LC755:
	ASCII(.string	"SUBSPS: mvc_vui_parameters_present_flag" )
.LC757:
	ASCII(.string	"SUBSPS Mvc Vui Ext decode error.\n" )
	.zero	6
.LC759:
	ASCII(.string	"SEI: frame_packing_arrangement_id" )
	.zero	6
.LC761:
	ASCII(.string	"SEI: frame_packing_arrangement_cancel_flag" )
	.zero	5
.LC763:
	ASCII(.string	"SEI: frame_packing_arrangement_type" )
	.zero	4
.LC765:
	ASCII(.string	"SEI: quincunx_sampling_flag" )
	.zero	4
.LC767:
	ASCII(.string	"SEI: content_interpretation_type" )
	.zero	7
.LC769:
	ASCII(.string	"SEI: spatial_flipping_flag" )
	.zero	5
.LC771:
	ASCII(.string	"SEI: frame0_flipped_flag" )
	.zero	7
.LC773:
	ASCII(.string	"SEI: field_views_flag" )
	.zero	2
.LC775:
	ASCII(.string	"SEI: current_frame_is_frame0_flag" )
	.zero	6
.LC777:
	ASCII(.string	"SEI: frame0_self_contained_flag" )
.LC779:
	ASCII(.string	"SEI: frame1_self_contained_flag" )
.LC781:
	ASCII(.string	"SEI: frame0_grid_position_x" )
	.zero	4
.LC783:
	ASCII(.string	"SEI: frame0_grid_position_y" )
	.zero	4
.LC785:
	ASCII(.string	"SEI: frame1_grid_position_x" )
	.zero	4
.LC787:
	ASCII(.string	"SEI: frame1_grid_position_y" )
	.zero	4
.LC789:
	ASCII(.string	"SEI: frame_packing_arrangement_reserved_byte" )
	.zero	3
.LC791:
	ASCII(.string	"SEI: frame_packing_arrangement_repetition_period" )
	.zero	7
.LC793:
	ASCII(.string	"SEI: frame_packing_arrangement_extension_flag" )
	.zero	2
.LC795:
	ASCII(.string	"DecPicTimingSEI but SPS is invalid.\n" )
	.zero	3
.LC797:
	ASCII(.string	"SEI: cpb_removal_delay" )
	.zero	1
.LC799:
	ASCII(.string	"SEI: dpb_output_delay" )
	.zero	2
.LC801:
	ASCII(.string	"pic_struct" )
	.zero	5
.LC803:
	ASCII(.string	"SEI nal dec payload type err\n" )
	.zero	2
.LC805:
	ASCII(.string	"SEI nal dec payloadSize err\n" )
	.zero	3
.LC808:
	ASCII(.string	"MVC_SEI_BUFFERING_PERIOD,offset = %d,payload_size = %d \n" )
	.zero	7
.LC811:
	ASCII(.string	"MVC_PassBytes err [%s][%d]\n" )
	.zero	4
.LC813:
	ASCII(.string	"MVC_SEI_PIC_TIMING,offset = %d,payload_size = %d \n" )
	.zero	5
.LC815:
	ASCII(.string	"MVC_SEI_PAN_SCAN_RECT,offset = %d,payload_size = %d \n" )
	.zero	2
.LC817:
	ASCII(.string	"MVC_SEI_FILLER_PAYLOAD,offset = %d,payload_size = %d \n" )
	.zero	1
.LC819:
	ASCII(.string	"MVC_SEI_USER_DATA_REGISTERED_ITU_T_T35, offset = %d, payload_size = %d \n" )
	.zero	7
.LC821:
	ASCII(.string	"MVC_SEI_USER_DATA_UNREGISTERED, offset = %d, payload_size = %d \n" )
	.zero	7
.LC823:
	ASCII(.string	"SEI: itu_t_t35_country_code" )
	.zero	4
.LC825:
	ASCII(.string	"SEI: itu_t_t35_country_code_extension_byte" )
	.zero	5
.LC827:
	ASCII(.string	"SEI: itu_t_t35_provider_code" )
	.zero	3
.LC830:
	ASCII(.string	"cann't dec usrdata\n" )
	.zero	4
.LC832:
	ASCII(.string	"MVC_SEI_RECOVERY_POINT,offset = %d,payload_size = %d \n" )
	.zero	1
.LC834:
	ASCII(.string	"MVC_SEI_DEC_REF_PIC_MARKING_REPETITION,offset = %d,payload_size = %d \n" )
	.zero	1
.LC836:
	ASCII(.string	"MVC_SEI_SPARE_PIC,offset = %d,payload_size = %d \n" )
	.zero	6
.LC838:
	ASCII(.string	"MVC_SEI_SCENE_INFO,offset = %d,payload_size = %d \n" )
	.zero	5
.LC840:
	ASCII(.string	"MVC_SEI_SUB_SEQ_INFO,offset = %d,payload_size = %d \n" )
	.zero	3
.LC842:
	ASCII(.string	"MVC_SEI_SUB_SEQ_LAYER_CHARACTERISTICS,offset = %d,payload_size = %d \n" )
	.zero	2
.LC844:
	ASCII(.string	"MVC_SEI_SUB_SEQ_CHARACTERISTICS,offset = %d,payload_size = %d \n" )
.LC846:
	ASCII(.string	"MVC_SEI_FULL_FRAME_FREEZE,offset = %d,payload_size = %d \n" )
	.zero	6
.LC848:
	ASCII(.string	"MVC_SEI_FULL_FRAME_FREEZE_RELEASE,offset = %d,payload_size = %d \n" )
	.zero	6
.LC850:
	ASCII(.string	"MVC_SEI_FULL_FRAME_SNAPSHOT,offset = %d,payload_size = %d \n" )
	.zero	4
.LC852:
	ASCII(.string	"MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START,offset = %d,payload_size = %d \n" )
	.zero	3
.LC854:
	ASCII(.string	"MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END,offset = %d,payload_size = %d \n" )
	.zero	5
.LC856:
	ASCII(.string	"MVC_SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,offset = %d,payload_size = %d \n" )
	.zero	5
.LC858:
	ASCII(.string	"MVC_SEI_FILM_GRAIN_CHARACTERISTICS,offset = %d,payload_size = %d \n" )
	.zero	5
.LC860:
	ASCII(.string	"MVC_SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,offset = %d,payload_size = %d \n" )
	.zero	3
.LC862:
	ASCII(.string	"MVC_SEI_STEREO_VIDEO_INFO,offset = %d,payload_size = %d \n" )
	.zero	6
.LC864:
	ASCII(.string	"too much usrdat, cann't dec SEI\n" )
	.zero	7
.LC868:
	ASCII(.string	"\n mvc the stream is error,len:%d\n" )
	.zero	6
.LC873:
	ASCII(.string	"\nSlice nal or IDR nal = %d\n" )
	.zero	4
.LC875:
	ASCII(.string	"sliceheader dec err\n" )
	.zero	3
.LC878:
	ASCII(.string	"MVC_InquireSliceProperty mvc flag %d is wrong\n" )
	.zero	1
.LC880:
	ASCII(.string	"MvcDec destroy\n" )
.LC882:
	ASCII(.string	"MVC recycle image\n" )
	.zero	5
.LC884:
	ASCII(.string	"pH264Ctx is %#x, pFrameStore is %#x\n" )
	.zero	3
.LC886:
	ASCII(.string	"line: %d, pstLogicFsImage is NULL\n" )
	.zero	5
.LC888:
	ASCII(.string	"InsertVO err:%d, MVC_ClearAll\n" )
	.zero	1
.LC890:
	ASCII(.string	"mvc.c line %d: MVC_FRAME para err(ret=%d), recycle image self\n" )
	.zero	1
.LC894:
	ASCII(.string	"[%s][%d] MVC_OutputFrmToVO return %d !\n" )
.LC897:
	ASCII(.string	"[%s][%d] MVC_OutputFrmToVO return %d\n" )
	.zero	2
.LC901:
	ASCII(.string	"%s %d MVC_OK != MVC_FlushDPB!!\n" )
.LC903:
	ASCII(.string	"%s %d MVC_OK != MVC_InitDPB!!\n" )
	.zero	1
.LC905:
	ASCII(.string	"----------------- MVC_IMODE -> %d ------------------\n" )
	.zero	2
.LC907:
	ASCII(.string	"Discard this B(poc=%d) befor P, is_ref_idc=%d.\n" )
.LC909:
	ASCII(.string	"***** VDM start, TotalPicNum=%d, pMvcCtx->CurrPic.structure=%d.\n" )
	.zero	7
.LC913:
	ASCII(.string	"MVC_DecVDM fail [%s][%d]\n" )
	.zero	6
.LC915:
	ASCII(.string	"\n receive streambuff=%p; phy_addr=0x%x; bitstream_length=%d\n" )
	.zero	3
.LC917:
	ASCII(.string	"nal_releaMVC_SE_ERR\n" )
	.zero	3
.LC919:
	ASCII(.string	"cann't find slot for current nal\n" )
	.zero	6
.LC922:
	ASCII(.string	"receive a zero packet\n" )
	.zero	1
.LC924:
	ASCII(.string	"clear all dec para\n" )
	.zero	4
.LC930:
	ASCII(.string	"line %d, get APC error, ret %d\n" )
.LC932:
	ASCII(.string	"InserFrmInDPB: cur pic struct = %d!\n" )
	.zero	3
.LC934:
	ASCII(.string	"line: %d, pImage is NULL!\n" )
	.zero	5
.LC936:
	ASCII(.string	"FS_ALLOC_ERR, MVC_ClearAll\n" )
	.zero	4
.LC939:
	ASCII(.string	"Can not new logic fs! MVC_ClearAll\n" )
	.zero	4
.LC943:
	ASCII(.string	"%s %d NULL== pMvcCtx->SPS || NULL == pMvcCtx->PPS || NULL == pMvcCtx->DecSlicePara!!\n" )
	.zero	2
.LC945:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC947:
	ASCII(.string	"CurrPic.state is 'MVC_EMPTY'\n" )
	.zero	2
.LC949:
	ASCII(.string	"MVC_StorePicInDpb return(%d) from L%d\n" )
	.zero	1
.LC951:
	ASCII(.string	"Is IDR, but init DPB failed!\n" )
	.zero	2
.LC953:
	ASCII(.string	"Is IDR, but FlushDPB failed!\n" )
	.zero	2
.LC955:
	ASCII(.string	"MVC_DirectOutput mode\n" )
	.zero	1
.LC957:
	ASCII(.string	"line %d, REPORT_IFRAME_ERR\n" )
	.zero	4
.LC960:
	ASCII(.string	"line %d, return %d\n" )
	.zero	4
.LC962:
	ASCII(.string	"err(%d) > ref_thr(%d)\n" )
	.zero	1
.LC964:
	ASCII(.string	"MVC_Marking return %d\n" )
	.zero	1
.LC966:
	ASCII(.string	"line %d: pMvcCtx->DPB.size = %d, ref %d, ltref %d\n" )
	.zero	5
.LC968:
	ASCII(.string	"line %d, MVC_GetMinPOC failed\n" )
	.zero	1
.LC971:
	ASCII(.string	"%s: pos(%d) = pre_pos, force return.\n" )
	.zero	2
.LC973:
	ASCII(.string	"GAP found while DPB is MVC_EMPTY!\n" )
	.zero	5
.LC975:
	ASCII(.string	"MVC_FRAME num gap try to fill it \n" )
	.zero	5
.LC977:
	ASCII(.string	"CurrFrameNum = %d  UnusedShortTermFrameNum = %d\n" )
	.zero	7
.LC979:
	ASCII(.string	"cann't allocate MVC_FRAME store when gap find\n" )
	.zero	1
.LC981:
	ASCII(.string	"allocate MVC_FRAME store when gap find\n" )
.LC983:
	ASCII(.string	"line %d: store gap pic err, ret=%d\n" )
	.zero	4
.LC987:
	ASCII(.string	"line %d: flush dpb return %d\n" )
	.zero	2
.LC989:
	ASCII(.string	"line %d: init dpb return %d\n" )
	.zero	3
.LC991:
	ASCII(.string	"resolution error, the CAP_LEVEL_USER_DEFINE_WITH_OPTION channel with s32ReRangeEn == 0 can't support such bitstream.\n" )
	.zero	2
.LC993:
	ASCII(.string	"s32MaxRefFrameNum < pMvcCtx->DPB.size, the CAP_LEVEL_USER_DEFINE_WITH_OPTION channel can't support such bitstream.\n" )
	.zero	4
.LC995:
	ASCII(.string	"mem arrange err, MVC_ClearAll\n" )
	.zero	1
.LC997:
	ASCII(.string	"get back frm\n" )
	.zero	2
.LC999:
	ASCII(.string	"line %d: MVC_FRAME gap(=%d) > dpb size(=%d)\n" )
	.zero	3
.LC1001:
	ASCII(.string	"MVC_FRAME num gap don't allowed but gap find\n" )
	.zero	2
.LC1003:
	ASCII(.string	"line %d: frame num(%d/%d) find gap in NON-I slice but here gap is not allowed\n" )
	.zero	1
.LC1005:
	ASCII(.string	"line %d: dec gap failed\n" )
	.zero	7
.LC1007:
	ASCII(.string	"line %d: alloc framestore failed\n" )
	.zero	6
.LC1009:
	ASCII(.string	"dec_pts: %lld\n" )
	.zero	1
.LC1011:
	ASCII(.string	"dec_usertag: %lld\n" )
	.zero	5
.LC1013:
	ASCII(.string	"line %d: CurrPic.frame_store is NULL\n" )
	.zero	2
.LC1018:
	ASCII(.string	"Start Reason: SliceParaNum, MaxBytesReceived = %d, %d(thr=%d)\n" )
	.zero	1
.LC1020:
	ASCII(.string	"Too many slice or bitstream, err!\n" )
	.zero	5
.LC1022:
	ASCII(.string	"Start Reason: new_pic_flag\n" )
	.zero	4
.LC1024:
	ASCII(.string	"init pic err, find next recover point or next valid sps, pps, or exit\n" )
	.zero	1
.LC1026:
	ASCII(.string	"MVC_DecList error, ret=%d\n" )
	.zero	5
.LC1028:
	ASCII(.string	"dec list err.\n" )
	.zero	1
.LC1030:
	ASCII(.string	"pMvcCtx->TotalNal = %d, type:%d\n" )
	.zero	7
.LC1032:
	ASCII(.string	"stop i want\n" )
	.zero	3
.LC1035:
	ASCII(.string	"***** NAL: IDR/Slice, nal_unit_type=%d, TotalSlice=%d\n" )
	.zero	1
.LC1037:
	ASCII(.string	"*******TotalPicNum=%d********\n" )
	.zero	1
.LC1040:
	ASCII(.string	"***** NAL: PPS, nal_unit_type=%d, TotalPPS=%d\n" )
	.zero	1
.LC1042:
	ASCII(.string	"PPS decode error.\n" )
	.zero	5
.LC1044:
	ASCII(.string	"***** NAL: SPS, nal_unit_type=%d, TotalSPS=%d\n" )
	.zero	1
.LC1046:
	ASCII(.string	"SPS decode error.\n" )
	.zero	5
.LC1048:
	ASCII(.string	"***** NAL: SEI, nal_unit_type=%d\n" )
	.zero	6
.LC1050:
	ASCII(.string	"SEI decode error.\n" )
	.zero	5
.LC1052:
	ASCII(.string	"***** NAL: AUD, nal_unit_type=%d\n" )
	.zero	6
.LC1054:
	ASCII(.string	"***** NAL: EOSEQ, nal_unit_type=%d\n" )
	.zero	4
.LC1056:
	ASCII(.string	"***** NAL: FILL, nal_unit_type=%d\n" )
	.zero	5
.LC1058:
	ASCII(.string	"***** NAL: EOSTREAM, nal_unit_type=%d\n" )
	.zero	1
.LC1060:
	ASCII(.string	"***** NAL: SPSEXT, nal_unit_type=%d\n" )
	.zero	3
.LC1062:
	ASCII(.string	"***** NAL: PREFIX, nal_unit_type=%d\n" )
	.zero	3
.LC1064:
	ASCII(.string	"***** NAL: SUBSPS, nal_unit_type=%d\n" )
	.zero	3
.LC1067:
	ASCII(.string	"***** NAL: AUX, nal_unit_type=%d\n" )
	.zero	6
.LC1069:
	ASCII(.string	"***** NAL: EOPIC, nal_unit_type=%d\n" )
	.zero	4
.LC1072:
	ASCII(.string	"***** NAL: UNSUPPORT, nal_unit_type=%d,nalu header:%x\n" )
	.zero	1
.LC1074:
	ASCII(.string	"nal_header != 0x00000100 not support.\n" )
	.zero	1
.LC1081:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC1084:
	ASCII(.string	"MVC_FindTrailZeros ERR\n" )
.LC1086:
	ASCII(.string	"store pic err, ret = %d\n" )
	.zero	7
.LC1088:
	ASCII(.string	"MVC_IMODE nal store pic err, ret = %d\n" )
	.zero	1
.LC1090:
	ASCII(.string	"MVC_IMODE nal flush dpb err, ret = %d\n" )
	.zero	1
.LC1092:
	ASCII(.string	"cann't find FrameStore\n" )
.LC1094:
	ASCII(.string	"========== MVC FrameStore state(is_used, is_in_dpb, MVC_IsOutDPB) ========\n" )
	.zero	4
.LC1096:
	ASCII(.string	"ReadImgNum = %d, NewImgNum = %d\n" )
	.zero	7
.LC1098:
	ASCII(.string	"%02d: %d %d %d\n" )
.LC1100:
	ASCII(.string	"FrameStore leak, MVC_ClearAll\n" )
	.bss
	.align	3
.LANCHOR2 = . + 0
.LANCHOR3 = . + 4352
	.type	pps_tmp.12896, %object
	.size	pps_tmp.12896, 2240
pps_tmp.12896:
	.zero	2240
	.type	sps_tmp.13032, %object
	.size	sps_tmp.13032, 3992
sps_tmp.13032:
	.zero	3992
	.type	MvcTmpBuf, %object
	.size	MvcTmpBuf, 68
MvcTmpBuf:
	.zero	68
	.type	cnt.11849, %object
	.size	cnt.11849, 4
cnt.11849:
	.zero	4
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
