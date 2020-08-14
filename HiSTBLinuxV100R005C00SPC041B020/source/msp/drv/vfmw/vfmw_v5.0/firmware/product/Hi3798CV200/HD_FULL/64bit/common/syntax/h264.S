#include "hi_asm_define.h"
	.arch armv8-a+fp+simd
	.file	"h264.c"
	.global	_mcount
	.text
	.align	2
	.p2align 3,,7
	.global	IsSTRefFlg
	.type	IsSTRefFlg, %function
IsSTRefFlg:
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
	.size	IsSTRefFlg, .-IsSTRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsLTRefFlg
	.type	IsLTRefFlg, %function
IsLTRefFlg:
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
	.size	IsLTRefFlg, .-IsLTRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_pic_by_pic_num_desc
	.type	compare_pic_by_pic_num_desc, %function
compare_pic_by_pic_num_desc:
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
	ldr	w1, [x0, 8]
	ldr	w2, [x2, 8]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_pic_by_pic_num_desc, .-compare_pic_by_pic_num_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_pic_by_lt_pic_num_asc
	.type	compare_pic_by_lt_pic_num_asc, %function
compare_pic_by_lt_pic_num_asc:
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
	ldr	w1, [x0, 4]
	ldr	w2, [x2, 4]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_pic_by_lt_pic_num_asc, .-compare_pic_by_lt_pic_num_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_fs_by_frame_num_desc
	.type	compare_fs_by_frame_num_desc, %function
compare_fs_by_frame_num_desc:
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
	ldr	w1, [x0, 640]
	ldr	w2, [x2, 640]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_fs_by_frame_num_desc, .-compare_fs_by_frame_num_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_fs_by_lt_pic_idx_asc
	.type	compare_fs_by_lt_pic_idx_asc, %function
compare_fs_by_lt_pic_idx_asc:
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
	ldr	w1, [x0, 644]
	ldr	w2, [x2, 644]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, hi
	csel	w0, w0, w3, cs
	ret
	.size	compare_fs_by_lt_pic_idx_asc, .-compare_fs_by_lt_pic_idx_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_pic_by_poc_asc
	.type	compare_pic_by_poc_asc, %function
compare_pic_by_poc_asc:
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
	ldr	w1, [x0, 12]
	ldr	w2, [x2, 12]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_pic_by_poc_asc, .-compare_pic_by_poc_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_pic_by_poc_desc
	.type	compare_pic_by_poc_desc, %function
compare_pic_by_poc_desc:
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
	ldr	w1, [x0, 12]
	ldr	w2, [x2, 12]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_pic_by_poc_desc, .-compare_pic_by_poc_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_fs_by_poc_asc
	.type	compare_fs_by_poc_asc, %function
compare_fs_by_poc_asc:
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
	ldr	w1, [x0, 648]
	ldr	w2, [x2, 648]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	cset	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_fs_by_poc_asc, .-compare_fs_by_poc_asc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	compare_fs_by_poc_desc
	.type	compare_fs_by_poc_desc, %function
compare_fs_by_poc_desc:
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
	ldr	w1, [x0, 648]
	ldr	w2, [x2, 648]
	ldp	x29, x30, [sp], 32
	cmp	w2, w1
	csetm	w0, gt
	csel	w0, w0, w3, ge
	ret
	.size	compare_fs_by_poc_desc, .-compare_fs_by_poc_desc
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	SetFrmRepeatCount.part.1, %function
SetFrmRepeatCount.part.1:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	add	x3, x19, 65536
	add	x19, x19, 24576
	ldp	w0, w2, [x3, 132]
	ldr	w1, [x19, 3712]
	lsr	w0, w0, 1
	add	w2, w1, w2, lsr 1
	add	w4, w2, 1
	cmp	w0, w4
	bcc	.L28
	sub	w0, w0, #1
	sub	w0, w0, w2
	cmp	w0, 1
	bls	.L29
	mov	w0, 2
	str	w0, [x20, 24]
	ldr	w0, [x3, 136]
	add	w0, w0, 4
	str	w0, [x3, 136]
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
	ldr	w0, [x3, 136]
	add	w0, w0, 2
	str	w0, [x3, 136]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	SetFrmRepeatCount.part.1, .-SetFrmRepeatCount.part.1
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	DumpList.part.5, %function
DumpList.part.5:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x24, x20, 20480
	mov	w21, 0
	bl	_mcount
	ldr	x23, .LC3
	add	x25, x24, 1628
.L33:
	ldr	w0, [x25]
	cbz	w0, .L37
	uxtw	x0, w21
	mov	w19, 0
	lsl	x22, x0, 5
	add	x22, x22, x0
	.p2align 2
.L34:
	add	x4, x22, x19, uxtw
	mov	w3, w19
	add	x4, x4, 3588
	mov	w2, w21
	mov	x1, x23
	mov	w0, 13
	add	w19, w19, 1
	ldr	x4, [x20, x4, lsl 3]
	ldr	x4, [x4, 32]
	ldr	w5, [x4, 648]
	ldr	w4, [x4, 636]
	bl	dprint_vfmw
	ldr	w0, [x25]
	cmp	w19, w0
	bcc	.L34
.L37:
	add	w21, w21, 1
	add	x25, x25, 4
	cmp	w21, 2
	bne	.L33
	ldr	w0, [x24, 1628]
	cbnz	w0, .L65
.L38:
	ldr	w0, [x24, 1632]
	cbnz	w0, .L66
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L66:
	ldr	x1, .LC15
	mov	w0, 13
	mov	w19, 0
	bl	dprint_vfmw
	ldr	w0, [x24, 1632]
	cbz	w0, .L53
	ldr	x23, .LC9
	ldr	x21, .LC13
	ldr	x22, .LC11
	b	.L58
	.p2align 3
.L67:
	cmp	w0, 2
	cbz	w0, .L50
	mov	x1, x21
	mov	w0, 13
	bne	.L48
	ldr	x2, [x2, 32]
	ldr	w2, [x2, 648]
	bl	dprint_vfmw
.L48:
	ldr	w0, [x24, 1632]
	add	w19, w19, 1
	cmp	w19, w0
	bcs	.L53
.L58:
	add	x0, x20, x19, uxtw 3
	ldr	x2, [x0, 28968]
	ldrb	w0, [x2]
	cmp	w0, 1
	bne	.L67
	ldr	x2, [x2, 32]
	mov	x1, x22
	mov	w0, 13
	add	w19, w19, 1
	ldr	w2, [x2, 648]
	bl	dprint_vfmw
	ldr	w0, [x24, 1632]
	cmp	w19, w0
	bcc	.L58
.L53:
	ldr	x1, .LC7
	mov	w0, 13
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L50:
	ldr	x2, [x2, 32]
	mov	x1, x23
	mov	w0, 13
	ldr	w2, [x2, 648]
	bl	dprint_vfmw
	b	.L48
	.p2align 3
.L65:
	ldr	x1, .LC5
	mov	w0, 13
	mov	w19, 0
	bl	dprint_vfmw
	ldr	w0, [x24, 1628]
	cbz	w0, .L45
	ldr	x23, .LC9
	ldr	x21, .LC13
	ldr	x22, .LC11
	b	.L59
	.p2align 3
.L68:
	cmp	w0, 2
	cbz	w0, .L42
	mov	x1, x21
	mov	w0, 13
	bne	.L40
	ldr	x2, [x2, 32]
	ldr	w2, [x2, 648]
	bl	dprint_vfmw
.L40:
	ldr	w0, [x24, 1628]
	add	w19, w19, 1
	cmp	w19, w0
	bcs	.L45
.L59:
	add	x0, x20, x19, uxtw 3
	ldr	x2, [x0, 28704]
	ldrb	w0, [x2]
	cmp	w0, 1
	bne	.L68
	ldr	x2, [x2, 32]
	mov	x1, x22
	mov	w0, 13
	add	w19, w19, 1
	ldr	w2, [x2, 648]
	bl	dprint_vfmw
	ldr	w0, [x24, 1628]
	cmp	w19, w0
	bcc	.L59
.L45:
	ldr	x1, .LC7
	mov	w0, 13
	bl	dprint_vfmw
	b	.L38
	.p2align 3
.L42:
	ldr	x2, [x2, 32]
	mov	x1, x23
	mov	w0, 13
	ldr	w2, [x2, 648]
	bl	dprint_vfmw
	b	.L40
	.size	DumpList.part.5, .-DumpList.part.5
	.align	3
.LC3:
	.xword	.LC2
	.align	3
.LC5:
	.xword	.LC4
	.align	3
.LC7:
	.xword	.LC6
	.align	3
.LC9:
	.xword	.LC8
	.align	3
.LC11:
	.xword	.LC10
	.align	3
.LC13:
	.xword	.LC12
	.align	3
.LC15:
	.xword	.LC14
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	CombinePacket.part.10, %function
CombinePacket.part.10:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC17
	mov	w0, 7
	bl	dprint_vfmw
	ldr	x2, [x19, 632]
	mov	w0, 7
	ldr	x1, .LC19
	ldp	w5, w3, [x2, 48]
	ldr	w4, [x2, 16]
	ldr	w2, [x2, 20]
	bl	dprint_vfmw
	ldr	x1, [x19, 632]
	ldr	x3, .LC20
	ldr	w2, [x1, 16]
	ldr	x0, [x1, 40]
	ldr	x3, [x3, 104]
	ldr	x1, [x1, 8]
	sub	x0, x0, x2
	blr	x3
	ldr	x2, [x19, 632]
	mov	w4, 1
	ldr	x1, .LC22
	mov	w0, 7
	ldr	w3, [x2, 68]
	ldr	w20, [x2, 36]
	str	w3, [x2, 36]
	ldr	x2, [x19, 632]
	str	wzr, [x2, 28]
	ldr	x2, [x19, 632]
	ldr	w5, [x2, 16]
	ldr	x3, [x2, 40]
	sub	x3, x3, x5
	str	x3, [x2, 8]
	ldr	x2, [x19, 632]
	ldr	w5, [x2, 16]
	ldr	w3, [x2, 52]
	sub	w3, w3, w5
	str	w3, [x2, 20]
	ldr	x2, [x19, 632]
	ldr	w5, [x2, 16]
	ldr	w3, [x2, 48]
	add	w3, w5, w3
	str	w3, [x2, 16]
	ldr	x2, [x19, 632]
	str	w4, [x2, 76]
	ldr	x2, [x19, 632]
	ldp	w3, w2, [x2, 16]
	bl	dprint_vfmw
	add	x0, x19, 65536
	mov	w1, w20
	ldr	w0, [x0, 116]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 632]
	str	xzr, [x0, 40]
	ldr	x0, [x19, 632]
	str	wzr, [x0, 60]
	ldr	x0, [x19, 632]
	str	wzr, [x0, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CombinePacket.part.10, .-CombinePacket.part.10
	.align	3
.LC17:
	.xword	.LC16
	.align	3
.LC19:
	.xword	.LC18
	.align	3
.LC20:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC22:
	.xword	.LC21
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ue_v
	.type	ue_v, %function
ue_v:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x21, x22, 12288
	mov	x23, x1
	add	x21, x21, 2600
	bl	_mcount
	mov	w1, 32
	mov	x0, x21
	bl	BsShow
	mov	w20, w0
	bl	ZerosMS_32
	cmp	w0, 15
	mov	w19, w0
	bls	.L74
	cmp	w0, 31
	bls	.L75
	add	x0, x22, 65536
	mov	w1, 1
	mov	w20, -1
	mov	w19, 32
	str	w1, [x0, 56]
.L72:
	ldr	x4, [x22, 632]
	mov	w3, w20
	ldr	x1, .LC24
	mov	x2, x23
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
.L74:
	lsl	w19, w0, 1
	mov	w2, 32
	add	w19, w19, 1
	mov	x0, x21
	sub	w2, w2, w19
	mov	w1, w19
	lsr	w20, w20, w2
	sub	w20, w20, #1
	bl	BsSkip
	b	.L72
	.p2align 3
.L75:
	mov	w1, w0
	mov	x0, x21
	add	w24, w19, 1
	lsl	w19, w19, 1
	bl	BsSkip
	add	w19, w19, 1
	mov	w1, w24
	mov	x0, x21
	bl	BsShow
	sub	w20, w0, #1
	mov	w1, w24
	mov	x0, x21
	bl	BsSkip
	b	.L72
	.size	ue_v, .-ue_v
	.align	3
.LC24:
	.xword	.LC23
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	se_v
	.type	se_v, %function
se_v:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x21, x22, 12288
	mov	x23, x1
	add	x21, x21, 2600
	bl	_mcount
	mov	w1, 32
	mov	x0, x21
	bl	BsShow
	mov	w20, w0
	bl	ZerosMS_32
	cmp	w0, 15
	mov	w19, w0
	bhi	.L77
	lsl	w19, w0, 1
	mov	w2, 32
	add	w19, w19, 1
	mov	x0, x21
	sub	w2, w2, w19
	mov	w1, w19
	lsr	w20, w20, w2
	and	w2, w20, 1
	lsr	w20, w20, 1
	cmp	w2, wzr
	csneg	w20, w20, w20, eq
	bl	BsSkip
.L79:
	ldr	x4, [x22, 632]
	mov	w3, w20
	ldr	x1, .LC25
	mov	x2, x23
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
.L77:
	cmp	w0, 31
	bls	.L86
	add	x0, x22, 65536
	mov	w1, 1
	mov	w20, 2147483647
	mov	w19, 32
	str	w1, [x0, 56]
	b	.L79
	.p2align 3
.L86:
	mov	w1, w0
	mov	x0, x21
	bl	BsSkip
	mov	w1, w19
	mov	x0, x21
	lsl	w19, w19, 1
	bl	BsGet
	mov	w20, w0
	mov	w1, 1
	mov	x0, x21
	add	w19, w19, 1
	bl	BsGet
	and	w0, w0, 1
	cmp	w0, wzr
	csneg	w20, w20, w20, eq
	b	.L79
	.size	se_v, .-se_v
	.align	3
.LC25:
	.xword	.LC23
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	u_v
	.type	u_v, %function
u_v:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x22, x2
	mov	w19, w1
	bl	_mcount
	add	x0, x21, 12288
	mov	w1, w19
	add	x0, x0, 2600
	bl	BsGet
	mov	w20, w0
	ldr	x4, [x21, 632]
	mov	w3, w0
	ldr	x1, .LC26
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
	.size	u_v, .-u_v
	.align	3
.LC26:
	.xword	.LC23
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	u_1
	.type	u_1, %function
u_1:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	add	x0, x20, 12288
	mov	w1, 1
	add	x0, x0, 2600
	bl	BsGet
	mov	w19, w0
	ldr	x5, [x20, 632]
	mov	w3, w0
	ldr	x1, .LC27
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
	.size	u_1, .-u_1
	.align	3
.LC27:
	.xword	.LC23
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_IsEosNal
	.type	H264_IsEosNal, %function
H264_IsEosNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 12288
	bl	_mcount
	add	x19, x19, 2600
	mov	w1, 32
	mov	x0, x19
	bl	BsGet
	mov	w20, w0
	mov	w1, 32
	mov	x0, x19
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w20, w2
	ccmp	w0, w1, 0, eq
	cset	w0, eq
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	H264_IsEosNal, .-H264_IsEosNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetMinPOC
	.type	GetMinPOC, %function
GetMinPOC:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x2
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	str	wzr, [x21]
	add	x1, x20, 24576
	mov	w0, 2147483647
	str	w0, [x19]
	ldr	w0, [x1, 3712]
	ldr	w2, [x1, 3716]
	cmp	w2, w0
	bls	.L91
	mov	w2, w0
	str	w0, [x1, 3716]
.L91:
	mov	w0, 0
	cbz	w2, .L90
	.p2align 2
.L98:
	add	x3, x20, x0, uxtw 3
	ldr	w5, [x19]
	ldr	x3, [x3, 27904]
	ldr	w4, [x3, 648]
	cmp	w5, w4
	ble	.L93
	ldrb	w3, [x3, 4]
	cbz	w3, .L93
	str	w4, [x19]
	str	w0, [x21]
.L93:
	ldr	w3, [x1, 3716]
	add	w0, w0, 1
	cmp	w3, w0
	bhi	.L98
.L90:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	GetMinPOC, .-GetMinPOC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetMaxPOC
	.type	GetMaxPOC, %function
GetMaxPOC:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x2
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	str	wzr, [x21]
	add	x1, x20, 24576
	str	wzr, [x19]
	ldr	w0, [x1, 3712]
	ldr	w2, [x1, 3716]
	cmp	w2, w0
	bls	.L102
	mov	w2, w0
	str	w0, [x1, 3716]
.L102:
	mov	w0, 0
	cbz	w2, .L101
	.p2align 2
.L109:
	add	x3, x20, x0, uxtw 3
	ldr	w5, [x19]
	ldr	x3, [x3, 27904]
	ldr	w4, [x3, 648]
	cmp	w5, w4
	bge	.L104
	ldrb	w3, [x3, 4]
	cbz	w3, .L104
	str	w4, [x19]
	str	w0, [x21]
.L104:
	ldr	w3, [x1, 3716]
	add	w0, w0, 1
	cmp	w3, w0
	bhi	.L109
.L101:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	GetMaxPOC, .-GetMaxPOC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FrameStoreRefFlg
	.type	FrameStoreRefFlg, %function
FrameStoreRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 2]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	cset	w0, ne
	ldp	x29, x30, [sp], 32
	ret
	.size	FrameStoreRefFlg, .-FrameStoreRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ShortTermRefFlg
	.type	ShortTermRefFlg, %function
ShortTermRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w1, [x19, 2]
	cmp	w1, 3
	beq	.L129
	tbnz	x1, 0, .L130
.L117:
	mov	w0, 0
	tbnz	x1, 1, .L118
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L130:
	ldr	w0, [x19, 720]
	and	w0, w0, 16776960
	cmp	w0, 65536
	bne	.L117
.L122:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L129:
	ldr	w0, [x19, 680]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L122
	ldr	w0, [x19, 720]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L122
.L118:
	ldr	w0, [x19, 760]
	ldr	x19, [sp, 16]
	and	w0, w0, 16776960
	cmp	w0, 65536
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	ShortTermRefFlg, .-ShortTermRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	LongTermRefFlg
	.type	LongTermRefFlg, %function
LongTermRefFlg:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w1, [x19, 2]
	cmp	w1, 3
	beq	.L147
	tbnz	x1, 0, .L148
.L135:
	mov	w0, 0
	tbnz	x1, 1, .L136
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L148:
	ldr	w0, [x19, 720]
	and	w0, w0, 16776960
	cmp	w0, 256
	bne	.L135
.L140:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L147:
	ldr	w0, [x19, 680]
	and	w0, w0, 16776960
	cmp	w0, 256
	beq	.L140
	ldr	w0, [x19, 720]
	and	w0, w0, 16776960
	cmp	w0, 256
	beq	.L140
.L136:
	ldr	w0, [x19, 760]
	ldr	x19, [sp, 16]
	and	w0, w0, 16776960
	cmp	w0, 256
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	LongTermRefFlg, .-LongTermRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UpdateLTReflist
	.type	UpdateLTReflist, %function
UpdateLTReflist:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x24, x22, 24576
	bl	_mcount
	ldr	w0, [x24, 3716]
	cbz	w0, .L156
	sub	w23, w0, #1
	add	x20, x24, 3328
	add	x23, x23, 3489
	mov	w19, 0
	add	x23, x22, x23, lsl 3
	.p2align 2
.L152:
	ldr	x21, [x20]
	add	x20, x20, 8
	mov	x0, x21
	bl	LongTermRefFlg
	add	x1, x22, x19, uxtw 3
	cmp	x20, x23
	cbz	w0, .L151
	add	w19, w19, 1
	str	x21, [x1, 28160]
.L151:
	bne	.L152
.L150:
	ldr	w1, [x24, 3712]
	str	w19, [x24, 3724]
	cmp	w1, w19
	bls	.L149
	.p2align 2
.L159:
	add	x0, x22, x19, uxtw 3
	add	w19, w19, 1
	cmp	w19, w1
	str	xzr, [x0, 28160]
	bne	.L159
.L149:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L156:
	mov	w19, 0
	b	.L150
	.size	UpdateLTReflist, .-UpdateLTReflist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UpdateReflist
	.type	UpdateReflist, %function
UpdateReflist:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x22, x0
	mov	x0, x30
	add	x24, x22, 24576
	bl	_mcount
	ldr	w0, [x24, 3716]
	cbz	w0, .L170
	sub	w23, w0, #1
	add	x20, x24, 3328
	add	x23, x23, 3489
	mov	w19, 0
	add	x23, x22, x23, lsl 3
	.p2align 2
.L166:
	ldr	x21, [x20]
	add	x20, x20, 8
	mov	x0, x21
	bl	ShortTermRefFlg
	add	x1, x22, x19, uxtw 3
	cmp	x20, x23
	cbz	w0, .L165
	add	w19, w19, 1
	str	x21, [x1, 28032]
.L165:
	bne	.L166
.L164:
	ldr	w1, [x24, 3712]
	str	w19, [x24, 3720]
	cmp	w1, w19
	bls	.L163
	.p2align 2
.L173:
	add	x0, x22, x19, uxtw 3
	add	w19, w19, 1
	cmp	w19, w1
	str	xzr, [x0, 28032]
	bne	.L173
.L163:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L170:
	mov	w19, 0
	b	.L164
	.size	UpdateReflist, .-UpdateReflist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetPicNumX
	.type	GetPicNumX, %function
GetPicNumX:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldrb	w0, [x20, 3]
	ldr	w2, [x20, 656]
	cbz	w0, .L179
	lsl	w2, w2, 1
	add	w2, w2, 1
.L179:
	mvn	w0, w19
	add	w0, w0, w2
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	GetPicNumX, .-GetPicNumX
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UnMarkFrameStoreRef
	.type	UnMarkFrameStoreRef, %function
UnMarkFrameStoreRef:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 1]
	tbz	x0, 0, .L181
	ldrb	w1, [x19, 2]
	strb	wzr, [x19, 721]
	and	w1, w1, 2
	strb	wzr, [x19, 722]
	strb	w1, [x19, 2]
.L181:
	tbz	x0, 1, .L182
	ldrb	w1, [x19, 2]
	strb	wzr, [x19, 761]
	and	w1, w1, 1
	strb	wzr, [x19, 762]
	strb	w1, [x19, 2]
.L182:
	cmp	w0, 3
	bne	.L183
	strb	wzr, [x19, 682]
	strb	wzr, [x19, 681]
.L183:
	add	x20, x20, 65536
	strb	wzr, [x19, 2]
	ldr	w1, [x19, 632]
	mov	w2, 0
	ldr	w0, [x20, 116]
	bl	FSP_SetRef
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	UnMarkFrameStoreRef, .-UnMarkFrameStoreRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	SlidingWinMark.part.2, %function
SlidingWinMark.part.2:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	add	x19, x22, 24576
	bl	_mcount
	ldr	w0, [x19, 3716]
	cbz	w0, .L190
	sub	w21, w0, #1
	add	x19, x19, 3328
	add	x21, x21, 3489
	add	x21, x22, x21, lsl 3
	.p2align 2
.L193:
	ldr	x20, [x19]
	add	x19, x19, 8
	mov	x0, x20
	ldrb	w1, [x20, 2]
	cbz	w1, .L192
	bl	ShortTermRefFlg
	cbnz	w0, .L204
.L192:
	cmp	x19, x21
	bne	.L193
.L190:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L204:
	mov	x1, x20
	mov	x0, x22
	bl	UnMarkFrameStoreRef
	mov	x0, x22
	bl	UpdateReflist
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	SlidingWinMark.part.2, .-SlidingWinMark.part.2
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UnMarkLTFrmByFrmIdx
	.type	UnMarkLTFrmByFrmIdx, %function
UnMarkLTFrmByFrmIdx:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	x0, x20, 24576
	ldr	w4, [x0, 3724]
	cbz	w4, .L205
	ldr	x1, [x20, 28160]
	ldr	w0, [x1, 644]
	cmp	w19, w0
	beq	.L207
	sub	w4, w4, #1
	mov	x0, x20
	add	x4, x20, x4, lsl 3
	b	.L209
	.p2align 3
.L210:
	ldr	x1, [x0, 28168]
	add	x0, x0, 8
	ldr	w3, [x1, 644]
	cmp	w3, w19
	beq	.L207
.L209:
	cmp	x0, x4
	bne	.L210
.L205:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L207:
	mov	x0, x20
	bl	UnMarkFrameStoreRef
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	UnMarkLTFrmByFrmIdx, .-UnMarkLTFrmByFrmIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UnMarkLTFldByFrmIdx
	.type	UnMarkLTFldByFrmIdx, %function
UnMarkLTFldByFrmIdx:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	mov	x21, x1
	mov	w20, w2
	mov	w19, w3
	bl	_mcount
	add	x0, x22, 24576
	ldr	w4, [x0, 3724]
	cbz	w4, .L216
	sub	w6, w4, #1
	add	x0, x0, 3584
	add	x6, x6, 3521
	add	x6, x22, x6, lsl 3
	b	.L220
	.p2align 3
.L218:
	cmp	x0, x6
	beq	.L216
.L220:
	ldr	x4, [x0]
	add	x0, x0, 8
	ldr	w5, [x4, 644]
	cmp	w5, w19
	bne	.L218
	cmp	w20, 1
	beq	.L232
	cmp	w20, 2
	bne	.L218
	ldrb	w1, [x4, 721]
	cmp	w1, wzr
	ccmp	x4, x21, 0, ne
	beq	.L218
.L231:
	mov	x1, x4
	mov	x0, x22
	bl	UnMarkFrameStoreRef
.L216:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L232:
	ldrb	w1, [x4, 761]
	cmp	w1, wzr
	ccmp	x4, x21, 0, ne
	beq	.L218
	b	.L231
	.size	UnMarkLTFldByFrmIdx, .-UnMarkLTFldByFrmIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UnMarkSTRef
	.type	UnMarkSTRef, %function
UnMarkSTRef:
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
	cbnz	w0, .L234
	add	x0, x19, 24576
	ldr	w3, [x20, 656]
	sub	w3, w3, #1
	ldr	w1, [x0, 3720]
	sub	w2, w3, w21
	cbz	w1, .L233
	sub	w4, w1, #1
	add	x0, x0, 3456
	add	x4, x4, 3505
	add	x4, x19, x4, lsl 3
	b	.L240
	.p2align 3
.L239:
	cmp	x0, x4
	beq	.L233
.L240:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldr	w3, [x1, 688]
	cmp	w3, w2
	bne	.L239
	ldrb	w3, [x1, 2]
	cmp	w3, 3
	bne	.L239
	ldr	w3, [x1, 680]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L239
	mov	x0, x19
	bl	UnMarkFrameStoreRef
.L233:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L234:
	add	x1, x19, 24576
	ldr	w0, [x20, 656]
	add	x4, x1, 3456
	mov	w3, 0
	lsl	w0, w0, 1
	ldr	w6, [x1, 3720]
	sub	w2, w0, w21
	cbz	w6, .L233
	.p2align 2
.L254:
	ldr	x1, [x4]
	add	x4, x4, 8
	ldrb	w0, [x1, 2]
	and	w7, w0, 1
	tbz	x0, 0, .L241
	ldr	w5, [x1, 720]
	and	w5, w5, 16776960
	cmp	w5, 65536
	beq	.L266
.L241:
	tbz	x0, 1, .L243
	ldr	w0, [x1, 760]
	and	w0, w0, 16776960
	cmp	w0, 65536
	beq	.L267
.L243:
	add	w3, w3, 1
	cmp	w3, w6
	bne	.L254
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L266:
	ldr	w5, [x1, 728]
	cmp	w5, w2
	bne	.L241
	add	x3, x19, x3, uxtw 3
	and	w0, w0, 2
	strb	w0, [x1, 2]
	ldr	x0, [x3, 28032]
	strb	wzr, [x0, 722]
	b	.L265
	.p2align 3
.L267:
	ldr	w0, [x1, 768]
	cmp	w0, w2
	bne	.L243
	add	x3, x19, x3, uxtw 3
	strb	w7, [x1, 2]
	ldr	x0, [x3, 28032]
	strb	wzr, [x0, 762]
.L265:
	ldr	x0, [x3, 28032]
	ldrb	w1, [x0, 680]
	cmp	w1, 3
	ccmp	w1, wzr, 4, ne
	bne	.L244
	strb	wzr, [x0, 682]
	ldr	x0, [x3, 28032]
	strb	wzr, [x0, 681]
	ldr	x0, [x3, 28032]
.L244:
	ldrb	w1, [x0, 2]
	cbnz	w1, .L233
	ldr	w1, [x0, 668]
	add	x3, x19, 65536
	mov	w2, 0
	add	x1, x1, 12288
	add	x1, x1, 3748
	str	wzr, [x19, x1, lsl 2]
	ldr	w1, [x0, 632]
	ldr	w0, [x3, 116]
	bl	FSP_SetRef
	b	.L233
	.size	UnMarkSTRef, .-UnMarkSTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UnMarkLTRef
	.type	UnMarkLTRef, %function
UnMarkLTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x1
	mov	x20, x0
	mov	x0, x30
	mov	w19, w2
	bl	_mcount
	ldrb	w0, [x21, 3]
	cbz	w0, .L269
	add	x0, x20, 24576
	mov	w3, 0
	add	x4, x0, 3584
	ldr	w2, [x0, 3724]
	cbz	w2, .L268
	.p2align 2
.L289:
	ldr	x1, [x4]
	add	x4, x4, 8
	ldrb	w0, [x1, 2]
	and	w6, w0, 1
	tbz	x0, 0, .L274
	ldr	w5, [x1, 720]
	and	w5, w5, 16776960
	cmp	w5, 256
	beq	.L301
.L274:
	tbz	x0, 1, .L276
	ldr	w0, [x1, 760]
	and	w0, w0, 16776960
	cmp	w0, 256
	beq	.L302
.L276:
	add	w3, w3, 1
	cmp	w3, w2
	bne	.L289
.L268:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L301:
	ldr	w5, [x1, 724]
	cmp	w19, w5
	bne	.L274
	add	x3, x20, x3, uxtw 3
	and	w0, w0, 2
	strb	w0, [x1, 2]
	ldr	x0, [x3, 28160]
	strb	wzr, [x0, 721]
	b	.L300
	.p2align 3
.L302:
	ldr	w0, [x1, 764]
	cmp	w19, w0
	bne	.L276
	add	x3, x20, x3, uxtw 3
	strb	w6, [x1, 2]
	ldr	x0, [x3, 28160]
	strb	wzr, [x0, 761]
.L300:
	ldr	x0, [x3, 28160]
	ldrb	w1, [x0, 680]
	cmp	w1, 3
	ccmp	w1, wzr, 4, ne
	bne	.L277
	strb	wzr, [x0, 682]
	ldr	x0, [x3, 28160]
	strb	wzr, [x0, 681]
	ldr	x0, [x3, 28160]
.L277:
	ldrb	w1, [x0, 2]
	cbnz	w1, .L268
	ldr	w0, [x0, 668]
	add	x0, x0, 12288
	add	x0, x0, 3748
	str	wzr, [x20, x0, lsl 2]
	b	.L268
	.p2align 3
.L269:
	add	x0, x20, 24576
	ldr	w1, [x0, 3724]
	cbz	w1, .L268
	sub	w2, w1, #1
	add	x0, x0, 3584
	add	x2, x2, 3521
	add	x2, x20, x2, lsl 3
	b	.L280
	.p2align 3
.L279:
	cmp	x0, x2
	beq	.L268
.L280:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldr	w3, [x1, 684]
	cmp	w3, w19
	bne	.L279
	ldrb	w3, [x1, 2]
	cmp	w3, 3
	bne	.L279
	ldr	w3, [x1, 680]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L279
	mov	x0, x20
	bl	UnMarkFrameStoreRef
	b	.L268
	.size	UnMarkLTRef, .-UnMarkLTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MarkPicLTRef
	.type	MarkPicLTRef, %function
MarkPicLTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x3
	mov	x21, x0
	mov	x0, x30
	mov	w19, w2
	mov	w20, w1
	bl	_mcount
	ldrb	w2, [x22, 3]
	add	x0, x21, 24576
	cbz	w2, .L304
	ldr	w7, [x0, 3720]
	mov	w3, 0
	add	x4, x0, 3456
	cbz	w7, .L303
	.p2align 2
.L325:
	ldr	x0, [x4]
	ldrb	w5, [x0, 2]
	tbz	x5, 0, .L310
	ldr	w6, [x0, 720]
	and	w6, w6, 16776960
	cmp	w6, 65536
	beq	.L331
.L310:
	tbz	x5, 1, .L313
	ldr	w5, [x0, 760]
	and	w5, w5, 16776960
	cmp	w5, 65536
	beq	.L332
.L313:
	add	w3, w3, 1
	add	x4, x4, 8
	cmp	w3, w7
	bne	.L325
.L303:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L331:
	ldr	w1, [x0, 728]
	cmp	w19, w1
	bne	.L310
	ldrb	w1, [x0, 761]
	cmp	w1, 1
	bne	.L314
	ldr	w1, [x0, 644]
	cmp	w20, w1
	bne	.L313
.L314:
	add	x3, x21, x3, uxtw 3
	str	w20, [x0, 644]
	cmp	w2, 1
	lsl	w0, w20, 1
	csinc	w0, w0, w0, ne
	mov	w1, 1
	ldr	x2, [x3, 28032]
	str	w0, [x2, 724]
	ldr	x0, [x3, 28032]
	strb	w1, [x0, 721]
	ldr	x0, [x3, 28032]
	strb	wzr, [x0, 722]
	ldr	x0, [x3, 28032]
	ldrb	w1, [x0, 680]
	cmp	w1, 3
	ccmp	w1, wzr, 4, ne
	bne	.L303
	ldrb	w2, [x0, 721]
	ldrb	w1, [x0, 761]
	cmp	w1, w2
	bne	.L303
	strb	w1, [x0, 681]
	ldr	x0, [x3, 28032]
	ldrb	w1, [x0, 722]
	strb	w1, [x0, 682]
	b	.L303
	.p2align 3
.L332:
	ldr	w1, [x0, 768]
	cmp	w1, w19
	bne	.L313
	ldrb	w1, [x0, 721]
	cmp	w1, 1
	bne	.L317
	ldr	w1, [x0, 644]
	cmp	w1, w20
	bne	.L313
.L317:
	add	x3, x21, x3, uxtw 3
	str	w20, [x0, 644]
	cmp	w2, 2
	lsl	w0, w20, 1
	csinc	w0, w0, w0, ne
	mov	w1, 1
	ldr	x2, [x3, 28032]
	str	w0, [x2, 764]
	ldr	x0, [x3, 28032]
	strb	w1, [x0, 761]
	ldr	x0, [x3, 28032]
	strb	wzr, [x0, 762]
	ldr	x0, [x3, 28032]
	ldrb	w1, [x0, 680]
	cmp	w1, 3
	ccmp	w1, wzr, 4, ne
	bne	.L303
	ldrb	w2, [x0, 721]
	ldrb	w1, [x0, 761]
	cmp	w1, w2
	bne	.L303
	strb	w1, [x0, 681]
	ldr	x0, [x3, 28032]
	ldrb	w1, [x0, 762]
	strb	w1, [x0, 682]
	b	.L303
	.p2align 3
.L304:
	ldr	w7, [x0, 3720]
	cbz	w7, .L303
	add	x0, x0, 3456
	mov	w4, 0
	b	.L309
	.p2align 3
.L308:
	add	w4, w4, 1
	cmp	w4, w7
	beq	.L303
.L309:
	ldr	x5, [x0]
	add	x0, x0, 8
	ldrb	w6, [x5, 2]
	cmp	w6, 3
	bne	.L308
	ldr	w6, [x5, 680]
	and	w6, w6, 16776960
	cmp	w6, 65536
	bne	.L308
	ldr	w1, [x5, 688]
	cmp	w19, w1
	bne	.L308
	add	x4, x21, x4, uxtw 3
	str	w20, [x5, 644]
	mov	w0, 1
	ldr	x1, [x4, 28032]
	str	w20, [x1, 684]
	ldr	x1, [x4, 28032]
	strb	w0, [x1, 681]
	ldr	x1, [x4, 28032]
	strb	w0, [x1, 721]
	ldr	x1, [x4, 28032]
	strb	w0, [x1, 761]
	ldr	x0, [x4, 28032]
	strb	wzr, [x0, 682]
	ldr	x0, [x4, 28032]
	strb	wzr, [x0, 722]
	ldr	x0, [x4, 28032]
	strb	wzr, [x0, 762]
	b	.L303
	.size	MarkPicLTRef, .-MarkPicLTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MarkSTToLTRef
	.type	MarkSTToLTRef, %function
MarkSTToLTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x1
	mov	x21, x0
	mov	x0, x30
	mov	w19, w2
	mov	w20, w3
	bl	_mcount
	ldrb	w0, [x22, 3]
	cbnz	w0, .L334
	add	x0, x21, 24576
	ldr	w2, [x22, 656]
	sub	w2, w2, #1
	ldr	w4, [x0, 3724]
	sub	w19, w2, w19
	cbz	w4, .L336
	ldr	x1, [x21, 28160]
	ldr	w0, [x1, 644]
	cmp	w20, w0
	beq	.L338
	sub	w4, w4, #1
	mov	x0, x21
	add	x4, x21, x4, lsl 3
	b	.L340
	.p2align 3
.L341:
	ldr	x1, [x0, 28168]
	add	x0, x0, 8
	ldr	w3, [x1, 644]
	cmp	w20, w3
	beq	.L338
.L340:
	cmp	x4, x0
	bne	.L341
.L336:
	mov	x3, x22
	mov	w2, w19
	mov	w1, w20
	mov	x0, x21
	bl	MarkPicLTRef
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L334:
	add	x0, x21, 24576
	ldr	w2, [x22, 656]
	lsl	w2, w2, 1
	ldr	w1, [x0, 3720]
	sub	w19, w2, w19
	cbz	w1, .L336
	sub	w5, w1, #1
	add	x0, x0, 3456
	add	x5, x5, 3505
	add	x5, x21, x5, lsl 3
	.p2align 2
.L344:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w3, [x1, 2]
	tbz	x3, 0, .L342
	ldr	w4, [x1, 720]
	and	w4, w4, 16776960
	cmp	w4, 65536
	beq	.L354
.L342:
	tbz	x3, 1, .L343
	ldr	w3, [x1, 760]
	and	w3, w3, 16776960
	cmp	w3, 65536
	beq	.L355
.L343:
	cmp	x0, x5
	bne	.L344
	b	.L336
	.p2align 3
.L354:
	ldr	w2, [x1, 728]
	cmp	w2, w19
	bne	.L342
	mov	w3, w20
	mov	w2, 1
	mov	x0, x21
	bl	UnMarkLTFldByFrmIdx
	b	.L336
	.p2align 3
.L355:
	ldr	w2, [x1, 768]
	cmp	w2, w19
	bne	.L343
	mov	w3, w20
	mov	w2, 2
	mov	x0, x21
	bl	UnMarkLTFldByFrmIdx
	b	.L336
	.p2align 3
.L338:
	mov	x0, x21
	bl	UnMarkFrameStoreRef
	b	.L336
	.size	MarkSTToLTRef, .-MarkSTToLTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UpdateMaxLTFrmIdx
	.type	UpdateMaxLTFrmIdx, %function
UpdateMaxLTFrmIdx:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	add	x21, x22, 24576
	mov	w20, w1
	bl	_mcount
	ldr	w0, [x21, 3724]
	str	w20, [x21, 3728]
	cbz	w0, .L356
	mov	w19, 0
	mov	w2, w20
	b	.L358
	.p2align 3
.L359:
	ldr	w0, [x21, 3724]
	cmp	w0, w19
	bls	.L356
.L363:
	ldr	w2, [x21, 3728]
.L358:
	add	x0, x22, x19, uxtw 3
	add	w19, w19, 1
	ldr	x1, [x0, 28160]
	ldr	w0, [x1, 644]
	cmp	w0, w2
	bcc	.L359
	mov	x0, x22
	bl	UnMarkFrameStoreRef
	ldr	w0, [x21, 3724]
	cmp	w0, w19
	bhi	.L363
.L356:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	UpdateMaxLTFrmIdx, .-UpdateMaxLTFrmIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UnMarkAllSTRef
	.type	UnMarkAllSTRef, %function
UnMarkAllSTRef:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 24576
	mov	w19, 0
	bl	_mcount
	ldr	w0, [x21, 3720]
	cbz	w0, .L364
	.p2align 2
.L368:
	add	x1, x20, x19, uxtw 3
	mov	x0, x20
	add	w19, w19, 1
	ldr	x1, [x1, 28032]
	bl	UnMarkFrameStoreRef
	ldr	w0, [x21, 3720]
	cmp	w0, w19
	bhi	.L368
.L364:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	UnMarkAllSTRef, .-UnMarkAllSTRef
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	MarkCurrPicLT
	.type	MarkCurrPicLT, %function
MarkCurrPicLT:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x1
	mov	x21, x0
	mov	x0, x30
	mov	w19, w2
	bl	_mcount
	ldrb	w2, [x20, 3]
	cbnz	w2, .L371
	add	x0, x21, 24576
	ldr	w3, [x0, 3724]
	cbz	w3, .L372
	ldr	x1, [x21, 28160]
	ldr	w0, [x1, 644]
	cmp	w19, w0
	beq	.L373
	sub	w3, w3, #1
	mov	x0, x21
	add	x3, x21, x3, lsl 3
	b	.L375
	.p2align 3
.L376:
	ldr	x1, [x0, 28168]
	add	x0, x0, 8
	ldr	w2, [x1, 644]
	cmp	w19, w2
	beq	.L373
.L375:
	cmp	x0, x3
	bne	.L376
.L372:
	strb	wzr, [x20, 5]
	mov	w0, 1
	str	w19, [x20, 660]
	strb	w0, [x20, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L373:
	mov	x0, x21
	bl	UnMarkFrameStoreRef
	strb	wzr, [x20, 5]
	str	w19, [x20, 660]
	mov	w0, 1
	strb	w0, [x20, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L371:
	ldr	x1, [x20, 640]
	mov	w3, w19
	mov	x0, x21
	bl	UnMarkLTFldByFrmIdx
	strb	wzr, [x20, 5]
	str	w19, [x20, 660]
	mov	w0, 1
	strb	w0, [x20, 4]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	MarkCurrPicLT, .-MarkCurrPicLT
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	RemoveFrameStoreOutDPB
	.type	RemoveFrameStoreOutDPB, %function
RemoveFrameStoreOutDPB:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	uxtw	x21, w1
	mov	x20, x0
	mov	x0, x30
	mov	x19, x21
	add	x22, x20, x21, lsl 3
	bl	_mcount
	ldr	x1, [x22, 27904]
	cbz	x1, .L382
	ldr	w0, [x1, 672]
	add	x23, x20, 24576
	add	x24, x20, 65536
	add	x0, x0, 4096
	add	x0, x20, x0, lsl 2
	str	wzr, [x0, 11928]
	ldr	w0, [x23, 3996]
	cmp	w0, wzr
	cset	w2, ne
	sub	w0, w0, w2
	str	w0, [x23, 3996]
	ldr	w1, [x1, 632]
	ldr	w0, [x24, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L385
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	beq	.L386
	ldr	x1, [x22, 27904]
	mov	w2, 0
	ldr	w0, [x24, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
.L386:
	ldr	x1, [x22, 27904]
	mov	w2, 0
	ldr	w0, [x24, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetRef
.L385:
	ldr	x0, [x22, 27904]
	strb	wzr, [x0, 1]
	ldr	x0, [x22, 27904]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 27904]
	strb	wzr, [x0, 2]
	ldr	w3, [x23, 3716]
	sub	w3, w3, #1
	cmp	w19, w3
	bcs	.L388
	.p2align 2
.L398:
	add	w0, w19, 1
	add	x1, x21, 3488
	add	x2, x0, 3488
	mov	w19, w0
	cmp	w0, w3
	mov	x21, x0
	ldr	x0, [x20, x2, lsl 3]
	str	x0, [x20, x1, lsl 3]
	bne	.L398
.L388:
	add	x20, x20, x3, uxtw 3
	str	xzr, [x20, 27904]
	str	w3, [x23, 3716]
.L382:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	RemoveFrameStoreOutDPB, .-RemoveFrameStoreOutDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	RemoveUnUsedFrameStore
	.type	RemoveUnUsedFrameStore, %function
RemoveUnUsedFrameStore:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	cbz	x22, .L412
	add	x0, x22, 24576
	ldr	w23, [x0, 3716]
	cbz	w23, .L411
	mov	w21, 0
	mov	w20, 0
	add	x24, x22, 65536
	mov	w25, -1
	b	.L410
	.p2align 3
.L421:
	ldrb	w1, [x0, 4]
	cbnz	w1, .L408
	ldr	w1, [x0, 800]
	cmp	w1, 1
	beq	.L408
	strb	wzr, [x0, 1]
	mov	w1, w21
	mov	x0, x22
	add	w20, w20, 1
	bl	RemoveFrameStoreOutDPB
	cmp	w23, w20
	beq	.L411
	.p2align 2
.L410:
	add	x19, x22, x21, uxtw 3
	ldr	w0, [x24, 116]
	ldr	x1, [x19, 27904]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	cbz	x0, .L405
	ldrsh	w1, [x0]
	cbnz	w1, .L406
	ldr	x1, [x19, 27904]
	ldrb	w2, [x1, 2]
	cbnz	w2, .L407
	ldrsb	w0, [x0]
	cbnz	w0, .L420
.L407:
	str	w25, [x1, 632]
	ldr	x0, [x19, 27904]
	strb	wzr, [x0, 2]
.L405:
	ldr	x0, [x19, 27904]
	ldr	w1, [x0, 800]
	cmp	w1, 1
	cset	w1, eq
	strb	w1, [x0, 4]
.L406:
	ldr	x0, [x19, 27904]
	ldrb	w1, [x0, 2]
	cbz	w1, .L421
.L408:
	add	w20, w20, 1
	add	w21, w21, 1
	cmp	w23, w20
	bne	.L410
.L411:
	mov	w0, 0
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L420:
	ldr	w1, [x1, 632]
	ldr	w0, [x24, 116]
	bl	FSP_SetRef
	ldr	x1, [x19, 27904]
	b	.L407
	.p2align 3
.L412:
	mov	w0, -1
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.size	RemoveUnUsedFrameStore, .-RemoveUnUsedFrameStore
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CheckFrameStore
	.type	CheckFrameStore, %function
CheckFrameStore:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	add	x0, x20, 61440
	add	x2, x20, 65536
	ldr	w1, [x0, 2788]
	cbnz	w1, .L423
	ldr	w1, [x19, 800]
	add	x2, x20, 65536
	cmp	w1, 1
	beq	.L424
	ldr	w1, [x2, 44]
	cbz	w1, .L450
.L424:
	mov	w1, 1
	str	w1, [x0, 2788]
.L423:
	ldr	w0, [x2, 116]
	ldr	w1, [x19, 632]
	bl	FSP_GetLogicFs
	cbz	x0, .L426
	ldrsb	w3, [x0, 1]
	cmp	w3, 3
	ccmp	w3, wzr, 4, ne
	beq	.L451
	ldrb	w2, [x19, 1]
	ldrb	w3, [x19]
	cbz	w2, .L434
	cmp	w3, 1
	beq	.L434
	ldr	w1, [x19]
	mov	w3, 50332416
	and	w1, w1, -16711936
	cmp	w1, w3
	beq	.L452
	cmp	w2, 1
	beq	.L453
	cmp	w2, 2
	beq	.L454
.L432:
	ldr	x1, [x20]
	ldr	w2, [x0, 168]
	mov	w0, 0
	ldr	w3, [x1, 4]
	cmp	w2, w3
	bhi	.L455
.L445:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L450:
	ldr	x1, [x20]
	ldr	w1, [x1, 12]
	cbnz	w1, .L424
	mov	w0, -1
	b	.L445
	.p2align 3
.L451:
	ldr	x1, .LC31
	mov	w0, 1
	ldr	w2, [x19, 632]
	bl	dprint_vfmw
.L428:
	ldr	w0, [x19, 800]
	cmp	w0, 1
	beq	.L435
	mov	w0, -3
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L453:
	ldrb	w3, [x19, 3]
	tbz	x3, 0, .L432
	ldr	x1, .LC39
	mov	w0, w2
	bl	dprint_vfmw
	mov	w0, -1
	b	.L445
	.p2align 3
.L454:
	ldrb	w3, [x19, 3]
	tbz	x3, 1, .L432
	ldr	x1, .LC41
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L445
	.p2align 3
.L435:
	mov	w1, 2
	mov	w0, -3
	str	w1, [x19, 800]
	b	.L445
	.p2align 3
.L426:
	ldr	x1, .LC33
	mov	x2, x19
	ldr	w4, [x19, 632]
	mov	w0, 1
	ldr	w3, [x19, 800]
	bl	dprint_vfmw
	b	.L428
	.p2align 3
.L434:
	ldr	x1, .LC35
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L445
	.p2align 3
.L455:
	ldr	x1, .LC43
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L445
.L452:
	ldrb	w3, [x19, 3]
	mov	w0, 1
	ldr	x1, .LC37
	bl	dprint_vfmw
	mov	w0, -1
	b	.L445
	.size	CheckFrameStore, .-CheckFrameStore
	.align	3
.LC31:
	.xword	.LC30
	.align	3
.LC33:
	.xword	.LC32
	.align	3
.LC35:
	.xword	.LC34
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
	.global	ExchangePts
	.type	ExchangePts, %function
ExchangePts:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	mov	x25, x1
	mov	x21, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x25, 608]
	cmn	x0, #1
	beq	.L456
	add	x26, x21, 24576
	ldr	w0, [x26, 3716]
	cbz	w0, .L456
	mov	w24, -1
	mov	w19, 0
	mov	x23, -2
	add	x22, x21, 65536
	.p2align 2
.L461:
	add	x20, x21, x19, sxtw 3
	ldr	x0, [x20, 27904]
	cbz	x0, .L460
	ldr	w1, [x0, 632]
	ldr	w0, [x22, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L460
	ldrsb	w0, [x0, 1]
	cmp	w0, 3
	ccmp	w0, wzr, 4, ne
	beq	.L460
	ldr	x0, [x20, 27904]
	ldr	x0, [x0, 608]
	cmp	x0, x23
	bcs	.L460
	mov	w24, w19
	mov	x23, x0
	.p2align 2
.L460:
	ldr	w0, [x26, 3716]
	add	w19, w19, 1
	cmp	w0, w19
	bhi	.L461
	cmn	w24, #1
	beq	.L456
	ldr	x2, [x25, 608]
	cmp	x23, x2
	bcc	.L475
.L456:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L475:
	ldr	x1, .LC45
	mov	x3, x23
	add	x21, x21, x24, sxtw 3
	mov	w0, 29
	bl	dprint_vfmw
	ldr	x0, [x21, 27904]
	ldr	x1, [x25, 608]
	str	x1, [x0, 608]
	str	x23, [x25, 608]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	ExchangePts, .-ExchangePts
	.align	3
.LC45:
	.xword	.LC44
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetImagePara
	.type	GetImagePara, %function
GetImagePara:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	x19, x1
	bl	_mcount
	add	x1, x20, 20480
	mov	w0, 2208
	ldr	x3, [x20, 14968]
	mov	w4, -772
	ldr	w5, [x19, 116]
	add	x8, x20, 61440
	ldr	w1, [x1, 1596]
	mov	w2, 64764
	ldr	x6, [x20, 14960]
	and	w4, w5, w4
	mov	w7, 4000
	movk	w2, 0xfffd, lsl 16
	umull	x0, w1, w0
	and	w2, w5, w2
	mov	w1, 262144
	add	x0, x3, x0
	ldrb	w5, [x19, 1]
	ldr	w3, [x0, 28]
	str	w4, [x19, 116]
	ldr	w4, [x8, 2796]
	umull	x0, w3, w7
	str	w2, [x19, 116]
	lsl	w3, w4, 10
	str	w3, [x19, 144]
	add	x3, x6, x0
	ldr	x0, [x20]
	ldr	w0, [x0, 592]
	cmp	w0, 1
	csel	w0, wzr, w1, ne
	cmp	w5, 3
	orr	w2, w0, w2
	str	w2, [x19, 116]
	bhi	.L520
	ldr	x0, .LC46
	ldrb	w0, [x0,w5,uxtw]
	adr	x1, .Lrtx480
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx480:
	.section	.rodata
	.align	0
	.align	2
.L480:
	.byte	(.L479 - .Lrtx480) / 4
	.byte	(.L481 - .Lrtx480) / 4
	.byte	(.L482 - .Lrtx480) / 4
	.byte	(.L479 - .Lrtx480) / 4
	.text
	.p2align 3
.L481:
	ldr	w0, [x19, 660]
	mov	w1, 3
	ldr	w6, [x19, 664]
	and	w0, w0, w1
	ldr	w4, [x19, 740]
	orr	w0, w0, 1792
	str	w6, [x19, 168]
	orr	w2, w0, w2
	str	w2, [x19, 116]
	stp	w4, w1, [x19, 108]
.L520:
	add	x22, x20, 65536
	add	x21, x20, 24576
.L478:
	ldrb	w0, [x3, 18]
	mov	w1, 0
	str	w0, [x19, 484]
	str	w0, [x19, 488]
	ldrb	w0, [x3, 22]
	cbz	w0, .L497
	ldr	w1, [x3, 3332]
.L497:
	mov	w0, 1
	str	w1, [x19, 496]
	str	w0, [x19, 492]
	ldr	x0, [x20]
	ldr	w0, [x0, 12]
	cmp	w0, wzr
	ble	.L498
	ldrb	w0, [x19, 680]
	cbz	w0, .L522
.L498:
	mov	x0, x20
	mov	x1, x19
	bl	ExchangePts
	ldrsb	w0, [x21, 3192]
	cbz	w0, .L499
	ldr	w0, [x21, 3208]
	cmp	w0, 3
	beq	.L523
	cmp	w0, 4
	beq	.L524
	cmp	w0, 5
	beq	.L525
	str	wzr, [x19, 364]
	mov	w0, 0
	ldr	x1, .LC48
	ldr	w2, [x21, 3208]
	bl	dprint_vfmw
	.p2align 2
.L499:
	ldr	w1, [x19, 632]
	ldr	w0, [x22, 116]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L476
	ldr	x3, .LC49
	mov	x2, 624
	add	x1, x19, 8
	ldr	x3, [x3, 104]
	blr	x3
	str	wzr, [x20, 376]
	mov	w2, 4
	mov	w1, 1
	ldrb	w0, [x19, 680]
	strb	w1, [x20, 371]
	cmp	w0, wzr
	csel	w0, w0, w2, eq
	strb	w0, [x20, 364]
	ldr	w0, [x19, 704]
	str	w0, [x20, 380]
	strb	w1, [x20, 369]
	str	w0, [x20, 384]
	str	w0, [x20, 388]
.L476:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L522:
	cmp	w5, 3
	bne	.L498
	ldr	w0, [x19, 116]
	and	w0, w0, -769
	str	w0, [x19, 116]
	b	.L498
	.p2align 3
.L479:
	ldrb	w0, [x19, 680]
	cbnz	w0, .L483
	ldr	w1, [x19, 664]
	ldr	w0, [x19, 656]
	str	w1, [x19, 168]
	ldr	w6, [x19, 660]
	ldr	w8, [x19, 740]
	ldr	w7, [x19, 780]
	cbnz	w0, .L484
	mov	w0, 3584
	add	x22, x20, 65536
.L485:
	ldr	w4, [x19, 732]
	ldr	w1, [x19, 772]
	cmp	w4, w1
	beq	.L526
	cset	w4, le
.L495:
	add	x21, x20, 24576
	ldrb	w1, [x21, 3228]
	cmp	w1, 3
	beq	.L506
	cmp	w1, 4
	lsl	w4, w4, 12
	mov	w1, 0
	csel	w4, w4, w1, ne
.L496:
	and	w1, w6, 3
	and	w2, w2, -12289
	orr	w1, w1, w4
	orr	w2, w2, w0
	orr	w2, w1, w2
	str	w2, [x19, 116]
	stp	w8, w7, [x19, 108]
	b	.L478
	.p2align 3
.L482:
	ldr	w0, [x19, 660]
	mov	w1, 2816
	ldr	w7, [x19, 664]
	mov	w6, 3
	and	w0, w0, 3
	ldr	w4, [x19, 780]
	orr	w0, w0, w1
	add	x22, x20, 65536
	orr	w2, w0, w2
	add	x21, x20, 24576
	stp	w6, w4, [x19, 108]
	str	w2, [x19, 116]
	str	w7, [x19, 168]
	b	.L478
	.p2align 3
.L523:
	mov	w0, 1
	str	w0, [x19, 364]
	b	.L499
	.p2align 3
.L525:
	mov	w0, 3
	str	w0, [x19, 364]
	b	.L499
	.p2align 3
.L483:
	add	x22, x20, 65536
	ldr	w4, [x19, 736]
	ldr	w9, [x19, 776]
	ldr	w0, [x22, 112]
	cmp	w4, w0
	bhi	.L527
	cmp	w0, w9
	bcc	.L489
.L487:
	ldr	w0, [x19, 664]
	ldr	w6, [x19, 660]
	ldr	w8, [x19, 740]
	ldr	w7, [x19, 780]
	str	w0, [x19, 168]
	mov	w0, 3
.L490:
	ldrb	w1, [x19, 3]
	cmp	w1, 1
	beq	.L528
	cmp	w1, 2
	beq	.L493
	lsl	w0, w0, 10
	orr	w0, w0, 768
	stp	w8, w7, [x19, 108]
	b	.L485
	.p2align 3
.L489:
	ldr	w6, [x19, 740]
	mov	w0, 1
	ldr	w7, [x19, 780]
	mov	w8, w6
	str	w4, [x19, 168]
	b	.L490
	.p2align 3
.L524:
	mov	w0, 2
	str	w0, [x19, 364]
	b	.L499
	.p2align 3
.L506:
	mov	w4, 4096
	b	.L496
	.p2align 3
.L526:
	ldr	x4, [x19, 120]
	mov	x1, 720
	movk	x1, 0x1e0, lsl 32
	cmp	x4, x1
	cset	w4, ne
	b	.L495
	.p2align 3
.L527:
	cmp	w0, w9
	bcc	.L487
	ldr	w6, [x19, 780]
	mov	w0, 2
	ldr	w8, [x19, 740]
	mov	w7, w6
	str	w9, [x19, 168]
	b	.L490
	.p2align 3
.L484:
	mov	w0, 3840
	add	x22, x20, 65536
	b	.L485
	.p2align 3
.L528:
	mov	w6, w7
	mov	w0, 2816
	str	w9, [x19, 168]
	stp	w8, w7, [x19, 108]
	b	.L485
	.p2align 3
.L493:
	mov	w6, w8
	mov	w0, 1792
	str	w4, [x19, 168]
	stp	w8, w7, [x19, 108]
	b	.L485
	.size	GetImagePara, .-GetImagePara
	.align	3
.LC46:
	.xword	.L480
	.align	3
.LC48:
	.xword	.LC47
	.align	3
.LC49:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SetFrmRepeatCount
	.type	SetFrmRepeatCount, %function
SetFrmRepeatCount:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	ldr	x0, [x19]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bls	.L530
	mov	x1, x20
	mov	x0, x19
	bl	SetFrmRepeatCount.part.1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L530:
	str	wzr, [x20, 24]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	SetFrmRepeatCount, .-SetFrmRepeatCount
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SplitFrmToFlds
	.type	SplitFrmToFlds, %function
SplitFrmToFlds:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w4, [x19, 681]
	mov	w6, 1
	ldrb	w3, [x19, 682]
	mov	w5, 2
	ldr	w2, [x19, 696]
	ldr	w1, [x19, 700]
	ldr	x0, [x19, 712]
	strb	w4, [x19, 721]
	strb	w3, [x19, 722]
	strb	w6, [x19, 720]
	strb	w4, [x19, 761]
	strb	w3, [x19, 762]
	strb	w5, [x19, 760]
	str	w2, [x19, 736]
	str	w1, [x19, 740]
	str	x0, [x19, 752]
	str	w2, [x19, 776]
	str	w1, [x19, 780]
	str	x0, [x19, 792]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	SplitFrmToFlds, .-SplitFrmToFlds
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CombineFldsToFrm
	.type	CombineFldsToFrm, %function
CombineFldsToFrm:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	w1, [x19, 732]
	mov	w5, 3
	ldr	w3, [x19, 736]
	ldr	w2, [x19, 772]
	ldr	w0, [x19, 776]
	cmp	w1, w2
	ldr	x4, [x19, 752]
	add	w0, w3, w0
	ldrb	w3, [x19, 761]
	csel	w1, w1, w2, le
	lsr	w0, w0, 1
	strb	w5, [x19, 680]
	str	w1, [x19, 648]
	str	w1, [x19, 692]
	str	w0, [x19, 696]
	str	x4, [x19, 712]
	cbz	w3, .L534
	ldrb	w1, [x19, 721]
	cbz	w1, .L534
	mov	w1, 1
	strb	w1, [x19, 681]
.L536:
	mov	w3, 0
.L537:
	ldr	w1, [x19, 740]
	ldr	w2, [x19, 780]
	str	w0, [x19, 664]
	cmp	w1, w2
	strb	w3, [x19, 682]
	csel	w0, w1, w2, cs
	str	w0, [x19, 660]
	str	w0, [x19, 700]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L534:
	ldr	w1, [x19, 760]
	strb	wzr, [x19, 681]
	tst	w1, 16776960
	beq	.L536
	ldr	w1, [x19, 720]
	mov	w3, 1
	tst	w1, 16776960
	bne	.L537
	b	.L536
	.size	CombineFldsToFrm, .-CombineFldsToFrm
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetAPC
	.type	GetAPC, %function
GetAPC:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x22, x1
	mov	x0, x30
	mov	w19, w2
	bl	_mcount
	cbz	x22, .L570
	cmp	w19, 1
	beq	.L550
	cmp	w19, 2
	beq	.L551
	ldr	w23, [x22, 732]
	ldr	w24, [x22, 772]
.L552:
	add	x21, x20, 24576
	ldr	w2, [x21, 3992]
	cbz	w2, .L561
	ldr	w0, [x21, 3736]
	cbz	w0, .L571
	add	x0, x21, 3740
	mov	w19, 0
	b	.L555
	.p2align 3
.L559:
	ldr	w1, [x0], 4
	cbz	w1, .L553
.L555:
	add	w19, w19, 1
	cmp	w19, w2
	bne	.L559
.L561:
	mov	w0, -1
.L565:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L551:
	ldr	w23, [x22, 772]
	mov	w24, w23
	b	.L552
.L571:
	mov	w19, 0
	.p2align 2
.L553:
	add	x0, x20, 65536
	ldr	w1, [x22, 632]
	ldr	w0, [x0, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L556
	ldr	x1, [x0, 640]
	cbz	x1, .L556
	add	x20, x20, x19, uxtw 2
	mov	w1, 1
	add	x20, x20, 24576
	mov	w4, 16
	mov	w3, w19
	str	w1, [x20, 3736]
	ldr	x1, .LC55
	ldr	x0, [x0, 640]
	ldr	w0, [x0, 8]
	str	w0, [x20, 3800]
	mov	w0, 13
	str	w23, [x20, 3864]
	str	w24, [x20, 3928]
	ldr	w2, [x21, 3996]
	cmp	w2, 15
	csinc	w2, w4, w2, hi
	str	w2, [x21, 3996]
	ldr	w2, [x22, 164]
	bl	dprint_vfmw
	str	w19, [x22, 672]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L550:
	ldr	w23, [x22, 732]
	mov	w24, w23
	b	.L552
.L556:
	ldrb	w3, [x22, 2]
	mov	x4, x0
	ldr	x1, .LC57
	mov	x5, 0
	ldr	w2, [x22, 632]
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L565
.L570:
	ldr	x2, .LC51
	mov	w3, 2220
	ldr	x1, .LC53
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L565
	.size	GetAPC, .-GetAPC
	.align	3
.LC51:
	.xword	.LC50
	.align	3
.LC53:
	.xword	.LC52
	.align	3
.LC55:
	.xword	.LC54
	.align	3
.LC57:
	.xword	.LC56
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SlidingWinMark
	.type	SlidingWinMark, %function
SlidingWinMark:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 24576
	add	x0, x19, 16384
	ldr	w2, [x1, 3724]
	ldr	w0, [x0, 1836]
	ldr	w1, [x1, 3720]
	sub	w0, w0, w2
	cmp	w1, w0
	bne	.L572
	mov	x0, x19
	bl	SlidingWinMark.part.2
.L572:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	SlidingWinMark, .-SlidingWinMark
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DumpDPB
	.type	DumpDPB, %function
DumpDPB:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x22, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC58
	ldr	w0, [x0]
	tbz	x0, 14, .L574
	add	x21, x22, 24576
	ldr	w0, [x21, 3716]
	cbz	w0, .L580
	ldr	x20, .LC61
	mov	w19, 0
	.p2align 2
.L579:
	add	x3, x22, x19, uxtw 3
	mov	w2, w19
	mov	x1, x20
	mov	w0, 14
	add	w19, w19, 1
	ldr	x3, [x3, 27904]
	ldrb	w5, [x3, 2]
	ldr	w4, [x3, 648]
	ldr	w3, [x3, 636]
	bl	dprint_vfmw
	ldr	w0, [x21, 3716]
	cmp	w0, w19
	bhi	.L579
.L580:
	ldr	x25, .LC59
	mov	w0, 14
	mov	x1, x25
	bl	dprint_vfmw
	ldr	w0, [x21, 3720]
	cbz	w0, .L578
	ldr	x24, .LC63
	mov	w19, 0
	ldr	x23, .LC65
	.p2align 2
.L581:
	add	x20, x22, x19, uxtw 3
	mov	w2, w19
	mov	x1, x24
	mov	w0, 14
	ldr	x3, [x20, 28032]
	ldrb	w5, [x3, 2]
	ldr	w4, [x3, 648]
	ldr	w3, [x3, 636]
	bl	dprint_vfmw
	ldr	x3, [x20, 28032]
	mov	w2, w19
	mov	x1, x23
	mov	w0, 14
	add	w19, w19, 1
	ldr	w3, [x3, 692]
	bl	dprint_vfmw
	ldr	w0, [x21, 3720]
	cmp	w0, w19
	bhi	.L581
.L578:
	mov	x1, x25
	mov	w0, 14
	bl	dprint_vfmw
.L574:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.size	DumpDPB, .-DumpDPB
	.align	3
.LC58:
	.xword	g_PrintEnable
	.align	3
.LC59:
	.xword	.LC6
	.align	3
.LC61:
	.xword	.LC60
	.align	3
.LC63:
	.xword	.LC62
	.align	3
.LC65:
	.xword	.LC64
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UpdateCurrFrameInfo
	.type	UpdateCurrFrameInfo, %function
UpdateCurrFrameInfo:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 61440
	mov	w2, 3
	ldr	x19, [x1, 752]
	ldrb	w3, [x1, 114]
	strb	w3, [x19]
	ldrb	w3, [x1, 116]
	cbnz	w3, .L588
	ldrb	w2, [x1, 117]
	mov	w0, 3
	cmp	w2, wzr
	csel	w2, w2, w0, eq
.L588:
	add	x3, x1, 112
	strb	w2, [x19, 2]
	mov	w2, 3
	ldr	w0, [x3, 656]
	str	w0, [x19, 636]
	ldr	w0, [x3, 692]
	str	w0, [x19, 664]
	ldr	w0, [x3, 684]
	str	w0, [x19, 660]
	ldr	w0, [x3, 668]
	str	w0, [x19, 648]
	ldr	w0, [x3, 716]
	str	w0, [x19, 668]
	ldr	w0, [x3, 660]
	str	w0, [x19, 644]
	ldrb	w0, [x3, 9]
	str	w0, [x19, 656]
	ldrb	w0, [x3, 7]
	cmp	w0, 1
	csel	w0, w2, wzr, eq
	strb	w0, [x19, 3]
	ldrb	w4, [x3, 3]
	cmp	w4, 1
	beq	.L591
	cbz	w4, .L592
	cmp	w4, 2
	bne	.L587
	str	x19, [x19, 792]
	mov	w5, 1
	strb	w5, [x19, 760]
	ldrb	w5, [x3, 4]
	strb	w5, [x19, 761]
	ldrb	w5, [x3, 5]
	strb	w5, [x19, 762]
	ldr	w5, [x3, 676]
	str	w5, [x19, 772]
	ldr	w6, [x3, 684]
	str	w6, [x19, 780]
	ldr	w5, [x3, 692]
	str	w5, [x19, 776]
	str	wzr, [x19, 656]
	ldrb	w7, [x3, 1]
	cbz	w7, .L614
	ldr	w4, [x19, 736]
	orr	w0, w0, 2
	strb	w2, [x19, 1]
	mov	w2, 0
	add	w5, w5, w4
	str	w5, [x19, 664]
	str	w5, [x19, 696]
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	csel	w0, w0, w2, eq
	strb	w0, [x19, 3]
	ldr	x0, [x1, 728]
	cmn	x0, #1
	beq	.L600
	ldr	x2, [x19, 608]
	cmn	x2, #1
	beq	.L615
.L600:
	ldr	w2, [x19, 740]
.L618:
	cmp	w2, w6
	mov	x0, x19
	csel	w6, w2, w6, cs
	str	w6, [x19, 660]
	ldr	w1, [x1, 780]
	str	w1, [x19, 648]
	bl	CombineFldsToFrm
.L587:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L592:
	strb	w2, [x19, 1]
	add	x1, x1, 128
	strb	wzr, [x19, 680]
	mov	x2, 624
	str	x19, [x19, 712]
	add	x0, x19, 8
	ldrb	w23, [x3, 4]
	strb	w23, [x19, 681]
	ldrb	w22, [x3, 5]
	strb	w22, [x19, 682]
	ldr	w4, [x3, 672]
	str	w4, [x19, 692]
	ldr	w20, [x3, 684]
	str	w20, [x19, 700]
	ldr	w21, [x3, 692]
	str	w21, [x19, 696]
	ldr	w4, [x3, 676]
	str	w4, [x19, 732]
	ldr	w3, [x3, 680]
	str	w3, [x19, 772]
	bl	memcpy
	strb	w23, [x19, 721]
	strb	w22, [x19, 722]
	mov	w1, 1
	strb	w23, [x19, 761]
	mov	w0, 2
	strb	w1, [x19, 720]
	strb	w22, [x19, 762]
	strb	w0, [x19, 760]
	str	w21, [x19, 736]
	str	w20, [x19, 740]
	str	x19, [x19, 752]
	str	w21, [x19, 776]
	str	w20, [x19, 780]
	str	x19, [x19, 792]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L591:
	strb	w4, [x19, 720]
	str	x19, [x19, 752]
	ldrb	w5, [x3, 4]
	strb	w5, [x19, 721]
	ldrb	w5, [x3, 5]
	strb	w5, [x19, 722]
	ldr	w5, [x3, 676]
	str	w5, [x19, 732]
	ldr	w6, [x3, 684]
	str	w6, [x19, 740]
	ldr	w5, [x3, 692]
	str	w5, [x19, 736]
	str	wzr, [x19, 656]
	ldrb	w7, [x3, 1]
	cbz	w7, .L616
	ldr	w4, [x19, 776]
	orr	w0, w0, 1
	strb	w2, [x19, 1]
	mov	w2, 0
	add	w5, w5, w4
	str	w5, [x19, 664]
	str	w5, [x19, 696]
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	csel	w0, w0, w2, eq
	strb	w0, [x19, 3]
	ldr	x0, [x1, 728]
	cmn	x0, #1
	beq	.L596
	ldr	x2, [x19, 608]
	cmn	x2, #1
	beq	.L617
.L596:
	ldr	w2, [x19, 780]
	mov	x0, x19
	cmp	w2, w6
	csel	w6, w2, w6, cs
	str	w6, [x19, 660]
	ldr	w1, [x1, 780]
	str	w1, [x19, 648]
	bl	CombineFldsToFrm
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L614:
	strb	w4, [x19, 1]
	mov	x2, 624
	str	w5, [x19, 664]
	add	x1, x1, 128
	str	w5, [x19, 696]
	add	x0, x19, 8
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	csel	w7, wzr, w4, ne
	strb	w7, [x19, 3]
	bl	memcpy
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L616:
	strb	w4, [x19, 1]
	mov	x2, 624
	str	w5, [x19, 664]
	add	x1, x1, 128
	str	w5, [x19, 696]
	add	x0, x19, 8
	ldrb	w3, [x3, 7]
	cmp	w3, 1
	cset	w3, eq
	strb	w3, [x19, 3]
	bl	memcpy
	b	.L587
	.p2align 3
.L617:
	str	x0, [x19, 608]
	b	.L596
	.p2align 3
.L615:
	str	x0, [x19, 608]
	ldr	w2, [x19, 740]
	b	.L618
	.size	UpdateCurrFrameInfo, .-UpdateCurrFrameInfo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SimpleSlideDPB
	.type	SimpleSlideDPB, %function
SimpleSlideDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 24576
	ldr	w1, [x0, 3716]
	ldr	w0, [x0, 3712]
	cmp	w1, w0
	bcs	.L625
.L620:
	add	x19, x19, 61440
	ldr	w0, [x19, 776]
	strb	wzr, [x19, 116]
	cbz	w0, .L619
	mov	w0, 1
	strb	w0, [x19, 117]
.L619:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L625:
	ldr	x1, [x19, 27904]
	mov	x0, x19
	bl	UnMarkFrameStoreRef
	mov	w1, 0
	mov	x0, x19
	bl	RemoveFrameStoreOutDPB
	b	.L620
	.size	SimpleSlideDPB, .-SimpleSlideDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReleaseNAL
	.type	ReleaseNAL, %function
ReleaseNAL:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w21, w0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L635
	ldr	x23, .LC69
	add	x19, x20, 8
	strb	wzr, [x20, 1]
	add	x22, x20, 72
	strb	wzr, [x20, 4]
.L630:
	ldr	x1, [x19]
	mov	w0, w21
	cbz	x1, .L629
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
.L629:
	add	x19, x19, 32
	cmp	x19, x22
	bne	.L630
	strb	wzr, [x20]
	str	wzr, [x20, 76]
.L626:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L635:
	ldr	x2, .LC66
	mov	w3, 4204
	ldr	x1, .LC67
	mov	w0, 0
	bl	dprint_vfmw
	b	.L626
	.size	ReleaseNAL, .-ReleaseNAL
	.align	3
.LC66:
	.xword	.LC50
	.align	3
.LC67:
	.xword	.LC52
	.align	3
.LC69:
	.xword	.LC68
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearCurrNal
	.type	ClearCurrNal, %function
ClearCurrNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, [x19, 632]
	cbz	x1, .L637
	add	x0, x19, 65536
	ldr	w0, [x0, 116]
	bl	ReleaseNAL
	str	xzr, [x19, 632]
.L637:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ClearCurrNal, .-ClearCurrNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearCurrSlice
	.type	ClearCurrSlice, %function
ClearCurrSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 65536
	bl	_mcount
	ldr	x1, .LC71
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbz	x1, .L642
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L642:
	ldr	w1, [x19, 72]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 72]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ClearCurrSlice, .-ClearCurrSlice
	.align	3
.LC71:
	.xword	.LC70
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearAllNal
	.type	ClearAllNal, %function
ClearAllNal:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 640
	add	x21, x20, 65536
	add	x20, x20, 12288
	bl	_mcount
	add	x20, x20, 2600
	b	.L648
	.p2align 3
.L647:
	add	x19, x19, 104
	cmp	x19, x20
	beq	.L650
.L648:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L647
	ldr	w0, [x21, 116]
	mov	x1, x19
	add	x19, x19, 104
	bl	ReleaseNAL
	cmp	x19, x20
	bne	.L648
.L650:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	ClearAllNal, .-ClearAllNal
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearAllSlice
	.type	ClearAllSlice, %function
ClearAllSlice:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 61440
	add	x21, x19, 65536
	add	x19, x20, 1616
	add	x20, x20, 2704
	bl	_mcount
	.p2align 2
.L653:
	ldr	x0, [x19]
	mov	x1, x0
	cbz	x0, .L652
	ldr	w0, [x21, 116]
	bl	ReleaseNAL
	str	xzr, [x19]
.L652:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L653
	str	wzr, [x21, 40]
	mov	w0, 0
	str	wzr, [x21, 100]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	ClearAllSlice, .-ClearAllSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_ClearCurrPic
	.type	H264_ClearCurrPic, %function
H264_ClearCurrPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	ClearAllSlice
	add	x0, x19, 20480
	mov	w4, 32
	ldr	w8, [x19, 14948]
	mov	w7, 23352
	movk	w7, 0x1, lsl 16
	mov	w6, 2
	strb	w4, [x0, 3234]
	add	x4, x0, 3072
	strb	wzr, [x0, 3232]
	mov	w5, 7
	strb	w6, [x0, 3233]
	mov	w3, 3
	str	w8, [x0, 3240]
	mov	w2, -1
	str	w5, [x0, 3244]
	mov	w1, 262144
	str	w7, [x0, 3248]
	stp	w3, w2, [x4, 184]
	stp	wzr, wzr, [x4, 192]
	str	w1, [x0, 3272]
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	H264_ClearCurrPic, .-H264_ClearCurrPic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_ArrangeVahbMem
	.type	H264_ArrangeVahbMem, %function
H264_ArrangeVahbMem:
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
	ldr	x3, .LC74
	mov	x2, 32
	mov	w1, 0
	add	x0, x29, 64
	ldr	x3, [x3, 96]
	blr	x3
	strb	wzr, [x29, 64]
	ldr	x0, [x19]
	stp	w20, w21, [x29, 88]
	strb	wzr, [x29, 65]
	ldr	w0, [x0, 28]
	stp	w20, w21, [x29, 72]
	stp	w20, w21, [x29, 80]
	cmp	w0, 24
	beq	.L660
	add	x0, x19, 24576
	mov	w1, 5
	strb	wzr, [x29, 68]
	sxtb	w4, w22
	strb	w1, [x29, 69]
	ldr	w0, [x0, 3712]
	add	w0, w0, 1
	strb	w0, [x29, 67]
.L666:
	add	x20, x19, 65536
	add	x1, x29, 64
	strb	w4, [x29, 70]
	strb	wzr, [x29, 66]
	ldr	w0, [x20, 116]
	bl	FSP_ConfigInstance
	cbnz	w0, .L661
	ldr	x1, [x19]
	add	x3, x29, 60
	ldr	w0, [x20, 116]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	mov	w1, w0
	beq	.L669
	mov	w0, 1
	cbnz	w1, .L663
.L667:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L660:
	mov	w1, 1
	mov	w0, 5
	strb	w1, [x29, 67]
	mov	w4, 1
	strb	wzr, [x29, 68]
	strb	w0, [x29, 69]
	b	.L666
	.p2align 3
.L669:
	add	x0, x19, 8
	bl	ResetVoQueue
.L663:
	ldr	x1, .LC76
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L667
	.p2align 3
.L661:
	ldr	x1, .LC78
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L667
	.size	H264_ArrangeVahbMem, .-H264_ArrangeVahbMem
	.align	3
.LC74:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC76:
	.xword	.LC75
	.align	3
.LC78:
	.xword	.LC77
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	RepairList
	.type	RepairList, %function
RepairList:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x7, x19, 20480
	ldrb	w2, [x7, 1584]
	cmp	w2, 1
	beq	.L689
	cbz	w2, .L673
	cmp	w2, 2
	mov	w0, 0
	bne	.L701
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L701:
	ldr	x1, .LC80
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearCurrSlice
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L673:
	mov	w11, 1
.L672:
	add	x6, x19, 28672
	add	x7, x7, 1628
	add	x6, x6, 32
	mov	x8, 0
	mov	x9, 0
	mov	w2, 0
	mov	w13, 1
	add	x12, x19, 61440
	.p2align 2
.L685:
	ldr	w0, [x7]
	cbz	w0, .L675
	ldr	x1, [x6]
	cbnz	x1, .L702
	sub	w3, w0, #1
	add	x10, x8, 3589
	add	x0, x6, 8
	add	x5, x10, x3, uxtw
	add	x5, x19, x5, lsl 3
	b	.L678
	.p2align 3
.L679:
	ldr	x4, [x0], 8
	cbnz	x4, .L676
.L678:
	cmp	x0, x5
	bne	.L679
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L703
	ldr	x9, [x12, 752]
	add	x9, x9, 680
.L686:
	add	x3, x10, x3, uxtw
	mov	x0, x6
	add	x3, x19, x3, lsl 3
	cbz	x1, .L704
	.p2align 2
.L681:
	add	x0, x0, 8
	cmp	x0, x3
	beq	.L682
.L705:
	ldr	x1, [x0]
	cbnz	x1, .L681
.L704:
	str	x9, [x0]
	add	x0, x0, 8
	cmp	x0, x3
	bne	.L705
	.p2align 2
.L682:
	add	w2, w2, 1
	add	x7, x7, 4
	cmp	w2, w11
	add	x6, x6, 264
	add	x8, x8, 33
	bcc	.L685
.L707:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L675:
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L706
	str	w13, [x7]
	add	w2, w2, 1
	cmp	w2, w11
	add	x7, x7, 4
	ldr	x0, [x12, 752]
	add	x8, x8, 33
	add	x0, x0, 680
	str	x0, [x6]
	add	x6, x6, 264
	bcc	.L685
	b	.L707
.L702:
	sub	w3, w0, #1
	mov	x4, x1
	add	x10, x8, 3589
	.p2align 2
.L676:
	mov	x9, x4
	b	.L686
.L689:
	mov	w11, 2
	b	.L672
.L706:
	ldr	x1, .LC84
	mov	w3, 0
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearCurrSlice
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L703:
	ldr	x1, .LC82
	mov	w3, 0
	mov	x2, x9
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearCurrSlice
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
	.size	RepairList, .-RepairList
	.align	3
.LC80:
	.xword	.LC79
	.align	3
.LC82:
	.xword	.LC81
	.align	3
.LC84:
	.xword	.LC83
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetShortTermPicPoint
	.type	GetShortTermPicPoint, %function
GetShortTermPicPoint:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	x0, x20, 20480
	mov	w5, 0
	ldrb	w1, [x0, 1585]
	cbz	w1, .L709
	ldrb	w0, [x0, 1586]
	mov	w5, 2
	cmp	w0, wzr
	csinc	w5, w5, wzr, ne
.L709:
	add	x0, x20, 24576
	ldr	w2, [x0, 3720]
	cbz	w2, .L717
	sub	w2, w2, #1
	add	x0, x0, 3456
	add	x2, x2, 3505
	add	x4, x20, x2, lsl 3
	b	.L714
	.p2align 3
.L727:
	cmp	w3, 3
	beq	.L726
.L712:
	add	x0, x0, 8
	cmp	x0, x4
	beq	.L717
.L714:
	ldr	x2, [x0]
	ldrb	w3, [x2, 2]
	cbz	w5, .L727
	tbz	x3, 0, .L713
	ldr	w6, [x2, 720]
	and	w6, w6, 16776960
	cmp	w6, 65536
	beq	.L728
.L713:
	tbz	x3, 1, .L712
	ldr	w3, [x2, 760]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L712
	ldr	w1, [x2, 768]
	cmp	w19, w1
	bne	.L712
	add	x0, x2, 760
	b	.L710
	.p2align 3
.L728:
	ldr	w1, [x2, 728]
	cmp	w1, w19
	bne	.L713
	add	x0, x2, 720
	b	.L710
	.p2align 3
.L726:
	ldr	w3, [x2, 680]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L712
	ldr	w1, [x2, 688]
	cmp	w19, w1
	bne	.L712
	add	x0, x2, 680
	b	.L710
	.p2align 3
.L717:
	mov	x0, 0
.L710:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	GetShortTermPicPoint, .-GetShortTermPicPoint
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetLongTermPicPoint
	.type	GetLongTermPicPoint, %function
GetLongTermPicPoint:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	add	x0, x20, 20480
	mov	w5, 0
	ldrb	w1, [x0, 1585]
	cbz	w1, .L730
	ldrb	w0, [x0, 1586]
	mov	w5, 2
	cmp	w0, wzr
	csinc	w5, w5, wzr, ne
.L730:
	add	x0, x20, 24576
	ldr	w2, [x0, 3724]
	cbz	w2, .L738
	sub	w2, w2, #1
	add	x0, x0, 3584
	add	x2, x2, 3521
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
	ldrb	w3, [x2, 2]
	cbz	w5, .L748
	tbz	x3, 0, .L734
	ldr	w6, [x2, 720]
	and	w6, w6, 16776960
	cmp	w6, 256
	beq	.L749
.L734:
	tbz	x3, 1, .L733
	ldr	w3, [x2, 760]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L733
	ldr	w1, [x2, 764]
	cmp	w19, w1
	bne	.L733
	add	x0, x2, 760
	b	.L731
	.p2align 3
.L749:
	ldr	w1, [x2, 724]
	cmp	w1, w19
	bne	.L734
	add	x0, x2, 720
	b	.L731
	.p2align 3
.L747:
	ldr	w3, [x2, 680]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L733
	ldr	w1, [x2, 684]
	cmp	w19, w1
	bne	.L733
	add	x0, x2, 680
	b	.L731
	.p2align 3
.L738:
	mov	x0, 0
.L731:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	GetLongTermPicPoint, .-GetLongTermPicPoint
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReorderSTList
	.type	ReorderSTList, %function
ReorderSTList:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	mov	w22, w3
	mov	x21, x4
	mov	w20, w2
	mov	x19, x1
	bl	_mcount
	mov	w1, w22
	mov	x0, x23
	bl	GetShortTermPicPoint
	ldr	w5, [x21]
	add	w2, w20, 1
	cmp	w2, w5
	ble	.L751
	mov	w1, w2
	.p2align 2
.L752:
	add	x5, x19, x1, sxtw 3
	ldr	x5, [x5, -8]
	str	x5, [x19, w1, sxtw 3]
	sub	w1, w1, #1
	ldr	w5, [x21]
	cmp	w5, w1
	blt	.L752
.L751:
	add	w1, w5, 1
	str	w1, [x21]
	str	x0, [x19, w5, sxtw 3]
	ldr	w0, [x21]
	cmp	w2, w0
	mov	w4, w0
	blt	.L750
	.p2align 2
.L760:
	ldr	x1, [x19, w0, sxtw 3]
	add	w0, w0, 1
	cbz	x1, .L754
	ldrb	w5, [x1, 1]
	cbnz	w5, .L755
	ldr	w5, [x1, 8]
	cmp	w22, w5
	beq	.L754
.L755:
	str	x1, [x19, w4, sxtw 3]
	add	w4, w4, 1
.L754:
	cmp	w2, w0
	bge	.L760
.L750:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	ReorderSTList, .-ReorderSTList
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReorderLTList
	.type	ReorderLTList, %function
ReorderLTList:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	mov	w22, w3
	mov	x21, x4
	mov	w20, w2
	mov	x19, x1
	bl	_mcount
	mov	w1, w22
	mov	x0, x23
	bl	GetLongTermPicPoint
	ldr	w5, [x21]
	add	w2, w20, 1
	cmp	w2, w5
	ble	.L765
	mov	w1, w2
	.p2align 2
.L766:
	add	x5, x19, x1, sxtw 3
	ldr	x5, [x5, -8]
	str	x5, [x19, w1, sxtw 3]
	sub	w1, w1, #1
	ldr	w5, [x21]
	cmp	w5, w1
	blt	.L766
.L765:
	add	w1, w5, 1
	str	w1, [x21]
	str	x0, [x19, w5, sxtw 3]
	ldr	w0, [x21]
	cmp	w2, w0
	mov	w4, w0
	blt	.L764
	.p2align 2
.L776:
	ldr	x1, [x19, w0, sxtw 3]
	add	w0, w0, 1
	cbz	x1, .L768
	ldrb	w5, [x1, 1]
	cbz	w5, .L769
	ldr	w5, [x1, 4]
	cmp	w22, w5
	beq	.L768
.L769:
	str	x1, [x19, w4, sxtw 3]
	add	w4, w4, 1
.L768:
	cmp	w2, w0
	bge	.L776
.L764:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	ReorderLTList, .-ReorderLTList
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReorderRefPiclist
	.type	ReorderRefPiclist, %function
ReorderRefPiclist:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	mov	x22, x1
	mov	w21, w3
	mov	x25, x4
	mov	x28, x5
	str	x6, [x29, 104]
	bl	_mcount
	add	x0, x20, 20480
	str	wzr, [x29, 124]
	ldrb	w1, [x0, 1585]
	ldr	w2, [x0, 1596]
	cbz	w1, .L782
	mov	w1, 2208
	ldr	w26, [x0, 1604]
	ldr	x3, [x20, 14968]
	mov	w23, 2
	umull	x0, w2, w1
	ldr	x2, [x20, 14960]
	lsl	w26, w26, 1
	add	x1, x3, x0
	mov	w0, 4000
	add	w26, w26, 1
	ldr	w1, [x1, 28]
	umull	x0, w1, w0
	add	x0, x2, x0
	ldr	w0, [x0, 2196]
	add	w0, w0, 4
	lsl	w23, w23, w0
.L792:
	cmn	w21, #2
	beq	.L781
	ldr	w3, [x25]
	cmp	w3, 3
	beq	.L781
	add	w24, w21, 1
	mov	w27, w26
	lsl	x24, x24, 2
	mov	x19, 0
	b	.L785
	.p2align 3
.L786:
	ldr	x3, [x29, 104]
	ldr	w3, [x3, x19]
	bl	ReorderLTList
	cmp	x24, x19
	add	x19, x19, 4
	beq	.L781
.L791:
	ldr	w3, [x25, x19]
	cmp	w3, 3
	beq	.L781
.L785:
	cmp	w3, 1
	add	x4, x29, 124
	mov	w2, w21
	mov	x1, x22
	mov	x0, x20
	bhi	.L786
	ldr	w0, [x28, x19]
	cbnz	w3, .L787
	mvn	w0, w0
	add	w27, w27, w0
	add	w0, w27, w23
	cmp	w27, wzr
	csel	w27, w0, w27, lt
.L788:
	cmp	w26, w27
	sub	w3, w27, w23
	csel	w3, w3, w27, lt
	add	x4, x29, 124
	mov	w2, w21
	mov	x1, x22
	mov	x0, x20
	bl	ReorderSTList
	cmp	x24, x19
	add	x19, x19, 4
	bne	.L791
.L781:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L787:
	add	w0, w0, 1
	add	w27, w27, w0
	cmp	w23, w27
	sub	w0, w27, w23
	csel	w27, w0, w27, le
	b	.L788
	.p2align 3
.L782:
	mov	w1, 2208
	ldr	w26, [x0, 1604]
	ldr	x3, [x20, 14968]
	mov	w23, 1
	umull	x0, w2, w1
	ldr	x2, [x20, 14960]
	add	x1, x3, x0
	mov	w0, 4000
	ldr	w1, [x1, 28]
	umull	x0, w1, w0
	add	x0, x2, x0
	ldr	w0, [x0, 2196]
	add	w0, w0, 4
	lsl	w23, w23, w0
	b	.L792
	.size	ReorderRefPiclist, .-ReorderRefPiclist
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReorderListX
	.type	ReorderListX, %function
ReorderListX:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 20480
	bl	_mcount
	ldrb	w0, [x19, 1584]
	cmp	w0, 2
	beq	.L800
	ldrb	w1, [x19, 796]
	cbnz	w1, .L812
.L803:
	ldr	w1, [x19, 1640]
	cmp	w0, 1
	add	w0, w1, 1
	str	w0, [x19, 1628]
	bne	.L800
	ldrb	w0, [x19, 797]
	cbnz	w0, .L813
.L805:
	ldr	w0, [x19, 1644]
	add	w0, w0, 1
	str	w0, [x19, 1632]
.L800:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L812:
	add	x1, x20, 28672
	ldr	w3, [x19, 1640]
	mov	x0, x20
	add	x6, x19, 1328
	add	x5, x19, 1064
	add	x4, x19, 800
	add	x2, x19, 1628
	add	x1, x1, 32
	bl	ReorderRefPiclist
	ldrb	w0, [x19, 1584]
	b	.L803
	.p2align 3
.L813:
	add	x1, x20, 28672
	ldr	w3, [x19, 1644]
	add	x6, x19, 1456
	add	x5, x19, 1196
	add	x4, x19, 932
	add	x2, x19, 1632
	add	x1, x1, 296
	mov	x0, x20
	bl	ReorderRefPiclist
	b	.L805
	.size	ReorderListX, .-ReorderListX
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GenPiclistfromFrmlist
	.type	GenPiclistfromFrmlist, %function
GenPiclistfromFrmlist:
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
	cbnz	w23, .L836
	cmp	w21, 1
	ldr	x23, .LC86
	beq	.L876
.L816:
	cmp	w21, 2
	bne	.L825
	mov	w21, 0
	mov	w20, 0
	.p2align 2
.L827:
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	bcs	.L825
.L877:
	cmp	w21, w22
	bcc	.L860
	b	.L832
	.p2align 3
.L828:
	add	w21, w21, 1
	cmp	w22, w21
	bls	.L832
.L860:
	ldr	x0, [x19, w21, uxtw 3]
	add	x26, x19, x21, uxtw 3
	ldrb	w1, [x0, 1]
	tbz	x1, 1, .L828
	add	x0, x0, 760
	blr	x23
	cbz	w0, .L828
	ldr	w1, [x24]
	add	w21, w21, 1
	ldr	x0, [x26]
	add	x0, x0, 760
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
.L832:
	cmp	w20, w22
	bcc	.L859
	b	.L827
	.p2align 3
.L833:
	add	w20, w20, 1
	cmp	w22, w20
	bls	.L827
.L859:
	ldr	x0, [x19, w20, uxtw 3]
	add	x26, x19, x20, uxtw 3
	ldrb	w1, [x0, 1]
	tbz	x1, 0, .L833
	add	x0, x0, 720
	blr	x23
	cbz	w0, .L833
	ldr	w1, [x24]
	add	w20, w20, 1
	ldr	x0, [x26]
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	add	x0, x0, 720
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	bcc	.L877
.L825:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L836:
	cmp	w21, 1
	ldr	x23, .LC85
	bne	.L816
.L876:
	mov	w21, 0
	mov	w20, 0
	.p2align 2
.L817:
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	bcs	.L825
	cmp	w20, w22
	bcc	.L858
	b	.L875
	.p2align 3
.L818:
	add	w20, w20, 1
	cmp	w20, w22
	bcs	.L875
.L858:
	ldr	x0, [x19, w20, uxtw 3]
	add	x26, x19, x20, uxtw 3
	ldrb	w1, [x0, 1]
	tbz	x1, 0, .L818
	add	x0, x0, 720
	blr	x23
	cbz	w0, .L818
	ldr	w1, [x24]
	add	w20, w20, 1
	ldr	x0, [x26]
	add	x0, x0, 720
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	.p2align 2
.L875:
	cmp	w21, w22
	bcs	.L817
	ldr	x0, [x19, w21, uxtw 3]
	add	x26, x19, x21, uxtw 3
	ldrb	w1, [x0, 1]
	tbnz	x1, 1, .L878
.L823:
	add	w21, w21, 1
	b	.L875
	.p2align 3
.L878:
	add	x0, x0, 760
	blr	x23
	cbz	w0, .L823
	ldr	w1, [x24]
	add	w21, w21, 1
	ldr	x0, [x26]
	add	x0, x0, 760
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	b	.L817
	.size	GenPiclistfromFrmlist, .-GenPiclistfromFrmlist
	.align	3
.LC85:
	.xword	IsLTRefFlg
	.align	3
.LC86:
	.xword	IsSTRefFlg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InitListX
	.type	InitListX, %function
InitListX:
	stp	x29, x30, [sp, -480]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 20480
	add	x22, x19, 61440
	bl	_mcount
	ldrb	w0, [x20, 1584]
	cbnz	w0, .L880
	ldrb	w0, [x22, 115]
	add	x21, x19, 24576
	cbnz	w0, .L1000
	ldr	w0, [x21, 3720]
	cbz	w0, .L947
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 3505
	add	x0, x21, 3456
	add	x3, x19, x3, lsl 3
	b	.L886
	.p2align 3
.L885:
	cmp	x3, x0
	beq	.L884
.L886:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cmp	w2, 3
	bne	.L885
	ldr	w2, [x1, 680]
	and	w2, w2, 16776960
	cmp	w2, 65536
	bne	.L885
	add	x2, x19, x23, uxtw 3
	add	x1, x1, 680
	cmp	x3, x0
	add	w23, w23, 1
	str	x1, [x2, 28704]
	bne	.L886
	.p2align 2
.L884:
	add	x0, x19, 28672
	ldr	x3, .LC87
	mov	w1, w23
	mov	w2, 8
	add	x0, x0, 32
	bl	qsort
	str	w23, [x20, 1628]
	ldr	w1, [x21, 3724]
	cbz	w1, .L948
	sub	w3, w1, #1
	add	x0, x21, 3584
	add	x3, x3, 3521
	mov	w21, w23
	add	x3, x19, x3, lsl 3
	b	.L889
	.p2align 3
.L888:
	cmp	x0, x3
	beq	.L1001
.L889:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cmp	w2, 3
	bne	.L888
	ldr	w2, [x1, 680]
	and	w2, w2, 16776960
	cmp	w2, 256
	bne	.L888
	add	x2, x19, x21, uxtw 3
	add	x1, x1, 680
	cmp	x0, x3
	add	w21, w21, 1
	str	x1, [x2, 28704]
	bne	.L889
	.p2align 2
.L1001:
	sub	w1, w21, w23
.L887:
	uxtw	x0, w23
	ldr	x3, .LC88
	add	x0, x0, 3588
	mov	w2, 8
	add	x0, x19, x0, lsl 3
	bl	qsort
	str	w21, [x20, 1628]
	mov	w2, w21
	str	wzr, [x20, 1632]
	cbz	w2, .L1002
.L895:
	ldr	w0, [x20, 1640]
	mov	w1, 0
	str	wzr, [x20, 1632]
	add	w0, w0, 1
	cmp	w0, w2
	csel	w0, w0, w2, ls
	str	w0, [x20, 1628]
	cmp	w0, 32
	bhi	.L942
	add	x2, x19, x0, uxtw 3
	add	w0, w0, 1
	cmp	w0, 33
	str	xzr, [x2, 28704]
	bne	.L940
	.p2align 2
.L941:
	cmp	w1, 32
	bhi	.L939
	.p2align 2
.L942:
	add	x0, x19, x1, uxtw 3
	add	w1, w1, 1
	cmp	w1, 33
	str	xzr, [x0, 28968]
	bne	.L942
.L939:
	mov	w0, 0
.L991:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 480
	ret
	.p2align 3
.L880:
	ldrb	w0, [x22, 115]
	add	x21, x19, 24576
	cbz	w0, .L901
	ldr	w0, [x21, 3720]
	cbz	w0, .L1003
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 3505
	add	x0, x21, 3456
	add	x3, x19, x3, lsl 3
	.p2align 2
.L920:
	ldr	x2, [x0]
	add	x0, x0, 8
	ldrb	w1, [x2, 1]
	cbz	w1, .L919
	ldr	w4, [x2, 648]
	add	x1, x29, 480
	ldr	w5, [x22, 780]
	add	x1, x1, x23, uxtw 3
	sub	x1, x1, #4096
	cmp	w5, w4
	blt	.L919
	add	w23, w23, 1
	str	x2, [x1, 3712]
.L919:
	cmp	x0, x3
	bne	.L920
	add	x26, x29, 96
	ldr	x3, .LC95
	mov	w2, 8
	mov	w1, w23
	mov	x0, x26
	bl	qsort
	ldr	w24, [x21, 3720]
	cbz	w24, .L921
.L945:
	sub	w1, w24, #1
	add	x0, x21, 3456
	add	x1, x1, 3505
	mov	w25, w23
	add	x1, x19, x1, lsl 3
	.p2align 2
.L923:
	ldr	x3, [x0]
	add	x0, x0, 8
	ldrb	w2, [x3, 1]
	cbz	w2, .L922
	ldr	w4, [x3, 648]
	add	x2, x29, 480
	ldr	w5, [x22, 780]
	add	x2, x2, x25, uxtw 3
	sub	x2, x2, #4096
	cmp	w5, w4
	bge	.L922
	add	w25, w25, 1
	str	x3, [x2, 3712]
.L922:
	cmp	x0, x1
	bne	.L923
	sub	w24, w25, w23
	ldr	x3, .LC96
	mov	w2, 8
	mov	w1, w24
	add	x0, x26, x23, uxtw 3
	bl	qsort
	cbz	w23, .L924
.L944:
	mov	x0, 0
	.p2align 2
.L925:
	add	w1, w24, w0
	add	x3, x29, 480
	ldr	x2, [x26, x0, lsl 3]
	add	x0, x0, 1
	add	x1, x3, x1, lsl 3
	cmp	w23, w0
	str	x2, [x1, -256]
	bhi	.L925
.L924:
	cmp	w25, w23
	bls	.L926
	mov	w0, w23
	.p2align 2
.L927:
	add	x2, x29, 480
	sub	w1, w0, w23
	add	x2, x2, x0, uxtw 3
	add	x3, x29, 480
	sub	x2, x2, #4096
	add	x1, x3, x1, lsl 3
	add	w0, w0, 1
	cmp	w25, w0
	ldr	x2, [x2, 3712]
	str	x2, [x1, -256]
	bne	.L927
.L926:
	str	wzr, [x20, 1628]
	add	x23, x19, 28672
	str	wzr, [x20, 1632]
	add	x24, x23, 32
	add	x27, x20, 1628
	mov	x1, x26
	ldrb	w0, [x22, 115]
	mov	w2, w25
	mov	w5, 0
	mov	x4, x27
	mov	x3, x24
	add	x23, x23, 296
	add	x26, x20, 1632
	bl	GenPiclistfromFrmlist
	ldrb	w0, [x22, 115]
	mov	w2, w25
	mov	w5, 0
	mov	x4, x26
	mov	x3, x23
	add	x1, x29, 224
	add	x25, x29, 352
	bl	GenPiclistfromFrmlist
	ldr	w28, [x21, 3724]
	cbz	w28, .L928
	add	x21, x21, 3584
	mov	x0, 0
	add	x25, x29, 352
	.p2align 2
.L929:
	ldr	x1, [x21], 8
	str	x1, [x25, x0, lsl 3]
	add	x0, x0, 1
	cmp	w28, w0
	bhi	.L929
.L928:
	ldr	x3, .LC90
	mov	w2, 8
	mov	w1, w28
	mov	x0, x25
	bl	qsort
	ldrb	w0, [x22, 115]
	mov	w5, 1
	mov	x4, x27
	mov	x3, x24
	mov	w2, w28
	mov	x1, x25
	bl	GenPiclistfromFrmlist
	ldrb	w0, [x22, 115]
	mov	x4, x26
	mov	x3, x23
	mov	x1, x25
	mov	w5, 1
	mov	w2, w28
	bl	GenPiclistfromFrmlist
	ldr	w3, [x20, 1628]
	ldr	w4, [x20, 1632]
	cmp	w4, w3
	cset	w1, eq
.L918:
	cmp	w3, 1
	cset	w0, hi
	tst	w1, w0
	beq	.L930
	cbz	w3, .L931
	ldr	x6, [x19, 28704]
	ldr	x0, [x19, 28968]
	cmp	x6, x0
	bne	.L930
	sub	w0, w3, #1
	mov	x2, x19
	add	x0, x19, x0, lsl 3
.L933:
	cmp	x0, x2
	beq	.L935
	ldr	x5, [x2, 28712]
	add	x2, x2, 8
	ldr	x1, [x2, 28968]
	cmp	x5, x1
	beq	.L933
	.p2align 2
.L930:
	orr	w0, w3, w4
	cbz	w0, .L1004
	ldr	w0, [x20, 1640]
	ldr	w1, [x20, 1644]
	add	w0, w0, 1
	add	w1, w1, 1
	cmp	w0, w3
	csel	w0, w0, w3, ls
	cmp	w1, w4
	csel	w1, w1, w4, ls
	str	w0, [x20, 1628]
	cmp	w0, 32
	str	w1, [x20, 1632]
	bhi	.L941
	.p2align 2
.L940:
	add	x2, x19, x0, uxtw 3
	add	w0, w0, 1
	cmp	w0, 33
	str	xzr, [x2, 28704]
	bne	.L940
	b	.L941
	.p2align 3
.L901:
	ldr	w0, [x21, 3720]
	cbz	w0, .L949
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 3505
	add	x0, x21, 3456
	add	x3, x19, x3, lsl 3
	b	.L906
	.p2align 3
.L905:
	cmp	x0, x3
	beq	.L904
.L906:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cmp	w2, 3
	bne	.L905
	ldr	w2, [x1, 692]
	add	x5, x19, x23, uxtw 3
	ldr	w4, [x22, 784]
	add	x1, x1, 680
	cmp	w4, w2
	blt	.L905
	str	x1, [x5, 28704]
	cmp	x0, x3
	add	w23, w23, 1
	bne	.L906
	.p2align 2
.L904:
	add	x25, x19, 28672
	ldr	x3, .LC93
	mov	w2, 8
	mov	w1, w23
	add	x0, x25, 32
	bl	qsort
	ldr	w24, [x21, 3720]
	cbz	w24, .L907
	sub	w2, w24, #1
	add	x0, x21, 3456
	add	x2, x2, 3505
	mov	w26, w23
	add	x2, x19, x2, lsl 3
	b	.L909
	.p2align 3
.L908:
	cmp	x0, x2
	beq	.L1005
.L909:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w3, [x1, 2]
	cmp	w3, 3
	bne	.L908
	ldr	w3, [x1, 692]
	add	x5, x19, x26, uxtw 3
	ldr	w4, [x22, 784]
	add	x1, x1, 680
	cmp	w4, w3
	bge	.L908
	str	x1, [x5, 28704]
	cmp	x0, x2
	add	w26, w26, 1
	bne	.L909
	.p2align 2
.L1005:
	uxtw	x0, w23
	sub	w24, w26, w23
	add	x0, x0, 3588
	ldr	x3, .LC94
	uxtw	x22, w26
	mov	w2, 8
	add	x0, x19, x0, lsl 3
	mov	w1, w24
	add	x22, x22, 3588
	bl	qsort
	add	x22, x19, x22, lsl 3
	cbz	w23, .L911
.L943:
	add	x25, x25, 32
	mov	w0, 0
	.p2align 2
.L912:
	add	w1, w24, w0
	ldr	x2, [x25], 8
	add	x1, x1, 3621
	add	w0, w0, 1
	cmp	w0, w23
	str	x2, [x19, x1, lsl 3]
	bcc	.L912
.L911:
	cmp	w26, w23
	bls	.L913
	mov	w0, w23
	.p2align 2
.L914:
	sub	w1, w0, w23
	add	x2, x19, x0, uxtw 3
	add	x1, x1, 3621
	add	w0, w0, 1
	cmp	w0, w26
	ldr	x2, [x2, 28704]
	str	x2, [x19, x1, lsl 3]
	bne	.L914
.L913:
	str	w26, [x20, 1628]
	str	w26, [x20, 1632]
	ldr	w1, [x21, 3724]
	cbz	w1, .L950
	sub	w3, w1, #1
	add	x0, x21, 3584
	add	x3, x3, 3521
	mov	w21, w26
	add	x3, x19, x3, lsl 3
	b	.L917
	.p2align 3
.L916:
	cmp	x3, x0
	beq	.L1006
.L917:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 1]
	cmp	w2, 3
	bne	.L916
	ldrb	w2, [x1, 681]
	cmp	w2, 1
	bne	.L916
	add	x2, x19, x21, uxtw 3
	add	x1, x1, 680
	cmp	x3, x0
	add	w21, w21, 1
	str	x1, [x2, 28704]
	str	x1, [x2, 28968]
	bne	.L917
	.p2align 2
.L1006:
	sub	w1, w21, w26
.L915:
	ldr	x23, .LC88
	mov	w2, 8
	mov	x0, x22
	mov	x3, x23
	bl	qsort
	ldr	w0, [x20, 1628]
	mov	x3, x23
	mov	w2, 8
	sub	w1, w21, w0
	uxtw	x0, w0
	add	x0, x0, 3621
	add	x0, x19, x0, lsl 3
	bl	qsort
	str	w21, [x20, 1628]
	mov	w4, w21
	mov	w3, w21
	mov	w1, 1
	str	w21, [x20, 1632]
	b	.L918
	.p2align 3
.L1000:
	ldr	w0, [x21, 3720]
	mov	w23, 0
	cbz	w0, .L883
	sub	w4, w0, #1
	mov	w23, 0
	add	x4, x4, 3505
	add	x0, x21, 3456
	add	x4, x19, x4, lsl 3
	.p2align 2
.L892:
	ldr	x2, [x0]
	add	x1, x29, 480
	add	x1, x1, x23, uxtw 3
	add	x0, x0, 8
	sub	x1, x1, #4096
	cmp	x4, x0
	ldrb	w3, [x2, 2]
	cbz	w3, .L891
	add	w23, w23, 1
	str	x2, [x1, 3712]
.L891:
	bne	.L892
.L883:
	ldr	x3, .LC89
	mov	w1, w23
	mov	w2, 8
	add	x0, x29, 96
	add	x24, x19, 28672
	add	x26, x20, 1628
	bl	qsort
	str	wzr, [x20, 1628]
	add	x24, x24, 32
	mov	w2, w23
	ldrb	w0, [x22, 115]
	mov	w5, 0
	mov	x4, x26
	add	x1, x29, 96
	mov	x3, x24
	add	x25, x29, 352
	bl	GenPiclistfromFrmlist
	ldr	w23, [x21, 3724]
	cbz	w23, .L893
	add	x21, x21, 3584
	mov	x0, 0
	add	x25, x29, 352
	.p2align 2
.L894:
	ldr	x1, [x21], 8
	str	x1, [x25, x0, lsl 3]
	add	x0, x0, 1
	cmp	w23, w0
	bhi	.L894
.L893:
	ldr	x3, .LC90
	mov	w2, 8
	mov	w1, w23
	mov	x0, x25
	bl	qsort
	ldrb	w0, [x22, 115]
	mov	w2, w23
	mov	w5, 1
	mov	x4, x26
	mov	x3, x24
	mov	x1, x25
	bl	GenPiclistfromFrmlist
	str	wzr, [x20, 1632]
	ldr	w2, [x20, 1628]
	cbnz	w2, .L895
.L1002:
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L896
	ldrb	w0, [x22, 115]
	cbz	w0, .L1007
	cmp	w0, 1
	ldr	x0, [x22, 752]
	beq	.L1008
	add	x0, x0, 760
	mov	w2, 1
	str	x0, [x19, 28704]
	b	.L895
	.p2align 3
.L1007:
	ldr	x0, [x22, 752]
	mov	w2, 1
	add	x0, x0, 680
	str	x0, [x19, 28704]
	b	.L895
.L931:
	ldr	x6, [x19, 28968]
.L935:
	ldr	x0, [x19, 28976]
	str	x0, [x19, 28968]
	str	x6, [x19, 28976]
	b	.L930
.L1008:
	add	x0, x0, 720
	mov	w2, 1
	str	x0, [x19, 28704]
	b	.L895
.L1003:
	add	x26, x29, 96
	ldr	x3, .LC95
	mov	w2, 8
	mov	w1, 0
	mov	x0, x26
	mov	w23, 0
	bl	qsort
	ldr	w24, [x21, 3720]
	cbnz	w24, .L945
	ldr	x3, .LC96
	mov	w2, 8
	mov	w1, 0
	mov	x0, x26
	mov	w25, 0
	bl	qsort
	b	.L926
	.p2align 3
.L949:
	mov	w23, 0
	b	.L904
.L907:
	uxtw	x22, w23
	ldr	x3, .LC94
	add	x22, x22, 3588
	mov	w2, 8
	mov	w1, 0
	mov	w26, w23
	add	x22, x19, x22, lsl 3
	mov	x0, x22
	bl	qsort
	cbnz	w23, .L943
	b	.L913
	.p2align 3
.L950:
	mov	w21, w26
	b	.L915
.L948:
	mov	w21, w23
	b	.L887
.L947:
	mov	w23, 0
	b	.L884
.L921:
	ldr	x3, .LC96
	mov	w2, 8
	mov	w1, 0
	add	x0, x26, x23, uxtw 3
	mov	w25, w23
	bl	qsort
	cbnz	w23, .L944
	b	.L926
	.p2align 3
.L896:
	ldr	x1, .LC92
.L996:
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearCurrSlice
	mov	w0, -1
	b	.L991
.L1004:
	ldr	x1, .LC98
	b	.L996
	.size	InitListX, .-InitListX
	.align	3
.LC87:
	.xword	compare_pic_by_pic_num_desc
	.align	3
.LC88:
	.xword	compare_pic_by_lt_pic_num_asc
	.align	3
.LC89:
	.xword	compare_fs_by_frame_num_desc
	.align	3
.LC90:
	.xword	compare_fs_by_lt_pic_idx_asc
	.align	3
.LC92:
	.xword	.LC91
	.align	3
.LC93:
	.xword	compare_pic_by_poc_desc
	.align	3
.LC94:
	.xword	compare_pic_by_poc_asc
	.align	3
.LC95:
	.xword	compare_fs_by_poc_desc
	.align	3
.LC96:
	.xword	compare_fs_by_poc_asc
	.align	3
.LC98:
	.xword	.LC97
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DumpList
	.type	DumpList, %function
DumpList:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, .LC99
	ldr	w0, [x0]
	tbz	x0, 13, .L1009
	mov	x0, x19
	bl	DumpList.part.5
.L1009:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	DumpList, .-DumpList
	.align	3
.LC99:
	.xword	g_PrintEnable
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FindNearestPOCPicId
	.type	FindNearestPOCPicId, %function
FindNearestPOCPicId:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 24576
	ldr	w0, [x2, 3716]
	cbz	w0, .L1018
	add	x1, x19, 61440
	ldr	x3, [x19, 27904]
	cmp	w0, 1
	ldr	w7, [x1, 780]
	ldr	w1, [x3, 648]
	sub	w1, w7, w1
	sxtw	x1, w1
	eor	x6, x1, x1, asr 63
	sub	x6, x6, x1, asr 63
	bls	.L1019
	sub	w0, w0, #2
	add	x2, x2, 3336
	add	x1, x0, 3490
	mov	w0, 0
	add	x5, x19, x1, lsl 3
	.p2align 2
.L1017:
	ldr	x4, [x2]
	add	x2, x2, 8
	ldr	w1, [x4, 648]
	sub	w1, w7, w1
	sxtw	x1, w1
	eor	x3, x1, x1, asr 63
	sub	x3, x3, x1, asr 63
	cmp	w3, w6
	bge	.L1016
	ldr	w0, [x4, 164]
	mov	w6, w3
.L1016:
	cmp	x2, x5
	bne	.L1017
.L1015:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1019:
	mov	w0, 0
	b	.L1015
.L1018:
	mov	w0, -1
	b	.L1015
	.size	FindNearestPOCPicId, .-FindNearestPOCPicId
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FindMinRefIdx
	.type	FindMinRefIdx, %function
FindMinRefIdx:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 20480
	ldrb	w1, [x0, 1584]
	cmp	w1, 2
	beq	.L1021
	add	x1, x19, 61440
	ldrb	w1, [x1, 115]
	cbnz	w1, .L1051
	add	x6, x19, 24576
	ldr	w5, [x0, 1628]
	add	x6, x6, 4000
	mov	w4, 0
	add	x7, x19, 28672
	.p2align 2
.L1025:
	cbz	w5, .L1029
	add	x1, x7, 32
	mov	w3, 32
	mov	w0, 0
	b	.L1027
	.p2align 3
.L1026:
	add	w0, w0, 1
	cmp	w0, w5
	beq	.L1052
.L1027:
	ldr	x2, [x1]
	add	x1, x1, 8
	ldr	x2, [x2, 32]
	ldr	w2, [x2, 672]
	cmp	w2, w4
	bne	.L1026
	cmp	w3, w0
	csel	w3, w3, w0, ls
	add	w0, w0, 1
	stp	w3, w3, [x6]
	cmp	w0, w5
	bne	.L1027
.L1052:
	cmp	w3, 31
	bhi	.L1029
.L1028:
	add	w4, w4, 1
	add	x6, x6, 8
	cmp	w4, 16
	bne	.L1025
.L1021:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1051:
	ldr	w6, [x0, 1628]
	add	x7, x19, 24576
	add	x7, x7, 4000
	mov	w4, 0
	add	x8, x19, 28672
	cbz	w6, .L1033
	.p2align 2
.L1054:
	add	x2, x8, 32
	mov	w5, 32
	mov	w1, 0
	b	.L1031
	.p2align 3
.L1030:
	add	w1, w1, 1
	cmp	w1, w6
	beq	.L1053
.L1031:
	ldr	x0, [x2]
	add	x2, x2, 8
	ldr	x3, [x0, 32]
	ldrb	w0, [x0]
	cmp	w0, 2
	ldr	w0, [x3, 672]
	cset	w3, eq
	orr	w0, w3, w0, lsl 1
	cmp	w0, w4
	bne	.L1030
	cmp	w5, w1
	csel	w5, w5, w1, ls
	add	w1, w1, 1
	str	w5, [x7]
	cmp	w1, w6
	bne	.L1031
.L1053:
	cmp	w5, 31
	bhi	.L1033
.L1032:
	add	w4, w4, 1
	add	x7, x7, 4
	cmp	w4, 32
	beq	.L1021
	cbnz	w6, .L1054
	.p2align 2
.L1033:
	str	wzr, [x7]
	b	.L1032
	.p2align 3
.L1029:
	stp	wzr, wzr, [x6]
	b	.L1028
	.size	FindMinRefIdx, .-FindMinRefIdx
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecList
	.type	DecList, %function
DecList:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 24576
	mov	w7, -1
	ldr	w0, [x1, 3716]
	cbz	w0, .L1056
	add	x2, x19, 61440
	ldr	x3, [x19, 27904]
	cmp	w0, 1
	ldr	w6, [x2, 780]
	ldr	w2, [x3, 648]
	sub	w2, w6, w2
	sxtw	x2, w2
	eor	x5, x2, x2, asr 63
	sub	x5, x5, x2, asr 63
	bls	.L1065
	sub	w4, w0, #2
	add	x1, x1, 3336
	add	x4, x4, 3490
	mov	w7, 0
	add	x4, x19, x4, lsl 3
	.p2align 2
.L1058:
	ldr	x3, [x1]
	add	x1, x1, 8
	ldr	w0, [x3, 648]
	sub	w0, w6, w0
	sxtw	x0, w0
	eor	x2, x0, x0, asr 63
	sub	x2, x2, x0, asr 63
	cmp	w2, w5
	bge	.L1057
	ldr	w7, [x3, 164]
	mov	w5, w2
.L1057:
	cmp	x1, x4
	bne	.L1058
.L1056:
	add	x20, x19, 20480
	mov	w21, -1
	ldrb	w0, [x20, 1584]
	str	w21, [x20, 3212]
	cmp	w0, 2
	str	w7, [x20, 3216]
	beq	.L1075
	mov	x0, x19
	bl	InitListX
	cbnz	w0, .L1076
	ldr	x1, .LC103
	mov	w0, 13
	bl	dprint_vfmw
	ldr	x22, .LC104
	ldr	w0, [x22]
	tbnz	x0, 13, .L1077
.L1062:
	mov	x0, x19
	bl	ReorderListX
	mov	x0, x19
	bl	RepairList
	cbnz	w0, .L1066
	mov	x0, x19
	bl	FindMinRefIdx
	ldr	x0, [x19, 28704]
	cbz	x0, .L1067
	ldr	x0, [x0, 32]
	ldr	w0, [x0, 164]
.L1063:
	ldr	x1, .LC106
	mov	w21, 0
	str	w0, [x20, 3212]
	mov	w0, 13
	bl	dprint_vfmw
	ldr	w0, [x22]
	tbz	x0, 13, .L1060
	mov	x0, x19
	bl	DumpList.part.5
.L1060:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1075:
	str	wzr, [x20, 1628]
	mov	w21, 0
	str	wzr, [x20, 1632]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1077:
	mov	x0, x19
	bl	DumpList.part.5
	b	.L1062
	.p2align 3
.L1067:
	mov	w0, -1
	b	.L1063
	.p2align 3
.L1066:
	mov	w21, -1
	b	.L1060
.L1065:
	mov	w7, 0
	b	.L1056
.L1076:
	ldr	x1, .LC101
	mov	w0, 13
	bl	dprint_vfmw
	b	.L1060
	.size	DecList, .-DecList
	.align	3
.LC101:
	.xword	.LC100
	.align	3
.LC103:
	.xword	.LC102
	.align	3
.LC104:
	.xword	g_PrintEnable
	.align	3
.LC106:
	.xword	.LC105
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	NoPicOut
	.type	NoPicOut, %function
NoPicOut:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 61440
	ldr	w0, [x1, 2772]
	cbz	w0, .L1085
	sub	w2, w0, #1
	mov	w4, 808
	mov	x3, 30044
	add	x0, x19, 28672
	add	x0, x0, 564
	umaddl	x3, w2, w4, x3
	add	x3, x19, x3
	b	.L1084
	.p2align 3
.L1092:
	ldrb	w2, [x0]
	cmp	w2, 1
	beq	.L1082
	add	x0, x0, 808
	cmp	x0, x3
	beq	.L1085
.L1084:
	ldr	w2, [x0, 648]
	cmp	w2, 1
	bne	.L1092
.L1082:
	strb	wzr, [x0, -3]
	strb	wzr, [x0, -2]
	add	x0, x0, 808
	cmp	x0, x3
	bne	.L1084
.L1085:
	ldr	w0, [x1, 2776]
	cbz	w0, .L1078
	sub	w0, w0, #1
	add	x1, x1, 2704
	add	x0, x0, 12288
	add	x0, x0, 3749
	add	x0, x19, x0, lsl 2
	.p2align 2
.L1086:
	str	wzr, [x1], 4
	cmp	x1, x0
	bne	.L1086
.L1078:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	NoPicOut, .-NoPicOut
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetBackPicFromVOQueue
	.type	GetBackPicFromVOQueue, %function
GetBackPicFromVOQueue:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x25, x0
	mov	x0, x30
	add	x20, x25, 65536
	bl	_mcount
	ldr	w0, [x20, 116]
	bl	FSP_GetFspType
	cbz	w0, .L1110
.L1094:
	add	x24, x25, 61440
	mov	w23, 0
	mov	w21, 808
	ldr	w0, [x24, 2772]
	cbz	w0, .L1101
	.p2align 2
.L1105:
	uxtw	x0, w23
	lsl	x1, x0, 2
	sub	x1, x1, x0
	umull	x22, w23, w21
	add	x1, x1, x1, lsl 4
	lsl	x1, x1, 1
	add	x22, x22, 28672
	sub	x0, x1, x0
	add	x19, x22, 1128
	add	x22, x22, 1160
	add	x19, x25, x19
	add	x0, x25, x0, lsl 3
	add	x22, x25, x22
	add	x0, x0, 28672
	strb	wzr, [x0, 561]
	strb	wzr, [x0, 562]
.L1099:
	ldr	x1, [x19]
	cbnz	x1, .L1111
.L1098:
	add	x19, x19, 8
	cmp	x19, x22
	bne	.L1099
	ldr	w0, [x24, 2772]
	add	w23, w23, 1
	cmp	w0, w23
	bhi	.L1105
.L1101:
	ldr	w0, [x24, 2776]
	cbz	w0, .L1093
	sub	w1, w0, #1
	add	x0, x24, 2704
	add	x1, x1, 12288
	add	x1, x1, 3749
	add	x25, x25, x1, lsl 2
	.p2align 2
.L1102:
	str	wzr, [x0], 4
	cmp	x0, x25
	bne	.L1102
.L1093:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1111:
	ldr	w0, [x20, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L1098
.L1110:
	add	x0, x25, 8
	bl	ResetVoQueue
	b	.L1094
	.size	GetBackPicFromVOQueue, .-GetBackPicFromVOQueue
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	wait_vo
	.type	wait_vo, %function
wait_vo:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	wait_vo, .-wait_vo
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetReRangeFlag
	.type	GetReRangeFlag, %function
GetReRangeFlag:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	x20, x1
	mov	w19, 4000
	bl	_mcount
	add	x1, x21, 20480
	mov	w0, 2208
	ldr	x5, [x21, 14968]
	ldr	x2, [x21, 14960]
	ldr	w1, [x1, 1596]
	ldr	w4, [x21, 14932]
	ldr	w3, [x21, 14936]
	umull	x0, w1, w0
	mov	w1, 2
	add	x0, x5, x0
	ldr	w0, [x0, 28]
	umull	x0, w0, w19
	add	x19, x2, x0
	ldrb	w0, [x19, 18]
	ldr	w5, [x19, 3252]
	sub	w0, w1, w0
	ldr	w2, [x19, 3248]
	madd	w0, w5, w0, w0
	add	w1, w2, 1
	cmp	w4, w1
	ccmp	w3, w0, 0, eq
	bne	.L1114
	add	x6, x21, 24576
	ldr	w5, [x19, 3272]
	mov	w2, 0
	ldr	w6, [x6, 3712]
	add	w6, w6, 1
	cmp	w5, w6
	bls	.L1115
.L1114:
	ldr	x2, .LC107
	ldr	x6, [x2]
	cbz	x6, .L1118
	add	x5, x21, 65536
	ubfiz	w2, w1, 4, 12
	ubfiz	w3, w3, 4, 12
	ubfiz	w1, w0, 4, 12
	ubfiz	w4, w4, 4, 12
	strh	w3, [x29, 58]
	ldr	w0, [x5, 116]
	mov	w3, 8
	strh	w2, [x29, 60]
	add	x2, x29, 56
	strh	w1, [x29, 62]
	mov	w1, 2
	strh	w4, [x29, 56]
	blr	x6
.L1118:
	ldr	w5, [x19, 3272]
	mov	w2, 1
.L1115:
	add	x1, x21, 16384
	ldr	w3, [x19, 36]
	ldr	w4, [x21, 15012]
	mov	w0, w2
	ldr	w1, [x1, 1864]
	cmp	w4, w3
	ccmp	w1, w5, 0, eq
	cset	w1, ne
	orr	w1, w2, w1
	str	w1, [x20]
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.size	GetReRangeFlag, .-GetReRangeFlag
	.align	3
.LC107:
	.xword	g_event_report
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecPOC
	.type	DecPOC, %function
DecPOC:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 20480
	mov	w2, 2208
	ldr	x6, [x19, 14968]
	mov	w1, 4000
	ldr	x5, [x19, 14960]
	mov	w4, 1
	ldr	w3, [x0, 1596]
	ldrb	w7, [x0, 1587]
	umull	x2, w3, w2
	add	x2, x6, x2
	ldr	w2, [x2, 28]
	umull	x1, w2, w1
	add	x1, x5, x1
	ldr	w3, [x1, 2200]
	ldr	w2, [x1, 2196]
	cmp	w3, w4
	add	w2, w2, 4
	lsl	w2, w4, w2
	beq	.L1121
	cbz	w3, .L1122
	cmp	w3, 2
	bne	.L1119
	cmp	w7, 5
	beq	.L1176
	ldr	w1, [x0, 728]
	cbz	w1, .L1155
	ldr	w1, [x0, 768]
	mov	w3, 0
	mov	w2, 0
	str	wzr, [x0, 764]
	str	wzr, [x0, 776]
.L1156:
	str	w2, [x0, 772]
.L1158:
	ldr	w4, [x0, 1616]
	add	w2, w1, w2
	str	w2, [x0, 760]
	lsl	w1, w2, 1
	cbnz	w4, .L1159
	sub	w1, w1, #1
	str	w1, [x0, 748]
.L1160:
	ldrb	w2, [x0, 1585]
	cbnz	w2, .L1161
	str	w1, [x0, 736]
	str	w1, [x0, 740]
	str	w1, [x0, 744]
.L1154:
	ldr	w1, [x0, 768]
	str	w1, [x0, 764]
	str	w3, [x0, 776]
.L1119:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1122:
	ldr	w1, [x1, 2204]
	cmp	w7, 5
	add	w1, w1, 4
	lsl	w4, w4, w1
	beq	.L1128
	ldr	w1, [x0, 728]
	cbnz	w1, .L1126
	ldr	w5, [x0, 712]
	ldr	w3, [x0, 716]
	mov	w2, w5
.L1127:
	ldr	w1, [x0, 704]
	cmp	w1, w3
	bcs	.L1125
	sub	w6, w3, w1
	cmp	w6, w4, lsr 1
	bcc	.L1125
	add	w2, w2, w4
	str	w2, [x0, 720]
.L1129:
	ldrb	w3, [x0, 1585]
	cbnz	w3, .L1131
.L1179:
	ldr	w3, [x0, 708]
	add	w1, w1, w2
	str	w1, [x0, 736]
	add	w2, w1, w3
	str	w2, [x0, 740]
	cmp	w1, w2
	csel	w1, w1, w2, le
	str	w1, [x0, 748]
.L1132:
	ldr	w3, [x0, 764]
	ldr	w2, [x0, 768]
	str	w1, [x0, 744]
	cmp	w2, w3
	beq	.L1134
	str	w2, [x0, 764]
.L1134:
	ldr	w1, [x0, 1616]
	cbz	w1, .L1119
	ldr	w2, [x0, 704]
	ldr	w1, [x0, 720]
	str	w1, [x0, 712]
	str	w2, [x0, 716]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1121:
	cmp	w7, 5
	beq	.L1177
	ldr	w3, [x0, 728]
	cbz	w3, .L1137
	mov	w2, 0
	str	wzr, [x0, 764]
	str	wzr, [x0, 776]
.L1138:
	str	w2, [x0, 772]
.L1136:
	ldr	w2, [x1, 2216]
	cbz	w2, .L1140
	ldr	w2, [x0, 768]
	ldr	w5, [x0, 772]
	ldr	w6, [x0, 1616]
	add	w5, w5, w2
	str	w5, [x0, 760]
	cbnz	w6, .L1142
	cbz	w5, .L1143
	sub	w5, w5, #1
	str	w5, [x0, 760]
.L1142:
	str	wzr, [x0, 792]
	ldr	w2, [x1, 2216]
	cmp	w2, wzr
	ble	.L1144
.L1163:
	mov	w4, 0
	mov	w3, 0
	.p2align 2
.L1145:
	add	x2, x1, x3, sxtw 2
	add	w3, w3, 1
	ldr	w2, [x2, 2220]
	add	w4, w4, w2
	str	w4, [x0, 792]
	ldr	w2, [x1, 2216]
	cmp	w2, w3
	bgt	.L1145
.L1144:
	cbz	w5, .L1146
	sub	w5, w5, #1
	ldr	w7, [x0, 792]
	udiv	w2, w5, w2
	str	w2, [x0, 784]
	ldr	w4, [x1, 2216]
	udiv	w3, w5, w4
	mul	w2, w2, w7
	str	w2, [x0, 788]
	msub	w5, w3, w4, w5
	str	w5, [x0, 780]
	tbnz	w5, #31, .L1147
	mov	w3, 0
	.p2align 2
.L1148:
	add	x4, x1, x3, sxtw 2
	add	w3, w3, 1
	cmp	w5, w3
	ldr	w4, [x4, 2220]
	add	w2, w2, w4
	str	w2, [x0, 788]
	bge	.L1148
.L1147:
	cbnz	w6, .L1149
	ldr	w3, [x1, 2208]
	add	w2, w2, w3
	str	w2, [x0, 788]
.L1149:
	ldrb	w3, [x0, 1585]
	cbnz	w3, .L1150
	ldr	w3, [x0, 752]
	ldr	w4, [x0, 756]
	add	w2, w2, w3
	str	w2, [x0, 736]
	ldr	w1, [x1, 2212]
	add	w1, w2, w1
	add	w1, w1, w4
	str	w1, [x0, 740]
	cmp	w2, w1
	csel	w2, w2, w1, le
	str	w2, [x0, 748]
.L1151:
	ldr	w3, [x0, 768]
	ldr	w1, [x0, 772]
	str	w2, [x0, 744]
	str	w3, [x0, 764]
	str	w1, [x0, 776]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1126:
	ldr	w5, [x0, 732]
	cbz	w5, .L1178
.L1128:
	ldr	w1, [x0, 704]
	mov	w2, 0
	mov	w5, 0
	str	wzr, [x0, 712]
	str	wzr, [x0, 716]
.L1125:
	cmp	w1, w3
	bls	.L1130
	sub	w3, w1, w3
	cmp	w3, w4, lsr 1
	bls	.L1130
	sub	w2, w2, w4
	str	w2, [x0, 720]
	b	.L1129
	.p2align 3
.L1176:
	add	x1, x0, 512
	mov	w3, 0
	stp	wzr, wzr, [x1, 224]
	stp	wzr, wzr, [x1, 232]
	str	wzr, [x0, 772]
	b	.L1154
	.p2align 3
.L1177:
	str	wzr, [x0, 772]
	b	.L1136
	.p2align 3
.L1130:
	ldrb	w3, [x0, 1585]
	str	w5, [x0, 720]
	cbz	w3, .L1179
.L1131:
	ldrb	w3, [x0, 1586]
	add	w1, w1, w2
	cbnz	w3, .L1133
	str	w1, [x0, 736]
	str	w1, [x0, 748]
	b	.L1132
	.p2align 3
.L1140:
	ldr	w6, [x0, 1616]
	str	wzr, [x0, 760]
.L1143:
	str	wzr, [x0, 792]
	mov	w5, 0
	ldr	w2, [x1, 2216]
	cmp	w2, wzr
	bgt	.L1163
	.p2align 2
.L1146:
	mov	w2, 0
	str	wzr, [x0, 788]
	b	.L1147
	.p2align 3
.L1150:
	ldrb	w3, [x0, 1586]
	cbnz	w3, .L1152
	ldr	w1, [x0, 752]
	add	w2, w2, w1
	str	w2, [x0, 736]
	str	w2, [x0, 748]
	b	.L1151
	.p2align 3
.L1155:
	ldr	w3, [x0, 764]
	ldr	w1, [x0, 768]
	cmp	w1, w3
	ldr	w3, [x0, 776]
	bcs	.L1157
	add	w2, w2, w3
	str	w2, [x0, 772]
	mov	w3, w2
	b	.L1158
	.p2align 3
.L1137:
	ldr	w3, [x0, 764]
	ldr	w4, [x0, 768]
	cmp	w4, w3
	bcs	.L1139
	ldr	w3, [x0, 776]
	add	w2, w3, w2
	str	w2, [x0, 772]
	b	.L1136
	.p2align 3
.L1161:
	ldrb	w2, [x0, 1586]
	str	w1, [x0, 744]
	cbnz	w2, .L1162
	str	w1, [x0, 736]
	b	.L1154
	.p2align 3
.L1159:
	str	w1, [x0, 748]
	b	.L1160
	.p2align 3
.L1133:
	str	w1, [x0, 740]
	str	w1, [x0, 748]
	b	.L1132
	.p2align 3
.L1152:
	ldr	w3, [x1, 2212]
	ldr	w1, [x0, 752]
	add	w2, w2, w3
	add	w2, w2, w1
	str	w2, [x0, 740]
	str	w2, [x0, 748]
	b	.L1151
	.p2align 3
.L1178:
	ldr	w3, [x0, 736]
	mov	w2, 0
	str	wzr, [x0, 712]
	str	w3, [x0, 716]
	b	.L1127
	.p2align 3
.L1162:
	str	w1, [x0, 740]
	b	.L1154
.L1139:
	ldr	w2, [x0, 776]
	b	.L1138
.L1157:
	mov	w2, w3
	b	.L1156
	.size	DecPOC, .-DecPOC
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CalcPicNum
	.type	CalcPicNum, %function
CalcPicNum:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x2, x19, 20480
	mov	w1, 2208
	ldr	x4, [x19, 14968]
	mov	w0, 4000
	ldr	x3, [x19, 14960]
	add	x6, x19, 61440
	ldr	w2, [x2, 1596]
	mov	w7, 1
	umull	x1, w2, w1
	ldrb	w2, [x6, 115]
	add	x1, x4, x1
	ldr	w1, [x1, 28]
	umull	x0, w1, w0
	add	x0, x3, x0
	ldr	w0, [x0, 2196]
	add	w0, w0, 4
	lsl	w7, w7, w0
	add	x0, x19, 24576
	cbz	w2, .L1181
	cmp	w2, 1
	mov	w1, 0
	cset	w8, eq
	cmp	w2, 2
	ldr	w2, [x0, 3720]
	cset	w9, eq
	cbz	w2, .L1201
	.p2align 2
.L1218:
	add	x3, x19, x1, uxtw 3
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	cbz	w4, .L1195
	ldr	w4, [x2, 636]
	ldr	w5, [x6, 768]
	cmp	w4, w5
	bls	.L1196
	sub	w4, w4, w7
	str	w4, [x2, 640]
.L1197:
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	tbz	x4, 0, .L1198
	ldr	w5, [x2, 720]
	and	w5, w5, 16776960
	cmp	w5, 65536
	beq	.L1228
.L1198:
	tbz	x4, 1, .L1195
	ldr	w3, [x2, 760]
	and	w3, w3, 16776960
	cmp	w3, 65536
	beq	.L1229
	.p2align 2
.L1195:
	ldr	w2, [x0, 3720]
	add	w1, w1, 1
	cmp	w2, w1
	bhi	.L1218
.L1201:
	ldr	w2, [x0, 3724]
	mov	w1, 0
	cbz	w2, .L1180
	.p2align 2
.L1217:
	add	x4, x19, x1, uxtw 3
	add	w1, w1, 1
	ldr	x3, [x4, 28160]
	ldrb	w2, [x3, 2]
	cbz	w2, .L1203
	tbz	x2, 0, .L1204
	ldr	w5, [x3, 720]
	and	w5, w5, 16776960
	cmp	w5, 256
	beq	.L1230
.L1204:
	tbz	x2, 1, .L1203
	ldr	w2, [x3, 760]
	and	w2, w2, 16776960
	cmp	w2, 256
	beq	.L1231
	.p2align 2
.L1203:
	ldr	w2, [x0, 3724]
	cmp	w2, w1
	bhi	.L1217
.L1180:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1196:
	str	w4, [x2, 640]
	b	.L1197
	.p2align 3
.L1181:
	ldr	w2, [x0, 3720]
	mov	w1, 0
	cbnz	w2, .L1216
	.p2align 2
.L1191:
	ldr	w2, [x0, 3724]
	mov	w1, 0
	cbz	w2, .L1180
	add	x2, x19, x1, uxtw 3
	add	w1, w1, 1
	ldr	x2, [x2, 28160]
	ldrb	w3, [x2, 2]
	cmp	w3, 3
	beq	.L1232
	.p2align 2
.L1192:
	ldr	w2, [x0, 3724]
	cmp	w2, w1
	bls	.L1180
	add	x2, x19, x1, uxtw 3
	add	w1, w1, 1
	ldr	x2, [x2, 28160]
	ldrb	w3, [x2, 2]
	cmp	w3, 3
	bne	.L1192
.L1232:
	ldr	w3, [x2, 680]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L1192
	ldr	w3, [x2, 644]
	str	w3, [x2, 684]
	b	.L1192
	.p2align 3
.L1234:
	ldr	w4, [x2, 680]
	and	w4, w4, 16776960
	cmp	w4, 65536
	bne	.L1187
	ldr	w4, [x2, 636]
	ldr	w5, [x6, 768]
	cmp	w4, w5
	sub	w5, w4, w7
	bhi	.L1233
	str	w4, [x2, 640]
.L1189:
	ldr	x2, [x3, 28032]
	ldr	w3, [x2, 640]
	str	w3, [x2, 688]
	.p2align 2
.L1187:
	ldr	w2, [x0, 3720]
	add	w1, w1, 1
	cmp	w2, w1
	bls	.L1191
.L1216:
	add	x3, x19, x1, uxtw 3
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	cmp	w4, 3
	bne	.L1187
	b	.L1234
	.p2align 3
.L1230:
	ldr	w2, [x3, 644]
	add	w2, w8, w2, lsl 1
	str	w2, [x3, 724]
	ldr	x3, [x4, 28160]
	ldrb	w2, [x3, 2]
	b	.L1204
	.p2align 3
.L1228:
	ldr	w4, [x2, 640]
	add	w4, w8, w4, lsl 1
	str	w4, [x2, 728]
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	b	.L1198
	.p2align 3
.L1231:
	ldr	w2, [x3, 644]
	add	w2, w9, w2, lsl 1
	str	w2, [x3, 764]
	b	.L1203
	.p2align 3
.L1229:
	ldr	w3, [x2, 640]
	add	w3, w9, w3, lsl 1
	str	w3, [x2, 768]
	b	.L1195
	.p2align 3
.L1233:
	str	w5, [x2, 640]
	b	.L1189
	.size	CalcPicNum, .-CalcPicNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsOutDPB
	.type	IsOutDPB, %function
IsOutDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x20, x1
	mov	x0, x30
	bl	_mcount
	cbz	x20, .L1240
	add	x0, x19, 24576
	ldr	w3, [x0, 3716]
	cbz	w3, .L1240
	ldr	x0, [x19, 27904]
	cmp	x20, x0
	beq	.L1242
	sub	w3, w3, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L1237
	.p2align 3
.L1238:
	ldr	x2, [x0, 27904]
	cmp	x20, x2
	beq	.L1242
.L1237:
	cmp	x0, x3
	add	x0, x0, 8
	bne	.L1238
.L1240:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1242:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	IsOutDPB, .-IsOutDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	WriteCurrPicYUV
	.type	WriteCurrPicYUV, %function
WriteCurrPicYUV:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 61440
	bl	_mcount
	ldr	x1, .LC109
	mov	w0, 2
	ldr	x3, [x19, 752]
	ldrb	w20, [x19, 115]
	ldr	w3, [x3, 164]
	mov	w2, w20
	bl	dprint_vfmw
	cmp	w20, 3
	ldr	w3, [x19, 808]
	ccmp	w20, wzr, 4, ne
	ldr	w4, [x19, 812]
	cset	w0, eq
	ldr	x1, .LC111
	cmp	w0, wzr
	mov	w2, 1
	csinc	w2, w2, w20, ne
	mov	w0, 22
	bl	dprint_vfmw
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	WriteCurrPicYUV, .-WriteCurrPicYUV
	.align	3
.LC109:
	.xword	.LC108
	.align	3
.LC111:
	.xword	.LC110
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	WritePicMsg
	.type	WritePicMsg, %function
WritePicMsg:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	add	x19, x21, 61440
	add	x20, x21, 65536
	bl	_mcount
	add	x23, x21, 12288
	ldr	x1, .LC113
	mov	w0, 2
	ldrb	w3, [x19, 115]
	add	x25, x21, 16384
	ldr	w4, [x19, 284]
	ldr	w2, [x19, 776]
	ldr	w5, [x19, 828]
	bl	dprint_vfmw
	str	wzr, [x20, 40]
	ldr	x0, [x19, 728]
	str	x0, [x19, 2848]
	ldr	w0, [x19, 272]
	ldr	x4, [x20, 32]
	ldr	w5, [x19, 268]
	str	w0, [x19, 3632]
	ldr	w3, [x19, 276]
	ldr	w2, [x19, 776]
	ldrb	w0, [x19, 115]
	str	w5, [x19, 3628]
	str	w3, [x19, 3636]
	str	x4, [x19, 3848]
	strb	w0, [x19, 2840]
	str	w2, [x19, 2856]
	ldr	w3, [x19, 812]
	ldrb	w2, [x23, 2707]
	ldr	w0, [x19, 816]
	str	w3, [x19, 2860]
	str	w0, [x19, 2864]
	add	x0, x21, 20480
	str	w2, [x19, 2872]
	ldr	w2, [x23, 5936]
	add	w2, w2, 1
	str	w2, [x19, 2868]
	ldrb	w2, [x23, 2706]
	strb	w2, [x19, 2843]
	ldr	w2, [x25, 2652]
	str	w2, [x19, 2876]
	ldrb	w2, [x25, 2592]
	str	w2, [x19, 2880]
	ldr	w1, [x23, 2728]
	strb	w1, [x19, 2841]
	ldrb	w1, [x25, 2596]
	str	w1, [x19, 2884]
	ldr	w1, [x0, 748]
	str	w1, [x19, 2888]
	ldr	x1, [x19, 752]
	ldr	w2, [x0, 736]
	str	w2, [x19, 2892]
	ldr	w0, [x0, 740]
	str	w0, [x19, 2896]
	ldr	w1, [x1, 632]
	ldr	w0, [x20, 116]
	bl	FSP_GetLogicFs
	mov	x24, x0
	cbz	x0, .L1280
	ldr	x2, [x0, 640]
	ldr	x3, [x0, 648]
	cbz	x2, .L1250
	cbz	x3, .L1250
	ldr	w0, [x20, 116]
	add	x3, x19, 3276
	add	x2, x19, 3148
	add	x1, x19, 3536
	add	x22, x21, 24576
	bl	FSP_GetDecFsAddrTab
	ldr	x0, [x24, 640]
	add	x2, x19, 3540
	add	x1, x19, 3620
	ldr	w0, [x0, 8]
	str	w0, [x19, 3132]
	ldr	w0, [x20, 116]
	bl	FSP_GetPmvAddrTab
	ldr	w0, [x19, 828]
	str	w0, [x19, 3140]
	ldr	w0, [x24, 668]
	str	w0, [x19, 3624]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 16]
	str	w0, [x19, 3136]
	ldr	x0, [x24, 648]
	ldr	w0, [x0, 76]
	str	w0, [x19, 3144]
	ldr	w4, [x22, 3992]
	str	w4, [x19, 3840]
	cbz	w4, .L1256
	sub	w3, w4, #1
	add	x0, x22, 3800
	add	x3, x3, 4096
	add	x1, x19, 3648
	add	x3, x3, 2999
	add	x3, x21, x3, lsl 2
	.p2align 2
.L1255:
	ldr	w2, [x0]
	add	x0, x0, 4
	str	w2, [x1]
	add	x1, x1, 4
	cmp	x3, x0
	ldr	w2, [x0, 60]
	str	w2, [x1, 60]
	ldr	w2, [x0, 124]
	str	w2, [x1, 124]
	bne	.L1255
.L1256:
	ldrb	w0, [x23, 2713]
	cbnz	w0, .L1281
	ldrb	w0, [x25, 2610]
	cbnz	w0, .L1257
	ldr	x8, .LC118
	add	x5, x19, 2900
	mov	w1, 0
	.p2align 2
.L1258:
	and	w0, w1, 3
	add	w1, w1, 1
	cmp	w1, 24
	ldr	w0, [x8, w0, uxtw 2]
	ubfx	x3, x0, 8, 8
	lsr	w2, w0, 16
	ubfiz	w2, w2, 8, 8
	and	w0, w0, -16776961
	orr	w2, w2, w3, lsl 16
	orr	w0, w2, w0
	str	w0, [x5], 4
	bne	.L1258
	add	x19, x19, 2996
	mov	w5, 0
	.p2align 2
.L1259:
	add	w0, w5, 1
	and	w2, w5, 14
	add	x1, x8, 16
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
	ubfiz	w9, w3, 8, 8
	and	w0, w0, -16777216
	and	w3, w1, 255
	ubfx	x1, x1, 8, 8
	orr	w0, w0, w2, lsl 16
	orr	w7, w10, w7, lsl 24
	orr	w3, w9, w3
	orr	w1, w1, w6, lsl 8
	orr	w2, w7, w3
	orr	w0, w1, w0
	stp	w2, w0, [x19, -8]
	bne	.L1259
.L1265:
	mov	w23, 0
	cbz	w4, .L1261
	.p2align 2
.L1269:
	add	x19, x21, x23, uxtw 2
	ldr	w0, [x20, 116]
	add	x24, x19, 24576
	mov	w1, 0
	add	x19, x19, 61440
	add	w23, w23, 1
	ldr	w2, [x24, 3800]
	bl	FSP_GetStoreType
	cmp	w0, wzr
	cset	w0, ne
	str	w0, [x19, 3984]
	ldr	w0, [x24, 3800]
	str	w0, [x19, 4048]
	ldr	w0, [x22, 3992]
	cmp	w0, w23
	bhi	.L1269
.L1261:
	mov	w0, 0
.L1248:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1281:
	ldrb	w0, [x25, 2610]
.L1257:
	cmp	w0, 1
	add	x5, x19, 2900
	mov	w1, 0
	beq	.L1263
	.p2align 2
.L1266:
	uxtw	x0, w1
	add	w1, w1, 1
	add	x0, x0, 4096
	cmp	w1, 24
	add	x0, x21, x0, lsl 2
	ldr	w0, [x0, 564]
	ubfx	x3, x0, 8, 8
	lsr	w2, w0, 16
	ubfiz	w2, w2, 8, 8
	and	w0, w0, -16776961
	orr	w2, w2, w3, lsl 16
	orr	w0, w2, w0
	str	w0, [x5], 4
	bne	.L1266
	add	x19, x19, 2996
	mov	w9, 1
	mov	w5, 0
	.p2align 2
.L1267:
	ubfx	x1, x5, 3, 29
	ubfiz	x0, x5, 1, 3
	and	x2, x9, 15
	lsl	x1, x1, 4
	add	x2, x1, x2
	add	x1, x1, x0
	add	x1, x1, 4096
	add	x0, x2, 4096
	add	x19, x19, 8
	add	w5, w5, 1
	add	x0, x21, x0, lsl 2
	add	x1, x21, x1, lsl 2
	cmp	w5, 16
	add	w9, w9, 2
	ldr	w0, [x0, 660]
	ldr	w1, [x1, 660]
	lsr	w3, w0, 16
	ubfx	x7, x0, 8, 8
	lsr	w8, w1, 16
	lsr	w6, w1, 24
	ubfiz	w11, w0, 16, 8
	ubfiz	w10, w8, 8, 8
	ubfx	x2, x1, 8, 8
	and	w8, w1, 255
	lsl	w6, w6, 8
	and	w1, w0, -16777216
	orr	w0, w2, w1
	orr	w3, w11, w3, lsl 24
	orr	w8, w10, w8
	orr	w6, w6, w7, lsl 16
	orr	w1, w8, w3
	orr	w0, w6, w0
	stp	w1, w0, [x19, -8]
	bne	.L1267
	b	.L1265
	.p2align 3
.L1263:
	uxtw	x0, w1
	add	w1, w1, 1
	add	x0, x0, 4096
	cmp	w1, 24
	add	x0, x0, 1144
	ldr	w0, [x21, x0, lsl 2]
	ubfx	x3, x0, 8, 8
	lsr	w2, w0, 16
	ubfiz	w2, w2, 8, 8
	and	w0, w0, -16776961
	orr	w2, w2, w3, lsl 16
	orr	w0, w2, w0
	str	w0, [x5], 4
	bne	.L1263
	add	x19, x19, 2996
	mov	w9, 1
	mov	w5, 0
	.p2align 2
.L1264:
	ubfx	x1, x5, 3, 29
	ubfiz	x0, x5, 1, 3
	and	x2, x9, 15
	lsl	x1, x1, 4
	add	x2, x1, x2
	add	x1, x1, x0
	add	x1, x1, 4096
	add	x0, x2, 4096
	add	x0, x0, 1168
	add	x1, x1, 1168
	add	x19, x19, 8
	add	w5, w5, 1
	cmp	w5, 16
	add	w9, w9, 2
	ldr	w0, [x21, x0, lsl 2]
	ldr	w1, [x21, x1, lsl 2]
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
	bne	.L1264
	b	.L1265
.L1250:
	ldr	x1, .LC117
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1248
.L1280:
	ldr	x2, [x19, 752]
	mov	w0, 0
	ldr	x1, .LC115
	ldr	w2, [x2, 632]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1248
	.size	WritePicMsg, .-WritePicMsg
	.align	3
.LC113:
	.xword	.LC112
	.align	3
.LC115:
	.xword	.LC114
	.align	3
.LC117:
	.xword	.LC116
	.align	3
.LC118:
	.xword	.LANCHOR0
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	UpdatePicQpInf
	.type	UpdatePicQpInf, %function
UpdatePicQpInf:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	w20, w0
	mov	x19, x1
	mov	x0, x30
	bl	_mcount
	ldp	w1, w0, [x19, 24]
	cmp	w1, w20
	csel	w1, w1, w20, ge
	cmp	w0, w20
	csel	w0, w0, w20, le
	stp	w1, w0, [x19, 24]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	UpdatePicQpInf, .-UpdatePicQpInf
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	WriteSliceMsg
	.type	WriteSliceMsg, %function
WriteSliceMsg:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x22, x0
	mov	x0, x30
	add	x21, x22, 65536
	bl	_mcount
	ldr	w5, [x21, 40]
	ldr	x6, [x21, 32]
	add	w0, w5, 1
	str	w0, [x21, 40]
	uxtw	x20, w5
	ldr	w1, [x22, 14940]
	lsl	x4, x20, 7
	sub	x4, x4, x20
	sub	w1, w1, #1
	lsl	x4, x4, 5
	cmp	w5, w1
	add	x19, x6, x4
	bcs	.L1284
	lsl	x1, x0, 7
	sub	x0, x1, x0
	add	x0, x6, x0, lsl 5
	str	x0, [x19, 4056]
.L1285:
	ldr	x3, [x22, 632]
	ldrb	w2, [x3]
	ldr	w0, [x3, 76]
	cmp	w2, 1
	ccmp	w2, w0, 2, ls
	bcs	.L1286
	.p2align 2
.L1287:
	uxtw	x0, w2
	add	w2, w2, 1
	lsl	x1, x0, 5
	add	x3, x3, x1
	add	x0, x19, x0, lsl 2
	ldr	w3, [x3, 28]
	str	w3, [x0, 16]
	ldr	x3, [x22, 632]
	add	x3, x3, x1
	ldr	w3, [x3, 24]
	str	w3, [x0, 8]
	ldr	x3, [x22, 632]
	add	x1, x3, x1
	ldr	w1, [x1, 32]
	str	w1, [x0, 24]
	ldr	x3, [x22, 632]
	ldr	w0, [x3, 76]
	cmp	w0, w2
	ccmp	w2, 1, 2, hi
	bls	.L1287
.L1286:
	cmp	w0, 1
	bhi	.L1292
	.p2align 2
.L1363:
	add	x1, x19, x0, uxtw 2
	add	w0, w0, 1
	cmp	w0, 2
	str	wzr, [x1, 8]
	str	wzr, [x1, 16]
	str	wzr, [x1, 24]
	bne	.L1363
.L1292:
	add	x23, x22, 61440
	add	x24, x22, 16384
	add	x2, x24, 2592
	add	x26, x22, 20480
	add	x1, x22, 12288
	ldrb	w0, [x23, 121]
	strb	w0, [x19, 1]
	ldr	w0, [x2, 48]
	ldr	w7, [x26, 3196]
	add	w0, w0, 26
	add	w0, w0, w7
	str	w0, [x19, 32]
	ldr	w7, [x26, 3192]
	str	w7, [x19, 36]
	ldr	w7, [x26, 1644]
	str	w7, [x19, 40]
	ldr	w7, [x26, 1640]
	str	w7, [x19, 44]
	ldrb	w7, [x26, 1584]
	strb	w7, [x6, x4]
	ldr	w4, [x26, 1636]
	str	w4, [x19, 48]
	ldrb	w4, [x26, 1590]
	strb	w4, [x19, 2]
	ldrb	w1, [x1, 2708]
	strb	w1, [x19, 3]
	ldr	w1, [x26, 1628]
	str	w1, [x19, 52]
	ldr	w1, [x26, 1632]
	str	w1, [x19, 56]
	ldrb	w1, [x26, 1584]
	cmp	w1, 1
	beq	.L1388
	cbz	w1, .L1389
	strb	wzr, [x19, 4]
.L1293:
	ldr	w2, [x24, 2644]
	str	w2, [x19, 60]
	ldr	w1, [x24, 2648]
	str	w1, [x19, 64]
	ldr	w1, [x26, 3204]
	str	w1, [x19, 68]
	ldr	w1, [x26, 3208]
	str	w1, [x19, 72]
	ldr	w1, [x26, 3200]
	str	w1, [x19, 76]
	ldr	x1, [x23, 752]
	cbnz	w5, .L1295
	str	w0, [x1, 704]
	ldr	x0, [x23, 752]
	ldr	w1, [x19, 32]
	str	w1, [x0, 708]
.L1296:
	ldrb	w0, [x26, 1584]
	cmp	w0, 2
	beq	.L1334
	ldr	w0, [x26, 1628]
	cbz	w0, .L1337
	ldr	x1, [x22, 28704]
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1300
	sub	w4, w0, #1
	mov	x0, x22
	add	x4, x22, x4, lsl 3
	b	.L1302
	.p2align 3
.L1304:
	ldr	x1, [x0, 28712]
	add	x0, x0, 8
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1300
.L1302:
	cmp	x4, x0
	bne	.L1304
.L1337:
	str	wzr, [x29, 108]
.L1299:
	ldr	w0, [x26, 1632]
	cbz	w0, .L1338
	ldr	x1, [x22, 28968]
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1306
	sub	w4, w0, #1
	mov	x0, x22
	add	x4, x22, x4, lsl 3
	b	.L1308
	.p2align 3
.L1310:
	ldr	x1, [x0, 28976]
	add	x0, x0, 8
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1306
.L1308:
	cmp	x4, x0
	bne	.L1310
.L1338:
	str	wzr, [x29, 104]
.L1305:
	ldrb	w0, [x23, 115]
	cbz	w0, .L1311
	ldr	w0, [x26, 1628]
	mov	w25, 0
	cbnz	w0, .L1362
	b	.L1327
	.p2align 3
.L1391:
	cbz	x0, .L1344
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1323:
	lsl	x0, x27, 5
	add	x0, x0, x27, lsl 2
	add	x0, x19, x0
	str	w1, [x0, 1640]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 672]
	str	w1, [x0, 1644]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 648]
	str	w1, [x0, 1632]
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1]
	strb	w1, [x0, 1624]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1, 680]
	strb	w1, [x0, 1626]
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1]
	cmp	w1, 1
	beq	.L1390
	strb	w1, [x0, 1630]
	add	w25, w25, 1
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1631]
	ldr	x1, [x28, 28704]
	ldr	w1, [x1, 12]
	str	w1, [x0, 1656]
	ldr	w0, [x26, 1628]
	cmp	w0, w25
	bls	.L1327
.L1362:
	uxtw	x27, w25
	ldr	w0, [x21, 116]
	add	x28, x22, x27, lsl 3
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x4, [x28, 28704]
	lsl	x1, x27, 5
	add	x1, x1, x27, lsl 2
	add	x1, x19, x1
	ldr	x4, [x4, 32]
	ldr	w4, [x4, 668]
	str	w4, [x1, 1636]
	ldr	x4, [x28, 28704]
	ldr	x4, [x4, 32]
	ldrb	w4, [x4, 1]
	strb	w4, [x1, 1625]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1391
	ldr	w1, [x29, 108]
	b	.L1323
	.p2align 3
.L1284:
	str	xzr, [x19, 4056]
	b	.L1285
	.p2align 3
.L1389:
	ldrb	w1, [x2, 2]
	strb	w1, [x19, 4]
	b	.L1293
	.p2align 3
.L1390:
	strb	w1, [x0, 1628]
	add	w25, w25, 1
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1629]
	ldr	x1, [x28, 28704]
	ldr	w1, [x1, 12]
	str	w1, [x0, 1652]
	ldr	w0, [x26, 1628]
	cmp	w0, w25
	bhi	.L1362
.L1327:
	ldrb	w0, [x26, 1584]
	cmp	w0, 1
	beq	.L1392
.L1314:
	add	x1, x22, 24576
	add	x0, x19, 3928
	add	x1, x1, 4000
	add	x4, x19, 4056
	.p2align 2
.L1320:
	ldr	w2, [x1], 4
	str	w2, [x0], 4
	cmp	x4, x0
	bne	.L1320
	ldrb	w0, [x24, 2594]
	str	w0, [x23, 3124]
	ldr	w1, [x24, 2636]
	str	w1, [x23, 3128]
	cbz	w0, .L1332
	ldrb	w0, [x26, 1584]
	cbnz	w0, .L1332
.L1333:
	ldr	w0, [x26, 1648]
	mov	w2, 0
	str	w0, [x19, 80]
	ldr	w0, [x26, 1652]
	str	w0, [x19, 84]
	.p2align 2
.L1335:
	ubfiz	x0, x2, 2, 32
	add	w2, w2, 1
	add	x1, x22, x0
	add	x0, x19, x0
	add	x1, x1, 20480
	ldr	w4, [x1, 1656]
	str	w4, [x0, 88]
	ldr	w4, [x1, 1784]
	str	w4, [x0, 216]
	ldr	w4, [x1, 1912]
	str	w4, [x0, 344]
	ldr	w4, [x1, 2424]
	str	w4, [x0, 856]
	ldr	w4, [x1, 2552]
	str	w4, [x0, 984]
	ldr	w1, [x1, 2680]
	str	w1, [x0, 1112]
	ldr	w0, [x26, 1640]
	cmp	w0, w2
	bcs	.L1335
	ldrb	w0, [x26, 1584]
	cmp	w0, 1
	beq	.L1393
	.p2align 2
.L1334:
	add	x20, x20, 4096
	ldr	x0, [x22, 632]
	add	x20, x20, 3786
	str	x0, [x22, x20, lsl 3]
	str	xzr, [x22, 632]
.L1283:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1295:
	ldr	w2, [x1, 704]
	cmp	w2, w0
	csel	w2, w2, w0, ge
	str	w2, [x1, 704]
	ldr	w2, [x1, 708]
	cmp	w2, w0
	csel	w0, w2, w0, le
	str	w0, [x1, 708]
	b	.L1296
	.p2align 3
.L1344:
	mov	w1, 0
	b	.L1323
	.p2align 3
.L1300:
	ldr	w1, [x1, 632]
	ldr	w0, [x21, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L1394
	ldr	x0, [x0, 640]
	ldr	w0, [x0, 8]
	str	w0, [x29, 108]
	b	.L1299
	.p2align 3
.L1332:
	cmp	w1, 1
	bne	.L1334
	ldrb	w0, [x26, 1584]
	cmp	w0, 1
	bne	.L1334
	b	.L1333
	.p2align 3
.L1311:
	ldr	w0, [x26, 1628]
	mov	w25, 0
	cbnz	w0, .L1361
	b	.L1319
	.p2align 3
.L1395:
	cbz	x0, .L1340
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1317:
	lsl	x0, x27, 5
	add	w25, w25, 1
	add	x0, x0, x27, lsl 2
	add	x0, x19, x0
	str	w1, [x0, 1640]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 672]
	str	w1, [x0, 1644]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 648]
	str	w1, [x0, 1632]
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1627]
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1]
	strb	w1, [x0, 1626]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 692]
	str	w1, [x0, 1648]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 732]
	str	w1, [x0, 1652]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 772]
	str	w1, [x0, 1656]
	strb	wzr, [x0, 1624]
	ldr	w0, [x26, 1628]
	cmp	w0, w25
	bls	.L1319
.L1361:
	uxtw	x27, w25
	ldr	w0, [x21, 116]
	add	x28, x22, x27, lsl 3
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x4, [x28, 28704]
	lsl	x1, x27, 5
	add	x1, x1, x27, lsl 2
	add	x1, x19, x1
	ldr	x4, [x4, 32]
	ldr	w4, [x4, 668]
	str	w4, [x1, 1636]
	ldr	x4, [x28, 28704]
	ldr	x4, [x4, 32]
	ldrb	w4, [x4, 1]
	strb	w4, [x1, 1625]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1395
	ldr	w1, [x29, 108]
	b	.L1317
	.p2align 3
.L1340:
	mov	w1, 0
	b	.L1317
	.p2align 3
.L1319:
	ldrb	w0, [x26, 1584]
	cmp	w0, 1
	bne	.L1314
	ldr	w0, [x26, 1632]
	cbz	w0, .L1314
	mov	w28, 0
	b	.L1322
	.p2align 3
.L1396:
	cbz	x0, .L1342
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1321:
	lsl	x0, x25, 5
	add	w28, w28, 1
	add	x0, x0, x25, lsl 2
	add	x0, x19, x0
	str	w1, [x0, 1640]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 672]
	str	w1, [x0, 2796]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 648]
	str	w1, [x0, 2784]
	ldr	x1, [x27, 28968]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2779]
	ldr	x1, [x27, 28968]
	ldrb	w1, [x1]
	strb	w1, [x0, 2778]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 692]
	str	w1, [x0, 2800]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 732]
	str	w1, [x0, 2804]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 772]
	str	w1, [x0, 2808]
	strb	wzr, [x0, 2776]
	ldr	w0, [x26, 1632]
	cmp	w0, w28
	bls	.L1314
.L1322:
	uxtw	x25, w28
	ldr	w0, [x21, 116]
	add	x27, x22, x25, lsl 3
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w2, [x1, 668]
	lsl	x1, x25, 5
	add	x1, x1, x25, lsl 2
	add	x1, x19, x1
	str	w2, [x1, 2788]
	ldr	x2, [x27, 28968]
	ldr	x2, [x2, 32]
	ldrb	w2, [x2, 1]
	strb	w2, [x1, 2777]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1396
	ldr	w1, [x29, 104]
	b	.L1321
	.p2align 3
.L1306:
	ldr	w1, [x1, 632]
	ldr	w0, [x21, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L1397
	ldr	x0, [x0, 640]
	ldr	w0, [x0, 8]
	str	w0, [x29, 104]
	b	.L1305
	.p2align 3
.L1388:
	ldr	w1, [x2, 44]
	strb	w1, [x19, 4]
	b	.L1293
.L1393:
	mov	w2, 0
	.p2align 2
.L1336:
	ubfiz	x0, x2, 2, 32
	add	w2, w2, 1
	add	x1, x22, x0
	add	x0, x19, x0
	add	x1, x1, 20480
	ldr	w4, [x1, 2040]
	str	w4, [x0, 472]
	ldr	w4, [x1, 2168]
	str	w4, [x0, 600]
	ldr	w4, [x1, 2296]
	str	w4, [x0, 728]
	ldr	w4, [x1, 2808]
	str	w4, [x0, 1240]
	ldr	w4, [x1, 2936]
	str	w4, [x0, 1368]
	ldr	w1, [x1, 3064]
	str	w1, [x0, 1496]
	ldr	w0, [x26, 1644]
	cmp	w0, w2
	bcs	.L1336
	b	.L1334
.L1342:
	mov	w1, 0
	b	.L1321
.L1392:
	ldr	w0, [x26, 1632]
	cbz	w0, .L1314
	mov	w28, 0
	b	.L1331
	.p2align 3
.L1399:
	cbz	x0, .L1346
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1328:
	lsl	x0, x25, 5
	add	x0, x0, x25, lsl 2
	add	x0, x19, x0
	str	w1, [x0, 2792]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 672]
	str	w1, [x0, 2796]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 648]
	str	w1, [x0, 2784]
	ldr	x1, [x27, 28968]
	ldrb	w1, [x1]
	strb	w1, [x0, 2776]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1, 680]
	strb	w1, [x0, 2778]
	ldr	x1, [x27, 28968]
	ldrb	w1, [x1]
	cmp	w1, 1
	beq	.L1398
	strb	w1, [x0, 2782]
	ldr	x1, [x27, 28968]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2783]
	ldr	x1, [x27, 28968]
	ldr	w1, [x1, 12]
	str	w1, [x0, 2808]
.L1330:
	ldr	w0, [x26, 1632]
	add	w28, w28, 1
	cmp	w0, w28
	bls	.L1314
.L1331:
	uxtw	x25, w28
	ldr	w0, [x21, 116]
	add	x27, x22, x25, lsl 3
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldr	w2, [x1, 668]
	lsl	x1, x25, 5
	add	x1, x1, x25, lsl 2
	add	x1, x19, x1
	str	w2, [x1, 2788]
	ldr	x2, [x27, 28968]
	ldr	x2, [x2, 32]
	ldrb	w2, [x2, 1]
	strb	w2, [x1, 2777]
	ldr	x1, [x27, 28968]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1399
	ldr	w1, [x29, 104]
	b	.L1328
	.p2align 3
.L1398:
	strb	w1, [x0, 2780]
	ldr	x1, [x27, 28968]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2781]
	ldr	x1, [x27, 28968]
	ldr	w1, [x1, 12]
	str	w1, [x0, 2804]
	b	.L1330
.L1346:
	mov	w1, 0
	b	.L1328
.L1394:
	ldr	x1, .LC121
	mov	w2, 7517
	bl	dprint_vfmw
	b	.L1283
.L1397:
	ldr	x1, .LC121
	mov	w2, 7538
	bl	dprint_vfmw
	b	.L1283
	.size	WriteSliceMsg, .-WriteSliceMsg
	.align	3
.LC121:
	.xword	.LC120
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	StopPicNum
	.type	StopPicNum, %function
StopPicNum:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	mov	x0, x30
	bl	_mcount
	ldp	x29, x30, [sp], 16
	ret
	.size	StopPicNum, .-StopPicNum
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_GetPicStreamSize
	.type	H264_GetPicStreamSize, %function
H264_GetPicStreamSize:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 61440
	bl	_mcount
	mov	w0, 0
	ldr	x2, [x19, 3848]
	cbz	x2, .L1402
	.p2align 2
.L1403:
	ldp	w3, w1, [x2, 8]
	ldr	x2, [x2, 4056]
	add	w1, w3, w1
	add	w1, w1, 7
	add	w0, w0, w1, lsr 3
	cbnz	x2, .L1403
.L1402:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	H264_GetPicStreamSize, .-H264_GetPicStreamSize
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SliceCheck
	.type	SliceCheck, %function
SliceCheck:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 20480
	add	x20, x19, 65536
	bl	_mcount
	ldr	x1, .LC123
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1636]
	ldr	w1, [x20, 56]
	cbnz	w1, .L1416
	mov	w1, 262143
	cmp	w0, w1
	bhi	.L1417
	ldr	w1, [x19, 14932]
	ldr	w2, [x19, 14936]
	mul	w1, w1, w2
	sub	w1, w1, #1
	cmp	w0, w1
	bhi	.L1416
	ldr	x1, .LC127
	mov	x0, x19
	bl	ue_v
	mov	w2, w0
	ldr	x1, .LC129
	mov	w19, w0
	mov	w0, 19
	bl	dprint_vfmw
	ldr	w0, [x20, 56]
	cbnz	w0, .L1416
	cmp	w19, 9
	bls	.L1418
	ldr	x1, .LC132
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
.L1416:
	mov	w0, -1
.L1408:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1418:
	ldr	x0, .LC130
	ldrb	w0, [x0,w19,uxtw]
	adr	x1, .Lrtx1413
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1413:
	.section	.rodata
	.align	0
	.align	2
.L1413:
	.byte	(.L1412 - .Lrtx1413) / 4
	.byte	(.L1414 - .Lrtx1413) / 4
	.byte	(.L1415 - .Lrtx1413) / 4
	.byte	(.L1412 - .Lrtx1413) / 4
	.byte	(.L1415 - .Lrtx1413) / 4
	.byte	(.L1412 - .Lrtx1413) / 4
	.byte	(.L1414 - .Lrtx1413) / 4
	.byte	(.L1415 - .Lrtx1413) / 4
	.byte	(.L1412 - .Lrtx1413) / 4
	.byte	(.L1415 - .Lrtx1413) / 4
	.text
	.p2align 3
.L1412:
	strb	wzr, [x21, 1584]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1415:
	mov	w1, 2
	strb	w1, [x21, 1584]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1414:
	mov	w1, 1
	strb	w1, [x21, 1584]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1417:
	ldr	x1, .LC125
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1408
	.size	SliceCheck, .-SliceCheck
	.align	3
.LC123:
	.xword	.LC122
	.align	3
.LC125:
	.xword	.LC124
	.align	3
.LC127:
	.xword	.LC126
	.align	3
.LC129:
	.xword	.LC128
	.align	3
.LC130:
	.xword	.L1413
	.align	3
.LC132:
	.xword	.LC131
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PPSSPSCheck
	.type	PPSSPSCheck, %function
PPSSPSCheck:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 20480
	bl	_mcount
	ldr	x1, .LC134
	mov	w0, 19
	ldr	w2, [x20, 1596]
	bl	dprint_vfmw
	ldr	w2, [x20, 1596]
	mov	w0, 2208
	ldr	x1, [x19, 14968]
	umull	x0, w2, w0
	add	x0, x1, x0
	ldrb	w1, [x0, 19]
	cbz	w1, .L1423
	ldr	w2, [x0, 28]
	mov	w1, 4000
	ldr	x3, [x19, 14960]
	mov	w0, 0
	umull	x1, w2, w1
	add	x1, x3, x1
	ldrb	w1, [x1, 23]
	cbz	w1, .L1424
.L1421:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1423:
	ldr	x1, .LC136
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1421
	.p2align 3
.L1424:
	ldr	x1, .LC138
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1421
	.size	PPSSPSCheck, .-PPSSPSCheck
	.align	3
.LC134:
	.xword	.LC133
	.align	3
.LC136:
	.xword	.LC135
	.align	3
.LC138:
	.xword	.LC137
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PPSSPSCheckTmpId
	.type	PPSSPSCheckTmpId, %function
PPSSPSCheckTmpId:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	ldr	x1, .LC140
	mov	w2, w20
	mov	w0, 19
	bl	dprint_vfmw
	mov	w0, 2208
	ldr	x1, [x19, 14968]
	umull	x0, w20, w0
	add	x0, x1, x0
	ldrb	w1, [x0, 19]
	cbz	w1, .L1430
	ldr	w1, [x19, 14944]
	ldr	w2, [x0, 28]
	sub	w0, w1, #1
	cmp	w2, w0
	bhi	.L1431
	mov	w1, 4000
	ldr	x3, [x19, 14960]
	mov	w0, 0
	umull	x1, w2, w1
	add	x1, x3, x1
	ldrb	w1, [x1, 23]
	cbz	w1, .L1432
.L1427:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1430:
	ldr	x1, .LC142
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1427
	.p2align 3
.L1431:
	ldr	x1, .LC144
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1427
	.p2align 3
.L1432:
	ldr	x1, .LC146
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1427
	.size	PPSSPSCheckTmpId, .-PPSSPSCheckTmpId
	.align	3
.LC140:
	.xword	.LC139
	.align	3
.LC142:
	.xword	.LC141
	.align	3
.LC144:
	.xword	.LC143
	.align	3
.LC146:
	.xword	.LC145
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsNewPic
	.type	IsNewPic, %function
IsNewPic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 20480
	ldr	w2, [x19, 15012]
	ldr	w0, [x1, 1596]
	ldr	w3, [x1, 3240]
	ldr	w6, [x1, 3236]
	cmp	w3, w0
	ldr	w5, [x1, 3248]
	ldr	w0, [x1, 1604]
	ccmp	w6, w2, 0, eq
	ldrb	w4, [x1, 1585]
	cset	w2, ne
	ldrb	w3, [x1, 3232]
	cmp	w5, w0
	ccmp	w3, w4, 0, eq
	cset	w0, ne
	cmp	w4, wzr
	ccmp	w3, wzr, 4, ne
	orr	w2, w2, w0
	beq	.L1435
	ldrb	w0, [x1, 1586]
	ldrb	w3, [x1, 3233]
	cmp	w3, w0
	cset	w0, ne
	orr	w2, w2, w0
.L1435:
	ldr	w3, [x1, 1616]
	ldr	w0, [x1, 3244]
	cmp	w0, w3
	beq	.L1440
	cmp	w3, wzr
	ccmp	w0, wzr, 4, ne
	cset	w0, eq
.L1436:
	ldrb	w4, [x1, 3234]
	ldrb	w3, [x1, 1587]
	cmp	w4, w3
	cset	w5, ne
	cmp	w4, 5
	ccmp	w3, 5, 0, eq
	orr	w2, w5, w2
	orr	w2, w2, w0
	beq	.L1444
.L1437:
	add	x19, x19, 16384
	ldr	w0, [x19, 792]
	cbnz	w0, .L1438
	ldr	w4, [x1, 704]
	ldr	w5, [x1, 3256]
	ldr	w0, [x1, 708]
	ldr	w3, [x1, 3260]
	cmp	w5, w4
	ccmp	w3, w0, 0, eq
	cset	w0, ne
	orr	w2, w2, w0
.L1439:
	ldr	w3, [x1, 1636]
	mov	w0, 19
	ldr	x1, .LC148
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
.L1438:
	cmp	w0, 1
	bne	.L1439
	ldr	w4, [x1, 752]
	ldr	w5, [x1, 3264]
	ldr	w0, [x1, 756]
	ldr	w3, [x1, 3268]
	cmp	w5, w4
	ccmp	w3, w0, 0, eq
	cset	w0, ne
	orr	w2, w2, w0
	b	.L1439
	.p2align 3
.L1440:
	mov	w0, 0
	b	.L1436
	.p2align 3
.L1444:
	ldr	w0, [x1, 1624]
	ldr	w3, [x1, 3252]
	cmp	w3, w0
	cset	w0, ne
	orr	w2, w2, w0
	b	.L1437
	.size	IsNewPic, .-IsNewPic
	.align	3
.LC148:
	.xword	.LC147
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ref_pic_list_reordering
	.type	ref_pic_list_reordering, %function
ref_pic_list_reordering:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 20480
	mov	w23, 1
	bl	_mcount
	mov	w1, 2208
	ldr	x5, [x19, 14968]
	ldr	w2, [x20, 1596]
	mov	w0, 4000
	ldr	x4, [x19, 14960]
	ldrb	w3, [x20, 1585]
	umull	x1, w2, w1
	add	x1, x5, x1
	ldr	w1, [x1, 28]
	strb	wzr, [x20, 796]
	strb	wzr, [x20, 797]
	umull	x0, w1, w0
	add	x0, x4, x0
	ldr	w0, [x0, 2196]
	cbz	w3, .L1446
	add	w0, w0, 5
	add	x22, x19, 65536
	lsl	w23, w23, w0
	ldrb	w0, [x20, 1584]
	cmp	w0, 1
	bls	.L1497
.L1496:
	ldr	w3, [x22, 56]
.L1464:
	cmp	w3, wzr
	csetm	w0, ne
.L1495:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1446:
	add	w0, w0, 4
	add	x22, x19, 65536
	lsl	w23, w23, w0
	ldrb	w0, [x20, 1584]
	cmp	w0, 1
	bhi	.L1496
.L1497:
	ldr	x1, .LC150
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 796]
	cbz	w0, .L1455
	ldr	w0, [x20, 1640]
	cmn	w0, #2
	beq	.L1478
	ldr	x24, .LC152
	mov	w27, 0
	ldr	x26, .LC160
	ldr	x25, .LC156
	b	.L1461
	.p2align 3
.L1456:
	cmp	w0, 2
	beq	.L1498
.L1459:
	ldr	w0, [x20, 1640]
	add	w0, w0, 2
	cmp	w0, w27
	bls	.L1499
.L1461:
	add	x21, x19, x27, uxtw 2
	mov	x1, x24
	add	x21, x21, 20480
	mov	x0, x19
	add	w27, w27, 1
	bl	ue_v
	cmp	w0, 3
	str	w0, [x21, 800]
	ldr	w3, [x22, 56]
	bhi	.L1452
	cbnz	w3, .L1452
	beq	.L1455
	cmp	w0, 1
	bhi	.L1456
	mov	x1, x25
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1064]
	cmp	w23, w0
	ldr	w3, [x22, 56]
	bls	.L1458
	cbz	w3, .L1459
.L1458:
	ldr	x1, .LC158
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1495
	.p2align 3
.L1455:
	ldrb	w0, [x20, 1584]
.L1448:
	cmp	w0, 1
	bne	.L1496
	ldr	x1, .LC164
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 797]
	cbz	w0, .L1496
	ldr	w0, [x20, 1644]
	cmn	w0, #2
	beq	.L1496
	ldr	x24, .LC166
	mov	w27, 0
	ldr	x26, .LC174
	ldr	x25, .LC170
	b	.L1476
	.p2align 3
.L1470:
	cmp	w0, 2
	beq	.L1500
	ldr	w0, [x20, 1644]
	add	w0, w0, 2
	cmp	w27, w0
	beq	.L1477
.L1475:
	add	w27, w27, 1
	cmp	w27, w0
	bcs	.L1496
.L1476:
	add	x21, x19, x27, uxtw 2
	mov	x1, x24
	add	x21, x21, 20480
	mov	x0, x19
	bl	ue_v
	cmp	w0, 3
	str	w0, [x21, 932]
	ldr	w3, [x22, 56]
	bhi	.L1468
	cbnz	w3, .L1468
	beq	.L1464
	cmp	w0, 1
	bhi	.L1470
	mov	x1, x25
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1196]
	cmp	w23, w0
	ldr	w3, [x22, 56]
	bls	.L1472
	cbnz	w3, .L1472
.L1473:
	ldr	w0, [x20, 1644]
	add	w0, w0, 2
	cmp	w0, w27
	bne	.L1475
	ldr	w1, [x21, 932]
	cmp	w1, 3
	beq	.L1475
.L1477:
	ldr	x1, .LC176
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1495
	.p2align 3
.L1498:
	mov	x1, x26
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1328]
	b	.L1459
	.p2align 3
.L1500:
	mov	x1, x26
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1456]
	b	.L1473
	.p2align 3
.L1452:
	ldr	x1, .LC154
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1495
	.p2align 3
.L1468:
	ldr	x1, .LC168
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1495
	.p2align 3
.L1499:
	bne	.L1455
.L1450:
	uxtw	x0, w27
	add	x0, x0, 4096
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 4896]
	cmp	w0, 3
	bne	.L1462
	ldrb	w0, [x20, 1584]
	add	x22, x19, 65536
	b	.L1448
.L1472:
	ldr	x1, .LC172
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1495
.L1478:
	mov	w27, 0
	b	.L1450
.L1462:
	ldr	x1, .LC162
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1495
	.size	ref_pic_list_reordering, .-ref_pic_list_reordering
	.align	3
.LC150:
	.xword	.LC149
	.align	3
.LC152:
	.xword	.LC151
	.align	3
.LC154:
	.xword	.LC153
	.align	3
.LC156:
	.xword	.LC155
	.align	3
.LC158:
	.xword	.LC157
	.align	3
.LC160:
	.xword	.LC159
	.align	3
.LC162:
	.xword	.LC161
	.align	3
.LC164:
	.xword	.LC163
	.align	3
.LC166:
	.xword	.LC165
	.align	3
.LC168:
	.xword	.LC167
	.align	3
.LC170:
	.xword	.LC169
	.align	3
.LC172:
	.xword	.LC171
	.align	3
.LC174:
	.xword	.LC173
	.align	3
.LC176:
	.xword	.LC175
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	pred_weight_table
	.type	pred_weight_table, %function
pred_weight_table:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x26, x0
	mov	x0, x30
	add	x21, x26, 20480
	mov	w24, 2208
	bl	_mcount
	mov	w20, 4000
	ldr	x2, [x26, 14968]
	mov	x0, x26
	ldr	w4, [x21, 1596]
	mov	w19, 1
	ldr	x1, .LC178
	mov	w25, 0
	ldr	x5, [x26, 14960]
	umull	x4, w4, w24
	add	x24, x2, x4
	ldr	w2, [x24, 28]
	umull	x2, w2, w20
	add	x20, x5, x2
	bl	ue_v
	str	w0, [x21, 1648]
	lsl	w23, w19, w0
	ldr	w1, [x20, 40]
	cbnz	w1, .L1526
.L1502:
	cmp	w0, 7
	bhi	.L1505
	ldr	w0, [x21, 1652]
	cmp	w0, 7
	bhi	.L1505
	add	x0, x26, 65536
	str	x0, [x29, 104]
	ldr	w2, [x0, 56]
	cbnz	w2, .L1504
	ldr	x22, .LC184
	mov	w27, 0
	b	.L1513
	.p2align 3
.L1508:
	str	w23, [x1, 1656]
	add	x0, x26, x19, lsl 2
	str	wzr, [x1, 2424]
	add	x0, x0, 20480
	ldr	w1, [x20, 40]
	cbnz	w1, .L1527
.L1510:
	str	wzr, [x0, 1784]
	str	wzr, [x0, 1912]
	str	wzr, [x0, 2552]
	str	wzr, [x0, 2680]
.L1512:
	ldr	w0, [x21, 1640]
	add	w27, w27, 1
	cmp	w0, w27
	bcc	.L1528
.L1513:
	mov	x1, x22
	mov	x0, x26
	uxtw	x19, w27
	bl	u_1
	add	x1, x26, x19, lsl 2
	add	x1, x1, 20480
	cbz	w0, .L1508
	ldr	x1, .LC186
	mov	x0, x26
	bl	se_v
	add	x4, x26, x19, lsl 2
	ldr	x1, .LC188
	add	x28, x4, 20480
	str	w0, [x28, 1656]
	mov	x0, x26
	bl	se_v
	str	w0, [x28, 2424]
	add	x0, x26, x19, lsl 2
	ldr	w1, [x20, 40]
	add	x0, x0, 20480
	cbz	w1, .L1510
.L1527:
	ldr	x1, .LC190
	mov	x0, x26
	bl	u_1
	add	x1, x26, x19, lsl 2
	add	x1, x1, 20480
	cbnz	w0, .L1529
	str	w25, [x1, 1784]
	add	w27, w27, 1
	str	w25, [x1, 1912]
	str	wzr, [x1, 2552]
	str	wzr, [x1, 2680]
	ldr	w0, [x21, 1640]
	cmp	w0, w27
	bcs	.L1513
.L1528:
	ldr	w0, [x24, 44]
	cmp	w0, 1
	beq	.L1514
.L1515:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 56]
	cmp	w0, wzr
	csetm	w0, ne
.L1507:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1529:
	ldr	x1, .LC192
	mov	x0, x26
	add	x19, x26, x19, lsl 2
	add	x19, x19, 20480
	bl	se_v
	ldr	x1, .LC194
	str	w0, [x19, 1784]
	mov	x0, x26
	bl	se_v
	str	w0, [x19, 2552]
	ldr	x1, .LC192
	mov	x0, x26
	bl	se_v
	str	w0, [x19, 1912]
	ldr	x1, .LC194
	mov	x0, x26
	bl	se_v
	str	w0, [x19, 2680]
	b	.L1512
	.p2align 3
.L1526:
	ldr	x1, .LC180
	mov	x0, x26
	bl	ue_v
	mov	w25, w0
	ldr	w0, [x21, 1648]
	str	w25, [x21, 1652]
	lsl	w25, w19, w25
	b	.L1502
	.p2align 3
.L1514:
	ldrb	w0, [x21, 1584]
	cmp	w0, 1
	bne	.L1515
	ldr	x22, .LC196
	mov	w28, 0
	ldr	x27, .LC198
	b	.L1521
	.p2align 3
.L1516:
	add	x0, x26, x19, lsl 2
	add	x0, x0, 20480
	str	w23, [x0, 2040]
	str	wzr, [x0, 2808]
	add	x0, x26, x19, lsl 2
	add	x0, x0, 20480
	ldr	w1, [x20, 40]
	cbnz	w1, .L1530
.L1518:
	str	wzr, [x0, 2168]
	str	wzr, [x0, 2296]
	str	wzr, [x0, 2936]
	str	wzr, [x0, 3064]
.L1520:
	ldr	w0, [x21, 1644]
	add	w28, w28, 1
	cmp	w0, w28
	bcc	.L1515
.L1521:
	mov	x1, x22
	mov	x0, x26
	uxtw	x19, w28
	bl	u_1
	cbz	w0, .L1516
	mov	x1, x27
	mov	x0, x26
	bl	se_v
	add	x4, x26, x19, lsl 2
	ldr	x1, .LC200
	add	x24, x4, 20480
	str	w0, [x24, 2040]
	mov	x0, x26
	bl	se_v
	str	w0, [x24, 2808]
	add	x0, x26, x19, lsl 2
	ldr	w1, [x20, 40]
	add	x0, x0, 20480
	cbz	w1, .L1518
.L1530:
	ldr	x1, .LC202
	mov	x0, x26
	bl	u_1
	cbnz	w0, .L1531
	add	x0, x26, x19, lsl 2
	add	x0, x0, 20480
	str	w25, [x0, 2168]
	str	w25, [x0, 2296]
	str	wzr, [x0, 2936]
	str	wzr, [x0, 3064]
	b	.L1520
.L1505:
	add	x3, x26, 65536
	ldr	w2, [x3, 56]
.L1504:
	ldr	x1, .LC182
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1507
.L1531:
	ldr	x1, .LC204
	mov	x0, x26
	add	x19, x26, x19, lsl 2
	add	x19, x19, 20480
	bl	se_v
	ldr	x1, .LC206
	str	w0, [x19, 2168]
	mov	x0, x26
	bl	se_v
	str	w0, [x19, 2936]
	ldr	x1, .LC204
	mov	x0, x26
	bl	se_v
	str	w0, [x19, 2296]
	ldr	x1, .LC206
	mov	x0, x26
	bl	se_v
	str	w0, [x19, 3064]
	b	.L1520
	.size	pred_weight_table, .-pred_weight_table
	.align	3
.LC178:
	.xword	.LC177
	.align	3
.LC180:
	.xword	.LC179
	.align	3
.LC182:
	.xword	.LC181
	.align	3
.LC184:
	.xword	.LC183
	.align	3
.LC186:
	.xword	.LC185
	.align	3
.LC188:
	.xword	.LC187
	.align	3
.LC190:
	.xword	.LC189
	.align	3
.LC192:
	.xword	.LC191
	.align	3
.LC194:
	.xword	.LC193
	.align	3
.LC196:
	.xword	.LC195
	.align	3
.LC198:
	.xword	.LC197
	.align	3
.LC200:
	.xword	.LC199
	.align	3
.LC202:
	.xword	.LC201
	.align	3
.LC204:
	.xword	.LC203
	.align	3
.LC206:
	.xword	.LC205
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecMMCO
	.type	DecMMCO, %function
DecMMCO:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	w19, w1
	mov	x0, x30
	bl	_mcount
	cbnz	w19, .L1533
	mov	w22, 100
	ldr	x21, .LC210
	ldr	x25, .LC212
	add	x23, x20, 65536
	ldr	x27, .LC218
	ldr	x24, .LC216
	ldr	x26, .LC214
	b	.L1544
	.p2align 3
.L1545:
	cmp	w19, 2
	beq	.L1558
.L1546:
	cmp	w19, 6
	cset	w0, eq
	orr	w0, w28, w0
	cbnz	w0, .L1549
	cmp	w19, 4
	beq	.L1559
.L1550:
	cmp	w19, 6
	bhi	.L1542
	ldr	w0, [x23, 56]
	cbnz	w0, .L1542
.L1560:
	cbz	w19, .L1552
.L1551:
	subs	w22, w22, #1
	beq	.L1553
.L1544:
	mov	x1, x21
	mov	x0, x20
	bl	ue_v
	cmp	w0, 3
	mov	w19, w0
	and	w0, w0, -3
	cset	w28, eq
	cmp	w0, 1
	bne	.L1545
	mov	x1, x25
	mov	x0, x20
	bl	ue_v
	cmp	w19, 2
	bne	.L1546
.L1558:
	mov	x1, x26
	mov	x0, x20
	bl	ue_v
	ldr	w0, [x23, 56]
	cbz	w0, .L1560
	.p2align 2
.L1542:
	ldr	x1, .LC220
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L1536:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1549:
	mov	x1, x24
	mov	x0, x20
	bl	ue_v
	cmp	w19, 4
	bne	.L1550
.L1559:
	mov	x1, x27
	mov	x0, x20
	bl	ue_v
	ldr	w0, [x23, 56]
	cbz	w0, .L1551
	b	.L1542
	.p2align 3
.L1533:
	ldr	x21, .LC210
	mov	w26, 0
	ldr	x25, .LC212
	add	x22, x20, 24576
	ldr	x24, .LC216
	add	x23, x20, 65536
	b	.L1535
	.p2align 3
.L1537:
	cmp	w1, 2
	beq	.L1561
.L1538:
	cmp	w1, 3
	ccmp	w1, 6, 4, ne
	beq	.L1562
.L1539:
	cmp	w1, 4
	beq	.L1563
.L1540:
	cmp	w1, 6
	bhi	.L1542
	ldr	w0, [x23, 56]
	cbnz	w0, .L1542
	add	w26, w26, 1
	cbz	w1, .L1552
	cmp	w26, 100
	beq	.L1564
.L1535:
	ldr	w28, [x22, 3188]
	mov	x1, x21
	mov	x0, x20
	uxtw	x27, w26
	lsl	x19, x27, 4
	bl	ue_v
	uxtw	x28, w28
	mov	w1, w0
	lsl	x0, x28, 6
	sub	x0, x0, x28
	add	x19, x19, x27, lsl 2
	lsl	x0, x0, 2
	and	w2, w1, -3
	sub	x0, x0, x28
	cmp	w2, 1
	lsl	x0, x0, 1
	sub	x0, x0, x28
	add	x19, x19, x0, lsl 2
	add	x19, x20, x19
	add	x19, x19, 20480
	str	w1, [x19, 3280]
	bne	.L1537
	mov	x1, x25
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3284]
	ldr	w1, [x19, 3280]
	cmp	w1, 2
	bne	.L1538
.L1561:
	ldr	x1, .LC214
	mov	x0, x20
	bl	ue_v
	lsl	x1, x28, 6
	sub	x1, x1, x28
	lsl	x1, x1, 2
	sub	x1, x1, x28
	lsl	x1, x1, 1
	sub	x2, x1, x28
	lsl	x1, x27, 4
	add	x1, x1, x27, lsl 2
	add	x1, x1, x2, lsl 2
	add	x1, x20, x1
	add	x1, x1, 20480
	str	w0, [x1, 3288]
	ldr	w1, [x1, 3280]
	cmp	w1, 3
	ccmp	w1, 6, 4, ne
	bne	.L1539
	.p2align 2
.L1562:
	mov	x1, x24
	mov	x0, x20
	bl	ue_v
	lsl	x1, x28, 6
	lsl	x2, x27, 4
	sub	x1, x1, x28
	add	x2, x2, x27, lsl 2
	lsl	x1, x1, 2
	sub	x1, x1, x28
	lsl	x1, x1, 1
	sub	x1, x1, x28
	add	x2, x2, x1, lsl 2
	add	x2, x20, x2
	add	x2, x2, 20480
	ldr	w1, [x2, 3280]
	str	w0, [x2, 3292]
	cmp	w1, 4
	bne	.L1540
.L1563:
	ldr	x1, .LC218
	mov	x0, x20
	bl	ue_v
	lsl	x1, x28, 6
	sub	x1, x1, x28
	lsl	x1, x1, 2
	sub	x1, x1, x28
	lsl	x1, x1, 1
	sub	x28, x1, x28
	lsl	x1, x27, 4
	add	x3, x1, x27, lsl 2
	add	x3, x3, x28, lsl 2
	add	x3, x20, x3
	add	x3, x3, 20480
	ldr	w1, [x3, 3280]
	str	w0, [x3, 3296]
	b	.L1540
	.p2align 3
.L1552:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1564:
	ldr	x1, .LC208
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1536
.L1553:
	mov	w0, -1
	b	.L1536
	.size	DecMMCO, .-DecMMCO
	.align	3
.LC208:
	.xword	.LC207
	.align	3
.LC210:
	.xword	.LC209
	.align	3
.LC212:
	.xword	.LC211
	.align	3
.LC214:
	.xword	.LC213
	.align	3
.LC216:
	.xword	.LC215
	.align	3
.LC218:
	.xword	.LC217
	.align	3
.LC220:
	.xword	.LC219
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	dec_ref_pic_marking
	.type	dec_ref_pic_marking, %function
dec_ref_pic_marking:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 24576
	bl	_mcount
	add	x1, x21, 20480
	ldr	w23, [x20, 3188]
	ldrb	w0, [x1, 1589]
	cbz	w0, .L1566
	cmp	w23, wzr
	cset	w19, eq
	str	w19, [x20, 3188]
	ldrb	w0, [x1, 1587]
	cmp	w0, 5
	beq	.L1584
.L1567:
	ldr	x1, .LC228
	mov	x0, x21
	bl	u_1
	mov	w4, w0
	lsl	x3, x19, 6
	ldr	x1, .LC230
	sub	x3, x3, x19
	uxtb	w2, w0
	lsl	x3, x3, 2
	mov	w0, 16
	sub	x3, x3, x19
	lsl	x3, x3, 1
	sub	x19, x3, x19
	add	x19, x21, x19, lsl 2
	add	x19, x19, 20480
	strb	w4, [x19, 3279]
	bl	dprint_vfmw
	ldrb	w0, [x19, 3279]
	cbnz	w0, .L1569
.L1573:
	mov	w22, 0
.L1581:
	mov	w0, w22
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1566:
	uxtw	x19, w23
	lsl	x0, x19, 6
	sub	x0, x0, x19
	lsl	x0, x0, 2
	sub	x0, x0, x19
	lsl	x0, x0, 1
	sub	x19, x0, x19
	add	x19, x21, x19, lsl 2
	add	x19, x19, 20480
	ldrb	w0, [x19, 3276]
	cbnz	w0, .L1585
	ldrb	w0, [x1, 1587]
	uxtw	x19, w23
	cmp	w0, 5
	bne	.L1567
.L1584:
	ldr	x1, .LC222
	mov	x0, x21
	mov	w22, 0
	bl	u_1
	lsl	x2, x19, 6
	ldr	x1, .LC224
	sub	x2, x2, x19
	lsl	x2, x2, 2
	sub	x2, x2, x19
	lsl	x2, x2, 1
	sub	x19, x2, x19
	add	x19, x21, x19, lsl 2
	add	x19, x19, 20480
	strb	w0, [x19, 3277]
	mov	x0, x21
	bl	u_1
	strb	w0, [x19, 3278]
	ldrb	w2, [x19, 3277]
	mov	w0, 16
	ldr	x1, .LC226
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1585:
	ldrb	w0, [x1, 1587]
	cmp	w0, 5
	beq	.L1586
	ldr	x1, .LC228
	mov	x0, x21
	ldrb	w20, [x19, 3279]
	bl	u_1
	cmp	w20, w0
	bne	.L1587
	ldrb	w0, [x19, 3279]
	cbz	w0, .L1573
	mov	w1, 0
	mov	x0, x21
	bl	DecMMCO
	mov	w22, w0
	b	.L1581
	.p2align 3
.L1569:
	mov	w1, 1
	mov	x0, x21
	bl	DecMMCO
	cmn	w0, #1
	mov	w22, w0
	beq	.L1588
	mov	w0, 1
	strb	w0, [x19, 3276]
	mov	w22, 0
	mov	w0, w22
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1586:
	ldr	x1, .LC222
	mov	x0, x21
	ldrb	w20, [x19, 3277]
	bl	u_1
	cmp	w20, w0
	bne	.L1589
	ldr	x1, .LC224
	mov	x0, x21
	ldrb	w19, [x19, 3278]
	bl	u_1
	cmp	w19, w0
	beq	.L1573
	ldr	x1, .LC236
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1581
.L1587:
	ldr	x1, .LC238
	add	x21, x21, 65536
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	ldr	x1, .LC240
	mov	w0, 1
	ldr	w4, [x21, 60]
	ldr	w2, [x21, 80]
	ldr	w3, [x21, 96]
	bl	dprint_vfmw
	b	.L1581
.L1589:
	ldr	x1, .LC234
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1581
.L1588:
	strb	wzr, [x19, 3276]
	mov	w0, 1
	ldr	x1, .LC232
	str	w23, [x20, 3188]
	bl	dprint_vfmw
	b	.L1581
	.size	dec_ref_pic_marking, .-dec_ref_pic_marking
	.align	3
.LC222:
	.xword	.LC221
	.align	3
.LC224:
	.xword	.LC223
	.align	3
.LC226:
	.xword	.LC225
	.align	3
.LC228:
	.xword	.LC227
	.align	3
.LC230:
	.xword	.LC229
	.align	3
.LC232:
	.xword	.LC231
	.align	3
.LC234:
	.xword	.LC233
	.align	3
.LC236:
	.xword	.LC235
	.align	3
.LC238:
	.xword	.LC237
	.align	3
.LC240:
	.xword	.LC239
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ProcessSliceHeaderFirstPart
	.type	ProcessSliceHeaderFirstPart, %function
ProcessSliceHeaderFirstPart:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x23, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC242
	mov	x0, x23
	bl	ue_v
	mov	w21, w0
	ldr	w0, [x23, 14948]
	sub	w0, w0, #1
	cmp	w21, w0
	bhi	.L1591
	add	x24, x23, 65536
	ldr	w0, [x24, 56]
	cbnz	w0, .L1591
	mov	w1, w21
	mov	x0, x23
	bl	PPSSPSCheckTmpId
	mov	w22, w0
	cbnz	w0, .L1654
	add	x19, x23, 20480
	mov	w1, 2208
	mov	w20, 4000
	umull	x1, w21, w1
	str	w21, [x19, 1596]
	ldrb	w0, [x19, 1587]
	ldr	x21, [x23, 14968]
	cmp	w0, 5
	ldr	x2, [x23, 14960]
	add	x21, x21, x1
	ldr	w0, [x21, 28]
	umull	x0, w0, w20
	add	x20, x2, x0
	beq	.L1595
	ldr	w0, [x20, 3244]
	cbz	w0, .L1595
.L1596:
	ldr	w1, [x20, 2196]
	mov	x0, x23
	ldr	x2, .LC250
	add	w1, w1, 4
	bl	u_v
	str	w0, [x19, 1604]
	ldr	w1, [x24, 56]
	cbnz	w1, .L1620
	ldrb	w1, [x19, 1587]
	cmp	w1, 5
	beq	.L1655
.L1599:
	strb	wzr, [x19, 1585]
	strb	wzr, [x19, 1586]
	ldrb	w0, [x20, 18]
	cbz	w0, .L1656
	ldr	w1, [x20, 3252]
	ldr	w2, [x20, 3248]
	add	w0, w1, 1
	madd	w0, w2, w0, w0
.L1623:
	ldrb	w1, [x20, 19]
	cbnz	w1, .L1606
	strb	wzr, [x19, 1588]
.L1608:
	ldr	w1, [x19, 1636]
	cmp	w1, w0
	bcs	.L1657
.L1609:
	ldrb	w0, [x19, 1587]
	cmp	w0, 5
	beq	.L1658
.L1611:
	str	wzr, [x19, 704]
	str	wzr, [x19, 708]
	ldr	w0, [x20, 2200]
	cbz	w0, .L1659
.L1615:
	str	wzr, [x19, 752]
	str	wzr, [x19, 756]
	ldr	w0, [x20, 2200]
	cmp	w0, 1
	beq	.L1660
.L1618:
	ldr	w0, [x24, 56]
	cbnz	w0, .L1620
	ldrb	w0, [x21, 17]
	cbz	w0, .L1621
	ldr	x1, .LC274
	mov	x0, x23
	bl	ue_v
	cbnz	w0, .L1622
	ldr	w0, [x24, 56]
	cbnz	w0, .L1622
.L1621:
	mov	x0, x23
	bl	IsNewPic
	uxtb	w0, w0
	strb	w0, [x19, 1589]
	cbnz	w0, .L1634
	ldr	w1, [x19, 1636]
	ldr	w0, [x19, 3272]
	cmp	w1, w0
	bls	.L1661
.L1634:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1595:
	ldrb	w0, [x19, 1584]
	cmp	w0, 2
	beq	.L1596
	ldr	x1, .LC248
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1634
	.p2align 3
.L1606:
	mov	w1, 1
	strb	w1, [x19, 1588]
.L1625:
	ldr	w1, [x19, 1636]
	cmp	w1, w0, lsr 1
	bcc	.L1609
	ldr	x1, .LC258
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1634
	.p2align 3
.L1622:
	ldr	x1, .LC276
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC277
	ldr	x4, [x0]
	cbz	x4, .L1620
	ldr	w0, [x24, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	mov	w22, -1
	blr	x4
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1656:
	ldr	x1, .LC254
	mov	x0, x23
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 1585]
	cbnz	w0, .L1662
	ldr	w2, [x20, 3252]
	ldr	w1, [x20, 3248]
	add	w0, w2, 1
	ldrb	w3, [x20, 18]
	madd	w1, w1, w0, w0
	lsl	w0, w1, 1
	cbz	w3, .L1623
.L1603:
	ldrb	w2, [x19, 1585]
	mov	w0, 1
	b	.L1604
	.p2align 3
.L1659:
	ldr	w1, [x20, 2204]
	mov	x0, x23
	ldr	x2, .LC266
	add	w1, w1, 4
	bl	u_v
	str	w0, [x19, 704]
	ldrb	w0, [x21, 1]
	cbz	w0, .L1615
	ldrb	w0, [x19, 1585]
	cbnz	w0, .L1615
	ldr	x1, .LC268
	mov	x0, x23
	bl	se_v
	str	w0, [x19, 708]
	b	.L1615
	.p2align 3
.L1655:
	str	w0, [x19, 1600]
	cbz	w0, .L1599
	ldr	x1, .LC252
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1599
	.p2align 3
.L1660:
	ldrb	w0, [x20, 16]
	cbnz	w0, .L1618
	ldr	x1, .LC270
	mov	x0, x23
	bl	se_v
	str	w0, [x19, 752]
	ldrb	w0, [x21, 1]
	cmp	w0, 1
	bne	.L1618
	ldrb	w0, [x19, 1585]
	cbnz	w0, .L1618
	ldr	x1, .LC272
	mov	x0, x23
	bl	se_v
	str	w0, [x19, 756]
	b	.L1618
	.p2align 3
.L1620:
	mov	w22, -1
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1662:
	ldr	x1, .LC256
	mov	x0, x23
	bl	u_1
	strb	w0, [x19, 1586]
	ldr	w0, [x20, 3252]
	ldr	w1, [x20, 3248]
	ldrb	w2, [x20, 18]
	add	w0, w0, 1
	madd	w1, w1, w0, w0
	cbnz	w2, .L1603
	ldrb	w2, [x19, 1585]
	mov	w0, 2
.L1604:
	mul	w0, w0, w1
	cbz	w2, .L1623
	strb	wzr, [x19, 1588]
	cmp	w2, 1
	bne	.L1608
	b	.L1625
	.p2align 3
.L1658:
	ldr	x1, .LC262
	mov	x0, x23
	bl	ue_v
	str	w0, [x19, 1624]
	mov	w1, 65535
	cmp	w0, w1
	bhi	.L1612
	ldr	w0, [x24, 56]
	cbz	w0, .L1611
.L1612:
	ldr	x1, .LC264
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1611
	.p2align 3
.L1591:
	ldr	x1, .LC244
	mov	w2, w21
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1654:
	ldr	x1, .LC246
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1634
.L1657:
	ldr	x1, .LC260
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1634
.L1661:
	ldr	x1, .LC279
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1634
	.size	ProcessSliceHeaderFirstPart, .-ProcessSliceHeaderFirstPart
	.align	3
.LC242:
	.xword	.LC241
	.align	3
.LC244:
	.xword	.LC243
	.align	3
.LC246:
	.xword	.LC245
	.align	3
.LC248:
	.xword	.LC247
	.align	3
.LC250:
	.xword	.LC249
	.align	3
.LC252:
	.xword	.LC251
	.align	3
.LC254:
	.xword	.LC253
	.align	3
.LC256:
	.xword	.LC255
	.align	3
.LC258:
	.xword	.LC257
	.align	3
.LC260:
	.xword	.LC259
	.align	3
.LC262:
	.xword	.LC261
	.align	3
.LC264:
	.xword	.LC263
	.align	3
.LC266:
	.xword	.LC265
	.align	3
.LC268:
	.xword	.LC267
	.align	3
.LC270:
	.xword	.LC269
	.align	3
.LC272:
	.xword	.LC271
	.align	3
.LC274:
	.xword	.LC273
	.align	3
.LC276:
	.xword	.LC275
	.align	3
.LC277:
	.xword	g_event_report
	.align	3
.LC279:
	.xword	.LC278
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ProcessSliceHeaderSecondPart
	.type	ProcessSliceHeaderSecondPart, %function
ProcessSliceHeaderSecondPart:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 20480
	bl	_mcount
	ldr	w0, [x20, 14948]
	ldr	w2, [x19, 1596]
	sub	w0, w0, #1
	cmp	w0, w2
	bcc	.L1664
	add	x24, x20, 65536
	ldr	w0, [x24, 56]
	cbnz	w0, .L1664
	mov	x0, x20
	bl	PPSSPSCheck
	cbnz	w0, .L1747
	ldr	w22, [x19, 1596]
	mov	w0, 2208
	ldrb	w1, [x19, 1584]
	ldr	x25, [x20, 14968]
	umull	x22, w22, w0
	cmp	w1, 1
	add	x21, x25, x22
	beq	.L1748
	ldr	w0, [x21, 36]
	cmp	w1, 1
	str	w0, [x19, 1640]
	ldr	w0, [x21, 40]
	str	w0, [x19, 1644]
	bls	.L1749
.L1669:
	cmp	w1, 2
	beq	.L1695
.L1679:
	mov	w0, 32
	str	w0, [x19, 1644]
.L1680:
	mov	x0, x20
	bl	ref_pic_list_reordering
	mov	w23, w0
	cbnz	w0, .L1750
	ldrb	w0, [x21, 2]
	cbz	w0, .L1682
	ldrb	w0, [x19, 1584]
	cbnz	w0, .L1682
	mov	w0, 1
	mov	w2, w0
.L1756:
	ldr	x1, .LC297
	strb	w0, [x19, 1591]
	mov	w0, 19
	bl	dprint_vfmw
	str	wzr, [x19, 1648]
	ldrb	w0, [x19, 1591]
	str	wzr, [x19, 1652]
	cbz	w0, .L1684
.L1752:
	mov	x0, x20
	bl	pred_weight_table
	cbz	w0, .L1684
	ldr	x1, .LC299
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1682:
	ldr	w0, [x21, 44]
	cmp	w0, 1
	beq	.L1751
.L1699:
	mov	w0, 0
	ldr	x1, .LC297
	strb	w0, [x19, 1591]
	mov	w2, 0
	mov	w0, 19
	bl	dprint_vfmw
	str	wzr, [x19, 1648]
	ldrb	w0, [x19, 1591]
	str	wzr, [x19, 1652]
	cbnz	w0, .L1752
.L1684:
	ldr	w0, [x19, 1616]
	cbnz	w0, .L1753
.L1685:
	str	wzr, [x19, 3192]
	ldrb	w0, [x25, x22]
	cbz	w0, .L1686
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bls	.L1754
.L1686:
	ldr	x1, .LC307
	mov	x0, x20
	bl	se_v
	add	x1, x19, 3072
	stp	w0, wzr, [x1, 124]
	stp	wzr, wzr, [x1, 132]
	ldrb	w0, [x21, 3]
	cbnz	w0, .L1755
.L1715:
	mov	w0, w23
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1755:
	ldr	x1, .LC309
	mov	x0, x20
	bl	ue_v
	cmp	w0, 2
	str	w0, [x19, 3200]
	bhi	.L1689
	ldr	w1, [x24, 56]
	cbnz	w1, .L1689
	cmp	w0, 1
	beq	.L1715
	ldr	x1, .LC313
	mov	x0, x20
	bl	se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 3204]
	bhi	.L1692
	ldr	w0, [x24, 56]
	cbnz	w0, .L1692
	ldr	x1, .LC317
	mov	x0, x20
	bl	se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 3208]
	bhi	.L1694
	ldr	w0, [x24, 56]
	cbz	w0, .L1715
.L1694:
	ldr	x1, .LC319
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1753:
	mov	x0, x20
	bl	dec_ref_pic_marking
	cbz	w0, .L1685
	ldr	x1, .LC301
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1695:
	mov	w0, 32
	str	w0, [x19, 1640]
	b	.L1679
	.p2align 3
.L1751:
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bne	.L1699
	mov	w2, w0
	b	.L1756
	.p2align 3
.L1748:
	ldr	x1, .LC283
	mov	x0, x20
	bl	u_1
	strb	w0, [x19, 1590]
	ldrb	w1, [x19, 1584]
	ldr	w0, [x21, 36]
	str	w0, [x19, 1640]
	cmp	w1, 1
	ldr	w0, [x21, 40]
	str	w0, [x19, 1644]
	bhi	.L1669
	.p2align 2
.L1749:
	ldr	x1, .LC285
	mov	x0, x20
	bl	u_1
	cbnz	w0, .L1670
	ldr	w1, [x19, 1640]
.L1671:
	ldrb	w0, [x19, 1585]
	cbnz	w0, .L1672
	cmp	w1, 15
	bhi	.L1673
	ldr	w0, [x19, 1644]
	cmp	w0, 15
	bhi	.L1757
.L1674:
	ldr	w0, [x24, 56]
	cbnz	w0, .L1673
.L1675:
	ldrb	w0, [x19, 1584]
	cmp	w0, 2
	beq	.L1695
	cmp	w0, 1
	beq	.L1680
	b	.L1679
	.p2align 3
.L1664:
	ldr	x1, .LC280
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	ldr	x25, [sp, 64]
	mov	w0, w23
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1754:
	ldr	x1, .LC303
	mov	x0, x20
	bl	ue_v
	cmp	w0, 2
	str	w0, [x19, 3192]
	bhi	.L1687
	ldr	w0, [x24, 56]
	cbz	w0, .L1686
.L1687:
	ldr	x1, .LC305
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1672:
	cmp	w1, 31
	bhi	.L1676
	ldr	w0, [x19, 1644]
	cmp	w0, 31
	bls	.L1677
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bne	.L1677
.L1676:
	ldr	x1, .LC293
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1677:
	ldr	w0, [x24, 56]
	cbz	w0, .L1675
	b	.L1676
	.p2align 3
.L1757:
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bne	.L1674
.L1673:
	ldr	x1, .LC291
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1670:
	ldr	x1, .LC287
	mov	x0, x20
	bl	ue_v
	mov	w1, w0
	ldrb	w0, [x19, 1584]
	str	w1, [x19, 1640]
	cmp	w0, 1
	bne	.L1671
	ldr	x1, .LC289
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 1644]
	ldr	w1, [x19, 1640]
	b	.L1671
	.p2align 3
.L1692:
	ldr	x1, .LC315
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1689:
	ldr	x1, .LC311
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1747:
	ldr	x1, .LC281
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.p2align 3
.L1750:
	ldr	x1, .LC295
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1715
	.size	ProcessSliceHeaderSecondPart, .-ProcessSliceHeaderSecondPart
	.align	3
.LC280:
	.xword	.LC243
	.align	3
.LC281:
	.xword	.LC245
	.align	3
.LC283:
	.xword	.LC282
	.align	3
.LC285:
	.xword	.LC284
	.align	3
.LC287:
	.xword	.LC286
	.align	3
.LC289:
	.xword	.LC288
	.align	3
.LC291:
	.xword	.LC290
	.align	3
.LC293:
	.xword	.LC292
	.align	3
.LC295:
	.xword	.LC294
	.align	3
.LC297:
	.xword	.LC296
	.align	3
.LC299:
	.xword	.LC298
	.align	3
.LC301:
	.xword	.LC300
	.align	3
.LC303:
	.xword	.LC302
	.align	3
.LC305:
	.xword	.LC304
	.align	3
.LC307:
	.xword	.LC306
	.align	3
.LC309:
	.xword	.LC308
	.align	3
.LC311:
	.xword	.LC310
	.align	3
.LC313:
	.xword	.LC312
	.align	3
.LC315:
	.xword	.LC314
	.align	3
.LC317:
	.xword	.LC316
	.align	3
.LC319:
	.xword	.LC318
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ExitSlice
	.type	ExitSlice, %function
ExitSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 20480
	mov	w2, 2208
	ldr	x8, [x19, 14968]
	mov	w1, 4000
	ldr	x7, [x19, 14960]
	ldr	w4, [x0, 1596]
	ldrb	w3, [x0, 1585]
	ldr	w5, [x0, 1604]
	umull	x2, w4, w2
	ldr	w6, [x0, 1636]
	add	x2, x8, x2
	ldr	w2, [x2, 28]
	str	w4, [x0, 3240]
	umull	x1, w2, w1
	add	x1, x7, x1
	ldr	w2, [x1, 36]
	str	w2, [x0, 3236]
	strb	w3, [x0, 3232]
	str	w5, [x0, 3248]
	str	w6, [x0, 3272]
	cbz	w3, .L1759
	ldrb	w2, [x0, 1586]
	strb	w2, [x0, 3233]
.L1759:
	ldrb	w2, [x0, 1587]
	ldr	w3, [x0, 1616]
	strb	w2, [x0, 3234]
	cmp	w2, 5
	str	w3, [x0, 3244]
	beq	.L1766
.L1760:
	ldr	w2, [x1, 2200]
	cbnz	w2, .L1761
	ldr	w2, [x0, 708]
	ldr	w3, [x0, 704]
	str	w3, [x0, 3256]
	str	w2, [x0, 3260]
	ldr	w2, [x1, 2200]
.L1761:
	cmp	w2, 1
	bne	.L1762
	ldr	w2, [x0, 752]
	ldr	w1, [x0, 756]
	str	w2, [x0, 3264]
	str	w1, [x0, 3268]
.L1762:
	add	x19, x19, 65536
	ldr	w0, [x19, 72]
	add	w0, w0, 1
	str	w0, [x19, 72]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1766:
	ldr	w2, [x0, 1624]
	str	w2, [x0, 3252]
	b	.L1760
	.size	ExitSlice, .-ExitSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PicTypeStatistic
	.type	PicTypeStatistic, %function
PicTypeStatistic:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 20480
	ldrb	w0, [x1, 1584]
	cmp	w0, 1
	beq	.L1769
	cbz	w0, .L1770
	cmp	w0, 2
	bne	.L1767
	ldrb	w0, [x1, 1587]
	add	x19, x19, 61440
	cmp	w0, 5
	beq	.L1777
.L1772:
	str	wzr, [x19, 800]
.L1767:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1770:
	add	x19, x19, 61440
	ldr	w0, [x19, 796]
	cmp	w0, 2
	beq	.L1772
	mov	w0, 1
	str	wzr, [x19, 800]
	str	w0, [x19, 796]
	b	.L1767
	.p2align 3
.L1769:
	add	x19, x19, 61440
	mov	w0, 2
	str	wzr, [x19, 800]
	str	w0, [x19, 796]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1777:
	mov	w0, 1
	str	w0, [x19, 800]
	b	.L1767
	.size	PicTypeStatistic, .-PicTypeStatistic
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CalcStreamBits
	.type	CalcStreamBits, %function
CalcStreamBits:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 16384
	ldrb	w0, [x0, 2592]
	cbz	w0, .L1779
	ldr	x1, [x19, 632]
	ldr	w0, [x1, 72]
	add	w0, w0, 7
	and	w0, w0, -8
	str	w0, [x1, 72]
	ldr	x1, [x19, 632]
	ldr	w0, [x1, 80]
	sub	w0, w0, #1
	str	w0, [x1, 80]
.L1779:
	ldr	x3, [x19, 632]
	ldrb	w21, [x3]
	cbz	w21, .L1790
	sub	w1, w21, #1
	mov	w4, 32
	mov	x2, 48
	add	x0, x3, 16
	mov	w20, 0
	umaddl	x2, w1, w4, x2
	add	x2, x3, x2
	.p2align 2
.L1781:
	ldr	w1, [x0], 32
	cmp	x0, x2
	add	w20, w20, w1
	bne	.L1781
.L1780:
	sbfiz	x4, x21, 5, 32
	ldr	w2, [x3, 72]
	add	x4, x3, x4
	ldr	w5, [x3, 80]
	ldr	x1, .LC321
	mov	w3, w20
	mov	w0, 7
	ldr	w4, [x4, 16]
	bl	dprint_vfmw
	ldr	x2, [x19, 632]
	ubfiz	x3, x21, 5, 8
	ldrb	w0, [x2]
	ldr	w1, [x2, 72]
	add	x0, x2, x0, lsl 5
	and	w1, w1, 7
	str	w1, [x0, 28]
	ldr	x2, [x19, 632]
	ldrb	w1, [x2]
	ldr	w0, [x2, 72]
	add	x1, x2, x1, lsl 5
	ldr	w2, [x1, 20]
	add	w0, w2, w0, lsr 3
	mov	w2, w21
	sub	w0, w0, w20
	str	w0, [x1, 32]
	ldr	x4, [x19, 632]
	ldrb	w1, [x4]
	ldr	w0, [x4, 72]
	add	x4, x4, x1, lsl 5
	ldr	w1, [x4, 16]
	add	w20, w20, w1
	lsl	w20, w20, 3
	sub	w0, w20, w0
	str	w0, [x4, 24]
	ldr	x1, [x19, 632]
	ldrb	w0, [x1]
	add	x0, x1, x0, lsl 5
	ldp	w4, w1, [x0, 28]
	ubfiz	w1, w1, 3, 2
	add	w1, w1, w4
	str	w1, [x0, 28]
	ldr	x1, .LC323
	ldr	x4, [x19, 632]
	ldrb	w0, [x4]
	add	x4, x4, x0, lsl 5
	mov	w0, 7
	ldr	w5, [x4, 32]
	and	w5, w5, -4
	str	w5, [x4, 32]
	ldr	x6, [x19, 632]
	add	x3, x6, x3
	ldrb	w5, [x6]
	ldp	w4, w3, [x3, 24]
	add	x5, x6, x5, lsl 5
	ldr	w5, [x5, 32]
	bl	dprint_vfmw
	ldr	x0, [x19, 632]
	ldr	w2, [x0, 76]
	sub	w1, w2, #1
	cmp	w1, 1
	bhi	.L1801
	ldrb	w20, [x0]
	add	w3, w20, 1
	cmp	w3, 1
	ccmp	w2, w3, 0, ls
	bls	.L1784
	ldr	x21, .LC327
	add	w20, w20, 2
	.p2align 2
.L1785:
	sub	w2, w20, #1
	mov	x1, x21
	ubfiz	x3, x2, 5, 32
	add	x4, x0, x3
	mov	w0, 7
	str	wzr, [x4, 28]
	ldr	x4, [x19, 632]
	add	x4, x4, x3
	ldr	w5, [x4, 20]
	str	w5, [x4, 32]
	ldr	x4, [x19, 632]
	add	x4, x4, x3
	ldr	w5, [x4, 16]
	lsl	w5, w5, 3
	str	w5, [x4, 24]
	ldr	x4, [x19, 632]
	add	x4, x4, x3
	ldp	w6, w5, [x4, 28]
	ubfiz	w5, w5, 3, 2
	add	w5, w5, w6
	str	w5, [x4, 28]
	ldr	x4, [x19, 632]
	add	x4, x4, x3
	ldr	w5, [x4, 32]
	and	w5, w5, -4
	str	w5, [x4, 32]
	ldr	x4, [x19, 632]
	add	x3, x4, x3
	ldp	w4, w3, [x3, 24]
	bl	dprint_vfmw
	ldr	x0, [x19, 632]
	cmp	w20, 1
	cset	w3, ls
	ldr	w1, [x0, 76]
	cmp	w1, w20
	add	w20, w20, 1
	cset	w2, hi
	tst	w3, w2
	bne	.L1785
	sub	w1, w1, #1
.L1784:
	ubfiz	x1, x1, 5, 32
	ldr	w2, [x0, 80]
	add	x0, x0, x1
	add	x5, x19, 61440
	ldr	w1, [x0, 24]
	sub	w1, w1, w2
	str	w1, [x0, 24]
	ldr	x3, [x19, 632]
	ldrb	w0, [x3]
	ldr	w4, [x3, 76]
	cmp	w0, w4
	bcs	.L1778
	.p2align 2
.L1795:
	ubfiz	x1, x0, 5, 32
	ldr	w2, [x5, 824]
	add	x1, x3, x1
	add	w0, w0, 1
	ldr	w1, [x1, 32]
	cmp	w1, w2
	bcs	.L1787
	str	w1, [x5, 824]
	ldr	w4, [x3, 76]
.L1787:
	cmp	w4, w0
	bhi	.L1795
.L1778:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1801:
	ldr	x1, .LC325
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L1790:
	mov	w20, 0
	b	.L1780
	.size	CalcStreamBits, .-CalcStreamBits
	.align	3
.LC321:
	.xword	.LC320
	.align	3
.LC323:
	.xword	.LC322
	.align	3
.LC325:
	.xword	.LC324
	.align	3
.LC327:
	.xword	.LC326
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Scaling_List
	.type	Scaling_List, %function
Scaling_List:
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
	ble	.L1802
	sub	w26, w27, #1
	mov	w0, 8
	ldr	x21, .LC330
	add	x26, x26, 1
	mov	x20, 0
	mov	w24, w0
	mov	w23, 255
	b	.L1807
	.p2align 3
.L1806:
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
	beq	.L1802
.L1807:
	cmp	w27, 16
	add	x1, x21, 16
	beq	.L1815
	ldrb	w19, [x20, x1]
.L1805:
	add	x20, x20, 1
	cbz	w0, .L1806
	ldr	x1, .LC332
	ldr	x0, [x29, 104]
	bl	se_v
	add	w0, w0, w24
	and	w0, w0, 255
	cmp	w19, wzr
	ccmp	w0, wzr, 0, eq
	cset	w1, eq
	cmp	w0, wzr
	csel	w24, w24, w0, eq
	str	w1, [x22]
	b	.L1806
	.p2align 3
.L1815:
	ldrb	w19, [x21, x20]
	b	.L1805
	.p2align 3
.L1802:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.size	Scaling_List, .-Scaling_List
	.align	3
.LC330:
	.xword	.LANCHOR1
	.align	3
.LC332:
	.xword	.LC331
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ProcessPPS
	.type	ProcessPPS, %function
ProcessPPS:
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
	add	x21, x19, 65536
	bl	_mcount
	ldr	x1, .LC336
	mov	x0, x19
	bl	u_1
	strb	w0, [x20]
	ldr	w0, [x21, 56]
	cbnz	w0, .L1867
	ldr	x1, .LC338
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 1]
	ldr	w0, [x21, 56]
	cbnz	w0, .L1867
	ldr	x1, .LC340
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 32]
	cbnz	w0, .L1820
	ldr	w0, [x21, 56]
	cbz	w0, .L1821
.L1820:
	ldr	x1, .LC342
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC343
	ldr	x4, [x0]
	cbz	x4, .L1867
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, -1
.L1818:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1821:
	ldr	x1, .LC345
	mov	x0, x19
	bl	ue_v
	cmp	w0, 31
	str	w0, [x20, 36]
	bhi	.L1822
	ldr	w0, [x21, 56]
	cbnz	w0, .L1822
	ldr	x1, .LC349
	mov	x0, x19
	bl	ue_v
	cmp	w0, 31
	str	w0, [x20, 40]
	bhi	.L1824
	ldr	w0, [x21, 56]
	cbnz	w0, .L1824
	ldr	x1, .LC353
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 2]
	ldr	w0, [x21, 56]
	cbnz	w0, .L1867
	ldr	x2, .LC355
	mov	w1, 2
	mov	x0, x19
	bl	u_v
	cmp	w0, 2
	str	w0, [x20, 44]
	bhi	.L1868
	ldr	x1, .LC359
	mov	x0, x19
	bl	se_v
	str	w0, [x20, 48]
	ldr	x1, .LC361
	mov	x0, x19
	bl	se_v
	ldr	x1, .LC363
	mov	x0, x19
	bl	se_v
	str	w0, [x20, 52]
	ldr	x1, .LC365
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 3]
	ldr	x1, .LC367
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 4]
	ldr	x1, .LC369
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 17]
	cbnz	w0, .L1869
.L1827:
	ldr	w1, [x20, 52]
	add	x0, x19, 12288
	stp	w1, wzr, [x20, 56]
	strb	wzr, [x20, 18]
	add	x0, x0, 2600
	bl	BsResidBits
	ldr	x1, [x19, 632]
	ldr	w1, [x1, 80]
	add	w1, w1, 3
	cmp	w0, w1
	bcc	.L1848
	ldr	x1, .LC373
	mov	x0, x19
	bl	u_1
	str	w0, [x20, 60]
	ldr	x1, .LC375
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 18]
	beq	.L1829
.L1847:
	ldr	x1, .LC377
	mov	x0, x19
	bl	se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x20, 56]
	bhi	.L1830
	ldr	w0, [x21, 56]
	cbz	w0, .L1848
.L1830:
	ldr	x1, .LC384
	mov	w0, 1
	bl	dprint_vfmw
.L1867:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, -1
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1848:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1869:
	ldr	x1, .LC371
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1827
	.p2align 3
.L1822:
	ldr	x1, .LC347
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1818
	.p2align 3
.L1824:
	ldr	x1, .LC351
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1818
	.p2align 3
.L1829:
	ldr	x28, .LC380
	add	x22, x29, 120
	ldr	x26, .LC379
	add	x24, x20, 5
	add	x23, x20, 1984
	add	x25, x20, 64
	mov	w27, 0
	b	.L1838
	.p2align 3
.L1874:
	ldr	w0, [x20, 28]
	mov	w1, 4000
	umull	x0, w0, w1
	ldr	x1, [x19, 14960]
	add	x0, x1, x0
	ldrb	w1, [x0, 25]
	cmp	w1, 1
	beq	.L1870
	ldr	x0, .LC380
	ldr	x3, [x0, 104]
	ldr	x0, .LC334
	add	x1, x0, 96
	cbz	w27, .L1871
.L1866:
	mov	x2, 16
	mov	x0, x23
	blr	x3
.L1833:
	add	w27, w27, 1
	add	x22, x22, 4
	cmp	w27, 6
	add	x24, x24, 1
	add	x23, x23, 16
	add	x25, x25, 64
	beq	.L1872
.L1838:
	mov	x1, x26
	mov	x0, x19
	bl	u_1
	cmp	w0, 1
	strb	w0, [x24]
	str	w0, [x22]
	beq	.L1873
	cmp	w27, wzr
	ccmp	w27, 3, 4, ne
	beq	.L1874
	sub	w1, w27, #1
	ldr	x3, [x28, 104]
	add	x1, x1, 124
	add	x1, x20, x1, lsl 4
	b	.L1866
	.p2align 3
.L1872:
	ldr	x22, .LC380
	add	x26, x20, 2080
	ldr	x24, .LC334
	add	x27, x20, 448
	mov	x28, 0
	add	x23, x29, 112
	mov	w25, 4000
.L1846:
	ldr	w0, [x20, 60]
	mov	w8, w28
	cmp	w0, 1
	beq	.L1875
	add	x0, x20, x28
	str	wzr, [x23, x28, lsl 2]
	strb	wzr, [x0, 11]
.L1841:
	ldr	w0, [x20, 28]
	ldr	x1, [x19, 14960]
	umull	x0, w0, w25
	add	x0, x1, x0
	ldrb	w1, [x0, 25]
	cmp	w1, 1
	beq	.L1876
	add	x1, x24, 176
	ldr	x3, [x22, 104]
	cbz	w8, .L1877
.L1845:
	mov	x2, 64
	mov	x0, x26
	blr	x3
.L1842:
	add	x28, x28, 1
	add	x26, x26, 64
	cmp	x28, 2
	add	x27, x27, 256
	bne	.L1846
	b	.L1847
	.p2align 3
.L1873:
	mov	x0, x19
	mov	x4, x22
	mov	w3, 16
	mov	x2, x23
	mov	x1, x25
	bl	Scaling_List
	ldr	w0, [x22]
	cmp	w0, 1
	bne	.L1833
	ldr	x0, .LC334
	cmp	w27, 2
	add	x1, x0, 80
	add	x0, x0, 96
	csel	x1, x0, x1, hi
.L1865:
	ldr	x0, .LC380
	mov	x2, 16
	ldr	x3, [x0, 104]
	mov	x0, x23
	blr	x3
	b	.L1833
.L1870:
	uxtw	x1, w27
	add	x1, x0, x1, lsl 4
	add	x1, x1, 1972
	b	.L1865
.L1868:
	ldr	x1, .LC357
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1818
.L1875:
	ldr	x1, .LC382
	mov	x0, x19
	str	w28, [x29, 108]
	bl	u_1
	cmp	w0, 1
	add	x1, x20, x28
	str	w0, [x23, x28, lsl 2]
	ldr	w8, [x29, 108]
	strb	w0, [x1, 11]
	bne	.L1841
	mov	x0, x19
	add	x4, x23, x28, lsl 2
	mov	w3, 64
	mov	x2, x26
	mov	x1, x27
	str	w8, [x29, 108]
	bl	Scaling_List
	ldr	w0, [x23, x28, lsl 2]
	ldr	w8, [x29, 108]
	cmp	w0, 1
	bne	.L1842
	ldr	x0, .LC380
	ldr	x3, [x0, 104]
	ldr	x0, .LC334
	add	x1, x0, 176
	cbnz	w8, .L1845
	add	x1, x0, 112
	b	.L1845
	.p2align 3
.L1876:
	ubfiz	x8, x8, 6, 32
	mov	x2, 64
	add	x1, x0, x8
	ldr	x0, .LC380
	add	x1, x1, 2068
	ldr	x3, [x0, 104]
	mov	x0, x26
	blr	x3
	b	.L1842
.L1871:
	add	x1, x0, 80
	b	.L1866
.L1877:
	ldr	x1, .LC334
	add	x1, x1, 112
	b	.L1845
	.size	ProcessPPS, .-ProcessPPS
	.align	3
.LC334:
	.xword	.LANCHOR0
	.align	3
.LC336:
	.xword	.LC335
	.align	3
.LC338:
	.xword	.LC337
	.align	3
.LC340:
	.xword	.LC339
	.align	3
.LC342:
	.xword	.LC341
	.align	3
.LC343:
	.xword	g_event_report
	.align	3
.LC345:
	.xword	.LC344
	.align	3
.LC347:
	.xword	.LC346
	.align	3
.LC349:
	.xword	.LC348
	.align	3
.LC351:
	.xword	.LC350
	.align	3
.LC353:
	.xword	.LC352
	.align	3
.LC355:
	.xword	.LC354
	.align	3
.LC357:
	.xword	.LC356
	.align	3
.LC359:
	.xword	.LC358
	.align	3
.LC361:
	.xword	.LC360
	.align	3
.LC363:
	.xword	.LC362
	.align	3
.LC365:
	.xword	.LC364
	.align	3
.LC367:
	.xword	.LC366
	.align	3
.LC369:
	.xword	.LC368
	.align	3
.LC371:
	.xword	.LC370
	.align	3
.LC373:
	.xword	.LC372
	.align	3
.LC375:
	.xword	.LC374
	.align	3
.LC377:
	.xword	.LC376
	.align	3
.LC379:
	.xword	.LC378
	.align	3
.LC380:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC382:
	.xword	.LC381
	.align	3
.LC384:
	.xword	.LC383
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PPSEqual
	.type	PPSEqual, %function
PPSEqual:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cbz	x19, .L1900
	cbz	x20, .L1901
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
	ldr	w10, [x19, 60]
	cmp	w5, w4
	ldrb	w5, [x20, 2]
	ccmp	w3, w0, 0, eq
	ldr	w4, [x19, 44]
	cset	w3, eq
	cmp	w7, w5
	ldp	w0, w5, [x20, 44]
	ldr	w7, [x19, 52]
	and	w1, w1, w2
	ccmp	w4, w0, 0, eq
	ldr	w4, [x20, 52]
	cset	w0, eq
	cmp	w6, w5
	ldrb	w5, [x20, 3]
	and	w1, w3, w1
	ldrb	w6, [x19, 3]
	ccmp	w7, w4, 0, eq
	cset	w4, eq
	ldrb	w2, [x20, 4]
	and	w0, w0, w1
	cmp	w6, w5
	ldrb	w5, [x19, 4]
	and	w0, w4, w0
	ldr	w4, [x20, 60]
	ccmp	w5, w2, 0, eq
	ldrb	w3, [x19, 18]
	ldrb	w1, [x20, 18]
	cset	w2, eq
	cmp	w4, w10
	and	w0, w2, w0
	ccmp	w1, w3, 0, eq
	cset	w2, eq
	and	w2, w2, w0
	cbz	w3, .L1882
	lsl	w10, w10, 1
	cmn	w10, #6
	beq	.L1882
	mov	x1, x20
	add	x7, x19, 64
	mov	w9, -6
	mov	x8, 0
	.p2align 2
.L1887:
	add	x3, x19, x8
	add	x0, x20, x8
	ldrsb	w3, [x3, 5]
	ldrsb	w0, [x0, 5]
	cmp	w0, w3
	cset	w0, eq
	and	w2, w2, w0
	cbz	w3, .L1883
	cmp	w8, 5
	bhi	.L1884
	sub	x0, x7, #64
	mov	x3, x1
	.p2align 2
.L1885:
	ldr	w5, [x0, 64]
	add	x0, x0, 4
	ldr	w4, [x3, 64]
	add	x3, x3, 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x7, x0
	and	w2, w2, w4
	bne	.L1885
.L1883:
	add	w9, w9, 1
	add	x8, x8, 1
	cmp	w9, w10
	add	x1, x1, 64
	add	x7, x7, 64
	bne	.L1887
.L1882:
	ldr	w0, [x20, 56]
	ldr	w1, [x19, 56]
	cmp	w1, w0
	cset	w0, eq
	tst	w0, w2
	csetm	w0, eq
.L1880:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1884:
	ubfiz	x0, x9, 8, 32
	add	x3, x0, 448
	add	x6, x0, 704
	add	x6, x19, x6
	add	x0, x19, x3
	add	x3, x20, x3
	.p2align 2
.L1886:
	ldr	w5, [x0], 4
	ldr	w4, [x3], 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x6, x0
	and	w2, w2, w4
	bne	.L1886
	add	w9, w9, 1
	add	x8, x8, 1
	cmp	w9, w10
	add	x1, x1, 64
	add	x7, x7, 64
	bne	.L1887
	b	.L1882
.L1901:
	ldr	x2, .LC385
	mov	w3, 9727
	ldr	x1, .LC386
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1880
.L1900:
	ldr	x2, .LC385
	mov	w3, 9726
	ldr	x1, .LC386
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1880
	.size	PPSEqual, .-PPSEqual
	.align	3
.LC385:
	.xword	.LC50
	.align	3
.LC386:
	.xword	.LC52
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_DecPPS
	.type	H264_DecPPS, %function
H264_DecPPS:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x1, .LC388
	mov	x0, x19
	bl	ue_v
	mov	w20, w0
	ldr	w0, [x19, 14948]
	sub	w0, w0, #1
	cmp	w20, w0
	bls	.L1903
	ldr	x1, .LC390
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 14948]
	sub	w1, w0, #1
	cmp	w20, w1
	bls	.L1911
	ldr	x1, .LC391
	ldr	x4, [x1]
	cbz	x4, .L1911
	add	x19, x19, 65536
	mov	w3, 8
	stp	w20, w0, [x29, 56]
	add	x2, x29, 56
	mov	w1, 110
	ldr	w0, [x19, 116]
	blr	x4
	mov	w0, -1
.L1904:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1903:
	ldr	x1, .LC393
	mov	x0, x19
	bl	ue_v
	mov	w2, w0
	ldr	w0, [x19, 14944]
	sub	w0, w0, #1
	cmp	w2, w0
	bhi	.L1915
	mov	w0, 4000
	ldr	x1, [x19, 14960]
	umull	x0, w2, w0
	add	x0, x1, x0
	ldrb	w0, [x0, 23]
	cbz	w0, .L1916
	mov	w21, 2208
	ldr	x0, [x19, 14968]
	umull	x21, w20, w21
	add	x0, x0, x21
	ldrb	w1, [x0, 19]
	cbz	w1, .L1907
	ldr	x22, .LC398
	mov	x0, x19
	mov	x1, x22
	stp	w20, w2, [x22, 24]
	bl	ProcessPPS
	cbnz	w0, .L1917
	ldr	x1, [x19, 14968]
	mov	x0, x22
	add	x1, x1, x21
	bl	PPSEqual
	cbz	w0, .L1904
	ldr	x3, .LC401
	mov	w1, 1
	ldr	x0, [x19, 14968]
	mov	x2, 2208
	strb	w1, [x22, 20]
	ldr	x3, [x3, 112]
	add	x0, x0, x21
	strb	w1, [x22, 19]
	mov	x1, x22
	blr	x3
	mov	w0, 0
	b	.L1904
	.p2align 3
.L1907:
	str	w20, [x0, 24]
	mov	x0, x19
	ldr	x1, [x19, 14968]
	add	x1, x1, x21
	str	w2, [x1, 28]
	ldr	x1, [x19, 14968]
	add	x1, x1, x21
	bl	ProcessPPS
	cbnz	w0, .L1918
	ldr	x1, [x19, 14968]
	mov	w2, 1
	add	x1, x1, x21
	strb	w2, [x1, 20]
	ldr	x1, [x19, 14968]
	add	x21, x1, x21
	strb	w2, [x21, 19]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1911:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, -1
	ldp	x29, x30, [sp], 64
	ret
.L1915:
	ldr	x1, .LC395
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1904
.L1917:
	ldr	x1, .LC400
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1904
.L1916:
	ldr	x1, .LC397
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1904
.L1918:
	ldr	x1, .LC400
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 14968]
	mov	w2, 1
	mov	w0, -1
	add	x1, x1, x21
	strb	w2, [x1, 20]
	ldr	x1, [x19, 14968]
	add	x21, x1, x21
	strb	wzr, [x21, 19]
	b	.L1904
	.size	H264_DecPPS, .-H264_DecPPS
	.align	3
.LC388:
	.xword	.LC387
	.align	3
.LC390:
	.xword	.LC389
	.align	3
.LC391:
	.xword	g_event_report
	.align	3
.LC393:
	.xword	.LC392
	.align	3
.LC395:
	.xword	.LC394
	.align	3
.LC397:
	.xword	.LC396
	.align	3
.LC398:
	.xword	.LANCHOR2
	.align	3
.LC400:
	.xword	.LC399
	.align	3
.LC401:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	vui_parameters
	.type	vui_parameters, %function
vui_parameters:
	stp	x29, x30, [sp, -96]!
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
	ldr	x1, .LC403
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20]
	cbnz	w0, .L1962
	strb	wzr, [x20, 15]
.L1921:
	ldr	x1, .LC411
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 1]
	cbnz	w0, .L1963
.L1922:
	ldr	x1, .LC415
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 3]
	cbnz	w0, .L1964
	mov	w0, 5
	str	w0, [x20, 28]
.L1924:
	ldr	x1, .LC429
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 6]
	cbnz	w0, .L1965
.L1925:
	ldr	x1, .LC435
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 8]
	cbnz	w0, .L1966
.L1927:
	ldr	x1, .LC443
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 7]
	cbz	w0, .L1929
	ldr	x1, .LC445
	mov	x0, x19
	add	x24, x20, 84
	bl	ue_v
	ldr	x2, .LC447
	mov	w1, 4
	str	w0, [x24, 36]
	mov	x0, x19
	bl	u_v
	strb	w0, [x24, 32]
	ldr	x2, .LC449
	mov	w1, 4
	mov	x0, x19
	bl	u_v
	strb	w0, [x24, 33]
	ldr	w1, [x24, 36]
	cmp	w1, 31
	bhi	.L1932
	add	x28, x19, 65536
	ldr	w0, [x28, 56]
	cbnz	w0, .L1932
	ldr	x27, .LC453
	mov	w21, 0
	ldr	x26, .LC455
	ldr	x25, .LC457
	.p2align 2
.L1933:
	uxtw	x22, w21
	mov	x1, x27
	mov	x0, x19
	add	w21, w21, 1
	add	x23, x20, x22, lsl 2
	bl	ue_v
	mov	x1, x26
	str	w0, [x23, 124]
	mov	x0, x19
	bl	ue_v
	str	w0, [x23, 252]
	mov	x1, x25
	mov	x0, x19
	bl	u_1
	strb	w0, [x24, x22]
	ldr	w0, [x24, 36]
	cmp	w0, w21
	bcs	.L1933
	ldr	x2, .LC459
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 296]
	ldr	x2, .LC461
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 300]
	ldr	x2, .LC463
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 304]
	ldr	x2, .LC465
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 308]
.L1929:
	ldr	x1, .LC467
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 10]
	cbnz	w0, .L1967
	ldrb	w0, [x20, 7]
	add	x28, x19, 65536
	cbz	w0, .L1937
.L1938:
	ldr	x1, .LC469
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 14]
.L1937:
	ldr	x1, .LC471
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 11]
	ldr	x1, .LC473
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 12]
	cbnz	w0, .L1939
	ldr	w0, [x28, 56]
.L1940:
	cmp	w0, wzr
	csetm	w0, ne
.L1961:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1966:
	ldr	x2, .LC437
	mov	w1, 32
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 52]
	ldr	x2, .LC439
	mov	w1, 32
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 56]
	ldr	x1, .LC441
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x20, 9]
	bne	.L1927
	ldr	w1, [x20, 52]
	cbz	w1, .L1927
	ldr	w0, [x20, 56]
	add	x2, x19, 61440
	udiv	w0, w0, w1
	lsr	w0, w0, 1
	str	w0, [x2, 2796]
	b	.L1927
	.p2align 3
.L1965:
	ldr	x1, .LC431
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 44]
	ldr	x1, .LC433
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 48]
	b	.L1925
	.p2align 3
.L1964:
	ldr	x2, .LC417
	mov	w1, 3
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 28]
	ldr	x1, .LC419
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 4]
	ldr	x1, .LC421
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 5]
	cbz	w0, .L1924
	ldr	x2, .LC423
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 32]
	ldr	x2, .LC425
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 36]
	ldr	x2, .LC427
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 40]
	b	.L1924
	.p2align 3
.L1963:
	ldr	x1, .LC413
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 2]
	b	.L1922
	.p2align 3
.L1962:
	ldr	x2, .LC405
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	uxtb	w0, w0
	cmp	w0, 255
	strb	w0, [x20, 15]
	bne	.L1921
	ldr	x2, .LC407
	mov	w1, 16
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 16]
	ldr	x2, .LC409
	mov	w1, 16
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 20]
	b	.L1921
	.p2align 3
.L1967:
	ldr	x1, .LC445
	mov	x0, x19
	add	x24, x20, 396
	bl	ue_v
	ldr	x2, .LC447
	mov	w1, 4
	str	w0, [x24, 36]
	mov	x0, x19
	bl	u_v
	strb	w0, [x24, 32]
	ldr	x2, .LC449
	mov	w1, 4
	mov	x0, x19
	bl	u_v
	strb	w0, [x24, 33]
	ldr	w1, [x24, 36]
	cmp	w1, 31
	bhi	.L1932
	add	x28, x19, 65536
	ldr	w0, [x28, 56]
	cbnz	w0, .L1932
	ldr	x27, .LC453
	mov	w21, 0
	ldr	x26, .LC455
	ldr	x25, .LC457
	.p2align 2
.L1935:
	uxtw	x22, w21
	mov	x1, x27
	mov	x0, x19
	add	w21, w21, 1
	add	x23, x20, x22, lsl 2
	bl	ue_v
	mov	x1, x26
	str	w0, [x23, 436]
	mov	x0, x19
	bl	ue_v
	str	w0, [x23, 564]
	mov	x1, x25
	mov	x0, x19
	bl	u_1
	strb	w0, [x24, x22]
	ldr	w0, [x24, 36]
	cmp	w0, w21
	bcs	.L1935
	ldr	x2, .LC459
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 296]
	ldr	x2, .LC461
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 300]
	ldr	x2, .LC463
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 304]
	ldr	x2, .LC465
	mov	w1, 5
	mov	x0, x19
	bl	u_v
	str	w0, [x24, 308]
	ldrb	w0, [x20, 7]
	cbnz	w0, .L1938
	ldrb	w0, [x20, 10]
	cbnz	w0, .L1938
	b	.L1937
	.p2align 3
.L1939:
	ldr	x1, .LC475
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 13]
	ldr	x1, .LC477
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 60]
	ldr	x1, .LC479
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 64]
	ldr	x1, .LC481
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 72]
	ldr	x1, .LC483
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 68]
	ldr	x1, .LC485
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 76]
	ldr	x1, .LC487
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 80]
	ldr	w0, [x28, 56]
	cbz	w0, .L1940
	mov	w0, -1
	b	.L1961
.L1932:
	ldr	x1, .LC451
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1961
	.size	vui_parameters, .-vui_parameters
	.align	3
.LC403:
	.xword	.LC402
	.align	3
.LC405:
	.xword	.LC404
	.align	3
.LC407:
	.xword	.LC406
	.align	3
.LC409:
	.xword	.LC408
	.align	3
.LC411:
	.xword	.LC410
	.align	3
.LC413:
	.xword	.LC412
	.align	3
.LC415:
	.xword	.LC414
	.align	3
.LC417:
	.xword	.LC416
	.align	3
.LC419:
	.xword	.LC418
	.align	3
.LC421:
	.xword	.LC420
	.align	3
.LC423:
	.xword	.LC422
	.align	3
.LC425:
	.xword	.LC424
	.align	3
.LC427:
	.xword	.LC426
	.align	3
.LC429:
	.xword	.LC428
	.align	3
.LC431:
	.xword	.LC430
	.align	3
.LC433:
	.xword	.LC432
	.align	3
.LC435:
	.xword	.LC434
	.align	3
.LC437:
	.xword	.LC436
	.align	3
.LC439:
	.xword	.LC438
	.align	3
.LC441:
	.xword	.LC440
	.align	3
.LC443:
	.xword	.LC442
	.align	3
.LC445:
	.xword	.LC444
	.align	3
.LC447:
	.xword	.LC446
	.align	3
.LC449:
	.xword	.LC448
	.align	3
.LC451:
	.xword	.LC450
	.align	3
.LC453:
	.xword	.LC452
	.align	3
.LC455:
	.xword	.LC454
	.align	3
.LC457:
	.xword	.LC456
	.align	3
.LC459:
	.xword	.LC458
	.align	3
.LC461:
	.xword	.LC460
	.align	3
.LC463:
	.xword	.LC462
	.align	3
.LC465:
	.xword	.LC464
	.align	3
.LC467:
	.xword	.LC466
	.align	3
.LC469:
	.xword	.LC468
	.align	3
.LC471:
	.xword	.LC470
	.align	3
.LC473:
	.xword	.LC472
	.align	3
.LC475:
	.xword	.LC474
	.align	3
.LC477:
	.xword	.LC476
	.align	3
.LC479:
	.xword	.LC478
	.align	3
.LC481:
	.xword	.LC480
	.align	3
.LC483:
	.xword	.LC482
	.align	3
.LC485:
	.xword	.LC484
	.align	3
.LC487:
	.xword	.LC486
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	SPSEqual
	.type	SPSEqual, %function
SPSEqual:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	x20, x1
	bl	_mcount
	cbz	x19, .L2000
	cbz	x20, .L2001
	ldp	w3, w6, [x19, 28]
	ldp	w2, w5, [x20, 28]
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
	ldr	w2, [x20, 36]
	ccmp	w1, w0, 0, eq
	ldr	w4, [x19, 36]
	cset	w0, eq
	ldrb	w1, [x19, 25]
	and	w0, w3, w0
	cmp	w6, w5
	ldrb	w3, [x20, 25]
	ccmp	w4, w2, 0, eq
	cset	w2, eq
	cmp	w3, w1
	and	w0, w2, w0
	cset	w2, eq
	and	w0, w2, w0
	cbz	w1, .L1972
	mov	x2, x19
	mov	x8, x20
	mov	w9, -6
	mov	x1, 0
	.p2align 2
.L1977:
	add	x4, x19, x1
	add	x3, x20, x1
	add	x6, x2, 64
	ldrsb	w3, [x3, 4]
	ldrsb	w4, [x4, 4]
	cmp	w3, w4
	cset	w3, eq
	and	w0, w0, w3
	cbz	w4, .L1973
	cmp	w1, 5
	bhi	.L1974
	mov	x3, x8
	.p2align 2
.L1975:
	ldr	w5, [x2, 52]
	add	x2, x2, 4
	ldr	w4, [x3, 52]
	add	x3, x3, 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x6, x2
	and	w0, w0, w4
	bne	.L1975
.L1973:
	add	x1, x1, 1
	add	w9, w9, 1
	cmp	x1, 8
	mov	x2, x6
	add	x8, x8, 64
	bne	.L1977
.L1972:
	ldr	w4, [x19, 2196]
	ldr	w3, [x20, 2196]
	ldr	w2, [x20, 2200]
	ldr	w1, [x19, 2200]
	cmp	w4, w3
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w0, w2, w0
	cbz	w1, .L1979
	cmp	w1, 1
	bne	.L1978
	ldrb	w4, [x19, 16]
	ldrb	w3, [x20, 16]
	ldr	w2, [x19, 2208]
	ldr	w1, [x20, 2208]
	cmp	w4, w3
	ldr	w5, [x19, 2212]
	ldr	w4, [x20, 2212]
	ccmp	w2, w1, 0, eq
	ldr	w2, [x20, 2216]
	cset	w3, eq
	ldr	w1, [x19, 2216]
	cmp	w5, w4
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w2, w3, w2
	and	w0, w0, w2
	cbz	w1, .L1978
	sub	w5, w1, #1
	add	x2, x20, 2220
	add	x5, x5, 556
	add	x1, x19, 2220
	add	x5, x19, x5, lsl 2
	.p2align 2
.L1981:
	ldr	w4, [x1], 4
	ldr	w3, [x2], 4
	cmp	w4, w3
	cset	w3, eq
	cmp	x1, x5
	and	w0, w0, w3
	bne	.L1981
.L1978:
	ldr	w4, [x19, 3244]
	ldr	w3, [x20, 3244]
	ldrb	w2, [x19, 17]
	ldrb	w1, [x20, 17]
	cmp	w4, w3
	ldr	w6, [x19, 3248]
	ldr	w4, [x20, 3248]
	ccmp	w2, w1, 0, eq
	ldr	w5, [x19, 3252]
	cset	w2, eq
	ldr	w1, [x20, 3252]
	cmp	w6, w4
	ldrb	w3, [x19, 18]
	ldrb	w4, [x20, 18]
	ccmp	w5, w1, 0, eq
	cset	w1, eq
	cmp	w4, w3
	and	w2, w2, w1
	cset	w1, eq
	and	w1, w2, w1
	and	w0, w1, w0
	cbnz	w3, .L1982
	ldrb	w1, [x20, 19]
	ldrb	w2, [x19, 19]
	cmp	w2, w1
	cset	w1, eq
	and	w0, w0, w1
.L1982:
	ldrb	w4, [x19, 20]
	ldrb	w3, [x20, 20]
	ldrb	w2, [x20, 21]
	ldrb	w1, [x19, 21]
	cmp	w4, w3
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w0, w2, w0
	cbnz	w1, .L2002
.L1983:
	ldrb	w1, [x20, 22]
	ldrb	w2, [x19, 22]
	cmp	w2, w1
	cset	w1, eq
	tst	w1, w0
	csetm	w0, eq
.L1970:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2002:
	ldr	w4, [x19, 3256]
	ldr	w3, [x20, 3256]
	ldr	w2, [x19, 3260]
	ldr	w1, [x20, 3260]
	cmp	w4, w3
	ldr	w5, [x19, 3264]
	ldr	w4, [x20, 3264]
	ccmp	w2, w1, 0, eq
	ldr	w3, [x19, 3268]
	cset	w2, eq
	ldr	w1, [x20, 3268]
	cmp	w5, w4
	ccmp	w3, w1, 0, eq
	cset	w1, eq
	and	w1, w2, w1
	and	w0, w0, w1
	b	.L1983
	.p2align 3
.L1974:
	ubfiz	x3, x9, 8, 32
	add	x4, x3, 436
	add	x7, x3, 692
	add	x7, x19, x7
	add	x3, x19, x4
	add	x4, x20, x4
	.p2align 2
.L1976:
	ldr	w6, [x3], 4
	ldr	w5, [x4], 4
	cmp	w6, w5
	cset	w5, eq
	cmp	x3, x7
	and	w0, w0, w5
	bne	.L1976
	add	x1, x1, 1
	add	x6, x2, 64
	cmp	x1, 8
	add	w9, w9, 1
	mov	x2, x6
	add	x8, x8, 64
	bne	.L1977
	b	.L1972
	.p2align 3
.L1979:
	ldr	w1, [x20, 2204]
	ldr	w2, [x19, 2204]
	cmp	w2, w1
	cset	w1, eq
	and	w0, w0, w1
	b	.L1978
.L2001:
	ldr	x2, .LC488
	mov	w3, 9960
	ldr	x1, .LC489
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1970
.L2000:
	ldr	x2, .LC488
	mov	w3, 9959
	ldr	x1, .LC489
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1970
	.size	SPSEqual, .-SPSEqual
	.align	3
.LC488:
	.xword	.LC50
	.align	3
.LC489:
	.xword	.LC52
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264GetDar
	.type	H264GetDar, %function
H264GetDar:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	w19, w0
	mov	x0, x30
	mov	w20, w1
	mov	w22, w2
	mov	w21, w3
	mov	w23, w4
	bl	_mcount
	cbz	w19, .L2014
	mov	w0, 255
	cmp	w19, 16
	ccmp	w19, w0, 4, gt
	bne	.L2014
	cmp	w20, wzr
	ccmp	w22, wzr, 4, ne
	ccmp	w19, w0, 0, eq
	beq	.L2014
	cmp	w19, 1
	beq	.L2015
	cmp	w19, w0
	beq	.L2005
	ldr	x0, .LC490
	sbfiz	x19, x19, 3, 32
	add	x0, x0, 80
	add	x1, x0, x19
	ldr	w20, [x0, x19]
	ldr	w22, [x1, 4]
.L2005:
	lsl	w20, w20, 10
	lsl	w21, w21, 10
	sdiv	w20, w20, w22
	sdiv	w2, w21, w23
	mul	w2, w20, w2
	cmp	w2, wzr
	add	w0, w2, 1023
	csel	w2, w0, w2, lt
	asr	w2, w2, 10
	subs	w1, w2, #2406
	bmi	.L2025
.L2006:
	subs	w0, w2, #2263
	bmi	.L2026
	cmp	w1, w0
	mov	w19, 4
	blt	.L2004
.L2010:
	subs	w1, w2, #1820
	bmi	.L2027
	cmp	w0, w1
	mov	w19, 3
	blt	.L2004
.L2011:
	subs	w0, w2, #1365
	bmi	.L2028
.L2009:
	mov	w19, 2
	cmp	w1, w0
	csinc	w19, w19, wzr, lt
	b	.L2004
	.p2align 3
.L2014:
	mov	w19, 0
	mov	w2, 0
.L2004:
	ldr	x1, .LC492
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
.L2015:
	mov	w19, 5
	mov	w2, 0
	b	.L2004
	.p2align 3
.L2026:
	mov	w0, 2263
	mov	w19, 4
	sub	w0, w0, w2
	cmp	w1, w0
	blt	.L2004
	b	.L2010
	.p2align 3
.L2025:
	mov	w1, 2406
	sub	w1, w1, w2
	b	.L2006
	.p2align 3
.L2027:
	mov	w1, 1820
	mov	w19, 3
	sub	w1, w1, w2
	cmp	w0, w1
	blt	.L2004
	b	.L2011
.L2028:
	mov	w0, 1365
	sub	w0, w0, w2
	b	.L2009
	.size	H264GetDar, .-H264GetDar
	.align	3
.LC490:
	.xword	.LANCHOR1
	.align	3
.LC492:
	.xword	.LC491
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ProcessSPS
	.type	ProcessSPS, %function
ProcessSPS:
	stp	x29, x30, [sp, -144]!
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
	cbz	x19, .L2149
	ldr	w0, [x19, 28]
	mov	w2, 144
	mov	w1, 110
	mov	w3, 86
	cmp	w0, 122
	ccmp	w0, w2, 4, ne
	cset	w2, eq
	cmp	w0, 100
	ccmp	w0, w1, 4, ne
	cset	w1, eq
	cmp	w0, 83
	ccmp	w0, w3, 4, ne
	orr	w0, w2, w1
	cset	w1, eq
	orr	w0, w1, w0
	cbnz	w0, .L2150
	mov	w0, 1
	strb	wzr, [x19, 25]
	str	w0, [x19, 40]
.L2044:
	ldr	x1, .LC519
	mov	x0, x20
	bl	ue_v
	cmp	w0, 12
	str	w0, [x19, 2196]
	bhi	.L2057
	add	x21, x20, 65536
	ldr	w0, [x21, 56]
	cbnz	w0, .L2057
	ldr	x1, .LC523
	mov	x0, x20
	bl	ue_v
	cmp	w0, 2
	str	w0, [x19, 2200]
	bhi	.L2059
	ldr	w1, [x21, 56]
	cbnz	w1, .L2059
	cbz	w0, .L2151
	cmp	w0, 1
	beq	.L2152
.L2063:
	ldr	x1, .LC580
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3244]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2147
	ldr	x1, .LC543
	mov	x0, x20
	bl	u_1
	strb	w0, [x19, 17]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2147
	ldr	x1, .LC545
	mov	x0, x20
	bl	ue_v
	sub	w1, w0, #1
	cmp	w1, 509
	str	w0, [x19, 3248]
	bhi	.L2070
	ldr	w1, [x21, 56]
	cbnz	w1, .L2070
	ldr	x1, .LC549
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3252]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2147
	ldr	x1, .LC551
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 18]
	ldr	w1, [x21, 56]
	cbnz	w1, .L2147
	cbnz	w0, .L2153
	ldr	x1, .LC582
	mov	x0, x20
	bl	u_1
	ldr	w23, [x19, 3252]
	strb	w0, [x19, 19]
	add	w23, w23, 1
	cmp	w23, 255
	bhi	.L2103
	ldr	w3, [x19, 3248]
	cmp	w23, 1
	mov	w24, 2
	add	w3, w3, 1
	ccmp	w3, 3, 0, hi
	cset	w0, ls
	cbz	w0, .L2074
.L2103:
	ldr	x1, .LC553
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2150:
	ldr	x1, .LC498
	mov	x0, x20
	bl	ue_v
	cmp	w0, 1
	str	w0, [x19, 40]
	bhi	.L2033
	add	x21, x20, 65536
	ldr	w0, [x21, 56]
	cbnz	w0, .L2033
	ldr	x1, .LC502
	mov	x0, x20
	bl	ue_v
	uxth	w0, w0
	add	w2, w0, 8
	str	w2, [x19, 44]
	cmp	w2, 8
	beq	.L2154
	ldr	x1, .LC504
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2036
.L2162:
	ldr	w1, [x19, 44]
	mov	w3, 8
	ldr	w0, [x21, 116]
	add	x2, x29, 120
	stp	wzr, w1, [x29, 120]
	mov	w1, 119
	blr	x4
.L2036:
	ldr	x1, .LC507
	mov	x0, x20
	bl	ue_v
	uxth	w0, w0
	add	w2, w0, 8
	str	w2, [x19, 48]
	cmp	w2, 8
	beq	.L2155
	ldr	x1, .LC509
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2039
.L2161:
	ldr	w1, [x19, 48]
	mov	w3, 8
	ldr	w0, [x21, 116]
	add	x2, x29, 120
	stp	wzr, w1, [x29, 120]
	mov	w1, 119
	blr	x4
.L2039:
	ldr	x1, .LC511
	mov	x0, x20
	bl	u_1
	cbnz	w0, .L2041
	ldr	w0, [x21, 56]
	cbnz	w0, .L2041
	ldr	x1, .LC515
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 25]
	bne	.L2044
	ldr	x26, .LC494
	add	x21, x29, 120
	ldr	x25, .LC516
	add	x23, x19, 4
	add	x22, x19, 1972
	add	x24, x19, 52
	mov	w27, 0
	ldr	x28, .LC517
	b	.L2050
	.p2align 3
.L2158:
	add	x0, x26, 96
	cmp	w27, wzr
	ldr	x3, [x28, 104]
	csel	x1, x0, x1, ne
	mov	x2, 16
	mov	x0, x22
	blr	x3
.L2046:
	add	w27, w27, 1
	add	x21, x21, 4
	cmp	w27, 6
	add	x23, x23, 1
	add	x22, x22, 16
	add	x24, x24, 64
	beq	.L2156
.L2050:
	mov	x1, x25
	mov	x0, x20
	bl	u_1
	strb	w0, [x23]
	ldr	x1, .LC494
	cmp	w0, 1
	str	w0, [x21]
	add	x1, x1, 80
	beq	.L2157
	cmp	w27, wzr
	ccmp	w27, 3, 4, ne
	beq	.L2158
	ldr	x0, .LC517
	sub	w1, w27, #1
	mov	x2, 16
	add	w27, w27, 1
	add	x1, x19, x1, lsl 4
	add	x21, x21, 4
	ldr	x3, [x0, 104]
	add	x1, x1, 1972
	mov	x0, x22
	add	x23, x23, 1
	add	x22, x22, 16
	add	x24, x24, 64
	blr	x3
	cmp	w27, 6
	bne	.L2050
.L2156:
	ldr	x24, .LC517
	add	x21, x19, 2068
	ldr	x23, .LC494
	add	x22, x19, 436
	mov	x27, 0
	add	x26, x29, 104
	mov	x28, x24
.L2055:
	mov	x1, x25
	mov	x0, x20
	bl	u_1
	cmp	w0, 1
	add	x2, x19, x27
	str	w0, [x26, x27, lsl 2]
	add	x1, x23, 176
	strb	w0, [x2, 10]
	beq	.L2159
	ldr	x3, [x24, 104]
	cbz	w27, .L2160
.L2054:
	mov	x0, x21
	mov	x2, 64
.L2143:
	blr	x3
.L2052:
	add	x27, x27, 1
	add	x21, x21, 64
	cmp	x27, 2
	add	x22, x22, 256
	bne	.L2055
	b	.L2044
	.p2align 3
.L2155:
	ldr	w0, [x21, 56]
	cbz	w0, .L2039
	ldr	x1, .LC509
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbnz	x4, .L2161
	b	.L2039
	.p2align 3
.L2154:
	ldr	w0, [x21, 56]
	cbz	w0, .L2036
	ldr	x1, .LC504
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbnz	x4, .L2162
	b	.L2036
	.p2align 3
.L2101:
	ldr	w2, [x0, 668]
	ldr	w1, [x0, 672]
	cmp	w25, w2
	ccmp	w23, w1, 2, ls
	bls	.L2100
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2147
	ldr	w0, [x21, 116]
	mov	w3, 8
	strh	w2, [x29, 116]
	add	x2, x29, 112
	strh	w1, [x29, 118]
	mov	w1, 107
	strh	w25, [x29, 112]
	strh	w23, [x29, 114]
	blr	x4
	.p2align 2
.L2147:
	mov	w0, -1
.L2139:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2153:
	ldr	w23, [x19, 3252]
	strb	wzr, [x19, 19]
	sub	w0, w23, #1
	cmp	w0, 509
	bhi	.L2103
	ldr	w3, [x19, 3248]
	mov	w24, 1
	add	w23, w23, 1
	add	w3, w3, w24
.L2074:
	mul	w23, w24, w23
	lsl	w25, w3, 4
	mov	w1, 100663296
	lsl	w23, w23, 4
	mul	w0, w25, w23
	lsr	w0, w0, 1
	lsl	w22, w0, 2
	sub	w0, w22, w0
	cmp	w0, w1
	bhi	.L2163
	ldr	w2, [x19, 32]
	sub	w1, w2, #10
	cmp	w1, 31
	bls	.L2164
.L2076:
	ldr	x1, .LC558
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2151:
	ldr	x1, .LC527
	mov	x0, x20
	bl	ue_v
	cmp	w0, 12
	str	w0, [x19, 2204]
	bhi	.L2062
	ldr	w0, [x21, 56]
	cbz	w0, .L2063
.L2062:
	ldr	x1, .LC529
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2164:
	ldr	x3, .LC556
	ldrh	w1, [x3,w1,uxtw #1]
	adr	x3, .Lrtx2078
	add	x1, x3, w1, sxth #2
	br	x1
.Lrtx2078:
	.section	.rodata
	.align	0
	.align	2
.L2078:
	.2byte	(.L2077 - .Lrtx2078) / 4
	.2byte	(.L2110 - .Lrtx2078) / 4
	.2byte	(.L2080 - .Lrtx2078) / 4
	.2byte	(.L2080 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2080 - .Lrtx2078) / 4
	.2byte	(.L2081 - .Lrtx2078) / 4
	.2byte	(.L2083 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2083 - .Lrtx2078) / 4
	.2byte	(.L2084 - .Lrtx2078) / 4
	.2byte	(.L2085 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2076 - .Lrtx2078) / 4
	.2byte	(.L2087 - .Lrtx2078) / 4
	.2byte	(.L2087 - .Lrtx2078) / 4
	.text
	.p2align 3
.L2080:
	mov	w22, 60416
	movk	w22, 0xd, lsl 16
.L2079:
	udiv	w22, w22, w0
	mov	w2, 16
	ldr	x1, .LC560
	mov	w0, 21
	cmp	w22, w2
	csel	w22, w22, w2, ls
	mov	w2, w22
	bl	dprint_vfmw
	ldr	w2, [x19, 3244]
	cmp	w2, w22
	bhi	.L2088
	ldr	w0, [x21, 56]
	cbz	w0, .L2089
.L2088:
	ldr	x1, .LC562
	mov	w3, w22
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 3244]
	cmp	w0, 16
	bhi	.L2144
	cmp	w22, w0
	csel	w22, w22, w0, cs
.L2144:
	str	w22, [x19, 3244]
.L2089:
	ldr	x1, .LC564
	mov	x0, x20
	bl	u_1
	strb	w0, [x19, 20]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2147
	ldr	x1, .LC566
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 21]
	ldr	w1, [x21, 56]
	cbnz	w1, .L2147
	cbnz	w0, .L2165
.L2091:
	add	x2, x19, 3072
	lsr	w1, w25, 1
	lsr	w0, w23, 1
	stp	w25, w23, [x2, 204]
	stp	w1, w0, [x2, 212]
.L2093:
	ldr	x1, .LC576
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 22]
	ldr	w1, [x21, 56]
	cbnz	w1, .L2147
	mov	w1, 2
	str	w1, [x19, 3332]
	cbnz	w0, .L2166
	mov	w0, 5
	strb	wzr, [x19, 3307]
	str	w0, [x19, 3320]
	mov	w0, 0
.L2096:
	ldr	w1, [x19, 3308]
	mov	w4, w23
	ldr	w2, [x19, 3312]
	mov	w3, w25
	bl	H264GetDar
	str	w0, [x19, 3316]
	ldr	w1, [x19, 3244]
	cbz	w1, .L2099
	cmp	w22, w1
	csel	w22, w22, w1, ls
.L2099:
	cmp	w22, 16
	mov	w1, 16
	ldr	x0, [x20]
	csel	w22, w22, w1, ls
	str	w22, [x20, 14952]
	ldr	w1, [x0, 28]
	cmp	w1, 25
	beq	.L2167
.L2100:
	add	w22, w22, 1
	str	w22, [x19, 3272]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2087:
	mov	w22, 12582912
	b	.L2079
	.p2align 3
.L2083:
	mov	w22, 30208
	movk	w22, 0x2f, lsl 16
	b	.L2079
	.p2align 3
.L2157:
	mov	x0, x20
	mov	x4, x21
	mov	x2, x22
	mov	x1, x24
	mov	w3, 16
	bl	Scaling_List
	ldr	w0, [x21]
	cmp	w0, 1
	bne	.L2046
	ldr	x0, .LC494
	cmp	w27, 2
	mov	x2, 16
	add	x1, x0, 80
	add	x0, x0, 96
	csel	x1, x0, x1, hi
	ldr	x0, .LC517
	ldr	x3, [x0, 104]
	mov	x0, x22
	blr	x3
	b	.L2046
	.p2align 3
.L2033:
	ldr	x1, .LC500
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
.L2057:
	ldr	x1, .LC521
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2165:
	ldr	x1, .LC568
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3256]
	ldr	x1, .LC570
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3260]
	ldr	x1, .LC572
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3264]
	ldr	x1, .LC574
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3268]
	ldr	w1, [x19, 3264]
	ldr	w2, [x19, 3256]
	lsl	w1, w1, 1
	ldr	w3, [x19, 3260]
	lsl	w2, w2, 1
	mul	w1, w1, w24
	sub	w4, w25, w2
	lsl	w24, w24, 1
	sub	w3, w4, w3, lsl 1
	sub	w4, w23, w1
	cmp	w3, wzr
	msub	w0, w24, w0, w4
	ccmp	w0, wzr, 4, gt
	ble	.L2091
	add	x4, x19, 3072
	add	w2, w2, w3, lsr 1
	add	w1, w1, w0, lsr 1
	stp	w3, w0, [x4, 204]
	stp	w2, w1, [x4, 212]
	b	.L2093
	.p2align 3
.L2166:
	add	x1, x19, 3292
	mov	x0, x20
	bl	vui_parameters
	cbnz	w0, .L2146
	ldrb	w0, [x19, 3304]
	cbz	w0, .L2146
	ldr	w2, [x19, 3372]
	cmp	w2, wzr
	ccmp	w22, w2, 0, ne
	bcc	.L2168
	ldr	w22, [x19, 3244]
	ldrb	w0, [x19, 3307]
	cmp	w22, w2
	csel	w22, w22, w2, cs
	b	.L2096
.L2168:
	ldr	x1, .LC578
	mov	w3, w22
	mov	w0, 1
	bl	dprint_vfmw
	.p2align 2
.L2146:
	ldrb	w0, [x19, 3307]
	b	.L2096
	.p2align 3
.L2167:
	ldr	w1, [x0, 708]
	cbnz	w1, .L2100
	ldr	w1, [x0, 692]
	cmp	w22, w1
	bls	.L2101
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2147
	ldr	w0, [x21, 116]
	mov	w3, 8
	stp	w22, w1, [x29, 112]
	add	x2, x29, 112
	mov	w1, 106
	blr	x4
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2041:
	ldr	x1, .LC513
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2152:
	ldr	x1, .LC531
	mov	x0, x20
	bl	u_1
	strb	w0, [x19, 16]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2147
	ldr	x1, .LC533
	mov	x0, x20
	bl	se_v
	str	w0, [x19, 2208]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2147
	ldr	x1, .LC535
	mov	x0, x20
	bl	se_v
	str	w0, [x19, 2212]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2147
	ldr	x1, .LC537
	mov	x0, x20
	bl	ue_v
	cmp	w0, 255
	str	w0, [x19, 2216]
	bhi	.L2066
	ldr	w1, [x21, 56]
	cbnz	w1, .L2066
	cbz	w0, .L2063
	ldr	x23, .LC541
	mov	w22, 0
	b	.L2069
	.p2align 3
.L2169:
	ldr	w0, [x19, 2216]
	cmp	w0, w22
	bls	.L2063
.L2069:
	mov	x1, x23
	mov	x0, x20
	bl	se_v
	add	x1, x19, x22, uxtw 2
	add	w22, w22, 1
	str	w0, [x1, 2220]
	ldr	w0, [x21, 56]
	cbz	w0, .L2169
	b	.L2147
	.p2align 3
.L2070:
	ldr	x1, .LC547
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2110:
	mov	w22, 17920
	movk	w22, 0x5, lsl 16
	b	.L2079
	.p2align 3
.L2084:
	mov	w22, 30720
	movk	w22, 0x69, lsl 16
	b	.L2079
	.p2align 3
.L2081:
	mov	w22, 55296
	movk	w22, 0x1b, lsl 16
	b	.L2079
	.p2align 3
.L2077:
	mov	w22, 20992
	movk	w22, 0x2, lsl 16
	b	.L2079
	.p2align 3
.L2085:
	mov	w22, 7864320
	b	.L2079
.L2066:
	ldr	x1, .LC539
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
	.p2align 3
.L2159:
	mov	x0, x20
	add	x4, x26, x27, lsl 2
	mov	x2, x21
	mov	x1, x22
	mov	w3, 64
	bl	Scaling_List
	ldr	w0, [x26, x27, lsl 2]
	cmp	w0, 1
	bne	.L2052
	ldr	x1, .LC494
	mov	x2, 64
	mov	x0, x21
	ldr	x3, [x28, 104]
	add	x1, x1, 176
	cbnz	w27, .L2143
	ldr	x1, .LC494
	add	x1, x1, 112
	b	.L2143
	.p2align 3
.L2059:
	ldr	x1, .LC525
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
.L2163:
	ldr	x1, .LC555
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
.L2149:
	ldr	x2, .LC495
	mov	w3, 10150
	ldr	x1, .LC496
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2139
.L2160:
	ldr	x1, .LC494
	add	x1, x1, 112
	b	.L2054
	.size	ProcessSPS, .-ProcessSPS
	.align	3
.LC494:
	.xword	.LANCHOR0
	.align	3
.LC495:
	.xword	.LC50
	.align	3
.LC496:
	.xword	.LC52
	.align	3
.LC498:
	.xword	.LC497
	.align	3
.LC500:
	.xword	.LC499
	.align	3
.LC502:
	.xword	.LC501
	.align	3
.LC504:
	.xword	.LC503
	.align	3
.LC505:
	.xword	g_event_report
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
.LC515:
	.xword	.LC514
	.align	3
.LC516:
	.xword	.LC378
	.align	3
.LC517:
	.xword	vfmw_Osal_Func_Ptr_S
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
.LC555:
	.xword	.LC554
	.align	3
.LC556:
	.xword	.L2078
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
.LC578:
	.xword	.LC577
	.align	3
.LC580:
	.xword	.LC579
	.align	3
.LC582:
	.xword	.LC581
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Sps_SvcExt
	.type	Sps_SvcExt, %function
Sps_SvcExt:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	mov	w20, 4000
	bl	_mcount
	add	x1, x19, 20480
	mov	w3, 2208
	ldr	x2, [x19, 14968]
	add	x0, x19, 12288
	ldr	x5, [x19, 14960]
	add	x0, x0, 2600
	ldr	w4, [x1, 1596]
	mov	w1, 32
	umull	x3, w4, w3
	add	x3, x2, x3
	ldr	w2, [x3, 28]
	umull	x2, w2, w20
	add	x20, x5, x2
	bl	BsGet
	mov	w21, w0
	ldr	x1, .LC584
	mov	x0, x19
	bl	u_1
	mov	w2, w0
	ldr	x1, .LC586
	mov	w0, 17
	bl	dprint_vfmw
	ldr	x2, .LC588
	mov	w1, 2
	mov	x0, x19
	bl	u_v
	cmp	w0, 1
	beq	.L2181
	ldr	w0, [x20, 40]
	sub	w1, w0, #1
	cmp	w1, 1
	bls	.L2182
.L2173:
	cmp	w0, 1
	beq	.L2183
.L2174:
	ldr	x1, .LC600
	mov	x0, x19
	bl	u_1
	cbnz	w0, .L2184
.L2175:
	ldr	x1, .LC606
	mov	x0, x19
	mov	w19, 0
	bl	u_1
	cmp	w0, 1
	beq	.L2172
	ldr	x1, .LC608
	mov	w2, w0
	ldr	w4, [x20, 40]
	mov	w3, w21
	mov	w0, 1
	bl	dprint_vfmw
.L2172:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2184:
	ldr	x1, .LC602
	mov	x0, x19
	bl	u_1
	mov	w2, w0
	ldr	x1, .LC604
	mov	w0, 17
	bl	dprint_vfmw
	b	.L2175
	.p2align 3
.L2183:
	ldr	x2, .LC596
	mov	w1, 2
	mov	x0, x19
	bl	u_v
	mov	w2, w0
	ldr	x1, .LC598
	mov	w0, 17
	bl	dprint_vfmw
	b	.L2174
	.p2align 3
.L2182:
	ldr	x1, .LC592
	mov	x0, x19
	bl	u_1
	mov	w2, w0
	ldr	x1, .LC594
	mov	w0, 17
	bl	dprint_vfmw
	ldr	w0, [x20, 40]
	b	.L2173
.L2181:
	ldr	x1, .LC590
	mov	w19, -1
	bl	dprint_vfmw
	b	.L2172
	.size	Sps_SvcExt, .-Sps_SvcExt
	.align	3
.LC584:
	.xword	.LC583
	.align	3
.LC586:
	.xword	.LC585
	.align	3
.LC588:
	.xword	.LC587
	.align	3
.LC590:
	.xword	.LC589
	.align	3
.LC592:
	.xword	.LC591
	.align	3
.LC594:
	.xword	.LC593
	.align	3
.LC596:
	.xword	.LC595
	.align	3
.LC598:
	.xword	.LC597
	.align	3
.LC600:
	.xword	.LC599
	.align	3
.LC602:
	.xword	.LC601
	.align	3
.LC604:
	.xword	.LC603
	.align	3
.LC606:
	.xword	.LC605
	.align	3
.LC608:
	.xword	.LC607
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_DecSPS
	.type	H264_DecSPS, %function
H264_DecSPS:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x22, x19, 65536
	bl	_mcount
	ldr	x2, .LC610
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	mov	w21, w0
	ldr	x1, .LC612
	mov	x0, x19
	bl	u_1
	mov	w26, w0
	ldr	x1, .LC614
	mov	x0, x19
	bl	u_1
	mov	w25, w0
	ldr	x1, .LC616
	mov	x0, x19
	bl	u_1
	mov	w24, w0
	ldr	x1, .LC618
	mov	x0, x19
	bl	u_1
	mov	w23, w0
	ldr	x2, .LC620
	mov	w1, 4
	mov	x0, x19
	bl	u_v
	ldr	x2, .LC622
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	mov	w20, w0
	ldr	x1, .LC624
	mov	x0, x19
	bl	ue_v
	mov	w27, w0
	ldr	w0, [x22, 56]
	cbnz	w0, .L2214
	ldr	w0, [x19, 14944]
	sub	w0, w0, #1
	cmp	w27, w0
	bhi	.L2215
	sub	w0, w21, #66
	mov	w22, w21
	cmp	w0, 78
	bls	.L2216
.L2190:
	ldr	x1, .LC635
	mov	w2, w21
	mov	w0, 1
	mov	w22, 100
	bl	dprint_vfmw
.L2193:
	cmp	w20, 9
	mov	w2, w20
	bls	.L2213
	cmp	w20, 41
	bhi	.L2213
	mov	w20, 4000
	ldr	x0, [x19, 14960]
	umull	x20, w27, w20
	add	x0, x0, x20
	ldrb	w1, [x0, 23]
	cbz	w1, .L2198
.L2219:
	ldr	x21, .LC638
	mov	x0, x19
	add	x21, x21, 2208
	strb	w26, [x21]
	mov	x1, x21
	strb	w25, [x21, 1]
	strb	w24, [x21, 2]
	strb	w23, [x21, 3]
	stp	w22, w2, [x21, 28]
	str	w27, [x21, 36]
	bl	ProcessSPS
	cbnz	w0, .L2217
	ldr	x1, [x19, 14960]
	mov	x0, x21
	add	x1, x1, x20
	bl	SPSEqual
	cbnz	w0, .L2218
.L2200:
	mov	w0, 86
	cmp	w22, 83
	ccmp	w22, w0, 4, ne
	beq	.L2202
.L2203:
	mov	w0, 0
.L2187:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L2216:
	ldr	x1, .LC627
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx2192
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2192:
	.section	.rodata
	.align	0
	.align	2
.L2192:
	.byte	(.L2191 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2193 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2193 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2193 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2194 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2193 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2195 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2195 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2190 - .Lrtx2192) / 4
	.byte	(.L2195 - .Lrtx2192) / 4
	.text
	.p2align 3
.L2202:
	mov	x0, x19
	bl	Sps_SvcExt
	cbz	w0, .L2203
	ldr	x1, .LC643
	mov	w0, 1
	bl	dprint_vfmw
.L2214:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	mov	w0, -1
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L2213:
	ldr	x1, .LC637
	mov	w3, 41
	mov	w0, 1
	mov	w20, 4000
	bl	dprint_vfmw
	umull	x20, w27, w20
	ldr	x0, [x19, 14960]
	mov	w2, 41
	add	x0, x0, x20
	ldrb	w1, [x0, 23]
	cbnz	w1, .L2219
.L2198:
	str	w22, [x0, 28]
	ldr	x0, [x19, 14960]
	strb	w26, [x0, x20]
	mov	x0, x19
	ldr	x1, [x19, 14960]
	add	x1, x1, x20
	strb	w25, [x1, 1]
	ldr	x1, [x19, 14960]
	add	x1, x1, x20
	strb	w24, [x1, 2]
	ldr	x1, [x19, 14960]
	add	x1, x1, x20
	strb	w23, [x1, 3]
	ldr	x1, [x19, 14960]
	add	x1, x1, x20
	str	w2, [x1, 32]
	ldr	x1, [x19, 14960]
	add	x1, x1, x20
	str	w27, [x1, 36]
	ldr	x1, [x19, 14960]
	add	x1, x1, x20
	bl	ProcessSPS
	cbnz	w0, .L2220
	ldr	x0, [x19, 14960]
	mov	w1, 1
	add	x0, x0, x20
	strb	w1, [x0, 24]
	ldr	x0, [x19, 14960]
	add	x20, x0, x20
	strb	w1, [x20, 23]
	b	.L2200
	.p2align 3
.L2215:
	ldr	x1, .LC625
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 14944]
	sub	w0, w1, #1
	cmp	w27, w0
	bls	.L2214
	ldr	x0, .LC626
	ldr	x4, [x0]
	cbz	x4, .L2214
	ldr	w0, [x22, 116]
	add	x2, x29, 104
	stp	w27, w1, [x29, 104]
	mov	w3, 8
	mov	w1, 109
	blr	x4
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	mov	w0, -1
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L2194:
	ldr	x1, .LC631
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2193
	.p2align 3
.L2195:
	ldr	x1, .LC633
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2193
	.p2align 3
.L2191:
	ldr	x1, .LC629
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2193
	.p2align 3
.L2218:
	ldr	x3, .LC641
	mov	w1, 1
	ldr	x0, [x19, 14960]
	mov	x2, 4000
	strb	w1, [x21, 24]
	ldr	x3, [x3, 112]
	add	x0, x0, x20
	strb	w1, [x21, 23]
	mov	x1, x21
	blr	x3
	b	.L2200
	.p2align 3
.L2217:
	ldr	x1, .LC640
	mov	w3, w27
	mov	w2, 10776
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2187
.L2220:
	ldr	x1, .LC640
	mov	w2, 10801
	mov	w3, w27
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x19, 14960]
	mov	w2, 1
	mov	w0, -1
	add	x1, x1, x20
	strb	w2, [x1, 24]
	ldr	x1, [x19, 14960]
	add	x20, x1, x20
	strb	wzr, [x20, 23]
	b	.L2187
	.size	H264_DecSPS, .-H264_DecSPS
	.align	3
.LC610:
	.xword	.LC609
	.align	3
.LC612:
	.xword	.LC611
	.align	3
.LC614:
	.xword	.LC613
	.align	3
.LC616:
	.xword	.LC615
	.align	3
.LC618:
	.xword	.LC617
	.align	3
.LC620:
	.xword	.LC619
	.align	3
.LC622:
	.xword	.LC621
	.align	3
.LC624:
	.xword	.LC623
	.align	3
.LC625:
	.xword	.LC394
	.align	3
.LC626:
	.xword	g_event_report
	.align	3
.LC627:
	.xword	.L2192
	.align	3
.LC629:
	.xword	.LC628
	.align	3
.LC631:
	.xword	.LC630
	.align	3
.LC633:
	.xword	.LC632
	.align	3
.LC635:
	.xword	.LC634
	.align	3
.LC637:
	.xword	.LC636
	.align	3
.LC638:
	.xword	.LANCHOR2
	.align	3
.LC640:
	.xword	.LC639
	.align	3
.LC641:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC643:
	.xword	.LC642
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	PassBytes
	.type	PassBytes, %function
PassBytes:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	mov	w19, w1
	bl	_mcount
	ldr	x0, [x20, 632]
	cmp	w19, wzr
	ccmp	x0, xzr, 4, ne
	beq	.L2230
	ldrb	w2, [x0]
	sbfiz	x1, x2, 5, 32
	add	x1, x0, x1
	ldr	x1, [x1, 8]
	cbz	x1, .L2230
	ldr	w1, [x0, 76]
	cmp	w2, 1
	ccmp	w1, 2, 2, ls
	bhi	.L2230
	cmp	w1, w2
	bls	.L2230
	uxtw	x5, w2
	add	x3, x0, x5, lsl 5
	ldr	w1, [x3, 28]
	ldr	w3, [x3, 16]
	add	w1, w1, w19, lsl 3
	lsl	w3, w3, 3
	cmp	w1, w3
	bhi	.L2225
	b	.L2223
	.p2align 3
.L2226:
	ldr	w4, [x6, 28]
	ldr	w6, [x6, 16]
	sub	w4, w4, w3
	add	w1, w1, w4
	lsl	w3, w6, 3
	cmp	w1, w3
	bls	.L2223
.L2225:
	add	x0, x0, x5, lsl 5
	add	w5, w2, 1
	mov	x2, x5
	str	w3, [x0, 28]
	ldr	x0, [x20, 632]
	ldrb	w4, [x0]
	add	w4, w4, 1
	strb	w4, [x0]
	ldr	x0, [x20, 632]
	add	x6, x0, x5, lsl 5
	ldr	w4, [x0, 76]
	cmp	w4, w5
	bhi	.L2226
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2223:
	add	x5, x0, x5, lsl 5
	mov	w0, 1
	str	w1, [x5, 28]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2230:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	PassBytes, .-PassBytes
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	GetBytes
	.type	GetBytes, %function
GetBytes:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x21, x0
	mov	x19, x1
	mov	x0, x30
	mov	w20, w2
	bl	_mcount
	cbz	x19, .L2246
	ldr	x1, [x21, 632]
	cmp	x1, xzr
	ccmp	w20, wzr, 4, ne
	ble	.L2246
	ldrb	w2, [x1]
	sbfiz	x0, x2, 5, 32
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cbz	x0, .L2246
	ldr	w0, [x1, 76]
	cmp	w2, 1
	ccmp	w0, 2, 2, ls
	bhi	.L2246
	cmp	w0, w2
	bls	.L2246
	mov	w6, 0
.L2241:
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
	bcs	.L2236
	ldrb	w0, [x4, x0]
	add	w6, w6, 1
	strb	w0, [x19]
	cmp	w6, w20
	add	x19, x19, 1
	add	x5, x5, 1
	ldr	x0, [x21, 632]
	add	x0, x0, x7
	ldr	w3, [x0, 28]
	add	w3, w3, 8
	str	w3, [x0, 28]
	bcc	.L2239
	b	.L2238
	.p2align 3
.L2240:
	ldrb	w1, [x5], 1
	add	w6, w6, 1
	strb	w1, [x19], 1
	cmp	w6, w20
	ldr	x3, [x21, 632]
	add	x3, x3, x7
	ldr	w4, [x3, 28]
	add	w4, w4, 8
	str	w4, [x3, 28]
	bcs	.L2238
.L2239:
	cmp	x8, x5
	bne	.L2240
	ldr	x1, [x21, 632]
.L2236:
	ldrb	w0, [x1]
	add	w2, w2, 1
	add	w0, w0, 1
	strb	w0, [x1]
	ldr	x1, [x21, 632]
	ldr	w0, [x1, 76]
	cmp	w0, w2
	bhi	.L2241
	mov	w0, w6
	b	.L2235
	.p2align 3
.L2238:
	mov	w0, w20
.L2235:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2246:
	mov	w0, -1
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	GetBytes, .-GetBytes
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_DecFramePackingSEI
	.type	H264_DecFramePackingSEI, %function
H264_DecFramePackingSEI:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x21, x19, 24576
	bl	_mcount
	add	x20, x21, 3192
	ldr	x1, .LC646
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 24]
	ldr	x2, .LC648
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	sxtb	w0, w0
	strb	w0, [x20, 1]
	cbz	w0, .L2254
.L2249:
	ldr	x2, .LC680
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x21, 3194]
	mov	w1, 1
	strb	w1, [x21, 3192]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2254:
	ldr	x2, .LC650
	mov	w1, 7
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 16]
	ldr	x2, .LC652
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 3]
	ldr	x2, .LC654
	mov	w1, 6
	mov	x0, x19
	bl	u_v
	str	w0, [x20, 20]
	ldr	x2, .LC656
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 4]
	ldr	x2, .LC658
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 5]
	ldr	x2, .LC660
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 6]
	ldr	x2, .LC662
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 7]
	ldr	x2, .LC664
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 8]
	ldr	x2, .LC666
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 9]
	ldrsb	w1, [x20, 3]
	cbnz	w1, .L2250
	ldr	w0, [x20, 16]
	cmp	w0, 5
	beq	.L2250
	ldr	x2, .LC668
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 10]
	ldr	x2, .LC670
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 11]
	ldr	x2, .LC672
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 12]
	ldr	x2, .LC674
	mov	w1, 1
	mov	x0, x19
	bl	u_v
	strb	w0, [x20, 13]
.L2250:
	ldr	x2, .LC676
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	str	w0, [x21, 3220]
	ldr	x1, .LC678
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 3224]
	b	.L2249
	.size	H264_DecFramePackingSEI, .-H264_DecFramePackingSEI
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_Sei_Get_Dpb_Output_Time
	.type	H264_Sei_Get_Dpb_Output_Time, %function
H264_Sei_Get_Dpb_Output_Time:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 20480
	mov	w1, 2208
	ldr	x9, [x19, 14968]
	add	x3, x19, 24576
	ldr	x8, [x19, 14960]
	ldr	w2, [x0, 1596]
	mov	w0, 4000
	ldr	w5, [x3, 3248]
	ldr	w6, [x3, 3244]
	umull	x1, w2, w1
	add	w2, w5, 1
	ldr	w5, [x3, 3232]
	add	x1, x9, x1
	add	w6, w6, 1
	cmp	w6, 1
	ccmp	w5, wzr, 0, ne
	ldr	w1, [x1, 28]
	str	w2, [x3, 3248]
	ldr	w2, [x3, 3236]
	umull	x0, w1, w0
	str	w6, [x3, 3244]
	add	x0, x8, x0
	str	w5, [x3, 3252]
	add	x0, x0, 3292
	str	w2, [x3, 3260]
	ldr	w1, [x0, 52]
	str	w1, [x3, 3296]
	ldr	w1, [x0, 56]
	str	w1, [x3, 3300]
	bne	.L2256
	ldr	w5, [x3, 3256]
	add	w5, w5, 2
	str	w5, [x3, 3252]
.L2256:
	cmp	w6, 1
	add	x4, x3, 3240
	beq	.L2263
	ldrb	w1, [x4, 81]
	cmp	w1, 1
	ldr	x1, [x4, 48]
	add	x1, x1, x5, uxtw
	str	x1, [x4, 40]
	beq	.L2264
.L2258:
	strb	wzr, [x3, 3321]
	add	x1, x1, x2, uxtw
	lsr	x1, x1, 1
	ldr	w2, [x0, 76]
	ldr	w4, [x0, 52]
	sub	x2, x1, x2
	mul	x2, x2, x4
	str	x2, [x3, 3264]
	ldr	w0, [x0, 76]
	str	w5, [x3, 3256]
	sub	x1, x1, x0
	str	x1, [x3, 3272]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2263:
	mov	x1, 0
	str	xzr, [x3, 3280]
	str	xzr, [x3, 3288]
	b	.L2258
	.p2align 3
.L2264:
	str	x1, [x4, 48]
	b	.L2258
	.size	H264_Sei_Get_Dpb_Output_Time, .-H264_Sei_Get_Dpb_Output_Time
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_Get_Last_IDR_Frame_Info
	.type	H264_Get_Last_IDR_Frame_Info, %function
H264_Get_Last_IDR_Frame_Info:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, w1
	bl	_mcount
	cmp	w20, 5
	beq	.L2270
.L2265:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2270:
	ldr	x0, [x19]
	ldr	x0, [x0, 56]
	cmn	x0, #1
	beq	.L2265
	ldr	x1, [x19, 27848]
	str	x0, [x19, 27880]
	str	x1, [x19, 27888]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	H264_Get_Last_IDR_Frame_Info, .-H264_Get_Last_IDR_Frame_Info
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_Sei_Get_Frame_Pts
	.type	H264_Sei_Get_Frame_Pts, %function
H264_Sei_Get_Frame_Pts:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 24576
	ldr	x3, [x19, 27848]
	ldr	x6, [x19, 27880]
	ldr	x7, [x19, 27888]
	ldr	w4, [x1, 3296]
	ldr	w5, [x1, 3300]
	cbz	w4, .L2274
	mov	w1, w4
	mov	w2, w5
	b	.L2273
	.p2align 3
.L2275:
	mov	w1, w0
.L2273:
	udiv	w0, w2, w1
	msub	w0, w0, w1, w2
	mov	w2, w1
	cbnz	w0, .L2275
.L2272:
	udiv	w4, w4, w1
	sub	x0, x3, x7
	lsl	w2, w0, 6
	add	x19, x19, 61440
	sub	w2, w2, w0
	lsl	w2, w2, 1
	sub	w0, w2, w0
	lsl	w0, w0, 3
	udiv	w1, w5, w1
	mul	w0, w0, w4
	udiv	w1, w0, w1
	add	x1, x1, x6
	str	x1, [x19, 728]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L2274:
	mov	w1, w5
	b	.L2272
	.size	H264_Sei_Get_Frame_Pts, .-H264_Sei_Get_Frame_Pts
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_DecPicTimingSEI
	.type	H264_DecPicTimingSEI, %function
H264_DecPicTimingSEI:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	mov	w19, 4000
	bl	_mcount
	ldr	w0, [x20, 15012]
	ldr	x3, [x20, 14960]
	umull	x19, w0, w19
	add	x19, x3, x19
	ldrb	w0, [x19, 23]
	cbnz	w0, .L2277
	ldr	w1, [x20, 14944]
	cmp	w1, wzr
	ble	.L2278
	ldrb	w0, [x3, 23]
	cbnz	w0, .L2296
	sub	w1, w1, #1
	add	x0, x3, 4000
	add	x2, x1, 1
	lsl	x1, x2, 6
	sub	x1, x1, x2
	lsl	x1, x1, 1
	sub	x1, x1, x2
	add	x2, x3, x1, lsl 5
	b	.L2281
	.p2align 3
.L2282:
	ldrb	w1, [x1]
	cbnz	w1, .L2279
.L2281:
	cmp	x0, x2
	mov	x19, x0
	add	x0, x0, 4000
	sub	x1, x0, #3977
	bne	.L2282
.L2278:
	ldr	x1, .LC682
	mov	w0, 1
	mov	w19, -1
	bl	dprint_vfmw
.L2283:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L2296:
	mov	x19, x3
	.p2align 2
.L2279:
	cbz	x19, .L2278
.L2277:
	ldrb	w0, [x19, 22]
	cbz	w0, .L2284
	ldrb	w0, [x19, 3299]
	cbz	w0, .L2297
	ldr	w1, [x19, 3676]
	ldr	w22, [x19, 3680]
	add	w1, w1, 1
	add	w22, w22, 1
.L2287:
	ldr	x2, .LC684
	mov	x0, x20
	add	x21, x20, 24576
	bl	u_v
	ldr	x2, .LC686
	mov	w1, w22
	str	w0, [x21, 3232]
	mov	x0, x20
	bl	u_v
	str	w0, [x21, 3236]
.L2284:
	ldrb	w0, [x19, 3303]
	mov	w19, 0
	cbz	w0, .L2283
	ldr	x2, .LC688
	mov	x0, x20
	add	x20, x20, 24576
	mov	w1, 4
	bl	u_v
	strb	w0, [x20, 3228]
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2297:
	ldrb	w0, [x19, 3302]
	cbz	w0, .L2284
	ldr	w1, [x19, 3988]
	ldr	w22, [x19, 3992]
	add	w1, w1, 1
	add	w22, w22, 1
	b	.L2287
	.size	H264_DecPicTimingSEI, .-H264_DecPicTimingSEI
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_DecSEI
	.type	H264_DecSEI, %function
H264_DecSEI:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x28, x0
	mov	x0, x30
	add	x20, x29, 144
	add	x21, x28, 12288
	bl	_mcount
	mov	w22, 0
	ldr	x0, [x28, 632]
	mov	w1, 32
	add	x19, x21, 2600
	strb	wzr, [x20, -1]!
	str	w1, [x0, 28]
	b	.L2301
	.p2align 3
.L2526:
	mov	w1, 8
	mov	x0, x19
	bl	BsSkip
	cmp	w23, wzr
	ble	.L2300
	ldrb	w0, [x29, 143]
	cmp	w0, 255
	bne	.L2525
.L2301:
	mov	w2, 1
	mov	x1, x20
	mov	x0, x28
	bl	GetBytes
	mov	w23, w0
	ldr	w2, [x28, 14912]
	ldr	w0, [x28, 14920]
	ldrb	w1, [x29, 143]
	add	w0, w0, 8
	cmp	w0, w2, lsl 3
	add	w22, w22, w1
	bls	.L2526
.L2300:
	ldr	x1, .LC690
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2303
	.p2align 3
.L2525:
	ldr	x26, .LC696
	mov	w27, 0
	ldr	x24, .LC693
	add	x0, x26, 216
	str	x0, [x29, 112]
	.p2align 2
.L2481:
	mov	w19, 0
	add	x23, x21, 2600
	b	.L2307
	.p2align 3
.L2528:
	mov	w1, 8
	mov	x0, x23
	bl	BsSkip
	cmp	w25, wzr
	ble	.L2305
	ldrb	w0, [x29, 143]
	cmp	w0, 255
	bne	.L2527
.L2307:
	mov	w2, 1
	mov	x1, x20
	mov	x0, x28
	bl	GetBytes
	mov	w25, w0
	ldr	w2, [x28, 14912]
	ldr	w0, [x28, 14920]
	ldrb	w1, [x29, 143]
	add	w0, w0, 8
	cmp	w0, w2, lsl 3
	add	w19, w19, w1
	bls	.L2528
.L2305:
	ldr	x1, .LC692
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L2303:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2527:
	cmp	w19, 409600
	bgt	.L2305
	ldr	x0, [x28, 632]
	ldr	w1, [x0, 76]
	ldr	w2, [x0, 28]
	cmp	w1, 2
	beq	.L2529
.L2309:
	cmp	w22, 45
	bls	.L2530
.L2310:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2531
.L2471:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2474
	.p2align 3
.L2475:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2474:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2475
	.p2align 2
.L2521:
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
.L2337:
	cmp	w0, 3
	bhi	.L2532
.L2476:
	mov	w22, 0
	add	x19, x21, 2600
	b	.L2480
	.p2align 3
.L2478:
	mov	w1, 8
	mov	x0, x19
	bl	BsSkip
	cmp	w23, wzr
	ble	.L2479
	ldrb	w0, [x29, 143]
	cmp	w0, 255
	bne	.L2533
.L2480:
	mov	w2, 1
	mov	x1, x20
	mov	x0, x28
	bl	GetBytes
	mov	w23, w0
	ldr	w2, [x28, 14912]
	ldr	w0, [x28, 14920]
	ldrb	w1, [x29, 143]
	add	w0, w0, 8
	cmp	w0, w2, lsl 3
	add	w22, w22, w1
	bls	.L2478
.L2479:
	mov	w0, 0
.L2534:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2533:
	cmp	w0, 128
	bne	.L2481
	mov	w0, 0
	b	.L2534
	.p2align 3
.L2530:
	ldrh	w0, [x24,w22,uxtw #1]
	adr	x1, .Lrtx2312
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx2312:
	.section	.rodata
	.align	0
	.align	2
.L2312:
	.2byte	(.L2311 - .Lrtx2312) / 4
	.2byte	(.L2313 - .Lrtx2312) / 4
	.2byte	(.L2314 - .Lrtx2312) / 4
	.2byte	(.L2315 - .Lrtx2312) / 4
	.2byte	(.L2316 - .Lrtx2312) / 4
	.2byte	(.L2317 - .Lrtx2312) / 4
	.2byte	(.L2318 - .Lrtx2312) / 4
	.2byte	(.L2319 - .Lrtx2312) / 4
	.2byte	(.L2320 - .Lrtx2312) / 4
	.2byte	(.L2321 - .Lrtx2312) / 4
	.2byte	(.L2322 - .Lrtx2312) / 4
	.2byte	(.L2323 - .Lrtx2312) / 4
	.2byte	(.L2324 - .Lrtx2312) / 4
	.2byte	(.L2325 - .Lrtx2312) / 4
	.2byte	(.L2326 - .Lrtx2312) / 4
	.2byte	(.L2327 - .Lrtx2312) / 4
	.2byte	(.L2328 - .Lrtx2312) / 4
	.2byte	(.L2329 - .Lrtx2312) / 4
	.2byte	(.L2330 - .Lrtx2312) / 4
	.2byte	(.L2331 - .Lrtx2312) / 4
	.2byte	(.L2332 - .Lrtx2312) / 4
	.2byte	(.L2333 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2310 - .Lrtx2312) / 4
	.2byte	(.L2334 - .Lrtx2312) / 4
	.text
.L2334:
	mov	w1, w19
	mov	x0, x28
	ldr	w23, [x28, 14920]
	bl	H264_DecFramePackingSEI
	ldr	w3, [x28, 14920]
	sub	w0, w3, w23
	lsl	w23, w19, 3
	cmp	w0, w23
	bge	.L2464
	sub	w26, w23, w0
	ldr	w0, [x28, 14912]
	cmp	w26, wzr
	add	w22, w26, 7
	csel	w22, w22, w26, lt
	lsl	w0, w0, 3
	asr	w22, w22, 3
	cmp	w22, wzr
	ble	.L2466
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2466
	add	x25, x21, 2600
	mov	w23, 0
	b	.L2467
	.p2align 3
.L2468:
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2466
.L2467:
	mov	w1, 8
	mov	x0, x25
	add	w23, w23, 1
	bl	BsSkip
	cmp	w23, w22
	bne	.L2468
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
.L2466:
	asr	w1, w26, 31
	lsr	w2, w1, 29
	add	w1, w26, w2
	and	w1, w1, 7
	sub	w1, w1, w2
	add	w3, w1, w3
	cmp	w3, w0
	bls	.L2535
.L2464:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2521
	ldr	x1, .LC698
	mov	w3, 12065
	ldr	x2, .LC754
	mov	w0, 20
	bl	dprint_vfmw
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
	b	.L2337
.L2333:
	ldr	x1, .LC751
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2536
.L2458:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2461
	.p2align 3
.L2462:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2461:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2462
	b	.L2521
.L2332:
	ldr	x1, .LC749
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2537
.L2453:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2456
	.p2align 3
.L2457:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2456:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2457
	b	.L2521
.L2329:
	ldr	x1, .LC743
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2538
.L2438:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2441
	.p2align 3
.L2442:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2441:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2442
	b	.L2521
.L2328:
	ldr	x1, .LC741
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2539
.L2433:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2436
	.p2align 3
.L2437:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2436:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2437
	b	.L2521
.L2327:
	ldr	x1, .LC739
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2540
.L2428:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2431
	.p2align 3
.L2432:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2431:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2432
	b	.L2521
.L2326:
	ldr	x1, .LC737
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2541
.L2423:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2426
	.p2align 3
.L2427:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2426:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2427
	b	.L2521
.L2331:
	ldr	x1, .LC747
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2542
.L2448:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2451
	.p2align 3
.L2452:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2451:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2452
	b	.L2521
.L2330:
	ldr	x1, .LC745
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2543
.L2443:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2446
	.p2align 3
.L2447:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2446:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2447
	b	.L2521
.L2325:
	ldr	x1, .LC735
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2544
.L2418:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2421
	.p2align 3
.L2422:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2421:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2422
	b	.L2521
.L2324:
	ldr	x1, .LC733
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2545
.L2413:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2416
	.p2align 3
.L2417:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2416:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2417
	b	.L2521
.L2323:
	ldr	x1, .LC731
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2546
.L2408:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2411
	.p2align 3
.L2412:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2411:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2412
	b	.L2521
.L2322:
	ldr	x1, .LC729
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2547
.L2403:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2406
	.p2align 3
.L2407:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2406:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2407
	b	.L2521
.L2321:
	ldr	x1, .LC727
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2548
.L2398:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2401
	.p2align 3
.L2402:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2401:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2402
	b	.L2521
.L2320:
	ldr	x1, .LC725
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2549
.L2393:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2396
	.p2align 3
.L2397:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2396:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2397
	b	.L2521
.L2319:
	ldr	x1, .LC723
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2550
.L2388:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2391
	.p2align 3
.L2392:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2391:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2392
	b	.L2521
.L2318:
	ldr	x1, .LC721
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	cmp	w19, wzr
	ble	.L2385
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2385
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2386
	.p2align 3
.L2387:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2385
.L2386:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2387
.L2385:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2521
	ldr	x2, .LC696
	mov	w3, 11637
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
	b	.L2337
.L2317:
	cbnz	w27, .L2316
	ldr	x1, .LC708
	add	x22, x28, 61440
	mov	w3, w19
	mov	w0, 20
	add	x25, x28, 65536
	bl	dprint_vfmw
	ldr	w23, [x22, 2800]
	cmp	w23, 3
	bhi	.L2551
.L2361:
	ldr	w0, [x25, 116]
	bl	GetUsd
	uxtw	x1, w23
	add	x1, x1, 4096
	add	x1, x28, x1, lsl 3
	str	x0, [x1, 31480]
	ldr	w0, [x22, 2800]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x7, [x0, 31480]
	cbz	x7, .L2484
	ldr	x3, [x28, 632]
	cmp	w19, 1024
	mov	w6, 1024
	csel	w26, w19, w6, le
	ldrb	w2, [x3]
	ldr	w5, [x3, 76]
	cmp	w2, w5
	bcs	.L2485
	mov	w23, 0
	.p2align 2
.L2363:
	ubfiz	x1, x2, 5, 32
	add	w2, w2, 1
	add	x1, x3, x1
	cmp	w2, w5
	ldr	w0, [x1, 28]
	ldr	w1, [x1, 16]
	add	w0, w0, 7
	sub	w0, w1, w0, lsr 3
	add	w23, w23, w0
	bne	.L2363
	mov	w0, w23
.L2362:
	cmp	w26, w0
	csel	w23, w23, w26, gt
	cbnz	w27, .L2552
	mov	w3, 0
	mov	w5, 0
.L2365:
	sub	w2, w23, w3
	cmp	w2, wzr
	ble	.L2553
	ldr	w0, [x22, 2800]
	str	w5, [x29, 120]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x1, [x0, 31480]
	mov	x0, x28
	bl	GetBytes
	cmp	w0, wzr
	mov	w23, w0
	ldr	w5, [x29, 120]
	ble	.L2554
	sub	w0, w26, w5
	cmp	w23, w0
	bge	.L2371
	ldr	w0, [x22, 2800]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	w23, [x0, 1052]
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2372
	add	x19, x21, 2600
	mov	w26, 0
	b	.L2373
	.p2align 3
.L2376:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2372
.L2373:
	mov	w1, 8
	mov	x0, x19
	add	w26, w26, 1
	bl	BsSkip
	cmp	w23, w26
	bne	.L2376
.L2372:
	ldr	w0, [x22, 2800]
	ldr	x1, [x28]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x1, [x1, 56]
	ldr	x0, [x0, 31480]
	str	x1, [x0, 1056]
	mov	w1, 8
	ldr	x0, .LC715
	ldr	x5, [x0]
	ldr	w0, [x22, 2800]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	w1, [x0, 1040]
	ldr	w0, [x22, 2800]
	ldr	w1, [x25, 76]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	w1, [x0, 1044]
	cbz	x5, .L2375
	ldr	w1, [x22, 2800]
	mov	w3, 1064
	ldr	w0, [x25, 116]
	add	x1, x1, 4096
	add	x1, x28, x1, lsl 3
	ldr	x2, [x1, 31480]
	mov	w1, 6
	blr	x5
.L2375:
	ldr	w0, [x22, 2800]
	add	w0, w0, 1
	str	w0, [x22, 2800]
	cmp	w0, 3
	bls	.L2476
.L2532:
	ldr	x1, .LC753
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2303
.L2313:
	ldr	x1, .LC700
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	ldr	w23, [x28, 14920]
	mov	w1, w19
	mov	x0, x28
	bl	H264_DecPicTimingSEI
	ldr	w3, [x28, 14920]
	sub	w0, w3, w23
	lsl	w23, w19, 3
	cmp	w0, w23
	bge	.L2342
	sub	w23, w23, w0
	ldr	w0, [x28, 14912]
	cmp	w23, wzr
	add	w22, w23, 7
	csel	w22, w22, w23, lt
	lsl	w0, w0, 3
	asr	w22, w22, 3
	cmp	w22, wzr
	ble	.L2344
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2344
	add	x25, x21, 2600
	mov	w26, 0
	b	.L2345
	.p2align 3
.L2346:
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2344
.L2345:
	mov	w1, 8
	mov	x0, x25
	add	w26, w26, 1
	bl	BsSkip
	cmp	w26, w22
	bne	.L2346
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
.L2344:
	asr	w1, w23, 31
	lsr	w2, w1, 29
	add	w1, w23, w2
	and	w1, w1, 7
	sub	w1, w1, w2
	add	w3, w1, w3
	cmp	w3, w0
	bls	.L2555
.L2342:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2521
	ldr	x2, .LC696
	mov	w3, 11385
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
	b	.L2337
.L2311:
	add	x0, x28, 24576
	mov	w3, 1
	ldr	w1, [x0, 3240]
	strb	w3, [x0, 3321]
	mov	w3, w19
	add	w1, w1, 1
	str	w1, [x0, 3240]
	ldr	x1, .LC695
	str	wzr, [x0, 3248]
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2556
.L2335:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2339
	.p2align 3
.L2340:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2339:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2340
	b	.L2521
.L2315:
	ldr	x1, .LC704
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2557
.L2354:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2357
	.p2align 3
.L2358:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2357:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2358
	b	.L2521
.L2314:
	ldr	x1, .LC702
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2558
.L2349:
	cmp	w19, wzr
	ble	.L2521
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2352
	.p2align 3
.L2353:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2521
.L2352:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2353
	b	.L2521
.L2316:
	ldr	x1, .LC706
	add	x22, x28, 61440
	mov	w3, w19
	mov	w0, 20
	mov	w27, 1
	add	x25, x28, 65536
	bl	dprint_vfmw
	ldr	w23, [x22, 2800]
	cmp	w23, 3
	bls	.L2361
.L2551:
	ldr	w0, [x25, 116]
	mov	w23, 0
	ldr	x1, [x22, 2808]
	bl	FreeUsdByDec
	ldr	w0, [x25, 116]
	ldr	x1, [x22, 2816]
	bl	FreeUsdByDec
	ldr	w0, [x25, 116]
	ldr	x1, [x22, 2824]
	bl	FreeUsdByDec
	ldr	w0, [x25, 116]
	ldr	x1, [x22, 2832]
	bl	FreeUsdByDec
	str	wzr, [x22, 2800]
	add	x0, x22, 2560
	stp	xzr, xzr, [x0, 248]
	stp	xzr, xzr, [x0, 264]
	b	.L2361
	.p2align 3
.L2529:
	ldr	w0, [x0, 60]
	add	w2, w2, w0
	lsr	w2, w2, 3
	b	.L2309
.L2371:
	ldr	w0, [x22, 2800]
	sub	w23, w19, w5
	cmp	w26, w19
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	w23, [x0, 1052]
	bcc	.L2559
.L2378:
	cmp	w23, wzr
	ble	.L2372
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2372
	add	x19, x21, 2600
	mov	w26, 0
	b	.L2380
	.p2align 3
.L2381:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2372
.L2380:
	mov	w1, 8
	mov	x0, x19
	add	w26, w26, 1
	bl	BsSkip
	cmp	w23, w26
	bne	.L2381
	b	.L2372
.L2535:
	add	x0, x21, 2600
	bl	BsSkip
	b	.L2464
.L2555:
	add	x0, x21, 2600
	bl	BsSkip
	b	.L2342
.L2552:
	strb	w27, [x7, 1026]
	mov	w1, 8
	ldr	x2, .LC710
	mov	x0, x28
	ldr	w3, [x22, 2800]
	add	x3, x3, 4096
	add	x3, x28, x3, lsl 3
	ldr	x3, [x3, 31480]
	str	x3, [x29, 120]
	bl	u_v
	ldr	x3, [x29, 120]
	strb	w0, [x3, 1027]
	ldr	w0, [x22, 2800]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x3, [x0, 31480]
	ldrb	w0, [x3, 1027]
	cmp	w0, 255
	beq	.L2366
	strb	wzr, [x3, 1028]
	mov	w3, 3
	mov	w5, w3
.L2367:
	ldr	w0, [x22, 2800]
	mov	w1, 16
	ldr	x2, .LC714
	add	x0, x0, 4096
	stp	w3, w5, [x29, 104]
	add	x0, x28, x0, lsl 3
	ldr	x7, [x0, 31480]
	mov	x0, x28
	str	x7, [x29, 120]
	bl	u_v
	ldr	x7, [x29, 120]
	ldr	w3, [x29, 104]
	str	w3, [x29, 120]
	mov	w1, w3
	strh	w0, [x7, 1030]
	mov	x0, x28
	bl	PassBytes
	ldr	w3, [x29, 120]
	ldr	w5, [x29, 108]
	cmp	w23, w3
	bne	.L2365
	b	.L2372
.L2559:
	sub	w1, w19, w26
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2378
	ldr	x2, .LC696
	mov	w3, 11584
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2378
.L2485:
	mov	w0, 0
	mov	w23, 0
	b	.L2362
.L2550:
	ldr	x2, .LC696
	mov	w3, 11649
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2388
.L2558:
	ldr	x2, .LC696
	mov	w3, 11397
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2349
.L2557:
	ldr	x2, .LC696
	mov	w3, 11423
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2354
.L2556:
	ldr	x2, .LC696
	mov	w3, 11332
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2335
.L2531:
	ldr	x1, .LC698
	mov	w3, 12077
	ldr	x2, [x29, 112]
	mov	w0, 20
	bl	dprint_vfmw
	b	.L2471
.L2537:
	ldr	x2, .LC696
	mov	w3, 11987
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2453
.L2536:
	ldr	x1, .LC698
	mov	w3, 12013
	ldr	x2, .LC754
	mov	w0, 20
	bl	dprint_vfmw
	b	.L2458
.L2541:
	ldr	x2, .LC696
	mov	w3, 11831
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2423
.L2540:
	ldr	x2, .LC696
	mov	w3, 11857
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2428
.L2539:
	ldr	x2, .LC696
	mov	w3, 11883
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2433
.L2538:
	ldr	x2, .LC696
	mov	w3, 11909
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2438
.L2549:
	ldr	x2, .LC696
	mov	w3, 11675
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2393
.L2548:
	ldr	x2, .LC696
	mov	w3, 11701
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2398
.L2547:
	ldr	x2, .LC696
	mov	w3, 11727
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2403
.L2546:
	ldr	x2, .LC696
	mov	w3, 11753
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2408
.L2545:
	ldr	x2, .LC696
	mov	w3, 11779
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2413
.L2544:
	ldr	x2, .LC696
	mov	w3, 11805
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2418
.L2543:
	ldr	x2, .LC696
	mov	w3, 11935
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2443
.L2542:
	ldr	x2, .LC696
	mov	w3, 11961
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2448
.L2553:
	ldr	x1, .LC717
	mov	w0, 1
	bl	dprint_vfmw
.L2522:
	ldr	w1, [x22, 2800]
	ldr	w0, [x25, 116]
	add	x1, x1, 4096
	add	x1, x28, x1, lsl 3
	ldr	x1, [x1, 31480]
	bl	FreeUsdByDec
	ldr	w1, [x22, 2800]
	mov	w0, -1
	add	x1, x1, 4096
	add	x4, x28, x1, lsl 3
	str	xzr, [x4, 31480]
	b	.L2303
.L2366:
	ldr	x2, .LC712
	mov	w1, 8
	mov	x0, x28
	str	x3, [x29, 120]
	bl	u_v
	ldr	x3, [x29, 120]
	strb	w0, [x3, 1028]
	mov	w3, 4
	mov	w5, w3
	b	.L2367
.L2554:
	ldr	x1, .LC719
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2522
.L2484:
	mov	w0, -1
	b	.L2303
	.size	H264_DecSEI, .-H264_DecSEI
	.align	3
.LC690:
	.xword	.LC689
	.align	3
.LC692:
	.xword	.LC691
	.align	3
.LC693:
	.xword	.L2312
	.align	3
.LC695:
	.xword	.LC694
	.align	3
.LC696:
	.xword	.LANCHOR1
	.align	3
.LC698:
	.xword	.LC697
	.align	3
.LC700:
	.xword	.LC699
	.align	3
.LC702:
	.xword	.LC701
	.align	3
.LC704:
	.xword	.LC703
	.align	3
.LC706:
	.xword	.LC705
	.align	3
.LC708:
	.xword	.LC707
	.align	3
.LC710:
	.xword	.LC709
	.align	3
.LC712:
	.xword	.LC711
	.align	3
.LC714:
	.xword	.LC713
	.align	3
.LC715:
	.xword	g_event_report
	.align	3
.LC717:
	.xword	.LC716
	.align	3
.LC719:
	.xword	.LC718
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
.LC741:
	.xword	.LC740
	.align	3
.LC743:
	.xword	.LC742
	.align	3
.LC745:
	.xword	.LC744
	.align	3
.LC747:
	.xword	.LC746
	.align	3
.LC749:
	.xword	.LC748
	.align	3
.LC751:
	.xword	.LC750
	.align	3
.LC753:
	.xword	.LC752
	.align	3
.LC754:
	.xword	.LANCHOR1+216
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InitOldSlice
	.type	InitOldSlice, %function
InitOldSlice:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 20480
	mov	w4, 32
	ldr	w8, [x19, 14948]
	mov	w7, 23352
	movk	w7, 0x1, lsl 16
	mov	w6, 2
	strb	w4, [x0, 3234]
	add	x4, x0, 3072
	strb	wzr, [x0, 3232]
	mov	w5, 7
	strb	w6, [x0, 3233]
	mov	w3, 3
	str	w8, [x0, 3240]
	mov	w2, -1
	str	w5, [x0, 3244]
	mov	w1, 262144
	str	w7, [x0, 3248]
	stp	w3, w2, [x4, 184]
	stp	wzr, wzr, [x4, 192]
	str	w1, [x0, 3272]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	InitOldSlice, .-InitOldSlice
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_IsNewPicNal
	.type	H264_IsNewPicNal, %function
H264_IsNewPicNal:
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
	bls	.L2570
.L2569:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2570:
	ldr	x1, .LC755
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx2564
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2564:
	.section	.rodata
	.align	0
	.align	2
.L2564:
	.byte	(.L2563 - .Lrtx2564) / 4
	.byte	(.L2569 - .Lrtx2564) / 4
	.byte	(.L2569 - .Lrtx2564) / 4
	.byte	(.L2569 - .Lrtx2564) / 4
	.byte	(.L2563 - .Lrtx2564) / 4
	.byte	(.L2569 - .Lrtx2564) / 4
	.byte	(.L2565 - .Lrtx2564) / 4
	.byte	(.L2565 - .Lrtx2564) / 4
	.text
	.p2align 3
.L2565:
	ldr	w0, [x19]
	mov	w1, 1
	str	w1, [x19]
	cmp	w0, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2563:
	ldr	w0, [x19]
	cbz	w0, .L2566
.L2568:
	mov	w0, -1
	str	wzr, [x19]
.L2571:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2566:
	ldr	w20, [x20, 4]
	rev	w20, w20
	mov	w0, w20
	bl	ZerosMS_32
	cmp	w0, 15
	bhi	.L2568
	mov	w1, 31
	str	wzr, [x19]
	sub	w0, w1, w0, lsl 1
	lsr	w20, w20, w0
	cmp	w20, 1
	csetm	w0, ne
	b	.L2571
	.size	H264_IsNewPicNal, .-H264_IsNewPicNal
	.align	3
.LC755:
	.xword	.L2564
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FindZeroBitsInSeg
	.type	FindZeroBitsInSeg, %function
FindZeroBitsInSeg:
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
	ble	.L2592
	and	w1, w23, 63
	add	w0, w23, 63
	cmp	w1, wzr
	cset	w26, ne
	cmp	w23, wzr
	csel	w0, w0, w23, lt
	add	w26, w26, w0, asr 6
	cmp	w26, wzr
	ble	.L2585
	ldr	x20, .LC759
	mov	w25, w23
	ldr	x27, .LC758
	mov	w19, 0
	mov	w24, 64
	add	x20, x20, 1856
	.p2align 2
.L2584:
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
	cbnz	w1, .L2576
	cmp	x0, x20
	add	w19, w19, 1
	bne	.L2579
	b	.L2577
	.p2align 3
.L2580:
	add	w19, w19, 1
	beq	.L2577
.L2579:
	ldrb	w1, [x0, -1]!
	cmp	x0, x20
	cbz	w1, .L2580
.L2576:
	lsl	w0, w19, 3
.L2575:
	cmp	w19, w23
	mov	w1, 0
	bge	.L2581
	sxtw	x2, w23
	sub	x2, x2, #1
	sub	x19, x2, x19, sxtw
	ldrb	w2, [x22, x19]
	tbnz	x2, 0, .L2581
	lsr	w2, w2, 1
	mov	w1, 1
	.p2align 2
.L2583:
	tbnz	x2, 0, .L2581
	add	w1, w1, 1
	lsr	w2, w2, 1
	cmp	w1, 8
	bne	.L2583
.L2581:
	add	w0, w1, w0
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2577:
	subs	w26, w26, #1
	sub	w25, w25, #64
	bne	.L2584
	b	.L2576
.L2592:
	ldr	x1, .LC757
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
.L2585:
	mov	w0, 0
	mov	w19, 0
	b	.L2575
	.size	FindZeroBitsInSeg, .-FindZeroBitsInSeg
	.align	3
.LC757:
	.xword	.LC756
	.align	3
.LC758:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC759:
	.xword	.LANCHOR3
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FindTrailZeros
	.type	FindTrailZeros, %function
FindTrailZeros:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L2599
	ldr	x2, [x19, 632]
	ldr	w0, [x2, 76]
	cmp	w0, 1
	bls	.L2598
	ldr	w1, [x2, 48]
	ldr	x0, [x2, 40]
	bl	FindZeroBitsInSeg
	ldr	x2, [x19, 632]
	ldr	w1, [x2, 48]
	cmp	w0, w1, lsl 3
	bcs	.L2602
	cmn	w0, #1
	beq	.L2598
	add	w0, w0, 1
	str	w0, [x2, 80]
	ldr	x0, [x19, 632]
	ldr	w1, [x0, 76]
	cmp	w1, 1
	bls	.L2600
.L2603:
	ldr	w2, [x0, 48]
	mov	w20, 0
	ldr	w1, [x0, 80]
	cmp	w1, w2, lsl 3
	bcc	.L2594
	add	x2, x19, 65536
	ldr	w1, [x0, 68]
	ldr	w0, [x2, 116]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 632]
	mov	w1, 1
	str	xzr, [x0, 40]
	ldr	x0, [x19, 632]
	str	w1, [x0, 76]
	ldr	x0, [x19, 632]
	str	wzr, [x0, 80]
.L2594:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2602:
	add	x0, x19, 65536
	ldr	w1, [x2, 68]
	ldr	w0, [x0, 116]
	bl	SM_ReleaseStreamSeg
	ldr	x0, [x19, 632]
	mov	w1, 1
	str	xzr, [x0, 40]
	ldr	x0, [x19, 632]
	str	w1, [x0, 76]
	ldr	x2, [x19, 632]
.L2598:
	ldr	w1, [x2, 16]
	ldr	x0, [x2, 8]
	bl	FindZeroBitsInSeg
	add	w0, w0, 1
	ldr	x2, [x19, 632]
	str	w0, [x2, 80]
	ldr	x0, [x19, 632]
	ldr	w1, [x0, 76]
	cmp	w1, 1
	bhi	.L2603
.L2600:
	mov	w20, 0
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L2599:
	mov	w20, -1
	b	.L2594
	.size	FindTrailZeros, .-FindTrailZeros
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	CombinePacket
	.type	CombinePacket, %function
CombinePacket:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x19, 632]
	ldr	w0, [x0, 16]
	cmp	w0, 4095
	bhi	.L2604
	mov	x0, x19
	bl	CombinePacket.part.10
.L2604:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	CombinePacket, .-CombinePacket
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FindNaluArraySlot
	.type	FindNaluArraySlot, %function
FindNaluArraySlot:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	ldrb	w0, [x19, 641]
	cbz	w0, .L2611
	add	x0, x19, 745
	mov	x1, 1
	b	.L2609
	.p2align 3
.L2608:
	add	w1, w1, 1
	cmp	w1, 137
	beq	.L2614
.L2609:
	ldrb	w2, [x0]
	add	x0, x0, 104
	cbnz	w2, .L2608
	mov	w0, w1
.L2607:
	lsl	x2, x1, 3
	mov	w3, 1
	sub	x2, x2, x1
	lsl	x2, x2, x3
	sub	x1, x2, x1
	add	x1, x19, x1, lsl 3
	strb	w3, [x1, 641]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2614:
	mov	w0, -1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L2611:
	mov	w0, 0
	mov	x1, 0
	b	.L2607
	.size	FindNaluArraySlot, .-FindNaluArraySlot
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InquireSliceExtProperty
	.type	InquireSliceExtProperty, %function
InquireSliceExtProperty:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x0, x19, 12288
	mov	w1, 24
	add	x0, x0, 2600
	bl	BsGet
	ubfx	x2, x0, 23, 1
	ldr	x1, [x19, 632]
	strb	w2, [x1, 3]
	ldr	x3, [x19, 632]
	ldrb	w1, [x3, 3]
	cbnz	w1, .L2616
	add	x1, x19, 61440
	ldrb	w2, [x1, 1600]
.L2617:
	cmp	w2, 1
	bne	.L2618
	mov	w0, 5
	strb	w0, [x3, 2]
.L2618:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2616:
	add	x1, x19, 61440
	ubfx	x5, x0, 16, 6
	ubfx	x4, x0, 15, 1
	ubfx	x2, x0, 22, 1
	ubfx	x9, x0, 12, 3
	ubfx	x8, x0, 8, 4
	ubfx	x7, x0, 5, 3
	ubfx	x6, x0, 4, 1
	strb	w5, [x1, 1601]
	ubfx	x5, x0, 3, 1
	strb	w4, [x1, 1602]
	ubfx	x4, x0, 2, 1
	strb	w2, [x1, 1600]
	and	w0, w0, 3
	strb	w9, [x1, 1603]
	strb	w8, [x1, 1604]
	strb	w7, [x1, 1605]
	strb	w6, [x1, 1606]
	strb	w5, [x1, 1607]
	strb	w4, [x1, 1608]
	strb	w0, [x1, 1609]
	b	.L2617
	.size	InquireSliceExtProperty, .-InquireSliceExtProperty
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InquireSliceProperty
	.type	InquireSliceProperty, %function
InquireSliceProperty:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x23, x1
	mov	x20, x2
	bl	_mcount
	add	x21, x19, 20480
	add	x2, x19, 65536
	ldr	x1, .LC762
	mov	w0, 22
	ldr	w2, [x2, 60]
	sub	w2, w2, #1
	bl	dprint_vfmw
	str	wzr, [x20]
	str	wzr, [x23]
	mov	x0, x19
	ldr	x1, [x19, 632]
	ldrb	w2, [x1, 2]
	strb	w2, [x21, 1587]
	ldr	w1, [x1, 84]
	str	w1, [x21, 1616]
	bl	SliceCheck
	mov	w22, w0
	cbnz	w0, .L2623
	mov	x0, x19
	bl	ProcessSliceHeaderFirstPart
	cbnz	w0, .L2627
	ldrb	w3, [x21, 1589]
	mov	w1, 2208
	mov	w0, 4000
	mov	w2, 2
	cmp	w3, 1
	cset	w3, eq
	str	w3, [x23]
	ldr	w3, [x21, 1596]
	ldr	x4, [x19, 14968]
	ldr	x6, [x19, 14960]
	umull	x1, w3, w1
	ldr	w5, [x19, 14936]
	add	x1, x4, x1
	ldr	w4, [x19, 14932]
	ldr	w1, [x1, 28]
	umull	x0, w1, w0
	add	x0, x6, x0
	ldrb	w1, [x0, 18]
	ldr	w6, [x0, 3252]
	sub	w1, w2, w1
	ldr	w3, [x0, 3248]
	madd	w1, w6, w1, w1
	add	w2, w3, 1
	cmp	w1, w5
	ccmp	w2, w4, 0, eq
	bne	.L2628
.L2622:
	add	x19, x19, 24576
	ldr	w1, [x0, 3272]
	ldr	w0, [x19, 3712]
	add	w0, w0, 1
	cmp	w1, w0
	bls	.L2620
	mov	w0, 1
	str	w0, [x20]
.L2620:
	mov	w0, w22
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2628:
	mov	w1, 1
	str	w1, [x20]
	b	.L2622
	.p2align 3
.L2623:
	mov	w22, -1
	b	.L2620
	.p2align 3
.L2627:
	ldr	x1, .LC764
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L2620
	.size	InquireSliceProperty, .-InquireSliceProperty
	.align	3
.LC762:
	.xword	.LC761
	.align	3
.LC764:
	.xword	.LC763
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	HaveSliceToDec
	.type	HaveSliceToDec, %function
HaveSliceToDec:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 65536
	bl	_mcount
	ldr	w0, [x19, 40]
	ldr	x19, [sp, 16]
	cmp	w0, wzr
	csetm	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	HaveSliceToDec, .-HaveSliceToDec
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IsRefListWrong
	.type	IsRefListWrong, %function
IsRefListWrong:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 20480
	bl	_mcount
	ldr	w0, [x21, 1628]
	cbz	w0, .L2631
	mov	w19, 0
	add	x22, x20, 65536
	b	.L2635
	.p2align 3
.L2632:
	ldr	x0, [x0, 640]
	cbz	x0, .L2634
	ldr	w0, [x21, 1628]
	cmp	w0, w19
	bls	.L2659
.L2635:
	add	x1, x20, x19, sxtw 3
	ldr	w0, [x22, 116]
	add	w19, w19, 1
	ldr	x1, [x1, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	cbnz	x0, .L2632
.L2634:
	mov	w0, -1
.L2661:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2631:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2659:
	cbz	w0, .L2631
	ldr	w0, [x21, 1632]
	mov	w19, 0
	cbnz	w0, .L2648
	b	.L2631
	.p2align 3
.L2660:
	ldr	x0, [x0, 640]
	cbz	x0, .L2634
	ldr	w0, [x21, 1632]
	cmp	w0, w19
	bls	.L2631
.L2648:
	add	x1, x20, x19, sxtw 3
	ldr	w0, [x22, 116]
	add	w19, w19, 1
	ldr	x1, [x1, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	cbnz	x0, .L2660
	mov	w0, -1
	b	.L2661
	.size	IsRefListWrong, .-IsRefListWrong
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_Destroy
	.type	H264DEC_Destroy, %function
H264DEC_Destroy:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 12288
	add	x19, x20, 640
	bl	_mcount
	add	x22, x20, 65536
	mov	x0, x20
	add	x21, x21, 2600
	bl	ClearAllSlice
	add	x0, x20, 20480
	mov	w2, 23352
	ldr	w3, [x20, 14948]
	movk	w2, 0x1, lsl 16
	mov	w1, 7
	mov	w5, 2
	mov	w4, 32
	str	w3, [x0, 3240]
	str	w1, [x0, 3244]
	mov	w3, 3
	str	w2, [x0, 3248]
	mov	w1, 262144
	mov	w2, -1
	strb	wzr, [x0, 3232]
	strb	w5, [x0, 3233]
	strb	w4, [x0, 3234]
	str	wzr, [x0, 3264]
	str	wzr, [x0, 3268]
	str	w3, [x0, 3256]
	str	w2, [x0, 3260]
	str	w1, [x0, 3272]
	b	.L2664
	.p2align 3
.L2663:
	add	x19, x19, 104
	cmp	x21, x19
	beq	.L2684
.L2664:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L2663
	ldr	w0, [x22, 116]
	mov	x1, x19
	add	x19, x19, 104
	bl	ReleaseNAL
	cmp	x21, x19
	bne	.L2664
.L2684:
	ldr	w0, [x22, 52]
	cmp	w0, 1
	beq	.L2685
.L2665:
	add	x21, x20, 61440
	mov	w23, 0
	ldr	w0, [x21, 2800]
	cbz	w0, .L2670
	.p2align 2
.L2678:
	add	x19, x20, x23, uxtw 3
	add	w23, w23, 1
	add	x19, x19, 61440
	ldr	x0, [x19, 2808]
	mov	x1, x0
	cbz	x0, .L2668
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19, 2808]
.L2668:
	ldr	w0, [x21, 2800]
	cmp	w0, w23
	bhi	.L2678
.L2670:
	add	x20, x20, 28672
	add	x21, x21, 712
	add	x20, x20, 1160
	.p2align 2
.L2667:
	sub	x19, x20, #32
.L2672:
	ldr	x1, [x19]
	cbnz	x1, .L2686
.L2671:
	add	x19, x19, 8
	cmp	x20, x19
	bne	.L2672
	add	x20, x20, 808
	cmp	x20, x21
	bne	.L2667
	ldr	x1, .LC766
	mov	w0, 2
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2686:
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L2671
.L2685:
	str	wzr, [x22, 52]
	b	.L2665
	.size	H264DEC_Destroy, .-H264DEC_Destroy
	.align	3
.LC766:
	.xword	.LC765
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_RecycleImage
	.type	H264DEC_RecycleImage, %function
H264DEC_RecycleImage:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x21, x0
	mov	x0, x30
	mov	w22, w1
	add	x21, x21, 65536
	bl	_mcount
	ldr	x1, .LC768
	mov	w0, 2
	bl	dprint_vfmw
	ldr	w0, [x21, 116]
	mov	w1, w22
	bl	FSP_GetLogicFs
	cbz	x0, .L2692
	ldrsb	w1, [x0, 1]
	cbz	w1, .L2692
	add	x19, x0, 568
	add	x20, x0, 600
.L2690:
	ldr	x1, [x19]
	cbnz	x1, .L2697
.L2689:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L2690
	ldr	w0, [x21, 116]
	mov	w2, 0
	mov	w1, w22
	bl	FSP_SetDisplay
	mov	w0, 0
.L2688:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2697:
	ldr	w0, [x21, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L2689
	.p2align 3
.L2692:
	mov	w0, -1
	b	.L2688
	.size	H264DEC_RecycleImage, .-H264DEC_RecycleImage
	.align	3
.LC768:
	.xword	.LC767
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	OutputFrmToVO
	.type	OutputFrmToVO, %function
OutputFrmToVO:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	mov	w22, w2
	bl	_mcount
	cmp	x19, xzr
	ccmp	x21, xzr, 4, ne
	beq	.L2737
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, wzr
	ble	.L2701
	add	x0, x19, 65536
	ldr	w0, [x0, 44]
	cmp	w0, 2
	beq	.L2701
	add	x19, x19, 61440
	ldr	x0, [x19, 1584]
	cbz	x0, .L2703
	strb	wzr, [x0, 1]
	mov	w0, 1
	str	xzr, [x19, 1584]
.L2700:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2701:
	mov	x1, x21
	mov	x0, x19
	bl	CheckFrameStore
	cmn	w0, #3
	beq	.L2703
	add	x20, x19, 61440
	ldr	x1, [x20, 1584]
	cbz	x1, .L2704
	mov	x0, x19
	bl	GetImagePara
	ldr	x1, [x20, 1584]
	mov	x0, x19
	bl	CheckFrameStore
	cbz	w0, .L2738
	cmn	w0, #3
	beq	.L2711
	ldr	x1, .LC776
	mov	w3, w0
	mov	w2, 2046
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x20, 1584]
	mov	x0, x19
	ldr	w1, [x1, 164]
	bl	H264DEC_RecycleImage
.L2711:
	ldr	x0, [x20, 1584]
	strb	wzr, [x0, 1]
	ldr	x0, [x20, 1584]
	str	xzr, [x20, 1584]
	cmp	x21, x0
	beq	.L2739
.L2704:
	mov	x1, x21
	mov	x0, x19
	bl	GetImagePara
	mov	x1, x21
	mov	x0, x19
	bl	CheckFrameStore
	cbz	w0, .L2740
	cmn	w0, #3
	beq	.L2703
	ldr	x1, .LC776
	mov	w3, w0
	mov	w2, 2104
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x21, 164]
	mov	x0, x19
	bl	H264DEC_RecycleImage
	mov	w0, 2
	b	.L2700
	.p2align 3
.L2737:
	ldr	x1, .LC770
	mov	x3, x21
	mov	x2, x19
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, 2
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2703:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2740:
	add	x23, x19, 65536
	ldr	w1, [x21, 632]
	mov	w2, 1
	ldr	w0, [x23, 116]
	bl	FSP_SetDisplay
	ldr	w1, [x21, 632]
	ldr	w0, [x23, 116]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L2741
	cmp	w22, 1
	beq	.L2742
.L2715:
	ldr	w0, [x23, 116]
	mov	x4, x20
	add	x3, x19, 8
	mov	x2, x19
	mov	w1, 0
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L2743
	ldr	x0, [x19]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L2744
	str	wzr, [x20, 24]
.L2718:
	ldr	w1, [x23, 136]
	mov	w0, 0
	ldr	w2, [x23, 144]
	add	w1, w1, 2
	str	w1, [x23, 136]
	add	w2, w2, 1
	str	w2, [x23, 144]
	b	.L2700
	.p2align 3
.L2738:
	ldr	x0, [x20, 1584]
	add	x23, x19, 65536
	mov	w2, 1
	ldr	w1, [x0, 632]
	ldr	w0, [x23, 116]
	bl	FSP_SetDisplay
	ldr	x1, [x20, 1584]
	ldr	w0, [x23, 116]
	ldr	w1, [x1, 632]
	bl	FSP_GetFsImagePtr
	mov	x24, x0
	cbz	x0, .L2745
	ldr	x0, [x20, 1584]
	cmp	x0, x21
	ccmp	w22, 1, 0, eq
	beq	.L2746
.L2707:
	ldr	w0, [x23, 116]
	mov	x4, x24
	add	x3, x19, 8
	mov	x2, x19
	mov	w1, 0
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L2747
	ldr	x0, [x19]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L2748
	str	wzr, [x24, 24]
.L2710:
	ldr	w0, [x23, 136]
	ldr	w1, [x23, 144]
	add	w0, w0, 2
	str	w0, [x23, 136]
	add	w1, w1, 1
	str	w1, [x23, 144]
	b	.L2711
	.p2align 3
.L2739:
	mov	w0, 0
	b	.L2700
	.p2align 3
.L2742:
	str	w22, [x0, 188]
	b	.L2715
	.p2align 3
.L2744:
	mov	x1, x20
	mov	x0, x19
	bl	SetFrmRepeatCount.part.1
	b	.L2718
	.p2align 3
.L2748:
	mov	x1, x24
	mov	x0, x19
	bl	SetFrmRepeatCount.part.1
	b	.L2710
.L2743:
	ldr	x1, .LC774
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x21, 632]
	mov	w2, 0
	ldr	w0, [x23, 116]
	bl	FSP_SetDisplay
	mov	w1, 1
	mov	x0, x19
	bl	ClearAll
	mov	w0, -1
	b	.L2700
.L2747:
	ldr	x1, .LC774
	mov	w2, w0
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x0, [x20, 1584]
	mov	w2, 0
	strb	wzr, [x0, 1]
	ldr	x1, [x20, 1584]
	ldr	w0, [x23, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
	str	xzr, [x20, 1584]
	mov	w1, 1
	mov	x0, x19
	bl	ClearAll
	mov	w0, -1
	b	.L2700
	.p2align 3
.L2746:
	mov	w0, 1
	str	w0, [x24, 188]
	b	.L2707
.L2741:
	ldr	x1, .LC778
	mov	w2, 2074
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2700
.L2745:
	ldr	x1, .LC772
	mov	w2, 2017
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2700
	.size	OutputFrmToVO, .-OutputFrmToVO
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	OutputFrmFromDPB
	.type	OutputFrmFromDPB, %function
OutputFrmFromDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	w21, w1
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, x21, uxtw 3
	bl	_mcount
	mov	w2, 0
	mov	x0, x20
	ldr	x1, [x19, 27904]
	strb	wzr, [x1, 4]
	ldr	x1, [x19, 27904]
	bl	OutputFrmToVO
	mov	w22, w0
	ldr	x0, [x19, 27904]
	ldrb	w0, [x0, 2]
	cbnz	w0, .L2750
	mov	w1, w21
	mov	x0, x20
	bl	RemoveFrameStoreOutDPB
.L2750:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.size	OutputFrmFromDPB, .-OutputFrmFromDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	FlushDPB
	.type	FlushDPB, %function
FlushDPB:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	str	x21, [sp, 32]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 24576
	mov	w21, 0
	bl	_mcount
	ldr	w0, [x19, 3716]
	cbnz	w0, .L2769
	b	.L2753
	.p2align 3
.L2754:
	ldr	w0, [x19, 3716]
	cmp	w0, w21
	bls	.L2753
.L2769:
	add	x0, x20, x21, uxtw 3
	add	w21, w21, 1
	ldr	x1, [x0, 27904]
	ldrb	w0, [x1, 2]
	cbz	w0, .L2754
	mov	x0, x20
	bl	UnMarkFrameStoreRef
	ldr	w0, [x19, 3716]
	cmp	w0, w21
	bhi	.L2769
	.p2align 2
.L2753:
	mov	x0, x20
	bl	RemoveUnUsedFrameStore
	cbnz	w0, .L2753
	.p2align 2
.L2757:
	ldr	w6, [x19, 3716]
	cbz	w6, .L2774
	ldr	w0, [x19, 3712]
	cmp	w6, w0
	bls	.L2758
	str	w0, [x19, 3716]
	cbz	w0, .L2764
	mov	w6, w0
.L2758:
	add	x2, x19, 3328
	mov	w5, 2147483647
	mov	w0, 0
	mov	w1, 0
	.p2align 2
.L2761:
	ldr	x4, [x2]
	add	x2, x2, 8
	ldr	w3, [x4, 648]
	cmp	w3, w5
	bge	.L2760
	ldrb	w4, [x4, 4]
	cmp	w4, wzr
	csel	w5, w5, w3, eq
	csel	w1, w1, w0, eq
.L2760:
	add	w0, w0, 1
	cmp	w6, w0
	bhi	.L2761
	mov	x0, x20
	bl	OutputFrmFromDPB
	cmn	w0, #1
	bne	.L2757
.L2762:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L2764:
	mov	w1, 0
	mov	x0, x20
	bl	OutputFrmFromDPB
	cmn	w0, #1
	bne	.L2757
	b	.L2762
	.p2align 3
.L2774:
	mov	w0, 0
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.size	FlushDPB, .-FlushDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AdaptiveMemMark
	.type	AdaptiveMemMark, %function
AdaptiveMemMark:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	add	x26, x21, 61440
	add	x25, x21, 20480
	bl	_mcount
	ldr	x24, .LC779
	add	x23, x26, 112
	mov	w27, 1
	ldr	x22, [x26, 760]
	add	x20, x22, 8
	add	x22, x22, 2008
	.p2align 2
.L2788:
	ldr	w0, [x20, -4]
	cmp	w0, 6
	bhi	.L2776
	ldrb	w0, [x24,w0,uxtw]
	adr	x1, .Lrtx2778
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2778:
	.section	.rodata
	.align	0
	.align	2
.L2778:
	.byte	(.L2777 - .Lrtx2778) / 4
	.byte	(.L2779 - .Lrtx2778) / 4
	.byte	(.L2780 - .Lrtx2778) / 4
	.byte	(.L2781 - .Lrtx2778) / 4
	.byte	(.L2782 - .Lrtx2778) / 4
	.byte	(.L2783 - .Lrtx2778) / 4
	.byte	(.L2784 - .Lrtx2778) / 4
	.text
	.p2align 3
.L2783:
	add	x28, x21, 24576
	mov	w19, 0
	ldr	w0, [x28, 3720]
	cbz	w0, .L2787
	.p2align 2
.L2795:
	add	x1, x21, x19, uxtw 3
	mov	x0, x21
	add	w19, w19, 1
	ldr	x1, [x1, 28032]
	bl	UnMarkFrameStoreRef
	ldr	w0, [x28, 3720]
	cmp	w19, w0
	bcc	.L2795
.L2787:
	mov	x0, x21
	bl	UpdateReflist
	mov	w1, 0
	mov	x0, x21
	bl	UpdateMaxLTFrmIdx
	mov	x0, x21
	bl	UpdateLTReflist
	str	w27, [x25, 728]
	.p2align 2
.L2776:
	add	x20, x20, 20
	cmp	x20, x22
	bne	.L2788
	.p2align 2
.L2777:
	ldr	w1, [x25, 728]
	mov	w0, 0
	cmp	w1, 1
	beq	.L2798
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2782:
	ldr	w1, [x20, 12]
	mov	x0, x21
	bl	UpdateMaxLTFrmIdx
	mov	x0, x21
	bl	UpdateLTReflist
	b	.L2776
	.p2align 3
.L2781:
	ldr	w2, [x20]
	mov	x1, x23
	ldr	w3, [x20, 8]
	mov	x0, x21
	bl	MarkSTToLTRef
	mov	x0, x21
	bl	UpdateReflist
	mov	x0, x21
	bl	UpdateLTReflist
	b	.L2776
	.p2align 3
.L2780:
	ldr	w2, [x20, 4]
	mov	x0, x21
	mov	x1, x23
	bl	UnMarkLTRef
	mov	x0, x21
	bl	UpdateLTReflist
	b	.L2776
	.p2align 3
.L2779:
	ldr	w2, [x20]
	mov	x0, x21
	mov	x1, x23
	bl	UnMarkSTRef
	mov	x0, x21
	bl	UpdateReflist
	b	.L2776
	.p2align 3
.L2784:
	ldr	w2, [x20, 8]
	mov	x1, x23
	mov	x0, x21
	bl	MarkCurrPicLT
	b	.L2776
	.p2align 3
.L2798:
	ldrb	w0, [x26, 115]
	str	wzr, [x26, 768]
	cmp	w0, 1
	beq	.L2791
	cbz	w0, .L2792
	cmp	w0, 2
	bne	.L2790
	str	wzr, [x26, 780]
	str	wzr, [x26, 792]
	str	wzr, [x25, 740]
	str	wzr, [x25, 748]
.L2790:
	mov	x0, x21
	bl	FlushDPB
	cmp	w0, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L2792:
	ldr	w2, [x26, 780]
	ldr	w0, [x26, 788]
	ldr	w1, [x26, 792]
	sub	w0, w0, w2
	str	w0, [x26, 788]
	sub	w1, w1, w2
	str	w1, [x26, 792]
	cmp	w0, w1
	csel	w0, w0, w1, le
	str	w0, [x26, 780]
	str	w0, [x26, 784]
	ldr	w2, [x25, 748]
	ldr	w0, [x25, 736]
	ldr	w1, [x25, 740]
	sub	w0, w0, w2
	str	w0, [x25, 736]
	sub	w1, w1, w2
	str	w1, [x25, 740]
	cmp	w0, w1
	csel	w0, w0, w1, le
	str	w0, [x25, 744]
	str	w0, [x25, 748]
	b	.L2790
.L2791:
	str	wzr, [x26, 780]
	str	wzr, [x26, 788]
	str	wzr, [x25, 736]
	str	wzr, [x25, 748]
	b	.L2790
	.size	AdaptiveMemMark, .-AdaptiveMemMark
	.align	3
.LC779:
	.xword	.L2778
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	IDRMemMarking
	.type	IDRMemMarking, %function
IDRMemMarking:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x21, x0
	mov	x0, x30
	add	x24, x21, 61440
	bl	_mcount
	ldr	x0, [x24, 760]
	ldrb	w0, [x0, 1]
	cbz	w0, .L2800
	add	x23, x21, 24576
	mov	w20, 0
	add	x22, x21, 65536
	mov	w25, 1
	ldr	w0, [x23, 3716]
	cbnz	w0, .L2815
	b	.L2807
	.p2align 3
.L2804:
	strb	wzr, [x0, 682]
	strb	wzr, [x0, 681]
	ldr	x0, [x19, 27904]
	strb	wzr, [x0, 722]
	strb	wzr, [x0, 721]
	ldr	x0, [x19, 27904]
	strb	wzr, [x0, 762]
	strb	wzr, [x0, 761]
	ldr	x0, [x19, 27904]
	strb	wzr, [x0, 2]
	ldr	x1, [x19, 27904]
	ldr	w0, [x22, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetRef
	ldr	x0, [x19, 27904]
	strb	wzr, [x0, 1]
	ldr	x0, [x19, 27904]
	strb	wzr, [x0, 4]
	ldr	x0, [x19, 27904]
	str	w25, [x0, 652]
	ldr	x1, [x19, 27904]
	ldr	w0, [x1, 672]
	add	x0, x0, 4096
	add	x0, x21, x0, lsl 2
	str	wzr, [x0, 11928]
	ldr	w1, [x1, 632]
	ldr	w0, [x22, 116]
	bl	FSP_GetDisplay
	cmp	w0, 3
	mov	w2, 0
	beq	.L2805
	ldr	x1, [x19, 27904]
	ldr	w0, [x22, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
.L2805:
	str	xzr, [x19, 27904]
	str	xzr, [x19, 28032]
	str	xzr, [x19, 28160]
	ldr	w0, [x23, 3716]
	cmp	w0, w20
	bls	.L2807
.L2815:
	add	x19, x21, x20, uxtw 3
	mov	w2, 0
	add	w20, w20, 1
	ldr	x0, [x19, 27904]
	ldr	w1, [x0, 800]
	cmp	w1, 1
	bne	.L2804
	str	w1, [x24, 2788]
	b	.L2804
	.p2align 3
.L2807:
	str	wzr, [x23, 3716]
	ldr	w0, [x24, 2776]
	cbz	w0, .L2803
	sub	w1, w0, #1
	add	x0, x24, 2704
	add	x1, x1, 12288
	add	x1, x1, 3749
	add	x1, x21, x1, lsl 2
	.p2align 2
.L2810:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L2810
.L2803:
	mov	x0, x21
	bl	UpdateReflist
	mov	x0, x21
	bl	UpdateLTReflist
	ldr	x0, [x24, 760]
	ldrb	w0, [x0, 2]
	cbnz	w0, .L2821
	mov	w0, 1
	str	wzr, [x23, 3728]
	strb	w0, [x24, 117]
	mov	w0, 0
	strb	wzr, [x24, 116]
.L2811:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L2821:
	mov	w0, 1
	str	w0, [x23, 3728]
	strb	w0, [x24, 116]
	mov	w0, 0
	strb	wzr, [x24, 117]
	str	wzr, [x24, 772]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L2800:
	mov	x0, x21
	bl	FlushDPB
	cbnz	w0, .L2812
	add	x23, x21, 24576
	b	.L2803
.L2812:
	mov	w0, -1
	b	.L2811
	.size	IDRMemMarking, .-IDRMemMarking
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	Marking
	.type	Marking, %function
Marking:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 61440
	bl	_mcount
	add	x0, x20, 20480
	str	wzr, [x0, 728]
	ldrb	w1, [x19, 115]
	cmp	w1, 2
	cset	w1, eq
	str	w1, [x0, 732]
	ldrb	w0, [x19, 118]
	cmp	w0, 5
	beq	.L2823
	ldr	w0, [x19, 796]
	cbnz	w0, .L2824
	ldr	x0, [x20]
	ldr	w1, [x0, 640]
	cmp	w1, 1
	beq	.L2848
.L2824:
	ldr	x0, [x19, 760]
	ldrb	w0, [x0, 3]
	cbz	w0, .L2849
	ldr	w0, [x19, 776]
	cbnz	w0, .L2850
.L2831:
	mov	w0, 0
.L2828:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2849:
	ldr	w0, [x19, 776]
	cbz	w0, .L2831
.L2830:
	ldrb	w1, [x19, 113]
	cbnz	w1, .L2835
	add	x1, x20, 24576
	add	x2, x20, 16384
	ldr	w4, [x1, 3724]
	ldr	w2, [x2, 1836]
	ldr	w3, [x1, 3720]
	sub	w1, w2, w4
	cmp	w3, w1
	beq	.L2851
	strb	wzr, [x19, 116]
.L2833:
	cbz	w0, .L2831
.L2835:
	ldrb	w1, [x19, 116]
	mov	w0, 0
	cmp	w1, wzr
	cset	w1, eq
	strb	w1, [x19, 117]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2850:
	mov	x0, x20
	bl	AdaptiveMemMark
	cbnz	w0, .L2825
.L2832:
	ldrb	w0, [x19, 118]
	cmp	w0, 5
	bne	.L2852
	ldr	w0, [x19, 776]
	b	.L2833
	.p2align 3
.L2848:
	ldr	w1, [x0, 636]
	cbnz	w1, .L2824
	ldr	w1, [x0, 644]
	cbnz	w1, .L2824
	ldr	w0, [x0, 604]
	add	w0, w0, 1024
	cmp	w0, 2048
	bls	.L2824
	.p2align 2
.L2823:
	mov	x0, x20
	bl	IDRMemMarking
	cbz	w0, .L2832
.L2825:
	mov	w0, -1
	b	.L2828
	.p2align 3
.L2851:
	mov	x0, x20
	bl	SlidingWinMark.part.2
	strb	wzr, [x19, 116]
	ldr	w0, [x19, 776]
	b	.L2833
.L2852:
	ldr	w0, [x19, 776]
	cbz	w0, .L2831
	ldr	x1, [x19, 760]
	ldrb	w1, [x1, 3]
	cbnz	w1, .L2835
	b	.L2830
	.size	Marking, .-Marking
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DirectOutput
	.type	DirectOutput, %function
DirectOutput:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 61440
	bl	_mcount
	ldrb	w0, [x19, 115]
	cmp	w0, 1
	beq	.L2855
	cbz	w0, .L2856
	cmp	w0, 2
	beq	.L2857
	mov	w21, 0
.L2854:
	mov	w0, w21
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2857:
	ldr	x0, [x19, 752]
	mov	w1, 3
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 113]
	cbnz	w0, .L2873
	ldr	x1, [x19, 1584]
	cbz	x1, .L2874
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	mov	w21, w0
	cbnz	w0, .L2939
.L2875:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 792]
.L2876:
	ldr	x0, [x19, 1584]
	mov	w22, 2
	ldrb	w3, [x19, 114]
	mov	x2, 624
	add	x1, x19, 128
	add	x23, x20, 65536
	strb	w3, [x0]
	ldr	x0, [x19, 1584]
	strb	w22, [x0, 1]
	ldr	x0, [x19, 1584]
	strb	wzr, [x0, 4]
	ldr	x0, [x19, 1584]
	str	wzr, [x0, 652]
	ldr	x0, [x19, 1584]
	strb	wzr, [x0, 2]
	ldr	x0, [x19, 1584]
	ldr	w3, [x19, 796]
	str	w3, [x0, 660]
	ldr	x0, [x19, 1584]
	str	wzr, [x0, 656]
	ldr	x0, [x19, 1584]
	add	x0, x0, 8
	bl	memcpy
	ldr	x1, [x19, 1584]
	mov	w0, 2
	ldr	w2, [x19, 804]
	str	w2, [x1, 776]
	ldr	x1, [x19, 1584]
	ldr	w2, [x1, 776]
	str	w2, [x1, 664]
	str	w2, [x1, 696]
	ldr	x1, [x19, 1584]
	strb	w22, [x1, 760]
	ldrb	w2, [x19, 119]
	ldr	x1, [x19, 1584]
	cmp	w2, 1
	csel	w0, wzr, w0, ne
	strb	w0, [x1, 3]
	ldr	w22, [x23, 48]
	cmp	w22, 1
	bne	.L2854
.L2933:
	ldr	x1, [x19, 1584]
	mov	w2, 0
	ldr	w0, [x23, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetRef
	ldr	x1, [x19, 1584]
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	mov	w21, w0
	ldr	x0, [x19, 1584]
	cbz	x0, .L2884
	ldr	x1, [x19, 752]
	mov	w2, w22
	ldr	w0, [x23, 116]
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
.L2884:
	str	xzr, [x19, 1584]
.L2941:
	mov	w0, w21
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2856:
	ldr	x0, [x19, 752]
	strb	wzr, [x0, 680]
	ldr	x1, [x19, 1584]
	cbz	x1, .L2929
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	cbnz	w0, .L2940
.L2859:
	ldr	x0, [x19, 1584]
	cbz	x0, .L2929
	ldr	x0, [x19, 752]
	add	x22, x20, 65536
	mov	w2, 1
	ldr	w1, [x0, 632]
	ldr	w0, [x22, 116]
	bl	FSP_ClearLogicFs
.L2858:
	ldr	x0, [x19, 752]
	mov	w3, 3
	str	x0, [x19, 1584]
	mov	x2, 624
	add	x1, x19, 128
	str	x0, [x0, 712]
	ldr	x0, [x19, 1584]
	ldr	x4, [x19, 752]
	str	x4, [x0, 752]
	ldr	x0, [x19, 1584]
	ldr	x4, [x19, 752]
	str	x4, [x0, 792]
	ldr	x0, [x19, 1584]
	ldrb	w4, [x19, 114]
	strb	w4, [x0]
	ldr	x0, [x19, 1584]
	strb	w3, [x0, 1]
	ldr	x0, [x19, 1584]
	strb	wzr, [x0, 2]
	ldr	x0, [x19, 1584]
	strb	wzr, [x0, 4]
	ldr	x0, [x19, 1584]
	str	wzr, [x0, 652]
	ldr	x0, [x19, 1584]
	add	x0, x0, 8
	bl	memcpy
	ldr	x1, [x19, 1584]
	mov	w0, 3
	ldr	w3, [x19, 804]
	mov	w2, 0
	str	w3, [x1, 664]
	str	w3, [x1, 696]
	ldr	x1, [x19, 1584]
	strb	wzr, [x1, 680]
	ldr	x1, [x19, 1584]
	ldr	w3, [x19, 796]
	str	w3, [x1, 660]
	ldr	x1, [x19, 1584]
	ldrb	w3, [x19, 121]
	str	w3, [x1, 656]
	ldrb	w3, [x19, 119]
	ldr	x1, [x19, 1584]
	cmp	w3, 1
	csel	w0, wzr, w0, ne
	strb	w0, [x1, 3]
.L2936:
	ldr	x1, [x19, 1584]
	ldr	w0, [x22, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetRef
	ldr	x1, [x19, 1584]
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	mov	w21, w0
	ldr	x0, [x19, 1584]
	cbz	x0, .L2884
	ldr	x1, [x19, 752]
	mov	w2, 1
	ldr	w0, [x22, 116]
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
	str	xzr, [x19, 1584]
	b	.L2941
	.p2align 3
.L2855:
	ldr	x0, [x19, 752]
	mov	w1, 3
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 113]
	cbnz	w0, .L2862
	ldr	x1, [x19, 1584]
	cbz	x1, .L2863
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	mov	w21, w0
	cbnz	w0, .L2942
.L2864:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 752]
.L2865:
	ldr	x0, [x19, 1584]
	mov	w22, 1
	ldrb	w3, [x19, 114]
	add	x1, x19, 128
	mov	x2, 624
	add	x23, x20, 65536
	strb	w3, [x0]
	ldr	x0, [x19, 1584]
	strb	w22, [x0, 1]
	ldr	x0, [x19, 1584]
	strb	wzr, [x0, 4]
	ldr	x0, [x19, 1584]
	str	wzr, [x0, 652]
	ldr	x0, [x19, 1584]
	strb	wzr, [x0, 2]
	ldr	x0, [x19, 1584]
	ldr	w3, [x19, 796]
	str	w3, [x0, 660]
	ldr	x0, [x19, 1584]
	str	wzr, [x0, 656]
	ldr	x0, [x19, 1584]
	add	x0, x0, 8
	bl	memcpy
	ldr	x0, [x19, 1584]
	ldr	w1, [x19, 804]
	str	w1, [x0, 736]
	ldr	x0, [x19, 1584]
	ldr	w1, [x0, 736]
	str	w1, [x0, 664]
	str	w1, [x0, 696]
	ldr	x0, [x19, 1584]
	strb	w22, [x0, 720]
	ldrb	w1, [x19, 119]
	ldr	x0, [x19, 1584]
	cmp	w1, 1
	cset	w1, eq
	strb	w1, [x0, 3]
	ldr	w22, [x23, 48]
	cmp	w22, 1
	beq	.L2933
	mov	w0, w21
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2873:
	ldr	x0, [x19, 1584]
	cbz	x0, .L2943
.L2879:
	ldr	x27, [x0, 608]
	mov	x2, 624
	ldp	w24, w23, [x0, 16]
	ldp	w22, w21, [x0, 24]
	ldr	x26, [x0, 616]
	add	x1, x19, 128
	ldr	x25, [x0, 624]
	add	x0, x0, 8
	bl	memcpy
	ldr	x0, [x19, 728]
	cmn	x0, #1
	ccmn	x27, #1, 4, eq
	beq	.L2880
	ldr	x0, [x19, 1584]
	str	x27, [x0, 608]
.L2880:
	ldr	x0, [x19, 736]
	ldr	x1, [x19, 1584]
	cmp	x0, x26
	csel	x0, x0, x26, ls
	str	x0, [x1, 616]
	ldr	x0, [x19, 744]
	ldr	x1, [x19, 1584]
	cmp	x0, x25
	csel	x0, x0, x25, ls
	str	x0, [x1, 624]
	ldr	w0, [x19, 136]
	ldr	x1, [x19, 1584]
	cmp	w0, w24
	csel	w0, w0, w24, ls
	str	w0, [x1, 16]
	ldr	w0, [x19, 140]
	ldr	x1, [x19, 1584]
	cmp	w0, w23
	csel	w0, w0, w23, ls
	str	w0, [x1, 20]
	ldr	w0, [x19, 144]
	ldr	x1, [x19, 1584]
	cmp	w0, w22
	csel	w0, w0, w22, ls
	str	w0, [x1, 24]
	ldr	w0, [x19, 148]
	ldr	x1, [x19, 1584]
	cmp	w0, w21
	csel	w0, w0, w21, ls
	str	w0, [x1, 28]
	ldr	x1, [x19, 1584]
	ldrb	w0, [x1]
	cbz	w0, .L2881
	ldrb	w0, [x19, 114]
.L2881:
	strb	w0, [x1]
	mov	w4, 2
	mov	w3, 3
	ldr	x1, [x19, 1584]
	ldrb	w0, [x1, 1]
	orr	w0, w0, 2
	strb	w0, [x1, 1]
	ldr	x1, [x19, 1584]
	ldr	w0, [x19, 796]
	ldr	w2, [x1, 660]
	cmp	w0, w2
	csel	w0, w0, w2, cs
	str	w0, [x1, 660]
	ldr	x0, [x19, 1584]
	strb	w4, [x0, 760]
	ldr	x0, [x19, 1584]
	strb	w3, [x0, 680]
	ldr	x0, [x19, 1584]
	ldr	w1, [x19, 804]
	str	w1, [x0, 776]
	ldr	x1, [x19, 1584]
	ldr	w2, [x1, 736]
	ldr	w0, [x1, 776]
	add	w0, w2, w0
	lsr	w0, w0, 1
	str	w0, [x1, 696]
	ldr	x0, [x19, 1584]
	ldr	w1, [x0, 696]
	str	w1, [x0, 664]
	ldr	x0, [x19, 1584]
	ldr	x1, [x0, 792]
	str	x1, [x0, 712]
	ldrb	w0, [x19, 119]
	ldr	x1, [x19, 1584]
	cmp	w0, 1
	beq	.L2944
.L2882:
	ldrb	w0, [x1, 3]
.L2883:
	strb	w0, [x1, 3]
	add	x22, x20, 65536
	mov	w2, 0
	b	.L2936
	.p2align 3
.L2862:
	ldr	x0, [x19, 1584]
	cbz	x0, .L2945
.L2867:
	ldr	x27, [x0, 608]
	mov	x2, 624
	ldp	w24, w23, [x0, 16]
	ldp	w22, w21, [x0, 24]
	ldr	x26, [x0, 616]
	add	x1, x19, 128
	ldr	x25, [x0, 624]
	add	x0, x0, 8
	bl	memcpy
	ldr	x0, [x19, 728]
	cmn	x0, #1
	ccmn	x27, #1, 4, eq
	beq	.L2868
	ldr	x0, [x19, 1584]
	str	x27, [x0, 608]
.L2868:
	ldr	x0, [x19, 736]
	ldr	x1, [x19, 1584]
	cmp	x0, x26
	csel	x0, x0, x26, ls
	str	x0, [x1, 616]
	ldr	x0, [x19, 744]
	ldr	x1, [x19, 1584]
	cmp	x0, x25
	csel	x0, x0, x25, ls
	str	x0, [x1, 624]
	ldr	w0, [x19, 136]
	ldr	x1, [x19, 1584]
	cmp	w0, w24
	csel	w0, w0, w24, ls
	str	w0, [x1, 16]
	ldr	w0, [x19, 140]
	ldr	x1, [x19, 1584]
	cmp	w0, w23
	csel	w0, w0, w23, ls
	str	w0, [x1, 20]
	ldr	w0, [x19, 144]
	ldr	x1, [x19, 1584]
	cmp	w0, w22
	csel	w0, w0, w22, ls
	str	w0, [x1, 24]
	ldr	w0, [x19, 148]
	ldr	x1, [x19, 1584]
	cmp	w0, w21
	csel	w0, w0, w21, ls
	str	w0, [x1, 28]
	ldr	x1, [x19, 1584]
	ldrb	w0, [x1]
	cbz	w0, .L2869
	ldrb	w0, [x19, 114]
.L2869:
	strb	w0, [x1]
	mov	w4, 1
	mov	w3, 3
	ldr	x1, [x19, 1584]
	ldrb	w0, [x1, 1]
	orr	w0, w0, 1
	strb	w0, [x1, 1]
	ldr	x1, [x19, 1584]
	ldr	w0, [x19, 796]
	ldr	w2, [x1, 660]
	cmp	w0, w2
	csel	w0, w0, w2, cs
	str	w0, [x1, 660]
	ldr	x0, [x19, 1584]
	strb	w4, [x0, 720]
	ldr	x0, [x19, 1584]
	strb	w3, [x0, 680]
	ldr	x0, [x19, 1584]
	ldr	w1, [x19, 804]
	str	w1, [x0, 736]
	ldr	x1, [x19, 1584]
	ldr	w2, [x1, 736]
	ldr	w0, [x1, 776]
	add	w0, w2, w0
	lsr	w0, w0, w4
	str	w0, [x1, 696]
	ldr	x0, [x19, 1584]
	ldr	w1, [x0, 696]
	str	w1, [x0, 664]
	ldr	x0, [x19, 1584]
	ldr	x1, [x0, 752]
	str	x1, [x0, 712]
	ldrb	w0, [x19, 119]
	ldr	x1, [x19, 1584]
	cmp	w0, 1
	bne	.L2882
	ldrb	w0, [x1, 3]
	orr	w0, w0, 1
	uxtb	w0, w0
	b	.L2883
	.p2align 3
.L2929:
	add	x22, x20, 65536
	b	.L2858
	.p2align 3
.L2940:
	ldr	x2, .LC780
	mov	w4, w0
	ldr	x1, .LC782
	mov	w3, 2704
	add	x2, x2, 232
	mov	w0, 22
	bl	dprint_vfmw
	b	.L2859
	.p2align 3
.L2944:
	ldrb	w0, [x1, 3]
	orr	w0, w0, 2
	uxtb	w0, w0
	b	.L2883
	.p2align 3
.L2863:
	ldr	x0, [x19, 752]
	mov	w21, 0
	str	x0, [x19, 1584]
	str	x0, [x0, 752]
	b	.L2865
	.p2align 3
.L2874:
	ldr	x0, [x19, 752]
	mov	w21, 0
	str	x0, [x19, 1584]
	str	x0, [x0, 792]
	b	.L2876
	.p2align 3
.L2942:
	ldr	x2, .LC780
	mov	w4, w0
	ldr	x1, .LC782
	mov	w3, 2756
	add	x2, x2, 232
	mov	w0, 22
	bl	dprint_vfmw
	b	.L2864
	.p2align 3
.L2939:
	ldr	x2, .LC780
	mov	w4, w0
	ldr	x1, .LC782
	mov	w3, 2898
	add	x2, x2, 232
	mov	w0, 22
	bl	dprint_vfmw
	b	.L2875
	.p2align 3
.L2943:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 792]
	ldr	x0, [x19, 1584]
	b	.L2879
	.p2align 3
.L2945:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 752]
	ldr	x0, [x19, 1584]
	b	.L2867
	.size	DirectOutput, .-DirectOutput
	.align	3
.LC780:
	.xword	.LANCHOR1
	.align	3
.LC782:
	.xword	.LC781
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_GetRemainImg
	.type	H264DEC_GetRemainImg, %function
H264DEC_GetRemainImg:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x20, x21, 24576
	mov	w19, 0
	bl	_mcount
	ldr	w0, [x20, 3716]
	cbnz	w0, .L2966
	b	.L2948
	.p2align 3
.L2949:
	ldr	w0, [x20, 3716]
	cmp	w0, w19
	bls	.L2948
.L2966:
	add	x0, x21, x19, uxtw 3
	add	w19, w19, 1
	ldr	x1, [x0, 27904]
	ldrb	w0, [x1, 2]
	cbz	w0, .L2949
	mov	x0, x21
	bl	UnMarkFrameStoreRef
	ldr	w0, [x20, 3716]
	cmp	w0, w19
	bhi	.L2966
	.p2align 2
.L2948:
	mov	x0, x21
	bl	RemoveUnUsedFrameStore
	cbnz	w0, .L2948
	mov	w23, -1
	.p2align 2
.L2952:
	ldr	w5, [x20, 3716]
	cbz	w5, .L2972
.L2958:
	ldr	w0, [x20, 3712]
	cmp	w5, w0
	bls	.L2953
	str	w0, [x20, 3716]
	cbz	w0, .L2960
	mov	w5, w0
.L2953:
	add	x1, x20, 3328
	mov	w4, 2147483647
	mov	w0, 0
	mov	w19, 0
	.p2align 2
.L2956:
	ldr	x3, [x1]
	add	x1, x1, 8
	ldr	w2, [x3, 648]
	cmp	w2, w4
	bge	.L2955
	ldrb	w3, [x3, 4]
	cmp	w3, wzr
	csel	w4, w4, w2, eq
	csel	w19, w19, w0, eq
.L2955:
	add	w0, w0, 1
	cmp	w5, w0
	bhi	.L2956
.L2954:
	add	x22, x21, x19, uxtw 3
	mov	x0, x21
	ldr	x1, [x22, 27904]
	strb	wzr, [x1, 4]
	ldr	w2, [x20, 3716]
	ldr	x1, [x22, 27904]
	bl	OutputFrmToVO
	mov	w23, w0
	ldr	x0, [x22, 27904]
	ldrb	w0, [x0, 2]
	cbnz	w0, .L2952
	mov	w1, w19
	mov	x0, x21
	bl	RemoveFrameStoreOutDPB
	ldr	w5, [x20, 3716]
	cbnz	w5, .L2958
.L2972:
	cbz	w23, .L2959
	add	x0, x21, 8
	bl	GetVoLastImageID
	mov	w23, w0
.L2959:
	mov	w0, w23
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L2960:
	mov	w19, 0
	b	.L2954
	.size	H264DEC_GetRemainImg, .-H264DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	InitDPB.part.12, %function
InitDPB.part.12:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x23, x21, 24576
	add	x20, x21, 65536
	add	x19, x23, 3328
	add	x22, x23, 3456
	bl	_mcount
	.p2align 2
.L2977:
	ldr	x0, [x19]
	mov	w2, 1
	cbz	x0, .L2974
	ldr	w1, [x0, 632]
	ldr	w0, [x20, 116]
	bl	FSP_ClearLogicFs
.L2974:
	ldr	x0, [x19, 128]
	mov	w2, 1
	cbz	x0, .L2975
	ldr	w1, [x0, 632]
	ldr	w0, [x20, 116]
	bl	FSP_ClearLogicFs
.L2975:
	ldr	x0, [x19, 256]
	mov	w2, 1
	cbz	x0, .L2976
	ldr	w1, [x0, 632]
	ldr	w0, [x20, 116]
	bl	FSP_ClearLogicFs
.L2976:
	str	xzr, [x19, 128]
	str	xzr, [x19, 256]
	str	xzr, [x19], 8
	cmp	x19, x22
	bne	.L2977
	add	x0, x21, 16384
	add	x1, x23, 3584
	add	x19, x21, 61440
	ldr	w0, [x0, 1864]
	stp	wzr, wzr, [x1, 136]
	sub	w0, w0, #1
	stp	w0, wzr, [x1, 128]
	str	wzr, [x23, 3728]
	ldr	x1, [x19, 1584]
	cbz	x1, .L2979
	add	x20, x21, 65536
	ldr	w0, [x20, 44]
	cmp	w0, 2
	beq	.L3009
.L2980:
	ldr	w0, [x20, 116]
	mov	w2, 1
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
	ldr	x0, [x19, 752]
	cbz	x0, .L2983
	ldr	x2, [x19, 1584]
	ldr	w1, [x0, 632]
	ldr	w0, [x2, 632]
	cmp	w0, w1
	beq	.L2983
	ldr	w0, [x20, 116]
	mov	w2, 1
	bl	FSP_ClearLogicFs
.L2983:
	str	xzr, [x19, 1584]
.L2979:
	ldr	x3, .LC785
	mov	w1, 0
	mov	x2, 392
	add	x0, x23, 3736
	ldr	x3, [x3, 96]
	blr	x3
	str	wzr, [x23, 3996]
	ldr	w0, [x23, 3712]
	add	x1, x19, 2772
	str	w0, [x23, 3992]
	add	x0, x19, 2704
	.p2align 2
.L2984:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L2984
	add	x2, x21, 28672
	add	x1, x2, 32
	add	x0, x2, 296
	.p2align 2
.L2985:
	str	xzr, [x1], 8
	cmp	x1, x0
	bne	.L2985
	add	x21, x21, 20480
	add	x1, x2, 560
	str	wzr, [x21, 1628]
	.p2align 2
.L2986:
	str	xzr, [x0], 8
	cmp	x1, x0
	bne	.L2986
	str	wzr, [x21, 1632]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L3009:
	mov	w2, 0
	mov	x0, x21
	bl	OutputFrmToVO
	cbnz	w0, .L3010
.L2981:
	ldr	x1, [x19, 1584]
	cbz	x1, .L2979
	b	.L2980
	.p2align 3
.L3010:
	ldr	x2, .LC783
	mov	w4, w0
	ldr	x1, .LC784
	mov	w3, 5669
	add	x2, x2, 248
	mov	w0, 22
	bl	dprint_vfmw
	b	.L2981
	.size	InitDPB.part.12, .-InitDPB.part.12
	.align	3
.LC783:
	.xword	.LANCHOR1
	.align	3
.LC784:
	.xword	.LC781
	.align	3
.LC785:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InitDPB
	.type	InitDPB, %function
InitDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	cbz	x19, .L3013
	mov	x0, x19
	bl	ClearAllSlice
	mov	x0, x19
	bl	InitDPB.part.12
.L3012:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3013:
	mov	w0, -1
	b	.L3012
	.size	InitDPB, .-InitDPB
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearDPB
	.type	ClearDPB, %function
ClearDPB:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	mov	x0, x19
	bl	FlushDPB
	cbz	w0, .L3015
	ldr	x1, .LC787
	mov	w3, w0
	mov	w2, 501
	mov	w0, 0
	bl	dprint_vfmw
.L3015:
	mov	x0, x19
	bl	InitDPB
	add	x1, x19, 61440
	add	x19, x19, 65536
	mov	w0, 0
	str	wzr, [x1, 2788]
	str	wzr, [x19, 120]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	ClearDPB, .-ClearDPB
	.align	3
.LC787:
	.xword	.LC786
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecVDM
	.type	DecVDM, %function
DecVDM:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x22, x0
	mov	x0, x30
	add	x21, x22, 61440
	bl	_mcount
	ldrb	w0, [x21, 115]
	cbz	w0, .L3020
	ldrb	w0, [x21, 113]
	add	x19, x22, 65536
	cmp	w0, 1
	beq	.L3020
.L3021:
	ldr	w20, [x19, 104]
	ldr	w0, [x19, 44]
	ubfx	x20, x20, 16, 3
	cmp	w20, w0
	beq	.L3022
	sub	w0, w0, #2
	cmp	w0, 1
	ccmp	w20, 2, 4, ls
	beq	.L3022
	cmp	w20, 1
	ccmp	w20, 4, 4, hi
	beq	.L3104
.L3022:
	cmp	w20, 2
	str	w20, [x19, 44]
	beq	.L3025
	cmp	w20, 3
	beq	.L3026
	cmp	w20, 1
	beq	.L3105
	ldr	w0, [x19, 40]
	cbz	w0, .L3029
	cmp	w20, 4
	ldr	w20, [x21, 796]
	bne	.L3035
	cbnz	w20, .L3035
	ldr	w0, [x21, 2792]
	cbnz	w0, .L3036
	mov	w0, 2
	str	w0, [x21, 2792]
	b	.L3048
	.p2align 3
.L3020:
	add	x19, x22, 65536
	ldp	w0, w1, [x19, 140]
	add	w0, w0, 1
	cmp	w0, w1
	csel	w0, w0, w1, ge
	str	w0, [x19, 140]
	sub	w0, w0, w1
	ldr	x1, [x22]
	str	w0, [x1, 576]
	b	.L3021
	.p2align 3
.L3026:
	mov	x0, x22
	mov	w19, -1
	bl	ClearAllSlice
	add	x1, x22, 20480
	ldr	w4, [x22, 14948]
	add	x6, x1, 3072
	mov	w3, 23352
	movk	w3, 0x1, lsl 16
	mov	w2, 2
	strb	wzr, [x1, 3232]
	mov	w0, 7
	strb	w2, [x1, 3233]
	mov	w2, 262144
	str	w0, [x1, 3244]
	mov	w0, 32
	str	w4, [x1, 3240]
	str	w3, [x1, 3248]
	stp	w20, w19, [x6, 184]
	stp	wzr, wzr, [x6, 192]
	strb	w0, [x1, 3234]
	mov	x0, x22
	str	w2, [x1, 3272]
	bl	ClearDPB
	mov	w0, w19
.L3097:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3105:
	ldr	w20, [x21, 796]
	cmp	w20, 2
	bne	.L3031
	ldr	w0, [x21, 776]
	cbz	w0, .L3032
.L3031:
	ldr	w0, [x19, 40]
	cbz	w0, .L3029
.L3035:
	cmp	w20, 2
	beq	.L3106
	cmp	w20, 1
	bne	.L3048
	str	w20, [x21, 2792]
.L3048:
	ldr	w2, [x19, 80]
	mov	w0, 2
	ldr	x1, .LC797
	ldrb	w3, [x21, 115]
	sub	w2, w2, #1
	bl	dprint_vfmw
	ldr	w0, [x21, 824]
	ldr	w1, [x21, 820]
	str	w0, [x21, 3640]
	mov	w0, 1
	str	w1, [x21, 3644]
	strb	w0, [x21, 2842]
	ldr	w0, [x19, 40]
	str	w0, [x21, 3844]
	ldr	w0, [x19, 80]
	cbz	w0, .L3039
	ldr	w4, [x21, 796]
	cbnz	w4, .L3040
	ldr	x0, .LC792
	ldr	x5, [x0]
	cbz	x5, .L3041
	ldr	x1, [x21, 3848]
	cbz	x1, .L3042
	mov	w2, 0
	.p2align 2
.L3043:
	ldp	w3, w0, [x1, 8]
	ldr	x1, [x1, 4056]
	add	w0, w3, w0
	add	w0, w0, 7
	add	w2, w2, w0, lsr 3
	mov	w4, w2
	cbnz	x1, .L3043
.L3042:
	add	x2, x29, 64
	ldr	w0, [x19, 116]
	mov	w1, 20
	mov	w3, 4
	str	w4, [x2, -8]!
	blr	x5
	ldr	x1, [x22]
.L3044:
	add	x2, x21, 2840
	str	x2, [x22, 616]
	ldr	w0, [x1, 864]
	cmp	w0, 1
	beq	.L3107
	cmp	w0, 2
	beq	.L3108
.L3047:
	ldr	w2, [x1, 592]
	mov	w0, 0
	str	w2, [x19, 16]
	ldr	w1, [x1, 632]
	str	w1, [x19, 20]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3025:
	ldr	w0, [x21, 796]
	cbnz	w0, .L3032
	ldr	w0, [x19, 40]
	cbz	w0, .L3029
	ldr	x0, [x19, 32]
	ldr	w0, [x0, 48]
	cbz	w0, .L3048
	mov	w1, 1
	mov	x0, x22
	strb	w1, [x21, 119]
	mov	w19, -1
	bl	ClearAllSlice
	add	x2, x22, 20480
	mov	w3, 23352
	movk	w3, 0x1, lsl 16
	mov	w0, 7
	ldr	w4, [x22, 14948]
	ldr	x1, .LC791
	str	w0, [x2, 3244]
	mov	w0, 3
	str	w3, [x2, 3248]
	mov	w3, 32
	str	w0, [x2, 3256]
	mov	w0, 0
	strb	w3, [x2, 3234]
	mov	w3, 262144
	strb	wzr, [x2, 3232]
	strb	w20, [x2, 3233]
	str	w4, [x2, 3240]
	str	w19, [x2, 3260]
	str	wzr, [x2, 3264]
	str	wzr, [x2, 3268]
	str	w3, [x2, 3272]
	bl	dprint_vfmw
	mov	w0, w19
	b	.L3097
	.p2align 3
.L3029:
	mov	x0, x22
	mov	w20, -1
	bl	ClearAllSlice
	add	x0, x22, 20480
	mov	w5, 7
	ldr	w7, [x22, 14948]
	mov	w1, 23352
	ldr	x2, .LC792
	movk	w1, 0x1, lsl 16
	str	w5, [x0, 3244]
	add	x5, x0, 3072
	str	w1, [x0, 3248]
	mov	w6, 2
	strb	wzr, [x0, 3232]
	mov	w3, 32
	strb	w6, [x0, 3233]
	mov	w1, 262144
	strb	w3, [x0, 3234]
	str	w7, [x0, 3240]
	ldr	x4, [x2]
	mov	w2, 3
	stp	wzr, wzr, [x5, 192]
	stp	w2, w20, [x5, 184]
	str	w1, [x0, 3272]
	cbz	x4, .L3100
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3106:
	ldr	w0, [x21, 2792]
	cmp	w0, 2
	bne	.L3048
	ldr	x0, [x22]
	ldr	w0, [x0, 12]
	cbnz	w0, .L3048
	ldr	x0, .LC793
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L3048
	ldr	x1, .LC795
	mov	w0, 1
	ldr	w3, [x21, 776]
	ldr	w2, [x21, 780]
	bl	dprint_vfmw
	mov	x0, x22
	bl	ClearAllSlice
	ldr	x3, .LC792
	add	x0, x22, 20480
	mov	w1, 23352
	ldr	w2, [x22, 14948]
	movk	w1, 0x1, lsl 16
	mov	w6, 7
	ldr	x4, [x3]
	mov	w5, 32
	mov	w3, 3
	str	w2, [x0, 3240]
	str	w1, [x0, 3248]
	mov	w2, -1
	mov	w1, 262144
	strb	wzr, [x0, 3232]
	strb	w20, [x0, 3233]
	strb	w5, [x0, 3234]
	str	w6, [x0, 3244]
	str	w3, [x0, 3256]
	str	wzr, [x0, 3264]
	str	wzr, [x0, 3268]
	str	w2, [x0, 3260]
	str	w1, [x0, 3272]
	cbz	x4, .L3038
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3038:
	ldr	x1, [x21, 752]
	mov	w2, 0
	ldr	w0, [x19, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
	.p2align 2
.L3100:
	mov	w0, -1
	b	.L3097
	.p2align 3
.L3039:
	ldr	x1, [x22]
	b	.L3047
	.p2align 3
.L3040:
	add	x0, x22, 24576
	ldr	w0, [x0, 3716]
	cbnz	w0, .L3041
	ldr	x1, [x22]
	ldr	w0, [x1, 12]
	cmp	w0, 2
	beq	.L3044
	ldr	x0, .LC792
	ldr	x4, [x0]
	cbz	x4, .L3045
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3045:
	mov	x0, x22
.L3102:
	bl	ClearAllSlice
	add	x1, x22, 20480
	ldr	w8, [x22, 14948]
	mov	w7, 23352
	mov	w6, 2
	movk	w7, 0x1, lsl 16
	mov	w0, -1
	mov	w5, 7
	mov	w4, 32
	mov	w3, 3
	mov	w2, 262144
	strb	wzr, [x1, 3232]
	strb	w6, [x1, 3233]
	str	w8, [x1, 3240]
	str	w7, [x1, 3248]
	b	.L3099
	.p2align 3
.L3041:
	ldr	x1, [x22]
	b	.L3044
	.p2align 3
.L3107:
	cbz	x2, .L3047
	ldr	w20, [x21, 796]
	cmp	w20, 2
	bne	.L3047
	str	xzr, [x22, 616]
	mov	w2, 0
	ldr	x1, [x21, 752]
	ldr	w0, [x19, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
	mov	x0, x22
	b	.L3101
	.p2align 3
.L3108:
	ldr	w0, [x1, 868]
	cmp	w0, wzr
	ccmp	x2, xzr, 4, gt
	beq	.L3047
	ldr	w20, [x21, 796]
	cmp	w20, 2
	bne	.L3047
	str	xzr, [x22, 616]
	mov	w2, 0
	ldr	x1, [x21, 752]
	ldr	w0, [x19, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
	ldr	x2, [x22]
	mov	x0, x22
	ldr	w1, [x2, 868]
	sub	w1, w1, #1
	str	w1, [x2, 868]
.L3101:
	bl	ClearAllSlice
	add	x1, x22, 20480
	ldr	w7, [x22, 14948]
	mov	w6, 23352
	mov	w0, -1
	movk	w6, 0x1, lsl 16
	mov	w5, 7
	mov	w4, 32
	mov	w3, 3
	mov	w2, 262144
	strb	wzr, [x1, 3232]
	strb	w20, [x1, 3233]
	str	w7, [x1, 3240]
	str	w6, [x1, 3248]
	.p2align 2
.L3099:
	strb	w4, [x1, 3234]
	add	x4, x1, 3072
	str	w5, [x1, 3244]
	stp	w3, w0, [x4, 184]
	stp	wzr, wzr, [x4, 192]
	str	w2, [x1, 3272]
	b	.L3097
	.p2align 3
.L3036:
	mov	w0, 1
	str	w0, [x21, 2792]
	b	.L3048
.L3104:
	ldr	x1, .LC789
	mov	w2, w20
	mov	w0, 2
	mov	w21, -1
	bl	dprint_vfmw
	str	w20, [x19, 44]
	mov	x0, x22
	bl	ClearAllSlice
	add	x1, x22, 20480
	ldr	w4, [x22, 14948]
	add	x7, x1, 3072
	mov	w2, 2
	mov	w3, 23352
	mov	w0, 7
	strb	w2, [x1, 3233]
	movk	w3, 0x1, lsl 16
	str	w0, [x1, 3244]
	mov	w2, 32
	strb	wzr, [x1, 3232]
	mov	w0, 3
	strb	w2, [x1, 3234]
	mov	w2, 262144
	str	w4, [x1, 3240]
	str	w3, [x1, 3248]
	stp	w21, wzr, [x7, 188]
	stp	wzr, w2, [x7, 196]
	str	w0, [x1, 3256]
	mov	x0, x22
	bl	ClearDPB
	mov	w0, w21
	b	.L3097
.L3032:
	mov	w1, 1
	mov	x0, x22
	strb	w1, [x21, 119]
	b	.L3102
	.size	DecVDM, .-DecVDM
	.align	3
.LC789:
	.xword	.LC788
	.align	3
.LC791:
	.xword	.LC790
	.align	3
.LC792:
	.xword	g_event_report
	.align	3
.LC793:
	.xword	g_allow_B_before_P
	.align	3
.LC795:
	.xword	.LC794
	.align	3
.LC797:
	.xword	.LC796
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_FlushDecoder
	.type	H264_FlushDecoder, %function
H264_FlushDecoder:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	mov	w20, 1
	bl	_mcount
	add	x1, x19, 20480
	mov	w2, 1
	mov	x0, x19
	strb	w2, [x1, 1589]
	str	wzr, [x1, 1636]
	str	w20, [x19, 624]
	bl	DecVDM
	cbz	w0, .L3109
	ldr	x1, .LC799
	mov	w0, w20
	bl	dprint_vfmw
.L3109:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	H264_FlushDecoder, .-H264_FlushDecoder
	.align	3
.LC799:
	.xword	.LC798
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ReceivePacket
	.type	ReceivePacket, %function
ReceivePacket:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x1
	mov	x20, x0
	mov	x0, x30
	add	x21, x20, 65536
	bl	_mcount
	ldr	x1, .LC801
	mov	w0, 7
	ldp	w3, w4, [x19, 16]
	ldr	x2, [x19, 8]
	bl	dprint_vfmw
	str	wzr, [x21, 148]
	ldr	x0, [x20]
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L3179
.L3115:
	ldr	x0, [x20, 632]
	cbz	x0, .L3120
	ldrb	w1, [x19]
	cmp	w1, 1
	beq	.L3180
.L3118:
	ldr	x1, .LC803
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbz	x1, .L3120
	ldr	w0, [x21, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3120:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	ble	.L3122
	ldr	x0, [x19, 8]
	cbz	x0, .L3122
	ldr	w0, [x19, 16]
	cbz	w0, .L3122
	ldrb	w0, [x20, 641]
	cbz	w0, .L3140
	add	x1, x20, 745
	mov	x0, 1
	b	.L3125
	.p2align 3
.L3124:
	add	w0, w0, 1
	cmp	w0, 137
	beq	.L3138
.L3125:
	ldrb	w2, [x1]
	add	x1, x1, 104
	cbnz	w2, .L3124
	mov	w2, w0
.L3123:
	lsl	x1, x0, 3
	mov	w3, 1
	sub	x1, x1, x0
	cmn	w2, #1
	lsl	x1, x1, x3
	sub	x0, x1, x0
	add	x0, x20, x0, lsl 3
	strb	w3, [x0, 641]
	beq	.L3138
	sxtw	x1, w2
	mov	w2, 1
	lsl	x0, x1, 3
	sub	x0, x0, x1
	lsl	x0, x0, 1
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x1, x0, 640
	add	x0, x20, x0
	add	x1, x20, x1
	str	x1, [x20, 632]
	ldr	x1, [x19, 8]
	str	x1, [x0, 648]
	ldr	w1, [x19, 20]
	str	w1, [x0, 656]
	ldr	w1, [x19, 16]
	str	w1, [x0, 660]
	ldr	x0, [x20, 632]
	str	wzr, [x0, 28]
	ldr	x0, [x20, 632]
	ldr	w1, [x19, 24]
	str	w1, [x0, 36]
	ldr	x0, [x20, 632]
	ldr	x1, [x19, 32]
	str	x1, [x0, 96]
	ldr	x0, [x20, 632]
	str	w2, [x0, 76]
	ldr	x0, [x20, 632]
	strb	wzr, [x0]
	ldr	x0, [x20, 632]
	ldrb	w1, [x19]
	strb	w1, [x0, 4]
	ldr	x0, [x20, 632]
	b	.L3130
	.p2align 3
.L3180:
	ldr	w2, [x0, 76]
	cmp	w2, 1
	bhi	.L3118
	ldrb	w2, [x0, 4]
	cmp	w2, 1
	bne	.L3119
	b	.L3118
	.p2align 3
.L3179:
	ldr	w0, [x0, 872]
	cbz	w0, .L3115
	ldr	x0, [x20, 632]
	cbz	x0, .L3120
	ldrb	w1, [x19]
.L3119:
	strb	w1, [x0, 4]
	ldr	x0, [x20]
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L3181
.L3132:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	ble	.L3133
	ldr	x1, [x19, 8]
	cbz	x1, .L3133
	ldr	w0, [x19, 16]
	cbz	w0, .L3133
	ldr	x0, [x20, 632]
	ldr	w2, [x0, 76]
	cmp	w2, 1
	bls	.L3182
.L3134:
	mov	w1, 1
	strb	w1, [x0, 4]
	ldr	w1, [x19, 24]
	ldr	w0, [x21, 116]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC806
	ldr	x4, [x0]
	cbz	x4, .L3176
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3176:
	ldr	x0, [x20, 632]
.L3130:
	ldrb	w1, [x19, 1]
	strb	w1, [x0, 5]
	ldr	x0, [x20, 632]
	ldp	x21, x22, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldrb	w0, [x0, 4]
	ldp	x29, x30, [sp], 48
	cmp	w0, wzr
	csetm	w0, eq
	ret
	.p2align 3
.L3122:
	ldr	x1, .LC808
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 24]
	ldr	w0, [x21, 116]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC806
	ldr	x4, [x0]
	cbz	x4, .L3178
.L3177:
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3178:
	mov	w0, -1
.L3185:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3133:
	ldr	x0, [x20, 632]
	b	.L3134
	.p2align 3
.L3181:
	ldr	w0, [x0, 872]
	cbnz	w0, .L3176
	b	.L3132
	.p2align 3
.L3182:
	ubfiz	x2, x2, 5, 32
	add	x0, x0, x2
	str	x1, [x0, 8]
	ldr	x1, [x20, 632]
	ldr	w2, [x19, 20]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	w2, [x0, 16]
	ldr	x1, [x20, 632]
	ldr	w2, [x19, 16]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	w2, [x0, 20]
	ldr	x1, [x20, 632]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	wzr, [x0, 28]
	ldr	x1, [x20, 632]
	ldr	w2, [x19, 24]
	ldr	w0, [x1, 76]
	add	x0, x1, x0, lsl 5
	str	w2, [x0, 36]
	ldr	x1, [x20, 632]
	ldr	w0, [x1, 76]
	add	w0, w0, 1
	str	w0, [x1, 76]
	ldr	x0, [x20, 632]
	ldr	w1, [x0, 16]
	cmp	w1, 4095
	bls	.L3183
.L3135:
	mov	w1, 1
	str	w1, [x21, 148]
	b	.L3130
	.p2align 3
.L3183:
	mov	x0, x20
	bl	CombinePacket.part.10
	ldr	x0, [x20, 632]
	b	.L3135
.L3138:
	ldr	x1, .LC805
	mov	w0, 0
	add	x22, x20, 12288
	add	x19, x20, 640
	add	x22, x22, 2600
	bl	dprint_vfmw
	b	.L3127
	.p2align 3
.L3126:
	add	x19, x19, 104
	cmp	x19, x22
	beq	.L3184
.L3127:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L3126
	ldr	w0, [x21, 116]
	mov	x1, x19
	bl	ReleaseNAL
	b	.L3126
.L3184:
	mov	x0, x20
	mov	w19, -1
	bl	ClearAllSlice
	add	x1, x20, 20480
	mov	w0, 7
	ldr	w4, [x20, 14948]
	mov	w2, 2
	mov	w3, 23352
	str	w0, [x1, 3244]
	add	x0, x1, 3072
	strb	w2, [x1, 3233]
	movk	w3, 0x1, lsl 16
	str	w4, [x1, 3240]
	mov	w2, 32
	strb	wzr, [x1, 3232]
	strb	w2, [x1, 3234]
	mov	w2, 262144
	str	w3, [x1, 3248]
	stp	w19, wzr, [x0, 188]
	stp	wzr, w2, [x0, 196]
	mov	w0, 3
	str	w0, [x1, 3256]
	mov	x0, x20
	bl	ClearDPB
	ldr	x0, .LC806
	ldr	x4, [x0]
	cbnz	x4, .L3177
	mov	w0, -1
	b	.L3185
.L3140:
	mov	w2, 0
	mov	x0, 0
	b	.L3123
	.size	ReceivePacket, .-ReceivePacket
	.align	3
.LC801:
	.xword	.LC800
	.align	3
.LC803:
	.xword	.LC802
	.align	3
.LC805:
	.xword	.LC804
	.align	3
.LC806:
	.xword	g_event_report
	.align	3
.LC808:
	.xword	.LC807
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	ClearAll
	.type	ClearAll, %function
ClearAll:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x20, x0
	mov	w21, w1
	mov	x0, x30
	add	x1, x20, 20480
	mov	x23, x1
	add	x19, x20, 640
	stp	x1, x20, [x29, 96]
	bl	_mcount
	add	x22, x20, 65536
	ldr	x1, .LC810
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	ClearAllSlice
	mov	x1, x20
	mov	w2, 23352
	movk	w2, 0x1, lsl 16
	mov	w0, 7
	add	x20, x20, 12288
	ldr	w3, [x1, 14948]
	mov	w1, 2
	str	w3, [x23, 3240]
	add	x20, x20, 2600
	strb	w1, [x23, 3233]
	mov	w3, 32
	mov	w1, -1
	str	w0, [x23, 3244]
	str	w2, [x23, 3248]
	mov	w0, 262144
	mov	w2, 3
	strb	wzr, [x23, 3232]
	str	w1, [x23, 3260]
	str	wzr, [x23, 3264]
	str	wzr, [x23, 3268]
	strb	w3, [x23, 3234]
	str	w2, [x23, 3256]
	str	w0, [x23, 3272]
	b	.L3188
	.p2align 3
.L3187:
	add	x19, x19, 104
	cmp	x19, x20
	beq	.L3216
.L3188:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L3187
	ldr	w0, [x22, 116]
	mov	x1, x19
	add	x19, x19, 104
	bl	ReleaseNAL
	cmp	x19, x20
	bne	.L3188
.L3216:
	ldr	x19, [x29, 104]
	mov	x0, x19
	bl	InitDPB
	cbz	w21, .L3189
	ldr	w0, [x22, 116]
	add	x1, x19, 8
	bl	FSP_ClearNotInVoQueue
.L3190:
	ldr	x0, [x29, 104]
	mov	w25, 0
	mov	w28, -1
	mov	w21, 1
	add	x19, x0, 28672
	mov	w27, 17
	mov	w26, 16
	mov	w23, 2
	add	x19, x19, 560
	.p2align 2
.L3193:
	add	x0, x19, 512
	strb	wzr, [x19]
	strb	wzr, [x19, 1]
	add	x20, x19, 568
	strb	wzr, [x19, 2]
	add	x24, x19, 600
	strb	wzr, [x19, 4]
	strb	wzr, [x19, 680]
	strb	w21, [x19, 720]
	strb	w23, [x19, 760]
	str	w25, [x19, 164]
	stp	wzr, w28, [x0, 124]
	stp	wzr, wzr, [x0, 132]
	str	w21, [x19, 652]
	str	w27, [x19, 668]
	str	w26, [x19, 672]
	str	x19, [x19, 712]
	str	x19, [x19, 752]
	str	x19, [x19, 792]
.L3192:
	ldr	x1, [x20]
	cbnz	x1, .L3217
.L3191:
	add	x20, x20, 8
	cmp	x20, x24
	bne	.L3192
	add	w25, w25, 1
	add	x19, x19, 808
	cmp	w25, 40
	bne	.L3193
	ldr	x0, [x29, 104]
	add	x21, x0, 61440
	add	x19, x21, 688
	add	x20, x21, 720
.L3195:
	ldr	x1, [x19]
	cbnz	x1, .L3218
.L3194:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L3195
	ldr	w0, [x21, 2800]
	mov	w20, 0
	cbz	w0, .L3199
	.p2align 2
.L3206:
	ldr	x0, [x29, 104]
	add	x19, x0, x20, uxtw 3
	add	w20, w20, 1
	add	x19, x19, 61440
	ldr	x0, [x19, 2808]
	mov	x1, x0
	cbz	x0, .L3197
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19, 2808]
.L3197:
	ldr	w0, [x21, 2800]
	cmp	w0, w20
	bhi	.L3206
.L3199:
	str	wzr, [x21, 2800]
	mov	x2, 1648
	str	wzr, [x22, 92]
	mov	w1, 0
	str	wzr, [x21, 2780]
	ldp	x19, x6, [x29, 96]
	str	wzr, [x22, 40]
	str	wzr, [x22, 48]
	add	x3, x6, 24576
	str	wzr, [x22, 100]
	str	wzr, [x21, 2788]
	str	wzr, [x22, 120]
	str	wzr, [x21, 2792]
	str	xzr, [x6, 632]
	ldr	x0, .LC811
	strb	wzr, [x3, 1184]
	str	wzr, [x3, 3188]
	strb	wzr, [x19, 3276]
	ldr	x4, [x0, 96]
	add	x0, x19, 1584
	strb	wzr, [x3, 1185]
	strb	wzr, [x19, 3277]
	blr	x4
	mov	w1, 2
	mov	w0, 1
	strb	w1, [x19, 1584]
	strb	w0, [x19, 1589]
	mov	w0, 256
	strb	wzr, [x21, 112]
	str	w0, [x21, 1560]
	mov	w0, 0
	strb	wzr, [x21, 1552]
	str	wzr, [x21, 1556]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3217:
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x20]
	b	.L3191
.L3218:
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L3194
.L3189:
	ldr	x0, [x29, 104]
	add	x0, x0, 8
	bl	ResetVoQueue
	ldr	w0, [x22, 116]
	bl	FSP_EmptyInstance
	b	.L3190
	.size	ClearAll, .-ClearAll
	.align	3
.LC810:
	.xword	.LC809
	.align	3
.LC811:
	.xword	vfmw_Osal_Func_Ptr_S
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InsertFrmInDPB
	.type	InsertFrmInDPB, %function
InsertFrmInDPB:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x23, x0
	mov	x0, x30
	mov	w20, w1
	mov	x19, x2
	bl	_mcount
	cmp	w20, 15
	bhi	.L3314
	ldrb	w21, [x19, 3]
	cmp	w21, 1
	beq	.L3223
	cbz	w21, .L3224
	cmp	w21, 2
	bne	.L3315
	ldr	x0, [x19, 640]
	mov	w1, 3
	uxtw	x22, w20
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 1]
	cbz	w0, .L3242
	add	x20, x23, x22, lsl 3
	ldr	x0, [x20, 27904]
	cbz	x0, .L3242
	strb	w21, [x0, 760]
	ldr	x0, [x20, 27904]
	ldr	x1, [x19, 640]
	str	x1, [x0, 792]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x19, 4]
	strb	w1, [x0, 761]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x19, 5]
	strb	w1, [x0, 762]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 680]
	str	w1, [x0, 772]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 684]
	str	w1, [x0, 780]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 692]
	str	w1, [x0, 776]
	ldr	x0, [x20, 27904]
	ldr	w2, [x0, 736]
	ldr	w1, [x0, 776]
	add	w1, w2, w1
	str	w1, [x0, 664]
	str	w1, [x0, 696]
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1]
	cbz	w0, .L3248
	ldrb	w0, [x19, 2]
.L3248:
	strb	w0, [x1]
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 1]
	orr	w0, w0, 2
	strb	w0, [x1, 1]
	ldrb	w0, [x19, 7]
	ldr	x1, [x20, 27904]
	cmp	w0, 1
	beq	.L3316
	ldrb	w0, [x1, 3]
.L3250:
	strb	w0, [x1, 3]
	mov	x2, 624
	add	x1, x19, 16
	ldr	x3, [x20, 27904]
	add	x0, x3, 8
	ldr	x4, [x3, 608]
	str	x4, [x29, 104]
	ldp	w26, w25, [x3, 16]
	ldp	w24, w21, [x3, 24]
	ldr	x28, [x3, 616]
	ldr	x27, [x3, 624]
	bl	memcpy
	ldr	x0, [x19, 616]
	ldr	x4, [x29, 104]
	cmn	x0, #1
	ccmn	x4, #1, 4, eq
	beq	.L3251
	ldr	x0, [x20, 27904]
	str	x4, [x0, 608]
.L3251:
	ldr	x0, [x19, 624]
	ldr	x1, [x20, 27904]
	cmp	x0, x28
	csel	x0, x0, x28, ls
	str	x0, [x1, 616]
	ldr	x0, [x19, 632]
	ldr	x1, [x20, 27904]
	cmp	x0, x27
	csel	x0, x0, x27, ls
	str	x0, [x1, 624]
	ldr	w0, [x19, 24]
	ldr	x1, [x20, 27904]
	cmp	w0, w26
	csel	w0, w0, w26, ls
	str	w0, [x1, 16]
	ldr	w0, [x19, 28]
	ldr	x1, [x20, 27904]
	cmp	w0, w25
	csel	w0, w0, w25, ls
	str	w0, [x1, 20]
	ldr	w0, [x19, 32]
	ldr	x1, [x20, 27904]
	cmp	w0, w24
	csel	w0, w0, w24, ls
	str	w0, [x1, 24]
	ldr	w0, [x19, 36]
	ldr	x1, [x20, 27904]
	cmp	w0, w21
	csel	w0, w0, w21, ls
	str	w0, [x1, 28]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3252
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3252
.L3253:
	ldr	x1, [x20, 27904]
	ldr	w0, [x1, 740]
	ldr	w2, [x1, 780]
	cmp	w0, w2
	csel	w0, w0, w2, cs
	str	w0, [x1, 660]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 668]
	str	w1, [x0, 648]
	ldr	x0, [x20, 27904]
	bl	CombineFldsToFrm
	ldr	x1, [x20, 27904]
	ldr	w0, [x1, 672]
	ldr	w1, [x1, 772]
	add	x0, x0, 4096
	add	x0, x23, x0, lsl 2
	str	w1, [x0, 12120]
	b	.L3241
	.p2align 3
.L3224:
	uxtw	x22, w20
	ldr	x1, [x19, 640]
	mov	w2, 3
	mov	w0, 3
	add	x20, x23, x22, lsl 3
	str	x1, [x20, 27904]
	ldrb	w3, [x19, 2]
	strb	w3, [x1]
	ldr	x1, [x20, 27904]
	strb	w2, [x1, 1]
	ldr	x2, [x19, 640]
	ldr	x1, [x20, 27904]
	ldrb	w2, [x2, 4]
	strb	w2, [x1, 4]
	ldr	x2, [x19, 640]
	ldr	x1, [x20, 27904]
	ldr	w2, [x2, 652]
	str	w2, [x1, 652]
	ldrb	w2, [x19, 4]
	ldr	x1, [x20, 27904]
	cbz	w2, .L3317
.L3226:
	strb	w0, [x1, 2]
	mov	x2, 624
	add	x1, x19, 16
	mov	w21, 3
	ldr	x0, [x20, 27904]
	ldr	w3, [x19, 716]
	str	w3, [x0, 668]
	ldr	x0, [x20, 27904]
	add	x0, x0, 8
	bl	memcpy
	ldr	x0, [x20, 27904]
	mov	w2, 2
	ldr	w1, [x19, 692]
	mov	w6, 1
	str	w1, [x0, 664]
	str	w1, [x0, 696]
	ldr	x0, [x20, 27904]
	ldr	x1, [x19, 640]
	str	x1, [x0, 712]
	str	x1, [x0, 752]
	str	x1, [x0, 792]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 656]
	str	w1, [x0, 636]
	ldr	x0, [x20, 27904]
	strb	wzr, [x0, 680]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x19, 4]
	strb	w1, [x0, 681]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x19, 5]
	strb	w1, [x0, 682]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 684]
	str	w1, [x0, 660]
	str	w1, [x0, 700]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x19, 9]
	str	w1, [x0, 656]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 668]
	str	w1, [x0, 648]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 672]
	str	w1, [x0, 692]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 676]
	str	w1, [x0, 732]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 680]
	str	w1, [x0, 772]
	ldrb	w1, [x19, 7]
	ldr	x0, [x20, 27904]
	cmp	w1, 1
	csel	w21, wzr, w21, ne
	strb	w21, [x0, 3]
	ldr	x0, [x20, 27904]
	ldr	x1, [x0, 712]
	ldrb	w5, [x0, 681]
	ldrb	w4, [x0, 682]
	strb	w2, [x0, 760]
	ldr	w3, [x0, 696]
	ldr	w2, [x0, 700]
	str	w2, [x0, 740]
	str	w2, [x0, 780]
	mov	w2, 0
	strb	w5, [x0, 721]
	strb	w4, [x0, 722]
	strb	w6, [x0, 720]
	strb	w5, [x0, 761]
	strb	w4, [x0, 762]
	str	w3, [x0, 736]
	str	x1, [x0, 752]
	str	w3, [x0, 776]
	str	x1, [x0, 792]
	mov	x0, x23
	ldr	x1, [x19, 640]
	bl	GetAPC
	cbnz	w0, .L3318
.L3241:
	add	x3, x22, 3488
	add	x0, x23, 65536
	mov	w2, 1
	ldr	x1, [x23, x3, lsl 3]
	ldr	w0, [x0, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetRef
	mov	w0, 0
.L3221:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3315:
	ldr	x1, .LC817
	mov	w2, w21
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x23
	bl	ClearAll
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3223:
	ldr	x0, [x19, 640]
	mov	w1, 3
	uxtw	x22, w20
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 1]
	cbz	w0, .L3229
	add	x20, x23, x22, lsl 3
	ldr	x0, [x20, 27904]
	cbz	x0, .L3229
	strb	w21, [x0, 720]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 684]
	str	w1, [x0, 740]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x19, 4]
	strb	w1, [x0, 721]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x19, 5]
	strb	w1, [x0, 722]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 676]
	str	w1, [x0, 732]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 684]
	str	w1, [x0, 740]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 692]
	str	w1, [x0, 736]
	ldr	x0, [x20, 27904]
	ldr	w2, [x0, 736]
	ldr	w1, [x0, 776]
	add	w1, w2, w1
	str	w1, [x0, 664]
	str	w1, [x0, 696]
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1]
	cbz	w0, .L3234
	ldrb	w0, [x19, 2]
.L3234:
	strb	w0, [x1]
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 1]
	orr	w0, w0, 1
	strb	w0, [x1, 1]
	ldrb	w0, [x19, 7]
	ldr	x1, [x20, 27904]
	cmp	w0, 1
	beq	.L3319
	ldrb	w0, [x1, 3]
.L3236:
	strb	w0, [x1, 3]
	mov	x2, 624
	add	x1, x19, 16
	ldr	x3, [x20, 27904]
	add	x0, x3, 8
	ldr	x4, [x3, 608]
	str	x4, [x29, 104]
	ldp	w26, w25, [x3, 16]
	ldp	w24, w21, [x3, 24]
	ldr	x28, [x3, 616]
	ldr	x27, [x3, 624]
	bl	memcpy
	ldr	x0, [x19, 616]
	ldr	x4, [x29, 104]
	cmn	x0, #1
	ccmn	x4, #1, 4, eq
	beq	.L3237
	ldr	x0, [x20, 27904]
	str	x4, [x0, 608]
.L3237:
	ldr	x0, [x19, 624]
	ldr	x1, [x20, 27904]
	cmp	x0, x28
	csel	x0, x0, x28, ls
	str	x0, [x1, 616]
	ldr	x0, [x19, 632]
	ldr	x1, [x20, 27904]
	cmp	x0, x27
	csel	x0, x0, x27, ls
	str	x0, [x1, 624]
	ldr	w0, [x19, 24]
	ldr	x1, [x20, 27904]
	cmp	w0, w26
	csel	w0, w0, w26, ls
	str	w0, [x1, 16]
	ldr	w0, [x19, 28]
	ldr	x1, [x20, 27904]
	cmp	w0, w25
	csel	w0, w0, w25, ls
	str	w0, [x1, 20]
	ldr	w0, [x19, 32]
	ldr	x1, [x20, 27904]
	cmp	w0, w24
	csel	w0, w0, w24, ls
	str	w0, [x1, 24]
	ldr	w0, [x19, 36]
	ldr	x1, [x20, 27904]
	cmp	w0, w21
	csel	w0, w0, w21, ls
	str	w0, [x1, 28]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3238
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3238
.L3239:
	ldr	x1, [x20, 27904]
	ldr	w0, [x1, 740]
	ldr	w2, [x1, 780]
	cmp	w0, w2
	csel	w0, w0, w2, cs
	str	w0, [x1, 660]
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 668]
	str	w1, [x0, 648]
	ldr	x0, [x20, 27904]
	bl	CombineFldsToFrm
	ldr	x1, [x20, 27904]
	ldr	w0, [x1, 672]
	ldr	w1, [x1, 732]
	add	x0, x0, 4096
	add	x0, x23, x0, lsl 2
	str	w1, [x0, 12056]
	b	.L3241
	.p2align 3
.L3229:
	add	x20, x23, x22, lsl 3
	ldr	x0, [x19, 640]
	mov	w1, 1
	str	x0, [x20, 27904]
	strb	w1, [x0, 720]
	ldr	x0, [x20, 27904]
	ldr	x2, [x19, 640]
	str	x2, [x0, 752]
	ldr	x0, [x20, 27904]
	ldrb	w2, [x19, 4]
	strb	w2, [x0, 721]
	ldr	x0, [x20, 27904]
	ldrb	w2, [x19, 5]
	strb	w2, [x0, 722]
	ldr	x0, [x20, 27904]
	ldr	w2, [x19, 676]
	str	w2, [x0, 732]
	ldr	x0, [x20, 27904]
	ldr	w2, [x19, 684]
	str	w2, [x0, 740]
	ldr	x0, [x20, 27904]
	ldr	w2, [x19, 692]
	str	w2, [x0, 736]
	ldr	x0, [x20, 27904]
	ldr	w2, [x0, 736]
	str	w2, [x0, 664]
	str	w2, [x0, 696]
	ldr	x0, [x20, 27904]
	ldrb	w2, [x19, 2]
	strb	w2, [x0]
	ldr	x0, [x20, 27904]
	strb	w1, [x0, 1]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x1, 4]
	strb	w1, [x0, 4]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 27904]
	ldr	w1, [x1, 652]
	str	w1, [x0, 652]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3231
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3231
.L3232:
	ldr	x0, [x20, 27904]
	mov	x2, 624
	add	x1, x19, 16
	str	wzr, [x0, 656]
	ldr	x0, [x20, 27904]
	add	x0, x0, 8
	bl	memcpy
	ldr	x1, [x20, 27904]
	mov	w2, 1
	ldr	w3, [x19, 716]
	mov	x0, x23
	str	w3, [x1, 668]
	ldr	x1, [x20, 27904]
	ldr	w3, [x19, 656]
	str	w3, [x1, 636]
	ldr	x1, [x20, 27904]
	ldr	w3, [x19, 668]
	str	w3, [x1, 648]
	ldr	x1, [x20, 27904]
	ldr	w3, [x19, 684]
	str	w3, [x1, 660]
	ldrb	w3, [x19, 7]
	ldr	x1, [x20, 27904]
	cmp	w3, w2
	cset	w3, eq
	strb	w3, [x1, 3]
	ldr	x1, [x19, 640]
	bl	GetAPC
	cbz	w0, .L3241
	ldr	x1, .LC815
	mov	w3, w0
	mov	w2, 2394
	b	.L3312
	.p2align 3
.L3242:
	add	x20, x23, x22, lsl 3
	ldr	x0, [x19, 640]
	mov	w1, 2
	str	x0, [x20, 27904]
	strb	w1, [x0, 760]
	ldr	x0, [x20, 27904]
	ldr	x2, [x19, 640]
	str	x2, [x0, 792]
	ldr	x0, [x20, 27904]
	ldrb	w2, [x19, 4]
	strb	w2, [x0, 761]
	ldr	x0, [x20, 27904]
	ldrb	w2, [x19, 5]
	strb	w2, [x0, 762]
	ldr	x0, [x20, 27904]
	ldr	w2, [x19, 680]
	str	w2, [x0, 772]
	ldr	x0, [x20, 27904]
	ldr	w2, [x19, 684]
	str	w2, [x0, 780]
	ldr	x0, [x20, 27904]
	ldr	w2, [x19, 692]
	str	w2, [x0, 776]
	ldr	x0, [x20, 27904]
	ldr	w2, [x0, 736]
	str	w2, [x0, 664]
	str	w2, [x0, 696]
	ldr	x0, [x20, 27904]
	ldrb	w2, [x19, 2]
	strb	w2, [x0]
	ldr	x0, [x20, 27904]
	strb	w1, [x0, 1]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 27904]
	ldrb	w1, [x1, 4]
	strb	w1, [x0, 4]
	ldr	x1, [x19, 640]
	ldr	x0, [x20, 27904]
	ldr	w1, [x1, 652]
	str	w1, [x0, 652]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	beq	.L3244
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3244
.L3245:
	ldr	x0, [x20, 27904]
	mov	x2, 624
	add	x1, x19, 16
	str	wzr, [x0, 656]
	ldr	x0, [x20, 27904]
	add	x0, x0, 8
	bl	memcpy
	ldr	x1, [x20, 27904]
	mov	w2, 2
	ldr	w3, [x19, 716]
	mov	x0, x23
	str	w3, [x1, 668]
	ldr	x1, [x20, 27904]
	ldr	w3, [x19, 656]
	str	w3, [x1, 636]
	ldr	x1, [x20, 27904]
	ldr	w3, [x19, 668]
	str	w3, [x1, 648]
	ldr	x1, [x20, 27904]
	ldr	w3, [x19, 684]
	str	w3, [x1, 660]
	str	w3, [x1, 780]
	ldrb	w3, [x19, 7]
	ldr	x1, [x20, 27904]
	cmp	w3, 1
	csel	w21, w21, wzr, eq
	strb	w21, [x1, 3]
	ldr	x1, [x19, 640]
	bl	GetAPC
	cbz	w0, .L3241
	ldr	x1, .LC815
	mov	w3, w0
	mov	w2, 2539
.L3312:
	mov	w0, 0
	bl	dprint_vfmw
	mov	x0, x23
	bl	ClearDPB
	mov	w0, -1
	b	.L3221
	.p2align 3
.L3317:
	ldrb	w0, [x19, 5]
	mov	w2, 3
	cmp	w0, wzr
	csel	w0, w0, w2, eq
	b	.L3226
	.p2align 3
.L3244:
	ldr	x0, [x20, 27904]
	mov	w1, 2
	strb	w1, [x0, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3245
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3245
	.p2align 3
.L3231:
	ldr	x0, [x20, 27904]
	mov	w1, 1
	strb	w1, [x0, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3232
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3232
	.p2align 3
.L3252:
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 2
	strb	w0, [x1, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3253
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3253
	.p2align 3
.L3238:
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 1
	strb	w0, [x1, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3239
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3239
	.p2align 3
.L3316:
	ldrb	w0, [x1, 3]
	orr	w0, w0, 2
	uxtb	w0, w0
	b	.L3250
	.p2align 3
.L3319:
	ldrb	w0, [x1, 3]
	orr	w0, w0, 1
	uxtb	w0, w0
	b	.L3236
	.p2align 3
.L3314:
	ldr	x1, .LC813
	mov	w3, w20
	mov	w2, 2303
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3221
.L3318:
	ldr	x1, .LC815
	mov	w3, w0
	mov	w2, 2341
	b	.L3312
	.size	InsertFrmInDPB, .-InsertFrmInDPB
	.align	3
.LC813:
	.xword	.LC812
	.align	3
.LC815:
	.xword	.LC814
	.align	3
.LC817:
	.xword	.LC816
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	StorePicInDPB
	.type	StorePicInDPB, %function
StorePicInDPB:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x24, x0
	mov	x0, x30
	bl	_mcount
	ldr	x0, [x24]
	ldr	w0, [x0, 596]
	cmp	w0, 1
	beq	.L3504
	add	x22, x24, 61440
	stp	wzr, wzr, [x22, 208]
.L3323:
	add	x19, x24, 20480
	str	wzr, [x19, 728]
	ldrb	w1, [x22, 115]
	cmp	w1, 2
	cset	w0, eq
	str	w0, [x19, 732]
	ldrb	w0, [x22, 112]
	cbz	w0, .L3505
	add	x21, x24, 65536
	ldr	w0, [x21, 44]
	ldr	w2, [x21, 128]
	cmp	w0, 2
	str	w2, [x21, 124]
	beq	.L3506
	ldr	x20, [x24]
	ldr	w0, [x20, 12]
	cmp	w0, wzr
	ble	.L3507
	ldrb	w2, [x22, 114]
	cbnz	w2, .L3331
.L3512:
	sub	w2, w1, #1
	ldr	x25, [x22, 752]
	uxtb	w2, w2
	cbnz	w1, .L3508
.L3332:
	cmp	w2, 1
	bls	.L3509
.L3486:
	cbz	w1, .L3337
	ldrb	w2, [x25, 1]
	cbnz	w2, .L3337
	.p2align 2
.L3331:
	cmp	w0, 2
	beq	.L3510
.L3358:
	ldr	w6, [x22, 776]
	cbz	w6, .L3367
	ldr	w3, [x21, 108]
	ldr	w2, [x22, 804]
	cmp	w3, w2
	bcs	.L3368
	ldrb	w0, [x22, 114]
	cbz	w0, .L3511
.L3368:
	mov	x0, x24
	bl	Marking
	cbz	w0, .L3370
	ldr	x1, .LC835
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 3961
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
	b	.L3412
.L3505:
	ldr	x1, .LC820
	add	x21, x24, 65536
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 3666
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
.L3412:
	ldr	x1, [x22, 752]
	mov	w2, 1
	ldr	w0, [x21, 116]
	mov	w19, -1
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
	mov	w3, w19
	mov	w2, 4180
	mov	x1, x20
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 1]
.L3410:
	strb	wzr, [x22, 112]
	mov	w0, w19
.L3493:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
.L3504:
	ldr	x1, .LC818
	add	x22, x24, 61440
	ldr	x1, [x1]
	ldr	w2, [x1, 4180]
	ldr	w1, [x1, 4184]
	cmp	w1, wzr
	csinc	w1, w1, wzr, ne
	udiv	w1, w2, w1
	stp	w0, w1, [x22, 208]
	b	.L3323
.L3507:
	ldr	w2, [x21, 120]
	cmp	w2, 2
	beq	.L3331
	ldrb	w2, [x22, 114]
	cbnz	w2, .L3331
	b	.L3512
.L3510:
	ldr	w0, [x22, 776]
	cbz	w0, .L3360
	ldrb	w0, [x22, 118]
	cmp	w0, 5
	beq	.L3361
.L3365:
	mov	x0, x24
	add	x19, x24, 24576
	bl	SimpleSlideDPB
	ldr	w1, [x19, 3716]
	add	x2, x22, 112
	mov	x0, x24
	bl	InsertFrmInDPB
	cbz	w0, .L3513
	ldr	x20, .LC831
	mov	w3, w0
	mov	w2, 3927
	mov	w0, 0
	mov	x1, x20
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 3928
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3412
.L3508:
	ldrb	w3, [x25, 1]
	cmp	w3, wzr
	ccmp	w2, 1, 0, eq
	bls	.L3332
	cmp	w0, 2
	bne	.L3358
	b	.L3510
.L3509:
	ldr	w2, [x21, 120]
	cbnz	w2, .L3334
	mov	w0, 1
	mov	x2, 720
	str	w0, [x21, 120]
	add	x1, x22, 112
	add	x0, x22, 832
	bl	memcpy
	ldr	w0, [x20, 12]
	b	.L3331
.L3337:
	add	x23, x22, 112
.L3339:
	cbz	w1, .L3340
	ldrb	w0, [x23, 1]
	cmp	w0, 1
	beq	.L3514
.L3340:
	ldr	w0, [x23, 692]
	str	w0, [x25, 664]
.L3341:
	mov	x1, x25
	mov	x0, x24
	bl	GetImagePara
	ldr	w0, [x21, 116]
	ldr	w1, [x25, 632]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L3515
	ldrb	w0, [x23, 3]
	cbz	w0, .L3343
	ldrb	w0, [x23, 1]
	cmp	w0, 1
	beq	.L3516
.L3343:
	ldr	x0, [x23, 616]
.L3495:
	str	x0, [x20, 600]
.L3344:
	ldr	x0, [x23, 624]
	str	x0, [x20, 608]
	ldr	x0, [x23, 632]
	str	x0, [x20, 616]
	ldr	w0, [x23, 24]
	str	w0, [x20, 8]
	ldr	w0, [x23, 28]
	str	w0, [x20, 12]
	ldr	w0, [x23, 32]
	str	w0, [x20, 16]
	ldr	w0, [x23, 36]
	str	w0, [x20, 20]
	ldr	w1, [x25, 664]
	str	w1, [x20, 160]
	ldr	w0, [x23, 244]
	str	w0, [x20, 228]
	ldr	w0, [x23, 240]
	str	w0, [x20, 224]
	ldr	x0, [x23, 248]
	str	x0, [x20, 232]
	cbz	w1, .L3350
	ldr	w0, [x20, 108]
	and	w0, w0, 3
	cbz	w0, .L3517
.L3348:
	ldr	x0, [x24]
	ldr	w2, [x0, 4]
	cmp	w2, w1
	bcs	.L3350
	ldr	x0, .LC829
	ldr	x4, [x0]
	cbz	x4, .L3357
	ldr	w0, [x21, 116]
	mov	w3, 8
	stp	w1, w2, [x29, 88]
	add	x2, x29, 88
	mov	w1, 104
	blr	x4
.L3357:
	ldr	w0, [x21, 116]
	mov	w2, 0
	ldr	w1, [x25, 632]
	bl	FSP_SetDisplay
	ldr	x1, .LC822
	mov	w3, 3884
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
	b	.L3412
.L3334:
	cmp	w2, 1
	bne	.L3486
	add	x23, x22, 112
	ldr	x25, [x22, 1472]
	b	.L3339
.L3370:
	mov	x0, x24
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	mov	x0, x24
	bl	RemoveUnUsedFrameStore
	cbz	w0, .L3360
	ldr	x1, .LC837
	mov	w0, 22
	bl	dprint_vfmw
.L3360:
	ldrb	w0, [x22, 113]
	cmp	w0, 1
	beq	.L3372
	add	x23, x24, 24576
.L3376:
	ldr	w6, [x22, 776]
	cbz	w6, .L3518
	ldr	w4, [x23, 3720]
	ldr	w5, [x23, 3724]
	ldr	w3, [x23, 3712]
	add	w0, w4, w5
	cmp	w3, w0
	bhi	.L3384
	ldr	x0, [x24]
	ldr	w0, [x0, 12]
	cmp	w0, 3
	beq	.L3385
	ldr	x0, .LC838
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L3385
	ldr	x1, .LC840
	mov	w2, 4040
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x24
	bl	ClearDPB
	ldr	x1, .LC822
	mov	w3, 4066
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
	b	.L3412
.L3367:
	ldrb	w0, [x22, 113]
	cmp	w0, 1
	beq	.L3372
	add	x23, x24, 24576
	ldr	w3, [x23, 3712]
.L3384:
	mov	w7, -1
	mov	w8, w7
	.p2align 2
.L3401:
	ldr	w1, [x23, 3716]
	cmp	w1, w3
	bcc	.L3519
	cmp	w1, w3
	mov	w19, w1
	bls	.L3391
	mov	w19, w3
	str	w3, [x23, 3716]
.L3391:
	cbz	w19, .L3420
	add	x1, x23, 3328
	mov	w5, 2147483647
	mov	w20, 0
	mov	w0, 0
	.p2align 2
.L3394:
	ldr	x4, [x1]
	add	x1, x1, 8
	ldr	w2, [x4, 648]
	cmp	w2, w5
	bge	.L3393
	ldrb	w4, [x4, 4]
	cmp	w4, wzr
	csel	w20, w20, w0, eq
	csel	w5, w5, w2, eq
.L3393:
	add	w0, w0, 1
	cmp	w0, w19
	bne	.L3394
.L3392:
	cbnz	w6, .L3395
	ldr	w0, [x22, 780]
	cmp	w0, w5
	blt	.L3520
.L3395:
	cmp	w7, w19
	ccmp	w8, w20, 0, eq
	beq	.L3521
	mov	w1, w20
	mov	x0, x24
	bl	OutputFrmFromDPB
	cmn	w0, #1
	mov	w25, w0
	beq	.L3400
	mov	w7, w19
	mov	w8, w20
	ldr	w6, [x22, 776]
	ldr	w3, [x23, 3712]
	b	.L3401
.L3420:
	mov	w5, 2147483647
	mov	w20, 0
	b	.L3392
.L3517:
	ldr	x1, .LC828
	mov	w2, 3851
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC829
	ldr	x4, [x0]
	cbz	x4, .L3349
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 111
	blr	x4
.L3349:
	ldr	w1, [x20, 160]
	cbnz	w1, .L3348
.L3350:
	ldr	w0, [x21, 116]
	mov	w2, 1
	ldr	w1, [x25, 632]
	bl	FSP_SetDisplay
	ldr	w0, [x21, 116]
	mov	x4, x20
	add	x3, x24, 8
	mov	x2, x24
	mov	w1, 0
	bl	InsertImgToVoQueue
	cmp	w0, 1
	beq	.L3522
	ldr	w0, [x21, 116]
	mov	w2, 0
	ldr	w1, [x25, 632]
	bl	FSP_SetDisplay
.L3352:
	ldr	x0, [x24]
	ldr	w0, [x0, 604]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L3523
	str	wzr, [x20, 24]
.L3354:
	ldr	w0, [x21, 136]
	ldr	w1, [x21, 120]
	add	w0, w0, 2
	str	w0, [x21, 136]
	cmp	w1, 2
	beq	.L3356
	mov	w0, 1
	str	w0, [x25, 800]
.L3356:
	mov	w0, 2
	str	w0, [x21, 120]
	ldr	x0, [x24]
	ldr	w0, [x0, 12]
	b	.L3331
.L3506:
	ldr	x1, .LC824
	bl	dprint_vfmw
	mov	x0, x24
	bl	DirectOutput
	mov	w19, w0
	ldr	w1, [x22, 2776]
	ldr	w0, [x22, 828]
	cmp	w0, w1
	beq	.L3327
	uxtw	x0, w0
	add	x0, x0, 12288
	add	x0, x0, 3748
	str	wzr, [x24, x0, lsl 2]
	str	w1, [x22, 828]
.L3327:
	cmp	w19, 1
	beq	.L3524
.L3328:
	ldr	x1, .LC822
	mov	w3, 3727
	mov	w2, w19
	mov	w0, 14
	bl	dprint_vfmw
.L3329:
	cmn	w19, #1
	bne	.L3410
	ldr	x20, .LC831
	b	.L3412
	.p2align 3
.L3372:
	ldr	x0, [x22, 1584]
	ldr	x3, [x22, 752]
	cmp	x3, x0
	ccmp	x0, xzr, 4, eq
	beq	.L3525
	mov	x0, x24
	bl	DirectOutput
	cmn	w0, #1
	mov	w19, w0
	beq	.L3526
	cmp	w0, 1
	bne	.L3380
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 1]
.L3380:
	ldr	x1, .LC822
	mov	w3, 3997
	mov	w2, w19
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3329
	.p2align 3
.L3519:
	cbz	w6, .L3403
	ldrb	w0, [x22, 116]
	cbnz	w0, .L3403
	ldr	w0, [x23, 3720]
	cbz	w0, .L3403
	mov	w0, 0
	b	.L3408
	.p2align 3
.L3405:
	ldr	w1, [x23, 3720]
	cmp	w1, w0
	bls	.L3527
.L3408:
	add	x1, x24, x0, uxtw 3
	ldr	w3, [x22, 768]
	add	w0, w0, 1
	ldr	x2, [x1, 28032]
	ldr	w4, [x2, 636]
	cmp	w4, w3
	bne	.L3405
	strb	wzr, [x2, 2]
	ldr	x2, [x1, 28032]
	ldrb	w3, [x2, 4]
	cmp	w3, 1
	beq	.L3528
.L3406:
	ldr	w1, [x2, 668]
	ldr	w3, [x22, 2776]
	cmp	w1, w3
	uxtw	x1, w1
	add	x1, x1, 12288
	beq	.L3405
	add	x1, x1, 3748
	str	wzr, [x24, x1, lsl 2]
	str	w3, [x2, 668]
	b	.L3405
.L3527:
	ldr	w1, [x23, 3716]
.L3403:
	add	x2, x22, 112
	mov	x0, x24
	bl	InsertFrmInDPB
	ldr	w1, [x23, 3716]
	str	w1, [x22, 1576]
	cbz	w0, .L3409
	ldr	x20, .LC831
	mov	w3, w0
	mov	w2, 4153
	mov	w0, 1
	mov	x1, x20
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 4154
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3412
.L3400:
	ldr	x20, .LC831
	mov	w3, w0
	mov	w2, 4116
	mov	w0, 1
	mov	x1, x20
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 4117
	mov	w2, w25
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3412
.L3511:
	ldr	x1, .LC833
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC829
	ldr	x4, [x0]
	cbz	x4, .L3369
	ldr	w1, [x21, 108]
	mov	w3, 8
	ldr	w2, [x22, 804]
	ldr	w0, [x21, 116]
	stp	w2, w1, [x29, 88]
	add	x2, x29, 88
	mov	w1, 104
	blr	x4
.L3369:
	mov	x0, x24
	mov	w20, -1
	bl	ClearAllSlice
	ldr	w0, [x24, 14948]
	mov	w1, 2
	str	w0, [x19, 3240]
	add	x0, x19, 3072
	strb	w1, [x19, 3233]
	mov	w1, 32
	strb	wzr, [x19, 3232]
	strb	w1, [x19, 3234]
	mov	w1, 262144
	stp	w20, wzr, [x0, 188]
	stp	wzr, w1, [x0, 196]
	mov	w0, 23352
	movk	w0, 0x1, lsl 16
	str	w0, [x19, 3248]
	mov	w0, 7
	str	w0, [x19, 3244]
	mov	w0, 3
	str	w0, [x19, 3256]
	mov	x0, x24
	bl	ClearDPB
	ldr	x1, .LC822
	mov	w2, w20
	mov	w3, 3952
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
	b	.L3412
.L3522:
	ldr	w0, [x21, 144]
	add	w0, w0, 1
	str	w0, [x21, 144]
	b	.L3352
.L3513:
	ldr	w1, [x19, 3716]
	mov	x0, x24
	add	w1, w1, 1
	str	w1, [x19, 3716]
	mov	w19, 0
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	ldr	x1, .LC822
	mov	w3, 3936
	mov	w2, 0
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3410
.L3385:
	ldr	w6, [x23, 3716]
	cmp	w6, w3
	bls	.L3387
	mov	w6, w3
	str	w3, [x23, 3716]
.L3387:
	cbz	w6, .L3417
	add	x2, x23, 3328
	mov	w1, 0
	mov	w0, 0
	mov	w5, 2147483647
	.p2align 2
.L3390:
	ldr	x4, [x2]
	add	x2, x2, 8
	ldr	w3, [x4, 648]
	cmp	w3, w5
	bge	.L3389
	ldrb	w4, [x4, 4]
	cmp	w4, wzr
	csel	w1, w1, w0, eq
	csel	w5, w5, w3, eq
.L3389:
	add	w0, w0, 1
	cmp	w0, w6
	bne	.L3390
.L3388:
	mov	x0, x24
	bl	RemoveFrameStoreOutDPB
	mov	x0, x24
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	ldr	w6, [x22, 776]
	ldr	w3, [x23, 3712]
	b	.L3384
.L3516:
	ldr	x0, [x23, 616]
	cmn	x0, #1
	bne	.L3344
	ldr	x0, [x22, 1448]
	cmn	x0, #1
	bne	.L3495
	b	.L3344
	.p2align 3
.L3521:
	ldr	x2, .LC841
	mov	w3, w20
	ldr	x1, .LC843
	mov	w0, 1
	add	x2, x2, 256
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 4103
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
	b	.L3412
.L3520:
	ldr	w0, [x21, 44]
	cmp	w0, 2
	beq	.L3529
.L3396:
	mov	x0, x24
	bl	DirectOutput
	cmn	w0, #1
	mov	w19, w0
	beq	.L3530
	cmp	w0, 1
	bne	.L3398
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 1]
.L3398:
	ldr	x1, .LC822
	mov	w3, 4096
	mov	w2, w19
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3329
.L3409:
	add	w1, w1, 1
	mov	x0, x24
	str	w1, [x23, 3716]
	mov	w19, 0
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	b	.L3410
.L3518:
	ldr	w3, [x23, 3712]
	b	.L3384
.L3525:
	add	x23, x24, 24576
	ldr	w4, [x23, 3716]
	cbz	w4, .L3376
	ldr	x0, [x24, 27904]
	cmp	x3, x0
	beq	.L3531
	add	x0, x23, 3336
	mov	w1, 0
	.p2align 2
.L3381:
	add	w1, w1, 1
	cmp	w1, w4
	beq	.L3376
	ldr	x2, [x0], 8
	cmp	x3, x2
	bne	.L3381
.L3377:
	add	x2, x22, 112
	mov	x0, x24
	bl	InsertFrmInDPB
	cmn	w0, #1
	mov	w19, w0
	beq	.L3532
	mov	x0, x24
	mov	w19, 0
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	ldr	x1, .LC822
	mov	w3, 4017
	mov	w2, 0
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3410
.L3524:
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 1]
	b	.L3328
.L3523:
	mov	x1, x20
	mov	x0, x24
	bl	SetFrmRepeatCount.part.1
	b	.L3354
.L3514:
	mov	w0, 3
	strb	w0, [x25, 1]
	ldr	w1, [x25, 664]
	ldr	w0, [x23, 692]
	add	w0, w1, w0
	str	w0, [x25, 664]
	b	.L3341
.L3361:
	mov	x0, x24
	bl	IDRMemMarking
	cbz	w0, .L3365
	mov	w0, -1
	b	.L3493
	.p2align 3
.L3528:
	str	w3, [x2, 652]
	ldr	x2, [x1, 28032]
	strb	wzr, [x2, 4]
	ldr	x2, [x1, 28032]
	b	.L3406
.L3529:
	ldrb	w0, [x22, 115]
	cbz	w0, .L3396
	ldr	x0, [x22, 752]
	str	x0, [x22, 1592]
	b	.L3396
.L3532:
	ldr	x20, .LC831
	mov	w3, w0
	mov	w2, 4010
	mov	w0, 1
	mov	x1, x20
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 4011
	mov	w2, w19
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3412
.L3515:
	ldr	x1, .LC826
	mov	w2, 3819
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 3820
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
	b	.L3412
.L3530:
	ldr	x1, .LC831
	mov	w3, w0
	mov	w2, 4087
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3398
.L3417:
	mov	w1, 0
	b	.L3388
.L3526:
	ldr	x1, .LC831
	mov	w3, w0
	mov	w2, 3987
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3380
.L3531:
	mov	w1, 0
	b	.L3377
	.size	StorePicInDPB, .-StorePicInDPB
	.align	3
.LC818:
	.xword	g_HwMem
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
.LC838:
	.xword	g_apple_airplay_mirrorinig_flag
	.align	3
.LC840:
	.xword	.LC839
	.align	3
.LC841:
	.xword	.LANCHOR1
	.align	3
.LC843:
	.xword	.LC842
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	AllocFrameStore
	.type	AllocFrameStore, %function
AllocFrameStore:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	str	x27, [sp, 80]
	mov	x26, x0
	mov	x0, x30
	add	x20, x26, 20480
	mov	w23, w1
	bl	_mcount
	ldrb	w0, [x20, 1585]
	cbz	w0, .L3534
	ldrb	w1, [x20, 1586]
	cbz	w1, .L3602
	add	x21, x26, 65536
	ldr	w22, [x21, 44]
	cmp	w22, 2
	beq	.L3560
	add	x25, x26, 61440
	mov	w22, 2
	ldrb	w1, [x25, 1552]
	strb	wzr, [x25, 113]
	cmp	w1, 1
	bne	.L3562
	b	.L3559
	.p2align 3
.L3534:
	add	x21, x26, 65536
	ldr	w0, [x21, 44]
	cmp	w0, 2
	beq	.L3603
	add	x25, x26, 61440
	mov	w22, 0
	strb	wzr, [x25, 113]
.L3562:
	ldr	w3, [x25, 2772]
	strb	wzr, [x25, 112]
	cbz	w3, .L3553
	add	x19, x26, 28672
	mov	x24, 0
	add	x19, x19, 560
	.p2align 2
.L3552:
	ldrb	w0, [x19, 1]
	cbnz	w0, .L3547
	cbz	x19, .L3548
	add	x0, x26, 24576
	ldr	w2, [x0, 3716]
	cbz	w2, .L3548
	ldr	x0, [x26, 27904]
	cmp	x0, x19
	beq	.L3547
	sub	w2, w2, #1
	mov	x0, x26
	add	x2, x26, x2, lsl 3
	b	.L3550
	.p2align 3
.L3551:
	ldr	x1, [x0, 27904]
	cmp	x1, x19
	beq	.L3547
.L3550:
	cmp	x0, x2
	add	x0, x0, 8
	bne	.L3551
.L3548:
	lsl	x0, x24, 2
	mov	w27, 1
	sub	x0, x0, x24
	str	x19, [x25, 752]
	strb	w27, [x25, 112]
	mov	w3, 101
	add	x1, x0, x0, lsl 4
	mov	x2, 624
	lsl	x1, x1, x27
	add	x0, x25, 128
	sub	x24, x1, x24
	add	x24, x26, x24, lsl 3
	add	x24, x24, 28672
	add	x1, x24, 568
	str	wzr, [x24, 1224]
	str	w3, [x25, 804]
	bl	memcpy
	str	wzr, [x25, 288]
	strb	wzr, [x24, 563]
	str	wzr, [x19, 652]
	ldr	x0, [x25, 752]
	strb	wzr, [x0, 2]
	ldr	x0, [x25, 752]
	strb	w27, [x0, 4]
	ldr	x0, [x25, 752]
	str	wzr, [x0, 800]
	ldrb	w19, [x25, 112]
	cmp	w19, 1
	bne	.L3553
	cmp	w23, wzr
	ldr	w0, [x21, 116]
	cset	w1, eq
	bl	FSP_NewLogicFs
	mov	w23, w0
	mov	w1, w0
	ldr	w0, [x21, 116]
	bl	FSP_GetLogicFs
	mov	x24, x0
	cmp	w23, wzr
	ccmp	x0, xzr, 4, ge
	beq	.L3554
	add	x1, x0, 8
	ldr	x0, [x26]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L3604
.L3555:
	ldr	x19, .LC846
	mov	x2, 624
	ldr	x0, [x25, 752]
	ldr	x3, [x19, 104]
	add	x0, x0, 8
	blr	x3
	ldr	x1, [x25, 752]
	mov	x2, 624
	ldr	x3, [x19, 104]
	add	x0, x25, 128
	add	x1, x1, 8
	blr	x3
	ldr	x0, [x25, 752]
	mov	w4, w23
	str	w23, [x0, 632]
	ldrsb	w0, [x24, 4]
	str	w0, [x25, 828]
	ldr	x3, [x25, 752]
	ldrb	w0, [x20, 1585]
.L3544:
	cmp	w0, wzr
	uxtb	w1, w22
	mov	w2, 3
	strb	w1, [x25, 115]
	csel	w0, w0, w2, eq
	strb	w0, [x3, 680]
	ldrb	w0, [x25, 113]
	cmp	w0, 1
	beq	.L3605
	strb	w1, [x25, 1552]
	ldr	w0, [x20, 1604]
	str	w0, [x25, 1560]
	ldr	w0, [x20, 1616]
	str	w0, [x25, 1556]
	ldr	w0, [x21, 128]
	str	w0, [x25, 1568]
	str	w4, [x25, 1572]
	cbnz	w22, .L3606
	strb	wzr, [x25, 1553]
	mov	w0, 0
.L3594:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L3602:
	add	x21, x26, 65536
	ldr	w1, [x21, 44]
	cmp	w1, 2
	beq	.L3607
	add	x25, x26, 61440
	mov	w22, 1
	ldrb	w1, [x25, 1552]
	strb	wzr, [x25, 113]
	cmp	w1, 2
	bne	.L3562
.L3559:
	ldr	w1, [x20, 1604]
	ldr	w2, [x25, 1560]
	cmp	w2, w1
	bne	.L3562
	ldr	w1, [x20, 1616]
	cbnz	w1, .L3538
	ldr	w1, [x25, 1556]
	cbnz	w1, .L3562
.L3539:
	ldp	w2, w1, [x21, 124]
	sub	w1, w1, w2
	cmp	w1, 2
	bhi	.L3562
	ldr	x3, [x25, 752]
	mov	w1, 1
	strb	w1, [x25, 113]
	cbz	x3, .L3541
	cmp	w22, 2
	beq	.L3608
	cmp	w22, 1
	mov	w4, 0
	bne	.L3544
	ldrb	w1, [x3, 1]
	cmp	w1, 2
	beq	.L3544
.L3541:
	strb	wzr, [x25, 113]
	b	.L3562
	.p2align 3
.L3603:
	mov	w22, 0
.L3560:
	add	x25, x26, 61440
	strb	wzr, [x25, 113]
	b	.L3562
	.p2align 3
.L3547:
	add	w24, w24, 1
	add	x19, x19, 808
	cmp	w24, w3
	bne	.L3552
.L3553:
	ldr	x1, .LC845
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x26
	bl	ClearAll
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	mov	w0, -1
	ldp	x29, x30, [sp], 112
	ret
.L3606:
	mov	w1, 1
	strb	w1, [x25, 1553]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L3607:
	mov	w22, 1
	b	.L3560
.L3605:
	strb	wzr, [x25, 1552]
	mov	w0, 0
	ldr	w1, [x20, 1604]
	str	w1, [x25, 1560]
	ldr	w1, [x20, 1616]
	str	w1, [x25, 1556]
	ldr	w1, [x21, 128]
	str	w1, [x25, 1568]
	strb	wzr, [x25, 1553]
	str	w4, [x25, 1572]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L3604:
	ldr	w0, [x26, 14936]
	ldr	w19, [x1, 140]
	ldr	w2, [x26, 14932]
	lsl	w0, w0, 4
	str	x1, [x29, 104]
	mul	w19, w0, w19
	lsl	w2, w2, 4
	stp	w2, w0, [x1, 112]
	ldr	w0, [x1, 44]
	add	w0, w19, w0
	str	w0, [x1, 48]
	bl	MEM_Phy2Vir
	ldr	x1, [x29, 104]
	ldr	w2, [x1, 28]
	ldr	w3, [x1, 140]
	str	x0, [x1, 512]
	add	w0, w19, w2
	ldr	w2, [x1, 60]
	str	w0, [x1, 32]
	add	w0, w3, w0
	str	w0, [x1, 40]
	add	w0, w19, w2
	str	w0, [x1, 64]
	bl	MEM_Phy2Vir
	ldr	x1, [x29, 104]
	str	x0, [x1, 528]
	b	.L3555
.L3538:
	ldr	w1, [x25, 1556]
	cbz	w1, .L3562
	b	.L3539
.L3554:
	ldr	x2, [x25, 752]
	mov	w0, 0
	strb	wzr, [x25, 112]
	ldr	x1, .LC848
	strb	wzr, [x2, 4]
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x26
	bl	ClearAll
	mov	w0, -1
	b	.L3594
.L3608:
	ldrb	w0, [x3, 1]
	cmp	w0, 1
	bne	.L3541
	ldrb	w0, [x20, 1585]
	mov	w4, 0
	b	.L3544
	.size	AllocFrameStore, .-AllocFrameStore
	.align	3
.LC845:
	.xword	.LC844
	.align	3
.LC846:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC848:
	.xword	.LC847
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	DecGap
	.type	DecGap, %function
DecGap:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x19, x0
	mov	x0, x30
	add	x25, x19, 24576
	add	x20, x19, 20480
	bl	_mcount
	add	x28, x19, 16384
	add	x0, x19, 12288
	add	x27, x19, 61440
	ldr	w1, [x25, 3188]
	mov	w22, 2
	ldr	w2, [x20, 1604]
	ldrb	w0, [x0, 2705]
	ldr	w5, [x20, 1600]
	cmp	w0, 1
	lsl	x0, x1, 6
	sub	x0, x0, x1
	ldr	w21, [x28, 788]
	lsl	x0, x0, 2
	str	w2, [x29, 120]
	sub	x0, x0, x1
	lsl	x0, x0, 1
	sub	x0, x0, x1
	lsl	x0, x0, 2
	add	x1, x19, x0
	add	x0, x0, 20480
	add	x1, x1, 20480
	add	x0, x19, x0
	add	x0, x0, 3276
	ldrb	w1, [x1, 3279]
	str	w1, [x29, 104]
	ldr	w1, [x20, 752]
	str	w1, [x29, 112]
	ldr	w1, [x20, 756]
	str	x0, [x27, 760]
	str	w1, [x29, 108]
	csel	w1, wzr, w22, ne
	ldr	w0, [x25, 3716]
	str	w1, [x29, 116]
	cbz	w0, .L3633
	mov	w0, 1
	add	w21, w21, 4
	add	w2, w5, 1
	lsl	w21, w0, w21
	ldr	x1, .LC852
	mov	w0, w22
	udiv	w5, w2, w21
	str	wzr, [x20, 752]
	str	wzr, [x20, 756]
	msub	w26, w5, w21, w2
	bl	dprint_vfmw
	ldr	x1, .LC854
	mov	w3, w26
	ldr	w2, [x29, 120]
	mov	w0, w22
	bl	dprint_vfmw
	ldr	w1, [x25, 3712]
	ldr	w0, [x25, 3716]
	cmp	w0, w1
	bhi	.L3634
.L3613:
	cbz	w0, .L3624
	sub	w3, w0, #1
	mov	w22, 0
	add	x3, x3, 3489
	add	x0, x25, 3328
	add	x3, x19, x3, lsl 3
	.p2align 2
.L3616:
	ldr	x2, [x0]
	add	x0, x0, 8
	ldr	w1, [x2, 648]
	cmp	w22, w1
	bge	.L3615
	ldrb	w2, [x2, 4]
	cmp	w2, wzr
	csel	w22, w22, w1, eq
.L3615:
	cmp	x0, x3
	bne	.L3616
.L3614:
	ldr	w0, [x29, 120]
	mov	w23, 1
	str	wzr, [x29, 124]
	mov	w24, w23
	cmp	w0, w26
	bne	.L3629
	b	.L3621
	.p2align 3
.L3619:
	mov	x1, 0
	mov	x0, x19
	strb	wzr, [x27, 116]
	bl	StorePicInDPB
	add	w1, w26, 1
	cbnz	w0, .L3635
.L3620:
	sdiv	w0, w1, w21
	str	w26, [x20, 1600]
	msub	w26, w0, w21, w1
	ldr	w0, [x29, 120]
	cmp	w0, w26
	beq	.L3621
	ldr	w0, [x25, 3716]
	ldr	w1, [x29, 124]
	cmp	w1, w0
	bhi	.L3621
.L3629:
	mov	w1, 1
	mov	x0, x19
	bl	AllocFrameStore
	cbnz	w0, .L3636
	ldr	x1, .LC858
	mov	w0, 2
	bl	dprint_vfmw
	strb	w24, [x27, 118]
	ldr	x0, [x27, 752]
	str	w26, [x0, 636]
	str	w26, [x27, 768]
	ldr	x0, [x27, 752]
	str	w26, [x0, 688]
	ldr	w0, [x29, 116]
	str	w0, [x27, 776]
	ldr	x0, [x27, 752]
	str	wzr, [x0, 684]
	ldr	x0, [x27, 752]
	str	w23, [x0, 652]
	ldr	x0, [x27, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x27, 752]
	strb	w23, [x0]
	strb	w24, [x27, 114]
	ldr	x0, [x27, 760]
	strb	wzr, [x0, 3]
	ldr	w0, [x28, 792]
	cbz	w0, .L3619
	mov	x0, x19
	str	w26, [x20, 768]
	bl	DecPOC
	ldr	w0, [x20, 748]
	str	w0, [x27, 780]
	ldr	w0, [x20, 744]
	str	w0, [x27, 784]
	ldr	w0, [x20, 736]
	str	w0, [x27, 788]
	cmp	w0, w22
	ldr	w0, [x20, 740]
	str	w0, [x27, 792]
	bls	.L3619
	cmp	w0, w22
	ldr	w0, [x29, 124]
	mov	x1, 0
	strb	wzr, [x27, 116]
	csinc	w0, w0, w0, ls
	str	w0, [x29, 124]
	mov	x0, x19
	bl	StorePicInDPB
	add	w1, w26, 1
	cbz	w0, .L3620
.L3635:
	ldr	w1, [x29, 112]
	mov	w3, w0
	str	w1, [x20, 752]
	mov	w2, 6545
	ldr	w1, [x29, 108]
	mov	w0, 0
	str	w1, [x20, 756]
	ldrb	w5, [x29, 104]
	ldr	x4, [x27, 760]
	ldr	x1, .LC860
	strb	w5, [x4, 3]
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
.L3634:
	mov	w0, w1
	str	w1, [x25, 3716]
	b	.L3613
	.p2align 3
.L3633:
	ldr	x1, .LC850
	bl	dprint_vfmw
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L3621:
	ldr	w0, [x29, 112]
	str	w0, [x20, 752]
	ldr	w0, [x29, 108]
	str	w0, [x20, 756]
	mov	w0, 0
	ldrb	w2, [x29, 104]
	ldr	x1, [x27, 760]
	strb	w2, [x1, 3]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L3636:
	ldr	w0, [x29, 112]
	str	w0, [x20, 752]
	ldr	w0, [x29, 108]
	str	w0, [x20, 756]
	mov	w0, 0
	ldrb	w3, [x29, 104]
	ldr	x2, [x27, 760]
	ldr	x1, .LC856
	strb	w3, [x2, 3]
	bl	dprint_vfmw
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
.L3624:
	mov	w22, 0
	b	.L3614
	.size	DecGap, .-DecGap
	.align	3
.LC850:
	.xword	.LC849
	.align	3
.LC852:
	.xword	.LC851
	.align	3
.LC854:
	.xword	.LC853
	.align	3
.LC856:
	.xword	.LC855
	.align	3
.LC858:
	.xword	.LC857
	.align	3
.LC860:
	.xword	.LC859
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_Init
	.type	H264DEC_Init, %function
H264DEC_Init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x21, x1
	bl	_mcount
	ldr	x22, .LC861
	mov	x2, 176
	mov	w1, 0
	movk	x2, 0x1, lsl 16
	mov	x0, x19
	ldr	x3, [x22, 96]
	ldr	w20, [x19, 608]
	blr	x3
	str	w20, [x19, 608]
	add	x0, x19, 8
	bl	ResetVoQueue
	str	x21, [x19]
	str	xzr, [x19, 616]
	str	wzr, [x19, 624]
	ldr	w0, [x21, 28]
	cmp	w0, 25
	beq	.L3647
	mov	w2, 32
	mov	w1, 256
	mov	w0, 136
	str	w2, [x19, 14944]
	str	w0, [x19, 14940]
	str	w1, [x19, 14948]
.L3639:
	ldr	w0, [x21, 584]
	mov	w23, 2208
	add	x20, x19, 65536
	bl	MEM_Phy2Vir
	ldr	x1, [x19]
	mov	w2, 4000
	ldr	w4, [x19, 14944]
	ldr	w5, [x19, 14948]
	ldrsw	x3, [x1, 580]
	smull	x2, w4, w2
	add	x3, x0, x3
	str	x3, [x19, 14960]
	add	x1, x3, x2
	str	x1, [x19, 14968]
	cmp	x1, xzr
	smaddl	x0, w5, w23, x1
	ccmp	x3, xzr, 4, ne
	str	x0, [x20, 32]
	ccmp	x0, xzr, 4, ne
	beq	.L3648
	ldr	w6, [x21, 608]
	cmp	w6, wzr
	ccmp	x3, xzr, 4, eq
	ccmp	x1, xzr, 4, ne
	beq	.L3642
	ldr	x4, [x22, 96]
	mov	w1, 0
	mov	x0, x3
	blr	x4
	ldr	w2, [x19, 14948]
	mov	w1, 0
	ldr	x0, [x19, 14968]
	ldr	x3, [x22, 96]
	smull	x2, w2, w23
	blr	x3
	ldr	w4, [x19, 14944]
	ldr	w5, [x19, 14948]
	ldr	x0, [x20, 32]
.L3642:
	add	x2, x19, 16384
	ldr	x6, [x22, 96]
	mov	w1, 0
	str	w5, [x2, 2616]
	str	w4, [x19, 15012]
	ldrsw	x3, [x19, 14940]
	lsl	x2, x3, 7
	sub	x2, x2, x3
	lsl	x2, x2, 5
	blr	x6
	add	x1, x19, 61440
	mov	w2, 40
	stp	wzr, wzr, [x20, 60]
	stp	wzr, wzr, [x20, 68]
	stp	wzr, wzr, [x20, 76]
	stp	wzr, wzr, [x20, 84]
	str	w2, [x1, 2772]
	add	x3, x1, 2560
	mov	w0, 17
	str	w0, [x1, 2776]
	str	wzr, [x20, 52]
	mov	w2, 25
	str	wzr, [x1, 2780]
	mov	x0, x19
	str	wzr, [x1, 2788]
	str	wzr, [x1, 2800]
	stp	xzr, xzr, [x3, 248]
	stp	xzr, xzr, [x3, 264]
	str	wzr, [x20, 120]
	str	wzr, [x1, 2792]
	str	w2, [x1, 2796]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x20, 116]
	beq	.L3649
	mov	w1, 0
	mov	x0, x19
	bl	ClearAll
	mov	w0, 1
.L3641:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3647:
	ldr	w0, [x21, 684]
	str	w0, [x19, 14944]
	ldr	w0, [x21, 688]
	str	w0, [x19, 14948]
	ldr	w0, [x21, 676]
	str	w0, [x19, 14940]
	b	.L3639
.L3648:
	ldr	x2, .LC862
	mov	w3, 13089
	ldr	x1, .LC864
	mov	w0, 0
	add	x2, x2, 272
	bl	dprint_vfmw
	mov	w0, -20
	b	.L3641
	.p2align 3
.L3649:
	ldr	x1, .LC866
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L3641
	.size	H264DEC_Init, .-H264DEC_Init
	.align	3
.LC861:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC862:
	.xword	.LANCHOR1
	.align	3
.LC864:
	.xword	.LC863
	.align	3
.LC866:
	.xword	.LC865
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	InitPic
	.type	InitPic, %function
InitPic:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	mov	x21, x0
	mov	x0, x30
	add	x23, x21, 20480
	mov	w25, 2208
	bl	_mcount
	add	x19, x21, 61440
	ldr	x0, [x21, 14968]
	mov	w20, 4000
	ldr	w1, [x23, 1596]
	add	x22, x21, 65536
	ldrb	w2, [x19, 112]
	ldr	x3, [x21, 14960]
	umull	x1, w1, w25
	add	x25, x0, x1
	ldr	w0, [x25, 28]
	umull	x0, w0, w20
	add	x20, x3, x0
	cbz	w2, .L3651
	ldr	x0, [x19, 752]
	cbz	x0, .L3652
	ldrb	w1, [x0, 1]
	cbz	w1, .L3785
	strb	wzr, [x19, 112]
.L3651:
	add	x1, x29, 108
	mov	x0, x21
	bl	GetReRangeFlag
	mov	w26, w0
	ldr	w1, [x21, 15012]
	ldr	w0, [x20, 36]
	cmp	w1, w0
	beq	.L3786
.L3654:
	ldr	x1, .LC870
	add	x0, x21, 12288
	strb	wzr, [x20, 24]
	mov	x2, 4000
	add	x0, x0, 2688
	ldr	x3, [x1, 112]
	mov	x1, x20
	blr	x3
	ldr	w0, [x29, 108]
	cbz	w0, .L3656
.L3796:
	ldr	x0, [x21]
	ldr	w1, [x0, 708]
	cbnz	w1, .L3657
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L3787
.L3657:
	ldr	w0, [x22, 76]
	add	w0, w0, 1
	str	w0, [x22, 76]
	cbnz	w26, .L3669
	ldr	x0, [x21]
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L3669
	mov	x0, x21
	bl	FlushDPB
	cbnz	w0, .L3788
.L3672:
	cbz	x21, .L3717
.L3790:
	mov	x0, x21
	bl	ClearAllSlice
	mov	x0, x21
	bl	InitDPB.part.12
	cbnz	w0, .L3673
	cbnz	w26, .L3675
	ldr	x0, [x21]
	add	x24, x21, 16384
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L3675
.L3668:
	ldr	w1, [x24, 2616]
	ldr	w0, [x25, 24]
	cmp	w1, w0
	bne	.L3679
	ldrb	w0, [x25, 20]
	cbnz	w0, .L3679
.L3680:
	ldr	w0, [x23, 728]
	ldr	w3, [x24, 788]
	cbz	w0, .L3681
	str	wzr, [x23, 1600]
	str	wzr, [x19, 1560]
.L3681:
	ldr	w0, [x22, 44]
	cmp	w0, 2
	beq	.L3682
	ldr	x2, [x21]
	ldr	w0, [x2, 12]
	sub	w0, w0, #2
	cmp	w0, 1
	bls	.L3682
	ldr	w1, [x23, 1600]
	ldr	w4, [x23, 1604]
	cmp	w4, w1
	beq	.L3683
	mov	w0, 1
	add	w3, w3, 4
	lsl	w3, w0, w3
	add	w5, w1, w0
	udiv	w0, w5, w3
	msub	w0, w0, w3, w5
	cmp	w4, w0
	beq	.L3718
	ldr	w0, [x2, 600]
	cbnz	w0, .L3684
	ldr	w0, [x2, 640]
	cmp	w0, 1
	beq	.L3789
.L3685:
	cmp	w4, w1
	bls	.L3686
	ldr	w0, [x24, 1864]
	sub	w1, w4, w1
	cmp	w0, w1
	bcs	.L3687
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	bne	.L3692
	cbnz	w4, .L3690
	sub	w4, w3, #1
.L3691:
	mov	x0, x21
	str	w4, [x23, 1600]
	bl	FlushDPB
	cbz	w0, .L3687
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6873
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
	.p2align 3
.L3679:
	ldr	x0, .LC870
	mov	x2, 2208
	strb	wzr, [x25, 20]
	mov	x1, x25
	ldr	x3, [x0, 112]
	add	x0, x24, 2592
	blr	x3
	b	.L3680
	.p2align 3
.L3669:
	ldr	x1, .LC881
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x22, 160]
	cmp	w0, 1
	bne	.L3672
	mov	x0, x21
	bl	GetBackPicFromVOQueue
	cbnz	x21, .L3790
	.p2align 2
.L3717:
	mov	w0, -1
.L3673:
	ldr	x1, .LC873
	mov	w3, w0
	mov	w2, 6792
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
.L3718:
	mov	w1, w4
	.p2align 2
.L3683:
	ldr	w0, [x23, 1616]
	cbz	w0, .L3702
	str	w1, [x23, 1600]
.L3702:
	mov	x0, x21
	str	w1, [x23, 768]
	bl	DecPOC
	mov	w1, 0
	mov	x0, x21
	bl	AllocFrameStore
	cbnz	w0, .L3791
	strb	wzr, [x19, 119]
	mov	w0, 1
	strb	w0, [x19, 112]
	mov	w1, -1
	mov	w4, 1
	ldrb	w0, [x23, 1587]
	strb	w0, [x19, 118]
	ldr	w5, [x23, 1604]
	str	w5, [x19, 768]
	ldr	w3, [x23, 1616]
	str	w3, [x19, 776]
	ldr	w0, [x23, 748]
	str	w0, [x19, 780]
	ldr	w0, [x23, 744]
	str	w0, [x19, 784]
	ldr	w0, [x23, 736]
	str	w0, [x19, 788]
	ldr	w0, [x23, 740]
	str	w0, [x19, 792]
	strb	wzr, [x19, 114]
	strb	wzr, [x19, 117]
	strb	wzr, [x19, 116]
	str	w1, [x19, 824]
	ldrb	w2, [x23, 1585]
	cbz	w2, .L3704
	strb	w4, [x23, 1585]
	mov	w2, w4
	mov	w4, 2
.L3704:
	add	x25, x21, 12288
	ldr	w6, [x24, 1844]
	mov	w1, 2
	ldrb	w0, [x25, 2706]
	sub	w0, w1, w0
	madd	w0, w6, w0, w0
	str	w0, [x19, 816]
	udiv	w0, w0, w4
	str	w0, [x19, 812]
	ldr	w1, [x24, 1840]
	add	w1, w1, 1
	str	w1, [x19, 808]
	mul	w0, w0, w1
	str	w0, [x19, 820]
	ldrb	w0, [x24, 2592]
	strb	w0, [x19, 120]
	cbnz	w2, .L3720
	ldrb	w0, [x25, 2707]
	mov	w1, 0
	cbnz	w0, .L3792
.L3705:
	strb	w0, [x19, 121]
	str	w1, [x22, 24]
	cbz	w3, .L3706
	str	w5, [x23, 1600]
.L3706:
	add	x2, x21, 24576
	mov	x23, -1
	ldr	x1, .LC890
	mov	w0, 29
	ldr	w3, [x2, 3188]
	lsl	x2, x3, 6
	sub	x2, x2, x3
	lsl	x2, x2, 2
	sub	x2, x2, x3
	lsl	x2, x2, 1
	sub	x2, x2, x3
	add	x2, x21, x2, lsl 2
	add	x2, x2, 20480
	add	x2, x2, 3276
	str	x2, [x19, 760]
	ldr	x2, [x21]
	ldr	x3, [x2, 56]
	str	x3, [x19, 728]
	ldr	x3, [x2, 72]
	str	x3, [x19, 736]
	ldr	x3, [x2, 88]
	str	x3, [x19, 744]
	ldr	w3, [x2, 104]
	str	w3, [x19, 136]
	ldr	w3, [x2, 108]
	str	w3, [x19, 140]
	ldr	w3, [x2, 112]
	str	w3, [x19, 144]
	ldr	w3, [x2, 128]
	str	w3, [x19, 148]
	ldr	w3, [x20, 44]
	str	w3, [x19, 304]
	ldr	w3, [x20, 48]
	str	w3, [x19, 308]
	str	x23, [x2, 56]
	ldr	x2, [x19, 728]
	bl	dprint_vfmw
	ldr	x1, .LC892
	mov	w0, 29
	ldr	x2, [x19, 736]
	bl	dprint_vfmw
	ldr	w1, [x24, 1840]
	mov	w2, 2
	add	x0, x19, 128
	add	w1, w1, 1
	lsl	w1, w1, 4
	str	w1, [x19, 240]
	ldrb	w1, [x25, 2706]
	ldr	w3, [x24, 1844]
	sub	w1, w2, w1
	madd	w1, w3, w1, w1
	lsl	w1, w1, 4
	str	w1, [x19, 244]
	ldr	w2, [x21, 15016]
	ldr	w1, [x24, 1908]
	cmp	w2, 1
	ldr	w3, [x24, 1912]
	cset	w2, ne
	and	w1, w1, 7
	lsl	w4, w1, 14
	ubfiz	w3, w3, 5, 3
	orr	w2, w4, w2, lsl 2
	orr	w2, w2, w3
	str	w2, [x19, 236]
	bl	SetAspectRatio
	ldrb	w2, [x19, 115]
	ldr	w1, [x22, 76]
	cmp	w2, wzr
	ldr	x0, [x19, 752]
	str	w1, [x19, 292]
	cset	w1, ne
	str	w1, [x19, 224]
	cbz	x0, .L3793
	ldr	w1, [x19, 236]
	str	w1, [x0, 116]
	ldr	x0, [x19, 752]
	ldr	w1, [x19, 224]
	str	w1, [x0, 104]
	ldr	x0, [x19, 752]
	ldr	w1, [x19, 132]
	str	w1, [x0, 12]
	ldr	x0, [x19, 752]
	ldr	w1, [x19, 128]
	str	w1, [x0, 8]
	ldr	x0, [x19, 752]
	ldr	w1, [x19, 240]
	str	w1, [x0, 120]
	ldr	x0, [x19, 752]
	ldr	w1, [x19, 244]
	str	w1, [x0, 124]
	ldr	x1, [x19, 752]
	ldr	w0, [x20, 3284]
	str	w0, [x1, 136]
	str	w0, [x19, 256]
	ldr	x1, [x19, 752]
	ldr	w0, [x20, 3288]
	str	w0, [x1, 140]
	str	w0, [x19, 260]
	ldr	x1, [x19, 752]
	ldr	w0, [x20, 3276]
	str	w0, [x1, 128]
	str	w0, [x19, 248]
	ldr	x1, [x19, 752]
	ldr	w0, [x20, 3280]
	str	w0, [x1, 132]
	str	w0, [x19, 252]
	ldr	x1, [x19, 752]
	ldr	w0, [x22, 116]
	ldr	w1, [x1, 632]
	bl	FSP_GetDispPhyFs
	cbz	x0, .L3708
	ldr	w3, [x19, 224]
	mov	w1, 0
	ldr	w2, [x0, 8]
	cmp	w3, wzr
	ldr	w0, [x22, 116]
	cset	w3, ne
	bl	FSP_SetStoreType
.L3708:
	ldr	w1, [x19, 2800]
	add	x4, x19, 688
	add	x5, x19, 720
.L3712:
	ldr	x0, [x4]
	cbnz	x0, .L3709
	cbz	w1, .L3709
	ldr	x0, [x19, 2808]
	subs	w1, w1, #1
	str	x0, [x4]
	beq	.L3710
	add	x3, x19, 2808
	mov	w2, 0
	.p2align 2
.L3711:
	add	w0, w2, 1
	mov	x2, x0
	cmp	w0, w1
	add	x0, x0, 4096
	add	x0, x21, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	x0, [x3], 8
	bne	.L3711
.L3710:
	uxtw	x0, w1
	add	x0, x0, 4096
	add	x0, x21, x0, lsl 3
	str	xzr, [x0, 31480]
	str	w1, [x19, 2800]
.L3709:
	add	x4, x4, 8
	cmp	x5, x4
	bne	.L3712
	cbz	w1, .L3714
	cmp	w1, wzr
	ble	.L3714
	.p2align 2
.L3715:
	add	x23, x21, x1, sxtw 3
	sub	w20, w1, #1
	add	x23, x23, 61440
	ldr	w0, [x22, 116]
	ldr	x1, [x23, 2800]
	bl	FreeUsdByDec
	str	xzr, [x23, 2800]
	mov	w1, w20
	cbnz	w20, .L3715
.L3714:
	mov	x0, x21
	str	wzr, [x19, 796]
	str	wzr, [x19, 2800]
	bl	CalcPicNum
	mov	w0, 0
.L3659:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L3694:
	cbnz	w4, .L3696
	sub	w4, w3, #1
.L3697:
	mov	x0, x21
	str	w4, [x23, 1600]
	bl	FlushDPB
	cbnz	w0, .L3794
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	bne	.L3699
	.p2align 2
.L3684:
	mov	x0, x21
	bl	DecGap
	cbnz	w0, .L3795
.L3682:
	ldr	w1, [x23, 1604]
	b	.L3683
	.p2align 3
.L3786:
	ldrb	w0, [x20, 24]
	cbnz	w0, .L3654
	ldr	w0, [x29, 108]
	cbnz	w0, .L3796
.L3656:
	ldr	x0, [x21]
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L3657
	add	x24, x21, 16384
	b	.L3668
	.p2align 3
.L3720:
	mov	w1, 0
	mov	w0, 0
	b	.L3705
	.p2align 3
.L3675:
	add	x24, x21, 16384
	add	x1, x21, 12288
	add	x3, x19, 2772
	ldr	w4, [x24, 1864]
	str	w4, [x19, 2776]
	ldr	w0, [x24, 1844]
	ldrb	w26, [x1, 2706]
	mov	w1, 2
	add	w0, w0, 1
	ldr	w27, [x24, 1840]
	sub	w26, w1, w26
	lsl	w0, w0, 4
	add	w27, w27, 1
	mul	w26, w0, w26
	lsl	w27, w27, 4
	mov	x0, x21
	mov	w1, w27
	mov	w2, w26
	bl	H264_ArrangeVahbMem
	cmp	w0, 1
	beq	.L3797
.L3781:
	ldr	x0, [x21]
	ldr	w0, [x0, 872]
	cmp	w0, 1
	beq	.L3798
	ldr	x1, .LC879
	mov	w0, 0
	str	wzr, [x21, 14932]
	str	wzr, [x21, 14936]
	bl	dprint_vfmw
	mov	w1, 0
	mov	x0, x21
	bl	ClearAll
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3792:
	mov	w1, 1
	mov	w0, w1
	b	.L3705
	.p2align 3
.L3785:
	ldr	w1, [x0, 632]
	mov	w2, 1
	ldr	w0, [x22, 116]
	bl	FSP_ClearLogicFs
	ldr	x0, [x19, 752]
	strb	wzr, [x0, 1]
	ldr	x0, [x19, 752]
	strb	wzr, [x0, 4]
	strb	wzr, [x19, 112]
	b	.L3651
	.p2align 3
.L3798:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, -2
	ldp	x29, x30, [sp], 112
	ret
.L3787:
	ldr	w0, [x22, 76]
	add	w0, w0, 1
	str	w0, [x22, 76]
	cbnz	w26, .L3799
.L3658:
	cbz	x21, .L3716
	mov	x0, x21
	bl	ClearAllSlice
	mov	x0, x21
	bl	InitDPB.part.12
	mov	w3, w0
	cbnz	w0, .L3660
	add	x24, x21, 16384
	add	x1, x21, 12288
	mov	w2, 2
	ldrb	w27, [x1, 2706]
	ldr	w1, [x24, 1844]
	ldr	w28, [x24, 1840]
	sub	w27, w2, w27
	add	w1, w1, 1
	lsl	w1, w1, 4
	add	w28, w28, 1
	lsl	w28, w28, 4
	mul	w27, w1, w27
	cbz	w26, .L3662
	ldr	w1, [x21, 14932]
	cbnz	w1, .L3662
	ldr	w4, [x24, 1864]
	str	w4, [x19, 2776]
	ldr	x3, [x21]
	ldr	w1, [x3, 668]
	cmp	w28, w1
	bhi	.L3663
	ldr	w2, [x3, 672]
	cmp	w27, w2
	bhi	.L3663
	add	x5, x21, 24576
	ldr	w6, [x3, 692]
	ldr	w5, [x5, 3712]
	cmp	w6, w5
	bcc	.L3800
	ldr	w0, [x3, 696]
	cbnz	w0, .L3666
	mov	w4, 1
	str	w4, [x19, 2776]
	ldr	w1, [x3, 668]
	ldr	w2, [x3, 672]
.L3666:
	add	x3, x19, 2772
	mov	x0, x21
	bl	H264_ArrangeVahbMem
	cmp	w0, 1
	bne	.L3781
.L3662:
	lsr	w28, w28, 4
	lsr	w27, w27, 4
	str	w28, [x21, 14932]
	str	w27, [x21, 14936]
	b	.L3668
	.p2align 3
.L3797:
	lsr	w27, w27, 4
	lsr	w26, w26, 4
	str	w27, [x21, 14932]
	str	w26, [x21, 14936]
	b	.L3668
.L3686:
	sub	w0, w4, w1
	ldr	w1, [x24, 1864]
	add	w0, w0, w3
	cmp	w1, w0
	bcs	.L3684
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	beq	.L3694
.L3699:
	ldr	w0, [x22, 112]
	cbnz	w0, .L3684
	mov	x0, x21
	bl	ClearAllSlice
	ldr	x0, .LC882
	mov	w1, 23352
	ldr	w2, [x21, 14948]
	movk	w1, 0x1, lsl 16
	mov	w6, 2
	mov	w5, 7
	ldrb	w0, [x0]
	mov	w4, 32
	mov	w3, 3
	str	w2, [x23, 3240]
	str	w1, [x23, 3248]
	mov	w2, -1
	mov	w1, 262144
	strb	wzr, [x23, 3232]
	strb	w6, [x23, 3233]
	strb	w4, [x23, 3234]
	str	w5, [x23, 3244]
	str	w3, [x23, 3256]
	str	wzr, [x23, 3264]
	str	wzr, [x23, 3268]
	str	w2, [x23, 3260]
	str	w1, [x23, 3272]
	cbnz	w0, .L3700
	mov	x0, x21
	bl	ClearDPB
.L3700:
	ldr	w0, [x23, 1600]
	mov	w2, 6922
	ldr	w3, [x23, 1604]
	ldr	x1, .LC884
	ldr	w4, [x24, 1864]
	sub	w3, w3, w0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
	.p2align 3
.L3652:
	ldr	x2, .LC867
	mov	w3, 6623
	ldr	x1, .LC869
	mov	w0, 22
	add	x2, x2, 288
	bl	dprint_vfmw
	strb	wzr, [x19, 112]
	b	.L3651
.L3687:
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	beq	.L3684
.L3692:
	ldr	w0, [x22, 112]
	cbnz	w0, .L3684
	mov	x0, x21
	bl	ClearAllSlice
	ldr	x0, .LC882
	mov	w1, 23352
	ldr	w2, [x21, 14948]
	movk	w1, 0x1, lsl 16
	mov	w6, 2
	mov	w5, 7
	ldrb	w0, [x0]
	mov	w4, 32
	mov	w3, 3
	str	w2, [x23, 3240]
	str	w1, [x23, 3248]
	mov	w2, -1
	mov	w1, 262144
	strb	wzr, [x23, 3232]
	strb	w6, [x23, 3233]
	strb	w4, [x23, 3234]
	str	w5, [x23, 3244]
	str	w3, [x23, 3256]
	str	wzr, [x23, 3264]
	str	wzr, [x23, 3268]
	str	w2, [x23, 3260]
	str	w1, [x23, 3272]
	cbz	w0, .L3801
.L3693:
	ldr	w0, [x23, 1600]
	mov	w2, 6890
	ldr	w3, [x23, 1604]
	ldr	x1, .LC884
	ldr	w4, [x24, 1864]
	sub	w3, w3, w0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
	.p2align 3
.L3716:
	mov	w3, -1
.L3660:
	ldr	x1, .LC873
	mov	w2, 6666
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
	.p2align 3
.L3799:
	mov	x0, x21
	bl	FlushDPB
	cbz	w0, .L3658
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6657
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
	.p2align 3
.L3788:
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6783
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
.L3795:
	ldr	x1, .LC886
	mov	w2, 6934
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
	.p2align 3
.L3789:
	ldr	w0, [x2, 636]
	cbnz	w0, .L3685
	ldr	w0, [x2, 644]
	cbnz	w0, .L3685
	ldr	w0, [x2, 604]
	add	w0, w0, 1024
	cmp	w0, 2048
	bhi	.L3684
	b	.L3685
.L3791:
	ldr	x1, .LC888
	mov	w2, 6958
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
.L3793:
	ldr	x2, .LC867
	mov	w3, 7045
	ldr	x1, .LC894
	mov	w0, 22
	add	x2, x2, 288
	bl	dprint_vfmw
	mov	w0, w23
	b	.L3659
.L3696:
	sub	w4, w4, #1
	b	.L3697
.L3690:
	sub	w4, w4, #1
	b	.L3691
.L3663:
	ldr	x1, .LC875
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
.L3800:
	ldr	x1, .LC877
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
.L3794:
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6906
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3659
.L3801:
	mov	x0, x21
	bl	ClearDPB
	b	.L3693
	.size	InitPic, .-InitPic
	.align	3
.LC867:
	.xword	.LANCHOR1
	.align	3
.LC869:
	.xword	.LC868
	.align	3
.LC870:
	.xword	vfmw_Osal_Func_Ptr_S
	.align	3
.LC871:
	.xword	.LC786
	.align	3
.LC873:
	.xword	.LC872
	.align	3
.LC875:
	.xword	.LC874
	.align	3
.LC877:
	.xword	.LC876
	.align	3
.LC879:
	.xword	.LC878
	.align	3
.LC881:
	.xword	.LC880
	.align	3
.LC882:
	.xword	g_apple_airplay_mirrorinig_flag
	.align	3
.LC884:
	.xword	.LC883
	.align	3
.LC886:
	.xword	.LC885
	.align	3
.LC888:
	.xword	.LC887
	.align	3
.LC890:
	.xword	.LC889
	.align	3
.LC892:
	.xword	.LC891
	.align	3
.LC894:
	.xword	.LC893
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_DecSlice
	.type	H264_DecSlice, %function
H264_DecSlice:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 65536
	add	x21, x19, 20480
	bl	_mcount
	ldr	x1, .LC895
	mov	w0, 22
	ldr	w2, [x20, 60]
	sub	w2, w2, #1
	bl	dprint_vfmw
	ldr	x1, [x19, 632]
	mov	x0, x19
	ldrb	w2, [x1, 2]
	strb	w2, [x21, 1587]
	ldr	w1, [x1, 84]
	str	w1, [x21, 1616]
	bl	ProcessSliceHeaderFirstPart
	cbnz	w0, .L3850
	mov	x0, x19
	bl	ProcessSliceHeaderSecondPart
	cbnz	w0, .L3850
	ldr	w2, [x20, 40]
	mov	w3, 0
	add	x22, x19, 61440
	cbz	w2, .L3809
	sub	w9, w2, #1
	add	x22, x19, 61440
	add	x9, x9, 4096
	add	x4, x22, 1616
	add	x9, x9, 3787
	mov	w10, 0
	mov	w3, 0
	mov	x0, 0
	add	x9, x19, x9, lsl 3
	.p2align 2
.L3817:
	ldr	x1, [x4]
	ldr	x6, [x1, 8]
	cbz	x6, .L3831
	ldr	x5, [x1, 40]
	ldr	w7, [x1, 16]
	cbz	x5, .L3832
.L3857:
	ldr	w1, [x1, 48]
.L3812:
	cbz	x0, .L3813
	cmp	x6, x0
	mov	w8, w7
	bls	.L3814
	sub	x0, x6, x0
	sub	w0, w0, w10
	add	w8, w0, w7
.L3814:
	add	w3, w1, w3
	add	w3, w8, w3
.L3815:
	cbz	x5, .L3851
	mov	x0, x5
	mov	w10, w1
.L3816:
	add	x4, x4, 8
	cmp	x9, x4
	bne	.L3817
.L3809:
	str	w3, [x20, 100]
	ldr	w0, [x22, 808]
	ldr	w4, [x22, 812]
	mul	w4, w0, w4
	cmp	w4, 1620
	bgt	.L3852
	lsl	w0, w4, 2
	mov	w1, 32769
	sub	w4, w0, w4
	mov	w0, 16384
	lsl	w4, w4, 8
	cmp	w4, w1
	asr	w4, w4, 1
	csel	w4, w4, w0, gt
.L3819:
	ldr	w0, [x19, 14940]
	cmp	w0, w2
	bls	.L3820
	cmp	w4, w3
	bcc	.L3820
	ldrb	w0, [x21, 1589]
	cbz	w0, .L3823
	ldr	x1, .LC903
	mov	w0, 2
	bl	dprint_vfmw
	ldrb	w0, [x21, 1587]
	cmp	w0, 5
	ldr	w0, [x20, 80]
	beq	.L3853
.L3825:
	add	w0, w0, 1
	str	wzr, [x20, 96]
	str	w0, [x20, 80]
	ldrb	w0, [x22, 115]
	cbnz	w0, .L3826
	ldr	w0, [x20, 84]
	add	w0, w0, 1
	str	w0, [x20, 84]
.L3827:
	mov	x0, x19
	bl	InitPic
	cmn	w0, #2
	beq	.L3806
	cbnz	w0, .L3854
	mov	x0, x19
	bl	WritePicMsg
.L3823:
	mov	x0, x19
	bl	PicTypeStatistic
	mov	x0, x19
	bl	DecList
	cbnz	w0, .L3855
	mov	x0, x19
	bl	IsRefListWrong
	cbnz	w0, .L3856
	mov	x0, x19
	bl	ExitSlice
	mov	x0, x19
	bl	CalcStreamBits
	mov	x0, x19
	bl	WriteSliceMsg
	mov	w0, 1
	str	w0, [x22, 2780]
	mov	w0, 0
	ldr	w1, [x20, 96]
	add	w1, w1, 1
	str	w1, [x20, 96]
.L3806:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3851:
	mov	x0, x6
	mov	w10, w7
	b	.L3816
	.p2align 3
.L3813:
	add	w3, w7, w3
	add	w3, w3, w1
	b	.L3815
	.p2align 3
.L3831:
	ldr	x5, [x1, 40]
	mov	w7, 0
	cbnz	x5, .L3857
	.p2align 2
.L3832:
	mov	w1, 0
	b	.L3812
	.p2align 3
.L3852:
	lsl	w4, w4, 8
	b	.L3819
	.p2align 3
.L3826:
	ldr	w0, [x20, 88]
	add	w0, w0, 1
	str	w0, [x20, 88]
	b	.L3827
	.p2align 3
.L3853:
	str	w0, [x20, 92]
	b	.L3825
.L3850:
	ldr	x1, .LC896
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC897
	ldr	x4, [x0]
	cbz	x4, .L3848
	add	x2, x29, 64
	ldr	w0, [x20, 116]
	mov	w5, -1
	mov	w3, 4
	mov	w1, 100
	str	w5, [x2, -8]!
	blr	x4
.L3848:
	mov	x0, x19
	bl	ClearCurrSlice
	mov	w0, -1
	b	.L3806
.L3820:
	ldr	x1, .LC899
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x19, 14940]
	ldr	w2, [x20, 40]
	cmp	w2, w1
	bcc	.L3822
	ldr	x0, .LC897
	ldr	x4, [x0]
	cbz	x4, .L3822
	ldr	w0, [x20, 116]
	mov	w3, 8
	stp	w2, w1, [x29, 56]
	add	x2, x29, 56
	mov	w1, 108
	blr	x4
.L3822:
	ldr	x1, .LC901
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearAllSlice
	mov	w3, 32
	ldr	w7, [x19, 14948]
	strb	w3, [x21, 3234]
	add	x3, x21, 3072
	mov	w6, 23352
	strb	wzr, [x21, 3232]
	str	w7, [x21, 3240]
	movk	w6, 0x1, lsl 16
	mov	w5, 2
	str	w6, [x21, 3248]
	strb	w5, [x21, 3233]
	mov	w4, 7
	str	w4, [x21, 3244]
	mov	w0, -1
	mov	w2, 3
	mov	w1, 262144
	stp	w2, w0, [x3, 184]
	stp	wzr, wzr, [x3, 192]
	str	w1, [x21, 3272]
	b	.L3806
.L3855:
	ldr	x1, .LC907
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3806
.L3856:
	ldr	x1, .LC909
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3848
.L3854:
	ldr	x1, .LC905
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3806
	.size	H264_DecSlice, .-H264_DecSlice
	.align	3
.LC895:
	.xword	.LC761
	.align	3
.LC896:
	.xword	.LC763
	.align	3
.LC897:
	.xword	g_event_report
	.align	3
.LC899:
	.xword	.LC898
	.align	3
.LC901:
	.xword	.LC900
	.align	3
.LC903:
	.xword	.LC902
	.align	3
.LC905:
	.xword	.LC904
	.align	3
.LC907:
	.xword	.LC906
	.align	3
.LC909:
	.xword	.LC908
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264_DecOneNal
	.type	H264_DecOneNal, %function
H264_DecOneNal:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	mov	w21, w1
	add	x19, x20, 65536
	bl	_mcount
	ldr	x3, [x20, 632]
	ldr	w0, [x3, 76]
	cbz	w0, .L3863
	ldr	w2, [x19, 100]
	mov	w0, 0
	.p2align 2
.L3862:
	ubfiz	x1, x0, 5, 32
	add	w0, w0, 1
	add	x1, x3, x1
	ldr	w1, [x1, 16]
	add	w2, w2, w1
	str	w2, [x19, 100]
	ldr	w1, [x3, 76]
	cmp	w1, w0
	bhi	.L3862
.L3863:
	add	x24, x20, 12288
	mov	w1, 32
	add	x23, x24, 2600
	strb	wzr, [x3]
	mov	x0, x23
	bl	BsGet
	mov	w22, w0
	ldr	x1, [x20, 632]
	and	w2, w0, 31
	ubfx	x4, x22, 5, 2
	ubfx	x3, x22, 7, 1
	ldr	w0, [x1, 72]
	add	w0, w0, 32
	str	w0, [x1, 72]
	ldr	x1, .LC911
	ldr	x0, [x20, 632]
	strb	w2, [x0, 2]
	mov	w0, 22
	ldr	x2, [x20, 632]
	str	w4, [x2, 84]
	ldr	x2, [x20, 632]
	str	w3, [x2, 88]
	str	wzr, [x19, 56]
	ldr	w2, [x19, 60]
	ldr	x3, [x20, 632]
	ldrb	w3, [x3, 2]
	bl	dprint_vfmw
	ldr	w0, [x19, 60]
	cmp	w0, 371
	beq	.L4015
	add	w0, w0, 1
	str	w0, [x19, 60]
	ldr	x0, [x20, 632]
	ldrb	w2, [x0, 2]
	cmp	w2, 20
	beq	.L4016
.L3864:
	sub	w0, w2, #1
	cmp	w0, 29
	bls	.L4017
.L3870:
	ldr	x1, .LC952
	mov	w3, w22
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x21, .LC925
	cmp	w22, 256
	beq	.L3899
	ldr	x4, [x21]
	cbz	x4, .L3900
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L3899:
	ldr	x1, [x20, 632]
	cbz	x1, .L3901
.L3902:
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3901:
	ldr	x4, [x21]
	cbz	x4, .L4003
.L4002:
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L4003:
	mov	w0, 0
.L4018:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4017:
	ldr	x1, .LC920
	ldrh	w0, [x1,w0,uxtw #1]
	adr	x1, .Lrtx3872
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx3872:
	.section	.rodata
	.align	0
	.align	2
.L3872:
	.2byte	(.L3871 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3871 - .Lrtx3872) / 4
	.2byte	(.L3873 - .Lrtx3872) / 4
	.2byte	(.L3874 - .Lrtx3872) / 4
	.2byte	(.L3875 - .Lrtx3872) / 4
	.2byte	(.L3876 - .Lrtx3872) / 4
	.2byte	(.L3877 - .Lrtx3872) / 4
	.2byte	(.L3878 - .Lrtx3872) / 4
	.2byte	(.L3879 - .Lrtx3872) / 4
	.2byte	(.L3880 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3874 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3881 - .Lrtx3872) / 4
	.2byte	(.L3871 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3870 - .Lrtx3872) / 4
	.2byte	(.L3882 - .Lrtx3872) / 4
	.text
.L3882:
	ldr	x1, .LC950
	mov	w2, 30
	mov	w0, 22
	bl	dprint_vfmw
	cbnz	w21, .L3897
	ldr	x1, [x20, 632]
	cbz	x1, .L3897
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3897:
	mov	w1, 32
	mov	x0, x23
	bl	BsGet
	mov	w21, w0
	mov	w1, 32
	mov	x0, x23
	bl	BsGet
	mov	w2, 20553
	mov	w1, 20036
	movk	w2, 0x4853, lsl 16
	movk	w1, 0x4345, lsl 16
	cmp	w21, w2
	ccmp	w0, w1, 0, eq
	bne	.L4003
	ldr	x21, .LC925
	ldr	x4, [x21]
	cbz	x4, .L3898
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3898:
	mov	x0, x20
	str	wzr, [x20, 624]
	bl	DecVDM
	cbz	w0, .L4003
.L4010:
	ldr	x4, [x21]
	cbz	x4, .L3999
.L4000:
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3999:
	mov	w0, -1
.L4022:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L3881:
	ldr	x1, .LC948
	mov	w2, 19
	mov	w0, 22
	bl	dprint_vfmw
.L4008:
	ldr	x1, [x20, 632]
	cbz	x1, .L4003
.L4001:
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
	mov	w0, 0
.L3997:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L3879:
	ldr	x1, .LC942
	mov	w2, 12
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4001
	mov	w0, 0
	b	.L4018
.L3880:
	ldr	x1, .LC946
	mov	w2, 13
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4001
	mov	w0, 0
	b	.L4018
.L3878:
	ldr	x1, .LC944
	mov	w2, 11
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_IsEosNal
	uxtb	w1, w0
	cmp	w1, 1
	bne	.L4008
	ldr	x21, .LC925
	strb	w0, [x24, 2640]
	ldr	x4, [x21]
	cbz	x4, .L3896
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 117
	blr	x4
.L3896:
	mov	x0, x20
	str	wzr, [x20, 624]
	bl	DecVDM
	cbz	w0, .L4008
	b	.L4010
.L3877:
	ldr	x1, .LC940
	mov	w2, 10
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4001
	mov	w0, 0
	b	.L4018
.L3876:
	ldr	x1, .LC938
	mov	w2, 9
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4001
	mov	w0, 0
	b	.L4018
.L3875:
	ldr	x1, .LC927
	mov	w2, 8
	ldr	w3, [x19, 68]
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_DecPPS
	mov	w21, w0
	ldr	x1, [x20, 632]
	cbz	x1, .L3889
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3889:
	cbnz	w21, .L4019
	ldr	w1, [x19, 68]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 68]
	b	.L3997
.L3874:
	ldr	x1, .LC930
	mov	w0, 22
	ldr	w3, [x19, 64]
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_DecSPS
	mov	w21, w0
	ldr	x1, [x20, 632]
	cbz	x1, .L3891
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3891:
	cbnz	w21, .L4020
	ldr	w1, [x19, 64]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 64]
	b	.L3997
.L3873:
	ldr	x1, .LC934
	mov	w2, 6
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_DecSEI
	mov	w21, w0
	ldr	x1, [x20, 632]
	cbz	x1, .L3893
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3893:
	cbz	w21, .L4003
	ldr	x1, .LC936
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC925
	ldr	x4, [x0]
	cbnz	x4, .L4002
	mov	w0, 0
	b	.L4018
.L3871:
	ldr	x1, .LC922
	mov	w0, 22
	ldr	w3, [x19, 72]
	add	x21, x20, 20480
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	mov	x0, x20
	ldrb	w2, [x1, 2]
	strb	w2, [x21, 1587]
	ldr	w2, [x1, 84]
	str	w2, [x21, 1616]
	ldrb	w1, [x1, 5]
	strb	w1, [x21, 1592]
	bl	SliceCheck
	cbnz	w0, .L4021
	mov	x0, x20
	bl	H264_DecSlice
	cmn	w0, #2
	beq	.L3997
	cbz	w0, .L3885
	ldr	x0, .LC925
	ldr	x4, [x0]
	cbnz	x4, .L4000
	mov	w0, -1
	b	.L4022
	.p2align 3
.L4015:
	ldr	x1, .LC913
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x19, 60]
	add	w0, w0, 1
	str	w0, [x19, 60]
	ldr	x0, [x20, 632]
	ldrb	w2, [x0, 2]
	cmp	w2, 20
	bne	.L3864
	.p2align 2
.L4016:
	mov	w1, 24
	mov	x0, x23
	bl	BsGet
	mov	w22, w0
	ldr	x1, [x20, 632]
	ubfx	x2, x22, 23, 1
	add	x0, x20, 61440
	strb	w2, [x1, 3]
	ldr	x2, [x20, 632]
	ldrb	w1, [x2, 3]
	cbz	w1, .L3865
	ubfx	x3, x22, 22, 1
	ubfx	x1, x22, 16, 6
	strb	w3, [x0, 1600]
	ubfx	x3, x22, 15, 1
	strb	w1, [x0, 1601]
	ubfx	x1, x22, 12, 3
	strb	w3, [x0, 1602]
	ubfx	x3, x22, 8, 4
	strb	w1, [x0, 1603]
	ubfx	x1, x22, 5, 3
	strb	w3, [x0, 1604]
	ubfx	x3, x22, 4, 1
	strb	w1, [x0, 1605]
	ubfx	x1, x22, 3, 1
	strb	w3, [x0, 1606]
	ubfx	x3, x22, 2, 1
	strb	w1, [x0, 1607]
	and	w1, w22, 3
	strb	w3, [x0, 1608]
	strb	w1, [x0, 1609]
.L3865:
	mov	x1, 16711680
	ldr	x3, [x0, 1600]
	movk	x1, 0xff, lsl 32
	and	x1, x3, x1
	cmp	x1, 65536
	beq	.L3866
	ldrb	w3, [x0, 1602]
	ldrb	w2, [x0, 1604]
	mov	w0, 1
	ldr	x1, .LC915
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbz	x1, .L3999
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
	mov	w0, -1
	b	.L3997
.L3866:
	ldrb	w1, [x0, 1600]
	cmp	w1, 1
	beq	.L4023
.L3869:
	ldrb	w4, [x0, 1605]
	ldrb	w3, [x0, 1603]
	ldrb	w2, [x0, 1604]
	mov	w0, 22
	ldr	x1, .LC917
	bl	dprint_vfmw
	ldr	x1, .LC919
	mov	w2, w22
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	ldr	w0, [x1, 72]
	add	w0, w0, 24
	str	w0, [x1, 72]
	ldr	x0, [x20, 632]
	ldrb	w2, [x0, 2]
	b	.L3864
.L4021:
	ldr	x1, .LC924
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x20
	bl	ClearCurrSlice
	ldr	x0, .LC925
	ldr	x4, [x0]
	cbnz	x4, .L4000
	mov	w0, -1
	b	.L4022
.L4020:
	ldr	x1, .LC932
.L4013:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, .LC925
	ldr	x4, [x21]
	cbz	x4, .L3999
	add	x2, x29, 80
	ldr	w0, [x19, 116]
	mov	w20, -1
	mov	w3, 4
	mov	w1, 100
	str	w20, [x2, -8]!
	blr	x4
	ldr	x4, [x21]
	cbz	x4, .L3999
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, w20
	b	.L3997
.L4019:
	ldr	x1, .LC928
	b	.L4013
.L4023:
	mov	w1, 5
	strb	w1, [x2, 2]
	b	.L3869
.L3900:
	ldr	x1, [x20, 632]
	cbnz	x1, .L3902
	mov	w0, 0
	b	.L4018
.L3885:
	ldrb	w0, [x21, 1592]
	cmp	w0, 1
	bne	.L4003
	ldr	x21, .LC925
	ldr	x4, [x21]
	cbz	x4, .L3887
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3887:
	mov	x0, x20
	str	wzr, [x20, 624]
	bl	DecVDM
	cbnz	w0, .L4010
	mov	w1, 1
	mov	w0, 0
	strb	w1, [x24, 2641]
	b	.L3997
	.size	H264_DecOneNal, .-H264_DecOneNal
	.align	3
.LC911:
	.xword	.LC910
	.align	3
.LC913:
	.xword	.LC912
	.align	3
.LC915:
	.xword	.LC914
	.align	3
.LC917:
	.xword	.LC916
	.align	3
.LC919:
	.xword	.LC918
	.align	3
.LC920:
	.xword	.L3872
	.align	3
.LC922:
	.xword	.LC921
	.align	3
.LC924:
	.xword	.LC923
	.align	3
.LC925:
	.xword	g_event_report
	.align	3
.LC927:
	.xword	.LC926
	.align	3
.LC928:
	.xword	.LC399
	.align	3
.LC930:
	.xword	.LC929
	.align	3
.LC932:
	.xword	.LC931
	.align	3
.LC934:
	.xword	.LC933
	.align	3
.LC936:
	.xword	.LC935
	.align	3
.LC938:
	.xword	.LC937
	.align	3
.LC940:
	.xword	.LC939
	.align	3
.LC942:
	.xword	.LC941
	.align	3
.LC944:
	.xword	.LC943
	.align	3
.LC946:
	.xword	.LC945
	.align	3
.LC948:
	.xword	.LC947
	.align	3
.LC950:
	.xword	.LC949
	.align	3
.LC952:
	.xword	.LC951
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_DecodePacket
	.type	H264DEC_DecodePacket, %function
H264DEC_DecodePacket:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	str	wzr, [x29, 84]
	stp	wzr, wzr, [x29, 76]
	cbz	x22, .L4089
	ldr	x0, [x19]
	add	x20, x19, 65536
	add	x21, x19, 61440
	ldr	w2, [x0, 8]
	ubfiz	w2, w2, 16, 3
	str	w2, [x20, 104]
	ldr	w1, [x0]
	str	w1, [x20, 108]
	ldr	w1, [x0, 4]
	str	w1, [x20, 112]
	str	wzr, [x20, 56]
	ldr	w1, [x21, 2784]
	cmp	w1, 1
	beq	.L4027
	ldr	w1, [x0, 832]
	cmp	w1, 1
	beq	.L4090
.L4028:
	mov	x1, x22
	mov	x0, x19
	bl	ReceivePacket
	ldr	x1, [x19, 632]
	str	x1, [x20, 152]
.L4030:
	str	wzr, [x21, 2784]
	cbz	w0, .L4031
.L4050:
	mov	w0, 0
.L4085:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L4031:
	ldr	x0, [x19, 632]
.L4029:
	str	wzr, [x0, 72]
	add	x23, x19, 12288
	add	x22, x23, 2600
	ldr	x1, [x19, 632]
	mov	x0, x22
	ldr	w2, [x1, 16]
	ldr	x1, [x1, 8]
	bl	BsInit
	mov	w1, 32
	mov	x0, x22
	bl	BsGet
	uxtb	w0, w0
	ldr	x3, [x19, 632]
	and	w2, w0, 31
	mov	w1, 27
	and	w1, w0, w1
	ubfx	x0, x0, 5, 2
	strb	w2, [x3, 2]
	cmp	w1, 1
	ccmp	w2, 20, 4, ne
	ldr	x1, [x19, 632]
	str	w0, [x1, 84]
	beq	.L4091
.L4032:
	mov	x0, x19
	bl	FindTrailZeros
	cbnz	w0, .L4092
	ldr	x1, [x19, 632]
	mov	x0, x22
	str	wzr, [x1, 72]
	ldr	x1, [x19, 632]
	ldr	w2, [x1, 16]
	ldr	x1, [x1, 8]
	bl	BsInit
	mov	w1, 0
	mov	x0, x19
	bl	H264_DecOneNal
	cmn	w0, #2
	beq	.L4085
	cbz	w0, .L4050
	ldr	x1, .LC963
	mov	w0, 22
	bl	dprint_vfmw
	.p2align 2
.L4088:
	mov	w0, -1
	b	.L4085
	.p2align 3
.L4090:
	ldr	w0, [x0, 872]
	cmp	w0, 1
	bne	.L4028
	ldr	w0, [x20, 148]
	cmp	w0, 1
	bne	.L4028
	ldr	x0, [x20, 152]
	str	x0, [x19, 632]
	str	wzr, [x20, 148]
	str	wzr, [x21, 2784]
	b	.L4029
	.p2align 3
.L4027:
	ldr	x0, [x19, 632]
	cmp	x0, xzr
	csetm	w0, eq
	b	.L4030
	.p2align 3
.L4091:
	cmp	w2, 20
	beq	.L4093
.L4033:
	add	x2, x29, 92
	add	x1, x29, 88
	mov	x0, x19
	bl	InquireSliceProperty
	cbz	w0, .L4094
	ldr	x1, [x19, 632]
	cbz	x1, .L4048
	ldr	w0, [x20, 116]
	bl	ReleaseNAL
	str	xzr, [x19, 632]
.L4048:
	ldr	x0, .LC959
	ldr	x4, [x0]
	cbz	x4, .L4088
	ldr	w0, [x20, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, -1
	b	.L4085
	.p2align 3
.L4094:
	ldr	w1, [x29, 88]
	ldr	w0, [x20, 128]
	cmp	w1, 1
	csinc	w0, w0, w0, ne
	str	w0, [x20, 128]
	beq	.L4095
	ldrb	w0, [x23, 2641]
	cmp	w0, 1
	beq	.L4051
.L4038:
	ldr	w0, [x29, 92]
	cmp	w0, 1
	bne	.L4032
	mov	x0, x19
	bl	FlushDPB
	cbnz	w0, .L4096
.L4044:
	ldr	w0, [x20, 116]
	bl	FSP_GetFspType
	cbnz	w0, .L4032
	ldr	w0, [x20, 116]
	add	x3, x29, 84
	add	x2, x29, 80
	add	x1, x29, 76
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19]
	ldr	w0, [x0, 708]
	cmp	w0, 1
	bne	.L4032
	ldr	w1, [x29, 84]
	cmp	w1, wzr
	ble	.L4097
.L4045:
	str	wzr, [x20, 160]
.L4087:
	mov	w1, 1
	mov	w0, -2
	str	w1, [x21, 2784]
	b	.L4085
.L4097:
	ldr	w1, [x29, 80]
	cmp	w1, 32
	bgt	.L4045
	ldr	x1, [x19, 616]
	cbnz	x1, .L4045
	ldr	w3, [x20, 80]
	cbz	w3, .L4032
	ldr	w6, [x19, 14932]
	cbz	w6, .L4032
	ldr	w7, [x19, 14936]
	ldr	w5, [x20, 164]
	cmp	w7, w5
	beq	.L4032
	ldr	w4, [x20, 168]
	cmp	w6, w4
	beq	.L4032
	ldr	x1, .LC958
	ldr	w2, [x29, 92]
	bl	dprint_vfmw
	str	wzr, [x20, 160]
	ldr	x0, .LC959
	ldr	w1, [x19, 14936]
	str	w1, [x20, 164]
	ldr	x4, [x0]
	ldr	w0, [x19, 14932]
	str	w0, [x20, 168]
	cbz	x4, .L4032
	ldr	w0, [x20, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L4032
	.p2align 3
.L4051:
	strb	wzr, [x23, 2641]
	b	.L4038
	.p2align 3
.L4095:
	add	x24, x19, 20480
	ldrb	w0, [x24, 1585]
	cmp	w0, 1
	ldr	w0, [x20, 132]
	beq	.L4098
	add	w0, w0, 2
	str	w0, [x20, 132]
.L4037:
	ldrb	w0, [x23, 2641]
	cmp	w0, 1
	beq	.L4051
	ldr	w0, [x20, 40]
	cbz	w0, .L4038
	ldr	x3, [x19, 632]
	mov	w2, 12
	ldr	x1, .LC955
	mov	x0, x22
	add	x1, x1, 432
	str	wzr, [x3, 72]
	bl	BsInit
	mov	w1, 1
	mov	x0, x19
	bl	H264_DecOneNal
	ldrb	w0, [x24, 1585]
	cmp	w0, 1
	ldr	w0, [x20, 132]
	beq	.L4099
	sub	w0, w0, #2
	str	w0, [x20, 132]
	b	.L4087
	.p2align 3
.L4093:
	mov	x0, x19
	bl	InquireSliceExtProperty
	b	.L4033
	.p2align 3
.L4092:
	ldr	x1, .LC961
	mov	w0, 22
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4085
.L4098:
	add	w0, w0, 1
	str	w0, [x20, 132]
	b	.L4037
.L4096:
	ldr	x1, .LC956
	mov	w3, w0
	mov	w2, 13812
	mov	w0, 0
	bl	dprint_vfmw
	b	.L4044
.L4089:
	ldr	x2, .LC953
	mov	w3, 13676
	ldr	x1, .LC954
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L4085
.L4099:
	sub	w0, w0, #1
	str	w0, [x20, 132]
	b	.L4087
	.size	H264DEC_DecodePacket, .-H264DEC_DecodePacket
	.align	3
.LC953:
	.xword	.LC50
	.align	3
.LC954:
	.xword	.LC52
	.align	3
.LC955:
	.xword	.LANCHOR0
	.align	3
.LC956:
	.xword	.LC786
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_VDMPostProc
	.type	H264DEC_VDMPostProc, %function
H264DEC_VDMPostProc:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	mov	w23, w1
	mov	x24, x3
	mov	w25, w4
	mov	w21, w5
	bl	_mcount
	ldr	w20, [x19, 624]
	add	x22, x19, 61440
	cbnz	w20, .L4101
	str	w23, [x22, 804]
	cbz	w25, .L4102
	ldr	x5, .LC964
	sbfiz	x21, x21, 6, 32
	add	x21, x5, x21
	ldr	w0, [x21, 44]
	cmp	w0, 3
	beq	.L4118
	cmp	w0, 1
	beq	.L4102
	mov	x1, x24
	mov	x0, x19
	bl	StorePicInDPB
	cmp	w0, 1
	mov	w21, w0
	bls	.L4106
.L4121:
	ldr	x1, [x22, 752]
	add	x0, x19, 65536
	mov	w2, 1
	mov	w23, -1
	mov	w20, w23
	ldr	w0, [x0, 116]
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC966
	mov	w2, w21
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearAllSlice
	add	x1, x19, 20480
	ldr	w4, [x19, 14948]
	add	x5, x1, 3072
	mov	w2, 2
	mov	w3, 23352
	mov	w0, 7
	strb	w2, [x1, 3233]
	movk	w3, 0x1, lsl 16
	str	w0, [x1, 3244]
	mov	w2, 32
	strb	wzr, [x1, 3232]
	mov	w0, 3
	strb	w2, [x1, 3234]
	mov	w2, 262144
	str	w4, [x1, 3240]
	str	w3, [x1, 3248]
	stp	w23, wzr, [x5, 188]
	stp	wzr, w2, [x5, 196]
	str	w0, [x1, 3256]
	mov	x0, x19
	bl	ClearDPB
	str	xzr, [x22, 752]
.L4104:
	mov	w0, w20
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4101:
	mov	x0, x19
	str	w23, [x22, 804]
	bl	ClearAllSlice
	mov	x1, x24
	mov	x0, x19
	bl	StorePicInDPB
	cbnz	w0, .L4119
.L4107:
	mov	x0, x19
	bl	FlushDPB
	cbnz	w0, .L4120
.L4108:
	add	x0, x19, 20480
	str	wzr, [x22, 2780]
	mov	w4, 32
	mov	w7, 23352
	ldr	w8, [x19, 14948]
	movk	w7, 0x1, lsl 16
	strb	w4, [x0, 3234]
	add	x4, x0, 3072
	strb	wzr, [x0, 3232]
	mov	w6, 2
	str	w8, [x0, 3240]
	mov	w5, 7
	strb	w6, [x0, 3233]
	mov	w3, 3
	str	w5, [x0, 3244]
	mov	w2, -1
	str	w7, [x0, 3248]
	mov	w1, 262144
	stp	w3, w2, [x4, 184]
	stp	wzr, wzr, [x4, 192]
	str	w1, [x0, 3272]
	mov	w20, 0
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4102:
	mov	x0, x19
	bl	ClearAllSlice
	mov	x1, x24
	mov	x0, x19
	bl	StorePicInDPB
	cmp	w0, 1
	mov	w21, w0
	bhi	.L4121
.L4106:
	mov	w0, 1
	str	w0, [x22, 2780]
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4120:
	ldr	x1, .LC970
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	b	.L4108
	.p2align 3
.L4119:
	ldr	x1, .LC968
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	b	.L4107
	.p2align 3
.L4118:
	mov	x0, x19
	bl	ClearAllSlice
	b	.L4104
	.size	H264DEC_VDMPostProc, .-H264DEC_VDMPostProc
	.align	3
.LC964:
	.xword	g_VdmDrvParam
	.align	3
.LC966:
	.xword	.LC965
	.align	3
.LC968:
	.xword	.LC967
	.align	3
.LC970:
	.xword	.LC969
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_GetImageBuffer
	.type	H264DEC_GetImageBuffer, %function
H264DEC_GetImageBuffer:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x19, x21, 61440
	bl	_mcount
	ldr	w0, [x19, 2772]
	cbz	w0, .L4139
	sub	w0, w0, #1
	mov	w4, 808
	mov	x2, 30040
	add	x1, x21, 28672
	add	x1, x1, 560
	umaddl	x4, w0, w4, x2
	add	x4, x21, x4
	.p2align 2
.L4130:
	ldrb	w0, [x1, 1]
	cbnz	w0, .L4125
	cbz	x1, .L4126
	add	x23, x21, 24576
	ldr	w3, [x23, 3716]
	cbz	w3, .L4126
	ldr	x0, [x21, 27904]
	cmp	x0, x1
	beq	.L4125
	sub	w3, w3, #1
	mov	x0, x21
	add	x3, x21, x3, lsl 3
	b	.L4127
	.p2align 3
.L4128:
	ldr	x2, [x0, 27904]
	cmp	x2, x1
	beq	.L4125
.L4127:
	cmp	x3, x0
	add	x0, x0, 8
	bne	.L4128
.L4126:
	add	x19, x21, 65536
	ldr	w0, [x19, 116]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L4139
	cmn	w0, #1
	beq	.L4160
.L4158:
	mov	w0, 0
.L4162:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4125:
	add	x1, x1, 808
	cmp	x1, x4
	bne	.L4130
	add	x2, x29, 76
	add	x1, x29, 72
	add	x0, x21, 8
	bl	GetQueueImgNum
	ldr	x1, .LC972
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, .LC974
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 2772]
	cbz	w0, .L4161
	ldr	x20, .LC978
	mov	w22, 0
	add	x23, x21, 24576
	.p2align 2
.L4137:
	uxtw	x1, w22
	lsl	x0, x1, 2
	sub	x0, x0, x1
	add	x0, x0, x0, lsl 4
	lsl	x0, x0, 1
	sub	x1, x0, x1
	lsl	x1, x1, 3
	add	x0, x21, x1
	add	x1, x1, 28672
	add	x0, x0, 28672
	add	x1, x1, 560
	adds	x1, x21, x1
	ldrb	w3, [x0, 561]
	ldrb	w4, [x0, 564]
	beq	.L4142
	ldr	w5, [x23, 3716]
	cbz	w5, .L4142
	ldr	x0, [x21, 27904]
	cmp	x1, x0
	beq	.L4144
	sub	w5, w5, #1
	mov	x0, x21
	add	x5, x21, x5, lsl 3
	b	.L4135
	.p2align 3
.L4136:
	ldr	x2, [x0, 27904]
	cmp	x1, x2
	beq	.L4144
.L4135:
	cmp	x5, x0
	add	x0, x0, 8
	bne	.L4136
.L4142:
	mov	w5, 1
.L4134:
	mov	w2, w22
	mov	x1, x20
	mov	w0, 0
	add	w22, w22, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 2772]
	cmp	w0, w22
	bhi	.L4137
.L4138:
	ldp	w2, w3, [x29, 72]
	ldr	x1, .LC976
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 2772]
	ldr	w2, [x23, 3716]
	ldp	w3, w1, [x29, 72]
	sub	w0, w0, #2
	sub	w0, w0, w2
	add	w1, w3, w1
	cmp	w1, w0
	bge	.L4158
	ldr	x1, .LC980
	mov	w0, 0
	bl	dprint_vfmw
	mov	w1, 1
	mov	x0, x21
	bl	ClearAll
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4139:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	mov	w0, 1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4144:
	mov	w5, 0
	b	.L4134
.L4160:
	ldr	w0, [x19, 116]
	add	x1, x21, 8
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	b	.L4162
.L4161:
	add	x23, x21, 24576
	b	.L4138
	.size	H264DEC_GetImageBuffer, .-H264DEC_GetImageBuffer
	.align	3
.LC972:
	.xword	.LC971
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
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_GetImageWidth
	.type	H264DEC_GetImageWidth, %function
H264DEC_GetImageWidth:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x19, x19, 16384
	bl	_mcount
	ldr	w0, [x19, 1840]
	ldr	x19, [sp, 16]
	add	w0, w0, 1
	lsl	w0, w0, 4
	ldp	x29, x30, [sp], 32
	ret
	.size	H264DEC_GetImageWidth, .-H264DEC_GetImageWidth
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_GetImageHeight
	.type	H264DEC_GetImageHeight, %function
H264DEC_GetImageHeight:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	bl	_mcount
	add	x1, x19, 16384
	add	x19, x19, 12288
	mov	w0, 2
	ldr	w1, [x1, 1844]
	ldrb	w2, [x19, 2706]
	add	w1, w1, 1
	ldr	x19, [sp, 16]
	sub	w0, w0, w2
	lsl	w1, w1, 4
	ldp	x29, x30, [sp], 32
	mul	w0, w1, w0
	ret
	.size	H264DEC_GetImageHeight, .-H264DEC_GetImageHeight
	.global	TmpBuf
	.global	g_CabacMN
	.global	quant8_org
	.global	quant_org
	.global	quant8_inter_default
	.global	quant8_intra_default
	.global	quant_inter_default
	.global	quant_intra_default
	.global	g_ZZ_SCAN8
	.global	g_ZZ_SCAN
	.global	CalcZeroNum
	.global	g_AspecRatioIdc
	.global	g_NalTypeEOPIC
	.section	.rodata
	.align	3
.LANCHOR1 = . + 0
	.type	g_ZZ_SCAN, %object
	.size	g_ZZ_SCAN, 16
g_ZZ_SCAN:
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
	.type	g_ZZ_SCAN8, %object
	.size	g_ZZ_SCAN8, 64
g_ZZ_SCAN8:
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
	.type	SarTable.12849, %object
	.size	SarTable.12849, 136
SarTable.12849:
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
	.type	__func__.13030, %object
	.size	__func__.13030, 12
__func__.13030:
	.string	"H264_DecSEI"
	.zero	4
	.type	__func__.11883, %object
	.size	__func__.11883, 13
__func__.11883:
	.string	"DirectOutput"
	.zero	3
	.type	__func__.12348, %object
	.size	__func__.12348, 8
__func__.12348:
	.string	"InitDPB"
	.type	__func__.12001, %object
	.size	__func__.12001, 14
__func__.12001:
	.string	"StorePicInDPB"
	.zero	2
	.type	__FUNCTION__.13267, %object
	.size	__FUNCTION__.13267, 13
__FUNCTION__.13267:
	.string	"H264DEC_Init"
	.zero	3
	.type	__FUNCTION__.12485, %object
	.size	__FUNCTION__.12485, 8
__FUNCTION__.12485:
	.string	"InitPic"
	.data
	.align	3
.LANCHOR0 = . + 0
	.type	quant_org, %object
	.size	quant_org, 16
quant_org:
	.word	269488144
	.word	269488144
	.word	269488144
	.word	269488144
	.type	quant8_org, %object
	.size	quant8_org, 64
quant8_org:
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
	.type	quant_intra_default, %object
	.size	quant_intra_default, 16
quant_intra_default:
	.word	471076102
	.word	538711053
	.word	622861332
	.word	707076124
	.type	quant_inter_default, %object
	.size	quant_inter_default, 16
quant_inter_default:
	.word	403967498
	.word	454562830
	.word	505092116
	.word	572398360
	.type	quant8_intra_default, %object
	.size	quant8_intra_default, 64
quant8_intra_default:
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
	.type	quant8_inter_default, %object
	.size	quant8_inter_default, 256
quant8_inter_default:
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
	.type	g_NalTypeEOPIC, %object
	.size	g_NalTypeEOPIC, 12
g_NalTypeEOPIC:
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
	.type	g_CabacMN, %object
	.size	g_CabacMN, 5120
g_CabacMN:
	.word	388044546
	.word	352326208
	.word	153820416
	.word	721207
	.word	4522025
	.word	4128831
	.word	4128768
	.word	0
	.word	351339062
	.word	55239807
	.word	-378996171
	.word	-13236429
	.word	521477919
	.word	422707200
	.word	0
	.word	0
	.word	87683918
	.word	-180289218
	.word	17367089
	.word	-613020375
	.word	54458195
	.word	72745057
	.word	-112721920
	.word	0
	.word	21230414
	.word	-180289218
	.word	440602714
	.word	157864575
	.word	-328728576
	.word	0
	.word	0
	.word	0
	.word	204602441
	.word	288553558
	.word	-278922691
	.word	154007875
	.word	-78971830
	.word	-78579384
	.word	20578304
	.word	0
	.word	-45745583
	.word	-178256329
	.word	121895766
	.word	39321658
	.word	-45287842
	.word	87426117
	.word	-45023144
	.word	0
	.word	370745856
	.word	268442146
	.word	320219136
	.word	721207
	.word	4522025
	.word	4128831
	.word	4128768
	.word	0
	.word	351339062
	.word	55239807
	.word	-378996171
	.word	-13236429
	.word	521477919
	.word	422707200
	.word	0
	.word	0
	.word	37878343
	.word	-212925132
	.word	-32963543
	.word	-478802647
	.word	54458195
	.word	72745057
	.word	-112721920
	.word	0
	.word	-62064057
	.word	-212925132
	.word	956442916
	.word	440783743
	.word	-245039104
	.word	0
	.word	0
	.word	0
	.word	154336582
	.word	171312709
	.word	-212205516
	.word	137101122
	.word	-11730618
	.word	-27854008
	.word	3997696
	.word	0
	.word	-28968110
	.word	-161480133
	.word	38534487
	.word	-43777736
	.word	-45417899
	.word	3931473
	.word	-111740065
	.word	0
	.word	487594240
	.word	234886184
	.word	336207104
	.word	721207
	.word	4522025
	.word	4128831
	.word	4128768
	.word	0
	.word	351339062
	.word	55239807
	.word	-378996171
	.word	-13236429
	.word	521477919
	.word	422707200
	.word	0
	.word	0
	.word	437320789
	.word	-395967175
	.word	-164364994
	.word	-446493399
	.word	54458195
	.word	72745057
	.word	-112721920
	.word	0
	.word	-28640171
	.word	-395967175
	.word	905979178
	.word	207741055
	.word	-361431040
	.word	0
	.word	0
	.word	0
	.word	104460105
	.word	238680669
	.word	-229049812
	.word	70714167
	.word	-61866421
	.word	-194905806
	.word	20709376
	.word	0
	.word	-178654873
	.word	-344714439
	.word	339346516
	.word	106955071
	.word	-78253206
	.word	88016459
	.word	-44368027
	.word	0
	.word	388044546
	.word	352326208
	.word	153820416
	.word	3013710
	.word	-44040151
	.word	4128831
	.word	4128768
	.word	0
	.word	351339062
	.word	55239807
	.word	-378996171
	.word	-13236429
	.word	203950899
	.word	238747648
	.word	0
	.word	0
	.word	87683918
	.word	-180289218
	.word	17367089
	.word	-613020375
	.word	54458195
	.word	72745057
	.word	-112721920
	.word	0
	.word	21230414
	.word	-180289218
	.word	440602714
	.word	157864575
	.word	-328728576
	.word	0
	.word	0
	.word	0
	.word	204602441
	.word	288553558
	.word	-278922691
	.word	154007875
	.word	-78971830
	.word	-78579384
	.word	20578304
	.word	0
	.word	-45745583
	.word	-178256329
	.word	121895766
	.word	39321658
	.word	-45287842
	.word	87426117
	.word	-45023144
	.word	0
	.word	370745856
	.word	268442146
	.word	320219136
	.word	219088691
	.word	38797353
	.word	4128831
	.word	4128768
	.word	0
	.word	351339062
	.word	55239807
	.word	-378996171
	.word	-13236429
	.word	421532977
	.word	355860480
	.word	0
	.word	0
	.word	37878343
	.word	-212925132
	.word	-32963543
	.word	-478802647
	.word	54458195
	.word	72745057
	.word	-112721920
	.word	0
	.word	-62064057
	.word	-212925132
	.word	956442916
	.word	440783743
	.word	-245039104
	.word	0
	.word	0
	.word	0
	.word	154336582
	.word	171312709
	.word	-212205516
	.word	137101122
	.word	-11730618
	.word	-27854008
	.word	3997696
	.word	0
	.word	-28968110
	.word	-161480133
	.word	38534487
	.word	-43777736
	.word	-45417899
	.word	3931473
	.word	-111740065
	.word	0
	.word	487594240
	.word	234886184
	.word	336207104
	.word	119732056
	.word	-327221207
	.word	4128831
	.word	4128768
	.word	0
	.word	351339062
	.word	55239807
	.word	-378996171
	.word	-13236429
	.word	354489138
	.word	289210368
	.word	0
	.word	0
	.word	437320789
	.word	-395967175
	.word	-164364994
	.word	-446493399
	.word	54458195
	.word	72745057
	.word	-112721920
	.word	0
	.word	-28640171
	.word	-395967175
	.word	905979178
	.word	207741055
	.word	-361431040
	.word	0
	.word	0
	.word	0
	.word	104460105
	.word	238680669
	.word	-229049812
	.word	70714167
	.word	-61866421
	.word	-194905806
	.word	20709376
	.word	0
	.word	-178654873
	.word	-344714439
	.word	339346516
	.word	106955071
	.word	-78253206
	.word	88016459
	.word	-44368027
	.word	0
	.word	-276827290
	.word	5437770
	.word	-345250433
	.word	-511711105
	.word	-295705217
	.word	-344792449
	.word	0
	.word	0
	.word	-277089165
	.word	-260377229
	.word	-45614550
	.word	-80544458
	.word	-29491142
	.word	20971080
	.word	-11864229
	.word	0
	.word	-111282857
	.word	-45221049
	.word	-62915516
	.word	-195757762
	.word	-113178563
	.word	87621186
	.word	20971581
	.word	-28442624
	.word	402657033
	.word	135859474
	.word	252251411
	.word	170200082
	.word	102568993
	.word	253625389
	.word	20578366
	.word	121438208
	.word	-94504364
	.word	-129040318
	.word	-12124098
	.word	-29557189
	.word	-78906050
	.word	-63244478
	.word	-11599801
	.word	54788096
	.word	252053011
	.word	118492174
	.word	302845195
	.word	219090704
	.word	202837271
	.word	252972570
	.word	237769000
	.word	288292864
	.word	-197132732
	.word	-246090904
	.word	-79430885
	.word	-47710676
	.word	3076160
	.word	-129698226
	.word	-95553962
	.word	0
	.word	20055860
	.word	170065964
	.word	187040045
	.word	3015980
	.word	521208115
	.word	120724499
	.word	270601790
	.word	0
	.word	203819821
	.word	254217002
	.word	220991533
	.word	204016177
	.word	505549354
	.word	171381043
	.word	288227417
	.word	0
	.word	170719550
	.word	254021160
	.word	270208029
	.word	19665956
	.word	304088362
	.word	19925566
	.word	288229696
	.word	0
	.word	403772693
	.word	420880155
	.word	371004195
	.word	238160441
	.word	121634381
	.word	-61670050
	.word	155579501
	.word	0
	.word	-45680547
	.word	-161815937
	.word	-195235529
	.word	-163775938
	.word	-62786487
	.word	-129173168
	.word	-145166482
	.word	0
	.word	-210964124
	.word	-211422373
	.word	-195103144
	.word	-262867370
	.word	-111938729
	.word	-312606394
	.word	4782922
	.word	305856512
	.word	451745519
	.word	451878631
	.word	485237225
	.word	635773417
	.word	686040815
	.word	569714929
	.word	704259585
	.word	688979968
	.word	-194448031
	.word	-262080168
	.word	-128583339
	.word	-145362088
	.word	71499085
	.word	-45286836
	.word	171638604
	.word	-11337728
	.word	618472670
	.word	551953890
	.word	752886510
	.word	586230001
	.word	569975803
	.word	553657858
	.word	554506019
	.word	221904896
	.word	-11863455
	.word	-111416193
	.word	-178131884
	.word	-179307959
	.word	-62196906
	.word	-211749535
	.word	-311099392
	.word	0
	.word	-127471260
	.word	6225920
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	519707395
	.word	437649408
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-110890401
	.word	39780352
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	503126528
	.word	371064832
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-63374510
	.word	-112399747
	.word	-129041631
	.word	-63898059
	.word	-46206137
	.word	-162532266
	.word	-212143519
	.word	0
	.word	-62193080
	.word	-179569337
	.word	254735173
	.word	4063297
	.word	354808136
	.word	154734646
	.word	4066376
	.word	0
	.word	636494844
	.word	653797117
	.word	620963333
	.word	704652048
	.word	655756848
	.word	455415100
	.word	205783649
	.word	0
	.word	5045066
	.word	4650315
	.word	21233729
	.word	-230096066
	.word	71237444
	.word	-213185737
	.word	88083526
	.word	0
	.word	536421361
	.word	587014659
	.word	620700165
	.word	537600773
	.word	571221771
	.word	505225754
	.word	489100098
	.word	0
	.word	0
	.word	0
	.word	-45351145
	.word	19005739
	.word	3604023
	.word	3998016
	.word	4519772
	.word	0
	.word	-277287824
	.word	-294455979
	.word	-245566887
	.word	-431492783
	.word	-229638076
	.word	-230234056
	.word	-381360078
	.word	-179699712
	.word	401808115
	.word	686895602
	.word	738405638
	.word	740434230
	.word	324141056
	.word	0
	.word	0
	.word	0
	.word	-227871903
	.word	-245697446
	.word	-296159671
	.word	-145755562
	.word	-162924986
	.word	-163185854
	.word	-146736326
	.word	37421056
	.word	368449781
	.word	486022399
	.word	486743305
	.word	588520740
	.word	239271936
	.word	0
	.word	0
	.word	0
	.word	-444668830
	.word	-412751549
	.word	-464327586
	.word	-262935954
	.word	-346296730
	.word	-211950721
	.word	0
	.word	0
	.word	-111346855
	.word	-111086740
	.word	-95617492
	.word	2949172
	.word	-46072261
	.word	-62456757
	.word	-128782490
	.word	0
	.word	-27919794
	.word	-11798195
	.word	37094706
	.word	-45874894
	.word	103480401
	.word	20970566
	.word	4391481
	.word	-28573696
	.word	186384936
	.word	53215281
	.word	3015212
	.word	36896815
	.word	69665342
	.word	103678006
	.word	53871162
	.word	71237632
	.word	-211095446
	.word	-162010254
	.word	-294718878
	.word	-361829526
	.word	-295179413
	.word	-378739088
	.word	-161418145
	.word	-77922304
	.word	235604750
	.word	151720459
	.word	352917502
	.word	552673521
	.word	585836521
	.word	719268321
	.word	786704116
	.word	354222080
	.word	-47251647
	.word	-12978339
	.word	-145947880
	.word	2752560
	.word	3668539
	.word	-112724909
	.word	-178789769
	.word	0
	.word	186844224
	.word	20777763
	.word	303631384
	.word	220007716
	.word	-161613495
	.word	-28766930
	.word	154270052
	.word	0
	.word	104007225
	.word	120915508
	.word	104270637
	.word	237242421
	.word	-11401417
	.word	-45215954
	.word	371195732
	.word	0
	.word	-144840098
	.word	-78247613
	.word	-61802923
	.word	-12187844
	.word	154797373
	.word	204607282
	.word	305205558
	.word	0
	.word	770192851
	.word	820396501
	.word	736962550
	.word	503910938
	.word	337313849
	.word	-229442741
	.word	-312353411
	.word	0
	.word	-45418660
	.word	-128456322
	.word	20643101
	.word	-14417626
	.word	36436535
	.word	3801152
	.word	-45418918
	.word	0
	.word	154468917
	.word	87424573
	.word	3670072
	.word	-213910724
	.word	-12712903
	.word	-96140231
	.word	237438003
	.word	222560256
	.word	419897081
	.word	469965828
	.word	536879871
	.word	503717382
	.word	537468691
	.word	437983774
	.word	622074914
	.word	289800192
	.word	170460974
	.word	-13433039
	.word	137627945
	.word	103744055
	.word	220793388
	.word	103941429
	.word	221316159
	.word	104857600
	.word	452992000
	.word	536615686
	.word	570957322
	.word	404103443
	.word	371202591
	.word	355015212
	.word	388960321
	.word	239534080
	.word	87426620
	.word	104531525
	.word	4652061
	.word	85919530
	.word	20708922
	.word	-45548207
	.word	-178192384
	.word	0
	.word	54526269
	.word	155123712
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	21169467
	.word	155385856
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-28967365
	.word	105250816
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	138151487
	.word	289472512
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-13631420
	.word	-62523304
	.word	3803141
	.word	168693266
	.word	219873832
	.word	3865926
	.word	-95422379
	.word	0
	.word	120721447
	.word	86770740
	.word	187759420
	.word	-12910533
	.word	371262764
	.word	237764430
	.word	3934533
	.word	0
	.word	270406176
	.word	304289309
	.word	404690726
	.word	304813097
	.word	188680507
	.word	155254622
	.word	-27658388
	.word	0
	.word	170658079
	.word	204145461
	.word	237111846
	.word	-46920408
	.word	287311660
	.word	119934258
	.word	171514411
	.word	0
	.word	353900308
	.word	437721888
	.word	471276568
	.word	388503584
	.word	471668522
	.word	322508341
	.word	373099350
	.word	0
	.word	0
	.word	0
	.word	-96274141
	.word	-114624467
	.word	-80677832
	.word	-96666818
	.word	-129828788
	.word	0
	.word	-61867705
	.word	-79300794
	.word	-129829308
	.word	-313920443
	.word	-263786173
	.word	-331420346
	.word	-264050111
	.word	-331415552
	.word	167647991
	.word	569845753
	.word	704523523
	.word	822684955
	.word	607846400
	.word	0
	.word	0
	.word	0
	.word	-78251439
	.word	-162662063
	.word	-279908791
	.word	-62196141
	.word	-146278589
	.word	-12715966
	.word	-230555589
	.word	37421056
	.word	368255474
	.word	670641918
	.word	855784454
	.word	1024538402
	.word	708706304
	.word	0
	.word	0
	.word	0
	.word	-645927333
	.word	-278862255
	.word	-580720538
	.word	-379460233
	.word	-396104850
	.word	-295248769
	.word	0
	.word	0
	.word	5307225
	.word	-111215524
	.word	-378474169
	.word	-113442763
	.word	-79497907
	.word	-145688748
	.word	-162013569
	.word	0
	.word	-211291301
	.word	-145100196
	.word	-129174441
	.word	-378607511
	.word	-162599824
	.word	-278678913
	.word	-1166029185
	.word	-763428864
	.word	70060572
	.word	169812469
	.word	886379023
	.word	469771242
	.word	652681728
	.word	669917428
	.word	1721892864
	.word	955318272
	.word	-344004228
	.word	-328276354
	.word	-411242647
	.word	-444996235
	.word	-278469003
	.word	-445325446
	.word	-161484188
	.word	-127991808
	.word	335155962
	.word	234887924
	.word	535831015
	.word	568993252
	.word	669133538
	.word	802565596
	.word	836643311
	.word	537460736
	.word	2555969
	.word	-246096513
	.word	-345637593
	.word	-80348867
	.word	-179572403
	.word	-279189653
	.word	-412097414
	.word	0
	.word	-62719154
	.word	-62392248
	.word	37486391
	.word	-112788917
	.word	-128328073
	.word	-45408236
	.word	504812671
	.word	0
	.word	570170634
	.word	637219811
	.word	670512350
	.word	1170621377
	.word	754655239
	.word	937631745
	.word	6948
	.word	0
	.word	-277879694
	.word	-94765488
	.word	-61671595
	.word	-128843960
	.word	88080707
	.word	154665029
	.word	21301061
	.word	0
	.word	1169768385
	.word	1119899062
	.word	920204509
	.word	703996928
	.word	687808014
	.word	471471909
	.word	204933953
	.word	0
	.word	-28707736
	.word	-144973441
	.word	-179505620
	.word	-164301255
	.word	-147132344
	.word	-112853947
	.word	-78973098
	.word	0
	.word	3603261
	.word	3866428
	.word	-46270397
	.word	-413863350
	.word	-79624908
	.word	37290045
	.word	-146410170
	.word	305594368
	.word	568795874
	.word	618931940
	.word	652550894
	.word	602940146
	.word	553133562
	.word	587210250
	.word	470948377
	.word	489226240
	.word	-112854461
	.word	-263324096
	.word	37616195
	.word	-46136775
	.word	-46006974
	.word	4065587
	.word	-12386745
	.word	-28639232
	.word	635381724
	.word	685450978
	.word	786377442
	.word	686305763
	.word	653535478
	.word	654126843
	.word	521411870
	.word	422313984
	.word	53937976
	.word	121047101
	.word	37943074
	.word	18877215
	.word	87359031
	.word	-29163447
	.word	-128385024
	.word	0
	.word	-62455750
	.word	121438208
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	4915784
	.word	139264000
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-12126392
	.word	238813184
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	204475185
	.word	439156736
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-46858172
	.word	-112527528
	.word	53741316
	.word	168300808
	.word	269681445
	.word	-12715191
	.word	-12125106
	.word	0
	.word	153686553
	.word	153093419
	.word	154075180
	.word	3342894
	.word	321322050
	.word	254151722
	.word	153223257
	.word	0
	.word	237179423
	.word	287511838
	.word	288166954
	.word	304945946
	.word	271976258
	.word	272108361
	.word	172226164
	.word	0
	.word	270860335
	.word	304286501
	.word	204016169
	.word	36703273
	.word	220790843
	.word	53613352
	.word	54661682
	.word	0
	.word	370546454
	.word	454369556
	.word	438050328
	.word	455217706
	.word	421990962
	.word	205919542
	.word	239536979
	.word	0
	.word	0
	.word	0
	.word	-79233000
	.word	-14352854
	.word	-30083271
	.word	-96469951
	.word	-62654126
	.word	0
	.word	-78251439
	.word	-162662063
	.word	-279908791
	.word	-62196141
	.word	-146278589
	.word	-12715966
	.word	-230555589
	.word	37421056
	.word	301342963
	.word	720843259
	.word	889208835
	.word	1141522971
	.word	792264704
	.word	0
	.word	0
	.word	0
	.word	-44958388
	.word	-112657842
	.word	-196545980
	.word	-45680052
	.word	-79496386
	.word	3800125
	.word	-147062474
	.word	37355520
	.word	301342963
	.word	720843259
	.word	889208835
	.word	1141522971
	.word	792264704
	.word	0
	.word	0
	.word	0
	.word	-595595429
	.word	-228595884
	.word	-413731751
	.word	-279191169
	.word	-229839257
	.word	-212149377
	.word	0
	.word	0
	.word	189793612
	.word	39060814
	.word	-395056558
	.word	-146931659
	.word	3928661
	.word	-212208802
	.word	-178461825
	.word	0
	.word	-61410216
	.word	-78447288
	.word	-62654392
	.word	-262539451
	.word	-12909246
	.word	70908999
	.word	-28900806
	.word	-11927552
	.word	69664810
	.word	119671581
	.word	136250917
	.word	120193832
	.word	136383787
	.word	220464175
	.word	53936698
	.word	104595456
	.word	-360716417
	.word	-411507329
	.word	-311236235
	.word	-411638155
	.word	-395189130
	.word	-512173188
	.word	-161549978
	.word	-161284096
	.word	301142263
	.word	301210603
	.word	635578840
	.word	718745809
	.word	669003480
	.word	885862103
	.word	886647789
	.word	537591808
	.word	-97057731
	.word	-229385345
	.word	-345705927
	.word	-196872889
	.word	-162665643
	.word	-262608536
	.word	-245179521
	.word	0
	.word	-64159931
	.word	4127027
	.word	-63964630
	.word	-47580619
	.word	139261774
	.word	-179107532
	.word	4455258
	.word	0
	.word	137104172
	.word	237635376
	.word	70779956
	.word	220530993
	.word	322570800
	.word	204275781
	.word	337709119
	.word	0
	.word	-211104129
	.word	-77852359
	.word	-78187682
	.word	-195297715
	.word	4718409
	.word	71366993
	.word	88084281
	.word	0
	.word	1036597458
	.word	1053708733
	.word	770450430
	.word	470753793
	.word	654384657
	.word	338039341
	.word	255197263
	.word	0
	.word	-78645912
	.word	-178527617
	.word	-162334672
	.word	-130156478
	.word	-112790965
	.word	-162531501
	.word	-195236244
	.word	0
	.word	21229896
	.word	-78907312
	.word	-346821824
	.word	-216012992
	.word	-480376501
	.word	289405002
	.word	-81527269
	.word	224067584
	.word	602808807
	.word	486348810
	.word	452993778
	.word	886319080
	.word	319889177
	.word	605886497
	.word	571416084
	.word	373882880
	.word	21168196
	.word	-163380924
	.word	5046848
	.word	4520782
	.word	121046331
	.word	37817910
	.word	254543164
	.word	38141952
	.word	619718130
	.word	636428289
	.word	571415823
	.word	404300310
	.word	521151250
	.word	521937193
	.word	605821743
	.word	356384768
	.word	4324943
	.word	4783196
	.word	-61871547
	.word	-263455174
	.word	20640590
	.word	-145490863
	.word	-211615744
	.word	0
	.word	54655301
	.word	139264000
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	337777439
	.word	455868416
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-28512682
	.word	205914112
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	304026394
	.word	605552640
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-96992444
	.word	-129502366
	.word	-212731354
	.word	-213976518
	.word	-29626295
	.word	-162729130
	.word	-145492393
	.word	0
	.word	-163445954
	.word	-45814703
	.word	1966343
	.word	-48764086
	.word	272820604
	.word	287649006
	.word	853469823
	.word	0
	.word	319819556
	.word	254022940
	.word	420814356
	.word	520887056
	.word	405405789
	.word	238554937
	.word	438757503
	.word	0
	.word	88142918
	.word	188155192
	.word	4522561
	.word	-95812298
	.word	121043532
	.word	-179110323
	.word	-28501718
	.word	0
	.word	404167440
	.word	404627229
	.word	371791402
	.word	272371508
	.word	238814030
	.word	-260369099
	.word	372775229
	.word	0
	.word	0
	.word	0
	.word	-146278107
	.word	-131271375
	.word	-164039621
	.word	-130025661
	.word	-96143793
	.word	0
	.word	-45156278
	.word	-146213304
	.word	-297012153
	.word	-180356282
	.word	-280235448
	.word	-263981003
	.word	-230951116
	.word	-180092928
	.word	167649014
	.word	536617471
	.word	554114828
	.word	622272294
	.word	339738624
	.word	0
	.word	0
	.word	0
	.word	-45156278
	.word	-146213304
	.word	-297012153
	.word	-180356282
	.word	-280235448
	.word	-263981003
	.word	-230951116
	.word	-180092928
	.word	167649014
	.word	536617471
	.word	554114828
	.word	622272294
	.word	339738624
	.word	0
	.word	0
	.word	0
	.type	CalcZeroNum, %object
	.size	CalcZeroNum, 256
CalcZeroNum:
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
	.type	g_AspecRatioIdc, %object
	.size	g_AspecRatioIdc, 112
g_AspecRatioIdc:
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
.LC2:
	ASCII(.string	"list[%d][%d]: frame_num=%d, poc=%d\n" )
	.zero	4
.LC4:
	ASCII(.string	"L0: " )
	.zero	3
.LC6:
	ASCII(.string	"\n" )
	.zero	6
.LC8:
	ASCII(.string	"%d " )
	.zero	4
.LC10:
	ASCII(.string	"%dt " )
	.zero	3
.LC12:
	ASCII(.string	"%db " )
	.zero	3
.LC14:
	ASCII(.string	"L1:" )
	.zero	4
.LC16:
	ASCII(.string	"\n\nEND of the bit buffer, copy the first packet!\n\n" )
	.zero	6
.LC18:
	ASCII(.string	"0:phy:0x%x, 1:phy:0x%x; len0:%d len1:%d\n" )
	.zero	7
.LC21:
	ASCII(.string	"0:phy:0x%x, len0:%d\n" )
	.zero	3
.LC23:
	ASCII(.string	"%-50s%50d\n" )
	.zero	5
.LC30:
	ASCII(.string	"frame %d, state=%d, is already output\n" )
	.zero	1
.LC32:
	ASCII(.string	"plfs(fs=%p, eFoState=%d) is null, logic_fs_id = %d\n" )
	.zero	4
.LC34:
	ASCII(.string	"not used frame: (%d,%d)\n" )
	.zero	7
.LC36:
	ASCII(.string	"frame skip: is_used=%d,skip=%d\n" )
.LC38:
	ASCII(.string	"topfield skip: is_used=%d,skip=%d\n" )
	.zero	5
.LC40:
	ASCII(.string	"bottom field skip: is_used=%d,skip=%d\n" )
	.zero	1
.LC42:
	ASCII(.string	"err(%d) > out_thr(%d)\n" )
	.zero	1
.LC44:
	ASCII(.string	"exg_pts: %lld <-> %lld\n" )
.LC47:
	ASCII(.string	"stream frame packing type is %d! what can we do?\n" )
	.zero	6
.LC50:
	ASCII(.string	"firmware/common/syntax/h264.c" )
	.zero	2
.LC52:
	ASCII(.string	"NULL pointer: %s, L%d\n" )
	.zero	1
.LC54:
	ASCII(.string	"framestore %d allocate apc %d\n" )
	.zero	1
.LC56:
	ASCII(.string	"find APC, but logic_fs_id %d abnormal(ref=%d,plfs=%p, pdfs=%p)\n" )
.LC60:
	ASCII(.string	"pH264Ctx->DPB.fs[%d]: frame_num=%d poc=%d is_reference=%d\n" )
	.zero	5
.LC62:
	ASCII(.string	"pH264Ctx->DPB.fs_ref[%d]: frame_num=%d poc=%d is_reference=%d\n" )
	.zero	1
.LC64:
	ASCII(.string	"pH264Ctx->DPB.fs_ref[%d]: frame.poc=%d \n" )
	.zero	7
.LC68:
	ASCII(.string	"\n VFMW ** release streambuff=%p   bitstream_length=%d\n" )
	.zero	1
.LC70:
	ASCII(.string	"clear curr slice.\n" )
	.zero	5
.LC75:
	ASCII(.string	"partition fs memory fail!\n" )
	.zero	5
.LC77:
	ASCII(.string	"ERROR: FSP_ConfigInstance fail!\n" )
	.zero	7
.LC79:
	ASCII(.string	"slice_type %d invalid, clear current slice.\n" )
	.zero	3
.LC81:
	ASCII(.string	"pRepairPic(%p)/found(%d) = 0, clear current slice.\n" )
	.zero	4
.LC83:
	ASCII(.string	"listXsize[%d] = %d, clear current slice.\n" )
	.zero	6
.LC91:
	ASCII(.string	"for P slice size of list equal 0.\n" )
	.zero	5
.LC97:
	ASCII(.string	"for B slice size of two list all equal 0.\n" )
	.zero	5
.LC100:
	ASCII(.string	"init list error.\n" )
	.zero	6
.LC102:
	ASCII(.string	"L0: list after init.\n" )
	.zero	2
.LC105:
	ASCII(.string	"L0: list after reorder.\n" )
	.zero	7
.LC108:
	ASCII(.string	"save pic yuv :  structure = %d;  idc = %d\n" )
	.zero	5
.LC110:
	ASCII(.string	"structure = %d pH264Ctx->CurrPic.pic_width_in_mb = %d pH264Ctx->CurrPic.pic_height_in_mb = %d\n" )
	.zero	1
.LC112:
	ASCII(.string	"nal_ref_idc=%d, structure=%d, image_id=%d, pmv_idc=%d\n" )
	.zero	1
.LC114:
	ASCII(.string	"logic frame id(=%d): get LogicFs error!\n" )
	.zero	7
.LC116:
	ASCII(.string	"phy fs is null: pstDecodeFs = %p, pstDispOutFs = %p\n" )
	.zero	3
.LC120:
	ASCII(.string	"line: %d pstLogicFs is null\n" )
	.zero	3
.LC122:
	ASCII(.string	"SH: first_mb_in_slice" )
	.zero	2
.LC124:
	ASCII(.string	"slicecheck first_mb_in_slice >= MAX_MB_NUM_IN_PIC error.\n" )
	.zero	6
.LC126:
	ASCII(.string	"SH: slice_type" )
	.zero	1
.LC128:
	ASCII(.string	"slice type = %d\n" )
	.zero	7
.LC131:
	ASCII(.string	"slice type = %d, err\n" )
	.zero	2
.LC133:
	ASCII(.string	"slice  pps id = %d\n" )
	.zero	4
.LC135:
	ASCII(.string	"pps with this pic_parameter_set_id = %d havn't decode\n" )
	.zero	1
.LC137:
	ASCII(.string	"sps with this pic_parameter_set_id = %d havn't decode\n" )
	.zero	1
.LC139:
	ASCII(.string	"tmp slice  pps id = %d\n" )
.LC141:
	ASCII(.string	"PPSSPSCheckTmpId: pps with this pic_parameter_set_id = %d havn't decode\n" )
	.zero	7
.LC143:
	ASCII(.string	"PPSSPSCheckTmpId: seq_parameter_set_id out of range.\n" )
	.zero	2
.LC145:
	ASCII(.string	"PPSSPSCheckTmpId: sps with this pic_parameter_set_id = %d havn't decode\n" )
	.zero	7
.LC147:
	ASCII(.string	"new pic flag = %d\n" )
	.zero	5
.LC149:
	ASCII(.string	"SH: ref_pic_list_reordering_flag_l0" )
	.zero	4
.LC151:
	ASCII(.string	"SH: reordering_of_pic_nums_idc_l0" )
	.zero	6
.LC153:
	ASCII(.string	"reorder idc l0 = %d, g_SeErrFlag=%d\n" )
	.zero	3
.LC155:
	ASCII(.string	"SH: abs_diff_pic_num_minus1_l0" )
	.zero	1
.LC157:
	ASCII(.string	"abs_diff_pic_num_minus1_l0 = %d, g_SeErrFlag=%d\n" )
	.zero	7
.LC159:
	ASCII(.string	"SH: long_term_pic_idx_l0" )
	.zero	7
.LC161:
	ASCII(.string	"num of idc l0 exceed\n" )
	.zero	2
.LC163:
	ASCII(.string	"SH: ref_pic_list_reordering_flag_l1" )
	.zero	4
.LC165:
	ASCII(.string	"SH: reordering_of_pic_nums_idc_l1" )
	.zero	6
.LC167:
	ASCII(.string	"reorder idc l1 = %d, g_SeErrFlag=%d\n" )
	.zero	3
.LC169:
	ASCII(.string	"SH: abs_diff_pic_num_minus1_l1" )
	.zero	1
.LC171:
	ASCII(.string	"abs_diff_pic_num_minus1_l1 = %d, g_SeErrFlag=%d\n" )
	.zero	7
.LC173:
	ASCII(.string	"SH: long_term_pic_idx_l1" )
	.zero	7
.LC175:
	ASCII(.string	"num of idc l1 exceed\n" )
	.zero	2
.LC177:
	ASCII(.string	"SH: luma_log2_weight_denom" )
	.zero	5
.LC179:
	ASCII(.string	"SH: chroma_log2_weight_denom" )
	.zero	3
.LC181:
	ASCII(.string	"WP log2 exceed  and err flag = %d\n" )
	.zero	5
.LC183:
	ASCII(.string	"SH: luma_weight_flag_l0" )
.LC185:
	ASCII(.string	"SH: luma_weight_l0" )
	.zero	5
.LC187:
	ASCII(.string	"SH: luma_offset_l0" )
	.zero	5
.LC189:
	ASCII(.string	"SH: chroma_weight_flag_l0" )
	.zero	6
.LC191:
	ASCII(.string	"SH: chroma_weight_l0" )
	.zero	3
.LC193:
	ASCII(.string	"SH: chroma_offset_l0" )
	.zero	3
.LC195:
	ASCII(.string	"SH: luma_weight_flag_l1" )
.LC197:
	ASCII(.string	"SH: luma_weight_l1" )
	.zero	5
.LC199:
	ASCII(.string	"SH: luma_offset_l1" )
	.zero	5
.LC201:
	ASCII(.string	"SH: chroma_weight_flag_l1" )
	.zero	6
.LC203:
	ASCII(.string	"SH: chroma_weight_l1" )
	.zero	3
.LC205:
	ASCII(.string	"SH: chroma_offset_l1" )
	.zero	3
.LC207:
	ASCII(.string	"overflow MAX_MMCO_LEN\n" )
	.zero	1
.LC209:
	ASCII(.string	"SH: memory_management_control_operation" )
.LC211:
	ASCII(.string	"SH: difference_of_pic_nums_minus1" )
	.zero	6
.LC213:
	ASCII(.string	"SH: long_term_pic_num" )
	.zero	2
.LC215:
	ASCII(.string	"SH: long_term_frame_idx" )
.LC217:
	ASCII(.string	"SH: max_long_term_frame_idx_plus1" )
	.zero	6
.LC219:
	ASCII(.string	"mmco exceed 6\n" )
	.zero	1
.LC221:
	ASCII(.string	"SH: no_output_of_prior_pics_flag" )
	.zero	7
.LC223:
	ASCII(.string	"SH: long_term_reference_flag" )
	.zero	3
.LC225:
	ASCII(.string	"IDR and no_output_of_prior_pics_flag = %d\n" )
	.zero	5
.LC227:
	ASCII(.string	"SH: adaptive_ref_pic_marking_mode_flag" )
	.zero	1
.LC229:
	ASCII(.string	"adaptive_ref_pic_marking_mode_flag = %d\n" )
	.zero	7
.LC231:
	ASCII(.string	"MMCO para would save dec err\n" )
	.zero	2
.LC233:
	ASCII(.string	"mulitislice diff no_out_of_prior_pics_flag, used first.\n" )
	.zero	7
.LC235:
	ASCII(.string	"mulitislice diff long_term_reference_flag, used first.\n" )
.LC237:
	ASCII(.string	"mulitislice diff adaptive_ref_pic_marking_mode_flag, used first.\n" )
	.zero	6
.LC239:
	ASCII(.string	"TotalPicNum=%d, SliceNumInPic=%d, TotalNal=%d\n" )
	.zero	1
.LC241:
	ASCII(.string	"SH: pic_parameter_set_id" )
	.zero	7
.LC243:
	ASCII(.string	"pps id:%d in sliceheader err\n" )
	.zero	2
.LC245:
	ASCII(.string	"PPS or SPS of this slice not valid\n" )
	.zero	4
.LC247:
	ASCII(.string	"recover point but isn't I_SLICE.\n" )
	.zero	6
.LC249:
	ASCII(.string	"SH: frame_num" )
	.zero	2
.LC251:
	ASCII(.string	"IDR NAL but frame_num!=0.\n" )
	.zero	5
.LC253:
	ASCII(.string	"SH: field_pic_flag" )
	.zero	5
.LC255:
	ASCII(.string	"SH: bottom_field_flag" )
	.zero	2
.LC257:
	ASCII(.string	"field first_mb_in_slice bigger than pic size\n" )
	.zero	2
.LC259:
	ASCII(.string	"frame first_mb_in_slice bigger than pic size\n" )
	.zero	2
.LC261:
	ASCII(.string	"SH: idr_pic_id" )
	.zero	1
.LC263:
	ASCII(.string	"idr_pic_id bigger than 65535\n" )
	.zero	2
.LC265:
	ASCII(.string	"SH: pic_order_cnt_lsb" )
	.zero	2
.LC267:
	ASCII(.string	"SH: delta_pic_order_cnt_bottom" )
	.zero	1
.LC269:
	ASCII(.string	"SH: delta_pic_order_cnt[0]" )
	.zero	5
.LC271:
	ASCII(.string	"SH: delta_pic_order_cnt[1]" )
	.zero	5
.LC273:
	ASCII(.string	"SH: redundant_pic_cnt" )
	.zero	2
.LC275:
	ASCII(.string	"redundant pic not support.\n" )
	.zero	4
.LC278:
	ASCII(.string	"first_mb_in_slice in currslice small than prevslice in same pic\n" )
	.zero	7
.LC282:
	ASCII(.string	"SH: direct_spatial_mv_pred_flag" )
.LC284:
	ASCII(.string	"SH: num_ref_idx_override_flag" )
	.zero	2
.LC286:
	ASCII(.string	"SH: num_ref_idx_l0_active_minus1" )
	.zero	7
.LC288:
	ASCII(.string	"SH: num_ref_idx_l1_active_minus1" )
	.zero	7
.LC290:
	ASCII(.string	"frame num_ref_idx_lx_active_minus1 exceed\n" )
	.zero	5
.LC292:
	ASCII(.string	"field num_ref_idx_lx_active_minus1 exceed\n" )
	.zero	5
.LC294:
	ASCII(.string	"reordering para dec err\n" )
	.zero	7
.LC296:
	ASCII(.string	"apply_weights_flag=%d\n" )
	.zero	1
.LC298:
	ASCII(.string	"wpt dec err\n" )
	.zero	3
.LC300:
	ASCII(.string	"mark para dec err\n" )
	.zero	5
.LC302:
	ASCII(.string	"SH: cabac_init_idc" )
	.zero	5
.LC304:
	ASCII(.string	"cabac_init_idc bigger than 3\n" )
	.zero	2
.LC306:
	ASCII(.string	"SH: slice_qp_delta" )
	.zero	5
.LC308:
	ASCII(.string	"SH: disable_deblocking_filter_idc" )
	.zero	6
.LC310:
	ASCII(.string	"disable_deblocking_filter_idc dec err\n" )
	.zero	1
.LC312:
	ASCII(.string	"SH: slice_alpha_c0_offset_div2" )
	.zero	1
.LC314:
	ASCII(.string	"slice_alpha_c0_offset_div2 dec err\n" )
	.zero	4
.LC316:
	ASCII(.string	"SH: slice_beta_offset_div2" )
	.zero	5
.LC318:
	ASCII(.string	"slice_beta_offset_div2 dec err\n" )
.LC320:
	ASCII(.string	"pH264Ctx->pCurrNal->nal_bitoffset = %d; code_len_byte = %d; bitstream_length = %d; pH264Ctx->pCurrNal->nal_trail_zero_bit_num = %d\n" )
	.zero	4
.LC322:
	ASCII(.string	"stream[%d]: bitsoffset = %d; valid_bitlen = %d; bytespos = %x\n" )
	.zero	1
.LC324:
	ASCII(.string	"nal_segment = %d is not expected value\n" )
.LC326:
	ASCII(.string	"stream[%d]: bitsoffset = %d; valid_bitlen = %d\n" )
.LC331:
	ASCII(.string	"   : delta_sl   " )
	.zero	7
.LC335:
	ASCII(.string	"PPS: entropy_coding_mode_flag" )
	.zero	2
.LC337:
	ASCII(.string	"PPS: pic_order_present_flag" )
	.zero	4
.LC339:
	ASCII(.string	"PPS: num_slice_groups_minus1" )
	.zero	3
.LC341:
	ASCII(.string	"Baseline stream with FMO, not support.\n" )
.LC344:
	ASCII(.string	"PPS: num_ref_idx_l0_active_minus1" )
	.zero	6
.LC346:
	ASCII(.string	"num_ref_idx_l0_active_minus1 out of range.\n" )
	.zero	4
.LC348:
	ASCII(.string	"PPS: num_ref_idx_l1_active_minus1" )
	.zero	6
.LC350:
	ASCII(.string	"num_ref_idx_l1_active_minus1 out of range.\n" )
	.zero	4
.LC352:
	ASCII(.string	"PPS: weighted prediction flag" )
	.zero	2
.LC354:
	ASCII(.string	"PPS: weighted_bipred_idc" )
	.zero	7
.LC356:
	ASCII(.string	"weighted_bipred_idc out of range\n" )
	.zero	6
.LC358:
	ASCII(.string	"PPS: pic_init_qp_minus26" )
	.zero	7
.LC360:
	ASCII(.string	"PPS: pic_init_qs_minus26" )
	.zero	7
.LC362:
	ASCII(.string	"PPS: chroma_qp_index_offset" )
	.zero	4
.LC364:
	ASCII(.string	"PPS: deblocking_filter_control_present_flag" )
	.zero	4
.LC366:
	ASCII(.string	"PPS: constrained_intra_pred_flag" )
	.zero	7
.LC368:
	ASCII(.string	"PPS: redundant_pic_cnt_present_flag" )
	.zero	4
.LC370:
	ASCII(.string	"redundant pic not support when find redundant slice later, exit\n" )
	.zero	7
.LC372:
	ASCII(.string	"PPS: transform_8x8_mode_flag" )
	.zero	3
.LC374:
	ASCII(.string	"PPS: pic_scaling_matrix_present_flag" )
	.zero	3
.LC376:
	ASCII(.string	"PPS: second_chroma_qp_index_offset" )
	.zero	5
.LC378:
	ASCII(.string	"SPS: seq_scaling_list_present_flag" )
	.zero	5
.LC381:
	ASCII(.string	"PPS: pic_scaling_list_present_flag" )
	.zero	5
.LC383:
	ASCII(.string	"second_chroma_qp_index_offset out of range.\n" )
	.zero	3
.LC387:
	ASCII(.string	"PPS: pic_parameter_set_id" )
	.zero	6
.LC389:
	ASCII(.string	"pic_parameter_set_id out of range.\n" )
	.zero	4
.LC392:
	ASCII(.string	"PPS: seq_parameter_set_id" )
	.zero	6
.LC394:
	ASCII(.string	"seq_parameter_set_id out of range.\n" )
	.zero	4
.LC396:
	ASCII(.string	"SPS haven't decode.\n" )
	.zero	3
.LC399:
	ASCII(.string	"PPS decode error.\n" )
	.zero	5
.LC402:
	ASCII(.string	"VUI: aspect_ratio_info_present_flag" )
	.zero	4
.LC404:
	ASCII(.string	"VUI: aspect_ratio_idc" )
	.zero	2
.LC406:
	ASCII(.string	"VUI: sar_width" )
	.zero	1
.LC408:
	ASCII(.string	"VUI: sar_height" )
.LC410:
	ASCII(.string	"VUI: overscan_info_present_flag" )
.LC412:
	ASCII(.string	"VUI: overscan_appropriate_flag" )
	.zero	1
.LC414:
	ASCII(.string	"VUI: video_signal_type_present_flag" )
	.zero	4
.LC416:
	ASCII(.string	"VUI: video_format" )
	.zero	6
.LC418:
	ASCII(.string	"VUI: video_full_range_flag" )
	.zero	5
.LC420:
	ASCII(.string	"VUI: color_description_present_flag" )
	.zero	4
.LC422:
	ASCII(.string	"VUI: colour_primaries" )
	.zero	2
.LC424:
	ASCII(.string	"VUI: transfer_characteristics" )
	.zero	2
.LC426:
	ASCII(.string	"VUI: matrix_coefficients" )
	.zero	7
.LC428:
	ASCII(.string	"VUI: chroma_loc_info_present_flag" )
	.zero	6
.LC430:
	ASCII(.string	"VUI: chroma_sample_loc_type_top_field" )
	.zero	2
.LC432:
	ASCII(.string	"VUI: chroma_sample_loc_type_bottom_field" )
	.zero	7
.LC434:
	ASCII(.string	"VUI: timing_info_present_flag" )
	.zero	2
.LC436:
	ASCII(.string	"VUI: num_units_in_tick" )
	.zero	1
.LC438:
	ASCII(.string	"VUI: time_scale" )
.LC440:
	ASCII(.string	"VUI: fixed_frame_rate_flag" )
	.zero	5
.LC442:
	ASCII(.string	"VUI: nal_hrd_parameters_present_flag" )
	.zero	3
.LC444:
	ASCII(.string	"VUI: cpb_cnt_minus1" )
	.zero	4
.LC446:
	ASCII(.string	"VUI: bit_rate_scale" )
	.zero	4
.LC448:
	ASCII(.string	"VUI: cpb_size_scale" )
	.zero	4
.LC450:
	ASCII(.string	"hrd->cpb_cnt_minus1 out of range\n" )
	.zero	6
.LC452:
	ASCII(.string	"VUI: bit_rate_value_minus1" )
	.zero	5
.LC454:
	ASCII(.string	"VUI: cpb_size_value_minus1" )
	.zero	5
.LC456:
	ASCII(.string	"VUI: cbr_flag" )
	.zero	2
.LC458:
	ASCII(.string	"VUI: initial_cpb_removal_delay_length_minus1" )
	.zero	3
.LC460:
	ASCII(.string	"VUI: cpb_removal_delay_length_minus1" )
	.zero	3
.LC462:
	ASCII(.string	"VUI: dpb_output_delay_length_minus1" )
	.zero	4
.LC464:
	ASCII(.string	"VUI: time_offset_length" )
.LC466:
	ASCII(.string	"VUI: vcl_hrd_parameters_present_flag" )
	.zero	3
.LC468:
	ASCII(.string	"VUI: low_delay_hrd_flag" )
.LC470:
	ASCII(.string	"VUI: pic_struct_present_flag" )
	.zero	3
.LC472:
	ASCII(.string	"VUI: bitstream_restriction_flag" )
.LC474:
	ASCII(.string	"VUI: motion_vectors_over_pic_boundaries_flag" )
	.zero	3
.LC476:
	ASCII(.string	"VUI: max_bytes_per_pic_denom" )
	.zero	3
.LC478:
	ASCII(.string	"VUI: max_bits_per_mb_denom" )
	.zero	5
.LC480:
	ASCII(.string	"VUI: log2_max_mv_length_horizontal" )
	.zero	5
.LC482:
	ASCII(.string	"VUI: log2_max_mv_length_vertical" )
	.zero	7
.LC484:
	ASCII(.string	"VUI: num_reorder_frames" )
.LC486:
	ASCII(.string	"VUI: max_dec_frame_buffering" )
	.zero	3
.LC491:
	ASCII(.string	"dar=%d, final_dar=%d\n" )
	.zero	2
.LC497:
	ASCII(.string	"SPS: chroma_format_idc" )
	.zero	1
.LC499:
	ASCII(.string	"pSPS->chroma_format_idc out of range.\n" )
	.zero	1
.LC501:
	ASCII(.string	"SPS: bit_depth_luma_minus8" )
	.zero	5
.LC503:
	ASCII(.string	"bit_depth_luma(%d) not equal 8.\n" )
	.zero	7
.LC506:
	ASCII(.string	"SPS: bit_depth_chroma_minus8" )
	.zero	3
.LC508:
	ASCII(.string	"bit_depth_chroma(%d) not equal 8.\n" )
	.zero	5
.LC510:
	ASCII(.string	"SPS: qpprime_y_zero_trans_bypass_flag" )
	.zero	2
.LC512:
	ASCII(.string	"qpprime_y_zero_trans_bypass_flag not equal 0.\n" )
	.zero	1
.LC514:
	ASCII(.string	"SPS: seq_scaling_matrix_present_flag" )
	.zero	3
.LC518:
	ASCII(.string	"SPS: log2_max_frame_num_minus4" )
	.zero	1
.LC520:
	ASCII(.string	"pSPS->log2_max_frame_num_minus4 out of range.\n" )
	.zero	1
.LC522:
	ASCII(.string	"SPS: pic_order_cnt_type" )
.LC524:
	ASCII(.string	"pSPS->pic_order_cnt_type out of range.\n" )
.LC526:
	ASCII(.string	"SPS: log2_max_pic_order_cnt_lsb_minus4" )
	.zero	1
.LC528:
	ASCII(.string	"pSPS->log2_max_pic_order_cnt_lsb_minus4 out of range.\n" )
	.zero	1
.LC530:
	ASCII(.string	"SPS: delta_pic_order_always_zero_flag" )
	.zero	2
.LC532:
	ASCII(.string	"SPS: offset_for_non_ref_pic" )
	.zero	4
.LC534:
	ASCII(.string	"SPS: offset_for_top_to_bottom_field" )
	.zero	4
.LC536:
	ASCII(.string	"SPS: num_ref_frames_in_pic_order_cnt_cycle" )
	.zero	5
.LC538:
	ASCII(.string	"pSPS->num_ref_frames_in_pic_order_cnt_cycle out of range.\n" )
	.zero	5
.LC540:
	ASCII(.string	"SPS: offset_for_ref_frame[i]" )
	.zero	3
.LC542:
	ASCII(.string	"SPS: gaps_in_frame_num_value_allowed_flag" )
	.zero	6
.LC544:
	ASCII(.string	"SPS: pic_width_in_mbs_minus1" )
	.zero	3
.LC546:
	ASCII(.string	"pSPS->pic_width_in_mbs_minus1 out of range(=%d).\n" )
	.zero	6
.LC548:
	ASCII(.string	"SPS: pic_height_in_map_units_minus1" )
	.zero	4
.LC550:
	ASCII(.string	"SPS: frame_mbs_only_flag" )
	.zero	7
.LC552:
	ASCII(.string	"pSPS->pic_height_in_map_units_minus1 out of range.\n" )
	.zero	4
.LC554:
	ASCII(.string	"pic size too large.\n" )
	.zero	3
.LC557:
	ASCII(.string	"level_idc %d not support.\n" )
	.zero	5
.LC559:
	ASCII(.string	"dpb size according level : %d\n" )
	.zero	1
.LC561:
	ASCII(.string	"pSPS->num_ref_frames(%d) > DPB size(%d), try to select the reasonable one.\n" )
	.zero	4
.LC563:
	ASCII(.string	"SPS: direct_8x8_inference_flag" )
	.zero	1
.LC565:
	ASCII(.string	"SPS: frame_cropping_flag" )
	.zero	7
.LC567:
	ASCII(.string	"SPS: frame_cropping_rect_left_offset" )
	.zero	3
.LC569:
	ASCII(.string	"SPS: frame_cropping_rect_right_offset" )
	.zero	2
.LC571:
	ASCII(.string	"SPS: frame_cropping_rect_top_offset" )
	.zero	4
.LC573:
	ASCII(.string	"SPS: frame_cropping_rect_bottom_offset" )
	.zero	1
.LC575:
	ASCII(.string	"SPS: vui_parameters_present_flag" )
	.zero	7
.LC577:
	ASCII(.string	"max_dec_frame_buffering(%d) > MaxDpbSize(%d)\n" )
	.zero	2
.LC579:
	ASCII(.string	"SPS: num_ref_frames" )
	.zero	4
.LC581:
	ASCII(.string	"SPS: mb_adaptive_frame_field_flag" )
	.zero	6
.LC583:
	ASCII(.string	"SPS_SVC_EXT: inter_layer_deblocking_filter_control_present_flag" )
.LC585:
	ASCII(.string	"inter_layer_present_flag:%d!\n" )
	.zero	2
.LC587:
	ASCII(.string	"SPS_SVC_EXT: extended_spatial_scalability_idc" )
	.zero	2
.LC589:
	ASCII(.string	"spatial scalability not support!!\n" )
	.zero	5
.LC591:
	ASCII(.string	"SPS_SVC_EXT: chroma_phase_x_plus1_flag" )
	.zero	1
.LC593:
	ASCII(.string	"chroma_phase_x_plus1_flag:%d!!\n" )
.LC595:
	ASCII(.string	"SPS_SVC_EXT: chroma_phase_y_plus1" )
	.zero	6
.LC597:
	ASCII(.string	"chroma_phase_y_plus1:%d!\n" )
	.zero	6
.LC599:
	ASCII(.string	"SPS_SVC_EXT: seq_tcoeff_level_prediction_flag" )
	.zero	2
.LC601:
	ASCII(.string	"SPS_SVC_EXT: adaptive_tcoeff_level_prediction_flag" )
	.zero	5
.LC603:
	ASCII(.string	"adaptive_tcoeff_level_prediction_flag:%d!\n" )
	.zero	5
.LC605:
	ASCII(.string	"SPS_SVC_EXT: slice_header_restriction_flag" )
	.zero	5
.LC607:
	ASCII(.string	"slice_header_restriction_flag error:%d,%x,%d\n" )
	.zero	2
.LC609:
	ASCII(.string	"SPS: profile_idc" )
	.zero	7
.LC611:
	ASCII(.string	"SPS: constrained_set0_flag" )
	.zero	5
.LC613:
	ASCII(.string	"SPS: constrained_set1_flag" )
	.zero	5
.LC615:
	ASCII(.string	"SPS: constrained_set2_flag" )
	.zero	5
.LC617:
	ASCII(.string	"SPS: constrained_set3_flag" )
	.zero	5
.LC619:
	ASCII(.string	"SPS: reserved_zero_4bits" )
	.zero	7
.LC621:
	ASCII(.string	"SPS: level_idc" )
	.zero	1
.LC623:
	ASCII(.string	"SPS: seq_parameter_set_id" )
	.zero	6
.LC628:
	ASCII(.string	"Baseline stream, try to decode, exit when FMO occurred.\n" )
	.zero	7
.LC630:
	ASCII(.string	"EXTENDED stream, try to decode, exit when datapartition occurred.\n" )
	.zero	5
.LC632:
	ASCII(.string	"others High profile stream, try to decode, exit when high profile not support occurred.\n" )
	.zero	7
.LC634:
	ASCII(.string	"profile_idc = %5d error, try to decode as main profile.\n" )
	.zero	7
.LC636:
	ASCII(.string	"level_idc = %5d error, try to decode as level_idc %d.\n" )
	.zero	1
.LC639:
	ASCII(.string	"Line %d: SPS[%d] decode error.\n" )
.LC642:
	ASCII(.string	"svc ext error:\n" )
.LC645:
	ASCII(.string	"SEI: frame_packing_arrangement_id" )
	.zero	6
.LC647:
	ASCII(.string	"SEI: frame_packing_arrangement_cancel_flag" )
	.zero	5
.LC649:
	ASCII(.string	"SEI: frame_packing_arrangement_type" )
	.zero	4
.LC651:
	ASCII(.string	"SEI: quincunx_sampling_flag" )
	.zero	4
.LC653:
	ASCII(.string	"SEI: content_interpretation_type" )
	.zero	7
.LC655:
	ASCII(.string	"SEI: spatial_flipping_flag" )
	.zero	5
.LC657:
	ASCII(.string	"SEI: frame0_flipped_flag" )
	.zero	7
.LC659:
	ASCII(.string	"SEI: field_views_flag" )
	.zero	2
.LC661:
	ASCII(.string	"SEI: current_frame_is_frame0_flag" )
	.zero	6
.LC663:
	ASCII(.string	"SEI: frame0_self_contained_flag" )
.LC665:
	ASCII(.string	"SEI: frame1_self_contained_flag" )
.LC667:
	ASCII(.string	"SEI: frame0_grid_position_x" )
	.zero	4
.LC669:
	ASCII(.string	"SEI: frame0_grid_position_y" )
	.zero	4
.LC671:
	ASCII(.string	"SEI: frame1_grid_position_x" )
	.zero	4
.LC673:
	ASCII(.string	"SEI: frame1_grid_position_y" )
	.zero	4
.LC675:
	ASCII(.string	"SEI: frame_packing_arrangement_reserved_byte" )
	.zero	3
.LC677:
	ASCII(.string	"SEI: frame_packing_arrangement_repetition_period" )
	.zero	7
.LC679:
	ASCII(.string	"SEI: frame_packing_arrangement_extension_flag" )
	.zero	2
.LC681:
	ASCII(.string	"DecPicTimingSEI but SPS is invalid.\n" )
	.zero	3
.LC683:
	ASCII(.string	"SEI: cpb_removal_delay" )
	.zero	1
.LC685:
	ASCII(.string	"SEI: dpb_output_delay" )
	.zero	2
.LC687:
	ASCII(.string	"pic_struct" )
	.zero	5
.LC689:
	ASCII(.string	"SEI nal dec payload type err\n" )
	.zero	2
.LC691:
	ASCII(.string	"SEI nal dec payloadSize err\n" )
	.zero	3
.LC694:
	ASCII(.string	"SEI_BUFFERING_PERIOD,offset = %d,payload_size = %d \n" )
	.zero	3
.LC697:
	ASCII(.string	"PassBytes err [%s][%d]\n" )
.LC699:
	ASCII(.string	"SEI_PIC_TIMING,offset = %d,payload_size = %d \n" )
	.zero	1
.LC701:
	ASCII(.string	"SEI_PAN_SCAN_RECT,offset = %d,payload_size = %d \n" )
	.zero	6
.LC703:
	ASCII(.string	"SEI_FILLER_PAYLOAD,offset = %d,payload_size = %d \n" )
	.zero	5
.LC705:
	ASCII(.string	"SEI_USER_DATA_REGISTERED_ITU_T_T35, offset = %d, payload_size = %d \n" )
	.zero	3
.LC707:
	ASCII(.string	"SEI_USER_DATA_UNREGISTERED, offset = %d, payload_size = %d \n" )
	.zero	3
.LC709:
	ASCII(.string	"SEI: itu_t_t35_country_code" )
	.zero	4
.LC711:
	ASCII(.string	"SEI: itu_t_t35_country_code_extension_byte" )
	.zero	5
.LC713:
	ASCII(.string	"SEI: itu_t_t35_provider_code" )
	.zero	3
.LC716:
	ASCII(.string	"len_tmp = %d cann't dec usrdata\n" )
	.zero	7
.LC718:
	ASCII(.string	"cann't dec usrdata\n" )
	.zero	4
.LC720:
	ASCII(.string	"SEI_RECOVERY_POINT,offset = %d,payload_size = %d \n" )
	.zero	5
.LC722:
	ASCII(.string	"SEI_DEC_REF_PIC_MARKING_REPETITION,offset = %d,payload_size = %d \n" )
	.zero	5
.LC724:
	ASCII(.string	"SEI_SPARE_PIC,offset = %d,payload_size = %d \n" )
	.zero	2
.LC726:
	ASCII(.string	"SEI_SCENE_INFO,offset = %d,payload_size = %d \n" )
	.zero	1
.LC728:
	ASCII(.string	"SEI_SUB_SEQ_INFO,offset = %d,payload_size = %d \n" )
	.zero	7
.LC730:
	ASCII(.string	"SEI_SUB_SEQ_LAYER_CHARACTERISTICS,offset = %d,payload_size = %d \n" )
	.zero	6
.LC732:
	ASCII(.string	"SEI_SUB_SEQ_CHARACTERISTICS,offset = %d,payload_size = %d \n" )
	.zero	4
.LC734:
	ASCII(.string	"SEI_FULL_FRAME_FREEZE,offset = %d,payload_size = %d \n" )
	.zero	2
.LC736:
	ASCII(.string	"SEI_FULL_FRAME_FREEZE_RELEASE,offset = %d,payload_size = %d \n" )
	.zero	2
.LC738:
	ASCII(.string	"SEI_FULL_FRAME_SNAPSHOT,offset = %d,payload_size = %d \n" )
.LC740:
	ASCII(.string	"SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START,offset = %d,payload_size = %d \n" )
	.zero	7
.LC742:
	ASCII(.string	"SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END,offset = %d,payload_size = %d \n" )
	.zero	1
.LC744:
	ASCII(.string	"SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,offset = %d,payload_size = %d \n" )
	.zero	1
.LC746:
	ASCII(.string	"SEI_FILM_GRAIN_CHARACTERISTICS,offset = %d,payload_size = %d \n" )
	.zero	1
.LC748:
	ASCII(.string	"SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,offset = %d,payload_size = %d \n" )
	.zero	7
.LC750:
	ASCII(.string	"SEI_STEREO_VIDEO_INFO,offset = %d,payload_size = %d \n" )
	.zero	2
.LC752:
	ASCII(.string	"too much usrdat, cann't dec SEI\n" )
	.zero	7
.LC756:
	ASCII(.string	"\nthe stream is error,len:%d\n" )
	.zero	3
.LC761:
	ASCII(.string	"\nSlice nal or IDR nal = %d\n" )
	.zero	4
.LC763:
	ASCII(.string	"sliceheader dec err\n" )
	.zero	3
.LC765:
	ASCII(.string	"H264Dec destroy\n" )
	.zero	7
.LC767:
	ASCII(.string	"H264 recycle image\n" )
	.zero	4
.LC769:
	ASCII(.string	"pH264Ctx is %p, pFrameStore is %p\n" )
	.zero	5
.LC771:
	ASCII(.string	"line; %d pstLogicFsImage is NULL!\n" )
	.zero	5
.LC773:
	ASCII(.string	"InsertVO err:%d, ClearAll\n" )
	.zero	5
.LC775:
	ASCII(.string	"H264.c line %d: frame para err(ret=%d), recycle image self\n" )
	.zero	4
.LC777:
	ASCII(.string	"line: %d pstLogicFsImage is NULL!\n" )
	.zero	5
.LC781:
	ASCII(.string	"[%s][%d] OutputFrmToVO return %d\n" )
	.zero	6
.LC786:
	ASCII(.string	"line %d: flush dpb return %d\n" )
	.zero	2
.LC788:
	ASCII(.string	"----------------- Imode -> %d ------------------\n" )
	.zero	6
.LC790:
	ASCII(.string	"I_MODE but first_mb_in_slice != 0\n" )
	.zero	5
.LC794:
	ASCII(.string	"Discard this B(poc=%d) befor P, is_ref_idc=%d.\n" )
.LC796:
	ASCII(.string	"***** VDM start, TotalPicNum=%d, pH264Ctx->CurrPic.structure=%d.\n" )
	.zero	6
.LC798:
	ASCII(.string	"DecVDM ERR\n" )
	.zero	4
.LC800:
	ASCII(.string	"\n receive streambuff=%p; phy_addr=0x%x; bitstream_length=%d\n" )
	.zero	3
.LC802:
	ASCII(.string	"nal_release_err\n" )
	.zero	7
.LC804:
	ASCII(.string	"cann't find slot for current nal\n" )
	.zero	6
.LC807:
	ASCII(.string	"receive a zero packet\n" )
	.zero	1
.LC809:
	ASCII(.string	"clear all dec para\n" )
	.zero	4
.LC812:
	ASCII(.string	"[%s %d], pos is %d out of range\n" )
	.zero	7
.LC814:
	ASCII(.string	"line %d, get APC error, ret %d\n" )
.LC816:
	ASCII(.string	"InserFrmInDPB: cur pic struct = %d!\n" )
	.zero	3
.LC819:
	ASCII(.string	"CurrPic.state is 'EMPTY'\n" )
	.zero	6
.LC821:
	ASCII(.string	"StorePicInDPB return(%d) from L%d\n" )
	.zero	5
.LC823:
	ASCII(.string	"directoutput mode\n" )
	.zero	5
.LC825:
	ASCII(.string	"line: %d pImg is NULL!\n" )
.LC827:
	ASCII(.string	"line %d, REPORT_IFRAME_ERR\n" )
	.zero	4
.LC830:
	ASCII(.string	"line %d, return %d\n" )
	.zero	4
.LC832:
	ASCII(.string	"err(%d) > ref_thr(%d)\n" )
	.zero	1
.LC834:
	ASCII(.string	"marking return %d\n" )
	.zero	5
.LC836:
	ASCII(.string	"return value is unexpeced!\n" )
	.zero	4
.LC839:
	ASCII(.string	"line %d: pH264Ctx->DPB.size = %d, ref %d, ltref %d\n" )
	.zero	4
.LC842:
	ASCII(.string	"%s: pos(%d) = pre_pos, force return.\n" )
	.zero	2
.LC844:
	ASCII(.string	"FS_ALLOC_ERR, ClearAll\n" )
.LC847:
	ASCII(.string	"Can not new logic fs! ClearAll\n" )
.LC849:
	ASCII(.string	"GAP found while DPB is empty!\n" )
	.zero	1
.LC851:
	ASCII(.string	"frame num gap try to fill it \n" )
	.zero	1
.LC853:
	ASCII(.string	"CurrFrameNum = %d  UnusedShortTermFrameNum = %d\n" )
	.zero	7
.LC855:
	ASCII(.string	"cann't allocate frame store when gap find\n" )
	.zero	5
.LC857:
	ASCII(.string	"allocate frame store when gap find\n" )
	.zero	4
.LC859:
	ASCII(.string	"line %d: store gap pic err, ret=%d\n" )
	.zero	4
.LC863:
	ASCII(.string	"%s %d unknow error!!\n" )
	.zero	2
.LC865:
	ASCII(.string	"-1 == VCTRL_GetChanIDByCtx() Err! \n" )
	.zero	4
.LC868:
	ASCII(.string	"%s %d frame_store is null!!\n" )
	.zero	3
.LC872:
	ASCII(.string	"line %d: init dpb return %d\n" )
	.zero	3
.LC874:
	ASCII(.string	"resolution error, the CAP_LEVEL_USER_DEFINE_WITH_OPTION channel with s32ReRangeEn == 0 can't support such bitstream.\n" )
	.zero	2
.LC876:
	ASCII(.string	"s32MaxRefFrameNum < pH264Ctx->DPB.size, the CAP_LEVEL_USER_DEFINE_WITH_OPTION channel can't support such bitstream.\n" )
	.zero	3
.LC878:
	ASCII(.string	"mem arrange err, ClearAll\n" )
	.zero	5
.LC880:
	ASCII(.string	"get back frm\n" )
	.zero	2
.LC883:
	ASCII(.string	"line %d: frame gap(=%d) > dpb size(=%d)\n" )
	.zero	7
.LC885:
	ASCII(.string	"line %d: dec gap failed\n" )
	.zero	7
.LC887:
	ASCII(.string	"line %d: alloc framestore failed\n" )
	.zero	6
.LC889:
	ASCII(.string	"dec_pts: %lld\n" )
	.zero	1
.LC891:
	ASCII(.string	"dec_usertag: %lld\n" )
	.zero	5
.LC893:
	ASCII(.string	"%s %d NULL == pH264Ctx->CurrPic.frame_store!!\n" )
	.zero	1
.LC898:
	ASCII(.string	"Start Reason: SliceParaNum, MaxBytesReceived = %d, %d(thr=%d)\n" )
	.zero	1
.LC900:
	ASCII(.string	"Too many slice or bitstream, err!\n" )
	.zero	5
.LC902:
	ASCII(.string	"Start Reason: new_pic_flag\n" )
	.zero	4
.LC904:
	ASCII(.string	"init pic err, find next recover point or next valid sps, pps, or exit\n" )
	.zero	1
.LC906:
	ASCII(.string	"DecList error, ret=%d\n" )
	.zero	1
.LC908:
	ASCII(.string	"dec list err.\n" )
	.zero	1
.LC910:
	ASCII(.string	"pH264Ctx->TotalNal = %d, type:%d\n" )
	.zero	6
.LC912:
	ASCII(.string	"stop i want\n" )
	.zero	3
.LC914:
	ASCII(.string	"svc_extension_flag error:priority_id:%d, pred_flag%d\n" )
	.zero	2
.LC916:
	ASCII(.string	"%d,%d,%d\n" )
	.zero	6
.LC918:
	ASCII(.string	"svc stream,nal_header:%x\n" )
	.zero	6
.LC921:
	ASCII(.string	"***** NAL: IDR/Slice, nal_unit_type=%d, TotalSlice=%d\n" )
	.zero	1
.LC923:
	ASCII(.string	"SliceCheck failed, clear current slice.\n" )
	.zero	7
.LC926:
	ASCII(.string	"***** NAL: PPS, nal_unit_type=%d, TotalPPS=%d\n" )
	.zero	1
.LC929:
	ASCII(.string	"***** NAL: SPS, nal_unit_type=%d, TotalSPS=%d\n" )
	.zero	1
.LC931:
	ASCII(.string	"SPS decode error.\n" )
	.zero	5
.LC933:
	ASCII(.string	"***** NAL: SEI, nal_unit_type=%d\n" )
	.zero	6
.LC935:
	ASCII(.string	"SEI decode error.\n" )
	.zero	5
.LC937:
	ASCII(.string	"***** NAL: AUD, nal_unit_type=%d\n" )
	.zero	6
.LC939:
	ASCII(.string	"***** NAL: EOSEQ, nal_unit_type=%d\n" )
	.zero	4
.LC941:
	ASCII(.string	"***** NAL: FILL, nal_unit_type=%d\n" )
	.zero	5
.LC943:
	ASCII(.string	"***** NAL: EOSTREAM, nal_unit_type=%d\n" )
	.zero	1
.LC945:
	ASCII(.string	"***** NAL: SPSEXT, nal_unit_type=%d\n" )
	.zero	3
.LC947:
	ASCII(.string	"***** NAL: AUX, nal_unit_type=%d\n" )
	.zero	6
.LC949:
	ASCII(.string	"***** NAL: EOPIC, nal_unit_type=%d\n" )
	.zero	4
.LC951:
	ASCII(.string	"***** NAL: UNSUPPORT, nal_unit_type=%d,nalu header:%x\n" )
	.zero	1
.LC957:
	ASCII(.string	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d,new w/h = %d/%d\n" )
	.zero	3
.LC960:
	ASCII(.string	"FindTrailZeros ERR\n" )
	.zero	4
.LC962:
	ASCII(.string	"H264_DecOneNal ERR\n" )
	.zero	4
.LC965:
	ASCII(.string	"store pic err, ret = %d\n" )
	.zero	7
.LC967:
	ASCII(.string	"imode nal store pic err, ret = %d\n" )
	.zero	5
.LC969:
	ASCII(.string	"imode nal flush dpb err, ret = %d\n" )
	.zero	5
.LC971:
	ASCII(.string	"cann't find FrameStore\n" )
.LC973:
	ASCII(.string	"========== H264 FrameStore state(is_used, is_in_dpb, IsOutDPB) ========\n" )
	.zero	7
.LC975:
	ASCII(.string	"ReadImgNum = %d, NewImgNum = %d\n" )
	.zero	7
.LC977:
	ASCII(.string	"%02d: %d %d %d\n" )
.LC979:
	ASCII(.string	"FrameStore leak, ClearAll\n" )
	.bss
	.align	3
.LANCHOR2 = . + 0
.LANCHOR3 = . + 4352
	.type	pps_tmp.12769, %object
	.size	pps_tmp.12769, 2208
pps_tmp.12769:
	.zero	2208
	.type	sps_tmp.12918, %object
	.size	sps_tmp.12918, 4000
sps_tmp.12918:
	.zero	4000
	.type	TmpBuf, %object
	.size	TmpBuf, 68
TmpBuf:
	.zero	68
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
