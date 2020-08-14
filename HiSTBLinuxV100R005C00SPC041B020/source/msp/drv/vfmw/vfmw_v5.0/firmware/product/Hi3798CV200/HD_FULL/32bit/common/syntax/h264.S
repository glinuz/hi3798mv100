#include "hi_asm_define.h"
	.arch armv7-a
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"h264.c"
	.text
	.align	2
	.global	IsSTRefFlg
	.type	IsSTRefFlg, %function
IsSTRefFlg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r0, [r0]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #65536
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	IsSTRefFlg, .-IsSTRefFlg
	.align	2
	.global	IsLTRefFlg
	.type	IsLTRefFlg, %function
IsLTRefFlg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r0, [r0]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #256
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	IsLTRefFlg, .-IsLTRefFlg
	.align	2
	.global	compare_pic_by_pic_num_desc
	.type	compare_pic_by_pic_num_desc, %function
compare_pic_by_pic_num_desc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #8]
	ldr	r3, [r3, #8]
	cmp	r2, r3
	blt	.L5
	mvngt	r0, #0
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L5:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_pic_by_pic_num_desc, .-compare_pic_by_pic_num_desc
	.align	2
	.global	compare_pic_by_lt_pic_num_asc
	.type	compare_pic_by_lt_pic_num_asc, %function
compare_pic_by_lt_pic_num_asc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #4]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	blt	.L9
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L9:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_pic_by_lt_pic_num_asc, .-compare_pic_by_lt_pic_num_asc
	.align	2
	.global	compare_fs_by_frame_num_desc
	.type	compare_fs_by_frame_num_desc, %function
compare_fs_by_frame_num_desc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #640]
	ldr	r3, [r3, #640]
	cmp	r2, r3
	blt	.L12
	mvngt	r0, #0
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L12:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_fs_by_frame_num_desc, .-compare_fs_by_frame_num_desc
	.align	2
	.global	compare_fs_by_lt_pic_idx_asc
	.type	compare_fs_by_lt_pic_idx_asc, %function
compare_fs_by_lt_pic_idx_asc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #644]
	ldr	r3, [r3, #644]
	cmp	r2, r3
	bcc	.L15
	movhi	r0, #1
	movls	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L15:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_fs_by_lt_pic_idx_asc, .-compare_fs_by_lt_pic_idx_asc
	.align	2
	.global	compare_pic_by_poc_asc
	.type	compare_pic_by_poc_asc, %function
compare_pic_by_poc_asc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #12]
	ldr	r3, [r3, #12]
	cmp	r2, r3
	blt	.L18
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L18:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_pic_by_poc_asc, .-compare_pic_by_poc_asc
	.align	2
	.global	compare_pic_by_poc_desc
	.type	compare_pic_by_poc_desc, %function
compare_pic_by_poc_desc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #12]
	ldr	r3, [r3, #12]
	cmp	r2, r3
	blt	.L21
	mvngt	r0, #0
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L21:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_pic_by_poc_desc, .-compare_pic_by_poc_desc
	.align	2
	.global	compare_fs_by_poc_asc
	.type	compare_fs_by_poc_asc, %function
compare_fs_by_poc_asc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #648]
	ldr	r3, [r3, #648]
	cmp	r2, r3
	blt	.L24
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L24:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_fs_by_poc_asc, .-compare_fs_by_poc_asc
	.align	2
	.global	compare_fs_by_poc_desc
	.type	compare_fs_by_poc_desc, %function
compare_fs_by_poc_desc:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0]
	ldr	r3, [r1]
	ldr	r2, [r2, #648]
	ldr	r3, [r3, #648]
	cmp	r2, r3
	blt	.L27
	mvngt	r0, #0
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L27:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	compare_fs_by_poc_desc, .-compare_fs_by_poc_desc
	.align	2
	.type	SetFrmRepeatCount.part.1, %function
SetFrmRepeatCount.part.1:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	ip, r0, #61440
	add	r0, r0, #24576
	ldr	lr, [ip, #1156]
	ldr	r2, [r0, #2144]
	ldr	r3, [ip, #1152]
	add	r2, r2, lr, lsr #1
	add	r0, r2, #1
	mov	r3, r3, lsr #1
	cmp	r3, r0
	movcc	r3, #0
	strcc	r3, [r1, #24]
	ldmccfd	sp, {fp, sp, pc}
	sub	r3, r3, #1
	rsb	r3, r2, r3
	cmp	r3, #1
	movhi	r3, #2
	movls	r3, #1
	strhi	r3, [r1, #24]
	strls	r3, [r1, #24]
	ldrhi	r3, [ip, #1156]
	ldrls	r3, [ip, #1156]
	addhi	r3, r3, #4
	addls	r3, r3, #2
	str	r3, [ip, #1156]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	SetFrmRepeatCount.part.1, .-SetFrmRepeatCount.part.1
	.align	2
	.type	DumpList.part.5, %function
DumpList.part.5:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r8, r0, #26880
	add	r8, r8, #248
	add	r10, r0, #20480
	add	r10, r10, #248
	mov	r7, r0
	mov	r6, #0
	mov	r9, r8
.L33:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	movne	r5, r9
	movne	r4, #0
	beq	.L36
.L34:
	ldr	ip, [r5, #4]!
	mov	r3, r4
	mov	r2, r6
	ldr	r1, .L64
	mov	r0, #13
	add	r4, r4, #1
	ldr	ip, [ip, #32]
	ldr	lr, [ip, #648]
	str	lr, [sp, #4]
	ldr	ip, [ip, #636]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r10]
	cmp	r4, r3
	bcc	.L34
.L36:
	add	r6, r6, #1
	add	r9, r9, #132
	cmp	r6, #2
	bne	.L33
	add	r5, r7, #20480
	ldr	r3, [r5, #252]
	cmp	r3, #0
	bne	.L60
.L37:
	ldr	r3, [r5, #256]
	cmp	r3, #0
	bne	.L61
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L61:
	ldr	r1, .L64+4
	mov	r0, #13
	bl	dprint_vfmw
	ldr	r3, [r5, #256]
	cmp	r3, #0
	addne	r7, r7, #27136
	movne	r4, #0
	addne	r7, r7, #124
	bne	.L50
	b	.L51
.L62:
	bcc	.L48
	cmp	r2, #2
	ldr	r1, .L64+8
	mov	r0, #13
	bne	.L46
	ldr	r3, [r3, #32]
	ldr	r2, [r3, #648]
	bl	dprint_vfmw
.L46:
	ldr	r3, [r5, #256]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L51
.L50:
	ldr	r3, [r7, #4]!
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L62
	ldr	r3, [r3, #32]
	mov	r0, #13
	ldr	r1, .L64+12
	add	r4, r4, #1
	ldr	r2, [r3, #648]
	bl	dprint_vfmw
	ldr	r3, [r5, #256]
	cmp	r4, r3
	bcc	.L50
.L51:
	ldr	r1, .L64+16
	mov	r0, #13
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L48:
	ldr	r3, [r3, #32]
	mov	r0, #13
	ldr	r1, .L64+20
	ldr	r2, [r3, #648]
	bl	dprint_vfmw
	b	.L46
.L60:
	ldr	r1, .L64+24
	mov	r0, #13
	bl	dprint_vfmw
	ldr	r3, [r5, #252]
	cmp	r3, #0
	movne	r4, #0
	bne	.L38
	b	.L43
.L63:
	bcc	.L41
	cmp	r2, #2
	ldr	r1, .L64+8
	mov	r0, #13
	bne	.L39
	ldr	r3, [r3, #32]
	ldr	r2, [r3, #648]
	bl	dprint_vfmw
.L39:
	ldr	r3, [r5, #252]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L43
.L38:
	ldr	r3, [r8, #4]!
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L63
	ldr	r3, [r3, #32]
	mov	r0, #13
	ldr	r1, .L64+12
	add	r4, r4, #1
	ldr	r2, [r3, #648]
	bl	dprint_vfmw
	ldr	r3, [r5, #252]
	cmp	r4, r3
	bcc	.L38
.L43:
	ldr	r1, .L64+16
	mov	r0, #13
	bl	dprint_vfmw
	b	.L37
.L41:
	ldr	r3, [r3, #32]
	mov	r0, #13
	ldr	r1, .L64+20
	ldr	r2, [r3, #648]
	bl	dprint_vfmw
	b	.L39
.L65:
	.align	2
.L64:
	.word	.LC0
	.word	.LC6
	.word	.LC5
	.word	.LC4
	.word	.LC2
	.word	.LC3
	.word	.LC1
	UNWIND(.fnend)
	.size	DumpList.part.5, .-DumpList.part.5
	.align	2
	.type	CombinePacket.part.10, %function
CombinePacket.part.10:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r1, .L67
	mov	r4, r0
	mov	r0, #7
	bl	dprint_vfmw
	ldr	ip, [r4, #368]
	ldr	r1, .L67+4
	mov	r0, #7
	mov	r5, #0
	ldr	lr, [ip, #40]
	ldr	r3, [ip, #44]
	ldr	r2, [ip, #16]
	str	lr, [sp, #4]
	ldr	ip, [ip, #12]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #368]
	ldr	ip, .L67+8
	ldr	r2, [r3, #12]
	ldr	r0, [r3, #36]
	ldr	r1, [r3, #8]
	rsb	r0, r2, r0
	ldr	r3, [ip, #52]
	blx	r3
	ldr	r3, [r4, #368]
	mov	ip, #1
	ldr	r1, .L67+12
	mov	r0, #7
	ldr	r2, [r3, #60]
	ldr	r6, [r3, #32]
	str	r2, [r3, #32]
	ldr	r3, [r4, #368]
	str	r5, [r3, #24]
	ldr	r3, [r4, #368]
	ldr	lr, [r3, #12]
	ldr	r2, [r3, #36]
	rsb	r2, lr, r2
	str	r2, [r3, #8]
	ldr	r3, [r4, #368]
	ldr	lr, [r3, #12]
	ldr	r2, [r3, #44]
	rsb	r2, lr, r2
	str	r2, [r3, #16]
	ldr	r3, [r4, #368]
	ldr	lr, [r3, #40]
	ldr	r2, [r3, #12]
	add	r2, r2, lr
	str	r2, [r3, #12]
	ldr	r3, [r4, #368]
	str	ip, [r3, #68]
	ldr	r2, [r4, #368]
	ldr	r3, [r2, #12]
	ldr	r2, [r2, #16]
	bl	dprint_vfmw
	add	r3, r4, #61440
	mov	r1, r6
	ldr	r0, [r3, #1136]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #368]
	str	r5, [r3, #36]
	ldr	r3, [r4, #368]
	str	r5, [r3, #52]
	ldr	r3, [r4, #368]
	str	r5, [r3, #40]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L68:
	.align	2
.L67:
	.word	.LC7
	.word	.LC8
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC9
	UNWIND(.fnend)
	.size	CombinePacket.part.10, .-CombinePacket.part.10
	.align	2
	.global	ue_v
	.type	ue_v, %function
ue_v:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r7, r0, #13504
	mov	r8, r0
	add	r7, r7, #24
	mov	r6, r1
	mov	r1, #32
	mov	r0, r7
	bl	BsShow
	mov	r5, r0
	bl	ZerosMS_32
	cmp	r0, #15
	mov	r4, r0
	bls	.L73
	cmp	r0, #31
	bls	.L74
	add	r3, r8, #61440
	mvn	r5, #0
	mov	r7, #32
	mov	r2, #1
	str	r2, [r3, #1076]
.L71:
	ldr	ip, [r8, #368]
	mov	r2, r6
	mov	r3, r5
	ldr	r1, .L75
	mov	r0, #21
	ldr	lr, [ip, #64]
	add	r4, lr, r7
	str	r4, [ip, #64]
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L73:
	mov	r4, r0, asl #1
	mov	r0, r7
	add	r7, r4, #1
	rsb	r4, r4, #31
	mov	r4, r5, lsr r4
	mov	r1, r7
	sub	r5, r4, #1
	bl	BsSkip
	b	.L71
.L74:
	add	r5, r0, #1
	mov	r1, r0
	mov	r0, r7
	bl	BsSkip
	mov	r1, r5
	mov	r0, r7
	bl	BsShow
	mov	r1, r5
	sub	r5, r0, #1
	mov	r0, r7
	bl	BsSkip
	mov	ip, r4, asl #1
	add	r7, ip, #1
	b	.L71
.L76:
	.align	2
.L75:
	.word	.LC10
	UNWIND(.fnend)
	.size	ue_v, .-ue_v
	.align	2
	.global	se_v
	.type	se_v, %function
se_v:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #13504
	mov	r8, r0
	add	r6, r6, #24
	mov	r7, r1
	mov	r1, #32
	mov	r0, r6
	bl	BsShow
	mov	r4, r0
	bl	ZerosMS_32
	cmp	r0, #15
	mov	r5, r0
	bhi	.L78
	mov	r5, r0, asl #1
	mov	r0, r6
	rsb	r3, r5, #31
	add	r5, r5, #1
	mov	r3, r4, lsr r3
	and	r2, r3, #1
	mov	r1, r5
	rsb	r4, r2, #0
	eor	r4, r4, r3, lsr #1
	add	r4, r4, r2
	bl	BsSkip
.L79:
	ldr	lr, [r8, #368]
	mov	r2, r7
	mov	r3, r4
	ldr	r1, .L82
	mov	r0, #21
	ldr	ip, [lr, #64]
	add	r5, ip, r5
	str	r5, [lr, #64]
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L78:
	cmp	r0, #31
	bls	.L81
	add	r3, r8, #61440
	mov	r2, #1
	mvn	r4, #-2147483648
	mov	r5, #32
	str	r2, [r3, #1076]
	b	.L79
.L81:
	mov	r1, r0
	mov	r0, r6
	bl	BsSkip
	mov	r1, r5
	mov	r0, r6
	mov	r5, r5, asl #1
	bl	BsGet
	mov	r1, #1
	add	r5, r5, #1
	mov	r9, r0
	mov	r0, r6
	bl	BsGet
	and	r0, r0, #1
	rsb	r4, r0, #0
	eor	r9, r9, r4
	add	r4, r9, r0
	b	.L79
.L83:
	.align	2
.L82:
	.word	.LC10
	UNWIND(.fnend)
	.size	se_v, .-se_v
	.align	2
	.global	u_v
	.type	u_v, %function
u_v:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r5, r0
	add	r0, r0, #13504
	add	r0, r0, #24
	mov	r6, r1
	mov	r7, r2
	bl	BsGet
	ldr	lr, [r5, #368]
	mov	r2, r7
	ldr	r1, .L85
	ldr	ip, [lr, #64]
	add	ip, ip, r6
	str	ip, [lr, #64]
	mov	r4, r0
	mov	r0, #21
	mov	r3, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L86:
	.align	2
.L85:
	.word	.LC10
	UNWIND(.fnend)
	.size	u_v, .-u_v
	.align	2
	.global	u_1
	.type	u_1, %function
u_1:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r5, r0
	add	r0, r0, #13504
	mov	r6, r1
	add	r0, r0, #24
	mov	r1, #1
	bl	BsGet
	ldr	lr, [r5, #368]
	mov	r2, r6
	ldr	r1, .L88
	ldr	ip, [lr, #64]
	add	ip, ip, #1
	str	ip, [lr, #64]
	mov	r4, r0
	mov	r3, r0
	mov	r0, #21
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L89:
	.align	2
.L88:
	.word	.LC10
	UNWIND(.fnend)
	.size	u_1, .-u_1
	.align	2
	.global	H264_IsEosNal
	.type	H264_IsEosNal, %function
H264_IsEosNal:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #13504
	mov	r1, #32
	add	r4, r4, #24
	mov	r0, r4
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r4
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r5, r3
	moveq	r0, #1
	movne	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	H264_IsEosNal, .-H264_IsEosNal
	.align	2
	.global	GetMinPOC
	.type	GetMinPOC, %function
GetMinPOC:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #24576
	mov	ip, #0
	mvn	r3, #-2147483648
	str	ip, [r2]
	str	r3, [r1]
	ldr	r3, [r5, #2148]
	ldr	ip, [r5, #2144]
	cmp	r3, ip
	strhi	ip, [r5, #2148]
	movhi	r3, ip
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r0, r0, #26368
	mov	r3, #0
	add	r0, r0, #156
.L95:
	ldr	ip, [r0, #4]!
	ldr	r4, [r1]
	ldr	lr, [ip, #648]
	cmp	r4, lr
	ble	.L94
	ldrb	ip, [ip, #4]	@ zero_extendqisi2
	cmp	ip, #0
	strne	lr, [r1]
	strne	r3, [r2]
.L94:
	ldr	ip, [r5, #2148]
	add	r3, r3, #1
	cmp	ip, r3
	bhi	.L95
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	GetMinPOC, .-GetMinPOC
	.align	2
	.global	GetMaxPOC
	.type	GetMaxPOC, %function
GetMaxPOC:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #24576
	mov	r3, #0
	str	r3, [r2]
	str	r3, [r1]
	ldr	r3, [r5, #2148]
	ldr	ip, [r5, #2144]
	cmp	r3, ip
	strhi	ip, [r5, #2148]
	movhi	r3, ip
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r0, r0, #26368
	mov	r3, #0
	add	r0, r0, #156
.L105:
	ldr	ip, [r0, #4]!
	ldr	r4, [r1]
	ldr	lr, [ip, #648]
	cmp	r4, lr
	bge	.L104
	ldrb	ip, [ip, #4]	@ zero_extendqisi2
	cmp	ip, #0
	strne	lr, [r1]
	strne	r3, [r2]
.L104:
	ldr	ip, [r5, #2148]
	add	r3, r3, #1
	cmp	ip, r3
	bhi	.L105
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	GetMaxPOC, .-GetMaxPOC
	.align	2
	.global	FrameStoreRefFlg
	.type	FrameStoreRefFlg, %function
FrameStoreRefFlg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r0, [r0, #2]	@ zero_extendqisi2
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	FrameStoreRefFlg, .-FrameStoreRefFlg
	.align	2
	.global	ShortTermRefFlg
	.type	ShortTermRefFlg, %function
ShortTermRefFlg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r0, #2]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L127
	tst	r3, #1
	bne	.L128
.L116:
	and	r3, r3, #2
	ands	r3, r3, #255
	beq	.L120
.L117:
	ldr	r0, [r0, #748]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #65536
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L120:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L128:
	ldr	r2, [r0, #712]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #65536
	bne	.L116
.L121:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L127:
	ldr	r3, [r0, #676]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L121
	ldr	r3, [r0, #712]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	bne	.L117
	b	.L121
	UNWIND(.fnend)
	.size	ShortTermRefFlg, .-ShortTermRefFlg
	.align	2
	.global	LongTermRefFlg
	.type	LongTermRefFlg, %function
LongTermRefFlg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r0, #2]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L144
	tst	r3, #1
	bne	.L145
.L133:
	and	r3, r3, #2
	ands	r3, r3, #255
	beq	.L137
.L134:
	ldr	r0, [r0, #748]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #256
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L137:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L145:
	ldr	r2, [r0, #712]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #256
	bne	.L133
.L138:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L144:
	ldr	r3, [r0, #676]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	beq	.L138
	ldr	r3, [r0, #712]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	bne	.L134
	b	.L138
	UNWIND(.fnend)
	.size	LongTermRefFlg, .-LongTermRefFlg
	.align	2
	.global	UpdateLTReflist
	.type	UpdateLTReflist, %function
UpdateLTReflist:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r9, r0, #24576
	mov	r6, r0
	ldr	r8, [r9, #2148]
	cmp	r8, #0
	beq	.L153
	add	r10, r0, #26368
	mov	r4, #0
	add	r10, r10, #156
	mov	r5, r4
.L149:
	ldr	r7, [r10, #4]!
	add	r5, r5, #1
	mov	r0, r7
	bl	LongTermRefFlg
	add	r3, r4, #6656
	add	r3, r3, #8
	cmp	r0, #0
	strne	r7, [r6, r3, asl #2]
	addne	r4, r4, #1
	cmp	r5, r8
	bne	.L149
.L147:
	ldr	r3, [r9, #2144]
	str	r4, [r9, #2156]
	cmp	r3, r4
	ldmlsfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	add	r0, r4, #6656
	mov	r2, #0
	add	r0, r0, #7
	add	r0, r6, r0, lsl #2
.L151:
	add	r4, r4, #1
	str	r2, [r0, #4]!
	cmp	r4, r3
	bne	.L151
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L153:
	mov	r4, r8
	b	.L147
	UNWIND(.fnend)
	.size	UpdateLTReflist, .-UpdateLTReflist
	.align	2
	.global	UpdateReflist
	.type	UpdateReflist, %function
UpdateReflist:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r9, r0, #24576
	mov	r6, r0
	ldr	r8, [r9, #2148]
	cmp	r8, #0
	beq	.L166
	add	r10, r0, #26368
	mov	r4, #0
	add	r10, r10, #156
	mov	r5, r4
.L162:
	ldr	r7, [r10, #4]!
	add	r5, r5, #1
	mov	r0, r7
	bl	ShortTermRefFlg
	add	r3, r4, #6592
	add	r3, r3, #56
	cmp	r0, #0
	strne	r7, [r6, r3, asl #2]
	addne	r4, r4, #1
	cmp	r5, r8
	bne	.L162
.L160:
	ldr	r3, [r9, #2144]
	str	r4, [r9, #2152]
	cmp	r3, r4
	ldmlsfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	add	r0, r4, #6592
	mov	r2, #0
	add	r0, r0, #55
	add	r0, r6, r0, lsl #2
.L164:
	add	r4, r4, #1
	str	r2, [r0, #4]!
	cmp	r4, r3
	bne	.L164
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L166:
	mov	r4, r8
	b	.L160
	UNWIND(.fnend)
	.size	UpdateReflist, .-UpdateReflist
	.align	2
	.global	GetPicNumX
	.type	GetPicNumX, %function
GetPicNumX:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r0, #3]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r0, #648]
	mvn	r0, r1
	movne	r3, r3, asl #1
	addne	r3, r3, #1
	add	r0, r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	GetPicNumX, .-GetPicNumX
	.align	2
	.global	UnMarkFrameStoreRef
	.type	UnMarkFrameStoreRef, %function
UnMarkFrameStoreRef:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L176
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	mov	ip, #0
	strb	ip, [r1, #713]
	and	r2, r2, #2
	strb	ip, [r1, #714]
	strb	r2, [r1, #2]
.L176:
	tst	r3, #2
	beq	.L177
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	mov	ip, #0
	strb	ip, [r1, #749]
	and	r2, r2, #1
	strb	ip, [r1, #750]
	strb	r2, [r1, #2]
.L177:
	cmp	r3, #3
	add	r0, r0, #61440
	moveq	r3, #0
	streqb	r3, [r1, #678]
	streqb	r3, [r1, #677]
	mov	r3, #0
	mov	r2, r3
	strb	r3, [r1, #2]
	ldr	r1, [r1, #632]
	ldr	r0, [r0, #1136]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	FSP_SetRef
	UNWIND(.fnend)
	.size	UnMarkFrameStoreRef, .-UnMarkFrameStoreRef
	.align	2
	.type	SlidingWinMark.part.2, %function
SlidingWinMark.part.2:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #24576
	mov	r8, r0
	ldr	r7, [r3, #2148]
	cmp	r7, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r6, r0, #26368
	mov	r4, #0
	add	r6, r6, #156
.L188:
	ldr	r5, [r6, #4]!
	add	r4, r4, #1
	ldrb	r3, [r5, #2]	@ zero_extendqisi2
	mov	r0, r5
	cmp	r3, #0
	beq	.L187
	bl	ShortTermRefFlg
	cmp	r0, #0
	bne	.L199
.L187:
	cmp	r4, r7
	bne	.L188
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L199:
	mov	r0, r8
	mov	r1, r5
	bl	UnMarkFrameStoreRef
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	UpdateReflist
	UNWIND(.fnend)
	.size	SlidingWinMark.part.2, .-SlidingWinMark.part.2
	.align	2
	.global	UnMarkLTFrmByFrmIdx
	.type	UnMarkLTFrmByFrmIdx, %function
UnMarkLTFrmByFrmIdx:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #24576
	ldr	r4, [r3, #2156]
	cmp	r4, #0
	ldmeqfd	sp, {r4, fp, sp, pc}
	ldr	ip, [r3, #2080]
	ldr	r3, [ip, #644]
	cmp	r3, r1
	addne	r2, r0, #26624
	movne	r3, #0
	addne	r2, r2, #32
	bne	.L204
	b	.L202
.L205:
	ldr	ip, [r2, #4]!
	ldr	lr, [ip, #644]
	cmp	lr, r1
	beq	.L202
.L204:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L205
	ldmfd	sp, {r4, fp, sp, pc}
.L202:
	mov	r1, ip
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	b	UnMarkFrameStoreRef
	UNWIND(.fnend)
	.size	UnMarkLTFrmByFrmIdx, .-UnMarkLTFrmByFrmIdx
	.align	2
	.global	UnMarkLTFldByFrmIdx
	.type	UnMarkLTFldByFrmIdx, %function
UnMarkLTFldByFrmIdx:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	ip, r0, #24576
	ldr	r6, [ip, #2156]
	cmp	r6, #0
	ldmeqfd	sp, {r4, r5, r6, fp, sp, pc}
	add	r4, r0, #26624
	mov	ip, #0
	add	r4, r4, #28
	b	.L215
.L213:
	cmp	ip, r6
	beq	.L229
.L215:
	ldr	lr, [r4, #4]!
	add	ip, ip, #1
	ldr	r5, [lr, #644]
	cmp	r5, r3
	bne	.L213
	cmp	r2, #1
	ldreqb	r5, [lr, #749]	@ zero_extendqisi2
	beq	.L228
	cmp	r2, #2
	bne	.L213
	ldrb	r5, [lr, #713]	@ zero_extendqisi2
.L228:
	clz	r5, r5
	mov	r5, r5, lsr #5
	cmp	lr, r1
	orrne	r5, r5, #1
	cmp	r5, #0
	beq	.L213
	mov	r1, lr
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	UnMarkFrameStoreRef
.L229:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	UnMarkLTFldByFrmIdx, .-UnMarkLTFldByFrmIdx
	.align	2
	.global	UnMarkSTRef
	.type	UnMarkSTRef, %function
UnMarkSTRef:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L231
	add	ip, r0, #24576
	ldr	r1, [r1, #648]
	ldr	r4, [ip, #2152]
	sub	r1, r1, #1
	rsb	r2, r2, r1
	cmp	r4, #0
	beq	.L260
	add	ip, r0, #26368
	add	ip, ip, #220
	b	.L237
.L236:
	cmp	r3, r4
	beq	.L261
.L237:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #684]
	cmp	lr, r2
	bne	.L236
	ldrb	lr, [r1, #2]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L236
	ldr	lr, [r1, #676]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L236
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	UnMarkFrameStoreRef
.L231:
	add	r3, r0, #24576
	ldr	r1, [r1, #648]
	ldr	r5, [r3, #2152]
	rsb	r2, r2, r1, lsl #1
	cmp	r5, #0
	beq	.L262
	add	r4, r0, #26368
	mov	ip, #0
	add	r4, r4, #220
	b	.L242
.L238:
	tst	r3, #2
	beq	.L240
	ldr	r3, [r1, #748]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L263
.L240:
	add	ip, ip, #1
	cmp	ip, r5
	beq	.L264
.L242:
	ldr	r1, [r4, #4]!
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	ands	r6, r3, #1
	beq	.L238
	ldr	lr, [r1, #712]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L238
	ldr	lr, [r1, #720]
	cmp	lr, r2
	bne	.L238
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #24576
	strb	r3, [r1, #2]
	mov	r1, #0
	ldr	r3, [r2, #2016]
	strb	r1, [r3, #714]
	ldr	r3, [r2, #2016]
	ldrb	ip, [r3, #676]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #678]
	ldreq	r3, [r2, #2016]
	streqb	r1, [r3, #677]
	ldreq	r3, [r2, #2016]
	ldrb	r1, [r3, #2]	@ zero_extendqisi2
	cmp	r1, #0
	ldmnefd	sp, {r4, r5, r6, fp, sp, pc}
	ldr	ip, [r3, #668]
	mov	r2, r1
	add	lr, r0, #61440
	add	ip, ip, #15232
	add	ip, ip, #42
	str	r1, [r0, ip, asl #2]
	ldr	r1, [r3, #632]
.L257:
	ldr	r0, [lr, #1136]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	FSP_SetRef
.L263:
	ldr	r3, [r1, #756]
	cmp	r3, r2
	bne	.L240
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #2]
	add	r3, r3, #24576
	mov	r2, #0
	ldr	r1, [r3, #2016]
	strb	r2, [r1, #750]
	ldr	r1, [r3, #2016]
	ldrb	ip, [r1, #676]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r2
	streqb	r2, [r1, #678]
	ldreq	r1, [r3, #2016]
	streqb	r2, [r1, #677]
	ldreq	r1, [r3, #2016]
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, fp, sp, pc}
	ldr	ip, [r1, #668]
	add	lr, r0, #61440
	mov	r2, r3
	add	ip, ip, #15232
	add	ip, ip, #42
	str	r3, [r0, ip, asl #2]
	ldr	r1, [r1, #632]
	b	.L257
.L264:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L261:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L260:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L262:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	UnMarkSTRef, .-UnMarkSTRef
	.align	2
	.global	UnMarkLTRef
	.type	UnMarkLTRef, %function
UnMarkLTRef:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L266
	add	r3, r0, #24576
	ldr	r5, [r3, #2156]
	cmp	r5, #0
	beq	.L294
	add	r4, r0, #26624
	mov	ip, #0
	add	r4, r4, #28
	b	.L275
.L271:
	tst	r3, #2
	beq	.L273
	ldr	r3, [r1, #748]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	beq	.L295
.L273:
	add	ip, ip, #1
	cmp	ip, r5
	beq	.L296
.L275:
	ldr	r1, [r4, #4]!
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	ands	r6, r3, #1
	beq	.L271
	ldr	lr, [r1, #712]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L271
	ldr	lr, [r1, #716]
	cmp	r2, lr
	bne	.L271
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #24576
	strb	r3, [r1, #2]
	mov	r1, #0
	ldr	r3, [r2, #2080]
	strb	r1, [r3, #713]
	ldr	r3, [r2, #2080]
	ldrb	ip, [r3, #676]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #678]
	ldreq	r3, [r2, #2080]
	streqb	r1, [r3, #677]
	ldreq	r3, [r2, #2080]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	cmp	r2, #0
	ldreq	r3, [r3, #668]
	addeq	r3, r3, #15232
	addeq	r3, r3, #42
	streq	r2, [r0, r3, asl #2]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L266:
	add	r1, r0, #24576
	ldr	r4, [r1, #2156]
	cmp	r4, #0
	beq	.L297
	add	ip, r0, #26624
	add	ip, ip, #28
	b	.L277
.L276:
	cmp	r3, r4
	beq	.L298
.L277:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #680]
	cmp	lr, r2
	bne	.L276
	ldrb	lr, [r1, #2]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L276
	ldr	lr, [r1, #676]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L276
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	UnMarkFrameStoreRef
.L295:
	ldr	r3, [r1, #752]
	cmp	r2, r3
	bne	.L273
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #2]
	add	r3, r3, #24576
	mov	r1, #0
	ldr	r2, [r3, #2080]
	strb	r1, [r2, #749]
	ldr	r2, [r3, #2080]
	ldrb	ip, [r2, #676]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r2, #678]
	ldreq	r2, [r3, #2080]
	streqb	r1, [r2, #677]
	ldreq	r2, [r3, #2080]
	ldrb	r1, [r2, #2]	@ zero_extendqisi2
	cmp	r1, #0
	ldreq	r3, [r2, #668]
	addeq	r3, r3, #15232
	addeq	r3, r3, #42
	streq	r1, [r0, r3, asl #2]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L298:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L296:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L297:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L294:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	UnMarkLTRef, .-UnMarkLTRef
	.align	2
	.global	MarkPicLTRef
	.type	MarkPicLTRef, %function
MarkPicLTRef:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r5, [r3, #3]	@ zero_extendqisi2
	add	r3, r0, #24576
	cmp	r5, #0
	beq	.L300
	ldr	r7, [r3, #2152]
	cmp	r7, #0
	beq	.L329
	add	r6, r0, #26368
	mov	ip, #0
	add	r6, r6, #220
	b	.L314
.L306:
	tst	r4, #2
	beq	.L309
	ldr	lr, [r3, #748]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	beq	.L330
.L309:
	add	ip, ip, #1
	cmp	ip, r7
	beq	.L331
.L314:
	ldr	r3, [r6, #4]!
	ldrb	r4, [r3, #2]	@ zero_extendqisi2
	tst	r4, #1
	beq	.L306
	ldr	lr, [r3, #712]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L306
	ldr	lr, [r3, #720]
	cmp	lr, r2
	bne	.L306
	ldrb	lr, [r3, #749]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L310
	ldr	lr, [r3, #644]
	cmp	lr, r1
	bne	.L309
.L310:
	str	r1, [r3, #644]
	sub	r3, r5, #1
	add	r2, r0, ip, lsl #2
	clz	r3, r3
	add	r2, r2, #24576
	mov	ip, #1
	mov	r3, r3, lsr #5
	mov	r0, #0
	add	r1, r3, r1, lsl #1
	ldr	r3, [r2, #2016]
	str	r1, [r3, #716]
	ldr	r3, [r2, #2016]
	strb	ip, [r3, #713]
	ldr	r3, [r2, #2016]
	strb	r0, [r3, #714]
	ldr	r3, [r2, #2016]
	ldrb	r1, [r3, #676]	@ zero_extendqisi2
	cmp	r1, #3
	cmpne	r1, r0
	bne	.L332
	ldrb	r1, [r3, #749]	@ zero_extendqisi2
	ldrb	r0, [r3, #713]	@ zero_extendqisi2
	cmp	r1, r0
	streqb	r1, [r3, #677]
	ldreq	r3, [r2, #2016]
	ldreqb	r2, [r3, #714]	@ zero_extendqisi2
	streqb	r2, [r3, #678]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L300:
	ldr	r6, [r3, #2152]
	cmp	r6, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	lr, r0, #26368
	mov	r3, r5
	add	lr, lr, #220
	b	.L305
.L304:
	add	r3, r3, #1
	cmp	r3, r6
	beq	.L333
.L305:
	ldr	ip, [lr, #4]!
	ldrb	r4, [ip, #2]	@ zero_extendqisi2
	cmp	r4, #3
	bne	.L304
	ldr	r4, [ip, #676]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	bne	.L304
	ldr	r4, [ip, #684]
	cmp	r4, r2
	bne	.L304
	add	r3, r0, r3, lsl #2
	str	r1, [ip, #644]
	add	r3, r3, #24576
	mov	r0, #1
	mov	r2, #0
	ldr	ip, [r3, #2016]
	str	r1, [ip, #680]
	ldr	r1, [r3, #2016]
	strb	r0, [r1, #677]
	ldr	r1, [r3, #2016]
	strb	r0, [r1, #713]
	ldr	r1, [r3, #2016]
	strb	r0, [r1, #749]
	ldr	r1, [r3, #2016]
	strb	r2, [r1, #678]
	ldr	r1, [r3, #2016]
	strb	r2, [r1, #714]
	ldr	r3, [r3, #2016]
	strb	r2, [r3, #750]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L330:
	ldr	lr, [r3, #756]
	cmp	lr, r2
	bne	.L309
	ldrb	lr, [r3, #713]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L313
	ldr	lr, [r3, #644]
	cmp	lr, r1
	bne	.L309
.L313:
	str	r1, [r3, #644]
	sub	r3, r5, #2
	add	r0, r0, ip, lsl #2
	clz	r3, r3
	add	r0, r0, #24576
	mov	ip, #1
	mov	r3, r3, lsr #5
	mov	r2, #0
	add	r1, r3, r1, lsl #1
	ldr	r3, [r0, #2016]
	str	r1, [r3, #752]
	ldr	r3, [r0, #2016]
	strb	ip, [r3, #749]
	ldr	r3, [r0, #2016]
	strb	r2, [r3, #750]
	ldr	r3, [r0, #2016]
	ldrb	r2, [r3, #676]	@ zero_extendqisi2
	cmp	r2, #3
	cmpne	r2, #0
	bne	.L334
	ldrb	r2, [r3, #749]	@ zero_extendqisi2
	ldrb	r1, [r3, #713]	@ zero_extendqisi2
	cmp	r2, r1
	streqb	r2, [r3, #677]
	ldreq	r3, [r0, #2016]
	ldreqb	r2, [r3, #750]	@ zero_extendqisi2
	streqb	r2, [r3, #678]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L331:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L333:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L332:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L334:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L329:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MarkPicLTRef, .-MarkPicLTRef
	.align	2
	.global	MarkSTToLTRef
	.type	MarkSTToLTRef, %function
MarkSTToLTRef:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r6, r1
	ldrb	r1, [r1, #3]	@ zero_extendqisi2
	mov	r8, r3
	mov	r7, r0
	cmp	r1, #0
	bne	.L336
	ldr	r5, [r6, #648]
	mov	r1, r3
	sub	r5, r5, #1
	rsb	r5, r2, r5
	bl	UnMarkLTFrmByFrmIdx
.L337:
	mov	r3, r6
	mov	r2, r5
	mov	r1, r8
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MarkPicLTRef
.L336:
	add	r3, r0, #24576
	ldr	r5, [r6, #648]
	ldr	r1, [r3, #2152]
	rsb	r5, r2, r5, lsl #1
	cmp	r1, #0
	beq	.L337
	add	r2, r0, #26368
	mov	lr, #0
	add	r2, r2, #220
	b	.L340
.L338:
	tst	r3, #2
	beq	.L339
	ldr	r3, [ip, #748]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L351
.L339:
	cmp	lr, r1
	beq	.L337
.L340:
	ldr	ip, [r2, #4]!
	add	lr, lr, #1
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L338
	ldr	r4, [ip, #712]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	bne	.L338
	ldr	r0, [ip, #720]
	cmp	r0, r5
	bne	.L338
	mov	r1, ip
	mov	r3, r8
	mov	r2, #1
	mov	r0, r7
	bl	UnMarkLTFldByFrmIdx
	b	.L337
.L351:
	ldr	r3, [ip, #756]
	cmp	r3, r5
	bne	.L339
	mov	r1, ip
	mov	r3, r8
	mov	r2, #2
	mov	r0, r7
	bl	UnMarkLTFldByFrmIdx
	b	.L337
	UNWIND(.fnend)
	.size	MarkSTToLTRef, .-MarkSTToLTRef
	.align	2
	.global	UpdateMaxLTFrmIdx
	.type	UpdateMaxLTFrmIdx, %function
UpdateMaxLTFrmIdx:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #24576
	mov	r7, r0
	ldr	r3, [r6, #2156]
	str	r1, [r6, #2160]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, r0, #26624
	mov	r3, r1
	add	r5, r5, #28
	mov	r4, #0
	b	.L356
.L354:
	ldr	r3, [r6, #2156]
	cmp	r3, r4
	ldmlsfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r6, #2160]
.L356:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	ldr	r2, [r1, #644]
	cmp	r2, r3
	bcc	.L354
	mov	r0, r7
	bl	UnMarkFrameStoreRef
	b	.L354
	UNWIND(.fnend)
	.size	UpdateMaxLTFrmIdx, .-UpdateMaxLTFrmIdx
	.align	2
	.global	UnMarkAllSTRef
	.type	UnMarkAllSTRef, %function
UnMarkAllSTRef:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r7, r0, #24576
	mov	r6, r0
	ldr	r3, [r7, #2152]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, r0, #26368
	mov	r4, #0
	add	r5, r5, #220
.L359:
	ldr	r1, [r5, #4]!
	mov	r0, r6
	bl	UnMarkFrameStoreRef
	ldr	r3, [r7, #2152]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L359
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	UnMarkAllSTRef, .-UnMarkAllSTRef
	.align	2
	.global	MarkCurrPicLT
	.type	MarkCurrPicLT, %function
MarkCurrPicLT:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r5, r2
	ldrb	r2, [r1, #3]	@ zero_extendqisi2
	mov	r4, r1
	cmp	r2, #0
	beq	.L365
	mov	r3, r5
	ldr	r1, [r1, #640]
	bl	UnMarkLTFldByFrmIdx
.L364:
	mov	r2, #1
	mov	r3, #0
	str	r5, [r4, #652]
	strb	r2, [r4, #4]
	strb	r3, [r4, #5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L365:
	mov	r1, r5
	bl	UnMarkLTFrmByFrmIdx
	b	.L364
	UNWIND(.fnend)
	.size	MarkCurrPicLT, .-MarkCurrPicLT
	.align	2
	.global	RemoveFrameStoreOutDPB
	.type	RemoveFrameStoreOutDPB, %function
RemoveFrameStoreOutDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, r1, lsl #2
	mov	r4, r1
	add	r5, r5, #24576
	mov	r6, r0
	ldr	r2, [r5, #1952]
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r2, #672]
	add	r7, r0, #24576
	mov	r1, #0
	add	r8, r0, #61440
	add	r3, r3, #6656
	add	r3, r3, #28
	add	r3, r0, r3, lsl #2
	str	r1, [r3, #4]
	ldr	r3, [r7, #2424]
	cmp	r3, r1
	subne	r3, r3, #1
	str	r3, [r7, #2424]
	ldr	r1, [r2, #632]
	ldr	r0, [r8, #1136]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L369
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L370
	ldr	r3, [r5, #1952]
	mov	r2, #0
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
.L370:
	ldr	r3, [r5, #1952]
	mov	r2, #0
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
.L369:
	ldr	r2, [r5, #1952]
	mov	r3, #0
	strb	r3, [r2, #1]
	ldr	r2, [r5, #1952]
	strb	r3, [r2, #4]
	ldr	r2, [r5, #1952]
	strb	r3, [r2, #2]
	ldr	r1, [r7, #2148]
	sub	r1, r1, #1
	cmp	r4, r1
	addcc	r3, r4, #6592
	addcc	r3, r3, #40
	addcc	r3, r6, r3, lsl #2
	bcs	.L373
.L372:
	add	r4, r4, #1
	ldr	r2, [r3, #4]!
	cmp	r4, r1
	str	r2, [r3, #-4]
	bne	.L372
.L373:
	add	r3, r1, #6592
	mov	r2, #0
	add	r3, r3, #40
	str	r2, [r6, r3, asl #2]
	str	r1, [r7, #2148]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	RemoveFrameStoreOutDPB, .-RemoveFrameStoreOutDPB
	.align	2
	.global	RemoveUnUsedFrameStore
	.type	RemoveUnUsedFrameStore, %function
RemoveUnUsedFrameStore:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r5, r0, #0
	beq	.L397
	add	r3, r5, #24576
	ldr	r6, [r3, #2148]
	cmp	r6, #0
	addne	r7, r5, #61440
	movne	r10, #0
	movne	r8, r10
	movne	r9, r10
	bne	.L395
	b	.L396
.L406:
	ldrb	r1, [r2, #4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L393
	ldr	r0, [r2, #784]
	cmp	r0, #1
	beq	.L393
	strb	r1, [r2, #1]
	mov	r0, r5
	mov	r1, r10
	add	r8, r8, #1
	bl	RemoveFrameStoreOutDPB
	cmp	r8, r6
	beq	.L396
.L395:
	add	r4, r5, r10, lsl #2
	ldr	r0, [r7, #1136]
	add	r4, r4, #24576
	ldr	r2, [r4, #1952]
	ldr	r1, [r2, #632]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L390
	ldrsh	r2, [r0]
	cmp	r2, #0
	bne	.L391
	ldr	r1, [r4, #1952]
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L392
	ldrsb	r0, [r0]
	cmp	r0, #0
	bne	.L405
.L392:
	mvn	r3, #0
	str	r3, [r1, #632]
	ldr	r2, [r4, #1952]
	strb	r9, [r2, #2]
.L390:
	ldr	r1, [r4, #1952]
	ldr	r2, [r1, #784]
	sub	r2, r2, #1
	clz	r2, r2
	mov	r2, r2, lsr #5
	strb	r2, [r1, #4]
.L391:
	ldr	r2, [r4, #1952]
	ldrb	r1, [r2, #2]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L406
.L393:
	add	r8, r8, #1
	add	r10, r10, #1
	cmp	r8, r6
	bne	.L395
.L396:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L405:
	ldr	r1, [r1, #632]
	ldr	r0, [r7, #1136]
	bl	FSP_SetRef
	ldr	r1, [r4, #1952]
	b	.L392
.L397:
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	UNWIND(.fnend)
	.size	RemoveUnUsedFrameStore, .-RemoveUnUsedFrameStore
	.align	2
	.global	CheckFrameStore
	.type	CheckFrameStore, %function
CheckFrameStore:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r3, r0, #57344
	mov	r5, r0
	mov	r4, r1
	ldr	r2, [r3, #3836]
	cmp	r2, #0
	addne	r2, r0, #61440
	bne	.L408
	ldr	r2, [r1, #784]
	cmp	r2, #1
	add	r2, r0, #61440
	beq	.L409
	ldr	r1, [r2, #1064]
	cmp	r1, #0
	beq	.L435
.L409:
	mov	r1, #1
	str	r1, [r3, #3836]
.L408:
	ldr	r0, [r2, #1136]
	ldr	r1, [r4, #632]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L411
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	cmpne	r3, #0
	beq	.L436
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L419
	cmp	r3, #1
	beq	.L419
	ldr	r3, [r4]
	mov	r1, #768
	movt	r1, 768
	bic	r3, r3, #16711680
	bic	r3, r3, #255
	cmp	r3, r1
	beq	.L437
	cmp	r2, #1
	beq	.L438
	cmp	r2, #2
	beq	.L439
.L417:
	ldr	r3, [r5]
	ldr	r2, [r0, #168]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	movls	r0, #0
	bhi	.L440
.L434:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L435:
	ldr	r1, [r0]
	ldr	r1, [r1, #12]
	cmp	r1, #0
	bne	.L409
	mvn	r0, #0
	b	.L434
.L436:
	ldr	r2, [r4, #632]
	mov	r0, #1
	ldr	r1, .L441
	bl	dprint_vfmw
.L413:
	ldr	r3, [r4, #784]
	cmp	r3, #1
	beq	.L420
	mvn	r0, #2
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L438:
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L417
	mov	r0, r2
	ldr	r1, .L441+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L434
.L439:
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	tst	r3, #2
	beq	.L417
	ldr	r1, .L441+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L434
.L420:
	mov	r3, #2
	mvn	r0, #2
	str	r3, [r4, #784]
	b	.L434
.L411:
	ldr	ip, [r4, #632]
	mov	r2, r4
	ldr	r3, [r4, #784]
	mov	r0, #1
	ldr	r1, .L441+12
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L413
.L419:
	ldr	r1, .L441+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L434
.L440:
	ldr	r1, .L441+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L434
.L437:
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	mov	r0, #1
	ldr	r1, .L441+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L434
.L442:
	.align	2
.L441:
	.word	.LC11
	.word	.LC15
	.word	.LC16
	.word	.LC12
	.word	.LC13
	.word	.LC17
	.word	.LC14
	UNWIND(.fnend)
	.size	CheckFrameStore, .-CheckFrameStore
	.align	2
	.global	ExchangePts
	.type	ExchangePts, %function
ExchangePts:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r10, r1, #608
	mvn	r5, #0
	mvn	r4, #0
	ldrd	r2, [r10]
	str	r0, [fp, #-52]
	cmp	r3, r5
	cmpeq	r2, r4
	beq	.L443
	add	r9, r0, #24576
	ldr	r3, [r9, #2148]
	cmp	r3, #0
	beq	.L443
	add	r8, r0, #26368
	add	r5, r0, #61440
	add	r8, r8, #156
	mvn	r6, #1
	mvn	r7, #0
	mov	r4, #0
	mvn	r3, #0
	str	r3, [fp, #-48]
.L448:
	ldr	r3, [r8, #4]!
	cmp	r3, #0
	beq	.L447
	ldr	r1, [r3, #632]
	ldr	r0, [r5, #1136]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L447
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	cmpne	r3, #0
	beq	.L447
	ldr	r3, [r8]
	add	r3, r3, #608
	ldrd	r0, [r3]
	cmp	r1, r7
	cmpeq	r0, r6
	strcc	r4, [fp, #-48]
	movcc	r6, r0
	movcc	r7, r1
.L447:
	ldr	r3, [r9, #2148]
	add	r4, r4, #1
	cmp	r4, r3
	bcc	.L448
	ldr	r3, [fp, #-48]
	cmn	r3, #1
	beq	.L443
	ldrd	r2, [r10]
	cmp	r3, r7
	cmpeq	r2, r6
	bhi	.L462
.L443:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L462:
	strd	r6, [sp]
	mov	r0, #29
	ldr	r1, .L463
	bl	dprint_vfmw
	ldr	r3, [fp, #-48]
	ldr	r1, [fp, #-52]
	add	r8, r3, #6592
	ldrd	r2, [r10]
	add	r8, r8, #40
	ldr	r1, [r1, r8, asl #2]
	add	r1, r1, #608
	strd	r2, [r1]
	strd	r6, [r10]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L464:
	.align	2
.L463:
	.word	.LC18
	UNWIND(.fnend)
	.size	ExchangePts, .-ExchangePts
	.align	2
	.global	GetImagePara
	.type	GetImagePara, %function
GetImagePara:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	lr, r0, #12288
	add	r3, r0, #20480
	mov	r8, #2208
	ldr	ip, [r1, #116]
	ldr	r2, [r3, #220]
	mov	r4, r1
	ldr	r7, [lr, #1304]
	add	r6, r0, #57344
	ldr	r3, [lr, #1300]
	movw	r5, #64764
	mov	lr, #4000
	movt	r5, 65533
	mla	r1, r8, r2, r7
	bic	r2, ip, #768
	bic	r2, r2, #3
	and	r5, r5, ip
	ldrb	ip, [r4, #1]	@ zero_extendqisi2
	ldr	r1, [r1, #28]
	str	r2, [r4, #116]
	ldr	r2, [r6, #3844]
	mla	r1, lr, r1, r3
	str	r5, [r4, #116]
	mov	r2, r2, asl #10
	str	r2, [r4, #144]
	ldr	r3, [r0]
	ldr	r3, [r3, #592]
	cmp	r3, #1
	moveq	r3, #262144
	movne	r3, #0
	orr	r3, r3, r5
	str	r3, [r4, #116]
	cmp	ip, #3
	ldrls	pc, [pc, ip, asl #2]
	b	.L503
.L469:
	.word	.L468
	.word	.L470
	.word	.L471
	.word	.L468
.L471:
	ldr	r2, [r4, #660]
	mov	r6, #3
	ldr	lr, [r4, #664]
	add	r5, r0, #61440
	ldr	ip, [r4, #768]
	and	r2, r2, r6
	str	r6, [r4, #108]
	add	r6, r0, #24576
	orr	r2, r2, #2816
	str	lr, [r4, #168]
	orr	r3, r2, r3
	str	ip, [r4, #112]
	str	r3, [r4, #116]
.L467:
	ldrb	r3, [r1, #18]	@ zero_extendqisi2
	str	r3, [r4, #484]
	str	r3, [r4, #488]
	ldrb	r3, [r1, #22]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r1, #3332]
	str	r3, [r4, #496]
	mov	r3, #1
	str	r3, [r4, #492]
	ldr	r3, [r0]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ble	.L488
	ldrb	r3, [r4, #676]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L488
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #3
	ldreq	r3, [r4, #116]
	biceq	r3, r3, #768
	streq	r3, [r4, #116]
.L488:
	mov	r1, r4
	bl	ExchangePts
	ldrb	r3, [r6, #1816]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L489
	ldr	r3, [r6, #1832]
	cmp	r3, #3
	moveq	r3, #1
	streq	r3, [r4, #364]
	beq	.L489
	cmp	r3, #4
	moveq	r3, #2
	streq	r3, [r4, #364]
	beq	.L489
	cmp	r3, #5
	moveq	r3, #3
	streq	r3, [r4, #364]
	beq	.L489
	mov	r0, #0
	ldr	r1, .L514
	str	r0, [r4, #364]
	ldr	r2, [r6, #1832]
	bl	dprint_vfmw
.L489:
	ldr	r0, [r5, #1136]
	ldr	r1, [r4, #632]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L514+4
	mov	r2, #624
	add	r1, r4, #8
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #0
	str	r3, [r5, #376]
	mov	r2, #1
	ldrb	r3, [r4, #676]	@ zero_extendqisi2
	strb	r2, [r5, #371]
	cmp	r3, #0
	movne	r3, #4
	strb	r3, [r5, #364]
	ldr	r3, [r4, #700]
	strb	r2, [r5, #369]
	str	r3, [r5, #380]
	str	r3, [r5, #388]
	str	r3, [r5, #384]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L470:
	ldr	r2, [r4, #660]
	mov	ip, #3
	ldr	r7, [r4, #664]
	add	r5, r0, #61440
	ldr	lr, [r4, #732]
	and	r2, r2, #3
	orr	r2, r2, #1792
	add	r6, r0, #24576
	orr	r3, r2, r3
	str	r7, [r4, #168]
	str	r3, [r4, #116]
	str	lr, [r4, #108]
	str	ip, [r4, #112]
	b	.L467
.L468:
	ldrb	r2, [r4, #676]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L472
	ldr	r2, [r4, #656]
	add	r5, r0, #61440
	ldr	ip, [r4, #664]
	mov	r9, #3072
	cmp	r2, #0
	ldr	r7, [r4, #732]
	ldr	r2, [r4, #660]
	ldr	lr, [r4, #768]
	moveq	r8, #512
	movne	r8, #768
	str	ip, [r4, #168]
.L474:
	ldr	r6, [r4, #724]
	ldr	ip, [r4, #760]
	cmp	r6, ip
	beq	.L511
	movle	ip, #1
	movgt	ip, #0
.L485:
	add	r6, r0, #24576
	ldrb	r10, [r6, #1852]	@ zero_extendqisi2
	cmp	r10, #3
	moveq	ip, #4096
	beq	.L486
	cmp	r10, #4
	movne	ip, ip, asl #12
	moveq	ip, #0
.L486:
	orr	r8, r9, r8
	bic	r3, r3, #12288
	orr	r3, r8, r3
	and	r2, r2, #3
	orr	r2, r3, r2
	str	r7, [r4, #108]
	orr	r2, r2, ip
	str	lr, [r4, #112]
	str	r2, [r4, #116]
	b	.L467
.L472:
	add	r5, r0, #61440
	ldr	r7, [r4, #728]
	ldr	ip, [r4, #764]
	ldr	r2, [r5, #1132]
	cmp	r7, r2
	bhi	.L512
	cmp	r2, ip
	bcc	.L478
.L476:
	ldr	ip, [r4, #664]
	mov	r8, #3
	ldr	r2, [r4, #660]
	ldr	r6, [r4, #732]
	str	ip, [r4, #168]
	ldr	ip, [r4, #768]
.L479:
	ldrb	lr, [r4, #3]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L513
	cmp	lr, #2
	streq	r7, [r4, #168]
	movne	r9, r8, asl #10
	moveq	r2, r6
	moveq	r9, #1024
.L481:
	mov	r7, r6
	mov	lr, ip
	mov	r8, #768
	str	r6, [r4, #108]
	str	ip, [r4, #112]
	b	.L474
.L478:
	ldr	r2, [r4, #732]
	mov	r8, #1
	ldr	ip, [r4, #768]
	str	r7, [r4, #168]
	mov	r6, r2
	b	.L479
.L511:
	ldr	ip, [r4, #120]
	cmp	ip, #720
	movne	ip, #1
	bne	.L485
	ldr	ip, [r4, #124]
	subs	ip, ip, #480
	movne	ip, #1
	b	.L485
.L512:
	cmp	r2, ip
	bcc	.L476
	ldr	r2, [r4, #768]
	mov	r8, #2
	str	ip, [r4, #168]
	ldr	r6, [r4, #732]
	mov	ip, r2
	b	.L479
.L513:
	ldr	lr, [r4, #764]
	mov	r2, ip
	mov	r9, #2048
	str	lr, [r4, #168]
	b	.L481
.L503:
	add	r5, r0, #61440
	add	r6, r0, #24576
	b	.L467
.L515:
	.align	2
.L514:
	.word	.LC19
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	GetImagePara, .-GetImagePara
	.align	2
	.global	SetFrmRepeatCount
	.type	SetFrmRepeatCount, %function
SetFrmRepeatCount:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r1, #24]
	ldmccfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	SetFrmRepeatCount.part.1
	UNWIND(.fnend)
	.size	SetFrmRepeatCount, .-SetFrmRepeatCount
	.align	2
	.global	SplitFrmToFlds
	.type	SplitFrmToFlds, %function
SplitFrmToFlds:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	lr, [r0, #677]	@ zero_extendqisi2
	mov	r5, #1
	ldrb	ip, [r0, #678]	@ zero_extendqisi2
	mov	r4, #2
	ldr	r1, [r0, #692]
	ldr	r2, [r0, #696]
	ldr	r3, [r0, #708]
	strb	r5, [r0, #712]
	strb	r4, [r0, #748]
	strb	lr, [r0, #713]
	strb	lr, [r0, #749]
	strb	ip, [r0, #714]
	strb	ip, [r0, #750]
	str	r1, [r0, #728]
	str	r1, [r0, #764]
	str	r2, [r0, #732]
	str	r2, [r0, #768]
	str	r3, [r0, #744]
	str	r3, [r0, #780]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	SplitFrmToFlds, .-SplitFrmToFlds
	.align	2
	.global	CombineFldsToFrm
	.type	CombineFldsToFrm, %function
CombineFldsToFrm:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r0, #760]
	mov	lr, #3
	ldr	r1, [r0, #724]
	ldr	r3, [r0, #764]
	cmp	r2, r1
	ldr	r5, [r0, #728]
	ldrb	r4, [r0, #749]	@ zero_extendqisi2
	ldr	ip, [r0, #744]
	movge	r2, r1
	add	r3, r3, r5
	cmp	r4, #0
	strb	lr, [r0, #676]
	mov	r3, r3, lsr #1
	str	ip, [r0, #708]
	str	r2, [r0, #688]
	str	r2, [r0, #648]
	str	r3, [r0, #692]
	beq	.L520
	ldrb	r2, [r0, #713]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L521
.L520:
	ldr	r2, [r0, #748]
	mov	r1, #0
	strb	r1, [r0, #677]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, r1
	beq	.L525
	ldr	r2, [r0, #712]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	movne	ip, #1
	bne	.L523
.L525:
	mov	ip, #0
.L523:
	ldr	r2, [r0, #768]
	ldr	r1, [r0, #732]
	str	r3, [r0, #664]
	cmp	r2, r1
	strb	ip, [r0, #678]
	movcs	r3, r2
	movcc	r3, r1
	str	r3, [r0, #696]
	str	r3, [r0, #660]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L521:
	mov	r1, #1
	strb	r1, [r0, #677]
	b	.L525
	UNWIND(.fnend)
	.size	CombineFldsToFrm, .-CombineFldsToFrm
	.align	2
	.global	GetAPC
	.type	GetAPC, %function
GetAPC:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	subs	r7, r1, #0
	mov	r5, r0
	beq	.L556
	cmp	r2, #1
	beq	.L537
	cmp	r2, #2
	beq	.L538
	ldr	r8, [r7, #724]
	ldr	r9, [r7, #760]
.L539:
	add	r6, r5, #24576
	ldr	ip, [r6, #2420]
	cmp	ip, #0
	beq	.L548
	ldr	r4, [r6, #2164]
	cmp	r4, #0
	addne	r3, r5, #26624
	movne	r4, #0
	addne	r3, r3, #116
	bne	.L542
	b	.L540
.L546:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L540
.L542:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L546
.L548:
	mvn	r0, #0
.L552:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L538:
	ldr	r8, [r7, #760]
	mov	r9, r8
	b	.L539
.L540:
	add	r3, r5, #61440
	ldr	r1, [r7, #632]
	ldr	r0, [r3, #1136]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L543
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L543
	add	r5, r5, r4, lsl #2
	mov	r3, #1
	add	r5, r5, #24576
	ldr	r1, .L557
	str	r3, [r5, #2164]
	ldr	r3, [r0, #640]
	mov	r0, #13
	ldr	r3, [r3, #8]
	str	r8, [r5, #2292]
	str	r9, [r5, #2356]
	str	r3, [r5, #2228]
	ldr	r3, [r6, #2424]
	cmp	r3, #15
	addls	r3, r3, #1
	movhi	r3, #16
	str	r3, [r6, #2424]
	mov	r3, r4
	ldr	r2, [r7, #164]
	bl	dprint_vfmw
	mov	r0, #0
	str	r4, [r7, #672]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L537:
	ldr	r8, [r7, #724]
	mov	r9, r8
	b	.L539
.L543:
	mov	r1, #0
	ldr	r2, [r7, #632]
	ldrb	r3, [r7, #2]	@ zero_extendqisi2
	stmia	sp, {r0, r1}
	mov	r0, r1
	ldr	r1, .L557+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L552
.L556:
	mov	r0, r7
	movw	r3, #2220
	ldr	r2, .L557+8
	ldr	r1, .L557+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L552
.L558:
	.align	2
.L557:
	.word	.LC22
	.word	.LC23
	.word	.LC20
	.word	.LC21
	UNWIND(.fnend)
	.size	GetAPC, .-GetAPC
	.align	2
	.global	SlidingWinMark
	.type	SlidingWinMark, %function
SlidingWinMark:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #24576
	add	r3, r0, #16384
	ldr	r1, [r2, #2156]
	ldr	r3, [r3, #456]
	ldr	r2, [r2, #2152]
	rsb	r3, r1, r3
	cmp	r2, r3
	ldmnefd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	SlidingWinMark.part.2
	UNWIND(.fnend)
	.size	SlidingWinMark, .-SlidingWinMark
	.align	2
	.global	DumpDPB
	.type	DumpDPB, %function
DumpDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r3, .L571
	mov	r5, r0
	ldr	r3, [r3]
	tst	r3, #16384
	beq	.L561
	add	r6, r0, #24576
	ldr	r3, [r6, #2148]
	cmp	r3, #0
	addne	r7, r0, #26368
	movne	r4, #0
	addne	r7, r7, #156
	beq	.L567
.L566:
	ldr	ip, [r7, #4]!
	mov	r2, r4
	ldr	r1, .L571+4
	mov	r0, #14
	add	r4, r4, #1
	ldrb	lr, [ip, #2]	@ zero_extendqisi2
	ldr	r3, [ip, #636]
	str	lr, [sp, #4]
	ldr	ip, [ip, #648]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r6, #2148]
	cmp	r3, r4
	bhi	.L566
.L567:
	ldr	r1, .L571+8
	mov	r0, #14
	bl	dprint_vfmw
	ldr	r3, [r6, #2152]
	cmp	r3, #0
	addne	r5, r5, #26368
	movne	r4, #0
	addne	r5, r5, #220
	beq	.L565
.L568:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L571+12
	mov	r0, #14
	ldrb	lr, [ip, #2]	@ zero_extendqisi2
	ldr	r3, [ip, #636]
	str	lr, [sp, #4]
	ldr	ip, [ip, #648]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r5]
	mov	r2, r4
	ldr	r1, .L571+16
	mov	r0, #14
	add	r4, r4, #1
	ldr	r3, [r3, #688]
	bl	dprint_vfmw
	ldr	r3, [r6, #2152]
	cmp	r3, r4
	bhi	.L568
.L565:
	ldr	r1, .L571+8
	mov	r0, #14
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L561:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L572:
	.align	2
.L571:
	.word	g_PrintEnable
	.word	.LC24
	.word	.LC2
	.word	.LC25
	.word	.LC26
	UNWIND(.fnend)
	.size	DumpDPB, .-DumpDPB
	.align	2
	.global	UpdateCurrFrameInfo
	.type	UpdateCurrFrameInfo, %function
UpdateCurrFrameInfo:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #57344
	ldrb	r2, [r3, #1738]	@ zero_extendqisi2
	ldr	r4, [r3, #2376]
	strb	r2, [r4]
	ldrb	r2, [r3, #1740]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
	bne	.L574
	ldrb	r2, [r3, #1741]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
.L574:
	strb	r2, [r4, #2]
	ldr	r2, [r3, #2384]
	str	r2, [r4, #636]
	ldr	r2, [r3, #2420]
	str	r2, [r4, #664]
	ldr	r2, [r3, #2412]
	str	r2, [r4, #660]
	ldr	r2, [r3, #2396]
	str	r2, [r4, #648]
	ldr	r2, [r3, #2444]
	str	r2, [r4, #668]
	ldr	r2, [r3, #2388]
	str	r2, [r4, #644]
	ldrb	r2, [r3, #1745]	@ zero_extendqisi2
	str	r2, [r4, #656]
	ldrb	r2, [r3, #1743]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r1, #3
	movne	r1, #0
	strb	r1, [r4, #3]
	ldrb	r2, [r3, #1739]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L577
	bcc	.L578
	cmp	r2, #2
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	str	r4, [r4, #780]
	mov	r0, #1
	strb	r0, [r4, #748]
	mov	r5, #0
	ldrb	r0, [r3, #1740]	@ zero_extendqisi2
	strb	r0, [r4, #749]
	ldrb	r0, [r3, #1741]	@ zero_extendqisi2
	strb	r0, [r4, #750]
	ldr	r0, [r3, #2404]
	str	r0, [r4, #760]
	ldr	ip, [r3, #2412]
	str	ip, [r4, #768]
	ldr	r0, [r3, #2420]
	str	r5, [r4, #656]
	str	r0, [r4, #764]
	ldrb	lr, [r3, #1737]	@ zero_extendqisi2
	cmp	lr, r5
	beq	.L595
	ldr	r2, [r4, #728]
	mov	lr, #3
	strb	lr, [r4, #1]
	mvn	r7, #0
	add	r0, r0, r2
	str	r0, [r4, #664]
	str	r0, [r4, #692]
	mvn	r6, #0
	ldrb	r2, [r3, #1743]	@ zero_extendqisi2
	cmp	r2, #1
	add	r2, r3, #2352
	orreq	r5, r1, #2
	strb	r5, [r4, #3]
	ldrd	r8, [r2]
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L586
	add	r2, r4, #608
	ldrd	r0, [r2]
	cmp	r1, r7
	cmpeq	r0, r6
	beq	.L596
.L586:
	ldr	r2, [r4, #732]
.L593:
	cmp	r2, ip
	mov	r0, r4
	movcc	r2, ip
	str	r2, [r4, #660]
	ldr	r3, [r3, #2396]
	str	r3, [r4, #648]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	CombineFldsToFrm
.L578:
	str	r4, [r4, #708]
	mov	r1, #3
	mov	r2, #0
	strb	r1, [r4, #1]
	strb	r2, [r4, #676]
	add	r1, r3, #1744
	ldrb	r5, [r3, #1740]	@ zero_extendqisi2
	mov	r2, #624
	add	r1, r1, #8
	add	r0, r4, #8
	strb	r5, [r4, #677]
	ldrb	r6, [r3, #1741]	@ zero_extendqisi2
	strb	r6, [r4, #678]
	ldr	ip, [r3, #2400]
	str	ip, [r4, #688]
	ldr	r7, [r3, #2412]
	str	r7, [r4, #696]
	ldr	r8, [r3, #2420]
	str	r8, [r4, #692]
	ldr	ip, [r3, #2404]
	str	ip, [r4, #724]
	ldr	r3, [r3, #2408]
	str	r3, [r4, #760]
	bl	memcpy
	mov	r2, #1
	mov	r3, #2
	str	r8, [r4, #728]
	str	r8, [r4, #764]
	str	r7, [r4, #732]
	str	r7, [r4, #768]
	strb	r6, [r4, #714]
	strb	r6, [r4, #750]
	strb	r5, [r4, #713]
	strb	r5, [r4, #749]
	str	r4, [r4, #744]
	str	r4, [r4, #780]
	strb	r2, [r4, #712]
	strb	r3, [r4, #748]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L577:
	strb	r2, [r4, #712]
	mov	lr, #0
	str	r4, [r4, #744]
	ldrb	r0, [r3, #1740]	@ zero_extendqisi2
	strb	r0, [r4, #713]
	ldrb	r0, [r3, #1741]	@ zero_extendqisi2
	strb	r0, [r4, #714]
	ldr	r0, [r3, #2404]
	str	r0, [r4, #724]
	ldr	ip, [r3, #2412]
	str	ip, [r4, #732]
	ldr	r0, [r3, #2420]
	str	lr, [r4, #656]
	str	r0, [r4, #728]
	ldrb	r5, [r3, #1737]	@ zero_extendqisi2
	cmp	r5, lr
	beq	.L597
	ldr	r2, [r4, #764]
	mov	r5, #3
	strb	r5, [r4, #1]
	mvn	r7, #0
	add	r0, r0, r2
	str	r0, [r4, #664]
	str	r0, [r4, #692]
	mvn	r6, #0
	ldrb	r2, [r3, #1743]	@ zero_extendqisi2
	cmp	r2, #1
	add	r2, r3, #2352
	orreq	lr, r1, #1
	strb	lr, [r4, #3]
	ldrd	r8, [r2]
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L582
	add	r2, r4, #608
	ldrd	r0, [r2]
	cmp	r1, r7
	cmpeq	r0, r6
	streqd	r8, [r2]
.L582:
	ldr	r2, [r4, #768]
	b	.L593
.L595:
	strb	r2, [r4, #1]
	add	r1, r3, #1744
	str	r0, [r4, #664]
	add	r1, r1, #8
	str	r0, [r4, #692]
	mov	r2, #624
	ldrb	r3, [r3, #1743]	@ zero_extendqisi2
	add	r0, r4, #8
	cmp	r3, #1
	moveq	lr, #2
	strb	lr, [r4, #3]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L597:
	strb	r2, [r4, #1]
	add	r1, r3, #1744
	str	r0, [r4, #664]
	add	r1, r1, #8
	str	r0, [r4, #692]
	mov	r2, #624
	ldrb	r3, [r3, #1743]	@ zero_extendqisi2
	add	r0, r4, #8
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r4, #3]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L596:
	strd	r8, [r2]
	b	.L586
	UNWIND(.fnend)
	.size	UpdateCurrFrameInfo, .-UpdateCurrFrameInfo
	.align	2
	.global	SimpleSlideDPB
	.type	SimpleSlideDPB, %function
SimpleSlideDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #24576
	mov	r4, r0
	ldr	r1, [r3, #2148]
	ldr	r2, [r3, #2144]
	cmp	r1, r2
	bcs	.L604
.L599:
	add	r0, r4, #57344
	mov	r2, #0
	ldr	r3, [r0, #2392]
	strb	r2, [r0, #1740]
	cmp	r3, r2
	movne	r3, #1
	strneb	r3, [r0, #1741]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L604:
	ldr	r1, [r3, #1952]
	bl	UnMarkFrameStoreRef
	mov	r1, #0
	mov	r0, r4
	bl	RemoveFrameStoreOutDPB
	b	.L599
	UNWIND(.fnend)
	.size	SimpleSlideDPB, .-SimpleSlideDPB
	.align	2
	.global	ReleaseNAL
	.type	ReleaseNAL, %function
ReleaseNAL:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L613
	add	r7, r5, #56
	mov	r4, r5
	mov	r3, #0
	strb	r3, [r5, #1]
	strb	r3, [r5, #4]
.L608:
	ldr	r3, [r4, #8]
	mov	r0, r6
	cmp	r3, #0
	beq	.L607
	ldr	r1, [r4, #32]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #12]
	ldr	r2, [r4, #8]
	mov	r0, #7
	ldr	r1, .L614
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #8]
	str	r3, [r4, #24]
	str	r3, [r4, #12]
.L607:
	add	r4, r4, #28
	cmp	r4, r7
	bne	.L608
	mov	r3, #0
	strb	r3, [r5]
	str	r3, [r5, #68]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L613:
	mov	r0, r5
	movw	r3, #4204
	ldr	r2, .L614+4
	ldr	r1, .L614+8
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L615:
	.align	2
.L614:
	.word	.LC27
	.word	.LC20
	.word	.LC21
	UNWIND(.fnend)
	.size	ReleaseNAL, .-ReleaseNAL
	.align	2
	.global	ClearCurrNal
	.type	ClearCurrNal, %function
ClearCurrNal:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, [r0, #368]
	mov	r4, r0
	cmp	r1, #0
	beq	.L617
	add	r3, r0, #61440
	ldr	r0, [r3, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L617:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	ClearCurrNal, .-ClearCurrNal
	.align	2
	.global	ClearCurrSlice
	.type	ClearCurrSlice, %function
ClearCurrSlice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, .L626
	mov	r5, r0
	mov	r0, #1
	add	r4, r5, #61440
	bl	dprint_vfmw
	ldr	r1, [r5, #368]
	cmp	r1, #0
	beq	.L622
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r5, #368]
.L622:
	ldr	r3, [r4, #1092]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #1092]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L627:
	.align	2
.L626:
	.word	.LC28
	UNWIND(.fnend)
	.size	ClearCurrSlice, .-ClearCurrSlice
	.align	2
	.global	ClearAllNal
	.type	ClearAllNal, %function
ClearAllNal:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #13504
	add	r4, r0, #376
	add	r5, r5, #24
	add	r6, r0, #61440
	b	.L630
.L629:
	add	r4, r4, #96
	cmp	r4, r5
	beq	.L632
.L630:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L629
	mov	r1, r4
	ldr	r0, [r6, #1136]
	add	r4, r4, #96
	bl	ReleaseNAL
	cmp	r4, r5
	bne	.L630
.L632:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	ClearAllNal, .-ClearAllNal
	.align	2
	.global	ClearAllSlice
	.type	ClearAllSlice, %function
ClearAllSlice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #60416
	add	r5, r0, #60928
	add	r4, r4, #132
	add	r5, r5, #164
	add	r6, r0, #61440
	mov	r7, #0
.L635:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L634
	ldr	r0, [r6, #1136]
	bl	ReleaseNAL
	str	r7, [r4]
.L634:
	cmp	r4, r5
	bne	.L635
	mov	r0, #0
	str	r0, [r6, #1060]
	str	r0, [r6, #1120]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	ClearAllSlice, .-ClearAllSlice
	.align	2
	.global	H264_ClearCurrPic
	.type	H264_ClearCurrPic, %function
H264_ClearCurrPic:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	bl	ClearAllSlice
	add	r0, r4, #12288
	add	r3, r4, #20480
	mov	r2, #0
	movw	r1, #23352
	ldr	r0, [r0, #1292]
	movt	r1, 1
	mov	ip, #2
	str	r1, [r3, #1872]
	strb	ip, [r3, #1857]
	mov	r1, #7
	str	r0, [r3, #1864]
	mov	r0, #32
	str	r1, [r3, #1868]
	mov	r1, #3
	strb	r0, [r3, #1858]
	mvn	r0, #0
	str	r1, [r3, #1880]
	mov	r1, #262144
	str	r0, [r3, #1884]
	mov	r0, r2
	str	r1, [r3, #1896]
	strb	r2, [r3, #1856]
	str	r2, [r3, #1888]
	str	r2, [r3, #1892]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	H264_ClearCurrPic, .-H264_ClearCurrPic
	.align	2
	.global	H264_ArrangeVahbMem
	.type	H264_ArrangeVahbMem, %function
H264_ArrangeVahbMem:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	ldr	r3, .L651
	mov	r5, r1
	mov	r6, r2
	mov	r4, r0
	mov	r2, #32
	ldr	r3, [r3, #48]
	sub	r0, fp, #60
	mov	r1, #0
	blx	r3
	ldr	r3, [r4]
	str	r6, [fp, #-32]
	mov	r0, #0
	str	r5, [fp, #-52]
	ldr	r3, [r3, #28]
	str	r6, [fp, #-48]
	cmp	r3, #24
	str	r5, [fp, #-44]
	str	r6, [fp, #-40]
	str	r5, [fp, #-36]
	strb	r0, [fp, #-60]
	strb	r0, [fp, #-59]
	beq	.L642
	add	r2, r4, #24576
	ldrb	r3, [fp, #4]	@ zero_extendqisi2
	mov	r1, #5
	strb	r0, [fp, #-56]
	ldr	r2, [r2, #2144]
	strb	r1, [fp, #-55]
	add	r2, r2, #1
	strb	r2, [fp, #-57]
.L648:
	add	r5, r4, #61440
	sub	r1, fp, #60
	mov	r6, #0
	strb	r3, [fp, #-54]
	ldr	r0, [r5, #1136]
	strb	r6, [fp, #-58]
	bl	FSP_ConfigInstance
	cmp	r0, r6
	bne	.L643
	ldr	r1, [r4]
	sub	r3, fp, #64
	ldr	r0, [r5, #1136]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L650
	cmp	r0, #0
	moveq	r0, #1
	bne	.L645
.L649:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L642:
	mov	r3, #1
	mov	r2, #5
	strb	r0, [fp, #-56]
	strb	r3, [fp, #-57]
	strb	r2, [fp, #-55]
	b	.L648
.L650:
	add	r0, r4, #8
	bl	ResetVoQueue
.L645:
	ldr	r1, .L651+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L649
.L643:
	mov	r0, r6
	ldr	r1, .L651+8
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L649
.L652:
	.align	2
.L651:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC29
	.word	.LC30
	UNWIND(.fnend)
	.size	H264_ArrangeVahbMem, .-H264_ArrangeVahbMem
	.align	2
	.global	RepairList
	.type	RepairList, %function
RepairList:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	mov	r4, r0
	ldrb	r2, [r3, #208]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L655
	bcc	.L670
	cmp	r2, #2
	bne	.L679
.L671:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L679:
	ldr	r1, .L683
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L670:
	mov	r6, #1
.L656:
	add	r0, r4, #20480
	add	lr, r4, #26880
	add	r0, r0, #248
	mov	r5, #0
	add	lr, lr, #252
	add	r7, r4, #57344
	ldr	r3, [r0, #4]!
	mov	r2, r5
	mov	r8, #1
	cmp	r3, #0
	beq	.L658
.L681:
	ldr	r10, [lr]
	cmp	r10, #0
	bne	.L672
	mov	ip, lr
	mov	r1, r10
	b	.L660
.L661:
	ldr	r9, [ip, #4]!
	cmp	r9, #0
	bne	.L659
.L660:
	add	r1, r1, #1
	cmp	r1, r3
	bne	.L661
	ldr	r1, [r4]
	ldr	r1, [r1, #12]
	cmp	r1, #2
	bne	.L680
	ldr	r5, [r7, #2376]
	add	r5, r5, #676
.L663:
	mov	r9, r10
	mov	ip, lr
	mov	r1, #0
	b	.L669
.L665:
	ldr	r9, [ip, #4]!
.L669:
	add	r1, r1, #1
	cmp	r9, #0
	streq	r5, [ip]
	cmp	r1, r3
	bne	.L665
	add	r2, r2, #1
	add	lr, lr, #132
	cmp	r6, r2
	bls	.L671
.L678:
	ldr	r3, [r0, #4]!
	cmp	r3, #0
	bne	.L681
.L658:
	ldr	r1, [r4]
	ldr	r1, [r1, #12]
	cmp	r1, #2
	bne	.L682
	add	r2, r2, #1
	str	r8, [r0]
	cmp	r6, r2
	ldr	r3, [r7, #2376]
	add	lr, lr, #132
	add	r3, r3, #676
	str	r3, [lr, #-132]
	bhi	.L678
	b	.L671
.L672:
	mov	r9, r10
.L659:
	mov	r5, r9
	b	.L663
.L655:
	mov	r6, #2
	b	.L656
.L682:
	ldr	r1, .L683+4
.L676:
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L680:
	mov	r2, r5
	mov	r3, #0
	ldr	r1, .L683+8
	b	.L676
.L684:
	.align	2
.L683:
	.word	.LC31
	.word	.LC33
	.word	.LC32
	UNWIND(.fnend)
	.size	RepairList, .-RepairList
	.align	2
	.global	GetShortTermPicPoint
	.type	GetShortTermPicPoint, %function
GetShortTermPicPoint:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	ldrb	r4, [r3, #209]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L686
	ldrb	r3, [r3, #210]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r4, #2
	moveq	r4, #1
.L686:
	add	r3, r0, #24576
	ldr	r5, [r3, #2152]
	cmp	r5, #0
	beq	.L694
	add	r3, r0, #26368
	mov	r2, #0
	add	r3, r3, #224
	b	.L691
.L704:
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L702
.L689:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L703
.L691:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L704
	ldrb	lr, [r0, #2]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L690
	ldr	ip, [r0, #712]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	beq	.L705
.L690:
	tst	lr, #2
	beq	.L689
	ldr	ip, [r0, #748]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L689
	ldr	ip, [r0, #756]
	cmp	ip, r1
	bne	.L689
	add	r0, r0, #748
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L702:
	ldr	ip, [r0, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L689
	ldr	ip, [r0, #684]
	cmp	ip, r1
	bne	.L689
	add	r0, r0, #676
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L705:
	ldr	ip, [r0, #720]
	cmp	ip, r1
	bne	.L690
	add	r0, r0, #712
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L703:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L694:
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	GetShortTermPicPoint, .-GetShortTermPicPoint
	.align	2
	.global	GetLongTermPicPoint
	.type	GetLongTermPicPoint, %function
GetLongTermPicPoint:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	ldrb	r4, [r3, #209]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L707
	ldrb	r3, [r3, #210]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r4, #2
	moveq	r4, #1
.L707:
	add	r3, r0, #24576
	ldr	r5, [r3, #2156]
	cmp	r5, #0
	beq	.L715
	add	r3, r0, #26624
	mov	r2, #0
	add	r3, r3, #32
	b	.L712
.L725:
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L723
.L710:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L724
.L712:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L725
	ldrb	lr, [r0, #2]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L711
	ldr	ip, [r0, #712]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	beq	.L726
.L711:
	tst	lr, #2
	beq	.L710
	ldr	ip, [r0, #748]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L710
	ldr	ip, [r0, #752]
	cmp	ip, r1
	bne	.L710
	add	r0, r0, #748
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L723:
	ldr	ip, [r0, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L710
	ldr	ip, [r0, #680]
	cmp	ip, r1
	bne	.L710
	add	r0, r0, #676
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L726:
	ldr	ip, [r0, #716]
	cmp	ip, r1
	bne	.L711
	add	r0, r0, #712
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L724:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L715:
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	GetLongTermPicPoint, .-GetLongTermPicPoint
	.align	2
	.global	ReorderSTList
	.type	ReorderSTList, %function
ReorderSTList:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r7, [fp, #4]
	mov	r5, r1
	mov	r1, r3
	mov	r8, r2
	mov	r6, r3
	bl	GetShortTermPicPoint
	ldr	r4, [r7]
	add	r2, r8, #1
	cmp	r2, r4
	ble	.L728
	add	lr, r5, r2, lsl #2
	mov	ip, r2
.L729:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L729
.L728:
	add	r3, r4, #1
	str	r3, [r7]
	str	r0, [r5, r4, asl #2]
	ldr	ip, [r7]
	cmp	r2, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r5, ip, lsl #2
	mov	lr, ip
.L733:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L731
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L732
	ldr	r3, [r0, #8]
	cmp	r3, r6
	beq	.L731
.L732:
	str	r0, [r5, lr, asl #2]
	add	lr, lr, #1
.L731:
	cmp	ip, r2
	bne	.L733
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	ReorderSTList, .-ReorderSTList
	.align	2
	.global	ReorderLTList
	.type	ReorderLTList, %function
ReorderLTList:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r7, [fp, #4]
	mov	r5, r1
	mov	r1, r3
	mov	r8, r2
	mov	r6, r3
	bl	GetLongTermPicPoint
	ldr	r4, [r7]
	add	r2, r8, #1
	cmp	r2, r4
	ble	.L741
	add	lr, r5, r2, lsl #2
	mov	ip, r2
.L742:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L742
.L741:
	add	r3, r4, #1
	str	r3, [r7]
	str	r0, [r5, r4, asl #2]
	ldr	ip, [r7]
	cmp	r2, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r5, ip, lsl #2
	mov	lr, ip
.L746:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L744
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L745
	ldr	r3, [r0, #4]
	cmp	r3, r6
	beq	.L744
.L745:
	str	r0, [r5, lr, asl #2]
	add	lr, lr, #1
.L744:
	cmp	ip, r2
	bne	.L746
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	ReorderLTList, .-ReorderLTList
	.align	2
	.global	ReorderRefPiclist
	.type	ReorderRefPiclist, %function
ReorderRefPiclist:
	UNWIND(.fnstart)
	@ args = 12, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r2, r0, #20480
	str	r1, [fp, #-56]
	mov	r8, r3
	ldrb	r1, [r2, #209]	@ zero_extendqisi2
	mov	r3, #0
	mov	r7, r0
	str	r3, [fp, #-48]
	cmp	r1, r3
	ldr	ip, [fp, #4]
	add	r3, r0, #12288
	beq	.L757
	ldr	lr, [r2, #220]
	mov	r4, #2208
	ldr	r5, [r3, #1304]
	mov	r0, #4000
	ldr	r6, [r2, #228]
	mov	r1, #2
	ldr	r3, [r3, #1300]
	mla	r2, r4, lr, r5
	mov	lr, r6, asl #1
	add	lr, lr, #1
	str	lr, [fp, #-64]
	ldr	r2, [r2, #28]
	mla	r3, r0, r2, r3
	ldr	r3, [r3, #2196]
	add	r3, r3, #4
	mov	r3, r1, asl r3
	str	r3, [fp, #-60]
.L767:
	adds	r9, r8, #2
	beq	.L756
	ldr	r3, [ip]
	cmp	r3, #3
	beq	.L756
	mov	r2, r8
	ldr	r6, [fp, #8]
	ldr	r5, [fp, #-64]
	mov	r4, #0
	mov	r8, ip
	mov	r10, r2
	b	.L760
.L761:
	ldr	r3, [fp, #12]
	sub	r1, fp, #48
	mov	r2, r10
	mov	r0, r7
	ldr	r3, [r3, r4, asl #2]
	add	r4, r4, #1
	str	r1, [sp]
	ldr	r1, [fp, #-56]
	bl	ReorderLTList
	cmp	r4, r9
	beq	.L756
.L766:
	ldr	r3, [r8, #4]!
	add	r6, r6, #4
	cmp	r3, #3
	beq	.L756
.L760:
	cmp	r3, #1
	bhi	.L761
	cmp	r3, #0
	ldr	r3, [r6]
	bne	.L762
	mvn	r3, r3
	adds	r5, r5, r3
	ldrmi	r3, [fp, #-60]
	addmi	r5, r5, r3
.L763:
	ldr	r3, [fp, #-64]
	sub	r2, fp, #48
	ldr	r1, [fp, #-56]
	mov	r0, r7
	cmp	r3, r5
	str	r2, [sp]
	add	r4, r4, #1
	mov	r2, r10
	ldrlt	r3, [fp, #-60]
	movge	r3, r5
	rsblt	r3, r3, r5
	bl	ReorderSTList
	cmp	r4, r9
	bne	.L766
.L756:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L762:
	add	r3, r3, #1
	add	r5, r5, r3
	ldr	r3, [fp, #-60]
	cmp	r3, r5
	rsble	r5, r3, r5
	b	.L763
.L757:
	ldr	r1, [r2, #220]
	mov	r4, #2208
	ldr	r5, [r3, #1304]
	mov	lr, #4000
	ldr	r2, [r2, #228]
	mov	r0, #1
	ldr	r3, [r3, #1300]
	mla	r1, r4, r1, r5
	str	r2, [fp, #-64]
	ldr	r2, [r1, #28]
	mla	r3, lr, r2, r3
	ldr	r3, [r3, #2196]
	add	r3, r3, #4
	mov	r3, r0, asl r3
	str	r3, [fp, #-60]
	b	.L767
	UNWIND(.fnend)
	.size	ReorderRefPiclist, .-ReorderRefPiclist
	.align	2
	.global	ReorderListX
	.type	ReorderListX, %function
ReorderListX:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	add	r4, r0, #20480
	mov	r5, r0
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L775
	add	r6, r0, #16384
	ldrb	r2, [r6, #3512]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L787
.L778:
	cmp	r3, #1
	ldr	r3, [r4, #264]
	add	r3, r3, #1
	str	r3, [r4, #252]
	bne	.L775
	ldrb	r3, [r6, #3513]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L788
.L780:
	ldr	r3, [r4, #268]
	add	r3, r3, #1
	str	r3, [r4, #256]
.L775:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L787:
	add	r7, r0, #20224
	add	lr, r0, #19968
	add	ip, r0, #19712
	add	r1, r0, #26880
	ldr	r3, [r4, #264]
	add	lr, lr, #196
	add	ip, ip, #188
	add	r7, r7, #204
	stmia	sp, {ip, lr}
	add	r2, r4, #252
	str	r7, [sp, #8]
	add	r1, r1, #252
	bl	ReorderRefPiclist
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	b	.L778
.L788:
	add	ip, r5, #20480
	add	r0, r5, #20224
	add	r2, r5, #19968
	add	r1, r5, #27136
	ldr	r3, [r4, #268]
	add	ip, ip, #76
	add	r0, r0, #72
	add	r2, r2, #64
	stmib	sp, {r0, ip}
	add	r1, r1, #128
	str	r2, [sp]
	mov	r0, r5
	add	r2, r5, #20736
	bl	ReorderRefPiclist
	b	.L780
	UNWIND(.fnend)
	.size	ReorderListX, .-ReorderListX
	.align	2
	.global	GenPiclistfromFrmlist
	.type	GenPiclistfromFrmlist, %function
GenPiclistfromFrmlist:
	UNWIND(.fnstart)
	@ args = 8, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r2, #31
	ldr	r6, [fp, #8]
	mov	r7, r1
	ldr	lr, .L852
	movcc	r4, r2
	ldr	ip, .L852+4
	movcs	r4, #31
	cmp	r6, #0
	str	r3, [fp, #-52]
	movne	r6, lr
	moveq	r6, ip
	cmp	r0, #1
	moveq	r5, #0
	moveq	r10, r5
	beq	.L792
	cmp	r0, #2
	moveq	r5, #0
	moveq	r10, r5
	bne	.L800
.L801:
	cmp	r10, r4
	movcs	r3, #0
	movcc	r3, #1
	cmp	r5, r4
	str	r3, [fp, #-48]
	movcs	r2, #0
	movcc	r2, #1
	orrs	r3, r2, r3
	beq	.L800
	cmp	r2, #0
	beq	.L806
	add	r8, r7, r5, lsl #2
	b	.L805
.L802:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L806
.L805:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	ip, [r0, #1]	@ zero_extendqisi2
	tst	ip, #2
	beq	.L802
	add	r0, r0, #748
	blx	r6
	cmp	r0, #0
	beq	.L802
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #748
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L806:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r10, lsl #2
	bne	.L808
	b	.L801
.L807:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L801
.L808:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	tst	r2, #1
	beq	.L807
	add	r0, r0, #712
	blx	r6
	cmp	r0, #0
	beq	.L807
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #712
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
	b	.L801
.L851:
	add	r0, r0, #712
	blx	r6
	cmp	r0, #0
	beq	.L793
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r8]
	ldr	r0, [r3]
	add	r2, r2, #712
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L797:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r5, lsl #2
	bne	.L799
	b	.L792
.L798:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L792
.L799:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	tst	r2, #2
	beq	.L798
	add	r0, r0, #748
	blx	r6
	cmp	r0, #0
	beq	.L798
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #748
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L792:
	cmp	r10, r4
	movcs	r2, #0
	movcc	r2, #1
	cmp	r5, r4
	movcs	r3, #0
	movcc	r3, #1
	str	r3, [fp, #-48]
	orrs	r3, r3, r2
	beq	.L800
	cmp	r2, #0
	beq	.L797
	add	r9, r7, r10, lsl #2
	b	.L796
.L793:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L797
.L796:
	mov	r8, r9
	ldr	r0, [r9], #4
	ldrb	ip, [r0, #1]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L793
	b	.L851
.L800:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L853:
	.align	2
.L852:
	.word	IsLTRefFlg
	.word	IsSTRefFlg
	UNWIND(.fnend)
	.size	GenPiclistfromFrmlist, .-GenPiclistfromFrmlist
	.align	2
	.global	InitListX
	.type	InitListX, %function
InitListX:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 208
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #220)
	sub	sp, sp, #220
	add	r6, r0, #20480
	mov	r4, r0
	add	r8, r0, #57344
	ldrb	r3, [r6, #208]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L855
	ldrb	r7, [r8, #1739]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L979
	add	r3, r0, #24576
	str	r3, [fp, #-240]
	ldr	ip, [r3, #2152]
	cmp	ip, #0
	beq	.L922
	add	r1, r0, #26368
	mov	r3, r7
	add	r1, r1, #220
	b	.L861
.L860:
	cmp	r3, ip
	beq	.L859
.L861:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L860
	ldr	r0, [r2, #676]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #65536
	bne	.L860
	add	r0, r7, #6720
	cmp	r3, ip
	add	r0, r0, #62
	add	r2, r2, #676
	add	r7, r7, #1
	add	r0, r4, r0, lsl #2
	str	r2, [r0, #4]
	bne	.L861
.L859:
	add	r0, r4, #26880
	ldr	r3, .L987
	mov	r1, r7
	add	r0, r0, #252
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	str	r7, [r6, #252]
	ldr	r1, [r3, #2156]
	cmp	r1, #0
	beq	.L923
	add	r0, r4, #26624
	mov	r5, r7
	add	r0, r0, #28
	mov	r3, #0
	b	.L864
.L863:
	cmp	r3, r1
	beq	.L980
.L864:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L863
	ldr	ip, [r2, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L863
	add	ip, r5, #6720
	cmp	r3, r1
	add	ip, ip, #62
	add	r2, r2, #676
	add	r5, r5, #1
	add	ip, r4, ip, lsl #2
	str	r2, [ip, #4]
	bne	.L864
.L980:
	rsb	r1, r7, r5
.L862:
	add	r0, r4, r7, lsl #2
	ldr	r3, .L987+4
	add	r0, r0, #26880
	mov	r2, #4
	add	r0, r0, #252
	bl	qsort
	mov	r2, r5
	cmp	r2, #0
	mov	r3, #0
	str	r5, [r6, #252]
	str	r3, [r6, #256]
	beq	.L981
.L870:
	ldr	r0, [r6, #264]
	ldr	r1, [r6, #268]
	add	r0, r0, #1
	cmp	r2, r0
	add	r1, r1, #1
	movcs	r2, r0
	cmp	r3, r1
	str	r2, [r6, #252]
	movcs	r3, r1
	cmp	r2, #32
	str	r3, [r6, #256]
	bhi	.L914
	add	r2, r2, #6720
	add	r1, r4, #27136
	add	r2, r2, #62
	add	r1, r1, #124
	mov	r0, #0
	add	r2, r4, r2, lsl #2
.L913:
	str	r0, [r2, #4]!
	cmp	r2, r1
	bne	.L913
.L914:
	cmp	r3, #32
	bhi	.L912
	add	r3, r3, #6784
	add	r1, r4, #27392
	add	r3, r3, #31
	mov	r2, #0
	add	r4, r4, r3, lsl #2
.L915:
	str	r2, [r4, #4]!
	cmp	r4, r1
	bne	.L915
.L912:
	mov	r0, #0
.L965:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L855:
	ldrb	r5, [r8, #1739]	@ zero_extendqisi2
	add	r3, r0, #24576
	str	r3, [fp, #-240]
	cmp	r5, #0
	beq	.L876
	ldr	ip, [r3, #2152]
	cmp	ip, #0
	beq	.L982
	add	r9, r0, #26368
	mov	r5, #0
	add	r9, r9, #220
	mov	r3, r5
	mov	r1, r9
.L895:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L894
	sub	r0, fp, #44
	ldr	lr, [r8, #2396]
	add	r7, r0, r5, lsl #2
	ldr	r0, [r2, #648]
	cmp	lr, r0
	strge	r2, [r7, #-192]
	addge	r5, r5, #1
.L894:
	cmp	r3, ip
	bne	.L895
	sub	r10, fp, #236
	ldr	r3, .L987+8
	mov	r2, #4
	mov	r1, r5
	mov	r0, r10
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2152]
	cmp	r7, #0
	beq	.L976
.L920:
	mov	r3, r9
	mov	r2, #0
	mov	r9, r5
.L898:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L897
	sub	r0, fp, #44
	ldr	ip, [r8, #2396]
	add	lr, r0, r9, lsl #2
	ldr	r0, [r1, #648]
	cmp	ip, r0
	strlt	r1, [lr, #-192]
	addlt	r9, r9, #1
.L897:
	cmp	r2, r7
	bne	.L898
	rsb	r7, r5, r9
	mov	r3, r5, asl #2
	add	r0, r10, r3
	str	r3, [fp, #-244]
	mov	r1, r7
	ldr	r3, .L987+12
	mov	r2, #4
	bl	qsort
	cmp	r5, #0
	subeq	ip, fp, #172
	beq	.L899
.L917:
	sub	ip, fp, #172
	mov	r2, r10
	add	r7, ip, r7, lsl #2
	mov	r3, #0
.L900:
	add	r3, r3, #1
	ldr	r1, [r2], #4
	cmp	r3, r5
	str	r1, [r7], #4
	bcc	.L900
.L899:
	cmp	r9, r5
	bls	.L901
	ldr	r3, [fp, #-244]
	add	r2, r10, r3
	mov	r3, ip
.L902:
	add	r5, r5, #1
	ldr	r1, [r2], #4
	cmp	r5, r9
	str	r1, [r3], #4
	bne	.L902
.L901:
	add	r2, r4, #20480
	add	r3, r4, #26880
	add	r2, r2, #252
	mov	r5, #0
	add	r3, r3, #252
	str	r5, [r6, #252]
	str	r5, [r6, #256]
	mov	r1, r10
	ldrb	r0, [r8, #1739]	@ zero_extendqisi2
	add	r10, r4, #27136
	str	ip, [fp, #-252]
	mov	ip, r2
	str	r2, [fp, #-248]
	mov	r2, r9
	str	ip, [sp]
	add	r10, r10, #128
	str	r5, [sp, #4]
	str	r3, [fp, #-244]
	bl	GenPiclistfromFrmlist
	ldr	ip, [fp, #-252]
	mov	r2, r9
	ldrb	r0, [r8, #1739]	@ zero_extendqisi2
	mov	r3, r10
	str	r5, [sp, #4]
	mov	r1, ip
	add	ip, r4, #20736
	str	ip, [sp]
	str	ip, [fp, #-252]
	bl	GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2156]
	cmp	r7, r5
	subeq	r9, fp, #108
	beq	.L903
	add	r3, r4, #26624
	sub	r9, fp, #108
	add	r3, r3, #28
	mov	r2, r9
.L904:
	add	r5, r5, #1
	ldr	r1, [r3, #4]!
	cmp	r5, r7
	str	r1, [r2], #4
	bne	.L904
.L903:
	ldr	r3, .L987+16
	mov	r2, #4
	mov	r1, r7
	mov	r0, r9
	bl	qsort
	ldr	r3, [fp, #-248]
	ldrb	r0, [r8, #1739]	@ zero_extendqisi2
	mov	r5, #1
	mov	r2, r7
	mov	r1, r9
	str	r3, [sp]
	str	r5, [sp, #4]
	ldr	r3, [fp, #-244]
	bl	GenPiclistfromFrmlist
	ldr	r2, [fp, #-252]
	ldrb	r0, [r8, #1739]	@ zero_extendqisi2
	mov	r3, r10
	mov	r1, r9
	str	r5, [sp, #4]
	str	r2, [sp]
	mov	r2, r7
	bl	GenPiclistfromFrmlist
	ldr	r2, [r6, #252]
	ldr	r3, [r6, #256]
	rsb	r1, r3, r2
	clz	r1, r1
	mov	r1, r1, lsr #5
.L893:
	cmp	r2, #1
	movls	r1, #0
	andhi	r1, r1, #1
	cmp	r1, #0
	beq	.L905
	cmp	r2, #0
	ldr	r1, [fp, #-240]
	beq	.L906
	ldr	r7, [r1, #2556]
	ldr	r1, [r1, #2688]
	cmp	r7, r1
	bne	.L905
	add	r0, r4, #27136
	ldr	ip, [fp, #-244]
	add	r0, r0, #128
	mov	r1, #0
	b	.L907
.L908:
	ldr	r5, [ip, #4]!
	ldr	lr, [r0, #4]!
	cmp	r5, lr
	bne	.L905
.L907:
	add	r1, r1, #1
	cmp	r1, r2
	bne	.L908
.L909:
	ldr	r0, [fp, #-240]
	ldr	r1, [r0, #2692]
	str	r7, [r0, #2692]
	str	r1, [r0, #2688]
.L905:
	orrs	r1, r2, r3
	bne	.L870
	ldr	r1, .L987+20
.L971:
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	b	.L965
.L876:
	ldr	r10, [r3, #2152]
	cmp	r10, #0
	beq	.L879
	add	r9, r0, #26368
	mov	r3, r5
	add	r9, r9, #220
	mov	r1, r9
	b	.L881
.L880:
	cmp	r3, r10
	beq	.L983
.L881:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L880
	add	r0, r5, #6720
	ldr	ip, [r2, #688]
	add	r0, r0, #62
	ldr	lr, [r8, #2400]
	add	r2, r2, #676
	add	r0, r4, r0, lsl #2
	cmp	lr, ip
	addge	r5, r5, #1
	strge	r2, [r0, #4]
	cmp	r3, r10
	bne	.L881
.L983:
	add	r3, r4, #26880
	mov	r1, r5
	mov	r2, r3
	add	r2, r2, #252
	ldr	r3, .L987+24
	mov	r0, r2
	str	r2, [fp, #-244]
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2152]
	cmp	r7, #0
	beq	.L975
.L918:
	mov	r3, r9
	mov	r10, r5
	mov	r2, #0
	b	.L884
.L883:
	cmp	r2, r7
	beq	.L984
.L884:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L883
	add	r0, r10, #6720
	ldr	ip, [r1, #688]
	add	r0, r0, #62
	ldr	lr, [r8, #2400]
	add	r1, r1, #676
	add	r0, r4, r0, lsl #2
	cmp	lr, ip
	addlt	r10, r10, #1
	strlt	r1, [r0, #4]
	cmp	r2, r7
	bne	.L884
.L984:
	add	r8, r5, #6720
	rsb	r7, r5, r10
	add	r8, r8, #62
	mov	r2, #4
	ldr	r3, .L987+28
	mov	r1, r7
	add	r0, r4, r8, lsl #2
	add	r9, r4, r10, lsl #2
	add	r0, r0, r2
	add	r9, r9, #26880
	bl	qsort
	cmp	r5, #0
	add	r9, r9, #252
	beq	.L886
.L916:
	add	r7, r7, #6784
	add	r2, r4, #26880
	add	r7, r7, #31
	add	r2, r2, #248
	mov	r3, #0
	add	r7, r4, r7, lsl #2
.L887:
	add	r3, r3, #1
	ldr	r1, [r2, #4]!
	cmp	r3, r5
	str	r1, [r7, #4]!
	bcc	.L887
.L886:
	cmp	r10, r5
	bls	.L888
	add	r3, r4, #27136
	add	r8, r4, r8, lsl #2
	add	r3, r3, #124
.L889:
	add	r5, r5, #1
	ldr	r2, [r8, #4]!
	cmp	r5, r10
	str	r2, [r3, #4]!
	bne	.L889
.L888:
	ldr	r3, [fp, #-240]
	str	r10, [r6, #256]
	str	r10, [r6, #252]
	ldr	r1, [r3, #2156]
	cmp	r1, #0
	beq	.L925
	add	r0, r4, #26624
	mov	r5, r10
	add	r0, r0, #28
	mov	r3, #0
	b	.L892
.L891:
	cmp	r3, r1
	beq	.L985
.L892:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #1]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L891
	ldrb	ip, [r2, #677]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L891
	add	ip, r4, r5, lsl #2
	cmp	r3, r1
	add	ip, ip, #24576
	add	r2, r2, #676
	add	r5, r5, #1
	str	r2, [ip, #2556]
	str	r2, [ip, #2688]
	bne	.L892
.L985:
	rsb	r1, r10, r5
.L890:
	mov	r0, r9
	ldr	r3, .L987+4
	mov	r2, #4
	bl	qsort
	ldr	r0, [r6, #252]
	ldr	r3, .L987+4
	mov	r2, #4
	rsb	r1, r0, r5
	add	r0, r4, r0, lsl #2
	add	r0, r0, #27136
	add	r0, r0, #128
	bl	qsort
	mov	r3, r5
	str	r5, [r6, #256]
	mov	r2, r5
	str	r5, [r6, #252]
	mov	r1, #1
	b	.L893
.L979:
	add	r2, r0, #24576
	str	r2, [fp, #-240]
	ldr	lr, [r2, #2152]
	cmp	lr, #0
	moveq	r7, lr
	beq	.L858
	add	r1, r0, #26368
	mov	r7, r3
	add	r1, r1, #220
.L867:
	ldr	r2, [r1, #4]!
	sub	r0, fp, #44
	add	ip, r0, r7, lsl #2
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	strne	r2, [ip, #-192]
	addne	r7, r7, #1
	cmp	r3, lr
	bne	.L867
.L858:
	add	r10, r4, #26880
	mov	r1, r7
	ldr	r3, .L987+32
	mov	r2, #4
	sub	r0, fp, #236
	add	r10, r10, #252
	bl	qsort
	add	ip, r4, #20480
	add	ip, ip, #252
	mov	r5, #0
	mov	r2, r7
	str	r5, [r6, #252]
	mov	r3, r10
	ldrb	r0, [r8, #1739]	@ zero_extendqisi2
	sub	r1, fp, #236
	str	ip, [sp]
	str	r5, [sp, #4]
	str	ip, [fp, #-244]
	bl	GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	ip, [fp, #-244]
	ldr	r7, [r3, #2156]
	cmp	r7, r5
	subeq	r9, fp, #108
	beq	.L868
	add	r3, r4, #26624
	sub	r9, fp, #108
	add	r3, r3, #28
	mov	r2, r9
.L869:
	add	r5, r5, #1
	ldr	r1, [r3, #4]!
	cmp	r5, r7
	str	r1, [r2], #4
	bne	.L869
.L868:
	ldr	r3, .L987+16
	mov	r2, #4
	mov	r1, r7
	mov	r0, r9
	str	ip, [fp, #-244]
	bl	qsort
	ldr	ip, [fp, #-244]
	mov	r3, r10
	ldrb	r0, [r8, #1739]	@ zero_extendqisi2
	mov	r2, r7
	mov	r1, r9
	str	ip, [sp]
	mov	ip, #1
	str	ip, [sp, #4]
	bl	GenPiclistfromFrmlist
	ldr	r2, [r6, #252]
	mov	r3, #0
	str	r3, [r6, #256]
	cmp	r2, #0
	bne	.L870
.L981:
	ldr	r3, [r4]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L871
	ldrb	r3, [r8, #1739]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L986
	cmp	r3, #1
	ldr	r2, [fp, #-240]
	ldr	r3, [r8, #2376]
	addeq	r3, r3, #712
	addne	r3, r3, #748
	str	r3, [r2, #2556]
.L873:
	mov	r3, #0
	mov	r2, #1
	b	.L870
.L986:
	ldr	r2, [fp, #-240]
	ldr	r3, [r8, #2376]
	add	r3, r3, #676
	str	r3, [r2, #2556]
	b	.L873
.L906:
	ldr	r7, [r1, #2688]
	b	.L909
.L975:
	add	r8, r5, #6720
	mov	r2, #4
	add	r8, r8, #62
	ldr	r3, .L987+28
	mov	r1, r7
	add	r9, r4, r8, lsl #2
	add	r9, r9, r2
	mov	r0, r9
	bl	qsort
	subs	r10, r5, #0
	bne	.L916
	b	.L888
.L976:
	mov	r3, r5, asl #2
	mov	r1, r7
	str	r3, [fp, #-244]
	mov	r2, r3
	ldr	r3, .L987+12
	add	r0, r10, r2
	mov	r2, #4
	bl	qsort
	subs	r9, r5, #0
	bne	.L917
.L972:
	sub	ip, fp, #172
	b	.L901
.L922:
	mov	r7, ip
	b	.L859
.L982:
	sub	r10, fp, #236
	ldr	r3, .L987+8
	mov	r1, ip
	mov	r2, #4
	mov	r0, r10
	str	ip, [fp, #-244]
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	ip, [fp, #-244]
	ldr	r7, [r3, #2152]
	cmp	r7, #0
	addne	r9, r4, #26368
	movne	r5, ip
	addne	r9, r9, #220
	bne	.L920
	ldr	r3, .L987+12
	mov	r2, #4
	mov	r1, r7
	mov	r0, r10
	mov	r9, r7
	bl	qsort
	b	.L972
.L879:
	add	r3, r0, #26880
	mov	r1, r10
	mov	r2, r3
	add	r2, r2, #252
	ldr	r3, .L987+24
	mov	r0, r2
	str	r2, [fp, #-244]
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2152]
	cmp	r7, #0
	addne	r9, r4, #26368
	movne	r5, r10
	addne	r9, r9, #220
	bne	.L918
	ldr	r9, [fp, #-244]
	mov	r2, #4
	ldr	r3, .L987+28
	mov	r1, r7
	mov	r10, r7
	mov	r0, r9
	bl	qsort
	b	.L888
.L923:
	mov	r5, r7
	b	.L862
.L925:
	mov	r5, r10
	b	.L890
.L871:
	ldr	r1, .L987+36
	b	.L971
.L988:
	.align	2
.L987:
	.word	compare_pic_by_pic_num_desc
	.word	compare_pic_by_lt_pic_num_asc
	.word	compare_fs_by_poc_desc
	.word	compare_fs_by_poc_asc
	.word	compare_fs_by_lt_pic_idx_asc
	.word	.LC35
	.word	compare_pic_by_poc_desc
	.word	compare_pic_by_poc_asc
	.word	compare_fs_by_frame_num_desc
	.word	.LC34
	UNWIND(.fnend)
	.size	InitListX, .-InitListX
	.align	2
	.global	DumpList
	.type	DumpList, %function
DumpList:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L991
	ldr	r3, [r3]
	tst	r3, #8192
	ldmeqfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DumpList.part.5
.L992:
	.align	2
.L991:
	.word	g_PrintEnable
	UNWIND(.fnend)
	.size	DumpList, .-DumpList
	.align	2
	.global	FindNearestPOCPicId
	.type	FindNearestPOCPicId, %function
FindNearestPOCPicId:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #24576
	ldr	r4, [r3, #2148]
	cmp	r4, #0
	beq	.L998
	ldr	r3, [r3, #1952]
	add	r2, r0, #57344
	ldr	r5, [r2, #2396]
	ldr	lr, [r3, #648]
	rsb	lr, lr, r5
	cmp	lr, #0
	rsblt	lr, lr, #0
	cmp	r4, #1
	bls	.L999
	add	r1, r0, #26368
	mov	r2, #1
	add	r1, r1, #160
	mov	r0, #0
.L997:
	ldr	ip, [r1, #4]!
	add	r2, r2, #1
	ldr	r3, [ip, #648]
	rsb	r3, r3, r5
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, lr
	ldrlt	r0, [ip, #164]
	movlt	lr, r3
	cmp	r2, r4
	bne	.L997
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L999:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L998:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	FindNearestPOCPicId, .-FindNearestPOCPicId
	.align	2
	.global	FindMinRefIdx
	.type	FindMinRefIdx, %function
FindMinRefIdx:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	ldrb	r2, [r3, #208]	@ zero_extendqisi2
	cmp	r2, #2
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	add	r2, r0, #57344
	add	r5, r0, #26880
	ldrb	ip, [r2, #1739]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L1031
	ldr	lr, [r3, #252]
	mov	r4, r5
	mov	r6, r5
	mov	r7, ip
	cmp	lr, #0
	add	r5, r5, #124
	add	r4, r4, #128
	add	r6, r6, #248
	beq	.L1009
.L1033:
	mov	r2, r6
	mov	r0, #32
	mov	r3, #0
	b	.L1007
.L1006:
	add	r3, r3, #1
	cmp	r3, lr
	beq	.L1032
.L1007:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #672]
	cmp	r1, ip
	bne	.L1006
	cmp	r0, r3
	movcs	r0, r3
	add	r3, r3, #1
	cmp	r3, lr
	str	r0, [r4, #-4]
	str	r0, [r5, #4]
	bne	.L1007
.L1032:
	cmp	r0, #31
	bhi	.L1009
.L1008:
	add	ip, ip, #1
	add	r5, r5, #8
	cmp	ip, #16
	add	r4, r4, #8
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	lr, #0
	bne	.L1033
.L1009:
	str	r7, [r4, #-4]
	str	r7, [r5, #4]
	b	.L1008
.L1031:
	ldr	r4, [r3, #252]
	mov	ip, #0
	mov	r6, r5
	mov	r7, ip
	cmp	r4, #0
	add	r5, r5, #124
	add	r6, r6, #248
	beq	.L1013
.L1035:
	mov	r1, r6
	mov	lr, #32
	mov	r2, #0
	b	.L1011
.L1010:
	add	r2, r2, #1
	cmp	r2, r4
	beq	.L1034
.L1011:
	ldr	r3, [r1, #4]!
	ldr	r8, [r3, #32]
	ldrb	r0, [r3]	@ zero_extendqisi2
	ldr	r3, [r8, #672]
	mov	r3, r3, asl #1
	cmp	r0, #2
	orreq	r3, r3, #1
	cmp	r3, ip
	bne	.L1010
	cmp	lr, r2
	movcs	lr, r2
	add	r2, r2, #1
	cmp	r2, r4
	str	lr, [r5]
	bne	.L1011
.L1034:
	cmp	lr, #31
	bhi	.L1013
.L1012:
	add	ip, ip, #1
	add	r5, r5, #4
	cmp	ip, #32
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r4, #0
	bne	.L1035
.L1013:
	str	r7, [r5]
	b	.L1012
	UNWIND(.fnend)
	.size	FindMinRefIdx, .-FindMinRefIdx
	.align	2
	.global	DecList
	.type	DecList, %function
DecList:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r6, r0
	bl	FindNearestPOCPicId
	add	r5, r6, #20480
	mvn	r4, #0
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	str	r4, [r5, #1836]
	cmp	r3, #2
	str	r0, [r5, #1840]
	beq	.L1049
	mov	r0, r6
	bl	InitListX
	cmp	r0, #0
	bne	.L1050
	ldr	r4, .L1053
	mov	r0, #13
	ldr	r1, .L1053+4
	bl	dprint_vfmw
	ldr	r3, [r4]
	tst	r3, #8192
	bne	.L1051
.L1040:
	mov	r0, r6
	bl	ReorderListX
	mov	r0, r6
	bl	RepairList
	cmp	r0, #0
	bne	.L1042
	mov	r0, r6
	bl	FindMinRefIdx
	add	r3, r6, #24576
	mov	r0, #13
	ldr	r1, .L1053+8
	ldr	r3, [r3, #2556]
	cmp	r3, #0
	ldrne	r3, [r3, #32]
	mvneq	r3, #0
	ldrne	r3, [r3, #164]
	str	r3, [r5, #1836]
	bl	dprint_vfmw
	ldr	r4, [r4]
	ands	r4, r4, #8192
	bne	.L1052
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1052:
	mov	r0, r6
	bl	DumpList.part.5
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1049:
	mov	r0, #0
	str	r0, [r5, #252]
	str	r0, [r5, #256]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1051:
	mov	r0, r6
	bl	DumpList.part.5
	b	.L1040
.L1042:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1050:
	ldr	r1, .L1053+12
	mov	r0, #13
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1054:
	.align	2
.L1053:
	.word	g_PrintEnable
	.word	.LC37
	.word	.LC38
	.word	.LC36
	UNWIND(.fnend)
	.size	DecList, .-DecList
	.align	2
	.global	NoPicOut
	.type	NoPicOut, %function
NoPicOut:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, #57344
	ldr	ip, [r4, #3820]
	cmp	ip, #0
	addne	r3, r0, #27904
	movne	r2, #0
	addne	r3, r3, #148
	movne	lr, r2
	bne	.L1061
	b	.L1062
.L1070:
	ldrb	r1, [r3, #-648]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1059
	cmp	r2, ip
	add	r3, r3, #792
	beq	.L1062
.L1061:
	ldr	r1, [r3]
	add	r2, r2, #1
	cmp	r1, #1
	bne	.L1070
.L1059:
	cmp	r2, ip
	strb	lr, [r3, #-651]
	strb	lr, [r3, #-650]
	add	r3, r3, #792
	bne	.L1061
.L1062:
	ldr	r2, [r4, #3824]
	cmp	r2, #0
	beq	.L1071
	add	r0, r0, #60928
	mov	r3, #0
	add	r0, r0, #164
	mov	r1, r3
.L1063:
	add	r3, r3, #1
	str	r1, [r0, #4]!
	cmp	r3, r2
	bne	.L1063
	ldmfd	sp, {r4, fp, sp, pc}
.L1071:
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	NoPicOut, .-NoPicOut
	.align	2
	.global	GetBackPicFromVOQueue
	.type	GetBackPicFromVOQueue, %function
GetBackPicFromVOQueue:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r7, r0, #61440
	str	r0, [fp, #-48]
	ldr	r0, [r7, #1136]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1088
.L1073:
	ldr	r3, [fp, #-48]
	add	r6, r3, #57344
	ldr	r3, [r6, #3820]
	cmp	r3, #0
	beq	.L1080
	ldr	r3, [fp, #-48]
	mov	r8, #0
	mov	r9, #0
	mov	r10, #0
	add	r5, r3, #27904
	add	r5, r5, #88
.L1079:
	sub	r4, r5, #32
	mov	r3, #0
	strb	r3, [r5, #-591]
	strb	r3, [r5, #-590]
.L1078:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1089
.L1077:
	cmp	r4, r5
	bne	.L1078
	ldr	r3, [r6, #3820]
	add	r10, r10, #1
	add	r5, r5, #792
	cmp	r3, r10
	bhi	.L1079
.L1080:
	ldr	r2, [r6, #3824]
	cmp	r2, #0
	beq	.L1072
	ldr	r3, [fp, #-48]
	add	r6, r3, #60928
	mov	r3, #0
	add	r6, r6, #164
	mov	r1, r3
.L1081:
	add	r3, r3, #1
	str	r1, [r6, #4]!
	cmp	r3, r2
	bne	.L1081
.L1072:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1089:
	mov	r1, r2
	ldr	r0, [r7, #1136]
	bl	FreeUsdByDec
	strd	r8, [r4]
	b	.L1077
.L1088:
	ldr	r3, [fp, #-48]
	add	r0, r3, #8
	bl	ResetVoQueue
	b	.L1073
	UNWIND(.fnend)
	.size	GetBackPicFromVOQueue, .-GetBackPicFromVOQueue
	.align	2
	.global	wait_vo
	.type	wait_vo, %function
wait_vo:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	wait_vo, .-wait_vo
	.align	2
	.global	GetReRangeFlag
	.type	GetReRangeFlag, %function
GetReRangeFlag:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r7, r0, #12288
	add	r3, r0, #20480
	mov	r5, r0
	ldr	r0, [r7, #1304]
	mov	ip, #2208
	ldr	r3, [r3, #220]
	mov	r4, #4000
	ldr	r2, [r7, #1300]
	mov	r6, r1
	ldr	lr, [r7, #1276]
	mla	r3, ip, r3, r0
	ldr	r0, [r7, #1280]
	ldr	r3, [r3, #28]
	mla	r4, r4, r3, r2
	ldrb	r3, [r4, #18]	@ zero_extendqisi2
	ldr	r1, [r4, #3252]
	rsb	r3, r3, #2
	ldr	r2, [r4, #3248]
	mla	r3, r1, r3, r3
	add	r2, r2, #1
	cmp	r2, lr
	cmpeq	r3, r0
	movne	r1, #1
	moveq	r1, #0
	bne	.L1092
	add	r8, r5, #24576
	ldr	ip, [r4, #3272]
	ldr	r8, [r8, #2144]
	add	r8, r8, #1
	cmp	ip, r8
	movls	r3, r1
	bls	.L1093
.L1092:
	ldr	r1, .L1097
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L1096
	add	r1, r5, #61440
	mov	r0, r0, asl #4
	mov	r2, r2, asl #4
	mov	r3, r3, asl #4
	mov	lr, lr, asl #4
	strh	r0, [fp, #-42]	@ movhi
	strh	r2, [fp, #-40]	@ movhi
	sub	r2, fp, #44
	strh	r3, [fp, #-38]	@ movhi
	mov	r3, #8
	ldr	r0, [r1, #1136]
	mov	r1, #2
	strh	lr, [fp, #-44]	@ movhi
	blx	ip
.L1096:
	ldr	ip, [r4, #3272]
	mov	r3, #1
.L1093:
	add	r5, r5, #16384
	ldr	r2, [r4, #36]
	ldr	lr, [r7, #1344]
	mov	r0, r3
	ldr	r1, [r5, #484]
	cmp	lr, r2
	cmpeq	r1, ip
	movne	r2, #1
	moveq	r2, #0
	orr	r3, r2, r3
	str	r3, [r6]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1098:
	.align	2
.L1097:
	.word	g_event_report
	UNWIND(.fnend)
	.size	GetReRangeFlag, .-GetReRangeFlag
	.global	__aeabi_uidiv
	.global	__aeabi_uidivmod
	.align	2
	.global	DecPOC
	.type	DecPOC, %function
DecPOC:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #12288
	add	r6, r0, #20480
	mov	r4, #2208
	mov	r5, #4000
	ldr	r1, [r6, #220]
	mov	ip, #1
	ldr	r3, [r2, #1304]
	ldr	lr, [r2, #1300]
	ldrb	r2, [r6, #211]	@ zero_extendqisi2
	mla	r3, r4, r1, r3
	sub	r2, r2, #5
	clz	r2, r2
	mov	r2, r2, lsr #5
	ldr	r3, [r3, #28]
	mla	r5, r5, r3, lr
	ldr	r1, [r5, #2200]
	ldr	r3, [r5, #2196]
	cmp	r1, ip
	ldr	lr, [r5, #2204]
	add	r3, r3, #4
	mov	r3, ip, asl r3
	beq	.L1101
	bcc	.L1102
	cmp	r1, #2
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r2, #0
	add	r4, r0, #16384
	bne	.L1157
	ldr	r1, [r4, #3444]
	cmp	r1, #0
	bne	.L1135
	ldr	r2, [r4, #3484]
	ldr	r0, [r4, #3480]
	ldr	r1, [r4, #3492]
	cmp	r2, r0
	addcc	r1, r1, r3
	strcc	r1, [r4, #3488]
	bcs	.L1137
.L1138:
	add	r3, r1, r2
	str	r3, [r4, #3476]
	ldr	r0, [r6, #240]
	mov	r3, r3, asl #1
	cmp	r0, #0
	subeq	r3, r3, #1
	str	r3, [r4, #3464]
	ldrb	r0, [r6, #209]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1141
	str	r3, [r4, #3460]
	str	r3, [r4, #3456]
	str	r3, [r4, #3452]
.L1134:
	str	r2, [r4, #3480]
	str	r1, [r4, #3492]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1102:
	cmp	r2, #0
	add	lr, lr, #4
	add	r4, r0, #16384
	mov	ip, ip, asl lr
	bne	.L1108
	ldr	r3, [r4, #3444]
	cmp	r3, #0
	bne	.L1106
	ldr	lr, [r4, #3428]
	ldr	r1, [r4, #3432]
	mov	r2, lr
.L1107:
	ldr	r0, [r4, #3420]
	cmp	r0, r1
	bcs	.L1105
	rsb	r3, r0, r1
	cmp	r3, ip, lsr #1
	addcs	r2, r2, ip
	strcs	r2, [r4, #3436]
	bcc	.L1105
.L1109:
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1111
.L1159:
	ldr	r3, [r4, #3424]
	add	r2, r2, r0
	str	r2, [r4, #3452]
	add	r3, r2, r3
	str	r3, [r4, #3456]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r4, #3464]
.L1112:
	ldr	r2, [r4, #3484]
	ldr	r1, [r4, #3480]
	str	r3, [r4, #3460]
	cmp	r2, r1
	strne	r2, [r4, #3480]
	ldr	r3, [r6, #240]
	cmp	r3, #0
	ldrne	r2, [r4, #3420]
	ldrne	r3, [r4, #3436]
	strne	r2, [r4, #3432]
	strne	r3, [r4, #3428]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1101:
	cmp	r2, #0
	add	r4, r0, #16384
	movne	r3, #0
	strne	r3, [r4, #3488]
	bne	.L1116
	ldr	r1, [r4, #3444]
	cmp	r1, #0
	beq	.L1117
	mov	r3, r2
	str	r2, [r4, #3492]
	str	r2, [r4, #3480]
.L1118:
	str	r3, [r4, #3488]
.L1116:
	ldr	r3, [r5, #2216]
	cmp	r3, #0
	beq	.L1120
	ldr	r3, [r4, #3484]
	ldr	r0, [r4, #3488]
	add	r0, r3, r0
	str	r0, [r4, #3476]
	ldr	r7, [r6, #240]
	cmp	r7, #0
	bne	.L1122
	cmp	r0, #0
	beq	.L1123
	sub	r0, r0, #1
	str	r0, [r4, #3476]
.L1122:
	mov	r3, #0
	str	r3, [r4, #3508]
	ldr	r1, [r5, #2216]
	cmp	r1, r3
	bgt	.L1143
	cmp	r0, #0
	beq	.L1126
.L1160:
	sub	r8, r0, #1
	mov	r0, r8
	bl	__aeabi_uidiv
	mov	r9, r0
	mov	r0, r8
	str	r9, [r4, #3500]
	ldr	r1, [r5, #2216]
	bl	__aeabi_uidivmod
	ldr	r0, [r4, #3508]
	mul	r0, r0, r9
	str	r0, [r4, #3504]
	cmp	r1, #0
	str	r1, [r4, #3496]
	blt	.L1127
	add	r2, r5, #2208
	add	r1, r1, #1
	add	r2, r2, #8
	mov	r3, #0
.L1128:
	add	r3, r3, #1
	ldr	ip, [r2, #4]!
	cmp	r3, r1
	add	r0, r0, ip
	str	r0, [r4, #3504]
	bne	.L1128
.L1127:
	cmp	r7, #0
	ldreq	r3, [r5, #2208]
	addeq	r0, r0, r3
	streq	r0, [r4, #3504]
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1130
	ldr	r2, [r4, #3468]
	ldr	r1, [r4, #3472]
	add	r2, r0, r2
	str	r2, [r4, #3452]
	ldr	r3, [r5, #2212]
	add	r3, r2, r3
	add	r0, r3, r1
	str	r0, [r4, #3456]
	cmp	r0, r2
	movge	r0, r2
	str	r0, [r4, #3464]
.L1131:
	ldr	r2, [r4, #3484]
	ldr	r3, [r4, #3488]
	str	r0, [r4, #3460]
	str	r2, [r4, #3480]
	str	r3, [r4, #3492]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1106:
	ldr	lr, [r4, #3448]
	cmp	lr, #0
	beq	.L1158
.L1108:
	ldr	r0, [r4, #3420]
	mov	r3, #0
	mov	lr, r3
	mov	r2, r1
	str	r3, [r4, #3428]
	str	r3, [r4, #3432]
.L1105:
	cmp	r0, r1
	bls	.L1110
	rsb	r1, r1, r0
	cmp	r1, ip, lsr #1
	rsbhi	r2, ip, r2
	strhi	r2, [r4, #3436]
	bhi	.L1109
.L1110:
	str	lr, [r4, #3436]
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1159
.L1111:
	ldrb	r3, [r6, #210]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r2, r0
	streq	r3, [r4, #3452]
	strne	r3, [r4, #3456]
	str	r3, [r4, #3464]
	b	.L1112
.L1157:
	mov	r3, #0
	ldr	r2, [r4, #3484]
	mov	r1, r3
	str	r3, [r4, #3488]
	str	r3, [r4, #3456]
	str	r3, [r4, #3452]
	str	r3, [r4, #3460]
	str	r3, [r4, #3464]
	b	.L1134
.L1120:
	str	r3, [r4, #3476]
	ldr	r7, [r6, #240]
.L1123:
	mov	r0, #0
	str	r0, [r4, #3508]
	ldr	r3, [r5, #2216]
	cmp	r3, r0
	ble	.L1126
.L1143:
	add	ip, r5, #2208
	mov	r3, #0
	add	ip, ip, #8
	mov	r2, r3
.L1125:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	add	r3, r3, r1
	str	r3, [r4, #3508]
	ldr	r1, [r5, #2216]
	cmp	r1, r2
	bgt	.L1125
	cmp	r0, #0
	bne	.L1160
.L1126:
	mov	r0, #0
	str	r0, [r4, #3504]
	b	.L1127
.L1130:
	ldrb	r3, [r6, #210]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r2, [r5, #2212]
	ldreq	r3, [r4, #3468]
	ldrne	r3, [r4, #3468]
	addne	r0, r0, r2
	addeq	r0, r0, r3
	streq	r0, [r4, #3452]
	addne	r0, r0, r3
	strne	r0, [r4, #3456]
	str	r0, [r4, #3464]
	b	.L1131
.L1117:
	ldr	r1, [r4, #3484]
	ldr	r2, [r4, #3480]
	cmp	r1, r2
	bcs	.L1119
	ldr	r2, [r4, #3492]
	add	r3, r3, r2
	str	r3, [r4, #3488]
	b	.L1116
.L1141:
	ldrb	r0, [r6, #210]	@ zero_extendqisi2
	str	r3, [r4, #3460]
	cmp	r0, #0
	streq	r3, [r4, #3452]
	strne	r3, [r4, #3456]
	b	.L1134
.L1135:
	str	r2, [r4, #3480]
	mov	r1, r2
	str	r2, [r4, #3492]
	ldr	r2, [r4, #3484]
.L1137:
	str	r1, [r4, #3488]
	b	.L1138
.L1158:
	ldr	r3, [r4, #3452]
	mov	r2, r1
	str	lr, [r4, #3428]
	mov	r1, r3
	str	r3, [r4, #3432]
	b	.L1107
.L1119:
	ldr	r3, [r4, #3492]
	b	.L1118
	UNWIND(.fnend)
	.size	DecPOC, .-DecPOC
	.align	2
	.global	CalcPicNum
	.type	CalcPicNum, %function
CalcPicNum:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #12288
	add	r3, r0, #20480
	mov	r4, #2208
	mov	lr, #4000
	ldr	ip, [r3, #220]
	add	r6, r0, #57344
	ldr	r1, [r2, #1304]
	mov	r7, #1
	ldr	r3, [r2, #1300]
	ldrb	r2, [r6, #1739]	@ zero_extendqisi2
	mla	r1, r4, ip, r1
	cmp	r2, #0
	ldr	r1, [r1, #28]
	mla	r3, lr, r1, r3
	ldr	r3, [r3, #2196]
	add	r3, r3, #4
	mov	r7, r7, asl r3
	add	r3, r0, #24576
	beq	.L1162
	sub	r5, r2, #1
	sub	r4, r2, #2
	ldr	r2, [r3, #2152]
	clz	r5, r5
	clz	r4, r4
	cmp	r2, #0
	mov	r5, r5, lsr #5
	addne	lr, r0, #26368
	mov	r4, r4, lsr #5
	movne	r1, #0
	addne	lr, lr, #220
	beq	.L1182
.L1181:
	ldr	ip, [lr, #4]!
	ldrb	r2, [ip, #2]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1176
	ldr	r2, [ip, #636]
	ldr	r8, [r6, #2384]
	cmp	r2, r8
	rsbhi	r2, r7, r2
	str	r2, [ip, #640]
	ldr	r2, [lr]
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L1179
	ldr	r8, [r2, #712]
	bic	r8, r8, #-16777216
	bic	r8, r8, #255
	cmp	r8, #65536
	ldreq	ip, [r2, #640]
	addeq	ip, r5, ip, lsl #1
	streq	ip, [r2, #720]
	ldreq	r2, [lr]
	ldreqb	ip, [r2, #2]	@ zero_extendqisi2
.L1179:
	tst	ip, #2
	beq	.L1176
	ldr	ip, [r2, #748]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	ldreq	ip, [r2, #640]
	addeq	ip, r4, ip, lsl #1
	streq	ip, [r2, #756]
.L1176:
	ldr	r2, [r3, #2152]
	add	r1, r1, #1
	cmp	r2, r1
	bhi	.L1181
.L1182:
	ldr	r2, [r3, #2156]
	cmp	r2, #0
	beq	.L1206
	add	r0, r0, #26624
	mov	r2, #0
	add	r0, r0, #28
.L1187:
	ldr	ip, [r0, #4]!
	add	r2, r2, #1
	ldrb	r1, [ip, #2]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1184
	tst	r1, #1
	beq	.L1185
	ldr	lr, [ip, #712]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	ldreq	r1, [ip, #644]
	addeq	r1, r5, r1, lsl #1
	streq	r1, [ip, #716]
	ldreq	ip, [r0]
	ldreqb	r1, [ip, #2]	@ zero_extendqisi2
.L1185:
	tst	r1, #2
	beq	.L1184
	ldr	r1, [ip, #748]
	bic	r1, r1, #-16777216
	bic	r1, r1, #255
	cmp	r1, #256
	ldreq	r1, [ip, #644]
	addeq	r1, r4, r1, lsl #1
	streq	r1, [ip, #752]
.L1184:
	ldr	r1, [r3, #2156]
	cmp	r1, r2
	bhi	.L1187
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1162:
	ldr	r1, [r3, #2152]
	cmp	r1, #0
	addne	ip, r0, #26368
	addne	ip, ip, #220
	bne	.L1171
.L1172:
	ldr	r2, [r3, #2156]
	cmp	r2, #0
	beq	.L1207
	add	r0, r0, #26624
	mov	r2, #0
	add	r0, r0, #28
	b	.L1174
.L1173:
	ldr	r1, [r3, #2156]
	cmp	r1, r2
	bls	.L1208
.L1174:
	ldr	r1, [r0, #4]!
	add	r2, r2, #1
	ldrb	ip, [r1, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1173
	ldr	ip, [r1, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	ldreq	ip, [r1, #644]
	streq	ip, [r1, #680]
	ldr	r1, [r3, #2156]
	cmp	r1, r2
	bhi	.L1174
.L1208:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1209:
	ldr	lr, [r1, #676]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L1168
	ldr	lr, [r1, #636]
	ldr	r4, [r6, #2384]
	rsb	r5, r7, lr
	cmp	lr, r4
	strhi	r5, [r1, #640]
	strls	lr, [r1, #640]
	ldr	r1, [ip]
	ldr	lr, [r1, #640]
	str	lr, [r1, #684]
.L1168:
	ldr	r1, [r3, #2152]
	add	r2, r2, #1
	cmp	r1, r2
	bls	.L1172
.L1171:
	ldr	r1, [ip, #4]!
	ldrb	lr, [r1, #2]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L1168
	b	.L1209
.L1207:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1206:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	CalcPicNum, .-CalcPicNum
	.align	2
	.global	IsOutDPB
	.type	IsOutDPB, %function
IsOutDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	beq	.L1215
	add	r3, r0, #24576
	ldr	ip, [r3, #2148]
	cmp	ip, #0
	beq	.L1215
	ldr	r3, [r3, #1952]
	cmp	r3, r1
	beq	.L1217
	add	r0, r0, #26368
	mov	r3, #0
	add	r0, r0, #160
	b	.L1212
.L1213:
	ldr	r2, [r0, #4]!
	cmp	r2, r1
	beq	.L1217
.L1212:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1213
.L1215:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1217:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	IsOutDPB, .-IsOutDPB
	.align	2
	.global	WriteCurrPicYUV
	.type	WriteCurrPicYUV, %function
WriteCurrPicYUV:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r5, r0, #57344
	ldr	r1, .L1221
	mov	r0, #2
	ldrb	r4, [r5, #1739]	@ zero_extendqisi2
	ldr	r3, [r5, #2376]
	mov	r2, r4
	ldr	r3, [r3, #164]
	bl	dprint_vfmw
	ldr	ip, [r5, #2428]
	cmp	r4, #3
	cmpne	r4, #0
	ldr	r3, [r5, #2424]
	addne	r2, r4, #1
	ldr	r1, .L1221+4
	moveq	r2, #1
	str	ip, [sp]
	mov	r0, #22
	bl	dprint_vfmw
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1222:
	.align	2
.L1221:
	.word	.LC39
	.word	.LC40
	UNWIND(.fnend)
	.size	WriteCurrPicYUV, .-WriteCurrPicYUV
	.align	2
	.global	WritePicMsg
	.type	WritePicMsg, %function
WritePicMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r10, r0, #57344
	mov	r4, r0
	ldr	r1, .L1259
	ldr	lr, [r10, #2444]
	add	r5, r4, #61440
	ldr	ip, [r10, #1908]
	mov	r0, #2
	ldrb	r3, [r10, #1739]	@ zero_extendqisi2
	add	r9, r4, #12288
	ldr	r2, [r10, #2392]
	add	r8, r4, #16384
	stmia	sp, {ip, lr}
	bl	dprint_vfmw
	movw	r2, #59696
	mov	r3, #0
	str	r3, [r5, #1060]
	ldrd	r0, [r4, r2]
	add	r3, r4, #61184
	strd	r0, [r3, #40]
	ldr	r3, [r5, #1056]
	str	r3, [r5, #784]
	ldr	r3, [r10, #1892]
	str	r3, [r5, #564]
	ldr	r3, [r10, #1896]
	str	r3, [r5, #568]
	ldr	r3, [r10, #1900]
	str	r3, [r5, #572]
	ldr	r2, [r10, #2392]
	ldrb	r3, [r10, #1739]	@ zero_extendqisi2
	ldr	r1, [r10, #2428]
	str	r2, [r10, #3888]
	strb	r3, [r10, #3872]
	ldr	r2, [r10, #2432]
	ldrb	r3, [r9, #1327]	@ zero_extendqisi2
	str	r1, [r10, #3892]
	str	r2, [r10, #3896]
	str	r3, [r10, #3904]
	ldr	r3, [r8, #460]
	add	r3, r3, #1
	str	r3, [r10, #3900]
	ldrb	r3, [r9, #1326]	@ zero_extendqisi2
	strb	r3, [r10, #3875]
	ldr	r3, [r8, #1272]
	str	r3, [r10, #3908]
	ldrb	r3, [r8, #1212]	@ zero_extendqisi2
	str	r3, [r10, #3912]
	ldr	r3, [r9, #1348]
	strb	r3, [r10, #3873]
	ldrb	r3, [r8, #1216]	@ zero_extendqisi2
	str	r3, [r10, #3916]
	ldr	r2, [r8, #3464]
	ldr	r3, [r10, #2376]
	str	r2, [r10, #3920]
	ldr	r2, [r8, #3452]
	str	r2, [r10, #3924]
	ldr	r2, [r8, #3456]
	str	r2, [r10, #3928]
	ldr	r1, [r3, #632]
	ldr	r0, [r5, #1136]
	bl	FSP_GetLogicFs
	subs	r7, r0, #0
	beq	.L1257
	ldr	r2, [r7, #640]
	ldr	r3, [r7, #644]
	cmp	r2, #0
	beq	.L1227
	cmp	r3, #0
	beq	.L1227
	add	r3, r4, #61440
	add	r1, r4, #61696
	add	r1, r1, #216
	ldr	r0, [r5, #1136]
	mov	r2, r3
	add	r3, r3, #212
	add	r2, r2, #84
	add	r6, r4, #24576
	bl	FSP_GetDecFsAddrTab
	ldr	r3, [r7, #640]
	add	r2, r4, #61696
	add	r1, r4, #61952
	add	r2, r2, #220
	add	r1, r1, #44
	ldr	r3, [r3, #8]
	ldr	r0, [r5, #1136]
	str	r3, [r5, #68]
	bl	FSP_GetPmvAddrTab
	ldr	r3, [r10, #2444]
	str	r3, [r5, #76]
	ldr	r3, [r7, #656]
	str	r3, [r5, #560]
	ldr	r3, [r7, #644]
	ldr	r3, [r3, #16]
	str	r3, [r5, #72]
	ldr	r3, [r7, #644]
	ldr	r3, [r3, #76]
	str	r3, [r5, #80]
	ldr	r3, [r6, #2420]
	cmp	r3, #0
	str	r3, [fp, #-48]
	str	r3, [r5, #776]
	beq	.L1233
	add	lr, r4, #61952
	add	r10, r4, #26624
	add	r1, r4, #26880
	ldr	r7, [fp, #-48]
	mov	ip, r10
	mov	r0, lr
	mov	r2, lr
	add	r10, r10, #176
	add	lr, lr, #68
	add	ip, ip, #240
	add	r0, r0, #132
	add	r1, r1, #48
	add	r2, r2, #196
	mov	r3, #0
	str	r4, [fp, #-52]
.L1232:
	ldr	r4, [r10, #4]!
	add	r3, r3, #1
	cmp	r3, r7
	str	r4, [lr, #4]!
	ldr	r4, [ip, #4]!
	str	r4, [r0, #4]!
	ldr	r4, [r1, #4]!
	str	r4, [r2, #4]!
	bne	.L1232
	ldr	r4, [fp, #-52]
.L1233:
	ldrb	r3, [r9, #1333]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1258
	ldrb	r1, [r8, #1230]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L1234
	add	r0, r4, #61184
	ldr	r9, .L1259+4
	add	r0, r0, #88
.L1235:
	and	r3, r1, #3
	add	r1, r1, #1
	cmp	r1, #24
	ldr	r3, [r9, r3, asl #2]
	bic	r2, r3, #16711680
	ubfx	ip, r3, #8, #8
	bic	r2, r2, #65280
	mov	r3, r3, lsr #8
	orr	r2, r2, ip, asl #16
	and	r3, r3, #65280
	orr	r3, r2, r3
	str	r3, [r0, #4]!
	bne	.L1235
	add	r8, r4, #61184
	mov	r7, #0
	add	r8, r8, #188
.L1236:
	add	r3, r7, #1
	and	r2, r7, #14
	and	r3, r3, #15
	add	r7, r7, #2
	add	r2, r9, r2, lsl #2
	cmp	r7, #32
	add	r3, r9, r3, lsl #2
	ldr	r2, [r2, #16]
	ldr	r1, [r3, #16]
	ubfx	r3, r2, #8, #8
	mov	r0, r2, lsr #24
	uxtb	r10, r2
	uxtb	lr, r1
	mov	ip, r1, lsr #16
	orr	r3, r3, r0, asl #8
	mov	r2, r2, lsr #8
	mov	lr, lr, asl #16
	and	r0, r1, #-16777216
	orr	ip, lr, ip, asl #24
	ubfx	r1, r1, #8, #8
	orr	ip, ip, r10
	and	r2, r2, #65280
	orr	r3, r3, r0
	orr	r2, ip, r2
	orr	r3, r3, r1, asl #16
	stmia	r8, {r2, r3}
	add	r8, r8, #8
	bne	.L1236
.L1241:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L1238
	add	r9, r4, #62208
	add	r7, r4, #26624
	add	r7, r7, #180
	mov	r8, #0
	mov	r4, r9
	add	r9, r9, #144
	add	r4, r4, #208
.L1244:
	ldr	r2, [r7]
	mov	r1, #0
	ldr	r0, [r5, #1136]
	add	r8, r8, #1
	bl	FSP_GetStoreType
	adds	r0, r0, #0
	movne	r0, #1
	str	r0, [r9, #4]!
	ldr	r3, [r7], #4
	str	r3, [r4, #4]!
	ldr	r3, [r6, #2420]
	cmp	r3, r8
	bhi	.L1244
.L1238:
	mov	r0, #0
.L1254:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1258:
	ldrb	r1, [r8, #1230]	@ zero_extendqisi2
.L1234:
	cmp	r1, #1
	add	ip, r4, #61184
	mov	r0, #0
	add	ip, ip, #88
	beq	.L1239
.L1242:
	add	r3, r0, #3888
	add	r0, r0, #1
	add	r3, r3, #2
	cmp	r0, #24
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #8]
	bic	r2, r3, #16711680
	ubfx	r1, r3, #8, #8
	bic	r2, r2, #65280
	mov	r3, r3, lsr #8
	orr	r2, r2, r1, asl #16
	and	r3, r3, #65280
	orr	r3, r2, r3
	str	r3, [ip, #4]!
	bne	.L1242
	add	r9, r4, #61184
	mov	r10, #1
	add	r9, r9, #188
	mov	r8, #0
.L1243:
	mov	r2, r8, lsr #3
	mov	r3, r8, asl #1
	and	r1, r10, #15
	and	r0, r3, #14
	mov	r3, r2, asl #4
	add	r8, r8, #1
	add	r2, r3, r1
	add	r3, r3, r0
	add	r2, r2, #3904
	add	r3, r3, #3904
	add	r2, r2, #10
	add	r3, r3, #10
	cmp	r8, #16
	add	r10, r10, #2
	add	r2, r4, r2, lsl #2
	add	r3, r4, r3, lsl #2
	ldr	r1, [r2, #8]
	ldr	r3, [r3, #8]
	ubfx	r7, r1, #8, #8
	and	r0, r1, #-16777216
	mov	r2, r1, lsr #16
	uxtb	r1, r1
	mov	ip, r3, lsr #24
	uxtb	lr, r3
	mov	r2, r2, asl #24
	mov	ip, ip, asl #8
	orr	r2, r2, r1, asl #16
	orr	ip, ip, r7, asl #16
	mov	r1, r3, lsr #8
	orr	r2, r2, lr
	and	r1, r1, #65280
	ubfx	r3, r3, #8, #8
	orr	r0, ip, r0
	orr	r2, r2, r1
	orr	r3, r0, r3
	stmia	r9, {r2, r3}
	add	r9, r9, #8
	bne	.L1243
	b	.L1241
.L1239:
	add	r3, r0, #4864
	add	r0, r0, #1
	add	r3, r3, #30
	cmp	r0, #24
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #4]
	bic	r2, r3, #16711680
	ubfx	r1, r3, #8, #8
	bic	r2, r2, #65280
	mov	r3, r3, lsr #8
	orr	r2, r2, r1, asl #16
	and	r3, r3, #65280
	orr	r3, r2, r3
	str	r3, [ip, #4]!
	bne	.L1239
	add	r9, r4, #61184
	mov	r10, #1
	add	r9, r9, #188
	mov	r8, #0
.L1240:
	mov	r2, r8, lsr #3
	mov	r3, r8, asl #1
	and	r1, r10, #15
	and	r0, r3, #14
	mov	r3, r2, asl #4
	add	r8, r8, #1
	add	r2, r3, r1
	add	r3, r3, r0
	add	r2, r2, #4864
	add	r3, r3, #4864
	add	r2, r2, #54
	add	r3, r3, #54
	cmp	r8, #16
	add	r10, r10, #2
	add	r2, r4, r2, lsl #2
	add	r3, r4, r3, lsl #2
	ldr	r1, [r2, #4]
	ldr	r3, [r3, #4]
	ubfx	r7, r1, #8, #8
	and	r0, r1, #-16777216
	mov	r2, r1, lsr #16
	uxtb	r1, r1
	mov	ip, r3, lsr #24
	uxtb	lr, r3
	mov	r2, r2, asl #24
	mov	ip, ip, asl #8
	orr	r2, r2, r1, asl #16
	orr	ip, ip, r7, asl #16
	mov	r1, r3, lsr #8
	orr	r2, r2, lr
	and	r1, r1, #65280
	ubfx	r3, r3, #8, #8
	orr	r0, ip, r0
	orr	r2, r2, r1
	orr	r3, r0, r3
	stmia	r9, {r2, r3}
	add	r9, r9, #8
	bne	.L1240
	b	.L1241
.L1227:
	ldr	r1, .L1259+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1254
.L1257:
	ldr	r3, [r10, #2376]
	ldr	r1, .L1259+12
	ldr	r2, [r3, #632]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1254
.L1260:
	.align	2
.L1259:
	.word	.LC41
	.word	.LANCHOR0
	.word	.LC43
	.word	.LC42
	UNWIND(.fnend)
	.size	WritePicMsg, .-WritePicMsg
	.align	2
	.global	UpdatePicQpInf
	.type	UpdatePicQpInf, %function
UpdatePicQpInf:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r2, [r1, #24]
	ldr	r3, [r1, #28]
	cmp	r2, r0
	movlt	r2, r0
	cmp	r3, r0
	str	r2, [r1, #24]
	movge	r3, r0
	str	r3, [r1, #28]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	UpdatePicQpInf, .-UpdatePicQpInf
	.align	2
	.global	WriteSliceMsg
	.type	WriteSliceMsg, %function
WriteSliceMsg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #28)
	sub	sp, sp, #28
	add	r7, r0, #61440
	add	ip, r0, #12288
	movw	r1, #4060
	ldr	r3, [r7, #1060]
	mov	r4, r0
	ldr	r0, [r7, #1056]
	mov	r2, r3
	str	r3, [fp, #-48]
	mul	r1, r1, r2
	add	r3, r3, #1
	str	r3, [r7, #1060]
	ldr	r3, [ip, #1284]
	add	r6, r0, r1
	sub	r3, r3, #1
	cmp	r2, r3
	addcc	r3, r1, #4048
	addcc	r3, r3, #12
	movcs	r3, #0
	addcc	r3, r0, r3
	str	r3, [r6, #4056]
	ldr	r2, [r4, #368]
	ldrb	lr, [r2]	@ zero_extendqisi2
	ldr	r3, [r2, #68]
	cmp	lr, #1
	cmpls	lr, r3
	bcs	.L1265
	add	r5, lr, #3
	add	r5, r6, r5, lsl #2
.L1266:
	mov	r3, lr, asl #5
	sub	r3, r3, lr, asl #2
	add	lr, lr, #1
	add	r2, r2, r3
	ldr	r2, [r2, #24]
	str	r2, [r5, #4]!
	ldr	r2, [r4, #368]
	add	r2, r2, r3
	ldr	r2, [r2, #20]
	str	r2, [r5, #-8]
	ldr	r2, [r4, #368]
	add	r3, r2, r3
	ldr	r3, [r3, #28]
	str	r3, [r5, #8]
	ldr	r2, [r4, #368]
	ldr	r3, [r2, #68]
	cmp	r3, lr
	movhi	r8, #1
	movls	r8, #0
	cmp	lr, #1
	movhi	r8, #0
	cmp	r8, #0
	bne	.L1266
.L1265:
	cmp	r3, #1
	bhi	.L1271
	add	r3, r3, #3
	add	lr, r6, #20
	mov	r2, #0
	add	r3, r6, r3, lsl #2
.L1270:
	str	r2, [r3, #4]!
	cmp	r3, lr
	str	r2, [r3, #-8]
	str	r2, [r3, #8]
	bne	.L1270
.L1271:
	add	r10, r4, #57344
	add	r9, r4, #16384
	add	r5, r4, #20480
	ldrb	r3, [r10, #1745]	@ zero_extendqisi2
	strb	r3, [r6, #1]
	ldr	r3, [r9, #1260]
	ldr	r2, [r5, #1820]
	add	r3, r3, #26
	add	r3, r3, r2
	str	r3, [r6, #32]
	ldr	r2, [r5, #1816]
	str	r2, [r6, #36]
	ldr	r2, [r5, #268]
	str	r2, [r6, #40]
	ldr	r2, [r5, #264]
	str	r2, [r6, #44]
	ldrb	r2, [r5, #208]	@ zero_extendqisi2
	strb	r2, [r0, r1]
	ldr	r2, [r5, #260]
	str	r2, [r6, #48]
	ldrb	r2, [r5, #214]	@ zero_extendqisi2
	strb	r2, [r6, #2]
	ldrb	r2, [ip, #1328]	@ zero_extendqisi2
	strb	r2, [r6, #3]
	ldr	r2, [r5, #252]
	str	r2, [r6, #52]
	ldr	r2, [r5, #256]
	str	r2, [r6, #56]
	ldrb	r2, [r5, #208]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1367
	cmp	r2, #0
	ldreqb	r2, [r9, #1214]	@ zero_extendqisi2
	movne	r2, #0
	strb	r2, [r6, #4]
.L1272:
	ldr	r2, [r9, #1264]
	ldr	r1, [fp, #-48]
	str	r2, [r6, #60]
	cmp	r1, #0
	ldr	r2, [r9, #1268]
	str	r2, [r6, #64]
	ldr	r2, [r5, #1828]
	str	r2, [r6, #68]
	ldr	r2, [r5, #1832]
	str	r2, [r6, #72]
	ldr	r2, [r5, #1824]
	str	r2, [r6, #76]
	ldr	r2, [r10, #2376]
	bne	.L1274
	str	r3, [r2, #700]
	ldr	r3, [r10, #2376]
	ldr	r2, [r6, #32]
	str	r2, [r3, #704]
.L1275:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1313
	ldr	r8, [r5, #252]
	cmp	r8, #0
	beq	.L1316
	add	r3, r4, #24576
	ldr	r3, [r3, #2556]
	ldr	r1, [r3, #32]
	ldrb	r3, [r1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r2, r4, #26880
	movne	r3, #0
	addne	r2, r2, #252
	bne	.L1281
	b	.L1279
.L1283:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #32]
	ldrb	r0, [r1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1279
.L1281:
	add	r3, r3, #1
	cmp	r3, r8
	bne	.L1283
	mov	r3, #0
	str	r3, [fp, #-52]
.L1278:
	ldr	ip, [r5, #256]
	cmp	ip, #0
	beq	.L1317
	add	r3, r4, #24576
	ldr	r3, [r3, #2688]
	ldr	r1, [r3, #32]
	ldrb	r3, [r1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r2, r4, #27136
	movne	r3, #0
	addne	r2, r2, #128
	bne	.L1287
	b	.L1285
.L1289:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #32]
	ldrb	r0, [r1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1285
.L1287:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1289
	mov	r3, #0
	str	r3, [fp, #-56]
.L1284:
	ldrb	ip, [r10, #1739]	@ zero_extendqisi2
	ldr	r3, [r5, #252]
	cmp	ip, #0
	beq	.L1290
	cmp	r3, #0
	beq	.L1305
	add	r10, r4, #26880
	mov	r3, r6
	mov	r8, #0
	str	r6, [fp, #-60]
	str	r4, [fp, #-64]
	mov	r6, r5
	add	r10, r10, #252
	mov	r5, r8
	ldr	r4, [fp, #-52]
	mov	r8, r3
	b	.L1304
.L1302:
	strb	r1, [r8, #1630]
	ldr	r1, [r10]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r8, #1631]
	ldr	r1, [r10]
	ldr	r1, [r1, #12]
	str	r1, [r8, #1656]
.L1303:
	ldr	r1, [r6, #252]
	add	r5, r5, #1
	add	r10, r10, #4
	add	r8, r8, #36
	cmp	r1, r5
	bls	.L1368
.L1304:
	ldr	r1, [r10]
	ldr	r0, [r7, #1136]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #632]
	bl	FSP_GetLogicFs
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #668]
	str	r1, [r8, #1636]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r8, #1625]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r4
	beq	.L1301
	cmp	r0, #0
	ldrne	r1, [r0, #640]
	ldrne	r0, [r1, #8]
.L1301:
	str	r0, [r8, #1640]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #672]
	str	r1, [r8, #1644]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #648]
	str	r1, [r8, #1632]
	ldr	r1, [r10]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r8, #1624]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1, #676]	@ zero_extendqisi2
	strb	r1, [r8, #1626]
	ldr	r1, [r10]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L1302
	strb	r1, [r8, #1628]
	ldr	r1, [r10]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r8, #1629]
	ldr	r1, [r10]
	ldr	r1, [r1, #12]
	str	r1, [r8, #1652]
	b	.L1303
.L1368:
	mov	r5, r6
	ldr	r4, [fp, #-64]
	ldr	r6, [fp, #-60]
.L1305:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1369
.L1293:
	add	r2, r4, #26880
	add	r3, r6, #3920
	add	r0, r6, #4048
	add	r2, r2, #120
	add	r3, r3, #4
	add	r0, r0, #4
.L1310:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L1310
	ldrb	r3, [r9, #1214]	@ zero_extendqisi2
	cmp	r3, #0
	str	r3, [r7, #60]
	ldr	r3, [r9, #1256]
	str	r3, [r7, #64]
	beq	.L1311
	ldrb	r2, [r5, #208]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1312
.L1311:
	cmp	r3, #1
	beq	.L1370
.L1313:
	ldr	r3, [fp, #-48]
	mov	r2, #0
	ldr	r1, [r4, #368]
	add	r3, r3, #15104
	add	r3, r3, #34
	str	r1, [r4, r3, asl #2]
	str	r2, [r4, #368]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1274:
	ldr	r0, [r2, #700]
	ldr	r1, [r2, #704]
	cmp	r0, r3
	movlt	r0, r3
	cmp	r1, r3
	str	r0, [r2, #700]
	movge	r1, r3
	str	r1, [r2, #704]
	b	.L1275
.L1279:
	ldr	r1, [r1, #632]
	ldr	r0, [r7, #1136]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	movweq	r2, #7517
	beq	.L1363
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-52]
	b	.L1278
.L1370:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1313
.L1312:
	ldr	r3, [r5, #272]
	add	r9, r4, #20736
	add	lr, r4, #21504
	add	r7, r4, #20992
	add	r0, r4, #21760
	mov	r8, r9
	str	r3, [r6, #80]
	mov	ip, lr
	ldr	r10, [r5, #276]
	add	r9, r9, #20
	add	r8, r8, #148
	add	r7, r7, #20
	add	lr, lr, #20
	add	ip, ip, #148
	add	r0, r0, #20
	mov	r1, r6
	mov	r3, r6
	mov	r2, #0
	str	r6, [fp, #-52]
	str	r10, [r1, #84]!
.L1314:
	ldr	r10, [r9, #4]!
	add	r2, r2, #1
	add	r3, r3, #4
	str	r10, [r1, #4]!
	ldr	r10, [r8, #4]!
	str	r10, [r3, #212]
	ldr	r10, [r7, #4]!
	str	r10, [r3, #340]
	ldr	r10, [lr, #4]!
	str	r10, [r3, #852]
	ldr	r10, [ip, #4]!
	str	r10, [r3, #980]
	ldr	r10, [r0, #4]!
	str	r10, [r3, #1108]
	ldr	r10, [r5, #264]
	cmp	r10, r2
	bcs	.L1314
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1313
	add	lr, r4, #21248
	add	r1, r4, #22016
	add	r7, r4, #20992
	add	r0, r4, #21760
	ldr	r8, [fp, #-52]
	mov	ip, lr
	mov	r2, r1
	add	r6, r6, #468
	add	r7, r7, #148
	add	lr, lr, #20
	add	ip, ip, #148
	add	r0, r0, #148
	add	r1, r1, #20
	add	r2, r2, #148
	mov	r3, #0
.L1315:
	ldr	r9, [r7, #4]!
	add	r3, r3, #1
	add	r8, r8, #4
	str	r9, [r6, #4]!
	ldr	r9, [lr, #4]!
	str	r9, [r8, #596]
	ldr	r9, [ip, #4]!
	str	r9, [r8, #724]
	ldr	r9, [r0, #4]!
	str	r9, [r8, #1236]
	ldr	r9, [r1, #4]!
	str	r9, [r8, #1364]
	ldr	r9, [r2, #4]!
	str	r9, [r8, #1492]
	ldr	r9, [r5, #268]
	cmp	r9, r3
	bcs	.L1315
	b	.L1313
.L1290:
	cmp	r3, #0
	beq	.L1298
	add	r2, r4, #26880
	str	r4, [fp, #-64]
	add	r2, r2, #252
	ldr	r4, [fp, #-52]
	mov	r8, r6
	str	r6, [fp, #-60]
	mov	r10, r2
	mov	r6, r5
	mov	r5, ip
.L1297:
	ldr	r1, [r10]
	ldr	r0, [r7, #1136]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #632]
	bl	FSP_GetLogicFs
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #668]
	str	r1, [r8, #1636]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r8, #1625]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r4
	beq	.L1296
	cmp	r0, #0
	ldrne	r1, [r0, #640]
	ldrne	r0, [r1, #8]
.L1296:
	str	r0, [r8, #1640]
	mov	r3, #0
	ldr	r1, [r10]
	add	r5, r5, #1
	add	r8, r8, #36
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #672]
	str	r1, [r8, #1608]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #648]
	str	r1, [r8, #1596]
	ldr	r1, [r10]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r8, #1591]
	ldr	r1, [r10]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r8, #1590]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #688]
	str	r1, [r8, #1612]
	ldr	r1, [r10]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #724]
	str	r1, [r8, #1616]
	ldr	r1, [r10], #4
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #760]
	strb	r3, [r8, #1588]
	str	r1, [r8, #1620]
	ldr	r1, [r6, #252]
	cmp	r1, r5
	bhi	.L1297
	mov	r5, r6
	ldr	r4, [fp, #-64]
	ldr	r6, [fp, #-60]
.L1298:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1293
	ldr	r3, [r5, #256]
	cmp	r3, #0
	beq	.L1293
	add	r3, r4, #27136
	str	r4, [fp, #-60]
	add	r3, r3, #128
	ldr	r4, [fp, #-56]
	mov	r8, #0
	mov	r10, r6
	str	r6, [fp, #-52]
	mov	r6, r5
	mov	r5, r8
	mov	r8, r3
.L1300:
	ldr	r1, [r8]
	ldr	r0, [r7, #1136]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #632]
	bl	FSP_GetLogicFs
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #668]
	str	r1, [r10, #2788]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2777]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r4
	beq	.L1299
	cmp	r0, #0
	ldrne	r1, [r0, #640]
	ldrne	r0, [r1, #8]
.L1299:
	str	r0, [r10, #1640]
	mov	r3, #0
	ldr	r1, [r8]
	add	r5, r5, #1
	add	r10, r10, #36
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #672]
	str	r1, [r10, #2760]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #648]
	str	r1, [r10, #2748]
	ldr	r1, [r8]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2743]
	ldr	r1, [r8]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r10, #2742]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #688]
	str	r1, [r10, #2764]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #724]
	str	r1, [r10, #2768]
	ldr	r1, [r8], #4
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #760]
	strb	r3, [r10, #2740]
	str	r1, [r10, #2772]
	ldr	r1, [r6, #256]
	cmp	r1, r5
	bhi	.L1300
.L1362:
	mov	r5, r6
	ldr	r4, [fp, #-60]
	ldr	r6, [fp, #-52]
	b	.L1293
.L1285:
	ldr	r1, [r1, #632]
	ldr	r0, [r7, #1136]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L1371
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-56]
	b	.L1284
.L1367:
	ldr	r2, [r9, #1256]
	strb	r2, [r6, #4]
	b	.L1272
.L1369:
	ldr	r3, [r5, #256]
	cmp	r3, #0
	beq	.L1293
	add	r8, r4, #27136
	mov	r3, #0
	mov	r10, r6
	str	r6, [fp, #-52]
	str	r4, [fp, #-60]
	mov	r6, r5
	add	r8, r8, #128
	ldr	r4, [fp, #-56]
	mov	r5, r3
	b	.L1309
.L1307:
	strb	r1, [r10, #2782]
	ldr	r1, [r8]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2783]
	ldr	r1, [r8]
	ldr	r1, [r1, #12]
	str	r1, [r10, #2808]
.L1308:
	ldr	r1, [r6, #256]
	add	r5, r5, #1
	add	r8, r8, #4
	add	r10, r10, #36
	cmp	r1, r5
	bls	.L1362
.L1309:
	ldr	r1, [r8]
	ldr	r0, [r7, #1136]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #632]
	bl	FSP_GetLogicFs
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #668]
	str	r1, [r10, #2788]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2777]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r4
	beq	.L1306
	cmp	r0, #0
	ldrne	r1, [r0, #640]
	ldrne	r0, [r1, #8]
.L1306:
	str	r0, [r10, #2792]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #672]
	str	r1, [r10, #2796]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #648]
	str	r1, [r10, #2784]
	ldr	r1, [r8]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r10, #2776]
	ldr	r1, [r8]
	ldr	r1, [r1, #32]
	ldrb	r1, [r1, #676]	@ zero_extendqisi2
	strb	r1, [r10, #2778]
	ldr	r1, [r8]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L1307
	strb	r1, [r10, #2780]
	ldr	r1, [r8]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2781]
	ldr	r1, [r8]
	ldr	r1, [r1, #12]
	str	r1, [r10, #2804]
	b	.L1308
.L1371:
	movw	r2, #7538
.L1363:
	ldr	r1, .L1372
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L1316:
	str	r8, [fp, #-52]
	b	.L1278
.L1317:
	str	ip, [fp, #-56]
	b	.L1284
.L1373:
	.align	2
.L1372:
	.word	.LC44
	UNWIND(.fnend)
	.size	WriteSliceMsg, .-WriteSliceMsg
	.align	2
	.global	StopPicNum
	.type	StopPicNum, %function
StopPicNum:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	StopPicNum, .-StopPicNum
	.align	2
	.global	H264_GetPicStreamSize
	.type	H264_GetPicStreamSize, %function
H264_GetPicStreamSize:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r0, r0, #61440
	ldr	r2, [r0, #784]
	cmp	r2, #0
	beq	.L1378
	mov	r0, #0
.L1377:
	ldr	r3, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r1
	add	r3, r3, #7
	cmp	r2, #0
	add	r0, r0, r3, lsr #3
	bne	.L1377
	ldmfd	sp, {fp, sp, pc}
.L1378:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	H264_GetPicStreamSize, .-H264_GetPicStreamSize
	.align	2
	.global	SliceCheck
	.type	SliceCheck, %function
SliceCheck:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, .L1391
	add	r4, r0, #20480
	add	r6, r0, #61440
	mov	r5, r0
	bl	ue_v
	str	r0, [r4, #260]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L1389
	cmp	r0, #262144
	bcs	.L1390
	add	r2, r5, #12288
	ldr	r1, [r2, #1276]
	ldr	r3, [r2, #1280]
	mul	r3, r3, r1
	sub	r3, r3, #1
	cmp	r0, r3
	bhi	.L1389
	mov	r0, r5
	ldr	r1, .L1391+4
	bl	ue_v
	ldr	r1, .L1391+8
	mov	r2, r0
	mov	r5, r0
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L1389
	cmp	r5, #9
	bhi	.L1385
	mov	r1, #1
	movw	r0, #297
	mov	r2, r1, asl r5
	and	r0, r0, r2
	cmp	r0, #0
	bne	.L1386
	ands	r3, r2, #660
	bne	.L1387
	tst	r2, #66
	beq	.L1385
	strb	r1, [r4, #208]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1386:
	strb	r3, [r4, #208]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1385:
	mov	r2, r5
	ldr	r1, .L1391+12
	mov	r0, #1
	bl	dprint_vfmw
.L1389:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1387:
	mov	r3, #2
	strb	r3, [r4, #208]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1390:
	ldr	r1, .L1391+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1392:
	.align	2
.L1391:
	.word	.LC45
	.word	.LC47
	.word	.LC48
	.word	.LC49
	.word	.LC46
	UNWIND(.fnend)
	.size	SliceCheck, .-SliceCheck
	.align	2
	.global	PPSSPSCheck
	.type	PPSSPSCheck, %function
PPSSPSCheck:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #20480
	mov	r4, r0
	add	r4, r4, #12288
	ldr	r1, .L1399
	ldr	r2, [r5, #220]
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r1, [r4, #1304]
	ldr	r2, [r5, #220]
	mov	r3, #2208
	mla	r3, r3, r2, r1
	ldrb	r1, [r3, #19]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1397
	ldr	r2, [r3, #28]
	mov	r3, #4000
	ldr	r1, [r4, #1300]
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1398
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1397:
	ldr	r1, .L1399+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1398:
	ldr	r1, .L1399+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1400:
	.align	2
.L1399:
	.word	.LC50
	.word	.LC51
	.word	.LC52
	UNWIND(.fnend)
	.size	PPSSPSCheck, .-PPSSPSCheck
	.align	2
	.global	PPSSPSCheckTmpId
	.type	PPSSPSCheckTmpId, %function
PPSSPSCheckTmpId:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r2, r1
	mov	r5, r1
	mov	r4, r0
	ldr	r1, .L1409
	mov	r0, #19
	bl	dprint_vfmw
	add	r0, r4, #12288
	mov	r3, #2208
	ldr	r2, [r0, #1304]
	mla	r3, r3, r5, r2
	ldrb	r2, [r3, #19]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1406
	ldr	r1, [r0, #1288]
	ldr	r2, [r3, #28]
	sub	r3, r1, #1
	cmp	r2, r3
	bhi	.L1407
	ldr	r1, [r0, #1300]
	mov	r3, #4000
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1408
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1406:
	mov	r2, r5
	ldr	r1, .L1409+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1407:
	ldr	r1, .L1409+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1408:
	ldr	r1, .L1409+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1410:
	.align	2
.L1409:
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC56
	UNWIND(.fnend)
	.size	PPSSPSCheckTmpId, .-PPSSPSCheckTmpId
	.align	2
	.global	IsNewPic
	.type	IsNewPic, %function
IsNewPic:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	add	lr, r0, #12288
	ldr	ip, [r3, #1860]
	ldr	r1, [lr, #1344]
	ldr	r2, [r3, #1864]
	ldr	r4, [r3, #220]
	ldr	r6, [r3, #1872]
	cmp	r2, r4
	cmpeq	ip, r1
	ldr	r5, [r3, #228]
	ldrb	r4, [r3, #1856]	@ zero_extendqisi2
	ldrb	ip, [r3, #209]	@ zero_extendqisi2
	movne	r1, #1
	moveq	r1, #0
	cmp	r6, r5
	moveq	r2, r1
	orrne	r2, r1, #1
	cmp	r4, ip
	orrne	r2, r2, #1
	cmp	r4, #0
	cmpne	ip, #0
	beq	.L1413
	ldrb	ip, [r3, #1857]	@ zero_extendqisi2
	ldrb	r1, [r3, #210]	@ zero_extendqisi2
	cmp	ip, r1
	orrne	r2, r2, #1
.L1413:
	ldr	r1, [r3, #1868]
	ldr	ip, [r3, #240]
	cmp	r1, ip
	moveq	r1, #0
	beq	.L1414
	cmp	ip, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
.L1414:
	ldrb	ip, [r3, #1858]	@ zero_extendqisi2
	ldrb	r4, [r3, #211]	@ zero_extendqisi2
	cmp	ip, r4
	orrne	r2, r2, #1
	cmp	r4, #5
	cmpeq	ip, #5
	orr	r2, r2, r1
	beq	.L1422
.L1415:
	ldr	r1, [lr, #3508]
	cmp	r1, #0
	bne	.L1416
	add	r0, r0, #16384
	ldr	r1, [r3, #1880]
	ldr	ip, [r3, #1884]
	ldr	lr, [r0, #3424]
	ldr	r0, [r0, #3420]
	cmp	r1, r0
	cmpeq	ip, lr
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
.L1417:
	ldr	r4, [r3, #260]
	mov	r0, #19
	ldr	r1, .L1423
	cmp	r4, #0
	movne	r4, r2
	orreq	r4, r2, #1
	mov	r2, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1416:
	cmp	r1, #1
	bne	.L1417
	add	r0, r0, #16384
	ldr	r1, [r3, #1888]
	ldr	ip, [r3, #1892]
	ldr	lr, [r0, #3472]
	ldr	r0, [r0, #3468]
	cmp	r1, r0
	cmpeq	ip, lr
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
	b	.L1417
.L1422:
	ldr	ip, [r3, #1876]
	ldr	r1, [r3, #248]
	cmp	ip, r1
	orrne	r2, r2, #1
	b	.L1415
.L1424:
	.align	2
.L1423:
	.word	.LC57
	UNWIND(.fnend)
	.size	IsNewPic, .-IsNewPic
	.align	2
	.global	ref_pic_list_reordering
	.type	ref_pic_list_reordering, %function
ref_pic_list_reordering:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #20480
	add	r3, r0, #12288
	mov	lr, #2208
	mov	ip, #4000
	ldr	r2, [r3, #1304]
	add	r8, r0, #16384
	ldr	r1, [r5, #220]
	mov	r4, r0
	ldr	r3, [r3, #1300]
	mov	r0, #0
	mov	r7, #1
	mla	r2, lr, r1, r2
	ldr	r2, [r2, #28]
	strb	r0, [r8, #3512]
	strb	r0, [r8, #3513]
	mla	r3, ip, r2, r3
	ldrb	r1, [r5, #209]	@ zero_extendqisi2
	cmp	r1, r0
	ldr	r3, [r3, #2196]
	addne	r3, r3, #5
	addeq	r3, r3, #4
	mov	r7, r7, asl r3
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	addhi	r6, r4, #61440
	bls	.L1479
.L1478:
	ldr	r3, [r6, #1076]
.L1444:
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1479:
	ldr	r1, .L1484
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r8, #3512]
	cmp	r0, #0
	beq	.L1477
	ldr	r3, [r5, #264]
	cmn	r3, #2
	beq	.L1458
	add	r10, r4, #19712
	add	r6, r4, #61440
	add	r10, r10, #184
	mov	r9, #0
	b	.L1441
.L1436:
	cmp	r0, #2
	beq	.L1480
.L1439:
	ldr	r3, [r5, #264]
	add	r3, r3, #2
	cmp	r3, r9
	bls	.L1481
.L1441:
	ldr	r1, .L1484+4
	mov	r0, r4
	bl	ue_v
	add	r9, r9, #1
	cmp	r0, #3
	str	r0, [r10, #4]!
	ldr	r3, [r6, #1076]
	bhi	.L1432
	cmp	r3, #0
	bne	.L1432
	cmp	r0, #3
	beq	.L1435
	cmp	r0, #1
	bhi	.L1436
	ldr	r1, .L1484+8
	mov	r0, r4
	bl	ue_v
	cmp	r7, r0
	str	r0, [r10, #264]
	ldr	r3, [r6, #1076]
	bls	.L1438
	cmp	r3, #0
	beq	.L1439
.L1438:
	mov	r2, r0
	ldr	r1, .L1484+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1480:
	ldr	r1, .L1484+16
	mov	r0, r4
	bl	ue_v
	str	r0, [r10, #528]
	b	.L1439
.L1477:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	add	r6, r4, #61440
.L1428:
	cmp	r3, #1
	bne	.L1478
	ldr	r1, .L1484+20
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r8, #3513]
	cmp	r0, #0
	beq	.L1478
	ldr	r3, [r5, #268]
	cmn	r3, #2
	beq	.L1478
	add	r9, r4, #19968
	mov	r8, #0
	add	r9, r9, #60
	b	.L1456
.L1455:
	add	r8, r8, #1
	cmp	r8, r3
	bcs	.L1478
.L1456:
	ldr	r1, .L1484+24
	mov	r0, r4
	bl	ue_v
	cmp	r0, #3
	str	r0, [r9, #4]!
	ldr	r3, [r6, #1076]
	bhi	.L1448
	cmp	r3, #0
	bne	.L1448
	cmp	r0, #3
	beq	.L1444
	cmp	r0, #1
	bls	.L1482
	cmp	r0, #2
	beq	.L1483
	ldr	r3, [r5, #268]
	add	r3, r3, #2
	cmp	r3, r8
	bne	.L1455
.L1457:
	ldr	r1, .L1484+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1482:
	ldr	r1, .L1484+32
	mov	r0, r4
	bl	ue_v
	cmp	r7, r0
	str	r0, [r9, #264]
	ldr	r3, [r6, #1076]
	bls	.L1452
	cmp	r3, #0
	bne	.L1452
.L1453:
	ldr	r2, [r5, #268]
	add	r3, r2, #2
	cmp	r3, r8
	bne	.L1455
	add	r2, r2, #4992
	add	r2, r2, #18
	ldr	r2, [r4, r2, asl #2]
	cmp	r2, #3
	beq	.L1455
	b	.L1457
.L1483:
	ldr	r1, .L1484+36
	mov	r0, r4
	bl	ue_v
	str	r0, [r9, #524]
	b	.L1453
.L1435:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	b	.L1428
.L1432:
	mov	r2, r0
	ldr	r1, .L1484+40
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1448:
	mov	r2, r0
	ldr	r1, .L1484+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1481:
	cmp	r9, r3
	bne	.L1435
.L1430:
	add	r3, r3, #4928
	add	r3, r3, #46
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #4]
	cmp	r3, #3
	beq	.L1477
	ldr	r1, .L1484+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1452:
	mov	r2, r0
	ldr	r1, .L1484+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1458:
	mov	r3, #0
	b	.L1430
.L1485:
	.align	2
.L1484:
	.word	.LC58
	.word	.LC59
	.word	.LC61
	.word	.LC62
	.word	.LC63
	.word	.LC65
	.word	.LC66
	.word	.LC71
	.word	.LC68
	.word	.LC70
	.word	.LC60
	.word	.LC67
	.word	.LC64
	.word	.LC69
	UNWIND(.fnend)
	.size	ref_pic_list_reordering, .-ref_pic_list_reordering
	.align	2
	.global	pred_weight_table
	.type	pred_weight_table, %function
pred_weight_table:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r3, r0, #12288
	add	r6, r0, #20480
	mov	r10, #2208
	ldr	r2, [r3, #1304]
	mov	r7, #4000
	ldr	ip, [r6, #220]
	mov	r5, r0
	ldr	r3, [r3, #1300]
	mov	r4, #1
	ldr	r1, .L1516
	mla	r2, r10, ip, r2
	str	r2, [fp, #-48]
	ldr	r2, [r2, #28]
	mla	r7, r7, r2, r3
	bl	ue_v
	str	r0, [r6, #272]
	mov	r8, r4, asl r0
	ldr	r9, [r7, #40]
	cmp	r9, #0
	bne	.L1510
.L1487:
	cmp	r0, #7
	bhi	.L1490
	ldr	r3, [r6, #276]
	cmp	r3, #7
	bhi	.L1490
	add	r3, r5, #61440
	str	r3, [fp, #-52]
	ldr	r2, [r3, #1076]
	cmp	r2, #0
	addeq	r10, r5, #20736
	moveq	r4, r2
	addeq	r10, r10, #152
	beq	.L1498
	b	.L1489
.L1494:
	ldr	r2, [r7, #40]
	cmp	r2, #0
	bne	.L1511
.L1495:
	str	r2, [r10]
	str	r2, [r10, #768]
	str	r2, [r10, #128]
	str	r2, [r10, #896]
.L1497:
	ldr	r2, [r6, #264]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r2, r4
	bcc	.L1512
.L1498:
	ldr	r1, .L1516+4
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1494
	ldr	r1, .L1516+8
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+12
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #640]
	ldr	r2, [r7, #40]
	cmp	r2, #0
	beq	.L1495
.L1511:
	ldr	r1, .L1516+16
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1513
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1497
.L1512:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #44]
	cmp	r3, #1
	beq	.L1499
.L1500:
	ldr	r3, [fp, #-52]
	ldr	r0, [r3, #1076]
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
.L1492:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1513:
	ldr	r1, .L1516+20
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+24
	str	r0, [r10]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+20
	str	r0, [r10, #768]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+24
	str	r0, [r10, #128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #896]
	b	.L1497
.L1510:
	ldr	r1, .L1516+28
	mov	r0, r5
	bl	ue_v
	mov	r3, r0
	ldr	r0, [r6, #272]
	mov	r9, r4, asl r3
	str	r3, [r6, #276]
	b	.L1487
.L1499:
	ldrb	r3, [r6, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1500
	add	r10, r5, #21248
	mov	r4, #0
	add	r10, r10, #24
	b	.L1506
.L1502:
	ldr	r3, [r7, #40]
	cmp	r3, #0
	bne	.L1514
.L1503:
	str	r3, [r10]
	str	r3, [r10, #768]
	str	r3, [r10, #128]
	str	r3, [r10, #896]
.L1505:
	ldr	r3, [r6, #268]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r3, r4
	bcc	.L1500
.L1506:
	ldr	r1, .L1516+32
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1502
	ldr	r1, .L1516+36
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+40
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #640]
	ldr	r3, [r7, #40]
	cmp	r3, #0
	beq	.L1503
.L1514:
	ldr	r1, .L1516+44
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1515
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1505
.L1490:
	add	r5, r5, #61440
	ldr	r2, [r5, #1076]
.L1489:
	ldr	r1, .L1516+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1492
.L1515:
	ldr	r1, .L1516+52
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+56
	str	r0, [r10]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+52
	str	r0, [r10, #768]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1516+56
	str	r0, [r10, #128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #896]
	b	.L1505
.L1517:
	.align	2
.L1516:
	.word	.LC72
	.word	.LC75
	.word	.LC76
	.word	.LC77
	.word	.LC78
	.word	.LC79
	.word	.LC80
	.word	.LC73
	.word	.LC81
	.word	.LC82
	.word	.LC83
	.word	.LC84
	.word	.LC74
	.word	.LC85
	.word	.LC86
	UNWIND(.fnend)
	.size	pred_weight_table, .-pred_weight_table
	.align	2
	.global	DecMMCO
	.type	DecMMCO, %function
DecMMCO:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	mov	r9, r0
	moveq	r6, #100
	beq	.L1530
	add	r8, r0, #24576
	mov	r10, #0
	b	.L1521
.L1552:
	ldr	r1, .L1558
	mov	r0, r9
	bl	ue_v
	cmp	r4, #2
	beq	.L1550
.L1532:
	cmp	r4, #6
	orreq	r5, r5, #1
	cmp	r5, #0
	bne	.L1535
	cmp	r4, #4
	beq	.L1551
.L1536:
	cmp	r4, #6
	bhi	.L1528
.L1533:
	add	r3, r9, #61440
	ldr	r3, [r3, #1076]
	cmp	r3, #0
	bne	.L1528
	cmp	r4, #0
	beq	.L1538
.L1537:
	subs	r6, r6, #1
	beq	.L1539
.L1530:
	ldr	r1, .L1558+4
	mov	r0, r9
	bl	ue_v
	bic	r3, r0, #2
	sub	r5, r0, #3
	clz	r5, r5
	cmp	r3, #1
	mov	r4, r0
	mov	r5, r5, lsr #5
	beq	.L1552
	cmp	r4, #2
	bne	.L1532
.L1550:
	ldr	r1, .L1558+8
	mov	r0, r9
	bl	ue_v
	cmp	r5, #0
	beq	.L1533
.L1535:
	ldr	r1, .L1558+12
	mov	r0, r9
	bl	ue_v
	cmp	r4, #4
	bne	.L1536
.L1551:
	ldr	r1, .L1558+16
	mov	r0, r9
	bl	ue_v
	add	r3, r9, #61440
	ldr	r3, [r3, #1076]
	cmp	r3, #0
	beq	.L1537
.L1528:
	ldr	r1, .L1558+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1557:
	ldr	r1, .L1558
	mov	r0, r9
	bl	ue_v
	ldr	r2, [r4, #8]
	cmp	r2, #2
	str	r0, [r4, #12]
	beq	.L1553
.L1524:
	cmp	r2, #3
	cmpne	r2, #6
	beq	.L1554
.L1525:
	cmp	r2, #4
	beq	.L1555
.L1526:
	cmp	r2, #6
	bhi	.L1528
	add	r3, r9, #61440
	ldr	r3, [r3, #1076]
	cmp	r3, #0
	bne	.L1528
	cmp	r2, #0
	add	r10, r10, #1
	beq	.L1538
	cmp	r10, #100
	beq	.L1556
.L1521:
	ldr	r1, .L1558+4
	mov	r0, r9
	ldr	r5, [r8, #1812]
	bl	ue_v
	mov	r6, r10, asl #2
	mov	r7, r10, asl #4
	add	r4, r6, r7
	movw	r3, #2004
	mla	r4, r3, r5, r4
	add	r4, r9, r4
	add	r4, r4, #22272
	add	r4, r4, #104
	bic	r1, r0, #2
	str	r0, [r4, #8]
	cmp	r1, #1
	mov	r2, r0
	beq	.L1557
	cmp	r2, #2
	bne	.L1524
.L1553:
	ldr	r1, .L1558+8
	mov	r0, r9
	bl	ue_v
	add	r3, r6, r7
	movw	r2, #2004
	mla	r3, r2, r5, r3
	add	r3, r9, r3
	add	r2, r3, #22272
	str	r0, [r2, #120]
	ldr	r2, [r2, #112]
	cmp	r2, #3
	cmpne	r2, #6
	bne	.L1525
.L1554:
	ldr	r1, .L1558+12
	mov	r0, r9
	bl	ue_v
	add	r3, r6, r7
	movw	r2, #2004
	mla	r3, r2, r5, r3
	add	r3, r9, r3
	add	r2, r3, #22272
	str	r0, [r2, #124]
	ldr	r2, [r2, #112]
	cmp	r2, #4
	bne	.L1526
.L1555:
	ldr	r1, .L1558+16
	mov	r0, r9
	bl	ue_v
	add	r6, r6, r7
	movw	r7, #2004
	mla	r5, r7, r5, r6
	add	r5, r9, r5
	add	r3, r5, #22272
	add	r5, r3, #104
	ldr	r2, [r3, #112]
	str	r0, [r3, #128]
	b	.L1526
.L1538:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1556:
	ldr	r1, .L1558+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1539:
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1559:
	.align	2
.L1558:
	.word	.LC89
	.word	.LC88
	.word	.LC90
	.word	.LC91
	.word	.LC92
	.word	.LC93
	.word	.LC87
	UNWIND(.fnend)
	.size	DecMMCO, .-DecMMCO
	.align	2
	.global	dec_ref_pic_marking
	.type	dec_ref_pic_marking, %function
dec_ref_pic_marking:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r3, r0, #20480
	add	r6, r0, #24576
	mov	r5, r0
	ldrb	r8, [r3, #213]	@ zero_extendqisi2
	ldr	r7, [r6, #1812]
	cmp	r8, #0
	beq	.L1561
	clz	r4, r7
	mov	r4, r4, lsr #5
	str	r4, [r6, #1812]
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1579
.L1562:
	ldr	r1, .L1584
	mov	r0, r5
	bl	u_1
	movw	r1, #2004
	mla	r4, r1, r4, r5
	ldr	r1, .L1584+4
	add	r4, r4, #22272
	add	r4, r4, #104
	mov	r3, r0
	uxtb	r2, r0
	strb	r3, [r4, #7]
	mov	r0, #16
	bl	dprint_vfmw
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1564
.L1568:
	mov	r8, #0
.L1576:
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1561:
	movw	r4, #2004
	mla	r4, r4, r7, r0
	add	r4, r4, #22272
	add	r4, r4, #104
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1580
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	mov	r4, r7
	cmp	r3, #5
	bne	.L1562
.L1579:
	ldr	r1, .L1584+8
	mov	r0, r5
	bl	u_1
	movw	r3, #2004
	mla	r4, r3, r4, r5
	ldr	r1, .L1584+12
	mov	r8, #0
	add	r4, r4, #22272
	strb	r0, [r4, #109]
	mov	r0, r5
	bl	u_1
	ldrb	r2, [r4, #109]	@ zero_extendqisi2
	ldr	r1, .L1584+16
	strb	r0, [r4, #110]
	mov	r0, #16
	bl	dprint_vfmw
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1580:
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1581
	ldr	r1, .L1584
	ldrb	r6, [r4, #7]	@ zero_extendqisi2
	bl	u_1
	cmp	r6, r0
	bne	.L1582
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1568
	mov	r1, r8
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	DecMMCO
.L1564:
	mov	r0, r5
	mov	r1, #1
	bl	DecMMCO
	cmn	r0, #1
	mov	r8, r0
	movne	r3, #1
	movne	r8, #0
	strneb	r3, [r4, #4]
	bne	.L1576
	mov	r3, #0
	ldr	r1, .L1584+20
	strb	r3, [r4, #4]
	mov	r0, #1
	str	r7, [r6, #1812]
	bl	dprint_vfmw
	b	.L1576
.L1581:
	ldr	r1, .L1584+8
	ldrb	r6, [r4, #5]	@ zero_extendqisi2
	bl	u_1
	cmp	r6, r0
	bne	.L1583
	mov	r0, r5
	ldr	r1, .L1584+12
	ldrb	r4, [r4, #6]	@ zero_extendqisi2
	bl	u_1
	cmp	r4, r0
	beq	.L1568
	ldr	r1, .L1584+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1576
.L1582:
	add	r5, r5, #61440
	ldr	r1, .L1584+28
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	ldr	ip, [r5, #1080]
	ldr	r3, [r5, #1116]
	mov	r0, #1
	ldr	r2, [r5, #1100]
	ldr	r1, .L1584+32
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L1576
.L1583:
	ldr	r1, .L1584+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1576
.L1585:
	.align	2
.L1584:
	.word	.LC97
	.word	.LC98
	.word	.LC94
	.word	.LC95
	.word	.LC96
	.word	.LC99
	.word	.LC101
	.word	.LC102
	.word	.LC103
	.word	.LC100
	UNWIND(.fnend)
	.size	dec_ref_pic_marking, .-dec_ref_pic_marking
	.align	2
	.global	ProcessSliceHeaderFirstPart
	.type	ProcessSliceHeaderFirstPart, %function
ProcessSliceHeaderFirstPart:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, .L1660
	add	r5, r0, #12288
	mov	r6, r0
	bl	ue_v
	ldr	r3, [r5, #1292]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r10, r0
	bhi	.L1587
	add	r9, r6, #61440
	ldr	r3, [r9, #1076]
	cmp	r3, #0
	bne	.L1587
	mov	r1, r0
	mov	r0, r6
	bl	PPSSPSCheckTmpId
	subs	r8, r0, #0
	bne	.L1651
	add	r4, r6, #20480
	mov	r7, #2208
	mov	r1, #4000
	str	r10, [r4, #220]
	ldr	r2, [r5, #1304]
	ldrb	r0, [r4, #211]	@ zero_extendqisi2
	ldr	r3, [r5, #1300]
	mla	r7, r7, r10, r2
	cmp	r0, #5
	ldr	r2, [r7, #28]
	mla	r5, r1, r2, r3
	beq	.L1591
	ldr	r3, [r5, #3244]
	cmp	r3, #0
	beq	.L1591
.L1592:
	ldr	r1, [r5, #2196]
	mov	r0, r6
	ldr	r2, .L1660+4
	add	r1, r1, #4
	bl	u_v
	str	r0, [r4, #228]
	ldr	r3, [r9, #1076]
	cmp	r3, #0
	bne	.L1649
	ldrb	r3, [r4, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1652
.L1595:
	mov	r3, #0
	strb	r3, [r4, #209]
	strb	r3, [r4, #210]
	ldrb	r3, [r5, #18]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1653
	ldr	r2, [r5, #3248]
	ldr	r1, [r5, #3252]
	add	r3, r2, #1
	mla	r3, r1, r3, r3
.L1619:
	ldrb	r2, [r5, #19]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1602
	strb	r2, [r4, #212]
.L1604:
	ldr	r2, [r4, #260]
	cmp	r2, r3
	bcs	.L1654
.L1605:
	ldrb	r3, [r4, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1655
.L1607:
	add	r10, r6, #16384
	mov	r3, #0
	str	r3, [r10, #3420]
	str	r3, [r10, #3424]
	ldr	r3, [r5, #2200]
	cmp	r3, #0
	beq	.L1656
.L1611:
	mov	r3, #0
	str	r3, [r10, #3468]
	str	r3, [r10, #3472]
	ldr	r3, [r5, #2200]
	cmp	r3, #1
	beq	.L1657
.L1614:
	ldr	r3, [r9, #1076]
	cmp	r3, #0
	bne	.L1649
	ldrb	r3, [r7, #17]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1617
	ldr	r1, .L1660+8
	mov	r0, r6
	bl	ue_v
	cmp	r0, #0
	bne	.L1618
	ldr	r3, [r9, #1076]
	cmp	r3, #0
	bne	.L1618
.L1617:
	mov	r0, r6
	bl	IsNewPic
	uxtb	r0, r0
	strb	r0, [r4, #213]
	cmp	r0, #0
	bne	.L1631
	ldr	r2, [r4, #260]
	ldr	r3, [r4, #1896]
	cmp	r2, r3
	bls	.L1658
.L1631:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1591:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1592
	ldr	r1, .L1660+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1631
.L1602:
	mov	r2, #1
	strb	r2, [r4, #212]
.L1622:
	ldr	r2, [r4, #260]
	cmp	r2, r3, lsr #1
	bcc	.L1605
	ldr	r1, .L1660+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1631
.L1618:
	ldr	r1, .L1660+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L1660+24
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1649
	mov	r3, #0
	ldr	r0, [r9, #1136]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L1649:
	mvn	r8, #0
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1653:
	ldr	r1, .L1660+28
	mov	r0, r6
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #209]
	cmp	r0, #0
	bne	.L1659
	ldr	r2, [r5, #3248]
	ldrb	ip, [r5, #18]	@ zero_extendqisi2
	ldr	r1, [r5, #3252]
	add	r3, r2, #1
	cmp	ip, #0
	mla	r3, r1, r3, r3
	bne	.L1621
	mov	r3, r3, asl #1
	b	.L1619
.L1656:
	ldr	r1, [r5, #2204]
	mov	r0, r6
	ldr	r2, .L1660+32
	add	r1, r1, #4
	bl	u_v
	str	r0, [r10, #3420]
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1611
	ldrb	r3, [r4, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1611
	ldr	r1, .L1660+36
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3424]
	b	.L1611
.L1652:
	cmp	r0, #0
	str	r0, [r4, #224]
	beq	.L1595
	ldr	r1, .L1660+40
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1595
.L1657:
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1614
	ldr	r1, .L1660+44
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3468]
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1614
	ldrb	r3, [r4, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1614
	ldr	r1, .L1660+48
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3472]
	b	.L1614
.L1659:
	ldr	r1, .L1660+52
	mov	r0, r6
	bl	u_1
	strb	r0, [r4, #210]
	ldr	r2, [r5, #3248]
	ldrb	r0, [r5, #18]	@ zero_extendqisi2
	ldr	r1, [r5, #3252]
	add	r3, r2, #1
	cmp	r0, #0
	ldrb	r0, [r4, #209]	@ zero_extendqisi2
	mla	r3, r1, r3, r3
	moveq	r2, #2
	beq	.L1600
.L1621:
	mov	r2, #1
.L1600:
	cmp	r0, #0
	mul	r3, r2, r3
	beq	.L1619
	cmp	r0, #1
	mov	r2, #0
	strb	r2, [r4, #212]
	bne	.L1604
	b	.L1622
.L1655:
	ldr	r1, .L1660+56
	mov	r0, r6
	bl	ue_v
	cmp	r0, #65536
	str	r0, [r4, #248]
	bcs	.L1608
	ldr	r3, [r9, #1076]
	cmp	r3, #0
	beq	.L1607
.L1608:
	ldr	r1, .L1660+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1607
.L1587:
	mov	r2, r10
	ldr	r1, .L1660+64
	mvn	r8, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1651:
	ldr	r1, .L1660+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1631
.L1654:
	ldr	r1, .L1660+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1631
.L1658:
	ldr	r1, .L1660+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1631
.L1661:
	.align	2
.L1660:
	.word	.LC104
	.word	.LC108
	.word	.LC120
	.word	.LC107
	.word	.LC112
	.word	.LC121
	.word	g_event_report
	.word	.LC110
	.word	.LC116
	.word	.LC117
	.word	.LC109
	.word	.LC118
	.word	.LC119
	.word	.LC111
	.word	.LC114
	.word	.LC115
	.word	.LC105
	.word	.LC106
	.word	.LC113
	.word	.LC122
	UNWIND(.fnend)
	.size	ProcessSliceHeaderFirstPart, .-ProcessSliceHeaderFirstPart
	.align	2
	.global	ProcessSliceHeaderSecondPart
	.type	ProcessSliceHeaderSecondPart, %function
ProcessSliceHeaderSecondPart:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #12288
	add	r4, r0, #20480
	mov	r5, r0
	ldr	r3, [r6, #1292]
	ldr	r2, [r4, #220]
	sub	r3, r3, #1
	cmp	r3, r2
	bcc	.L1663
	add	r8, r0, #61440
	ldr	r3, [r8, #1076]
	cmp	r3, #0
	bne	.L1663
	bl	PPSSPSCheck
	cmp	r0, #0
	bne	.L1745
	ldr	r2, [r4, #220]
	mov	r7, #2208
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	ldr	r9, [r6, #1304]
	mul	r7, r7, r2
	cmp	r3, #1
	add	r6, r9, r7
	beq	.L1746
.L1667:
	ldr	r2, [r6, #36]
	cmp	r3, #1
	str	r2, [r4, #264]
	ldr	r2, [r6, #40]
	str	r2, [r4, #268]
	bls	.L1747
	cmp	r3, #2
	beq	.L1694
.L1678:
	mov	r3, #32
	str	r3, [r4, #268]
.L1679:
	mov	r0, r5
	bl	ref_pic_list_reordering
	subs	r10, r0, #0
	bne	.L1748
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1681
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r2, #1
	moveq	r3, r2
	beq	.L1682
.L1681:
	ldr	r3, [r6, #44]
	cmp	r3, #1
	beq	.L1749
.L1698:
	mov	r2, #0
	mov	r3, r2
.L1682:
	strb	r3, [r4, #215]
	mov	r0, #19
	ldr	r1, .L1755
	bl	dprint_vfmw
	ldrb	r2, [r4, #215]	@ zero_extendqisi2
	mov	r3, #0
	str	r3, [r4, #272]
	cmp	r2, r3
	str	r3, [r4, #276]
	bne	.L1750
.L1683:
	ldr	r3, [r4, #240]
	cmp	r3, #0
	bne	.L1751
.L1684:
	mov	r3, #0
	str	r3, [r4, #1816]
	ldrb	r3, [r9, r7]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1685
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L1752
.L1685:
	ldr	r1, .L1755+4
	mov	r0, r5
	bl	se_v
	mov	r3, #0
	str	r3, [r4, #1824]
	str	r3, [r4, #1828]
	str	r3, [r4, #1832]
	str	r0, [r4, #1820]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1753
.L1714:
	mov	r0, r10
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1694:
	mov	r3, #32
	str	r3, [r4, #264]
	b	.L1678
.L1753:
	ldr	r1, .L1755+8
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #1824]
	bhi	.L1688
	ldr	r3, [r8, #1076]
	cmp	r3, #0
	bne	.L1688
	cmp	r0, #1
	beq	.L1714
	ldr	r1, .L1755+12
	mov	r0, r5
	bl	se_v
	add	r3, r0, #6
	str	r0, [r4, #1828]
	cmp	r3, #12
	bhi	.L1691
	ldr	r3, [r8, #1076]
	cmp	r3, #0
	bne	.L1691
	mov	r0, r5
	ldr	r1, .L1755+16
	bl	se_v
	add	r3, r0, #6
	str	r0, [r4, #1832]
	cmp	r3, #12
	bhi	.L1693
	ldr	r3, [r8, #1076]
	cmp	r3, #0
	beq	.L1714
.L1693:
	ldr	r1, .L1755+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1750:
	mov	r0, r5
	bl	pred_weight_table
	cmp	r0, #0
	beq	.L1683
	ldr	r1, .L1755+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1751:
	mov	r0, r5
	bl	dec_ref_pic_marking
	cmp	r0, #0
	beq	.L1684
	ldr	r1, .L1755+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1749:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r2, r3
	bne	.L1698
	b	.L1682
.L1746:
	ldr	r1, .L1755+32
	mov	r0, r5
	bl	u_1
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	strb	r0, [r4, #214]
	b	.L1667
.L1747:
	ldr	r1, .L1755+36
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1669
	ldr	r3, [r4, #264]
.L1670:
	ldrb	r2, [r4, #209]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1671
	cmp	r3, #15
	bhi	.L1672
	ldr	r3, [r4, #268]
	cmp	r3, #15
	bhi	.L1754
.L1673:
	ldr	r3, [r8, #1076]
	cmp	r3, #0
	bne	.L1672
.L1674:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1694
	cmp	r3, #1
	bne	.L1678
	b	.L1679
.L1663:
	ldr	r1, .L1755+40
	mvn	r10, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r10
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1752:
	ldr	r1, .L1755+44
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #1816]
	bhi	.L1686
	ldr	r3, [r8, #1076]
	cmp	r3, #0
	beq	.L1685
.L1686:
	ldr	r1, .L1755+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1671:
	cmp	r3, #31
	bhi	.L1675
	ldr	r3, [r4, #268]
	cmp	r3, #31
	bls	.L1676
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1676
.L1675:
	ldr	r1, .L1755+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1676:
	ldr	r3, [r8, #1076]
	cmp	r3, #0
	beq	.L1674
	b	.L1675
.L1754:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1673
.L1672:
	ldr	r1, .L1755+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1669:
	ldr	r1, .L1755+60
	mov	r0, r5
	bl	ue_v
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	cmp	r2, #1
	mov	r3, r0
	str	r0, [r4, #264]
	bne	.L1670
	ldr	r1, .L1755+64
	mov	r0, r5
	bl	ue_v
	ldr	r3, [r4, #264]
	str	r0, [r4, #268]
	b	.L1670
.L1691:
	ldr	r1, .L1755+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1688:
	ldr	r1, .L1755+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1745:
	ldr	r1, .L1755+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1748:
	ldr	r1, .L1755+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1714
.L1756:
	.align	2
.L1755:
	.word	.LC130
	.word	.LC135
	.word	.LC136
	.word	.LC138
	.word	.LC140
	.word	.LC141
	.word	.LC131
	.word	.LC132
	.word	.LC123
	.word	.LC124
	.word	.LC105
	.word	.LC133
	.word	.LC134
	.word	.LC128
	.word	.LC127
	.word	.LC125
	.word	.LC126
	.word	.LC139
	.word	.LC137
	.word	.LC106
	.word	.LC129
	UNWIND(.fnend)
	.size	ProcessSliceHeaderSecondPart, .-ProcessSliceHeaderSecondPart
	.align	2
	.global	ExitSlice
	.type	ExitSlice, %function
ExitSlice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	add	lr, r0, #12288
	mov	r1, #2208
	mov	r7, #4000
	ldr	r2, [lr, #1304]
	ldr	ip, [r3, #220]
	ldr	r5, [lr, #1300]
	ldrb	r6, [r3, #209]	@ zero_extendqisi2
	mla	r1, r1, ip, r2
	cmp	r6, #0
	ldr	r4, [r3, #260]
	ldr	lr, [r3, #228]
	ldr	r2, [r1, #28]
	str	ip, [r3, #1864]
	ldr	ip, [r3, #240]
	mla	r2, r7, r2, r5
	ldr	r1, [r2, #36]
	str	r4, [r3, #1896]
	str	lr, [r3, #1872]
	str	r1, [r3, #1860]
	ldrneb	r1, [r3, #210]	@ zero_extendqisi2
	strb	r6, [r3, #1856]
	str	ip, [r3, #1868]
	strneb	r1, [r3, #1857]
	ldrb	r1, [r3, #211]	@ zero_extendqisi2
	cmp	r1, #5
	strb	r1, [r3, #1858]
	ldreq	r1, [r3, #248]
	streq	r1, [r3, #1876]
	ldr	r1, [r2, #2200]
	cmp	r1, #0
	bne	.L1760
	add	r1, r0, #16384
	ldr	ip, [r1, #3420]
	str	ip, [r3, #1880]
	ldr	r1, [r1, #3424]
	str	r1, [r3, #1884]
	ldr	r1, [r2, #2200]
.L1760:
	cmp	r1, #1
	addeq	r2, r0, #16384
	add	r0, r0, #61440
	ldreq	r1, [r2, #3468]
	streq	r1, [r3, #1888]
	ldreq	r2, [r2, #3472]
	streq	r2, [r3, #1892]
	ldr	r3, [r0, #1092]
	add	r3, r3, #1
	str	r3, [r0, #1092]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	ExitSlice, .-ExitSlice
	.align	2
	.global	PicTypeStatistic
	.type	PicTypeStatistic, %function
PicTypeStatistic:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #20480
	ldrb	r3, [r2, #208]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1767
	bcc	.L1768
	cmp	r3, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r2, #211]	@ zero_extendqisi2
	add	r0, r0, #57344
	cmp	r3, #5
	beq	.L1777
.L1770:
	mov	r3, #0
	str	r3, [r0, #2416]
	ldmfd	sp, {fp, sp, pc}
.L1768:
	add	r0, r0, #57344
	ldr	r3, [r0, #2412]
	cmp	r3, #2
	movne	r3, #1
	strne	r3, [r0, #2412]
	b	.L1770
.L1767:
	add	r0, r0, #57344
	mov	r2, #2
	mov	r3, #0
	str	r2, [r0, #2412]
	str	r3, [r0, #2416]
	ldmfd	sp, {fp, sp, pc}
.L1777:
	mov	r3, #1
	str	r3, [r0, #2416]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	PicTypeStatistic, .-PicTypeStatistic
	.align	2
	.global	CalcStreamBits
	.type	CalcStreamBits, %function
CalcStreamBits:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r3, r0, #16384
	mov	r4, r0
	ldrb	r3, [r3, #1212]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1779
	ldr	r2, [r0, #368]
	ldr	r3, [r2, #64]
	add	r3, r3, #7
	bic	r3, r3, #7
	str	r3, [r2, #64]
	ldr	r2, [r0, #368]
	ldr	r3, [r2, #72]
	sub	r3, r3, #1
	str	r3, [r2, #72]
.L1779:
	ldr	r3, [r4, #368]
	ldrb	r6, [r3]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L1790
	mov	r1, #0
	mov	r0, r3
	mov	r5, r1
.L1781:
	add	r1, r1, #1
	ldr	ip, [r0, #12]
	cmp	r1, r6
	add	r0, r0, #28
	add	r5, r5, ip
	bne	.L1781
.L1780:
	mov	r7, r6, asl #5
	ldr	r0, [r3, #72]
	sub	r7, r7, r6, asl #2
	ldr	r2, [r3, #64]
	add	r1, r3, r7
	mov	r3, r5
	str	r0, [sp, #4]
	mov	r0, #7
	ldr	ip, [r1, #12]
	ldr	r1, .L1801
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #368]
	mov	r2, r6
	mov	r0, #7
	ldr	r1, .L1801+4
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	ip, [r3, #64]
	mov	lr, r6, asl #5
	and	ip, ip, r0
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	str	ip, [r3, #24]
	ldr	r3, [r4, #368]
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	r8, [r3, #64]
	mov	lr, r6, asl #5
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	ldr	ip, [r3, #16]
	add	ip, ip, r8, lsr #3
	rsb	ip, r5, ip
	str	ip, [r3, #28]
	ldr	ip, [r4, #368]
	ldrb	r6, [ip]	@ zero_extendqisi2
	ldr	lr, [ip, #64]
	mov	r3, r6, asl #5
	sub	r3, r3, r6, asl #2
	add	ip, ip, r3
	ldr	r3, [ip, #12]
	add	r3, r5, r3
	rsb	r3, lr, r3, lsl #3
	str	r3, [ip, #20]
	ldr	ip, [r4, #368]
	ldrb	lr, [ip]	@ zero_extendqisi2
	mov	r3, lr, asl #5
	sub	r3, r3, lr, asl #2
	add	r3, ip, r3
	ldr	ip, [r3, #28]
	ldr	lr, [r3, #24]
	and	ip, ip, #3
	add	ip, lr, ip, lsl #3
	str	ip, [r3, #24]
	ldr	ip, [r4, #368]
	ldrb	lr, [ip]	@ zero_extendqisi2
	mov	r3, lr, asl #5
	sub	r3, r3, lr, asl #2
	add	r3, ip, r3
	ldr	ip, [r3, #28]
	bic	ip, ip, #3
	str	ip, [r3, #28]
	ldr	ip, [r4, #368]
	add	r7, ip, r7
	ldrb	r5, [ip]	@ zero_extendqisi2
	ldr	r3, [r7, #24]
	mov	lr, r5, asl #5
	sub	lr, lr, r5, asl #2
	add	ip, ip, lr
	ldr	ip, [ip, #28]
	str	ip, [sp, #4]
	ldr	ip, [r7, #20]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	ip, [r4, #368]
	ldr	r2, [ip, #68]
	sub	r3, r2, #1
	cmp	r3, #1
	bhi	.L1800
	ldrb	r6, [ip]	@ zero_extendqisi2
	add	r1, r6, #1
	cmp	r2, r1
	movhi	r2, #1
	movls	r2, #0
	cmp	r1, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L1783
	add	r6, r6, #2
	mov	r5, #0
.L1784:
	sub	r3, r6, #1
	ldr	r1, .L1801+8
	mov	r0, #7
	mov	lr, r3, asl #5
	mov	r2, r3
	sub	r3, lr, r3, asl #2
	add	ip, ip, r3
	str	r5, [ip, #24]
	ldr	ip, [r4, #368]
	add	ip, ip, r3
	ldr	lr, [ip, #16]
	str	lr, [ip, #28]
	ldr	ip, [r4, #368]
	add	ip, ip, r3
	ldr	lr, [ip, #12]
	mov	lr, lr, asl #3
	str	lr, [ip, #20]
	ldr	ip, [r4, #368]
	add	ip, ip, r3
	ldr	lr, [ip, #28]
	ldr	r7, [ip, #24]
	and	lr, lr, #3
	add	lr, r7, lr, lsl #3
	str	lr, [ip, #24]
	ldr	ip, [r4, #368]
	add	ip, ip, r3
	ldr	lr, [ip, #28]
	bic	lr, lr, #3
	str	lr, [ip, #28]
	ldr	ip, [r4, #368]
	add	ip, ip, r3
	ldr	r3, [ip, #24]
	ldr	ip, [ip, #20]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	ip, [r4, #368]
	ldr	r3, [ip, #68]
	cmp	r6, #1
	cmpls	r6, r3
	add	r6, r6, #1
	movcc	r2, #1
	movcs	r2, #0
	cmp	r2, #0
	bne	.L1784
	sub	r3, r3, #1
.L1783:
	mov	r2, r3, asl #5
	ldr	r1, [ip, #72]
	sub	r3, r2, r3, asl #2
	add	ip, ip, r3
	ldr	r3, [ip, #20]
	rsb	r3, r1, r3
	str	r3, [ip, #20]
	ldr	ip, [r4, #368]
	ldrb	r1, [ip]	@ zero_extendqisi2
	ldr	r3, [ip, #68]
	cmp	r1, r3
	bcs	.L1778
	sxth	r3, r1
	add	r4, r4, #57344
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, ip, r3
.L1788:
	ldr	r2, [r3, #28]
	add	r1, r1, #1
	ldr	r0, [r4, #2440]
	add	r3, r3, #28
	cmp	r2, r0
	strcc	r2, [r4, #2440]
	ldr	r2, [ip, #68]
	cmp	r2, r1
	bhi	.L1788
.L1778:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1800:
	ldr	r1, .L1801+12
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	dprint_vfmw
.L1790:
	mov	r5, r6
	b	.L1780
.L1802:
	.align	2
.L1801:
	.word	.LC142
	.word	.LC143
	.word	.LC145
	.word	.LC144
	UNWIND(.fnend)
	.size	CalcStreamBits, .-CalcStreamBits
	.align	2
	.global	Scaling_List
	.type	Scaling_List, %function
Scaling_List:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	r7, r3, #0
	str	r0, [fp, #-52]
	mov	r6, r2
	ble	.L1803
	mov	r0, #8
	mov	r4, #0
	mov	r2, r7
	mov	r9, r6
	ldr	r8, .L1814
	mov	r10, #255
	mov	r7, r1
	mov	r6, r0
	mov	r5, r4
	b	.L1808
.L1807:
	mov	lr, r4, lsr #2
	and	r3, r4, #3
	str	r6, [r7, r4, asl #2]
	uxtb	r1, r6
	mov	r4, r3, asl #3
	ldr	r3, [r9, lr, asl #2]
	cmp	r5, r2
	bic	r3, r3, r10, asl r4
	orr	r1, r3, r1, asl r4
	str	r1, [r9, lr, asl #2]
	beq	.L1803
.L1808:
	cmp	r2, #16
	ldreq	r3, .L1814+4
	ldrneb	r4, [r5, r8]	@ zero_extendqisi2
	ldreqb	r4, [r5, r3]	@ zero_extendqisi2
	cmp	r0, #0
	add	r5, r5, #1
	beq	.L1807
	ldr	r1, .L1814+8
	ldr	r0, [fp, #-52]
	str	r2, [fp, #-48]
	bl	se_v
	ldr	r2, [fp, #4]
	add	r0, r6, r0
	ands	r0, r0, #255
	moveq	r3, #1
	movne	r3, #0
	cmp	r3, #0
	moveq	r6, r0
	cmp	r4, #0
	movne	r3, #0
	andeq	r3, r3, #1
	str	r3, [r2]
	ldr	r2, [fp, #-48]
	b	.L1807
.L1803:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1815:
	.align	2
.L1814:
	.word	.LANCHOR1+16
	.word	.LANCHOR1
	.word	.LC146
	UNWIND(.fnend)
	.size	Scaling_List, .-Scaling_List
	.align	2
	.global	ProcessPPS
	.type	ProcessPPS, %function
ProcessPPS:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #52)
	sub	sp, sp, #52
	mov	r4, r1
	ldr	r1, .L1874
	mov	r5, r0
	bl	u_1
	add	r6, r5, #61440
	strb	r0, [r4]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L1865
	ldr	r1, .L1874+4
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #1]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L1865
	ldr	r1, .L1874+8
	mov	r0, r5
	bl	ue_v
	cmp	r0, #0
	str	r0, [r4, #32]
	bne	.L1820
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	beq	.L1821
.L1820:
	ldr	r1, .L1874+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L1874+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1865
	mov	r3, #0
	ldr	r0, [r6, #1136]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L1818:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1828:
	ldr	r1, .L1874+20
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1874+24
	str	r0, [r4, #60]
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #18]
	cmp	r0, #1
	beq	.L1829
.L1847:
	mov	r0, r5
	ldr	r1, .L1874+28
	bl	se_v
	add	r3, r0, #12
	str	r0, [r4, #56]
	cmp	r3, #24
	bhi	.L1830
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	beq	.L1848
.L1830:
	ldr	r1, .L1874+32
	mov	r0, #1
	bl	dprint_vfmw
.L1865:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1821:
	ldr	r1, .L1874+36
	mov	r0, r5
	bl	ue_v
	cmp	r0, #31
	str	r0, [r4, #36]
	bhi	.L1822
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L1822
	ldr	r1, .L1874+40
	mov	r0, r5
	bl	ue_v
	cmp	r0, #31
	str	r0, [r4, #40]
	bhi	.L1824
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L1824
	ldr	r1, .L1874+44
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #2]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L1865
	ldr	r2, .L1874+48
	mov	r1, #2
	mov	r0, r5
	bl	u_v
	cmp	r0, #2
	str	r0, [r4, #44]
	bhi	.L1866
	ldr	r1, .L1874+52
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1874+56
	str	r0, [r4, #48]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1874+60
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1874+64
	str	r0, [r4, #52]
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1874+68
	strb	r0, [r4, #3]
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1874+72
	strb	r0, [r4, #4]
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #17]
	cmp	r0, #0
	bne	.L1867
.L1827:
	ldr	r3, [r4, #52]
	add	r0, r5, #13504
	mov	r7, #0
	add	r0, r0, #24
	str	r7, [r4, #60]
	str	r3, [r4, #56]
	strb	r7, [r4, #18]
	bl	BsResidBits
	ldr	r3, [r5, #368]
	ldr	r3, [r3, #72]
	add	r3, r3, #3
	cmp	r0, r3
	bcs	.L1828
.L1848:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1867:
	ldr	r1, .L1874+76
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1827
.L1822:
	ldr	r1, .L1874+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1818
.L1824:
	ldr	r1, .L1874+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1818
.L1829:
	add	r3, r4, #4
	str	r6, [fp, #-80]
	sub	r9, fp, #72
	add	r8, r4, #1984
	add	r10, r4, #64
	mov	r6, r3
	b	.L1838
.L1871:
	ldr	lr, [lr, #1300]
	ldr	r3, [r4, #28]
	mla	r3, ip, r3, lr
	ldrb	ip, [r3, #25]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L1868
	ldr	r3, .L1874+88
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L1874+92
	ldr	r3, [r3, #52]
	blx	r3
.L1833:
	add	r7, r7, #1
	add	r8, r8, #16
	cmp	r7, #6
	add	r10, r10, #64
	beq	.L1869
.L1838:
	ldr	r1, .L1874+96
	mov	r0, r5
	bl	u_1
	clz	r1, r7
	add	lr, r5, #12288
	mov	ip, #4000
	mov	r2, #16
	mov	r1, r1, lsr #5
	cmp	r0, #1
	mov	r3, r0
	mov	r0, r8
	str	r3, [r9, #4]!
	strb	r3, [r6, #1]!
	beq	.L1870
	cmp	r7, #0
	cmpne	r7, #3
	beq	.L1871
	ldr	r3, .L1874+92
	mov	r2, #16
	sub	r1, r8, #16
	mov	r0, r8
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1833
.L1869:
	ldr	r6, [fp, #-80]
	add	ip, r4, #448
	add	r3, r5, #12288
	sub	r8, fp, #76
	add	r9, r4, #2080
	add	r10, r4, #11
	str	r5, [fp, #-80]
	mov	r7, #0
	str	r6, [fp, #-84]
	mov	r5, r3
	mov	r6, r4
	mov	r4, ip
.L1846:
	ldr	r2, [r6, #60]
	mov	r3, #0
	cmp	r2, #1
	strne	r3, [r8]
	strneb	r3, [r10]
	beq	.L1872
.L1841:
	ldr	r1, [r6, #28]
	mov	lr, #4000
	ldr	r3, [r5, #1300]
	mov	r0, r9
	mov	r2, #64
	mla	r3, lr, r1, r3
	ldrb	r1, [r3, #25]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1873
	ldr	r1, .L1874+100
	cmp	r7, #0
	ldr	r3, .L1874+92
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L1842:
	add	r7, r7, #1
	add	r9, r9, #64
	cmp	r7, #2
	add	r4, r4, #256
	add	r8, r8, #4
	add	r10, r10, #1
	bne	.L1846
	mov	r4, r6
	ldr	r5, [fp, #-80]
	ldr	r6, [fp, #-84]
	b	.L1847
.L1870:
	mov	r3, r2
	str	r9, [sp]
	mov	r2, r8
	mov	r1, r10
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r9]
	cmp	r3, #1
	bne	.L1833
	ldr	r1, .L1874+88
	mov	r2, #16
	ldr	r3, .L1874+92
	cmp	r7, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r8
	blx	r3
	b	.L1833
.L1868:
	add	r1, r3, r7, lsl #4
	ldr	r3, .L1874+92
	add	r1, r1, #1968
	mov	r2, #16
	add	r1, r1, #4
	mov	r0, r8
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1833
.L1866:
	ldr	r1, .L1874+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1818
.L1872:
	ldr	r1, .L1874+108
	ldr	r0, [fp, #-80]
	bl	u_1
	mov	r2, r9
	mov	r1, r4
	mov	r3, #64
	mov	lr, r0
	cmp	lr, #1
	ldr	r0, [fp, #-80]
	str	lr, [r8]
	strb	lr, [r10]
	bne	.L1841
	str	r8, [sp]
	bl	Scaling_List
	ldr	r3, [r8]
	cmp	r3, #1
	bne	.L1842
	ldr	r1, .L1874+100
	mov	r2, #64
	ldr	r3, .L1874+92
	cmp	r7, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r9
	blx	r3
	b	.L1842
.L1873:
	add	r1, r3, r7, lsl #6
	ldr	r3, .L1874+92
	add	r1, r1, #2064
	add	r1, r1, #4
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1842
.L1875:
	.align	2
.L1874:
	.word	.LC147
	.word	.LC148
	.word	.LC149
	.word	.LC150
	.word	g_event_report
	.word	.LC165
	.word	.LC166
	.word	.LC167
	.word	.LC170
	.word	.LC151
	.word	.LC153
	.word	.LC155
	.word	.LC156
	.word	.LC158
	.word	.LC159
	.word	.LC160
	.word	.LC161
	.word	.LC162
	.word	.LC163
	.word	.LC164
	.word	.LC152
	.word	.LC154
	.word	.LANCHOR0+80
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC168
	.word	.LANCHOR0+112
	.word	.LC157
	.word	.LC169
	UNWIND(.fnend)
	.size	ProcessPPS, .-ProcessPPS
	.align	2
	.global	PPSEqual
	.type	PPSEqual, %function
PPSEqual:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	subs	r6, r0, #0
	beq	.L1899
	cmp	r1, #0
	beq	.L1900
	ldr	r2, [r6, #24]
	ldr	ip, [r6, #28]
	ldr	r0, [r1, #28]
	ldr	r4, [r1, #24]
	ldrb	lr, [r6]	@ zero_extendqisi2
	cmp	ip, r0
	cmpeq	r2, r4
	ldrb	r3, [r1]	@ zero_extendqisi2
	ldrb	r4, [r6, #1]	@ zero_extendqisi2
	ldrb	ip, [r1, #1]	@ zero_extendqisi2
	moveq	r2, #1
	movne	r2, #0
	cmp	lr, r3
	movne	r0, #0
	andeq	r0, r2, #1
	ldr	lr, [r6, #36]
	cmp	r4, ip
	movne	r3, #0
	andeq	r3, r0, #1
	ldr	r2, [r1, #36]
	ldr	r4, [r6, #40]
	ldr	r0, [r1, #40]
	cmp	lr, r2
	movne	ip, #0
	andeq	ip, r3, #1
	ldrb	lr, [r6, #2]	@ zero_extendqisi2
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r4, r0
	movne	r2, #0
	andeq	r2, ip, #1
	ldr	r4, [r6, #44]
	ldr	ip, [r1, #44]
	cmp	lr, r3
	movne	r0, #0
	andeq	r0, r2, #1
	ldr	lr, [r6, #48]
	ldr	r2, [r1, #48]
	cmp	r4, ip
	movne	r3, #0
	andeq	r3, r0, #1
	ldr	r4, [r6, #52]
	ldr	r0, [r1, #52]
	cmp	lr, r2
	movne	ip, #0
	andeq	ip, r3, #1
	ldrb	r5, [r6, #3]	@ zero_extendqisi2
	ldrb	lr, [r1, #3]	@ zero_extendqisi2
	cmp	r4, r0
	movne	r2, #0
	andeq	r2, ip, #1
	ldrb	r3, [r6, #4]	@ zero_extendqisi2
	ldrb	r4, [r1, #4]	@ zero_extendqisi2
	cmp	r5, lr
	movne	r0, #0
	andeq	r0, r2, #1
	ldr	ip, [r6, #60]
	ldr	lr, [r1, #60]
	cmp	r3, r4
	movne	r3, #0
	andeq	r3, r0, #1
	ldrb	r2, [r1, #18]	@ zero_extendqisi2
	ldrb	r0, [r6, #18]	@ zero_extendqisi2
	cmp	ip, lr
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r0
	movne	r2, #0
	andeq	r2, r3, #1
	cmp	r0, #0
	beq	.L1880
	mov	ip, ip, asl #1
	adds	r3, ip, #6
	str	r3, [fp, #-48]
	beq	.L1880
	add	r0, r1, #4
	sub	r8, r1, #1088
	add	r3, r6, #4
	sub	r8, r8, #4
	add	r9, r1, #64
	mov	r5, r0
	mov	r10, r6
	mov	r7, r6
	str	r6, [fp, #-52]
	mov	r0, #0
	mov	r6, r3
	str	r1, [fp, #-56]
.L1885:
	ldrb	r3, [r6, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r3
	cmp	ip, r1
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, #0
	beq	.L1898
	cmp	r0, #5
	bhi	.L1882
	add	r4, r10, #64
	add	r10, r10, #128
	mov	r1, r9
	mov	r3, r4
.L1883:
	ldr	lr, [r3], #4
	ldr	ip, [r1], #4
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, r10
	bne	.L1883
.L1881:
	ldr	r3, [fp, #-48]
	add	r0, r0, #1
	add	r7, r7, #256
	add	r8, r8, #256
	cmp	r0, r3
	mov	r10, r4
	add	r9, r9, #64
	bne	.L1885
	ldr	r6, [fp, #-52]
	ldr	r1, [fp, #-56]
.L1880:
	ldr	r0, [r6, #56]
	ldr	r3, [r1, #56]
	cmp	r0, r3
	movne	r0, #0
	andeq	r0, r2, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L1878:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1882:
	sub	r3, r7, #1088
	sub	r4, r7, #836
	sub	r3, r3, #4
	mov	r1, r8
.L1884:
	ldr	lr, [r3, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, r4
	bne	.L1884
.L1898:
	add	r4, r10, #64
	b	.L1881
.L1900:
	mov	r0, r1
	movw	r3, #9727
	ldr	r2, .L1901
	ldr	r1, .L1901+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1878
.L1899:
	movw	r3, #9726
	ldr	r2, .L1901
	ldr	r1, .L1901+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1878
.L1902:
	.align	2
.L1901:
	.word	.LC20
	.word	.LC21
	UNWIND(.fnend)
	.size	PPSEqual, .-PPSEqual
	.align	2
	.global	H264_DecPPS
	.type	H264_DecPPS, %function
H264_DecPPS:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r1, .L1920
	add	r4, r0, #12288
	mov	r5, r0
	bl	ue_v
	ldr	r3, [r4, #1292]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r6, r0
	bls	.L1904
	ldr	r1, .L1920+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #1292]
	sub	r2, r3, #1
	cmp	r6, r2
	bls	.L1912
	ldr	r2, .L1920+8
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L1912
	add	r5, r5, #61440
	str	r3, [fp, #-40]
	str	r6, [fp, #-44]
	mov	r3, #8
	ldr	r0, [r5, #1136]
	sub	r2, fp, #44
	mov	r1, #110
	blx	ip
	mvn	r0, #0
.L1905:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1904:
	ldr	r1, .L1920+12
	mov	r0, r5
	bl	ue_v
	ldr	r3, [r4, #1288]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	ip, r0
	bhi	.L1916
	ldr	r2, [r4, #1300]
	mov	r3, #4000
	mla	r3, r3, r0, r2
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1917
	mov	r9, #2208
	ldr	r3, [r4, #1304]
	mul	r7, r9, r6
	add	r3, r3, r7
	ldrb	r8, [r3, #19]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L1908
	ldr	r8, .L1920+16
	mov	r0, r5
	mov	r1, r8
	str	r6, [r8, #24]
	str	ip, [r8, #28]
	bl	ProcessPPS
	subs	r5, r0, #0
	bne	.L1918
	ldr	r1, [r4, #1304]
	mov	r0, r8
	add	r1, r1, r7
	bl	PPSEqual
	cmp	r0, #0
	beq	.L1905
	ldr	ip, .L1920+20
	mov	r3, #1
	ldr	r0, [r4, #1304]
	mov	r2, r9
	mov	r1, r8
	strb	r3, [r8, #20]
	add	r0, r0, r7
	ldr	ip, [ip, #56]
	strb	r3, [r8, #19]
	blx	ip
	mov	r0, r5
	b	.L1905
.L1908:
	str	r6, [r3, #24]
	mov	r0, r5
	ldr	r3, [r4, #1304]
	add	r3, r3, r7
	str	ip, [r3, #28]
	ldr	r1, [r4, #1304]
	add	r1, r1, r7
	bl	ProcessPPS
	cmp	r0, #0
	bne	.L1919
	ldr	r3, [r4, #1304]
	mov	r2, #1
	add	r3, r3, r7
	strb	r2, [r3, #20]
	ldr	r3, [r4, #1304]
	add	r7, r3, r7
	strb	r2, [r7, #19]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1912:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1916:
	ldr	r1, .L1920+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1905
.L1918:
	ldr	r1, .L1920+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1905
.L1917:
	ldr	r1, .L1920+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1905
.L1919:
	ldr	r1, .L1920+28
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #1304]
	mov	r2, #1
	mvn	r0, #0
	add	r3, r3, r7
	strb	r2, [r3, #20]
	ldr	r3, [r4, #1304]
	add	r7, r3, r7
	strb	r8, [r7, #19]
	b	.L1905
.L1921:
	.align	2
.L1920:
	.word	.LC171
	.word	.LC172
	.word	g_event_report
	.word	.LC173
	.word	.LANCHOR2
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC174
	.word	.LC176
	.word	.LC175
	UNWIND(.fnend)
	.size	H264_DecPPS, .-H264_DecPPS
	.align	2
	.global	vui_parameters
	.type	vui_parameters, %function
vui_parameters:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r5, r1
	ldr	r1, .L1971
	mov	r4, r0
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	streqb	r0, [r5, #15]
	bne	.L1965
.L1924:
	ldr	r1, .L1971+4
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #1]
	cmp	r0, #0
	bne	.L1966
.L1925:
	ldr	r1, .L1971+8
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #3]
	cmp	r0, #0
	moveq	r3, #5
	streq	r3, [r5, #28]
	bne	.L1967
.L1927:
	ldr	r1, .L1971+12
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #6]
	cmp	r0, #0
	bne	.L1968
.L1928:
	ldr	r1, .L1971+16
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #8]
	cmp	r0, #0
	bne	.L1969
.L1930:
	ldr	r1, .L1971+20
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #7]
	cmp	r0, #0
	beq	.L1932
	ldr	r1, .L1971+24
	mov	r0, r4
	bl	ue_v
	ldr	r2, .L1971+28
	mov	r1, #4
	str	r0, [r5, #120]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+32
	mov	r1, #4
	strb	r0, [r5, #116]
	mov	r0, r4
	bl	u_v
	ldr	r3, [r5, #120]
	cmp	r3, #31
	strb	r0, [r5, #117]
	bhi	.L1935
	add	r9, r4, #61440
	ldr	r6, [r9, #1076]
	cmp	r6, #0
	bne	.L1935
	add	r7, r5, #120
	add	r8, r5, #83
.L1936:
	ldr	r1, .L1971+36
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #120]
	cmp	r3, r6
	bcs	.L1936
	ldr	r2, .L1971+48
	mov	r1, #5
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+52
	mov	r1, #5
	str	r0, [r5, #380]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+56
	mov	r1, #5
	str	r0, [r5, #384]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+60
	mov	r1, #5
	str	r0, [r5, #388]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #392]
.L1932:
	ldr	r1, .L1971+64
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #10]
	cmp	r0, #0
	bne	.L1970
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	add	r9, r4, #61440
	cmp	r3, #0
	bne	.L1941
.L1940:
	ldr	r1, .L1971+68
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1971+72
	strb	r0, [r5, #11]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #12]
	cmp	r0, #0
	bne	.L1942
	ldr	r0, [r9, #1076]
.L1943:
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1970:
	ldr	r1, .L1971+24
	mov	r0, r4
	bl	ue_v
	ldr	r2, .L1971+28
	mov	r1, #4
	str	r0, [r5, #432]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+32
	mov	r1, #4
	strb	r0, [r5, #428]
	mov	r0, r4
	bl	u_v
	ldr	r3, [r5, #432]
	cmp	r3, #31
	strb	r0, [r5, #429]
	bhi	.L1935
	add	r9, r4, #61440
	ldr	r6, [r9, #1076]
	cmp	r6, #0
	bne	.L1935
	add	r8, r5, #392
	add	r7, r5, #432
	add	r8, r8, #3
.L1938:
	ldr	r1, .L1971+36
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #432]
	cmp	r3, r6
	bcs	.L1938
	ldr	r2, .L1971+48
	mov	r1, #5
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+52
	mov	r1, #5
	str	r0, [r5, #692]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+56
	mov	r1, #5
	str	r0, [r5, #696]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+60
	mov	r1, #5
	str	r0, [r5, #700]
	mov	r0, r4
	bl	u_v
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r5, #704]
	bne	.L1941
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1940
.L1941:
	ldr	r1, .L1971+76
	mov	r0, r4
	bl	u_1
	strb	r0, [r5, #14]
	b	.L1940
.L1969:
	ldr	r2, .L1971+80
	mov	r1, #32
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+84
	mov	r1, #32
	str	r0, [r5, #52]
	mov	r0, r4
	bl	u_v
	ldr	r1, .L1971+88
	str	r0, [r5, #56]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #9]
	cmp	r0, #1
	bne	.L1930
	ldr	r1, [r5, #52]
	cmp	r1, #0
	beq	.L1930
	ldr	r0, [r5, #56]
	bl	__aeabi_uidiv
	add	r3, r4, #57344
	mov	r0, r0, lsr #1
	str	r0, [r3, #3844]
	b	.L1930
.L1968:
	ldr	r1, .L1971+92
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+96
	str	r0, [r5, #44]
	mov	r0, r4
	bl	ue_v
	str	r0, [r5, #48]
	b	.L1928
.L1967:
	ldr	r2, .L1971+100
	mov	r1, #3
	mov	r0, r4
	bl	u_v
	ldr	r1, .L1971+104
	str	r0, [r5, #28]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1971+108
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #5]
	cmp	r0, #0
	beq	.L1927
	ldr	r2, .L1971+112
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+116
	mov	r1, #8
	str	r0, [r5, #32]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+120
	mov	r1, #8
	str	r0, [r5, #36]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #40]
	b	.L1927
.L1966:
	ldr	r1, .L1971+124
	mov	r0, r4
	bl	u_1
	strb	r0, [r5, #2]
	b	.L1925
.L1965:
	ldr	r2, .L1971+128
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	uxtb	r0, r0
	strb	r0, [r5, #15]
	cmp	r0, #255
	bne	.L1924
	ldr	r2, .L1971+132
	mov	r1, #16
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1971+136
	mov	r1, #16
	str	r0, [r5, #16]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #20]
	b	.L1924
.L1942:
	ldr	r1, .L1971+140
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1971+144
	strb	r0, [r5, #13]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+148
	str	r0, [r5, #60]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+152
	str	r0, [r5, #64]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+156
	str	r0, [r5, #72]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+160
	str	r0, [r5, #68]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1971+164
	str	r0, [r5, #76]
	mov	r0, r4
	bl	ue_v
	str	r0, [r5, #80]
	ldr	r0, [r9, #1076]
	cmp	r0, #0
	beq	.L1943
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1935:
	ldr	r1, .L1971+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1972:
	.align	2
.L1971:
	.word	.LC177
	.word	.LC181
	.word	.LC183
	.word	.LC190
	.word	.LC193
	.word	.LC197
	.word	.LC198
	.word	.LC199
	.word	.LC200
	.word	.LC202
	.word	.LC203
	.word	.LC204
	.word	.LC205
	.word	.LC206
	.word	.LC207
	.word	.LC208
	.word	.LC209
	.word	.LC211
	.word	.LC212
	.word	.LC210
	.word	.LC194
	.word	.LC195
	.word	.LC196
	.word	.LC191
	.word	.LC192
	.word	.LC184
	.word	.LC185
	.word	.LC186
	.word	.LC187
	.word	.LC188
	.word	.LC189
	.word	.LC182
	.word	.LC178
	.word	.LC179
	.word	.LC180
	.word	.LC213
	.word	.LC214
	.word	.LC215
	.word	.LC216
	.word	.LC217
	.word	.LC218
	.word	.LC219
	.word	.LC201
	UNWIND(.fnend)
	.size	vui_parameters, .-vui_parameters
	.align	2
	.global	SPSEqual
	.type	SPSEqual, %function
SPSEqual:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	r5, r0, #0
	beq	.L2005
	cmp	r1, #0
	beq	.L2006
	ldrb	lr, [r1]	@ zero_extendqisi2
	ldrb	r3, [r5]	@ zero_extendqisi2
	ldr	ip, [r5, #28]
	ldr	r2, [r1, #28]
	ldrb	r0, [r5, #1]	@ zero_extendqisi2
	cmp	r3, lr
	cmpeq	ip, r2
	ldrb	r6, [r1, #1]	@ zero_extendqisi2
	ldrb	r2, [r5, #2]	@ zero_extendqisi2
	ldrb	r4, [r1, #2]	@ zero_extendqisi2
	moveq	lr, #1
	movne	lr, #0
	ldr	r3, [r5, #32]
	cmp	r0, r6
	movne	ip, #0
	andeq	ip, lr, #1
	ldr	r6, [r1, #32]
	cmp	r2, r4
	movne	r0, #0
	andeq	r0, ip, #1
	ldr	lr, [r1, #36]
	ldr	r4, [r5, #36]
	cmp	r3, r6
	movne	r2, #0
	andeq	r2, r0, #1
	ldrb	ip, [r1, #25]	@ zero_extendqisi2
	ldrb	r0, [r5, #25]	@ zero_extendqisi2
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r2, #1
	cmp	ip, r0
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r0, #0
	beq	.L1977
	add	r0, r1, #3
	add	r2, r5, #3
	sub	r9, r1, #1104
	add	r7, r1, #48
	mov	ip, r0
	mov	r8, r5
	mov	r6, r5
	str	r5, [fp, #-48]
	mov	r0, #0
	mov	r4, r2
	mov	r5, ip
	str	r1, [fp, #-52]
.L1982:
	ldrb	r2, [r4, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r2
	cmp	ip, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, #0
	beq	.L1978
	cmp	r0, #5
	bhi	.L1979
	add	r2, r6, #48
	add	r10, r6, #112
	mov	r1, r7
.L1980:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L1980
.L1978:
	add	r0, r0, #1
	add	r8, r8, #256
	cmp	r0, #8
	add	r9, r9, #256
	add	r6, r6, #64
	add	r7, r7, #64
	bne	.L1982
	ldr	r5, [fp, #-48]
	ldr	r1, [fp, #-52]
.L1977:
	ldr	r2, [r1, #2196]
	ldr	ip, [r5, #2200]
	ldr	r0, [r1, #2200]
	ldr	lr, [r5, #2196]
	cmp	ip, r0
	cmpeq	lr, r2
	moveq	r2, #1
	movne	r2, #0
	cmp	ip, #0
	and	r3, r3, r2
	beq	.L1984
	cmp	ip, #1
	bne	.L1983
	ldr	r0, [r1, #2208]
	ldr	ip, [r5, #2208]
	ldrb	r2, [r5, #16]	@ zero_extendqisi2
	ldrb	r6, [r1, #16]	@ zero_extendqisi2
	ldr	r4, [r5, #2212]
	cmp	ip, r0
	cmpeq	r2, r6
	ldr	lr, [r1, #2212]
	ldr	r6, [r5, #2216]
	ldr	ip, [r1, #2216]
	moveq	r0, #1
	movne	r0, #0
	cmp	r4, lr
	movne	r2, #0
	andeq	r2, r0, #1
	cmp	r6, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r6, #0
	and	r3, r3, r2
	beq	.L1983
	add	ip, r5, #2208
	add	r0, r1, #2208
	add	ip, ip, #8
	add	r0, r0, #8
	mov	r2, #0
.L1986:
	ldr	r4, [ip, #4]!
	add	r2, r2, #1
	ldr	lr, [r0, #4]!
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r6
	bne	.L1986
.L1983:
	ldrb	ip, [r1, #17]	@ zero_extendqisi2
	ldrb	lr, [r5, #17]	@ zero_extendqisi2
	ldr	r0, [r5, #3244]
	ldr	r4, [r1, #3244]
	ldr	r2, [r5, #3248]
	cmp	lr, ip
	cmpeq	r0, r4
	ldr	r7, [r1, #3248]
	ldr	r6, [r5, #3252]
	ldr	r4, [r1, #3252]
	moveq	ip, #1
	movne	ip, #0
	ldrb	lr, [r1, #18]	@ zero_extendqisi2
	cmp	r2, r7
	movne	r0, #0
	andeq	r0, ip, #1
	ldrb	ip, [r5, #18]	@ zero_extendqisi2
	cmp	r6, r4
	movne	r2, #0
	andeq	r2, r0, #1
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	ip, #0
	and	r3, r3, r2
	bne	.L1987
	ldrb	r0, [r5, #19]	@ zero_extendqisi2
	ldrb	r2, [r1, #19]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
.L1987:
	ldrb	r2, [r1, #20]	@ zero_extendqisi2
	ldrb	ip, [r5, #21]	@ zero_extendqisi2
	ldrb	r0, [r1, #21]	@ zero_extendqisi2
	ldrb	lr, [r5, #20]	@ zero_extendqisi2
	cmp	r0, ip
	cmpeq	lr, r2
	moveq	r2, #1
	movne	r2, #0
	cmp	ip, #0
	and	r3, r3, r2
	bne	.L2007
.L1988:
	ldrb	r0, [r5, #22]	@ zero_extendqisi2
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L1975:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2007:
	ldr	ip, [r1, #3260]
	ldr	lr, [r5, #3260]
	ldr	r0, [r5, #3256]
	ldr	r4, [r1, #3256]
	ldr	r2, [r1, #3264]
	cmp	lr, ip
	cmpeq	r0, r4
	ldr	r6, [r5, #3264]
	ldr	r4, [r5, #3268]
	ldr	lr, [r1, #3268]
	moveq	ip, #1
	movne	ip, #0
	cmp	r6, r2
	movne	r0, #0
	andeq	r0, ip, #1
	cmp	r4, lr
	movne	r2, #0
	andeq	r2, r0, #1
	and	r3, r3, r2
	b	.L1988
.L1979:
	sub	r2, r8, #1104
	sub	r10, r8, #848
	mov	r1, r9
.L1981:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L1981
	b	.L1978
.L1984:
	ldr	r0, [r5, #2204]
	ldr	r2, [r1, #2204]
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
	b	.L1983
.L2006:
	mov	r0, r1
	movw	r3, #9960
	ldr	r2, .L2008
	ldr	r1, .L2008+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1975
.L2005:
	movw	r3, #9959
	ldr	r2, .L2008
	ldr	r1, .L2008+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1975
.L2009:
	.align	2
.L2008:
	.word	.LC20
	.word	.LC21
	UNWIND(.fnend)
	.size	SPSEqual, .-SPSEqual
	.global	__aeabi_idiv
	.align	2
	.global	H264GetDar
	.type	H264GetDar, %function
H264GetDar:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	r4, r0, #0
	mov	lr, r1
	beq	.L2033
	subs	r1, r4, #255
	movne	r1, #1
	cmp	r4, #16
	movle	r0, #0
	andgt	r0, r1, #1
	cmp	r0, #0
	bne	.L2022
	cmp	r2, #0
	cmpne	lr, #0
	moveq	ip, #1
	movne	ip, #0
	cmp	r4, #255
	movne	ip, #0
	andeq	ip, ip, #1
	cmp	ip, #0
	bne	.L2023
	cmp	r4, #1
	moveq	r2, ip
	moveq	r4, #5
	beq	.L2011
	cmp	r1, #0
	str	r3, [fp, #-24]
	ldrne	r0, .L2034
	addne	r4, r0, r4, lsl #3
	ldrne	r2, [r4, #84]
	ldrne	lr, [r4, #80]
	mov	r1, r2
	mov	r0, lr, asl #10
	bl	__aeabi_idiv
	ldr	r3, [fp, #-24]
	ldr	r1, [fp, #4]
	mov	r4, r0
	mov	r0, r3, asl #10
	bl	__aeabi_idiv
	movw	r3, #2405
	movw	r1, #2262
	mul	r0, r0, r4
	add	r2, r0, #1020
	cmp	r0, #0
	add	r2, r2, #3
	movlt	r0, r2
	mov	r2, r0, asr #10
	cmp	r2, r3
	rsble	r3, r2, #2400
	subgt	r3, r2, #2400
	addle	r3, r3, #6
	subgt	r3, r3, #6
	cmp	r2, r1
	bgt	.L2015
	rsb	r1, r2, #2256
	add	r1, r1, #7
	cmp	r3, r1
	bge	.L2016
.L2026:
	mov	r4, #4
	b	.L2011
.L2022:
	mov	r4, #0
.L2033:
	mov	r2, r4
.L2011:
	mov	r3, r4
	ldr	r1, .L2034+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L2015:
	sub	r1, r2, #2256
	sub	r1, r1, #7
	cmp	r3, r1
	blt	.L2026
.L2016:
	movw	r3, #1819
	cmp	r2, r3
	bgt	.L2017
	rsb	r3, r2, #1808
	add	r3, r3, #12
	cmp	r1, r3
	blt	.L2028
.L2018:
	movw	r1, #1364
	cmp	r2, r1
	rsble	r4, r2, #1360
	subgt	r4, r2, #1360
	addle	r4, r4, #5
	subgt	r4, r4, #5
	cmp	r3, r4
	movlt	r4, #2
	movge	r4, #1
	b	.L2011
.L2023:
	mov	r4, r0
	mov	r2, r0
	b	.L2011
.L2017:
	sub	r3, r2, #1808
	sub	r3, r3, #12
	cmp	r1, r3
	bge	.L2018
.L2028:
	mov	r4, #3
	b	.L2011
.L2035:
	.align	2
.L2034:
	.word	.LANCHOR1
	.word	.LC220
	UNWIND(.fnend)
	.size	H264GetDar, .-H264GetDar
	.align	2
	.global	ProcessSPS
	.type	ProcessSPS, %function
ProcessSPS:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #52)
	sub	sp, sp, #52
	subs	r4, r1, #0
	mov	r5, r0
	beq	.L2150
	ldr	r2, [r4, #28]
	cmp	r2, #100
	cmpne	r2, #110
	moveq	r3, #1
	movne	r3, #0
	cmp	r2, #122
	orreq	r3, r3, #1
	cmp	r2, #144
	orreq	r3, r3, #1
	cmp	r2, #83
	orreq	r3, r3, #1
	cmp	r2, #86
	orreq	r3, r3, #1
	cmp	r3, #0
	streqb	r3, [r4, #25]
	moveq	r3, #1
	streq	r3, [r4, #40]
	bne	.L2151
.L2051:
	ldr	r1, .L2167
	mov	r0, r5
	bl	ue_v
	cmp	r0, #12
	str	r0, [r4, #2196]
	bhi	.L2064
	add	r6, r5, #61440
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2064
	ldr	r1, .L2167+4
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #2200]
	bhi	.L2066
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2066
	cmp	r0, #0
	beq	.L2152
	cmp	r0, #1
	beq	.L2153
.L2072:
	ldr	r1, .L2167+8
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #3244]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2167+12
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #17]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2167+16
	mov	r0, r5
	bl	ue_v
	movw	r8, #509
	sub	r3, r0, #1
	str	r0, [r4, #3248]
	cmp	r3, r8
	bhi	.L2077
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2077
	ldr	r1, .L2167+20
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #3252]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2167+24
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #18]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	cmp	r0, #0
	bne	.L2154
	ldr	r1, .L2167+28
	mov	r0, r5
	bl	u_1
	ldr	r7, [r4, #3252]
	add	r7, r7, #1
	cmp	r7, #255
	strb	r0, [r4, #19]
	bhi	.L2109
	ldr	r3, [r4, #3248]
	add	r3, r3, #1
	cmp	r7, #1
	cmphi	r3, #3
	movhi	r9, #2
	bhi	.L2081
.L2109:
	ldr	r1, .L2167+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2151:
	ldr	r1, .L2167+36
	bl	ue_v
	cmp	r0, #1
	str	r0, [r4, #40]
	bhi	.L2040
	add	r6, r5, #61440
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2040
	ldr	r1, .L2167+40
	mov	r0, r5
	bl	ue_v
	uxth	r0, r0
	add	r2, r0, #8
	str	r2, [r4, #44]
	cmp	r2, #8
	beq	.L2155
.L2042:
	ldr	r1, .L2167+44
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2167+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2043
	ldr	r0, [r4, #44]
	mov	r2, #0
	mov	r3, #8
	str	r2, [fp, #-68]
	mov	r1, #119
	sub	r2, fp, #68
	str	r0, [fp, #-64]
	ldr	r0, [r6, #1136]
	blx	ip
.L2043:
	ldr	r1, .L2167+52
	mov	r0, r5
	bl	ue_v
	uxth	r0, r0
	add	r2, r0, #8
	str	r2, [r4, #48]
	cmp	r2, #8
	beq	.L2156
.L2045:
	ldr	r1, .L2167+56
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2167+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2046
	ldr	r0, [r4, #48]
	mov	r2, #0
	mov	r3, #8
	str	r2, [fp, #-68]
	mov	r1, #119
	sub	r2, fp, #68
	str	r0, [fp, #-64]
	ldr	r0, [r6, #1136]
	blx	ip
.L2046:
	ldr	r1, .L2167+60
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L2048
	ldr	r6, [r6, #1076]
	cmp	r6, #0
	bne	.L2048
	ldr	r1, .L2167+64
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #25]
	cmp	r0, #1
	bne	.L2051
	add	r10, r4, #1968
	sub	r7, fp, #72
	add	r10, r10, #4
	add	r9, r4, #3
	add	r8, r4, #52
	b	.L2057
.L2159:
	ldr	r3, .L2167+68
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2167+72
	ldr	r3, [r3, #52]
	blx	r3
.L2053:
	add	r6, r6, #1
	add	r10, r10, #16
	cmp	r6, #6
	add	r8, r8, #64
	beq	.L2157
.L2057:
	ldr	r1, .L2167+76
	mov	r0, r5
	bl	u_1
	clz	r1, r6
	mov	r2, #16
	mov	r1, r1, lsr #5
	cmp	r0, #1
	mov	r3, r0
	mov	r0, r10
	str	r3, [r7, #4]!
	strb	r3, [r9, #1]!
	beq	.L2158
	cmp	r6, #0
	cmpne	r6, #3
	beq	.L2159
	ldr	r3, .L2167+72
	mov	r2, #16
	sub	r1, r10, #16
	mov	r0, r10
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2053
.L2156:
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	beq	.L2046
	b	.L2045
.L2155:
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	beq	.L2043
	b	.L2042
.L2108:
	cmp	r1, r10
	cmpcs	r0, r7
	bcs	.L2107
	ldr	r3, .L2167+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2148
	strh	r1, [fp, #-72]	@ movhi
	mov	r3, #8
	strh	r0, [fp, #-70]	@ movhi
	sub	r2, fp, #76
	strh	r10, [fp, #-76]	@ movhi
	mov	r1, #107
	strh	r7, [fp, #-74]	@ movhi
	ldr	r0, [r6, #1136]
	blx	ip
.L2148:
	mvn	r0, #0
.L2141:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2154:
	ldr	r7, [r4, #3252]
	strb	r3, [r4, #19]
	sub	r3, r7, #1
	cmp	r3, r8
	bhi	.L2109
	ldr	r3, [r4, #3248]
	mov	r9, #1
	add	r7, r7, #1
	add	r3, r3, r9
.L2081:
	mul	r7, r7, r9
	mov	r10, r3, asl #4
	mov	r7, r7, asl #4
	mul	r1, r10, r7
	add	r1, r1, r1, lsr #1
	cmp	r1, #100663296
	bhi	.L2160
	ldr	r2, [r4, #32]
	sub	r3, r2, #10
	cmp	r3, #31
	ldrls	pc, [pc, r3, asl #2]
	b	.L2083
.L2085:
	.word	.L2084
	.word	.L2086
	.word	.L2116
	.word	.L2116
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2116
	.word	.L2088
	.word	.L2090
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2090
	.word	.L2091
	.word	.L2092
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2083
	.word	.L2094
	.word	.L2094
.L2152:
	ldr	r1, .L2167+80
	mov	r0, r5
	bl	ue_v
	cmp	r0, #12
	str	r0, [r4, #2204]
	bhi	.L2069
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	beq	.L2072
.L2069:
	ldr	r1, .L2167+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2116:
	mov	r0, #60416
	movt	r0, 13
.L2087:
	bl	__aeabi_uidiv
	ldr	r1, .L2167+88
	cmp	r0, #16
	movcc	r8, r0
	movcs	r8, #16
	mov	r2, r8
	mov	r0, #21
	bl	dprint_vfmw
	ldr	r2, [r4, #3244]
	cmp	r2, r8
	bhi	.L2095
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	beq	.L2096
.L2095:
	mov	r3, r8
	ldr	r1, .L2167+92
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #3244]
	cmp	r3, #16
	bhi	.L2145
	cmp	r8, r3
	movcc	r8, r3
.L2145:
	str	r8, [r4, #3244]
.L2096:
	ldr	r1, .L2167+96
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #20]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2167+100
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #21]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	cmp	r0, #0
	bne	.L2161
.L2098:
	mov	r2, r10, lsr #1
	mov	r3, r7, lsr #1
	str	r10, [r4, #3276]
	str	r7, [r4, #3280]
	str	r2, [r4, #3284]
	str	r3, [r4, #3288]
.L2100:
	ldr	r1, .L2167+104
	mov	r0, r5
	bl	u_1
	uxtb	r3, r0
	strb	r3, [r4, #22]
	ldr	r2, [r6, #1076]
	cmp	r2, #0
	bne	.L2148
	cmp	r3, #0
	mov	r2, #2
	str	r2, [r4, #3332]
	bne	.L2162
	mov	r0, r3
	strb	r3, [r4, #3307]
	mov	r3, #5
	str	r3, [r4, #3320]
.L2103:
	ldr	r2, [r4, #3312]
	mov	r3, r10
	ldr	r1, [r4, #3308]
	str	r7, [sp]
	bl	H264GetDar
	ldr	r3, [r4, #3244]
	cmp	r3, #0
	str	r0, [r4, #3316]
	beq	.L2106
	cmp	r8, r3
	movcs	r8, r3
.L2106:
	cmp	r8, #16
	add	r3, r5, #12288
	movcs	r8, #16
	str	r8, [r3, #1296]
	ldr	r3, [r5]
	ldr	r2, [r3, #28]
	cmp	r2, #25
	beq	.L2163
.L2107:
	add	r8, r8, #1
	mov	r0, #0
	str	r8, [r4, #3272]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2094:
	mov	r0, #12582912
	b	.L2087
.L2090:
	mov	r0, #30208
	movt	r0, 47
	b	.L2087
.L2157:
	add	r6, r4, #2064
	sub	r7, fp, #88
	add	r6, r6, #4
	add	r9, r4, #9
	add	r8, r4, #436
	mov	r10, #0
.L2062:
	ldr	r1, .L2167+76
	mov	r0, r5
	bl	u_1
	mov	r2, #64
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r6
	str	r3, [r7, #4]!
	strb	r3, [r9, #1]!
	beq	.L2164
	ldr	r1, .L2167+108
	cmp	r10, #0
	ldr	r3, .L2167+72
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2059:
	add	r10, r10, #1
	add	r6, r6, #64
	cmp	r10, #2
	add	r8, r8, #256
	bne	.L2062
	b	.L2051
.L2158:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r10
	mov	r1, r8
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2053
	ldr	r1, .L2167+68
	mov	r2, #16
	ldr	r3, .L2167+72
	cmp	r6, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r10
	blx	r3
	b	.L2053
.L2040:
	ldr	r1, .L2167+112
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2064:
	ldr	r1, .L2167+116
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2161:
	ldr	r1, .L2167+120
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2167+124
	str	r0, [r4, #3256]
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2167+128
	str	r0, [r4, #3260]
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2167+132
	str	r0, [r4, #3264]
	mov	r0, r5
	bl	ue_v
	ldr	r2, [r4, #3264]
	ldr	r1, [r4, #3256]
	mov	ip, r9, asl #1
	ldr	r3, [r4, #3260]
	mov	r2, r2, asl #1
	mov	r1, r1, asl #1
	mul	r9, r9, r2
	rsb	r2, r1, r10
	sub	r3, r2, r3, asl #1
	rsb	r2, r9, r7
	str	r0, [r4, #3268]
	mls	r0, r0, ip, r2
	cmp	r3, #0
	cmpgt	r0, #0
	ble	.L2098
	add	r1, r1, r3, lsr #1
	add	r9, r9, r0, lsr #1
	str	r1, [r4, #3284]
	str	r9, [r4, #3288]
	str	r3, [r4, #3276]
	str	r0, [r4, #3280]
	b	.L2100
.L2162:
	add	r1, r4, #3280
	mov	r0, r5
	add	r1, r1, #12
	bl	vui_parameters
	cmp	r0, #0
	bne	.L2147
	ldrb	r3, [r4, #3304]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2147
	ldr	r2, [r4, #3372]
	clz	r3, r2
	mov	r3, r3, lsr #5
	cmp	r8, r2
	orrcc	r3, r3, #1
	cmp	r3, #0
	bne	.L2165
	ldr	r3, [r4, #3244]
	ldrb	r0, [r4, #3307]	@ zero_extendqisi2
	cmp	r2, r3
	movcs	r8, r2
	movcc	r8, r3
	b	.L2103
.L2165:
	mov	r3, r8
	ldr	r1, .L2167+136
	mov	r0, #1
	bl	dprint_vfmw
.L2147:
	ldrb	r0, [r4, #3307]	@ zero_extendqisi2
	b	.L2103
.L2163:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L2107
	ldr	r2, [r3, #692]
	ldr	r1, [r3, #668]
	cmp	r8, r2
	ldr	r0, [r3, #672]
	bls	.L2108
	ldr	r3, .L2167+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2148
	str	r2, [fp, #-72]
	mov	r3, #8
	str	r8, [fp, #-76]
	sub	r2, fp, #76
	ldr	r0, [r6, #1136]
	mov	r1, #106
	blx	ip
	mvn	r0, #0
	b	.L2141
.L2048:
	ldr	r1, .L2167+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2153:
	ldr	r1, .L2167+144
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #16]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2167+148
	mov	r0, r5
	bl	se_v
	str	r0, [r4, #2208]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2167+152
	mov	r0, r5
	bl	se_v
	str	r0, [r4, #2212]
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2167+156
	mov	r0, r5
	bl	ue_v
	cmp	r0, #255
	str	r0, [r4, #2216]
	bhi	.L2074
	ldr	r7, [r6, #1076]
	cmp	r7, #0
	bne	.L2074
	cmp	r0, #0
	addne	r8, r4, #2208
	addne	r8, r8, #8
	bne	.L2076
	b	.L2072
.L2166:
	ldr	r3, [r4, #2216]
	cmp	r3, r7
	bls	.L2072
.L2076:
	ldr	r1, .L2167+160
	mov	r0, r5
	bl	se_v
	add	r7, r7, #1
	str	r0, [r8, #4]!
	ldr	r3, [r6, #1076]
	cmp	r3, #0
	beq	.L2166
	b	.L2148
.L2077:
	mov	r2, r0
	ldr	r1, .L2167+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2091:
	mov	r0, #30720
	movt	r0, 105
	b	.L2087
.L2088:
	mov	r0, #55296
	movt	r0, 27
	b	.L2087
.L2092:
	mov	r0, #7864320
	b	.L2087
.L2086:
	mov	r0, #17920
	movt	r0, 5
	b	.L2087
.L2084:
	mov	r0, #20992
	movt	r0, 2
	b	.L2087
.L2083:
	ldr	r1, .L2167+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2074:
	ldr	r1, .L2167+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2164:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2059
	ldr	r1, .L2167+108
	mov	r2, #64
	ldr	r3, .L2167+72
	cmp	r10, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2059
.L2066:
	ldr	r1, .L2167+176
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2160:
	ldr	r1, .L2167+180
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2150:
	mov	r0, r4
	movw	r3, #10150
	ldr	r2, .L2167+184
	ldr	r1, .L2167+188
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2141
.L2168:
	.align	2
.L2167:
	.word	.LC230
	.word	.LC232
	.word	.LC235
	.word	.LC243
	.word	.LC244
	.word	.LC246
	.word	.LC247
	.word	.LC261
	.word	.LC248
	.word	.LC221
	.word	.LC223
	.word	.LC224
	.word	g_event_report
	.word	.LC225
	.word	.LC226
	.word	.LC227
	.word	.LC229
	.word	.LANCHOR0+80
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC168
	.word	.LC234
	.word	.LC236
	.word	.LC251
	.word	.LC252
	.word	.LC253
	.word	.LC254
	.word	.LC259
	.word	.LANCHOR0+112
	.word	.LC222
	.word	.LC231
	.word	.LC255
	.word	.LC256
	.word	.LC257
	.word	.LC258
	.word	.LC260
	.word	.LC228
	.word	.LC237
	.word	.LC238
	.word	.LC239
	.word	.LC240
	.word	.LC242
	.word	.LC245
	.word	.LC250
	.word	.LC241
	.word	.LC233
	.word	.LC249
	.word	.LC20
	.word	.LC21
	UNWIND(.fnend)
	.size	ProcessSPS, .-ProcessSPS
	.align	2
	.global	Sps_SvcExt
	.type	Sps_SvcExt, %function
Sps_SvcExt:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r1, r0, #12288
	add	r2, r0, #20480
	mov	lr, #2208
	ldr	r3, [r1, #1304]
	mov	r4, r0
	ldr	r2, [r2, #220]
	add	r0, r0, #13504
	ldr	ip, [r1, #1300]
	mov	r5, #4000
	mov	r1, #32
	add	r0, r0, #24
	mla	r3, lr, r2, r3
	ldr	r3, [r3, #28]
	mla	r5, r5, r3, ip
	bl	BsGet
	ldr	r1, .L2183
	mov	r6, r0
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2183+4
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	mov	r0, r4
	ldr	r2, .L2183+8
	mov	r1, #2
	bl	u_v
	cmp	r0, #1
	beq	.L2179
	ldr	r2, [r5, #40]
	sub	r1, r2, #1
	cmp	r1, #1
	bls	.L2180
.L2172:
	cmp	r2, #1
	beq	.L2181
.L2173:
	ldr	r1, .L2183+12
	mov	r0, r4
	bl	u_1
	cmp	r0, #0
	bne	.L2182
.L2174:
	mov	r0, r4
	ldr	r1, .L2183+16
	bl	u_1
	cmp	r0, #1
	moveq	r0, #0
	beq	.L2171
	ldr	ip, [r5, #40]
	mov	r2, r0
	mov	r3, r6
	ldr	r1, .L2183+20
	mov	r0, #1
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r0, #0
.L2171:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2182:
	ldr	r1, .L2183+24
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2183+28
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	b	.L2174
.L2181:
	ldr	r2, .L2183+32
	mov	r1, #2
	mov	r0, r4
	bl	u_v
	ldr	r1, .L2183+36
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	b	.L2173
.L2180:
	ldr	r1, .L2183+40
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2183+44
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	ldr	r2, [r5, #40]
	b	.L2172
.L2179:
	ldr	r1, .L2183+48
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2171
.L2184:
	.align	2
.L2183:
	.word	.LC262
	.word	.LC263
	.word	.LC264
	.word	.LC270
	.word	.LC273
	.word	.LC274
	.word	.LC271
	.word	.LC272
	.word	.LC268
	.word	.LC269
	.word	.LC266
	.word	.LC267
	.word	.LC265
	UNWIND(.fnend)
	.size	Sps_SvcExt, .-Sps_SvcExt
	.align	2
	.global	H264_DecSPS
	.type	H264_DecSPS, %function
H264_DecSPS:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	r2, .L2220
	mov	r1, #8
	mov	r4, r0
	add	r7, r0, #61440
	bl	u_v
	ldr	r1, .L2220+4
	mov	r10, r0
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2220+8
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2220+12
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2220+16
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	u_1
	ldr	r2, .L2220+20
	mov	r1, #4
	str	r0, [fp, #-68]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L2220+24
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	ldr	r1, .L2220+28
	mov	r6, r0
	mov	r0, r4
	bl	ue_v
	ldr	r2, [r7, #1076]
	cmp	r2, #0
	mov	r9, r0
	bne	.L2214
	add	r5, r4, #12288
	ldr	r2, [r5, #1288]
	sub	r2, r2, #1
	cmp	r0, r2
	bhi	.L2215
	sub	r2, r10, #66
	mov	r8, r10
	cmp	r2, #78
	ldrls	pc, [pc, r2, asl #2]
	b	.L2190
.L2192:
	.word	.L2191
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2193
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2193
	.word	.L2190
	.word	.L2190
	.word	.L2193
	.word	.L2190
	.word	.L2194
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2193
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2195
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2195
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2190
	.word	.L2195
.L2191:
	ldr	r1, .L2220+32
	mov	r0, #1
	bl	dprint_vfmw
.L2193:
	cmp	r6, #9
	mov	r2, r6
	bls	.L2213
.L2219:
	cmp	r6, #41
	bhi	.L2213
.L2197:
	mov	r3, #4000
	ldr	r1, [r5, #1300]
	mul	r7, r3, r9
	add	r1, r1, r7
	ldrb	r6, [r1, #23]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L2198
	ldr	r6, .L2220+36
	mov	r0, r4
	str	r3, [fp, #-72]
	ldrb	r3, [fp, #-56]	@ zero_extendqisi2
	add	r1, r6, #2208
	str	r8, [r6, #2236]
	str	r2, [r6, #2240]
	strb	r3, [r6, #2208]
	ldrb	r3, [fp, #-60]	@ zero_extendqisi2
	str	r9, [r6, #2244]
	strb	r3, [r6, #2209]
	ldrb	r3, [fp, #-64]	@ zero_extendqisi2
	strb	r3, [r6, #2210]
	ldrb	r3, [fp, #-68]	@ zero_extendqisi2
	strb	r3, [r6, #2211]
	bl	ProcessSPS
	ldr	r3, [fp, #-72]
	cmp	r0, #0
	bne	.L2216
	ldr	r1, [r5, #1300]
	add	r0, r6, #2208
	str	r3, [fp, #-56]
	add	r1, r1, r7
	bl	SPSEqual
	ldr	r3, [fp, #-56]
	cmp	r0, #0
	bne	.L2217
.L2200:
	cmp	r8, #83
	cmpne	r8, #86
	beq	.L2202
.L2203:
	mov	r0, #0
.L2187:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2213:
	mov	r3, #41
	ldr	r1, .L2220+40
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, #41
	b	.L2197
.L2198:
	str	r8, [r1, #28]
	mov	r0, r4
	ldr	r3, [r5, #1300]
	ldrb	r1, [fp, #-56]	@ zero_extendqisi2
	strb	r1, [r3, r7]
	ldr	r3, [r5, #1300]
	ldrb	r1, [fp, #-60]	@ zero_extendqisi2
	add	r3, r3, r7
	strb	r1, [r3, #1]
	ldr	r3, [r5, #1300]
	ldrb	r1, [fp, #-64]	@ zero_extendqisi2
	add	r3, r3, r7
	strb	r1, [r3, #2]
	ldr	r3, [r5, #1300]
	ldrb	r1, [fp, #-68]	@ zero_extendqisi2
	add	r3, r3, r7
	strb	r1, [r3, #3]
	ldr	r3, [r5, #1300]
	add	r3, r3, r7
	str	r2, [r3, #32]
	ldr	r3, [r5, #1300]
	add	r3, r3, r7
	str	r9, [r3, #36]
	ldr	r1, [r5, #1300]
	add	r1, r1, r7
	bl	ProcessSPS
	cmp	r0, #0
	bne	.L2218
	ldr	r3, [r5, #1300]
	mov	r2, #1
	cmp	r8, #83
	cmpne	r8, #86
	add	r3, r3, r7
	strb	r2, [r3, #24]
	ldr	r3, [r5, #1300]
	add	r7, r3, r7
	strb	r2, [r7, #23]
	bne	.L2203
.L2202:
	mov	r0, r4
	bl	Sps_SvcExt
	cmp	r0, #0
	beq	.L2203
	ldr	r1, .L2220+44
	mov	r0, #1
	bl	dprint_vfmw
.L2214:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2215:
	ldr	r1, .L2220+48
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r5, #1288]
	sub	r3, r2, #1
	cmp	r9, r3
	bls	.L2214
	ldr	r3, .L2220+52
	ldr	r4, [r3]
	cmp	r4, #0
	beq	.L2214
	str	r2, [fp, #-48]
	mov	r3, #8
	str	r9, [fp, #-52]
	sub	r2, fp, #52
	ldr	r0, [r7, #1136]
	mov	r1, #109
	blx	r4
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2195:
	ldr	r1, .L2220+56
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2219
	b	.L2213
.L2194:
	ldr	r1, .L2220+60
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2219
	b	.L2213
.L2190:
	mov	r2, r10
	ldr	r1, .L2220+64
	mov	r0, #1
	mov	r8, #100
	bl	dprint_vfmw
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2219
	b	.L2213
.L2217:
	ldr	ip, .L2220+68
	mov	r2, r3
	ldr	r0, [r5, #1300]
	mov	r3, #1
	add	r1, r6, #2208
	strb	r3, [r6, #2232]
	add	r0, r0, r7
	ldr	ip, [ip, #56]
	strb	r3, [r6, #2231]
	blx	ip
	b	.L2200
.L2216:
	mov	r3, r9
	movw	r2, #10776
	ldr	r1, .L2220+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2187
.L2218:
	mov	r3, r9
	movw	r2, #10801
	ldr	r1, .L2220+72
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #1300]
	mov	r2, #1
	mvn	r0, #0
	add	r3, r3, r7
	strb	r2, [r3, #24]
	ldr	r3, [r5, #1300]
	add	r7, r3, r7
	strb	r6, [r7, #23]
	b	.L2187
.L2221:
	.align	2
.L2220:
	.word	.LC275
	.word	.LC276
	.word	.LC277
	.word	.LC278
	.word	.LC279
	.word	.LC280
	.word	.LC281
	.word	.LC282
	.word	.LC283
	.word	.LANCHOR2
	.word	.LC287
	.word	.LC289
	.word	.LC174
	.word	g_event_report
	.word	.LC285
	.word	.LC284
	.word	.LC286
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC288
	UNWIND(.fnend)
	.size	H264_DecSPS, .-H264_DecSPS
	.align	2
	.global	PassBytes
	.type	PassBytes, %function
PassBytes:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, [r0, #368]
	cmp	ip, #0
	cmpne	r1, #0
	beq	.L2231
	ldrb	r3, [ip]	@ zero_extendqisi2
	mov	r2, r3, asl #2
	mov	r5, r3, asl #5
	rsb	r4, r2, r5
	add	r4, ip, r4
	add	r6, r4, #8
	ldr	lr, [r4, #8]
	cmp	lr, #0
	beq	.L2231
	ldr	lr, [ip, #68]
	cmp	r3, #1
	cmpls	lr, #2
	bhi	.L2231
	cmp	lr, r3
	bls	.L2231
	ldr	lr, [r4, #12]
	mov	r6, r2
	ldr	r4, [r4, #24]
	mov	lr, lr, asl #3
	add	r1, r4, r1, lsl #3
	cmp	r1, lr
	bhi	.L2226
	b	.L2224
.L2227:
	ldr	r4, [r2, #24]
	ldr	r2, [r2, #12]
	rsb	lr, lr, r4
	add	r1, r1, lr
	mov	lr, r2, asl #3
	cmp	r1, lr
	bls	.L2224
.L2226:
	rsb	r2, r6, r5
	add	r3, r3, #1
	add	ip, ip, r2
	mov	r6, r3, asl #2
	mov	r5, r3, asl #5
	str	lr, [ip, #24]
	rsb	r2, r6, r5
	ldr	ip, [r0, #368]
	ldrb	r4, [ip]	@ zero_extendqisi2
	add	r4, r4, #1
	strb	r4, [ip]
	ldr	ip, [r0, #368]
	add	r2, ip, r2
	ldr	r4, [ip, #68]
	cmp	r4, r3
	bhi	.L2227
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2224:
	rsb	r2, r6, r5
	mov	r0, #1
	add	ip, ip, r2
	str	r1, [ip, #24]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2231:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	PassBytes, .-PassBytes
	.align	2
	.global	GetBytes
	.type	GetBytes, %function
GetBytes:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	beq	.L2246
	ldr	r3, [r0, #368]
	cmp	r3, #0
	cmpne	r2, #0
	ble	.L2246
	ldrb	r7, [r3]	@ zero_extendqisi2
	mov	ip, r7, asl #5
	sub	ip, ip, r7, asl #2
	add	ip, r3, ip
	ldr	ip, [ip, #8]
	cmp	ip, #0
	beq	.L2246
	ldr	ip, [r3, #68]
	cmp	r7, #1
	cmpls	ip, #2
	movhi	lr, #1
	movls	lr, #0
	bhi	.L2246
	cmp	ip, r7
	bls	.L2246
.L2241:
	mov	r5, r7, asl #5
	sub	r5, r5, r7, asl #2
	add	r3, r3, r5
	ldr	r4, [r3, #24]
	ldr	r8, [r3, #8]
	add	r4, r4, #7
	ldr	r6, [r3, #12]
	add	ip, r8, r4, lsr #3
	add	r6, r8, r6
	cmp	ip, r6
	bcs	.L2237
	ldrb	r3, [r8, r4, lsr #3]	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	lr, r2
	add	r4, r1, #1
	add	ip, ip, #1
	strb	r3, [r1]
	b	.L2248
.L2239:
	cmp	ip, r6
	mov	r1, r4
	beq	.L2237
	ldrb	r3, [ip], #1	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	lr, r2
	strb	r3, [r4], #1
.L2248:
	ldr	r3, [r0, #368]
	add	r3, r3, r5
	ldr	r1, [r3, #24]
	add	r1, r1, #8
	str	r1, [r3, #24]
	bcc	.L2239
	mov	r0, r2
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2237:
	ldr	ip, [r0, #368]
	add	r7, r7, #1
	ldrb	r3, [ip]	@ zero_extendqisi2
	add	r3, r3, #1
	strb	r3, [ip]
	ldr	r3, [r0, #368]
	ldr	ip, [r3, #68]
	cmp	ip, r7
	bhi	.L2241
	mov	r0, lr
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2246:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	GetBytes, .-GetBytes
	.align	2
	.global	H264_DecFramePackingSEI
	.type	H264_DecFramePackingSEI, %function
H264_DecFramePackingSEI:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, .L2256
	mov	r5, r0
	bl	ue_v
	add	r4, r5, #24576
	ldr	r2, .L2256+4
	mov	r1, #1
	str	r0, [r4, #1840]
	mov	r0, r5
	bl	u_v
	uxtb	r0, r0
	strb	r0, [r4, #1817]
	cmp	r0, #0
	beq	.L2255
.L2250:
	mov	r0, r5
	ldr	r2, .L2256+8
	mov	r1, #1
	bl	u_v
	mov	r3, #1
	strb	r3, [r4, #1816]
	strb	r0, [r4, #1818]
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2255:
	ldr	r2, .L2256+12
	mov	r1, #7
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+16
	mov	r1, #1
	str	r0, [r4, #1832]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+20
	mov	r1, #6
	strb	r0, [r4, #1819]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+24
	mov	r1, #1
	str	r0, [r4, #1836]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+28
	mov	r1, #1
	strb	r0, [r4, #1820]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+32
	mov	r1, #1
	strb	r0, [r4, #1821]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+36
	mov	r1, #1
	strb	r0, [r4, #1822]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+40
	mov	r1, #1
	strb	r0, [r4, #1823]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+44
	mov	r1, #1
	strb	r0, [r4, #1824]
	mov	r0, r5
	bl	u_v
	ldrb	r3, [r4, #1819]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #1825]
	bne	.L2251
	ldr	r3, [r4, #1832]
	cmp	r3, #5
	beq	.L2251
	ldr	r2, .L2256+48
	mov	r1, #1
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+52
	mov	r1, #1
	strb	r0, [r4, #1826]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+56
	mov	r1, #1
	strb	r0, [r4, #1827]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2256+60
	mov	r1, #1
	strb	r0, [r4, #1828]
	mov	r0, r5
	bl	u_v
	strb	r0, [r4, #1829]
.L2251:
	ldr	r2, .L2256+64
	mov	r1, #8
	mov	r0, r5
	bl	u_v
	ldr	r1, .L2256+68
	str	r0, [r4, #1844]
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #1848]
	b	.L2250
.L2257:
	.align	2
.L2256:
	.word	.LC290
	.word	.LC291
	.word	.LC307
	.word	.LC292
	.word	.LC293
	.word	.LC294
	.word	.LC295
	.word	.LC296
	.word	.LC297
	.word	.LC298
	.word	.LC299
	.word	.LC300
	.word	.LC301
	.word	.LC302
	.word	.LC303
	.word	.LC304
	.word	.LC305
	.word	.LC306
	UNWIND(.fnend)
	.size	H264_DecFramePackingSEI, .-H264_DecFramePackingSEI
	.align	2
	.global	H264_Sei_Get_Dpb_Output_Time
	.type	H264_Sei_Get_Dpb_Output_Time, %function
H264_Sei_Get_Dpb_Output_Time:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #12288
	add	r3, r0, #20480
	mov	r4, #2208
	add	r0, r0, #24576
	ldr	lr, [r2, #1304]
	mov	r1, #4000
	ldr	r3, [r3, #220]
	ldr	r6, [r2, #1300]
	ldr	ip, [r0, #1856]
	mla	r4, r4, r3, lr
	ldr	r3, [r0, #1868]
	ldr	lr, [r0, #1860]
	add	r3, r3, #1
	ldr	r2, [r0, #1872]
	subs	r5, r3, #1
	add	r2, r2, #1
	ldr	r4, [r4, #28]
	movne	r5, #1
	cmp	ip, #0
	str	ip, [r0, #1876]
	str	r2, [r0, #1872]
	mla	r1, r1, r4, r6
	movne	r5, #0
	cmp	r5, #0
	str	r3, [r0, #1868]
	str	lr, [r0, #1884]
	ldrne	ip, [r0, #1880]
	ldr	r2, [r1, #3344]
	addne	ip, ip, #2
	str	r2, [r0, #1920]
	ldr	r2, [r1, #3348]
	strne	ip, [r0, #1876]
	cmp	r3, #1
	str	r2, [r0, #1924]
	beq	.L2266
	ldrb	r3, [r0, #1945]	@ zero_extendqisi2
	add	r4, r0, #1872
	cmp	r3, #1
	ldrd	r2, [r4, #40]
	beq	.L2267
	adds	r2, r2, ip
	adc	r3, r3, #0
	strd	r2, [r4, #32]
.L2261:
	adds	r2, r2, lr
	mov	lr, #0
	adc	r3, r3, #0
	strb	lr, [r0, #1945]
	ldr	r4, [r1, #3368]
	movs	r3, r3, lsr #1
	mov	r2, r2, rrx
	ldr	r8, [r1, #3344]
	add	lr, r0, #1872
	subs	r6, r2, r4
	sbc	r7, r3, #0
	umull	r4, r5, r6, r8
	mla	r5, r8, r7, r5
	strd	r4, [lr, #16]
	ldr	r1, [r1, #3368]
	subs	r2, r2, r1
	sbc	r3, r3, #0
	strd	r2, [lr, #24]
	str	ip, [r0, #1880]
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2266:
	mov	r6, #0
	str	r6, [r0, #1904]
	mov	r2, r6
	mov	r3, r6
	str	r6, [r0, #1912]
	str	r6, [r0, #1908]
	str	r6, [r0, #1916]
	b	.L2261
.L2267:
	adds	r2, r2, ip
	adc	r3, r3, #0
	strd	r2, [r4, #32]
	strd	r2, [r4, #40]
	b	.L2261
	UNWIND(.fnend)
	.size	H264_Sei_Get_Dpb_Output_Time, .-H264_Sei_Get_Dpb_Output_Time
	.align	2
	.global	H264_Get_Last_IDR_Frame_Info
	.type	H264_Get_Last_IDR_Frame_Info, %function
H264_Get_Last_IDR_Frame_Info:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #5
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, [r0]
	mvn	r3, #0
	mvn	r2, #0
	ldrd	r4, [r1, #56]
	cmp	r5, r3
	cmpeq	r4, r2
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r2, r0, #26368
	movw	r1, #26512
	add	r3, r2, #112
	strd	r4, [r2, #136]
	ldrd	r2, [r3, #-8]
	strd	r2, [r0, r1]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	H264_Get_Last_IDR_Frame_Info, .-H264_Get_Last_IDR_Frame_Info
	.align	2
	.global	H264_Sei_Get_Frame_Pts
	.type	H264_Sei_Get_Frame_Pts, %function
H264_Sei_Get_Frame_Pts:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r2, r0, #26368
	add	r1, r0, #24576
	mov	r4, r0
	ldrd	r6, [r2, #136]
	mov	r3, r2
	ldr	r5, [r1, #1920]
	movw	r0, #26512
	ldr	r10, [r1, #1924]
	strd	r6, [fp, #-52]
	cmp	r5, #0
	ldrd	r6, [r3, #104]
	ldrd	r2, [r4, r0]
	strd	r2, [fp, #-60]
	beq	.L2273
	mov	r9, r5
	mov	r0, r10
	b	.L2272
.L2274:
	mov	r9, r1
.L2272:
	mov	r1, r9
	bl	__aeabi_uidivmod
	mov	r0, r9
	cmp	r1, #0
	bne	.L2274
.L2271:
	ldrd	r2, [fp, #-60]
	mov	r1, r9
	mov	r0, r5
	mov	r8, #1000
	subs	r2, r6, r2
	movw	r5, #59696
	mul	r6, r8, r2
	bl	__aeabi_uidiv
	mov	r1, r9
	mul	r6, r0, r6
	mov	r0, r10
	bl	__aeabi_uidiv
	mov	r1, r0
	mov	r0, r6
	bl	__aeabi_uidiv
	ldrd	r2, [fp, #-52]
	adds	r2, r2, r0
	adc	r3, r3, #0
	mov	r0, r2
	mov	r1, r3
	strd	r0, [r4, r5]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2273:
	mov	r9, r10
	b	.L2271
	UNWIND(.fnend)
	.size	H264_Sei_Get_Frame_Pts, .-H264_Sei_Get_Frame_Pts
	.align	2
	.global	H264_DecPicTimingSEI
	.type	H264_DecPicTimingSEI, %function
H264_DecPicTimingSEI:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #12288
	mov	r4, #4000
	mov	r5, r0
	ldr	r1, [r2, #1344]
	ldr	r3, [r2, #1300]
	mla	r4, r4, r1, r3
	ldrb	r1, [r4, #23]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2276
	ldr	ip, [r2, #1288]
	cmp	ip, #0
	ble	.L2277
	ldrb	r2, [r3, #23]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #4000
	beq	.L2280
	b	.L2299
.L2281:
	ldrb	r1, [r3, #-3977]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2278
.L2280:
	add	r2, r2, #1
	mov	r4, r3
	cmp	r2, ip
	add	r3, r3, #4000
	bne	.L2281
.L2277:
	ldr	r1, .L2301
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
.L2282:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2299:
	mov	r4, r3
.L2278:
	cmp	r4, #0
	beq	.L2277
.L2276:
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2283
	ldrb	r3, [r4, #3299]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2300
	ldr	r1, [r4, #3676]
	ldr	r7, [r4, #3680]
	add	r1, r1, #1
	add	r7, r7, #1
.L2286:
	ldr	r2, .L2301+4
	mov	r0, r5
	bl	u_v
	add	r6, r5, #24576
	mov	r1, r7
	ldr	r2, .L2301+8
	str	r0, [r6, #1856]
	mov	r0, r5
	bl	u_v
	str	r0, [r6, #1860]
.L2283:
	ldrb	r0, [r4, #3303]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	r3, r0
	beq	.L2282
	ldr	r2, .L2301+12
	mov	r1, #4
	mov	r0, r5
	add	r5, r5, #24576
	bl	u_v
	mov	r3, #0
	strb	r0, [r5, #1852]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2300:
	ldrb	r3, [r4, #3302]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2283
	ldr	r1, [r4, #3988]
	ldr	r7, [r4, #3992]
	add	r1, r1, #1
	add	r7, r7, #1
	b	.L2286
.L2302:
	.align	2
.L2301:
	.word	.LC308
	.word	.LC309
	.word	.LC310
	.word	.LC311
	UNWIND(.fnend)
	.size	H264_DecPicTimingSEI, .-H264_DecPicTimingSEI
	.align	2
	.global	H264_DecSEI
	.type	H264_DecSEI, %function
H264_DecSEI:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	r2, [r0, #368]
	sub	r8, fp, #44
	mov	r3, #0
	add	r6, r0, #13504
	mov	r1, #32
	mov	r7, r0
	add	r6, r6, #24
	add	r4, r0, #12288
	mov	r9, r3
	strb	r3, [r8, #-1]!
	str	r1, [r2, #24]
	b	.L2306
.L2537:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2305
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2536
.L2306:
	mov	r2, #1
	mov	r1, r8
	mov	r0, r7
	bl	GetBytes
	ldr	r3, [r4, #1264]
	ldr	ip, [r4, #1256]
	mov	r1, #8
	add	r3, r3, r1
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, ip, asl #3
	add	r9, r9, r2
	mov	r5, r0
	mov	r0, r6
	bls	.L2537
.L2305:
	ldr	r1, .L2570
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2308
.L2536:
	mov	r3, #0
	str	r3, [fp, #-68]
.L2486:
	mov	r5, #0
	b	.L2312
.L2539:
	bl	BsSkip
	cmp	r10, #0
	ble	.L2310
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2538
.L2312:
	mov	r2, #1
	mov	r1, r8
	mov	r0, r7
	bl	GetBytes
	ldr	r3, [r4, #1264]
	ldr	ip, [r4, #1256]
	mov	r1, #8
	add	r3, r3, r1
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, ip, asl #3
	add	r5, r5, r2
	mov	r10, r0
	mov	r0, r6
	bls	.L2539
.L2310:
	ldr	r1, .L2570+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2308:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2538:
	cmp	r5, #409600
	bgt	.L2310
	ldr	r3, [r7, #368]
	ldr	r2, [r3, #68]
	cmp	r2, #2
	ldreq	r2, [r3, #52]
	ldreq	r3, [r3, #24]
	ldrne	r2, [r3, #24]
	addeq	r2, r2, r3
	moveq	r2, r2, lsr #3
	cmp	r9, #45
	ldrls	pc, [pc, r9, asl #2]
	b	.L2315
.L2317:
	.word	.L2316
	.word	.L2318
	.word	.L2319
	.word	.L2320
	.word	.L2321
	.word	.L2322
	.word	.L2323
	.word	.L2324
	.word	.L2325
	.word	.L2326
	.word	.L2327
	.word	.L2328
	.word	.L2329
	.word	.L2330
	.word	.L2331
	.word	.L2332
	.word	.L2333
	.word	.L2334
	.word	.L2335
	.word	.L2336
	.word	.L2337
	.word	.L2338
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2315
	.word	.L2339
.L2339:
	mov	r1, r5
	mov	r0, r7
	ldr	r9, [r4, #1264]
	bl	H264_DecFramePackingSEI
	ldr	r0, [r4, #1264]
	mov	r10, r5, asl #3
	rsb	r3, r9, r0
	cmp	r3, r10
	bge	.L2469
	rsb	r3, r3, r10
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r9, r3, #7
	movge	r9, r3
	ldr	r3, [r4, #1256]
	mov	r9, r9, asr #3
	cmp	r9, #0
	movle	r3, r3, asl #3
	ble	.L2471
	add	r2, r0, #8
	mov	r3, r3, asl #3
	cmp	r2, r3
	bhi	.L2471
	mov	r10, #0
	b	.L2472
.L2473:
	add	r2, r0, #8
	cmp	r2, r3
	bhi	.L2471
.L2472:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #1256]
	cmp	r10, r9
	ldr	r0, [r4, #1264]
	mov	r3, r3, asl #3
	bne	.L2473
.L2471:
	ldr	r2, [fp, #-56]
	mov	r1, r2, asr #31
	mov	r1, r1, lsr #29
	add	r2, r2, r1
	and	r2, r2, #7
	rsb	r1, r1, r2
	add	r2, r0, r1
	cmp	r3, r2
	bcs	.L2540
.L2469:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2541
.L2532:
	add	r3, r7, #57344
	ldr	r3, [r3, #3848]
.L2342:
	cmp	r3, #3
	bhi	.L2542
.L2481:
	mov	r9, #0
	b	.L2485
.L2483:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2484
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2543
.L2485:
	mov	r2, #1
	mov	r1, r8
	mov	r0, r7
	bl	GetBytes
	ldr	r3, [r4, #1264]
	ldr	ip, [r4, #1256]
	mov	r1, #8
	add	r3, r3, r1
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, ip, asl #3
	add	r9, r9, r2
	mov	r5, r0
	mov	r0, r6
	bls	.L2483
.L2484:
	mov	r0, #0
.L2568:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2338:
	mov	r3, r5
	ldr	r1, .L2570+8
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2544
.L2463:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2466
.L2467:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2466:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2467
	b	.L2532
.L2337:
	mov	r3, r5
	ldr	r1, .L2570+12
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2545
.L2458:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2461
.L2462:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2461:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2462
	b	.L2532
.L2335:
	mov	r3, r5
	ldr	r1, .L2570+16
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2546
.L2448:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2451
.L2452:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2451:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2452
	b	.L2532
.L2334:
	mov	r3, r5
	ldr	r1, .L2570+20
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2547
.L2443:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2446
.L2447:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2446:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2447
	b	.L2532
.L2333:
	mov	r3, r5
	ldr	r1, .L2570+24
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2548
.L2438:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2441
.L2442:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2441:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2442
	b	.L2532
.L2332:
	mov	r3, r5
	ldr	r1, .L2570+28
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2549
.L2433:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2436
.L2437:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2436:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2437
	b	.L2532
.L2336:
	mov	r3, r5
	ldr	r1, .L2570+32
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2550
.L2453:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2456
.L2457:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2456:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2457
	b	.L2532
.L2331:
	mov	r3, r5
	ldr	r1, .L2570+36
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2551
.L2428:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2431
.L2432:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2431:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2432
	b	.L2532
.L2330:
	mov	r3, r5
	ldr	r1, .L2570+40
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2552
.L2423:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2426
.L2427:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2426:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2427
	b	.L2532
.L2329:
	mov	r3, r5
	ldr	r1, .L2570+44
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2553
.L2418:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2421
.L2422:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2421:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2422
	b	.L2532
.L2328:
	mov	r3, r5
	ldr	r1, .L2570+48
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2554
.L2413:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2416
.L2417:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2416:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2417
	b	.L2532
.L2327:
	mov	r3, r5
	ldr	r1, .L2570+52
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2555
.L2408:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2411
.L2412:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2411:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2412
	b	.L2532
.L2326:
	mov	r3, r5
	ldr	r1, .L2570+56
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2556
.L2403:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2406
.L2407:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2406:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2407
	b	.L2532
.L2325:
	mov	r3, r5
	ldr	r1, .L2570+60
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2557
.L2398:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2401
.L2402:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2401:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2402
	b	.L2532
.L2324:
	mov	r3, r5
	ldr	r1, .L2570+64
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2558
.L2393:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2396
.L2397:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2396:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2397
	b	.L2532
.L2323:
	mov	r3, r5
	ldr	r1, .L2570+68
	mov	r0, #20
	bl	dprint_vfmw
	cmp	r5, #0
	ble	.L2390
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2390
	mov	r9, #0
	b	.L2391
.L2392:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2390
.L2391:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2392
.L2390:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	bge	.L2532
	movw	r3, #11637
	b	.L2533
.L2322:
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	bne	.L2321
	mov	r3, r5
	ldr	r1, .L2570+72
	mov	r0, #20
	bl	dprint_vfmw
.L2365:
	add	r10, r7, #57344
	add	r3, r7, #61440
	ldr	r9, [r10, #3848]
	cmp	r9, #3
	strls	r3, [fp, #-56]
	bhi	.L2559
.L2366:
	ldr	r3, [fp, #-56]
	ldr	r0, [r3, #1136]
	bl	GetUsd
	add	r3, r9, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	str	r0, [r3, #4]
	ldr	r3, [r10, #3848]
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	lr, [r3, #4]
	cmp	lr, #0
	beq	.L2489
	ldr	r2, [r7, #368]
	cmp	r5, #1024
	movlt	r3, r5
	movge	r3, #1024
	ldrb	r0, [r2]	@ zero_extendqisi2
	ldr	ip, [r2, #68]
	str	r3, [fp, #-60]
	cmp	r0, ip
	bcs	.L2490
	sxth	r3, r0
	mov	r9, #0
	mov	r1, r3, asl #5
	sub	r1, r1, r3, asl #2
	add	r1, r2, r1
.L2368:
	ldr	r3, [r1, #24]
	add	r0, r0, #1
	ldr	r2, [r1, #12]
	cmp	r0, ip
	add	r3, r3, #7
	add	r1, r1, #28
	sub	r3, r2, r3, lsr #3
	add	r9, r9, r3
	bne	.L2368
	mov	r2, r9
.L2367:
	ldr	r1, [fp, #-60]
	ldrsb	r3, [fp, #-68]
	cmp	r1, r2
	movle	r9, r1
	cmp	r3, #1
	movne	r3, #0
	strne	r3, [fp, #-64]
	beq	.L2560
.L2370:
	rsb	r2, r3, r9
	cmp	r2, #0
	ble	.L2561
	ldr	r3, [r10, #3848]
	mov	r0, r7
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r1, [r3, #4]
	bl	GetBytes
	subs	r9, r0, #0
	ble	.L2562
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-64]
	rsb	r3, r2, r3
	cmp	r9, r3
	ldr	r3, [r10, #3848]
	bge	.L2376
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r9, [r3, #1052]
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2377
	mov	r5, #0
	b	.L2378
.L2381:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2377
.L2378:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2381
.L2377:
	ldr	r3, [r10, #3848]
	mov	r2, #8
	ldr	r1, [r7]
	add	r3, r3, #15296
	ldr	ip, .L2570+108
	add	r3, r3, #2
	ldrd	r0, [r1, #56]
	add	r3, r7, r3, lsl #2
	ldr	ip, [ip]
	ldr	r3, [r3, #4]
	cmp	ip, #0
	add	r3, r3, #1056
	strd	r0, [r3]
	ldr	r3, [r10, #3848]
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r2, [r3, #1040]
	ldr	r3, [r10, #3848]
	ldr	r2, [fp, #-56]
	add	r3, r3, #15296
	add	r3, r3, #2
	ldr	r2, [r2, #1096]
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r2, [r3, #1044]
	beq	.L2380
	ldr	r2, [r10, #3848]
	movw	r3, #1064
	ldr	r1, [fp, #-56]
.L2534:
	add	r2, r2, #15296
	ldr	r0, [r1, #1136]
	add	r2, r2, #2
	mov	r1, #6
	add	r2, r7, r2, lsl #2
	ldr	r2, [r2, #4]
	blx	ip
.L2380:
	ldr	r3, [r10, #3848]
	add	r3, r3, #1
	str	r3, [r10, #3848]
	cmp	r3, #3
	bls	.L2481
.L2542:
	ldr	r1, .L2570+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2308
.L2319:
	mov	r3, r5
	ldr	r1, .L2570+80
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2563
.L2354:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2357
.L2358:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2357:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2358
	b	.L2532
.L2318:
	mov	r3, r5
	ldr	r1, .L2570+84
	mov	r0, #20
	mov	r10, r5, asl #3
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	ldr	r9, [r4, #1264]
	bl	H264_DecPicTimingSEI
	ldr	r2, [r4, #1264]
	rsb	r3, r9, r2
	cmp	r3, r10
	bge	.L2347
	rsb	r3, r3, r10
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r9, r3, #7
	movge	r9, r3
	ldr	r3, [r4, #1256]
	mov	r9, r9, asr #3
	cmp	r9, #0
	movle	r3, r3, asl #3
	ble	.L2349
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bhi	.L2349
	mov	r10, #0
	b	.L2350
.L2351:
	add	r1, r2, #8
	cmp	r1, r3
	bhi	.L2349
.L2350:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #1256]
	cmp	r10, r9
	ldr	r2, [r4, #1264]
	mov	r3, r3, asl #3
	bne	.L2351
.L2349:
	ldr	ip, [fp, #-56]
	mov	r1, ip, asr #31
	mov	r0, r1, lsr #29
	add	r1, ip, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r2, r1
	cmp	r2, r3
	bls	.L2564
.L2347:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	bge	.L2532
	movw	r3, #11385
	b	.L2533
.L2320:
	mov	r3, r5
	ldr	r1, .L2570+88
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2565
.L2359:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2362
.L2363:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2362:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2363
	b	.L2532
.L2316:
	add	r1, r7, #24576
	mov	lr, #1
	mov	r0, #0
	mov	r3, r5
	ldr	ip, [r1, #1864]
	str	r0, [r1, #1872]
	mov	r0, #20
	add	ip, ip, lr
	strb	lr, [r1, #1945]
	str	ip, [r1, #1864]
	ldr	r1, .L2570+92
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2566
.L2340:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2344
.L2345:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2344:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2345
	b	.L2532
.L2321:
	mov	r3, r5
	ldr	r1, .L2570+96
	mov	r0, #20
	bl	dprint_vfmw
	mov	r3, #1
	str	r3, [fp, #-68]
	b	.L2365
.L2315:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2567
.L2476:
	cmp	r5, #0
	ble	.L2532
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
	mov	r9, #0
	b	.L2479
.L2480:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2532
.L2479:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2480
	b	.L2532
.L2543:
	cmp	r3, #128
	bne	.L2486
	mov	r0, #0
	b	.L2568
.L2376:
	add	r3, r3, #15296
	ldr	r2, [fp, #-60]
	add	r3, r3, #2
	cmp	r2, r5
	ldr	r2, [fp, #-64]
	add	r3, r7, r3, lsl #2
	rsb	r9, r2, r5
	ldr	r3, [r3, #4]
	str	r9, [r3, #1052]
	bcc	.L2569
.L2383:
	cmp	r9, #0
	ble	.L2377
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2377
	mov	r5, #0
	b	.L2385
.L2571:
	.align	2
.L2570:
	.word	.LC312
	.word	.LC313
	.word	.LC341
	.word	.LC340
	.word	.LC338
	.word	.LC337
	.word	.LC336
	.word	.LC335
	.word	.LC339
	.word	.LC334
	.word	.LC333
	.word	.LC332
	.word	.LC331
	.word	.LC330
	.word	.LC329
	.word	.LC328
	.word	.LC327
	.word	.LC326
	.word	.LC320
	.word	.LC342
	.word	.LC317
	.word	.LC316
	.word	.LC318
	.word	.LC314
	.word	.LC319
	.word	.LC321
	.word	.LC323
	.word	g_event_report
	.word	.LANCHOR1+216
	.word	.LC315
	.word	.LC322
	.word	.LC324
	.word	.LC325
.L2386:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2377
.L2385:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2386
	b	.L2377
.L2559:
	mov	r9, r3
	ldr	r0, [r3, #1136]
	ldr	r1, [r10, #3852]
	str	r3, [fp, #-56]
	bl	FreeUsdByDec
	ldr	r0, [r9, #1136]
	ldr	r1, [r10, #3856]
	bl	FreeUsdByDec
	ldr	r0, [r9, #1136]
	ldr	r1, [r10, #3860]
	bl	FreeUsdByDec
	ldr	r0, [r9, #1136]
	ldr	r1, [r10, #3864]
	bl	FreeUsdByDec
	mov	r3, #0
	mov	r9, r3
	str	r3, [r10, #3852]
	str	r3, [r10, #3856]
	str	r3, [r10, #3860]
	str	r3, [r10, #3864]
	str	r3, [r10, #3848]
	b	.L2366
.L2541:
	movw	r3, #12065
.L2533:
	ldr	r2, .L2570+112
	mov	r0, #20
	ldr	r1, .L2570+116
	bl	dprint_vfmw
	add	r3, r7, #57344
	ldr	r3, [r3, #3848]
	b	.L2342
.L2540:
	mov	r0, r6
	bl	BsSkip
	b	.L2469
.L2564:
	mov	r0, r6
	bl	BsSkip
	b	.L2347
.L2560:
	strb	r3, [lr, #1026]
	mov	r1, #8
	ldr	r3, [r10, #3848]
	mov	r0, r7
	ldr	r2, .L2570+100
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r3, [fp, #-64]
	bl	u_v
	ldr	r3, [fp, #-64]
	strb	r0, [r3, #1027]
	ldr	r3, [r10, #3848]
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	ip, [r3, #4]
	ldrb	r3, [ip, #1027]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L2371
	mov	r3, #3
	mov	r2, #0
	str	r3, [fp, #-64]
	strb	r2, [ip, #1028]
.L2372:
	ldr	ip, [r10, #3848]
	mov	r1, #16
	ldr	r2, .L2570+104
	mov	r0, r7
	add	ip, ip, #15296
	str	r3, [fp, #-72]
	add	ip, ip, #2
	add	ip, r7, ip, lsl #2
	ldr	ip, [ip, #4]
	str	ip, [fp, #-76]
	bl	u_v
	ldr	ip, [fp, #-76]
	ldr	r3, [fp, #-72]
	add	ip, ip, #1024
	mov	r1, r3
	strh	r0, [ip, #6]	@ movhi
	mov	r0, r7
	bl	PassBytes
	ldr	r3, [fp, #-72]
	cmp	r9, r3
	bne	.L2370
	ldr	r3, [r10, #3848]
	mov	r2, #8
	ldr	r1, [r7]
	add	r3, r3, #15296
	ldr	ip, .L2570+108
	add	r3, r3, #2
	ldrd	r0, [r1, #56]
	add	r3, r7, r3, lsl #2
	ldr	ip, [ip]
	ldr	r3, [r3, #4]
	cmp	ip, #0
	add	r3, r3, #1056
	strd	r0, [r3]
	ldr	r3, [r10, #3848]
	ldr	r1, [fp, #-56]
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r2, [r3, #1040]
	ldr	r3, [r10, #3848]
	ldr	r2, [r1, #1096]
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r2, [r3, #1044]
	beq	.L2380
	ldr	r2, [r10, #3848]
	movw	r3, #1064
	b	.L2534
.L2569:
	ldr	r3, [fp, #-60]
	mov	r0, r7
	rsb	r1, r3, r5
	bl	PassBytes
	cmp	r0, #0
	bge	.L2383
	mov	r3, #11584
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2383
.L2490:
	mov	r2, #0
	mov	r9, r2
	b	.L2367
.L2567:
	movw	r3, #12077
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2476
.L2566:
	movw	r3, #11332
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2340
.L2565:
	movw	r3, #11423
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2359
.L2563:
	movw	r3, #11397
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2354
.L2558:
	movw	r3, #11649
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2393
.L2557:
	movw	r3, #11675
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2398
.L2556:
	movw	r3, #11701
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2403
.L2555:
	movw	r3, #11727
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2408
.L2554:
	movw	r3, #11753
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2413
.L2553:
	movw	r3, #11779
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2418
.L2552:
	movw	r3, #11805
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2423
.L2551:
	movw	r3, #11831
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2428
.L2550:
	movw	r3, #11961
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2453
.L2549:
	movw	r3, #11857
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2433
.L2548:
	movw	r3, #11883
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2438
.L2547:
	movw	r3, #11909
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2443
.L2546:
	movw	r3, #11935
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2448
.L2545:
	movw	r3, #11987
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2458
.L2544:
	movw	r3, #12013
	ldr	r2, .L2570+112
	ldr	r1, .L2570+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2463
.L2371:
	ldr	r2, .L2570+120
	mov	r1, #8
	mov	r0, r7
	mov	r3, #4
	str	ip, [fp, #-72]
	str	r3, [fp, #-64]
	bl	u_v
	ldr	ip, [fp, #-72]
	ldr	r3, [fp, #-64]
	strb	r0, [ip, #1028]
	b	.L2372
.L2489:
	mvn	r0, #0
	b	.L2308
.L2561:
	ldr	r1, .L2570+124
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r10, #3848]
	ldr	r2, [fp, #-56]
	add	r3, r3, #15296
	add	r3, r3, #2
	ldr	r0, [r2, #1136]
	add	r3, r7, r3, lsl #2
	ldr	r1, [r3, #4]
	bl	FreeUsdByDec
	ldr	r3, [r10, #3848]
	mov	r2, #0
	mvn	r0, #0
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	str	r2, [r3, #4]
	b	.L2308
.L2562:
	ldr	r1, .L2570+128
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r10, #3848]
	ldr	r2, [fp, #-56]
	add	r3, r3, #15296
	add	r3, r3, #2
	ldr	r0, [r2, #1136]
	add	r3, r7, r3, lsl #2
	ldr	r1, [r3, #4]
	bl	FreeUsdByDec
	ldr	r3, [r10, #3848]
	mov	r2, #0
	mvn	r0, #0
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r7, r7, r3, lsl #2
	str	r2, [r7, #4]
	b	.L2308
	UNWIND(.fnend)
	.size	H264_DecSEI, .-H264_DecSEI
	.align	2
	.global	InitOldSlice
	.type	InitOldSlice, %function
InitOldSlice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #12288
	add	r0, r0, #20480
	movw	r3, #23352
	ldr	r1, [r2, #1292]
	movt	r3, 1
	mov	r2, #2
	str	r3, [r0, #1872]
	strb	r2, [r0, #1857]
	mov	r3, #32
	mov	r2, #7
	strb	r3, [r0, #1858]
	str	r2, [r0, #1868]
	mvn	r3, #0
	mov	r2, #3
	str	r3, [r0, #1884]
	str	r2, [r0, #1880]
	mov	r3, #0
	mov	r2, #262144
	str	r1, [r0, #1864]
	str	r2, [r0, #1896]
	strb	r3, [r0, #1856]
	str	r3, [r0, #1888]
	str	r3, [r0, #1892]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	InitOldSlice, .-InitOldSlice
	.align	2
	.global	H264_IsNewPicNal
	.type	H264_IsNewPicNal, %function
H264_IsNewPicNal:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	mov	r5, r0
	and	r3, r3, #31
	sub	r3, r3, #1
	cmp	r3, #7
	ldrls	pc, [pc, r3, asl #2]
	b	.L2581
.L2576:
	.word	.L2575
	.word	.L2581
	.word	.L2581
	.word	.L2581
	.word	.L2575
	.word	.L2581
	.word	.L2577
	.word	.L2577
.L2577:
	ldr	r3, [r0]
	mov	r2, #1
	str	r2, [r0]
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2575:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L2578
.L2580:
	mvn	r0, #0
.L2579:
	mov	r2, #0
	str	r2, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2581:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2578:
	ldrb	ip, [r1, #5]	@ zero_extendqisi2
	ldrb	r3, [r1, #6]	@ zero_extendqisi2
	ldrb	r2, [r1, #7]	@ zero_extendqisi2
	mov	ip, ip, asl #16
	ldrb	r4, [r1, #4]	@ zero_extendqisi2
	orr	r3, ip, r3, asl #8
	orr	r3, r3, r2
	orr	r4, r3, r4, asl #24
	mov	r0, r4
	bl	ZerosMS_32
	cmp	r0, #15
	bhi	.L2580
	mov	r3, r0, asl #1
	rsb	r3, r3, #31
	mov	r3, r4, lsr r3
	subs	r0, r3, #1
	mvnne	r0, #0
	b	.L2579
	UNWIND(.fnend)
	.size	H264_IsNewPicNal, .-H264_IsNewPicNal
	.align	2
	.global	FindZeroBitsInSeg
	.type	FindZeroBitsInSeg, %function
FindZeroBitsInSeg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r0, #0
	cmpne	r1, #0
	mov	r9, r0
	str	r1, [fp, #-48]
	movle	r4, #1
	movgt	r4, #0
	ble	.L2608
	ldr	r3, [fp, #-48]
	subs	r7, r3, #0
	add	r8, r3, #63
	movge	r8, r3
	ands	r3, r3, #63
	movne	r3, #1
	add	r8, r3, r8, asr #6
	cmp	r8, #0
	ble	.L2596
	ldr	r5, .L2609
	ldr	r10, .L2609+4
.L2594:
	cmp	r7, #64
	ldr	r3, [r10, #52]
	ldr	r0, .L2609
	movcc	r6, r7
	movcs	r6, #64
	mov	r2, r6
	rsb	r1, r6, r7
	sub	r6, r6, #1
	add	r1, r9, r1
	blx	r3
	add	r3, r5, r6
	ldrb	r2, [r5, r6]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2586
	ldr	r2, .L2609
	add	r4, r4, #1
	cmp	r3, r2
	bne	.L2589
	b	.L2587
.L2590:
	cmp	r3, r5
	add	r4, r4, #1
	beq	.L2587
.L2589:
	ldrb	r2, [r3, #-1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2590
.L2586:
	mov	r0, r4, asl #3
.L2585:
	ldr	r3, [fp, #-48]
	cmp	r4, r3
	bge	.L2598
	sub	r3, r3, #1
	rsb	r4, r4, r3
	ldrb	r2, [r9, r4]	@ zero_extendqisi2
	tst	r2, #1
	moveq	r2, r2, lsr #1
	moveq	r3, #1
	bne	.L2598
.L2593:
	tst	r2, #1
	mov	r2, r2, lsr #1
	bne	.L2591
	add	r3, r3, #1
	cmp	r3, #8
	bne	.L2593
	b	.L2591
.L2587:
	subs	r8, r8, #1
	sub	r7, r7, #64
	bne	.L2594
	b	.L2586
.L2598:
	mov	r3, #0
.L2591:
	add	r0, r3, r0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2608:
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L2609+8
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2596:
	mov	r0, r4
	b	.L2585
.L2610:
	.align	2
.L2609:
	.word	.LANCHOR3-1976
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC343
	UNWIND(.fnend)
	.size	FindZeroBitsInSeg, .-FindZeroBitsInSeg
	.align	2
	.global	FindTrailZeros
	.type	FindTrailZeros, %function
FindTrailZeros:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	beq	.L2617
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2616
	ldr	r1, [r3, #40]
	ldr	r0, [r3, #36]
	bl	FindZeroBitsInSeg
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #40]
	cmp	r0, r2, asl #3
	bcs	.L2620
	cmn	r0, #1
	beq	.L2616
.L2615:
	add	r0, r0, #1
	str	r0, [r3, #72]
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2619
	ldr	r1, [r3, #40]
	ldr	r2, [r3, #72]
	cmp	r2, r1, asl #3
	bcs	.L2621
.L2619:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2621:
	add	r2, r4, #61440
	ldr	r1, [r3, #60]
	ldr	r0, [r2, #1136]
	bl	SM_ReleaseStreamSeg
	ldr	r2, [r4, #368]
	mov	r3, #0
	mov	r1, #1
	mov	r0, r3
	str	r3, [r2, #36]
	ldr	r2, [r4, #368]
	str	r1, [r2, #68]
	ldr	r2, [r4, #368]
	str	r3, [r2, #72]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2620:
	add	r2, r4, #61440
	ldr	r1, [r3, #60]
	ldr	r0, [r2, #1136]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #368]
	mov	r1, #0
	mov	r2, #1
	str	r1, [r3, #36]
	ldr	r3, [r4, #368]
	str	r2, [r3, #68]
	ldr	r3, [r4, #368]
.L2616:
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	bl	FindZeroBitsInSeg
	ldr	r3, [r4, #368]
	b	.L2615
.L2617:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	FindTrailZeros, .-FindTrailZeros
	.align	2
	.global	CombinePacket
	.type	CombinePacket, %function
CombinePacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, [r0, #368]
	ldr	r3, [r3, #12]
	cmp	r3, #4096
	ldmcsfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	CombinePacket.part.10
	UNWIND(.fnend)
	.size	CombinePacket, .-CombinePacket
	.align	2
	.global	FindNaluArraySlot
	.type	FindNaluArraySlot, %function
FindNaluArraySlot:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	ip, r0
	ldrb	r0, [r0, #377]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L2629
	mov	r2, ip
	mov	r3, #1
	b	.L2627
.L2626:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L2632
.L2627:
	ldrb	r1, [r2, #473]	@ zero_extendqisi2
	add	r2, r2, #96
	cmp	r1, #0
	bne	.L2626
	mov	r0, r3
.L2625:
	mov	r2, r3, asl #7
	mov	r1, #1
	sub	r3, r2, r3, asl #5
	add	ip, ip, r3
	strb	r1, [ip, #377]
	ldmfd	sp, {fp, sp, pc}
.L2632:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2629:
	mov	r3, r0
	b	.L2625
	UNWIND(.fnend)
	.size	FindNaluArraySlot, .-FindNaluArraySlot
	.align	2
	.global	InquireSliceExtProperty
	.type	InquireSliceExtProperty, %function
InquireSliceExtProperty:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	add	r0, r0, #13504
	add	r0, r0, #24
	mov	r1, #24
	bl	BsGet
	ldr	r3, [r4, #368]
	ubfx	r2, r0, #23, #1
	strb	r2, [r3, #3]
	ldr	r2, [r4, #368]
	ldrb	r3, [r2, #3]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r4, #57344
	ldreqb	r0, [r3, #3196]	@ zero_extendqisi2
	beq	.L2635
	ubfx	lr, r0, #16, #6
	ubfx	ip, r0, #15, #1
	ubfx	r1, r0, #12, #3
	strb	lr, [r3, #3197]
	strb	ip, [r3, #3198]
	ubfx	lr, r0, #8, #4
	ubfx	ip, r0, #5, #3
	strb	r1, [r3, #3199]
	strb	lr, [r3, #3200]
	ubfx	r1, r0, #4, #1
	ubfx	lr, r0, #3, #1
	strb	ip, [r3, #3201]
	strb	r1, [r3, #3202]
	ubfx	ip, r0, #2, #1
	and	r1, r0, #3
	strb	lr, [r3, #3203]
	ubfx	r0, r0, #22, #1
	strb	ip, [r3, #3204]
	strb	r1, [r3, #3205]
	strb	r0, [r3, #3196]
.L2635:
	cmp	r0, #1
	mov	r0, #0
	moveq	r3, #5
	streqb	r3, [r2, #2]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	InquireSliceExtProperty, .-InquireSliceExtProperty
	.align	2
	.global	InquireSliceProperty
	.type	InquireSliceProperty, %function
InquireSliceProperty:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #61440
	mov	r5, r2
	mov	r4, r0
	mov	r8, r1
	ldr	r2, [r3, #1080]
	mov	r0, #22
	ldr	r1, .L2646
	add	r6, r4, #20480
	sub	r2, r2, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r5]
	mov	r0, r4
	str	r3, [r8]
	ldr	r3, [r4, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r6, #211]
	ldr	r3, [r3, #76]
	str	r3, [r6, #240]
	bl	SliceCheck
	subs	r7, r0, #0
	bne	.L2641
	mov	r0, r4
	bl	ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L2645
	ldrb	r0, [r6, #213]	@ zero_extendqisi2
	add	r9, r4, #12288
	mov	r2, #2208
	mov	r3, #4000
	sub	r0, r0, #1
	add	r4, r4, #24576
	clz	r0, r0
	mov	r0, r0, lsr #5
	str	r0, [r8]
	ldr	r1, [r6, #220]
	ldr	r0, [r9, #1304]
	ldr	ip, [r9, #1300]
	ldr	lr, [r9, #1280]
	mla	r2, r2, r1, r0
	ldr	r0, [r9, #1276]
	ldr	r2, [r2, #28]
	mla	r3, r3, r2, ip
	ldrb	ip, [r3, #18]	@ zero_extendqisi2
	ldr	r6, [r3, #3252]
	rsb	r1, ip, #2
	ldr	r2, [r3, #3248]
	mla	r1, r6, r1, r1
	add	r2, r2, #1
	cmp	r0, r2
	cmpeq	lr, r1
	movne	r2, #1
	strne	r2, [r5]
	ldr	r2, [r3, #3272]
	ldr	r3, [r4, #2144]
	add	r3, r3, #1
	cmp	r2, r3
	movhi	r3, #1
	strhi	r3, [r5]
.L2638:
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2641:
	mvn	r7, #0
	b	.L2638
.L2645:
	ldr	r1, .L2646+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L2638
.L2647:
	.align	2
.L2646:
	.word	.LC344
	.word	.LC345
	UNWIND(.fnend)
	.size	InquireSliceProperty, .-InquireSliceProperty
	.align	2
	.global	HaveSliceToDec
	.type	HaveSliceToDec, %function
HaveSliceToDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r0, r0, #61440
	ldr	r0, [r0, #1060]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	HaveSliceToDec, .-HaveSliceToDec
	.align	2
	.global	IsRefListWrong
	.type	IsRefListWrong, %function
IsRefListWrong:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #20480
	mov	r7, r0
	ldr	r3, [r6, #252]
	cmp	r3, #0
	beq	.L2650
	add	r5, r0, #26880
	add	r8, r0, #61440
	add	r5, r5, #248
	mov	r4, #0
	b	.L2654
.L2651:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L2653
	ldr	r3, [r6, #252]
	cmp	r4, r3
	bcs	.L2676
.L2654:
	ldr	r3, [r5, #4]!
	add	r4, r4, #1
	ldr	r0, [r8, #1136]
	ldr	r3, [r3, #32]
	ldr	r1, [r3, #632]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L2651
.L2653:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2676:
	cmp	r3, #0
	beq	.L2650
	ldr	r3, [r6, #256]
	cmp	r3, #0
	addne	r7, r7, #27136
	movne	r4, #0
	addne	r7, r7, #124
	bne	.L2657
	b	.L2650
.L2677:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L2653
	ldr	r3, [r6, #256]
	cmp	r4, r3
	bcs	.L2650
.L2657:
	ldr	r3, [r7, #4]!
	add	r4, r4, #1
	ldr	r0, [r8, #1136]
	ldr	r3, [r3, #32]
	ldr	r1, [r3, #632]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L2677
	b	.L2653
.L2650:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	IsRefListWrong, .-IsRefListWrong
	.align	2
	.global	H264DEC_Destroy
	.type	H264DEC_Destroy, %function
H264DEC_Destroy:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r8, r0
	add	r5, r0, #61440
	bl	H264_ClearCurrPic
	mov	r0, r8
	bl	ClearAllNal
	ldr	r3, [r5, #1072]
	add	r9, r8, #57344
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [r5, #1072]
	ldr	r3, [r9, #3848]
	cmp	r3, #0
	beq	.L2680
	add	r6, r8, #61184
	mov	r4, #0
	add	r6, r6, #8
	mov	r7, r4
.L2682:
	ldr	r1, [r6, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L2681
	ldr	r0, [r5, #1136]
	bl	FreeUsdByDec
	str	r7, [r6]
.L2681:
	ldr	r3, [r9, #3848]
	cmp	r3, r4
	bhi	.L2682
.L2680:
	add	r9, r8, #27904
	add	r8, r8, #59648
	add	r9, r9, #88
	add	r8, r8, #24
	mov	r6, #0
	mov	r7, #0
.L2686:
	sub	r4, r9, #32
.L2684:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L2698
.L2683:
	cmp	r4, r9
	bne	.L2684
	add	r9, r4, #792
	cmp	r9, r8
	bne	.L2686
	ldr	r1, .L2699
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L2698:
	mov	r1, r2
	ldr	r0, [r5, #1136]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L2683
.L2700:
	.align	2
.L2699:
	.word	.LC346
	UNWIND(.fnend)
	.size	H264DEC_Destroy, .-H264DEC_Destroy
	.align	2
	.global	H264DEC_RecycleImage
	.type	H264DEC_RecycleImage, %function
H264DEC_RecycleImage:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r5, r0, #61440
	mov	r8, r1
	mov	r0, #2
	ldr	r1, .L2712
	bl	dprint_vfmw
	ldr	r0, [r5, #1136]
	mov	r1, r8
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L2706
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	beq	.L2706
	add	r4, r0, #560
	add	r9, r0, #592
	mov	r6, #0
	mov	r7, #0
.L2704:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L2711
.L2703:
	cmp	r4, r9
	bne	.L2704
	ldr	r0, [r5, #1136]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2711:
	mov	r1, r2
	ldr	r0, [r5, #1136]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L2703
.L2706:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2713:
	.align	2
.L2712:
	.word	.LC347
	UNWIND(.fnend)
	.size	H264DEC_RecycleImage, .-H264DEC_RecycleImage
	.align	2
	.global	OutputFrmToVO
	.type	OutputFrmToVO, %function
OutputFrmToVO:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	cmp	r1, #0
	cmpne	r0, #0
	mov	r6, r2
	mov	r4, r0
	moveq	ip, #1
	movne	ip, #0
	mov	r7, r1
	beq	.L2753
	ldr	r0, [r0]
	ldr	r0, [r0, #12]
	cmp	r0, #0
	ble	.L2717
	add	r3, r4, #61440
	ldr	r3, [r3, #1064]
	cmp	r3, #2
	beq	.L2717
	add	r4, r4, #57344
	ldr	r3, [r4, #3188]
	cmp	r3, #0
	strneb	ip, [r3, #1]
	movne	r0, #1
	strne	ip, [r4, #3188]
	beq	.L2719
.L2716:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2717:
	mov	r1, r7
	mov	r0, r4
	bl	CheckFrameStore
	cmn	r0, #3
	beq	.L2719
	add	r5, r4, #57344
	ldr	r1, [r5, #3188]
	cmp	r1, #0
	beq	.L2720
	mov	r0, r4
	bl	GetImagePara
	ldr	r1, [r5, #3188]
	mov	r0, r4
	bl	CheckFrameStore
	subs	r3, r0, #0
	beq	.L2754
	cmn	r3, #3
	beq	.L2727
	ldr	r1, .L2762
	movw	r2, #2046
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #3188]
	mov	r0, r4
	ldr	r1, [r3, #164]
	bl	H264DEC_RecycleImage
.L2727:
	ldr	r3, [r5, #3188]
	mov	r0, #0
	strb	r0, [r3, #1]
	ldr	r3, [r5, #3188]
	str	r0, [r5, #3188]
	cmp	r7, r3
	beq	.L2716
.L2720:
	mov	r1, r7
	mov	r0, r4
	bl	GetImagePara
	mov	r1, r7
	mov	r0, r4
	bl	CheckFrameStore
	subs	r3, r0, #0
	beq	.L2755
	cmn	r3, #3
	beq	.L2719
	movw	r2, #2104
	ldr	r1, .L2762
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r7, #164]
	mov	r0, r4
	bl	H264DEC_RecycleImage
	mov	r0, #2
	b	.L2716
.L2753:
	mov	r3, r1
	mov	r2, r0
	ldr	r1, .L2762+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2719:
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2755:
	add	r8, r4, #61440
	mov	r2, #1
	ldr	r1, [r7, #632]
	ldr	r0, [r8, #1136]
	bl	FSP_SetDisplay
	ldr	r0, [r8, #1136]
	ldr	r1, [r7, #632]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L2756
	cmp	r6, #1
	add	r3, r4, #8
	streq	r6, [r5, #188]
	mov	r2, r4
	ldr	r0, [r8, #1136]
	mov	r1, #0
	str	r5, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L2757
	ldr	r3, [r4]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r5, #24]
	bcs	.L2758
.L2734:
	ldr	r2, [r8, #1164]
	mov	r0, #0
	ldr	r3, [r8, #1156]
	add	r2, r2, #1
	str	r2, [r8, #1164]
	add	r3, r3, #2
	str	r3, [r8, #1156]
	b	.L2716
.L2754:
	ldr	r3, [r5, #3188]
	add	r8, r4, #61440
	mov	r2, #1
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	ldr	r3, [r5, #3188]
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L2759
	ldr	r3, [r5, #3188]
	mov	r2, r4
	mov	r1, #0
	cmp	r7, r3
	cmpeq	r6, #1
	moveq	r3, #1
	streq	r3, [r9, #188]
	add	r3, r4, #8
	ldr	r0, [r8, #1136]
	str	r9, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L2760
	ldr	r3, [r4]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r9, #24]
	bcs	.L2761
.L2726:
	ldr	r2, [r8, #1164]
	ldr	r3, [r8, #1156]
	add	r2, r2, #1
	str	r2, [r8, #1164]
	add	r3, r3, #2
	str	r3, [r8, #1156]
	b	.L2727
.L2758:
	mov	r1, r5
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L2734
.L2761:
	mov	r1, r9
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L2726
.L2757:
	mov	r2, r0
	ldr	r1, .L2762+8
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r7, #632]
	ldr	r0, [r8, #1136]
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, r4
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	b	.L2716
.L2760:
	mov	r2, r0
	ldr	r1, .L2762+8
	mov	r0, #0
	mov	r6, r0
	bl	dprint_vfmw
	ldr	r3, [r5, #3188]
	mov	r2, r6
	strb	r6, [r3, #1]
	ldr	r3, [r5, #3188]
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	str	r6, [r5, #3188]
	mov	r0, r4
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	b	.L2716
.L2756:
	movw	r2, #2074
	ldr	r1, .L2762+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2716
.L2759:
	movw	r2, #2017
	ldr	r1, .L2762+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2716
.L2763:
	.align	2
.L2762:
	.word	.LC351
	.word	.LC348
	.word	.LC350
	.word	.LC352
	.word	.LC349
	UNWIND(.fnend)
	.size	OutputFrmToVO, .-OutputFrmToVO
	.align	2
	.global	OutputFrmFromDPB
	.type	OutputFrmFromDPB, %function
OutputFrmFromDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r4, r0, r1, lsl #2
	mov	r6, r1
	add	r4, r4, #24576
	mov	r3, #0
	mov	r2, r3
	mov	r5, r0
	ldr	r1, [r4, #1952]
	strb	r3, [r1, #4]
	ldr	r1, [r4, #1952]
	bl	OutputFrmToVO
	ldr	r3, [r4, #1952]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r7, r0
	bne	.L2765
	mov	r1, r6
	mov	r0, r5
	bl	RemoveFrameStoreOutDPB
.L2765:
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	OutputFrmFromDPB, .-OutputFrmFromDPB
	.align	2
	.global	FlushDPB
	.type	FlushDPB, %function
FlushDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r6, r0, #24576
	mov	r4, r0
	ldr	r3, [r6, #2148]
	cmp	r3, #0
	beq	.L2768
	add	r7, r0, #26368
	mov	r5, #0
	add	r7, r7, #156
	b	.L2770
.L2769:
	ldr	r3, [r6, #2148]
	cmp	r3, r5
	bls	.L2768
.L2770:
	ldr	r1, [r7, #4]!
	add	r5, r5, #1
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2769
	mov	r0, r4
	bl	UnMarkFrameStoreRef
	ldr	r3, [r6, #2148]
	cmp	r3, r5
	bhi	.L2770
.L2768:
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L2772
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L2768
	b	.L2772
.L2774:
	bl	GetMinPOC
	ldr	r1, [fp, #-36]
	mov	r0, r4
	bl	OutputFrmFromDPB
	cmn	r0, #1
	beq	.L2773
.L2772:
	ldr	r3, [r6, #2148]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	bne	.L2774
	mov	r0, r3
.L2773:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	FlushDPB, .-FlushDPB
	.align	2
	.global	AdaptiveMemMark
	.type	AdaptiveMemMark, %function
AdaptiveMemMark:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r3, r0, #57344
	add	r7, r0, #58880
	add	r2, r0, #26368
	ldr	r4, [r3, #2380]
	add	r7, r7, #200
	mov	r5, r0
	str	r3, [fp, #-56]
	add	r6, r4, #2000
	add	r4, r4, #4
	add	r6, r6, #4
	add	r3, r2, #220
	str	r3, [fp, #-52]
	add	r3, r0, #16384
	str	r3, [fp, #-48]
.L2795:
	ldr	r3, [r4]
	cmp	r3, #6
	ldrls	pc, [pc, r3, asl #2]
	b	.L2783
.L2785:
	.word	.L2784
	.word	.L2786
	.word	.L2787
	.word	.L2788
	.word	.L2789
	.word	.L2790
	.word	.L2791
.L2791:
	ldr	r2, [r4, #12]
	mov	r1, r7
	mov	r0, r5
	bl	MarkCurrPicLT
.L2783:
	add	r4, r4, #20
	cmp	r4, r6
	bne	.L2795
.L2784:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #3444]
	cmp	r3, #1
	movne	r0, #0
	beq	.L2804
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2790:
	add	r8, r5, #24576
	ldr	r3, [r8, #2152]
	cmp	r3, #0
	ldrne	r9, [fp, #-52]
	movne	r10, #0
	beq	.L2794
.L2793:
	ldr	r1, [r9, #4]!
	mov	r0, r5
	bl	UnMarkFrameStoreRef
	ldr	r1, [r8, #2152]
	add	r10, r10, #1
	cmp	r10, r1
	bcc	.L2793
.L2794:
	mov	r0, r5
	bl	UpdateReflist
	mov	r1, #0
	mov	r0, r5
	bl	UpdateMaxLTFrmIdx
	mov	r0, r5
	bl	UpdateLTReflist
	ldr	r2, [fp, #-48]
	mov	r3, #1
	str	r3, [r2, #3444]
	b	.L2783
.L2789:
	mov	r0, r5
	ldr	r1, [r4, #16]
	bl	UpdateMaxLTFrmIdx
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2783
.L2788:
	ldr	r3, [r4, #12]
	mov	r1, r7
	ldr	r2, [r4, #4]
	mov	r0, r5
	bl	MarkSTToLTRef
	mov	r0, r5
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2783
.L2787:
	mov	r0, r5
	ldr	r2, [r4, #8]
	mov	r1, r7
	bl	UnMarkLTRef
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2783
.L2786:
	mov	r0, r5
	ldr	r2, [r4, #4]
	mov	r1, r7
	bl	UnMarkSTRef
	mov	r0, r5
	bl	UpdateReflist
	b	.L2783
.L2804:
	ldr	r1, [fp, #-56]
	mov	r3, #0
	ldrb	r2, [r1, #1739]	@ zero_extendqisi2
	str	r3, [r1, #2384]
	cmp	r2, #1
	beq	.L2798
	bcc	.L2799
	cmp	r2, #2
	bne	.L2797
	ldr	r2, [fp, #-56]
	str	r3, [r2, #2408]
	str	r3, [r2, #2396]
	ldr	r2, [fp, #-48]
	str	r3, [r2, #3456]
	str	r3, [r2, #3464]
.L2797:
	mov	r0, r5
	bl	FlushDPB
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2799:
	ldr	r0, [fp, #-56]
	ldr	r1, [r0, #2396]
	ldr	r2, [r0, #2404]
	ldr	r3, [r0, #2408]
	rsb	r2, r1, r2
	str	r2, [r0, #2404]
	rsb	r3, r1, r3
	str	r3, [r0, #2408]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r0, #2400]
	str	r3, [r0, #2396]
	ldr	r0, [fp, #-48]
	ldr	r1, [r0, #3464]
	ldr	r2, [r0, #3452]
	ldr	r3, [r0, #3456]
	rsb	r2, r1, r2
	str	r2, [r0, #3452]
	rsb	r3, r1, r3
	str	r3, [r0, #3456]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r0, #3460]
	str	r3, [r0, #3464]
	b	.L2797
.L2798:
	ldr	r2, [fp, #-56]
	str	r3, [r2, #2404]
	str	r3, [r2, #2396]
	ldr	r2, [fp, #-48]
	str	r3, [r2, #3452]
	str	r3, [r2, #3464]
	b	.L2797
	UNWIND(.fnend)
	.size	AdaptiveMemMark, .-AdaptiveMemMark
	.align	2
	.global	IDRMemMarking
	.type	IDRMemMarking, %function
IDRMemMarking:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r3, r0, #57344
	str	r3, [fp, #-52]
	ldr	r3, [r3, #2380]
	str	r0, [fp, #-48]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2806
	add	r9, r0, #24576
	ldr	r3, [r9, #2148]
	cmp	r3, #0
	beq	.L2813
	ldr	r3, [fp, #-48]
	mov	r10, #0
	mov	r4, r10
	add	r8, r3, #26368
	add	r6, r3, #26624
	add	r6, r6, #28
	add	r5, r3, #61440
	mov	r7, r8
	add	r8, r8, #160
	add	r7, r7, #220
.L2812:
	ldr	r1, [r8]
	mov	r2, #0
	add	r10, r10, #1
	ldr	r0, [r1, #784]
	cmp	r0, #1
	ldreq	r3, [fp, #-52]
	streq	r0, [r3, #3836]
	strb	r4, [r1, #678]
	strb	r4, [r1, #677]
	ldr	r1, [r8]
	strb	r4, [r1, #714]
	strb	r4, [r1, #713]
	ldr	r1, [r8]
	strb	r4, [r1, #750]
	strb	r4, [r1, #749]
	ldr	r1, [r8]
	strb	r4, [r1, #2]
	ldr	r1, [r8]
	ldr	r0, [r5, #1136]
	ldr	r1, [r1, #632]
	bl	FSP_SetRef
	ldr	r2, [r8]
	mov	r3, #1
	strb	r4, [r2, #1]
	ldr	r2, [r8]
	strb	r4, [r2, #4]
	ldr	r2, [r8]
	str	r3, [r2, #652]
	ldr	r1, [r8]
	ldr	r3, [fp, #-48]
	ldr	r2, [r1, #672]
	add	r2, r2, #6656
	add	r2, r2, #28
	add	r2, r3, r2, lsl #2
	str	r4, [r2, #4]
	ldr	r1, [r1, #632]
	ldr	r0, [r5, #1136]
	bl	FSP_GetDisplay
	mov	r2, #0
	cmp	r0, #3
	beq	.L2811
	ldr	r1, [r8]
	ldr	r0, [r5, #1136]
	ldr	r1, [r1, #632]
	bl	FSP_SetDisplay
.L2811:
	str	r4, [r8], #4
	str	r4, [r7, #4]!
	str	r4, [r6, #4]!
	ldr	r2, [r9, #2148]
	cmp	r2, r10
	bhi	.L2812
.L2813:
	ldr	r2, [fp, #-52]
	mov	r3, #0
	str	r3, [r9, #2148]
	ldr	r1, [r2, #3824]
	cmp	r1, r3
	beq	.L2809
	ldr	r2, [fp, #-48]
	mov	r0, r3
	add	r2, r2, #60928
	add	r2, r2, #164
.L2816:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L2816
.L2809:
	ldr	r4, [fp, #-48]
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	ldr	r3, [fp, #-52]
	ldr	r3, [r3, #2380]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2826
	ldr	r2, [fp, #-52]
	mov	r0, r3
	str	r3, [r9, #2160]
	strb	r3, [r2, #1740]
	mov	r3, #1
	strb	r3, [r2, #1741]
.L2817:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2826:
	ldr	r1, [fp, #-52]
	mov	r3, #0
	mov	r2, #1
	mov	r0, r3
	str	r2, [r9, #2160]
	strb	r2, [r1, #1740]
	str	r3, [r1, #2388]
	strb	r3, [r1, #1741]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2806:
	bl	FlushDPB
	cmp	r0, #0
	ldreq	r3, [fp, #-48]
	addeq	r9, r3, #24576
	beq	.L2809
.L2818:
	mvn	r0, #0
	b	.L2817
	UNWIND(.fnend)
	.size	IDRMemMarking, .-IDRMemMarking
	.align	2
	.global	Marking
	.type	Marking, %function
Marking:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r6, r0, #16384
	add	r4, r0, #57344
	mov	r3, #0
	mov	r5, r0
	str	r3, [r6, #3444]
	ldrb	r3, [r4, #1739]	@ zero_extendqisi2
	sub	r3, r3, #2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r6, #3448]
	ldrb	r3, [r4, #1742]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L2828
	ldr	r3, [r4, #2412]
	cmp	r3, #0
	bne	.L2829
	ldr	r3, [r0]
	ldr	r2, [r3, #640]
	cmp	r2, #1
	beq	.L2853
.L2829:
	ldr	r3, [r4, #2380]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r4, #2392]
	beq	.L2854
	cmp	r3, #0
	bne	.L2855
.L2836:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2854:
	cmp	r3, #0
	beq	.L2836
.L2835:
	ldrb	r2, [r4, #1737]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2840
	add	r2, r5, #24576
	ldr	r1, [r6, #456]
	ldr	ip, [r2, #2156]
	ldr	r0, [r2, #2152]
	rsb	r2, ip, r1
	cmp	r0, r2
	beq	.L2856
.L2841:
	mov	r2, #0
	strb	r2, [r4, #1740]
.L2838:
	cmp	r3, #0
	beq	.L2836
.L2840:
	ldrb	r3, [r4, #1740]	@ zero_extendqisi2
	mov	r0, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r4, #1741]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2853:
	ldr	r2, [r3, #636]
	cmp	r2, #0
	bne	.L2829
	ldr	r2, [r3, #644]
	cmp	r2, #0
	bne	.L2829
	ldr	r3, [r3, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	bls	.L2829
.L2828:
	mov	r0, r5
	bl	IDRMemMarking
	cmp	r0, #0
	bne	.L2830
.L2837:
	ldrb	r3, [r4, #1742]	@ zero_extendqisi2
	cmp	r3, #5
	ldreq	r3, [r4, #2392]
	beq	.L2838
	ldr	r3, [r4, #2392]
	cmp	r3, #0
	beq	.L2836
	ldr	r2, [r4, #2380]
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2835
	b	.L2840
.L2855:
	mov	r0, r5
	bl	AdaptiveMemMark
	cmp	r0, #0
	beq	.L2837
.L2830:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2856:
	mov	r0, r5
	bl	SlidingWinMark.part.2
	ldr	r3, [r4, #2392]
	b	.L2841
	UNWIND(.fnend)
	.size	Marking, .-Marking
	.align	2
	.global	DirectOutput
	.type	DirectOutput, %function
DirectOutput:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r4, r0, #57344
	mov	r5, r0
	ldrb	r6, [r4, #1739]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L2859
	bcc	.L2860
	cmp	r6, #2
	beq	.L2861
	mov	r6, #0
.L2858:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2861:
	ldr	r3, [r4, #2376]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r2, [r4, #1737]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2949
	ldr	ip, [r4, #3188]
	add	r1, r4, #1744
	add	r1, r1, #8
	mov	r2, #624
	cmp	ip, #0
	ldreq	r3, [r4, #2376]
	streq	r3, [r4, #3188]
	streq	r3, [r3, #780]
	ldreq	ip, [r4, #3188]
	add	r3, ip, #624
	add	r0, ip, #8
	ldr	r10, [ip, #612]
	ldrd	r6, [r3, #-8]
	ldr	r9, [ip, #16]
	ldr	r8, [ip, #20]
	strd	r6, [fp, #-52]
	ldrd	r6, [r3]
	ldr	r3, [ip, #608]
	strd	r6, [fp, #-60]
	str	r3, [fp, #-64]
	ldr	r7, [ip, #24]
	ldr	r6, [ip, #28]
	bl	memcpy
	ldr	r3, [fp, #-64]
	add	r0, r5, #59648
	movw	r2, #59696
	ldr	ip, [r5, r2]
	and	r2, r3, r10
	ldr	r1, [r0, #52]
	adds	r2, r2, #1
	and	r1, r1, ip
	movne	r2, #1
	cmn	r1, #1
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r4, #3188]
	strne	r3, [r2, #608]
	add	r3, r5, #59648
	strne	r10, [r2, #612]
	add	r3, r3, #64
	ldr	r1, [r4, #3188]
	ldrd	r2, [r3, #-8]
	add	ip, r1, #624
	ldrd	r0, [fp, #-52]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r2, r0
	movw	r0, #59712
	movhi	r3, r1
	strd	r2, [ip, #-8]
	ldrd	r2, [r5, r0]
	ldrd	r0, [fp, #-60]
	ldr	ip, [r4, #3188]
	cmp	r3, r1
	cmpeq	r2, r0
	add	r1, ip, #624
	ldrhid	r2, [fp, #-60]
.L2889:
	strd	r2, [r1]
	mov	ip, #2
	ldr	r3, [r4, #1760]
	mov	r0, #3
	ldr	r2, [r4, #3188]
	cmp	r3, r9
	movcs	r3, r9
	str	r3, [r2, #16]
	ldr	r3, [r4, #1764]
	ldr	r2, [r4, #3188]
	cmp	r3, r8
	movcs	r3, r8
	str	r3, [r2, #20]
	ldr	r3, [r4, #1768]
	ldr	r2, [r4, #3188]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #24]
	ldr	r3, [r4, #1772]
	ldr	r2, [r4, #3188]
	cmp	r3, r6
	movcs	r3, r6
	str	r3, [r2, #28]
	ldr	r2, [r4, #3188]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #1738]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r2, [r4, #3188]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #1]
	ldr	r2, [r4, #3188]
	ldr	r1, [r4, #2412]
	ldr	r3, [r2, #660]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #660]
	ldr	r3, [r4, #3188]
	strb	ip, [r3, #748]
	ldr	r3, [r4, #3188]
	strb	r0, [r3, #676]
	ldr	r3, [r4, #3188]
	ldr	r2, [r4, #2420]
	str	r2, [r3, #764]
	ldr	r2, [r4, #3188]
	ldr	r3, [r2, #764]
	ldr	r1, [r2, #728]
	add	r3, r3, r1
	mov	r3, r3, lsr #1
	str	r3, [r2, #692]
	ldr	r3, [r4, #3188]
	ldr	r2, [r3, #692]
	str	r2, [r3, #664]
	ldr	r3, [r4, #3188]
	ldr	r2, [r3, #780]
	str	r2, [r3, #708]
	ldrb	r3, [r4, #1743]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #3188]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	beq	.L2946
.L2892:
	strb	r2, [r3, #3]
	add	r7, r5, #61440
	ldr	r3, [r4, #3188]
	mov	r2, #0
	ldr	r0, [r7, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r0, r5
	mov	r2, #0
	ldr	r1, [r4, #3188]
	bl	OutputFrmToVO
	ldr	r3, [r4, #3188]
	cmp	r3, #0
	mov	r6, r0
	beq	.L2893
	ldr	r3, [r4, #2376]
	mov	r2, #1
	ldr	r0, [r7, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	b	.L2893
.L2860:
	ldr	r3, [r4, #2376]
	mov	r2, #0
	strb	r2, [r3, #676]
	ldr	r1, [r4, #3188]
	cmp	r1, r2
	beq	.L2940
	bl	OutputFrmToVO
	cmp	r0, #0
	bne	.L2950
.L2863:
	ldr	r3, [r4, #3188]
	cmp	r3, #0
	beq	.L2940
	ldr	r3, [r4, #2376]
	add	r8, r5, #61440
	mov	r2, #1
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
.L2862:
	ldr	r3, [r4, #2376]
	mov	r0, #3
	mov	r7, #0
	add	r1, r4, #1744
	add	r1, r1, #8
	mov	r2, #624
	str	r3, [r4, #3188]
	str	r3, [r3, #708]
	ldr	r3, [r4, #3188]
	ldr	ip, [r4, #2376]
	str	ip, [r3, #744]
	ldr	r3, [r4, #3188]
	ldr	ip, [r4, #2376]
	str	ip, [r3, #780]
	ldrb	ip, [r4, #1738]	@ zero_extendqisi2
	ldr	r3, [r4, #3188]
	strb	ip, [r3]
	ldr	r3, [r4, #3188]
	strb	r0, [r3, #1]
	ldr	r3, [r4, #3188]
	strb	r7, [r3, #2]
	ldr	r3, [r4, #3188]
	strb	r7, [r3, #4]
	ldr	r3, [r4, #3188]
	str	r7, [r3, #652]
	ldr	r0, [r4, #3188]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r4, #3188]
	ldr	r1, [r4, #2420]
	mov	r2, r7
	str	r1, [r3, #692]
	str	r1, [r3, #664]
	ldr	r3, [r4, #3188]
	strb	r7, [r3, #676]
	ldr	r3, [r4, #3188]
	ldr	r1, [r4, #2412]
	str	r1, [r3, #660]
	ldr	r3, [r4, #3188]
	ldrb	r1, [r4, #1745]	@ zero_extendqisi2
	str	r1, [r3, #656]
	ldrb	r1, [r4, #1743]	@ zero_extendqisi2
	ldr	r3, [r4, #3188]
	cmp	r1, #1
	moveq	r6, #3
	strb	r6, [r3, #3]
	ldr	r3, [r4, #3188]
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r2, r7
	mov	r0, r5
	ldr	r1, [r4, #3188]
	bl	OutputFrmToVO
	ldr	r3, [r4, #3188]
	cmp	r3, r7
	mov	r6, r0
	beq	.L2893
	ldr	r3, [r4, #2376]
	mov	r2, #1
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
.L2893:
	mov	r3, #0
	mov	r0, r6
	str	r3, [r4, #3188]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2859:
	ldr	r3, [r4, #2376]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r2, [r4, #1737]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2867
	ldr	r1, [r4, #3188]
	cmp	r1, #0
	beq	.L2868
	bl	OutputFrmToVO
	subs	r6, r0, #0
	bne	.L2951
.L2869:
	ldr	r3, [r4, #2376]
	str	r3, [r4, #3188]
	str	r3, [r3, #744]
.L2870:
	ldr	r3, [r4, #3188]
	mov	r9, #1
	ldrb	r0, [r4, #1738]	@ zero_extendqisi2
	mov	r7, #0
	add	r1, r4, #1744
	mov	r2, #624
	add	r1, r1, #8
	add	r8, r5, #61440
	strb	r0, [r3]
	ldr	r3, [r4, #3188]
	strb	r9, [r3, #1]
	ldr	r3, [r4, #3188]
	strb	r7, [r3, #4]
	ldr	r3, [r4, #3188]
	str	r7, [r3, #652]
	ldr	r3, [r4, #3188]
	strb	r7, [r3, #2]
	ldr	r3, [r4, #3188]
	ldr	r0, [r4, #2412]
	str	r0, [r3, #660]
	ldr	r3, [r4, #3188]
	str	r7, [r3, #656]
	ldr	r0, [r4, #3188]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r4, #3188]
	ldr	r2, [r4, #2420]
	str	r2, [r3, #728]
	ldr	r3, [r4, #3188]
	ldr	r2, [r3, #728]
	str	r2, [r3, #692]
	str	r2, [r3, #664]
	ldr	r3, [r4, #3188]
	strb	r9, [r3, #712]
	ldrb	r3, [r4, #1743]	@ zero_extendqisi2
	ldr	r2, [r4, #3188]
	rsb	r3, r9, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r2, #3]
	ldr	r9, [r8, #1068]
	cmp	r9, #1
	bne	.L2858
.L2944:
	ldr	r3, [r4, #3188]
	mov	r2, r7
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r2, r7
	mov	r0, r5
	ldr	r1, [r4, #3188]
	bl	OutputFrmToVO
	ldr	r3, [r4, #3188]
	cmp	r3, r7
	mov	r6, r0
	beq	.L2893
	ldr	r3, [r4, #2376]
	mov	r2, r9
	ldr	r0, [r8, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	b	.L2893
.L2949:
	ldr	r1, [r4, #3188]
	cmp	r1, #0
	beq	.L2881
	bl	OutputFrmToVO
	subs	r6, r0, #0
	bne	.L2952
.L2882:
	ldr	r3, [r4, #2376]
	str	r3, [r4, #3188]
	str	r3, [r3, #780]
.L2883:
	ldr	r3, [r4, #3188]
	mov	r9, #2
	ldrb	r0, [r4, #1738]	@ zero_extendqisi2
	mov	r7, #0
	add	r1, r4, #1744
	mov	r2, #624
	add	r1, r1, #8
	add	r8, r5, #61440
	strb	r0, [r3]
	ldr	r3, [r4, #3188]
	strb	r9, [r3, #1]
	ldr	r3, [r4, #3188]
	strb	r7, [r3, #4]
	ldr	r3, [r4, #3188]
	str	r7, [r3, #652]
	ldr	r3, [r4, #3188]
	strb	r7, [r3, #2]
	ldr	r3, [r4, #3188]
	ldr	r0, [r4, #2412]
	str	r0, [r3, #660]
	ldr	r3, [r4, #3188]
	str	r7, [r3, #656]
	ldr	r0, [r4, #3188]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r4, #3188]
	ldr	r2, [r4, #2420]
	str	r2, [r3, #764]
	ldr	r3, [r4, #3188]
	ldr	r2, [r3, #764]
	str	r2, [r3, #692]
	str	r2, [r3, #664]
	ldr	r3, [r4, #3188]
	strb	r9, [r3, #748]
	ldrb	r2, [r4, #1743]	@ zero_extendqisi2
	ldr	r3, [r4, #3188]
	cmp	r2, #1
	moveq	r2, r9
	movne	r2, r7
	strb	r2, [r3, #3]
	ldr	r9, [r8, #1068]
	cmp	r9, #1
	beq	.L2944
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2867:
	ldr	ip, [r4, #3188]
	add	r1, r4, #1744
	add	r1, r1, #8
	mov	r2, #624
	cmp	ip, #0
	ldreq	r3, [r4, #2376]
	streq	r3, [r4, #3188]
	streq	r3, [r3, #744]
	ldreq	ip, [r4, #3188]
	add	r3, ip, #624
	add	r0, ip, #8
	ldr	r10, [ip, #612]
	ldrd	r6, [r3, #-8]
	ldr	r9, [ip, #16]
	ldr	r8, [ip, #20]
	strd	r6, [fp, #-52]
	ldrd	r6, [r3]
	ldr	r3, [ip, #608]
	strd	r6, [fp, #-60]
	str	r3, [fp, #-64]
	ldr	r7, [ip, #24]
	ldr	r6, [ip, #28]
	bl	memcpy
	ldr	r3, [fp, #-64]
	add	r0, r5, #59648
	movw	r2, #59696
	ldr	ip, [r5, r2]
	and	r2, r3, r10
	ldr	r1, [r0, #52]
	adds	r2, r2, #1
	and	r1, r1, ip
	movne	r2, #1
	cmn	r1, #1
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r4, #3188]
	strne	r3, [r2, #608]
	add	r3, r5, #59648
	strne	r10, [r2, #612]
	add	r3, r3, #64
	ldr	r1, [r4, #3188]
	ldrd	r2, [r3, #-8]
	add	ip, r1, #624
	ldrd	r0, [fp, #-52]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r2, r0
	movw	r0, #59712
	movhi	r3, r1
	strd	r2, [ip, #-8]
	ldrd	r2, [r5, r0]
	ldrd	r0, [fp, #-60]
	ldr	ip, [r4, #3188]
	cmp	r3, r1
	cmpeq	r2, r0
	add	r1, ip, #624
	ldrhid	r2, [fp, #-60]
.L2875:
	strd	r2, [r1]
	mov	ip, #1
	ldr	r3, [r4, #1760]
	mov	r0, #3
	ldr	r2, [r4, #3188]
	cmp	r3, r9
	movcs	r3, r9
	str	r3, [r2, #16]
	ldr	r3, [r4, #1764]
	ldr	r2, [r4, #3188]
	cmp	r3, r8
	movcs	r3, r8
	str	r3, [r2, #20]
	ldr	r3, [r4, #1768]
	ldr	r2, [r4, #3188]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #24]
	ldr	r3, [r4, #1772]
	ldr	r2, [r4, #3188]
	cmp	r3, r6
	movcs	r3, r6
	str	r3, [r2, #28]
	ldr	r2, [r4, #3188]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #1738]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r2, [r4, #3188]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #1]
	ldr	r2, [r4, #3188]
	ldr	r1, [r4, #2412]
	ldr	r3, [r2, #660]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #660]
	ldr	r3, [r4, #3188]
	strb	ip, [r3, #712]
	ldr	r3, [r4, #3188]
	strb	r0, [r3, #676]
	ldr	r3, [r4, #3188]
	ldr	r2, [r4, #2420]
	str	r2, [r3, #728]
	ldr	r2, [r4, #3188]
	ldr	r3, [r2, #764]
	ldr	r1, [r2, #728]
	add	r3, r3, r1
	mov	r3, r3, lsr ip
	str	r3, [r2, #692]
	ldr	r3, [r4, #3188]
	ldr	r2, [r3, #692]
	str	r2, [r3, #664]
	ldr	r3, [r4, #3188]
	ldr	r2, [r3, #744]
	str	r2, [r3, #708]
	ldrb	r3, [r4, #1743]	@ zero_extendqisi2
	cmp	r3, ip
	ldr	r3, [r4, #3188]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bne	.L2892
.L2946:
	orr	r2, r2, ip
	b	.L2892
.L2940:
	add	r8, r5, #61440
	b	.L2862
.L2950:
	str	r0, [sp]
	mov	r3, #2704
	ldr	r2, .L2953
	mov	r0, #22
	ldr	r1, .L2953+4
	bl	dprint_vfmw
	b	.L2863
.L2881:
	ldr	r3, [r4, #2376]
	mov	r6, r1
	str	r3, [r4, #3188]
	str	r3, [r3, #780]
	b	.L2883
.L2868:
	ldr	r3, [r4, #2376]
	mov	r6, r1
	str	r3, [r4, #3188]
	str	r3, [r3, #744]
	b	.L2870
.L2952:
	str	r6, [sp]
	movw	r3, #2898
	ldr	r2, .L2953
	mov	r0, #22
	ldr	r1, .L2953+4
	bl	dprint_vfmw
	b	.L2882
.L2951:
	str	r6, [sp]
	movw	r3, #2756
	ldr	r2, .L2953
	mov	r0, #22
	ldr	r1, .L2953+4
	bl	dprint_vfmw
	b	.L2869
.L2954:
	.align	2
.L2953:
	.word	.LANCHOR1+228
	.word	.LC353
	UNWIND(.fnend)
	.size	DirectOutput, .-DirectOutput
	.align	2
	.global	H264DEC_GetRemainImg
	.type	H264DEC_GetRemainImg, %function
H264DEC_GetRemainImg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r5, r0, #24576
	mov	r4, r0
	ldr	r3, [r5, #2148]
	cmp	r3, #0
	beq	.L2957
	add	r7, r0, #26368
	mov	r6, #0
	add	r7, r7, #156
	b	.L2959
.L2958:
	ldr	r3, [r5, #2148]
	cmp	r3, r6
	bls	.L2957
.L2959:
	ldr	r1, [r7, #4]!
	add	r6, r6, #1
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2958
	mov	r0, r4
	bl	UnMarkFrameStoreRef
	ldr	r3, [r5, #2148]
	cmp	r3, r6
	bhi	.L2959
.L2957:
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L2957
	mvn	r6, #0
.L2961:
	ldr	r3, [r5, #2148]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	beq	.L2973
	bl	GetMinPOC
	ldr	r3, [fp, #-36]
	mov	r1, #0
	mov	r0, r4
	add	r3, r4, r3, lsl #2
	add	r3, r3, #24576
	ldr	r2, [r3, #1952]
	strb	r1, [r2, #4]
	ldr	r1, [r3, #1952]
	ldr	r2, [r5, #2148]
	bl	OutputFrmToVO
	ldr	r1, [fp, #-36]
	add	r3, r1, #6592
	add	r3, r3, #40
	ldr	r3, [r4, r3, asl #2]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r6, r0
	bne	.L2961
	mov	r0, r4
	bl	RemoveFrameStoreOutDPB
	b	.L2961
.L2973:
	cmp	r6, #0
	beq	.L2964
	add	r0, r4, #8
	bl	GetVoLastImageID
	mov	r6, r0
.L2964:
	mov	r0, r6
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	H264DEC_GetRemainImg, .-H264DEC_GetRemainImg
	.align	2
	.global	InitDPB
	.type	InitDPB, %function
InitDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	subs	r6, r0, #0
	beq	.L2989
	bl	ClearAllSlice
	add	r7, r6, #26368
	add	r5, r6, #26624
	add	r9, r6, #61440
	mov	r4, r7
	add	r5, r5, #32
	add	r7, r7, #160
	add	r4, r4, #224
	mov	r8, #0
.L2979:
	ldr	r3, [r7]
	mov	r2, #1
	add	r8, r8, r2
	cmp	r3, #0
	beq	.L2976
	ldr	r1, [r3, #632]
	ldr	r0, [r9, #1136]
	bl	FSP_ClearLogicFs
.L2976:
	ldr	r3, [r4]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2977
	ldr	r1, [r3, #632]
	ldr	r0, [r9, #1136]
	bl	FSP_ClearLogicFs
.L2977:
	ldr	r3, [r5]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2978
	ldr	r1, [r3, #632]
	ldr	r0, [r9, #1136]
	bl	FSP_ClearLogicFs
.L2978:
	cmp	r8, #16
	mov	r2, #0
	str	r2, [r5], #4
	str	r2, [r4], #4
	str	r2, [r7], #4
	bne	.L2979
	add	r3, r6, #16384
	add	r4, r6, #24576
	add	r5, r6, #57344
	ldr	r3, [r3, #484]
	str	r2, [r4, #2148]
	sub	r3, r3, #1
	str	r2, [r4, #2152]
	str	r3, [r4, #2144]
	str	r2, [r4, #2156]
	str	r2, [r4, #2160]
	ldr	r1, [r5, #3188]
	cmp	r1, r2
	beq	.L2981
	add	r7, r6, #61440
	ldr	r3, [r7, #1064]
	cmp	r3, #2
	beq	.L3012
.L2982:
	ldr	r1, [r1, #632]
	mov	r2, #1
	ldr	r0, [r7, #1136]
	bl	FSP_ClearLogicFs
	ldr	r3, [r5, #2376]
	cmp	r3, #0
	beq	.L2985
	ldr	r2, [r5, #3188]
	ldr	r1, [r3, #632]
	ldr	r3, [r2, #632]
	cmp	r3, r1
	beq	.L2985
	ldr	r0, [r7, #1136]
	mov	r2, #1
	bl	FSP_ClearLogicFs
.L2985:
	mov	r3, #0
	str	r3, [r5, #3188]
.L2981:
	ldr	r3, .L3014
	add	r0, r6, #26624
	mov	r2, #392
	mov	r1, #0
	add	r0, r0, #116
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r1, [r4, #2144]
	add	r3, r6, #60928
	mov	r0, #0
	str	r0, [r4, #2424]
	mov	r2, r3
	add	r3, r3, #164
	add	r2, r2, #232
	str	r1, [r4, #2420]
	mov	r1, r0
.L2986:
	str	r1, [r3, #4]!
	cmp	r3, r2
	bne	.L2986
	add	r2, r6, #26880
	add	r3, r6, #27136
	add	r2, r2, #248
	add	r3, r3, #124
	mov	r0, #0
.L2987:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L2987
	add	ip, r6, #20480
	add	r6, r6, #27392
	mov	r2, r1
	str	r1, [ip, #252]
.L2988:
	str	r2, [r3, #4]!
	cmp	r3, r6
	mov	r0, #0
	bne	.L2988
	str	r0, [ip, #256]
.L2975:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3012:
	mov	r0, r6
	bl	OutputFrmToVO
	cmp	r0, #0
	bne	.L3013
.L2983:
	ldr	r1, [r5, #3188]
	cmp	r1, #0
	beq	.L2981
	b	.L2982
.L3013:
	str	r0, [sp]
	movw	r3, #5669
	ldr	r2, .L3014+4
	mov	r0, #22
	ldr	r1, .L3014+8
	bl	dprint_vfmw
	b	.L2983
.L2989:
	mvn	r0, #0
	b	.L2975
.L3015:
	.align	2
.L3014:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+244
	.word	.LC353
	UNWIND(.fnend)
	.size	InitDPB, .-InitDPB
	.align	2
	.global	ClearDPB
	.type	ClearDPB, %function
ClearDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r4, r0
	bl	FlushDPB
	subs	r3, r0, #0
	beq	.L3017
	movw	r2, #501
	ldr	r1, .L3021
	mov	r0, #0
	bl	dprint_vfmw
.L3017:
	mov	r0, r4
	bl	InitDPB
	add	r2, r4, #57344
	add	r4, r4, #61440
	mov	r3, #0
	mov	r0, r3
	str	r3, [r2, #3836]
	str	r3, [r4, #1140]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3022:
	.align	2
.L3021:
	.word	.LC354
	UNWIND(.fnend)
	.size	ClearDPB, .-ClearDPB
	.align	2
	.global	DecVDM
	.type	DecVDM, %function
DecVDM:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r6, r0, #57344
	mov	r7, r0
	add	r4, r0, #61440
	ldrb	r3, [r6, #1739]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3024
	ldrb	r3, [r6, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3024
.L3025:
	ldr	r5, [r4, #1124]
	ldr	r3, [r4, #1064]
	ubfx	r5, r5, #16, #3
	cmp	r3, r5
	beq	.L3026
	subs	r2, r5, #2
	sub	r3, r3, #2
	movne	r2, #1
	cmp	r3, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L3026
	cmp	r5, #4
	cmpne	r5, #1
	bls	.L3117
.L3026:
	cmp	r5, #2
	str	r5, [r4, #1064]
	beq	.L3029
	cmp	r5, #3
	beq	.L3030
	cmp	r5, #1
	beq	.L3118
	ldr	r3, [r4, #1060]
	cmp	r3, #0
	beq	.L3033
	cmp	r5, #4
	ldr	r3, [r6, #2412]
	beq	.L3119
.L3040:
	cmp	r3, #2
	beq	.L3057
.L3044:
	cmp	r3, #1
	streq	r3, [r6, #3840]
.L3046:
	ldr	r2, [r4, #1100]
	mov	r0, #2
	ldrb	r3, [r6, #1739]	@ zero_extendqisi2
	sub	r2, r2, #1
	ldr	r1, .L3124
	bl	dprint_vfmw
	ldr	r2, [r6, #2436]
	mov	r3, #1
	str	r2, [r4, #580]
	strb	r3, [r6, #3874]
	ldr	r1, [r4, #1100]
	ldr	r2, [r6, #2440]
	ldr	r3, [r4, #1060]
	cmp	r1, #0
	str	r2, [r4, #576]
	str	r3, [r4, #780]
	beq	.L3048
	ldr	r1, [r6, #2412]
	cmp	r1, #0
	beq	.L3120
	add	r3, r7, #24576
	ldr	r3, [r3, #2148]
	cmp	r3, #0
	bne	.L3050
	ldr	r2, [r7]
	ldr	r1, [r2, #12]
	cmp	r1, #2
	bne	.L3121
.L3053:
	add	r3, r7, #61184
	add	r3, r3, #32
	str	r3, [r7, #360]
	ldr	r1, [r2, #864]
	cmp	r1, #1
	beq	.L3122
	cmp	r1, #2
	bne	.L3056
	ldr	r1, [r2, #868]
	cmp	r3, #0
	cmpne	r1, #0
	ble	.L3056
	ldr	r3, [r6, #2412]
	cmp	r3, #2
	bne	.L3056
	mov	r2, #0
	str	r2, [r7, #360]
	ldr	r3, [r6, #2376]
	ldr	r0, [r4, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	ldr	r2, [r7]
	mov	r0, r7
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3109
.L3024:
	ldr	r3, [r4, #1160]
	ldr	r2, [r4, #1164]
	add	r3, r3, #1
	cmp	r3, r2
	movlt	r3, r2
	str	r3, [r4, #1160]
	ldr	r1, [r7]
	rsb	r3, r2, r3
	str	r3, [r1, #576]
	b	.L3025
.L3117:
	mov	r2, r5
	ldr	r1, .L3124+4
	mov	r0, #2
	bl	dprint_vfmw
	str	r5, [r4, #1064]
.L3030:
	mov	r0, r7
	bl	H264_ClearCurrPic
	mov	r0, r7
	bl	ClearDPB
	mvn	r0, #0
.L3109:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3048:
	ldr	r2, [r7]
.L3056:
	ldr	r3, [r2, #592]
	mov	r0, #0
	str	r3, [r4, #1044]
	ldr	r3, [r2, #632]
	str	r3, [r4, #1048]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3029:
	ldr	r5, [r6, #2412]
	cmp	r5, #0
	bne	.L3036
	ldr	r3, [r4, #1060]
	cmp	r3, #0
	beq	.L3033
	ldr	r3, [r4, #1056]
	ldr	r3, [r3, #48]
	cmp	r3, #0
	beq	.L3046
	mov	r3, #1
	mov	r0, r7
	strb	r3, [r6, #1743]
	bl	H264_ClearCurrPic
	mov	r0, r5
	ldr	r1, .L3124+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3109
.L3118:
	ldr	r3, [r6, #2412]
	cmp	r3, #2
	beq	.L3123
	ldr	r2, [r4, #1060]
	cmp	r2, #0
	bne	.L3044
.L3033:
	mov	r0, r7
	bl	H264_ClearCurrPic
	ldr	r3, .L3124+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3113
	mov	r3, #0
	ldr	r0, [r4, #1136]
	mov	r2, r3
	mov	r1, #127
	blx	ip
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3120:
	ldr	r3, .L3124+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3050
	ldr	r2, [r4, #784]
	cmp	r2, #0
	beq	.L3051
.L3052:
	ldr	r3, [r2, #8]
	ldr	r0, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3052
.L3051:
	sub	r2, fp, #28
	mov	r3, #4
	ldr	r0, [r4, #1136]
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
	ldr	r2, [r7]
	b	.L3053
.L3123:
	ldr	r3, [r6, #2392]
	cmp	r3, #0
	beq	.L3036
	ldr	r3, [r4, #1060]
	cmp	r3, #0
	beq	.L3033
.L3057:
	ldr	r3, [r6, #3840]
	cmp	r3, #2
	bne	.L3046
	ldr	r3, [r7]
	ldr	r5, [r3, #12]
	cmp	r5, #0
	bne	.L3046
	ldr	r3, .L3124+16
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3046
	ldr	r3, [r6, #2392]
	mov	r0, #1
	ldr	r2, [r6, #2396]
	ldr	r1, .L3124+20
	bl	dprint_vfmw
	mov	r0, r7
	bl	H264_ClearCurrPic
	ldr	r3, .L3124+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3047
	mov	r3, r5
	mov	r2, r5
	mov	r1, #127
	ldr	r0, [r4, #1136]
	blx	ip
.L3047:
	ldr	r3, [r6, #2376]
	mov	r2, #0
	ldr	r0, [r4, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L3109
.L3122:
	cmp	r3, #0
	beq	.L3056
	ldr	r3, [r6, #2412]
	cmp	r3, #2
	bne	.L3056
	mov	r2, #0
	str	r2, [r7, #360]
	ldr	r3, [r6, #2376]
	ldr	r0, [r4, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
.L3112:
	mov	r0, r7
	bl	H264_ClearCurrPic
.L3113:
	mvn	r0, #0
	b	.L3109
.L3119:
	cmp	r3, #0
	bne	.L3040
	ldr	r3, [r6, #3840]
	cmp	r3, #0
	moveq	r3, #2
	streq	r3, [r6, #3840]
	movne	r3, #1
	strne	r3, [r6, #3840]
	b	.L3046
.L3050:
	ldr	r2, [r7]
	b	.L3053
.L3121:
	ldr	r2, .L3124+12
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3112
	ldr	r0, [r4, #1136]
	mov	r2, r3
	mov	r1, #127
	blx	ip
	b	.L3112
.L3036:
	mov	r3, #1
	mov	r0, r7
	strb	r3, [r6, #1743]
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3109
.L3125:
	.align	2
.L3124:
	.word	.LC358
	.word	.LC355
	.word	.LC356
	.word	g_event_report
	.word	g_allow_B_before_P
	.word	.LC357
	UNWIND(.fnend)
	.size	DecVDM, .-DecVDM
	.align	2
	.global	H264_FlushDecoder
	.type	H264_FlushDecoder, %function
H264_FlushDecoder:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r2, r0, #20480
	mov	r1, #0
	mov	r4, #1
	str	r1, [r2, #260]
	strb	r4, [r2, #213]
	str	r4, [r0, #364]
	bl	DecVDM
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	mov	r0, r4
	ldr	r1, .L3128
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L3129:
	.align	2
.L3128:
	.word	.LC359
	UNWIND(.fnend)
	.size	H264_FlushDecoder, .-H264_FlushDecoder
	.align	2
	.global	ReceivePacket
	.type	ReceivePacket, %function
ReceivePacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	ip, [r1, #12]
	add	r6, r0, #61440
	ldr	r3, [r1, #8]
	ldr	r2, [r1, #4]
	mov	r4, r0
	mov	r5, r1
	mov	r0, #7
	ldr	r1, .L3193
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r6, #1168]
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3188
.L3131:
	ldr	r3, [r4, #368]
	cmp	r3, #0
	beq	.L3151
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3189
.L3134:
	ldr	r1, .L3193+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r4, #368]
	cmp	r1, #0
	beq	.L3151
	ldr	r0, [r6, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L3151:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3138
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L3138
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3138
	ldrb	r3, [r4, #377]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3154
	mov	r2, r4
	mov	r3, #1
	b	.L3141
.L3140:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L3152
.L3141:
	ldrb	r1, [r2, #473]	@ zero_extendqisi2
	add	r2, r2, #96
	cmp	r1, #0
	bne	.L3140
	mov	r2, r3
.L3139:
	mov	r1, r3, asl #7
	cmn	r2, #1
	sub	r3, r1, r3, asl #5
	mov	r1, #1
	add	r3, r4, r3
	strb	r1, [r3, #377]
	beq	.L3152
	mov	r3, r2, asl #7
	mov	r0, #0
	sub	r3, r3, r2, asl #5
	add	r2, r3, #376
	add	r3, r4, r3
	add	r2, r4, r2
	str	r2, [r4, #368]
	ldr	r2, [r5, #4]
	str	r2, [r3, #384]
	ldr	r2, [r5, #12]
	str	r2, [r3, #388]
	ldr	r2, [r5, #8]
	str	r2, [r3, #392]
	ldr	r3, [r4, #368]
	str	r0, [r3, #24]
	ldr	r3, [r4, #368]
	ldr	r2, [r5, #16]
	str	r2, [r3, #32]
	ldrd	r2, [r5, #24]
	ldr	ip, [r4, #368]
	strd	r2, [ip, #88]
	ldr	r3, [r4, #368]
	str	r1, [r3, #68]
	ldr	r3, [r4, #368]
	strb	r0, [r3]
	ldr	r3, [r4, #368]
	ldrb	r2, [r5]	@ zero_extendqisi2
	strb	r2, [r3, #4]
	ldr	r3, [r4, #368]
	b	.L3144
.L3189:
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bhi	.L3134
	ldrb	r1, [r3, #4]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3135
	b	.L3134
.L3188:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	beq	.L3131
	ldr	r3, [r4, #368]
	cmp	r3, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	beq	.L3151
.L3135:
	strb	r2, [r3, #4]
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3190
.L3146:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3147
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq	.L3147
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3147
	ldr	r3, [r4, #368]
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bls	.L3191
.L3148:
	mov	r2, #1
	strb	r2, [r3, #4]
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #1136]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3193+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3185
	mov	r3, #0
	ldr	r0, [r6, #1136]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3185:
	ldr	r3, [r4, #368]
.L3144:
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	strb	r2, [r3, #5]
	ldr	r3, [r4, #368]
	ldrb	r0, [r3, #4]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3138:
	ldr	r1, .L3193+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #1136]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3193+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3187
.L3186:
	mov	r3, #0
	ldr	r0, [r6, #1136]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3187:
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3147:
	ldr	r3, [r4, #368]
	b	.L3148
.L3190:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	bne	.L3185
	b	.L3146
.L3191:
	mov	ip, r1, asl #5
	mov	r0, #0
	sub	r1, ip, r1, asl #2
	add	r3, r3, r1
	str	r2, [r3, #8]
	ldr	r3, [r4, #368]
	ldr	ip, [r5, #12]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	ip, [r3, #12]
	ldr	r3, [r4, #368]
	ldr	ip, [r5, #8]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	ip, [r3, #16]
	ldr	r3, [r4, #368]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	r0, [r3, #24]
	ldr	r3, [r4, #368]
	ldr	r0, [r5, #16]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	r0, [r3, #32]
	ldr	r2, [r4, #368]
	ldr	r3, [r2, #68]
	add	r3, r3, #1
	str	r3, [r2, #68]
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #12]
	cmp	r2, #4096
	bcc	.L3192
.L3149:
	mov	r2, #1
	str	r2, [r6, #1168]
	b	.L3144
.L3192:
	mov	r0, r4
	bl	CombinePacket.part.10
	ldr	r3, [r4, #368]
	b	.L3149
.L3152:
	ldr	r1, .L3193+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearAllNal
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	ldr	r3, .L3193+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3186
	b	.L3187
.L3154:
	mov	r2, r3
	b	.L3139
.L3194:
	.align	2
.L3193:
	.word	.LC360
	.word	.LC361
	.word	g_event_report
	.word	.LC363
	.word	.LC362
	UNWIND(.fnend)
	.size	ReceivePacket, .-ReceivePacket
	.align	2
	.global	ClearAll
	.type	ClearAll, %function
ClearAll:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r8, r0
	mov	r4, r1
	mov	r0, #22
	ldr	r1, .L3223
	bl	dprint_vfmw
	mov	r0, r8
	bl	H264_ClearCurrPic
	mov	r0, r8
	bl	ClearAllNal
	mov	r0, r8
	bl	InitDPB
	cmp	r4, #0
	add	r7, r8, #61440
	beq	.L3196
	add	r1, r8, #8
	ldr	r0, [r7, #1136]
	bl	FSP_ClearNotInVoQueue
.L3197:
	add	r4, r8, #27904
	mov	r6, #0
	add	r4, r4, #88
.L3200:
	sub	r5, r4, #32
	mov	r3, #0
	sub	r2, r4, #592
	mov	r1, #1
	mvn	r9, #0
	mov	lr, #17
	mov	ip, #16
	mov	r0, #2
	str	r6, [r4, #-428]
	strb	r3, [r4, #-592]
	strb	r3, [r4, #-591]
	strb	r3, [r4, #-590]
	str	r3, [r4, #52]
	str	r3, [r4, #44]
	str	r3, [r4, #56]
	strb	r3, [r4, #-588]
	strb	r3, [r4, #84]
	str	r9, [r4, #48]
	str	r1, [r4, #60]
	strb	r1, [r4, #120]
	str	lr, [r4, #76]
	str	ip, [r4, #80]
	str	r2, [r4, #188]
	str	r2, [r4, #152]
	str	r2, [r4, #116]
	strb	r0, [r4, #156]
.L3199:
	ldrd	r2, [r5, #8]!
	orrs	r1, r2, r3
	bne	.L3221
.L3198:
	cmp	r5, r4
	bne	.L3199
	add	r6, r6, #1
	add	r4, r5, #792
	cmp	r6, #40
	bne	.L3200
	add	r5, r8, #59648
	mov	r4, r5
	add	r5, r5, #32
.L3202:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3222
.L3201:
	cmp	r4, r5
	bne	.L3202
	add	r6, r8, #57344
	ldr	r3, [r6, #3848]
	cmp	r3, #0
	beq	.L3206
	add	r5, r8, #61184
	mov	r4, #0
	add	r5, r5, #8
	mov	r9, r4
.L3205:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3204
	ldr	r0, [r7, #1136]
	bl	FreeUsdByDec
	str	r9, [r5]
.L3204:
	ldr	r3, [r6, #3848]
	cmp	r3, r4
	bhi	.L3205
.L3206:
	ldr	r3, .L3223+4
	add	r5, r8, #20480
	add	ip, r8, #24576
	mov	r4, #0
	add	r0, r5, #208
	str	r4, [r6, #3848]
	mov	r1, r4
	str	r4, [r7, #1112]
	ldr	r3, [r3, #48]
	mov	r2, #1648
	str	r4, [r6, #3828]
	str	r4, [r7, #1060]
	str	r4, [r7, #1120]
	str	r4, [r7, #1068]
	str	r4, [r6, #3836]
	str	r4, [r7, #1140]
	str	r4, [r6, #3840]
	str	r4, [r8, #368]
	str	r4, [ip, #1812]
	strb	r4, [r5, #3904]
	strb	r4, [r5, #1900]
	strb	r4, [r5, #3905]
	strb	r4, [r5, #1901]
	blx	r3
	mov	r2, #2
	mov	r3, #1
	strb	r2, [r5, #208]
	strb	r3, [r5, #213]
	mov	r0, r4
	mov	r3, #256
	strb	r4, [r6, #1736]
	strb	r4, [r6, #3160]
	str	r4, [r6, #3164]
	str	r3, [r6, #3168]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3221:
	mov	r1, r2
	ldr	r0, [r7, #1136]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r5]
	b	.L3198
.L3222:
	mov	r1, r2
	ldr	r0, [r7, #1136]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r4]
	b	.L3201
.L3196:
	add	r0, r8, #8
	bl	ResetVoQueue
	ldr	r0, [r7, #1136]
	bl	FSP_EmptyInstance
	b	.L3197
.L3224:
	.align	2
.L3223:
	.word	.LC364
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	ClearAll, .-ClearAll
	.align	2
	.global	InsertFrmInDPB
	.type	InsertFrmInDPB, %function
InsertFrmInDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	cmp	r1, #15
	mov	r9, r1
	mov	r5, r0
	mov	r4, r2
	bhi	.L3324
	ldrb	r7, [r2, #3]	@ zero_extendqisi2
	cmp	r7, #1
	beq	.L3229
	bcc	.L3230
	cmp	r7, #2
	bne	.L3325
	ldr	r3, [r2, #640]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r3, r1, asl #2
	beq	.L3250
	add	r6, r0, r3
	add	r2, r6, #24576
	str	r2, [fp, #-48]
	ldr	r2, [r2, #1952]
	cmp	r2, #0
	beq	.L3250
	ldr	r0, [fp, #-48]
	strb	r7, [r2, #748]
	ldr	r2, [r4, #640]
	ldr	r3, [r0, #1952]
	str	r2, [r3, #780]
	ldr	r3, [r0, #1952]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #749]
	ldr	r3, [r0, #1952]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #750]
	ldr	r3, [r0, #1952]
	ldr	r2, [r4, #672]
	str	r2, [r3, #760]
	ldr	r3, [r0, #1952]
	ldr	r2, [r4, #676]
	str	r2, [r3, #768]
	ldr	r3, [r0, #1952]
	ldr	r2, [r4, #684]
	str	r2, [r3, #764]
	ldr	r3, [r0, #1952]
	ldr	r1, [r3, #728]
	ldr	r2, [r3, #764]
	add	r2, r2, r1
	str	r2, [r3, #692]
	str	r2, [r3, #664]
	ldr	r2, [r0, #1952]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r3, [fp, #-48]
	ldr	r2, [r3, #1952]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #1]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [fp, #-48]
	cmp	r3, #1
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #1952]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	orreq	r2, r2, #2
	strb	r2, [r3, #3]
	mov	r2, #624
	ldr	lr, [r1, #1952]
	add	r1, r4, #16
	add	ip, lr, r2
	add	r0, lr, #8
	ldr	r3, [lr, #608]
	ldrd	r6, [ip, #-8]
	ldr	r10, [lr, #16]
	str	r3, [fp, #-72]
	strd	r6, [fp, #-60]
	ldr	r3, [lr, #612]
	ldrd	r6, [ip]
	ldr	ip, [lr, #20]
	ldr	r8, [lr, #24]
	strd	r6, [fp, #-68]
	mov	r6, r3
	str	ip, [fp, #-52]
	ldr	r7, [lr, #28]
	bl	memcpy
	ldr	r3, [fp, #-72]
	add	r2, r4, #624
	and	ip, r3, r6
	ldmdb	r2, {r0, lr}
	adds	r1, ip, #1
	ldr	ip, [fp, #-52]
	and	r0, r0, lr
	movne	r1, #1
	cmn	r0, #1
	movne	r1, #0
	cmp	r1, #0
	ldrne	r1, [fp, #-48]
	ldrne	r1, [r1, #1952]
	strne	r3, [r1, #608]
	movne	r3, r6
	strne	r3, [r1, #612]
	ldr	r1, [fp, #-48]
	ldrd	r2, [r2]
	ldr	lr, [r1, #1952]
	ldrd	r0, [fp, #-60]
	cmp	r3, r1
	add	r1, lr, #624
	cmpeq	r2, r0
	ldrhid	r2, [fp, #-60]
.L3260:
	strd	r2, [r1, #-8]
	add	r0, r4, #640
	ldr	r3, [fp, #-48]
	ldr	r1, [r3, #1952]
	ldrd	r2, [r0, #-8]
	add	lr, r1, #624
	ldrd	r0, [fp, #-68]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r3, r1
	ldr	r1, [fp, #-48]
	movhi	r2, r0
	strd	r2, [lr]
	ldr	r3, [r4, #24]
	ldr	r2, [r1, #1952]
	cmp	r3, r10
	movcs	r3, r10
	str	r3, [r2, #16]
	ldr	r3, [r4, #28]
	ldr	r2, [r1, #1952]
	cmp	r3, ip
	movcs	r3, ip
	str	r3, [r2, #20]
	ldr	r3, [r4, #32]
	ldr	r2, [r1, #1952]
	cmp	r3, r8
	movcs	r3, r8
	str	r3, [r2, #24]
	ldr	r3, [r4, #36]
	ldr	r2, [r1, #1952]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #28]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3262
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3262
.L3263:
	ldr	r0, [fp, #-48]
	ldr	r2, [r0, #1952]
	ldr	r3, [r2, #768]
	ldr	r1, [r2, #732]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #660]
	ldr	r2, [r4, #660]
	mov	r4, r0
	ldr	r3, [r0, #1952]
	str	r2, [r3, #648]
	ldr	r0, [r0, #1952]
	bl	CombineFldsToFrm
	ldr	r2, [r4, #1952]
	ldr	r3, [r2, #672]
	ldr	r2, [r2, #760]
	add	r3, r3, #6720
	add	r3, r3, #12
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3249
.L3230:
	ldr	r2, [r2, #640]
	add	r8, r0, r1, lsl #2
	add	r8, r8, #24576
	mov	r3, #3
	str	r2, [r8, #1952]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r2]
	ldr	r2, [r8, #1952]
	strb	r3, [r2, #1]
	ldr	r1, [r4, #640]
	ldr	r2, [r8, #1952]
	ldrb	r1, [r1, #4]	@ zero_extendqisi2
	strb	r1, [r2, #4]
	ldr	r1, [r4, #640]
	ldr	r2, [r8, #1952]
	ldr	r1, [r1, #652]
	str	r1, [r2, #652]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	ldr	r2, [r8, #1952]
	cmp	r1, #0
	beq	.L3326
.L3232:
	strb	r3, [r2, #2]
	add	r1, r4, #16
	ldr	r3, [r8, #1952]
	mov	r2, #624
	ldr	r0, [r4, #708]
	mov	r6, #0
	mov	r10, #1
	str	r0, [r3, #668]
	ldr	r0, [r8, #1952]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #684]
	mov	ip, #2
	mov	r2, r6
	mov	r0, r5
	str	r1, [r3, #692]
	str	r1, [r3, #664]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #640]
	str	r1, [r3, #780]
	str	r1, [r3, #744]
	str	r1, [r3, #708]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #652]
	str	r1, [r3, #644]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #648]
	str	r1, [r3, #636]
	ldr	r3, [r8, #1952]
	strb	r6, [r3, #676]
	ldr	r3, [r8, #1952]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	strb	r1, [r3, #677]
	ldr	r3, [r8, #1952]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	strb	r1, [r3, #678]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #676]
	str	r1, [r3, #696]
	str	r1, [r3, #660]
	ldr	r3, [r8, #1952]
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	str	r1, [r3, #656]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #660]
	str	r1, [r3, #648]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #664]
	str	r1, [r3, #688]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #668]
	str	r1, [r3, #724]
	ldr	r3, [r8, #1952]
	ldr	r1, [r4, #672]
	str	r1, [r3, #760]
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	ldr	r3, [r8, #1952]
	cmp	r1, r10
	moveq	r7, #3
	strb	r7, [r3, #3]
	ldr	r3, [r8, #1952]
	ldr	r1, [r3, #708]
	ldrb	r8, [r3, #677]	@ zero_extendqisi2
	ldrb	r7, [r3, #678]	@ zero_extendqisi2
	ldr	lr, [r3, #692]
	strb	ip, [r3, #748]
	ldr	ip, [r3, #696]
	strb	r10, [r3, #712]
	strb	r8, [r3, #713]
	strb	r8, [r3, #749]
	strb	r7, [r3, #714]
	strb	r7, [r3, #750]
	str	lr, [r3, #728]
	str	lr, [r3, #764]
	str	ip, [r3, #732]
	str	ip, [r3, #768]
	str	r1, [r3, #744]
	str	r1, [r3, #780]
	ldr	r1, [r4, #640]
	bl	GetAPC
	cmp	r0, #0
	movne	r3, r0
	movwne	r2, #2341
	movne	r0, r6
	bne	.L3322
.L3249:
	add	r3, r9, #6592
	add	r1, r5, #61440
	add	r3, r3, #40
	mov	r2, #1
	ldr	r0, [r1, #1136]
	ldr	r3, [r5, r3, asl #2]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r0, #0
.L3227:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3325:
	mov	r2, r7
	ldr	r1, .L3327
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r5
	mov	r1, #1
	bl	ClearAll
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3229:
	ldr	r3, [r2, #640]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r3, r1, asl #2
	beq	.L3235
	add	r6, r0, r3
	add	r2, r6, #24576
	str	r2, [fp, #-48]
	ldr	r2, [r2, #1952]
	cmp	r2, #0
	beq	.L3235
	ldr	r0, [fp, #-48]
	strb	r7, [r2, #712]
	ldr	r2, [r4, #676]
	ldr	r3, [r0, #1952]
	str	r2, [r3, #732]
	ldr	r3, [r0, #1952]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #713]
	ldr	r3, [r0, #1952]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #714]
	ldr	r3, [r0, #1952]
	ldr	r2, [r4, #668]
	str	r2, [r3, #724]
	ldr	r3, [r0, #1952]
	ldr	r2, [r4, #676]
	str	r2, [r3, #732]
	ldr	r3, [r0, #1952]
	ldr	r2, [r4, #684]
	str	r2, [r3, #728]
	ldr	r3, [r0, #1952]
	ldr	r1, [r3, #728]
	ldr	r2, [r3, #764]
	add	r2, r2, r1
	str	r2, [r3, #692]
	str	r2, [r3, #664]
	ldr	r2, [r0, #1952]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r3, [fp, #-48]
	ldr	r2, [r3, #1952]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #1]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [fp, #-48]
	cmp	r3, #1
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #1952]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	orreq	r2, r2, #1
	strb	r2, [r3, #3]
	mov	r2, #624
	ldr	lr, [r1, #1952]
	add	r1, r4, #16
	add	ip, lr, r2
	add	r0, lr, #8
	ldr	r3, [lr, #608]
	ldrd	r6, [ip, #-8]
	ldr	r10, [lr, #16]
	str	r3, [fp, #-72]
	strd	r6, [fp, #-60]
	ldr	r3, [lr, #612]
	ldrd	r6, [ip]
	ldr	ip, [lr, #20]
	ldr	r8, [lr, #24]
	strd	r6, [fp, #-68]
	mov	r6, r3
	str	ip, [fp, #-52]
	ldr	r7, [lr, #28]
	bl	memcpy
	ldr	r3, [fp, #-72]
	add	r2, r4, #624
	and	ip, r3, r6
	ldmdb	r2, {r0, lr}
	adds	r1, ip, #1
	ldr	ip, [fp, #-52]
	and	r0, r0, lr
	movne	r1, #1
	cmn	r0, #1
	movne	r1, #0
	cmp	r1, #0
	ldrne	r1, [fp, #-48]
	ldrne	r1, [r1, #1952]
	strne	r3, [r1, #608]
	movne	r3, r6
	strne	r3, [r1, #612]
	ldr	r1, [fp, #-48]
	ldrd	r2, [r2]
	ldr	lr, [r1, #1952]
	ldrd	r0, [fp, #-60]
	cmp	r3, r1
	add	r1, lr, #624
	cmpeq	r2, r0
	ldrhid	r2, [fp, #-60]
.L3244:
	strd	r2, [r1, #-8]
	add	r0, r4, #640
	ldr	r3, [fp, #-48]
	ldr	r1, [r3, #1952]
	ldrd	r2, [r0, #-8]
	add	lr, r1, #624
	ldrd	r0, [fp, #-68]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r3, r1
	ldr	r1, [fp, #-48]
	movhi	r2, r0
	strd	r2, [lr]
	ldr	r3, [r4, #24]
	ldr	r2, [r1, #1952]
	cmp	r3, r10
	movcs	r3, r10
	str	r3, [r2, #16]
	ldr	r3, [r4, #28]
	ldr	r2, [r1, #1952]
	cmp	r3, ip
	movcs	r3, ip
	str	r3, [r2, #20]
	ldr	r3, [r4, #32]
	ldr	r2, [r1, #1952]
	cmp	r3, r8
	movcs	r3, r8
	str	r3, [r2, #24]
	ldr	r3, [r4, #36]
	ldr	r2, [r1, #1952]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #28]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3246
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3246
.L3247:
	ldr	r0, [fp, #-48]
	ldr	r2, [r0, #1952]
	ldr	r3, [r2, #768]
	ldr	r1, [r2, #732]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #660]
	ldr	r2, [r4, #660]
	mov	r4, r0
	ldr	r3, [r0, #1952]
	str	r2, [r3, #648]
	ldr	r0, [r0, #1952]
	bl	CombineFldsToFrm
	ldr	r2, [r4, #1952]
	ldr	r3, [r2, #672]
	ldr	r2, [r2, #724]
	add	r3, r3, #6656
	add	r3, r3, #60
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3249
.L3235:
	add	r6, r5, r3
	ldr	r3, [r4, #640]
	add	r6, r6, #24576
	mov	r2, #1
	str	r3, [r6, #1952]
	strb	r2, [r3, #712]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #640]
	str	r1, [r3, #744]
	ldr	r3, [r6, #1952]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	strb	r1, [r3, #713]
	ldr	r3, [r6, #1952]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	strb	r1, [r3, #714]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #668]
	str	r1, [r3, #724]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #676]
	str	r1, [r3, #732]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #684]
	str	r1, [r3, #728]
	ldr	r3, [r6, #1952]
	ldr	r1, [r3, #728]
	str	r1, [r3, #692]
	str	r1, [r3, #664]
	ldr	r3, [r6, #1952]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r3]
	ldr	r3, [r6, #1952]
	strb	r2, [r3, #1]
	ldr	r2, [r4, #640]
	ldr	r3, [r6, #1952]
	ldrb	r2, [r2, #4]	@ zero_extendqisi2
	strb	r2, [r3, #4]
	ldr	r2, [r4, #640]
	ldr	r3, [r6, #1952]
	ldr	r2, [r2, #652]
	str	r2, [r3, #652]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3237
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3237
.L3238:
	ldr	r3, [r6, #1952]
	mov	r7, #0
	mov	r2, #624
	add	r1, r4, #16
	str	r7, [r3, #656]
	ldr	r0, [r6, #1952]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #708]
	mov	r2, #1
	mov	r0, r5
	str	r1, [r3, #668]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #648]
	str	r1, [r3, #636]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #660]
	str	r1, [r3, #648]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #676]
	str	r1, [r3, #660]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [r6, #1952]
	rsb	r3, r2, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r1, #3]
	ldr	r1, [r4, #640]
	bl	GetAPC
	cmp	r0, #0
	beq	.L3249
	mov	r3, r0
	movw	r2, #2394
	mov	r0, r7
.L3322:
	ldr	r1, .L3327+4
	bl	dprint_vfmw
	mov	r0, r5
	bl	ClearDPB
	mvn	r0, #0
	b	.L3227
.L3250:
	add	r6, r5, r3
	ldr	r3, [r4, #640]
	add	r6, r6, #24576
	mov	r2, #2
	str	r3, [r6, #1952]
	strb	r2, [r3, #748]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #640]
	str	r1, [r3, #780]
	ldr	r3, [r6, #1952]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	strb	r1, [r3, #749]
	ldr	r3, [r6, #1952]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	strb	r1, [r3, #750]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #672]
	str	r1, [r3, #760]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #676]
	str	r1, [r3, #768]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #684]
	str	r1, [r3, #764]
	ldr	r3, [r6, #1952]
	ldr	r1, [r3, #728]
	str	r1, [r3, #692]
	str	r1, [r3, #664]
	ldr	r3, [r6, #1952]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r3]
	ldr	r3, [r6, #1952]
	strb	r2, [r3, #1]
	ldr	r2, [r4, #640]
	ldr	r3, [r6, #1952]
	ldrb	r2, [r2, #4]	@ zero_extendqisi2
	strb	r2, [r3, #4]
	ldr	r2, [r4, #640]
	ldr	r3, [r6, #1952]
	ldr	r2, [r2, #652]
	str	r2, [r3, #652]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3252
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3252
.L3253:
	ldr	r3, [r6, #1952]
	mov	r8, #0
	mov	r2, #624
	add	r1, r4, #16
	str	r8, [r3, #656]
	ldr	r0, [r6, #1952]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #708]
	mov	r2, #2
	mov	r0, r5
	str	r1, [r3, #668]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #648]
	str	r1, [r3, #636]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #660]
	str	r1, [r3, #648]
	ldr	r3, [r6, #1952]
	ldr	r1, [r4, #676]
	str	r1, [r3, #768]
	str	r1, [r3, #660]
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	ldr	r3, [r6, #1952]
	cmp	r1, #1
	moveq	r7, r2
	movne	r7, r8
	strb	r7, [r3, #3]
	ldr	r1, [r4, #640]
	bl	GetAPC
	cmp	r0, #0
	movne	r3, r0
	movwne	r2, #2539
	movne	r0, r8
	beq	.L3249
	b	.L3322
.L3326:
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	b	.L3232
.L3237:
	ldr	r3, [r6, #1952]
	mov	r2, #1
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, r2
	ldreq	r3, [r6, #1952]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #644]
	b	.L3238
.L3252:
	ldr	r3, [r6, #1952]
	mov	r2, #2
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r6, #1952]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #644]
	b	.L3253
.L3262:
	ldr	r1, [fp, #-48]
	ldr	r2, [r1, #1952]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r3, r1
	ldreq	r2, [r4, #652]
	ldreq	r3, [r3, #1952]
	streq	r2, [r3, #644]
	b	.L3263
.L3246:
	ldr	r1, [fp, #-48]
	ldr	r2, [r1, #1952]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r3, r1
	ldreq	r2, [r4, #652]
	ldreq	r3, [r3, #1952]
	streq	r2, [r3, #644]
	b	.L3247
.L3324:
	mov	r3, r1
	movw	r2, #2303
	ldr	r1, .L3327+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3227
.L3328:
	.align	2
.L3327:
	.word	.LC367
	.word	.LC366
	.word	.LC365
	UNWIND(.fnend)
	.size	InsertFrmInDPB, .-InsertFrmInDPB
	.align	2
	.global	StorePicInDPB
	.type	StorePicInDPB, %function
StorePicInDPB:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	ldr	r3, [r0]
	mov	r4, r0
	ldr	r3, [r3, #596]
	cmp	r3, #1
	beq	.L3495
	add	r5, r0, #57344
	mov	r3, #0
	str	r3, [r5, #1832]
	str	r3, [r5, #1836]
.L3332:
	add	r3, r4, #16384
	mov	r8, #0
	str	r8, [r3, #3444]
	ldrb	r2, [r5, #1739]	@ zero_extendqisi2
	sub	r1, r2, #2
	clz	r1, r1
	mov	r1, r1, lsr #5
	str	r1, [r3, #3448]
	ldrb	r0, [r5, #1736]	@ zero_extendqisi2
	cmp	r0, r8
	beq	.L3496
	add	r6, r4, #61440
	ldr	r0, [r6, #1064]
	ldr	r3, [r6, #1148]
	cmp	r0, #2
	str	r3, [r6, #1144]
	beq	.L3497
	ldr	r7, [r4]
	ldr	r3, [r7, #12]
	cmp	r3, #0
	ble	.L3498
	ldrb	r1, [r5, #1738]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3340
.L3506:
	sub	r1, r2, #1
	ldr	r9, [r5, #2376]
	cmp	r1, #1
	movhi	r1, #0
	movls	r1, #1
	cmp	r2, #0
	beq	.L3341
	ldrb	r0, [r9, #1]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	ip, r1
	orrne	ip, r1, #1
	cmp	ip, #0
	beq	.L3340
	cmp	r1, #0
	beq	.L3499
.L3415:
	ldr	r1, [r6, #1140]
	cmp	r1, #0
	bne	.L3344
	add	r1, r5, #1728
	mov	r3, #1
	add	r1, r1, #8
	str	r3, [r6, #1140]
	mov	r2, #712
	add	r0, r5, #2448
	bl	memcpy
	ldr	r3, [r7, #12]
.L3340:
	cmp	r3, #2
	ldr	r3, [r5, #2392]
	beq	.L3500
	cmp	r3, #0
	beq	.L3376
	ldr	r3, [r6, #1128]
	ldr	r2, [r5, #2420]
	cmp	r3, r2
	bcs	.L3377
	ldrb	r1, [r5, #1738]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L3501
.L3377:
	mov	r0, r4
	bl	Marking
	subs	r2, r0, #0
	beq	.L3379
	ldr	r1, .L3520
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3961
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3341:
	cmp	r1, #0
	bne	.L3415
.L3347:
	add	r7, r4, #58880
	add	r7, r7, #200
.L3349:
	cmp	r2, #0
	beq	.L3350
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3502
.L3350:
	ldr	r3, [r7, #684]
	str	r3, [r9, #664]
.L3351:
	mov	r1, r9
	mov	r0, r4
	bl	GetImagePara
	ldr	r1, [r9, #632]
	ldr	r0, [r6, #1136]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L3503
	ldrb	r3, [r7, #3]	@ zero_extendqisi2
	add	ip, r7, #624
	cmp	r3, #0
	beq	.L3353
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3504
.L3353:
	ldrd	r2, [ip, #-8]
	add	r1, r8, #608
.L3487:
	strd	r2, [r1, #-8]
.L3354:
	ldrd	r2, [ip]
	add	ip, r7, #640
	add	r0, r8, #624
	strd	r2, [r1]
	ldrd	r2, [ip, #-8]
	strd	r2, [r0, #-8]
	ldr	r3, [r7, #24]
	str	r3, [r8, #8]
	ldr	r3, [r7, #28]
	str	r3, [r8, #12]
	ldr	r3, [r7, #32]
	str	r3, [r8, #16]
	ldr	r3, [r7, #36]
	str	r3, [r8, #20]
	ldr	r1, [r9, #664]
	cmp	r1, #0
	str	r1, [r8, #160]
	ldr	r3, [r7, #244]
	str	r3, [r8, #228]
	ldr	r3, [r7, #240]
	str	r3, [r8, #224]
	ldrd	r2, [r7, #248]
	strd	r2, [r8, #232]
	beq	.L3355
	ldr	r3, [r8, #108]
	ands	r7, r3, #3
	beq	.L3505
.L3356:
	ldr	r3, [r4]
	ldr	r2, [r3, #4]
	cmp	r2, r1
	bcs	.L3355
	ldr	r3, .L3520+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3366
	str	r1, [fp, #-52]
	mov	r3, #8
	str	r2, [fp, #-48]
	mov	r1, #104
	sub	r2, fp, #52
	ldr	r0, [r6, #1136]
	blx	ip
.L3366:
	ldr	r1, [r9, #632]
	mov	r2, #0
	ldr	r0, [r6, #1136]
	bl	FSP_SetDisplay
	movw	r3, #3884
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3496:
	ldr	r1, .L3520+12
	add	r6, r4, #61440
	bl	dprint_vfmw
	movw	r3, #3666
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
.L3414:
	ldr	r3, [r5, #2376]
	mov	r2, #1
	ldr	r0, [r6, #1136]
	mvn	r7, #0
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	mov	r3, r7
	movw	r2, #4180
	ldr	r1, .L3520+16
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r5, #2376]
	mov	r3, #0
	strb	r3, [r2, #4]
	ldr	r2, [r5, #2376]
	strb	r3, [r2, #1]
.L3412:
	mov	r0, r7
	mov	r3, #0
	strb	r3, [r5, #1736]
.L3484:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3495:
	ldr	r2, .L3520+20
	add	r5, r0, #57344
	ldr	r2, [r2]
	add	r2, r2, #4096
	ldr	r0, [r2, #84]
	ldr	r1, [r2, #88]
	str	r3, [r5, #1832]
	cmp	r1, #0
	moveq	r1, #1
	bl	__aeabi_uidiv
	str	r0, [r5, #1836]
	b	.L3332
.L3498:
	ldr	r1, [r6, #1140]
	cmp	r1, #2
	beq	.L3340
	ldrb	r1, [r5, #1738]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3340
	b	.L3506
.L3500:
	cmp	r3, #0
	beq	.L3369
	ldrb	r3, [r5, #1742]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3370
.L3374:
	mov	r0, r4
	add	r7, r4, #24576
	bl	SimpleSlideDPB
	add	r2, r4, #58880
	ldr	r1, [r7, #2148]
	add	r2, r2, #200
	mov	r0, r4
	bl	InsertFrmInDPB
	subs	r8, r0, #0
	beq	.L3507
	mov	r3, r8
	movw	r2, #3927
	ldr	r1, .L3520+16
	mov	r0, #0
	bl	dprint_vfmw
	movw	r3, #3928
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3505:
	movw	r2, #3851
	ldr	r1, .L3520+24
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3520+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3488
	mov	r3, r7
	mov	r2, r7
	mov	r1, #111
	ldr	r0, [r6, #1136]
	blx	ip
.L3488:
	ldr	r1, [r8, #160]
	cmp	r1, #0
	bne	.L3356
.L3355:
	mov	r2, #1
	ldr	r1, [r9, #632]
	ldr	r0, [r6, #1136]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #1136]
	add	r3, r4, #8
	str	r8, [sp]
	mov	r2, r4
	mov	r1, #0
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L3360
	mov	r2, #0
	ldr	r1, [r9, #632]
	ldr	r0, [r6, #1136]
	bl	FSP_SetDisplay
.L3361:
	ldr	r3, [r4]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r8, #24]
	bcs	.L3508
.L3363:
	ldr	r2, [r6, #1140]
	ldr	r3, [r6, #1156]
	cmp	r2, #2
	add	r3, r3, #2
	str	r3, [r6, #1156]
	movne	r3, #1
	strne	r3, [r9, #784]
	mov	r3, #2
	str	r3, [r6, #1140]
	ldr	r3, [r4]
	ldr	r3, [r3, #12]
	b	.L3340
.L3499:
	cmp	r0, #0
	beq	.L3340
	b	.L3347
.L3379:
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L3369
	ldr	r1, .L3520+28
	mov	r0, #22
	bl	dprint_vfmw
.L3369:
	ldrb	r3, [r5, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3381
	add	r7, r4, #24576
.L3385:
	ldr	r3, [r5, #2392]
	cmp	r3, #0
	beq	.L3509
	ldr	r2, [r7, #2152]
	ldr	r1, [r7, #2156]
	ldr	r3, [r7, #2144]
	add	r0, r1, r2
	cmp	r3, r0
	bhi	.L3393
	ldr	r0, [r4]
	ldr	r0, [r0, #12]
	cmp	r0, #3
	beq	.L3394
	ldr	r0, .L3520+32
	ldrb	r0, [r0]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L3394
	str	r1, [sp, #4]
	mov	r0, #1
	str	r2, [sp]
	movw	r2, #4040
	ldr	r1, .L3520+36
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearDPB
	movw	r3, #4066
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3376:
	ldrb	r3, [r5, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3381
	add	r7, r4, #24576
	ldr	r3, [r7, #2144]
.L3393:
	mvn	r10, #0
	mov	r9, r10
	b	.L3403
.L3404:
	bl	GetMinPOC
	ldr	r3, [r5, #2392]
	cmp	r3, #0
	bne	.L3396
	ldr	r2, [r5, #2396]
	ldr	r3, [fp, #-56]
	cmp	r2, r3
	blt	.L3510
.L3396:
	ldr	r8, [r7, #2148]
	cmp	r8, r10
	ldrne	r9, [fp, #-52]
	bne	.L3401
	ldr	r3, [fp, #-52]
	cmp	r3, r9
	beq	.L3511
	mov	r9, r3
.L3401:
	mov	r1, r9
	mov	r0, r4
	bl	OutputFrmFromDPB
	mov	r10, r8
	cmn	r0, #1
	beq	.L3402
	ldr	r3, [r7, #2144]
.L3403:
	ldr	ip, [r7, #2148]
	sub	r2, fp, #52
	sub	r1, fp, #56
	mov	r0, r4
	cmp	ip, r3
	bcs	.L3404
	ldr	r3, [r5, #2392]
	cmp	r3, #0
	beq	.L3405
	ldrb	r3, [r5, #1740]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3405
	ldr	r2, [r7, #2152]
	cmp	r2, #0
	beq	.L3405
	add	r2, r4, #26368
	mov	lr, r3
	add	r2, r2, #220
	b	.L3410
.L3407:
	ldr	r1, [r7, #2152]
	cmp	r1, r3
	bls	.L3512
.L3410:
	ldr	r1, [r2, #4]!
	add	r3, r3, #1
	ldr	r0, [r5, #2384]
	ldr	ip, [r1, #636]
	cmp	ip, r0
	bne	.L3407
	strb	lr, [r1, #2]
	ldr	r1, [r2]
	ldrb	r0, [r1, #4]	@ zero_extendqisi2
	cmp	r0, #1
	streq	r0, [r1, #652]
	ldreq	r1, [r2]
	streqb	lr, [r1, #4]
	ldreq	r1, [r2]
	ldr	r8, [r5, #3824]
	ldr	ip, [r1, #668]
	add	r0, ip, #15232
	cmp	ip, r8
	add	r0, r0, #42
	strne	lr, [r4, r0, asl #2]
	strne	r8, [r1, #668]
	b	.L3407
.L3512:
	ldr	ip, [r7, #2148]
.L3405:
	add	r2, r4, #58880
	mov	r1, ip
	add	r2, r2, #200
	mov	r0, r4
	bl	InsertFrmInDPB
	ldr	r2, [r7, #2148]
	str	r2, [r5, #3184]
	subs	r3, r0, #0
	bne	.L3513
	add	r2, r2, #1
	mov	r0, r4
	str	r2, [r7, #2148]
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
.L3375:
	mov	r7, #0
	b	.L3412
.L3509:
	ldr	r3, [r7, #2144]
	b	.L3393
.L3381:
	ldr	r3, [r5, #3188]
	ldr	r0, [r5, #2376]
	cmp	r3, #0
	rsb	r1, r3, r0
	clz	r1, r1
	mov	r1, r1, lsr #5
	moveq	r1, #0
	cmp	r1, #0
	beq	.L3514
	mov	r0, r4
	bl	DirectOutput
	cmn	r0, #1
	mov	r7, r0
	beq	.L3515
	cmp	r0, #1
	ldreq	r2, [r5, #2376]
	moveq	r3, #0
	streqb	r3, [r2, #4]
	ldreq	r2, [r5, #2376]
	streqb	r3, [r2, #1]
.L3389:
	movw	r3, #3997
	mov	r2, r7
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3338
.L3497:
	ldr	r1, .L3520+40
	bl	dprint_vfmw
	mov	r0, r4
	bl	DirectOutput
	ldr	r3, [r5, #2444]
	ldr	r2, [r5, #3824]
	ldr	r1, .L3520+4
	cmp	r3, r2
	addne	r3, r3, #15232
	addne	r3, r3, #42
	strne	r8, [r4, r3, asl #2]
	strne	r2, [r5, #2444]
	cmp	r0, #1
	mov	r7, r0
	ldreq	r2, [r5, #2376]
	moveq	r3, #0
	streqb	r3, [r2, #4]
	ldreq	r2, [r5, #2376]
	streqb	r3, [r2, #1]
	mov	r2, r0
	movw	r3, #3727
	mov	r0, #14
	bl	dprint_vfmw
.L3338:
	cmn	r7, #1
	bne	.L3412
	b	.L3414
.L3344:
	cmp	r1, #1
	beq	.L3516
	cmp	r2, #0
	beq	.L3347
	ldrb	r1, [r9, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3347
	b	.L3340
.L3514:
	add	r7, r4, #24576
	ldr	ip, [r7, #2148]
	cmp	ip, #0
	beq	.L3385
	ldr	r3, [r7, #1952]
	cmp	r0, r3
	addne	r3, r4, #26368
	addne	r3, r3, #160
	beq	.L3386
.L3390:
	add	r1, r1, #1
	cmp	r1, ip
	beq	.L3385
	ldr	r2, [r3, #4]!
	cmp	r0, r2
	bne	.L3390
.L3386:
	add	r2, r4, #58880
	mov	r0, r4
	add	r2, r2, #200
	bl	InsertFrmInDPB
	cmn	r0, #1
	mov	r7, r0
	beq	.L3517
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	movw	r3, #4017
	mov	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3375
.L3402:
	mov	r3, r0
	str	r0, [fp, #-64]
	movw	r2, #4116
	ldr	r1, .L3520+16
	mov	r0, #1
	bl	dprint_vfmw
	ldr	ip, [fp, #-64]
	movw	r3, #4117
	ldr	r1, .L3520+4
	mov	r0, #14
	mov	r2, ip
	bl	dprint_vfmw
	b	.L3414
.L3513:
	movw	r2, #4153
	ldr	r1, .L3520+16
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #4154
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3360:
	ldr	r3, [r6, #1164]
	add	r3, r3, #1
	str	r3, [r6, #1164]
	b	.L3361
.L3507:
	ldr	r3, [r7, #2148]
	mov	r0, r4
	add	r3, r3, #1
	str	r3, [r7, #2148]
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	mov	r2, r8
	mov	r3, #3936
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3375
.L3394:
	sub	r2, fp, #52
	sub	r1, fp, #56
	mov	r0, r4
	bl	GetMinPOC
	ldr	r1, [fp, #-52]
	mov	r0, r4
	bl	RemoveFrameStoreOutDPB
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	ldr	r3, [r7, #2144]
	b	.L3393
.L3504:
	ldrd	r0, [ip, #-8]
	mvn	r3, #0
	mvn	r2, #0
	cmp	r1, r3
	cmpeq	r0, r2
	addne	r1, r8, #608
	bne	.L3354
	add	r3, r4, #60416
	ldrd	r2, [r3, #-8]
	cmp	r3, r1
	add	r1, r8, #608
	cmpeq	r2, r0
	bne	.L3487
	b	.L3354
.L3515:
	mov	r3, r0
	movw	r2, #3987
	ldr	r1, .L3520+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3389
.L3501:
	ldr	r1, .L3520+44
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3520+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3378
	ldr	r7, [r5, #2420]
	mov	r3, #8
	ldr	lr, [r6, #1128]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r6, #1136]
	str	r7, [fp, #-52]
	str	lr, [fp, #-48]
	blx	ip
.L3378:
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	mov	r3, #3952
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3510:
	ldr	r3, [r6, #1064]
	cmp	r3, #2
	beq	.L3518
.L3397:
	mov	r0, r4
	bl	DirectOutput
	cmn	r0, #1
	mov	r7, r0
	beq	.L3519
	cmp	r0, #1
	ldreq	r2, [r5, #2376]
	moveq	r3, #0
	streqb	r3, [r2, #4]
	ldreq	r2, [r5, #2376]
	streqb	r3, [r2, #1]
.L3399:
	mov	r3, #4096
	mov	r2, r7
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3338
.L3511:
	mov	r3, r9
	ldr	r2, .L3520+48
	ldr	r1, .L3520+52
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #4103
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3508:
	mov	r1, r8
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L3363
.L3370:
	mov	r0, r4
	bl	IDRMemMarking
	cmp	r0, #0
	beq	.L3374
	mvn	r0, #0
	b	.L3484
.L3502:
	mov	r2, #3
	ldr	r3, [r9, #664]
	strb	r2, [r9, #1]
	ldr	r2, [r7, #684]
	add	r3, r3, r2
	str	r3, [r9, #664]
	b	.L3351
.L3516:
	add	r7, r4, #58880
	ldr	r9, [r5, #3088]
	add	r7, r7, #200
	b	.L3349
.L3518:
	ldrb	r3, [r5, #1739]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r5, #2376]
	strne	r3, [r5, #3192]
	b	.L3397
.L3517:
	mov	r3, r0
	movw	r2, #4010
	ldr	r1, .L3520+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, r7
	movw	r3, #4011
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3503:
	movw	r2, #3819
	ldr	r1, .L3520+56
	bl	dprint_vfmw
	movw	r3, #3820
	mvn	r2, #0
	ldr	r1, .L3520+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3414
.L3519:
	mov	r3, r0
	movw	r2, #4087
	ldr	r1, .L3520+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3399
.L3521:
	.align	2
.L3520:
	.word	.LC375
	.word	.LC369
	.word	g_event_report
	.word	.LC368
	.word	.LC373
	.word	g_HwMem
	.word	.LC372
	.word	.LC376
	.word	g_apple_airplay_mirrorinig_flag
	.word	.LC377
	.word	.LC370
	.word	.LC374
	.word	.LANCHOR1+252
	.word	.LC378
	.word	.LC371
	UNWIND(.fnend)
	.size	StorePicInDPB, .-StorePicInDPB
	.align	2
	.global	AllocFrameStore
	.type	AllocFrameStore, %function
AllocFrameStore:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r8, r0, #20480
	mov	r7, r0
	str	r1, [fp, #-56]
	ldrb	r3, [r8, #209]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3523
	ldrb	r3, [r8, #210]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3587
	add	r6, r0, #61440
	ldr	r3, [r6, #1064]
	cmp	r3, #2
	beq	.L3582
	add	r5, r7, #57344
	mov	r2, #0
	ldrb	r3, [r5, #3160]	@ zero_extendqisi2
	strb	r2, [r5, #1737]
	cmp	r3, #1
	mov	r3, #2
	str	r3, [fp, #-52]
	bne	.L3553
.L3549:
	ldr	r2, [r5, #3168]
	ldr	r3, [r8, #228]
	cmp	r2, r3
	bne	.L3553
	ldr	r3, [r8, #240]
	cmp	r3, #0
	ldr	r3, [r5, #3164]
	beq	.L3588
	cmp	r3, #0
	beq	.L3553
.L3529:
	ldr	r3, [r6, #1148]
	ldr	r2, [r6, #1144]
	rsb	r3, r2, r3
	cmp	r3, #2
	bhi	.L3553
	ldr	r1, [r5, #2376]
	mov	r3, #1
	strb	r3, [r5, #1737]
	cmp	r1, #0
	beq	.L3531
	ldr	r3, [fp, #-52]
	cmp	r3, #2
	beq	.L3589
	ldr	r3, [fp, #-52]
	cmp	r3, #1
	beq	.L3590
.L3556:
	mov	ip, #0
	b	.L3535
.L3523:
	add	r6, r0, #61440
	ldr	r2, [r6, #1064]
	cmp	r2, #2
	strne	r3, [fp, #-52]
	addne	r5, r0, #57344
	strneb	r3, [r5, #1737]
	beq	.L3582
.L3553:
	ldr	r4, [r5, #3820]
	mov	lr, #0
	strb	lr, [r5, #1736]
	cmp	r4, lr
	beq	.L3543
	add	ip, r7, #27392
	add	r10, r7, #26368
	add	ip, ip, #8
	add	r10, r10, #160
	add	r9, r7, #24576
.L3542:
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3538
	cmp	ip, #0
	beq	.L3558
	ldr	r1, [r9, #2148]
	cmp	r1, #0
	beq	.L3558
	ldr	r2, [r9, #1952]
	cmp	r2, ip
	beq	.L3538
	mov	r2, r10
	b	.L3540
.L3541:
	ldr	r0, [r2, #4]!
	cmp	r0, ip
	beq	.L3538
.L3540:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3541
.L3558:
	mov	r9, #792
	movw	r2, #28064
	mla	r9, r9, lr, r7
	add	r0, r5, #1744
	mov	r10, #0
	mov	r3, #1
	mov	r4, ip
	str	ip, [r5, #2376]
	add	r1, r9, #27392
	strb	r3, [r5, #1736]
	mov	ip, #101
	add	r0, r0, #8
	str	r10, [r9, r2]
	add	r1, r1, #16
	mov	r2, #624
	str	ip, [r5, #2420]
	str	r3, [fp, #-48]
	bl	memcpy
	add	lr, r9, #27392
	str	r10, [r5, #1912]
	ldr	r3, [fp, #-48]
	strb	r10, [lr, #11]
	str	r10, [r4, #652]
	ldr	r2, [r5, #2376]
	strb	r10, [r2, #2]
	ldr	r2, [r5, #2376]
	strb	r3, [r2, #4]
	ldr	r3, [r5, #2376]
	str	r10, [r3, #784]
	ldrb	r4, [r5, #1736]	@ zero_extendqisi2
	cmp	r4, #1
	bne	.L3543
	ldr	r3, [fp, #-56]
	ldr	r0, [r6, #1136]
	clz	r1, r3
	mov	r1, r1, lsr #5
	bl	FSP_NewLogicFs
	mov	r1, r0
	str	r0, [fp, #-48]
	ldr	r0, [r6, #1136]
	bl	FSP_GetLogicFs
	ldr	r3, [fp, #-48]
	mvn	r2, r3
	mov	r2, r2, lsr #31
	cmp	r0, #0
	mov	r9, r0
	moveq	r2, #0
	cmp	r2, #0
	beq	.L3544
	ldr	r2, [r7]
	ldr	r2, [r2, #28]
	cmp	r2, #25
	beq	.L3591
.L3545:
	ldr	r4, .L3594
	mov	r2, #624
	ldr	r0, [r5, #2376]
	add	r1, r9, #8
	str	r3, [fp, #-48]
	ldr	ip, [r4, #52]
	add	r0, r0, #8
	blx	ip
	ldr	r1, [r5, #2376]
	add	r0, r7, #58880
	ldr	ip, [r4, #52]
	add	r1, r1, #8
	mov	r2, #624
	add	r0, r0, #216
	blx	ip
	ldr	r3, [fp, #-48]
	ldr	r2, [r5, #2376]
	mov	ip, r3
	str	r3, [r2, #632]
	ldrsb	r3, [r9, #4]
	ldr	r1, [r5, #2376]
	str	r3, [r5, #2444]
.L3535:
	ldrb	r2, [fp, #-52]	@ zero_extendqisi2
	strb	r2, [r5, #1739]
	ldrb	r3, [r8, #209]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	strb	r3, [r1, #676]
	ldrb	r3, [r5, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3592
	ldr	r3, [fp, #-52]
	strb	r2, [r5, #3160]
	cmp	r3, #0
	ldr	r3, [r8, #228]
	ldreq	r0, [fp, #-52]
	movne	r0, #0
	str	r3, [r5, #3168]
	ldr	r3, [r8, #240]
	str	r3, [r5, #3164]
	ldr	r3, [r6, #1148]
	str	ip, [r5, #3180]
	str	r3, [r5, #3176]
	movne	r3, #1
	moveq	r3, r0
	strb	r3, [r5, #3161]
.L3578:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3538:
	add	lr, lr, #1
	add	ip, ip, #792
	cmp	lr, r4
	bne	.L3542
.L3543:
	ldr	r1, .L3594+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r7
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3587:
	add	r6, r0, #61440
	ldr	r2, [r6, #1064]
	cmp	r2, #2
	beq	.L3593
	add	r5, r7, #57344
	ldrb	r2, [r5, #3160]	@ zero_extendqisi2
	strb	r3, [r5, #1737]
	mov	r3, #1
	cmp	r2, #2
	str	r3, [fp, #-52]
	bne	.L3553
	b	.L3549
.L3593:
	mov	r3, #1
.L3582:
	add	r5, r7, #57344
	str	r3, [fp, #-52]
.L3584:
	mov	r3, #0
	strb	r3, [r5, #1737]
	b	.L3553
.L3591:
	add	r1, r7, #12288
	ldr	r4, [r0, #148]
	ldr	r0, [r0, #52]
	ldr	r2, [r1, #1280]
	ldr	r1, [r1, #1276]
	mov	r2, r2, asl #4
	str	r2, [r9, #124]
	mov	r1, r1, asl #4
	str	r1, [r9, #120]
	mul	r4, r4, r2
	add	r0, r4, r0
	str	r0, [r9, #56]
	bl	MEM_Phy2Vir
	ldr	r1, [r9, #36]
	ldr	r2, [r9, #148]
	str	r10, [r9, #524]
	str	r0, [r9, #520]
	ldr	r0, [r9, #68]
	add	r0, r4, r0
	add	r4, r4, r1
	str	r0, [r9, #72]
	str	r4, [r9, #40]
	add	r4, r4, r2
	str	r4, [r9, #48]
	bl	MEM_Phy2Vir
	str	r10, [r9, #540]
	ldr	r3, [fp, #-48]
	str	r0, [r9, #536]
	b	.L3545
.L3592:
	mov	r3, #0
	strb	r3, [r5, #3160]
	ldr	r2, [r8, #228]
	mov	r0, r3
	str	r2, [r5, #3168]
	ldr	r2, [r8, #240]
	str	r2, [r5, #3164]
	ldr	r2, [r6, #1148]
	strb	r3, [r5, #3161]
	str	ip, [r5, #3180]
	str	r2, [r5, #3176]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3588:
	cmp	r3, #0
	beq	.L3529
	b	.L3553
.L3544:
	ldr	r3, [r5, #2376]
	mov	r0, r2
	strb	r2, [r5, #1736]
	ldr	r1, .L3594+8
	strb	r2, [r3, #4]
	bl	dprint_vfmw
	mov	r1, r4
	mov	r0, r7
	bl	ClearAll
	mvn	r0, #0
	b	.L3578
.L3590:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3584
.L3534:
	ldr	r1, [r5, #2376]
	cmp	r1, #0
	bne	.L3556
	b	.L3553
.L3589:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3584
	b	.L3534
.L3531:
	strb	r1, [r5, #1737]
	b	.L3553
.L3595:
	.align	2
.L3594:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC379
	.word	.LC380
	UNWIND(.fnend)
	.size	AllocFrameStore, .-AllocFrameStore
	.global	__aeabi_idivmod
	.align	2
	.global	DecGap
	.type	DecGap, %function
DecGap:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #52)
	sub	sp, sp, #52
	add	r10, r0, #24576
	mov	r6, r0
	add	r2, r0, #12288
	ldr	r0, [r10, #1812]
	add	r1, r6, #20480
	movw	r3, #2004
	str	r1, [fp, #-76]
	mov	ip, r1
	ldrb	r1, [r2, #1325]	@ zero_extendqisi2
	mul	r3, r3, r0
	cmp	r1, #1
	mov	r5, #0
	mov	r1, ip
	moveq	r1, #2
	movne	r1, r5
	str	r1, [fp, #-68]
	add	r1, r6, r3
	add	r1, r1, #22272
	ldr	r0, [ip, #228]
	add	r8, r6, #16384
	add	r4, r6, #57344
	mov	r3, r1
	add	r1, r1, #104
	str	r2, [fp, #-72]
	add	r3, r3, #108
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	ldr	r2, [r2, #3504]
	str	r0, [fp, #-60]
	str	r1, [fp, #-92]
	ldr	r0, [ip, #224]
	ldr	r1, [r8, #3472]
	ldr	ip, [r8, #3468]
	str	r3, [r4, #2380]
	ldr	r7, [r10, #2148]
	str	ip, [fp, #-84]
	cmp	r7, r5
	str	r1, [fp, #-88]
	str	r5, [fp, #-52]
	str	r5, [fp, #-48]
	beq	.L3612
	add	r2, r2, #4
	mov	r7, #1
	mov	r3, r7, asl r2
	add	r0, r0, r7
	str	r3, [fp, #-80]
	mov	r1, r3
	bl	__aeabi_uidivmod
	str	r5, [r8, #3472]
	mov	r0, #2
	str	r5, [r8, #3468]
	mov	r9, r1
	ldr	r1, .L3615
	bl	dprint_vfmw
	mov	r3, r9
	ldr	r2, [fp, #-60]
	mov	r0, #2
	ldr	r1, .L3615+4
	bl	dprint_vfmw
	sub	r2, fp, #52
	sub	r1, fp, #48
	mov	r0, r6
	bl	GetMaxPOC
	ldr	r3, [fp, #-60]
	cmp	r3, r9
	beq	.L3604
	mov	r3, r10
	str	r5, [fp, #-64]
	mov	r10, r4
	mov	r5, r7
	mov	r4, r3
	b	.L3605
.L3602:
	mov	r1, r7
	mov	r0, r6
	strb	r7, [r10, #1740]
	bl	StorePicInDPB
	ldr	r1, [fp, #-80]
	subs	r3, r0, #0
	add	r0, r9, #1
	bne	.L3613
	ldr	r3, [fp, #-76]
	str	r9, [r3, #224]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-60]
	cmp	r3, r1
	mov	r9, r1
	beq	.L3611
	ldr	r3, [r4, #2148]
	ldr	r2, [fp, #-64]
	cmp	r2, r3
	bhi	.L3611
.L3605:
	mov	r1, #1
	mov	r0, r6
	bl	AllocFrameStore
	mov	r7, #0
	ldr	r1, .L3615+8
	subs	r3, r0, #0
	mov	r0, #2
	bne	.L3614
	str	r3, [fp, #-56]
	bl	dprint_vfmw
	ldr	r1, [r10, #2376]
	strb	r5, [r10, #1742]
	ldr	r3, [fp, #-68]
	str	r9, [r1, #636]
	ldr	r1, [r10, #2376]
	str	r9, [r10, #2384]
	str	r9, [r1, #684]
	str	r3, [r10, #2392]
	ldr	r1, [r10, #2376]
	ldr	r3, [fp, #-56]
	str	r3, [r1, #680]
	ldr	r1, [r10, #2376]
	str	r5, [r1, #652]
	ldr	r1, [r10, #2376]
	strb	r3, [r1, #4]
	ldr	r1, [r10, #2376]
	strb	r5, [r1]
	ldr	r1, [r10, #2380]
	strb	r5, [r10, #1738]
	strb	r3, [r1, #3]
	ldr	r3, [fp, #-72]
	ldr	r3, [r3, #3508]
	cmp	r3, #0
	beq	.L3602
	str	r9, [r8, #3484]
	mov	r0, r6
	bl	DecPOC
	ldr	r3, [r8, #3464]
	ldr	r1, [fp, #-48]
	str	r3, [r10, #2396]
	ldr	r3, [r8, #3460]
	str	r3, [r10, #2400]
	ldr	r3, [r8, #3452]
	cmp	r3, r1
	str	r3, [r10, #2404]
	ldr	r3, [r8, #3456]
	str	r3, [r10, #2408]
	bls	.L3602
	cmp	r1, r3
	ldrcc	r3, [fp, #-64]
	addcc	r3, r3, #1
	strcc	r3, [fp, #-64]
	b	.L3602
.L3612:
	ldr	r1, .L3615+12
	mov	r0, r7
	bl	dprint_vfmw
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3611:
	mov	r4, r10
.L3604:
	ldr	r3, [fp, #-84]
	mov	r0, #0
	ldrb	r2, [fp, #-92]	@ zero_extendqisi2
	str	r3, [r8, #3468]
	ldr	r3, [fp, #-88]
	str	r3, [r8, #3472]
	ldr	r3, [r4, #2380]
	strb	r2, [r3, #3]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3614:
	ldr	r3, [fp, #-84]
	mov	r0, r7
	ldrb	r2, [fp, #-92]	@ zero_extendqisi2
	ldr	r1, .L3615+16
	str	r3, [r8, #3468]
	ldr	r3, [fp, #-88]
	str	r3, [r8, #3472]
	ldr	r3, [r10, #2380]
	strb	r2, [r3, #3]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3613:
	ldr	r2, [fp, #-84]
	mov	r0, r7
	ldrb	lr, [fp, #-92]	@ zero_extendqisi2
	ldr	r1, .L3615+20
	str	r2, [r8, #3468]
	ldr	r2, [fp, #-88]
	str	r2, [r8, #3472]
	movw	r2, #6545
	ldr	ip, [r10, #2380]
	strb	lr, [ip, #3]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3616:
	.align	2
.L3615:
	.word	.LC382
	.word	.LC383
	.word	.LC385
	.word	.LC381
	.word	.LC384
	.word	.LC386
	UNWIND(.fnend)
	.size	DecGap, .-DecGap
	.align	2
	.global	H264DEC_Init
	.type	H264DEC_Init, %function
H264DEC_Init:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r8, .L3630
	mov	r4, r0
	ldr	r5, [r0, #352]
	mov	r7, r1
	movw	r2, #62632
	mov	r1, #0
	ldr	r3, [r8, #48]
	blx	r3
	str	r5, [r4, #352]
	add	r0, r4, #8
	bl	ResetVoQueue
	mov	r3, #0
	str	r7, [r4]
	str	r3, [r4, #360]
	str	r3, [r4, #364]
	ldr	r3, [r7, #28]
	cmp	r3, #25
	beq	.L3627
	add	r6, r4, #12288
	mov	r1, #32
	mov	r2, #256
	mov	r3, #136
	str	r1, [r6, #1288]
	str	r2, [r6, #1292]
	str	r3, [r6, #1284]
.L3619:
	ldr	r0, [r7, #584]
	add	r5, r4, #61440
	bl	MEM_Phy2Vir
	ldr	lr, [r6, #1288]
	ldr	r3, [r4]
	mov	r2, #4000
	ldr	r1, [r6, #1292]
	mul	r2, r2, lr
	ldr	r3, [r3, #580]
	add	r3, r0, r3
	mov	r0, #2208
	add	ip, r3, r2
	str	r3, [r6, #1300]
	cmp	r3, #0
	cmpne	ip, #0
	str	ip, [r6, #1304]
	mla	r0, r0, r1, ip
	moveq	r9, #1
	movne	r9, #0
	cmp	r0, #0
	orreq	r9, r9, #1
	cmp	r9, #0
	str	r0, [r5, #1056]
	bne	.L3628
	ldr	r7, [r7, #608]
	mov	r10, r7
	adds	r7, r3, #0
	movne	r7, #1
	cmp	r10, #0
	movne	r7, #0
	cmp	ip, #0
	moveq	ip, #0
	andne	ip, r7, #1
	cmp	ip, #0
	beq	.L3622
	mov	r0, r3
	mov	r1, r9
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r2, [r6, #1292]
	mov	ip, #2208
	mov	r1, r9
	ldr	r3, [r8, #48]
	ldr	r0, [r6, #1304]
	mul	r2, ip, r2
	blx	r3
	ldr	r1, [r6, #1292]
	ldr	lr, [r6, #1288]
	ldr	r0, [r5, #1056]
.L3622:
	add	r3, r4, #16384
	movw	r2, #4060
	ldr	ip, [r8, #48]
	str	r1, [r3, #1236]
	mov	r1, #0
	ldr	r3, [r6, #1284]
	mov	r7, r1
	str	lr, [r6, #1344]
	mul	r2, r2, r3
	blx	ip
	add	r3, r4, #57344
	str	r7, [r5, #1080]
	mov	ip, #40
	str	r7, [r5, #1084]
	mov	r1, #17
	str	r7, [r5, #1088]
	mov	r2, #25
	str	r7, [r5, #1092]
	mov	r0, r4
	str	r7, [r5, #1096]
	str	r7, [r5, #1100]
	str	r7, [r5, #1104]
	str	r7, [r5, #1108]
	str	ip, [r3, #3820]
	str	r1, [r3, #3824]
	str	r7, [r5, #1072]
	str	r7, [r3, #3848]
	str	r7, [r3, #3852]
	str	r7, [r3, #3856]
	str	r7, [r3, #3860]
	str	r7, [r3, #3864]
	str	r7, [r3, #3828]
	str	r7, [r3, #3836]
	str	r7, [r5, #1140]
	str	r7, [r3, #3840]
	str	r2, [r3, #3844]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r5, #1136]
	beq	.L3629
	mov	r1, r7
	mov	r0, r4
	bl	ClearAll
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3627:
	ldr	r3, [r7, #684]
	add	r6, r4, #12288
	str	r3, [r6, #1288]
	ldr	r3, [r7, #688]
	str	r3, [r6, #1292]
	ldr	r3, [r7, #676]
	str	r3, [r6, #1284]
	b	.L3619
.L3628:
	movw	r3, #13089
	ldr	r2, .L3630+4
	ldr	r1, .L3630+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3629:
	mov	r0, r7
	ldr	r1, .L3630+12
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3631:
	.align	2
.L3630:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+268
	.word	.LC387
	.word	.LC388
	UNWIND(.fnend)
	.size	H264DEC_Init, .-H264DEC_Init
	.align	2
	.global	InitPic
	.type	InitPic, %function
InitPic:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #36)
	sub	sp, sp, #36
	add	r10, r0, #12288
	add	r8, r0, #20480
	mov	r5, r0
	ldr	r0, [r10, #1304]
	mov	r9, #2208
	ldr	ip, [r8, #220]
	add	r4, r5, #57344
	ldr	r3, [r10, #1300]
	mov	r1, #4000
	ldrb	r2, [r4, #1736]	@ zero_extendqisi2
	mla	r9, r9, ip, r0
	cmp	r2, #0
	addeq	r7, r5, #61440
	ldr	r2, [r9, #28]
	mla	r3, r1, r2, r3
	str	r3, [fp, #-56]
	beq	.L3633
	ldr	r3, [r4, #2376]
	cmp	r3, #0
	beq	.L3634
	ldrb	r6, [r3, #1]	@ zero_extendqisi2
	add	r7, r5, #61440
	cmp	r6, #0
	beq	.L3769
.L3635:
	mov	r3, #0
	strb	r3, [r4, #1736]
.L3633:
	sub	r1, fp, #48
	mov	r0, r5
	bl	GetReRangeFlag
	ldr	r3, [fp, #-56]
	ldr	r2, [r10, #1344]
	ldr	r3, [r3, #36]
	cmp	r2, r3
	mov	r6, r0
	beq	.L3770
.L3636:
	ldr	r1, [fp, #-56]
	add	r0, r5, #13568
	ldr	r3, .L3786
	mov	r2, #0
	add	r0, r0, #28
	strb	r2, [r1, #24]
	mov	r2, #4000
	ldr	r3, [r3, #56]
	blx	r3
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r5]
	beq	.L3638
.L3783:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L3639
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L3771
.L3639:
	ldr	r3, [r7, #1096]
	cmp	r6, #0
	add	r3, r3, #1
	str	r3, [r7, #1096]
	bne	.L3650
	ldr	r3, [r5]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L3650
	mov	r0, r5
	bl	FlushDPB
	cmp	r0, #0
	bne	.L3772
	mov	r0, r5
	bl	InitDPB
	cmp	r0, #0
	bne	.L3702
.L3655:
	ldr	r3, [r5]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L3658
.L3763:
	add	r6, r5, #16384
.L3649:
	ldr	r2, [r6, #1236]
	ldr	r3, [r9, #24]
	cmp	r2, r3
	bne	.L3661
	ldrb	r3, [r9, #20]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3661
.L3662:
	ldr	r3, [r6, #3444]
	ldr	r2, [r10, #3504]
	cmp	r3, #0
	movne	r3, #0
	strne	r3, [r8, #224]
	strne	r3, [r4, #3168]
	ldr	r3, [r7, #1064]
	cmp	r3, #2
	beq	.L3664
	ldr	ip, [r5]
	ldr	r3, [ip, #12]
	str	ip, [fp, #-60]
	sub	r3, r3, #2
	cmp	r3, #1
	bls	.L3664
	ldr	r3, [r8, #228]
	ldr	r9, [r8, #224]
	cmp	r3, r9
	str	r3, [fp, #-64]
	beq	.L3665
	mov	r1, #1
	add	r2, r2, #4
	mov	r2, r1, asl r2
	add	r0, r9, r1
	str	r2, [fp, #-68]
	mov	r1, r2
	bl	__aeabi_uidivmod
	ldr	r3, [fp, #-64]
	cmp	r3, r1
	beq	.L3703
	ldr	ip, [fp, #-60]
	ldr	r1, [ip, #600]
	cmp	r1, #0
	bne	.L3666
	ldr	r1, [ip, #640]
	ldr	r2, [fp, #-68]
	cmp	r1, #1
	beq	.L3773
.L3667:
	cmp	r3, r9
	bls	.L3668
	ldr	r1, [r6, #484]
	rsb	r9, r9, r3
	cmp	r1, r9
	bcc	.L3774
.L3669:
	ldrb	r3, [r8, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3666
.L3674:
	ldr	r3, [r7, #1132]
	cmp	r3, #0
	bne	.L3666
	mov	r0, r5
	bl	H264_ClearCurrPic
	ldr	r3, .L3786+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3775
.L3675:
	ldr	ip, [r6, #484]
	movw	r2, #6890
.L3764:
	ldr	r1, [r8, #228]
	mov	r0, #0
	ldr	r3, [r8, #224]
	str	ip, [sp]
	rsb	r3, r3, r1
	ldr	r1, .L3786+8
	bl	dprint_vfmw
	mvn	r0, #0
.L3757:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3661:
	ldr	r3, .L3786
	add	r0, r5, #17408
	mov	r2, #0
	mov	r1, r9
	strb	r2, [r9, #20]
	add	r0, r0, #188
	ldr	r3, [r3, #56]
	mov	r2, #2208
	blx	r3
	b	.L3662
.L3650:
	ldr	r1, .L3786+12
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, [r7, #1176]
	cmp	r3, #1
	beq	.L3776
.L3652:
	mov	r0, r5
	bl	InitDPB
	cmp	r0, #0
	bne	.L3702
	cmp	r6, #0
	beq	.L3655
.L3658:
	add	r6, r5, #16384
	add	r3, r5, #60928
	add	r3, r3, #236
	mov	r0, r5
	ldr	ip, [r6, #484]
	str	ip, [r4, #3824]
	ldr	r2, [r6, #464]
	ldrb	r1, [r10, #1326]	@ zero_extendqisi2
	add	r2, r2, #1
	ldr	lr, [r6, #460]
	rsb	r1, r1, #2
	str	ip, [sp]
	mov	r2, r2, asl #4
	add	ip, lr, #1
	mul	r2, r1, r2
	mov	ip, ip, asl #4
	str	ip, [fp, #-60]
	mov	r1, ip
	str	r2, [fp, #-64]
	bl	H264_ArrangeVahbMem
	ldr	ip, [fp, #-60]
	cmp	r0, #1
	beq	.L3777
.L3762:
	ldr	r3, [r5]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	mvneq	r0, #1
	beq	.L3757
	mov	r4, #0
	ldr	r1, .L3786+16
	str	r4, [r10, #1276]
	mov	r0, r4
	str	r4, [r10, #1280]
	bl	dprint_vfmw
	mov	r1, r4
	mov	r0, r5
	bl	ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3703:
	mov	r9, r3
.L3665:
	ldr	r3, [r8, #240]
	mov	r0, r5
	cmp	r3, #0
	strne	r9, [r8, #224]
	str	r9, [r6, #3484]
	bl	DecPOC
	mov	r1, #0
	mov	r0, r5
	bl	AllocFrameStore
	cmp	r0, #0
	bne	.L3778
	strb	r0, [r4, #1743]
	mov	r1, #1
	strb	r1, [r4, #1736]
	mvn	ip, #0
	ldrb	r3, [r8, #211]	@ zero_extendqisi2
	strb	r3, [r4, #1742]
	ldr	r2, [r8, #228]
	str	r2, [r4, #2384]
	ldr	r3, [r8, #240]
	str	r2, [fp, #-60]
	str	r3, [r4, #2392]
	ldr	lr, [r6, #3464]
	str	r3, [fp, #-64]
	str	lr, [r4, #2396]
	ldr	lr, [r6, #3460]
	str	lr, [r4, #2400]
	ldr	lr, [r6, #3452]
	str	lr, [r4, #2404]
	ldr	lr, [r6, #3456]
	strb	r0, [r4, #1738]
	strb	r0, [r4, #1741]
	str	lr, [r4, #2408]
	strb	r0, [r4, #1740]
	str	ip, [r4, #2440]
	ldrb	ip, [r8, #209]	@ zero_extendqisi2
	cmp	ip, #0
	strneb	r1, [r8, #209]
	ldrb	r0, [r10, #1326]	@ zero_extendqisi2
	ldr	lr, [r6, #464]
	movne	ip, r1
	rsb	r9, r0, #2
	movne	r1, #2
	str	ip, [fp, #-68]
	mla	r9, lr, r9, r9
	mov	r0, r9
	bl	__aeabi_uidiv
	str	r9, [r4, #2432]
	ldr	ip, [fp, #-68]
	ldr	r2, [fp, #-60]
	cmp	ip, #0
	ldr	r3, [fp, #-64]
	str	r0, [r4, #2428]
	ldr	r1, [r6, #460]
	add	r1, r1, #1
	str	r1, [r4, #2424]
	mul	r1, r1, r0
	movne	r0, #0
	str	r1, [r4, #2436]
	ldrb	r1, [r6, #1212]	@ zero_extendqisi2
	strb	r1, [r4, #1744]
	movne	r1, r0
	bne	.L3687
	ldrb	r1, [r10, #1327]	@ zero_extendqisi2
	cmp	r1, #0
	movne	r0, #1
	movne	r1, r0
	beq	.L3779
.L3687:
	strb	r1, [r4, #1745]
	add	r1, r5, #24576
	str	r0, [r7, #1052]
	cmp	r3, #0
	strne	r2, [r8, #224]
	movw	r3, #2004
	ldr	lr, [r1, #1812]
	add	r0, r5, #59648
	add	r1, r0, #64
	movw	r2, #59696
	str	r1, [fp, #-60]
	movw	ip, #59712
	mla	r3, r3, lr, r5
	ldr	lr, [fp, #-60]
	ldr	r1, .L3786+20
	mov	r0, #29
	add	r3, r3, #22272
	add	r3, r3, #108
	str	r3, [r4, #2380]
	ldr	r3, [r5]
	ldrd	r8, [r3, #56]
	strd	r8, [r5, r2]
	ldrd	r8, [r3, #72]
	strd	r8, [lr, #-8]
	ldrd	r8, [r3, #88]
	ldr	lr, [fp, #-56]
	strd	r8, [r5, ip]
	mvn	r8, #0
	ldr	ip, [r3, #104]
	mvn	r9, #0
	str	ip, [r4, #1760]
	ldr	ip, [r3, #108]
	str	ip, [r4, #1764]
	ldr	ip, [r3, #112]
	str	ip, [r4, #1768]
	ldr	ip, [r3, #128]
	str	ip, [r4, #1772]
	ldr	ip, [lr, #44]
	str	ip, [r4, #1928]
	ldr	ip, [lr, #48]
	str	ip, [r4, #1932]
	strd	r8, [r3, #56]
	ldrd	r2, [r2, r5]
	bl	dprint_vfmw
	ldr	lr, [fp, #-60]
	ldr	r1, .L3786+24
	mov	r0, #29
	ldrd	r2, [lr, #-8]
	bl	dprint_vfmw
	ldr	r3, [r6, #460]
	add	r0, r5, #58880
	add	r3, r3, #1
	add	r0, r0, #216
	mov	r3, r3, asl #4
	str	r3, [r4, #1864]
	ldrb	r2, [r10, #1326]	@ zero_extendqisi2
	ldr	r1, [r6, #464]
	rsb	r3, r2, #2
	mla	r3, r1, r3, r3
	mov	r3, r3, asl #4
	str	r3, [r4, #1868]
	ldr	r2, [r10, #1348]
	ldr	r3, [r6, #532]
	subs	r2, r2, #1
	ldr	r1, [r6, #528]
	movne	r2, #1
	and	r1, r1, #7
	mov	r3, r3, asl #5
	mov	r2, r2, asl #2
	orr	r2, r2, r1, asl #14
	uxtb	r3, r3
	orr	r3, r2, r3
	str	r3, [r4, #1860]
	bl	SetAspectRatio
	ldrb	r3, [r4, #1739]	@ zero_extendqisi2
	ldr	r2, [r4, #2376]
	adds	r3, r3, #0
	ldr	r1, [r7, #1096]
	movne	r3, #1
	cmp	r2, #0
	str	r3, [r4, #1848]
	str	r1, [r4, #1916]
	beq	.L3780
	ldr	r3, [r4, #1860]
	ldr	r1, [fp, #-56]
	str	r3, [r2, #116]
	ldr	r3, [r4, #2376]
	ldr	r2, [r4, #1848]
	str	r2, [r3, #104]
	ldr	r3, [r4, #2376]
	ldr	r2, [r4, #1756]
	str	r2, [r3, #12]
	ldr	r3, [r4, #2376]
	ldr	r2, [r4, #1752]
	str	r2, [r3, #8]
	ldr	r3, [r4, #2376]
	ldr	r2, [r4, #1864]
	str	r2, [r3, #120]
	ldr	r3, [r4, #2376]
	ldr	r2, [r4, #1868]
	str	r2, [r3, #124]
	ldr	r3, [r1, #3284]
	ldr	r2, [r4, #2376]
	str	r3, [r2, #136]
	str	r3, [r4, #1880]
	ldr	r2, [r4, #2376]
	ldr	r3, [r1, #3288]
	str	r3, [r2, #140]
	str	r3, [r4, #1884]
	ldr	r2, [r4, #2376]
	ldr	r3, [r1, #3276]
	str	r3, [r2, #128]
	str	r3, [r4, #1872]
	ldr	r2, [r4, #2376]
	ldr	r3, [r1, #3280]
	str	r3, [r2, #132]
	str	r3, [r4, #1876]
	ldr	r3, [r4, #2376]
	ldr	r0, [r7, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_GetDispPhyFs
	cmp	r0, #0
	beq	.L3690
	ldr	r3, [r4, #1848]
	mov	r1, #0
	ldr	r2, [r0, #8]
	subs	r3, r3, r1
	ldr	r0, [r7, #1136]
	movne	r3, #1
	bl	FSP_SetStoreType
.L3690:
	add	lr, r5, #59648
	add	r6, r5, #61184
	ldr	r3, [r4, #3848]
	add	lr, lr, #32
	add	r6, r6, #12
	add	ip, r5, #59648
	mov	r8, #0
.L3694:
	ldrd	r0, [ip, #8]!
	orrs	r2, r0, r1
	bne	.L3691
	cmp	r3, #0
	beq	.L3691
	ldr	r2, [r4, #3852]
	subs	r3, r3, #1
	stmia	ip, {r2, r8}
	beq	.L3692
	mov	r1, r6
	mov	r2, #0
.L3693:
	add	r2, r2, #1
	ldr	r0, [r1, #4]!
	cmp	r2, r3
	str	r0, [r1, #-4]
	bne	.L3693
.L3692:
	add	r2, r3, #15296
	add	r2, r2, #2
	add	r2, r5, r2, lsl #2
	str	r8, [r2, #4]
	str	r3, [r4, #3848]
.L3691:
	cmp	ip, lr
	bne	.L3694
	cmp	r3, #0
	beq	.L3696
	mov	r8, r3
	ble	.L3696
	add	r6, r3, #15296
	mov	r9, #0
	add	r6, r6, #3
	add	r6, r5, r6, lsl #2
.L3697:
	ldr	r1, [r6, #-4]!
	sub	r8, r8, #1
	ldr	r0, [r7, #1136]
	bl	FreeUsdByDec
	cmp	r8, #0
	str	r9, [r6]
	bne	.L3697
.L3696:
	mov	r0, r5
	mov	r5, #0
	str	r5, [r4, #3848]
	str	r5, [r4, #2412]
	bl	CalcPicNum
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3676:
	cmp	r3, #0
	mov	r0, r5
	subeq	r3, r2, #1
	subne	r3, r3, #1
	str	r3, [r8, #224]
	bl	FlushDPB
	subs	r3, r0, #0
	bne	.L3781
	ldrb	r3, [r8, #208]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3681
.L3666:
	mov	r0, r5
	bl	DecGap
	cmp	r0, #0
	bne	.L3782
.L3664:
	ldr	r9, [r8, #228]
	b	.L3665
.L3770:
	ldr	r3, [fp, #-56]
	ldrb	r3, [r3, #24]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3636
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r5]
	bne	.L3783
.L3638:
	ldr	r3, [r3, #872]
	cmp	r3, #1
	bne	.L3763
	b	.L3639
.L3779:
	mov	r0, ip
	b	.L3687
.L3771:
	ldr	r3, [r7, #1096]
	cmp	r6, #0
	add	r3, r3, #1
	str	r3, [r7, #1096]
	beq	.L3640
	mov	r0, r5
	str	r2, [fp, #-60]
	bl	FlushDPB
	ldr	r2, [fp, #-60]
	subs	r3, r0, #0
	bne	.L3784
	mov	r0, r5
	bl	InitDPB
	subs	r3, r0, #0
	bne	.L3698
	add	r6, r5, #16384
	ldrb	r3, [r10, #1326]	@ zero_extendqisi2
	ldr	r0, [r10, #1276]
	ldr	r1, [r6, #464]
	rsb	r2, r3, #2
	ldr	ip, [r6, #460]
	cmp	r0, #0
	add	r3, r1, #1
	add	ip, ip, #1
	mov	r3, r3, asl #4
	mov	ip, ip, asl #4
	mul	r3, r2, r3
	str	r3, [fp, #-60]
	bne	.L3647
	ldr	r0, [r6, #484]
	str	r0, [r4, #3824]
	ldr	r3, [r5]
	ldr	r1, [r3, #668]
	cmp	r1, ip
	bcc	.L3643
	ldr	r2, [r3, #672]
	ldr	lr, [fp, #-60]
	cmp	r2, lr
	str	r2, [fp, #-64]
	bcc	.L3643
	add	lr, r5, #24576
	ldr	r2, [r3, #692]
	ldr	lr, [lr, #2144]
	cmp	r2, lr
	bcc	.L3785
	ldr	lr, [r3, #696]
	str	ip, [fp, #-68]
	cmp	lr, #0
	moveq	r0, #1
	streq	r0, [r4, #3824]
	ldreq	r1, [r3, #668]
	ldreq	r3, [r3, #672]
	str	r0, [sp]
	mov	r0, r5
	streq	r3, [fp, #-64]
	add	r3, r5, #60928
	ldr	r2, [fp, #-64]
	add	r3, r3, #236
	bl	H264_ArrangeVahbMem
	ldr	ip, [fp, #-68]
	cmp	r0, #1
	bne	.L3762
	b	.L3647
.L3769:
	ldr	r1, [r3, #632]
	mov	r2, #1
	ldr	r0, [r7, #1136]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2376]
	strb	r6, [r3, #1]
	ldr	r3, [r4, #2376]
	strb	r6, [r3, #4]
	b	.L3635
.L3777:
	ldr	r3, [fp, #-64]
	mov	ip, ip, lsr #4
	str	ip, [r10, #1276]
	mov	r3, r3, lsr #4
	str	r3, [r10, #1280]
	b	.L3649
.L3776:
	mov	r0, r5
	bl	GetBackPicFromVOQueue
	b	.L3652
.L3668:
	rsb	r1, r9, r3
	ldr	r0, [r6, #484]
	add	r1, r1, r2
	cmp	r0, r1
	bcs	.L3666
	ldrb	r1, [r8, #208]	@ zero_extendqisi2
	cmp	r1, #2
	beq	.L3676
.L3681:
	ldr	r3, [r7, #1132]
	cmp	r3, #0
	bne	.L3666
	mov	r0, r5
	bl	H264_ClearCurrPic
	ldr	r3, .L3786+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3682
	mov	r0, r5
	bl	ClearDPB
.L3682:
	ldr	ip, [r6, #484]
	movw	r2, #6922
	b	.L3764
.L3640:
	mov	r0, r5
	bl	InitDPB
	subs	r3, r0, #0
	bne	.L3698
	add	r6, r5, #16384
	ldrb	r3, [r10, #1326]	@ zero_extendqisi2
	ldr	r1, [r6, #464]
	rsb	r2, r3, #2
	ldr	ip, [r6, #460]
	add	r3, r1, #1
	add	ip, ip, #1
	mov	r3, r3, asl #4
	mov	ip, ip, asl #4
	mul	r3, r2, r3
	str	r3, [fp, #-60]
.L3647:
	ldr	r3, [fp, #-60]
	mov	ip, ip, lsr #4
	str	ip, [r10, #1276]
	mov	r3, r3, lsr #4
	str	r3, [r10, #1280]
	b	.L3649
.L3634:
	movw	r3, #6623
	ldr	r2, .L3786+28
	ldr	r1, .L3786+32
	mov	r0, #22
	bl	dprint_vfmw
	add	r7, r5, #61440
	b	.L3635
.L3774:
	ldrb	r1, [r8, #208]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L3674
	cmp	r3, #0
	mov	r0, r5
	subeq	r3, r2, #1
	subne	r3, r3, #1
	str	r3, [r8, #224]
	bl	FlushDPB
	subs	r3, r0, #0
	beq	.L3669
	movw	r2, #6873
	ldr	r1, .L3786+36
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3782:
	movw	r2, #6934
	ldr	r1, .L3786+40
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3773:
	ldr	r1, [ip, #636]
	cmp	r1, #0
	bne	.L3667
	ldr	r1, [ip, #644]
	cmp	r1, #0
	bne	.L3667
	ldr	r1, [ip, #604]
	add	r1, r1, #1024
	cmp	r1, #2048
	bhi	.L3666
	b	.L3667
.L3778:
	movw	r2, #6958
	ldr	r1, .L3786+44
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3702:
	mov	r3, r0
	movw	r2, #6792
	ldr	r1, .L3786+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3780:
	movw	r3, #7045
	ldr	r2, .L3786+28
	ldr	r1, .L3786+52
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3772:
	mov	r3, r0
	movw	r2, #6783
	mov	r0, r6
	ldr	r1, .L3786+36
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3643:
	ldr	r1, .L3786+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3784:
	mov	r0, r2
	ldr	r1, .L3786+36
	movw	r2, #6657
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3698:
	movw	r2, #6666
	ldr	r1, .L3786+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3775:
	mov	r0, r5
	bl	ClearDPB
	b	.L3675
.L3781:
	movw	r2, #6906
	ldr	r1, .L3786+36
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3785:
	ldr	r1, .L3786+60
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3757
.L3787:
	.align	2
.L3786:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_apple_airplay_mirrorinig_flag
	.word	.LC395
	.word	.LC394
	.word	.LC393
	.word	.LC398
	.word	.LC399
	.word	.LANCHOR1+284
	.word	.LC389
	.word	.LC354
	.word	.LC396
	.word	.LC397
	.word	.LC390
	.word	.LC400
	.word	.LC391
	.word	.LC392
	UNWIND(.fnend)
	.size	InitPic, .-InitPic
	.align	2
	.global	H264_DecSlice
	.type	H264_DecSlice, %function
H264_DecSlice:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	add	r5, r0, #61440
	mov	r4, r0
	ldr	r1, .L3842
	ldr	r2, [r5, #1080]
	mov	r0, #22
	add	r6, r4, #20480
	sub	r2, r2, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #368]
	mov	r0, r4
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r6, #211]
	ldr	r3, [r3, #76]
	str	r3, [r6, #240]
	bl	ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L3835
	mov	r0, r4
	bl	ProcessSliceHeaderSecondPart
	cmp	r0, #0
	bne	.L3835
	ldr	r2, [r5, #1060]
	cmp	r2, #0
	moveq	r3, r2
	beq	.L3795
	add	r9, r4, #60416
	mov	r3, r0
	add	r9, r9, #132
	mov	r1, r0
	mov	r8, r0
	b	.L3803
.L3836:
	cmp	r7, r1
	add	r3, ip, r3
	rsbhi	r0, r0, r10
	rsbhi	r1, r1, r7
	addhi	r0, r1, r0
	movls	r0, r10
	add	r3, r3, r0
.L3801:
	cmp	lr, #0
	add	r8, r8, #1
	movne	r1, lr
	movne	r0, ip
	moveq	r1, r7
	moveq	r0, r10
	cmp	r8, r2
	beq	.L3795
.L3803:
	ldr	ip, [r9, #4]!
	ldr	r7, [ip, #8]
	ldr	lr, [ip, #36]
	cmp	r7, #0
	ldrne	r10, [ip, #12]
	moveq	r10, r7
	cmp	lr, #0
	ldrne	ip, [ip, #40]
	moveq	ip, lr
	cmp	r1, #0
	bne	.L3836
	add	r3, r10, r3
	add	r3, r3, ip
	b	.L3801
.L3795:
	add	r7, r4, #57344
	str	r3, [r5, #1120]
	movw	r0, #1620
	ldr	ip, [r7, #2428]
	ldr	r1, [r7, #2424]
	mul	r1, r1, ip
	cmp	r1, r0
	movgt	r1, r1, asl #8
	ble	.L3837
.L3805:
	add	r8, r4, #12288
	ldr	r0, [r8, #1284]
	cmp	r0, r2
	bls	.L3806
	cmp	r1, r3
	bcc	.L3806
	ldrb	r3, [r6, #213]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3838
.L3809:
	mov	r0, r4
	bl	PicTypeStatistic
	mov	r0, r4
	bl	DecList
	cmp	r0, #0
	bne	.L3839
	mov	r0, r4
	bl	IsRefListWrong
	subs	r6, r0, #0
	bne	.L3840
	mov	r0, r4
	bl	ExitSlice
	mov	r0, r4
	bl	CalcStreamBits
	mov	r0, r4
	bl	WriteSliceMsg
	mov	r3, #1
	str	r3, [r7, #3828]
	mov	r0, r6
	ldr	r3, [r5, #1116]
	add	r3, r3, #1
	str	r3, [r5, #1116]
.L3792:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3837:
	mov	r0, r1, asl #9
	sub	r1, r0, r1, asl #7
	cmp	r1, #16384
	movlt	r1, #16384
	b	.L3805
.L3838:
	ldr	r1, .L3842+4
	mov	r0, #2
	bl	dprint_vfmw
	ldrb	r3, [r6, #211]	@ zero_extendqisi2
	mov	r2, #0
	str	r2, [r5, #1116]
	cmp	r3, #5
	ldr	r3, [r5, #1100]
	mov	r0, r4
	streq	r3, [r5, #1112]
	add	r3, r3, #1
	str	r3, [r5, #1100]
	ldrb	r3, [r7, #1739]	@ zero_extendqisi2
	cmp	r3, r2
	ldreq	r3, [r5, #1104]
	ldrne	r3, [r5, #1108]
	addeq	r3, r3, #1
	addne	r3, r3, #1
	streq	r3, [r5, #1104]
	strne	r3, [r5, #1108]
	bl	InitPic
	cmn	r0, #2
	beq	.L3792
	cmp	r0, #0
	bne	.L3841
	mov	r0, r4
	bl	WritePicMsg
	b	.L3809
.L3835:
	ldr	r1, .L3842+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3842+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3833
	sub	r2, fp, #44
	mvn	r1, #0
	ldr	r0, [r5, #1136]
	mov	r3, #4
	str	r1, [r2, #-8]!
	mov	r1, #100
	blx	ip
.L3833:
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	b	.L3792
.L3806:
	str	r1, [sp]
	mov	r0, #0
	ldr	r1, .L3842+16
	bl	dprint_vfmw
	ldr	r2, [r8, #1284]
	ldr	r3, [r5, #1060]
	cmp	r3, r2
	bcc	.L3808
	ldr	r1, .L3842+12
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L3808
	str	r3, [fp, #-52]
	mov	r1, #108
	str	r2, [fp, #-48]
	mov	r3, #8
	sub	r2, fp, #52
	ldr	r0, [r5, #1136]
	blx	ip
.L3808:
	ldr	r1, .L3842+20
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3792
.L3839:
	mov	r2, r0
	ldr	r1, .L3842+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3792
.L3840:
	ldr	r1, .L3842+28
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3833
.L3841:
	ldr	r1, .L3842+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3792
.L3843:
	.align	2
.L3842:
	.word	.LC344
	.word	.LC403
	.word	.LC345
	.word	g_event_report
	.word	.LC401
	.word	.LC402
	.word	.LC405
	.word	.LC406
	.word	.LC404
	UNWIND(.fnend)
	.size	H264_DecSlice, .-H264_DecSlice
	.align	2
	.global	H264_DecOneNal
	.type	H264_DecOneNal, %function
H264_DecOneNal:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	mov	r5, r1
	ldr	r1, [r0, #368]
	add	r4, r0, #61440
	mov	r6, r0
	ldr	r3, [r1, #68]
	cmp	r3, #0
	movne	ip, r1
	movne	r2, #0
	ldrne	r3, [r4, #1120]
	beq	.L3849
.L3848:
	ldr	lr, [ip, #12]
	add	r2, r2, #1
	add	ip, ip, #28
	add	r3, r3, lr
	str	r3, [r4, #1120]
	ldr	lr, [r1, #68]
	cmp	lr, r2
	bhi	.L3848
.L3849:
	add	r8, r6, #13504
	mov	r9, #0
	add	r8, r8, #24
	strb	r9, [r1]
	mov	r1, #32
	mov	r0, r8
	bl	BsGet
	ldr	r2, [r6, #368]
	ldr	r1, .L4010
	ldr	r3, [r2, #64]
	add	r3, r3, #32
	str	r3, [r2, #64]
	ldr	r3, [r6, #368]
	and	r10, r0, #31
	strb	r10, [r3, #2]
	ldr	r3, [r6, #368]
	ubfx	lr, r0, #5, #2
	ubfx	ip, r0, #7, #1
	mov	r7, r0
	mov	r0, #22
	str	lr, [r3, #76]
	ldr	r3, [r6, #368]
	str	ip, [r3, #80]
	str	r9, [r4, #1076]
	ldr	r3, [r6, #368]
	ldr	r2, [r4, #1080]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	bl	dprint_vfmw
	ldr	r1, [r4, #1080]
	movw	r3, #371
	cmp	r1, r3
	beq	.L4003
.L3847:
	add	r1, r1, #1
	str	r1, [r4, #1080]
	ldr	r3, [r6, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	cmp	r2, #20
	beq	.L4004
.L3850:
	sub	r3, r2, #1
	cmp	r3, #29
	ldrls	pc, [pc, r3, asl #2]
	b	.L3858
.L3860:
	.word	.L3859
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3859
	.word	.L3861
	.word	.L3862
	.word	.L3863
	.word	.L3864
	.word	.L3865
	.word	.L3866
	.word	.L3867
	.word	.L3868
	.word	.L3858
	.word	.L3862
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3869
	.word	.L3859
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3858
	.word	.L3870
.L3870:
	mov	r2, #30
	ldr	r1, .L4010+4
	mov	r0, #22
	bl	dprint_vfmw
	cmp	r5, #0
	bne	.L3885
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3885
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	str	r5, [r6, #368]
.L3885:
	mov	r1, #32
	mov	r0, r8
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r8
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L3989
	ldr	r5, .L4010+8
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3886
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #1136]
	blx	ip
.L3886:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, r7
	beq	.L3989
.L3998:
	ldr	ip, [r5]
	cmp	ip, r7
	beq	.L3984
.L3986:
	ldr	r0, [r4, #1136]
	mov	r3, r7
	mov	r2, r7
	mov	r1, #113
	blx	ip
.L3984:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3859:
	ldr	r3, [r4, #1092]
	mov	r0, #22
	ldr	r1, .L4010+12
	add	r5, r6, #20480
	bl	dprint_vfmw
	ldr	r3, [r6, #368]
	mov	r0, r6
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r5, #211]
	ldr	r2, [r3, #76]
	str	r2, [r5, #240]
	ldrb	r3, [r3, #5]	@ zero_extendqisi2
	strb	r3, [r5, #216]
	bl	SliceCheck
	subs	r7, r0, #0
	bne	.L4005
	mov	r0, r6
	bl	H264_DecSlice
	cmn	r0, #2
	beq	.L3982
	cmp	r0, #0
	beq	.L3873
	ldr	r3, .L4010+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3986
	b	.L3984
.L3861:
	ldr	r1, .L4010+16
	mov	r2, #6
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_DecSEI
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3881
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3881:
	cmp	r5, #0
	bne	.L4006
.L3989:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3862:
	ldr	r1, .L4010+20
	mov	r0, #22
	ldr	r3, [r4, #1084]
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_DecSPS
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3879
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3879:
	cmp	r5, #0
	bne	.L4007
	ldr	r3, [r4, #1084]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #1084]
	b	.L3982
.L3866:
	mov	r2, #11
	ldr	r1, .L4010+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_IsEosNal
	cmp	r0, #1
	beq	.L4008
.L3991:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3989
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r6, #368]
.L3982:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3867:
	ldr	r1, .L4010+28
	mov	r2, #12
.L3993:
	mov	r0, #22
	bl	dprint_vfmw
	b	.L3991
.L3868:
	mov	r2, #13
	ldr	r1, .L4010+32
	b	.L3993
.L3869:
	mov	r2, #19
	ldr	r1, .L4010+36
	b	.L3993
.L3864:
	mov	r2, #9
	ldr	r1, .L4010+40
	b	.L3993
.L3865:
	mov	r2, #10
	ldr	r1, .L4010+44
	b	.L3993
.L3863:
	ldr	r1, .L4010+48
	mov	r2, #8
	ldr	r3, [r4, #1088]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_DecPPS
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3877
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3877:
	cmp	r5, #0
	bne	.L4009
	ldr	r3, [r4, #1088]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #1088]
	b	.L3982
.L3858:
	mov	r3, r7
	ldr	r1, .L4010+52
	mov	r0, #22
	ldr	r5, .L4010+8
	bl	dprint_vfmw
	cmp	r7, #256
	beq	.L3887
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3888
	mov	r3, #0
	mov	r1, #102
	mov	r2, r3
	ldr	r0, [r4, #1136]
	blx	ip
.L3887:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3889
.L3890:
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3889:
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3989
.L3988:
	mov	r3, #0
	ldr	r0, [r4, #1136]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L3989
.L4004:
	mov	r1, #24
	mov	r0, r8
	bl	BsGet
	ldr	r3, [r6, #368]
	add	r1, r6, #57344
	ubfx	r2, r0, #23, #1
	mov	r7, r0
	strb	r2, [r3, #3]
	ldr	r0, [r6, #368]
	ldrb	r3, [r0, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3851
	ldrb	r2, [r1, #3200]	@ zero_extendqisi2
	ldrb	r3, [r1, #3198]	@ zero_extendqisi2
.L3852:
	cmp	r2, #0
	bne	.L3853
	cmp	r3, #1
	beq	.L3854
.L3853:
	ldr	r1, .L4010+56
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3984
	ldr	r0, [r4, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	mvn	r0, #0
	str	r3, [r6, #368]
	b	.L3982
.L4003:
	ldr	r1, .L4010+60
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r1, [r4, #1080]
	b	.L3847
.L3851:
	ubfx	r2, r7, #22, #1
	ubfx	ip, r7, #16, #6
	ubfx	r3, r7, #12, #3
	strb	r2, [r1, #3196]
	strb	ip, [r1, #3197]
	ubfx	r2, r7, #5, #3
	ubfx	ip, r7, #4, #1
	strb	r3, [r1, #3199]
	strb	r2, [r1, #3201]
	ubfx	r3, r7, #3, #1
	ubfx	r2, r7, #2, #1
	strb	ip, [r1, #3202]
	strb	r3, [r1, #3203]
	and	ip, r7, #3
	ubfx	r3, r7, #15, #1
	strb	r2, [r1, #3204]
	strb	ip, [r1, #3205]
	ubfx	r2, r7, #8, #4
	strb	r3, [r1, #3198]
	strb	r2, [r1, #3200]
	b	.L3852
.L4005:
	ldr	r1, .L4010+64
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	bl	ClearCurrSlice
	ldr	r3, .L4010+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3984
.L3985:
	mov	r3, #0
	ldr	r0, [r4, #1136]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L3982
.L4009:
	ldr	r5, .L4010+8
	mov	r0, #1
	ldr	r1, .L4010+68
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3984
.L4001:
	sub	r2, fp, #44
	mvn	r6, #0
	mov	r3, #4
	mov	r1, #100
	str	r6, [r2, #-4]!
	ldr	r0, [r4, #1136]
	blx	ip
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L3985
	b	.L3984
.L4007:
	ldr	r5, .L4010+8
	mov	r0, #1
	ldr	r1, .L4010+72
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4001
	b	.L3984
.L4006:
	ldr	r1, .L4010+76
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4010+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3988
	b	.L3989
.L3888:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	bne	.L3890
	b	.L3989
.L3854:
	ldrb	r3, [r1, #3196]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r3, #5
	streqb	r3, [r0, #2]
	ldrb	ip, [r1, #3201]	@ zero_extendqisi2
	mov	r0, #22
	ldreqb	r2, [r1, #3200]	@ zero_extendqisi2
	ldrb	r3, [r1, #3199]	@ zero_extendqisi2
	ldr	r1, .L4010+80
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r2, r7
	ldr	r1, .L4010+84
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r2, [r6, #368]
	ldr	r3, [r2, #64]
	add	r3, r3, #24
	str	r3, [r2, #64]
	ldr	r3, [r6, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	b	.L3850
.L4008:
	ldr	r5, .L4010+8
	add	r3, r6, #12288
	strb	r0, [r3, #1272]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3884
	mov	r3, #0
	mov	r1, #117
	mov	r2, r3
	ldr	r0, [r4, #1136]
	blx	ip
.L3884:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, r7
	beq	.L3991
	b	.L3998
.L3873:
	ldrb	r3, [r5, #216]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3989
	ldr	r5, .L4010+8
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3875
	mov	r3, r0
	mov	r2, r0
	mov	r1, #112
	ldr	r0, [r4, #1136]
	blx	ip
.L3875:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, #0
	bne	.L3998
	add	r6, r6, #12288
	mov	r3, #1
	strb	r3, [r6, #1273]
	b	.L3982
.L4011:
	.align	2
.L4010:
	.word	.LC407
	.word	.LC425
	.word	g_event_report
	.word	.LC412
	.word	.LC417
	.word	.LC415
	.word	.LC422
	.word	.LC421
	.word	.LC423
	.word	.LC424
	.word	.LC419
	.word	.LC420
	.word	.LC414
	.word	.LC426
	.word	.LC409
	.word	.LC408
	.word	.LC413
	.word	.LC176
	.word	.LC416
	.word	.LC418
	.word	.LC410
	.word	.LC411
	UNWIND(.fnend)
	.size	H264_DecOneNal, .-H264_DecOneNal
	.align	2
	.global	H264DEC_DecodePacket
	.type	H264DEC_DecodePacket, %function
H264DEC_DecodePacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #40)
	sub	sp, sp, #40
	cmp	r1, #0
	mov	ip, #0
	mov	r4, r0
	str	ip, [fp, #-48]
	str	ip, [fp, #-52]
	str	ip, [fp, #-56]
	beq	.L4079
	ldr	r2, [r0]
	add	r5, r0, #61440
	add	r6, r0, #57344
	ldr	r3, [r2, #8]
	mov	r3, r3, asl #16
	and	r3, r3, #458752
	str	r3, [r5, #1124]
	ldr	r3, [r2]
	str	r3, [r5, #1128]
	ldr	r3, [r2, #4]
	str	ip, [r5, #1076]
	str	r3, [r5, #1132]
	ldr	r3, [r6, #3832]
	cmp	r3, #1
	beq	.L4015
	ldr	r3, [r2, #832]
	cmp	r3, #1
	beq	.L4080
.L4016:
	mov	r0, r4
	bl	ReceivePacket
	ldr	r3, [r4, #368]
	str	r3, [r5, #1172]
.L4018:
	cmp	r0, #0
	mov	r3, #0
	str	r3, [r6, #3832]
	beq	.L4019
.L4038:
	mov	r0, #0
.L4076:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4080:
	ldr	r3, [r2, #872]
	cmp	r3, #1
	bne	.L4016
	ldr	r3, [r5, #1168]
	cmp	r3, #1
	bne	.L4016
	ldr	r3, [r5, #1172]
	str	r3, [r0, #368]
	str	ip, [r5, #1168]
	str	ip, [r6, #3832]
	b	.L4017
.L4019:
	ldr	r3, [r4, #368]
.L4017:
	mov	r2, #0
	add	r7, r4, #13504
	str	r2, [r3, #64]
	add	r7, r7, #24
	ldr	r3, [r4, #368]
	mov	r0, r7
	ldr	r2, [r3, #12]
	ldr	r1, [r3, #8]
	bl	BsInit
	mov	r1, #32
	mov	r0, r7
	bl	BsGet
	ldr	r1, [r4, #368]
	uxtb	r3, r0
	and	r2, r3, #31
	and	r0, r3, #27
	strb	r2, [r1, #2]
	sub	ip, r2, #20
	ldr	r1, [r4, #368]
	clz	ip, ip
	cmp	r2, #20
	cmpne	r0, #1
	ubfx	r3, r3, #5, #2
	mov	ip, ip, lsr #5
	str	r3, [r1, #76]
	beq	.L4081
.L4020:
	mov	r0, r4
	bl	FindTrailZeros
	subs	r5, r0, #0
	bne	.L4082
	ldr	r3, [r4, #368]
	mov	r0, r7
	str	r5, [r3, #64]
	ldr	r3, [r4, #368]
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #12]
	bl	BsInit
	mov	r1, r5
	mov	r0, r4
	bl	H264_DecOneNal
	cmn	r0, #2
	beq	.L4076
	cmp	r0, #0
	beq	.L4038
	ldr	r1, .L4088
	mov	r0, #22
	bl	dprint_vfmw
.L4078:
	mvn	r0, #0
	b	.L4076
.L4015:
	ldr	r0, [r0, #368]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	b	.L4018
.L4081:
	cmp	ip, #0
	bne	.L4083
.L4021:
	sub	r2, fp, #40
	sub	r1, fp, #44
	mov	r0, r4
	bl	InquireSliceProperty
	cmp	r0, #0
	beq	.L4084
	ldr	r1, [r4, #368]
	cmp	r1, #0
	beq	.L4036
	ldr	r0, [r5, #1136]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L4036:
	ldr	r3, .L4088+4
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4078
	mov	r3, #0
	ldr	r0, [r5, #1136]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4076
.L4084:
	ldr	r2, [fp, #-44]
	ldr	r1, [r5, #1148]
	sub	r3, r2, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #0
	add	r3, r3, r1
	str	r3, [r5, #1148]
	bne	.L4085
	add	r8, r4, #12288
	ldrb	r3, [r8, #1273]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L4039
.L4026:
	ldr	r3, [fp, #-40]
	cmp	r3, #1
	bne	.L4020
	mov	r0, r4
	bl	FlushDPB
	subs	r3, r0, #0
	bne	.L4086
.L4032:
	ldr	r0, [r5, #1136]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L4020
	sub	r3, fp, #48
	ldr	r0, [r5, #1136]
	sub	r2, fp, #52
	sub	r1, fp, #56
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r4]
	ldr	r0, [r3, #708]
	cmp	r0, #1
	bne	.L4020
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ble	.L4087
.L4033:
	mov	r2, #0
	mov	r3, #1
	str	r2, [r5, #1176]
	mvn	r0, #1
	str	r3, [r6, #3832]
	b	.L4076
.L4085:
	add	r9, r4, #20480
	add	r8, r4, #12288
	ldrb	r3, [r9, #209]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r5, #1152]
	addeq	r3, r3, #1
	addne	r3, r3, #2
	str	r3, [r5, #1152]
	ldrb	r3, [r8, #1273]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L4039
	ldr	r3, [r5, #1060]
	cmp	r3, #0
	beq	.L4026
	ldr	r3, [r4, #368]
	mov	ip, #0
	mov	r0, r7
	mov	r2, #12
	ldr	r1, .L4088+8
	str	ip, [r3, #64]
	bl	BsInit
	mov	r0, r4
	mov	r1, #1
	bl	H264_DecOneNal
	ldrb	r3, [r9, #209]	@ zero_extendqisi2
	mvn	r0, #1
	cmp	r3, #1
	ldr	r3, [r5, #1152]
	subeq	r3, r3, #1
	subne	r3, r3, #2
	str	r3, [r5, #1152]
	mov	r3, #1
	str	r3, [r6, #3832]
	b	.L4076
.L4039:
	mov	r3, #0
	strb	r3, [r8, #1273]
	b	.L4026
.L4083:
	mov	r0, r4
	bl	InquireSliceExtProperty
	b	.L4021
.L4082:
	ldr	r1, .L4088+12
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4076
.L4086:
	movw	r2, #13812
	ldr	r1, .L4088+16
	mov	r0, #0
	bl	dprint_vfmw
	b	.L4032
.L4079:
	mov	r0, r1
	movw	r3, #13676
	ldr	r2, .L4088+20
	ldr	r1, .L4088+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4076
.L4087:
	ldr	r3, [fp, #-52]
	cmp	r3, #32
	bgt	.L4033
	ldr	r9, [r4, #360]
	cmp	r9, #0
	bne	.L4033
	ldr	r3, [r5, #1100]
	cmp	r3, #0
	beq	.L4020
	ldr	r2, [r8, #1276]
	cmp	r2, #0
	beq	.L4020
	ldr	lr, [r8, #1280]
	ldr	ip, [r5, #1180]
	cmp	lr, ip
	beq	.L4020
	ldr	r1, [r5, #1184]
	cmp	r2, r1
	beq	.L4020
	stmia	sp, {r1, ip}
	str	r2, [sp, #8]
	str	lr, [sp, #12]
	ldr	r2, [fp, #-40]
	ldr	r1, .L4088+28
	bl	dprint_vfmw
	ldr	r2, .L4088+4
	str	r9, [r5, #1176]
	ldr	r3, [r8, #1280]
	ldr	ip, [r2]
	str	r3, [r5, #1180]
	cmp	ip, #0
	ldr	r3, [r8, #1276]
	str	r3, [r5, #1184]
	beq	.L4020
	mov	r3, #0
	ldr	r0, [r5, #1136]
	mov	r2, r3
	mov	r1, #120
	blx	ip
	b	.L4020
.L4089:
	.align	2
.L4088:
	.word	.LC429
	.word	g_event_report
	.word	.LANCHOR0+432
	.word	.LC428
	.word	.LC354
	.word	.LC20
	.word	.LC21
	.word	.LC427
	UNWIND(.fnend)
	.size	H264DEC_DecodePacket, .-H264DEC_DecodePacket
	.align	2
	.global	H264DEC_VDMPostProc
	.type	H264DEC_VDMPostProc, %function
H264DEC_VDMPostProc:
	UNWIND(.fnstart)
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, [r0, #364]
	mov	r4, r0
	mov	r7, r3
	ldr	r2, [fp, #8]
	cmp	r5, #0
	bne	.L4091
	ldr	r3, [fp, #4]
	add	r6, r0, #57344
	cmp	r3, #0
	str	r1, [r6, #2420]
	beq	.L4092
	ldr	r3, .L4111
	mov	ip, r2, asl #6
	sub	r2, ip, r2, asl #3
	add	r2, r2, r3
	ldr	r3, [r2, #4]
	cmp	r3, #3
	beq	.L4108
	cmp	r3, #1
	beq	.L4092
.L4095:
	mov	r1, r7
	mov	r0, r4
	bl	StorePicInDPB
	cmp	r0, #1
	mov	r7, r0
	bls	.L4096
	ldr	r3, [r6, #2376]
	add	r1, r4, #61440
	mov	r2, #1
	mvn	r5, #0
	ldr	r0, [r1, #1136]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	mov	r2, r7
	ldr	r1, .L4111+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	mov	r3, #0
	str	r3, [r6, #2376]
.L4094:
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4091:
	add	r6, r0, #57344
	str	r1, [r6, #2420]
	bl	ClearAllSlice
	mov	r1, r7
	mov	r0, r4
	bl	StorePicInDPB
	subs	r2, r0, #0
	bne	.L4109
.L4097:
	mov	r0, r4
	bl	FlushDPB
	subs	r2, r0, #0
	bne	.L4110
.L4098:
	add	r2, r4, #12288
	mov	r3, #0
	str	r3, [r6, #3828]
	add	r4, r4, #20480
	ldr	r1, [r2, #1292]
	mov	r5, r3
	movw	r2, #23352
	strb	r3, [r4, #1856]
	str	r3, [r4, #1888]
	movt	r2, 1
	str	r1, [r4, #1864]
	mov	r0, #7
	mov	r1, #2
	str	r3, [r4, #1892]
	str	r2, [r4, #1872]
	mov	r3, #32
	strb	r1, [r4, #1857]
	mvn	r2, #0
	mov	r1, #3
	str	r0, [r4, #1868]
	strb	r3, [r4, #1858]
	mov	r0, r5
	mov	r3, #262144
	str	r1, [r4, #1880]
	str	r2, [r4, #1884]
	str	r3, [r4, #1896]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4092:
	mov	r0, r4
	bl	ClearAllSlice
	b	.L4095
.L4096:
	mov	r3, #1
	mov	r0, r5
	str	r3, [r6, #3828]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4110:
	ldr	r1, .L4111+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4098
.L4109:
	ldr	r1, .L4111+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4097
.L4108:
	bl	ClearAllSlice
	b	.L4094
.L4112:
	.align	2
.L4111:
	.word	g_VdmDrvParam+40
	.word	.LC430
	.word	.LC432
	.word	.LC431
	UNWIND(.fnend)
	.size	H264DEC_VDMPostProc, .-H264DEC_VDMPostProc
	.align	2
	.global	H264DEC_GetImageBuffer
	.type	H264DEC_GetImageBuffer, %function
H264DEC_GetImageBuffer:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #16)
	sub	sp, sp, #16
	add	r6, r0, #57344
	mov	r5, r0
	ldr	r7, [r6, #3820]
	cmp	r7, #0
	beq	.L4129
	add	r4, r0, #27392
	add	r9, r0, #26368
	add	r4, r4, #8
	add	r9, r9, #160
	add	r8, r0, #24576
	mov	lr, #0
	mov	r1, r4
.L4121:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4116
	cmp	r1, #0
	beq	.L4117
	ldr	r0, [r8, #2148]
	cmp	r0, #0
	beq	.L4117
	ldr	r2, [r8, #1952]
	cmp	r2, r1
	beq	.L4116
	mov	r2, r9
	b	.L4118
.L4119:
	ldr	ip, [r2, #4]!
	cmp	ip, r1
	beq	.L4116
.L4118:
	add	r3, r3, #1
	cmp	r3, r0
	bne	.L4119
.L4117:
	add	r4, r5, #61440
	ldr	r0, [r4, #1136]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L4129
	cmn	r0, #1
	bne	.L4149
	add	r1, r5, #8
	ldr	r0, [r4, #1136]
	bl	FSP_ClearNotInVoQueue
	b	.L4149
.L4116:
	add	lr, lr, #1
	add	r1, r1, #792
	cmp	lr, r7
	bne	.L4121
	sub	r2, fp, #40
	sub	r1, fp, #44
	add	r0, r5, #8
	bl	GetQueueImgNum
	ldr	r1, .L4153
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, .L4153+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r6, #3820]
	cmp	r3, #0
	addne	r8, r5, #26368
	addne	r9, r5, #24576
	addne	r8, r8, #160
	movne	r7, #0
	beq	.L4151
.L4122:
	cmp	r4, #0
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	ldrb	lr, [r4, #4]	@ zero_extendqisi2
	beq	.L4133
	ldr	r0, [r9, #2148]
	cmp	r0, #0
	beq	.L4133
	ldr	r2, [r9, #1952]
	cmp	r4, r2
	beq	.L4135
	mov	r1, r8
	mov	ip, #0
	b	.L4126
.L4127:
	ldr	r2, [r1, #4]!
	cmp	r4, r2
	beq	.L4135
.L4126:
	add	ip, ip, #1
	cmp	ip, r0
	bne	.L4127
.L4133:
	mov	r2, #1
.L4125:
	str	r2, [sp, #4]
	mov	r0, #0
	mov	r2, r7
	str	lr, [sp]
	ldr	r1, .L4153+8
	add	r7, r7, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #3820]
	add	r4, r4, #792
	cmp	r3, r7
	bhi	.L4122
.L4128:
	ldr	r3, [fp, #-40]
	mov	r0, #0
	ldr	r2, [fp, #-44]
	ldr	r1, .L4153+12
	bl	dprint_vfmw
	ldr	r3, [r6, #3820]
	ldr	r0, [r9, #2148]
	ldr	r2, [fp, #-44]
	sub	r3, r3, #2
	ldr	r1, [fp, #-40]
	rsb	r3, r0, r3
	add	r2, r2, r1
	cmp	r2, r3
	blt	.L4152
.L4149:
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4129:
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4135:
	mov	r2, #0
	b	.L4125
.L4152:
	ldr	r1, .L4153+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r5
	mov	r1, #1
	bl	ClearAll
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4151:
	add	r9, r5, #24576
	b	.L4128
.L4154:
	.align	2
.L4153:
	.word	.LC433
	.word	.LC434
	.word	.LC436
	.word	.LC435
	.word	.LC437
	UNWIND(.fnend)
	.size	H264DEC_GetImageBuffer, .-H264DEC_GetImageBuffer
	.align	2
	.global	H264DEC_GetImageWidth
	.type	H264DEC_GetImageWidth, %function
H264DEC_GetImageWidth:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r0, r0, #16384
	ldr	r0, [r0, #460]
	add	r0, r0, #1
	mov	r0, r0, asl #4
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	H264DEC_GetImageWidth, .-H264DEC_GetImageWidth
	.align	2
	.global	H264DEC_GetImageHeight
	.type	H264DEC_GetImageHeight, %function
H264DEC_GetImageHeight:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #16384
	add	r0, r0, #12288
	ldr	r3, [r3, #464]
	ldrb	r0, [r0, #1326]	@ zero_extendqisi2
	add	r3, r3, #1
	rsb	r0, r0, #2
	mov	r3, r3, asl #4
	mul	r0, r0, r3
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
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
	.align	2
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
	.type	SarTable.15518, %object
	.size	SarTable.15518, 136
SarTable.15518:
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
	.type	__func__.15699, %object
	.size	__func__.15699, 12
__func__.15699:
	.ascii	"H264_DecSEI\000"
	.type	__func__.14552, %object
	.size	__func__.14552, 13
__func__.14552:
	.ascii	"DirectOutput\000"
	.space	3
	.type	__func__.15017, %object
	.size	__func__.15017, 8
__func__.15017:
	.ascii	"InitDPB\000"
	.type	__func__.14670, %object
	.size	__func__.14670, 14
__func__.14670:
	.ascii	"StorePicInDPB\000"
	.space	2
	.type	__FUNCTION__.15936, %object
	.size	__FUNCTION__.15936, 13
__FUNCTION__.15936:
	.ascii	"H264DEC_Init\000"
	.space	3
	.type	__FUNCTION__.15154, %object
	.size	__FUNCTION__.15154, 8
__FUNCTION__.15154:
	.ascii	"InitPic\000"
	.data
	.align	2
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
	.space	192
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
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"list[%d][%d]: frame_num=%d, poc=%d\012\000" )
.LC1:
	ASCII(.ascii	"L0: \000" )
	.space	3
.LC2:
	ASCII(.ascii	"\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"%d \000" )
.LC4:
	ASCII(.ascii	"%dt \000" )
	.space	3
.LC5:
	ASCII(.ascii	"%db \000" )
	.space	3
.LC6:
	ASCII(.ascii	"L1:\000" )
.LC7:
	ASCII(.ascii	"\012\012END of the bit buffer, copy the first packe" )
	ASCII(.ascii	"t!\012\012\000" )
	.space	2
.LC8:
	ASCII(.ascii	"0:phy:0x%x, 1:phy:0x%x; len0:%d len1:%d\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"0:phy:0x%x, len0:%d\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"%-50s%50d\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"frame %d, state=%d, is already output\012\000" )
	.space	1
.LC12:
	ASCII(.ascii	"plfs(fs=%p, eFoState=%d) is null, logic_fs_id = %d\012" )
	ASCII(.ascii	"\000" )
.LC13:
	ASCII(.ascii	"not used frame: (%d,%d)\012\000" )
	.space	3
.LC14:
	ASCII(.ascii	"frame skip: is_used=%d,skip=%d\012\000" )
.LC15:
	ASCII(.ascii	"topfield skip: is_used=%d,skip=%d\012\000" )
	.space	1
.LC16:
	ASCII(.ascii	"bottom field skip: is_used=%d,skip=%d\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"err(%d) > out_thr(%d)\012\000" )
	.space	1
.LC18:
	ASCII(.ascii	"exg_pts: %lld <-> %lld\012\000" )
.LC19:
	ASCII(.ascii	"stream frame packing type is %d! what can we do?\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC20:
	ASCII(.ascii	"firmware/common/syntax/h264.c\000" )
	.space	2
.LC21:
	ASCII(.ascii	"NULL pointer: %s, L%d\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"framestore %d allocate apc %d\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"find APC, but logic_fs_id %d abnormal(ref=%d,plfs=%" )
	ASCII(.ascii	"p, pdfs=%p)\012\000" )
.LC24:
	ASCII(.ascii	"pH264Ctx->DPB.fs[%d]: frame_num=%d poc=%d is_refere" )
	ASCII(.ascii	"nce=%d\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"pH264Ctx->DPB.fs_ref[%d]: frame_num=%d poc=%d is_re" )
	ASCII(.ascii	"ference=%d\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"pH264Ctx->DPB.fs_ref[%d]: frame.poc=%d \012\000" )
	.space	3
.LC27:
	ASCII(.ascii	"\012 VFMW ** release streambuff=%p   bitstream_leng" )
	ASCII(.ascii	"th=%d\012\000" )
	.space	1
.LC28:
	ASCII(.ascii	"clear curr slice.\012\000" )
	.space	1
.LC29:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC30:
	ASCII(.ascii	"ERROR: FSP_ConfigInstance fail!\012\000" )
	.space	3
.LC31:
	ASCII(.ascii	"slice_type %d invalid, clear current slice.\012\000" )
	.space	3
.LC32:
	ASCII(.ascii	"pRepairPic(%p)/found(%d) = 0, clear current slice.\012" )
	ASCII(.ascii	"\000" )
.LC33:
	ASCII(.ascii	"listXsize[%d] = %d, clear current slice.\012\000" )
	.space	2
.LC34:
	ASCII(.ascii	"for P slice size of list equal 0.\012\000" )
	.space	1
.LC35:
	ASCII(.ascii	"for B slice size of two list all equal 0.\012\000" )
	.space	1
.LC36:
	ASCII(.ascii	"init list error.\012\000" )
	.space	2
.LC37:
	ASCII(.ascii	"L0: list after init.\012\000" )
	.space	2
.LC38:
	ASCII(.ascii	"L0: list after reorder.\012\000" )
	.space	3
.LC39:
	ASCII(.ascii	"save pic yuv :  structure = %d;  idc = %d\012\000" )
	.space	1
.LC40:
	ASCII(.ascii	"structure = %d pH264Ctx->CurrPic.pic_width_in_mb = " )
	ASCII(.ascii	"%d pH264Ctx->CurrPic.pic_height_in_mb = %d\012\000" )
	.space	1
.LC41:
	ASCII(.ascii	"nal_ref_idc=%d, structure=%d, image_id=%d, pmv_idc=" )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC42:
	ASCII(.ascii	"logic frame id(=%d): get LogicFs error!\012\000" )
	.space	3
.LC43:
	ASCII(.ascii	"phy fs is null: pstDecodeFs = %p, pstDispOutFs = %p" )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC44:
	ASCII(.ascii	"line: %d pstLogicFs is null\012\000" )
	.space	3
.LC45:
	ASCII(.ascii	"SH: first_mb_in_slice\000" )
	.space	2
.LC46:
	ASCII(.ascii	"slicecheck first_mb_in_slice >= MAX_MB_NUM_IN_PIC e" )
	ASCII(.ascii	"rror.\012\000" )
	.space	2
.LC47:
	ASCII(.ascii	"SH: slice_type\000" )
	.space	1
.LC48:
	ASCII(.ascii	"slice type = %d\012\000" )
	.space	3
.LC49:
	ASCII(.ascii	"slice type = %d, err\012\000" )
	.space	2
.LC50:
	ASCII(.ascii	"slice  pps id = %d\012\000" )
.LC51:
	ASCII(.ascii	"pps with this pic_parameter_set_id = %d havn't deco" )
	ASCII(.ascii	"de\012\000" )
	.space	1
.LC52:
	ASCII(.ascii	"sps with this pic_parameter_set_id = %d havn't deco" )
	ASCII(.ascii	"de\012\000" )
	.space	1
.LC53:
	ASCII(.ascii	"tmp slice  pps id = %d\012\000" )
.LC54:
	ASCII(.ascii	"PPSSPSCheckTmpId: pps with this pic_parameter_set_i" )
	ASCII(.ascii	"d = %d havn't decode\012\000" )
	.space	3
.LC55:
	ASCII(.ascii	"PPSSPSCheckTmpId: seq_parameter_set_id out of range" )
	ASCII(.ascii	".\012\000" )
	.space	2
.LC56:
	ASCII(.ascii	"PPSSPSCheckTmpId: sps with this pic_parameter_set_i" )
	ASCII(.ascii	"d = %d havn't decode\012\000" )
	.space	3
.LC57:
	ASCII(.ascii	"new pic flag = %d\012\000" )
	.space	1
.LC58:
	ASCII(.ascii	"SH: ref_pic_list_reordering_flag_l0\000" )
.LC59:
	ASCII(.ascii	"SH: reordering_of_pic_nums_idc_l0\000" )
	.space	2
.LC60:
	ASCII(.ascii	"reorder idc l0 = %d, g_SeErrFlag=%d\012\000" )
	.space	3
.LC61:
	ASCII(.ascii	"SH: abs_diff_pic_num_minus1_l0\000" )
	.space	1
.LC62:
	ASCII(.ascii	"abs_diff_pic_num_minus1_l0 = %d, g_SeErrFlag=%d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC63:
	ASCII(.ascii	"SH: long_term_pic_idx_l0\000" )
	.space	3
.LC64:
	ASCII(.ascii	"num of idc l0 exceed\012\000" )
	.space	2
.LC65:
	ASCII(.ascii	"SH: ref_pic_list_reordering_flag_l1\000" )
.LC66:
	ASCII(.ascii	"SH: reordering_of_pic_nums_idc_l1\000" )
	.space	2
.LC67:
	ASCII(.ascii	"reorder idc l1 = %d, g_SeErrFlag=%d\012\000" )
	.space	3
.LC68:
	ASCII(.ascii	"SH: abs_diff_pic_num_minus1_l1\000" )
	.space	1
.LC69:
	ASCII(.ascii	"abs_diff_pic_num_minus1_l1 = %d, g_SeErrFlag=%d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC70:
	ASCII(.ascii	"SH: long_term_pic_idx_l1\000" )
	.space	3
.LC71:
	ASCII(.ascii	"num of idc l1 exceed\012\000" )
	.space	2
.LC72:
	ASCII(.ascii	"SH: luma_log2_weight_denom\000" )
	.space	1
.LC73:
	ASCII(.ascii	"SH: chroma_log2_weight_denom\000" )
	.space	3
.LC74:
	ASCII(.ascii	"WP log2 exceed  and err flag = %d\012\000" )
	.space	1
.LC75:
	ASCII(.ascii	"SH: luma_weight_flag_l0\000" )
.LC76:
	ASCII(.ascii	"SH: luma_weight_l0\000" )
	.space	1
.LC77:
	ASCII(.ascii	"SH: luma_offset_l0\000" )
	.space	1
.LC78:
	ASCII(.ascii	"SH: chroma_weight_flag_l0\000" )
	.space	2
.LC79:
	ASCII(.ascii	"SH: chroma_weight_l0\000" )
	.space	3
.LC80:
	ASCII(.ascii	"SH: chroma_offset_l0\000" )
	.space	3
.LC81:
	ASCII(.ascii	"SH: luma_weight_flag_l1\000" )
.LC82:
	ASCII(.ascii	"SH: luma_weight_l1\000" )
	.space	1
.LC83:
	ASCII(.ascii	"SH: luma_offset_l1\000" )
	.space	1
.LC84:
	ASCII(.ascii	"SH: chroma_weight_flag_l1\000" )
	.space	2
.LC85:
	ASCII(.ascii	"SH: chroma_weight_l1\000" )
	.space	3
.LC86:
	ASCII(.ascii	"SH: chroma_offset_l1\000" )
	.space	3
.LC87:
	ASCII(.ascii	"overflow MAX_MMCO_LEN\012\000" )
	.space	1
.LC88:
	ASCII(.ascii	"SH: memory_management_control_operation\000" )
.LC89:
	ASCII(.ascii	"SH: difference_of_pic_nums_minus1\000" )
	.space	2
.LC90:
	ASCII(.ascii	"SH: long_term_pic_num\000" )
	.space	2
.LC91:
	ASCII(.ascii	"SH: long_term_frame_idx\000" )
.LC92:
	ASCII(.ascii	"SH: max_long_term_frame_idx_plus1\000" )
	.space	2
.LC93:
	ASCII(.ascii	"mmco exceed 6\012\000" )
	.space	1
.LC94:
	ASCII(.ascii	"SH: no_output_of_prior_pics_flag\000" )
	.space	3
.LC95:
	ASCII(.ascii	"SH: long_term_reference_flag\000" )
	.space	3
.LC96:
	ASCII(.ascii	"IDR and no_output_of_prior_pics_flag = %d\012\000" )
	.space	1
.LC97:
	ASCII(.ascii	"SH: adaptive_ref_pic_marking_mode_flag\000" )
	.space	1
.LC98:
	ASCII(.ascii	"adaptive_ref_pic_marking_mode_flag = %d\012\000" )
	.space	3
.LC99:
	ASCII(.ascii	"MMCO para would save dec err\012\000" )
	.space	2
.LC100:
	ASCII(.ascii	"mulitislice diff no_out_of_prior_pics_flag, used fi" )
	ASCII(.ascii	"rst.\012\000" )
	.space	3
.LC101:
	ASCII(.ascii	"mulitislice diff long_term_reference_flag, used fir" )
	ASCII(.ascii	"st.\012\000" )
.LC102:
	ASCII(.ascii	"mulitislice diff adaptive_ref_pic_marking_mode_flag" )
	ASCII(.ascii	", used first.\012\000" )
	.space	2
.LC103:
	ASCII(.ascii	"TotalPicNum=%d, SliceNumInPic=%d, TotalNal=%d\012\000" )
	.space	1
.LC104:
	ASCII(.ascii	"SH: pic_parameter_set_id\000" )
	.space	3
.LC105:
	ASCII(.ascii	"pps id:%d in sliceheader err\012\000" )
	.space	2
.LC106:
	ASCII(.ascii	"PPS or SPS of this slice not valid\012\000" )
.LC107:
	ASCII(.ascii	"recover point but isn't I_SLICE.\012\000" )
	.space	2
.LC108:
	ASCII(.ascii	"SH: frame_num\000" )
	.space	2
.LC109:
	ASCII(.ascii	"IDR NAL but frame_num!=0.\012\000" )
	.space	1
.LC110:
	ASCII(.ascii	"SH: field_pic_flag\000" )
	.space	1
.LC111:
	ASCII(.ascii	"SH: bottom_field_flag\000" )
	.space	2
.LC112:
	ASCII(.ascii	"field first_mb_in_slice bigger than pic size\012\000" )
	.space	2
.LC113:
	ASCII(.ascii	"frame first_mb_in_slice bigger than pic size\012\000" )
	.space	2
.LC114:
	ASCII(.ascii	"SH: idr_pic_id\000" )
	.space	1
.LC115:
	ASCII(.ascii	"idr_pic_id bigger than 65535\012\000" )
	.space	2
.LC116:
	ASCII(.ascii	"SH: pic_order_cnt_lsb\000" )
	.space	2
.LC117:
	ASCII(.ascii	"SH: delta_pic_order_cnt_bottom\000" )
	.space	1
.LC118:
	ASCII(.ascii	"SH: delta_pic_order_cnt[0]\000" )
	.space	1
.LC119:
	ASCII(.ascii	"SH: delta_pic_order_cnt[1]\000" )
	.space	1
.LC120:
	ASCII(.ascii	"SH: redundant_pic_cnt\000" )
	.space	2
.LC121:
	ASCII(.ascii	"redundant pic not support.\012\000" )
.LC122:
	ASCII(.ascii	"first_mb_in_slice in currslice small than prevslice" )
	ASCII(.ascii	" in same pic\012\000" )
	.space	3
.LC123:
	ASCII(.ascii	"SH: direct_spatial_mv_pred_flag\000" )
.LC124:
	ASCII(.ascii	"SH: num_ref_idx_override_flag\000" )
	.space	2
.LC125:
	ASCII(.ascii	"SH: num_ref_idx_l0_active_minus1\000" )
	.space	3
.LC126:
	ASCII(.ascii	"SH: num_ref_idx_l1_active_minus1\000" )
	.space	3
.LC127:
	ASCII(.ascii	"frame num_ref_idx_lx_active_minus1 exceed\012\000" )
	.space	1
.LC128:
	ASCII(.ascii	"field num_ref_idx_lx_active_minus1 exceed\012\000" )
	.space	1
.LC129:
	ASCII(.ascii	"reordering para dec err\012\000" )
	.space	3
.LC130:
	ASCII(.ascii	"apply_weights_flag=%d\012\000" )
	.space	1
.LC131:
	ASCII(.ascii	"wpt dec err\012\000" )
	.space	3
.LC132:
	ASCII(.ascii	"mark para dec err\012\000" )
	.space	1
.LC133:
	ASCII(.ascii	"SH: cabac_init_idc\000" )
	.space	1
.LC134:
	ASCII(.ascii	"cabac_init_idc bigger than 3\012\000" )
	.space	2
.LC135:
	ASCII(.ascii	"SH: slice_qp_delta\000" )
	.space	1
.LC136:
	ASCII(.ascii	"SH: disable_deblocking_filter_idc\000" )
	.space	2
.LC137:
	ASCII(.ascii	"disable_deblocking_filter_idc dec err\012\000" )
	.space	1
.LC138:
	ASCII(.ascii	"SH: slice_alpha_c0_offset_div2\000" )
	.space	1
.LC139:
	ASCII(.ascii	"slice_alpha_c0_offset_div2 dec err\012\000" )
.LC140:
	ASCII(.ascii	"SH: slice_beta_offset_div2\000" )
	.space	1
.LC141:
	ASCII(.ascii	"slice_beta_offset_div2 dec err\012\000" )
.LC142:
	ASCII(.ascii	"pH264Ctx->pCurrNal->nal_bitoffset = %d; code_len_by" )
	ASCII(.ascii	"te = %d; bitstream_length = %d; pH264Ctx->pCurrNal-" )
	ASCII(.ascii	">nal_trail_zero_bit_num = %d\012\000" )
.LC143:
	ASCII(.ascii	"stream[%d]: bitsoffset = %d; valid_bitlen = %d; byt" )
	ASCII(.ascii	"espos = %x\012\000" )
	.space	1
.LC144:
	ASCII(.ascii	"nal_segment = %d is not expected value\012\000" )
.LC145:
	ASCII(.ascii	"stream[%d]: bitsoffset = %d; valid_bitlen = %d\012\000" )
.LC146:
	ASCII(.ascii	"   : delta_sl   \000" )
	.space	3
.LC147:
	ASCII(.ascii	"PPS: entropy_coding_mode_flag\000" )
	.space	2
.LC148:
	ASCII(.ascii	"PPS: pic_order_present_flag\000" )
.LC149:
	ASCII(.ascii	"PPS: num_slice_groups_minus1\000" )
	.space	3
.LC150:
	ASCII(.ascii	"Baseline stream with FMO, not support.\012\000" )
.LC151:
	ASCII(.ascii	"PPS: num_ref_idx_l0_active_minus1\000" )
	.space	2
.LC152:
	ASCII(.ascii	"num_ref_idx_l0_active_minus1 out of range.\012\000" )
.LC153:
	ASCII(.ascii	"PPS: num_ref_idx_l1_active_minus1\000" )
	.space	2
.LC154:
	ASCII(.ascii	"num_ref_idx_l1_active_minus1 out of range.\012\000" )
.LC155:
	ASCII(.ascii	"PPS: weighted prediction flag\000" )
	.space	2
.LC156:
	ASCII(.ascii	"PPS: weighted_bipred_idc\000" )
	.space	3
.LC157:
	ASCII(.ascii	"weighted_bipred_idc out of range\012\000" )
	.space	2
.LC158:
	ASCII(.ascii	"PPS: pic_init_qp_minus26\000" )
	.space	3
.LC159:
	ASCII(.ascii	"PPS: pic_init_qs_minus26\000" )
	.space	3
.LC160:
	ASCII(.ascii	"PPS: chroma_qp_index_offset\000" )
.LC161:
	ASCII(.ascii	"PPS: deblocking_filter_control_present_flag\000" )
.LC162:
	ASCII(.ascii	"PPS: constrained_intra_pred_flag\000" )
	.space	3
.LC163:
	ASCII(.ascii	"PPS: redundant_pic_cnt_present_flag\000" )
.LC164:
	ASCII(.ascii	"redundant pic not support when find redundant slice" )
	ASCII(.ascii	" later, exit\012\000" )
	.space	3
.LC165:
	ASCII(.ascii	"PPS: transform_8x8_mode_flag\000" )
	.space	3
.LC166:
	ASCII(.ascii	"PPS: pic_scaling_matrix_present_flag\000" )
	.space	3
.LC167:
	ASCII(.ascii	"PPS: second_chroma_qp_index_offset\000" )
	.space	1
.LC168:
	ASCII(.ascii	"SPS: seq_scaling_list_present_flag\000" )
	.space	1
.LC169:
	ASCII(.ascii	"PPS: pic_scaling_list_present_flag\000" )
	.space	1
.LC170:
	ASCII(.ascii	"second_chroma_qp_index_offset out of range.\012\000" )
	.space	3
.LC171:
	ASCII(.ascii	"PPS: pic_parameter_set_id\000" )
	.space	2
.LC172:
	ASCII(.ascii	"pic_parameter_set_id out of range.\012\000" )
.LC173:
	ASCII(.ascii	"PPS: seq_parameter_set_id\000" )
	.space	2
.LC174:
	ASCII(.ascii	"seq_parameter_set_id out of range.\012\000" )
.LC175:
	ASCII(.ascii	"SPS haven't decode.\012\000" )
	.space	3
.LC176:
	ASCII(.ascii	"PPS decode error.\012\000" )
	.space	1
.LC177:
	ASCII(.ascii	"VUI: aspect_ratio_info_present_flag\000" )
.LC178:
	ASCII(.ascii	"VUI: aspect_ratio_idc\000" )
	.space	2
.LC179:
	ASCII(.ascii	"VUI: sar_width\000" )
	.space	1
.LC180:
	ASCII(.ascii	"VUI: sar_height\000" )
.LC181:
	ASCII(.ascii	"VUI: overscan_info_present_flag\000" )
.LC182:
	ASCII(.ascii	"VUI: overscan_appropriate_flag\000" )
	.space	1
.LC183:
	ASCII(.ascii	"VUI: video_signal_type_present_flag\000" )
.LC184:
	ASCII(.ascii	"VUI: video_format\000" )
	.space	2
.LC185:
	ASCII(.ascii	"VUI: video_full_range_flag\000" )
	.space	1
.LC186:
	ASCII(.ascii	"VUI: color_description_present_flag\000" )
.LC187:
	ASCII(.ascii	"VUI: colour_primaries\000" )
	.space	2
.LC188:
	ASCII(.ascii	"VUI: transfer_characteristics\000" )
	.space	2
.LC189:
	ASCII(.ascii	"VUI: matrix_coefficients\000" )
	.space	3
.LC190:
	ASCII(.ascii	"VUI: chroma_loc_info_present_flag\000" )
	.space	2
.LC191:
	ASCII(.ascii	"VUI: chroma_sample_loc_type_top_field\000" )
	.space	2
.LC192:
	ASCII(.ascii	"VUI: chroma_sample_loc_type_bottom_field\000" )
	.space	3
.LC193:
	ASCII(.ascii	"VUI: timing_info_present_flag\000" )
	.space	2
.LC194:
	ASCII(.ascii	"VUI: num_units_in_tick\000" )
	.space	1
.LC195:
	ASCII(.ascii	"VUI: time_scale\000" )
.LC196:
	ASCII(.ascii	"VUI: fixed_frame_rate_flag\000" )
	.space	1
.LC197:
	ASCII(.ascii	"VUI: nal_hrd_parameters_present_flag\000" )
	.space	3
.LC198:
	ASCII(.ascii	"VUI: cpb_cnt_minus1\000" )
.LC199:
	ASCII(.ascii	"VUI: bit_rate_scale\000" )
.LC200:
	ASCII(.ascii	"VUI: cpb_size_scale\000" )
.LC201:
	ASCII(.ascii	"hrd->cpb_cnt_minus1 out of range\012\000" )
	.space	2
.LC202:
	ASCII(.ascii	"VUI: bit_rate_value_minus1\000" )
	.space	1
.LC203:
	ASCII(.ascii	"VUI: cpb_size_value_minus1\000" )
	.space	1
.LC204:
	ASCII(.ascii	"VUI: cbr_flag\000" )
	.space	2
.LC205:
	ASCII(.ascii	"VUI: initial_cpb_removal_delay_length_minus1\000" )
	.space	3
.LC206:
	ASCII(.ascii	"VUI: cpb_removal_delay_length_minus1\000" )
	.space	3
.LC207:
	ASCII(.ascii	"VUI: dpb_output_delay_length_minus1\000" )
.LC208:
	ASCII(.ascii	"VUI: time_offset_length\000" )
.LC209:
	ASCII(.ascii	"VUI: vcl_hrd_parameters_present_flag\000" )
	.space	3
.LC210:
	ASCII(.ascii	"VUI: low_delay_hrd_flag\000" )
.LC211:
	ASCII(.ascii	"VUI: pic_struct_present_flag\000" )
	.space	3
.LC212:
	ASCII(.ascii	"VUI: bitstream_restriction_flag\000" )
.LC213:
	ASCII(.ascii	"VUI: motion_vectors_over_pic_boundaries_flag\000" )
	.space	3
.LC214:
	ASCII(.ascii	"VUI: max_bytes_per_pic_denom\000" )
	.space	3
.LC215:
	ASCII(.ascii	"VUI: max_bits_per_mb_denom\000" )
	.space	1
.LC216:
	ASCII(.ascii	"VUI: log2_max_mv_length_horizontal\000" )
	.space	1
.LC217:
	ASCII(.ascii	"VUI: log2_max_mv_length_vertical\000" )
	.space	3
.LC218:
	ASCII(.ascii	"VUI: num_reorder_frames\000" )
.LC219:
	ASCII(.ascii	"VUI: max_dec_frame_buffering\000" )
	.space	3
.LC220:
	ASCII(.ascii	"dar=%d, final_dar=%d\012\000" )
	.space	2
.LC221:
	ASCII(.ascii	"SPS: chroma_format_idc\000" )
	.space	1
.LC222:
	ASCII(.ascii	"pSPS->chroma_format_idc out of range.\012\000" )
	.space	1
.LC223:
	ASCII(.ascii	"SPS: bit_depth_luma_minus8\000" )
	.space	1
.LC224:
	ASCII(.ascii	"bit_depth_luma(%d) not equal 8.\012\000" )
	.space	3
.LC225:
	ASCII(.ascii	"SPS: bit_depth_chroma_minus8\000" )
	.space	3
.LC226:
	ASCII(.ascii	"bit_depth_chroma(%d) not equal 8.\012\000" )
	.space	1
.LC227:
	ASCII(.ascii	"SPS: qpprime_y_zero_trans_bypass_flag\000" )
	.space	2
.LC228:
	ASCII(.ascii	"qpprime_y_zero_trans_bypass_flag not equal 0.\012\000" )
	.space	1
.LC229:
	ASCII(.ascii	"SPS: seq_scaling_matrix_present_flag\000" )
	.space	3
.LC230:
	ASCII(.ascii	"SPS: log2_max_frame_num_minus4\000" )
	.space	1
.LC231:
	ASCII(.ascii	"pSPS->log2_max_frame_num_minus4 out of range.\012\000" )
	.space	1
.LC232:
	ASCII(.ascii	"SPS: pic_order_cnt_type\000" )
.LC233:
	ASCII(.ascii	"pSPS->pic_order_cnt_type out of range.\012\000" )
.LC234:
	ASCII(.ascii	"SPS: log2_max_pic_order_cnt_lsb_minus4\000" )
	.space	1
.LC235:
	ASCII(.ascii	"SPS: num_ref_frames\000" )
.LC236:
	ASCII(.ascii	"pSPS->log2_max_pic_order_cnt_lsb_minus4 out of rang" )
	ASCII(.ascii	"e.\012\000" )
	.space	1
.LC237:
	ASCII(.ascii	"SPS: delta_pic_order_always_zero_flag\000" )
	.space	2
.LC238:
	ASCII(.ascii	"SPS: offset_for_non_ref_pic\000" )
.LC239:
	ASCII(.ascii	"SPS: offset_for_top_to_bottom_field\000" )
.LC240:
	ASCII(.ascii	"SPS: num_ref_frames_in_pic_order_cnt_cycle\000" )
	.space	1
.LC241:
	ASCII(.ascii	"pSPS->num_ref_frames_in_pic_order_cnt_cycle out of " )
	ASCII(.ascii	"range.\012\000" )
	.space	1
.LC242:
	ASCII(.ascii	"SPS: offset_for_ref_frame[i]\000" )
	.space	3
.LC243:
	ASCII(.ascii	"SPS: gaps_in_frame_num_value_allowed_flag\000" )
	.space	2
.LC244:
	ASCII(.ascii	"SPS: pic_width_in_mbs_minus1\000" )
	.space	3
.LC245:
	ASCII(.ascii	"pSPS->pic_width_in_mbs_minus1 out of range(=%d).\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC246:
	ASCII(.ascii	"SPS: pic_height_in_map_units_minus1\000" )
.LC247:
	ASCII(.ascii	"SPS: frame_mbs_only_flag\000" )
	.space	3
.LC248:
	ASCII(.ascii	"pSPS->pic_height_in_map_units_minus1 out of range.\012" )
	ASCII(.ascii	"\000" )
.LC249:
	ASCII(.ascii	"pic size too large.\012\000" )
	.space	3
.LC250:
	ASCII(.ascii	"level_idc %d not support.\012\000" )
	.space	1
.LC251:
	ASCII(.ascii	"dpb size according level : %d\012\000" )
	.space	1
.LC252:
	ASCII(.ascii	"pSPS->num_ref_frames(%d) > DPB size(%d), try to sel" )
	ASCII(.ascii	"ect the reasonable one.\012\000" )
.LC253:
	ASCII(.ascii	"SPS: direct_8x8_inference_flag\000" )
	.space	1
.LC254:
	ASCII(.ascii	"SPS: frame_cropping_flag\000" )
	.space	3
.LC255:
	ASCII(.ascii	"SPS: frame_cropping_rect_left_offset\000" )
	.space	3
.LC256:
	ASCII(.ascii	"SPS: frame_cropping_rect_right_offset\000" )
	.space	2
.LC257:
	ASCII(.ascii	"SPS: frame_cropping_rect_top_offset\000" )
.LC258:
	ASCII(.ascii	"SPS: frame_cropping_rect_bottom_offset\000" )
	.space	1
.LC259:
	ASCII(.ascii	"SPS: vui_parameters_present_flag\000" )
	.space	3
.LC260:
	ASCII(.ascii	"max_dec_frame_buffering(%d) > MaxDpbSize(%d)\012\000" )
	.space	2
.LC261:
	ASCII(.ascii	"SPS: mb_adaptive_frame_field_flag\000" )
	.space	2
.LC262:
	ASCII(.ascii	"SPS_SVC_EXT: inter_layer_deblocking_filter_control_" )
	ASCII(.ascii	"present_flag\000" )
.LC263:
	ASCII(.ascii	"inter_layer_present_flag:%d!\012\000" )
	.space	2
.LC264:
	ASCII(.ascii	"SPS_SVC_EXT: extended_spatial_scalability_idc\000" )
	.space	2
.LC265:
	ASCII(.ascii	"spatial scalability not support!!\012\000" )
	.space	1
.LC266:
	ASCII(.ascii	"SPS_SVC_EXT: chroma_phase_x_plus1_flag\000" )
	.space	1
.LC267:
	ASCII(.ascii	"chroma_phase_x_plus1_flag:%d!!\012\000" )
.LC268:
	ASCII(.ascii	"SPS_SVC_EXT: chroma_phase_y_plus1\000" )
	.space	2
.LC269:
	ASCII(.ascii	"chroma_phase_y_plus1:%d!\012\000" )
	.space	2
.LC270:
	ASCII(.ascii	"SPS_SVC_EXT: seq_tcoeff_level_prediction_flag\000" )
	.space	2
.LC271:
	ASCII(.ascii	"SPS_SVC_EXT: adaptive_tcoeff_level_prediction_flag\000" )
	.space	1
.LC272:
	ASCII(.ascii	"adaptive_tcoeff_level_prediction_flag:%d!\012\000" )
	.space	1
.LC273:
	ASCII(.ascii	"SPS_SVC_EXT: slice_header_restriction_flag\000" )
	.space	1
.LC274:
	ASCII(.ascii	"slice_header_restriction_flag error:%d,%x,%d\012\000" )
	.space	2
.LC275:
	ASCII(.ascii	"SPS: profile_idc\000" )
	.space	3
.LC276:
	ASCII(.ascii	"SPS: constrained_set0_flag\000" )
	.space	1
.LC277:
	ASCII(.ascii	"SPS: constrained_set1_flag\000" )
	.space	1
.LC278:
	ASCII(.ascii	"SPS: constrained_set2_flag\000" )
	.space	1
.LC279:
	ASCII(.ascii	"SPS: constrained_set3_flag\000" )
	.space	1
.LC280:
	ASCII(.ascii	"SPS: reserved_zero_4bits\000" )
	.space	3
.LC281:
	ASCII(.ascii	"SPS: level_idc\000" )
	.space	1
.LC282:
	ASCII(.ascii	"SPS: seq_parameter_set_id\000" )
	.space	2
.LC283:
	ASCII(.ascii	"Baseline stream, try to decode, exit when FMO occur" )
	ASCII(.ascii	"red.\012\000" )
	.space	3
.LC284:
	ASCII(.ascii	"EXTENDED stream, try to decode, exit when dataparti" )
	ASCII(.ascii	"tion occurred.\012\000" )
	.space	1
.LC285:
	ASCII(.ascii	"others High profile stream, try to decode, exit whe" )
	ASCII(.ascii	"n high profile not support occurred.\012\000" )
	.space	3
.LC286:
	ASCII(.ascii	"profile_idc = %5d error, try to decode as main prof" )
	ASCII(.ascii	"ile.\012\000" )
	.space	3
.LC287:
	ASCII(.ascii	"level_idc = %5d error, try to decode as level_idc %" )
	ASCII(.ascii	"d.\012\000" )
	.space	1
.LC288:
	ASCII(.ascii	"Line %d: SPS[%d] decode error.\012\000" )
.LC289:
	ASCII(.ascii	"svc ext error:\012\000" )
.LC290:
	ASCII(.ascii	"SEI: frame_packing_arrangement_id\000" )
	.space	2
.LC291:
	ASCII(.ascii	"SEI: frame_packing_arrangement_cancel_flag\000" )
	.space	1
.LC292:
	ASCII(.ascii	"SEI: frame_packing_arrangement_type\000" )
.LC293:
	ASCII(.ascii	"SEI: quincunx_sampling_flag\000" )
.LC294:
	ASCII(.ascii	"SEI: content_interpretation_type\000" )
	.space	3
.LC295:
	ASCII(.ascii	"SEI: spatial_flipping_flag\000" )
	.space	1
.LC296:
	ASCII(.ascii	"SEI: frame0_flipped_flag\000" )
	.space	3
.LC297:
	ASCII(.ascii	"SEI: field_views_flag\000" )
	.space	2
.LC298:
	ASCII(.ascii	"SEI: current_frame_is_frame0_flag\000" )
	.space	2
.LC299:
	ASCII(.ascii	"SEI: frame0_self_contained_flag\000" )
.LC300:
	ASCII(.ascii	"SEI: frame1_self_contained_flag\000" )
.LC301:
	ASCII(.ascii	"SEI: frame0_grid_position_x\000" )
.LC302:
	ASCII(.ascii	"SEI: frame0_grid_position_y\000" )
.LC303:
	ASCII(.ascii	"SEI: frame1_grid_position_x\000" )
.LC304:
	ASCII(.ascii	"SEI: frame1_grid_position_y\000" )
.LC305:
	ASCII(.ascii	"SEI: frame_packing_arrangement_reserved_byte\000" )
	.space	3
.LC306:
	ASCII(.ascii	"SEI: frame_packing_arrangement_repetition_period\000" )
	.space	3
.LC307:
	ASCII(.ascii	"SEI: frame_packing_arrangement_extension_flag\000" )
	.space	2
.LC308:
	ASCII(.ascii	"DecPicTimingSEI but SPS is invalid.\012\000" )
	.space	3
.LC309:
	ASCII(.ascii	"SEI: cpb_removal_delay\000" )
	.space	1
.LC310:
	ASCII(.ascii	"SEI: dpb_output_delay\000" )
	.space	2
.LC311:
	ASCII(.ascii	"pic_struct\000" )
	.space	1
.LC312:
	ASCII(.ascii	"SEI nal dec payload type err\012\000" )
	.space	2
.LC313:
	ASCII(.ascii	"SEI nal dec payloadSize err\012\000" )
	.space	3
.LC314:
	ASCII(.ascii	"SEI_BUFFERING_PERIOD,offset = %d,payload_size = %d " )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC315:
	ASCII(.ascii	"PassBytes err [%s][%d]\012\000" )
.LC316:
	ASCII(.ascii	"SEI_PIC_TIMING,offset = %d,payload_size = %d \012\000" )
	.space	1
.LC317:
	ASCII(.ascii	"SEI_PAN_SCAN_RECT,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC318:
	ASCII(.ascii	"SEI_FILLER_PAYLOAD,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC319:
	ASCII(.ascii	"SEI_USER_DATA_REGISTERED_ITU_T_T35, offset = %d, pa" )
	ASCII(.ascii	"yload_size = %d \012\000" )
	.space	3
.LC320:
	ASCII(.ascii	"SEI_USER_DATA_UNREGISTERED, offset = %d, payload_si" )
	ASCII(.ascii	"ze = %d \012\000" )
	.space	3
.LC321:
	ASCII(.ascii	"SEI: itu_t_t35_country_code\000" )
.LC322:
	ASCII(.ascii	"SEI: itu_t_t35_country_code_extension_byte\000" )
	.space	1
.LC323:
	ASCII(.ascii	"SEI: itu_t_t35_provider_code\000" )
	.space	3
.LC324:
	ASCII(.ascii	"len_tmp = %d cann't dec usrdata\012\000" )
	.space	3
.LC325:
	ASCII(.ascii	"cann't dec usrdata\012\000" )
.LC326:
	ASCII(.ascii	"SEI_RECOVERY_POINT,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC327:
	ASCII(.ascii	"SEI_DEC_REF_PIC_MARKING_REPETITION,offset = %d,payl" )
	ASCII(.ascii	"oad_size = %d \012\000" )
	.space	1
.LC328:
	ASCII(.ascii	"SEI_SPARE_PIC,offset = %d,payload_size = %d \012\000" )
	.space	2
.LC329:
	ASCII(.ascii	"SEI_SCENE_INFO,offset = %d,payload_size = %d \012\000" )
	.space	1
.LC330:
	ASCII(.ascii	"SEI_SUB_SEQ_INFO,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC331:
	ASCII(.ascii	"SEI_SUB_SEQ_LAYER_CHARACTERISTICS,offset = %d,paylo" )
	ASCII(.ascii	"ad_size = %d \012\000" )
	.space	2
.LC332:
	ASCII(.ascii	"SEI_SUB_SEQ_CHARACTERISTICS,offset = %d,payload_siz" )
	ASCII(.ascii	"e = %d \012\000" )
.LC333:
	ASCII(.ascii	"SEI_FULL_FRAME_FREEZE,offset = %d,payload_size = %d" )
	ASCII(.ascii	" \012\000" )
	.space	2
.LC334:
	ASCII(.ascii	"SEI_FULL_FRAME_FREEZE_RELEASE,offset = %d,payload_s" )
	ASCII(.ascii	"ize = %d \012\000" )
	.space	2
.LC335:
	ASCII(.ascii	"SEI_FULL_FRAME_SNAPSHOT,offset = %d,payload_size = " )
	ASCII(.ascii	"%d \012\000" )
.LC336:
	ASCII(.ascii	"SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START,offset = %" )
	ASCII(.ascii	"d,payload_size = %d \012\000" )
	.space	3
.LC337:
	ASCII(.ascii	"SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END,offset = %d," )
	ASCII(.ascii	"payload_size = %d \012\000" )
	.space	1
.LC338:
	ASCII(.ascii	"SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,offset = %d," )
	ASCII(.ascii	"payload_size = %d \012\000" )
	.space	1
.LC339:
	ASCII(.ascii	"SEI_FILM_GRAIN_CHARACTERISTICS,offset = %d,payload_" )
	ASCII(.ascii	"size = %d \012\000" )
	.space	1
.LC340:
	ASCII(.ascii	"SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,offset = %" )
	ASCII(.ascii	"d,payload_size = %d \012\000" )
	.space	3
.LC341:
	ASCII(.ascii	"SEI_STEREO_VIDEO_INFO,offset = %d,payload_size = %d" )
	ASCII(.ascii	" \012\000" )
	.space	2
.LC342:
	ASCII(.ascii	"too much usrdat, cann't dec SEI\012\000" )
	.space	3
.LC343:
	ASCII(.ascii	"\012the stream is error,len:%d\012\000" )
	.space	3
.LC344:
	ASCII(.ascii	"\012Slice nal or IDR nal = %d\012\000" )
.LC345:
	ASCII(.ascii	"sliceheader dec err\012\000" )
	.space	3
.LC346:
	ASCII(.ascii	"H264Dec destroy\012\000" )
	.space	3
.LC347:
	ASCII(.ascii	"H264 recycle image\012\000" )
.LC348:
	ASCII(.ascii	"pH264Ctx is %p, pFrameStore is %p\012\000" )
	.space	1
.LC349:
	ASCII(.ascii	"line; %d pstLogicFsImage is NULL!\012\000" )
	.space	1
.LC350:
	ASCII(.ascii	"InsertVO err:%d, ClearAll\012\000" )
	.space	1
.LC351:
	ASCII(.ascii	"H264.c line %d: frame para err(ret=%d), recycle ima" )
	ASCII(.ascii	"ge self\012\000" )
.LC352:
	ASCII(.ascii	"line: %d pstLogicFsImage is NULL!\012\000" )
	.space	1
.LC353:
	ASCII(.ascii	"[%s][%d] OutputFrmToVO return %d\012\000" )
	.space	2
.LC354:
	ASCII(.ascii	"line %d: flush dpb return %d\012\000" )
	.space	2
.LC355:
	ASCII(.ascii	"----------------- Imode -> %d ------------------\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC356:
	ASCII(.ascii	"I_MODE but first_mb_in_slice != 0\012\000" )
	.space	1
.LC357:
	ASCII(.ascii	"Discard this B(poc=%d) befor P, is_ref_idc=%d.\012\000" )
.LC358:
	ASCII(.ascii	"***** VDM start, TotalPicNum=%d, pH264Ctx->CurrPic." )
	ASCII(.ascii	"structure=%d.\012\000" )
	.space	2
.LC359:
	ASCII(.ascii	"DecVDM ERR\012\000" )
.LC360:
	ASCII(.ascii	"\012 receive streambuff=%p; phy_addr=0x%x; bitstrea" )
	ASCII(.ascii	"m_length=%d\012\000" )
	.space	3
.LC361:
	ASCII(.ascii	"nal_release_err\012\000" )
	.space	3
.LC362:
	ASCII(.ascii	"cann't find slot for current nal\012\000" )
	.space	2
.LC363:
	ASCII(.ascii	"receive a zero packet\012\000" )
	.space	1
.LC364:
	ASCII(.ascii	"clear all dec para\012\000" )
.LC365:
	ASCII(.ascii	"[%s %d], pos is %d out of range\012\000" )
	.space	3
.LC366:
	ASCII(.ascii	"line %d, get APC error, ret %d\012\000" )
.LC367:
	ASCII(.ascii	"InserFrmInDPB: cur pic struct = %d!\012\000" )
	.space	3
.LC368:
	ASCII(.ascii	"CurrPic.state is 'EMPTY'\012\000" )
	.space	2
.LC369:
	ASCII(.ascii	"StorePicInDPB return(%d) from L%d\012\000" )
	.space	1
.LC370:
	ASCII(.ascii	"directoutput mode\012\000" )
	.space	1
.LC371:
	ASCII(.ascii	"line: %d pImg is NULL!\012\000" )
.LC372:
	ASCII(.ascii	"line %d, REPORT_IFRAME_ERR\012\000" )
.LC373:
	ASCII(.ascii	"line %d, return %d\012\000" )
.LC374:
	ASCII(.ascii	"err(%d) > ref_thr(%d)\012\000" )
	.space	1
.LC375:
	ASCII(.ascii	"marking return %d\012\000" )
	.space	1
.LC376:
	ASCII(.ascii	"return value is unexpeced!\012\000" )
.LC377:
	ASCII(.ascii	"line %d: pH264Ctx->DPB.size = %d, ref %d, ltref %d\012" )
	ASCII(.ascii	"\000" )
.LC378:
	ASCII(.ascii	"%s: pos(%d) = pre_pos, force return.\012\000" )
	.space	2
.LC379:
	ASCII(.ascii	"FS_ALLOC_ERR, ClearAll\012\000" )
.LC380:
	ASCII(.ascii	"Can not new logic fs! ClearAll\012\000" )
.LC381:
	ASCII(.ascii	"GAP found while DPB is empty!\012\000" )
	.space	1
.LC382:
	ASCII(.ascii	"frame num gap try to fill it \012\000" )
	.space	1
.LC383:
	ASCII(.ascii	"CurrFrameNum = %d  UnusedShortTermFrameNum = %d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC384:
	ASCII(.ascii	"cann't allocate frame store when gap find\012\000" )
	.space	1
.LC385:
	ASCII(.ascii	"allocate frame store when gap find\012\000" )
.LC386:
	ASCII(.ascii	"line %d: store gap pic err, ret=%d\012\000" )
.LC387:
	ASCII(.ascii	"%s %d unknow error!!\012\000" )
	.space	2
.LC388:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC389:
	ASCII(.ascii	"%s %d frame_store is null!!\012\000" )
	.space	3
.LC390:
	ASCII(.ascii	"line %d: init dpb return %d\012\000" )
	.space	3
.LC391:
	ASCII(.ascii	"resolution error, the CAP_LEVEL_USER_DEFINE_WITH_OP" )
	ASCII(.ascii	"TION channel with s32ReRangeEn == 0 can't support s" )
	ASCII(.ascii	"uch bitstream.\012\000" )
	.space	2
.LC392:
	ASCII(.ascii	"s32MaxRefFrameNum < pH264Ctx->DPB.size, the CAP_LEV" )
	ASCII(.ascii	"EL_USER_DEFINE_WITH_OPTION channel can't support su" )
	ASCII(.ascii	"ch bitstream.\012\000" )
	.space	3
.LC393:
	ASCII(.ascii	"mem arrange err, ClearAll\012\000" )
	.space	1
.LC394:
	ASCII(.ascii	"get back frm\012\000" )
	.space	2
.LC395:
	ASCII(.ascii	"line %d: frame gap(=%d) > dpb size(=%d)\012\000" )
	.space	3
.LC396:
	ASCII(.ascii	"line %d: dec gap failed\012\000" )
	.space	3
.LC397:
	ASCII(.ascii	"line %d: alloc framestore failed\012\000" )
	.space	2
.LC398:
	ASCII(.ascii	"dec_pts: %lld\012\000" )
	.space	1
.LC399:
	ASCII(.ascii	"dec_usertag: %lld\012\000" )
	.space	1
.LC400:
	ASCII(.ascii	"%s %d NULL == pH264Ctx->CurrPic.frame_store!!\012\000" )
	.space	1
.LC401:
	ASCII(.ascii	"Start Reason: SliceParaNum, MaxBytesReceived = %d, " )
	ASCII(.ascii	"%d(thr=%d)\012\000" )
	.space	1
.LC402:
	ASCII(.ascii	"Too many slice or bitstream, err!\012\000" )
	.space	1
.LC403:
	ASCII(.ascii	"Start Reason: new_pic_flag\012\000" )
.LC404:
	ASCII(.ascii	"init pic err, find next recover point or next valid" )
	ASCII(.ascii	" sps, pps, or exit\012\000" )
	.space	1
.LC405:
	ASCII(.ascii	"DecList error, ret=%d\012\000" )
	.space	1
.LC406:
	ASCII(.ascii	"dec list err.\012\000" )
	.space	1
.LC407:
	ASCII(.ascii	"pH264Ctx->TotalNal = %d, type:%d\012\000" )
	.space	2
.LC408:
	ASCII(.ascii	"stop i want\012\000" )
	.space	3
.LC409:
	ASCII(.ascii	"svc_extension_flag error:priority_id:%d, pred_flag%" )
	ASCII(.ascii	"d\012\000" )
	.space	2
.LC410:
	ASCII(.ascii	"%d,%d,%d\012\000" )
	.space	2
.LC411:
	ASCII(.ascii	"svc stream,nal_header:%x\012\000" )
	.space	2
.LC412:
	ASCII(.ascii	"***** NAL: IDR/Slice, nal_unit_type=%d, TotalSlice=" )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC413:
	ASCII(.ascii	"SliceCheck failed, clear current slice.\012\000" )
	.space	3
.LC414:
	ASCII(.ascii	"***** NAL: PPS, nal_unit_type=%d, TotalPPS=%d\012\000" )
	.space	1
.LC415:
	ASCII(.ascii	"***** NAL: SPS, nal_unit_type=%d, TotalSPS=%d\012\000" )
	.space	1
.LC416:
	ASCII(.ascii	"SPS decode error.\012\000" )
	.space	1
.LC417:
	ASCII(.ascii	"***** NAL: SEI, nal_unit_type=%d\012\000" )
	.space	2
.LC418:
	ASCII(.ascii	"SEI decode error.\012\000" )
	.space	1
.LC419:
	ASCII(.ascii	"***** NAL: AUD, nal_unit_type=%d\012\000" )
	.space	2
.LC420:
	ASCII(.ascii	"***** NAL: EOSEQ, nal_unit_type=%d\012\000" )
.LC421:
	ASCII(.ascii	"***** NAL: FILL, nal_unit_type=%d\012\000" )
	.space	1
.LC422:
	ASCII(.ascii	"***** NAL: EOSTREAM, nal_unit_type=%d\012\000" )
	.space	1
.LC423:
	ASCII(.ascii	"***** NAL: SPSEXT, nal_unit_type=%d\012\000" )
	.space	3
.LC424:
	ASCII(.ascii	"***** NAL: AUX, nal_unit_type=%d\012\000" )
	.space	2
.LC425:
	ASCII(.ascii	"***** NAL: EOPIC, nal_unit_type=%d\012\000" )
.LC426:
	ASCII(.ascii	"***** NAL: UNSUPPORT, nal_unit_type=%d,nalu header:" )
	ASCII(.ascii	"%x\012\000" )
	.space	1
.LC427:
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC428:
	ASCII(.ascii	"FindTrailZeros ERR\012\000" )
.LC429:
	ASCII(.ascii	"H264_DecOneNal ERR\012\000" )
.LC430:
	ASCII(.ascii	"store pic err, ret = %d\012\000" )
	.space	3
.LC431:
	ASCII(.ascii	"imode nal store pic err, ret = %d\012\000" )
	.space	1
.LC432:
	ASCII(.ascii	"imode nal flush dpb err, ret = %d\012\000" )
	.space	1
.LC433:
	ASCII(.ascii	"cann't find FrameStore\012\000" )
.LC434:
	ASCII(.ascii	"========== H264 FrameStore state(is_used, is_in_dpb" )
	ASCII(.ascii	", IsOutDPB) ========\012\000" )
	.space	3
.LC435:
	ASCII(.ascii	"ReadImgNum = %d, NewImgNum = %d\012\000" )
	.space	3
.LC436:
	ASCII(.ascii	"%02d: %d %d %d\012\000" )
.LC437:
	ASCII(.ascii	"FrameStore leak, ClearAll\012\000" )
	.bss
	.align	2
.LANCHOR2 = . + 0
.LANCHOR3 = . + 8184
	.type	pps_tmp.15438, %object
	.size	pps_tmp.15438, 2208
pps_tmp.15438:
	.space	2208
	.type	sps_tmp.15587, %object
	.size	sps_tmp.15587, 4000
sps_tmp.15587:
	.space	4000
	.type	TmpBuf, %object
	.size	TmpBuf, 68
TmpBuf:
	.space	68
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
