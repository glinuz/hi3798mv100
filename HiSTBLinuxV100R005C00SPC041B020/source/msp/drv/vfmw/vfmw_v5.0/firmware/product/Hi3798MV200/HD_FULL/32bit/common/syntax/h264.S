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
	ldr	lr, [ip, #1192]
	ldr	r2, [r0, #2144]
	ldr	r3, [ip, #1188]
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
	ldrhi	r3, [ip, #1192]
	ldrls	r3, [ip, #1192]
	addhi	r3, r3, #4
	addls	r3, r3, #2
	str	r3, [ip, #1192]
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
	ldr	r0, [r3, #1144]
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
	str	r2, [r3, #1084]
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
	str	r2, [r3, #1084]
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
	ldr	r0, [r0, #1144]
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
	ldr	r0, [lr, #1144]
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
	ldr	r0, [r8, #1144]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L369
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L370
	ldr	r3, [r5, #1952]
	mov	r2, #0
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
.L370:
	ldr	r3, [r5, #1952]
	mov	r2, #0
	ldr	r0, [r8, #1144]
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
	ldr	r0, [r7, #1144]
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
	ldr	r0, [r7, #1144]
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
	ldr	r1, [r2, #1072]
	cmp	r1, #0
	beq	.L435
.L409:
	mov	r1, #1
	str	r1, [r3, #3836]
.L408:
	ldr	r0, [r2, #1144]
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
	ldr	r0, [r5, #1144]
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
	add	ip, r0, #12288
	add	r3, r0, #20480
	mov	r7, #2208
	ldr	lr, [r1, #116]
	ldr	r2, [ip, #1304]
	mov	r4, r1
	ldr	r3, [r3, #220]
	mov	r6, #4000
	ldr	r5, [ip, #1300]
	add	ip, r0, #57344
	mla	r2, r7, r3, r2
	bic	r3, lr, #768
	bic	r3, r3, #3
	ldr	r1, [r2, #28]
	str	r3, [r4, #116]
	ldr	r2, [ip, #3844]
	mla	r1, r6, r1, r5
	mov	r2, r2, asl #10
	str	r2, [r4, #144]
	ldrb	r2, [r1, #18]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L466
	ldr	r2, [ip, #2424]
	mov	r2, r2, asl #4
	cmp	r2, #1920
	bhi	.L467
.L466:
	orr	r3, r3, #131072
	str	r3, [r4, #116]
.L468:
	ldr	r2, [r0]
	ldr	r2, [r2, #592]
	cmp	r2, #1
	movne	ip, #0
	beq	.L518
.L469:
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	orr	r3, ip, r3
	str	r3, [r4, #116]
	cmp	r2, #3
	ldrls	pc, [pc, r2, asl #2]
	b	.L508
.L472:
	.word	.L471
	.word	.L473
	.word	.L474
	.word	.L471
.L474:
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
.L470:
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
	ble	.L491
	ldrb	r3, [r4, #676]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L491
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #3
	ldreq	r3, [r4, #116]
	biceq	r3, r3, #768
	streq	r3, [r4, #116]
.L491:
	mov	r1, r4
	bl	ExchangePts
	ldrb	r3, [r6, #1816]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L492
	ldr	r3, [r6, #1832]
	cmp	r3, #3
	moveq	r3, #1
	streq	r3, [r4, #364]
	beq	.L492
	cmp	r3, #4
	moveq	r3, #2
	streq	r3, [r4, #364]
	beq	.L492
	cmp	r3, #5
	moveq	r3, #3
	streq	r3, [r4, #364]
	beq	.L492
	mov	r0, #0
	ldr	r1, .L522
	str	r0, [r4, #364]
	ldr	r2, [r6, #1832]
	bl	dprint_vfmw
.L492:
	ldr	r0, [r5, #1144]
	ldr	r1, [r4, #632]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L522+4
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
.L473:
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
	b	.L470
.L471:
	ldrb	r2, [r4, #676]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L475
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
.L477:
	ldr	r6, [r4, #724]
	ldr	ip, [r4, #760]
	cmp	r6, ip
	beq	.L519
	movle	ip, #1
	movgt	ip, #0
.L488:
	add	r6, r0, #24576
	ldrb	r10, [r6, #1852]	@ zero_extendqisi2
	cmp	r10, #3
	moveq	ip, #4096
	beq	.L489
	cmp	r10, #4
	movne	ip, ip, asl #12
	moveq	ip, #0
.L489:
	orr	r8, r9, r8
	bic	r3, r3, #12288
	orr	r3, r8, r3
	and	r2, r2, #3
	orr	r2, r3, r2
	str	r7, [r4, #108]
	orr	r2, r2, ip
	str	lr, [r4, #112]
	str	r2, [r4, #116]
	b	.L470
.L467:
	movw	r3, #64764
	movt	r3, 65533
	and	r3, r3, lr
	str	r3, [r4, #116]
	b	.L468
.L518:
	tst	r3, #131072
	movne	ip, #0
	moveq	ip, #262144
	b	.L469
.L475:
	add	r5, r0, #61440
	ldr	r7, [r4, #728]
	ldr	ip, [r4, #764]
	ldr	r2, [r5, #1140]
	cmp	r7, r2
	bhi	.L520
	cmp	r2, ip
	bcc	.L481
.L479:
	ldr	ip, [r4, #664]
	mov	r8, #3
	ldr	r2, [r4, #660]
	ldr	r6, [r4, #732]
	str	ip, [r4, #168]
	ldr	ip, [r4, #768]
.L482:
	ldrb	lr, [r4, #3]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L521
	cmp	lr, #2
	streq	r7, [r4, #168]
	movne	r9, r8, asl #10
	moveq	r2, r6
	moveq	r9, #1024
.L484:
	mov	r7, r6
	mov	lr, ip
	mov	r8, #768
	str	r6, [r4, #108]
	str	ip, [r4, #112]
	b	.L477
.L481:
	ldr	r2, [r4, #732]
	mov	r8, #1
	ldr	ip, [r4, #768]
	str	r7, [r4, #168]
	mov	r6, r2
	b	.L482
.L519:
	ldr	ip, [r4, #120]
	cmp	ip, #720
	movne	ip, #1
	bne	.L488
	ldr	ip, [r4, #124]
	subs	ip, ip, #480
	movne	ip, #1
	b	.L488
.L520:
	cmp	r2, ip
	bcc	.L479
	ldr	r2, [r4, #768]
	mov	r8, #2
	str	ip, [r4, #168]
	ldr	r6, [r4, #732]
	mov	ip, r2
	b	.L482
.L521:
	ldr	lr, [r4, #764]
	mov	r2, ip
	mov	r9, #2048
	str	lr, [r4, #168]
	b	.L484
.L508:
	add	r5, r0, #61440
	add	r6, r0, #24576
	b	.L470
.L523:
	.align	2
.L522:
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
	beq	.L528
	ldrb	r2, [r0, #713]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L529
.L528:
	ldr	r2, [r0, #748]
	mov	r1, #0
	strb	r1, [r0, #677]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, r1
	beq	.L533
	ldr	r2, [r0, #712]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	movne	ip, #1
	bne	.L531
.L533:
	mov	ip, #0
.L531:
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
.L529:
	mov	r1, #1
	strb	r1, [r0, #677]
	b	.L533
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
	beq	.L564
	cmp	r2, #1
	beq	.L545
	cmp	r2, #2
	beq	.L546
	ldr	r8, [r7, #724]
	ldr	r9, [r7, #760]
.L547:
	add	r6, r5, #24576
	ldr	ip, [r6, #2420]
	cmp	ip, #0
	beq	.L556
	ldr	r4, [r6, #2164]
	cmp	r4, #0
	addne	r3, r5, #26624
	movne	r4, #0
	addne	r3, r3, #116
	bne	.L550
	b	.L548
.L554:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L548
.L550:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L554
.L556:
	mvn	r0, #0
.L560:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L546:
	ldr	r8, [r7, #760]
	mov	r9, r8
	b	.L547
.L548:
	add	r3, r5, #61440
	ldr	r1, [r7, #632]
	ldr	r0, [r3, #1144]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L551
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L551
	add	r5, r5, r4, lsl #2
	mov	r3, #1
	add	r5, r5, #24576
	ldr	r1, .L565
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
.L545:
	ldr	r8, [r7, #724]
	mov	r9, r8
	b	.L547
.L551:
	mov	r1, #0
	ldr	r2, [r7, #632]
	ldrb	r3, [r7, #2]	@ zero_extendqisi2
	stmia	sp, {r0, r1}
	mov	r0, r1
	ldr	r1, .L565+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L560
.L564:
	mov	r0, r7
	movw	r3, #2220
	ldr	r2, .L565+8
	ldr	r1, .L565+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L560
.L566:
	.align	2
.L565:
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
	ldr	r3, .L579
	mov	r5, r0
	ldr	r3, [r3]
	tst	r3, #16384
	beq	.L569
	add	r6, r0, #24576
	ldr	r3, [r6, #2148]
	cmp	r3, #0
	addne	r7, r0, #26368
	movne	r4, #0
	addne	r7, r7, #156
	beq	.L575
.L574:
	ldr	ip, [r7, #4]!
	mov	r2, r4
	ldr	r1, .L579+4
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
	bhi	.L574
.L575:
	ldr	r1, .L579+8
	mov	r0, #14
	bl	dprint_vfmw
	ldr	r3, [r6, #2152]
	cmp	r3, #0
	addne	r5, r5, #26368
	movne	r4, #0
	addne	r5, r5, #220
	beq	.L573
.L576:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L579+12
	mov	r0, #14
	ldrb	lr, [ip, #2]	@ zero_extendqisi2
	ldr	r3, [ip, #636]
	str	lr, [sp, #4]
	ldr	ip, [ip, #648]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r5]
	mov	r2, r4
	ldr	r1, .L579+16
	mov	r0, #14
	add	r4, r4, #1
	ldr	r3, [r3, #688]
	bl	dprint_vfmw
	ldr	r3, [r6, #2152]
	cmp	r3, r4
	bhi	.L576
.L573:
	ldr	r1, .L579+8
	mov	r0, #14
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L569:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L580:
	.align	2
.L579:
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
	bne	.L582
	ldrb	r2, [r3, #1741]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
.L582:
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
	beq	.L585
	bcc	.L586
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
	beq	.L603
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
	beq	.L594
	add	r2, r4, #608
	ldrd	r0, [r2]
	cmp	r1, r7
	cmpeq	r0, r6
	beq	.L604
.L594:
	ldr	r2, [r4, #732]
.L601:
	cmp	r2, ip
	mov	r0, r4
	movcc	r2, ip
	str	r2, [r4, #660]
	ldr	r3, [r3, #2396]
	str	r3, [r4, #648]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	CombineFldsToFrm
.L586:
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
.L585:
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
	beq	.L605
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
	beq	.L590
	add	r2, r4, #608
	ldrd	r0, [r2]
	cmp	r1, r7
	cmpeq	r0, r6
	streqd	r8, [r2]
.L590:
	ldr	r2, [r4, #768]
	b	.L601
.L603:
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
.L605:
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
.L604:
	strd	r8, [r2]
	b	.L594
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
	bcs	.L612
.L607:
	add	r0, r4, #57344
	mov	r2, #0
	ldr	r3, [r0, #2392]
	strb	r2, [r0, #1740]
	cmp	r3, r2
	movne	r3, #1
	strneb	r3, [r0, #1741]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L612:
	ldr	r1, [r3, #1952]
	bl	UnMarkFrameStoreRef
	mov	r1, #0
	mov	r0, r4
	bl	RemoveFrameStoreOutDPB
	b	.L607
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
	beq	.L621
	add	r7, r5, #56
	mov	r4, r5
	mov	r3, #0
	strb	r3, [r5, #1]
	strb	r3, [r5, #4]
.L616:
	ldr	r3, [r4, #8]
	mov	r0, r6
	cmp	r3, #0
	beq	.L615
	ldr	r1, [r4, #32]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #12]
	ldr	r2, [r4, #8]
	mov	r0, #7
	ldr	r1, .L622
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r4, #8]
	str	r3, [r4, #24]
	str	r3, [r4, #12]
.L615:
	add	r4, r4, #28
	cmp	r4, r7
	bne	.L616
	mov	r3, #0
	strb	r3, [r5]
	str	r3, [r5, #68]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L621:
	mov	r0, r5
	movw	r3, #4204
	ldr	r2, .L622+4
	ldr	r1, .L622+8
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L623:
	.align	2
.L622:
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
	beq	.L625
	add	r3, r0, #61440
	ldr	r0, [r3, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L625:
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
	ldr	r1, .L634
	mov	r5, r0
	mov	r0, #1
	add	r4, r5, #61440
	bl	dprint_vfmw
	ldr	r1, [r5, #368]
	cmp	r1, #0
	beq	.L630
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r5, #368]
.L630:
	ldr	r3, [r4, #1100]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #1100]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L635:
	.align	2
.L634:
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
	b	.L638
.L637:
	add	r4, r4, #96
	cmp	r4, r5
	beq	.L640
.L638:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L637
	mov	r1, r4
	ldr	r0, [r6, #1144]
	add	r4, r4, #96
	bl	ReleaseNAL
	cmp	r4, r5
	bne	.L638
.L640:
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
.L643:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L642
	ldr	r0, [r6, #1144]
	bl	ReleaseNAL
	str	r7, [r4]
.L642:
	cmp	r4, r5
	bne	.L643
	mov	r0, #0
	str	r0, [r6, #1068]
	str	r0, [r6, #1128]
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
	ldr	r3, .L659
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
	beq	.L650
	add	r2, r4, #24576
	ldrb	r3, [fp, #4]	@ zero_extendqisi2
	mov	r1, #5
	strb	r0, [fp, #-56]
	ldr	r2, [r2, #2144]
	strb	r1, [fp, #-55]
	add	r2, r2, #1
	strb	r2, [fp, #-57]
.L656:
	add	r5, r4, #61440
	sub	r1, fp, #60
	mov	r6, #0
	strb	r3, [fp, #-54]
	ldr	r0, [r5, #1144]
	strb	r6, [fp, #-58]
	bl	FSP_ConfigInstance
	cmp	r0, r6
	bne	.L651
	ldr	r1, [r4]
	sub	r3, fp, #64
	ldr	r0, [r5, #1144]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, #2
	beq	.L658
	cmp	r0, #0
	moveq	r0, #1
	bne	.L653
.L657:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L650:
	mov	r3, #1
	mov	r2, #5
	strb	r0, [fp, #-56]
	strb	r3, [fp, #-57]
	strb	r2, [fp, #-55]
	b	.L656
.L658:
	add	r0, r4, #8
	bl	ResetVoQueue
.L653:
	ldr	r1, .L659+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L657
.L651:
	mov	r0, r6
	ldr	r1, .L659+8
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L657
.L660:
	.align	2
.L659:
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
	beq	.L663
	bcc	.L678
	cmp	r2, #2
	bne	.L687
.L679:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L687:
	ldr	r1, .L691
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L678:
	mov	r6, #1
.L664:
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
	beq	.L666
.L689:
	ldr	r10, [lr]
	cmp	r10, #0
	bne	.L680
	mov	ip, lr
	mov	r1, r10
	b	.L668
.L669:
	ldr	r9, [ip, #4]!
	cmp	r9, #0
	bne	.L667
.L668:
	add	r1, r1, #1
	cmp	r1, r3
	bne	.L669
	ldr	r1, [r4]
	ldr	r1, [r1, #12]
	cmp	r1, #2
	bne	.L688
	ldr	r5, [r7, #2376]
	add	r5, r5, #676
.L671:
	mov	r9, r10
	mov	ip, lr
	mov	r1, #0
	b	.L677
.L673:
	ldr	r9, [ip, #4]!
.L677:
	add	r1, r1, #1
	cmp	r9, #0
	streq	r5, [ip]
	cmp	r1, r3
	bne	.L673
	add	r2, r2, #1
	add	lr, lr, #132
	cmp	r6, r2
	bls	.L679
.L686:
	ldr	r3, [r0, #4]!
	cmp	r3, #0
	bne	.L689
.L666:
	ldr	r1, [r4]
	ldr	r1, [r1, #12]
	cmp	r1, #2
	bne	.L690
	add	r2, r2, #1
	str	r8, [r0]
	cmp	r6, r2
	ldr	r3, [r7, #2376]
	add	lr, lr, #132
	add	r3, r3, #676
	str	r3, [lr, #-132]
	bhi	.L686
	b	.L679
.L680:
	mov	r9, r10
.L667:
	mov	r5, r9
	b	.L671
.L663:
	mov	r6, #2
	b	.L664
.L690:
	ldr	r1, .L691+4
.L684:
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L688:
	mov	r2, r5
	mov	r3, #0
	ldr	r1, .L691+8
	b	.L684
.L692:
	.align	2
.L691:
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
	beq	.L694
	ldrb	r3, [r3, #210]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r4, #2
	moveq	r4, #1
.L694:
	add	r3, r0, #24576
	ldr	r5, [r3, #2152]
	cmp	r5, #0
	beq	.L702
	add	r3, r0, #26368
	mov	r2, #0
	add	r3, r3, #224
	b	.L699
.L712:
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L710
.L697:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L711
.L699:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L712
	ldrb	lr, [r0, #2]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L698
	ldr	ip, [r0, #712]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	beq	.L713
.L698:
	tst	lr, #2
	beq	.L697
	ldr	ip, [r0, #748]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L697
	ldr	ip, [r0, #756]
	cmp	ip, r1
	bne	.L697
	add	r0, r0, #748
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L710:
	ldr	ip, [r0, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L697
	ldr	ip, [r0, #684]
	cmp	ip, r1
	bne	.L697
	add	r0, r0, #676
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L713:
	ldr	ip, [r0, #720]
	cmp	ip, r1
	bne	.L698
	add	r0, r0, #712
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L711:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L702:
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
	beq	.L715
	ldrb	r3, [r3, #210]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r4, #2
	moveq	r4, #1
.L715:
	add	r3, r0, #24576
	ldr	r5, [r3, #2156]
	cmp	r5, #0
	beq	.L723
	add	r3, r0, #26624
	mov	r2, #0
	add	r3, r3, #32
	b	.L720
.L733:
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L731
.L718:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L732
.L720:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L733
	ldrb	lr, [r0, #2]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L719
	ldr	ip, [r0, #712]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	beq	.L734
.L719:
	tst	lr, #2
	beq	.L718
	ldr	ip, [r0, #748]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L718
	ldr	ip, [r0, #752]
	cmp	ip, r1
	bne	.L718
	add	r0, r0, #748
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L731:
	ldr	ip, [r0, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L718
	ldr	ip, [r0, #680]
	cmp	ip, r1
	bne	.L718
	add	r0, r0, #676
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L734:
	ldr	ip, [r0, #716]
	cmp	ip, r1
	bne	.L719
	add	r0, r0, #712
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L732:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L723:
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
	ble	.L736
	add	lr, r5, r2, lsl #2
	mov	ip, r2
.L737:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L737
.L736:
	add	r3, r4, #1
	str	r3, [r7]
	str	r0, [r5, r4, asl #2]
	ldr	ip, [r7]
	cmp	r2, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r5, ip, lsl #2
	mov	lr, ip
.L741:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L739
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L740
	ldr	r3, [r0, #8]
	cmp	r3, r6
	beq	.L739
.L740:
	str	r0, [r5, lr, asl #2]
	add	lr, lr, #1
.L739:
	cmp	ip, r2
	bne	.L741
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
	ble	.L749
	add	lr, r5, r2, lsl #2
	mov	ip, r2
.L750:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L750
.L749:
	add	r3, r4, #1
	str	r3, [r7]
	str	r0, [r5, r4, asl #2]
	ldr	ip, [r7]
	cmp	r2, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r5, ip, lsl #2
	mov	lr, ip
.L754:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L752
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L753
	ldr	r3, [r0, #4]
	cmp	r3, r6
	beq	.L752
.L753:
	str	r0, [r5, lr, asl #2]
	add	lr, lr, #1
.L752:
	cmp	ip, r2
	bne	.L754
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
	beq	.L765
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
.L775:
	adds	r9, r8, #2
	beq	.L764
	ldr	r3, [ip]
	cmp	r3, #3
	beq	.L764
	mov	r2, r8
	ldr	r6, [fp, #8]
	ldr	r5, [fp, #-64]
	mov	r4, #0
	mov	r8, ip
	mov	r10, r2
	b	.L768
.L769:
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
	beq	.L764
.L774:
	ldr	r3, [r8, #4]!
	add	r6, r6, #4
	cmp	r3, #3
	beq	.L764
.L768:
	cmp	r3, #1
	bhi	.L769
	cmp	r3, #0
	ldr	r3, [r6]
	bne	.L770
	mvn	r3, r3
	adds	r5, r5, r3
	ldrmi	r3, [fp, #-60]
	addmi	r5, r5, r3
.L771:
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
	bne	.L774
.L764:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L770:
	add	r3, r3, #1
	add	r5, r5, r3
	ldr	r3, [fp, #-60]
	cmp	r3, r5
	rsble	r5, r3, r5
	b	.L771
.L765:
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
	b	.L775
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
	beq	.L783
	add	r6, r0, #16384
	ldrb	r2, [r6, #3512]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L795
.L786:
	cmp	r3, #1
	ldr	r3, [r4, #264]
	add	r3, r3, #1
	str	r3, [r4, #252]
	bne	.L783
	ldrb	r3, [r6, #3513]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L796
.L788:
	ldr	r3, [r4, #268]
	add	r3, r3, #1
	str	r3, [r4, #256]
.L783:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L795:
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
	b	.L786
.L796:
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
	b	.L788
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
	ldr	lr, .L860
	movcc	r4, r2
	ldr	ip, .L860+4
	movcs	r4, #31
	cmp	r6, #0
	str	r3, [fp, #-52]
	movne	r6, lr
	moveq	r6, ip
	cmp	r0, #1
	moveq	r5, #0
	moveq	r10, r5
	beq	.L800
	cmp	r0, #2
	moveq	r5, #0
	moveq	r10, r5
	bne	.L808
.L809:
	cmp	r10, r4
	movcs	r3, #0
	movcc	r3, #1
	cmp	r5, r4
	str	r3, [fp, #-48]
	movcs	r2, #0
	movcc	r2, #1
	orrs	r3, r2, r3
	beq	.L808
	cmp	r2, #0
	beq	.L814
	add	r8, r7, r5, lsl #2
	b	.L813
.L810:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L814
.L813:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	ip, [r0, #1]	@ zero_extendqisi2
	tst	ip, #2
	beq	.L810
	add	r0, r0, #748
	blx	r6
	cmp	r0, #0
	beq	.L810
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
.L814:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r10, lsl #2
	bne	.L816
	b	.L809
.L815:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L809
.L816:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	tst	r2, #1
	beq	.L815
	add	r0, r0, #712
	blx	r6
	cmp	r0, #0
	beq	.L815
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
	b	.L809
.L859:
	add	r0, r0, #712
	blx	r6
	cmp	r0, #0
	beq	.L801
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
.L805:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r5, lsl #2
	bne	.L807
	b	.L800
.L806:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L800
.L807:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	tst	r2, #2
	beq	.L806
	add	r0, r0, #748
	blx	r6
	cmp	r0, #0
	beq	.L806
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
.L800:
	cmp	r10, r4
	movcs	r2, #0
	movcc	r2, #1
	cmp	r5, r4
	movcs	r3, #0
	movcc	r3, #1
	str	r3, [fp, #-48]
	orrs	r3, r3, r2
	beq	.L808
	cmp	r2, #0
	beq	.L805
	add	r9, r7, r10, lsl #2
	b	.L804
.L801:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L805
.L804:
	mov	r8, r9
	ldr	r0, [r9], #4
	ldrb	ip, [r0, #1]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L801
	b	.L859
.L808:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L861:
	.align	2
.L860:
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
	bne	.L863
	ldrb	r7, [r8, #1739]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L987
	add	r3, r0, #24576
	str	r3, [fp, #-240]
	ldr	ip, [r3, #2152]
	cmp	ip, #0
	beq	.L930
	add	r1, r0, #26368
	mov	r3, r7
	add	r1, r1, #220
	b	.L869
.L868:
	cmp	r3, ip
	beq	.L867
.L869:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L868
	ldr	r0, [r2, #676]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #65536
	bne	.L868
	add	r0, r7, #6720
	cmp	r3, ip
	add	r0, r0, #62
	add	r2, r2, #676
	add	r7, r7, #1
	add	r0, r4, r0, lsl #2
	str	r2, [r0, #4]
	bne	.L869
.L867:
	add	r0, r4, #26880
	ldr	r3, .L995
	mov	r1, r7
	add	r0, r0, #252
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	str	r7, [r6, #252]
	ldr	r1, [r3, #2156]
	cmp	r1, #0
	beq	.L931
	add	r0, r4, #26624
	mov	r5, r7
	add	r0, r0, #28
	mov	r3, #0
	b	.L872
.L871:
	cmp	r3, r1
	beq	.L988
.L872:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L871
	ldr	ip, [r2, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L871
	add	ip, r5, #6720
	cmp	r3, r1
	add	ip, ip, #62
	add	r2, r2, #676
	add	r5, r5, #1
	add	ip, r4, ip, lsl #2
	str	r2, [ip, #4]
	bne	.L872
.L988:
	rsb	r1, r7, r5
.L870:
	add	r0, r4, r7, lsl #2
	ldr	r3, .L995+4
	add	r0, r0, #26880
	mov	r2, #4
	add	r0, r0, #252
	bl	qsort
	mov	r2, r5
	cmp	r2, #0
	mov	r3, #0
	str	r5, [r6, #252]
	str	r3, [r6, #256]
	beq	.L989
.L878:
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
	bhi	.L922
	add	r2, r2, #6720
	add	r1, r4, #27136
	add	r2, r2, #62
	add	r1, r1, #124
	mov	r0, #0
	add	r2, r4, r2, lsl #2
.L921:
	str	r0, [r2, #4]!
	cmp	r2, r1
	bne	.L921
.L922:
	cmp	r3, #32
	bhi	.L920
	add	r3, r3, #6784
	add	r1, r4, #27392
	add	r3, r3, #31
	mov	r2, #0
	add	r4, r4, r3, lsl #2
.L923:
	str	r2, [r4, #4]!
	cmp	r4, r1
	bne	.L923
.L920:
	mov	r0, #0
.L973:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L863:
	ldrb	r5, [r8, #1739]	@ zero_extendqisi2
	add	r3, r0, #24576
	str	r3, [fp, #-240]
	cmp	r5, #0
	beq	.L884
	ldr	ip, [r3, #2152]
	cmp	ip, #0
	beq	.L990
	add	r9, r0, #26368
	mov	r5, #0
	add	r9, r9, #220
	mov	r3, r5
	mov	r1, r9
.L903:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L902
	sub	r0, fp, #44
	ldr	lr, [r8, #2396]
	add	r7, r0, r5, lsl #2
	ldr	r0, [r2, #648]
	cmp	lr, r0
	strge	r2, [r7, #-192]
	addge	r5, r5, #1
.L902:
	cmp	r3, ip
	bne	.L903
	sub	r10, fp, #236
	ldr	r3, .L995+8
	mov	r2, #4
	mov	r1, r5
	mov	r0, r10
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2152]
	cmp	r7, #0
	beq	.L984
.L928:
	mov	r3, r9
	mov	r2, #0
	mov	r9, r5
.L906:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L905
	sub	r0, fp, #44
	ldr	ip, [r8, #2396]
	add	lr, r0, r9, lsl #2
	ldr	r0, [r1, #648]
	cmp	ip, r0
	strlt	r1, [lr, #-192]
	addlt	r9, r9, #1
.L905:
	cmp	r2, r7
	bne	.L906
	rsb	r7, r5, r9
	mov	r3, r5, asl #2
	add	r0, r10, r3
	str	r3, [fp, #-244]
	mov	r1, r7
	ldr	r3, .L995+12
	mov	r2, #4
	bl	qsort
	cmp	r5, #0
	subeq	ip, fp, #172
	beq	.L907
.L925:
	sub	ip, fp, #172
	mov	r2, r10
	add	r7, ip, r7, lsl #2
	mov	r3, #0
.L908:
	add	r3, r3, #1
	ldr	r1, [r2], #4
	cmp	r3, r5
	str	r1, [r7], #4
	bcc	.L908
.L907:
	cmp	r9, r5
	bls	.L909
	ldr	r3, [fp, #-244]
	add	r2, r10, r3
	mov	r3, ip
.L910:
	add	r5, r5, #1
	ldr	r1, [r2], #4
	cmp	r5, r9
	str	r1, [r3], #4
	bne	.L910
.L909:
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
	beq	.L911
	add	r3, r4, #26624
	sub	r9, fp, #108
	add	r3, r3, #28
	mov	r2, r9
.L912:
	add	r5, r5, #1
	ldr	r1, [r3, #4]!
	cmp	r5, r7
	str	r1, [r2], #4
	bne	.L912
.L911:
	ldr	r3, .L995+16
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
.L901:
	cmp	r2, #1
	movls	r1, #0
	andhi	r1, r1, #1
	cmp	r1, #0
	beq	.L913
	cmp	r2, #0
	ldr	r1, [fp, #-240]
	beq	.L914
	ldr	r7, [r1, #2556]
	ldr	r1, [r1, #2688]
	cmp	r7, r1
	bne	.L913
	add	r0, r4, #27136
	ldr	ip, [fp, #-244]
	add	r0, r0, #128
	mov	r1, #0
	b	.L915
.L916:
	ldr	r5, [ip, #4]!
	ldr	lr, [r0, #4]!
	cmp	r5, lr
	bne	.L913
.L915:
	add	r1, r1, #1
	cmp	r1, r2
	bne	.L916
.L917:
	ldr	r0, [fp, #-240]
	ldr	r1, [r0, #2692]
	str	r7, [r0, #2692]
	str	r1, [r0, #2688]
.L913:
	orrs	r1, r2, r3
	bne	.L878
	ldr	r1, .L995+20
.L979:
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	b	.L973
.L884:
	ldr	r10, [r3, #2152]
	cmp	r10, #0
	beq	.L887
	add	r9, r0, #26368
	mov	r3, r5
	add	r9, r9, #220
	mov	r1, r9
	b	.L889
.L888:
	cmp	r3, r10
	beq	.L991
.L889:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L888
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
	bne	.L889
.L991:
	add	r3, r4, #26880
	mov	r1, r5
	mov	r2, r3
	add	r2, r2, #252
	ldr	r3, .L995+24
	mov	r0, r2
	str	r2, [fp, #-244]
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2152]
	cmp	r7, #0
	beq	.L983
.L926:
	mov	r3, r9
	mov	r10, r5
	mov	r2, #0
	b	.L892
.L891:
	cmp	r2, r7
	beq	.L992
.L892:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L891
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
	bne	.L892
.L992:
	add	r8, r5, #6720
	rsb	r7, r5, r10
	add	r8, r8, #62
	mov	r2, #4
	ldr	r3, .L995+28
	mov	r1, r7
	add	r0, r4, r8, lsl #2
	add	r9, r4, r10, lsl #2
	add	r0, r0, r2
	add	r9, r9, #26880
	bl	qsort
	cmp	r5, #0
	add	r9, r9, #252
	beq	.L894
.L924:
	add	r7, r7, #6784
	add	r2, r4, #26880
	add	r7, r7, #31
	add	r2, r2, #248
	mov	r3, #0
	add	r7, r4, r7, lsl #2
.L895:
	add	r3, r3, #1
	ldr	r1, [r2, #4]!
	cmp	r3, r5
	str	r1, [r7, #4]!
	bcc	.L895
.L894:
	cmp	r10, r5
	bls	.L896
	add	r3, r4, #27136
	add	r8, r4, r8, lsl #2
	add	r3, r3, #124
.L897:
	add	r5, r5, #1
	ldr	r2, [r8, #4]!
	cmp	r5, r10
	str	r2, [r3, #4]!
	bne	.L897
.L896:
	ldr	r3, [fp, #-240]
	str	r10, [r6, #256]
	str	r10, [r6, #252]
	ldr	r1, [r3, #2156]
	cmp	r1, #0
	beq	.L933
	add	r0, r4, #26624
	mov	r5, r10
	add	r0, r0, #28
	mov	r3, #0
	b	.L900
.L899:
	cmp	r3, r1
	beq	.L993
.L900:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #1]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L899
	ldrb	ip, [r2, #677]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L899
	add	ip, r4, r5, lsl #2
	cmp	r3, r1
	add	ip, ip, #24576
	add	r2, r2, #676
	add	r5, r5, #1
	str	r2, [ip, #2556]
	str	r2, [ip, #2688]
	bne	.L900
.L993:
	rsb	r1, r10, r5
.L898:
	mov	r0, r9
	ldr	r3, .L995+4
	mov	r2, #4
	bl	qsort
	ldr	r0, [r6, #252]
	ldr	r3, .L995+4
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
	b	.L901
.L987:
	add	r2, r0, #24576
	str	r2, [fp, #-240]
	ldr	lr, [r2, #2152]
	cmp	lr, #0
	moveq	r7, lr
	beq	.L866
	add	r1, r0, #26368
	mov	r7, r3
	add	r1, r1, #220
.L875:
	ldr	r2, [r1, #4]!
	sub	r0, fp, #44
	add	ip, r0, r7, lsl #2
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	strne	r2, [ip, #-192]
	addne	r7, r7, #1
	cmp	r3, lr
	bne	.L875
.L866:
	add	r10, r4, #26880
	mov	r1, r7
	ldr	r3, .L995+32
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
	beq	.L876
	add	r3, r4, #26624
	sub	r9, fp, #108
	add	r3, r3, #28
	mov	r2, r9
.L877:
	add	r5, r5, #1
	ldr	r1, [r3, #4]!
	cmp	r5, r7
	str	r1, [r2], #4
	bne	.L877
.L876:
	ldr	r3, .L995+16
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
	bne	.L878
.L989:
	ldr	r3, [r4]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L879
	ldrb	r3, [r8, #1739]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L994
	cmp	r3, #1
	ldr	r2, [fp, #-240]
	ldr	r3, [r8, #2376]
	addeq	r3, r3, #712
	addne	r3, r3, #748
	str	r3, [r2, #2556]
.L881:
	mov	r3, #0
	mov	r2, #1
	b	.L878
.L994:
	ldr	r2, [fp, #-240]
	ldr	r3, [r8, #2376]
	add	r3, r3, #676
	str	r3, [r2, #2556]
	b	.L881
.L914:
	ldr	r7, [r1, #2688]
	b	.L917
.L983:
	add	r8, r5, #6720
	mov	r2, #4
	add	r8, r8, #62
	ldr	r3, .L995+28
	mov	r1, r7
	add	r9, r4, r8, lsl #2
	add	r9, r9, r2
	mov	r0, r9
	bl	qsort
	subs	r10, r5, #0
	bne	.L924
	b	.L896
.L984:
	mov	r3, r5, asl #2
	mov	r1, r7
	str	r3, [fp, #-244]
	mov	r2, r3
	ldr	r3, .L995+12
	add	r0, r10, r2
	mov	r2, #4
	bl	qsort
	subs	r9, r5, #0
	bne	.L925
.L980:
	sub	ip, fp, #172
	b	.L909
.L930:
	mov	r7, ip
	b	.L867
.L990:
	sub	r10, fp, #236
	ldr	r3, .L995+8
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
	bne	.L928
	ldr	r3, .L995+12
	mov	r2, #4
	mov	r1, r7
	mov	r0, r10
	mov	r9, r7
	bl	qsort
	b	.L980
.L887:
	add	r3, r0, #26880
	mov	r1, r10
	mov	r2, r3
	add	r2, r2, #252
	ldr	r3, .L995+24
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
	bne	.L926
	ldr	r9, [fp, #-244]
	mov	r2, #4
	ldr	r3, .L995+28
	mov	r1, r7
	mov	r10, r7
	mov	r0, r9
	bl	qsort
	b	.L896
.L931:
	mov	r5, r7
	b	.L870
.L933:
	mov	r5, r10
	b	.L898
.L879:
	ldr	r1, .L995+36
	b	.L979
.L996:
	.align	2
.L995:
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
	ldr	r3, .L999
	ldr	r3, [r3]
	tst	r3, #8192
	ldmeqfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DumpList.part.5
.L1000:
	.align	2
.L999:
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
	beq	.L1006
	ldr	r3, [r3, #1952]
	add	r2, r0, #57344
	ldr	r5, [r2, #2396]
	ldr	lr, [r3, #648]
	rsb	lr, lr, r5
	cmp	lr, #0
	rsblt	lr, lr, #0
	cmp	r4, #1
	bls	.L1007
	add	r1, r0, #26368
	mov	r2, #1
	add	r1, r1, #160
	mov	r0, #0
.L1005:
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
	bne	.L1005
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1007:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1006:
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
	bne	.L1039
	ldr	lr, [r3, #252]
	mov	r4, r5
	mov	r6, r5
	mov	r7, ip
	cmp	lr, #0
	add	r5, r5, #124
	add	r4, r4, #128
	add	r6, r6, #248
	beq	.L1017
.L1041:
	mov	r2, r6
	mov	r0, #32
	mov	r3, #0
	b	.L1015
.L1014:
	add	r3, r3, #1
	cmp	r3, lr
	beq	.L1040
.L1015:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #32]
	ldr	r1, [r1, #672]
	cmp	r1, ip
	bne	.L1014
	cmp	r0, r3
	movcs	r0, r3
	add	r3, r3, #1
	cmp	r3, lr
	str	r0, [r4, #-4]
	str	r0, [r5, #4]
	bne	.L1015
.L1040:
	cmp	r0, #31
	bhi	.L1017
.L1016:
	add	ip, ip, #1
	add	r5, r5, #8
	cmp	ip, #16
	add	r4, r4, #8
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	lr, #0
	bne	.L1041
.L1017:
	str	r7, [r4, #-4]
	str	r7, [r5, #4]
	b	.L1016
.L1039:
	ldr	r4, [r3, #252]
	mov	ip, #0
	mov	r6, r5
	mov	r7, ip
	cmp	r4, #0
	add	r5, r5, #124
	add	r6, r6, #248
	beq	.L1021
.L1043:
	mov	r1, r6
	mov	lr, #32
	mov	r2, #0
	b	.L1019
.L1018:
	add	r2, r2, #1
	cmp	r2, r4
	beq	.L1042
.L1019:
	ldr	r3, [r1, #4]!
	ldr	r8, [r3, #32]
	ldrb	r0, [r3]	@ zero_extendqisi2
	ldr	r3, [r8, #672]
	mov	r3, r3, asl #1
	cmp	r0, #2
	orreq	r3, r3, #1
	cmp	r3, ip
	bne	.L1018
	cmp	lr, r2
	movcs	lr, r2
	add	r2, r2, #1
	cmp	r2, r4
	str	lr, [r5]
	bne	.L1019
.L1042:
	cmp	lr, #31
	bhi	.L1021
.L1020:
	add	ip, ip, #1
	add	r5, r5, #4
	cmp	ip, #32
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r4, #0
	bne	.L1043
.L1021:
	str	r7, [r5]
	b	.L1020
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
	beq	.L1057
	mov	r0, r6
	bl	InitListX
	cmp	r0, #0
	bne	.L1058
	ldr	r4, .L1061
	mov	r0, #13
	ldr	r1, .L1061+4
	bl	dprint_vfmw
	ldr	r3, [r4]
	tst	r3, #8192
	bne	.L1059
.L1048:
	mov	r0, r6
	bl	ReorderListX
	mov	r0, r6
	bl	RepairList
	cmp	r0, #0
	bne	.L1050
	mov	r0, r6
	bl	FindMinRefIdx
	add	r3, r6, #24576
	mov	r0, #13
	ldr	r1, .L1061+8
	ldr	r3, [r3, #2556]
	cmp	r3, #0
	ldrne	r3, [r3, #32]
	mvneq	r3, #0
	ldrne	r3, [r3, #164]
	str	r3, [r5, #1836]
	bl	dprint_vfmw
	ldr	r4, [r4]
	ands	r4, r4, #8192
	bne	.L1060
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1060:
	mov	r0, r6
	bl	DumpList.part.5
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1057:
	mov	r0, #0
	str	r0, [r5, #252]
	str	r0, [r5, #256]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1059:
	mov	r0, r6
	bl	DumpList.part.5
	b	.L1048
.L1050:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1058:
	ldr	r1, .L1061+12
	mov	r0, #13
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1062:
	.align	2
.L1061:
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
	bne	.L1069
	b	.L1070
.L1078:
	ldrb	r1, [r3, #-648]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1067
	cmp	r2, ip
	add	r3, r3, #792
	beq	.L1070
.L1069:
	ldr	r1, [r3]
	add	r2, r2, #1
	cmp	r1, #1
	bne	.L1078
.L1067:
	cmp	r2, ip
	strb	lr, [r3, #-651]
	strb	lr, [r3, #-650]
	add	r3, r3, #792
	bne	.L1069
.L1070:
	ldr	r2, [r4, #3824]
	cmp	r2, #0
	beq	.L1079
	add	r0, r0, #60928
	mov	r3, #0
	add	r0, r0, #164
	mov	r1, r3
.L1071:
	add	r3, r3, #1
	str	r1, [r0, #4]!
	cmp	r3, r2
	bne	.L1071
	ldmfd	sp, {r4, fp, sp, pc}
.L1079:
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
	ldr	r0, [r7, #1144]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1096
.L1081:
	ldr	r3, [fp, #-48]
	add	r6, r3, #57344
	ldr	r3, [r6, #3820]
	cmp	r3, #0
	beq	.L1088
	ldr	r3, [fp, #-48]
	mov	r8, #0
	mov	r9, #0
	mov	r10, #0
	add	r5, r3, #27904
	add	r5, r5, #88
.L1087:
	sub	r4, r5, #32
	mov	r3, #0
	strb	r3, [r5, #-591]
	strb	r3, [r5, #-590]
.L1086:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1097
.L1085:
	cmp	r4, r5
	bne	.L1086
	ldr	r3, [r6, #3820]
	add	r10, r10, #1
	add	r5, r5, #792
	cmp	r3, r10
	bhi	.L1087
.L1088:
	ldr	r2, [r6, #3824]
	cmp	r2, #0
	beq	.L1080
	ldr	r3, [fp, #-48]
	add	r6, r3, #60928
	mov	r3, #0
	add	r6, r6, #164
	mov	r1, r3
.L1089:
	add	r3, r3, #1
	str	r1, [r6, #4]!
	cmp	r3, r2
	bne	.L1089
.L1080:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1097:
	mov	r1, r2
	ldr	r0, [r7, #1144]
	bl	FreeUsdByDec
	strd	r8, [r4]
	b	.L1085
.L1096:
	ldr	r3, [fp, #-48]
	add	r0, r3, #8
	bl	ResetVoQueue
	b	.L1081
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
	bne	.L1100
	add	r8, r5, #24576
	ldr	ip, [r4, #3272]
	ldr	r8, [r8, #2144]
	add	r8, r8, #1
	cmp	ip, r8
	movls	r3, r1
	bls	.L1101
.L1100:
	ldr	r1, .L1105
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L1104
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
	ldr	r0, [r1, #1144]
	mov	r1, #2
	strh	lr, [fp, #-44]	@ movhi
	blx	ip
.L1104:
	ldr	ip, [r4, #3272]
	mov	r3, #1
.L1101:
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
.L1106:
	.align	2
.L1105:
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
	beq	.L1109
	bcc	.L1110
	cmp	r1, #2
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r2, #0
	add	r4, r0, #16384
	bne	.L1165
	ldr	r1, [r4, #3444]
	cmp	r1, #0
	bne	.L1143
	ldr	r2, [r4, #3484]
	ldr	r0, [r4, #3480]
	ldr	r1, [r4, #3492]
	cmp	r2, r0
	addcc	r1, r1, r3
	strcc	r1, [r4, #3488]
	bcs	.L1145
.L1146:
	add	r3, r1, r2
	str	r3, [r4, #3476]
	ldr	r0, [r6, #240]
	mov	r3, r3, asl #1
	cmp	r0, #0
	subeq	r3, r3, #1
	str	r3, [r4, #3464]
	ldrb	r0, [r6, #209]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1149
	str	r3, [r4, #3460]
	str	r3, [r4, #3456]
	str	r3, [r4, #3452]
.L1142:
	str	r2, [r4, #3480]
	str	r1, [r4, #3492]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1110:
	cmp	r2, #0
	add	lr, lr, #4
	add	r4, r0, #16384
	mov	ip, ip, asl lr
	bne	.L1116
	ldr	r3, [r4, #3444]
	cmp	r3, #0
	bne	.L1114
	ldr	lr, [r4, #3428]
	ldr	r1, [r4, #3432]
	mov	r2, lr
.L1115:
	ldr	r0, [r4, #3420]
	cmp	r0, r1
	bcs	.L1113
	rsb	r3, r0, r1
	cmp	r3, ip, lsr #1
	addcs	r2, r2, ip
	strcs	r2, [r4, #3436]
	bcc	.L1113
.L1117:
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1119
.L1167:
	ldr	r3, [r4, #3424]
	add	r2, r2, r0
	str	r2, [r4, #3452]
	add	r3, r2, r3
	str	r3, [r4, #3456]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r4, #3464]
.L1120:
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
.L1109:
	cmp	r2, #0
	add	r4, r0, #16384
	movne	r3, #0
	strne	r3, [r4, #3488]
	bne	.L1124
	ldr	r1, [r4, #3444]
	cmp	r1, #0
	beq	.L1125
	mov	r3, r2
	str	r2, [r4, #3492]
	str	r2, [r4, #3480]
.L1126:
	str	r3, [r4, #3488]
.L1124:
	ldr	r3, [r5, #2216]
	cmp	r3, #0
	beq	.L1128
	ldr	r3, [r4, #3484]
	ldr	r0, [r4, #3488]
	add	r0, r3, r0
	str	r0, [r4, #3476]
	ldr	r7, [r6, #240]
	cmp	r7, #0
	bne	.L1130
	cmp	r0, #0
	beq	.L1131
	sub	r0, r0, #1
	str	r0, [r4, #3476]
.L1130:
	mov	r3, #0
	str	r3, [r4, #3508]
	ldr	r1, [r5, #2216]
	cmp	r1, r3
	bgt	.L1151
	cmp	r0, #0
	beq	.L1134
.L1168:
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
	blt	.L1135
	add	r2, r5, #2208
	add	r1, r1, #1
	add	r2, r2, #8
	mov	r3, #0
.L1136:
	add	r3, r3, #1
	ldr	ip, [r2, #4]!
	cmp	r3, r1
	add	r0, r0, ip
	str	r0, [r4, #3504]
	bne	.L1136
.L1135:
	cmp	r7, #0
	ldreq	r3, [r5, #2208]
	addeq	r0, r0, r3
	streq	r0, [r4, #3504]
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1138
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
.L1139:
	ldr	r2, [r4, #3484]
	ldr	r3, [r4, #3488]
	str	r0, [r4, #3460]
	str	r2, [r4, #3480]
	str	r3, [r4, #3492]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1114:
	ldr	lr, [r4, #3448]
	cmp	lr, #0
	beq	.L1166
.L1116:
	ldr	r0, [r4, #3420]
	mov	r3, #0
	mov	lr, r3
	mov	r2, r1
	str	r3, [r4, #3428]
	str	r3, [r4, #3432]
.L1113:
	cmp	r0, r1
	bls	.L1118
	rsb	r1, r1, r0
	cmp	r1, ip, lsr #1
	rsbhi	r2, ip, r2
	strhi	r2, [r4, #3436]
	bhi	.L1117
.L1118:
	str	lr, [r4, #3436]
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1167
.L1119:
	ldrb	r3, [r6, #210]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r2, r0
	streq	r3, [r4, #3452]
	strne	r3, [r4, #3456]
	str	r3, [r4, #3464]
	b	.L1120
.L1165:
	mov	r3, #0
	ldr	r2, [r4, #3484]
	mov	r1, r3
	str	r3, [r4, #3488]
	str	r3, [r4, #3456]
	str	r3, [r4, #3452]
	str	r3, [r4, #3460]
	str	r3, [r4, #3464]
	b	.L1142
.L1128:
	str	r3, [r4, #3476]
	ldr	r7, [r6, #240]
.L1131:
	mov	r0, #0
	str	r0, [r4, #3508]
	ldr	r3, [r5, #2216]
	cmp	r3, r0
	ble	.L1134
.L1151:
	add	ip, r5, #2208
	mov	r3, #0
	add	ip, ip, #8
	mov	r2, r3
.L1133:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	add	r3, r3, r1
	str	r3, [r4, #3508]
	ldr	r1, [r5, #2216]
	cmp	r1, r2
	bgt	.L1133
	cmp	r0, #0
	bne	.L1168
.L1134:
	mov	r0, #0
	str	r0, [r4, #3504]
	b	.L1135
.L1138:
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
	b	.L1139
.L1125:
	ldr	r1, [r4, #3484]
	ldr	r2, [r4, #3480]
	cmp	r1, r2
	bcs	.L1127
	ldr	r2, [r4, #3492]
	add	r3, r3, r2
	str	r3, [r4, #3488]
	b	.L1124
.L1149:
	ldrb	r0, [r6, #210]	@ zero_extendqisi2
	str	r3, [r4, #3460]
	cmp	r0, #0
	streq	r3, [r4, #3452]
	strne	r3, [r4, #3456]
	b	.L1142
.L1143:
	str	r2, [r4, #3480]
	mov	r1, r2
	str	r2, [r4, #3492]
	ldr	r2, [r4, #3484]
.L1145:
	str	r1, [r4, #3488]
	b	.L1146
.L1166:
	ldr	r3, [r4, #3452]
	mov	r2, r1
	str	lr, [r4, #3428]
	mov	r1, r3
	str	r3, [r4, #3432]
	b	.L1115
.L1127:
	ldr	r3, [r4, #3492]
	b	.L1126
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
	beq	.L1170
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
	beq	.L1190
.L1189:
	ldr	ip, [lr, #4]!
	ldrb	r2, [ip, #2]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1184
	ldr	r2, [ip, #636]
	ldr	r8, [r6, #2384]
	cmp	r2, r8
	rsbhi	r2, r7, r2
	str	r2, [ip, #640]
	ldr	r2, [lr]
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L1187
	ldr	r8, [r2, #712]
	bic	r8, r8, #-16777216
	bic	r8, r8, #255
	cmp	r8, #65536
	ldreq	ip, [r2, #640]
	addeq	ip, r5, ip, lsl #1
	streq	ip, [r2, #720]
	ldreq	r2, [lr]
	ldreqb	ip, [r2, #2]	@ zero_extendqisi2
.L1187:
	tst	ip, #2
	beq	.L1184
	ldr	ip, [r2, #748]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	ldreq	ip, [r2, #640]
	addeq	ip, r4, ip, lsl #1
	streq	ip, [r2, #756]
.L1184:
	ldr	r2, [r3, #2152]
	add	r1, r1, #1
	cmp	r2, r1
	bhi	.L1189
.L1190:
	ldr	r2, [r3, #2156]
	cmp	r2, #0
	beq	.L1214
	add	r0, r0, #26624
	mov	r2, #0
	add	r0, r0, #28
.L1195:
	ldr	ip, [r0, #4]!
	add	r2, r2, #1
	ldrb	r1, [ip, #2]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1192
	tst	r1, #1
	beq	.L1193
	ldr	lr, [ip, #712]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	ldreq	r1, [ip, #644]
	addeq	r1, r5, r1, lsl #1
	streq	r1, [ip, #716]
	ldreq	ip, [r0]
	ldreqb	r1, [ip, #2]	@ zero_extendqisi2
.L1193:
	tst	r1, #2
	beq	.L1192
	ldr	r1, [ip, #748]
	bic	r1, r1, #-16777216
	bic	r1, r1, #255
	cmp	r1, #256
	ldreq	r1, [ip, #644]
	addeq	r1, r4, r1, lsl #1
	streq	r1, [ip, #752]
.L1192:
	ldr	r1, [r3, #2156]
	cmp	r1, r2
	bhi	.L1195
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1170:
	ldr	r1, [r3, #2152]
	cmp	r1, #0
	addne	ip, r0, #26368
	addne	ip, ip, #220
	bne	.L1179
.L1180:
	ldr	r2, [r3, #2156]
	cmp	r2, #0
	beq	.L1215
	add	r0, r0, #26624
	mov	r2, #0
	add	r0, r0, #28
	b	.L1182
.L1181:
	ldr	r1, [r3, #2156]
	cmp	r1, r2
	bls	.L1216
.L1182:
	ldr	r1, [r0, #4]!
	add	r2, r2, #1
	ldrb	ip, [r1, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1181
	ldr	ip, [r1, #676]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	ldreq	ip, [r1, #644]
	streq	ip, [r1, #680]
	ldr	r1, [r3, #2156]
	cmp	r1, r2
	bhi	.L1182
.L1216:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1217:
	ldr	lr, [r1, #676]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L1176
	ldr	lr, [r1, #636]
	ldr	r4, [r6, #2384]
	rsb	r5, r7, lr
	cmp	lr, r4
	strhi	r5, [r1, #640]
	strls	lr, [r1, #640]
	ldr	r1, [ip]
	ldr	lr, [r1, #640]
	str	lr, [r1, #684]
.L1176:
	ldr	r1, [r3, #2152]
	add	r2, r2, #1
	cmp	r1, r2
	bls	.L1180
.L1179:
	ldr	r1, [ip, #4]!
	ldrb	lr, [r1, #2]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L1176
	b	.L1217
.L1215:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1214:
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
	beq	.L1223
	add	r3, r0, #24576
	ldr	ip, [r3, #2148]
	cmp	ip, #0
	beq	.L1223
	ldr	r3, [r3, #1952]
	cmp	r3, r1
	beq	.L1225
	add	r0, r0, #26368
	mov	r3, #0
	add	r0, r0, #160
	b	.L1220
.L1221:
	ldr	r2, [r0, #4]!
	cmp	r2, r1
	beq	.L1225
.L1220:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1221
.L1223:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1225:
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
	ldr	r1, .L1229
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
	ldr	r1, .L1229+4
	moveq	r2, #1
	str	ip, [sp]
	mov	r0, #22
	bl	dprint_vfmw
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1230:
	.align	2
.L1229:
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
	add	r7, r0, #57344
	add	r5, r0, #61440
	mov	r4, r0
	ldr	lr, [r7, #2444]
	mov	r0, #2
	ldr	ip, [r7, #1908]
	ldrb	r3, [r7, #1739]	@ zero_extendqisi2
	ldr	r2, [r7, #2392]
	ldr	r1, .L1269
	stmia	sp, {ip, lr}
	bl	dprint_vfmw
	ldr	lr, [r5, #1156]
	mov	r3, #0
	str	r3, [r5, #1068]
	cmp	lr, #1
	str	r3, [r5, #1168]
	beq	.L1232
	add	r2, r4, #17408
	add	r3, r4, #13568
	add	r2, r2, #188
	add	r3, r3, #28
.L1233:
	movw	r1, #59696
	add	ip, r4, #61184
	ldrd	r0, [r1, r4]
	add	r8, r4, #16384
	cmp	lr, #1
	strd	r0, [ip, #40]
	ldr	r1, [r5, #1064]
	str	r1, [r5, #784]
	ldr	r1, [r7, #1892]
	str	r1, [r5, #564]
	ldr	r1, [r7, #1896]
	str	r1, [r5, #568]
	ldr	r1, [r7, #1900]
	str	r1, [r5, #572]
	ldr	r0, [r7, #2392]
	ldrb	r1, [r7, #1739]	@ zero_extendqisi2
	ldr	ip, [r7, #2428]
	str	r0, [r7, #3888]
	strb	r1, [r7, #3872]
	ldr	r0, [r7, #2432]
	ldrb	r1, [r3, #19]	@ zero_extendqisi2
	str	ip, [r7, #3892]
	str	r0, [r7, #3896]
	str	r1, [r7, #3904]
	ldr	r1, [r3, #3248]
	add	r1, r1, #1
	str	r1, [r7, #3900]
	ldrb	r1, [r3, #18]	@ zero_extendqisi2
	strb	r1, [r7, #3875]
	ldr	r1, [r2, #60]
	str	r1, [r7, #3908]
	ldrb	r1, [r2]	@ zero_extendqisi2
	str	r1, [r7, #3912]
	ldr	r3, [r3, #40]
	strb	r3, [r7, #3873]
	ldrb	r3, [r2, #4]	@ zero_extendqisi2
	str	r3, [r7, #3916]
	ldr	r3, [r8, #3464]
	str	r3, [r7, #3920]
	ldr	r3, [r8, #3452]
	str	r3, [r7, #3924]
	ldr	r3, [r8, #3456]
	str	r3, [r7, #3928]
	beq	.L1249
	ldr	r3, [r7, #2376]
	ldr	r0, [r5, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_GetLogicFs
	subs	r9, r0, #0
	beq	.L1268
	ldr	r2, [r9, #640]
	ldr	r3, [r9, #644]
	cmp	r2, #0
	beq	.L1238
	cmp	r3, #0
	beq	.L1238
	add	r3, r4, #61440
	add	r1, r4, #61696
	add	r1, r1, #216
	ldr	r0, [r5, #1144]
	mov	r2, r3
	add	r3, r3, #212
	add	r2, r2, #84
	add	r6, r4, #24576
	bl	FSP_GetDecFsAddrTab
	ldr	r3, [r9, #640]
	add	r2, r4, #61696
	add	r1, r4, #61952
	add	r2, r2, #220
	add	r1, r1, #44
	ldr	r3, [r3, #8]
	ldr	r0, [r5, #1144]
	str	r3, [r5, #68]
	bl	FSP_GetPmvAddrTab
	ldr	r3, [r7, #2444]
	str	r3, [r5, #76]
	ldr	r3, [r9, #656]
	str	r3, [r5, #560]
	ldr	r3, [r9, #644]
	ldr	r3, [r3, #16]
	str	r3, [r5, #72]
	ldr	r3, [r9, #644]
	ldr	r3, [r3, #76]
	str	r3, [r5, #80]
	ldr	r3, [r6, #2420]
	cmp	r3, #0
	str	r3, [fp, #-48]
	str	r3, [r5, #776]
	beq	.L1244
	add	lr, r4, #61952
	add	r7, r4, #26624
	add	r1, r4, #26880
	ldr	r10, [fp, #-48]
	mov	ip, r7
	mov	r0, lr
	mov	r2, lr
	add	r7, r7, #176
	add	lr, lr, #68
	add	ip, ip, #240
	add	r0, r0, #132
	add	r1, r1, #48
	add	r2, r2, #196
	mov	r3, #0
.L1243:
	ldr	r9, [r7, #4]!
	add	r3, r3, #1
	cmp	r3, r10
	str	r9, [lr, #4]!
	ldr	r9, [ip, #4]!
	str	r9, [r0, #4]!
	ldr	r9, [r1, #4]!
	str	r9, [r2, #4]!
	bne	.L1243
.L1244:
	add	r3, r4, #12288
	ldrb	r3, [r3, #1333]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1241
	ldrb	r1, [r8, #1230]	@ zero_extendqisi2
.L1245:
	cmp	r1, #1
	add	ip, r4, #61184
	mov	r0, #0
	add	ip, ip, #88
	beq	.L1250
.L1253:
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
	bne	.L1253
	add	r9, r4, #61184
	mov	r10, #1
	add	r9, r9, #188
	mov	r8, #0
.L1254:
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
	bne	.L1254
.L1252:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L1249
	add	r9, r4, #62208
	add	r7, r4, #26624
	add	r7, r7, #180
	mov	r8, #0
	mov	r4, r9
	add	r9, r9, #144
	add	r4, r4, #208
.L1255:
	ldr	r2, [r7]
	mov	r1, #0
	ldr	r0, [r5, #1144]
	add	r8, r8, #1
	bl	FSP_GetStoreType
	adds	r0, r0, #0
	movne	r0, #1
	str	r0, [r9, #4]!
	ldr	r3, [r7], #4
	str	r3, [r4, #4]!
	ldr	r3, [r6, #2420]
	cmp	r3, r8
	bhi	.L1255
.L1249:
	mov	r0, #0
.L1265:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1250:
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
	bne	.L1250
	add	r9, r4, #61184
	mov	r10, #1
	add	r9, r9, #188
	mov	r8, #0
.L1251:
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
	bne	.L1251
	b	.L1252
.L1241:
	ldrb	r1, [r8, #1230]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L1245
	add	r0, r4, #61184
	ldr	r9, .L1269+4
	add	r0, r0, #88
.L1246:
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
	bne	.L1246
	add	r8, r4, #61184
	mov	r7, #0
	add	r8, r8, #188
.L1247:
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
	bne	.L1247
	b	.L1252
.L1232:
	add	r1, r4, #12288
	add	r0, r4, #20480
	mov	r6, #2208
	mov	ip, #4000
	ldr	r3, [r1, #1304]
	ldr	r2, [r0, #220]
	ldr	r1, [r1, #1300]
	mla	r2, r6, r2, r3
	ldr	r3, [r2, #28]
	mla	r3, ip, r3, r1
	b	.L1233
.L1238:
	ldr	r1, .L1269+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1265
.L1268:
	ldr	r3, [r7, #2376]
	ldr	r1, .L1269+12
	ldr	r2, [r3, #632]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1265
.L1270:
	.align	2
.L1269:
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
	add	r7, r0, #61440
	mov	r4, r0
	ldr	r3, [r7, #1156]
	ldr	r2, [r7, #1068]
	cmp	r3, #1
	str	r2, [fp, #-48]
	beq	.L1273
	add	ip, r0, #17408
	add	r8, r0, #13568
	add	ip, ip, #188
	add	r8, r8, #28
	add	r3, r0, #12288
	add	r5, r0, #20480
.L1274:
	ldr	r1, [fp, #-48]
	movw	lr, #4060
	ldr	r9, [r7, #1064]
	add	r2, r1, #1
	str	r2, [r7, #1068]
	mul	lr, lr, r1
	ldr	r3, [r3, #1284]
	sub	r3, r3, #1
	cmp	r1, r3
	add	r6, r9, lr
	addcc	r3, lr, #4048
	addcc	r3, r3, #12
	movcs	r3, #0
	addcc	r3, r9, r3
	str	r3, [r6, #4056]
	ldr	r1, [r4, #368]
	ldrb	r2, [r1]	@ zero_extendqisi2
	ldr	r3, [r1, #68]
	cmp	r2, #1
	cmpls	r2, r3
	bcs	.L1277
	add	r0, r2, #3
	add	r0, r6, r0, lsl #2
.L1278:
	mov	r3, r2, asl #5
	sub	r3, r3, r2, asl #2
	add	r2, r2, #1
	add	r1, r1, r3
	ldr	r1, [r1, #24]
	str	r1, [r0, #4]!
	ldr	r1, [r4, #368]
	add	r1, r1, r3
	ldr	r1, [r1, #20]
	str	r1, [r0, #-8]
	ldr	r1, [r4, #368]
	add	r3, r1, r3
	ldr	r3, [r3, #28]
	str	r3, [r0, #8]
	ldr	r1, [r4, #368]
	ldr	r3, [r1, #68]
	cmp	r3, r2
	movhi	r10, #1
	movls	r10, #0
	cmp	r2, #1
	movhi	r10, #0
	cmp	r10, #0
	bne	.L1278
.L1277:
	cmp	r3, #1
	bhi	.L1283
	add	r3, r3, #3
	add	r1, r6, #20
	mov	r2, #0
	add	r3, r6, r3, lsl #2
.L1282:
	str	r2, [r3, #4]!
	cmp	r3, r1
	str	r2, [r3, #-8]
	str	r2, [r3, #8]
	bne	.L1282
.L1283:
	add	r10, r4, #57344
	ldrb	r3, [r10, #1745]	@ zero_extendqisi2
	strb	r3, [r6, #1]
	ldr	r3, [ip, #48]
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
	strb	r2, [r9, lr]
	ldr	r2, [r5, #260]
	str	r2, [r6, #48]
	ldrb	r2, [r5, #214]	@ zero_extendqisi2
	strb	r2, [r6, #2]
	ldrb	r2, [r8, #20]	@ zero_extendqisi2
	strb	r2, [r6, #3]
	ldr	r2, [r5, #252]
	str	r2, [r6, #52]
	ldr	r2, [r5, #256]
	str	r2, [r6, #56]
	ldrb	r2, [r5, #208]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1381
	cmp	r2, #0
	ldreqb	r2, [ip, #2]	@ zero_extendqisi2
	movne	r2, #0
	strb	r2, [r6, #4]
.L1284:
	ldr	r2, [ip, #52]
	str	r2, [r6, #60]
	ldr	r2, [ip, #56]
	str	r2, [r6, #64]
	ldr	r2, [r5, #1828]
	str	r2, [r6, #68]
	ldr	r2, [r5, #1832]
	str	r2, [r6, #72]
	ldr	r2, [r5, #1824]
	str	r2, [r6, #76]
	ldr	r2, [r7, #1156]
	cmp	r2, #1
	beq	.L1272
	ldr	r2, [fp, #-48]
	cmp	r2, #0
	ldr	r2, [r10, #2376]
	beq	.L1382
	ldr	r0, [r2, #700]
	ldr	r1, [r2, #704]
	cmp	r0, r3
	movlt	r0, r3
	cmp	r1, r3
	str	r0, [r2, #700]
	movge	r1, r3
	str	r1, [r2, #704]
.L1288:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1325
	ldr	r8, [r5, #252]
	cmp	r8, #0
	beq	.L1328
	add	r3, r4, #24576
	ldr	r3, [r3, #2556]
	ldr	r1, [r3, #32]
	ldrb	r3, [r1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r2, r4, #26880
	movne	r3, #0
	addne	r2, r2, #252
	bne	.L1293
	b	.L1291
.L1295:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #32]
	ldrb	r0, [r1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1291
.L1293:
	add	r3, r3, #1
	cmp	r3, r8
	bne	.L1295
	mov	r3, #0
	str	r3, [fp, #-52]
.L1290:
	ldr	r9, [r5, #256]
	cmp	r9, #0
	beq	.L1329
	add	r3, r4, #24576
	ldr	r3, [r3, #2688]
	ldr	r1, [r3, #32]
	ldrb	r3, [r1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r2, r4, #27136
	movne	r3, #0
	addne	r2, r2, #128
	bne	.L1299
	b	.L1297
.L1301:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #32]
	ldrb	r0, [r1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1297
.L1299:
	add	r3, r3, #1
	cmp	r3, r9
	bne	.L1301
	mov	r3, #0
	str	r3, [fp, #-56]
.L1296:
	ldrb	ip, [r10, #1739]	@ zero_extendqisi2
	ldr	r3, [r5, #252]
	cmp	ip, #0
	beq	.L1302
	cmp	r3, #0
	beq	.L1317
	add	r10, r4, #26880
	mov	r9, #0
	str	r4, [fp, #-60]
	mov	r8, r6
	mov	r4, r5
	add	r10, r10, #252
	mov	r5, r9
	ldr	r9, [fp, #-52]
	b	.L1316
.L1314:
	strb	r2, [r8, #1630]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #1631]
	ldr	r2, [r10]
	ldr	r2, [r2, #12]
	str	r2, [r8, #1656]
.L1315:
	ldr	r2, [r4, #252]
	add	r5, r5, #1
	add	r10, r10, #4
	add	r8, r8, #36
	cmp	r2, r5
	bls	.L1383
.L1316:
	ldr	r2, [r10]
	ldr	r0, [r7, #1144]
	ldr	r2, [r2, #32]
	ldr	r1, [r2, #632]
	bl	FSP_GetLogicFs
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #668]
	str	r2, [r8, #1636]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #1625]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r9
	beq	.L1313
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1313:
	str	r0, [r8, #1640]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #672]
	str	r2, [r8, #1644]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #648]
	str	r2, [r8, #1632]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r8, #1624]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2, #676]	@ zero_extendqisi2
	strb	r2, [r8, #1626]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1314
	strb	r2, [r8, #1628]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #1629]
	ldr	r2, [r10]
	ldr	r2, [r2, #12]
	str	r2, [r8, #1652]
	b	.L1315
.L1383:
	mov	r5, r4
	ldr	r4, [fp, #-60]
.L1317:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1384
.L1305:
	add	r2, r4, #26880
	add	r3, r6, #3920
	add	r0, r6, #4048
	add	r2, r2, #120
	add	r3, r3, #4
	add	r0, r0, #4
.L1322:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L1322
	add	r3, r4, #16384
	ldrb	r2, [r3, #1214]	@ zero_extendqisi2
	cmp	r2, #0
	str	r2, [r7, #60]
	ldr	r3, [r3, #1256]
	str	r3, [r7, #64]
	beq	.L1323
	ldrb	r2, [r5, #208]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1324
.L1323:
	cmp	r3, #1
	beq	.L1385
.L1325:
	ldr	r3, [fp, #-48]
	mov	r2, #0
	ldr	r1, [r4, #368]
	add	r3, r3, #15104
	add	r3, r3, #34
	str	r1, [r4, r3, asl #2]
	str	r2, [r4, #368]
.L1272:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1382:
	str	r3, [r2, #700]
	ldr	r3, [r10, #2376]
	ldr	r2, [r6, #32]
	str	r2, [r3, #704]
	b	.L1288
.L1291:
	ldr	r1, [r1, #632]
	ldr	r0, [r7, #1144]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	movweq	r2, #7517
	beq	.L1377
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-52]
	b	.L1290
.L1381:
	ldr	r2, [ip, #44]
	strb	r2, [r6, #4]
	b	.L1284
.L1273:
	add	r3, r0, #12288
	add	r5, r0, #20480
	mov	ip, #2208
	mov	r8, #4000
	ldr	r1, [r3, #1304]
	ldr	r0, [r5, #220]
	ldr	r2, [r3, #1300]
	mla	ip, ip, r0, r1
	ldr	r1, [ip, #28]
	mla	r8, r8, r1, r2
	b	.L1274
.L1302:
	cmp	r3, #0
	beq	.L1310
	add	r10, r4, #26880
	ldr	r9, [fp, #-52]
	add	r10, r10, #252
	str	r4, [fp, #-60]
	mov	r8, r6
	mov	r4, r5
	mov	r5, ip
.L1309:
	ldr	r2, [r10]
	ldr	r0, [r7, #1144]
	ldr	r2, [r2, #32]
	ldr	r1, [r2, #632]
	bl	FSP_GetLogicFs
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #668]
	str	r2, [r8, #1636]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #1625]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r9
	beq	.L1308
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1308:
	str	r0, [r8, #1640]
	mov	r3, #0
	ldr	r2, [r10]
	add	r5, r5, #1
	add	r8, r8, #36
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #672]
	str	r2, [r8, #1608]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #648]
	str	r2, [r8, #1596]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #1591]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r8, #1590]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #688]
	str	r2, [r8, #1612]
	ldr	r2, [r10]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #724]
	str	r2, [r8, #1616]
	ldr	r2, [r10], #4
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #760]
	strb	r3, [r8, #1588]
	str	r2, [r8, #1620]
	ldr	r2, [r4, #252]
	cmp	r2, r5
	bhi	.L1309
	mov	r5, r4
	ldr	r4, [fp, #-60]
.L1310:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1305
	ldr	r3, [r5, #256]
	cmp	r3, #0
	beq	.L1305
	mov	r9, #0
	add	r8, r4, #27136
	str	r4, [fp, #-52]
	mov	r4, r5
	mov	r5, r9
	ldr	r9, [fp, #-56]
	add	r8, r8, #128
	mov	r10, r6
.L1312:
	ldr	r2, [r8]
	ldr	r0, [r7, #1144]
	ldr	r2, [r2, #32]
	ldr	r1, [r2, #632]
	bl	FSP_GetLogicFs
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #668]
	str	r2, [r10, #2788]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2777]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r9
	beq	.L1311
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1311:
	str	r0, [r10, #1640]
	mov	r3, #0
	ldr	r2, [r8]
	add	r5, r5, #1
	add	r10, r10, #36
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #672]
	str	r2, [r10, #2760]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #648]
	str	r2, [r10, #2748]
	ldr	r2, [r8]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2743]
	ldr	r2, [r8]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r10, #2742]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #688]
	str	r2, [r10, #2764]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #724]
	str	r2, [r10, #2768]
	ldr	r2, [r8], #4
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #760]
	strb	r3, [r10, #2740]
	str	r2, [r10, #2772]
	ldr	r2, [r4, #256]
	cmp	r2, r5
	bhi	.L1312
.L1376:
	mov	r5, r4
	ldr	r4, [fp, #-52]
	b	.L1305
.L1297:
	ldr	r1, [r1, #632]
	ldr	r0, [r7, #1144]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L1386
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-56]
	b	.L1296
.L1385:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1325
.L1324:
	ldr	r3, [r5, #272]
	add	r8, r4, #20736
	add	ip, r4, #21504
	add	lr, r4, #20992
	add	r1, r4, #21760
	mov	r7, r8
	str	r3, [r6, #80]
	mov	r0, ip
	ldr	r9, [r5, #276]
	add	r8, r8, #20
	add	r7, r7, #148
	add	lr, lr, #20
	add	ip, ip, #20
	add	r0, r0, #148
	add	r1, r1, #20
	mov	r10, r6
	mov	r3, r6
	mov	r2, #0
	str	r6, [fp, #-52]
	str	r9, [r10, #84]!
.L1326:
	ldr	r9, [r8, #4]!
	add	r2, r2, #1
	add	r3, r3, #4
	str	r9, [r10, #4]!
	ldr	r9, [r7, #4]!
	str	r9, [r3, #212]
	ldr	r9, [lr, #4]!
	str	r9, [r3, #340]
	ldr	r9, [ip, #4]!
	str	r9, [r3, #852]
	ldr	r9, [r0, #4]!
	str	r9, [r3, #980]
	ldr	r9, [r1, #4]!
	str	r9, [r3, #1108]
	ldr	r9, [r5, #264]
	cmp	r9, r2
	bcs	.L1326
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1325
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
.L1327:
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
	bcs	.L1327
	b	.L1325
.L1384:
	ldr	r3, [r5, #256]
	cmp	r3, #0
	beq	.L1305
	add	r8, r4, #27136
	mov	r9, #0
	str	r4, [fp, #-52]
	mov	r10, r6
	mov	r4, r5
	add	r8, r8, #128
	mov	r5, r9
	ldr	r9, [fp, #-56]
	b	.L1321
.L1319:
	strb	r2, [r10, #2782]
	ldr	r2, [r8]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2783]
	ldr	r2, [r8]
	ldr	r2, [r2, #12]
	str	r2, [r10, #2808]
.L1320:
	ldr	r2, [r4, #256]
	add	r5, r5, #1
	add	r8, r8, #4
	add	r10, r10, #36
	cmp	r2, r5
	bls	.L1376
.L1321:
	ldr	r2, [r8]
	ldr	r0, [r7, #1144]
	ldr	r2, [r2, #32]
	ldr	r1, [r2, #632]
	bl	FSP_GetLogicFs
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #668]
	str	r2, [r10, #2788]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2777]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r9
	beq	.L1318
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1318:
	str	r0, [r10, #2792]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #672]
	str	r2, [r10, #2796]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldr	r2, [r2, #648]
	str	r2, [r10, #2784]
	ldr	r2, [r8]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r10, #2776]
	ldr	r2, [r8]
	ldr	r2, [r2, #32]
	ldrb	r2, [r2, #676]	@ zero_extendqisi2
	strb	r2, [r10, #2778]
	ldr	r2, [r8]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1319
	strb	r2, [r10, #2780]
	ldr	r2, [r8]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2781]
	ldr	r2, [r8]
	ldr	r2, [r2, #12]
	str	r2, [r10, #2804]
	b	.L1320
.L1328:
	str	r8, [fp, #-52]
	b	.L1290
.L1329:
	str	r9, [fp, #-56]
	b	.L1296
.L1386:
	movw	r2, #7538
.L1377:
	ldr	r1, .L1387
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L1388:
	.align	2
.L1387:
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
	beq	.L1393
	mov	r0, #0
.L1392:
	ldr	r3, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r1
	add	r3, r3, #7
	cmp	r2, #0
	add	r0, r0, r3, lsr #3
	bne	.L1392
	ldmfd	sp, {fp, sp, pc}
.L1393:
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
	ldr	r1, .L1406
	add	r4, r0, #20480
	add	r6, r0, #61440
	mov	r5, r0
	bl	ue_v
	str	r0, [r4, #260]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L1404
	cmp	r0, #262144
	bcs	.L1405
	add	r2, r5, #12288
	ldr	r1, [r2, #1276]
	ldr	r3, [r2, #1280]
	mul	r3, r3, r1
	sub	r3, r3, #1
	cmp	r0, r3
	bhi	.L1404
	mov	r0, r5
	ldr	r1, .L1406+4
	bl	ue_v
	ldr	r1, .L1406+8
	mov	r2, r0
	mov	r5, r0
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L1404
	cmp	r5, #9
	bhi	.L1400
	mov	r1, #1
	movw	r0, #297
	mov	r2, r1, asl r5
	and	r0, r0, r2
	cmp	r0, #0
	bne	.L1401
	ands	r3, r2, #660
	bne	.L1402
	tst	r2, #66
	beq	.L1400
	strb	r1, [r4, #208]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1401:
	strb	r3, [r4, #208]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1400:
	mov	r2, r5
	ldr	r1, .L1406+12
	mov	r0, #1
	bl	dprint_vfmw
.L1404:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1402:
	mov	r3, #2
	strb	r3, [r4, #208]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1405:
	ldr	r1, .L1406+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1407:
	.align	2
.L1406:
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
	ldr	r1, .L1414
	ldr	r2, [r5, #220]
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r1, [r4, #1304]
	ldr	r2, [r5, #220]
	mov	r3, #2208
	mla	r3, r3, r2, r1
	ldrb	r1, [r3, #19]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1412
	ldr	r2, [r3, #28]
	mov	r3, #4000
	ldr	r1, [r4, #1300]
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1413
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1412:
	ldr	r1, .L1414+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1413:
	ldr	r1, .L1414+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1415:
	.align	2
.L1414:
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
	ldr	r1, .L1424
	mov	r0, #19
	bl	dprint_vfmw
	add	r0, r4, #12288
	mov	r3, #2208
	ldr	r2, [r0, #1304]
	mla	r3, r3, r5, r2
	ldrb	r2, [r3, #19]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1421
	ldr	r1, [r0, #1288]
	ldr	r2, [r3, #28]
	sub	r3, r1, #1
	cmp	r2, r3
	bhi	.L1422
	ldr	r1, [r0, #1300]
	mov	r3, #4000
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1423
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1421:
	mov	r2, r5
	ldr	r1, .L1424+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1422:
	ldr	r1, .L1424+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1423:
	ldr	r1, .L1424+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1425:
	.align	2
.L1424:
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
	beq	.L1428
	ldrb	ip, [r3, #1857]	@ zero_extendqisi2
	ldrb	r1, [r3, #210]	@ zero_extendqisi2
	cmp	ip, r1
	orrne	r2, r2, #1
.L1428:
	ldr	r1, [r3, #1868]
	ldr	ip, [r3, #240]
	cmp	r1, ip
	moveq	r1, #0
	beq	.L1429
	cmp	ip, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
.L1429:
	ldrb	ip, [r3, #1858]	@ zero_extendqisi2
	ldrb	r4, [r3, #211]	@ zero_extendqisi2
	cmp	ip, r4
	orrne	r2, r2, #1
	cmp	r4, #5
	cmpeq	ip, #5
	orr	r2, r2, r1
	beq	.L1437
.L1430:
	ldr	r1, [lr, #3508]
	cmp	r1, #0
	bne	.L1431
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
.L1432:
	ldr	r4, [r3, #260]
	mov	r0, #19
	ldr	r1, .L1438
	cmp	r4, #0
	movne	r4, r2
	orreq	r4, r2, #1
	mov	r2, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1431:
	cmp	r1, #1
	bne	.L1432
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
	b	.L1432
.L1437:
	ldr	ip, [r3, #1876]
	ldr	r1, [r3, #248]
	cmp	ip, r1
	orrne	r2, r2, #1
	b	.L1430
.L1439:
	.align	2
.L1438:
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
	bls	.L1494
.L1493:
	ldr	r3, [r6, #1084]
.L1459:
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1494:
	ldr	r1, .L1499
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r8, #3512]
	cmp	r0, #0
	beq	.L1492
	ldr	r3, [r5, #264]
	cmn	r3, #2
	beq	.L1473
	add	r10, r4, #19712
	add	r6, r4, #61440
	add	r10, r10, #184
	mov	r9, #0
	b	.L1456
.L1451:
	cmp	r0, #2
	beq	.L1495
.L1454:
	ldr	r3, [r5, #264]
	add	r3, r3, #2
	cmp	r3, r9
	bls	.L1496
.L1456:
	ldr	r1, .L1499+4
	mov	r0, r4
	bl	ue_v
	add	r9, r9, #1
	cmp	r0, #3
	str	r0, [r10, #4]!
	ldr	r3, [r6, #1084]
	bhi	.L1447
	cmp	r3, #0
	bne	.L1447
	cmp	r0, #3
	beq	.L1450
	cmp	r0, #1
	bhi	.L1451
	ldr	r1, .L1499+8
	mov	r0, r4
	bl	ue_v
	cmp	r7, r0
	str	r0, [r10, #264]
	ldr	r3, [r6, #1084]
	bls	.L1453
	cmp	r3, #0
	beq	.L1454
.L1453:
	mov	r2, r0
	ldr	r1, .L1499+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1495:
	ldr	r1, .L1499+16
	mov	r0, r4
	bl	ue_v
	str	r0, [r10, #528]
	b	.L1454
.L1492:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	add	r6, r4, #61440
.L1443:
	cmp	r3, #1
	bne	.L1493
	ldr	r1, .L1499+20
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r8, #3513]
	cmp	r0, #0
	beq	.L1493
	ldr	r3, [r5, #268]
	cmn	r3, #2
	beq	.L1493
	add	r9, r4, #19968
	mov	r8, #0
	add	r9, r9, #60
	b	.L1471
.L1470:
	add	r8, r8, #1
	cmp	r8, r3
	bcs	.L1493
.L1471:
	ldr	r1, .L1499+24
	mov	r0, r4
	bl	ue_v
	cmp	r0, #3
	str	r0, [r9, #4]!
	ldr	r3, [r6, #1084]
	bhi	.L1463
	cmp	r3, #0
	bne	.L1463
	cmp	r0, #3
	beq	.L1459
	cmp	r0, #1
	bls	.L1497
	cmp	r0, #2
	beq	.L1498
	ldr	r3, [r5, #268]
	add	r3, r3, #2
	cmp	r3, r8
	bne	.L1470
.L1472:
	ldr	r1, .L1499+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1497:
	ldr	r1, .L1499+32
	mov	r0, r4
	bl	ue_v
	cmp	r7, r0
	str	r0, [r9, #264]
	ldr	r3, [r6, #1084]
	bls	.L1467
	cmp	r3, #0
	bne	.L1467
.L1468:
	ldr	r2, [r5, #268]
	add	r3, r2, #2
	cmp	r3, r8
	bne	.L1470
	add	r2, r2, #4992
	add	r2, r2, #18
	ldr	r2, [r4, r2, asl #2]
	cmp	r2, #3
	beq	.L1470
	b	.L1472
.L1498:
	ldr	r1, .L1499+36
	mov	r0, r4
	bl	ue_v
	str	r0, [r9, #524]
	b	.L1468
.L1450:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	b	.L1443
.L1447:
	mov	r2, r0
	ldr	r1, .L1499+40
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1463:
	mov	r2, r0
	ldr	r1, .L1499+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1496:
	cmp	r9, r3
	bne	.L1450
.L1445:
	add	r3, r3, #4928
	add	r3, r3, #46
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #4]
	cmp	r3, #3
	beq	.L1492
	ldr	r1, .L1499+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1467:
	mov	r2, r0
	ldr	r1, .L1499+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1473:
	mov	r3, #0
	b	.L1445
.L1500:
	.align	2
.L1499:
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
	ldr	r1, .L1531
	mla	r2, r10, ip, r2
	str	r2, [fp, #-48]
	ldr	r2, [r2, #28]
	mla	r7, r7, r2, r3
	bl	ue_v
	str	r0, [r6, #272]
	mov	r8, r4, asl r0
	ldr	r9, [r7, #40]
	cmp	r9, #0
	bne	.L1525
.L1502:
	cmp	r0, #7
	bhi	.L1505
	ldr	r3, [r6, #276]
	cmp	r3, #7
	bhi	.L1505
	add	r3, r5, #61440
	str	r3, [fp, #-52]
	ldr	r2, [r3, #1084]
	cmp	r2, #0
	addeq	r10, r5, #20736
	moveq	r4, r2
	addeq	r10, r10, #152
	beq	.L1513
	b	.L1504
.L1509:
	ldr	r2, [r7, #40]
	cmp	r2, #0
	bne	.L1526
.L1510:
	str	r2, [r10]
	str	r2, [r10, #768]
	str	r2, [r10, #128]
	str	r2, [r10, #896]
.L1512:
	ldr	r2, [r6, #264]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r2, r4
	bcc	.L1527
.L1513:
	ldr	r1, .L1531+4
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1509
	ldr	r1, .L1531+8
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+12
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #640]
	ldr	r2, [r7, #40]
	cmp	r2, #0
	beq	.L1510
.L1526:
	ldr	r1, .L1531+16
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1528
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1512
.L1527:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #44]
	cmp	r3, #1
	beq	.L1514
.L1515:
	ldr	r3, [fp, #-52]
	ldr	r0, [r3, #1084]
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
.L1507:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1528:
	ldr	r1, .L1531+20
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+24
	str	r0, [r10]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+20
	str	r0, [r10, #768]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+24
	str	r0, [r10, #128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #896]
	b	.L1512
.L1525:
	ldr	r1, .L1531+28
	mov	r0, r5
	bl	ue_v
	mov	r3, r0
	ldr	r0, [r6, #272]
	mov	r9, r4, asl r3
	str	r3, [r6, #276]
	b	.L1502
.L1514:
	ldrb	r3, [r6, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1515
	add	r10, r5, #21248
	mov	r4, #0
	add	r10, r10, #24
	b	.L1521
.L1517:
	ldr	r3, [r7, #40]
	cmp	r3, #0
	bne	.L1529
.L1518:
	str	r3, [r10]
	str	r3, [r10, #768]
	str	r3, [r10, #128]
	str	r3, [r10, #896]
.L1520:
	ldr	r3, [r6, #268]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r3, r4
	bcc	.L1515
.L1521:
	ldr	r1, .L1531+32
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1517
	ldr	r1, .L1531+36
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+40
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #640]
	ldr	r3, [r7, #40]
	cmp	r3, #0
	beq	.L1518
.L1529:
	ldr	r1, .L1531+44
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1530
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1520
.L1505:
	add	r5, r5, #61440
	ldr	r2, [r5, #1084]
.L1504:
	ldr	r1, .L1531+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1507
.L1530:
	ldr	r1, .L1531+52
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+56
	str	r0, [r10]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+52
	str	r0, [r10, #768]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1531+56
	str	r0, [r10, #128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #896]
	b	.L1520
.L1532:
	.align	2
.L1531:
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
	beq	.L1545
	add	r8, r0, #24576
	mov	r10, #0
	b	.L1536
.L1567:
	ldr	r1, .L1573
	mov	r0, r9
	bl	ue_v
	cmp	r4, #2
	beq	.L1565
.L1547:
	cmp	r4, #6
	orreq	r5, r5, #1
	cmp	r5, #0
	bne	.L1550
	cmp	r4, #4
	beq	.L1566
.L1551:
	cmp	r4, #6
	bhi	.L1543
.L1548:
	add	r3, r9, #61440
	ldr	r3, [r3, #1084]
	cmp	r3, #0
	bne	.L1543
	cmp	r4, #0
	beq	.L1553
.L1552:
	subs	r6, r6, #1
	beq	.L1554
.L1545:
	ldr	r1, .L1573+4
	mov	r0, r9
	bl	ue_v
	bic	r3, r0, #2
	sub	r5, r0, #3
	clz	r5, r5
	cmp	r3, #1
	mov	r4, r0
	mov	r5, r5, lsr #5
	beq	.L1567
	cmp	r4, #2
	bne	.L1547
.L1565:
	ldr	r1, .L1573+8
	mov	r0, r9
	bl	ue_v
	cmp	r5, #0
	beq	.L1548
.L1550:
	ldr	r1, .L1573+12
	mov	r0, r9
	bl	ue_v
	cmp	r4, #4
	bne	.L1551
.L1566:
	ldr	r1, .L1573+16
	mov	r0, r9
	bl	ue_v
	add	r3, r9, #61440
	ldr	r3, [r3, #1084]
	cmp	r3, #0
	beq	.L1552
.L1543:
	ldr	r1, .L1573+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1572:
	ldr	r1, .L1573
	mov	r0, r9
	bl	ue_v
	ldr	r2, [r4, #8]
	cmp	r2, #2
	str	r0, [r4, #12]
	beq	.L1568
.L1539:
	cmp	r2, #3
	cmpne	r2, #6
	beq	.L1569
.L1540:
	cmp	r2, #4
	beq	.L1570
.L1541:
	cmp	r2, #6
	bhi	.L1543
	add	r3, r9, #61440
	ldr	r3, [r3, #1084]
	cmp	r3, #0
	bne	.L1543
	cmp	r2, #0
	add	r10, r10, #1
	beq	.L1553
	cmp	r10, #100
	beq	.L1571
.L1536:
	ldr	r1, .L1573+4
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
	beq	.L1572
	cmp	r2, #2
	bne	.L1539
.L1568:
	ldr	r1, .L1573+8
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
	bne	.L1540
.L1569:
	ldr	r1, .L1573+12
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
	bne	.L1541
.L1570:
	ldr	r1, .L1573+16
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
	b	.L1541
.L1553:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1571:
	ldr	r1, .L1573+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1554:
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1574:
	.align	2
.L1573:
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
	beq	.L1576
	clz	r4, r7
	mov	r4, r4, lsr #5
	str	r4, [r6, #1812]
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1594
.L1577:
	ldr	r1, .L1599
	mov	r0, r5
	bl	u_1
	movw	r1, #2004
	mla	r4, r1, r4, r5
	ldr	r1, .L1599+4
	add	r4, r4, #22272
	add	r4, r4, #104
	mov	r3, r0
	uxtb	r2, r0
	strb	r3, [r4, #7]
	mov	r0, #16
	bl	dprint_vfmw
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1579
.L1583:
	mov	r8, #0
.L1591:
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1576:
	movw	r4, #2004
	mla	r4, r4, r7, r0
	add	r4, r4, #22272
	add	r4, r4, #104
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1595
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	mov	r4, r7
	cmp	r3, #5
	bne	.L1577
.L1594:
	ldr	r1, .L1599+8
	mov	r0, r5
	bl	u_1
	movw	r3, #2004
	mla	r4, r3, r4, r5
	ldr	r1, .L1599+12
	mov	r8, #0
	add	r4, r4, #22272
	strb	r0, [r4, #109]
	mov	r0, r5
	bl	u_1
	ldrb	r2, [r4, #109]	@ zero_extendqisi2
	ldr	r1, .L1599+16
	strb	r0, [r4, #110]
	mov	r0, #16
	bl	dprint_vfmw
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1595:
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1596
	ldr	r1, .L1599
	ldrb	r6, [r4, #7]	@ zero_extendqisi2
	bl	u_1
	cmp	r6, r0
	bne	.L1597
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1583
	mov	r1, r8
	mov	r0, r5
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	DecMMCO
.L1579:
	mov	r0, r5
	mov	r1, #1
	bl	DecMMCO
	cmn	r0, #1
	mov	r8, r0
	movne	r3, #1
	movne	r8, #0
	strneb	r3, [r4, #4]
	bne	.L1591
	mov	r3, #0
	ldr	r1, .L1599+20
	strb	r3, [r4, #4]
	mov	r0, #1
	str	r7, [r6, #1812]
	bl	dprint_vfmw
	b	.L1591
.L1596:
	ldr	r1, .L1599+8
	ldrb	r6, [r4, #5]	@ zero_extendqisi2
	bl	u_1
	cmp	r6, r0
	bne	.L1598
	mov	r0, r5
	ldr	r1, .L1599+12
	ldrb	r4, [r4, #6]	@ zero_extendqisi2
	bl	u_1
	cmp	r4, r0
	beq	.L1583
	ldr	r1, .L1599+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1591
.L1597:
	add	r5, r5, #61440
	ldr	r1, .L1599+28
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	ldr	ip, [r5, #1088]
	ldr	r3, [r5, #1124]
	mov	r0, #1
	ldr	r2, [r5, #1108]
	ldr	r1, .L1599+32
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L1591
.L1598:
	ldr	r1, .L1599+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1591
.L1600:
	.align	2
.L1599:
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
	ldr	r1, .L1675
	add	r5, r0, #12288
	mov	r6, r0
	bl	ue_v
	ldr	r3, [r5, #1292]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r10, r0
	bhi	.L1602
	add	r9, r6, #61440
	ldr	r3, [r9, #1084]
	cmp	r3, #0
	bne	.L1602
	mov	r1, r0
	mov	r0, r6
	bl	PPSSPSCheckTmpId
	subs	r8, r0, #0
	bne	.L1666
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
	beq	.L1606
	ldr	r3, [r5, #3244]
	cmp	r3, #0
	beq	.L1606
.L1607:
	ldr	r1, [r5, #2196]
	mov	r0, r6
	ldr	r2, .L1675+4
	add	r1, r1, #4
	bl	u_v
	str	r0, [r4, #228]
	ldr	r3, [r9, #1084]
	cmp	r3, #0
	bne	.L1664
	ldrb	r3, [r4, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1667
.L1610:
	mov	r3, #0
	strb	r3, [r4, #209]
	strb	r3, [r4, #210]
	ldrb	r3, [r5, #18]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1668
	ldr	r2, [r5, #3248]
	ldr	r1, [r5, #3252]
	add	r3, r2, #1
	mla	r3, r1, r3, r3
.L1634:
	ldrb	r2, [r5, #19]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1617
	strb	r2, [r4, #212]
.L1619:
	ldr	r2, [r4, #260]
	cmp	r2, r3
	bcs	.L1669
.L1620:
	ldrb	r3, [r4, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1670
.L1622:
	add	r10, r6, #16384
	mov	r3, #0
	str	r3, [r10, #3420]
	str	r3, [r10, #3424]
	ldr	r3, [r5, #2200]
	cmp	r3, #0
	beq	.L1671
.L1626:
	mov	r3, #0
	str	r3, [r10, #3468]
	str	r3, [r10, #3472]
	ldr	r3, [r5, #2200]
	cmp	r3, #1
	beq	.L1672
.L1629:
	ldr	r3, [r9, #1084]
	cmp	r3, #0
	bne	.L1664
	ldrb	r3, [r7, #17]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1632
	ldr	r1, .L1675+8
	mov	r0, r6
	bl	ue_v
	cmp	r0, #0
	bne	.L1633
	ldr	r3, [r9, #1084]
	cmp	r3, #0
	bne	.L1633
.L1632:
	mov	r0, r6
	bl	IsNewPic
	uxtb	r0, r0
	strb	r0, [r4, #213]
	cmp	r0, #0
	bne	.L1646
	ldr	r2, [r4, #260]
	ldr	r3, [r4, #1896]
	cmp	r2, r3
	bls	.L1673
.L1646:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1606:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1607
	ldr	r1, .L1675+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1646
.L1617:
	mov	r2, #1
	strb	r2, [r4, #212]
.L1637:
	ldr	r2, [r4, #260]
	cmp	r2, r3, lsr #1
	bcc	.L1620
	ldr	r1, .L1675+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1646
.L1633:
	ldr	r1, .L1675+20
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L1675+24
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1664
	mov	r3, #0
	ldr	r0, [r9, #1144]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L1664:
	mvn	r8, #0
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1668:
	ldr	r1, .L1675+28
	mov	r0, r6
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #209]
	cmp	r0, #0
	bne	.L1674
	ldr	r2, [r5, #3248]
	ldrb	ip, [r5, #18]	@ zero_extendqisi2
	ldr	r1, [r5, #3252]
	add	r3, r2, #1
	cmp	ip, #0
	mla	r3, r1, r3, r3
	bne	.L1636
	mov	r3, r3, asl #1
	b	.L1634
.L1671:
	ldr	r1, [r5, #2204]
	mov	r0, r6
	ldr	r2, .L1675+32
	add	r1, r1, #4
	bl	u_v
	str	r0, [r10, #3420]
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1626
	ldrb	r3, [r4, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1626
	ldr	r1, .L1675+36
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3424]
	b	.L1626
.L1667:
	cmp	r0, #0
	str	r0, [r4, #224]
	beq	.L1610
	ldr	r1, .L1675+40
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1610
.L1672:
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1629
	ldr	r1, .L1675+44
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3468]
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1629
	ldrb	r3, [r4, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1629
	ldr	r1, .L1675+48
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3472]
	b	.L1629
.L1674:
	ldr	r1, .L1675+52
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
	beq	.L1615
.L1636:
	mov	r2, #1
.L1615:
	cmp	r0, #0
	mul	r3, r2, r3
	beq	.L1634
	cmp	r0, #1
	mov	r2, #0
	strb	r2, [r4, #212]
	bne	.L1619
	b	.L1637
.L1670:
	ldr	r1, .L1675+56
	mov	r0, r6
	bl	ue_v
	cmp	r0, #65536
	str	r0, [r4, #248]
	bcs	.L1623
	ldr	r3, [r9, #1084]
	cmp	r3, #0
	beq	.L1622
.L1623:
	ldr	r1, .L1675+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1622
.L1602:
	mov	r2, r10
	ldr	r1, .L1675+64
	mvn	r8, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1666:
	ldr	r1, .L1675+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1646
.L1669:
	ldr	r1, .L1675+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1646
.L1673:
	ldr	r1, .L1675+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1646
.L1676:
	.align	2
.L1675:
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
	bcc	.L1678
	add	r8, r0, #61440
	ldr	r3, [r8, #1084]
	cmp	r3, #0
	bne	.L1678
	bl	PPSSPSCheck
	cmp	r0, #0
	bne	.L1760
	ldr	r2, [r4, #220]
	mov	r7, #2208
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	ldr	r9, [r6, #1304]
	mul	r7, r7, r2
	cmp	r3, #1
	add	r6, r9, r7
	beq	.L1761
.L1682:
	ldr	r2, [r6, #36]
	cmp	r3, #1
	str	r2, [r4, #264]
	ldr	r2, [r6, #40]
	str	r2, [r4, #268]
	bls	.L1762
	cmp	r3, #2
	beq	.L1709
.L1693:
	mov	r3, #32
	str	r3, [r4, #268]
.L1694:
	mov	r0, r5
	bl	ref_pic_list_reordering
	subs	r10, r0, #0
	bne	.L1763
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1696
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r2, #1
	moveq	r3, r2
	beq	.L1697
.L1696:
	ldr	r3, [r6, #44]
	cmp	r3, #1
	beq	.L1764
.L1713:
	mov	r2, #0
	mov	r3, r2
.L1697:
	strb	r3, [r4, #215]
	mov	r0, #19
	ldr	r1, .L1770
	bl	dprint_vfmw
	ldrb	r2, [r4, #215]	@ zero_extendqisi2
	mov	r3, #0
	str	r3, [r4, #272]
	cmp	r2, r3
	str	r3, [r4, #276]
	bne	.L1765
.L1698:
	ldr	r3, [r4, #240]
	cmp	r3, #0
	bne	.L1766
.L1699:
	mov	r3, #0
	str	r3, [r4, #1816]
	ldrb	r3, [r9, r7]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1700
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L1767
.L1700:
	ldr	r1, .L1770+4
	mov	r0, r5
	bl	se_v
	mov	r3, #0
	str	r3, [r4, #1824]
	str	r3, [r4, #1828]
	str	r3, [r4, #1832]
	str	r0, [r4, #1820]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1768
.L1729:
	mov	r0, r10
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1709:
	mov	r3, #32
	str	r3, [r4, #264]
	b	.L1693
.L1768:
	ldr	r1, .L1770+8
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #1824]
	bhi	.L1703
	ldr	r3, [r8, #1084]
	cmp	r3, #0
	bne	.L1703
	cmp	r0, #1
	beq	.L1729
	ldr	r1, .L1770+12
	mov	r0, r5
	bl	se_v
	add	r3, r0, #6
	str	r0, [r4, #1828]
	cmp	r3, #12
	bhi	.L1706
	ldr	r3, [r8, #1084]
	cmp	r3, #0
	bne	.L1706
	mov	r0, r5
	ldr	r1, .L1770+16
	bl	se_v
	add	r3, r0, #6
	str	r0, [r4, #1832]
	cmp	r3, #12
	bhi	.L1708
	ldr	r3, [r8, #1084]
	cmp	r3, #0
	beq	.L1729
.L1708:
	ldr	r1, .L1770+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1765:
	mov	r0, r5
	bl	pred_weight_table
	cmp	r0, #0
	beq	.L1698
	ldr	r1, .L1770+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1766:
	mov	r0, r5
	bl	dec_ref_pic_marking
	cmp	r0, #0
	beq	.L1699
	ldr	r1, .L1770+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1764:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r2, r3
	bne	.L1713
	b	.L1697
.L1761:
	ldr	r1, .L1770+32
	mov	r0, r5
	bl	u_1
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	strb	r0, [r4, #214]
	b	.L1682
.L1762:
	ldr	r1, .L1770+36
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1684
	ldr	r3, [r4, #264]
.L1685:
	ldrb	r2, [r4, #209]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1686
	cmp	r3, #15
	bhi	.L1687
	ldr	r3, [r4, #268]
	cmp	r3, #15
	bhi	.L1769
.L1688:
	ldr	r3, [r8, #1084]
	cmp	r3, #0
	bne	.L1687
.L1689:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1709
	cmp	r3, #1
	bne	.L1693
	b	.L1694
.L1678:
	ldr	r1, .L1770+40
	mvn	r10, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r10
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1767:
	ldr	r1, .L1770+44
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #1816]
	bhi	.L1701
	ldr	r3, [r8, #1084]
	cmp	r3, #0
	beq	.L1700
.L1701:
	ldr	r1, .L1770+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1686:
	cmp	r3, #31
	bhi	.L1690
	ldr	r3, [r4, #268]
	cmp	r3, #31
	bls	.L1691
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1691
.L1690:
	ldr	r1, .L1770+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1691:
	ldr	r3, [r8, #1084]
	cmp	r3, #0
	beq	.L1689
	b	.L1690
.L1769:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1688
.L1687:
	ldr	r1, .L1770+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1684:
	ldr	r1, .L1770+60
	mov	r0, r5
	bl	ue_v
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	cmp	r2, #1
	mov	r3, r0
	str	r0, [r4, #264]
	bne	.L1685
	ldr	r1, .L1770+64
	mov	r0, r5
	bl	ue_v
	ldr	r3, [r4, #264]
	str	r0, [r4, #268]
	b	.L1685
.L1706:
	ldr	r1, .L1770+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1703:
	ldr	r1, .L1770+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1760:
	ldr	r1, .L1770+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1763:
	ldr	r1, .L1770+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r10, #0
	b	.L1729
.L1771:
	.align	2
.L1770:
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
	bne	.L1775
	add	r1, r0, #16384
	ldr	ip, [r1, #3420]
	str	ip, [r3, #1880]
	ldr	r1, [r1, #3424]
	str	r1, [r3, #1884]
	ldr	r1, [r2, #2200]
.L1775:
	cmp	r1, #1
	addeq	r2, r0, #16384
	add	r0, r0, #61440
	ldreq	r1, [r2, #3468]
	streq	r1, [r3, #1888]
	ldreq	r2, [r2, #3472]
	streq	r2, [r3, #1892]
	ldr	r3, [r0, #1100]
	add	r3, r3, #1
	str	r3, [r0, #1100]
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
	beq	.L1782
	bcc	.L1783
	cmp	r3, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r2, #211]	@ zero_extendqisi2
	add	r0, r0, #57344
	cmp	r3, #5
	beq	.L1792
.L1785:
	mov	r3, #0
	str	r3, [r0, #2416]
	ldmfd	sp, {fp, sp, pc}
.L1783:
	add	r0, r0, #57344
	ldr	r3, [r0, #2412]
	cmp	r3, #2
	movne	r3, #1
	strne	r3, [r0, #2412]
	b	.L1785
.L1782:
	add	r0, r0, #57344
	mov	r2, #2
	mov	r3, #0
	str	r2, [r0, #2412]
	str	r3, [r0, #2416]
	ldmfd	sp, {fp, sp, pc}
.L1792:
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
	beq	.L1794
	ldr	r2, [r0, #368]
	ldr	r3, [r2, #64]
	add	r3, r3, #7
	bic	r3, r3, #7
	str	r3, [r2, #64]
	ldr	r2, [r0, #368]
	ldr	r3, [r2, #72]
	sub	r3, r3, #1
	str	r3, [r2, #72]
.L1794:
	ldr	r3, [r4, #368]
	ldrb	r6, [r3]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L1805
	mov	r1, #0
	mov	r0, r3
	mov	r5, r1
.L1796:
	add	r1, r1, #1
	ldr	ip, [r0, #12]
	cmp	r1, r6
	add	r0, r0, #28
	add	r5, r5, ip
	bne	.L1796
.L1795:
	mov	r7, r6, asl #5
	ldr	r0, [r3, #72]
	sub	r7, r7, r6, asl #2
	ldr	r2, [r3, #64]
	add	r1, r3, r7
	mov	r3, r5
	str	r0, [sp, #4]
	mov	r0, #7
	ldr	ip, [r1, #12]
	ldr	r1, .L1816
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #368]
	mov	r2, r6
	mov	r0, #7
	ldr	r1, .L1816+4
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
	bhi	.L1815
	ldrb	r6, [ip]	@ zero_extendqisi2
	add	r1, r6, #1
	cmp	r2, r1
	movhi	r2, #1
	movls	r2, #0
	cmp	r1, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L1798
	add	r6, r6, #2
	mov	r5, #0
.L1799:
	sub	r3, r6, #1
	ldr	r1, .L1816+8
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
	bne	.L1799
	sub	r3, r3, #1
.L1798:
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
	bcs	.L1793
	sxth	r3, r1
	add	r4, r4, #57344
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, ip, r3
.L1803:
	ldr	r2, [r3, #28]
	add	r1, r1, #1
	ldr	r0, [r4, #2440]
	add	r3, r3, #28
	cmp	r2, r0
	strcc	r2, [r4, #2440]
	ldr	r2, [ip, #68]
	cmp	r2, r1
	bhi	.L1803
.L1793:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1815:
	ldr	r1, .L1816+12
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	dprint_vfmw
.L1805:
	mov	r5, r6
	b	.L1795
.L1817:
	.align	2
.L1816:
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
	ble	.L1818
	mov	r0, #8
	mov	r4, #0
	mov	r2, r7
	mov	r9, r6
	ldr	r8, .L1829
	mov	r10, #255
	mov	r7, r1
	mov	r6, r0
	mov	r5, r4
	b	.L1823
.L1822:
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
	beq	.L1818
.L1823:
	cmp	r2, #16
	ldreq	r3, .L1829+4
	ldrneb	r4, [r5, r8]	@ zero_extendqisi2
	ldreqb	r4, [r5, r3]	@ zero_extendqisi2
	cmp	r0, #0
	add	r5, r5, #1
	beq	.L1822
	ldr	r1, .L1829+8
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
	b	.L1822
.L1818:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1830:
	.align	2
.L1829:
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
	ldr	r1, .L1889
	mov	r5, r0
	bl	u_1
	add	r6, r5, #61440
	strb	r0, [r4]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L1880
	ldr	r1, .L1889+4
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #1]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L1880
	ldr	r1, .L1889+8
	mov	r0, r5
	bl	ue_v
	cmp	r0, #0
	str	r0, [r4, #32]
	bne	.L1835
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	beq	.L1836
.L1835:
	ldr	r1, .L1889+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L1889+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1880
	mov	r3, #0
	ldr	r0, [r6, #1144]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L1833:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1843:
	ldr	r1, .L1889+20
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1889+24
	str	r0, [r4, #60]
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #18]
	cmp	r0, #1
	beq	.L1844
.L1862:
	mov	r0, r5
	ldr	r1, .L1889+28
	bl	se_v
	add	r3, r0, #12
	str	r0, [r4, #56]
	cmp	r3, #24
	bhi	.L1845
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	beq	.L1863
.L1845:
	ldr	r1, .L1889+32
	mov	r0, #1
	bl	dprint_vfmw
.L1880:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1836:
	ldr	r1, .L1889+36
	mov	r0, r5
	bl	ue_v
	cmp	r0, #31
	str	r0, [r4, #36]
	bhi	.L1837
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L1837
	ldr	r1, .L1889+40
	mov	r0, r5
	bl	ue_v
	cmp	r0, #31
	str	r0, [r4, #40]
	bhi	.L1839
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L1839
	ldr	r1, .L1889+44
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #2]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L1880
	ldr	r2, .L1889+48
	mov	r1, #2
	mov	r0, r5
	bl	u_v
	cmp	r0, #2
	str	r0, [r4, #44]
	bhi	.L1881
	ldr	r1, .L1889+52
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1889+56
	str	r0, [r4, #48]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1889+60
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1889+64
	str	r0, [r4, #52]
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1889+68
	strb	r0, [r4, #3]
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1889+72
	strb	r0, [r4, #4]
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #17]
	cmp	r0, #0
	bne	.L1882
.L1842:
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
	bcs	.L1843
.L1863:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1882:
	ldr	r1, .L1889+76
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1842
.L1837:
	ldr	r1, .L1889+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1833
.L1839:
	ldr	r1, .L1889+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1833
.L1844:
	add	r3, r4, #4
	str	r6, [fp, #-80]
	sub	r9, fp, #72
	add	r8, r4, #1984
	add	r10, r4, #64
	mov	r6, r3
	b	.L1853
.L1886:
	ldr	lr, [lr, #1300]
	ldr	r3, [r4, #28]
	mla	r3, ip, r3, lr
	ldrb	ip, [r3, #25]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L1883
	ldr	r3, .L1889+88
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L1889+92
	ldr	r3, [r3, #52]
	blx	r3
.L1848:
	add	r7, r7, #1
	add	r8, r8, #16
	cmp	r7, #6
	add	r10, r10, #64
	beq	.L1884
.L1853:
	ldr	r1, .L1889+96
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
	beq	.L1885
	cmp	r7, #0
	cmpne	r7, #3
	beq	.L1886
	ldr	r3, .L1889+92
	mov	r2, #16
	sub	r1, r8, #16
	mov	r0, r8
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1848
.L1884:
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
.L1861:
	ldr	r2, [r6, #60]
	mov	r3, #0
	cmp	r2, #1
	strne	r3, [r8]
	strneb	r3, [r10]
	beq	.L1887
.L1856:
	ldr	r1, [r6, #28]
	mov	lr, #4000
	ldr	r3, [r5, #1300]
	mov	r0, r9
	mov	r2, #64
	mla	r3, lr, r1, r3
	ldrb	r1, [r3, #25]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1888
	ldr	r1, .L1889+100
	cmp	r7, #0
	ldr	r3, .L1889+92
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L1857:
	add	r7, r7, #1
	add	r9, r9, #64
	cmp	r7, #2
	add	r4, r4, #256
	add	r8, r8, #4
	add	r10, r10, #1
	bne	.L1861
	mov	r4, r6
	ldr	r5, [fp, #-80]
	ldr	r6, [fp, #-84]
	b	.L1862
.L1885:
	mov	r3, r2
	str	r9, [sp]
	mov	r2, r8
	mov	r1, r10
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r9]
	cmp	r3, #1
	bne	.L1848
	ldr	r1, .L1889+88
	mov	r2, #16
	ldr	r3, .L1889+92
	cmp	r7, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r8
	blx	r3
	b	.L1848
.L1883:
	add	r1, r3, r7, lsl #4
	ldr	r3, .L1889+92
	add	r1, r1, #1968
	mov	r2, #16
	add	r1, r1, #4
	mov	r0, r8
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1848
.L1881:
	ldr	r1, .L1889+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1833
.L1887:
	ldr	r1, .L1889+108
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
	bne	.L1856
	str	r8, [sp]
	bl	Scaling_List
	ldr	r3, [r8]
	cmp	r3, #1
	bne	.L1857
	ldr	r1, .L1889+100
	mov	r2, #64
	ldr	r3, .L1889+92
	cmp	r7, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r9
	blx	r3
	b	.L1857
.L1888:
	add	r1, r3, r7, lsl #6
	ldr	r3, .L1889+92
	add	r1, r1, #2064
	add	r1, r1, #4
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1857
.L1890:
	.align	2
.L1889:
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
	beq	.L1914
	cmp	r1, #0
	beq	.L1915
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
	beq	.L1895
	mov	ip, ip, asl #1
	adds	r3, ip, #6
	str	r3, [fp, #-48]
	beq	.L1895
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
.L1900:
	ldrb	r3, [r6, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r3
	cmp	ip, r1
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, #0
	beq	.L1913
	cmp	r0, #5
	bhi	.L1897
	add	r4, r10, #64
	add	r10, r10, #128
	mov	r1, r9
	mov	r3, r4
.L1898:
	ldr	lr, [r3], #4
	ldr	ip, [r1], #4
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, r10
	bne	.L1898
.L1896:
	ldr	r3, [fp, #-48]
	add	r0, r0, #1
	add	r7, r7, #256
	add	r8, r8, #256
	cmp	r0, r3
	mov	r10, r4
	add	r9, r9, #64
	bne	.L1900
	ldr	r6, [fp, #-52]
	ldr	r1, [fp, #-56]
.L1895:
	ldr	r0, [r6, #56]
	ldr	r3, [r1, #56]
	cmp	r0, r3
	movne	r0, #0
	andeq	r0, r2, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L1893:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1897:
	sub	r3, r7, #1088
	sub	r4, r7, #836
	sub	r3, r3, #4
	mov	r1, r8
.L1899:
	ldr	lr, [r3, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, r4
	bne	.L1899
.L1913:
	add	r4, r10, #64
	b	.L1896
.L1915:
	mov	r0, r1
	movw	r3, #9727
	ldr	r2, .L1916
	ldr	r1, .L1916+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1893
.L1914:
	movw	r3, #9726
	ldr	r2, .L1916
	ldr	r1, .L1916+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1893
.L1917:
	.align	2
.L1916:
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
	ldr	r1, .L1935
	add	r4, r0, #12288
	mov	r5, r0
	bl	ue_v
	ldr	r3, [r4, #1292]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r6, r0
	bls	.L1919
	ldr	r1, .L1935+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #1292]
	sub	r2, r3, #1
	cmp	r6, r2
	bls	.L1927
	ldr	r2, .L1935+8
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L1927
	add	r5, r5, #61440
	str	r3, [fp, #-40]
	str	r6, [fp, #-44]
	mov	r3, #8
	ldr	r0, [r5, #1144]
	sub	r2, fp, #44
	mov	r1, #110
	blx	ip
	mvn	r0, #0
.L1920:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1919:
	ldr	r1, .L1935+12
	mov	r0, r5
	bl	ue_v
	ldr	r3, [r4, #1288]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	ip, r0
	bhi	.L1931
	ldr	r2, [r4, #1300]
	mov	r3, #4000
	mla	r3, r3, r0, r2
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1932
	mov	r9, #2208
	ldr	r3, [r4, #1304]
	mul	r7, r9, r6
	add	r3, r3, r7
	ldrb	r8, [r3, #19]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L1923
	ldr	r8, .L1935+16
	mov	r0, r5
	mov	r1, r8
	str	r6, [r8, #24]
	str	ip, [r8, #28]
	bl	ProcessPPS
	subs	r5, r0, #0
	bne	.L1933
	ldr	r1, [r4, #1304]
	mov	r0, r8
	add	r1, r1, r7
	bl	PPSEqual
	cmp	r0, #0
	beq	.L1920
	ldr	ip, .L1935+20
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
	b	.L1920
.L1923:
	str	r6, [r3, #24]
	mov	r0, r5
	ldr	r3, [r4, #1304]
	add	r3, r3, r7
	str	ip, [r3, #28]
	ldr	r1, [r4, #1304]
	add	r1, r1, r7
	bl	ProcessPPS
	cmp	r0, #0
	bne	.L1934
	ldr	r3, [r4, #1304]
	mov	r2, #1
	add	r3, r3, r7
	strb	r2, [r3, #20]
	ldr	r3, [r4, #1304]
	add	r7, r3, r7
	strb	r2, [r7, #19]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1927:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1931:
	ldr	r1, .L1935+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1920
.L1933:
	ldr	r1, .L1935+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1920
.L1932:
	ldr	r1, .L1935+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1920
.L1934:
	ldr	r1, .L1935+28
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
	b	.L1920
.L1936:
	.align	2
.L1935:
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
	ldr	r1, .L1986
	mov	r4, r0
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	streqb	r0, [r5, #15]
	bne	.L1980
.L1939:
	ldr	r1, .L1986+4
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #1]
	cmp	r0, #0
	bne	.L1981
.L1940:
	ldr	r1, .L1986+8
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #3]
	cmp	r0, #0
	moveq	r3, #5
	streq	r3, [r5, #28]
	bne	.L1982
.L1942:
	ldr	r1, .L1986+12
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #6]
	cmp	r0, #0
	bne	.L1983
.L1943:
	ldr	r1, .L1986+16
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #8]
	cmp	r0, #0
	bne	.L1984
.L1945:
	ldr	r1, .L1986+20
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #7]
	cmp	r0, #0
	beq	.L1947
	ldr	r1, .L1986+24
	mov	r0, r4
	bl	ue_v
	ldr	r2, .L1986+28
	mov	r1, #4
	str	r0, [r5, #120]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+32
	mov	r1, #4
	strb	r0, [r5, #116]
	mov	r0, r4
	bl	u_v
	ldr	r3, [r5, #120]
	cmp	r3, #31
	strb	r0, [r5, #117]
	bhi	.L1950
	add	r9, r4, #61440
	ldr	r6, [r9, #1084]
	cmp	r6, #0
	bne	.L1950
	add	r7, r5, #120
	add	r8, r5, #83
.L1951:
	ldr	r1, .L1986+36
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #120]
	cmp	r3, r6
	bcs	.L1951
	ldr	r2, .L1986+48
	mov	r1, #5
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+52
	mov	r1, #5
	str	r0, [r5, #380]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+56
	mov	r1, #5
	str	r0, [r5, #384]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+60
	mov	r1, #5
	str	r0, [r5, #388]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #392]
.L1947:
	ldr	r1, .L1986+64
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #10]
	cmp	r0, #0
	bne	.L1985
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	add	r9, r4, #61440
	cmp	r3, #0
	bne	.L1956
.L1955:
	ldr	r1, .L1986+68
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1986+72
	strb	r0, [r5, #11]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #12]
	cmp	r0, #0
	bne	.L1957
	ldr	r0, [r9, #1084]
.L1958:
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1985:
	ldr	r1, .L1986+24
	mov	r0, r4
	bl	ue_v
	ldr	r2, .L1986+28
	mov	r1, #4
	str	r0, [r5, #432]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+32
	mov	r1, #4
	strb	r0, [r5, #428]
	mov	r0, r4
	bl	u_v
	ldr	r3, [r5, #432]
	cmp	r3, #31
	strb	r0, [r5, #429]
	bhi	.L1950
	add	r9, r4, #61440
	ldr	r6, [r9, #1084]
	cmp	r6, #0
	bne	.L1950
	add	r8, r5, #392
	add	r7, r5, #432
	add	r8, r8, #3
.L1953:
	ldr	r1, .L1986+36
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #432]
	cmp	r3, r6
	bcs	.L1953
	ldr	r2, .L1986+48
	mov	r1, #5
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+52
	mov	r1, #5
	str	r0, [r5, #692]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+56
	mov	r1, #5
	str	r0, [r5, #696]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+60
	mov	r1, #5
	str	r0, [r5, #700]
	mov	r0, r4
	bl	u_v
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r5, #704]
	bne	.L1956
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1955
.L1956:
	ldr	r1, .L1986+76
	mov	r0, r4
	bl	u_1
	strb	r0, [r5, #14]
	b	.L1955
.L1984:
	ldr	r2, .L1986+80
	mov	r1, #32
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+84
	mov	r1, #32
	str	r0, [r5, #52]
	mov	r0, r4
	bl	u_v
	ldr	r1, .L1986+88
	str	r0, [r5, #56]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #9]
	cmp	r0, #1
	bne	.L1945
	ldr	r1, [r5, #52]
	cmp	r1, #0
	beq	.L1945
	ldr	r0, [r5, #56]
	bl	__aeabi_uidiv
	add	r3, r4, #57344
	mov	r0, r0, lsr #1
	str	r0, [r3, #3844]
	b	.L1945
.L1983:
	ldr	r1, .L1986+92
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+96
	str	r0, [r5, #44]
	mov	r0, r4
	bl	ue_v
	str	r0, [r5, #48]
	b	.L1943
.L1982:
	ldr	r2, .L1986+100
	mov	r1, #3
	mov	r0, r4
	bl	u_v
	ldr	r1, .L1986+104
	str	r0, [r5, #28]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1986+108
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #5]
	cmp	r0, #0
	beq	.L1942
	ldr	r2, .L1986+112
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+116
	mov	r1, #8
	str	r0, [r5, #32]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+120
	mov	r1, #8
	str	r0, [r5, #36]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #40]
	b	.L1942
.L1981:
	ldr	r1, .L1986+124
	mov	r0, r4
	bl	u_1
	strb	r0, [r5, #2]
	b	.L1940
.L1980:
	ldr	r2, .L1986+128
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	uxtb	r0, r0
	strb	r0, [r5, #15]
	cmp	r0, #255
	bne	.L1939
	ldr	r2, .L1986+132
	mov	r1, #16
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1986+136
	mov	r1, #16
	str	r0, [r5, #16]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #20]
	b	.L1939
.L1957:
	ldr	r1, .L1986+140
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1986+144
	strb	r0, [r5, #13]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+148
	str	r0, [r5, #60]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+152
	str	r0, [r5, #64]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+156
	str	r0, [r5, #72]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+160
	str	r0, [r5, #68]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1986+164
	str	r0, [r5, #76]
	mov	r0, r4
	bl	ue_v
	str	r0, [r5, #80]
	ldr	r0, [r9, #1084]
	cmp	r0, #0
	beq	.L1958
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1950:
	ldr	r1, .L1986+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1987:
	.align	2
.L1986:
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
	beq	.L2020
	cmp	r1, #0
	beq	.L2021
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
	beq	.L1992
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
.L1997:
	ldrb	r2, [r4, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r2
	cmp	ip, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, #0
	beq	.L1993
	cmp	r0, #5
	bhi	.L1994
	add	r2, r6, #48
	add	r10, r6, #112
	mov	r1, r7
.L1995:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L1995
.L1993:
	add	r0, r0, #1
	add	r8, r8, #256
	cmp	r0, #8
	add	r9, r9, #256
	add	r6, r6, #64
	add	r7, r7, #64
	bne	.L1997
	ldr	r5, [fp, #-48]
	ldr	r1, [fp, #-52]
.L1992:
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
	beq	.L1999
	cmp	ip, #1
	bne	.L1998
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
	beq	.L1998
	add	ip, r5, #2208
	add	r0, r1, #2208
	add	ip, ip, #8
	add	r0, r0, #8
	mov	r2, #0
.L2001:
	ldr	r4, [ip, #4]!
	add	r2, r2, #1
	ldr	lr, [r0, #4]!
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r6
	bne	.L2001
.L1998:
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
	bne	.L2002
	ldrb	r0, [r5, #19]	@ zero_extendqisi2
	ldrb	r2, [r1, #19]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
.L2002:
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
	bne	.L2022
.L2003:
	ldrb	r0, [r5, #22]	@ zero_extendqisi2
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L1990:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2022:
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
	b	.L2003
.L1994:
	sub	r2, r8, #1104
	sub	r10, r8, #848
	mov	r1, r9
.L1996:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L1996
	b	.L1993
.L1999:
	ldr	r0, [r5, #2204]
	ldr	r2, [r1, #2204]
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
	b	.L1998
.L2021:
	mov	r0, r1
	movw	r3, #9960
	ldr	r2, .L2023
	ldr	r1, .L2023+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1990
.L2020:
	movw	r3, #9959
	ldr	r2, .L2023
	ldr	r1, .L2023+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1990
.L2024:
	.align	2
.L2023:
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
	beq	.L2048
	subs	r1, r4, #255
	movne	r1, #1
	cmp	r4, #16
	movle	r0, #0
	andgt	r0, r1, #1
	cmp	r0, #0
	bne	.L2037
	cmp	r2, #0
	cmpne	lr, #0
	moveq	ip, #1
	movne	ip, #0
	cmp	r4, #255
	movne	ip, #0
	andeq	ip, ip, #1
	cmp	ip, #0
	bne	.L2038
	cmp	r4, #1
	moveq	r2, ip
	moveq	r4, #5
	beq	.L2026
	cmp	r1, #0
	str	r3, [fp, #-24]
	ldrne	r0, .L2049
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
	bgt	.L2030
	rsb	r1, r2, #2256
	add	r1, r1, #7
	cmp	r3, r1
	bge	.L2031
.L2041:
	mov	r4, #4
	b	.L2026
.L2037:
	mov	r4, #0
.L2048:
	mov	r2, r4
.L2026:
	mov	r3, r4
	ldr	r1, .L2049+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L2030:
	sub	r1, r2, #2256
	sub	r1, r1, #7
	cmp	r3, r1
	blt	.L2041
.L2031:
	movw	r3, #1819
	cmp	r2, r3
	bgt	.L2032
	rsb	r3, r2, #1808
	add	r3, r3, #12
	cmp	r1, r3
	blt	.L2043
.L2033:
	movw	r1, #1364
	cmp	r2, r1
	rsble	r4, r2, #1360
	subgt	r4, r2, #1360
	addle	r4, r4, #5
	subgt	r4, r4, #5
	cmp	r3, r4
	movlt	r4, #2
	movge	r4, #1
	b	.L2026
.L2038:
	mov	r4, r0
	mov	r2, r0
	b	.L2026
.L2032:
	sub	r3, r2, #1808
	sub	r3, r3, #12
	cmp	r1, r3
	bge	.L2033
.L2043:
	mov	r4, #3
	b	.L2026
.L2050:
	.align	2
.L2049:
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
	beq	.L2165
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
	bne	.L2166
.L2066:
	ldr	r1, .L2182
	mov	r0, r5
	bl	ue_v
	cmp	r0, #12
	str	r0, [r4, #2196]
	bhi	.L2079
	add	r6, r5, #61440
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2079
	ldr	r1, .L2182+4
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #2200]
	bhi	.L2081
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2081
	cmp	r0, #0
	beq	.L2167
	cmp	r0, #1
	beq	.L2168
.L2087:
	ldr	r1, .L2182+8
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #3244]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	ldr	r1, .L2182+12
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #17]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	ldr	r1, .L2182+16
	mov	r0, r5
	bl	ue_v
	movw	r8, #509
	sub	r3, r0, #1
	str	r0, [r4, #3248]
	cmp	r3, r8
	bhi	.L2092
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2092
	ldr	r1, .L2182+20
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #3252]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	ldr	r1, .L2182+24
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #18]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	cmp	r0, #0
	bne	.L2169
	ldr	r1, .L2182+28
	mov	r0, r5
	bl	u_1
	ldr	r7, [r4, #3252]
	add	r7, r7, #1
	cmp	r7, #255
	strb	r0, [r4, #19]
	bhi	.L2124
	ldr	r3, [r4, #3248]
	add	r3, r3, #1
	cmp	r7, #1
	cmphi	r3, #3
	movhi	r9, #2
	bhi	.L2096
.L2124:
	ldr	r1, .L2182+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2166:
	ldr	r1, .L2182+36
	bl	ue_v
	cmp	r0, #1
	str	r0, [r4, #40]
	bhi	.L2055
	add	r6, r5, #61440
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2055
	ldr	r1, .L2182+40
	mov	r0, r5
	bl	ue_v
	uxth	r0, r0
	add	r2, r0, #8
	str	r2, [r4, #44]
	cmp	r2, #8
	beq	.L2170
.L2057:
	ldr	r1, .L2182+44
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2182+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2058
	ldr	r0, [r4, #44]
	mov	r2, #0
	mov	r3, #8
	str	r2, [fp, #-68]
	mov	r1, #119
	sub	r2, fp, #68
	str	r0, [fp, #-64]
	ldr	r0, [r6, #1144]
	blx	ip
.L2058:
	ldr	r1, .L2182+52
	mov	r0, r5
	bl	ue_v
	uxth	r0, r0
	add	r2, r0, #8
	str	r2, [r4, #48]
	cmp	r2, #8
	beq	.L2171
.L2060:
	ldr	r1, .L2182+56
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2182+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2061
	ldr	r0, [r4, #48]
	mov	r2, #0
	mov	r3, #8
	str	r2, [fp, #-68]
	mov	r1, #119
	sub	r2, fp, #68
	str	r0, [fp, #-64]
	ldr	r0, [r6, #1144]
	blx	ip
.L2061:
	ldr	r1, .L2182+60
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L2063
	ldr	r6, [r6, #1084]
	cmp	r6, #0
	bne	.L2063
	ldr	r1, .L2182+64
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #25]
	cmp	r0, #1
	bne	.L2066
	add	r10, r4, #1968
	sub	r7, fp, #72
	add	r10, r10, #4
	add	r9, r4, #3
	add	r8, r4, #52
	b	.L2072
.L2174:
	ldr	r3, .L2182+68
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2182+72
	ldr	r3, [r3, #52]
	blx	r3
.L2068:
	add	r6, r6, #1
	add	r10, r10, #16
	cmp	r6, #6
	add	r8, r8, #64
	beq	.L2172
.L2072:
	ldr	r1, .L2182+76
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
	beq	.L2173
	cmp	r6, #0
	cmpne	r6, #3
	beq	.L2174
	ldr	r3, .L2182+72
	mov	r2, #16
	sub	r1, r10, #16
	mov	r0, r10
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2068
.L2171:
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	beq	.L2061
	b	.L2060
.L2170:
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	beq	.L2058
	b	.L2057
.L2123:
	cmp	r1, r10
	cmpcs	r0, r7
	bcs	.L2122
	ldr	r3, .L2182+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2163
	strh	r1, [fp, #-72]	@ movhi
	mov	r3, #8
	strh	r0, [fp, #-70]	@ movhi
	sub	r2, fp, #76
	strh	r10, [fp, #-76]	@ movhi
	mov	r1, #107
	strh	r7, [fp, #-74]	@ movhi
	ldr	r0, [r6, #1144]
	blx	ip
.L2163:
	mvn	r0, #0
.L2156:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2169:
	ldr	r7, [r4, #3252]
	strb	r3, [r4, #19]
	sub	r3, r7, #1
	cmp	r3, r8
	bhi	.L2124
	ldr	r3, [r4, #3248]
	mov	r9, #1
	add	r7, r7, #1
	add	r3, r3, r9
.L2096:
	mul	r7, r7, r9
	mov	r10, r3, asl #4
	mov	r7, r7, asl #4
	mul	r1, r10, r7
	add	r1, r1, r1, lsr #1
	cmp	r1, #100663296
	bhi	.L2175
	ldr	r2, [r4, #32]
	sub	r3, r2, #10
	cmp	r3, #31
	ldrls	pc, [pc, r3, asl #2]
	b	.L2098
.L2100:
	.word	.L2099
	.word	.L2101
	.word	.L2131
	.word	.L2131
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2131
	.word	.L2103
	.word	.L2105
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2105
	.word	.L2106
	.word	.L2107
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2098
	.word	.L2109
	.word	.L2109
.L2167:
	ldr	r1, .L2182+80
	mov	r0, r5
	bl	ue_v
	cmp	r0, #12
	str	r0, [r4, #2204]
	bhi	.L2084
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	beq	.L2087
.L2084:
	ldr	r1, .L2182+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2131:
	mov	r0, #60416
	movt	r0, 13
.L2102:
	bl	__aeabi_uidiv
	ldr	r1, .L2182+88
	cmp	r0, #16
	movcc	r8, r0
	movcs	r8, #16
	mov	r2, r8
	mov	r0, #21
	bl	dprint_vfmw
	ldr	r2, [r4, #3244]
	cmp	r2, r8
	bhi	.L2110
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	beq	.L2111
.L2110:
	mov	r3, r8
	ldr	r1, .L2182+92
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r4, #3244]
	cmp	r3, #16
	bhi	.L2160
	cmp	r8, r3
	movcc	r8, r3
.L2160:
	str	r8, [r4, #3244]
.L2111:
	ldr	r1, .L2182+96
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #20]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	ldr	r1, .L2182+100
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #21]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	cmp	r0, #0
	bne	.L2176
.L2113:
	mov	r2, r10, lsr #1
	mov	r3, r7, lsr #1
	str	r10, [r4, #3276]
	str	r7, [r4, #3280]
	str	r2, [r4, #3284]
	str	r3, [r4, #3288]
.L2115:
	ldr	r1, .L2182+104
	mov	r0, r5
	bl	u_1
	uxtb	r3, r0
	strb	r3, [r4, #22]
	ldr	r2, [r6, #1084]
	cmp	r2, #0
	bne	.L2163
	cmp	r3, #0
	mov	r2, #2
	str	r2, [r4, #3332]
	bne	.L2177
	mov	r0, r3
	strb	r3, [r4, #3307]
	mov	r3, #5
	str	r3, [r4, #3320]
.L2118:
	ldr	r2, [r4, #3312]
	mov	r3, r10
	ldr	r1, [r4, #3308]
	str	r7, [sp]
	bl	H264GetDar
	ldr	r3, [r4, #3244]
	cmp	r3, #0
	str	r0, [r4, #3316]
	beq	.L2121
	cmp	r8, r3
	movcs	r8, r3
.L2121:
	cmp	r8, #16
	add	r3, r5, #12288
	movcs	r8, #16
	str	r8, [r3, #1296]
	ldr	r3, [r5]
	ldr	r2, [r3, #28]
	cmp	r2, #25
	beq	.L2178
.L2122:
	add	r8, r8, #1
	mov	r0, #0
	str	r8, [r4, #3272]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2109:
	mov	r0, #12582912
	b	.L2102
.L2105:
	mov	r0, #30208
	movt	r0, 47
	b	.L2102
.L2172:
	add	r6, r4, #2064
	sub	r7, fp, #88
	add	r6, r6, #4
	add	r9, r4, #9
	add	r8, r4, #436
	mov	r10, #0
.L2077:
	ldr	r1, .L2182+76
	mov	r0, r5
	bl	u_1
	mov	r2, #64
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r6
	str	r3, [r7, #4]!
	strb	r3, [r9, #1]!
	beq	.L2179
	ldr	r1, .L2182+108
	cmp	r10, #0
	ldr	r3, .L2182+72
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2074:
	add	r10, r10, #1
	add	r6, r6, #64
	cmp	r10, #2
	add	r8, r8, #256
	bne	.L2077
	b	.L2066
.L2173:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r10
	mov	r1, r8
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2068
	ldr	r1, .L2182+68
	mov	r2, #16
	ldr	r3, .L2182+72
	cmp	r6, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r10
	blx	r3
	b	.L2068
.L2055:
	ldr	r1, .L2182+112
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2079:
	ldr	r1, .L2182+116
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2176:
	ldr	r1, .L2182+120
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2182+124
	str	r0, [r4, #3256]
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2182+128
	str	r0, [r4, #3260]
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2182+132
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
	ble	.L2113
	add	r1, r1, r3, lsr #1
	add	r9, r9, r0, lsr #1
	str	r1, [r4, #3284]
	str	r9, [r4, #3288]
	str	r3, [r4, #3276]
	str	r0, [r4, #3280]
	b	.L2115
.L2177:
	add	r1, r4, #3280
	mov	r0, r5
	add	r1, r1, #12
	bl	vui_parameters
	cmp	r0, #0
	bne	.L2162
	ldrb	r3, [r4, #3304]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2162
	ldr	r2, [r4, #3372]
	clz	r3, r2
	mov	r3, r3, lsr #5
	cmp	r8, r2
	orrcc	r3, r3, #1
	cmp	r3, #0
	bne	.L2180
	ldr	r3, [r4, #3244]
	ldrb	r0, [r4, #3307]	@ zero_extendqisi2
	cmp	r2, r3
	movcs	r8, r2
	movcc	r8, r3
	b	.L2118
.L2180:
	mov	r3, r8
	ldr	r1, .L2182+136
	mov	r0, #1
	bl	dprint_vfmw
.L2162:
	ldrb	r0, [r4, #3307]	@ zero_extendqisi2
	b	.L2118
.L2178:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L2122
	ldr	r2, [r3, #692]
	ldr	r1, [r3, #668]
	cmp	r8, r2
	ldr	r0, [r3, #672]
	bls	.L2123
	ldr	r3, .L2182+48
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2163
	str	r2, [fp, #-72]
	mov	r3, #8
	str	r8, [fp, #-76]
	sub	r2, fp, #76
	ldr	r0, [r6, #1144]
	mov	r1, #106
	blx	ip
	mvn	r0, #0
	b	.L2156
.L2063:
	ldr	r1, .L2182+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2168:
	ldr	r1, .L2182+144
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #16]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	ldr	r1, .L2182+148
	mov	r0, r5
	bl	se_v
	str	r0, [r4, #2208]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	ldr	r1, .L2182+152
	mov	r0, r5
	bl	se_v
	str	r0, [r4, #2212]
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	bne	.L2163
	ldr	r1, .L2182+156
	mov	r0, r5
	bl	ue_v
	cmp	r0, #255
	str	r0, [r4, #2216]
	bhi	.L2089
	ldr	r7, [r6, #1084]
	cmp	r7, #0
	bne	.L2089
	cmp	r0, #0
	addne	r8, r4, #2208
	addne	r8, r8, #8
	bne	.L2091
	b	.L2087
.L2181:
	ldr	r3, [r4, #2216]
	cmp	r3, r7
	bls	.L2087
.L2091:
	ldr	r1, .L2182+160
	mov	r0, r5
	bl	se_v
	add	r7, r7, #1
	str	r0, [r8, #4]!
	ldr	r3, [r6, #1084]
	cmp	r3, #0
	beq	.L2181
	b	.L2163
.L2092:
	mov	r2, r0
	ldr	r1, .L2182+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2106:
	mov	r0, #30720
	movt	r0, 105
	b	.L2102
.L2103:
	mov	r0, #55296
	movt	r0, 27
	b	.L2102
.L2107:
	mov	r0, #7864320
	b	.L2102
.L2101:
	mov	r0, #17920
	movt	r0, 5
	b	.L2102
.L2099:
	mov	r0, #20992
	movt	r0, 2
	b	.L2102
.L2098:
	ldr	r1, .L2182+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2089:
	ldr	r1, .L2182+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2179:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2074
	ldr	r1, .L2182+108
	mov	r2, #64
	ldr	r3, .L2182+72
	cmp	r10, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2074
.L2081:
	ldr	r1, .L2182+176
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2175:
	ldr	r1, .L2182+180
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2165:
	mov	r0, r4
	movw	r3, #10150
	ldr	r2, .L2182+184
	ldr	r1, .L2182+188
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2156
.L2183:
	.align	2
.L2182:
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
	ldr	r1, .L2198
	mov	r6, r0
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2198+4
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	mov	r0, r4
	ldr	r2, .L2198+8
	mov	r1, #2
	bl	u_v
	cmp	r0, #1
	beq	.L2194
	ldr	r2, [r5, #40]
	sub	r1, r2, #1
	cmp	r1, #1
	bls	.L2195
.L2187:
	cmp	r2, #1
	beq	.L2196
.L2188:
	ldr	r1, .L2198+12
	mov	r0, r4
	bl	u_1
	cmp	r0, #0
	bne	.L2197
.L2189:
	mov	r0, r4
	ldr	r1, .L2198+16
	bl	u_1
	cmp	r0, #1
	moveq	r0, #0
	beq	.L2186
	ldr	ip, [r5, #40]
	mov	r2, r0
	mov	r3, r6
	ldr	r1, .L2198+20
	mov	r0, #1
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r0, #0
.L2186:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2197:
	ldr	r1, .L2198+24
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2198+28
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	b	.L2189
.L2196:
	ldr	r2, .L2198+32
	mov	r1, #2
	mov	r0, r4
	bl	u_v
	ldr	r1, .L2198+36
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	b	.L2188
.L2195:
	ldr	r1, .L2198+40
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2198+44
	mov	r2, r0
	mov	r0, #17
	bl	dprint_vfmw
	ldr	r2, [r5, #40]
	b	.L2187
.L2194:
	ldr	r1, .L2198+48
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2186
.L2199:
	.align	2
.L2198:
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
	ldr	r2, .L2235
	mov	r1, #8
	mov	r4, r0
	add	r7, r0, #61440
	bl	u_v
	ldr	r1, .L2235+4
	mov	r10, r0
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2235+8
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2235+12
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2235+16
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	u_1
	ldr	r2, .L2235+20
	mov	r1, #4
	str	r0, [fp, #-68]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L2235+24
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	ldr	r1, .L2235+28
	mov	r6, r0
	mov	r0, r4
	bl	ue_v
	ldr	r2, [r7, #1084]
	cmp	r2, #0
	mov	r9, r0
	bne	.L2229
	add	r5, r4, #12288
	ldr	r2, [r5, #1288]
	sub	r2, r2, #1
	cmp	r0, r2
	bhi	.L2230
	sub	r2, r10, #66
	mov	r8, r10
	cmp	r2, #78
	ldrls	pc, [pc, r2, asl #2]
	b	.L2205
.L2207:
	.word	.L2206
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2208
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2208
	.word	.L2205
	.word	.L2205
	.word	.L2208
	.word	.L2205
	.word	.L2209
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2208
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2210
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2210
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2205
	.word	.L2210
.L2206:
	ldr	r1, .L2235+32
	mov	r0, #1
	bl	dprint_vfmw
.L2208:
	cmp	r6, #9
	mov	r2, r6
	bls	.L2228
.L2234:
	cmp	r6, #41
	bhi	.L2228
.L2212:
	mov	r3, #4000
	ldr	r1, [r5, #1300]
	mul	r7, r3, r9
	add	r1, r1, r7
	ldrb	r6, [r1, #23]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L2213
	ldr	r6, .L2235+36
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
	bne	.L2231
	ldr	r1, [r5, #1300]
	add	r0, r6, #2208
	str	r3, [fp, #-56]
	add	r1, r1, r7
	bl	SPSEqual
	ldr	r3, [fp, #-56]
	cmp	r0, #0
	bne	.L2232
.L2215:
	cmp	r8, #83
	cmpne	r8, #86
	beq	.L2217
.L2218:
	mov	r0, #0
.L2202:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2228:
	mov	r3, #41
	ldr	r1, .L2235+40
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, #41
	b	.L2212
.L2213:
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
	bne	.L2233
	ldr	r3, [r5, #1300]
	mov	r2, #1
	cmp	r8, #83
	cmpne	r8, #86
	add	r3, r3, r7
	strb	r2, [r3, #24]
	ldr	r3, [r5, #1300]
	add	r7, r3, r7
	strb	r2, [r7, #23]
	bne	.L2218
.L2217:
	mov	r0, r4
	bl	Sps_SvcExt
	cmp	r0, #0
	beq	.L2218
	ldr	r1, .L2235+44
	mov	r0, #1
	bl	dprint_vfmw
.L2229:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2230:
	ldr	r1, .L2235+48
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r5, #1288]
	sub	r3, r2, #1
	cmp	r9, r3
	bls	.L2229
	ldr	r3, .L2235+52
	ldr	r4, [r3]
	cmp	r4, #0
	beq	.L2229
	str	r2, [fp, #-48]
	mov	r3, #8
	str	r9, [fp, #-52]
	sub	r2, fp, #52
	ldr	r0, [r7, #1144]
	mov	r1, #109
	blx	r4
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2210:
	ldr	r1, .L2235+56
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2234
	b	.L2228
.L2209:
	ldr	r1, .L2235+60
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2234
	b	.L2228
.L2205:
	mov	r2, r10
	ldr	r1, .L2235+64
	mov	r0, #1
	mov	r8, #100
	bl	dprint_vfmw
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2234
	b	.L2228
.L2232:
	ldr	ip, .L2235+68
	mov	r2, r3
	ldr	r0, [r5, #1300]
	mov	r3, #1
	add	r1, r6, #2208
	strb	r3, [r6, #2232]
	add	r0, r0, r7
	ldr	ip, [ip, #56]
	strb	r3, [r6, #2231]
	blx	ip
	b	.L2215
.L2231:
	mov	r3, r9
	movw	r2, #10776
	ldr	r1, .L2235+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2202
.L2233:
	mov	r3, r9
	movw	r2, #10801
	ldr	r1, .L2235+72
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
	b	.L2202
.L2236:
	.align	2
.L2235:
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
	beq	.L2246
	ldrb	r3, [ip]	@ zero_extendqisi2
	mov	r2, r3, asl #2
	mov	r5, r3, asl #5
	rsb	r4, r2, r5
	add	r4, ip, r4
	add	r6, r4, #8
	ldr	lr, [r4, #8]
	cmp	lr, #0
	beq	.L2246
	ldr	lr, [ip, #68]
	cmp	r3, #1
	cmpls	lr, #2
	bhi	.L2246
	cmp	lr, r3
	bls	.L2246
	ldr	lr, [r4, #12]
	mov	r6, r2
	ldr	r4, [r4, #24]
	mov	lr, lr, asl #3
	add	r1, r4, r1, lsl #3
	cmp	r1, lr
	bhi	.L2241
	b	.L2239
.L2242:
	ldr	r4, [r2, #24]
	ldr	r2, [r2, #12]
	rsb	lr, lr, r4
	add	r1, r1, lr
	mov	lr, r2, asl #3
	cmp	r1, lr
	bls	.L2239
.L2241:
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
	bhi	.L2242
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2239:
	rsb	r2, r6, r5
	mov	r0, #1
	add	ip, ip, r2
	str	r1, [ip, #24]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2246:
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
	beq	.L2261
	ldr	r3, [r0, #368]
	cmp	r3, #0
	cmpne	r2, #0
	ble	.L2261
	ldrb	r7, [r3]	@ zero_extendqisi2
	mov	ip, r7, asl #5
	sub	ip, ip, r7, asl #2
	add	ip, r3, ip
	ldr	ip, [ip, #8]
	cmp	ip, #0
	beq	.L2261
	ldr	ip, [r3, #68]
	cmp	r7, #1
	cmpls	ip, #2
	movhi	lr, #1
	movls	lr, #0
	bhi	.L2261
	cmp	ip, r7
	bls	.L2261
.L2256:
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
	bcs	.L2252
	ldrb	r3, [r8, r4, lsr #3]	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	lr, r2
	add	r4, r1, #1
	add	ip, ip, #1
	strb	r3, [r1]
	b	.L2263
.L2254:
	cmp	ip, r6
	mov	r1, r4
	beq	.L2252
	ldrb	r3, [ip], #1	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	lr, r2
	strb	r3, [r4], #1
.L2263:
	ldr	r3, [r0, #368]
	add	r3, r3, r5
	ldr	r1, [r3, #24]
	add	r1, r1, #8
	str	r1, [r3, #24]
	bcc	.L2254
	mov	r0, r2
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2252:
	ldr	ip, [r0, #368]
	add	r7, r7, #1
	ldrb	r3, [ip]	@ zero_extendqisi2
	add	r3, r3, #1
	strb	r3, [ip]
	ldr	r3, [r0, #368]
	ldr	ip, [r3, #68]
	cmp	ip, r7
	bhi	.L2256
	mov	r0, lr
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2261:
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
	ldr	r1, .L2271
	mov	r5, r0
	bl	ue_v
	add	r4, r5, #24576
	ldr	r2, .L2271+4
	mov	r1, #1
	str	r0, [r4, #1840]
	mov	r0, r5
	bl	u_v
	uxtb	r0, r0
	strb	r0, [r4, #1817]
	cmp	r0, #0
	beq	.L2270
.L2265:
	mov	r0, r5
	ldr	r2, .L2271+8
	mov	r1, #1
	bl	u_v
	mov	r3, #1
	strb	r3, [r4, #1816]
	strb	r0, [r4, #1818]
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2270:
	ldr	r2, .L2271+12
	mov	r1, #7
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+16
	mov	r1, #1
	str	r0, [r4, #1832]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+20
	mov	r1, #6
	strb	r0, [r4, #1819]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+24
	mov	r1, #1
	str	r0, [r4, #1836]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+28
	mov	r1, #1
	strb	r0, [r4, #1820]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+32
	mov	r1, #1
	strb	r0, [r4, #1821]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+36
	mov	r1, #1
	strb	r0, [r4, #1822]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+40
	mov	r1, #1
	strb	r0, [r4, #1823]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+44
	mov	r1, #1
	strb	r0, [r4, #1824]
	mov	r0, r5
	bl	u_v
	ldrb	r3, [r4, #1819]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #1825]
	bne	.L2266
	ldr	r3, [r4, #1832]
	cmp	r3, #5
	beq	.L2266
	ldr	r2, .L2271+48
	mov	r1, #1
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+52
	mov	r1, #1
	strb	r0, [r4, #1826]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+56
	mov	r1, #1
	strb	r0, [r4, #1827]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2271+60
	mov	r1, #1
	strb	r0, [r4, #1828]
	mov	r0, r5
	bl	u_v
	strb	r0, [r4, #1829]
.L2266:
	ldr	r2, .L2271+64
	mov	r1, #8
	mov	r0, r5
	bl	u_v
	ldr	r1, .L2271+68
	str	r0, [r4, #1844]
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #1848]
	b	.L2265
.L2272:
	.align	2
.L2271:
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
	beq	.L2281
	ldrb	r3, [r0, #1945]	@ zero_extendqisi2
	add	r4, r0, #1872
	cmp	r3, #1
	ldrd	r2, [r4, #40]
	beq	.L2282
	adds	r2, r2, ip
	adc	r3, r3, #0
	strd	r2, [r4, #32]
.L2276:
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
.L2281:
	mov	r6, #0
	str	r6, [r0, #1904]
	mov	r2, r6
	mov	r3, r6
	str	r6, [r0, #1912]
	str	r6, [r0, #1908]
	str	r6, [r0, #1916]
	b	.L2276
.L2282:
	adds	r2, r2, ip
	adc	r3, r3, #0
	strd	r2, [r4, #32]
	strd	r2, [r4, #40]
	b	.L2276
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
	beq	.L2288
	mov	r9, r5
	mov	r0, r10
	b	.L2287
.L2289:
	mov	r9, r1
.L2287:
	mov	r1, r9
	bl	__aeabi_uidivmod
	mov	r0, r9
	cmp	r1, #0
	bne	.L2289
.L2286:
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
.L2288:
	mov	r9, r10
	b	.L2286
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
	bne	.L2291
	ldr	ip, [r2, #1288]
	cmp	ip, #0
	ble	.L2292
	ldrb	r2, [r3, #23]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #4000
	beq	.L2295
	b	.L2314
.L2296:
	ldrb	r1, [r3, #-3977]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2293
.L2295:
	add	r2, r2, #1
	mov	r4, r3
	cmp	r2, ip
	add	r3, r3, #4000
	bne	.L2296
.L2292:
	ldr	r1, .L2316
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
.L2297:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2314:
	mov	r4, r3
.L2293:
	cmp	r4, #0
	beq	.L2292
.L2291:
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2298
	ldrb	r3, [r4, #3299]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2315
	ldr	r1, [r4, #3676]
	ldr	r7, [r4, #3680]
	add	r1, r1, #1
	add	r7, r7, #1
.L2301:
	ldr	r2, .L2316+4
	mov	r0, r5
	bl	u_v
	add	r6, r5, #24576
	mov	r1, r7
	ldr	r2, .L2316+8
	str	r0, [r6, #1856]
	mov	r0, r5
	bl	u_v
	str	r0, [r6, #1860]
.L2298:
	ldrb	r0, [r4, #3303]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	r3, r0
	beq	.L2297
	ldr	r2, .L2316+12
	mov	r1, #4
	mov	r0, r5
	add	r5, r5, #24576
	bl	u_v
	mov	r3, #0
	strb	r0, [r5, #1852]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2315:
	ldrb	r3, [r4, #3302]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2298
	ldr	r1, [r4, #3988]
	ldr	r7, [r4, #3992]
	add	r1, r1, #1
	add	r7, r7, #1
	b	.L2301
.L2317:
	.align	2
.L2316:
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
	b	.L2321
.L2552:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2320
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2551
.L2321:
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
	bls	.L2552
.L2320:
	ldr	r1, .L2585
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2323
.L2551:
	mov	r3, #0
	str	r3, [fp, #-68]
.L2501:
	mov	r5, #0
	b	.L2327
.L2554:
	bl	BsSkip
	cmp	r10, #0
	ble	.L2325
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2553
.L2327:
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
	bls	.L2554
.L2325:
	ldr	r1, .L2585+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2323:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2553:
	cmp	r5, #409600
	bgt	.L2325
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
	b	.L2330
.L2332:
	.word	.L2331
	.word	.L2333
	.word	.L2334
	.word	.L2335
	.word	.L2336
	.word	.L2337
	.word	.L2338
	.word	.L2339
	.word	.L2340
	.word	.L2341
	.word	.L2342
	.word	.L2343
	.word	.L2344
	.word	.L2345
	.word	.L2346
	.word	.L2347
	.word	.L2348
	.word	.L2349
	.word	.L2350
	.word	.L2351
	.word	.L2352
	.word	.L2353
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2330
	.word	.L2354
.L2354:
	mov	r1, r5
	mov	r0, r7
	ldr	r9, [r4, #1264]
	bl	H264_DecFramePackingSEI
	ldr	r0, [r4, #1264]
	mov	r10, r5, asl #3
	rsb	r3, r9, r0
	cmp	r3, r10
	bge	.L2484
	rsb	r3, r3, r10
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r9, r3, #7
	movge	r9, r3
	ldr	r3, [r4, #1256]
	mov	r9, r9, asr #3
	cmp	r9, #0
	movle	r3, r3, asl #3
	ble	.L2486
	add	r2, r0, #8
	mov	r3, r3, asl #3
	cmp	r2, r3
	bhi	.L2486
	mov	r10, #0
	b	.L2487
.L2488:
	add	r2, r0, #8
	cmp	r2, r3
	bhi	.L2486
.L2487:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #1256]
	cmp	r10, r9
	ldr	r0, [r4, #1264]
	mov	r3, r3, asl #3
	bne	.L2488
.L2486:
	ldr	r2, [fp, #-56]
	mov	r1, r2, asr #31
	mov	r1, r1, lsr #29
	add	r2, r2, r1
	and	r2, r2, #7
	rsb	r1, r1, r2
	add	r2, r0, r1
	cmp	r3, r2
	bcs	.L2555
.L2484:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2556
.L2547:
	add	r3, r7, #57344
	ldr	r3, [r3, #3848]
.L2357:
	cmp	r3, #3
	bhi	.L2557
.L2496:
	mov	r9, #0
	b	.L2500
.L2498:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2499
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2558
.L2500:
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
	bls	.L2498
.L2499:
	mov	r0, #0
.L2583:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2353:
	mov	r3, r5
	ldr	r1, .L2585+8
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2559
.L2478:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2481
.L2482:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2481:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2482
	b	.L2547
.L2352:
	mov	r3, r5
	ldr	r1, .L2585+12
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2560
.L2473:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2476
.L2477:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2476:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2477
	b	.L2547
.L2350:
	mov	r3, r5
	ldr	r1, .L2585+16
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2561
.L2463:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2466
.L2467:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2466:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2467
	b	.L2547
.L2349:
	mov	r3, r5
	ldr	r1, .L2585+20
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2562
.L2458:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2461
.L2462:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2461:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2462
	b	.L2547
.L2348:
	mov	r3, r5
	ldr	r1, .L2585+24
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2563
.L2453:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2456
.L2457:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2456:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2457
	b	.L2547
.L2347:
	mov	r3, r5
	ldr	r1, .L2585+28
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2564
.L2448:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2451
.L2452:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2451:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2452
	b	.L2547
.L2351:
	mov	r3, r5
	ldr	r1, .L2585+32
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2565
.L2468:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2471
.L2472:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2471:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2472
	b	.L2547
.L2346:
	mov	r3, r5
	ldr	r1, .L2585+36
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2566
.L2443:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2446
.L2447:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2446:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2447
	b	.L2547
.L2345:
	mov	r3, r5
	ldr	r1, .L2585+40
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2567
.L2438:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2441
.L2442:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2441:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2442
	b	.L2547
.L2344:
	mov	r3, r5
	ldr	r1, .L2585+44
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2568
.L2433:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2436
.L2437:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2436:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2437
	b	.L2547
.L2343:
	mov	r3, r5
	ldr	r1, .L2585+48
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2569
.L2428:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2431
.L2432:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2431:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2432
	b	.L2547
.L2342:
	mov	r3, r5
	ldr	r1, .L2585+52
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2570
.L2423:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2426
.L2427:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2426:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2427
	b	.L2547
.L2341:
	mov	r3, r5
	ldr	r1, .L2585+56
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2571
.L2418:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2421
.L2422:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2421:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2422
	b	.L2547
.L2340:
	mov	r3, r5
	ldr	r1, .L2585+60
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2572
.L2413:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2416
.L2417:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2416:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2417
	b	.L2547
.L2339:
	mov	r3, r5
	ldr	r1, .L2585+64
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2573
.L2408:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2411
.L2412:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2411:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2412
	b	.L2547
.L2338:
	mov	r3, r5
	ldr	r1, .L2585+68
	mov	r0, #20
	bl	dprint_vfmw
	cmp	r5, #0
	ble	.L2405
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2405
	mov	r9, #0
	b	.L2406
.L2407:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2405
.L2406:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2407
.L2405:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	bge	.L2547
	movw	r3, #11637
	b	.L2548
.L2337:
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	bne	.L2336
	mov	r3, r5
	ldr	r1, .L2585+72
	mov	r0, #20
	bl	dprint_vfmw
.L2380:
	add	r10, r7, #57344
	add	r3, r7, #61440
	ldr	r9, [r10, #3848]
	cmp	r9, #3
	strls	r3, [fp, #-56]
	bhi	.L2574
.L2381:
	ldr	r3, [fp, #-56]
	ldr	r0, [r3, #1144]
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
	beq	.L2504
	ldr	r2, [r7, #368]
	cmp	r5, #1024
	movlt	r3, r5
	movge	r3, #1024
	ldrb	r0, [r2]	@ zero_extendqisi2
	ldr	ip, [r2, #68]
	str	r3, [fp, #-60]
	cmp	r0, ip
	bcs	.L2505
	sxth	r3, r0
	mov	r9, #0
	mov	r1, r3, asl #5
	sub	r1, r1, r3, asl #2
	add	r1, r2, r1
.L2383:
	ldr	r3, [r1, #24]
	add	r0, r0, #1
	ldr	r2, [r1, #12]
	cmp	r0, ip
	add	r3, r3, #7
	add	r1, r1, #28
	sub	r3, r2, r3, lsr #3
	add	r9, r9, r3
	bne	.L2383
	mov	r2, r9
.L2382:
	ldr	r1, [fp, #-60]
	ldrsb	r3, [fp, #-68]
	cmp	r1, r2
	movle	r9, r1
	cmp	r3, #1
	movne	r3, #0
	strne	r3, [fp, #-64]
	beq	.L2575
.L2385:
	rsb	r2, r3, r9
	cmp	r2, #0
	ble	.L2576
	ldr	r3, [r10, #3848]
	mov	r0, r7
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r1, [r3, #4]
	bl	GetBytes
	subs	r9, r0, #0
	ble	.L2577
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-64]
	rsb	r3, r2, r3
	cmp	r9, r3
	ldr	r3, [r10, #3848]
	bge	.L2391
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r9, [r3, #1052]
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2392
	mov	r5, #0
	b	.L2393
.L2396:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2392
.L2393:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2396
.L2392:
	ldr	r3, [r10, #3848]
	mov	r2, #8
	ldr	r1, [r7]
	add	r3, r3, #15296
	ldr	ip, .L2585+108
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
	ldr	r2, [r2, #1104]
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r2, [r3, #1044]
	beq	.L2395
	ldr	r2, [r10, #3848]
	movw	r3, #1064
	ldr	r1, [fp, #-56]
.L2549:
	add	r2, r2, #15296
	ldr	r0, [r1, #1144]
	add	r2, r2, #2
	mov	r1, #6
	add	r2, r7, r2, lsl #2
	ldr	r2, [r2, #4]
	blx	ip
.L2395:
	ldr	r3, [r10, #3848]
	add	r3, r3, #1
	str	r3, [r10, #3848]
	cmp	r3, #3
	bls	.L2496
.L2557:
	ldr	r1, .L2585+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2323
.L2334:
	mov	r3, r5
	ldr	r1, .L2585+80
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2578
.L2369:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2372
.L2373:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2372:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2373
	b	.L2547
.L2333:
	mov	r3, r5
	ldr	r1, .L2585+84
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
	bge	.L2362
	rsb	r3, r3, r10
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r9, r3, #7
	movge	r9, r3
	ldr	r3, [r4, #1256]
	mov	r9, r9, asr #3
	cmp	r9, #0
	movle	r3, r3, asl #3
	ble	.L2364
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bhi	.L2364
	mov	r10, #0
	b	.L2365
.L2366:
	add	r1, r2, #8
	cmp	r1, r3
	bhi	.L2364
.L2365:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #1256]
	cmp	r10, r9
	ldr	r2, [r4, #1264]
	mov	r3, r3, asl #3
	bne	.L2366
.L2364:
	ldr	ip, [fp, #-56]
	mov	r1, ip, asr #31
	mov	r0, r1, lsr #29
	add	r1, ip, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r2, r1
	cmp	r2, r3
	bls	.L2579
.L2362:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	bge	.L2547
	movw	r3, #11385
	b	.L2548
.L2335:
	mov	r3, r5
	ldr	r1, .L2585+88
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2580
.L2374:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2377
.L2378:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2377:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2378
	b	.L2547
.L2331:
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
	ldr	r1, .L2585+92
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2581
.L2355:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2359
.L2360:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2359:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2360
	b	.L2547
.L2336:
	mov	r3, r5
	ldr	r1, .L2585+96
	mov	r0, #20
	bl	dprint_vfmw
	mov	r3, #1
	str	r3, [fp, #-68]
	b	.L2380
.L2330:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2582
.L2491:
	cmp	r5, #0
	ble	.L2547
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
	mov	r9, #0
	b	.L2494
.L2495:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2547
.L2494:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2495
	b	.L2547
.L2558:
	cmp	r3, #128
	bne	.L2501
	mov	r0, #0
	b	.L2583
.L2391:
	add	r3, r3, #15296
	ldr	r2, [fp, #-60]
	add	r3, r3, #2
	cmp	r2, r5
	ldr	r2, [fp, #-64]
	add	r3, r7, r3, lsl #2
	rsb	r9, r2, r5
	ldr	r3, [r3, #4]
	str	r9, [r3, #1052]
	bcc	.L2584
.L2398:
	cmp	r9, #0
	ble	.L2392
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2392
	mov	r5, #0
	b	.L2400
.L2586:
	.align	2
.L2585:
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
.L2401:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2392
.L2400:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2401
	b	.L2392
.L2574:
	mov	r9, r3
	ldr	r0, [r3, #1144]
	ldr	r1, [r10, #3852]
	str	r3, [fp, #-56]
	bl	FreeUsdByDec
	ldr	r0, [r9, #1144]
	ldr	r1, [r10, #3856]
	bl	FreeUsdByDec
	ldr	r0, [r9, #1144]
	ldr	r1, [r10, #3860]
	bl	FreeUsdByDec
	ldr	r0, [r9, #1144]
	ldr	r1, [r10, #3864]
	bl	FreeUsdByDec
	mov	r3, #0
	mov	r9, r3
	str	r3, [r10, #3852]
	str	r3, [r10, #3856]
	str	r3, [r10, #3860]
	str	r3, [r10, #3864]
	str	r3, [r10, #3848]
	b	.L2381
.L2556:
	movw	r3, #12065
.L2548:
	ldr	r2, .L2585+112
	mov	r0, #20
	ldr	r1, .L2585+116
	bl	dprint_vfmw
	add	r3, r7, #57344
	ldr	r3, [r3, #3848]
	b	.L2357
.L2555:
	mov	r0, r6
	bl	BsSkip
	b	.L2484
.L2579:
	mov	r0, r6
	bl	BsSkip
	b	.L2362
.L2575:
	strb	r3, [lr, #1026]
	mov	r1, #8
	ldr	r3, [r10, #3848]
	mov	r0, r7
	ldr	r2, .L2585+100
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
	beq	.L2386
	mov	r3, #3
	mov	r2, #0
	str	r3, [fp, #-64]
	strb	r2, [ip, #1028]
.L2387:
	ldr	ip, [r10, #3848]
	mov	r1, #16
	ldr	r2, .L2585+104
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
	bne	.L2385
	ldr	r3, [r10, #3848]
	mov	r2, #8
	ldr	r1, [r7]
	add	r3, r3, #15296
	ldr	ip, .L2585+108
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
	ldr	r2, [r1, #1104]
	add	r3, r3, #15296
	add	r3, r3, #2
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #4]
	str	r2, [r3, #1044]
	beq	.L2395
	ldr	r2, [r10, #3848]
	movw	r3, #1064
	b	.L2549
.L2584:
	ldr	r3, [fp, #-60]
	mov	r0, r7
	rsb	r1, r3, r5
	bl	PassBytes
	cmp	r0, #0
	bge	.L2398
	mov	r3, #11584
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2398
.L2505:
	mov	r2, #0
	mov	r9, r2
	b	.L2382
.L2582:
	movw	r3, #12077
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2491
.L2581:
	movw	r3, #11332
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2355
.L2580:
	movw	r3, #11423
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2374
.L2578:
	movw	r3, #11397
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2369
.L2573:
	movw	r3, #11649
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2408
.L2572:
	movw	r3, #11675
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2413
.L2571:
	movw	r3, #11701
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2418
.L2570:
	movw	r3, #11727
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2423
.L2569:
	movw	r3, #11753
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2428
.L2568:
	movw	r3, #11779
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2433
.L2567:
	movw	r3, #11805
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2438
.L2566:
	movw	r3, #11831
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2443
.L2565:
	movw	r3, #11961
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2468
.L2564:
	movw	r3, #11857
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2448
.L2563:
	movw	r3, #11883
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2453
.L2562:
	movw	r3, #11909
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2458
.L2561:
	movw	r3, #11935
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2463
.L2560:
	movw	r3, #11987
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2473
.L2559:
	movw	r3, #12013
	ldr	r2, .L2585+112
	ldr	r1, .L2585+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2478
.L2386:
	ldr	r2, .L2585+120
	mov	r1, #8
	mov	r0, r7
	mov	r3, #4
	str	ip, [fp, #-72]
	str	r3, [fp, #-64]
	bl	u_v
	ldr	ip, [fp, #-72]
	ldr	r3, [fp, #-64]
	strb	r0, [ip, #1028]
	b	.L2387
.L2504:
	mvn	r0, #0
	b	.L2323
.L2576:
	ldr	r1, .L2585+124
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r10, #3848]
	ldr	r2, [fp, #-56]
	add	r3, r3, #15296
	add	r3, r3, #2
	ldr	r0, [r2, #1144]
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
	b	.L2323
.L2577:
	ldr	r1, .L2585+128
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r10, #3848]
	ldr	r2, [fp, #-56]
	add	r3, r3, #15296
	add	r3, r3, #2
	ldr	r0, [r2, #1144]
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
	b	.L2323
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
	b	.L2596
.L2591:
	.word	.L2590
	.word	.L2596
	.word	.L2596
	.word	.L2596
	.word	.L2590
	.word	.L2596
	.word	.L2592
	.word	.L2592
.L2592:
	ldr	r3, [r0]
	mov	r2, #1
	str	r2, [r0]
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2590:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L2593
.L2595:
	mvn	r0, #0
.L2594:
	mov	r2, #0
	str	r2, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2596:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2593:
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
	bhi	.L2595
	mov	r3, r0, asl #1
	rsb	r3, r3, #31
	mov	r3, r4, lsr r3
	subs	r0, r3, #1
	mvnne	r0, #0
	b	.L2594
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
	ble	.L2623
	ldr	r3, [fp, #-48]
	subs	r7, r3, #0
	add	r8, r3, #63
	movge	r8, r3
	ands	r3, r3, #63
	movne	r3, #1
	add	r8, r3, r8, asr #6
	cmp	r8, #0
	ble	.L2611
	ldr	r5, .L2624
	ldr	r10, .L2624+4
.L2609:
	cmp	r7, #64
	ldr	r3, [r10, #52]
	ldr	r0, .L2624
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
	bne	.L2601
	ldr	r2, .L2624
	add	r4, r4, #1
	cmp	r3, r2
	bne	.L2604
	b	.L2602
.L2605:
	cmp	r3, r5
	add	r4, r4, #1
	beq	.L2602
.L2604:
	ldrb	r2, [r3, #-1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2605
.L2601:
	mov	r0, r4, asl #3
.L2600:
	ldr	r3, [fp, #-48]
	cmp	r4, r3
	bge	.L2613
	sub	r3, r3, #1
	rsb	r4, r4, r3
	ldrb	r2, [r9, r4]	@ zero_extendqisi2
	tst	r2, #1
	moveq	r2, r2, lsr #1
	moveq	r3, #1
	bne	.L2613
.L2608:
	tst	r2, #1
	mov	r2, r2, lsr #1
	bne	.L2606
	add	r3, r3, #1
	cmp	r3, #8
	bne	.L2608
	b	.L2606
.L2602:
	subs	r8, r8, #1
	sub	r7, r7, #64
	bne	.L2609
	b	.L2601
.L2613:
	mov	r3, #0
.L2606:
	add	r0, r3, r0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2623:
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L2624+8
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2611:
	mov	r0, r4
	b	.L2600
.L2625:
	.align	2
.L2624:
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
	beq	.L2632
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2631
	ldr	r1, [r3, #40]
	ldr	r0, [r3, #36]
	bl	FindZeroBitsInSeg
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #40]
	cmp	r0, r2, asl #3
	bcs	.L2635
	cmn	r0, #1
	beq	.L2631
.L2630:
	add	r0, r0, #1
	str	r0, [r3, #72]
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2634
	ldr	r1, [r3, #40]
	ldr	r2, [r3, #72]
	cmp	r2, r1, asl #3
	bcs	.L2636
.L2634:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2636:
	add	r2, r4, #61440
	ldr	r1, [r3, #60]
	ldr	r0, [r2, #1144]
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
.L2635:
	add	r2, r4, #61440
	ldr	r1, [r3, #60]
	ldr	r0, [r2, #1144]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #368]
	mov	r1, #0
	mov	r2, #1
	str	r1, [r3, #36]
	ldr	r3, [r4, #368]
	str	r2, [r3, #68]
	ldr	r3, [r4, #368]
.L2631:
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	bl	FindZeroBitsInSeg
	ldr	r3, [r4, #368]
	b	.L2630
.L2632:
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
	beq	.L2644
	mov	r2, ip
	mov	r3, #1
	b	.L2642
.L2641:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L2647
.L2642:
	ldrb	r1, [r2, #473]	@ zero_extendqisi2
	add	r2, r2, #96
	cmp	r1, #0
	bne	.L2641
	mov	r0, r3
.L2640:
	mov	r2, r3, asl #7
	mov	r1, #1
	sub	r3, r2, r3, asl #5
	add	ip, ip, r3
	strb	r1, [ip, #377]
	ldmfd	sp, {fp, sp, pc}
.L2647:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2644:
	mov	r3, r0
	b	.L2640
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
	beq	.L2650
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
.L2650:
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r7, r0, #61440
	mov	r6, r2
	mov	r4, r0
	mov	r9, r1
	ldr	r2, [r7, #1088]
	mov	r0, #22
	ldr	r1, .L2663
	add	r5, r4, #20480
	sub	r2, r2, #1
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r6]
	mov	r0, r4
	str	r3, [r9]
	ldr	r3, [r4, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r5, #211]
	ldr	r3, [r3, #76]
	str	r3, [r5, #240]
	bl	SliceCheck
	subs	r8, r0, #0
	bne	.L2657
	mov	r0, r4
	bl	ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L2661
	ldrb	r0, [r5, #213]	@ zero_extendqisi2
	add	r3, r4, #12288
	mov	r2, #2208
	mov	ip, #4000
	sub	r0, r0, #1
	clz	r0, r0
	mov	r0, r0, lsr #5
	str	r0, [r9]
	ldr	r0, [r5, #220]
	ldr	r5, [r3, #1304]
	ldr	lr, [r7, #1156]
	ldr	r1, [r3, #1300]
	mla	r2, r2, r0, r5
	cmp	lr, #1
	ldr	r9, [r3, #1280]
	ldr	lr, [r3, #1276]
	ldr	r3, [r2, #28]
	mla	r1, ip, r3, r1
	ldrb	r3, [r1, #18]	@ zero_extendqisi2
	ldr	r5, [r1, #3252]
	rsb	r3, r3, #2
	ldr	r2, [r1, #3248]
	mla	r5, r5, r3, r3
	add	r10, r2, #1
	beq	.L2662
	cmp	lr, r10
	cmpeq	r9, r5
	add	r4, r4, #24576
	movne	r3, #1
	strne	r3, [r6]
	ldr	r3, [r4, #2144]
	ldr	r2, [r1, #3272]
	add	r3, r3, #1
	cmp	r2, r3
	movhi	r3, #1
	strhi	r3, [r6]
.L2653:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2662:
	add	r0, r4, #13504
	add	r0, r0, #24
	bl	BsResidBits
	ldr	r1, [r7, #1068]
	add	r1, r1, #1
	mov	r8, r0
	mul	r0, r5, r10
	bl	__aeabi_uidiv
	mov	r3, r8, lsr #31
	cmp	r8, r0
	movge	r0, r3
	orrlt	r0, r3, #1
	rsb	r8, r0, #0
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2657:
	mvn	r8, #0
	b	.L2653
.L2661:
	ldr	r1, .L2663+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L2653
.L2664:
	.align	2
.L2663:
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
	ldr	r0, [r0, #1068]
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
	beq	.L2667
	add	r5, r0, #26880
	add	r8, r0, #61440
	add	r5, r5, #248
	mov	r4, #0
	b	.L2671
.L2668:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L2670
	ldr	r3, [r6, #252]
	cmp	r4, r3
	bcs	.L2693
.L2671:
	ldr	r3, [r5, #4]!
	add	r4, r4, #1
	ldr	r0, [r8, #1144]
	ldr	r3, [r3, #32]
	ldr	r1, [r3, #632]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L2668
.L2670:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2693:
	cmp	r3, #0
	beq	.L2667
	ldr	r3, [r6, #256]
	cmp	r3, #0
	addne	r7, r7, #27136
	movne	r4, #0
	addne	r7, r7, #124
	bne	.L2674
	b	.L2667
.L2694:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L2670
	ldr	r3, [r6, #256]
	cmp	r4, r3
	bcs	.L2667
.L2674:
	ldr	r3, [r7, #4]!
	add	r4, r4, #1
	ldr	r0, [r8, #1144]
	ldr	r3, [r3, #32]
	ldr	r1, [r3, #632]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L2694
	b	.L2670
.L2667:
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
	ldr	r3, [r5, #1080]
	add	r9, r8, #57344
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [r5, #1080]
	ldr	r3, [r9, #3848]
	cmp	r3, #0
	beq	.L2697
	add	r6, r8, #61184
	mov	r4, #0
	add	r6, r6, #8
	mov	r7, r4
.L2699:
	ldr	r1, [r6, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L2698
	ldr	r0, [r5, #1144]
	bl	FreeUsdByDec
	str	r7, [r6]
.L2698:
	ldr	r3, [r9, #3848]
	cmp	r3, r4
	bhi	.L2699
.L2697:
	add	r9, r8, #27904
	add	r8, r8, #59648
	add	r9, r9, #88
	add	r8, r8, #24
	mov	r6, #0
	mov	r7, #0
.L2703:
	sub	r4, r9, #32
.L2701:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L2715
.L2700:
	cmp	r4, r9
	bne	.L2701
	add	r9, r4, #792
	cmp	r9, r8
	bne	.L2703
	ldr	r1, .L2716
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L2715:
	mov	r1, r2
	ldr	r0, [r5, #1144]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L2700
.L2717:
	.align	2
.L2716:
	.word	.LC346
	UNWIND(.fnend)
	.size	H264DEC_Destroy, .-H264DEC_Destroy
	.align	2
	.global	H264DEC_IsNeedSedOnly
	.type	H264DEC_IsNeedSedOnly, %function
H264DEC_IsNeedSedOnly:
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
	add	r4, r0, #61440
	ldr	r3, [r4, #1068]
	cmp	r3, #0
	beq	.L2721
	ldr	r2, [r0, #360]
	cmp	r2, #0
	beq	.L2726
.L2721:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2726:
	ldr	r2, [r4, #1168]
	cmp	r3, r2
	beq	.L2721
	ldr	r3, [r4, #1156]
	cmp	r3, #1
	beq	.L2727
	cmp	r3, #0
	bne	.L2721
	ldr	r0, [r4, #1144]
	bl	VCTRL_CheckFreshRaw
	cmp	r0, #0
	beq	.L2721
	ldr	r0, [r4, #1144]
	bl	VCTRL_CheckFreshSeg
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2727:
	ldr	r0, [r0, #368]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	H264DEC_IsNeedSedOnly, .-H264DEC_IsNeedSedOnly
	.align	2
	.global	H264DEC_IsSliceUnit
	.type	H264DEC_IsSliceUnit, %function
H264DEC_IsSliceUnit:
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
	bic	r3, r0, #4
	cmp	r0, #20
	cmpne	r3, #1
	moveq	r0, #1
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	H264DEC_IsSliceUnit, .-H264DEC_IsSliceUnit
	.align	2
	.global	H264DEC_IsFrameEndNal
	.type	H264DEC_IsFrameEndNal, %function
H264DEC_IsFrameEndNal:
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
	sub	r3, r0, #7
	cmp	r3, #1
	bls	.L2731
	bic	r3, r0, #4
	cmp	r3, #11
	cmpne	r0, #30
	moveq	r0, #1
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2731:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	H264DEC_IsFrameEndNal, .-H264DEC_IsFrameEndNal
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
	ldr	r1, .L2743
	bl	dprint_vfmw
	ldr	r0, [r5, #1144]
	mov	r1, r8
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L2737
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	beq	.L2737
	add	r4, r0, #560
	add	r9, r0, #592
	mov	r6, #0
	mov	r7, #0
.L2735:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L2742
.L2734:
	cmp	r4, r9
	bne	.L2735
	ldr	r0, [r5, #1144]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2742:
	mov	r1, r2
	ldr	r0, [r5, #1144]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L2734
.L2737:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2744:
	.align	2
.L2743:
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
	beq	.L2784
	ldr	r0, [r0]
	ldr	r0, [r0, #12]
	cmp	r0, #0
	ble	.L2748
	add	r3, r4, #61440
	ldr	r3, [r3, #1072]
	cmp	r3, #2
	beq	.L2748
	add	r4, r4, #57344
	ldr	r3, [r4, #3188]
	cmp	r3, #0
	strneb	ip, [r3, #1]
	movne	r0, #1
	strne	ip, [r4, #3188]
	beq	.L2750
.L2747:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2748:
	mov	r1, r7
	mov	r0, r4
	bl	CheckFrameStore
	cmn	r0, #3
	beq	.L2750
	add	r5, r4, #57344
	ldr	r1, [r5, #3188]
	cmp	r1, #0
	beq	.L2751
	mov	r0, r4
	bl	GetImagePara
	ldr	r1, [r5, #3188]
	mov	r0, r4
	bl	CheckFrameStore
	subs	r3, r0, #0
	beq	.L2785
	cmn	r3, #3
	beq	.L2758
	ldr	r1, .L2793
	movw	r2, #2046
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #3188]
	mov	r0, r4
	ldr	r1, [r3, #164]
	bl	H264DEC_RecycleImage
.L2758:
	ldr	r3, [r5, #3188]
	mov	r0, #0
	strb	r0, [r3, #1]
	ldr	r3, [r5, #3188]
	str	r0, [r5, #3188]
	cmp	r7, r3
	beq	.L2747
.L2751:
	mov	r1, r7
	mov	r0, r4
	bl	GetImagePara
	mov	r1, r7
	mov	r0, r4
	bl	CheckFrameStore
	subs	r3, r0, #0
	beq	.L2786
	cmn	r3, #3
	beq	.L2750
	movw	r2, #2104
	ldr	r1, .L2793
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r7, #164]
	mov	r0, r4
	bl	H264DEC_RecycleImage
	mov	r0, #2
	b	.L2747
.L2784:
	mov	r3, r1
	mov	r2, r0
	ldr	r1, .L2793+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2750:
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2786:
	add	r8, r4, #61440
	mov	r2, #1
	ldr	r1, [r7, #632]
	ldr	r0, [r8, #1144]
	bl	FSP_SetDisplay
	ldr	r0, [r8, #1144]
	ldr	r1, [r7, #632]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L2787
	cmp	r6, #1
	add	r3, r4, #8
	streq	r6, [r5, #188]
	mov	r2, r4
	ldr	r0, [r8, #1144]
	mov	r1, #0
	str	r5, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L2788
	ldr	r3, [r4]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r5, #24]
	bcs	.L2789
.L2765:
	ldr	r2, [r8, #1200]
	mov	r0, #0
	ldr	r3, [r8, #1192]
	add	r2, r2, #1
	str	r2, [r8, #1200]
	add	r3, r3, #2
	str	r3, [r8, #1192]
	b	.L2747
.L2785:
	ldr	r3, [r5, #3188]
	add	r8, r4, #61440
	mov	r2, #1
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	ldr	r3, [r5, #3188]
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_GetFsImagePtr
	subs	r9, r0, #0
	beq	.L2790
	ldr	r3, [r5, #3188]
	mov	r2, r4
	mov	r1, #0
	cmp	r7, r3
	cmpeq	r6, #1
	moveq	r3, #1
	streq	r3, [r9, #188]
	add	r3, r4, #8
	ldr	r0, [r8, #1144]
	str	r9, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L2791
	ldr	r3, [r4]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r9, #24]
	bcs	.L2792
.L2757:
	ldr	r2, [r8, #1200]
	ldr	r3, [r8, #1192]
	add	r2, r2, #1
	str	r2, [r8, #1200]
	add	r3, r3, #2
	str	r3, [r8, #1192]
	b	.L2758
.L2789:
	mov	r1, r5
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L2765
.L2792:
	mov	r1, r9
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L2757
.L2788:
	mov	r2, r0
	ldr	r1, .L2793+8
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, [r7, #632]
	ldr	r0, [r8, #1144]
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, r4
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	b	.L2747
.L2791:
	mov	r2, r0
	ldr	r1, .L2793+8
	mov	r0, #0
	mov	r6, r0
	bl	dprint_vfmw
	ldr	r3, [r5, #3188]
	mov	r2, r6
	strb	r6, [r3, #1]
	ldr	r3, [r5, #3188]
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	str	r6, [r5, #3188]
	mov	r0, r4
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	b	.L2747
.L2787:
	movw	r2, #2074
	ldr	r1, .L2793+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2747
.L2790:
	movw	r2, #2017
	ldr	r1, .L2793+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2747
.L2794:
	.align	2
.L2793:
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
	bne	.L2796
	mov	r1, r6
	mov	r0, r5
	bl	RemoveFrameStoreOutDPB
.L2796:
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
	beq	.L2799
	add	r7, r0, #26368
	mov	r5, #0
	add	r7, r7, #156
	b	.L2801
.L2800:
	ldr	r3, [r6, #2148]
	cmp	r3, r5
	bls	.L2799
.L2801:
	ldr	r1, [r7, #4]!
	add	r5, r5, #1
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2800
	mov	r0, r4
	bl	UnMarkFrameStoreRef
	ldr	r3, [r6, #2148]
	cmp	r3, r5
	bhi	.L2801
.L2799:
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L2803
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L2799
	b	.L2803
.L2805:
	bl	GetMinPOC
	ldr	r1, [fp, #-36]
	mov	r0, r4
	bl	OutputFrmFromDPB
	cmn	r0, #1
	beq	.L2804
.L2803:
	ldr	r3, [r6, #2148]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	bne	.L2805
	mov	r0, r3
.L2804:
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
.L2826:
	ldr	r3, [r4]
	cmp	r3, #6
	ldrls	pc, [pc, r3, asl #2]
	b	.L2814
.L2816:
	.word	.L2815
	.word	.L2817
	.word	.L2818
	.word	.L2819
	.word	.L2820
	.word	.L2821
	.word	.L2822
.L2822:
	ldr	r2, [r4, #12]
	mov	r1, r7
	mov	r0, r5
	bl	MarkCurrPicLT
.L2814:
	add	r4, r4, #20
	cmp	r4, r6
	bne	.L2826
.L2815:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #3444]
	cmp	r3, #1
	movne	r0, #0
	beq	.L2835
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2821:
	add	r8, r5, #24576
	ldr	r3, [r8, #2152]
	cmp	r3, #0
	ldrne	r9, [fp, #-52]
	movne	r10, #0
	beq	.L2825
.L2824:
	ldr	r1, [r9, #4]!
	mov	r0, r5
	bl	UnMarkFrameStoreRef
	ldr	r1, [r8, #2152]
	add	r10, r10, #1
	cmp	r10, r1
	bcc	.L2824
.L2825:
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
	b	.L2814
.L2820:
	mov	r0, r5
	ldr	r1, [r4, #16]
	bl	UpdateMaxLTFrmIdx
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2814
.L2819:
	ldr	r3, [r4, #12]
	mov	r1, r7
	ldr	r2, [r4, #4]
	mov	r0, r5
	bl	MarkSTToLTRef
	mov	r0, r5
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2814
.L2818:
	mov	r0, r5
	ldr	r2, [r4, #8]
	mov	r1, r7
	bl	UnMarkLTRef
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2814
.L2817:
	mov	r0, r5
	ldr	r2, [r4, #4]
	mov	r1, r7
	bl	UnMarkSTRef
	mov	r0, r5
	bl	UpdateReflist
	b	.L2814
.L2835:
	ldr	r1, [fp, #-56]
	mov	r3, #0
	ldrb	r2, [r1, #1739]	@ zero_extendqisi2
	str	r3, [r1, #2384]
	cmp	r2, #1
	beq	.L2829
	bcc	.L2830
	cmp	r2, #2
	bne	.L2828
	ldr	r2, [fp, #-56]
	str	r3, [r2, #2408]
	str	r3, [r2, #2396]
	ldr	r2, [fp, #-48]
	str	r3, [r2, #3456]
	str	r3, [r2, #3464]
.L2828:
	mov	r0, r5
	bl	FlushDPB
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2830:
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
	b	.L2828
.L2829:
	ldr	r2, [fp, #-56]
	str	r3, [r2, #2404]
	str	r3, [r2, #2396]
	ldr	r2, [fp, #-48]
	str	r3, [r2, #3452]
	str	r3, [r2, #3464]
	b	.L2828
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
	beq	.L2837
	add	r9, r0, #24576
	ldr	r3, [r9, #2148]
	cmp	r3, #0
	beq	.L2844
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
.L2843:
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
	ldr	r0, [r5, #1144]
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
	ldr	r0, [r5, #1144]
	bl	FSP_GetDisplay
	mov	r2, #0
	cmp	r0, #3
	beq	.L2842
	ldr	r1, [r8]
	ldr	r0, [r5, #1144]
	ldr	r1, [r1, #632]
	bl	FSP_SetDisplay
.L2842:
	str	r4, [r8], #4
	str	r4, [r7, #4]!
	str	r4, [r6, #4]!
	ldr	r2, [r9, #2148]
	cmp	r2, r10
	bhi	.L2843
.L2844:
	ldr	r2, [fp, #-52]
	mov	r3, #0
	str	r3, [r9, #2148]
	ldr	r1, [r2, #3824]
	cmp	r1, r3
	beq	.L2840
	ldr	r2, [fp, #-48]
	mov	r0, r3
	add	r2, r2, #60928
	add	r2, r2, #164
.L2847:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L2847
.L2840:
	ldr	r4, [fp, #-48]
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	ldr	r3, [fp, #-52]
	ldr	r3, [r3, #2380]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2857
	ldr	r2, [fp, #-52]
	mov	r0, r3
	str	r3, [r9, #2160]
	strb	r3, [r2, #1740]
	mov	r3, #1
	strb	r3, [r2, #1741]
.L2848:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2857:
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
.L2837:
	bl	FlushDPB
	cmp	r0, #0
	ldreq	r3, [fp, #-48]
	addeq	r9, r3, #24576
	beq	.L2840
.L2849:
	mvn	r0, #0
	b	.L2848
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
	beq	.L2859
	ldr	r3, [r4, #2412]
	cmp	r3, #0
	bne	.L2860
	ldr	r3, [r0]
	ldr	r2, [r3, #640]
	cmp	r2, #1
	beq	.L2884
.L2860:
	ldr	r3, [r4, #2380]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r4, #2392]
	beq	.L2885
	cmp	r3, #0
	bne	.L2886
.L2867:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2885:
	cmp	r3, #0
	beq	.L2867
.L2866:
	ldrb	r2, [r4, #1737]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2871
	add	r2, r5, #24576
	ldr	r1, [r6, #456]
	ldr	ip, [r2, #2156]
	ldr	r0, [r2, #2152]
	rsb	r2, ip, r1
	cmp	r0, r2
	beq	.L2887
.L2872:
	mov	r2, #0
	strb	r2, [r4, #1740]
.L2869:
	cmp	r3, #0
	beq	.L2867
.L2871:
	ldrb	r3, [r4, #1740]	@ zero_extendqisi2
	mov	r0, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r4, #1741]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2884:
	ldr	r2, [r3, #636]
	cmp	r2, #0
	bne	.L2860
	ldr	r2, [r3, #644]
	cmp	r2, #0
	bne	.L2860
	ldr	r3, [r3, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	bls	.L2860
.L2859:
	mov	r0, r5
	bl	IDRMemMarking
	cmp	r0, #0
	bne	.L2861
.L2868:
	ldrb	r3, [r4, #1742]	@ zero_extendqisi2
	cmp	r3, #5
	ldreq	r3, [r4, #2392]
	beq	.L2869
	ldr	r3, [r4, #2392]
	cmp	r3, #0
	beq	.L2867
	ldr	r2, [r4, #2380]
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2866
	b	.L2871
.L2886:
	mov	r0, r5
	bl	AdaptiveMemMark
	cmp	r0, #0
	beq	.L2868
.L2861:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2887:
	mov	r0, r5
	bl	SlidingWinMark.part.2
	ldr	r3, [r4, #2392]
	b	.L2872
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
	beq	.L2890
	bcc	.L2891
	cmp	r6, #2
	beq	.L2892
	mov	r6, #0
.L2889:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2892:
	ldr	r3, [r4, #2376]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r2, [r4, #1737]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2980
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
.L2920:
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
	beq	.L2977
.L2923:
	strb	r2, [r3, #3]
	add	r7, r5, #61440
	ldr	r3, [r4, #3188]
	mov	r2, #0
	ldr	r0, [r7, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r0, r5
	mov	r2, #0
	ldr	r1, [r4, #3188]
	bl	OutputFrmToVO
	ldr	r3, [r4, #3188]
	cmp	r3, #0
	mov	r6, r0
	beq	.L2924
	ldr	r3, [r4, #2376]
	mov	r2, #1
	ldr	r0, [r7, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	b	.L2924
.L2891:
	ldr	r3, [r4, #2376]
	mov	r2, #0
	strb	r2, [r3, #676]
	ldr	r1, [r4, #3188]
	cmp	r1, r2
	beq	.L2971
	bl	OutputFrmToVO
	cmp	r0, #0
	bne	.L2981
.L2894:
	ldr	r3, [r4, #3188]
	cmp	r3, #0
	beq	.L2971
	ldr	r3, [r4, #2376]
	add	r8, r5, #61440
	mov	r2, #1
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
.L2893:
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
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r2, r7
	mov	r0, r5
	ldr	r1, [r4, #3188]
	bl	OutputFrmToVO
	ldr	r3, [r4, #3188]
	cmp	r3, r7
	mov	r6, r0
	beq	.L2924
	ldr	r3, [r4, #2376]
	mov	r2, #1
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
.L2924:
	mov	r3, #0
	mov	r0, r6
	str	r3, [r4, #3188]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2890:
	ldr	r3, [r4, #2376]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r2, [r4, #1737]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2898
	ldr	r1, [r4, #3188]
	cmp	r1, #0
	beq	.L2899
	bl	OutputFrmToVO
	subs	r6, r0, #0
	bne	.L2982
.L2900:
	ldr	r3, [r4, #2376]
	str	r3, [r4, #3188]
	str	r3, [r3, #744]
.L2901:
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
	ldr	r9, [r8, #1076]
	cmp	r9, #1
	bne	.L2889
.L2975:
	ldr	r3, [r4, #3188]
	mov	r2, r7
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r2, r7
	mov	r0, r5
	ldr	r1, [r4, #3188]
	bl	OutputFrmToVO
	ldr	r3, [r4, #3188]
	cmp	r3, r7
	mov	r6, r0
	beq	.L2924
	ldr	r3, [r4, #2376]
	mov	r2, r9
	ldr	r0, [r8, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	b	.L2924
.L2980:
	ldr	r1, [r4, #3188]
	cmp	r1, #0
	beq	.L2912
	bl	OutputFrmToVO
	subs	r6, r0, #0
	bne	.L2983
.L2913:
	ldr	r3, [r4, #2376]
	str	r3, [r4, #3188]
	str	r3, [r3, #780]
.L2914:
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
	ldr	r9, [r8, #1076]
	cmp	r9, #1
	beq	.L2975
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2898:
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
.L2906:
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
	bne	.L2923
.L2977:
	orr	r2, r2, ip
	b	.L2923
.L2971:
	add	r8, r5, #61440
	b	.L2893
.L2981:
	str	r0, [sp]
	mov	r3, #2704
	ldr	r2, .L2984
	mov	r0, #22
	ldr	r1, .L2984+4
	bl	dprint_vfmw
	b	.L2894
.L2912:
	ldr	r3, [r4, #2376]
	mov	r6, r1
	str	r3, [r4, #3188]
	str	r3, [r3, #780]
	b	.L2914
.L2899:
	ldr	r3, [r4, #2376]
	mov	r6, r1
	str	r3, [r4, #3188]
	str	r3, [r3, #744]
	b	.L2901
.L2983:
	str	r6, [sp]
	movw	r3, #2898
	ldr	r2, .L2984
	mov	r0, #22
	ldr	r1, .L2984+4
	bl	dprint_vfmw
	b	.L2913
.L2982:
	str	r6, [sp]
	movw	r3, #2756
	ldr	r2, .L2984
	mov	r0, #22
	ldr	r1, .L2984+4
	bl	dprint_vfmw
	b	.L2900
.L2985:
	.align	2
.L2984:
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
	beq	.L2988
	add	r7, r0, #26368
	mov	r6, #0
	add	r7, r7, #156
	b	.L2990
.L2989:
	ldr	r3, [r5, #2148]
	cmp	r3, r6
	bls	.L2988
.L2990:
	ldr	r1, [r7, #4]!
	add	r6, r6, #1
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2989
	mov	r0, r4
	bl	UnMarkFrameStoreRef
	ldr	r3, [r5, #2148]
	cmp	r3, r6
	bhi	.L2990
.L2988:
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L2988
	mvn	r6, #0
.L2992:
	ldr	r3, [r5, #2148]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	beq	.L3004
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
	bne	.L2992
	mov	r0, r4
	bl	RemoveFrameStoreOutDPB
	b	.L2992
.L3004:
	cmp	r6, #0
	beq	.L2995
	add	r0, r4, #8
	bl	GetVoLastImageID
	mov	r6, r0
.L2995:
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
	beq	.L3020
	bl	ClearAllSlice
	add	r7, r6, #26368
	add	r5, r6, #26624
	add	r9, r6, #61440
	mov	r4, r7
	add	r5, r5, #32
	add	r7, r7, #160
	add	r4, r4, #224
	mov	r8, #0
.L3010:
	ldr	r3, [r7]
	mov	r2, #1
	add	r8, r8, r2
	cmp	r3, #0
	beq	.L3007
	ldr	r1, [r3, #632]
	ldr	r0, [r9, #1144]
	bl	FSP_ClearLogicFs
.L3007:
	ldr	r3, [r4]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3008
	ldr	r1, [r3, #632]
	ldr	r0, [r9, #1144]
	bl	FSP_ClearLogicFs
.L3008:
	ldr	r3, [r5]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3009
	ldr	r1, [r3, #632]
	ldr	r0, [r9, #1144]
	bl	FSP_ClearLogicFs
.L3009:
	cmp	r8, #16
	mov	r2, #0
	str	r2, [r5], #4
	str	r2, [r4], #4
	str	r2, [r7], #4
	bne	.L3010
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
	beq	.L3012
	add	r7, r6, #61440
	ldr	r3, [r7, #1072]
	cmp	r3, #2
	beq	.L3043
.L3013:
	ldr	r1, [r1, #632]
	mov	r2, #1
	ldr	r0, [r7, #1144]
	bl	FSP_ClearLogicFs
	ldr	r3, [r5, #2376]
	cmp	r3, #0
	beq	.L3016
	ldr	r2, [r5, #3188]
	ldr	r1, [r3, #632]
	ldr	r3, [r2, #632]
	cmp	r3, r1
	beq	.L3016
	ldr	r0, [r7, #1144]
	mov	r2, #1
	bl	FSP_ClearLogicFs
.L3016:
	mov	r3, #0
	str	r3, [r5, #3188]
.L3012:
	ldr	r3, .L3045
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
.L3017:
	str	r1, [r3, #4]!
	cmp	r3, r2
	bne	.L3017
	add	r2, r6, #26880
	add	r3, r6, #27136
	add	r2, r2, #248
	add	r3, r3, #124
	mov	r0, #0
.L3018:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L3018
	add	ip, r6, #20480
	add	r6, r6, #27392
	mov	r2, r1
	str	r1, [ip, #252]
.L3019:
	str	r2, [r3, #4]!
	cmp	r3, r6
	mov	r0, #0
	bne	.L3019
	str	r0, [ip, #256]
.L3006:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3043:
	mov	r0, r6
	bl	OutputFrmToVO
	cmp	r0, #0
	bne	.L3044
.L3014:
	ldr	r1, [r5, #3188]
	cmp	r1, #0
	beq	.L3012
	b	.L3013
.L3044:
	str	r0, [sp]
	movw	r3, #5669
	ldr	r2, .L3045+4
	mov	r0, #22
	ldr	r1, .L3045+8
	bl	dprint_vfmw
	b	.L3014
.L3020:
	mvn	r0, #0
	b	.L3006
.L3046:
	.align	2
.L3045:
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
	beq	.L3048
	movw	r2, #501
	ldr	r1, .L3052
	mov	r0, #0
	bl	dprint_vfmw
.L3048:
	mov	r0, r4
	bl	InitDPB
	add	r2, r4, #57344
	add	r4, r4, #61440
	mov	r3, #0
	mov	r0, r3
	str	r3, [r2, #3836]
	str	r3, [r4, #1176]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3053:
	.align	2
.L3052:
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
	beq	.L3055
	ldrb	r3, [r6, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3055
.L3056:
	ldr	r5, [r4, #1132]
	ldr	r3, [r4, #1072]
	ubfx	r5, r5, #16, #3
	cmp	r3, r5
	beq	.L3057
	subs	r2, r5, #2
	sub	r3, r3, #2
	movne	r2, #1
	cmp	r3, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L3057
	cmp	r5, #4
	cmpne	r5, #1
	bls	.L3149
.L3057:
	cmp	r5, #2
	str	r5, [r4, #1072]
	beq	.L3060
	cmp	r5, #3
	beq	.L3061
	cmp	r5, #1
	beq	.L3150
	ldr	r3, [r4, #1068]
	cmp	r3, #0
	beq	.L3064
	cmp	r5, #4
	ldr	r3, [r6, #2412]
	beq	.L3151
.L3071:
	cmp	r3, #2
	beq	.L3089
.L3075:
	cmp	r3, #1
	streq	r3, [r6, #3840]
.L3077:
	ldr	r2, [r4, #1108]
	mov	r0, #2
	ldrb	r3, [r6, #1739]	@ zero_extendqisi2
	sub	r2, r2, #1
	ldr	r1, .L3157
	bl	dprint_vfmw
	ldr	r2, [r6, #2436]
	mov	r3, #1
	str	r2, [r4, #580]
	strb	r3, [r6, #3874]
	ldr	r1, [r4, #1108]
	ldr	r2, [r6, #2440]
	ldr	r3, [r4, #1068]
	cmp	r1, #0
	str	r2, [r4, #576]
	str	r3, [r4, #780]
	ldreq	r2, [r7]
	beq	.L3080
	ldr	r1, [r6, #2412]
	cmp	r1, #0
	beq	.L3152
	add	r3, r7, #24576
	ldr	r3, [r3, #2148]
	cmp	r3, #0
	bne	.L3082
	ldr	r2, [r7]
	ldr	r1, [r2, #12]
	cmp	r1, #2
	bne	.L3153
.L3085:
	add	r3, r7, #61184
	add	r3, r3, #32
	str	r3, [r7, #360]
	ldr	r1, [r2, #864]
	cmp	r1, #1
	beq	.L3154
	cmp	r1, #2
	beq	.L3155
.L3088:
	mov	r3, #0
	str	r3, [r4, #1056]
.L3080:
	ldr	r3, [r2, #592]
	mov	r0, #0
	str	r3, [r4, #1044]
	ldr	r3, [r2, #632]
	str	r3, [r4, #1048]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3055:
	ldr	r3, [r4, #1196]
	ldr	r2, [r4, #1200]
	add	r3, r3, #1
	cmp	r3, r2
	movlt	r3, r2
	str	r3, [r4, #1196]
	ldr	r1, [r7]
	rsb	r3, r2, r3
	str	r3, [r1, #576]
	b	.L3056
.L3149:
	mov	r2, r5
	ldr	r1, .L3157+4
	mov	r0, #2
	bl	dprint_vfmw
	str	r5, [r4, #1072]
.L3061:
	mov	r0, r7
	bl	H264_ClearCurrPic
	mov	r0, r7
	bl	ClearDPB
	mvn	r0, #0
.L3141:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3060:
	ldr	r5, [r6, #2412]
	cmp	r5, #0
	bne	.L3067
	ldr	r3, [r4, #1068]
	cmp	r3, #0
	beq	.L3064
	ldr	r3, [r4, #1064]
	ldr	r3, [r3, #48]
	cmp	r3, #0
	beq	.L3077
	mov	r3, #1
	mov	r0, r7
	strb	r3, [r6, #1743]
	bl	H264_ClearCurrPic
	mov	r0, r5
	ldr	r1, .L3157+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3141
.L3150:
	ldr	r3, [r6, #2412]
	cmp	r3, #2
	beq	.L3156
	ldr	r2, [r4, #1068]
	cmp	r2, #0
	bne	.L3075
.L3064:
	mov	r0, r7
	bl	H264_ClearCurrPic
	ldr	r3, .L3157+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3145
	mov	r3, #0
	ldr	r0, [r4, #1144]
	mov	r2, r3
	mov	r1, #127
	blx	ip
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3152:
	ldr	r3, .L3157+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3082
	ldr	r2, [r4, #784]
	cmp	r2, #0
	beq	.L3083
.L3084:
	ldr	r3, [r2, #8]
	ldr	r0, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3084
.L3083:
	sub	r2, fp, #28
	mov	r3, #4
	ldr	r0, [r4, #1144]
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
	ldr	r2, [r7]
	b	.L3085
.L3156:
	ldr	r3, [r6, #2392]
	cmp	r3, #0
	beq	.L3067
	ldr	r3, [r4, #1068]
	cmp	r3, #0
	beq	.L3064
.L3089:
	ldr	r3, [r6, #3840]
	cmp	r3, #2
	bne	.L3077
	ldr	r3, [r7]
	ldr	r5, [r3, #12]
	cmp	r5, #0
	bne	.L3077
	ldr	r3, .L3157+16
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3077
	ldr	r3, [r6, #2392]
	mov	r0, #1
	ldr	r2, [r6, #2396]
	ldr	r1, .L3157+20
	bl	dprint_vfmw
	mov	r0, r7
	bl	H264_ClearCurrPic
	ldr	r3, .L3157+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3078
	mov	r3, r5
	mov	r2, r5
	mov	r1, #127
	ldr	r0, [r4, #1144]
	blx	ip
.L3078:
	ldr	r3, [r6, #2376]
	mov	r2, #0
	ldr	r0, [r4, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L3141
.L3154:
	cmp	r3, #0
	beq	.L3088
	ldr	r3, [r6, #2412]
	cmp	r3, #2
	bne	.L3088
	mov	r2, #0
	str	r2, [r7, #360]
	ldr	r3, [r6, #2376]
	ldr	r0, [r4, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
.L3144:
	mov	r0, r7
	bl	H264_ClearCurrPic
.L3145:
	mvn	r0, #0
	b	.L3141
.L3151:
	cmp	r3, #0
	bne	.L3071
	ldr	r3, [r6, #3840]
	cmp	r3, #0
	moveq	r3, #2
	streq	r3, [r6, #3840]
	movne	r3, #1
	strne	r3, [r6, #3840]
	b	.L3077
.L3082:
	ldr	r2, [r7]
	b	.L3085
.L3155:
	ldr	r1, [r2, #868]
	cmp	r3, #0
	cmpne	r1, #0
	ble	.L3088
	ldr	r3, [r6, #2412]
	cmp	r3, #2
	bne	.L3088
	mov	r2, #0
	str	r2, [r7, #360]
	ldr	r3, [r6, #2376]
	ldr	r0, [r4, #1144]
	ldr	r1, [r3, #632]
	bl	FSP_SetDisplay
	ldr	r2, [r7]
	mov	r0, r7
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3141
.L3153:
	ldr	r2, .L3157+12
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3144
	ldr	r0, [r4, #1144]
	mov	r2, r3
	mov	r1, #127
	blx	ip
	b	.L3144
.L3067:
	mov	r3, #1
	mov	r0, r7
	strb	r3, [r6, #1743]
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3141
.L3158:
	.align	2
.L3157:
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
	ldr	r1, .L3161
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L3162:
	.align	2
.L3161:
	.word	.LC359
	UNWIND(.fnend)
	.size	H264_FlushDecoder, .-H264_FlushDecoder
	.align	2
	.global	H264DEC_SetDecParam
	.type	H264DEC_SetDecParam, %function
H264DEC_SetDecParam:
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
	add	r4, r0, #61440
	mov	r5, r0
	ldr	r3, [r4, #1160]
	cmp	r3, #1
	beq	.L3174
.L3164:
	mov	r0, r5
	bl	H264DEC_IsNeedSedOnly
	cmp	r0, #1
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r2, [r4, #1068]
	add	ip, r5, #57344
	str	r0, [r4, #1056]
	add	r3, r5, #61184
	add	r3, r3, #32
	mov	r1, #0
	str	r2, [r4, #1168]
	ldr	r0, [ip, #2436]
	str	r2, [r4, #780]
	str	r0, [r4, #580]
	ldr	r2, [r5]
	ldr	r2, [r2, #632]
	str	r2, [r4, #1048]
	str	r3, [r5, #360]
	str	r1, [r4, #1160]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3174:
	ldr	r3, [r4, #1068]
	cmp	r3, #0
	beq	.L3164
	ldr	r3, [r0, #360]
	cmp	r3, #0
	bne	.L3164
	bl	DecVDM
	ldr	r6, [r5, #360]
	cmp	r6, #0
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r1, [r5, #368]
	cmp	r1, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	str	r6, [r5, #368]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	H264DEC_SetDecParam, .-H264DEC_SetDecParam
	.align	2
	.global	H264DEC_CheckSedOnly
	.type	H264DEC_CheckSedOnly, %function
H264DEC_CheckSedOnly:
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
	add	r4, r0, #61440
	mov	r3, #0
	mov	r5, r0
	ldr	r2, [r4, #1152]
	str	r3, [r4, #1156]
	cmp	r2, #2
	beq	.L3206
	cmp	r2, #1
	beq	.L3207
.L3192:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3207:
	ldr	r2, [r4, #1160]
	cmp	r2, #1
	bne	.L3192
	str	r3, [r4, #1160]
.L3205:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3206:
	ldr	r2, [r4, #1148]
	cmp	r2, r3
	beq	.L3205
	ldr	r2, [r4, #1160]
	cmp	r2, #1
	beq	.L3192
	ldr	r0, [r4, #1144]
	bl	VCTRL_CheckFreshSeg
	cmp	r0, #0
	beq	.L3205
	ldr	r0, [r4, #1144]
	bl	VCTRL_CheckFreshRaw
	cmp	r0, #0
	beq	.L3205
	ldr	r3, [r5, #368]
	ldr	r2, [r3, #12]
	ldr	r1, [r3, #8]
	cmp	r2, #3
	bls	.L3205
	add	r6, r5, #13504
	add	r6, r6, #24
	mov	r0, r6
	bl	BsInit
	mov	r1, #24
	mov	r0, r6
	bl	BsSkip
	mov	r0, r6
	bl	BsResidBits
	cmp	r0, #7
	ble	.L3205
	mov	r1, #3
	mov	r0, r6
	bl	BsSkip
	mov	r1, #5
	mov	r0, r6
	bl	BsGet
	cmp	r0, #20
	mov	r7, r0
	beq	.L3208
	bic	r3, r0, #4
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #1
	beq	.L3181
	ldr	r2, [r4, #1068]
	cmp	r2, #0
	beq	.L3205
	mov	r0, #0
.L3187:
	bic	r2, r7, #8
	cmp	r2, #7
	beq	.L3185
	cmp	r7, #8
	cmpne	r7, #30
	moveq	r2, #1
	movne	r2, #0
	cmp	r7, #11
	movne	r7, r2
	orreq	r7, r2, #1
	cmp	r0, #0
	moveq	r0, r7
	orrne	r0, r7, #1
	cmp	r0, #0
	bne	.L3185
	cmp	r3, #1
	bne	.L3205
.L3188:
	add	r5, r5, #57344
	mov	r3, #0
	mov	r2, #1
	str	r3, [r4, #1160]
	str	r2, [r4, #1156]
	mov	r0, r3
	ldrb	r3, [r5, #1736]	@ zero_extendqisi2
	str	r3, [r4, #1164]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3185:
	mov	r0, r5
	bl	DecVDM
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3208:
	mov	r0, r6
	bl	BsResidBits
	cmp	r0, #23
	ble	.L3205
	mov	r1, #24
	mov	r0, r6
	bl	BsGet
	tst	r0, #8388608
	bne	.L3209
.L3181:
	mov	r0, r6
	bl	BsResidBits
	cmp	r0, #0
	movle	r0, #0
	ble	.L3184
	mov	r0, r6
	mov	r1, #1
	bl	BsGet
.L3184:
	ldr	r3, [r4, #1068]
	cmp	r3, #0
	beq	.L3188
	mov	r3, #1
	b	.L3187
.L3209:
	tst	r0, #4194304
	moveq	r7, #20
	movne	r7, #5
	b	.L3181
	UNWIND(.fnend)
	.size	H264DEC_CheckSedOnly, .-H264DEC_CheckSedOnly
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
	ldr	r1, .L3273
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r6, #1204]
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3268
.L3211:
	ldr	r3, [r4, #368]
	cmp	r3, #0
	beq	.L3231
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3269
.L3214:
	ldr	r1, .L3273+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r4, #368]
	cmp	r1, #0
	beq	.L3231
	ldr	r0, [r6, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L3231:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3218
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L3218
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3218
	ldrb	r3, [r4, #377]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3234
	mov	r2, r4
	mov	r3, #1
	b	.L3221
.L3220:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L3232
.L3221:
	ldrb	r1, [r2, #473]	@ zero_extendqisi2
	add	r2, r2, #96
	cmp	r1, #0
	bne	.L3220
	mov	r2, r3
.L3219:
	mov	r1, r3, asl #7
	cmn	r2, #1
	sub	r3, r1, r3, asl #5
	mov	r1, #1
	add	r3, r4, r3
	strb	r1, [r3, #377]
	beq	.L3232
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
	ldr	r3, [r5, #20]
	str	r3, [r6, #1152]
	ldr	r3, [r4, #368]
	b	.L3224
.L3269:
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bhi	.L3214
	ldrb	r1, [r3, #4]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3215
	b	.L3214
.L3268:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	beq	.L3211
	ldr	r3, [r4, #368]
	cmp	r3, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	beq	.L3231
.L3215:
	strb	r2, [r3, #4]
	ldr	r3, [r4]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3270
.L3226:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3227
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq	.L3227
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3227
	ldr	r3, [r4, #368]
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bls	.L3271
.L3228:
	mov	r2, #1
	strb	r2, [r3, #4]
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #1144]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3273+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3265
	mov	r3, #0
	ldr	r0, [r6, #1144]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3265:
	ldr	r3, [r4, #368]
.L3224:
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	strb	r2, [r3, #5]
	ldr	r3, [r4, #368]
	ldrb	r0, [r3, #4]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3218:
	ldr	r1, .L3273+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #1144]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3273+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3267
.L3266:
	mov	r3, #0
	ldr	r0, [r6, #1144]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3267:
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3227:
	ldr	r3, [r4, #368]
	b	.L3228
.L3270:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	bne	.L3265
	b	.L3226
.L3271:
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
	bcc	.L3272
.L3229:
	mov	r2, #1
	str	r2, [r6, #1204]
	b	.L3224
.L3272:
	mov	r0, r4
	bl	CombinePacket.part.10
	ldr	r3, [r4, #368]
	b	.L3229
.L3232:
	ldr	r1, .L3273+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearAllNal
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	ldr	r3, .L3273+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3266
	b	.L3267
.L3234:
	mov	r2, r3
	b	.L3219
.L3274:
	.align	2
.L3273:
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
	ldr	r1, .L3303
	bl	dprint_vfmw
	mov	r0, r8
	bl	H264_ClearCurrPic
	mov	r0, r8
	bl	ClearAllNal
	mov	r0, r8
	bl	InitDPB
	cmp	r4, #0
	add	r7, r8, #61440
	beq	.L3276
	add	r1, r8, #8
	ldr	r0, [r7, #1144]
	bl	FSP_ClearNotInVoQueue
.L3277:
	add	r4, r8, #27904
	mov	r6, #0
	add	r4, r4, #88
.L3280:
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
.L3279:
	ldrd	r2, [r5, #8]!
	orrs	r1, r2, r3
	bne	.L3301
.L3278:
	cmp	r5, r4
	bne	.L3279
	add	r6, r6, #1
	add	r4, r5, #792
	cmp	r6, #40
	bne	.L3280
	add	r5, r8, #59648
	mov	r4, r5
	add	r5, r5, #32
.L3282:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3302
.L3281:
	cmp	r4, r5
	bne	.L3282
	add	r6, r8, #57344
	ldr	r3, [r6, #3848]
	cmp	r3, #0
	beq	.L3286
	add	r5, r8, #61184
	mov	r4, #0
	add	r5, r5, #8
	mov	r9, r4
.L3285:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3284
	ldr	r0, [r7, #1144]
	bl	FreeUsdByDec
	str	r9, [r5]
.L3284:
	ldr	r3, [r6, #3848]
	cmp	r3, r4
	bhi	.L3285
.L3286:
	ldr	r3, .L3303+4
	add	r5, r8, #20480
	add	ip, r8, #24576
	mov	r4, #0
	add	r0, r5, #208
	str	r4, [r6, #3848]
	mov	r1, r4
	str	r4, [r7, #1120]
	ldr	r3, [r3, #48]
	mov	r2, #1648
	str	r4, [r6, #3828]
	str	r4, [r7, #1068]
	str	r4, [r7, #1128]
	str	r4, [r7, #1076]
	str	r4, [r6, #3836]
	str	r4, [r7, #1176]
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
.L3301:
	mov	r1, r2
	ldr	r0, [r7, #1144]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r5]
	b	.L3278
.L3302:
	mov	r1, r2
	ldr	r0, [r7, #1144]
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	strd	r2, [r4]
	b	.L3281
.L3276:
	add	r0, r8, #8
	bl	ResetVoQueue
	ldr	r0, [r7, #1144]
	bl	FSP_EmptyInstance
	b	.L3277
.L3304:
	.align	2
.L3303:
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
	bhi	.L3404
	ldrb	r7, [r2, #3]	@ zero_extendqisi2
	cmp	r7, #1
	beq	.L3309
	bcc	.L3310
	cmp	r7, #2
	bne	.L3405
	ldr	r3, [r2, #640]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r3, r1, asl #2
	beq	.L3330
	add	r6, r0, r3
	add	r2, r6, #24576
	str	r2, [fp, #-48]
	ldr	r2, [r2, #1952]
	cmp	r2, #0
	beq	.L3330
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
.L3340:
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
	beq	.L3342
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3342
.L3343:
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
	b	.L3329
.L3310:
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
	beq	.L3406
.L3312:
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
	bne	.L3402
.L3329:
	add	r3, r9, #6592
	add	r1, r5, #61440
	add	r3, r3, #40
	mov	r2, #1
	ldr	r0, [r1, #1144]
	ldr	r3, [r5, r3, asl #2]
	ldr	r1, [r3, #632]
	bl	FSP_SetRef
	mov	r0, #0
.L3307:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3405:
	mov	r2, r7
	ldr	r1, .L3407
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r5
	mov	r1, #1
	bl	ClearAll
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3309:
	ldr	r3, [r2, #640]
	mov	r2, #3
	strb	r2, [r3, #676]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r3, r1, asl #2
	beq	.L3315
	add	r6, r0, r3
	add	r2, r6, #24576
	str	r2, [fp, #-48]
	ldr	r2, [r2, #1952]
	cmp	r2, #0
	beq	.L3315
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
.L3324:
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
	beq	.L3326
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3326
.L3327:
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
	b	.L3329
.L3315:
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
	beq	.L3317
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3317
.L3318:
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
	beq	.L3329
	mov	r3, r0
	movw	r2, #2394
	mov	r0, r7
.L3402:
	ldr	r1, .L3407+4
	bl	dprint_vfmw
	mov	r0, r5
	bl	ClearDPB
	mvn	r0, #0
	b	.L3307
.L3330:
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
	beq	.L3332
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3332
.L3333:
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
	beq	.L3329
	b	.L3402
.L3406:
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	b	.L3312
.L3317:
	ldr	r3, [r6, #1952]
	mov	r2, #1
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, r2
	ldreq	r3, [r6, #1952]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #644]
	b	.L3318
.L3332:
	ldr	r3, [r6, #1952]
	mov	r2, #2
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r6, #1952]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #644]
	b	.L3333
.L3342:
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
	b	.L3343
.L3326:
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
	b	.L3327
.L3404:
	mov	r3, r1
	movw	r2, #2303
	ldr	r1, .L3407+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3307
.L3408:
	.align	2
.L3407:
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
	beq	.L3575
	add	r5, r0, #57344
	mov	r3, #0
	str	r3, [r5, #1832]
	str	r3, [r5, #1836]
.L3412:
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
	beq	.L3576
	add	r6, r4, #61440
	ldr	r0, [r6, #1072]
	ldr	r3, [r6, #1184]
	cmp	r0, #2
	str	r3, [r6, #1180]
	beq	.L3577
	ldr	r7, [r4]
	ldr	r3, [r7, #12]
	cmp	r3, #0
	ble	.L3578
	ldrb	r1, [r5, #1738]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3420
.L3586:
	sub	r1, r2, #1
	ldr	r9, [r5, #2376]
	cmp	r1, #1
	movhi	r1, #0
	movls	r1, #1
	cmp	r2, #0
	beq	.L3421
	ldrb	r0, [r9, #1]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	ip, r1
	orrne	ip, r1, #1
	cmp	ip, #0
	beq	.L3420
	cmp	r1, #0
	beq	.L3579
.L3495:
	ldr	r1, [r6, #1176]
	cmp	r1, #0
	bne	.L3424
	add	r1, r5, #1728
	mov	r3, #1
	add	r1, r1, #8
	str	r3, [r6, #1176]
	mov	r2, #712
	add	r0, r5, #2448
	bl	memcpy
	ldr	r3, [r7, #12]
.L3420:
	cmp	r3, #2
	ldr	r3, [r5, #2392]
	beq	.L3580
	cmp	r3, #0
	beq	.L3456
	ldr	r3, [r6, #1136]
	ldr	r2, [r5, #2420]
	cmp	r3, r2
	bcs	.L3457
	ldrb	r1, [r5, #1738]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L3581
.L3457:
	mov	r0, r4
	bl	Marking
	subs	r2, r0, #0
	beq	.L3459
	ldr	r1, .L3600
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3961
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3421:
	cmp	r1, #0
	bne	.L3495
.L3427:
	add	r7, r4, #58880
	add	r7, r7, #200
.L3429:
	cmp	r2, #0
	beq	.L3430
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3582
.L3430:
	ldr	r3, [r7, #684]
	str	r3, [r9, #664]
.L3431:
	mov	r1, r9
	mov	r0, r4
	bl	GetImagePara
	ldr	r1, [r9, #632]
	ldr	r0, [r6, #1144]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L3583
	ldrb	r3, [r7, #3]	@ zero_extendqisi2
	add	ip, r7, #624
	cmp	r3, #0
	beq	.L3433
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3584
.L3433:
	ldrd	r2, [ip, #-8]
	add	r1, r8, #608
.L3567:
	strd	r2, [r1, #-8]
.L3434:
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
	beq	.L3435
	ldr	r3, [r8, #108]
	ands	r7, r3, #3
	beq	.L3585
.L3436:
	ldr	r3, [r4]
	ldr	r2, [r3, #4]
	cmp	r2, r1
	bcs	.L3435
	ldr	r3, .L3600+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3446
	str	r1, [fp, #-52]
	mov	r3, #8
	str	r2, [fp, #-48]
	mov	r1, #104
	sub	r2, fp, #52
	ldr	r0, [r6, #1144]
	blx	ip
.L3446:
	ldr	r1, [r9, #632]
	mov	r2, #0
	ldr	r0, [r6, #1144]
	bl	FSP_SetDisplay
	movw	r3, #3884
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3576:
	ldr	r1, .L3600+12
	add	r6, r4, #61440
	bl	dprint_vfmw
	movw	r3, #3666
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
.L3494:
	ldr	r3, [r5, #2376]
	mov	r2, #1
	ldr	r0, [r6, #1144]
	mvn	r7, #0
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	mov	r3, r7
	movw	r2, #4180
	ldr	r1, .L3600+16
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r5, #2376]
	mov	r3, #0
	strb	r3, [r2, #4]
	ldr	r2, [r5, #2376]
	strb	r3, [r2, #1]
.L3492:
	mov	r0, r7
	mov	r3, #0
	strb	r3, [r5, #1736]
.L3564:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3575:
	ldr	r2, .L3600+20
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
	b	.L3412
.L3578:
	ldr	r1, [r6, #1176]
	cmp	r1, #2
	beq	.L3420
	ldrb	r1, [r5, #1738]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3420
	b	.L3586
.L3580:
	cmp	r3, #0
	beq	.L3449
	ldrb	r3, [r5, #1742]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3450
.L3454:
	mov	r0, r4
	add	r7, r4, #24576
	bl	SimpleSlideDPB
	add	r2, r4, #58880
	ldr	r1, [r7, #2148]
	add	r2, r2, #200
	mov	r0, r4
	bl	InsertFrmInDPB
	subs	r8, r0, #0
	beq	.L3587
	mov	r3, r8
	movw	r2, #3927
	ldr	r1, .L3600+16
	mov	r0, #0
	bl	dprint_vfmw
	movw	r3, #3928
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3585:
	movw	r2, #3851
	ldr	r1, .L3600+24
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3600+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3568
	mov	r3, r7
	mov	r2, r7
	mov	r1, #111
	ldr	r0, [r6, #1144]
	blx	ip
.L3568:
	ldr	r1, [r8, #160]
	cmp	r1, #0
	bne	.L3436
.L3435:
	mov	r2, #1
	ldr	r1, [r9, #632]
	ldr	r0, [r6, #1144]
	bl	FSP_SetDisplay
	ldr	r0, [r6, #1144]
	add	r3, r4, #8
	str	r8, [sp]
	mov	r2, r4
	mov	r1, #0
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L3440
	mov	r2, #0
	ldr	r1, [r9, #632]
	ldr	r0, [r6, #1144]
	bl	FSP_SetDisplay
.L3441:
	ldr	r3, [r4]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r8, #24]
	bcs	.L3588
.L3443:
	ldr	r2, [r6, #1176]
	ldr	r3, [r6, #1192]
	cmp	r2, #2
	add	r3, r3, #2
	str	r3, [r6, #1192]
	movne	r3, #1
	strne	r3, [r9, #784]
	mov	r3, #2
	str	r3, [r6, #1176]
	ldr	r3, [r4]
	ldr	r3, [r3, #12]
	b	.L3420
.L3579:
	cmp	r0, #0
	beq	.L3420
	b	.L3427
.L3459:
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L3449
	ldr	r1, .L3600+28
	mov	r0, #22
	bl	dprint_vfmw
.L3449:
	ldrb	r3, [r5, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3461
	add	r7, r4, #24576
.L3465:
	ldr	r3, [r5, #2392]
	cmp	r3, #0
	beq	.L3589
	ldr	r2, [r7, #2152]
	ldr	r1, [r7, #2156]
	ldr	r3, [r7, #2144]
	add	r0, r1, r2
	cmp	r3, r0
	bhi	.L3473
	ldr	r0, [r4]
	ldr	r0, [r0, #12]
	cmp	r0, #3
	beq	.L3474
	ldr	r0, .L3600+32
	ldrb	r0, [r0]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L3474
	str	r1, [sp, #4]
	mov	r0, #1
	str	r2, [sp]
	movw	r2, #4040
	ldr	r1, .L3600+36
	bl	dprint_vfmw
	mov	r0, r4
	bl	ClearDPB
	movw	r3, #4066
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3456:
	ldrb	r3, [r5, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3461
	add	r7, r4, #24576
	ldr	r3, [r7, #2144]
.L3473:
	mvn	r10, #0
	mov	r9, r10
	b	.L3483
.L3484:
	bl	GetMinPOC
	ldr	r3, [r5, #2392]
	cmp	r3, #0
	bne	.L3476
	ldr	r2, [r5, #2396]
	ldr	r3, [fp, #-56]
	cmp	r2, r3
	blt	.L3590
.L3476:
	ldr	r8, [r7, #2148]
	cmp	r8, r10
	ldrne	r9, [fp, #-52]
	bne	.L3481
	ldr	r3, [fp, #-52]
	cmp	r3, r9
	beq	.L3591
	mov	r9, r3
.L3481:
	mov	r1, r9
	mov	r0, r4
	bl	OutputFrmFromDPB
	mov	r10, r8
	cmn	r0, #1
	beq	.L3482
	ldr	r3, [r7, #2144]
.L3483:
	ldr	ip, [r7, #2148]
	sub	r2, fp, #52
	sub	r1, fp, #56
	mov	r0, r4
	cmp	ip, r3
	bcs	.L3484
	ldr	r3, [r5, #2392]
	cmp	r3, #0
	beq	.L3485
	ldrb	r3, [r5, #1740]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3485
	ldr	r2, [r7, #2152]
	cmp	r2, #0
	beq	.L3485
	add	r2, r4, #26368
	mov	lr, r3
	add	r2, r2, #220
	b	.L3490
.L3487:
	ldr	r1, [r7, #2152]
	cmp	r1, r3
	bls	.L3592
.L3490:
	ldr	r1, [r2, #4]!
	add	r3, r3, #1
	ldr	r0, [r5, #2384]
	ldr	ip, [r1, #636]
	cmp	ip, r0
	bne	.L3487
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
	b	.L3487
.L3592:
	ldr	ip, [r7, #2148]
.L3485:
	add	r2, r4, #58880
	mov	r1, ip
	add	r2, r2, #200
	mov	r0, r4
	bl	InsertFrmInDPB
	ldr	r2, [r7, #2148]
	str	r2, [r5, #3184]
	subs	r3, r0, #0
	bne	.L3593
	add	r2, r2, #1
	mov	r0, r4
	str	r2, [r7, #2148]
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
.L3455:
	mov	r7, #0
	b	.L3492
.L3589:
	ldr	r3, [r7, #2144]
	b	.L3473
.L3461:
	ldr	r3, [r5, #3188]
	ldr	r0, [r5, #2376]
	cmp	r3, #0
	rsb	r1, r3, r0
	clz	r1, r1
	mov	r1, r1, lsr #5
	moveq	r1, #0
	cmp	r1, #0
	beq	.L3594
	mov	r0, r4
	bl	DirectOutput
	cmn	r0, #1
	mov	r7, r0
	beq	.L3595
	cmp	r0, #1
	ldreq	r2, [r5, #2376]
	moveq	r3, #0
	streqb	r3, [r2, #4]
	ldreq	r2, [r5, #2376]
	streqb	r3, [r2, #1]
.L3469:
	movw	r3, #3997
	mov	r2, r7
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3418
.L3577:
	ldr	r1, .L3600+40
	bl	dprint_vfmw
	mov	r0, r4
	bl	DirectOutput
	ldr	r3, [r5, #2444]
	ldr	r2, [r5, #3824]
	ldr	r1, .L3600+4
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
.L3418:
	cmn	r7, #1
	bne	.L3492
	b	.L3494
.L3424:
	cmp	r1, #1
	beq	.L3596
	cmp	r2, #0
	beq	.L3427
	ldrb	r1, [r9, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3427
	b	.L3420
.L3594:
	add	r7, r4, #24576
	ldr	ip, [r7, #2148]
	cmp	ip, #0
	beq	.L3465
	ldr	r3, [r7, #1952]
	cmp	r0, r3
	addne	r3, r4, #26368
	addne	r3, r3, #160
	beq	.L3466
.L3470:
	add	r1, r1, #1
	cmp	r1, ip
	beq	.L3465
	ldr	r2, [r3, #4]!
	cmp	r0, r2
	bne	.L3470
.L3466:
	add	r2, r4, #58880
	mov	r0, r4
	add	r2, r2, #200
	bl	InsertFrmInDPB
	cmn	r0, #1
	mov	r7, r0
	beq	.L3597
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	movw	r3, #4017
	mov	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3455
.L3482:
	mov	r3, r0
	str	r0, [fp, #-64]
	movw	r2, #4116
	ldr	r1, .L3600+16
	mov	r0, #1
	bl	dprint_vfmw
	ldr	ip, [fp, #-64]
	movw	r3, #4117
	ldr	r1, .L3600+4
	mov	r0, #14
	mov	r2, ip
	bl	dprint_vfmw
	b	.L3494
.L3593:
	movw	r2, #4153
	ldr	r1, .L3600+16
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #4154
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3440:
	ldr	r3, [r6, #1200]
	add	r3, r3, #1
	str	r3, [r6, #1200]
	b	.L3441
.L3587:
	ldr	r3, [r7, #2148]
	mov	r0, r4
	add	r3, r3, #1
	str	r3, [r7, #2148]
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	mov	r2, r8
	mov	r3, #3936
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3455
.L3474:
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
	b	.L3473
.L3584:
	ldrd	r0, [ip, #-8]
	mvn	r3, #0
	mvn	r2, #0
	cmp	r1, r3
	cmpeq	r0, r2
	addne	r1, r8, #608
	bne	.L3434
	add	r3, r4, #60416
	ldrd	r2, [r3, #-8]
	cmp	r3, r1
	add	r1, r8, #608
	cmpeq	r2, r0
	bne	.L3567
	b	.L3434
.L3595:
	mov	r3, r0
	movw	r2, #3987
	ldr	r1, .L3600+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3469
.L3581:
	ldr	r1, .L3600+44
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3600+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3458
	ldr	r7, [r5, #2420]
	mov	r3, #8
	ldr	lr, [r6, #1136]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r6, #1144]
	str	r7, [fp, #-52]
	str	lr, [fp, #-48]
	blx	ip
.L3458:
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	mov	r3, #3952
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3590:
	ldr	r3, [r6, #1072]
	cmp	r3, #2
	beq	.L3598
.L3477:
	mov	r0, r4
	bl	DirectOutput
	cmn	r0, #1
	mov	r7, r0
	beq	.L3599
	cmp	r0, #1
	ldreq	r2, [r5, #2376]
	moveq	r3, #0
	streqb	r3, [r2, #4]
	ldreq	r2, [r5, #2376]
	streqb	r3, [r2, #1]
.L3479:
	mov	r3, #4096
	mov	r2, r7
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3418
.L3591:
	mov	r3, r9
	ldr	r2, .L3600+48
	ldr	r1, .L3600+52
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #4103
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3588:
	mov	r1, r8
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L3443
.L3450:
	mov	r0, r4
	bl	IDRMemMarking
	cmp	r0, #0
	beq	.L3454
	mvn	r0, #0
	b	.L3564
.L3582:
	mov	r2, #3
	ldr	r3, [r9, #664]
	strb	r2, [r9, #1]
	ldr	r2, [r7, #684]
	add	r3, r3, r2
	str	r3, [r9, #664]
	b	.L3431
.L3596:
	add	r7, r4, #58880
	ldr	r9, [r5, #3088]
	add	r7, r7, #200
	b	.L3429
.L3598:
	ldrb	r3, [r5, #1739]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r5, #2376]
	strne	r3, [r5, #3192]
	b	.L3477
.L3597:
	mov	r3, r0
	movw	r2, #4010
	ldr	r1, .L3600+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, r7
	movw	r3, #4011
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3583:
	movw	r2, #3819
	ldr	r1, .L3600+56
	bl	dprint_vfmw
	movw	r3, #3820
	mvn	r2, #0
	ldr	r1, .L3600+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3494
.L3599:
	mov	r3, r0
	movw	r2, #4087
	ldr	r1, .L3600+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3479
.L3601:
	.align	2
.L3600:
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
	beq	.L3603
	ldrb	r3, [r8, #210]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3667
	add	r6, r0, #61440
	ldr	r3, [r6, #1072]
	cmp	r3, #2
	beq	.L3662
	add	r5, r7, #57344
	mov	r2, #0
	ldrb	r3, [r5, #3160]	@ zero_extendqisi2
	strb	r2, [r5, #1737]
	cmp	r3, #1
	mov	r3, #2
	str	r3, [fp, #-52]
	bne	.L3633
.L3629:
	ldr	r2, [r5, #3168]
	ldr	r3, [r8, #228]
	cmp	r2, r3
	bne	.L3633
	ldr	r3, [r8, #240]
	cmp	r3, #0
	ldr	r3, [r5, #3164]
	beq	.L3668
	cmp	r3, #0
	beq	.L3633
.L3609:
	ldr	r3, [r6, #1184]
	ldr	r2, [r6, #1180]
	rsb	r3, r2, r3
	cmp	r3, #2
	bhi	.L3633
	ldr	r1, [r5, #2376]
	mov	r3, #1
	strb	r3, [r5, #1737]
	cmp	r1, #0
	beq	.L3611
	ldr	r3, [fp, #-52]
	cmp	r3, #2
	beq	.L3669
	ldr	r3, [fp, #-52]
	cmp	r3, #1
	beq	.L3670
.L3636:
	mov	ip, #0
	b	.L3615
.L3603:
	add	r6, r0, #61440
	ldr	r2, [r6, #1072]
	cmp	r2, #2
	strne	r3, [fp, #-52]
	addne	r5, r0, #57344
	strneb	r3, [r5, #1737]
	beq	.L3662
.L3633:
	ldr	r4, [r5, #3820]
	mov	lr, #0
	strb	lr, [r5, #1736]
	cmp	r4, lr
	beq	.L3623
	add	ip, r7, #27392
	add	r10, r7, #26368
	add	ip, ip, #8
	add	r10, r10, #160
	add	r9, r7, #24576
.L3622:
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3618
	cmp	ip, #0
	beq	.L3638
	ldr	r1, [r9, #2148]
	cmp	r1, #0
	beq	.L3638
	ldr	r2, [r9, #1952]
	cmp	r2, ip
	beq	.L3618
	mov	r2, r10
	b	.L3620
.L3621:
	ldr	r0, [r2, #4]!
	cmp	r0, ip
	beq	.L3618
.L3620:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3621
.L3638:
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
	bne	.L3623
	ldr	r3, [fp, #-56]
	ldr	r0, [r6, #1144]
	clz	r1, r3
	mov	r1, r1, lsr #5
	bl	FSP_NewLogicFs
	mov	r1, r0
	str	r0, [fp, #-48]
	ldr	r0, [r6, #1144]
	bl	FSP_GetLogicFs
	ldr	r3, [fp, #-48]
	mvn	r2, r3
	mov	r2, r2, lsr #31
	cmp	r0, #0
	mov	r9, r0
	moveq	r2, #0
	cmp	r2, #0
	beq	.L3624
	ldr	r2, [r7]
	ldr	r2, [r2, #28]
	cmp	r2, #25
	beq	.L3671
.L3625:
	ldr	r4, .L3674
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
.L3615:
	ldrb	r2, [fp, #-52]	@ zero_extendqisi2
	strb	r2, [r5, #1739]
	ldrb	r3, [r8, #209]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	strb	r3, [r1, #676]
	ldrb	r3, [r5, #1737]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3672
	ldr	r3, [fp, #-52]
	strb	r2, [r5, #3160]
	cmp	r3, #0
	ldr	r3, [r8, #228]
	ldreq	r0, [fp, #-52]
	movne	r0, #0
	str	r3, [r5, #3168]
	ldr	r3, [r8, #240]
	str	r3, [r5, #3164]
	ldr	r3, [r6, #1184]
	str	ip, [r5, #3180]
	str	r3, [r5, #3176]
	movne	r3, #1
	moveq	r3, r0
	strb	r3, [r5, #3161]
.L3658:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3618:
	add	lr, lr, #1
	add	ip, ip, #792
	cmp	lr, r4
	bne	.L3622
.L3623:
	ldr	r1, .L3674+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r7
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3667:
	add	r6, r0, #61440
	ldr	r2, [r6, #1072]
	cmp	r2, #2
	beq	.L3673
	add	r5, r7, #57344
	ldrb	r2, [r5, #3160]	@ zero_extendqisi2
	strb	r3, [r5, #1737]
	mov	r3, #1
	cmp	r2, #2
	str	r3, [fp, #-52]
	bne	.L3633
	b	.L3629
.L3673:
	mov	r3, #1
.L3662:
	add	r5, r7, #57344
	str	r3, [fp, #-52]
.L3664:
	mov	r3, #0
	strb	r3, [r5, #1737]
	b	.L3633
.L3671:
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
	b	.L3625
.L3672:
	mov	r3, #0
	strb	r3, [r5, #3160]
	ldr	r2, [r8, #228]
	mov	r0, r3
	str	r2, [r5, #3168]
	ldr	r2, [r8, #240]
	str	r2, [r5, #3164]
	ldr	r2, [r6, #1184]
	strb	r3, [r5, #3161]
	str	ip, [r5, #3180]
	str	r2, [r5, #3176]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3668:
	cmp	r3, #0
	beq	.L3609
	b	.L3633
.L3624:
	ldr	r3, [r5, #2376]
	mov	r0, r2
	strb	r2, [r5, #1736]
	ldr	r1, .L3674+8
	strb	r2, [r3, #4]
	bl	dprint_vfmw
	mov	r1, r4
	mov	r0, r7
	bl	ClearAll
	mvn	r0, #0
	b	.L3658
.L3670:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3664
.L3614:
	ldr	r1, [r5, #2376]
	cmp	r1, #0
	bne	.L3636
	b	.L3633
.L3669:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3664
	b	.L3614
.L3611:
	strb	r1, [r5, #1737]
	b	.L3633
.L3675:
	.align	2
.L3674:
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
	beq	.L3692
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
	ldr	r1, .L3695
	bl	dprint_vfmw
	mov	r3, r9
	ldr	r2, [fp, #-60]
	mov	r0, #2
	ldr	r1, .L3695+4
	bl	dprint_vfmw
	sub	r2, fp, #52
	sub	r1, fp, #48
	mov	r0, r6
	bl	GetMaxPOC
	ldr	r3, [fp, #-60]
	cmp	r3, r9
	beq	.L3684
	mov	r3, r10
	str	r5, [fp, #-64]
	mov	r10, r4
	mov	r5, r7
	mov	r4, r3
	b	.L3685
.L3682:
	mov	r1, r7
	mov	r0, r6
	strb	r7, [r10, #1740]
	bl	StorePicInDPB
	ldr	r1, [fp, #-80]
	subs	r3, r0, #0
	add	r0, r9, #1
	bne	.L3693
	ldr	r3, [fp, #-76]
	str	r9, [r3, #224]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-60]
	cmp	r3, r1
	mov	r9, r1
	beq	.L3691
	ldr	r3, [r4, #2148]
	ldr	r2, [fp, #-64]
	cmp	r2, r3
	bhi	.L3691
.L3685:
	mov	r1, #1
	mov	r0, r6
	bl	AllocFrameStore
	mov	r7, #0
	ldr	r1, .L3695+8
	subs	r3, r0, #0
	mov	r0, #2
	bne	.L3694
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
	beq	.L3682
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
	bls	.L3682
	cmp	r1, r3
	ldrcc	r3, [fp, #-64]
	addcc	r3, r3, #1
	strcc	r3, [fp, #-64]
	b	.L3682
.L3692:
	ldr	r1, .L3695+12
	mov	r0, r7
	bl	dprint_vfmw
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3691:
	mov	r4, r10
.L3684:
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
.L3694:
	ldr	r3, [fp, #-84]
	mov	r0, r7
	ldrb	r2, [fp, #-92]	@ zero_extendqisi2
	ldr	r1, .L3695+16
	str	r3, [r8, #3468]
	ldr	r3, [fp, #-88]
	str	r3, [r8, #3472]
	ldr	r3, [r10, #2380]
	strb	r2, [r3, #3]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3693:
	ldr	r2, [fp, #-84]
	mov	r0, r7
	ldrb	lr, [fp, #-92]	@ zero_extendqisi2
	ldr	r1, .L3695+20
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
.L3696:
	.align	2
.L3695:
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
	ldr	r8, .L3710
	mov	r5, r0
	ldr	r4, [r0, #352]
	mov	r7, r1
	movw	r2, #62664
	mov	r1, #0
	ldr	r3, [r8, #48]
	blx	r3
	str	r4, [r5, #352]
	add	r0, r5, #8
	bl	ResetVoQueue
	mov	r3, #0
	str	r7, [r5]
	str	r3, [r5, #360]
	str	r3, [r5, #364]
	ldr	r3, [r7, #28]
	cmp	r3, #25
	beq	.L3707
	add	r6, r5, #12288
	mov	r1, #32
	mov	r2, #256
	mov	r3, #136
	str	r1, [r6, #1288]
	str	r2, [r6, #1292]
	str	r3, [r6, #1284]
.L3699:
	ldr	r0, [r7, #584]
	add	r4, r5, #61440
	bl	MEM_Phy2Vir
	ldr	lr, [r6, #1288]
	ldr	r3, [r5]
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
	str	r0, [r4, #1064]
	bne	.L3708
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
	beq	.L3702
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
	ldr	r0, [r4, #1064]
.L3702:
	add	r3, r5, #16384
	movw	r2, #4060
	ldr	ip, [r8, #48]
	str	r1, [r3, #1236]
	mov	r1, #0
	ldr	r3, [r6, #1284]
	mov	r7, r1
	str	lr, [r6, #1344]
	mul	r2, r2, r3
	blx	ip
	add	r3, r5, #57344
	str	r7, [r4, #1088]
	mov	ip, #40
	str	r7, [r4, #1092]
	mov	r1, #17
	str	r7, [r4, #1096]
	mov	r2, #25
	str	r7, [r4, #1100]
	mov	r0, r5
	str	r7, [r4, #1104]
	str	r7, [r4, #1108]
	str	r7, [r4, #1112]
	str	r7, [r4, #1116]
	str	ip, [r3, #3820]
	str	r1, [r3, #3824]
	str	r7, [r4, #1080]
	str	r7, [r3, #3848]
	str	r7, [r3, #3852]
	str	r7, [r3, #3856]
	str	r7, [r3, #3860]
	str	r7, [r3, #3864]
	str	r7, [r3, #3828]
	str	r7, [r3, #3836]
	str	r7, [r4, #1176]
	str	r7, [r3, #3840]
	str	r2, [r3, #3844]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #1144]
	beq	.L3709
	mov	r1, r7
	mov	r0, r5
	bl	ClearAll
	str	r7, [r4, #1172]
	ldr	r3, [r5]
	mov	r0, #1
	ldr	r3, [r3, #624]
	str	r3, [r4, #1148]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3707:
	ldr	r3, [r7, #684]
	add	r6, r5, #12288
	str	r3, [r6, #1288]
	ldr	r3, [r7, #688]
	str	r3, [r6, #1292]
	ldr	r3, [r7, #676]
	str	r3, [r6, #1284]
	b	.L3699
.L3708:
	movw	r3, #13089
	ldr	r2, .L3710+4
	ldr	r1, .L3710+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3709:
	mov	r0, r7
	ldr	r1, .L3710+12
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3711:
	.align	2
.L3710:
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
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	add	r9, r0, #20480
	mov	r7, #2208
	add	r3, r0, #12288
	ldr	r2, [r9, #220]
	mov	r5, r0
	mov	r1, r3
	str	r3, [fp, #-60]
	ldr	r3, [r3, #1304]
	add	r0, r0, #61440
	mul	r7, r7, r2
	ldr	ip, [r0, #1156]
	ldr	r2, [r1, #1300]
	mov	r6, #4000
	cmp	ip, #1
	str	r0, [fp, #-56]
	add	r10, r3, r7
	ldr	r1, [r10, #28]
	mla	r6, r6, r1, r2
	beq	.L3851
	add	r4, r5, #57344
	ldrb	r3, [r4, #1736]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3716
	ldr	r3, [r4, #2376]
	cmp	r3, #0
	beq	.L3717
	ldrb	r7, [r3, #1]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L3852
.L3718:
	mov	r3, #0
	strb	r3, [r4, #1736]
.L3716:
	sub	r1, fp, #48
	mov	r0, r5
	bl	GetReRangeFlag
	ldr	r3, [fp, #-60]
	ldr	r2, [r3, #1344]
	ldr	r3, [r6, #36]
	cmp	r2, r3
	mov	r7, r0
	beq	.L3853
.L3719:
	ldr	r3, .L3867
	add	r0, r5, #13568
	mov	r2, #0
	add	r0, r0, #28
	strb	r2, [r6, #24]
	mov	r1, r6
	mov	r2, #4000
	ldr	r3, [r3, #56]
	blx	r3
.L3720:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r5]
	bne	.L3854
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L3722
.L3846:
	add	r7, r5, #16384
.L3731:
	ldr	r2, [r7, #1236]
	ldr	r3, [r10, #24]
	cmp	r2, r3
	bne	.L3743
	ldrb	r3, [r10, #20]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3743
.L3744:
	ldr	r3, [r7, #3444]
	ldr	r2, [fp, #-60]
	cmp	r3, #0
	ldr	ip, [r2, #3504]
	movne	r3, #0
	strne	r3, [r9, #224]
	strne	r3, [r4, #3168]
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #1072]
	cmp	r3, #2
	beq	.L3746
	ldr	r3, [r5]
	ldr	r2, [r3, #12]
	str	r3, [fp, #-64]
	sub	r2, r2, #2
	cmp	r2, #1
	bls	.L3746
	ldr	r2, [r9, #228]
	ldr	r10, [r9, #224]
	cmp	r2, r10
	str	r2, [fp, #-68]
	beq	.L3747
	mov	r1, #1
	add	ip, ip, #4
	mov	ip, r1, asl ip
	add	r0, r10, r1
	str	ip, [fp, #-72]
	mov	r1, ip
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-68]
	cmp	r2, r1
	beq	.L3787
	ldr	r3, [fp, #-64]
	ldr	r1, [r3, #600]
	cmp	r1, #0
	bne	.L3748
	ldr	r1, [r3, #640]
	ldr	ip, [fp, #-72]
	cmp	r1, #1
	beq	.L3855
.L3749:
	cmp	r2, r10
	bhi	.L3856
	rsb	r3, r10, r2
	ldr	r1, [r7, #484]
	add	r3, r3, ip
	cmp	r1, r3
	bcs	.L3748
	ldrb	r3, [r9, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3758
.L3763:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #1140]
	cmp	r3, #0
	bne	.L3748
	mov	r0, r5
	bl	H264_ClearCurrPic
	ldr	r3, .L3867+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3764
	mov	r0, r5
	bl	ClearDPB
.L3764:
	ldr	ip, [r7, #484]
	movw	r2, #6922
.L3847:
	ldr	r1, [r9, #228]
	mov	r0, #0
	ldr	r3, [r9, #224]
	str	ip, [sp]
	rsb	r3, r3, r1
	ldr	r1, .L3867+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3854:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L3722
	ldr	r3, [r3, #28]
	cmp	r3, #25
	bne	.L3722
	ldr	r1, [fp, #-56]
	cmp	r7, #0
	ldr	r3, [r1, #1104]
	add	r3, r3, #1
	str	r3, [r1, #1104]
	beq	.L3723
	mov	r0, r5
	str	r2, [fp, #-64]
	bl	FlushDPB
	ldr	r2, [fp, #-64]
	subs	r3, r0, #0
	bne	.L3857
	mov	r0, r5
	bl	InitDPB
	subs	r3, r0, #0
	bne	.L3780
	ldr	r2, [fp, #-60]
	add	r7, r5, #16384
	ldr	r1, [r7, #464]
	ldrb	r3, [r2, #1326]	@ zero_extendqisi2
	ldr	r0, [r2, #1276]
	rsb	r2, r3, #2
	add	r3, r1, #1
	ldr	ip, [r7, #460]
	cmp	r0, #0
	mov	r3, r3, asl #4
	add	ip, ip, #1
	mul	r3, r2, r3
	mov	ip, ip, asl #4
	str	r3, [fp, #-64]
	bne	.L3729
	ldr	r0, [r7, #484]
	str	r0, [r4, #3824]
	ldr	r3, [r5]
	ldr	r1, [r3, #668]
	cmp	r1, ip
	bcc	.L3725
	ldr	r2, [r3, #672]
	ldr	lr, [fp, #-64]
	cmp	r2, lr
	bcc	.L3725
	add	lr, r5, #24576
	ldr	r8, [r3, #692]
	ldr	lr, [lr, #2144]
	cmp	r8, lr
	bcc	.L3858
	ldr	lr, [r3, #696]
	str	ip, [fp, #-68]
	cmp	lr, #0
	moveq	r0, #1
	streq	r0, [r4, #3824]
	ldreq	r1, [r3, #668]
	ldreq	r2, [r3, #672]
	add	r3, r5, #60928
	str	r0, [sp]
	add	r3, r3, #236
	mov	r0, r5
	bl	H264_ArrangeVahbMem
	ldr	ip, [fp, #-68]
	cmp	r0, #1
	beq	.L3729
.L3845:
	ldr	r3, [r5]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	mvneq	r0, #1
	beq	.L3839
	ldr	r3, [fp, #-60]
	mov	r4, #0
	ldr	r1, .L3867+12
	mov	r0, r4
	str	r4, [r3, #1276]
	str	r4, [r3, #1280]
	bl	dprint_vfmw
	mov	r1, r4
	mov	r0, r5
	bl	ClearAll
	mvn	r0, #0
	b	.L3839
.L3853:
	ldrb	r3, [r6, #24]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3720
	b	.L3719
.L3722:
	ldr	r2, [fp, #-56]
	cmp	r7, #0
	ldr	r3, [r2, #1104]
	add	r3, r3, #1
	str	r3, [r2, #1104]
	bne	.L3732
	ldr	r3, [r5]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L3732
	mov	r0, r5
	bl	FlushDPB
	cmp	r0, #0
	bne	.L3859
	mov	r0, r5
	bl	InitDPB
	cmp	r0, #0
	beq	.L3737
.L3784:
	mov	r3, r0
	movw	r2, #6792
	ldr	r1, .L3867+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3732:
	ldr	r1, .L3867+20
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #1212]
	cmp	r3, #1
	beq	.L3860
.L3734:
	mov	r0, r5
	bl	InitDPB
	cmp	r0, #0
	bne	.L3784
	cmp	r7, #0
	bne	.L3740
.L3737:
	ldr	r3, [r5]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	bne	.L3846
.L3740:
	add	r7, r5, #16384
	ldr	r1, [fp, #-60]
	add	r3, r5, #60928
	mov	r0, r5
	ldr	ip, [r7, #484]
	add	r3, r3, #236
	str	ip, [r4, #3824]
	ldr	r2, [r7, #464]
	ldrb	r1, [r1, #1326]	@ zero_extendqisi2
	add	r2, r2, #1
	ldr	lr, [r7, #460]
	rsb	r1, r1, #2
	str	ip, [sp]
	mov	r2, r2, asl #4
	add	ip, lr, #1
	mul	r2, r1, r2
	mov	ip, ip, asl #4
	str	ip, [fp, #-64]
	mov	r1, ip
	mov	r8, r2
	bl	H264_ArrangeVahbMem
	ldr	ip, [fp, #-64]
	cmp	r0, #1
	bne	.L3845
	ldr	r2, [fp, #-60]
	mov	ip, ip, lsr #4
	mov	r3, r8, lsr #4
	str	ip, [r2, #1276]
	str	r3, [r2, #1280]
	b	.L3731
.L3743:
	ldr	r3, .L3867
	add	r0, r5, #17408
	mov	r2, #0
	mov	r1, r10
	strb	r2, [r10, #20]
	add	r0, r0, #188
	ldr	r3, [r3, #56]
	mov	r2, #2208
	blx	r3
	b	.L3744
.L3787:
	mov	r10, r2
.L3747:
	ldr	r3, [r9, #240]
	mov	r0, r5
	cmp	r3, #0
	strne	r10, [r9, #224]
	str	r10, [r7, #3484]
	bl	DecPOC
	mov	r1, #0
	mov	r0, r5
	bl	AllocFrameStore
	cmp	r0, #0
	bne	.L3861
	strb	r0, [r4, #1743]
	mov	r1, #1
	strb	r1, [r4, #1736]
	mvn	r3, #0
	ldrb	r2, [r9, #211]	@ zero_extendqisi2
	strb	r2, [r4, #1742]
	ldr	ip, [r9, #228]
	str	ip, [r4, #2384]
	ldr	r2, [r9, #240]
	str	ip, [fp, #-64]
	str	r2, [r4, #2392]
	ldr	lr, [r7, #3464]
	str	r2, [fp, #-68]
	str	lr, [r4, #2396]
	ldr	lr, [r7, #3460]
	str	lr, [r4, #2400]
	ldr	lr, [r7, #3452]
	str	lr, [r4, #2404]
	ldr	lr, [r7, #3456]
	strb	r0, [r4, #1738]
	strb	r0, [r4, #1741]
	str	lr, [r4, #2408]
	strb	r0, [r4, #1740]
	str	r3, [r4, #2440]
	ldrb	r3, [r9, #209]	@ zero_extendqisi2
	cmp	r3, #0
	strneb	r1, [r9, #209]
	ldr	lr, [r7, #464]
	movne	r3, r1
	str	r3, [fp, #-72]
	ldr	r3, [fp, #-60]
	movne	r1, #2
	ldrb	r0, [r3, #1326]	@ zero_extendqisi2
	rsb	r10, r0, #2
	mla	r10, lr, r10, r10
	mov	r0, r10
	bl	__aeabi_uidiv
	str	r10, [r4, #2432]
	ldr	r3, [fp, #-72]
	ldr	ip, [fp, #-64]
	cmp	r3, #0
	ldr	r2, [fp, #-68]
	movne	r3, #0
	str	r0, [r4, #2428]
	ldr	lr, [r7, #460]
	add	r1, lr, #1
	str	r1, [r4, #2424]
	mul	r1, r1, r0
	str	r1, [r4, #2436]
	ldrb	r1, [r7, #1212]	@ zero_extendqisi2
	strb	r1, [r4, #1744]
	movne	r1, r3
	bne	.L3769
	ldr	r1, [fp, #-60]
	ldrb	r1, [r1, #1327]	@ zero_extendqisi2
	cmp	r1, #0
	movne	r3, #1
	movne	r1, r3
.L3769:
	cmp	r2, #0
	ldr	r2, [fp, #-56]
	strb	r1, [r4, #1745]
	add	r1, r5, #24576
	add	r10, r5, #59648
	mov	r0, #29
	str	r3, [r2, #1052]
	movw	r3, #2004
	strne	ip, [r9, #224]
	movw	r2, #59696
	ldr	lr, [r1, #1812]
	movw	ip, #59712
	ldr	r1, .L3867+24
	mla	r3, r3, lr, r5
	add	r3, r3, #22272
	add	r3, r3, #108
	str	r3, [r4, #2380]
	ldr	r3, [r5]
	ldrd	r8, [r3, #56]
	strd	r8, [r5, r2]
	ldrd	r8, [r3, #72]
	strd	r8, [r10, #56]
	ldrd	r8, [r3, #88]
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
	ldr	ip, [r6, #44]
	str	ip, [r4, #1928]
	ldr	ip, [r6, #48]
	str	ip, [r4, #1932]
	strd	r8, [r3, #56]
	ldrd	r2, [r2, r5]
	bl	dprint_vfmw
	ldrd	r2, [r10, #56]
	ldr	r1, .L3867+28
	mov	r0, #29
	bl	dprint_vfmw
	ldr	r3, [r7, #460]
	ldr	ip, [fp, #-60]
	add	r0, r5, #58880
	add	r3, r3, #1
	add	r0, r0, #216
	mov	r3, r3, asl #4
	str	r3, [r4, #1864]
	ldrb	r2, [ip, #1326]	@ zero_extendqisi2
	ldr	r1, [r7, #464]
	rsb	r3, r2, #2
	mla	r3, r1, r3, r3
	mov	r3, r3, asl #4
	str	r3, [r4, #1868]
	ldr	r2, [ip, #1348]
	ldr	r3, [r7, #532]
	subs	r2, r2, #1
	ldr	r1, [r7, #528]
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
	ldr	r1, [fp, #-56]
	adds	r3, r3, #0
	ldr	r2, [r4, #2376]
	ldr	r1, [r1, #1104]
	movne	r3, #1
	cmp	r2, #0
	str	r3, [r4, #1848]
	str	r1, [r4, #1916]
	beq	.L3862
	ldr	r3, [r4, #1860]
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
	ldr	r3, [r6, #3284]
	ldr	r2, [r4, #2376]
	str	r3, [r2, #136]
	str	r3, [r4, #1880]
	ldr	r2, [r4, #2376]
	ldr	r3, [r6, #3288]
	str	r3, [r2, #140]
	str	r3, [r4, #1884]
	ldr	r2, [r4, #2376]
	ldr	r3, [r6, #3276]
	str	r3, [r2, #128]
	str	r3, [r4, #1872]
	ldr	r2, [r4, #2376]
	ldr	r3, [r6, #3280]
	str	r3, [r2, #132]
	str	r3, [r4, #1876]
	ldr	r3, [r4, #2376]
	ldr	r6, [fp, #-56]
	ldr	r1, [r3, #632]
	ldr	r0, [r6, #1144]
	bl	FSP_GetDispPhyFs
	cmp	r0, #0
	beq	.L3772
	ldr	r3, [r4, #1848]
	mov	r1, #0
	ldr	r2, [r0, #8]
	subs	r3, r3, r1
	ldr	r0, [r6, #1144]
	movne	r3, #1
	bl	FSP_SetStoreType
.L3772:
	add	lr, r5, #59648
	add	r6, r5, #61184
	ldr	r3, [r4, #3848]
	add	lr, lr, #32
	ldr	r8, [fp, #-56]
	add	r6, r6, #12
	add	ip, r5, #59648
	mov	r7, #0
.L3776:
	ldrd	r0, [ip, #8]!
	orrs	r2, r0, r1
	bne	.L3773
	cmp	r3, #0
	beq	.L3773
	ldr	r2, [r4, #3852]
	subs	r3, r3, #1
	stmia	ip, {r2, r7}
	beq	.L3774
	mov	r1, r6
	mov	r2, #0
.L3775:
	add	r2, r2, #1
	ldr	r0, [r1, #4]!
	cmp	r2, r3
	str	r0, [r1, #-4]
	bne	.L3775
.L3774:
	add	r2, r3, #15296
	add	r2, r2, #2
	add	r2, r5, r2, lsl #2
	str	r7, [r2, #4]
	str	r3, [r4, #3848]
.L3773:
	cmp	ip, lr
	bne	.L3776
	cmp	r3, #0
	str	r8, [fp, #-56]
	beq	.L3778
	mov	r7, r3
	ble	.L3778
	add	r6, r3, #15296
	ldr	r8, [fp, #-56]
	add	r6, r6, #3
	mov	r9, #0
	add	r6, r5, r6, lsl #2
.L3779:
	ldr	r1, [r6, #-4]!
	sub	r7, r7, #1
	ldr	r0, [r8, #1144]
	bl	FreeUsdByDec
	cmp	r7, #0
	str	r9, [r6]
	bne	.L3779
.L3778:
	mov	r0, r5
	mov	r5, #0
	str	r5, [r4, #3848]
	str	r5, [r4, #2412]
	bl	CalcPicNum
	mov	r0, r5
.L3839:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3758:
	cmp	r2, #0
	mov	r0, r5
	subeq	r2, ip, #1
	subne	r2, r2, #1
	str	r2, [r9, #224]
	bl	FlushDPB
	subs	r3, r0, #0
	bne	.L3863
	ldrb	r3, [r9, #208]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3763
.L3748:
	mov	r0, r5
	bl	DecGap
	cmp	r0, #0
	bne	.L3864
.L3746:
	ldr	r10, [r9, #228]
	b	.L3747
.L3851:
	ldrb	r0, [r6, #18]	@ zero_extendqisi2
	add	r4, r5, #57344
	ldr	r2, [r6, #3252]
	ldrb	r5, [r9, #209]	@ zero_extendqisi2
	rsb	r0, r0, #2
	str	r3, [fp, #-64]
	add	r1, r5, #1
	str	ip, [fp, #-60]
	mla	r0, r2, r0, r0
	bl	__aeabi_uidiv
	ldr	r3, [fp, #-64]
	cmp	r5, #0
	ldr	ip, [fp, #-60]
	str	r0, [r4, #2428]
	ldrb	r2, [r6, #18]	@ zero_extendqisi2
	ldr	lr, [r6, #3252]
	rsb	r1, r2, #2
	mla	r1, lr, r1, r1
	str	r1, [r4, #2432]
	ldr	r1, [r6, #3248]
	add	r2, r1, #1
	str	r2, [r4, #2424]
	mul	r2, r2, r0
	str	r2, [r4, #2436]
	ldrb	r3, [r3, r7]	@ zero_extendqisi2
	strb	r3, [r4, #1744]
	beq	.L3865
	mov	ip, #0
.L3843:
	mov	r3, ip
.L3714:
	strb	r3, [r4, #1745]
	mov	r0, #0
	ldr	r3, [fp, #-56]
	str	ip, [r3, #1052]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3865:
	ldrb	r3, [r6, #19]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	ip, r3
	beq	.L3714
	b	.L3843
.L3852:
	ldr	r1, [r3, #632]
	mov	r2, #1
	ldr	r3, [fp, #-56]
	ldr	r0, [r3, #1144]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #2376]
	strb	r7, [r3, #1]
	ldr	r3, [r4, #2376]
	strb	r7, [r3, #4]
	b	.L3718
.L3856:
	ldr	r3, [r7, #484]
	rsb	r10, r10, r2
	cmp	r3, r10
	bcs	.L3751
	ldrb	r3, [r9, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3752
.L3756:
	ldr	r3, [fp, #-56]
	ldr	r3, [r3, #1140]
	cmp	r3, #0
	bne	.L3748
	mov	r0, r5
	bl	H264_ClearCurrPic
	ldr	r3, .L3867+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3866
.L3757:
	ldr	ip, [r7, #484]
	movw	r2, #6890
	b	.L3847
.L3723:
	mov	r0, r5
	bl	InitDPB
	subs	r3, r0, #0
	bne	.L3780
	ldr	r3, [fp, #-60]
	add	r7, r5, #16384
	ldr	r1, [r7, #464]
	ldrb	r3, [r3, #1326]	@ zero_extendqisi2
	ldr	ip, [r7, #460]
	rsb	r2, r3, #2
	add	r3, r1, #1
	add	ip, ip, #1
	mov	r3, r3, asl #4
	mov	ip, ip, asl #4
	mul	r3, r2, r3
	str	r3, [fp, #-64]
.L3729:
	ldr	r2, [fp, #-60]
	mov	ip, ip, lsr #4
	ldr	r3, [fp, #-64]
	str	ip, [r2, #1276]
	mov	r3, r3, lsr #4
	str	r3, [r2, #1280]
	b	.L3731
.L3864:
	movw	r2, #6934
	ldr	r1, .L3867+32
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3860:
	mov	r0, r5
	bl	GetBackPicFromVOQueue
	b	.L3734
.L3717:
	movw	r3, #6623
	ldr	r2, .L3867+36
	ldr	r1, .L3867+40
	mov	r0, #22
	bl	dprint_vfmw
	b	.L3718
.L3751:
	ldrb	r3, [r9, #208]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3756
	b	.L3748
.L3855:
	ldr	r1, [r3, #636]
	cmp	r1, #0
	bne	.L3749
	ldr	r1, [r3, #644]
	cmp	r1, #0
	bne	.L3749
	ldr	r3, [r3, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	bhi	.L3748
	b	.L3749
.L3861:
	movw	r2, #6958
	ldr	r1, .L3867+44
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3752:
	cmp	r2, #0
	mov	r0, r5
	subeq	r2, ip, #1
	subne	r2, r2, #1
	str	r2, [r9, #224]
	bl	FlushDPB
	subs	r3, r0, #0
	beq	.L3751
	movw	r2, #6873
	ldr	r1, .L3867+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3862:
	movw	r3, #7045
	ldr	r2, .L3867+36
	ldr	r1, .L3867+52
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3859:
	mov	r3, r0
	movw	r2, #6783
	mov	r0, r7
	ldr	r1, .L3867+48
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3780:
	movw	r2, #6666
	ldr	r1, .L3867+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3725:
	ldr	r1, .L3867+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3857:
	mov	r0, r2
	ldr	r1, .L3867+48
	movw	r2, #6657
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3866:
	mov	r0, r5
	bl	ClearDPB
	b	.L3757
.L3863:
	movw	r2, #6906
	ldr	r1, .L3867+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3858:
	ldr	r1, .L3867+60
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3839
.L3868:
	.align	2
.L3867:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_apple_airplay_mirrorinig_flag
	.word	.LC395
	.word	.LC393
	.word	.LC390
	.word	.LC394
	.word	.LC398
	.word	.LC399
	.word	.LC396
	.word	.LANCHOR1+284
	.word	.LC389
	.word	.LC397
	.word	.LC354
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
	ldr	r1, .L3923
	ldr	r2, [r5, #1088]
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
	bne	.L3917
	mov	r0, r4
	bl	ProcessSliceHeaderSecondPart
	cmp	r0, #0
	bne	.L3917
	ldr	r2, [r5, #1068]
	cmp	r2, #0
	moveq	r3, r2
	beq	.L3876
	add	r9, r4, #60416
	mov	r3, r0
	add	r9, r9, #132
	mov	r1, r0
	mov	r8, r0
	b	.L3884
.L3918:
	cmp	r7, r1
	add	r3, ip, r3
	rsbhi	r0, r0, r10
	rsbhi	r1, r1, r7
	addhi	r0, r1, r0
	movls	r0, r10
	add	r3, r3, r0
.L3882:
	cmp	lr, #0
	add	r8, r8, #1
	movne	r1, lr
	movne	r0, ip
	moveq	r1, r7
	moveq	r0, r10
	cmp	r8, r2
	beq	.L3876
.L3884:
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
	bne	.L3918
	add	r3, r10, r3
	add	r3, r3, ip
	b	.L3882
.L3876:
	add	r7, r4, #57344
	str	r3, [r5, #1128]
	movw	r0, #1620
	ldr	ip, [r7, #2428]
	ldr	r1, [r7, #2424]
	mul	r1, r1, ip
	cmp	r1, r0
	movgt	r1, r1, asl #8
	ble	.L3919
.L3886:
	add	r8, r4, #12288
	ldr	r0, [r8, #1284]
	cmp	r0, r2
	bls	.L3887
	cmp	r1, r3
	bcc	.L3887
	ldrb	r3, [r6, #213]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3890
	ldr	r1, .L3923+4
	mov	r0, #2
	bl	dprint_vfmw
	ldrb	r3, [r6, #211]	@ zero_extendqisi2
	mov	r2, #0
	str	r2, [r5, #1124]
	cmp	r3, #5
	ldr	r3, [r5, #1108]
	mov	r0, r4
	streq	r3, [r5, #1120]
	add	r3, r3, #1
	str	r3, [r5, #1108]
	ldrb	r3, [r7, #1739]	@ zero_extendqisi2
	cmp	r3, r2
	ldreq	r3, [r5, #1112]
	ldrne	r3, [r5, #1116]
	addeq	r3, r3, #1
	addne	r3, r3, #1
	streq	r3, [r5, #1112]
	strne	r3, [r5, #1116]
	bl	InitPic
	cmn	r0, #2
	beq	.L3873
	cmp	r0, #0
	bne	.L3920
	mov	r0, r4
	bl	WritePicMsg
.L3890:
	mov	r0, r4
	bl	PicTypeStatistic
	ldr	r3, [r5, #1156]
	cmp	r3, #0
	bne	.L3896
	mov	r0, r4
	bl	DecList
	cmp	r0, #0
	bne	.L3921
	mov	r0, r4
	bl	IsRefListWrong
	cmp	r0, #0
	bne	.L3922
	mov	r0, r4
	bl	ExitSlice
.L3896:
	mov	r0, r4
	bl	CalcStreamBits
	mov	r0, r4
	bl	WriteSliceMsg
	mov	r3, #1
	str	r3, [r7, #3828]
	mov	r0, #0
	ldr	r3, [r5, #1124]
	add	r3, r3, #1
	str	r3, [r5, #1124]
.L3873:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3919:
	mov	r0, r1, asl #9
	sub	r1, r0, r1, asl #7
	cmp	r1, #16384
	movlt	r1, #16384
	b	.L3886
.L3917:
	ldr	r1, .L3923+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L3923+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3915
	sub	r2, fp, #44
	mvn	r1, #0
	ldr	r0, [r5, #1144]
	mov	r3, #4
	str	r1, [r2, #-8]!
	mov	r1, #100
	blx	ip
.L3915:
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	b	.L3873
.L3887:
	str	r1, [sp]
	mov	r0, #0
	ldr	r1, .L3923+16
	bl	dprint_vfmw
	ldr	r2, [r8, #1284]
	ldr	r3, [r5, #1068]
	cmp	r3, r2
	bcc	.L3889
	ldr	r1, .L3923+12
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L3889
	str	r3, [fp, #-52]
	mov	r1, #108
	str	r2, [fp, #-48]
	mov	r3, #8
	sub	r2, fp, #52
	ldr	r0, [r5, #1144]
	blx	ip
.L3889:
	ldr	r1, .L3923+20
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3873
.L3921:
	mov	r2, r0
	ldr	r1, .L3923+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3873
.L3922:
	ldr	r1, .L3923+28
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3915
.L3920:
	ldr	r1, .L3923+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3873
.L3924:
	.align	2
.L3923:
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
	ldrne	r3, [r4, #1128]
	beq	.L3930
.L3929:
	ldr	lr, [ip, #12]
	add	r2, r2, #1
	add	ip, ip, #28
	add	r3, r3, lr
	str	r3, [r4, #1128]
	ldr	lr, [r1, #68]
	cmp	lr, r2
	bhi	.L3929
.L3930:
	add	r8, r6, #13504
	mov	r9, #0
	add	r8, r8, #24
	strb	r9, [r1]
	mov	r1, #32
	mov	r0, r8
	bl	BsGet
	ldr	r2, [r6, #368]
	ldr	r1, .L4091
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
	str	r9, [r4, #1084]
	ldr	r3, [r6, #368]
	ldr	r2, [r4, #1088]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	bl	dprint_vfmw
	ldr	r1, [r4, #1088]
	movw	r3, #371
	cmp	r1, r3
	beq	.L4084
.L3928:
	add	r1, r1, #1
	str	r1, [r4, #1088]
	ldr	r3, [r6, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	cmp	r2, #20
	beq	.L4085
.L3931:
	sub	r3, r2, #1
	cmp	r3, #29
	ldrls	pc, [pc, r3, asl #2]
	b	.L3939
.L3941:
	.word	.L3940
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3940
	.word	.L3942
	.word	.L3943
	.word	.L3944
	.word	.L3945
	.word	.L3946
	.word	.L3947
	.word	.L3948
	.word	.L3949
	.word	.L3939
	.word	.L3943
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3950
	.word	.L3940
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3939
	.word	.L3951
.L3951:
	mov	r2, #30
	ldr	r1, .L4091+4
	mov	r0, #22
	bl	dprint_vfmw
	cmp	r5, #0
	bne	.L3966
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3966
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	str	r5, [r6, #368]
.L3966:
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
	bne	.L4070
	ldr	r5, .L4091+8
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3967
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #1144]
	blx	ip
.L3967:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, r7
	beq	.L4070
.L4079:
	ldr	ip, [r5]
	cmp	ip, r7
	beq	.L4065
.L4067:
	ldr	r0, [r4, #1144]
	mov	r3, r7
	mov	r2, r7
	mov	r1, #113
	blx	ip
.L4065:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3940:
	ldr	r3, [r4, #1100]
	mov	r0, #22
	ldr	r1, .L4091+12
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
	bne	.L4086
	mov	r0, r6
	bl	H264_DecSlice
	cmn	r0, #2
	beq	.L4063
	cmp	r0, #0
	beq	.L3954
	ldr	r3, .L4091+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L4067
	b	.L4065
.L3942:
	ldr	r1, .L4091+16
	mov	r2, #6
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_DecSEI
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3962
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3962:
	cmp	r5, #0
	bne	.L4087
.L4070:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3943:
	ldr	r1, .L4091+20
	mov	r0, #22
	ldr	r3, [r4, #1092]
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_DecSPS
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3960
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3960:
	cmp	r5, #0
	bne	.L4088
	ldr	r3, [r4, #1092]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #1092]
	b	.L4063
.L3947:
	mov	r2, #11
	ldr	r1, .L4091+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_IsEosNal
	cmp	r0, #1
	beq	.L4089
.L4072:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L4070
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r6, #368]
.L4063:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3948:
	ldr	r1, .L4091+28
	mov	r2, #12
.L4074:
	mov	r0, #22
	bl	dprint_vfmw
	b	.L4072
.L3949:
	mov	r2, #13
	ldr	r1, .L4091+32
	b	.L4074
.L3950:
	mov	r2, #19
	ldr	r1, .L4091+36
	b	.L4074
.L3945:
	mov	r2, #9
	ldr	r1, .L4091+40
	b	.L4074
.L3946:
	mov	r2, #10
	ldr	r1, .L4091+44
	b	.L4074
.L3944:
	ldr	r1, .L4091+48
	mov	r2, #8
	ldr	r3, [r4, #1096]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r6
	bl	H264_DecPPS
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3958
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3958:
	cmp	r5, #0
	bne	.L4090
	ldr	r3, [r4, #1096]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #1096]
	b	.L4063
.L3939:
	mov	r3, r7
	ldr	r1, .L4091+52
	mov	r0, #22
	ldr	r5, .L4091+8
	bl	dprint_vfmw
	cmp	r7, #256
	beq	.L3968
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3969
	mov	r3, #0
	mov	r1, #102
	mov	r2, r3
	ldr	r0, [r4, #1144]
	blx	ip
.L3968:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3970
.L3971:
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3970:
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4070
.L4069:
	mov	r3, #0
	ldr	r0, [r4, #1144]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L4070
.L4085:
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
	bne	.L3932
	ldrb	r2, [r1, #3200]	@ zero_extendqisi2
	ldrb	r3, [r1, #3198]	@ zero_extendqisi2
.L3933:
	cmp	r2, #0
	bne	.L3934
	cmp	r3, #1
	beq	.L3935
.L3934:
	ldr	r1, .L4091+56
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L4065
	ldr	r0, [r4, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	mvn	r0, #0
	str	r3, [r6, #368]
	b	.L4063
.L4084:
	ldr	r1, .L4091+60
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r1, [r4, #1088]
	b	.L3928
.L3932:
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
	b	.L3933
.L4086:
	ldr	r1, .L4091+64
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	bl	ClearCurrSlice
	ldr	r3, .L4091+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4065
.L4066:
	mov	r3, #0
	ldr	r0, [r4, #1144]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4063
.L4090:
	ldr	r5, .L4091+8
	mov	r0, #1
	ldr	r1, .L4091+68
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4065
.L4082:
	sub	r2, fp, #44
	mvn	r6, #0
	mov	r3, #4
	mov	r1, #100
	str	r6, [r2, #-4]!
	ldr	r0, [r4, #1144]
	blx	ip
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4066
	b	.L4065
.L4088:
	ldr	r5, .L4091+8
	mov	r0, #1
	ldr	r1, .L4091+72
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4082
	b	.L4065
.L4087:
	ldr	r1, .L4091+76
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4091+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L4069
	b	.L4070
.L3969:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	bne	.L3971
	b	.L4070
.L3935:
	ldrb	r3, [r1, #3196]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r3, #5
	streqb	r3, [r0, #2]
	ldrb	ip, [r1, #3201]	@ zero_extendqisi2
	mov	r0, #22
	ldreqb	r2, [r1, #3200]	@ zero_extendqisi2
	ldrb	r3, [r1, #3199]	@ zero_extendqisi2
	ldr	r1, .L4091+80
	str	ip, [sp]
	bl	dprint_vfmw
	mov	r2, r7
	ldr	r1, .L4091+84
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r2, [r6, #368]
	ldr	r3, [r2, #64]
	add	r3, r3, #24
	str	r3, [r2, #64]
	ldr	r3, [r6, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	b	.L3931
.L4089:
	ldr	r5, .L4091+8
	add	r3, r6, #12288
	strb	r0, [r3, #1272]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3965
	mov	r3, #0
	mov	r1, #117
	mov	r2, r3
	ldr	r0, [r4, #1144]
	blx	ip
.L3965:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, r7
	beq	.L4072
	b	.L4079
.L3954:
	ldrb	r3, [r5, #216]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L4070
	ldr	r5, .L4091+8
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3956
	mov	r3, r0
	mov	r2, r0
	mov	r1, #112
	ldr	r0, [r4, #1144]
	blx	ip
.L3956:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, #0
	bne	.L4079
	add	r6, r6, #12288
	mov	r3, #1
	strb	r3, [r6, #1273]
	b	.L4063
.L4092:
	.align	2
.L4091:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	cmp	r1, #0
	mov	ip, #0
	mov	r4, r0
	str	ip, [fp, #-56]
	str	ip, [fp, #-60]
	str	ip, [fp, #-64]
	beq	.L4161
	ldr	r2, [r0]
	add	r5, r0, #61440
	add	r6, r0, #57344
	ldr	r3, [r2, #8]
	mov	r3, r3, asl #16
	and	r3, r3, #458752
	str	r3, [r5, #1132]
	ldr	r3, [r2]
	str	r3, [r5, #1136]
	ldr	r3, [r2, #4]
	str	ip, [r5, #1084]
	str	r3, [r5, #1140]
	ldr	r3, [r6, #3832]
	cmp	r3, #1
	beq	.L4096
	ldr	r3, [r2, #832]
	cmp	r3, #1
	beq	.L4162
.L4097:
	mov	r0, r4
	bl	ReceivePacket
	ldr	r3, [r4, #368]
	str	r3, [r5, #1208]
.L4099:
	cmp	r0, #0
	mov	r3, #0
	str	r3, [r6, #3832]
	beq	.L4098
.L4120:
	mov	r7, #0
.L4159:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4162:
	ldr	r3, [r2, #872]
	cmp	r3, #1
	bne	.L4097
	ldr	r3, [r5, #1204]
	cmp	r3, #1
	bne	.L4097
	ldr	r3, [r5, #1208]
	str	r3, [r0, #368]
	str	ip, [r5, #1204]
	str	ip, [r6, #3832]
.L4098:
	mov	r0, r4
	bl	H264DEC_CheckSedOnly
	subs	r7, r0, #0
	beq	.L4100
	ldr	r1, [r4, #368]
	cmp	r1, #0
	beq	.L4123
	ldr	r0, [r5, #1144]
	mvn	r7, #0
	bl	ReleaseNAL
	mov	r3, #0
	mov	r0, r7
	str	r3, [r4, #368]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4096:
	ldr	r0, [r0, #368]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	b	.L4099
.L4123:
	mvn	r7, #0
	b	.L4159
.L4100:
	ldr	r3, [r4, #368]
	add	r8, r4, #13504
	add	r8, r8, #24
	str	r7, [r3, #64]
	mov	r0, r8
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #12]
	ldr	r1, [r3, #8]
	bl	BsInit
	mov	r1, #32
	mov	r0, r8
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
	beq	.L4163
.L4101:
	mov	r0, r4
	bl	FindTrailZeros
	subs	r6, r0, #0
	bne	.L4164
	ldr	r3, [r4, #368]
	mov	r0, r8
	str	r6, [r3, #64]
	ldr	r3, [r4, #368]
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #12]
	bl	BsInit
	mov	r1, r6
	mov	r0, r4
	bl	H264_DecOneNal
	cmn	r0, #2
	moveq	r7, r0
	beq	.L4159
	cmp	r0, #0
	bne	.L4165
	ldr	r3, [r5, #1148]
	cmp	r3, #1
	bne	.L4120
	mov	r0, r4
	bl	H264DEC_SetDecParam
	b	.L4159
.L4163:
	cmp	ip, #0
	bne	.L4166
.L4102:
	sub	r2, fp, #48
	sub	r1, fp, #52
	mov	r0, r4
	bl	InquireSliceProperty
	cmp	r0, #0
	bne	.L4103
	ldr	r2, [fp, #-52]
	ldr	r1, [r5, #1184]
	sub	r3, r2, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #0
	add	r3, r3, r1
	str	r3, [r5, #1184]
	bne	.L4167
	add	r9, r4, #12288
	ldrb	r3, [r9, #1273]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L4121
.L4107:
	ldr	r3, [fp, #-48]
	cmp	r3, #1
	bne	.L4101
	mov	r0, r4
	bl	FlushDPB
	subs	r3, r0, #0
	bne	.L4168
.L4113:
	ldr	r0, [r5, #1144]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L4101
	sub	r3, fp, #56
	ldr	r0, [r5, #1144]
	sub	r2, fp, #60
	sub	r1, fp, #64
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r4]
	ldr	r0, [r3, #708]
	cmp	r0, #1
	bne	.L4101
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	ble	.L4169
.L4114:
	mov	r2, #0
	mov	r3, #1
	str	r2, [r5, #1212]
	mvn	r7, #1
	str	r3, [r6, #3832]
	b	.L4159
.L4103:
	ldr	r1, [r4, #368]
	cmp	r1, #0
	beq	.L4117
	ldr	r0, [r5, #1144]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L4117:
	ldr	r3, .L4170
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4123
	mov	r3, #0
	ldr	r0, [r5, #1144]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r7, #0
	b	.L4159
.L4121:
	mov	r3, #0
	strb	r3, [r9, #1273]
	b	.L4107
.L4167:
	add	r10, r4, #20480
	add	r9, r4, #12288
	ldrb	r3, [r10, #209]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r5, #1188]
	addeq	r3, r3, #1
	addne	r3, r3, #2
	str	r3, [r5, #1188]
	ldrb	r3, [r9, #1273]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L4121
	ldr	r3, [r5, #1068]
	cmp	r3, #0
	beq	.L4107
	ldr	r3, [r4, #368]
	mov	ip, #0
	mov	r0, r8
	mov	r2, #12
	ldr	r1, .L4170+4
	mvn	r7, #1
	str	ip, [r3, #64]
	bl	BsInit
	mov	r0, r4
	mov	r1, #1
	bl	H264_DecOneNal
	ldrb	r3, [r10, #209]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r5, #1188]
	subeq	r3, r3, #1
	subne	r3, r3, #2
	str	r3, [r5, #1188]
	mov	r3, #1
	str	r3, [r6, #3832]
	b	.L4159
.L4166:
	mov	r0, r4
	bl	InquireSliceExtProperty
	b	.L4102
.L4161:
	mov	r0, r1
	movw	r3, #13676
	ldr	r2, .L4170+8
	mvn	r7, #0
	ldr	r1, .L4170+12
	bl	dprint_vfmw
	b	.L4159
.L4164:
	ldr	r1, .L4170+16
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L4159
.L4168:
	movw	r2, #13812
	ldr	r1, .L4170+20
	mov	r0, #0
	bl	dprint_vfmw
	b	.L4113
.L4165:
	ldr	r1, .L4170+24
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L4159
.L4169:
	ldr	r3, [fp, #-60]
	cmp	r3, #32
	bgt	.L4114
	ldr	r10, [r4, #360]
	cmp	r10, #0
	bne	.L4114
	ldr	r3, [r5, #1108]
	cmp	r3, #0
	beq	.L4101
	ldr	r2, [r9, #1276]
	cmp	r2, #0
	beq	.L4101
	ldr	lr, [r9, #1280]
	ldr	ip, [r5, #1216]
	cmp	lr, ip
	beq	.L4101
	ldr	r1, [r5, #1220]
	cmp	r2, r1
	beq	.L4101
	stmia	sp, {r1, ip}
	str	r2, [sp, #8]
	str	lr, [sp, #12]
	ldr	r2, [fp, #-48]
	ldr	r1, .L4170+28
	bl	dprint_vfmw
	ldr	r2, .L4170
	str	r10, [r5, #1212]
	ldr	r3, [r9, #1280]
	ldr	ip, [r2]
	str	r3, [r5, #1216]
	cmp	ip, #0
	ldr	r3, [r9, #1276]
	str	r3, [r5, #1220]
	beq	.L4101
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r5, #1144]
	blx	ip
	b	.L4101
.L4171:
	.align	2
.L4170:
	.word	g_event_report
	.word	.LANCHOR0+432
	.word	.LC20
	.word	.LC21
	.word	.LC428
	.word	.LC354
	.word	.LC429
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r7, r0, #61440
	mov	r4, r0
	mov	r8, r3
	ldr	ip, [fp, #8]
	ldr	r2, [r7, #1172]
	cmp	r2, #1
	bls	.L4173
.L4176:
	ldr	r0, [r4]
	add	r5, r4, #57344
	ldr	r3, [r0, #8]
	cmp	r3, #2
	beq	.L4174
	ldr	r3, [r5, #2424]
	mov	r3, r3, asl #4
	cmp	r3, #1920
	movhi	r3, #0
	strhi	r3, [r7, #1148]
	bls	.L4174
	ldr	r6, [r4, #364]
	cmp	r6, #0
	bne	.L4179
.L4200:
	ldr	r3, [fp, #4]
	str	r1, [r5, #2420]
	cmp	r3, #0
	beq	.L4180
	ldr	r3, .L4203
	mov	r0, ip, asl #6
	sub	r2, r0, ip, asl #3
	add	r2, r2, r3
	ldr	r3, [r2, #4]
	cmp	r3, #3
	beq	.L4199
	cmp	r3, #1
	beq	.L4180
.L4183:
	mov	r1, r8
	mov	r0, r4
	bl	StorePicInDPB
	cmp	r0, #1
	mov	r8, r0
	bls	.L4184
	ldr	r3, [r5, #2376]
	mov	r2, #1
	ldr	r0, [r7, #1144]
	mvn	r6, #0
	ldr	r1, [r3, #632]
	bl	FSP_ClearLogicFs
	mov	r2, r8
	ldr	r1, .L4203+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	mov	r3, #0
	str	r3, [r5, #2376]
.L4182:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4173:
	add	r2, r2, #1
	str	r2, [r7, #1172]
	cmp	r2, #1
	bhi	.L4176
	ldr	r0, [r0]
	add	r5, r4, #57344
.L4174:
	ldr	r3, [r0, #624]
	str	r3, [r7, #1148]
	ldr	r6, [r4, #364]
	cmp	r6, #0
	beq	.L4200
.L4179:
	str	r1, [r5, #2420]
	mov	r0, r4
	bl	ClearAllSlice
	mov	r1, r8
	mov	r0, r4
	bl	StorePicInDPB
	subs	r2, r0, #0
	bne	.L4201
.L4185:
	mov	r0, r4
	bl	FlushDPB
	subs	r2, r0, #0
	bne	.L4202
.L4186:
	add	r2, r4, #12288
	mov	r3, #0
	str	r3, [r5, #3828]
	add	r4, r4, #20480
	ldr	r1, [r2, #1292]
	mov	r6, r3
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
	mov	r0, r6
	mov	r3, #262144
	str	r1, [r4, #1880]
	str	r2, [r4, #1884]
	str	r3, [r4, #1896]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4180:
	mov	r0, r4
	bl	ClearAllSlice
	b	.L4183
.L4184:
	mov	r3, #1
	mov	r0, r6
	str	r3, [r5, #3828]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4202:
	ldr	r1, .L4203+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4186
.L4201:
	ldr	r1, .L4203+12
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4185
.L4199:
	mov	r0, r4
	bl	ClearAllSlice
	b	.L4182
.L4204:
	.align	2
.L4203:
	.word	g_VdmDrvParam+40
	.word	.LC430
	.word	.LC432
	.word	.LC431
	UNWIND(.fnend)
	.size	H264DEC_VDMPostProc, .-H264DEC_VDMPostProc
	.align	2
	.global	H264DEC_VDMProcSedOnly
	.type	H264DEC_VDMProcSedOnly, %function
H264DEC_VDMProcSedOnly:
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
	ldr	r5, .L4217
	mov	r3, #180
	ldr	r2, [r0, #360]
	add	r7, r0, #61440
	ldr	ip, .L4217+4
	mov	r6, r0
	mla	r1, r3, r1, r5
	ldr	r4, [r2, #20]
	ldr	r3, [r2, #28]
	mov	r0, #22
	ldr	r2, [r7, #1144]
	add	r5, r1, #16
	ldr	r1, .L4217+8
	mul	r4, r3, r4
	ldr	r8, [ip, r2, asl #2]
	ldr	r3, [r5, #4]
	sub	r4, r4, #1
	mov	r2, r4
	bl	dprint_vfmw
	ldr	r3, [r5, #4]
	cmp	r3, r4
	ldr	r3, [r7, #1156]
	beq	.L4215
	cmp	r3, #1
	beq	.L4216
.L4210:
	mov	r3, #0
	mov	r0, #0
	str	r3, [r6, #360]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4215:
	cmp	r3, #1
	beq	.L4207
	mov	r0, r6
	bl	DecVDM
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4216:
	ldr	r3, [r7, #1068]
	mov	r4, #0
	str	r4, [r7, #1160]
	sub	r3, r3, #1
	str	r3, [r7, #1068]
	ldr	r1, [r6, #368]
	cmp	r1, r4
	beq	.L4210
	ldr	r0, [r7, #1144]
	bl	ReleaseNAL
	str	r4, [r6, #368]
	b	.L4210
.L4207:
	ldr	r2, [r7, #1068]
	add	r0, r6, #57344
	str	r3, [r7, #1160]
	mov	r1, #0
	sub	r2, r2, #1
	str	r2, [r7, #1068]
	strb	r3, [r8, #745]
	str	r3, [r0, #3832]
	mov	r0, #0
	str	r1, [r6, #360]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4218:
	.align	2
.L4217:
	.word	g_BackUp
	.word	s_pstVfmwChan
	.word	.LC433
	UNWIND(.fnend)
	.size	H264DEC_VDMProcSedOnly, .-H264DEC_VDMProcSedOnly
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
	beq	.L4235
	add	r4, r0, #27392
	add	r9, r0, #26368
	add	r4, r4, #8
	add	r9, r9, #160
	add	r8, r0, #24576
	mov	lr, #0
	mov	r1, r4
.L4227:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4222
	cmp	r1, #0
	beq	.L4223
	ldr	r0, [r8, #2148]
	cmp	r0, #0
	beq	.L4223
	ldr	r2, [r8, #1952]
	cmp	r2, r1
	beq	.L4222
	mov	r2, r9
	b	.L4224
.L4225:
	ldr	ip, [r2, #4]!
	cmp	ip, r1
	beq	.L4222
.L4224:
	add	r3, r3, #1
	cmp	r3, r0
	bne	.L4225
.L4223:
	add	r4, r5, #61440
	ldr	r0, [r4, #1144]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L4235
	cmn	r0, #1
	bne	.L4255
	add	r1, r5, #8
	ldr	r0, [r4, #1144]
	bl	FSP_ClearNotInVoQueue
	b	.L4255
.L4222:
	add	lr, lr, #1
	add	r1, r1, #792
	cmp	lr, r7
	bne	.L4227
	sub	r2, fp, #40
	sub	r1, fp, #44
	add	r0, r5, #8
	bl	GetQueueImgNum
	ldr	r1, .L4259
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, .L4259+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r6, #3820]
	cmp	r3, #0
	addne	r8, r5, #26368
	addne	r9, r5, #24576
	addne	r8, r8, #160
	movne	r7, #0
	beq	.L4257
.L4228:
	cmp	r4, #0
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	ldrb	lr, [r4, #4]	@ zero_extendqisi2
	beq	.L4239
	ldr	r0, [r9, #2148]
	cmp	r0, #0
	beq	.L4239
	ldr	r2, [r9, #1952]
	cmp	r4, r2
	beq	.L4241
	mov	r1, r8
	mov	ip, #0
	b	.L4232
.L4233:
	ldr	r2, [r1, #4]!
	cmp	r4, r2
	beq	.L4241
.L4232:
	add	ip, ip, #1
	cmp	ip, r0
	bne	.L4233
.L4239:
	mov	r2, #1
.L4231:
	str	r2, [sp, #4]
	mov	r0, #0
	mov	r2, r7
	str	lr, [sp]
	ldr	r1, .L4259+8
	add	r7, r7, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #3820]
	add	r4, r4, #792
	cmp	r3, r7
	bhi	.L4228
.L4234:
	ldr	r3, [fp, #-40]
	mov	r0, #0
	ldr	r2, [fp, #-44]
	ldr	r1, .L4259+12
	bl	dprint_vfmw
	ldr	r3, [r6, #3820]
	ldr	r0, [r9, #2148]
	ldr	r2, [fp, #-44]
	sub	r3, r3, #2
	ldr	r1, [fp, #-40]
	rsb	r3, r0, r3
	add	r2, r2, r1
	cmp	r2, r3
	blt	.L4258
.L4255:
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4235:
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4241:
	mov	r2, #0
	b	.L4231
.L4258:
	ldr	r1, .L4259+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r5
	mov	r1, #1
	bl	ClearAll
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4257:
	add	r9, r5, #24576
	b	.L4234
.L4260:
	.align	2
.L4259:
	.word	.LC434
	.word	.LC435
	.word	.LC437
	.word	.LC436
	.word	.LC438
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
	.type	SarTable.15566, %object
	.size	SarTable.15566, 136
SarTable.15566:
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
	.type	__func__.15747, %object
	.size	__func__.15747, 12
__func__.15747:
	.ascii	"H264_DecSEI\000"
	.type	__func__.14600, %object
	.size	__func__.14600, 13
__func__.14600:
	.ascii	"DirectOutput\000"
	.space	3
	.type	__func__.15065, %object
	.size	__func__.15065, 8
__func__.15065:
	.ascii	"InitDPB\000"
	.type	__func__.14718, %object
	.size	__func__.14718, 14
__func__.14718:
	.ascii	"StorePicInDPB\000"
	.space	2
	.type	__FUNCTION__.15986, %object
	.size	__FUNCTION__.15986, 13
__FUNCTION__.15986:
	.ascii	"H264DEC_Init\000"
	.space	3
	.type	__FUNCTION__.15202, %object
	.size	__FUNCTION__.15202, 8
__FUNCTION__.15202:
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
	ASCII(.ascii	"CtbNumOneFrame=%d, SedEnd0=%d\012\000" )
	.space	1
.LC434:
	ASCII(.ascii	"cann't find FrameStore\012\000" )
.LC435:
	ASCII(.ascii	"========== H264 FrameStore state(is_used, is_in_dpb" )
	ASCII(.ascii	", IsOutDPB) ========\012\000" )
	.space	3
.LC436:
	ASCII(.ascii	"ReadImgNum = %d, NewImgNum = %d\012\000" )
	.space	3
.LC437:
	ASCII(.ascii	"%02d: %d %d %d\012\000" )
.LC438:
	ASCII(.ascii	"FrameStore leak, ClearAll\012\000" )
	.bss
	.align	2
.LANCHOR2 = . + 0
.LANCHOR3 = . + 8184
	.type	pps_tmp.15486, %object
	.size	pps_tmp.15486, 2208
pps_tmp.15486:
	.space	2208
	.type	sps_tmp.15635, %object
	.size	sps_tmp.15635, 4000
sps_tmp.15635:
	.space	4000
	.type	TmpBuf, %object
	.size	TmpBuf, 68
TmpBuf:
	.space	68
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
