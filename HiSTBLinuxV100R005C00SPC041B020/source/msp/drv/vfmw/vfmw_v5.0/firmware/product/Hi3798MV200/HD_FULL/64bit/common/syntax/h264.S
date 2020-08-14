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
	ldp	w0, w2, [x3, 160]
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
	ldr	w0, [x3, 164]
	add	w0, w0, 4
	str	w0, [x3, 164]
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
	ldr	w0, [x3, 164]
	add	w0, w0, 2
	str	w0, [x3, 164]
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
	add	x0, x20, 20480
	mov	w2, 2208
	ldr	x7, [x20, 14968]
	mov	w1, 4000
	ldr	w5, [x19, 116]
	add	x4, x20, 61440
	ldr	w3, [x0, 1596]
	mov	w0, -772
	ldr	x6, [x20, 14960]
	and	w0, w5, w0
	umull	x2, w3, w2
	add	x2, x7, x2
	ldr	w2, [x2, 28]
	str	w0, [x19, 116]
	umull	x1, w2, w1
	ldr	w3, [x4, 2796]
	add	x1, x6, x1
	lsl	w2, w3, 10
	str	w2, [x19, 144]
	ldrb	w2, [x1, 18]
	cbz	w2, .L477
	ldr	w2, [x4, 808]
	lsl	w2, w2, 4
	cmp	w2, 1920
	bhi	.L478
.L477:
	orr	w0, w0, 131072
	str	w0, [x19, 116]
	mov	w2, 0
	ldr	x3, [x20]
	ldr	w3, [x3, 632]
	cmp	w3, 1
	beq	.L529
.L480:
	ldrb	w3, [x19, 1]
	orr	w0, w0, w2
	str	w0, [x19, 116]
	cmp	w3, 3
	bhi	.L527
	ldr	x2, .LC46
	ldrh	w2, [x2,w3,uxtw #1]
	adr	x4, .Lrtx483
	add	x2, x4, w2, sxth #2
	br	x2
.Lrtx483:
	.section	.rodata
	.align	0
	.align	2
.L483:
	.2byte	(.L482 - .Lrtx483) / 4
	.2byte	(.L484 - .Lrtx483) / 4
	.2byte	(.L485 - .Lrtx483) / 4
	.2byte	(.L482 - .Lrtx483) / 4
	.text
	.p2align 3
.L484:
	ldr	w2, [x19, 660]
	mov	w4, 3
	ldr	w6, [x19, 664]
	and	w2, w2, w4
	ldr	w5, [x19, 740]
	orr	w2, w2, 1792
	str	w6, [x19, 168]
	orr	w0, w2, w0
	str	w0, [x19, 116]
	stp	w5, w4, [x19, 108]
.L527:
	add	x22, x20, 65536
	add	x21, x20, 24576
.L481:
	ldrb	w0, [x1, 18]
	mov	w2, 0
	str	w0, [x19, 484]
	str	w0, [x19, 488]
	ldrb	w0, [x1, 22]
	cbz	w0, .L500
	ldr	w2, [x1, 3332]
.L500:
	mov	w0, 1
	str	w2, [x19, 496]
	str	w0, [x19, 492]
	ldr	x0, [x20]
	ldr	w0, [x0, 12]
	cmp	w0, wzr
	ble	.L501
	ldrb	w0, [x19, 680]
	cbz	w0, .L530
.L501:
	mov	x0, x20
	mov	x1, x19
	bl	ExchangePts
	ldrsb	w0, [x21, 3192]
	cbz	w0, .L502
	ldr	w0, [x21, 3208]
	cmp	w0, 3
	beq	.L531
	cmp	w0, 4
	beq	.L532
	cmp	w0, 5
	beq	.L533
	str	wzr, [x19, 364]
	mov	w0, 0
	ldr	x1, .LC48
	ldr	w2, [x21, 3208]
	bl	dprint_vfmw
	.p2align 2
.L502:
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
.L530:
	cmp	w3, 3
	bne	.L501
	ldr	w0, [x19, 116]
	and	w0, w0, -769
	str	w0, [x19, 116]
	b	.L501
	.p2align 3
.L478:
	mov	w0, 64764
	mov	w2, 0
	movk	w0, 0xfffd, lsl 16
	and	w0, w5, w0
	str	w0, [x19, 116]
	ldr	x3, [x20]
	ldr	w3, [x3, 632]
	cmp	w3, 1
	bne	.L480
.L529:
	and	w3, w0, 131072
	mov	w2, 262144
	cmp	w3, wzr
	csel	w2, wzr, w2, ne
	b	.L480
	.p2align 3
.L482:
	ldrb	w2, [x19, 680]
	cbnz	w2, .L486
	ldr	w4, [x19, 664]
	ldr	w2, [x19, 656]
	str	w4, [x19, 168]
	ldr	w6, [x19, 660]
	ldr	w8, [x19, 740]
	ldr	w7, [x19, 780]
	cbnz	w2, .L487
	mov	w2, 3584
	add	x22, x20, 65536
.L488:
	ldr	w5, [x19, 732]
	ldr	w4, [x19, 772]
	cmp	w5, w4
	beq	.L534
	cset	w5, le
.L498:
	add	x21, x20, 24576
	ldrb	w4, [x21, 3228]
	cmp	w4, 3
	beq	.L510
	cmp	w4, 4
	lsl	w5, w5, 12
	mov	w4, 0
	csel	w5, w5, w4, ne
.L499:
	and	w4, w6, 3
	and	w0, w0, -12289
	orr	w4, w4, w5
	orr	w0, w0, w2
	orr	w0, w4, w0
	str	w0, [x19, 116]
	stp	w8, w7, [x19, 108]
	b	.L481
	.p2align 3
.L485:
	ldr	w2, [x19, 660]
	mov	w4, 2816
	ldr	w7, [x19, 664]
	mov	w6, 3
	and	w2, w2, 3
	ldr	w5, [x19, 780]
	orr	w2, w2, w4
	add	x22, x20, 65536
	orr	w0, w2, w0
	add	x21, x20, 24576
	stp	w6, w5, [x19, 108]
	str	w0, [x19, 116]
	str	w7, [x19, 168]
	b	.L481
	.p2align 3
.L531:
	mov	w0, 1
	str	w0, [x19, 364]
	b	.L502
	.p2align 3
.L533:
	mov	w0, 3
	str	w0, [x19, 364]
	b	.L502
	.p2align 3
.L486:
	add	x22, x20, 65536
	ldr	w5, [x19, 736]
	ldr	w9, [x19, 776]
	ldr	w2, [x22, 112]
	cmp	w5, w2
	bhi	.L535
	cmp	w2, w9
	bcc	.L492
.L490:
	ldr	w2, [x19, 664]
	ldr	w6, [x19, 660]
	ldr	w8, [x19, 740]
	ldr	w7, [x19, 780]
	str	w2, [x19, 168]
	mov	w2, 3
.L493:
	ldrb	w4, [x19, 3]
	cmp	w4, 1
	beq	.L536
	cmp	w4, 2
	beq	.L496
	lsl	w2, w2, 10
	orr	w2, w2, 768
	stp	w8, w7, [x19, 108]
	b	.L488
	.p2align 3
.L492:
	ldr	w6, [x19, 740]
	mov	w2, 1
	ldr	w7, [x19, 780]
	mov	w8, w6
	str	w5, [x19, 168]
	b	.L493
	.p2align 3
.L532:
	mov	w0, 2
	str	w0, [x19, 364]
	b	.L502
	.p2align 3
.L510:
	mov	w5, 4096
	b	.L499
	.p2align 3
.L534:
	ldr	x5, [x19, 120]
	mov	x4, 720
	movk	x4, 0x1e0, lsl 32
	cmp	x5, x4
	cset	w5, ne
	b	.L498
	.p2align 3
.L535:
	cmp	w2, w9
	bcc	.L490
	ldr	w6, [x19, 780]
	mov	w2, 2
	ldr	w8, [x19, 740]
	mov	w7, w6
	str	w9, [x19, 168]
	b	.L493
	.p2align 3
.L487:
	mov	w2, 3840
	add	x22, x20, 65536
	b	.L488
	.p2align 3
.L536:
	mov	w6, w7
	mov	w2, 2816
	str	w9, [x19, 168]
	stp	w8, w7, [x19, 108]
	b	.L488
	.p2align 3
.L496:
	mov	w6, w8
	mov	w2, 1792
	str	w5, [x19, 168]
	stp	w8, w7, [x19, 108]
	b	.L488
	.size	GetImagePara, .-GetImagePara
	.align	3
.LC46:
	.xword	.L483
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
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bls	.L538
	mov	x1, x20
	mov	x0, x19
	bl	SetFrmRepeatCount.part.1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L538:
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
	cbz	w3, .L542
	ldrb	w1, [x19, 721]
	cbz	w1, .L542
	mov	w1, 1
	strb	w1, [x19, 681]
.L544:
	mov	w3, 0
.L545:
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
.L542:
	ldr	w1, [x19, 760]
	strb	wzr, [x19, 681]
	tst	w1, 16776960
	beq	.L544
	ldr	w1, [x19, 720]
	mov	w3, 1
	tst	w1, 16776960
	bne	.L545
	b	.L544
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
	cbz	x22, .L578
	cmp	w19, 1
	beq	.L558
	cmp	w19, 2
	beq	.L559
	ldr	w23, [x22, 732]
	ldr	w24, [x22, 772]
.L560:
	add	x21, x20, 24576
	ldr	w2, [x21, 3992]
	cbz	w2, .L569
	ldr	w0, [x21, 3736]
	cbz	w0, .L579
	add	x0, x21, 3740
	mov	w19, 0
	b	.L563
	.p2align 3
.L567:
	ldr	w1, [x0], 4
	cbz	w1, .L561
.L563:
	add	w19, w19, 1
	cmp	w19, w2
	bne	.L567
.L569:
	mov	w0, -1
.L573:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L559:
	ldr	w23, [x22, 772]
	mov	w24, w23
	b	.L560
.L579:
	mov	w19, 0
	.p2align 2
.L561:
	add	x0, x20, 65536
	ldr	w1, [x22, 632]
	ldr	w0, [x0, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L564
	ldr	x1, [x0, 640]
	cbz	x1, .L564
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
.L558:
	ldr	w23, [x22, 732]
	mov	w24, w23
	b	.L560
.L564:
	ldrb	w3, [x22, 2]
	mov	x4, x0
	ldr	x1, .LC57
	mov	x5, 0
	ldr	w2, [x22, 632]
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L573
.L578:
	ldr	x2, .LC51
	mov	w3, 2220
	ldr	x1, .LC53
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L573
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
	bne	.L580
	mov	x0, x19
	bl	SlidingWinMark.part.2
.L580:
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
	tbz	x0, 14, .L582
	add	x21, x22, 24576
	ldr	w0, [x21, 3716]
	cbz	w0, .L588
	ldr	x20, .LC61
	mov	w19, 0
	.p2align 2
.L587:
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
	bhi	.L587
.L588:
	ldr	x25, .LC59
	mov	w0, 14
	mov	x1, x25
	bl	dprint_vfmw
	ldr	w0, [x21, 3720]
	cbz	w0, .L586
	ldr	x24, .LC63
	mov	w19, 0
	ldr	x23, .LC65
	.p2align 2
.L589:
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
	bhi	.L589
.L586:
	mov	x1, x25
	mov	w0, 14
	bl	dprint_vfmw
.L582:
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
	cbnz	w3, .L596
	ldrb	w2, [x1, 117]
	mov	w0, 3
	cmp	w2, wzr
	csel	w2, w2, w0, eq
.L596:
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
	beq	.L599
	cbz	w4, .L600
	cmp	w4, 2
	bne	.L595
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
	cbz	w7, .L622
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
	beq	.L608
	ldr	x2, [x19, 608]
	cmn	x2, #1
	beq	.L623
.L608:
	ldr	w2, [x19, 740]
.L626:
	cmp	w2, w6
	mov	x0, x19
	csel	w6, w2, w6, cs
	str	w6, [x19, 660]
	ldr	w1, [x1, 780]
	str	w1, [x19, 648]
	bl	CombineFldsToFrm
.L595:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L600:
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
.L599:
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
	cbz	w7, .L624
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
	beq	.L604
	ldr	x2, [x19, 608]
	cmn	x2, #1
	beq	.L625
.L604:
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
.L622:
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
.L624:
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
	b	.L595
	.p2align 3
.L625:
	str	x0, [x19, 608]
	b	.L604
	.p2align 3
.L623:
	str	x0, [x19, 608]
	ldr	w2, [x19, 740]
	b	.L626
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
	bcs	.L633
.L628:
	add	x19, x19, 61440
	ldr	w0, [x19, 776]
	strb	wzr, [x19, 116]
	cbz	w0, .L627
	mov	w0, 1
	strb	w0, [x19, 117]
.L627:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L633:
	ldr	x1, [x19, 27904]
	mov	x0, x19
	bl	UnMarkFrameStoreRef
	mov	w1, 0
	mov	x0, x19
	bl	RemoveFrameStoreOutDPB
	b	.L628
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
	cbz	x20, .L643
	ldr	x23, .LC69
	add	x19, x20, 8
	strb	wzr, [x20, 1]
	add	x22, x20, 72
	strb	wzr, [x20, 4]
.L638:
	ldr	x1, [x19]
	mov	w0, w21
	cbz	x1, .L637
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
.L637:
	add	x19, x19, 32
	cmp	x19, x22
	bne	.L638
	strb	wzr, [x20]
	str	wzr, [x20, 76]
.L634:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L643:
	ldr	x2, .LC66
	mov	w3, 4204
	ldr	x1, .LC67
	mov	w0, 0
	bl	dprint_vfmw
	b	.L634
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
	cbz	x1, .L645
	add	x0, x19, 65536
	ldr	w0, [x0, 116]
	bl	ReleaseNAL
	str	xzr, [x19, 632]
.L645:
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
	cbz	x1, .L650
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L650:
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
	b	.L656
	.p2align 3
.L655:
	add	x19, x19, 104
	cmp	x19, x20
	beq	.L658
.L656:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L655
	ldr	w0, [x21, 116]
	mov	x1, x19
	add	x19, x19, 104
	bl	ReleaseNAL
	cmp	x19, x20
	bne	.L656
.L658:
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
.L661:
	ldr	x0, [x19]
	mov	x1, x0
	cbz	x0, .L660
	ldr	w0, [x21, 116]
	bl	ReleaseNAL
	str	xzr, [x19]
.L660:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L661
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
	beq	.L668
	add	x0, x19, 24576
	mov	w1, 5
	strb	wzr, [x29, 68]
	sxtb	w4, w22
	strb	w1, [x29, 69]
	ldr	w0, [x0, 3712]
	add	w0, w0, 1
	strb	w0, [x29, 67]
.L674:
	add	x20, x19, 65536
	add	x1, x29, 64
	strb	w4, [x29, 70]
	strb	wzr, [x29, 66]
	ldr	w0, [x20, 116]
	bl	FSP_ConfigInstance
	cbnz	w0, .L669
	ldr	x1, [x19]
	add	x3, x29, 60
	ldr	w0, [x20, 116]
	ldr	w2, [x1, 24]
	ldr	w1, [x1, 20]
	bl	FSP_ConfigFrameBuf
	cmp	w0, 2
	mov	w1, w0
	beq	.L677
	mov	w0, 1
	cbnz	w1, .L671
.L675:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L668:
	mov	w1, 1
	mov	w0, 5
	strb	w1, [x29, 67]
	mov	w4, 1
	strb	wzr, [x29, 68]
	strb	w0, [x29, 69]
	b	.L674
	.p2align 3
.L677:
	add	x0, x19, 8
	bl	ResetVoQueue
.L671:
	ldr	x1, .LC76
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L675
	.p2align 3
.L669:
	ldr	x1, .LC78
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L675
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
	beq	.L697
	cbz	w2, .L681
	cmp	w2, 2
	mov	w0, 0
	bne	.L709
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L709:
	ldr	x1, .LC80
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearCurrSlice
	ldr	x19, [sp, 16]
	mov	w0, -1
	ldp	x29, x30, [sp], 32
	ret
.L681:
	mov	w11, 1
.L680:
	add	x6, x19, 28672
	add	x7, x7, 1628
	add	x6, x6, 32
	mov	x8, 0
	mov	x9, 0
	mov	w2, 0
	mov	w13, 1
	add	x12, x19, 61440
	.p2align 2
.L693:
	ldr	w0, [x7]
	cbz	w0, .L683
	ldr	x1, [x6]
	cbnz	x1, .L710
	sub	w3, w0, #1
	add	x10, x8, 3589
	add	x0, x6, 8
	add	x5, x10, x3, uxtw
	add	x5, x19, x5, lsl 3
	b	.L686
	.p2align 3
.L687:
	ldr	x4, [x0], 8
	cbnz	x4, .L684
.L686:
	cmp	x0, x5
	bne	.L687
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L711
	ldr	x9, [x12, 752]
	add	x9, x9, 680
.L694:
	add	x3, x10, x3, uxtw
	mov	x0, x6
	add	x3, x19, x3, lsl 3
	cbz	x1, .L712
	.p2align 2
.L689:
	add	x0, x0, 8
	cmp	x0, x3
	beq	.L690
.L713:
	ldr	x1, [x0]
	cbnz	x1, .L689
.L712:
	str	x9, [x0]
	add	x0, x0, 8
	cmp	x0, x3
	bne	.L713
	.p2align 2
.L690:
	add	w2, w2, 1
	add	x7, x7, 4
	cmp	w2, w11
	add	x6, x6, 264
	add	x8, x8, 33
	bcc	.L693
.L715:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L683:
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L714
	str	w13, [x7]
	add	w2, w2, 1
	cmp	w2, w11
	add	x7, x7, 4
	ldr	x0, [x12, 752]
	add	x8, x8, 33
	add	x0, x0, 680
	str	x0, [x6]
	add	x6, x6, 264
	bcc	.L693
	b	.L715
.L710:
	sub	w3, w0, #1
	mov	x4, x1
	add	x10, x8, 3589
	.p2align 2
.L684:
	mov	x9, x4
	b	.L694
.L697:
	mov	w11, 2
	b	.L680
.L714:
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
.L711:
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
	cbz	w1, .L717
	ldrb	w0, [x0, 1586]
	mov	w5, 2
	cmp	w0, wzr
	csinc	w5, w5, wzr, ne
.L717:
	add	x0, x20, 24576
	ldr	w2, [x0, 3720]
	cbz	w2, .L725
	sub	w2, w2, #1
	add	x0, x0, 3456
	add	x2, x2, 3505
	add	x4, x20, x2, lsl 3
	b	.L722
	.p2align 3
.L735:
	cmp	w3, 3
	beq	.L734
.L720:
	add	x0, x0, 8
	cmp	x0, x4
	beq	.L725
.L722:
	ldr	x2, [x0]
	ldrb	w3, [x2, 2]
	cbz	w5, .L735
	tbz	x3, 0, .L721
	ldr	w6, [x2, 720]
	and	w6, w6, 16776960
	cmp	w6, 65536
	beq	.L736
.L721:
	tbz	x3, 1, .L720
	ldr	w3, [x2, 760]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L720
	ldr	w1, [x2, 768]
	cmp	w19, w1
	bne	.L720
	add	x0, x2, 760
	b	.L718
	.p2align 3
.L736:
	ldr	w1, [x2, 728]
	cmp	w1, w19
	bne	.L721
	add	x0, x2, 720
	b	.L718
	.p2align 3
.L734:
	ldr	w3, [x2, 680]
	and	w3, w3, 16776960
	cmp	w3, 65536
	bne	.L720
	ldr	w1, [x2, 688]
	cmp	w19, w1
	bne	.L720
	add	x0, x2, 680
	b	.L718
	.p2align 3
.L725:
	mov	x0, 0
.L718:
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
	cbz	w1, .L738
	ldrb	w0, [x0, 1586]
	mov	w5, 2
	cmp	w0, wzr
	csinc	w5, w5, wzr, ne
.L738:
	add	x0, x20, 24576
	ldr	w2, [x0, 3724]
	cbz	w2, .L746
	sub	w2, w2, #1
	add	x0, x0, 3584
	add	x2, x2, 3521
	add	x4, x20, x2, lsl 3
	b	.L743
	.p2align 3
.L756:
	cmp	w3, 3
	beq	.L755
.L741:
	add	x0, x0, 8
	cmp	x0, x4
	beq	.L746
.L743:
	ldr	x2, [x0]
	ldrb	w3, [x2, 2]
	cbz	w5, .L756
	tbz	x3, 0, .L742
	ldr	w6, [x2, 720]
	and	w6, w6, 16776960
	cmp	w6, 256
	beq	.L757
.L742:
	tbz	x3, 1, .L741
	ldr	w3, [x2, 760]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L741
	ldr	w1, [x2, 764]
	cmp	w19, w1
	bne	.L741
	add	x0, x2, 760
	b	.L739
	.p2align 3
.L757:
	ldr	w1, [x2, 724]
	cmp	w1, w19
	bne	.L742
	add	x0, x2, 720
	b	.L739
	.p2align 3
.L755:
	ldr	w3, [x2, 680]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L741
	ldr	w1, [x2, 684]
	cmp	w19, w1
	bne	.L741
	add	x0, x2, 680
	b	.L739
	.p2align 3
.L746:
	mov	x0, 0
.L739:
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
	ble	.L759
	mov	w1, w2
	.p2align 2
.L760:
	add	x5, x19, x1, sxtw 3
	ldr	x5, [x5, -8]
	str	x5, [x19, w1, sxtw 3]
	sub	w1, w1, #1
	ldr	w5, [x21]
	cmp	w5, w1
	blt	.L760
.L759:
	add	w1, w5, 1
	str	w1, [x21]
	str	x0, [x19, w5, sxtw 3]
	ldr	w0, [x21]
	cmp	w2, w0
	mov	w4, w0
	blt	.L758
	.p2align 2
.L768:
	ldr	x1, [x19, w0, sxtw 3]
	add	w0, w0, 1
	cbz	x1, .L762
	ldrb	w5, [x1, 1]
	cbnz	w5, .L763
	ldr	w5, [x1, 8]
	cmp	w22, w5
	beq	.L762
.L763:
	str	x1, [x19, w4, sxtw 3]
	add	w4, w4, 1
.L762:
	cmp	w2, w0
	bge	.L768
.L758:
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
	ble	.L773
	mov	w1, w2
	.p2align 2
.L774:
	add	x5, x19, x1, sxtw 3
	ldr	x5, [x5, -8]
	str	x5, [x19, w1, sxtw 3]
	sub	w1, w1, #1
	ldr	w5, [x21]
	cmp	w5, w1
	blt	.L774
.L773:
	add	w1, w5, 1
	str	w1, [x21]
	str	x0, [x19, w5, sxtw 3]
	ldr	w0, [x21]
	cmp	w2, w0
	mov	w4, w0
	blt	.L772
	.p2align 2
.L784:
	ldr	x1, [x19, w0, sxtw 3]
	add	w0, w0, 1
	cbz	x1, .L776
	ldrb	w5, [x1, 1]
	cbz	w5, .L777
	ldr	w5, [x1, 4]
	cmp	w22, w5
	beq	.L776
.L777:
	str	x1, [x19, w4, sxtw 3]
	add	w4, w4, 1
.L776:
	cmp	w2, w0
	bge	.L784
.L772:
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
	cbz	w1, .L790
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
.L800:
	cmn	w21, #2
	beq	.L789
	ldr	w3, [x25]
	cmp	w3, 3
	beq	.L789
	add	w24, w21, 1
	mov	w27, w26
	lsl	x24, x24, 2
	mov	x19, 0
	b	.L793
	.p2align 3
.L794:
	ldr	x3, [x29, 104]
	ldr	w3, [x3, x19]
	bl	ReorderLTList
	cmp	x24, x19
	add	x19, x19, 4
	beq	.L789
.L799:
	ldr	w3, [x25, x19]
	cmp	w3, 3
	beq	.L789
.L793:
	cmp	w3, 1
	add	x4, x29, 124
	mov	w2, w21
	mov	x1, x22
	mov	x0, x20
	bhi	.L794
	ldr	w0, [x28, x19]
	cbnz	w3, .L795
	mvn	w0, w0
	add	w27, w27, w0
	add	w0, w27, w23
	cmp	w27, wzr
	csel	w27, w0, w27, lt
.L796:
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
	bne	.L799
.L789:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 128
	ret
	.p2align 3
.L795:
	add	w0, w0, 1
	add	w27, w27, w0
	cmp	w23, w27
	sub	w0, w27, w23
	csel	w27, w0, w27, le
	b	.L796
	.p2align 3
.L790:
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
	b	.L800
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
	beq	.L808
	ldrb	w1, [x19, 796]
	cbnz	w1, .L820
.L811:
	ldr	w1, [x19, 1640]
	cmp	w0, 1
	add	w0, w1, 1
	str	w0, [x19, 1628]
	bne	.L808
	ldrb	w0, [x19, 797]
	cbnz	w0, .L821
.L813:
	ldr	w0, [x19, 1644]
	add	w0, w0, 1
	str	w0, [x19, 1632]
.L808:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L820:
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
	b	.L811
	.p2align 3
.L821:
	add	x1, x20, 28672
	ldr	w3, [x19, 1644]
	add	x6, x19, 1456
	add	x5, x19, 1196
	add	x4, x19, 932
	add	x2, x19, 1632
	add	x1, x1, 296
	mov	x0, x20
	bl	ReorderRefPiclist
	b	.L813
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
	cbnz	w23, .L844
	cmp	w21, 1
	ldr	x23, .LC86
	beq	.L884
.L824:
	cmp	w21, 2
	bne	.L833
	mov	w21, 0
	mov	w20, 0
	.p2align 2
.L835:
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	bcs	.L833
.L885:
	cmp	w21, w22
	bcc	.L868
	b	.L840
	.p2align 3
.L836:
	add	w21, w21, 1
	cmp	w22, w21
	bls	.L840
.L868:
	ldr	x0, [x19, w21, uxtw 3]
	add	x26, x19, x21, uxtw 3
	ldrb	w1, [x0, 1]
	tbz	x1, 1, .L836
	add	x0, x0, 760
	blr	x23
	cbz	w0, .L836
	ldr	w1, [x24]
	add	w21, w21, 1
	ldr	x0, [x26]
	add	x0, x0, 760
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
.L840:
	cmp	w20, w22
	bcc	.L867
	b	.L835
	.p2align 3
.L841:
	add	w20, w20, 1
	cmp	w22, w20
	bls	.L835
.L867:
	ldr	x0, [x19, w20, uxtw 3]
	add	x26, x19, x20, uxtw 3
	ldrb	w1, [x0, 1]
	tbz	x1, 0, .L841
	add	x0, x0, 720
	blr	x23
	cbz	w0, .L841
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
	bcc	.L885
.L833:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
.L844:
	cmp	w21, 1
	ldr	x23, .LC85
	bne	.L824
.L884:
	mov	w21, 0
	mov	w20, 0
	.p2align 2
.L825:
	cmp	w20, w22
	ccmp	w21, w22, 0, cs
	bcs	.L833
	cmp	w20, w22
	bcc	.L866
	b	.L883
	.p2align 3
.L826:
	add	w20, w20, 1
	cmp	w20, w22
	bcs	.L883
.L866:
	ldr	x0, [x19, w20, uxtw 3]
	add	x26, x19, x20, uxtw 3
	ldrb	w1, [x0, 1]
	tbz	x1, 0, .L826
	add	x0, x0, 720
	blr	x23
	cbz	w0, .L826
	ldr	w1, [x24]
	add	w20, w20, 1
	ldr	x0, [x26]
	add	x0, x0, 720
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	.p2align 2
.L883:
	cmp	w21, w22
	bcs	.L825
	ldr	x0, [x19, w21, uxtw 3]
	add	x26, x19, x21, uxtw 3
	ldrb	w1, [x0, 1]
	tbnz	x1, 1, .L886
.L831:
	add	w21, w21, 1
	b	.L883
	.p2align 3
.L886:
	add	x0, x0, 760
	blr	x23
	cbz	w0, .L831
	ldr	w1, [x24]
	add	w21, w21, 1
	ldr	x0, [x26]
	add	x0, x0, 760
	str	x0, [x25, w1, uxtw 3]
	ldr	w0, [x24]
	add	w0, w0, 1
	str	w0, [x24]
	b	.L825
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
	cbnz	w0, .L888
	ldrb	w0, [x22, 115]
	add	x21, x19, 24576
	cbnz	w0, .L1008
	ldr	w0, [x21, 3720]
	cbz	w0, .L955
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 3505
	add	x0, x21, 3456
	add	x3, x19, x3, lsl 3
	b	.L894
	.p2align 3
.L893:
	cmp	x3, x0
	beq	.L892
.L894:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cmp	w2, 3
	bne	.L893
	ldr	w2, [x1, 680]
	and	w2, w2, 16776960
	cmp	w2, 65536
	bne	.L893
	add	x2, x19, x23, uxtw 3
	add	x1, x1, 680
	cmp	x3, x0
	add	w23, w23, 1
	str	x1, [x2, 28704]
	bne	.L894
	.p2align 2
.L892:
	add	x0, x19, 28672
	ldr	x3, .LC87
	mov	w1, w23
	mov	w2, 8
	add	x0, x0, 32
	bl	qsort
	str	w23, [x20, 1628]
	ldr	w1, [x21, 3724]
	cbz	w1, .L956
	sub	w3, w1, #1
	add	x0, x21, 3584
	add	x3, x3, 3521
	mov	w21, w23
	add	x3, x19, x3, lsl 3
	b	.L897
	.p2align 3
.L896:
	cmp	x0, x3
	beq	.L1009
.L897:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cmp	w2, 3
	bne	.L896
	ldr	w2, [x1, 680]
	and	w2, w2, 16776960
	cmp	w2, 256
	bne	.L896
	add	x2, x19, x21, uxtw 3
	add	x1, x1, 680
	cmp	x0, x3
	add	w21, w21, 1
	str	x1, [x2, 28704]
	bne	.L897
	.p2align 2
.L1009:
	sub	w1, w21, w23
.L895:
	uxtw	x0, w23
	ldr	x3, .LC88
	add	x0, x0, 3588
	mov	w2, 8
	add	x0, x19, x0, lsl 3
	bl	qsort
	str	w21, [x20, 1628]
	mov	w2, w21
	str	wzr, [x20, 1632]
	cbz	w2, .L1010
.L903:
	ldr	w0, [x20, 1640]
	mov	w1, 0
	str	wzr, [x20, 1632]
	add	w0, w0, 1
	cmp	w0, w2
	csel	w0, w0, w2, ls
	str	w0, [x20, 1628]
	cmp	w0, 32
	bhi	.L950
	add	x2, x19, x0, uxtw 3
	add	w0, w0, 1
	cmp	w0, 33
	str	xzr, [x2, 28704]
	bne	.L948
	.p2align 2
.L949:
	cmp	w1, 32
	bhi	.L947
	.p2align 2
.L950:
	add	x0, x19, x1, uxtw 3
	add	w1, w1, 1
	cmp	w1, 33
	str	xzr, [x0, 28968]
	bne	.L950
.L947:
	mov	w0, 0
.L999:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 480
	ret
	.p2align 3
.L888:
	ldrb	w0, [x22, 115]
	add	x21, x19, 24576
	cbz	w0, .L909
	ldr	w0, [x21, 3720]
	cbz	w0, .L1011
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 3505
	add	x0, x21, 3456
	add	x3, x19, x3, lsl 3
	.p2align 2
.L928:
	ldr	x2, [x0]
	add	x0, x0, 8
	ldrb	w1, [x2, 1]
	cbz	w1, .L927
	ldr	w4, [x2, 648]
	add	x1, x29, 480
	ldr	w5, [x22, 780]
	add	x1, x1, x23, uxtw 3
	sub	x1, x1, #4096
	cmp	w5, w4
	blt	.L927
	add	w23, w23, 1
	str	x2, [x1, 3712]
.L927:
	cmp	x0, x3
	bne	.L928
	add	x26, x29, 96
	ldr	x3, .LC95
	mov	w2, 8
	mov	w1, w23
	mov	x0, x26
	bl	qsort
	ldr	w24, [x21, 3720]
	cbz	w24, .L929
.L953:
	sub	w1, w24, #1
	add	x0, x21, 3456
	add	x1, x1, 3505
	mov	w25, w23
	add	x1, x19, x1, lsl 3
	.p2align 2
.L931:
	ldr	x3, [x0]
	add	x0, x0, 8
	ldrb	w2, [x3, 1]
	cbz	w2, .L930
	ldr	w4, [x3, 648]
	add	x2, x29, 480
	ldr	w5, [x22, 780]
	add	x2, x2, x25, uxtw 3
	sub	x2, x2, #4096
	cmp	w5, w4
	bge	.L930
	add	w25, w25, 1
	str	x3, [x2, 3712]
.L930:
	cmp	x0, x1
	bne	.L931
	sub	w24, w25, w23
	ldr	x3, .LC96
	mov	w2, 8
	mov	w1, w24
	add	x0, x26, x23, uxtw 3
	bl	qsort
	cbz	w23, .L932
.L952:
	mov	x0, 0
	.p2align 2
.L933:
	add	w1, w24, w0
	add	x3, x29, 480
	ldr	x2, [x26, x0, lsl 3]
	add	x0, x0, 1
	add	x1, x3, x1, lsl 3
	cmp	w23, w0
	str	x2, [x1, -256]
	bhi	.L933
.L932:
	cmp	w25, w23
	bls	.L934
	mov	w0, w23
	.p2align 2
.L935:
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
	bne	.L935
.L934:
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
	cbz	w28, .L936
	add	x21, x21, 3584
	mov	x0, 0
	add	x25, x29, 352
	.p2align 2
.L937:
	ldr	x1, [x21], 8
	str	x1, [x25, x0, lsl 3]
	add	x0, x0, 1
	cmp	w28, w0
	bhi	.L937
.L936:
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
.L926:
	cmp	w3, 1
	cset	w0, hi
	tst	w1, w0
	beq	.L938
	cbz	w3, .L939
	ldr	x6, [x19, 28704]
	ldr	x0, [x19, 28968]
	cmp	x6, x0
	bne	.L938
	sub	w0, w3, #1
	mov	x2, x19
	add	x0, x19, x0, lsl 3
.L941:
	cmp	x0, x2
	beq	.L943
	ldr	x5, [x2, 28712]
	add	x2, x2, 8
	ldr	x1, [x2, 28968]
	cmp	x5, x1
	beq	.L941
	.p2align 2
.L938:
	orr	w0, w3, w4
	cbz	w0, .L1012
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
	bhi	.L949
	.p2align 2
.L948:
	add	x2, x19, x0, uxtw 3
	add	w0, w0, 1
	cmp	w0, 33
	str	xzr, [x2, 28704]
	bne	.L948
	b	.L949
	.p2align 3
.L909:
	ldr	w0, [x21, 3720]
	cbz	w0, .L957
	sub	w3, w0, #1
	mov	w23, 0
	add	x3, x3, 3505
	add	x0, x21, 3456
	add	x3, x19, x3, lsl 3
	b	.L914
	.p2align 3
.L913:
	cmp	x0, x3
	beq	.L912
.L914:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 2]
	cmp	w2, 3
	bne	.L913
	ldr	w2, [x1, 692]
	add	x5, x19, x23, uxtw 3
	ldr	w4, [x22, 784]
	add	x1, x1, 680
	cmp	w4, w2
	blt	.L913
	str	x1, [x5, 28704]
	cmp	x0, x3
	add	w23, w23, 1
	bne	.L914
	.p2align 2
.L912:
	add	x25, x19, 28672
	ldr	x3, .LC93
	mov	w2, 8
	mov	w1, w23
	add	x0, x25, 32
	bl	qsort
	ldr	w24, [x21, 3720]
	cbz	w24, .L915
	sub	w2, w24, #1
	add	x0, x21, 3456
	add	x2, x2, 3505
	mov	w26, w23
	add	x2, x19, x2, lsl 3
	b	.L917
	.p2align 3
.L916:
	cmp	x0, x2
	beq	.L1013
.L917:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w3, [x1, 2]
	cmp	w3, 3
	bne	.L916
	ldr	w3, [x1, 692]
	add	x5, x19, x26, uxtw 3
	ldr	w4, [x22, 784]
	add	x1, x1, 680
	cmp	w4, w3
	bge	.L916
	str	x1, [x5, 28704]
	cmp	x0, x2
	add	w26, w26, 1
	bne	.L917
	.p2align 2
.L1013:
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
	cbz	w23, .L919
.L951:
	add	x25, x25, 32
	mov	w0, 0
	.p2align 2
.L920:
	add	w1, w24, w0
	ldr	x2, [x25], 8
	add	x1, x1, 3621
	add	w0, w0, 1
	cmp	w0, w23
	str	x2, [x19, x1, lsl 3]
	bcc	.L920
.L919:
	cmp	w26, w23
	bls	.L921
	mov	w0, w23
	.p2align 2
.L922:
	sub	w1, w0, w23
	add	x2, x19, x0, uxtw 3
	add	x1, x1, 3621
	add	w0, w0, 1
	cmp	w0, w26
	ldr	x2, [x2, 28704]
	str	x2, [x19, x1, lsl 3]
	bne	.L922
.L921:
	str	w26, [x20, 1628]
	str	w26, [x20, 1632]
	ldr	w1, [x21, 3724]
	cbz	w1, .L958
	sub	w3, w1, #1
	add	x0, x21, 3584
	add	x3, x3, 3521
	mov	w21, w26
	add	x3, x19, x3, lsl 3
	b	.L925
	.p2align 3
.L924:
	cmp	x3, x0
	beq	.L1014
.L925:
	ldr	x1, [x0]
	add	x0, x0, 8
	ldrb	w2, [x1, 1]
	cmp	w2, 3
	bne	.L924
	ldrb	w2, [x1, 681]
	cmp	w2, 1
	bne	.L924
	add	x2, x19, x21, uxtw 3
	add	x1, x1, 680
	cmp	x3, x0
	add	w21, w21, 1
	str	x1, [x2, 28704]
	str	x1, [x2, 28968]
	bne	.L925
	.p2align 2
.L1014:
	sub	w1, w21, w26
.L923:
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
	b	.L926
	.p2align 3
.L1008:
	ldr	w0, [x21, 3720]
	mov	w23, 0
	cbz	w0, .L891
	sub	w4, w0, #1
	mov	w23, 0
	add	x4, x4, 3505
	add	x0, x21, 3456
	add	x4, x19, x4, lsl 3
	.p2align 2
.L900:
	ldr	x2, [x0]
	add	x1, x29, 480
	add	x1, x1, x23, uxtw 3
	add	x0, x0, 8
	sub	x1, x1, #4096
	cmp	x4, x0
	ldrb	w3, [x2, 2]
	cbz	w3, .L899
	add	w23, w23, 1
	str	x2, [x1, 3712]
.L899:
	bne	.L900
.L891:
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
	cbz	w23, .L901
	add	x21, x21, 3584
	mov	x0, 0
	add	x25, x29, 352
	.p2align 2
.L902:
	ldr	x1, [x21], 8
	str	x1, [x25, x0, lsl 3]
	add	x0, x0, 1
	cmp	w23, w0
	bhi	.L902
.L901:
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
	cbnz	w2, .L903
.L1010:
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, 2
	bne	.L904
	ldrb	w0, [x22, 115]
	cbz	w0, .L1015
	cmp	w0, 1
	ldr	x0, [x22, 752]
	beq	.L1016
	add	x0, x0, 760
	mov	w2, 1
	str	x0, [x19, 28704]
	b	.L903
	.p2align 3
.L1015:
	ldr	x0, [x22, 752]
	mov	w2, 1
	add	x0, x0, 680
	str	x0, [x19, 28704]
	b	.L903
.L939:
	ldr	x6, [x19, 28968]
.L943:
	ldr	x0, [x19, 28976]
	str	x0, [x19, 28968]
	str	x6, [x19, 28976]
	b	.L938
.L1016:
	add	x0, x0, 720
	mov	w2, 1
	str	x0, [x19, 28704]
	b	.L903
.L1011:
	add	x26, x29, 96
	ldr	x3, .LC95
	mov	w2, 8
	mov	w1, 0
	mov	x0, x26
	mov	w23, 0
	bl	qsort
	ldr	w24, [x21, 3720]
	cbnz	w24, .L953
	ldr	x3, .LC96
	mov	w2, 8
	mov	w1, 0
	mov	x0, x26
	mov	w25, 0
	bl	qsort
	b	.L934
	.p2align 3
.L957:
	mov	w23, 0
	b	.L912
.L915:
	uxtw	x22, w23
	ldr	x3, .LC94
	add	x22, x22, 3588
	mov	w2, 8
	mov	w1, 0
	mov	w26, w23
	add	x22, x19, x22, lsl 3
	mov	x0, x22
	bl	qsort
	cbnz	w23, .L951
	b	.L921
	.p2align 3
.L958:
	mov	w21, w26
	b	.L923
.L956:
	mov	w21, w23
	b	.L895
.L955:
	mov	w23, 0
	b	.L892
.L929:
	ldr	x3, .LC96
	mov	w2, 8
	mov	w1, 0
	add	x0, x26, x23, uxtw 3
	mov	w25, w23
	bl	qsort
	cbnz	w23, .L952
	b	.L934
	.p2align 3
.L904:
	ldr	x1, .LC92
.L1004:
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x19
	bl	ClearCurrSlice
	mov	w0, -1
	b	.L999
.L1012:
	ldr	x1, .LC98
	b	.L1004
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
	tbz	x0, 13, .L1017
	mov	x0, x19
	bl	DumpList.part.5
.L1017:
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
	cbz	w0, .L1026
	add	x1, x19, 61440
	ldr	x3, [x19, 27904]
	cmp	w0, 1
	ldr	w7, [x1, 780]
	ldr	w1, [x3, 648]
	sub	w1, w7, w1
	sxtw	x1, w1
	eor	x6, x1, x1, asr 63
	sub	x6, x6, x1, asr 63
	bls	.L1027
	sub	w0, w0, #2
	add	x2, x2, 3336
	add	x1, x0, 3490
	mov	w0, 0
	add	x5, x19, x1, lsl 3
	.p2align 2
.L1025:
	ldr	x4, [x2]
	add	x2, x2, 8
	ldr	w1, [x4, 648]
	sub	w1, w7, w1
	sxtw	x1, w1
	eor	x3, x1, x1, asr 63
	sub	x3, x3, x1, asr 63
	cmp	w3, w6
	bge	.L1024
	ldr	w0, [x4, 164]
	mov	w6, w3
.L1024:
	cmp	x2, x5
	bne	.L1025
.L1023:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1027:
	mov	w0, 0
	b	.L1023
.L1026:
	mov	w0, -1
	b	.L1023
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
	beq	.L1029
	add	x1, x19, 61440
	ldrb	w1, [x1, 115]
	cbnz	w1, .L1059
	add	x6, x19, 24576
	ldr	w5, [x0, 1628]
	add	x6, x6, 4000
	mov	w4, 0
	add	x7, x19, 28672
	.p2align 2
.L1033:
	cbz	w5, .L1037
	add	x1, x7, 32
	mov	w3, 32
	mov	w0, 0
	b	.L1035
	.p2align 3
.L1034:
	add	w0, w0, 1
	cmp	w0, w5
	beq	.L1060
.L1035:
	ldr	x2, [x1]
	add	x1, x1, 8
	ldr	x2, [x2, 32]
	ldr	w2, [x2, 672]
	cmp	w2, w4
	bne	.L1034
	cmp	w3, w0
	csel	w3, w3, w0, ls
	add	w0, w0, 1
	stp	w3, w3, [x6]
	cmp	w0, w5
	bne	.L1035
.L1060:
	cmp	w3, 31
	bhi	.L1037
.L1036:
	add	w4, w4, 1
	add	x6, x6, 8
	cmp	w4, 16
	bne	.L1033
.L1029:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L1059:
	ldr	w6, [x0, 1628]
	add	x7, x19, 24576
	add	x7, x7, 4000
	mov	w4, 0
	add	x8, x19, 28672
	cbz	w6, .L1041
	.p2align 2
.L1062:
	add	x2, x8, 32
	mov	w5, 32
	mov	w1, 0
	b	.L1039
	.p2align 3
.L1038:
	add	w1, w1, 1
	cmp	w1, w6
	beq	.L1061
.L1039:
	ldr	x0, [x2]
	add	x2, x2, 8
	ldr	x3, [x0, 32]
	ldrb	w0, [x0]
	cmp	w0, 2
	ldr	w0, [x3, 672]
	cset	w3, eq
	orr	w0, w3, w0, lsl 1
	cmp	w0, w4
	bne	.L1038
	cmp	w5, w1
	csel	w5, w5, w1, ls
	add	w1, w1, 1
	str	w5, [x7]
	cmp	w1, w6
	bne	.L1039
.L1061:
	cmp	w5, 31
	bhi	.L1041
.L1040:
	add	w4, w4, 1
	add	x7, x7, 4
	cmp	w4, 32
	beq	.L1029
	cbnz	w6, .L1062
	.p2align 2
.L1041:
	str	wzr, [x7]
	b	.L1040
	.p2align 3
.L1037:
	stp	wzr, wzr, [x6]
	b	.L1036
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
	cbz	w0, .L1064
	add	x2, x19, 61440
	ldr	x3, [x19, 27904]
	cmp	w0, 1
	ldr	w6, [x2, 780]
	ldr	w2, [x3, 648]
	sub	w2, w6, w2
	sxtw	x2, w2
	eor	x5, x2, x2, asr 63
	sub	x5, x5, x2, asr 63
	bls	.L1073
	sub	w4, w0, #2
	add	x1, x1, 3336
	add	x4, x4, 3490
	mov	w7, 0
	add	x4, x19, x4, lsl 3
	.p2align 2
.L1066:
	ldr	x3, [x1]
	add	x1, x1, 8
	ldr	w0, [x3, 648]
	sub	w0, w6, w0
	sxtw	x0, w0
	eor	x2, x0, x0, asr 63
	sub	x2, x2, x0, asr 63
	cmp	w2, w5
	bge	.L1065
	ldr	w7, [x3, 164]
	mov	w5, w2
.L1065:
	cmp	x1, x4
	bne	.L1066
.L1064:
	add	x20, x19, 20480
	mov	w21, -1
	ldrb	w0, [x20, 1584]
	str	w21, [x20, 3212]
	cmp	w0, 2
	str	w7, [x20, 3216]
	beq	.L1083
	mov	x0, x19
	bl	InitListX
	cbnz	w0, .L1084
	ldr	x1, .LC103
	mov	w0, 13
	bl	dprint_vfmw
	ldr	x22, .LC104
	ldr	w0, [x22]
	tbnz	x0, 13, .L1085
.L1070:
	mov	x0, x19
	bl	ReorderListX
	mov	x0, x19
	bl	RepairList
	cbnz	w0, .L1074
	mov	x0, x19
	bl	FindMinRefIdx
	ldr	x0, [x19, 28704]
	cbz	x0, .L1075
	ldr	x0, [x0, 32]
	ldr	w0, [x0, 164]
.L1071:
	ldr	x1, .LC106
	mov	w21, 0
	str	w0, [x20, 3212]
	mov	w0, 13
	bl	dprint_vfmw
	ldr	w0, [x22]
	tbz	x0, 13, .L1068
	mov	x0, x19
	bl	DumpList.part.5
.L1068:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1083:
	str	wzr, [x20, 1628]
	mov	w21, 0
	str	wzr, [x20, 1632]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1085:
	mov	x0, x19
	bl	DumpList.part.5
	b	.L1070
	.p2align 3
.L1075:
	mov	w0, -1
	b	.L1071
	.p2align 3
.L1074:
	mov	w21, -1
	b	.L1068
.L1073:
	mov	w7, 0
	b	.L1064
.L1084:
	ldr	x1, .LC101
	mov	w0, 13
	bl	dprint_vfmw
	b	.L1068
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
	cbz	w0, .L1093
	sub	w2, w0, #1
	mov	w4, 808
	mov	x3, 30044
	add	x0, x19, 28672
	add	x0, x0, 564
	umaddl	x3, w2, w4, x3
	add	x3, x19, x3
	b	.L1092
	.p2align 3
.L1100:
	ldrb	w2, [x0]
	cmp	w2, 1
	beq	.L1090
	add	x0, x0, 808
	cmp	x0, x3
	beq	.L1093
.L1092:
	ldr	w2, [x0, 648]
	cmp	w2, 1
	bne	.L1100
.L1090:
	strb	wzr, [x0, -3]
	strb	wzr, [x0, -2]
	add	x0, x0, 808
	cmp	x0, x3
	bne	.L1092
.L1093:
	ldr	w0, [x1, 2776]
	cbz	w0, .L1086
	sub	w0, w0, #1
	add	x1, x1, 2704
	add	x0, x0, 12288
	add	x0, x0, 3749
	add	x0, x19, x0, lsl 2
	.p2align 2
.L1094:
	str	wzr, [x1], 4
	cmp	x1, x0
	bne	.L1094
.L1086:
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
	cbz	w0, .L1118
.L1102:
	add	x24, x25, 61440
	mov	w23, 0
	mov	w21, 808
	ldr	w0, [x24, 2772]
	cbz	w0, .L1109
	.p2align 2
.L1113:
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
.L1107:
	ldr	x1, [x19]
	cbnz	x1, .L1119
.L1106:
	add	x19, x19, 8
	cmp	x19, x22
	bne	.L1107
	ldr	w0, [x24, 2772]
	add	w23, w23, 1
	cmp	w0, w23
	bhi	.L1113
.L1109:
	ldr	w0, [x24, 2776]
	cbz	w0, .L1101
	sub	w1, w0, #1
	add	x0, x24, 2704
	add	x1, x1, 12288
	add	x1, x1, 3749
	add	x25, x25, x1, lsl 2
	.p2align 2
.L1110:
	str	wzr, [x0], 4
	cmp	x0, x25
	bne	.L1110
.L1101:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1119:
	ldr	w0, [x20, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L1106
.L1118:
	add	x0, x25, 8
	bl	ResetVoQueue
	b	.L1102
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
	bne	.L1122
	add	x6, x21, 24576
	ldr	w5, [x19, 3272]
	mov	w2, 0
	ldr	w6, [x6, 3712]
	add	w6, w6, 1
	cmp	w5, w6
	bls	.L1123
.L1122:
	ldr	x2, .LC107
	ldr	x6, [x2]
	cbz	x6, .L1126
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
.L1126:
	ldr	w5, [x19, 3272]
	mov	w2, 1
.L1123:
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
	beq	.L1129
	cbz	w3, .L1130
	cmp	w3, 2
	bne	.L1127
	cmp	w7, 5
	beq	.L1184
	ldr	w1, [x0, 728]
	cbz	w1, .L1163
	ldr	w1, [x0, 768]
	mov	w3, 0
	mov	w2, 0
	str	wzr, [x0, 764]
	str	wzr, [x0, 776]
.L1164:
	str	w2, [x0, 772]
.L1166:
	ldr	w4, [x0, 1616]
	add	w2, w1, w2
	str	w2, [x0, 760]
	lsl	w1, w2, 1
	cbnz	w4, .L1167
	sub	w1, w1, #1
	str	w1, [x0, 748]
.L1168:
	ldrb	w2, [x0, 1585]
	cbnz	w2, .L1169
	str	w1, [x0, 736]
	str	w1, [x0, 740]
	str	w1, [x0, 744]
.L1162:
	ldr	w1, [x0, 768]
	str	w1, [x0, 764]
	str	w3, [x0, 776]
.L1127:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1130:
	ldr	w1, [x1, 2204]
	cmp	w7, 5
	add	w1, w1, 4
	lsl	w4, w4, w1
	beq	.L1136
	ldr	w1, [x0, 728]
	cbnz	w1, .L1134
	ldr	w5, [x0, 712]
	ldr	w3, [x0, 716]
	mov	w2, w5
.L1135:
	ldr	w1, [x0, 704]
	cmp	w1, w3
	bcs	.L1133
	sub	w6, w3, w1
	cmp	w6, w4, lsr 1
	bcc	.L1133
	add	w2, w2, w4
	str	w2, [x0, 720]
.L1137:
	ldrb	w3, [x0, 1585]
	cbnz	w3, .L1139
.L1187:
	ldr	w3, [x0, 708]
	add	w1, w1, w2
	str	w1, [x0, 736]
	add	w2, w1, w3
	str	w2, [x0, 740]
	cmp	w1, w2
	csel	w1, w1, w2, le
	str	w1, [x0, 748]
.L1140:
	ldr	w3, [x0, 764]
	ldr	w2, [x0, 768]
	str	w1, [x0, 744]
	cmp	w2, w3
	beq	.L1142
	str	w2, [x0, 764]
.L1142:
	ldr	w1, [x0, 1616]
	cbz	w1, .L1127
	ldr	w2, [x0, 704]
	ldr	w1, [x0, 720]
	str	w1, [x0, 712]
	str	w2, [x0, 716]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1129:
	cmp	w7, 5
	beq	.L1185
	ldr	w3, [x0, 728]
	cbz	w3, .L1145
	mov	w2, 0
	str	wzr, [x0, 764]
	str	wzr, [x0, 776]
.L1146:
	str	w2, [x0, 772]
.L1144:
	ldr	w2, [x1, 2216]
	cbz	w2, .L1148
	ldr	w2, [x0, 768]
	ldr	w5, [x0, 772]
	ldr	w6, [x0, 1616]
	add	w5, w5, w2
	str	w5, [x0, 760]
	cbnz	w6, .L1150
	cbz	w5, .L1151
	sub	w5, w5, #1
	str	w5, [x0, 760]
.L1150:
	str	wzr, [x0, 792]
	ldr	w2, [x1, 2216]
	cmp	w2, wzr
	ble	.L1152
.L1171:
	mov	w4, 0
	mov	w3, 0
	.p2align 2
.L1153:
	add	x2, x1, x3, sxtw 2
	add	w3, w3, 1
	ldr	w2, [x2, 2220]
	add	w4, w4, w2
	str	w4, [x0, 792]
	ldr	w2, [x1, 2216]
	cmp	w2, w3
	bgt	.L1153
.L1152:
	cbz	w5, .L1154
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
	tbnz	w5, #31, .L1155
	mov	w3, 0
	.p2align 2
.L1156:
	add	x4, x1, x3, sxtw 2
	add	w3, w3, 1
	cmp	w5, w3
	ldr	w4, [x4, 2220]
	add	w2, w2, w4
	str	w2, [x0, 788]
	bge	.L1156
.L1155:
	cbnz	w6, .L1157
	ldr	w3, [x1, 2208]
	add	w2, w2, w3
	str	w2, [x0, 788]
.L1157:
	ldrb	w3, [x0, 1585]
	cbnz	w3, .L1158
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
.L1159:
	ldr	w3, [x0, 768]
	ldr	w1, [x0, 772]
	str	w2, [x0, 744]
	str	w3, [x0, 764]
	str	w1, [x0, 776]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1134:
	ldr	w5, [x0, 732]
	cbz	w5, .L1186
.L1136:
	ldr	w1, [x0, 704]
	mov	w2, 0
	mov	w5, 0
	str	wzr, [x0, 712]
	str	wzr, [x0, 716]
.L1133:
	cmp	w1, w3
	bls	.L1138
	sub	w3, w1, w3
	cmp	w3, w4, lsr 1
	bls	.L1138
	sub	w2, w2, w4
	str	w2, [x0, 720]
	b	.L1137
	.p2align 3
.L1184:
	add	x1, x0, 512
	mov	w3, 0
	stp	wzr, wzr, [x1, 224]
	stp	wzr, wzr, [x1, 232]
	str	wzr, [x0, 772]
	b	.L1162
	.p2align 3
.L1185:
	str	wzr, [x0, 772]
	b	.L1144
	.p2align 3
.L1138:
	ldrb	w3, [x0, 1585]
	str	w5, [x0, 720]
	cbz	w3, .L1187
.L1139:
	ldrb	w3, [x0, 1586]
	add	w1, w1, w2
	cbnz	w3, .L1141
	str	w1, [x0, 736]
	str	w1, [x0, 748]
	b	.L1140
	.p2align 3
.L1148:
	ldr	w6, [x0, 1616]
	str	wzr, [x0, 760]
.L1151:
	str	wzr, [x0, 792]
	mov	w5, 0
	ldr	w2, [x1, 2216]
	cmp	w2, wzr
	bgt	.L1171
	.p2align 2
.L1154:
	mov	w2, 0
	str	wzr, [x0, 788]
	b	.L1155
	.p2align 3
.L1158:
	ldrb	w3, [x0, 1586]
	cbnz	w3, .L1160
	ldr	w1, [x0, 752]
	add	w2, w2, w1
	str	w2, [x0, 736]
	str	w2, [x0, 748]
	b	.L1159
	.p2align 3
.L1163:
	ldr	w3, [x0, 764]
	ldr	w1, [x0, 768]
	cmp	w1, w3
	ldr	w3, [x0, 776]
	bcs	.L1165
	add	w2, w2, w3
	str	w2, [x0, 772]
	mov	w3, w2
	b	.L1166
	.p2align 3
.L1145:
	ldr	w3, [x0, 764]
	ldr	w4, [x0, 768]
	cmp	w4, w3
	bcs	.L1147
	ldr	w3, [x0, 776]
	add	w2, w3, w2
	str	w2, [x0, 772]
	b	.L1144
	.p2align 3
.L1169:
	ldrb	w2, [x0, 1586]
	str	w1, [x0, 744]
	cbnz	w2, .L1170
	str	w1, [x0, 736]
	b	.L1162
	.p2align 3
.L1167:
	str	w1, [x0, 748]
	b	.L1168
	.p2align 3
.L1141:
	str	w1, [x0, 740]
	str	w1, [x0, 748]
	b	.L1140
	.p2align 3
.L1160:
	ldr	w3, [x1, 2212]
	ldr	w1, [x0, 752]
	add	w2, w2, w3
	add	w2, w2, w1
	str	w2, [x0, 740]
	str	w2, [x0, 748]
	b	.L1159
	.p2align 3
.L1186:
	ldr	w3, [x0, 736]
	mov	w2, 0
	str	wzr, [x0, 712]
	str	w3, [x0, 716]
	b	.L1135
	.p2align 3
.L1170:
	str	w1, [x0, 740]
	b	.L1162
.L1147:
	ldr	w2, [x0, 776]
	b	.L1146
.L1165:
	mov	w2, w3
	b	.L1164
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
	cbz	w2, .L1189
	cmp	w2, 1
	mov	w1, 0
	cset	w8, eq
	cmp	w2, 2
	ldr	w2, [x0, 3720]
	cset	w9, eq
	cbz	w2, .L1209
	.p2align 2
.L1226:
	add	x3, x19, x1, uxtw 3
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	cbz	w4, .L1203
	ldr	w4, [x2, 636]
	ldr	w5, [x6, 768]
	cmp	w4, w5
	bls	.L1204
	sub	w4, w4, w7
	str	w4, [x2, 640]
.L1205:
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	tbz	x4, 0, .L1206
	ldr	w5, [x2, 720]
	and	w5, w5, 16776960
	cmp	w5, 65536
	beq	.L1236
.L1206:
	tbz	x4, 1, .L1203
	ldr	w3, [x2, 760]
	and	w3, w3, 16776960
	cmp	w3, 65536
	beq	.L1237
	.p2align 2
.L1203:
	ldr	w2, [x0, 3720]
	add	w1, w1, 1
	cmp	w2, w1
	bhi	.L1226
.L1209:
	ldr	w2, [x0, 3724]
	mov	w1, 0
	cbz	w2, .L1188
	.p2align 2
.L1225:
	add	x4, x19, x1, uxtw 3
	add	w1, w1, 1
	ldr	x3, [x4, 28160]
	ldrb	w2, [x3, 2]
	cbz	w2, .L1211
	tbz	x2, 0, .L1212
	ldr	w5, [x3, 720]
	and	w5, w5, 16776960
	cmp	w5, 256
	beq	.L1238
.L1212:
	tbz	x2, 1, .L1211
	ldr	w2, [x3, 760]
	and	w2, w2, 16776960
	cmp	w2, 256
	beq	.L1239
	.p2align 2
.L1211:
	ldr	w2, [x0, 3724]
	cmp	w2, w1
	bhi	.L1225
.L1188:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1204:
	str	w4, [x2, 640]
	b	.L1205
	.p2align 3
.L1189:
	ldr	w2, [x0, 3720]
	mov	w1, 0
	cbnz	w2, .L1224
	.p2align 2
.L1199:
	ldr	w2, [x0, 3724]
	mov	w1, 0
	cbz	w2, .L1188
	add	x2, x19, x1, uxtw 3
	add	w1, w1, 1
	ldr	x2, [x2, 28160]
	ldrb	w3, [x2, 2]
	cmp	w3, 3
	beq	.L1240
	.p2align 2
.L1200:
	ldr	w2, [x0, 3724]
	cmp	w2, w1
	bls	.L1188
	add	x2, x19, x1, uxtw 3
	add	w1, w1, 1
	ldr	x2, [x2, 28160]
	ldrb	w3, [x2, 2]
	cmp	w3, 3
	bne	.L1200
.L1240:
	ldr	w3, [x2, 680]
	and	w3, w3, 16776960
	cmp	w3, 256
	bne	.L1200
	ldr	w3, [x2, 644]
	str	w3, [x2, 684]
	b	.L1200
	.p2align 3
.L1242:
	ldr	w4, [x2, 680]
	and	w4, w4, 16776960
	cmp	w4, 65536
	bne	.L1195
	ldr	w4, [x2, 636]
	ldr	w5, [x6, 768]
	cmp	w4, w5
	sub	w5, w4, w7
	bhi	.L1241
	str	w4, [x2, 640]
.L1197:
	ldr	x2, [x3, 28032]
	ldr	w3, [x2, 640]
	str	w3, [x2, 688]
	.p2align 2
.L1195:
	ldr	w2, [x0, 3720]
	add	w1, w1, 1
	cmp	w2, w1
	bls	.L1199
.L1224:
	add	x3, x19, x1, uxtw 3
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	cmp	w4, 3
	bne	.L1195
	b	.L1242
	.p2align 3
.L1238:
	ldr	w2, [x3, 644]
	add	w2, w8, w2, lsl 1
	str	w2, [x3, 724]
	ldr	x3, [x4, 28160]
	ldrb	w2, [x3, 2]
	b	.L1212
	.p2align 3
.L1236:
	ldr	w4, [x2, 640]
	add	w4, w8, w4, lsl 1
	str	w4, [x2, 728]
	ldr	x2, [x3, 28032]
	ldrb	w4, [x2, 2]
	b	.L1206
	.p2align 3
.L1239:
	ldr	w2, [x3, 644]
	add	w2, w9, w2, lsl 1
	str	w2, [x3, 764]
	b	.L1211
	.p2align 3
.L1237:
	ldr	w3, [x2, 640]
	add	w3, w9, w3, lsl 1
	str	w3, [x2, 768]
	b	.L1203
	.p2align 3
.L1241:
	str	w5, [x2, 640]
	b	.L1197
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
	cbz	x20, .L1248
	add	x0, x19, 24576
	ldr	w3, [x0, 3716]
	cbz	w3, .L1248
	ldr	x0, [x19, 27904]
	cmp	x20, x0
	beq	.L1250
	sub	w3, w3, #1
	mov	x0, x19
	add	x3, x19, x3, lsl 3
	b	.L1245
	.p2align 3
.L1246:
	ldr	x2, [x0, 27904]
	cmp	x20, x2
	beq	.L1250
.L1245:
	cmp	x0, x3
	add	x0, x0, 8
	bne	.L1246
.L1248:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1250:
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
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	add	x19, x21, 61440
	add	x20, x21, 65536
	bl	_mcount
	ldr	x1, .LC113
	mov	w0, 2
	ldr	w4, [x19, 284]
	ldrb	w3, [x19, 115]
	ldr	w2, [x19, 776]
	ldr	w5, [x19, 828]
	bl	dprint_vfmw
	str	wzr, [x20, 40]
	ldr	w4, [x20, 128]
	str	wzr, [x20, 140]
	cmp	w4, 1
	beq	.L1255
	add	x2, x21, 16384
	add	x0, x21, 12288
	add	x2, x2, 2592
	add	x0, x0, 2688
	add	x3, x21, 20480
.L1256:
	ldr	x1, [x19, 728]
	cmp	w4, 1
	str	x1, [x19, 2848]
	ldr	w1, [x19, 272]
	ldr	x6, [x20, 32]
	ldr	w5, [x19, 276]
	str	w1, [x19, 3632]
	ldr	w4, [x19, 776]
	ldrb	w1, [x19, 115]
	ldr	w7, [x19, 268]
	str	w5, [x19, 3636]
	strb	w1, [x19, 2840]
	str	w4, [x19, 2856]
	str	w7, [x19, 3628]
	str	x6, [x19, 3848]
	ldr	w1, [x19, 816]
	ldrb	w4, [x0, 19]
	ldr	w5, [x19, 812]
	str	w5, [x19, 2860]
	str	w1, [x19, 2864]
	str	w4, [x19, 2872]
	ldr	w1, [x0, 3248]
	add	w1, w1, 1
	str	w1, [x19, 2868]
	ldrb	w1, [x0, 18]
	strb	w1, [x19, 2843]
	ldr	w1, [x2, 60]
	str	w1, [x19, 2876]
	ldrb	w1, [x2]
	str	w1, [x19, 2880]
	ldr	w0, [x0, 40]
	strb	w0, [x19, 2841]
	ldrb	w0, [x2, 4]
	str	w0, [x19, 2884]
	ldr	w0, [x3, 748]
	str	w0, [x19, 2888]
	ldr	w0, [x3, 736]
	str	w0, [x19, 2892]
	ldr	w0, [x3, 740]
	str	w0, [x19, 2896]
	beq	.L1272
	ldr	x1, [x19, 752]
	ldr	w0, [x20, 116]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	mov	x23, x0
	cbz	x0, .L1291
	ldr	x2, [x0, 640]
	ldr	x3, [x0, 648]
	cbz	x2, .L1261
	cbz	x3, .L1261
	ldr	w0, [x20, 116]
	add	x3, x19, 3276
	add	x2, x19, 3148
	add	x1, x19, 3536
	add	x22, x21, 24576
	bl	FSP_GetDecFsAddrTab
	ldr	x0, [x23, 640]
	add	x2, x19, 3540
	add	x1, x19, 3620
	ldr	w0, [x0, 8]
	str	w0, [x19, 3132]
	ldr	w0, [x20, 116]
	bl	FSP_GetPmvAddrTab
	ldr	w0, [x19, 828]
	str	w0, [x19, 3140]
	ldr	w0, [x23, 668]
	str	w0, [x19, 3624]
	ldr	x0, [x23, 648]
	ldr	w0, [x0, 16]
	str	w0, [x19, 3136]
	ldr	x0, [x23, 648]
	ldr	w0, [x0, 76]
	str	w0, [x19, 3144]
	ldr	w4, [x22, 3992]
	str	w4, [x19, 3840]
	cbz	w4, .L1267
	sub	w3, w4, #1
	add	x0, x22, 3800
	add	x3, x3, 4096
	add	x1, x19, 3648
	add	x3, x3, 2999
	add	x3, x21, x3, lsl 2
	.p2align 2
.L1266:
	ldr	w2, [x0]
	add	x0, x0, 4
	str	w2, [x1]
	add	x1, x1, 4
	cmp	x3, x0
	ldr	w2, [x0, 60]
	str	w2, [x1, 60]
	ldr	w2, [x0, 124]
	str	w2, [x1, 124]
	bne	.L1266
.L1267:
	add	x0, x21, 12288
	ldrb	w0, [x0, 2713]
	cbz	w0, .L1264
	add	x0, x21, 16384
	ldrb	w0, [x0, 2610]
.L1268:
	cmp	w0, 1
	add	x5, x19, 2900
	mov	w1, 0
	beq	.L1274
	.p2align 2
.L1277:
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
	bne	.L1277
	add	x19, x19, 2996
	mov	w9, 1
	mov	w5, 0
	.p2align 2
.L1278:
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
	bne	.L1278
.L1276:
	mov	w23, 0
	cbz	w4, .L1272
	.p2align 2
.L1280:
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
	bhi	.L1280
.L1272:
	mov	w0, 0
.L1258:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1274:
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
	bne	.L1274
	add	x19, x19, 2996
	mov	w9, 1
	mov	w5, 0
	.p2align 2
.L1275:
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
	bne	.L1275
	b	.L1276
	.p2align 3
.L1264:
	add	x0, x21, 16384
	ldrb	w0, [x0, 2610]
	cbnz	w0, .L1268
	ldr	x8, .LC118
	add	x5, x19, 2900
	mov	w1, 0
	.p2align 2
.L1269:
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
	bne	.L1269
	add	x19, x19, 2996
	mov	w5, 0
	.p2align 2
.L1270:
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
	bne	.L1270
	b	.L1276
	.p2align 3
.L1255:
	add	x3, x21, 20480
	mov	w2, 2208
	ldr	x6, [x21, 14968]
	mov	w0, 4000
	ldr	x5, [x21, 14960]
	ldr	w1, [x3, 1596]
	umull	x1, w1, w2
	add	x2, x6, x1
	ldr	w1, [x2, 28]
	umull	x0, w1, w0
	add	x0, x5, x0
	b	.L1256
.L1261:
	ldr	x1, .LC117
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1258
.L1291:
	ldr	x2, [x19, 752]
	mov	w0, 0
	ldr	x1, .LC115
	ldr	w2, [x2, 632]
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1258
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
	add	x20, x22, 65536
	bl	_mcount
	ldr	w0, [x20, 128]
	ldr	w7, [x20, 40]
	cmp	w0, 1
	beq	.L1294
	add	x4, x22, 16384
	add	x6, x22, 12288
	add	x4, x4, 2592
	add	x6, x6, 2688
	add	x27, x22, 20480
.L1295:
	add	w0, w7, 1
	str	w0, [x20, 40]
	uxtw	x21, w7
	ldr	x8, [x20, 32]
	ldr	w1, [x22, 14940]
	lsl	x5, x21, 7
	sub	x5, x5, x21
	sub	w1, w1, #1
	lsl	x5, x5, 5
	cmp	w7, w1
	add	x19, x8, x5
	bcs	.L1296
	lsl	x1, x0, 7
	sub	x0, x1, x0
	add	x0, x8, x0, lsl 5
	str	x0, [x19, 4056]
.L1297:
	ldr	x3, [x22, 632]
	ldrb	w2, [x3]
	ldr	w0, [x3, 76]
	cmp	w2, 1
	ccmp	w2, w0, 2, ls
	bcs	.L1298
	.p2align 2
.L1299:
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
	bls	.L1299
.L1298:
	cmp	w0, 1
	bhi	.L1304
	.p2align 2
.L1377:
	add	x1, x19, x0, uxtw 2
	add	w0, w0, 1
	cmp	w0, 2
	str	wzr, [x1, 8]
	str	wzr, [x1, 16]
	str	wzr, [x1, 24]
	bne	.L1377
.L1304:
	add	x24, x22, 61440
	ldrb	w0, [x24, 121]
	strb	w0, [x19, 1]
	ldr	w0, [x4, 48]
	ldr	w1, [x27, 3196]
	add	w0, w0, 26
	add	w0, w0, w1
	str	w0, [x19, 32]
	ldr	w1, [x27, 3192]
	str	w1, [x19, 36]
	ldr	w1, [x27, 1644]
	str	w1, [x19, 40]
	ldr	w1, [x27, 1640]
	str	w1, [x19, 44]
	ldrb	w1, [x27, 1584]
	strb	w1, [x8, x5]
	ldr	w1, [x27, 1636]
	str	w1, [x19, 48]
	ldrb	w1, [x27, 1590]
	strb	w1, [x19, 2]
	ldrb	w1, [x6, 20]
	strb	w1, [x19, 3]
	ldr	w1, [x27, 1628]
	str	w1, [x19, 52]
	ldr	w1, [x27, 1632]
	str	w1, [x19, 56]
	ldrb	w1, [x27, 1584]
	cmp	w1, 1
	beq	.L1403
	cbz	w1, .L1404
	strb	wzr, [x19, 4]
.L1305:
	ldr	w1, [x4, 52]
	str	w1, [x19, 60]
	ldr	w1, [x4, 56]
	str	w1, [x19, 64]
	ldr	w1, [x27, 3204]
	str	w1, [x19, 68]
	ldr	w1, [x27, 3208]
	str	w1, [x19, 72]
	ldr	w1, [x27, 3200]
	str	w1, [x19, 76]
	ldr	w1, [x20, 128]
	cmp	w1, 1
	beq	.L1293
	ldr	x1, [x24, 752]
	cbnz	w7, .L1308
	str	w0, [x1, 704]
	ldr	x0, [x24, 752]
	ldr	w1, [x19, 32]
	str	w1, [x0, 708]
.L1309:
	ldrb	w0, [x27, 1584]
	cmp	w0, 2
	beq	.L1346
	ldr	w0, [x27, 1628]
	cbz	w0, .L1349
	ldr	x1, [x22, 28704]
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1312
	sub	w3, w0, #1
	mov	x0, x22
	add	x3, x22, x3, lsl 3
	b	.L1314
	.p2align 3
.L1316:
	ldr	x1, [x0, 28712]
	add	x0, x0, 8
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1312
.L1314:
	cmp	x3, x0
	bne	.L1316
.L1349:
	mov	w25, 0
.L1311:
	ldr	w0, [x27, 1632]
	cbz	w0, .L1350
	ldr	x1, [x22, 28968]
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1318
	sub	w3, w0, #1
	mov	x0, x22
	add	x3, x22, x3, lsl 3
	b	.L1320
	.p2align 3
.L1322:
	ldr	x1, [x0, 28976]
	add	x0, x0, 8
	ldr	x1, [x1, 32]
	ldrb	w2, [x1]
	cbz	w2, .L1318
.L1320:
	cmp	x3, x0
	bne	.L1322
.L1350:
	str	wzr, [x29, 108]
.L1317:
	ldrb	w0, [x24, 115]
	cbz	w0, .L1323
	ldr	w0, [x27, 1628]
	mov	w26, 0
	cbnz	w0, .L1376
	b	.L1339
	.p2align 3
.L1406:
	cbz	x0, .L1356
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1335:
	lsl	x0, x23, 5
	add	x0, x0, x23, lsl 2
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
	beq	.L1405
	strb	w1, [x0, 1630]
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1631]
	ldr	x1, [x28, 28704]
	ldr	w1, [x1, 12]
	str	w1, [x0, 1656]
.L1337:
	ldr	w0, [x27, 1628]
	add	w26, w26, 1
	cmp	w0, w26
	bls	.L1339
.L1376:
	uxtw	x23, w26
	ldr	w0, [x20, 116]
	add	x28, x22, x23, lsl 3
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x3, [x28, 28704]
	lsl	x1, x23, 5
	add	x1, x1, x23, lsl 2
	add	x1, x19, x1
	ldr	x3, [x3, 32]
	ldr	w3, [x3, 668]
	str	w3, [x1, 1636]
	ldr	x3, [x28, 28704]
	ldr	x3, [x3, 32]
	ldrb	w3, [x3, 1]
	strb	w3, [x1, 1625]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1406
	mov	w1, w25
	b	.L1335
	.p2align 3
.L1296:
	str	xzr, [x19, 4056]
	b	.L1297
	.p2align 3
.L1404:
	ldrb	w1, [x4, 2]
	strb	w1, [x19, 4]
	b	.L1305
	.p2align 3
.L1339:
	ldrb	w0, [x27, 1584]
	cmp	w0, 1
	beq	.L1407
.L1326:
	add	x1, x22, 24576
	add	x0, x19, 3928
	add	x1, x1, 4000
	add	x3, x19, 4056
	.p2align 2
.L1332:
	ldr	w2, [x1], 4
	str	w2, [x0], 4
	cmp	x3, x0
	bne	.L1332
	add	x1, x22, 16384
	ldrb	w0, [x1, 2594]
	str	w0, [x24, 3124]
	ldr	w1, [x1, 2636]
	str	w1, [x24, 3128]
	cbz	w0, .L1344
	ldrb	w0, [x27, 1584]
	cbz	w0, .L1345
.L1344:
	cmp	w1, 1
	beq	.L1408
.L1346:
	add	x21, x21, 4096
	ldr	x0, [x22, 632]
	add	x21, x21, 3786
	str	x0, [x22, x21, lsl 3]
	str	xzr, [x22, 632]
.L1293:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1405:
	strb	w1, [x0, 1628]
	ldr	x1, [x28, 28704]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 1629]
	ldr	x1, [x28, 28704]
	ldr	w1, [x1, 12]
	str	w1, [x0, 1652]
	b	.L1337
	.p2align 3
