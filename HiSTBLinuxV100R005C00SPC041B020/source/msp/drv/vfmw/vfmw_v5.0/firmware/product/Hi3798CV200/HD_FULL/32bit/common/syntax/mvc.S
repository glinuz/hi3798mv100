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
	.file	"mvc.c"
	.text
	.align	2
	.global	MVC_IsSTRefFlg
	.type	MVC_IsSTRefFlg, %function
MVC_IsSTRefFlg:
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
	.size	MVC_IsSTRefFlg, .-MVC_IsSTRefFlg
	.align	2
	.global	MVC_IsLTRefFlg
	.type	MVC_IsLTRefFlg, %function
MVC_IsLTRefFlg:
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
	.size	MVC_IsLTRefFlg, .-MVC_IsLTRefFlg
	.align	2
	.global	MVC_compare_pic_by_pic_num_desc
	.type	MVC_compare_pic_by_pic_num_desc, %function
MVC_compare_pic_by_pic_num_desc:
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
	.size	MVC_compare_pic_by_pic_num_desc, .-MVC_compare_pic_by_pic_num_desc
	.align	2
	.global	MVC_compare_pic_by_lt_pic_num_asc
	.type	MVC_compare_pic_by_lt_pic_num_asc, %function
MVC_compare_pic_by_lt_pic_num_asc:
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
	.size	MVC_compare_pic_by_lt_pic_num_asc, .-MVC_compare_pic_by_lt_pic_num_asc
	.align	2
	.global	MVC_compare_fs_by_frame_num_desc
	.type	MVC_compare_fs_by_frame_num_desc, %function
MVC_compare_fs_by_frame_num_desc:
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
	ldr	r2, [r2, #24]
	ldr	r3, [r3, #24]
	cmp	r2, r3
	blt	.L12
	mvngt	r0, #0
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L12:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_compare_fs_by_frame_num_desc, .-MVC_compare_fs_by_frame_num_desc
	.align	2
	.global	MVC_compare_fs_by_lt_pic_idx_asc
	.type	MVC_compare_fs_by_lt_pic_idx_asc, %function
MVC_compare_fs_by_lt_pic_idx_asc:
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
	ldr	r2, [r2, #28]
	ldr	r3, [r3, #28]
	cmp	r2, r3
	bcc	.L15
	movhi	r0, #1
	movls	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L15:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_compare_fs_by_lt_pic_idx_asc, .-MVC_compare_fs_by_lt_pic_idx_asc
	.align	2
	.global	MVC_compare_pic_by_poc_asc
	.type	MVC_compare_pic_by_poc_asc, %function
MVC_compare_pic_by_poc_asc:
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
	.size	MVC_compare_pic_by_poc_asc, .-MVC_compare_pic_by_poc_asc
	.align	2
	.global	MVC_compare_pic_by_poc_desc
	.type	MVC_compare_pic_by_poc_desc, %function
MVC_compare_pic_by_poc_desc:
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
	.size	MVC_compare_pic_by_poc_desc, .-MVC_compare_pic_by_poc_desc
	.align	2
	.global	MVC_compare_fs_by_poc_asc
	.type	MVC_compare_fs_by_poc_asc, %function
MVC_compare_fs_by_poc_asc:
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
	ldr	r2, [r2, #32]
	ldr	r3, [r3, #32]
	cmp	r2, r3
	blt	.L24
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L24:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_compare_fs_by_poc_asc, .-MVC_compare_fs_by_poc_asc
	.align	2
	.global	MVC_compare_fs_by_poc_desc
	.type	MVC_compare_fs_by_poc_desc, %function
MVC_compare_fs_by_poc_desc:
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
	ldr	r2, [r2, #32]
	ldr	r3, [r3, #32]
	cmp	r2, r3
	blt	.L27
	mvngt	r0, #0
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L27:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_compare_fs_by_poc_desc, .-MVC_compare_fs_by_poc_desc
	.align	2
	.type	MVC_SetFrmRepeatCount.part.1, %function
MVC_SetFrmRepeatCount.part.1:
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
	add	ip, r0, #11075584
	ldr	lr, [r0, #136]
	add	ip, ip, #45056
	ldr	r3, [r0, #132]
	ldr	r2, [ip, #2856]
	mov	r3, r3, lsr #1
	add	r2, r2, lr, lsr #1
	add	ip, r2, #1
	cmp	r3, ip
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
	ldrhi	r3, [r0, #136]
	ldrls	r3, [r0, #136]
	addhi	r3, r3, #4
	addls	r3, r3, #2
	str	r3, [r0, #136]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_SetFrmRepeatCount.part.1, .-MVC_SetFrmRepeatCount.part.1
	.align	2
	.type	MVC_CombinePacket.part.12, %function
MVC_CombinePacket.part.12:
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
	ldr	r1, .L33
	mov	r4, r0
	mov	r0, #7
	bl	dprint_vfmw
	ldr	ip, [r4, #232]
	ldr	r1, .L33+4
	mov	r0, #7
	mov	r5, #0
	ldr	lr, [ip, #40]
	ldr	r3, [ip, #44]
	ldr	r2, [ip, #16]
	str	lr, [sp, #4]
	ldr	ip, [ip, #12]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #232]
	ldr	ip, .L33+8
	ldr	r2, [r3, #12]
	ldr	r0, [r3, #36]
	ldr	r1, [r3, #8]
	rsb	r0, r2, r0
	ldr	r3, [ip, #52]
	blx	r3
	ldr	r3, [r4, #232]
	mov	ip, #1
	ldr	r1, .L33+12
	mov	r0, #7
	ldr	r2, [r3, #60]
	ldr	r6, [r3, #32]
	str	r2, [r3, #32]
	ldr	r3, [r4, #232]
	str	r5, [r3, #24]
	ldr	r3, [r4, #232]
	ldr	lr, [r3, #12]
	ldr	r2, [r3, #36]
	rsb	r2, lr, r2
	str	r2, [r3, #8]
	ldr	r3, [r4, #232]
	ldr	lr, [r3, #12]
	ldr	r2, [r3, #44]
	rsb	r2, lr, r2
	str	r2, [r3, #16]
	ldr	r3, [r4, #232]
	ldr	lr, [r3, #40]
	ldr	r2, [r3, #12]
	add	r2, r2, lr
	str	r2, [r3, #12]
	ldr	r3, [r4, #232]
	str	ip, [r3, #68]
	ldr	r2, [r4, #232]
	ldr	r3, [r2, #12]
	ldr	r2, [r2, #16]
	bl	dprint_vfmw
	mov	r1, r6
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #232]
	str	r5, [r3, #36]
	ldr	r3, [r4, #232]
	str	r5, [r3, #52]
	ldr	r3, [r4, #232]
	str	r5, [r3, #40]
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L34:
	.align	2
.L33:
	.word	.LC0
	.word	.LC1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	UNWIND(.fnend)
	.size	MVC_CombinePacket.part.12, .-MVC_CombinePacket.part.12
	.align	2
	.global	mvc_ue_v
	.type	mvc_ue_v, %function
mvc_ue_v:
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
	add	r8, r0, #548
	mov	r6, r0
	mov	r7, r1
	mov	r1, #32
	mov	r0, r8
	bl	BsShow
	mov	r5, r0
	bl	ZerosMS_32
	cmp	r0, #15
	mov	r4, r0
	bls	.L39
	cmp	r0, #31
	bls	.L40
	mvn	r5, #0
	mov	r8, #32
	mov	r3, #1
	strb	r3, [r6, #10]
.L37:
	ldr	r6, [r6, #232]
	mov	r2, r7
	mov	r3, r5
	ldr	r1, .L41
	mov	r0, #21
	ldr	lr, [r6, #64]
	add	r4, lr, r8
	str	r4, [r6, #64]
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L39:
	mov	r4, r0, asl #1
	mov	r0, r8
	add	r8, r4, #1
	rsb	r4, r4, #31
	mov	r4, r5, lsr r4
	mov	r1, r8
	sub	r5, r4, #1
	bl	BsSkip
	b	.L37
.L40:
	add	r5, r0, #1
	mov	r1, r0
	mov	r0, r8
	bl	BsSkip
	mov	r1, r5
	mov	r0, r8
	bl	BsShow
	mov	r1, r5
	sub	r5, r0, #1
	mov	r0, r8
	bl	BsSkip
	mov	ip, r4, asl #1
	add	r8, ip, #1
	b	.L37
.L42:
	.align	2
.L41:
	.word	.LC3
	UNWIND(.fnend)
	.size	mvc_ue_v, .-mvc_ue_v
	.align	2
	.global	mvc_se_v
	.type	mvc_se_v, %function
mvc_se_v:
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
	add	r7, r0, #548
	mov	r8, r0
	mov	r6, r1
	mov	r1, #32
	mov	r0, r7
	bl	BsShow
	mov	r4, r0
	bl	ZerosMS_32
	cmp	r0, #15
	mov	r5, r0
	bhi	.L44
	mov	r5, r0, asl #1
	mov	r0, r7
	rsb	r3, r5, #31
	add	r5, r5, #1
	mov	r3, r4, lsr r3
	and	r2, r3, #1
	mov	r1, r5
	rsb	r4, r2, #0
	eor	r3, r4, r3, lsr #1
	add	r4, r3, r2
	bl	BsSkip
.L45:
	ldr	lr, [r8, #232]
	mov	r2, r6
	mov	r3, r4
	ldr	r1, .L48
	mov	r0, #21
	ldr	ip, [lr, #64]
	add	r5, ip, r5
	str	r5, [lr, #64]
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L44:
	cmp	r0, #31
	bls	.L47
	mov	r3, #1
	mvn	r4, #-2147483648
	mov	r5, #32
	strb	r3, [r8, #10]
	b	.L45
.L47:
	mov	r1, r0
	mov	r0, r7
	bl	BsSkip
	mov	r1, r5
	mov	r0, r7
	mov	r5, r5, asl #1
	bl	BsGet
	mov	r1, #1
	add	r5, r5, #1
	mov	r9, r0
	mov	r0, r7
	bl	BsGet
	and	r0, r0, #1
	rsb	r4, r0, #0
	eor	r9, r9, r4
	add	r4, r9, r0
	b	.L45
.L49:
	.align	2
.L48:
	.word	.LC3
	UNWIND(.fnend)
	.size	mvc_se_v, .-mvc_se_v
	.align	2
	.global	mvc_u_v
	.type	mvc_u_v, %function
mvc_u_v:
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
	add	r0, r0, #548
	mov	r6, r1
	mov	r7, r2
	bl	BsGet
	ldr	lr, [r5, #232]
	mov	r2, r7
	ldr	r1, .L51
	ldr	ip, [lr, #64]
	add	ip, ip, r6
	str	ip, [lr, #64]
	mov	r4, r0
	mov	r3, r0
	mov	r0, #21
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L52:
	.align	2
.L51:
	.word	.LC3
	UNWIND(.fnend)
	.size	mvc_u_v, .-mvc_u_v
	.align	2
	.global	mvc_u_1
	.type	mvc_u_1, %function
mvc_u_1:
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
	mov	r6, r1
	add	r0, r0, #548
	mov	r1, #1
	bl	BsGet
	ldr	lr, [r5, #232]
	mov	r2, r6
	ldr	r1, .L54
	ldr	ip, [lr, #64]
	add	ip, ip, #1
	str	ip, [lr, #64]
	mov	r4, r0
	mov	r3, r0
	mov	r0, #21
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L55:
	.align	2
.L54:
	.word	.LC3
	UNWIND(.fnend)
	.size	mvc_u_1, .-mvc_u_1
	.align	2
	.global	MVC_GetMinPOC
	.type	MVC_GetMinPOC, %function
MVC_GetMinPOC:
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
	add	r10, r0, #11075584
	add	r10, r10, #45056
	mov	r9, r3
	mov	r8, r2
	mvn	r3, #-2147483648
	mvn	r2, #0
	str	r2, [r9]
	str	r3, [r8]
	mov	r6, r0
	ldr	r3, [r10, #2856]
	mov	r7, r1
	ldr	r2, [r10, #2860]
	cmp	r2, r3
	strhi	r3, [r10, #2860]
	cmp	r3, #0
	beq	.L63
	movw	r5, #47716
	mvn	r3, #0
	movt	r5, 169
	str	r3, [fp, #-48]
	add	r5, r0, r5
	mov	r4, #0
	b	.L62
.L60:
	ldr	r1, [r8]
	ldr	r2, [r3, #32]
	cmp	r1, r2
	ble	.L59
	ldrb	r1, [r3, #5]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L59
	ldrb	r1, [r0, #1]	@ zero_extendqisi2
	sub	r1, r1, #1
	cmp	r1, #1
	bls	.L78
.L59:
	ldr	r3, [r10, #2856]
	add	r4, r4, #1
	cmp	r3, r4
	bls	.L63
.L62:
	ldr	r3, [r5, #4]!
	cmp	r3, #0
	beq	.L59
	ldrsb	r1, [r3, #6]
	ldr	r0, [r6, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L59
	cmn	r7, #1
	ldr	r3, [r5]
	bne	.L60
	ldrb	r2, [r3, #5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L59
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	sub	r2, r2, #1
	cmp	r2, #1
	bhi	.L59
	ldr	r1, [r8]
	ldr	r2, [r3, #32]
	cmp	r1, r2
	ble	.L61
	str	r2, [r8]
	str	r4, [r9]
	add	r4, r4, #1
	ldr	r3, [r5]
	ldr	r3, [r3, #56]
	str	r3, [fp, #-48]
	ldr	r3, [r10, #2856]
	cmp	r3, r4
	bhi	.L62
.L63:
	ldr	r0, [r9]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L78:
	ldr	r3, [r3, #56]
	cmp	r7, r3
	streq	r2, [r8]
	streq	r4, [r9]
	b	.L59
.L61:
	bne	.L59
	ldr	r3, [r3, #56]
	ldr	r2, [fp, #-48]
	cmp	r3, r2
	strlt	r4, [r9]
	ldrlt	r3, [r5]
	ldrlt	r3, [r3, #56]
	strlt	r3, [fp, #-48]
	b	.L59
	UNWIND(.fnend)
	.size	MVC_GetMinPOC, .-MVC_GetMinPOC
	.align	2
	.global	MVC_FrameStoreRefFlg
	.type	MVC_FrameStoreRefFlg, %function
MVC_FrameStoreRefFlg:
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
	ldrb	r0, [r0, #3]	@ zero_extendqisi2
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_FrameStoreRefFlg, .-MVC_FrameStoreRefFlg
	.align	2
	.global	MVC_NonLongTermRefFlg
	.type	MVC_NonLongTermRefFlg, %function
MVC_NonLongTermRefFlg:
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
	cmp	r3, #3
	beq	.L95
	tst	r3, #1
	bne	.L96
.L84:
	and	r3, r3, #2
	ands	r3, r3, #255
	beq	.L88
.L85:
	ldrb	r0, [r0, #769]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L88:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L96:
	ldrb	r2, [r0, #733]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L84
.L89:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L95:
	ldrb	r3, [r0, #697]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L89
	ldrb	r3, [r0, #733]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L85
	b	.L89
	UNWIND(.fnend)
	.size	MVC_NonLongTermRefFlg, .-MVC_NonLongTermRefFlg
	.align	2
	.global	MVC_ShortTermRefFlg
	.type	MVC_ShortTermRefFlg, %function
MVC_ShortTermRefFlg:
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
	cmp	r3, #3
	beq	.L112
	tst	r3, #1
	bne	.L113
.L101:
	and	r3, r3, #2
	ands	r3, r3, #255
	beq	.L105
.L102:
	ldr	r0, [r0, #768]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #65536
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L105:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L113:
	ldr	r2, [r0, #732]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #65536
	bne	.L101
.L106:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L112:
	ldr	r3, [r0, #696]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L106
	ldr	r3, [r0, #732]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	bne	.L102
	b	.L106
	UNWIND(.fnend)
	.size	MVC_ShortTermRefFlg, .-MVC_ShortTermRefFlg
	.align	2
	.global	MVC_LongTermRefFlg
	.type	MVC_LongTermRefFlg, %function
MVC_LongTermRefFlg:
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
	cmp	r3, #3
	beq	.L129
	tst	r3, #1
	bne	.L130
.L118:
	and	r3, r3, #2
	ands	r3, r3, #255
	beq	.L122
.L119:
	ldr	r0, [r0, #768]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	sub	r0, r0, #256
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L122:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L130:
	ldr	r2, [r0, #732]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #256
	bne	.L118
.L123:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L129:
	ldr	r3, [r0, #696]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	beq	.L123
	ldr	r3, [r0, #732]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	bne	.L119
	b	.L123
	UNWIND(.fnend)
	.size	MVC_LongTermRefFlg, .-MVC_LongTermRefFlg
	.align	2
	.global	MVC_UpdateLTReflist
	.type	MVC_UpdateLTReflist, %function
MVC_UpdateLTReflist:
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
	add	r3, r0, #11075584
	add	r3, r3, #45056
	mov	r7, r0
	str	r3, [fp, #-48]
	ldr	r10, [r3, #2856]
	cmp	r10, #0
	beq	.L132
	movw	r8, #47716
	add	r9, r0, #11141120
	movt	r8, 169
	mov	r4, #0
	add	r9, r9, #12288
	add	r8, r0, r8
	mov	r6, r4
.L134:
	ldr	r5, [r8, #4]!
	add	r6, r6, #1
	cmp	r5, #0
	mov	r0, r5
	beq	.L133
	bl	MVC_LongTermRefFlg
	cmp	r0, #0
	beq	.L133
	ldr	r2, [r9, #3496]
	ldr	r1, [r5, #56]
	cmp	r1, r2
	movweq	r2, #28346
	movteq	r2, 42
	addeq	r2, r4, r2
	addeq	r4, r4, #1
	streq	r5, [r7, r2, asl #2]
.L133:
	cmp	r6, r10
	bne	.L134
	ldr	r3, [fp, #-48]
	cmp	r4, r10
	str	r4, [r3, #2868]
	bcs	.L131
	movw	r0, #28345
	mov	r2, #0
	movt	r0, 42
	add	r0, r4, r0
	add	r0, r7, r0, lsl #2
.L137:
	add	r4, r4, #1
	str	r2, [r0, #4]!
	cmp	r4, r10
	bne	.L137
.L131:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L132:
	ldr	r3, [fp, #-48]
	str	r10, [r3, #2868]
	b	.L131
	UNWIND(.fnend)
	.size	MVC_UpdateLTReflist, .-MVC_UpdateLTReflist
	.align	2
	.global	MVC_UpdateReflist
	.type	MVC_UpdateReflist, %function
MVC_UpdateReflist:
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
	add	r3, r0, #11075584
	add	r3, r3, #45056
	mov	r7, r0
	str	r3, [fp, #-48]
	ldr	r10, [r3, #2856]
	cmp	r10, #0
	beq	.L148
	movw	r8, #47716
	add	r9, r0, #11141120
	movt	r8, 169
	mov	r4, #0
	add	r9, r9, #12288
	add	r8, r0, r8
	mov	r6, r4
.L150:
	ldr	r5, [r8, #4]!
	add	r6, r6, #1
	cmp	r5, #0
	mov	r0, r5
	beq	.L149
	bl	MVC_ShortTermRefFlg
	cmp	r0, #0
	beq	.L149
	ldr	r2, [r9, #3496]
	ldr	r1, [r5, #56]
	cmp	r1, r2
	movweq	r2, #28330
	movteq	r2, 42
	addeq	r2, r4, r2
	addeq	r4, r4, #1
	streq	r5, [r7, r2, asl #2]
.L149:
	cmp	r6, r10
	bne	.L150
	ldr	r3, [fp, #-48]
	cmp	r4, r10
	str	r4, [r3, #2864]
	bcs	.L147
	movw	r0, #28329
	mov	r2, #0
	movt	r0, 42
	add	r0, r4, r0
	add	r0, r7, r0, lsl #2
.L153:
	add	r4, r4, #1
	str	r2, [r0, #4]!
	cmp	r4, r10
	bne	.L153
.L147:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L148:
	ldr	r3, [fp, #-48]
	str	r10, [r3, #2864]
	b	.L147
	UNWIND(.fnend)
	.size	MVC_UpdateReflist, .-MVC_UpdateReflist
	.align	2
	.global	MVC_GetPicNumX
	.type	MVC_GetPicNumX, %function
MVC_GetPicNumX:
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
	.size	MVC_GetPicNumX, .-MVC_GetPicNumX
	.align	2
	.global	MVC_UnMarkFrameStoreRef
	.type	MVC_UnMarkFrameStoreRef, %function
MVC_UnMarkFrameStoreRef:
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
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	mov	r3, r1
	tst	r2, #1
	beq	.L167
	ldrb	r1, [r1, #3]	@ zero_extendqisi2
	mov	ip, #0
	strb	ip, [r3, #733]
	and	r1, r1, #2
	strb	ip, [r3, #734]
	strb	r1, [r3, #3]
.L167:
	tst	r2, #2
	beq	.L168
	ldrb	r1, [r3, #3]	@ zero_extendqisi2
	mov	ip, #0
	strb	ip, [r3, #769]
	and	r1, r1, #1
	strb	ip, [r3, #770]
	strb	r1, [r3, #3]
.L168:
	cmp	r2, #3
	ldrsb	r1, [r3, #6]
	moveq	r2, #0
	streqb	r2, [r3, #698]
	streqb	r2, [r3, #697]
	mov	r2, #0
	strb	r2, [r3, #3]
	ldr	r0, [r0, #120]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	FSP_SetRef
	UNWIND(.fnend)
	.size	MVC_UnMarkFrameStoreRef, .-MVC_UnMarkFrameStoreRef
	.align	2
	.global	MVC_UnMarkLTFrmByFrmIdx
	.type	MVC_UnMarkLTFrmByFrmIdx, %function
MVC_UnMarkLTFrmByFrmIdx:
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
	add	r3, r0, #11075584
	add	r3, r3, #45056
	ldr	r4, [r3, #2868]
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, fp, sp, pc}
	movw	ip, #47844
	add	r6, r0, #11141120
	movt	ip, 169
	add	r6, r6, #12288
	add	ip, r0, ip
	mov	r3, #0
	b	.L179
.L178:
	cmp	r3, r4
	beq	.L184
.L179:
	ldr	r2, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r2, #28]
	cmp	lr, r1
	bne	.L178
	ldr	r5, [r2, #56]
	ldr	lr, [r6, #3496]
	cmp	r5, lr
	bne	.L178
	mov	r1, r2
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, lr}
	b	MVC_UnMarkFrameStoreRef
.L184:
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_UnMarkLTFrmByFrmIdx, .-MVC_UnMarkLTFrmByFrmIdx
	.align	2
	.global	MVC_UnMarkLTFldByFrmIdx
	.type	MVC_UnMarkLTFldByFrmIdx, %function
MVC_UnMarkLTFldByFrmIdx:
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
	add	ip, r0, #11075584
	add	ip, ip, #45056
	ldr	r6, [ip, #2868]
	cmp	r6, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	movw	r4, #47844
	add	r8, r0, #11141120
	movt	r4, 169
	add	r8, r8, #12288
	add	r4, r0, r4
	mov	ip, #0
	b	.L189
.L187:
	cmp	ip, r6
	beq	.L203
.L189:
	ldr	lr, [r4, #4]!
	add	ip, ip, #1
	ldr	r5, [lr, #28]
	cmp	r5, r3
	bne	.L187
	ldr	r7, [lr, #56]
	ldr	r5, [r8, #3496]
	cmp	r7, r5
	bne	.L187
	cmp	r2, #1
	ldreqb	r5, [lr, #769]	@ zero_extendqisi2
	beq	.L202
	cmp	r2, #2
	bne	.L187
	ldrb	r5, [lr, #733]	@ zero_extendqisi2
.L202:
	clz	r5, r5
	mov	r5, r5, lsr #5
	cmp	lr, r1
	orrne	r5, r5, #1
	cmp	r5, #0
	beq	.L187
	mov	r1, lr
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	MVC_UnMarkFrameStoreRef
.L203:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_UnMarkLTFldByFrmIdx, .-MVC_UnMarkLTFldByFrmIdx
	.align	2
	.global	MVC_UnMarkSTRef
	.type	MVC_UnMarkSTRef, %function
MVC_UnMarkSTRef:
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
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L205
	add	ip, r0, #11075584
	ldr	r1, [r1, #648]
	add	ip, ip, #45056
	sub	r1, r1, #1
	ldr	r4, [ip, #2864]
	rsb	r2, r2, r1
	cmp	r4, #0
	beq	.L234
	movw	ip, #47780
	add	r5, r0, #11141120
	movt	ip, 169
	add	r5, r5, #12288
	add	ip, r0, ip
	b	.L211
.L210:
	cmp	r3, r4
	beq	.L235
.L211:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #708]
	cmp	lr, r2
	bne	.L210
	ldrb	lr, [r1, #3]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L210
	ldr	lr, [r1, #696]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L210
	ldr	r6, [r1, #56]
	ldr	lr, [r5, #3496]
	cmp	r6, lr
	bne	.L210
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	MVC_UnMarkFrameStoreRef
.L205:
	add	r3, r0, #11075584
	ldr	r1, [r1, #648]
	add	r3, r3, #45056
	ldr	r5, [r3, #2864]
	rsb	r2, r2, r1, lsl #1
	cmp	r5, #0
	beq	.L236
	movw	r4, #47780
	add	r7, r0, #11141120
	movt	r4, 169
	add	r7, r7, #12288
	add	r4, r0, r4
	mov	ip, #0
	b	.L216
.L212:
	tst	r3, #2
	beq	.L214
	ldr	r3, [r1, #768]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L237
.L214:
	add	ip, ip, #1
	cmp	ip, r5
	beq	.L238
.L216:
	ldr	r1, [r4, #4]!
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	ands	r6, r3, #1
	beq	.L212
	ldr	lr, [r1, #732]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L212
	ldr	r8, [r1, #56]
	ldr	lr, [r7, #3496]
	cmp	r8, lr
	bne	.L212
	ldr	lr, [r1, #744]
	cmp	lr, r2
	bne	.L212
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #11075584
	strb	r3, [r1, #3]
	add	r2, r2, #45056
	mov	r1, #0
	ldr	r3, [r2, #2728]
	strb	r1, [r3, #734]
	ldr	r3, [r2, #2728]
	ldrb	ip, [r3, #696]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #698]
	ldreq	r3, [r2, #2728]
	streqb	r1, [r3, #697]
	ldreq	r3, [r2, #2728]
	ldrb	ip, [r3, #3]	@ zero_extendqisi2
	cmp	ip, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	ldr	r1, [r3, #48]
	mov	r2, ip
	add	r1, r0, r1, lsl #2
	str	ip, [r1, #148]
	ldrsb	r1, [r3, #6]
	b	.L231
.L237:
	ldr	lr, [r1, #56]
	ldr	r3, [r7, #3496]
	cmp	lr, r3
	bne	.L214
	ldr	r3, [r1, #780]
	cmp	r3, r2
	bne	.L214
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #3]
	add	r3, r3, #11075584
	mov	r2, #0
	add	r3, r3, #45056
	ldr	r1, [r3, #2728]
	strb	r2, [r1, #770]
	ldr	r1, [r3, #2728]
	ldrb	ip, [r1, #696]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r2
	streqb	r2, [r1, #698]
	ldreq	r1, [r3, #2728]
	streqb	r2, [r1, #697]
	ldreq	r1, [r3, #2728]
	ldrb	ip, [r1, #3]	@ zero_extendqisi2
	cmp	ip, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	ldr	r3, [r1, #48]
	mov	r2, ip
	add	r3, r0, r3, lsl #2
	str	ip, [r3, #148]
	ldrsb	r1, [r1, #6]
.L231:
	ldr	r0, [r0, #120]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	FSP_SetRef
.L238:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L235:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L234:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L236:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_UnMarkSTRef, .-MVC_UnMarkSTRef
	.align	2
	.global	MVC_UnMarkLTRef
	.type	MVC_UnMarkLTRef, %function
MVC_UnMarkLTRef:
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
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L240
	add	r3, r0, #11075584
	add	r3, r3, #45056
	ldr	r5, [r3, #2868]
	cmp	r5, #0
	beq	.L269
	movw	r4, #47844
	add	r7, r0, #11141120
	movt	r4, 169
	add	r7, r7, #12288
	add	r4, r0, r4
	mov	ip, #0
	b	.L249
.L245:
	tst	r3, #2
	beq	.L247
	ldr	r3, [r1, #768]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	beq	.L270
.L247:
	add	ip, ip, #1
	cmp	ip, r5
	beq	.L271
.L249:
	ldr	r1, [r4, #4]!
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	ands	r6, r3, #1
	beq	.L245
	ldr	lr, [r1, #732]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L245
	ldr	r8, [r1, #56]
	ldr	lr, [r7, #3496]
	cmp	r8, lr
	bne	.L245
	ldr	lr, [r1, #740]
	cmp	r2, lr
	bne	.L245
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #11075584
	strb	r3, [r1, #3]
	add	r2, r2, #45056
	mov	r1, #0
	ldr	r3, [r2, #2792]
	strb	r1, [r3, #733]
	ldr	r3, [r2, #2792]
	ldrb	ip, [r3, #696]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #698]
	ldreq	r3, [r2, #2792]
	streqb	r1, [r3, #697]
	ldreq	r3, [r2, #2792]
	ldrb	ip, [r3, #3]	@ zero_extendqisi2
	cmp	ip, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	ldr	r1, [r3, #48]
	mov	r2, ip
	add	r1, r0, r1, lsl #2
	str	ip, [r1, #148]
	ldrsb	r1, [r3, #6]
	b	.L266
.L240:
	add	r1, r0, #11075584
	add	r1, r1, #45056
	ldr	r4, [r1, #2868]
	cmp	r4, #0
	beq	.L272
	movw	ip, #47844
	add	r5, r0, #11141120
	movt	ip, 169
	add	r5, r5, #12288
	add	ip, r0, ip
	b	.L251
.L250:
	cmp	r3, r4
	beq	.L273
.L251:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #704]
	cmp	lr, r2
	bne	.L250
	ldrb	lr, [r1, #3]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L250
	ldr	lr, [r1, #696]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L250
	ldr	r6, [r1, #56]
	ldr	lr, [r5, #3496]
	cmp	r6, lr
	bne	.L250
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	MVC_UnMarkFrameStoreRef
.L270:
	ldr	lr, [r1, #56]
	ldr	r3, [r7, #3496]
	cmp	lr, r3
	bne	.L247
	ldr	r3, [r1, #776]
	cmp	r2, r3
	bne	.L247
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #3]
	add	r3, r3, #11075584
	mov	r2, #0
	add	r3, r3, #45056
	ldr	r1, [r3, #2792]
	strb	r2, [r1, #769]
	ldr	r1, [r3, #2792]
	ldrb	ip, [r1, #696]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r2
	streqb	r2, [r1, #698]
	ldreq	r1, [r3, #2792]
	streqb	r2, [r1, #697]
	ldreq	r1, [r3, #2792]
	ldrb	ip, [r1, #3]	@ zero_extendqisi2
	cmp	ip, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	ldr	r3, [r1, #48]
	mov	r2, ip
	add	r3, r0, r3, lsl #2
	str	ip, [r3, #148]
	ldrsb	r1, [r1, #6]
.L266:
	ldr	r0, [r0, #120]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	FSP_SetRef
.L273:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L271:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L272:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L269:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_UnMarkLTRef, .-MVC_UnMarkLTRef
	.align	2
	.global	MVC_MarkPicLTRef
	.type	MVC_MarkPicLTRef, %function
MVC_MarkPicLTRef:
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
	ldrb	r6, [r3, #3]	@ zero_extendqisi2
	add	r3, r0, #11075584
	add	r3, r3, #45056
	cmp	r6, #0
	beq	.L275
	ldr	r7, [r3, #2864]
	cmp	r7, #0
	beq	.L304
	movw	r5, #47780
	add	r8, r0, #11141120
	movt	r5, 169
	add	r8, r8, #12288
	add	r5, r0, r5
	mov	ip, #0
	b	.L289
.L281:
	tst	r4, #2
	beq	.L284
	ldr	lr, [r3, #768]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	beq	.L305
.L284:
	add	ip, ip, #1
	cmp	ip, r7
	beq	.L306
.L289:
	ldr	r3, [r5, #4]!
	ldrb	r4, [r3, #3]	@ zero_extendqisi2
	tst	r4, #1
	beq	.L281
	ldr	lr, [r3, #732]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L281
	ldr	lr, [r3, #744]
	cmp	lr, r2
	bne	.L281
	ldr	r9, [r3, #56]
	ldr	lr, [r8, #3496]
	cmp	r9, lr
	bne	.L281
	ldrb	lr, [r3, #769]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L285
	ldr	lr, [r3, #28]
	cmp	lr, r1
	bne	.L284
.L285:
	add	r0, r0, ip, lsl #2
	str	r1, [r3, #28]
	add	r3, r0, #11075584
	sub	r6, r6, #1
	add	r3, r3, #45056
	clz	r6, r6
	mov	ip, #1
	mov	r0, #0
	ldr	r2, [r3, #2728]
	mov	r6, r6, lsr #5
	add	r1, r6, r1, lsl #1
	str	r1, [r2, #740]
	ldr	r2, [r3, #2728]
	strb	ip, [r2, #733]
	ldr	r2, [r3, #2728]
	strb	r0, [r2, #734]
	ldr	r2, [r3, #2728]
	ldrb	r1, [r2, #696]	@ zero_extendqisi2
	cmp	r1, #3
	cmpne	r1, r0
	bne	.L307
	ldrb	r1, [r2, #769]	@ zero_extendqisi2
	ldrb	r0, [r2, #733]	@ zero_extendqisi2
	cmp	r1, r0
	streqb	r1, [r2, #697]
	ldreq	r3, [r3, #2728]
	ldreqb	r2, [r3, #734]	@ zero_extendqisi2
	streqb	r2, [r3, #698]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L275:
	ldr	r4, [r3, #2864]
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	movw	ip, #47780
	add	r5, r0, #11141120
	movt	ip, 169
	add	r5, r5, #12288
	add	ip, r0, ip
	b	.L280
.L279:
	add	r6, r6, #1
	cmp	r6, r4
	beq	.L308
.L280:
	ldr	r3, [ip, #4]!
	ldrb	lr, [r3, #3]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L279
	ldr	lr, [r3, #696]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L279
	ldr	lr, [r3, #708]
	cmp	lr, r2
	bne	.L279
	ldr	r7, [r3, #56]
	ldr	lr, [r5, #3496]
	cmp	r7, lr
	bne	.L279
	add	r6, r0, r6, lsl #2
	str	r1, [r3, #28]
	add	r3, r6, #11075584
	mov	r0, #1
	add	r3, r3, #45056
	mov	r2, #0
	ldr	ip, [r3, #2728]
	str	r1, [ip, #704]
	ldr	r1, [r3, #2728]
	strb	r0, [r1, #697]
	ldr	r1, [r3, #2728]
	strb	r0, [r1, #733]
	ldr	r1, [r3, #2728]
	strb	r0, [r1, #769]
	ldr	r1, [r3, #2728]
	strb	r2, [r1, #698]
	ldr	r1, [r3, #2728]
	strb	r2, [r1, #734]
	ldr	r3, [r3, #2728]
	strb	r2, [r3, #770]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L305:
	ldr	lr, [r3, #780]
	cmp	lr, r2
	bne	.L284
	ldr	r4, [r3, #56]
	ldr	lr, [r8, #3496]
	cmp	r4, lr
	bne	.L284
	ldrb	lr, [r3, #733]	@ zero_extendqisi2
	cmp	lr, #1
	bne	.L288
	ldr	lr, [r3, #28]
	cmp	lr, r1
	bne	.L284
.L288:
	add	r0, r0, ip, lsl #2
	str	r1, [r3, #28]
	add	r3, r0, #11075584
	sub	r6, r6, #2
	add	r3, r3, #45056
	clz	r6, r6
	mov	ip, #1
	mov	r0, #0
	ldr	r2, [r3, #2728]
	mov	r6, r6, lsr #5
	add	r1, r6, r1, lsl #1
	str	r1, [r2, #776]
	ldr	r2, [r3, #2728]
	strb	ip, [r2, #769]
	ldr	r2, [r3, #2728]
	strb	r0, [r2, #770]
	ldr	r2, [r3, #2728]
	ldrb	r1, [r2, #696]	@ zero_extendqisi2
	cmp	r1, #3
	cmpne	r1, r0
	bne	.L309
	ldrb	r1, [r2, #769]	@ zero_extendqisi2
	ldrb	r0, [r2, #733]	@ zero_extendqisi2
	cmp	r1, r0
	streqb	r1, [r2, #697]
	ldreq	r3, [r3, #2728]
	ldreqb	r2, [r3, #770]	@ zero_extendqisi2
	streqb	r2, [r3, #698]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L306:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L308:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L307:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L309:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L304:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_MarkPicLTRef, .-MVC_MarkPicLTRef
	.align	2
	.global	MVC_MarkSTToLTRef
	.type	MVC_MarkSTToLTRef, %function
MVC_MarkSTToLTRef:
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
	bne	.L311
	ldr	r5, [r6, #648]
	mov	r1, r3
	sub	r5, r5, #1
	rsb	r5, r2, r5
	bl	MVC_UnMarkLTFrmByFrmIdx
.L312:
	mov	r3, r6
	mov	r2, r5
	mov	r1, r8
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MVC_MarkPicLTRef
.L311:
	add	r3, r0, #11075584
	ldr	r5, [r6, #648]
	add	r3, r3, #45056
	ldr	r1, [r3, #2864]
	rsb	r5, r2, r5, lsl #1
	cmp	r1, #0
	beq	.L312
	movw	r2, #47780
	add	r0, r0, #11141120
	movt	r2, 169
	add	r0, r0, #12288
	add	r2, r7, r2
	mov	lr, #0
	b	.L315
.L313:
	tst	r3, #2
	beq	.L314
	ldr	r3, [ip, #768]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L326
.L314:
	cmp	lr, r1
	beq	.L312
.L315:
	ldr	ip, [r2, #4]!
	add	lr, lr, #1
	ldrb	r3, [ip, #3]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L313
	ldr	r4, [ip, #732]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	bne	.L313
	ldr	r4, [ip, #744]
	cmp	r4, r5
	bne	.L313
	ldr	r9, [ip, #56]
	ldr	r4, [r0, #3496]
	cmp	r9, r4
	bne	.L313
	mov	r1, ip
	mov	r3, r8
	mov	r2, #1
	mov	r0, r7
	bl	MVC_UnMarkLTFldByFrmIdx
	b	.L312
.L326:
	ldr	r3, [ip, #780]
	cmp	r3, r5
	bne	.L314
	ldr	r4, [ip, #56]
	ldr	r3, [r0, #3496]
	cmp	r4, r3
	bne	.L314
	mov	r1, ip
	mov	r3, r8
	mov	r2, #2
	mov	r0, r7
	bl	MVC_UnMarkLTFldByFrmIdx
	b	.L312
	UNWIND(.fnend)
	.size	MVC_MarkSTToLTRef, .-MVC_MarkSTToLTRef
	.align	2
	.global	MVC_UpdateMaxLTFrmIdx
	.type	MVC_UpdateMaxLTFrmIdx, %function
MVC_UpdateMaxLTFrmIdx:
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
	add	r5, r0, #11075584
	mov	r8, r0
	add	r5, r5, #45056
	ldr	r3, [r5, #2868]
	str	r1, [r5, #2872]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	movw	r6, #47844
	add	r7, r0, #11141120
	movt	r6, 169
	add	r7, r7, #12288
	add	r6, r0, r6
	mov	r3, r1
	mov	r4, #0
	b	.L331
.L329:
	ldr	r3, [r5, #2868]
	cmp	r3, r4
	ldmlsfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r5, #2872]
.L331:
	ldr	r1, [r6, #4]!
	add	r4, r4, #1
	ldr	r2, [r1, #28]
	cmp	r2, r3
	bcc	.L329
	ldr	r2, [r1, #56]
	ldr	r3, [r7, #3496]
	cmp	r2, r3
	bne	.L329
	mov	r0, r8
	bl	MVC_UnMarkFrameStoreRef
	b	.L329
	UNWIND(.fnend)
	.size	MVC_UpdateMaxLTFrmIdx, .-MVC_UpdateMaxLTFrmIdx
	.align	2
	.global	MVC_UnMarkAllSTRef
	.type	MVC_UnMarkAllSTRef, %function
MVC_UnMarkAllSTRef:
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
	add	r6, r0, #11075584
	mov	r8, r0
	add	r6, r6, #45056
	ldr	r3, [r6, #2864]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	movw	r5, #47780
	add	r7, r0, #11141120
	movt	r5, 169
	add	r7, r7, #12288
	add	r5, r0, r5
	mov	r4, #0
	b	.L335
.L334:
	ldr	r3, [r6, #2864]
	cmp	r3, r4
	bls	.L338
.L335:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	ldr	r3, [r7, #3496]
	ldr	r2, [r1, #56]
	cmp	r2, r3
	bne	.L334
	mov	r0, r8
	bl	MVC_UnMarkFrameStoreRef
	ldr	r3, [r6, #2864]
	cmp	r3, r4
	bhi	.L335
.L338:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_UnMarkAllSTRef, .-MVC_UnMarkAllSTRef
	.align	2
	.global	MVC_MarkCurrPicLT
	.type	MVC_MarkCurrPicLT, %function
MVC_MarkCurrPicLT:
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
	beq	.L342
	mov	r3, r5
	ldr	r1, [r1, #640]
	bl	MVC_UnMarkLTFldByFrmIdx
.L341:
	mov	r2, #1
	mov	r3, #0
	str	r5, [r4, #652]
	strb	r2, [r4, #4]
	strb	r3, [r4, #5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L342:
	mov	r1, r5
	bl	MVC_UnMarkLTFrmByFrmIdx
	b	.L341
	UNWIND(.fnend)
	.size	MVC_MarkCurrPicLT, .-MVC_MarkCurrPicLT
	.align	2
	.global	MVC_RemoveFrameStoreOutDPB
	.type	MVC_RemoveFrameStoreOutDPB, %function
MVC_RemoveFrameStoreOutDPB:
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
	add	r1, r0, r1, lsl #2
	mov	r5, r0
	add	r4, r1, #11075584
	add	r7, r4, #45056
	ldr	r2, [r7, #2664]
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r1, [r2, #52]
	movw	r3, #47928
	add	r6, r0, #11075584
	movt	r3, 169
	add	r1, r0, r1
	add	r0, r6, #45056
	add	r3, r1, r3
	mov	r1, #0
	strb	r1, [r3, #4]
	ldr	r3, [r0, #3088]
	cmp	r3, r1
	subne	r3, r3, #1
	str	r3, [r0, #3088]
	ldrsb	r1, [r2, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L346
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L347
	ldr	r3, [r7, #2664]
	mov	r2, #0
	ldr	r0, [r5, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
.L347:
	ldr	r3, [r7, #2664]
	mov	r2, #0
	ldr	r0, [r5, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
.L346:
	add	r1, r4, #45056
	mov	r3, #0
	add	r6, r6, #45056
	ldr	r2, [r1, #2664]
	strb	r3, [r2, #2]
	ldr	r2, [r1, #2664]
	strb	r3, [r2, #5]
	ldr	r2, [r1, #2664]
	strb	r3, [r2, #3]
	str	r3, [r1, #2664]
	ldr	r3, [r6, #2860]
	sub	r3, r3, #1
	str	r3, [r6, #2860]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_RemoveFrameStoreOutDPB, .-MVC_RemoveFrameStoreOutDPB
	.align	2
	.global	MVC_RemoveUnUsedFrameStore
	.type	MVC_RemoveUnUsedFrameStore, %function
MVC_RemoveUnUsedFrameStore:
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
	add	r3, r0, #11075584
	mov	r8, r0
	add	r3, r3, #45056
	mov	r7, r1
	ldr	r6, [r3, #2856]
	cmp	r6, #0
	beq	.L375
	movw	r5, #47716
	mov	r4, #0
	movt	r5, 169
	add	r5, r0, r5
.L363:
	ldr	r3, [r5, #4]!
	cmp	r3, #0
	beq	.L362
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L362
	ldrb	r2, [r3, #5]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L362
	ldr	r1, [r3, #56]
	cmn	r7, #1
	cmpne	r1, r7
	beq	.L376
.L362:
	add	r4, r4, #1
	cmp	r4, r6
	bne	.L363
.L375:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L376:
	ldr	ip, [r3, #16]
	mov	r1, r4
	mov	r0, r8
	cmp	ip, #1
	beq	.L362
	strb	r2, [r3, #2]
	bl	MVC_RemoveFrameStoreOutDPB
	b	.L362
	UNWIND(.fnend)
	.size	MVC_RemoveUnUsedFrameStore, .-MVC_RemoveUnUsedFrameStore
	.align	2
	.global	MVC_CheckFrameStore
	.type	MVC_CheckFrameStore, %function
MVC_CheckFrameStore:
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
	ldrb	r3, [r0, #6]	@ zero_extendqisi2
	mov	r5, r0
	mov	r4, r1
	cmp	r3, #0
	bne	.L378
	ldrsb	r3, [r1, #8]
	cmp	r3, #1
	bne	.L390
	strb	r3, [r0, #6]
.L378:
	ldrsb	r1, [r4, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L380
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	cmpne	r3, #0
	beq	.L400
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L389
	cmp	r3, #1
	beq	.L389
	cmp	r2, #3
	beq	.L401
	cmp	r2, #1
	beq	.L402
	cmp	r2, #2
	beq	.L403
.L385:
	ldr	r3, [r5, #224]
	ldr	r2, [r0, #168]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	movls	r0, #0
	bhi	.L404
.L398:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L400:
	ldrsb	r2, [r4, #6]
	mov	r0, #1
	ldr	r1, .L405
	bl	dprint_vfmw
.L382:
	ldr	r3, [r4, #16]
	cmp	r3, #1
	mvnne	r0, #2
	moveq	r3, #2
	mvneq	r0, #2
	streq	r3, [r4, #16]
	b	.L398
.L401:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L385
	mov	r2, r3
	ldr	r1, .L405+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L398
.L402:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L385
	mov	r0, r2
	ldr	r1, .L405+8
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L398
.L403:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	tst	r3, #2
	beq	.L385
	ldr	r1, .L405+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L398
.L390:
	mvn	r0, #0
	b	.L398
.L380:
	ldrsb	ip, [r4, #6]
	mov	r2, r4
	ldr	r3, [r4, #16]
	mov	r0, #1
	ldr	r1, .L405+16
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L382
.L389:
	ldr	r1, .L405+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L398
.L404:
	ldr	r1, .L405+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L398
.L406:
	.align	2
.L405:
	.word	.LC4
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC5
	.word	.LC6
	.word	.LC10
	UNWIND(.fnend)
	.size	MVC_CheckFrameStore, .-MVC_CheckFrameStore
	.align	2
	.global	MVC_ExchangePts
	.type	MVC_ExchangePts, %function
MVC_ExchangePts:
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
	add	r8, r1, #672
	mvn	r7, #0
	mvn	r6, #0
	ldrd	r2, [r8]
	mov	r5, r0
	cmp	r3, r7
	cmpeq	r2, r6
	beq	.L407
	add	r9, r0, #11075584
	add	r9, r9, #45056
	ldr	r3, [r9, #2856]
	cmp	r3, #0
	beq	.L407
	movw	r10, #47716
	mvn	r6, #1
	movt	r10, 169
	add	r10, r0, r10
	mvn	r7, #0
	mov	r4, #0
	mvn	r3, #0
	str	r3, [fp, #-48]
.L412:
	ldr	r3, [r10, #4]!
	cmp	r3, #0
	beq	.L411
	ldrsb	r1, [r3, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L411
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	cmpne	r3, #0
	beq	.L411
	ldr	r3, [r10]
	add	r3, r3, #672
	ldrd	r0, [r3]
	cmp	r1, r7
	cmpeq	r0, r6
	strcc	r4, [fp, #-48]
	movcc	r6, r0
	movcc	r7, r1
.L411:
	ldr	r3, [r9, #2856]
	add	r4, r4, #1
	cmp	r4, r3
	bcc	.L412
	ldr	r3, [fp, #-48]
	cmn	r3, #1
	beq	.L407
	ldrd	r2, [r8]
	cmp	r3, r7
	cmpeq	r2, r6
	bhi	.L426
.L407:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L426:
	strd	r6, [sp]
	mov	r0, #29
	ldr	r1, .L427
	bl	dprint_vfmw
	ldr	r3, [fp, #-48]
	movw	r1, #28314
	movt	r1, 42
	add	r1, r3, r1
	ldrd	r2, [r8]
	ldr	r1, [r5, r1, asl #2]
	add	r1, r1, #672
	strd	r2, [r1]
	strd	r6, [r8]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L428:
	.align	2
.L427:
	.word	.LC11
	UNWIND(.fnend)
	.size	MVC_ExchangePts, .-MVC_ExchangePts
	.align	2
	.global	MVC_GetImagePara
	.type	MVC_GetImagePara, %function
MVC_GetImagePara:
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
	add	lr, r0, #11075584
	mov	r5, r0
	add	r0, lr, #40960
	mov	r4, r1
	ldr	r2, [r5, #252]
	mov	r6, #2240
	ldr	r1, [r0, #1016]
	movw	ip, #64764
	ldr	r3, [r4, #180]
	movt	ip, 65533
	ldr	r7, [r5, #248]
	movw	r8, #3992
	mla	r2, r6, r1, r2
	and	ip, ip, r3
	ldrb	r6, [r4, #2]	@ zero_extendqisi2
	ldr	r2, [r2, #28]
	str	ip, [r4, #180]
	ldr	r3, [r5, #224]
	mla	r1, r8, r2, r7
	ldr	r3, [r3, #592]
	cmp	r3, #1
	moveq	r3, #262144
	movne	r3, #0
	orr	r3, r3, ip
	str	r3, [r4, #180]
	ldr	r2, [r5, #56]
	mov	r2, r2, asl #10
	str	r2, [r4, #208]
	cmp	r6, #3
	ldrls	pc, [pc, r6, asl #2]
	b	.L469
.L433:
	.word	.L432
	.word	.L434
	.word	.L435
	.word	.L432
.L435:
	ldrb	ip, [r4]	@ zero_extendqisi2
	add	r2, r5, #11075584
	ldr	lr, [r4, #44]
	add	r2, r2, #45056
	and	ip, ip, #3
	orr	ip, ip, #2816
	orr	r3, ip, r3
	str	lr, [r4, #232]
	str	r3, [r4, #180]
.L431:
	ldrb	r3, [r1, #20]	@ zero_extendqisi2
	str	r3, [r4, #548]
	str	r3, [r4, #552]
	ldrb	r3, [r1, #24]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r1, #68]
	str	r3, [r4, #560]
	mov	r3, #1
	str	r3, [r4, #556]
	ldrb	r3, [r0, #1011]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r3, #0
	str	r3, [r4, #272]
	ldr	r3, [r5, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ble	.L453
	ldrb	r3, [r4, #696]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L453
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cmp	r3, #3
	ldreq	r3, [r4, #180]
	biceq	r3, r3, #768
	streq	r3, [r4, #180]
.L453:
	ldrb	r3, [r2, #2616]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L454
	ldr	r3, [r2, #2632]
	cmp	r3, #3
	moveq	r3, #1
	streq	r3, [r4, #428]
	beq	.L454
	cmp	r3, #4
	moveq	r3, #2
	streq	r3, [r4, #428]
	beq	.L454
	cmp	r3, #5
	moveq	r3, #3
	streq	r3, [r4, #428]
	beq	.L454
	mov	r0, #0
	ldr	r1, .L481
	str	r0, [r4, #428]
	ldr	r2, [r2, #2632]
	bl	dprint_vfmw
.L454:
	ldr	r0, [r5, #120]
	ldrsb	r1, [r4, #6]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L481+4
	mov	r2, #624
	add	r1, r4, #72
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #15
	str	r3, [r5, #376]
	mov	r2, #1
	ldrb	r3, [r4, #696]	@ zero_extendqisi2
	strb	r2, [r5, #371]
	cmp	r3, #0
	movne	r3, #4
	strb	r3, [r5, #364]
	ldr	r3, [r4, #724]
	strb	r2, [r5, #369]
	str	r3, [r5, #380]
	str	r3, [r5, #388]
	str	r3, [r5, #384]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L434:
	ldrb	ip, [r4]	@ zero_extendqisi2
	add	r2, r5, #11075584
	ldr	lr, [r4, #44]
	add	r2, r2, #45056
	and	ip, ip, #3
	orr	ip, ip, #1792
	orr	r3, ip, r3
	str	lr, [r4, #232]
	str	r3, [r4, #180]
	b	.L431
.L432:
	ldrb	r2, [r4, #696]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L436
	ldr	ip, [r4, #40]
	mov	r7, #3072
	ldr	r2, [r4, #44]
	cmp	ip, #0
	ldrb	ip, [r4]	@ zero_extendqisi2
	moveq	r6, #512
	movne	r6, #768
	str	r2, [r4, #232]
.L437:
	ldr	r8, [r4, #748]
	ldr	r2, [r4, #784]
	cmp	r8, r2
	beq	.L478
	movle	r9, #1
	movgt	r9, #0
.L448:
	add	r2, lr, #45056
	ldrb	r8, [r2, #2652]	@ zero_extendqisi2
	cmp	r8, #3
	moveq	lr, #4096
	beq	.L449
	cmp	r8, #4
	movne	lr, r9, asl #12
	moveq	lr, #0
.L449:
	orr	r6, r7, r6
	bic	r3, r3, #12288
	orr	r3, r6, r3
	and	ip, ip, #3
	orr	ip, r3, ip
	orr	ip, ip, lr
	str	ip, [r4, #180]
	b	.L431
.L436:
	ldr	r8, [r4, #756]
	ldr	r2, [r5, #116]
	cmp	r8, r2
	bhi	.L479
	ldr	ip, [r4, #792]
	cmp	r2, ip
	bcc	.L441
.L439:
	ldr	r2, [r4, #44]
	mov	r6, #3
	ldrb	ip, [r4]	@ zero_extendqisi2
	ldrb	r10, [r4, #735]	@ zero_extendqisi2
	ldrb	r9, [r4, #771]	@ zero_extendqisi2
	str	r2, [r4, #232]
.L442:
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L480
	cmp	r2, #2
	streq	r8, [r4, #232]
	movne	r7, r6, asl #10
	moveq	ip, r10
	moveq	r7, #1024
.L444:
	mov	r6, #768
	str	r10, [r4, #172]
	str	r9, [r4, #176]
	b	.L437
.L441:
	ldrb	ip, [r4, #735]	@ zero_extendqisi2
	mov	r6, #1
	ldrb	r9, [r4, #771]	@ zero_extendqisi2
	str	r8, [r4, #232]
	mov	r10, ip
	b	.L442
.L478:
	ldr	r2, [r4, #184]
	cmp	r2, #720
	movne	r9, #1
	bne	.L448
	ldr	r9, [r4, #188]
	subs	r9, r9, #480
	movne	r9, #1
	b	.L448
.L479:
	ldr	r7, [r4, #792]
	cmp	r2, r7
	bcc	.L439
	ldrb	ip, [r4, #771]	@ zero_extendqisi2
	mov	r6, #2
	str	r7, [r4, #232]
	ldrb	r10, [r4, #735]	@ zero_extendqisi2
	mov	r9, ip
	b	.L442
.L480:
	ldr	r2, [r4, #792]
	mov	ip, r9
	mov	r7, #2048
	str	r2, [r4, #232]
	b	.L444
.L469:
	add	r2, r5, #11075584
	add	r2, r2, #45056
	b	.L431
.L482:
	.align	2
.L481:
	.word	.LC12
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MVC_GetImagePara, .-MVC_GetImagePara
	.align	2
	.global	MVC_SetFrmRepeatCount
	.type	MVC_SetFrmRepeatCount, %function
MVC_SetFrmRepeatCount:
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
	ldr	r3, [r0, #224]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r1, #24]
	ldmccfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_SetFrmRepeatCount.part.1
	UNWIND(.fnend)
	.size	MVC_SetFrmRepeatCount, .-MVC_SetFrmRepeatCount
	.align	2
	.global	MVC_SplitFrmToFlds
	.type	MVC_SplitFrmToFlds, %function
MVC_SplitFrmToFlds:
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
	ldrb	lr, [r0, #697]	@ zero_extendqisi2
	mov	r5, #1
	ldrb	ip, [r0, #698]	@ zero_extendqisi2
	mov	r4, #2
	ldr	r1, [r0, #720]
	ldrb	r2, [r0, #699]	@ zero_extendqisi2
	ldr	r3, [r0, #700]
	strb	r5, [r0, #732]
	strb	r4, [r0, #768]
	strb	lr, [r0, #733]
	strb	lr, [r0, #769]
	strb	ip, [r0, #734]
	strb	ip, [r0, #770]
	str	r1, [r0, #756]
	str	r1, [r0, #792]
	strb	r2, [r0, #735]
	strb	r2, [r0, #771]
	str	r3, [r0, #736]
	str	r3, [r0, #772]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_SplitFrmToFlds, .-MVC_SplitFrmToFlds
	.align	2
	.global	MVC_CombineFldsToFrm
	.type	MVC_CombineFldsToFrm, %function
MVC_CombineFldsToFrm:
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
	ldr	r2, [r0, #784]
	mov	lr, #3
	ldr	r1, [r0, #748]
	ldr	r3, [r0, #792]
	cmp	r2, r1
	ldr	r5, [r0, #756]
	ldrb	r4, [r0, #769]	@ zero_extendqisi2
	ldr	ip, [r0, #736]
	movge	r2, r1
	add	r3, r3, r5
	cmp	r4, #0
	strb	lr, [r0, #696]
	mov	r3, r3, lsr #1
	str	ip, [r0, #700]
	str	r2, [r0, #712]
	str	r2, [r0, #32]
	str	r3, [r0, #720]
	beq	.L487
	ldrb	r2, [r0, #733]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L488
.L487:
	ldr	r2, [r0, #768]
	mov	r1, #0
	strb	r1, [r0, #697]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, r1
	beq	.L492
	ldr	r2, [r0, #732]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	movne	ip, #1
	bne	.L490
.L492:
	mov	ip, #0
.L490:
	ldrb	r2, [r0, #771]	@ zero_extendqisi2
	ldrb	r1, [r0, #735]	@ zero_extendqisi2
	str	r3, [r0, #44]
	cmp	r2, r1
	strb	ip, [r0, #698]
	movcs	r3, r2
	movcc	r3, r1
	strb	r3, [r0, #699]
	strb	r3, [r0]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L488:
	mov	r1, #1
	strb	r1, [r0, #697]
	b	.L492
	UNWIND(.fnend)
	.size	MVC_CombineFldsToFrm, .-MVC_CombineFldsToFrm
	.align	2
	.global	MVC_GetAPC
	.type	MVC_GetAPC, %function
MVC_GetAPC:
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
	subs	r6, r1, #0
	mov	r5, r0
	beq	.L523
	cmp	r2, #1
	beq	.L504
	cmp	r2, #2
	beq	.L505
	ldr	r9, [r6, #748]
	ldr	r10, [r6, #784]
.L506:
	add	r8, r5, #11075584
	add	r7, r8, #45056
	ldr	ip, [r7, #3084]
	cmp	ip, #0
	beq	.L515
	ldrb	r4, [r7, #2876]	@ zero_extendqisi2
	cmp	r4, #0
	movwne	r3, #47932
	movne	r4, #0
	movtne	r3, 169
	addne	r3, r5, r3
	bne	.L509
	b	.L507
.L513:
	ldrb	r2, [r3, #1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L507
.L509:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L513
.L515:
	mvn	r0, #0
.L519:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L505:
	ldr	r9, [r6, #784]
	mov	r10, r9
	b	.L506
.L507:
	ldrsb	r1, [r6, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L510
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L510
	add	r2, r5, r4
	movw	r3, #47928
	movt	r3, 169
	add	r3, r2, r3
	mov	r2, #1
	add	r5, r5, r4, lsl #2
	strb	r2, [r3, #4]
	add	r5, r5, #11075584
	ldr	r3, [r0, #640]
	add	r5, r5, #45056
	add	r8, r8, #45056
	ldr	r1, .L524
	mov	r0, #13
	ldr	r3, [r3, #8]
	str	r9, [r5, #2956]
	str	r10, [r5, #3020]
	str	r3, [r5, #2892]
	ldr	r3, [r7, #3088]
	cmp	r3, #15
	addls	r2, r3, r2
	mov	r3, r4
	movhi	r2, #16
	str	r2, [r8, #3088]
	ldr	r2, [r6, #228]
	bl	dprint_vfmw
	mov	r0, #0
	str	r4, [r6, #52]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L504:
	ldr	r9, [r6, #748]
	mov	r10, r9
	b	.L506
.L510:
	mov	r1, #0
	ldrsb	r2, [r6, #6]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	stmia	sp, {r0, r1}
	mov	r0, r1
	ldr	r1, .L524+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L519
.L523:
	mov	r0, r6
	movw	r3, #2085
	ldr	r2, .L524+8
	ldr	r1, .L524+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L519
.L525:
	.align	2
.L524:
	.word	.LC15
	.word	.LC16
	.word	.LC13
	.word	.LC14
	UNWIND(.fnend)
	.size	MVC_GetAPC, .-MVC_GetAPC
	.align	2
	.global	MVC_SlidingWinMark
	.type	MVC_SlidingWinMark, %function
MVC_SlidingWinMark:
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
	add	r3, r0, #11075584
	ldr	r2, [r0, #236]
	add	r3, r3, #45056
	mov	r10, r0
	ldr	r0, [r3, #2868]
	ldr	r2, [r2, #3944]
	ldr	r1, [r3, #2864]
	rsb	r2, r0, r2
	cmp	r1, r2
	beq	.L551
.L526:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L551:
	ldr	r8, [r3, #2856]
	cmp	r8, #0
	beq	.L531
	movw	r6, #47716
	add	r1, r10, #11141120
	movt	r6, 169
	mov	r2, #0
	add	r1, r1, #12288
	add	r6, r10, r6
	mov	r5, r2
	mvn	r9, #-2147483648
.L530:
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	beq	.L529
	ldr	r7, [r4, #64]
	cmp	r9, r7
	bls	.L529
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	mov	r0, r4
	cmp	r3, #0
	beq	.L529
	str	r1, [fp, #-52]
	str	r2, [fp, #-48]
	bl	MVC_NonLongTermRefFlg
	ldr	r2, [fp, #-48]
	ldr	r1, [fp, #-52]
	cmp	r0, #0
	beq	.L529
	ldr	r0, [r4, #56]
	ldr	r3, [r1, #3496]
	cmp	r0, r3
	moveq	r9, r7
	moveq	r2, r5
.L529:
	add	r5, r5, #1
	cmp	r5, r8
	bne	.L530
.L528:
	movw	r3, #28314
	movt	r3, 42
	add	r3, r2, r3
	ldr	r4, [r10, r3, asl #2]
	cmp	r4, #0
	beq	.L526
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L526
	mov	r0, r4
	bl	MVC_NonLongTermRefFlg
	cmp	r0, #0
	beq	.L526
	add	r3, r10, #11141120
	ldr	r2, [r4, #56]
	add	r3, r3, #12288
	ldr	r3, [r3, #3496]
	cmp	r2, r3
	bne	.L526
	mov	r0, r10
	mov	r1, r4
	bl	MVC_UnMarkFrameStoreRef
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	MVC_UpdateReflist
.L531:
	mov	r2, r8
	b	.L528
	UNWIND(.fnend)
	.size	MVC_SlidingWinMark, .-MVC_SlidingWinMark
	.align	2
	.global	MVC_DumpDPB
	.type	MVC_DumpDPB, %function
MVC_DumpDPB:
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
	ldr	r3, .L562
	mov	r7, r0
	ldr	r3, [r3]
	tst	r3, #16384
	beq	.L552
	add	r6, r0, #11075584
	add	r6, r6, #45056
	ldr	r3, [r6, #2860]
	cmp	r3, #0
	movwne	r5, #47716
	movne	r4, #0
	movtne	r5, 169
	addne	r5, r0, r5
	beq	.L558
.L557:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L562+4
	mov	r0, #14
	add	r4, r4, #1
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	ldr	r3, [ip, #20]
	str	lr, [sp, #4]
	ldr	ip, [ip, #32]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r6, #2860]
	cmp	r3, r4
	bhi	.L557
.L558:
	ldr	r1, .L562+8
	mov	r0, #14
	bl	dprint_vfmw
	ldr	r3, [r6, #2864]
	cmp	r3, #0
	movwne	r5, #47780
	movne	r4, #0
	movtne	r5, 169
	addne	r5, r7, r5
	beq	.L556
.L559:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L562+12
	mov	r0, #14
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	ldr	r3, [ip, #20]
	str	lr, [sp, #4]
	ldr	ip, [ip, #32]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r5]
	mov	r2, r4
	ldr	r1, .L562+16
	mov	r0, #14
	add	r4, r4, #1
	ldr	r3, [r3, #712]
	bl	dprint_vfmw
	ldr	r3, [r6, #2864]
	cmp	r3, r4
	bhi	.L559
.L556:
	ldr	r1, .L562+8
	mov	r0, #14
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L552:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L563:
	.align	2
.L562:
	.word	g_PrintEnable
	.word	.LC18
	.word	.LC17
	.word	.LC19
	.word	.LC20
	UNWIND(.fnend)
	.size	MVC_DumpDPB, .-MVC_DumpDPB
	.align	2
	.global	MVC_UpdateCurrFrameInfo
	.type	MVC_UpdateCurrFrameInfo, %function
MVC_UpdateCurrFrameInfo:
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
	add	r0, r0, #11141120
	add	r3, r0, #12288
	ldrb	r2, [r3, #2778]	@ zero_extendqisi2
	ldr	r4, [r3, #3416]
	strb	r2, [r4, #1]
	ldrb	r2, [r3, #2780]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
	bne	.L565
	ldrb	r2, [r3, #2781]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
.L565:
	strb	r2, [r4, #3]
	ldr	r2, [r3, #3424]
	str	r2, [r4, #20]
	ldr	r2, [r3, #3468]
	str	r2, [r4, #44]
	ldrb	r2, [r3, #2788]	@ zero_extendqisi2
	strb	r2, [r4]
	ldr	r2, [r3, #3436]
	str	r2, [r4, #32]
	ldr	r2, [r3, #3492]
	str	r2, [r4, #48]
	ldr	r2, [r3, #3428]
	str	r2, [r4, #28]
	ldrb	r2, [r3, #2785]	@ zero_extendqisi2
	str	r2, [r4, #40]
	ldrb	r2, [r3, #2783]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r1, #3
	movne	r1, #0
	strb	r1, [r4, #4]
	ldrb	r2, [r3, #2779]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L568
	bcc	.L569
	cmp	r2, #2
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	str	r4, [r4, #772]
	mov	ip, #1
	strb	ip, [r4, #768]
	mov	r6, #0
	ldrb	ip, [r3, #2780]	@ zero_extendqisi2
	strb	ip, [r4, #769]
	ldrb	ip, [r3, #2781]	@ zero_extendqisi2
	strb	ip, [r4, #770]
	ldr	ip, [r3, #3444]
	str	ip, [r4, #784]
	ldrb	ip, [r3, #2788]	@ zero_extendqisi2
	strb	ip, [r4, #771]
	ldr	lr, [r3, #3468]
	str	r6, [r4, #40]
	str	lr, [r4, #792]
	ldrb	r5, [r3, #2777]	@ zero_extendqisi2
	cmp	r5, r6
	beq	.L586
	ldr	r2, [r4, #756]
	mov	r5, #3
	strb	r5, [r4, #2]
	add	r3, r3, #3392
	add	lr, lr, r2
	str	lr, [r4, #44]
	str	lr, [r4, #720]
	mvn	r7, #0
	ldrb	r2, [r3, #-609]	@ zero_extendqisi2
	cmp	r2, #1
	orreq	r6, r1, #2
	strb	r6, [r4, #4]
	ldrd	r8, [r3]
	mvn	r6, #0
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L577
	add	r1, r4, #672
	ldrd	r2, [r1]
	cmp	r3, r7
	cmpeq	r2, r6
	beq	.L587
.L577:
	ldrb	r3, [r4, #735]	@ zero_extendqisi2
.L584:
	add	r1, r0, #12288
	cmp	r3, ip
	mov	r0, r4
	movcc	r3, ip
	strb	r3, [r4]
	ldr	r3, [r1, #3436]
	str	r3, [r4, #32]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MVC_CombineFldsToFrm
.L569:
	str	r4, [r4, #700]
	mov	r1, #3
	mov	r2, #0
	strb	r1, [r4, #2]
	strb	r2, [r4, #696]
	add	r1, r3, #2784
	ldrb	r5, [r3, #2780]	@ zero_extendqisi2
	mov	r2, #624
	add	r1, r1, #8
	add	r0, r4, #72
	strb	r5, [r4, #697]
	ldrb	r6, [r3, #2781]	@ zero_extendqisi2
	strb	r6, [r4, #698]
	ldr	ip, [r3, #3440]
	str	ip, [r4, #712]
	ldrb	r7, [r3, #2788]	@ zero_extendqisi2
	strb	r7, [r4, #699]
	ldr	r8, [r3, #3468]
	str	r8, [r4, #720]
	ldr	ip, [r3, #3444]
	str	ip, [r4, #748]
	ldr	r3, [r3, #3448]
	str	r3, [r4, #784]
	bl	memcpy
	mov	r2, #1
	mov	r3, #2
	str	r8, [r4, #756]
	str	r8, [r4, #792]
	strb	r7, [r4, #735]
	strb	r7, [r4, #771]
	strb	r6, [r4, #734]
	strb	r6, [r4, #770]
	strb	r5, [r4, #733]
	strb	r5, [r4, #769]
	str	r4, [r4, #736]
	str	r4, [r4, #772]
	strb	r2, [r4, #732]
	strb	r3, [r4, #768]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L568:
	strb	r2, [r4, #732]
	mov	r5, #0
	str	r4, [r4, #736]
	ldrb	ip, [r3, #2780]	@ zero_extendqisi2
	strb	ip, [r4, #733]
	ldrb	ip, [r3, #2781]	@ zero_extendqisi2
	strb	ip, [r4, #734]
	ldr	ip, [r3, #3444]
	str	ip, [r4, #748]
	ldrb	ip, [r3, #2788]	@ zero_extendqisi2
	strb	ip, [r4, #735]
	ldr	lr, [r3, #3468]
	str	r5, [r4, #40]
	str	lr, [r4, #756]
	ldrb	r6, [r3, #2777]	@ zero_extendqisi2
	cmp	r6, r5
	beq	.L588
	ldr	r2, [r4, #792]
	mov	r6, #3
	strb	r6, [r4, #2]
	add	r3, r3, #3392
	add	lr, lr, r2
	str	lr, [r4, #44]
	str	lr, [r4, #720]
	mvn	r7, #0
	ldrb	r2, [r3, #-609]	@ zero_extendqisi2
	mvn	r6, #0
	cmp	r2, #1
	orreq	r5, r1, #1
	strb	r5, [r4, #4]
	ldrd	r8, [r3]
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L573
	add	r1, r4, #672
	ldrd	r2, [r1]
	cmp	r3, r7
	cmpeq	r2, r6
	streqd	r8, [r1]
.L573:
	ldrb	r3, [r4, #771]	@ zero_extendqisi2
	b	.L584
.L586:
	strb	r2, [r4, #2]
	add	r1, r0, #15040
	str	lr, [r4, #44]
	add	r1, r1, #40
	str	lr, [r4, #720]
	add	r0, r4, #72
	ldrb	r3, [r3, #2783]	@ zero_extendqisi2
	mov	r2, #624
	cmp	r3, #1
	moveq	r5, #2
	strb	r5, [r4, #4]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L588:
	strb	r2, [r4, #2]
	add	r1, r3, #2784
	str	lr, [r4, #44]
	add	r1, r1, #8
	str	lr, [r4, #720]
	add	r0, r4, #72
	ldrb	r3, [r3, #2783]	@ zero_extendqisi2
	mov	r2, #624
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r4, #4]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L587:
	strd	r8, [r1]
	b	.L577
	UNWIND(.fnend)
	.size	MVC_UpdateCurrFrameInfo, .-MVC_UpdateCurrFrameInfo
	.align	2
	.global	MVC_SimpleSlideDPB
	.type	MVC_SimpleSlideDPB, %function
MVC_SimpleSlideDPB:
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
	add	r3, r0, #11075584
	mov	r7, r0
	add	r3, r3, #45056
	ldr	r5, [r3, #2856]
	ldr	r3, [r3, #2860]
	cmp	r3, r5
	addcc	r6, r0, #11141120
	bcc	.L590
	cmp	r5, #0
	beq	.L595
	movw	ip, #47716
	add	r6, r0, #11141120
	movt	ip, 169
	mov	r1, #0
	add	r8, r6, #12288
	add	ip, r0, ip
	mov	r3, r1
	mvn	r4, #-2147483648
.L593:
	ldr	r2, [ip, #4]!
	cmp	r2, #0
	beq	.L592
	ldr	lr, [r2, #64]
	cmp	r4, lr
	bls	.L592
	ldr	r0, [r2, #56]
	ldr	r2, [r8, #3496]
	cmp	r0, r2
	moveq	r4, lr
	moveq	r1, r3
.L592:
	add	r3, r3, #1
	cmp	r3, r5
	bne	.L593
	mov	r5, r1
.L591:
	movw	r3, #28314
	mov	r0, r7
	movt	r3, 42
	add	r3, r1, r3
	ldr	r1, [r7, r3, asl #2]
	bl	MVC_UnMarkFrameStoreRef
	mov	r1, r5
	mov	r0, r7
	bl	MVC_RemoveFrameStoreOutDPB
.L590:
	add	r3, r6, #12288
	mov	r1, #0
	ldr	r2, [r3, #3432]
	strb	r1, [r3, #2780]
	cmp	r2, r1
	movne	r2, #1
	strneb	r2, [r3, #2781]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L595:
	add	r6, r0, #11141120
	mov	r1, r5
	b	.L591
	UNWIND(.fnend)
	.size	MVC_SimpleSlideDPB, .-MVC_SimpleSlideDPB
	.align	2
	.global	MVC_ReleaseNAL
	.type	MVC_ReleaseNAL, %function
MVC_ReleaseNAL:
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
	strb	r3, [r5, #3]
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
	movw	r3, #3998
	ldr	r2, .L614+4
	ldr	r1, .L614+8
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L615:
	.align	2
.L614:
	.word	.LC21
	.word	.LC13
	.word	.LC14
	UNWIND(.fnend)
	.size	MVC_ReleaseNAL, .-MVC_ReleaseNAL
	.align	2
	.global	MVC_ClearCurrNal
	.type	MVC_ClearCurrNal, %function
MVC_ClearCurrNal:
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
	ldr	r1, [r0, #232]
	mov	r4, r0
	cmp	r1, #0
	beq	.L617
	ldr	r0, [r0, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L617:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_ClearCurrNal, .-MVC_ClearCurrNal
	.align	2
	.global	MVC_ClearCurrSlice
	.type	MVC_ClearCurrSlice, %function
MVC_ClearCurrSlice:
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
	mov	r4, r0
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L622
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L622:
	ldr	r3, [r4, #80]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #80]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L627:
	.align	2
.L626:
	.word	.LC22
	UNWIND(.fnend)
	.size	MVC_ClearCurrSlice, .-MVC_ClearCurrSlice
	.align	2
	.global	MVC_ClearAllNal
	.type	MVC_ClearAllNal, %function
MVC_ClearAllNal:
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
	add	r4, r0, #936
	add	r5, r0, #12992
	b	.L630
.L629:
	add	r4, r4, #88
	cmp	r4, r5
	beq	.L632
.L630:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L629
	mov	r1, r4
	ldr	r0, [r6, #120]
	add	r4, r4, #88
	bl	MVC_ReleaseNAL
	cmp	r4, r5
	bne	.L630
.L632:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_ClearAllNal, .-MVC_ClearAllNal
	.align	2
	.global	MVC_ClearAllSlice
	.type	MVC_ClearAllSlice, %function
MVC_ClearAllSlice:
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
	movw	r4, #16544
	movw	r5, #17088
	movt	r4, 170
	movt	r5, 170
	add	r4, r0, r4
	add	r5, r0, r5
	mov	r6, r0
	mov	r7, #0
.L635:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L634
	ldr	r0, [r6, #120]
	bl	MVC_ReleaseNAL
	str	r7, [r4]
.L634:
	cmp	r4, r5
	bne	.L635
	mov	r0, #0
	str	r0, [r6, #64]
	str	r0, [r6, #104]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_ClearAllSlice, .-MVC_ClearAllSlice
	.align	2
	.global	MVC_ClearCurrPic
	.type	MVC_ClearCurrPic, %function
MVC_ClearCurrPic:
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
	bl	MVC_ClearAllSlice
	ldr	r0, [r4, #40]
	add	r3, r4, #11075584
	add	r3, r3, #40960
	mov	r2, #0
	movw	r1, #23352
	movt	r1, 1
	str	r0, [r3, #2668]
	str	r1, [r3, #2672]
	mov	r0, #2
	mov	r1, #7
	strb	r0, [r3, #2657]
	strb	r1, [r3, #2659]
	mov	r0, #32
	mov	r1, #3
	strb	r0, [r3, #2658]
	str	r1, [r3, #2680]
	mvn	r0, #0
	mov	r1, #262144
	str	r0, [r3, #2684]
	str	r1, [r3, #2696]
	mov	r0, r2
	strb	r2, [r3, #2656]
	str	r2, [r3, #2688]
	str	r2, [r3, #2692]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_ClearCurrPic, .-MVC_ClearCurrPic
	.align	2
	.global	MVC_ArrangeVahbMem
	.type	MVC_ArrangeVahbMem, %function
MVC_ArrangeVahbMem:
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
	ldr	r3, [r4, #224]
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
	add	r2, r4, #11075584
	ldrb	r3, [fp, #4]	@ zero_extendqisi2
	add	r2, r2, #45056
	mov	r1, #5
	strb	r0, [fp, #-56]
	ldr	r2, [r2, #2856]
	strb	r1, [fp, #-55]
	add	r2, r2, #3
	strb	r2, [fp, #-57]
.L648:
	sub	r1, fp, #60
	ldr	r0, [r4, #120]
	mov	r5, #0
	strb	r3, [fp, #-54]
	strb	r5, [fp, #-58]
	bl	FSP_ConfigInstance
	cmp	r0, r5
	bne	.L643
	ldr	r1, [r4, #224]
	sub	r3, fp, #64
	ldr	r0, [r4, #120]
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
	add	r0, r4, #584
	bl	ResetVoQueue
.L645:
	ldr	r1, .L651+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L649
.L643:
	mov	r0, r5
	ldr	r1, .L651+8
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L649
.L652:
	.align	2
.L651:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC23
	.word	.LC24
	UNWIND(.fnend)
	.size	MVC_ArrangeVahbMem, .-MVC_ArrangeVahbMem
	.align	2
	.global	MVC_RepairList
	.type	MVC_RepairList, %function
MVC_RepairList:
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
	add	r3, r0, #11075584
	mov	r7, r0
	add	r3, r3, #40960
	ldrb	r3, [r3, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L655
	bcc	.L670
	cmp	r3, #2
	bne	.L676
.L671:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L667:
	mov	r0, r7
.L676:
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L655:
	mov	r0, #2
.L656:
	movw	r6, #42004
	add	r8, r7, #11141120
	movt	r6, 169
	add	r6, r7, r6
	add	r8, r8, #12288
	add	r5, r7, #256
	ldr	r2, [r6, #4]!
	mov	lr, r7
	mov	r4, #0
	mov	r9, #1
	cmp	r2, #0
	beq	.L658
.L678:
	ldr	r10, [lr, #256]
	cmp	r10, #0
	bne	.L672
	mov	ip, r5
	mov	r3, r10
	b	.L660
.L661:
	ldr	r1, [ip, #4]!
	cmp	r1, #0
	bne	.L663
.L660:
	add	r3, r3, #1
	cmp	r3, r2
	bne	.L661
	ldr	r3, [r7, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L667
	ldr	r1, [r8, #3416]
	add	r1, r1, #696
.L663:
	mov	ip, r5
	mov	r3, #0
	b	.L669
.L665:
	ldr	r10, [ip, #4]!
.L669:
	add	r3, r3, #1
	cmp	r10, #0
	streq	r1, [ip]
	cmp	r3, r2
	bne	.L665
.L666:
	add	r4, r4, #1
	add	r5, r5, #132
	cmp	r0, r4
	add	lr, lr, #132
	bls	.L671
	ldr	r2, [r6, #4]!
	cmp	r2, #0
	bne	.L678
.L658:
	ldr	r3, [r7, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L667
	str	r9, [r6]
	ldr	r3, [r8, #3416]
	add	r3, r3, #696
	str	r3, [lr, #256]
	b	.L666
.L672:
	mov	r1, r10
	b	.L663
.L670:
	mov	r0, #1
	b	.L656
	UNWIND(.fnend)
	.size	MVC_RepairList, .-MVC_RepairList
	.align	2
	.global	MVC_GetShortTermPicPoint
	.type	MVC_GetShortTermPicPoint, %function
MVC_GetShortTermPicPoint:
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
	add	r3, r0, #11075584
	add	r2, r3, #40960
	ldrb	r4, [r2, #1001]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L680
	ldrb	r2, [r2, #1002]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r4, #2
	moveq	r4, #1
.L680:
	add	r3, r3, #45056
	ldr	r5, [r3, #2864]
	cmp	r5, #0
	beq	.L688
	movw	r3, #47784
	mov	r2, #0
	movt	r3, 169
	add	r3, r0, r3
	b	.L685
.L698:
	ldrb	ip, [r0, #3]	@ zero_extendqisi2
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
	ldrb	lr, [r0, #3]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L684
	ldr	ip, [r0, #732]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	beq	.L699
.L684:
	tst	lr, #2
	beq	.L683
	ldr	ip, [r0, #768]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L683
	ldr	ip, [r0, #780]
	cmp	ip, r1
	bne	.L683
	add	r0, r0, #768
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L696:
	ldr	ip, [r0, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L683
	ldr	ip, [r0, #708]
	cmp	ip, r1
	bne	.L683
	add	r0, r0, #696
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L699:
	ldr	ip, [r0, #744]
	cmp	ip, r1
	bne	.L684
	add	r0, r0, #732
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L697:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L688:
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GetShortTermPicPoint, .-MVC_GetShortTermPicPoint
	.align	2
	.global	MVC_GetLongTermPicPoint
	.type	MVC_GetLongTermPicPoint, %function
MVC_GetLongTermPicPoint:
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
	add	r3, r0, #11075584
	add	r2, r3, #40960
	ldrb	r4, [r2, #1001]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L701
	ldrb	r2, [r2, #1002]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r4, #2
	moveq	r4, #1
.L701:
	add	r3, r3, #45056
	ldr	r5, [r3, #2868]
	cmp	r5, #0
	beq	.L709
	movw	r3, #47848
	mov	r2, #0
	movt	r3, 169
	add	r3, r0, r3
	b	.L706
.L719:
	ldrb	ip, [r0, #3]	@ zero_extendqisi2
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
	ldrb	lr, [r0, #3]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L705
	ldr	ip, [r0, #732]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	beq	.L720
.L705:
	tst	lr, #2
	beq	.L704
	ldr	ip, [r0, #768]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L704
	ldr	ip, [r0, #776]
	cmp	ip, r1
	bne	.L704
	add	r0, r0, #768
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L717:
	ldr	ip, [r0, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L704
	ldr	ip, [r0, #704]
	cmp	ip, r1
	bne	.L704
	add	r0, r0, #696
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L720:
	ldr	ip, [r0, #740]
	cmp	ip, r1
	bne	.L705
	add	r0, r0, #732
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L718:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L709:
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GetLongTermPicPoint, .-MVC_GetLongTermPicPoint
	.align	2
	.global	MVC_ReorderSTList
	.type	MVC_ReorderSTList, %function
MVC_ReorderSTList:
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
	mov	ip, r1, asl #7
	ldr	r7, [fp, #4]
	add	r6, ip, r1, lsl #2
	mov	r1, r3
	add	r6, r6, #256
	mov	r5, r3
	mov	r8, r2
	add	r6, r0, r6
	bl	MVC_GetShortTermPicPoint
	ldr	r4, [r7]
	add	r3, r8, #1
	ldr	r1, [fp, #8]
	cmp	r3, r4
	ble	.L722
	add	lr, r6, r3, lsl #2
	mov	ip, r3
.L723:
	ldr	r4, [lr, #-4]!
	sub	ip, ip, #1
	str	r4, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L723
.L722:
	add	r2, r4, #1
	str	r2, [r7]
	str	r0, [r6, r4, asl #2]
	ldr	ip, [r7]
	cmp	r3, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r6, ip, lsl #2
	mov	lr, ip
	b	.L727
.L726:
	str	r0, [r6, lr, asl #2]
	add	lr, lr, #1
.L725:
	cmp	ip, r2
	beq	.L735
.L727:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L725
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L726
	ldr	r3, [r0, #12]
	cmp	r3, r5
	bne	.L726
	ldr	r3, [r0, #4]
	ldr	r3, [r3, #56]
	cmp	r3, r1
	bne	.L726
	cmp	ip, r2
	bne	.L727
.L735:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_ReorderSTList, .-MVC_ReorderSTList
	.align	2
	.global	MVC_ReorderLTList
	.type	MVC_ReorderLTList, %function
MVC_ReorderLTList:
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
	mov	ip, r1, asl #7
	ldr	r7, [fp, #4]
	add	r6, ip, r1, lsl #2
	mov	r1, r3
	add	r6, r6, #256
	mov	r5, r3
	mov	r8, r2
	add	r6, r0, r6
	bl	MVC_GetLongTermPicPoint
	ldr	r4, [r7]
	add	r3, r8, #1
	ldr	r1, [fp, #8]
	cmp	r3, r4
	ble	.L737
	add	lr, r6, r3, lsl #2
	mov	ip, r3
.L738:
	ldr	r4, [lr, #-4]!
	sub	ip, ip, #1
	str	r4, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L738
.L737:
	add	r2, r4, #1
	str	r2, [r7]
	str	r0, [r6, r4, asl #2]
	ldr	ip, [r7]
	cmp	r3, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r6, ip, lsl #2
	mov	lr, ip
	b	.L742
.L741:
	str	r0, [r6, lr, asl #2]
	add	lr, lr, #1
.L740:
	cmp	ip, r2
	beq	.L753
.L742:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L740
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L741
	ldr	r3, [r0, #8]
	cmp	r3, r5
	bne	.L741
	ldr	r3, [r0, #4]
	ldr	r3, [r3, #56]
	cmp	r3, r1
	bne	.L741
	cmp	ip, r2
	bne	.L742
.L753:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_ReorderLTList, .-MVC_ReorderLTList
	.align	2
	.global	MVC_GetMaxViewIdx
	.type	MVC_GetMaxViewIdx, %function
MVC_GetMaxViewIdx:
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
	add	ip, r0, #10747904
	movw	lr, #22868
	add	ip, ip, #20480
	movt	lr, 164
	add	lr, r0, lr
	ldr	r5, [ip, #2384]
	adds	r5, r5, #1
	beq	.L761
	ldr	ip, [ip, #2388]
	cmp	r1, ip
	beq	.L762
	mov	r6, r5
	mov	ip, #0
	b	.L756
.L758:
	ldr	r4, [lr, #4]!
	cmp	r1, r4
	beq	.L764
.L756:
	add	ip, ip, #1
	cmp	ip, r5
	bne	.L758
	cmp	r6, #0
	blt	.L765
.L755:
	cmp	r2, #0
	add	r6, r6, r3, lsl #1
	movwne	r3, #5718
	movweq	r3, #5726
	movt	r3, 41
	add	r3, r6, r3
	add	r0, r0, r3, lsl #2
	ldr	r0, [r0, #4]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L764:
	mov	r6, ip
	cmp	r6, #0
	bge	.L755
.L765:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L761:
	mov	r6, r5
	b	.L755
.L762:
	mov	r6, #0
	b	.L755
	UNWIND(.fnend)
	.size	MVC_GetMaxViewIdx, .-MVC_GetMaxViewIdx
	.align	2
	.global	mvc_get_inter_view_pic
	.type	mvc_get_inter_view_pic, %function
mvc_get_inter_view_pic:
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
	movw	ip, #27286
	mov	r4, #1616
	movt	ip, 42
	add	ip, r3, ip
	mla	r3, r4, r3, r0
	ldr	lr, [r0, ip, asl #2]
	movw	ip, #31360
	cmp	lr, #0
	movt	ip, 169
	add	ip, r3, ip
	beq	.L772
	add	r4, r0, #11141120
	mov	r3, #0
	mov	r0, ip
	add	r4, r4, #12288
	b	.L771
.L768:
	cmp	r3, lr
	add	r0, r0, #808
	beq	.L774
.L771:
	ldr	ip, [r0, #56]
	add	r3, r3, #1
	cmp	ip, r1
	bne	.L768
	ldrb	ip, [r4, #2779]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L769
	ldr	ip, [r0, #716]
	cmp	ip, r2
	bne	.L768
	add	r0, r0, #696
	ldmfd	sp, {r4, fp, sp, pc}
.L769:
	cmp	ip, #1
	beq	.L775
	cmp	ip, #2
	bne	.L768
	ldr	ip, [r0, #788]
	cmp	ip, r2
	bne	.L768
	add	r0, r0, #768
	ldmfd	sp, {r4, fp, sp, pc}
.L775:
	ldr	ip, [r0, #752]
	cmp	ip, r2
	bne	.L768
	add	r0, r0, #732
	ldmfd	sp, {r4, fp, sp, pc}
.L774:
	mov	r0, #0
	ldmfd	sp, {r4, fp, sp, pc}
.L772:
	mov	r0, lr
	ldmfd	sp, {r4, fp, sp, pc}
	UNWIND(.fnend)
	.size	mvc_get_inter_view_pic, .-mvc_get_inter_view_pic
	.align	2
	.global	mvc_reorder_interview
	.type	mvc_reorder_interview, %function
mvc_reorder_interview:
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
	ldr	r4, [fp, #8]
	mov	r7, r2
	ldr	r6, [fp, #4]
	mov	r5, r3
	mov	r8, r1
	mov	r1, r3
	mov	r2, r4, asl #7
	mov	r3, r4
	add	r4, r2, r4, lsl #2
	mov	r2, r6
	add	r4, r4, #256
	add	r4, r0, r4
	bl	mvc_get_inter_view_pic
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r7]
	add	r1, r8, #1
	cmp	r1, r3
	bls	.L778
	add	lr, r4, r1, lsl #2
	mov	ip, r1
.L779:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r3, [r7]
	cmp	r3, ip
	bcc	.L779
.L778:
	add	r2, r3, #1
	str	r2, [r7]
	str	r0, [r4, r3, asl #2]
	ldr	ip, [r7]
	cmp	r1, ip
	ldmccfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r3, r4, ip, lsl #2
	mov	lr, ip
	b	.L783
.L781:
	cmp	r1, ip
	str	r0, [r4, lr, asl #2]
	add	lr, lr, #1
	bcc	.L790
.L783:
	ldr	r0, [r3], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L781
	ldr	r2, [r0, #4]
	ldr	r2, [r2, #56]
	cmp	r2, r5
	bne	.L781
	ldr	r2, [r0, #20]
	cmp	r2, r6
	bne	.L781
	cmp	r1, ip
	bcs	.L783
.L790:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	mvc_reorder_interview, .-mvc_reorder_interview
	.align	2
	.global	MVC_ReorderRefPiclist
	.type	MVC_ReorderRefPiclist, %function
MVC_ReorderRefPiclist:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #68)
	sub	sp, sp, #68
	add	r9, r0, #11075584
	add	r9, r9, #40960
	str	r2, [fp, #-64]
	mov	r2, #0
	mov	r5, r0
	ldrb	r3, [r9, #1001]	@ zero_extendqisi2
	mov	r10, r1
	str	r2, [fp, #-48]
	cmp	r3, r2
	beq	.L792
	ldrb	r3, [r9, #1002]	@ zero_extendqisi2
	cmp	r3, r2
	movne	r3, #2
	moveq	r3, #1
.L792:
	cmp	r10, #1
	bls	.L841
.L791:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L841:
	ldr	r0, [r5, #236]
	cmp	r10, #0
	movweq	r1, #40912
	movweq	r8, #41176
	movweq	r2, #41440
	movweq	r7, #41696
	movwne	r1, #41044
	movwne	r8, #41308
	movwne	r2, #41568
	movwne	r7, #41828
	cmp	r3, #0
	ldr	r3, [r0, #2896]
	movt	r2, 169
	add	r2, r5, r2
	add	r3, r3, #4
	str	r2, [fp, #-100]
	mov	r2, #1
	movt	r1, 169
	mov	r3, r2, asl r3
	str	r3, [fp, #-88]
	ldr	r3, [r9, #1028]
	movt	r8, 169
	ldrne	r2, [fp, #-88]
	movt	r7, 169
	add	r1, r5, r1
	add	r8, r5, r8
	movne	r3, r3, asl #1
	add	r7, r5, r7
	addne	r3, r3, #1
	str	r3, [fp, #-92]
	ldr	r3, [fp, #-64]
	movne	r2, r2, asl #1
	strne	r2, [fp, #-88]
	adds	r3, r3, #2
	str	r3, [fp, #-72]
	beq	.L791
	ldr	r2, [r1]
	cmp	r2, #3
	beq	.L791
	movw	r3, #22868
	str	r9, [fp, #-68]
	mov	r0, r3
	mov	r3, #0
	mov	r4, r3
	str	r3, [fp, #-76]
	str	r3, [fp, #-80]
	mov	r6, r3
	ldr	r3, [fp, #-92]
	movt	r0, 164
	mov	r9, r1
	add	r0, r5, r0
	str	r0, [fp, #-96]
	str	r3, [fp, #-60]
	mov	r3, #1
	str	r3, [fp, #-84]
	b	.L799
.L800:
	cmp	r2, #2
	beq	.L842
	ldr	r3, [fp, #-84]
	cmp	r3, #1
	beq	.L806
	ldr	r3, [fp, #-68]
	ldrb	r3, [r3, #1009]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	mov	r3, r10, asl #1
.L807:
	cmp	r2, #4
	ldr	r2, [r7]
	beq	.L843
	add	r2, r2, #1
	add	r4, r2, r4
	ldr	r2, [fp, #-76]
	cmp	r2, r4
	rsble	r4, r2, r4
.L817:
	ldr	r2, [fp, #-56]
	add	r0, r5, #11141120
	add	r0, r0, #12288
	ldr	r1, [fp, #-64]
	cmp	r2, #0
	ldr	r2, [fp, #-80]
	add	r3, r3, r2
	movwne	r2, #5722
	add	r3, r3, r4
	movweq	r2, #5730
	movt	r2, 41
	add	r2, r3, r2
	add	r2, r5, r2, lsl #2
	ldr	r3, [r2, #4]
	sub	r2, fp, #48
	str	r10, [sp, #4]
	ldr	ip, [r0, #3436]
	mov	r0, r5
	str	ip, [sp]
	bl	mvc_reorder_interview
.L804:
	ldr	r3, [fp, #-72]
	add	r6, r6, #1
	cmp	r6, r3
	beq	.L791
	ldr	r2, [r9, #4]!
	add	r7, r7, #4
	add	r8, r8, #4
	cmp	r2, #3
	beq	.L791
.L799:
	cmp	r2, #1
	bhi	.L800
	cmp	r2, #0
	ldr	r3, [r8]
	bne	.L801
	ldr	r2, [fp, #-60]
	mvn	r3, r3
	adds	r3, r2, r3
	str	r3, [fp, #-60]
	bmi	.L844
.L802:
	ldr	r2, [fp, #-60]
	mov	r1, r10
	ldr	r3, [fp, #-92]
	mov	r0, r5
	cmp	r3, r2
	movlt	r3, r2
	ldrlt	r2, [fp, #-88]
	ldrge	r3, [fp, #-60]
	rsblt	r3, r2, r3
	ldr	r2, [fp, #-68]
	ldr	ip, [r2, #2640]
	sub	r2, fp, #48
	str	r2, [sp]
	ldr	r2, [fp, #-64]
	str	ip, [sp, #4]
	bl	MVC_ReorderSTList
	b	.L804
.L843:
	mvn	r2, r2
	adds	r4, r2, r4
	ldrmi	r2, [fp, #-76]
	addmi	r4, r4, r2
	b	.L817
.L806:
	add	r3, r5, #10747904
	ldr	r1, [fp, #-68]
	add	r3, r3, #20480
	ldr	ip, [r3, #2384]
	ldr	r1, [r1, #2640]
	adds	ip, ip, #1
	beq	.L808
	ldr	r3, [r3, #2388]
	cmp	r1, r3
	beq	.L809
	ldr	r0, [fp, #-96]
	mov	r3, #0
	str	ip, [fp, #-80]
	b	.L810
.L812:
	ldr	lr, [r0, #4]!
	cmp	r1, lr
	beq	.L826
.L810:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L812
.L811:
	ldr	r3, [fp, #-68]
	mov	r4, ip
	ldr	r0, [fp, #-96]
	ldrb	r3, [r3, #1009]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	mov	r3, #0
	b	.L822
.L814:
	ldr	lr, [r0, #4]!
	cmp	r1, lr
	beq	.L827
.L822:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L814
	cmp	r4, #0
	blt	.L828
.L821:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	mov	r3, r10, asl #1
	beq	.L815
	add	r4, r3, r4
	movw	r1, #5718
	movt	r1, 41
	add	r1, r4, r1
	mov	r0, #0
	str	r0, [fp, #-84]
	add	r1, r5, r1, lsl #2
	mvn	r4, #0
	ldr	r1, [r1, #4]
	str	r1, [fp, #-76]
	b	.L807
.L801:
	ldr	r2, [fp, #-60]
	add	r3, r3, #1
	add	r3, r2, r3
	ldr	r2, [fp, #-88]
	str	r3, [fp, #-60]
	cmp	r2, r3
	rsble	r3, r2, r3
	strle	r3, [fp, #-60]
	b	.L802
.L842:
	ldr	r1, [fp, #-68]
	sub	r0, fp, #48
	ldr	r3, [fp, #-100]
	ldr	r2, [fp, #-64]
	ldr	ip, [r1, #2640]
	mov	r1, r10
	ldr	r3, [r3, r6, asl #2]
	str	r0, [sp]
	mov	r0, r5
	str	ip, [sp, #4]
	bl	MVC_ReorderLTList
	b	.L804
.L826:
	str	r3, [fp, #-80]
	b	.L811
.L827:
	mov	r4, r3
	cmp	r4, #0
	bge	.L821
.L828:
	mov	r3, #0
	mvn	r4, #0
	mov	r1, r3
	str	r3, [fp, #-84]
	str	r1, [fp, #-76]
	mov	r3, r10, asl #1
	b	.L807
.L815:
	add	r4, r3, r4
	movw	r1, #5726
	movt	r1, 41
	add	r1, r4, r1
	ldr	r0, [fp, #-56]
	mvn	r4, #0
	add	r1, r5, r1, lsl #2
	str	r0, [fp, #-84]
	ldr	r1, [r1, #4]
	str	r1, [fp, #-76]
	b	.L807
.L844:
	ldr	r2, [fp, #-88]
	add	r3, r3, r2
	str	r3, [fp, #-60]
	b	.L802
.L808:
	ldr	r3, [fp, #-68]
	mov	r4, ip
	str	ip, [fp, #-80]
	ldrb	r3, [r3, #1009]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	b	.L821
.L809:
	ldr	r1, [fp, #-68]
	mov	r3, #0
	mov	r4, r3
	str	r3, [fp, #-80]
	ldrb	r1, [r1, #1009]	@ zero_extendqisi2
	str	r1, [fp, #-56]
	b	.L821
	UNWIND(.fnend)
	.size	MVC_ReorderRefPiclist, .-MVC_ReorderRefPiclist
	.align	2
	.global	MVC_ReorderListX
	.type	MVC_ReorderListX, %function
MVC_ReorderListX:
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
	add	r4, r0, #11075584
	mov	r6, r0
	add	r5, r4, #40960
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	cmp	r1, #2
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r7, r4, #36864
	ldrb	r3, [r7, #4044]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L857
.L848:
	ldr	r3, [r5, #1060]
	cmp	r1, #1
	add	r3, r3, #1
	str	r3, [r5, #1048]
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldrb	r3, [r7, #4045]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L858
.L850:
	add	r4, r4, #40960
	ldr	r3, [r4, #1064]
	add	r3, r3, #1
	str	r3, [r4, #1052]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L857:
	mov	r1, #0
	ldr	r2, [r5, #1060]
	bl	MVC_ReorderRefPiclist
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	b	.L848
.L858:
	ldr	r2, [r5, #1064]
	mov	r0, r6
	bl	MVC_ReorderRefPiclist
	b	.L850
	UNWIND(.fnend)
	.size	MVC_ReorderListX, .-MVC_ReorderListX
	.align	2
	.global	MVC_GenPiclistfromFrmlist
	.type	MVC_GenPiclistfromFrmlist, %function
MVC_GenPiclistfromFrmlist:
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
	ldr	lr, .L922
	movcc	r4, r2
	ldr	ip, .L922+4
	movcs	r4, #31
	cmp	r6, #0
	str	r3, [fp, #-52]
	movne	r6, lr
	moveq	r6, ip
	cmp	r0, #1
	moveq	r5, #0
	moveq	r10, r5
	beq	.L862
	cmp	r0, #2
	moveq	r5, #0
	moveq	r10, r5
	bne	.L870
.L871:
	cmp	r10, r4
	movcs	r3, #0
	movcc	r3, #1
	cmp	r5, r4
	str	r3, [fp, #-48]
	movcs	r2, #0
	movcc	r2, #1
	orrs	r3, r2, r3
	beq	.L870
	cmp	r2, #0
	beq	.L876
	add	r8, r7, r5, lsl #2
	b	.L875
.L872:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L876
.L875:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	tst	ip, #2
	beq	.L872
	add	r0, r0, #768
	blx	r6
	cmp	r0, #0
	beq	.L872
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #768
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L876:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r10, lsl #2
	bne	.L878
	b	.L871
.L877:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L871
.L878:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	tst	r2, #1
	beq	.L877
	add	r0, r0, #732
	blx	r6
	cmp	r0, #0
	beq	.L877
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #732
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
	b	.L871
.L921:
	add	r0, r0, #732
	blx	r6
	cmp	r0, #0
	beq	.L863
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r8]
	ldr	r0, [r3]
	add	r2, r2, #732
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L867:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r5, lsl #2
	bne	.L869
	b	.L862
.L868:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L862
.L869:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	tst	r2, #2
	beq	.L868
	add	r0, r0, #768
	blx	r6
	cmp	r0, #0
	beq	.L868
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #768
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L862:
	cmp	r10, r4
	movcs	r2, #0
	movcc	r2, #1
	cmp	r5, r4
	movcs	r3, #0
	movcc	r3, #1
	str	r3, [fp, #-48]
	orrs	r3, r3, r2
	beq	.L870
	cmp	r2, #0
	beq	.L867
	add	r9, r7, r10, lsl #2
	b	.L866
.L863:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L867
.L866:
	mov	r8, r9
	ldr	r0, [r9], #4
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L863
	b	.L921
.L870:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L923:
	.align	2
.L922:
	.word	MVC_IsLTRefFlg
	.word	MVC_IsSTRefFlg
	UNWIND(.fnend)
	.size	MVC_GenPiclistfromFrmlist, .-MVC_GenPiclistfromFrmlist
	.align	2
	.global	MVC_GetBaseViewId
	.type	MVC_GetBaseViewId, %function
MVC_GetBaseViewId:
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
	add	r3, r0, #12992
	mov	r2, #0
	add	r3, r3, #16
.L927:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L925
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L935
.L925:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L927
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L935:
	movw	r3, #8500
	movt	r3, 5
	mla	r2, r3, r2, r0
	add	r2, r2, #12992
	ldr	r0, [r2, #20]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GetBaseViewId, .-MVC_GetBaseViewId
	.align	2
	.global	MVC_GetVOIdx
	.type	MVC_GetVOIdx, %function
MVC_GetVOIdx:
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
	beq	.L940
	ldr	r3, [r0]
	cmp	r3, r2
	beq	.L941
	mov	ip, r0
	mov	r3, #0
	mov	r0, r1
	b	.L938
.L939:
	ldr	lr, [ip, #4]!
	cmp	lr, r2
	beq	.L942
.L938:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L939
	ldmfd	sp, {fp, sp, pc}
.L942:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L940:
	mov	r0, r1
	ldmfd	sp, {fp, sp, pc}
.L941:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GetVOIdx, .-MVC_GetVOIdx
	.align	2
	.global	MVC_is_view_id_in_ref_view_list
	.type	MVC_is_view_id_in_ref_view_list, %function
MVC_is_view_id_in_ref_view_list:
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
	cmp	r2, #0
	beq	.L947
	ldr	r3, [r1]
	cmp	r3, r0
	beq	.L948
	mov	r3, #0
	b	.L945
.L946:
	ldr	ip, [r1, #4]!
	cmp	ip, r0
	beq	.L944
.L945:
	add	r3, r3, #1
	cmp	r3, r2
	mov	lr, r3
	bne	.L946
.L944:
	cmp	r2, #0
	cmpne	r2, lr
	movhi	r0, #1
	movls	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L947:
	mov	lr, r2
	b	.L944
.L948:
	mov	lr, #0
	b	.L944
	UNWIND(.fnend)
	.size	MVC_is_view_id_in_ref_view_list, .-MVC_is_view_id_in_ref_view_list
	.align	2
	.global	MVC_GenPiclistfromFrmlist_Interview
	.type	MVC_GenPiclistfromFrmlist_Interview, %function
MVC_GenPiclistfromFrmlist_Interview:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #1
	ldr	ip, [fp, #4]
	beq	.L965
	cmp	r0, #2
	ldmnefd	sp, {fp, sp, pc}
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r0, [ip]
	add	r1, r1, #768
	mov	lr, #0
.L956:
	add	lr, lr, #1
	str	r1, [r3, r0, asl #2]
	cmp	lr, r2
	ldr	r0, [ip]
	add	r1, r1, #808
	add	r0, r0, #1
	str	r0, [ip]
	bne	.L956
	ldmfd	sp, {fp, sp, pc}
.L965:
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r0, [ip]
	add	r1, r1, #732
	mov	lr, #0
.L952:
	add	lr, lr, #1
	str	r1, [r3, r0, asl #2]
	cmp	lr, r2
	ldr	r0, [ip]
	add	r1, r1, #808
	add	r0, r0, #1
	str	r0, [ip]
	bne	.L952
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GenPiclistfromFrmlist_Interview, .-MVC_GenPiclistfromFrmlist_Interview
	.align	2
	.global	mvc_append_interview_list
	.type	mvc_append_interview_list, %function
mvc_append_interview_list:
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
	add	lr, r0, #10747904
	add	lr, lr, #20480
	add	ip, r0, #11075584
	add	r5, ip, #40960
	mov	r9, r0
	ldr	r6, [lr, #2384]
	movw	r0, #22868
	movt	r0, 164
	str	r2, [fp, #-48]
	adds	r6, r6, #1
	ldr	r7, [r5, #2640]
	add	r0, r9, r0
	beq	.L985
	ldr	r2, [lr, #2388]
	cmp	r7, r2
	beq	.L986
	mov	r2, #0
	b	.L968
.L969:
	ldr	lr, [r0, #4]!
	cmp	r7, lr
	beq	.L967
.L968:
	add	r2, r2, #1
	cmp	r2, r6
	mov	r4, r2
	bne	.L969
.L967:
	ldrb	r2, [r5, #1009]	@ zero_extendqisi2
	add	r4, r4, r1, lsl #1
	add	r6, r9, #11141120
	add	ip, ip, #45056
	cmp	r2, #0
	add	r6, r6, #12288
	movwne	r2, #5718
	movweq	r2, #5726
	movtne	r2, 41
	movteq	r2, 41
	addne	r2, r4, r2
	addeq	r2, r4, r2
	ldrb	r8, [r6, #2779]	@ zero_extendqisi2
	addne	r4, r9, r4, lsl #2
	add	r2, r9, r2, lsl #2
	movwne	r10, #22892
	addeq	r4, r9, r4, lsl #2
	movweq	r10, #22924
	ldr	r7, [r2, #4]
	sub	r8, r8, #2
	ldr	r2, [ip, #2856]
	movt	r10, 164
	clz	r8, r8
	add	r10, r4, r10
	subs	r4, r2, #1
	mov	r8, r8, lsr #5
	bmi	.L966
	movw	r5, #28314
	movt	r5, 42
	add	r5, r2, r5
	mov	r2, r3
	mov	r3, r9
	add	r5, r9, r5, lsl #2
	mov	r9, r2
	b	.L983
.L1009:
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r2, #3
	ldreq	r0, [r1, #716]
	bne	.L973
.L975:
	add	r2, r1, r8
	ldrb	r2, [r2, #12]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L973
	ldr	r2, [r6, #3436]
	cmp	r2, r0
	beq	.L1008
.L973:
	subs	r4, r4, #1
	bmi	.L966
.L983:
	ldr	r1, [r5, #-4]!
	cmp	r1, #0
	beq	.L973
	ldrb	r2, [r6, #2779]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1009
	cmp	r2, #1
	beq	.L1010
	cmp	r2, #2
	bne	.L973
	ldrb	r0, [r6, #2777]	@ zero_extendqisi2
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	ubfx	r2, r2, #1, #1
	beq	.L978
	ldr	r0, [r6, #3416]
	ldr	ip, [r1, #752]
	ldr	r0, [r0, #752]
	cmp	ip, r0
	movne	r2, #0
	andeq	r2, r2, #1
.L978:
	cmp	r2, #0
	ldrne	r0, [r1, #788]
	bne	.L975
	subs	r4, r4, #1
	bpl	.L983
.L966:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1010:
	ldrb	r0, [r6, #2777]	@ zero_extendqisi2
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	and	r2, r2, #1
	beq	.L977
	ldr	r0, [r6, #3416]
	ldr	ip, [r1, #788]
	ldr	r0, [r0, #788]
	cmp	ip, r0
	movne	r2, #0
	andeq	r2, r2, #1
.L977:
	cmp	r2, #0
	ldrne	r0, [r1, #752]
	bne	.L975
	b	.L973
.L1008:
	cmp	r7, #0
	ldr	lr, [r1, #56]
	beq	.L973
	ldr	r2, [r10]
	cmp	lr, r2
	beq	.L979
	mov	r0, r10
	mov	r2, #0
.L980:
	add	r2, r2, #1
	cmp	r2, r7
	beq	.L973
	ldr	ip, [r0, #4]!
	cmp	lr, ip
	bne	.L980
.L979:
	str	r3, [fp, #-52]
	mov	r2, #808
	ldr	r3, .L1011
	ldr	r0, [r9]
	ldr	ip, [r3, #52]
	ldr	r3, [fp, #-48]
	mla	r0, r2, r0, r3
	blx	ip
	ldr	r2, [r9]
	ldr	r0, [fp, #-48]
	mov	r1, #808
	mov	ip, #808
	ldr	r3, [fp, #-52]
	mla	r2, r1, r2, r0
	str	r2, [r2, #772]
	str	r2, [r2, #736]
	str	r2, [r2, #700]
	ldr	r2, [r9]
	ldr	r1, [r3, #52]
	mla	r2, ip, r2, r0
	sub	r1, r1, #1
	str	r1, [r2, #48]
	ldr	r2, [r9]
	cmp	r2, r7
	beq	.L966
	add	r2, r2, #1
	str	r2, [r9]
	b	.L973
.L985:
	mov	r4, r6
	b	.L967
.L986:
	mov	r4, #0
	b	.L967
.L1012:
	.align	2
.L1011:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	mvc_append_interview_list, .-mvc_append_interview_list
	.align	2
	.global	MVC_InitListX
	.type	MVC_InitListX, %function
MVC_InitListX:
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
	add	r2, r0, #11075584
	add	r5, r2, #40960
	add	r7, r0, #11141120
	mov	r3, #0
	add	r7, r7, #12288
	ldrb	r9, [r5, #1000]	@ zero_extendqisi2
	mov	r6, r0
	str	r3, [r5, #2652]
	cmp	r9, r3
	str	r3, [r5, #2648]
	ldrb	r4, [r7, #2779]	@ zero_extendqisi2
	bne	.L1014
	cmp	r4, r3
	add	r3, r2, #45056
	str	r3, [fp, #-240]
	bne	.L1172
	ldr	ip, [r3, #2864]
	cmp	ip, #0
	beq	.L1098
	movw	r1, #47780
	mov	r3, r4
	movt	r1, 169
	add	r1, r0, r1
	b	.L1020
.L1019:
	cmp	r3, ip
	beq	.L1018
.L1020:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1019
	ldr	r0, [r2, #696]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #65536
	addeq	r0, r4, #64
	addeq	r2, r2, #696
	addeq	r4, r4, #1
	streq	r2, [r6, r0, asl #2]
	cmp	r3, ip
	bne	.L1020
.L1018:
	add	r3, r6, #256
	mov	r1, r4
	str	r3, [fp, #-244]
	mov	r2, #4
	mov	r0, r3
	ldr	r3, .L1182
	bl	qsort
	ldr	r3, [fp, #-240]
	str	r4, [r5, #1048]
	ldr	r1, [r3, #2868]
	cmp	r1, #0
	beq	.L1099
	movw	r0, #47844
	mov	r8, r4
	movt	r0, 169
	mov	r3, #0
	add	r0, r6, r0
	b	.L1023
.L1022:
	cmp	r3, r1
	beq	.L1173
.L1023:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #3]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1022
	ldr	ip, [r2, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	addeq	ip, r8, #64
	addeq	r2, r2, #696
	addeq	r8, r8, #1
	streq	r2, [r6, ip, asl #2]
	cmp	r3, r1
	bne	.L1023
.L1173:
	rsb	r1, r4, r8
.L1021:
	add	r0, r4, #64
	ldr	r3, .L1182+4
	mov	r2, #4
	add	r0, r6, r0, lsl #2
	bl	qsort
	str	r8, [r5, #1048]
	b	.L1024
.L1014:
	cmp	r4, #0
	beq	.L1041
	add	r2, r2, #45056
	str	r2, [fp, #-240]
	ldr	ip, [r2, #2864]
	cmp	ip, #0
	beq	.L1174
	movw	r9, #47780
	mov	r4, r3
	movt	r9, 169
	add	r9, r0, r9
	mov	r1, r9
.L1060:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1059
	sub	r0, fp, #44
	ldr	lr, [r7, #3436]
	add	r8, r0, r4, lsl #2
	ldr	r0, [r2, #32]
	cmp	lr, r0
	strge	r2, [r8, #-192]
	addge	r4, r4, #1
.L1059:
	cmp	r3, ip
	bne	.L1060
	sub	r10, fp, #236
	ldr	r3, .L1182+8
	mov	r2, #4
	mov	r1, r4
	mov	r0, r10
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2864]
	cmp	r8, #0
	beq	.L1169
.L1096:
	mov	r3, r9
	mov	r2, #0
	mov	r9, r4
.L1063:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1062
	sub	r0, fp, #44
	ldr	ip, [r7, #3436]
	add	lr, r0, r9, lsl #2
	ldr	r0, [r1, #32]
	cmp	ip, r0
	strlt	r1, [lr, #-192]
	addlt	r9, r9, #1
.L1062:
	cmp	r2, r8
	bne	.L1063
	rsb	r8, r4, r9
	mov	r3, r4, asl #2
	add	r0, r10, r3
	str	r3, [fp, #-244]
	mov	r1, r8
	ldr	r3, .L1182+12
	mov	r2, #4
	bl	qsort
	cmp	r4, #0
	subeq	ip, fp, #172
	beq	.L1064
.L1091:
	sub	ip, fp, #172
	mov	r2, r10
	add	r8, ip, r8, lsl #2
	mov	r3, #0
.L1065:
	add	r3, r3, #1
	ldr	r1, [r2], #4
	cmp	r3, r4
	str	r1, [r8], #4
	bcc	.L1065
.L1064:
	cmp	r9, r4
	bls	.L1066
	ldr	r3, [fp, #-244]
	add	r2, r10, r3
	mov	r3, ip
.L1067:
	add	r4, r4, #1
	ldr	r1, [r2], #4
	cmp	r4, r9
	str	r1, [r3], #4
	bne	.L1067
.L1066:
	movw	r8, #42008
	mov	r4, #0
	add	r3, r6, #256
	str	r4, [r5, #1048]
	str	r4, [r5, #1052]
	mov	r1, r10
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	mov	r2, r9
	str	r4, [sp, #4]
	movt	r8, 169
	add	r8, r6, r8
	str	r8, [sp]
	str	ip, [fp, #-252]
	movw	r10, #42012
	str	r3, [fp, #-244]
	bl	MVC_GenPiclistfromFrmlist
	ldr	ip, [fp, #-252]
	add	r3, r6, #388
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	mov	r2, r9
	str	r4, [sp, #4]
	movt	r10, 169
	mov	r1, ip
	add	r10, r6, r10
	str	r10, [sp]
	str	r3, [fp, #-248]
	bl	MVC_GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	r9, [r3, #2868]
	cmp	r9, r4
	subeq	ip, fp, #108
	beq	.L1068
	movw	r3, #47844
	sub	ip, fp, #108
	movt	r3, 169
	add	r3, r6, r3
	mov	r2, ip
.L1069:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r9
	str	r1, [r2], #4
	bne	.L1069
.L1068:
	mov	r0, ip
	ldr	r3, .L1182+16
	mov	r2, #4
	mov	r1, r9
	str	ip, [fp, #-240]
	bl	qsort
	ldr	ip, [fp, #-240]
	mov	r4, #1
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	mov	r2, r9
	ldr	r3, [fp, #-244]
	mov	r1, ip
	str	r4, [sp, #4]
	str	r8, [sp]
	bl	MVC_GenPiclistfromFrmlist
	ldr	ip, [fp, #-240]
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	mov	r2, r9
	str	r4, [sp, #4]
	ldr	r3, [fp, #-248]
	mov	r1, ip
	str	r10, [sp]
	bl	MVC_GenPiclistfromFrmlist
	ldr	r4, [r5, #1048]
	ldr	r3, [r5, #1052]
	rsb	r3, r3, r4
	clz	r3, r3
	mov	r3, r3, lsr #5
.L1058:
	cmp	r4, #1
	movls	r3, #0
	andhi	r3, r3, #1
	cmp	r3, #0
	beq	.L1070
	cmp	r4, #0
	beq	.L1071
	ldr	lr, [r6, #256]
	ldr	r3, [r6, #388]
	cmp	lr, r3
	bne	.L1070
	ldr	r1, [fp, #-244]
	add	r2, r6, #388
	mov	r3, #0
	b	.L1072
.L1073:
	ldr	ip, [r1, #4]!
	ldr	r0, [r2, #4]!
	cmp	ip, r0
	bne	.L1070
.L1072:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L1073
.L1074:
	ldr	r3, [r6, #392]
	str	lr, [r6, #392]
	str	r3, [r6, #388]
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
.L1092:
	cmp	r3, #0
	bne	.L1161
.L1075:
	movw	r4, #31360
	movw	r3, #43608
	movt	r4, 169
	add	r4, r6, r4
	mov	r0, r6
	movt	r3, 169
	mov	r1, #0
	mov	r2, r4
	add	r3, r6, r3
	bl	mvc_append_interview_list
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1175
	ldr	r1, [r5, #2648]
	ldr	ip, [r5, #1048]
	cmp	r1, #0
	beq	.L1077
	add	r2, ip, #63
	movw	r3, #32056
	movt	r3, 169
	add	r3, r6, r3
	add	r2, r6, r2, lsl #2
.L1078:
	add	r0, r0, #1
	str	r3, [r2, #4]!
	cmp	r0, r1
	add	r3, r3, #808
	bne	.L1078
	add	ip, ip, r0
.L1077:
	str	ip, [r5, #1048]
.L1079:
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1176
.L1162:
	ldr	r0, [r5, #1048]
.L1076:
	cmp	r1, #0
	bne	.L1034
	cmp	r0, #0
	ldreq	r1, .L1182+20
	beq	.L1163
.L1033:
	ldr	r2, [r5, #1060]
	ldr	r3, [r5, #1064]
	add	r2, r2, #1
	ldr	r1, [r5, #1052]
	cmp	r0, r2
	add	r3, r3, #1
	movcc	r2, r0
	cmp	r3, r1
	str	r2, [r5, #1048]
	movcs	r3, r1
	cmp	r2, #32
	str	r3, [r5, #1052]
	bhi	.L1085
.L1093:
	add	r1, r2, #63
	mov	r0, #0
	add	r1, r6, r1, lsl #2
.L1086:
	add	r2, r2, #1
	str	r0, [r1, #4]!
	cmp	r2, #32
	bls	.L1086
.L1085:
	cmp	r3, #32
	bhi	.L1089
	add	r3, r3, #96
	add	r1, r6, #516
	mov	r2, #0
	add	r6, r6, r3, lsl #2
.L1088:
	str	r2, [r6, #4]!
	cmp	r6, r1
	bne	.L1088
.L1089:
	mov	r0, #0
.L1155:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1041:
	add	r3, r2, #45056
	str	r3, [fp, #-240]
	ldr	r10, [r3, #2864]
	cmp	r10, #0
	beq	.L1044
	movw	r9, #47780
	mov	r3, r4
	movt	r9, 169
	add	r9, r0, r9
	mov	r1, r9
	b	.L1046
.L1045:
	cmp	r3, r10
	beq	.L1177
.L1046:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1045
	ldr	r0, [r2, #712]
	add	lr, r4, #64
	ldr	ip, [r7, #3440]
	add	r2, r2, #696
	cmp	ip, r0
	strge	r2, [r6, lr, asl #2]
	addge	r4, r4, #1
	cmp	r3, r10
	bne	.L1046
.L1177:
	add	r3, r6, #256
	mov	r2, #4
	str	r3, [fp, #-244]
	mov	r1, r4
	mov	r0, r3
	ldr	r3, .L1182+24
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2864]
	cmp	r8, #0
	beq	.L1168
.L1094:
	mov	r3, r9
	mov	r10, r4
	mov	r2, #0
	b	.L1049
.L1048:
	cmp	r2, r8
	beq	.L1178
.L1049:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1048
	ldr	r0, [r1, #712]
	add	lr, r10, #64
	ldr	ip, [r7, #3440]
	add	r1, r1, #696
	cmp	ip, r0
	strlt	r1, [r6, lr, asl #2]
	addlt	r10, r10, #1
	cmp	r2, r8
	bne	.L1049
.L1178:
	rsb	r8, r4, r10
	add	r0, r4, #64
	ldr	r3, .L1182+28
	mov	r2, #4
	mov	r1, r8
	add	r0, r6, r0, lsl #2
	bl	qsort
	add	r9, r10, #64
	cmp	r4, #0
	add	r9, r6, r9, lsl #2
	beq	.L1051
.L1090:
	add	r8, r8, #96
	add	r2, r6, #252
	mov	r3, #0
	add	r8, r6, r8, lsl #2
.L1052:
	add	r3, r3, #1
	ldr	r1, [r2, #4]!
	cmp	r3, r4
	str	r1, [r8, #4]!
	bcc	.L1052
.L1051:
	cmp	r10, r4
	bls	.L1053
	add	r3, r4, #63
	add	r2, r6, #384
	add	r3, r6, r3, lsl #2
.L1054:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r10
	str	r1, [r2, #4]!
	bne	.L1054
.L1053:
	ldr	r3, [fp, #-240]
	str	r10, [r5, #1052]
	str	r10, [r5, #1048]
	ldr	r1, [r3, #2868]
	cmp	r1, #0
	beq	.L1100
	movw	r0, #47844
	mov	r4, r10
	movt	r0, 169
	mov	r3, #0
	add	r0, r6, r0
	b	.L1057
.L1056:
	cmp	r3, r1
	beq	.L1179
.L1057:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1056
	ldrb	ip, [r2, #697]	@ zero_extendqisi2
	cmp	ip, #1
	addeq	ip, r6, r4, lsl #2
	addeq	r2, r2, #696
	addeq	r4, r4, #1
	streq	r2, [ip, #256]
	streq	r2, [ip, #388]
	cmp	r3, r1
	bne	.L1057
.L1179:
	rsb	r1, r10, r4
.L1055:
	mov	r0, r9
	ldr	r3, .L1182+4
	mov	r2, #4
	bl	qsort
	ldr	r1, [r5, #1048]
	ldr	r3, .L1182+4
	mov	r2, #4
	add	r0, r1, #97
	rsb	r1, r1, r4
	add	r0, r6, r0, lsl #2
	bl	qsort
	str	r4, [r5, #1052]
	str	r4, [r5, #1048]
	mov	r3, #1
	b	.L1058
.L1172:
	ldr	lr, [r3, #2864]
	cmp	lr, #0
	moveq	r9, lr
	beq	.L1017
	movw	r1, #47780
	mov	r3, r9
	movt	r1, 169
	add	r1, r0, r1
.L1026:
	ldr	r2, [r1, #4]!
	sub	r0, fp, #44
	add	ip, r0, r9, lsl #2
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #0
	strne	r2, [ip, #-192]
	addne	r9, r9, #1
	cmp	r3, lr
	bne	.L1026
.L1017:
	mov	r1, r9
	ldr	r3, .L1182+32
	mov	r2, #4
	sub	r0, fp, #236
	movw	r8, #42008
	bl	qsort
	mov	r4, #0
	add	r3, r6, #256
	str	r4, [r5, #1048]
	mov	r2, r9
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	sub	r1, fp, #236
	str	r4, [sp, #4]
	movt	r8, 169
	add	r8, r6, r8
	str	r8, [sp]
	str	r3, [fp, #-244]
	bl	MVC_GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	r9, [r3, #2868]
	cmp	r9, r4
	subeq	ip, fp, #108
	beq	.L1027
	movw	r3, #47844
	sub	ip, fp, #108
	movt	r3, 169
	add	r3, r6, r3
	mov	r2, ip
.L1028:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r9
	str	r1, [r2], #4
	bne	.L1028
.L1027:
	mov	r0, ip
	ldr	r3, .L1182+16
	mov	r2, #4
	mov	r1, r9
	str	ip, [fp, #-240]
	bl	qsort
	ldr	ip, [fp, #-240]
	mov	r3, #1
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	mov	r2, r9
	str	r3, [sp, #4]
	str	r8, [sp]
	mov	r1, ip
	ldr	r3, [fp, #-244]
	bl	MVC_GenPiclistfromFrmlist
.L1024:
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
	mov	r2, #0
	str	r2, [r5, #1052]
	cmp	r3, r2
	beq	.L1075
	ldr	r0, [r5, #1048]
	cmp	r0, #0
	bne	.L1180
	ldr	r3, [r6, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L1035
	ldrb	r3, [r7, #2779]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1036
	ldr	r3, [r7, #3416]
	add	r3, r3, #696
	str	r3, [r6, #256]
.L1037:
	mov	r3, #1
	str	r3, [r5, #1048]
.L1161:
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	b	.L1162
.L1180:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1033
.L1034:
	cmp	r0, #0
	bne	.L1033
	ldr	r1, [r5, #1052]
	cmp	r1, #0
	beq	.L1181
	ldr	r3, [r5, #1064]
	mov	r2, r0
	str	r0, [r5, #1048]
	add	r3, r3, #1
	cmp	r3, r1
	movcs	r3, r1
	str	r3, [r5, #1052]
	b	.L1093
.L1070:
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
	b	.L1092
.L1175:
	movw	ip, #42008
	ldr	r2, [r5, #2648]
	ldr	r3, [fp, #-244]
	mov	r1, r4
	movt	ip, 169
	add	ip, r6, ip
	str	ip, [sp]
	bl	MVC_GenPiclistfromFrmlist_Interview
	b	.L1079
.L1036:
	cmp	r3, #1
	ldr	r3, [r7, #3416]
	addeq	r3, r3, #732
	addne	r3, r3, #768
	str	r3, [r6, #256]
	b	.L1037
.L1071:
	ldr	lr, [r6, #388]
	b	.L1074
.L1176:
	movw	r4, #32976
	movw	r3, #43612
	movt	r4, 169
	add	r4, r6, r4
	mov	r0, r6
	movt	r3, 169
	mov	r2, r4
	add	r3, r6, r3
	bl	mvc_append_interview_list
	ldrb	r0, [r7, #2779]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1081
	ldr	r1, [r5, #2652]
	ldr	ip, [r5, #1052]
	cmp	r1, #0
	beq	.L1082
	add	r2, ip, #96
	movw	r3, #33672
	movt	r3, 169
	add	r3, r6, r3
	add	r2, r6, r2, lsl #2
.L1083:
	add	r0, r0, #1
	str	r3, [r2, #4]!
	cmp	r0, r1
	add	r3, r3, #808
	bne	.L1083
	add	ip, ip, r0
.L1082:
	str	ip, [r5, #1052]
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	ldr	r0, [r5, #1048]
	b	.L1076
.L1169:
	mov	r3, r4, asl #2
	mov	r1, r8
	str	r3, [fp, #-244]
	mov	r2, r3
	ldr	r3, .L1182+12
	add	r0, r10, r2
	mov	r2, #4
	bl	qsort
	subs	r9, r4, #0
	bne	.L1091
.L1164:
	sub	ip, fp, #172
	b	.L1066
.L1168:
	add	r9, r4, #64
	ldr	r3, .L1182+28
	mov	r2, #4
	mov	r1, r8
	add	r9, r6, r9, lsl #2
	mov	r0, r9
	bl	qsort
	subs	r10, r4, #0
	bne	.L1090
	b	.L1053
.L1098:
	mov	r4, ip
	b	.L1018
.L1099:
	mov	r8, r4
	b	.L1021
.L1100:
	mov	r4, r10
	b	.L1055
.L1174:
	sub	r10, fp, #236
	ldr	r3, .L1182+8
	mov	r1, ip
	mov	r2, #4
	mov	r0, r10
	str	ip, [fp, #-244]
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	ip, [fp, #-244]
	ldr	r8, [r3, #2864]
	cmp	r8, #0
	movwne	r9, #47780
	movne	r4, ip
	movtne	r9, 169
	addne	r9, r6, r9
	bne	.L1096
	ldr	r3, .L1182+12
	mov	r2, #4
	mov	r1, r8
	mov	r0, r10
	mov	r9, r8
	bl	qsort
	b	.L1164
.L1044:
	add	r3, r0, #256
	mov	r2, #4
	str	r3, [fp, #-244]
	mov	r1, r10
	mov	r0, r3
	ldr	r3, .L1182+24
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2864]
	cmp	r8, #0
	movwne	r9, #47780
	movne	r4, r10
	movtne	r9, 169
	addne	r9, r6, r9
	bne	.L1094
	ldr	r9, [fp, #-244]
	mov	r2, #4
	ldr	r3, .L1182+28
	mov	r1, r8
	mov	r10, r8
	mov	r0, r9
	bl	qsort
	b	.L1053
.L1181:
	ldr	r1, .L1182+36
.L1163:
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	b	.L1155
.L1081:
	movw	ip, #42012
	ldr	r2, [r5, #2652]
	mov	r1, r4
	movt	ip, 169
	add	r3, r6, #388
	add	ip, r6, ip
	str	ip, [sp]
	bl	MVC_GenPiclistfromFrmlist_Interview
	b	.L1161
.L1035:
	ldr	r1, .L1182+40
	b	.L1163
.L1183:
	.align	2
.L1182:
	.word	MVC_compare_pic_by_pic_num_desc
	.word	MVC_compare_pic_by_lt_pic_num_asc
	.word	MVC_compare_fs_by_poc_desc
	.word	MVC_compare_fs_by_poc_asc
	.word	MVC_compare_fs_by_lt_pic_idx_asc
	.word	.LC26
	.word	MVC_compare_pic_by_poc_desc
	.word	MVC_compare_pic_by_poc_asc
	.word	MVC_compare_fs_by_frame_num_desc
	.word	.LC27
	.word	.LC25
	UNWIND(.fnend)
	.size	MVC_InitListX, .-MVC_InitListX
	.align	2
	.global	MVC_DumpList
	.type	MVC_DumpList, %function
MVC_DumpList:
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
	ldr	r3, .L1197
	ldr	r3, [r3]
	tst	r3, #8192
	beq	.L1184
	movw	r8, #42004
	add	r7, r0, #252
	movt	r8, 169
	add	r8, r0, r8
	mov	r6, #0
.L1186:
	ldr	r3, [r8, #4]!
	cmp	r3, #0
	movne	r5, r7
	movne	r4, #0
	beq	.L1188
.L1187:
	ldr	ip, [r5, #4]!
	mov	r3, r4
	mov	r2, r6
	ldr	r1, .L1197+4
	mov	r0, #13
	add	r4, r4, #1
	ldr	ip, [ip, #4]
	ldr	lr, [ip, #32]
	str	lr, [sp, #4]
	ldr	ip, [ip, #20]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r8]
	cmp	r3, r4
	bhi	.L1187
.L1188:
	add	r6, r6, #1
	add	r7, r7, #132
	cmp	r6, #2
	bne	.L1186
.L1184:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1198:
	.align	2
.L1197:
	.word	g_PrintEnable
	.word	.LC28
	UNWIND(.fnend)
	.size	MVC_DumpList, .-MVC_DumpList
	.align	2
	.global	MVC_FindNearestPOCPicId
	.type	MVC_FindNearestPOCPicId, %function
MVC_FindNearestPOCPicId:
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
	add	r3, r0, #11075584
	add	r3, r3, #45056
	ldr	r2, [r3, #2860]
	cmp	r2, #0
	beq	.L1204
	ldr	r4, [r3, #2856]
	add	r3, r0, #11141120
	add	r3, r3, #12288
	cmp	r4, #0
	ldr	r5, [r3, #3436]
	beq	.L1205
	movw	ip, #47716
	mov	r6, #0
	movt	ip, 169
	add	ip, r0, ip
	mov	r1, r6
	mvn	lr, #-2147483648
.L1203:
	ldr	r2, [ip, #4]!
	add	r1, r1, #1
	cmp	r2, #0
	beq	.L1202
	ldr	r3, [r2, #32]
	rsb	r3, r3, r5
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, lr
	ldrlt	r6, [r2, #228]
	movlt	lr, r3
.L1202:
	cmp	r1, r4
	bne	.L1203
.L1201:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1205:
	mov	r6, r4
	b	.L1201
.L1204:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_FindNearestPOCPicId, .-MVC_FindNearestPOCPicId
	.align	2
	.global	MVC_FindMinRefIdx
	.type	MVC_FindMinRefIdx, %function
MVC_FindMinRefIdx:
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
	add	r3, r0, #11075584
	add	r3, r3, #40960
	ldrb	r2, [r3, #1000]	@ zero_extendqisi2
	cmp	r2, #2
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	add	r2, r0, #11141120
	movw	r6, #48148
	add	r2, r2, #12288
	ldrb	lr, [r2, #2779]	@ zero_extendqisi2
	cmp	lr, #0
	bne	.L1240
	ldr	r4, [r3, #1048]
	movw	r5, #48152
	movt	r6, 169
	movt	r5, 169
	cmp	r4, #0
	add	r6, r0, r6
	add	r5, r0, r5
	mov	r7, lr
	beq	.L1218
.L1242:
	add	r2, r0, #252
	mov	ip, #32
	mov	r3, #0
	b	.L1216
.L1215:
	add	r3, r3, #1
	cmp	r3, r4
	beq	.L1241
.L1216:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #52]
	cmp	r1, lr
	bne	.L1215
	cmp	ip, r3
	movcs	ip, r3
	add	r3, r3, #1
	cmp	r3, r4
	str	ip, [r5, #-4]
	str	ip, [r6, #4]
	bne	.L1216
.L1241:
	cmp	ip, #31
	bhi	.L1218
.L1217:
	add	lr, lr, #1
	add	r6, r6, #8
	cmp	lr, #16
	add	r5, r5, #8
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r4, #0
	bne	.L1242
.L1218:
	str	r7, [r5, #-4]
	str	r7, [r6, #4]
	b	.L1217
.L1240:
	ldr	r5, [r3, #1048]
	mov	lr, #0
	movt	r6, 169
	mov	r7, lr
	cmp	r5, #0
	add	r6, r0, r6
	beq	.L1222
.L1244:
	add	r1, r0, #252
	mov	r4, #32
	mov	r2, #0
	b	.L1220
.L1219:
	add	r2, r2, #1
	cmp	r2, r5
	beq	.L1243
.L1220:
	ldr	r3, [r1, #4]!
	ldr	r8, [r3, #4]
	ldrb	ip, [r3]	@ zero_extendqisi2
	ldr	r3, [r8, #52]
	mov	r3, r3, asl #1
	cmp	ip, #2
	orreq	r3, r3, #1
	cmp	r3, lr
	bne	.L1219
	cmp	r4, r2
	movcs	r4, r2
	add	r2, r2, #1
	cmp	r2, r5
	str	r4, [r6]
	bne	.L1220
.L1243:
	cmp	r4, #31
	bhi	.L1222
.L1221:
	add	lr, lr, #1
	add	r6, r6, #4
	cmp	lr, #32
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r5, #0
	bne	.L1244
.L1222:
	str	r7, [r6]
	b	.L1221
	UNWIND(.fnend)
	.size	MVC_FindMinRefIdx, .-MVC_FindMinRefIdx
	.align	2
	.global	MVC_DecList
	.type	MVC_DecList, %function
MVC_DecList:
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
	mov	r4, r0
	bl	MVC_FindNearestPOCPicId
	add	r5, r4, #11075584
	mvn	r6, #0
	add	r3, r5, #40960
	ldrb	r2, [r3, #1000]	@ zero_extendqisi2
	str	r6, [r3, #2632]
	cmp	r2, #2
	str	r0, [r3, #2636]
	beq	.L1252
	mov	r0, r4
	bl	MVC_InitListX
	cmp	r0, #0
	bne	.L1253
	mov	r0, r4
	bl	MVC_ReorderListX
	mov	r0, r4
	bl	MVC_RepairList
	cmp	r0, #0
	bne	.L1250
	mov	r0, r4
	add	r5, r5, #40960
	bl	MVC_FindMinRefIdx
	ldr	r3, [r4, #256]
	mov	r0, #0
	cmp	r3, #0
	ldrne	r3, [r3, #4]
	ldrne	r6, [r3, #228]
	str	r6, [r5, #2632]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1252:
	mov	r0, #0
	str	r0, [r3, #1048]
	str	r0, [r3, #1052]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1250:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1253:
	ldr	r1, .L1254
	mov	r0, #13
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1255:
	.align	2
.L1254:
	.word	.LC29
	UNWIND(.fnend)
	.size	MVC_DecList, .-MVC_DecList
	.align	2
	.global	MVC_NoPicOut
	.type	MVC_NoPicOut, %function
MVC_NoPicOut:
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
	ldr	ip, [r0, #48]
	cmp	ip, #0
	beq	.L1263
	movw	r3, #48287
	mov	r2, #0
	movt	r3, 169
	mov	lr, r2
	add	r3, r0, r3
	b	.L1262
.L1271:
	ldrb	r1, [r3, #-2]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1260
	cmp	r2, ip
	add	r3, r3, #808
	beq	.L1263
.L1262:
	ldrb	r1, [r3]	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r1, #1
	bne	.L1271
.L1260:
	cmp	r2, ip
	strb	lr, [r3, #-5]
	strb	lr, [r3, #-4]
	add	r3, r3, #808
	bne	.L1262
.L1263:
	ldr	r2, [r0, #52]
	cmp	r2, #0
	beq	.L1272
	mov	r3, #0
	add	r0, r0, #144
	mov	r1, r3
.L1264:
	add	r3, r3, #1
	str	r1, [r0, #4]!
	cmp	r3, r2
	bne	.L1264
	ldmfd	sp, {fp, sp, pc}
.L1272:
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_NoPicOut, .-MVC_NoPicOut
	.align	2
	.global	MVC_GetBackPicFromVOQueue
	.type	MVC_GetBackPicFromVOQueue, %function
MVC_GetBackPicFromVOQueue:
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
	ldr	r0, [r0, #120]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1290
.L1274:
	ldr	r3, [r8, #48]
	cmp	r3, #0
	beq	.L1281
	movw	r5, #48936
	mov	r6, #0
	movt	r5, 169
	add	r5, r8, r5
	mov	r7, #0
	mov	r9, #0
.L1280:
	sub	r4, r5, #32
	mov	r3, #0
	strb	r3, [r5, #-654]
	strb	r3, [r5, #-653]
.L1279:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1291
.L1278:
	cmp	r4, r5
	bne	.L1279
	ldr	r3, [r8, #48]
	add	r9, r9, #1
	add	r5, r5, #808
	cmp	r3, r9
	bhi	.L1280
.L1281:
	ldr	r2, [r8, #52]
	cmp	r2, #0
	beq	.L1292
	mov	r3, #0
	add	r8, r8, #144
	mov	r1, r3
.L1282:
	add	r3, r3, #1
	str	r1, [r8, #4]!
	cmp	r3, r2
	bne	.L1282
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1291:
	mov	r1, r2
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L1278
.L1290:
	add	r0, r8, #584
	bl	ResetVoQueue
	b	.L1274
.L1292:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GetBackPicFromVOQueue, .-MVC_GetBackPicFromVOQueue
	.align	2
	.global	mvc_wait_vo
	.type	mvc_wait_vo, %function
mvc_wait_vo:
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
	.size	mvc_wait_vo, .-mvc_wait_vo
	.align	2
	.global	MVC_RoundLog2
	.type	MVC_RoundLog2, %function
MVC_RoundLog2:
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
	mul	r0, r0, r0
	mov	r3, #0
	mov	r2, #1
.L1295:
	add	r3, r3, #1
	cmp	r0, r2, asl r3
	bge	.L1295
	mov	r0, r3, asr #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_RoundLog2, .-MVC_RoundLog2
	.align	2
	.global	MVC_GetReRangeFlag
	.type	MVC_GetReRangeFlag, %function
MVC_GetReRangeFlag:
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
	add	ip, r0, #11075584
	add	r6, ip, #40960
	ldr	lr, [r0, #252]
	mov	r4, r0
	mov	r7, #2240
	ldrb	r0, [r6, #1012]	@ zero_extendqisi2
	mov	r5, r1
	ldr	r3, [r6, #1016]
	sxtb	r2, r0
	cmn	r2, #1
	mla	r3, r7, r3, lr
	beq	.L1314
	cmp	r0, #0
	bne	.L1300
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	ldr	r3, [r3, #28]
	cmp	r2, #1
	beq	.L1315
	ldr	r2, [r4, #28]
	cmp	r2, r3
	beq	.L1302
	ldr	r1, .L1317
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1300:
	ldr	r1, .L1317+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1314:
	ldr	r2, [r3, #28]
	movw	r7, #3992
	ldr	r3, [r4, #248]
	mla	r7, r7, r2, r3
.L1299:
	ldrb	r3, [r7, #20]	@ zero_extendqisi2
	ldr	r1, [r7, #3952]
	rsb	r3, r3, #2
	ldr	r2, [r7, #3948]
	ldr	lr, [r4, #12]
	mla	r3, r1, r3, r3
	ldr	r0, [r4, #16]
	add	r2, r2, #1
	cmp	r2, lr
	cmpeq	r3, r0
	movne	r8, #1
	moveq	r8, #0
	bne	.L1304
	add	r1, ip, #45056
	ldr	ip, [r7, #3972]
	ldr	r1, [r1, #2856]
	add	r1, r1, #1
	cmp	ip, r1
	movls	lr, r8
	bls	.L1305
.L1304:
	ldr	r1, .L1317+8
	ldr	ip, [r1]
	cmp	ip, #0
	moveq	lr, #1
	beq	.L1305
	mov	lr, lr, asl #4
	mov	r1, r3, asl #4
	mov	r0, r0, asl #4
	strh	lr, [fp, #-44]	@ movhi
	mov	r2, r2, asl #4
	strh	r0, [fp, #-42]	@ movhi
	strh	r2, [fp, #-40]	@ movhi
	mov	r3, #8
	strh	r1, [fp, #-38]	@ movhi
	sub	r2, fp, #44
	mov	r1, #2
	ldr	r0, [r4, #120]
	blx	ip
	mov	lr, #1
.L1305:
	ldrb	r3, [r6, #1011]	@ zero_extendqisi2
	cmp	r3, #1
	ldrne	ip, [r7, #3972]
	subne	ip, ip, #1
	beq	.L1316
.L1308:
	ldr	r3, [r4, #44]
	mov	r0, lr
	cmp	r3, ip
	orrne	lr, lr, #1
	str	lr, [r5]
	str	ip, [r4, #44]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1302:
	movw	r7, #35364
	movt	r7, 168
	add	r7, r4, r7
	b	.L1299
.L1316:
	movw	r1, #39336
	add	r3, r4, #290816
	movt	r1, 168
	add	r3, r3, #808
	add	r1, r4, r1
	mov	ip, #0
.L1307:
	ldr	r2, [r3]
	add	r3, r3, #335872
	add	r3, r3, #308
	cmp	r2, ip
	subhi	ip, r2, #1
	cmp	r3, r1
	bne	.L1307
	add	ip, ip, #1
	mov	ip, ip, asl #1
	cmp	ip, #16
	movcs	ip, #16
	b	.L1308
.L1315:
	movw	r7, #8500
	movt	r7, 5
	mla	r7, r7, r3, r4
	add	r7, r7, #286720
	add	r7, r7, #932
	b	.L1299
.L1318:
	.align	2
.L1317:
	.word	.LC30
	.word	.LC31
	.word	g_event_report
	UNWIND(.fnend)
	.size	MVC_GetReRangeFlag, .-MVC_GetReRangeFlag
	.global	__aeabi_uidiv
	.global	__aeabi_uidivmod
	.align	2
	.global	MVC_DecPOC
	.type	MVC_DecPOC, %function
MVC_DecPOC:
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
	add	r6, r0, #11075584
	ldr	r5, [r0, #236]
	add	r7, r6, #40960
	mov	r2, #1
	ldrb	r1, [r7, #1003]	@ zero_extendqisi2
	ldr	ip, [r5, #2900]
	ldr	r3, [r5, #2896]
	sub	r1, r1, #5
	clz	r1, r1
	cmp	ip, r2
	add	r3, r3, #4
	ldr	lr, [r5, #2904]
	mov	r1, r1, lsr #5
	mov	r3, r2, asl r3
	beq	.L1321
	bcc	.L1322
	cmp	ip, #2
	ldmnefd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	cmp	r1, #0
	bne	.L1377
	add	r4, r6, #36864
	ldrb	r2, [r4, #3960]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1355
	ldr	r1, [r4, #4016]
	ldr	r0, [r4, #4012]
	ldr	r2, [r4, #4024]
	cmp	r1, r0
	addcc	r2, r2, r3
	strcc	r2, [r4, #4020]
	bcs	.L1357
.L1358:
	add	r3, r2, r1
	str	r3, [r4, #4008]
	ldrb	r0, [r7, #1008]	@ zero_extendqisi2
	mov	r3, r3, asl #1
	cmp	r0, #0
	subeq	r3, r3, #1
	str	r3, [r4, #3996]
	ldrb	r0, [r7, #1001]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1361
	str	r3, [r4, #3992]
	str	r3, [r4, #3988]
	str	r3, [r4, #3984]
.L1354:
	add	r6, r6, #36864
	str	r1, [r6, #4012]
	str	r2, [r6, #4024]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1322:
	cmp	r1, #0
	add	lr, lr, #4
	mov	r2, r2, asl lr
	bne	.L1378
	add	r4, r6, #36864
	ldrb	r3, [r4, #3960]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1326
	ldr	lr, [r4, #3972]
	ldr	ip, [r4, #3976]
	mov	r1, lr
.L1327:
	ldr	r0, [r4, #3964]
	cmp	r0, ip
	bcs	.L1325
	rsb	r3, r0, ip
	cmp	r3, r2, lsr #1
	addcs	r1, r1, r2
	strcs	r1, [r4, #3980]
	bcs	.L1329
.L1325:
	cmp	r0, ip
	bls	.L1330
	rsb	r3, ip, r0
	cmp	r3, r2, lsr #1
	rsbhi	r1, r2, r1
	strhi	r1, [r4, #3980]
	bls	.L1330
.L1329:
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1331
.L1380:
	ldr	r3, [r4, #3968]
	add	r1, r1, r0
	str	r1, [r4, #3984]
	add	r3, r1, r3
	str	r3, [r4, #3988]
	cmp	r3, r1
	movge	r3, r1
	str	r3, [r4, #3996]
.L1332:
	ldr	r2, [r4, #4016]
	str	r3, [r4, #3992]
	add	r3, r6, #40960
	ldr	r1, [r4, #4012]
	cmp	r2, r1
	strne	r2, [r4, #4012]
	ldrb	r3, [r3, #1008]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r6, r6, #36864
	ldrne	r2, [r6, #3964]
	ldrne	r3, [r6, #3980]
	strne	r2, [r6, #3976]
	strne	r3, [r6, #3972]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1321:
	cmp	r1, #0
	add	r4, r6, #36864
	movne	r3, #0
	strne	r3, [r4, #4020]
	bne	.L1336
	ldrb	r2, [r4, #3960]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1337
	mov	r3, r1
	str	r1, [r4, #4024]
	str	r1, [r4, #4012]
.L1338:
	str	r3, [r4, #4020]
.L1336:
	ldr	r3, [r5, #2916]
	cmp	r3, #0
	beq	.L1340
	ldr	r3, [r4, #4016]
	ldr	r0, [r4, #4020]
	add	r0, r3, r0
	str	r0, [r4, #4008]
	ldrb	r8, [r7, #1008]	@ zero_extendqisi2
	cmp	r8, #0
	bne	.L1342
	cmp	r0, #0
	beq	.L1343
	sub	r0, r0, #1
	str	r0, [r4, #4008]
.L1342:
	mov	r3, #0
	str	r3, [r4, #4040]
	ldr	r1, [r5, #2916]
	cmp	r1, r3
	bgt	.L1363
	cmp	r0, #0
	beq	.L1346
.L1379:
	sub	r9, r0, #1
	mov	r0, r9
	bl	__aeabi_uidiv
	mov	r10, r0
	mov	r0, r9
	str	r10, [r4, #4032]
	ldr	r1, [r5, #2916]
	bl	__aeabi_uidivmod
	ldr	r0, [r4, #4040]
	mul	r0, r0, r10
	str	r0, [r4, #4036]
	cmp	r1, #0
	str	r1, [r4, #4028]
	blt	.L1347
	add	r2, r5, #2912
	add	r1, r1, #1
	add	r2, r2, #4
	mov	r3, #0
.L1348:
	add	r3, r3, #1
	ldr	ip, [r2, #4]!
	cmp	r3, r1
	add	r0, r0, ip
	str	r0, [r4, #4036]
	bne	.L1348
.L1347:
	cmp	r8, #0
	ldreq	r3, [r5, #2908]
	addeq	r0, r0, r3
	streq	r0, [r4, #4036]
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1350
	ldr	r2, [r4, #4000]
	ldr	r1, [r4, #4004]
	add	r2, r0, r2
	str	r2, [r4, #3984]
	ldr	r3, [r5, #2912]
	add	r3, r2, r3
	add	r0, r3, r1
	str	r0, [r4, #3988]
	cmp	r0, r2
	movge	r0, r2
	str	r0, [r4, #3996]
.L1351:
	add	r6, r6, #36864
	ldr	r2, [r6, #4016]
	ldr	r3, [r6, #4020]
	str	r0, [r6, #3992]
	str	r2, [r6, #4012]
	str	r3, [r6, #4024]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1340:
	str	r3, [r4, #4008]
	ldrb	r8, [r7, #1008]	@ zero_extendqisi2
.L1343:
	mov	r0, #0
	str	r0, [r4, #4040]
	ldr	r3, [r5, #2916]
	cmp	r3, r0
	ble	.L1346
.L1363:
	add	ip, r5, #2912
	mov	r3, #0
	add	ip, ip, #4
	mov	r2, r3
.L1345:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	add	r3, r3, r1
	str	r3, [r4, #4040]
	ldr	r1, [r5, #2916]
	cmp	r1, r2
	bgt	.L1345
	cmp	r0, #0
	bne	.L1379
.L1346:
	mov	r0, #0
	str	r0, [r4, #4036]
	b	.L1347
.L1378:
	add	r4, r0, #11075584
	add	r4, r4, #36864
.L1328:
	mov	r3, #0
	ldr	r0, [r4, #3964]
	str	r3, [r4, #3972]
	mov	lr, r3
	str	r3, [r4, #3976]
	mov	r1, ip
	b	.L1325
.L1377:
	add	r3, r6, #36864
	mov	r1, #0
	mov	r2, r1
	str	r1, [r3, #4020]
	str	r1, [r3, #3988]
	str	r1, [r3, #3984]
	str	r1, [r3, #3992]
	str	r1, [r3, #3996]
	ldr	r1, [r3, #4016]
	b	.L1354
.L1330:
	str	lr, [r4, #3980]
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1380
.L1331:
	ldrb	r3, [r7, #1002]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r1, r0
	streq	r3, [r4, #3984]
	strne	r3, [r4, #3988]
	str	r3, [r4, #3996]
	b	.L1332
.L1350:
	ldrb	r3, [r7, #1002]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r2, [r5, #2912]
	ldreq	r3, [r4, #4000]
	ldrne	r3, [r4, #4000]
	addne	r0, r0, r2
	addeq	r0, r0, r3
	streq	r0, [r4, #3984]
	addne	r0, r0, r3
	strne	r0, [r4, #3988]
	str	r0, [r4, #3996]
	b	.L1351
.L1326:
	movw	r3, #3961
	ldrsb	r3, [r4, r3]
	cmp	r3, #0
	bne	.L1328
	ldr	r0, [r4, #3984]
	mov	r1, ip
	mov	lr, r3
	str	r3, [r4, #3972]
	mov	ip, r0
	str	r0, [r4, #3976]
	b	.L1327
.L1337:
	ldr	r1, [r4, #4016]
	ldr	r2, [r4, #4012]
	cmp	r1, r2
	bcs	.L1339
	ldr	r2, [r4, #4024]
	add	r3, r3, r2
	str	r3, [r4, #4020]
	b	.L1336
.L1361:
	ldrb	r0, [r7, #1002]	@ zero_extendqisi2
	str	r3, [r4, #3992]
	cmp	r0, #0
	streq	r3, [r4, #3984]
	strne	r3, [r4, #3988]
	b	.L1354
.L1355:
	str	r1, [r4, #4012]
	mov	r2, r1
	str	r1, [r4, #4024]
	ldr	r1, [r4, #4016]
.L1357:
	str	r2, [r4, #4020]
	b	.L1358
.L1339:
	ldr	r3, [r4, #4024]
	b	.L1338
	UNWIND(.fnend)
	.size	MVC_DecPOC, .-MVC_DecPOC
	.align	2
	.global	MVC_CalcPicNum
	.type	MVC_CalcPicNum, %function
MVC_CalcPicNum:
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
	add	r5, r0, #11141120
	ldr	r3, [r0, #236]
	add	r5, r5, #12288
	mov	r6, #1
	ldrb	r2, [r5, #2779]	@ zero_extendqisi2
	ldr	r3, [r3, #2896]
	cmp	r2, #0
	add	r3, r3, #4
	mov	r6, r6, asl r3
	add	r3, r0, #11075584
	beq	.L1382
	add	r3, r3, #45056
	sub	r7, r2, #1
	sub	r2, r2, #2
	clz	r7, r7
	ldr	r1, [r3, #2864]
	clz	r2, r2
	mov	r7, r7, lsr #5
	cmp	r1, #0
	mov	r2, r2, lsr #5
	movwne	lr, #47780
	movne	r1, #0
	movtne	lr, 169
	addne	lr, r0, lr
	beq	.L1402
.L1401:
	ldr	ip, [lr, #4]!
	ldrb	r4, [ip, #3]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L1396
	ldr	r4, [ip, #20]
	ldr	r8, [r5, #3424]
	cmp	r4, r8
	rsbhi	r4, r6, r4
	str	r4, [ip, #24]
	ldr	ip, [lr]
	ldrb	r4, [ip, #3]	@ zero_extendqisi2
	tst	r4, #1
	beq	.L1399
	ldr	r8, [ip, #732]
	bic	r8, r8, #-16777216
	bic	r8, r8, #255
	cmp	r8, #65536
	ldreq	r4, [ip, #24]
	addeq	r4, r7, r4, lsl #1
	streq	r4, [ip, #744]
	ldreq	ip, [lr]
	ldreqb	r4, [ip, #3]	@ zero_extendqisi2
.L1399:
	tst	r4, #2
	beq	.L1396
	ldr	r4, [ip, #768]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	ldreq	r4, [ip, #24]
	addeq	r4, r2, r4, lsl #1
	streq	r4, [ip, #780]
.L1396:
	ldr	ip, [r3, #2864]
	add	r1, r1, #1
	cmp	ip, r1
	bhi	.L1401
.L1402:
	ldr	r1, [r3, #2868]
	cmp	r1, #0
	beq	.L1426
	movw	r4, #47844
	mov	ip, #0
	movt	r4, 169
	add	r4, r0, r4
.L1407:
	ldr	lr, [r4, #4]!
	add	ip, ip, #1
	ldrb	r1, [lr, #3]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1404
	tst	r1, #1
	beq	.L1405
	ldr	r0, [lr, #732]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #256
	ldreq	r1, [lr, #28]
	addeq	r1, r7, r1, lsl #1
	streq	r1, [lr, #740]
	ldreq	lr, [r4]
	ldreqb	r1, [lr, #3]	@ zero_extendqisi2
.L1405:
	tst	r1, #2
	beq	.L1404
	ldr	r1, [lr, #768]
	bic	r1, r1, #-16777216
	bic	r1, r1, #255
	cmp	r1, #256
	ldreq	r1, [lr, #28]
	addeq	r1, r2, r1, lsl #1
	streq	r1, [lr, #776]
.L1404:
	ldr	r1, [r3, #2868]
	cmp	r1, ip
	bhi	.L1407
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1382:
	add	r3, r3, #45056
	ldr	r1, [r3, #2864]
	cmp	r1, #0
	movwne	lr, #47780
	movtne	lr, 169
	addne	lr, r0, lr
	bne	.L1391
.L1392:
	ldr	r2, [r3, #2868]
	cmp	r2, #0
	beq	.L1427
	movw	ip, #47844
	mov	r2, #0
	movt	ip, 169
	add	ip, r0, ip
	b	.L1394
.L1393:
	ldr	r1, [r3, #2868]
	cmp	r1, r2
	bls	.L1428
.L1394:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1393
	ldr	r0, [r1, #696]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #256
	ldreq	r0, [r1, #28]
	streq	r0, [r1, #704]
	ldr	r1, [r3, #2868]
	cmp	r1, r2
	bhi	.L1394
.L1428:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1429:
	ldr	ip, [r1, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L1388
	ldr	ip, [r1, #20]
	ldr	r4, [r5, #3424]
	rsb	r7, r6, ip
	cmp	ip, r4
	strhi	r7, [r1, #24]
	strls	ip, [r1, #24]
	ldr	r1, [lr]
	ldr	ip, [r1, #24]
	str	ip, [r1, #708]
.L1388:
	ldr	r1, [r3, #2864]
	add	r2, r2, #1
	cmp	r1, r2
	bls	.L1392
.L1391:
	ldr	r1, [lr, #4]!
	ldrb	ip, [r1, #3]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1388
	b	.L1429
.L1427:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1426:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_CalcPicNum, .-MVC_CalcPicNum
	.align	2
	.global	MVC_IsOutDPB
	.type	MVC_IsOutDPB, %function
MVC_IsOutDPB:
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
	beq	.L1435
	add	r3, r0, #11075584
	add	r3, r3, #45056
	ldr	lr, [r3, #2856]
	cmp	lr, #0
	beq	.L1435
	ldr	r2, [r3, #2664]
	rsb	r3, r2, r1
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L1437
	movw	ip, #47720
	movt	ip, 169
	add	ip, r0, ip
	b	.L1432
.L1433:
	ldr	r2, [ip, #4]!
	rsb	r0, r2, r1
	cmp	r2, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L1437
.L1432:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L1433
.L1435:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1437:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_IsOutDPB, .-MVC_IsOutDPB
	.align	2
	.global	mvc_combine_scalinglist
	.type	mvc_combine_scalinglist, %function
mvc_combine_scalinglist:
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
	cmp	r2, #0
	ldmlefd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	ldr	r6, .L1445
	mov	lr, #0
	mov	r5, #255
	sub	r7, r6, #16
.L1442:
	cmp	r2, #16
	ldreqb	ip, [lr, r7]	@ zero_extendqisi2
	ldrneb	ip, [lr, r6]	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	lr, r2
	and	r3, ip, #3
	mov	r4, ip, lsr #2
	ldrb	r8, [r0, ip, asl #2]	@ zero_extendqisi2
	mov	r3, r3, asl #3
	ldr	ip, [r1, r4, asl #2]
	bic	ip, ip, r5, asl r3
	orr	r3, ip, r8, asl r3
	str	r3, [r1, r4, asl #2]
	bne	.L1442
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1446:
	.align	2
.L1445:
	.word	.LANCHOR0+16
	UNWIND(.fnend)
	.size	mvc_combine_scalinglist, .-mvc_combine_scalinglist
	.align	2
	.global	mvc_assign_quant_params
	.type	mvc_assign_quant_params, %function
mvc_assign_quant_params:
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
	subs	r3, r0, #0
	str	r1, [fp, #-48]
	str	r3, [fp, #-52]
	beq	.L1478
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L1479
	ldr	r3, [fp, #-48]
	ldrb	r3, [r3, #18]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1467
	ldr	r3, [fp, #-48]
	mov	r4, #0
	ldr	r1, [fp, #-52]
	add	r2, r3, #2016
	add	r8, r3, #1728
	str	r2, [fp, #-56]
	add	r9, r1, #2384
	mov	r6, r2
	add	r7, r3, #1984
	add	r5, r3, #5
	ldr	r10, .L1484
	b	.L1452
.L1482:
	cmp	r4, #0
	beq	.L1480
	cmp	r4, #3
	beq	.L1481
	ldr	r3, [r10, #52]
	mov	r2, #16
	sub	r1, r6, #16
	mov	r0, r6
	blx	r3
.L1460:
	add	r4, r4, #1
	cmp	r4, #7
	bhi	.L1467
.L1466:
	add	r8, r8, #64
	add	r9, r9, #64
	add	r6, r6, #16
	add	r7, r7, #4
	add	r5, r5, #1
.L1452:
	cmp	r4, #5
	ldrsb	r3, [r5]
	bhi	.L1453
	cmp	r3, #0
	beq	.L1482
	ldr	r3, [r7]
	cmp	r3, #0
	beq	.L1460
	ldr	r1, .L1484+4
	cmp	r4, #2
	mov	r2, #16
	ldr	r3, [r10, #52]
	add	r0, r1, r2
	add	r4, r4, #1
	movhi	r1, r0
	mov	r0, r6
	blx	r3
	cmp	r4, #7
	bls	.L1466
.L1467:
	mov	r0, #0
.L1449:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1453:
	cmp	r3, #0
	bne	.L1462
	ldr	r3, [fp, #-52]
	mov	r2, #64
	ldrb	r3, [r3, #27]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r10, #52]
	bne	.L1483
.L1477:
	ldr	r1, .L1484+8
	cmp	r4, #6
	add	r0, r1, r2
	movne	r1, r0
	mov	r0, r8
	blx	r3
	b	.L1460
.L1462:
	ldr	r3, [r7]
	cmp	r3, #0
	beq	.L1460
	ldr	r3, [r10, #52]
	mov	r2, #64
	b	.L1477
.L1483:
	mov	r1, r9
	mov	r0, r8
	blx	r3
	b	.L1460
.L1478:
	movw	r3, #6950
	ldr	r2, .L1484+12
	ldr	r1, .L1484+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1449
.L1479:
	mov	r0, r3
	ldr	r2, .L1484+12
	movw	r3, #6951
	ldr	r1, .L1484+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1449
.L1481:
	ldr	r3, [fp, #-52]
	mov	r2, #16
	ldr	r0, [fp, #-48]
	mov	r4, #4
	ldrb	r3, [r3, #27]	@ zero_extendqisi2
	add	r0, r0, #2064
	cmp	r3, #0
	ldr	r3, [r10, #52]
	ldrne	r1, [fp, #-52]
	ldreq	r1, .L1484+20
	addne	r1, r1, #2720
	blx	r3
	b	.L1466
.L1480:
	ldr	r3, [fp, #-52]
	mov	r2, #16
	ldr	r0, [fp, #-56]
	mov	r4, #1
	ldrb	r3, [r3, #27]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r10, #52]
	ldrne	r1, [fp, #-52]
	ldreq	r1, .L1484+4
	addne	r1, r1, #2672
	blx	r3
	b	.L1466
.L1485:
	.align	2
.L1484:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1
	.word	.LANCHOR1+32
	.word	.LC13
	.word	.LC14
	.word	.LANCHOR1+16
	UNWIND(.fnend)
	.size	mvc_assign_quant_params, .-mvc_assign_quant_params
	.align	2
	.global	MVC_WriteCurrPicYUV
	.type	MVC_WriteCurrPicYUV, %function
MVC_WriteCurrPicYUV:
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
	add	r5, r0, #11141120
	add	r3, r5, #12288
	add	r5, r5, #12288
	ldr	r1, .L1489
	mov	r0, #2
	ldrb	r4, [r3, #2779]	@ zero_extendqisi2
	ldr	r3, [r3, #3416]
	mov	r2, r4
	ldr	r3, [r3, #228]
	bl	dprint_vfmw
	ldr	ip, [r5, #3476]
	cmp	r4, #3
	cmpne	r4, #0
	ldr	r3, [r5, #3472]
	addne	r2, r4, #1
	ldr	r1, .L1489+4
	moveq	r2, #1
	str	ip, [sp]
	mov	r0, #22
	bl	dprint_vfmw
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1490:
	.align	2
.L1489:
	.word	.LC32
	.word	.LC33
	UNWIND(.fnend)
	.size	MVC_WriteCurrPicYUV, .-MVC_WriteCurrPicYUV
	.align	2
	.global	MVC_WritePicMsg
	.type	MVC_WritePicMsg, %function
MVC_WritePicMsg:
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
	mov	r6, r0
	add	r4, r0, #11141120
	add	r7, r6, #11075584
	add	r10, r4, #12288
	add	ip, r7, #32768
	str	ip, [fp, #-52]
	mov	r0, #2
	ldr	lr, [r10, #3492]
	mov	r5, ip
	ldr	ip, [r10, #2948]
	add	r8, r4, #16384
	ldrb	r3, [r10, #2779]	@ zero_extendqisi2
	add	r9, r7, #36864
	ldr	r2, [r10, #3432]
	ldr	r1, .L1527
	stmia	sp, {ip, lr}
	bl	dprint_vfmw
	add	r2, r4, #15680
	mov	r3, #0
	str	r3, [r6, #64]
	ldrd	r0, [r2]
	movw	r3, #17104
	movt	r3, 170
	strd	r0, [r6, r3]
	ldr	r3, [r6, #544]
	str	r3, [r8, #1720]
	ldr	r3, [r10, #2932]
	str	r3, [r8, #1500]
	ldr	r3, [r10, #2936]
	str	r3, [r8, #1504]
	ldr	r3, [r10, #2940]
	str	r3, [r8, #1508]
	ldr	r3, [r10, #3432]
	str	r3, [r8, #728]
	ldrb	r3, [r10, #2779]	@ zero_extendqisi2
	strb	r3, [r8, #712]
	ldr	r3, [r6, #236]
	ldrb	r2, [r3, #21]	@ zero_extendqisi2
	str	r2, [r8, #744]
	ldr	r2, [r10, #3476]
	str	r2, [r8, #732]
	ldr	r2, [r10, #3480]
	str	r2, [r8, #736]
	ldr	r2, [r3, #3948]
	add	r2, r2, #1
	str	r2, [r8, #740]
	ldrb	r2, [r5, #1844]	@ zero_extendqisi2
	strb	r2, [r8, #715]
	ldr	r2, [r9, #1780]
	str	r2, [r8, #748]
	ldrb	r2, [r9, #1720]	@ zero_extendqisi2
	str	r2, [r8, #752]
	ldr	r3, [r3, #748]
	strb	r3, [r8, #713]
	ldrb	r3, [r9, #1724]	@ zero_extendqisi2
	str	r3, [r8, #756]
	ldr	r3, [r9, #3996]
	str	r3, [r8, #760]
	ldr	r3, [r9, #3984]
	str	r3, [r8, #764]
	ldr	r3, [r9, #3988]
	str	r3, [r8, #768]
	ldr	r3, [r10, #3416]
	ldr	r0, [r6, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetLogicFs
	subs	r5, r0, #0
	beq	.L1525
	ldr	r2, [r5, #640]
	ldr	r3, [r5, #644]
	cmp	r2, #0
	beq	.L1495
	cmp	r3, #0
	beq	.L1495
	movw	r3, #17532
	movw	r2, #17404
	movw	r1, #17792
	ldr	r0, [r6, #120]
	movt	r3, 170
	movt	r2, 170
	add	r3, r6, r3
	add	r2, r6, r2
	movt	r1, 170
	add	r1, r6, r1
	bl	FSP_GetDecFsAddrTab
	ldr	r3, [r5, #640]
	movw	r2, #17796
	movw	r1, #17876
	movt	r2, 170
	movt	r1, 170
	ldr	r3, [r3, #8]
	add	r2, r6, r2
	add	r1, r6, r1
	add	r7, r7, #45056
	str	r3, [r8, #1004]
	ldr	r0, [r6, #120]
	bl	FSP_GetPmvAddrTab
	ldr	r3, [r10, #3492]
	str	r3, [r8, #1012]
	ldr	r3, [r5, #656]
	str	r3, [r8, #1496]
	ldr	r3, [r5, #644]
	ldr	r3, [r3, #16]
	str	r3, [r8, #1008]
	ldr	r3, [r5, #644]
	ldr	r3, [r3, #76]
	str	r3, [r8, #1016]
	ldr	r3, [r7, #3084]
	cmp	r3, #0
	str	r3, [fp, #-48]
	str	r3, [r8, #1712]
	beq	.L1501
	movw	r8, #47944
	movw	lr, #17900
	movw	ip, #48008
	movw	r0, #17964
	movw	r1, #48072
	movw	r2, #18028
	ldr	r10, [fp, #-48]
	movt	r8, 169
	movt	lr, 170
	movt	ip, 169
	movt	r0, 170
	movt	r1, 169
	movt	r2, 170
	add	r8, r6, r8
	add	lr, r6, lr
	add	ip, r6, ip
	add	r0, r6, r0
	add	r1, r6, r1
	add	r2, r6, r2
	mov	r3, #0
.L1500:
	ldr	r5, [r8, #4]!
	add	r3, r3, #1
	cmp	r3, r10
	str	r5, [lr, #4]!
	ldr	r5, [ip, #4]!
	str	r5, [r0, #4]!
	ldr	r5, [r1, #4]!
	str	r5, [r2, #4]!
	bne	.L1500
.L1501:
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3, #1851]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1526
	ldrb	ip, [r9, #1738]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L1502
	ldr	r0, .L1527+4
	add	r4, r4, #17152
.L1503:
	and	r3, ip, #3
	add	ip, ip, #1
	cmp	ip, #24
	add	r3, r0, r3, lsl #2
	ldr	r3, [r3, #352]
	bic	r2, r3, #16711680
	ubfx	r1, r3, #8, #8
	bic	r2, r2, #65280
	mov	r3, r3, lsr #8
	orr	r2, r2, r1, asl #16
	and	r3, r3, #65280
	orr	r3, r2, r3
	str	r3, [r4, #4]!
	bne	.L1503
	movw	r8, #17252
	mov	r5, #0
	movt	r8, 170
	add	r8, r6, r8
.L1504:
	add	r3, r5, #1
	and	r2, r5, #14
	and	r3, r3, #15
	add	r5, r5, #2
	add	r2, r0, r2, lsl #2
	cmp	r5, #32
	add	r3, r0, r3, lsl #2
	ldr	r1, [r2, #368]
	ldr	ip, [r3, #368]
	ubfx	r3, r1, #8, #8
	mov	r2, r1, lsr #24
	uxtb	r9, r1
	uxtb	r4, ip
	mov	lr, ip, lsr #16
	orr	r3, r3, r2, asl #8
	mov	r1, r1, lsr #8
	mov	r4, r4, asl #16
	and	r2, ip, #-16777216
	orr	lr, r4, lr, asl #24
	ubfx	ip, ip, #8, #8
	orr	r3, r3, r2
	orr	lr, lr, r9
	and	r1, r1, #65280
	orr	r3, r3, ip, asl #16
	orr	r2, lr, r1
	stmia	r8, {r2, r3}
	add	r8, r8, #8
	bne	.L1504
.L1509:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L1506
	movw	r4, #47948
	movw	r9, #18232
	movw	r8, #18296
	movt	r4, 169
	movt	r9, 170
	movt	r8, 170
	add	r4, r6, r4
	add	r9, r6, r9
	add	r8, r6, r8
	mov	r5, #0
.L1512:
	ldr	r2, [r4]
	mov	r1, #0
	ldr	r0, [r6, #120]
	add	r5, r5, #1
	bl	FSP_GetStoreType
	adds	r0, r0, #0
	movne	r0, #1
	str	r0, [r9, #4]!
	ldr	r3, [r4], #4
	str	r3, [r8, #4]!
	ldr	r3, [r7, #3084]
	cmp	r3, r5
	bhi	.L1512
.L1506:
	mov	r0, #0
.L1522:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1526:
	ldrb	ip, [r9, #1738]	@ zero_extendqisi2
.L1502:
	cmp	ip, #1
	add	r4, r4, #17152
	mov	r0, #0
	beq	.L1507
.L1510:
	movw	r3, #25700
	movt	r3, 42
	add	r3, r0, r3
	add	r0, r0, #1
	ldr	r3, [r6, r3, asl #2]
	cmp	r0, #24
	bic	r2, r3, #16711680
	ubfx	r1, r3, #8, #8
	bic	r2, r2, #65280
	mov	r3, r3, lsr #8
	orr	r2, r2, r1, asl #16
	and	r3, r3, #65280
	orr	r3, r2, r3
	str	r3, [r4, #4]!
	bne	.L1510
	movw	r9, #17252
	mov	r10, #1
	movt	r9, 170
	add	r9, r6, r9
	mov	r8, #0
.L1511:
	mov	r2, r8, lsr #3
	mov	r3, r8, asl #1
	and	r1, r3, #14
	and	r0, r10, #15
	mov	r3, r2, asl #4
	movw	r2, #25724
	add	r0, r3, r0
	add	r3, r3, r1
	mov	r1, r2
	movt	r2, 42
	movt	r1, 42
	add	r2, r0, r2
	add	r1, r3, r1
	add	r8, r8, #1
	ldr	r2, [r6, r2, asl #2]
	cmp	r8, #16
	ldr	r3, [r6, r1, asl #2]
	add	r10, r10, #2
	ubfx	r5, r2, #8, #8
	and	lr, r2, #-16777216
	mov	r0, r2, lsr #16
	uxtb	r1, r2
	mov	ip, r3, lsr #24
	uxtb	r4, r3
	mov	r2, r0, asl #24
	mov	r0, r3, lsr #8
	mov	ip, ip, asl #8
	orr	r2, r2, r1, asl #16
	orr	ip, ip, r5, asl #16
	orr	r1, r2, r4
	ubfx	r3, r3, #8, #8
	and	r2, r0, #65280
	orr	ip, ip, lr
	orr	r2, r1, r2
	orr	r3, ip, r3
	stmia	r9, {r2, r3}
	add	r9, r9, #8
	bne	.L1511
	b	.L1509
.L1507:
	movw	r3, #26534
	movt	r3, 42
	add	r3, r0, r3
	add	r0, r0, #1
	ldr	r3, [r6, r3, asl #2]
	cmp	r0, #24
	bic	r2, r3, #16711680
	ubfx	r1, r3, #8, #8
	bic	r2, r2, #65280
	mov	r3, r3, lsr #8
	orr	r2, r2, r1, asl #16
	and	r3, r3, #65280
	orr	r3, r2, r3
	str	r3, [r4, #4]!
	bne	.L1507
	movw	r9, #17252
	mov	r10, #1
	movt	r9, 170
	add	r9, r6, r9
	mov	r8, #0
.L1508:
	mov	r2, r8, lsr #3
	mov	r3, r8, asl #1
	and	r1, r3, #14
	and	r0, r10, #15
	mov	r3, r2, asl #4
	movw	r2, #26558
	add	r0, r3, r0
	add	r3, r3, r1
	mov	r1, r2
	movt	r2, 42
	movt	r1, 42
	add	r2, r0, r2
	add	r1, r3, r1
	add	r8, r8, #1
	ldr	r2, [r6, r2, asl #2]
	cmp	r8, #16
	ldr	r3, [r6, r1, asl #2]
	add	r10, r10, #2
	ubfx	r5, r2, #8, #8
	and	lr, r2, #-16777216
	mov	r0, r2, lsr #16
	uxtb	r1, r2
	mov	ip, r3, lsr #24
	uxtb	r4, r3
	mov	r2, r0, asl #24
	mov	r0, r3, lsr #8
	mov	ip, ip, asl #8
	orr	r2, r2, r1, asl #16
	orr	ip, ip, r5, asl #16
	orr	r1, r2, r4
	ubfx	r3, r3, #8, #8
	and	r2, r0, #65280
	orr	ip, ip, lr
	orr	r2, r1, r2
	orr	r3, ip, r3
	stmia	r9, {r2, r3}
	add	r9, r9, #8
	bne	.L1508
	b	.L1509
.L1495:
	ldr	r1, .L1527+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1522
.L1525:
	ldr	r3, [r10, #3416]
	ldr	r1, .L1527+12
	ldrsb	r2, [r3, #6]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1522
.L1528:
	.align	2
.L1527:
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC36
	.word	.LC35
	UNWIND(.fnend)
	.size	MVC_WritePicMsg, .-MVC_WritePicMsg
	.align	2
	.global	MVC_UpdatePicQpInf
	.type	MVC_UpdatePicQpInf, %function
MVC_UpdatePicQpInf:
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
	.size	MVC_UpdatePicQpInf, .-MVC_UpdatePicQpInf
	.align	2
	.global	MVC_WriteSliceMsg
	.type	MVC_WriteSliceMsg, %function
MVC_WriteSliceMsg:
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
	ldr	r3, [r0, #64]
	movw	r1, #4060
	mov	r4, r0
	mov	ip, r3
	str	r3, [fp, #-48]
	mul	r1, r1, ip
	ldr	r3, [r0, #32]
	ldr	r0, [r0, #544]
	add	r2, ip, #1
	sub	r3, r3, #1
	str	r2, [r4, #64]
	cmp	ip, r3
	add	r6, r0, r1
	addcc	r3, r1, #4048
	addcc	r3, r3, #12
	movcs	r3, #0
	addcc	r3, r0, r3
	str	r3, [r6, #4056]
	ldr	r2, [r4, #232]
	ldrb	ip, [r2]	@ zero_extendqisi2
	ldr	r3, [r2, #68]
	cmp	ip, #1
	cmpls	ip, r3
	bcs	.L1533
	add	lr, ip, #3
	add	lr, r6, lr, lsl #2
.L1534:
	mov	r3, ip, asl #5
	sub	r3, r3, ip, asl #2
	add	ip, ip, #1
	add	r2, r2, r3
	ldr	r2, [r2, #24]
	str	r2, [lr, #4]!
	ldr	r2, [r4, #232]
	add	r2, r2, r3
	ldr	r2, [r2, #20]
	str	r2, [lr, #-8]
	ldr	r2, [r4, #232]
	add	r3, r2, r3
	ldr	r3, [r3, #28]
	str	r3, [lr, #8]
	ldr	r2, [r4, #232]
	ldr	r3, [r2, #68]
	cmp	r3, ip
	movhi	r5, #1
	movls	r5, #0
	cmp	ip, #1
	movhi	r5, #0
	cmp	r5, #0
	bne	.L1534
.L1533:
	cmp	r3, #1
	bhi	.L1539
	add	r3, r3, #3
	add	ip, r6, #20
	mov	r2, #0
	add	r3, r6, r3, lsl #2
.L1538:
	str	r2, [r3, #4]!
	cmp	r3, ip
	str	r2, [r3, #-8]
	str	r2, [r3, #8]
	bne	.L1538
.L1539:
	add	r3, r4, #11141120
	str	r3, [fp, #-52]
	add	r10, r3, #12288
	ldr	r3, [fp, #-48]
	add	r5, r4, #11075584
	cmp	r3, #0
	ldrb	r3, [r10, #2785]	@ zero_extendqisi2
	add	r8, r5, #36864
	add	r5, r5, #40960
	strb	r3, [r6, #1]
	ldr	r3, [r8, #1768]
	ldr	r2, [r5, #2616]
	add	r3, r3, #26
	add	r3, r3, r2
	str	r3, [r6, #32]
	bne	.L1635
	ldr	r2, [r10, #3416]
	str	r3, [r2, #724]
	ldr	r3, [r10, #3416]
	ldr	r2, [r6, #32]
	str	r2, [r3, #728]
.L1540:
	ldr	r3, [r5, #2612]
	str	r3, [r6, #36]
	ldr	r3, [r5, #1064]
	str	r3, [r6, #40]
	ldr	r3, [r5, #1060]
	str	r3, [r6, #44]
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	strb	r3, [r0, r1]
	ldr	r3, [r5, #1056]
	str	r3, [r6, #48]
	ldrb	r3, [r5, #1006]	@ zero_extendqisi2
	strb	r3, [r6, #2]
	ldr	r3, [r4, #236]
	ldrb	r3, [r3, #22]	@ zero_extendqisi2
	strb	r3, [r6, #3]
	ldr	r3, [r5, #1048]
	str	r3, [r6, #52]
	ldr	r3, [r5, #1052]
	str	r3, [r6, #56]
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r8, #1764]
	streqb	r3, [r6, #4]
	beq	.L1542
	cmp	r3, #0
	ldreqb	r3, [r8, #1722]	@ zero_extendqisi2
	movne	r3, #0
	strb	r3, [r6, #4]
.L1542:
	ldr	r3, [r8, #1772]
	str	r3, [r6, #60]
	ldr	r3, [r8, #1776]
	str	r3, [r6, #64]
	ldr	r3, [r5, #2624]
	str	r3, [r6, #68]
	ldr	r3, [r5, #2628]
	str	r3, [r6, #72]
	ldr	r3, [r5, #2620]
	str	r3, [r6, #76]
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1581
	ldr	r7, [r5, #1048]
	cmp	r7, #0
	beq	.L1584
	ldr	r3, [r4, #256]
	ldr	r2, [r3, #4]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r1, r4, #256
	movne	r3, #0
	bne	.L1549
	b	.L1547
.L1551:
	ldr	r2, [r1, #4]!
	ldr	r2, [r2, #4]
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1547
.L1549:
	add	r3, r3, #1
	cmp	r3, r7
	bne	.L1551
	mov	r3, #0
	str	r3, [fp, #-56]
.L1546:
	ldr	ip, [r5, #1052]
	cmp	ip, #0
	beq	.L1585
	ldr	r3, [r4, #388]
	ldr	r2, [r3, #4]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r1, r4, #388
	movne	r3, #0
	bne	.L1555
	b	.L1553
.L1557:
	ldr	r2, [r1, #4]!
	ldr	r2, [r2, #4]
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1553
.L1555:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1557
	mov	r3, #0
	str	r3, [fp, #-60]
.L1552:
	ldrb	ip, [r10, #2779]	@ zero_extendqisi2
	ldr	r3, [r5, #1048]
	cmp	ip, #0
	beq	.L1558
	cmp	r3, #0
	beq	.L1573
	mov	r7, #0
	mov	r9, r6
	str	r6, [fp, #-64]
	add	r10, r4, #256
	mov	r6, r5
	mov	r5, r7
	ldr	r7, [fp, #-56]
	b	.L1572
.L1570:
	strb	r2, [r9, #1630]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r9, #1631]
	ldr	r2, [r10]
	ldr	r2, [r2, #16]
	str	r2, [r9, #1656]
.L1571:
	ldr	r2, [r6, #1048]
	add	r5, r5, #1
	add	r10, r10, #4
	add	r9, r9, #36
	cmp	r2, r5
	bls	.L1636
.L1572:
	ldr	r2, [r10]
	ldr	r0, [r4, #120]
	ldr	r2, [r2, #4]
	ldrsb	r1, [r2, #6]
	bl	FSP_GetLogicFs
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #48]
	str	r2, [r9, #1636]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	strb	r2, [r9, #1625]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r7
	beq	.L1569
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1569:
	str	r0, [r9, #1640]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #52]
	str	r2, [r9, #1644]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #32]
	str	r2, [r9, #1632]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r9, #1624]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #696]	@ zero_extendqisi2
	strb	r2, [r9, #1626]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1570
	strb	r2, [r9, #1628]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r9, #1629]
	ldr	r2, [r10]
	ldr	r2, [r2, #16]
	str	r2, [r9, #1652]
	b	.L1571
.L1636:
	mov	r5, r6
	ldr	r6, [fp, #-64]
.L1573:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1637
.L1561:
	movw	r2, #48144
	add	r3, r6, #3920
	add	r0, r6, #4048
	movt	r2, 169
	add	r3, r3, #4
	add	r2, r4, r2
	add	r0, r0, #4
.L1578:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L1578
	ldr	r2, [fp, #-52]
	ldrb	r3, [r8, #1722]	@ zero_extendqisi2
	add	r9, r2, #16384
	cmp	r3, #0
	str	r3, [r9, #996]
	ldr	r3, [r8, #1764]
	str	r3, [r9, #1000]
	beq	.L1579
	ldrb	r2, [r5, #1000]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1580
.L1579:
	cmp	r3, #1
	beq	.L1638
.L1581:
	ldr	r2, [fp, #-48]
	movw	r3, #36904
	movt	r3, 42
	ldr	r1, [r4, #232]
	add	r3, r2, r3
	mov	r2, #0
	add	r3, r4, r3, lsl #2
	str	r1, [r3, #4]
	str	r2, [r4, #232]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1635:
	ldr	r2, [r10, #3416]
	ldr	lr, [r2, #724]
	ldr	ip, [r2, #728]
	cmp	lr, r3
	movlt	lr, r3
	cmp	ip, r3
	str	lr, [r2, #724]
	movge	ip, r3
	str	ip, [r2, #728]
	b	.L1540
.L1547:
	ldrsb	r1, [r2, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	movweq	r3, #8012
	beq	.L1631
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-56]
	b	.L1546
.L1638:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1581
.L1580:
	ldr	r3, [r5, #1068]
	movw	r8, #42032
	movw	r7, #42160
	movw	lr, #42288
	movw	ip, #42800
	movw	r0, #42928
	str	r3, [r6, #80]
	movw	r1, #43056
	ldr	r10, [r5, #1072]
	movt	r8, 169
	movt	r7, 169
	movt	lr, 169
	movt	ip, 169
	movt	r0, 169
	movt	r1, 169
	add	r8, r4, r8
	add	r7, r4, r7
	add	lr, r4, lr
	add	ip, r4, ip
	add	r0, r4, r0
	add	r1, r4, r1
	mov	r9, r6
	mov	r3, r6
	mov	r2, #0
	str	r6, [fp, #-52]
	str	r10, [r9, #84]!
.L1582:
	ldr	r10, [r8, #4]!
	add	r2, r2, #1
	add	r3, r3, #4
	str	r10, [r9, #4]!
	ldr	r10, [r7, #4]!
	str	r10, [r3, #212]
	ldr	r10, [lr, #4]!
	str	r10, [r3, #340]
	ldr	r10, [ip, #4]!
	str	r10, [r3, #852]
	ldr	r10, [r0, #4]!
	str	r10, [r3, #980]
	ldr	r10, [r1, #4]!
	str	r10, [r3, #1108]
	ldr	r10, [r5, #1060]
	cmp	r10, r2
	bcs	.L1582
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1581
	movw	r7, #42416
	movw	lr, #42544
	movw	ip, #42672
	movw	r0, #43184
	movw	r1, #43312
	movw	r2, #43440
	ldr	r8, [fp, #-52]
	movt	r7, 169
	movt	lr, 169
	movt	ip, 169
	movt	r0, 169
	movt	r1, 169
	movt	r2, 169
	add	r6, r6, #468
	add	r7, r4, r7
	add	lr, r4, lr
	add	ip, r4, ip
	add	r0, r4, r0
	add	r1, r4, r1
	add	r2, r4, r2
	mov	r3, #0
.L1583:
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
	ldr	r9, [r5, #1064]
	cmp	r9, r3
	bcs	.L1583
	b	.L1581
.L1558:
	cmp	r3, #0
	beq	.L1566
	ldr	r9, [fp, #-56]
	add	r10, r4, #256
	mov	r7, r6
	str	r6, [fp, #-64]
	mov	r6, r5
	mov	r5, ip
.L1565:
	ldr	r2, [r10]
	ldr	r0, [r4, #120]
	ldr	r2, [r2, #4]
	ldrsb	r1, [r2, #6]
	bl	FSP_GetLogicFs
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #48]
	str	r2, [r7, #1636]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	strb	r2, [r7, #1625]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r9
	beq	.L1564
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1564:
	str	r0, [r7, #1640]
	mov	r3, #0
	ldr	r2, [r10]
	add	r5, r5, #1
	add	r7, r7, #36
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #52]
	str	r2, [r7, #1608]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #32]
	str	r2, [r7, #1596]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r7, #1591]
	ldr	r2, [r10]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r7, #1590]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #712]
	str	r2, [r7, #1612]
	ldr	r2, [r10]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #748]
	str	r2, [r7, #1616]
	ldr	r2, [r10], #4
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #784]
	strb	r3, [r7, #1588]
	str	r2, [r7, #1620]
	ldr	r2, [r6, #1048]
	cmp	r2, r5
	bhi	.L1565
	mov	r5, r6
	ldr	r6, [fp, #-64]
.L1566:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1561
	ldr	r3, [r5, #1052]
	cmp	r3, #0
	beq	.L1561
	mov	r9, #0
	mov	r10, r6
	str	r6, [fp, #-56]
	mov	r6, r5
	mov	r5, r9
	ldr	r9, [fp, #-60]
	add	r7, r4, #388
.L1568:
	ldr	r2, [r7]
	ldr	r0, [r4, #120]
	ldr	r2, [r2, #4]
	ldrsb	r1, [r2, #6]
	bl	FSP_GetLogicFs
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #48]
	str	r2, [r10, #2788]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	strb	r2, [r10, #2777]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r9
	beq	.L1567
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1567:
	str	r0, [r10, #1640]
	mov	r3, #0
	ldr	r2, [r7]
	add	r5, r5, #1
	add	r10, r10, #36
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #52]
	str	r2, [r10, #2760]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #32]
	str	r2, [r10, #2748]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2743]
	ldr	r2, [r7]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r10, #2742]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #712]
	str	r2, [r10, #2764]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #748]
	str	r2, [r10, #2768]
	ldr	r2, [r7], #4
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #784]
	strb	r3, [r10, #2740]
	str	r2, [r10, #2772]
	ldr	r2, [r6, #1052]
	cmp	r2, r5
	bhi	.L1568
.L1630:
	mov	r5, r6
	ldr	r6, [fp, #-56]
	b	.L1561
.L1553:
	ldrsb	r1, [r2, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L1639
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-60]
	b	.L1552
.L1637:
	ldr	r3, [r5, #1052]
	cmp	r3, #0
	beq	.L1561
	mov	r3, #0
	mov	r10, r6
	str	r6, [fp, #-56]
	add	r7, r4, #388
	mov	r6, r5
	ldr	r9, [fp, #-60]
	mov	r5, r3
	b	.L1577
.L1575:
	strb	r2, [r10, #2782]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2783]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r10, #2808]
.L1576:
	ldr	r2, [r6, #1052]
	add	r5, r5, #1
	add	r7, r7, #4
	add	r10, r10, #36
	cmp	r2, r5
	bls	.L1630
.L1577:
	ldr	r2, [r7]
	ldr	r0, [r4, #120]
	ldr	r2, [r2, #4]
	ldrsb	r1, [r2, #6]
	bl	FSP_GetLogicFs
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #48]
	str	r2, [r10, #2788]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	strb	r2, [r10, #2777]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r0, r9
	beq	.L1574
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1574:
	str	r0, [r10, #2792]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #52]
	str	r2, [r10, #2796]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldr	r2, [r2, #32]
	str	r2, [r10, #2784]
	ldr	r2, [r7]
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r10, #2776]
	ldr	r2, [r7]
	ldr	r2, [r2, #4]
	ldrb	r2, [r2, #696]	@ zero_extendqisi2
	strb	r2, [r10, #2778]
	ldr	r2, [r7]
	ldrb	r2, [r2]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1575
	strb	r2, [r10, #2780]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2781]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r10, #2804]
	b	.L1576
.L1639:
	movw	r3, #8033
.L1631:
	ldr	r2, .L1640
	ldr	r1, .L1640+4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L1584:
	str	r7, [fp, #-56]
	b	.L1546
.L1585:
	str	ip, [fp, #-60]
	b	.L1552
.L1641:
	.align	2
.L1640:
	.word	.LANCHOR0+80
	.word	.LC37
	UNWIND(.fnend)
	.size	MVC_WriteSliceMsg, .-MVC_WriteSliceMsg
	.align	2
	.global	MVC_StopPicNum
	.type	MVC_StopPicNum, %function
MVC_StopPicNum:
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
	.size	MVC_StopPicNum, .-MVC_StopPicNum
	.align	2
	.global	MVC_GetPicStreamSize
	.type	MVC_GetPicStreamSize, %function
MVC_GetPicStreamSize:
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
	add	r0, r0, #11141120
	add	r0, r0, #16384
	ldr	r2, [r0, #1720]
	cmp	r2, #0
	beq	.L1646
	mov	r0, #0
.L1645:
	ldr	r3, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r1
	add	r3, r3, #7
	cmp	r2, #0
	add	r0, r0, r3, lsr #3
	bne	.L1645
	ldmfd	sp, {fp, sp, pc}
.L1646:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GetPicStreamSize, .-MVC_GetPicStreamSize
	.align	2
	.global	MVC_SliceCheck
	.type	MVC_SliceCheck, %function
MVC_SliceCheck:
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
	ldr	r1, .L1659
	mov	r4, r0
	add	r5, r0, #11075584
	bl	mvc_ue_v
	add	r5, r5, #40960
	str	r0, [r5, #1056]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1657
	cmp	r0, #262144
	bcs	.L1658
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #16]
	mul	r3, r3, r2
	sub	r3, r3, #1
	cmp	r0, r3
	bhi	.L1657
	ldr	r1, .L1659+4
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L1659+8
	mov	r2, r0
	mov	r6, r0
	mov	r0, #19
	bl	dprint_vfmw
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1657
	cmp	r6, #9
	bhi	.L1653
	mov	r1, #1
	movw	r0, #297
	mov	r2, r1, asl r6
	and	r0, r0, r2
	cmp	r0, #0
	bne	.L1654
	ands	r3, r2, #660
	bne	.L1655
	tst	r2, #66
	beq	.L1653
	strb	r1, [r5, #1000]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1654:
	strb	r3, [r5, #1000]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1653:
	mov	r2, r6
	ldr	r1, .L1659+12
	mov	r0, #1
	bl	dprint_vfmw
.L1657:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1655:
	mov	r3, #2
	strb	r3, [r5, #1000]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1658:
	ldr	r1, .L1659+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1660:
	.align	2
.L1659:
	.word	.LC38
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LC39
	UNWIND(.fnend)
	.size	MVC_SliceCheck, .-MVC_SliceCheck
	.align	2
	.global	MVC_PPSSPSCheck
	.type	MVC_PPSSPSCheck, %function
MVC_PPSSPSCheck:
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
	add	r5, r0, #11075584
	mov	r4, r0
	add	r5, r5, #40960
	ldr	r1, .L1673
	mov	r0, #19
	ldr	r2, [r5, #1016]
	bl	dprint_vfmw
	ldr	r3, [r4, #252]
	ldr	r2, [r5, #1016]
	mov	r1, #2240
	mla	r1, r1, r2, r3
	ldrb	r3, [r1, #19]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1671
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
	sxtb	r5, r3
	cmn	r5, #1
	beq	.L1672
	cmp	r3, #0
	bne	.L1666
	ldr	r2, [r1, #28]
	movw	r3, #8500
	movt	r3, 5
	mul	r3, r3, r2
	add	r0, r4, r3
	add	ip, r0, #12992
	add	r3, r0, #286720
	add	ip, ip, #8
	add	r3, r3, #932
	ldrb	ip, [ip, #4]	@ zero_extendqisi2
	cmp	ip, #0
	beq	.L1667
	add	r0, r0, #286720
	ldrb	r0, [r0, #957]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1667
.L1665:
	mov	r0, #0
	str	r1, [r4, #244]
	str	r3, [r4, #240]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1672:
	ldr	r0, [r4, #248]
	movw	r3, #3992
	ldr	r2, [r1, #28]
	mla	r3, r3, r2, r0
	ldrb	r0, [r3, #25]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1665
	ldr	r1, .L1673+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1667:
	ldr	r1, .L1673+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1671:
	ldr	r1, .L1673+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1666:
	mov	r2, r5
	ldr	r1, .L1673+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1674:
	.align	2
.L1673:
	.word	.LC43
	.word	.LC45
	.word	.LC46
	.word	.LC44
	.word	.LC31
	UNWIND(.fnend)
	.size	MVC_PPSSPSCheck, .-MVC_PPSSPSCheck
	.align	2
	.global	MVC_PPSSPSCheckTmpId
	.type	MVC_PPSSPSCheckTmpId, %function
MVC_PPSSPSCheckTmpId:
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
	ldr	r1, .L1684
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r2, [r4, #252]
	mov	r3, #2240
	mla	r3, r3, r5, r2
	ldrb	r2, [r3, #19]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1681
	ldr	r1, [r4, #36]
	ldr	r2, [r3, #28]
	sub	r3, r1, #1
	cmp	r2, r3
	bhi	.L1682
	movw	r3, #41972
	movt	r3, 169
	add	r3, r4, r3
	ldrsb	r5, [r3]
	cmn	r5, #1
	beq	.L1683
.L1680:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1683:
	ldr	r1, [r4, #248]
	movw	r3, #3992
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #25]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1680
	ldr	r1, .L1684+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1681:
	mov	r2, r5
	ldr	r1, .L1684+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1682:
	ldr	r1, .L1684+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1685:
	.align	2
.L1684:
	.word	.LC47
	.word	.LC50
	.word	.LC48
	.word	.LC49
	UNWIND(.fnend)
	.size	MVC_PPSSPSCheckTmpId, .-MVC_PPSSPSCheckTmpId
	.align	2
	.global	MVC_IsNewPic
	.type	MVC_IsNewPic, %function
MVC_IsNewPic:
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
	add	ip, r0, #11075584
	ldr	lr, [r0, #236]
	add	r3, ip, #40960
	ldr	r1, [lr, #744]
	ldr	r0, [r3, #2664]
	ldr	r2, [r3, #2668]
	ldr	r4, [r3, #1016]
	ldr	r6, [r3, #2672]
	cmp	r2, r4
	cmpeq	r0, r1
	ldr	r5, [r3, #1028]
	ldrb	r4, [r3, #2656]	@ zero_extendqisi2
	ldrb	r0, [r3, #1001]	@ zero_extendqisi2
	movne	r1, #1
	moveq	r1, #0
	cmp	r6, r5
	moveq	r2, r1
	orrne	r2, r1, #1
	cmp	r4, r0
	orrne	r2, r2, #1
	cmp	r4, #0
	cmpne	r0, #0
	beq	.L1688
	ldrb	r0, [r3, #2657]	@ zero_extendqisi2
	ldrb	r1, [r3, #1002]	@ zero_extendqisi2
	cmp	r0, r1
	orrne	r2, r2, #1
.L1688:
	ldrb	r1, [r3, #2659]	@ zero_extendqisi2
	ldrb	r0, [r3, #1008]	@ zero_extendqisi2
	cmp	r1, r0
	moveq	r1, #0
	beq	.L1689
	cmp	r0, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
.L1689:
	ldrb	r0, [r3, #2658]	@ zero_extendqisi2
	ldrb	r4, [r3, #1003]	@ zero_extendqisi2
	cmp	r0, r4
	orrne	r2, r2, #1
	cmp	r4, #5
	cmpeq	r0, #5
	orr	r2, r2, r1
	beq	.L1701
.L1690:
	ldr	r1, [lr, #2900]
	cmp	r1, #0
	bne	.L1691
	add	r1, ip, #36864
	ldr	r0, [r3, #2684]
	ldr	r4, [r3, #2680]
	ldr	r5, [r1, #3968]
	ldr	lr, [r1, #3964]
	cmp	r4, lr
	cmpeq	r0, r5
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
.L1692:
	ldrb	r1, [r3, #1011]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1693
	ldr	r1, [r3, #2700]
	ldrb	r0, [r3, #2660]	@ zero_extendqisi2
	ldrb	r6, [r3, #1009]	@ zero_extendqisi2
	ldr	r5, [r3, #2640]
	ldrb	r4, [r3, #2661]	@ zero_extendqisi2
	cmp	r1, r5
	cmpeq	r0, r6
	ldrb	lr, [r3, #1010]	@ zero_extendqisi2
	movne	r1, #1
	moveq	r1, #0
	cmp	r4, lr
	moveq	r3, r1
	orrne	r3, r1, #1
	orr	r2, r2, r3
.L1693:
	add	r3, ip, #40960
	ldr	r1, .L1702
	mov	r0, #19
	ldr	r4, [r3, #1056]
	cmp	r4, #0
	movne	r4, r2
	orreq	r4, r2, #1
	mov	r2, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1691:
	cmp	r1, #1
	bne	.L1692
	add	r1, ip, #36864
	ldr	r0, [r3, #2692]
	ldr	r4, [r3, #2688]
	ldr	r5, [r1, #4004]
	ldr	lr, [r1, #4000]
	cmp	r4, lr
	cmpeq	r0, r5
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
	b	.L1692
.L1701:
	ldr	r0, [r3, #2676]
	ldr	r1, [r3, #1044]
	cmp	r0, r1
	orrne	r2, r2, #1
	b	.L1690
.L1703:
	.align	2
.L1702:
	.word	.LC51
	UNWIND(.fnend)
	.size	MVC_IsNewPic, .-MVC_IsNewPic
	.align	2
	.global	mvc_ref_pic_list_reordering
	.type	mvc_ref_pic_list_reordering, %function
mvc_ref_pic_list_reordering:
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
	add	r5, r0, #11075584
	ldr	r2, [r0, #240]
	add	r8, r5, #40960
	add	r5, r5, #36864
	mov	r3, #0
	mov	r6, #1
	ldrb	r1, [r8, #1011]	@ zero_extendqisi2
	mov	r4, r0
	strb	r3, [r5, #4044]
	cmp	r1, r3
	strb	r3, [r5, #4045]
	ldrb	r3, [r8, #1001]	@ zero_extendqisi2
	movne	r7, #5
	moveq	r7, #3
	cmp	r3, #0
	ldr	r3, [r2, #2896]
	addne	r3, r3, #5
	addeq	r3, r3, #4
	mov	r6, r6, asl r3
	ldrb	r3, [r8, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L1759
.L1758:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
.L1725:
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1759:
	ldr	r1, .L1766
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #4044]
	cmp	r0, #0
	beq	.L1715
	ldr	r3, [r8, #1060]
	cmn	r3, #2
	beq	.L1741
	movw	r10, #40908
	mov	r9, #0
	movt	r10, 169
	add	r10, r4, r10
	b	.L1722
.L1716:
	cmp	r0, #2
	beq	.L1760
	sub	r0, r0, #4
	cmp	r0, #1
	bls	.L1761
.L1719:
	ldr	r3, [r8, #1060]
	add	r9, r9, #1
	add	r3, r3, #2
	cmp	r3, r9
	bls	.L1762
.L1722:
	ldr	r1, .L1766+4
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r7, r0
	str	r0, [r10, #4]!
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bcc	.L1712
	cmp	r3, #0
	bne	.L1712
	cmp	r0, #3
	beq	.L1715
	cmp	r0, #1
	bhi	.L1716
	ldr	r1, .L1766+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, r0
	str	r0, [r10, #264]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bls	.L1718
	cmp	r3, #0
	beq	.L1719
.L1718:
	mov	r2, r0
	ldr	r1, .L1766+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1715:
	ldrb	r3, [r8, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1758
	ldr	r1, .L1766+16
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #4045]
	cmp	r0, #0
	beq	.L1758
	ldr	r3, [r8, #1064]
	cmn	r3, #2
	beq	.L1758
	movw	r9, #41040
	mov	r5, #0
	movt	r9, 169
	add	r9, r4, r9
	b	.L1738
.L1737:
	add	r5, r5, #1
	cmp	r5, r3
	bcs	.L1758
.L1738:
	ldr	r1, .L1766+20
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r7, r0
	str	r0, [r9, #4]!
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bcc	.L1729
	cmp	r3, #0
	bne	.L1729
	cmp	r0, #3
	beq	.L1725
	cmp	r0, #1
	bls	.L1763
	cmp	r0, #2
	beq	.L1764
	sub	r0, r0, #4
	cmp	r0, #1
	bls	.L1765
	ldr	r3, [r8, #1064]
	add	r3, r3, #2
	cmp	r3, r5
	bne	.L1737
.L1739:
	ldr	r1, .L1766+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1763:
	ldr	r1, .L1766+28
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, r0
	str	r0, [r9, #264]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bls	.L1733
	cmp	r3, #0
	bne	.L1733
.L1734:
	ldr	r2, [r8, #1064]
	add	r3, r2, #2
	cmp	r3, r5
	bne	.L1737
	movw	r1, #26646
	movt	r1, 42
	add	r1, r2, r1
	add	r1, r4, r1, lsl #2
	ldr	r2, [r1, #4]
	cmp	r2, #3
	beq	.L1737
	b	.L1739
.L1760:
	ldr	r1, .L1766+32
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r10, #528]
	b	.L1719
.L1764:
	ldr	r1, .L1766+36
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #524]
	b	.L1734
.L1761:
	ldr	r1, .L1766+40
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r10, #784]
	b	.L1719
.L1765:
	ldr	r1, .L1766+44
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #784]
	b	.L1734
.L1712:
	mov	r2, r0
	ldr	r1, .L1766+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1729:
	mov	r2, r0
	ldr	r1, .L1766+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1762:
	cmp	r9, r3
	bne	.L1715
.L1710:
	movw	r2, #26610
	movt	r2, 42
	add	r2, r3, r2
	add	r2, r4, r2, lsl #2
	ldr	r3, [r2, #8]
	cmp	r3, #3
	beq	.L1715
	ldr	r1, .L1766+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1733:
	mov	r2, r0
	ldr	r1, .L1766+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1741:
	mov	r3, #0
	b	.L1710
.L1767:
	.align	2
.L1766:
	.word	.LC52
	.word	.LC53
	.word	.LC55
	.word	.LC56
	.word	.LC60
	.word	.LC61
	.word	.LC67
	.word	.LC63
	.word	.LC57
	.word	.LC65
	.word	.LC58
	.word	.LC66
	.word	.LC54
	.word	.LC62
	.word	.LC59
	.word	.LC64
	UNWIND(.fnend)
	.size	mvc_ref_pic_list_reordering, .-mvc_ref_pic_list_reordering
	.align	2
	.global	mvc_pred_weight_table
	.type	mvc_pred_weight_table, %function
mvc_pred_weight_table:
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
	ldr	r3, [r0, #244]
	ldr	r1, .L1798
	add	r6, r0, #11075584
	ldr	r7, [r0, #240]
	mov	r5, r0
	str	r3, [fp, #-48]
	bl	mvc_ue_v
	add	r6, r6, #40960
	mov	r4, #1
	str	r0, [r6, #1068]
	mov	r8, r4, asl r0
	ldr	r9, [r7, #748]
	cmp	r9, #0
	bne	.L1792
.L1769:
	cmp	r0, #7
	bhi	.L1772
	ldr	r3, [r6, #1072]
	cmp	r3, #7
	bhi	.L1772
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
	cmp	r2, #0
	movweq	r10, #42164
	moveq	r4, r2
	movteq	r10, 169
	addeq	r10, r5, r10
	beq	.L1780
	b	.L1771
.L1776:
	ldr	r2, [r7, #748]
	cmp	r2, #0
	bne	.L1793
.L1777:
	str	r2, [r10]
	str	r2, [r10, #768]
	str	r2, [r10, #128]
	str	r2, [r10, #896]
.L1779:
	ldr	r2, [r6, #1060]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r2, r4
	bcc	.L1794
.L1780:
	ldr	r1, .L1798+4
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1776
	ldr	r1, .L1798+8
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+12
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r10, #640]
	ldr	r2, [r7, #748]
	cmp	r2, #0
	beq	.L1777
.L1793:
	ldr	r1, .L1798+16
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L1795
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1779
.L1794:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #44]
	cmp	r3, #1
	beq	.L1781
.L1782:
	ldrb	r0, [r5, #10]	@ zero_extendqisi2
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
.L1774:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1795:
	ldr	r1, .L1798+20
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+24
	str	r0, [r10]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+20
	str	r0, [r10, #768]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+24
	str	r0, [r10, #128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r10, #896]
	b	.L1779
.L1792:
	ldr	r1, .L1798+28
	mov	r0, r5
	bl	mvc_ue_v
	mov	r3, r0
	ldr	r0, [r6, #1068]
	mov	r9, r4, asl r3
	str	r3, [r6, #1072]
	b	.L1769
.L1781:
	ldrb	r3, [r6, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1782
	movw	r4, #42548
	mov	r10, #0
	movt	r4, 169
	add	r4, r5, r4
	b	.L1788
.L1784:
	ldr	r3, [r7, #748]
	cmp	r3, #0
	bne	.L1796
.L1785:
	str	r3, [r4]
	str	r3, [r4, #768]
	str	r3, [r4, #128]
	str	r3, [r4, #896]
.L1787:
	ldr	r3, [r6, #1064]
	add	r10, r10, #1
	add	r4, r4, #4
	cmp	r3, r10
	bcc	.L1782
.L1788:
	ldr	r1, .L1798+32
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	streq	r8, [r4, #-128]
	streq	r0, [r4, #640]
	beq	.L1784
	ldr	r1, .L1798+36
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+40
	str	r0, [r4, #-128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r4, #640]
	ldr	r3, [r7, #748]
	cmp	r3, #0
	beq	.L1785
.L1796:
	ldr	r1, .L1798+44
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L1797
	str	r9, [r4]
	str	r0, [r4, #768]
	str	r9, [r4, #128]
	str	r0, [r4, #896]
	b	.L1787
.L1772:
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
.L1771:
	ldr	r1, .L1798+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1774
.L1797:
	ldr	r1, .L1798+52
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+56
	str	r0, [r4]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+52
	str	r0, [r4, #768]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+56
	str	r0, [r4, #128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r4, #896]
	b	.L1787
.L1799:
	.align	2
.L1798:
	.word	.LC68
	.word	.LC71
	.word	.LC72
	.word	.LC73
	.word	.LC74
	.word	.LC75
	.word	.LC76
	.word	.LC69
	.word	.LC77
	.word	.LC78
	.word	.LC79
	.word	.LC80
	.word	.LC70
	.word	.LC81
	.word	.LC82
	UNWIND(.fnend)
	.size	mvc_pred_weight_table, .-mvc_pred_weight_table
	.align	2
	.global	MVC_DecMMCO
	.type	MVC_DecMMCO, %function
MVC_DecMMCO:
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
	cmp	r1, #0
	mov	r4, r0
	moveq	r7, #100
	beq	.L1812
	mov	r9, #0
	b	.L1803
.L1834:
	ldr	r1, .L1840
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r5, #2
	beq	.L1832
.L1814:
	cmp	r5, #6
	orreq	r6, r6, #1
	cmp	r6, #0
	bne	.L1817
	cmp	r5, #4
	beq	.L1833
.L1818:
	cmp	r5, #6
	bhi	.L1810
.L1815:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1810
	cmp	r5, #0
	beq	.L1820
.L1819:
	subs	r7, r7, #1
	beq	.L1821
.L1812:
	ldr	r1, .L1840+4
	mov	r0, r4
	bl	mvc_ue_v
	bic	r3, r0, #2
	sub	r6, r0, #3
	clz	r6, r6
	cmp	r3, #1
	mov	r5, r0
	mov	r6, r6, lsr #5
	beq	.L1834
	cmp	r5, #2
	bne	.L1814
.L1832:
	ldr	r1, .L1840+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, #0
	beq	.L1815
.L1817:
	ldr	r1, .L1840+12
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r5, #4
	bne	.L1818
.L1833:
	ldr	r1, .L1840+16
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1819
.L1810:
	ldr	r1, .L1840+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1839:
	ldr	r1, .L1840
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r3, [r8, #4]
	cmp	r3, #2
	str	r0, [r8, #8]
	beq	.L1835
.L1806:
	cmp	r3, #3
	cmpne	r3, #6
	beq	.L1836
.L1807:
	cmp	r3, #4
	beq	.L1837
.L1808:
	cmp	r3, #6
	bhi	.L1810
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1810
	cmp	r3, #0
	add	r9, r9, #1
	beq	.L1820
	cmp	r9, #100
	beq	.L1838
.L1803:
	ldr	r1, .L1840+4
	mov	r0, r4
	ldrb	r5, [r4, #3]	@ zero_extendqisi2
	bl	mvc_ue_v
	mov	r6, r9, asl #2
	mov	r7, r9, asl #4
	add	r1, r6, r7
	movw	r2, #2004
	movw	r8, #43664
	mla	r2, r2, r5, r1
	movt	r8, 169
	add	r2, r4, r2
	add	r8, r2, r8
	mov	r3, r0
	bic	r0, r0, #2
	cmp	r0, #1
	str	r3, [r8, #4]
	beq	.L1839
	cmp	r3, #2
	bne	.L1806
.L1835:
	ldr	r1, .L1840+8
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r6, r7
	movw	r2, #2004
	movw	r1, #43672
	mla	r3, r2, r5, r3
	movw	r2, #43664
	movt	r1, 169
	movt	r2, 169
	add	r3, r4, r3
	add	r1, r3, r1
	add	r2, r3, r2
	str	r0, [r1, #4]
	ldr	r3, [r2, #4]
	cmp	r3, #3
	cmpne	r3, #6
	bne	.L1807
.L1836:
	ldr	r1, .L1840+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r6, r7
	movw	r2, #2004
	movw	r1, #43672
	mla	r3, r2, r5, r3
	movw	r2, #43664
	movt	r1, 169
	movt	r2, 169
	add	r3, r4, r3
	add	r1, r3, r1
	add	r2, r3, r2
	str	r0, [r1, #8]
	ldr	r3, [r2, #4]
	cmp	r3, #4
	bne	.L1808
.L1837:
	ldr	r1, .L1840+16
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, r7
	movw	r7, #2004
	movw	r2, #43680
	mla	r5, r7, r5, r6
	movw	r3, #43664
	movt	r2, 169
	movt	r3, 169
	add	r5, r4, r5
	add	r2, r5, r2
	add	r3, r5, r3
	str	r0, [r2, #4]
	ldr	r3, [r3, #4]
	b	.L1808
.L1820:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1838:
	ldr	r1, .L1840+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1821:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1841:
	.align	2
.L1840:
	.word	.LC85
	.word	.LC84
	.word	.LC86
	.word	.LC87
	.word	.LC88
	.word	.LC89
	.word	.LC83
	UNWIND(.fnend)
	.size	MVC_DecMMCO, .-MVC_DecMMCO
	.align	2
	.global	mvc_dec_ref_pic_marking
	.type	mvc_dec_ref_pic_marking, %function
mvc_dec_ref_pic_marking:
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
	add	r3, r0, #11075584
	add	r3, r3, #40960
	mov	r4, r0
	ldrb	r7, [r0, #3]	@ zero_extendqisi2
	ldrb	r8, [r3, #1005]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L1843
	clz	r2, r7
	mov	r2, r2, lsr #5
	strb	r2, [r0, #3]
	mov	r6, r2
.L1852:
	ldrb	r3, [r3, #1003]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1863
	ldr	r1, .L1867
	mov	r0, r4
	bl	mvc_u_1
	movw	r2, #2004
	mla	r6, r2, r6, r4
	movw	r5, #43664
	movt	r5, 169
	ldr	r1, .L1867+4
	add	r5, r6, r5
	mov	r3, r0
	uxtb	r2, r0
	strb	r3, [r5, #3]
	mov	r0, #16
	bl	dprint_vfmw
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1846
.L1850:
	mov	r8, #0
.L1860:
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1843:
	movw	r2, #2004
	movw	r1, #43664
	mla	r2, r2, r7, r0
	movt	r1, 169
	movw	r5, #43664
	mov	r6, r7
	movt	r5, 169
	add	r5, r2, r5
	ldrb	r2, [r2, r1]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1852
	ldrb	r3, [r3, #1003]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1864
	ldr	r1, .L1867
	ldrb	r6, [r5, #3]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r6, r0
	bne	.L1865
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1850
	mov	r1, r8
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	MVC_DecMMCO
.L1846:
	mov	r1, #1
	mov	r0, r4
	bl	MVC_DecMMCO
	cmn	r0, #1
	mov	r8, r0
	movne	r3, #1
	movne	r8, #0
	strneb	r3, [r5]
	bne	.L1860
	mov	r3, #0
	ldr	r1, .L1867+8
	strb	r3, [r5]
	mov	r0, #1
	strb	r7, [r4, #3]
	bl	dprint_vfmw
	b	.L1860
.L1864:
	ldr	r1, .L1867+12
	ldrb	r6, [r5, #1]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r6, r0
	bne	.L1866
	mov	r0, r4
	ldr	r1, .L1867+16
	ldrb	r4, [r5, #2]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r4, r0
	beq	.L1850
	ldr	r1, .L1867+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1860
.L1863:
	ldr	r1, .L1867+12
	mov	r0, r4
	bl	mvc_u_1
	movw	r2, #2004
	mla	r2, r2, r6, r4
	movw	r5, #43664
	movt	r5, 169
	ldr	r1, .L1867+16
	mov	r8, #0
	add	r5, r2, r5
	strb	r0, [r5, #1]
	mov	r0, r4
	bl	mvc_u_1
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	ldr	r1, .L1867+24
	strb	r0, [r5, #2]
	mov	r0, #16
	bl	dprint_vfmw
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1865:
	ldr	r1, .L1867+28
	mov	r0, #1
	bl	dprint_vfmw
	ldr	ip, [r4, #68]
	ldr	r3, [r4, #100]
	mov	r0, #1
	ldr	r2, [r4, #88]
	mvn	r8, #0
	ldr	r1, .L1867+32
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L1860
.L1866:
	ldr	r1, .L1867+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1860
.L1868:
	.align	2
.L1867:
	.word	.LC93
	.word	.LC94
	.word	.LC95
	.word	.LC90
	.word	.LC91
	.word	.LC97
	.word	.LC92
	.word	.LC98
	.word	.LC99
	.word	.LC96
	UNWIND(.fnend)
	.size	mvc_dec_ref_pic_marking, .-mvc_dec_ref_pic_marking
	.align	2
	.global	MVC_ProcessSliceHeaderFirstPart
	.type	MVC_ProcessSliceHeaderFirstPart, %function
MVC_ProcessSliceHeaderFirstPart:
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
	ldr	r1, .L1934
	mov	r4, r0
	bl	mvc_ue_v
	ldr	r3, [r4, #40]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r6, r0
	bhi	.L1870
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1870
	mov	r1, r0
	mov	r0, r4
	bl	MVC_PPSSPSCheckTmpId
	cmp	r0, #0
	bne	.L1925
	add	r7, r4, #11075584
	mov	r0, r4
	add	r5, r7, #40960
	str	r6, [r5, #1016]
	bl	MVC_PPSSPSCheck
	subs	r8, r0, #0
	bne	.L1926
	ldr	r6, [r4, #240]
	mov	r0, r4
	ldr	r2, .L1934+4
	ldr	r9, [r4, #244]
	ldr	r1, [r6, #2896]
	add	r1, r1, #4
	bl	mvc_u_v
	str	r0, [r5, #1028]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1923
	adds	r0, r0, #0
	ldrb	r3, [r5, #1003]	@ zero_extendqisi2
	movne	r0, #1
	cmp	r3, #5
	movne	r0, #0
	cmp	r0, #0
	bne	.L1927
.L1876:
	mov	r3, #0
	strb	r3, [r5, #1001]
	strb	r3, [r5, #1002]
	ldrb	r3, [r6, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1928
	ldr	r2, [r6, #3948]
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	mla	r3, r1, r3, r3
.L1899:
	ldrb	r2, [r6, #21]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1882
	strb	r2, [r5, #1004]
.L1884:
	ldr	r2, [r5, #1056]
	cmp	r2, r3
	bcs	.L1929
.L1885:
	ldrb	r3, [r5, #1003]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1930
.L1887:
	add	r10, r7, #36864
	mov	r3, #0
	str	r3, [r10, #3964]
	str	r3, [r10, #3968]
	ldr	r3, [r6, #2900]
	cmp	r3, #0
	beq	.L1931
.L1891:
	mov	r3, #0
	str	r3, [r10, #4000]
	str	r3, [r10, #4004]
	ldr	r3, [r6, #2900]
	cmp	r3, #1
	beq	.L1932
.L1894:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1923
	ldrb	r3, [r9, #17]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1897
	ldr	r1, .L1934+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L1898
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1897
.L1898:
	ldr	r1, .L1934+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L1934+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1923
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L1923:
	mvn	r8, #0
.L1909:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1928:
	ldr	r1, .L1934+20
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #1001]
	cmp	r0, #0
	bne	.L1933
	ldr	r2, [r6, #3948]
	ldrb	ip, [r6, #20]	@ zero_extendqisi2
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	cmp	ip, #0
	mla	r3, r1, r3, r3
	bne	.L1901
	mov	r3, r3, asl #1
	b	.L1899
.L1882:
	mov	r2, #1
	strb	r2, [r5, #1004]
.L1902:
	ldr	r2, [r5, #1056]
	cmp	r2, r3, lsr #1
	bcc	.L1885
	ldr	r1, .L1934+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1909
.L1933:
	ldr	r1, .L1934+28
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #1002]
	ldr	r2, [r6, #3948]
	ldrb	r0, [r6, #20]	@ zero_extendqisi2
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	cmp	r0, #0
	ldrb	r0, [r5, #1001]	@ zero_extendqisi2
	mla	r3, r1, r3, r3
	moveq	r2, #2
	beq	.L1880
.L1901:
	mov	r2, #1
.L1880:
	cmp	r0, #0
	mul	r3, r2, r3
	beq	.L1899
	cmp	r0, #1
	mov	r2, #0
	strb	r2, [r5, #1004]
	bne	.L1884
	b	.L1902
.L1897:
	mov	r0, r4
	add	r7, r7, #40960
	bl	MVC_IsNewPic
	uxtb	r0, r0
	strb	r0, [r7, #1005]
	cmp	r0, #0
	bne	.L1909
	ldr	r2, [r7, #1056]
	ldr	r3, [r7, #2696]
	cmp	r2, r3
	bhi	.L1909
	ldr	r1, .L1934+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1909
.L1931:
	ldr	r1, [r6, #2904]
	mov	r0, r4
	ldr	r2, .L1934+36
	add	r1, r1, #4
	bl	mvc_u_v
	str	r0, [r10, #3964]
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1891
	ldrb	r3, [r5, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1891
	ldr	r1, .L1934+40
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #3968]
	b	.L1891
.L1932:
	ldrb	r3, [r6, #18]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1894
	ldr	r1, .L1934+44
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #4000]
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1894
	ldrb	r3, [r5, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1894
	ldr	r1, .L1934+48
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #4004]
	b	.L1894
.L1870:
	mov	r2, r6
	ldr	r1, .L1934+52
	mvn	r8, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1930:
	ldr	r1, .L1934+56
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #65536
	str	r0, [r5, #1044]
	bcs	.L1888
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1887
.L1888:
	ldr	r1, .L1934+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1887
.L1925:
	ldr	r1, .L1934+64
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1909
.L1926:
	ldr	r1, .L1934+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1909
.L1927:
	ldr	r3, [r5, #2640]
	cmp	r3, #0
	bgt	.L1876
	ldr	r1, .L1934+72
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1876
.L1929:
	ldr	r1, .L1934+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1909
.L1935:
	.align	2
.L1934:
	.word	.LC100
	.word	.LC104
	.word	.LC116
	.word	.LC117
	.word	g_event_report
	.word	.LC106
	.word	.LC108
	.word	.LC107
	.word	.LC118
	.word	.LC112
	.word	.LC113
	.word	.LC114
	.word	.LC115
	.word	.LC101
	.word	.LC110
	.word	.LC111
	.word	.LC102
	.word	.LC103
	.word	.LC105
	.word	.LC109
	UNWIND(.fnend)
	.size	MVC_ProcessSliceHeaderFirstPart, .-MVC_ProcessSliceHeaderFirstPart
	.align	2
	.global	MVC_ProcessSliceHeaderSecondPart
	.type	MVC_ProcessSliceHeaderSecondPart, %function
MVC_ProcessSliceHeaderSecondPart:
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
	add	r4, r0, #11075584
	ldr	r3, [r0, #40]
	add	r4, r4, #40960
	mov	r5, r0
	sub	r3, r3, #1
	ldr	r6, [r0, #244]
	ldr	r2, [r4, #1016]
	cmp	r3, r2
	bcc	.L1937
	ldrb	r7, [r0, #10]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L1937
	bl	MVC_PPSSPSCheck
	cmp	r0, #0
	bne	.L2019
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L2020
.L1941:
	ldr	r2, [r6, #36]
	cmp	r3, #1
	str	r2, [r4, #1060]
	ldr	r2, [r6, #40]
	str	r2, [r4, #1064]
	bls	.L2021
	cmp	r3, #2
	beq	.L1968
.L1952:
	mov	r3, #32
	str	r3, [r4, #1064]
.L1953:
	mov	r0, r5
	bl	mvc_ref_pic_list_reordering
	subs	r8, r0, #0
	bne	.L2022
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1955
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r7, #1
	moveq	r2, r7
	beq	.L1956
.L1955:
	ldr	r3, [r6, #44]
	cmp	r3, #1
	beq	.L2023
.L1972:
	mov	r2, #0
.L1956:
	strb	r7, [r4, #1007]
	mov	r0, #19
	ldr	r1, .L2029
	bl	dprint_vfmw
	ldrb	r2, [r4, #1007]	@ zero_extendqisi2
	mov	r3, #0
	str	r3, [r4, #1068]
	cmp	r2, r3
	str	r3, [r4, #1072]
	bne	.L2024
.L1957:
	ldrb	r3, [r4, #1008]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2025
.L1958:
	mov	r3, #0
	str	r3, [r4, #2612]
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1959
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L2026
.L1959:
	ldr	r1, .L2029+4
	mov	r0, r5
	bl	mvc_se_v
	mov	r3, #0
	str	r3, [r4, #2620]
	str	r3, [r4, #2624]
	str	r3, [r4, #2628]
	str	r0, [r4, #2616]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2027
.L1988:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1968:
	mov	r3, #32
	str	r3, [r4, #1060]
	b	.L1952
.L2027:
	ldr	r1, .L2029+8
	mov	r0, r5
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r4, #2620]
	bhi	.L1962
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1962
	cmp	r0, #1
	beq	.L1988
	ldr	r1, .L2029+12
	mov	r0, r5
	bl	mvc_se_v
	add	r3, r0, #6
	str	r0, [r4, #2624]
	cmp	r3, #12
	bhi	.L1965
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1965
	ldr	r1, .L2029+16
	mov	r0, r5
	bl	mvc_se_v
	add	r3, r0, #6
	str	r0, [r4, #2628]
	cmp	r3, #12
	bhi	.L1967
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1988
.L1967:
	ldr	r1, .L2029+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L2024:
	mov	r0, r5
	bl	mvc_pred_weight_table
	cmp	r0, #0
	beq	.L1957
	ldr	r1, .L2029+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L2025:
	mov	r0, r5
	bl	mvc_dec_ref_pic_marking
	cmp	r0, #0
	beq	.L1958
	ldr	r1, .L2029+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L2023:
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r7, r3
	moveq	r2, r7
	bne	.L1972
	b	.L1956
.L2020:
	ldr	r1, .L2029+32
	mov	r0, r5
	bl	mvc_u_1
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	strb	r0, [r4, #1006]
	b	.L1941
.L2021:
	ldr	r1, .L2029+36
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L1943
	ldr	r3, [r4, #1060]
.L1944:
	ldrb	r2, [r4, #1001]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1945
	cmp	r3, #15
	bhi	.L1946
	ldr	r3, [r4, #1064]
	cmp	r3, #15
	bhi	.L2028
.L1947:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1946
.L1948:
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1968
	cmp	r3, #1
	bne	.L1952
	b	.L1953
.L1937:
	ldr	r1, .L2029+40
	mvn	r8, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2026:
	ldr	r1, .L2029+44
	mov	r0, r5
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r4, #2612]
	bhi	.L1960
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1959
.L1960:
	ldr	r1, .L2029+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L1945:
	cmp	r3, #31
	bhi	.L1949
	ldr	r3, [r4, #1064]
	cmp	r3, #31
	bls	.L1950
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1950
.L1949:
	ldr	r1, .L2029+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L1950:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1948
	b	.L1949
.L2028:
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1947
.L1946:
	ldr	r1, .L2029+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L1943:
	ldr	r1, .L2029+60
	mov	r0, r5
	bl	mvc_ue_v
	ldrb	r2, [r4, #1000]	@ zero_extendqisi2
	cmp	r2, #1
	mov	r3, r0
	str	r0, [r4, #1060]
	bne	.L1944
	ldr	r1, .L2029+64
	mov	r0, r5
	bl	mvc_ue_v
	ldr	r3, [r4, #1060]
	str	r0, [r4, #1064]
	b	.L1944
.L1965:
	ldr	r1, .L2029+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L1962:
	ldr	r1, .L2029+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L2019:
	ldr	r1, .L2029+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L2022:
	ldr	r1, .L2029+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1988
.L2030:
	.align	2
.L2029:
	.word	.LC126
	.word	.LC131
	.word	.LC132
	.word	.LC134
	.word	.LC136
	.word	.LC137
	.word	.LC127
	.word	.LC128
	.word	.LC119
	.word	.LC120
	.word	.LC101
	.word	.LC129
	.word	.LC130
	.word	.LC124
	.word	.LC123
	.word	.LC121
	.word	.LC122
	.word	.LC135
	.word	.LC133
	.word	.LC102
	.word	.LC125
	UNWIND(.fnend)
	.size	MVC_ProcessSliceHeaderSecondPart, .-MVC_ProcessSliceHeaderSecondPart
	.align	2
	.global	MVC_ExitSlice
	.type	MVC_ExitSlice, %function
MVC_ExitSlice:
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
	add	r2, r0, #11075584
	ldr	ip, [r0, #236]
	add	r3, r2, #40960
	ldrb	r1, [r3, #1001]	@ zero_extendqisi2
	ldr	lr, [r3, #1016]
	cmp	r1, #0
	ldr	r5, [r3, #1056]
	str	lr, [r3, #2668]
	ldr	r4, [ip, #744]
	strb	r1, [r3, #2656]
	ldrneb	r1, [r3, #1002]	@ zero_extendqisi2
	ldr	lr, [r3, #1028]
	str	r5, [r3, #2696]
	strneb	r1, [r3, #2657]
	ldrb	r1, [r3, #1003]	@ zero_extendqisi2
	str	lr, [r3, #2672]
	cmp	r1, #5
	ldrb	lr, [r3, #1008]	@ zero_extendqisi2
	strb	r1, [r3, #2658]
	ldreq	r1, [r3, #1044]
	str	r4, [r3, #2664]
	strb	lr, [r3, #2659]
	streq	r1, [r3, #2676]
	ldr	r1, [ip, #2900]
	cmp	r1, #0
	bne	.L2034
	add	r1, r2, #36864
	ldr	lr, [r1, #3964]
	str	lr, [r3, #2680]
	ldr	r1, [r1, #3968]
	str	r1, [r3, #2684]
	ldr	r1, [ip, #2900]
.L2034:
	cmp	r1, #1
	addeq	r2, r2, #36864
	ldreq	r1, [r2, #4000]
	streq	r1, [r3, #2688]
	ldreq	r2, [r2, #4004]
	streq	r2, [r3, #2692]
	ldrb	r2, [r3, #1011]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L2036
	ldr	ip, [r3, #2640]
	ldrb	r1, [r3, #1009]	@ zero_extendqisi2
	ldrb	r2, [r3, #1010]	@ zero_extendqisi2
	str	ip, [r3, #2700]
	strb	r1, [r3, #2660]
	strb	r2, [r3, #2661]
.L2036:
	ldr	r3, [r0, #80]
	add	r3, r3, #1
	str	r3, [r0, #80]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_ExitSlice, .-MVC_ExitSlice
	.align	2
	.global	MVC_PicTypeStatistic
	.type	MVC_PicTypeStatistic, %function
MVC_PicTypeStatistic:
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
	add	r3, r0, #11075584
	add	r3, r3, #40960
	ldrb	r2, [r3, #1000]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L2042
	bcc	.L2043
	cmp	r2, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r3, #1003]	@ zero_extendqisi2
	add	r0, r0, #11141120
	cmp	r3, #5
	beq	.L2052
.L2045:
	add	r0, r0, #12288
	mov	r3, #0
	strb	r3, [r0, #2789]
	ldmfd	sp, {fp, sp, pc}
.L2043:
	add	r0, r0, #11141120
	add	r3, r0, #12288
	ldrb	r2, [r3, #2788]	@ zero_extendqisi2
	cmp	r2, #2
	movne	r2, #1
	strneb	r2, [r3, #2788]
	b	.L2045
.L2042:
	add	r0, r0, #11141120
	mov	r2, #2
	add	r0, r0, #12288
	mov	r3, #0
	strb	r2, [r0, #2788]
	strb	r3, [r0, #2789]
	ldmfd	sp, {fp, sp, pc}
.L2052:
	add	r0, r0, #12288
	mov	r3, #1
	strb	r3, [r0, #2789]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_PicTypeStatistic, .-MVC_PicTypeStatistic
	.align	2
	.global	MVC_CalcStreamBits
	.type	MVC_CalcStreamBits, %function
MVC_CalcStreamBits:
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
	add	r3, r0, #11075584
	add	r3, r3, #36864
	mov	r4, r0
	ldrb	r3, [r3, #1720]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2054
	ldr	r2, [r0, #232]
	ldr	r3, [r2, #64]
	add	r3, r3, #7
	bic	r3, r3, #7
	str	r3, [r2, #64]
	ldr	r2, [r0, #232]
	ldr	r3, [r2, #72]
	sub	r3, r3, #1
	str	r3, [r2, #72]
.L2054:
	ldr	r3, [r4, #232]
	ldrb	r6, [r3]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L2065
	mov	r1, #0
	mov	r0, r3
	mov	r5, r1
.L2056:
	add	r1, r1, #1
	ldr	ip, [r0, #12]
	cmp	r1, r6
	add	r0, r0, #28
	add	r5, r5, ip
	bne	.L2056
.L2055:
	mov	r7, r6, asl #5
	ldr	r0, [r3, #72]
	sub	r7, r7, r6, asl #2
	ldr	r2, [r3, #64]
	add	r1, r3, r7
	mov	r3, r5
	str	r0, [sp, #4]
	mov	r0, #7
	ldr	ip, [r1, #12]
	ldr	r1, .L2076
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #232]
	mov	r2, r6
	mov	r0, #7
	ldr	r1, .L2076+4
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	ip, [r3, #64]
	mov	lr, r6, asl #5
	and	ip, ip, r0
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	str	ip, [r3, #24]
	ldr	r3, [r4, #232]
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	r8, [r3, #64]
	mov	lr, r6, asl #5
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	ldr	ip, [r3, #16]
	add	ip, ip, r8, lsr #3
	rsb	ip, r5, ip
	str	ip, [r3, #28]
	ldr	ip, [r4, #232]
	ldrb	r6, [ip]	@ zero_extendqisi2
	ldr	lr, [ip, #64]
	mov	r3, r6, asl #5
	sub	r3, r3, r6, asl #2
	add	ip, ip, r3
	ldr	r3, [ip, #12]
	add	r3, r5, r3
	rsb	r3, lr, r3, lsl #3
	str	r3, [ip, #20]
	ldr	ip, [r4, #232]
	ldrb	lr, [ip]	@ zero_extendqisi2
	mov	r3, lr, asl #5
	sub	r3, r3, lr, asl #2
	add	r3, ip, r3
	ldr	ip, [r3, #28]
	ldr	lr, [r3, #24]
	and	ip, ip, #3
	add	ip, lr, ip, lsl #3
	str	ip, [r3, #24]
	ldr	ip, [r4, #232]
	ldrb	lr, [ip]	@ zero_extendqisi2
	mov	r3, lr, asl #5
	sub	r3, r3, lr, asl #2
	add	r3, ip, r3
	ldr	ip, [r3, #28]
	bic	ip, ip, #3
	str	ip, [r3, #28]
	ldr	ip, [r4, #232]
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
	ldr	ip, [r4, #232]
	ldr	r2, [ip, #68]
	sub	r3, r2, #1
	cmp	r3, #1
	bhi	.L2075
	ldrb	r6, [ip]	@ zero_extendqisi2
	add	r1, r6, #1
	cmp	r2, r1
	movhi	r2, #1
	movls	r2, #0
	cmp	r1, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L2058
	add	r6, r6, #2
	mov	r5, #0
.L2059:
	sub	r3, r6, #1
	ldr	r1, .L2076+8
	mov	r0, #7
	mov	lr, r3, asl #5
	mov	r2, r3
	sub	r3, lr, r3, asl #2
	add	ip, ip, r3
	str	r5, [ip, #24]
	ldr	ip, [r4, #232]
	add	ip, ip, r3
	ldr	lr, [ip, #16]
	str	lr, [ip, #28]
	ldr	ip, [r4, #232]
	add	ip, ip, r3
	ldr	lr, [ip, #12]
	mov	lr, lr, asl #3
	str	lr, [ip, #20]
	ldr	ip, [r4, #232]
	add	ip, ip, r3
	ldr	lr, [ip, #28]
	ldr	r7, [ip, #24]
	and	lr, lr, #3
	add	lr, r7, lr, lsl #3
	str	lr, [ip, #24]
	ldr	ip, [r4, #232]
	add	ip, ip, r3
	ldr	lr, [ip, #28]
	bic	lr, lr, #3
	str	lr, [ip, #28]
	ldr	ip, [r4, #232]
	add	ip, ip, r3
	ldr	r3, [ip, #24]
	ldr	ip, [ip, #20]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	ip, [r4, #232]
	ldr	r3, [ip, #68]
	cmp	r6, #1
	cmpls	r6, r3
	add	r6, r6, #1
	movcc	r2, #1
	movcs	r2, #0
	cmp	r2, #0
	bne	.L2059
	sub	r3, r3, #1
.L2058:
	mov	r2, r3, asl #5
	ldr	r1, [ip, #72]
	sub	r3, r2, r3, asl #2
	add	ip, ip, r3
	ldr	r3, [ip, #20]
	rsb	r3, r1, r3
	str	r3, [ip, #20]
	ldr	ip, [r4, #232]
	ldrb	r1, [ip]	@ zero_extendqisi2
	ldr	r3, [ip, #68]
	cmp	r1, r3
	bcs	.L2053
	sxth	r3, r1
	add	r4, r4, #11141120
	add	r4, r4, #12288
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, ip, r3
.L2063:
	ldr	r2, [r3, #28]
	add	r1, r1, #1
	ldr	r0, [r4, #3488]
	add	r3, r3, #28
	cmp	r2, r0
	strcc	r2, [r4, #3488]
	ldr	r2, [ip, #68]
	cmp	r2, r1
	bhi	.L2063
.L2053:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2075:
	ldr	r1, .L2076+12
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	dprint_vfmw
.L2065:
	mov	r5, r6
	b	.L2055
.L2077:
	.align	2
.L2076:
	.word	.LC138
	.word	.LC139
	.word	.LC141
	.word	.LC140
	UNWIND(.fnend)
	.size	MVC_CalcStreamBits, .-MVC_CalcStreamBits
	.align	2
	.global	MVC_Scaling_List
	.type	MVC_Scaling_List, %function
MVC_Scaling_List:
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
	ble	.L2078
	mov	r0, #8
	mov	r4, #0
	mov	r2, r7
	mov	r9, r6
	ldr	r8, .L2089
	mov	r10, #255
	mov	r7, r1
	mov	r6, r0
	mov	r5, r4
	b	.L2083
.L2082:
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
	beq	.L2078
.L2083:
	cmp	r2, #16
	ldreq	r3, .L2089+4
	ldrneb	r4, [r5, r8]	@ zero_extendqisi2
	ldreqb	r4, [r5, r3]	@ zero_extendqisi2
	cmp	r0, #0
	add	r5, r5, #1
	beq	.L2082
	ldr	r1, .L2089+8
	ldr	r0, [fp, #-52]
	str	r2, [fp, #-48]
	bl	mvc_se_v
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
	b	.L2082
.L2078:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2090:
	.align	2
.L2089:
	.word	.LANCHOR0+16
	.word	.LANCHOR0
	.word	.LC142
	UNWIND(.fnend)
	.size	MVC_Scaling_List, .-MVC_Scaling_List
	.align	2
	.global	MVC_ProcessPPS
	.type	MVC_ProcessPPS, %function
MVC_ProcessPPS:
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
	mov	r5, r1
	ldr	r1, .L2149
	mov	r4, r0
	bl	mvc_u_1
	strb	r0, [r5]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2140
	ldr	r1, .L2149+4
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #1]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2140
	ldr	r1, .L2149+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	str	r0, [r5, #32]
	bne	.L2095
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2096
.L2095:
	ldr	r1, .L2149+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2149+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2140
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L2093:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2103:
	ldr	r1, .L2149+20
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2149+24
	str	r0, [r5, #60]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #18]
	cmp	r0, #1
	beq	.L2104
.L2122:
	ldr	r1, .L2149+28
	mov	r0, r4
	bl	mvc_se_v
	add	r3, r0, #12
	str	r0, [r5, #56]
	cmp	r3, #24
	bhi	.L2105
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2123
.L2105:
	ldr	r1, .L2149+32
	mov	r0, #1
	bl	dprint_vfmw
.L2140:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2096:
	ldr	r1, .L2149+36
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #31
	str	r0, [r5, #36]
	bhi	.L2097
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2097
	ldr	r1, .L2149+40
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #31
	str	r0, [r5, #40]
	bhi	.L2099
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2099
	ldr	r1, .L2149+44
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #2]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2140
	ldr	r2, .L2149+48
	mov	r1, #2
	mov	r0, r4
	bl	mvc_u_v
	cmp	r0, #2
	str	r0, [r5, #44]
	bhi	.L2141
	ldr	r1, .L2149+52
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2149+56
	str	r0, [r5, #48]
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2149+60
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2149+64
	str	r0, [r5, #52]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2149+68
	strb	r0, [r5, #3]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2149+72
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #17]
	cmp	r0, #0
	bne	.L2142
.L2102:
	ldr	r3, [r5, #52]
	mov	r6, #0
	add	r0, r4, #548
	str	r6, [r5, #60]
	strb	r6, [r5, #18]
	str	r3, [r5, #56]
	bl	BsResidBits
	ldr	r3, [r4, #232]
	ldr	r3, [r3, #72]
	add	r3, r3, #3
	cmp	r0, r3
	bcs	.L2103
.L2123:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2142:
	ldr	r1, .L2149+76
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2102
.L2097:
	ldr	r1, .L2149+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2093
.L2099:
	ldr	r1, .L2149+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2093
.L2104:
	sub	r8, fp, #72
	add	r10, r5, #4
	add	r7, r5, #2016
	add	r9, r5, #64
	b	.L2113
.L2146:
	ldr	lr, [r5, #28]
	ldr	r3, [r4, #248]
	mla	r3, ip, lr, r3
	ldrb	ip, [r3, #27]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L2143
	ldr	r3, .L2149+88
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2149+92
	ldr	r3, [r3, #52]
	blx	r3
.L2108:
	add	r6, r6, #1
	add	r7, r7, #16
	cmp	r6, #6
	add	r9, r9, #64
	beq	.L2144
.L2113:
	ldr	r1, .L2149+96
	mov	r0, r4
	bl	mvc_u_1
	movw	ip, #3992
	clz	r1, r6
	mov	r2, #16
	cmp	r0, #1
	mov	r3, r0
	mov	r0, r7
	str	r3, [r8, #4]!
	strb	r3, [r10, #1]!
	beq	.L2145
	cmp	r6, #0
	cmpne	r6, #3
	mov	r1, r1, lsr #5
	beq	.L2146
	ldr	r3, .L2149+92
	mov	r2, #16
	sub	r1, r7, #16
	mov	r0, r7
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2108
.L2144:
	add	r8, r5, #2112
	add	r10, r5, #448
	sub	r7, fp, #76
	add	r9, r5, #11
	mov	r6, #0
.L2121:
	ldr	r2, [r5, #60]
	mov	r3, #0
	cmp	r2, #1
	strne	r3, [r7]
	strneb	r3, [r9]
	beq	.L2147
.L2116:
	ldr	r1, [r4, #248]
	movw	ip, #3992
	ldr	r3, [r5, #28]
	mov	r0, r8
	mov	r2, #64
	mla	r3, ip, r3, r1
	ldrb	r1, [r3, #27]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L2148
	ldr	r1, .L2149+100
	cmp	r6, #0
	ldr	r3, .L2149+92
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2117:
	add	r6, r6, #1
	add	r8, r8, #64
	cmp	r6, #2
	add	r10, r10, #256
	add	r7, r7, #4
	add	r9, r9, #1
	bne	.L2121
	b	.L2122
.L2145:
	mov	r3, r2
	str	r8, [sp]
	mov	r2, r7
	mov	r1, r9
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r8]
	cmp	r3, #1
	bne	.L2108
	ldr	r1, .L2149+88
	mov	r2, #16
	ldr	r3, .L2149+92
	cmp	r6, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r7
	blx	r3
	b	.L2108
.L2143:
	ldr	r2, .L2149+92
	add	r1, r6, #167
	mov	r0, r7
	add	r1, r3, r1, lsl #4
	ldr	ip, [r2, #52]
	mov	r2, #16
	blx	ip
	b	.L2108
.L2141:
	ldr	r1, .L2149+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2093
.L2147:
	ldr	r1, .L2149+108
	mov	r0, r4
	bl	mvc_u_1
	mov	r2, r8
	mov	r1, r10
	mov	r3, #64
	mov	ip, r0
	cmp	ip, #1
	mov	r0, r4
	str	ip, [r7]
	strb	ip, [r9]
	bne	.L2116
	str	r7, [sp]
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2117
	ldr	r1, .L2149+100
	mov	r2, #64
	ldr	r3, .L2149+92
	cmp	r6, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r8
	blx	r3
	b	.L2117
.L2148:
	add	r1, r3, r6, lsl #6
	ldr	r3, .L2149+92
	add	r1, r1, #2768
	ldr	ip, [r3, #52]
	blx	ip
	b	.L2117
.L2150:
	.align	2
.L2149:
	.word	.LC143
	.word	.LC144
	.word	.LC145
	.word	.LC146
	.word	g_event_report
	.word	.LC161
	.word	.LC162
	.word	.LC163
	.word	.LC166
	.word	.LC147
	.word	.LC149
	.word	.LC151
	.word	.LC152
	.word	.LC154
	.word	.LC155
	.word	.LC156
	.word	.LC157
	.word	.LC158
	.word	.LC159
	.word	.LC160
	.word	.LC148
	.word	.LC150
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC164
	.word	.LANCHOR1+32
	.word	.LC153
	.word	.LC165
	UNWIND(.fnend)
	.size	MVC_ProcessPPS, .-MVC_ProcessPPS
	.align	2
	.global	MVC_PPSEqual
	.type	MVC_PPSEqual, %function
MVC_PPSEqual:
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
	subs	r6, r0, #0
	beq	.L2178
	cmp	r1, #0
	beq	.L2179
	ldr	r2, [r1, #28]
	ldr	r0, [r6, #28]
	ldr	r3, [r6, #24]
	ldr	ip, [r1, #24]
	ldrb	lr, [r6]	@ zero_extendqisi2
	cmp	r0, r2
	cmpeq	r3, ip
	ldrb	r4, [r1]	@ zero_extendqisi2
	ldrb	ip, [r6, #1]	@ zero_extendqisi2
	ldrb	r5, [r1, #1]	@ zero_extendqisi2
	moveq	r2, #1
	movne	r2, #0
	ldr	r0, [r6, #36]
	cmp	lr, r4
	movne	r3, #0
	andeq	r3, r2, #1
	ldr	r4, [r1, #36]
	cmp	ip, r5
	movne	lr, #0
	andeq	lr, r3, #1
	ldr	r2, [r6, #40]
	ldr	r5, [r1, #40]
	cmp	r0, r4
	movne	ip, #0
	andeq	ip, lr, #1
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	ldrb	r4, [r1, #2]	@ zero_extendqisi2
	cmp	r2, r5
	movne	r0, #0
	andeq	r0, ip, #1
	ldr	lr, [r6, #44]
	ldr	r5, [r1, #44]
	cmp	r3, r4
	movne	r2, #0
	andeq	r2, r0, #1
	ldr	ip, [r6, #48]
	ldr	r4, [r1, #48]
	cmp	lr, r5
	movne	r3, #0
	andeq	r3, r2, #1
	ldr	r0, [r6, #52]
	ldr	r5, [r1, #52]
	cmp	ip, r4
	movne	lr, #0
	andeq	lr, r3, #1
	ldrb	r2, [r6, #3]	@ zero_extendqisi2
	ldrb	r4, [r1, #3]	@ zero_extendqisi2
	cmp	r0, r5
	movne	ip, #0
	andeq	ip, lr, #1
	ldrb	r3, [r6, #4]	@ zero_extendqisi2
	ldrb	lr, [r1, #4]	@ zero_extendqisi2
	cmp	r2, r4
	movne	r0, #0
	andeq	r0, ip, #1
	ldr	ip, [r6, #60]
	ldr	r4, [r1, #60]
	cmp	r3, lr
	movne	r2, #0
	andeq	r2, r0, #1
	ldrb	r0, [r6, #18]	@ zero_extendqisi2
	ldrb	lr, [r1, #18]	@ zero_extendqisi2
	cmp	ip, r4
	movne	r3, #0
	andeq	r3, r2, #1
	cmp	lr, r0
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r0, #0
	beq	.L2155
	mov	ip, ip, asl #1
	adds	r2, ip, #6
	str	r2, [fp, #-56]
	beq	.L2155
	add	r2, r6, #4
	sub	r9, r1, #1088
	str	r2, [fp, #-52]
	add	r2, r1, #4
	str	r2, [fp, #-48]
	add	r2, r1, #64
	sub	r9, r9, #4
	add	r7, r1, #1984
	add	r8, r6, #1984
	mov	r10, r6
	mov	r5, r6
	str	r6, [fp, #-60]
	mov	r0, #0
	mov	r6, r2
	str	r1, [fp, #-64]
.L2160:
	ldr	r1, [fp, #-52]
	ldrb	r2, [r1, #1]!	@ zero_extendqisi2
	str	r1, [fp, #-52]
	ldr	r1, [fp, #-48]
	ldrsb	ip, [r1, #1]!
	str	r1, [fp, #-48]
	sxtb	r1, r2
	cmp	ip, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, #0
	beq	.L2177
	ldr	r2, [r8]
	ldr	r1, [r7]
	cmp	r2, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r0, #5
	bhi	.L2157
	cmp	r2, #0
	add	r4, r10, #64
	bne	.L2156
	add	r10, r10, #128
	mov	r1, r6
	mov	r2, r4
.L2158:
	ldr	lr, [r2], #4
	ldr	ip, [r1], #4
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2158
.L2156:
	ldr	r2, [fp, #-56]
	add	r0, r0, #1
	add	r8, r8, #4
	add	r7, r7, #4
	cmp	r0, r2
	add	r5, r5, #256
	add	r9, r9, #256
	mov	r10, r4
	add	r6, r6, #64
	bne	.L2160
	ldr	r6, [fp, #-60]
	ldr	r1, [fp, #-64]
.L2155:
	ldr	r0, [r6, #56]
	ldr	r2, [r1, #56]
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L2153:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2157:
	cmp	r2, #0
	bne	.L2177
	sub	r2, r5, #1088
	sub	r4, r5, #836
	sub	r2, r2, #4
	mov	r1, r9
.L2159:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r4
	bne	.L2159
.L2177:
	add	r4, r10, #64
	b	.L2156
.L2179:
	mov	r0, r1
	movw	r3, #10242
	ldr	r2, .L2180
	ldr	r1, .L2180+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2153
.L2178:
	movw	r3, #10241
	ldr	r2, .L2180
	ldr	r1, .L2180+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2153
.L2181:
	.align	2
.L2180:
	.word	.LC13
	.word	.LC14
	UNWIND(.fnend)
	.size	MVC_PPSEqual, .-MVC_PPSEqual
	.align	2
	.global	MVC_DecPPS
	.type	MVC_DecPPS, %function
MVC_DecPPS:
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
	ldr	r1, .L2197
	mov	r4, r0
	bl	mvc_ue_v
	ldr	r3, [r4, #40]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r5, r0
	bls	.L2183
	mov	r2, r0
	ldr	r1, .L2197+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r4, #40]
	sub	r3, r2, #1
	cmp	r5, r3
	bls	.L2190
	ldr	r3, .L2197+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2190
	str	r2, [fp, #-40]
	mov	r3, #8
	str	r5, [fp, #-44]
	sub	r2, fp, #44
	ldr	r0, [r4, #120]
	mov	r1, #110
	blx	ip
	mvn	r0, #0
.L2184:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2183:
	ldr	r1, .L2197+12
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r3, [r4, #36]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	ip, r0
	bhi	.L2194
	mov	r8, #2240
	ldr	r3, [r4, #252]
	mul	r6, r8, r5
	add	r3, r3, r6
	ldrb	r7, [r3, #19]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L2186
	ldr	r7, .L2197+16
	mov	r0, r4
	mov	r1, r7
	str	r5, [r7, #24]
	str	ip, [r7, #28]
	bl	MVC_ProcessPPS
	subs	r5, r0, #0
	bne	.L2195
	ldr	r1, [r4, #252]
	mov	r0, r7
	add	r1, r1, r6
	bl	MVC_PPSEqual
	cmp	r0, #0
	beq	.L2184
	ldr	ip, .L2197+20
	mov	r3, #1
	ldr	r0, [r4, #252]
	mov	r2, r8
	mov	r1, r7
	strb	r3, [r7, #20]
	add	r0, r0, r6
	ldr	ip, [ip, #56]
	strb	r3, [r7, #19]
	blx	ip
	mov	r0, r5
	b	.L2184
.L2186:
	str	r5, [r3, #24]
	mov	r0, r4
	ldr	r3, [r4, #252]
	add	r3, r3, r6
	str	ip, [r3, #28]
	ldr	r1, [r4, #252]
	add	r1, r1, r6
	bl	MVC_ProcessPPS
	cmp	r0, #0
	bne	.L2196
	ldr	r3, [r4, #252]
	mov	r2, #1
	add	r3, r3, r6
	strb	r2, [r3, #20]
	ldr	r3, [r4, #252]
	add	r6, r3, r6
	strb	r2, [r6, #19]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2190:
	mvn	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2194:
	ldr	r1, .L2197+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2184
.L2195:
	movw	r2, #10005
	ldr	r1, .L2197+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2184
.L2196:
	movw	r2, #10025
	ldr	r1, .L2197+28
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #252]
	mov	r2, #1
	mvn	r0, #0
	add	r3, r3, r6
	strb	r2, [r3, #20]
	ldr	r3, [r4, #252]
	add	r6, r3, r6
	strb	r7, [r6, #19]
	b	.L2184
.L2198:
	.align	2
.L2197:
	.word	.LC167
	.word	.LC168
	.word	g_event_report
	.word	.LC169
	.word	.LANCHOR2
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC170
	.word	.LC171
	UNWIND(.fnend)
	.size	MVC_DecPPS, .-MVC_DecPPS
	.align	2
	.global	mvc_vui_parameters
	.type	mvc_vui_parameters, %function
mvc_vui_parameters:
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
	ldr	r1, .L2247
	mov	r4, r0
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	streqb	r0, [r5, #15]
	bne	.L2241
.L2201:
	ldr	r1, .L2247+4
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #1]
	cmp	r0, #0
	bne	.L2242
.L2202:
	ldr	r1, .L2247+8
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #3]
	cmp	r0, #0
	moveq	r3, #5
	streq	r3, [r5, #28]
	bne	.L2243
.L2204:
	ldr	r1, .L2247+12
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #6]
	cmp	r0, #0
	bne	.L2244
.L2205:
	ldr	r1, .L2247+16
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #8]
	cmp	r0, #0
	bne	.L2245
.L2207:
	ldr	r1, .L2247+20
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #7]
	cmp	r0, #0
	beq	.L2209
	ldr	r1, .L2247+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r2, .L2247+28
	mov	r1, #4
	str	r0, [r5, #96]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+32
	mov	r1, #4
	strb	r0, [r5, #92]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [r5, #96]
	cmp	r3, #31
	strb	r0, [r5, #93]
	bhi	.L2212
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2212
	add	r7, r5, #96
	add	r8, r5, #59
.L2213:
	ldr	r1, .L2247+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #96]
	cmp	r3, r6
	bcs	.L2213
	ldr	r2, .L2247+48
	mov	r1, #5
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+52
	mov	r1, #5
	str	r0, [r5, #356]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+56
	mov	r1, #5
	str	r0, [r5, #360]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+60
	mov	r1, #5
	str	r0, [r5, #364]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #368]
.L2209:
	ldr	r1, .L2247+64
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #10]
	cmp	r0, #0
	bne	.L2246
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2218
.L2217:
	ldr	r1, .L2247+68
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2247+72
	strb	r0, [r5, #11]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #12]
	cmp	r0, #0
	bne	.L2219
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
.L2220:
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2246:
	ldr	r1, .L2247+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r2, .L2247+28
	mov	r1, #4
	str	r0, [r5, #408]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+32
	mov	r1, #4
	strb	r0, [r5, #404]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [r5, #408]
	cmp	r3, #31
	strb	r0, [r5, #405]
	bhi	.L2212
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2212
	add	r8, r5, #368
	add	r7, r5, #408
	add	r8, r8, #3
.L2215:
	ldr	r1, .L2247+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #408]
	cmp	r3, r6
	bcs	.L2215
	ldr	r2, .L2247+48
	mov	r1, #5
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+52
	mov	r1, #5
	str	r0, [r5, #668]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+56
	mov	r1, #5
	str	r0, [r5, #672]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+60
	mov	r1, #5
	str	r0, [r5, #676]
	mov	r0, r4
	bl	mvc_u_v
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r5, #680]
	bne	.L2218
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2217
.L2218:
	ldr	r1, .L2247+76
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #14]
	b	.L2217
.L2245:
	ldr	r2, .L2247+80
	mov	r1, #32
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+84
	mov	r1, #32
	str	r0, [r5, #52]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2247+88
	str	r0, [r5, #56]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #9]
	cmp	r0, #1
	bne	.L2207
	ldr	r1, [r5, #52]
	cmp	r1, #0
	beq	.L2207
	ldr	r0, [r5, #56]
	bl	__aeabi_uidiv
	mov	r0, r0, lsr #1
	str	r0, [r4, #56]
	b	.L2207
.L2244:
	ldr	r1, .L2247+92
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+96
	str	r0, [r5, #44]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #48]
	b	.L2205
.L2243:
	ldr	r2, .L2247+100
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2247+104
	str	r0, [r5, #28]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2247+108
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #5]
	cmp	r0, #0
	beq	.L2204
	ldr	r2, .L2247+112
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+116
	mov	r1, #8
	str	r0, [r5, #32]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+120
	mov	r1, #8
	str	r0, [r5, #36]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #40]
	b	.L2204
.L2242:
	ldr	r1, .L2247+124
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #2]
	b	.L2202
.L2241:
	ldr	r2, .L2247+128
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	uxtb	r0, r0
	strb	r0, [r5, #15]
	cmp	r0, #255
	bne	.L2201
	ldr	r2, .L2247+132
	mov	r1, #16
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2247+136
	mov	r1, #16
	str	r0, [r5, #16]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #20]
	b	.L2201
.L2219:
	ldr	r1, .L2247+140
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2247+144
	strb	r0, [r5, #13]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+148
	str	r0, [r5, #684]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+152
	str	r0, [r5, #688]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+156
	str	r0, [r5, #696]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+160
	str	r0, [r5, #692]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2247+164
	str	r0, [r5, #700]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #704]
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L2220
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2212:
	ldr	r1, .L2247+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2248:
	.align	2
.L2247:
	.word	.LC172
	.word	.LC176
	.word	.LC178
	.word	.LC185
	.word	.LC188
	.word	.LC192
	.word	.LC193
	.word	.LC194
	.word	.LC195
	.word	.LC197
	.word	.LC198
	.word	.LC199
	.word	.LC200
	.word	.LC201
	.word	.LC202
	.word	.LC203
	.word	.LC204
	.word	.LC206
	.word	.LC207
	.word	.LC205
	.word	.LC189
	.word	.LC190
	.word	.LC191
	.word	.LC186
	.word	.LC187
	.word	.LC179
	.word	.LC180
	.word	.LC181
	.word	.LC182
	.word	.LC183
	.word	.LC184
	.word	.LC177
	.word	.LC173
	.word	.LC174
	.word	.LC175
	.word	.LC208
	.word	.LC209
	.word	.LC210
	.word	.LC211
	.word	.LC212
	.word	.LC213
	.word	.LC214
	.word	.LC196
	UNWIND(.fnend)
	.size	mvc_vui_parameters, .-mvc_vui_parameters
	.align	2
	.global	MVC_SPSEqual
	.type	MVC_SPSEqual, %function
MVC_SPSEqual:
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
	beq	.L2281
	cmp	r1, #0
	beq	.L2282
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
	ldrb	ip, [r1, #27]	@ zero_extendqisi2
	ldrb	r0, [r5, #27]	@ zero_extendqisi2
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r2, #1
	cmp	ip, r0
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r0, #0
	beq	.L2253
	add	r0, r1, #5
	add	r2, r5, #5
	sub	r9, r1, #404
	add	r7, r1, #748
	mov	ip, r0
	mov	r8, r5
	mov	r6, r5
	str	r5, [fp, #-48]
	mov	r0, #0
	mov	r4, r2
	mov	r5, ip
	str	r1, [fp, #-52]
.L2258:
	ldrb	r2, [r4, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r2
	cmp	ip, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, #0
	beq	.L2254
	cmp	r0, #5
	bhi	.L2255
	add	r2, r6, #748
	add	r10, r6, #812
	mov	r1, r7
.L2256:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2256
.L2254:
	add	r0, r0, #1
	add	r8, r8, #256
	cmp	r0, #8
	add	r9, r9, #256
	add	r6, r6, #64
	add	r7, r7, #64
	bne	.L2258
	ldr	r5, [fp, #-48]
	ldr	r1, [fp, #-52]
.L2253:
	ldr	r2, [r1, #2896]
	ldr	ip, [r5, #2900]
	ldr	r0, [r1, #2900]
	ldr	lr, [r5, #2896]
	cmp	ip, r0
	cmpeq	lr, r2
	moveq	r2, #1
	movne	r2, #0
	cmp	ip, #0
	and	r3, r3, r2
	beq	.L2260
	cmp	ip, #1
	bne	.L2259
	ldr	r0, [r1, #2908]
	ldr	ip, [r5, #2908]
	ldrb	r2, [r5, #18]	@ zero_extendqisi2
	ldrb	r6, [r1, #18]	@ zero_extendqisi2
	ldr	r4, [r5, #2912]
	cmp	ip, r0
	cmpeq	r2, r6
	ldr	lr, [r1, #2912]
	ldr	r6, [r5, #2916]
	ldr	ip, [r1, #2916]
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
	beq	.L2259
	add	ip, r5, #2912
	add	r0, r1, #2912
	add	ip, ip, #4
	add	r0, r0, #4
	mov	r2, #0
.L2262:
	ldr	r4, [ip, #4]!
	add	r2, r2, #1
	ldr	lr, [r0, #4]!
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r6
	bne	.L2262
.L2259:
	ldrb	ip, [r1, #19]	@ zero_extendqisi2
	ldrb	lr, [r5, #19]	@ zero_extendqisi2
	ldr	r0, [r5, #3944]
	ldr	r4, [r1, #3944]
	ldr	r2, [r5, #3948]
	cmp	lr, ip
	cmpeq	r0, r4
	ldr	r7, [r1, #3948]
	ldr	r6, [r5, #3952]
	ldr	r4, [r1, #3952]
	moveq	ip, #1
	movne	ip, #0
	ldrb	lr, [r1, #20]	@ zero_extendqisi2
	cmp	r2, r7
	movne	r0, #0
	andeq	r0, ip, #1
	ldrb	ip, [r5, #20]	@ zero_extendqisi2
	cmp	r6, r4
	movne	r2, #0
	andeq	r2, r0, #1
	cmp	lr, ip
	movne	r2, #0
	andeq	r2, r2, #1
	cmp	ip, #0
	and	r3, r3, r2
	bne	.L2263
	ldrb	r0, [r5, #21]	@ zero_extendqisi2
	ldrb	r2, [r1, #21]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
.L2263:
	ldrb	r2, [r1, #22]	@ zero_extendqisi2
	ldrb	ip, [r5, #23]	@ zero_extendqisi2
	ldrb	r0, [r1, #23]	@ zero_extendqisi2
	ldrb	lr, [r5, #22]	@ zero_extendqisi2
	cmp	r0, ip
	cmpeq	lr, r2
	moveq	r2, #1
	movne	r2, #0
	cmp	ip, #0
	and	r3, r3, r2
	bne	.L2283
.L2264:
	ldrb	r0, [r5, #24]	@ zero_extendqisi2
	ldrb	r2, [r1, #24]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L2251:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2283:
	ldr	ip, [r1, #3960]
	ldr	lr, [r5, #3960]
	ldr	r0, [r5, #3956]
	ldr	r4, [r1, #3956]
	ldr	r2, [r1, #3964]
	cmp	lr, ip
	cmpeq	r0, r4
	ldr	r6, [r5, #3964]
	ldr	r4, [r5, #3968]
	ldr	lr, [r1, #3968]
	moveq	ip, #1
	movne	ip, #0
	cmp	r6, r2
	movne	r0, #0
	andeq	r0, ip, #1
	cmp	r4, lr
	movne	r2, #0
	andeq	r2, r0, #1
	and	r3, r3, r2
	b	.L2264
.L2255:
	sub	r2, r8, #404
	sub	r10, r8, #148
	mov	r1, r9
.L2257:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2257
	b	.L2254
.L2260:
	ldr	r0, [r5, #2904]
	ldr	r2, [r1, #2904]
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
	b	.L2259
.L2282:
	mov	r0, r1
	movw	r3, #10486
	ldr	r2, .L2284
	ldr	r1, .L2284+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2251
.L2281:
	movw	r3, #10485
	ldr	r2, .L2284
	ldr	r1, .L2284+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2251
.L2285:
	.align	2
.L2284:
	.word	.LC13
	.word	.LC14
	UNWIND(.fnend)
	.size	MVC_SPSEqual, .-MVC_SPSEqual
	.global	__aeabi_idiv
	.align	2
	.global	MVC_GetDar
	.type	MVC_GetDar, %function
MVC_GetDar:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	beq	.L2309
	subs	ip, r4, #255
	movne	ip, #1
	cmp	r4, #16
	movle	lr, #0
	andgt	lr, ip, #1
	cmp	lr, #0
	bne	.L2298
	cmp	r2, #0
	cmpne	r1, #0
	moveq	r0, #1
	movne	r0, #0
	cmp	r4, #255
	movne	r0, #0
	andeq	r0, r0, #1
	cmp	r0, #0
	bne	.L2299
	cmp	r4, #1
	moveq	r2, r0
	moveq	r4, #5
	beq	.L2287
	cmp	ip, #0
	ldrne	r0, .L2310
	addne	r4, r0, r4, lsl #3
	ldrne	r1, [r4, #100]
	ldrne	r2, [r4, #104]
	mul	r0, r3, r1
	ldr	r3, [fp, #4]
	mul	r1, r3, r2
	mov	r0, r0, asl #10
	bl	__aeabi_idiv
	movw	r3, #2405
	movw	r1, #2262
	cmp	r0, r3
	mov	r2, r0
	rsble	r3, r0, #2400
	subgt	r3, r0, #2400
	addle	r3, r3, #6
	subgt	r3, r3, #6
	cmp	r0, r1
	bgt	.L2291
	rsb	r1, r0, #2256
	add	r1, r1, #7
	cmp	r3, r1
	bge	.L2292
.L2302:
	mov	r4, #4
	b	.L2287
.L2298:
	mov	r4, #0
.L2309:
	mov	r2, r4
.L2287:
	mov	r3, r4
	ldr	r1, .L2310+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2291:
	sub	r1, r0, #2256
	sub	r1, r1, #7
	cmp	r3, r1
	blt	.L2302
.L2292:
	movw	r3, #1819
	cmp	r2, r3
	bgt	.L2293
	rsb	r3, r2, #1808
	add	r3, r3, #12
	cmp	r1, r3
	blt	.L2304
.L2294:
	movw	r1, #1364
	cmp	r2, r1
	rsble	r4, r2, #1360
	subgt	r4, r2, #1360
	addle	r4, r4, #5
	subgt	r4, r4, #5
	cmp	r3, r4
	movlt	r4, #2
	movge	r4, #1
	b	.L2287
.L2299:
	mov	r4, lr
	mov	r2, lr
	b	.L2287
.L2293:
	sub	r3, r2, #1808
	sub	r3, r3, #12
	cmp	r1, r3
	bge	.L2294
.L2304:
	mov	r4, #3
	b	.L2287
.L2311:
	.align	2
.L2310:
	.word	.LANCHOR0
	.word	.LC215
	UNWIND(.fnend)
	.size	MVC_GetDar, .-MVC_GetDar
	.align	2
	.global	MVC_ProcessSPS
	.type	MVC_ProcessSPS, %function
MVC_ProcessSPS:
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
	subs	r5, r1, #0
	mov	r4, r0
	beq	.L2425
	ldr	r3, [r5, #736]
	cmp	r3, #100
	cmpne	r3, #122
	bic	r1, r3, #16
	sub	r3, r3, #110
	moveq	r2, #1
	movne	r2, #0
	cmp	r1, #128
	orreq	r2, r2, #1
	bics	r3, r3, #8
	orreq	r3, r2, #1
	movne	r3, r2
	cmp	r3, #0
	streqb	r3, [r5, #27]
	moveq	r3, #1
	streq	r3, [r5, #748]
	bne	.L2426
.L2325:
	ldr	r1, .L2440
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #12
	str	r0, [r5, #2896]
	bhi	.L2338
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2338
	ldr	r1, .L2440+4
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r5, #2900]
	bhi	.L2340
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2340
	cmp	r0, #0
	beq	.L2427
	cmp	r0, #1
	beq	.L2428
.L2346:
	ldr	r1, .L2440+8
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #3944]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	ldr	r1, .L2440+12
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #19]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	ldr	r1, .L2440+16
	mov	r0, r4
	bl	mvc_ue_v
	movw	r7, #509
	sub	r3, r0, #1
	str	r0, [r5, #3948]
	cmp	r3, r7
	bhi	.L2351
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2351
	ldr	r1, .L2440+20
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #3952]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	ldr	r1, .L2440+24
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #20]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	cmp	r0, #0
	bne	.L2429
	ldr	r1, .L2440+28
	mov	r0, r4
	bl	mvc_u_1
	ldr	r6, [r5, #3952]
	add	r6, r6, #1
	cmp	r6, #255
	strb	r0, [r5, #21]
	bhi	.L2384
	ldr	r3, [r5, #3948]
	add	r3, r3, #1
	cmp	r6, #1
	cmphi	r3, #3
	movhi	r8, #2
	bhi	.L2355
.L2384:
	ldr	r1, .L2440+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2426:
	ldr	r1, .L2440+36
	bl	mvc_ue_v
	cmp	r0, #1
	str	r0, [r5, #748]
	bhi	.L2316
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2316
	ldr	r1, .L2440+40
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L2318
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2319
.L2318:
	ldr	r1, .L2440+44
	mov	r0, #1
	bl	dprint_vfmw
.L2319:
	ldr	r1, .L2440+48
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L2320
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2321
.L2320:
	ldr	r1, .L2440+52
	mov	r0, #1
	bl	dprint_vfmw
.L2321:
	ldr	r1, .L2440+56
	mov	r0, r4
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L2322
	ldrb	r10, [r4, #10]	@ zero_extendqisi2
	cmp	r10, #0
	bne	.L2322
	ldr	r1, .L2440+60
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #27]
	cmp	r0, #1
	bne	.L2325
	sub	r7, fp, #72
	add	r9, r5, #5
	add	r6, r5, #2672
	add	r8, r5, #752
	b	.L2331
.L2432:
	ldr	r3, .L2440+64
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2440+68
	ldr	r3, [r3, #52]
	blx	r3
.L2327:
	add	r10, r10, #1
	add	r6, r6, #16
	cmp	r10, #6
	add	r8, r8, #64
	beq	.L2430
.L2331:
	ldr	r1, .L2440+72
	mov	r0, r4
	bl	mvc_u_1
	clz	r1, r10
	mov	r2, #16
	mov	r1, r1, lsr #5
	cmp	r0, #1
	mov	r3, r0
	mov	r0, r6
	str	r3, [r7, #4]!
	strb	r3, [r9, #1]!
	beq	.L2431
	cmp	r10, #0
	cmpne	r10, #3
	beq	.L2432
	ldr	r3, .L2440+68
	mov	r2, #16
	sub	r1, r6, #16
	mov	r0, r6
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2327
.L2382:
	cmp	r1, r9
	cmpcs	r0, r6
	bcs	.L2381
	ldr	r3, .L2440+76
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2423
	strh	r1, [fp, #-72]	@ movhi
	mov	r3, #8
	strh	r0, [fp, #-70]	@ movhi
	sub	r2, fp, #76
	strh	r9, [fp, #-76]	@ movhi
	mov	r1, #107
	strh	r6, [fp, #-74]	@ movhi
	ldr	r0, [r4, #120]
	blx	ip
.L2423:
	mvn	r0, #0
.L2416:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2429:
	ldr	r6, [r5, #3952]
	strb	r3, [r5, #21]
	sub	r3, r6, #1
	cmp	r3, r7
	bhi	.L2384
	ldr	r3, [r5, #3948]
	mov	r8, #1
	add	r6, r6, #1
	add	r3, r3, r8
.L2355:
	mul	r6, r6, r8
	mov	r9, r3, asl #4
	mov	r6, r6, asl #4
	mul	r1, r9, r6
	add	r1, r1, r1, lsr #1
	cmp	r1, #100663296
	bhi	.L2433
	ldr	r2, [r5, #740]
	sub	r3, r2, #10
	cmp	r3, #31
	ldrls	pc, [pc, r3, asl #2]
	b	.L2357
.L2359:
	.word	.L2358
	.word	.L2360
	.word	.L2391
	.word	.L2391
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2391
	.word	.L2362
	.word	.L2364
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2364
	.word	.L2365
	.word	.L2366
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2357
	.word	.L2368
	.word	.L2368
.L2427:
	ldr	r1, .L2440+80
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #12
	str	r0, [r5, #2904]
	bhi	.L2343
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2346
.L2343:
	ldr	r1, .L2440+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2391:
	mov	r0, #60416
	movt	r0, 13
.L2361:
	bl	__aeabi_uidiv
	ldr	r1, .L2440+88
	cmp	r0, #16
	movcc	r7, r0
	movcs	r7, #16
	mov	r2, r7
	mov	r0, #21
	bl	dprint_vfmw
	ldr	r2, [r5, #3944]
	cmp	r2, r7
	bhi	.L2369
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2370
.L2369:
	mov	r3, r7
	ldr	r1, .L2440+92
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r5, #3944]
	cmp	r3, #16
	bhi	.L2420
	cmp	r7, r3
	movcc	r7, r3
.L2420:
	str	r7, [r5, #3944]
.L2370:
	ldr	r1, .L2440+96
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #22]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	ldr	r1, .L2440+100
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #23]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	cmp	r0, #0
	bne	.L2434
.L2372:
	mov	r2, r9, lsr #1
	mov	r3, r6, lsr #1
	str	r9, [r5, #3976]
	str	r6, [r5, #3980]
	str	r2, [r5, #3984]
	str	r3, [r5, #3988]
.L2374:
	ldr	r1, .L2440+104
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r3, r0
	strb	r3, [r5, #24]
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2423
	cmp	r3, #0
	mov	r2, #2
	str	r2, [r5, #68]
	bne	.L2435
	mov	r0, r3
	strb	r3, [r5, #43]
	mov	r3, #5
	str	r3, [r5, #56]
.L2377:
	ldr	r2, [r5, #48]
	mov	r3, r9
	ldr	r1, [r5, #44]
	str	r6, [sp]
	bl	MVC_GetDar
	ldr	r3, [r5, #3944]
	cmp	r3, #0
	str	r0, [r5, #52]
	beq	.L2380
	cmp	r7, r3
	movcs	r7, r3
.L2380:
	ldr	r3, [r4, #224]
	cmp	r7, #16
	movcs	r7, #16
	ldr	r2, [r3, #28]
	cmp	r2, #25
	beq	.L2436
.L2381:
	cmp	r6, #1920
	bcc	.L2383
	cmp	r7, #4
	movcs	r7, #4
.L2383:
	add	r7, r7, #1
	mov	r0, #0
	str	r7, [r5, #3972]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2368:
	mov	r0, #12582912
	b	.L2361
.L2364:
	mov	r0, #30208
	movt	r0, 47
	b	.L2361
.L2430:
	sub	r7, fp, #88
	add	r9, r5, #11
	add	r6, r5, #2768
	add	r8, r5, #1136
	mov	r10, #0
.L2336:
	ldr	r1, .L2440+72
	mov	r0, r4
	bl	mvc_u_1
	mov	r2, #64
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r6
	str	r3, [r7, #4]!
	strb	r3, [r9, #1]!
	beq	.L2437
	ldr	r1, .L2440+108
	cmp	r10, #0
	ldr	r3, .L2440+68
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2333:
	add	r10, r10, #1
	add	r6, r6, #64
	cmp	r10, #2
	add	r8, r8, #256
	bne	.L2336
	b	.L2325
.L2431:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2327
	ldr	r1, .L2440+64
	mov	r2, #16
	ldr	r3, .L2440+68
	cmp	r10, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2327
.L2316:
	ldr	r1, .L2440+112
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2338:
	ldr	r1, .L2440+116
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2434:
	ldr	r1, .L2440+120
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2440+124
	str	r0, [r5, #3956]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2440+128
	str	r0, [r5, #3960]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2440+132
	str	r0, [r5, #3964]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r3, [r5, #3964]
	ldr	r2, [r5, #3956]
	mov	r1, r8, asl #1
	ldr	ip, [r5, #3960]
	mov	r3, r3, asl #1
	mov	r2, r2, asl #1
	mul	r8, r8, r3
	rsb	r3, r2, r9
	sub	r3, r3, ip, asl #1
	rsb	ip, r8, r6
	str	r0, [r5, #3968]
	mls	r0, r0, r1, ip
	cmp	r3, #0
	cmpgt	r0, #0
	ble	.L2372
	add	r2, r2, r3, lsr #1
	add	r8, r8, r0, lsr #1
	str	r2, [r5, #3984]
	str	r8, [r5, #3988]
	str	r3, [r5, #3976]
	str	r0, [r5, #3980]
	b	.L2374
.L2435:
	add	r1, r5, #28
	mov	r0, r4
	bl	mvc_vui_parameters
	cmp	r0, #0
	bne	.L2422
	ldrb	r3, [r5, #40]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2422
	ldr	r2, [r5, #732]
	clz	r3, r2
	mov	r3, r3, lsr #5
	cmp	r7, r2
	orrcc	r3, r3, #1
	cmp	r3, #0
	bne	.L2438
	ldr	r3, [r5, #3944]
	ldrb	r0, [r5, #43]	@ zero_extendqisi2
	cmp	r2, r3
	movcs	r7, r2
	movcc	r7, r3
	b	.L2377
.L2438:
	mov	r3, r7
	ldr	r1, .L2440+136
	mov	r0, #1
	bl	dprint_vfmw
.L2422:
	ldrb	r0, [r5, #43]	@ zero_extendqisi2
	b	.L2377
.L2436:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L2381
	ldr	r2, [r3, #692]
	ldr	r1, [r3, #668]
	cmp	r7, r2
	ldr	r0, [r3, #672]
	bls	.L2382
	ldr	r3, .L2440+76
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2423
	str	r2, [fp, #-72]
	mov	r3, #8
	str	r7, [fp, #-76]
	sub	r2, fp, #76
	ldr	r0, [r4, #120]
	mov	r1, #106
	blx	ip
	mvn	r0, #0
	b	.L2416
.L2322:
	ldr	r1, .L2440+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2428:
	ldr	r1, .L2440+144
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #18]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	ldr	r1, .L2440+148
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r5, #2908]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	ldr	r1, .L2440+152
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r5, #2912]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2423
	ldr	r1, .L2440+156
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #255
	str	r0, [r5, #2916]
	bhi	.L2348
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2348
	cmp	r0, #0
	addne	r7, r5, #2912
	addne	r7, r7, #4
	bne	.L2350
	b	.L2346
.L2439:
	ldr	r3, [r5, #2916]
	cmp	r3, r6
	bls	.L2346
.L2350:
	ldr	r1, .L2440+160
	mov	r0, r4
	bl	mvc_se_v
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2439
	b	.L2423
.L2351:
	mov	r2, r0
	ldr	r1, .L2440+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2365:
	mov	r0, #30720
	movt	r0, 105
	b	.L2361
.L2362:
	mov	r0, #55296
	movt	r0, 27
	b	.L2361
.L2366:
	mov	r0, #7864320
	b	.L2361
.L2360:
	mov	r0, #17920
	movt	r0, 5
	b	.L2361
.L2358:
	mov	r0, #20992
	movt	r0, 2
	b	.L2361
.L2357:
	ldr	r1, .L2440+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2348:
	ldr	r1, .L2440+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2437:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2333
	ldr	r1, .L2440+108
	mov	r2, #64
	ldr	r3, .L2440+68
	cmp	r10, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2333
.L2340:
	ldr	r1, .L2440+176
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2433:
	ldr	r1, .L2440+180
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2425:
	mov	r0, r5
	movw	r3, #10675
	ldr	r2, .L2440+184
	ldr	r1, .L2440+188
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2416
.L2441:
	.align	2
.L2440:
	.word	.LC225
	.word	.LC227
	.word	.LC230
	.word	.LC238
	.word	.LC239
	.word	.LC241
	.word	.LC242
	.word	.LC256
	.word	.LC243
	.word	.LC216
	.word	.LC218
	.word	.LC219
	.word	.LC220
	.word	.LC221
	.word	.LC222
	.word	.LC224
	.word	.LANCHOR1
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC164
	.word	g_event_report
	.word	.LC229
	.word	.LC231
	.word	.LC246
	.word	.LC247
	.word	.LC248
	.word	.LC249
	.word	.LC254
	.word	.LANCHOR1+32
	.word	.LC217
	.word	.LC226
	.word	.LC250
	.word	.LC251
	.word	.LC252
	.word	.LC253
	.word	.LC255
	.word	.LC223
	.word	.LC232
	.word	.LC233
	.word	.LC234
	.word	.LC235
	.word	.LC237
	.word	.LC240
	.word	.LC245
	.word	.LC236
	.word	.LC228
	.word	.LC244
	.word	.LC13
	.word	.LC14
	UNWIND(.fnend)
	.size	MVC_ProcessSPS, .-MVC_ProcessSPS
	.align	2
	.global	MVC_DecSPS
	.type	MVC_DecSPS, %function
MVC_DecSPS:
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
	ldr	r2, .L2487
	mov	r1, #8
	mov	r4, r0
	bl	mvc_u_v
	ldr	r1, .L2487+4
	mov	r7, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2487+8
	mov	r9, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2487+12
	mov	r8, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2487+16
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2487+20
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2487+24
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r2, .L2487+28
	mov	r1, #2
	str	r0, [fp, #-68]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2487+32
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2487+36
	mov	r5, r0
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	mov	r6, r0
	bne	.L2480
	ldr	r2, [r4, #36]
	sub	r2, r2, #1
	cmp	r0, r2
	bhi	.L2482
	cmp	r7, #100
	mov	r10, r7
	beq	.L2448
	bhi	.L2449
	cmp	r7, #77
	beq	.L2448
	cmp	r7, #88
	beq	.L2450
	cmp	r7, #66
	bne	.L2447
	ldr	r1, .L2487+40
	mov	r0, #1
	bl	dprint_vfmw
.L2448:
	cmp	r5, #9
	mov	r2, r5
	bls	.L2479
.L2485:
	cmp	r5, #41
	bhi	.L2479
.L2454:
	movw	r1, #3992
	ldr	r3, [r4, #248]
	mul	r5, r1, r6
	add	r3, r3, r5
	ldrb	ip, [r3, #25]	@ zero_extendqisi2
	cmp	ip, #0
	beq	.L2455
	ldr	r3, .L2487+44
	mov	r0, r4
	ldrb	ip, [fp, #-56]	@ zero_extendqisi2
	add	r1, r3, #2240
	str	r10, [r3, #2976]
	strb	ip, [r3, #2242]
	ldrb	ip, [fp, #-60]	@ zero_extendqisi2
	strb	r9, [r3, #2240]
	strb	r8, [r3, #2241]
	strb	ip, [r3, #2243]
	ldrb	ip, [fp, #-64]	@ zero_extendqisi2
	str	r2, [r3, #2980]
	str	r6, [r3, #2984]
	strb	ip, [r3, #2244]
	ldrb	ip, [fp, #-68]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	strb	ip, [r3, #2245]
	bl	MVC_ProcessSPS
	ldr	r3, [fp, #-56]
	subs	r7, r0, #0
	bne	.L2483
	ldr	r1, [r4, #248]
	add	r0, r3, #2240
	str	r3, [fp, #-56]
	add	r1, r1, r5
	bl	MVC_SPSEqual
	ldr	r3, [fp, #-56]
	cmp	r0, #0
	bne	.L2484
.L2444:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2449:
	cmp	r7, #122
	beq	.L2452
	cmp	r7, #144
	beq	.L2452
	cmp	r7, #110
	beq	.L2452
.L2447:
	mov	r2, r7
	ldr	r1, .L2487+48
	mov	r0, #1
	mov	r10, #100
	bl	dprint_vfmw
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2485
.L2479:
	mov	r3, #41
	ldr	r1, .L2487+52
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, #41
	b	.L2454
.L2455:
	str	r10, [r3, #736]
	mov	r0, r4
	ldr	r3, [r4, #248]
	ldrb	r1, [fp, #-56]	@ zero_extendqisi2
	str	ip, [fp, #-72]
	strb	r9, [r3, r5]
	ldr	r3, [r4, #248]
	add	r3, r3, r5
	strb	r8, [r3, #1]
	ldr	r3, [r4, #248]
	add	r3, r3, r5
	strb	r1, [r3, #2]
	ldr	r3, [r4, #248]
	ldrb	r1, [fp, #-60]	@ zero_extendqisi2
	add	r3, r3, r5
	strb	r1, [r3, #3]
	ldr	r3, [r4, #248]
	ldrb	r1, [fp, #-64]	@ zero_extendqisi2
	add	r3, r3, r5
	strb	r1, [r3, #4]
	ldr	r3, [r4, #248]
	ldrb	r1, [fp, #-68]	@ zero_extendqisi2
	add	r3, r3, r5
	strb	r1, [r3, #5]
	ldr	r3, [r4, #248]
	add	r3, r3, r5
	str	r2, [r3, #740]
	ldr	r3, [r4, #248]
	add	r3, r3, r5
	str	r6, [r3, #744]
	ldr	r1, [r4, #248]
	add	r1, r1, r5
	bl	MVC_ProcessSPS
	ldr	ip, [fp, #-72]
	cmp	r0, #0
	bne	.L2486
	ldr	r3, [r4, #248]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #26]
	ldr	r3, [r4, #248]
	add	r5, r3, r5
	strb	r2, [r5, #25]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2482:
	ldr	r1, .L2487+56
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r4, #36]
	sub	r3, r2, #1
	cmp	r6, r3
	bls	.L2480
	ldr	r3, .L2487+60
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2480
	str	r2, [fp, #-48]
	mov	r3, #8
	str	r6, [fp, #-52]
	sub	r2, fp, #52
	ldr	r0, [r4, #120]
	mov	r1, #109
	blx	ip
.L2480:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2452:
	ldr	r1, .L2487+64
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2485
	b	.L2479
.L2450:
	ldr	r1, .L2487+68
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2485
	b	.L2479
.L2484:
	ldr	lr, .L2487+72
	mov	ip, #1
	ldr	r0, [r4, #248]
	movw	r2, #3992
	add	r1, r3, #2240
	strb	ip, [r3, #2266]
	add	r0, r0, r5
	ldr	r4, [lr, #56]
	strb	ip, [r3, #2265]
	blx	r4
	mov	r0, r7
	b	.L2444
.L2483:
	mov	r3, r6
	movw	r2, #11219
	ldr	r1, .L2487+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2444
.L2486:
	mov	r3, r6
	movw	r2, #11246
	ldr	r1, .L2487+76
	mov	r0, #1
	str	ip, [fp, #-56]
	bl	dprint_vfmw
	ldr	r3, [r4, #248]
	mov	r2, #1
	ldr	ip, [fp, #-56]
	mvn	r0, #0
	add	r3, r3, r5
	strb	r2, [r3, #26]
	ldr	r3, [r4, #248]
	add	r5, r3, r5
	strb	ip, [r5, #25]
	b	.L2444
.L2488:
	.align	2
.L2487:
	.word	.LC257
	.word	.LC258
	.word	.LC259
	.word	.LC260
	.word	.LC261
	.word	.LC262
	.word	.LC263
	.word	.LC264
	.word	.LC265
	.word	.LC266
	.word	.LC267
	.word	.LANCHOR2
	.word	.LC270
	.word	.LC271
	.word	.LC170
	.word	g_event_report
	.word	.LC269
	.word	.LC268
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC272
	UNWIND(.fnend)
	.size	MVC_DecSPS, .-MVC_DecSPS
	.align	2
	.global	MVC_FreeMvcExtMem
	.type	MVC_FreeMvcExtMem, %function
MVC_FreeMvcExtMem:
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
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_FreeMvcExtMem, .-MVC_FreeMvcExtMem
	.align	2
	.global	MVC_FreeMvcVuiExtMem
	.type	MVC_FreeMvcVuiExtMem, %function
MVC_FreeMvcVuiExtMem:
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
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_FreeMvcVuiExtMem, .-MVC_FreeMvcVuiExtMem
	.align	2
	.global	MVC_ProcessSUBSPSMvcExt
	.type	MVC_ProcessSUBSPSMvcExt, %function
MVC_ProcessSUBSPSMvcExt:
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
	subs	r5, r1, #0
	mov	r4, r0
	beq	.L2553
	ldr	r1, .L2554
	bl	mvc_ue_v
	cmp	r0, #1
	str	r0, [r5, #4]
	bhi	.L2494
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	addeq	r7, r5, #4
	bne	.L2494
.L2496:
	ldr	r1, .L2554+4
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldr	r3, [r5, #4]
	cmp	r3, r6
	bcs	.L2496
	cmp	r3, #0
	mov	r2, #0
	str	r2, [r5, #16]
	str	r2, [r5, #24]
	beq	.L2497
	add	r8, r5, #16
	mov	r7, #1
.L2507:
	ldr	r1, .L2554+8
	mov	r0, r4
	bl	mvc_ue_v
	add	r9, r8, #4
	str	r0, [r8, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcs	r3, #15
	cmp	r0, r3
	bhi	.L2498
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2498
	cmp	r0, #0
	addne	r10, r8, #16
	beq	.L2503
.L2502:
	ldr	r1, .L2554+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r10, #4]!
	ldr	r3, [r9]
	cmp	r6, r3
	bcc	.L2502
.L2503:
	ldr	r1, .L2554+16
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #8]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcc	r2, r3
	movcs	r2, #15
	cmp	r0, r2
	bhi	.L2500
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2500
	cmp	r0, #0
	addne	r8, r8, #24
	beq	.L2505
.L2506:
	ldr	r1, .L2554+20
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r8, #4]!
	ldr	r3, [r9, #8]
	cmp	r6, r3
	bcc	.L2506
	ldr	r3, [r5, #4]
.L2505:
	add	r7, r7, #1
	mov	r8, r9
	cmp	r7, r3
	bls	.L2507
	cmp	r3, #0
	mov	r3, #0
	addne	r8, r5, #48
	str	r3, [r5, #48]
	str	r3, [r5, #56]
	movne	r7, #1
	beq	.L2521
.L2520:
	ldr	r1, .L2554+24
	mov	r0, r4
	bl	mvc_ue_v
	add	r9, r8, #4
	str	r0, [r8, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcs	r3, #15
	cmp	r0, r3
	bhi	.L2511
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2511
	cmp	r0, #0
	addne	r10, r8, #16
	beq	.L2516
.L2515:
	ldr	r1, .L2554+28
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r10, #4]!
	ldr	r3, [r9]
	cmp	r6, r3
	bcc	.L2515
.L2516:
	ldr	r1, .L2554+32
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #8]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcc	r2, r3
	movcs	r2, #15
	cmp	r0, r2
	bhi	.L2513
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2513
	cmp	r0, #0
	addne	r8, r8, #24
	beq	.L2518
.L2519:
	ldr	r1, .L2554+36
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r8, #4]!
	ldr	r3, [r9, #8]
	cmp	r6, r3
	bcc	.L2519
	ldr	r3, [r5, #4]
.L2518:
	add	r7, r7, #1
	mov	r8, r9
	cmp	r7, r3
	bls	.L2520
.L2521:
	ldr	r1, .L2554+40
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r0, #1
	str	r0, [r5, #80]
	cmp	r3, #16
	str	r3, [fp, #-60]
	bhi	.L2509
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2509
	ldr	r2, [fp, #-60]
	cmp	r2, #0
	beq	.L2526
	add	r2, r5, #8384
	add	r1, r5, #80
	add	r2, r2, #16
	str	r1, [fp, #-72]
	str	r2, [fp, #-68]
	add	r2, r5, #212
	str	r3, [fp, #-56]
	str	r2, [fp, #-64]
	str	r5, [fp, #-76]
.L2525:
	ldr	r2, .L2554+44
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-72]
	ldr	r1, .L2554+48
	str	r0, [r3, #4]!
	mov	r0, r4
	mov	r5, r3
	str	r3, [fp, #-72]
	bl	mvc_ue_v
	add	r3, r0, #1
	str	r0, [r5, #64]
	str	r3, [fp, #-52]
	mov	r2, r3
	cmp	r2, #64
	bhi	.L2523
	ldrb	r7, [r4, #10]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L2523
	cmp	r2, #0
	beq	.L2533
	ldr	r3, [fp, #-64]
	movw	r10, #8188
	ldr	r2, [fp, #-68]
	movt	r10, 4
	add	r10, r3, r10
	mov	r8, r3
	str	r2, [fp, #-48]
.L2532:
	ldr	r2, .L2554+52
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2554+56
	str	r0, [r8], #4
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r0, #1
	str	r0, [r8, #4092]
	cmp	r6, #64
	bhi	.L2527
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2527
	cmp	r6, #0
	movne	r5, r3
	ldrne	r9, [fp, #-48]
	beq	.L2531
.L2528:
	ldr	r1, .L2554+60
	mov	r0, r4
	bl	mvc_ue_v
	add	r5, r5, #1
	cmp	r6, r5
	str	r0, [r9, #4]!
	bne	.L2528
.L2531:
	ldr	r1, .L2554+64
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #1024
	str	r0, [r10, #4]!
	bcs	.L2529
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2529
	ldr	r3, [fp, #-48]
	add	r7, r7, #1
	add	r3, r3, #256
	str	r3, [fp, #-48]
	ldr	r3, [fp, #-52]
	cmp	r3, r7
	bne	.L2532
.L2533:
	ldr	r2, [fp, #-64]
	ldr	r3, [fp, #-56]
	add	r2, r2, #256
	str	r2, [fp, #-64]
	ldr	r2, [fp, #-60]
	add	r3, r3, #1
	str	r3, [fp, #-56]
	cmp	r2, r3
	ldr	r3, [fp, #-68]
	add	r3, r3, #16384
	str	r3, [fp, #-68]
	bne	.L2525
.L2526:
	mov	r0, #0
	b	.L2545
.L2498:
	mov	r2, r0
	ldr	r1, .L2554+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2545:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2500:
	mov	r2, r0
	ldr	r1, .L2554+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2513:
	mov	r2, r0
	ldr	r1, .L2554+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2511:
	mov	r2, r0
	ldr	r1, .L2554+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2494:
	mov	r2, r0
	movw	r3, #65534
	ldr	r1, .L2554+84
	mov	r0, #1
	str	r3, [r5, #80]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2497:
	str	r3, [r5, #48]
	str	r3, [r5, #56]
	b	.L2521
.L2529:
	mov	r2, r0
	ldr	r1, .L2554+88
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2527:
	ldr	r3, [fp, #-56]
	movw	ip, #65534
	ldr	r5, [fp, #-76]
	mov	r2, r6
	ldr	r1, .L2554+92
	mov	r0, #1
	add	r3, r7, r3, lsl #6
	add	r3, r3, #1072
	add	r3, r3, #4
	add	r3, r5, r3, lsl #2
	str	ip, [r3, #4]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2553:
	mov	r0, r5
	movw	r3, #11366
	ldr	r2, .L2554+96
	ldr	r1, .L2554+100
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2509:
	movw	r3, #65534
	ldr	r2, [fp, #-60]
	ldr	r1, .L2554+104
	mov	r0, #1
	str	r3, [r5, #80]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2523:
	ldr	r3, [fp, #-56]
	mov	r0, #1
	ldr	r5, [fp, #-76]
	ldr	r2, [fp, #-52]
	add	r5, r5, r3, lsl #2
	ldr	r1, .L2554+108
	movw	r3, #65534
	str	r3, [r5, #148]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2545
.L2555:
	.align	2
.L2554:
	.word	.LC273
	.word	.LC275
	.word	.LC276
	.word	.LC279
	.word	.LC277
	.word	.LC281
	.word	.LC283
	.word	.LC286
	.word	.LC284
	.word	.LC288
	.word	.LC282
	.word	.LC290
	.word	.LC291
	.word	.LC293
	.word	.LC294
	.word	.LC297
	.word	.LC295
	.word	.LC278
	.word	.LC280
	.word	.LC287
	.word	.LC285
	.word	.LC274
	.word	.LC298
	.word	.LC296
	.word	.LC13
	.word	.LC14
	.word	.LC289
	.word	.LC292
	UNWIND(.fnend)
	.size	MVC_ProcessSUBSPSMvcExt, .-MVC_ProcessSUBSPSMvcExt
	.align	2
	.global	MVC_ProcessSUBSPSMvcVuiExt
	.type	MVC_ProcessSUBSPSMvcVuiExt, %function
MVC_ProcessSUBSPSMvcVuiExt:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #76)
	sub	sp, sp, #76
	mov	r5, r1
	ldr	r1, .L2589
	mov	r4, r0
	bl	mvc_ue_v
	add	r3, r5, #278528
	cmp	r0, #1024
	str	r0, [r3, #560]
	bcs	.L2586
	adds	r6, r0, #1
	mov	r1, #0
	adc	r7, r1, #0
	strd	r6, [fp, #-116]
	orrs	r2, r6, r7
	beq	.L2574
	add	r1, r5, #294912
	add	r9, r5, #315392
	add	r9, r9, #856
	movw	r2, #16687
	mov	ip, r1
	movw	r6, #34136
	movw	r1, #16751
	movw	r0, #33844
	mov	r8, r5
	movt	r2, 4
	add	ip, ip, #820
	add	r2, r5, r2
	movt	r6, 4
	str	r2, [fp, #-104]
	movt	r1, 4
	add	r6, r5, r6
	add	r2, r5, r1
	movt	r0, 4
	str	r2, [fp, #-96]
	add	r2, r3, #111
	str	ip, [fp, #-76]
	str	r2, [fp, #-88]
	add	r2, r5, r0
	str	r6, [fp, #-48]
	str	r2, [fp, #-80]
	add	r2, r3, #560
	str	r2, [fp, #-92]
	add	r2, r3, #176
	str	r2, [fp, #-72]
	add	r2, r3, #432
	str	r2, [fp, #-52]
	add	r2, r3, #496
	str	r2, [fp, #-56]
	add	r2, r3, #240
	add	r3, r3, #816
	str	r2, [fp, #-84]
	str	r3, [fp, #-60]
	mov	r2, #1
	mov	r3, #0
	strd	r2, [fp, #-68]
	mov	r3, #0
	str	r3, [fp, #-100]
.L2573:
	ldr	r2, .L2589+4
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-88]
	ldr	r1, .L2589+8
	strb	r0, [r3, #1]!
	mov	r0, r4
	str	r3, [fp, #-88]
	bl	mvc_ue_v
	ldr	r3, [fp, #-92]
	adds	r5, r0, #1
	str	r0, [r3, #4]!
	str	r3, [fp, #-92]
	beq	.L2563
	ldr	r10, [fp, #-60]
	mov	r7, #0
	ldr	r6, [fp, #-48]
.L2560:
	ldr	r1, .L2589+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r7, r7, #1
	cmp	r5, r7
	str	r0, [r10, #4]!
	bne	.L2560
	str	r6, [fp, #-48]
.L2563:
	ldr	r1, .L2589+16
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-96]
	uxtb	r0, r0
	strb	r0, [r3, #1]!
	cmp	r0, #0
	str	r3, [fp, #-96]
	bne	.L2587
.L2562:
	ldr	r1, .L2589+20
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-52]
	uxtb	r0, r0
	strb	r0, [r3]
	cmp	r0, #0
	beq	.L2564
	ldr	r1, .L2589+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r5, [fp, #-48]
	ldr	r2, .L2589+28
	mov	r1, #4
	str	r0, [r5]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34132
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2589+32
	mov	r1, #4
	strb	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34133
	movt	r3, 4
	add	r3, r8, r3
	strb	r0, [r3]
	ldr	r3, [r5]
	cmp	r3, #31
	bhi	.L2566
	ldrb	r5, [r4, #10]	@ zero_extendqisi2
	cmp	r5, #0
	bne	.L2566
	ldr	r6, [fp, #-48]
	movw	r10, #34099
	movt	r10, 4
	add	r10, r8, r10
	mov	r7, r6
.L2567:
	ldr	r1, .L2589+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2589+40
	add	r5, r5, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2589+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r10, #1]!
	ldr	r3, [r6]
	cmp	r3, r5
	bcs	.L2567
	ldr	r2, .L2589+48
	mov	r1, #5
	mov	r0, r4
	str	r6, [fp, #-48]
	bl	mvc_u_v
	movw	r3, #34396
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2589+52
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	add	r3, r8, #294912
	ldr	r2, .L2589+56
	mov	r1, #5
	str	r0, [r3, #1632]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34404
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2589+60
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34408
	movt	r3, 4
	add	r3, r8, r3
	str	r0, [r3]
.L2564:
	ldr	r1, .L2589+64
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-56]
	uxtb	r0, r0
	strb	r0, [r3]
	cmp	r0, #0
	bne	.L2588
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2572
.L2571:
	ldr	r1, .L2589+68
	mov	r0, r4
	bl	mvc_u_1
	ldrd	r2, [fp, #-68]
	ldrd	r6, [fp, #-116]
	add	r8, r8, #312
	add	r9, r9, #312
	cmp	r7, r3
	ldr	r3, [fp, #-100]
	cmpeq	r6, r2
	add	r1, r3, #1
	ldr	r3, [fp, #-72]
	str	r1, [fp, #-100]
	add	r3, r3, #1
	str	r3, [fp, #-72]
	movhi	r3, #1
	movls	r3, #0
	cmp	r1, #63
	movhi	r1, #0
	andls	r1, r3, #1
	ldrd	r2, [fp, #-68]
	adds	r6, r2, #1
	adc	r7, r3, #0
	ldr	r3, [fp, #-52]
	cmp	r1, #0
	strd	r6, [fp, #-68]
	add	r3, r3, #1
	str	r3, [fp, #-52]
	ldr	r3, [fp, #-56]
	add	r3, r3, #1
	str	r3, [fp, #-56]
	ldr	r3, [fp, #-48]
	add	r3, r3, #312
	str	r3, [fp, #-48]
	ldr	r3, [fp, #-76]
	add	r3, r3, #4
	str	r3, [fp, #-76]
	ldr	r3, [fp, #-80]
	add	r3, r3, #4
	str	r3, [fp, #-80]
	ldr	r3, [fp, #-84]
	add	r3, r3, #1
	str	r3, [fp, #-84]
	ldr	r3, [fp, #-60]
	add	r3, r3, #256
	str	r3, [fp, #-60]
	ldr	r3, [fp, #-104]
	strb	r0, [r3, #1]!
	str	r3, [fp, #-104]
	bne	.L2573
.L2574:
	mov	r0, #0
.L2584:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2588:
	ldr	r1, .L2589+24
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r8, #315392
	ldr	r2, .L2589+28
	mov	r1, #4
	mov	r5, r3
	str	r3, [fp, #-108]
	str	r0, [r9]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2589+32
	mov	r1, #4
	strb	r0, [r5, #852]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #54101
	movt	r3, 4
	add	r3, r8, r3
	strb	r0, [r3]
	ldr	r3, [r9]
	cmp	r3, #31
	bhi	.L2566
	ldrb	r5, [r4, #10]	@ zero_extendqisi2
	cmp	r5, #0
	bne	.L2566
	movw	r10, #54067
	ldr	r6, [fp, #-48]
	movt	r10, 4
	add	r10, r8, r10
	mov	r7, r9
.L2569:
	ldr	r1, .L2589+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2589+40
	add	r5, r5, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2589+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r10, #1]!
	ldr	r3, [r9]
	cmp	r3, r5
	bcs	.L2569
	ldr	r2, .L2589+48
	mov	r1, #5
	mov	r0, r4
	str	r6, [fp, #-48]
	bl	mvc_u_v
	movw	r3, #54364
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2589+52
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-108]
	ldr	r2, .L2589+56
	mov	r1, #5
	str	r0, [r3, #1120]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #54372
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2589+60
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #54376
	movt	r3, 4
	add	r3, r8, r3
	str	r0, [r3]
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2572
	ldr	r3, [fp, #-56]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2571
.L2572:
	ldr	r1, .L2589+72
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-72]
	strb	r0, [r3]
	b	.L2571
.L2587:
	ldr	r2, .L2589+76
	mov	r1, #32
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-76]
	ldr	r2, .L2589+80
	mov	r1, #32
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-80]
	ldr	r1, .L2589+84
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-84]
	strb	r0, [r3]
	b	.L2562
.L2566:
	ldr	r1, .L2589+88
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2586:
	mov	r2, r0
	ldr	r1, .L2589+92
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2584
.L2590:
	.align	2
.L2589:
	.word	.LC299
	.word	.LC301
	.word	.LC302
	.word	.LC304
	.word	.LC303
	.word	.LC308
	.word	.LC309
	.word	.LC310
	.word	.LC311
	.word	.LC312
	.word	.LC313
	.word	.LC314
	.word	.LC315
	.word	.LC316
	.word	.LC317
	.word	.LC318
	.word	.LC319
	.word	.LC321
	.word	.LC320
	.word	.LC305
	.word	.LC306
	.word	.LC307
	.word	.LC196
	.word	.LC300
	UNWIND(.fnend)
	.size	MVC_ProcessSUBSPSMvcVuiExt, .-MVC_ProcessSUBSPSMvcVuiExt
	.align	2
	.global	MVC_DecSubSPS
	.type	MVC_DecSubSPS, %function
MVC_DecSubSPS:
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
	ldr	r2, .L2619
	mov	r1, #8
	mov	r4, r0
	bl	mvc_u_v
	ldr	r1, .L2619+4
	mov	r7, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2619+8
	mov	r10, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2619+12
	str	r0, [fp, #-48]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2619+16
	str	r0, [fp, #-52]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2619+20
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2619+24
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r2, .L2619+28
	mov	r1, #2
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2619+32
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2619+36
	mov	r8, r0
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r5, r0
	bne	.L2604
	cmp	r0, #31
	bhi	.L2614
	sub	r3, r7, #66
	cmp	r3, #78
	ldrls	pc, [pc, r3, asl #2]
	b	.L2594
.L2596:
	.word	.L2595
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2597
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2598
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2597
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2599
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2597
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2599
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2597
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2594
	.word	.L2599
.L2595:
	ldr	r1, .L2619+40
	mov	r0, #1
	bl	dprint_vfmw
.L2597:
	cmp	r8, #41
	bhi	.L2615
.L2600:
	movw	r3, #8500
	movw	r2, #26248
	movt	r3, 5
	movt	r2, 4
	mul	r3, r3, r5
	mov	r0, r4
	add	r9, r4, r3
	add	r6, r9, #286720
	add	r2, r9, r2
	str	r3, [fp, #-68]
	add	r1, r6, #932
	ldrb	r3, [fp, #-48]	@ zero_extendqisi2
	str	r8, [r6, #1672]
	str	r7, [r6, #1668]
	strb	r3, [r6, #933]
	ldrb	r3, [fp, #-52]	@ zero_extendqisi2
	strb	r10, [r6, #932]
	strb	r3, [r6, #934]
	ldrb	r3, [fp, #-56]	@ zero_extendqisi2
	strb	r3, [r6, #935]
	ldrb	r3, [fp, #-60]	@ zero_extendqisi2
	strb	r3, [r6, #936]
	ldrb	r3, [fp, #-64]	@ zero_extendqisi2
	strb	r3, [r6, #937]
	str	r5, [r2, #4]
	bl	MVC_ProcessSPS
	ldr	r3, [fp, #-68]
	subs	r8, r0, #0
	bne	.L2616
	cmp	r7, #118
	cmpne	r7, #128
	mov	r10, #1
	strb	r10, [r6, #957]
	beq	.L2617
.L2602:
	movw	r3, #8500
	mov	r2, #1
	movt	r3, 5
	mla	r4, r3, r5, r4
	add	r4, r4, #12992
	strb	r2, [r4, #12]
.L2592:
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2615:
	mov	r2, r8
	ldr	r1, .L2619+44
	mov	r0, #1
	mov	r8, #41
	bl	dprint_vfmw
	b	.L2600
.L2617:
	ldr	r1, .L2619+48
	mov	r0, r4
	str	r3, [fp, #-48]
	bl	mvc_u_1
	ldr	r3, [fp, #-48]
	add	r9, r9, #12992
	add	r9, r9, #8
	add	r3, r4, r3
	add	r7, r3, #12992
	add	r7, r7, #12
	mov	r1, r7
	strb	r0, [r9, #5]
	mov	r0, r4
	bl	MVC_ProcessSUBSPSMvcExt
	cmp	r0, #0
	bne	.L2618
	ldr	r1, .L2619+52
	mov	r0, r4
	bl	mvc_u_1
	cmp	r0, #0
	str	r0, [r6, #928]
	beq	.L2602
	mov	r1, r7
	mov	r0, r4
	bl	MVC_ProcessSUBSPSMvcVuiExt
	cmp	r0, #0
	beq	.L2602
	mov	r0, r10
	ldr	r1, .L2619+56
	bl	dprint_vfmw
	strb	r8, [r9, #4]
	mvn	r8, #0
	b	.L2592
.L2599:
	ldr	r1, .L2619+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2597
.L2598:
	ldr	r1, .L2619+64
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2597
.L2594:
	mov	r2, r7
	ldr	r1, .L2619+68
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2604:
	mvn	r8, #0
	b	.L2592
.L2614:
	ldr	r1, .L2619+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L2592
.L2616:
	ldr	r1, .L2619+76
	mov	r0, #1
	bl	dprint_vfmw
	add	ip, r9, #12992
	mov	r3, #0
	mvn	r8, #0
	strb	r3, [ip, #12]
	strb	r3, [r6, #957]
	b	.L2592
.L2618:
	mov	r0, r10
	ldr	r1, .L2619+80
	bl	dprint_vfmw
	strb	r8, [r9, #4]
	mvn	r8, #0
	b	.L2592
.L2620:
	.align	2
.L2619:
	.word	.LC322
	.word	.LC323
	.word	.LC324
	.word	.LC325
	.word	.LC326
	.word	.LC327
	.word	.LC328
	.word	.LC329
	.word	.LC330
	.word	.LC331
	.word	.LC267
	.word	.LC333
	.word	.LC335
	.word	.LC337
	.word	.LC338
	.word	.LC269
	.word	.LC268
	.word	.LC332
	.word	.LC170
	.word	.LC334
	.word	.LC336
	UNWIND(.fnend)
	.size	MVC_DecSubSPS, .-MVC_DecSubSPS
	.align	2
	.global	MVC_PassBytes
	.type	MVC_PassBytes, %function
MVC_PassBytes:
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
	ldr	ip, [r0, #232]
	cmp	ip, #0
	cmpne	r1, #0
	beq	.L2630
	ldrb	r3, [ip]	@ zero_extendqisi2
	mov	r2, r3, asl #2
	mov	r5, r3, asl #5
	rsb	r4, r2, r5
	add	r4, ip, r4
	add	r6, r4, #8
	ldr	lr, [r4, #8]
	cmp	lr, #0
	beq	.L2630
	ldr	lr, [ip, #68]
	cmp	r3, #1
	cmpls	lr, #2
	bhi	.L2630
	cmp	lr, r3
	bls	.L2630
	ldr	lr, [r4, #12]
	mov	r6, r2
	ldr	r4, [r4, #24]
	mov	lr, lr, asl #3
	add	r1, r4, r1, lsl #3
	cmp	r1, lr
	bhi	.L2625
	b	.L2623
.L2626:
	ldr	r4, [r2, #24]
	ldr	r2, [r2, #12]
	rsb	lr, lr, r4
	add	r1, r1, lr
	mov	lr, r2, asl #3
	cmp	r1, lr
	bls	.L2623
.L2625:
	rsb	r2, r6, r5
	add	r3, r3, #1
	add	ip, ip, r2
	mov	r6, r3, asl #2
	mov	r5, r3, asl #5
	str	lr, [ip, #24]
	rsb	r2, r6, r5
	ldr	ip, [r0, #232]
	ldrb	r4, [ip]	@ zero_extendqisi2
	add	r4, r4, #1
	strb	r4, [ip]
	ldr	ip, [r0, #232]
	add	r2, ip, r2
	ldr	r4, [ip, #68]
	cmp	r4, r3
	bhi	.L2626
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2623:
	rsb	r2, r6, r5
	mov	r0, #1
	add	ip, ip, r2
	str	r1, [ip, #24]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2630:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_PassBytes, .-MVC_PassBytes
	.align	2
	.global	MVC_GetBytes
	.type	MVC_GetBytes, %function
MVC_GetBytes:
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
	beq	.L2646
	ldr	r3, [r0, #232]
	cmp	r3, #0
	cmpne	r2, #0
	beq	.L2646
	ldrb	r7, [r3]	@ zero_extendqisi2
	mov	ip, r7, asl #5
	sub	ip, ip, r7, asl #2
	add	ip, r3, ip
	ldr	ip, [ip, #8]
	cmp	ip, #0
	beq	.L2646
	ldr	ip, [r3, #68]
	cmp	r7, #1
	cmpls	ip, #2
	movhi	lr, #1
	movls	lr, #0
	bhi	.L2646
	cmp	ip, r7
	bls	.L2646
.L2641:
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
	bcs	.L2636
	ldrb	r3, [r8, r4, lsr #3]	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	r2, lr
	add	r4, r1, #1
	add	ip, ip, #1
	strb	r3, [r1]
	b	.L2648
.L2639:
	cmp	ip, r6
	mov	r1, r4
	beq	.L2636
	ldrb	r3, [ip], #1	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	r2, lr
	strb	r3, [r4], #1
.L2648:
	ldr	r3, [r0, #232]
	add	r3, r3, r5
	ldr	r1, [r3, #24]
	add	r1, r1, #8
	str	r1, [r3, #24]
	bhi	.L2639
	mov	r0, r2
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2636:
	ldr	ip, [r0, #232]
	add	r7, r7, #1
	ldrb	r3, [ip]	@ zero_extendqisi2
	add	r3, r3, #1
	strb	r3, [ip]
	ldr	r3, [r0, #232]
	ldr	ip, [r3, #68]
	cmp	ip, r7
	bhi	.L2641
	mov	r0, lr
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2646:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_GetBytes, .-MVC_GetBytes
	.align	2
	.global	MVC_DecFramePackingSEI
	.type	MVC_DecFramePackingSEI, %function
MVC_DecFramePackingSEI:
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
	ldr	r1, .L2656
	mov	r4, r0
	bl	mvc_ue_v
	add	r6, r4, #11075584
	add	r5, r6, #45056
	ldr	r2, .L2656+4
	mov	r1, #1
	str	r0, [r5, #2640]
	mov	r0, r4
	bl	mvc_u_v
	uxtb	r0, r0
	strb	r0, [r5, #2617]
	cmp	r0, #0
	beq	.L2655
.L2650:
	mov	r0, r4
	ldr	r2, .L2656+8
	mov	r1, #1
	add	r6, r6, #45056
	bl	mvc_u_v
	mov	r3, #1
	strb	r3, [r6, #2616]
	strb	r0, [r6, #2618]
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2655:
	ldr	r2, .L2656+12
	mov	r1, #7
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+16
	mov	r1, #1
	str	r0, [r5, #2632]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+20
	mov	r1, #6
	strb	r0, [r5, #2619]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+24
	mov	r1, #1
	str	r0, [r5, #2636]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+28
	mov	r1, #1
	strb	r0, [r5, #2620]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+32
	mov	r1, #1
	strb	r0, [r5, #2621]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+36
	mov	r1, #1
	strb	r0, [r5, #2622]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+40
	mov	r1, #1
	strb	r0, [r5, #2623]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+44
	mov	r1, #1
	strb	r0, [r5, #2624]
	mov	r0, r4
	bl	mvc_u_v
	ldrb	r3, [r5, #2619]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r5, #2625]
	bne	.L2651
	ldr	r3, [r5, #2632]
	cmp	r3, #5
	beq	.L2651
	ldr	r2, .L2656+48
	mov	r1, #1
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+52
	mov	r1, #1
	strb	r0, [r5, #2626]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+56
	mov	r1, #1
	strb	r0, [r5, #2627]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2656+60
	mov	r1, #1
	strb	r0, [r5, #2628]
	mov	r0, r4
	bl	mvc_u_v
	strb	r0, [r5, #2629]
.L2651:
	ldr	r2, .L2656+64
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2656+68
	str	r0, [r5, #2644]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #2648]
	b	.L2650
.L2657:
	.align	2
.L2656:
	.word	.LC339
	.word	.LC340
	.word	.LC356
	.word	.LC341
	.word	.LC342
	.word	.LC343
	.word	.LC344
	.word	.LC345
	.word	.LC346
	.word	.LC347
	.word	.LC348
	.word	.LC349
	.word	.LC350
	.word	.LC351
	.word	.LC352
	.word	.LC353
	.word	.LC354
	.word	.LC355
	UNWIND(.fnend)
	.size	MVC_DecFramePackingSEI, .-MVC_DecFramePackingSEI
	.align	2
	.global	MVC_DecPicTimingSEI
	.type	MVC_DecPicTimingSEI, %function
MVC_DecPicTimingSEI:
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
	add	r6, r0, #11075584
	ldr	r3, [r0, #248]
	add	r2, r6, #32768
	movw	r4, #3992
	mov	r5, r0
	ldr	r2, [r2, #2568]
	mla	r4, r4, r2, r3
	ldrb	r2, [r4, #25]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2659
	ldr	ip, [r0, #36]
	cmp	ip, #0
	ble	.L2660
	ldrb	r2, [r3, #25]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #3984
	addeq	r3, r3, #8
	beq	.L2663
	b	.L2682
.L2664:
	ldrb	r1, [r3, #-3967]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2661
.L2663:
	add	r2, r2, #1
	mov	r4, r3
	cmp	r2, ip
	add	r3, r3, #3984
	add	r3, r3, #8
	bne	.L2664
.L2660:
	ldr	r1, .L2684
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
.L2665:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2682:
	mov	r4, r3
.L2661:
	cmp	r4, #0
	beq	.L2660
.L2659:
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2666
	ldrb	r3, [r4, #35]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2683
	ldr	r1, [r4, #388]
	ldr	r8, [r4, #392]
	add	r1, r1, #1
	add	r8, r8, #1
.L2669:
	ldr	r2, .L2684+4
	mov	r0, r5
	bl	mvc_u_v
	add	r7, r6, #45056
	mov	r1, r8
	ldr	r2, .L2684+8
	str	r0, [r7, #2656]
	mov	r0, r5
	bl	mvc_u_v
	str	r0, [r7, #2660]
.L2666:
	ldrb	r0, [r4, #39]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	r3, r0
	beq	.L2665
	mov	r0, r5
	ldr	r2, .L2684+12
	mov	r1, #4
	add	r6, r6, #45056
	bl	mvc_u_v
	mov	r3, #0
	strb	r0, [r6, #2652]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2683:
	ldrb	r3, [r4, #38]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2666
	ldr	r1, [r4, #700]
	ldr	r8, [r4, #704]
	add	r1, r1, #1
	add	r8, r8, #1
	b	.L2669
.L2685:
	.align	2
.L2684:
	.word	.LC357
	.word	.LC358
	.word	.LC359
	.word	.LC360
	UNWIND(.fnend)
	.size	MVC_DecPicTimingSEI, .-MVC_DecPicTimingSEI
	.align	2
	.global	MVC_DecSEI
	.type	MVC_DecSEI, %function
MVC_DecSEI:
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
	ldr	r2, [r0, #232]
	sub	r7, fp, #44
	mov	r3, #0
	mov	r1, #32
	mov	r4, r0
	add	r6, r0, #548
	mov	r8, r3
	strb	r3, [r7, #-1]!
	str	r1, [r2, #24]
	b	.L2689
.L2871:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2688
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2870
.L2689:
	mov	r2, #1
	mov	r1, r7
	mov	r0, r4
	bl	MVC_GetBytes
	ldr	r3, [r4, #572]
	ldr	ip, [r4, #564]
	mov	r1, #8
	add	r3, r3, r1
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, ip, asl #3
	add	r8, r8, r2
	mov	r5, r0
	mov	r0, r6
	bls	.L2871
.L2688:
	ldr	r1, .L2902
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2691
.L2870:
	mov	r3, #0
	str	r3, [fp, #-56]
.L2825:
	mov	r5, #0
	b	.L2695
.L2873:
	bl	BsSkip
	cmp	r9, #0
	ble	.L2693
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2872
.L2695:
	mov	r2, #1
	mov	r1, r7
	mov	r0, r4
	bl	MVC_GetBytes
	ldr	r3, [r4, #572]
	ldr	ip, [r4, #564]
	mov	r1, #8
	add	r3, r3, r1
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, ip, asl #3
	add	r5, r5, r2
	mov	r9, r0
	mov	r0, r6
	bls	.L2873
.L2693:
	ldr	r1, .L2902+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2872:
	cmp	r5, #409600
	bgt	.L2693
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #68]
	cmp	r2, #2
	ldreq	r2, [r3, #52]
	ldreq	r3, [r3, #24]
	ldrne	r2, [r3, #24]
	addeq	r2, r2, r3
	moveq	r2, r2, lsr #3
	cmp	r8, #45
	ldrls	pc, [pc, r8, asl #2]
	b	.L2698
.L2700:
	.word	.L2699
	.word	.L2701
	.word	.L2702
	.word	.L2703
	.word	.L2704
	.word	.L2705
	.word	.L2706
	.word	.L2707
	.word	.L2708
	.word	.L2709
	.word	.L2710
	.word	.L2711
	.word	.L2712
	.word	.L2713
	.word	.L2714
	.word	.L2715
	.word	.L2716
	.word	.L2717
	.word	.L2718
	.word	.L2719
	.word	.L2720
	.word	.L2721
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2698
	.word	.L2722
.L2722:
	mov	r1, r5
	mov	r0, r4
	ldr	r8, [r4, #572]
	bl	MVC_DecFramePackingSEI
	ldr	r2, [r4, #572]
	mov	r9, r5, asl #3
	rsb	r3, r8, r2
	cmp	r3, r9
	bge	.L2811
	rsb	r9, r3, r9
	ldr	r3, [r4, #564]
	cmp	r9, #0
	add	r8, r9, #7
	movge	r8, r9
	mov	r8, r8, asr #3
	cmp	r8, #0
	movle	r3, r3, asl #3
	ble	.L2813
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bhi	.L2813
	mov	r10, #0
	b	.L2814
.L2815:
	add	r1, r2, #8
	cmp	r1, r3
	bhi	.L2813
.L2814:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #564]
	cmp	r10, r8
	ldr	r2, [r4, #572]
	mov	r3, r3, asl #3
	bne	.L2815
.L2813:
	mov	r1, r9, asr #31
	mov	r0, r1, lsr #29
	add	r1, r9, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r2, r1
	cmp	r3, r2
	bcs	.L2874
.L2811:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2875
.L2724:
	ldr	r3, [r4, #60]
	cmp	r3, #3
	bhi	.L2876
.L2820:
	mov	r8, #0
	b	.L2824
.L2822:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2823
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2877
.L2824:
	mov	r2, #1
	mov	r1, r7
	mov	r0, r4
	bl	MVC_GetBytes
	ldr	r3, [r4, #572]
	ldr	ip, [r4, #564]
	mov	r1, #8
	add	r3, r3, r1
	ldrb	r2, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, ip, asl #3
	add	r8, r8, r2
	mov	r5, r0
	mov	r0, r6
	bls	.L2822
.L2823:
	mov	r0, #0
.L2691:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2721:
	mov	r3, r5
	ldr	r1, .L2902+8
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2878
.L2807:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2808
.L2809:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2808:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2809
	ldr	r3, [r4, #60]
	cmp	r3, #3
	bls	.L2820
.L2876:
	ldr	r1, .L2902+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2720:
	mov	r3, r5
	ldr	r1, .L2902+16
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2879
.L2804:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2805
.L2806:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2805:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2806
	b	.L2724
.L2719:
	mov	r3, r5
	ldr	r1, .L2902+20
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2880
.L2801:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2802
.L2803:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2802:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2803
	b	.L2724
.L2717:
	mov	r3, r5
	ldr	r1, .L2902+24
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2881
.L2795:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2796
.L2797:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2796:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2797
	b	.L2724
.L2718:
	mov	r3, r5
	ldr	r1, .L2902+28
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2882
.L2798:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2799
.L2800:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2799:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2800
	b	.L2724
.L2716:
	mov	r3, r5
	ldr	r1, .L2902+32
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2883
.L2792:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2793
.L2794:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2793:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2794
	b	.L2724
.L2715:
	mov	r3, r5
	ldr	r1, .L2902+36
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2884
.L2789:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2790
.L2791:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2790:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2791
	b	.L2724
.L2707:
	mov	r3, r5
	ldr	r1, .L2902+40
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2885
.L2765:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2766
.L2767:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2766:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2767
	b	.L2724
.L2711:
	mov	r3, r5
	ldr	r1, .L2902+44
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2886
.L2777:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2778
.L2779:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2778:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2779
	b	.L2724
.L2702:
	mov	r3, r5
	ldr	r1, .L2902+48
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2887
.L2734:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2735
.L2736:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2735:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2736
	b	.L2724
.L2713:
	mov	r3, r5
	ldr	r1, .L2902+52
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2888
.L2783:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2784
.L2785:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2784:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2785
	b	.L2724
.L2705:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	bne	.L2704
	mov	r3, r5
	ldr	r1, .L2902+56
	mov	r0, #20
	bl	dprint_vfmw
.L2741:
	ldr	r0, [r4, #120]
	ldr	r8, [r4, #60]
	bl	GetUsd
	add	r8, r8, #132
	str	r0, [r4, r8, asl #2]
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	r1, [r4, r3, asl #2]
	cmp	r1, #0
	beq	.L2828
	ldr	r2, [r4, #232]
	cmp	r5, #1024
	movlt	r10, r5
	movge	r10, #1024
	ldrb	r0, [r2]	@ zero_extendqisi2
	ldr	ip, [r2, #68]
	cmp	r0, ip
	bcs	.L2829
	sxth	r3, r0
	mov	r8, #0
	mov	lr, r3, asl #5
	sub	r3, lr, r3, asl #2
	add	r2, r2, r3
.L2743:
	ldr	r3, [r2, #24]
	add	r0, r0, #1
	ldr	lr, [r2, #12]
	cmp	r0, ip
	add	r3, r3, #7
	add	r2, r2, #28
	sub	r3, lr, r3, lsr #3
	add	r8, r8, r3
	bne	.L2743
	mov	r2, r8
.L2742:
	ldrsb	r3, [fp, #-56]
	cmp	r2, r10
	movge	r8, r10
	cmp	r3, #1
	movne	r3, #0
	movne	r9, r3
	beq	.L2889
.L2745:
	rsb	r2, r3, r8
	mov	r0, r4
	bl	MVC_GetBytes
	subs	r8, r0, #0
	ble	.L2890
	rsb	r3, r9, r10
	cmp	r8, r3
	ldr	r3, [r4, #60]
	blt	.L2891
	add	r3, r3, #132
	cmp	r10, r5
	rsb	r9, r9, r5
	ldr	r3, [r4, r3, asl #2]
	str	r9, [r3, #1052]
	bcc	.L2892
.L2756:
	cmp	r9, #0
	ble	.L2751
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2751
	mov	r5, #0
	b	.L2758
.L2759:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2751
.L2758:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2759
.L2751:
	ldr	r3, [r4, #60]
	mov	r2, #8
	ldr	r1, [r4, #224]
	add	r3, r3, #132
	ldr	ip, .L2902+60
	ldr	r3, [r4, r3, asl #2]
	ldrd	r0, [r1, #56]
	add	r3, r3, #1056
	ldr	ip, [ip]
	strd	r0, [r3]
	cmp	ip, #0
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	r3, [r4, r3, asl #2]
	str	r2, [r3, #1040]
	ldr	r3, [r4, #60]
	ldr	r2, [r4, #84]
	add	r3, r3, #132
	ldr	r3, [r4, r3, asl #2]
	str	r2, [r3, #1044]
	beq	.L2724
	ldr	r2, [r4, #60]
	movw	r3, #1064
	mov	r1, #6
	ldr	r0, [r4, #120]
	add	r2, r2, #132
	ldr	r2, [r4, r2, asl #2]
	blx	ip
	b	.L2724
.L2709:
	mov	r3, r5
	ldr	r1, .L2902+64
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2893
.L2771:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2772
.L2773:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2772:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2773
	b	.L2724
.L2708:
	mov	r3, r5
	ldr	r1, .L2902+68
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2894
.L2768:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2769
.L2770:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2769:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2770
	b	.L2724
.L2710:
	mov	r3, r5
	ldr	r1, .L2902+72
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2895
.L2774:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2775
.L2776:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2775:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2776
	b	.L2724
.L2699:
	mov	r3, r5
	ldr	r1, .L2902+76
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2896
.L2723:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2725
.L2726:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2725:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2726
	b	.L2724
.L2714:
	mov	r3, r5
	ldr	r1, .L2902+80
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2897
.L2786:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2787
.L2788:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2787:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2788
	b	.L2724
.L2706:
	mov	r3, r5
	ldr	r1, .L2902+84
	mov	r0, #20
	bl	dprint_vfmw
	cmp	r5, #0
	ble	.L2762
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2762
	mov	r8, #0
	b	.L2763
.L2764:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2762
.L2763:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2764
.L2762:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2724
	mov	r3, #12416
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2724
.L2712:
	mov	r3, r5
	ldr	r1, .L2902+88
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2898
.L2780:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2781
.L2782:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2781:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2782
	b	.L2724
.L2701:
	mov	r3, r5
	ldr	r1, .L2902+92
	mov	r0, #20
	mov	r9, r5, asl #3
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	ldr	r8, [r4, #572]
	bl	MVC_DecPicTimingSEI
	ldr	r2, [r4, #572]
	rsb	r3, r8, r2
	cmp	r3, r9
	bge	.L2728
	rsb	r9, r3, r9
	ldr	r3, [r4, #564]
	cmp	r9, #0
	add	r8, r9, #7
	movge	r8, r9
	mov	r8, r8, asr #3
	cmp	r8, #0
	movle	r3, r3, asl #3
	ble	.L2730
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bhi	.L2730
	mov	r10, #0
	b	.L2731
.L2732:
	add	r1, r2, #8
	cmp	r1, r3
	bhi	.L2730
.L2731:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #564]
	cmp	r10, r8
	ldr	r2, [r4, #572]
	mov	r3, r3, asl #3
	bne	.L2732
.L2730:
	mov	r1, r9, asr #31
	mov	r0, r1, lsr #29
	add	r1, r9, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r2, r1
	cmp	r3, r2
	bcs	.L2899
.L2728:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2724
	movw	r3, #12190
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2724
.L2703:
	mov	r3, r5
	ldr	r1, .L2902+96
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2900
.L2737:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2738
.L2739:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2738:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2739
	b	.L2724
.L2698:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2901
.L2817:
	cmp	r5, #0
	ble	.L2724
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
	mov	r8, #0
	b	.L2818
.L2819:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2724
.L2818:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2819
	b	.L2724
.L2877:
	cmp	r3, #128
	bne	.L2825
	b	.L2823
.L2704:
	mov	r3, r5
	ldr	r1, .L2902+100
	mov	r0, #20
	bl	dprint_vfmw
	mov	r3, #1
	str	r3, [fp, #-56]
	b	.L2741
.L2891:
	add	r3, r3, #132
	ldr	r3, [r4, r3, asl #2]
	str	r8, [r3, #1052]
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2751
	mov	r5, #0
	b	.L2752
.L2754:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2751
.L2752:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2754
	b	.L2751
.L2903:
	.align	2
.L2902:
	.word	.LC361
	.word	.LC362
	.word	.LC389
	.word	.LC390
	.word	.LC388
	.word	.LC387
	.word	.LC385
	.word	.LC386
	.word	.LC384
	.word	.LC383
	.word	.LC375
	.word	.LC379
	.word	.LC366
	.word	.LC381
	.word	.LC369
	.word	g_event_report
	.word	.LC377
	.word	.LC376
	.word	.LC378
	.word	.LC363
	.word	.LC382
	.word	.LC374
	.word	.LC380
	.word	.LC365
	.word	.LC367
	.word	.LC368
	.word	.LC370
	.word	.LC372
	.word	.LANCHOR0+236
	.word	.LC364
	.word	.LC371
	.word	.LC373
.L2889:
	strb	r3, [r1, #1026]
	mov	r0, r4
	ldr	r3, [r4, #60]
	mov	r1, #8
	ldr	r2, .L2902+104
	add	r3, r3, #132
	ldr	r9, [r4, r3, asl #2]
	bl	mvc_u_v
	strb	r0, [r9, #1027]
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	ip, [r4, r3, asl #2]
	ldrb	r3, [ip, #1027]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L2746
	mov	r3, #3
	mov	r2, #0
	mov	r9, r3
	strb	r2, [ip, #1028]
.L2747:
	ldr	ip, [r4, #60]
	mov	r1, #16
	ldr	r2, .L2902+108
	mov	r0, r4
	add	ip, ip, #132
	str	r3, [fp, #-60]
	ldr	ip, [r4, ip, asl #2]
	str	ip, [fp, #-64]
	bl	mvc_u_v
	ldr	ip, [fp, #-64]
	ldr	r3, [fp, #-60]
	add	ip, ip, #1024
	mov	r1, r3
	strh	r0, [ip, #6]	@ movhi
	mov	r0, r4
	bl	MVC_PassBytes
	ldr	r3, [fp, #-60]
	cmp	r8, r3
	beq	.L2751
	ldr	r2, [r4, #60]
	add	r2, r2, #132
	ldr	r1, [r4, r2, asl #2]
	b	.L2745
.L2884:
	movw	r3, #12636
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2789
.L2885:
	movw	r3, #12428
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2765
.L2886:
	movw	r3, #12532
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2777
.L2887:
	movw	r3, #12202
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2734
.L2888:
	movw	r3, #12584
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2783
.L2880:
	movw	r3, #12740
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2801
.L2881:
	movw	r3, #12688
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2795
.L2882:
	movw	r3, #12714
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2798
.L2883:
	movw	r3, #12662
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2792
.L2878:
	movw	r3, #12792
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2807
.L2879:
	movw	r3, #12766
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2804
.L2894:
	movw	r3, #12454
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2768
.L2895:
	movw	r3, #12506
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2774
.L2896:
	movw	r3, #12137
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2723
.L2897:
	movw	r3, #12610
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2786
.L2893:
	mov	r3, #12480
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2771
.L2898:
	movw	r3, #12558
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2780
.L2900:
	movw	r3, #12228
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2737
.L2875:
	movw	r3, #12844
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2724
.L2901:
	movw	r3, #12856
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2817
.L2899:
	mov	r0, r6
	bl	BsSkip
	b	.L2728
.L2874:
	mov	r0, r6
	bl	BsSkip
	b	.L2811
.L2892:
	rsb	r1, r10, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2756
	movw	r3, #12364
	ldr	r2, .L2902+112
	ldr	r1, .L2902+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2756
.L2829:
	mov	r2, #0
	mov	r8, r2
	b	.L2742
.L2746:
	ldr	r2, .L2902+120
	mov	r1, #8
	mov	r0, r4
	mov	r3, #4
	str	ip, [fp, #-64]
	str	r3, [fp, #-60]
	bl	mvc_u_v
	ldr	r3, [fp, #-60]
	ldr	ip, [fp, #-64]
	mov	r9, r3
	strb	r0, [ip, #1028]
	b	.L2747
.L2828:
	mvn	r0, #0
	b	.L2691
.L2890:
	ldr	r1, .L2902+124
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #60]
	ldr	r0, [r4, #120]
	add	r3, r3, #132
	ldr	r1, [r4, r3, asl #2]
	bl	FreeUsdByDec
	ldr	r3, [r4, #60]
	mov	r2, #0
	mvn	r0, #0
	add	r3, r3, #132
	str	r2, [r4, r3, asl #2]
	b	.L2691
	UNWIND(.fnend)
	.size	MVC_DecSEI, .-MVC_DecSEI
	.align	2
	.global	MVC_InitOldSlice
	.type	MVC_InitOldSlice, %function
MVC_InitOldSlice:
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
	ldr	r1, [r0, #40]
	add	r3, r0, #11075584
	add	r3, r3, #40960
	movw	r2, #23352
	movt	r2, 1
	mov	r0, #2
	str	r1, [r3, #2668]
	mov	r1, #7
	str	r2, [r3, #2672]
	mov	r2, #32
	strb	r1, [r3, #2659]
	mov	r1, #3
	strb	r2, [r3, #2658]
	mvn	r2, #0
	str	r1, [r3, #2680]
	mov	r1, #262144
	str	r2, [r3, #2684]
	mov	r2, #0
	strb	r0, [r3, #2657]
	str	r1, [r3, #2696]
	strb	r2, [r3, #2656]
	str	r2, [r3, #2688]
	str	r2, [r3, #2692]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_InitOldSlice, .-MVC_InitOldSlice
	.align	2
	.global	MVC_IsNewPicNal
	.type	MVC_IsNewPicNal, %function
MVC_IsNewPicNal:
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
	b	.L2913
.L2908:
	.word	.L2907
	.word	.L2913
	.word	.L2913
	.word	.L2913
	.word	.L2907
	.word	.L2913
	.word	.L2909
	.word	.L2909
.L2909:
	ldr	r3, [r0]
	mov	r2, #1
	str	r2, [r0]
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2907:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L2910
.L2912:
	mvn	r0, #0
.L2911:
	mov	r2, #0
	str	r2, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2913:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2910:
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
	bhi	.L2912
	mov	r3, r0, asl #1
	rsb	r3, r3, #31
	mov	r3, r4, lsr r3
	subs	r0, r3, #1
	mvnne	r0, #0
	b	.L2911
	UNWIND(.fnend)
	.size	MVC_IsNewPicNal, .-MVC_IsNewPicNal
	.align	2
	.global	MVC_FindZeroBitsInSeg
	.type	MVC_FindZeroBitsInSeg, %function
MVC_FindZeroBitsInSeg:
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
	ble	.L2940
	ldr	r3, [fp, #-48]
	subs	r7, r3, #0
	add	r8, r3, #63
	movge	r8, r3
	ands	r3, r3, #63
	movne	r3, #1
	add	r8, r3, r8, asr #6
	cmp	r8, #0
	ble	.L2928
	ldr	r5, .L2941
	ldr	r10, .L2941+4
.L2926:
	cmp	r7, #64
	ldr	r3, [r10, #52]
	ldr	r0, .L2941
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
	bne	.L2918
	ldr	r2, .L2941
	add	r4, r4, #1
	cmp	r3, r2
	bne	.L2921
	b	.L2919
.L2922:
	cmp	r3, r5
	add	r4, r4, #1
	beq	.L2919
.L2921:
	ldrb	r2, [r3, #-1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2922
.L2918:
	mov	r0, r4, asl #3
.L2917:
	ldr	r3, [fp, #-48]
	cmp	r4, r3
	bge	.L2930
	sub	r3, r3, #1
	rsb	r4, r4, r3
	ldrb	r2, [r9, r4]	@ zero_extendqisi2
	tst	r2, #1
	moveq	r2, r2, lsr #1
	moveq	r3, #1
	bne	.L2930
.L2925:
	tst	r2, #1
	mov	r2, r2, lsr #1
	bne	.L2923
	add	r3, r3, #1
	cmp	r3, #8
	bne	.L2925
	b	.L2923
.L2919:
	subs	r8, r8, #1
	sub	r7, r7, #64
	bne	.L2926
	b	.L2918
.L2930:
	mov	r3, #0
.L2923:
	add	r0, r3, r0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2940:
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L2941+8
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2928:
	mov	r0, r4
	b	.L2917
.L2942:
	.align	2
.L2941:
	.word	.LANCHOR3-1952
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC391
	UNWIND(.fnend)
	.size	MVC_FindZeroBitsInSeg, .-MVC_FindZeroBitsInSeg
	.align	2
	.global	MVC_FindTrailZeros
	.type	MVC_FindTrailZeros, %function
MVC_FindTrailZeros:
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
	beq	.L2949
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2948
	ldr	r1, [r3, #40]
	ldr	r0, [r3, #36]
	bl	MVC_FindZeroBitsInSeg
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #40]
	cmp	r0, r2, asl #3
	bcs	.L2952
	cmn	r0, #1
	beq	.L2948
.L2947:
	add	r0, r0, #1
	str	r0, [r3, #72]
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2951
	ldr	r1, [r3, #40]
	ldr	r2, [r3, #72]
	cmp	r2, r1, asl #3
	bcs	.L2953
.L2951:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2953:
	ldr	r1, [r3, #60]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r2, [r4, #232]
	mov	r3, #0
	mov	r1, #1
	mov	r0, r3
	str	r3, [r2, #36]
	ldr	r2, [r4, #232]
	str	r1, [r2, #68]
	ldr	r2, [r4, #232]
	str	r3, [r2, #72]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2952:
	ldr	r1, [r3, #60]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #232]
	mov	r1, #0
	mov	r2, #1
	str	r1, [r3, #36]
	ldr	r3, [r4, #232]
	str	r2, [r3, #68]
	ldr	r3, [r4, #232]
.L2948:
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	bl	MVC_FindZeroBitsInSeg
	ldr	r3, [r4, #232]
	b	.L2947
.L2949:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_FindTrailZeros, .-MVC_FindTrailZeros
	.align	2
	.global	MVC_CombinePacket
	.type	MVC_CombinePacket, %function
MVC_CombinePacket:
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
	ldr	r3, [r0, #232]
	ldr	r3, [r3, #12]
	cmp	r3, #4096
	ldmcsfd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_CombinePacket.part.12
	UNWIND(.fnend)
	.size	MVC_CombinePacket, .-MVC_CombinePacket
	.align	2
	.global	MVC_FindNaluArraySlot
	.type	MVC_FindNaluArraySlot, %function
MVC_FindNaluArraySlot:
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
	ldrb	r2, [r0, #937]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2961
	mov	r2, r0
	mov	r3, #1
	b	.L2959
.L2958:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L2964
.L2959:
	ldrb	r1, [r2, #1025]	@ zero_extendqisi2
	add	r2, r2, #88
	cmp	r1, #0
	bne	.L2958
	mov	r2, r3
.L2957:
	mov	ip, #88
	mov	r1, #1
	mla	r3, ip, r3, r0
	mov	r0, r2
	strb	r1, [r3, #937]
	ldmfd	sp, {fp, sp, pc}
.L2964:
	mvn	r2, #0
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L2961:
	mov	r3, r2
	b	.L2957
	UNWIND(.fnend)
	.size	MVC_FindNaluArraySlot, .-MVC_FindNaluArraySlot
	.align	2
	.global	MVC_InquireSliceProperty
	.type	MVC_InquireSliceProperty, %function
MVC_InquireSliceProperty:
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
	ldr	r3, [r0, #68]
	mov	r4, r0
	mov	r9, r1
	mov	r7, r2
	ldr	r1, .L3008
	sub	r2, r3, #1
	mov	r0, #22
	add	r6, r4, #11075584
	bl	dprint_vfmw
	mov	r3, #0
	str	r3, [r7]
	add	r5, r6, #40960
	str	r3, [r9]
	add	r1, r4, #12288
	ldr	r2, [r4, #232]
	mvn	ip, #0
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	strb	r0, [r5, #1003]
	ldrb	lr, [r2, #4]	@ zero_extendqisi2
	strb	lr, [r5, #1008]
	ldrb	r2, [r2, #5]	@ zero_extendqisi2
	strb	r3, [r5, #1011]
	str	ip, [r5, #2640]
	strb	r2, [r5, #1012]
	ldrb	r3, [r1, #704]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3001
	cmp	r2, #255
	beq	.L3002
.L2967:
	mov	r0, r4
	bl	MVC_SliceCheck
	subs	r8, r0, #0
	bne	.L2983
	mov	r0, r4
	bl	MVC_ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L3003
	ldrb	r3, [r5, #1005]	@ zero_extendqisi2
	mov	ip, #2240
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r9]
	ldrb	r1, [r5, #1012]	@ zero_extendqisi2
	ldr	r3, [r5, #1016]
	ldr	r0, [r4, #252]
	sxtb	r2, r1
	cmn	r2, #1
	mla	r3, ip, r3, r0
	beq	.L3004
	cmp	r1, #0
	bne	.L2979
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	ldr	r3, [r3, #28]
	cmp	r2, #1
	beq	.L3005
	ldr	r2, [r4, #28]
	cmp	r2, r3
	movweq	r3, #35364
	movteq	r3, 168
	addeq	r3, r4, r3
	bne	.L3006
.L2978:
	ldrb	r2, [r3, #20]	@ zero_extendqisi2
	add	r6, r6, #45056
	ldr	r0, [r3, #3952]
	rsb	r1, r2, #2
	ldr	r2, [r3, #3948]
	ldr	ip, [r4, #16]
	mla	r1, r0, r1, r1
	ldr	r0, [r4, #12]
	add	r2, r2, #1
	cmp	r0, r2
	cmpeq	ip, r1
	movne	r2, #1
	strne	r2, [r7]
	ldr	r2, [r3, #3972]
	ldr	r3, [r6, #2856]
	add	r3, r3, #1
	cmp	r2, r3
	movhi	r3, #1
	strhi	r3, [r7]
.L2975:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3001:
	strb	r3, [r5, #1011]
	ldr	r3, [r1, #712]
	str	r3, [r5, #2640]
	ldrb	r3, [r1, #708]	@ zero_extendqisi2
	strb	r3, [r5, #1009]
	ldrb	r3, [r1, #709]	@ zero_extendqisi2
	strb	r3, [r5, #1010]
	b	.L2967
.L3004:
	ldr	r3, [r3, #28]
	movw	r1, #3992
	ldr	r2, [r4, #248]
	mla	r3, r1, r3, r2
	b	.L2978
.L3002:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq	.L2967
	add	r3, r4, #10747904
	add	r3, r3, #20480
	ldr	r2, [r3, #2384]
	cmp	r2, #0
	bne	.L2970
	ldrb	r2, [r3, #2380]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2970
	add	r3, r4, #12992
	add	r3, r3, #16
.L2973:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L2971
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3007
.L2971:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L2973
	mvn	r3, #0
	b	.L3000
.L3005:
	movw	r2, #8500
	movt	r2, 5
	mla	r3, r2, r3, r4
	add	r3, r3, #286720
	add	r3, r3, #932
	b	.L2978
.L2970:
	ldr	r3, [r3, #2388]
.L3000:
	cmn	r3, #1
	str	r3, [r5, #2640]
	beq	.L2967
	sub	r0, r0, #5
	mov	r3, #1
	clz	r0, r0
	strb	r3, [r5, #1011]
	strb	r3, [r5, #1010]
	mov	r0, r0, lsr #5
	strb	r0, [r5, #1009]
	b	.L2967
.L2983:
	mvn	r8, #0
	b	.L2975
.L3006:
	ldr	r1, .L3008+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L2975
.L3003:
	ldr	r1, .L3008+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L2975
.L2979:
	ldr	r1, .L3008+12
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L2975
.L3007:
	movw	r3, #8500
	movt	r3, 5
	mla	r3, r3, r2, r4
	add	r3, r3, #12992
	add	r3, r3, #16
	ldr	r3, [r3, #4]
	b	.L3000
.L3009:
	.align	2
.L3008:
	.word	.LC392
	.word	.LC30
	.word	.LC393
	.word	.LC394
	UNWIND(.fnend)
	.size	MVC_InquireSliceProperty, .-MVC_InquireSliceProperty
	.align	2
	.global	MVC_HaveSliceToDec
	.type	MVC_HaveSliceToDec, %function
MVC_HaveSliceToDec:
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
	ldr	r0, [r0, #64]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_HaveSliceToDec, .-MVC_HaveSliceToDec
	.align	2
	.global	MVC_IsRefListWrong
	.type	MVC_IsRefListWrong, %function
MVC_IsRefListWrong:
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
	add	r5, r0, #11075584
	mov	r7, r0
	add	r5, r5, #40960
	ldr	r3, [r5, #1048]
	cmp	r3, #0
	beq	.L3012
	add	r6, r0, #252
	mov	r4, #0
	b	.L3016
.L3013:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L3015
	ldr	r3, [r5, #1048]
	cmp	r4, r3
	bcs	.L3038
.L3016:
	ldr	r3, [r6, #4]!
	add	r4, r4, #1
	ldr	r0, [r7, #120]
	ldr	r3, [r3, #4]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L3013
.L3015:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3038:
	cmp	r3, #0
	beq	.L3012
	ldr	r3, [r5, #1052]
	cmp	r3, #0
	addne	r6, r7, #384
	movne	r4, #0
	bne	.L3019
	b	.L3012
.L3039:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L3015
	ldr	r3, [r5, #1052]
	cmp	r4, r3
	bcs	.L3012
.L3019:
	ldr	r3, [r6, #4]!
	add	r4, r4, #1
	ldr	r0, [r7, #120]
	ldr	r3, [r3, #4]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L3039
	b	.L3015
.L3012:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_IsRefListWrong, .-MVC_IsRefListWrong
	.align	2
	.global	MVC_DEC_Destroy
	.type	MVC_DEC_Destroy, %function
MVC_DEC_Destroy:
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
	mov	r5, r0
	bl	MVC_ClearCurrPic
	mov	r0, r5
	bl	MVC_ClearAllNal
	ldr	r3, [r5, #60]
	cmp	r3, #0
	beq	.L3041
	mov	r4, #0
	add	r6, r5, #524
	mov	r7, r4
.L3043:
	ldr	r1, [r6, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3042
	ldr	r0, [r5, #120]
	bl	FreeUsdByDec
	str	r7, [r6]
.L3042:
	ldr	r3, [r5, #60]
	cmp	r3, r4
	bhi	.L3043
.L3041:
	movw	r9, #48936
	movw	r8, #15720
	movt	r9, 169
	movt	r8, 170
	add	r9, r5, r9
	add	r8, r5, r8
	mov	r6, #0
	mov	r7, #0
.L3047:
	sub	r4, r9, #32
.L3045:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3059
.L3044:
	cmp	r4, r9
	bne	.L3045
	add	r9, r4, #808
	cmp	r9, r8
	bne	.L3047
	ldr	r1, .L3060
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L3059:
	mov	r1, r2
	ldr	r0, [r5, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L3044
.L3061:
	.align	2
.L3060:
	.word	.LC395
	UNWIND(.fnend)
	.size	MVC_DEC_Destroy, .-MVC_DEC_Destroy
	.align	2
	.global	MVC_DEC_RecycleImage
	.type	MVC_DEC_RecycleImage, %function
MVC_DEC_RecycleImage:
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
	mov	r5, r0
	mov	r8, r1
	mov	r0, #2
	ldr	r1, .L3073
	bl	dprint_vfmw
	mov	r1, r8
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L3067
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	beq	.L3067
	add	r4, r0, #560
	add	r9, r0, #592
	mov	r6, #0
	mov	r7, #0
.L3065:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3072
.L3064:
	cmp	r4, r9
	bne	.L3065
	ldr	r0, [r5, #120]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3072:
	mov	r1, r2
	ldr	r0, [r5, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L3064
.L3067:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3074:
	.align	2
.L3073:
	.word	.LC396
	UNWIND(.fnend)
	.size	MVC_DEC_RecycleImage, .-MVC_DEC_RecycleImage
	.align	2
	.global	MVC_OutputFrmToVO
	.type	MVC_OutputFrmToVO, %function
MVC_OutputFrmToVO:
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
	clz	r5, r1
	mov	r7, r2
	mov	r6, r1
	mov	r5, r5, lsr #5
	mov	r4, r0
	cmp	r0, #0
	movne	ip, r5
	orreq	ip, r5, #1
	cmp	ip, #0
	bne	.L3109
	ldr	r3, [r0, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ble	.L3078
	ldrb	r3, [r0, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3078
	ldr	r3, [r0, #520]
	cmp	r3, #0
	strneb	ip, [r3, #2]
	movne	r0, #1
	strne	ip, [r4, #520]
	beq	.L3080
.L3108:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3078:
	mov	r1, r6
	mov	r0, r4
	bl	MVC_CheckFrameStore
	cmn	r0, #3
	beq	.L3080
	ldr	r1, [r4, #520]
	cmp	r1, #0
	beq	.L3082
	mov	r0, r4
	bl	MVC_GetImagePara
	ldr	r1, [r4, #520]
	mov	r0, r4
	bl	MVC_CheckFrameStore
	subs	r8, r0, #0
	beq	.L3110
	cmn	r8, #3
	beq	.L3090
	mov	r3, r8
	movw	r2, #1910
	ldr	r1, .L3118
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #520]
	mov	r0, r4
	sub	r8, r8, #1
	clz	r8, r8
	ldr	r1, [r3, #228]
	bl	MVC_DEC_RecycleImage
	mov	r8, r8, lsr #5
.L3089:
	ldr	r3, [r4, #520]
	tst	r8, r5
	mov	r0, #0
	strb	r0, [r3, #2]
	str	r0, [r4, #520]
	bne	.L3108
.L3082:
	mov	r1, r6
	mov	r0, r4
	bl	MVC_GetImagePara
	mov	r1, r6
	mov	r0, r4
	bl	MVC_CheckFrameStore
	subs	r3, r0, #0
	beq	.L3111
	movw	r2, #1959
	ldr	r1, .L3118
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r6, #228]
	mov	r0, r4
	bl	MVC_DEC_RecycleImage
	mov	r0, #2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3109:
	mov	r3, r1
	mov	r2, r0
	ldr	r1, .L3118+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3080:
	mov	r0, #1
	b	.L3108
.L3111:
	mov	r2, #1
	ldrsb	r1, [r6, #6]
	ldr	r0, [r4, #120]
	bl	FSP_SetDisplay
	ldrsb	r1, [r6, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L3112
	cmp	r7, #1
	add	r3, r4, #584
	streq	r7, [r5, #188]
	mov	r2, r4
	ldr	r0, [r4, #120]
	mov	r1, #15
	str	r5, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L3113
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r5, #24]
	bcs	.L3114
.L3097:
	ldr	r2, [r4, #144]
	mov	r0, #0
	ldr	r3, [r4, #136]
	add	r2, r2, #1
	str	r2, [r4, #144]
	add	r3, r3, #2
	str	r3, [r4, #136]
	b	.L3108
.L3113:
	mov	r2, r0
	ldr	r1, .L3118+8
	mov	r0, #0
	bl	dprint_vfmw
	ldrsb	r1, [r6, #6]
	mov	r2, #0
	ldr	r0, [r4, #120]
	bl	FSP_SetDisplay
	mov	r0, r4
	mov	r1, #1
	bl	MVC_ClearAll
	mvn	r0, #0
	b	.L3108
.L3090:
	ldr	r2, [r4, #520]
	mov	r3, #0
	strb	r3, [r2, #2]
	str	r3, [r4, #520]
	b	.L3082
.L3110:
	ldr	r3, [r4, #520]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	ldr	r3, [r4, #520]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetFsImagePtr
	subs	r8, r0, #0
	beq	.L3115
	ldr	r3, [r4, #520]
	mov	r2, r4
	mov	r1, #15
	cmp	r6, r3
	cmpeq	r7, #1
	moveq	r3, #1
	streq	r3, [r8, #188]
	add	r3, r4, #584
	ldr	r0, [r4, #120]
	str	r8, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L3116
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r8, #24]
	bcs	.L3117
.L3088:
	ldr	r2, [r4, #144]
	mov	r8, #1
	ldr	r3, [r4, #136]
	add	r2, r2, r8
	str	r2, [r4, #144]
	add	r3, r3, #2
	str	r3, [r4, #136]
	b	.L3089
.L3114:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3097
.L3117:
	mov	r1, r8
	mov	r0, r4
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3088
.L3116:
	mov	r2, r0
	ldr	r1, .L3118+8
	mov	r0, #0
	mov	r5, r0
	bl	dprint_vfmw
	ldr	r3, [r4, #520]
	mov	r2, r5
	strb	r5, [r3, #2]
	ldr	r3, [r4, #520]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	str	r5, [r4, #520]
	mov	r0, r4
	mov	r1, #1
	bl	MVC_ClearAll
	mvn	r0, #0
	b	.L3108
.L3112:
	movw	r2, #1933
	ldr	r1, .L3118+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3108
.L3115:
	movw	r2, #1881
	ldr	r1, .L3118+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3108
.L3119:
	.align	2
.L3118:
	.word	.LC400
	.word	.LC397
	.word	.LC399
	.word	.LC398
	UNWIND(.fnend)
	.size	MVC_OutputFrmToVO, .-MVC_OutputFrmToVO
	.align	2
	.global	MVC_OutputFrmFromDPB
	.type	MVC_OutputFrmFromDPB, %function
MVC_OutputFrmFromDPB:
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
	add	r4, r4, #11075584
	mov	r5, r0
	add	r4, r4, #45056
	ldr	r3, [r4, #2664]
	cmp	r3, #0
	moveq	r4, r3
	beq	.L3121
	mov	r2, #0
	strb	r2, [r3, #5]
	ldr	r1, [r4, #2664]
	bl	MVC_OutputFrmToVO
	ldr	r3, [r4, #2664]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	cmn	r0, #1
	movne	r0, #0
	moveq	r0, #1
	cmp	r2, #0
	rsb	r4, r0, #0
	beq	.L3123
.L3121:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3123:
	mov	r0, r5
	mov	r1, r6
	bl	MVC_RemoveFrameStoreOutDPB
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_OutputFrmFromDPB, .-MVC_OutputFrmFromDPB
	.align	2
	.global	MVC_FlushDPB
	.type	MVC_FlushDPB, %function
MVC_FlushDPB:
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
	subs	r5, r0, #0
	mov	r4, r1
	beq	.L3143
	add	r8, r5, #11075584
	add	r8, r8, #45056
	ldr	r3, [r8, #2856]
	cmp	r3, #0
	movwne	r7, #47716
	movne	r6, #0
	movtne	r7, 169
	addne	r7, r5, r7
	beq	.L3131
.L3130:
	ldr	r1, [r7, #4]!
	add	r6, r6, #1
	cmp	r1, #0
	beq	.L3129
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3129
	ldr	r3, [r1, #56]
	cmn	r4, #1
	cmpne	r3, r4
	bne	.L3129
	mov	r0, r5
	bl	MVC_UnMarkFrameStoreRef
.L3129:
	ldr	r3, [r8, #2856]
	cmp	r3, r6
	bhi	.L3130
.L3131:
	mov	r1, r4
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
	ldr	r0, [r8, #2856]
	cmp	r0, #0
	beq	.L3128
	movw	r1, #47716
	mov	r7, #0
	movt	r1, 169
	add	r1, r5, r1
	mov	r2, r7
	mvn	r6, #0
.L3135:
	ldr	r3, [r1, #4]!
	add	r2, r2, #1
	cmp	r3, #0
	beq	.L3133
	ldr	r3, [r3, #56]
	cmn	r4, #1
	cmpne	r4, r3
	addeq	r7, r7, #1
	cmp	r4, r3
	cmnne	r4, #1
	movne	r6, r3
.L3133:
	cmp	r2, r0
	bne	.L3135
	cmp	r7, #0
	beq	.L3128
	adds	r8, r6, #1
	movne	r8, #1
	cmp	r4, r6
	movle	r9, #0
	andgt	r9, r8, #1
	b	.L3141
.L3142:
	subs	r7, r7, #1
	beq	.L3128
.L3141:
	cmp	r9, #0
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r1, r6
	mov	r0, r5
	beq	.L3138
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3128
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	bne	.L3143
.L3138:
	mov	r1, r4
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3128
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	bne	.L3143
	cmp	r4, r6
	movge	r3, #0
	andlt	r3, r8, #1
	cmp	r3, #0
	beq	.L3142
	mov	r1, r6
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3128
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	beq	.L3142
.L3143:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3128:
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_FlushDPB, .-MVC_FlushDPB
	.align	2
	.global	MVC_AdaptiveMemMark
	.type	MVC_AdaptiveMemMark, %function
MVC_AdaptiveMemMark:
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
	add	r9, r0, #11141120
	add	r8, r9, #12288
	add	r7, r0, #11075584
	movw	r6, #15064
	add	r7, r7, #36864
	ldr	ip, [r8, #3420]
	movt	r6, 170
	add	r6, r0, r6
	mov	r4, r0
	add	r5, ip, #2000
	add	r10, ip, #4
	add	r5, r5, #4
	add	r3, r0, #11075584
	str	r3, [fp, #-48]
.L3190:
	ldr	r3, [r10]
	cmp	r3, #6
	ldrls	pc, [pc, r3, asl #2]
	b	.L3181
.L3183:
	.word	.L3182
	.word	.L3184
	.word	.L3185
	.word	.L3186
	.word	.L3187
	.word	.L3188
	.word	.L3189
.L3189:
	ldr	r2, [r10, #12]
	mov	r1, r6
	mov	r0, r4
	bl	MVC_MarkCurrPicLT
.L3181:
	add	r10, r10, #20
	cmp	r10, r5
	bne	.L3190
.L3182:
	ldrb	r3, [r7, #3960]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r0, #0
	beq	.L3198
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3188:
	mov	r0, r4
	bl	MVC_UnMarkAllSTRef
	mov	r0, r4
	bl	MVC_UpdateReflist
	mov	r1, #0
	mov	r0, r4
	bl	MVC_UpdateMaxLTFrmIdx
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	mov	r3, #1
	strb	r3, [r7, #3960]
	b	.L3181
.L3187:
	mov	r0, r4
	ldr	r1, [r10, #16]
	bl	MVC_UpdateMaxLTFrmIdx
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	b	.L3181
.L3186:
	ldr	r3, [r10, #12]
	mov	r1, r6
	ldr	r2, [r10, #4]
	mov	r0, r4
	bl	MVC_MarkSTToLTRef
	mov	r0, r4
	bl	MVC_UpdateReflist
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	b	.L3181
.L3185:
	mov	r0, r4
	ldr	r2, [r10, #8]
	mov	r1, r6
	bl	MVC_UnMarkLTRef
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	b	.L3181
.L3184:
	mov	r0, r4
	ldr	r2, [r10, #4]
	mov	r1, r6
	bl	MVC_UnMarkSTRef
	mov	r0, r4
	bl	MVC_UpdateReflist
	b	.L3181
.L3198:
	ldr	r3, [fp, #-48]
	mov	r2, #0
	str	r2, [r8, #3424]
	add	r9, r9, #16384
	add	r10, r3, #40960
	movw	r3, #26878
	movt	r3, 42
	ldr	r1, [r10, #2644]
	add	r3, r1, r3
	add	r3, r4, r3, lsl #2
	str	r2, [r3, #4]
	str	r2, [r9, #144]
	ldrb	r3, [r8, #2779]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3193
	bcc	.L3194
	cmp	r3, #2
	streq	r2, [r8, #3448]
	streq	r2, [r8, #3436]
	streq	r2, [r7, #3988]
	streq	r2, [r7, #3996]
.L3192:
	ldr	r1, [r8, #3496]
	mov	r0, r4
	bl	MVC_FlushDPB
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3194:
	ldr	r1, [r8, #3436]
	ldr	r2, [r8, #3444]
	ldr	r3, [r8, #3448]
	rsb	r2, r1, r2
	str	r2, [r8, #3444]
	rsb	r3, r1, r3
	str	r3, [r8, #3448]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r8, #3440]
	str	r3, [r8, #3436]
	ldr	r1, [r7, #3996]
	ldr	r2, [r7, #3984]
	ldr	r3, [r7, #3988]
	rsb	r2, r1, r2
	str	r2, [r7, #3984]
	rsb	r3, r1, r3
	str	r3, [r7, #3988]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r7, #3992]
	str	r3, [r7, #3996]
	b	.L3192
.L3193:
	str	r2, [r8, #3444]
	str	r2, [r8, #3436]
	str	r2, [r7, #3984]
	str	r2, [r7, #3996]
	b	.L3192
	UNWIND(.fnend)
	.size	MVC_AdaptiveMemMark, .-MVC_AdaptiveMemMark
	.align	2
	.global	MVC_IDRMemMarking
	.type	MVC_IDRMemMarking, %function
MVC_IDRMemMarking:
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
	add	r6, r0, #11141120
	add	r6, r6, #12288
	mov	r7, r0
	ldr	r3, [r6, #3420]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3200
	ldr	lr, [r0, #52]
	cmp	lr, #0
	beq	.L3209
	movw	r1, #47716
	mov	r2, #0
	movt	r1, 169
	add	r0, r0, #148
	add	r1, r7, r1
	mov	r4, r2
.L3208:
	ldr	r3, [r1, #4]!
	add	r2, r2, #1
	cmp	r3, #0
	beq	.L3205
	ldr	ip, [r3, #16]
	cmp	ip, #1
	streqb	ip, [r7, #6]
	ldr	ip, [r3, #56]
	ldr	r3, [r6, #3496]
	cmp	ip, r3
	streq	r4, [r0]
.L3205:
	cmp	r2, lr
	add	r0, r0, #4
	bne	.L3208
.L3209:
	add	r9, r7, #11075584
	add	ip, r9, #45056
	ldr	r10, [ip, #2856]
	cmp	r10, #0
	beq	.L3203
	movw	r4, #47716
	mov	r5, #0
	movt	r4, 169
	mov	r8, r5
	add	r4, r7, r4
	b	.L3214
.L3212:
	cmp	r5, r10
	beq	.L3203
.L3214:
	ldr	r1, [r4, #4]!
	add	r5, r5, #1
	cmp	r1, #0
	beq	.L3212
	ldr	r0, [r1, #56]
	ldr	r2, [r6, #3496]
	cmp	r0, r2
	bne	.L3212
	strb	r8, [r1, #698]
	mov	r2, #0
	strb	r8, [r1, #697]
	ldr	r1, [r4]
	str	ip, [fp, #-48]
	strb	r8, [r1, #734]
	strb	r8, [r1, #733]
	ldr	r1, [r4]
	strb	r8, [r1, #770]
	strb	r8, [r1, #769]
	ldr	r1, [r4]
	strb	r8, [r1, #3]
	ldr	r1, [r4]
	ldr	r0, [r7, #120]
	ldrsb	r1, [r1, #6]
	bl	FSP_SetRef
	ldr	r1, [r4]
	mov	r3, #1
	movw	r2, #47928
	movt	r2, 169
	strb	r8, [r1, #2]
	ldr	r1, [r4]
	strb	r8, [r1, #5]
	ldr	r1, [r4]
	strb	r3, [r1, #7]
	ldr	r0, [r4]
	ldr	r1, [r0, #52]
	add	r1, r7, r1
	add	r2, r1, r2
	strb	r8, [r2, #4]
	ldrsb	r1, [r0, #6]
	ldr	r0, [r7, #120]
	bl	FSP_GetDisplay
	mov	r2, #0
	ldr	ip, [fp, #-48]
	cmp	r0, #3
	beq	.L3213
	ldr	r1, [r4]
	ldr	r0, [r7, #120]
	ldrsb	r1, [r1, #6]
	bl	FSP_SetDisplay
	ldr	ip, [fp, #-48]
.L3213:
	str	r8, [r4]
	cmp	r5, r10
	str	r8, [r4, #64]
	str	r8, [r4, #128]
	ldr	r2, [ip, #2860]
	sub	r2, r2, #1
	str	r2, [ip, #2860]
	bne	.L3214
.L3203:
	mov	r0, r7
	bl	MVC_UpdateReflist
	mov	r0, r7
	bl	MVC_UpdateLTReflist
	ldr	r3, [r6, #3420]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3227
	add	r9, r9, #45056
	mov	r0, r3
	mov	r2, #1
	str	r3, [r9, #2872]
	strb	r3, [r6, #2780]
	strb	r2, [r6, #2781]
.L3215:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3227:
	add	r9, r9, #45056
	mov	r3, #0
	mov	r2, #1
	mov	r0, r3
	str	r2, [r9, #2872]
	strb	r2, [r6, #2780]
	str	r3, [r6, #3428]
	strb	r3, [r6, #2781]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3200:
	ldr	r1, [r6, #3496]
	bl	MVC_FlushDPB
	cmp	r0, #0
	addeq	r9, r7, #11075584
	beq	.L3203
.L3216:
	mvn	r0, #0
	b	.L3215
	UNWIND(.fnend)
	.size	MVC_IDRMemMarking, .-MVC_IDRMemMarking
	.align	2
	.global	MVC_Marking
	.type	MVC_Marking, %function
MVC_Marking:
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
	add	r3, r0, #11075584
	add	r5, r0, #11141120
	add	r3, r3, #36864
	add	r4, r5, #12288
	mov	r2, #0
	mov	r6, r0
	strb	r2, [r3, #3960]
	ldrb	r2, [r4, #2779]	@ zero_extendqisi2
	sub	r2, r2, #2
	clz	r2, r2
	mov	r2, r2, lsr #5
	strb	r2, [r3, #3961]
	ldrb	r3, [r4, #2782]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3229
	ldrb	r3, [r4, #2788]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3230
	ldr	r3, [r0, #224]
	ldr	r2, [r3, #640]
	cmp	r2, #1
	beq	.L3251
.L3230:
	ldr	r3, [r4, #3420]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r4, #3432]
	beq	.L3252
	cmp	r3, #0
	bne	.L3253
.L3237:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3252:
	cmp	r3, #0
	beq	.L3237
.L3236:
	ldrb	r7, [r4, #2777]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L3254
.L3240:
	add	r5, r5, #12288
	mov	r0, #0
	ldrb	r3, [r5, #2780]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r5, #2781]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3251:
	ldr	r2, [r3, #636]
	cmp	r2, #0
	bne	.L3230
	ldr	r2, [r3, #644]
	cmp	r2, #0
	bne	.L3230
	ldr	r3, [r3, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	bls	.L3230
.L3229:
	mov	r0, r6
	bl	MVC_IDRMemMarking
	cmp	r0, #0
	bne	.L3231
.L3238:
	ldrb	r3, [r4, #2782]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3233
	ldr	r3, [r4, #3432]
	cmp	r3, #0
	beq	.L3237
	ldr	r3, [r4, #3420]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3236
	b	.L3240
.L3254:
	mov	r0, r6
	bl	MVC_SlidingWinMark
	ldr	r3, [r4, #3420]
	strb	r7, [r3]
	strb	r7, [r4, #2780]
.L3233:
	ldr	r3, [r4, #3432]
	cmp	r3, #0
	bne	.L3240
	b	.L3237
.L3253:
	mov	r0, r6
	bl	MVC_AdaptiveMemMark
	ldr	r3, [r4, #3420]
	mov	r2, #0
	strb	r2, [r3]
	cmp	r0, r2
	beq	.L3238
.L3231:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_Marking, .-MVC_Marking
	.align	2
	.global	MVC_DirectOutput
	.type	MVC_DirectOutput, %function
MVC_DirectOutput:
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
	add	r10, r0, #11141120
	add	r5, r10, #12288
	mov	r4, r0
	ldrb	r6, [r5, #2779]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L3257
	bcc	.L3258
	cmp	r6, #2
	beq	.L3259
	mov	r6, #0
.L3256:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3259:
	ldr	r3, [r5, #3416]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r2, [r5, #2777]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3277
	ldr	r1, [r0, #520]
	cmp	r1, #0
	beq	.L3278
	bl	MVC_OutputFrmToVO
	ldr	r3, [r5, #3416]
	str	r3, [r4, #520]
	str	r3, [r3, #772]
	mov	r6, r0
.L3279:
	ldr	r3, [r4, #520]
	mov	r8, #2
	ldrb	r0, [r5, #2778]	@ zero_extendqisi2
	mov	r7, #0
	add	r1, r5, #2784
	mov	r2, #624
	add	r1, r1, #8
	strb	r0, [r3, #1]
	ldr	r3, [r4, #520]
	strb	r8, [r3, #2]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #5]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #7]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #3]
	ldr	r3, [r4, #520]
	ldrb	r0, [r5, #2788]	@ zero_extendqisi2
	strb	r0, [r3]
	ldr	r3, [r4, #520]
	str	r7, [r3, #40]
	ldr	r0, [r4, #520]
	add	r0, r0, #72
	bl	memcpy
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #3468]
	str	r2, [r3, #792]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #792]
	str	r2, [r3, #720]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	strb	r8, [r3, #768]
	ldrb	r2, [r5, #2783]	@ zero_extendqisi2
	ldr	r3, [r4, #520]
	cmp	r2, #1
	movne	r2, r7
	moveq	r2, r8
	strb	r2, [r3, #4]
.L3336:
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #3436]
	str	r2, [r3, #32]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #3496]
	str	r2, [r3, #56]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #3500]
	str	r2, [r3, #60]
	ldrb	r8, [r4, #9]	@ zero_extendqisi2
	cmp	r8, #1
	bne	.L3256
	ldr	r3, [r4, #520]
	mov	r2, r7
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
	mov	r2, r7
	ldr	r1, [r4, #520]
	mov	r0, r4
	bl	MVC_OutputFrmToVO
	ldr	r3, [r4, #520]
	cmp	r3, r7
	mov	r6, r0
	beq	.L3289
	ldr	r3, [r5, #3416]
	mov	r2, r8
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	b	.L3289
.L3258:
	ldr	r3, [r5, #3416]
	mov	r2, #0
	strb	r2, [r3, #696]
	ldr	r1, [r0, #520]
	cmp	r1, r2
	beq	.L3260
	bl	MVC_OutputFrmToVO
	cmp	r0, #0
	bne	.L3339
.L3261:
	ldr	r3, [r4, #520]
	cmp	r3, #0
	beq	.L3260
	ldr	r3, [r5, #3416]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
.L3260:
	ldr	r3, [r5, #3416]
	mov	r0, #3
	mov	r7, #0
	add	r1, r5, #2784
	add	r1, r1, #8
	mov	r2, #624
	str	r3, [r4, #520]
	str	r3, [r3, #700]
	ldr	r3, [r4, #520]
	ldr	ip, [r5, #3416]
	str	ip, [r3, #736]
	ldr	r3, [r4, #520]
	ldr	ip, [r5, #3416]
	str	ip, [r3, #772]
	ldrb	ip, [r5, #2778]	@ zero_extendqisi2
	ldr	r3, [r4, #520]
	strb	ip, [r3, #1]
	ldr	r3, [r4, #520]
	strb	r0, [r3, #2]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #3]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #5]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #7]
	ldr	r0, [r4, #520]
	add	r0, r0, #72
	bl	memcpy
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #3468]
	mov	r2, r7
	str	r1, [r3, #720]
	str	r1, [r3, #44]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #696]
	ldr	r3, [r4, #520]
	ldrb	r1, [r5, #2788]	@ zero_extendqisi2
	strb	r1, [r3]
	ldr	r3, [r4, #520]
	ldrb	r1, [r5, #2785]	@ zero_extendqisi2
	str	r1, [r3, #40]
	ldrb	r1, [r5, #2783]	@ zero_extendqisi2
	ldr	r3, [r4, #520]
	cmp	r1, #1
	moveq	r6, #3
	strb	r6, [r3, #4]
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #3436]
	str	r1, [r3, #32]
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #3496]
	str	r1, [r3, #56]
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #3500]
	str	r1, [r3, #60]
	ldr	r3, [r4, #520]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
	mov	r2, r7
	ldr	r1, [r4, #520]
	mov	r0, r4
	bl	MVC_OutputFrmToVO
	ldr	r3, [r4, #520]
	cmp	r3, r7
	mov	r6, r0
	beq	.L3289
.L3331:
	ldr	r3, [r5, #3416]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
.L3289:
	mov	r3, #0
	mov	r0, r6
	str	r3, [r4, #520]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3257:
	ldr	r3, [r5, #3416]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r2, [r5, #2777]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3265
	ldr	r1, [r0, #520]
	cmp	r1, #0
	beq	.L3266
	bl	MVC_OutputFrmToVO
	ldr	r3, [r5, #3416]
	str	r3, [r4, #520]
	str	r3, [r3, #736]
	mov	r6, r0
.L3267:
	ldr	r3, [r4, #520]
	mov	r8, #1
	ldrb	r0, [r5, #2778]	@ zero_extendqisi2
	mov	r7, #0
	add	r1, r5, #2784
	mov	r2, #624
	add	r1, r1, #8
	strb	r0, [r3, #1]
	ldr	r3, [r4, #520]
	strb	r8, [r3, #2]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #5]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #7]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #3]
	ldr	r3, [r4, #520]
	ldrb	r0, [r5, #2788]	@ zero_extendqisi2
	strb	r0, [r3]
	ldr	r3, [r4, #520]
	str	r7, [r3, #40]
	ldr	r0, [r4, #520]
	add	r0, r0, #72
	bl	memcpy
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #3468]
	str	r2, [r3, #756]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #756]
	str	r2, [r3, #720]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	strb	r8, [r3, #732]
	ldrb	r3, [r5, #2783]	@ zero_extendqisi2
	ldr	r2, [r4, #520]
	rsb	r3, r8, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r2, #4]
	b	.L3336
.L3277:
	ldr	r3, [r0, #520]
	add	r1, r5, #2784
	add	r1, r1, #8
	mov	r2, #624
	cmp	r3, #0
	add	r10, r10, #15680
	ldreq	r3, [r5, #3416]
	streq	r3, [r0, #520]
	streq	r3, [r3, #772]
	ldreq	r3, [r0, #520]
	add	ip, r3, #688
	add	r0, r3, #72
	ldrd	r6, [ip, #-8]
	ldrd	r8, [ip]
	ldr	ip, [r3, #672]
	ldr	r3, [r3, #676]
	str	ip, [fp, #-52]
	str	r3, [fp, #-48]
	bl	memcpy
	ldr	r3, [fp, #-48]
	ldr	ip, [fp, #-52]
	ldmia	r10, {r0, r1}
	and	r2, ip, r3
	adds	r2, r2, #1
	and	r1, r1, r0
	movw	r0, #15696
	movne	r2, #1
	cmn	r1, #1
	movt	r0, 170
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r4, #520]
	strne	r3, [r2, #676]
	movw	r3, #15696
	movt	r3, 170
	add	r3, r4, r3
	strne	ip, [r2, #672]
	mov	ip, #2
	ldrd	r2, [r3, #-8]
	ldr	r1, [r4, #520]
	cmp	r3, r7
	cmpeq	r2, r6
	add	r1, r1, #688
	movhi	r2, r6
	movhi	r3, r7
	strd	r2, [r1, #-8]
	ldrd	r0, [r0, r4]
	ldr	r3, [r4, #520]
	cmp	r1, r9
	cmpeq	r0, r8
	add	r3, r3, #688
	movhi	r0, r8
	movhi	r1, r9
	strd	r0, [r3]
	mov	r0, #3
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r5, #2778]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #2]
	ldr	r2, [r4, #520]
	ldrb	r1, [r5, #2788]	@ zero_extendqisi2
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r4, #520]
	strb	ip, [r3, #768]
	ldr	r3, [r4, #520]
	strb	r0, [r3, #696]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #3468]
	str	r2, [r3, #792]
	ldr	r2, [r4, #520]
	ldr	r3, [r2, #792]
	ldr	r1, [r2, #756]
	add	r3, r3, r1
	mov	r3, r3, lsr #1
	str	r3, [r2, #720]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #720]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #772]
	str	r2, [r3, #700]
	ldrb	r3, [r5, #2783]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #520]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	beq	.L3337
.L3288:
	strb	r2, [r3, #4]
	mov	r2, #0
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #3436]
	str	r1, [r3, #32]
	ldr	r3, [r4, #520]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
	mov	r2, #0
	ldr	r1, [r4, #520]
	mov	r0, r4
	bl	MVC_OutputFrmToVO
	ldr	r3, [r4, #520]
	cmp	r3, #0
	mov	r6, r0
	bne	.L3331
	b	.L3289
.L3265:
	ldr	r3, [r0, #520]
	add	r1, r5, #2784
	add	r1, r1, #8
	mov	r2, #624
	cmp	r3, #0
	add	r10, r10, #15680
	ldreq	r3, [r5, #3416]
	streq	r3, [r0, #520]
	streq	r3, [r3, #736]
	ldreq	r3, [r0, #520]
	add	ip, r3, #688
	add	r0, r3, #72
	ldrd	r8, [ip, #-8]
	ldrd	r6, [ip]
	ldr	ip, [r3, #672]
	ldr	r3, [r3, #676]
	str	ip, [fp, #-52]
	str	r3, [fp, #-48]
	bl	memcpy
	ldr	r3, [fp, #-48]
	ldr	ip, [fp, #-52]
	ldmia	r10, {r0, r1}
	and	r2, ip, r3
	adds	r2, r2, #1
	and	r1, r1, r0
	movw	r0, #15696
	movne	r2, #1
	cmn	r1, #1
	movt	r0, 170
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r4, #520]
	strne	r3, [r2, #676]
	movw	r3, #15696
	movt	r3, 170
	add	r3, r4, r3
	strne	ip, [r2, #672]
	mov	ip, #1
	ldrd	r2, [r3, #-8]
	ldr	r1, [r4, #520]
	cmp	r3, r9
	cmpeq	r2, r8
	add	r1, r1, #688
	movhi	r2, r8
	movhi	r3, r9
	strd	r2, [r1, #-8]
	ldrd	r0, [r0, r4]
	ldr	r3, [r4, #520]
	cmp	r1, r7
	cmpeq	r0, r6
	add	r3, r3, #688
	movhi	r0, r6
	movhi	r1, r7
	strd	r0, [r3]
	mov	r0, #3
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r5, #2778]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #2]
	ldr	r2, [r4, #520]
	ldrb	r1, [r5, #2788]	@ zero_extendqisi2
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r4, #520]
	strb	ip, [r3, #732]
	ldr	r3, [r4, #520]
	strb	r0, [r3, #696]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #3468]
	str	r2, [r3, #756]
	ldr	r2, [r4, #520]
	ldr	r3, [r2, #792]
	ldr	r1, [r2, #756]
	add	r3, r3, r1
	mov	r3, r3, lsr ip
	str	r3, [r2, #720]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #720]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #736]
	str	r2, [r3, #700]
	ldrb	r3, [r5, #2783]	@ zero_extendqisi2
	cmp	r3, ip
	ldr	r3, [r4, #520]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	bne	.L3288
.L3337:
	orr	r2, r2, ip
	b	.L3288
.L3339:
	str	r0, [sp]
	movw	r3, #2510
	ldr	r2, .L3340
	mov	r0, #22
	ldr	r1, .L3340+4
	bl	dprint_vfmw
	b	.L3261
.L3278:
	ldr	r3, [r5, #3416]
	mov	r6, r1
	str	r3, [r0, #520]
	str	r3, [r3, #772]
	b	.L3279
.L3266:
	ldr	r3, [r5, #3416]
	mov	r6, r1
	str	r3, [r0, #520]
	str	r3, [r3, #736]
	b	.L3267
.L3341:
	.align	2
.L3340:
	.word	.LANCHOR0+248
	.word	.LC401
	UNWIND(.fnend)
	.size	MVC_DirectOutput, .-MVC_DirectOutput
	.align	2
	.global	MVC_DEC_GetRemainImg
	.type	MVC_DEC_GetRemainImg, %function
MVC_DEC_GetRemainImg:
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
	add	r6, r0, #11075584
	add	r6, r6, #45056
	mov	r5, r0
	ldr	r3, [r6, #2856]
	cmp	r3, #0
	beq	.L3356
	movw	r8, #47716
	mov	r4, #0
	movt	r8, 169
	add	r8, r0, r8
	mov	r7, r4
	mov	r9, r8
	b	.L3345
.L3344:
	ldr	r3, [r6, #2856]
	cmp	r3, r7
	bls	.L3383
.L3345:
	ldr	r3, [r9, #4]!
	add	r7, r7, #1
	cmp	r3, #0
	beq	.L3344
	ldrb	r2, [r3, #5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3344
	ldrsb	r1, [r3, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L3344
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	sub	r3, r3, #1
	cmp	r3, #1
	ldr	r3, [r6, #2856]
	addls	r4, r4, #1
	cmp	r3, r7
	bhi	.L3345
.L3383:
	cmp	r4, #0
	beq	.L3346
	mvn	r7, #0
	b	.L3349
.L3347:
	ldr	r1, [r5, r3, asl #2]
	bl	MVC_OutputFrmToVO
	subs	r4, r4, #1
	mov	r7, r0
	beq	.L3381
.L3349:
	sub	r3, fp, #44
	sub	r2, fp, #40
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	movw	r3, #28314
	mov	r2, r4
	cmn	r1, #1
	movt	r3, 42
	mov	r0, r5
	add	r3, r1, r3
	bne	.L3347
.L3381:
	ldr	r3, [r6, #2856]
	cmp	r3, #0
	beq	.L3343
.L3348:
	mov	r4, #0
	b	.L3352
.L3351:
	ldr	r3, [r6, #2856]
	cmp	r3, r4
	bls	.L3343
.L3352:
	ldr	r1, [r8, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3351
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3351
	mov	r0, r5
	bl	MVC_UnMarkFrameStoreRef
	ldr	r3, [r6, #2856]
	cmp	r3, r4
	bhi	.L3352
.L3343:
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
	cmp	r7, #0
	bne	.L3354
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3346:
	cmp	r3, #0
	mvnne	r7, #0
	bne	.L3348
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
.L3354:
	add	r0, r5, #584
	bl	GetVoLastImageID
	mov	r7, r0
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3356:
	mvn	r7, #0
	b	.L3343
	UNWIND(.fnend)
	.size	MVC_DEC_GetRemainImg, .-MVC_DEC_GetRemainImg
	.align	2
	.global	MVC_InitDPB
	.type	MVC_InitDPB, %function
MVC_InitDPB:
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
	subs	r4, r0, #0
	beq	.L3401
	bl	MVC_ClearAllSlice
	movw	r7, #47720
	movw	r6, #47848
	movw	r5, #47784
	movt	r7, 169
	movt	r6, 169
	movt	r5, 169
	mov	r8, #0
	add	r7, r4, r7
	add	r6, r4, r6
	add	r5, r4, r5
	mov	r9, r8
.L3389:
	ldr	r3, [r7]
	mov	r2, #1
	add	r8, r8, r2
	cmp	r3, #0
	beq	.L3386
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3386:
	ldr	r3, [r5]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3387
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3387:
	ldr	r3, [r6]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3388
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3388:
	cmp	r8, #16
	str	r9, [r6], #4
	mov	r2, #0
	str	r9, [r5], #4
	str	r9, [r7], #4
	bne	.L3389
	ldr	r3, [r4, #44]
	add	r5, r4, #11075584
	add	r6, r5, #45056
	str	r3, [r6, #2856]
	str	r2, [r6, #2860]
	str	r2, [r6, #2864]
	str	r2, [r6, #2868]
	str	r2, [r6, #2872]
	ldr	r1, [r4, #520]
	cmp	r1, r2
	beq	.L3391
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3392
.L3396:
	add	r3, r4, #11141120
	add	r3, r3, #12288
	ldr	r3, [r3, #3416]
	cmp	r3, #0
	beq	.L3394
	ldrsb	r1, [r3, #6]
	mov	r2, #1
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3394:
	mov	r3, #0
	str	r3, [r4, #520]
.L3391:
	ldr	r3, .L3419
	movw	r0, #47932
	mov	r2, #344
	mov	r1, #0
	movt	r0, 169
	add	r0, r4, r0
	ldr	r3, [r3, #48]
	blx	r3
	ldr	ip, [r6, #2856]
	mov	r0, #0
	add	r3, r4, #144
	add	r1, r4, #216
	mov	r2, r0
	str	ip, [r6, #3084]
	str	r0, [r6, #3088]
.L3398:
	str	r2, [r3, #4]!
	cmp	r3, r1
	bne	.L3398
	add	r2, r4, #252
	add	r3, r4, #384
	mov	r0, #0
.L3399:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L3399
	add	r2, r5, #40960
	add	r4, r4, #516
	mov	r0, r1
	str	r1, [r2, #1048]
.L3400:
	str	r0, [r3, #4]!
	cmp	r3, r4
	mov	r2, #0
	bne	.L3400
	add	r5, r5, #40960
	mov	r0, r2
	str	r2, [r5, #1052]
.L3385:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3392:
	mov	r0, r4
	bl	MVC_OutputFrmToVO
	cmp	r0, #0
	bne	.L3418
.L3395:
	ldr	r3, [r4, #520]
	cmp	r3, #0
	bne	.L3396
	b	.L3391
.L3418:
	str	r0, [sp]
	movw	r3, #5922
	ldr	r2, .L3419+4
	mov	r0, #22
	ldr	r1, .L3419+8
	bl	dprint_vfmw
	b	.L3395
.L3401:
	mvn	r0, #0
	b	.L3385
.L3420:
	.align	2
.L3419:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+268
	.word	.LC402
	UNWIND(.fnend)
	.size	MVC_InitDPB, .-MVC_InitDPB
	.align	2
	.global	MVC_ClearDPB
	.type	MVC_ClearDPB, %function
MVC_ClearDPB:
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
	bl	MVC_FlushDPB
	cmp	r0, #0
	bne	.L3430
.L3422:
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L3423
	movw	r3, #319
	ldr	r2, .L3431
	ldr	r1, .L3431+4
	mov	r0, #22
	bl	dprint_vfmw
.L3423:
	mov	r0, #0
	strb	r0, [r4, #6]
	str	r0, [r4, #220]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3430:
	movw	r3, #314
	ldr	r2, .L3431
	ldr	r1, .L3431+8
	mov	r0, #22
	bl	dprint_vfmw
	b	.L3422
.L3432:
	.align	2
.L3431:
	.word	.LANCHOR0+280
	.word	.LC404
	.word	.LC403
	UNWIND(.fnend)
	.size	MVC_ClearDPB, .-MVC_ClearDPB
	.align	2
	.global	MVC_DecVDM
	.type	MVC_DecVDM, %function
MVC_DecVDM:
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
	add	r6, r0, #11141120
	add	r5, r6, #12288
	mov	r4, r0
	ldrb	r3, [r5, #2779]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3434
	ldrb	r3, [r5, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3434
.L3435:
	ldr	r3, [r4, #108]
	ldrb	r2, [r4, #8]	@ zero_extendqisi2
	ubfx	r7, r3, #16, #2
	cmp	r7, r2
	beq	.L3436
	sub	r2, r2, #2
	cmp	r2, #1
	cmpls	r7, #1
	bls	.L3493
.L3436:
	uxtb	r3, r7
	strb	r3, [r4, #8]
	cmp	r3, #2
	beq	.L3439
	cmp	r3, #3
	beq	.L3440
	cmp	r3, #1
	beq	.L3494
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3495
.L3447:
	ldr	r3, [r4, #64]
.L3443:
	ldrb	r2, [r5, #2789]	@ zero_extendqisi2
	strb	r2, [r4, #11]
.L3448:
	cmp	r3, #0
	beq	.L3491
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3496
	cmp	r3, #1
	bne	.L3451
	ldr	r2, [r5, #3496]
	cmp	r2, #0
	streqb	r3, [r4, #7]
.L3451:
	ldr	r2, [r4, #88]
	mov	r0, #2
	ldrb	r3, [r5, #2779]	@ zero_extendqisi2
	sub	r2, r2, #1
	ldr	r1, .L3499
	bl	dprint_vfmw
	ldr	r1, [r5, #3484]
	add	r3, r6, #16384
	mov	r2, #1
	str	r1, [r3, #1516]
	strb	r2, [r3, #714]
	ldr	r2, [r5, #3488]
	str	r2, [r3, #1512]
	ldr	r2, [r4, #64]
	str	r2, [r3, #1716]
	ldr	r0, [r4, #88]
	cmp	r0, #0
	beq	.L3488
	ldrb	r1, [r5, #2788]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3452
	ldr	r2, .L3499+4
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3453
	ldr	r2, [r3, #1720]
	cmp	r2, #0
	beq	.L3462
.L3455:
	ldr	r3, [r2, #8]
	ldr	r0, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3455
.L3454:
	sub	r2, fp, #28
	mov	r3, #4
	ldr	r0, [r4, #120]
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
	ldr	r2, [r4, #224]
.L3456:
	movw	r3, #17096
	movt	r3, 170
	add	r3, r4, r3
	str	r3, [r4, #228]
	ldr	r1, [r2, #864]
	cmp	r1, #1
	beq	.L3497
	cmp	r1, #2
	beq	.L3498
.L3459:
	ldr	r3, [r2, #592]
	add	r6, r6, #16384
	mov	r0, #0
	str	r3, [r6, #1980]
	ldr	r3, [r2, #632]
	str	r3, [r6, #1984]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3434:
	ldr	r3, [r4, #140]
	ldr	r2, [r4, #144]
	add	r3, r3, #1
	ldr	r1, [r4, #224]
	cmp	r3, r2
	movlt	r3, r2
	str	r3, [r4, #140]
	rsb	r3, r2, r3
	str	r3, [r1, #576]
	b	.L3435
.L3493:
	mov	r2, r7
	ldr	r1, .L3499+8
	mov	r0, #2
	bl	dprint_vfmw
	strb	r7, [r4, #8]
.L3440:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mov	r0, r4
	mvn	r1, #0
	bl	MVC_ClearDPB
	mvn	r0, #0
.L3488:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3494:
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3447
	ldr	r3, [r5, #3432]
	cmp	r3, #0
	beq	.L3444
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3447
.L3460:
	ldrb	r2, [r5, #2789]	@ zero_extendqisi2
	ldr	r3, [r4, #64]
	strb	r2, [r4, #11]
	b	.L3448
.L3439:
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3444
	ldr	r3, [r4, #64]
	cmp	r3, #0
	beq	.L3443
	ldr	r2, [r4, #544]
	ldr	r2, [r2, #48]
	cmp	r2, #0
	beq	.L3443
.L3444:
	mov	r3, #1
	mov	r0, r4
	strb	r3, [r5, #2783]
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3488
.L3452:
	add	r3, r4, #11075584
	add	r3, r3, #45056
	ldr	r3, [r3, #2860]
	cmp	r3, #0
	bne	.L3453
	ldr	r2, [r4, #224]
	ldr	r3, [r2, #12]
	cmp	r3, #2
	beq	.L3456
.L3491:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3488
.L3495:
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3447
	b	.L3460
.L3496:
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3451
	ldr	r3, [r4, #224]
	ldr	r7, [r3, #12]
	cmp	r7, #0
	bne	.L3451
	ldr	r3, [r5, #3432]
	mov	r0, #1
	ldr	r2, [r5, #3436]
	ldr	r1, .L3499+12
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_ClearCurrPic
	ldr	r3, [r5, #3416]
	ldr	r0, [r4, #120]
	mov	r2, r7
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L3488
.L3453:
	ldr	r2, [r4, #224]
	b	.L3456
.L3497:
	cmp	r3, #0
	beq	.L3459
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3459
	mov	r2, #0
	str	r2, [r4, #228]
	ldr	r3, [r5, #3416]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	b	.L3491
.L3498:
	ldr	r1, [r2, #868]
	cmp	r3, #0
	cmpne	r1, #0
	ble	.L3459
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3459
	mov	r2, #0
	str	r2, [r4, #228]
	ldr	r3, [r5, #3416]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	ldr	r2, [r4, #224]
	mov	r0, r4
	ldr	r3, [r2, #868]
	sub	r3, r3, #1
	str	r3, [r2, #868]
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3488
.L3462:
	mov	r1, r2
	b	.L3454
.L3500:
	.align	2
.L3499:
	.word	.LC407
	.word	g_event_report
	.word	.LC405
	.word	.LC406
	UNWIND(.fnend)
	.size	MVC_DecVDM, .-MVC_DecVDM
	.align	2
	.global	MVC_FlushDecoder
	.type	MVC_FlushDecoder, %function
MVC_FlushDecoder:
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
	add	r3, r0, #11075584
	mov	r1, #1
	add	r3, r3, #40960
	mov	ip, #0
	strb	r1, [r3, #1005]
	str	ip, [r3, #1056]
	strb	r1, [r0]
	bl	MVC_DecVDM
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	movw	r3, #12955
	ldr	r2, .L3503
	ldr	r1, .L3503+4
	mov	r0, #22
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L3504:
	.align	2
.L3503:
	.word	.LANCHOR0+296
	.word	.LC408
	UNWIND(.fnend)
	.size	MVC_FlushDecoder, .-MVC_FlushDecoder
	.align	2
	.global	MVC_ReceivePacket
	.type	MVC_ReceivePacket, %function
MVC_ReceivePacket:
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
	ldr	r3, [r1, #8]
	mov	r4, r0
	ldr	r2, [r1, #4]
	mov	r5, r1
	mov	r0, #7
	add	r6, r4, #11141120
	ldr	r1, .L3568
	str	ip, [sp]
	bl	dprint_vfmw
	add	r3, r6, #16384
	mov	r2, #0
	str	r2, [r3, #1992]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3563
.L3506:
	ldr	r3, [r4, #232]
	cmp	r3, #0
	beq	.L3526
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3564
.L3509:
	ldr	r1, .L3568+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L3526
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L3526:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3513
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L3513
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3513
	ldrb	r3, [r4, #937]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3529
	mov	r2, r4
	mov	r3, #1
	b	.L3516
.L3515:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L3527
.L3516:
	ldrb	r1, [r2, #1025]	@ zero_extendqisi2
	add	r2, r2, #88
	cmp	r1, #0
	bne	.L3515
	mov	r1, r3
.L3514:
	mov	r2, #88
	cmn	r1, #1
	mla	r3, r2, r3, r4
	mov	r0, #1
	strb	r0, [r3, #937]
	beq	.L3527
	mul	r3, r2, r1
	mov	r1, #0
	add	r2, r3, #936
	add	r3, r4, r3
	add	r2, r4, r2
	str	r2, [r4, #232]
	ldr	r2, [r5, #4]
	str	r2, [r3, #944]
	ldr	r2, [r5, #12]
	str	r2, [r3, #948]
	ldr	r2, [r5, #8]
	str	r2, [r3, #952]
	ldr	r3, [r4, #232]
	str	r1, [r3, #24]
	ldr	r3, [r4, #232]
	ldr	r2, [r5, #16]
	str	r2, [r3, #32]
	ldrd	r2, [r5, #24]
	ldr	ip, [r4, #232]
	strd	r2, [ip, #80]
	ldr	r3, [r4, #232]
	str	r0, [r3, #68]
	ldr	r3, [r4, #232]
	strb	r1, [r3]
	ldr	r3, [r4, #232]
	ldrb	r2, [r5]	@ zero_extendqisi2
	strb	r2, [r3, #3]
	ldr	r3, [r4, #232]
	b	.L3519
.L3564:
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bhi	.L3509
	ldrb	r1, [r3, #3]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3510
	b	.L3509
.L3563:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	beq	.L3506
	ldr	r3, [r4, #232]
	cmp	r3, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	beq	.L3526
.L3510:
	strb	r2, [r3, #3]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3565
.L3521:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3522
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq	.L3522
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3522
	ldr	r3, [r4, #232]
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bls	.L3566
.L3523:
	mov	r2, #1
	strb	r2, [r3, #3]
	ldr	r1, [r5, #16]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3568+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3560
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L3560:
	ldr	r3, [r4, #232]
.L3519:
	ldrb	r0, [r3, #3]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3513:
	ldr	r1, .L3568+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r5, #16]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3568+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3562
.L3561:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3562:
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3522:
	ldr	r3, [r4, #232]
	b	.L3523
.L3565:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	bne	.L3560
	b	.L3521
.L3566:
	mov	ip, r1, asl #5
	mov	r0, #0
	sub	r1, ip, r1, asl #2
	add	r3, r3, r1
	str	r2, [r3, #8]
	ldr	r3, [r4, #232]
	ldr	ip, [r5, #12]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	ip, [r3, #12]
	ldr	r3, [r4, #232]
	ldr	ip, [r5, #8]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	ip, [r3, #16]
	ldr	r3, [r4, #232]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	r0, [r3, #24]
	ldr	r3, [r4, #232]
	ldr	r0, [r5, #16]
	ldr	r1, [r3, #68]
	mov	r2, r1, asl #5
	sub	r2, r2, r1, asl #2
	add	r3, r3, r2
	str	r0, [r3, #32]
	ldr	r2, [r4, #232]
	ldr	r3, [r2, #68]
	add	r3, r3, #1
	str	r3, [r2, #68]
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #12]
	cmp	r2, #4096
	bcc	.L3567
.L3524:
	add	r6, r6, #16384
	mov	r2, #1
	str	r2, [r6, #1992]
	b	.L3519
.L3567:
	mov	r0, r4
	bl	MVC_CombinePacket.part.12
	ldr	r3, [r4, #232]
	b	.L3524
.L3527:
	ldr	r1, .L3568+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_ClearAllNal
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r3, .L3568+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3561
	b	.L3562
.L3529:
	mov	r1, r3
	b	.L3514
.L3569:
	.align	2
.L3568:
	.word	.LC409
	.word	.LC410
	.word	g_event_report
	.word	.LC412
	.word	.LC411
	UNWIND(.fnend)
	.size	MVC_ReceivePacket, .-MVC_ReceivePacket
	.align	2
	.global	MVC_ClearAll
	.type	MVC_ClearAll, %function
MVC_ClearAll:
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
	mov	r8, r0
	mov	r4, r1
	mov	r0, #22
	ldr	r1, .L3603
	bl	dprint_vfmw
	mov	r0, r8
	bl	MVC_ClearCurrPic
	mov	r0, r8
	bl	MVC_ClearAllNal
	mov	r0, r8
	bl	MVC_InitDPB
	cmp	r0, #0
	bne	.L3600
.L3571:
	cmp	r4, #0
	beq	.L3572
	add	r1, r8, #584
	ldr	r0, [r8, #120]
	bl	FSP_ClearNotInVoQueue
.L3573:
	movw	r4, #48280
	mov	r10, #0
	movt	r4, 169
	add	r4, r8, r4
	mov	r6, #0
	mov	r7, #0
.L3576:
	add	r5, r4, #624
	add	r9, r4, #656
	mov	r3, #0
	mov	r2, #1
	mvn	lr, #0
	mov	ip, #18
	mov	r0, #16
	mov	r1, #2
	str	r10, [r4, #228]
	str	r4, [r4, #772]
	str	r4, [r4, #736]
	str	r4, [r4, #700]
	strb	r3, [r4, #1]
	strb	r3, [r4, #2]
	strb	r3, [r4, #3]
	str	r3, [r4, #28]
	str	r3, [r4, #20]
	str	r3, [r4, #32]
	strb	r3, [r4, #5]
	strb	r3, [r4, #696]
	str	lr, [r4, #24]
	strb	r2, [r4, #7]
	strb	r2, [r4, #732]
	str	ip, [r4, #48]
	str	r0, [r4, #52]
	strb	r1, [r4, #768]
.L3575:
	ldrd	r2, [r5, #8]!
	orrs	r1, r2, r3
	bne	.L3601
.L3574:
	cmp	r5, r9
	bne	.L3575
	add	r10, r10, #1
	add	r4, r4, #808
	cmp	r10, #40
	bne	.L3576
	movw	r4, #15632
	movw	r5, #15664
	movt	r4, 170
	movt	r5, 170
	add	r4, r8, r4
	add	r5, r8, r5
	mov	r6, #0
	mov	r7, #0
.L3578:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3602
.L3577:
	cmp	r4, r5
	bne	.L3578
	ldr	r3, [r8, #60]
	cmp	r3, #0
	beq	.L3582
	mov	r4, #0
	add	r5, r8, #524
	mov	r6, r4
.L3581:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3580
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	str	r6, [r5]
.L3580:
	ldr	r3, [r8, #60]
	cmp	r3, r4
	bhi	.L3581
.L3582:
	add	r5, r8, #11075584
	ldr	ip, .L3603+4
	add	r3, r5, #45056
	add	r5, r5, #40960
	mov	r4, #0
	movw	r0, #41960
	strb	r4, [r8, #4]
	mov	r1, r4
	strb	r4, [r8, #9]
	movw	r2, #1656
	strb	r4, [r8, #6]
	movt	r0, 169
	strb	r4, [r8, #7]
	add	r0, r8, r0
	strb	r4, [r8, #3]
	str	r4, [r8, #60]
	str	r4, [r8, #64]
	str	r4, [r8, #104]
	str	r4, [r8, #220]
	str	r4, [r8, #232]
	ldr	ip, [ip, #48]
	strb	r4, [r3, #612]
	strb	r4, [r5, #2704]
	strb	r4, [r3, #613]
	strb	r4, [r5, #2705]
	blx	ip
	add	r3, r8, #11141120
	add	r1, r3, #12288
	add	r3, r3, #16384
	mov	r2, #2
	mov	r0, #1
	strb	r2, [r5, #1000]
	mov	r2, #256
	strb	r0, [r5, #1005]
	mov	r0, r4
	strb	r4, [r1, #2776]
	strb	r4, [r3, #136]
	str	r4, [r3, #140]
	str	r2, [r3, #144]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3601:
	mov	r1, r2
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	strd	r6, [r5]
	b	.L3574
.L3602:
	mov	r1, r2
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L3577
.L3572:
	add	r0, r8, #584
	bl	ResetVoQueue
	ldr	r0, [r8, #120]
	bl	FSP_EmptyInstance
	b	.L3573
.L3600:
	mov	r3, #344
	ldr	r2, .L3603+8
	ldr	r1, .L3603+12
	mov	r0, #22
	bl	dprint_vfmw
	b	.L3571
.L3604:
	.align	2
.L3603:
	.word	.LC413
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+316
	.word	.LC404
	UNWIND(.fnend)
	.size	MVC_ClearAll, .-MVC_ClearAll
	.align	2
	.global	MVC_InsertFrmInDPB
	.type	MVC_InsertFrmInDPB, %function
MVC_InsertFrmInDPB:
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
	add	r3, r0, #11075584
	add	r3, r3, #45056
	mov	r4, r2
	ldrb	r7, [r4, #3]	@ zero_extendqisi2
	mov	r6, r0
	ldr	r3, [r3, #2860]
	str	r1, [fp, #-48]
	cmp	r3, #0
	ldreq	r2, .L3706
	streq	r3, [r2, #-1884]
	cmp	r7, #1
	beq	.L3608
	bcc	.L3609
	cmp	r7, #2
	beq	.L3610
	mov	r2, r7
	ldr	r1, .L3706+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	mov	r1, #1
	bl	MVC_ClearAll
	mov	r0, #0
.L3614:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3610:
	ldr	r3, [r4, #640]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [fp, #-48]
	mov	r3, r3, asl #2
	beq	.L3630
	add	r8, r0, r3
	add	r2, r8, #11075584
	str	r2, [fp, #-52]
	add	r5, r2, #45056
	ldr	r2, [r5, #2664]
	cmp	r2, #0
	beq	.L3630
	strb	r7, [r2, #768]
	ldr	r3, [r5, #2664]
	ldr	r2, [r4, #640]
	str	r2, [r3, #772]
	ldr	r3, [r5, #2664]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #769]
	ldr	r3, [r5, #2664]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #770]
	ldr	r3, [r5, #2664]
	ldr	r2, [r4, #672]
	str	r2, [r3, #784]
	ldr	r3, [r5, #2664]
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strb	r2, [r3, #771]
	ldr	r3, [r5, #2664]
	ldr	r2, [r4, #692]
	str	r2, [r3, #792]
	ldr	r3, [r5, #2664]
	ldr	r1, [r3, #756]
	ldr	r2, [r3, #792]
	add	r2, r2, r1
	str	r2, [r3, #720]
	str	r2, [r3, #44]
	add	r1, r4, #16
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r5, #2664]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	orreq	r2, r2, #2
	strb	r2, [r3, #4]
	mov	r2, #624
	ldr	r3, [r5, #2664]
	add	ip, r3, #688
	add	r0, r3, #72
	ldr	r10, [r3, #672]
	ldrd	r8, [ip, #-8]
	ldr	r7, [r3, #676]
	strd	r8, [fp, #-60]
	ldrd	r8, [ip]
	strd	r8, [fp, #-68]
	bl	memcpy
	add	r3, r4, #624
	and	r2, r10, r7
	adds	r2, r2, #1
	ldrd	r8, [fp, #-60]
	ldmdb	r3, {r0, r1}
	movne	r2, #1
	and	r1, r1, r0
	add	r0, r4, #640
	cmn	r1, #1
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r5, #2664]
	strne	r10, [r2, #672]
	strne	r7, [r2, #676]
	ldrd	r2, [r3]
	ldr	r1, [r5, #2664]
	cmp	r3, r9
	cmpeq	r2, r8
	add	r1, r1, #688
	movhi	r2, r8
	movhi	r3, r9
	ldrd	r8, [fp, #-68]
	strd	r2, [r1, #-8]
	ldrd	r2, [r0, #-8]
	ldr	r1, [r5, #2664]
	cmp	r3, r9
	cmpeq	r2, r8
	add	r1, r1, #688
	movhi	r3, r9
	movhi	r2, r8
	strd	r2, [r1]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3642
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3642
.L3643:
	ldr	r3, [fp, #-52]
	add	r8, r3, #45056
	ldr	r2, [r8, #2664]
	ldrb	r3, [r2, #771]	@ zero_extendqisi2
	ldrb	r1, [r2, #735]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r8, #2664]
	ldr	r2, [r4, #688]
	str	r2, [r3, #788]
	ldr	r2, [r8, #2664]
	ldr	r3, [r2, #788]
	ldr	r1, [r2, #752]
	cmp	r3, r1
	movge	r3, r1
	str	r3, [r2, #36]
	ldr	r0, [r8, #2664]
	bl	MVC_CombineFldsToFrm
	ldr	r2, [r8, #2664]
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	movw	r3, #28402
	movt	r3, 42
	strb	r1, [r2, #13]
	ldrb	r1, [r4, #10]	@ zero_extendqisi2
	ldr	r2, [r8, #2664]
	strb	r1, [r2, #11]
	ldr	r2, [r8, #2664]
	ldr	r1, [r2, #52]
	ldr	r2, [r2, #784]
	add	r3, r1, r3
	add	r3, r6, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3629
.L3609:
	ldr	r3, [fp, #-48]
	ldr	r2, [r4, #640]
	add	r3, r0, r3, lsl #2
	add	r10, r3, #11075584
	mov	r3, #3
	add	r5, r10, #45056
	str	r2, [r5, #2664]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r2, #1]
	ldr	r2, [r5, #2664]
	strb	r3, [r2, #2]
	ldr	r1, [r4, #640]
	ldr	r2, [r5, #2664]
	ldrb	r1, [r1, #5]	@ zero_extendqisi2
	strb	r1, [r2, #5]
	ldr	r1, [r4, #640]
	ldr	r2, [r5, #2664]
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	strb	r1, [r2, #7]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	ldr	r2, [r5, #2664]
	cmp	r1, #0
	beq	.L3705
.L3611:
	strb	r3, [r2, #3]
	add	r1, r4, #16
	ldr	r3, [r5, #2664]
	mov	r2, #624
	ldr	r0, [r4, #716]
	mov	r8, #0
	str	r0, [r3, #48]
	ldr	r0, [r5, #2664]
	add	r0, r0, #72
	bl	memcpy
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #692]
	add	r3, r10, #45056
	mov	lr, #1
	ldr	r10, .L3706
	mov	r2, r8
	mov	r0, r6
	str	ip, [r1, #720]
	str	ip, [r1, #44]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #640]
	str	ip, [r1, #772]
	str	ip, [r1, #736]
	str	ip, [r1, #700]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #652]
	str	ip, [r1, #28]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #648]
	str	ip, [r1, #20]
	ldr	r1, [r5, #2664]
	strb	r8, [r1, #696]
	ldr	r1, [r5, #2664]
	ldrb	ip, [r4, #4]	@ zero_extendqisi2
	strb	ip, [r1, #697]
	ldr	r1, [r5, #2664]
	ldrb	ip, [r4, #5]	@ zero_extendqisi2
	strb	ip, [r1, #698]
	ldr	r1, [r5, #2664]
	ldrb	ip, [r4, #12]	@ zero_extendqisi2
	strb	ip, [r1, #699]
	strb	ip, [r1]
	ldr	r1, [r5, #2664]
	ldrb	ip, [r4, #9]	@ zero_extendqisi2
	str	ip, [r1, #40]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #660]
	str	ip, [r1, #32]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #664]
	str	ip, [r1, #712]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #668]
	str	ip, [r1, #748]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #672]
	str	ip, [r1, #784]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #676]
	str	ip, [r1, #36]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #680]
	str	ip, [r1, #716]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #684]
	str	ip, [r1, #752]
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #688]
	str	ip, [r1, #788]
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	ldr	ip, [r5, #2664]
	cmp	r1, lr
	moveq	r7, #3
	strb	r7, [ip, #4]
	ldr	r1, [r3, #2664]
	ldr	ip, [r4, #720]
	str	ip, [r1, #56]
	ldr	r1, [r3, #2664]
	ldr	ip, [r4, #724]
	str	ip, [r1, #60]
	ldr	r1, [r3, #2664]
	ldrb	ip, [r4, #11]	@ zero_extendqisi2
	strb	ip, [r1, #13]
	strb	ip, [r1, #12]
	ldrb	r5, [r4, #10]	@ zero_extendqisi2
	ldr	ip, [r3, #2664]
	ldr	r1, [r10, #-1884]
	strb	r5, [ip, #11]
	add	r7, r1, lr
	strb	r5, [ip, #10]
	ldr	ip, [r3, #2664]
	str	r7, [r10, #-1884]
	str	r1, [ip, #64]
	ldr	r3, [r3, #2664]
	ldr	r1, [r3, #700]
	ldrb	r10, [r3, #697]	@ zero_extendqisi2
	ldrb	r7, [r3, #698]	@ zero_extendqisi2
	ldr	r5, [r3, #720]
	ldrb	ip, [r3, #699]	@ zero_extendqisi2
	strb	lr, [r3, #732]
	mov	lr, #2
	strb	r10, [r3, #733]
	strb	lr, [r3, #768]
	strb	r10, [r3, #769]
	strb	r7, [r3, #734]
	strb	r7, [r3, #770]
	str	r5, [r3, #756]
	str	r5, [r3, #792]
	strb	ip, [r3, #735]
	strb	ip, [r3, #771]
	str	r1, [r3, #736]
	str	r1, [r3, #772]
	ldr	r1, [r4, #640]
	bl	MVC_GetAPC
	cmp	r0, #0
	movne	r3, r0
	movwne	r2, #2205
	movne	r0, r8
	bne	.L3703
.L3629:
	ldr	r2, [fp, #-48]
	movw	r3, #28314
	movt	r3, 42
	add	r3, r2, r3
	ldr	r3, [r6, r3, asl #2]
	ldrb	r0, [r3, #3]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L3614
	ldr	r0, [r6, #120]
	mov	r2, #1
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3608:
	ldr	r3, [r4, #640]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [fp, #-48]
	mov	r3, r3, asl #2
	beq	.L3615
	add	r8, r0, r3
	add	r2, r8, #11075584
	str	r2, [fp, #-52]
	add	r5, r2, #45056
	ldr	r2, [r5, #2664]
	cmp	r2, #0
	beq	.L3615
	strb	r7, [r2, #732]
	ldr	r3, [r5, #2664]
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strb	r2, [r3, #735]
	ldr	r3, [r5, #2664]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #733]
	ldr	r3, [r5, #2664]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #734]
	ldr	r3, [r5, #2664]
	ldr	r2, [r4, #668]
	str	r2, [r3, #748]
	ldr	r3, [r5, #2664]
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strb	r2, [r3, #735]
	ldr	r3, [r5, #2664]
	ldr	r2, [r4, #692]
	str	r2, [r3, #756]
	ldr	r3, [r5, #2664]
	ldr	r1, [r3, #756]
	ldr	r2, [r3, #792]
	add	r2, r2, r1
	str	r2, [r3, #720]
	str	r2, [r3, #44]
	add	r1, r4, #16
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r5, #2664]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	orreq	r2, r2, #1
	strb	r2, [r3, #4]
	mov	r2, #624
	ldr	r3, [r5, #2664]
	add	ip, r3, #688
	add	r0, r3, #72
	ldr	r10, [r3, #672]
	ldrd	r8, [ip, #-8]
	ldr	r7, [r3, #676]
	strd	r8, [fp, #-60]
	ldrd	r8, [ip]
	strd	r8, [fp, #-68]
	bl	memcpy
	add	r3, r4, #624
	and	r2, r10, r7
	adds	r2, r2, #1
	ldrd	r8, [fp, #-60]
	ldmdb	r3, {r0, r1}
	movne	r2, #1
	and	r1, r1, r0
	add	r0, r4, #640
	cmn	r1, #1
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r5, #2664]
	strne	r10, [r2, #672]
	strne	r7, [r2, #676]
	ldrd	r2, [r3]
	ldr	r1, [r5, #2664]
	cmp	r3, r9
	cmpeq	r2, r8
	add	r1, r1, #688
	movhi	r2, r8
	movhi	r3, r9
	ldrd	r8, [fp, #-68]
	strd	r2, [r1, #-8]
	ldrd	r2, [r0, #-8]
	ldr	r1, [r5, #2664]
	cmp	r3, r9
	cmpeq	r2, r8
	add	r1, r1, #688
	movhi	r3, r9
	movhi	r2, r8
	strd	r2, [r1]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3626
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3626
.L3627:
	ldr	r3, [fp, #-52]
	add	r8, r3, #45056
	ldr	r2, [r8, #2664]
	ldrb	r3, [r2, #771]	@ zero_extendqisi2
	ldrb	r1, [r2, #735]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r8, #2664]
	ldr	r2, [r4, #684]
	str	r2, [r3, #752]
	ldr	r2, [r8, #2664]
	ldr	r3, [r2, #788]
	ldr	r1, [r2, #752]
	cmp	r3, r1
	movge	r3, r1
	str	r3, [r2, #36]
	ldr	r3, [r8, #2664]
	ldrb	r2, [r4, #11]	@ zero_extendqisi2
	strb	r2, [r3, #12]
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	ldr	r3, [r8, #2664]
	strb	r2, [r3, #10]
	ldr	r0, [r8, #2664]
	bl	MVC_CombineFldsToFrm
	ldr	r2, [r8, #2664]
	movw	r3, #28386
	movt	r3, 42
	ldr	r1, [r2, #52]
	ldr	r2, [r2, #748]
	add	r3, r1, r3
	add	r3, r6, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3629
.L3615:
	add	r5, r6, r3
	ldr	r2, [r4, #640]
	add	r5, r5, #11075584
	mov	r1, #1
	add	r3, r5, #45056
	str	r2, [r3, #2664]
	strb	r1, [r2, #732]
	ldr	r2, [r3, #2664]
	ldr	r0, [r4, #640]
	str	r0, [r2, #736]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	strb	r0, [r2, #733]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #5]	@ zero_extendqisi2
	strb	r0, [r2, #734]
	ldr	r2, [r3, #2664]
	ldr	r0, [r4, #668]
	str	r0, [r2, #748]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #12]	@ zero_extendqisi2
	strb	r0, [r2, #735]
	ldr	r2, [r3, #2664]
	ldr	r0, [r4, #692]
	str	r0, [r2, #756]
	ldr	r2, [r3, #2664]
	ldr	r0, [r2, #756]
	str	r0, [r2, #720]
	str	r0, [r2, #44]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #2]	@ zero_extendqisi2
	strb	r0, [r2, #1]
	ldr	r2, [r3, #2664]
	strb	r1, [r2, #2]
	ldr	r1, [r4, #640]
	ldr	r2, [r3, #2664]
	ldrb	r1, [r1, #5]	@ zero_extendqisi2
	strb	r1, [r2, #5]
	ldr	r1, [r4, #640]
	ldr	r2, [r3, #2664]
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	strb	r1, [r2, #7]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3617
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3617
.L3618:
	add	r5, r5, #45056
	mov	r7, #0
	mov	r2, #624
	add	r1, r4, #16
	ldr	r3, [r5, #2664]
	str	r7, [r3, #40]
	ldr	r0, [r5, #2664]
	add	r0, r0, #72
	bl	memcpy
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #716]
	mov	r2, #1
	ldr	r3, .L3706
	mov	r0, r6
	str	ip, [r1, #48]
	ldr	ip, [r5, #2664]
	ldr	lr, [r4, #648]
	ldr	r1, [r3, #-1884]
	str	lr, [ip, #20]
	add	ip, r1, r2
	ldr	lr, [r5, #2664]
	ldr	r8, [r4, #660]
	str	r8, [lr, #32]
	ldr	lr, [r5, #2664]
	ldr	r8, [r4, #676]
	str	r8, [lr, #36]
	ldr	lr, [r5, #2664]
	ldrb	r8, [r4, #12]	@ zero_extendqisi2
	strb	r8, [lr]
	ldr	lr, [r5, #2664]
	ldr	r8, [r4, #684]
	str	r8, [lr, #752]
	ldr	lr, [r5, #2664]
	ldr	r8, [r4, #720]
	str	r8, [lr, #56]
	ldr	lr, [r5, #2664]
	ldr	r8, [r4, #724]
	str	r8, [lr, #60]
	ldr	lr, [r5, #2664]
	ldrb	r8, [r4, #11]	@ zero_extendqisi2
	strb	r8, [lr, #12]
	ldr	lr, [r5, #2664]
	ldrb	r8, [r4, #10]	@ zero_extendqisi2
	strb	r8, [lr, #10]
	ldr	lr, [r5, #2664]
	str	r1, [lr, #64]
	str	ip, [r3, #-1884]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [r5, #2664]
	rsb	r3, r2, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r1, #4]
	ldr	r1, [r4, #640]
	bl	MVC_GetAPC
	cmp	r0, #0
	beq	.L3629
	mov	r3, r0
	movw	r2, #2268
	mov	r0, r7
.L3703:
	ldr	r1, .L3706+8
	bl	dprint_vfmw
	mov	r0, r6
	mvn	r1, #0
	bl	MVC_ClearDPB
	mvn	r0, #0
	b	.L3614
.L3630:
	add	r5, r6, r3
	ldr	r2, [r4, #640]
	add	r5, r5, #11075584
	mov	r1, #2
	add	r3, r5, #45056
	str	r2, [r3, #2664]
	strb	r1, [r2, #768]
	ldr	r2, [r3, #2664]
	ldr	r0, [r4, #640]
	str	r0, [r2, #772]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	strb	r0, [r2, #769]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #5]	@ zero_extendqisi2
	strb	r0, [r2, #770]
	ldr	r2, [r3, #2664]
	ldr	r0, [r4, #672]
	str	r0, [r2, #784]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #12]	@ zero_extendqisi2
	strb	r0, [r2, #771]
	ldr	r2, [r3, #2664]
	ldr	r0, [r4, #692]
	str	r0, [r2, #792]
	ldr	r2, [r3, #2664]
	ldr	r0, [r2, #756]
	str	r0, [r2, #720]
	str	r0, [r2, #44]
	ldr	r2, [r3, #2664]
	ldrb	r0, [r4, #2]	@ zero_extendqisi2
	strb	r0, [r2, #1]
	ldr	r2, [r3, #2664]
	strb	r1, [r2, #2]
	ldr	r1, [r4, #640]
	ldr	r2, [r3, #2664]
	ldrb	r1, [r1, #5]	@ zero_extendqisi2
	strb	r1, [r2, #5]
	ldr	r1, [r4, #640]
	ldr	r2, [r3, #2664]
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	strb	r1, [r2, #7]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3632
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3632
.L3633:
	add	r5, r5, #45056
	mov	r8, #0
	mov	r2, #624
	add	r1, r4, #16
	ldr	r3, [r5, #2664]
	str	r8, [r3, #40]
	ldr	r0, [r5, #2664]
	add	r0, r0, #72
	bl	memcpy
	ldr	r1, [r5, #2664]
	ldr	ip, [r4, #716]
	mov	r2, #2
	ldr	r3, .L3706
	mov	r0, r6
	str	ip, [r1, #48]
	ldr	ip, [r5, #2664]
	ldr	lr, [r4, #648]
	ldr	r1, [r3, #-1884]
	str	lr, [ip, #20]
	add	r7, r1, #1
	ldr	ip, [r5, #2664]
	ldr	lr, [r4, #660]
	str	lr, [ip, #32]
	ldr	ip, [r5, #2664]
	ldr	lr, [r4, #676]
	str	lr, [ip, #36]
	ldr	ip, [r5, #2664]
	ldrb	lr, [r4, #12]	@ zero_extendqisi2
	strb	lr, [ip, #771]
	strb	lr, [ip]
	ldr	ip, [r5, #2664]
	ldr	lr, [r4, #688]
	str	lr, [ip, #788]
	ldr	ip, [r5, #2664]
	ldr	lr, [r4, #720]
	str	lr, [ip, #56]
	ldr	ip, [r5, #2664]
	ldr	lr, [r4, #724]
	str	lr, [ip, #60]
	ldr	ip, [r5, #2664]
	ldrb	lr, [r4, #11]	@ zero_extendqisi2
	strb	lr, [ip, #13]
	ldr	ip, [r5, #2664]
	ldrb	lr, [r4, #10]	@ zero_extendqisi2
	strb	lr, [ip, #11]
	ldr	ip, [r5, #2664]
	str	r1, [ip, #64]
	ldrb	ip, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [r5, #2664]
	cmp	ip, #1
	str	r7, [r3, #-1884]
	moveq	r7, r2
	movne	r7, r8
	strb	r7, [r1, #4]
	ldr	r1, [r4, #640]
	bl	MVC_GetAPC
	cmp	r0, #0
	movne	r3, r0
	movwne	r2, #2393
	movne	r0, r8
	beq	.L3629
	b	.L3703
.L3705:
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	b	.L3611
.L3632:
	ldr	r2, [r3, #2664]
	mov	r1, #2
	strb	r1, [r2, #3]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	ldreq	r3, [r3, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3633
.L3617:
	ldr	r2, [r3, #2664]
	mov	r1, #1
	strb	r1, [r2, #3]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, r1
	ldreq	r3, [r3, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3618
.L3626:
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #3]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #3]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r5, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3627
.L3642:
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #3]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #3]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r5, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3643
.L3707:
	.align	2
.L3706:
	.word	.LANCHOR3
	.word	.LC415
	.word	.LC414
	UNWIND(.fnend)
	.size	MVC_InsertFrmInDPB, .-MVC_InsertFrmInDPB
	.align	2
	.global	MVC_AllocFrameStore
	.type	MVC_AllocFrameStore, %function
MVC_AllocFrameStore:
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
	add	r9, r0, #11075584
	add	r7, r9, #40960
	mov	r6, r0
	str	r1, [fp, #-52]
	ldrb	r5, [r7, #1001]	@ zero_extendqisi2
	cmp	r5, #0
	beq	.L3709
	ldrb	r3, [r7, #1002]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r5, #2
	moveq	r5, #1
.L3709:
	ldr	r3, [fp, #-52]
	cmp	r3, #1
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	beq	.L3710
	cmp	r3, #2
	beq	.L3792
	add	r3, r6, #11141120
	str	r3, [fp, #-56]
	mov	r2, r3
	sub	r3, r5, #1
	add	r4, r2, #12288
	cmp	r3, #1
	mov	r3, #0
	strb	r3, [r4, #2777]
	bls	.L3793
.L3732:
	mov	r8, #0
	strb	r8, [r4, #2776]
	ldr	r10, [r6, #48]
	cmp	r10, r8
	beq	.L3739
	movw	ip, #48280
	add	r9, r9, #45056
	movt	ip, 169
	add	ip, r6, ip
	movw	r3, #47720
	movt	r3, 169
	add	r3, r6, r3
	str	r3, [fp, #-48]
.L3738:
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3734
	cmp	ip, #0
	beq	.L3751
	ldr	lr, [r9, #2856]
	cmp	lr, #0
	beq	.L3751
	ldr	r2, [r9, #2664]
	rsb	r3, ip, r2
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L3734
	ldr	r1, [fp, #-48]
	b	.L3736
.L3737:
	ldr	r2, [r1, #4]!
	rsb	r0, ip, r2
	cmp	r2, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L3734
.L3736:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L3737
.L3751:
	mov	r3, #808
	movw	r2, #48320
	mla	r8, r3, r8, r6
	movt	r2, 169
	movw	r1, #48352
	add	r0, r4, #2784
	mov	r9, #0
	mov	r3, #1
	add	r2, r8, r2
	strb	r3, [r4, #2776]
	str	ip, [r4, #3416]
	movt	r1, 169
	str	r9, [r2, #4]
	add	r1, r8, r1
	mov	r2, #101
	add	r0, r0, #8
	str	r2, [r4, #3468]
	mov	r2, #624
	mov	r10, ip
	str	r3, [fp, #-48]
	bl	memcpy
	movw	r2, #48280
	movt	r2, 169
	add	r2, r8, r2
	str	r9, [r4, #2952]
	strb	r9, [r2, #4]
	strb	r9, [r10, #7]
	ldr	r2, [r4, #3416]
	ldr	r3, [fp, #-48]
	strb	r9, [r2, #3]
	ldr	r2, [r4, #3416]
	strb	r3, [r2, #5]
	ldr	r3, [r4, #3416]
	str	r9, [r3, #16]
	ldrb	r3, [r4, #2776]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3739
	str	r3, [fp, #-48]
	ldr	r3, [fp, #-52]
	ldr	r0, [r6, #120]
	clz	r1, r3
	mov	r1, r1, lsr #5
	bl	FSP_NewLogicFs
	mov	r8, r0
	mov	r1, r0
	ldr	r0, [r6, #120]
	bl	FSP_GetLogicFs
	mvn	r2, r8
	mov	r2, r2, lsr #31
	ldr	r3, [fp, #-48]
	cmp	r0, #0
	mov	r10, r0
	moveq	r2, #0
	cmp	r2, #0
	beq	.L3740
	ldr	r3, [r6, #224]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L3794
.L3741:
	ldr	r9, .L3801
	mov	r2, #624
	ldr	r0, [r4, #3416]
	add	r1, r10, #8
	ldr	r3, [r9, #52]
	add	r0, r0, #72
	blx	r3
	ldr	r1, [r4, #3416]
	movw	r0, #15080
	ldr	r3, [r9, #52]
	movt	r0, 170
	add	r1, r1, #72
	add	r0, r6, r0
	mov	r2, #624
	blx	r3
	ldr	r3, [r4, #3416]
	strb	r8, [r3, #6]
	ldrsb	r3, [r10, #4]
	str	r3, [r4, #3492]
.L3731:
	ldr	r3, [fp, #-52]
	uxtb	r2, r5
	strb	r2, [r4, #2779]
	cmp	r3, #1
	moveq	r0, #0
	beq	.L3785
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	ldr	r1, [r4, #3416]
	cmp	r3, #0
	movne	r3, #3
	strb	r3, [r1, #696]
	ldrb	r3, [r4, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [fp, #-56]
	add	r3, r3, #16384
	beq	.L3795
	strb	r2, [r3, #136]
	cmp	r5, #0
	ldr	r2, [r7, #1028]
	movne	r0, #0
	moveq	r0, r5
	str	r2, [r3, #144]
	ldrb	r2, [r7, #1008]	@ zero_extendqisi2
	str	r2, [r3, #140]
	ldr	r2, [r6, #128]
	str	r2, [r3, #148]
	ldr	r2, [r7, #2640]
	str	r8, [r3, #152]
	streqb	r5, [r3, #137]
	str	r2, [r3, #160]
	movne	r2, #1
	strneb	r2, [r3, #137]
.L3785:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3734:
	add	r8, r8, #1
	add	ip, ip, #808
	cmp	r8, r10
	bne	.L3738
.L3739:
	ldr	r1, .L3801+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	mov	r1, #1
	bl	MVC_ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3792:
	add	r4, r6, #11141120
	str	r4, [fp, #-56]
	add	r4, r4, #12288
.L3729:
	mov	r3, #0
	strb	r3, [r4, #2777]
	b	.L3732
.L3710:
	cmp	r3, #2
	beq	.L3796
	add	r3, r6, #11141120
	str	r3, [fp, #-56]
	mov	r2, r3
	add	r4, r2, #12288
	mov	r3, #0
	mov	r5, r3
	strb	r3, [r4, #2777]
	b	.L3732
.L3794:
	ldr	r3, [r6, #16]
	ldr	r1, [r0, #148]
	ldr	r2, [r6, #12]
	mov	r3, r3, asl #4
	ldr	r0, [r0, #52]
	str	r3, [r10, #124]
	mul	r3, r1, r3
	mov	r2, r2, asl #4
	str	r2, [r10, #120]
	add	r0, r3, r0
	str	r0, [r10, #56]
	str	r3, [fp, #-48]
	bl	MEM_Phy2Vir
	ldr	r3, [fp, #-48]
	ldr	r1, [r10, #36]
	ldr	r2, [r10, #148]
	str	r9, [r10, #524]
	str	r0, [r10, #520]
	ldr	r0, [r10, #68]
	add	r0, r3, r0
	add	r3, r3, r1
	str	r0, [r10, #72]
	str	r3, [r10, #40]
	add	r3, r3, r2
	str	r3, [r10, #48]
	bl	MEM_Phy2Vir
	str	r9, [r10, #540]
	str	r0, [r10, #536]
	b	.L3741
.L3793:
	add	r3, r2, #16384
	ldrb	r2, [r3, #136]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3713
	ldr	r0, [r3, #160]
	ldr	r1, [r7, #2640]
	cmp	r0, r1
	beq	.L3714
.L3713:
	add	r3, r9, #45056
	ldr	r1, [r3, #2856]
	subs	r3, r1, #1
	bmi	.L3732
	movw	r2, #28314
	movt	r2, 42
	add	r2, r1, r2
	add	r2, r6, r2, lsl #2
	b	.L3725
.L3716:
	cmp	r5, #2
	beq	.L3797
.L3715:
	subs	r3, r3, #1
	bmi	.L3732
.L3725:
	ldr	r8, [r2, #-4]!
	cmp	r8, #0
	beq	.L3715
	cmp	r5, #1
	bne	.L3716
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L3715
.L3717:
	ldr	r0, [r7, #1028]
	ldr	r1, [r8, #20]
	cmp	r0, r1
	bne	.L3715
	ldr	r0, [r7, #2640]
	ldr	r1, [r8, #56]
	cmp	r0, r1
	bne	.L3715
	ldrb	r1, [r7, #1008]	@ zero_extendqisi2
	cmp	r1, #0
	ldrb	r1, [r8, #3]	@ zero_extendqisi2
	beq	.L3798
	cmp	r1, #0
	beq	.L3715
.L3721:
	add	r0, r4, #2784
	mov	r3, #1
	mov	r2, #624
	strb	r3, [r4, #2777]
	add	r1, r8, #72
	str	r8, [r4, #3416]
	add	r0, r0, #8
	bl	memcpy
	ldrsb	r1, [r8, #6]
	ldr	r0, [r6, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L3799
	ldrb	r2, [r4, #2777]	@ zero_extendqisi2
	ldrsb	r3, [r3, #4]
	cmp	r2, #0
	str	r3, [r4, #3492]
	beq	.L3732
	ldr	r3, [r4, #3416]
	cmp	r3, #0
	beq	.L3732
.L3789:
	mov	r8, #0
	b	.L3731
.L3797:
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3715
	b	.L3717
.L3795:
	mov	r2, #0
	strb	r2, [r3, #136]
	ldr	r1, [r7, #1028]
	mov	r0, r2
	str	r1, [r3, #144]
	ldrb	r1, [r7, #1008]	@ zero_extendqisi2
	str	r1, [r3, #140]
	ldr	r1, [r6, #128]
	str	r8, [r3, #152]
	strb	r2, [r3, #137]
	str	r1, [r3, #148]
	ldr	r2, [r7, #2640]
	str	r2, [r3, #160]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3796:
	add	r4, r6, #11141120
	mov	r5, #0
	str	r4, [fp, #-56]
	add	r4, r4, #12288
	b	.L3729
.L3798:
	cmp	r1, #0
	bne	.L3715
	b	.L3721
.L3714:
	sub	r8, r5, #2
	cmp	r2, #1
	cmpeq	r5, #2
	clz	r8, r8
	mov	r8, r8, lsr #5
	beq	.L3726
	cmp	r5, #1
	cmpeq	r2, #2
	bne	.L3732
.L3726:
	ldr	r1, [r3, #144]
	ldr	r2, [r7, #1028]
	cmp	r1, r2
	bne	.L3732
	ldrb	r2, [r7, #1008]	@ zero_extendqisi2
	ldr	r3, [r3, #140]
	cmp	r2, #0
	bne	.L3727
	cmp	r3, #0
	bne	.L3732
.L3728:
	ldr	r3, [r6, #128]
	ldr	r2, [r6, #124]
	rsb	r3, r2, r3
	cmp	r3, #2
	bhi	.L3732
	ldr	r3, [r4, #3416]
	mov	r2, #1
	strb	r2, [r4, #2777]
	cmp	r3, #0
	beq	.L3729
	cmp	r8, #0
	bne	.L3800
	cmp	r5, #1
	bne	.L3731
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3729
	b	.L3731
.L3740:
	ldr	ip, [r4, #3416]
	mov	r0, r2
	strb	r2, [r4, #2776]
	ldr	r1, .L3801+8
	strb	r2, [ip, #5]
	str	r3, [fp, #-48]
	bl	dprint_vfmw
	ldr	r3, [fp, #-48]
	mov	r0, r6
	mov	r1, r3
	bl	MVC_ClearAll
	mvn	r0, #0
	b	.L3785
.L3799:
	movw	r2, #6591
	ldr	r1, .L3801+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3785
.L3800:
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, r2
	bne	.L3729
	b	.L3789
.L3727:
	cmp	r3, #0
	beq	.L3732
	b	.L3728
.L3802:
	.align	2
.L3801:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC417
	.word	.LC418
	.word	.LC416
	UNWIND(.fnend)
	.size	MVC_AllocFrameStore, .-MVC_AllocFrameStore
	.align	2
	.global	MVC_DEC_Init
	.type	MVC_DEC_Init, %function
MVC_DEC_Init:
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
	ldr	r8, .L3815
	movw	r2, #18392
	ldr	r4, [r0, #928]
	mov	r6, r0
	mov	r5, r1
	movt	r2, 170
	mov	r1, #0
	ldr	r3, [r8, #48]
	blx	r3
	str	r4, [r6, #928]
	add	r0, r6, #584
	mov	r4, #2240
	bl	ResetVoQueue
	str	r5, [r6, #224]
	mov	r3, #0
	str	r3, [r6, #228]
	strb	r3, [r6]
	ldr	r3, [r5, #28]
	cmp	r3, #25
	ldreq	r3, [r5, #684]
	movne	r3, #32
	movne	r2, #256
	strne	r3, [r6, #36]
	movne	r3, #136
	streq	r3, [r6, #36]
	ldreq	r3, [r5, #688]
	streq	r3, [r6, #40]
	ldreq	r3, [r5, #676]
	str	r3, [r6, #32]
	strne	r2, [r6, #40]
	ldr	r0, [r5, #584]
	bl	MEM_Phy2Vir
	ldr	ip, [r6, #36]
	ldr	r3, [r6, #224]
	movw	r2, #3992
	ldr	lr, [r6, #40]
	mul	r2, r2, ip
	ldr	r3, [r3, #580]
	add	r3, r0, r3
	str	r3, [r6, #248]
	add	r0, r3, r2
	str	r0, [r6, #252]
	cmp	r3, #0
	cmpne	r0, #0
	mla	r0, r4, lr, r0
	moveq	r1, #1
	movne	r1, #0
	cmp	r0, #0
	orreq	r1, r1, #1
	cmp	r1, #0
	str	r0, [r6, #544]
	bne	.L3812
	ldr	r5, [r5, #608]
	cmp	r5, #0
	beq	.L3813
.L3808:
	add	r3, r6, #11075584
	movw	r2, #4060
	add	r1, r3, #36864
	add	r3, r3, #32768
	ldr	r9, [r8, #48]
	add	r4, r6, #12992
	str	lr, [r1, #1744]
	mov	r1, #0
	str	ip, [r3, #2568]
	mov	r5, r1
	ldr	r3, [r6, #32]
	add	r4, r4, #12
	movw	r7, #22860
	movt	r7, 164
	add	r7, r6, r7
	mul	r2, r2, r3
	blx	r9
	ldr	r3, [r6, #108]
	mov	r2, #9856
	mov	lr, #40
	mov	ip, #25
	str	lr, [r6, #48]
	strb	r5, [r6, #4]
	ubfx	r3, r3, #16, #2
	strb	r5, [r6, #6]
	mov	r1, r5
	strb	r3, [r6, #8]
	movt	r2, 164
	strb	r5, [r6, #7]
	mov	r3, #18
	str	ip, [r6, #56]
	mov	r0, r4
	str	r3, [r6, #52]
	str	r5, [r6, #68]
	ldr	r3, [r8, #48]
	str	r5, [r6, #72]
	str	r5, [r6, #76]
	str	r5, [r6, #80]
	str	r5, [r6, #84]
	str	r5, [r6, #88]
	str	r5, [r6, #92]
	str	r5, [r6, #96]
	str	r5, [r6, #60]
	str	r5, [r6, #528]
	str	r5, [r6, #532]
	str	r5, [r6, #536]
	str	r5, [r6, #540]
	str	r5, [r6, #220]
	blx	r3
	movw	r2, #8500
	ldr	r3, [r8, #48]
	movt	r2, 5
	mov	r0, r7
	mov	r1, r5
	blx	r3
	mov	r3, r4
	mov	r2, r5
.L3809:
	strb	r2, [r3]
	add	ip, r3, #274432
	add	r3, r3, #335872
	mov	r4, #0
	add	r3, r3, #308
	strb	r2, [ip, #241]
	cmp	r3, r7
	bne	.L3809
	add	r2, r6, #12288
	movw	r3, #34592
	mov	r1, #32
	movt	r3, 169
	strb	r4, [r2, #704]
	add	r3, r6, r3
	mov	r0, r6
	str	r3, [r6, #236]
	str	r4, [r6, #20]
	strb	r4, [r6, #2]
	str	r1, [r6, #24]
	str	r1, [r6, #28]
	bl	VCTRL_GetChanIDByCtx
	cmn	r0, #1
	str	r0, [r6, #120]
	beq	.L3814
	mov	r1, r4
	mov	r0, r6
	bl	MVC_ClearAll
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3813:
	mov	r0, r3
	mov	r1, r5
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r2, [r6, #40]
	mov	r1, r5
	ldr	r3, [r8, #48]
	ldr	r0, [r6, #252]
	mul	r2, r4, r2
	blx	r3
	ldr	lr, [r6, #40]
	ldr	ip, [r6, #36]
	ldr	r0, [r6, #544]
	b	.L3808
.L3812:
	movw	r3, #13884
	ldr	r2, .L3815+4
	ldr	r1, .L3815+8
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3814:
	mov	r0, r4
	ldr	r1, .L3815+12
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3816:
	.align	2
.L3815:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+332
	.word	.LC419
	.word	.LC420
	UNWIND(.fnend)
	.size	MVC_DEC_Init, .-MVC_DEC_Init
	.align	2
	.global	MVC_StorePicInDpb
	.type	MVC_StorePicInDpb, %function
MVC_StorePicInDpb:
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
	add	r4, r0, #11075584
	add	r7, r0, #11141120
	add	r2, r4, #36864
	add	r6, r7, #12288
	mov	r8, #0
	strb	r8, [r2, #3960]
	mov	r5, r0
	ldrb	r3, [r6, #2779]	@ zero_extendqisi2
	sub	r3, r3, #2
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r2, #3961]
	ldrb	r3, [r6, #2776]	@ zero_extendqisi2
	cmp	r3, r8
	beq	.L3980
	ldr	r3, [r0, #128]
	str	r3, [r0, #124]
	ldrb	r3, [r6, #2789]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3981
.L3820:
	ldrb	r0, [r5, #8]	@ zero_extendqisi2
	cmp	r0, #2
	beq	.L3982
	ldr	r8, [r5, #224]
	ldr	r3, [r8, #12]
	cmp	r3, #0
	ble	.L3983
.L3825:
	ldrb	r2, [r6, #2778]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3984
.L3826:
	cmp	r3, #2
	ldr	r3, [r6, #3432]
	beq	.L3985
.L3851:
	cmp	r3, #0
	beq	.L3858
	ldr	r3, [r5, #112]
	ldr	r2, [r6, #3468]
	cmp	r3, r2
	bcs	.L3859
	ldrb	r1, [r6, #2778]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L3986
.L3859:
	mov	r0, r5
	bl	MVC_Marking
	cmp	r0, #0
	bne	.L3987
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	ldr	r1, [r6, #3496]
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
.L3852:
	ldrb	r3, [r6, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3862
	add	r4, r4, #45056
	ldr	ip, [r4, #2856]
.L3863:
	ldr	r3, [r6, #3432]
	cmp	r3, #0
	beq	.L3872
	ldr	r2, [r4, #2864]
	ldr	r3, [r4, #2868]
	add	r1, r3, r2
	cmp	r1, ip
	bcs	.L3897
.L3872:
	mvn	r9, #0
	mov	r8, r9
	b	.L3881
.L3876:
	ldr	r3, [r6, #3432]
	mov	r1, r8
	mov	r0, r5
	mov	r9, r2
	cmp	r3, #0
	bne	.L3877
	ldr	r2, [r6, #3436]
	ldr	r3, [fp, #-56]
	cmp	r2, r3
	blt	.L3988
.L3877:
	bl	MVC_OutputFrmFromDPB
	cmn	r0, #1
	mov	r10, r0
	beq	.L3880
	ldr	ip, [r4, #2856]
.L3881:
	ldr	lr, [r4, #2860]
	sub	r3, fp, #52
	sub	r2, fp, #56
	mvn	r1, #0
	cmp	lr, ip
	mov	r0, r5
	bcc	.L3874
	bl	MVC_GetMinPOC
	cmp	r0, #0
	blt	.L3989
	ldr	r2, [r4, #2860]
	cmp	r2, r9
	ldrne	r8, [fp, #-52]
	bne	.L3876
	ldr	r3, [fp, #-52]
	cmp	r3, r8
	beq	.L3990
	mov	r8, r3
	b	.L3876
.L3981:
	ldr	r3, [r6, #3420]
	ldrb	r9, [r3, #1]	@ zero_extendqisi2
	cmp	r9, #0
	beq	.L3821
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L3820
	mov	r0, r8
	ldr	r1, .L4006
	bl	dprint_vfmw
	movw	r3, #3592
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L3984:
	ldrb	r1, [r6, #2779]	@ zero_extendqisi2
	ldr	r10, [r6, #3416]
	sub	r2, r1, #1
	cmp	r2, #1
	movhi	r2, #0
	movls	r2, #1
	cmp	r1, #0
	beq	.L3827
	ldrb	r0, [r10, #2]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	ip, r2
	orrne	ip, r2, #1
	cmp	ip, #0
	beq	.L3826
	cmp	r2, #0
	beq	.L3991
.L3900:
	ldr	r2, [r5, #220]
	cmp	r2, #0
	bne	.L3830
	add	r1, r6, #2768
	mov	r3, #1
	add	r1, r1, #8
	str	r3, [r5, #220]
	mov	r2, #728
	add	r0, r6, #3504
	bl	memcpy
	ldr	r3, [r8, #12]
	b	.L3826
.L3858:
	ldrb	r3, [r6, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3862
	add	r4, r4, #45056
	ldr	ip, [r4, #2856]
	b	.L3872
.L3827:
	cmp	r2, #0
	bne	.L3900
.L3833:
	cmp	r3, #0
	ble	.L3903
	movw	r3, #15064
	mov	r8, r10
	movt	r3, 170
	add	r3, r5, r3
.L3836:
	cmp	r1, #0
	beq	.L3838
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3992
.L3838:
	ldr	r2, [r3, #692]
	str	r2, [r8, #44]
.L3839:
	mov	r1, r8
	mov	r0, r5
	str	r3, [fp, #-64]
	bl	MVC_GetImagePara
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetFsImagePtr
	ldr	r3, [fp, #-64]
	subs	r9, r0, #0
	beq	.L3993
	add	lr, r3, #624
	add	r1, r3, #640
	add	r0, r9, #608
	add	ip, r9, #624
	ldrd	r2, [lr, #-8]
	strd	r2, [r0, #-8]
	ldrd	r2, [lr]
	strd	r2, [r0]
	ldrd	r2, [r1, #-8]
	strd	r2, [ip, #-8]
	ldr	r2, [r8, #44]
	cmp	r2, #0
	str	r2, [r9, #160]
	beq	.L3846
	ldr	r3, [r9, #108]
	ands	r3, r3, #3
	beq	.L3994
.L3844:
	ldr	r3, [r5, #224]
	ldr	r3, [r3, #4]
	cmp	r3, r2
	bcc	.L3995
.L3846:
	mov	r2, #1
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
	ldr	r0, [r5, #120]
	add	r3, r5, #584
	str	r9, [sp]
	mov	r2, r5
	mov	r1, #15
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L3843
	mov	r2, #0
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
.L3843:
	ldr	r3, [r5, #224]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r9, #24]
	bcs	.L3996
.L3849:
	ldr	r3, [r5, #136]
	add	r3, r3, #2
	str	r3, [r5, #136]
.L3837:
	ldr	r3, [r5, #220]
	cmp	r3, #2
	beq	.L3850
.L3835:
	mov	r3, #1
	str	r3, [r8, #16]
	strb	r3, [r10, #8]
.L3850:
	ldr	r3, [r5, #224]
	mov	r2, #2
	str	r2, [r5, #220]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	ldr	r3, [r6, #3432]
	bne	.L3851
.L3985:
	cmp	r3, #0
	beq	.L3852
	add	r4, r4, #45056
	mov	r0, r5
	bl	MVC_SimpleSlideDPB
	ldr	r1, [r4, #2856]
	cmp	r1, #0
	beq	.L3853
	ldr	r3, [r4, #2664]
	cmp	r3, #0
	beq	.L3905
	movw	r2, #47720
	mov	r3, #0
	movt	r2, 169
	add	r2, r5, r2
	b	.L3854
.L3855:
	ldr	r0, [r2, #4]!
	cmp	r0, #0
	beq	.L3905
.L3854:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3855
.L3853:
	movw	r2, #15064
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	subs	r8, r0, #0
	bne	.L3997
	ldr	r1, [r4, #2860]
	mov	r0, r5
	add	r1, r1, #1
	str	r1, [r4, #2860]
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	mov	r2, r8
	movw	r3, #3773
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3857
.L3862:
	ldr	r3, [r5, #520]
	ldr	lr, [r6, #3416]
	cmp	r3, #0
	rsb	r2, r3, lr
	clz	r2, r2
	mov	r2, r2, lsr #5
	moveq	r2, #0
	cmp	r2, #0
	beq	.L3998
	mov	r0, r5
	bl	MVC_DirectOutput
	cmn	r0, #1
	mov	r4, r0
	beq	.L3999
.L3868:
	movw	r3, #3825
	mov	r2, r4
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3824
.L3989:
	ldr	r1, .L4006+8
	movw	r2, #3875
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
.L3874:
	ldr	r3, [r6, #3432]
	cmp	r3, #0
	beq	.L3886
	ldrb	r3, [r6, #2780]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3886
	ldr	r2, [r4, #2864]
	cmp	r2, #0
	beq	.L3886
	movw	ip, #47780
	mov	lr, r3
	movt	ip, 169
	add	ip, r5, ip
	b	.L3891
.L3888:
	ldr	r2, [r4, #2864]
	cmp	r2, r3
	bls	.L3886
.L3891:
	ldr	r2, [ip, #4]!
	add	r3, r3, #1
	ldr	r1, [r6, #3424]
	ldr	r0, [r2, #20]
	cmp	r0, r1
	bne	.L3888
	ldr	r0, [r2, #56]
	ldr	r1, [r6, #3496]
	cmp	r0, r1
	bne	.L3888
	strb	lr, [r2, #3]
	ldr	r1, [ip]
	ldrb	r2, [r1, #5]	@ zero_extendqisi2
	cmp	r2, #1
	streqb	r2, [r1, #7]
	ldreq	r2, [ip]
	streqb	lr, [r2, #5]
	ldreq	r1, [ip]
	ldr	r0, [r5, #52]
	ldr	r2, [r1, #48]
	cmp	r2, r0
	add	r2, r5, r2, lsl #2
	strne	lr, [r2, #148]
	strne	r0, [r1, #48]
	b	.L3888
.L3886:
	ldr	r1, [r4, #2856]
	cmp	r1, #0
	beq	.L3885
	ldr	r3, [r4, #2664]
	cmp	r3, #0
	beq	.L3909
	movw	r2, #47720
	mov	r3, #0
	movt	r2, 169
	add	r2, r5, r2
	b	.L3892
.L3893:
	ldr	r0, [r2, #4]!
	cmp	r0, #0
	beq	.L3909
.L3892:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3893
.L3885:
	movw	r2, #15064
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	cmp	r0, #0
	bne	.L4000
	ldr	r3, [r4, #2860]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #2860]
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
.L3857:
	mov	r4, #0
.L3895:
	add	r7, r7, #12288
	mov	r3, #0
	mov	r0, r4
	strb	r3, [r7, #2776]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3821:
	mvn	r1, #0
	bl	MVC_FlushDPB
	cmp	r0, #0
	beq	.L3820
	mov	r0, r9
	ldr	r1, .L4006+12
	bl	dprint_vfmw
	movw	r3, #3602
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L3983:
	ldr	r2, [r5, #220]
	cmp	r2, #2
	bne	.L3825
	b	.L3826
.L3998:
	add	r4, r4, #45056
	ldr	ip, [r4, #2856]
	cmp	ip, #0
	beq	.L3865
	ldr	r3, [r4, #2664]
	adds	r1, r3, #0
	movne	r1, #1
	cmp	lr, r3
	movne	r1, #0
	cmp	r1, #0
	movweq	r2, #47720
	movteq	r2, 169
	addeq	r2, r5, r2
	bne	.L4001
.L3869:
	add	r1, r1, #1
	cmp	r1, ip
	beq	.L3863
	ldr	r3, [r2, #4]!
	adds	r0, r3, #0
	movne	r0, #1
	cmp	lr, r3
	movne	r0, #0
	cmp	r0, #0
	beq	.L3869
.L3866:
	movw	r2, #15064
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	cmn	r0, #1
	mov	r4, r0
	beq	.L4002
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	movw	r3, #3851
	mov	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3857
.L3991:
	cmp	r0, #0
	beq	.L3826
	b	.L3833
.L3909:
	mov	r1, r3
	b	.L3885
.L3880:
	mov	r3, r0
	movw	r2, #3915
	ldr	r1, .L4006+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, r10
	movw	r3, #3916
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
.L3899:
	ldr	r3, [r6, #3416]
	mov	r2, #1
	ldr	r0, [r5, #120]
	mvn	r4, #0
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	mov	r3, r4
	movw	r2, #3974
	ldr	r1, .L4006+16
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r6, #3416]
	mov	r3, #0
	strb	r3, [r2, #5]
	ldr	r2, [r6, #3416]
	strb	r3, [r2, #2]
	b	.L3895
.L3982:
	ldr	r1, .L4006+20
	bl	dprint_vfmw
	mov	r0, r5
	bl	MVC_DirectOutput
	ldr	r3, [r6, #3492]
	ldr	r2, [r5, #52]
	cmp	r3, r2
	addne	r3, r3, #36
	movne	r1, #0
	addne	r3, r5, r3, lsl #2
	mov	r4, r0
	strne	r1, [r3, #4]
	strne	r2, [r6, #3492]
	movw	r3, #3621
	mov	r2, r0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
.L3824:
	cmn	r4, #1
	bne	.L3895
	b	.L3899
.L3830:
	cmp	r2, #1
	beq	.L4003
	cmp	r1, #0
	beq	.L3833
	ldrb	r2, [r10, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3833
	b	.L3826
.L3988:
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L4004
.L3878:
	mov	r0, r5
	bl	MVC_DirectOutput
	cmn	r0, #1
	mov	r4, r0
	beq	.L4005
.L3879:
	movw	r3, #3907
	mov	r2, r4
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3824
.L3903:
	mov	r8, r10
	b	.L3837
.L3990:
	mov	r3, r8
	ldr	r2, .L4006+24
	ldr	r1, .L4006+28
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3884
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L3996:
	mov	r1, r9
	mov	r0, r5
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3849
.L3905:
	mov	r1, r3
	b	.L3853
.L4004:
	ldrb	r3, [r6, #2779]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r6, #3416]
	strne	r3, [r5, #524]
	b	.L3878
.L4003:
	cmp	r3, #0
	add	r3, r7, #16384
	ldr	r8, [r3, #48]
	ble	.L3835
	movw	r3, #15792
	ldrb	r1, [r6, #3507]	@ zero_extendqisi2
	movt	r3, 170
	add	r3, r5, r3
	b	.L3836
.L3994:
	movw	r2, #3702
	ldr	r1, .L4006+32
	mov	r0, #1
	str	r3, [fp, #-64]
	bl	dprint_vfmw
	ldr	r2, .L4006+36
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3845
	ldr	r3, [fp, #-64]
	mov	r1, #111
	ldr	r0, [r5, #120]
	mov	r2, r3
	blx	ip
.L3845:
	ldr	r2, [r9, #160]
	cmp	r2, #0
	bne	.L3844
	b	.L3846
.L3992:
	mov	r1, #3
	ldr	r2, [r8, #44]
	strb	r1, [r8, #2]
	ldr	r1, [r3, #692]
	add	r2, r2, r1
	str	r2, [r8, #44]
	b	.L3839
.L3980:
	mov	r0, r3
	ldr	r1, .L4006+40
	bl	dprint_vfmw
	movw	r3, #3578
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L4005:
	mov	r3, r0
	mov	r2, #3904
	ldr	r1, .L4006+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3879
.L3987:
	mov	r2, r0
	ldr	r1, .L4006+44
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3798
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L4000:
	mov	r3, r0
	movw	r2, #3960
	ldr	r1, .L4006+16
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3961
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L4001:
	mov	r1, r2
	b	.L3866
.L3865:
	ldr	r3, [r6, #3432]
	cmp	r3, #0
	beq	.L3872
	ldr	r2, [r4, #2864]
	ldr	r3, [r4, #2868]
.L3897:
	str	r3, [sp, #4]
	mov	r0, #1
	mov	r3, ip
	str	r2, [sp]
	ldr	r1, .L4006+48
	movw	r2, #3864
	bl	dprint_vfmw
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
	movw	r3, #3867
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L3986:
	ldr	r1, .L4006+52
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4006+36
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3860
	ldr	r4, [r6, #3468]
	mov	r3, #8
	ldr	lr, [r5, #112]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r5, #120]
	str	r4, [fp, #-52]
	str	lr, [fp, #-48]
	blx	ip
.L3860:
	mov	r0, r5
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
	movw	r3, #3789
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L3999:
	mov	r3, r0
	movw	r2, #3821
	ldr	r1, .L4006+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3868
.L3997:
	mov	r3, r8
	movw	r2, #3764
	ldr	r1, .L4006+16
	mov	r0, #0
	bl	dprint_vfmw
	movw	r3, #3765
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L4002:
	mov	r3, r0
	movw	r2, #3844
	ldr	r1, .L4006+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, r4
	movw	r3, #3845
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L3995:
	ldrsb	r1, [r8, #6]
	mov	r2, #0
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
	movw	r3, #3721
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L3993:
	movw	r3, #3691
	mvn	r2, #0
	ldr	r1, .L4006+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3899
.L4007:
	.align	2
.L4006:
	.word	.LC423
	.word	.LC422
	.word	.LC431
	.word	.LC424
	.word	.LC427
	.word	.LC425
	.word	.LANCHOR0+348
	.word	.LC432
	.word	.LC426
	.word	g_event_report
	.word	.LC421
	.word	.LC429
	.word	.LC430
	.word	.LC428
	UNWIND(.fnend)
	.size	MVC_StorePicInDpb, .-MVC_StorePicInDpb
	.global	__aeabi_idivmod
	.align	2
	.global	MVC_DecGap
	.type	MVC_DecGap, %function
MVC_DecGap:
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
	ldrb	r1, [r0, #3]	@ zero_extendqisi2
	add	r3, r0, #11075584
	movw	lr, #2004
	add	r7, r3, #40960
	mov	r5, r0
	mul	lr, lr, r1
	ldr	r2, [r7, #2644]
	ldr	r1, [r0, #236]
	add	r0, r0, #11141120
	mov	ip, r3
	str	r3, [fp, #-80]
	movw	r3, #26878
	mov	r4, r0
	movt	r3, 42
	str	r0, [fp, #-76]
	add	r3, r2, r3
	add	r0, r5, lr
	movw	r2, #43664
	add	r8, ip, #36864
	movt	r2, 169
	add	r2, r0, r2
	ldrb	r0, [r1, #19]	@ zero_extendqisi2
	add	r6, r5, r3, lsl #2
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	movw	r3, #43664
	cmp	r0, #1
	add	r4, r4, #12288
	add	ip, ip, #45056
	movt	r3, 169
	str	r2, [fp, #-72]
	add	r3, lr, r3
	ldr	r2, [r7, #1028]
	add	r3, r5, r3
	ldr	lr, [r6, #4]
	str	r2, [fp, #-52]
	ldr	r2, [r8, #4000]
	str	r2, [fp, #-64]
	moveq	r2, #2
	movne	r2, #0
	str	r2, [fp, #-56]
	ldr	r2, [r8, #4004]
	str	r2, [fp, #-68]
	ldr	r2, [r1, #2896]
	str	r3, [r4, #3420]
	ldr	r6, [ip, #2860]
	cmp	r6, #0
	beq	.L4023
	add	r2, r2, #4
	mov	r6, #1
	mov	r3, r6, asl r2
	add	r0, lr, r6
	str	r3, [fp, #-60]
	mov	r1, r3
	bl	__aeabi_uidivmod
	mov	r3, #0
	mov	r0, #2
	str	r3, [r8, #4004]
	str	r3, [r8, #4000]
	mov	r10, r1
	ldr	r1, .L4026
	bl	dprint_vfmw
	ldr	r9, [fp, #-52]
	mov	r3, r10
	ldr	r1, .L4026+4
	mov	r0, #2
	mov	r2, r9
	bl	dprint_vfmw
	cmp	r9, r10
	mov	r3, r9
	movgt	r3, r4
	movgt	r4, r10
	movgt	r10, r3
	bgt	.L4016
	b	.L4017
.L4014:
	strb	r9, [r10, #2780]
	mov	r0, r5
	bl	MVC_StorePicInDpb
	movw	lr, #26878
	movt	lr, 42
	ldr	r1, [fp, #-60]
	subs	r3, r0, #0
	add	r0, r4, #1
	bne	.L4024
	ldr	r3, [r7, #2644]
	add	lr, r3, lr
	add	lr, r5, lr, lsl #2
	str	r4, [lr, #4]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-52]
	cmp	r3, r1
	mov	r4, r1
	ble	.L4017
.L4016:
	mov	r1, #1
	mov	r0, r5
	bl	MVC_AllocFrameStore
	mov	r9, #0
	ldr	r1, .L4026+8
	subs	r3, r0, #0
	mov	r0, #2
	bne	.L4025
	str	r3, [fp, #-48]
	bl	dprint_vfmw
	ldr	r1, [r10, #3416]
	strb	r6, [r10, #2782]
	ldr	r3, [fp, #-56]
	str	r4, [r1, #20]
	ldr	r1, [r10, #3416]
	str	r4, [r10, #3424]
	str	r4, [r1, #708]
	str	r3, [r10, #3432]
	ldr	r1, [r10, #3416]
	ldr	r3, [fp, #-48]
	str	r3, [r1, #704]
	ldr	r1, [r10, #3416]
	strb	r6, [r1, #7]
	ldr	r1, [r10, #3416]
	strb	r3, [r1, #5]
	ldr	r1, [r10, #3416]
	strb	r6, [r1, #1]
	strb	r6, [r10, #2778]
	ldr	r1, [r10, #3420]
	ldr	r0, [r7, #2640]
	str	r0, [r10, #3496]
	strb	r3, [r1, #3]
	ldr	r3, [r5, #236]
	ldr	r3, [r3, #2900]
	cmp	r3, #0
	beq	.L4014
	str	r4, [r8, #4016]
	mov	r0, r5
	bl	MVC_DecPOC
	ldr	r3, [r8, #3996]
	str	r3, [r10, #3436]
	ldr	r3, [r8, #3992]
	str	r3, [r10, #3440]
	ldr	r3, [r8, #3984]
	str	r3, [r10, #3444]
	ldr	r3, [r8, #3988]
	str	r3, [r10, #3448]
	b	.L4014
.L4023:
	ldr	r1, .L4026+12
	mov	r0, r6
	bl	dprint_vfmw
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4017:
	ldr	r2, [fp, #-64]
	mov	r0, #0
	ldr	r3, [fp, #-76]
	str	r2, [r8, #4000]
	add	r3, r3, #12288
	ldr	r2, [fp, #-68]
	str	r2, [r8, #4004]
	ldrb	r2, [fp, #-72]	@ zero_extendqisi2
	ldr	r3, [r3, #3420]
	strb	r2, [r3, #3]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4025:
	ldr	r2, [fp, #-64]
	mov	r0, r9
	ldr	r3, [fp, #-76]
	ldr	r1, .L4026+16
	str	r2, [r8, #4000]
	add	r3, r3, #12288
	ldr	r2, [fp, #-68]
	str	r2, [r8, #4004]
	ldrb	r2, [fp, #-72]	@ zero_extendqisi2
	ldr	r3, [r3, #3420]
	strb	r2, [r3, #3]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4024:
	ldr	r2, [fp, #-80]
	mov	r0, r9
	ldr	ip, [fp, #-64]
	add	r1, r2, #36864
	ldrb	lr, [fp, #-72]	@ zero_extendqisi2
	movw	r2, #6891
	str	ip, [r1, #4000]
	ldr	ip, [fp, #-68]
	str	ip, [r1, #4004]
	ldr	ip, [r10, #3420]
	ldr	r1, .L4026+20
	strb	lr, [ip, #3]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4027:
	.align	2
.L4026:
	.word	.LC434
	.word	.LC435
	.word	.LC437
	.word	.LC433
	.word	.LC436
	.word	.LC438
	UNWIND(.fnend)
	.size	MVC_DecGap, .-MVC_DecGap
	.align	2
	.global	MVC_InitPic
	.type	MVC_InitPic, %function
MVC_InitPic:
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
	add	r3, r0, #11141120
	add	r5, r3, #12288
	add	r8, r0, #11075584
	add	r6, r8, #40960
	mov	r4, r0
	str	r3, [fp, #-56]
	mov	r1, #0
	ldr	r3, [r0, #252]
	mov	r9, #2240
	ldrb	r0, [r5, #2776]	@ zero_extendqisi2
	ldr	r2, [r6, #1016]
	cmp	r0, r1
	str	r1, [fp, #-48]
	mla	r9, r9, r2, r3
	beq	.L4029
	ldr	r3, [r5, #3416]
	cmp	r3, r1
	beq	.L4030
	ldrb	r7, [r3, #2]	@ zero_extendqisi2
	cmp	r7, r1
	beq	.L4188
.L4030:
	mov	r3, #0
	strb	r3, [r5, #2776]
.L4029:
	sub	r1, fp, #48
	mov	r0, r4
	bl	MVC_GetReRangeFlag
	ldrb	r3, [r6, #1012]	@ zero_extendqisi2
	cmp	r3, #255
	mov	r10, r0
	beq	.L4189
.L4031:
	cmp	r3, #0
	bne	.L4034
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L4190
	ldr	r3, [r9, #28]
	ldr	r2, [r4, #28]
	cmp	r2, r3
	movweq	r3, #35364
	movteq	r3, 168
	addeq	r3, r4, r3
	streq	r3, [r4, #236]
	bne	.L4191
.L4034:
	add	r7, r8, #36864
	ldr	r3, [r9, #24]
	ldr	r2, [r7, #1744]
	cmp	r2, r3
	beq	.L4192
.L4038:
	mov	r3, #0
	mov	r1, r9
	strb	r3, [r9, #20]
	ldr	r0, [r4, #236]
	bl	mvc_assign_quant_params
	ldr	r3, .L4206
	movw	r0, #38584
	mov	r2, #2240
	movt	r0, 169
	mov	r1, r9
	add	r0, r4, r0
	ldr	r3, [r3, #56]
	blx	r3
.L4039:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r4, #224]
	bne	.L4193
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L4041
.L4050:
	ldr	r2, [r7, #1744]
	ldr	r3, [r9, #24]
	cmp	r2, r3
	bne	.L4059
	ldrb	r3, [r9, #20]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4059
.L4060:
	ldrb	r3, [r6, #1012]	@ zero_extendqisi2
	cmp	r3, #255
	moveq	r3, #0
	streq	r3, [r6, #2644]
	beq	.L4062
	cmp	r3, #0
	bne	.L4062
	add	r1, r4, #10747904
	movw	r2, #22868
	add	r1, r1, #20480
	movt	r2, 164
	ldr	ip, [r6, #2640]
	add	r2, r4, r2
	ldr	r0, [r1, #2384]
	adds	r0, r0, #1
	beq	.L4063
	ldr	r1, [r1, #2388]
	cmp	ip, r1
	beq	.L4104
	mov	lr, r0
	b	.L4064
.L4065:
	ldr	r1, [r2, #4]!
	cmp	ip, r1
	beq	.L4063
.L4064:
	add	r3, r3, #1
	cmp	r3, lr
	mov	r0, r3
	bne	.L4065
.L4063:
	str	r0, [r6, #2644]
.L4062:
	ldrb	r2, [r6, #1003]	@ zero_extendqisi2
	ldr	r3, [r4, #236]
	cmp	r2, #5
	ldreq	r2, [r6, #2644]
	ldr	r9, [r3, #2896]
	movweq	r3, #26878
	movteq	r3, 42
	addeq	r3, r2, r3
	ldreq	r10, [r6, #1028]
	addeq	r3, r4, r3, lsl #2
	ldrne	r10, [r6, #1028]
	streq	r10, [r3, #4]
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L4068
	ldr	r2, [r4, #224]
	ldr	r3, [r2, #12]
	str	r2, [fp, #-60]
	cmp	r3, #2
	beq	.L4068
	ldr	r1, [r6, #2644]
	movw	r3, #26878
	movt	r3, 42
	add	r3, r1, r3
	add	r3, r4, r3, lsl #2
	ldr	r0, [r3, #4]
	cmp	r0, r10
	beq	.L4068
	mov	r3, #1
	add	r9, r9, #4
	mov	r9, r3, asl r9
	add	r0, r0, r3
	str	r3, [fp, #-64]
	mov	r1, r9
	bl	__aeabi_uidivmod
	cmp	r1, r10
	beq	.L4068
	ldr	r3, [fp, #-64]
	ldr	r2, [fp, #-60]
	str	r3, [r2, #600]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #600]
	cmp	r2, #0
	beq	.L4194
.L4069:
	add	r3, r8, #32768
	ldrb	r3, [r3, #1843]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4078
	ldr	r1, .L4206+4
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #604]
	bics	r3, r3, #1024
	bne	.L4185
	ldr	r2, [r6, #2640]
	cmp	r2, #0
	bne	.L4185
	ldr	r0, [r6, #2644]
	movw	r3, #26878
	movt	r3, 42
	mov	r1, r9
	add	r3, r0, r3
	str	r2, [fp, #-60]
	ldr	r10, [r6, #1028]
	add	r3, r4, r3, lsl #2
	ldr	r0, [r3, #4]
	add	r0, r0, #1
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-60]
	cmp	r10, r1
	bcs	.L4068
	ldrb	r3, [r6, #1000]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L4068
	mov	r0, r4
	str	r2, [fp, #-56]
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r3, [r6, #2644]
	movw	r0, #26878
	mov	r1, r9
	movt	r0, 42
	add	r0, r3, r0
	ldr	r5, [r6, #1028]
	add	r0, r4, r0, lsl #2
	ldr	r0, [r0, #4]
	add	r0, r0, #1
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-56]
	mov	r3, r5
	mov	r0, r2
	movw	r2, #7417
	str	r1, [sp]
	ldr	r1, .L4206+8
	bl	dprint_vfmw
	mvn	r0, #0
.L4176:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4193:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L4041
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L4195
.L4041:
	ldr	r3, [r4, #84]
	cmp	r10, #0
	add	r3, r3, #1
	str	r3, [r4, #84]
	bne	.L4051
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L4052
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	cmp	r0, #0
	bne	.L4196
.L4184:
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	bne	.L4102
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	bne	.L4050
.L4056:
	ldr	r1, [r7, #1700]
	add	ip, r8, #32768
	add	r3, r4, #48
	mov	r0, r4
	mov	r1, r1, asl #1
	str	r1, [r4, #52]
	ldr	r2, [r7, #1680]
	ldrb	r10, [ip, #1844]	@ zero_extendqisi2
	add	r2, r2, #1
	ldr	ip, [r7, #1676]
	rsb	r10, r10, #2
	str	r1, [sp]
	mov	r2, r2, asl #4
	add	ip, ip, #1
	mul	r10, r10, r2
	mov	ip, ip, asl #4
	str	ip, [fp, #-60]
	mov	r1, ip
	mov	r2, r10
	bl	MVC_ArrangeVahbMem
	ldr	ip, [fp, #-60]
	cmp	r0, #1
	beq	.L4197
.L4182:
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	mvneq	r0, #1
	beq	.L4176
	mov	r5, #0
	ldr	r1, .L4206+12
	str	r5, [r4, #12]
	mov	r0, r5
	str	r5, [r4, #16]
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4059:
	ldr	r3, .L4206
	movw	r0, #38584
	mov	r2, #0
	movt	r0, 169
	strb	r2, [r9, #20]
	add	r0, r4, r0
	mov	r1, r9
	ldr	r3, [r3, #56]
	mov	r2, #2240
	blx	r3
	b	.L4060
.L4078:
	mov	r0, r4
	bl	MVC_DecGap
	cmp	r0, #0
	bne	.L4081
.L4185:
	ldr	r10, [r6, #1028]
.L4068:
	ldrb	r3, [r6, #1008]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4082
	ldr	r2, [r6, #2644]
	movw	r3, #26878
	movt	r3, 42
	add	r3, r2, r3
	add	r3, r4, r3, lsl #2
	str	r10, [r3, #4]
.L4082:
	mov	r0, r4
	str	r10, [r7, #4016]
	bl	MVC_DecPOC
	mov	r1, #0
	mov	r0, r4
	bl	MVC_AllocFrameStore
	cmp	r0, #0
	bne	.L4198
	strb	r0, [r5, #2783]
	mov	r1, #1
	strb	r1, [r5, #2776]
	ldrb	r3, [r6, #1003]	@ zero_extendqisi2
	strb	r3, [r5, #2782]
	ldr	r10, [r6, #1028]
	str	r10, [r5, #3424]
	ldrb	r9, [r6, #1008]	@ zero_extendqisi2
	str	r9, [r5, #3432]
	ldr	ip, [r7, #3996]
	str	ip, [r5, #3436]
	ldr	r3, [r7, #3992]
	str	r3, [r5, #3440]
	ldr	r2, [r7, #3984]
	str	r2, [r5, #3444]
	ldr	lr, [r7, #3988]
	str	r3, [r5, #3456]
	mvn	r3, #0
	str	ip, [r5, #3452]
	str	lr, [r5, #3464]
	str	r2, [r5, #3460]
	str	lr, [r5, #3448]
	strb	r0, [r5, #2778]
	strb	r0, [r5, #2781]
	strb	r0, [r5, #2780]
	str	r3, [r5, #3488]
	ldrb	r3, [r6, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	strneb	r1, [r6, #1001]
	ldr	r2, [r4, #236]
	movne	r3, r1
	movne	r1, #2
	str	r3, [fp, #-64]
	ldrb	r0, [r2, #20]	@ zero_extendqisi2
	ldr	ip, [r2, #3952]
	rsb	r0, r0, #2
	str	r2, [fp, #-60]
	mla	r0, ip, r0, r0
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-60]
	ldr	r3, [fp, #-64]
	cmp	r3, #0
	str	r0, [r5, #3476]
	ldrb	r1, [r2, #20]	@ zero_extendqisi2
	ldr	ip, [r2, #3952]
	rsb	r3, r1, #2
	mla	r3, ip, r3, r3
	str	r3, [r5, #3480]
	ldr	r1, [r2, #3948]
	add	r3, r1, #1
	str	r3, [r5, #3472]
	mul	r3, r3, r0
	str	r3, [r5, #3484]
	ldrb	r3, [r7, #1720]	@ zero_extendqisi2
	strb	r3, [r5, #2784]
	movne	r3, #0
	bne	.L4085
	ldrb	r3, [r2, #21]	@ zero_extendqisi2
	adds	r3, r3, #0
	movne	r3, #1
.L4085:
	cmp	r9, #0
	strb	r3, [r5, #2785]
	beq	.L4086
	ldr	r2, [r6, #2644]
	movw	r3, #26878
	movt	r3, 42
	add	r3, r2, r3
	add	r3, r4, r3, lsl #2
	str	r10, [r3, #4]
.L4086:
	ldr	r3, [r5, #3416]
	cmp	r3, #0
	ldrneb	r2, [r6, #1012]	@ zero_extendqisi2
	strneb	r2, [r3, #9]
	mvn	r3, #0
	str	r3, [r5, #3496]
	ldrb	r3, [r6, #1011]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4088
	ldr	r3, [r6, #2640]
	str	r3, [r5, #3496]
	ldrb	r3, [r6, #1009]	@ zero_extendqisi2
	strb	r3, [r5, #2786]
	ldrb	r3, [r6, #1010]	@ zero_extendqisi2
	strb	r3, [r5, #2787]
.L4088:
	ldr	r3, [r6, #2644]
	movw	r0, #2004
	movw	ip, #43664
	ldr	r2, [fp, #-56]
	movt	ip, 169
	movw	r7, #15696
	str	r3, [r5, #3500]
	add	lr, r2, #15680
	ldrb	r10, [r4, #3]	@ zero_extendqisi2
	movt	r7, 170
	add	r7, r4, r7
	movw	r9, #15696
	movt	r9, 170
	ldr	r1, .L4206+16
	mla	r10, r0, r10, r4
	mov	r0, #29
	add	r8, r8, #32768
	add	ip, r10, ip
	str	ip, [r5, #3420]
	ldr	ip, [r4, #224]
	ldrd	r2, [ip, #56]
	strd	r2, [lr]
	ldrd	r2, [ip, #72]
	strd	r2, [r7, #-8]
	ldrd	r2, [ip, #88]
	strd	r2, [r4, r9]
	mvn	r2, #0
	mvn	r3, #0
	strd	r2, [ip, #56]
	ldrd	r2, [lr]
	bl	dprint_vfmw
	ldrd	r2, [r7, #-8]
	ldr	r1, .L4206+20
	mov	r0, #29
	bl	dprint_vfmw
	ldr	ip, [r4, #236]
	movw	lr, #8864
	movw	r0, #15080
	movt	r0, 170
	add	r0, r4, r0
	ldr	r3, [ip, #3948]
	add	r3, r3, #1
	mov	r3, r3, asl #4
	str	r3, [r5, #2904]
	ldrb	r2, [ip, #20]	@ zero_extendqisi2
	ldr	r1, [ip, #3952]
	rsb	r3, r2, #2
	mla	r3, r1, r3, r3
	mov	r3, r3, asl #4
	str	r3, [r5, #2908]
	ldr	r1, [r8, #1876]
	str	lr, [r5, #2900]
	ldr	r2, [ip, #56]
	and	r1, r1, #7
	ldrb	r3, [ip, #52]	@ zero_extendqisi2
	ldr	ip, [ip, #748]
	mov	r2, r2, asl #5
	subs	ip, ip, #1
	uxtb	r2, r2
	movne	ip, #1
	orr	r3, r2, r3, asl #14
	orr	r3, r3, ip, asl #2
	str	r3, [r5, #2900]
	bl	SetAspectRatio
	ldr	r0, [r5, #3416]
	ldr	r3, [r4, #84]
	cmp	r0, #0
	str	r3, [r5, #2956]
	beq	.L4199
	ldr	r3, [r4, #236]
	ldr	r3, [r3, #3984]
	str	r3, [r0, #200]
	str	r3, [r5, #2920]
	ldr	r3, [r4, #236]
	ldr	r2, [r5, #3416]
	ldr	r3, [r3, #3988]
	str	r3, [r2, #204]
	str	r3, [r5, #2924]
	ldr	r3, [r4, #236]
	ldr	r2, [r5, #3416]
	ldr	r3, [r3, #3976]
	str	r3, [r2, #192]
	str	r3, [r5, #2912]
	ldr	r2, [r4, #236]
	ldr	r3, [r5, #3416]
	ldr	r2, [r2, #3980]
	str	r2, [r3, #196]
	ldrb	r3, [r5, #2779]	@ zero_extendqisi2
	ldr	r1, [r5, #3416]
	ldr	r0, [r5, #2900]
	adds	r3, r3, #0
	str	r2, [r5, #2916]
	movne	r3, #1
	str	r3, [r5, #2888]
	str	r0, [r1, #180]
	ldr	r2, [r5, #2888]
	ldr	r3, [r5, #3416]
	str	r2, [r3, #168]
	ldr	r3, [r6, #1028]
	ldr	r2, [r5, #3416]
	str	r3, [r5, #2976]
	ldr	r1, [r6, #2640]
	str	r1, [r5, #2984]
	str	r3, [r2, #256]
	ldr	r3, [r5, #3416]
	ldr	r2, [r6, #2640]
	str	r2, [r3, #264]
	ldr	r3, [r5, #3416]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetDispPhyFs
	cmp	r0, #0
	beq	.L4090
	ldr	r3, [r5, #2888]
	mov	r1, #0
	ldr	r2, [r0, #8]
	subs	r3, r3, r1
	ldr	r0, [r4, #120]
	movne	r3, #1
	bl	FSP_SetStoreType
.L4090:
	movw	ip, #15632
	movw	lr, #15664
	ldr	r3, [r4, #60]
	movt	ip, 170
	movt	lr, 170
	add	ip, r4, ip
	add	lr, r4, lr
	mov	r6, #0
.L4094:
	ldrd	r0, [ip, #8]!
	orrs	r2, r0, r1
	bne	.L4091
	cmp	r3, #0
	beq	.L4091
	ldr	r2, [r4, #528]
	subs	r3, r3, #1
	stmia	ip, {r2, r6}
	beq	.L4092
	add	r1, r4, #528
	mov	r2, #0
.L4093:
	add	r2, r2, #1
	ldr	r0, [r1, #4]!
	cmp	r2, r3
	str	r0, [r1, #-4]
	bne	.L4093
.L4092:
	add	r2, r3, #132
	str	r6, [r4, r2, asl #2]
	str	r3, [r4, #60]
.L4091:
	cmp	ip, lr
	bne	.L4094
	cmp	r3, #0
	beq	.L4096
	mov	r6, r3
	ble	.L4096
	add	r7, r3, #132
	mov	r8, #0
	add	r7, r4, r7, lsl #2
.L4097:
	ldr	r1, [r7, #-4]!
	sub	r6, r6, #1
	ldr	r0, [r4, #120]
	bl	FreeUsdByDec
	cmp	r6, #0
	str	r8, [r7]
	bne	.L4097
.L4096:
	mov	r6, #0
	mov	r0, r4
	str	r6, [r4, #60]
	strb	r6, [r5, #2788]
	bl	MVC_UpdateReflist
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	mov	r0, r4
	bl	MVC_CalcPicNum
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4192:
	ldrb	r3, [r9, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4039
	b	.L4038
.L4189:
	mov	r2, #1
	strb	r2, [r4, #2]
	ldr	r2, [r9, #28]
	movw	ip, #3992
	ldr	r0, [r4, #248]
	add	r1, r8, #32768
	movw	r7, #34592
	str	r2, [r4, #24]
	ldr	r2, [r9, #28]
	ldr	r1, [r1, #2568]
	mla	r2, ip, r2, r0
	ldr	r0, [r2, #744]
	cmp	r1, r0
	beq	.L4200
.L4032:
	ldr	r3, .L4206
	movt	r7, 169
	ldr	r0, [r4, #84]
	add	r7, r4, r7
	mov	ip, #0
	mov	r1, r2
	add	r0, r0, #1
	str	r0, [r4, #84]
	strb	ip, [r2, #26]
	mov	r0, r7
	ldr	r3, [r3, #56]
	movw	r2, #3992
	blx	r3
	ldrb	r3, [r6, #1012]	@ zero_extendqisi2
.L4033:
	str	r7, [r4, #236]
	b	.L4031
.L4052:
	ldr	r1, .L4206+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_GetBackPicFromVOQueue
	b	.L4184
.L4200:
	ldrb	r1, [r2, #26]	@ zero_extendqisi2
	cmp	r1, #0
	movteq	r7, 169
	addeq	r7, r4, r7
	beq	.L4033
	b	.L4032
.L4188:
	ldrsb	r1, [r3, #6]
	mov	r2, #1
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
	ldr	r3, [r5, #3416]
	strb	r7, [r3, #2]
	ldr	r3, [r5, #3416]
	strb	r7, [r3, #5]
	b	.L4030
.L4194:
	ldr	r2, [r3, #640]
	cmp	r2, #1
	beq	.L4201
.L4070:
	ldr	r3, [r6, #2644]
	ldr	r1, [r6, #1028]
	add	r3, r4, r3, lsl #2
	add	r3, r3, #11075584
	add	r2, r3, #40960
	ldr	r2, [r2, #1020]
	cmp	r1, r2
	bls	.L4071
	ldr	ip, [r7, #1700]
	rsb	r0, r2, r1
	cmp	r2, #0
	cmpne	ip, r0
	bcs	.L4069
	ldrb	r2, [r6, #1000]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L4202
	add	r3, r3, #40960
	cmp	r1, #0
	subne	r2, r1, #1
	subeq	r2, r9, #1
	mvn	r1, #0
	str	r2, [r3, #1020]
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r3, r0, #0
	beq	.L4069
	movw	r2, #7363
	ldr	r1, .L4206+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4195:
	ldr	r3, [r4, #84]
	cmp	r10, #0
	add	r3, r3, #1
	str	r3, [r4, #84]
	beq	.L4042
	mvn	r1, #0
	mov	r0, r4
	str	r2, [fp, #-60]
	bl	MVC_FlushDPB
	ldr	r2, [fp, #-60]
	subs	r3, r0, #0
	bne	.L4203
	mov	r0, r4
	bl	MVC_InitDPB
	subs	r3, r0, #0
	bne	.L4099
	add	r2, r8, #32768
	ldr	r3, [r7, #1680]
	ldr	r10, [r7, #1676]
	ldrb	ip, [r2, #1844]	@ zero_extendqisi2
	add	r3, r3, #1
	ldr	r2, [r4, #12]
	add	r10, r10, #1
	mov	r3, r3, asl #4
	rsb	ip, ip, #2
	cmp	r2, #0
	mov	r10, r10, asl #4
	mul	ip, ip, r3
	bne	.L4048
	ldr	r3, [r7, #1700]
	ldr	r0, [r4, #224]
	mov	r3, r3, asl #1
	str	r3, [r4, #52]
	ldr	r1, [r0, #668]
	str	r3, [fp, #-60]
	cmp	r1, r10
	bcc	.L4044
	ldr	r2, [r0, #672]
	cmp	r2, ip
	bcc	.L4044
	add	lr, r8, #45056
	ldr	r3, [r0, #692]
	ldr	lr, [lr, #2856]
	cmp	r3, lr
	bcc	.L4204
	ldr	lr, [r0, #696]
	str	ip, [fp, #-64]
	cmp	lr, #0
	moveq	r3, #1
	streq	r3, [r4, #52]
	streq	r3, [fp, #-60]
	ldr	r3, [fp, #-60]
	ldreq	r1, [r0, #668]
	ldreq	r2, [r0, #672]
	mov	r0, r4
	str	r3, [sp]
	add	r3, r4, #48
	bl	MVC_ArrangeVahbMem
	ldr	ip, [fp, #-64]
	cmp	r0, #1
	bne	.L4182
	b	.L4048
.L4190:
	strb	r3, [r4, #2]
	movw	r1, #8500
	ldr	r3, [r9, #28]
	movt	r1, 5
	mov	r2, r1
	ldr	ip, .L4206
	movw	r0, #22860
	mla	r1, r1, r3, r4
	movt	r0, 164
	ldr	r3, [ip, #56]
	add	r0, r4, r0
	add	r1, r1, #12992
	add	r1, r1, #12
	blx	r3
	add	r2, r4, #11010048
	add	r2, r2, #32768
	movw	r3, #35364
	movt	r3, 168
	add	r3, r4, r3
	str	r3, [r4, #236]
	ldr	r3, [r2, #3340]
	str	r3, [r4, #28]
	b	.L4034
.L4051:
	ldr	r1, .L4206+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_GetBackPicFromVOQueue
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L4056
.L4102:
	mov	r3, r0
	movw	r2, #7252
	ldr	r1, .L4206+32
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4197:
	mov	ip, ip, lsr #4
	mov	r10, r10, lsr #4
	str	ip, [r4, #12]
	str	r10, [r4, #16]
	b	.L4050
.L4071:
	rsb	r0, r2, r1
	ldr	ip, [r7, #1700]
	add	r0, r0, r9
	cmp	r2, #0
	cmpne	ip, r0
	bcs	.L4069
	ldrb	r2, [r6, #1000]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L4205
	add	r3, r3, #40960
	cmp	r1, #0
	subne	r2, r1, #1
	subeq	r2, r9, #1
	mvn	r1, #0
	str	r2, [r3, #1020]
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r3, r0, #0
	beq	.L4069
	movw	r2, #7390
	ldr	r1, .L4206+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4042:
	mov	r0, r4
	bl	MVC_InitDPB
	subs	r3, r0, #0
	bne	.L4099
	ldr	r3, [r7, #1680]
	add	r2, r8, #32768
	ldr	r10, [r7, #1676]
	add	r3, r3, #1
	ldrb	ip, [r2, #1844]	@ zero_extendqisi2
	add	r10, r10, #1
	mov	r3, r3, asl #4
	rsb	ip, ip, #2
	mov	r10, r10, asl #4
	mul	ip, ip, r3
.L4048:
	mov	r10, r10, lsr #4
	mov	ip, ip, lsr #4
	str	r10, [r4, #12]
	str	ip, [r4, #16]
	b	.L4050
.L4191:
	ldr	r1, .L4206+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4081:
	movw	r2, #7444
	ldr	r1, .L4206+40
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4201:
	ldr	r2, [r3, #636]
	cmp	r2, #0
	bne	.L4070
	ldr	r2, [r3, #644]
	cmp	r2, #0
	bne	.L4070
	ldr	r3, [r3, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	bhi	.L4069
	b	.L4070
.L4104:
	mov	r0, r3
	b	.L4063
.L4198:
	movw	r2, #7464
	ldr	r1, .L4206+44
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4199:
	movw	r2, #7572
	ldr	r1, .L4206+48
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4196:
	mov	r3, r0
	movw	r2, #7243
	mov	r0, r10
	ldr	r1, .L4206+28
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4202:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r2, [r6, #2644]
	ldr	r3, [r7, #1700]
	movw	r1, #26878
	ldr	ip, [r6, #1028]
	movt	r1, 42
	add	r1, r2, r1
	movw	r2, #7352
.L4186:
	add	r4, r4, r1, lsl #2
	str	r3, [sp]
	ldr	r1, .L4206+52
	mov	r0, #0
	ldr	r3, [r4, #4]
	rsb	r3, r3, ip
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4099:
	mov	r2, #7168
	ldr	r1, .L4206+32
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4205:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r2, [r6, #2644]
	movw	r1, #26878
	ldr	r3, [r7, #1700]
	movt	r1, 42
	ldr	ip, [r6, #1028]
	add	r1, r2, r1
	movw	r2, #7379
	b	.L4186
.L4044:
	ldr	r1, .L4206+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4203:
	mov	r0, r2
	ldr	r1, .L4206+28
	movw	r2, #7159
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4204:
	ldr	r1, .L4206+60
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4176
.L4207:
	.align	2
.L4206:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC445
	.word	.LC446
	.word	.LC443
	.word	.LC449
	.word	.LC450
	.word	.LC452
	.word	.LC439
	.word	.LC440
	.word	.LC30
	.word	.LC447
	.word	.LC448
	.word	.LC451
	.word	.LC444
	.word	.LC441
	.word	.LC442
	UNWIND(.fnend)
	.size	MVC_InitPic, .-MVC_InitPic
	.align	2
	.global	MVC_DecSlice
	.type	MVC_DecSlice, %function
MVC_DecSlice:
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
	ldr	r2, [r0, #68]
	mov	r5, r0
	ldr	r1, .L4260
	sub	r2, r2, #1
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, [r5, #232]
	add	r4, r5, #11075584
	mov	r0, r5
	add	r4, r4, #40960
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r4, #1003]
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	strb	r3, [r4, #1008]
	bl	MVC_ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L4253
	mov	r0, r5
	bl	MVC_ProcessSliceHeaderSecondPart
	cmp	r0, #0
	bne	.L4253
	ldr	r2, [r5, #64]
	cmp	r2, #0
	moveq	r3, r2
	beq	.L4215
	mov	r1, r0
	movw	r8, #16544
	movt	r8, 170
	mov	r3, r1
	add	r8, r5, r8
	mov	r6, r1
	b	.L4223
.L4254:
	cmp	lr, r1
	add	r3, r7, r3
	rsbhi	r1, r1, lr
	rsbhi	r0, r0, r9
	addhi	r1, r1, r0
	movls	r1, r9
	add	r3, r3, r1
.L4221:
	cmp	ip, #0
	add	r6, r6, #1
	movne	r1, ip
	movne	r0, r7
	moveq	r1, lr
	moveq	r0, r9
	cmp	r6, r2
	beq	.L4215
.L4223:
	ldr	r7, [r8, #4]!
	ldr	lr, [r7, #8]
	ldr	ip, [r7, #36]
	cmp	lr, #0
	ldrne	r9, [r7, #12]
	moveq	r9, lr
	cmp	ip, #0
	ldrne	r7, [r7, #40]
	moveq	r7, ip
	cmp	r1, #0
	bne	.L4254
	add	r3, r9, r3
	add	r3, r3, r7
	b	.L4221
.L4215:
	add	r6, r5, #11141120
	str	r3, [r5, #104]
	add	r6, r6, #12288
	movw	r0, #1620
	ldr	ip, [r6, #3476]
	ldr	r1, [r6, #3472]
	mul	r1, r1, ip
	cmp	r1, r0
	movgt	r1, r1, asl #7
	ble	.L4255
.L4225:
	ldr	r0, [r5, #32]
	cmp	r0, r2
	bls	.L4226
	cmp	r1, r3
	bcc	.L4226
	ldrb	r3, [r4, #1005]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4256
.L4229:
	mov	r0, r5
	bl	MVC_PicTypeStatistic
	mov	r0, r5
	bl	MVC_DecList
	cmp	r0, #0
	bne	.L4257
	mov	r0, r5
	bl	MVC_IsRefListWrong
	subs	r4, r0, #0
	bne	.L4258
	mov	r0, r5
	bl	MVC_CalcStreamBits
	mov	r0, r5
	bl	MVC_WriteSliceMsg
	mov	r0, r5
	bl	MVC_ExitSlice
	ldr	r3, [r5, #100]
	mov	r0, r4
	mov	r2, #1
	add	r3, r3, r2
	strb	r2, [r5, #4]
	str	r3, [r5, #100]
.L4212:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4255:
	mov	r0, r1, asl #9
	sub	r1, r0, r1, asl #7
	cmp	r1, #16384
	movlt	r1, #16384
	b	.L4225
.L4256:
	ldr	r1, .L4260+4
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r3, [r5, #88]
	mov	r2, #0
	str	r2, [r5, #100]
	add	r3, r3, #1
	str	r3, [r5, #88]
	ldrb	r3, [r6, #2779]	@ zero_extendqisi2
	mov	r0, r5
	cmp	r3, r2
	ldreq	r3, [r5, #92]
	ldrne	r3, [r5, #96]
	addeq	r3, r3, #1
	addne	r3, r3, #1
	streq	r3, [r5, #92]
	strne	r3, [r5, #96]
	bl	MVC_InitPic
	cmn	r0, #2
	beq	.L4212
	cmp	r0, #0
	bne	.L4259
	mov	r0, r5
	bl	MVC_WritePicMsg
	b	.L4229
.L4253:
	ldr	r1, .L4260+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4260+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4251
	sub	r2, fp, #36
	mvn	r1, #0
	mov	r3, #4
	str	r1, [r2, #-8]!
	mov	r1, #100
	ldr	r0, [r5, #120]
	blx	ip
.L4251:
	mov	r0, r5
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	b	.L4212
.L4226:
	str	r1, [sp]
	mov	r0, #0
	ldr	r1, .L4260+16
	bl	dprint_vfmw
	ldr	r3, [r5, #64]
	ldr	r2, [r5, #32]
	cmp	r3, r2
	bcc	.L4228
	ldr	r1, .L4260+12
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L4228
	str	r3, [fp, #-44]
	mov	r1, #108
	str	r2, [fp, #-40]
	mov	r3, #8
	sub	r2, fp, #44
	ldr	r0, [r5, #120]
	blx	ip
.L4228:
	ldr	r1, .L4260+20
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L4212
.L4257:
	mov	r2, r0
	ldr	r1, .L4260+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4212
.L4258:
	ldr	r1, .L4260+28
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4251
.L4259:
	ldr	r1, .L4260+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4212
.L4261:
	.align	2
.L4260:
	.word	.LC392
	.word	.LC455
	.word	.LC393
	.word	g_event_report
	.word	.LC453
	.word	.LC454
	.word	.LC457
	.word	.LC458
	.word	.LC456
	UNWIND(.fnend)
	.size	MVC_DecSlice, .-MVC_DecSlice
	.align	2
	.global	MVC_DecOneNal
	.type	MVC_DecOneNal, %function
MVC_DecOneNal:
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
	ldr	lr, [r0, #232]
	mov	r4, r0
	mov	r5, r1
	ldr	r3, [lr, #68]
	cmp	r3, #0
	ldrne	ip, [r0, #104]
	movne	r2, lr
	movne	r3, #0
	beq	.L4267
.L4266:
	ldr	r0, [r2, #12]
	add	r3, r3, #1
	add	r2, r2, #28
	add	ip, ip, r0
	str	ip, [r4, #104]
	ldr	r0, [lr, #68]
	cmp	r0, r3
	bhi	.L4266
.L4267:
	add	r7, r4, #548
	mov	r8, #0
	mov	r1, #32
	strb	r8, [lr]
	mov	r0, r7
	bl	BsGet
	ldr	r3, [r4, #232]
	mvn	r1, #0
	and	ip, r0, #31
	strb	ip, [r3, #2]
	ldr	r3, [r4, #232]
	mov	r6, r0
	ubfx	r0, r0, #5, #2
	ubfx	r2, r6, #7, #1
	strb	r0, [r3, #4]
	ldr	r3, [r4, #232]
	str	r2, [r3, #76]
	ldr	r2, [r4, #232]
	ldr	r3, [r2, #64]
	add	r3, r3, #32
	str	r3, [r2, #64]
	ldr	r3, [r4, #232]
	strb	r8, [r4, #10]
	strb	r1, [r3, #5]
	ldr	r3, [r4, #232]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #14
	cmpne	r3, #20
	beq	.L4448
.L4265:
	ldr	r2, [r4, #68]
	mov	r0, #22
	ldr	r1, .L4459
	bl	dprint_vfmw
	ldr	r3, [r4, #68]
	movw	r2, #371
	cmp	r3, r2
	beq	.L4449
.L4270:
	ldr	r2, [r4, #232]
	add	r3, r3, #1
	str	r3, [r4, #68]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	sub	r3, r2, #1
	cmp	r3, #29
	ldrls	pc, [pc, r3, asl #2]
	b	.L4271
.L4273:
	.word	.L4272
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4272
	.word	.L4274
	.word	.L4275
	.word	.L4276
	.word	.L4277
	.word	.L4278
	.word	.L4279
	.word	.L4280
	.word	.L4281
	.word	.L4282
	.word	.L4283
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4284
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4271
	.word	.L4285
.L4448:
	mov	r1, #24
	mov	r0, r7
	bl	BsGet
	ldr	r3, [r4, #232]
	ubfx	r2, r0, #23, #1
	strb	r2, [r3, #5]
	ldr	r3, [r4, #232]
	mov	r6, r0
	ldrsb	r2, [r3, #5]
	cmp	r2, #0
	bne	.L4268
	add	r2, r4, #12288
	mov	r1, #1
	ubfx	r0, r0, #16, #6
	ubfx	ip, r6, #2, #1
	strb	r1, [r2, #704]
	ubfx	lr, r6, #6, #10
	ubfx	r1, r6, #3, #3
	strb	r0, [r2, #706]
	strb	r1, [r2, #707]
	ubfx	r0, r6, #1, #1
	strb	ip, [r2, #708]
	and	r1, r6, #1
	str	lr, [r2, #712]
	ubfx	ip, r6, #22, #1
	strb	r0, [r2, #709]
	strb	r1, [r2, #710]
	strb	ip, [r2, #705]
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	cmp	r2, #20
	beq	.L4450
.L4268:
	ldr	r2, [r3, #64]
	add	r2, r2, #24
	str	r2, [r3, #64]
	ldr	r3, [r4, #232]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	b	.L4265
.L4272:
	ldr	r3, [r4, #80]
	mov	r0, #22
	ldr	r1, .L4459+4
	bl	dprint_vfmw
	ldr	r3, [r4, #232]
	add	r0, r4, #11075584
	add	r2, r4, #12288
	add	r0, r0, #40960
	mvn	lr, #0
	ldrb	ip, [r3, #2]	@ zero_extendqisi2
	mov	r1, #0
	strb	ip, [r0, #1003]
	ldrb	r5, [r3, #4]	@ zero_extendqisi2
	strb	r5, [r0, #1008]
	ldrb	r3, [r3, #5]	@ zero_extendqisi2
	str	lr, [r0, #2640]
	strb	r1, [r0, #1011]
	strb	r3, [r0, #1012]
	ldrb	lr, [r2, #704]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L4451
	cmp	r3, #255
	beq	.L4452
.L4287:
	ldr	r2, [r4, #88]
	mov	r0, #21
	ldr	r1, .L4459+8
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_SliceCheck
	subs	r5, r0, #0
	mov	r0, r4
	bne	.L4453
	bl	MVC_DecSlice
	cmn	r0, #2
	beq	.L4298
	cmp	r0, #0
	beq	.L4429
	ldr	r3, .L4459+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4428
	ldr	r0, [r4, #120]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L4428:
	mvn	r0, #0
	b	.L4298
.L4274:
	ldr	r1, .L4459+16
	mov	r2, #6
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecSEI
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4305
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4305:
	cmp	r5, #0
	bne	.L4454
.L4429:
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L4275:
	ldr	r1, .L4459+20
	mov	r2, #7
	ldr	r3, [r4, #72]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecSPS
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4303
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4303:
	cmp	r5, #0
	bne	.L4455
	ldr	r3, [r4, #72]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #72]
	b	.L4298
.L4277:
	ldr	r1, .L4459+24
	mov	r2, #9
.L4436:
	mov	r0, #22
	bl	dprint_vfmw
.L4441:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4429
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #232]
.L4298:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L4278:
	mov	r2, #10
	ldr	r1, .L4459+28
	b	.L4436
.L4276:
	ldr	r1, .L4459+32
	mov	r2, #8
	ldr	r3, [r4, #76]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecPPS
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4301
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4301:
	cmp	r5, #0
	bne	.L4456
	ldr	r3, [r4, #76]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #76]
	b	.L4298
.L4279:
	mov	r2, #11
	ldr	r1, .L4459+36
	mov	r0, #22
	bl	dprint_vfmw
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
	bne	.L4441
	ldr	r5, .L4459+12
	mov	r3, #1
	strb	r3, [r4, #1]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4308
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L4308:
	mov	r6, #0
	mov	r0, r4
	strb	r6, [r4]
	bl	MVC_DecVDM
	cmp	r0, r6
	beq	.L4441
	b	.L4444
.L4280:
	mov	r2, #12
	ldr	r1, .L4459+40
	b	.L4436
.L4281:
	mov	r2, #13
	ldr	r1, .L4459+44
	b	.L4436
.L4282:
	mov	r2, #14
	ldr	r1, .L4459+48
	b	.L4436
.L4283:
	ldr	r1, .L4459+52
	mov	r2, #15
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecSubSPS
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4309
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4309:
	cmp	r5, #0
	bne	.L4457
	ldr	r3, [r4, #20]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #20]
	b	.L4298
.L4284:
	mov	r2, #19
	ldr	r1, .L4459+56
	b	.L4436
.L4285:
	mov	r2, #30
	ldr	r1, .L4459+60
	mov	r0, #22
	bl	dprint_vfmw
	cmp	r5, #0
	bne	.L4311
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4311
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	str	r5, [r4, #232]
.L4311:
	mov	r1, #32
	mov	r0, r7
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r7
	bl	BsGet
	ldr	r3, .L4459+64
	rev	r5, r5
	ldr	r2, [r3, #440]
	ldr	r3, [r3, #436]
	rev	r0, r0
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L4429
	ldr	r5, .L4459+12
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4312
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L4312:
	mov	r6, #0
	mov	r0, r4
	strb	r6, [r4]
	bl	MVC_DecVDM
	cmp	r0, r6
	beq	.L4429
.L4444:
	ldr	ip, [r5]
	cmp	ip, r6
	beq	.L4428
	ldr	r0, [r4, #120]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4298
.L4271:
	mov	r3, r6
	ldr	r1, .L4459+68
	mov	r0, #22
	bl	dprint_vfmw
	cmp	r6, #256
	beq	.L4313
	ldr	r1, .L4459+72
	mov	r0, #1
	bl	dprint_vfmw
.L4313:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4314
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4314:
	ldr	r3, .L4459+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4429
.L4433:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L4429
.L4449:
	ldr	r1, .L4459+76
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, [r4, #68]
	b	.L4270
.L4450:
	cmp	ip, #0
	movne	r2, #1
	moveq	r2, #5
	strb	r2, [r3, #2]
	ldr	r3, [r4, #232]
	b	.L4268
.L4453:
	bl	MVC_ClearCurrSlice
	ldr	r3, .L4459+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4428
.L4297:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4298
.L4456:
	ldr	r5, .L4459+12
	mov	r0, #1
	ldr	r1, .L4459+80
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4428
.L4446:
	sub	r2, fp, #36
	mvn	r6, #0
	mov	r3, #4
	mov	r1, #100
	str	r6, [r2, #-4]!
	ldr	r0, [r4, #120]
	blx	ip
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4297
	b	.L4428
.L4457:
	ldr	r1, .L4459+84
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4459+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4428
	sub	r2, fp, #36
	ldr	r0, [r4, #120]
	mov	r3, #4
	mvn	r4, #0
	mov	r1, #100
	str	r4, [r2, #-4]!
	blx	ip
	mov	r0, r4
	b	.L4298
.L4455:
	ldr	r5, .L4459+12
	mov	r0, #1
	ldr	r1, .L4459+88
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4446
	b	.L4428
.L4454:
	ldr	r1, .L4459+92
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4459+12
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L4433
	b	.L4429
.L4452:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq	.L4287
	add	r3, r4, #10747904
	add	r3, r3, #20480
	ldr	r2, [r3, #2384]
	cmp	r2, #0
	beq	.L4290
	ldrb	r2, [r3, #2380]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L4291
.L4290:
	add	r3, r4, #12992
	mov	r2, #0
	add	r3, r3, #16
.L4294:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L4292
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L4458
.L4292:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L4294
	mvn	r3, #0
.L4427:
	cmn	r3, #1
	str	r3, [r0, #2640]
	beq	.L4287
	sub	ip, ip, #5
	mov	r3, #1
	clz	ip, ip
	strb	r3, [r0, #1011]
	strb	r3, [r0, #1010]
	mov	ip, ip, lsr #5
	strb	ip, [r0, #1009]
	b	.L4287
.L4451:
	strb	lr, [r0, #1011]
	ldr	r3, [r2, #712]
	str	r3, [r0, #2640]
	ldrb	r3, [r2, #708]	@ zero_extendqisi2
	strb	r3, [r0, #1009]
	ldrb	r3, [r2, #709]	@ zero_extendqisi2
	strb	r3, [r0, #1010]
	strb	r1, [r2, #704]
	b	.L4287
.L4458:
	movw	r3, #8500
	movt	r3, 5
	mla	r3, r3, r2, r4
	add	r3, r3, #12992
	add	r3, r3, #16
	ldr	r3, [r3, #4]
	b	.L4427
.L4291:
	ldr	r3, [r3, #2388]
	b	.L4427
.L4460:
	.align	2
.L4459:
	.word	.LC459
	.word	.LC461
	.word	.LC462
	.word	g_event_report
	.word	.LC467
	.word	.LC465
	.word	.LC469
	.word	.LC470
	.word	.LC463
	.word	.LC472
	.word	.LC471
	.word	.LC473
	.word	.LC474
	.word	.LC475
	.word	.LC476
	.word	.LC477
	.word	.LANCHOR1
	.word	.LC478
	.word	.LC479
	.word	.LC460
	.word	.LC464
	.word	.LC334
	.word	.LC466
	.word	.LC468
	UNWIND(.fnend)
	.size	MVC_DecOneNal, .-MVC_DecOneNal
	.align	2
	.global	MVC_DEC_DecodePacket
	.type	MVC_DEC_DecodePacket, %function
MVC_DEC_DecodePacket:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
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
	cmp	r1, #0
	mov	ip, #0
	mov	r4, r0
	str	ip, [fp, #-40]
	str	ip, [fp, #-44]
	str	ip, [fp, #-48]
	beq	.L4524
	ldr	r2, [r0, #224]
	ldrb	lr, [r0, #5]	@ zero_extendqisi2
	ldr	r3, [r2, #8]
	cmp	lr, #1
	mov	r3, r3, asl #16
	and	r3, r3, #196608
	str	r3, [r0, #108]
	ldr	r3, [r2]
	str	r3, [r0, #112]
	ldr	r3, [r2, #4]
	strb	ip, [r0, #10]
	str	r3, [r0, #116]
	beq	.L4464
	ldr	r3, [r2, #832]
	cmp	r3, #1
	addne	r5, r0, #11141120
	addne	r5, r5, #16384
	beq	.L4525
.L4465:
	mov	r0, r4
	bl	MVC_ReceivePacket
	ldr	r3, [r4, #232]
	str	r3, [r5, #1996]
.L4467:
	cmp	r0, #0
	mov	r3, #0
	strb	r3, [r4, #5]
	beq	.L4468
.L4487:
	mov	r0, #0
.L4519:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L4525:
	ldr	r3, [r2, #872]
	add	r5, r0, #11141120
	add	r5, r5, #16384
	cmp	r3, #1
	bne	.L4465
	ldr	r3, [r5, #1992]
	cmp	r3, #1
	bne	.L4465
	ldr	r3, [r5, #1996]
	str	r3, [r0, #232]
	str	ip, [r5, #1992]
	strb	ip, [r0, #5]
	b	.L4466
.L4468:
	ldr	r3, [r4, #232]
.L4466:
	mov	r2, #0
	str	r2, [r3, #64]
	ldr	r3, [r4, #232]
	add	r6, r4, #548
	mov	r0, r6
	ldr	r2, [r3, #12]
	ldr	r1, [r3, #8]
	bl	BsInit
	mov	r1, #32
	mov	r0, r6
	bl	BsGet
	ldr	r3, [r4, #232]
	mvn	r2, #0
	mov	r5, r0
	and	r0, r0, #31
	strb	r0, [r3, #2]
	ubfx	r1, r5, #5, #2
	ldr	r3, [r4, #232]
	strb	r1, [r3, #4]
	ldr	r3, [r4, #232]
	strb	r2, [r3, #5]
	ldr	r3, [r4, #232]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #20
	beq	.L4526
.L4522:
	and	r5, r5, #27
	mov	r3, #0
	cmp	r5, #1
	strb	r3, [r4, #10]
	beq	.L4473
.L4474:
	mov	r0, r4
	bl	MVC_FindTrailZeros
	cmp	r0, #0
	bne	.L4527
.L4486:
	ldr	r3, [r4, #232]
	mov	r5, #0
	mov	r0, r6
	str	r5, [r3, #64]
	ldr	r3, [r4, #232]
	ldr	r1, [r3, #8]
	ldr	r2, [r3, #12]
	bl	BsInit
	mov	r1, r5
	mov	r0, r4
	bl	MVC_DecOneNal
	cmn	r0, #2
	beq	.L4519
	cmp	r0, r5
	beq	.L4487
	ldr	r3, .L4531
	ldr	ip, [r3]
	cmp	ip, r5
	beq	.L4523
	ldr	r0, [r4, #120]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L4523:
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L4464:
	ldr	r0, [r0, #232]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	b	.L4467
.L4526:
	mov	r1, #24
	mov	r0, r6
	bl	BsGet
	ldr	r3, [r4, #232]
	ubfx	r2, r0, #23, #1
	strb	r2, [r3, #5]
	ldr	r1, [r4, #232]
	ldrsb	r2, [r1, #5]
	cmp	r2, #0
	bne	.L4522
	ubfx	ip, r0, #22, #1
	add	r3, r4, #12288
	cmp	ip, #0
	mov	r5, #1
	strb	ip, [r3, #705]
	ubfx	lr, r0, #6, #10
	ubfx	ip, r0, #2, #1
	strb	r5, [r3, #704]
	strb	ip, [r3, #708]
	ubfx	r0, r0, #1, #1
	str	lr, [r3, #712]
	movne	ip, r5
	strb	r0, [r3, #709]
	moveq	ip, #5
	strb	ip, [r1, #2]
	strb	r2, [r4, #10]
.L4473:
	sub	r2, fp, #32
	sub	r1, fp, #36
	mov	r0, r4
	bl	MVC_InquireSliceProperty
	cmp	r0, #0
	bne	.L4475
	ldr	r3, [fp, #-36]
	ldr	r2, [r4, #128]
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #0
	add	r3, r2, r3
	str	r3, [r4, #128]
	bne	.L4528
.L4476:
	ldr	r3, [fp, #-32]
	cmp	r3, #1
	bne	.L4474
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	cmp	r0, #0
	bne	.L4529
.L4483:
	ldr	r0, [r4, #120]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L4474
	sub	r3, fp, #40
	sub	r2, fp, #44
	sub	r1, fp, #48
	ldr	r0, [r4, #120]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	bne	.L4474
	ldr	r3, [fp, #-40]
	cmp	r3, #0
	ble	.L4530
.L4480:
	mov	r3, #1
	mvn	r0, #1
	strb	r3, [r4, #5]
	b	.L4519
.L4475:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4485
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4485:
	ldr	r3, .L4531
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4523
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4519
.L4527:
	ldr	r1, .L4531+4
	mov	r0, #22
	bl	dprint_vfmw
	b	.L4486
.L4528:
	add	r5, r4, #11075584
	add	r5, r5, #40960
	ldrb	r3, [r5, #1001]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #132]
	addeq	r3, r3, #1
	addne	r3, r3, #2
	str	r3, [r4, #132]
	ldr	r3, [r4, #64]
	cmp	r3, #0
	beq	.L4476
	ldr	r3, [r4, #232]
	mov	ip, #0
	mov	r0, r6
	mov	r2, #12
	ldr	r1, .L4531+8
	str	ip, [r3, #64]
	bl	BsInit
	mov	r1, #1
	mov	r0, r4
	bl	MVC_DecOneNal
	ldrb	r3, [r5, #1001]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #132]
	subeq	r3, r3, #1
	subne	r3, r3, #2
	str	r3, [r4, #132]
	b	.L4480
.L4529:
	mov	r3, #0
	movw	r2, #14134
	ldr	r1, .L4531+12
	mov	r0, #22
	bl	dprint_vfmw
	b	.L4483
.L4524:
	mov	r0, r1
	movw	r3, #14017
	ldr	r2, .L4531+16
	ldr	r1, .L4531+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4519
.L4530:
	ldr	r3, [fp, #-44]
	cmp	r3, #32
	bgt	.L4480
	ldr	r3, [r4, #228]
	cmp	r3, #0
	bne	.L4480
	ldr	r3, [r4, #88]
	cmp	r3, #0
	beq	.L4474
	ldr	r1, [r4, #12]
	cmp	r1, #0
	beq	.L4474
	add	r5, r4, #11141120
	ldr	lr, [r4, #16]
	add	r5, r5, #16384
	ldr	r0, [r5, #2000]
	cmp	lr, r0
	beq	.L4474
	ldr	ip, [r5, #2004]
	cmp	r1, ip
	beq	.L4474
	stmib	sp, {r0, r1, lr}
	mov	r0, r2
	str	ip, [sp]
	ldr	r1, .L4531+24
	bl	dprint_vfmw
	ldr	r2, .L4531
	ldr	r3, [r4, #16]
	ldr	ip, [r2]
	str	r3, [r5, #2000]
	ldr	r3, [r4, #12]
	cmp	ip, #0
	str	r3, [r5, #2004]
	beq	.L4474
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
	b	.L4474
.L4532:
	.align	2
.L4531:
	.word	g_event_report
	.word	.LC481
	.word	.LANCHOR1+432
	.word	.LC439
	.word	.LC13
	.word	.LC14
	.word	.LC480
	UNWIND(.fnend)
	.size	MVC_DEC_DecodePacket, .-MVC_DEC_DecodePacket
	.align	2
	.global	MVC_DEC_VDMPostProc
	.type	MVC_DEC_VDMPostProc, %function
MVC_DEC_VDMPostProc:
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
	ldrb	r7, [r0]	@ zero_extendqisi2
	mov	r4, r0
	cmp	r7, #0
	bne	.L4534
	add	r6, r0, #11141120
	add	r6, r6, #12288
	str	r1, [r6, #3468]
	bl	MVC_ClearAllSlice
	mov	r0, r4
	bl	MVC_StorePicInDpb
	subs	r5, r0, #0
	bne	.L4545
	mov	r3, #1
	mov	r0, r5
	strb	r3, [r4, #4]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4534:
	add	r3, r0, #11141120
	add	r3, r3, #12288
	str	r1, [r3, #3468]
	bl	MVC_ClearAllSlice
	mov	r0, r4
	bl	MVC_StorePicInDpb
	subs	r2, r0, #0
	bne	.L4546
.L4537:
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r2, r0, #0
	bne	.L4547
.L4538:
	ldr	ip, [r4, #40]
	add	r3, r4, #11075584
	add	r3, r3, #40960
	mov	r1, #0
	movw	r2, #23352
	strb	r1, [r4, #4]
	mov	r0, r1
	movt	r2, 1
	str	ip, [r3, #2668]
	mov	r5, #2
	strb	r1, [r3, #2656]
	mov	r4, #7
	str	r1, [r3, #2688]
	mov	lr, #32
	str	r1, [r3, #2692]
	mov	ip, #3
	str	r2, [r3, #2672]
	mvn	r1, #0
	mov	r2, #262144
	strb	r5, [r3, #2657]
	strb	r4, [r3, #2659]
	strb	lr, [r3, #2658]
	str	ip, [r3, #2680]
	str	r1, [r3, #2684]
	str	r2, [r3, #2696]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4545:
	ldr	r3, [r6, #3416]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	mov	r2, r5
	ldr	r1, .L4548
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mov	r0, r4
	mvn	r1, #0
	bl	MVC_ClearDPB
	str	r7, [r6, #3416]
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4547:
	ldr	r1, .L4548+4
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4538
.L4546:
	ldr	r1, .L4548+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4537
.L4549:
	.align	2
.L4548:
	.word	.LC482
	.word	.LC484
	.word	.LC483
	UNWIND(.fnend)
	.size	MVC_DEC_VDMPostProc, .-MVC_DEC_VDMPostProc
	.align	2
	.global	MVC_DEC_GetImageBuffer
	.type	MVC_DEC_GetImageBuffer, %function
MVC_DEC_GetImageBuffer:
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
	ldr	r7, [r0, #48]
	mov	r5, r0
	cmp	r7, #0
	beq	.L4566
	movw	r4, #48280
	movw	r9, #47720
	movt	r4, 169
	add	r8, r0, #11075584
	add	r4, r0, r4
	movt	r9, 169
	add	r8, r8, #45056
	add	r9, r0, r9
	mov	ip, r4
	mov	r6, #0
.L4558:
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4553
	cmp	ip, #0
	beq	.L4554
	ldr	lr, [r8, #2856]
	cmp	lr, #0
	beq	.L4554
	ldr	r2, [r8, #2664]
	rsb	r3, ip, r2
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L4553
	mov	r1, r9
	b	.L4555
.L4556:
	ldr	r2, [r1, #4]!
	rsb	r0, ip, r2
	cmp	r2, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L4553
.L4555:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L4556
.L4554:
	ldr	r0, [r5, #120]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L4566
	ldr	r0, [r5, #120]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	bne	.L4586
	add	r1, r5, #584
	ldr	r0, [r5, #120]
	bl	FSP_ClearNotInVoQueue
	b	.L4586
.L4553:
	add	r6, r6, #1
	add	ip, ip, #808
	cmp	r6, r7
	bne	.L4558
	sub	r2, fp, #40
	sub	r1, fp, #44
	add	r0, r5, #584
	bl	GetQueueImgNum
	ldr	r1, .L4590
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, .L4590+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	cmp	r3, #0
	beq	.L4588
	movw	r7, #47720
	add	r8, r5, #11075584
	movt	r7, 169
	add	r8, r8, #45056
	add	r7, r5, r7
	mov	r6, #0
.L4559:
	cmp	r4, #0
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r9, [r4, #5]	@ zero_extendqisi2
	beq	.L4570
	ldr	lr, [r8, #2856]
	cmp	lr, #0
	beq	.L4570
	ldr	r2, [r8, #2664]
	rsb	r1, r4, r2
	cmp	r2, #0
	clz	r1, r1
	mov	r1, r1, lsr #5
	moveq	r1, #0
	cmp	r1, #0
	bne	.L4572
	mov	r2, r7
	b	.L4563
.L4564:
	ldr	ip, [r2, #4]!
	rsb	r0, r4, ip
	cmp	ip, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L4572
.L4563:
	add	r1, r1, #1
	cmp	r1, lr
	bne	.L4564
.L4570:
	mov	r2, #1
.L4562:
	str	r2, [sp, #4]
	mov	r0, #0
	mov	r2, r6
	str	r9, [sp]
	ldr	r1, .L4590+8
	add	r6, r6, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	add	r4, r4, #808
	cmp	r3, r6
	bhi	.L4559
.L4565:
	ldr	r3, [fp, #-40]
	mov	r0, #0
	ldr	r2, [fp, #-44]
	ldr	r1, .L4590+12
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	ldr	r0, [r8, #2860]
	ldr	r2, [fp, #-44]
	sub	r3, r3, #2
	ldr	r1, [fp, #-40]
	rsb	r3, r0, r3
	add	r2, r2, r1
	cmp	r2, r3
	blt	.L4589
.L4586:
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4566:
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4572:
	mov	r2, #0
	b	.L4562
.L4589:
	ldr	r1, .L4590+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r5
	mov	r1, #1
	bl	MVC_ClearAll
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4588:
	add	r8, r5, #11075584
	add	r8, r8, #45056
	b	.L4565
.L4591:
	.align	2
.L4590:
	.word	.LC485
	.word	.LC486
	.word	.LC488
	.word	.LC487
	.word	.LC489
	UNWIND(.fnend)
	.size	MVC_DEC_GetImageBuffer, .-MVC_DEC_GetImageBuffer
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
	.align	2
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
	.type	__FUNCTION__.15352, %object
	.size	__FUNCTION__.15352, 18
__FUNCTION__.15352:
	.ascii	"MVC_WriteSliceMsg\000"
	.space	2
	.type	MVC_SarTable.15645, %object
	.size	MVC_SarTable.15645, 136
MVC_SarTable.15645:
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
	.type	__func__.15879, %object
	.size	__func__.15879, 11
__func__.15879:
	.ascii	"MVC_DecSEI\000"
	.space	1
	.type	__func__.14532, %object
	.size	__func__.14532, 17
__func__.14532:
	.ascii	"MVC_DirectOutput\000"
	.space	3
	.type	__func__.15112, %object
	.size	__func__.15112, 12
__func__.15112:
	.ascii	"MVC_InitDPB\000"
	.type	__FUNCTION__.14252, %object
	.size	__FUNCTION__.14252, 13
__FUNCTION__.14252:
	.ascii	"MVC_ClearDPB\000"
	.space	3
	.type	__func__.15990, %object
	.size	__func__.15990, 17
__func__.15990:
	.ascii	"MVC_FlushDecoder\000"
	.space	3
	.type	__FUNCTION__.14260, %object
	.size	__FUNCTION__.14260, 13
__FUNCTION__.14260:
	.ascii	"MVC_ClearAll\000"
	.space	3
	.type	__FUNCTION__.16117, %object
	.size	__FUNCTION__.16117, 13
__FUNCTION__.16117:
	.ascii	"MVC_DEC_Init\000"
	.space	3
	.type	__func__.14682, %object
	.size	__func__.14682, 18
__func__.14682:
	.ascii	"MVC_StorePicInDpb\000"
	.data
	.align	2
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
	.space	192
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
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"\012\012END of the bit buffer, copy the first packe" )
	ASCII(.ascii	"t!\012\012\000" )
	.space	2
.LC1:
	ASCII(.ascii	"0:phy:0x%x, 1:phy:0x%x; len0:%d len1:%d\012\000" )
	.space	3
.LC2:
	ASCII(.ascii	"0:phy:0x%x, len0:%d\012\000" )
	.space	3
.LC3:
	ASCII(.ascii	"%-50s%50d\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"MVC_FRAME %d, state=%d, is already output\012\000" )
	.space	1
.LC5:
	ASCII(.ascii	"plfs(fs=%p, eFoState=%d) is null, logic_fs_id = %d\012" )
	ASCII(.ascii	"\000" )
.LC6:
	ASCII(.ascii	"not used MVC_FRAME: (%d,%d)\012\000" )
	.space	3
.LC7:
	ASCII(.ascii	"MVC_FRAME skip: is_used=%d,skip=%d\012\000" )
.LC8:
	ASCII(.ascii	"topfield skip\012\000" )
	.space	1
.LC9:
	ASCII(.ascii	"bottom field skip\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"err(%d) > out_thr(%d)\012\000" )
	.space	1
.LC11:
	ASCII(.ascii	"exg_pts: %lld <-> %lld\012\000" )
.LC12:
	ASCII(.ascii	"stream MVC_FRAME packing type is %d! what can we do" )
	ASCII(.ascii	"?\012\000" )
	.space	2
.LC13:
	ASCII(.ascii	"firmware/common/syntax/mvc.c\000" )
	.space	3
.LC14:
	ASCII(.ascii	"NULL pointer: %s, L%d\012\000" )
	.space	1
.LC15:
	ASCII(.ascii	"framestore %d allocate apc %d\012\000" )
	.space	1
.LC16:
	ASCII(.ascii	"find APC, but logic_fs_id %d abnormal(ref=%d,plfs=%" )
	ASCII(.ascii	"p, pdfs=%p)\012\000" )
.LC17:
	ASCII(.ascii	"\012\000" )
	.space	2
.LC18:
	ASCII(.ascii	"pMvcCtx->DPB.fs[%d]: frame_num=%d poc=%d is_referen" )
	ASCII(.ascii	"ce=%d\012\000" )
	.space	2
.LC19:
	ASCII(.ascii	"pMvcCtx->DPB.fs_ref[%d]: frame_num=%d poc=%d is_ref" )
	ASCII(.ascii	"erence=%d\012\000" )
	.space	2
.LC20:
	ASCII(.ascii	"pMvcCtx->DPB.fs_ref[%d]: MVC_FRAME.poc=%d \012\000" )
.LC21:
	ASCII(.ascii	"\012 VFMW ** release streambuff=%p   bitstream_leng" )
	ASCII(.ascii	"th=%d\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"clear curr slice.\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC24:
	ASCII(.ascii	"ERROR: FSP_ConfigInstance fail!\012\000" )
	.space	3
.LC25:
	ASCII(.ascii	"for P slice size of list equal 0(1st).\012\000" )
.LC26:
	ASCII(.ascii	"for P slice size of list equal 0(2nd).\012\000" )
.LC27:
	ASCII(.ascii	"for B slice size of two list all equal 0.\012\000" )
	.space	1
.LC28:
	ASCII(.ascii	"list[%d][%d]: frame_num=%d, poc=%d\012\000" )
.LC29:
	ASCII(.ascii	"init list error.\012\000" )
	.space	2
.LC30:
	ASCII(.ascii	"In one access unit,all non-base view should have th" )
	ASCII(.ascii	"e same subsps!\012\000" )
	.space	1
.LC31:
	ASCII(.ascii	"sps but mvc flag %d is wrong\012\000" )
	.space	2
.LC32:
	ASCII(.ascii	"save pic yuv :  structure = %d;  idc = %d\012\000" )
	.space	1
.LC33:
	ASCII(.ascii	"structure = %d pMvcCtx->CurrPic.pic_width_in_mb = %" )
	ASCII(.ascii	"d pMvcCtx->CurrPic.pic_height_in_mb = %d\012\000" )
	.space	3
.LC34:
	ASCII(.ascii	"nal_ref_idc=%d, structure=%d, image_id=%d, pmv_idc=" )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC35:
	ASCII(.ascii	"logic MVC_FRAME id(=%d): get LogicFs error!\012\000" )
	.space	3
.LC36:
	ASCII(.ascii	"phy fs is null: pstDecodeFs = %p, pstDispOutFs = %p" )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC37:
	ASCII(.ascii	"fuction return value is null,%s %d unknow error!!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC38:
	ASCII(.ascii	"SH: first_mb_in_slice\000" )
	.space	2
.LC39:
	ASCII(.ascii	"MVC_SliceCheck first_mb_in_slice >= MAX_MB_NUM_IN_P" )
	ASCII(.ascii	"IC error.\012\000" )
	.space	2
.LC40:
	ASCII(.ascii	"SH: slice_type\000" )
	.space	1
.LC41:
	ASCII(.ascii	"slice type = %d\012\000" )
	.space	3
.LC42:
	ASCII(.ascii	"slice type = %d, err\012\000" )
	.space	2
.LC43:
	ASCII(.ascii	"slice  pps id = %d\012\000" )
.LC44:
	ASCII(.ascii	"pps with this pic_parameter_set_id = %d havn't deco" )
	ASCII(.ascii	"de\012\000" )
	.space	1
.LC45:
	ASCII(.ascii	"sps with this pic_parameter_set_id havn't decode\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC46:
	ASCII(.ascii	"subsps with this seq_parameter_set_id = %d havn't d" )
	ASCII(.ascii	"ecoded\012\000" )
	.space	1
.LC47:
	ASCII(.ascii	"tmp slice  pps id = %d\012\000" )
.LC48:
	ASCII(.ascii	"MVC_PPSSPSCheckTmpId: pps with this pic_parameter_s" )
	ASCII(.ascii	"et_id = %d havn't decode\012\000" )
	.space	3
.LC49:
	ASCII(.ascii	"MVC_PPSSPSCheckTmpId: seq_parameter_set_id out of r" )
	ASCII(.ascii	"ange.\012\000" )
	.space	2
.LC50:
	ASCII(.ascii	"MVC_PPSSPSCheckTmpId: sps with this pic_parameter_s" )
	ASCII(.ascii	"et_id = %d havn't decode\012\000" )
	.space	3
.LC51:
	ASCII(.ascii	"new pic flag = %d\012\000" )
	.space	1
.LC52:
	ASCII(.ascii	"SH: ref_pic_list_reordering_flag_l0\000" )
.LC53:
	ASCII(.ascii	"SH: reordering_of_pic_nums_idc_l0\000" )
	.space	2
.LC54:
	ASCII(.ascii	"reorder idc l0 = %d, g_SeErrFlag=%d\012\000" )
	.space	3
.LC55:
	ASCII(.ascii	"SH: abs_diff_pic_num_minus1_l0\000" )
	.space	1
.LC56:
	ASCII(.ascii	"abs_diff_pic_num_minus1_l0 = %d, g_SeErrFlag=%d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC57:
	ASCII(.ascii	"SH: long_term_pic_idx_l0\000" )
	.space	3
.LC58:
	ASCII(.ascii	"SH: abs_diff_view_idx_minus1_l0\000" )
.LC59:
	ASCII(.ascii	"num of idc l0 exceed\012\000" )
	.space	2
.LC60:
	ASCII(.ascii	"SH: ref_pic_list_reordering_flag_l1\000" )
.LC61:
	ASCII(.ascii	"SH: reordering_of_pic_nums_idc_l1\000" )
	.space	2
.LC62:
	ASCII(.ascii	"reorder idc l1 = %d, g_SeErrFlag=%d\012\000" )
	.space	3
.LC63:
	ASCII(.ascii	"SH: abs_diff_pic_num_minus1_l1\000" )
	.space	1
.LC64:
	ASCII(.ascii	"abs_diff_pic_num_minus1_l1 = %d, g_SeErrFlag=%d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC65:
	ASCII(.ascii	"SH: long_term_pic_idx_l1\000" )
	.space	3
.LC66:
	ASCII(.ascii	"SH: abs_diff_view_idx_minus1_l1\000" )
.LC67:
	ASCII(.ascii	"num of idc l1 exceed\012\000" )
	.space	2
.LC68:
	ASCII(.ascii	"SH: luma_log2_weight_denom\000" )
	.space	1
.LC69:
	ASCII(.ascii	"SH: chroma_log2_weight_denom\000" )
	.space	3
.LC70:
	ASCII(.ascii	"WP log2 exceed  and err flag = %d\012\000" )
	.space	1
.LC71:
	ASCII(.ascii	"SH: luma_weight_flag_l0\000" )
.LC72:
	ASCII(.ascii	"SH: luma_weight_l0\000" )
	.space	1
.LC73:
	ASCII(.ascii	"SH: luma_offset_l0\000" )
	.space	1
.LC74:
	ASCII(.ascii	"SH: chroma_weight_flag_l0\000" )
	.space	2
.LC75:
	ASCII(.ascii	"SH: chroma_weight_l0\000" )
	.space	3
.LC76:
	ASCII(.ascii	"SH: chroma_offset_l0\000" )
	.space	3
.LC77:
	ASCII(.ascii	"SH: luma_weight_flag_l1\000" )
.LC78:
	ASCII(.ascii	"SH: luma_weight_l1\000" )
	.space	1
.LC79:
	ASCII(.ascii	"SH: luma_offset_l1\000" )
	.space	1
.LC80:
	ASCII(.ascii	"SH: chroma_weight_flag_l1\000" )
	.space	2
.LC81:
	ASCII(.ascii	"SH: chroma_weight_l1\000" )
	.space	3
.LC82:
	ASCII(.ascii	"SH: chroma_offset_l1\000" )
	.space	3
.LC83:
	ASCII(.ascii	"overflow MVC_MAX_MMCO_LEN\012\000" )
	.space	1
.LC84:
	ASCII(.ascii	"SH: memory_management_control_operation\000" )
.LC85:
	ASCII(.ascii	"SH: difference_of_pic_nums_minus1\000" )
	.space	2
.LC86:
	ASCII(.ascii	"SH: long_term_pic_num\000" )
	.space	2
.LC87:
	ASCII(.ascii	"SH: long_term_frame_idx\000" )
.LC88:
	ASCII(.ascii	"SH: max_long_term_frame_idx_plus1\000" )
	.space	2
.LC89:
	ASCII(.ascii	"mmco exceed 6\012\000" )
	.space	1
.LC90:
	ASCII(.ascii	"SH: no_output_of_prior_pics_flag\000" )
	.space	3
.LC91:
	ASCII(.ascii	"SH: long_term_reference_flag\000" )
	.space	3
.LC92:
	ASCII(.ascii	"IDR and no_output_of_prior_pics_flag = %d\012\000" )
	.space	1
.LC93:
	ASCII(.ascii	"SH: adaptive_ref_pic_marking_mode_flag\000" )
	.space	1
.LC94:
	ASCII(.ascii	"adaptive_ref_pic_marking_mode_flag = %d\012\000" )
	.space	3
.LC95:
	ASCII(.ascii	"MMCO para would save dec err\012\000" )
	.space	2
.LC96:
	ASCII(.ascii	"mulitislice diff no_out_of_prior_pics_flag, used fi" )
	ASCII(.ascii	"rst.\012\000" )
	.space	3
.LC97:
	ASCII(.ascii	"mulitislice diff long_term_reference_flag, used fir" )
	ASCII(.ascii	"st.\012\000" )
.LC98:
	ASCII(.ascii	"mulitislice diff adaptive_ref_pic_marking_mode_flag" )
	ASCII(.ascii	", used first.\012\000" )
	.space	2
.LC99:
	ASCII(.ascii	"TotalPicNum=%d, SliceNumInPic=%d, TotalNal=%d\012\000" )
	.space	1
.LC100:
	ASCII(.ascii	"SH: pic_parameter_set_id\000" )
	.space	3
.LC101:
	ASCII(.ascii	"pps id:%d in sliceheader err\012\000" )
	.space	2
.LC102:
	ASCII(.ascii	"PPS or SPS of this slice not valid\012\000" )
.LC103:
	ASCII(.ascii	"MVC_PPSSPSCheck failed, PPS or SPS of this slice no" )
	ASCII(.ascii	"t valid\012\000" )
.LC104:
	ASCII(.ascii	"SH: frame_num\000" )
	.space	2
.LC105:
	ASCII(.ascii	"IDR NAL but frame_num!=0.\012\000" )
	.space	1
.LC106:
	ASCII(.ascii	"SH: field_pic_flag\000" )
	.space	1
.LC107:
	ASCII(.ascii	"SH: bottom_field_flag\000" )
	.space	2
.LC108:
	ASCII(.ascii	"field first_mb_in_slice bigger than pic size\012\000" )
	.space	2
.LC109:
	ASCII(.ascii	"MVC_FRAME first_mb_in_slice bigger than pic size\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC110:
	ASCII(.ascii	"SH: idr_pic_id\000" )
	.space	1
.LC111:
	ASCII(.ascii	"idr_pic_id bigger than 65535\012\000" )
	.space	2
.LC112:
	ASCII(.ascii	"SH: pic_order_cnt_lsb\000" )
	.space	2
.LC113:
	ASCII(.ascii	"SH: delta_pic_order_cnt_bottom\000" )
	.space	1
.LC114:
	ASCII(.ascii	"SH: delta_pic_order_cnt[0]\000" )
	.space	1
.LC115:
	ASCII(.ascii	"SH: delta_pic_order_cnt[1]\000" )
	.space	1
.LC116:
	ASCII(.ascii	"SH: redundant_pic_cnt\000" )
	.space	2
.LC117:
	ASCII(.ascii	"redundant pic not support.\012\000" )
.LC118:
	ASCII(.ascii	"first_mb_in_slice in currslice small than prevslice" )
	ASCII(.ascii	" in same pic\012\000" )
	.space	3
.LC119:
	ASCII(.ascii	"SH: direct_spatial_mv_pred_flag\000" )
.LC120:
	ASCII(.ascii	"SH: num_ref_idx_override_flag\000" )
	.space	2
.LC121:
	ASCII(.ascii	"SH: num_ref_idx_l0_active_minus1\000" )
	.space	3
.LC122:
	ASCII(.ascii	"SH: num_ref_idx_l1_active_minus1\000" )
	.space	3
.LC123:
	ASCII(.ascii	"MVC_FRAME num_ref_idx_lx_active_minus1 exceed\012\000" )
	.space	1
.LC124:
	ASCII(.ascii	"field num_ref_idx_lx_active_minus1 exceed\012\000" )
	.space	1
.LC125:
	ASCII(.ascii	"reordering para dec err\012\000" )
	.space	3
.LC126:
	ASCII(.ascii	"apply_weights_flag=%d\012\000" )
	.space	1
.LC127:
	ASCII(.ascii	"wpt dec err\012\000" )
	.space	3
.LC128:
	ASCII(.ascii	"mark para dec err\012\000" )
	.space	1
.LC129:
	ASCII(.ascii	"SH: cabac_init_idc\000" )
	.space	1
.LC130:
	ASCII(.ascii	"cabac_init_idc bigger than 3\012\000" )
	.space	2
.LC131:
	ASCII(.ascii	"SH: slice_qp_delta\000" )
	.space	1
.LC132:
	ASCII(.ascii	"SH: disable_deblocking_filter_idc\000" )
	.space	2
.LC133:
	ASCII(.ascii	"disable_deblocking_filter_idc dec err\012\000" )
	.space	1
.LC134:
	ASCII(.ascii	"SH: slice_alpha_c0_offset_div2\000" )
	.space	1
.LC135:
	ASCII(.ascii	"slice_alpha_c0_offset_div2 dec err\012\000" )
.LC136:
	ASCII(.ascii	"SH: slice_beta_offset_div2\000" )
	.space	1
.LC137:
	ASCII(.ascii	"slice_beta_offset_div2 dec err\012\000" )
.LC138:
	ASCII(.ascii	"pMvcCtx->pCurrNal->nal_bitoffset = %d; code_len_byt" )
	ASCII(.ascii	"e = %d; bitstream_length = %d; pMvcCtx->pCurrNal->n" )
	ASCII(.ascii	"al_trail_zero_bit_num = %d\012\000" )
	.space	2
.LC139:
	ASCII(.ascii	"stream[%d]: bitsoffset = %d; valid_bitlen = %d; byt" )
	ASCII(.ascii	"espos = %x\012\000" )
	.space	1
.LC140:
	ASCII(.ascii	"nal_segment = %d is not expected value\012\000" )
.LC141:
	ASCII(.ascii	"stream[%d]: bitsoffset = %d; valid_bitlen = %d\012\000" )
.LC142:
	ASCII(.ascii	"   : delta_sl   \000" )
	.space	3
.LC143:
	ASCII(.ascii	"PPS: entropy_coding_mode_flag\000" )
	.space	2
.LC144:
	ASCII(.ascii	"PPS: pic_order_present_flag\000" )
.LC145:
	ASCII(.ascii	"PPS: num_slice_groups_minus1\000" )
	.space	3
.LC146:
	ASCII(.ascii	"MVC_BASELINE stream with FMO, not support.\012\000" )
.LC147:
	ASCII(.ascii	"PPS: num_ref_idx_l0_active_minus1\000" )
	.space	2
.LC148:
	ASCII(.ascii	"num_ref_idx_l0_active_minus1 out of range.\012\000" )
.LC149:
	ASCII(.ascii	"PPS: num_ref_idx_l1_active_minus1\000" )
	.space	2
.LC150:
	ASCII(.ascii	"num_ref_idx_l1_active_minus1 out of range.\012\000" )
.LC151:
	ASCII(.ascii	"PPS: weighted prediction flag\000" )
	.space	2
.LC152:
	ASCII(.ascii	"PPS: weighted_bipred_idc\000" )
	.space	3
.LC153:
	ASCII(.ascii	"weighted_bipred_idc out of range\012\000" )
	.space	2
.LC154:
	ASCII(.ascii	"PPS: pic_init_qp_minus26\000" )
	.space	3
.LC155:
	ASCII(.ascii	"PPS: pic_init_qs_minus26\000" )
	.space	3
.LC156:
	ASCII(.ascii	"PPS: chroma_qp_index_offset\000" )
.LC157:
	ASCII(.ascii	"PPS: deblocking_filter_control_present_flag\000" )
.LC158:
	ASCII(.ascii	"PPS: constrained_intra_pred_flag\000" )
	.space	3
.LC159:
	ASCII(.ascii	"PPS: redundant_pic_cnt_present_flag\000" )
.LC160:
	ASCII(.ascii	"redundant pic not support when find redundant slice" )
	ASCII(.ascii	" later, exit\012\000" )
	.space	3
.LC161:
	ASCII(.ascii	"PPS: transform_8x8_mode_flag\000" )
	.space	3
.LC162:
	ASCII(.ascii	"PPS: pic_scaling_matrix_present_flag\000" )
	.space	3
.LC163:
	ASCII(.ascii	"PPS: second_chroma_qp_index_offset\000" )
	.space	1
.LC164:
	ASCII(.ascii	"SPS: seq_scaling_list_present_flag\000" )
	.space	1
.LC165:
	ASCII(.ascii	"PPS: pic_scaling_list_present_flag\000" )
	.space	1
.LC166:
	ASCII(.ascii	"second_chroma_qp_index_offset out of range.\012\000" )
	.space	3
.LC167:
	ASCII(.ascii	"PPS: pic_parameter_set_id\000" )
	.space	2
.LC168:
	ASCII(.ascii	"pic_parameter_set_id(%d) out of range.\012\000" )
.LC169:
	ASCII(.ascii	"PPS: seq_parameter_set_id\000" )
	.space	2
.LC170:
	ASCII(.ascii	"seq_parameter_set_id out of range.\012\000" )
.LC171:
	ASCII(.ascii	"PPS decode error line: %d.\012\000" )
.LC172:
	ASCII(.ascii	"VUI: aspect_ratio_info_present_flag\000" )
.LC173:
	ASCII(.ascii	"VUI: aspect_ratio_idc\000" )
	.space	2
.LC174:
	ASCII(.ascii	"VUI: sar_width\000" )
	.space	1
.LC175:
	ASCII(.ascii	"VUI: sar_height\000" )
.LC176:
	ASCII(.ascii	"VUI: overscan_info_present_flag\000" )
.LC177:
	ASCII(.ascii	"VUI: overscan_appropriate_flag\000" )
	.space	1
.LC178:
	ASCII(.ascii	"VUI: video_signal_type_present_flag\000" )
.LC179:
	ASCII(.ascii	"VUI: video_format\000" )
	.space	2
.LC180:
	ASCII(.ascii	"VUI: video_full_range_flag\000" )
	.space	1
.LC181:
	ASCII(.ascii	"VUI: color_description_present_flag\000" )
.LC182:
	ASCII(.ascii	"VUI: colour_primaries\000" )
	.space	2
.LC183:
	ASCII(.ascii	"VUI: transfer_characteristics\000" )
	.space	2
.LC184:
	ASCII(.ascii	"VUI: matrix_coefficients\000" )
	.space	3
.LC185:
	ASCII(.ascii	"VUI: chroma_loc_info_present_flag\000" )
	.space	2
.LC186:
	ASCII(.ascii	"VUI: chroma_sample_loc_type_top_field\000" )
	.space	2
.LC187:
	ASCII(.ascii	"VUI: chroma_sample_loc_type_bottom_field\000" )
	.space	3
.LC188:
	ASCII(.ascii	"VUI: timing_info_present_flag\000" )
	.space	2
.LC189:
	ASCII(.ascii	"VUI: num_units_in_tick\000" )
	.space	1
.LC190:
	ASCII(.ascii	"VUI: time_scale\000" )
.LC191:
	ASCII(.ascii	"VUI: fixed_frame_rate_flag\000" )
	.space	1
.LC192:
	ASCII(.ascii	"VUI: nal_hrd_parameters_present_flag\000" )
	.space	3
.LC193:
	ASCII(.ascii	"VUI: cpb_cnt_minus1\000" )
.LC194:
	ASCII(.ascii	"VUI: bit_rate_scale\000" )
.LC195:
	ASCII(.ascii	"VUI: cpb_size_scale\000" )
.LC196:
	ASCII(.ascii	"hrd->cpb_cnt_minus1 out of range\012\000" )
	.space	2
.LC197:
	ASCII(.ascii	"VUI: bit_rate_value_minus1\000" )
	.space	1
.LC198:
	ASCII(.ascii	"VUI: cpb_size_value_minus1\000" )
	.space	1
.LC199:
	ASCII(.ascii	"VUI: cbr_flag\000" )
	.space	2
.LC200:
	ASCII(.ascii	"VUI: initial_cpb_removal_delay_length_minus1\000" )
	.space	3
.LC201:
	ASCII(.ascii	"VUI: cpb_removal_delay_length_minus1\000" )
	.space	3
.LC202:
	ASCII(.ascii	"VUI: dpb_output_delay_length_minus1\000" )
.LC203:
	ASCII(.ascii	"VUI: time_offset_length\000" )
.LC204:
	ASCII(.ascii	"VUI: vcl_hrd_parameters_present_flag\000" )
	.space	3
.LC205:
	ASCII(.ascii	"VUI: low_delay_hrd_flag\000" )
.LC206:
	ASCII(.ascii	"VUI: pic_struct_present_flag\000" )
	.space	3
.LC207:
	ASCII(.ascii	"VUI: bitstream_restriction_flag\000" )
.LC208:
	ASCII(.ascii	"VUI: motion_vectors_over_pic_boundaries_flag\000" )
	.space	3
.LC209:
	ASCII(.ascii	"VUI: max_bytes_per_pic_denom\000" )
	.space	3
.LC210:
	ASCII(.ascii	"VUI: max_bits_per_mb_denom\000" )
	.space	1
.LC211:
	ASCII(.ascii	"VUI: log2_max_mv_length_horizontal\000" )
	.space	1
.LC212:
	ASCII(.ascii	"VUI: log2_max_mv_length_vertical\000" )
	.space	3
.LC213:
	ASCII(.ascii	"VUI: num_reorder_frames\000" )
.LC214:
	ASCII(.ascii	"VUI: max_dec_frame_buffering\000" )
	.space	3
.LC215:
	ASCII(.ascii	"dar=%d, final_dar=%d\012\000" )
	.space	2
.LC216:
	ASCII(.ascii	"SPS: chroma_format_idc\000" )
	.space	1
.LC217:
	ASCII(.ascii	"pSPS->chroma_format_idc out of range.\012\000" )
	.space	1
.LC218:
	ASCII(.ascii	"SPS: bit_depth_luma_minus8\000" )
	.space	1
.LC219:
	ASCII(.ascii	"bit_depth_luma_minus8 not equal 0.\012\000" )
.LC220:
	ASCII(.ascii	"SPS: bit_depth_chroma_minus8\000" )
	.space	3
.LC221:
	ASCII(.ascii	"bit_depth_chroma_minus8 not equal 0.\012\000" )
	.space	2
.LC222:
	ASCII(.ascii	"SPS: qpprime_y_zero_trans_bypass_flag\000" )
	.space	2
.LC223:
	ASCII(.ascii	"qpprime_y_zero_trans_bypass_flag not equal 0.\012\000" )
	.space	1
.LC224:
	ASCII(.ascii	"SPS: seq_scaling_matrix_present_flag\000" )
	.space	3
.LC225:
	ASCII(.ascii	"SPS: log2_max_frame_num_minus4\000" )
	.space	1
.LC226:
	ASCII(.ascii	"pSPS->log2_max_frame_num_minus4 out of range.\012\000" )
	.space	1
.LC227:
	ASCII(.ascii	"SPS: pic_order_cnt_type\000" )
.LC228:
	ASCII(.ascii	"pSPS->pic_order_cnt_type out of range.\012\000" )
.LC229:
	ASCII(.ascii	"SPS: log2_max_pic_order_cnt_lsb_minus4\000" )
	.space	1
.LC230:
	ASCII(.ascii	"SPS: num_ref_frames\000" )
.LC231:
	ASCII(.ascii	"pSPS->log2_max_pic_order_cnt_lsb_minus4 out of rang" )
	ASCII(.ascii	"e.\012\000" )
	.space	1
.LC232:
	ASCII(.ascii	"SPS: delta_pic_order_always_zero_flag\000" )
	.space	2
.LC233:
	ASCII(.ascii	"SPS: offset_for_non_ref_pic\000" )
.LC234:
	ASCII(.ascii	"SPS: offset_for_top_to_bottom_field\000" )
.LC235:
	ASCII(.ascii	"SPS: num_ref_frames_in_pic_order_cnt_cycle\000" )
	.space	1
.LC236:
	ASCII(.ascii	"pSPS->num_ref_frames_in_pic_order_cnt_cycle out of " )
	ASCII(.ascii	"range.\012\000" )
	.space	1
.LC237:
	ASCII(.ascii	"SPS: offset_for_ref_frame[i]\000" )
	.space	3
.LC238:
	ASCII(.ascii	"SPS: gaps_in_frame_num_value_allowed_flag\000" )
	.space	2
.LC239:
	ASCII(.ascii	"SPS: pic_width_in_mbs_minus1\000" )
	.space	3
.LC240:
	ASCII(.ascii	"pSPS->pic_width_in_mbs_minus1 out of range(=%d).\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC241:
	ASCII(.ascii	"SPS: pic_height_in_map_units_minus1\000" )
.LC242:
	ASCII(.ascii	"SPS: frame_mbs_only_flag\000" )
	.space	3
.LC243:
	ASCII(.ascii	"pSPS->pic_height_in_map_units_minus1 out of range.\012" )
	ASCII(.ascii	"\000" )
.LC244:
	ASCII(.ascii	"pic size too large.\012\000" )
	.space	3
.LC245:
	ASCII(.ascii	"level_idc %d not support.\012\000" )
	.space	1
.LC246:
	ASCII(.ascii	"dpb size according level : %d\012\000" )
	.space	1
.LC247:
	ASCII(.ascii	"pSPS->num_ref_frames(%d) > DPB size(%d), try to sel" )
	ASCII(.ascii	"ect the reasonable one.\012\000" )
.LC248:
	ASCII(.ascii	"SPS: direct_8x8_inference_flag\000" )
	.space	1
.LC249:
	ASCII(.ascii	"SPS: frame_cropping_flag\000" )
	.space	3
.LC250:
	ASCII(.ascii	"SPS: frame_cropping_rect_left_offset\000" )
	.space	3
.LC251:
	ASCII(.ascii	"SPS: frame_cropping_rect_right_offset\000" )
	.space	2
.LC252:
	ASCII(.ascii	"SPS: frame_cropping_rect_top_offset\000" )
.LC253:
	ASCII(.ascii	"SPS: frame_cropping_rect_bottom_offset\000" )
	.space	1
.LC254:
	ASCII(.ascii	"SPS: vui_parameters_present_flag\000" )
	.space	3
.LC255:
	ASCII(.ascii	"max_dec_frame_buffering(%d) > MaxDpbSize(%d)\012\000" )
	.space	2
.LC256:
	ASCII(.ascii	"SPS: mb_adaptive_frame_field_flag\000" )
	.space	2
.LC257:
	ASCII(.ascii	"SPS: profile_idc\000" )
	.space	3
.LC258:
	ASCII(.ascii	"SPS: constrained_set0_flag\000" )
	.space	1
.LC259:
	ASCII(.ascii	"SPS: constrained_set1_flag\000" )
	.space	1
.LC260:
	ASCII(.ascii	"SPS: constrained_set2_flag\000" )
	.space	1
.LC261:
	ASCII(.ascii	"SPS: constrained_set3_flag\000" )
	.space	1
.LC262:
	ASCII(.ascii	"SPS: constrained_set4_flag\000" )
	.space	1
.LC263:
	ASCII(.ascii	"SPS: constrained_set5_flag\000" )
	.space	1
.LC264:
	ASCII(.ascii	"SPS: reserved_zero_2bits\000" )
	.space	3
.LC265:
	ASCII(.ascii	"SPS: level_idc\000" )
	.space	1
.LC266:
	ASCII(.ascii	"SPS: seq_parameter_set_id\000" )
	.space	2
.LC267:
	ASCII(.ascii	"MVC_BASELINE stream, try to decode, exit when FMO o" )
	ASCII(.ascii	"ccurred.\012\000" )
	.space	3
.LC268:
	ASCII(.ascii	"MVC_EXTENDED stream, try to decode, exit when datap" )
	ASCII(.ascii	"artition occurred.\012\000" )
	.space	1
.LC269:
	ASCII(.ascii	"others High profile stream, try to decode, exit whe" )
	ASCII(.ascii	"n high profile not support occurred.\012\000" )
	.space	3
.LC270:
	ASCII(.ascii	"profile_idc = %5d error, try to decode as main prof" )
	ASCII(.ascii	"ile.\012\000" )
	.space	3
.LC271:
	ASCII(.ascii	"level_idc = %5d error, try to decode as level_idc %" )
	ASCII(.ascii	"d.\012\000" )
	.space	1
.LC272:
	ASCII(.ascii	"Line %d: SPS[%d] decode error.\012\000" )
.LC273:
	ASCII(.ascii	"SUBSPS: num_views_minus1\000" )
	.space	3
.LC274:
	ASCII(.ascii	"pSubsps->num_views_minus1(%d) out of range.\012\000" )
	.space	3
.LC275:
	ASCII(.ascii	"SUBSPS: view_id[]\000" )
	.space	2
.LC276:
	ASCII(.ascii	"SUBSPS: num_anchor_refs_l0[]\000" )
	.space	3
.LC277:
	ASCII(.ascii	"SUBSPS: num_anchor_refs_l1[]\000" )
	.space	3
.LC278:
	ASCII(.ascii	"pSubsps->num_anchor_refs_l0(%d) out of range.\012\000" )
	.space	1
.LC279:
	ASCII(.ascii	"SUBSPS: anchor_ref_l0[][]\000" )
	.space	2
.LC280:
	ASCII(.ascii	"pSubsps->num_anchor_refs_l1(%d) out of range.\012\000" )
	.space	1
.LC281:
	ASCII(.ascii	"SUBSPS: anchor_ref_l1[][]\000" )
	.space	2
.LC282:
	ASCII(.ascii	"SUBSPS: num_level_values_signalled_minus1\000" )
	.space	2
.LC283:
	ASCII(.ascii	"SUBSPS: num_non_anchor_refs_l0[]\000" )
	.space	3
.LC284:
	ASCII(.ascii	"SUBSPS: num_non_anchor_refs_l1[]\000" )
	.space	3
.LC285:
	ASCII(.ascii	"pSubsps->num_non_anchor_refs_l0(%d) out of range.\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC286:
	ASCII(.ascii	"SUBSPS: non_anchor_ref_l0[][]\000" )
	.space	2
.LC287:
	ASCII(.ascii	"pSubsps->num_non_anchor_refs_l1(%d) out of range.\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC288:
	ASCII(.ascii	"SUBSPS: non_anchor_ref_l1[][]\000" )
	.space	2
.LC289:
	ASCII(.ascii	"num_level(%d) out of range.\012\000" )
	.space	3
.LC290:
	ASCII(.ascii	"SUBSPS: level_idc[]\000" )
.LC291:
	ASCII(.ascii	"SUBSPS: num_applicable_ops_minus1\000" )
	.space	2
.LC292:
	ASCII(.ascii	"num_ops(%d) out of range.\012\000" )
	.space	1
.LC293:
	ASCII(.ascii	"SUBSPS: applicable_op_temporal_id[][]\000" )
	.space	2
.LC294:
	ASCII(.ascii	"SUBSPS: applicable_op_num_target_views_minus1[][]\000" )
	.space	2
.LC295:
	ASCII(.ascii	"SUBSPS: applicable_op_num_views_minus1\000" )
	.space	1
.LC296:
	ASCII(.ascii	"applicable_op_num_target_views(%d) out of range.\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC297:
	ASCII(.ascii	"SUBSPS: applicable_op_target_view_id[][][]\000" )
	.space	1
.LC298:
	ASCII(.ascii	"pSubsps->applicable_op_num_views_minus1(%d) out of " )
	ASCII(.ascii	"range.\012\000" )
	.space	1
.LC299:
	ASCII(.ascii	"MVC VUI: vui_mvc_num_ops_minus1\000" )
.LC300:
	ASCII(.ascii	"vui_mvc_num_ops_minus1 %d out of range\012\000" )
.LC301:
	ASCII(.ascii	"MVC VUI: vui_mvc_temporal_id[]\000" )
	.space	1
.LC302:
	ASCII(.ascii	"SUBSPS VUI: vui_mvc_num_target_output_views_minus1[" )
	ASCII(.ascii	"]\000" )
	.space	3
.LC303:
	ASCII(.ascii	"MVC VUI: vui_mvc_timing_info_present_flag\000" )
	.space	2
.LC304:
	ASCII(.ascii	"MVC VUI: vui_mvc_view_id[][]\000" )
	.space	3
.LC305:
	ASCII(.ascii	"MVC VUI: vui_mvc_num_units_in_tick\000" )
	.space	1
.LC306:
	ASCII(.ascii	"MVC VUI: vui_mvc_time_scale[]\000" )
	.space	2
.LC307:
	ASCII(.ascii	"MVC VUI: vui_mvc_fixed_frame_rate_flag\000" )
	.space	1
.LC308:
	ASCII(.ascii	"MVC VUI: vui_mvc_nal_hrd_parameters_present_flag[]\000" )
	.space	1
.LC309:
	ASCII(.ascii	"MVC VUI: cpb_cnt_minus1\000" )
.LC310:
	ASCII(.ascii	"MVC VUI: bit_rate_scale\000" )
.LC311:
	ASCII(.ascii	"MVC VUI: cpb_size_scale\000" )
.LC312:
	ASCII(.ascii	"MVC VUI: bit_rate_value_minus1\000" )
	.space	1
.LC313:
	ASCII(.ascii	"MVC VUI: cpb_size_value_minus1\000" )
	.space	1
.LC314:
	ASCII(.ascii	"MVC VUI: cbr_flag\000" )
	.space	2
.LC315:
	ASCII(.ascii	"MVC VUI: initial_cpb_removal_delay_length_minus1\000" )
	.space	3
.LC316:
	ASCII(.ascii	"MVC VUI: cpb_removal_delay_length_minus1\000" )
	.space	3
.LC317:
	ASCII(.ascii	"MVC VUI: dpb_output_delay_length_minus1\000" )
.LC318:
	ASCII(.ascii	"MVC VUI: time_offset_length\000" )
.LC319:
	ASCII(.ascii	"MVC VUI: vui_mvc_vcl_hrd_parameters_present_flag[]\000" )
	.space	1
.LC320:
	ASCII(.ascii	"MVC VUI: vui_mvc_low_delay_hrd_flag[]\000" )
	.space	2
.LC321:
	ASCII(.ascii	"MVC VUI: vui_mvc_pic_struct_present_flag[]\000" )
	.space	1
.LC322:
	ASCII(.ascii	"SUBSPS: profile_idc\000" )
.LC323:
	ASCII(.ascii	"SUBSPS: constrained_set0_flag\000" )
	.space	2
.LC324:
	ASCII(.ascii	"SUBSPS: constrained_set1_flag\000" )
	.space	2
.LC325:
	ASCII(.ascii	"SUBSPS: constrained_set2_flag\000" )
	.space	2
.LC326:
	ASCII(.ascii	"SUBSPS: constrained_set3_flag\000" )
	.space	2
.LC327:
	ASCII(.ascii	"SUBSPS: constrained_set4_flag\000" )
	.space	2
.LC328:
	ASCII(.ascii	"SUBSPS: constrained_set5_flag\000" )
	.space	2
.LC329:
	ASCII(.ascii	"SUBSPS: reserved_zero_2bits\000" )
.LC330:
	ASCII(.ascii	"SUBSPS: level_idc\000" )
	.space	2
.LC331:
	ASCII(.ascii	"SUBSPS: seq_parameter_set_id\000" )
	.space	3
.LC332:
	ASCII(.ascii	"profile_idc = %5d error\012\000" )
	.space	3
.LC333:
	ASCII(.ascii	"level_idc = %5d error, try to decode as level_idc 4" )
	ASCII(.ascii	"1.\012\000" )
	.space	1
.LC334:
	ASCII(.ascii	"SUBSPS decode error.\012\000" )
	.space	2
.LC335:
	ASCII(.ascii	"SUBSPS: bit_equal_to_one\000" )
	.space	3
.LC336:
	ASCII(.ascii	"SUBSPS Mvc Ext decode error.\012\000" )
	.space	2
.LC337:
	ASCII(.ascii	"SUBSPS: mvc_vui_parameters_present_flag\000" )
.LC338:
	ASCII(.ascii	"SUBSPS Mvc Vui Ext decode error.\012\000" )
	.space	2
.LC339:
	ASCII(.ascii	"SEI: frame_packing_arrangement_id\000" )
	.space	2
.LC340:
	ASCII(.ascii	"SEI: frame_packing_arrangement_cancel_flag\000" )
	.space	1
.LC341:
	ASCII(.ascii	"SEI: frame_packing_arrangement_type\000" )
.LC342:
	ASCII(.ascii	"SEI: quincunx_sampling_flag\000" )
.LC343:
	ASCII(.ascii	"SEI: content_interpretation_type\000" )
	.space	3
.LC344:
	ASCII(.ascii	"SEI: spatial_flipping_flag\000" )
	.space	1
.LC345:
	ASCII(.ascii	"SEI: frame0_flipped_flag\000" )
	.space	3
.LC346:
	ASCII(.ascii	"SEI: field_views_flag\000" )
	.space	2
.LC347:
	ASCII(.ascii	"SEI: current_frame_is_frame0_flag\000" )
	.space	2
.LC348:
	ASCII(.ascii	"SEI: frame0_self_contained_flag\000" )
.LC349:
	ASCII(.ascii	"SEI: frame1_self_contained_flag\000" )
.LC350:
	ASCII(.ascii	"SEI: frame0_grid_position_x\000" )
.LC351:
	ASCII(.ascii	"SEI: frame0_grid_position_y\000" )
.LC352:
	ASCII(.ascii	"SEI: frame1_grid_position_x\000" )
.LC353:
	ASCII(.ascii	"SEI: frame1_grid_position_y\000" )
.LC354:
	ASCII(.ascii	"SEI: frame_packing_arrangement_reserved_byte\000" )
	.space	3
.LC355:
	ASCII(.ascii	"SEI: frame_packing_arrangement_repetition_period\000" )
	.space	3
.LC356:
	ASCII(.ascii	"SEI: frame_packing_arrangement_extension_flag\000" )
	.space	2
.LC357:
	ASCII(.ascii	"DecPicTimingSEI but SPS is invalid.\012\000" )
	.space	3
.LC358:
	ASCII(.ascii	"SEI: cpb_removal_delay\000" )
	.space	1
.LC359:
	ASCII(.ascii	"SEI: dpb_output_delay\000" )
	.space	2
.LC360:
	ASCII(.ascii	"pic_struct\000" )
	.space	1
.LC361:
	ASCII(.ascii	"SEI nal dec payload type err\012\000" )
	.space	2
.LC362:
	ASCII(.ascii	"SEI nal dec payloadSize err\012\000" )
	.space	3
.LC363:
	ASCII(.ascii	"MVC_SEI_BUFFERING_PERIOD,offset = %d,payload_size =" )
	ASCII(.ascii	" %d \012\000" )
	.space	3
.LC364:
	ASCII(.ascii	"MVC_PassBytes err [%s][%d]\012\000" )
.LC365:
	ASCII(.ascii	"MVC_SEI_PIC_TIMING,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC366:
	ASCII(.ascii	"MVC_SEI_PAN_SCAN_RECT,offset = %d,payload_size = %d" )
	ASCII(.ascii	" \012\000" )
	.space	2
.LC367:
	ASCII(.ascii	"MVC_SEI_FILLER_PAYLOAD,offset = %d,payload_size = %" )
	ASCII(.ascii	"d \012\000" )
	.space	1
.LC368:
	ASCII(.ascii	"MVC_SEI_USER_DATA_REGISTERED_ITU_T_T35, offset = %d" )
	ASCII(.ascii	", payload_size = %d \012\000" )
	.space	3
.LC369:
	ASCII(.ascii	"MVC_SEI_USER_DATA_UNREGISTERED, offset = %d, payloa" )
	ASCII(.ascii	"d_size = %d \012\000" )
	.space	3
.LC370:
	ASCII(.ascii	"SEI: itu_t_t35_country_code\000" )
.LC371:
	ASCII(.ascii	"SEI: itu_t_t35_country_code_extension_byte\000" )
	.space	1
.LC372:
	ASCII(.ascii	"SEI: itu_t_t35_provider_code\000" )
	.space	3
.LC373:
	ASCII(.ascii	"cann't dec usrdata\012\000" )
.LC374:
	ASCII(.ascii	"MVC_SEI_RECOVERY_POINT,offset = %d,payload_size = %" )
	ASCII(.ascii	"d \012\000" )
	.space	1
.LC375:
	ASCII(.ascii	"MVC_SEI_DEC_REF_PIC_MARKING_REPETITION,offset = %d," )
	ASCII(.ascii	"payload_size = %d \012\000" )
	.space	1
.LC376:
	ASCII(.ascii	"MVC_SEI_SPARE_PIC,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC377:
	ASCII(.ascii	"MVC_SEI_SCENE_INFO,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC378:
	ASCII(.ascii	"MVC_SEI_SUB_SEQ_INFO,offset = %d,payload_size = %d " )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC379:
	ASCII(.ascii	"MVC_SEI_SUB_SEQ_LAYER_CHARACTERISTICS,offset = %d,p" )
	ASCII(.ascii	"ayload_size = %d \012\000" )
	.space	2
.LC380:
	ASCII(.ascii	"MVC_SEI_SUB_SEQ_CHARACTERISTICS,offset = %d,payload" )
	ASCII(.ascii	"_size = %d \012\000" )
.LC381:
	ASCII(.ascii	"MVC_SEI_FULL_FRAME_FREEZE,offset = %d,payload_size " )
	ASCII(.ascii	"= %d \012\000" )
	.space	2
.LC382:
	ASCII(.ascii	"MVC_SEI_FULL_FRAME_FREEZE_RELEASE,offset = %d,paylo" )
	ASCII(.ascii	"ad_size = %d \012\000" )
	.space	2
.LC383:
	ASCII(.ascii	"MVC_SEI_FULL_FRAME_SNAPSHOT,offset = %d,payload_siz" )
	ASCII(.ascii	"e = %d \012\000" )
.LC384:
	ASCII(.ascii	"MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START,offset" )
	ASCII(.ascii	" = %d,payload_size = %d \012\000" )
	.space	3
.LC385:
	ASCII(.ascii	"MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END,offset =" )
	ASCII(.ascii	" %d,payload_size = %d \012\000" )
	.space	1
.LC386:
	ASCII(.ascii	"MVC_SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,offset =" )
	ASCII(.ascii	" %d,payload_size = %d \012\000" )
	.space	1
.LC387:
	ASCII(.ascii	"MVC_SEI_FILM_GRAIN_CHARACTERISTICS,offset = %d,payl" )
	ASCII(.ascii	"oad_size = %d \012\000" )
	.space	1
.LC388:
	ASCII(.ascii	"MVC_SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,offset" )
	ASCII(.ascii	" = %d,payload_size = %d \012\000" )
	.space	3
.LC389:
	ASCII(.ascii	"MVC_SEI_STEREO_VIDEO_INFO,offset = %d,payload_size " )
	ASCII(.ascii	"= %d \012\000" )
	.space	2
.LC390:
	ASCII(.ascii	"too much usrdat, cann't dec SEI\012\000" )
	.space	3
.LC391:
	ASCII(.ascii	"\012 mvc the stream is error,len:%d\012\000" )
	.space	2
.LC392:
	ASCII(.ascii	"\012Slice nal or IDR nal = %d\012\000" )
.LC393:
	ASCII(.ascii	"sliceheader dec err\012\000" )
	.space	3
.LC394:
	ASCII(.ascii	"MVC_InquireSliceProperty mvc flag %d is wrong\012\000" )
	.space	1
.LC395:
	ASCII(.ascii	"MvcDec destroy\012\000" )
.LC396:
	ASCII(.ascii	"MVC recycle image\012\000" )
	.space	1
.LC397:
	ASCII(.ascii	"pH264Ctx is %#x, pFrameStore is %#x\012\000" )
	.space	3
.LC398:
	ASCII(.ascii	"line: %d, pstLogicFsImage is NULL\012\000" )
	.space	1
.LC399:
	ASCII(.ascii	"InsertVO err:%d, MVC_ClearAll\012\000" )
	.space	1
.LC400:
	ASCII(.ascii	"mvc.c line %d: MVC_FRAME para err(ret=%d), recycle " )
	ASCII(.ascii	"image self\012\000" )
	.space	1
.LC401:
	ASCII(.ascii	"[%s][%d] MVC_OutputFrmToVO return %d !\012\000" )
.LC402:
	ASCII(.ascii	"[%s][%d] MVC_OutputFrmToVO return %d\012\000" )
	.space	2
.LC403:
	ASCII(.ascii	"%s %d MVC_OK != MVC_FlushDPB!!\012\000" )
.LC404:
	ASCII(.ascii	"%s %d MVC_OK != MVC_InitDPB!!\012\000" )
	.space	1
.LC405:
	ASCII(.ascii	"----------------- MVC_IMODE -> %d -----------------" )
	ASCII(.ascii	"-\012\000" )
	.space	2
.LC406:
	ASCII(.ascii	"Discard this B(poc=%d) befor P, is_ref_idc=%d.\012\000" )
.LC407:
	ASCII(.ascii	"***** VDM start, TotalPicNum=%d, pMvcCtx->CurrPic.s" )
	ASCII(.ascii	"tructure=%d.\012\000" )
	.space	3
.LC408:
	ASCII(.ascii	"MVC_DecVDM fail [%s][%d]\012\000" )
	.space	2
.LC409:
	ASCII(.ascii	"\012 receive streambuff=%p; phy_addr=0x%x; bitstrea" )
	ASCII(.ascii	"m_length=%d\012\000" )
	.space	3
.LC410:
	ASCII(.ascii	"nal_releaMVC_SE_ERR\012\000" )
	.space	3
.LC411:
	ASCII(.ascii	"cann't find slot for current nal\012\000" )
	.space	2
.LC412:
	ASCII(.ascii	"receive a zero packet\012\000" )
	.space	1
.LC413:
	ASCII(.ascii	"clear all dec para\012\000" )
.LC414:
	ASCII(.ascii	"line %d, get APC error, ret %d\012\000" )
.LC415:
	ASCII(.ascii	"InserFrmInDPB: cur pic struct = %d!\012\000" )
	.space	3
.LC416:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC417:
	ASCII(.ascii	"FS_ALLOC_ERR, MVC_ClearAll\012\000" )
.LC418:
	ASCII(.ascii	"Can not new logic fs! MVC_ClearAll\012\000" )
.LC419:
	ASCII(.ascii	"%s %d NULL== pMvcCtx->SPS || NULL == pMvcCtx->PPS |" )
	ASCII(.ascii	"| NULL == pMvcCtx->DecSlicePara!!\012\000" )
	.space	2
.LC420:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC421:
	ASCII(.ascii	"CurrPic.state is 'MVC_EMPTY'\012\000" )
	.space	2
.LC422:
	ASCII(.ascii	"MVC_StorePicInDpb return(%d) from L%d\012\000" )
	.space	1
.LC423:
	ASCII(.ascii	"Is IDR, but init DPB failed!\012\000" )
	.space	2
.LC424:
	ASCII(.ascii	"Is IDR, but FlushDPB failed!\012\000" )
	.space	2
.LC425:
	ASCII(.ascii	"MVC_DirectOutput mode\012\000" )
	.space	1
.LC426:
	ASCII(.ascii	"line %d, REPORT_IFRAME_ERR\012\000" )
.LC427:
	ASCII(.ascii	"line %d, return %d\012\000" )
.LC428:
	ASCII(.ascii	"err(%d) > ref_thr(%d)\012\000" )
	.space	1
.LC429:
	ASCII(.ascii	"MVC_Marking return %d\012\000" )
	.space	1
.LC430:
	ASCII(.ascii	"line %d: pMvcCtx->DPB.size = %d, ref %d, ltref %d\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC431:
	ASCII(.ascii	"line %d, MVC_GetMinPOC failed\012\000" )
	.space	1
.LC432:
	ASCII(.ascii	"%s: pos(%d) = pre_pos, force return.\012\000" )
	.space	2
.LC433:
	ASCII(.ascii	"GAP found while DPB is MVC_EMPTY!\012\000" )
	.space	1
.LC434:
	ASCII(.ascii	"MVC_FRAME num gap try to fill it \012\000" )
	.space	1
.LC435:
	ASCII(.ascii	"CurrFrameNum = %d  UnusedShortTermFrameNum = %d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC436:
	ASCII(.ascii	"cann't allocate MVC_FRAME store when gap find\012\000" )
	.space	1
.LC437:
	ASCII(.ascii	"allocate MVC_FRAME store when gap find\012\000" )
.LC438:
	ASCII(.ascii	"line %d: store gap pic err, ret=%d\012\000" )
.LC439:
	ASCII(.ascii	"line %d: flush dpb return %d\012\000" )
	.space	2
.LC440:
	ASCII(.ascii	"line %d: init dpb return %d\012\000" )
	.space	3
.LC441:
	ASCII(.ascii	"resolution error, the CAP_LEVEL_USER_DEFINE_WITH_OP" )
	ASCII(.ascii	"TION channel with s32ReRangeEn == 0 can't support s" )
	ASCII(.ascii	"uch bitstream.\012\000" )
	.space	2
.LC442:
	ASCII(.ascii	"s32MaxRefFrameNum < pMvcCtx->DPB.size, the CAP_LEVE" )
	ASCII(.ascii	"L_USER_DEFINE_WITH_OPTION channel can't support suc" )
	ASCII(.ascii	"h bitstream.\012\000" )
.LC443:
	ASCII(.ascii	"mem arrange err, MVC_ClearAll\012\000" )
	.space	1
.LC444:
	ASCII(.ascii	"line %d: MVC_FRAME gap(=%d) > dpb size(=%d)\012\000" )
	.space	3
.LC445:
	ASCII(.ascii	"MVC_FRAME num gap don't allowed but gap find\012\000" )
	.space	2
.LC446:
	ASCII(.ascii	"line %d: frame num(%d/%d) find gap in NON-I slice b" )
	ASCII(.ascii	"ut here gap is not allowed\012\000" )
	.space	1
.LC447:
	ASCII(.ascii	"line %d: dec gap failed\012\000" )
	.space	3
.LC448:
	ASCII(.ascii	"line %d: alloc framestore failed\012\000" )
	.space	2
.LC449:
	ASCII(.ascii	"dec_pts: %lld\012\000" )
	.space	1
.LC450:
	ASCII(.ascii	"dec_usertag: %lld\012\000" )
	.space	1
.LC451:
	ASCII(.ascii	"line %d: CurrPic.frame_store is NULL\012\000" )
	.space	2
.LC452:
	ASCII(.ascii	"get back frm\012\000" )
	.space	2
.LC453:
	ASCII(.ascii	"Start Reason: SliceParaNum, MaxBytesReceived = %d, " )
	ASCII(.ascii	"%d(thr=%d)\012\000" )
	.space	1
.LC454:
	ASCII(.ascii	"Too many slice or bitstream, err!\012\000" )
	.space	1
.LC455:
	ASCII(.ascii	"Start Reason: new_pic_flag\012\000" )
.LC456:
	ASCII(.ascii	"init pic err, find next recover point or next valid" )
	ASCII(.ascii	" sps, pps, or exit\012\000" )
	.space	1
.LC457:
	ASCII(.ascii	"MVC_DecList error, ret=%d\012\000" )
	.space	1
.LC458:
	ASCII(.ascii	"dec list err.\012\000" )
	.space	1
.LC459:
	ASCII(.ascii	"pMvcCtx->TotalNal = %d, type:%d\012\000" )
	.space	3
.LC460:
	ASCII(.ascii	"stop i want\012\000" )
	.space	3
.LC461:
	ASCII(.ascii	"***** NAL: IDR/Slice, nal_unit_type=%d, TotalSlice=" )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC462:
	ASCII(.ascii	"*******TotalPicNum=%d********\012\000" )
	.space	1
.LC463:
	ASCII(.ascii	"***** NAL: PPS, nal_unit_type=%d, TotalPPS=%d\012\000" )
	.space	1
.LC464:
	ASCII(.ascii	"PPS decode error.\012\000" )
	.space	1
.LC465:
	ASCII(.ascii	"***** NAL: SPS, nal_unit_type=%d, TotalSPS=%d\012\000" )
	.space	1
.LC466:
	ASCII(.ascii	"SPS decode error.\012\000" )
	.space	1
.LC467:
	ASCII(.ascii	"***** NAL: SEI, nal_unit_type=%d\012\000" )
	.space	2
.LC468:
	ASCII(.ascii	"SEI decode error.\012\000" )
	.space	1
.LC469:
	ASCII(.ascii	"***** NAL: AUD, nal_unit_type=%d\012\000" )
	.space	2
.LC470:
	ASCII(.ascii	"***** NAL: EOSEQ, nal_unit_type=%d\012\000" )
.LC471:
	ASCII(.ascii	"***** NAL: FILL, nal_unit_type=%d\012\000" )
	.space	1
.LC472:
	ASCII(.ascii	"***** NAL: EOSTREAM, nal_unit_type=%d\012\000" )
	.space	1
.LC473:
	ASCII(.ascii	"***** NAL: SPSEXT, nal_unit_type=%d\012\000" )
	.space	3
.LC474:
	ASCII(.ascii	"***** NAL: PREFIX, nal_unit_type=%d\012\000" )
	.space	3
.LC475:
	ASCII(.ascii	"***** NAL: SUBSPS, nal_unit_type=%d\012\000" )
	.space	3
.LC476:
	ASCII(.ascii	"***** NAL: AUX, nal_unit_type=%d\012\000" )
	.space	2
.LC477:
	ASCII(.ascii	"***** NAL: EOPIC, nal_unit_type=%d\012\000" )
.LC478:
	ASCII(.ascii	"***** NAL: UNSUPPORT, nal_unit_type=%d,nalu header:" )
	ASCII(.ascii	"%x\012\000" )
	.space	1
.LC479:
	ASCII(.ascii	"nal_header != 0x00000100 not support.\012\000" )
	.space	1
.LC480:
	ASCII(.ascii	"IsSizeChanged = %d,TotalPicNum = %d,old w/h = %d/%d" )
	ASCII(.ascii	",new w/h = %d/%d\012\000" )
	.space	3
.LC481:
	ASCII(.ascii	"MVC_FindTrailZeros ERR\012\000" )
.LC482:
	ASCII(.ascii	"store pic err, ret = %d\012\000" )
	.space	3
.LC483:
	ASCII(.ascii	"MVC_IMODE nal store pic err, ret = %d\012\000" )
	.space	1
.LC484:
	ASCII(.ascii	"MVC_IMODE nal flush dpb err, ret = %d\012\000" )
	.space	1
.LC485:
	ASCII(.ascii	"cann't find FrameStore\012\000" )
.LC486:
	ASCII(.ascii	"========== MVC FrameStore state(is_used, is_in_dpb," )
	ASCII(.ascii	" MVC_IsOutDPB) ========\012\000" )
.LC487:
	ASCII(.ascii	"ReadImgNum = %d, NewImgNum = %d\012\000" )
	.space	3
.LC488:
	ASCII(.ascii	"%02d: %d %d %d\012\000" )
.LC489:
	ASCII(.ascii	"FrameStore leak, MVC_ClearAll\012\000" )
	.bss
	.align	2
.LANCHOR2 = . + 0
.LANCHOR3 = . + 8184
	.type	pps_tmp.15565, %object
	.size	pps_tmp.15565, 2240
pps_tmp.15565:
	.space	2240
	.type	sps_tmp.15701, %object
	.size	sps_tmp.15701, 3992
sps_tmp.15701:
	.space	3992
	.type	MvcTmpBuf, %object
	.size	MvcTmpBuf, 68
MvcTmpBuf:
	.space	68
	.type	cnt.14518, %object
	.size	cnt.14518, 4
cnt.14518:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
