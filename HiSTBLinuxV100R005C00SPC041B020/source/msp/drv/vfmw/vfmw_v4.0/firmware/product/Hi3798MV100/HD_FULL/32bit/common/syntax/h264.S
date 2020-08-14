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
	ldr	r2, [r2, #12]
	ldr	r3, [r3, #12]
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
	ldr	r2, [r2, #8]
	ldr	r3, [r3, #8]
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
	ldr	r2, [r2, #628]
	ldr	r3, [r3, #628]
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
	ldr	r2, [r2, #632]
	ldr	r3, [r3, #632]
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
	ldr	r2, [r2, #16]
	ldr	r3, [r3, #16]
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
	ldr	r2, [r2, #16]
	ldr	r3, [r3, #16]
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
	ldr	r2, [r2, #636]
	ldr	r3, [r3, #636]
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
	ldr	r2, [r2, #636]
	ldr	r3, [r3, #636]
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
	add	ip, r0, #53248
	add	r0, r0, #24576
	ldr	lr, [ip, #4076]
	ldr	r2, [r0, #2056]
	ldr	r3, [ip, #4072]
	add	r2, r2, lr, lsr #1
	add	r0, r2, #1
	mov	r3, r3, lsr #1
	cmp	r3, r0
	movcc	r3, #0
	strcc	r3, [r1, #84]
	ldmccfd	sp, {fp, sp, pc}
	sub	r3, r3, #1
	rsb	r3, r2, r3
	cmp	r3, #1
	movhi	r3, #2
	movls	r3, #1
	strhi	r3, [r1, #84]
	strls	r3, [r1, #84]
	ldrhi	r3, [ip, #4076]
	ldrls	r3, [ip, #4076]
	addhi	r3, r3, #4
	addls	r3, r3, #2
	str	r3, [ip, #4076]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	SetFrmRepeatCount.part.1, .-SetFrmRepeatCount.part.1
	.align	2
	.type	DumpList.part.5, %function
DumpList.part.5:
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
	add	r9, r0, #26880
	add	r9, r9, #160
	add	r7, r0, #20480
	ldr	r8, .L64
	add	r7, r7, #248
	mov	r6, #0
	mov	r10, r9
	str	r0, [fp, #-48]
.L33:
	ldr	r3, [r7, #4]!
	cmp	r3, #0
	movne	r5, r10
	movne	r4, #0
	beq	.L36
.L34:
	ldr	r0, [r5, #4]!
	mov	r3, r4
	mov	r2, r6
	ldr	r1, .L64+4
	add	r4, r4, #1
	ldr	ip, [r0, #4]
	mov	r0, #13
	ldr	lr, [ip, #636]
	str	lr, [sp, #4]
	ldr	ip, [ip, #624]
	str	ip, [sp]
	ldr	ip, [r8, #68]
	blx	ip
	ldr	r3, [r7]
	cmp	r4, r3
	bcc	.L34
.L36:
	add	r6, r6, #1
	add	r10, r10, #132
	cmp	r6, #2
	bne	.L33
	ldr	r3, [fp, #-48]
	add	r5, r3, #20480
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
	ldr	r6, .L64
	mov	r0, #13
	ldr	r1, .L64+8
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r5, #256]
	cmp	r3, #0
	beq	.L51
	ldr	r3, [fp, #-48]
	mov	r4, #0
	add	r8, r3, #27136
	add	r8, r8, #36
	b	.L50
.L62:
	bcc	.L48
	cmp	r2, #2
	ldr	r1, .L64+12
	mov	r0, #13
	bne	.L46
	ldr	r2, [r3, #4]
	ldr	r3, [r6, #68]
	ldr	r2, [r2, #636]
	blx	r3
.L46:
	ldr	r3, [r5, #256]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L51
.L50:
	ldr	r3, [r8, #4]!
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L62
	ldr	r2, [r3, #4]
	mov	r0, #13
	ldr	r3, [r6, #68]
	add	r4, r4, #1
	ldr	r1, .L64+16
	ldr	r2, [r2, #636]
	blx	r3
	ldr	r3, [r5, #256]
	cmp	r4, r3
	bcc	.L50
.L51:
	ldr	r3, [r6, #68]
	mov	r0, #13
	ldr	r1, .L64+20
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L48:
	ldr	r2, [r3, #4]
	mov	r0, #13
	ldr	r3, [r6, #68]
	ldr	r1, .L64+24
	ldr	r2, [r2, #636]
	blx	r3
	b	.L46
.L60:
	ldr	r6, .L64
	mov	r0, #13
	ldr	r1, .L64+28
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r5, #252]
	cmp	r3, #0
	movne	r4, #0
	bne	.L38
	b	.L43
.L63:
	bcc	.L41
	cmp	r2, #2
	ldr	r1, .L64+12
	mov	r0, #13
	bne	.L39
	ldr	r2, [r3, #4]
	ldr	r3, [r6, #68]
	ldr	r2, [r2, #636]
	blx	r3
.L39:
	ldr	r3, [r5, #252]
	add	r4, r4, #1
	cmp	r4, r3
	bcs	.L43
.L38:
	ldr	r3, [r9, #4]!
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L63
	ldr	r2, [r3, #4]
	mov	r0, #13
	ldr	r3, [r6, #68]
	add	r4, r4, #1
	ldr	r1, .L64+16
	ldr	r2, [r2, #636]
	blx	r3
	ldr	r3, [r5, #252]
	cmp	r4, r3
	bcc	.L38
.L43:
	ldr	r3, [r6, #68]
	mov	r0, #13
	ldr	r1, .L64+20
	blx	r3
	b	.L37
.L41:
	ldr	r2, [r3, #4]
	mov	r0, #13
	ldr	r3, [r6, #68]
	ldr	r1, .L64+24
	ldr	r2, [r2, #636]
	blx	r3
	b	.L39
.L65:
	.align	2
.L64:
	.word	vfmw_Osal_Func_Ptr_S
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
	.type	CombinePacket.part.9, %function
CombinePacket.part.9:
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
	ldr	r6, .L67
	mov	r4, r0
	ldr	r1, .L67+4
	mov	r0, #7
	mov	r5, #0
	ldr	r3, [r6, #68]
	blx	r3
	ldr	ip, [r4, #368]
	ldr	r7, [r6, #68]
	mov	r0, #7
	ldr	r1, .L67+8
	ldr	lr, [ip, #40]
	ldr	r3, [ip, #44]
	ldr	r2, [ip, #16]
	str	lr, [sp, #4]
	ldr	ip, [ip, #12]
	str	ip, [sp]
	blx	r7
	ldr	r3, [r4, #368]
	ldr	ip, [r6, #52]
	ldr	r2, [r3, #12]
	ldr	r0, [r3, #36]
	ldr	r1, [r3, #8]
	rsb	r0, r2, r0
	blx	ip
	ldr	r3, [r4, #368]
	mov	ip, #1
	ldr	r1, .L67+12
	mov	r0, #7
	ldr	r2, [r3, #60]
	ldr	r7, [r3, #32]
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
	ldr	ip, [r6, #68]
	ldr	r3, [r2, #12]
	ldr	r2, [r2, #16]
	blx	ip
	add	r3, r4, #53248
	mov	r1, r7
	ldr	r0, [r3, #4056]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #368]
	str	r5, [r3, #36]
	ldr	r3, [r4, #368]
	str	r5, [r3, #52]
	ldr	r3, [r4, #368]
	str	r5, [r3, #40]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L68:
	.align	2
.L67:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC7
	.word	.LC8
	.word	.LC9
	UNWIND(.fnend)
	.size	CombinePacket.part.9, .-CombinePacket.part.9
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
	add	r3, r8, #53248
	mvn	r5, #0
	mov	r7, #32
	mov	r2, #1
	str	r2, [r3, #3996]
.L71:
	ldr	ip, [r8, #368]
	mov	r2, r6
	ldr	r6, .L75
	mov	r3, r5
	ldr	r1, .L75+4
	mov	r0, #21
	ldr	lr, [ip, #64]
	add	r4, lr, r7
	str	r4, [ip, #64]
	ldr	ip, [r6, #68]
	blx	ip
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
	.word	vfmw_Osal_Func_Ptr_S
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
	mov	r5, r0
	bl	ZerosMS_32
	cmp	r0, #15
	mov	r4, r0
	bhi	.L78
	mov	r4, r0, asl #1
	mov	r0, r6
	rsb	r3, r4, #31
	add	r4, r4, #1
	mov	r3, r5, lsr r3
	and	r2, r3, #1
	mov	r1, r4
	rsb	r5, r2, #0
	eor	r5, r5, r3, lsr #1
	add	r5, r5, r2
	bl	BsSkip
.L79:
	ldr	lr, [r8, #368]
	mov	r2, r7
	ldr	r6, .L82
	mov	r3, r5
	ldr	r1, .L82+4
	mov	r0, #21
	ldr	ip, [lr, #64]
	add	r4, ip, r4
	str	r4, [lr, #64]
	ldr	ip, [r6, #68]
	blx	ip
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L78:
	cmp	r0, #31
	bls	.L81
	add	r3, r8, #53248
	mov	r2, #1
	mvn	r5, #-2147483648
	mov	r4, #32
	str	r2, [r3, #3996]
	b	.L79
.L81:
	mov	r1, r0
	mov	r0, r6
	bl	BsSkip
	mov	r1, r4
	mov	r0, r6
	mov	r4, r4, asl #1
	bl	BsGet
	mov	r1, #1
	add	r4, r4, #1
	mov	r9, r0
	mov	r0, r6
	bl	BsGet
	and	r0, r0, #1
	rsb	r5, r0, #0
	eor	r9, r9, r5
	add	r5, r9, r0
	b	.L79
.L83:
	.align	2
.L82:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r5, .L85
	ldr	r1, .L85+4
	ldr	ip, [lr, #64]
	add	ip, ip, r6
	str	ip, [lr, #64]
	ldr	ip, [r5, #68]
	mov	r4, r0
	mov	r0, #21
	mov	r3, r4
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L86:
	.align	2
.L85:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r5, .L88
	mov	r2, r6
	ldr	r1, .L88+4
	ldr	ip, [lr, #64]
	add	ip, ip, #1
	str	ip, [lr, #64]
	ldr	ip, [r5, #68]
	mov	r4, r0
	mov	r3, r0
	mov	r0, #21
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L89:
	.align	2
.L88:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC10
	UNWIND(.fnend)
	.size	u_1, .-u_1
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
	ldr	r3, [r5, #2060]
	ldr	ip, [r5, #2056]
	cmp	r3, ip
	strhi	ip, [r5, #2060]
	movhi	r3, ip
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r0, r0, #26368
	mov	r3, #0
	add	r0, r0, #68
.L94:
	ldr	ip, [r0, #4]!
	ldr	r4, [r1]
	ldr	lr, [ip, #636]
	cmp	r4, lr
	ble	.L93
	ldrb	ip, [ip, #4]	@ zero_extendqisi2
	cmp	ip, #0
	strne	lr, [r1]
	strne	r3, [r2]
.L93:
	ldr	ip, [r5, #2060]
	add	r3, r3, #1
	cmp	ip, r3
	bhi	.L94
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
	ldr	r3, [r5, #2060]
	ldr	ip, [r5, #2056]
	cmp	r3, ip
	strhi	ip, [r5, #2060]
	movhi	r3, ip
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	add	r0, r0, #26368
	mov	r3, #0
	add	r0, r0, #68
.L104:
	ldr	ip, [r0, #4]!
	ldr	r4, [r1]
	ldr	lr, [ip, #636]
	cmp	r4, lr
	bge	.L103
	ldrb	ip, [ip, #4]	@ zero_extendqisi2
	cmp	ip, #0
	strne	lr, [r1]
	strne	r3, [r2]
.L103:
	ldr	ip, [r5, #2060]
	add	r3, r3, #1
	cmp	ip, r3
	bhi	.L104
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
	beq	.L126
	tst	r3, #1
	bne	.L127
.L115:
	and	r3, r3, #2
	ands	r3, r3, #255
	beq	.L119
.L116:
	ldr	r0, [r0, #584]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #65536
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L119:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L127:
	ldr	r2, [r0, #548]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #65536
	bne	.L115
.L120:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L126:
	ldr	r3, [r0, #512]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L120
	ldr	r3, [r0, #548]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	bne	.L116
	b	.L120
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
	beq	.L143
	tst	r3, #1
	bne	.L144
.L132:
	and	r3, r3, #2
	ands	r3, r3, #255
	beq	.L136
.L133:
	ldr	r0, [r0, #584]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #256
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L136:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L144:
	ldr	r2, [r0, #548]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #256
	bne	.L132
.L137:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L143:
	ldr	r3, [r0, #512]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	beq	.L137
	ldr	r3, [r0, #548]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	bne	.L133
	b	.L137
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
	ldr	r8, [r9, #2060]
	cmp	r8, #0
	beq	.L152
	add	r10, r0, #26368
	mov	r4, #0
	add	r10, r10, #68
	mov	r5, r4
.L148:
	ldr	r7, [r10, #4]!
	add	r5, r5, #1
	mov	r0, r7
	bl	LongTermRefFlg
	add	r3, r4, #6592
	add	r3, r3, #50
	cmp	r0, #0
	strne	r7, [r6, r3, asl #2]
	addne	r4, r4, #1
	cmp	r5, r8
	bne	.L148
.L146:
	ldr	r3, [r9, #2056]
	str	r4, [r9, #2068]
	cmp	r3, r4
	ldmlsfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	add	r0, r4, #6592
	mov	r2, #0
	add	r0, r0, #49
	add	r0, r6, r0, lsl #2
.L150:
	add	r4, r4, #1
	str	r2, [r0, #4]!
	cmp	r4, r3
	bne	.L150
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L152:
	mov	r4, r8
	b	.L146
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
	ldr	r8, [r9, #2060]
	cmp	r8, #0
	beq	.L165
	add	r10, r0, #26368
	mov	r4, #0
	add	r10, r10, #68
	mov	r5, r4
.L161:
	ldr	r7, [r10, #4]!
	add	r5, r5, #1
	mov	r0, r7
	bl	ShortTermRefFlg
	add	r3, r4, #6592
	add	r3, r3, #34
	cmp	r0, #0
	strne	r7, [r6, r3, asl #2]
	addne	r4, r4, #1
	cmp	r5, r8
	bne	.L161
.L159:
	ldr	r3, [r9, #2056]
	str	r4, [r9, #2064]
	cmp	r3, r4
	ldmlsfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	add	r0, r4, #6592
	mov	r2, #0
	add	r0, r0, #33
	add	r0, r6, r0, lsl #2
.L163:
	add	r4, r4, #1
	str	r2, [r0, #4]!
	cmp	r4, r3
	bne	.L163
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L165:
	mov	r4, r8
	b	.L159
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
	ldr	r3, [r0, #528]
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
	beq	.L175
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	mov	ip, #0
	strb	ip, [r1, #549]
	and	r2, r2, #2
	strb	ip, [r1, #550]
	strb	r2, [r1, #2]
.L175:
	tst	r3, #2
	beq	.L176
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	mov	ip, #0
	strb	ip, [r1, #585]
	and	r2, r2, #1
	strb	ip, [r1, #586]
	strb	r2, [r1, #2]
.L176:
	cmp	r3, #3
	add	r0, r0, #53248
	moveq	r3, #0
	streqb	r3, [r1, #514]
	streqb	r3, [r1, #513]
	mov	r3, #0
	mov	r2, r3
	strb	r3, [r1, #2]
	ldr	r1, [r1, #620]
	ldr	r0, [r0, #4056]
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
	ldr	r7, [r3, #2060]
	cmp	r7, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r6, r0, #26368
	mov	r4, #0
	add	r6, r6, #68
.L187:
	ldr	r5, [r6, #4]!
	add	r4, r4, #1
	ldrb	r3, [r5, #2]	@ zero_extendqisi2
	mov	r0, r5
	cmp	r3, #0
	beq	.L186
	bl	ShortTermRefFlg
	cmp	r0, #0
	bne	.L198
.L186:
	cmp	r4, r7
	bne	.L187
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L198:
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
	ldr	r4, [r3, #2068]
	cmp	r4, #0
	ldmeqfd	sp, {r4, fp, sp, pc}
	ldr	ip, [r3, #1992]
	ldr	r3, [ip, #632]
	cmp	r3, r1
	addne	r2, r0, #26368
	movne	r3, #0
	addne	r2, r2, #200
	bne	.L203
	b	.L201
.L204:
	ldr	ip, [r2, #4]!
	ldr	lr, [ip, #632]
	cmp	lr, r1
	beq	.L201
.L203:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L204
	ldmfd	sp, {r4, fp, sp, pc}
.L201:
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
	ldr	r6, [ip, #2068]
	cmp	r6, #0
	ldmeqfd	sp, {r4, r5, r6, fp, sp, pc}
	add	r4, r0, #26368
	mov	ip, #0
	add	r4, r4, #196
	b	.L214
.L212:
	cmp	ip, r6
	beq	.L228
.L214:
	ldr	lr, [r4, #4]!
	add	ip, ip, #1
	ldr	r5, [lr, #632]
	cmp	r5, r3
	bne	.L212
	cmp	r2, #1
	ldreqb	r5, [lr, #585]	@ zero_extendqisi2
	beq	.L227
	cmp	r2, #2
	bne	.L212
	ldrb	r5, [lr, #549]	@ zero_extendqisi2
.L227:
	clz	r5, r5
	mov	r5, r5, lsr #5
	cmp	lr, r1
	orrne	r5, r5, #1
	cmp	r5, #0
	beq	.L212
	mov	r1, lr
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	UnMarkFrameStoreRef
.L228:
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
	bne	.L230
	add	ip, r0, #24576
	ldr	r1, [r1, #528]
	ldr	r4, [ip, #2064]
	sub	r1, r1, #1
	rsb	r2, r2, r1
	cmp	r4, #0
	beq	.L259
	add	ip, r0, #26368
	add	ip, ip, #132
	b	.L236
.L235:
	cmp	r3, r4
	beq	.L260
.L236:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #524]
	cmp	lr, r2
	bne	.L235
	ldrb	lr, [r1, #2]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L235
	ldr	lr, [r1, #512]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L235
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	UnMarkFrameStoreRef
.L230:
	add	r3, r0, #24576
	ldr	r1, [r1, #528]
	ldr	r5, [r3, #2064]
	rsb	r2, r2, r1, lsl #1
	cmp	r5, #0
	beq	.L261
	add	r4, r0, #26368
	mov	ip, #0
	add	r4, r4, #132
	b	.L241
.L237:
	tst	r3, #2
	beq	.L239
	ldr	r3, [r1, #584]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L262
.L239:
	add	ip, ip, #1
	cmp	ip, r5
	beq	.L263
.L241:
	ldr	r1, [r4, #4]!
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	ands	r6, r3, #1
	beq	.L237
	ldr	lr, [r1, #548]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L237
	ldr	lr, [r1, #560]
	cmp	lr, r2
	bne	.L237
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #24576
	strb	r3, [r1, #2]
	mov	r1, #0
	ldr	r3, [r2, #1928]
	strb	r1, [r3, #550]
	ldr	r3, [r2, #1928]
	ldrb	ip, [r3, #512]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #514]
	ldreq	r3, [r2, #1928]
	streqb	r1, [r3, #513]
	ldreq	r3, [r2, #1928]
	ldrb	r1, [r3, #2]	@ zero_extendqisi2
	cmp	r1, #0
	ldmnefd	sp, {r4, r5, r6, fp, sp, pc}
	ldr	ip, [r3, #660]
	add	lr, r0, #53248
	mov	r2, r1
	add	ip, ip, #13952
	add	ip, ip, #40
	add	r0, r0, ip, lsl #2
	str	r1, [r0, #4]
	ldr	r1, [r3, #620]
.L256:
	ldr	r0, [lr, #4056]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	FSP_SetRef
.L262:
	ldr	r3, [r1, #596]
	cmp	r3, r2
	bne	.L239
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #2]
	add	r3, r3, #24576
	mov	r2, #0
	ldr	r1, [r3, #1928]
	strb	r2, [r1, #586]
	ldr	r1, [r3, #1928]
	ldrb	ip, [r1, #512]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r2
	streqb	r2, [r1, #514]
	ldreq	r1, [r3, #1928]
	streqb	r2, [r1, #513]
	ldreq	r1, [r3, #1928]
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	ldmnefd	sp, {r4, r5, r6, fp, sp, pc}
	ldr	ip, [r1, #660]
	add	lr, r0, #53248
	mov	r2, r3
	add	ip, ip, #13952
	add	ip, ip, #40
	add	r0, r0, ip, lsl #2
	str	r3, [r0, #4]
	ldr	r1, [r1, #620]
	b	.L256
.L263:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L260:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L259:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L261:
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
	beq	.L265
	add	r3, r0, #24576
	ldr	r5, [r3, #2068]
	cmp	r5, #0
	beq	.L293
	add	r4, r0, #26368
	mov	ip, #0
	add	r4, r4, #196
	b	.L274
.L270:
	tst	r3, #2
	beq	.L272
	ldr	r3, [r1, #584]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	beq	.L294
.L272:
	add	ip, ip, #1
	cmp	ip, r5
	beq	.L295
.L274:
	ldr	r1, [r4, #4]!
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	ands	r6, r3, #1
	beq	.L270
	ldr	lr, [r1, #548]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L270
	ldr	lr, [r1, #556]
	cmp	r2, lr
	bne	.L270
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #24576
	strb	r3, [r1, #2]
	mov	r1, #0
	ldr	r3, [r2, #1992]
	strb	r1, [r3, #549]
	ldr	r3, [r2, #1992]
	ldrb	ip, [r3, #512]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #514]
	ldreq	r3, [r2, #1992]
	streqb	r1, [r3, #513]
	ldreq	r3, [r2, #1992]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	cmp	r2, #0
	ldmnefd	sp, {r4, r5, r6, fp, sp, pc}
	ldr	r3, [r3, #660]
	add	r3, r3, #13952
	add	r3, r3, #40
	add	r3, r0, r3, lsl #2
	str	r2, [r3, #4]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L265:
	add	r1, r0, #24576
	ldr	r4, [r1, #2068]
	cmp	r4, #0
	beq	.L296
	add	ip, r0, #26368
	add	ip, ip, #196
	b	.L276
.L275:
	cmp	r3, r4
	beq	.L297
.L276:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #520]
	cmp	lr, r2
	bne	.L275
	ldrb	lr, [r1, #2]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L275
	ldr	lr, [r1, #512]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L275
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	UnMarkFrameStoreRef
.L294:
	ldr	r3, [r1, #592]
	cmp	r2, r3
	bne	.L272
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #2]
	add	r3, r3, #24576
	mov	r1, #0
	ldr	r2, [r3, #1992]
	strb	r1, [r2, #585]
	ldr	r2, [r3, #1992]
	ldrb	ip, [r2, #512]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r2, #514]
	ldreq	r2, [r3, #1992]
	streqb	r1, [r2, #513]
	ldreq	r2, [r3, #1992]
	ldrb	r1, [r2, #2]	@ zero_extendqisi2
	cmp	r1, #0
	ldmnefd	sp, {r4, r5, r6, fp, sp, pc}
	ldr	r3, [r2, #660]
	add	r3, r3, #13952
	add	r3, r3, #40
	add	r3, r0, r3, lsl #2
	str	r1, [r3, #4]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L297:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L295:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L296:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L293:
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
	beq	.L299
	ldr	r7, [r3, #2064]
	cmp	r7, #0
	beq	.L328
	add	r6, r0, #26368
	mov	ip, #0
	add	r6, r6, #132
	b	.L313
.L305:
	tst	r4, #2
	beq	.L308
	ldr	lr, [r3, #584]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	beq	.L329
.L308:
	add	ip, ip, #1
	cmp	ip, r7
	beq	.L330
.L313:
	ldr	r3, [r6, #4]!
	ldrb	r4, [r3, #2]	@ zero_extendqisi2
	tst	r4, #1
	beq	.L305
	ldr	lr, [r3, #548]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L305
	ldr	lr, [r3, #560]
	cmp	lr, r2
	bne	.L305
	ldrb	lr, [r3, #585]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L309
	ldr	lr, [r3, #632]
	cmp	lr, r1
	bne	.L308
.L309:
	str	r1, [r3, #632]
	sub	r3, r5, #1
	add	r2, r0, ip, lsl #2
	clz	r3, r3
	add	r2, r2, #24576
	mov	ip, #1
	mov	r3, r3, lsr #5
	mov	r0, #0
	add	r1, r3, r1, lsl #1
	ldr	r3, [r2, #1928]
	str	r1, [r3, #556]
	ldr	r3, [r2, #1928]
	strb	ip, [r3, #549]
	ldr	r3, [r2, #1928]
	strb	r0, [r3, #550]
	ldr	r3, [r2, #1928]
	ldrb	r1, [r3, #512]	@ zero_extendqisi2
	cmp	r1, #3
	cmpne	r1, r0
	bne	.L331
	ldrb	r1, [r3, #585]	@ zero_extendqisi2
	ldrb	r0, [r3, #549]	@ zero_extendqisi2
	cmp	r1, r0
	streqb	r1, [r3, #513]
	ldreq	r3, [r2, #1928]
	ldreqb	r2, [r3, #550]	@ zero_extendqisi2
	streqb	r2, [r3, #514]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L299:
	ldr	r6, [r3, #2064]
	cmp	r6, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	lr, r0, #26368
	mov	r3, r5
	add	lr, lr, #132
	b	.L304
.L303:
	add	r3, r3, #1
	cmp	r3, r6
	beq	.L332
.L304:
	ldr	ip, [lr, #4]!
	ldrb	r4, [ip, #2]	@ zero_extendqisi2
	cmp	r4, #3
	bne	.L303
	ldr	r4, [ip, #512]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	bne	.L303
	ldr	r4, [ip, #524]
	cmp	r4, r2
	bne	.L303
	add	r3, r0, r3, lsl #2
	str	r1, [ip, #632]
	add	r3, r3, #24576
	mov	r0, #1
	mov	r2, #0
	ldr	ip, [r3, #1928]
	str	r1, [ip, #520]
	ldr	r1, [r3, #1928]
	strb	r0, [r1, #513]
	ldr	r1, [r3, #1928]
	strb	r0, [r1, #549]
	ldr	r1, [r3, #1928]
	strb	r0, [r1, #585]
	ldr	r1, [r3, #1928]
	strb	r2, [r1, #514]
	ldr	r1, [r3, #1928]
	strb	r2, [r1, #550]
	ldr	r3, [r3, #1928]
	strb	r2, [r3, #586]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L329:
	ldr	lr, [r3, #596]
	cmp	lr, r2
	bne	.L308
	ldrb	lr, [r3, #549]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L312
	ldr	lr, [r3, #632]
	cmp	lr, r1
	bne	.L308
.L312:
	str	r1, [r3, #632]
	sub	r3, r5, #2
	add	r0, r0, ip, lsl #2
	clz	r3, r3
	add	r0, r0, #24576
	mov	ip, #1
	mov	r3, r3, lsr #5
	mov	r2, #0
	add	r1, r3, r1, lsl #1
	ldr	r3, [r0, #1928]
	str	r1, [r3, #592]
	ldr	r3, [r0, #1928]
	strb	ip, [r3, #585]
	ldr	r3, [r0, #1928]
	strb	r2, [r3, #586]
	ldr	r3, [r0, #1928]
	ldrb	r2, [r3, #512]	@ zero_extendqisi2
	cmp	r2, #3
	cmpne	r2, #0
	bne	.L333
	ldrb	r2, [r3, #585]	@ zero_extendqisi2
	ldrb	r1, [r3, #549]	@ zero_extendqisi2
	cmp	r2, r1
	streqb	r2, [r3, #513]
	ldreq	r3, [r0, #1928]
	ldreqb	r2, [r3, #586]	@ zero_extendqisi2
	streqb	r2, [r3, #514]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L330:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L332:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L331:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L333:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L328:
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
	bne	.L335
	ldr	r5, [r6, #528]
	mov	r1, r3
	sub	r5, r5, #1
	rsb	r5, r2, r5
	bl	UnMarkLTFrmByFrmIdx
.L336:
	mov	r3, r6
	mov	r2, r5
	mov	r1, r8
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MarkPicLTRef
.L335:
	add	r3, r0, #24576
	ldr	r5, [r6, #528]
	ldr	r1, [r3, #2064]
	rsb	r5, r2, r5, lsl #1
	cmp	r1, #0
	beq	.L336
	add	r2, r0, #26368
	mov	lr, #0
	add	r2, r2, #132
	b	.L339
.L337:
	tst	r3, #2
	beq	.L338
	ldr	r3, [ip, #584]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L350
.L338:
	cmp	lr, r1
	beq	.L336
.L339:
	ldr	ip, [r2, #4]!
	add	lr, lr, #1
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L337
	ldr	r4, [ip, #548]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	bne	.L337
	ldr	r0, [ip, #560]
	cmp	r0, r5
	bne	.L337
	mov	r1, ip
	mov	r3, r8
	mov	r2, #1
	mov	r0, r7
	bl	UnMarkLTFldByFrmIdx
	b	.L336
.L350:
	ldr	r3, [ip, #596]
	cmp	r3, r5
	bne	.L338
	mov	r1, ip
	mov	r3, r8
	mov	r2, #2
	mov	r0, r7
	bl	UnMarkLTFldByFrmIdx
	b	.L336
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
	ldr	r3, [r6, #2068]
	str	r1, [r6, #2072]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, r0, #26368
	mov	r3, r1
	add	r5, r5, #196
	mov	r4, #0
	b	.L355
.L353:
	ldr	r3, [r6, #2068]
	cmp	r3, r4
	ldmlsfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r6, #2072]
.L355:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	ldr	r2, [r1, #632]
	cmp	r2, r3
	bcc	.L353
	mov	r0, r7
	bl	UnMarkFrameStoreRef
	b	.L353
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
	ldr	r3, [r7, #2064]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r5, r0, #26368
	mov	r4, #0
	add	r5, r5, #132
.L358:
	ldr	r1, [r5, #4]!
	mov	r0, r6
	bl	UnMarkFrameStoreRef
	ldr	r3, [r7, #2064]
	add	r4, r4, #1
	cmp	r3, r4
	bhi	.L358
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
	beq	.L364
	mov	r3, r5
	ldr	r1, [r1, #520]
	bl	UnMarkLTFldByFrmIdx
.L363:
	mov	r2, #1
	mov	r3, #0
	str	r5, [r4, #532]
	strb	r2, [r4, #4]
	strb	r3, [r4, #5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L364:
	mov	r1, r5
	bl	UnMarkLTFrmByFrmIdx
	b	.L363
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
	ldr	r2, [r5, #1864]
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r2, #664]
	add	r7, r0, #24576
	mov	r1, #0
	add	r8, r0, #53248
	add	r3, r3, #6656
	add	r3, r3, #6
	add	r3, r0, r3, lsl #2
	str	r1, [r3, #4]
	ldr	r3, [r7, #2336]
	cmp	r3, r1
	subne	r3, r3, #1
	str	r3, [r7, #2336]
	ldr	r1, [r2, #620]
	ldr	r0, [r8, #4056]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L368
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L369
	ldr	r3, [r5, #1864]
	mov	r2, #0
	ldr	r0, [r8, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetDisplay
.L369:
	ldr	r3, [r5, #1864]
	mov	r2, #0
	ldr	r0, [r8, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetRef
.L368:
	ldr	r2, [r5, #1864]
	mov	r3, #0
	strb	r3, [r2, #1]
	ldr	r2, [r5, #1864]
	strb	r3, [r2, #4]
	ldr	r2, [r5, #1864]
	strb	r3, [r2, #2]
	ldr	r1, [r7, #2060]
	sub	r1, r1, #1
	cmp	r4, r1
	addcc	r3, r4, #6592
	addcc	r3, r3, #18
	addcc	r3, r6, r3, lsl #2
	bcs	.L372
.L371:
	add	r4, r4, #1
	ldr	r2, [r3, #4]!
	cmp	r4, r1
	str	r2, [r3, #-4]
	bne	.L371
.L372:
	add	r3, r1, #6592
	mov	r2, #0
	add	r3, r3, #18
	str	r2, [r6, r3, asl #2]
	str	r1, [r7, #2060]
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
	beq	.L396
	add	r3, r5, #24576
	ldr	r6, [r3, #2060]
	cmp	r6, #0
	addne	r7, r5, #53248
	movne	r10, #0
	movne	r8, r10
	movne	r9, r10
	bne	.L394
	b	.L395
.L405:
	ldrb	r1, [r2, #4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L392
	ldr	r0, [r2, #640]
	cmp	r0, #1
	beq	.L392
	strb	r1, [r2, #1]
	mov	r0, r5
	mov	r1, r10
	add	r8, r8, #1
	bl	RemoveFrameStoreOutDPB
	cmp	r8, r6
	beq	.L395
.L394:
	add	r4, r5, r10, lsl #2
	ldr	r0, [r7, #4056]
	add	r4, r4, #24576
	ldr	r2, [r4, #1864]
	ldr	r1, [r2, #620]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L389
	ldrsh	r2, [r0]
	cmp	r2, #0
	bne	.L390
	ldr	r1, [r4, #1864]
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L391
	ldrsb	r0, [r0]
	cmp	r0, #0
	bne	.L404
.L391:
	mvn	r3, #0
	str	r3, [r1, #620]
	ldr	r2, [r4, #1864]
	strb	r9, [r2, #2]
.L389:
	ldr	r1, [r4, #1864]
	ldr	r2, [r1, #640]
	sub	r2, r2, #1
	clz	r2, r2
	mov	r2, r2, lsr #5
	strb	r2, [r1, #4]
.L390:
	ldr	r2, [r4, #1864]
	ldrb	r1, [r2, #2]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L405
.L392:
	add	r8, r8, #1
	add	r10, r10, #1
	cmp	r8, r6
	bne	.L394
.L395:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L404:
	ldr	r1, [r1, #620]
	ldr	r0, [r7, #4056]
	bl	FSP_SetRef
	ldr	r1, [r4, #1864]
	b	.L391
.L396:
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
	add	r3, r0, #53248
	mov	r5, r0
	mov	r4, r1
	ldr	r2, [r3, #2808]
	cmp	r2, #0
	bne	.L407
	ldr	r2, [r1, #640]
	cmp	r2, #1
	beq	.L408
	ldr	r2, [r3, #3984]
	cmp	r2, #0
	beq	.L432
.L408:
	mov	r2, #1
	str	r2, [r3, #2808]
.L407:
	ldr	r0, [r3, #4056]
	ldr	r1, [r4, #620]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L410
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	cmpne	r3, #0
	beq	.L433
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L418
	cmp	r3, #1
	beq	.L418
	ldr	r3, [r4]
	mov	r1, #768
	movt	r1, 768
	bic	r3, r3, #16711680
	bic	r3, r3, #255
	cmp	r3, r1
	beq	.L434
	cmp	r2, #1
	beq	.L435
	cmp	r2, #2
	beq	.L436
.L416:
	ldr	r3, [r5]
	ldr	r2, [r0, #208]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	movls	r0, #0
	bhi	.L437
.L431:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L432:
	ldr	r2, [r0]
	ldr	r2, [r2, #12]
	cmp	r2, #0
	bne	.L408
	mvn	r0, #0
	b	.L431
.L433:
	ldr	ip, .L438
	mov	r0, #1
	ldr	r2, [r4, #620]
	ldr	r1, .L438+4
	ldr	ip, [ip, #68]
	blx	ip
.L412:
	ldr	r3, [r4, #640]
	cmp	r3, #1
	beq	.L419
	mvn	r0, #2
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L435:
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L416
	ldr	ip, .L438
	mov	r0, r2
	ldr	r1, .L438+8
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L431
.L436:
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	tst	r3, #2
	beq	.L416
	ldr	ip, .L438
	mov	r0, #1
	ldr	r1, .L438+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L431
.L419:
	mov	r3, #2
	mvn	r0, #2
	str	r3, [r4, #640]
	b	.L431
.L410:
	ldr	r1, .L438
	mov	r2, r4
	ldr	lr, [r4, #620]
	mov	r0, #1
	ldr	r3, [r4, #640]
	ldr	ip, [r1, #68]
	ldr	r1, .L438+16
	str	lr, [sp]
	blx	ip
	b	.L412
.L418:
	ldr	ip, .L438
	mov	r0, #1
	ldr	r1, .L438+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L431
.L437:
	ldr	ip, .L438
	mov	r0, #1
	ldr	r1, .L438+24
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L431
.L434:
	ldr	ip, .L438
	mov	r0, #1
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	ldr	r1, .L438+28
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L431
.L439:
	.align	2
.L438:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldrd	r2, [r1, #16]
	mvn	r5, #0
	mvn	r4, #0
	cmp	r3, r5
	mov	r10, r1
	cmpeq	r2, r4
	str	r0, [fp, #-52]
	beq	.L440
	add	r9, r0, #24576
	ldr	r3, [r9, #2060]
	cmp	r3, #0
	beq	.L440
	add	r8, r0, #26368
	add	r5, r0, #53248
	add	r8, r8, #68
	mvn	r6, #1
	mvn	r7, #0
	mov	r4, #0
	mvn	r3, #0
	str	r3, [fp, #-48]
.L445:
	ldr	r3, [r8, #4]!
	cmp	r3, #0
	beq	.L444
	ldr	r1, [r3, #620]
	ldr	r0, [r5, #4056]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L444
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	cmpne	r3, #0
	beq	.L444
	ldr	r3, [r8]
	ldrd	r0, [r3, #16]
	cmp	r1, r7
	cmpeq	r0, r6
	strcc	r4, [fp, #-48]
	movcc	r6, r0
	movcc	r7, r1
.L444:
	ldr	r3, [r9, #2060]
	add	r4, r4, #1
	cmp	r4, r3
	bcc	.L445
	ldr	r3, [fp, #-48]
	cmn	r3, #1
	beq	.L440
	ldrd	r2, [r10, #16]
	cmp	r3, r7
	cmpeq	r2, r6
	bhi	.L459
.L440:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L459:
	ldr	ip, .L460
	mov	r0, #29
	strd	r6, [sp]
	ldr	r1, .L460+4
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r3, [fp, #-48]
	ldr	r1, [fp, #-52]
	add	r8, r3, #6592
	ldrd	r2, [r10, #16]
	add	r8, r8, #18
	ldr	r1, [r1, r8, asl #2]
	strd	r2, [r1, #16]
	strd	r6, [r10, #16]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L461:
	.align	2
.L460:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r2, r0, #20480
	mov	r8, #2208
	mov	r4, r1
	ldr	r3, [lr, #1304]
	add	r5, r0, #53248
	ldr	r2, [r2, #220]
	mov	r7, #4000
	ldr	ip, [r1, #168]
	ldr	r6, [lr, #1300]
	mla	r1, r8, r2, r3
	bic	r2, ip, #768
	bic	r2, r2, #3
	ldrb	lr, [r4, #1]	@ zero_extendqisi2
	ldr	r3, [r1, #28]
	str	r2, [r4, #168]
	ldr	r1, [r5, #2816]
	mla	r3, r7, r3, r6
	mov	r1, r1, asl #10
	str	r1, [r4, #196]
	cmp	lr, #3
	ldrls	pc, [pc, lr, asl #2]
	b	.L498
.L465:
	.word	.L464
	.word	.L466
	.word	.L467
	.word	.L464
.L467:
	ldr	r1, [r4, #652]
	mov	r7, #3
	ldr	lr, [r4, #656]
	add	r6, r0, #24576
	ldr	ip, [r4, #608]
	and	r1, r1, r7
	orr	r2, r2, r1
	str	r7, [r4, #160]
	orr	r2, r2, #2816
	str	lr, [r4, #208]
	str	r2, [r4, #168]
	str	ip, [r4, #164]
.L463:
	ldrb	r2, [r3, #18]	@ zero_extendqisi2
	str	r2, [r4, #492]
	str	r2, [r4, #496]
	ldrb	r2, [r3, #22]	@ zero_extendqisi2
	cmp	r2, #0
	ldrne	r2, [r3, #68]
	mov	r3, #1
	str	r3, [r4, #500]
	str	r2, [r4, #504]
	ldr	r3, [r0]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ble	.L484
	ldrb	r3, [r4, #512]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L484
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #3
	ldreq	r3, [r4, #168]
	biceq	r3, r3, #768
	streq	r3, [r4, #168]
.L484:
	mov	r1, r4
	bl	ExchangePts
	ldrb	r3, [r6, #1816]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L485
	ldr	r3, [r6, #1832]
	cmp	r3, #3
	moveq	r3, #1
	streq	r3, [r4, #372]
	beq	.L485
	cmp	r3, #4
	moveq	r3, #2
	streq	r3, [r4, #372]
	beq	.L485
	cmp	r3, #5
	moveq	r3, #3
	streq	r3, [r4, #372]
	beq	.L485
	ldr	ip, .L509
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #372]
	ldr	r1, .L509+4
	ldr	r2, [r6, #1832]
	ldr	r3, [ip, #68]
	blx	r3
.L485:
	ldr	r0, [r5, #4056]
	ldr	r1, [r4, #620]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L509
	mov	r2, #504
	add	r1, r4, #8
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #0
	str	r3, [r5, #384]
	mov	r2, #1
	ldrb	r3, [r4, #512]	@ zero_extendqisi2
	strb	r2, [r5, #379]
	cmp	r3, #0
	movne	r3, #4
	strb	r3, [r5, #372]
	ldr	r3, [r4, #540]
	strb	r2, [r5, #377]
	str	r3, [r5, #388]
	str	r3, [r5, #396]
	str	r3, [r5, #392]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L466:
	ldr	r1, [r4, #652]
	mov	r7, #3
	ldr	lr, [r4, #656]
	add	r6, r0, #24576
	ldr	ip, [r4, #572]
	and	r1, r1, r7
	orr	r2, r2, r1
	str	r7, [r4, #164]
	orr	r2, r2, #1792
	str	lr, [r4, #208]
	str	r2, [r4, #168]
	str	ip, [r4, #160]
	b	.L463
.L464:
	ldrb	r2, [r4, #512]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L468
	ldr	r2, [r4, #648]
	mov	r9, #3072
	ldr	r1, [r4, #656]
	cmp	r2, #0
	ldr	r7, [r4, #572]
	ldr	r2, [r4, #652]
	ldr	lr, [r4, #608]
	moveq	r8, #512
	movne	r8, #768
	str	r1, [r4, #208]
.L470:
	ldr	r6, [r4, #564]
	ldr	r1, [r4, #600]
	cmp	r6, r1
	beq	.L506
	movle	r1, #1
	movgt	r1, #0
.L481:
	add	r6, r0, #24576
	ldrb	r10, [r6, #1852]	@ zero_extendqisi2
	cmp	r10, #3
	moveq	r1, #4096
	beq	.L482
	cmp	r10, #4
	movne	r1, r1, asl #12
	moveq	r1, #0
.L482:
	bic	ip, ip, #13056
	orr	r8, r9, r8
	bic	ip, ip, #3
	and	r2, r2, #3
	orr	ip, r8, ip
	str	r7, [r4, #160]
	orr	r2, ip, r2
	str	lr, [r4, #164]
	orr	r2, r2, r1
	str	r2, [r4, #168]
	b	.L463
.L468:
	ldr	r7, [r4, #568]
	ldr	r2, [r5, #4052]
	ldr	r1, [r4, #604]
	cmp	r7, r2
	bhi	.L507
	cmp	r2, r1
	bcc	.L474
.L472:
	ldr	r1, [r4, #656]
	mov	r8, #3
	ldr	r2, [r4, #652]
	ldr	r6, [r4, #572]
	str	r1, [r4, #208]
	ldr	r1, [r4, #608]
.L475:
	ldrb	lr, [r4, #3]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L508
	cmp	lr, #2
	streq	r7, [r4, #208]
	movne	r9, r8, asl #10
	moveq	r2, r6
	moveq	r9, #1024
.L477:
	mov	r7, r6
	mov	lr, r1
	mov	r8, #768
	str	r6, [r4, #160]
	str	r1, [r4, #164]
	b	.L470
.L474:
	ldr	r2, [r4, #572]
	mov	r8, #1
	ldr	r1, [r4, #608]
	str	r7, [r4, #208]
	mov	r6, r2
	b	.L475
.L506:
	ldr	r1, [r4, #172]
	cmp	r1, #720
	movne	r1, #1
	bne	.L481
	ldr	r1, [r4, #176]
	subs	r1, r1, #480
	movne	r1, #1
	b	.L481
.L507:
	cmp	r2, r1
	bcc	.L472
	ldr	r2, [r4, #608]
	mov	r8, #2
	str	r1, [r4, #208]
	ldr	r6, [r4, #572]
	mov	r1, r2
	b	.L475
.L508:
	ldr	lr, [r4, #604]
	mov	r2, r1
	mov	r9, #2048
	str	lr, [r4, #208]
	b	.L477
.L498:
	add	r6, r0, #24576
	b	.L463
.L510:
	.align	2
.L509:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC19
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
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r1, #84]
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
	ldrb	lr, [r0, #513]	@ zero_extendqisi2
	mov	r5, #1
	ldrb	ip, [r0, #514]	@ zero_extendqisi2
	mov	r4, #2
	ldr	r1, [r0, #532]
	ldr	r2, [r0, #536]
	ldr	r3, [r0, #516]
	strb	r5, [r0, #548]
	strb	r4, [r0, #584]
	strb	lr, [r0, #549]
	strb	lr, [r0, #585]
	strb	ip, [r0, #550]
	strb	ip, [r0, #586]
	str	r1, [r0, #568]
	str	r1, [r0, #604]
	str	r2, [r0, #572]
	str	r2, [r0, #608]
	str	r3, [r0, #552]
	str	r3, [r0, #588]
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
	ldr	r2, [r0, #600]
	mov	lr, #3
	ldr	r1, [r0, #564]
	ldr	r3, [r0, #604]
	cmp	r2, r1
	ldr	r5, [r0, #568]
	ldrb	r4, [r0, #585]	@ zero_extendqisi2
	ldr	ip, [r0, #552]
	movge	r2, r1
	add	r3, r3, r5
	cmp	r4, #0
	strb	lr, [r0, #512]
	mov	r3, r3, lsr #1
	str	ip, [r0, #516]
	str	r2, [r0, #528]
	str	r2, [r0, #636]
	str	r3, [r0, #532]
	beq	.L515
	ldrb	r2, [r0, #549]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L516
.L515:
	ldr	r2, [r0, #584]
	mov	r1, #0
	strb	r1, [r0, #513]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, r1
	beq	.L520
	ldr	r2, [r0, #548]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	movne	ip, #1
	bne	.L518
.L520:
	mov	ip, #0
.L518:
	ldr	r2, [r0, #608]
	ldr	r1, [r0, #572]
	str	r3, [r0, #656]
	cmp	r2, r1
	strb	ip, [r0, #514]
	movcs	r3, r2
	movcc	r3, r1
	str	r3, [r0, #536]
	str	r3, [r0, #652]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L516:
	mov	r1, #1
	strb	r1, [r0, #513]
	b	.L520
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
	beq	.L551
	cmp	r2, #1
	beq	.L532
	cmp	r2, #2
	beq	.L533
	ldr	r8, [r7, #564]
	ldr	r9, [r7, #600]
.L534:
	add	r6, r5, #24576
	ldr	ip, [r6, #2332]
	cmp	ip, #0
	beq	.L543
	ldr	r4, [r6, #2076]
	cmp	r4, #0
	addne	r3, r5, #26624
	movne	r4, #0
	addne	r3, r3, #28
	bne	.L537
	b	.L535
.L541:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L535
.L537:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L541
.L543:
	mvn	r0, #0
.L547:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L533:
	ldr	r8, [r7, #600]
	mov	r9, r8
	b	.L534
.L535:
	add	r3, r5, #53248
	ldr	r1, [r7, #620]
	ldr	r0, [r3, #4056]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L538
	ldr	r3, [r0, #520]
	cmp	r3, #0
	beq	.L538
	add	r5, r5, r4, lsl #2
	mov	r3, #1
	add	r5, r5, #24576
	ldr	r1, .L552
	str	r3, [r5, #2076]
	ldr	r3, [r0, #520]
	mov	r0, #13
	ldr	ip, [r1, #68]
	ldr	r1, .L552+4
	ldr	r3, [r3, #4]
	str	r8, [r5, #2204]
	str	r9, [r5, #2268]
	str	r3, [r5, #2140]
	ldr	r3, [r6, #2336]
	cmp	r3, #15
	addls	r2, r3, #1
	mov	r3, r4
	movhi	r2, #16
	str	r2, [r6, #2336]
	ldr	r2, [r7, #204]
	blx	ip
	mov	r0, #0
	str	r4, [r7, #664]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L532:
	ldr	r8, [r7, #564]
	mov	r9, r8
	b	.L534
.L538:
	ldr	r1, .L552
	ldr	r2, [r7, #620]
	ldrb	r3, [r7, #2]	@ zero_extendqisi2
	ldr	ip, [r1, #68]
	mov	r1, #0
	stmia	sp, {r0, r1}
	mov	r0, r1
	ldr	r1, .L552+8
	blx	ip
	mvn	r0, #0
	b	.L547
.L551:
	ldr	ip, .L552
	mov	r0, r7
	movw	r3, #2213
	ldr	r2, .L552+12
	ldr	r1, .L552+16
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L547
.L553:
	.align	2
.L552:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, [r2, #2068]
	ldr	r3, [r3, #1164]
	ldr	r2, [r2, #2064]
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r3, .L567
	mov	r5, r0
	ldr	r3, [r3]
	tst	r3, #16384
	beq	.L556
	add	r7, r0, #24576
	ldr	r3, [r7, #2060]
	cmp	r3, #0
	beq	.L566
	add	r8, r0, #26368
	ldr	r6, .L567+4
	add	r8, r8, #68
	mov	r4, #0
.L561:
	ldr	ip, [r8, #4]!
	mov	r2, r4
	ldr	r9, [r6, #68]
	mov	r0, #14
	ldr	r1, .L567+8
	add	r4, r4, #1
	ldrb	lr, [ip, #2]	@ zero_extendqisi2
	ldr	r3, [ip, #624]
	str	lr, [sp, #4]
	ldr	ip, [ip, #636]
	str	ip, [sp]
	blx	r9
	ldr	r3, [r7, #2060]
	cmp	r3, r4
	bhi	.L561
.L562:
	ldr	r3, [r6, #68]
	mov	r0, #14
	ldr	r1, .L567+12
	blx	r3
	ldr	r3, [r7, #2064]
	cmp	r3, #0
	addne	r5, r5, #26368
	movne	r4, #0
	addne	r5, r5, #132
	beq	.L560
.L563:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L567+16
	mov	r0, #14
	ldr	r8, [r6, #68]
	ldrb	lr, [ip, #2]	@ zero_extendqisi2
	ldr	r3, [ip, #624]
	str	lr, [sp, #4]
	ldr	ip, [ip, #636]
	str	ip, [sp]
	blx	r8
	ldr	r3, [r5]
	mov	r2, r4
	ldr	ip, [r6, #68]
	ldr	r1, .L567+20
	mov	r0, #14
	ldr	r3, [r3, #528]
	add	r4, r4, #1
	blx	ip
	ldr	r3, [r7, #2064]
	cmp	r3, r4
	bhi	.L563
.L560:
	ldr	r3, [r6, #68]
	mov	r0, #14
	ldr	r1, .L567+12
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L556:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L566:
	ldr	r6, .L567+4
	b	.L562
.L568:
	.align	2
.L567:
	.word	g_PrintEnable
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r3, r0, #53248
	ldrb	r2, [r3, #946]	@ zero_extendqisi2
	ldr	r4, [r3, #1464]
	strb	r2, [r4]
	ldrb	r2, [r3, #948]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
	bne	.L570
	ldrb	r2, [r3, #949]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
.L570:
	strb	r2, [r4, #2]
	ldr	r2, [r3, #1472]
	str	r2, [r4, #624]
	ldr	r2, [r3, #1508]
	str	r2, [r4, #656]
	ldr	r2, [r3, #1500]
	str	r2, [r4, #652]
	ldr	r2, [r3, #1484]
	str	r2, [r4, #636]
	ldr	r2, [r3, #1532]
	str	r2, [r4, #660]
	ldr	r2, [r3, #1476]
	str	r2, [r4, #632]
	ldrb	r2, [r3, #953]	@ zero_extendqisi2
	str	r2, [r4, #648]
	ldrb	r2, [r3, #951]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r1, #3
	movne	r1, #0
	strb	r1, [r4, #3]
	ldrb	r2, [r3, #947]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L573
	bcc	.L574
	cmp	r2, #2
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	str	r4, [r4, #588]
	mov	r0, #1
	strb	r0, [r4, #584]
	mov	r0, #0
	ldrb	ip, [r3, #948]	@ zero_extendqisi2
	strb	ip, [r4, #585]
	ldrb	ip, [r3, #949]	@ zero_extendqisi2
	strb	ip, [r4, #586]
	ldr	ip, [r3, #1492]
	str	ip, [r4, #600]
	ldr	lr, [r3, #1500]
	str	lr, [r4, #608]
	ldr	ip, [r3, #1508]
	str	r0, [r4, #648]
	str	ip, [r4, #604]
	ldrb	r5, [r3, #945]	@ zero_extendqisi2
	cmp	r5, r0
	beq	.L591
	ldr	r2, [r4, #568]
	mov	r5, #3
	strb	r5, [r4, #1]
	mvn	r7, #0
	add	ip, ip, r2
	str	ip, [r4, #656]
	str	ip, [r4, #532]
	mvn	r6, #0
	ldrb	r2, [r3, #951]	@ zero_extendqisi2
	cmp	r2, #1
	add	r2, r3, #944
	orreq	r0, r1, #2
	strb	r0, [r4, #3]
	ldrd	r8, [r2, #24]
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L582
	ldrd	r0, [r4, #16]
	cmp	r1, r7
	cmpeq	r0, r6
	beq	.L592
.L582:
	ldr	r2, [r4, #572]
.L589:
	cmp	r2, lr
	mov	r0, r4
	movcc	r2, lr
	str	r2, [r4, #652]
	ldr	r3, [r3, #1484]
	str	r3, [r4, #636]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	CombineFldsToFrm
.L574:
	str	r4, [r4, #516]
	mov	r1, #3
	mov	r2, #0
	strb	r1, [r4, #1]
	strb	r2, [r4, #512]
	add	r1, r3, #960
	ldrb	r5, [r3, #948]	@ zero_extendqisi2
	mov	r2, #504
	add	r0, r4, #8
	strb	r5, [r4, #513]
	ldrb	r6, [r3, #949]	@ zero_extendqisi2
	strb	r6, [r4, #514]
	ldr	ip, [r3, #1488]
	str	ip, [r4, #528]
	ldr	r7, [r3, #1500]
	str	r7, [r4, #536]
	ldr	r8, [r3, #1508]
	str	r8, [r4, #532]
	ldr	ip, [r3, #1492]
	str	ip, [r4, #564]
	ldr	r3, [r3, #1496]
	str	r3, [r4, #600]
	bl	memcpy
	mov	r2, #1
	mov	r3, #2
	str	r8, [r4, #568]
	str	r8, [r4, #604]
	str	r7, [r4, #572]
	str	r7, [r4, #608]
	strb	r6, [r4, #550]
	strb	r6, [r4, #586]
	strb	r5, [r4, #549]
	strb	r5, [r4, #585]
	str	r4, [r4, #552]
	str	r4, [r4, #588]
	strb	r2, [r4, #548]
	strb	r3, [r4, #584]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L573:
	strb	r2, [r4, #548]
	mov	r0, #0
	str	r4, [r4, #552]
	ldrb	ip, [r3, #948]	@ zero_extendqisi2
	strb	ip, [r4, #549]
	ldrb	ip, [r3, #949]	@ zero_extendqisi2
	strb	ip, [r4, #550]
	ldr	ip, [r3, #1492]
	str	ip, [r4, #564]
	ldr	lr, [r3, #1500]
	str	lr, [r4, #572]
	ldr	ip, [r3, #1508]
	str	r0, [r4, #648]
	str	ip, [r4, #568]
	ldrb	r5, [r3, #945]	@ zero_extendqisi2
	cmp	r5, r0
	beq	.L593
	ldr	r2, [r4, #604]
	mov	r5, #3
	strb	r5, [r4, #1]
	mvn	r7, #0
	add	ip, ip, r2
	str	ip, [r4, #656]
	str	ip, [r4, #532]
	mvn	r6, #0
	ldrb	r2, [r3, #951]	@ zero_extendqisi2
	cmp	r2, #1
	add	r2, r3, #944
	orreq	r0, r1, #1
	strb	r0, [r4, #3]
	ldrd	r8, [r2, #24]
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L578
	ldrd	r0, [r4, #16]
	cmp	r1, r7
	cmpeq	r0, r6
	streqd	r8, [r4, #16]
.L578:
	ldr	r2, [r4, #608]
	b	.L589
.L591:
	strb	r2, [r4, #1]
	add	r1, r3, #960
	str	ip, [r4, #656]
	add	r0, r4, #8
	str	ip, [r4, #532]
	mov	r2, #504
	ldrb	r3, [r3, #951]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r5, #2
	strb	r5, [r4, #3]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L593:
	strb	r2, [r4, #1]
	add	r1, r3, #960
	str	ip, [r4, #656]
	add	r0, r4, #8
	str	ip, [r4, #532]
	mov	r2, #504
	ldrb	r3, [r3, #951]	@ zero_extendqisi2
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r4, #3]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L592:
	strd	r8, [r4, #16]
	b	.L582
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
	ldr	r1, [r3, #2060]
	ldr	r2, [r3, #2056]
	cmp	r1, r2
	bcs	.L600
.L595:
	add	r0, r4, #53248
	mov	r2, #0
	ldr	r3, [r0, #1480]
	strb	r2, [r0, #948]
	cmp	r3, r2
	movne	r3, #1
	strneb	r3, [r0, #949]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L600:
	ldr	r1, [r3, #1864]
	bl	UnMarkFrameStoreRef
	mov	r1, #0
	mov	r0, r4
	bl	RemoveFrameStoreOutDPB
	b	.L595
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L609
	ldr	r7, .L610
	add	r8, r5, #56
	mov	r4, r5
	mov	r3, #0
	strb	r3, [r5, #1]
	strb	r3, [r5, #4]
.L604:
	ldr	r3, [r4, #8]
	mov	r0, r6
	cmp	r3, #0
	beq	.L603
	ldr	r1, [r4, #32]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #12]
	ldr	ip, [r7, #68]
	mov	r0, #7
	ldr	r2, [r4, #8]
	ldr	r1, .L610+4
	blx	ip
	mov	r3, #0
	str	r3, [r4, #8]
	str	r3, [r4, #24]
	str	r3, [r4, #12]
.L603:
	add	r4, r4, #28
	cmp	r4, r8
	bne	.L604
	mov	r3, #0
	strb	r3, [r5]
	str	r3, [r5, #68]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L609:
	ldr	ip, .L610
	mov	r0, r5
	mov	r3, #4160
	ldr	r2, .L610+8
	ldr	r1, .L610+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L611:
	.align	2
.L610:
	.word	vfmw_Osal_Func_Ptr_S
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
	beq	.L613
	add	r3, r0, #53248
	ldr	r0, [r3, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L613:
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
	ldr	r3, .L622
	mov	r5, r0
	ldr	r1, .L622+4
	mov	r0, #1
	add	r4, r5, #53248
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r5, #368]
	cmp	r1, #0
	beq	.L618
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r5, #368]
.L618:
	ldr	r3, [r4, #4012]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #4012]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L623:
	.align	2
.L622:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r6, r0, #53248
	b	.L626
.L625:
	add	r4, r4, #96
	cmp	r4, r5
	beq	.L628
.L626:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L625
	mov	r1, r4
	ldr	r0, [r6, #4056]
	add	r4, r4, #96
	bl	ReleaseNAL
	cmp	r4, r5
	bne	.L626
.L628:
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
	add	r4, r0, #55296
	add	r5, r0, #55808
	add	r4, r4, #128
	add	r5, r5, #160
	add	r6, r0, #53248
	mov	r7, #0
.L631:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L630
	ldr	r0, [r6, #4056]
	bl	ReleaseNAL
	str	r7, [r4]
.L630:
	cmp	r4, r5
	bne	.L631
	mov	r0, #0
	str	r0, [r6, #3980]
	str	r0, [r6, #4040]
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #40)
	sub	sp, sp, #40
	ldr	r7, .L645
	mov	r4, r1
	mov	r6, r2
	mov	r5, r0
	mov	r2, #32
	ldr	r3, [r7, #48]
	sub	r0, fp, #68
	mov	r1, #0
	blx	r3
	ldr	r2, [r5]
	str	r6, [fp, #-40]
	mov	r3, #0
	str	r4, [fp, #-60]
	ldr	r2, [r2, #28]
	str	r6, [fp, #-56]
	cmp	r2, #24
	str	r4, [fp, #-52]
	str	r6, [fp, #-48]
	str	r4, [fp, #-44]
	strb	r3, [fp, #-68]
	strb	r3, [fp, #-67]
	beq	.L638
	add	r1, r5, #24576
	ldrb	r2, [fp, #4]	@ zero_extendqisi2
	strb	r3, [fp, #-63]
	mov	r0, #5
	ldr	r3, [r1, #2056]
	strb	r0, [fp, #-62]
	add	r3, r3, #1
	strb	r3, [fp, #-64]
.L642:
	add	r6, r5, #53248
	sub	r1, fp, #68
	strb	r2, [fp, #-61]
	mov	r8, #0
	ldr	r0, [r6, #4056]
	mov	r4, #1
	strb	r8, [fp, #-65]
	strb	r4, [fp, #-66]
	bl	FSP_ConfigInstance
	subs	r9, r0, #0
	bne	.L639
	ldr	r1, [r5]
	sub	r3, fp, #72
	ldr	r0, [r6, #4056]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_PartitionFsMemory
	cmp	r0, r8
	moveq	r0, r4
	bne	.L644
.L643:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L638:
	mov	r1, #5
	strb	r3, [fp, #-64]
	strb	r3, [fp, #-63]
	mov	r2, #1
	strb	r1, [fp, #-62]
	b	.L642
.L639:
	ldr	r3, [r7, #68]
	mov	r0, r8
	ldr	r1, .L645+4
	blx	r3
	mvn	r0, #19
	b	.L643
.L644:
	ldr	r3, [r7, #68]
	mov	r0, r9
	ldr	r1, .L645+8
	blx	r3
	mvn	r0, #19
	b	.L643
.L646:
	.align	2
.L645:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC30
	.word	.LC29
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
	beq	.L649
	bcc	.L664
	cmp	r2, #2
	bne	.L673
.L665:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L673:
	ldr	r3, .L677
	mov	r0, #1
	ldr	r1, .L677+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L664:
	mov	r6, #1
.L650:
	add	r0, r4, #20480
	add	lr, r4, #26880
	add	r0, r0, #248
	mov	r5, #0
	add	lr, lr, #164
	add	r7, r4, #53248
	ldr	r3, [r0, #4]!
	mov	r2, r5
	mov	r8, #1
	cmp	r3, #0
	beq	.L652
.L675:
	ldr	r10, [lr]
	cmp	r10, #0
	bne	.L666
	mov	ip, lr
	mov	r1, r10
	b	.L654
.L655:
	ldr	r9, [ip, #4]!
	cmp	r9, #0
	bne	.L653
.L654:
	add	r1, r1, #1
	cmp	r1, r3
	bne	.L655
	ldr	r1, [r4]
	ldr	r1, [r1, #12]
	cmp	r1, #2
	bne	.L674
	ldr	r5, [r7, #1464]
	add	r5, r5, #512
.L657:
	mov	r9, r10
	mov	ip, lr
	mov	r1, #0
	b	.L663
.L659:
	ldr	r9, [ip, #4]!
.L663:
	add	r1, r1, #1
	cmp	r9, #0
	streq	r5, [ip]
	cmp	r1, r3
	bne	.L659
	add	r2, r2, #1
	add	lr, lr, #132
	cmp	r6, r2
	bls	.L665
.L672:
	ldr	r3, [r0, #4]!
	cmp	r3, #0
	bne	.L675
.L652:
	ldr	r1, [r4]
	ldr	r1, [r1, #12]
	cmp	r1, #2
	bne	.L676
	add	r2, r2, #1
	str	r8, [r0]
	cmp	r6, r2
	ldr	r3, [r7, #1464]
	add	lr, lr, #132
	add	r3, r3, #512
	str	r3, [lr, #-132]
	bhi	.L672
	b	.L665
.L666:
	mov	r9, r10
.L653:
	mov	r5, r9
	b	.L657
.L649:
	mov	r6, #2
	b	.L650
.L676:
	ldr	ip, .L677
	ldr	r1, .L677+8
.L670:
	ldr	ip, [ip, #68]
	mov	r0, #1
	blx	ip
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L674:
	ldr	ip, .L677
	mov	r2, r5
	mov	r3, #0
	ldr	r1, .L677+12
	b	.L670
.L678:
	.align	2
.L677:
	.word	vfmw_Osal_Func_Ptr_S
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
	beq	.L680
	ldrb	r3, [r3, #210]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r4, #2
	moveq	r4, #1
.L680:
	add	r3, r0, #24576
	ldr	r5, [r3, #2064]
	cmp	r5, #0
	beq	.L688
	add	r3, r0, #26368
	mov	r2, #0
	add	r3, r3, #136
	b	.L685
.L698:
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L696
.L683:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L697
.L685:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L698
	ldrb	lr, [r0, #2]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L684
	ldr	ip, [r0, #548]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	beq	.L699
.L684:
	tst	lr, #2
	beq	.L683
	ldr	ip, [r0, #584]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L683
	ldr	ip, [r0, #596]
	cmp	ip, r1
	bne	.L683
	add	r0, r0, #584
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L696:
	ldr	ip, [r0, #512]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L683
	ldr	ip, [r0, #524]
	cmp	ip, r1
	bne	.L683
	add	r0, r0, #512
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L699:
	ldr	ip, [r0, #560]
	cmp	ip, r1
	bne	.L684
	add	r0, r0, #548
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L697:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L688:
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
	beq	.L701
	ldrb	r3, [r3, #210]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r4, #2
	moveq	r4, #1
.L701:
	add	r3, r0, #24576
	ldr	r5, [r3, #2068]
	cmp	r5, #0
	beq	.L709
	add	r3, r0, #26368
	mov	r2, #0
	add	r3, r3, #200
	b	.L706
.L719:
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L717
.L704:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L718
.L706:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L719
	ldrb	lr, [r0, #2]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L705
	ldr	ip, [r0, #548]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	beq	.L720
.L705:
	tst	lr, #2
	beq	.L704
	ldr	ip, [r0, #584]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L704
	ldr	ip, [r0, #592]
	cmp	ip, r1
	bne	.L704
	add	r0, r0, #584
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L717:
	ldr	ip, [r0, #512]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L704
	ldr	ip, [r0, #520]
	cmp	ip, r1
	bne	.L704
	add	r0, r0, #512
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L720:
	ldr	ip, [r0, #556]
	cmp	ip, r1
	bne	.L705
	add	r0, r0, #548
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L718:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L709:
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
	ble	.L722
	add	lr, r5, r2, lsl #2
	mov	ip, r2
.L723:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L723
.L722:
	add	r3, r4, #1
	str	r3, [r7]
	str	r0, [r5, r4, asl #2]
	ldr	ip, [r7]
	cmp	r2, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r5, ip, lsl #2
	mov	lr, ip
.L727:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L725
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L726
	ldr	r3, [r0, #12]
	cmp	r3, r6
	beq	.L725
.L726:
	str	r0, [r5, lr, asl #2]
	add	lr, lr, #1
.L725:
	cmp	ip, r2
	bne	.L727
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
	ble	.L735
	add	lr, r5, r2, lsl #2
	mov	ip, r2
.L736:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L736
.L735:
	add	r3, r4, #1
	str	r3, [r7]
	str	r0, [r5, r4, asl #2]
	ldr	ip, [r7]
	cmp	r2, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r5, ip, lsl #2
	mov	lr, ip
.L740:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L738
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L739
	ldr	r3, [r0, #8]
	cmp	r3, r6
	beq	.L738
.L739:
	str	r0, [r5, lr, asl #2]
	add	lr, lr, #1
.L738:
	cmp	ip, r2
	bne	.L740
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
	beq	.L751
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
	ldr	r3, [r3, #2904]
	add	r3, r3, #4
	mov	r3, r1, asl r3
	str	r3, [fp, #-60]
.L761:
	adds	r9, r8, #2
	beq	.L750
	ldr	r3, [ip]
	cmp	r3, #3
	beq	.L750
	mov	r2, r8
	ldr	r6, [fp, #8]
	ldr	r5, [fp, #-64]
	mov	r4, #0
	mov	r8, ip
	mov	r10, r2
	b	.L754
.L755:
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
	beq	.L750
.L760:
	ldr	r3, [r8, #4]!
	add	r6, r6, #4
	cmp	r3, #3
	beq	.L750
.L754:
	cmp	r3, #1
	bhi	.L755
	cmp	r3, #0
	ldr	r3, [r6]
	bne	.L756
	mvn	r3, r3
	adds	r5, r5, r3
	ldrmi	r3, [fp, #-60]
	addmi	r5, r5, r3
.L757:
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
	bne	.L760
.L750:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L756:
	add	r3, r3, #1
	add	r5, r5, r3
	ldr	r3, [fp, #-60]
	cmp	r3, r5
	rsble	r5, r3, r5
	b	.L757
.L751:
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
	ldr	r3, [r3, #2904]
	add	r3, r3, #4
	mov	r3, r0, asl r3
	str	r3, [fp, #-60]
	b	.L761
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
	beq	.L769
	add	r6, r0, #16384
	ldrb	r2, [r6, #3512]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L781
.L772:
	cmp	r3, #1
	ldr	r3, [r4, #264]
	add	r3, r3, #1
	str	r3, [r4, #252]
	bne	.L769
	ldrb	r3, [r6, #3513]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L782
.L774:
	ldr	r3, [r4, #268]
	add	r3, r3, #1
	str	r3, [r4, #256]
.L769:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L781:
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
	add	r1, r1, #164
	bl	ReorderRefPiclist
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	b	.L772
.L782:
	add	ip, r5, #20480
	add	r0, r5, #20224
	add	r2, r5, #19968
	add	r1, r5, #27136
	ldr	r3, [r4, #268]
	add	ip, ip, #76
	add	r0, r0, #72
	add	r2, r2, #64
	stmib	sp, {r0, ip}
	add	r1, r1, #40
	str	r2, [sp]
	mov	r0, r5
	add	r2, r5, #20736
	bl	ReorderRefPiclist
	b	.L774
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
	ldr	lr, .L846
	movcc	r4, r2
	ldr	ip, .L846+4
	movcs	r4, #31
	cmp	r6, #0
	str	r3, [fp, #-52]
	movne	r6, lr
	moveq	r6, ip
	cmp	r0, #1
	moveq	r5, #0
	moveq	r10, r5
	beq	.L786
	cmp	r0, #2
	moveq	r5, #0
	moveq	r10, r5
	bne	.L794
.L795:
	cmp	r10, r4
	movcs	r3, #0
	movcc	r3, #1
	cmp	r5, r4
	str	r3, [fp, #-48]
	movcs	r2, #0
	movcc	r2, #1
	orrs	r3, r2, r3
	beq	.L794
	cmp	r2, #0
	beq	.L800
	add	r8, r7, r5, lsl #2
	b	.L799
.L796:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L800
.L799:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	ip, [r0, #1]	@ zero_extendqisi2
	tst	ip, #2
	beq	.L796
	add	r0, r0, #584
	blx	r6
	cmp	r0, #0
	beq	.L796
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #584
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L800:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r10, lsl #2
	bne	.L802
	b	.L795
.L801:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L795
.L802:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	tst	r2, #1
	beq	.L801
	add	r0, r0, #548
	blx	r6
	cmp	r0, #0
	beq	.L801
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #548
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
	b	.L795
.L845:
	add	r0, r0, #548
	blx	r6
	cmp	r0, #0
	beq	.L787
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r8]
	ldr	r0, [r3]
	add	r2, r2, #548
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L791:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r5, lsl #2
	bne	.L793
	b	.L786
.L792:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L786
.L793:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	tst	r2, #2
	beq	.L792
	add	r0, r0, #584
	blx	r6
	cmp	r0, #0
	beq	.L792
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #584
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L786:
	cmp	r10, r4
	movcs	r2, #0
	movcc	r2, #1
	cmp	r5, r4
	movcs	r3, #0
	movcc	r3, #1
	str	r3, [fp, #-48]
	orrs	r3, r3, r2
	beq	.L794
	cmp	r2, #0
	beq	.L791
	add	r9, r7, r10, lsl #2
	b	.L790
.L787:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L791
.L790:
	mov	r8, r9
	ldr	r0, [r9], #4
	ldrb	ip, [r0, #1]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L787
	b	.L845
.L794:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L847:
	.align	2
.L846:
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
	add	r8, r0, #53248
	ldrb	r3, [r6, #208]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L849
	ldrb	r7, [r8, #947]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L973
	add	r3, r0, #24576
	str	r3, [fp, #-240]
	ldr	ip, [r3, #2064]
	cmp	ip, #0
	beq	.L916
	add	r1, r0, #26368
	mov	r3, r7
	add	r1, r1, #132
	b	.L855
.L854:
	cmp	r3, ip
	beq	.L853
.L855:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L854
	ldr	r0, [r2, #512]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #65536
	bne	.L854
	add	r0, r7, #6720
	cmp	r3, ip
	add	r0, r0, #40
	add	r2, r2, #512
	add	r7, r7, #1
	add	r0, r4, r0, lsl #2
	str	r2, [r0, #4]
	bne	.L855
.L853:
	add	r0, r4, #26880
	ldr	r3, .L981
	mov	r1, r7
	add	r0, r0, #164
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	str	r7, [r6, #252]
	ldr	r1, [r3, #2068]
	cmp	r1, #0
	beq	.L917
	add	r0, r4, #26368
	mov	r5, r7
	add	r0, r0, #196
	mov	r3, #0
	b	.L858
.L857:
	cmp	r3, r1
	beq	.L974
.L858:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L857
	ldr	ip, [r2, #512]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L857
	add	ip, r5, #6720
	cmp	r3, r1
	add	ip, ip, #40
	add	r2, r2, #512
	add	r5, r5, #1
	add	ip, r4, ip, lsl #2
	str	r2, [ip, #4]
	bne	.L858
.L974:
	rsb	r1, r7, r5
.L856:
	add	r0, r4, r7, lsl #2
	ldr	r3, .L981+4
	add	r0, r0, #26880
	mov	r2, #4
	add	r0, r0, #164
	bl	qsort
	mov	r2, r5
	cmp	r2, #0
	mov	r3, #0
	str	r5, [r6, #252]
	str	r3, [r6, #256]
	beq	.L975
.L864:
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
	bhi	.L908
	add	r2, r2, #6720
	add	r1, r4, #27136
	add	r2, r2, #40
	add	r1, r1, #36
	mov	r0, #0
	add	r2, r4, r2, lsl #2
.L907:
	str	r0, [r2, #4]!
	cmp	r2, r1
	bne	.L907
.L908:
	cmp	r3, #32
	bhi	.L906
	add	r3, r3, #6784
	add	r2, r4, #27136
	add	r3, r3, #9
	add	r2, r2, #168
	mov	r1, #0
	add	r4, r4, r3, lsl #2
.L909:
	str	r1, [r4, #4]!
	cmp	r4, r2
	bne	.L909
.L906:
	mov	r0, #0
.L959:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L849:
	ldrb	r5, [r8, #947]	@ zero_extendqisi2
	add	r3, r0, #24576
	str	r3, [fp, #-240]
	cmp	r5, #0
	beq	.L870
	ldr	ip, [r3, #2064]
	cmp	ip, #0
	beq	.L976
	add	r9, r0, #26368
	mov	r5, #0
	add	r9, r9, #132
	mov	r3, r5
	mov	r1, r9
.L889:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L888
	sub	r0, fp, #44
	ldr	lr, [r8, #1484]
	add	r7, r0, r5, lsl #2
	ldr	r0, [r2, #636]
	cmp	lr, r0
	strge	r2, [r7, #-192]
	addge	r5, r5, #1
.L888:
	cmp	r3, ip
	bne	.L889
	sub	r10, fp, #236
	ldr	r3, .L981+8
	mov	r2, #4
	mov	r1, r5
	mov	r0, r10
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2064]
	cmp	r7, #0
	beq	.L970
.L914:
	mov	r3, r9
	mov	r2, #0
	mov	r9, r5
.L892:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L891
	sub	r0, fp, #44
	ldr	ip, [r8, #1484]
	add	lr, r0, r9, lsl #2
	ldr	r0, [r1, #636]
	cmp	ip, r0
	strlt	r1, [lr, #-192]
	addlt	r9, r9, #1
.L891:
	cmp	r2, r7
	bne	.L892
	rsb	r7, r5, r9
	mov	r3, r5, asl #2
	add	r0, r10, r3
	str	r3, [fp, #-244]
	mov	r1, r7
	ldr	r3, .L981+12
	mov	r2, #4
	bl	qsort
	cmp	r5, #0
	subeq	ip, fp, #172
	beq	.L893
.L911:
	sub	ip, fp, #172
	mov	r2, r10
	add	r7, ip, r7, lsl #2
	mov	r3, #0
.L894:
	add	r3, r3, #1
	ldr	r1, [r2], #4
	cmp	r3, r5
	str	r1, [r7], #4
	bcc	.L894
.L893:
	cmp	r9, r5
	bls	.L895
	ldr	r3, [fp, #-244]
	add	r2, r10, r3
	mov	r3, ip
.L896:
	add	r5, r5, #1
	ldr	r1, [r2], #4
	cmp	r5, r9
	str	r1, [r3], #4
	bne	.L896
.L895:
	add	r2, r4, #20480
	add	r3, r4, #26880
	add	r2, r2, #252
	mov	r5, #0
	add	r3, r3, #164
	str	r5, [r6, #252]
	str	r5, [r6, #256]
	mov	r1, r10
	ldrb	r0, [r8, #947]	@ zero_extendqisi2
	add	r10, r4, #27136
	str	ip, [fp, #-252]
	mov	ip, r2
	str	r2, [fp, #-248]
	mov	r2, r9
	str	ip, [sp]
	add	r10, r10, #40
	str	r5, [sp, #4]
	str	r3, [fp, #-244]
	bl	GenPiclistfromFrmlist
	ldr	ip, [fp, #-252]
	mov	r2, r9
	ldrb	r0, [r8, #947]	@ zero_extendqisi2
	mov	r3, r10
	str	r5, [sp, #4]
	mov	r1, ip
	add	ip, r4, #20736
	str	ip, [sp]
	str	ip, [fp, #-252]
	bl	GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2068]
	cmp	r7, r5
	subeq	r9, fp, #108
	beq	.L897
	add	r3, r4, #26368
	sub	r9, fp, #108
	add	r3, r3, #196
	mov	r2, r9
.L898:
	add	r5, r5, #1
	ldr	r1, [r3, #4]!
	cmp	r5, r7
	str	r1, [r2], #4
	bne	.L898
.L897:
	ldr	r3, .L981+16
	mov	r2, #4
	mov	r1, r7
	mov	r0, r9
	bl	qsort
	ldr	r3, [fp, #-248]
	ldrb	r0, [r8, #947]	@ zero_extendqisi2
	mov	r5, #1
	mov	r2, r7
	mov	r1, r9
	str	r3, [sp]
	str	r5, [sp, #4]
	ldr	r3, [fp, #-244]
	bl	GenPiclistfromFrmlist
	ldr	r2, [fp, #-252]
	ldrb	r0, [r8, #947]	@ zero_extendqisi2
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
.L887:
	cmp	r2, #1
	movls	r1, #0
	andhi	r1, r1, #1
	cmp	r1, #0
	beq	.L899
	cmp	r2, #0
	ldr	r1, [fp, #-240]
	beq	.L900
	ldr	r7, [r1, #2468]
	ldr	r1, [r1, #2600]
	cmp	r7, r1
	bne	.L899
	add	r0, r4, #27136
	ldr	ip, [fp, #-244]
	add	r0, r0, #40
	mov	r1, #0
	b	.L901
.L902:
	ldr	r5, [ip, #4]!
	ldr	lr, [r0, #4]!
	cmp	r5, lr
	bne	.L899
.L901:
	add	r1, r1, #1
	cmp	r1, r2
	bne	.L902
.L903:
	ldr	r0, [fp, #-240]
	ldr	r1, [r0, #2604]
	str	r7, [r0, #2604]
	str	r1, [r0, #2600]
.L899:
	orrs	r1, r2, r3
	bne	.L864
	ldr	r3, .L981+20
	mov	r0, #1
	ldr	r1, .L981+24
.L965:
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	bl	ClearCurrSlice
	mvn	r0, #0
	b	.L959
.L870:
	ldr	r10, [r3, #2064]
	cmp	r10, #0
	beq	.L873
	add	r9, r0, #26368
	mov	r3, r5
	add	r9, r9, #132
	mov	r1, r9
	b	.L875
.L874:
	cmp	r3, r10
	beq	.L977
.L875:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L874
	add	r0, r5, #6720
	ldr	ip, [r2, #528]
	add	r0, r0, #40
	ldr	lr, [r8, #1488]
	add	r2, r2, #512
	add	r0, r4, r0, lsl #2
	cmp	lr, ip
	addge	r5, r5, #1
	strge	r2, [r0, #4]
	cmp	r3, r10
	bne	.L875
.L977:
	add	r3, r4, #26880
	mov	r1, r5
	mov	r2, r3
	add	r2, r2, #164
	ldr	r3, .L981+28
	mov	r0, r2
	str	r2, [fp, #-244]
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2064]
	cmp	r7, #0
	beq	.L969
.L912:
	mov	r3, r9
	mov	r10, r5
	mov	r2, #0
	b	.L878
.L877:
	cmp	r2, r7
	beq	.L978
.L878:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L877
	add	r0, r10, #6720
	ldr	ip, [r1, #528]
	add	r0, r0, #40
	ldr	lr, [r8, #1488]
	add	r1, r1, #512
	add	r0, r4, r0, lsl #2
	cmp	lr, ip
	addlt	r10, r10, #1
	strlt	r1, [r0, #4]
	cmp	r2, r7
	bne	.L878
.L978:
	add	r8, r5, #6720
	rsb	r7, r5, r10
	add	r8, r8, #40
	mov	r2, #4
	ldr	r3, .L981+32
	mov	r1, r7
	add	r0, r4, r8, lsl #2
	add	r9, r4, r10, lsl #2
	add	r0, r0, r2
	add	r9, r9, #26880
	bl	qsort
	cmp	r5, #0
	add	r9, r9, #164
	beq	.L880
.L910:
	add	r7, r7, #6784
	add	r2, r4, #26880
	add	r7, r7, #9
	add	r2, r2, #160
	mov	r3, #0
	add	r7, r4, r7, lsl #2
.L881:
	add	r3, r3, #1
	ldr	r1, [r2, #4]!
	cmp	r3, r5
	str	r1, [r7, #4]!
	bcc	.L881
.L880:
	cmp	r10, r5
	bls	.L882
	add	r3, r4, #27136
	add	r8, r4, r8, lsl #2
	add	r3, r3, #36
.L883:
	add	r5, r5, #1
	ldr	r2, [r8, #4]!
	cmp	r5, r10
	str	r2, [r3, #4]!
	bne	.L883
.L882:
	ldr	r3, [fp, #-240]
	str	r10, [r6, #256]
	str	r10, [r6, #252]
	ldr	r1, [r3, #2068]
	cmp	r1, #0
	beq	.L919
	add	r0, r4, #26368
	mov	r5, r10
	add	r0, r0, #196
	mov	r3, #0
	b	.L886
.L885:
	cmp	r3, r1
	beq	.L979
.L886:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #1]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L885
	ldrb	ip, [r2, #513]	@ zero_extendqisi2
	cmp	ip, #1
	bne	.L885
	add	ip, r4, r5, lsl #2
	cmp	r3, r1
	add	ip, ip, #24576
	add	r2, r2, #512
	add	r5, r5, #1
	str	r2, [ip, #2468]
	str	r2, [ip, #2600]
	bne	.L886
.L979:
	rsb	r1, r10, r5
.L884:
	mov	r0, r9
	ldr	r3, .L981+4
	mov	r2, #4
	bl	qsort
	ldr	r0, [r6, #252]
	ldr	r3, .L981+4
	mov	r2, #4
	rsb	r1, r0, r5
	add	r0, r4, r0, lsl #2
	add	r0, r0, #27136
	add	r0, r0, #40
	bl	qsort
	mov	r3, r5
	str	r5, [r6, #256]
	mov	r2, r5
	str	r5, [r6, #252]
	mov	r1, #1
	b	.L887
.L973:
	add	r2, r0, #24576
	str	r2, [fp, #-240]
	ldr	lr, [r2, #2064]
	cmp	lr, #0
	moveq	r7, lr
	beq	.L852
	add	r1, r0, #26368
	mov	r7, r3
	add	r1, r1, #132
.L861:
	ldr	r2, [r1, #4]!
	sub	r0, fp, #44
	add	ip, r0, r7, lsl #2
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	strne	r2, [ip, #-192]
	addne	r7, r7, #1
	cmp	r3, lr
	bne	.L861
.L852:
	add	r10, r4, #26880
	mov	r1, r7
	ldr	r3, .L981+36
	mov	r2, #4
	sub	r0, fp, #236
	add	r10, r10, #164
	bl	qsort
	add	ip, r4, #20480
	add	ip, ip, #252
	mov	r5, #0
	mov	r2, r7
	str	r5, [r6, #252]
	mov	r3, r10
	ldrb	r0, [r8, #947]	@ zero_extendqisi2
	sub	r1, fp, #236
	str	ip, [sp]
	str	r5, [sp, #4]
	str	ip, [fp, #-244]
	bl	GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	ip, [fp, #-244]
	ldr	r7, [r3, #2068]
	cmp	r7, r5
	subeq	r9, fp, #108
	beq	.L862
	add	r3, r4, #26368
	sub	r9, fp, #108
	add	r3, r3, #196
	mov	r2, r9
.L863:
	add	r5, r5, #1
	ldr	r1, [r3, #4]!
	cmp	r5, r7
	str	r1, [r2], #4
	bne	.L863
.L862:
	ldr	r3, .L981+16
	mov	r2, #4
	mov	r1, r7
	mov	r0, r9
	str	ip, [fp, #-244]
	bl	qsort
	ldr	ip, [fp, #-244]
	mov	r3, r10
	ldrb	r0, [r8, #947]	@ zero_extendqisi2
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
	bne	.L864
.L975:
	ldr	r3, [r4]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L865
	ldrb	r3, [r8, #947]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L980
	cmp	r3, #1
	ldr	r2, [fp, #-240]
	ldr	r3, [r8, #1464]
	addeq	r3, r3, #548
	addne	r3, r3, #584
	str	r3, [r2, #2468]
.L867:
	mov	r3, #0
	mov	r2, #1
	b	.L864
.L980:
	ldr	r2, [fp, #-240]
	ldr	r3, [r8, #1464]
	add	r3, r3, #512
	str	r3, [r2, #2468]
	b	.L867
.L900:
	ldr	r7, [r1, #2600]
	b	.L903
.L969:
	add	r8, r5, #6720
	mov	r2, #4
	add	r8, r8, #40
	ldr	r3, .L981+32
	mov	r1, r7
	add	r9, r4, r8, lsl #2
	add	r9, r9, r2
	mov	r0, r9
	bl	qsort
	subs	r10, r5, #0
	bne	.L910
	b	.L882
.L970:
	mov	r3, r5, asl #2
	mov	r1, r7
	str	r3, [fp, #-244]
	mov	r2, r3
	ldr	r3, .L981+12
	add	r0, r10, r2
	mov	r2, #4
	bl	qsort
	subs	r9, r5, #0
	bne	.L911
.L966:
	sub	ip, fp, #172
	b	.L895
.L916:
	mov	r7, ip
	b	.L853
.L976:
	sub	r10, fp, #236
	ldr	r3, .L981+8
	mov	r1, ip
	mov	r2, #4
	mov	r0, r10
	str	ip, [fp, #-244]
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	ip, [fp, #-244]
	ldr	r7, [r3, #2064]
	cmp	r7, #0
	addne	r9, r4, #26368
	movne	r5, ip
	addne	r9, r9, #132
	bne	.L914
	ldr	r3, .L981+12
	mov	r2, #4
	mov	r1, r7
	mov	r0, r10
	mov	r9, r7
	bl	qsort
	b	.L966
.L873:
	add	r3, r0, #26880
	mov	r1, r10
	mov	r2, r3
	add	r2, r2, #164
	ldr	r3, .L981+28
	mov	r0, r2
	str	r2, [fp, #-244]
	mov	r2, #4
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r7, [r3, #2064]
	cmp	r7, #0
	addne	r9, r4, #26368
	movne	r5, r10
	addne	r9, r9, #132
	bne	.L912
	ldr	r9, [fp, #-244]
	mov	r2, #4
	ldr	r3, .L981+32
	mov	r1, r7
	mov	r10, r7
	mov	r0, r9
	bl	qsort
	b	.L882
.L917:
	mov	r5, r7
	b	.L856
.L919:
	mov	r5, r10
	b	.L884
.L865:
	ldr	r3, .L981+20
	mov	r0, #1
	ldr	r1, .L981+40
	b	.L965
.L982:
	.align	2
.L981:
	.word	compare_pic_by_pic_num_desc
	.word	compare_pic_by_lt_pic_num_asc
	.word	compare_fs_by_poc_desc
	.word	compare_fs_by_poc_asc
	.word	compare_fs_by_lt_pic_idx_asc
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r3, .L985
	ldr	r3, [r3]
	tst	r3, #8192
	ldmeqfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DumpList.part.5
.L986:
	.align	2
.L985:
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
	ldr	r4, [r3, #2060]
	cmp	r4, #0
	beq	.L992
	ldr	r3, [r3, #1864]
	add	r2, r0, #53248
	ldr	r5, [r2, #1484]
	ldr	lr, [r3, #636]
	rsb	lr, lr, r5
	cmp	lr, #0
	rsblt	lr, lr, #0
	cmp	r4, #1
	bls	.L993
	add	r1, r0, #26368
	mov	r2, #1
	add	r1, r1, #72
	mov	r0, #0
.L991:
	ldr	ip, [r1, #4]!
	add	r2, r2, #1
	ldr	r3, [ip, #636]
	rsb	r3, r3, r5
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, lr
	ldrlt	r0, [ip, #204]
	movlt	lr, r3
	cmp	r2, r4
	bne	.L991
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L993:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L992:
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
	add	r2, r0, #53248
	add	r5, r0, #26880
	ldrb	ip, [r2, #947]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L1025
	ldr	lr, [r3, #252]
	mov	r4, r5
	mov	r6, r5
	mov	r7, ip
	cmp	lr, #0
	add	r5, r5, #36
	add	r4, r4, #40
	add	r6, r6, #160
	beq	.L1003
.L1027:
	mov	r2, r6
	mov	r0, #32
	mov	r3, #0
	b	.L1001
.L1000:
	add	r3, r3, #1
	cmp	r3, lr
	beq	.L1026
.L1001:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #664]
	cmp	r1, ip
	bne	.L1000
	cmp	r0, r3
	movcs	r0, r3
	add	r3, r3, #1
	cmp	r3, lr
	str	r0, [r4, #-4]
	str	r0, [r5, #4]
	bne	.L1001
.L1026:
	cmp	r0, #31
	bhi	.L1003
.L1002:
	add	ip, ip, #1
	add	r5, r5, #8
	cmp	ip, #16
	add	r4, r4, #8
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	lr, #0
	bne	.L1027
.L1003:
	str	r7, [r4, #-4]
	str	r7, [r5, #4]
	b	.L1002
.L1025:
	ldr	r4, [r3, #252]
	mov	ip, #0
	mov	r6, r5
	mov	r7, ip
	cmp	r4, #0
	add	r5, r5, #36
	add	r6, r6, #160
	beq	.L1007
.L1029:
	mov	r1, r6
	mov	lr, #32
	mov	r2, #0
	b	.L1005
.L1004:
	add	r2, r2, #1
	cmp	r2, r4
	beq	.L1028
.L1005:
	ldr	r3, [r1, #4]!
	ldr	r8, [r3, #4]
	ldrb	r0, [r3]	@ zero_extendqisi2
	ldr	r3, [r8, #664]
	mov	r3, r3, asl #1
	cmp	r0, #2
	orreq	r3, r3, #1
	cmp	r3, ip
	bne	.L1004
	cmp	lr, r2
	movcs	lr, r2
	add	r2, r2, #1
	cmp	r2, r4
	str	lr, [r5]
	bne	.L1005
.L1028:
	cmp	lr, #31
	bhi	.L1007
.L1006:
	add	ip, ip, #1
	add	r5, r5, #4
	cmp	ip, #32
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r4, #0
	bne	.L1029
.L1007:
	str	r7, [r5]
	b	.L1006
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
	beq	.L1043
	mov	r0, r6
	bl	InitListX
	cmp	r0, #0
	bne	.L1044
	ldr	r7, .L1047
	mov	r0, #13
	ldr	r4, .L1047+4
	ldr	r1, .L1047+8
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r3, [r4]
	tst	r3, #8192
	bne	.L1045
.L1034:
	mov	r0, r6
	bl	ReorderListX
	mov	r0, r6
	bl	RepairList
	cmp	r0, #0
	bne	.L1036
	mov	r0, r6
	bl	FindMinRefIdx
	add	r3, r6, #24576
	mov	r0, #13
	ldr	r1, .L1047+12
	ldr	r3, [r3, #2468]
	cmp	r3, #0
	ldrne	r3, [r3, #4]
	mvneq	r3, #0
	ldrne	r3, [r3, #204]
	str	r3, [r5, #1836]
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r4, [r4]
	ands	r4, r4, #8192
	bne	.L1046
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1046:
	mov	r0, r6
	bl	DumpList.part.5
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1043:
	mov	r0, #0
	str	r0, [r5, #252]
	str	r0, [r5, #256]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1045:
	mov	r0, r6
	bl	DumpList.part.5
	b	.L1034
.L1036:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1044:
	ldr	r3, .L1047
	mov	r0, #13
	ldr	r1, .L1047+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1048:
	.align	2
.L1047:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r4, r0, #53248
	ldr	ip, [r4, #2792]
	cmp	ip, #0
	addne	r3, r0, #27904
	movne	r2, #0
	addne	r3, r3, #52
	movne	lr, r2
	bne	.L1055
	b	.L1056
.L1064:
	ldrb	r1, [r3, #-640]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1053
	cmp	r2, ip
	add	r3, r3, #672
	beq	.L1056
.L1055:
	ldr	r1, [r3]
	add	r2, r2, #1
	cmp	r1, #1
	bne	.L1064
.L1053:
	cmp	r2, ip
	strb	lr, [r3, #-643]
	strb	lr, [r3, #-642]
	add	r3, r3, #672
	bne	.L1055
.L1056:
	ldr	r2, [r4, #2796]
	cmp	r2, #0
	beq	.L1065
	add	r0, r0, #55808
	mov	r3, #0
	add	r0, r0, #160
	mov	r1, r3
.L1057:
	add	r3, r3, #1
	str	r1, [r0, #4]!
	cmp	r3, r2
	bne	.L1057
	ldmfd	sp, {r4, fp, sp, pc}
.L1065:
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	NoPicOut, .-NoPicOut
	.align	2
	.global	GetBackPicFromVOQueue
	.type	GetBackPicFromVOQueue, %function
GetBackPicFromVOQueue:
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
	add	r7, r0, #53248
	mov	r9, r0
	ldr	r0, [r7, #4056]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1083
.L1067:
	ldr	r3, [r7, #2792]
	cmp	r3, #0
	addne	r5, r9, #27392
	movne	r8, #0
	addne	r5, r5, #152
	movne	r6, r8
	beq	.L1074
.L1073:
	sub	r4, r5, #16
	strb	r6, [r5, #-231]
	strb	r6, [r5, #-230]
.L1072:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L1071
	ldr	r0, [r7, #4056]
	bl	FreeUsdByDec
	str	r6, [r4]
.L1071:
	cmp	r4, r5
	bne	.L1072
	ldr	r3, [r7, #2792]
	add	r8, r8, #1
	add	r5, r5, #672
	cmp	r3, r8
	bhi	.L1073
.L1074:
	ldr	r1, [r7, #2796]
	cmp	r1, #0
	beq	.L1084
	add	r2, r9, #55808
	mov	r3, #0
	add	r2, r2, #160
	mov	r0, r3
.L1075:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L1075
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1083:
	add	r0, r9, #8
	bl	ResetVoQueue
	b	.L1067
.L1084:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
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
	ldr	r1, [r4, #3960]
	rsb	r3, r3, #2
	ldr	r2, [r4, #3956]
	mla	r3, r1, r3, r3
	add	r2, r2, #1
	cmp	r2, lr
	cmpeq	r3, r0
	movne	r1, #1
	moveq	r1, #0
	bne	.L1087
	add	r8, r5, #24576
	ldr	ip, [r4, #3980]
	ldr	r8, [r8, #2056]
	add	r8, r8, #1
	cmp	ip, r8
	movls	r3, r1
	movls	r0, r3
	bls	.L1088
.L1087:
	ldr	r1, .L1092
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L1091
	add	r1, r5, #53248
	mov	lr, lr, asl #4
	mov	r0, r0, asl #4
	mov	r2, r2, asl #4
	strh	r0, [fp, #-42]	@ movhi
	mov	r3, r3, asl #4
	strh	r2, [fp, #-40]	@ movhi
	sub	r2, fp, #44
	strh	r3, [fp, #-38]	@ movhi
	mov	r3, #8
	ldr	r0, [r1, #4056]
	mov	r1, #2
	strh	lr, [fp, #-44]	@ movhi
	blx	ip
	ldr	ip, [r4, #3980]
	mov	r3, #1
	mov	r0, r3
.L1088:
	add	r5, r5, #16384
	ldr	r2, [r4, #744]
	ldr	lr, [r7, #2052]
	ldr	r1, [r5, #1192]
	cmp	lr, r2
	cmpeq	r1, ip
	movne	r2, #1
	moveq	r2, #0
	orr	r3, r2, r3
	str	r3, [r6]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1091:
	mov	r3, #1
	ldr	ip, [r4, #3980]
	mov	r0, r3
	b	.L1088
.L1093:
	.align	2
.L1092:
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
	ldr	r1, [r5, #2908]
	ldr	r3, [r5, #2904]
	cmp	r1, ip
	ldr	lr, [r5, #2912]
	add	r3, r3, #4
	mov	r3, ip, asl r3
	beq	.L1096
	bcc	.L1097
	cmp	r1, #2
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r2, #0
	add	r4, r0, #16384
	bne	.L1152
	ldr	r1, [r4, #3444]
	cmp	r1, #0
	bne	.L1130
	ldr	r2, [r4, #3484]
	ldr	r0, [r4, #3480]
	ldr	r1, [r4, #3492]
	cmp	r2, r0
	addcc	r1, r1, r3
	strcc	r1, [r4, #3488]
	bcs	.L1132
.L1133:
	add	r3, r1, r2
	str	r3, [r4, #3476]
	ldr	r0, [r6, #240]
	mov	r3, r3, asl #1
	cmp	r0, #0
	subeq	r3, r3, #1
	str	r3, [r4, #3464]
	ldrb	r0, [r6, #209]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1136
	str	r3, [r4, #3460]
	str	r3, [r4, #3456]
	str	r3, [r4, #3452]
.L1129:
	str	r2, [r4, #3480]
	str	r1, [r4, #3492]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1097:
	cmp	r2, #0
	add	lr, lr, #4
	add	r4, r0, #16384
	mov	ip, ip, asl lr
	bne	.L1103
	ldr	r3, [r4, #3444]
	cmp	r3, #0
	bne	.L1101
	ldr	lr, [r4, #3428]
	ldr	r1, [r4, #3432]
	mov	r2, lr
.L1102:
	ldr	r0, [r4, #3420]
	cmp	r0, r1
	bcs	.L1100
	rsb	r3, r0, r1
	cmp	r3, ip, lsr #1
	addcs	r2, r2, ip
	strcs	r2, [r4, #3436]
	bcc	.L1100
.L1104:
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1106
.L1154:
	ldr	r3, [r4, #3424]
	add	r2, r2, r0
	str	r2, [r4, #3452]
	add	r3, r2, r3
	str	r3, [r4, #3456]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r4, #3464]
.L1107:
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
.L1096:
	cmp	r2, #0
	add	r4, r0, #16384
	movne	r3, #0
	strne	r3, [r4, #3488]
	bne	.L1111
	ldr	r1, [r4, #3444]
	cmp	r1, #0
	beq	.L1112
	mov	r3, r2
	str	r2, [r4, #3492]
	str	r2, [r4, #3480]
.L1113:
	str	r3, [r4, #3488]
.L1111:
	ldr	r3, [r5, #2924]
	cmp	r3, #0
	beq	.L1115
	ldr	r3, [r4, #3484]
	ldr	r0, [r4, #3488]
	add	r0, r3, r0
	str	r0, [r4, #3476]
	ldr	r7, [r6, #240]
	cmp	r7, #0
	bne	.L1117
	cmp	r0, #0
	beq	.L1118
	sub	r0, r0, #1
	str	r0, [r4, #3476]
.L1117:
	mov	r3, #0
	str	r3, [r4, #3508]
	ldr	r1, [r5, #2924]
	cmp	r1, r3
	bgt	.L1138
	cmp	r0, #0
	beq	.L1121
.L1155:
	sub	r8, r0, #1
	mov	r0, r8
	bl	__aeabi_uidiv
	mov	r9, r0
	mov	r0, r8
	str	r9, [r4, #3500]
	ldr	r1, [r5, #2924]
	bl	__aeabi_uidivmod
	ldr	r0, [r4, #3508]
	mul	r0, r0, r9
	str	r0, [r4, #3504]
	cmp	r1, #0
	str	r1, [r4, #3496]
	blt	.L1122
	add	r2, r5, #2912
	add	r1, r1, #1
	add	r2, r2, #12
	mov	r3, #0
.L1123:
	add	r3, r3, #1
	ldr	ip, [r2, #4]!
	cmp	r3, r1
	add	r0, r0, ip
	str	r0, [r4, #3504]
	bne	.L1123
.L1122:
	cmp	r7, #0
	ldreq	r3, [r5, #2916]
	addeq	r0, r0, r3
	streq	r0, [r4, #3504]
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1125
	ldr	r2, [r4, #3468]
	ldr	r1, [r4, #3472]
	add	r2, r0, r2
	str	r2, [r4, #3452]
	ldr	r3, [r5, #2920]
	add	r3, r2, r3
	add	r0, r3, r1
	str	r0, [r4, #3456]
	cmp	r0, r2
	movge	r0, r2
	str	r0, [r4, #3464]
.L1126:
	ldr	r2, [r4, #3484]
	ldr	r3, [r4, #3488]
	str	r0, [r4, #3460]
	str	r2, [r4, #3480]
	str	r3, [r4, #3492]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1101:
	ldr	lr, [r4, #3448]
	cmp	lr, #0
	beq	.L1153
.L1103:
	ldr	r0, [r4, #3420]
	mov	r3, #0
	mov	lr, r3
	mov	r2, r1
	str	r3, [r4, #3428]
	str	r3, [r4, #3432]
.L1100:
	cmp	r0, r1
	bls	.L1105
	rsb	r1, r1, r0
	cmp	r1, ip, lsr #1
	rsbhi	r2, ip, r2
	strhi	r2, [r4, #3436]
	bhi	.L1104
.L1105:
	str	lr, [r4, #3436]
	ldrb	r3, [r6, #209]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1154
.L1106:
	ldrb	r3, [r6, #210]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r2, r0
	streq	r3, [r4, #3452]
	strne	r3, [r4, #3456]
	str	r3, [r4, #3464]
	b	.L1107
.L1152:
	mov	r3, #0
	ldr	r2, [r4, #3484]
	mov	r1, r3
	str	r3, [r4, #3488]
	str	r3, [r4, #3456]
	str	r3, [r4, #3452]
	str	r3, [r4, #3460]
	str	r3, [r4, #3464]
	b	.L1129
.L1115:
	str	r3, [r4, #3476]
	ldr	r7, [r6, #240]
.L1118:
	mov	r0, #0
	str	r0, [r4, #3508]
	ldr	r3, [r5, #2924]
	cmp	r3, r0
	ble	.L1121
.L1138:
	add	ip, r5, #2912
	mov	r3, #0
	add	ip, ip, #12
	mov	r2, r3
.L1120:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	add	r3, r3, r1
	str	r3, [r4, #3508]
	ldr	r1, [r5, #2924]
	cmp	r1, r2
	bgt	.L1120
	cmp	r0, #0
	bne	.L1155
.L1121:
	mov	r0, #0
	str	r0, [r4, #3504]
	b	.L1122
.L1125:
	ldrb	r3, [r6, #210]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r2, [r5, #2920]
	ldreq	r3, [r4, #3468]
	ldrne	r3, [r4, #3468]
	addne	r0, r0, r2
	addeq	r0, r0, r3
	streq	r0, [r4, #3452]
	addne	r0, r0, r3
	strne	r0, [r4, #3456]
	str	r0, [r4, #3464]
	b	.L1126
.L1112:
	ldr	r1, [r4, #3484]
	ldr	r2, [r4, #3480]
	cmp	r1, r2
	bcs	.L1114
	ldr	r2, [r4, #3492]
	add	r3, r3, r2
	str	r3, [r4, #3488]
	b	.L1111
.L1136:
	ldrb	r0, [r6, #210]	@ zero_extendqisi2
	str	r3, [r4, #3460]
	cmp	r0, #0
	streq	r3, [r4, #3452]
	strne	r3, [r4, #3456]
	b	.L1129
.L1130:
	str	r2, [r4, #3480]
	mov	r1, r2
	str	r2, [r4, #3492]
	ldr	r2, [r4, #3484]
.L1132:
	str	r1, [r4, #3488]
	b	.L1133
.L1153:
	ldr	r3, [r4, #3452]
	mov	r2, r1
	str	lr, [r4, #3428]
	mov	r1, r3
	str	r3, [r4, #3432]
	b	.L1102
.L1114:
	ldr	r3, [r4, #3492]
	b	.L1113
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
	add	r6, r0, #53248
	ldr	r1, [r2, #1304]
	mov	r7, #1
	ldr	r3, [r2, #1300]
	ldrb	r2, [r6, #947]	@ zero_extendqisi2
	mla	r1, r4, ip, r1
	cmp	r2, #0
	ldr	r1, [r1, #28]
	mla	r3, lr, r1, r3
	ldr	r3, [r3, #2904]
	add	r3, r3, #4
	mov	r7, r7, asl r3
	add	r3, r0, #24576
	beq	.L1157
	sub	r5, r2, #1
	sub	r4, r2, #2
	ldr	r2, [r3, #2064]
	clz	r5, r5
	clz	r4, r4
	cmp	r2, #0
	mov	r5, r5, lsr #5
	addne	lr, r0, #26368
	mov	r4, r4, lsr #5
	movne	r1, #0
	addne	lr, lr, #132
	beq	.L1177
.L1176:
	ldr	ip, [lr, #4]!
	ldrb	r2, [ip, #2]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1171
	ldr	r2, [ip, #624]
	ldr	r8, [r6, #1472]
	cmp	r2, r8
	rsbhi	r2, r7, r2
	str	r2, [ip, #628]
	ldr	r2, [lr]
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L1174
	ldr	r8, [r2, #548]
	bic	r8, r8, #-16777216
	bic	r8, r8, #255
	cmp	r8, #65536
	ldreq	ip, [r2, #628]
	addeq	ip, r5, ip, lsl #1
	streq	ip, [r2, #560]
	ldreq	r2, [lr]
	ldreqb	ip, [r2, #2]	@ zero_extendqisi2
.L1174:
	tst	ip, #2
	beq	.L1171
	ldr	ip, [r2, #584]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	ldreq	ip, [r2, #628]
	addeq	ip, r4, ip, lsl #1
	streq	ip, [r2, #596]
.L1171:
	ldr	r2, [r3, #2064]
	add	r1, r1, #1
	cmp	r2, r1
	bhi	.L1176
.L1177:
	ldr	r2, [r3, #2068]
	cmp	r2, #0
	beq	.L1201
	add	r0, r0, #26368
	mov	r2, #0
	add	r0, r0, #196
.L1182:
	ldr	ip, [r0, #4]!
	add	r2, r2, #1
	ldrb	r1, [ip, #2]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1179
	tst	r1, #1
	beq	.L1180
	ldr	lr, [ip, #548]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	ldreq	r1, [ip, #632]
	addeq	r1, r5, r1, lsl #1
	streq	r1, [ip, #556]
	ldreq	ip, [r0]
	ldreqb	r1, [ip, #2]	@ zero_extendqisi2
.L1180:
	tst	r1, #2
	beq	.L1179
	ldr	r1, [ip, #584]
	bic	r1, r1, #-16777216
	bic	r1, r1, #255
	cmp	r1, #256
	ldreq	r1, [ip, #632]
	addeq	r1, r4, r1, lsl #1
	streq	r1, [ip, #592]
.L1179:
	ldr	r1, [r3, #2068]
	cmp	r1, r2
	bhi	.L1182
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1157:
	ldr	r1, [r3, #2064]
	cmp	r1, #0
	addne	ip, r0, #26368
	addne	ip, ip, #132
	bne	.L1166
.L1167:
	ldr	r2, [r3, #2068]
	cmp	r2, #0
	beq	.L1202
	add	r0, r0, #26368
	mov	r2, #0
	add	r0, r0, #196
	b	.L1169
.L1168:
	ldr	r1, [r3, #2068]
	cmp	r1, r2
	bls	.L1203
.L1169:
	ldr	r1, [r0, #4]!
	add	r2, r2, #1
	ldrb	ip, [r1, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1168
	ldr	ip, [r1, #512]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	ldreq	ip, [r1, #632]
	streq	ip, [r1, #520]
	ldr	r1, [r3, #2068]
	cmp	r1, r2
	bhi	.L1169
.L1203:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1204:
	ldr	lr, [r1, #512]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L1163
	ldr	lr, [r1, #624]
	ldr	r4, [r6, #1472]
	rsb	r5, r7, lr
	cmp	lr, r4
	strhi	r5, [r1, #628]
	strls	lr, [r1, #628]
	ldr	r1, [ip]
	ldr	lr, [r1, #628]
	str	lr, [r1, #524]
.L1163:
	ldr	r1, [r3, #2064]
	add	r2, r2, #1
	cmp	r1, r2
	bls	.L1167
.L1166:
	ldr	r1, [ip, #4]!
	ldrb	lr, [r1, #2]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L1163
	b	.L1204
.L1202:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1201:
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
	beq	.L1210
	add	r3, r0, #24576
	ldr	ip, [r3, #2060]
	cmp	ip, #0
	beq	.L1210
	ldr	r3, [r3, #1864]
	cmp	r3, r1
	beq	.L1212
	add	r0, r0, #26368
	mov	r3, #0
	add	r0, r0, #72
	b	.L1207
.L1208:
	ldr	r2, [r0, #4]!
	cmp	r2, r1
	beq	.L1212
.L1207:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1208
.L1210:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1212:
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
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r5, r0, #53248
	ldr	r6, .L1216
	mov	r0, #2
	ldrb	r4, [r5, #947]	@ zero_extendqisi2
	ldr	r3, [r5, #1464]
	ldr	ip, [r6, #68]
	mov	r2, r4
	ldr	r1, .L1216+4
	ldr	r3, [r3, #204]
	blx	ip
	ldr	lr, [r5, #1516]
	cmp	r4, #3
	cmpne	r4, #0
	ldr	r3, [r5, #1512]
	addne	r2, r4, #1
	ldr	ip, [r6, #68]
	moveq	r2, #1
	ldr	r1, .L1216+8
	str	lr, [sp]
	mov	r0, #22
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1217:
	.align	2
.L1216:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r5, r0, #53248
	ldr	r6, .L1254
	mov	r4, r0
	ldr	lr, [r5, #1532]
	mov	r0, #2
	ldr	r1, [r5, #1156]
	mov	r7, #0
	ldrb	r3, [r5, #947]	@ zero_extendqisi2
	add	r9, r4, #12288
	ldr	r2, [r5, #1480]
	add	r8, r4, #16384
	ldr	ip, [r6, #68]
	stmia	sp, {r1, lr}
	ldr	r1, .L1254+4
	blx	ip
	add	r3, r4, #54016
	str	r7, [r5, #3980]
	movw	r2, #56096
	ldrd	r0, [r3, #200]
	strd	r0, [r4, r2]
	ldr	r0, [r5, #3976]
	ldr	r1, [r5, #1152]
	ldr	r2, [r5, #1480]
	ldrb	r3, [r5, #947]	@ zero_extendqisi2
	str	r0, [r5, #3708]
	str	r1, [r5, #3496]
	strb	r3, [r5, #2840]
	str	r2, [r5, #2856]
	ldr	r1, [r5, #1516]
	ldrb	r2, [r9, #1327]	@ zero_extendqisi2
	ldr	r3, [r5, #1520]
	str	r1, [r5, #2860]
	str	r2, [r5, #2872]
	str	r3, [r5, #2864]
	ldr	r3, [r8, #1168]
	add	r3, r3, #1
	str	r3, [r5, #2868]
	ldrb	r3, [r9, #1326]	@ zero_extendqisi2
	strb	r3, [r5, #2843]
	ldr	r3, [r8, #1272]
	str	r3, [r5, #2876]
	ldrb	r3, [r8, #1212]	@ zero_extendqisi2
	str	r3, [r5, #2880]
	ldr	r3, [r9, #2056]
	strb	r3, [r5, #2841]
	ldrb	r3, [r8, #1216]	@ zero_extendqisi2
	str	r3, [r5, #2884]
	ldr	r3, [r8, #3464]
	str	r3, [r5, #2888]
	ldr	r3, [r8, #3452]
	str	r3, [r5, #2892]
	ldr	r2, [r8, #3456]
	ldr	r3, [r5, #1464]
	ldr	r0, [r5, #4056]
	str	r2, [r5, #2896]
	ldr	r1, [r3, #620]
	bl	FSP_GetLogicFs
	subs	r10, r0, #0
	beq	.L1252
	ldr	r2, [r10, #520]
	ldr	r3, [r10, #524]
	cmp	r2, #0
	beq	.L1222
	cmp	r3, #0
	beq	.L1222
	add	r3, r4, #56320
	add	r1, r4, #56576
	add	r1, r1, #76
	ldr	r0, [r5, #4056]
	mov	r2, r3
	add	r3, r3, #204
	add	r2, r2, #76
	add	r6, r4, #24576
	bl	FSP_GetDecFsAddrTab
	ldr	r3, [r10, #520]
	add	r2, r4, #56576
	ldr	r0, [r5, #4056]
	ldr	r3, [r3, #4]
	mov	r1, r2
	add	r2, r2, #80
	add	r1, r1, #160
	str	r3, [r5, #3132]
	bl	FSP_GetPmvAddrTab
	ldr	r3, [r5, #1532]
	str	r3, [r5, #3140]
	ldr	r3, [r10, #536]
	str	r3, [r5, #3492]
	ldr	r3, [r10, #524]
	ldr	r3, [r3, #12]
	str	r3, [r5, #3136]
	ldr	r3, [r10, #524]
	ldr	r3, [r3, #44]
	str	r3, [r5, #3144]
	ldr	r3, [r6, #2332]
	cmp	r3, #0
	str	r3, [fp, #-48]
	str	r3, [r5, #3700]
	beq	.L1228
	add	r10, r4, #26624
	add	lr, r4, #56576
	add	r2, r4, #56832
	mov	r3, r7
	ldr	r7, [fp, #-48]
	mov	ip, r10
	mov	r0, lr
	mov	r1, r10
	add	lr, lr, #176
	add	r10, r10, #88
	add	ip, ip, #152
	add	r0, r0, #240
	add	r1, r1, #216
	add	r2, r2, #48
	str	r4, [fp, #-52]
.L1227:
	ldr	r4, [r10, #4]!
	add	r3, r3, #1
	cmp	r3, r7
	str	r4, [lr, #4]!
	ldr	r4, [ip, #4]!
	str	r4, [r0, #4]!
	ldr	r4, [r1, #4]!
	str	r4, [r2, #4]!
	bne	.L1227
	ldr	r4, [fp, #-52]
.L1228:
	ldrb	r3, [r9, #1333]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1253
	ldrb	r1, [r8, #1230]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L1229
	add	r0, r4, #56064
	ldr	r9, .L1254+8
	add	r0, r0, #80
.L1230:
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
	bne	.L1230
	add	r8, r4, #56064
	mov	r7, #0
	add	r8, r8, #180
.L1231:
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
	bne	.L1231
.L1236:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L1233
	add	r7, r4, #26624
	add	r9, r4, #56832
	add	r4, r4, #57088
	add	r7, r7, #92
	add	r9, r9, #252
	add	r4, r4, #60
	mov	r8, #0
.L1239:
	ldr	r2, [r7]
	mov	r1, #0
	ldr	r0, [r5, #4056]
	add	r8, r8, #1
	bl	FSP_GetStoreType
	adds	r0, r0, #0
	movne	r0, #1
	str	r0, [r9, #4]!
	ldr	r3, [r7], #4
	str	r3, [r4, #4]!
	ldr	r3, [r6, #2332]
	cmp	r3, r8
	bhi	.L1239
.L1233:
	mov	r0, #0
.L1249:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1253:
	ldrb	r1, [r8, #1230]	@ zero_extendqisi2
.L1229:
	cmp	r1, #1
	add	ip, r4, #56064
	mov	r0, #0
	add	ip, ip, #80
	beq	.L1234
.L1237:
	add	r3, r0, #4064
	add	r0, r0, #1
	add	r3, r3, #4
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
	bne	.L1237
	add	r9, r4, #56064
	mov	r10, #1
	add	r9, r9, #180
	mov	r8, #0
.L1238:
	mov	r2, r8, lsr #3
	and	r1, r10, #15
	mov	r3, r8, asl #1
	add	r8, r8, #1
	mov	r2, r2, asl #4
	and	r3, r3, #14
	add	r1, r2, r1
	add	r3, r2, r3
	add	r2, r1, #4080
	add	r3, r3, #4080
	add	r2, r2, #12
	add	r3, r3, #12
	cmp	r8, #16
	add	r10, r10, #2
	add	r2, r4, r2, lsl #2
	add	r3, r4, r3, lsl #2
	add	r9, r9, #8
	ldr	r1, [r2, #4]
	ldr	r3, [r3, #4]
	and	ip, r1, #-16777216
	ubfx	r0, r1, #8, #8
	uxtb	lr, r1
	mov	r1, r1, lsr #16
	mov	r7, r3, lsr #24
	ubfx	r2, r3, #8, #8
	mov	lr, lr, asl #16
	orr	r1, lr, r1, asl #24
	orr	r2, r2, r7, asl #8
	uxtb	lr, r3
	mov	r3, r3, lsr #8
	orr	r1, r1, lr
	and	r3, r3, #65280
	orr	r2, r2, ip
	orr	r3, r1, r3
	orr	r2, r2, r0, asl #16
	str	r3, [r9, #-8]
	str	r2, [r9, #-4]
	bne	.L1238
	b	.L1236
.L1234:
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
	bne	.L1234
	add	r9, r4, #56064
	mov	r10, #1
	add	r9, r9, #180
	mov	r8, #0
.L1235:
	mov	r2, r8, lsr #3
	and	r1, r10, #15
	mov	r3, r8, asl #1
	add	r8, r8, #1
	mov	r2, r2, asl #4
	and	r3, r3, #14
	add	r1, r2, r1
	add	r3, r2, r3
	add	r2, r1, #4864
	add	r3, r3, #4864
	add	r2, r2, #54
	add	r3, r3, #54
	cmp	r8, #16
	add	r10, r10, #2
	add	r2, r4, r2, lsl #2
	add	r3, r4, r3, lsl #2
	add	r9, r9, #8
	ldr	r1, [r2, #4]
	ldr	r3, [r3, #4]
	and	ip, r1, #-16777216
	ubfx	r0, r1, #8, #8
	uxtb	lr, r1
	mov	r1, r1, lsr #16
	mov	r7, r3, lsr #24
	ubfx	r2, r3, #8, #8
	mov	lr, lr, asl #16
	orr	r1, lr, r1, asl #24
	orr	r2, r2, r7, asl #8
	uxtb	lr, r3
	mov	r3, r3, lsr #8
	orr	r1, r1, lr
	and	r3, r3, #65280
	orr	r2, r2, ip
	orr	r3, r1, r3
	orr	r2, r2, r0, asl #16
	str	r3, [r9, #-8]
	str	r2, [r9, #-4]
	bne	.L1235
	b	.L1236
.L1222:
	ldr	ip, [r6, #68]
	mov	r0, #0
	ldr	r1, .L1254+12
	blx	ip
	mvn	r0, #0
	b	.L1249
.L1252:
	ldr	r2, [r5, #1464]
	ldr	r3, [r6, #68]
	ldr	r1, .L1254+16
	ldr	r2, [r2, #620]
	blx	r3
	mvn	r0, #0
	b	.L1249
.L1255:
	.align	2
.L1254:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r2, [r1, #28]
	ldr	r3, [r1, #32]
	cmp	r2, r0
	movlt	r2, r0
	cmp	r3, r0
	str	r2, [r1, #28]
	movge	r3, r0
	str	r3, [r1, #32]
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
	add	r9, r0, #53248
	add	lr, r0, #12288
	mov	r4, r0
	ldr	r3, [r9, #3980]
	movw	r0, #4060
	ldr	ip, [r9, #3976]
	mov	r2, r3
	str	r3, [fp, #-52]
	mul	r0, r0, r2
	add	r3, r3, #1
	str	r3, [r9, #3980]
	ldr	r3, [lr, #1284]
	add	r6, ip, r0
	sub	r3, r3, #1
	cmp	r2, r3
	addcc	r3, r0, #4048
	addcc	r3, r3, #12
	movcs	r3, #0
	addcc	r3, ip, r3
	str	r3, [r6, #4056]
	ldr	r1, [r4, #368]
	ldrb	r5, [r1]	@ zero_extendqisi2
	ldr	r3, [r1, #68]
	cmp	r5, #1
	cmpls	r5, r3
	bcs	.L1260
	add	r7, r5, #3
	add	r7, r6, r7, lsl #2
.L1261:
	mov	r3, r5, asl #5
	sub	r3, r3, r5, asl #2
	add	r5, r5, #1
	add	r1, r1, r3
	ldr	r2, [r1, #24]
	str	r2, [r7, #4]!
	ldr	r2, [r4, #368]
	add	r2, r2, r3
	ldr	r2, [r2, #20]
	str	r2, [r7, #-8]
	ldr	r2, [r4, #368]
	add	r3, r2, r3
	ldr	r3, [r3, #28]
	str	r3, [r7, #8]
	ldr	r1, [r4, #368]
	ldr	r3, [r1, #68]
	cmp	r3, r5
	movhi	r2, #1
	movls	r2, #0
	cmp	r5, #1
	movhi	r2, #0
	cmp	r2, #0
	bne	.L1261
.L1260:
	cmp	r3, #1
	bhi	.L1266
	add	r3, r3, #3
	add	r5, r6, #20
	mov	r1, #0
	add	r3, r6, r3, lsl #2
.L1265:
	str	r1, [r3, #4]!
	cmp	r3, r5
	str	r1, [r3, #-8]
	str	r1, [r3, #8]
	bne	.L1265
.L1266:
	ldrb	r3, [r9, #953]	@ zero_extendqisi2
	add	r2, r4, #16384
	ldr	r1, [fp, #-52]
	add	r5, r4, #20480
	str	r2, [fp, #-48]
	strb	r3, [r6, #1]
	cmp	r1, #0
	ldr	r3, [r2, #1260]
	ldr	r1, [r5, #1820]
	add	r3, r3, #26
	add	r3, r3, r1
	str	r3, [r6, #32]
	bne	.L1362
	ldr	r1, [r9, #1464]
	str	r3, [r1, #540]
	ldr	r3, [r9, #1464]
	ldr	r1, [r6, #32]
	str	r1, [r3, #544]
.L1267:
	ldr	r3, [r5, #1816]
	str	r3, [r6, #36]
	ldr	r3, [r5, #268]
	str	r3, [r6, #40]
	ldr	r3, [r5, #264]
	str	r3, [r6, #44]
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	strb	r3, [ip, r0]
	ldr	r3, [r5, #260]
	str	r3, [r6, #48]
	ldrb	r3, [r5, #214]	@ zero_extendqisi2
	strb	r3, [r6, #2]
	ldrb	r3, [lr, #1328]	@ zero_extendqisi2
	strb	r3, [r6, #3]
	ldr	r3, [r5, #252]
	str	r3, [r6, #52]
	ldr	r3, [r5, #256]
	str	r3, [r6, #56]
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1363
	cmp	r3, #0
	ldreq	r3, [fp, #-48]
	movne	r3, #0
	ldreqb	r3, [r3, #1214]	@ zero_extendqisi2
	strb	r3, [r6, #4]
.L1269:
	ldr	r2, [fp, #-48]
	ldr	r3, [r2, #1264]
	str	r3, [r6, #60]
	ldr	r3, [r2, #1268]
	str	r3, [r6, #64]
	ldr	r3, [r5, #1828]
	str	r3, [r6, #68]
	ldr	r3, [r5, #1832]
	str	r3, [r6, #72]
	ldr	r3, [r5, #1824]
	str	r3, [r6, #76]
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1308
	ldr	r8, [r5, #252]
	cmp	r8, #0
	beq	.L1311
	add	r3, r4, #24576
	ldr	r3, [r3, #2468]
	ldr	r0, [r3, #4]
	ldrb	r3, [r0]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r1, r4, #26880
	movne	r3, #0
	addne	r1, r1, #164
	bne	.L1276
	b	.L1274
.L1278:
	ldr	r2, [r1, #4]!
	ldr	r0, [r2, #4]
	ldrb	r2, [r0]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1274
.L1276:
	add	r3, r3, #1
	cmp	r3, r8
	bne	.L1278
	mov	r3, #0
	str	r3, [fp, #-56]
.L1273:
	ldr	lr, [r5, #256]
	cmp	lr, #0
	beq	.L1312
	add	r3, r4, #24576
	ldr	r3, [r3, #2600]
	ldr	r0, [r3, #4]
	ldrb	r3, [r0]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r1, r4, #27136
	movne	r3, #0
	addne	r1, r1, #40
	bne	.L1282
	b	.L1280
.L1284:
	ldr	r2, [r1, #4]!
	ldr	r0, [r2, #4]
	ldrb	r2, [r0]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1280
.L1282:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L1284
	mov	r3, #0
	str	r3, [fp, #-60]
.L1279:
	ldrb	ip, [r9, #947]	@ zero_extendqisi2
	ldr	r3, [r5, #252]
	cmp	ip, #0
	beq	.L1285
	cmp	r3, #0
	beq	.L1300
	add	r7, r4, #26880
	mov	r8, #0
	str	r4, [fp, #-64]
	mov	r10, r6
	mov	r4, r5
	add	r7, r7, #164
	mov	r5, r8
	ldr	r8, [fp, #-56]
	b	.L1299
.L1297:
	strb	r2, [r10, #1630]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #1631]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r10, #1656]
.L1298:
	ldr	r2, [r4, #252]
	add	r5, r5, #1
	add	r7, r7, #4
	add	r10, r10, #36
	cmp	r2, r5
	bls	.L1364
.L1299:
	ldr	r2, [r7]
	ldr	r0, [r9, #4056]
	ldr	r2, [r2, #4]
	ldr	r1, [r2, #620]
	bl	FSP_GetLogicFs
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #660]
	str	r2, [r10, #1636]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #1625]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r8
	beq	.L1296
	cmp	r0, #0
	ldrne	r2, [r0, #520]
	ldrne	r0, [r2, #4]
.L1296:
	str	r0, [r10, #1640]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #664]
	str	r2, [r10, #1644]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #636]
	str	r2, [r10, #1632]
	ldr	r2, [r7]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r10, #1624]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #512]	@ zero_extendqisi2
	strb	r2, [r10, #1626]
	ldr	r2, [r7]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1297
	strb	r2, [r10, #1628]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #1629]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r10, #1652]
	b	.L1298
.L1364:
	mov	r5, r4
	ldr	r4, [fp, #-64]
.L1300:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1365
.L1288:
	add	r1, r4, #26880
	add	r3, r6, #3920
	add	ip, r6, #4048
	add	r1, r1, #32
	add	r3, r3, #4
	add	ip, ip, #4
.L1305:
	ldr	r2, [r1, #4]!
	str	r2, [r3, #4]!
	cmp	r3, ip
	bne	.L1305
	ldr	r2, [fp, #-48]
	ldrb	r3, [r2, #1214]	@ zero_extendqisi2
	cmp	r3, #0
	str	r3, [r9, #3124]
	ldr	r3, [r2, #1256]
	str	r3, [r9, #3128]
	beq	.L1306
	ldrb	r2, [r5, #208]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1307
.L1306:
	cmp	r3, #1
	beq	.L1366
.L1308:
	ldr	r3, [fp, #-52]
	mov	r2, #0
	ldr	r1, [r4, #368]
	add	r3, r3, #13824
	add	r3, r3, #32
	add	r3, r4, r3, lsl #2
	str	r1, [r3, #4]
	str	r2, [r4, #368]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1362:
	ldr	r1, [r9, #1464]
	ldr	r8, [r1, #540]
	ldr	r7, [r1, #544]
	cmp	r8, r3
	movlt	r8, r3
	cmp	r7, r3
	str	r8, [r1, #540]
	movge	r7, r3
	str	r7, [r1, #544]
	b	.L1267
.L1274:
	ldr	r1, [r0, #620]
	ldr	r0, [r9, #4056]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	ldreq	r3, .L1368
	movweq	r2, #7389
	beq	.L1358
	ldr	r3, [r3, #520]
	ldr	r3, [r3, #4]
	str	r3, [fp, #-56]
	b	.L1273
.L1366:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1308
.L1307:
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
	str	r6, [fp, #-48]
	str	r10, [r1, #84]!
.L1309:
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
	bcs	.L1309
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1308
	add	lr, r4, #21248
	add	r1, r4, #22016
	add	r7, r4, #20992
	add	r0, r4, #21760
	ldr	r8, [fp, #-48]
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
.L1310:
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
	bcs	.L1310
	b	.L1308
.L1285:
	cmp	r3, #0
	beq	.L1293
	add	r10, r4, #26880
	ldr	r8, [fp, #-56]
	add	r10, r10, #164
	str	r4, [fp, #-64]
	mov	r7, r6
	mov	r4, r5
	mov	r5, ip
.L1292:
	ldr	r2, [r10]
	ldr	r0, [r9, #4056]
	ldr	r2, [r2, #4]
	ldr	r1, [r2, #620]
	bl	FSP_GetLogicFs
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #660]
	str	r2, [r7, #1636]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r7, #1625]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r8
	beq	.L1291
	cmp	r0, #0
	ldrne	r2, [r0, #520]
	ldrne	r0, [r2, #4]
.L1291:
	str	r0, [r7, #1640]
	mov	r3, #0
	ldr	r2, [r10]
	add	r5, r5, #1
	add	r7, r7, #36
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #664]
	str	r2, [r7, #1608]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #636]
	str	r2, [r7, #1596]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r7, #1591]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r7, #1590]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #528]
	str	r2, [r7, #1612]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #564]
	str	r2, [r7, #1616]
	ldr	r2, [r10], #4
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #600]
	strb	r3, [r7, #1588]
	str	r2, [r7, #1620]
	ldr	r2, [r4, #252]
	cmp	r2, r5
	bhi	.L1292
	mov	r5, r4
	ldr	r4, [fp, #-64]
.L1293:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1288
	ldr	r3, [r5, #256]
	cmp	r3, #0
	beq	.L1288
	mov	r7, #0
	add	r10, r4, #27136
	str	r4, [fp, #-56]
	mov	r4, r5
	mov	r5, r7
	ldr	r7, [fp, #-60]
	add	r10, r10, #40
	mov	r8, r6
.L1295:
	ldr	r2, [r10]
	ldr	r0, [r9, #4056]
	ldr	r2, [r2, #4]
	ldr	r1, [r2, #620]
	bl	FSP_GetLogicFs
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #660]
	str	r2, [r8, #2788]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #2777]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r7
	beq	.L1294
	cmp	r0, #0
	ldrne	r2, [r0, #520]
	ldrne	r0, [r2, #4]
.L1294:
	str	r0, [r8, #1640]
	mov	r3, #0
	ldr	r2, [r10]
	add	r5, r5, #1
	add	r8, r8, #36
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #664]
	str	r2, [r8, #2760]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #636]
	str	r2, [r8, #2748]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #2743]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r8, #2742]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #528]
	str	r2, [r8, #2764]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #564]
	str	r2, [r8, #2768]
	ldr	r2, [r10], #4
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #600]
	strb	r3, [r8, #2740]
	str	r2, [r8, #2772]
	ldr	r2, [r4, #256]
	cmp	r2, r5
	bhi	.L1295
.L1357:
	mov	r5, r4
	ldr	r4, [fp, #-56]
	b	.L1288
.L1280:
	ldr	r1, [r0, #620]
	ldr	r0, [r9, #4056]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L1367
	ldr	r3, [r3, #520]
	ldr	r3, [r3, #4]
	str	r3, [fp, #-60]
	b	.L1279
.L1363:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #1256]
	strb	r3, [r6, #4]
	b	.L1269
.L1365:
	ldr	r3, [r5, #256]
	cmp	r3, #0
	beq	.L1288
	add	r7, r4, #27136
	mov	r10, #0
	str	r4, [fp, #-56]
	mov	r8, r6
	mov	r4, r5
	add	r7, r7, #40
	mov	r5, r10
	ldr	r10, [fp, #-60]
	b	.L1304
.L1302:
	strb	r2, [r8, #2782]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #2783]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r8, #2808]
.L1303:
	ldr	r2, [r4, #256]
	add	r5, r5, #1
	add	r7, r7, #4
	add	r8, r8, #36
	cmp	r2, r5
	bls	.L1357
.L1304:
	ldr	r2, [r7]
	ldr	r0, [r9, #4056]
	ldr	r2, [r2, #4]
	ldr	r1, [r2, #620]
	bl	FSP_GetLogicFs
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #660]
	str	r2, [r8, #2788]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #2777]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r10
	beq	.L1301
	cmp	r0, #0
	ldrne	r2, [r0, #520]
	ldrne	r0, [r2, #4]
.L1301:
	str	r0, [r8, #2792]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #664]
	str	r2, [r8, #2796]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #636]
	str	r2, [r8, #2784]
	ldr	r2, [r7]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r8, #2776]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #512]	@ zero_extendqisi2
	strb	r2, [r8, #2778]
	ldr	r2, [r7]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1302
	strb	r2, [r8, #2780]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r8, #2781]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r8, #2804]
	b	.L1303
.L1367:
	ldr	r3, .L1368
	movw	r2, #7410
.L1358:
	ldr	r1, .L1368+4
	ldr	r3, [r3, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L1311:
	str	r8, [fp, #-56]
	b	.L1273
.L1312:
	str	lr, [fp, #-60]
	b	.L1279
.L1369:
	.align	2
.L1368:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r0, r0, #53248
	ldr	r2, [r0, #3708]
	cmp	r2, #0
	beq	.L1374
	mov	r0, #0
.L1373:
	ldr	r3, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r1
	add	r3, r3, #7
	cmp	r2, #0
	add	r0, r0, r3, lsr #3
	bne	.L1373
	ldmfd	sp, {fp, sp, pc}
.L1374:
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
	ldr	r1, .L1387
	add	r4, r0, #20480
	add	r7, r0, #53248
	mov	r5, r0
	bl	ue_v
	str	r0, [r4, #260]
	ldr	r3, [r7, #3996]
	cmp	r3, #0
	bne	.L1385
	cmp	r0, #262144
	bcs	.L1386
	add	r2, r5, #12288
	ldr	r1, [r2, #1276]
	ldr	r3, [r2, #1280]
	mul	r3, r3, r1
	sub	r3, r3, #1
	cmp	r0, r3
	bhi	.L1385
	mov	r0, r5
	ldr	r1, .L1387+4
	bl	ue_v
	ldr	r6, .L1387+8
	ldr	r1, .L1387+12
	ldr	r3, [r6, #68]
	mov	r2, r0
	mov	r5, r0
	mov	r0, #19
	blx	r3
	ldr	r3, [r7, #3996]
	cmp	r3, #0
	bne	.L1385
	cmp	r5, #9
	bhi	.L1381
	mov	r1, #1
	movw	r0, #297
	mov	r2, r1, asl r5
	and	r0, r0, r2
	cmp	r0, #0
	bne	.L1382
	ands	r3, r2, #660
	bne	.L1383
	tst	r2, #66
	beq	.L1381
	strb	r1, [r4, #208]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1382:
	strb	r3, [r4, #208]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1381:
	ldr	r3, [r6, #68]
	mov	r2, r5
	ldr	r1, .L1387+16
	mov	r0, #1
	blx	r3
.L1385:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1383:
	mov	r3, #2
	strb	r3, [r4, #208]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1386:
	ldr	r3, .L1387+8
	mov	r0, #1
	ldr	r1, .L1387+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1388:
	.align	2
.L1387:
	.word	.LC45
	.word	.LC47
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L1395
	add	r5, r0, #20480
	mov	r4, r0
	add	r4, r4, #12288
	ldr	r1, .L1395+4
	mov	r0, #19
	ldr	r2, [r5, #220]
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r1, [r4, #1304]
	ldr	r2, [r5, #220]
	mov	r3, #2208
	mla	r3, r3, r2, r1
	ldrb	r1, [r3, #19]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1393
	ldr	r2, [r3, #28]
	mov	r3, #4000
	ldr	r1, [r4, #1300]
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1394
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1393:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1395+8
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1394:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1395+12
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1396:
	.align	2
.L1395:
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L1405
	mov	r2, r1
	mov	r6, r1
	mov	r4, r0
	ldr	r1, .L1405+4
	mov	r0, #19
	ldr	r3, [r5, #68]
	blx	r3
	add	r0, r4, #12288
	mov	r3, #2208
	ldr	r2, [r0, #1304]
	mla	r3, r3, r6, r2
	ldrb	r2, [r3, #19]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1402
	ldr	r1, [r0, #1288]
	ldr	r2, [r3, #28]
	sub	r3, r1, #1
	cmp	r2, r3
	bhi	.L1403
	ldr	r1, [r0, #1300]
	mov	r3, #4000
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1404
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1402:
	ldr	r3, [r5, #68]
	mov	r2, r6
	ldr	r1, .L1405+8
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1403:
	ldr	r3, [r5, #68]
	mov	r0, #1
	ldr	r1, .L1405+12
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1404:
	ldr	r3, [r5, #68]
	mov	r0, #1
	ldr	r1, .L1405+16
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1406:
	.align	2
.L1405:
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r3, r0, #20480
	add	r1, r0, #12288
	ldr	ip, [r3, #1860]
	ldr	r1, [r1, #2052]
	ldr	r2, [r3, #1864]
	ldr	lr, [r3, #220]
	ldr	r5, [r3, #1872]
	cmp	r2, lr
	cmpeq	ip, r1
	ldr	r4, [r3, #228]
	ldrb	lr, [r3, #1856]	@ zero_extendqisi2
	ldrb	ip, [r3, #209]	@ zero_extendqisi2
	movne	r1, #1
	moveq	r1, #0
	cmp	r5, r4
	moveq	r2, r1
	orrne	r2, r1, #1
	cmp	lr, ip
	orrne	r2, r2, #1
	cmp	lr, #0
	cmpne	ip, #0
	beq	.L1409
	ldrb	ip, [r3, #1857]	@ zero_extendqisi2
	ldrb	r1, [r3, #210]	@ zero_extendqisi2
	cmp	ip, r1
	orrne	r2, r2, #1
.L1409:
	ldr	r1, [r3, #1868]
	ldr	ip, [r3, #240]
	cmp	r1, ip
	moveq	r1, #0
	beq	.L1410
	cmp	ip, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
.L1410:
	ldrb	ip, [r3, #1858]	@ zero_extendqisi2
	ldrb	lr, [r3, #211]	@ zero_extendqisi2
	cmp	ip, lr
	orrne	r2, r2, #1
	cmp	lr, #5
	cmpeq	ip, #5
	orr	r2, r2, r1
	beq	.L1418
.L1411:
	add	r0, r0, #16384
	ldr	r1, [r0, #120]
	cmp	r1, #0
	bne	.L1412
	ldr	r1, [r3, #1880]
	ldr	lr, [r0, #3424]
	ldr	ip, [r3, #1884]
	ldr	r0, [r0, #3420]
	cmp	r1, r0
	cmpeq	ip, lr
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
.L1413:
	ldr	r4, [r3, #260]
	mov	r0, #19
	ldr	r3, .L1419
	cmp	r4, #0
	movne	r4, r2
	orreq	r4, r2, #1
	ldr	r1, .L1419+4
	mov	r2, r4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1412:
	cmp	r1, #1
	bne	.L1413
	ldr	r1, [r3, #1888]
	ldr	lr, [r0, #3472]
	ldr	ip, [r3, #1892]
	ldr	r0, [r0, #3468]
	cmp	r1, r0
	cmpeq	ip, lr
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
	b	.L1413
.L1418:
	ldr	ip, [r3, #1876]
	ldr	r1, [r3, #248]
	cmp	ip, r1
	orrne	r2, r2, #1
	b	.L1411
.L1420:
	.align	2
.L1419:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r3, [r3, #2904]
	addne	r3, r3, #5
	addeq	r3, r3, #4
	mov	r7, r7, asl r3
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	cmp	r3, #1
	addhi	r6, r4, #53248
	bls	.L1475
.L1474:
	ldr	r3, [r6, #3996]
.L1440:
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1475:
	ldr	r1, .L1480
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r8, #3512]
	cmp	r0, #0
	beq	.L1473
	ldr	r3, [r5, #264]
	cmn	r3, #2
	beq	.L1454
	add	r10, r4, #19712
	add	r6, r4, #53248
	add	r10, r10, #184
	mov	r9, #0
	b	.L1437
.L1432:
	cmp	r0, #2
	beq	.L1476
.L1435:
	ldr	r3, [r5, #264]
	add	r3, r3, #2
	cmp	r3, r9
	bls	.L1477
.L1437:
	ldr	r1, .L1480+4
	mov	r0, r4
	bl	ue_v
	add	r9, r9, #1
	cmp	r0, #3
	str	r0, [r10, #4]!
	ldr	r3, [r6, #3996]
	bhi	.L1428
	cmp	r3, #0
	bne	.L1428
	cmp	r0, #3
	beq	.L1431
	cmp	r0, #1
	bhi	.L1432
	ldr	r1, .L1480+8
	mov	r0, r4
	bl	ue_v
	cmp	r7, r0
	str	r0, [r10, #264]
	ldr	r3, [r6, #3996]
	bls	.L1434
	cmp	r3, #0
	beq	.L1435
.L1434:
	ldr	ip, .L1480+12
	mov	r2, r0
	ldr	r1, .L1480+16
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1476:
	ldr	r1, .L1480+20
	mov	r0, r4
	bl	ue_v
	str	r0, [r10, #528]
	b	.L1435
.L1473:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	add	r6, r4, #53248
.L1424:
	cmp	r3, #1
	bne	.L1474
	ldr	r1, .L1480+24
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r8, #3513]
	cmp	r0, #0
	beq	.L1474
	ldr	r3, [r5, #268]
	cmn	r3, #2
	beq	.L1474
	add	r9, r4, #19968
	mov	r8, #0
	add	r9, r9, #60
	b	.L1452
.L1451:
	add	r8, r8, #1
	cmp	r8, r3
	bcs	.L1474
.L1452:
	ldr	r1, .L1480+28
	mov	r0, r4
	bl	ue_v
	cmp	r0, #3
	str	r0, [r9, #4]!
	ldr	r3, [r6, #3996]
	bhi	.L1444
	cmp	r3, #0
	bne	.L1444
	cmp	r0, #3
	beq	.L1440
	cmp	r0, #1
	bls	.L1478
	cmp	r0, #2
	beq	.L1479
	ldr	r3, [r5, #268]
	add	r3, r3, #2
	cmp	r3, r8
	bne	.L1451
.L1453:
	ldr	r3, .L1480+12
	mov	r0, #1
	ldr	r1, .L1480+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1478:
	ldr	r1, .L1480+36
	mov	r0, r4
	bl	ue_v
	cmp	r7, r0
	str	r0, [r9, #264]
	ldr	r3, [r6, #3996]
	bls	.L1448
	cmp	r3, #0
	bne	.L1448
.L1449:
	ldr	r2, [r5, #268]
	add	r3, r2, #2
	cmp	r3, r8
	bne	.L1451
	add	r2, r2, #4992
	add	r2, r2, #18
	ldr	r2, [r4, r2, asl #2]
	cmp	r2, #3
	beq	.L1451
	b	.L1453
.L1479:
	ldr	r1, .L1480+40
	mov	r0, r4
	bl	ue_v
	str	r0, [r9, #524]
	b	.L1449
.L1431:
	ldrb	r3, [r5, #208]	@ zero_extendqisi2
	b	.L1424
.L1428:
	ldr	ip, .L1480+12
	mov	r2, r0
	ldr	r1, .L1480+44
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1444:
	ldr	ip, .L1480+12
	mov	r2, r0
	ldr	r1, .L1480+48
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1477:
	cmp	r9, r3
	bne	.L1431
.L1426:
	add	r3, r3, #4928
	add	r3, r3, #46
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #4]
	cmp	r3, #3
	beq	.L1473
	ldr	r3, .L1480+12
	mov	r0, #1
	ldr	r1, .L1480+52
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1448:
	ldr	ip, .L1480+12
	mov	r2, r0
	ldr	r1, .L1480+56
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1454:
	mov	r3, #0
	b	.L1426
.L1481:
	.align	2
.L1480:
	.word	.LC58
	.word	.LC59
	.word	.LC61
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, .L1512
	mla	r2, r10, ip, r2
	str	r2, [fp, #-48]
	ldr	r2, [r2, #28]
	mla	r7, r7, r2, r3
	bl	ue_v
	str	r0, [r6, #272]
	mov	r8, r4, asl r0
	ldr	r9, [r7, #748]
	cmp	r9, #0
	bne	.L1506
.L1483:
	cmp	r0, #7
	bhi	.L1486
	ldr	r3, [r6, #276]
	cmp	r3, #7
	bhi	.L1486
	add	r3, r5, #53248
	str	r3, [fp, #-52]
	ldr	r2, [r3, #3996]
	cmp	r2, #0
	addeq	r10, r5, #20736
	moveq	r4, r2
	addeq	r10, r10, #152
	beq	.L1494
	b	.L1485
.L1490:
	ldr	r2, [r7, #748]
	cmp	r2, #0
	bne	.L1507
.L1491:
	str	r2, [r10]
	str	r2, [r10, #768]
	str	r2, [r10, #128]
	str	r2, [r10, #896]
.L1493:
	ldr	r2, [r6, #264]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r2, r4
	bcc	.L1508
.L1494:
	ldr	r1, .L1512+4
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1490
	ldr	r1, .L1512+8
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+12
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #640]
	ldr	r2, [r7, #748]
	cmp	r2, #0
	beq	.L1491
.L1507:
	ldr	r1, .L1512+16
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1509
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1493
.L1508:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #44]
	cmp	r3, #1
	beq	.L1495
.L1496:
	ldr	r3, [fp, #-52]
	ldr	r0, [r3, #3996]
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
.L1488:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1509:
	ldr	r1, .L1512+20
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+24
	str	r0, [r10]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+20
	str	r0, [r10, #768]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+24
	str	r0, [r10, #128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #896]
	b	.L1493
.L1506:
	ldr	r1, .L1512+28
	mov	r0, r5
	bl	ue_v
	mov	r3, r0
	ldr	r0, [r6, #272]
	mov	r9, r4, asl r3
	str	r3, [r6, #276]
	b	.L1483
.L1495:
	ldrb	r3, [r6, #208]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1496
	add	r10, r5, #21248
	mov	r4, #0
	add	r10, r10, #24
	b	.L1502
.L1498:
	ldr	r3, [r7, #748]
	cmp	r3, #0
	bne	.L1510
.L1499:
	str	r3, [r10]
	str	r3, [r10, #768]
	str	r3, [r10, #128]
	str	r3, [r10, #896]
.L1501:
	ldr	r3, [r6, #268]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r3, r4
	bcc	.L1496
.L1502:
	ldr	r1, .L1512+32
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1498
	ldr	r1, .L1512+36
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+40
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #640]
	ldr	r3, [r7, #748]
	cmp	r3, #0
	beq	.L1499
.L1510:
	ldr	r1, .L1512+44
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1511
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1501
.L1486:
	add	r5, r5, #53248
	ldr	r2, [r5, #3996]
.L1485:
	ldr	r3, .L1512+48
	mov	r0, #1
	ldr	r1, .L1512+52
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1488
.L1511:
	ldr	r1, .L1512+56
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+60
	str	r0, [r10]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+56
	str	r0, [r10, #768]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1512+60
	str	r0, [r10, #128]
	mov	r0, r5
	bl	se_v
	str	r0, [r10, #896]
	b	.L1501
.L1513:
	.align	2
.L1512:
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
	.word	vfmw_Osal_Func_Ptr_S
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
	beq	.L1526
	add	r8, r0, #24576
	mov	r10, #0
	b	.L1517
.L1549:
	ldr	r1, .L1555
	mov	r0, r9
	bl	ue_v
	cmp	r4, #2
	beq	.L1547
.L1528:
	cmp	r4, #6
	orreq	r5, r5, #1
	cmp	r5, #0
	bne	.L1531
	cmp	r4, #4
	beq	.L1548
.L1532:
	cmp	r4, #6
	bhi	.L1533
.L1529:
	add	r3, r9, #53248
	ldr	r3, [r3, #3996]
	cmp	r3, #0
	bne	.L1533
	cmp	r4, #0
	beq	.L1535
.L1534:
	subs	r6, r6, #1
	beq	.L1536
.L1526:
	ldr	r1, .L1555+4
	mov	r0, r9
	bl	ue_v
	bic	r3, r0, #2
	sub	r5, r0, #3
	clz	r5, r5
	cmp	r3, #1
	mov	r4, r0
	mov	r5, r5, lsr #5
	beq	.L1549
	cmp	r4, #2
	bne	.L1528
.L1547:
	ldr	r1, .L1555+8
	mov	r0, r9
	bl	ue_v
	cmp	r5, #0
	beq	.L1529
.L1531:
	ldr	r1, .L1555+12
	mov	r0, r9
	bl	ue_v
	cmp	r4, #4
	bne	.L1532
.L1548:
	ldr	r1, .L1555+16
	mov	r0, r9
	bl	ue_v
	add	r3, r9, #53248
	ldr	r3, [r3, #3996]
	cmp	r3, #0
	beq	.L1534
.L1533:
	ldr	r3, .L1555+20
	mov	r0, #1
	ldr	r1, .L1555+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1554:
	ldr	r1, .L1555
	mov	r0, r9
	bl	ue_v
	ldr	r2, [r4, #8]
	cmp	r2, #2
	str	r0, [r4, #12]
	beq	.L1550
.L1520:
	cmp	r2, #3
	cmpne	r2, #6
	beq	.L1551
.L1521:
	cmp	r2, #4
	beq	.L1552
.L1522:
	cmp	r2, #6
	bhi	.L1533
	add	r3, r9, #53248
	ldr	r3, [r3, #3996]
	cmp	r3, #0
	bne	.L1533
	cmp	r2, #0
	add	r10, r10, #1
	beq	.L1535
	cmp	r10, #100
	beq	.L1553
.L1517:
	ldr	r1, .L1555+4
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
	beq	.L1554
	cmp	r2, #2
	bne	.L1520
.L1550:
	ldr	r1, .L1555+8
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
	bne	.L1521
.L1551:
	ldr	r1, .L1555+12
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
	bne	.L1522
.L1552:
	ldr	r1, .L1555+16
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
	b	.L1522
.L1535:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1553:
	ldr	r3, .L1555+20
	mov	r0, #1
	ldr	r1, .L1555+28
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1536:
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1556:
	.align	2
.L1555:
	.word	.LC89
	.word	.LC88
	.word	.LC90
	.word	.LC91
	.word	.LC92
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r3, r0, #20480
	add	r6, r0, #24576
	mov	r5, r0
	ldrb	r8, [r3, #213]	@ zero_extendqisi2
	ldr	r7, [r6, #1812]
	cmp	r8, #0
	beq	.L1558
	clz	r4, r7
	mov	r4, r4, lsr #5
	str	r4, [r6, #1812]
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1576
.L1559:
	ldr	r1, .L1581
	mov	r0, r5
	bl	u_1
	movw	r2, #2004
	mla	r4, r2, r4, r5
	ldr	r9, .L1581+4
	ldr	r1, .L1581+8
	add	r4, r4, #22272
	ldr	ip, [r9, #68]
	add	r4, r4, #104
	mov	r3, r0
	uxtb	r2, r0
	strb	r3, [r4, #7]
	mov	r0, #16
	blx	ip
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1561
.L1565:
	mov	r8, #0
.L1573:
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1558:
	movw	r4, #2004
	mla	r4, r4, r7, r0
	add	r4, r4, #22272
	add	r4, r4, #104
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1577
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	mov	r4, r7
	cmp	r3, #5
	bne	.L1559
.L1576:
	ldr	r1, .L1581+12
	mov	r0, r5
	bl	u_1
	movw	r3, #2004
	mla	r4, r3, r4, r5
	ldr	r1, .L1581+16
	mov	r8, #0
	add	r4, r4, #22272
	strb	r0, [r4, #109]
	mov	r0, r5
	bl	u_1
	ldr	r3, .L1581+4
	ldrb	r2, [r4, #109]	@ zero_extendqisi2
	ldr	r1, .L1581+20
	ldr	r3, [r3, #68]
	strb	r0, [r4, #110]
	mov	r0, #16
	blx	r3
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1577:
	ldrb	r3, [r3, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1578
	ldr	r1, .L1581
	ldrb	r6, [r4, #7]	@ zero_extendqisi2
	bl	u_1
	cmp	r6, r0
	bne	.L1579
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1565
	mov	r1, r8
	mov	r0, r5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	DecMMCO
.L1561:
	mov	r0, r5
	mov	r1, #1
	bl	DecMMCO
	cmn	r0, #1
	mov	r8, r0
	movne	r3, #1
	movne	r8, #0
	strneb	r3, [r4, #4]
	bne	.L1573
	mov	r2, #0
	ldr	r3, [r9, #68]
	strb	r2, [r4, #4]
	mov	r0, #1
	ldr	r1, .L1581+24
	str	r7, [r6, #1812]
	blx	r3
	b	.L1573
.L1578:
	ldr	r1, .L1581+12
	ldrb	r6, [r4, #5]	@ zero_extendqisi2
	bl	u_1
	cmp	r6, r0
	bne	.L1580
	mov	r0, r5
	ldr	r1, .L1581+16
	ldrb	r4, [r4, #6]	@ zero_extendqisi2
	bl	u_1
	cmp	r4, r0
	beq	.L1565
	ldr	r3, .L1581+4
	mov	r0, #1
	ldr	r1, .L1581+28
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1573
.L1579:
	ldr	r4, .L1581+4
	add	r5, r5, #53248
	ldr	r1, .L1581+32
	mov	r0, #1
	mvn	r8, #0
	ldr	r3, [r4, #68]
	blx	r3
	ldr	lr, [r5, #4000]
	ldr	ip, [r4, #68]
	mov	r0, #1
	ldr	r3, [r5, #4036]
	ldr	r2, [r5, #4020]
	ldr	r1, .L1581+36
	str	lr, [sp]
	blx	ip
	b	.L1573
.L1580:
	ldr	r3, .L1581+4
	mov	r0, #1
	ldr	r1, .L1581+40
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1573
.L1582:
	.align	2
.L1581:
	.word	.LC97
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, .L1657
	add	r5, r0, #12288
	mov	r6, r0
	bl	ue_v
	ldr	r3, [r5, #1292]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r10, r0
	bhi	.L1584
	add	r9, r6, #53248
	ldr	r3, [r9, #3996]
	cmp	r3, #0
	bne	.L1584
	mov	r1, r0
	mov	r0, r6
	bl	PPSSPSCheckTmpId
	subs	r8, r0, #0
	bne	.L1648
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
	beq	.L1588
	ldr	r3, [r5, #3952]
	cmp	r3, #0
	beq	.L1588
.L1589:
	ldr	r1, [r5, #2904]
	mov	r0, r6
	ldr	r2, .L1657+4
	add	r1, r1, #4
	bl	u_v
	str	r0, [r4, #228]
	ldr	r3, [r9, #3996]
	cmp	r3, #0
	bne	.L1646
	ldrb	r3, [r4, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1649
.L1592:
	mov	r3, #0
	strb	r3, [r4, #209]
	strb	r3, [r4, #210]
	ldrb	r3, [r5, #18]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1650
	ldr	r2, [r5, #3956]
	ldr	r1, [r5, #3960]
	add	r3, r2, #1
	mla	r3, r1, r3, r3
.L1616:
	ldrb	r2, [r5, #19]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1599
	strb	r2, [r4, #212]
.L1601:
	ldr	r2, [r4, #260]
	cmp	r2, r3
	bcs	.L1651
.L1602:
	ldrb	r3, [r4, #211]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1652
.L1604:
	add	r10, r6, #16384
	mov	r3, #0
	str	r3, [r10, #3420]
	str	r3, [r10, #3424]
	ldr	r3, [r5, #2908]
	cmp	r3, #0
	beq	.L1653
.L1608:
	mov	r3, #0
	str	r3, [r10, #3468]
	str	r3, [r10, #3472]
	ldr	r3, [r5, #2908]
	cmp	r3, #1
	beq	.L1654
.L1611:
	ldr	r3, [r9, #3996]
	cmp	r3, #0
	bne	.L1646
	ldrb	r3, [r7, #17]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1614
	ldr	r1, .L1657+8
	mov	r0, r6
	bl	ue_v
	cmp	r0, #0
	bne	.L1615
	ldr	r3, [r9, #3996]
	cmp	r3, #0
	bne	.L1615
.L1614:
	mov	r0, r6
	bl	IsNewPic
	uxtb	r0, r0
	strb	r0, [r4, #213]
	cmp	r0, #0
	bne	.L1628
	ldr	r2, [r4, #260]
	ldr	r3, [r4, #1896]
	cmp	r2, r3
	bls	.L1655
.L1628:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1588:
	ldrb	r3, [r4, #208]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1589
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+16
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1628
.L1599:
	mov	r2, #1
	strb	r2, [r4, #212]
.L1619:
	ldr	r2, [r4, #260]
	cmp	r2, r3, lsr #1
	bcc	.L1602
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+20
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1628
.L1615:
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+24
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, .L1657+28
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1646
	mov	r3, #0
	ldr	r0, [r9, #4056]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L1646:
	mvn	r8, #0
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1650:
	ldr	r1, .L1657+32
	mov	r0, r6
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #209]
	cmp	r0, #0
	bne	.L1656
	ldr	r2, [r5, #3956]
	ldrb	ip, [r5, #18]	@ zero_extendqisi2
	ldr	r1, [r5, #3960]
	add	r3, r2, #1
	cmp	ip, #0
	mla	r3, r1, r3, r3
	bne	.L1618
	mov	r3, r3, asl #1
	b	.L1616
.L1653:
	ldr	r1, [r5, #2912]
	mov	r0, r6
	ldr	r2, .L1657+36
	add	r1, r1, #4
	bl	u_v
	str	r0, [r10, #3420]
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1608
	ldrb	r3, [r4, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1608
	ldr	r1, .L1657+40
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3424]
	b	.L1608
.L1649:
	cmp	r0, #0
	str	r0, [r4, #224]
	beq	.L1592
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+44
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1592
.L1654:
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1611
	ldr	r1, .L1657+48
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3468]
	ldrb	r3, [r7, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1611
	ldrb	r3, [r4, #209]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1611
	ldr	r1, .L1657+52
	mov	r0, r6
	bl	se_v
	str	r0, [r10, #3472]
	b	.L1611
.L1656:
	ldr	r1, .L1657+56
	mov	r0, r6
	bl	u_1
	strb	r0, [r4, #210]
	ldr	r2, [r5, #3956]
	ldrb	r0, [r5, #18]	@ zero_extendqisi2
	ldr	r1, [r5, #3960]
	add	r3, r2, #1
	cmp	r0, #0
	ldrb	r0, [r4, #209]	@ zero_extendqisi2
	mla	r3, r1, r3, r3
	moveq	r2, #2
	beq	.L1597
.L1618:
	mov	r2, #1
.L1597:
	cmp	r0, #0
	mul	r3, r2, r3
	beq	.L1616
	cmp	r0, #1
	mov	r2, #0
	strb	r2, [r4, #212]
	bne	.L1601
	b	.L1619
.L1652:
	ldr	r1, .L1657+60
	mov	r0, r6
	bl	ue_v
	cmp	r0, #65536
	str	r0, [r4, #248]
	bcs	.L1605
	ldr	r3, [r9, #3996]
	cmp	r3, #0
	beq	.L1604
.L1605:
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+64
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1604
.L1584:
	ldr	r3, .L1657+12
	mov	r2, r10
	ldr	r1, .L1657+68
	mvn	r8, #0
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1648:
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+72
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1628
.L1651:
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+76
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1628
.L1655:
	ldr	r3, .L1657+12
	mov	r0, #1
	ldr	r1, .L1657+80
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1628
.L1658:
	.align	2
.L1657:
	.word	.LC104
	.word	.LC108
	.word	.LC120
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r8, r0, #12288
	add	r4, r0, #20480
	mov	r5, r0
	ldr	r3, [r8, #1292]
	ldr	r2, [r4, #220]
	sub	r3, r3, #1
	cmp	r3, r2
	bcc	.L1660
	add	r7, r0, #53248
	ldr	r3, [r7, #3996]
	cmp	r3, #0
	bne	.L1660
	bl	PPSSPSCheck
	cmp	r0, #0
	bne	.L1742
	ldr	r3, [r4, #220]
	mov	r6, #2208
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	ldr	r8, [r8, #1304]
	mul	r6, r6, r3
	cmp	r2, #1
	add	r9, r8, r6
	beq	.L1743
.L1664:
	ldr	r1, [r9, #36]
	cmp	r2, #1
	str	r1, [r4, #264]
	ldr	r1, [r9, #40]
	str	r1, [r4, #268]
	bls	.L1744
	cmp	r2, #2
	beq	.L1691
.L1675:
	mov	r2, #32
	str	r2, [r4, #268]
.L1676:
	mov	r0, r5
	bl	ref_pic_list_reordering
	subs	r3, r0, #0
	str	r3, [fp, #-48]
	bne	.L1745
	ldrb	r2, [r9, #2]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1678
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	cmp	r2, #0
	moveq	r2, #1
	moveq	r1, r2
	beq	.L1679
.L1678:
	ldr	r2, [r9, #44]
	cmp	r2, #1
	beq	.L1746
.L1695:
	mov	r2, #0
	mov	r1, r2
.L1679:
	ldr	r10, .L1752
	mov	r0, #19
	strb	r1, [r4, #215]
	ldr	r1, .L1752+4
	ldr	ip, [r10, #68]
	blx	ip
	ldrb	r1, [r4, #215]	@ zero_extendqisi2
	mov	r2, #0
	str	r2, [r4, #272]
	cmp	r1, r2
	str	r2, [r4, #276]
	bne	.L1747
.L1680:
	ldr	r2, [r4, #240]
	cmp	r2, #0
	bne	.L1748
.L1681:
	mov	r2, #0
	str	r2, [r4, #1816]
	ldrb	r2, [r8, r6]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1682
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	cmp	r2, #1
	bls	.L1749
.L1682:
	ldr	r1, .L1752+8
	mov	r0, r5
	bl	se_v
	mov	r2, #0
	str	r2, [r4, #1824]
	str	r2, [r4, #1828]
	str	r2, [r4, #1832]
	str	r0, [r4, #1820]
	ldrb	r3, [r9, #3]	@ zero_extendqisi2
	cmp	r3, r2
	bne	.L1750
.L1711:
	ldr	r0, [fp, #-48]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1691:
	mov	r2, #32
	str	r2, [r4, #264]
	b	.L1675
.L1750:
	ldr	r1, .L1752+12
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #1824]
	bhi	.L1685
	ldr	r3, [r7, #3996]
	cmp	r3, #0
	bne	.L1685
	cmp	r0, #1
	beq	.L1711
	ldr	r1, .L1752+16
	mov	r0, r5
	bl	se_v
	add	r3, r0, #6
	str	r0, [r4, #1828]
	cmp	r3, #12
	bhi	.L1688
	ldr	r3, [r7, #3996]
	cmp	r3, #0
	bne	.L1688
	mov	r0, r5
	ldr	r1, .L1752+20
	bl	se_v
	add	r3, r0, #6
	str	r0, [r4, #1832]
	cmp	r3, #12
	bhi	.L1690
	ldr	r3, [r7, #3996]
	cmp	r3, #0
	beq	.L1711
.L1690:
	mvn	r2, #0
	ldr	r3, [r10, #68]
	ldr	r1, .L1752+24
	mov	r0, #1
	str	r2, [fp, #-48]
	blx	r3
	b	.L1711
.L1747:
	mov	r0, r5
	bl	pred_weight_table
	cmp	r0, #0
	beq	.L1680
	mvn	r2, #0
	ldr	r3, [r10, #68]
	ldr	r1, .L1752+28
	mov	r0, #1
	str	r2, [fp, #-48]
	blx	r3
	b	.L1711
.L1748:
	mov	r0, r5
	bl	dec_ref_pic_marking
	cmp	r0, #0
	beq	.L1681
	mvn	r2, #0
	ldr	r3, [r10, #68]
	ldr	r1, .L1752+32
	mov	r0, #1
	str	r2, [fp, #-48]
	blx	r3
	b	.L1711
.L1746:
	ldrb	r1, [r4, #208]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r2, r1
	bne	.L1695
	b	.L1679
.L1743:
	ldr	r1, .L1752+36
	mov	r0, r5
	bl	u_1
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	strb	r0, [r4, #214]
	b	.L1664
.L1744:
	ldr	r1, .L1752+40
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L1666
	ldr	r2, [r4, #264]
.L1667:
	ldrb	r1, [r4, #209]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L1668
	cmp	r2, #15
	bhi	.L1669
	ldr	r2, [r4, #268]
	cmp	r2, #15
	bhi	.L1751
.L1670:
	ldr	r2, [r7, #3996]
	cmp	r2, #0
	bne	.L1669
.L1671:
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	cmp	r2, #2
	beq	.L1691
	cmp	r2, #1
	bne	.L1675
	b	.L1676
.L1660:
	ldr	r3, .L1752
	mov	r0, #1
	ldr	r1, .L1752+44
	mvn	ip, #0
	str	ip, [fp, #-48]
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r0, [fp, #-48]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1749:
	ldr	r1, .L1752+48
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #1816]
	bhi	.L1683
	ldr	r2, [r7, #3996]
	cmp	r2, #0
	beq	.L1682
.L1683:
	mvn	r2, #0
	ldr	r3, [r10, #68]
	ldr	r1, .L1752+52
	mov	r0, #1
	str	r2, [fp, #-48]
	blx	r3
	b	.L1711
.L1668:
	cmp	r2, #31
	bhi	.L1672
	ldr	r2, [r4, #268]
	cmp	r2, #31
	bls	.L1673
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1673
.L1672:
	ldr	r3, .L1752
	mvn	r2, #0
	ldr	r1, .L1752+56
	mov	r0, #1
	str	r2, [fp, #-48]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1711
.L1673:
	ldr	r2, [r7, #3996]
	cmp	r2, #0
	beq	.L1671
	b	.L1672
.L1751:
	ldrb	r2, [r4, #208]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1670
.L1669:
	ldr	r3, .L1752
	mvn	r2, #0
	ldr	r1, .L1752+60
	mov	r0, #1
	str	r2, [fp, #-48]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1711
.L1666:
	ldr	r1, .L1752+64
	mov	r0, r5
	bl	ue_v
	ldrb	r1, [r4, #208]	@ zero_extendqisi2
	cmp	r1, #1
	mov	r2, r0
	str	r0, [r4, #264]
	bne	.L1667
	ldr	r1, .L1752+68
	mov	r0, r5
	bl	ue_v
	ldr	r2, [r4, #264]
	str	r0, [r4, #268]
	b	.L1667
.L1688:
	mvn	r2, #0
	ldr	r3, [r10, #68]
	ldr	r1, .L1752+72
	mov	r0, #1
	str	r2, [fp, #-48]
	blx	r3
	b	.L1711
.L1685:
	mvn	r2, #0
	ldr	r3, [r10, #68]
	ldr	r1, .L1752+76
	mov	r0, #1
	str	r2, [fp, #-48]
	blx	r3
	b	.L1711
.L1742:
	ldr	r3, .L1752
	mvn	r2, #0
	ldr	r1, .L1752+80
	mov	r0, #1
	str	r2, [fp, #-48]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1711
.L1745:
	ldr	r3, .L1752
	mvn	r2, #0
	ldr	r1, .L1752+84
	mov	r0, #1
	str	r2, [fp, #-48]
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1711
.L1753:
	.align	2
.L1752:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, [r2, #744]
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
	ldr	r1, [r2, #2908]
	cmp	r1, #0
	bne	.L1757
	add	r1, r0, #16384
	ldr	ip, [r1, #3420]
	str	ip, [r3, #1880]
	ldr	r1, [r1, #3424]
	str	r1, [r3, #1884]
	ldr	r1, [r2, #2908]
.L1757:
	cmp	r1, #1
	addeq	r2, r0, #16384
	add	r0, r0, #53248
	ldreq	r1, [r2, #3468]
	streq	r1, [r3, #1888]
	ldreq	r2, [r2, #3472]
	streq	r2, [r3, #1892]
	ldr	r3, [r0, #4012]
	add	r3, r3, #1
	str	r3, [r0, #4012]
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
	beq	.L1764
	bcc	.L1765
	cmp	r3, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r2, #211]	@ zero_extendqisi2
	add	r0, r0, #53248
	cmp	r3, #5
	beq	.L1774
.L1767:
	mov	r3, #0
	str	r3, [r0, #1504]
	ldmfd	sp, {fp, sp, pc}
.L1765:
	add	r0, r0, #53248
	ldr	r3, [r0, #1500]
	cmp	r3, #2
	movne	r3, #1
	strne	r3, [r0, #1500]
	b	.L1767
.L1764:
	add	r0, r0, #53248
	mov	r2, #2
	mov	r3, #0
	str	r2, [r0, #1500]
	str	r3, [r0, #1504]
	ldmfd	sp, {fp, sp, pc}
.L1774:
	mov	r3, #1
	str	r3, [r0, #1504]
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
	beq	.L1776
	ldr	r2, [r0, #368]
	ldr	r3, [r2, #64]
	add	r3, r3, #7
	bic	r3, r3, #7
	str	r3, [r2, #64]
	ldr	r2, [r0, #368]
	ldr	r3, [r2, #72]
	sub	r3, r3, #1
	str	r3, [r2, #72]
.L1776:
	ldr	ip, [r4, #368]
	ldrb	r6, [ip]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L1787
	mov	r1, #0
	mov	r0, ip
	mov	r5, r1
.L1778:
	add	r1, r1, #1
	ldr	r3, [r0, #12]
	cmp	r1, r6
	add	r0, r0, #28
	add	r5, r5, r3
	bne	.L1778
.L1777:
	mov	r8, r6, asl #5
	ldr	r3, [ip, #72]
	sub	r8, r8, r6, asl #2
	ldr	r2, [ip, #64]
	add	ip, ip, r8
	ldr	r7, .L1798
	str	r3, [sp, #4]
	mov	r0, #7
	ldr	lr, [ip, #12]
	mov	r3, r5
	ldr	ip, [r7, #68]
	ldr	r1, .L1798+4
	str	lr, [sp]
	blx	ip
	ldr	r3, [r4, #368]
	mov	r2, r6
	mov	r0, #7
	ldr	r1, .L1798+8
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	ip, [r3, #64]
	mov	lr, r6, asl #5
	and	ip, ip, r0
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	str	ip, [r3, #24]
	ldr	r3, [r4, #368]
	ldrb	ip, [r3]	@ zero_extendqisi2
	ldr	r6, [r3, #64]
	mov	lr, ip, asl #5
	sub	lr, lr, ip, asl #2
	add	r3, r3, lr
	ldr	ip, [r3, #16]
	add	ip, ip, r6, lsr #3
	rsb	ip, r5, ip
	str	ip, [r3, #28]
	ldr	r3, [r4, #368]
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	ip, [r3, #64]
	mov	lr, r6, asl #5
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	ldr	lr, [r3, #12]
	add	r5, r5, lr
	rsb	r5, ip, r5, lsl #3
	str	r5, [r3, #20]
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
	ldr	r5, [r7, #68]
	add	r8, ip, r8
	ldrb	r6, [ip]	@ zero_extendqisi2
	ldr	r3, [r8, #24]
	mov	lr, r6, asl #5
	sub	lr, lr, r6, asl #2
	add	ip, ip, lr
	ldr	ip, [ip, #28]
	str	ip, [sp, #4]
	ldr	ip, [r8, #20]
	str	ip, [sp]
	blx	r5
	ldr	ip, [r4, #368]
	ldr	r2, [ip, #68]
	sub	r3, r2, #1
	cmp	r3, #1
	bhi	.L1797
	ldrb	r5, [ip]	@ zero_extendqisi2
	add	r1, r5, #1
	cmp	r2, r1
	movhi	r2, #1
	movls	r2, #0
	cmp	r1, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L1780
	add	r5, r5, #2
	mov	r6, #0
.L1781:
	sub	r3, r5, #1
	ldr	r1, .L1798+12
	mov	r0, #7
	mov	lr, r3, asl #5
	mov	r2, r3
	sub	r3, lr, r3, asl #2
	add	ip, ip, r3
	str	r6, [ip, #24]
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
	ldr	r8, [ip, #24]
	and	lr, lr, #3
	add	lr, r8, lr, lsl #3
	str	lr, [ip, #24]
	ldr	ip, [r4, #368]
	add	ip, ip, r3
	ldr	lr, [ip, #28]
	bic	lr, lr, #3
	str	lr, [ip, #28]
	ldr	ip, [r4, #368]
	ldr	r8, [r7, #68]
	add	ip, ip, r3
	ldr	r3, [ip, #24]
	ldr	ip, [ip, #20]
	str	ip, [sp]
	blx	r8
	ldr	ip, [r4, #368]
	ldr	r3, [ip, #68]
	cmp	r5, #1
	cmpls	r5, r3
	add	r5, r5, #1
	movcc	r2, #1
	movcs	r2, #0
	cmp	r2, #0
	bne	.L1781
	sub	r3, r3, #1
.L1780:
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
	bcs	.L1775
	sxth	r3, r1
	add	r4, r4, #53248
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, ip, r3
.L1785:
	ldr	r2, [r3, #28]
	add	r1, r1, #1
	ldr	r0, [r4, #1528]
	add	r3, r3, #28
	cmp	r2, r0
	strcc	r2, [r4, #1528]
	ldr	r2, [ip, #68]
	cmp	r2, r1
	bhi	.L1785
.L1775:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1797:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L1798+16
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L1787:
	mov	r5, r6
	b	.L1777
.L1799:
	.align	2
.L1798:
	.word	vfmw_Osal_Func_Ptr_S
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
	mov	r9, r1
	ble	.L1800
	mov	r3, r7
	mov	r1, #8
	mov	r6, r9
	ldr	r8, .L1811
	mov	r5, r1
	mov	r4, #0
	mov	r10, #255
	mov	r7, r2
	mov	r9, r3
	b	.L1805
.L1804:
	mov	lr, ip, lsr #2
	and	r3, ip, #3
	str	r5, [r6, ip, asl #2]
	uxtb	r0, r5
	mov	ip, r3, asl #3
	ldr	r3, [r7, lr, asl #2]
	cmp	r4, r9
	bic	r3, r3, r10, asl ip
	orr	r0, r3, r0, asl ip
	str	r0, [r7, lr, asl #2]
	beq	.L1800
.L1805:
	cmp	r9, #16
	ldreq	r3, .L1811+4
	ldrneb	ip, [r4, r8]	@ zero_extendqisi2
	ldreqb	ip, [r4, r3]	@ zero_extendqisi2
	cmp	r1, #0
	add	r4, r4, #1
	beq	.L1804
	ldr	r1, .L1811+8
	ldr	r0, [fp, #-52]
	str	ip, [fp, #-48]
	bl	se_v
	ldr	r2, [fp, #4]
	ldr	ip, [fp, #-48]
	add	r0, r5, r0
	add	r0, r0, #256
	mov	r1, r0, asr #31
	mov	lr, r1, lsr #24
	add	r0, r0, lr
	uxtb	r0, r0
	rsb	r3, lr, r0
	rsb	r1, lr, r0
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #0
	moveq	r5, r1
	cmp	ip, #0
	movne	r3, #0
	andeq	r3, r3, #1
	str	r3, [r2]
	b	.L1804
.L1800:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1812:
	.align	2
.L1811:
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
	ldr	r1, .L1871
	mov	r5, r0
	bl	u_1
	add	r6, r5, #53248
	strb	r0, [r4]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L1862
	ldr	r1, .L1871+4
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #1]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L1862
	ldr	r1, .L1871+8
	mov	r0, r5
	bl	ue_v
	cmp	r0, #0
	str	r0, [r4, #32]
	bne	.L1817
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	beq	.L1818
.L1817:
	ldr	r3, .L1871+12
	mov	r0, #1
	ldr	r1, .L1871+16
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, .L1871+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1862
	mov	r3, #0
	ldr	r0, [r6, #4056]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L1815:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1825:
	ldr	r1, .L1871+24
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1871+28
	str	r0, [r4, #60]
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #18]
	cmp	r0, #1
	beq	.L1826
.L1844:
	mov	r0, r5
	ldr	r1, .L1871+32
	bl	se_v
	add	r3, r0, #12
	str	r0, [r4, #56]
	cmp	r3, #24
	bhi	.L1827
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	beq	.L1845
.L1827:
	ldr	r3, .L1871+12
	mov	r0, #1
	ldr	r1, .L1871+36
	ldr	r3, [r3, #68]
	blx	r3
.L1862:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1818:
	ldr	r1, .L1871+40
	mov	r0, r5
	bl	ue_v
	cmp	r0, #31
	str	r0, [r4, #36]
	bhi	.L1819
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L1819
	ldr	r1, .L1871+44
	mov	r0, r5
	bl	ue_v
	cmp	r0, #31
	str	r0, [r4, #40]
	bhi	.L1821
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L1821
	ldr	r1, .L1871+48
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #2]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L1862
	ldr	r2, .L1871+52
	mov	r1, #2
	mov	r0, r5
	bl	u_v
	cmp	r0, #2
	str	r0, [r4, #44]
	bhi	.L1863
	ldr	r1, .L1871+56
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1871+60
	str	r0, [r4, #48]
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1871+64
	mov	r0, r5
	bl	se_v
	ldr	r1, .L1871+68
	str	r0, [r4, #52]
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1871+72
	strb	r0, [r4, #3]
	mov	r0, r5
	bl	u_1
	ldr	r1, .L1871+76
	strb	r0, [r4, #4]
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #17]
	cmp	r0, #0
	bne	.L1864
.L1824:
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
	bcs	.L1825
.L1845:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1864:
	ldr	r3, .L1871+12
	mov	r0, #1
	ldr	r1, .L1871+80
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1824
.L1819:
	ldr	r3, .L1871+12
	mov	r0, #1
	ldr	r1, .L1871+84
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1815
.L1821:
	ldr	r3, .L1871+12
	mov	r0, #1
	ldr	r1, .L1871+88
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1815
.L1826:
	add	r3, r4, #4
	str	r6, [fp, #-80]
	sub	r9, fp, #72
	add	r8, r4, #1984
	add	r10, r4, #64
	mov	r6, r3
	b	.L1835
.L1868:
	ldr	lr, [lr, #1300]
	ldr	r3, [r4, #28]
	mla	r3, ip, r3, lr
	ldrb	ip, [r3, #25]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L1865
	ldr	r3, .L1871+92
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L1871+12
	ldr	r3, [r3, #52]
	blx	r3
.L1830:
	add	r7, r7, #1
	add	r8, r8, #16
	cmp	r7, #6
	add	r10, r10, #64
	beq	.L1866
.L1835:
	ldr	r1, .L1871+96
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
	beq	.L1867
	cmp	r7, #0
	cmpne	r7, #3
	beq	.L1868
	ldr	r3, .L1871+12
	mov	r2, #16
	sub	r1, r8, #16
	mov	r0, r8
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1830
.L1866:
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
.L1843:
	ldr	r2, [r6, #60]
	mov	r3, #0
	cmp	r2, #1
	strne	r3, [r8]
	strneb	r3, [r10]
	beq	.L1869
.L1838:
	ldr	r1, [r6, #28]
	mov	lr, #4000
	ldr	r3, [r5, #1300]
	mov	r0, r9
	mov	r2, #64
	mla	r3, lr, r1, r3
	ldrb	r1, [r3, #25]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1870
	ldr	r1, .L1871+100
	cmp	r7, #0
	ldr	r3, .L1871+12
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L1839:
	add	r7, r7, #1
	add	r9, r9, #64
	cmp	r7, #2
	add	r4, r4, #256
	add	r8, r8, #4
	add	r10, r10, #1
	bne	.L1843
	mov	r4, r6
	ldr	r5, [fp, #-80]
	ldr	r6, [fp, #-84]
	b	.L1844
.L1867:
	mov	r3, r2
	str	r9, [sp]
	mov	r2, r8
	mov	r1, r10
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r9]
	cmp	r3, #1
	bne	.L1830
	ldr	r1, .L1871+92
	mov	r2, #16
	ldr	r3, .L1871+12
	cmp	r7, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r8
	blx	r3
	b	.L1830
.L1865:
	add	r1, r3, r7, lsl #4
	ldr	r3, .L1871+12
	add	r1, r1, #2672
	mov	r2, #16
	add	r1, r1, #8
	mov	r0, r8
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1830
.L1863:
	ldr	r3, .L1871+12
	mov	r0, #1
	ldr	r1, .L1871+104
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1815
.L1869:
	ldr	r1, .L1871+108
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
	bne	.L1838
	str	r8, [sp]
	bl	Scaling_List
	ldr	r3, [r8]
	cmp	r3, #1
	bne	.L1839
	ldr	r1, .L1871+100
	mov	r2, #64
	ldr	r3, .L1871+12
	cmp	r7, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r9
	blx	r3
	b	.L1839
.L1870:
	add	r1, r3, r7, lsl #6
	ldr	r3, .L1871+12
	add	r1, r1, #2768
	add	r1, r1, #8
	ldr	r3, [r3, #52]
	blx	r3
	b	.L1839
.L1872:
	.align	2
.L1871:
	.word	.LC147
	.word	.LC148
	.word	.LC149
	.word	vfmw_Osal_Func_Ptr_S
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
	ldreq	ip, .L1898
	movweq	r3, #9571
	beq	.L1896
	cmp	r1, #0
	beq	.L1897
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
	beq	.L1877
	mov	ip, ip, asl #1
	adds	r3, ip, #6
	str	r3, [fp, #-48]
	beq	.L1877
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
.L1882:
	ldrb	r3, [r6, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r3
	cmp	ip, r1
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, #0
	beq	.L1895
	cmp	r0, #5
	bhi	.L1879
	add	r4, r10, #64
	add	r10, r10, #128
	mov	r1, r9
	mov	r3, r4
.L1880:
	ldr	lr, [r3], #4
	ldr	ip, [r1], #4
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, r10
	bne	.L1880
.L1878:
	ldr	r3, [fp, #-48]
	add	r0, r0, #1
	add	r7, r7, #256
	add	r8, r8, #256
	cmp	r0, r3
	mov	r10, r4
	add	r9, r9, #64
	bne	.L1882
	ldr	r6, [fp, #-52]
	ldr	r1, [fp, #-56]
.L1877:
	ldr	r0, [r6, #56]
	ldr	r3, [r1, #56]
	cmp	r0, r3
	movne	r0, #0
	andeq	r0, r2, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L1875:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1879:
	sub	r3, r7, #1088
	sub	r4, r7, #836
	sub	r3, r3, #4
	mov	r1, r8
.L1881:
	ldr	lr, [r3, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	r3, r4
	bne	.L1881
.L1895:
	add	r4, r10, #64
	b	.L1878
.L1897:
	ldr	ip, .L1898
	mov	r0, r1
	movw	r3, #9572
.L1896:
	ldr	r2, .L1898+4
	ldr	r1, .L1898+8
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L1875
.L1899:
	.align	2
.L1898:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, .L1917
	add	r4, r0, #12288
	mov	r5, r0
	bl	ue_v
	ldr	r3, [r4, #1292]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r6, r0
	bls	.L1901
	ldr	r3, .L1917+4
	mov	r0, #1
	ldr	r1, .L1917+8
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r4, #1292]
	sub	r2, r3, #1
	cmp	r6, r2
	bls	.L1909
	ldr	r2, .L1917+12
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L1909
	add	r5, r5, #53248
	str	r3, [fp, #-40]
	str	r6, [fp, #-44]
	mov	r3, #8
	ldr	r0, [r5, #4056]
	sub	r2, fp, #44
	mov	r1, #110
	blx	ip
	mvn	r0, #0
.L1902:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1901:
	ldr	r1, .L1917+16
	mov	r0, r5
	bl	ue_v
	ldr	r3, [r4, #1288]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	ip, r0
	bhi	.L1913
	ldr	r2, [r4, #1300]
	mov	r3, #4000
	mla	r3, r3, r0, r2
	ldrb	r3, [r3, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1914
	mov	r9, #2208
	ldr	r3, [r4, #1304]
	mul	r7, r9, r6
	add	r3, r3, r7
	ldrb	r8, [r3, #19]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L1905
	ldr	r8, .L1917+20
	mov	r0, r5
	mov	r1, r8
	str	r6, [r8, #24]
	str	ip, [r8, #28]
	bl	ProcessPPS
	subs	r5, r0, #0
	bne	.L1915
	ldr	r1, [r4, #1304]
	mov	r0, r8
	add	r1, r1, r7
	bl	PPSEqual
	cmp	r0, #0
	beq	.L1902
	ldr	ip, .L1917+4
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
	b	.L1902
.L1905:
	str	r6, [r3, #24]
	mov	r0, r5
	ldr	r3, [r4, #1304]
	add	r3, r3, r7
	str	ip, [r3, #28]
	ldr	r1, [r4, #1304]
	add	r1, r1, r7
	bl	ProcessPPS
	cmp	r0, #0
	bne	.L1916
	ldr	r3, [r4, #1304]
	mov	r2, #1
	add	r3, r3, r7
	strb	r2, [r3, #20]
	ldr	r3, [r4, #1304]
	add	r7, r3, r7
	strb	r2, [r7, #19]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1909:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1913:
	ldr	r3, .L1917+4
	mov	r0, #1
	ldr	r1, .L1917+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1902
.L1915:
	ldr	r3, .L1917+4
	mov	r0, #1
	ldr	r1, .L1917+28
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1902
.L1914:
	ldr	r3, .L1917+4
	mov	r0, #1
	ldr	r1, .L1917+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1902
.L1916:
	ldr	r3, .L1917+4
	mov	r0, #1
	ldr	r1, .L1917+28
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r4, #1304]
	mov	r2, #1
	mvn	r0, #0
	add	r3, r3, r7
	strb	r2, [r3, #20]
	ldr	r3, [r4, #1304]
	add	r7, r3, r7
	strb	r8, [r7, #19]
	b	.L1902
.L1918:
	.align	2
.L1917:
	.word	.LC171
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC172
	.word	g_event_report
	.word	.LC173
	.word	.LANCHOR2
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
	ldr	r1, .L1970
	mov	r4, r0
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	streqb	r0, [r5, #15]
	bne	.L1964
.L1921:
	ldr	r1, .L1970+4
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #1]
	cmp	r0, #0
	bne	.L1965
.L1922:
	ldr	r1, .L1970+8
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #3]
	cmp	r0, #0
	moveq	r3, #5
	streq	r3, [r5, #28]
	bne	.L1966
.L1924:
	ldr	r1, .L1970+12
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #6]
	cmp	r0, #0
	bne	.L1967
.L1925:
	ldr	r1, .L1970+16
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #8]
	cmp	r0, #0
	bne	.L1968
.L1927:
	ldr	r1, .L1970+20
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #7]
	cmp	r0, #0
	beq	.L1929
	ldr	r1, .L1970+24
	mov	r0, r4
	bl	ue_v
	ldr	r2, .L1970+28
	mov	r1, #4
	str	r0, [r5, #96]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+32
	mov	r1, #4
	strb	r0, [r5, #92]
	mov	r0, r4
	bl	u_v
	ldr	r3, [r5, #96]
	cmp	r3, #31
	strb	r0, [r5, #93]
	bhi	.L1935
	add	r9, r4, #53248
	ldr	r6, [r9, #3996]
	cmp	r6, #0
	addeq	r7, r5, #96
	addeq	r8, r5, #59
	bne	.L1935
.L1933:
	ldr	r1, .L1970+36
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #96]
	cmp	r3, r6
	bcs	.L1933
	ldr	r2, .L1970+48
	mov	r1, #5
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+52
	mov	r1, #5
	str	r0, [r5, #356]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+56
	mov	r1, #5
	str	r0, [r5, #360]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+60
	mov	r1, #5
	str	r0, [r5, #364]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #368]
.L1929:
	ldr	r1, .L1970+64
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #10]
	cmp	r0, #0
	bne	.L1969
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	add	r9, r4, #53248
	cmp	r3, #0
	bne	.L1940
.L1939:
	ldr	r1, .L1970+68
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1970+72
	strb	r0, [r5, #11]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #12]
	cmp	r0, #0
	bne	.L1941
	ldr	r0, [r9, #3996]
.L1942:
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1969:
	ldr	r1, .L1970+24
	mov	r0, r4
	bl	ue_v
	ldr	r2, .L1970+28
	mov	r1, #4
	str	r0, [r5, #408]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+32
	mov	r1, #4
	strb	r0, [r5, #404]
	mov	r0, r4
	bl	u_v
	ldr	r3, [r5, #408]
	cmp	r3, #31
	strb	r0, [r5, #405]
	bhi	.L1935
	add	r9, r4, #53248
	ldr	r6, [r9, #3996]
	cmp	r6, #0
	addeq	r8, r5, #368
	addeq	r7, r5, #408
	addeq	r8, r8, #3
	bne	.L1935
.L1937:
	ldr	r1, .L1970+36
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #408]
	cmp	r3, r6
	bcs	.L1937
	ldr	r2, .L1970+48
	mov	r1, #5
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+52
	mov	r1, #5
	str	r0, [r5, #668]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+56
	mov	r1, #5
	str	r0, [r5, #672]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+60
	mov	r1, #5
	str	r0, [r5, #676]
	mov	r0, r4
	bl	u_v
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r5, #680]
	bne	.L1940
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1939
.L1940:
	ldr	r1, .L1970+76
	mov	r0, r4
	bl	u_1
	strb	r0, [r5, #14]
	b	.L1939
.L1968:
	ldr	r2, .L1970+80
	mov	r1, #32
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+84
	mov	r1, #32
	str	r0, [r5, #52]
	mov	r0, r4
	bl	u_v
	ldr	r1, .L1970+88
	str	r0, [r5, #56]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #9]
	cmp	r0, #1
	bne	.L1927
	ldr	r1, [r5, #52]
	cmp	r1, #0
	beq	.L1927
	ldr	r0, [r5, #56]
	add	r9, r4, #53248
	bl	__aeabi_uidiv
	mov	r0, r0, lsr #1
	str	r0, [r9, #2816]
	b	.L1927
.L1967:
	ldr	r1, .L1970+92
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+96
	str	r0, [r5, #44]
	mov	r0, r4
	bl	ue_v
	str	r0, [r5, #48]
	b	.L1925
.L1966:
	ldr	r2, .L1970+100
	mov	r1, #3
	mov	r0, r4
	bl	u_v
	ldr	r1, .L1970+104
	str	r0, [r5, #28]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1970+108
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r5, #5]
	cmp	r0, #0
	beq	.L1924
	ldr	r2, .L1970+112
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+116
	mov	r1, #8
	str	r0, [r5, #32]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+120
	mov	r1, #8
	str	r0, [r5, #36]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #40]
	b	.L1924
.L1965:
	ldr	r1, .L1970+124
	mov	r0, r4
	bl	u_1
	strb	r0, [r5, #2]
	b	.L1922
.L1964:
	ldr	r2, .L1970+128
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	uxtb	r0, r0
	strb	r0, [r5, #15]
	cmp	r0, #255
	bne	.L1921
	ldr	r2, .L1970+132
	mov	r1, #16
	mov	r0, r4
	bl	u_v
	ldr	r2, .L1970+136
	mov	r1, #16
	str	r0, [r5, #16]
	mov	r0, r4
	bl	u_v
	str	r0, [r5, #20]
	b	.L1921
.L1941:
	ldr	r1, .L1970+140
	mov	r0, r4
	bl	u_1
	ldr	r1, .L1970+144
	strb	r0, [r5, #13]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+148
	str	r0, [r5, #684]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+152
	str	r0, [r5, #688]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+156
	str	r0, [r5, #696]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+160
	str	r0, [r5, #692]
	mov	r0, r4
	bl	ue_v
	ldr	r1, .L1970+164
	str	r0, [r5, #700]
	mov	r0, r4
	bl	ue_v
	str	r0, [r5, #704]
	ldr	r0, [r9, #3996]
	cmp	r0, #0
	beq	.L1942
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1935:
	ldr	r3, .L1970+168
	mov	r0, #1
	ldr	r1, .L1970+172
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1971:
	.align	2
.L1970:
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
	.word	vfmw_Osal_Func_Ptr_S
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
	ldreq	ip, .L2007
	movweq	r3, #9804
	beq	.L2004
	cmp	r1, #0
	beq	.L2005
	ldrb	lr, [r1]	@ zero_extendqisi2
	ldrb	r3, [r5]	@ zero_extendqisi2
	ldr	ip, [r5, #736]
	ldr	r2, [r1, #736]
	ldrb	r0, [r5, #1]	@ zero_extendqisi2
	cmp	r3, lr
	cmpeq	ip, r2
	ldrb	r6, [r1, #1]	@ zero_extendqisi2
	ldrb	r2, [r5, #2]	@ zero_extendqisi2
	ldrb	r4, [r1, #2]	@ zero_extendqisi2
	moveq	lr, #1
	movne	lr, #0
	ldr	r3, [r5, #740]
	cmp	r0, r6
	movne	ip, #0
	andeq	ip, lr, #1
	ldr	r6, [r1, #740]
	cmp	r2, r4
	movne	r0, #0
	andeq	r0, ip, #1
	ldr	lr, [r1, #744]
	ldr	r4, [r5, #744]
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
	beq	.L1976
	add	r0, r1, #3
	add	r2, r5, #3
	sub	r9, r1, #396
	add	r7, r1, #756
	mov	ip, r0
	mov	r8, r5
	mov	r6, r5
	str	r5, [fp, #-48]
	mov	r0, #0
	mov	r4, r2
	mov	r5, ip
	str	r1, [fp, #-52]
.L1981:
	ldrb	r2, [r4, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r2
	cmp	ip, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, #0
	beq	.L1977
	cmp	r0, #5
	bhi	.L1978
	add	r2, r6, #756
	add	r10, r6, #820
	mov	r1, r7
.L1979:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L1979
.L1977:
	add	r0, r0, #1
	add	r8, r8, #256
	cmp	r0, #8
	add	r9, r9, #256
	add	r6, r6, #64
	add	r7, r7, #64
	bne	.L1981
	ldr	r5, [fp, #-48]
	ldr	r1, [fp, #-52]
.L1976:
	ldr	r2, [r1, #2904]
	ldr	ip, [r5, #2908]
	ldr	r0, [r1, #2908]
	ldr	lr, [r5, #2904]
	cmp	ip, r0
	cmpeq	lr, r2
	moveq	r2, #1
	movne	r2, #0
	cmp	ip, #0
	and	r3, r3, r2
	beq	.L1983
	cmp	ip, #1
	bne	.L1982
	ldr	r0, [r1, #2916]
	ldr	ip, [r5, #2916]
	ldrb	r2, [r5, #16]	@ zero_extendqisi2
	ldrb	r6, [r1, #16]	@ zero_extendqisi2
	ldr	r4, [r5, #2920]
	cmp	ip, r0
	cmpeq	r2, r6
	ldr	lr, [r1, #2920]
	ldr	r6, [r5, #2924]
	ldr	ip, [r1, #2924]
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
	beq	.L1982
	add	ip, r5, #2912
	add	r0, r1, #2912
	add	ip, ip, #12
	add	r0, r0, #12
	mov	r2, #0
.L1985:
	ldr	r4, [ip, #4]!
	add	r2, r2, #1
	ldr	lr, [r0, #4]!
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r6
	bne	.L1985
.L1982:
	ldrb	ip, [r1, #17]	@ zero_extendqisi2
	ldrb	lr, [r5, #17]	@ zero_extendqisi2
	ldr	r0, [r5, #3952]
	ldr	r4, [r1, #3952]
	ldr	r2, [r5, #3956]
	cmp	lr, ip
	cmpeq	r0, r4
	ldr	r7, [r1, #3956]
	ldr	r6, [r5, #3960]
	ldr	r4, [r1, #3960]
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
	bne	.L1986
	ldrb	r0, [r5, #19]	@ zero_extendqisi2
	ldrb	r2, [r1, #19]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
.L1986:
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
	bne	.L2006
.L1987:
	ldrb	r0, [r5, #22]	@ zero_extendqisi2
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L1974:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2006:
	ldr	ip, [r1, #3968]
	ldr	lr, [r5, #3968]
	ldr	r0, [r5, #3964]
	ldr	r4, [r1, #3964]
	ldr	r2, [r1, #3972]
	cmp	lr, ip
	cmpeq	r0, r4
	ldr	r6, [r5, #3972]
	ldr	r4, [r5, #3976]
	ldr	lr, [r1, #3976]
	moveq	ip, #1
	movne	ip, #0
	cmp	r6, r2
	movne	r0, #0
	andeq	r0, ip, #1
	cmp	r4, lr
	movne	r2, #0
	andeq	r2, r0, #1
	and	r3, r3, r2
	b	.L1987
.L1978:
	sub	r2, r8, #396
	sub	r10, r8, #140
	mov	r1, r9
.L1980:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L1980
	b	.L1977
.L1983:
	ldr	r0, [r5, #2912]
	ldr	r2, [r1, #2912]
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
	b	.L1982
.L2005:
	ldr	ip, .L2007
	mov	r0, r1
	movw	r3, #9805
.L2004:
	ldr	r2, .L2007+4
	ldr	r1, .L2007+8
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L1974
.L2008:
	.align	2
.L2007:
	.word	vfmw_Osal_Func_Ptr_S
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
	cmp	r0, #0
	mov	ip, r1
	moveq	r4, r0
	beq	.L2010
	subs	lr, r0, #255
	movne	lr, #1
	cmp	r0, #16
	movle	r4, #0
	andgt	r4, lr, #1
	cmp	r4, #0
	movne	r4, #0
	bne	.L2010
	cmp	r2, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
	cmp	r0, #255
	movne	r1, #0
	andeq	r1, r1, #1
	cmp	r1, #0
	bne	.L2010
	cmp	r0, #1
	moveq	r4, #5
	beq	.L2010
	cmp	lr, #0
	str	r3, [fp, #-24]
	ldrne	r2, .L2032
	addne	r0, r2, r0, lsl #3
	ldrne	r2, [r0, #84]
	ldrne	ip, [r0, #80]
	mov	r1, r2
	mov	r0, ip, asl #10
	bl	__aeabi_idiv
	ldr	r3, [fp, #-24]
	ldr	r1, [fp, #4]
	mov	r4, r0
	mov	r0, r3, asl #10
	bl	__aeabi_idiv
	movw	r3, #2405
	movw	r2, #2262
	mul	r0, r0, r4
	add	r4, r0, #1020
	cmp	r0, #0
	add	r4, r4, #3
	movlt	r0, r4
	mov	r0, r0, asr #10
	cmp	r0, r3
	rsble	r3, r0, #2400
	subgt	r3, r0, #2400
	addle	r3, r3, #6
	subgt	r3, r3, #6
	cmp	r0, r2
	bgt	.L2014
	rsb	r2, r0, #2256
	add	r2, r2, #7
	cmp	r3, r2
	bge	.L2015
.L2025:
	mov	r4, #4
.L2010:
	ldr	r3, .L2032+4
	mov	r2, r4
	ldr	r1, .L2032+8
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L2014:
	sub	r2, r0, #2256
	sub	r2, r2, #7
	cmp	r3, r2
	blt	.L2025
.L2015:
	movw	r3, #1819
	cmp	r0, r3
	bgt	.L2016
	rsb	r3, r0, #1808
	add	r3, r3, #12
	cmp	r2, r3
	blt	.L2027
.L2017:
	movw	r2, #1364
	cmp	r0, r2
	rsble	r0, r0, #1360
	subgt	r0, r0, #1360
	addle	r0, r0, #5
	subgt	r0, r0, #5
	cmp	r3, r0
	movlt	r4, #2
	movge	r4, #1
	b	.L2010
.L2016:
	sub	r3, r0, #1808
	sub	r3, r3, #12
	cmp	r2, r3
	bge	.L2017
.L2027:
	mov	r4, #3
	b	.L2010
.L2033:
	.align	2
.L2032:
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC220
	UNWIND(.fnend)
	.size	H264GetDar, .-H264GetDar
	.align	2
	.global	ProcessSPS
	.type	ProcessSPS, %function
ProcessSPS:
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
	UNWIND(.pad #60)
	sub	sp, sp, #60
	subs	r4, r1, #0
	mov	r5, r0
	beq	.L2149
	ldr	r2, [r4, #736]
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
	streq	r3, [r4, #748]
	bne	.L2150
.L2049:
	ldr	r1, .L2166
	mov	r0, r5
	bl	ue_v
	cmp	r0, #12
	str	r0, [r4, #2904]
	bhi	.L2062
	add	r6, r5, #53248
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2062
	ldr	r1, .L2166+4
	mov	r0, r5
	bl	ue_v
	cmp	r0, #2
	str	r0, [r4, #2908]
	bhi	.L2064
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2064
	cmp	r0, #0
	beq	.L2151
	cmp	r0, #1
	beq	.L2152
.L2070:
	ldr	r1, .L2166+8
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #3952]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	ldr	r1, .L2166+12
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #17]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	ldr	r1, .L2166+16
	mov	r0, r5
	bl	ue_v
	movw	r8, #509
	sub	r3, r0, #1
	str	r0, [r4, #3956]
	cmp	r3, r8
	bhi	.L2075
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2075
	ldr	r1, .L2166+20
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #3960]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	ldr	r1, .L2166+24
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #18]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	cmp	r0, #0
	bne	.L2153
	ldr	r1, .L2166+28
	mov	r0, r5
	bl	u_1
	ldr	r7, [r4, #3960]
	add	r7, r7, #1
	cmp	r7, #255
	strb	r0, [r4, #19]
	bhi	.L2107
	ldr	r9, [r4, #3956]
	add	r9, r9, #1
	cmp	r7, #1
	cmphi	r9, #3
	movhi	r3, #2
	strhi	r3, [fp, #-88]
	bhi	.L2079
.L2107:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+36
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2150:
	ldr	r1, .L2166+40
	bl	ue_v
	cmp	r0, #1
	str	r0, [r4, #748]
	bhi	.L2038
	add	r6, r5, #53248
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2038
	ldr	r1, .L2166+44
	mov	r0, r5
	bl	ue_v
	uxth	r0, r0
	add	r2, r0, #8
	str	r2, [r4, #752]
	cmp	r2, #8
	beq	.L2154
.L2040:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+48
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, .L2166+52
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2041
	ldr	r0, [r4, #752]
	mov	r2, #0
	mov	r3, #8
	str	r2, [fp, #-68]
	mov	r1, #119
	sub	r2, fp, #68
	str	r0, [fp, #-64]
	ldr	r0, [r6, #4056]
	blx	ip
.L2041:
	ldr	r1, .L2166+56
	mov	r0, r5
	bl	ue_v
	uxth	r0, r0
	add	r2, r0, #8
	str	r2, [r4, #756]
	cmp	r2, #8
	beq	.L2155
.L2043:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+60
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, .L2166+52
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2044
	ldr	r0, [r4, #756]
	mov	r2, #0
	mov	r3, #8
	str	r2, [fp, #-68]
	mov	r1, #119
	sub	r2, fp, #68
	str	r0, [fp, #-64]
	ldr	r0, [r6, #4056]
	blx	ip
.L2044:
	ldr	r1, .L2166+64
	mov	r0, r5
	bl	u_1
	cmp	r0, #0
	bne	.L2046
	ldr	r6, [r6, #3996]
	cmp	r6, #0
	bne	.L2046
	ldr	r1, .L2166+68
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #25]
	cmp	r0, #1
	bne	.L2049
	add	r10, r4, #2672
	sub	r7, fp, #72
	add	r10, r10, #8
	add	r9, r4, #3
	add	r8, r4, #760
	b	.L2055
.L2158:
	ldr	r3, .L2166+72
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2166+32
	ldr	r3, [r3, #52]
	blx	r3
.L2051:
	add	r6, r6, #1
	add	r10, r10, #16
	cmp	r6, #6
	add	r8, r8, #64
	beq	.L2156
.L2055:
	ldr	r1, .L2166+76
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
	beq	.L2157
	cmp	r6, #0
	cmpne	r6, #3
	beq	.L2158
	ldr	r3, .L2166+32
	mov	r2, #16
	sub	r1, r10, #16
	mov	r0, r10
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2051
.L2155:
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	beq	.L2044
	b	.L2043
.L2154:
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	beq	.L2041
	b	.L2040
.L2106:
	cmp	r1, r9
	cmpcs	r0, r7
	bcs	.L2105
	ldr	r3, .L2166+52
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2147
	strh	r1, [fp, #-72]	@ movhi
	mov	r3, #8
	strh	r0, [fp, #-70]	@ movhi
	sub	r2, fp, #76
	strh	r9, [fp, #-76]	@ movhi
	mov	r1, #107
	strh	r7, [fp, #-74]	@ movhi
	ldr	r0, [r6, #4056]
	blx	ip
.L2147:
	mvn	r0, #0
.L2140:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2153:
	ldr	r7, [r4, #3960]
	strb	r3, [r4, #19]
	sub	r3, r7, #1
	cmp	r3, r8
	bhi	.L2107
	ldr	r9, [r4, #3956]
	mov	r3, #1
	add	r7, r7, #1
	str	r3, [fp, #-88]
	add	r9, r9, r3
.L2079:
	ldr	r3, [fp, #-88]
	mov	r9, r9, asl #4
	mul	r7, r7, r3
	mov	r7, r7, asl #4
	mul	r1, r9, r7
	add	r1, r1, r1, lsr #1
	cmp	r1, #100663296
	bhi	.L2159
	ldr	r2, [r4, #740]
	sub	r3, r2, #10
	cmp	r3, #31
	ldrls	pc, [pc, r3, asl #2]
	b	.L2081
.L2083:
	.word	.L2082
	.word	.L2084
	.word	.L2115
	.word	.L2115
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2115
	.word	.L2086
	.word	.L2088
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2088
	.word	.L2089
	.word	.L2090
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2081
	.word	.L2092
	.word	.L2092
.L2151:
	ldr	r1, .L2166+80
	mov	r0, r5
	bl	ue_v
	cmp	r0, #12
	str	r0, [r4, #2912]
	bhi	.L2067
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	beq	.L2070
.L2067:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+84
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2115:
	mov	r0, #60416
	movt	r0, 13
.L2085:
	bl	__aeabi_uidiv
	ldr	r10, .L2166+32
	ldr	r1, .L2166+88
	ldr	r3, [r10, #68]
	cmp	r0, #16
	movcc	r8, r0
	movcs	r8, #16
	mov	r2, r8
	mov	r0, #21
	blx	r3
	ldr	r2, [r4, #3952]
	cmp	r2, r8
	bhi	.L2093
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	beq	.L2094
.L2093:
	mov	r3, r8
	ldr	ip, [r10, #68]
	ldr	r1, .L2166+92
	mov	r0, #0
	blx	ip
	ldr	r3, [r4, #3952]
	cmp	r3, #16
	bhi	.L2144
	cmp	r8, r3
	movcc	r8, r3
.L2144:
	str	r8, [r4, #3952]
.L2094:
	ldr	r1, .L2166+96
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #20]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	ldr	r1, .L2166+100
	mov	r0, r5
	bl	u_1
	uxtb	r0, r0
	strb	r0, [r4, #21]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	cmp	r0, #0
	bne	.L2160
.L2096:
	mov	r2, r9, lsr #1
	mov	r3, r7, lsr #1
	str	r9, [r4, #3984]
	str	r7, [r4, #3988]
	str	r2, [r4, #3992]
	str	r3, [r4, #3996]
.L2098:
	ldr	r1, .L2166+104
	mov	r0, r5
	bl	u_1
	uxtb	r3, r0
	strb	r3, [r4, #22]
	ldr	r2, [r6, #3996]
	cmp	r2, #0
	bne	.L2147
	cmp	r3, #0
	mov	r2, #2
	str	r2, [r4, #68]
	bne	.L2161
	mov	r0, r3
	strb	r3, [r4, #43]
	mov	r3, #5
	str	r3, [r4, #56]
.L2101:
	ldr	r2, [r4, #48]
	mov	r3, r9
	ldr	r1, [r4, #44]
	str	r7, [sp]
	bl	H264GetDar
	ldr	r3, [r4, #3952]
	cmp	r3, #0
	str	r0, [r4, #52]
	beq	.L2104
	cmp	r8, r3
	movcs	r8, r3
.L2104:
	cmp	r8, #16
	add	r3, r5, #12288
	movcs	r8, #16
	str	r8, [r3, #1296]
	ldr	r3, [r5]
	ldr	r2, [r3, #28]
	cmp	r2, #25
	beq	.L2162
.L2105:
	add	r8, r8, #1
	mov	r0, #0
	str	r8, [r4, #3980]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2092:
	mov	r0, #12582912
	b	.L2085
.L2088:
	mov	r0, #30208
	movt	r0, 47
	b	.L2085
.L2156:
	add	r6, r4, #2768
	add	r7, r4, #1136
	add	r6, r6, #8
	add	r7, r7, #8
	sub	r8, fp, #88
	add	r9, r4, #9
	mov	r10, #0
.L2060:
	ldr	r1, .L2166+76
	mov	r0, r5
	bl	u_1
	mov	r2, #64
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r6
	str	r3, [r8, #4]!
	strb	r3, [r9, #1]!
	beq	.L2163
	ldr	r1, .L2166+108
	cmp	r10, #0
	ldr	r3, .L2166+32
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2057:
	add	r10, r10, #1
	add	r6, r6, #64
	cmp	r10, #2
	add	r7, r7, #256
	bne	.L2060
	b	.L2049
.L2157:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r10
	mov	r1, r8
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2051
	ldr	r1, .L2166+72
	mov	r2, #16
	ldr	r3, .L2166+32
	cmp	r6, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r10
	blx	r3
	b	.L2051
.L2038:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+112
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2062:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+116
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2160:
	ldr	r1, .L2166+120
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2166+124
	str	r0, [r4, #3964]
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2166+128
	str	r0, [r4, #3968]
	mov	r0, r5
	bl	ue_v
	ldr	r1, .L2166+132
	str	r0, [r4, #3972]
	mov	r0, r5
	bl	ue_v
	ldr	r3, [r4, #3972]
	ldr	lr, [fp, #-88]
	ldr	r1, [r4, #3964]
	mov	r3, r3, asl #1
	ldr	r2, [r4, #3968]
	mov	ip, lr, asl #1
	mul	r3, lr, r3
	mov	r1, r1, asl #1
	rsb	lr, r1, r9
	sub	r2, lr, r2, asl #1
	rsb	lr, r3, r7
	str	r0, [r4, #3976]
	mls	r0, r0, ip, lr
	cmp	r2, #0
	cmpgt	r0, #0
	ble	.L2096
	add	r1, r1, r2, lsr #1
	add	r3, r3, r0, lsr #1
	str	r1, [r4, #3992]
	str	r3, [r4, #3996]
	str	r2, [r4, #3984]
	str	r0, [r4, #3988]
	b	.L2098
.L2161:
	add	r1, r4, #28
	mov	r0, r5
	bl	vui_parameters
	cmp	r0, #0
	bne	.L2146
	ldrb	r3, [r4, #40]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2146
	ldr	r2, [r4, #732]
	clz	r3, r2
	mov	r3, r3, lsr #5
	cmp	r8, r2
	orrcc	r3, r3, #1
	cmp	r3, #0
	bne	.L2164
	ldr	r3, [r4, #3952]
	ldrb	r0, [r4, #43]	@ zero_extendqisi2
	cmp	r2, r3
	movcs	r8, r2
	movcc	r8, r3
	b	.L2101
.L2164:
	ldr	ip, [r10, #68]
	mov	r3, r8
	ldr	r1, .L2166+136
	mov	r0, #1
	blx	ip
.L2146:
	ldrb	r0, [r4, #43]	@ zero_extendqisi2
	b	.L2101
.L2162:
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L2105
	ldr	r2, [r3, #760]
	ldr	r1, [r3, #736]
	cmp	r8, r2
	ldr	r0, [r3, #740]
	bls	.L2106
	ldr	r3, .L2166+52
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2147
	str	r2, [fp, #-72]
	mov	r3, #8
	str	r8, [fp, #-76]
	sub	r2, fp, #76
	ldr	r0, [r6, #4056]
	mov	r1, #106
	blx	ip
	mvn	r0, #0
	b	.L2140
.L2046:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+140
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2152:
	ldr	r1, .L2166+144
	mov	r0, r5
	bl	u_1
	strb	r0, [r4, #16]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	ldr	r1, .L2166+148
	mov	r0, r5
	bl	se_v
	str	r0, [r4, #2916]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	ldr	r1, .L2166+152
	mov	r0, r5
	bl	se_v
	str	r0, [r4, #2920]
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	bne	.L2147
	ldr	r1, .L2166+156
	mov	r0, r5
	bl	ue_v
	cmp	r0, #255
	str	r0, [r4, #2924]
	bhi	.L2072
	ldr	r7, [r6, #3996]
	cmp	r7, #0
	bne	.L2072
	cmp	r0, #0
	addne	r8, r4, #2912
	addne	r8, r8, #12
	bne	.L2074
	b	.L2070
.L2165:
	ldr	r3, [r4, #2924]
	cmp	r3, r7
	bls	.L2070
.L2074:
	ldr	r1, .L2166+160
	mov	r0, r5
	bl	se_v
	add	r7, r7, #1
	str	r0, [r8, #4]!
	ldr	r3, [r6, #3996]
	cmp	r3, #0
	beq	.L2165
	b	.L2147
.L2075:
	ldr	r3, .L2166+32
	mov	r2, r0
	ldr	r1, .L2166+164
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2089:
	mov	r0, #30720
	movt	r0, 105
	b	.L2085
.L2086:
	mov	r0, #55296
	movt	r0, 27
	b	.L2085
.L2090:
	mov	r0, #7864320
	b	.L2085
.L2084:
	mov	r0, #17920
	movt	r0, 5
	b	.L2085
.L2082:
	mov	r0, #20992
	movt	r0, 2
	b	.L2085
.L2081:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+168
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2072:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+172
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2163:
	mov	r3, r2
	str	r8, [sp]
	mov	r2, r6
	mov	r1, r7
	mov	r0, r5
	bl	Scaling_List
	ldr	r3, [r8]
	cmp	r3, #1
	bne	.L2057
	ldr	r1, .L2166+108
	mov	r2, #64
	ldr	r3, .L2166+32
	cmp	r10, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2057
.L2064:
	ldr	r3, .L2166+32
	mov	r0, #1
	ldr	r1, .L2166+176
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2159:
	ldr	r3, .L2166+32
	mov	r0, #0
	ldr	r1, .L2166+180
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2140
.L2149:
	ldr	ip, .L2166+32
	mov	r0, r4
	movw	r3, #9995
	ldr	r2, .L2166+184
	ldr	r1, .L2166+188
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L2140
.L2167:
	.align	2
.L2166:
	.word	.LC230
	.word	.LC232
	.word	.LC235
	.word	.LC243
	.word	.LC244
	.word	.LC246
	.word	.LC247
	.word	.LC261
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
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
	ldr	r6, .L2182
	ldr	r3, [r3, #28]
	mla	r5, r5, r3, ip
	bl	BsGet
	ldr	r1, .L2182+4
	mov	r7, r0
	mov	r0, r4
	bl	u_1
	ldr	ip, [r6, #68]
	ldr	r1, .L2182+8
	mov	r2, r0
	mov	r0, #17
	blx	ip
	mov	r0, r4
	ldr	r2, .L2182+12
	mov	r1, #2
	bl	u_v
	cmp	r0, #1
	beq	.L2178
	ldr	r2, [r5, #748]
	sub	r3, r2, #1
	cmp	r3, #1
	bls	.L2179
.L2171:
	cmp	r2, #1
	beq	.L2180
.L2172:
	ldr	r1, .L2182+16
	mov	r0, r4
	bl	u_1
	cmp	r0, #0
	bne	.L2181
.L2173:
	mov	r0, r4
	ldr	r1, .L2182+20
	bl	u_1
	cmp	r0, #1
	moveq	r0, #0
	beq	.L2170
	ldr	lr, [r5, #748]
	mov	r2, r0
	mov	r3, r7
	ldr	ip, [r6, #68]
	ldr	r1, .L2182+24
	mov	r0, #1
	str	lr, [sp]
	blx	ip
	mov	r0, #0
.L2170:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2181:
	ldr	r1, .L2182+28
	mov	r0, r4
	bl	u_1
	ldr	r3, [r6, #68]
	ldr	r1, .L2182+32
	mov	r2, r0
	mov	r0, #17
	blx	r3
	b	.L2173
.L2180:
	ldr	r2, .L2182+36
	mov	r1, #2
	mov	r0, r4
	bl	u_v
	ldr	r3, [r6, #68]
	ldr	r1, .L2182+40
	mov	r2, r0
	mov	r0, #17
	blx	r3
	b	.L2172
.L2179:
	ldr	r1, .L2182+44
	mov	r0, r4
	bl	u_1
	ldr	r3, [r6, #68]
	ldr	r1, .L2182+48
	mov	r2, r0
	mov	r0, #17
	blx	r3
	ldr	r2, [r5, #748]
	b	.L2171
.L2178:
	ldr	r3, [r6, #68]
	ldr	r1, .L2182+52
	blx	r3
	mvn	r0, #0
	b	.L2170
.L2183:
	.align	2
.L2182:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r2, .L2219
	mov	r1, #8
	mov	r4, r0
	add	r7, r0, #53248
	bl	u_v
	ldr	r1, .L2219+4
	mov	r10, r0
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2219+8
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2219+12
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	u_1
	ldr	r1, .L2219+16
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	u_1
	ldr	r2, .L2219+20
	mov	r1, #4
	str	r0, [fp, #-68]
	mov	r0, r4
	bl	u_v
	ldr	r2, .L2219+24
	mov	r1, #8
	mov	r0, r4
	bl	u_v
	ldr	r1, .L2219+28
	mov	r6, r0
	mov	r0, r4
	bl	ue_v
	ldr	r2, [r7, #3996]
	cmp	r2, #0
	mov	r9, r0
	bne	.L2213
	add	r5, r4, #12288
	ldr	r2, [r5, #1288]
	sub	r2, r2, #1
	cmp	r0, r2
	bhi	.L2214
	sub	r2, r10, #66
	mov	r8, r10
	cmp	r2, #78
	ldrls	pc, [pc, r2, asl #2]
	b	.L2189
.L2191:
	.word	.L2190
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2192
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2192
	.word	.L2189
	.word	.L2189
	.word	.L2192
	.word	.L2189
	.word	.L2193
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2192
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2194
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2194
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2189
	.word	.L2194
.L2190:
	ldr	r3, .L2219+32
	mov	r0, #1
	ldr	r1, .L2219+36
	ldr	r3, [r3, #68]
	blx	r3
.L2192:
	cmp	r6, #9
	mov	r2, r6
	bls	.L2212
.L2218:
	cmp	r6, #41
	bhi	.L2212
.L2196:
	mov	r3, #4000
	ldr	r1, [r5, #1300]
	mul	r7, r3, r9
	add	r1, r1, r7
	ldrb	r6, [r1, #23]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L2197
	ldr	r6, .L2219+40
	mov	r0, r4
	str	r3, [fp, #-72]
	ldrb	r3, [fp, #-56]	@ zero_extendqisi2
	add	r1, r6, #2208
	str	r8, [r6, #2944]
	str	r2, [r6, #2948]
	strb	r3, [r6, #2208]
	ldrb	r3, [fp, #-60]	@ zero_extendqisi2
	str	r9, [r6, #2952]
	strb	r3, [r6, #2209]
	ldrb	r3, [fp, #-64]	@ zero_extendqisi2
	strb	r3, [r6, #2210]
	ldrb	r3, [fp, #-68]	@ zero_extendqisi2
	strb	r3, [r6, #2211]
	bl	ProcessSPS
	ldr	r3, [fp, #-72]
	cmp	r0, #0
	bne	.L2215
	ldr	r1, [r5, #1300]
	add	r0, r6, #2208
	str	r3, [fp, #-56]
	add	r1, r1, r7
	bl	SPSEqual
	ldr	r3, [fp, #-56]
	cmp	r0, #0
	bne	.L2216
.L2199:
	cmp	r8, #83
	cmpne	r8, #86
	beq	.L2201
.L2202:
	mov	r0, #0
.L2186:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2212:
	ldr	lr, .L2219+32
	mov	r3, #41
	ldr	r1, .L2219+44
	mov	r0, #1
	ldr	r6, [lr, #68]
	blx	r6
	mov	r2, #41
	b	.L2196
.L2197:
	str	r8, [r1, #736]
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
	str	r2, [r3, #740]
	ldr	r3, [r5, #1300]
	add	r3, r3, r7
	str	r9, [r3, #744]
	ldr	r1, [r5, #1300]
	add	r1, r1, r7
	bl	ProcessSPS
	cmp	r0, #0
	bne	.L2217
	ldr	r3, [r5, #1300]
	mov	r2, #1
	cmp	r8, #83
	cmpne	r8, #86
	add	r3, r3, r7
	strb	r2, [r3, #24]
	ldr	r3, [r5, #1300]
	add	r7, r3, r7
	strb	r2, [r7, #23]
	bne	.L2202
.L2201:
	mov	r0, r4
	bl	Sps_SvcExt
	cmp	r0, #0
	beq	.L2202
	ldr	r3, .L2219+32
	mov	r0, #1
	ldr	r1, .L2219+48
	ldr	r3, [r3, #68]
	blx	r3
.L2213:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2214:
	ldr	r3, .L2219+32
	mov	r0, #1
	ldr	r1, .L2219+52
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r2, [r5, #1288]
	sub	r3, r2, #1
	cmp	r9, r3
	bls	.L2213
	ldr	r3, .L2219+56
	ldr	r4, [r3]
	cmp	r4, #0
	beq	.L2213
	str	r2, [fp, #-48]
	mov	r3, #8
	str	r9, [fp, #-52]
	sub	r2, fp, #52
	ldr	r0, [r7, #4056]
	mov	r1, #109
	blx	r4
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2194:
	ldr	r3, .L2219+32
	mov	r0, #1
	ldr	r1, .L2219+60
	ldr	r3, [r3, #68]
	blx	r3
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2218
	b	.L2212
.L2193:
	ldr	r3, .L2219+32
	mov	r0, #1
	ldr	r1, .L2219+64
	ldr	r3, [r3, #68]
	blx	r3
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2218
	b	.L2212
.L2189:
	ldr	lr, .L2219+32
	mov	r2, r10
	ldr	r1, .L2219+68
	mov	r0, #1
	mov	r8, #100
	ldr	r3, [lr, #68]
	blx	r3
	cmp	r6, #9
	mov	r2, r6
	bhi	.L2218
	b	.L2212
.L2216:
	ldr	ip, .L2219+32
	mov	r2, r3
	ldr	r0, [r5, #1300]
	mov	r3, #1
	add	r1, r6, #2208
	strb	r3, [r6, #2232]
	add	r0, r0, r7
	ldr	ip, [ip, #56]
	strb	r3, [r6, #2231]
	blx	ip
	b	.L2199
.L2215:
	ldr	lr, .L2219+32
	mov	r3, r9
	movw	r2, #10621
	ldr	r1, .L2219+72
	mov	r0, #1
	ldr	ip, [lr, #68]
	blx	ip
	mvn	r0, #0
	b	.L2186
.L2217:
	ldr	lr, .L2219+32
	mov	r3, r9
	movw	r2, #10646
	ldr	r1, .L2219+72
	mov	r0, #1
	ldr	ip, [lr, #68]
	blx	ip
	ldr	r3, [r5, #1300]
	mov	r2, #1
	mvn	r0, #0
	add	r3, r3, r7
	strb	r2, [r3, #24]
	ldr	r3, [r5, #1300]
	add	r7, r3, r7
	strb	r6, [r7, #23]
	b	.L2186
.L2220:
	.align	2
.L2219:
	.word	.LC275
	.word	.LC276
	.word	.LC277
	.word	.LC278
	.word	.LC279
	.word	.LC280
	.word	.LC281
	.word	.LC282
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC283
	.word	.LANCHOR2
	.word	.LC287
	.word	.LC289
	.word	.LC174
	.word	g_event_report
	.word	.LC285
	.word	.LC284
	.word	.LC286
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
	beq	.L2230
	ldrb	r3, [ip]	@ zero_extendqisi2
	mov	r2, r3, asl #2
	mov	r5, r3, asl #5
	rsb	r4, r2, r5
	add	r4, ip, r4
	add	r6, r4, #8
	ldr	lr, [r4, #8]
	cmp	lr, #0
	beq	.L2230
	ldr	lr, [ip, #68]
	cmp	r3, #1
	cmpls	lr, #2
	bhi	.L2230
	cmp	lr, r3
	bls	.L2230
	ldr	lr, [r4, #12]
	mov	r6, r2
	ldr	r4, [r4, #24]
	mov	lr, lr, asl #3
	add	r1, r4, r1, lsl #3
	cmp	r1, lr
	bhi	.L2225
	b	.L2223
.L2226:
	ldr	r4, [r2, #24]
	ldr	r2, [r2, #12]
	rsb	lr, lr, r4
	add	r1, r1, lr
	mov	lr, r2, asl #3
	cmp	r1, lr
	bls	.L2223
.L2225:
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
	bhi	.L2226
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2223:
	rsb	r2, r6, r5
	mov	r0, #1
	add	ip, ip, r2
	str	r1, [ip, #24]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2230:
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
	beq	.L2245
	ldr	r3, [r0, #368]
	cmp	r3, #0
	cmpne	r2, #0
	ble	.L2245
	ldrb	r7, [r3]	@ zero_extendqisi2
	mov	ip, r7, asl #5
	sub	ip, ip, r7, asl #2
	add	ip, r3, ip
	ldr	ip, [ip, #8]
	cmp	ip, #0
	beq	.L2245
	ldr	ip, [r3, #68]
	cmp	r7, #1
	cmpls	ip, #2
	movhi	lr, #1
	movls	lr, #0
	bhi	.L2245
	cmp	ip, r7
	bls	.L2245
.L2240:
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
	bcs	.L2236
	ldrb	r3, [r8, r4, lsr #3]	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	lr, r2
	add	r4, r1, #1
	add	ip, ip, #1
	strb	r3, [r1]
	b	.L2247
.L2238:
	cmp	ip, r6
	mov	r1, r4
	beq	.L2236
	ldrb	r3, [ip], #1	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	lr, r2
	strb	r3, [r4], #1
.L2247:
	ldr	r3, [r0, #368]
	add	r3, r3, r5
	ldr	r1, [r3, #24]
	add	r1, r1, #8
	str	r1, [r3, #24]
	bcc	.L2238
	mov	r0, r2
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2236:
	ldr	ip, [r0, #368]
	add	r7, r7, #1
	ldrb	r3, [ip]	@ zero_extendqisi2
	add	r3, r3, #1
	strb	r3, [ip]
	ldr	r3, [r0, #368]
	ldr	ip, [r3, #68]
	cmp	ip, r7
	bhi	.L2240
	mov	r0, lr
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2245:
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
	ldr	r1, .L2255
	mov	r5, r0
	bl	ue_v
	add	r4, r5, #24576
	ldr	r2, .L2255+4
	mov	r1, #1
	str	r0, [r4, #1840]
	mov	r0, r5
	bl	u_v
	uxtb	r0, r0
	strb	r0, [r4, #1817]
	cmp	r0, #0
	beq	.L2254
.L2249:
	mov	r0, r5
	ldr	r2, .L2255+8
	mov	r1, #1
	bl	u_v
	mov	r3, #1
	strb	r3, [r4, #1816]
	strb	r0, [r4, #1818]
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2254:
	ldr	r2, .L2255+12
	mov	r1, #7
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+16
	mov	r1, #1
	str	r0, [r4, #1832]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+20
	mov	r1, #6
	strb	r0, [r4, #1819]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+24
	mov	r1, #1
	str	r0, [r4, #1836]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+28
	mov	r1, #1
	strb	r0, [r4, #1820]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+32
	mov	r1, #1
	strb	r0, [r4, #1821]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+36
	mov	r1, #1
	strb	r0, [r4, #1822]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+40
	mov	r1, #1
	strb	r0, [r4, #1823]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+44
	mov	r1, #1
	strb	r0, [r4, #1824]
	mov	r0, r5
	bl	u_v
	ldrb	r3, [r4, #1819]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r4, #1825]
	bne	.L2250
	ldr	r3, [r4, #1832]
	cmp	r3, #5
	beq	.L2250
	ldr	r2, .L2255+48
	mov	r1, #1
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+52
	mov	r1, #1
	strb	r0, [r4, #1826]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+56
	mov	r1, #1
	strb	r0, [r4, #1827]
	mov	r0, r5
	bl	u_v
	ldr	r2, .L2255+60
	mov	r1, #1
	strb	r0, [r4, #1828]
	mov	r0, r5
	bl	u_v
	strb	r0, [r4, #1829]
.L2250:
	ldr	r2, .L2255+64
	mov	r1, #8
	mov	r0, r5
	bl	u_v
	ldr	r1, .L2255+68
	str	r0, [r4, #1844]
	mov	r0, r5
	bl	ue_v
	str	r0, [r4, #1848]
	b	.L2249
.L2256:
	.align	2
.L2255:
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
	ldr	r1, [r2, #2052]
	ldr	r3, [r2, #1300]
	mla	r4, r4, r1, r3
	ldrb	r1, [r4, #23]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2258
	ldr	ip, [r2, #1288]
	cmp	ip, #0
	ble	.L2259
	ldrb	r2, [r3, #23]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #4000
	beq	.L2262
	b	.L2281
.L2263:
	ldrb	r1, [r3, #-3977]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2260
.L2262:
	add	r2, r2, #1
	mov	r4, r3
	cmp	r2, ip
	add	r3, r3, #4000
	bne	.L2263
.L2259:
	ldr	r3, .L2283
	mov	r0, #1
	ldr	r1, .L2283+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r3, #0
.L2264:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2281:
	mov	r4, r3
.L2260:
	cmp	r4, #0
	beq	.L2259
.L2258:
	ldrb	r3, [r4, #22]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2265
	ldrb	r3, [r4, #35]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2282
	ldr	r1, [r4, #388]
	ldr	r7, [r4, #392]
	add	r1, r1, #1
	add	r7, r7, #1
.L2268:
	ldr	r2, .L2283+8
	mov	r0, r5
	bl	u_v
	add	r6, r5, #24576
	mov	r1, r7
	ldr	r2, .L2283+12
	str	r0, [r6, #1856]
	mov	r0, r5
	bl	u_v
	str	r0, [r6, #1860]
.L2265:
	ldrb	r0, [r4, #39]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	r3, r0
	beq	.L2264
	ldr	r2, .L2283+16
	mov	r1, #4
	mov	r0, r5
	add	r5, r5, #24576
	bl	u_v
	mov	r3, #0
	strb	r0, [r5, #1852]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2282:
	ldrb	r3, [r4, #38]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2265
	ldr	r1, [r4, #700]
	ldr	r7, [r4, #704]
	add	r1, r1, #1
	add	r7, r7, #1
	b	.L2268
.L2284:
	.align	2
.L2283:
	.word	vfmw_Osal_Func_Ptr_S
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
	b	.L2288
.L2518:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2287
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2517
.L2288:
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
	ble	.L2518
.L2287:
	ldr	r3, .L2551+104
	mov	r0, #1
	ldr	r1, .L2551
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2290
.L2517:
	mov	r3, #0
	str	r3, [fp, #-64]
.L2468:
	mov	r5, #0
	b	.L2294
.L2520:
	bl	BsSkip
	cmp	r10, #0
	ble	.L2292
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2519
.L2294:
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
	ble	.L2520
.L2292:
	ldr	r3, .L2551+104
	mov	r0, #1
	ldr	r1, .L2551+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
.L2290:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2519:
	cmp	r5, #409600
	bgt	.L2292
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
	b	.L2297
.L2299:
	.word	.L2298
	.word	.L2300
	.word	.L2301
	.word	.L2302
	.word	.L2303
	.word	.L2304
	.word	.L2305
	.word	.L2306
	.word	.L2307
	.word	.L2308
	.word	.L2309
	.word	.L2310
	.word	.L2311
	.word	.L2312
	.word	.L2313
	.word	.L2314
	.word	.L2315
	.word	.L2316
	.word	.L2317
	.word	.L2318
	.word	.L2319
	.word	.L2320
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2297
	.word	.L2321
.L2321:
	mov	r1, r5
	mov	r0, r7
	ldr	r9, [r4, #1264]
	bl	H264_DecFramePackingSEI
	ldr	r0, [r4, #1264]
	mov	r10, r5, asl #3
	rsb	r3, r9, r0
	cmp	r3, r10
	bge	.L2451
	rsb	r3, r3, r10
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r9, r3, #7
	movge	r9, r3
	ldr	r3, [r4, #1256]
	mov	r9, r9, asr #3
	cmp	r9, #0
	movle	r3, r3, asl #3
	ble	.L2453
	add	r2, r0, #8
	mov	r3, r3, asl #3
	cmp	r2, r3
	bgt	.L2453
	mov	r10, #0
	b	.L2454
.L2455:
	add	r2, r0, #8
	cmp	r2, r3
	bgt	.L2453
.L2454:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #1256]
	cmp	r10, r9
	ldr	r0, [r4, #1264]
	mov	r3, r3, asl #3
	bne	.L2455
.L2453:
	ldr	r2, [fp, #-56]
	mov	r1, r2, asr #31
	mov	r1, r1, lsr #29
	add	r2, r2, r1
	and	r2, r2, #7
	rsb	r1, r1, r2
	add	r2, r1, r0
	cmp	r2, r3
	ble	.L2521
.L2451:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2522
.L2512:
	add	r3, r7, #53248
	ldr	r3, [r3, #2820]
.L2324:
	cmp	r3, #3
	bhi	.L2523
.L2463:
	mov	r9, #0
	b	.L2467
.L2465:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2466
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2524
.L2467:
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
	ble	.L2465
.L2466:
	mov	r0, #0
.L2549:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2320:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+8
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2525
.L2445:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2448
.L2449:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2448:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2449
	b	.L2512
.L2319:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+12
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2526
.L2440:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2443
.L2444:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2443:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2444
	b	.L2512
.L2317:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+16
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2527
.L2430:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2433
.L2434:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2433:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2434
	b	.L2512
.L2316:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+20
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2528
.L2425:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2428
.L2429:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2428:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2429
	b	.L2512
.L2315:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+24
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2529
.L2420:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2423
.L2424:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2423:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2424
	b	.L2512
.L2314:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+28
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2530
.L2415:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2418
.L2419:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2418:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2419
	b	.L2512
.L2318:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+32
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2531
.L2435:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2438
.L2439:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2438:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2439
	b	.L2512
.L2313:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+36
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2532
.L2410:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2413
.L2414:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2413:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2414
	b	.L2512
.L2312:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+40
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2533
.L2405:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2408
.L2409:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2408:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2409
	b	.L2512
.L2311:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+44
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2534
.L2400:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2403
.L2404:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2403:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2404
	b	.L2512
.L2310:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+48
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2535
.L2395:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2398
.L2399:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2398:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2399
	b	.L2512
.L2309:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+52
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2536
.L2390:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2393
.L2394:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2393:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2394
	b	.L2512
.L2308:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+56
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2537
.L2385:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2388
.L2389:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2388:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2389
	b	.L2512
.L2307:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+60
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2538
.L2380:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2383
.L2384:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2383:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2384
	b	.L2512
.L2306:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+64
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2539
.L2375:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2378
.L2379:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2378:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2379
	b	.L2512
.L2305:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+68
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	cmp	r5, #0
	ble	.L2372
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2372
	mov	r9, #0
	b	.L2373
.L2374:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2372
.L2373:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2374
.L2372:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	bge	.L2512
	ldr	r3, .L2551+104
	ldr	ip, [r3, #68]
	movw	r3, #11301
	b	.L2513
.L2304:
	ldr	r3, [fp, #-64]
	cmp	r3, #0
	bne	.L2303
	ldr	r3, .L2551+104
	add	r10, r7, #53248
	ldr	lr, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+72
	ldr	ip, [r3, #68]
	mov	r3, r5
	str	lr, [fp, #-68]
	blx	ip
	ldr	r9, [r10, #2820]
	cmp	r9, #3
	bhi	.L2540
.L2348:
	ldr	r0, [r10, #4056]
	bl	GetUsd
	add	r3, r9, #14016
	add	r3, r3, #2
	str	r0, [r7, r3, asl #2]
	ldr	r3, [r10, #2820]
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	lr, [r7, r3, asl #2]
	cmp	lr, #0
	beq	.L2471
	ldr	r2, [r7, #368]
	cmp	r5, #1024
	movlt	r3, r5
	movge	r3, #1024
	ldrb	r0, [r2]	@ zero_extendqisi2
	ldr	ip, [r2, #68]
	str	r3, [fp, #-56]
	cmp	r0, ip
	bcs	.L2472
	sxth	r3, r0
	mov	r9, #0
	mov	r1, r3, asl #5
	sub	r1, r1, r3, asl #2
	add	r1, r2, r1
.L2350:
	ldr	r3, [r1, #24]
	add	r0, r0, #1
	ldr	r2, [r1, #12]
	cmp	r0, ip
	add	r3, r3, #7
	add	r1, r1, #28
	sub	r3, r2, r3, lsr #3
	add	r9, r9, r3
	bne	.L2350
	mov	r2, r9
.L2349:
	ldr	r1, [fp, #-56]
	ldrsb	r3, [fp, #-64]
	cmp	r1, r2
	movle	r9, r1
	cmp	r3, #1
	movne	r3, #0
	strne	r3, [fp, #-60]
	beq	.L2541
.L2352:
	rsb	r2, r3, r9
	cmp	r2, #0
	ble	.L2542
	ldr	r3, [r10, #2820]
	mov	r0, r7
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	r1, [r7, r3, asl #2]
	bl	GetBytes
	subs	r9, r0, #0
	ble	.L2543
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-60]
	rsb	r3, r2, r3
	cmp	r9, r3
	ldr	r3, [r10, #2820]
	bge	.L2358
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	r3, [r7, r3, asl #2]
	str	r9, [r3, #1048]
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2359
	mov	r5, #0
	b	.L2360
.L2363:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2359
.L2360:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2363
.L2359:
	ldr	r3, [r10, #2820]
	mov	r2, #8
	ldr	r1, [r7]
	add	r3, r3, #14016
	ldr	ip, .L2551+76
	add	r3, r3, #2
	ldrd	r0, [r1, #48]
	ldr	r3, [r7, r3, asl #2]
	ldr	ip, [ip]
	add	r3, r3, #1056
	cmp	ip, #0
	strd	r0, [r3]
	ldr	r3, [r10, #2820]
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	r3, [r7, r3, asl #2]
	str	r2, [r3, #1036]
	ldr	r3, [r10, #2820]
	ldr	r2, [r10, #4016]
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	r3, [r7, r3, asl #2]
	str	r2, [r3, #1040]
	beq	.L2362
	ldr	r2, [r10, #2820]
	mov	r3, #1072
	mov	r1, #6
	ldr	r0, [r10, #4056]
	add	r2, r2, #14016
	add	r2, r2, #2
	ldr	r2, [r7, r2, asl #2]
	blx	ip
.L2362:
	ldr	r3, [r10, #2820]
	add	r3, r3, #1
	str	r3, [r10, #2820]
	cmp	r3, #3
	bls	.L2463
.L2523:
	ldr	r3, .L2551+104
	mov	r0, #1
	ldr	r1, .L2551+80
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2290
.L2301:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+84
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2544
.L2336:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2339
.L2340:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2339:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2340
	b	.L2512
.L2300:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+88
	mov	r10, r5, asl #3
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	ldr	r9, [r4, #1264]
	bl	H264_DecPicTimingSEI
	ldr	r2, [r4, #1264]
	rsb	r3, r9, r2
	cmp	r3, r10
	bge	.L2329
	rsb	r3, r3, r10
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r9, r3, #7
	movge	r9, r3
	ldr	r3, [r4, #1256]
	mov	r9, r9, asr #3
	cmp	r9, #0
	movle	r3, r3, asl #3
	ble	.L2331
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bgt	.L2331
	mov	r10, #0
	b	.L2332
.L2333:
	add	r1, r2, #8
	cmp	r1, r3
	bgt	.L2331
.L2332:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #1256]
	cmp	r10, r9
	ldr	r2, [r4, #1264]
	mov	r3, r3, asl #3
	bne	.L2333
.L2331:
	ldr	ip, [fp, #-56]
	mov	r1, ip, asr #31
	mov	r0, r1, lsr #29
	add	r1, ip, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r1, r2
	cmp	r2, r3
	ble	.L2545
.L2329:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	bge	.L2512
	ldr	r3, .L2551+104
	ldr	ip, [r3, #68]
	movw	r3, #11049
	b	.L2513
.L2302:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+92
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2546
.L2341:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2344
.L2345:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2344:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2345
	b	.L2512
.L2298:
	ldr	r3, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+96
	ldr	ip, [r3, #68]
	mov	r3, r5
	blx	ip
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2547
.L2322:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2326
.L2327:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2326:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2327
	b	.L2512
.L2552:
	.align	2
.L2551:
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
	.word	g_event_report
	.word	.LC342
	.word	.LC317
	.word	.LC316
	.word	.LC318
	.word	.LC314
	.word	.LC319
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC321
	.word	.LC323
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+216
	.word	.LC315
	.word	.LC324
	.word	.LC322
	.word	.LC325
.L2303:
	ldr	r3, .L2551+104
	add	r10, r7, #53248
	ldr	lr, .L2551+104
	mov	r0, #20
	ldr	r1, .L2551+100
	ldr	ip, [r3, #68]
	mov	r3, r5
	str	lr, [fp, #-68]
	blx	ip
	ldr	r9, [r10, #2820]
	mov	r3, #1
	str	r3, [fp, #-64]
	cmp	r9, #3
	bls	.L2348
.L2540:
	ldr	r1, [r10, #2824]
	ldr	r0, [r10, #4056]
	bl	FreeUsdByDec
	ldr	r1, [r10, #2828]
	ldr	r0, [r10, #4056]
	bl	FreeUsdByDec
	ldr	r1, [r10, #2832]
	ldr	r0, [r10, #4056]
	bl	FreeUsdByDec
	ldr	r1, [r10, #2836]
	ldr	r0, [r10, #4056]
	bl	FreeUsdByDec
	mov	r3, #0
	mov	r9, r3
	str	r3, [r10, #2824]
	str	r3, [r10, #2828]
	str	r3, [r10, #2832]
	str	r3, [r10, #2836]
	str	r3, [r10, #2820]
	b	.L2348
.L2297:
	mov	r1, r5
	mov	r0, r7
	bl	PassBytes
	cmp	r0, #0
	blt	.L2548
.L2458:
	cmp	r5, #0
	ble	.L2512
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
	mov	r9, #0
	b	.L2461
.L2462:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2512
.L2461:
	add	r9, r9, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2462
	b	.L2512
.L2524:
	cmp	r3, #128
	bne	.L2468
	mov	r0, #0
	b	.L2549
.L2358:
	add	r3, r3, #14016
	ldr	r2, [fp, #-56]
	add	r3, r3, #2
	cmp	r2, r5
	ldr	r2, [fp, #-60]
	ldr	r3, [r7, r3, asl #2]
	rsb	r9, r2, r5
	str	r9, [r3, #1048]
	bcc	.L2550
.L2365:
	cmp	r9, #0
	ble	.L2359
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2359
	mov	r5, #0
	b	.L2367
.L2368:
	ldr	r3, [r4, #1264]
	ldr	r2, [r4, #1256]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2359
.L2367:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2368
	b	.L2359
.L2522:
	ldr	r3, .L2551+104
	ldr	ip, [r3, #68]
	movw	r3, #11729
.L2513:
	ldr	r2, .L2551+120
	mov	r0, #20
	ldr	r1, .L2551+124
	blx	ip
	add	r3, r7, #53248
	ldr	r3, [r3, #2820]
	b	.L2324
.L2521:
	mov	r0, r6
	bl	BsSkip
	b	.L2451
.L2545:
	mov	r0, r6
	bl	BsSkip
	b	.L2329
.L2541:
	strb	r3, [lr, #1064]
	mov	r1, #8
	ldr	r3, [r10, #2820]
	mov	r0, r7
	ldr	r2, .L2551+108
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	r3, [r7, r3, asl #2]
	str	r3, [fp, #-60]
	bl	u_v
	ldr	r3, [fp, #-60]
	strb	r0, [r3, #1065]
	ldr	r3, [r10, #2820]
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	ip, [r7, r3, asl #2]
	ldrb	r3, [ip, #1065]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L2353
	mov	r3, #3
	mov	r2, #0
	str	r3, [fp, #-60]
	strb	r2, [ip, #1066]
.L2354:
	ldr	ip, [r10, #2820]
	mov	r1, #16
	ldr	r2, .L2551+112
	mov	r0, r7
	add	ip, ip, #14016
	str	r3, [fp, #-72]
	add	ip, ip, #2
	ldr	ip, [r7, ip, asl #2]
	str	ip, [fp, #-76]
	bl	u_v
	ldr	ip, [fp, #-76]
	ldr	r3, [fp, #-72]
	add	ip, ip, #1056
	mov	r1, r3
	strh	r0, [ip, #12]	@ movhi
	mov	r0, r7
	bl	PassBytes
	ldr	r3, [fp, #-72]
	cmp	r9, r3
	bne	.L2352
	b	.L2359
.L2550:
	ldr	r3, [fp, #-56]
	mov	r0, r7
	rsb	r1, r3, r5
	bl	PassBytes
	cmp	r0, #0
	bge	.L2365
	ldr	r3, [fp, #-68]
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11248
	blx	ip
	b	.L2365
.L2472:
	mov	r2, #0
	mov	r9, r2
	b	.L2349
.L2548:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11741
	blx	ip
	b	.L2458
.L2547:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #10996
	blx	ip
	b	.L2322
.L2546:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11087
	blx	ip
	b	.L2341
.L2544:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11061
	blx	ip
	b	.L2336
.L2539:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11313
	blx	ip
	b	.L2375
.L2538:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11339
	blx	ip
	b	.L2380
.L2537:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11365
	blx	ip
	b	.L2385
.L2536:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11391
	blx	ip
	b	.L2390
.L2535:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11417
	blx	ip
	b	.L2395
.L2534:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11443
	blx	ip
	b	.L2400
.L2533:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11469
	blx	ip
	b	.L2405
.L2532:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11495
	blx	ip
	b	.L2410
.L2531:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11625
	blx	ip
	b	.L2435
.L2530:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11521
	blx	ip
	b	.L2415
.L2529:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11547
	blx	ip
	b	.L2420
.L2528:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11573
	blx	ip
	b	.L2425
.L2527:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11599
	blx	ip
	b	.L2430
.L2526:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11651
	blx	ip
	b	.L2440
.L2525:
	ldr	r3, .L2551+116
	mov	r0, #20
	ldr	r2, .L2551+120
	ldr	r1, .L2551+124
	ldr	ip, [r3, #68]
	movw	r3, #11677
	blx	ip
	b	.L2445
.L2542:
	ldr	r3, [fp, #-68]
	mov	r0, #1
	ldr	r1, .L2551+128
	ldr	r3, [r3, #68]
	blx	r3
.L2514:
	ldr	r3, [r10, #2820]
	ldr	r0, [r10, #4056]
	add	r3, r3, #14016
	add	r3, r3, #2
	ldr	r1, [r7, r3, asl #2]
	bl	FreeUsdByDec
	ldr	r3, [r10, #2820]
	mov	r2, #0
	mvn	r0, #0
	add	r3, r3, #14016
	add	r3, r3, #2
	str	r2, [r7, r3, asl #2]
	b	.L2290
.L2353:
	ldr	r2, .L2551+132
	mov	r1, #8
	mov	r0, r7
	mov	r3, #4
	str	ip, [fp, #-72]
	str	r3, [fp, #-60]
	bl	u_v
	ldr	ip, [fp, #-72]
	ldr	r3, [fp, #-60]
	strb	r0, [ip, #1066]
	b	.L2354
.L2543:
	ldr	r3, [fp, #-68]
	mov	r0, #1
	ldr	r1, .L2551+136
	ldr	r3, [r3, #68]
	blx	r3
	b	.L2514
.L2471:
	mvn	r0, #0
	b	.L2290
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
	b	.L2562
.L2557:
	.word	.L2556
	.word	.L2562
	.word	.L2562
	.word	.L2562
	.word	.L2556
	.word	.L2562
	.word	.L2558
	.word	.L2558
.L2558:
	ldr	r3, [r0]
	mov	r2, #1
	str	r2, [r0]
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2556:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L2559
.L2561:
	mvn	r0, #0
.L2560:
	mov	r2, #0
	str	r2, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2562:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2559:
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
	bhi	.L2561
	mov	r3, r0, asl #1
	rsb	r3, r3, #31
	mov	r3, r4, lsr r3
	subs	r0, r3, #1
	mvnne	r0, #0
	b	.L2560
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
	ble	.L2589
	ldr	r3, [fp, #-48]
	subs	r7, r3, #0
	add	r8, r3, #63
	movge	r8, r3
	ands	r3, r3, #63
	movne	r3, #1
	add	r8, r3, r8, asr #6
	cmp	r8, #0
	ble	.L2577
	ldr	r5, .L2590
	ldr	r10, .L2590+4
.L2575:
	cmp	r7, #64
	ldr	r3, [r10, #52]
	ldr	r0, .L2590
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
	bne	.L2567
	ldr	r2, .L2590
	add	r4, r4, #1
	cmp	r3, r2
	bne	.L2570
	b	.L2568
.L2571:
	cmp	r3, r5
	add	r4, r4, #1
	beq	.L2568
.L2570:
	ldrb	r2, [r3, #-1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2571
.L2567:
	mov	r0, r4, asl #3
.L2566:
	ldr	r3, [fp, #-48]
	cmp	r4, r3
	bge	.L2579
	sub	r3, r3, #1
	rsb	r4, r4, r3
	ldrb	r2, [r9, r4]	@ zero_extendqisi2
	tst	r2, #1
	moveq	r2, r2, lsr #1
	moveq	r3, #1
	bne	.L2579
.L2574:
	tst	r2, #1
	mov	r2, r2, lsr #1
	bne	.L2572
	add	r3, r3, #1
	cmp	r3, #8
	bne	.L2574
	b	.L2572
.L2568:
	subs	r8, r8, #1
	sub	r7, r7, #64
	bne	.L2575
	b	.L2567
.L2579:
	mov	r3, #0
.L2572:
	add	r0, r3, r0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2589:
	ldr	r3, .L2590+4
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L2590+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2577:
	mov	r0, r4
	b	.L2566
.L2591:
	.align	2
.L2590:
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
	beq	.L2598
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2597
	ldr	r1, [r3, #40]
	ldr	r0, [r3, #36]
	bl	FindZeroBitsInSeg
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #40]
	cmp	r0, r2, asl #3
	bcs	.L2601
	cmn	r0, #1
	beq	.L2597
.L2596:
	add	r0, r0, #1
	str	r0, [r3, #72]
	ldr	r3, [r4, #368]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2600
	ldr	r1, [r3, #40]
	ldr	r2, [r3, #72]
	cmp	r2, r1, asl #3
	bcs	.L2602
.L2600:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2602:
	add	r2, r4, #53248
	ldr	r1, [r3, #60]
	ldr	r0, [r2, #4056]
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
.L2601:
	add	r2, r4, #53248
	ldr	r1, [r3, #60]
	ldr	r0, [r2, #4056]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #368]
	mov	r1, #0
	mov	r2, #1
	str	r1, [r3, #36]
	ldr	r3, [r4, #368]
	str	r2, [r3, #68]
	ldr	r3, [r4, #368]
.L2597:
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	bl	FindZeroBitsInSeg
	ldr	r3, [r4, #368]
	b	.L2596
.L2598:
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
	b	CombinePacket.part.9
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
	beq	.L2610
	mov	r2, ip
	mov	r3, #1
	b	.L2608
.L2607:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L2613
.L2608:
	ldrb	r1, [r2, #473]	@ zero_extendqisi2
	add	r2, r2, #96
	cmp	r1, #0
	bne	.L2607
	mov	r0, r3
.L2606:
	mov	r2, r3, asl #7
	mov	r1, #1
	sub	r3, r2, r3, asl #5
	add	ip, ip, r3
	strb	r1, [ip, #377]
	ldmfd	sp, {fp, sp, pc}
.L2613:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2610:
	mov	r3, r0
	b	.L2606
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
	add	r3, r4, #53248
	ldreqb	r0, [r3, #2168]	@ zero_extendqisi2
	beq	.L2616
	ubfx	lr, r0, #16, #6
	ubfx	ip, r0, #15, #1
	ubfx	r1, r0, #12, #3
	strb	lr, [r3, #2169]
	strb	ip, [r3, #2170]
	ubfx	lr, r0, #8, #4
	ubfx	ip, r0, #5, #3
	strb	r1, [r3, #2171]
	strb	lr, [r3, #2172]
	ubfx	r1, r0, #4, #1
	ubfx	lr, r0, #3, #1
	strb	ip, [r3, #2173]
	strb	r1, [r3, #2174]
	ubfx	ip, r0, #2, #1
	and	r1, r0, #3
	strb	lr, [r3, #2175]
	ubfx	r0, r0, #22, #1
	strb	ip, [r3, #2176]
	strb	r1, [r3, #2177]
	strb	r0, [r3, #2168]
.L2616:
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
	add	r6, r0, #53248
	ldr	r10, .L2638
	mov	r4, r0
	mov	r9, r1
	ldr	r3, [r6, #4000]
	mov	r5, r2
	ldr	r1, .L2638+4
	mov	r0, #22
	sub	r2, r3, #1
	ldr	r3, [r10, #68]
	blx	r3
	mov	r3, #0
	str	r3, [r5]
	add	r8, r4, #20480
	str	r3, [r9]
	mov	r0, r4
	ldr	r3, [r4, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r8, #211]
	ldr	r3, [r3, #76]
	str	r3, [r8, #240]
	bl	SliceCheck
	subs	r7, r0, #0
	bne	.L2626
	mov	r0, r4
	bl	ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L2637
	ldrb	r0, [r8, #213]	@ zero_extendqisi2
	add	r2, r4, #12288
	mov	r1, #2208
	mov	lr, #4000
	sub	r0, r0, #1
	add	r4, r4, #24576
	clz	r0, r0
	mov	r0, r0, lsr #5
	str	r0, [r9]
	ldr	ip, [r8, #220]
	ldr	r8, [r2, #1304]
	ldr	r3, [r2, #1300]
	ldr	r0, [r2, #1276]
	mla	r1, r1, ip, r8
	ldr	ip, [r2, #1280]
	ldr	r2, [r1, #28]
	mla	r3, lr, r2, r3
	ldrb	r2, [r3, #18]	@ zero_extendqisi2
	ldr	lr, [r3, #3960]
	rsb	r1, r2, #2
	ldr	r2, [r3, #3956]
	mla	r1, lr, r1, r1
	add	r2, r2, #1
	cmp	r0, r2
	cmpeq	ip, r1
	beq	.L2621
	mov	r2, #1
	str	r2, [r5]
	ldr	r2, [r4, #2056]
	ldr	r1, [r3, #3980]
	add	r3, r2, #1
	cmp	r1, r3
	bls	.L2623
.L2622:
	mov	r3, #1
	str	r3, [r5]
.L2623:
	ldr	r3, [r6, #4020]
	cmp	r3, #0
	cmpne	r0, #0
	beq	.L2619
	ldr	r3, .L2638+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2619
	mov	r3, #0
	ldr	r0, [r6, #4056]
	mov	r2, r3
	mov	r1, #120
	blx	ip
.L2619:
	mov	r0, r7
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2621:
	ldr	r2, [r3, #3980]
	ldr	r3, [r4, #2056]
	add	r3, r3, #1
	cmp	r2, r3
	bhi	.L2622
	ldr	r3, [r5]
	cmp	r3, #1
	beq	.L2623
	mov	r0, r7
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2626:
	mvn	r7, #0
	b	.L2619
.L2637:
	ldr	r3, [r10, #68]
	mov	r0, #1
	ldr	r1, .L2638+12
	mvn	r7, #0
	blx	r3
	b	.L2619
.L2639:
	.align	2
.L2638:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC344
	.word	g_event_report
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
	add	r0, r0, #53248
	ldr	r0, [r0, #3980]
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
	beq	.L2642
	add	r5, r0, #26880
	add	r8, r0, #53248
	add	r5, r5, #160
	mov	r4, #0
	b	.L2646
.L2643:
	ldr	r3, [r0, #520]
	cmp	r3, #0
	beq	.L2645
	ldr	r3, [r6, #252]
	cmp	r4, r3
	bcs	.L2668
.L2646:
	ldr	r3, [r5, #4]!
	add	r4, r4, #1
	ldr	r0, [r8, #4056]
	ldr	r3, [r3, #4]
	ldr	r1, [r3, #620]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L2643
.L2645:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2668:
	cmp	r3, #0
	beq	.L2642
	ldr	r3, [r6, #256]
	cmp	r3, #0
	addne	r7, r7, #27136
	movne	r4, #0
	addne	r7, r7, #36
	bne	.L2649
	b	.L2642
.L2669:
	ldr	r3, [r0, #520]
	cmp	r3, #0
	beq	.L2645
	ldr	r3, [r6, #256]
	cmp	r4, r3
	bcs	.L2642
.L2649:
	ldr	r3, [r7, #4]!
	add	r4, r4, #1
	ldr	r0, [r8, #4056]
	ldr	r3, [r3, #4]
	ldr	r1, [r3, #620]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L2669
	b	.L2645
.L2642:
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
	add	r6, r0, #53248
	mov	r8, r0
	bl	H264_ClearCurrPic
	mov	r0, r8
	bl	ClearAllNal
	ldr	r3, [r6, #3992]
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [r6, #3992]
	ldr	r3, [r6, #2820]
	cmp	r3, #0
	beq	.L2672
	add	r5, r8, #56064
	mov	r4, #0
	add	r5, r5, #4
	mov	r7, r4
.L2674:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L2673
	ldr	r0, [r6, #4056]
	bl	FreeUsdByDec
	str	r7, [r5]
.L2673:
	ldr	r3, [r6, #2820]
	cmp	r3, r4
	bhi	.L2674
.L2672:
	add	r5, r8, #27392
	add	r8, r8, #54272
	add	r5, r5, #152
	add	r8, r8, #152
	mov	r7, #0
.L2678:
	sub	r4, r5, #16
.L2676:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L2675
	ldr	r0, [r6, #4056]
	bl	FreeUsdByDec
	str	r7, [r4]
.L2675:
	cmp	r4, r5
	bne	.L2676
	add	r5, r4, #672
	cmp	r5, r8
	bne	.L2678
	ldr	r3, .L2690
	mov	r0, #2
	ldr	r1, .L2690+4
	ldr	r3, [r3, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L2691:
	.align	2
.L2690:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r3, .L2702
	add	r6, r0, #53248
	mov	r8, r1
	mov	r0, #2
	ldr	r1, .L2702+4
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r0, [r6, #4056]
	mov	r1, r8
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L2697
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	beq	.L2697
	add	r4, r0, #216
	add	r5, r0, #232
	mov	r7, #0
.L2695:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L2694
	ldr	r0, [r6, #4056]
	bl	FreeUsdByDec
	str	r7, [r4]
.L2694:
	cmp	r4, r5
	bne	.L2695
	ldr	r0, [r6, #4056]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2697:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2703:
	.align	2
.L2702:
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r1, #0
	cmpne	r0, #0
	mov	r7, r2
	mov	r4, r0
	moveq	ip, #1
	movne	ip, #0
	mov	r5, r1
	beq	.L2743
	ldr	r3, [r0]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ble	.L2707
	add	r3, r0, #53248
	ldr	r2, [r3, #3984]
	cmp	r2, #2
	beq	.L2707
	ldr	r2, [r3, #2160]
	cmp	r2, #0
	strneb	ip, [r2, #1]
	movne	r0, #1
	strne	ip, [r3, #2160]
	beq	.L2709
.L2706:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2707:
	mov	r1, r5
	mov	r0, r4
	bl	CheckFrameStore
	cmn	r0, #3
	beq	.L2709
	add	r6, r4, #53248
	ldr	r1, [r6, #2160]
	cmp	r1, #0
	beq	.L2710
	mov	r0, r4
	bl	GetImagePara
	ldr	r1, [r6, #2160]
	mov	r0, r4
	bl	CheckFrameStore
	subs	r3, r0, #0
	beq	.L2744
	cmn	r3, #3
	beq	.L2717
	ldr	ip, .L2752
	movw	r2, #2039
	ldr	r1, .L2752+4
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r3, [r6, #2160]
	mov	r0, r4
	ldr	r1, [r3, #204]
	bl	H264DEC_RecycleImage
.L2717:
	ldr	r3, [r6, #2160]
	mov	r0, #0
	strb	r0, [r3, #1]
	ldr	r3, [r6, #2160]
	str	r0, [r6, #2160]
	cmp	r5, r3
	beq	.L2706
.L2710:
	mov	r1, r5
	mov	r0, r4
	bl	GetImagePara
	mov	r1, r5
	mov	r0, r4
	bl	CheckFrameStore
	subs	r3, r0, #0
	beq	.L2745
	cmn	r3, #3
	beq	.L2709
	ldr	ip, .L2752
	movw	r2, #2097
	ldr	r1, .L2752+4
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r1, [r5, #204]
	mov	r0, r4
	bl	H264DEC_RecycleImage
	mov	r0, #2
	b	.L2706
.L2743:
	ldr	ip, .L2752
	mov	r3, r1
	mov	r2, r0
	ldr	r1, .L2752+8
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2709:
	mov	r0, #1
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2745:
	mov	r2, #1
	ldr	r1, [r5, #620]
	ldr	r0, [r6, #4056]
	bl	FSP_SetDisplay
	ldr	r1, [r5, #620]
	ldr	r0, [r6, #4056]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L2746
	cmp	r7, #1
	add	r3, r4, #8
	streq	r7, [r8, #244]
	mov	r2, r4
	ldr	r0, [r6, #4056]
	mov	r1, #0
	str	r8, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L2747
	ldr	r3, [r4]
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r8, #84]
	bcs	.L2748
.L2724:
	ldr	r2, [r6, #4084]
	mov	r0, #0
	ldr	r3, [r6, #4076]
	add	r2, r2, #1
	str	r2, [r6, #4084]
	add	r3, r3, #2
	str	r3, [r6, #4076]
	b	.L2706
.L2744:
	ldr	r3, [r6, #2160]
	mov	r2, #1
	ldr	r0, [r6, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetDisplay
	ldr	r3, [r6, #2160]
	ldr	r0, [r6, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L2749
	ldr	r3, [r6, #2160]
	mov	r2, r4
	mov	r1, #0
	cmp	r5, r3
	cmpeq	r7, #1
	moveq	r3, #1
	streq	r3, [r8, #244]
	add	r3, r4, #8
	ldr	r0, [r6, #4056]
	str	r8, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L2750
	ldr	r3, [r4]
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r8, #84]
	bcs	.L2751
.L2716:
	ldr	r2, [r6, #4084]
	ldr	r3, [r6, #4076]
	add	r2, r2, #1
	str	r2, [r6, #4084]
	add	r3, r3, #2
	str	r3, [r6, #4076]
	b	.L2717
.L2748:
	mov	r1, r8
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L2724
.L2751:
	mov	r1, r8
	mov	r0, r4
	bl	SetFrmRepeatCount.part.1
	b	.L2716
.L2747:
	ldr	r3, .L2752
	mov	r2, r0
	ldr	r1, .L2752+12
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r5, #620]
	ldr	r0, [r6, #4056]
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, r4
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	b	.L2706
.L2750:
	ldr	r3, .L2752
	mov	r2, r0
	ldr	r1, .L2752+12
	mov	r0, #0
	mov	r5, r0
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r6, #2160]
	mov	r2, r5
	strb	r5, [r3, #1]
	ldr	r3, [r6, #2160]
	ldr	r0, [r6, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetDisplay
	str	r5, [r6, #2160]
	mov	r0, r4
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	b	.L2706
.L2746:
	ldr	r3, .L2752
	movw	r2, #2067
	ldr	r1, .L2752+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2706
.L2749:
	ldr	r3, .L2752
	movw	r2, #2010
	ldr	r1, .L2752+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2706
.L2753:
	.align	2
.L2752:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, [r4, #1864]
	strb	r3, [r1, #4]
	ldr	r1, [r4, #1864]
	bl	OutputFrmToVO
	ldr	r3, [r4, #1864]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r7, r0
	bne	.L2755
	mov	r1, r6
	mov	r0, r5
	bl	RemoveFrameStoreOutDPB
.L2755:
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
	ldr	r3, [r6, #2060]
	cmp	r3, #0
	beq	.L2758
	add	r7, r0, #26368
	mov	r5, #0
	add	r7, r7, #68
	b	.L2760
.L2759:
	ldr	r3, [r6, #2060]
	cmp	r3, r5
	bls	.L2758
.L2760:
	ldr	r1, [r7, #4]!
	add	r5, r5, #1
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2759
	mov	r0, r4
	bl	UnMarkFrameStoreRef
	ldr	r3, [r6, #2060]
	cmp	r3, r5
	bhi	.L2760
.L2758:
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L2762
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L2758
	b	.L2762
.L2764:
	bl	GetMinPOC
	ldr	r1, [fp, #-36]
	mov	r0, r4
	bl	OutputFrmFromDPB
	cmn	r0, #1
	beq	.L2763
.L2762:
	ldr	r3, [r6, #2060]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	bne	.L2764
	mov	r0, r3
.L2763:
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
	add	r3, r0, #53248
	add	r7, r0, #54016
	add	r2, r0, #26368
	ldr	r4, [r3, #1468]
	add	r7, r7, #176
	mov	r5, r0
	str	r3, [fp, #-56]
	add	r6, r4, #2000
	add	r4, r4, #4
	add	r6, r6, #4
	add	r3, r2, #132
	str	r3, [fp, #-52]
	add	r3, r0, #16384
	str	r3, [fp, #-48]
.L2785:
	ldr	r3, [r4]
	cmp	r3, #6
	ldrls	pc, [pc, r3, asl #2]
	b	.L2773
.L2775:
	.word	.L2774
	.word	.L2776
	.word	.L2777
	.word	.L2778
	.word	.L2779
	.word	.L2780
	.word	.L2781
.L2781:
	ldr	r2, [r4, #12]
	mov	r1, r7
	mov	r0, r5
	bl	MarkCurrPicLT
.L2773:
	add	r4, r4, #20
	cmp	r4, r6
	bne	.L2785
.L2774:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #3444]
	cmp	r3, #1
	movne	r0, #0
	beq	.L2794
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2780:
	add	r8, r5, #24576
	ldr	r3, [r8, #2064]
	cmp	r3, #0
	ldrne	r9, [fp, #-52]
	movne	r10, #0
	beq	.L2784
.L2783:
	ldr	r1, [r9, #4]!
	mov	r0, r5
	bl	UnMarkFrameStoreRef
	ldr	r1, [r8, #2064]
	add	r10, r10, #1
	cmp	r10, r1
	bcc	.L2783
.L2784:
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
	b	.L2773
.L2779:
	mov	r0, r5
	ldr	r1, [r4, #16]
	bl	UpdateMaxLTFrmIdx
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2773
.L2778:
	ldr	r3, [r4, #12]
	mov	r1, r7
	ldr	r2, [r4, #4]
	mov	r0, r5
	bl	MarkSTToLTRef
	mov	r0, r5
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2773
.L2777:
	mov	r0, r5
	ldr	r2, [r4, #8]
	mov	r1, r7
	bl	UnMarkLTRef
	mov	r0, r5
	bl	UpdateLTReflist
	b	.L2773
.L2776:
	mov	r0, r5
	ldr	r2, [r4, #4]
	mov	r1, r7
	bl	UnMarkSTRef
	mov	r0, r5
	bl	UpdateReflist
	b	.L2773
.L2794:
	ldr	r1, [fp, #-56]
	mov	r3, #0
	ldrb	r2, [r1, #947]	@ zero_extendqisi2
	str	r3, [r1, #1472]
	cmp	r2, #1
	beq	.L2788
	bcc	.L2789
	cmp	r2, #2
	bne	.L2787
	ldr	r2, [fp, #-56]
	str	r3, [r2, #1496]
	str	r3, [r2, #1484]
	ldr	r2, [fp, #-48]
	str	r3, [r2, #3456]
	str	r3, [r2, #3464]
.L2787:
	mov	r0, r5
	bl	FlushDPB
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2789:
	ldr	r0, [fp, #-56]
	ldr	r1, [r0, #1484]
	ldr	r2, [r0, #1492]
	ldr	r3, [r0, #1496]
	rsb	r2, r1, r2
	str	r2, [r0, #1492]
	rsb	r3, r1, r3
	str	r3, [r0, #1496]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r0, #1488]
	str	r3, [r0, #1484]
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
	b	.L2787
.L2788:
	ldr	r2, [fp, #-56]
	str	r3, [r2, #1492]
	str	r3, [r2, #1484]
	ldr	r2, [fp, #-48]
	str	r3, [r2, #3452]
	str	r3, [r2, #3464]
	b	.L2787
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
	add	r5, r0, #53248
	str	r0, [fp, #-48]
	ldr	r3, [r5, #1468]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2796
	add	r9, r0, #24576
	ldr	r3, [r9, #2060]
	cmp	r3, #0
	beq	.L2803
	ldr	r3, [fp, #-48]
	mov	r10, #0
	mov	r4, r10
	add	r8, r3, #26368
	mov	r7, r8
	mov	r6, r8
	add	r7, r7, #132
	add	r8, r8, #72
	add	r6, r6, #196
.L2802:
	ldr	r1, [r8]
	mov	r2, #0
	add	r10, r10, #1
	ldr	r0, [r1, #640]
	cmp	r0, #1
	streq	r0, [r5, #2808]
	strb	r4, [r1, #514]
	strb	r4, [r1, #513]
	ldr	r1, [r8]
	strb	r4, [r1, #550]
	strb	r4, [r1, #549]
	ldr	r1, [r8]
	strb	r4, [r1, #586]
	strb	r4, [r1, #585]
	ldr	r1, [r8]
	strb	r4, [r1, #2]
	ldr	r1, [r8]
	ldr	r0, [r5, #4056]
	ldr	r1, [r1, #620]
	bl	FSP_SetRef
	ldr	r2, [r8]
	mov	r3, #1
	strb	r4, [r2, #1]
	ldr	r2, [r8]
	strb	r4, [r2, #4]
	ldr	r2, [r8]
	str	r3, [r2, #644]
	ldr	r1, [r8]
	ldr	r3, [fp, #-48]
	ldr	r2, [r1, #664]
	add	r2, r2, #6656
	add	r2, r2, #6
	add	r2, r3, r2, lsl #2
	str	r4, [r2, #4]
	ldr	r1, [r1, #620]
	ldr	r0, [r5, #4056]
	bl	FSP_GetDisplay
	mov	r2, #0
	cmp	r0, #3
	beq	.L2801
	ldr	r1, [r8]
	ldr	r0, [r5, #4056]
	ldr	r1, [r1, #620]
	bl	FSP_SetDisplay
.L2801:
	str	r4, [r8], #4
	str	r4, [r7, #4]!
	str	r4, [r6, #4]!
	ldr	r2, [r9, #2060]
	cmp	r2, r10
	bhi	.L2802
.L2803:
	mov	r3, #0
	str	r3, [r9, #2060]
	ldr	r1, [r5, #2796]
	cmp	r1, r3
	beq	.L2799
	ldr	r2, [fp, #-48]
	mov	r0, r3
	add	r2, r2, #55808
	add	r2, r2, #160
.L2806:
	add	r3, r3, #1
	str	r0, [r2, #4]!
	cmp	r3, r1
	bne	.L2806
.L2799:
	ldr	r4, [fp, #-48]
	mov	r0, r4
	bl	UpdateReflist
	mov	r0, r4
	bl	UpdateLTReflist
	ldr	r3, [r5, #1468]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2816
	str	r3, [r9, #2072]
	mov	r0, r3
	strb	r3, [r5, #948]
	mov	r3, #1
	strb	r3, [r5, #949]
.L2807:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2816:
	mov	r3, #0
	mov	r2, #1
	mov	r0, r3
	str	r2, [r9, #2072]
	strb	r2, [r5, #948]
	str	r3, [r5, #1476]
	strb	r3, [r5, #949]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2796:
	bl	FlushDPB
	cmp	r0, #0
	ldreq	r3, [fp, #-48]
	addeq	r9, r3, #24576
	beq	.L2799
.L2808:
	mvn	r0, #0
	b	.L2807
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
	add	r4, r0, #53248
	mov	r3, #0
	mov	r5, r0
	str	r3, [r6, #3444]
	ldrb	r3, [r4, #947]	@ zero_extendqisi2
	sub	r3, r3, #2
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r6, #3448]
	ldrb	r3, [r4, #950]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L2818
	ldr	r3, [r4, #1500]
	cmp	r3, #0
	bne	.L2819
	ldr	r3, [r0]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L2843
.L2819:
	ldr	r3, [r4, #1468]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r4, #1480]
	beq	.L2844
	cmp	r3, #0
	bne	.L2845
.L2826:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2844:
	cmp	r3, #0
	beq	.L2826
.L2825:
	ldrb	r2, [r4, #945]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2830
	add	r2, r5, #24576
	ldr	r1, [r6, #1164]
	ldr	ip, [r2, #2068]
	ldr	r0, [r2, #2064]
	rsb	r2, ip, r1
	cmp	r0, r2
	beq	.L2846
.L2831:
	mov	r2, #0
	strb	r2, [r4, #948]
.L2828:
	cmp	r3, #0
	beq	.L2826
.L2830:
	ldrb	r3, [r4, #948]	@ zero_extendqisi2
	mov	r0, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r4, #949]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2843:
	ldr	r2, [r3, #704]
	cmp	r2, #0
	bne	.L2819
	ldr	r2, [r3, #712]
	cmp	r2, #0
	bne	.L2819
	ldr	r3, [r3, #684]
	add	r3, r3, #1024
	cmp	r3, #2048
	bls	.L2819
.L2818:
	mov	r0, r5
	bl	IDRMemMarking
	cmp	r0, #0
	bne	.L2820
.L2827:
	ldrb	r3, [r4, #950]	@ zero_extendqisi2
	cmp	r3, #5
	ldreq	r3, [r4, #1480]
	beq	.L2828
	ldr	r3, [r4, #1480]
	cmp	r3, #0
	beq	.L2826
	ldr	r2, [r4, #1468]
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2825
	b	.L2830
.L2845:
	mov	r0, r5
	bl	AdaptiveMemMark
	cmp	r0, #0
	beq	.L2827
.L2820:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2846:
	mov	r0, r5
	bl	SlidingWinMark.part.2
	ldr	r3, [r4, #1480]
	b	.L2831
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
	add	r4, r0, #53248
	mov	r5, r0
	ldrb	r6, [r4, #947]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L2849
	bcc	.L2850
	cmp	r6, #2
	beq	.L2851
	mov	r6, #0
.L2848:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2851:
	ldr	r3, [r4, #1464]
	mov	r2, #3
	strb	r2, [r3, #512]
	ldrb	r2, [r4, #945]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2870
	ldr	r1, [r4, #2160]
	cmp	r1, #0
	beq	.L2871
	bl	OutputFrmToVO
	subs	r6, r0, #0
	bne	.L2938
.L2872:
	ldr	r3, [r4, #1464]
	str	r3, [r4, #2160]
	str	r3, [r3, #588]
.L2873:
	ldr	r3, [r4, #2160]
	mov	r8, #2
	ldrb	r0, [r4, #946]	@ zero_extendqisi2
	mov	r7, #0
	mov	r2, #504
	add	r1, r4, #960
	strb	r0, [r3]
	ldr	r3, [r4, #2160]
	strb	r8, [r3, #1]
	ldr	r3, [r4, #2160]
	strb	r7, [r3, #4]
	ldr	r3, [r4, #2160]
	str	r7, [r3, #644]
	ldr	r3, [r4, #2160]
	strb	r7, [r3, #2]
	ldr	r3, [r4, #2160]
	ldr	r0, [r4, #1500]
	str	r0, [r3, #652]
	ldr	r3, [r4, #2160]
	str	r7, [r3, #648]
	ldr	r0, [r4, #2160]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r4, #2160]
	ldr	r2, [r4, #1508]
	str	r2, [r3, #604]
	ldr	r3, [r4, #2160]
	ldr	r2, [r3, #604]
	str	r2, [r3, #532]
	str	r2, [r3, #656]
	ldr	r3, [r4, #2160]
	strb	r8, [r3, #584]
	ldrb	r2, [r4, #951]	@ zero_extendqisi2
	ldr	r3, [r4, #2160]
	cmp	r2, #1
	moveq	r2, r8
	movne	r2, r7
	strb	r2, [r3, #3]
	ldr	r8, [r4, #3988]
	cmp	r8, #1
	bne	.L2848
.L2933:
	ldr	r3, [r4, #2160]
	mov	r2, r7
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetRef
	mov	r2, r7
	mov	r0, r5
	ldr	r1, [r4, #2160]
	bl	OutputFrmToVO
	ldr	r3, [r4, #2160]
	cmp	r3, r7
	mov	r6, r0
	beq	.L2883
	ldr	r3, [r4, #1464]
	mov	r2, r8
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_ClearLogicFs
	b	.L2883
.L2850:
	ldr	r3, [r4, #1464]
	mov	r2, #0
	strb	r2, [r3, #512]
	ldr	r1, [r4, #2160]
	cmp	r1, r2
	beq	.L2852
	bl	OutputFrmToVO
	cmp	r0, #0
	bne	.L2939
.L2853:
	ldr	r3, [r4, #2160]
	cmp	r3, #0
	beq	.L2852
	ldr	r3, [r4, #1464]
	mov	r2, #1
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_ClearLogicFs
.L2852:
	ldr	r3, [r4, #1464]
	mov	r0, #3
	mov	r7, #0
	mov	r2, #504
	add	r1, r4, #960
	str	r3, [r4, #2160]
	str	r3, [r3, #516]
	ldr	r3, [r4, #2160]
	ldr	ip, [r4, #1464]
	str	ip, [r3, #552]
	ldr	r3, [r4, #2160]
	ldr	ip, [r4, #1464]
	str	ip, [r3, #588]
	ldrb	ip, [r4, #946]	@ zero_extendqisi2
	ldr	r3, [r4, #2160]
	strb	ip, [r3]
	ldr	r3, [r4, #2160]
	strb	r0, [r3, #1]
	ldr	r3, [r4, #2160]
	strb	r7, [r3, #2]
	ldr	r3, [r4, #2160]
	strb	r7, [r3, #4]
	ldr	r3, [r4, #2160]
	str	r7, [r3, #644]
	ldr	r0, [r4, #2160]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r4, #2160]
	ldr	r1, [r4, #1508]
	mov	r2, r7
	str	r1, [r3, #532]
	str	r1, [r3, #656]
	ldr	r3, [r4, #2160]
	strb	r7, [r3, #512]
	ldr	r3, [r4, #2160]
	ldr	r1, [r4, #1500]
	str	r1, [r3, #652]
	ldr	r3, [r4, #2160]
	ldrb	r1, [r4, #953]	@ zero_extendqisi2
	str	r1, [r3, #648]
	ldrb	r1, [r4, #951]	@ zero_extendqisi2
	ldr	r3, [r4, #2160]
	cmp	r1, #1
	moveq	r6, #3
	strb	r6, [r3, #3]
	ldr	r3, [r4, #2160]
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetRef
	mov	r2, r7
	mov	r0, r5
	ldr	r1, [r4, #2160]
	bl	OutputFrmToVO
	ldr	r3, [r4, #2160]
	cmp	r3, r7
	mov	r6, r0
	beq	.L2883
.L2931:
	ldr	r3, [r4, #1464]
	mov	r2, #1
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_ClearLogicFs
.L2883:
	mov	r3, #0
	mov	r0, r6
	str	r3, [r4, #2160]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2849:
	ldr	r3, [r4, #1464]
	mov	r2, #3
	strb	r2, [r3, #512]
	ldrb	r2, [r4, #945]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2857
	ldr	r1, [r4, #2160]
	cmp	r1, #0
	beq	.L2858
	bl	OutputFrmToVO
	subs	r6, r0, #0
	bne	.L2940
.L2859:
	ldr	r3, [r4, #1464]
	str	r3, [r4, #2160]
	str	r3, [r3, #552]
.L2860:
	ldr	r3, [r4, #2160]
	mov	r8, #1
	ldrb	r0, [r4, #946]	@ zero_extendqisi2
	mov	r7, #0
	mov	r2, #504
	add	r1, r4, #960
	strb	r0, [r3]
	ldr	r3, [r4, #2160]
	strb	r8, [r3, #1]
	ldr	r3, [r4, #2160]
	strb	r7, [r3, #4]
	ldr	r3, [r4, #2160]
	str	r7, [r3, #644]
	ldr	r3, [r4, #2160]
	strb	r7, [r3, #2]
	ldr	r3, [r4, #2160]
	ldr	r0, [r4, #1500]
	str	r0, [r3, #652]
	ldr	r3, [r4, #2160]
	str	r7, [r3, #648]
	ldr	r0, [r4, #2160]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r4, #2160]
	ldr	r2, [r4, #1508]
	str	r2, [r3, #568]
	ldr	r3, [r4, #2160]
	ldr	r2, [r3, #568]
	str	r2, [r3, #532]
	str	r2, [r3, #656]
	ldr	r3, [r4, #2160]
	strb	r8, [r3, #548]
	ldrb	r3, [r4, #951]	@ zero_extendqisi2
	ldr	r2, [r4, #2160]
	rsb	r3, r8, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r2, #3]
	ldr	r8, [r4, #3988]
	cmp	r8, #1
	beq	.L2933
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2870:
	ldr	ip, [r4, #2160]
	mov	r2, #504
	add	r1, r4, #960
	cmp	ip, #0
	ldreq	r3, [r4, #1464]
	streq	r3, [r4, #2160]
	streq	r3, [r3, #588]
	ldreq	ip, [r4, #2160]
	add	r0, ip, #8
	ldrd	r6, [ip, #24]
	ldr	r3, [ip, #16]
	ldr	r10, [ip, #20]
	strd	r6, [fp, #-52]
	ldrd	r6, [ip, #32]
	ldr	r9, [ip, #76]
	ldr	r8, [ip, #80]
	strd	r6, [fp, #-60]
	str	r3, [fp, #-64]
	ldr	r7, [ip, #84]
	ldr	r6, [ip, #88]
	bl	memcpy
	add	r2, r5, #54016
	ldr	r3, [fp, #-64]
	add	r2, r2, #208
	ldr	r0, [r2, #-8]
	and	r1, r3, r10
	ldr	r2, [r2, #-4]
	and	r0, r0, r2
	adds	r2, r1, #1
	movne	r2, #1
	cmn	r0, #1
	ldrd	r0, [fp, #-52]
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r4, #2160]
	strne	r3, [r2, #16]
	movw	r3, #54224
	strne	r10, [r2, #20]
	ldrd	r2, [r3, r5]
	ldr	ip, [r4, #2160]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r3, r1
	add	r1, r5, #54016
	movhi	r2, r0
	strd	r2, [ip, #24]
	ldrd	r2, [r1, #216]
	ldrd	r0, [fp, #-60]
	ldr	ip, [r4, #2160]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r2, r0
	movhi	r3, r1
	strd	r2, [ip, #32]
	mov	ip, #2
	ldr	r3, [r4, #1028]
	mov	r0, #3
	ldr	r2, [r4, #2160]
	cmp	r3, r9
	movcs	r3, r9
	str	r3, [r2, #76]
	ldr	r3, [r4, #1032]
	ldr	r2, [r4, #2160]
	cmp	r3, r8
	movcs	r3, r8
	str	r3, [r2, #80]
	ldr	r3, [r4, #1036]
	ldr	r2, [r4, #2160]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #84]
	ldr	r3, [r4, #1040]
	ldr	r2, [r4, #2160]
	cmp	r3, r6
	movcs	r3, r6
	str	r3, [r2, #88]
	ldr	r2, [r4, #2160]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #946]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r2, [r4, #2160]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #1]
	ldr	r2, [r4, #2160]
	ldr	r1, [r4, #1500]
	ldr	r3, [r2, #652]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #652]
	ldr	r3, [r4, #2160]
	strb	ip, [r3, #584]
	ldr	r3, [r4, #2160]
	strb	r0, [r3, #512]
	ldr	r3, [r4, #2160]
	ldr	r2, [r4, #1508]
	str	r2, [r3, #604]
	ldr	r2, [r4, #2160]
	ldr	r3, [r2, #604]
	ldr	r1, [r2, #568]
	add	r3, r3, r1
	mov	r3, r3, lsr #1
	str	r3, [r2, #532]
	ldr	r3, [r4, #2160]
	ldr	r2, [r3, #532]
	str	r2, [r3, #656]
	ldr	r3, [r4, #2160]
	ldr	r2, [r3, #588]
	str	r2, [r3, #516]
	ldrb	r3, [r4, #951]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #2160]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	beq	.L2935
.L2882:
	strb	r2, [r3, #3]
	mov	r2, #0
	ldr	r3, [r4, #2160]
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetRef
	mov	r0, r5
	mov	r2, #0
	ldr	r1, [r4, #2160]
	bl	OutputFrmToVO
	ldr	r3, [r4, #2160]
	cmp	r3, #0
	mov	r6, r0
	bne	.L2931
	b	.L2883
.L2857:
	ldr	ip, [r4, #2160]
	mov	r2, #504
	add	r1, r4, #960
	cmp	ip, #0
	ldreq	r3, [r4, #1464]
	streq	r3, [r4, #2160]
	streq	r3, [r3, #552]
	ldreq	ip, [r4, #2160]
	add	r0, ip, #8
	ldrd	r6, [ip, #24]
	ldr	r3, [ip, #16]
	ldr	r10, [ip, #20]
	strd	r6, [fp, #-52]
	ldrd	r6, [ip, #32]
	ldr	r9, [ip, #76]
	ldr	r8, [ip, #80]
	strd	r6, [fp, #-60]
	str	r3, [fp, #-64]
	ldr	r7, [ip, #84]
	ldr	r6, [ip, #88]
	bl	memcpy
	add	r2, r5, #54016
	ldr	r3, [fp, #-64]
	add	r2, r2, #208
	ldr	r0, [r2, #-8]
	and	r1, r3, r10
	ldr	r2, [r2, #-4]
	and	r0, r0, r2
	adds	r2, r1, #1
	movne	r2, #1
	cmn	r0, #1
	ldrd	r0, [fp, #-52]
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r4, #2160]
	strne	r3, [r2, #16]
	movw	r3, #54224
	strne	r10, [r2, #20]
	ldrd	r2, [r3, r5]
	ldr	ip, [r4, #2160]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r3, r1
	add	r1, r5, #54016
	movhi	r2, r0
	strd	r2, [ip, #24]
	ldrd	r2, [r1, #216]
	ldrd	r0, [fp, #-60]
	ldr	ip, [r4, #2160]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r2, r0
	movhi	r3, r1
	strd	r2, [ip, #32]
	mov	ip, #1
	ldr	r3, [r4, #1028]
	mov	r0, #3
	ldr	r2, [r4, #2160]
	cmp	r3, r9
	movcs	r3, r9
	str	r3, [r2, #76]
	ldr	r3, [r4, #1032]
	ldr	r2, [r4, #2160]
	cmp	r3, r8
	movcs	r3, r8
	str	r3, [r2, #80]
	ldr	r3, [r4, #1036]
	ldr	r2, [r4, #2160]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #84]
	ldr	r3, [r4, #1040]
	ldr	r2, [r4, #2160]
	cmp	r3, r6
	movcs	r3, r6
	str	r3, [r2, #88]
	ldr	r2, [r4, #2160]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #946]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r2, [r4, #2160]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #1]
	ldr	r2, [r4, #2160]
	ldr	r1, [r4, #1500]
	ldr	r3, [r2, #652]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #652]
	ldr	r3, [r4, #2160]
	strb	ip, [r3, #548]
	ldr	r3, [r4, #2160]
	strb	r0, [r3, #512]
	ldr	r3, [r4, #2160]
	ldr	r2, [r4, #1508]
	str	r2, [r3, #568]
	ldr	r2, [r4, #2160]
	ldr	r3, [r2, #604]
	ldr	r1, [r2, #568]
	add	r3, r3, r1
	mov	r3, r3, lsr ip
	str	r3, [r2, #532]
	ldr	r3, [r4, #2160]
	ldr	r2, [r3, #532]
	str	r2, [r3, #656]
	ldr	r3, [r4, #2160]
	ldr	r2, [r3, #552]
	str	r2, [r3, #516]
	ldrb	r3, [r4, #951]	@ zero_extendqisi2
	cmp	r3, ip
	ldr	r3, [r4, #2160]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	bne	.L2882
.L2935:
	orr	r2, r2, ip
	b	.L2882
.L2939:
	ldr	r1, .L2941
	movw	r3, #2691
	str	r0, [sp]
	mov	r0, #22
	ldr	r2, .L2941+4
	ldr	ip, [r1, #68]
	ldr	r1, .L2941+8
	blx	ip
	b	.L2853
.L2871:
	ldr	r3, [r4, #1464]
	mov	r6, r1
	str	r3, [r4, #2160]
	str	r3, [r3, #588]
	b	.L2873
.L2858:
	ldr	r3, [r4, #1464]
	mov	r6, r1
	str	r3, [r4, #2160]
	str	r3, [r3, #552]
	b	.L2860
.L2938:
	ldr	r1, .L2941
	movw	r3, #2885
	str	r6, [sp]
	mov	r0, #22
	ldr	r2, .L2941+4
	ldr	ip, [r1, #68]
	ldr	r1, .L2941+8
	blx	ip
	b	.L2872
.L2940:
	ldr	r1, .L2941
	movw	r3, #2743
	str	r6, [sp]
	mov	r0, #22
	ldr	r2, .L2941+4
	ldr	ip, [r1, #68]
	ldr	r1, .L2941+8
	blx	ip
	b	.L2859
.L2942:
	.align	2
.L2941:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r3, [r5, #2060]
	cmp	r3, #0
	beq	.L2945
	add	r7, r0, #26368
	mov	r6, #0
	add	r7, r7, #68
	b	.L2947
.L2946:
	ldr	r3, [r5, #2060]
	cmp	r3, r6
	bls	.L2945
.L2947:
	ldr	r1, [r7, #4]!
	add	r6, r6, #1
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2946
	mov	r0, r4
	bl	UnMarkFrameStoreRef
	ldr	r3, [r5, #2060]
	cmp	r3, r6
	bhi	.L2947
.L2945:
	mov	r0, r4
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	bne	.L2945
	mvn	r6, #0
.L2949:
	ldr	r3, [r5, #2060]
	sub	r2, fp, #36
	sub	r1, fp, #32
	mov	r0, r4
	cmp	r3, #0
	beq	.L2961
	bl	GetMinPOC
	ldr	r3, [fp, #-36]
	mov	r1, #0
	mov	r0, r4
	add	r3, r4, r3, lsl #2
	add	r3, r3, #24576
	ldr	r2, [r3, #1864]
	strb	r1, [r2, #4]
	ldr	r1, [r3, #1864]
	ldr	r2, [r5, #2060]
	bl	OutputFrmToVO
	ldr	r1, [fp, #-36]
	add	r3, r1, #6592
	add	r3, r3, #18
	ldr	r3, [r4, r3, asl #2]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r6, r0
	bne	.L2949
	mov	r0, r4
	bl	RemoveFrameStoreOutDPB
	b	.L2949
.L2961:
	cmp	r6, #0
	beq	.L2952
	add	r0, r4, #8
	bl	GetVoLastImageID
	mov	r6, r0
.L2952:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	r7, r0, #0
	beq	.L2979
	bl	ClearAllSlice
	add	r6, r7, #26368
	mov	r8, #0
	add	r10, r7, #53248
	mov	r5, r6
	mov	r4, r6
	add	r5, r5, #200
	add	r6, r6, #72
	add	r4, r4, #136
	mov	r9, r8
.L2967:
	ldr	r3, [r6]
	mov	r2, #1
	add	r8, r8, r2
	cmp	r3, #0
	beq	.L2964
	ldr	r1, [r3, #620]
	ldr	r0, [r10, #4056]
	bl	FSP_ClearLogicFs
.L2964:
	ldr	r3, [r4]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2965
	ldr	r1, [r3, #620]
	ldr	r0, [r10, #4056]
	bl	FSP_ClearLogicFs
.L2965:
	ldr	r3, [r5]
	mov	r2, #1
	cmp	r3, #0
	beq	.L2966
	ldr	r1, [r3, #620]
	ldr	r0, [r10, #4056]
	bl	FSP_ClearLogicFs
.L2966:
	cmp	r8, #16
	str	r9, [r5], #4
	mov	r2, #0
	str	r9, [r4], #4
	str	r9, [r6], #4
	bne	.L2967
	add	r3, r7, #16384
	add	r4, r7, #24576
	ldr	r3, [r3, #1192]
	str	r2, [r4, #2060]
	sub	r3, r3, #1
	str	r2, [r4, #2064]
	str	r3, [r4, #2056]
	str	r2, [r4, #2068]
	str	r2, [r4, #2072]
	ldr	r1, [r10, #2160]
	cmp	r1, r2
	ldreq	r5, .L2997
	beq	.L2969
	ldr	r3, [r10, #3984]
	cmp	r3, #2
	beq	.L2970
	ldr	r5, .L2997
.L2974:
	ldr	r3, [r10, #1464]
	cmp	r3, #0
	beq	.L2972
	ldr	r1, [r3, #620]
	mov	r2, #1
	ldr	r0, [r10, #4056]
	bl	FSP_ClearLogicFs
.L2972:
	mov	r3, #0
	str	r3, [r10, #2160]
.L2969:
	add	r0, r7, #26624
	ldr	r3, [r5, #48]
	mov	r2, #392
	mov	r1, #0
	add	r0, r0, #28
	blx	r3
	ldr	r1, [r4, #2056]
	add	r3, r7, #55808
	mov	r0, #0
	str	r0, [r4, #2336]
	mov	r2, r3
	add	r3, r3, #160
	add	r2, r2, #228
	str	r1, [r4, #2332]
	mov	r1, r0
.L2976:
	str	r1, [r3, #4]!
	cmp	r3, r2
	bne	.L2976
	add	r2, r7, #26880
	add	r3, r7, #27136
	add	r2, r2, #160
	add	r3, r3, #36
	mov	r0, #0
.L2977:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L2977
	add	r2, r7, #27136
	add	r7, r7, #20480
	add	r2, r2, #168
	mov	ip, r1
	str	r1, [r7, #252]
.L2978:
	str	ip, [r3, #4]!
	cmp	r3, r2
	mov	r0, #0
	bne	.L2978
	str	r0, [r7, #256]
.L2963:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2970:
	mov	r0, r7
	ldr	r5, .L2997
	bl	OutputFrmToVO
	cmp	r0, #0
	bne	.L2996
.L2973:
	ldr	r3, [r10, #2160]
	cmp	r3, #0
	bne	.L2974
	b	.L2969
.L2996:
	str	r0, [sp]
	movw	r3, #5624
	ldr	r2, .L2997+4
	mov	r0, #22
	ldr	ip, [r5, #68]
	ldr	r1, .L2997+8
	blx	ip
	b	.L2973
.L2979:
	mvn	r0, #0
	b	.L2963
.L2998:
	.align	2
.L2997:
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
	beq	.L3000
	ldr	ip, .L3004
	movw	r2, #505
	ldr	r1, .L3004+4
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
.L3000:
	mov	r0, r4
	add	r4, r4, #53248
	bl	InitDPB
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #2808]
	str	r3, [r4, #4060]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3005:
	.align	2
.L3004:
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r4, r0, #53248
	mov	r6, r0
	ldrb	r3, [r4, #947]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3007
	ldrb	r3, [r4, #945]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3007
.L3008:
	ldr	r5, [r4, #4044]
	ldr	r3, [r4, #3984]
	ubfx	r5, r5, #16, #3
	cmp	r3, r5
	beq	.L3009
	subs	r2, r5, #2
	sub	r3, r3, #2
	movne	r2, #1
	cmp	r3, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L3009
	cmp	r5, #4
	cmpne	r5, #1
	bls	.L3086
.L3009:
	cmp	r5, #2
	str	r5, [r4, #3984]
	beq	.L3012
	cmp	r5, #3
	beq	.L3013
	cmp	r5, #1
	beq	.L3087
	ldr	r3, [r4, #3980]
	cmp	r3, #0
	beq	.L3082
	cmp	r5, #4
	ldr	r3, [r4, #1500]
	beq	.L3088
.L3023:
	cmp	r3, #2
	beq	.L3037
.L3027:
	cmp	r3, #1
	streq	r3, [r4, #2812]
.L3029:
	ldr	r1, .L3092
	mov	r0, #2
	ldr	r2, [r4, #4020]
	ldrb	r3, [r4, #947]	@ zero_extendqisi2
	ldr	ip, [r1, #68]
	sub	r2, r2, #1
	ldr	r1, .L3092+4
	blx	ip
	ldr	r0, [r4, #4020]
	ldr	ip, [r4, #1524]
	mov	r3, #1
	ldr	r1, [r4, #1528]
	cmp	r0, #0
	ldr	r2, [r4, #3980]
	str	ip, [r4, #3504]
	str	r1, [r4, #3500]
	str	r2, [r4, #3704]
	strb	r3, [r4, #2842]
	beq	.L3079
	ldr	r1, [r4, #1500]
	cmp	r1, #0
	bne	.L3030
	ldr	r3, .L3092+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3031
	ldr	r2, [r4, #3708]
	cmp	r2, #0
	beq	.L3032
.L3033:
	ldr	r3, [r2, #8]
	ldr	r0, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3033
.L3032:
	sub	r2, fp, #28
	mov	r3, #4
	ldr	r0, [r4, #4056]
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
	ldr	r2, [r6]
.L3034:
	add	r3, r6, #56064
	add	r3, r3, #24
	str	r3, [r6, #360]
	ldr	r1, [r2, #884]
	cmp	r1, #1
	beq	.L3089
	cmp	r1, #2
	beq	.L3090
.L3036:
	ldr	r3, [r2, #676]
	mov	r0, #0
	str	r3, [r4, #3968]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3007:
	ldr	r3, [r4, #4080]
	ldr	r1, [r4, #4084]
	add	r3, r3, #1
	ldr	r2, [r6]
	cmp	r3, r1
	movlt	r3, r1
	str	r3, [r4, #4080]
	str	r3, [r2, #600]
	ldr	r3, [r4, #4080]
	ldr	r1, [r4, #4084]
	rsb	r3, r1, r3
	str	r3, [r2, #604]
	b	.L3008
.L3086:
	ldr	r3, .L3092
	mov	r2, r5
	ldr	r1, .L3092+12
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	str	r5, [r4, #3984]
.L3013:
	mov	r0, r6
	bl	H264_ClearCurrPic
	mov	r0, r6
	bl	ClearDPB
	mvn	r0, #0
.L3079:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3012:
	ldr	r5, [r4, #1500]
	cmp	r5, #0
	bne	.L3019
	ldr	r3, [r4, #3980]
	cmp	r3, #0
	beq	.L3082
	ldr	r3, [r4, #3976]
	ldr	r3, [r3, #48]
	cmp	r3, #0
	beq	.L3029
	mov	r3, #1
	mov	r0, r6
	strb	r3, [r4, #951]
	bl	H264_ClearCurrPic
	ldr	r3, .L3092
	mov	r0, r5
	ldr	r1, .L3092+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3079
.L3087:
	ldr	r3, [r4, #1500]
	cmp	r3, #2
	beq	.L3091
	ldr	r2, [r4, #3980]
	cmp	r2, #0
	bne	.L3027
.L3082:
	mov	r0, r6
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3079
.L3030:
	add	r3, r6, #24576
	ldr	r3, [r3, #2060]
	cmp	r3, #0
	bne	.L3031
	ldr	r2, [r6]
	ldr	r3, [r2, #12]
	cmp	r3, #2
	beq	.L3034
	b	.L3082
.L3088:
	cmp	r3, #0
	bne	.L3023
	ldr	r3, [r4, #2812]
	cmp	r3, #0
	moveq	r3, #2
	streq	r3, [r4, #2812]
	movne	r3, #1
	strne	r3, [r4, #2812]
	b	.L3029
.L3091:
	ldr	r3, [r4, #1480]
	cmp	r3, #0
	beq	.L3019
	ldr	r3, [r4, #3980]
	cmp	r3, #0
	beq	.L3082
.L3037:
	ldr	r3, [r4, #2812]
	cmp	r3, #2
	bne	.L3029
	ldr	r3, [r6]
	ldr	r5, [r3, #12]
	cmp	r5, #0
	bne	.L3029
	ldr	r3, .L3092+20
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3029
	ldr	r1, .L3092
	mov	r0, #1
	ldr	r3, [r4, #1480]
	ldr	r2, [r4, #1484]
	ldr	ip, [r1, #68]
	ldr	r1, .L3092+24
	blx	ip
	mov	r0, r6
	bl	H264_ClearCurrPic
	ldr	r3, [r4, #1464]
	ldr	r0, [r4, #4056]
	mov	r2, r5
	ldr	r1, [r3, #620]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L3079
.L3031:
	ldr	r2, [r6]
	b	.L3034
.L3089:
	cmp	r3, #0
	beq	.L3036
	ldr	r3, [r4, #1500]
	cmp	r3, #2
	bne	.L3036
	mov	r2, #0
	str	r2, [r6, #360]
	ldr	r3, [r4, #1464]
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetDisplay
	b	.L3082
.L3090:
	ldr	r1, [r2, #888]
	cmp	r3, #0
	cmpne	r1, #0
	ble	.L3036
	ldr	r3, [r4, #1500]
	cmp	r3, #2
	bne	.L3036
	mov	r2, #0
	str	r2, [r6, #360]
	ldr	r3, [r4, #1464]
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_SetDisplay
	ldr	r2, [r6]
	mov	r0, r6
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3079
.L3019:
	mov	r3, #1
	mov	r0, r6
	strb	r3, [r4, #951]
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3079
.L3093:
	.align	2
.L3092:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC358
	.word	g_event_report
	.word	.LC355
	.word	.LC356
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
	ldr	r3, .L3096
	mov	r0, r4
	ldr	r1, .L3096+4
	ldr	r3, [r3, #68]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L3097:
	.align	2
.L3096:
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r7, .L3161
	ldr	lr, [r1, #12]
	add	r6, r0, #53248
	ldr	r3, [r1, #8]
	mov	r4, r0
	ldr	r2, [r1, #4]
	mov	r5, r1
	mov	r0, #7
	ldr	ip, [r7, #68]
	ldr	r1, .L3161+4
	str	lr, [sp]
	blx	ip
	mov	r3, #0
	str	r3, [r6, #4088]
	ldr	r3, [r4]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L3156
.L3099:
	ldr	r3, [r4, #368]
	cmp	r3, #0
	beq	.L3119
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3157
.L3102:
	ldr	r1, .L3161+8
	mov	r0, #1
	ldr	r3, [r7, #68]
	blx	r3
	ldr	r1, [r4, #368]
	cmp	r1, #0
	beq	.L3119
	ldr	r0, [r6, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #368]
.L3119:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3106
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L3106
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3106
	ldrb	r3, [r4, #377]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3122
	mov	r2, r4
	mov	r3, #1
	b	.L3109
.L3108:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L3120
.L3109:
	ldrb	r1, [r2, #473]	@ zero_extendqisi2
	add	r2, r2, #96
	cmp	r1, #0
	bne	.L3108
	mov	r2, r3
.L3107:
	mov	r1, r3, asl #7
	cmn	r2, #1
	sub	r3, r1, r3, asl #5
	mov	r1, #1
	add	r3, r4, r3
	strb	r1, [r3, #377]
	beq	.L3120
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
	b	.L3112
.L3157:
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bhi	.L3102
	ldrb	r1, [r3, #4]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3103
	b	.L3102
.L3156:
	ldr	r3, [r3, #892]
	cmp	r3, #0
	beq	.L3099
	ldr	r3, [r4, #368]
	cmp	r3, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	beq	.L3119
.L3103:
	strb	r2, [r3, #4]
	ldr	r3, [r4]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L3158
.L3114:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3115
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq	.L3115
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3115
	ldr	r3, [r4, #368]
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bls	.L3159
.L3116:
	mov	r2, #1
	strb	r2, [r3, #4]
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #4056]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3161+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3153
	mov	r3, #0
	ldr	r0, [r6, #4056]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3153:
	ldr	r3, [r4, #368]
.L3112:
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	strb	r2, [r3, #5]
	ldr	r3, [r4, #368]
	ldrb	r0, [r3, #4]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3106:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L3161+16
	blx	r3
	ldr	r1, [r5, #16]
	ldr	r0, [r6, #4056]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3161+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3155
.L3154:
	mov	r3, #0
	ldr	r0, [r6, #4056]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3155:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3115:
	ldr	r3, [r4, #368]
	b	.L3116
.L3158:
	ldr	r3, [r3, #892]
	cmp	r3, #0
	bne	.L3153
	b	.L3114
.L3159:
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
	bcc	.L3160
.L3117:
	mov	r2, #1
	str	r2, [r6, #4088]
	b	.L3112
.L3160:
	mov	r0, r4
	bl	CombinePacket.part.9
	ldr	r3, [r4, #368]
	b	.L3117
.L3120:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L3161+20
	blx	r3
	mov	r0, r4
	bl	ClearAllNal
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	ldr	r3, .L3161+12
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3154
	b	.L3155
.L3122:
	mov	r2, r3
	b	.L3107
.L3162:
	.align	2
.L3161:
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r9, .L3189
	mov	r8, r0
	mov	r4, r1
	mov	r0, #22
	ldr	r1, .L3189+4
	add	r7, r8, #53248
	ldr	r3, [r9, #68]
	blx	r3
	mov	r0, r8
	bl	H264_ClearCurrPic
	mov	r0, r8
	bl	ClearAllNal
	mov	r0, r8
	bl	InitDPB
	cmp	r4, #0
	beq	.L3164
	add	r1, r8, #8
	ldr	r0, [r7, #4056]
	bl	FSP_ClearNotInVoQueue
.L3165:
	add	r4, r8, #27392
	mov	r6, #0
	add	r4, r4, #152
	mov	r10, r6
.L3168:
	sub	r5, r4, #16
	sub	r3, r4, #232
	mov	r2, #1
	mvn	lr, #0
	mov	ip, #17
	mov	r0, #16
	mov	r1, #2
	strb	r10, [r4, #-232]
	strb	r10, [r4, #-231]
	strb	r10, [r4, #-230]
	str	r10, [r4, #400]
	str	r10, [r4, #392]
	str	r10, [r4, #404]
	strb	r10, [r4, #-228]
	str	r6, [r4, #-28]
	strb	r10, [r4, #280]
	str	lr, [r4, #396]
	str	r2, [r4, #412]
	strb	r2, [r4, #316]
	str	ip, [r4, #428]
	str	r0, [r4, #432]
	str	r3, [r4, #356]
	str	r3, [r4, #320]
	str	r3, [r4, #284]
	strb	r1, [r4, #352]
.L3167:
	ldr	r1, [r5, #4]!
	cmp	r1, #0
	beq	.L3166
	ldr	r0, [r7, #4056]
	bl	FreeUsdByDec
	str	r10, [r5]
.L3166:
	cmp	r5, r4
	bne	.L3167
	add	r6, r6, #1
	add	r4, r5, #672
	cmp	r6, #40
	bne	.L3168
	add	r4, r8, #54272
	mov	r6, #0
	mov	r5, r4
	add	r4, r4, #144
	add	r5, r5, #160
.L3170:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L3169
	ldr	r0, [r7, #4056]
	bl	FreeUsdByDec
	str	r6, [r4]
.L3169:
	cmp	r4, r5
	bne	.L3170
	ldr	r3, [r7, #2820]
	cmp	r3, #0
	beq	.L3174
	add	r5, r8, #56064
	mov	r4, #0
	add	r5, r5, #4
	mov	r6, r4
.L3173:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3172
	ldr	r0, [r7, #4056]
	bl	FreeUsdByDec
	str	r6, [r5]
.L3172:
	ldr	r3, [r7, #2820]
	cmp	r3, r4
	bhi	.L3173
.L3174:
	add	r5, r8, #20480
	add	ip, r8, #24576
	mov	r4, #0
	ldr	r3, [r9, #48]
	str	r4, [r7, #2820]
	add	r0, r5, #208
	str	r4, [r7, #4032]
	mov	r1, r4
	str	r4, [r7, #2800]
	mov	r2, #1648
	str	r4, [r7, #3980]
	str	r4, [r7, #4040]
	str	r4, [r7, #3988]
	str	r4, [r7, #2808]
	str	r4, [r7, #4060]
	str	r4, [r7, #2812]
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
	strb	r4, [r7, #944]
	strb	r4, [r7, #2128]
	str	r4, [r7, #2132]
	str	r3, [r7, #2136]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3164:
	add	r0, r8, #8
	bl	ResetVoQueue
	ldr	r0, [r7, #4056]
	bl	FSP_EmptyInstance
	b	.L3165
.L3190:
	.align	2
.L3189:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC364
	UNWIND(.fnend)
	.size	ClearAll, .-ClearAll
	.align	2
	.global	InsertFrmInDPB
	.type	InsertFrmInDPB, %function
InsertFrmInDPB:
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
	UNWIND(.pad #44)
	sub	sp, sp, #44
	ldrb	r7, [r2, #3]	@ zero_extendqisi2
	mov	r4, r2
	mov	r5, r0
	cmp	r7, #1
	str	r1, [fp, #-48]
	beq	.L3193
	bcc	.L3194
	cmp	r7, #2
	beq	.L3195
	ldr	r3, .L3291
	mov	r2, r7
	ldr	r1, .L3291+4
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	mov	r1, #1
	bl	ClearAll
	mov	r0, #0
.L3199:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3195:
	ldr	r3, [r2, #520]
	mov	r2, #3
	strb	r2, [r3, #512]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [fp, #-48]
	mov	r3, r3, asl #2
	beq	.L3215
	add	r6, r0, r3
	add	r6, r6, #24576
	ldr	r2, [r6, #1864]
	cmp	r2, #0
	beq	.L3215
	strb	r7, [r2, #584]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #520]
	str	r2, [r3, #588]
	ldr	r3, [r6, #1864]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #585]
	ldr	r3, [r6, #1864]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #586]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #552]
	str	r2, [r3, #600]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #556]
	str	r2, [r3, #608]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #564]
	str	r2, [r3, #604]
	ldr	r3, [r6, #1864]
	ldr	r1, [r3, #568]
	ldr	r2, [r3, #604]
	add	r2, r2, r1
	str	r2, [r3, #532]
	str	r2, [r3, #656]
	add	r1, r4, #16
	ldr	r2, [r6, #1864]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r2, [r6, #1864]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #1]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r6, #1864]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	orreq	r2, r2, #2
	strb	r2, [r3, #3]
	mov	r2, #504
	ldr	lr, [r6, #1864]
	add	r0, lr, #8
	ldr	r3, [lr, #16]
	ldrd	r8, [lr, #24]
	ldr	ip, [lr, #80]
	str	r3, [fp, #-80]
	ldr	r3, [lr, #20]
	strd	r8, [fp, #-60]
	ldrd	r8, [lr, #32]
	str	r3, [fp, #-72]
	ldr	r3, [lr, #84]
	ldr	r10, [lr, #76]
	ldr	r7, [lr, #88]
	strd	r8, [fp, #-68]
	str	ip, [fp, #-76]
	str	r3, [fp, #-52]
	bl	memcpy
	ldr	r3, [fp, #-80]
	ldr	lr, [fp, #-72]
	ldr	r0, [r4, #24]
	ldr	r1, [r4, #28]
	and	ip, r3, lr
	adds	r2, ip, #1
	ldrd	r8, [fp, #-60]
	and	r1, r1, r0
	ldr	ip, [fp, #-76]
	movne	r2, #1
	cmn	r1, #1
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r6, #1864]
	strne	r3, [r2, #16]
	strne	lr, [r2, #20]
	ldrd	r2, [r4, #32]
	ldr	r1, [r6, #1864]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r2, r8
	movhi	r3, r9
	ldrd	r8, [fp, #-68]
	strd	r2, [r1, #24]
	ldrd	r2, [r4, #40]
	ldr	r1, [r6, #1864]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r2, r8
	movhi	r3, r9
	strd	r2, [r1, #32]
	ldr	r3, [r4, #84]
	ldr	r2, [r6, #1864]
	cmp	r3, r10
	ldr	r1, [fp, #-52]
	movcs	r3, r10
	str	r3, [r2, #76]
	ldr	r3, [r4, #88]
	ldr	r2, [r6, #1864]
	cmp	r3, ip
	movcs	r3, ip
	str	r3, [r2, #80]
	ldr	r3, [r4, #92]
	ldr	r2, [r6, #1864]
	cmp	r3, r1
	movcs	r3, r1
	str	r3, [r2, #84]
	ldr	r3, [r4, #96]
	ldr	r2, [r6, #1864]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #88]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3227
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3227
.L3228:
	ldr	r2, [r6, #1864]
	ldr	r3, [r2, #608]
	ldr	r1, [r2, #572]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #652]
	ldr	r2, [r4, #540]
	ldr	r3, [r6, #1864]
	str	r2, [r3, #636]
	ldr	r0, [r6, #1864]
	bl	CombineFldsToFrm
	ldr	r2, [r6, #1864]
	ldr	r3, [r2, #664]
	ldr	r2, [r2, #600]
	add	r3, r3, #6656
	add	r3, r3, #54
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3214
.L3194:
	ldr	r3, [fp, #-48]
	ldr	r2, [r2, #520]
	add	r8, r0, r3, lsl #2
	mov	r3, #3
	add	r8, r8, #24576
	str	r2, [r8, #1864]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r2]
	ldr	r2, [r8, #1864]
	strb	r3, [r2, #1]
	ldr	r1, [r4, #520]
	ldr	r2, [r8, #1864]
	ldrb	r1, [r1, #4]	@ zero_extendqisi2
	strb	r1, [r2, #4]
	ldr	r1, [r4, #520]
	ldr	r2, [r8, #1864]
	ldr	r1, [r1, #644]
	str	r1, [r2, #644]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	ldr	r2, [r8, #1864]
	cmp	r1, #0
	beq	.L3289
.L3196:
	strb	r3, [r2, #2]
	add	r1, r4, #16
	ldr	r3, [r8, #1864]
	mov	r2, #504
	ldr	r0, [r4, #588]
	mov	r6, #0
	mov	r10, #1
	str	r0, [r3, #660]
	ldr	r0, [r8, #1864]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #564]
	mov	ip, #2
	mov	r2, r6
	mov	r0, r5
	str	r1, [r3, #532]
	str	r1, [r3, #656]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #520]
	str	r1, [r3, #588]
	str	r1, [r3, #552]
	str	r1, [r3, #516]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #532]
	str	r1, [r3, #632]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #528]
	str	r1, [r3, #624]
	ldr	r3, [r8, #1864]
	strb	r6, [r3, #512]
	ldr	r3, [r8, #1864]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	strb	r1, [r3, #513]
	ldr	r3, [r8, #1864]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	strb	r1, [r3, #514]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #556]
	str	r1, [r3, #536]
	str	r1, [r3, #652]
	ldr	r3, [r8, #1864]
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	str	r1, [r3, #648]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #540]
	str	r1, [r3, #636]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #544]
	str	r1, [r3, #528]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #548]
	str	r1, [r3, #564]
	ldr	r3, [r8, #1864]
	ldr	r1, [r4, #552]
	str	r1, [r3, #600]
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	ldr	r3, [r8, #1864]
	cmp	r1, r10
	moveq	r7, #3
	strb	r7, [r3, #3]
	ldr	r3, [r8, #1864]
	ldr	r1, [r3, #516]
	ldrb	r8, [r3, #513]	@ zero_extendqisi2
	ldrb	r7, [r3, #514]	@ zero_extendqisi2
	ldr	lr, [r3, #532]
	strb	ip, [r3, #584]
	ldr	ip, [r3, #536]
	strb	r10, [r3, #548]
	strb	r8, [r3, #549]
	strb	r8, [r3, #585]
	strb	r7, [r3, #550]
	strb	r7, [r3, #586]
	str	lr, [r3, #568]
	str	lr, [r3, #604]
	str	ip, [r3, #572]
	str	ip, [r3, #608]
	str	r1, [r3, #552]
	str	r1, [r3, #588]
	ldr	r1, [r4, #520]
	bl	GetAPC
	cmp	r0, #0
	bne	.L3290
.L3214:
	ldr	r3, [fp, #-48]
	add	r1, r5, #53248
	mov	r2, #1
	add	r3, r3, #6592
	ldr	r0, [r1, #4056]
	add	r3, r3, #18
	ldr	r3, [r5, r3, asl #2]
	ldr	r1, [r3, #620]
	bl	FSP_SetRef
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3193:
	ldr	r3, [r2, #520]
	mov	r2, #3
	strb	r2, [r3, #512]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [fp, #-48]
	mov	r3, r3, asl #2
	beq	.L3200
	add	r6, r0, r3
	add	r6, r6, #24576
	ldr	r2, [r6, #1864]
	cmp	r2, #0
	beq	.L3200
	strb	r7, [r2, #548]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #556]
	str	r2, [r3, #572]
	ldr	r3, [r6, #1864]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #549]
	ldr	r3, [r6, #1864]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #550]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #548]
	str	r2, [r3, #564]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #556]
	str	r2, [r3, #572]
	ldr	r3, [r6, #1864]
	ldr	r2, [r4, #564]
	str	r2, [r3, #568]
	ldr	r3, [r6, #1864]
	ldr	r1, [r3, #568]
	ldr	r2, [r3, #604]
	add	r2, r2, r1
	str	r2, [r3, #532]
	str	r2, [r3, #656]
	add	r1, r4, #16
	ldr	r2, [r6, #1864]
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2]
	ldr	r2, [r6, #1864]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #1]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r6, #1864]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	orreq	r2, r2, #1
	strb	r2, [r3, #3]
	mov	r2, #504
	ldr	lr, [r6, #1864]
	add	r0, lr, #8
	ldr	r3, [lr, #16]
	ldrd	r8, [lr, #24]
	ldr	ip, [lr, #80]
	str	r3, [fp, #-80]
	ldr	r3, [lr, #20]
	strd	r8, [fp, #-60]
	ldrd	r8, [lr, #32]
	str	r3, [fp, #-72]
	ldr	r3, [lr, #84]
	ldr	r10, [lr, #76]
	ldr	r7, [lr, #88]
	strd	r8, [fp, #-68]
	str	ip, [fp, #-76]
	str	r3, [fp, #-52]
	bl	memcpy
	ldr	r3, [fp, #-80]
	ldr	lr, [fp, #-72]
	ldr	r0, [r4, #24]
	ldr	r1, [r4, #28]
	and	ip, r3, lr
	adds	r2, ip, #1
	ldrd	r8, [fp, #-60]
	and	r1, r1, r0
	ldr	ip, [fp, #-76]
	movne	r2, #1
	cmn	r1, #1
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r6, #1864]
	strne	r3, [r2, #16]
	strne	lr, [r2, #20]
	ldrd	r2, [r4, #32]
	ldr	r1, [r6, #1864]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r2, r8
	movhi	r3, r9
	ldrd	r8, [fp, #-68]
	strd	r2, [r1, #24]
	ldrd	r2, [r4, #40]
	ldr	r1, [r6, #1864]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r2, r8
	movhi	r3, r9
	strd	r2, [r1, #32]
	ldr	r3, [r4, #84]
	ldr	r2, [r6, #1864]
	cmp	r3, r10
	ldr	r1, [fp, #-52]
	movcs	r3, r10
	str	r3, [r2, #76]
	ldr	r3, [r4, #88]
	ldr	r2, [r6, #1864]
	cmp	r3, ip
	movcs	r3, ip
	str	r3, [r2, #80]
	ldr	r3, [r4, #92]
	ldr	r2, [r6, #1864]
	cmp	r3, r1
	movcs	r3, r1
	str	r3, [r2, #84]
	ldr	r3, [r4, #96]
	ldr	r2, [r6, #1864]
	cmp	r3, r7
	movcs	r3, r7
	str	r3, [r2, #88]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3211
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3211
.L3212:
	ldr	r2, [r6, #1864]
	ldr	r3, [r2, #608]
	ldr	r1, [r2, #572]
	cmp	r3, r1
	movcc	r3, r1
	str	r3, [r2, #652]
	ldr	r2, [r4, #540]
	ldr	r3, [r6, #1864]
	str	r2, [r3, #636]
	ldr	r0, [r6, #1864]
	bl	CombineFldsToFrm
	ldr	r2, [r6, #1864]
	ldr	r3, [r2, #664]
	ldr	r2, [r2, #564]
	add	r3, r3, #6656
	add	r3, r3, #38
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3214
.L3200:
	add	r6, r5, r3
	ldr	r3, [r4, #520]
	add	r6, r6, #24576
	mov	r2, #1
	str	r3, [r6, #1864]
	strb	r2, [r3, #548]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #520]
	str	r1, [r3, #552]
	ldr	r3, [r6, #1864]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	strb	r1, [r3, #549]
	ldr	r3, [r6, #1864]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	strb	r1, [r3, #550]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #548]
	str	r1, [r3, #564]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #556]
	str	r1, [r3, #572]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #564]
	str	r1, [r3, #568]
	ldr	r3, [r6, #1864]
	ldr	r1, [r3, #568]
	str	r1, [r3, #532]
	str	r1, [r3, #656]
	ldr	r3, [r6, #1864]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r3]
	ldr	r3, [r6, #1864]
	strb	r2, [r3, #1]
	ldr	r2, [r4, #520]
	ldr	r3, [r6, #1864]
	ldrb	r2, [r2, #4]	@ zero_extendqisi2
	strb	r2, [r3, #4]
	ldr	r2, [r4, #520]
	ldr	r3, [r6, #1864]
	ldr	r2, [r2, #644]
	str	r2, [r3, #644]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3202
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3202
.L3203:
	ldr	r3, [r6, #1864]
	mov	r7, #0
	mov	r2, #504
	add	r1, r4, #16
	str	r7, [r3, #648]
	ldr	r0, [r6, #1864]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #588]
	mov	r2, #1
	mov	r0, r5
	str	r1, [r3, #660]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #528]
	str	r1, [r3, #624]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #540]
	str	r1, [r3, #636]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #556]
	str	r1, [r3, #652]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [r6, #1864]
	rsb	r3, r2, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r1, #3]
	ldr	r1, [r4, #520]
	bl	GetAPC
	cmp	r0, #0
	beq	.L3214
	mov	r3, r0
	ldr	ip, .L3291
	mov	r0, r7
	movw	r2, #2381
	b	.L3287
.L3215:
	add	r6, r5, r3
	ldr	r3, [r4, #520]
	add	r6, r6, #24576
	mov	r2, #2
	str	r3, [r6, #1864]
	strb	r2, [r3, #584]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #520]
	str	r1, [r3, #588]
	ldr	r3, [r6, #1864]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	strb	r1, [r3, #585]
	ldr	r3, [r6, #1864]
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	strb	r1, [r3, #586]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #552]
	str	r1, [r3, #600]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #556]
	str	r1, [r3, #608]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #564]
	str	r1, [r3, #604]
	ldr	r3, [r6, #1864]
	ldr	r1, [r3, #568]
	str	r1, [r3, #532]
	str	r1, [r3, #656]
	ldr	r3, [r6, #1864]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r3]
	ldr	r3, [r6, #1864]
	strb	r2, [r3, #1]
	ldr	r2, [r4, #520]
	ldr	r3, [r6, #1864]
	ldrb	r2, [r2, #4]	@ zero_extendqisi2
	strb	r2, [r3, #4]
	ldr	r2, [r4, #520]
	ldr	r3, [r6, #1864]
	ldr	r2, [r2, #644]
	str	r2, [r3, #644]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3217
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3217
.L3218:
	ldr	r3, [r6, #1864]
	mov	r8, #0
	mov	r2, #504
	add	r1, r4, #16
	str	r8, [r3, #648]
	ldr	r0, [r6, #1864]
	add	r0, r0, #8
	bl	memcpy
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #588]
	mov	r2, #2
	mov	r0, r5
	str	r1, [r3, #660]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #528]
	str	r1, [r3, #624]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #540]
	str	r1, [r3, #636]
	ldr	r3, [r6, #1864]
	ldr	r1, [r4, #556]
	str	r1, [r3, #608]
	str	r1, [r3, #652]
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	ldr	r3, [r6, #1864]
	cmp	r1, #1
	moveq	r7, r2
	movne	r7, r8
	strb	r7, [r3, #3]
	ldr	r1, [r4, #520]
	bl	GetAPC
	cmp	r0, #0
	beq	.L3214
	ldr	ip, .L3291
	mov	r3, r0
	movw	r2, #2526
	mov	r0, r8
.L3287:
	ldr	r1, .L3291+8
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	bl	ClearDPB
	mvn	r0, #0
	b	.L3199
.L3289:
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	b	.L3196
.L3202:
	ldr	r3, [r6, #1864]
	mov	r2, #1
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, r2
	ldreq	r3, [r6, #1864]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #632]
	b	.L3203
.L3217:
	ldr	r3, [r6, #1864]
	mov	r2, #2
	strb	r2, [r3, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r6, #1864]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #632]
	b	.L3218
.L3211:
	ldr	r2, [r6, #1864]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r6, #1864]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #632]
	b	.L3212
.L3227:
	ldr	r2, [r6, #1864]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r6, #1864]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #632]
	b	.L3228
.L3290:
	mov	r3, r0
	ldr	ip, .L3291
	mov	r0, r6
	movw	r2, #2328
	b	.L3287
.L3292:
	.align	2
.L3291:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC366
	.word	.LC365
	UNWIND(.fnend)
	.size	InsertFrmInDPB, .-InsertFrmInDPB
	.align	2
	.global	StorePicInDPB
	.type	StorePicInDPB, %function
StorePicInDPB:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	add	r3, r0, #16384
	add	r4, r0, #53248
	mov	r8, #0
	str	r8, [r3, #3444]
	mov	r5, r0
	ldrb	r2, [r4, #947]	@ zero_extendqisi2
	sub	r1, r2, #2
	clz	r1, r1
	mov	r1, r1, lsr #5
	str	r1, [r3, #3448]
	ldrb	r0, [r4, #944]	@ zero_extendqisi2
	cmp	r0, r8
	beq	.L3460
	ldr	r0, [r4, #3984]
	ldr	r3, [r4, #4068]
	cmp	r0, #2
	str	r3, [r4, #4064]
	beq	.L3461
	ldr	r6, [r5]
	ldr	r3, [r6, #12]
	cmp	r3, #0
	ble	.L3462
	ldrb	r1, [r4, #946]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3301
.L3473:
	sub	r1, r2, #1
	ldr	r8, [r4, #1464]
	cmp	r1, #1
	movhi	r1, #0
	movls	r1, #1
	cmp	r2, #0
	beq	.L3302
	ldrb	r0, [r8, #1]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	ip, r1
	orrne	ip, r1, #1
	cmp	ip, #0
	beq	.L3301
	cmp	r1, #0
	beq	.L3463
.L3377:
	ldr	r1, [r4, #4060]
	cmp	r1, #0
	bne	.L3305
	mov	r3, #1
	mov	r2, #592
	str	r3, [r4, #4060]
	add	r1, r4, #944
	add	r0, r5, #54784
	bl	memcpy
	ldr	r3, [r6, #12]
.L3301:
	cmp	r3, #2
	ldr	r3, [r4, #1480]
	beq	.L3464
	cmp	r3, #0
	beq	.L3338
	ldr	r3, [r4, #4048]
	ldr	r2, [r4, #1508]
	cmp	r3, r2
	bcs	.L3339
	ldrb	r1, [r4, #946]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L3465
.L3339:
	mov	r0, r5
	bl	Marking
	subs	r2, r0, #0
	beq	.L3341
	ldr	r6, .L3486
	mov	r0, #1
	ldr	r1, .L3486+4
	ldr	r3, [r6, #68]
	blx	r3
	ldr	ip, [r6, #68]
	movw	r3, #3917
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3302:
	cmp	r1, #0
	bne	.L3377
.L3308:
	add	r6, r5, #54016
	add	r6, r6, #176
.L3310:
	cmp	r2, #0
	beq	.L3311
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3466
.L3311:
	ldr	r3, [r6, #564]
	str	r3, [r8, #656]
.L3312:
	mov	r1, r8
	mov	r0, r5
	bl	GetImagePara
	ldr	r1, [r8, #620]
	ldr	r0, [r4, #4056]
	bl	FSP_GetFsImagePtr
	subs	r7, r0, #0
	beq	.L3467
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3314
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3468
.L3314:
	ldrd	r2, [r6, #24]
.L3451:
	strd	r2, [r7, #8]
.L3315:
	ldrd	r2, [r6, #32]
	strd	r2, [r7, #16]
	ldrd	r2, [r6, #40]
	strd	r2, [r7, #24]
	ldr	r3, [r6, #84]
	str	r3, [r7, #68]
	ldr	r3, [r6, #88]
	str	r3, [r7, #72]
	ldr	r3, [r6, #92]
	str	r3, [r7, #76]
	ldr	r3, [r6, #96]
	str	r3, [r7, #80]
	ldr	r2, [r8, #656]
	cmp	r2, #0
	str	r2, [r7, #200]
	beq	.L3321
	ldr	r3, [r7, #160]
	ands	r9, r3, #3
	beq	.L3469
.L3319:
	ldr	r3, [r5]
	ldr	r1, [r3, #4]
	cmp	r1, r2
	bcc	.L3470
.L3321:
	mov	r2, #1
	ldr	r1, [r8, #620]
	ldr	r0, [r4, #4056]
	bl	FSP_SetDisplay
	ldr	r0, [r4, #4056]
	add	r3, r5, #8
	str	r7, [sp]
	mov	r2, r5
	mov	r1, #0
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L3471
	mov	r2, #0
	ldr	r1, [r8, #620]
	ldr	r0, [r4, #4056]
	bl	FSP_SetDisplay
.L3323:
	ldr	r3, [r5]
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r7, #84]
	bcs	.L3472
.L3325:
	ldr	r2, [r4, #4060]
	ldr	r3, [r4, #4076]
	cmp	r2, #2
	add	r3, r3, #2
	str	r3, [r4, #4076]
	movne	r3, #1
	strne	r3, [r8, #640]
	mov	r3, #2
	str	r3, [r4, #4060]
	ldr	r3, [r5]
	ldr	r3, [r3, #12]
	b	.L3301
.L3460:
	ldr	r6, .L3486
	ldr	r1, .L3486+12
	ldr	r3, [r6, #68]
	blx	r3
	ldr	ip, [r6, #68]
	movw	r3, #3636
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
.L3376:
	ldr	r3, [r4, #1464]
	mov	r2, #1
	ldr	r0, [r4, #4056]
	mvn	r7, #0
	ldr	r1, [r3, #620]
	bl	FSP_ClearLogicFs
	mov	r3, r7
	movw	r2, #4136
	ldr	ip, [r6, #68]
	ldr	r1, .L3486+16
	mov	r0, #1
	blx	ip
	ldr	r2, [r4, #1464]
	mov	r3, #0
	strb	r3, [r2, #4]
	ldr	r2, [r4, #1464]
	strb	r3, [r2, #1]
.L3374:
	mov	r0, r7
	mov	r3, #0
	strb	r3, [r4, #944]
.L3448:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3462:
	ldr	r1, [r4, #4060]
	cmp	r1, #2
	beq	.L3301
	ldrb	r1, [r4, #946]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3301
	b	.L3473
.L3464:
	cmp	r3, #0
	beq	.L3331
	ldrb	r3, [r4, #950]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3332
.L3336:
	mov	r0, r5
	add	r6, r5, #24576
	bl	SimpleSlideDPB
	add	r2, r5, #54016
	ldr	r1, [r6, #2060]
	add	r2, r2, #176
	mov	r0, r5
	bl	InsertFrmInDPB
	subs	r7, r0, #0
	beq	.L3474
	ldr	r6, .L3486
	mov	r3, r7
	movw	r2, #3883
	ldr	r1, .L3486+16
	mov	r0, #0
	ldr	ip, [r6, #68]
	blx	ip
	ldr	ip, [r6, #68]
	movw	r3, #3884
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3463:
	cmp	r0, #0
	beq	.L3301
	b	.L3308
.L3341:
	mov	r0, r5
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
	mov	r0, r5
	bl	RemoveUnUsedFrameStore
	cmp	r0, #0
	beq	.L3331
	ldr	r6, .L3486
	mov	r0, #22
	ldr	r1, .L3486+20
	ldr	r3, [r6, #68]
	blx	r3
.L3331:
	ldrb	r3, [r4, #945]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3343
	add	r6, r5, #24576
.L3347:
	ldr	r3, [r4, #1480]
	cmp	r3, #0
	beq	.L3475
	ldr	r1, [r6, #2064]
	ldr	r2, [r6, #2068]
	ldr	r3, [r6, #2056]
	add	r0, r2, r1
	cmp	r3, r0
	bhi	.L3355
	ldr	r0, [r5]
	ldr	r0, [r0, #12]
	cmp	r0, #3
	beq	.L3356
	ldr	r0, .L3486+24
	ldrb	r0, [r0]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L3356
	ldr	r6, .L3486
	mov	r0, #1
	str	r2, [sp, #4]
	movw	r2, #3996
	str	r1, [sp]
	ldr	r1, .L3486+28
	ldr	ip, [r6, #68]
	blx	ip
	mov	r0, r5
	bl	ClearDPB
	ldr	ip, [r6, #68]
	movw	r3, #4022
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3338:
	ldrb	r3, [r4, #945]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3343
	add	r6, r5, #24576
	ldr	r3, [r6, #2056]
.L3355:
	mvn	r9, #0
	mov	r8, r9
	b	.L3365
.L3366:
	bl	GetMinPOC
	ldr	r3, [r4, #1480]
	cmp	r3, #0
	bne	.L3358
	ldr	r2, [r4, #1484]
	ldr	r3, [fp, #-56]
	cmp	r2, r3
	blt	.L3476
.L3358:
	ldr	r7, [r6, #2060]
	cmp	r7, r9
	ldrne	r8, [fp, #-52]
	bne	.L3363
	ldr	r3, [fp, #-52]
	cmp	r3, r8
	beq	.L3477
	mov	r8, r3
.L3363:
	mov	r1, r8
	mov	r0, r5
	bl	OutputFrmFromDPB
	mov	r9, r7
	cmn	r0, #1
	mov	r10, r0
	beq	.L3364
	ldr	r3, [r6, #2056]
.L3365:
	ldr	ip, [r6, #2060]
	sub	r2, fp, #52
	sub	r1, fp, #56
	mov	r0, r5
	cmp	ip, r3
	bcs	.L3366
	ldr	r3, [r4, #1480]
	cmp	r3, #0
	beq	.L3367
	ldrb	r3, [r4, #948]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3367
	ldr	r2, [r6, #2064]
	cmp	r2, #0
	beq	.L3367
	add	r2, r5, #26368
	mov	lr, r3
	add	r2, r2, #132
	b	.L3372
.L3369:
	ldr	r1, [r6, #2064]
	cmp	r1, r3
	bls	.L3478
.L3372:
	ldr	r1, [r2, #4]!
	add	r3, r3, #1
	ldr	r0, [r4, #1472]
	ldr	ip, [r1, #624]
	cmp	ip, r0
	bne	.L3369
	strb	lr, [r1, #2]
	ldr	r0, [r2]
	ldrb	r1, [r0, #4]	@ zero_extendqisi2
	cmp	r1, #1
	streq	r1, [r0, #644]
	ldreq	r1, [r2]
	streqb	lr, [r1, #4]
	ldreq	r0, [r2]
	ldr	r7, [r4, #2796]
	ldr	ip, [r0, #660]
	add	r1, ip, #13952
	cmp	ip, r7
	add	r1, r1, #40
	addne	r1, r5, r1, lsl #2
	strne	lr, [r1, #4]
	strne	r7, [r0, #660]
	b	.L3369
.L3478:
	ldr	ip, [r6, #2060]
.L3367:
	add	r2, r5, #54016
	mov	r1, ip
	add	r2, r2, #176
	mov	r0, r5
	bl	InsertFrmInDPB
	ldr	r2, [r6, #2060]
	str	r2, [r4, #2156]
	subs	r3, r0, #0
	bne	.L3479
	add	r2, r2, #1
	mov	r0, r5
	str	r2, [r6, #2060]
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
.L3337:
	mov	r7, #0
	b	.L3374
.L3475:
	ldr	r3, [r6, #2056]
	b	.L3355
.L3343:
	ldr	r3, [r4, #2160]
	ldr	r0, [r4, #1464]
	cmp	r3, #0
	rsb	r1, r3, r0
	clz	r1, r1
	mov	r1, r1, lsr #5
	moveq	r1, #0
	cmp	r1, #0
	beq	.L3480
	mov	r0, r5
	bl	DirectOutput
	cmn	r0, #1
	mov	r7, r0
	beq	.L3481
	cmp	r0, #1
	ldreq	r2, [r4, #1464]
	moveq	r3, #0
	ldrne	r6, .L3486
	ldreq	r6, .L3486
	streqb	r3, [r2, #4]
	ldreq	r2, [r4, #1464]
	streqb	r3, [r2, #1]
.L3351:
	ldr	ip, [r6, #68]
	movw	r3, #3953
	mov	r2, r7
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3299
.L3461:
	ldr	r6, .L3486
	ldr	r1, .L3486+32
	ldr	r3, [r6, #68]
	blx	r3
	mov	r0, r5
	bl	DirectOutput
	ldr	r3, [r4, #1532]
	ldr	r2, [r4, #2796]
	ldr	r1, .L3486+8
	cmp	r3, r2
	addne	r3, r3, #13952
	addne	r3, r3, #40
	addne	r5, r5, r3, lsl #2
	strne	r8, [r5, #4]
	strne	r2, [r4, #1532]
	cmp	r0, #1
	mov	r7, r0
	ldreq	r2, [r4, #1464]
	moveq	r3, #0
	streqb	r3, [r2, #4]
	ldreq	r2, [r4, #1464]
	streqb	r3, [r2, #1]
	mov	r2, r0
	ldr	ip, [r6, #68]
	movw	r3, #3687
	mov	r0, #14
	blx	ip
.L3299:
	cmn	r7, #1
	bne	.L3374
	b	.L3376
.L3305:
	cmp	r1, #1
	beq	.L3482
	cmp	r2, #0
	beq	.L3308
	ldrb	r1, [r8, #1]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3308
	b	.L3301
.L3480:
	add	r6, r5, #24576
	ldr	ip, [r6, #2060]
	cmp	ip, #0
	beq	.L3347
	ldr	r3, [r6, #1864]
	cmp	r0, r3
	addne	r3, r5, #26368
	addne	r3, r3, #72
	beq	.L3348
.L3352:
	add	r1, r1, #1
	cmp	r1, ip
	beq	.L3347
	ldr	r2, [r3, #4]!
	cmp	r0, r2
	bne	.L3352
.L3348:
	add	r2, r5, #54016
	mov	r0, r5
	add	r2, r2, #176
	bl	InsertFrmInDPB
	cmn	r0, #1
	mov	r7, r0
	beq	.L3483
	mov	r0, r5
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
	ldr	ip, .L3486
	movw	r3, #3973
	mov	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3337
.L3364:
	ldr	r6, .L3486
	mov	r3, r0
	movw	r2, #4072
	ldr	r1, .L3486+16
	mov	r0, #1
	ldr	ip, [r6, #68]
	blx	ip
	mov	r2, r10
	ldr	ip, [r6, #68]
	movw	r3, #4073
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3470:
	ldr	r3, .L3486+36
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3328
	str	r2, [fp, #-52]
	mov	r3, #8
	str	r1, [fp, #-48]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r4, #4056]
	blx	ip
.L3328:
	ldr	r6, .L3486
	mov	r2, #0
	ldr	r1, [r8, #620]
	ldr	r0, [r4, #4056]
	bl	FSP_SetDisplay
	ldr	ip, [r6, #68]
	mov	r3, #3840
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3479:
	ldr	r6, .L3486
	movw	r2, #4109
	ldr	r1, .L3486+16
	mov	r0, #1
	ldr	ip, [r6, #68]
	blx	ip
	ldr	ip, [r6, #68]
	movw	r3, #4110
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3474:
	ldr	r3, [r6, #2060]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r6, #2060]
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
	ldr	ip, .L3486
	mov	r2, r7
	movw	r3, #3892
	ldr	r1, .L3486+8
	mov	r0, #14
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3337
.L3356:
	sub	r2, fp, #52
	sub	r1, fp, #56
	mov	r0, r5
	bl	GetMinPOC
	ldr	r1, [fp, #-52]
	mov	r0, r5
	bl	RemoveFrameStoreOutDPB
	mov	r0, r5
	bl	UpdateReflist
	mov	r0, r5
	bl	UpdateLTReflist
	ldr	r3, [r6, #2056]
	b	.L3355
.L3469:
	ldr	r6, .L3486
	movw	r2, #3807
	ldr	r1, .L3486+40
	mov	r0, #1
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, .L3486+36
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3320
	mov	r3, r9
	mov	r2, r9
	mov	r1, #111
	ldr	r0, [r4, #4056]
	blx	ip
.L3320:
	ldr	r2, [r7, #200]
	cmp	r2, #0
	bne	.L3319
	b	.L3321
.L3468:
	ldrd	r0, [r6, #24]
	mvn	r3, #0
	mvn	r2, #0
	cmp	r1, r3
	cmpeq	r0, r2
	bne	.L3315
	add	r3, r5, #54784
	add	r3, r3, #32
	ldrd	r2, [r3, #-8]
	cmp	r3, r1
	cmpeq	r2, r0
	bne	.L3451
	b	.L3315
.L3481:
	ldr	r6, .L3486
	mov	r3, r0
	movw	r2, #3943
	ldr	r1, .L3486+16
	mov	r0, #1
	ldr	ip, [r6, #68]
	blx	ip
	b	.L3351
.L3465:
	ldr	r6, .L3486
	mov	r0, #1
	ldr	r1, .L3486+44
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, .L3486+36
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3340
	ldr	r7, [r4, #1508]
	mov	r3, #8
	ldr	lr, [r4, #4048]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r4, #4056]
	str	r7, [fp, #-52]
	str	lr, [fp, #-48]
	blx	ip
.L3340:
	mov	r0, r5
	bl	H264_ClearCurrPic
	mov	r0, r5
	bl	ClearDPB
	ldr	ip, [r6, #68]
	movw	r3, #3908
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3471:
	ldr	r3, [r4, #4084]
	add	r3, r3, #1
	str	r3, [r4, #4084]
	b	.L3323
.L3476:
	ldr	r3, [r4, #3984]
	cmp	r3, #2
	beq	.L3484
.L3359:
	mov	r0, r5
	bl	DirectOutput
	cmn	r0, #1
	mov	r7, r0
	beq	.L3485
	cmp	r0, #1
	ldreq	r2, [r4, #1464]
	moveq	r3, #0
	ldrne	r6, .L3486
	ldreq	r6, .L3486
	streqb	r3, [r2, #4]
	ldreq	r2, [r4, #1464]
	streqb	r3, [r2, #1]
.L3361:
	ldr	ip, [r6, #68]
	movw	r3, #4052
	mov	r2, r7
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3299
.L3477:
	ldr	r6, .L3486
	mov	r3, r8
	ldr	r2, .L3486+48
	mov	r0, #1
	ldr	r1, .L3486+52
	ldr	ip, [r6, #68]
	blx	ip
	ldr	ip, [r6, #68]
	movw	r3, #4059
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3472:
	mov	r1, r7
	mov	r0, r5
	bl	SetFrmRepeatCount.part.1
	b	.L3325
.L3332:
	mov	r0, r5
	bl	IDRMemMarking
	cmp	r0, #0
	beq	.L3336
	mvn	r0, #0
	b	.L3448
.L3482:
	add	r6, r5, #54016
	ldr	r8, [r4, #2056]
	add	r6, r6, #176
	b	.L3310
.L3466:
	mov	r2, #3
	ldr	r3, [r8, #656]
	strb	r2, [r8, #1]
	ldr	r2, [r6, #564]
	add	r3, r3, r2
	str	r3, [r8, #656]
	b	.L3312
.L3484:
	ldrb	r3, [r4, #947]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r4, #1464]
	strne	r3, [r4, #2164]
	b	.L3359
.L3483:
	ldr	r6, .L3486
	mov	r3, r0
	movw	r2, #3966
	ldr	r1, .L3486+16
	mov	r0, #1
	ldr	ip, [r6, #68]
	blx	ip
	mov	r2, r7
	ldr	ip, [r6, #68]
	movw	r3, #3967
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3467:
	ldr	r6, .L3486
	movw	r2, #3779
	ldr	r1, .L3486+56
	ldr	r3, [r6, #68]
	blx	r3
	ldr	ip, [r6, #68]
	movw	r3, #3780
	mvn	r2, #0
	ldr	r1, .L3486+8
	mov	r0, #14
	blx	ip
	b	.L3376
.L3485:
	ldr	r6, .L3486
	mov	r3, r0
	movw	r2, #4043
	ldr	r1, .L3486+16
	mov	r0, #1
	ldr	ip, [r6, #68]
	blx	ip
	b	.L3361
.L3487:
	.align	2
.L3486:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC374
	.word	.LC368
	.word	.LC367
	.word	.LC372
	.word	.LC375
	.word	g_apple_airplay_mirrorinig_flag
	.word	.LC376
	.word	.LC369
	.word	g_event_report
	.word	.LC371
	.word	.LC373
	.word	.LANCHOR1+252
	.word	.LC377
	.word	.LC370
	UNWIND(.fnend)
	.size	StorePicInDPB, .-StorePicInDPB
	.align	2
	.global	AllocFrameStore
	.type	AllocFrameStore, %function
AllocFrameStore:
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
	add	r8, r0, #20480
	mov	r7, r0
	str	r1, [fp, #-52]
	ldrb	r6, [r8, #209]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L3489
	ldrb	r3, [r8, #210]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3551
	add	r5, r0, #53248
	ldr	r6, [r5, #3984]
	cmp	r6, #2
	beq	.L3527
	ldrb	r3, [r5, #2128]	@ zero_extendqisi2
	mov	r2, #0
	strb	r2, [r5, #945]
	cmp	r3, #1
	mov	r3, #2
	str	r3, [fp, #-48]
	bne	.L3519
.L3515:
	ldr	r2, [r5, #2136]
	ldr	r3, [r8, #228]
	cmp	r2, r3
	bne	.L3519
	ldr	r3, [r8, #240]
	cmp	r3, #0
	ldr	r3, [r5, #2132]
	beq	.L3552
	cmp	r3, #0
	beq	.L3519
.L3495:
	ldr	r3, [r5, #4068]
	ldr	r2, [r5, #4064]
	rsb	r3, r2, r3
	cmp	r3, #2
	bhi	.L3519
	ldr	r1, [r5, #1464]
	mov	r3, #1
	strb	r3, [r5, #945]
	cmp	r1, #0
	beq	.L3497
	ldr	r3, [fp, #-48]
	cmp	r3, #2
	beq	.L3553
	ldr	r3, [fp, #-48]
	cmp	r3, #1
	beq	.L3554
.L3522:
	mov	ip, #0
	b	.L3501
.L3489:
	add	r5, r0, #53248
	ldr	r3, [r5, #3984]
	cmp	r3, #2
	strneb	r6, [r5, #945]
	strne	r6, [fp, #-48]
	beq	.L3527
.L3519:
	ldr	r4, [r5, #2792]
	mov	lr, #0
	strb	lr, [r5, #944]
	cmp	r4, lr
	beq	.L3509
	add	ip, r7, #27136
	add	r10, r7, #26368
	add	ip, ip, #176
	add	r10, r10, #72
	add	r9, r7, #24576
.L3508:
	ldrb	r3, [ip, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3504
	cmp	ip, #0
	beq	.L3524
	ldr	r0, [r9, #2060]
	cmp	r0, #0
	beq	.L3524
	ldr	r2, [r9, #1864]
	cmp	r2, ip
	beq	.L3504
	mov	r2, r10
	b	.L3506
.L3507:
	ldr	r1, [r2, #4]!
	cmp	r1, ip
	beq	.L3504
.L3506:
	add	r3, r3, #1
	cmp	r3, r0
	bne	.L3507
.L3524:
	mov	r9, #672
	mov	r4, ip
	mla	r9, r9, lr, r7
	movw	ip, #27968
	mov	r10, #0
	str	r4, [r5, #1464]
	mov	r6, #1
	mov	lr, #101
	add	r1, r9, #27136
	strb	r6, [r5, #944]
	mov	r2, #504
	add	r1, r1, #184
	str	r10, [r9, ip]
	add	r0, r5, #960
	str	lr, [r5, #1508]
	bl	memcpy
	add	lr, r9, #27136
	str	r10, [r5, #1160]
	strb	r10, [lr, #179]
	str	r10, [r4, #644]
	ldr	r2, [r5, #1464]
	strb	r10, [r2, #2]
	ldr	r2, [r5, #1464]
	strb	r6, [r2, #4]
	ldr	r3, [r5, #1464]
	str	r10, [r3, #640]
	ldrb	r4, [r5, #944]	@ zero_extendqisi2
	cmp	r4, r6
	bne	.L3509
	ldr	r3, [fp, #-52]
	ldr	r0, [r5, #4056]
	clz	r1, r3
	mov	r1, r1, lsr #5
	bl	FSP_NewLogicFs
	mov	r10, r0
	mov	r1, r0
	ldr	r0, [r5, #4056]
	bl	FSP_GetLogicFs
	mvn	r3, r10
	mov	r3, r3, lsr #31
	cmp	r0, #0
	mov	r9, r0
	moveq	r3, #0
	cmp	r3, #0
	beq	.L3510
	ldr	r3, [r7]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L3555
.L3511:
	ldr	r4, .L3558
	mov	r2, #504
	ldr	r0, [r5, #1464]
	add	r1, r9, #8
	ldr	r3, [r4, #52]
	add	r0, r0, #8
	blx	r3
	ldr	r1, [r5, #1464]
	add	r0, r7, #54016
	ldr	r3, [r4, #52]
	add	r1, r1, #8
	add	r0, r0, #192
	mov	r2, #504
	blx	r3
	ldr	r3, [r5, #1464]
	mov	ip, r10
	str	r10, [r3, #620]
	ldrsb	r3, [r9, #4]
	ldr	r1, [r5, #1464]
	str	r3, [r5, #1532]
.L3501:
	ldrb	r2, [fp, #-48]	@ zero_extendqisi2
	strb	r2, [r5, #947]
	ldrb	r3, [r8, #209]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	strb	r3, [r1, #512]
	ldrb	r3, [r5, #945]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3556
	ldr	r3, [fp, #-48]
	strb	r2, [r5, #2128]
	cmp	r3, #0
	ldr	r3, [r8, #228]
	ldr	r2, [r5, #4068]
	ldreq	r0, [fp, #-48]
	movne	r0, #0
	str	r3, [r5, #2136]
	ldr	r3, [r8, #240]
	str	ip, [r5, #2148]
	str	r2, [r5, #2144]
	str	r3, [r5, #2132]
	movne	r3, #1
	moveq	r3, r0
	strb	r3, [r5, #2152]
.L3544:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3504:
	add	lr, lr, #1
	add	ip, ip, #672
	cmp	lr, r4
	bne	.L3508
.L3509:
	ldr	r3, .L3558
	mov	r0, #0
	ldr	r1, .L3558+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r7
	mov	r1, #1
	bl	ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3551:
	add	r5, r0, #53248
	ldr	r2, [r5, #3984]
	cmp	r2, #2
	beq	.L3557
	ldrb	r2, [r5, #2128]	@ zero_extendqisi2
	strb	r3, [r5, #945]
	mov	r3, #1
	cmp	r2, #2
	str	r3, [fp, #-48]
	bne	.L3519
	b	.L3515
.L3527:
	str	r6, [fp, #-48]
.L3548:
	mov	r3, #0
	strb	r3, [r5, #945]
	b	.L3519
.L3557:
	mov	r3, #1
	str	r3, [fp, #-48]
	b	.L3548
.L3555:
	add	r2, r7, #12288
	ldr	r4, [r0, #200]
	ldr	r0, [r0, #112]
	ldr	r3, [r2, #1280]
	ldr	r2, [r2, #1276]
	mov	r3, r3, asl #4
	str	r3, [r9, #176]
	mov	r2, r2, asl #4
	str	r2, [r9, #172]
	mul	r4, r4, r3
	add	r0, r4, r0
	str	r0, [r9, #116]
	bl	MEM_Phy2Vir
	ldr	r3, [r9, #128]
	ldr	r1, [r9, #96]
	add	r3, r4, r3
	ldr	r2, [r9, #200]
	add	r4, r4, r1
	str	r3, [r9, #132]
	str	r4, [r9, #100]
	add	r4, r4, r2
	str	r4, [r9, #108]
	str	r0, [r9, #44]
	mov	r0, r3
	bl	MEM_Phy2Vir
	str	r0, [r9, #60]
	b	.L3511
.L3556:
	mov	r3, #0
	strb	r3, [r5, #2128]
	ldr	r1, [r8, #228]
	mov	r0, r3
	ldr	r2, [r5, #4068]
	str	r1, [r5, #2136]
	ldr	r1, [r8, #240]
	strb	r3, [r5, #2152]
	str	ip, [r5, #2148]
	str	r1, [r5, #2132]
	str	r2, [r5, #2144]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3552:
	cmp	r3, #0
	beq	.L3495
	b	.L3519
.L3510:
	ldr	ip, [r5, #1464]
	mov	r0, r3
	ldr	r2, .L3558
	strb	r3, [r5, #944]
	ldr	r1, .L3558+8
	strb	r3, [ip, #4]
	ldr	r3, [r2, #68]
	blx	r3
	mov	r1, r4
	mov	r0, r7
	bl	ClearAll
	mvn	r0, #0
	b	.L3544
.L3554:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3548
.L3500:
	ldr	r1, [r5, #1464]
	cmp	r1, #0
	bne	.L3522
	b	.L3519
.L3553:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3548
	b	.L3500
.L3497:
	strb	r1, [r5, #945]
	b	.L3519
.L3559:
	.align	2
.L3558:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC378
	.word	.LC379
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
	add	r9, r0, #24576
	add	r2, r0, #12288
	movw	r3, #2004
	ldr	r1, [r9, #1812]
	mov	r5, #0
	ldrb	r2, [r2, #1325]	@ zero_extendqisi2
	mov	r6, r0
	add	r0, r0, #20480
	add	r7, r6, #16384
	mul	r3, r3, r1
	cmp	r2, #1
	ldr	ip, [r0, #228]
	add	r4, r6, #53248
	moveq	r1, #2
	movne	r1, r5
	str	r1, [fp, #-68]
	add	r1, r6, r3
	add	r1, r1, #22272
	str	r0, [fp, #-72]
	str	ip, [fp, #-64]
	mov	r3, r1
	add	r1, r1, #104
	ldr	ip, [r7, #3468]
	add	r3, r3, #108
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	ldr	r0, [r0, #224]
	ldr	r2, [r7, #116]
	str	r1, [fp, #-88]
	ldr	r1, [r7, #3472]
	str	r3, [r4, #1468]
	ldr	r8, [r9, #2060]
	str	ip, [fp, #-80]
	cmp	r8, r5
	str	r1, [fp, #-84]
	str	r5, [fp, #-52]
	str	r5, [fp, #-48]
	beq	.L3576
	add	r2, r2, #4
	mov	r8, #1
	mov	r3, r8, asl r2
	add	r0, r0, r8
	str	r3, [fp, #-76]
	mov	r1, r3
	bl	__aeabi_uidivmod
	ldr	r3, .L3579
	str	r5, [r7, #3472]
	mov	r0, #2
	str	r5, [r7, #3468]
	ldr	r3, [r3, #68]
	mov	r10, r1
	ldr	r1, .L3579+4
	blx	r3
	ldr	r3, .L3579
	ldr	r2, [fp, #-64]
	mov	r0, #2
	ldr	r1, .L3579+8
	ldr	ip, [r3, #68]
	mov	r3, r10
	blx	ip
	sub	r2, fp, #52
	sub	r1, fp, #48
	mov	r0, r6
	bl	GetMaxPOC
	ldr	r3, [fp, #-64]
	cmp	r3, r10
	beq	.L3568
	mov	r3, r9
	str	r5, [fp, #-60]
	mov	r9, r10
	mov	r5, r8
	mov	r10, r3
	b	.L3569
.L3566:
	mov	r1, r8
	mov	r0, r6
	strb	r8, [r4, #948]
	bl	StorePicInDPB
	ldr	r1, [fp, #-76]
	subs	r3, r0, #0
	add	r0, r9, #1
	bne	.L3577
	ldr	r3, [fp, #-72]
	str	r9, [r3, #224]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-64]
	cmp	r3, r1
	mov	r9, r1
	beq	.L3568
	ldr	r3, [r10, #2060]
	ldr	r2, [fp, #-60]
	cmp	r2, r3
	bhi	.L3568
.L3569:
	mov	r1, #1
	mov	r0, r6
	bl	AllocFrameStore
	mov	r8, #0
	ldr	r1, .L3579+12
	subs	r3, r0, #0
	mov	r0, #2
	bne	.L3578
	str	r3, [fp, #-56]
	ldr	r3, .L3579
	ldr	ip, [r3, #68]
	blx	ip
	ldr	r1, [r4, #1464]
	strb	r5, [r4, #950]
	ldr	r3, [fp, #-68]
	str	r9, [r1, #624]
	ldr	r1, [r4, #1464]
	str	r9, [r4, #1472]
	str	r9, [r1, #524]
	str	r3, [r4, #1480]
	ldr	r1, [r4, #1464]
	ldr	r3, [fp, #-56]
	str	r3, [r1, #520]
	ldr	r1, [r4, #1464]
	str	r5, [r1, #644]
	ldr	r1, [r4, #1464]
	strb	r3, [r1, #4]
	ldr	r1, [r4, #1464]
	strb	r5, [r1]
	ldr	r1, [r4, #1468]
	strb	r5, [r4, #946]
	strb	r3, [r1, #3]
	ldr	r3, [r7, #120]
	cmp	r3, #0
	beq	.L3566
	str	r9, [r7, #3484]
	mov	r0, r6
	bl	DecPOC
	ldr	r3, [r7, #3464]
	ldr	r1, [fp, #-48]
	str	r3, [r4, #1484]
	ldr	r3, [r7, #3460]
	str	r3, [r4, #1488]
	ldr	r3, [r7, #3452]
	cmp	r3, r1
	str	r3, [r4, #1492]
	ldr	r3, [r7, #3456]
	str	r3, [r4, #1496]
	ldrhi	r3, [fp, #-60]
	addhi	r3, r3, #1
	strhi	r3, [fp, #-60]
	b	.L3566
.L3576:
	ldr	r3, .L3579
	mov	r0, r8
	ldr	r1, .L3579+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3568:
	ldr	r3, [fp, #-80]
	mov	r0, #0
	ldrb	r2, [fp, #-88]	@ zero_extendqisi2
	str	r3, [r7, #3468]
	ldr	r3, [fp, #-84]
	str	r3, [r7, #3472]
	ldr	r3, [r4, #1468]
	strb	r2, [r3, #3]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3578:
	ldr	r3, [fp, #-80]
	mov	r0, r8
	ldrb	r2, [fp, #-88]	@ zero_extendqisi2
	ldr	r1, .L3579+20
	str	r3, [r7, #3468]
	ldr	r3, [fp, #-84]
	str	r3, [r7, #3472]
	ldr	r3, [r4, #1468]
	strb	r2, [r3, #3]
	ldr	r3, .L3579
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3577:
	ldr	r2, [fp, #-80]
	mov	r0, r8
	ldrb	lr, [fp, #-88]	@ zero_extendqisi2
	ldr	r1, .L3579+24
	str	r2, [r7, #3468]
	ldr	r2, [fp, #-84]
	str	r2, [r7, #3472]
	movw	r2, #6499
	ldr	ip, [r4, #1468]
	strb	lr, [ip, #3]
	ldr	ip, .L3579
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3580:
	.align	2
.L3579:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC381
	.word	.LC382
	.word	.LC384
	.word	.LC380
	.word	.LC383
	.word	.LC385
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
	ldr	r8, .L3594
	mov	r5, r0
	ldr	r4, [r0, #352]
	mov	r7, r1
	movw	r2, #57352
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
	beq	.L3591
	add	r6, r5, #12288
	mov	r1, #32
	mov	r2, #256
	mov	r3, #136
	str	r1, [r6, #1288]
	str	r2, [r6, #1292]
	str	r3, [r6, #1284]
.L3583:
	ldr	r0, [r7, #668]
	add	r4, r5, #53248
	bl	MEM_Phy2Vir
	ldr	lr, [r6, #1288]
	ldr	r3, [r5]
	mov	r2, #4000
	ldr	r1, [r6, #1292]
	mul	r2, r2, lr
	ldr	r3, [r3, #664]
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
	str	r0, [r4, #3976]
	bne	.L3592
	ldr	r7, [r7, #688]
	mov	r10, r7
	adds	r7, r3, #0
	movne	r7, #1
	cmp	r10, #0
	movne	r7, #0
	cmp	ip, #0
	moveq	ip, #0
	andne	ip, r7, #1
	cmp	ip, #0
	beq	.L3586
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
	ldr	r0, [r4, #3976]
.L3586:
	add	r3, r5, #16384
	movw	r2, #4060
	ldr	ip, [r8, #48]
	str	r1, [r3, #1236]
	mov	r1, #0
	ldr	r3, [r6, #1284]
	mov	r7, r1
	str	lr, [r6, #2052]
	ldr	r6, .L3594
	mul	r2, r2, r3
	blx	ip
	mov	r1, #40
	mov	r2, #17
	mov	r3, #25
	str	r7, [r4, #4000]
	mov	r0, r5
	str	r7, [r4, #4004]
	str	r7, [r4, #4008]
	str	r7, [r4, #4012]
	str	r7, [r4, #4016]
	str	r7, [r4, #4020]
	str	r7, [r4, #4024]
	str	r7, [r4, #4028]
	str	r7, [r4, #3992]
	str	r7, [r4, #2820]
	str	r7, [r4, #2824]
	str	r7, [r4, #2828]
	str	r7, [r4, #2832]
	str	r7, [r4, #2836]
	str	r7, [r4, #2800]
	str	r7, [r4, #2808]
	str	r7, [r4, #4060]
	str	r7, [r4, #2812]
	str	r1, [r4, #2792]
	str	r2, [r4, #2796]
	str	r3, [r4, #2816]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r4, #4056]
	beq	.L3593
	mov	r1, r7
	mov	r0, r5
	bl	ClearAll
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3591:
	ldr	r3, [r7, #752]
	add	r6, r5, #12288
	str	r3, [r6, #1288]
	ldr	r3, [r7, #756]
	str	r3, [r6, #1292]
	ldr	r3, [r7, #744]
	str	r3, [r6, #1284]
	b	.L3583
.L3592:
	ldr	ip, [r8, #68]
	movw	r3, #12738
	ldr	r2, .L3594+4
	mov	r0, #0
	ldr	r1, .L3594+8
	blx	ip
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3593:
	ldr	r3, [r6, #68]
	mov	r0, r7
	ldr	r1, .L3594+12
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3595:
	.align	2
.L3594:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+268
	.word	.LC386
	.word	.LC387
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
	add	r6, r0, #12288
	add	r7, r0, #20480
	mov	r5, r0
	ldr	r0, [r6, #1304]
	mov	r8, #2208
	ldr	ip, [r7, #220]
	add	r4, r5, #53248
	ldr	r3, [r6, #1300]
	mov	r1, #4000
	ldrb	r2, [r4, #944]	@ zero_extendqisi2
	mla	r8, r8, ip, r0
	cmp	r2, #0
	ldr	r2, [r8, #28]
	mla	r3, r1, r2, r3
	str	r3, [fp, #-56]
	beq	.L3597
	ldr	r3, [r4, #1464]
	cmp	r3, #0
	beq	.L3598
	ldrb	r9, [r3, #1]	@ zero_extendqisi2
	cmp	r9, #0
	beq	.L3728
.L3599:
	mov	r3, #0
	strb	r3, [r4, #944]
.L3597:
	sub	r1, fp, #48
	mov	r0, r5
	bl	GetReRangeFlag
	ldr	r3, [fp, #-56]
	ldr	r2, [r6, #2052]
	ldr	r3, [r3, #744]
	cmp	r2, r3
	mov	r10, r0
	beq	.L3729
.L3600:
	ldr	r3, .L3742
	add	r0, r5, #13568
	ldr	r1, [fp, #-56]
	add	r0, r0, #28
	mov	r2, #4000
	mov	ip, r3
	mov	r3, #0
	strb	r3, [r1, #24]
	ldr	r3, [ip, #56]
	blx	r3
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r5]
	beq	.L3602
.L3739:
	ldr	r9, [r3, #776]
	cmp	r9, #0
	bne	.L3603
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L3730
.L3603:
	ldr	r3, [r4, #4016]
	cmp	r10, #0
	add	r3, r3, #1
	str	r3, [r4, #4016]
	bne	.L3614
	ldr	r3, [r5]
	ldr	r3, [r3, #892]
	cmp	r3, #1
	beq	.L3614
	mov	r0, r5
	bl	FlushDPB
	cmp	r0, #0
	bne	.L3731
	mov	r0, r5
	bl	InitDPB
	cmp	r0, #0
	ldrne	r3, .L3742
	strne	r3, [fp, #-60]
	bne	.L3652
.L3619:
	ldr	r3, [r5]
	ldr	r3, [r3, #892]
	cmp	r3, #1
	beq	.L3622
.L3722:
	add	ip, r5, #16384
.L3613:
	ldr	r2, [ip, #1236]
	ldr	r3, [r8, #24]
	cmp	r2, r3
	bne	.L3625
	ldrb	r3, [r8, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3732
.L3625:
	ldr	r3, .L3742
	add	r0, r5, #17408
	mov	r1, r8
	add	r0, r0, #188
	str	ip, [fp, #-64]
	mov	r2, r3
	str	r3, [fp, #-60]
	mov	r3, #0
	strb	r3, [r8, #20]
	ldr	r3, [r2, #56]
	mov	r2, #2208
	blx	r3
	ldr	ip, [fp, #-64]
.L3626:
	ldr	r3, [ip, #3444]
	ldr	r2, [ip, #116]
	cmp	r3, #0
	movne	r3, #0
	strne	r3, [r7, #224]
	strne	r3, [r4, #2136]
	ldr	r3, [r4, #3984]
	cmp	r3, #2
	beq	.L3628
	ldr	r10, [r5]
	ldr	r3, [r10, #12]
	sub	r3, r3, #2
	cmp	r3, #1
	bls	.L3628
	ldr	r3, [r7, #228]
	ldr	r8, [r7, #224]
	cmp	r3, r8
	str	r3, [fp, #-64]
	beq	.L3629
	mov	r1, #1
	add	r2, r2, #4
	mov	r2, r1, asl r2
	add	r0, r8, r1
	str	ip, [fp, #-68]
	mov	r1, r2
	str	r2, [fp, #-72]
	bl	__aeabi_uidivmod
	ldr	r3, [fp, #-64]
	ldr	ip, [fp, #-68]
	cmp	r3, r1
	beq	.L3653
	ldr	r1, [r10, #680]
	cmp	r1, #0
	bne	.L3630
	ldr	r1, [r10, #708]
	ldr	r2, [fp, #-72]
	cmp	r1, #1
	beq	.L3733
.L3631:
	cmp	r3, r8
	bls	.L3632
	ldr	r1, [ip, #1192]
	rsb	r8, r8, r3
	cmp	r1, r8
	bcs	.L3630
	ldrb	r1, [r7, #208]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L3630
	cmp	r3, #0
	mov	r0, r5
	subeq	r3, r2, #1
	subne	r3, r3, #1
	str	r3, [r7, #224]
	str	ip, [fp, #-64]
	bl	FlushDPB
	ldr	ip, [fp, #-64]
	subs	r3, r0, #0
	beq	.L3630
	ldr	r2, [fp, #-60]
	mov	r0, #0
	ldr	r1, .L3742+4
	ldr	ip, [r2, #68]
	movw	r2, #6812
	blx	ip
	mvn	r0, #0
.L3720:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3614:
	ldr	r3, .L3742
	mov	r0, #22
	ldr	r1, .L3742+8
	str	r3, [fp, #-60]
	ldr	r3, [r3, #68]
	blx	r3
	add	r3, r5, #57344
	ldr	r3, [r3]
	cmp	r3, #1
	beq	.L3734
.L3616:
	mov	r0, r5
	bl	InitDPB
	cmp	r0, #0
	bne	.L3652
	cmp	r10, #0
	beq	.L3619
.L3622:
	add	ip, r5, #16384
	add	r3, r5, #55808
	add	r3, r3, #232
	mov	r0, r5
	ldr	r1, [ip, #1192]
	str	ip, [fp, #-60]
	str	r1, [r4, #2796]
	ldr	r2, [ip, #1172]
	ldrb	r9, [r6, #1326]	@ zero_extendqisi2
	add	r2, r2, #1
	ldr	r10, [ip, #1168]
	rsb	r9, r9, #2
	str	r1, [sp]
	mov	r2, r2, asl #4
	add	r10, r10, #1
	mul	r9, r9, r2
	mov	r10, r10, asl #4
	mov	r1, r10
	mov	r2, r9
	bl	H264_ArrangeVahbMem
	ldr	ip, [fp, #-60]
	cmp	r0, #1
	beq	.L3735
.L3724:
	ldr	r3, [r5]
	ldr	r3, [r3, #892]
	cmp	r3, #1
	mvneq	r0, #1
	beq	.L3720
	ldr	r3, .L3742
	mov	r4, #0
	mov	r0, r4
	str	r4, [r6, #1276]
	str	r4, [r6, #1280]
	ldr	r3, [r3, #68]
	ldr	r1, .L3742+12
	blx	r3
	mov	r1, r4
	mov	r0, r5
	bl	ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3733:
	ldr	r1, [r10, #704]
	cmp	r1, #0
	bne	.L3631
	ldr	r1, [r10, #712]
	cmp	r1, #0
	bne	.L3631
	ldr	r1, [r10, #684]
	add	r1, r1, #1024
	cmp	r1, #2048
	bls	.L3631
.L3630:
	mov	r0, r5
	str	ip, [fp, #-64]
	bl	DecGap
	cmp	r0, #0
	bne	.L3637
	ldr	r8, [r7, #228]
	ldr	ip, [fp, #-64]
.L3629:
	ldr	r3, [r7, #240]
	mov	r0, r5
	str	ip, [fp, #-64]
	cmp	r3, #0
	strne	r8, [r7, #224]
	str	r8, [ip, #3484]
	bl	DecPOC
	mov	r1, #0
	mov	r0, r5
	bl	AllocFrameStore
	ldr	ip, [fp, #-64]
	cmp	r0, #0
	bne	.L3736
	strb	r0, [r4, #951]
	mov	r1, #1
	strb	r1, [r4, #944]
	mvn	lr, #0
	ldrb	r3, [r7, #211]	@ zero_extendqisi2
	str	ip, [fp, #-64]
	strb	r3, [r4, #950]
	ldr	r2, [r7, #228]
	str	r2, [r4, #1472]
	ldr	r3, [r7, #240]
	str	r2, [fp, #-68]
	str	r3, [r4, #1480]
	ldr	r8, [ip, #3464]
	str	r3, [fp, #-72]
	str	r8, [r4, #1484]
	ldr	r8, [ip, #3460]
	str	r8, [r4, #1488]
	ldr	r8, [ip, #3452]
	str	r8, [r4, #1492]
	ldr	r8, [ip, #3456]
	strb	r0, [r4, #946]
	strb	r0, [r4, #949]
	str	r8, [r4, #1496]
	strb	r0, [r4, #948]
	str	lr, [r4, #1528]
	ldrb	r10, [r7, #209]	@ zero_extendqisi2
	cmp	r10, #0
	strneb	r1, [r7, #209]
	ldrb	r0, [r6, #1326]	@ zero_extendqisi2
	ldr	lr, [ip, #1172]
	movne	r10, r1
	rsb	r8, r0, #2
	movne	r1, #2
	mla	r8, lr, r8, r8
	mov	r0, r8
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-64]
	str	r8, [r4, #1520]
	cmp	r10, #0
	ldr	r2, [fp, #-68]
	movne	r10, #0
	ldr	r3, [fp, #-72]
	str	r0, [r4, #1516]
	ldr	lr, [ip, #1168]
	add	r1, lr, #1
	str	r1, [r4, #1512]
	mul	r1, r1, r0
	str	r1, [r4, #1524]
	ldrb	r1, [ip, #1212]	@ zero_extendqisi2
	strb	r1, [r4, #952]
	movne	r1, r10
	bne	.L3641
	ldrb	r1, [r6, #1327]	@ zero_extendqisi2
	cmp	r1, #0
	movne	r10, #1
	movne	r1, r10
.L3641:
	cmp	r3, #0
	add	r3, r5, #24576
	strb	r1, [r4, #953]
	movw	r8, #2004
	str	r10, [r4, #3972]
	mov	r0, #29
	strne	r2, [r7, #224]
	add	r2, r5, #54016
	ldr	r3, [r3, #1812]
	movw	r7, #54224
	mov	lr, r2
	str	ip, [fp, #-64]
	ldr	ip, [fp, #-56]
	add	r2, r2, #208
	mla	r3, r8, r3, r5
	ldr	r1, .L3742+16
	ldr	r10, .L3742
	add	r3, r3, #22272
	add	r3, r3, #108
	str	r3, [r4, #1468]
	ldr	r3, [r5]
	ldrd	r8, [r3, #48]
	strd	r8, [r2, #-8]
	ldrd	r8, [r3, #64]
	strd	r8, [r5, r7]
	ldrd	r8, [r3, #80]
	strd	r8, [lr, #216]
	mvn	r8, #0
	ldr	lr, [r3, #96]
	mvn	r9, #0
	str	lr, [r4, #1028]
	ldr	lr, [r3, #100]
	str	lr, [r4, #1032]
	ldr	lr, [r3, #104]
	str	lr, [r4, #1036]
	ldr	lr, [r3, #120]
	str	lr, [r4, #1040]
	ldr	lr, [ip, #752]
	str	lr, [r4, #1192]
	ldr	lr, [ip, #756]
	str	lr, [r4, #1196]
	strd	r8, [r3, #48]
	ldr	r9, [fp, #-60]
	ldrd	r2, [r2, #-8]
	ldr	r8, [r9, #68]
	blx	r8
	ldrd	r2, [r5, r7]
	ldr	r1, .L3742+20
	mov	r0, #29
	ldr	r8, [r9, #68]
	blx	r8
	ldr	ip, [fp, #-64]
	add	r0, r5, #54016
	add	r0, r0, #192
	ldr	r3, [ip, #1168]
	add	r3, r3, #1
	mov	r3, r3, asl #4
	str	r3, [r4, #1124]
	ldrb	r2, [r6, #1326]	@ zero_extendqisi2
	ldr	r1, [ip, #1172]
	rsb	r3, r2, #2
	mla	r3, r1, r3, r3
	mov	r3, r3, asl #4
	str	r3, [r4, #1128]
	ldr	r2, [r6, #2056]
	ldr	r3, [r6, #1364]
	subs	r2, r2, #1
	ldr	r1, [r6, #1360]
	movne	r2, #1
	and	r1, r1, #7
	mov	r3, r3, asl #5
	mov	r2, r2, asl #2
	orr	r2, r2, r1, asl #14
	uxtb	r3, r3
	orr	r3, r2, r3
	str	r3, [r4, #1120]
	bl	SetAspectRatio
	ldrb	r3, [r4, #947]	@ zero_extendqisi2
	ldr	r2, [r4, #1464]
	adds	r3, r3, #0
	ldr	r1, [r4, #4016]
	movne	r3, #1
	cmp	r2, #0
	str	r3, [r4, #1108]
	str	r1, [r4, #1164]
	beq	.L3737
	ldr	r3, [r4, #1120]
	ldr	r1, [fp, #-56]
	str	r3, [r2, #168]
	ldr	r3, [r4, #1464]
	ldr	r2, [r4, #1108]
	str	r2, [r3, #156]
	ldr	r3, [r4, #1464]
	ldr	r2, [r4, #1024]
	str	r2, [r3, #72]
	ldr	r3, [r4, #1464]
	ldr	r2, [r4, #1020]
	str	r2, [r3, #68]
	ldr	r3, [r4, #1464]
	ldr	r2, [r4, #1124]
	str	r2, [r3, #172]
	ldr	r3, [r4, #1464]
	ldr	r2, [r4, #1128]
	str	r2, [r3, #176]
	ldr	r3, [r1, #3992]
	ldr	r2, [r4, #1464]
	str	r3, [r2, #188]
	str	r3, [r4, #1140]
	ldr	r2, [r4, #1464]
	ldr	r3, [r1, #3996]
	str	r3, [r2, #192]
	str	r3, [r4, #1144]
	ldr	r2, [r4, #1464]
	ldr	r3, [r1, #3984]
	str	r3, [r2, #180]
	str	r3, [r4, #1132]
	ldr	r2, [r4, #1464]
	ldr	r3, [r1, #3988]
	str	r3, [r2, #184]
	str	r3, [r4, #1136]
	ldr	r3, [r4, #1464]
	ldr	r0, [r4, #4056]
	ldr	r1, [r3, #620]
	bl	FSP_GetDispPhyFs
	cmp	r0, #0
	beq	.L3644
	ldr	r3, [r4, #1108]
	mov	r1, #0
	ldr	r2, [r0, #4]
	subs	r3, r3, r1
	ldr	r0, [r4, #4056]
	movne	r3, #1
	bl	FSP_SetStoreType
.L3644:
	add	r2, r5, #54272
	add	lr, r5, #56064
	ldr	r6, [r4, #2820]
	add	lr, lr, #8
	mov	r0, r2
	add	r2, r2, #144
	add	r0, r0, #160
	mov	r7, #0
.L3648:
	ldr	r3, [r2, #4]!
	cmp	r3, #0
	beq	.L3738
.L3645:
	cmp	r2, r0
	bne	.L3648
	cmp	r6, #0
	beq	.L3650
	ble	.L3650
	add	r7, r6, #14016
	mov	r8, #0
	add	r7, r7, #2
	add	r7, r5, r7, lsl #2
.L3651:
	ldr	r1, [r7, #-4]!
	sub	r6, r6, #1
	ldr	r0, [r4, #4056]
	bl	FreeUsdByDec
	cmp	r6, #0
	str	r8, [r7]
	bne	.L3651
.L3650:
	mov	r0, r5
	mov	r5, #0
	str	r5, [r4, #2820]
	str	r5, [r4, #1500]
	bl	CalcPicNum
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3729:
	ldr	r3, [fp, #-56]
	ldrb	r3, [r3, #24]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3600
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r5]
	bne	.L3739
.L3602:
	ldr	r3, [r3, #892]
	cmp	r3, #1
	bne	.L3722
	b	.L3603
.L3738:
	cmp	r6, #0
	beq	.L3645
	ldr	r1, [r4, #2824]
	subs	r6, r6, #1
	str	r1, [r2]
	beq	.L3646
	mov	r1, lr
.L3647:
	add	r3, r3, #1
	ldr	ip, [r1, #4]!
	cmp	r3, r6
	str	ip, [r1, #-4]
	bne	.L3647
.L3646:
	add	r3, r6, #14016
	add	r3, r3, #2
	str	r7, [r5, r3, asl #2]
	str	r6, [r4, #2820]
	b	.L3645
.L3628:
	ldr	r8, [r7, #228]
	b	.L3629
.L3732:
	ldr	r3, .L3742
	str	r3, [fp, #-60]
	b	.L3626
.L3730:
	ldr	r3, [r4, #4016]
	cmp	r10, #0
	add	r3, r3, #1
	str	r3, [r4, #4016]
	beq	.L3604
	mov	r0, r5
	bl	FlushDPB
	subs	r3, r0, #0
	beq	.L3604
	ldr	ip, .L3742
	mov	r0, r9
	movw	r2, #6596
	ldr	r1, .L3742+4
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L3720
.L3653:
	mov	r8, r3
	b	.L3629
.L3604:
	mov	r0, r5
	bl	InitDPB
	subs	r3, r0, #0
	bne	.L3740
	add	ip, r5, #16384
	ldrb	r2, [r6, #1326]	@ zero_extendqisi2
	cmp	r10, #0
	ldr	r3, [ip, #1172]
	rsb	r10, r2, #2
	ldr	r9, [ip, #1168]
	add	r3, r3, #1
	add	r9, r9, #1
	mov	r3, r3, asl #4
	mov	r9, r9, asl #4
	mul	r10, r10, r3
	beq	.L3607
	ldr	r3, [r6, #1276]
	cmp	r3, #0
	bne	.L3607
	ldr	r3, [ip, #1192]
	str	r3, [r4, #2796]
	str	r3, [fp, #-60]
	ldr	r3, [r5]
	ldr	r1, [r3, #736]
	cmp	r9, r1
	bhi	.L3608
	ldr	r2, [r3, #740]
	cmp	r10, r2
	str	r2, [fp, #-64]
	bhi	.L3608
	add	lr, r5, #24576
	ldr	r2, [r3, #760]
	ldr	lr, [lr, #2056]
	cmp	r2, lr
	bcc	.L3741
	ldr	r0, [r3, #764]
	cmp	r0, #0
	bne	.L3611
	mov	r2, #1
	str	r2, [r4, #2796]
	ldr	r1, [r3, #736]
	ldr	r3, [r3, #740]
	str	r2, [fp, #-60]
	str	r3, [fp, #-64]
.L3611:
	ldr	r2, [fp, #-60]
	add	r3, r5, #55808
	add	r3, r3, #232
	mov	r0, r5
	str	ip, [fp, #-68]
	str	r2, [sp]
	ldr	r2, [fp, #-64]
	bl	H264_ArrangeVahbMem
	ldr	ip, [fp, #-68]
	cmp	r0, #1
	bne	.L3724
.L3607:
	mov	r9, r9, lsr #4
	mov	r10, r10, lsr #4
	str	r9, [r6, #1276]
	str	r10, [r6, #1280]
	b	.L3613
.L3728:
	ldr	r1, [r3, #620]
	mov	r2, #1
	ldr	r0, [r4, #4056]
	bl	FSP_ClearLogicFs
	ldr	r3, [r4, #1464]
	strb	r9, [r3, #1]
	ldr	r3, [r4, #1464]
	strb	r9, [r3, #4]
	b	.L3599
.L3735:
	mov	r10, r10, lsr #4
	mov	r9, r9, lsr #4
	str	r10, [r6, #1276]
	str	r9, [r6, #1280]
	b	.L3613
.L3734:
	mov	r0, r5
	bl	GetBackPicFromVOQueue
	b	.L3616
.L3632:
	rsb	r1, r8, r3
	ldr	r0, [ip, #1192]
	add	r1, r1, r2
	cmp	r0, r1
	bcs	.L3630
	ldrb	r1, [r7, #208]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L3630
	cmp	r3, #0
	mov	r0, r5
	subeq	r3, r2, #1
	subne	r3, r3, #1
	str	r3, [r7, #224]
	str	ip, [fp, #-64]
	bl	FlushDPB
	ldr	ip, [fp, #-64]
	subs	r3, r0, #0
	beq	.L3630
	ldr	r2, [fp, #-60]
	mov	r0, #0
	ldr	r1, .L3742+4
	ldr	ip, [r2, #68]
	movw	r2, #6830
	blx	ip
	mvn	r0, #0
	b	.L3720
.L3598:
	ldr	ip, .L3742
	movw	r3, #6562
	ldr	r2, .L3742+24
	mov	r0, #22
	ldr	r1, .L3742+28
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3599
.L3736:
	ldr	r3, [fp, #-60]
	movw	r2, #6867
	ldr	r1, .L3742+32
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3720
.L3652:
	ldr	r3, [fp, #-60]
	movw	r2, #6731
	ldr	r1, .L3742+36
	ldr	ip, [r3, #68]
	mov	r3, r0
	mov	r0, #0
	blx	ip
	mvn	r0, #0
	b	.L3720
.L3737:
	ldr	ip, [r10, #68]
	movw	r3, #6954
	ldr	r2, .L3742+24
	mov	r0, #22
	ldr	r1, .L3742+40
	blx	ip
	mvn	r0, #0
	b	.L3720
.L3731:
	ldr	ip, .L3742
	mov	r3, r0
	movw	r2, #6722
	mov	r0, r10
	ldr	r1, .L3742+4
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L3720
.L3637:
	ldr	r3, [fp, #-60]
	movw	r2, #6843
	ldr	r1, .L3742+44
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3720
.L3740:
	ldr	ip, .L3742
	movw	r2, #6605
	ldr	r1, .L3742+36
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L3720
.L3608:
	ldr	r3, .L3742
	mov	r0, #0
	ldr	r1, .L3742+48
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3720
.L3741:
	ldr	r3, .L3742
	ldr	r1, .L3742+52
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3720
.L3743:
	.align	2
.L3742:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC354
	.word	.LC393
	.word	.LC392
	.word	.LC396
	.word	.LC397
	.word	.LANCHOR1+284
	.word	.LC388
	.word	.LC395
	.word	.LC389
	.word	.LC398
	.word	.LC394
	.word	.LC390
	.word	.LC391
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
	add	r7, r0, #53248
	ldr	r3, .L3797
	mov	r5, r0
	ldr	r2, [r7, #4000]
	mov	r0, #22
	ldr	r1, .L3797+4
	add	r6, r5, #20480
	sub	r2, r2, #1
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r5, #368]
	mov	r0, r5
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r6, #211]
	ldr	r3, [r3, #76]
	str	r3, [r6, #240]
	bl	ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L3791
	mov	r0, r5
	bl	ProcessSliceHeaderSecondPart
	cmp	r0, #0
	bne	.L3791
	ldr	r2, [r7, #3980]
	cmp	r2, #0
	moveq	r3, r2
	beq	.L3751
	add	r10, r5, #55296
	mov	lr, r0
	add	r10, r10, #128
	mov	r3, r0
	mov	r8, lr
.L3759:
	ldr	ip, [r10, #4]!
	ldr	r4, [ip, #8]
	ldr	r1, [ip, #36]
	cmp	r4, #0
	ldrne	r9, [ip, #12]
	moveq	r9, r4
	cmp	r1, #0
	moveq	ip, r1
	ldrne	ip, [ip, #40]
	cmp	r0, #0
	addeq	r3, r9, r3
	addeq	r3, r3, ip
	beq	.L3757
	cmp	r4, r0
	add	r3, ip, r3
	addhi	r0, r0, lr
	rsbhi	r0, r0, r4
	movls	r0, r9
	addhi	r0, r0, r9
	add	r3, r3, r0
.L3757:
	cmp	r1, #0
	add	r8, r8, #1
	movne	r0, r1
	movne	lr, ip
	moveq	r0, r4
	moveq	lr, r9
	cmp	r8, r2
	bne	.L3759
.L3751:
	ldr	ip, [r7, #1516]
	movw	r0, #1620
	ldr	r1, [r7, #1512]
	str	r3, [r7, #4040]
	mul	r1, r1, ip
	cmp	r1, r0
	movgt	r1, r1, asl #8
	ble	.L3792
.L3761:
	add	r4, r5, #12288
	ldr	r0, [r4, #1284]
	cmp	r0, r2
	bls	.L3762
	cmp	r1, r3
	bcc	.L3762
	ldrb	r3, [r6, #213]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3793
.L3765:
	mov	r0, r5
	bl	PicTypeStatistic
	mov	r0, r5
	bl	DecList
	cmp	r0, #0
	bne	.L3794
	mov	r0, r5
	bl	IsRefListWrong
	subs	r4, r0, #0
	bne	.L3795
	mov	r0, r5
	bl	CalcStreamBits
	mov	r0, r5
	bl	WriteSliceMsg
	mov	r0, r5
	bl	ExitSlice
	ldr	r3, [r7, #4036]
	mov	r0, r4
	mov	r2, #1
	add	r3, r3, r2
	str	r2, [r7, #2800]
	str	r3, [r7, #4036]
.L3748:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3792:
	mov	r0, r1, asl #9
	sub	r1, r0, r1, asl #7
	cmp	r1, #16384
	movlt	r1, #16384
	b	.L3761
.L3793:
	ldr	r3, .L3797
	mov	r0, #2
	ldr	r1, .L3797+8
	ldr	r3, [r3, #68]
	blx	r3
	ldrb	r3, [r6, #211]	@ zero_extendqisi2
	ldrb	r1, [r7, #947]	@ zero_extendqisi2
	mov	r2, #0
	cmp	r3, #5
	ldr	r3, [r7, #4020]
	str	r2, [r7, #4036]
	mov	r0, r5
	streq	r3, [r7, #4032]
	cmp	r1, r2
	add	r3, r3, #1
	str	r3, [r7, #4020]
	ldreq	r3, [r7, #4024]
	ldrne	r3, [r7, #4028]
	addeq	r3, r3, #1
	addne	r3, r3, #1
	streq	r3, [r7, #4024]
	strne	r3, [r7, #4028]
	bl	InitPic
	cmn	r0, #2
	beq	.L3748
	cmp	r0, #0
	bne	.L3796
	mov	r0, r5
	bl	WritePicMsg
	b	.L3765
.L3791:
	ldr	r3, .L3797
	mov	r0, #1
	ldr	r1, .L3797+12
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, .L3797+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3789
	sub	r2, fp, #44
	mvn	r1, #0
	ldr	r0, [r7, #4056]
	mov	r3, #4
	str	r1, [r2, #-8]!
	mov	r1, #100
	blx	ip
.L3789:
	mov	r0, r5
	bl	ClearCurrSlice
	mvn	r0, #0
	b	.L3748
.L3762:
	str	r1, [sp]
	mov	r0, #0
	ldr	r1, .L3797
	ldr	ip, [r1, #68]
	ldr	r1, .L3797+20
	blx	ip
	ldr	r2, [r4, #1284]
	ldr	r3, [r7, #3980]
	cmp	r3, r2
	bcc	.L3764
	ldr	r1, .L3797+16
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L3764
	str	r3, [fp, #-52]
	mov	r1, #108
	str	r2, [fp, #-48]
	mov	r3, #8
	sub	r2, fp, #52
	ldr	r0, [r7, #4056]
	blx	ip
.L3764:
	ldr	r3, .L3797
	mov	r0, #1
	ldr	r1, .L3797+24
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	bl	H264_ClearCurrPic
	mvn	r0, #0
	b	.L3748
.L3794:
	ldr	r3, .L3797
	mov	r2, r0
	ldr	r1, .L3797+28
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3748
.L3795:
	ldr	r3, .L3797
	mov	r0, #1
	ldr	r1, .L3797+32
	ldr	r3, [r3, #68]
	blx	r3
	b	.L3789
.L3796:
	ldr	r3, .L3797
	mov	r0, #1
	ldr	r1, .L3797+36
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3748
.L3798:
	.align	2
.L3797:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC344
	.word	.LC401
	.word	.LC345
	.word	g_event_report
	.word	.LC399
	.word	.LC400
	.word	.LC403
	.word	.LC404
	.word	.LC402
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
	add	r4, r0, #53248
	mov	r6, r0
	ldr	r3, [r1, #68]
	cmp	r3, #0
	movne	ip, r1
	movne	r2, #0
	ldrne	r3, [r4, #4040]
	beq	.L3804
.L3803:
	ldr	lr, [ip, #12]
	add	r2, r2, #1
	add	ip, ip, #28
	add	r3, r3, lr
	str	r3, [r4, #4040]
	ldr	lr, [r1, #68]
	cmp	lr, r2
	bhi	.L3803
.L3804:
	add	r7, r6, #13504
	mov	r10, #0
	add	r7, r7, #24
	strb	r10, [r1]
	mov	r1, #32
	ldr	r8, .L3967
	mov	r0, r7
	bl	BsGet
	ldr	r3, [r6, #368]
	ldr	r1, .L3967+4
	and	lr, r0, #31
	strb	lr, [r3, #2]
	ldr	r3, [r6, #368]
	ubfx	ip, r0, #5, #2
	ubfx	r2, r0, #7, #1
	mov	r9, r0
	mov	r0, #22
	str	ip, [r3, #76]
	ldr	r3, [r6, #368]
	str	r2, [r3, #80]
	ldr	r2, [r6, #368]
	ldr	r3, [r2, #64]
	add	r3, r3, #32
	str	r3, [r2, #64]
	str	r10, [r4, #3996]
	ldr	r3, [r6, #368]
	ldr	ip, [r8, #68]
	ldr	r2, [r4, #4000]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	blx	ip
	ldr	r1, [r4, #4000]
	movw	r3, #371
	cmp	r1, r3
	beq	.L3961
.L3802:
	add	r1, r1, #1
	str	r1, [r4, #4000]
	ldr	r3, [r6, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	cmp	r2, #20
	beq	.L3962
.L3805:
	sub	r3, r2, #1
	cmp	r3, #29
	ldrls	pc, [pc, r3, asl #2]
	b	.L3813
.L3815:
	.word	.L3814
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3814
	.word	.L3816
	.word	.L3817
	.word	.L3818
	.word	.L3819
	.word	.L3820
	.word	.L3821
	.word	.L3822
	.word	.L3823
	.word	.L3813
	.word	.L3817
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3824
	.word	.L3814
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3813
	.word	.L3825
.L3825:
	ldr	r3, [r8, #68]
	mov	r2, #30
	ldr	r1, .L3967+8
	mov	r0, #22
	blx	r3
	cmp	r5, #0
	bne	.L3840
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3840
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	str	r5, [r6, #368]
.L3840:
	mov	r1, #32
	mov	r0, r7
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r7
	bl	BsGet
	ldr	r3, .L3967+12
	rev	r5, r5
	ldr	r2, [r3, #440]
	ldr	r3, [r3, #436]
	rev	r0, r0
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L3944
	ldr	r5, .L3967+16
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3841
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #4056]
	blx	ip
.L3841:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, r7
	beq	.L3944
.L3956:
	ldr	ip, [r5]
	cmp	ip, r7
	beq	.L3942
	ldr	r0, [r4, #4056]
	mov	r3, r7
	mov	r2, r7
	mov	r1, #113
	blx	ip
.L3942:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3814:
	ldr	ip, [r8, #68]
	mov	r0, #22
	ldr	r3, [r4, #4012]
	add	r5, r6, #20480
	ldr	r1, .L3967+20
	blx	ip
	ldr	r3, [r6, #368]
	mov	r0, r6
	ldr	r7, .L3967
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r5, #211]
	ldr	r2, [r3, #76]
	str	r2, [r5, #240]
	ldrb	r3, [r3, #5]	@ zero_extendqisi2
	strb	r3, [r5, #216]
	bl	SliceCheck
	subs	r8, r0, #0
	bne	.L3963
	mov	r0, r6
	bl	H264_DecSlice
	cmn	r0, #2
	beq	.L3940
	cmp	r0, #0
	beq	.L3828
	ldr	r3, .L3967+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3942
	ldr	r0, [r4, #4056]
	mov	r3, r8
	mov	r2, r8
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L3940
.L3816:
	ldr	r1, .L3967+24
	mov	r2, #6
	ldr	r3, [r8, #68]
	mov	r0, #22
	blx	r3
	mov	r0, r6
	bl	H264_DecSEI
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3836
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3836:
	cmp	r5, #0
	bne	.L3964
.L3944:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3817:
	ldr	r1, .L3967+28
	mov	r0, #22
	ldr	ip, [r8, #68]
	ldr	r3, [r4, #4004]
	blx	ip
	mov	r0, r6
	bl	H264_DecSPS
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3834
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3834:
	cmp	r5, #0
	bne	.L3965
	ldr	r3, [r4, #4004]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #4004]
	b	.L3940
.L3821:
	ldr	r3, [r8, #68]
	mov	r2, #11
	ldr	r1, .L3967+32
	mov	r0, #22
	blx	r3
	mov	r1, #32
	mov	r0, r7
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r7
	bl	BsGet
	movw	r2, #20036
	movw	r3, #20553
	movt	r2, 17221
	movt	r3, 18515
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L3949
	ldr	r5, .L3967+16
	add	r3, r6, #12288
	mov	r2, #1
	strb	r2, [r3, #1272]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3839
	mov	r3, #0
	mov	r1, #117
	mov	r2, r3
	ldr	r0, [r4, #4056]
	blx	ip
.L3839:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, r7
	beq	.L3949
	b	.L3956
.L3822:
	ldr	r3, [r8, #68]
	mov	r2, #12
	ldr	r1, .L3967+36
.L3951:
	mov	r0, #22
	blx	r3
.L3949:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3944
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r6, #368]
.L3940:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3823:
	ldr	r3, [r8, #68]
	mov	r2, #13
	ldr	r1, .L3967+40
	b	.L3951
.L3824:
	ldr	r3, [r8, #68]
	mov	r2, #19
	ldr	r1, .L3967+44
	b	.L3951
.L3819:
	ldr	r3, [r8, #68]
	mov	r2, #9
	ldr	r1, .L3967+48
	b	.L3951
.L3820:
	ldr	r3, [r8, #68]
	mov	r2, #10
	ldr	r1, .L3967+52
	b	.L3951
.L3818:
	ldr	r1, .L3967+56
	mov	r2, #8
	ldr	ip, [r8, #68]
	mov	r0, #22
	ldr	r3, [r4, #4008]
	blx	ip
	mov	r0, r6
	bl	H264_DecPPS
	ldr	r1, [r6, #368]
	cmp	r1, #0
	mov	r5, r0
	beq	.L3832
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3832:
	cmp	r5, #0
	bne	.L3966
	ldr	r3, [r4, #4008]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #4008]
	b	.L3940
.L3813:
	ldr	ip, [r8, #68]
	mov	r3, r9
	ldr	r1, .L3967+60
	mov	r0, #22
	blx	ip
	cmp	r9, #256
	ldr	r5, .L3967+16
	beq	.L3842
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3843
	mov	r3, #0
	mov	r1, #102
	mov	r2, r3
	ldr	r0, [r4, #4056]
	blx	ip
.L3842:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3844
.L3845:
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r6, #368]
.L3844:
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3944
.L3947:
	mov	r3, #0
	ldr	r0, [r4, #4056]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L3944
.L3962:
	mov	r1, #24
	mov	r0, r7
	bl	BsGet
	ldr	r3, [r6, #368]
	ubfx	r2, r0, #23, #1
	strb	r2, [r3, #3]
	ldr	r1, [r6, #368]
	mov	r9, r0
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3806
	ldrb	r2, [r4, #2172]	@ zero_extendqisi2
	ldrb	r3, [r4, #2170]	@ zero_extendqisi2
.L3807:
	cmp	r2, #0
	bne	.L3808
	cmp	r3, #1
	beq	.L3809
.L3808:
	ldr	r1, .L3967+64
	mov	r0, #1
	ldr	ip, [r8, #68]
	blx	ip
	ldr	r1, [r6, #368]
	cmp	r1, #0
	beq	.L3942
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	mvn	r0, #0
	str	r3, [r6, #368]
	b	.L3940
.L3961:
	ldr	r1, .L3967+68
	mov	r0, #22
	ldr	r3, [r8, #68]
	blx	r3
	ldr	r1, [r4, #4000]
	b	.L3802
.L3806:
	ubfx	r3, r0, #22, #1
	ubfx	r2, r0, #16, #6
	ubfx	ip, r9, #4, #1
	strb	r3, [r4, #2168]
	strb	r2, [r4, #2169]
	ubfx	r3, r0, #15, #1
	ubfx	r2, r9, #8, #4
	ubfx	r0, r0, #12, #3
	strb	ip, [r4, #2174]
	ubfx	ip, r9, #2, #1
	strb	r0, [r4, #2171]
	ubfx	r0, r9, #5, #3
	strb	r3, [r4, #2170]
	strb	r0, [r4, #2173]
	ubfx	r0, r9, #3, #1
	strb	r2, [r4, #2172]
	strb	r0, [r4, #2175]
	and	r0, r9, #3
	strb	ip, [r4, #2176]
	strb	r0, [r4, #2177]
	b	.L3807
.L3963:
	ldr	r3, [r7, #68]
	mov	r0, #1
	ldr	r1, .L3967+72
	blx	r3
	mov	r0, r6
	bl	ClearCurrSlice
	ldr	r3, .L3967+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3942
.L3943:
	mov	r3, #0
	ldr	r0, [r4, #4056]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L3940
.L3966:
	ldr	r5, .L3967+16
	mov	r0, #1
	ldr	r3, [r8, #68]
	ldr	r1, .L3967+76
	blx	r3
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3942
.L3959:
	sub	r2, fp, #44
	mvn	r6, #0
	mov	r3, #4
	mov	r1, #100
	str	r6, [r2, #-4]!
	ldr	r0, [r4, #4056]
	blx	ip
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L3943
	b	.L3942
.L3965:
	ldr	r5, .L3967+16
	mov	r0, #1
	ldr	r3, [r8, #68]
	ldr	r1, .L3967+80
	blx	r3
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L3959
	b	.L3942
.L3964:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L3967+84
	blx	r3
	ldr	r3, .L3967+16
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3947
	b	.L3944
.L3843:
	ldr	r1, [r6, #368]
	cmp	r1, #0
	bne	.L3845
	b	.L3944
.L3809:
	ldrb	r3, [r4, #2168]	@ zero_extendqisi2
	mov	r0, #22
	cmp	r3, #1
	moveq	r3, #5
	streqb	r3, [r1, #2]
	ldrb	lr, [r4, #2173]	@ zero_extendqisi2
	ldreqb	r2, [r4, #2172]	@ zero_extendqisi2
	ldrb	r3, [r4, #2171]	@ zero_extendqisi2
	ldr	ip, [r8, #68]
	ldr	r1, .L3967+88
	str	lr, [sp]
	blx	ip
	ldr	r3, [r8, #68]
	mov	r2, r9
	ldr	r1, .L3967+92
	mov	r0, #22
	blx	r3
	ldr	r2, [r6, #368]
	ldr	r3, [r2, #64]
	add	r3, r3, #24
	str	r3, [r2, #64]
	ldr	r3, [r6, #368]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	b	.L3805
.L3828:
	ldrb	r3, [r5, #216]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3944
	ldr	r5, .L3967+16
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L3830
	mov	r3, r0
	mov	r2, r0
	mov	r1, #112
	ldr	r0, [r4, #4056]
	blx	ip
.L3830:
	mov	r7, #0
	mov	r0, r6
	str	r7, [r6, #364]
	bl	DecVDM
	cmp	r0, #0
	bne	.L3956
	add	r6, r6, #12288
	mov	r3, #1
	strb	r3, [r6, #1273]
	b	.L3940
.L3968:
	.align	2
.L3967:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC405
	.word	.LC423
	.word	.LANCHOR0
	.word	g_event_report
	.word	.LC410
	.word	.LC415
	.word	.LC413
	.word	.LC420
	.word	.LC419
	.word	.LC421
	.word	.LC422
	.word	.LC417
	.word	.LC418
	.word	.LC412
	.word	.LC424
	.word	.LC407
	.word	.LC406
	.word	.LC411
	.word	.LC176
	.word	.LC414
	.word	.LC416
	.word	.LC408
	.word	.LC409
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	cmp	r1, #0
	mov	ip, #0
	mov	r5, r0
	str	ip, [fp, #-40]
	str	ip, [fp, #-44]
	str	ip, [fp, #-48]
	beq	.L4024
	ldr	r2, [r0]
	add	r4, r0, #53248
	ldr	lr, [r4, #2804]
	ldr	r3, [r2, #8]
	cmp	lr, #1
	mov	r3, r3, asl #16
	and	r3, r3, #458752
	str	r3, [r4, #4044]
	ldr	r3, [r2]
	str	r3, [r4, #4048]
	ldr	r3, [r2, #4]
	str	ip, [r4, #3996]
	str	r3, [r4, #4052]
	beq	.L3972
	ldr	r3, [r2, #852]
	cmp	r3, #1
	beq	.L4025
.L3973:
	mov	r0, r5
	bl	ReceivePacket
	ldr	r3, [r5, #368]
	str	r3, [r4, #4092]
.L3975:
	cmp	r0, #0
	mov	r3, #0
	str	r3, [r4, #2804]
	beq	.L3976
.L3992:
	mov	r0, #0
.L4021:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4025:
	ldr	r3, [r2, #892]
	cmp	r3, #1
	bne	.L3973
	ldr	r3, [r4, #4088]
	cmp	r3, #1
	bne	.L3973
	ldr	r3, [r4, #4092]
	str	r3, [r0, #368]
	str	ip, [r4, #4088]
	str	ip, [r4, #2804]
	b	.L3974
.L3976:
	ldr	r3, [r5, #368]
.L3974:
	mov	r2, #0
	add	r6, r5, #13504
	str	r2, [r3, #64]
	add	r6, r6, #24
	ldr	r3, [r5, #368]
	mov	r0, r6
	ldr	r2, [r3, #12]
	ldr	r1, [r3, #8]
	bl	BsInit
	mov	r1, #32
	mov	r0, r6
	bl	BsGet
	ldr	r1, [r5, #368]
	uxtb	r3, r0
	and	r2, r3, #31
	and	r0, r3, #27
	strb	r2, [r1, #2]
	sub	ip, r2, #20
	ldr	r1, [r5, #368]
	clz	ip, ip
	cmp	r2, #20
	cmpne	r0, #1
	ubfx	r3, r3, #5, #2
	mov	ip, ip, lsr #5
	str	r3, [r1, #76]
	beq	.L4026
.L3977:
	mov	r0, r5
	bl	FindTrailZeros
	subs	r4, r0, #0
	bne	.L4027
	ldr	r3, [r5, #368]
	mov	r0, r6
	str	r4, [r3, #64]
	ldr	r3, [r5, #368]
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #12]
	bl	BsInit
	mov	r1, r4
	mov	r0, r5
	bl	H264_DecOneNal
	cmn	r0, #2
	beq	.L4021
	cmp	r0, #0
	beq	.L3992
	ldr	r3, .L4032
	mov	r0, #22
	ldr	r1, .L4032+4
	ldr	r3, [r3, #68]
	blx	r3
.L4023:
	mvn	r0, #0
	b	.L4021
.L3972:
	ldr	r0, [r0, #368]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	b	.L3975
.L4026:
	cmp	ip, #0
	bne	.L4028
.L3978:
	sub	r2, fp, #32
	sub	r1, fp, #36
	mov	r0, r5
	bl	InquireSliceProperty
	cmp	r0, #0
	beq	.L4029
	ldr	r1, [r5, #368]
	cmp	r1, #0
	beq	.L3990
	ldr	r0, [r4, #4056]
	bl	ReleaseNAL
	mov	r3, #0
	str	r3, [r5, #368]
.L3990:
	ldr	r3, .L4032+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4023
	mov	r3, #0
	ldr	r0, [r4, #4056]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4021
.L4029:
	ldr	r2, [fp, #-36]
	ldr	r1, [r4, #4068]
	sub	r3, r2, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #0
	add	r3, r3, r1
	str	r3, [r4, #4068]
	bne	.L4030
	add	r3, r5, #12288
	ldrb	r2, [r3, #1273]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3993
.L3983:
	ldr	r3, [fp, #-32]
	cmp	r3, #1
	bne	.L3977
	mov	r0, r5
	bl	FlushDPB
	subs	r3, r0, #0
	bne	.L4031
.L3989:
	ldr	r0, [r4, #4056]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L3977
	sub	r3, fp, #40
	sub	r2, fp, #44
	sub	r1, fp, #48
	ldr	r0, [r4, #4056]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r5]
	ldr	r3, [r3, #776]
	cmp	r3, #1
	bne	.L3977
	ldr	r3, [fp, #-44]
	ldr	r2, [fp, #-40]
	add	r3, r3, r2
	cmp	r3, #0
	ble	.L3977
.L3986:
	mov	r3, #1
	mvn	r0, #1
	str	r3, [r4, #2804]
	b	.L4021
.L4030:
	add	r7, r5, #20480
	ldrb	r3, [r7, #209]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #4072]
	addeq	r3, r3, #1
	addne	r3, r3, #2
	str	r3, [r4, #4072]
	add	r3, r5, #12288
	ldrb	r2, [r3, #1273]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3993
	ldr	r3, [r4, #3980]
	cmp	r3, #0
	beq	.L3983
	ldr	r3, [r5, #368]
	mov	ip, #0
	mov	r0, r6
	mov	r2, #12
	ldr	r1, .L4032+12
	str	ip, [r3, #64]
	bl	BsInit
	mov	r0, r5
	mov	r1, #1
	bl	H264_DecOneNal
	ldrb	r3, [r7, #209]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #4072]
	subeq	r3, r3, #1
	subne	r3, r3, #2
	str	r3, [r4, #4072]
	b	.L3986
.L3993:
	mov	r2, #0
	strb	r2, [r3, #1273]
	b	.L3983
.L4028:
	mov	r0, r5
	bl	InquireSliceExtProperty
	b	.L3978
.L4027:
	ldr	r3, .L4032
	mov	r0, #22
	ldr	r1, .L4032+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4021
.L4031:
	ldr	ip, .L4032
	movw	r2, #12973
	ldr	r1, .L4032+20
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3989
.L4024:
	ldr	ip, .L4032
	mov	r0, r1
	mov	r3, #12864
	ldr	r2, .L4032+24
	ldr	r1, .L4032+28
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L4021
.L4033:
	.align	2
.L4032:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC426
	.word	g_event_report
	.word	.LANCHOR0+432
	.word	.LC425
	.word	.LC354
	.word	.LC20
	.word	.LC21
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
	ldr	r6, [r0, #364]
	mov	r4, r0
	mov	r7, r3
	ldr	r2, [fp, #8]
	cmp	r6, #0
	bne	.L4035
	ldr	r3, [fp, #4]
	add	r5, r0, #53248
	cmp	r3, #0
	str	r1, [r5, #1508]
	beq	.L4036
	ldr	r3, .L4055
	mov	ip, r2, asl #6
	sub	r2, ip, r2, asl #3
	add	r2, r2, r3
	ldr	r3, [r2, #4]
	cmp	r3, #3
	beq	.L4052
	cmp	r3, #1
	beq	.L4036
.L4039:
	mov	r1, r7
	mov	r0, r4
	bl	StorePicInDPB
	cmp	r0, #1
	mov	r7, r0
	bls	.L4040
	ldr	r3, [r5, #1464]
	mov	r2, #1
	ldr	r0, [r5, #4056]
	mvn	r6, #0
	ldr	r1, [r3, #620]
	bl	FSP_ClearLogicFs
	ldr	r3, .L4055+4
	mov	r2, r7
	ldr	r1, .L4055+8
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	bl	H264_ClearCurrPic
	mov	r0, r4
	bl	ClearDPB
	mov	r3, #0
	str	r3, [r5, #1464]
.L4038:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4035:
	add	r5, r0, #53248
	str	r1, [r5, #1508]
	bl	ClearAllSlice
	mov	r1, r7
	mov	r0, r4
	bl	StorePicInDPB
	subs	r2, r0, #0
	bne	.L4053
.L4041:
	mov	r0, r4
	bl	FlushDPB
	subs	r2, r0, #0
	bne	.L4054
.L4042:
	add	r2, r4, #12288
	mov	r3, #0
	str	r3, [r5, #2800]
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
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4036:
	mov	r0, r4
	bl	ClearAllSlice
	b	.L4039
.L4040:
	mov	r3, #1
	mov	r0, r6
	str	r3, [r5, #2800]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4054:
	ldr	r3, .L4055+4
	mov	r0, #1
	ldr	r1, .L4055+12
	ldr	r3, [r3, #68]
	blx	r3
	b	.L4042
.L4053:
	ldr	r3, .L4055+4
	mov	r0, #1
	ldr	r1, .L4055+16
	ldr	r3, [r3, #68]
	blx	r3
	b	.L4041
.L4052:
	bl	ClearAllSlice
	b	.L4038
.L4056:
	.align	2
.L4055:
	.word	g_VdmDrvParam+48
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC427
	.word	.LC429
	.word	.LC428
	UNWIND(.fnend)
	.size	H264DEC_VDMPostProc, .-H264DEC_VDMPostProc
	.align	2
	.global	H264DEC_GetImageBuffer
	.type	H264DEC_GetImageBuffer, %function
H264DEC_GetImageBuffer:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	add	r6, r0, #53248
	mov	r9, r0
	ldr	r5, [r6, #2792]
	cmp	r5, #0
	beq	.L4073
	add	r4, r0, #27136
	add	r8, r0, #26368
	add	r4, r4, #176
	add	r8, r8, #72
	add	r7, r0, #24576
	mov	lr, #0
	mov	r0, r4
.L4065:
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4060
	cmp	r0, #0
	beq	.L4061
	ldr	ip, [r7, #2060]
	cmp	ip, #0
	beq	.L4061
	ldr	r2, [r7, #1864]
	cmp	r2, r0
	beq	.L4060
	mov	r2, r8
	b	.L4062
.L4063:
	ldr	r1, [r2, #4]!
	cmp	r1, r0
	beq	.L4060
.L4062:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L4063
.L4061:
	ldr	r0, [r6, #4056]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L4073
	cmn	r0, #1
	bne	.L4095
	add	r1, r9, #8
	ldr	r0, [r6, #4056]
	bl	FSP_ClearNotInVoQueue
	b	.L4095
.L4060:
	add	lr, lr, #1
	add	r0, r0, #672
	cmp	lr, r5
	bne	.L4065
	ldr	r8, .L4099
	sub	r2, fp, #48
	sub	r1, fp, #52
	add	r0, r9, #8
	bl	GetQueueImgNum
	ldr	r1, .L4099+4
	ldr	r3, [r8, #68]
	mov	r0, #0
	blx	r3
	ldr	r3, [r8, #68]
	ldr	r1, .L4099+8
	mov	r0, #0
	blx	r3
	ldr	r3, [r6, #2792]
	cmp	r3, #0
	beq	.L4097
	add	r3, r9, #26368
	add	r10, r9, #24576
	add	r3, r3, #72
	mov	r5, #0
	str	r9, [fp, #-56]
	mov	r7, r3
.L4066:
	cmp	r4, #0
	ldr	ip, [r8, #68]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	ldrb	r9, [r4, #4]	@ zero_extendqisi2
	beq	.L4077
	ldr	lr, [r10, #2060]
	cmp	lr, #0
	beq	.L4077
	ldr	r2, [r10, #1864]
	cmp	r4, r2
	beq	.L4079
	mov	r2, r7
	mov	r1, #0
	b	.L4070
.L4071:
	ldr	r0, [r2, #4]!
	cmp	r4, r0
	beq	.L4079
.L4070:
	add	r1, r1, #1
	cmp	r1, lr
	bne	.L4071
.L4077:
	mov	r2, #1
.L4069:
	str	r2, [sp, #4]
	mov	r0, #0
	mov	r2, r5
	str	r9, [sp]
	ldr	r1, .L4099+12
	add	r5, r5, #1
	blx	ip
	ldr	r3, [r6, #2792]
	add	r4, r4, #672
	cmp	r3, r5
	bhi	.L4066
	ldr	r9, [fp, #-56]
.L4072:
	ldr	r3, [fp, #-48]
	mov	r0, #0
	ldr	r2, [fp, #-52]
	ldr	r1, .L4099+16
	ldr	ip, [r8, #68]
	blx	ip
	ldr	r3, [r6, #2792]
	ldr	r0, [r10, #2060]
	ldr	r2, [fp, #-52]
	sub	r3, r3, #2
	ldr	r1, [fp, #-48]
	rsb	r3, r0, r3
	ldr	r0, .L4099
	add	r2, r2, r1
	cmp	r2, r3
	blt	.L4098
.L4095:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4073:
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4079:
	mov	r2, #0
	b	.L4069
.L4098:
	ldr	r3, [r0, #68]
	mov	r0, #0
	ldr	r1, .L4099+20
	blx	r3
	mov	r0, r9
	mov	r1, #1
	bl	ClearAll
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4097:
	add	r10, r9, #24576
	b	.L4072
.L4100:
	.align	2
.L4099:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC430
	.word	.LC431
	.word	.LC433
	.word	.LC432
	.word	.LC434
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
	ldr	r0, [r0, #1168]
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
	ldr	r3, [r3, #1172]
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
	.type	SarTable.14862, %object
	.size	SarTable.14862, 136
SarTable.14862:
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
	.type	__func__.15014, %object
	.size	__func__.15014, 12
__func__.15014:
	.ascii	"H264_DecSEI\000"
	.type	__func__.13901, %object
	.size	__func__.13901, 13
__func__.13901:
	.ascii	"DirectOutput\000"
	.space	3
	.type	__func__.14364, %object
	.size	__func__.14364, 8
__func__.14364:
	.ascii	"InitDPB\000"
	.type	__func__.14028, %object
	.size	__func__.14028, 14
__func__.14028:
	.ascii	"StorePicInDPB\000"
	.space	2
	.type	__FUNCTION__.15253, %object
	.size	__FUNCTION__.15253, 13
__FUNCTION__.15253:
	.ascii	"H264DEC_Init\000"
	.space	3
	.type	__FUNCTION__.14501, %object
	.size	__FUNCTION__.14501, 8
__FUNCTION__.14501:
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
	ASCII(.ascii	"dar=%d\012\000" )
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
	ASCII(.ascii	"\012 receive streambuff=0x%08x; phy_addr=0x%x; bits" )
	ASCII(.ascii	"tream_length=%d\012\000" )
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
	ASCII(.ascii	"line %d, get APC error, ret %d\012\000" )
.LC366:
	ASCII(.ascii	"InserFrmInDPB: cur pic struct = %d!\012\000" )
	.space	3
.LC367:
	ASCII(.ascii	"CurrPic.state is 'EMPTY'\012\000" )
	.space	2
.LC368:
	ASCII(.ascii	"StorePicInDPB return(%d) from L%d\012\000" )
	.space	1
.LC369:
	ASCII(.ascii	"directoutput mode\012\000" )
	.space	1
.LC370:
	ASCII(.ascii	"line: %d pImg is NULL!\012\000" )
.LC371:
	ASCII(.ascii	"line %d, REPORT_IFRAME_ERR\012\000" )
.LC372:
	ASCII(.ascii	"line %d, return %d\012\000" )
.LC373:
	ASCII(.ascii	"err(%d) > ref_thr(%d)\012\000" )
	.space	1
.LC374:
	ASCII(.ascii	"marking return %d\012\000" )
	.space	1
.LC375:
	ASCII(.ascii	"return value is unexpeced!\012\000" )
.LC376:
	ASCII(.ascii	"line %d: pH264Ctx->DPB.size = %d, ref %d, ltref %d\012" )
	ASCII(.ascii	"\000" )
.LC377:
	ASCII(.ascii	"%s: pos(%d) = pre_pos, force return.\012\000" )
	.space	2
.LC378:
	ASCII(.ascii	"FS_ALLOC_ERR, ClearAll\012\000" )
.LC379:
	ASCII(.ascii	"Can not new logic fs! ClearAll\012\000" )
.LC380:
	ASCII(.ascii	"GAP found while DPB is empty!\012\000" )
	.space	1
.LC381:
	ASCII(.ascii	"frame num gap try to fill it \012\000" )
	.space	1
.LC382:
	ASCII(.ascii	"CurrFrameNum = %d  UnusedShortTermFrameNum = %d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC383:
	ASCII(.ascii	"cann't allocate frame store when gap find\012\000" )
	.space	1
.LC384:
	ASCII(.ascii	"allocate frame store when gap find\012\000" )
.LC385:
	ASCII(.ascii	"line %d: store gap pic err, ret=%d\012\000" )
.LC386:
	ASCII(.ascii	"%s %d unknow error!!\012\000" )
	.space	2
.LC387:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC388:
	ASCII(.ascii	"%s %d frame_store is null!!\012\000" )
	.space	3
.LC389:
	ASCII(.ascii	"line %d: init dpb return %d\012\000" )
	.space	3
.LC390:
	ASCII(.ascii	"resolution error, the CAP_LEVEL_USER_DEFINE_WITH_OP" )
	ASCII(.ascii	"TION channel with s32ReRangeEn == 0 can't support s" )
	ASCII(.ascii	"uch bitstream.\012\000" )
	.space	2
.LC391:
	ASCII(.ascii	"s32MaxRefFrameNum < pH264Ctx->DPB.size, the CAP_LEV" )
	ASCII(.ascii	"EL_USER_DEFINE_WITH_OPTION channel can't support su" )
	ASCII(.ascii	"ch bitstream.\012\000" )
	.space	3
.LC392:
	ASCII(.ascii	"mem arrange err, ClearAll\012\000" )
	.space	1
.LC393:
	ASCII(.ascii	"get back frm\012\000" )
	.space	2
.LC394:
	ASCII(.ascii	"line %d: dec gap failed\012\000" )
	.space	3
.LC395:
	ASCII(.ascii	"line %d: alloc framestore failed\012\000" )
	.space	2
.LC396:
	ASCII(.ascii	"dec_pts: %lld\012\000" )
	.space	1
.LC397:
	ASCII(.ascii	"dec_usertag: %lld\012\000" )
	.space	1
.LC398:
	ASCII(.ascii	"%s %d NULL == pH264Ctx->CurrPic.frame_store!!\012\000" )
	.space	1
.LC399:
	ASCII(.ascii	"Start Reason: SliceParaNum, MaxBytesReceived = %d, " )
	ASCII(.ascii	"%d(thr=%d)\012\000" )
	.space	1
.LC400:
	ASCII(.ascii	"Too many slice or bitstream, err!\012\000" )
	.space	1
.LC401:
	ASCII(.ascii	"Start Reason: new_pic_flag\012\000" )
.LC402:
	ASCII(.ascii	"init pic err, find next recover point or next valid" )
	ASCII(.ascii	" sps, pps, or exit\012\000" )
	.space	1
.LC403:
	ASCII(.ascii	"DecList error, ret=%d\012\000" )
	.space	1
.LC404:
	ASCII(.ascii	"dec list err.\012\000" )
	.space	1
.LC405:
	ASCII(.ascii	"pH264Ctx->TotalNal = %d, type:%d\012\000" )
	.space	2
.LC406:
	ASCII(.ascii	"stop i want\012\000" )
	.space	3
.LC407:
	ASCII(.ascii	"svc_extension_flag error:priority_id:%d, pred_flag%" )
	ASCII(.ascii	"d\012\000" )
	.space	2
.LC408:
	ASCII(.ascii	"%d,%d,%d\012\000" )
	.space	2
.LC409:
	ASCII(.ascii	"svc stream,nal_header:%x\012\000" )
	.space	2
.LC410:
	ASCII(.ascii	"***** NAL: IDR/Slice, nal_unit_type=%d, TotalSlice=" )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC411:
	ASCII(.ascii	"SliceCheck failed, clear current slice.\012\000" )
	.space	3
.LC412:
	ASCII(.ascii	"***** NAL: PPS, nal_unit_type=%d, TotalPPS=%d\012\000" )
	.space	1
.LC413:
	ASCII(.ascii	"***** NAL: SPS, nal_unit_type=%d, TotalSPS=%d\012\000" )
	.space	1
.LC414:
	ASCII(.ascii	"SPS decode error.\012\000" )
	.space	1
.LC415:
	ASCII(.ascii	"***** NAL: SEI, nal_unit_type=%d\012\000" )
	.space	2
.LC416:
	ASCII(.ascii	"SEI decode error.\012\000" )
	.space	1
.LC417:
	ASCII(.ascii	"***** NAL: AUD, nal_unit_type=%d\012\000" )
	.space	2
.LC418:
	ASCII(.ascii	"***** NAL: EOSEQ, nal_unit_type=%d\012\000" )
.LC419:
	ASCII(.ascii	"***** NAL: FILL, nal_unit_type=%d\012\000" )
	.space	1
.LC420:
	ASCII(.ascii	"***** NAL: EOSTREAM, nal_unit_type=%d\012\000" )
	.space	1
.LC421:
	ASCII(.ascii	"***** NAL: SPSEXT, nal_unit_type=%d\012\000" )
	.space	3
.LC422:
	ASCII(.ascii	"***** NAL: AUX, nal_unit_type=%d\012\000" )
	.space	2
.LC423:
	ASCII(.ascii	"***** NAL: EOPIC, nal_unit_type=%d\012\000" )
.LC424:
	ASCII(.ascii	"***** NAL: UNSUPPORT, nal_unit_type=%d,nalu header:" )
	ASCII(.ascii	"%x\012\000" )
	.space	1
.LC425:
	ASCII(.ascii	"FindTrailZeros ERR\012\000" )
.LC426:
	ASCII(.ascii	"H264_DecOneNal ERR\012\000" )
.LC427:
	ASCII(.ascii	"store pic err, ret = %d\012\000" )
	.space	3
.LC428:
	ASCII(.ascii	"imode nal store pic err, ret = %d\012\000" )
	.space	1
.LC429:
	ASCII(.ascii	"imode nal flush dpb err, ret = %d\012\000" )
	.space	1
.LC430:
	ASCII(.ascii	"cann't find FrameStore\012\000" )
.LC431:
	ASCII(.ascii	"========== H264 FrameStore state(is_used, is_in_dpb" )
	ASCII(.ascii	", IsOutDPB) ========\012\000" )
	.space	3
.LC432:
	ASCII(.ascii	"ReadImgNum = %d, NewImgNum = %d\012\000" )
	.space	3
.LC433:
	ASCII(.ascii	"%02d: %d %d %d\012\000" )
.LC434:
	ASCII(.ascii	"FrameStore leak, ClearAll\012\000" )
	.bss
	.align	2
.LANCHOR2 = . + 0
.LANCHOR3 = . + 8184
	.type	pps_tmp.14782, %object
	.size	pps_tmp.14782, 2208
pps_tmp.14782:
	.space	2208
	.type	sps_tmp.14931, %object
	.size	sps_tmp.14931, 4000
sps_tmp.14931:
	.space	4000
	.type	TmpBuf, %object
	.size	TmpBuf, 68
TmpBuf:
	.space	68
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