.L1308:
	ldr	w2, [x1, 704]
	cmp	w2, w0
	csel	w2, w2, w0, ge
	str	w2, [x1, 704]
	ldr	w2, [x1, 708]
	cmp	w2, w0
	csel	w0, w2, w0, le
	str	w0, [x1, 708]
	b	.L1309
	.p2align 3
.L1356:
	mov	w1, 0
	b	.L1335
	.p2align 3
.L1312:
	ldr	w1, [x1, 632]
	ldr	w0, [x20, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L1409
	ldr	x0, [x0, 640]
	ldr	w25, [x0, 8]
	b	.L1311
	.p2align 3
.L1403:
	ldr	w1, [x4, 44]
	strb	w1, [x19, 4]
	b	.L1305
	.p2align 3
.L1294:
	add	x27, x22, 20480
	mov	w4, 2208
	ldr	x0, [x22, 14968]
	mov	w6, 4000
	ldr	x2, [x22, 14960]
	ldr	w1, [x27, 1596]
	umull	x1, w1, w4
	add	x4, x0, x1
	ldr	w0, [x4, 28]
	umull	x0, w0, w6
	add	x6, x2, x0
	b	.L1295
	.p2align 3
.L1323:
	ldr	w0, [x27, 1628]
	mov	w26, 0
	cbnz	w0, .L1375
	b	.L1331
	.p2align 3
.L1410:
	cbz	x0, .L1352
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1329:
	lsl	x0, x23, 5
	add	w26, w26, 1
	add	x0, x0, x23, lsl 2
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
	ldr	w0, [x27, 1628]
	cmp	w0, w26
	bls	.L1331
.L1375:
	uxtw	x23, w26
	ldr	w0, [x20, 116]
	add	x28, x22, x23, lsl 3
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x3, [x28, 28704]
	lsl	x1, x23, 5
	add	x1, x1, x23, lsl 2
	add	x1, x19, x1
	ldr	x3, [x3, 32]
	ldr	w3, [x3, 668]
	str	w3, [x1, 1636]
	ldr	x3, [x28, 28704]
	ldr	x3, [x3, 32]
	ldrb	w3, [x3, 1]
	strb	w3, [x1, 1625]
	ldr	x1, [x28, 28704]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1410
	mov	w1, w25
	b	.L1329
	.p2align 3
.L1352:
	mov	w1, 0
	b	.L1329
	.p2align 3
.L1331:
	ldrb	w0, [x27, 1584]
	cmp	w0, 1
	bne	.L1326
	ldr	w0, [x27, 1632]
	cbz	w0, .L1326
	mov	w28, 0
	b	.L1334
	.p2align 3
.L1411:
	cbz	x0, .L1354
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1333:
	lsl	x0, x23, 5
	add	w28, w28, 1
	add	x0, x0, x23, lsl 2
	add	x0, x19, x0
	str	w1, [x0, 1640]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 672]
	str	w1, [x0, 2796]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 648]
	str	w1, [x0, 2784]
	ldr	x1, [x25, 28968]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2779]
	ldr	x1, [x25, 28968]
	ldrb	w1, [x1]
	strb	w1, [x0, 2778]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 692]
	str	w1, [x0, 2800]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 732]
	str	w1, [x0, 2804]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 772]
	str	w1, [x0, 2808]
	strb	wzr, [x0, 2776]
	ldr	w0, [x27, 1632]
	cmp	w0, w28
	bls	.L1326
.L1334:
	uxtw	x23, w28
	ldr	w0, [x20, 116]
	add	x25, x22, x23, lsl 3
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w2, [x1, 668]
	lsl	x1, x23, 5
	add	x1, x1, x23, lsl 2
	add	x1, x19, x1
	str	w2, [x1, 2788]
	ldr	x2, [x25, 28968]
	ldr	x2, [x2, 32]
	ldrb	w2, [x2, 1]
	strb	w2, [x1, 2777]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1411
	ldr	w1, [x29, 108]
	b	.L1333
	.p2align 3
.L1318:
	ldr	w1, [x1, 632]
	ldr	w0, [x20, 116]
	bl	FSP_GetLogicFs
	cbz	x0, .L1412
	ldr	x0, [x0, 640]
	ldr	w0, [x0, 8]
	str	w0, [x29, 108]
	b	.L1317
.L1408:
	ldrb	w0, [x27, 1584]
	cmp	w0, 1
	bne	.L1346
.L1345:
	ldr	w0, [x27, 1648]
	mov	w2, 0
	str	w0, [x19, 80]
	ldr	w0, [x27, 1652]
	str	w0, [x19, 84]
	.p2align 2
.L1347:
	ubfiz	x0, x2, 2, 32
	add	w2, w2, 1
	add	x1, x22, x0
	add	x0, x19, x0
	add	x1, x1, 20480
	ldr	w3, [x1, 1656]
	str	w3, [x0, 88]
	ldr	w3, [x1, 1784]
	str	w3, [x0, 216]
	ldr	w3, [x1, 1912]
	str	w3, [x0, 344]
	ldr	w3, [x1, 2424]
	str	w3, [x0, 856]
	ldr	w3, [x1, 2552]
	str	w3, [x0, 984]
	ldr	w1, [x1, 2680]
	str	w1, [x0, 1112]
	ldr	w0, [x27, 1640]
	cmp	w0, w2
	bcs	.L1347
	ldrb	w0, [x27, 1584]
	cmp	w0, 1
	bne	.L1346
	mov	w2, 0
	.p2align 2
.L1348:
	ubfiz	x0, x2, 2, 32
	add	w2, w2, 1
	add	x1, x22, x0
	add	x0, x19, x0
	add	x1, x1, 20480
	ldr	w3, [x1, 2040]
	str	w3, [x0, 472]
	ldr	w3, [x1, 2168]
	str	w3, [x0, 600]
	ldr	w3, [x1, 2296]
	str	w3, [x0, 728]
	ldr	w3, [x1, 2808]
	str	w3, [x0, 1240]
	ldr	w3, [x1, 2936]
	str	w3, [x0, 1368]
	ldr	w1, [x1, 3064]
	str	w1, [x0, 1496]
	ldr	w0, [x27, 1644]
	cmp	w0, w2
	bcs	.L1348
	b	.L1346
.L1354:
	mov	w1, 0
	b	.L1333
.L1407:
	ldr	w0, [x27, 1632]
	cbz	w0, .L1326
	mov	w28, 0
	b	.L1343
	.p2align 3
.L1414:
	cbz	x0, .L1358
	ldr	x0, [x0, 640]
	ldr	w1, [x0, 8]
.L1340:
	lsl	x0, x23, 5
	add	x0, x0, x23, lsl 2
	add	x0, x19, x0
	str	w1, [x0, 2792]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 672]
	str	w1, [x0, 2796]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 648]
	str	w1, [x0, 2784]
	ldr	x1, [x25, 28968]
	ldrb	w1, [x1]
	strb	w1, [x0, 2776]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1, 680]
	strb	w1, [x0, 2778]
	ldr	x1, [x25, 28968]
	ldrb	w1, [x1]
	cmp	w1, 1
	beq	.L1413
	strb	w1, [x0, 2782]
	ldr	x1, [x25, 28968]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2783]
	ldr	x1, [x25, 28968]
	ldr	w1, [x1, 12]
	str	w1, [x0, 2808]
.L1342:
	ldr	w0, [x27, 1632]
	add	w28, w28, 1
	cmp	w0, w28
	bls	.L1326
.L1343:
	uxtw	x23, w28
	ldr	w0, [x20, 116]
	add	x25, x22, x23, lsl 3
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldr	w2, [x1, 668]
	lsl	x1, x23, 5
	add	x1, x1, x23, lsl 2
	add	x1, x19, x1
	str	w2, [x1, 2788]
	ldr	x2, [x25, 28968]
	ldr	x2, [x2, 32]
	ldrb	w2, [x2, 1]
	strb	w2, [x1, 2777]
	ldr	x1, [x25, 28968]
	ldr	x1, [x1, 32]
	ldrb	w1, [x1]
	cmp	w1, 1
	bne	.L1414
	ldr	w1, [x29, 108]
	b	.L1340
	.p2align 3
.L1413:
	strb	w1, [x0, 2780]
	ldr	x1, [x25, 28968]
	ldrb	w1, [x1, 1]
	strb	w1, [x0, 2781]
	ldr	x1, [x25, 28968]
	ldr	w1, [x1, 12]
	str	w1, [x0, 2804]
	b	.L1342
.L1358:
	mov	w1, 0
	b	.L1340
.L1412:
	ldr	x1, .LC121
	mov	w2, 7538
	bl	dprint_vfmw
	b	.L1293
.L1409:
	ldr	x1, .LC121
	mov	w2, 7517
	bl	dprint_vfmw
	b	.L1293
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
	cbz	x2, .L1417
	.p2align 2
.L1418:
	ldp	w3, w1, [x2, 8]
	ldr	x2, [x2, 4056]
	add	w1, w3, w1
	add	w1, w1, 7
	add	w0, w0, w1, lsr 3
	cbnz	x2, .L1418
.L1417:
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
	cbnz	w1, .L1431
	mov	w1, 262143
	cmp	w0, w1
	bhi	.L1432
	ldr	w1, [x19, 14932]
	ldr	w2, [x19, 14936]
	mul	w1, w1, w2
	sub	w1, w1, #1
	cmp	w0, w1
	bhi	.L1431
	ldr	x1, .LC127
	mov	x0, x19
	bl	ue_v
	mov	w2, w0
	ldr	x1, .LC129
	mov	w19, w0
	mov	w0, 19
	bl	dprint_vfmw
	ldr	w0, [x20, 56]
	cbnz	w0, .L1431
	cmp	w19, 9
	bls	.L1433
	ldr	x1, .LC132
	mov	w2, w19
	mov	w0, 1
	bl	dprint_vfmw
.L1431:
	mov	w0, -1
.L1423:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1433:
	ldr	x0, .LC130
	ldrb	w0, [x0,w19,uxtw]
	adr	x1, .Lrtx1428
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx1428:
	.section	.rodata
	.align	0
	.align	2
.L1428:
	.byte	(.L1427 - .Lrtx1428) / 4
	.byte	(.L1429 - .Lrtx1428) / 4
	.byte	(.L1430 - .Lrtx1428) / 4
	.byte	(.L1427 - .Lrtx1428) / 4
	.byte	(.L1430 - .Lrtx1428) / 4
	.byte	(.L1427 - .Lrtx1428) / 4
	.byte	(.L1429 - .Lrtx1428) / 4
	.byte	(.L1430 - .Lrtx1428) / 4
	.byte	(.L1427 - .Lrtx1428) / 4
	.byte	(.L1430 - .Lrtx1428) / 4
	.text
	.p2align 3
.L1427:
	strb	wzr, [x21, 1584]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1430:
	mov	w1, 2
	strb	w1, [x21, 1584]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1429:
	mov	w1, 1
	strb	w1, [x21, 1584]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1432:
	ldr	x1, .LC125
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1423
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
	.xword	.L1428
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
	cbz	w1, .L1438
	ldr	w2, [x0, 28]
	mov	w1, 4000
	ldr	x3, [x19, 14960]
	mov	w0, 0
	umull	x1, w2, w1
	add	x1, x3, x1
	ldrb	w1, [x1, 23]
	cbz	w1, .L1439
.L1436:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1438:
	ldr	x1, .LC136
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1436
	.p2align 3
.L1439:
	ldr	x1, .LC138
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1436
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
	cbz	w1, .L1445
	ldr	w1, [x19, 14944]
	ldr	w2, [x0, 28]
	sub	w0, w1, #1
	cmp	w2, w0
	bhi	.L1446
	mov	w1, 4000
	ldr	x3, [x19, 14960]
	mov	w0, 0
	umull	x1, w2, w1
	add	x1, x3, x1
	ldrb	w1, [x1, 23]
	cbz	w1, .L1447
.L1442:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1445:
	ldr	x1, .LC142
	mov	w2, w20
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1442
	.p2align 3
.L1446:
	ldr	x1, .LC144
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1442
	.p2align 3
.L1447:
	ldr	x1, .LC146
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1442
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
	beq	.L1450
	ldrb	w0, [x1, 1586]
	ldrb	w3, [x1, 3233]
	cmp	w3, w0
	cset	w0, ne
	orr	w2, w2, w0
.L1450:
	ldr	w3, [x1, 1616]
	ldr	w0, [x1, 3244]
	cmp	w0, w3
	beq	.L1455
	cmp	w3, wzr
	ccmp	w0, wzr, 4, ne
	cset	w0, eq
.L1451:
	ldrb	w4, [x1, 3234]
	ldrb	w3, [x1, 1587]
	cmp	w4, w3
	cset	w5, ne
	cmp	w4, 5
	ccmp	w3, 5, 0, eq
	orr	w2, w5, w2
	orr	w2, w2, w0
	beq	.L1459
.L1452:
	add	x19, x19, 16384
	ldr	w0, [x19, 792]
	cbnz	w0, .L1453
	ldr	w4, [x1, 704]
	ldr	w5, [x1, 3256]
	ldr	w0, [x1, 708]
	ldr	w3, [x1, 3260]
	cmp	w5, w4
	ccmp	w3, w0, 0, eq
	cset	w0, ne
	orr	w2, w2, w0
.L1454:
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
.L1453:
	cmp	w0, 1
	bne	.L1454
	ldr	w4, [x1, 752]
	ldr	w5, [x1, 3264]
	ldr	w0, [x1, 756]
	ldr	w3, [x1, 3268]
	cmp	w5, w4
	ccmp	w3, w0, 0, eq
	cset	w0, ne
	orr	w2, w2, w0
	b	.L1454
	.p2align 3
.L1455:
	mov	w0, 0
	b	.L1451
	.p2align 3
.L1459:
	ldr	w0, [x1, 1624]
	ldr	w3, [x1, 3252]
	cmp	w3, w0
	cset	w0, ne
	orr	w2, w2, w0
	b	.L1452
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
	cbz	w3, .L1461
	add	w0, w0, 5
	add	x22, x19, 65536
	lsl	w23, w23, w0
	ldrb	w0, [x20, 1584]
	cmp	w0, 1
	bls	.L1512
.L1511:
	ldr	w3, [x22, 56]
.L1479:
	cmp	w3, wzr
	csetm	w0, ne
.L1510:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1461:
	add	w0, w0, 4
	add	x22, x19, 65536
	lsl	w23, w23, w0
	ldrb	w0, [x20, 1584]
	cmp	w0, 1
	bhi	.L1511
.L1512:
	ldr	x1, .LC150
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 796]
	cbz	w0, .L1470
	ldr	w0, [x20, 1640]
	cmn	w0, #2
	beq	.L1493
	ldr	x24, .LC152
	mov	w27, 0
	ldr	x26, .LC160
	ldr	x25, .LC156
	b	.L1476
	.p2align 3
.L1471:
	cmp	w0, 2
	beq	.L1513
.L1474:
	ldr	w0, [x20, 1640]
	add	w0, w0, 2
	cmp	w0, w27
	bls	.L1514
.L1476:
	add	x21, x19, x27, uxtw 2
	mov	x1, x24
	add	x21, x21, 20480
	mov	x0, x19
	add	w27, w27, 1
	bl	ue_v
	cmp	w0, 3
	str	w0, [x21, 800]
	ldr	w3, [x22, 56]
	bhi	.L1467
	cbnz	w3, .L1467
	beq	.L1470
	cmp	w0, 1
	bhi	.L1471
	mov	x1, x25
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1064]
	cmp	w23, w0
	ldr	w3, [x22, 56]
	bls	.L1473
	cbz	w3, .L1474
.L1473:
	ldr	x1, .LC158
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1510
	.p2align 3
.L1470:
	ldrb	w0, [x20, 1584]
.L1463:
	cmp	w0, 1
	bne	.L1511
	ldr	x1, .LC164
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 797]
	cbz	w0, .L1511
	ldr	w0, [x20, 1644]
	cmn	w0, #2
	beq	.L1511
	ldr	x24, .LC166
	mov	w27, 0
	ldr	x26, .LC174
	ldr	x25, .LC170
	b	.L1491
	.p2align 3
.L1485:
	cmp	w0, 2
	beq	.L1515
	ldr	w0, [x20, 1644]
	add	w0, w0, 2
	cmp	w27, w0
	beq	.L1492
.L1490:
	add	w27, w27, 1
	cmp	w27, w0
	bcs	.L1511
.L1491:
	add	x21, x19, x27, uxtw 2
	mov	x1, x24
	add	x21, x21, 20480
	mov	x0, x19
	bl	ue_v
	cmp	w0, 3
	str	w0, [x21, 932]
	ldr	w3, [x22, 56]
	bhi	.L1483
	cbnz	w3, .L1483
	beq	.L1479
	cmp	w0, 1
	bhi	.L1485
	mov	x1, x25
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1196]
	cmp	w23, w0
	ldr	w3, [x22, 56]
	bls	.L1487
	cbnz	w3, .L1487
.L1488:
	ldr	w0, [x20, 1644]
	add	w0, w0, 2
	cmp	w0, w27
	bne	.L1490
	ldr	w1, [x21, 932]
	cmp	w1, 3
	beq	.L1490
.L1492:
	ldr	x1, .LC176
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1510
	.p2align 3
.L1513:
	mov	x1, x26
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1328]
	b	.L1474
	.p2align 3
.L1515:
	mov	x1, x26
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 1456]
	b	.L1488
	.p2align 3
.L1467:
	ldr	x1, .LC154
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1510
	.p2align 3
.L1483:
	ldr	x1, .LC168
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1510
	.p2align 3
.L1514:
	bne	.L1470
.L1465:
	uxtw	x0, w27
	add	x0, x0, 4096
	add	x0, x19, x0, lsl 2
	ldr	w0, [x0, 4896]
	cmp	w0, 3
	bne	.L1477
	ldrb	w0, [x20, 1584]
	add	x22, x19, 65536
	b	.L1463
.L1487:
	ldr	x1, .LC172
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1510
.L1493:
	mov	w27, 0
	b	.L1465
.L1477:
	ldr	x1, .LC162
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1510
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
	cbnz	w1, .L1541
.L1517:
	cmp	w0, 7
	bhi	.L1520
	ldr	w0, [x21, 1652]
	cmp	w0, 7
	bhi	.L1520
	add	x0, x26, 65536
	str	x0, [x29, 104]
	ldr	w2, [x0, 56]
	cbnz	w2, .L1519
	ldr	x22, .LC184
	mov	w27, 0
	b	.L1528
	.p2align 3
.L1523:
	str	w23, [x1, 1656]
	add	x0, x26, x19, lsl 2
	str	wzr, [x1, 2424]
	add	x0, x0, 20480
	ldr	w1, [x20, 40]
	cbnz	w1, .L1542
.L1525:
	str	wzr, [x0, 1784]
	str	wzr, [x0, 1912]
	str	wzr, [x0, 2552]
	str	wzr, [x0, 2680]
.L1527:
	ldr	w0, [x21, 1640]
	add	w27, w27, 1
	cmp	w0, w27
	bcc	.L1543
.L1528:
	mov	x1, x22
	mov	x0, x26
	uxtw	x19, w27
	bl	u_1
	add	x1, x26, x19, lsl 2
	add	x1, x1, 20480
	cbz	w0, .L1523
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
	cbz	w1, .L1525
.L1542:
	ldr	x1, .LC190
	mov	x0, x26
	bl	u_1
	add	x1, x26, x19, lsl 2
	add	x1, x1, 20480
	cbnz	w0, .L1544
	str	w25, [x1, 1784]
	add	w27, w27, 1
	str	w25, [x1, 1912]
	str	wzr, [x1, 2552]
	str	wzr, [x1, 2680]
	ldr	w0, [x21, 1640]
	cmp	w0, w27
	bcs	.L1528
.L1543:
	ldr	w0, [x24, 44]
	cmp	w0, 1
	beq	.L1529
.L1530:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 56]
	cmp	w0, wzr
	csetm	w0, ne
.L1522:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L1544:
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
	b	.L1527
	.p2align 3
.L1541:
	ldr	x1, .LC180
	mov	x0, x26
	bl	ue_v
	mov	w25, w0
	ldr	w0, [x21, 1648]
	str	w25, [x21, 1652]
	lsl	w25, w19, w25
	b	.L1517
	.p2align 3
.L1529:
	ldrb	w0, [x21, 1584]
	cmp	w0, 1
	bne	.L1530
	ldr	x22, .LC196
	mov	w28, 0
	ldr	x27, .LC198
	b	.L1536
	.p2align 3
.L1531:
	add	x0, x26, x19, lsl 2
	add	x0, x0, 20480
	str	w23, [x0, 2040]
	str	wzr, [x0, 2808]
	add	x0, x26, x19, lsl 2
	add	x0, x0, 20480
	ldr	w1, [x20, 40]
	cbnz	w1, .L1545
.L1533:
	str	wzr, [x0, 2168]
	str	wzr, [x0, 2296]
	str	wzr, [x0, 2936]
	str	wzr, [x0, 3064]
.L1535:
	ldr	w0, [x21, 1644]
	add	w28, w28, 1
	cmp	w0, w28
	bcc	.L1530
.L1536:
	mov	x1, x22
	mov	x0, x26
	uxtw	x19, w28
	bl	u_1
	cbz	w0, .L1531
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
	cbz	w1, .L1533
.L1545:
	ldr	x1, .LC202
	mov	x0, x26
	bl	u_1
	cbnz	w0, .L1546
	add	x0, x26, x19, lsl 2
	add	x0, x0, 20480
	str	w25, [x0, 2168]
	str	w25, [x0, 2296]
	str	wzr, [x0, 2936]
	str	wzr, [x0, 3064]
	b	.L1535
.L1520:
	add	x3, x26, 65536
	ldr	w2, [x3, 56]
.L1519:
	ldr	x1, .LC182
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1522
.L1546:
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
	b	.L1535
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
	cbnz	w19, .L1548
	mov	w22, 100
	ldr	x21, .LC210
	ldr	x25, .LC212
	add	x23, x20, 65536
	ldr	x27, .LC218
	ldr	x24, .LC216
	ldr	x26, .LC214
	b	.L1559
	.p2align 3
.L1560:
	cmp	w19, 2
	beq	.L1573
.L1561:
	cmp	w19, 6
	cset	w0, eq
	orr	w0, w28, w0
	cbnz	w0, .L1564
	cmp	w19, 4
	beq	.L1574
.L1565:
	cmp	w19, 6
	bhi	.L1557
	ldr	w0, [x23, 56]
	cbnz	w0, .L1557
.L1575:
	cbz	w19, .L1567
.L1566:
	subs	w22, w22, #1
	beq	.L1568
.L1559:
	mov	x1, x21
	mov	x0, x20
	bl	ue_v
	cmp	w0, 3
	mov	w19, w0
	and	w0, w0, -3
	cset	w28, eq
	cmp	w0, 1
	bne	.L1560
	mov	x1, x25
	mov	x0, x20
	bl	ue_v
	cmp	w19, 2
	bne	.L1561
.L1573:
	mov	x1, x26
	mov	x0, x20
	bl	ue_v
	ldr	w0, [x23, 56]
	cbz	w0, .L1575
	.p2align 2
.L1557:
	ldr	x1, .LC220
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L1551:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1564:
	mov	x1, x24
	mov	x0, x20
	bl	ue_v
	cmp	w19, 4
	bne	.L1565
.L1574:
	mov	x1, x27
	mov	x0, x20
	bl	ue_v
	ldr	w0, [x23, 56]
	cbz	w0, .L1566
	b	.L1557
	.p2align 3
.L1548:
	ldr	x21, .LC210
	mov	w26, 0
	ldr	x25, .LC212
	add	x22, x20, 24576
	ldr	x24, .LC216
	add	x23, x20, 65536
	b	.L1550
	.p2align 3
.L1552:
	cmp	w1, 2
	beq	.L1576
.L1553:
	cmp	w1, 3
	ccmp	w1, 6, 4, ne
	beq	.L1577
.L1554:
	cmp	w1, 4
	beq	.L1578
.L1555:
	cmp	w1, 6
	bhi	.L1557
	ldr	w0, [x23, 56]
	cbnz	w0, .L1557
	add	w26, w26, 1
	cbz	w1, .L1567
	cmp	w26, 100
	beq	.L1579
.L1550:
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
	bne	.L1552
	mov	x1, x25
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3284]
	ldr	w1, [x19, 3280]
	cmp	w1, 2
	bne	.L1553
.L1576:
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
	bne	.L1554
	.p2align 2
.L1577:
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
	bne	.L1555
.L1578:
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
	b	.L1555
	.p2align 3
.L1567:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
.L1579:
	ldr	x1, .LC208
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1551
.L1568:
	mov	w0, -1
	b	.L1551
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
	cbz	w0, .L1581
	cmp	w23, wzr
	cset	w19, eq
	str	w19, [x20, 3188]
	ldrb	w0, [x1, 1587]
	cmp	w0, 5
	beq	.L1599
.L1582:
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
	cbnz	w0, .L1584
.L1588:
	mov	w22, 0
.L1596:
	mov	w0, w22
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1581:
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
	cbnz	w0, .L1600
	ldrb	w0, [x1, 1587]
	uxtw	x19, w23
	cmp	w0, 5
	bne	.L1582
.L1599:
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
.L1600:
	ldrb	w0, [x1, 1587]
	cmp	w0, 5
	beq	.L1601
	ldr	x1, .LC228
	mov	x0, x21
	ldrb	w20, [x19, 3279]
	bl	u_1
	cmp	w20, w0
	bne	.L1602
	ldrb	w0, [x19, 3279]
	cbz	w0, .L1588
	mov	w1, 0
	mov	x0, x21
	bl	DecMMCO
	mov	w22, w0
	b	.L1596
	.p2align 3
.L1584:
	mov	w1, 1
	mov	x0, x21
	bl	DecMMCO
	cmn	w0, #1
	mov	w22, w0
	beq	.L1603
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
.L1601:
	ldr	x1, .LC222
	mov	x0, x21
	ldrb	w20, [x19, 3277]
	bl	u_1
	cmp	w20, w0
	bne	.L1604
	ldr	x1, .LC224
	mov	x0, x21
	ldrb	w19, [x19, 3278]
	bl	u_1
	cmp	w19, w0
	beq	.L1588
	ldr	x1, .LC236
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1596
.L1602:
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
	b	.L1596
.L1604:
	ldr	x1, .LC234
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1596
.L1603:
	strb	wzr, [x19, 3276]
	mov	w0, 1
	ldr	x1, .LC232
	str	w23, [x20, 3188]
	bl	dprint_vfmw
	b	.L1596
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
	bhi	.L1606
	add	x24, x23, 65536
	ldr	w0, [x24, 56]
	cbnz	w0, .L1606
	mov	w1, w21
	mov	x0, x23
	bl	PPSSPSCheckTmpId
	mov	w22, w0
	cbnz	w0, .L1669
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
	beq	.L1610
	ldr	w0, [x20, 3244]
	cbz	w0, .L1610
.L1611:
	ldr	w1, [x20, 2196]
	mov	x0, x23
	ldr	x2, .LC250
	add	w1, w1, 4
	bl	u_v
	str	w0, [x19, 1604]
	ldr	w1, [x24, 56]
	cbnz	w1, .L1635
	ldrb	w1, [x19, 1587]
	cmp	w1, 5
	beq	.L1670
.L1614:
	strb	wzr, [x19, 1585]
	strb	wzr, [x19, 1586]
	ldrb	w0, [x20, 18]
	cbz	w0, .L1671
	ldr	w1, [x20, 3252]
	ldr	w2, [x20, 3248]
	add	w0, w1, 1
	madd	w0, w2, w0, w0
.L1638:
	ldrb	w1, [x20, 19]
	cbnz	w1, .L1621
	strb	wzr, [x19, 1588]
.L1623:
	ldr	w1, [x19, 1636]
	cmp	w1, w0
	bcs	.L1672
.L1624:
	ldrb	w0, [x19, 1587]
	cmp	w0, 5
	beq	.L1673
.L1626:
	str	wzr, [x19, 704]
	str	wzr, [x19, 708]
	ldr	w0, [x20, 2200]
	cbz	w0, .L1674
.L1630:
	str	wzr, [x19, 752]
	str	wzr, [x19, 756]
	ldr	w0, [x20, 2200]
	cmp	w0, 1
	beq	.L1675
.L1633:
	ldr	w0, [x24, 56]
	cbnz	w0, .L1635
	ldrb	w0, [x21, 17]
	cbz	w0, .L1636
	ldr	x1, .LC274
	mov	x0, x23
	bl	ue_v
	cbnz	w0, .L1637
	ldr	w0, [x24, 56]
	cbnz	w0, .L1637
.L1636:
	mov	x0, x23
	bl	IsNewPic
	uxtb	w0, w0
	strb	w0, [x19, 1589]
	cbnz	w0, .L1649
	ldr	w1, [x19, 1636]
	ldr	w0, [x19, 3272]
	cmp	w1, w0
	bls	.L1676
.L1649:
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1610:
	ldrb	w0, [x19, 1584]
	cmp	w0, 2
	beq	.L1611
	ldr	x1, .LC248
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1649
	.p2align 3
.L1621:
	mov	w1, 1
	strb	w1, [x19, 1588]
.L1640:
	ldr	w1, [x19, 1636]
	cmp	w1, w0, lsr 1
	bcc	.L1624
	ldr	x1, .LC258
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1649
	.p2align 3
.L1637:
	ldr	x1, .LC276
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC277
	ldr	x4, [x0]
	cbz	x4, .L1635
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
.L1671:
	ldr	x1, .LC254
	mov	x0, x23
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 1585]
	cbnz	w0, .L1677
	ldr	w2, [x20, 3252]
	ldr	w1, [x20, 3248]
	add	w0, w2, 1
	ldrb	w3, [x20, 18]
	madd	w1, w1, w0, w0
	lsl	w0, w1, 1
	cbz	w3, .L1638
.L1618:
	ldrb	w2, [x19, 1585]
	mov	w0, 1
	b	.L1619
	.p2align 3
.L1674:
	ldr	w1, [x20, 2204]
	mov	x0, x23
	ldr	x2, .LC266
	add	w1, w1, 4
	bl	u_v
	str	w0, [x19, 704]
	ldrb	w0, [x21, 1]
	cbz	w0, .L1630
	ldrb	w0, [x19, 1585]
	cbnz	w0, .L1630
	ldr	x1, .LC268
	mov	x0, x23
	bl	se_v
	str	w0, [x19, 708]
	b	.L1630
	.p2align 3
.L1670:
	str	w0, [x19, 1600]
	cbz	w0, .L1614
	ldr	x1, .LC252
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1614
	.p2align 3
.L1675:
	ldrb	w0, [x20, 16]
	cbnz	w0, .L1633
	ldr	x1, .LC270
	mov	x0, x23
	bl	se_v
	str	w0, [x19, 752]
	ldrb	w0, [x21, 1]
	cmp	w0, 1
	bne	.L1633
	ldrb	w0, [x19, 1585]
	cbnz	w0, .L1633
	ldr	x1, .LC272
	mov	x0, x23
	bl	se_v
	str	w0, [x19, 756]
	b	.L1633
	.p2align 3
.L1635:
	mov	w22, -1
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1677:
	ldr	x1, .LC256
	mov	x0, x23
	bl	u_1
	strb	w0, [x19, 1586]
	ldr	w0, [x20, 3252]
	ldr	w1, [x20, 3248]
	ldrb	w2, [x20, 18]
	add	w0, w0, 1
	madd	w1, w1, w0, w0
	cbnz	w2, .L1618
	ldrb	w2, [x19, 1585]
	mov	w0, 2
.L1619:
	mul	w0, w0, w1
	cbz	w2, .L1638
	strb	wzr, [x19, 1588]
	cmp	w2, 1
	bne	.L1623
	b	.L1640
	.p2align 3
.L1673:
	ldr	x1, .LC262
	mov	x0, x23
	bl	ue_v
	str	w0, [x19, 1624]
	mov	w1, 65535
	cmp	w0, w1
	bhi	.L1627
	ldr	w0, [x24, 56]
	cbz	w0, .L1626
.L1627:
	ldr	x1, .LC264
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1626
	.p2align 3
.L1606:
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
.L1669:
	ldr	x1, .LC246
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1649
.L1672:
	ldr	x1, .LC260
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1649
.L1676:
	ldr	x1, .LC279
	mov	w0, 1
	mov	w22, -1
	bl	dprint_vfmw
	b	.L1649
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
	bcc	.L1679
	add	x24, x20, 65536
	ldr	w0, [x24, 56]
	cbnz	w0, .L1679
	mov	x0, x20
	bl	PPSSPSCheck
	cbnz	w0, .L1762
	ldr	w22, [x19, 1596]
	mov	w0, 2208
	ldrb	w1, [x19, 1584]
	ldr	x25, [x20, 14968]
	umull	x22, w22, w0
	cmp	w1, 1
	add	x21, x25, x22
	beq	.L1763
	ldr	w0, [x21, 36]
	cmp	w1, 1
	str	w0, [x19, 1640]
	ldr	w0, [x21, 40]
	str	w0, [x19, 1644]
	bls	.L1764
.L1684:
	cmp	w1, 2
	beq	.L1710
.L1694:
	mov	w0, 32
	str	w0, [x19, 1644]
.L1695:
	mov	x0, x20
	bl	ref_pic_list_reordering
	mov	w23, w0
	cbnz	w0, .L1765
	ldrb	w0, [x21, 2]
	cbz	w0, .L1697
	ldrb	w0, [x19, 1584]
	cbnz	w0, .L1697
	mov	w0, 1
	mov	w2, w0
.L1771:
	ldr	x1, .LC297
	strb	w0, [x19, 1591]
	mov	w0, 19
	bl	dprint_vfmw
	str	wzr, [x19, 1648]
	ldrb	w0, [x19, 1591]
	str	wzr, [x19, 1652]
	cbz	w0, .L1699
.L1767:
	mov	x0, x20
	bl	pred_weight_table
	cbz	w0, .L1699
	ldr	x1, .LC299
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1697:
	ldr	w0, [x21, 44]
	cmp	w0, 1
	beq	.L1766
.L1714:
	mov	w0, 0
	ldr	x1, .LC297
	strb	w0, [x19, 1591]
	mov	w2, 0
	mov	w0, 19
	bl	dprint_vfmw
	str	wzr, [x19, 1648]
	ldrb	w0, [x19, 1591]
	str	wzr, [x19, 1652]
	cbnz	w0, .L1767
.L1699:
	ldr	w0, [x19, 1616]
	cbnz	w0, .L1768
.L1700:
	str	wzr, [x19, 3192]
	ldrb	w0, [x25, x22]
	cbz	w0, .L1701
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bls	.L1769
.L1701:
	ldr	x1, .LC307
	mov	x0, x20
	bl	se_v
	add	x1, x19, 3072
	stp	w0, wzr, [x1, 124]
	stp	wzr, wzr, [x1, 132]
	ldrb	w0, [x21, 3]
	cbnz	w0, .L1770
.L1730:
	mov	w0, w23
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L1770:
	ldr	x1, .LC309
	mov	x0, x20
	bl	ue_v
	cmp	w0, 2
	str	w0, [x19, 3200]
	bhi	.L1704
	ldr	w1, [x24, 56]
	cbnz	w1, .L1704
	cmp	w0, 1
	beq	.L1730
	ldr	x1, .LC313
	mov	x0, x20
	bl	se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 3204]
	bhi	.L1707
	ldr	w0, [x24, 56]
	cbnz	w0, .L1707
	ldr	x1, .LC317
	mov	x0, x20
	bl	se_v
	add	w1, w0, 6
	cmp	w1, 12
	str	w0, [x19, 3208]
	bhi	.L1709
	ldr	w0, [x24, 56]
	cbz	w0, .L1730
.L1709:
	ldr	x1, .LC319
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1768:
	mov	x0, x20
	bl	dec_ref_pic_marking
	cbz	w0, .L1700
	ldr	x1, .LC301
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1710:
	mov	w0, 32
	str	w0, [x19, 1640]
	b	.L1694
	.p2align 3
.L1766:
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bne	.L1714
	mov	w2, w0
	b	.L1771
	.p2align 3
.L1763:
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
	bhi	.L1684
	.p2align 2
.L1764:
	ldr	x1, .LC285
	mov	x0, x20
	bl	u_1
	cbnz	w0, .L1685
	ldr	w1, [x19, 1640]
.L1686:
	ldrb	w0, [x19, 1585]
	cbnz	w0, .L1687
	cmp	w1, 15
	bhi	.L1688
	ldr	w0, [x19, 1644]
	cmp	w0, 15
	bhi	.L1772
.L1689:
	ldr	w0, [x24, 56]
	cbnz	w0, .L1688
.L1690:
	ldrb	w0, [x19, 1584]
	cmp	w0, 2
	beq	.L1710
	cmp	w0, 1
	beq	.L1695
	b	.L1694
	.p2align 3
.L1679:
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
.L1769:
	ldr	x1, .LC303
	mov	x0, x20
	bl	ue_v
	cmp	w0, 2
	str	w0, [x19, 3192]
	bhi	.L1702
	ldr	w0, [x24, 56]
	cbz	w0, .L1701
.L1702:
	ldr	x1, .LC305
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1687:
	cmp	w1, 31
	bhi	.L1691
	ldr	w0, [x19, 1644]
	cmp	w0, 31
	bls	.L1692
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bne	.L1692
.L1691:
	ldr	x1, .LC293
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1692:
	ldr	w0, [x24, 56]
	cbz	w0, .L1690
	b	.L1691
	.p2align 3
.L1772:
	ldrb	w0, [x19, 1584]
	cmp	w0, 1
	bne	.L1689
.L1688:
	ldr	x1, .LC291
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1685:
	ldr	x1, .LC287
	mov	x0, x20
	bl	ue_v
	mov	w1, w0
	ldrb	w0, [x19, 1584]
	str	w1, [x19, 1640]
	cmp	w0, 1
	bne	.L1686
	ldr	x1, .LC289
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 1644]
	ldr	w1, [x19, 1640]
	b	.L1686
	.p2align 3
.L1707:
	ldr	x1, .LC315
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1704:
	ldr	x1, .LC311
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1762:
	ldr	x1, .LC281
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
	.p2align 3
.L1765:
	ldr	x1, .LC295
	mov	w0, 1
	mov	w23, -1
	bl	dprint_vfmw
	b	.L1730
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
	cbz	w3, .L1774
	ldrb	w2, [x0, 1586]
	strb	w2, [x0, 3233]
.L1774:
	ldrb	w2, [x0, 1587]
	ldr	w3, [x0, 1616]
	strb	w2, [x0, 3234]
	cmp	w2, 5
	str	w3, [x0, 3244]
	beq	.L1781
.L1775:
	ldr	w2, [x1, 2200]
	cbnz	w2, .L1776
	ldr	w2, [x0, 708]
	ldr	w3, [x0, 704]
	str	w3, [x0, 3256]
	str	w2, [x0, 3260]
	ldr	w2, [x1, 2200]
.L1776:
	cmp	w2, 1
	bne	.L1777
	ldr	w2, [x0, 752]
	ldr	w1, [x0, 756]
	str	w2, [x0, 3264]
	str	w1, [x0, 3268]
.L1777:
	add	x19, x19, 65536
	ldr	w0, [x19, 72]
	add	w0, w0, 1
	str	w0, [x19, 72]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1781:
	ldr	w2, [x0, 1624]
	str	w2, [x0, 3252]
	b	.L1775
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
	beq	.L1784
	cbz	w0, .L1785
	cmp	w0, 2
	bne	.L1782
	ldrb	w0, [x1, 1587]
	add	x19, x19, 61440
	cmp	w0, 5
	beq	.L1792
.L1787:
	str	wzr, [x19, 800]
.L1782:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1785:
	add	x19, x19, 61440
	ldr	w0, [x19, 796]
	cmp	w0, 2
	beq	.L1787
	mov	w0, 1
	str	wzr, [x19, 800]
	str	w0, [x19, 796]
	b	.L1782
	.p2align 3
.L1784:
	add	x19, x19, 61440
	mov	w0, 2
	str	wzr, [x19, 800]
	str	w0, [x19, 796]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1792:
	mov	w0, 1
	str	w0, [x19, 800]
	b	.L1782
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
	cbz	w0, .L1794
	ldr	x1, [x19, 632]
	ldr	w0, [x1, 72]
	add	w0, w0, 7
	and	w0, w0, -8
	str	w0, [x1, 72]
	ldr	x1, [x19, 632]
	ldr	w0, [x1, 80]
	sub	w0, w0, #1
	str	w0, [x1, 80]
.L1794:
	ldr	x3, [x19, 632]
	ldrb	w21, [x3]
	cbz	w21, .L1805
	sub	w1, w21, #1
	mov	w4, 32
	mov	x2, 48
	add	x0, x3, 16
	mov	w20, 0
	umaddl	x2, w1, w4, x2
	add	x2, x3, x2
	.p2align 2
.L1796:
	ldr	w1, [x0], 32
	cmp	x0, x2
	add	w20, w20, w1
	bne	.L1796
.L1795:
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
	bhi	.L1816
	ldrb	w20, [x0]
	add	w3, w20, 1
	cmp	w3, 1
	ccmp	w2, w3, 0, ls
	bls	.L1799
	ldr	x21, .LC327
	add	w20, w20, 2
	.p2align 2
.L1800:
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
	bne	.L1800
	sub	w1, w1, #1
.L1799:
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
	bcs	.L1793
	.p2align 2
.L1810:
	ubfiz	x1, x0, 5, 32
	ldr	w2, [x5, 824]
	add	x1, x3, x1
	add	w0, w0, 1
	ldr	w1, [x1, 32]
	cmp	w1, w2
	bcs	.L1802
	str	w1, [x5, 824]
	ldr	w4, [x3, 76]
.L1802:
	cmp	w4, w0
	bhi	.L1810
.L1793:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L1816:
	ldr	x1, .LC325
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
.L1805:
	mov	w20, 0
	b	.L1795
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
	ble	.L1817
	sub	w26, w27, #1
	mov	w0, 8
	ldr	x21, .LC330
	add	x26, x26, 1
	mov	x20, 0
	mov	w24, w0
	mov	w23, 255
	b	.L1822
	.p2align 3
.L1821:
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
	beq	.L1817
.L1822:
	cmp	w27, 16
	add	x1, x21, 16
	beq	.L1830
	ldrb	w19, [x20, x1]
.L1820:
	add	x20, x20, 1
	cbz	w0, .L1821
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
	b	.L1821
	.p2align 3
.L1830:
	ldrb	w19, [x21, x20]
	b	.L1820
	.p2align 3
.L1817:
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
	cbnz	w0, .L1882
	ldr	x1, .LC338
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 1]
	ldr	w0, [x21, 56]
	cbnz	w0, .L1882
	ldr	x1, .LC340
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 32]
	cbnz	w0, .L1835
	ldr	w0, [x21, 56]
	cbz	w0, .L1836
.L1835:
	ldr	x1, .LC342
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC343
	ldr	x4, [x0]
	cbz	x4, .L1882
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
	mov	w0, -1
.L1833:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1836:
	ldr	x1, .LC345
	mov	x0, x19
	bl	ue_v
	cmp	w0, 31
	str	w0, [x20, 36]
	bhi	.L1837
	ldr	w0, [x21, 56]
	cbnz	w0, .L1837
	ldr	x1, .LC349
	mov	x0, x19
	bl	ue_v
	cmp	w0, 31
	str	w0, [x20, 40]
	bhi	.L1839
	ldr	w0, [x21, 56]
	cbnz	w0, .L1839
	ldr	x1, .LC353
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 2]
	ldr	w0, [x21, 56]
	cbnz	w0, .L1882
	ldr	x2, .LC355
	mov	w1, 2
	mov	x0, x19
	bl	u_v
	cmp	w0, 2
	str	w0, [x20, 44]
	bhi	.L1883
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
	cbnz	w0, .L1884
.L1842:
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
	bcc	.L1863
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
	beq	.L1844
.L1862:
	ldr	x1, .LC377
	mov	x0, x19
	bl	se_v
	add	w1, w0, 12
	cmp	w1, 24
	str	w0, [x20, 56]
	bhi	.L1845
	ldr	w0, [x21, 56]
	cbz	w0, .L1863
.L1845:
	ldr	x1, .LC384
	mov	w0, 1
	bl	dprint_vfmw
.L1882:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, -1
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1863:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	mov	w0, 0
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L1884:
	ldr	x1, .LC371
	mov	w0, 1
	bl	dprint_vfmw
	b	.L1842
	.p2align 3
.L1837:
	ldr	x1, .LC347
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1833
	.p2align 3
.L1839:
	ldr	x1, .LC351
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1833
	.p2align 3
.L1844:
	ldr	x28, .LC380
	add	x22, x29, 120
	ldr	x26, .LC379
	add	x24, x20, 5
	add	x23, x20, 1984
	add	x25, x20, 64
	mov	w27, 0
	b	.L1853
	.p2align 3
.L1889:
	ldr	w0, [x20, 28]
	mov	w1, 4000
	umull	x0, w0, w1
	ldr	x1, [x19, 14960]
	add	x0, x1, x0
	ldrb	w1, [x0, 25]
	cmp	w1, 1
	beq	.L1885
	ldr	x0, .LC380
	ldr	x3, [x0, 104]
	ldr	x0, .LC334
	add	x1, x0, 96
	cbz	w27, .L1886
.L1881:
	mov	x2, 16
	mov	x0, x23
	blr	x3
.L1848:
	add	w27, w27, 1
	add	x22, x22, 4
	cmp	w27, 6
	add	x24, x24, 1
	add	x23, x23, 16
	add	x25, x25, 64
	beq	.L1887
.L1853:
	mov	x1, x26
	mov	x0, x19
	bl	u_1
	cmp	w0, 1
	strb	w0, [x24]
	str	w0, [x22]
	beq	.L1888
	cmp	w27, wzr
	ccmp	w27, 3, 4, ne
	beq	.L1889
	sub	w1, w27, #1
	ldr	x3, [x28, 104]
	add	x1, x1, 124
	add	x1, x20, x1, lsl 4
	b	.L1881
	.p2align 3
.L1887:
	ldr	x22, .LC380
	add	x26, x20, 2080
	ldr	x24, .LC334
	add	x27, x20, 448
	mov	x28, 0
	add	x23, x29, 112
	mov	w25, 4000
.L1861:
	ldr	w0, [x20, 60]
	mov	w8, w28
	cmp	w0, 1
	beq	.L1890
	add	x0, x20, x28
	str	wzr, [x23, x28, lsl 2]
	strb	wzr, [x0, 11]
.L1856:
	ldr	w0, [x20, 28]
	ldr	x1, [x19, 14960]
	umull	x0, w0, w25
	add	x0, x1, x0
	ldrb	w1, [x0, 25]
	cmp	w1, 1
	beq	.L1891
	add	x1, x24, 176
	ldr	x3, [x22, 104]
	cbz	w8, .L1892
.L1860:
	mov	x2, 64
	mov	x0, x26
	blr	x3
.L1857:
	add	x28, x28, 1
	add	x26, x26, 64
	cmp	x28, 2
	add	x27, x27, 256
	bne	.L1861
	b	.L1862
	.p2align 3
.L1888:
	mov	x0, x19
	mov	x4, x22
	mov	w3, 16
	mov	x2, x23
	mov	x1, x25
	bl	Scaling_List
	ldr	w0, [x22]
	cmp	w0, 1
	bne	.L1848
	ldr	x0, .LC334
	cmp	w27, 2
	add	x1, x0, 80
	add	x0, x0, 96
	csel	x1, x0, x1, hi
.L1880:
	ldr	x0, .LC380
	mov	x2, 16
	ldr	x3, [x0, 104]
	mov	x0, x23
	blr	x3
	b	.L1848
.L1885:
	uxtw	x1, w27
	add	x1, x0, x1, lsl 4
	add	x1, x1, 1972
	b	.L1880
.L1883:
	ldr	x1, .LC357
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1833
.L1890:
	ldr	x1, .LC382
	mov	x0, x19
	str	w28, [x29, 108]
	bl	u_1
	cmp	w0, 1
	add	x1, x20, x28
	str	w0, [x23, x28, lsl 2]
	ldr	w8, [x29, 108]
	strb	w0, [x1, 11]
	bne	.L1856
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
	bne	.L1857
	ldr	x0, .LC380
	ldr	x3, [x0, 104]
	ldr	x0, .LC334
	add	x1, x0, 176
	cbnz	w8, .L1860
	add	x1, x0, 112
	b	.L1860
	.p2align 3
.L1891:
	ubfiz	x8, x8, 6, 32
	mov	x2, 64
	add	x1, x0, x8
	ldr	x0, .LC380
	add	x1, x1, 2068
	ldr	x3, [x0, 104]
	mov	x0, x26
	blr	x3
	b	.L1857
.L1886:
	add	x1, x0, 80
	b	.L1881
.L1892:
	ldr	x1, .LC334
	add	x1, x1, 112
	b	.L1860
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
	cbz	x19, .L1915
	cbz	x20, .L1916
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
	cbz	w3, .L1897
	lsl	w10, w10, 1
	cmn	w10, #6
	beq	.L1897
	mov	x1, x20
	add	x7, x19, 64
	mov	w9, -6
	mov	x8, 0
	.p2align 2
.L1902:
	add	x3, x19, x8
	add	x0, x20, x8
	ldrsb	w3, [x3, 5]
	ldrsb	w0, [x0, 5]
	cmp	w0, w3
	cset	w0, eq
	and	w2, w2, w0
	cbz	w3, .L1898
	cmp	w8, 5
	bhi	.L1899
	sub	x0, x7, #64
	mov	x3, x1
	.p2align 2
.L1900:
	ldr	w5, [x0, 64]
	add	x0, x0, 4
	ldr	w4, [x3, 64]
	add	x3, x3, 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x7, x0
	and	w2, w2, w4
	bne	.L1900
.L1898:
	add	w9, w9, 1
	add	x8, x8, 1
	cmp	w9, w10
	add	x1, x1, 64
	add	x7, x7, 64
	bne	.L1902
.L1897:
	ldr	w0, [x20, 56]
	ldr	w1, [x19, 56]
	cmp	w1, w0
	cset	w0, eq
	tst	w0, w2
	csetm	w0, eq
.L1895:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L1899:
	ubfiz	x0, x9, 8, 32
	add	x3, x0, 448
	add	x6, x0, 704
	add	x6, x19, x6
	add	x0, x19, x3
	add	x3, x20, x3
	.p2align 2
.L1901:
	ldr	w5, [x0], 4
	ldr	w4, [x3], 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x6, x0
	and	w2, w2, w4
	bne	.L1901
	add	w9, w9, 1
	add	x8, x8, 1
	cmp	w9, w10
	add	x1, x1, 64
	add	x7, x7, 64
	bne	.L1902
	b	.L1897
.L1916:
	ldr	x2, .LC385
	mov	w3, 9727
	ldr	x1, .LC386
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1895
.L1915:
	ldr	x2, .LC385
	mov	w3, 9726
	ldr	x1, .LC386
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1895
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
	bls	.L1918
	ldr	x1, .LC390
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 14948]
	sub	w1, w0, #1
	cmp	w20, w1
	bls	.L1926
	ldr	x1, .LC391
	ldr	x4, [x1]
	cbz	x4, .L1926
	add	x19, x19, 65536
	mov	w3, 8
	stp	w20, w0, [x29, 56]
	add	x2, x29, 56
	mov	w1, 110
	ldr	w0, [x19, 116]
	blr	x4
	mov	w0, -1
.L1919:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L1918:
	ldr	x1, .LC393
	mov	x0, x19
	bl	ue_v
	mov	w2, w0
	ldr	w0, [x19, 14944]
	sub	w0, w0, #1
	cmp	w2, w0
	bhi	.L1930
	mov	w0, 4000
	ldr	x1, [x19, 14960]
	umull	x0, w2, w0
	add	x0, x1, x0
	ldrb	w0, [x0, 23]
	cbz	w0, .L1931
	mov	w21, 2208
	ldr	x0, [x19, 14968]
	umull	x21, w20, w21
	add	x0, x0, x21
	ldrb	w1, [x0, 19]
	cbz	w1, .L1922
	ldr	x22, .LC398
	mov	x0, x19
	mov	x1, x22
	stp	w20, w2, [x22, 24]
	bl	ProcessPPS
	cbnz	w0, .L1932
	ldr	x1, [x19, 14968]
	mov	x0, x22
	add	x1, x1, x21
	bl	PPSEqual
	cbz	w0, .L1919
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
	b	.L1919
	.p2align 3
.L1922:
	str	w20, [x0, 24]
	mov	x0, x19
	ldr	x1, [x19, 14968]
	add	x1, x1, x21
	str	w2, [x1, 28]
	ldr	x1, [x19, 14968]
	add	x1, x1, x21
	bl	ProcessPPS
	cbnz	w0, .L1933
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
.L1926:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	mov	w0, -1
	ldp	x29, x30, [sp], 64
	ret
.L1930:
	ldr	x1, .LC395
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1919
.L1932:
	ldr	x1, .LC400
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1919
.L1931:
	ldr	x1, .LC397
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1919
.L1933:
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
	b	.L1919
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
	cbnz	w0, .L1977
	strb	wzr, [x20, 15]
.L1936:
	ldr	x1, .LC411
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 1]
	cbnz	w0, .L1978
.L1937:
	ldr	x1, .LC415
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 3]
	cbnz	w0, .L1979
	mov	w0, 5
	str	w0, [x20, 28]
.L1939:
	ldr	x1, .LC429
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 6]
	cbnz	w0, .L1980
.L1940:
	ldr	x1, .LC435
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 8]
	cbnz	w0, .L1981
.L1942:
	ldr	x1, .LC443
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 7]
	cbz	w0, .L1944
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
	bhi	.L1947
	add	x28, x19, 65536
	ldr	w0, [x28, 56]
	cbnz	w0, .L1947
	ldr	x27, .LC453
	mov	w21, 0
	ldr	x26, .LC455
	ldr	x25, .LC457
	.p2align 2
.L1948:
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
	bcs	.L1948
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
.L1944:
	ldr	x1, .LC467
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 10]
	cbnz	w0, .L1982
	ldrb	w0, [x20, 7]
	add	x28, x19, 65536
	cbz	w0, .L1952
.L1953:
	ldr	x1, .LC469
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 14]
.L1952:
	ldr	x1, .LC471
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 11]
	ldr	x1, .LC473
	mov	x0, x19
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x20, 12]
	cbnz	w0, .L1954
	ldr	w0, [x28, 56]
.L1955:
	cmp	w0, wzr
	csetm	w0, ne
.L1976:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L1981:
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
	bne	.L1942
	ldr	w1, [x20, 52]
	cbz	w1, .L1942
	ldr	w0, [x20, 56]
	add	x2, x19, 61440
	udiv	w0, w0, w1
	lsr	w0, w0, 1
	str	w0, [x2, 2796]
	b	.L1942
	.p2align 3
.L1980:
	ldr	x1, .LC431
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 44]
	ldr	x1, .LC433
	mov	x0, x19
	bl	ue_v
	str	w0, [x20, 48]
	b	.L1940
	.p2align 3
.L1979:
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
	cbz	w0, .L1939
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
	b	.L1939
	.p2align 3
.L1978:
	ldr	x1, .LC413
	mov	x0, x19
	bl	u_1
	strb	w0, [x20, 2]
	b	.L1937
	.p2align 3
.L1977:
	ldr	x2, .LC405
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	uxtb	w0, w0
	cmp	w0, 255
	strb	w0, [x20, 15]
	bne	.L1936
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
	b	.L1936
	.p2align 3
.L1982:
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
	bhi	.L1947
	add	x28, x19, 65536
	ldr	w0, [x28, 56]
	cbnz	w0, .L1947
	ldr	x27, .LC453
	mov	w21, 0
	ldr	x26, .LC455
	ldr	x25, .LC457
	.p2align 2
.L1950:
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
	bcs	.L1950
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
	cbnz	w0, .L1953
	ldrb	w0, [x20, 10]
	cbnz	w0, .L1953
	b	.L1952
	.p2align 3
.L1954:
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
	cbz	w0, .L1955
	mov	w0, -1
	b	.L1976
.L1947:
	ldr	x1, .LC451
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1976
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
	cbz	x19, .L2015
	cbz	x20, .L2016
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
	cbz	w1, .L1987
	mov	x2, x19
	mov	x8, x20
	mov	w9, -6
	mov	x1, 0
	.p2align 2
.L1992:
	add	x4, x19, x1
	add	x3, x20, x1
	add	x6, x2, 64
	ldrsb	w3, [x3, 4]
	ldrsb	w4, [x4, 4]
	cmp	w3, w4
	cset	w3, eq
	and	w0, w0, w3
	cbz	w4, .L1988
	cmp	w1, 5
	bhi	.L1989
	mov	x3, x8
	.p2align 2
.L1990:
	ldr	w5, [x2, 52]
	add	x2, x2, 4
	ldr	w4, [x3, 52]
	add	x3, x3, 4
	cmp	w5, w4
	cset	w4, eq
	cmp	x6, x2
	and	w0, w0, w4
	bne	.L1990
.L1988:
	add	x1, x1, 1
	add	w9, w9, 1
	cmp	x1, 8
	mov	x2, x6
	add	x8, x8, 64
	bne	.L1992
.L1987:
	ldr	w4, [x19, 2196]
	ldr	w3, [x20, 2196]
	ldr	w2, [x20, 2200]
	ldr	w1, [x19, 2200]
	cmp	w4, w3
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w0, w2, w0
	cbz	w1, .L1994
	cmp	w1, 1
	bne	.L1993
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
	cbz	w1, .L1993
	sub	w5, w1, #1
	add	x2, x20, 2220
	add	x5, x5, 556
	add	x1, x19, 2220
	add	x5, x19, x5, lsl 2
	.p2align 2
.L1996:
	ldr	w4, [x1], 4
	ldr	w3, [x2], 4
	cmp	w4, w3
	cset	w3, eq
	cmp	x1, x5
	and	w0, w0, w3
	bne	.L1996
.L1993:
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
	cbnz	w3, .L1997
	ldrb	w1, [x20, 19]
	ldrb	w2, [x19, 19]
	cmp	w2, w1
	cset	w1, eq
	and	w0, w0, w1
.L1997:
	ldrb	w4, [x19, 20]
	ldrb	w3, [x20, 20]
	ldrb	w2, [x20, 21]
	ldrb	w1, [x19, 21]
	cmp	w4, w3
	ccmp	w2, w1, 0, eq
	cset	w2, eq
	and	w0, w2, w0
	cbnz	w1, .L2017
.L1998:
	ldrb	w1, [x20, 22]
	ldrb	w2, [x19, 22]
	cmp	w2, w1
	cset	w1, eq
	tst	w1, w0
	csetm	w0, eq
.L1985:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2017:
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
	b	.L1998
	.p2align 3
.L1989:
	ubfiz	x3, x9, 8, 32
	add	x4, x3, 436
	add	x7, x3, 692
	add	x7, x19, x7
	add	x3, x19, x4
	add	x4, x20, x4
	.p2align 2
.L1991:
	ldr	w6, [x3], 4
	ldr	w5, [x4], 4
	cmp	w6, w5
	cset	w5, eq
	cmp	x3, x7
	and	w0, w0, w5
	bne	.L1991
	add	x1, x1, 1
	add	x6, x2, 64
	cmp	x1, 8
	add	w9, w9, 1
	mov	x2, x6
	add	x8, x8, 64
	bne	.L1992
	b	.L1987
	.p2align 3
.L1994:
	ldr	w1, [x20, 2204]
	ldr	w2, [x19, 2204]
	cmp	w2, w1
	cset	w1, eq
	and	w0, w0, w1
	b	.L1993
.L2016:
	ldr	x2, .LC488
	mov	w3, 9960
	ldr	x1, .LC489
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1985
.L2015:
	ldr	x2, .LC488
	mov	w3, 9959
	ldr	x1, .LC489
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L1985
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
	cbz	w19, .L2029
	mov	w0, 255
	cmp	w19, 16
	ccmp	w19, w0, 4, gt
	bne	.L2029
	cmp	w20, wzr
	ccmp	w22, wzr, 4, ne
	ccmp	w19, w0, 0, eq
	beq	.L2029
	cmp	w19, 1
	beq	.L2030
	cmp	w19, w0
	beq	.L2020
	ldr	x0, .LC490
	sbfiz	x19, x19, 3, 32
	add	x0, x0, 80
	add	x1, x0, x19
	ldr	w20, [x0, x19]
	ldr	w22, [x1, 4]
.L2020:
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
	bmi	.L2040
.L2021:
	subs	w0, w2, #2263
	bmi	.L2041
	cmp	w1, w0
	mov	w19, 4
	blt	.L2019
.L2025:
	subs	w1, w2, #1820
	bmi	.L2042
	cmp	w0, w1
	mov	w19, 3
	blt	.L2019
.L2026:
	subs	w0, w2, #1365
	bmi	.L2043
.L2024:
	mov	w19, 2
	cmp	w1, w0
	csinc	w19, w19, wzr, lt
	b	.L2019
	.p2align 3
.L2029:
	mov	w19, 0
	mov	w2, 0
.L2019:
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
.L2030:
	mov	w19, 5
	mov	w2, 0
	b	.L2019
	.p2align 3
.L2041:
	mov	w0, 2263
	mov	w19, 4
	sub	w0, w0, w2
	cmp	w1, w0
	blt	.L2019
	b	.L2025
	.p2align 3
.L2040:
	mov	w1, 2406
	sub	w1, w1, w2
	b	.L2021
	.p2align 3
.L2042:
	mov	w1, 1820
	mov	w19, 3
	sub	w1, w1, w2
	cmp	w0, w1
	blt	.L2019
	b	.L2026
.L2043:
	mov	w0, 1365
	sub	w0, w0, w2
	b	.L2024
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
	cbz	x19, .L2164
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
	cbnz	w0, .L2165
	mov	w0, 1
	strb	wzr, [x19, 25]
	str	w0, [x19, 40]
.L2059:
	ldr	x1, .LC519
	mov	x0, x20
	bl	ue_v
	cmp	w0, 12
	str	w0, [x19, 2196]
	bhi	.L2072
	add	x21, x20, 65536
	ldr	w0, [x21, 56]
	cbnz	w0, .L2072
	ldr	x1, .LC523
	mov	x0, x20
	bl	ue_v
	cmp	w0, 2
	str	w0, [x19, 2200]
	bhi	.L2074
	ldr	w1, [x21, 56]
	cbnz	w1, .L2074
	cbz	w0, .L2166
	cmp	w0, 1
	beq	.L2167
.L2078:
	ldr	x1, .LC580
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3244]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2162
	ldr	x1, .LC543
	mov	x0, x20
	bl	u_1
	strb	w0, [x19, 17]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2162
	ldr	x1, .LC545
	mov	x0, x20
	bl	ue_v
	sub	w1, w0, #1
	cmp	w1, 509
	str	w0, [x19, 3248]
	bhi	.L2085
	ldr	w1, [x21, 56]
	cbnz	w1, .L2085
	ldr	x1, .LC549
	mov	x0, x20
	bl	ue_v
	str	w0, [x19, 3252]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2162
	ldr	x1, .LC551
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 18]
	ldr	w1, [x21, 56]
	cbnz	w1, .L2162
	cbnz	w0, .L2168
	ldr	x1, .LC582
	mov	x0, x20
	bl	u_1
	ldr	w23, [x19, 3252]
	strb	w0, [x19, 19]
	add	w23, w23, 1
	cmp	w23, 255
	bhi	.L2118
	ldr	w3, [x19, 3248]
	cmp	w23, 1
	mov	w24, 2
	add	w3, w3, 1
	ccmp	w3, 3, 0, hi
	cset	w0, ls
	cbz	w0, .L2089
.L2118:
	ldr	x1, .LC553
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2165:
	ldr	x1, .LC498
	mov	x0, x20
	bl	ue_v
	cmp	w0, 1
	str	w0, [x19, 40]
	bhi	.L2048
	add	x21, x20, 65536
	ldr	w0, [x21, 56]
	cbnz	w0, .L2048
	ldr	x1, .LC502
	mov	x0, x20
	bl	ue_v
	uxth	w0, w0
	add	w2, w0, 8
	str	w2, [x19, 44]
	cmp	w2, 8
	beq	.L2169
	ldr	x1, .LC504
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2051
.L2177:
	ldr	w1, [x19, 44]
	mov	w3, 8
	ldr	w0, [x21, 116]
	add	x2, x29, 120
	stp	wzr, w1, [x29, 120]
	mov	w1, 119
	blr	x4
.L2051:
	ldr	x1, .LC507
	mov	x0, x20
	bl	ue_v
	uxth	w0, w0
	add	w2, w0, 8
	str	w2, [x19, 48]
	cmp	w2, 8
	beq	.L2170
	ldr	x1, .LC509
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2054
.L2176:
	ldr	w1, [x19, 48]
	mov	w3, 8
	ldr	w0, [x21, 116]
	add	x2, x29, 120
	stp	wzr, w1, [x29, 120]
	mov	w1, 119
	blr	x4
.L2054:
	ldr	x1, .LC511
	mov	x0, x20
	bl	u_1
	cbnz	w0, .L2056
	ldr	w0, [x21, 56]
	cbnz	w0, .L2056
	ldr	x1, .LC515
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	cmp	w0, 1
	strb	w0, [x19, 25]
	bne	.L2059
	ldr	x26, .LC494
	add	x21, x29, 120
	ldr	x25, .LC516
	add	x23, x19, 4
	add	x22, x19, 1972
	add	x24, x19, 52
	mov	w27, 0
	ldr	x28, .LC517
	b	.L2065
	.p2align 3
.L2173:
	add	x0, x26, 96
	cmp	w27, wzr
	ldr	x3, [x28, 104]
	csel	x1, x0, x1, ne
	mov	x2, 16
	mov	x0, x22
	blr	x3
.L2061:
	add	w27, w27, 1
	add	x21, x21, 4
	cmp	w27, 6
	add	x23, x23, 1
	add	x22, x22, 16
	add	x24, x24, 64
	beq	.L2171
.L2065:
	mov	x1, x25
	mov	x0, x20
	bl	u_1
	strb	w0, [x23]
	ldr	x1, .LC494
	cmp	w0, 1
	str	w0, [x21]
	add	x1, x1, 80
	beq	.L2172
	cmp	w27, wzr
	ccmp	w27, 3, 4, ne
	beq	.L2173
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
	bne	.L2065
.L2171:
	ldr	x24, .LC517
	add	x21, x19, 2068
	ldr	x23, .LC494
	add	x22, x19, 436
	mov	x27, 0
	add	x26, x29, 104
	mov	x28, x24
.L2070:
	mov	x1, x25
	mov	x0, x20
	bl	u_1
	cmp	w0, 1
	add	x2, x19, x27
	str	w0, [x26, x27, lsl 2]
	add	x1, x23, 176
	strb	w0, [x2, 10]
	beq	.L2174
	ldr	x3, [x24, 104]
	cbz	w27, .L2175
.L2069:
	mov	x0, x21
	mov	x2, 64
.L2158:
	blr	x3
.L2067:
	add	x27, x27, 1
	add	x21, x21, 64
	cmp	x27, 2
	add	x22, x22, 256
	bne	.L2070
	b	.L2059
	.p2align 3
.L2170:
	ldr	w0, [x21, 56]
	cbz	w0, .L2054
	ldr	x1, .LC509
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbnz	x4, .L2176
	b	.L2054
	.p2align 3
.L2169:
	ldr	w0, [x21, 56]
	cbz	w0, .L2051
	ldr	x1, .LC504
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbnz	x4, .L2177
	b	.L2051
	.p2align 3
.L2116:
	ldr	w2, [x0, 708]
	ldr	w1, [x0, 712]
	cmp	w25, w2
	ccmp	w23, w1, 2, ls
	bls	.L2115
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2162
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
.L2162:
	mov	w0, -1
.L2154:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2168:
	ldr	w23, [x19, 3252]
	strb	wzr, [x19, 19]
	sub	w0, w23, #1
	cmp	w0, 509
	bhi	.L2118
	ldr	w3, [x19, 3248]
	mov	w24, 1
	add	w23, w23, 1
	add	w3, w3, w24
.L2089:
	mul	w23, w24, w23
	lsl	w25, w3, 4
	mov	w1, 100663296
	lsl	w23, w23, 4
	mul	w0, w25, w23
	lsr	w0, w0, 1
	lsl	w22, w0, 2
	sub	w0, w22, w0
	cmp	w0, w1
	bhi	.L2178
	ldr	w2, [x19, 32]
	sub	w1, w2, #10
	cmp	w1, 31
	bls	.L2179
.L2091:
	ldr	x1, .LC558
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2166:
	ldr	x1, .LC527
	mov	x0, x20
	bl	ue_v
	cmp	w0, 12
	str	w0, [x19, 2204]
	bhi	.L2077
	ldr	w0, [x21, 56]
	cbz	w0, .L2078
.L2077:
	ldr	x1, .LC529
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2179:
	ldr	x3, .LC556
	ldrh	w1, [x3,w1,uxtw #1]
	adr	x3, .Lrtx2093
	add	x1, x3, w1, sxth #2
	br	x1
.Lrtx2093:
	.section	.rodata
	.align	0
	.align	2
.L2093:
	.2byte	(.L2092 - .Lrtx2093) / 4
	.2byte	(.L2125 - .Lrtx2093) / 4
	.2byte	(.L2095 - .Lrtx2093) / 4
	.2byte	(.L2095 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2095 - .Lrtx2093) / 4
	.2byte	(.L2096 - .Lrtx2093) / 4
	.2byte	(.L2098 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2098 - .Lrtx2093) / 4
	.2byte	(.L2099 - .Lrtx2093) / 4
	.2byte	(.L2100 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2091 - .Lrtx2093) / 4
	.2byte	(.L2102 - .Lrtx2093) / 4
	.2byte	(.L2102 - .Lrtx2093) / 4
	.text
	.p2align 3
.L2095:
	mov	w22, 60416
	movk	w22, 0xd, lsl 16
.L2094:
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
	bhi	.L2103
	ldr	w0, [x21, 56]
	cbz	w0, .L2104
.L2103:
	ldr	x1, .LC562
	mov	w3, w22
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 3244]
	cmp	w0, 16
	bhi	.L2159
	cmp	w22, w0
	csel	w22, w22, w0, cs
.L2159:
	str	w22, [x19, 3244]
.L2104:
	ldr	x1, .LC564
	mov	x0, x20
	bl	u_1
	strb	w0, [x19, 20]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2162
	ldr	x1, .LC566
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 21]
	ldr	w1, [x21, 56]
	cbnz	w1, .L2162
	cbnz	w0, .L2180
.L2106:
	add	x2, x19, 3072
	lsr	w1, w25, 1
	lsr	w0, w23, 1
	stp	w25, w23, [x2, 204]
	stp	w1, w0, [x2, 212]
.L2108:
	ldr	x1, .LC576
	mov	x0, x20
	bl	u_1
	uxtb	w0, w0
	strb	w0, [x19, 22]
	ldr	w1, [x21, 56]
	cbnz	w1, .L2162
	mov	w1, 2
	str	w1, [x19, 3332]
	cbnz	w0, .L2181
	mov	w0, 5
	strb	wzr, [x19, 3307]
	str	w0, [x19, 3320]
	mov	w0, 0
.L2111:
	ldr	w1, [x19, 3308]
	mov	w4, w23
	ldr	w2, [x19, 3312]
	mov	w3, w25
	bl	H264GetDar
	str	w0, [x19, 3316]
	ldr	w1, [x19, 3244]
	cbz	w1, .L2114
	cmp	w22, w1
	csel	w22, w22, w1, ls
.L2114:
	cmp	w22, 16
	mov	w1, 16
	ldr	x0, [x20]
	csel	w22, w22, w1, ls
	str	w22, [x20, 14952]
	ldr	w1, [x0, 28]
	cmp	w1, 25
	beq	.L2182
.L2115:
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
.L2102:
	mov	w22, 12582912
	b	.L2094
	.p2align 3
.L2098:
	mov	w22, 30208
	movk	w22, 0x2f, lsl 16
	b	.L2094
	.p2align 3
.L2172:
	mov	x0, x20
	mov	x4, x21
	mov	x2, x22
	mov	x1, x24
	mov	w3, 16
	bl	Scaling_List
	ldr	w0, [x21]
	cmp	w0, 1
	bne	.L2061
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
	b	.L2061
	.p2align 3
.L2048:
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
.L2072:
	ldr	x1, .LC521
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2180:
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
	ble	.L2106
	add	x4, x19, 3072
	add	w2, w2, w3, lsr 1
	add	w1, w1, w0, lsr 1
	stp	w3, w0, [x4, 204]
	stp	w2, w1, [x4, 212]
	b	.L2108
	.p2align 3
.L2181:
	add	x1, x19, 3292
	mov	x0, x20
	bl	vui_parameters
	cbnz	w0, .L2161
	ldrb	w0, [x19, 3304]
	cbz	w0, .L2161
	ldr	w2, [x19, 3372]
	cmp	w2, wzr
	ccmp	w22, w2, 0, ne
	bcc	.L2183
	ldr	w22, [x19, 3244]
	ldrb	w0, [x19, 3307]
	cmp	w22, w2
	csel	w22, w22, w2, cs
	b	.L2111
.L2183:
	ldr	x1, .LC578
	mov	w3, w22
	mov	w0, 1
	bl	dprint_vfmw
	.p2align 2
.L2161:
	ldrb	w0, [x19, 3307]
	b	.L2111
	.p2align 3
.L2182:
	ldr	w1, [x0, 748]
	cbnz	w1, .L2115
	ldr	w1, [x0, 732]
	cmp	w22, w1
	bls	.L2116
	ldr	x0, .LC505
	ldr	x4, [x0]
	cbz	x4, .L2162
	ldr	w0, [x21, 116]
	mov	w3, 8
	stp	w22, w1, [x29, 112]
	add	x2, x29, 112
	mov	w1, 106
	blr	x4
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2056:
	ldr	x1, .LC513
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2167:
	ldr	x1, .LC531
	mov	x0, x20
	bl	u_1
	strb	w0, [x19, 16]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2162
	ldr	x1, .LC533
	mov	x0, x20
	bl	se_v
	str	w0, [x19, 2208]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2162
	ldr	x1, .LC535
	mov	x0, x20
	bl	se_v
	str	w0, [x19, 2212]
	ldr	w0, [x21, 56]
	cbnz	w0, .L2162
	ldr	x1, .LC537
	mov	x0, x20
	bl	ue_v
	cmp	w0, 255
	str	w0, [x19, 2216]
	bhi	.L2081
	ldr	w1, [x21, 56]
	cbnz	w1, .L2081
	cbz	w0, .L2078
	ldr	x23, .LC541
	mov	w22, 0
	b	.L2084
	.p2align 3
.L2184:
	ldr	w0, [x19, 2216]
	cmp	w0, w22
	bls	.L2078
.L2084:
	mov	x1, x23
	mov	x0, x20
	bl	se_v
	add	x1, x19, x22, uxtw 2
	add	w22, w22, 1
	str	w0, [x1, 2220]
	ldr	w0, [x21, 56]
	cbz	w0, .L2184
	b	.L2162
	.p2align 3
.L2085:
	ldr	x1, .LC547
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2125:
	mov	w22, 17920
	movk	w22, 0x5, lsl 16
	b	.L2094
	.p2align 3
.L2099:
	mov	w22, 30720
	movk	w22, 0x69, lsl 16
	b	.L2094
	.p2align 3
.L2096:
	mov	w22, 55296
	movk	w22, 0x1b, lsl 16
	b	.L2094
	.p2align 3
.L2092:
	mov	w22, 20992
	movk	w22, 0x2, lsl 16
	b	.L2094
	.p2align 3
.L2100:
	mov	w22, 7864320
	b	.L2094
.L2081:
	ldr	x1, .LC539
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
	.p2align 3
.L2174:
	mov	x0, x20
	add	x4, x26, x27, lsl 2
	mov	x2, x21
	mov	x1, x22
	mov	w3, 64
	bl	Scaling_List
	ldr	w0, [x26, x27, lsl 2]
	cmp	w0, 1
	bne	.L2067
	ldr	x1, .LC494
	mov	x2, 64
	mov	x0, x21
	ldr	x3, [x28, 104]
	add	x1, x1, 176
	cbnz	w27, .L2158
	ldr	x1, .LC494
	add	x1, x1, 112
	b	.L2158
	.p2align 3
.L2074:
	ldr	x1, .LC525
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
.L2178:
	ldr	x1, .LC555
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
.L2164:
	ldr	x2, .LC495
	mov	w3, 10150
	ldr	x1, .LC496
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2154
.L2175:
	ldr	x1, .LC494
	add	x1, x1, 112
	b	.L2069
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
	.xword	.L2093
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
	beq	.L2196
	ldr	w0, [x20, 40]
	sub	w1, w0, #1
	cmp	w1, 1
	bls	.L2197
.L2188:
	cmp	w0, 1
	beq	.L2198
.L2189:
	ldr	x1, .LC600
	mov	x0, x19
	bl	u_1
	cbnz	w0, .L2199
.L2190:
	ldr	x1, .LC606
	mov	x0, x19
	mov	w19, 0
	bl	u_1
	cmp	w0, 1
	beq	.L2187
	ldr	x1, .LC608
	mov	w2, w0
	ldr	w4, [x20, 40]
	mov	w3, w21
	mov	w0, 1
	bl	dprint_vfmw
.L2187:
	mov	w0, w19
	ldr	x21, [sp, 32]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2199:
	ldr	x1, .LC602
	mov	x0, x19
	bl	u_1
	mov	w2, w0
	ldr	x1, .LC604
	mov	w0, 17
	bl	dprint_vfmw
	b	.L2190
	.p2align 3
.L2198:
	ldr	x2, .LC596
	mov	w1, 2
	mov	x0, x19
	bl	u_v
	mov	w2, w0
	ldr	x1, .LC598
	mov	w0, 17
	bl	dprint_vfmw
	b	.L2189
	.p2align 3
.L2197:
	ldr	x1, .LC592
	mov	x0, x19
	bl	u_1
	mov	w2, w0
	ldr	x1, .LC594
	mov	w0, 17
	bl	dprint_vfmw
	ldr	w0, [x20, 40]
	b	.L2188
.L2196:
	ldr	x1, .LC590
	mov	w19, -1
	bl	dprint_vfmw
	b	.L2187
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
	cbnz	w0, .L2229
	ldr	w0, [x19, 14944]
	sub	w0, w0, #1
	cmp	w27, w0
	bhi	.L2230
	sub	w0, w21, #66
	mov	w22, w21
	cmp	w0, 78
	bls	.L2231
.L2205:
	ldr	x1, .LC635
	mov	w2, w21
	mov	w0, 1
	mov	w22, 100
	bl	dprint_vfmw
.L2208:
	cmp	w20, 9
	mov	w2, w20
	bls	.L2228
	cmp	w20, 41
	bhi	.L2228
	mov	w20, 4000
	ldr	x0, [x19, 14960]
	umull	x20, w27, w20
	add	x0, x0, x20
	ldrb	w1, [x0, 23]
	cbz	w1, .L2213
.L2234:
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
	cbnz	w0, .L2232
	ldr	x1, [x19, 14960]
	mov	x0, x21
	add	x1, x1, x20
	bl	SPSEqual
	cbnz	w0, .L2233
.L2215:
	mov	w0, 86
	cmp	w22, 83
	ccmp	w22, w0, 4, ne
	beq	.L2217
.L2218:
	mov	w0, 0
.L2202:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L2231:
	ldr	x1, .LC627
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx2207
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2207:
	.section	.rodata
	.align	0
	.align	2
.L2207:
	.byte	(.L2206 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2208 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2208 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2208 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2209 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2208 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2210 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2210 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2205 - .Lrtx2207) / 4
	.byte	(.L2210 - .Lrtx2207) / 4
	.text
	.p2align 3
.L2217:
	mov	x0, x19
	bl	Sps_SvcExt
	cbz	w0, .L2218
	ldr	x1, .LC643
	mov	w0, 1
	bl	dprint_vfmw
.L2229:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	mov	w0, -1
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L2228:
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
	cbnz	w1, .L2234
.L2213:
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
	cbnz	w0, .L2235
	ldr	x0, [x19, 14960]
	mov	w1, 1
	add	x0, x0, x20
	strb	w1, [x0, 24]
	ldr	x0, [x19, 14960]
	add	x20, x0, x20
	strb	w1, [x20, 23]
	b	.L2215
	.p2align 3
.L2230:
	ldr	x1, .LC625
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 14944]
	sub	w0, w1, #1
	cmp	w27, w0
	bls	.L2229
	ldr	x0, .LC626
	ldr	x4, [x0]
	cbz	x4, .L2229
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
.L2209:
	ldr	x1, .LC631
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2208
	.p2align 3
.L2210:
	ldr	x1, .LC633
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2208
	.p2align 3
.L2206:
	ldr	x1, .LC629
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2208
	.p2align 3
.L2233:
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
	b	.L2215
	.p2align 3
.L2232:
	ldr	x1, .LC640
	mov	w3, w27
	mov	w2, 10776
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2202
.L2235:
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
	b	.L2202
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
	.xword	.L2207
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
	beq	.L2245
	ldrb	w2, [x0]
	sbfiz	x1, x2, 5, 32
	add	x1, x0, x1
	ldr	x1, [x1, 8]
	cbz	x1, .L2245
	ldr	w1, [x0, 76]
	cmp	w2, 1
	ccmp	w1, 2, 2, ls
	bhi	.L2245
	cmp	w1, w2
	bls	.L2245
	uxtw	x5, w2
	add	x3, x0, x5, lsl 5
	ldr	w1, [x3, 28]
	ldr	w3, [x3, 16]
	add	w1, w1, w19, lsl 3
	lsl	w3, w3, 3
	cmp	w1, w3
	bhi	.L2240
	b	.L2238
	.p2align 3
.L2241:
	ldr	w4, [x6, 28]
	ldr	w6, [x6, 16]
	sub	w4, w4, w3
	add	w1, w1, w4
	lsl	w3, w6, 3
	cmp	w1, w3
	bls	.L2238
.L2240:
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
	bhi	.L2241
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2238:
	add	x5, x0, x5, lsl 5
	mov	w0, 1
	str	w1, [x5, 28]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2245:
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
	cbz	x19, .L2261
	ldr	x1, [x21, 632]
	cmp	x1, xzr
	ccmp	w20, wzr, 4, ne
	ble	.L2261
	ldrb	w2, [x1]
	sbfiz	x0, x2, 5, 32
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cbz	x0, .L2261
	ldr	w0, [x1, 76]
	cmp	w2, 1
	ccmp	w0, 2, 2, ls
	bhi	.L2261
	cmp	w0, w2
	bls	.L2261
	mov	w6, 0
.L2256:
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
	bcs	.L2251
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
	bcc	.L2254
	b	.L2253
	.p2align 3
.L2255:
	ldrb	w1, [x5], 1
	add	w6, w6, 1
	strb	w1, [x19], 1
	cmp	w6, w20
	ldr	x3, [x21, 632]
	add	x3, x3, x7
	ldr	w4, [x3, 28]
	add	w4, w4, 8
	str	w4, [x3, 28]
	bcs	.L2253
.L2254:
	cmp	x8, x5
	bne	.L2255
	ldr	x1, [x21, 632]
.L2251:
	ldrb	w0, [x1]
	add	w2, w2, 1
	add	w0, w0, 1
	strb	w0, [x1]
	ldr	x1, [x21, 632]
	ldr	w0, [x1, 76]
	cmp	w0, w2
	bhi	.L2256
	mov	w0, w6
	b	.L2250
	.p2align 3
.L2253:
	mov	w0, w20
.L2250:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2261:
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
	cbz	w0, .L2269
.L2264:
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
.L2269:
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
	cbnz	w1, .L2265
	ldr	w0, [x20, 16]
	cmp	w0, 5
	beq	.L2265
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
.L2265:
	ldr	x2, .LC676
	mov	w1, 8
	mov	x0, x19
	bl	u_v
	str	w0, [x21, 3220]
	ldr	x1, .LC678
	mov	x0, x19
	bl	ue_v
	str	w0, [x21, 3224]
	b	.L2264
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
	bne	.L2271
	ldr	w5, [x3, 3256]
	add	w5, w5, 2
	str	w5, [x3, 3252]
.L2271:
	cmp	w6, 1
	add	x4, x3, 3240
	beq	.L2278
	ldrb	w1, [x4, 81]
	cmp	w1, 1
	ldr	x1, [x4, 48]
	add	x1, x1, x5, uxtw
	str	x1, [x4, 40]
	beq	.L2279
.L2273:
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
.L2278:
	mov	x1, 0
	str	xzr, [x3, 3280]
	str	xzr, [x3, 3288]
	b	.L2273
	.p2align 3
.L2279:
	str	x1, [x4, 48]
	b	.L2273
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
	beq	.L2285
.L2280:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2285:
	ldr	x0, [x19]
	ldr	x0, [x0, 56]
	cmn	x0, #1
	beq	.L2280
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
	cbz	w4, .L2289
	mov	w1, w4
	mov	w2, w5
	b	.L2288
	.p2align 3
.L2290:
	mov	w1, w0
.L2288:
	udiv	w0, w2, w1
	msub	w0, w0, w1, w2
	mov	w2, w1
	cbnz	w0, .L2290
.L2287:
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
.L2289:
	mov	w1, w5
	b	.L2287
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
	cbnz	w0, .L2292
	ldr	w1, [x20, 14944]
	cmp	w1, wzr
	ble	.L2293
	ldrb	w0, [x3, 23]
	cbnz	w0, .L2311
	sub	w1, w1, #1
	add	x0, x3, 4000
	add	x2, x1, 1
	lsl	x1, x2, 6
	sub	x1, x1, x2
	lsl	x1, x1, 1
	sub	x1, x1, x2
	add	x2, x3, x1, lsl 5
	b	.L2296
	.p2align 3
.L2297:
	ldrb	w1, [x1]
	cbnz	w1, .L2294
.L2296:
	cmp	x0, x2
	mov	x19, x0
	add	x0, x0, 4000
	sub	x1, x0, #3977
	bne	.L2297
.L2293:
	ldr	x1, .LC682
	mov	w0, 1
	mov	w19, -1
	bl	dprint_vfmw
.L2298:
	mov	w0, w19
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L2311:
	mov	x19, x3
	.p2align 2
.L2294:
	cbz	x19, .L2293
.L2292:
	ldrb	w0, [x19, 22]
	cbz	w0, .L2299
	ldrb	w0, [x19, 3299]
	cbz	w0, .L2312
	ldr	w1, [x19, 3676]
	ldr	w22, [x19, 3680]
	add	w1, w1, 1
	add	w22, w22, 1
.L2302:
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
.L2299:
	ldrb	w0, [x19, 3303]
	mov	w19, 0
	cbz	w0, .L2298
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
.L2312:
	ldrb	w0, [x19, 3302]
	cbz	w0, .L2299
	ldr	w1, [x19, 3988]
	ldr	w22, [x19, 3992]
	add	w1, w1, 1
	add	w22, w22, 1
	b	.L2302
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
	b	.L2316
	.p2align 3
.L2541:
	mov	w1, 8
	mov	x0, x19
	bl	BsSkip
	cmp	w23, wzr
	ble	.L2315
	ldrb	w0, [x29, 143]
	cmp	w0, 255
	bne	.L2540
.L2316:
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
	bls	.L2541
.L2315:
	ldr	x1, .LC690
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2318
	.p2align 3
.L2540:
	ldr	x26, .LC696
	mov	w27, 0
	ldr	x24, .LC693
	add	x0, x26, 216
	str	x0, [x29, 112]
	.p2align 2
.L2496:
	mov	w19, 0
	add	x23, x21, 2600
	b	.L2322
	.p2align 3
.L2543:
	mov	w1, 8
	mov	x0, x23
	bl	BsSkip
	cmp	w25, wzr
	ble	.L2320
	ldrb	w0, [x29, 143]
	cmp	w0, 255
	bne	.L2542
.L2322:
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
	bls	.L2543
.L2320:
	ldr	x1, .LC692
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
.L2318:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2542:
	cmp	w19, 409600
	bgt	.L2320
	ldr	x0, [x28, 632]
	ldr	w1, [x0, 76]
	ldr	w2, [x0, 28]
	cmp	w1, 2
	beq	.L2544
.L2324:
	cmp	w22, 45
	bls	.L2545
.L2325:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2546
.L2486:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2489
	.p2align 3
.L2490:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2489:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2490
	.p2align 2
.L2536:
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
.L2352:
	cmp	w0, 3
	bhi	.L2547
.L2491:
	mov	w22, 0
	add	x19, x21, 2600
	b	.L2495
	.p2align 3
.L2493:
	mov	w1, 8
	mov	x0, x19
	bl	BsSkip
	cmp	w23, wzr
	ble	.L2494
	ldrb	w0, [x29, 143]
	cmp	w0, 255
	bne	.L2548
.L2495:
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
	bls	.L2493
.L2494:
	mov	w0, 0
.L2549:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 144
	ret
	.p2align 3
.L2548:
	cmp	w0, 128
	bne	.L2496
	mov	w0, 0
	b	.L2549
	.p2align 3
.L2545:
	ldrh	w0, [x24,w22,uxtw #1]
	adr	x1, .Lrtx2327
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx2327:
	.section	.rodata
	.align	0
	.align	2
.L2327:
	.2byte	(.L2326 - .Lrtx2327) / 4
	.2byte	(.L2328 - .Lrtx2327) / 4
	.2byte	(.L2329 - .Lrtx2327) / 4
	.2byte	(.L2330 - .Lrtx2327) / 4
	.2byte	(.L2331 - .Lrtx2327) / 4
	.2byte	(.L2332 - .Lrtx2327) / 4
	.2byte	(.L2333 - .Lrtx2327) / 4
	.2byte	(.L2334 - .Lrtx2327) / 4
	.2byte	(.L2335 - .Lrtx2327) / 4
	.2byte	(.L2336 - .Lrtx2327) / 4
	.2byte	(.L2337 - .Lrtx2327) / 4
	.2byte	(.L2338 - .Lrtx2327) / 4
	.2byte	(.L2339 - .Lrtx2327) / 4
	.2byte	(.L2340 - .Lrtx2327) / 4
	.2byte	(.L2341 - .Lrtx2327) / 4
	.2byte	(.L2342 - .Lrtx2327) / 4
	.2byte	(.L2343 - .Lrtx2327) / 4
	.2byte	(.L2344 - .Lrtx2327) / 4
	.2byte	(.L2345 - .Lrtx2327) / 4
	.2byte	(.L2346 - .Lrtx2327) / 4
	.2byte	(.L2347 - .Lrtx2327) / 4
	.2byte	(.L2348 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2325 - .Lrtx2327) / 4
	.2byte	(.L2349 - .Lrtx2327) / 4
	.text
.L2349:
	mov	w1, w19
	mov	x0, x28
	ldr	w23, [x28, 14920]
	bl	H264_DecFramePackingSEI
	ldr	w3, [x28, 14920]
	sub	w0, w3, w23
	lsl	w23, w19, 3
	cmp	w0, w23
	bge	.L2479
	sub	w26, w23, w0
	ldr	w0, [x28, 14912]
	cmp	w26, wzr
	add	w22, w26, 7
	csel	w22, w22, w26, lt
	lsl	w0, w0, 3
	asr	w22, w22, 3
	cmp	w22, wzr
	ble	.L2481
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2481
	add	x25, x21, 2600
	mov	w23, 0
	b	.L2482
	.p2align 3
.L2483:
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2481
.L2482:
	mov	w1, 8
	mov	x0, x25
	add	w23, w23, 1
	bl	BsSkip
	cmp	w23, w22
	bne	.L2483
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
.L2481:
	asr	w1, w26, 31
	lsr	w2, w1, 29
	add	w1, w26, w2
	and	w1, w1, 7
	sub	w1, w1, w2
	add	w3, w1, w3
	cmp	w3, w0
	bls	.L2550
.L2479:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2536
	ldr	x1, .LC698
	mov	w3, 12065
	ldr	x2, .LC754
	mov	w0, 20
	bl	dprint_vfmw
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
	b	.L2352
.L2348:
	ldr	x1, .LC751
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2551
.L2473:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2476
	.p2align 3
.L2477:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2476:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2477
	b	.L2536
.L2347:
	ldr	x1, .LC749
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2552
.L2468:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2471
	.p2align 3
.L2472:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2471:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2472
	b	.L2536
.L2344:
	ldr	x1, .LC743
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2553
.L2453:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2456
	.p2align 3
.L2457:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2456:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2457
	b	.L2536
.L2343:
	ldr	x1, .LC741
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2554
.L2448:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2451
	.p2align 3
.L2452:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2451:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2452
	b	.L2536
.L2342:
	ldr	x1, .LC739
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2555
.L2443:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2446
	.p2align 3
.L2447:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2446:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2447
	b	.L2536
.L2341:
	ldr	x1, .LC737
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2556
.L2438:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2441
	.p2align 3
.L2442:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2441:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2442
	b	.L2536
.L2346:
	ldr	x1, .LC747
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2557
.L2463:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2466
	.p2align 3
.L2467:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2466:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2467
	b	.L2536
.L2345:
	ldr	x1, .LC745
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2558
.L2458:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2461
	.p2align 3
.L2462:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2461:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2462
	b	.L2536
.L2340:
	ldr	x1, .LC735
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2559
.L2433:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2436
	.p2align 3
.L2437:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2436:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2437
	b	.L2536
.L2339:
	ldr	x1, .LC733
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2560
.L2428:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2431
	.p2align 3
.L2432:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2431:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2432
	b	.L2536
.L2338:
	ldr	x1, .LC731
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2561
.L2423:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2426
	.p2align 3
.L2427:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2426:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2427
	b	.L2536
.L2337:
	ldr	x1, .LC729
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2562
.L2418:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2421
	.p2align 3
.L2422:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2421:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2422
	b	.L2536
.L2336:
	ldr	x1, .LC727
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2563
.L2413:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2416
	.p2align 3
.L2417:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2416:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2417
	b	.L2536
.L2335:
	ldr	x1, .LC725
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2564
.L2408:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2411
	.p2align 3
.L2412:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2411:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2412
	b	.L2536
.L2334:
	ldr	x1, .LC723
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2565
.L2403:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2406
	.p2align 3
.L2407:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2406:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2407
	b	.L2536
.L2333:
	ldr	x1, .LC721
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	cmp	w19, wzr
	ble	.L2400
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2400
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2401
	.p2align 3
.L2402:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2400
.L2401:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2402
.L2400:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2536
	ldr	x2, .LC696
	mov	w3, 11637
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
	b	.L2352
.L2332:
	cbnz	w27, .L2331
	ldr	x1, .LC708
	add	x22, x28, 61440
	mov	w3, w19
	mov	w0, 20
	add	x25, x28, 65536
	bl	dprint_vfmw
	ldr	w23, [x22, 2800]
	cmp	w23, 3
	bhi	.L2566
.L2376:
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
	cbz	x7, .L2499
	ldr	x3, [x28, 632]
	cmp	w19, 1024
	mov	w6, 1024
	csel	w26, w19, w6, le
	ldrb	w2, [x3]
	ldr	w5, [x3, 76]
	cmp	w2, w5
	bcs	.L2500
	mov	w23, 0
	.p2align 2
.L2378:
	ubfiz	x1, x2, 5, 32
	add	w2, w2, 1
	add	x1, x3, x1
	cmp	w2, w5
	ldr	w0, [x1, 28]
	ldr	w1, [x1, 16]
	add	w0, w0, 7
	sub	w0, w1, w0, lsr 3
	add	w23, w23, w0
	bne	.L2378
	mov	w0, w23
.L2377:
	cmp	w26, w0
	csel	w23, w23, w26, gt
	cbnz	w27, .L2567
	mov	w3, 0
	mov	w5, 0
.L2380:
	sub	w2, w23, w3
	cmp	w2, wzr
	ble	.L2568
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
	ble	.L2569
	sub	w0, w26, w5
	cmp	w23, w0
	bge	.L2386
	ldr	w0, [x22, 2800]
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	w23, [x0, 1052]
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2387
	add	x19, x21, 2600
	mov	w26, 0
	b	.L2388
	.p2align 3
.L2391:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2387
.L2388:
	mov	w1, 8
	mov	x0, x19
	add	w26, w26, 1
	bl	BsSkip
	cmp	w23, w26
	bne	.L2391
.L2387:
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
	cbz	x5, .L2390
	ldr	w1, [x22, 2800]
	mov	w3, 1064
	ldr	w0, [x25, 116]
	add	x1, x1, 4096
	add	x1, x28, x1, lsl 3
	ldr	x2, [x1, 31480]
	mov	w1, 6
	blr	x5
.L2390:
	ldr	w0, [x22, 2800]
	add	w0, w0, 1
	str	w0, [x22, 2800]
	cmp	w0, 3
	bls	.L2491
.L2547:
	ldr	x1, .LC753
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2318
.L2328:
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
	bge	.L2357
	sub	w23, w23, w0
	ldr	w0, [x28, 14912]
	cmp	w23, wzr
	add	w22, w23, 7
	csel	w22, w22, w23, lt
	lsl	w0, w0, 3
	asr	w22, w22, 3
	cmp	w22, wzr
	ble	.L2359
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2359
	add	x25, x21, 2600
	mov	w26, 0
	b	.L2360
	.p2align 3
.L2361:
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
	add	w1, w3, 8
	cmp	w1, w0
	bhi	.L2359
.L2360:
	mov	w1, 8
	mov	x0, x25
	add	w26, w26, 1
	bl	BsSkip
	cmp	w26, w22
	bne	.L2361
	ldr	w0, [x28, 14912]
	ldr	w3, [x28, 14920]
	lsl	w0, w0, 3
.L2359:
	asr	w1, w23, 31
	lsr	w2, w1, 29
	add	w1, w23, w2
	and	w1, w1, 7
	sub	w1, w1, w2
	add	w3, w1, w3
	cmp	w3, w0
	bls	.L2570
.L2357:
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2536
	ldr	x2, .LC696
	mov	w3, 11385
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	add	x0, x28, 61440
	ldr	w0, [x0, 2800]
	b	.L2352
.L2326:
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
	tbnz	w0, #31, .L2571
.L2350:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2354
	.p2align 3
.L2355:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2354:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2355
	b	.L2536
.L2330:
	ldr	x1, .LC704
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2572
.L2369:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2372
	.p2align 3
.L2373:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2372:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2373
	b	.L2536
.L2329:
	ldr	x1, .LC702
	mov	w3, w19
	mov	w0, 20
	bl	dprint_vfmw
	mov	w1, w19
	mov	x0, x28
	bl	PassBytes
	tbnz	w0, #31, .L2573
.L2364:
	cmp	w19, wzr
	ble	.L2536
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
	add	x22, x21, 2600
	mov	w23, 0
	b	.L2367
	.p2align 3
.L2368:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2536
.L2367:
	mov	w1, 8
	mov	x0, x22
	add	w23, w23, 1
	bl	BsSkip
	cmp	w19, w23
	bne	.L2368
	b	.L2536
.L2331:
	ldr	x1, .LC706
	add	x22, x28, 61440
	mov	w3, w19
	mov	w0, 20
	mov	w27, 1
	add	x25, x28, 65536
	bl	dprint_vfmw
	ldr	w23, [x22, 2800]
	cmp	w23, 3
	bls	.L2376
.L2566:
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
	b	.L2376
	.p2align 3
.L2544:
	ldr	w0, [x0, 60]
	add	w2, w2, w0
	lsr	w2, w2, 3
	b	.L2324
.L2386:
	ldr	w0, [x22, 2800]
	sub	w23, w19, w5
	cmp	w26, w19
	add	x0, x0, 4096
	add	x0, x28, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	w23, [x0, 1052]
	bcc	.L2574
.L2393:
	cmp	w23, wzr
	ble	.L2387
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2387
	add	x19, x21, 2600
	mov	w26, 0
	b	.L2395
	.p2align 3
.L2396:
	ldr	w1, [x28, 14912]
	ldr	w0, [x28, 14920]
	add	w0, w0, 8
	cmp	w0, w1, lsl 3
	bhi	.L2387
.L2395:
	mov	w1, 8
	mov	x0, x19
	add	w26, w26, 1
	bl	BsSkip
	cmp	w23, w26
	bne	.L2396
	b	.L2387
.L2550:
	add	x0, x21, 2600
	bl	BsSkip
	b	.L2479
.L2570:
	add	x0, x21, 2600
	bl	BsSkip
	b	.L2357
.L2567:
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
	beq	.L2381
	strb	wzr, [x3, 1028]
	mov	w3, 3
	mov	w5, w3
.L2382:
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
	bne	.L2380
	b	.L2387
.L2574:
	sub	w1, w19, w26
	mov	x0, x28
	bl	PassBytes
	tbz	w0, #31, .L2393
	ldr	x2, .LC696
	mov	w3, 11584
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2393
.L2500:
	mov	w0, 0
	mov	w23, 0
	b	.L2377
.L2565:
	ldr	x2, .LC696
	mov	w3, 11649
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2403
.L2573:
	ldr	x2, .LC696
	mov	w3, 11397
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2364
.L2572:
	ldr	x2, .LC696
	mov	w3, 11423
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2369
.L2571:
	ldr	x2, .LC696
	mov	w3, 11332
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2350
.L2546:
	ldr	x1, .LC698
	mov	w3, 12077
	ldr	x2, [x29, 112]
	mov	w0, 20
	bl	dprint_vfmw
	b	.L2486
.L2552:
	ldr	x2, .LC696
	mov	w3, 11987
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2468
.L2551:
	ldr	x1, .LC698
	mov	w3, 12013
	ldr	x2, .LC754
	mov	w0, 20
	bl	dprint_vfmw
	b	.L2473
.L2556:
	ldr	x2, .LC696
	mov	w3, 11831
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2438
.L2555:
	ldr	x2, .LC696
	mov	w3, 11857
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2443
.L2554:
	ldr	x2, .LC696
	mov	w3, 11883
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2448
.L2553:
	ldr	x2, .LC696
	mov	w3, 11909
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2453
.L2564:
	ldr	x2, .LC696
	mov	w3, 11675
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2408
.L2563:
	ldr	x2, .LC696
	mov	w3, 11701
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2413
.L2562:
	ldr	x2, .LC696
	mov	w3, 11727
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2418
.L2561:
	ldr	x2, .LC696
	mov	w3, 11753
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2423
.L2560:
	ldr	x2, .LC696
	mov	w3, 11779
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2428
.L2559:
	ldr	x2, .LC696
	mov	w3, 11805
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2433
.L2558:
	ldr	x2, .LC696
	mov	w3, 11935
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2458
.L2557:
	ldr	x2, .LC696
	mov	w3, 11961
	ldr	x1, .LC698
	mov	w0, 20
	add	x2, x2, 216
	bl	dprint_vfmw
	b	.L2463
.L2568:
	ldr	x1, .LC717
	mov	w0, 1
	bl	dprint_vfmw
.L2537:
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
	b	.L2318
.L2381:
	ldr	x2, .LC712
	mov	w1, 8
	mov	x0, x28
	str	x3, [x29, 120]
	bl	u_v
	ldr	x3, [x29, 120]
	strb	w0, [x3, 1028]
	mov	w3, 4
	mov	w5, w3
	b	.L2382
.L2569:
	ldr	x1, .LC719
	mov	w0, 1
	bl	dprint_vfmw
	b	.L2537
.L2499:
	mov	w0, -1
	b	.L2318
	.size	H264_DecSEI, .-H264_DecSEI
	.align	3
.LC690:
	.xword	.LC689
	.align	3
.LC692:
	.xword	.LC691
	.align	3
.LC693:
	.xword	.L2327
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
	bls	.L2585
.L2584:
	mov	w0, -1
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2585:
	ldr	x1, .LC755
	ldrb	w0, [x1,w0,uxtw]
	adr	x1, .Lrtx2579
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2579:
	.section	.rodata
	.align	0
	.align	2
.L2579:
	.byte	(.L2578 - .Lrtx2579) / 4
	.byte	(.L2584 - .Lrtx2579) / 4
	.byte	(.L2584 - .Lrtx2579) / 4
	.byte	(.L2584 - .Lrtx2579) / 4
	.byte	(.L2578 - .Lrtx2579) / 4
	.byte	(.L2584 - .Lrtx2579) / 4
	.byte	(.L2580 - .Lrtx2579) / 4
	.byte	(.L2580 - .Lrtx2579) / 4
	.text
	.p2align 3
.L2580:
	ldr	w0, [x19]
	mov	w1, 1
	str	w1, [x19]
	cmp	w0, wzr
	csetm	w0, ne
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2578:
	ldr	w0, [x19]
	cbz	w0, .L2581
.L2583:
	mov	w0, -1
	str	wzr, [x19]
.L2586:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2581:
	ldr	w20, [x20, 4]
	rev	w20, w20
	mov	w0, w20
	bl	ZerosMS_32
	cmp	w0, 15
	bhi	.L2583
	mov	w1, 31
	str	wzr, [x19]
	sub	w0, w1, w0, lsl 1
	lsr	w20, w20, w0
	cmp	w20, 1
	csetm	w0, ne
	b	.L2586
	.size	H264_IsNewPicNal, .-H264_IsNewPicNal
	.align	3
.LC755:
	.xword	.L2579
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
	ble	.L2607
	and	w1, w23, 63
	add	w0, w23, 63
	cmp	w1, wzr
	cset	w26, ne
	cmp	w23, wzr
	csel	w0, w0, w23, lt
	add	w26, w26, w0, asr 6
	cmp	w26, wzr
	ble	.L2600
	ldr	x20, .LC759
	mov	w25, w23
	ldr	x27, .LC758
	mov	w19, 0
	mov	w24, 64
	add	x20, x20, 1856
	.p2align 2
.L2599:
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
	cbnz	w1, .L2591
	cmp	x0, x20
	add	w19, w19, 1
	bne	.L2594
	b	.L2592
	.p2align 3
.L2595:
	add	w19, w19, 1
	beq	.L2592
.L2594:
	ldrb	w1, [x0, -1]!
	cmp	x0, x20
	cbz	w1, .L2595
.L2591:
	lsl	w0, w19, 3
.L2590:
	cmp	w19, w23
	mov	w1, 0
	bge	.L2596
	sxtw	x2, w23
	sub	x2, x2, #1
	sub	x19, x2, x19, sxtw
	ldrb	w2, [x22, x19]
	tbnz	x2, 0, .L2596
	lsr	w2, w2, 1
	mov	w1, 1
	.p2align 2
.L2598:
	tbnz	x2, 0, .L2596
	add	w1, w1, 1
	lsr	w2, w2, 1
	cmp	w1, 8
	bne	.L2598
.L2596:
	add	w0, w1, w0
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2592:
	subs	w26, w26, #1
	sub	w25, w25, #64
	bne	.L2599
	b	.L2591
.L2607:
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
.L2600:
	mov	w0, 0
	mov	w19, 0
	b	.L2590
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
	cbz	x19, .L2614
	ldr	x2, [x19, 632]
	ldr	w0, [x2, 76]
	cmp	w0, 1
	bls	.L2613
	ldr	w1, [x2, 48]
	ldr	x0, [x2, 40]
	bl	FindZeroBitsInSeg
	ldr	x2, [x19, 632]
	ldr	w1, [x2, 48]
	cmp	w0, w1, lsl 3
	bcs	.L2617
	cmn	w0, #1
	beq	.L2613
	add	w0, w0, 1
	str	w0, [x2, 80]
	ldr	x0, [x19, 632]
	ldr	w1, [x0, 76]
	cmp	w1, 1
	bls	.L2615
.L2618:
	ldr	w2, [x0, 48]
	mov	w20, 0
	ldr	w1, [x0, 80]
	cmp	w1, w2, lsl 3
	bcc	.L2609
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
.L2609:
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2617:
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
.L2613:
	ldr	w1, [x2, 16]
	ldr	x0, [x2, 8]
	bl	FindZeroBitsInSeg
	add	w0, w0, 1
	ldr	x2, [x19, 632]
	str	w0, [x2, 80]
	ldr	x0, [x19, 632]
	ldr	w1, [x0, 76]
	cmp	w1, 1
	bhi	.L2618
.L2615:
	mov	w20, 0
	mov	w0, w20
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L2614:
	mov	w20, -1
	b	.L2609
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
	bhi	.L2619
	mov	x0, x19
	bl	CombinePacket.part.10
.L2619:
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
	cbz	w0, .L2626
	add	x0, x19, 745
	mov	x1, 1
	b	.L2624
	.p2align 3
.L2623:
	add	w1, w1, 1
	cmp	w1, 137
	beq	.L2629
.L2624:
	ldrb	w2, [x0]
	add	x0, x0, 104
	cbnz	w2, .L2623
	mov	w0, w1
.L2622:
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
.L2629:
	mov	w0, -1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
.L2626:
	mov	w0, 0
	mov	x1, 0
	b	.L2622
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
	cbnz	w1, .L2631
	add	x1, x19, 61440
	ldrb	w2, [x1, 1600]
.L2632:
	cmp	w2, 1
	bne	.L2633
	mov	w0, 5
	strb	w0, [x3, 2]
.L2633:
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2631:
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
	b	.L2632
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
	stp	x23, x24, [sp, 48]
	mov	x19, x0
	mov	x0, x30
	add	x22, x19, 65536
	mov	x23, x1
	mov	x21, x2
	bl	_mcount
	ldr	x1, .LC762
	mov	w0, 22
	ldr	w2, [x22, 60]
	add	x20, x19, 20480
	sub	w2, w2, #1
	bl	dprint_vfmw
	str	wzr, [x21]
	str	wzr, [x23]
	mov	x0, x19
	ldr	x1, [x19, 632]
	ldrb	w2, [x1, 2]
	strb	w2, [x20, 1587]
	ldr	w1, [x1, 84]
	str	w1, [x20, 1616]
	bl	SliceCheck
	mov	w24, w0
	cbnz	w0, .L2639
	mov	x0, x19
	bl	ProcessSliceHeaderFirstPart
	cbnz	w0, .L2643
	ldrb	w2, [x20, 1589]
	mov	w1, 2208
	mov	w0, 4000
	cmp	w2, 1
	cset	w2, eq
	str	w2, [x23]
	ldr	w2, [x20, 1596]
	ldr	x4, [x19, 14968]
	ldr	x3, [x19, 14960]
	umull	x1, w2, w1
	ldr	w2, [x22, 128]
	add	x1, x4, x1
	ldr	w5, [x19, 14932]
	cmp	w2, 1
	ldr	w4, [x19, 14936]
	ldr	w1, [x1, 28]
	umull	x0, w1, w0
	add	x0, x3, x0
	ldr	w2, [x0, 3248]
	ldrb	w1, [x0, 18]
	ldr	w20, [x0, 3252]
	add	w23, w2, 1
	mov	w2, 2
	sub	w1, w2, w1
	madd	w20, w20, w1, w1
	beq	.L2644
	cmp	w5, w23
	ccmp	w4, w20, 0, eq
	beq	.L2638
	mov	w1, 1
	str	w1, [x21]
.L2638:
	add	x19, x19, 24576
	ldr	w1, [x0, 3272]
	ldr	w0, [x19, 3712]
	add	w0, w0, 1
	cmp	w1, w0
	bls	.L2635
	mov	w0, 1
	str	w0, [x21]
.L2635:
	mov	w0, w24
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2644:
	add	x0, x19, 12288
	add	x0, x0, 2600
	bl	BsResidBits
	ldr	w1, [x22, 40]
	mul	w2, w23, w20
	add	w1, w1, 1
	ldp	x19, x20, [sp, 16]
	udiv	w1, w2, w1
	ldp	x21, x22, [sp, 32]
	cmp	w0, w1
	ccmp	w0, wzr, 1, ge
	csetm	w24, lt
	mov	w0, w24
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2639:
	mov	w24, -1
	b	.L2635
	.p2align 3
.L2643:
	ldr	x1, .LC764
	mov	w0, 1
	mov	w24, -1
	bl	dprint_vfmw
	b	.L2635
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
	cbz	w0, .L2647
	mov	w19, 0
	add	x22, x20, 65536
	b	.L2651
	.p2align 3
.L2648:
	ldr	x0, [x0, 640]
	cbz	x0, .L2650
	ldr	w0, [x21, 1628]
	cmp	w0, w19
	bls	.L2675
.L2651:
	add	x1, x20, x19, sxtw 3
	ldr	w0, [x22, 116]
	add	w19, w19, 1
	ldr	x1, [x1, 28704]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	cbnz	x0, .L2648
.L2650:
	mov	w0, -1
.L2677:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2647:
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2675:
	cbz	w0, .L2647
	ldr	w0, [x21, 1632]
	mov	w19, 0
	cbnz	w0, .L2664
	b	.L2647
	.p2align 3
.L2676:
	ldr	x0, [x0, 640]
	cbz	x0, .L2650
	ldr	w0, [x21, 1632]
	cmp	w0, w19
	bls	.L2647
.L2664:
	add	x1, x20, x19, sxtw 3
	ldr	w0, [x22, 116]
	add	w19, w19, 1
	ldr	x1, [x1, 28968]
	ldr	x1, [x1, 32]
	ldr	w1, [x1, 632]
	bl	FSP_GetLogicFs
	cbnz	x0, .L2676
	mov	w0, -1
	b	.L2677
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
	b	.L2680
	.p2align 3
.L2679:
	add	x19, x19, 104
	cmp	x21, x19
	beq	.L2700
.L2680:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L2679
	ldr	w0, [x22, 116]
	mov	x1, x19
	add	x19, x19, 104
	bl	ReleaseNAL
	cmp	x21, x19
	bne	.L2680
.L2700:
	ldr	w0, [x22, 52]
	cmp	w0, 1
	beq	.L2701
.L2681:
	add	x21, x20, 61440
	mov	w23, 0
	ldr	w0, [x21, 2800]
	cbz	w0, .L2686
	.p2align 2
.L2694:
	add	x19, x20, x23, uxtw 3
	add	w23, w23, 1
	add	x19, x19, 61440
	ldr	x0, [x19, 2808]
	mov	x1, x0
	cbz	x0, .L2684
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19, 2808]
.L2684:
	ldr	w0, [x21, 2800]
	cmp	w0, w23
	bhi	.L2694
.L2686:
	add	x20, x20, 28672
	add	x21, x21, 712
	add	x20, x20, 1160
	.p2align 2
.L2683:
	sub	x19, x20, #32
.L2688:
	ldr	x1, [x19]
	cbnz	x1, .L2702
.L2687:
	add	x19, x19, 8
	cmp	x20, x19
	bne	.L2688
	add	x20, x20, 808
	cmp	x20, x21
	bne	.L2683
	ldr	x1, .LC766
	mov	w0, 2
	bl	dprint_vfmw
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2702:
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L2687
.L2701:
	str	wzr, [x22, 52]
	b	.L2681
	.size	H264DEC_Destroy, .-H264DEC_Destroy
	.align	3
.LC766:
	.xword	.LC765
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_IsNeedSedOnly
	.type	H264DEC_IsNeedSedOnly, %function
H264DEC_IsNeedSedOnly:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 65536
	bl	_mcount
	ldr	w0, [x20, 40]
	cbz	w0, .L2706
	ldr	x1, [x19, 616]
	cbz	x1, .L2711
.L2706:
	mov	w0, 0
.L2705:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2711:
	ldr	w1, [x20, 140]
	cmp	w0, w1
	beq	.L2706
	ldr	w0, [x20, 128]
	cmp	w0, 1
	beq	.L2712
	cbnz	w0, .L2706
	ldr	w0, [x20, 116]
	bl	VCTRL_CheckFreshRaw
	cbz	w0, .L2706
	ldr	w0, [x20, 116]
	bl	VCTRL_CheckFreshSeg
	cmp	w0, wzr
	cset	w0, ne
	b	.L2705
	.p2align 3
.L2712:
	ldr	x0, [x19, 632]
	cmp	x0, xzr
	cset	w0, ne
	b	.L2705
	.size	H264DEC_IsNeedSedOnly, .-H264DEC_IsNeedSedOnly
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_IsSliceUnit
	.type	H264DEC_IsSliceUnit, %function
H264DEC_IsSliceUnit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	and	w0, w19, -5
	cmp	w0, 1
	ccmp	w19, 20, 4, ne
	ldr	x19, [sp, 16]
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.size	H264DEC_IsSliceUnit, .-H264DEC_IsSliceUnit
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_IsFrameEndNal
	.type	H264DEC_IsFrameEndNal, %function
H264DEC_IsFrameEndNal:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	mov	w19, w0
	mov	x0, x30
	bl	_mcount
	sub	w0, w19, #7
	cmp	w0, 1
	bls	.L2716
	and	w0, w19, -5
	cmp	w0, 11
	ccmp	w19, 30, 4, ne
	ldr	x19, [sp, 16]
	cset	w0, eq
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2716:
	mov	w0, 1
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.size	H264DEC_IsFrameEndNal, .-H264DEC_IsFrameEndNal
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
	cbz	x0, .L2722
	ldrsb	w1, [x0, 1]
	cbz	w1, .L2722
	add	x19, x0, 568
	add	x20, x0, 600
.L2720:
	ldr	x1, [x19]
	cbnz	x1, .L2727
.L2719:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L2720
	ldr	w0, [x21, 116]
	mov	w2, 0
	mov	w1, w22
	bl	FSP_SetDisplay
	mov	w0, 0
.L2718:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L2727:
	ldr	w0, [x21, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L2719
	.p2align 3
.L2722:
	mov	w0, -1
	b	.L2718
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
	beq	.L2767
	ldr	x0, [x19]
	ldr	w0, [x0, 12]
	cmp	w0, wzr
	ble	.L2731
	add	x0, x19, 65536
	ldr	w0, [x0, 44]
	cmp	w0, 2
	beq	.L2731
	add	x19, x19, 61440
	ldr	x0, [x19, 1584]
	cbz	x0, .L2733
	strb	wzr, [x0, 1]
	mov	w0, 1
	str	xzr, [x19, 1584]
.L2730:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2731:
	mov	x1, x21
	mov	x0, x19
	bl	CheckFrameStore
	cmn	w0, #3
	beq	.L2733
	add	x20, x19, 61440
	ldr	x1, [x20, 1584]
	cbz	x1, .L2734
	mov	x0, x19
	bl	GetImagePara
	ldr	x1, [x20, 1584]
	mov	x0, x19
	bl	CheckFrameStore
	cbz	w0, .L2768
	cmn	w0, #3
	beq	.L2741
	ldr	x1, .LC776
	mov	w3, w0
	mov	w2, 2046
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x20, 1584]
	mov	x0, x19
	ldr	w1, [x1, 164]
	bl	H264DEC_RecycleImage
.L2741:
	ldr	x0, [x20, 1584]
	strb	wzr, [x0, 1]
	ldr	x0, [x20, 1584]
	str	xzr, [x20, 1584]
	cmp	x21, x0
	beq	.L2769
.L2734:
	mov	x1, x21
	mov	x0, x19
	bl	GetImagePara
	mov	x1, x21
	mov	x0, x19
	bl	CheckFrameStore
	cbz	w0, .L2770
	cmn	w0, #3
	beq	.L2733
	ldr	x1, .LC776
	mov	w3, w0
	mov	w2, 2104
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x21, 164]
	mov	x0, x19
	bl	H264DEC_RecycleImage
	mov	w0, 2
	b	.L2730
	.p2align 3
.L2767:
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
.L2733:
	mov	w0, 1
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L2770:
	add	x23, x19, 65536
	ldr	w1, [x21, 632]
	mov	w2, 1
	ldr	w0, [x23, 116]
	bl	FSP_SetDisplay
	ldr	w1, [x21, 632]
	ldr	w0, [x23, 116]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L2771
	cmp	w22, 1
	beq	.L2772
.L2745:
	ldr	w0, [x23, 116]
	mov	x4, x20
	add	x3, x19, 8
	mov	x2, x19
	mov	w1, 0
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L2773
	ldr	x0, [x19]
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L2774
	str	wzr, [x20, 24]
.L2748:
	ldr	w1, [x23, 164]
	mov	w0, 0
	ldr	w2, [x23, 172]
	add	w1, w1, 2
	str	w1, [x23, 164]
	add	w2, w2, 1
	str	w2, [x23, 172]
	b	.L2730
	.p2align 3
.L2768:
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
	cbz	x0, .L2775
	ldr	x0, [x20, 1584]
	cmp	x0, x21
	ccmp	w22, 1, 0, eq
	beq	.L2776
.L2737:
	ldr	w0, [x23, 116]
	mov	x4, x24
	add	x3, x19, 8
	mov	x2, x19
	mov	w1, 0
	bl	InsertImgToVoQueue
	cmp	w0, 1
	bne	.L2777
	ldr	x0, [x19]
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L2778
	str	wzr, [x24, 24]
.L2740:
	ldr	w0, [x23, 164]
	ldr	w1, [x23, 172]
	add	w0, w0, 2
	str	w0, [x23, 164]
	add	w1, w1, 1
	str	w1, [x23, 172]
	b	.L2741
	.p2align 3
.L2769:
	mov	w0, 0
	b	.L2730
	.p2align 3
.L2772:
	str	w22, [x0, 188]
	b	.L2745
	.p2align 3
.L2774:
	mov	x1, x20
	mov	x0, x19
	bl	SetFrmRepeatCount.part.1
	b	.L2748
	.p2align 3
.L2778:
	mov	x1, x24
	mov	x0, x19
	bl	SetFrmRepeatCount.part.1
	b	.L2740
.L2773:
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
	b	.L2730
.L2777:
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
	b	.L2730
	.p2align 3
.L2776:
	mov	w0, 1
	str	w0, [x24, 188]
	b	.L2737
.L2771:
	ldr	x1, .LC778
	mov	w2, 2074
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2730
.L2775:
	ldr	x1, .LC772
	mov	w2, 2017
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L2730
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
	cbnz	w0, .L2780
	mov	w1, w21
	mov	x0, x20
	bl	RemoveFrameStoreOutDPB
.L2780:
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
	cbnz	w0, .L2799
	b	.L2783
	.p2align 3
.L2784:
	ldr	w0, [x19, 3716]
	cmp	w0, w21
	bls	.L2783
.L2799:
	add	x0, x20, x21, uxtw 3
	add	w21, w21, 1
	ldr	x1, [x0, 27904]
	ldrb	w0, [x1, 2]
	cbz	w0, .L2784
	mov	x0, x20
	bl	UnMarkFrameStoreRef
	ldr	w0, [x19, 3716]
	cmp	w0, w21
	bhi	.L2799
	.p2align 2
.L2783:
	mov	x0, x20
	bl	RemoveUnUsedFrameStore
	cbnz	w0, .L2783
	.p2align 2
.L2787:
	ldr	w6, [x19, 3716]
	cbz	w6, .L2804
	ldr	w0, [x19, 3712]
	cmp	w6, w0
	bls	.L2788
	str	w0, [x19, 3716]
	cbz	w0, .L2794
	mov	w6, w0
.L2788:
	add	x2, x19, 3328
	mov	w5, 2147483647
	mov	w0, 0
	mov	w1, 0
	.p2align 2
.L2791:
	ldr	x4, [x2]
	add	x2, x2, 8
	ldr	w3, [x4, 648]
	cmp	w3, w5
	bge	.L2790
	ldrb	w4, [x4, 4]
	cmp	w4, wzr
	csel	w5, w5, w3, eq
	csel	w1, w1, w0, eq
.L2790:
	add	w0, w0, 1
	cmp	w6, w0
	bhi	.L2791
	mov	x0, x20
	bl	OutputFrmFromDPB
	cmn	w0, #1
	bne	.L2787
.L2792:
	ldp	x19, x20, [sp, 16]
	ldr	x21, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
.L2794:
	mov	w1, 0
	mov	x0, x20
	bl	OutputFrmFromDPB
	cmn	w0, #1
	bne	.L2787
	b	.L2792
	.p2align 3
.L2804:
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
.L2818:
	ldr	w0, [x20, -4]
	cmp	w0, 6
	bhi	.L2806
	ldrb	w0, [x24,w0,uxtw]
	adr	x1, .Lrtx2808
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx2808:
	.section	.rodata
	.align	0
	.align	2
.L2808:
	.byte	(.L2807 - .Lrtx2808) / 4
	.byte	(.L2809 - .Lrtx2808) / 4
	.byte	(.L2810 - .Lrtx2808) / 4
	.byte	(.L2811 - .Lrtx2808) / 4
	.byte	(.L2812 - .Lrtx2808) / 4
	.byte	(.L2813 - .Lrtx2808) / 4
	.byte	(.L2814 - .Lrtx2808) / 4
	.text
	.p2align 3
.L2813:
	add	x28, x21, 24576
	mov	w19, 0
	ldr	w0, [x28, 3720]
	cbz	w0, .L2817
	.p2align 2
.L2825:
	add	x1, x21, x19, uxtw 3
	mov	x0, x21
	add	w19, w19, 1
	ldr	x1, [x1, 28032]
	bl	UnMarkFrameStoreRef
	ldr	w0, [x28, 3720]
	cmp	w19, w0
	bcc	.L2825
.L2817:
	mov	x0, x21
	bl	UpdateReflist
	mov	w1, 0
	mov	x0, x21
	bl	UpdateMaxLTFrmIdx
	mov	x0, x21
	bl	UpdateLTReflist
	str	w27, [x25, 728]
	.p2align 2
.L2806:
	add	x20, x20, 20
	cmp	x20, x22
	bne	.L2818
	.p2align 2
.L2807:
	ldr	w1, [x25, 728]
	mov	w0, 0
	cmp	w1, 1
	beq	.L2828
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2812:
	ldr	w1, [x20, 12]
	mov	x0, x21
	bl	UpdateMaxLTFrmIdx
	mov	x0, x21
	bl	UpdateLTReflist
	b	.L2806
	.p2align 3
.L2811:
	ldr	w2, [x20]
	mov	x1, x23
	ldr	w3, [x20, 8]
	mov	x0, x21
	bl	MarkSTToLTRef
	mov	x0, x21
	bl	UpdateReflist
	mov	x0, x21
	bl	UpdateLTReflist
	b	.L2806
	.p2align 3
.L2810:
	ldr	w2, [x20, 4]
	mov	x0, x21
	mov	x1, x23
	bl	UnMarkLTRef
	mov	x0, x21
	bl	UpdateLTReflist
	b	.L2806
	.p2align 3
.L2809:
	ldr	w2, [x20]
	mov	x0, x21
	mov	x1, x23
	bl	UnMarkSTRef
	mov	x0, x21
	bl	UpdateReflist
	b	.L2806
	.p2align 3
.L2814:
	ldr	w2, [x20, 8]
	mov	x1, x23
	mov	x0, x21
	bl	MarkCurrPicLT
	b	.L2806
	.p2align 3
.L2828:
	ldrb	w0, [x26, 115]
	str	wzr, [x26, 768]
	cmp	w0, 1
	beq	.L2821
	cbz	w0, .L2822
	cmp	w0, 2
	bne	.L2820
	str	wzr, [x26, 780]
	str	wzr, [x26, 792]
	str	wzr, [x25, 740]
	str	wzr, [x25, 748]
.L2820:
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
.L2822:
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
	b	.L2820
.L2821:
	str	wzr, [x26, 780]
	str	wzr, [x26, 788]
	str	wzr, [x25, 736]
	str	wzr, [x25, 748]
	b	.L2820
	.size	AdaptiveMemMark, .-AdaptiveMemMark
	.align	3
.LC779:
	.xword	.L2808
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
	cbz	w0, .L2830
	add	x23, x21, 24576
	mov	w20, 0
	add	x22, x21, 65536
	mov	w25, 1
	ldr	w0, [x23, 3716]
	cbnz	w0, .L2845
	b	.L2837
	.p2align 3
.L2834:
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
	beq	.L2835
	ldr	x1, [x19, 27904]
	ldr	w0, [x22, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
.L2835:
	str	xzr, [x19, 27904]
	str	xzr, [x19, 28032]
	str	xzr, [x19, 28160]
	ldr	w0, [x23, 3716]
	cmp	w0, w20
	bls	.L2837
.L2845:
	add	x19, x21, x20, uxtw 3
	mov	w2, 0
	add	w20, w20, 1
	ldr	x0, [x19, 27904]
	ldr	w1, [x0, 800]
	cmp	w1, 1
	bne	.L2834
	str	w1, [x24, 2788]
	b	.L2834
	.p2align 3
.L2837:
	str	wzr, [x23, 3716]
	ldr	w0, [x24, 2776]
	cbz	w0, .L2833
	sub	w1, w0, #1
	add	x0, x24, 2704
	add	x1, x1, 12288
	add	x1, x1, 3749
	add	x1, x21, x1, lsl 2
	.p2align 2
.L2840:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L2840
.L2833:
	mov	x0, x21
	bl	UpdateReflist
	mov	x0, x21
	bl	UpdateLTReflist
	ldr	x0, [x24, 760]
	ldrb	w0, [x0, 2]
	cbnz	w0, .L2851
	mov	w0, 1
	str	wzr, [x23, 3728]
	strb	w0, [x24, 117]
	mov	w0, 0
	strb	wzr, [x24, 116]
.L2841:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L2851:
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
.L2830:
	mov	x0, x21
	bl	FlushDPB
	cbnz	w0, .L2842
	add	x23, x21, 24576
	b	.L2833
.L2842:
	mov	w0, -1
	b	.L2841
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
	beq	.L2853
	ldr	w0, [x19, 796]
	cbnz	w0, .L2854
	ldr	x0, [x20]
	ldr	w1, [x0, 680]
	cmp	w1, 1
	beq	.L2878
.L2854:
	ldr	x0, [x19, 760]
	ldrb	w0, [x0, 3]
	cbz	w0, .L2879
	ldr	w0, [x19, 776]
	cbnz	w0, .L2880
.L2861:
	mov	w0, 0
.L2858:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2879:
	ldr	w0, [x19, 776]
	cbz	w0, .L2861
.L2860:
	ldrb	w1, [x19, 113]
	cbnz	w1, .L2865
	add	x1, x20, 24576
	add	x2, x20, 16384
	ldr	w4, [x1, 3724]
	ldr	w2, [x2, 1836]
	ldr	w3, [x1, 3720]
	sub	w1, w2, w4
	cmp	w3, w1
	beq	.L2881
	strb	wzr, [x19, 116]
.L2863:
	cbz	w0, .L2861
.L2865:
	ldrb	w1, [x19, 116]
	mov	w0, 0
	cmp	w1, wzr
	cset	w1, eq
	strb	w1, [x19, 117]
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L2880:
	mov	x0, x20
	bl	AdaptiveMemMark
	cbnz	w0, .L2855
.L2862:
	ldrb	w0, [x19, 118]
	cmp	w0, 5
	bne	.L2882
	ldr	w0, [x19, 776]
	b	.L2863
	.p2align 3
.L2878:
	ldr	w1, [x0, 676]
	cbnz	w1, .L2854
	ldr	w1, [x0, 684]
	cbnz	w1, .L2854
	ldr	w0, [x0, 644]
	add	w0, w0, 1024
	cmp	w0, 2048
	bls	.L2854
	.p2align 2
.L2853:
	mov	x0, x20
	bl	IDRMemMarking
	cbz	w0, .L2862
.L2855:
	mov	w0, -1
	b	.L2858
	.p2align 3
.L2881:
	mov	x0, x20
	bl	SlidingWinMark.part.2
	strb	wzr, [x19, 116]
	ldr	w0, [x19, 776]
	b	.L2863
.L2882:
	ldr	w0, [x19, 776]
	cbz	w0, .L2861
	ldr	x1, [x19, 760]
	ldrb	w1, [x1, 3]
	cbnz	w1, .L2865
	b	.L2860
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
	beq	.L2885
	cbz	w0, .L2886
	cmp	w0, 2
	beq	.L2887
	mov	w21, 0
.L2884:
	mov	w0, w21
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2887:
	ldr	x0, [x19, 752]
	mov	w1, 3
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 113]
	cbnz	w0, .L2903
	ldr	x1, [x19, 1584]
	cbz	x1, .L2904
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	mov	w21, w0
	cbnz	w0, .L2969
.L2905:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 792]
.L2906:
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
	bne	.L2884
.L2963:
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
	cbz	x0, .L2914
	ldr	x1, [x19, 752]
	mov	w2, w22
	ldr	w0, [x23, 116]
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
.L2914:
	str	xzr, [x19, 1584]
.L2971:
	mov	w0, w21
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2886:
	ldr	x0, [x19, 752]
	strb	wzr, [x0, 680]
	ldr	x1, [x19, 1584]
	cbz	x1, .L2959
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	cbnz	w0, .L2970
.L2889:
	ldr	x0, [x19, 1584]
	cbz	x0, .L2959
	ldr	x0, [x19, 752]
	add	x22, x20, 65536
	mov	w2, 1
	ldr	w1, [x0, 632]
	ldr	w0, [x22, 116]
	bl	FSP_ClearLogicFs
.L2888:
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
.L2966:
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
	cbz	x0, .L2914
	ldr	x1, [x19, 752]
	mov	w2, 1
	ldr	w0, [x22, 116]
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
	str	xzr, [x19, 1584]
	b	.L2971
	.p2align 3
.L2885:
	ldr	x0, [x19, 752]
	mov	w1, 3
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 113]
	cbnz	w0, .L2892
	ldr	x1, [x19, 1584]
	cbz	x1, .L2893
	mov	w2, 0
	mov	x0, x20
	bl	OutputFrmToVO
	mov	w21, w0
	cbnz	w0, .L2972
.L2894:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 752]
.L2895:
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
	beq	.L2963
	mov	w0, w21
	ldr	x27, [sp, 80]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
	.p2align 3
.L2903:
	ldr	x0, [x19, 1584]
	cbz	x0, .L2973
.L2909:
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
	beq	.L2910
	ldr	x0, [x19, 1584]
	str	x27, [x0, 608]
.L2910:
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
	cbz	w0, .L2911
	ldrb	w0, [x19, 114]
.L2911:
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
	beq	.L2974
.L2912:
	ldrb	w0, [x1, 3]
.L2913:
	strb	w0, [x1, 3]
	add	x22, x20, 65536
	mov	w2, 0
	b	.L2966
	.p2align 3
.L2892:
	ldr	x0, [x19, 1584]
	cbz	x0, .L2975
.L2897:
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
	beq	.L2898
	ldr	x0, [x19, 1584]
	str	x27, [x0, 608]
.L2898:
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
	cbz	w0, .L2899
	ldrb	w0, [x19, 114]
.L2899:
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
	bne	.L2912
	ldrb	w0, [x1, 3]
	orr	w0, w0, 1
	uxtb	w0, w0
	b	.L2913
	.p2align 3
.L2959:
	add	x22, x20, 65536
	b	.L2888
	.p2align 3
.L2970:
	ldr	x2, .LC780
	mov	w4, w0
	ldr	x1, .LC782
	mov	w3, 2704
	add	x2, x2, 232
	mov	w0, 22
	bl	dprint_vfmw
	b	.L2889
	.p2align 3
.L2974:
	ldrb	w0, [x1, 3]
	orr	w0, w0, 2
	uxtb	w0, w0
	b	.L2913
	.p2align 3
.L2893:
	ldr	x0, [x19, 752]
	mov	w21, 0
	str	x0, [x19, 1584]
	str	x0, [x0, 752]
	b	.L2895
	.p2align 3
.L2904:
	ldr	x0, [x19, 752]
	mov	w21, 0
	str	x0, [x19, 1584]
	str	x0, [x0, 792]
	b	.L2906
	.p2align 3
.L2972:
	ldr	x2, .LC780
	mov	w4, w0
	ldr	x1, .LC782
	mov	w3, 2756
	add	x2, x2, 232
	mov	w0, 22
	bl	dprint_vfmw
	b	.L2894
	.p2align 3
.L2969:
	ldr	x2, .LC780
	mov	w4, w0
	ldr	x1, .LC782
	mov	w3, 2898
	add	x2, x2, 232
	mov	w0, 22
	bl	dprint_vfmw
	b	.L2905
	.p2align 3
.L2973:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 792]
	ldr	x0, [x19, 1584]
	b	.L2909
	.p2align 3
.L2975:
	ldr	x0, [x19, 752]
	str	x0, [x19, 1584]
	str	x0, [x0, 752]
	ldr	x0, [x19, 1584]
	b	.L2897
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
	cbnz	w0, .L2996
	b	.L2978
	.p2align 3
.L2979:
	ldr	w0, [x20, 3716]
	cmp	w0, w19
	bls	.L2978
.L2996:
	add	x0, x21, x19, uxtw 3
	add	w19, w19, 1
	ldr	x1, [x0, 27904]
	ldrb	w0, [x1, 2]
	cbz	w0, .L2979
	mov	x0, x21
	bl	UnMarkFrameStoreRef
	ldr	w0, [x20, 3716]
	cmp	w0, w19
	bhi	.L2996
	.p2align 2
.L2978:
	mov	x0, x21
	bl	RemoveUnUsedFrameStore
	cbnz	w0, .L2978
	mov	w23, -1
	.p2align 2
.L2982:
	ldr	w5, [x20, 3716]
	cbz	w5, .L3002
.L2988:
	ldr	w0, [x20, 3712]
	cmp	w5, w0
	bls	.L2983
	str	w0, [x20, 3716]
	cbz	w0, .L2990
	mov	w5, w0
.L2983:
	add	x1, x20, 3328
	mov	w4, 2147483647
	mov	w0, 0
	mov	w19, 0
	.p2align 2
.L2986:
	ldr	x3, [x1]
	add	x1, x1, 8
	ldr	w2, [x3, 648]
	cmp	w2, w4
	bge	.L2985
	ldrb	w3, [x3, 4]
	cmp	w3, wzr
	csel	w4, w4, w2, eq
	csel	w19, w19, w0, eq
.L2985:
	add	w0, w0, 1
	cmp	w5, w0
	bhi	.L2986
.L2984:
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
	cbnz	w0, .L2982
	mov	w1, w19
	mov	x0, x21
	bl	RemoveFrameStoreOutDPB
	ldr	w5, [x20, 3716]
	cbnz	w5, .L2988
.L3002:
	cbz	w23, .L2989
	add	x0, x21, 8
	bl	GetVoLastImageID
	mov	w23, w0
.L2989:
	mov	w0, w23
	ldr	x23, [sp, 48]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
.L2990:
	mov	w19, 0
	b	.L2984
	.size	H264DEC_GetRemainImg, .-H264DEC_GetRemainImg
	.global	_mcount
	.align	2
	.p2align 3,,7
	.type	InitDPB.part.14, %function
InitDPB.part.14:
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
.L3007:
	ldr	x0, [x19]
	mov	w2, 1
	cbz	x0, .L3004
	ldr	w1, [x0, 632]
	ldr	w0, [x20, 116]
	bl	FSP_ClearLogicFs
.L3004:
	ldr	x0, [x19, 128]
	mov	w2, 1
	cbz	x0, .L3005
	ldr	w1, [x0, 632]
	ldr	w0, [x20, 116]
	bl	FSP_ClearLogicFs
.L3005:
	ldr	x0, [x19, 256]
	mov	w2, 1
	cbz	x0, .L3006
	ldr	w1, [x0, 632]
	ldr	w0, [x20, 116]
	bl	FSP_ClearLogicFs
.L3006:
	str	xzr, [x19, 128]
	str	xzr, [x19, 256]
	str	xzr, [x19], 8
	cmp	x19, x22
	bne	.L3007
	add	x0, x21, 16384
	add	x1, x23, 3584
	add	x19, x21, 61440
	ldr	w0, [x0, 1864]
	stp	wzr, wzr, [x1, 136]
	sub	w0, w0, #1
	stp	w0, wzr, [x1, 128]
	str	wzr, [x23, 3728]
	ldr	x1, [x19, 1584]
	cbz	x1, .L3009
	add	x20, x21, 65536
	ldr	w0, [x20, 44]
	cmp	w0, 2
	beq	.L3039
.L3010:
	ldr	w0, [x20, 116]
	mov	w2, 1
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
	ldr	x0, [x19, 752]
	cbz	x0, .L3013
	ldr	x2, [x19, 1584]
	ldr	w1, [x0, 632]
	ldr	w0, [x2, 632]
	cmp	w0, w1
	beq	.L3013
	ldr	w0, [x20, 116]
	mov	w2, 1
	bl	FSP_ClearLogicFs
.L3013:
	str	xzr, [x19, 1584]
.L3009:
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
.L3014:
	str	wzr, [x0], 4
	cmp	x0, x1
	bne	.L3014
	add	x2, x21, 28672
	add	x1, x2, 32
	add	x0, x2, 296
	.p2align 2
.L3015:
	str	xzr, [x1], 8
	cmp	x1, x0
	bne	.L3015
	add	x21, x21, 20480
	add	x1, x2, 560
	str	wzr, [x21, 1628]
	.p2align 2
.L3016:
	str	xzr, [x0], 8
	cmp	x1, x0
	bne	.L3016
	str	wzr, [x21, 1632]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
.L3039:
	mov	w2, 0
	mov	x0, x21
	bl	OutputFrmToVO
	cbnz	w0, .L3040
.L3011:
	ldr	x1, [x19, 1584]
	cbz	x1, .L3009
	b	.L3010
	.p2align 3
.L3040:
	ldr	x2, .LC783
	mov	w4, w0
	ldr	x1, .LC784
	mov	w3, 5669
	add	x2, x2, 248
	mov	w0, 22
	bl	dprint_vfmw
	b	.L3011
	.size	InitDPB.part.14, .-InitDPB.part.14
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
	cbz	x19, .L3043
	mov	x0, x19
	bl	ClearAllSlice
	mov	x0, x19
	bl	InitDPB.part.14
.L3042:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3043:
	mov	w0, -1
	b	.L3042
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
	cbz	w0, .L3045
	ldr	x1, .LC787
	mov	w3, w0
	mov	w2, 501
	mov	w0, 0
	bl	dprint_vfmw
.L3045:
	mov	x0, x19
	bl	InitDPB
	add	x1, x19, 61440
	add	x19, x19, 65536
	mov	w0, 0
	str	wzr, [x1, 2788]
	str	wzr, [x19, 148]
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
	cbz	w0, .L3050
	ldrb	w0, [x21, 113]
	add	x19, x22, 65536
	cmp	w0, 1
	beq	.L3050
.L3051:
	ldr	w20, [x19, 104]
	ldr	w0, [x19, 44]
	ubfx	x20, x20, 16, 3
	cmp	w20, w0
	beq	.L3052
	sub	w0, w0, #2
	cmp	w0, 1
	ccmp	w20, 2, 4, ls
	beq	.L3052
	cmp	w20, 1
	ccmp	w20, 4, 4, hi
	beq	.L3135
.L3052:
	cmp	w20, 2
	str	w20, [x19, 44]
	beq	.L3055
	cmp	w20, 3
	beq	.L3056
	cmp	w20, 1
	beq	.L3136
	ldr	w0, [x19, 40]
	cbz	w0, .L3059
	cmp	w20, 4
	ldr	w20, [x21, 796]
	bne	.L3065
	cbnz	w20, .L3065
	ldr	w0, [x21, 2792]
	cbnz	w0, .L3066
	mov	w0, 2
	str	w0, [x21, 2792]
	b	.L3079
	.p2align 3
.L3050:
	add	x19, x22, 65536
	ldp	w0, w1, [x19, 168]
	add	w0, w0, 1
	cmp	w0, w1
	csel	w0, w0, w1, ge
	str	w0, [x19, 168]
	sub	w0, w0, w1
	ldr	x1, [x22]
	str	w0, [x1, 616]
	b	.L3051
	.p2align 3
.L3056:
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
.L3128:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3136:
	ldr	w20, [x21, 796]
	cmp	w20, 2
	bne	.L3061
	ldr	w0, [x21, 776]
	cbz	w0, .L3062
.L3061:
	ldr	w0, [x19, 40]
	cbz	w0, .L3059
.L3065:
	cmp	w20, 2
	beq	.L3137
	cmp	w20, 1
	bne	.L3079
	str	w20, [x21, 2792]
.L3079:
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
	cbnz	w0, .L3069
	ldr	x1, [x22]
.L3070:
	ldr	w2, [x1, 632]
	mov	w0, 0
	str	w2, [x19, 16]
	ldr	w1, [x1, 672]
	str	w1, [x19, 20]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3055:
	ldr	w0, [x21, 796]
	cbnz	w0, .L3062
	ldr	w0, [x19, 40]
	cbz	w0, .L3059
	ldr	x0, [x19, 32]
	ldr	w0, [x0, 48]
	cbz	w0, .L3079
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
	b	.L3128
	.p2align 3
.L3059:
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
	cbz	x4, .L3131
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
.L3137:
	ldr	w0, [x21, 2792]
	cmp	w0, 2
	bne	.L3079
	ldr	x0, [x22]
	ldr	w0, [x0, 12]
	cbnz	w0, .L3079
	ldr	x0, .LC793
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L3079
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
	cbz	x4, .L3068
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3068:
	ldr	x1, [x21, 752]
	mov	w2, 0
	ldr	w0, [x19, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
	.p2align 2
.L3131:
	mov	w0, -1
	b	.L3128
	.p2align 3
.L3069:
	ldr	w4, [x21, 796]
	cbnz	w4, .L3071
	ldr	x0, .LC792
	ldr	x5, [x0]
	cbz	x5, .L3072
	ldr	x1, [x21, 3848]
	cbz	x1, .L3073
	mov	w2, 0
	.p2align 2
.L3074:
	ldp	w3, w0, [x1, 8]
	ldr	x1, [x1, 4056]
	add	w0, w3, w0
	add	w0, w0, 7
	add	w2, w2, w0, lsr 3
	mov	w4, w2
	cbnz	x1, .L3074
.L3073:
	add	x2, x29, 64
	ldr	w0, [x19, 116]
	mov	w1, 20
	mov	w3, 4
	str	w4, [x2, -8]!
	blr	x5
	ldr	x1, [x22]
.L3075:
	add	x2, x21, 2840
	str	x2, [x22, 616]
	ldr	w0, [x1, 904]
	cmp	w0, 1
	beq	.L3138
	cmp	w0, 2
	beq	.L3139
.L3078:
	str	wzr, [x19, 28]
	b	.L3070
	.p2align 3
.L3139:
	ldr	w0, [x1, 908]
	cmp	w0, wzr
	ccmp	x2, xzr, 4, gt
	beq	.L3078
	ldr	w20, [x21, 796]
	cmp	w20, 2
	bne	.L3078
	str	xzr, [x22, 616]
	mov	w2, 0
	ldr	x1, [x21, 752]
	ldr	w0, [x19, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
	ldr	x2, [x22]
	mov	x0, x22
	ldr	w1, [x2, 908]
	sub	w1, w1, #1
	str	w1, [x2, 908]
.L3132:
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
.L3130:
	strb	w4, [x1, 3234]
	add	x4, x1, 3072
	str	w5, [x1, 3244]
	stp	w3, w0, [x4, 184]
	stp	wzr, wzr, [x4, 192]
	str	w2, [x1, 3272]
	b	.L3128
	.p2align 3
.L3071:
	add	x0, x22, 24576
	ldr	w0, [x0, 3716]
	cbnz	w0, .L3072
	ldr	x1, [x22]
	ldr	w0, [x1, 12]
	cmp	w0, 2
	beq	.L3075
	ldr	x0, .LC792
	ldr	x4, [x0]
	cbz	x4, .L3076
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 127
	blr	x4
.L3076:
	mov	x0, x22
.L3133:
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
	b	.L3130
	.p2align 3
.L3072:
	ldr	x1, [x22]
	b	.L3075
	.p2align 3
.L3138:
	cbz	x2, .L3078
	ldr	w20, [x21, 796]
	cmp	w20, 2
	bne	.L3078
	str	xzr, [x22, 616]
	mov	w2, 0
	ldr	x1, [x21, 752]
	ldr	w0, [x19, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetDisplay
	mov	x0, x22
	b	.L3132
	.p2align 3
.L3066:
	mov	w0, 1
	str	w0, [x21, 2792]
	b	.L3079
.L3135:
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
	b	.L3128
.L3062:
	mov	w1, 1
	mov	x0, x22
	strb	w1, [x21, 119]
	b	.L3133
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
	cbz	w0, .L3140
	ldr	x1, .LC799
	mov	w0, w20
	bl	dprint_vfmw
.L3140:
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
	.global	H264DEC_SetDecParam
	.type	H264DEC_SetDecParam, %function
H264DEC_SetDecParam:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	mov	x19, x0
	mov	x0, x30
	add	x20, x19, 65536
	bl	_mcount
	ldr	w0, [x20, 132]
	cmp	w0, 1
	beq	.L3156
.L3146:
	mov	x0, x19
	bl	H264DEC_IsNeedSedOnly
	cmp	w0, 1
	bne	.L3145
	add	x1, x19, 61440
	ldr	w2, [x20, 40]
	str	w0, [x20, 28]
	add	x0, x1, 2840
	str	w2, [x20, 140]
	str	w2, [x1, 3844]
	ldr	w3, [x1, 820]
	str	w3, [x1, 3644]
	ldr	x1, [x19]
	ldr	w1, [x1, 672]
	str	w1, [x20, 20]
	str	x0, [x19, 616]
	str	wzr, [x20, 132]
.L3145:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 32
	ret
	.p2align 3
.L3156:
	ldr	w0, [x20, 40]
	cbz	w0, .L3146
	ldr	x0, [x19, 616]
	cbnz	x0, .L3146
	mov	x0, x19
	bl	DecVDM
	ldr	x0, [x19, 616]
	cbnz	x0, .L3145
	ldr	x1, [x19, 632]
	cbz	x1, .L3145
	ldr	w0, [x20, 116]
	bl	ReleaseNAL
	str	xzr, [x19, 632]
	b	.L3145
	.size	H264DEC_SetDecParam, .-H264DEC_SetDecParam
	.global	_mcount
	.align	2
	.p2align 3,,7
	.global	H264DEC_CheckSedOnly
	.type	H264DEC_CheckSedOnly, %function
H264DEC_CheckSedOnly:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x20, x0
	mov	x0, x30
	add	x19, x20, 65536
	bl	_mcount
	ldr	w0, [x19, 124]
	str	wzr, [x19, 128]
	cmp	w0, 2
	beq	.L3191
	cmp	w0, 1
	mov	w0, 0
	beq	.L3192
.L3185:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3192:
	ldr	w1, [x19, 132]
	cmp	w1, 1
	bne	.L3185
	mov	w0, -1
	str	wzr, [x19, 132]
	b	.L3185
	.p2align 3
.L3191:
	ldr	w0, [x19, 120]
	cbz	w0, .L3190
	ldr	w1, [x19, 132]
	mov	w0, 0
	cmp	w1, 1
	beq	.L3185
	ldr	w0, [x19, 116]
	bl	VCTRL_CheckFreshSeg
	cbz	w0, .L3190
	ldr	w0, [x19, 116]
	bl	VCTRL_CheckFreshRaw
	cbz	w0, .L3190
	ldr	x0, [x20, 632]
	ldr	w2, [x0, 16]
	ldr	x1, [x0, 8]
	cmp	w2, 3
	bls	.L3190
	add	x21, x20, 12288
	add	x21, x21, 2600
	mov	x0, x21
	bl	BsInit
	mov	w1, 24
	mov	x0, x21
	bl	BsSkip
	mov	x0, x21
	bl	BsResidBits
	cmp	w0, 7
	ble	.L3190
	mov	w1, 3
	mov	x0, x21
	bl	BsSkip
	mov	w1, 5
	mov	x0, x21
	bl	BsGet
	cmp	w0, 20
	mov	w22, w0
	beq	.L3193
	and	w0, w0, -5
	cmp	w0, 1
	cset	w23, eq
	beq	.L3163
	ldr	w1, [x19, 40]
	mov	w0, 0
	cbz	w1, .L3190
.L3170:
	and	w1, w22, -9
	cmp	w1, 7
	beq	.L3168
	cmp	w22, 8
	ccmp	w22, 30, 4, ne
	ccmp	w22, 11, 4, ne
	ccmp	w0, wzr, 0, ne
	bne	.L3168
	cbz	w23, .L3190
.L3171:
	add	x20, x20, 61440
	mov	w0, 1
	stp	w0, wzr, [x19, 128]
	mov	w0, 0
	ldrb	w1, [x20, 112]
	str	w1, [x19, 136]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3168:
	mov	x0, x20
	bl	DecVDM
	.p2align 2
.L3190:
	mov	w0, -1
	b	.L3185
	.p2align 3
.L3193:
	mov	x0, x21
	bl	BsResidBits
	cmp	w0, 23
	ble	.L3190
	mov	w1, 24
	mov	x0, x21
	mov	w23, 1
	bl	BsGet
	tbnz	x0, 23, .L3194
.L3163:
	mov	x0, x21
	bl	BsResidBits
	cmp	w0, wzr
	mov	w0, 0
	ble	.L3167
	mov	w1, 1
	mov	x0, x21
	bl	BsGet
.L3167:
	ldr	w1, [x19, 40]
	cbz	w1, .L3171
	b	.L3170
	.p2align 3
.L3194:
	ubfx	x0, x0, 22, 1
	mov	w1, 5
	cmp	w0, w23
	csel	w22, w22, w1, ne
	b	.L3163
	.size	H264DEC_CheckSedOnly, .-H264DEC_CheckSedOnly
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
	str	wzr, [x21, 176]
	ldr	x0, [x20]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L3260
.L3196:
	ldr	x0, [x20, 632]
	cbz	x0, .L3201
	ldrb	w1, [x19]
	cmp	w1, 1
	beq	.L3261
.L3199:
	ldr	x1, .LC803
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbz	x1, .L3201
	ldr	w0, [x21, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3201:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	ble	.L3203
	ldr	x0, [x19, 8]
	cbz	x0, .L3203
	ldr	w0, [x19, 16]
	cbz	w0, .L3203
	ldrb	w0, [x20, 641]
	cbz	w0, .L3221
	add	x1, x20, 745
	mov	x0, 1
	b	.L3206
	.p2align 3
.L3205:
	add	w0, w0, 1
	cmp	w0, 137
	beq	.L3219
.L3206:
	ldrb	w2, [x1]
	add	x1, x1, 104
	cbnz	w2, .L3205
	mov	w2, w0
.L3204:
	lsl	x1, x0, 3
	mov	w3, 1
	sub	x1, x1, x0
	cmn	w2, #1
	lsl	x1, x1, x3
	sub	x0, x1, x0
	add	x0, x20, x0, lsl 3
	strb	w3, [x0, 641]
	beq	.L3219
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
	ldr	w0, [x19, 28]
	str	w0, [x21, 124]
	ldr	x0, [x20, 632]
	b	.L3211
	.p2align 3
.L3261:
	ldr	w2, [x0, 76]
	cmp	w2, 1
	bhi	.L3199
	ldrb	w2, [x0, 4]
	cmp	w2, 1
	bne	.L3200
	b	.L3199
	.p2align 3
.L3260:
	ldr	w0, [x0, 912]
	cbz	w0, .L3196
	ldr	x0, [x20, 632]
	cbz	x0, .L3201
	ldrb	w1, [x19]
.L3200:
	strb	w1, [x0, 4]
	ldr	x0, [x20]
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L3262
.L3213:
	ldr	w0, [x19, 20]
	cmp	w0, wzr
	ble	.L3214
	ldr	x1, [x19, 8]
	cbz	x1, .L3214
	ldr	w0, [x19, 16]
	cbz	w0, .L3214
	ldr	x0, [x20, 632]
	ldr	w2, [x0, 76]
	cmp	w2, 1
	bls	.L3263
.L3215:
	mov	w1, 1
	strb	w1, [x0, 4]
	ldr	w1, [x19, 24]
	ldr	w0, [x21, 116]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC806
	ldr	x4, [x0]
	cbz	x4, .L3257
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3257:
	ldr	x0, [x20, 632]
.L3211:
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
.L3203:
	ldr	x1, .LC808
	mov	w0, 1
	bl	dprint_vfmw
	ldr	w1, [x19, 24]
	ldr	w0, [x21, 116]
	bl	SM_ReleaseStreamSeg
	ldr	x0, .LC806
	ldr	x4, [x0]
	cbz	x4, .L3259
.L3258:
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L3259:
	mov	w0, -1
.L3266:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	ret
	.p2align 3
.L3214:
	ldr	x0, [x20, 632]
	b	.L3215
	.p2align 3
.L3262:
	ldr	w0, [x0, 912]
	cbnz	w0, .L3257
	b	.L3213
	.p2align 3
.L3263:
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
	bls	.L3264
.L3216:
	mov	w1, 1
	str	w1, [x21, 176]
	b	.L3211
	.p2align 3
.L3264:
	mov	x0, x20
	bl	CombinePacket.part.10
	ldr	x0, [x20, 632]
	b	.L3216
.L3219:
	ldr	x1, .LC805
	mov	w0, 0
	add	x22, x20, 12288
	add	x19, x20, 640
	add	x22, x22, 2600
	bl	dprint_vfmw
	b	.L3208
	.p2align 3
.L3207:
	add	x19, x19, 104
	cmp	x19, x22
	beq	.L3265
.L3208:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L3207
	ldr	w0, [x21, 116]
	mov	x1, x19
	bl	ReleaseNAL
	b	.L3207
.L3265:
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
	cbnz	x4, .L3258
	mov	w0, -1
	b	.L3266
.L3221:
	mov	w2, 0
	mov	x0, 0
	b	.L3204
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
	b	.L3269
	.p2align 3
.L3268:
	add	x19, x19, 104
	cmp	x19, x20
	beq	.L3297
.L3269:
	ldrb	w0, [x19, 1]
	cmp	w0, 1
	bne	.L3268
	ldr	w0, [x22, 116]
	mov	x1, x19
	add	x19, x19, 104
	bl	ReleaseNAL
	cmp	x19, x20
	bne	.L3269
.L3297:
	ldr	x19, [x29, 104]
	mov	x0, x19
	bl	InitDPB
	cbz	w21, .L3270
	ldr	w0, [x22, 116]
	add	x1, x19, 8
	bl	FSP_ClearNotInVoQueue
.L3271:
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
.L3274:
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
.L3273:
	ldr	x1, [x20]
	cbnz	x1, .L3298
.L3272:
	add	x20, x20, 8
	cmp	x20, x24
	bne	.L3273
	add	w25, w25, 1
	add	x19, x19, 808
	cmp	w25, 40
	bne	.L3274
	ldr	x0, [x29, 104]
	add	x21, x0, 61440
	add	x19, x21, 688
	add	x20, x21, 720
.L3276:
	ldr	x1, [x19]
	cbnz	x1, .L3299
.L3275:
	add	x19, x19, 8
	cmp	x19, x20
	bne	.L3276
	ldr	w0, [x21, 2800]
	mov	w20, 0
	cbz	w0, .L3280
	.p2align 2
.L3287:
	ldr	x0, [x29, 104]
	add	x19, x0, x20, uxtw 3
	add	w20, w20, 1
	add	x19, x19, 61440
	ldr	x0, [x19, 2808]
	mov	x1, x0
	cbz	x0, .L3278
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19, 2808]
.L3278:
	ldr	w0, [x21, 2800]
	cmp	w0, w20
	bhi	.L3287
.L3280:
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
	str	wzr, [x22, 148]
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
.L3298:
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x20]
	b	.L3272
.L3299:
	ldr	w0, [x22, 116]
	bl	FreeUsdByDec
	str	xzr, [x19]
	b	.L3275
.L3270:
	ldr	x0, [x29, 104]
	add	x0, x0, 8
	bl	ResetVoQueue
	ldr	w0, [x22, 116]
	bl	FSP_EmptyInstance
	b	.L3271
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
	bhi	.L3395
	ldrb	w21, [x19, 3]
	cmp	w21, 1
	beq	.L3304
	cbz	w21, .L3305
	cmp	w21, 2
	bne	.L3396
	ldr	x0, [x19, 640]
	mov	w1, 3
	uxtw	x22, w20
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 1]
	cbz	w0, .L3323
	add	x20, x23, x22, lsl 3
	ldr	x0, [x20, 27904]
	cbz	x0, .L3323
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
	cbz	w0, .L3329
	ldrb	w0, [x19, 2]
.L3329:
	strb	w0, [x1]
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 1]
	orr	w0, w0, 2
	strb	w0, [x1, 1]
	ldrb	w0, [x19, 7]
	ldr	x1, [x20, 27904]
	cmp	w0, 1
	beq	.L3397
	ldrb	w0, [x1, 3]
.L3331:
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
	beq	.L3332
	ldr	x0, [x20, 27904]
	str	x4, [x0, 608]
.L3332:
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
	beq	.L3333
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3333
.L3334:
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
	b	.L3322
	.p2align 3
.L3305:
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
	cbz	w2, .L3398
.L3307:
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
	cbnz	w0, .L3399
.L3322:
	add	x3, x22, 3488
	add	x0, x23, 65536
	mov	w2, 1
	ldr	x1, [x23, x3, lsl 3]
	ldr	w0, [x0, 116]
	ldr	w1, [x1, 632]
	bl	FSP_SetRef
	mov	w0, 0
.L3302:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3396:
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
.L3304:
	ldr	x0, [x19, 640]
	mov	w1, 3
	uxtw	x22, w20
	strb	w1, [x0, 680]
	ldrb	w0, [x19, 1]
	cbz	w0, .L3310
	add	x20, x23, x22, lsl 3
	ldr	x0, [x20, 27904]
	cbz	x0, .L3310
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
	cbz	w0, .L3315
	ldrb	w0, [x19, 2]
.L3315:
	strb	w0, [x1]
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 1]
	orr	w0, w0, 1
	strb	w0, [x1, 1]
	ldrb	w0, [x19, 7]
	ldr	x1, [x20, 27904]
	cmp	w0, 1
	beq	.L3400
	ldrb	w0, [x1, 3]
.L3317:
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
	beq	.L3318
	ldr	x0, [x20, 27904]
	str	x4, [x0, 608]
.L3318:
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
	beq	.L3319
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3319
.L3320:
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
	b	.L3322
	.p2align 3
.L3310:
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
	beq	.L3312
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3312
.L3313:
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
	cbz	w0, .L3322
	ldr	x1, .LC815
	mov	w3, w0
	mov	w2, 2394
	b	.L3393
	.p2align 3
.L3323:
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
	beq	.L3325
	ldrb	w0, [x19, 5]
	cmp	w0, 1
	beq	.L3325
.L3326:
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
	cbz	w0, .L3322
	ldr	x1, .LC815
	mov	w3, w0
	mov	w2, 2539
.L3393:
	mov	w0, 0
	bl	dprint_vfmw
	mov	x0, x23
	bl	ClearDPB
	mov	w0, -1
	b	.L3302
	.p2align 3
.L3398:
	ldrb	w0, [x19, 5]
	mov	w2, 3
	cmp	w0, wzr
	csel	w0, w0, w2, eq
	b	.L3307
	.p2align 3
.L3325:
	ldr	x0, [x20, 27904]
	mov	w1, 2
	strb	w1, [x0, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3326
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3326
	.p2align 3
.L3312:
	ldr	x0, [x20, 27904]
	mov	w1, 1
	strb	w1, [x0, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3313
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3313
	.p2align 3
.L3333:
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 2
	strb	w0, [x1, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3334
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3334
	.p2align 3
.L3319:
	ldr	x1, [x20, 27904]
	ldrb	w0, [x1, 2]
	orr	w0, w0, 1
	strb	w0, [x1, 2]
	ldrb	w0, [x19, 4]
	cmp	w0, 1
	bne	.L3320
	ldr	x0, [x20, 27904]
	ldr	w1, [x19, 660]
	str	w1, [x0, 644]
	b	.L3320
	.p2align 3
.L3397:
	ldrb	w0, [x1, 3]
	orr	w0, w0, 2
	uxtb	w0, w0
	b	.L3331
	.p2align 3
.L3400:
	ldrb	w0, [x1, 3]
	orr	w0, w0, 1
	uxtb	w0, w0
	b	.L3317
	.p2align 3
.L3395:
	ldr	x1, .LC813
	mov	w3, w20
	mov	w2, 2303
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3302
.L3399:
	ldr	x1, .LC815
	mov	w3, w0
	mov	w2, 2341
	b	.L3393
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
	ldr	w0, [x0, 636]
	cmp	w0, 1
	beq	.L3585
	add	x22, x24, 61440
	stp	wzr, wzr, [x22, 208]
.L3404:
	add	x19, x24, 20480
	str	wzr, [x19, 728]
	ldrb	w1, [x22, 115]
	cmp	w1, 2
	cset	w0, eq
	str	w0, [x19, 732]
	ldrb	w0, [x22, 112]
	cbz	w0, .L3586
	add	x21, x24, 65536
	ldr	w0, [x21, 44]
	ldr	w2, [x21, 156]
	cmp	w0, 2
	str	w2, [x21, 152]
	beq	.L3587
	ldr	x20, [x24]
	ldr	w0, [x20, 12]
	cmp	w0, wzr
	ble	.L3588
	ldrb	w2, [x22, 114]
	cbnz	w2, .L3412
.L3593:
	sub	w2, w1, #1
	ldr	x25, [x22, 752]
	uxtb	w2, w2
	cbnz	w1, .L3589
.L3413:
	cmp	w2, 1
	bls	.L3590
.L3567:
	cbz	w1, .L3418
	ldrb	w2, [x25, 1]
	cbnz	w2, .L3418
	.p2align 2
.L3412:
	cmp	w0, 2
	beq	.L3591
.L3439:
	ldr	w6, [x22, 776]
	cbz	w6, .L3448
	ldr	w3, [x21, 108]
	ldr	w2, [x22, 804]
	cmp	w3, w2
	bcs	.L3449
	ldrb	w0, [x22, 114]
	cbz	w0, .L3592
.L3449:
	mov	x0, x24
	bl	Marking
	cbz	w0, .L3451
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
	b	.L3493
.L3586:
	ldr	x1, .LC820
	add	x21, x24, 65536
	bl	dprint_vfmw
	ldr	x1, .LC822
	mov	w3, 3666
	mov	w2, -1
	mov	w0, 14
	bl	dprint_vfmw
	ldr	x20, .LC831
.L3493:
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
.L3491:
	strb	wzr, [x22, 112]
	mov	w0, w19
.L3574:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 96
	ret
.L3585:
	ldr	x1, .LC818
	add	x22, x24, 61440
	ldr	x1, [x1]
	ldr	w2, [x1, 4180]
	ldr	w1, [x1, 4184]
	cmp	w1, wzr
	csinc	w1, w1, wzr, ne
	udiv	w1, w2, w1
	stp	w0, w1, [x22, 208]
	b	.L3404
.L3588:
	ldr	w2, [x21, 148]
	cmp	w2, 2
	beq	.L3412
	ldrb	w2, [x22, 114]
	cbnz	w2, .L3412
	b	.L3593
.L3591:
	ldr	w0, [x22, 776]
	cbz	w0, .L3441
	ldrb	w0, [x22, 118]
	cmp	w0, 5
	beq	.L3442
.L3446:
	mov	x0, x24
	add	x19, x24, 24576
	bl	SimpleSlideDPB
	ldr	w1, [x19, 3716]
	add	x2, x22, 112
	mov	x0, x24
	bl	InsertFrmInDPB
	cbz	w0, .L3594
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
	b	.L3493
.L3589:
	ldrb	w3, [x25, 1]
	cmp	w3, wzr
	ccmp	w2, 1, 0, eq
	bls	.L3413
	cmp	w0, 2
	bne	.L3439
	b	.L3591
.L3590:
	ldr	w2, [x21, 148]
	cbnz	w2, .L3415
	mov	w0, 1
	mov	x2, 720
	str	w0, [x21, 148]
	add	x1, x22, 112
	add	x0, x22, 832
	bl	memcpy
	ldr	w0, [x20, 12]
	b	.L3412
.L3418:
	add	x23, x22, 112
.L3420:
	cbz	w1, .L3421
	ldrb	w0, [x23, 1]
	cmp	w0, 1
	beq	.L3595
.L3421:
	ldr	w0, [x23, 692]
	str	w0, [x25, 664]
.L3422:
	mov	x1, x25
	mov	x0, x24
	bl	GetImagePara
	ldr	w0, [x21, 116]
	ldr	w1, [x25, 632]
	bl	FSP_GetFsImagePtr
	mov	x20, x0
	cbz	x0, .L3596
	ldrb	w0, [x23, 3]
	cbz	w0, .L3424
	ldrb	w0, [x23, 1]
	cmp	w0, 1
	beq	.L3597
.L3424:
	ldr	x0, [x23, 616]
.L3576:
	str	x0, [x20, 600]
.L3425:
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
	cbz	w1, .L3431
	ldr	w0, [x20, 108]
	and	w0, w0, 3
	cbz	w0, .L3598
.L3429:
	ldr	x0, [x24]
	ldr	w2, [x0, 4]
	cmp	w2, w1
	bcs	.L3431
	ldr	x0, .LC829
	ldr	x4, [x0]
	cbz	x4, .L3438
	ldr	w0, [x21, 116]
	mov	w3, 8
	stp	w1, w2, [x29, 88]
	add	x2, x29, 88
	mov	w1, 104
	blr	x4
.L3438:
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
	b	.L3493
.L3415:
	cmp	w2, 1
	bne	.L3567
	add	x23, x22, 112
	ldr	x25, [x22, 1472]
	b	.L3420
.L3451:
	mov	x0, x24
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	mov	x0, x24
	bl	RemoveUnUsedFrameStore
	cbz	w0, .L3441
	ldr	x1, .LC837
	mov	w0, 22
	bl	dprint_vfmw
.L3441:
	ldrb	w0, [x22, 113]
	cmp	w0, 1
	beq	.L3453
	add	x23, x24, 24576
.L3457:
	ldr	w6, [x22, 776]
	cbz	w6, .L3599
	ldr	w4, [x23, 3720]
	ldr	w5, [x23, 3724]
	ldr	w3, [x23, 3712]
	add	w0, w4, w5
	cmp	w3, w0
	bhi	.L3465
	ldr	x0, [x24]
	ldr	w0, [x0, 12]
	cmp	w0, 3
	beq	.L3466
	ldr	x0, .LC838
	ldrb	w0, [x0]
	cmp	w0, 1
	beq	.L3466
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
	b	.L3493
.L3448:
	ldrb	w0, [x22, 113]
	cmp	w0, 1
	beq	.L3453
	add	x23, x24, 24576
	ldr	w3, [x23, 3712]
.L3465:
	mov	w7, -1
	mov	w8, w7
	.p2align 2
.L3482:
	ldr	w1, [x23, 3716]
	cmp	w1, w3
	bcc	.L3600
	cmp	w1, w3
	mov	w19, w1
	bls	.L3472
	mov	w19, w3
	str	w3, [x23, 3716]
.L3472:
	cbz	w19, .L3501
	add	x1, x23, 3328
	mov	w5, 2147483647
	mov	w20, 0
	mov	w0, 0
	.p2align 2
.L3475:
	ldr	x4, [x1]
	add	x1, x1, 8
	ldr	w2, [x4, 648]
	cmp	w2, w5
	bge	.L3474
	ldrb	w4, [x4, 4]
	cmp	w4, wzr
	csel	w20, w20, w0, eq
	csel	w5, w5, w2, eq
.L3474:
	add	w0, w0, 1
	cmp	w0, w19
	bne	.L3475
.L3473:
	cbnz	w6, .L3476
	ldr	w0, [x22, 780]
	cmp	w0, w5
	blt	.L3601
.L3476:
	cmp	w7, w19
	ccmp	w8, w20, 0, eq
	beq	.L3602
	mov	w1, w20
	mov	x0, x24
	bl	OutputFrmFromDPB
	cmn	w0, #1
	mov	w25, w0
	beq	.L3481
	mov	w7, w19
	mov	w8, w20
	ldr	w6, [x22, 776]
	ldr	w3, [x23, 3712]
	b	.L3482
.L3501:
	mov	w5, 2147483647
	mov	w20, 0
	b	.L3473
.L3598:
	ldr	x1, .LC828
	mov	w2, 3851
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC829
	ldr	x4, [x0]
	cbz	x4, .L3430
	ldr	w0, [x21, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 111
	blr	x4
.L3430:
	ldr	w1, [x20, 160]
	cbnz	w1, .L3429
.L3431:
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
	beq	.L3603
	ldr	w0, [x21, 116]
	mov	w2, 0
	ldr	w1, [x25, 632]
	bl	FSP_SetDisplay
.L3433:
	ldr	x0, [x24]
	ldr	w0, [x0, 644]
	add	w0, w0, 2047
	cmp	w0, 4095
	bhi	.L3604
	str	wzr, [x20, 24]
.L3435:
	ldr	w0, [x21, 164]
	ldr	w1, [x21, 148]
	add	w0, w0, 2
	str	w0, [x21, 164]
	cmp	w1, 2
	beq	.L3437
	mov	w0, 1
	str	w0, [x25, 800]
.L3437:
	mov	w0, 2
	str	w0, [x21, 148]
	ldr	x0, [x24]
	ldr	w0, [x0, 12]
	b	.L3412
.L3587:
	ldr	x1, .LC824
	bl	dprint_vfmw
	mov	x0, x24
	bl	DirectOutput
	mov	w19, w0
	ldr	w1, [x22, 2776]
	ldr	w0, [x22, 828]
	cmp	w0, w1
	beq	.L3408
	uxtw	x0, w0
	add	x0, x0, 12288
	add	x0, x0, 3748
	str	wzr, [x24, x0, lsl 2]
	str	w1, [x22, 828]
.L3408:
	cmp	w19, 1
	beq	.L3605
.L3409:
	ldr	x1, .LC822
	mov	w3, 3727
	mov	w2, w19
	mov	w0, 14
	bl	dprint_vfmw
.L3410:
	cmn	w19, #1
	bne	.L3491
	ldr	x20, .LC831
	b	.L3493
	.p2align 3
.L3453:
	ldr	x0, [x22, 1584]
	ldr	x3, [x22, 752]
	cmp	x3, x0
	ccmp	x0, xzr, 4, eq
	beq	.L3606
	mov	x0, x24
	bl	DirectOutput
	cmn	w0, #1
	mov	w19, w0
	beq	.L3607
	cmp	w0, 1
	bne	.L3461
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 1]
.L3461:
	ldr	x1, .LC822
	mov	w3, 3997
	mov	w2, w19
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3410
	.p2align 3
.L3600:
	cbz	w6, .L3484
	ldrb	w0, [x22, 116]
	cbnz	w0, .L3484
	ldr	w0, [x23, 3720]
	cbz	w0, .L3484
	mov	w0, 0
	b	.L3489
	.p2align 3
.L3486:
	ldr	w1, [x23, 3720]
	cmp	w1, w0
	bls	.L3608
.L3489:
	add	x1, x24, x0, uxtw 3
	ldr	w3, [x22, 768]
	add	w0, w0, 1
	ldr	x2, [x1, 28032]
	ldr	w4, [x2, 636]
	cmp	w4, w3
	bne	.L3486
	strb	wzr, [x2, 2]
	ldr	x2, [x1, 28032]
	ldrb	w3, [x2, 4]
	cmp	w3, 1
	beq	.L3609
.L3487:
	ldr	w1, [x2, 668]
	ldr	w3, [x22, 2776]
	cmp	w1, w3
	uxtw	x1, w1
	add	x1, x1, 12288
	beq	.L3486
	add	x1, x1, 3748
	str	wzr, [x24, x1, lsl 2]
	str	w3, [x2, 668]
	b	.L3486
.L3608:
	ldr	w1, [x23, 3716]
.L3484:
	add	x2, x22, 112
	mov	x0, x24
	bl	InsertFrmInDPB
	ldr	w1, [x23, 3716]
	str	w1, [x22, 1576]
	cbz	w0, .L3490
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
	b	.L3493
.L3481:
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
	b	.L3493
.L3592:
	ldr	x1, .LC833
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC829
	ldr	x4, [x0]
	cbz	x4, .L3450
	ldr	w1, [x21, 108]
	mov	w3, 8
	ldr	w2, [x22, 804]
	ldr	w0, [x21, 116]
	stp	w2, w1, [x29, 88]
	add	x2, x29, 88
	mov	w1, 104
	blr	x4
.L3450:
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
	b	.L3493
.L3603:
	ldr	w0, [x21, 172]
	add	w0, w0, 1
	str	w0, [x21, 172]
	b	.L3433
.L3594:
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
	b	.L3491
.L3466:
	ldr	w6, [x23, 3716]
	cmp	w6, w3
	bls	.L3468
	mov	w6, w3
	str	w3, [x23, 3716]
.L3468:
	cbz	w6, .L3498
	add	x2, x23, 3328
	mov	w1, 0
	mov	w0, 0
	mov	w5, 2147483647
	.p2align 2
.L3471:
	ldr	x4, [x2]
	add	x2, x2, 8
	ldr	w3, [x4, 648]
	cmp	w3, w5
	bge	.L3470
	ldrb	w4, [x4, 4]
	cmp	w4, wzr
	csel	w1, w1, w0, eq
	csel	w5, w5, w3, eq
.L3470:
	add	w0, w0, 1
	cmp	w0, w6
	bne	.L3471
.L3469:
	mov	x0, x24
	bl	RemoveFrameStoreOutDPB
	mov	x0, x24
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	ldr	w6, [x22, 776]
	ldr	w3, [x23, 3712]
	b	.L3465
.L3597:
	ldr	x0, [x23, 616]
	cmn	x0, #1
	bne	.L3425
	ldr	x0, [x22, 1448]
	cmn	x0, #1
	bne	.L3576
	b	.L3425
	.p2align 3
.L3602:
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
	b	.L3493
.L3601:
	ldr	w0, [x21, 44]
	cmp	w0, 2
	beq	.L3610
.L3477:
	mov	x0, x24
	bl	DirectOutput
	cmn	w0, #1
	mov	w19, w0
	beq	.L3611
	cmp	w0, 1
	bne	.L3479
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 1]
.L3479:
	ldr	x1, .LC822
	mov	w3, 4096
	mov	w2, w19
	mov	w0, 14
	bl	dprint_vfmw
	b	.L3410
.L3490:
	add	w1, w1, 1
	mov	x0, x24
	str	w1, [x23, 3716]
	mov	w19, 0
	bl	UpdateReflist
	mov	x0, x24
	bl	UpdateLTReflist
	b	.L3491
.L3599:
	ldr	w3, [x23, 3712]
	b	.L3465
.L3606:
	add	x23, x24, 24576
	ldr	w4, [x23, 3716]
	cbz	w4, .L3457
	ldr	x0, [x24, 27904]
	cmp	x3, x0
	beq	.L3612
	add	x0, x23, 3336
	mov	w1, 0
	.p2align 2
.L3462:
	add	w1, w1, 1
	cmp	w1, w4
	beq	.L3457
	ldr	x2, [x0], 8
	cmp	x3, x2
	bne	.L3462
.L3458:
	add	x2, x22, 112
	mov	x0, x24
	bl	InsertFrmInDPB
	cmn	w0, #1
	mov	w19, w0
	beq	.L3613
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
	b	.L3491
.L3605:
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 4]
	ldr	x0, [x22, 752]
	strb	wzr, [x0, 1]
	b	.L3409
.L3604:
	mov	x1, x20
	mov	x0, x24
	bl	SetFrmRepeatCount.part.1
	b	.L3435
.L3595:
	mov	w0, 3
	strb	w0, [x25, 1]
	ldr	w1, [x25, 664]
	ldr	w0, [x23, 692]
	add	w0, w1, w0
	str	w0, [x25, 664]
	b	.L3422
.L3442:
	mov	x0, x24
	bl	IDRMemMarking
	cbz	w0, .L3446
	mov	w0, -1
	b	.L3574
	.p2align 3
.L3609:
	str	w3, [x2, 652]
	ldr	x2, [x1, 28032]
	strb	wzr, [x2, 4]
	ldr	x2, [x1, 28032]
	b	.L3487
.L3610:
	ldrb	w0, [x22, 115]
	cbz	w0, .L3477
	ldr	x0, [x22, 752]
	str	x0, [x22, 1592]
	b	.L3477
.L3613:
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
	b	.L3493
.L3596:
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
	b	.L3493
.L3611:
	ldr	x1, .LC831
	mov	w3, w0
	mov	w2, 4087
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3479
.L3498:
	mov	w1, 0
	b	.L3469
.L3607:
	ldr	x1, .LC831
	mov	w3, w0
	mov	w2, 3987
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3461
.L3612:
	mov	w1, 0
	b	.L3458
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
	cbz	w0, .L3615
	ldrb	w1, [x20, 1586]
	cbz	w1, .L3683
	add	x21, x26, 65536
	ldr	w22, [x21, 44]
	cmp	w22, 2
	beq	.L3641
	add	x25, x26, 61440
	mov	w22, 2
	ldrb	w1, [x25, 1552]
	strb	wzr, [x25, 113]
	cmp	w1, 1
	bne	.L3643
	b	.L3640
	.p2align 3
.L3615:
	add	x21, x26, 65536
	ldr	w0, [x21, 44]
	cmp	w0, 2
	beq	.L3684
	add	x25, x26, 61440
	mov	w22, 0
	strb	wzr, [x25, 113]
.L3643:
	ldr	w3, [x25, 2772]
	strb	wzr, [x25, 112]
	cbz	w3, .L3634
	add	x19, x26, 28672
	mov	x24, 0
	add	x19, x19, 560
	.p2align 2
.L3633:
	ldrb	w0, [x19, 1]
	cbnz	w0, .L3628
	cbz	x19, .L3629
	add	x0, x26, 24576
	ldr	w2, [x0, 3716]
	cbz	w2, .L3629
	ldr	x0, [x26, 27904]
	cmp	x0, x19
	beq	.L3628
	sub	w2, w2, #1
	mov	x0, x26
	add	x2, x26, x2, lsl 3
	b	.L3631
	.p2align 3
.L3632:
	ldr	x1, [x0, 27904]
	cmp	x1, x19
	beq	.L3628
.L3631:
	cmp	x0, x2
	add	x0, x0, 8
	bne	.L3632
.L3629:
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
	bne	.L3634
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
	beq	.L3635
	add	x1, x0, 8
	ldr	x0, [x26]
	ldr	w0, [x0, 28]
	cmp	w0, 25
	beq	.L3685
.L3636:
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
.L3625:
	cmp	w0, wzr
	uxtb	w1, w22
	mov	w2, 3
	strb	w1, [x25, 115]
	csel	w0, w0, w2, eq
	strb	w0, [x3, 680]
	ldrb	w0, [x25, 113]
	cmp	w0, 1
	beq	.L3686
	strb	w1, [x25, 1552]
	ldr	w0, [x20, 1604]
	str	w0, [x25, 1560]
	ldr	w0, [x20, 1616]
	str	w0, [x25, 1556]
	ldr	w0, [x21, 156]
	str	w0, [x25, 1568]
	str	w4, [x25, 1572]
	cbnz	w22, .L3687
	strb	wzr, [x25, 1553]
	mov	w0, 0
.L3675:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldr	x27, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L3683:
	add	x21, x26, 65536
	ldr	w1, [x21, 44]
	cmp	w1, 2
	beq	.L3688
	add	x25, x26, 61440
	mov	w22, 1
	ldrb	w1, [x25, 1552]
	strb	wzr, [x25, 113]
	cmp	w1, 2
	bne	.L3643
.L3640:
	ldr	w1, [x20, 1604]
	ldr	w2, [x25, 1560]
	cmp	w2, w1
	bne	.L3643
	ldr	w1, [x20, 1616]
	cbnz	w1, .L3619
	ldr	w1, [x25, 1556]
	cbnz	w1, .L3643
.L3620:
	ldp	w2, w1, [x21, 152]
	sub	w1, w1, w2
	cmp	w1, 2
	bhi	.L3643
	ldr	x3, [x25, 752]
	mov	w1, 1
	strb	w1, [x25, 113]
	cbz	x3, .L3622
	cmp	w22, 2
	beq	.L3689
	cmp	w22, 1
	mov	w4, 0
	bne	.L3625
	ldrb	w1, [x3, 1]
	cmp	w1, 2
	beq	.L3625
.L3622:
	strb	wzr, [x25, 113]
	b	.L3643
	.p2align 3
.L3684:
	mov	w22, 0
.L3641:
	add	x25, x26, 61440
	strb	wzr, [x25, 113]
	b	.L3643
	.p2align 3
.L3628:
	add	w24, w24, 1
	add	x19, x19, 808
	cmp	w24, w3
	bne	.L3633
.L3634:
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
.L3687:
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
.L3688:
	mov	w22, 1
	b	.L3641
.L3686:
	strb	wzr, [x25, 1552]
	mov	w0, 0
	ldr	w1, [x20, 1604]
	str	w1, [x25, 1560]
	ldr	w1, [x20, 1616]
	str	w1, [x25, 1556]
	ldr	w1, [x21, 156]
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
.L3685:
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
	b	.L3636
.L3619:
	ldr	w1, [x25, 1556]
	cbz	w1, .L3643
	b	.L3620
.L3635:
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
	b	.L3675
.L3689:
	ldrb	w0, [x3, 1]
	cmp	w0, 1
	bne	.L3622
	ldrb	w0, [x20, 1585]
	mov	w4, 0
	b	.L3625
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
	cbz	w0, .L3714
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
	bhi	.L3715
.L3694:
	cbz	w0, .L3705
	sub	w3, w0, #1
	mov	w22, 0
	add	x3, x3, 3489
	add	x0, x25, 3328
	add	x3, x19, x3, lsl 3
	.p2align 2
.L3697:
	ldr	x2, [x0]
	add	x0, x0, 8
	ldr	w1, [x2, 648]
	cmp	w22, w1
	bge	.L3696
	ldrb	w2, [x2, 4]
	cmp	w2, wzr
	csel	w22, w22, w1, eq
.L3696:
	cmp	x0, x3
	bne	.L3697
.L3695:
	ldr	w0, [x29, 120]
	mov	w23, 1
	str	wzr, [x29, 124]
	mov	w24, w23
	cmp	w0, w26
	bne	.L3710
	b	.L3702
	.p2align 3
.L3700:
	mov	x1, 0
	mov	x0, x19
	strb	wzr, [x27, 116]
	bl	StorePicInDPB
	add	w1, w26, 1
	cbnz	w0, .L3716
.L3701:
	sdiv	w0, w1, w21
	str	w26, [x20, 1600]
	msub	w26, w0, w21, w1
	ldr	w0, [x29, 120]
	cmp	w0, w26
	beq	.L3702
	ldr	w0, [x25, 3716]
	ldr	w1, [x29, 124]
	cmp	w1, w0
	bhi	.L3702
.L3710:
	mov	w1, 1
	mov	x0, x19
	bl	AllocFrameStore
	cbnz	w0, .L3717
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
	cbz	w0, .L3700
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
	bls	.L3700
	cmp	w0, w22
	ldr	w0, [x29, 124]
	mov	x1, 0
	strb	wzr, [x27, 116]
	csinc	w0, w0, w0, ls
	str	w0, [x29, 124]
	mov	x0, x19
	bl	StorePicInDPB
	add	w1, w26, 1
	cbz	w0, .L3701
.L3716:
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
.L3715:
	mov	w0, w1
	str	w1, [x25, 3716]
	b	.L3694
	.p2align 3
.L3714:
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
.L3702:
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
.L3717:
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
.L3705:
	mov	w22, 0
	b	.L3695
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
	mov	x2, 208
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
	beq	.L3728
	mov	w2, 32
	mov	w1, 256
	mov	w0, 136
	str	w2, [x19, 14944]
	str	w0, [x19, 14940]
	str	w1, [x19, 14948]
.L3720:
	ldr	w0, [x21, 624]
	mov	w23, 2208
	add	x20, x19, 65536
	bl	MEM_Phy2Vir
	ldr	x1, [x19]
	mov	w2, 4000
	ldr	w4, [x19, 14944]
	ldr	w5, [x19, 14948]
	ldrsw	x3, [x1, 620]
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
	beq	.L3729
	ldr	w6, [x21, 648]
	cmp	w6, wzr
	ccmp	x3, xzr, 4, eq
	ccmp	x1, xzr, 4, ne
	beq	.L3723
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
.L3723:
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
	str	wzr, [x20, 148]
	str	wzr, [x1, 2792]
	str	w2, [x1, 2796]
	bl	VCTRL_GetChanIDByCtx
	cmn	w0, #1
	str	w0, [x20, 116]
	beq	.L3730
	mov	w1, 0
	mov	x0, x19
	bl	ClearAll
	str	wzr, [x20, 144]
	mov	w0, 1
	ldr	x1, [x19]
	ldr	w1, [x1, 664]
	str	w1, [x20, 120]
.L3722:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3728:
	ldr	w0, [x21, 724]
	str	w0, [x19, 14944]
	ldr	w0, [x21, 728]
	str	w0, [x19, 14948]
	ldr	w0, [x21, 716]
	str	w0, [x19, 14940]
	b	.L3720
.L3729:
	ldr	x2, .LC862
	mov	w3, 13089
	ldr	x1, .LC864
	mov	w0, 0
	add	x2, x2, 272
	bl	dprint_vfmw
	mov	w0, -20
	b	.L3722
	.p2align 3
.L3730:
	ldr	x1, .LC866
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -20
	b	.L3722
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
	add	x22, x21, 65536
	bl	_mcount
	mov	w20, 4000
	mov	w1, 2208
	ldr	x4, [x21, 14968]
	ldr	w0, [x23, 1596]
	ldr	w3, [x22, 128]
	ldr	x2, [x21, 14960]
	umull	x0, w0, w1
	cmp	w3, 1
	add	x25, x4, x0
	ldr	w1, [x25, 28]
	umull	x1, w1, w20
	add	x20, x2, x1
	beq	.L3868
	add	x19, x21, 61440
	ldrb	w0, [x19, 112]
	cbz	w0, .L3735
	ldr	x0, [x19, 752]
	cbz	x0, .L3736
	ldrb	w1, [x0, 1]
	cbz	w1, .L3869
	strb	wzr, [x19, 112]
.L3735:
	add	x1, x29, 108
	mov	x0, x21
	bl	GetReRangeFlag
	mov	w26, w0
	ldr	w1, [x21, 15012]
	ldr	w0, [x20, 36]
	cmp	w1, w0
	beq	.L3870
.L3738:
	ldr	x1, .LC870
	add	x0, x21, 12288
	strb	wzr, [x20, 24]
	mov	x2, 4000
	add	x0, x0, 2688
	ldr	x3, [x1, 112]
	mov	x1, x20
	blr	x3
.L3739:
	ldr	w0, [x29, 108]
	cbnz	w0, .L3871
	ldr	x0, [x21]
	ldr	w0, [x0, 912]
	cmp	w0, 1
	beq	.L3741
	add	x24, x21, 16384
.L3751:
	ldr	w1, [x24, 2616]
	ldr	w0, [x25, 24]
	cmp	w1, w0
	bne	.L3762
	ldrb	w0, [x25, 20]
	cbnz	w0, .L3762
.L3763:
	ldr	w0, [x23, 728]
	ldr	w3, [x24, 788]
	cbz	w0, .L3764
	str	wzr, [x23, 1600]
	str	wzr, [x19, 1560]
.L3764:
	ldr	w0, [x22, 44]
	cmp	w0, 2
	beq	.L3765
	ldr	x2, [x21]
	ldr	w0, [x2, 12]
	sub	w0, w0, #2
	cmp	w0, 1
	bls	.L3765
	ldr	w1, [x23, 1600]
	ldr	w4, [x23, 1604]
	cmp	w4, w1
	beq	.L3766
	mov	w0, 1
	add	w3, w3, 4
	lsl	w3, w0, w3
	add	w5, w1, w0
	udiv	w0, w5, w3
	msub	w0, w0, w3, w5
	cmp	w4, w0
	beq	.L3803
	ldr	w0, [x2, 640]
	cbnz	w0, .L3767
	ldr	w0, [x2, 680]
	cmp	w0, 1
	beq	.L3872
.L3768:
	cmp	w4, w1
	bhi	.L3873
	sub	w0, w4, w1
	ldr	w1, [x24, 1864]
	add	w0, w0, w3
	cmp	w1, w0
	bcs	.L3767
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	beq	.L3777
.L3782:
	ldr	w0, [x22, 112]
	cbnz	w0, .L3767
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
	cbnz	w0, .L3783
	mov	x0, x21
	bl	ClearDPB
.L3783:
	ldr	w0, [x23, 1600]
	mov	w2, 6922
	ldr	w3, [x23, 1604]
	ldr	x1, .LC884
	ldr	w4, [x24, 1864]
	sub	w3, w3, w0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3871:
	ldr	x0, [x21]
	ldr	w1, [x0, 748]
	cbnz	w1, .L3741
	ldr	w0, [x0, 28]
	cmp	w0, 25
	bne	.L3741
	ldr	w0, [x22, 76]
	add	w0, w0, 1
	str	w0, [x22, 76]
	cbnz	w26, .L3874
.L3742:
	cbz	x21, .L3801
	mov	x0, x21
	bl	ClearAllSlice
	mov	x0, x21
	bl	InitDPB.part.14
	mov	w3, w0
	cbnz	w0, .L3743
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
	cbz	w26, .L3745
	ldr	w1, [x21, 14932]
	cbnz	w1, .L3745
	ldr	w4, [x24, 1864]
	str	w4, [x19, 2776]
	ldr	x3, [x21]
	ldr	w1, [x3, 708]
	cmp	w28, w1
	bhi	.L3746
	ldr	w2, [x3, 712]
	cmp	w27, w2
	bhi	.L3746
	add	x5, x21, 24576
	ldr	w6, [x3, 732]
	ldr	w5, [x5, 3712]
	cmp	w6, w5
	bcc	.L3875
	ldr	w0, [x3, 736]
	cbnz	w0, .L3749
	mov	w4, 1
	str	w4, [x19, 2776]
	ldr	w1, [x3, 708]
	ldr	w2, [x3, 712]
.L3749:
	add	x3, x19, 2772
	mov	x0, x21
	bl	H264_ArrangeVahbMem
	cmp	w0, 1
	bne	.L3865
.L3745:
	lsr	w28, w28, 4
	lsr	w27, w27, 4
	str	w28, [x21, 14932]
	str	w27, [x21, 14936]
	b	.L3751
	.p2align 3
.L3870:
	ldrb	w0, [x20, 24]
	cbz	w0, .L3739
	b	.L3738
	.p2align 3
.L3741:
	ldr	w0, [x22, 76]
	add	w0, w0, 1
	str	w0, [x22, 76]
	cbnz	w26, .L3752
	ldr	x0, [x21]
	ldr	w0, [x0, 912]
	cmp	w0, 1
	beq	.L3752
	mov	x0, x21
	bl	FlushDPB
	cbnz	w0, .L3876
.L3755:
	cbz	x21, .L3802
.L3878:
	mov	x0, x21
	bl	ClearAllSlice
	mov	x0, x21
	bl	InitDPB.part.14
	cbnz	w0, .L3756
	cbnz	w26, .L3758
	ldr	x0, [x21]
	add	x24, x21, 16384
	ldr	w0, [x0, 912]
	cmp	w0, 1
	bne	.L3751
.L3758:
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
	beq	.L3877
.L3865:
	ldr	x0, [x21]
	ldr	w0, [x0, 912]
	cmp	w0, 1
	bne	.L3750
	mov	w0, -2
	b	.L3734
	.p2align 3
.L3752:
	ldr	x1, .LC881
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x22, 192]
	cmp	w0, 1
	bne	.L3755
	mov	x0, x21
	bl	GetBackPicFromVOQueue
	cbnz	x21, .L3878
	.p2align 2
.L3802:
	mov	w0, -1
.L3756:
	ldr	x1, .LC873
	mov	w3, w0
	mov	w2, 6792
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3762:
	ldr	x0, .LC870
	mov	x2, 2208
	strb	wzr, [x25, 20]
	mov	x1, x25
	ldr	x3, [x0, 112]
	add	x0, x24, 2592
	blr	x3
	b	.L3763
.L3803:
	mov	w1, w4
	.p2align 2
.L3766:
	ldr	w0, [x23, 1616]
	cbz	w0, .L3785
	str	w1, [x23, 1600]
.L3785:
	mov	x0, x21
	str	w1, [x23, 768]
	bl	DecPOC
	mov	w1, 0
	mov	x0, x21
	bl	AllocFrameStore
	cbnz	w0, .L3879
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
	cbz	w2, .L3787
	strb	w4, [x23, 1585]
	mov	w2, w4
	mov	w4, 2
.L3787:
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
	cbnz	w2, .L3805
	ldrb	w0, [x25, 2707]
	mov	w1, 0
	cbnz	w0, .L3880
.L3788:
	strb	w0, [x19, 121]
	str	w1, [x22, 24]
	cbz	w3, .L3789
	str	w5, [x23, 1600]
.L3789:
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
	cbz	x0, .L3881
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
	cbz	x0, .L3791
	ldr	w3, [x19, 224]
	mov	w1, 0
	ldr	w2, [x0, 8]
	cmp	w3, wzr
	ldr	w0, [x22, 116]
	cset	w3, ne
	bl	FSP_SetStoreType
.L3791:
	ldr	w1, [x19, 2800]
	add	x4, x19, 688
	add	x5, x19, 720
.L3795:
	ldr	x0, [x4]
	cbnz	x0, .L3792
	cbz	w1, .L3792
	ldr	x0, [x19, 2808]
	subs	w1, w1, #1
	str	x0, [x4]
	beq	.L3793
	add	x3, x19, 2808
	mov	w2, 0
	.p2align 2
.L3794:
	add	w0, w2, 1
	mov	x2, x0
	cmp	w0, w1
	add	x0, x0, 4096
	add	x0, x21, x0, lsl 3
	ldr	x0, [x0, 31480]
	str	x0, [x3], 8
	bne	.L3794
.L3793:
	uxtw	x0, w1
	add	x0, x0, 4096
	add	x0, x21, x0, lsl 3
	str	xzr, [x0, 31480]
	str	w1, [x19, 2800]
.L3792:
	add	x4, x4, 8
	cmp	x5, x4
	bne	.L3795
	cbz	w1, .L3797
	cmp	w1, wzr
	ble	.L3797
	.p2align 2
.L3798:
	add	x23, x21, x1, sxtw 3
	sub	w20, w1, #1
	add	x23, x23, 61440
	ldr	w0, [x22, 116]
	ldr	x1, [x23, 2800]
	bl	FreeUsdByDec
	str	xzr, [x23, 2800]
	mov	w1, w20
	cbnz	w20, .L3798
.L3797:
	mov	x0, x21
	str	wzr, [x19, 796]
	str	wzr, [x19, 2800]
	bl	CalcPicNum
	mov	w0, 0
.L3734:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
.L3777:
	cbnz	w4, .L3779
	sub	w4, w3, #1
.L3780:
	mov	x0, x21
	str	w4, [x23, 1600]
	bl	FlushDPB
	cbnz	w0, .L3882
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	bne	.L3782
	.p2align 2
.L3767:
	mov	x0, x21
	bl	DecGap
	cbnz	w0, .L3883
.L3765:
	ldr	w1, [x23, 1604]
	b	.L3766
	.p2align 3
.L3868:
	ldrb	w2, [x20, 18]
	mov	w5, 2
	ldr	w7, [x20, 3252]
	add	x19, x21, 61440
	sub	w1, w5, w2
	ldrb	w6, [x23, 1585]
	madd	w1, w7, w1, w1
	add	w2, w6, 1
	udiv	w1, w1, w2
	str	w1, [x19, 812]
	ldrb	w2, [x20, 18]
	ldr	w7, [x20, 3252]
	sub	w2, w5, w2
	madd	w2, w7, w2, w2
	str	w2, [x19, 816]
	ldr	w2, [x20, 3248]
	add	w2, w2, 1
	str	w2, [x19, 808]
	mul	w1, w1, w2
	str	w1, [x19, 820]
	ldrb	w0, [x4, x0]
	strb	w0, [x19, 120]
	cbnz	w6, .L3799
	ldrb	w0, [x20, 19]
	cbnz	w0, .L3884
	mov	w3, 0
	b	.L3733
	.p2align 3
.L3805:
	mov	w1, 0
	mov	w0, 0
	b	.L3788
	.p2align 3
.L3799:
	mov	w3, 0
	mov	w0, 0
.L3733:
	strb	w0, [x19, 121]
	mov	w0, 0
	str	w3, [x22, 24]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L3880:
	mov	w1, 1
	mov	w0, w1
	b	.L3788
	.p2align 3
.L3869:
	ldr	w1, [x0, 632]
	mov	w2, 1
	ldr	w0, [x22, 116]
	bl	FSP_ClearLogicFs
	ldr	x0, [x19, 752]
	strb	wzr, [x0, 1]
	ldr	x0, [x19, 752]
	strb	wzr, [x0, 4]
	strb	wzr, [x19, 112]
	b	.L3735
	.p2align 3
.L3877:
	lsr	w27, w27, 4
	lsr	w26, w26, 4
	str	w27, [x21, 14932]
	str	w26, [x21, 14936]
	b	.L3751
	.p2align 3
.L3750:
	ldr	x1, .LC879
	mov	w0, 0
	str	wzr, [x21, 14932]
	str	wzr, [x21, 14936]
	bl	dprint_vfmw
	mov	w1, 0
	mov	x0, x21
	bl	ClearAll
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3884:
	mov	w0, w3
	b	.L3733
.L3873:
	ldr	w0, [x24, 1864]
	sub	w1, w4, w1
	cmp	w0, w1
	bcs	.L3770
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	beq	.L3771
.L3775:
	ldr	w0, [x22, 112]
	cbnz	w0, .L3767
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
	cbz	w0, .L3885
.L3776:
	ldr	w0, [x23, 1600]
	mov	w2, 6890
	ldr	w3, [x23, 1604]
	ldr	x1, .LC884
	ldr	w4, [x24, 1864]
	sub	w3, w3, w0
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3801:
	mov	w3, -1
.L3743:
	ldr	x1, .LC873
	mov	w2, 6666
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3874:
	mov	x0, x21
	bl	FlushDPB
	cbz	w0, .L3742
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6657
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3876:
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6783
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
.L3883:
	ldr	x1, .LC886
	mov	w2, 6934
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3736:
	ldr	x2, .LC867
	mov	w3, 6623
	ldr	x1, .LC869
	mov	w0, 22
	add	x2, x2, 288
	bl	dprint_vfmw
	strb	wzr, [x19, 112]
	b	.L3735
.L3770:
	ldrb	w0, [x23, 1584]
	cmp	w0, 2
	bne	.L3775
	b	.L3767
.L3872:
	ldr	w0, [x2, 676]
	cbnz	w0, .L3768
	ldr	w0, [x2, 684]
	cbnz	w0, .L3768
	ldr	w0, [x2, 644]
	add	w0, w0, 1024
	cmp	w0, 2048
	bhi	.L3767
	b	.L3768
.L3879:
	ldr	x1, .LC888
	mov	w2, 6958
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
.L3771:
	cbnz	w4, .L3773
	sub	w4, w3, #1
.L3774:
	mov	x0, x21
	str	w4, [x23, 1600]
	bl	FlushDPB
	cbz	w0, .L3770
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6873
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
	.p2align 3
.L3881:
	ldr	x2, .LC867
	mov	w3, 7045
	ldr	x1, .LC894
	mov	w0, 22
	add	x2, x2, 288
	bl	dprint_vfmw
	mov	w0, w23
	b	.L3734
.L3773:
	sub	w4, w4, #1
	b	.L3774
.L3779:
	sub	w4, w4, #1
	b	.L3780
.L3746:
	ldr	x1, .LC875
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
.L3875:
	ldr	x1, .LC877
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
.L3885:
	mov	x0, x21
	bl	ClearDPB
	b	.L3776
.L3882:
	ldr	x1, .LC871
	mov	w3, w0
	mov	w2, 6906
	mov	w0, 0
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3734
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
	cbnz	w0, .L3935
	mov	x0, x19
	bl	ProcessSliceHeaderSecondPart
	cbnz	w0, .L3935
	ldr	w2, [x20, 40]
	mov	w3, 0
	add	x22, x19, 61440
	cbz	w2, .L3893
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
.L3901:
	ldr	x1, [x4]
	ldr	x6, [x1, 8]
	cbz	x6, .L3916
	ldr	x5, [x1, 40]
	ldr	w7, [x1, 16]
	cbz	x5, .L3917
.L3942:
	ldr	w1, [x1, 48]
.L3896:
	cbz	x0, .L3897
	cmp	x6, x0
	mov	w8, w7
	bls	.L3898
	sub	x0, x6, x0
	sub	w0, w0, w10
	add	w8, w0, w7
.L3898:
	add	w3, w1, w3
	add	w3, w8, w3
.L3899:
	cbz	x5, .L3936
	mov	x0, x5
	mov	w10, w1
.L3900:
	add	x4, x4, 8
	cmp	x9, x4
	bne	.L3901
.L3893:
	str	w3, [x20, 100]
	ldr	w0, [x22, 808]
	ldr	w4, [x22, 812]
	mul	w4, w0, w4
	cmp	w4, 1620
	bgt	.L3937
	lsl	w0, w4, 2
	mov	w1, 32769
	sub	w4, w0, w4
	mov	w0, 16384
	lsl	w4, w4, 8
	cmp	w4, w1
	asr	w4, w4, 1
	csel	w4, w4, w0, gt
.L3903:
	ldr	w0, [x19, 14940]
	cmp	w0, w2
	bls	.L3904
	cmp	w4, w3
	bcc	.L3904
	ldrb	w0, [x21, 1589]
	cbz	w0, .L3907
	ldr	x1, .LC903
	mov	w0, 2
	bl	dprint_vfmw
	ldrb	w0, [x21, 1587]
	cmp	w0, 5
	ldr	w0, [x20, 80]
	beq	.L3938
.L3909:
	add	w0, w0, 1
	str	wzr, [x20, 96]
	str	w0, [x20, 80]
	ldrb	w0, [x22, 115]
	cbnz	w0, .L3910
	ldr	w0, [x20, 84]
	add	w0, w0, 1
	str	w0, [x20, 84]
.L3911:
	mov	x0, x19
	bl	InitPic
	cmn	w0, #2
	beq	.L3890
	cbnz	w0, .L3939
	mov	x0, x19
	bl	WritePicMsg
.L3907:
	mov	x0, x19
	bl	PicTypeStatistic
	ldr	w0, [x20, 128]
	cbnz	w0, .L3913
	mov	x0, x19
	bl	DecList
	cbnz	w0, .L3940
	mov	x0, x19
	bl	IsRefListWrong
	cbnz	w0, .L3941
	mov	x0, x19
	bl	ExitSlice
.L3913:
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
.L3890:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L3936:
	mov	x0, x6
	mov	w10, w7
	b	.L3900
	.p2align 3
.L3897:
	add	w3, w7, w3
	add	w3, w3, w1
	b	.L3899
	.p2align 3
.L3916:
	ldr	x5, [x1, 40]
	mov	w7, 0
	cbnz	x5, .L3942
	.p2align 2
.L3917:
	mov	w1, 0
	b	.L3896
	.p2align 3
.L3937:
	lsl	w4, w4, 8
	b	.L3903
	.p2align 3
.L3910:
	ldr	w0, [x20, 88]
	add	w0, w0, 1
	str	w0, [x20, 88]
	b	.L3911
	.p2align 3
.L3938:
	str	w0, [x20, 92]
	b	.L3909
.L3935:
	ldr	x1, .LC896
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC897
	ldr	x4, [x0]
	cbz	x4, .L3933
	add	x2, x29, 64
	ldr	w0, [x20, 116]
	mov	w5, -1
	mov	w3, 4
	mov	w1, 100
	str	w5, [x2, -8]!
	blr	x4
.L3933:
	mov	x0, x19
	bl	ClearCurrSlice
	mov	w0, -1
	b	.L3890
.L3904:
	ldr	x1, .LC899
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w1, [x19, 14940]
	ldr	w2, [x20, 40]
	cmp	w2, w1
	bcc	.L3906
	ldr	x0, .LC897
	ldr	x4, [x0]
	cbz	x4, .L3906
	ldr	w0, [x20, 116]
	mov	w3, 8
	stp	w2, w1, [x29, 56]
	add	x2, x29, 56
	mov	w1, 108
	blr	x4
.L3906:
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
	b	.L3890
.L3940:
	ldr	x1, .LC907
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3890
.L3939:
	ldr	x1, .LC905
	mov	w0, 1
	bl	dprint_vfmw
	mov	w0, -1
	b	.L3890
.L3941:
	ldr	x1, .LC909
	mov	w0, 1
	bl	dprint_vfmw
	b	.L3933
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
	cbz	w0, .L3948
	ldr	w2, [x19, 100]
	mov	w0, 0
	.p2align 2
.L3947:
	ubfiz	x1, x0, 5, 32
	add	w0, w0, 1
	add	x1, x3, x1
	ldr	w1, [x1, 16]
	add	w2, w2, w1
	str	w2, [x19, 100]
	ldr	w1, [x3, 76]
	cmp	w1, w0
	bhi	.L3947
.L3948:
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
	beq	.L4100
	add	w0, w0, 1
	str	w0, [x19, 60]
	ldr	x0, [x20, 632]
	ldrb	w2, [x0, 2]
	cmp	w2, 20
	beq	.L4101
.L3949:
	sub	w0, w2, #1
	cmp	w0, 29
	bls	.L4102
.L3955:
	ldr	x1, .LC952
	mov	w3, w22
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x21, .LC925
	cmp	w22, 256
	beq	.L3984
	ldr	x4, [x21]
	cbz	x4, .L3985
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 102
	blr	x4
.L3984:
	ldr	x1, [x20, 632]
	cbz	x1, .L3986
.L3987:
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3986:
	ldr	x4, [x21]
	cbz	x4, .L4088
.L4087:
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L4088:
	mov	w0, 0
.L4103:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4102:
	ldr	x1, .LC920
	ldrh	w0, [x1,w0,uxtw #1]
	adr	x1, .Lrtx3957
	add	x0, x1, w0, sxth #2
	br	x0
.Lrtx3957:
	.section	.rodata
	.align	0
	.align	2
.L3957:
	.2byte	(.L3956 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3956 - .Lrtx3957) / 4
	.2byte	(.L3958 - .Lrtx3957) / 4
	.2byte	(.L3959 - .Lrtx3957) / 4
	.2byte	(.L3960 - .Lrtx3957) / 4
	.2byte	(.L3961 - .Lrtx3957) / 4
	.2byte	(.L3962 - .Lrtx3957) / 4
	.2byte	(.L3963 - .Lrtx3957) / 4
	.2byte	(.L3964 - .Lrtx3957) / 4
	.2byte	(.L3965 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3959 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3966 - .Lrtx3957) / 4
	.2byte	(.L3956 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3955 - .Lrtx3957) / 4
	.2byte	(.L3967 - .Lrtx3957) / 4
	.text
.L3967:
	ldr	x1, .LC950
	mov	w2, 30
	mov	w0, 22
	bl	dprint_vfmw
	cbnz	w21, .L3982
	ldr	x1, [x20, 632]
	cbz	x1, .L3982
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3982:
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
	bne	.L4088
	ldr	x21, .LC925
	ldr	x4, [x21]
	cbz	x4, .L3983
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3983:
	mov	x0, x20
	str	wzr, [x20, 624]
	bl	DecVDM
	cbz	w0, .L4088
.L4095:
	ldr	x4, [x21]
	cbz	x4, .L4084
.L4085:
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
.L4084:
	mov	w0, -1
.L4107:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L3966:
	ldr	x1, .LC948
	mov	w2, 19
	mov	w0, 22
	bl	dprint_vfmw
.L4093:
	ldr	x1, [x20, 632]
	cbz	x1, .L4088
.L4086:
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
	mov	w0, 0
.L4082:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
.L3964:
	ldr	x1, .LC942
	mov	w2, 12
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4086
	mov	w0, 0
	b	.L4103
.L3965:
	ldr	x1, .LC946
	mov	w2, 13
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4086
	mov	w0, 0
	b	.L4103
.L3963:
	ldr	x1, .LC944
	mov	w2, 11
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_IsEosNal
	uxtb	w1, w0
	cmp	w1, 1
	bne	.L4093
	ldr	x21, .LC925
	strb	w0, [x24, 2640]
	ldr	x4, [x21]
	cbz	x4, .L3981
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 117
	blr	x4
.L3981:
	mov	x0, x20
	str	wzr, [x20, 624]
	bl	DecVDM
	cbz	w0, .L4093
	b	.L4095
.L3962:
	ldr	x1, .LC940
	mov	w2, 10
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4086
	mov	w0, 0
	b	.L4103
.L3961:
	ldr	x1, .LC938
	mov	w2, 9
	mov	w0, 22
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbnz	x1, .L4086
	mov	w0, 0
	b	.L4103
.L3960:
	ldr	x1, .LC927
	mov	w2, 8
	ldr	w3, [x19, 68]
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_DecPPS
	mov	w21, w0
	ldr	x1, [x20, 632]
	cbz	x1, .L3974
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3974:
	cbnz	w21, .L4104
	ldr	w1, [x19, 68]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 68]
	b	.L4082
.L3959:
	ldr	x1, .LC930
	mov	w0, 22
	ldr	w3, [x19, 64]
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_DecSPS
	mov	w21, w0
	ldr	x1, [x20, 632]
	cbz	x1, .L3976
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3976:
	cbnz	w21, .L4105
	ldr	w1, [x19, 64]
	mov	w0, 0
	add	w1, w1, 1
	str	w1, [x19, 64]
	b	.L4082
.L3958:
	ldr	x1, .LC934
	mov	w2, 6
	mov	w0, 22
	bl	dprint_vfmw
	mov	x0, x20
	bl	H264_DecSEI
	mov	w21, w0
	ldr	x1, [x20, 632]
	cbz	x1, .L3978
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
.L3978:
	cbz	w21, .L4088
	ldr	x1, .LC936
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x0, .LC925
	ldr	x4, [x0]
	cbnz	x4, .L4087
	mov	w0, 0
	b	.L4103
.L3956:
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
	cbnz	w0, .L4106
	mov	x0, x20
	bl	H264_DecSlice
	cmn	w0, #2
	beq	.L4082
	cbz	w0, .L3970
	ldr	x0, .LC925
	ldr	x4, [x0]
	cbnz	x4, .L4085
	mov	w0, -1
	b	.L4107
	.p2align 3
.L4100:
	ldr	x1, .LC913
	mov	w0, 22
	bl	dprint_vfmw
	ldr	w0, [x19, 60]
	add	w0, w0, 1
	str	w0, [x19, 60]
	ldr	x0, [x20, 632]
	ldrb	w2, [x0, 2]
	cmp	w2, 20
	bne	.L3949
	.p2align 2
.L4101:
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
	cbz	w1, .L3950
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
.L3950:
	mov	x1, 16711680
	ldr	x3, [x0, 1600]
	movk	x1, 0xff, lsl 32
	and	x1, x3, x1
	cmp	x1, 65536
	beq	.L3951
	ldrb	w3, [x0, 1602]
	ldrb	w2, [x0, 1604]
	mov	w0, 1
	ldr	x1, .LC915
	bl	dprint_vfmw
	ldr	x1, [x20, 632]
	cbz	x1, .L4084
	ldr	w0, [x19, 116]
	bl	ReleaseNAL
	str	xzr, [x20, 632]
	mov	w0, -1
	b	.L4082
.L3951:
	ldrb	w1, [x0, 1600]
	cmp	w1, 1
	beq	.L4108
.L3954:
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
	b	.L3949
.L4106:
	ldr	x1, .LC924
	mov	w0, 1
	bl	dprint_vfmw
	mov	x0, x20
	bl	ClearCurrSlice
	ldr	x0, .LC925
	ldr	x4, [x0]
	cbnz	x4, .L4085
	mov	w0, -1
	b	.L4107
.L4105:
	ldr	x1, .LC932
.L4098:
	mov	w0, 1
	bl	dprint_vfmw
	ldr	x21, .LC925
	ldr	x4, [x21]
	cbz	x4, .L4084
	add	x2, x29, 80
	ldr	w0, [x19, 116]
	mov	w20, -1
	mov	w3, 4
	mov	w1, 100
	str	w20, [x2, -8]!
	blr	x4
	ldr	x4, [x21]
	cbz	x4, .L4084
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	blr	x4
	mov	w0, w20
	b	.L4082
.L4104:
	ldr	x1, .LC928
	b	.L4098
.L4108:
	mov	w1, 5
	strb	w1, [x2, 2]
	b	.L3954
.L3985:
	ldr	x1, [x20, 632]
	cbnz	x1, .L3987
	mov	w0, 0
	b	.L4103
.L3970:
	ldrb	w0, [x21, 1592]
	cmp	w0, 1
	bne	.L4088
	ldr	x21, .LC925
	ldr	x4, [x21]
	cbz	x4, .L3972
	ldr	w0, [x19, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 112
	blr	x4
.L3972:
	mov	x0, x20
	str	wzr, [x20, 624]
	bl	DecVDM
	cbnz	w0, .L4095
	mov	w1, 1
	mov	w0, 0
	strb	w1, [x24, 2641]
	b	.L4082
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
	.xword	.L3957
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
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	mov	x22, x1
	bl	_mcount
	str	wzr, [x29, 100]
	stp	wzr, wzr, [x29, 92]
	cbz	x22, .L4175
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
	beq	.L4112
	ldr	w1, [x0, 872]
	cmp	w1, 1
	beq	.L4176
.L4113:
	mov	x1, x22
	mov	x0, x19
	bl	ReceivePacket
	ldr	x1, [x19, 632]
	str	x1, [x20, 184]
.L4115:
	str	wzr, [x21, 2784]
	cbz	w0, .L4114
.L4136:
	mov	w22, 0
.L4172:
	mov	w0, w22
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L4176:
	ldr	w0, [x0, 912]
	cmp	w0, 1
	bne	.L4113
	ldr	w0, [x20, 176]
	cmp	w0, 1
	bne	.L4113
	ldr	x0, [x20, 184]
	str	x0, [x19, 632]
	str	wzr, [x20, 176]
	str	wzr, [x21, 2784]
	.p2align 2
.L4114:
	mov	x0, x19
	bl	H264DEC_CheckSedOnly
	mov	w22, w0
	cbz	w0, .L4116
	ldr	x1, [x19, 632]
	cbz	x1, .L4139
	ldr	w0, [x20, 116]
	mov	w22, -1
	bl	ReleaseNAL
	str	xzr, [x19, 632]
	mov	w0, w22
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldr	x25, [sp, 64]
	ldp	x29, x30, [sp], 112
	ret
	.p2align 3
.L4112:
	ldr	x0, [x19, 632]
	cmp	x0, xzr
	csetm	w0, eq
	b	.L4115
	.p2align 3
.L4139:
	mov	w22, -1
	b	.L4172
	.p2align 3
.L4116:
	ldr	x0, [x19, 632]
	add	x24, x19, 12288
	add	x23, x24, 2600
	str	wzr, [x0, 72]
	mov	x0, x23
	ldr	x1, [x19, 632]
	ldr	w2, [x1, 16]
	ldr	x1, [x1, 8]
	bl	BsInit
	mov	w1, 32
	mov	x0, x23
	bl	BsGet
	uxtb	w0, w0
	ldr	x1, [x19, 632]
	and	w2, w0, 31
	ubfx	x3, x0, 5, 2
	strb	w2, [x1, 2]
	ldr	x1, [x19, 632]
	str	w3, [x1, 84]
	mov	w1, 27
	and	w0, w0, w1
	cmp	w0, 1
	ccmp	w2, 20, 4, ne
	beq	.L4177
.L4117:
	mov	x0, x19
	bl	FindTrailZeros
	cbnz	w0, .L4178
	ldr	x1, [x19, 632]
	mov	x0, x23
	str	wzr, [x1, 72]
	ldr	x1, [x19, 632]
	ldr	w2, [x1, 16]
	ldr	x1, [x1, 8]
	bl	BsInit
	mov	w1, 0
	mov	x0, x19
	bl	H264_DecOneNal
	cmn	w0, #2
	beq	.L4141
	cbnz	w0, .L4179
	ldr	w0, [x20, 120]
	cmp	w0, 1
	bne	.L4136
	mov	x0, x19
	bl	H264DEC_SetDecParam
	b	.L4172
	.p2align 3
.L4177:
	cmp	w2, 20
	beq	.L4180
.L4118:
	add	x2, x29, 108
	add	x1, x29, 104
	mov	x0, x19
	bl	InquireSliceProperty
	cbnz	w0, .L4119
	ldr	w1, [x29, 104]
	ldr	w0, [x20, 156]
	cmp	w1, 1
	csinc	w0, w0, w0, ne
	str	w0, [x20, 156]
	beq	.L4181
	ldrb	w0, [x24, 2641]
	cmp	w0, 1
	beq	.L4137
.L4123:
	ldr	w0, [x29, 108]
	cmp	w0, 1
	bne	.L4117
	mov	x0, x19
	bl	FlushDPB
	cbnz	w0, .L4182
.L4129:
	ldr	w0, [x20, 116]
	bl	FSP_GetFspType
	cbnz	w0, .L4117
	ldr	w0, [x20, 116]
	add	x3, x29, 100
	add	x2, x29, 96
	add	x1, x29, 92
	bl	VCTRL_GetChanImgNum
	ldr	x0, [x19]
	ldr	w0, [x0, 748]
	cmp	w0, 1
	bne	.L4117
	ldr	w1, [x29, 100]
	cmp	w1, wzr
	ble	.L4183
.L4130:
	str	wzr, [x20, 192]
.L4174:
	mov	w0, 1
	mov	w22, -2
	str	w0, [x21, 2784]
	b	.L4172
	.p2align 3
.L4119:
	ldr	x1, [x19, 632]
	cbz	x1, .L4133
	ldr	w0, [x20, 116]
	bl	ReleaseNAL
	str	xzr, [x19, 632]
.L4133:
	ldr	x0, .LC959
	ldr	x4, [x0]
	cbz	x4, .L4139
	ldr	w0, [x20, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 113
	mov	w22, -1
	blr	x4
	b	.L4172
.L4183:
	ldr	w1, [x29, 96]
	cmp	w1, 32
	bgt	.L4130
	ldr	x1, [x19, 616]
	cbnz	x1, .L4130
	ldr	w3, [x20, 80]
	cbz	w3, .L4117
	ldr	w6, [x19, 14932]
	cbz	w6, .L4117
	ldr	w7, [x19, 14936]
	ldr	w5, [x20, 196]
	cmp	w7, w5
	beq	.L4117
	ldr	w4, [x20, 200]
	cmp	w6, w4
	beq	.L4117
	ldr	x1, .LC958
	ldr	w2, [x29, 108]
	bl	dprint_vfmw
	str	wzr, [x20, 192]
	ldr	x0, .LC959
	ldr	w1, [x19, 14936]
	str	w1, [x20, 196]
	ldr	x4, [x0]
	ldr	w0, [x19, 14932]
	str	w0, [x20, 200]
	cbz	x4, .L4117
	ldr	w0, [x20, 116]
	mov	w3, 0
	mov	x2, 0
	mov	w1, 120
	blr	x4
	b	.L4117
	.p2align 3
.L4141:
	mov	w22, w0
	b	.L4172
	.p2align 3
.L4137:
	strb	wzr, [x24, 2641]
	b	.L4123
	.p2align 3
.L4181:
	add	x25, x19, 20480
	ldrb	w0, [x25, 1585]
	cmp	w0, 1
	ldr	w0, [x20, 160]
	beq	.L4184
	add	w0, w0, 2
	str	w0, [x20, 160]
.L4122:
	ldrb	w0, [x24, 2641]
	cmp	w0, 1
	beq	.L4137
	ldr	w0, [x20, 40]
	cbz	w0, .L4123
	ldr	x3, [x19, 632]
	mov	w2, 12
	ldr	x1, .LC955
	mov	x0, x23
	add	x1, x1, 432
	str	wzr, [x3, 72]
	bl	BsInit
	mov	w1, 1
	mov	x0, x19
	bl	H264_DecOneNal
	ldrb	w0, [x25, 1585]
	cmp	w0, 1
	ldr	w0, [x20, 160]
	beq	.L4185
	sub	w0, w0, #2
	str	w0, [x20, 160]
	b	.L4174
.L4180:
	mov	x0, x19
	bl	InquireSliceExtProperty
	b	.L4118
.L4178:
	ldr	x1, .LC961
	mov	w0, 22
	mov	w22, -1
	bl	dprint_vfmw
	b	.L4172
.L4175:
	ldr	x2, .LC953
	mov	w3, 13676
	ldr	x1, .LC954
	mov	w0, 0
	mov	w22, -1
	bl	dprint_vfmw
	b	.L4172
.L4184:
	add	w0, w0, 1
	str	w0, [x20, 160]
	b	.L4122
.L4182:
	ldr	x1, .LC956
	mov	w3, w0
	mov	w2, 13812
	mov	w0, 0
	bl	dprint_vfmw
	b	.L4129
.L4179:
	ldr	x1, .LC963
	mov	w0, 22
	mov	w22, -1
	bl	dprint_vfmw
	b	.L4172
.L4185:
	sub	w0, w0, #1
	str	w0, [x20, 160]
	b	.L4174
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
	stp	x25, x26, [sp, 64]
	mov	x19, x0
	mov	x0, x30
	add	x22, x19, 65536
	mov	w26, w1
	mov	x24, x3
	mov	w25, w4
	mov	w23, w5
	bl	_mcount
	ldr	w0, [x22, 144]
	cmp	w0, 1
	bls	.L4187
.L4190:
	ldr	x0, [x19]
	add	x20, x19, 61440
	ldr	w1, [x0, 8]
	cmp	w1, 2
	beq	.L4188
	ldr	w2, [x20, 808]
	lsl	w2, w2, 4
	cmp	w2, 1920
	bhi	.L4191
.L4188:
	ldr	w0, [x0, 664]
	str	w0, [x22, 120]
	ldr	w21, [x19, 624]
	str	w26, [x20, 804]
	cbnz	w21, .L4193
.L4214:
	cbz	w25, .L4194
	ldr	x5, .LC964
	sbfiz	x23, x23, 6, 32
	add	x23, x5, x23
	ldr	w0, [x23, 44]
	cmp	w0, 3
	beq	.L4213
	cmp	w0, 1
	beq	.L4194
	mov	x1, x24
	mov	x0, x19
	bl	StorePicInDPB
	cmp	w0, 1
	mov	w23, w0
	bls	.L4198
.L4217:
	ldr	x1, [x20, 752]
	mov	w2, 1
	ldr	w0, [x22, 116]
	mov	w22, -1
	mov	w21, w22
	ldr	w1, [x1, 632]
	bl	FSP_ClearLogicFs
	ldr	x1, .LC966
	mov	w2, w23
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
	stp	w22, wzr, [x5, 188]
	stp	wzr, w2, [x5, 196]
	str	w0, [x1, 3256]
	mov	x0, x19
	bl	ClearDPB
	str	xzr, [x20, 752]
.L4196:
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4191:
	str	wzr, [x22, 120]
	ldr	w21, [x19, 624]
	str	w26, [x20, 804]
	cbz	w21, .L4214
.L4193:
	mov	x0, x19
	bl	ClearAllSlice
	mov	x1, x24
	mov	x0, x19
	bl	StorePicInDPB
	cbnz	w0, .L4215
.L4199:
	mov	x0, x19
	bl	FlushDPB
	cbnz	w0, .L4216
.L4200:
	add	x0, x19, 20480
	str	wzr, [x20, 2780]
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
	mov	w21, 0
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4187:
	add	w0, w0, 1
	str	w0, [x22, 144]
	cmp	w0, 1
	bhi	.L4190
	add	x20, x19, 61440
	ldr	x0, [x19]
	b	.L4188
	.p2align 3
.L4194:
	mov	x0, x19
	bl	ClearAllSlice
	mov	x1, x24
	mov	x0, x19
	bl	StorePicInDPB
	cmp	w0, 1
	mov	w23, w0
	bhi	.L4217
.L4198:
	mov	w0, 1
	str	w0, [x20, 2780]
	mov	w0, w21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4216:
	ldr	x1, .LC970
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	b	.L4200
	.p2align 3
.L4215:
	ldr	x1, .LC968
	mov	w2, w0
	mov	w0, 1
	bl	dprint_vfmw
	b	.L4199
	.p2align 3
.L4213:
	mov	x0, x19
	bl	ClearAllSlice
	b	.L4196
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
	.global	H264DEC_VDMProcSedOnly
	.type	H264DEC_VDMProcSedOnly, %function
H264DEC_VDMProcSedOnly:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	mov	x21, x0
	mov	x0, x30
	mov	w20, w1
	add	x22, x21, 65536
	bl	_mcount
	sxtw	x3, w20
	ldr	x1, [x21, 616]
	lsl	x0, x3, 2
	ldr	x20, .LC972
	sub	x3, x0, x3
	ldr	x4, .LC971
	lsl	x0, x3, 4
	ldr	w2, [x1, 28]
	sub	x3, x0, x3
	ldr	w19, [x1, 20]
	ldrsw	x5, [x22, 116]
	mov	w0, 22
	add	x20, x20, x3, lsl 2
	ldr	x1, .LC974
	mul	w19, w2, w19
	ldr	x23, [x4, x5, lsl 3]
	sub	w19, w19, #1
	ldr	w3, [x20, 20]
	mov	w2, w19
	bl	dprint_vfmw
	ldr	w0, [x20, 20]
	cmp	w19, w0
	ldr	w0, [x22, 128]
	beq	.L4228
	cmp	w0, 1
	beq	.L4229
.L4223:
	str	xzr, [x21, 616]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4229:
	ldr	w0, [x22, 40]
	str	wzr, [x22, 132]
	sub	w0, w0, #1
	str	w0, [x22, 40]
	ldr	x1, [x21, 632]
	cbz	x1, .L4223
	ldr	w0, [x22, 116]
	bl	ReleaseNAL
	str	xzr, [x21, 632]
	b	.L4223
	.p2align 3
.L4228:
	cmp	w0, 1
	beq	.L4220
	mov	x0, x21
	bl	DecVDM
	ldr	x23, [sp, 48]
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 64
	ret
	.p2align 3
.L4220:
	ldr	w1, [x22, 40]
	add	x2, x21, 61440
	str	w0, [x22, 132]
	sub	w1, w1, #1
	str	w1, [x22, 40]
	strb	w0, [x23, 817]
	str	w0, [x2, 2784]
	mov	w0, 0
	str	xzr, [x21, 616]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	ret
	.size	H264DEC_VDMProcSedOnly, .-H264DEC_VDMProcSedOnly
	.align	3
.LC971:
	.xword	s_pstVfmwChan
	.align	3
.LC972:
	.xword	g_BackUp
	.align	3
.LC974:
	.xword	.LC973
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
	cbz	w0, .L4247
	sub	w0, w0, #1
	mov	w4, 808
	mov	x2, 30040
	add	x1, x21, 28672
	add	x1, x1, 560
	umaddl	x4, w0, w4, x2
	add	x4, x21, x4
	.p2align 2
.L4238:
	ldrb	w0, [x1, 1]
	cbnz	w0, .L4233
	cbz	x1, .L4234
	add	x23, x21, 24576
	ldr	w3, [x23, 3716]
	cbz	w3, .L4234
	ldr	x0, [x21, 27904]
	cmp	x0, x1
	beq	.L4233
	sub	w3, w3, #1
	mov	x0, x21
	add	x3, x21, x3, lsl 3
	b	.L4235
	.p2align 3
.L4236:
	ldr	x2, [x0, 27904]
	cmp	x2, x1
	beq	.L4233
.L4235:
	cmp	x3, x0
	add	x0, x0, 8
	bne	.L4236
.L4234:
	add	x19, x21, 65536
	ldr	w0, [x19, 116]
	bl	FSP_IsNewFsAvalible
	cmp	w0, 1
	beq	.L4247
	cmn	w0, #1
	beq	.L4268
.L4266:
	mov	w0, 0
.L4270:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4233:
	add	x1, x1, 808
	cmp	x1, x4
	bne	.L4238
	add	x2, x29, 76
	add	x1, x29, 72
	add	x0, x21, 8
	bl	GetQueueImgNum
	ldr	x1, .LC976
	mov	w0, 0
	bl	dprint_vfmw
	ldr	x1, .LC978
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 2772]
	cbz	w0, .L4269
	ldr	x20, .LC982
	mov	w22, 0
	add	x23, x21, 24576
	.p2align 2
.L4245:
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
	beq	.L4250
	ldr	w5, [x23, 3716]
	cbz	w5, .L4250
	ldr	x0, [x21, 27904]
	cmp	x1, x0
	beq	.L4252
	sub	w5, w5, #1
	mov	x0, x21
	add	x5, x21, x5, lsl 3
	b	.L4243
	.p2align 3
.L4244:
	ldr	x2, [x0, 27904]
	cmp	x1, x2
	beq	.L4252
.L4243:
	cmp	x5, x0
	add	x0, x0, 8
	bne	.L4244
.L4250:
	mov	w5, 1
.L4242:
	mov	w2, w22
	mov	x1, x20
	mov	w0, 0
	add	w22, w22, 1
	bl	dprint_vfmw
	ldr	w0, [x19, 2772]
	cmp	w0, w22
	bhi	.L4245
.L4246:
	ldp	w2, w3, [x29, 72]
	ldr	x1, .LC980
	mov	w0, 0
	bl	dprint_vfmw
	ldr	w0, [x19, 2772]
	ldr	w2, [x23, 3716]
	ldp	w3, w1, [x29, 72]
	sub	w0, w0, #2
	sub	w0, w0, w2
	add	w1, w3, w1
	cmp	w1, w0
	bge	.L4266
	ldr	x1, .LC984
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
.L4247:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	mov	w0, 1
	ldp	x29, x30, [sp], 80
	ret
	.p2align 3
.L4252:
	mov	w5, 0
	b	.L4242
.L4268:
	ldr	w0, [x19, 116]
	add	x1, x21, 8
	bl	FSP_ClearNotInVoQueue
	mov	w0, 0
	b	.L4270
.L4269:
	add	x23, x21, 24576
	b	.L4246
	.size	H264DEC_GetImageBuffer, .-H264DEC_GetImageBuffer
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
	.type	SarTable.12897, %object
	.size	SarTable.12897, 136
SarTable.12897:
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
	.type	__func__.13078, %object
	.size	__func__.13078, 12
__func__.13078:
	.string	"H264_DecSEI"
	.zero	4
	.type	__func__.11931, %object
	.size	__func__.11931, 13
__func__.11931:
	.string	"DirectOutput"
	.zero	3
	.type	__func__.12396, %object
	.size	__func__.12396, 8
__func__.12396:
	.string	"InitDPB"
	.type	__func__.12049, %object
	.size	__func__.12049, 14
__func__.12049:
	.string	"StorePicInDPB"
	.zero	2
	.type	__FUNCTION__.13317, %object
	.size	__FUNCTION__.13317, 13
__FUNCTION__.13317:
	.string	"H264DEC_Init"
	.zero	3
	.type	__FUNCTION__.12533, %object
	.size	__FUNCTION__.12533, 8
__FUNCTION__.12533:
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
.LC973:
	ASCII(.string	"CtbNumOneFrame=%d, SedEnd0=%d\n" )
	.zero	1
.LC975:
	ASCII(.string	"cann't find FrameStore\n" )
.LC977:
	ASCII(.string	"========== H264 FrameStore state(is_used, is_in_dpb, IsOutDPB) ========\n" )
	.zero	7
.LC979:
	ASCII(.string	"ReadImgNum = %d, NewImgNum = %d\n" )
	.zero	7
.LC981:
	ASCII(.string	"%02d: %d %d %d\n" )
.LC983:
	ASCII(.string	"FrameStore leak, ClearAll\n" )
	.bss
	.align	3
.LANCHOR2 = . + 0
.LANCHOR3 = . + 4352
	.type	pps_tmp.12817, %object
	.size	pps_tmp.12817, 2208
pps_tmp.12817:
	.zero	2208
	.type	sps_tmp.12966, %object
	.size	sps_tmp.12966, 4000
sps_tmp.12966:
	.zero	4000
	.type	TmpBuf, %object
	.size	TmpBuf, 68
TmpBuf:
	.zero	68
	.ident	"GCC: (gcc-linaro-5.1-2015.08 + glibc-2.22 (Build by czyong Wed Mar  9 18:57:48 CST 2016)) 5.1.1 20150608"
	.section	.note.GNU-stack,"",%progbits
