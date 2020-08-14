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
	add	ip, r0, #11075584
	mov	r5, r0
	add	r0, ip, #40960
	ldr	r2, [r5, #252]
	mov	r8, #2240
	ldr	lr, [r1, #180]
	ldr	r3, [r0, #1016]
	movw	r7, #3992
	ldr	r6, [r5, #248]
	mov	r4, r1
	mla	r2, r8, r3, r2
	bic	r3, lr, #768
	bic	r3, r3, #3
	ldr	r2, [r2, #28]
	str	r3, [r1, #180]
	mla	r1, r7, r2, r6
	ldrb	r2, [r1, #20]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L430
	add	r2, r5, #11141120
	add	r2, r2, #12288
	ldr	r2, [r2, #3472]
	mov	r2, r2, asl #4
	cmp	r2, #1920
	bhi	.L431
.L430:
	orr	r3, r3, #131072
	str	r3, [r4, #180]
.L432:
	ldr	r2, [r5, #224]
	ldr	r2, [r2, #592]
	cmp	r2, #1
	movne	r2, #0
	beq	.L485
.L433:
	orr	r3, r2, r3
	str	r3, [r4, #180]
	ldr	r2, [r5, #56]
	ldrb	lr, [r4, #2]	@ zero_extendqisi2
	mov	r2, r2, asl #10
	str	r2, [r4, #208]
	cmp	lr, #3
	ldrls	pc, [pc, lr, asl #2]
	b	.L474
.L436:
	.word	.L435
	.word	.L437
	.word	.L438
	.word	.L435
.L438:
	ldrb	ip, [r4]	@ zero_extendqisi2
	add	r2, r5, #11075584
	ldr	lr, [r4, #44]
	add	r2, r2, #45056
	and	ip, ip, #3
	orr	ip, ip, #2816
	orr	r3, ip, r3
	str	lr, [r4, #232]
	str	r3, [r4, #180]
.L434:
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
	ble	.L456
	ldrb	r3, [r4, #696]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L456
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cmp	r3, #3
	ldreq	r3, [r4, #180]
	biceq	r3, r3, #768
	streq	r3, [r4, #180]
.L456:
	ldrb	r3, [r2, #2616]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L457
	ldr	r3, [r2, #2632]
	cmp	r3, #3
	moveq	r3, #1
	streq	r3, [r4, #428]
	beq	.L457
	cmp	r3, #4
	moveq	r3, #2
	streq	r3, [r4, #428]
	beq	.L457
	cmp	r3, #5
	moveq	r3, #3
	streq	r3, [r4, #428]
	beq	.L457
	mov	r0, #0
	ldr	r1, .L489
	str	r0, [r4, #428]
	ldr	r2, [r2, #2632]
	bl	dprint_vfmw
.L457:
	ldr	r0, [r5, #120]
	ldrsb	r1, [r4, #6]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L489+4
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
.L437:
	ldrb	ip, [r4]	@ zero_extendqisi2
	add	r2, r5, #11075584
	ldr	lr, [r4, #44]
	add	r2, r2, #45056
	and	ip, ip, #3
	orr	ip, ip, #1792
	orr	r3, ip, r3
	str	lr, [r4, #232]
	str	r3, [r4, #180]
	b	.L434
.L435:
	ldrb	r2, [r4, #696]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L439
	ldr	lr, [r4, #40]
	mov	r7, #3072
	ldr	r2, [r4, #44]
	cmp	lr, #0
	ldrb	lr, [r4]	@ zero_extendqisi2
	moveq	r6, #512
	movne	r6, #768
	str	r2, [r4, #232]
.L440:
	ldr	r8, [r4, #748]
	ldr	r2, [r4, #784]
	cmp	r8, r2
	beq	.L486
	movle	r9, #1
	movgt	r9, #0
.L451:
	add	r2, ip, #45056
	ldrb	r8, [r2, #2652]	@ zero_extendqisi2
	cmp	r8, #3
	moveq	ip, #4096
	beq	.L452
	cmp	r8, #4
	movne	ip, r9, asl #12
	moveq	ip, #0
.L452:
	orr	r6, r7, r6
	bic	r3, r3, #12288
	orr	r3, r6, r3
	and	lr, lr, #3
	orr	lr, r3, lr
	orr	lr, lr, ip
	str	lr, [r4, #180]
	b	.L434
.L431:
	movw	r3, #64764
	movt	r3, 65533
	and	r3, r3, lr
	str	r3, [r4, #180]
	b	.L432
.L485:
	tst	r3, #131072
	movne	r2, #0
	moveq	r2, #262144
	b	.L433
.L439:
	ldr	r8, [r4, #756]
	ldr	r2, [r5, #116]
	cmp	r8, r2
	bhi	.L487
	ldr	lr, [r4, #792]
	cmp	r2, lr
	bcc	.L444
.L442:
	ldr	r2, [r4, #44]
	mov	r6, #3
	ldrb	lr, [r4]	@ zero_extendqisi2
	ldrb	r10, [r4, #735]	@ zero_extendqisi2
	ldrb	r9, [r4, #771]	@ zero_extendqisi2
	str	r2, [r4, #232]
.L445:
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L488
	cmp	r2, #2
	streq	r8, [r4, #232]
	movne	r7, r6, asl #10
	moveq	lr, r10
	moveq	r7, #1024
.L447:
	mov	r6, #768
	str	r10, [r4, #172]
	str	r9, [r4, #176]
	b	.L440
.L444:
	ldrb	lr, [r4, #735]	@ zero_extendqisi2
	mov	r6, #1
	ldrb	r9, [r4, #771]	@ zero_extendqisi2
	str	r8, [r4, #232]
	mov	r10, lr
	b	.L445
.L486:
	ldr	r2, [r4, #184]
	cmp	r2, #720
	movne	r9, #1
	bne	.L451
	ldr	r9, [r4, #188]
	subs	r9, r9, #480
	movne	r9, #1
	b	.L451
.L487:
	ldr	r7, [r4, #792]
	cmp	r2, r7
	bcc	.L442
	ldrb	lr, [r4, #771]	@ zero_extendqisi2
	mov	r6, #2
	str	r7, [r4, #232]
	ldrb	r10, [r4, #735]	@ zero_extendqisi2
	mov	r9, lr
	b	.L445
.L488:
	ldr	r2, [r4, #792]
	mov	lr, r9
	mov	r7, #2048
	str	r2, [r4, #232]
	b	.L447
.L474:
	add	r2, r5, #11075584
	add	r2, r2, #45056
	b	.L434
.L490:
	.align	2
.L489:
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
	beq	.L495
	ldrb	r2, [r0, #733]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L496
.L495:
	ldr	r2, [r0, #768]
	mov	r1, #0
	strb	r1, [r0, #697]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, r1
	beq	.L500
	ldr	r2, [r0, #732]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	movne	ip, #1
	bne	.L498
.L500:
	mov	ip, #0
.L498:
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
.L496:
	mov	r1, #1
	strb	r1, [r0, #697]
	b	.L500
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
	beq	.L531
	cmp	r2, #1
	beq	.L512
	cmp	r2, #2
	beq	.L513
	ldr	r9, [r6, #748]
	ldr	r10, [r6, #784]
.L514:
	add	r8, r5, #11075584
	add	r7, r8, #45056
	ldr	ip, [r7, #3084]
	cmp	ip, #0
	beq	.L523
	ldrb	r4, [r7, #2876]	@ zero_extendqisi2
	cmp	r4, #0
	movwne	r3, #47932
	movne	r4, #0
	movtne	r3, 169
	addne	r3, r5, r3
	bne	.L517
	b	.L515
.L521:
	ldrb	r2, [r3, #1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L515
.L517:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L521
.L523:
	mvn	r0, #0
.L527:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L513:
	ldr	r9, [r6, #784]
	mov	r10, r9
	b	.L514
.L515:
	ldrsb	r1, [r6, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L518
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L518
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
	ldr	r1, .L532
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
.L512:
	ldr	r9, [r6, #748]
	mov	r10, r9
	b	.L514
.L518:
	mov	r1, #0
	ldrsb	r2, [r6, #6]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	stmia	sp, {r0, r1}
	mov	r0, r1
	ldr	r1, .L532+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L527
.L531:
	mov	r0, r6
	movw	r3, #2085
	ldr	r2, .L532+8
	ldr	r1, .L532+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L527
.L533:
	.align	2
.L532:
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
	beq	.L559
.L534:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L559:
	ldr	r8, [r3, #2856]
	cmp	r8, #0
	beq	.L539
	movw	r6, #47716
	add	r1, r10, #11141120
	movt	r6, 169
	mov	r2, #0
	add	r1, r1, #12288
	add	r6, r10, r6
	mov	r5, r2
	mvn	r9, #-2147483648
.L538:
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	beq	.L537
	ldr	r7, [r4, #64]
	cmp	r9, r7
	bls	.L537
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	mov	r0, r4
	cmp	r3, #0
	beq	.L537
	str	r1, [fp, #-52]
	str	r2, [fp, #-48]
	bl	MVC_NonLongTermRefFlg
	ldr	r2, [fp, #-48]
	ldr	r1, [fp, #-52]
	cmp	r0, #0
	beq	.L537
	ldr	r0, [r4, #56]
	ldr	r3, [r1, #3496]
	cmp	r0, r3
	moveq	r9, r7
	moveq	r2, r5
.L537:
	add	r5, r5, #1
	cmp	r5, r8
	bne	.L538
.L536:
	movw	r3, #28314
	movt	r3, 42
	add	r3, r2, r3
	ldr	r4, [r10, r3, asl #2]
	cmp	r4, #0
	beq	.L534
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L534
	mov	r0, r4
	bl	MVC_NonLongTermRefFlg
	cmp	r0, #0
	beq	.L534
	add	r3, r10, #11141120
	ldr	r2, [r4, #56]
	add	r3, r3, #12288
	ldr	r3, [r3, #3496]
	cmp	r2, r3
	bne	.L534
	mov	r0, r10
	mov	r1, r4
	bl	MVC_UnMarkFrameStoreRef
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	MVC_UpdateReflist
.L539:
	mov	r2, r8
	b	.L536
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
	ldr	r3, .L570
	mov	r7, r0
	ldr	r3, [r3]
	tst	r3, #16384
	beq	.L560
	add	r6, r0, #11075584
	add	r6, r6, #45056
	ldr	r3, [r6, #2860]
	cmp	r3, #0
	movwne	r5, #47716
	movne	r4, #0
	movtne	r5, 169
	addne	r5, r0, r5
	beq	.L566
.L565:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L570+4
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
	bhi	.L565
.L566:
	ldr	r1, .L570+8
	mov	r0, #14
	bl	dprint_vfmw
	ldr	r3, [r6, #2864]
	cmp	r3, #0
	movwne	r5, #47780
	movne	r4, #0
	movtne	r5, 169
	addne	r5, r7, r5
	beq	.L564
.L567:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L570+12
	mov	r0, #14
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	ldr	r3, [ip, #20]
	str	lr, [sp, #4]
	ldr	ip, [ip, #32]
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r5]
	mov	r2, r4
	ldr	r1, .L570+16
	mov	r0, #14
	add	r4, r4, #1
	ldr	r3, [r3, #712]
	bl	dprint_vfmw
	ldr	r3, [r6, #2864]
	cmp	r3, r4
	bhi	.L567
.L564:
	ldr	r1, .L570+8
	mov	r0, #14
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L560:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L571:
	.align	2
.L570:
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
	bne	.L573
	ldrb	r2, [r3, #2781]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
.L573:
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
	beq	.L576
	bcc	.L577
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
	beq	.L594
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
	beq	.L585
	add	r1, r4, #672
	ldrd	r2, [r1]
	cmp	r3, r7
	cmpeq	r2, r6
	beq	.L595
.L585:
	ldrb	r3, [r4, #735]	@ zero_extendqisi2
.L592:
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
.L577:
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
.L576:
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
	beq	.L596
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
	beq	.L581
	add	r1, r4, #672
	ldrd	r2, [r1]
	cmp	r3, r7
	cmpeq	r2, r6
	streqd	r8, [r1]
.L581:
	ldrb	r3, [r4, #771]	@ zero_extendqisi2
	b	.L592
.L594:
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
.L596:
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
.L595:
	strd	r8, [r1]
	b	.L585
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
	bcc	.L598
	cmp	r5, #0
	beq	.L603
	movw	ip, #47716
	add	r6, r0, #11141120
	movt	ip, 169
	mov	r1, #0
	add	r8, r6, #12288
	add	ip, r0, ip
	mov	r3, r1
	mvn	r4, #-2147483648
.L601:
	ldr	r2, [ip, #4]!
	cmp	r2, #0
	beq	.L600
	ldr	lr, [r2, #64]
	cmp	r4, lr
	bls	.L600
	ldr	r0, [r2, #56]
	ldr	r2, [r8, #3496]
	cmp	r0, r2
	moveq	r4, lr
	moveq	r1, r3
.L600:
	add	r3, r3, #1
	cmp	r3, r5
	bne	.L601
	mov	r5, r1
.L599:
	movw	r3, #28314
	mov	r0, r7
	movt	r3, 42
	add	r3, r1, r3
	ldr	r1, [r7, r3, asl #2]
	bl	MVC_UnMarkFrameStoreRef
	mov	r1, r5
	mov	r0, r7
	bl	MVC_RemoveFrameStoreOutDPB
.L598:
	add	r3, r6, #12288
	mov	r1, #0
	ldr	r2, [r3, #3432]
	strb	r1, [r3, #2780]
	cmp	r2, r1
	movne	r2, #1
	strneb	r2, [r3, #2781]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L603:
	add	r6, r0, #11141120
	mov	r1, r5
	b	.L599
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
	beq	.L621
	add	r7, r5, #56
	mov	r4, r5
	mov	r3, #0
	strb	r3, [r5, #1]
	strb	r3, [r5, #3]
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
	movw	r3, #3998
	ldr	r2, .L622+4
	ldr	r1, .L622+8
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L623:
	.align	2
.L622:
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
	beq	.L625
	ldr	r0, [r0, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L625:
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
	ldr	r1, .L634
	mov	r4, r0
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L630
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L630:
	ldr	r3, [r4, #80]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #80]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L635:
	.align	2
.L634:
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
	b	.L638
.L637:
	add	r4, r4, #88
	cmp	r4, r5
	beq	.L640
.L638:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L637
	mov	r1, r4
	ldr	r0, [r6, #120]
	add	r4, r4, #88
	bl	MVC_ReleaseNAL
	cmp	r4, r5
	bne	.L638
.L640:
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
.L643:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L642
	ldr	r0, [r6, #120]
	bl	MVC_ReleaseNAL
	str	r7, [r4]
.L642:
	cmp	r4, r5
	bne	.L643
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
	ldr	r3, .L659
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
	beq	.L650
	add	r2, r4, #11075584
	ldrb	r3, [fp, #4]	@ zero_extendqisi2
	add	r2, r2, #45056
	mov	r1, #5
	strb	r0, [fp, #-56]
	ldr	r2, [r2, #2856]
	strb	r1, [fp, #-55]
	add	r2, r2, #3
	strb	r2, [fp, #-57]
.L656:
	sub	r1, fp, #60
	ldr	r0, [r4, #120]
	mov	r5, #0
	strb	r3, [fp, #-54]
	strb	r5, [fp, #-58]
	bl	FSP_ConfigInstance
	cmp	r0, r5
	bne	.L651
	ldr	r1, [r4, #224]
	sub	r3, fp, #64
	ldr	r0, [r4, #120]
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
	add	r0, r4, #584
	bl	ResetVoQueue
.L653:
	ldr	r1, .L659+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L657
.L651:
	mov	r0, r5
	ldr	r1, .L659+8
	bl	dprint_vfmw
	mvn	r0, #19
	b	.L657
.L660:
	.align	2
.L659:
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
	beq	.L663
	bcc	.L678
	cmp	r3, #2
	bne	.L684
.L679:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L675:
	mov	r0, r7
.L684:
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L663:
	mov	r0, #2
.L664:
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
	beq	.L666
.L686:
	ldr	r10, [lr, #256]
	cmp	r10, #0
	bne	.L680
	mov	ip, r5
	mov	r3, r10
	b	.L668
.L669:
	ldr	r1, [ip, #4]!
	cmp	r1, #0
	bne	.L671
.L668:
	add	r3, r3, #1
	cmp	r3, r2
	bne	.L669
	ldr	r3, [r7, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L675
	ldr	r1, [r8, #3416]
	add	r1, r1, #696
.L671:
	mov	ip, r5
	mov	r3, #0
	b	.L677
.L673:
	ldr	r10, [ip, #4]!
.L677:
	add	r3, r3, #1
	cmp	r10, #0
	streq	r1, [ip]
	cmp	r3, r2
	bne	.L673
.L674:
	add	r4, r4, #1
	add	r5, r5, #132
	cmp	r0, r4
	add	lr, lr, #132
	bls	.L679
	ldr	r2, [r6, #4]!
	cmp	r2, #0
	bne	.L686
.L666:
	ldr	r3, [r7, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L675
	str	r9, [r6]
	ldr	r3, [r8, #3416]
	add	r3, r3, #696
	str	r3, [lr, #256]
	b	.L674
.L680:
	mov	r1, r10
	b	.L671
.L678:
	mov	r0, #1
	b	.L664
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
	beq	.L688
	ldrb	r2, [r2, #1002]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r4, #2
	moveq	r4, #1
.L688:
	add	r3, r3, #45056
	ldr	r5, [r3, #2864]
	cmp	r5, #0
	beq	.L696
	movw	r3, #47784
	mov	r2, #0
	movt	r3, 169
	add	r3, r0, r3
	b	.L693
.L706:
	ldrb	ip, [r0, #3]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L704
.L691:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L705
.L693:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L706
	ldrb	lr, [r0, #3]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L692
	ldr	ip, [r0, #732]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	beq	.L707
.L692:
	tst	lr, #2
	beq	.L691
	ldr	ip, [r0, #768]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L691
	ldr	ip, [r0, #780]
	cmp	ip, r1
	bne	.L691
	add	r0, r0, #768
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L704:
	ldr	ip, [r0, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L691
	ldr	ip, [r0, #708]
	cmp	ip, r1
	bne	.L691
	add	r0, r0, #696
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L707:
	ldr	ip, [r0, #744]
	cmp	ip, r1
	bne	.L692
	add	r0, r0, #732
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L705:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L696:
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
	beq	.L709
	ldrb	r2, [r2, #1002]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r4, #2
	moveq	r4, #1
.L709:
	add	r3, r3, #45056
	ldr	r5, [r3, #2868]
	cmp	r5, #0
	beq	.L717
	movw	r3, #47848
	mov	r2, #0
	movt	r3, 169
	add	r3, r0, r3
	b	.L714
.L727:
	ldrb	ip, [r0, #3]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L725
.L712:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L726
.L714:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L727
	ldrb	lr, [r0, #3]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L713
	ldr	ip, [r0, #732]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	beq	.L728
.L713:
	tst	lr, #2
	beq	.L712
	ldr	ip, [r0, #768]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L712
	ldr	ip, [r0, #776]
	cmp	ip, r1
	bne	.L712
	add	r0, r0, #768
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L725:
	ldr	ip, [r0, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L712
	ldr	ip, [r0, #704]
	cmp	ip, r1
	bne	.L712
	add	r0, r0, #696
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L728:
	ldr	ip, [r0, #740]
	cmp	ip, r1
	bne	.L713
	add	r0, r0, #732
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L726:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L717:
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
	ble	.L730
	add	lr, r6, r3, lsl #2
	mov	ip, r3
.L731:
	ldr	r4, [lr, #-4]!
	sub	ip, ip, #1
	str	r4, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L731
.L730:
	add	r2, r4, #1
	str	r2, [r7]
	str	r0, [r6, r4, asl #2]
	ldr	ip, [r7]
	cmp	r3, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r6, ip, lsl #2
	mov	lr, ip
	b	.L735
.L734:
	str	r0, [r6, lr, asl #2]
	add	lr, lr, #1
.L733:
	cmp	ip, r2
	beq	.L743
.L735:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L733
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L734
	ldr	r3, [r0, #12]
	cmp	r3, r5
	bne	.L734
	ldr	r3, [r0, #4]
	ldr	r3, [r3, #56]
	cmp	r3, r1
	bne	.L734
	cmp	ip, r2
	bne	.L735
.L743:
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
	ble	.L745
	add	lr, r6, r3, lsl #2
	mov	ip, r3
.L746:
	ldr	r4, [lr, #-4]!
	sub	ip, ip, #1
	str	r4, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L746
.L745:
	add	r2, r4, #1
	str	r2, [r7]
	str	r0, [r6, r4, asl #2]
	ldr	ip, [r7]
	cmp	r3, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r6, ip, lsl #2
	mov	lr, ip
	b	.L750
.L749:
	str	r0, [r6, lr, asl #2]
	add	lr, lr, #1
.L748:
	cmp	ip, r2
	beq	.L761
.L750:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L748
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L749
	ldr	r3, [r0, #8]
	cmp	r3, r5
	bne	.L749
	ldr	r3, [r0, #4]
	ldr	r3, [r3, #56]
	cmp	r3, r1
	bne	.L749
	cmp	ip, r2
	bne	.L750
.L761:
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
	beq	.L769
	ldr	ip, [ip, #2388]
	cmp	r1, ip
	beq	.L770
	mov	r6, r5
	mov	ip, #0
	b	.L764
.L766:
	ldr	r4, [lr, #4]!
	cmp	r1, r4
	beq	.L772
.L764:
	add	ip, ip, #1
	cmp	ip, r5
	bne	.L766
	cmp	r6, #0
	blt	.L773
.L763:
	cmp	r2, #0
	add	r6, r6, r3, lsl #1
	movwne	r3, #5718
	movweq	r3, #5726
	movt	r3, 41
	add	r3, r6, r3
	add	r0, r0, r3, lsl #2
	ldr	r0, [r0, #4]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L772:
	mov	r6, ip
	cmp	r6, #0
	bge	.L763
.L773:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L769:
	mov	r6, r5
	b	.L763
.L770:
	mov	r6, #0
	b	.L763
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
	beq	.L780
	add	r4, r0, #11141120
	mov	r3, #0
	mov	r0, ip
	add	r4, r4, #12288
	b	.L779
.L776:
	cmp	r3, lr
	add	r0, r0, #808
	beq	.L782
.L779:
	ldr	ip, [r0, #56]
	add	r3, r3, #1
	cmp	ip, r1
	bne	.L776
	ldrb	ip, [r4, #2779]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L777
	ldr	ip, [r0, #716]
	cmp	ip, r2
	bne	.L776
	add	r0, r0, #696
	ldmfd	sp, {r4, fp, sp, pc}
.L777:
	cmp	ip, #1
	beq	.L783
	cmp	ip, #2
	bne	.L776
	ldr	ip, [r0, #788]
	cmp	ip, r2
	bne	.L776
	add	r0, r0, #768
	ldmfd	sp, {r4, fp, sp, pc}
.L783:
	ldr	ip, [r0, #752]
	cmp	ip, r2
	bne	.L776
	add	r0, r0, #732
	ldmfd	sp, {r4, fp, sp, pc}
.L782:
	mov	r0, #0
	ldmfd	sp, {r4, fp, sp, pc}
.L780:
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
	bls	.L786
	add	lr, r4, r1, lsl #2
	mov	ip, r1
.L787:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r3, [r7]
	cmp	r3, ip
	bcc	.L787
.L786:
	add	r2, r3, #1
	str	r2, [r7]
	str	r0, [r4, r3, asl #2]
	ldr	ip, [r7]
	cmp	r1, ip
	ldmccfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r3, r4, ip, lsl #2
	mov	lr, ip
	b	.L791
.L789:
	cmp	r1, ip
	str	r0, [r4, lr, asl #2]
	add	lr, lr, #1
	bcc	.L798
.L791:
	ldr	r0, [r3], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L789
	ldr	r2, [r0, #4]
	ldr	r2, [r2, #56]
	cmp	r2, r5
	bne	.L789
	ldr	r2, [r0, #20]
	cmp	r2, r6
	bne	.L789
	cmp	r1, ip
	bcs	.L791
.L798:
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
	beq	.L800
	ldrb	r3, [r9, #1002]	@ zero_extendqisi2
	cmp	r3, r2
	movne	r3, #2
	moveq	r3, #1
.L800:
	cmp	r10, #1
	bls	.L849
.L799:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L849:
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
	beq	.L799
	ldr	r2, [r1]
	cmp	r2, #3
	beq	.L799
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
	b	.L807
.L808:
	cmp	r2, #2
	beq	.L850
	ldr	r3, [fp, #-84]
	cmp	r3, #1
	beq	.L814
	ldr	r3, [fp, #-68]
	ldrb	r3, [r3, #1009]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	mov	r3, r10, asl #1
.L815:
	cmp	r2, #4
	ldr	r2, [r7]
	beq	.L851
	add	r2, r2, #1
	add	r4, r2, r4
	ldr	r2, [fp, #-76]
	cmp	r2, r4
	rsble	r4, r2, r4
.L825:
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
.L812:
	ldr	r3, [fp, #-72]
	add	r6, r6, #1
	cmp	r6, r3
	beq	.L799
	ldr	r2, [r9, #4]!
	add	r7, r7, #4
	add	r8, r8, #4
	cmp	r2, #3
	beq	.L799
.L807:
	cmp	r2, #1
	bhi	.L808
	cmp	r2, #0
	ldr	r3, [r8]
	bne	.L809
	ldr	r2, [fp, #-60]
	mvn	r3, r3
	adds	r3, r2, r3
	str	r3, [fp, #-60]
	bmi	.L852
.L810:
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
	b	.L812
.L851:
	mvn	r2, r2
	adds	r4, r2, r4
	ldrmi	r2, [fp, #-76]
	addmi	r4, r4, r2
	b	.L825
.L814:
	add	r3, r5, #10747904
	ldr	r1, [fp, #-68]
	add	r3, r3, #20480
	ldr	ip, [r3, #2384]
	ldr	r1, [r1, #2640]
	adds	ip, ip, #1
	beq	.L816
	ldr	r3, [r3, #2388]
	cmp	r1, r3
	beq	.L817
	ldr	r0, [fp, #-96]
	mov	r3, #0
	str	ip, [fp, #-80]
	b	.L818
.L820:
	ldr	lr, [r0, #4]!
	cmp	r1, lr
	beq	.L834
.L818:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L820
.L819:
	ldr	r3, [fp, #-68]
	mov	r4, ip
	ldr	r0, [fp, #-96]
	ldrb	r3, [r3, #1009]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	mov	r3, #0
	b	.L830
.L822:
	ldr	lr, [r0, #4]!
	cmp	r1, lr
	beq	.L835
.L830:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L822
	cmp	r4, #0
	blt	.L836
.L829:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	mov	r3, r10, asl #1
	beq	.L823
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
	b	.L815
.L809:
	ldr	r2, [fp, #-60]
	add	r3, r3, #1
	add	r3, r2, r3
	ldr	r2, [fp, #-88]
	str	r3, [fp, #-60]
	cmp	r2, r3
	rsble	r3, r2, r3
	strle	r3, [fp, #-60]
	b	.L810
.L850:
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
	b	.L812
.L834:
	str	r3, [fp, #-80]
	b	.L819
.L835:
	mov	r4, r3
	cmp	r4, #0
	bge	.L829
.L836:
	mov	r3, #0
	mvn	r4, #0
	mov	r1, r3
	str	r3, [fp, #-84]
	str	r1, [fp, #-76]
	mov	r3, r10, asl #1
	b	.L815
.L823:
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
	b	.L815
.L852:
	ldr	r2, [fp, #-88]
	add	r3, r3, r2
	str	r3, [fp, #-60]
	b	.L810
.L816:
	ldr	r3, [fp, #-68]
	mov	r4, ip
	str	ip, [fp, #-80]
	ldrb	r3, [r3, #1009]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	b	.L829
.L817:
	ldr	r1, [fp, #-68]
	mov	r3, #0
	mov	r4, r3
	str	r3, [fp, #-80]
	ldrb	r1, [r1, #1009]	@ zero_extendqisi2
	str	r1, [fp, #-56]
	b	.L829
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
	bne	.L865
.L856:
	ldr	r3, [r5, #1060]
	cmp	r1, #1
	add	r3, r3, #1
	str	r3, [r5, #1048]
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldrb	r3, [r7, #4045]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L866
.L858:
	add	r4, r4, #40960
	ldr	r3, [r4, #1064]
	add	r3, r3, #1
	str	r3, [r4, #1052]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L865:
	mov	r1, #0
	ldr	r2, [r5, #1060]
	bl	MVC_ReorderRefPiclist
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	b	.L856
.L866:
	ldr	r2, [r5, #1064]
	mov	r0, r6
	bl	MVC_ReorderRefPiclist
	b	.L858
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
	ldr	lr, .L930
	movcc	r4, r2
	ldr	ip, .L930+4
	movcs	r4, #31
	cmp	r6, #0
	str	r3, [fp, #-52]
	movne	r6, lr
	moveq	r6, ip
	cmp	r0, #1
	moveq	r5, #0
	moveq	r10, r5
	beq	.L870
	cmp	r0, #2
	moveq	r5, #0
	moveq	r10, r5
	bne	.L878
.L879:
	cmp	r10, r4
	movcs	r3, #0
	movcc	r3, #1
	cmp	r5, r4
	str	r3, [fp, #-48]
	movcs	r2, #0
	movcc	r2, #1
	orrs	r3, r2, r3
	beq	.L878
	cmp	r2, #0
	beq	.L884
	add	r8, r7, r5, lsl #2
	b	.L883
.L880:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L884
.L883:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	tst	ip, #2
	beq	.L880
	add	r0, r0, #768
	blx	r6
	cmp	r0, #0
	beq	.L880
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
.L884:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r10, lsl #2
	bne	.L886
	b	.L879
.L885:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L879
.L886:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	tst	r2, #1
	beq	.L885
	add	r0, r0, #732
	blx	r6
	cmp	r0, #0
	beq	.L885
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
	b	.L879
.L929:
	add	r0, r0, #732
	blx	r6
	cmp	r0, #0
	beq	.L871
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
.L875:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r5, lsl #2
	bne	.L877
	b	.L870
.L876:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L870
.L877:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	tst	r2, #2
	beq	.L876
	add	r0, r0, #768
	blx	r6
	cmp	r0, #0
	beq	.L876
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
.L870:
	cmp	r10, r4
	movcs	r2, #0
	movcc	r2, #1
	cmp	r5, r4
	movcs	r3, #0
	movcc	r3, #1
	str	r3, [fp, #-48]
	orrs	r3, r3, r2
	beq	.L878
	cmp	r2, #0
	beq	.L875
	add	r9, r7, r10, lsl #2
	b	.L874
.L871:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L875
.L874:
	mov	r8, r9
	ldr	r0, [r9], #4
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L871
	b	.L929
.L878:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L931:
	.align	2
.L930:
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
.L935:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L933
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L943
.L933:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L935
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L943:
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
	beq	.L948
	ldr	r3, [r0]
	cmp	r3, r2
	beq	.L949
	mov	ip, r0
	mov	r3, #0
	mov	r0, r1
	b	.L946
.L947:
	ldr	lr, [ip, #4]!
	cmp	lr, r2
	beq	.L950
.L946:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L947
	ldmfd	sp, {fp, sp, pc}
.L950:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L948:
	mov	r0, r1
	ldmfd	sp, {fp, sp, pc}
.L949:
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
	beq	.L955
	ldr	r3, [r1]
	cmp	r3, r0
	beq	.L956
	mov	r3, #0
	b	.L953
.L954:
	ldr	ip, [r1, #4]!
	cmp	ip, r0
	beq	.L952
.L953:
	add	r3, r3, #1
	cmp	r3, r2
	mov	lr, r3
	bne	.L954
.L952:
	cmp	r2, #0
	cmpne	r2, lr
	movhi	r0, #1
	movls	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L955:
	mov	lr, r2
	b	.L952
.L956:
	mov	lr, #0
	b	.L952
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
	beq	.L973
	cmp	r0, #2
	ldmnefd	sp, {fp, sp, pc}
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r0, [ip]
	add	r1, r1, #768
	mov	lr, #0
.L964:
	add	lr, lr, #1
	str	r1, [r3, r0, asl #2]
	cmp	lr, r2
	ldr	r0, [ip]
	add	r1, r1, #808
	add	r0, r0, #1
	str	r0, [ip]
	bne	.L964
	ldmfd	sp, {fp, sp, pc}
.L973:
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r0, [ip]
	add	r1, r1, #732
	mov	lr, #0
.L960:
	add	lr, lr, #1
	str	r1, [r3, r0, asl #2]
	cmp	lr, r2
	ldr	r0, [ip]
	add	r1, r1, #808
	add	r0, r0, #1
	str	r0, [ip]
	bne	.L960
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
	beq	.L993
	ldr	r2, [lr, #2388]
	cmp	r7, r2
	beq	.L994
	mov	r2, #0
	b	.L976
.L977:
	ldr	lr, [r0, #4]!
	cmp	r7, lr
	beq	.L975
.L976:
	add	r2, r2, #1
	cmp	r2, r6
	mov	r4, r2
	bne	.L977
.L975:
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
	bmi	.L974
	movw	r5, #28314
	movt	r5, 42
	add	r5, r2, r5
	mov	r2, r3
	mov	r3, r9
	add	r5, r9, r5, lsl #2
	mov	r9, r2
	b	.L991
.L1017:
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r2, #3
	ldreq	r0, [r1, #716]
	bne	.L981
.L983:
	add	r2, r1, r8
	ldrb	r2, [r2, #12]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L981
	ldr	r2, [r6, #3436]
	cmp	r2, r0
	beq	.L1016
.L981:
	subs	r4, r4, #1
	bmi	.L974
.L991:
	ldr	r1, [r5, #-4]!
	cmp	r1, #0
	beq	.L981
	ldrb	r2, [r6, #2779]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1017
	cmp	r2, #1
	beq	.L1018
	cmp	r2, #2
	bne	.L981
	ldrb	r0, [r6, #2777]	@ zero_extendqisi2
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	ubfx	r2, r2, #1, #1
	beq	.L986
	ldr	r0, [r6, #3416]
	ldr	ip, [r1, #752]
	ldr	r0, [r0, #752]
	cmp	ip, r0
	movne	r2, #0
	andeq	r2, r2, #1
.L986:
	cmp	r2, #0
	ldrne	r0, [r1, #788]
	bne	.L983
	subs	r4, r4, #1
	bpl	.L991
.L974:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1018:
	ldrb	r0, [r6, #2777]	@ zero_extendqisi2
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	and	r2, r2, #1
	beq	.L985
	ldr	r0, [r6, #3416]
	ldr	ip, [r1, #788]
	ldr	r0, [r0, #788]
	cmp	ip, r0
	movne	r2, #0
	andeq	r2, r2, #1
.L985:
	cmp	r2, #0
	ldrne	r0, [r1, #752]
	bne	.L983
	b	.L981
.L1016:
	cmp	r7, #0
	ldr	lr, [r1, #56]
	beq	.L981
	ldr	r2, [r10]
	cmp	lr, r2
	beq	.L987
	mov	r0, r10
	mov	r2, #0
.L988:
	add	r2, r2, #1
	cmp	r2, r7
	beq	.L981
	ldr	ip, [r0, #4]!
	cmp	lr, ip
	bne	.L988
.L987:
	str	r3, [fp, #-52]
	mov	r2, #808
	ldr	r3, .L1019
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
	beq	.L974
	add	r2, r2, #1
	str	r2, [r9]
	b	.L981
.L993:
	mov	r4, r6
	b	.L975
.L994:
	mov	r4, #0
	b	.L975
.L1020:
	.align	2
.L1019:
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
	bne	.L1022
	cmp	r4, r3
	add	r3, r2, #45056
	str	r3, [fp, #-240]
	bne	.L1180
	ldr	ip, [r3, #2864]
	cmp	ip, #0
	beq	.L1106
	movw	r1, #47780
	mov	r3, r4
	movt	r1, 169
	add	r1, r0, r1
	b	.L1028
.L1027:
	cmp	r3, ip
	beq	.L1026
.L1028:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1027
	ldr	r0, [r2, #696]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #65536
	addeq	r0, r4, #64
	addeq	r2, r2, #696
	addeq	r4, r4, #1
	streq	r2, [r6, r0, asl #2]
	cmp	r3, ip
	bne	.L1028
.L1026:
	add	r3, r6, #256
	mov	r1, r4
	str	r3, [fp, #-244]
	mov	r2, #4
	mov	r0, r3
	ldr	r3, .L1190
	bl	qsort
	ldr	r3, [fp, #-240]
	str	r4, [r5, #1048]
	ldr	r1, [r3, #2868]
	cmp	r1, #0
	beq	.L1107
	movw	r0, #47844
	mov	r8, r4
	movt	r0, 169
	mov	r3, #0
	add	r0, r6, r0
	b	.L1031
.L1030:
	cmp	r3, r1
	beq	.L1181
.L1031:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #3]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1030
	ldr	ip, [r2, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	addeq	ip, r8, #64
	addeq	r2, r2, #696
	addeq	r8, r8, #1
	streq	r2, [r6, ip, asl #2]
	cmp	r3, r1
	bne	.L1031
.L1181:
	rsb	r1, r4, r8
.L1029:
	add	r0, r4, #64
	ldr	r3, .L1190+4
	mov	r2, #4
	add	r0, r6, r0, lsl #2
	bl	qsort
	str	r8, [r5, #1048]
	b	.L1032
.L1022:
	cmp	r4, #0
	beq	.L1049
	add	r2, r2, #45056
	str	r2, [fp, #-240]
	ldr	ip, [r2, #2864]
	cmp	ip, #0
	beq	.L1182
	movw	r9, #47780
	mov	r4, r3
	movt	r9, 169
	add	r9, r0, r9
	mov	r1, r9
.L1068:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1067
	sub	r0, fp, #44
	ldr	lr, [r7, #3436]
	add	r8, r0, r4, lsl #2
	ldr	r0, [r2, #32]
	cmp	lr, r0
	strge	r2, [r8, #-192]
	addge	r4, r4, #1
.L1067:
	cmp	r3, ip
	bne	.L1068
	sub	r10, fp, #236
	ldr	r3, .L1190+8
	mov	r2, #4
	mov	r1, r4
	mov	r0, r10
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2864]
	cmp	r8, #0
	beq	.L1177
.L1104:
	mov	r3, r9
	mov	r2, #0
	mov	r9, r4
.L1071:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1070
	sub	r0, fp, #44
	ldr	ip, [r7, #3436]
	add	lr, r0, r9, lsl #2
	ldr	r0, [r1, #32]
	cmp	ip, r0
	strlt	r1, [lr, #-192]
	addlt	r9, r9, #1
.L1070:
	cmp	r2, r8
	bne	.L1071
	rsb	r8, r4, r9
	mov	r3, r4, asl #2
	add	r0, r10, r3
	str	r3, [fp, #-244]
	mov	r1, r8
	ldr	r3, .L1190+12
	mov	r2, #4
	bl	qsort
	cmp	r4, #0
	subeq	ip, fp, #172
	beq	.L1072
.L1099:
	sub	ip, fp, #172
	mov	r2, r10
	add	r8, ip, r8, lsl #2
	mov	r3, #0
.L1073:
	add	r3, r3, #1
	ldr	r1, [r2], #4
	cmp	r3, r4
	str	r1, [r8], #4
	bcc	.L1073
.L1072:
	cmp	r9, r4
	bls	.L1074
	ldr	r3, [fp, #-244]
	add	r2, r10, r3
	mov	r3, ip
.L1075:
	add	r4, r4, #1
	ldr	r1, [r2], #4
	cmp	r4, r9
	str	r1, [r3], #4
	bne	.L1075
.L1074:
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
	beq	.L1076
	movw	r3, #47844
	sub	ip, fp, #108
	movt	r3, 169
	add	r3, r6, r3
	mov	r2, ip
.L1077:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r9
	str	r1, [r2], #4
	bne	.L1077
.L1076:
	mov	r0, ip
	ldr	r3, .L1190+16
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
.L1066:
	cmp	r4, #1
	movls	r3, #0
	andhi	r3, r3, #1
	cmp	r3, #0
	beq	.L1078
	cmp	r4, #0
	beq	.L1079
	ldr	lr, [r6, #256]
	ldr	r3, [r6, #388]
	cmp	lr, r3
	bne	.L1078
	ldr	r1, [fp, #-244]
	add	r2, r6, #388
	mov	r3, #0
	b	.L1080
.L1081:
	ldr	ip, [r1, #4]!
	ldr	r0, [r2, #4]!
	cmp	ip, r0
	bne	.L1078
.L1080:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L1081
.L1082:
	ldr	r3, [r6, #392]
	str	lr, [r6, #392]
	str	r3, [r6, #388]
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
.L1100:
	cmp	r3, #0
	bne	.L1169
.L1083:
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
	bne	.L1183
	ldr	r1, [r5, #2648]
	ldr	ip, [r5, #1048]
	cmp	r1, #0
	beq	.L1085
	add	r2, ip, #63
	movw	r3, #32056
	movt	r3, 169
	add	r3, r6, r3
	add	r2, r6, r2, lsl #2
.L1086:
	add	r0, r0, #1
	str	r3, [r2, #4]!
	cmp	r0, r1
	add	r3, r3, #808
	bne	.L1086
	add	ip, ip, r0
.L1085:
	str	ip, [r5, #1048]
.L1087:
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1184
.L1170:
	ldr	r0, [r5, #1048]
.L1084:
	cmp	r1, #0
	bne	.L1042
	cmp	r0, #0
	ldreq	r1, .L1190+20
	beq	.L1171
.L1041:
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
	bhi	.L1093
.L1101:
	add	r1, r2, #63
	mov	r0, #0
	add	r1, r6, r1, lsl #2
.L1094:
	add	r2, r2, #1
	str	r0, [r1, #4]!
	cmp	r2, #32
	bls	.L1094
.L1093:
	cmp	r3, #32
	bhi	.L1097
	add	r3, r3, #96
	add	r1, r6, #516
	mov	r2, #0
	add	r6, r6, r3, lsl #2
.L1096:
	str	r2, [r6, #4]!
	cmp	r6, r1
	bne	.L1096
.L1097:
	mov	r0, #0
.L1163:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1049:
	add	r3, r2, #45056
	str	r3, [fp, #-240]
	ldr	r10, [r3, #2864]
	cmp	r10, #0
	beq	.L1052
	movw	r9, #47780
	mov	r3, r4
	movt	r9, 169
	add	r9, r0, r9
	mov	r1, r9
	b	.L1054
.L1053:
	cmp	r3, r10
	beq	.L1185
.L1054:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1053
	ldr	r0, [r2, #712]
	add	lr, r4, #64
	ldr	ip, [r7, #3440]
	add	r2, r2, #696
	cmp	ip, r0
	strge	r2, [r6, lr, asl #2]
	addge	r4, r4, #1
	cmp	r3, r10
	bne	.L1054
.L1185:
	add	r3, r6, #256
	mov	r2, #4
	str	r3, [fp, #-244]
	mov	r1, r4
	mov	r0, r3
	ldr	r3, .L1190+24
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2864]
	cmp	r8, #0
	beq	.L1176
.L1102:
	mov	r3, r9
	mov	r10, r4
	mov	r2, #0
	b	.L1057
.L1056:
	cmp	r2, r8
	beq	.L1186
.L1057:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1056
	ldr	r0, [r1, #712]
	add	lr, r10, #64
	ldr	ip, [r7, #3440]
	add	r1, r1, #696
	cmp	ip, r0
	strlt	r1, [r6, lr, asl #2]
	addlt	r10, r10, #1
	cmp	r2, r8
	bne	.L1057
.L1186:
	rsb	r8, r4, r10
	add	r0, r4, #64
	ldr	r3, .L1190+28
	mov	r2, #4
	mov	r1, r8
	add	r0, r6, r0, lsl #2
	bl	qsort
	add	r9, r10, #64
	cmp	r4, #0
	add	r9, r6, r9, lsl #2
	beq	.L1059
.L1098:
	add	r8, r8, #96
	add	r2, r6, #252
	mov	r3, #0
	add	r8, r6, r8, lsl #2
.L1060:
	add	r3, r3, #1
	ldr	r1, [r2, #4]!
	cmp	r3, r4
	str	r1, [r8, #4]!
	bcc	.L1060
.L1059:
	cmp	r10, r4
	bls	.L1061
	add	r3, r4, #63
	add	r2, r6, #384
	add	r3, r6, r3, lsl #2
.L1062:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r10
	str	r1, [r2, #4]!
	bne	.L1062
.L1061:
	ldr	r3, [fp, #-240]
	str	r10, [r5, #1052]
	str	r10, [r5, #1048]
	ldr	r1, [r3, #2868]
	cmp	r1, #0
	beq	.L1108
	movw	r0, #47844
	mov	r4, r10
	movt	r0, 169
	mov	r3, #0
	add	r0, r6, r0
	b	.L1065
.L1064:
	cmp	r3, r1
	beq	.L1187
.L1065:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1064
	ldrb	ip, [r2, #697]	@ zero_extendqisi2
	cmp	ip, #1
	addeq	ip, r6, r4, lsl #2
	addeq	r2, r2, #696
	addeq	r4, r4, #1
	streq	r2, [ip, #256]
	streq	r2, [ip, #388]
	cmp	r3, r1
	bne	.L1065
.L1187:
	rsb	r1, r10, r4
.L1063:
	mov	r0, r9
	ldr	r3, .L1190+4
	mov	r2, #4
	bl	qsort
	ldr	r1, [r5, #1048]
	ldr	r3, .L1190+4
	mov	r2, #4
	add	r0, r1, #97
	rsb	r1, r1, r4
	add	r0, r6, r0, lsl #2
	bl	qsort
	str	r4, [r5, #1052]
	str	r4, [r5, #1048]
	mov	r3, #1
	b	.L1066
.L1180:
	ldr	lr, [r3, #2864]
	cmp	lr, #0
	moveq	r9, lr
	beq	.L1025
	movw	r1, #47780
	mov	r3, r9
	movt	r1, 169
	add	r1, r0, r1
.L1034:
	ldr	r2, [r1, #4]!
	sub	r0, fp, #44
	add	ip, r0, r9, lsl #2
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #0
	strne	r2, [ip, #-192]
	addne	r9, r9, #1
	cmp	r3, lr
	bne	.L1034
.L1025:
	mov	r1, r9
	ldr	r3, .L1190+32
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
	beq	.L1035
	movw	r3, #47844
	sub	ip, fp, #108
	movt	r3, 169
	add	r3, r6, r3
	mov	r2, ip
.L1036:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r9
	str	r1, [r2], #4
	bne	.L1036
.L1035:
	mov	r0, ip
	ldr	r3, .L1190+16
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
.L1032:
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
	mov	r2, #0
	str	r2, [r5, #1052]
	cmp	r3, r2
	beq	.L1083
	ldr	r0, [r5, #1048]
	cmp	r0, #0
	bne	.L1188
	ldr	r3, [r6, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L1043
	ldrb	r3, [r7, #2779]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1044
	ldr	r3, [r7, #3416]
	add	r3, r3, #696
	str	r3, [r6, #256]
.L1045:
	mov	r3, #1
	str	r3, [r5, #1048]
.L1169:
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	b	.L1170
.L1188:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1041
.L1042:
	cmp	r0, #0
	bne	.L1041
	ldr	r1, [r5, #1052]
	cmp	r1, #0
	beq	.L1189
	ldr	r3, [r5, #1064]
	mov	r2, r0
	str	r0, [r5, #1048]
	add	r3, r3, #1
	cmp	r3, r1
	movcs	r3, r1
	str	r3, [r5, #1052]
	b	.L1101
.L1078:
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
	b	.L1100
.L1183:
	movw	ip, #42008
	ldr	r2, [r5, #2648]
	ldr	r3, [fp, #-244]
	mov	r1, r4
	movt	ip, 169
	add	ip, r6, ip
	str	ip, [sp]
	bl	MVC_GenPiclistfromFrmlist_Interview
	b	.L1087
.L1044:
	cmp	r3, #1
	ldr	r3, [r7, #3416]
	addeq	r3, r3, #732
	addne	r3, r3, #768
	str	r3, [r6, #256]
	b	.L1045
.L1079:
	ldr	lr, [r6, #388]
	b	.L1082
.L1184:
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
	bne	.L1089
	ldr	r1, [r5, #2652]
	ldr	ip, [r5, #1052]
	cmp	r1, #0
	beq	.L1090
	add	r2, ip, #96
	movw	r3, #33672
	movt	r3, 169
	add	r3, r6, r3
	add	r2, r6, r2, lsl #2
.L1091:
	add	r0, r0, #1
	str	r3, [r2, #4]!
	cmp	r0, r1
	add	r3, r3, #808
	bne	.L1091
	add	ip, ip, r0
.L1090:
	str	ip, [r5, #1052]
	ldrb	r1, [r5, #1000]	@ zero_extendqisi2
	ldr	r0, [r5, #1048]
	b	.L1084
.L1177:
	mov	r3, r4, asl #2
	mov	r1, r8
	str	r3, [fp, #-244]
	mov	r2, r3
	ldr	r3, .L1190+12
	add	r0, r10, r2
	mov	r2, #4
	bl	qsort
	subs	r9, r4, #0
	bne	.L1099
.L1172:
	sub	ip, fp, #172
	b	.L1074
.L1176:
	add	r9, r4, #64
	ldr	r3, .L1190+28
	mov	r2, #4
	mov	r1, r8
	add	r9, r6, r9, lsl #2
	mov	r0, r9
	bl	qsort
	subs	r10, r4, #0
	bne	.L1098
	b	.L1061
.L1106:
	mov	r4, ip
	b	.L1026
.L1107:
	mov	r8, r4
	b	.L1029
.L1108:
	mov	r4, r10
	b	.L1063
.L1182:
	sub	r10, fp, #236
	ldr	r3, .L1190+8
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
	bne	.L1104
	ldr	r3, .L1190+12
	mov	r2, #4
	mov	r1, r8
	mov	r0, r10
	mov	r9, r8
	bl	qsort
	b	.L1172
.L1052:
	add	r3, r0, #256
	mov	r2, #4
	str	r3, [fp, #-244]
	mov	r1, r10
	mov	r0, r3
	ldr	r3, .L1190+24
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2864]
	cmp	r8, #0
	movwne	r9, #47780
	movne	r4, r10
	movtne	r9, 169
	addne	r9, r6, r9
	bne	.L1102
	ldr	r9, [fp, #-244]
	mov	r2, #4
	ldr	r3, .L1190+28
	mov	r1, r8
	mov	r10, r8
	mov	r0, r9
	bl	qsort
	b	.L1061
.L1189:
	ldr	r1, .L1190+36
.L1171:
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r6
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	b	.L1163
.L1089:
	movw	ip, #42012
	ldr	r2, [r5, #2652]
	mov	r1, r4
	movt	ip, 169
	add	r3, r6, #388
	add	ip, r6, ip
	str	ip, [sp]
	bl	MVC_GenPiclistfromFrmlist_Interview
	b	.L1169
.L1043:
	ldr	r1, .L1190+40
	b	.L1171
.L1191:
	.align	2
.L1190:
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
	ldr	r3, .L1205
	ldr	r3, [r3]
	tst	r3, #8192
	beq	.L1192
	movw	r8, #42004
	add	r7, r0, #252
	movt	r8, 169
	add	r8, r0, r8
	mov	r6, #0
.L1194:
	ldr	r3, [r8, #4]!
	cmp	r3, #0
	movne	r5, r7
	movne	r4, #0
	beq	.L1196
.L1195:
	ldr	ip, [r5, #4]!
	mov	r3, r4
	mov	r2, r6
	ldr	r1, .L1205+4
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
	bhi	.L1195
.L1196:
	add	r6, r6, #1
	add	r7, r7, #132
	cmp	r6, #2
	bne	.L1194
.L1192:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1206:
	.align	2
.L1205:
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
	beq	.L1212
	ldr	r4, [r3, #2856]
	add	r3, r0, #11141120
	add	r3, r3, #12288
	cmp	r4, #0
	ldr	r5, [r3, #3436]
	beq	.L1213
	movw	ip, #47716
	mov	r6, #0
	movt	ip, 169
	add	ip, r0, ip
	mov	r1, r6
	mvn	lr, #-2147483648
.L1211:
	ldr	r2, [ip, #4]!
	add	r1, r1, #1
	cmp	r2, #0
	beq	.L1210
	ldr	r3, [r2, #32]
	rsb	r3, r3, r5
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, lr
	ldrlt	r6, [r2, #228]
	movlt	lr, r3
.L1210:
	cmp	r1, r4
	bne	.L1211
.L1209:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1213:
	mov	r6, r4
	b	.L1209
.L1212:
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
	bne	.L1248
	ldr	r4, [r3, #1048]
	movw	r5, #48152
	movt	r6, 169
	movt	r5, 169
	cmp	r4, #0
	add	r6, r0, r6
	add	r5, r0, r5
	mov	r7, lr
	beq	.L1226
.L1250:
	add	r2, r0, #252
	mov	ip, #32
	mov	r3, #0
	b	.L1224
.L1223:
	add	r3, r3, #1
	cmp	r3, r4
	beq	.L1249
.L1224:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #52]
	cmp	r1, lr
	bne	.L1223
	cmp	ip, r3
	movcs	ip, r3
	add	r3, r3, #1
	cmp	r3, r4
	str	ip, [r5, #-4]
	str	ip, [r6, #4]
	bne	.L1224
.L1249:
	cmp	ip, #31
	bhi	.L1226
.L1225:
	add	lr, lr, #1
	add	r6, r6, #8
	cmp	lr, #16
	add	r5, r5, #8
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r4, #0
	bne	.L1250
.L1226:
	str	r7, [r5, #-4]
	str	r7, [r6, #4]
	b	.L1225
.L1248:
	ldr	r5, [r3, #1048]
	mov	lr, #0
	movt	r6, 169
	mov	r7, lr
	cmp	r5, #0
	add	r6, r0, r6
	beq	.L1230
.L1252:
	add	r1, r0, #252
	mov	r4, #32
	mov	r2, #0
	b	.L1228
.L1227:
	add	r2, r2, #1
	cmp	r2, r5
	beq	.L1251
.L1228:
	ldr	r3, [r1, #4]!
	ldr	r8, [r3, #4]
	ldrb	ip, [r3]	@ zero_extendqisi2
	ldr	r3, [r8, #52]
	mov	r3, r3, asl #1
	cmp	ip, #2
	orreq	r3, r3, #1
	cmp	r3, lr
	bne	.L1227
	cmp	r4, r2
	movcs	r4, r2
	add	r2, r2, #1
	cmp	r2, r5
	str	r4, [r6]
	bne	.L1228
.L1251:
	cmp	r4, #31
	bhi	.L1230
.L1229:
	add	lr, lr, #1
	add	r6, r6, #4
	cmp	lr, #32
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r5, #0
	bne	.L1252
.L1230:
	str	r7, [r6]
	b	.L1229
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
	beq	.L1260
	mov	r0, r4
	bl	MVC_InitListX
	cmp	r0, #0
	bne	.L1261
	mov	r0, r4
	bl	MVC_ReorderListX
	mov	r0, r4
	bl	MVC_RepairList
	cmp	r0, #0
	bne	.L1258
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
.L1260:
	mov	r0, #0
	str	r0, [r3, #1048]
	str	r0, [r3, #1052]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1258:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1261:
	ldr	r1, .L1262
	mov	r0, #13
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1263:
	.align	2
.L1262:
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
	beq	.L1271
	movw	r3, #48287
	mov	r2, #0
	movt	r3, 169
	mov	lr, r2
	add	r3, r0, r3
	b	.L1270
.L1279:
	ldrb	r1, [r3, #-2]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1268
	cmp	r2, ip
	add	r3, r3, #808
	beq	.L1271
.L1270:
	ldrb	r1, [r3]	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r1, #1
	bne	.L1279
.L1268:
	cmp	r2, ip
	strb	lr, [r3, #-5]
	strb	lr, [r3, #-4]
	add	r3, r3, #808
	bne	.L1270
.L1271:
	ldr	r2, [r0, #52]
	cmp	r2, #0
	beq	.L1280
	mov	r3, #0
	add	r0, r0, #144
	mov	r1, r3
.L1272:
	add	r3, r3, #1
	str	r1, [r0, #4]!
	cmp	r3, r2
	bne	.L1272
	ldmfd	sp, {fp, sp, pc}
.L1280:
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
	beq	.L1298
.L1282:
	ldr	r3, [r8, #48]
	cmp	r3, #0
	beq	.L1289
	movw	r5, #48936
	mov	r6, #0
	movt	r5, 169
	add	r5, r8, r5
	mov	r7, #0
	mov	r9, #0
.L1288:
	sub	r4, r5, #32
	mov	r3, #0
	strb	r3, [r5, #-654]
	strb	r3, [r5, #-653]
.L1287:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L1299
.L1286:
	cmp	r4, r5
	bne	.L1287
	ldr	r3, [r8, #48]
	add	r9, r9, #1
	add	r5, r5, #808
	cmp	r3, r9
	bhi	.L1288
.L1289:
	ldr	r2, [r8, #52]
	cmp	r2, #0
	beq	.L1300
	mov	r3, #0
	add	r8, r8, #144
	mov	r1, r3
.L1290:
	add	r3, r3, #1
	str	r1, [r8, #4]!
	cmp	r3, r2
	bne	.L1290
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1299:
	mov	r1, r2
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L1286
.L1298:
	add	r0, r8, #584
	bl	ResetVoQueue
	b	.L1282
.L1300:
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
.L1303:
	add	r3, r3, #1
	cmp	r0, r2, asl r3
	bge	.L1303
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
	beq	.L1322
	cmp	r0, #0
	bne	.L1308
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	ldr	r3, [r3, #28]
	cmp	r2, #1
	beq	.L1323
	ldr	r2, [r4, #28]
	cmp	r2, r3
	beq	.L1310
	ldr	r1, .L1325
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1308:
	ldr	r1, .L1325+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1322:
	ldr	r2, [r3, #28]
	movw	r7, #3992
	ldr	r3, [r4, #248]
	mla	r7, r7, r2, r3
.L1307:
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
	bne	.L1312
	add	r1, ip, #45056
	ldr	ip, [r7, #3972]
	ldr	r1, [r1, #2856]
	add	r1, r1, #1
	cmp	ip, r1
	movls	lr, r8
	bls	.L1313
.L1312:
	ldr	r1, .L1325+8
	ldr	ip, [r1]
	cmp	ip, #0
	moveq	lr, #1
	beq	.L1313
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
.L1313:
	ldrb	r3, [r6, #1011]	@ zero_extendqisi2
	cmp	r3, #1
	ldrne	ip, [r7, #3972]
	subne	ip, ip, #1
	beq	.L1324
.L1316:
	ldr	r3, [r4, #44]
	mov	r0, lr
	cmp	r3, ip
	orrne	lr, lr, #1
	str	lr, [r5]
	str	ip, [r4, #44]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1310:
	movw	r7, #35364
	movt	r7, 168
	add	r7, r4, r7
	b	.L1307
.L1324:
	movw	r1, #39336
	add	r3, r4, #290816
	movt	r1, 168
	add	r3, r3, #808
	add	r1, r4, r1
	mov	ip, #0
.L1315:
	ldr	r2, [r3]
	add	r3, r3, #335872
	add	r3, r3, #308
	cmp	r2, ip
	subhi	ip, r2, #1
	cmp	r3, r1
	bne	.L1315
	add	ip, ip, #1
	mov	ip, ip, asl #1
	cmp	ip, #16
	movcs	ip, #16
	b	.L1316
.L1323:
	movw	r7, #8500
	movt	r7, 5
	mla	r7, r7, r3, r4
	add	r7, r7, #286720
	add	r7, r7, #932
	b	.L1307
.L1326:
	.align	2
.L1325:
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
	beq	.L1329
	bcc	.L1330
	cmp	ip, #2
	ldmnefd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	cmp	r1, #0
	bne	.L1385
	add	r4, r6, #36864
	ldrb	r2, [r4, #3960]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1363
	ldr	r1, [r4, #4016]
	ldr	r0, [r4, #4012]
	ldr	r2, [r4, #4024]
	cmp	r1, r0
	addcc	r2, r2, r3
	strcc	r2, [r4, #4020]
	bcs	.L1365
.L1366:
	add	r3, r2, r1
	str	r3, [r4, #4008]
	ldrb	r0, [r7, #1008]	@ zero_extendqisi2
	mov	r3, r3, asl #1
	cmp	r0, #0
	subeq	r3, r3, #1
	str	r3, [r4, #3996]
	ldrb	r0, [r7, #1001]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1369
	str	r3, [r4, #3992]
	str	r3, [r4, #3988]
	str	r3, [r4, #3984]
.L1362:
	add	r6, r6, #36864
	str	r1, [r6, #4012]
	str	r2, [r6, #4024]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1330:
	cmp	r1, #0
	add	lr, lr, #4
	mov	r2, r2, asl lr
	bne	.L1386
	add	r4, r6, #36864
	ldrb	r3, [r4, #3960]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1334
	ldr	lr, [r4, #3972]
	ldr	ip, [r4, #3976]
	mov	r1, lr
.L1335:
	ldr	r0, [r4, #3964]
	cmp	r0, ip
	bcs	.L1333
	rsb	r3, r0, ip
	cmp	r3, r2, lsr #1
	addcs	r1, r1, r2
	strcs	r1, [r4, #3980]
	bcs	.L1337
.L1333:
	cmp	r0, ip
	bls	.L1338
	rsb	r3, ip, r0
	cmp	r3, r2, lsr #1
	rsbhi	r1, r2, r1
	strhi	r1, [r4, #3980]
	bls	.L1338
.L1337:
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1339
.L1388:
	ldr	r3, [r4, #3968]
	add	r1, r1, r0
	str	r1, [r4, #3984]
	add	r3, r1, r3
	str	r3, [r4, #3988]
	cmp	r3, r1
	movge	r3, r1
	str	r3, [r4, #3996]
.L1340:
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
.L1329:
	cmp	r1, #0
	add	r4, r6, #36864
	movne	r3, #0
	strne	r3, [r4, #4020]
	bne	.L1344
	ldrb	r2, [r4, #3960]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1345
	mov	r3, r1
	str	r1, [r4, #4024]
	str	r1, [r4, #4012]
.L1346:
	str	r3, [r4, #4020]
.L1344:
	ldr	r3, [r5, #2916]
	cmp	r3, #0
	beq	.L1348
	ldr	r3, [r4, #4016]
	ldr	r0, [r4, #4020]
	add	r0, r3, r0
	str	r0, [r4, #4008]
	ldrb	r8, [r7, #1008]	@ zero_extendqisi2
	cmp	r8, #0
	bne	.L1350
	cmp	r0, #0
	beq	.L1351
	sub	r0, r0, #1
	str	r0, [r4, #4008]
.L1350:
	mov	r3, #0
	str	r3, [r4, #4040]
	ldr	r1, [r5, #2916]
	cmp	r1, r3
	bgt	.L1371
	cmp	r0, #0
	beq	.L1354
.L1387:
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
	blt	.L1355
	add	r2, r5, #2912
	add	r1, r1, #1
	add	r2, r2, #4
	mov	r3, #0
.L1356:
	add	r3, r3, #1
	ldr	ip, [r2, #4]!
	cmp	r3, r1
	add	r0, r0, ip
	str	r0, [r4, #4036]
	bne	.L1356
.L1355:
	cmp	r8, #0
	ldreq	r3, [r5, #2908]
	addeq	r0, r0, r3
	streq	r0, [r4, #4036]
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1358
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
.L1359:
	add	r6, r6, #36864
	ldr	r2, [r6, #4016]
	ldr	r3, [r6, #4020]
	str	r0, [r6, #3992]
	str	r2, [r6, #4012]
	str	r3, [r6, #4024]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1348:
	str	r3, [r4, #4008]
	ldrb	r8, [r7, #1008]	@ zero_extendqisi2
.L1351:
	mov	r0, #0
	str	r0, [r4, #4040]
	ldr	r3, [r5, #2916]
	cmp	r3, r0
	ble	.L1354
.L1371:
	add	ip, r5, #2912
	mov	r3, #0
	add	ip, ip, #4
	mov	r2, r3
.L1353:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	add	r3, r3, r1
	str	r3, [r4, #4040]
	ldr	r1, [r5, #2916]
	cmp	r1, r2
	bgt	.L1353
	cmp	r0, #0
	bne	.L1387
.L1354:
	mov	r0, #0
	str	r0, [r4, #4036]
	b	.L1355
.L1386:
	add	r4, r0, #11075584
	add	r4, r4, #36864
.L1336:
	mov	r3, #0
	ldr	r0, [r4, #3964]
	str	r3, [r4, #3972]
	mov	lr, r3
	str	r3, [r4, #3976]
	mov	r1, ip
	b	.L1333
.L1385:
	add	r3, r6, #36864
	mov	r1, #0
	mov	r2, r1
	str	r1, [r3, #4020]
	str	r1, [r3, #3988]
	str	r1, [r3, #3984]
	str	r1, [r3, #3992]
	str	r1, [r3, #3996]
	ldr	r1, [r3, #4016]
	b	.L1362
.L1338:
	str	lr, [r4, #3980]
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1388
.L1339:
	ldrb	r3, [r7, #1002]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r1, r0
	streq	r3, [r4, #3984]
	strne	r3, [r4, #3988]
	str	r3, [r4, #3996]
	b	.L1340
.L1358:
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
	b	.L1359
.L1334:
	movw	r3, #3961
	ldrsb	r3, [r4, r3]
	cmp	r3, #0
	bne	.L1336
	ldr	r0, [r4, #3984]
	mov	r1, ip
	mov	lr, r3
	str	r3, [r4, #3972]
	mov	ip, r0
	str	r0, [r4, #3976]
	b	.L1335
.L1345:
	ldr	r1, [r4, #4016]
	ldr	r2, [r4, #4012]
	cmp	r1, r2
	bcs	.L1347
	ldr	r2, [r4, #4024]
	add	r3, r3, r2
	str	r3, [r4, #4020]
	b	.L1344
.L1369:
	ldrb	r0, [r7, #1002]	@ zero_extendqisi2
	str	r3, [r4, #3992]
	cmp	r0, #0
	streq	r3, [r4, #3984]
	strne	r3, [r4, #3988]
	b	.L1362
.L1363:
	str	r1, [r4, #4012]
	mov	r2, r1
	str	r1, [r4, #4024]
	ldr	r1, [r4, #4016]
.L1365:
	str	r2, [r4, #4020]
	b	.L1366
.L1347:
	ldr	r3, [r4, #4024]
	b	.L1346
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
	beq	.L1390
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
	beq	.L1410
.L1409:
	ldr	ip, [lr, #4]!
	ldrb	r4, [ip, #3]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L1404
	ldr	r4, [ip, #20]
	ldr	r8, [r5, #3424]
	cmp	r4, r8
	rsbhi	r4, r6, r4
	str	r4, [ip, #24]
	ldr	ip, [lr]
	ldrb	r4, [ip, #3]	@ zero_extendqisi2
	tst	r4, #1
	beq	.L1407
	ldr	r8, [ip, #732]
	bic	r8, r8, #-16777216
	bic	r8, r8, #255
	cmp	r8, #65536
	ldreq	r4, [ip, #24]
	addeq	r4, r7, r4, lsl #1
	streq	r4, [ip, #744]
	ldreq	ip, [lr]
	ldreqb	r4, [ip, #3]	@ zero_extendqisi2
.L1407:
	tst	r4, #2
	beq	.L1404
	ldr	r4, [ip, #768]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	ldreq	r4, [ip, #24]
	addeq	r4, r2, r4, lsl #1
	streq	r4, [ip, #780]
.L1404:
	ldr	ip, [r3, #2864]
	add	r1, r1, #1
	cmp	ip, r1
	bhi	.L1409
.L1410:
	ldr	r1, [r3, #2868]
	cmp	r1, #0
	beq	.L1434
	movw	r4, #47844
	mov	ip, #0
	movt	r4, 169
	add	r4, r0, r4
.L1415:
	ldr	lr, [r4, #4]!
	add	ip, ip, #1
	ldrb	r1, [lr, #3]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1412
	tst	r1, #1
	beq	.L1413
	ldr	r0, [lr, #732]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #256
	ldreq	r1, [lr, #28]
	addeq	r1, r7, r1, lsl #1
	streq	r1, [lr, #740]
	ldreq	lr, [r4]
	ldreqb	r1, [lr, #3]	@ zero_extendqisi2
.L1413:
	tst	r1, #2
	beq	.L1412
	ldr	r1, [lr, #768]
	bic	r1, r1, #-16777216
	bic	r1, r1, #255
	cmp	r1, #256
	ldreq	r1, [lr, #28]
	addeq	r1, r2, r1, lsl #1
	streq	r1, [lr, #776]
.L1412:
	ldr	r1, [r3, #2868]
	cmp	r1, ip
	bhi	.L1415
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1390:
	add	r3, r3, #45056
	ldr	r1, [r3, #2864]
	cmp	r1, #0
	movwne	lr, #47780
	movtne	lr, 169
	addne	lr, r0, lr
	bne	.L1399
.L1400:
	ldr	r2, [r3, #2868]
	cmp	r2, #0
	beq	.L1435
	movw	ip, #47844
	mov	r2, #0
	movt	ip, 169
	add	ip, r0, ip
	b	.L1402
.L1401:
	ldr	r1, [r3, #2868]
	cmp	r1, r2
	bls	.L1436
.L1402:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1401
	ldr	r0, [r1, #696]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #256
	ldreq	r0, [r1, #28]
	streq	r0, [r1, #704]
	ldr	r1, [r3, #2868]
	cmp	r1, r2
	bhi	.L1402
.L1436:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1437:
	ldr	ip, [r1, #696]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L1396
	ldr	ip, [r1, #20]
	ldr	r4, [r5, #3424]
	rsb	r7, r6, ip
	cmp	ip, r4
	strhi	r7, [r1, #24]
	strls	ip, [r1, #24]
	ldr	r1, [lr]
	ldr	ip, [r1, #24]
	str	ip, [r1, #708]
.L1396:
	ldr	r1, [r3, #2864]
	add	r2, r2, #1
	cmp	r1, r2
	bls	.L1400
.L1399:
	ldr	r1, [lr, #4]!
	ldrb	ip, [r1, #3]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1396
	b	.L1437
.L1435:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1434:
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
	beq	.L1443
	add	r3, r0, #11075584
	add	r3, r3, #45056
	ldr	lr, [r3, #2856]
	cmp	lr, #0
	beq	.L1443
	ldr	r2, [r3, #2664]
	rsb	r3, r2, r1
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L1445
	movw	ip, #47720
	movt	ip, 169
	add	ip, r0, ip
	b	.L1440
.L1441:
	ldr	r2, [ip, #4]!
	rsb	r0, r2, r1
	cmp	r2, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L1445
.L1440:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L1441
.L1443:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L1445:
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
	ldr	r6, .L1453
	mov	lr, #0
	mov	r5, #255
	sub	r7, r6, #16
.L1450:
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
	bne	.L1450
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1454:
	.align	2
.L1453:
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
	beq	.L1486
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L1487
	ldr	r3, [fp, #-48]
	ldrb	r3, [r3, #18]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1475
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
	ldr	r10, .L1492
	b	.L1460
.L1490:
	cmp	r4, #0
	beq	.L1488
	cmp	r4, #3
	beq	.L1489
	ldr	r3, [r10, #52]
	mov	r2, #16
	sub	r1, r6, #16
	mov	r0, r6
	blx	r3
.L1468:
	add	r4, r4, #1
	cmp	r4, #7
	bhi	.L1475
.L1474:
	add	r8, r8, #64
	add	r9, r9, #64
	add	r6, r6, #16
	add	r7, r7, #4
	add	r5, r5, #1
.L1460:
	cmp	r4, #5
	ldrsb	r3, [r5]
	bhi	.L1461
	cmp	r3, #0
	beq	.L1490
	ldr	r3, [r7]
	cmp	r3, #0
	beq	.L1468
	ldr	r1, .L1492+4
	cmp	r4, #2
	mov	r2, #16
	ldr	r3, [r10, #52]
	add	r0, r1, r2
	add	r4, r4, #1
	movhi	r1, r0
	mov	r0, r6
	blx	r3
	cmp	r4, #7
	bls	.L1474
.L1475:
	mov	r0, #0
.L1457:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1461:
	cmp	r3, #0
	bne	.L1470
	ldr	r3, [fp, #-52]
	mov	r2, #64
	ldrb	r3, [r3, #27]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r10, #52]
	bne	.L1491
.L1485:
	ldr	r1, .L1492+8
	cmp	r4, #6
	add	r0, r1, r2
	movne	r1, r0
	mov	r0, r8
	blx	r3
	b	.L1468
.L1470:
	ldr	r3, [r7]
	cmp	r3, #0
	beq	.L1468
	ldr	r3, [r10, #52]
	mov	r2, #64
	b	.L1485
.L1491:
	mov	r1, r9
	mov	r0, r8
	blx	r3
	b	.L1468
.L1486:
	movw	r3, #6950
	ldr	r2, .L1492+12
	ldr	r1, .L1492+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1457
.L1487:
	mov	r0, r3
	ldr	r2, .L1492+12
	movw	r3, #6951
	ldr	r1, .L1492+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1457
.L1489:
	ldr	r3, [fp, #-52]
	mov	r2, #16
	ldr	r0, [fp, #-48]
	mov	r4, #4
	ldrb	r3, [r3, #27]	@ zero_extendqisi2
	add	r0, r0, #2064
	cmp	r3, #0
	ldr	r3, [r10, #52]
	ldrne	r1, [fp, #-52]
	ldreq	r1, .L1492+20
	addne	r1, r1, #2720
	blx	r3
	b	.L1474
.L1488:
	ldr	r3, [fp, #-52]
	mov	r2, #16
	ldr	r0, [fp, #-56]
	mov	r4, #1
	ldrb	r3, [r3, #27]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r10, #52]
	ldrne	r1, [fp, #-52]
	ldreq	r1, .L1492+4
	addne	r1, r1, #2672
	blx	r3
	b	.L1474
.L1493:
	.align	2
.L1492:
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
	ldr	r1, .L1497
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
	ldr	r1, .L1497+4
	moveq	r2, #1
	str	ip, [sp]
	mov	r0, #22
	bl	dprint_vfmw
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1498:
	.align	2
.L1497:
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
	ldr	r1, .L1535
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
	beq	.L1533
	ldr	r2, [r5, #640]
	ldr	r3, [r5, #644]
	cmp	r2, #0
	beq	.L1503
	cmp	r3, #0
	beq	.L1503
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
	beq	.L1509
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
.L1508:
	ldr	r5, [r8, #4]!
	add	r3, r3, #1
	cmp	r3, r10
	str	r5, [lr, #4]!
	ldr	r5, [ip, #4]!
	str	r5, [r0, #4]!
	ldr	r5, [r1, #4]!
	str	r5, [r2, #4]!
	bne	.L1508
.L1509:
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3, #1851]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1534
	ldrb	ip, [r9, #1738]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L1510
	ldr	r0, .L1535+4
	add	r4, r4, #17152
.L1511:
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
	bne	.L1511
	movw	r8, #17252
	mov	r5, #0
	movt	r8, 170
	add	r8, r6, r8
.L1512:
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
	bne	.L1512
.L1517:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	beq	.L1514
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
.L1520:
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
	bhi	.L1520
.L1514:
	mov	r0, #0
.L1530:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1534:
	ldrb	ip, [r9, #1738]	@ zero_extendqisi2
.L1510:
	cmp	ip, #1
	add	r4, r4, #17152
	mov	r0, #0
	beq	.L1515
.L1518:
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
	bne	.L1518
	movw	r9, #17252
	mov	r10, #1
	movt	r9, 170
	add	r9, r6, r9
	mov	r8, #0
.L1519:
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
	bne	.L1519
	b	.L1517
.L1515:
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
	bne	.L1515
	movw	r9, #17252
	mov	r10, #1
	movt	r9, 170
	add	r9, r6, r9
	mov	r8, #0
.L1516:
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
	bne	.L1516
	b	.L1517
.L1503:
	ldr	r1, .L1535+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1530
.L1533:
	ldr	r3, [r10, #3416]
	ldr	r1, .L1535+12
	ldrsb	r2, [r3, #6]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1530
.L1536:
	.align	2
.L1535:
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
	bcs	.L1541
	add	lr, ip, #3
	add	lr, r6, lr, lsl #2
.L1542:
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
	bne	.L1542
.L1541:
	cmp	r3, #1
	bhi	.L1547
	add	r3, r3, #3
	add	ip, r6, #20
	mov	r2, #0
	add	r3, r6, r3, lsl #2
.L1546:
	str	r2, [r3, #4]!
	cmp	r3, ip
	str	r2, [r3, #-8]
	str	r2, [r3, #8]
	bne	.L1546
.L1547:
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
	bne	.L1643
	ldr	r2, [r10, #3416]
	str	r3, [r2, #724]
	ldr	r3, [r10, #3416]
	ldr	r2, [r6, #32]
	str	r2, [r3, #728]
.L1548:
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
	beq	.L1550
	cmp	r3, #0
	ldreqb	r3, [r8, #1722]	@ zero_extendqisi2
	movne	r3, #0
	strb	r3, [r6, #4]
.L1550:
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
	beq	.L1589
	ldr	r7, [r5, #1048]
	cmp	r7, #0
	beq	.L1592
	ldr	r3, [r4, #256]
	ldr	r2, [r3, #4]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r1, r4, #256
	movne	r3, #0
	bne	.L1557
	b	.L1555
.L1559:
	ldr	r2, [r1, #4]!
	ldr	r2, [r2, #4]
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1555
.L1557:
	add	r3, r3, #1
	cmp	r3, r7
	bne	.L1559
	mov	r3, #0
	str	r3, [fp, #-56]
.L1554:
	ldr	ip, [r5, #1052]
	cmp	ip, #0
	beq	.L1593
	ldr	r3, [r4, #388]
	ldr	r2, [r3, #4]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r1, r4, #388
	movne	r3, #0
	bne	.L1563
	b	.L1561
.L1565:
	ldr	r2, [r1, #4]!
	ldr	r2, [r2, #4]
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1561
.L1563:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L1565
	mov	r3, #0
	str	r3, [fp, #-60]
.L1560:
	ldrb	ip, [r10, #2779]	@ zero_extendqisi2
	ldr	r3, [r5, #1048]
	cmp	ip, #0
	beq	.L1566
	cmp	r3, #0
	beq	.L1581
	mov	r7, #0
	mov	r9, r6
	str	r6, [fp, #-64]
	add	r10, r4, #256
	mov	r6, r5
	mov	r5, r7
	ldr	r7, [fp, #-56]
	b	.L1580
.L1578:
	strb	r2, [r9, #1630]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r9, #1631]
	ldr	r2, [r10]
	ldr	r2, [r2, #16]
	str	r2, [r9, #1656]
.L1579:
	ldr	r2, [r6, #1048]
	add	r5, r5, #1
	add	r10, r10, #4
	add	r9, r9, #36
	cmp	r2, r5
	bls	.L1644
.L1580:
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
	beq	.L1577
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1577:
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
	bne	.L1578
	strb	r2, [r9, #1628]
	ldr	r2, [r10]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r9, #1629]
	ldr	r2, [r10]
	ldr	r2, [r2, #16]
	str	r2, [r9, #1652]
	b	.L1579
.L1644:
	mov	r5, r6
	ldr	r6, [fp, #-64]
.L1581:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1645
.L1569:
	movw	r2, #48144
	add	r3, r6, #3920
	add	r0, r6, #4048
	movt	r2, 169
	add	r3, r3, #4
	add	r2, r4, r2
	add	r0, r0, #4
.L1586:
	ldr	r1, [r2, #4]!
	str	r1, [r3, #4]!
	cmp	r3, r0
	bne	.L1586
	ldr	r2, [fp, #-52]
	ldrb	r3, [r8, #1722]	@ zero_extendqisi2
	add	r9, r2, #16384
	cmp	r3, #0
	str	r3, [r9, #996]
	ldr	r3, [r8, #1764]
	str	r3, [r9, #1000]
	beq	.L1587
	ldrb	r2, [r5, #1000]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1588
.L1587:
	cmp	r3, #1
	beq	.L1646
.L1589:
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
.L1643:
	ldr	r2, [r10, #3416]
	ldr	lr, [r2, #724]
	ldr	ip, [r2, #728]
	cmp	lr, r3
	movlt	lr, r3
	cmp	ip, r3
	str	lr, [r2, #724]
	movge	ip, r3
	str	ip, [r2, #728]
	b	.L1548
.L1555:
	ldrsb	r1, [r2, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	movweq	r3, #8012
	beq	.L1639
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-56]
	b	.L1554
.L1646:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1589
.L1588:
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
.L1590:
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
	bcs	.L1590
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1589
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
.L1591:
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
	bcs	.L1591
	b	.L1589
.L1566:
	cmp	r3, #0
	beq	.L1574
	ldr	r9, [fp, #-56]
	add	r10, r4, #256
	mov	r7, r6
	str	r6, [fp, #-64]
	mov	r6, r5
	mov	r5, ip
.L1573:
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
	beq	.L1572
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1572:
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
	bhi	.L1573
	mov	r5, r6
	ldr	r6, [fp, #-64]
.L1574:
	ldrb	r3, [r5, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1569
	ldr	r3, [r5, #1052]
	cmp	r3, #0
	beq	.L1569
	mov	r9, #0
	mov	r10, r6
	str	r6, [fp, #-56]
	mov	r6, r5
	mov	r5, r9
	ldr	r9, [fp, #-60]
	add	r7, r4, #388
.L1576:
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
	beq	.L1575
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1575:
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
	bhi	.L1576
.L1638:
	mov	r5, r6
	ldr	r6, [fp, #-56]
	b	.L1569
.L1561:
	ldrsb	r1, [r2, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L1647
	ldr	r3, [r3, #640]
	ldr	r3, [r3, #8]
	str	r3, [fp, #-60]
	b	.L1560
.L1645:
	ldr	r3, [r5, #1052]
	cmp	r3, #0
	beq	.L1569
	mov	r3, #0
	mov	r10, r6
	str	r6, [fp, #-56]
	add	r7, r4, #388
	mov	r6, r5
	ldr	r9, [fp, #-60]
	mov	r5, r3
	b	.L1585
.L1583:
	strb	r2, [r10, #2782]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2783]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r10, #2808]
.L1584:
	ldr	r2, [r6, #1052]
	add	r5, r5, #1
	add	r7, r7, #4
	add	r10, r10, #36
	cmp	r2, r5
	bls	.L1638
.L1585:
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
	beq	.L1582
	cmp	r0, #0
	ldrne	r2, [r0, #640]
	ldrne	r0, [r2, #8]
.L1582:
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
	bne	.L1583
	strb	r2, [r10, #2780]
	ldr	r2, [r7]
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	strb	r2, [r10, #2781]
	ldr	r2, [r7]
	ldr	r2, [r2, #16]
	str	r2, [r10, #2804]
	b	.L1584
.L1647:
	movw	r3, #8033
.L1639:
	ldr	r2, .L1648
	ldr	r1, .L1648+4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L1592:
	str	r7, [fp, #-56]
	b	.L1554
.L1593:
	str	ip, [fp, #-60]
	b	.L1560
.L1649:
	.align	2
.L1648:
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
	beq	.L1654
	mov	r0, #0
.L1653:
	ldr	r3, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r1
	add	r3, r3, #7
	cmp	r2, #0
	add	r0, r0, r3, lsr #3
	bne	.L1653
	ldmfd	sp, {fp, sp, pc}
.L1654:
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
	ldr	r1, .L1667
	mov	r4, r0
	add	r5, r0, #11075584
	bl	mvc_ue_v
	add	r5, r5, #40960
	str	r0, [r5, #1056]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1665
	cmp	r0, #262144
	bcs	.L1666
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #16]
	mul	r3, r3, r2
	sub	r3, r3, #1
	cmp	r0, r3
	bhi	.L1665
	ldr	r1, .L1667+4
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L1667+8
	mov	r2, r0
	mov	r6, r0
	mov	r0, #19
	bl	dprint_vfmw
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1665
	cmp	r6, #9
	bhi	.L1661
	mov	r1, #1
	movw	r0, #297
	mov	r2, r1, asl r6
	and	r0, r0, r2
	cmp	r0, #0
	bne	.L1662
	ands	r3, r2, #660
	bne	.L1663
	tst	r2, #66
	beq	.L1661
	strb	r1, [r5, #1000]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1662:
	strb	r3, [r5, #1000]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1661:
	mov	r2, r6
	ldr	r1, .L1667+12
	mov	r0, #1
	bl	dprint_vfmw
.L1665:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1663:
	mov	r3, #2
	strb	r3, [r5, #1000]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1666:
	ldr	r1, .L1667+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1668:
	.align	2
.L1667:
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
	ldr	r1, .L1681
	mov	r0, #19
	ldr	r2, [r5, #1016]
	bl	dprint_vfmw
	ldr	r3, [r4, #252]
	ldr	r2, [r5, #1016]
	mov	r1, #2240
	mla	r1, r1, r2, r3
	ldrb	r3, [r1, #19]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1679
	ldrb	r3, [r5, #1012]	@ zero_extendqisi2
	sxtb	r5, r3
	cmn	r5, #1
	beq	.L1680
	cmp	r3, #0
	bne	.L1674
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
	beq	.L1675
	add	r0, r0, #286720
	ldrb	r0, [r0, #957]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1675
.L1673:
	mov	r0, #0
	str	r1, [r4, #244]
	str	r3, [r4, #240]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1680:
	ldr	r0, [r4, #248]
	movw	r3, #3992
	ldr	r2, [r1, #28]
	mla	r3, r3, r2, r0
	ldrb	r0, [r3, #25]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1673
	ldr	r1, .L1681+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1675:
	ldr	r1, .L1681+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1679:
	ldr	r1, .L1681+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1674:
	mov	r2, r5
	ldr	r1, .L1681+16
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1682:
	.align	2
.L1681:
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
	ldr	r1, .L1692
	mov	r0, #19
	bl	dprint_vfmw
	ldr	r2, [r4, #252]
	mov	r3, #2240
	mla	r3, r3, r5, r2
	ldrb	r2, [r3, #19]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1689
	ldr	r1, [r4, #36]
	ldr	r2, [r3, #28]
	sub	r3, r1, #1
	cmp	r2, r3
	bhi	.L1690
	movw	r3, #41972
	movt	r3, 169
	add	r3, r4, r3
	ldrsb	r5, [r3]
	cmn	r5, #1
	beq	.L1691
.L1688:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1691:
	ldr	r1, [r4, #248]
	movw	r3, #3992
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #25]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1688
	ldr	r1, .L1692+4
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1689:
	mov	r2, r5
	ldr	r1, .L1692+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1690:
	ldr	r1, .L1692+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1693:
	.align	2
.L1692:
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
	beq	.L1696
	ldrb	r0, [r3, #2657]	@ zero_extendqisi2
	ldrb	r1, [r3, #1002]	@ zero_extendqisi2
	cmp	r0, r1
	orrne	r2, r2, #1
.L1696:
	ldrb	r1, [r3, #2659]	@ zero_extendqisi2
	ldrb	r0, [r3, #1008]	@ zero_extendqisi2
	cmp	r1, r0
	moveq	r1, #0
	beq	.L1697
	cmp	r0, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
.L1697:
	ldrb	r0, [r3, #2658]	@ zero_extendqisi2
	ldrb	r4, [r3, #1003]	@ zero_extendqisi2
	cmp	r0, r4
	orrne	r2, r2, #1
	cmp	r4, #5
	cmpeq	r0, #5
	orr	r2, r2, r1
	beq	.L1709
.L1698:
	ldr	r1, [lr, #2900]
	cmp	r1, #0
	bne	.L1699
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
.L1700:
	ldrb	r1, [r3, #1011]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1701
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
.L1701:
	add	r3, ip, #40960
	ldr	r1, .L1710
	mov	r0, #19
	ldr	r4, [r3, #1056]
	cmp	r4, #0
	movne	r4, r2
	orreq	r4, r2, #1
	mov	r2, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1699:
	cmp	r1, #1
	bne	.L1700
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
	b	.L1700
.L1709:
	ldr	r0, [r3, #2676]
	ldr	r1, [r3, #1044]
	cmp	r0, r1
	orrne	r2, r2, #1
	b	.L1698
.L1711:
	.align	2
.L1710:
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
	bls	.L1767
.L1766:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
.L1733:
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1767:
	ldr	r1, .L1774
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #4044]
	cmp	r0, #0
	beq	.L1723
	ldr	r3, [r8, #1060]
	cmn	r3, #2
	beq	.L1749
	movw	r10, #40908
	mov	r9, #0
	movt	r10, 169
	add	r10, r4, r10
	b	.L1730
.L1724:
	cmp	r0, #2
	beq	.L1768
	sub	r0, r0, #4
	cmp	r0, #1
	bls	.L1769
.L1727:
	ldr	r3, [r8, #1060]
	add	r9, r9, #1
	add	r3, r3, #2
	cmp	r3, r9
	bls	.L1770
.L1730:
	ldr	r1, .L1774+4
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r7, r0
	str	r0, [r10, #4]!
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bcc	.L1720
	cmp	r3, #0
	bne	.L1720
	cmp	r0, #3
	beq	.L1723
	cmp	r0, #1
	bhi	.L1724
	ldr	r1, .L1774+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, r0
	str	r0, [r10, #264]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bls	.L1726
	cmp	r3, #0
	beq	.L1727
.L1726:
	mov	r2, r0
	ldr	r1, .L1774+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1723:
	ldrb	r3, [r8, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1766
	ldr	r1, .L1774+16
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #4045]
	cmp	r0, #0
	beq	.L1766
	ldr	r3, [r8, #1064]
	cmn	r3, #2
	beq	.L1766
	movw	r9, #41040
	mov	r5, #0
	movt	r9, 169
	add	r9, r4, r9
	b	.L1746
.L1745:
	add	r5, r5, #1
	cmp	r5, r3
	bcs	.L1766
.L1746:
	ldr	r1, .L1774+20
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r7, r0
	str	r0, [r9, #4]!
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bcc	.L1737
	cmp	r3, #0
	bne	.L1737
	cmp	r0, #3
	beq	.L1733
	cmp	r0, #1
	bls	.L1771
	cmp	r0, #2
	beq	.L1772
	sub	r0, r0, #4
	cmp	r0, #1
	bls	.L1773
	ldr	r3, [r8, #1064]
	add	r3, r3, #2
	cmp	r3, r5
	bne	.L1745
.L1747:
	ldr	r1, .L1774+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1771:
	ldr	r1, .L1774+28
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, r0
	str	r0, [r9, #264]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bls	.L1741
	cmp	r3, #0
	bne	.L1741
.L1742:
	ldr	r2, [r8, #1064]
	add	r3, r2, #2
	cmp	r3, r5
	bne	.L1745
	movw	r1, #26646
	movt	r1, 42
	add	r1, r2, r1
	add	r1, r4, r1, lsl #2
	ldr	r2, [r1, #4]
	cmp	r2, #3
	beq	.L1745
	b	.L1747
.L1768:
	ldr	r1, .L1774+32
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r10, #528]
	b	.L1727
.L1772:
	ldr	r1, .L1774+36
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #524]
	b	.L1742
.L1769:
	ldr	r1, .L1774+40
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r10, #784]
	b	.L1727
.L1773:
	ldr	r1, .L1774+44
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #784]
	b	.L1742
.L1720:
	mov	r2, r0
	ldr	r1, .L1774+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1737:
	mov	r2, r0
	ldr	r1, .L1774+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1770:
	cmp	r9, r3
	bne	.L1723
.L1718:
	movw	r2, #26610
	movt	r2, 42
	add	r2, r3, r2
	add	r2, r4, r2, lsl #2
	ldr	r3, [r2, #8]
	cmp	r3, #3
	beq	.L1723
	ldr	r1, .L1774+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1741:
	mov	r2, r0
	ldr	r1, .L1774+60
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1749:
	mov	r3, #0
	b	.L1718
.L1775:
	.align	2
.L1774:
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
	ldr	r1, .L1806
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
	bne	.L1800
.L1777:
	cmp	r0, #7
	bhi	.L1780
	ldr	r3, [r6, #1072]
	cmp	r3, #7
	bhi	.L1780
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
	cmp	r2, #0
	movweq	r10, #42164
	moveq	r4, r2
	movteq	r10, 169
	addeq	r10, r5, r10
	beq	.L1788
	b	.L1779
.L1784:
	ldr	r2, [r7, #748]
	cmp	r2, #0
	bne	.L1801
.L1785:
	str	r2, [r10]
	str	r2, [r10, #768]
	str	r2, [r10, #128]
	str	r2, [r10, #896]
.L1787:
	ldr	r2, [r6, #1060]
	add	r4, r4, #1
	add	r10, r10, #4
	cmp	r2, r4
	bcc	.L1802
.L1788:
	ldr	r1, .L1806+4
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	streq	r8, [r10, #-128]
	streq	r0, [r10, #640]
	beq	.L1784
	ldr	r1, .L1806+8
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+12
	str	r0, [r10, #-128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r10, #640]
	ldr	r2, [r7, #748]
	cmp	r2, #0
	beq	.L1785
.L1801:
	ldr	r1, .L1806+16
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L1803
	str	r9, [r10]
	str	r0, [r10, #768]
	str	r9, [r10, #128]
	str	r0, [r10, #896]
	b	.L1787
.L1802:
	ldr	r3, [fp, #-48]
	ldr	r3, [r3, #44]
	cmp	r3, #1
	beq	.L1789
.L1790:
	ldrb	r0, [r5, #10]	@ zero_extendqisi2
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
.L1782:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1803:
	ldr	r1, .L1806+20
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+24
	str	r0, [r10]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+20
	str	r0, [r10, #768]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+24
	str	r0, [r10, #128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r10, #896]
	b	.L1787
.L1800:
	ldr	r1, .L1806+28
	mov	r0, r5
	bl	mvc_ue_v
	mov	r3, r0
	ldr	r0, [r6, #1068]
	mov	r9, r4, asl r3
	str	r3, [r6, #1072]
	b	.L1777
.L1789:
	ldrb	r3, [r6, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1790
	movw	r4, #42548
	mov	r10, #0
	movt	r4, 169
	add	r4, r5, r4
	b	.L1796
.L1792:
	ldr	r3, [r7, #748]
	cmp	r3, #0
	bne	.L1804
.L1793:
	str	r3, [r4]
	str	r3, [r4, #768]
	str	r3, [r4, #128]
	str	r3, [r4, #896]
.L1795:
	ldr	r3, [r6, #1064]
	add	r10, r10, #1
	add	r4, r4, #4
	cmp	r3, r10
	bcc	.L1790
.L1796:
	ldr	r1, .L1806+32
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	streq	r8, [r4, #-128]
	streq	r0, [r4, #640]
	beq	.L1792
	ldr	r1, .L1806+36
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+40
	str	r0, [r4, #-128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r4, #640]
	ldr	r3, [r7, #748]
	cmp	r3, #0
	beq	.L1793
.L1804:
	ldr	r1, .L1806+44
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L1805
	str	r9, [r4]
	str	r0, [r4, #768]
	str	r9, [r4, #128]
	str	r0, [r4, #896]
	b	.L1795
.L1780:
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
.L1779:
	ldr	r1, .L1806+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1782
.L1805:
	ldr	r1, .L1806+52
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+56
	str	r0, [r4]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+52
	str	r0, [r4, #768]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1806+56
	str	r0, [r4, #128]
	mov	r0, r5
	bl	mvc_se_v
	str	r0, [r4, #896]
	b	.L1795
.L1807:
	.align	2
.L1806:
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
	beq	.L1820
	mov	r9, #0
	b	.L1811
.L1842:
	ldr	r1, .L1848
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r5, #2
	beq	.L1840
.L1822:
	cmp	r5, #6
	orreq	r6, r6, #1
	cmp	r6, #0
	bne	.L1825
	cmp	r5, #4
	beq	.L1841
.L1826:
	cmp	r5, #6
	bhi	.L1818
.L1823:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1818
	cmp	r5, #0
	beq	.L1828
.L1827:
	subs	r7, r7, #1
	beq	.L1829
.L1820:
	ldr	r1, .L1848+4
	mov	r0, r4
	bl	mvc_ue_v
	bic	r3, r0, #2
	sub	r6, r0, #3
	clz	r6, r6
	cmp	r3, #1
	mov	r5, r0
	mov	r6, r6, lsr #5
	beq	.L1842
	cmp	r5, #2
	bne	.L1822
.L1840:
	ldr	r1, .L1848+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, #0
	beq	.L1823
.L1825:
	ldr	r1, .L1848+12
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r5, #4
	bne	.L1826
.L1841:
	ldr	r1, .L1848+16
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1827
.L1818:
	ldr	r1, .L1848+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1847:
	ldr	r1, .L1848
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r3, [r8, #4]
	cmp	r3, #2
	str	r0, [r8, #8]
	beq	.L1843
.L1814:
	cmp	r3, #3
	cmpne	r3, #6
	beq	.L1844
.L1815:
	cmp	r3, #4
	beq	.L1845
.L1816:
	cmp	r3, #6
	bhi	.L1818
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1818
	cmp	r3, #0
	add	r9, r9, #1
	beq	.L1828
	cmp	r9, #100
	beq	.L1846
.L1811:
	ldr	r1, .L1848+4
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
	beq	.L1847
	cmp	r3, #2
	bne	.L1814
.L1843:
	ldr	r1, .L1848+8
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
	bne	.L1815
.L1844:
	ldr	r1, .L1848+12
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
	bne	.L1816
.L1845:
	ldr	r1, .L1848+16
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
	b	.L1816
.L1828:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1846:
	ldr	r1, .L1848+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1829:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1849:
	.align	2
.L1848:
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
	beq	.L1851
	clz	r2, r7
	mov	r2, r2, lsr #5
	strb	r2, [r0, #3]
	mov	r6, r2
.L1860:
	ldrb	r3, [r3, #1003]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1871
	ldr	r1, .L1875
	mov	r0, r4
	bl	mvc_u_1
	movw	r2, #2004
	mla	r6, r2, r6, r4
	movw	r5, #43664
	movt	r5, 169
	ldr	r1, .L1875+4
	add	r5, r6, r5
	mov	r3, r0
	uxtb	r2, r0
	strb	r3, [r5, #3]
	mov	r0, #16
	bl	dprint_vfmw
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1854
.L1858:
	mov	r8, #0
.L1868:
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1851:
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
	beq	.L1860
	ldrb	r3, [r3, #1003]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1872
	ldr	r1, .L1875
	ldrb	r6, [r5, #3]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r6, r0
	bne	.L1873
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1858
	mov	r1, r8
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	MVC_DecMMCO
.L1854:
	mov	r1, #1
	mov	r0, r4
	bl	MVC_DecMMCO
	cmn	r0, #1
	mov	r8, r0
	movne	r3, #1
	movne	r8, #0
	strneb	r3, [r5]
	bne	.L1868
	mov	r3, #0
	ldr	r1, .L1875+8
	strb	r3, [r5]
	mov	r0, #1
	strb	r7, [r4, #3]
	bl	dprint_vfmw
	b	.L1868
.L1872:
	ldr	r1, .L1875+12
	ldrb	r6, [r5, #1]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r6, r0
	bne	.L1874
	mov	r0, r4
	ldr	r1, .L1875+16
	ldrb	r4, [r5, #2]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r4, r0
	beq	.L1858
	ldr	r1, .L1875+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1868
.L1871:
	ldr	r1, .L1875+12
	mov	r0, r4
	bl	mvc_u_1
	movw	r2, #2004
	mla	r2, r2, r6, r4
	movw	r5, #43664
	movt	r5, 169
	ldr	r1, .L1875+16
	mov	r8, #0
	add	r5, r2, r5
	strb	r0, [r5, #1]
	mov	r0, r4
	bl	mvc_u_1
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	ldr	r1, .L1875+24
	strb	r0, [r5, #2]
	mov	r0, #16
	bl	dprint_vfmw
	mov	r0, r8
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1873:
	ldr	r1, .L1875+28
	mov	r0, #1
	bl	dprint_vfmw
	ldr	ip, [r4, #68]
	ldr	r3, [r4, #100]
	mov	r0, #1
	ldr	r2, [r4, #88]
	mvn	r8, #0
	ldr	r1, .L1875+32
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L1868
.L1874:
	ldr	r1, .L1875+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1868
.L1876:
	.align	2
.L1875:
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
	ldr	r1, .L1942
	mov	r4, r0
	bl	mvc_ue_v
	ldr	r3, [r4, #40]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r6, r0
	bhi	.L1878
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1878
	mov	r1, r0
	mov	r0, r4
	bl	MVC_PPSSPSCheckTmpId
	cmp	r0, #0
	bne	.L1933
	add	r7, r4, #11075584
	mov	r0, r4
	add	r5, r7, #40960
	str	r6, [r5, #1016]
	bl	MVC_PPSSPSCheck
	subs	r8, r0, #0
	bne	.L1934
	ldr	r6, [r4, #240]
	mov	r0, r4
	ldr	r2, .L1942+4
	ldr	r9, [r4, #244]
	ldr	r1, [r6, #2896]
	add	r1, r1, #4
	bl	mvc_u_v
	str	r0, [r5, #1028]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1931
	adds	r0, r0, #0
	ldrb	r3, [r5, #1003]	@ zero_extendqisi2
	movne	r0, #1
	cmp	r3, #5
	movne	r0, #0
	cmp	r0, #0
	bne	.L1935
.L1884:
	mov	r3, #0
	strb	r3, [r5, #1001]
	strb	r3, [r5, #1002]
	ldrb	r3, [r6, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1936
	ldr	r2, [r6, #3948]
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	mla	r3, r1, r3, r3
.L1907:
	ldrb	r2, [r6, #21]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1890
	strb	r2, [r5, #1004]
.L1892:
	ldr	r2, [r5, #1056]
	cmp	r2, r3
	bcs	.L1937
.L1893:
	ldrb	r3, [r5, #1003]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1938
.L1895:
	add	r10, r7, #36864
	mov	r3, #0
	str	r3, [r10, #3964]
	str	r3, [r10, #3968]
	ldr	r3, [r6, #2900]
	cmp	r3, #0
	beq	.L1939
.L1899:
	mov	r3, #0
	str	r3, [r10, #4000]
	str	r3, [r10, #4004]
	ldr	r3, [r6, #2900]
	cmp	r3, #1
	beq	.L1940
.L1902:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1931
	ldrb	r3, [r9, #17]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1905
	ldr	r1, .L1942+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L1906
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1905
.L1906:
	ldr	r1, .L1942+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L1942+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1931
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L1931:
	mvn	r8, #0
.L1917:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1936:
	ldr	r1, .L1942+20
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #1001]
	cmp	r0, #0
	bne	.L1941
	ldr	r2, [r6, #3948]
	ldrb	ip, [r6, #20]	@ zero_extendqisi2
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	cmp	ip, #0
	mla	r3, r1, r3, r3
	bne	.L1909
	mov	r3, r3, asl #1
	b	.L1907
.L1890:
	mov	r2, #1
	strb	r2, [r5, #1004]
.L1910:
	ldr	r2, [r5, #1056]
	cmp	r2, r3, lsr #1
	bcc	.L1893
	ldr	r1, .L1942+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1917
.L1941:
	ldr	r1, .L1942+28
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
	beq	.L1888
.L1909:
	mov	r2, #1
.L1888:
	cmp	r0, #0
	mul	r3, r2, r3
	beq	.L1907
	cmp	r0, #1
	mov	r2, #0
	strb	r2, [r5, #1004]
	bne	.L1892
	b	.L1910
.L1905:
	mov	r0, r4
	add	r7, r7, #40960
	bl	MVC_IsNewPic
	uxtb	r0, r0
	strb	r0, [r7, #1005]
	cmp	r0, #0
	bne	.L1917
	ldr	r2, [r7, #1056]
	ldr	r3, [r7, #2696]
	cmp	r2, r3
	bhi	.L1917
	ldr	r1, .L1942+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1917
.L1939:
	ldr	r1, [r6, #2904]
	mov	r0, r4
	ldr	r2, .L1942+36
	add	r1, r1, #4
	bl	mvc_u_v
	str	r0, [r10, #3964]
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1899
	ldrb	r3, [r5, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1899
	ldr	r1, .L1942+40
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #3968]
	b	.L1899
.L1940:
	ldrb	r3, [r6, #18]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1902
	ldr	r1, .L1942+44
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #4000]
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1902
	ldrb	r3, [r5, #1001]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1902
	ldr	r1, .L1942+48
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #4004]
	b	.L1902
.L1878:
	mov	r2, r6
	ldr	r1, .L1942+52
	mvn	r8, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1938:
	ldr	r1, .L1942+56
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #65536
	str	r0, [r5, #1044]
	bcs	.L1896
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1895
.L1896:
	ldr	r1, .L1942+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1895
.L1933:
	ldr	r1, .L1942+64
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1917
.L1934:
	ldr	r1, .L1942+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1917
.L1935:
	ldr	r3, [r5, #2640]
	cmp	r3, #0
	bgt	.L1884
	ldr	r1, .L1942+72
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1884
.L1937:
	ldr	r1, .L1942+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1917
.L1943:
	.align	2
.L1942:
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
	bcc	.L1945
	ldrb	r7, [r0, #10]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L1945
	bl	MVC_PPSSPSCheck
	cmp	r0, #0
	bne	.L2027
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L2028
.L1949:
	ldr	r2, [r6, #36]
	cmp	r3, #1
	str	r2, [r4, #1060]
	ldr	r2, [r6, #40]
	str	r2, [r4, #1064]
	bls	.L2029
	cmp	r3, #2
	beq	.L1976
.L1960:
	mov	r3, #32
	str	r3, [r4, #1064]
.L1961:
	mov	r0, r5
	bl	mvc_ref_pic_list_reordering
	subs	r8, r0, #0
	bne	.L2030
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1963
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r7, #1
	moveq	r2, r7
	beq	.L1964
.L1963:
	ldr	r3, [r6, #44]
	cmp	r3, #1
	beq	.L2031
.L1980:
	mov	r2, #0
.L1964:
	strb	r7, [r4, #1007]
	mov	r0, #19
	ldr	r1, .L2037
	bl	dprint_vfmw
	ldrb	r2, [r4, #1007]	@ zero_extendqisi2
	mov	r3, #0
	str	r3, [r4, #1068]
	cmp	r2, r3
	str	r3, [r4, #1072]
	bne	.L2032
.L1965:
	ldrb	r3, [r4, #1008]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2033
.L1966:
	mov	r3, #0
	str	r3, [r4, #2612]
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1967
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L2034
.L1967:
	ldr	r1, .L2037+4
	mov	r0, r5
	bl	mvc_se_v
	mov	r3, #0
	str	r3, [r4, #2620]
	str	r3, [r4, #2624]
	str	r3, [r4, #2628]
	str	r0, [r4, #2616]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2035
.L1996:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1976:
	mov	r3, #32
	str	r3, [r4, #1060]
	b	.L1960
.L2035:
	ldr	r1, .L2037+8
	mov	r0, r5
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r4, #2620]
	bhi	.L1970
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1970
	cmp	r0, #1
	beq	.L1996
	ldr	r1, .L2037+12
	mov	r0, r5
	bl	mvc_se_v
	add	r3, r0, #6
	str	r0, [r4, #2624]
	cmp	r3, #12
	bhi	.L1973
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1973
	ldr	r1, .L2037+16
	mov	r0, r5
	bl	mvc_se_v
	add	r3, r0, #6
	str	r0, [r4, #2628]
	cmp	r3, #12
	bhi	.L1975
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1996
.L1975:
	ldr	r1, .L2037+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L2032:
	mov	r0, r5
	bl	mvc_pred_weight_table
	cmp	r0, #0
	beq	.L1965
	ldr	r1, .L2037+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L2033:
	mov	r0, r5
	bl	mvc_dec_ref_pic_marking
	cmp	r0, #0
	beq	.L1966
	ldr	r1, .L2037+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L2031:
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r7, r3
	moveq	r2, r7
	bne	.L1980
	b	.L1964
.L2028:
	ldr	r1, .L2037+32
	mov	r0, r5
	bl	mvc_u_1
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	strb	r0, [r4, #1006]
	b	.L1949
.L2029:
	ldr	r1, .L2037+36
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L1951
	ldr	r3, [r4, #1060]
.L1952:
	ldrb	r2, [r4, #1001]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1953
	cmp	r3, #15
	bhi	.L1954
	ldr	r3, [r4, #1064]
	cmp	r3, #15
	bhi	.L2036
.L1955:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1954
.L1956:
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1976
	cmp	r3, #1
	bne	.L1960
	b	.L1961
.L1945:
	ldr	r1, .L2037+40
	mvn	r8, #0
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2034:
	ldr	r1, .L2037+44
	mov	r0, r5
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r4, #2612]
	bhi	.L1968
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1967
.L1968:
	ldr	r1, .L2037+48
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L1953:
	cmp	r3, #31
	bhi	.L1957
	ldr	r3, [r4, #1064]
	cmp	r3, #31
	bls	.L1958
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1958
.L1957:
	ldr	r1, .L2037+52
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L1958:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1956
	b	.L1957
.L2036:
	ldrb	r3, [r4, #1000]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1955
.L1954:
	ldr	r1, .L2037+56
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L1951:
	ldr	r1, .L2037+60
	mov	r0, r5
	bl	mvc_ue_v
	ldrb	r2, [r4, #1000]	@ zero_extendqisi2
	cmp	r2, #1
	mov	r3, r0
	str	r0, [r4, #1060]
	bne	.L1952
	ldr	r1, .L2037+64
	mov	r0, r5
	bl	mvc_ue_v
	ldr	r3, [r4, #1060]
	str	r0, [r4, #1064]
	b	.L1952
.L1973:
	ldr	r1, .L2037+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L1970:
	ldr	r1, .L2037+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L2027:
	ldr	r1, .L2037+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L2030:
	ldr	r1, .L2037+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1996
.L2038:
	.align	2
.L2037:
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
	bne	.L2042
	add	r1, r2, #36864
	ldr	lr, [r1, #3964]
	str	lr, [r3, #2680]
	ldr	r1, [r1, #3968]
	str	r1, [r3, #2684]
	ldr	r1, [ip, #2900]
.L2042:
	cmp	r1, #1
	addeq	r2, r2, #36864
	ldreq	r1, [r2, #4000]
	streq	r1, [r3, #2688]
	ldreq	r2, [r2, #4004]
	streq	r2, [r3, #2692]
	ldrb	r2, [r3, #1011]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L2044
	ldr	ip, [r3, #2640]
	ldrb	r1, [r3, #1009]	@ zero_extendqisi2
	ldrb	r2, [r3, #1010]	@ zero_extendqisi2
	str	ip, [r3, #2700]
	strb	r1, [r3, #2660]
	strb	r2, [r3, #2661]
.L2044:
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
	beq	.L2050
	bcc	.L2051
	cmp	r2, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r3, #1003]	@ zero_extendqisi2
	add	r0, r0, #11141120
	cmp	r3, #5
	beq	.L2060
.L2053:
	add	r0, r0, #12288
	mov	r3, #0
	strb	r3, [r0, #2789]
	ldmfd	sp, {fp, sp, pc}
.L2051:
	add	r0, r0, #11141120
	add	r3, r0, #12288
	ldrb	r2, [r3, #2788]	@ zero_extendqisi2
	cmp	r2, #2
	movne	r2, #1
	strneb	r2, [r3, #2788]
	b	.L2053
.L2050:
	add	r0, r0, #11141120
	mov	r2, #2
	add	r0, r0, #12288
	mov	r3, #0
	strb	r2, [r0, #2788]
	strb	r3, [r0, #2789]
	ldmfd	sp, {fp, sp, pc}
.L2060:
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
	beq	.L2062
	ldr	r2, [r0, #232]
	ldr	r3, [r2, #64]
	add	r3, r3, #7
	bic	r3, r3, #7
	str	r3, [r2, #64]
	ldr	r2, [r0, #232]
	ldr	r3, [r2, #72]
	sub	r3, r3, #1
	str	r3, [r2, #72]
.L2062:
	ldr	r3, [r4, #232]
	ldrb	r6, [r3]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L2073
	mov	r1, #0
	mov	r0, r3
	mov	r5, r1
.L2064:
	add	r1, r1, #1
	ldr	ip, [r0, #12]
	cmp	r1, r6
	add	r0, r0, #28
	add	r5, r5, ip
	bne	.L2064
.L2063:
	mov	r7, r6, asl #5
	ldr	r0, [r3, #72]
	sub	r7, r7, r6, asl #2
	ldr	r2, [r3, #64]
	add	r1, r3, r7
	mov	r3, r5
	str	r0, [sp, #4]
	mov	r0, #7
	ldr	ip, [r1, #12]
	ldr	r1, .L2084
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #232]
	mov	r2, r6
	mov	r0, #7
	ldr	r1, .L2084+4
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
	bhi	.L2083
	ldrb	r6, [ip]	@ zero_extendqisi2
	add	r1, r6, #1
	cmp	r2, r1
	movhi	r2, #1
	movls	r2, #0
	cmp	r1, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L2066
	add	r6, r6, #2
	mov	r5, #0
.L2067:
	sub	r3, r6, #1
	ldr	r1, .L2084+8
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
	bne	.L2067
	sub	r3, r3, #1
.L2066:
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
	bcs	.L2061
	sxth	r3, r1
	add	r4, r4, #11141120
	add	r4, r4, #12288
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, ip, r3
.L2071:
	ldr	r2, [r3, #28]
	add	r1, r1, #1
	ldr	r0, [r4, #3488]
	add	r3, r3, #28
	cmp	r2, r0
	strcc	r2, [r4, #3488]
	ldr	r2, [ip, #68]
	cmp	r2, r1
	bhi	.L2071
.L2061:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2083:
	ldr	r1, .L2084+12
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	dprint_vfmw
.L2073:
	mov	r5, r6
	b	.L2063
.L2085:
	.align	2
.L2084:
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
	ble	.L2086
	mov	r0, #8
	mov	r4, #0
	mov	r2, r7
	mov	r9, r6
	ldr	r8, .L2097
	mov	r10, #255
	mov	r7, r1
	mov	r6, r0
	mov	r5, r4
	b	.L2091
.L2090:
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
	beq	.L2086
.L2091:
	cmp	r2, #16
	ldreq	r3, .L2097+4
	ldrneb	r4, [r5, r8]	@ zero_extendqisi2
	ldreqb	r4, [r5, r3]	@ zero_extendqisi2
	cmp	r0, #0
	add	r5, r5, #1
	beq	.L2090
	ldr	r1, .L2097+8
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
	b	.L2090
.L2086:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2098:
	.align	2
.L2097:
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
	ldr	r1, .L2157
	mov	r4, r0
	bl	mvc_u_1
	strb	r0, [r5]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2157+4
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #1]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2148
	ldr	r1, .L2157+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	str	r0, [r5, #32]
	bne	.L2103
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2104
.L2103:
	ldr	r1, .L2157+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L2157+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2148
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L2101:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2111:
	ldr	r1, .L2157+20
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2157+24
	str	r0, [r5, #60]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #18]
	cmp	r0, #1
	beq	.L2112
.L2130:
	ldr	r1, .L2157+28
	mov	r0, r4
	bl	mvc_se_v
	add	r3, r0, #12
	str	r0, [r5, #56]
	cmp	r3, #24
	bhi	.L2113
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2131
.L2113:
	ldr	r1, .L2157+32
	mov	r0, #1
	bl	dprint_vfmw
.L2148:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2104:
	ldr	r1, .L2157+36
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #31
	str	r0, [r5, #36]
	bhi	.L2105
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2105
	ldr	r1, .L2157+40
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #31
	str	r0, [r5, #40]
	bhi	.L2107
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2107
	ldr	r1, .L2157+44
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #2]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2148
	ldr	r2, .L2157+48
	mov	r1, #2
	mov	r0, r4
	bl	mvc_u_v
	cmp	r0, #2
	str	r0, [r5, #44]
	bhi	.L2149
	ldr	r1, .L2157+52
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2157+56
	str	r0, [r5, #48]
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2157+60
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2157+64
	str	r0, [r5, #52]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2157+68
	strb	r0, [r5, #3]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2157+72
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #17]
	cmp	r0, #0
	bne	.L2150
.L2110:
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
	bcs	.L2111
.L2131:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2150:
	ldr	r1, .L2157+76
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2110
.L2105:
	ldr	r1, .L2157+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2101
.L2107:
	ldr	r1, .L2157+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2101
.L2112:
	sub	r8, fp, #72
	add	r10, r5, #4
	add	r7, r5, #2016
	add	r9, r5, #64
	b	.L2121
.L2154:
	ldr	lr, [r5, #28]
	ldr	r3, [r4, #248]
	mla	r3, ip, lr, r3
	ldrb	ip, [r3, #27]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L2151
	ldr	r3, .L2157+88
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2157+92
	ldr	r3, [r3, #52]
	blx	r3
.L2116:
	add	r6, r6, #1
	add	r7, r7, #16
	cmp	r6, #6
	add	r9, r9, #64
	beq	.L2152
.L2121:
	ldr	r1, .L2157+96
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
	beq	.L2153
	cmp	r6, #0
	cmpne	r6, #3
	mov	r1, r1, lsr #5
	beq	.L2154
	ldr	r3, .L2157+92
	mov	r2, #16
	sub	r1, r7, #16
	mov	r0, r7
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2116
.L2152:
	add	r8, r5, #2112
	add	r10, r5, #448
	sub	r7, fp, #76
	add	r9, r5, #11
	mov	r6, #0
.L2129:
	ldr	r2, [r5, #60]
	mov	r3, #0
	cmp	r2, #1
	strne	r3, [r7]
	strneb	r3, [r9]
	beq	.L2155
.L2124:
	ldr	r1, [r4, #248]
	movw	ip, #3992
	ldr	r3, [r5, #28]
	mov	r0, r8
	mov	r2, #64
	mla	r3, ip, r3, r1
	ldrb	r1, [r3, #27]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L2156
	ldr	r1, .L2157+100
	cmp	r6, #0
	ldr	r3, .L2157+92
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2125:
	add	r6, r6, #1
	add	r8, r8, #64
	cmp	r6, #2
	add	r10, r10, #256
	add	r7, r7, #4
	add	r9, r9, #1
	bne	.L2129
	b	.L2130
.L2153:
	mov	r3, r2
	str	r8, [sp]
	mov	r2, r7
	mov	r1, r9
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r8]
	cmp	r3, #1
	bne	.L2116
	ldr	r1, .L2157+88
	mov	r2, #16
	ldr	r3, .L2157+92
	cmp	r6, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r7
	blx	r3
	b	.L2116
.L2151:
	ldr	r2, .L2157+92
	add	r1, r6, #167
	mov	r0, r7
	add	r1, r3, r1, lsl #4
	ldr	ip, [r2, #52]
	mov	r2, #16
	blx	ip
	b	.L2116
.L2149:
	ldr	r1, .L2157+104
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2101
.L2155:
	ldr	r1, .L2157+108
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
	bne	.L2124
	str	r7, [sp]
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2125
	ldr	r1, .L2157+100
	mov	r2, #64
	ldr	r3, .L2157+92
	cmp	r6, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r8
	blx	r3
	b	.L2125
.L2156:
	add	r1, r3, r6, lsl #6
	ldr	r3, .L2157+92
	add	r1, r1, #2768
	ldr	ip, [r3, #52]
	blx	ip
	b	.L2125
.L2158:
	.align	2
.L2157:
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
	beq	.L2186
	cmp	r1, #0
	beq	.L2187
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
	beq	.L2163
	mov	ip, ip, asl #1
	adds	r2, ip, #6
	str	r2, [fp, #-56]
	beq	.L2163
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
.L2168:
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
	beq	.L2185
	ldr	r2, [r8]
	ldr	r1, [r7]
	cmp	r2, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r0, #5
	bhi	.L2165
	cmp	r2, #0
	add	r4, r10, #64
	bne	.L2164
	add	r10, r10, #128
	mov	r1, r6
	mov	r2, r4
.L2166:
	ldr	lr, [r2], #4
	ldr	ip, [r1], #4
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2166
.L2164:
	ldr	r2, [fp, #-56]
	add	r0, r0, #1
	add	r8, r8, #4
	add	r7, r7, #4
	cmp	r0, r2
	add	r5, r5, #256
	add	r9, r9, #256
	mov	r10, r4
	add	r6, r6, #64
	bne	.L2168
	ldr	r6, [fp, #-60]
	ldr	r1, [fp, #-64]
.L2163:
	ldr	r0, [r6, #56]
	ldr	r2, [r1, #56]
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L2161:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2165:
	cmp	r2, #0
	bne	.L2185
	sub	r2, r5, #1088
	sub	r4, r5, #836
	sub	r2, r2, #4
	mov	r1, r9
.L2167:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r4
	bne	.L2167
.L2185:
	add	r4, r10, #64
	b	.L2164
.L2187:
	mov	r0, r1
	movw	r3, #10242
	ldr	r2, .L2188
	ldr	r1, .L2188+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2161
.L2186:
	movw	r3, #10241
	ldr	r2, .L2188
	ldr	r1, .L2188+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2161
.L2189:
	.align	2
.L2188:
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
	ldr	r1, .L2205
	mov	r4, r0
	bl	mvc_ue_v
	ldr	r3, [r4, #40]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r5, r0
	bls	.L2191
	mov	r2, r0
	ldr	r1, .L2205+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r4, #40]
	sub	r3, r2, #1
	cmp	r5, r3
	bls	.L2198
	ldr	r3, .L2205+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2198
	str	r2, [fp, #-40]
	mov	r3, #8
	str	r5, [fp, #-44]
	sub	r2, fp, #44
	ldr	r0, [r4, #120]
	mov	r1, #110
	blx	ip
	mvn	r0, #0
.L2192:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2191:
	ldr	r1, .L2205+12
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r3, [r4, #36]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	ip, r0
	bhi	.L2202
	mov	r8, #2240
	ldr	r3, [r4, #252]
	mul	r6, r8, r5
	add	r3, r3, r6
	ldrb	r7, [r3, #19]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L2194
	ldr	r7, .L2205+16
	mov	r0, r4
	mov	r1, r7
	str	r5, [r7, #24]
	str	ip, [r7, #28]
	bl	MVC_ProcessPPS
	subs	r5, r0, #0
	bne	.L2203
	ldr	r1, [r4, #252]
	mov	r0, r7
	add	r1, r1, r6
	bl	MVC_PPSEqual
	cmp	r0, #0
	beq	.L2192
	ldr	ip, .L2205+20
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
	b	.L2192
.L2194:
	str	r5, [r3, #24]
	mov	r0, r4
	ldr	r3, [r4, #252]
	add	r3, r3, r6
	str	ip, [r3, #28]
	ldr	r1, [r4, #252]
	add	r1, r1, r6
	bl	MVC_ProcessPPS
	cmp	r0, #0
	bne	.L2204
	ldr	r3, [r4, #252]
	mov	r2, #1
	add	r3, r3, r6
	strb	r2, [r3, #20]
	ldr	r3, [r4, #252]
	add	r6, r3, r6
	strb	r2, [r6, #19]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2198:
	mvn	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2202:
	ldr	r1, .L2205+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2192
.L2203:
	movw	r2, #10005
	ldr	r1, .L2205+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2192
.L2204:
	movw	r2, #10025
	ldr	r1, .L2205+28
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
	b	.L2192
.L2206:
	.align	2
.L2205:
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
	ldr	r1, .L2255
	mov	r4, r0
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	streqb	r0, [r5, #15]
	bne	.L2249
.L2209:
	ldr	r1, .L2255+4
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #1]
	cmp	r0, #0
	bne	.L2250
.L2210:
	ldr	r1, .L2255+8
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #3]
	cmp	r0, #0
	moveq	r3, #5
	streq	r3, [r5, #28]
	bne	.L2251
.L2212:
	ldr	r1, .L2255+12
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #6]
	cmp	r0, #0
	bne	.L2252
.L2213:
	ldr	r1, .L2255+16
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #8]
	cmp	r0, #0
	bne	.L2253
.L2215:
	ldr	r1, .L2255+20
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #7]
	cmp	r0, #0
	beq	.L2217
	ldr	r1, .L2255+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r2, .L2255+28
	mov	r1, #4
	str	r0, [r5, #96]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+32
	mov	r1, #4
	strb	r0, [r5, #92]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [r5, #96]
	cmp	r3, #31
	strb	r0, [r5, #93]
	bhi	.L2220
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2220
	add	r7, r5, #96
	add	r8, r5, #59
.L2221:
	ldr	r1, .L2255+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #96]
	cmp	r3, r6
	bcs	.L2221
	ldr	r2, .L2255+48
	mov	r1, #5
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+52
	mov	r1, #5
	str	r0, [r5, #356]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+56
	mov	r1, #5
	str	r0, [r5, #360]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+60
	mov	r1, #5
	str	r0, [r5, #364]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #368]
.L2217:
	ldr	r1, .L2255+64
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #10]
	cmp	r0, #0
	bne	.L2254
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2226
.L2225:
	ldr	r1, .L2255+68
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2255+72
	strb	r0, [r5, #11]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #12]
	cmp	r0, #0
	bne	.L2227
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
.L2228:
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2254:
	ldr	r1, .L2255+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r2, .L2255+28
	mov	r1, #4
	str	r0, [r5, #408]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+32
	mov	r1, #4
	strb	r0, [r5, #404]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [r5, #408]
	cmp	r3, #31
	strb	r0, [r5, #405]
	bhi	.L2220
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2220
	add	r8, r5, #368
	add	r7, r5, #408
	add	r8, r8, #3
.L2223:
	ldr	r1, .L2255+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #408]
	cmp	r3, r6
	bcs	.L2223
	ldr	r2, .L2255+48
	mov	r1, #5
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+52
	mov	r1, #5
	str	r0, [r5, #668]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+56
	mov	r1, #5
	str	r0, [r5, #672]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+60
	mov	r1, #5
	str	r0, [r5, #676]
	mov	r0, r4
	bl	mvc_u_v
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r5, #680]
	bne	.L2226
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2225
.L2226:
	ldr	r1, .L2255+76
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #14]
	b	.L2225
.L2253:
	ldr	r2, .L2255+80
	mov	r1, #32
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+84
	mov	r1, #32
	str	r0, [r5, #52]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2255+88
	str	r0, [r5, #56]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #9]
	cmp	r0, #1
	bne	.L2215
	ldr	r1, [r5, #52]
	cmp	r1, #0
	beq	.L2215
	ldr	r0, [r5, #56]
	bl	__aeabi_uidiv
	mov	r0, r0, lsr #1
	str	r0, [r4, #56]
	b	.L2215
.L2252:
	ldr	r1, .L2255+92
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+96
	str	r0, [r5, #44]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #48]
	b	.L2213
.L2251:
	ldr	r2, .L2255+100
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2255+104
	str	r0, [r5, #28]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2255+108
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #5]
	cmp	r0, #0
	beq	.L2212
	ldr	r2, .L2255+112
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+116
	mov	r1, #8
	str	r0, [r5, #32]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+120
	mov	r1, #8
	str	r0, [r5, #36]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #40]
	b	.L2212
.L2250:
	ldr	r1, .L2255+124
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #2]
	b	.L2210
.L2249:
	ldr	r2, .L2255+128
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	uxtb	r0, r0
	strb	r0, [r5, #15]
	cmp	r0, #255
	bne	.L2209
	ldr	r2, .L2255+132
	mov	r1, #16
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2255+136
	mov	r1, #16
	str	r0, [r5, #16]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #20]
	b	.L2209
.L2227:
	ldr	r1, .L2255+140
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2255+144
	strb	r0, [r5, #13]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+148
	str	r0, [r5, #684]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+152
	str	r0, [r5, #688]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+156
	str	r0, [r5, #696]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+160
	str	r0, [r5, #692]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2255+164
	str	r0, [r5, #700]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #704]
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L2228
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2220:
	ldr	r1, .L2255+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2256:
	.align	2
.L2255:
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
	beq	.L2289
	cmp	r1, #0
	beq	.L2290
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
	beq	.L2261
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
.L2266:
	ldrb	r2, [r4, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r2
	cmp	ip, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, #0
	beq	.L2262
	cmp	r0, #5
	bhi	.L2263
	add	r2, r6, #748
	add	r10, r6, #812
	mov	r1, r7
.L2264:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2264
.L2262:
	add	r0, r0, #1
	add	r8, r8, #256
	cmp	r0, #8
	add	r9, r9, #256
	add	r6, r6, #64
	add	r7, r7, #64
	bne	.L2266
	ldr	r5, [fp, #-48]
	ldr	r1, [fp, #-52]
.L2261:
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
	beq	.L2268
	cmp	ip, #1
	bne	.L2267
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
	beq	.L2267
	add	ip, r5, #2912
	add	r0, r1, #2912
	add	ip, ip, #4
	add	r0, r0, #4
	mov	r2, #0
.L2270:
	ldr	r4, [ip, #4]!
	add	r2, r2, #1
	ldr	lr, [r0, #4]!
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r6
	bne	.L2270
.L2267:
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
	bne	.L2271
	ldrb	r0, [r5, #21]	@ zero_extendqisi2
	ldrb	r2, [r1, #21]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
.L2271:
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
	bne	.L2291
.L2272:
	ldrb	r0, [r5, #24]	@ zero_extendqisi2
	ldrb	r2, [r1, #24]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L2259:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2291:
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
	b	.L2272
.L2263:
	sub	r2, r8, #404
	sub	r10, r8, #148
	mov	r1, r9
.L2265:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2265
	b	.L2262
.L2268:
	ldr	r0, [r5, #2904]
	ldr	r2, [r1, #2904]
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
	b	.L2267
.L2290:
	mov	r0, r1
	movw	r3, #10486
	ldr	r2, .L2292
	ldr	r1, .L2292+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2259
.L2289:
	movw	r3, #10485
	ldr	r2, .L2292
	ldr	r1, .L2292+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2259
.L2293:
	.align	2
.L2292:
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
	beq	.L2317
	subs	ip, r4, #255
	movne	ip, #1
	cmp	r4, #16
	movle	lr, #0
	andgt	lr, ip, #1
	cmp	lr, #0
	bne	.L2306
	cmp	r2, #0
	cmpne	r1, #0
	moveq	r0, #1
	movne	r0, #0
	cmp	r4, #255
	movne	r0, #0
	andeq	r0, r0, #1
	cmp	r0, #0
	bne	.L2307
	cmp	r4, #1
	moveq	r2, r0
	moveq	r4, #5
	beq	.L2295
	cmp	ip, #0
	ldrne	r0, .L2318
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
	bgt	.L2299
	rsb	r1, r0, #2256
	add	r1, r1, #7
	cmp	r3, r1
	bge	.L2300
.L2310:
	mov	r4, #4
	b	.L2295
.L2306:
	mov	r4, #0
.L2317:
	mov	r2, r4
.L2295:
	mov	r3, r4
	ldr	r1, .L2318+4
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2299:
	sub	r1, r0, #2256
	sub	r1, r1, #7
	cmp	r3, r1
	blt	.L2310
.L2300:
	movw	r3, #1819
	cmp	r2, r3
	bgt	.L2301
	rsb	r3, r2, #1808
	add	r3, r3, #12
	cmp	r1, r3
	blt	.L2312
.L2302:
	movw	r1, #1364
	cmp	r2, r1
	rsble	r4, r2, #1360
	subgt	r4, r2, #1360
	addle	r4, r4, #5
	subgt	r4, r4, #5
	cmp	r3, r4
	movlt	r4, #2
	movge	r4, #1
	b	.L2295
.L2307:
	mov	r4, lr
	mov	r2, lr
	b	.L2295
.L2301:
	sub	r3, r2, #1808
	sub	r3, r3, #12
	cmp	r1, r3
	bge	.L2302
.L2312:
	mov	r4, #3
	b	.L2295
.L2319:
	.align	2
.L2318:
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
	beq	.L2433
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
	bne	.L2434
.L2333:
	ldr	r1, .L2448
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #12
	str	r0, [r5, #2896]
	bhi	.L2346
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2346
	ldr	r1, .L2448+4
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r5, #2900]
	bhi	.L2348
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2348
	cmp	r0, #0
	beq	.L2435
	cmp	r0, #1
	beq	.L2436
.L2354:
	ldr	r1, .L2448+8
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #3944]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	ldr	r1, .L2448+12
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #19]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	ldr	r1, .L2448+16
	mov	r0, r4
	bl	mvc_ue_v
	movw	r7, #509
	sub	r3, r0, #1
	str	r0, [r5, #3948]
	cmp	r3, r7
	bhi	.L2359
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2359
	ldr	r1, .L2448+20
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #3952]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	ldr	r1, .L2448+24
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #20]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	cmp	r0, #0
	bne	.L2437
	ldr	r1, .L2448+28
	mov	r0, r4
	bl	mvc_u_1
	ldr	r6, [r5, #3952]
	add	r6, r6, #1
	cmp	r6, #255
	strb	r0, [r5, #21]
	bhi	.L2392
	ldr	r3, [r5, #3948]
	add	r3, r3, #1
	cmp	r6, #1
	cmphi	r3, #3
	movhi	r8, #2
	bhi	.L2363
.L2392:
	ldr	r1, .L2448+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2434:
	ldr	r1, .L2448+36
	bl	mvc_ue_v
	cmp	r0, #1
	str	r0, [r5, #748]
	bhi	.L2324
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2324
	ldr	r1, .L2448+40
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L2326
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2327
.L2326:
	ldr	r1, .L2448+44
	mov	r0, #1
	bl	dprint_vfmw
.L2327:
	ldr	r1, .L2448+48
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L2328
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2329
.L2328:
	ldr	r1, .L2448+52
	mov	r0, #1
	bl	dprint_vfmw
.L2329:
	ldr	r1, .L2448+56
	mov	r0, r4
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L2330
	ldrb	r10, [r4, #10]	@ zero_extendqisi2
	cmp	r10, #0
	bne	.L2330
	ldr	r1, .L2448+60
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #27]
	cmp	r0, #1
	bne	.L2333
	sub	r7, fp, #72
	add	r9, r5, #5
	add	r6, r5, #2672
	add	r8, r5, #752
	b	.L2339
.L2440:
	ldr	r3, .L2448+64
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2448+68
	ldr	r3, [r3, #52]
	blx	r3
.L2335:
	add	r10, r10, #1
	add	r6, r6, #16
	cmp	r10, #6
	add	r8, r8, #64
	beq	.L2438
.L2339:
	ldr	r1, .L2448+72
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
	beq	.L2439
	cmp	r10, #0
	cmpne	r10, #3
	beq	.L2440
	ldr	r3, .L2448+68
	mov	r2, #16
	sub	r1, r6, #16
	mov	r0, r6
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2335
.L2390:
	cmp	r1, r9
	cmpcs	r0, r6
	bcs	.L2389
	ldr	r3, .L2448+76
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2431
	strh	r1, [fp, #-72]	@ movhi
	mov	r3, #8
	strh	r0, [fp, #-70]	@ movhi
	sub	r2, fp, #76
	strh	r9, [fp, #-76]	@ movhi
	mov	r1, #107
	strh	r6, [fp, #-74]	@ movhi
	ldr	r0, [r4, #120]
	blx	ip
.L2431:
	mvn	r0, #0
.L2424:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2437:
	ldr	r6, [r5, #3952]
	strb	r3, [r5, #21]
	sub	r3, r6, #1
	cmp	r3, r7
	bhi	.L2392
	ldr	r3, [r5, #3948]
	mov	r8, #1
	add	r6, r6, #1
	add	r3, r3, r8
.L2363:
	mul	r6, r6, r8
	mov	r9, r3, asl #4
	mov	r6, r6, asl #4
	mul	r1, r9, r6
	add	r1, r1, r1, lsr #1
	cmp	r1, #100663296
	bhi	.L2441
	ldr	r2, [r5, #740]
	sub	r3, r2, #10
	cmp	r3, #31
	ldrls	pc, [pc, r3, asl #2]
	b	.L2365
.L2367:
	.word	.L2366
	.word	.L2368
	.word	.L2399
	.word	.L2399
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2399
	.word	.L2370
	.word	.L2372
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2372
	.word	.L2373
	.word	.L2374
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2365
	.word	.L2376
	.word	.L2376
.L2435:
	ldr	r1, .L2448+80
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #12
	str	r0, [r5, #2904]
	bhi	.L2351
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2354
.L2351:
	ldr	r1, .L2448+84
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2399:
	mov	r0, #60416
	movt	r0, 13
.L2369:
	bl	__aeabi_uidiv
	ldr	r1, .L2448+88
	cmp	r0, #16
	movcc	r7, r0
	movcs	r7, #16
	mov	r2, r7
	mov	r0, #21
	bl	dprint_vfmw
	ldr	r2, [r5, #3944]
	cmp	r2, r7
	bhi	.L2377
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2378
.L2377:
	mov	r3, r7
	ldr	r1, .L2448+92
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r5, #3944]
	cmp	r3, #16
	bhi	.L2428
	cmp	r7, r3
	movcc	r7, r3
.L2428:
	str	r7, [r5, #3944]
.L2378:
	ldr	r1, .L2448+96
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #22]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	ldr	r1, .L2448+100
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #23]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	cmp	r0, #0
	bne	.L2442
.L2380:
	mov	r2, r9, lsr #1
	mov	r3, r6, lsr #1
	str	r9, [r5, #3976]
	str	r6, [r5, #3980]
	str	r2, [r5, #3984]
	str	r3, [r5, #3988]
.L2382:
	ldr	r1, .L2448+104
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r3, r0
	strb	r3, [r5, #24]
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2431
	cmp	r3, #0
	mov	r2, #2
	str	r2, [r5, #68]
	bne	.L2443
	mov	r0, r3
	strb	r3, [r5, #43]
	mov	r3, #5
	str	r3, [r5, #56]
.L2385:
	ldr	r2, [r5, #48]
	mov	r3, r9
	ldr	r1, [r5, #44]
	str	r6, [sp]
	bl	MVC_GetDar
	ldr	r3, [r5, #3944]
	cmp	r3, #0
	str	r0, [r5, #52]
	beq	.L2388
	cmp	r7, r3
	movcs	r7, r3
.L2388:
	ldr	r3, [r4, #224]
	cmp	r7, #16
	movcs	r7, #16
	ldr	r2, [r3, #28]
	cmp	r2, #25
	beq	.L2444
.L2389:
	cmp	r6, #1920
	bcc	.L2391
	cmp	r7, #4
	movcs	r7, #4
.L2391:
	add	r7, r7, #1
	mov	r0, #0
	str	r7, [r5, #3972]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2376:
	mov	r0, #12582912
	b	.L2369
.L2372:
	mov	r0, #30208
	movt	r0, 47
	b	.L2369
.L2438:
	sub	r7, fp, #88
	add	r9, r5, #11
	add	r6, r5, #2768
	add	r8, r5, #1136
	mov	r10, #0
.L2344:
	ldr	r1, .L2448+72
	mov	r0, r4
	bl	mvc_u_1
	mov	r2, #64
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r6
	str	r3, [r7, #4]!
	strb	r3, [r9, #1]!
	beq	.L2445
	ldr	r1, .L2448+108
	cmp	r10, #0
	ldr	r3, .L2448+68
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2341:
	add	r10, r10, #1
	add	r6, r6, #64
	cmp	r10, #2
	add	r8, r8, #256
	bne	.L2344
	b	.L2333
.L2439:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2335
	ldr	r1, .L2448+64
	mov	r2, #16
	ldr	r3, .L2448+68
	cmp	r10, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2335
.L2324:
	ldr	r1, .L2448+112
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2346:
	ldr	r1, .L2448+116
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2442:
	ldr	r1, .L2448+120
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2448+124
	str	r0, [r5, #3956]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2448+128
	str	r0, [r5, #3960]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2448+132
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
	ble	.L2380
	add	r2, r2, r3, lsr #1
	add	r8, r8, r0, lsr #1
	str	r2, [r5, #3984]
	str	r8, [r5, #3988]
	str	r3, [r5, #3976]
	str	r0, [r5, #3980]
	b	.L2382
.L2443:
	add	r1, r5, #28
	mov	r0, r4
	bl	mvc_vui_parameters
	cmp	r0, #0
	bne	.L2430
	ldrb	r3, [r5, #40]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2430
	ldr	r2, [r5, #732]
	clz	r3, r2
	mov	r3, r3, lsr #5
	cmp	r7, r2
	orrcc	r3, r3, #1
	cmp	r3, #0
	bne	.L2446
	ldr	r3, [r5, #3944]
	ldrb	r0, [r5, #43]	@ zero_extendqisi2
	cmp	r2, r3
	movcs	r7, r2
	movcc	r7, r3
	b	.L2385
.L2446:
	mov	r3, r7
	ldr	r1, .L2448+136
	mov	r0, #1
	bl	dprint_vfmw
.L2430:
	ldrb	r0, [r5, #43]	@ zero_extendqisi2
	b	.L2385
.L2444:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L2389
	ldr	r2, [r3, #692]
	ldr	r1, [r3, #668]
	cmp	r7, r2
	ldr	r0, [r3, #672]
	bls	.L2390
	ldr	r3, .L2448+76
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2431
	str	r2, [fp, #-72]
	mov	r3, #8
	str	r7, [fp, #-76]
	sub	r2, fp, #76
	ldr	r0, [r4, #120]
	mov	r1, #106
	blx	ip
	mvn	r0, #0
	b	.L2424
.L2330:
	ldr	r1, .L2448+140
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2436:
	ldr	r1, .L2448+144
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #18]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	ldr	r1, .L2448+148
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r5, #2908]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	ldr	r1, .L2448+152
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r5, #2912]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2431
	ldr	r1, .L2448+156
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #255
	str	r0, [r5, #2916]
	bhi	.L2356
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2356
	cmp	r0, #0
	addne	r7, r5, #2912
	addne	r7, r7, #4
	bne	.L2358
	b	.L2354
.L2447:
	ldr	r3, [r5, #2916]
	cmp	r3, r6
	bls	.L2354
.L2358:
	ldr	r1, .L2448+160
	mov	r0, r4
	bl	mvc_se_v
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2447
	b	.L2431
.L2359:
	mov	r2, r0
	ldr	r1, .L2448+164
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2373:
	mov	r0, #30720
	movt	r0, 105
	b	.L2369
.L2370:
	mov	r0, #55296
	movt	r0, 27
	b	.L2369
.L2374:
	mov	r0, #7864320
	b	.L2369
.L2368:
	mov	r0, #17920
	movt	r0, 5
	b	.L2369
.L2366:
	mov	r0, #20992
	movt	r0, 2
	b	.L2369
.L2365:
	ldr	r1, .L2448+168
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2356:
	ldr	r1, .L2448+172
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2445:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2341
	ldr	r1, .L2448+108
	mov	r2, #64
	ldr	r3, .L2448+68
	cmp	r10, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2341
.L2348:
	ldr	r1, .L2448+176
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2441:
	ldr	r1, .L2448+180
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2433:
	mov	r0, r5
	movw	r3, #10675
	ldr	r2, .L2448+184
	ldr	r1, .L2448+188
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2424
.L2449:
	.align	2
.L2448:
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
	ldr	r2, .L2495
	mov	r1, #8
	mov	r4, r0
	bl	mvc_u_v
	ldr	r1, .L2495+4
	mov	r7, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2495+8
	mov	r9, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2495+12
	mov	r8, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2495+16
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2495+20
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2495+24
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r2, .L2495+28
	mov	r1, #2
	str	r0, [fp, #-68]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2495+32
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2495+36
	mov	r5, r0
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	mov	r6, r0
	bne	.L2488
	ldr	r2, [r4, #36]
	sub	r2, r2, #1
	cmp	r0, r2
	bhi	.L2490
	cmp	r7, #100
	mov	r10, r7
	beq	.L2456
	bhi	.L2457
	cmp	r7, #77
	beq	.L2456
	cmp	r7, #88
	beq	.L2458
	cmp	r7, #66
	bne	.L2455
	ldr	r1, .L2495+40
	mov	r0, #1
	bl	dprint_vfmw
.L2456:
	cmp	r5, #9
	mov	r2, r5
	bls	.L2487
.L2493:
	cmp	r5, #41
	bhi	.L2487
.L2462:
	movw	r1, #3992
	ldr	r3, [r4, #248]
	mul	r5, r1, r6
	add	r3, r3, r5
	ldrb	ip, [r3, #25]	@ zero_extendqisi2
	cmp	ip, #0
	beq	.L2463
	ldr	r3, .L2495+44
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
	bne	.L2491
	ldr	r1, [r4, #248]
	add	r0, r3, #2240
	str	r3, [fp, #-56]
	add	r1, r1, r5
	bl	MVC_SPSEqual
	ldr	r3, [fp, #-56]
	cmp	r0, #0
	bne	.L2492
.L2452:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2457:
	cmp	r7, #122
	beq	.L2460
	cmp	r7, #144
	beq	.L2460
	cmp	r7, #110
	beq	.L2460
.L2455:
	mov	r2, r7
	ldr	r1, .L2495+48
	mov	r0, #1
	mov	r10, #100
	bl	dprint_vfmw
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2493
.L2487:
	mov	r3, #41
	ldr	r1, .L2495+52
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, #41
	b	.L2462
.L2463:
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
	bne	.L2494
	ldr	r3, [r4, #248]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #26]
	ldr	r3, [r4, #248]
	add	r5, r3, r5
	strb	r2, [r5, #25]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2490:
	ldr	r1, .L2495+56
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r4, #36]
	sub	r3, r2, #1
	cmp	r6, r3
	bls	.L2488
	ldr	r3, .L2495+60
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2488
	str	r2, [fp, #-48]
	mov	r3, #8
	str	r6, [fp, #-52]
	sub	r2, fp, #52
	ldr	r0, [r4, #120]
	mov	r1, #109
	blx	ip
.L2488:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2460:
	ldr	r1, .L2495+64
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2493
	b	.L2487
.L2458:
	ldr	r1, .L2495+68
	mov	r0, #1
	bl	dprint_vfmw
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2493
	b	.L2487
.L2492:
	ldr	lr, .L2495+72
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
	b	.L2452
.L2491:
	mov	r3, r6
	movw	r2, #11219
	ldr	r1, .L2495+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2452
.L2494:
	mov	r3, r6
	movw	r2, #11246
	ldr	r1, .L2495+76
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
	b	.L2452
.L2496:
	.align	2
.L2495:
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
	beq	.L2561
	ldr	r1, .L2562
	bl	mvc_ue_v
	cmp	r0, #1
	str	r0, [r5, #4]
	bhi	.L2502
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	addeq	r7, r5, #4
	bne	.L2502
.L2504:
	ldr	r1, .L2562+4
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldr	r3, [r5, #4]
	cmp	r3, r6
	bcs	.L2504
	cmp	r3, #0
	mov	r2, #0
	str	r2, [r5, #16]
	str	r2, [r5, #24]
	beq	.L2505
	add	r8, r5, #16
	mov	r7, #1
.L2515:
	ldr	r1, .L2562+8
	mov	r0, r4
	bl	mvc_ue_v
	add	r9, r8, #4
	str	r0, [r8, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcs	r3, #15
	cmp	r0, r3
	bhi	.L2506
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2506
	cmp	r0, #0
	addne	r10, r8, #16
	beq	.L2511
.L2510:
	ldr	r1, .L2562+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r10, #4]!
	ldr	r3, [r9]
	cmp	r6, r3
	bcc	.L2510
.L2511:
	ldr	r1, .L2562+16
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #8]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcc	r2, r3
	movcs	r2, #15
	cmp	r0, r2
	bhi	.L2508
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2508
	cmp	r0, #0
	addne	r8, r8, #24
	beq	.L2513
.L2514:
	ldr	r1, .L2562+20
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r8, #4]!
	ldr	r3, [r9, #8]
	cmp	r6, r3
	bcc	.L2514
	ldr	r3, [r5, #4]
.L2513:
	add	r7, r7, #1
	mov	r8, r9
	cmp	r7, r3
	bls	.L2515
	cmp	r3, #0
	mov	r3, #0
	addne	r8, r5, #48
	str	r3, [r5, #48]
	str	r3, [r5, #56]
	movne	r7, #1
	beq	.L2529
.L2528:
	ldr	r1, .L2562+24
	mov	r0, r4
	bl	mvc_ue_v
	add	r9, r8, #4
	str	r0, [r8, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcs	r3, #15
	cmp	r0, r3
	bhi	.L2519
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2519
	cmp	r0, #0
	addne	r10, r8, #16
	beq	.L2524
.L2523:
	ldr	r1, .L2562+28
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r10, #4]!
	ldr	r3, [r9]
	cmp	r6, r3
	bcc	.L2523
.L2524:
	ldr	r1, .L2562+32
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #8]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcc	r2, r3
	movcs	r2, #15
	cmp	r0, r2
	bhi	.L2521
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2521
	cmp	r0, #0
	addne	r8, r8, #24
	beq	.L2526
.L2527:
	ldr	r1, .L2562+36
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r8, #4]!
	ldr	r3, [r9, #8]
	cmp	r6, r3
	bcc	.L2527
	ldr	r3, [r5, #4]
.L2526:
	add	r7, r7, #1
	mov	r8, r9
	cmp	r7, r3
	bls	.L2528
.L2529:
	ldr	r1, .L2562+40
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r0, #1
	str	r0, [r5, #80]
	cmp	r3, #16
	str	r3, [fp, #-60]
	bhi	.L2517
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2517
	ldr	r2, [fp, #-60]
	cmp	r2, #0
	beq	.L2534
	add	r2, r5, #8384
	add	r1, r5, #80
	add	r2, r2, #16
	str	r1, [fp, #-72]
	str	r2, [fp, #-68]
	add	r2, r5, #212
	str	r3, [fp, #-56]
	str	r2, [fp, #-64]
	str	r5, [fp, #-76]
.L2533:
	ldr	r2, .L2562+44
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-72]
	ldr	r1, .L2562+48
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
	bhi	.L2531
	ldrb	r7, [r4, #10]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L2531
	cmp	r2, #0
	beq	.L2541
	ldr	r3, [fp, #-64]
	movw	r10, #8188
	ldr	r2, [fp, #-68]
	movt	r10, 4
	add	r10, r3, r10
	mov	r8, r3
	str	r2, [fp, #-48]
.L2540:
	ldr	r2, .L2562+52
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2562+56
	str	r0, [r8], #4
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r0, #1
	str	r0, [r8, #4092]
	cmp	r6, #64
	bhi	.L2535
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2535
	cmp	r6, #0
	movne	r5, r3
	ldrne	r9, [fp, #-48]
	beq	.L2539
.L2536:
	ldr	r1, .L2562+60
	mov	r0, r4
	bl	mvc_ue_v
	add	r5, r5, #1
	cmp	r6, r5
	str	r0, [r9, #4]!
	bne	.L2536
.L2539:
	ldr	r1, .L2562+64
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #1024
	str	r0, [r10, #4]!
	bcs	.L2537
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2537
	ldr	r3, [fp, #-48]
	add	r7, r7, #1
	add	r3, r3, #256
	str	r3, [fp, #-48]
	ldr	r3, [fp, #-52]
	cmp	r3, r7
	bne	.L2540
.L2541:
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
	bne	.L2533
.L2534:
	mov	r0, #0
	b	.L2553
.L2506:
	mov	r2, r0
	ldr	r1, .L2562+68
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
.L2553:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2508:
	mov	r2, r0
	ldr	r1, .L2562+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2521:
	mov	r2, r0
	ldr	r1, .L2562+76
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2519:
	mov	r2, r0
	ldr	r1, .L2562+80
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2502:
	mov	r2, r0
	movw	r3, #65534
	ldr	r1, .L2562+84
	mov	r0, #1
	str	r3, [r5, #80]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2505:
	str	r3, [r5, #48]
	str	r3, [r5, #56]
	b	.L2529
.L2537:
	mov	r2, r0
	ldr	r1, .L2562+88
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2535:
	ldr	r3, [fp, #-56]
	movw	ip, #65534
	ldr	r5, [fp, #-76]
	mov	r2, r6
	ldr	r1, .L2562+92
	mov	r0, #1
	add	r3, r7, r3, lsl #6
	add	r3, r3, #1072
	add	r3, r3, #4
	add	r3, r5, r3, lsl #2
	str	ip, [r3, #4]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2561:
	mov	r0, r5
	movw	r3, #11366
	ldr	r2, .L2562+96
	ldr	r1, .L2562+100
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2517:
	movw	r3, #65534
	ldr	r2, [fp, #-60]
	ldr	r1, .L2562+104
	mov	r0, #1
	str	r3, [r5, #80]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2531:
	ldr	r3, [fp, #-56]
	mov	r0, #1
	ldr	r5, [fp, #-76]
	ldr	r2, [fp, #-52]
	add	r5, r5, r3, lsl #2
	ldr	r1, .L2562+108
	movw	r3, #65534
	str	r3, [r5, #148]
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2553
.L2563:
	.align	2
.L2562:
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
	ldr	r1, .L2597
	mov	r4, r0
	bl	mvc_ue_v
	add	r3, r5, #278528
	cmp	r0, #1024
	str	r0, [r3, #560]
	bcs	.L2594
	adds	r6, r0, #1
	mov	r1, #0
	adc	r7, r1, #0
	strd	r6, [fp, #-116]
	orrs	r2, r6, r7
	beq	.L2582
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
.L2581:
	ldr	r2, .L2597+4
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-88]
	ldr	r1, .L2597+8
	strb	r0, [r3, #1]!
	mov	r0, r4
	str	r3, [fp, #-88]
	bl	mvc_ue_v
	ldr	r3, [fp, #-92]
	adds	r5, r0, #1
	str	r0, [r3, #4]!
	str	r3, [fp, #-92]
	beq	.L2571
	ldr	r10, [fp, #-60]
	mov	r7, #0
	ldr	r6, [fp, #-48]
.L2568:
	ldr	r1, .L2597+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r7, r7, #1
	cmp	r5, r7
	str	r0, [r10, #4]!
	bne	.L2568
	str	r6, [fp, #-48]
.L2571:
	ldr	r1, .L2597+16
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-96]
	uxtb	r0, r0
	strb	r0, [r3, #1]!
	cmp	r0, #0
	str	r3, [fp, #-96]
	bne	.L2595
.L2570:
	ldr	r1, .L2597+20
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-52]
	uxtb	r0, r0
	strb	r0, [r3]
	cmp	r0, #0
	beq	.L2572
	ldr	r1, .L2597+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r5, [fp, #-48]
	ldr	r2, .L2597+28
	mov	r1, #4
	str	r0, [r5]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34132
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2597+32
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
	bhi	.L2574
	ldrb	r5, [r4, #10]	@ zero_extendqisi2
	cmp	r5, #0
	bne	.L2574
	ldr	r6, [fp, #-48]
	movw	r10, #34099
	movt	r10, 4
	add	r10, r8, r10
	mov	r7, r6
.L2575:
	ldr	r1, .L2597+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2597+40
	add	r5, r5, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2597+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r10, #1]!
	ldr	r3, [r6]
	cmp	r3, r5
	bcs	.L2575
	ldr	r2, .L2597+48
	mov	r1, #5
	mov	r0, r4
	str	r6, [fp, #-48]
	bl	mvc_u_v
	movw	r3, #34396
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2597+52
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	add	r3, r8, #294912
	ldr	r2, .L2597+56
	mov	r1, #5
	str	r0, [r3, #1632]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34404
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2597+60
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34408
	movt	r3, 4
	add	r3, r8, r3
	str	r0, [r3]
.L2572:
	ldr	r1, .L2597+64
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-56]
	uxtb	r0, r0
	strb	r0, [r3]
	cmp	r0, #0
	bne	.L2596
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2580
.L2579:
	ldr	r1, .L2597+68
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
	bne	.L2581
.L2582:
	mov	r0, #0
.L2592:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2596:
	ldr	r1, .L2597+24
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r8, #315392
	ldr	r2, .L2597+28
	mov	r1, #4
	mov	r5, r3
	str	r3, [fp, #-108]
	str	r0, [r9]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2597+32
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
	bhi	.L2574
	ldrb	r5, [r4, #10]	@ zero_extendqisi2
	cmp	r5, #0
	bne	.L2574
	movw	r10, #54067
	ldr	r6, [fp, #-48]
	movt	r10, 4
	add	r10, r8, r10
	mov	r7, r9
.L2577:
	ldr	r1, .L2597+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2597+40
	add	r5, r5, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2597+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r10, #1]!
	ldr	r3, [r9]
	cmp	r3, r5
	bcs	.L2577
	ldr	r2, .L2597+48
	mov	r1, #5
	mov	r0, r4
	str	r6, [fp, #-48]
	bl	mvc_u_v
	movw	r3, #54364
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2597+52
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-108]
	ldr	r2, .L2597+56
	mov	r1, #5
	str	r0, [r3, #1120]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #54372
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2597+60
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
	bne	.L2580
	ldr	r3, [fp, #-56]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2579
.L2580:
	ldr	r1, .L2597+72
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-72]
	strb	r0, [r3]
	b	.L2579
.L2595:
	ldr	r2, .L2597+76
	mov	r1, #32
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-76]
	ldr	r2, .L2597+80
	mov	r1, #32
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-80]
	ldr	r1, .L2597+84
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-84]
	strb	r0, [r3]
	b	.L2570
.L2574:
	ldr	r1, .L2597+88
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2594:
	mov	r2, r0
	ldr	r1, .L2597+92
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2592
.L2598:
	.align	2
.L2597:
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
	ldr	r2, .L2627
	mov	r1, #8
	mov	r4, r0
	bl	mvc_u_v
	ldr	r1, .L2627+4
	mov	r7, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2627+8
	mov	r10, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2627+12
	str	r0, [fp, #-48]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2627+16
	str	r0, [fp, #-52]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2627+20
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2627+24
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r2, .L2627+28
	mov	r1, #2
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2627+32
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2627+36
	mov	r8, r0
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r5, r0
	bne	.L2612
	cmp	r0, #31
	bhi	.L2622
	sub	r3, r7, #66
	cmp	r3, #78
	ldrls	pc, [pc, r3, asl #2]
	b	.L2602
.L2604:
	.word	.L2603
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2605
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2606
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2605
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2607
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2605
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2607
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2605
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2602
	.word	.L2607
.L2603:
	ldr	r1, .L2627+40
	mov	r0, #1
	bl	dprint_vfmw
.L2605:
	cmp	r8, #41
	bhi	.L2623
.L2608:
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
	bne	.L2624
	cmp	r7, #118
	cmpne	r7, #128
	mov	r10, #1
	strb	r10, [r6, #957]
	beq	.L2625
.L2610:
	movw	r3, #8500
	mov	r2, #1
	movt	r3, 5
	mla	r4, r3, r5, r4
	add	r4, r4, #12992
	strb	r2, [r4, #12]
.L2600:
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2623:
	mov	r2, r8
	ldr	r1, .L2627+44
	mov	r0, #1
	mov	r8, #41
	bl	dprint_vfmw
	b	.L2608
.L2625:
	ldr	r1, .L2627+48
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
	bne	.L2626
	ldr	r1, .L2627+52
	mov	r0, r4
	bl	mvc_u_1
	cmp	r0, #0
	str	r0, [r6, #928]
	beq	.L2610
	mov	r1, r7
	mov	r0, r4
	bl	MVC_ProcessSUBSPSMvcVuiExt
	cmp	r0, #0
	beq	.L2610
	mov	r0, r10
	ldr	r1, .L2627+56
	bl	dprint_vfmw
	strb	r8, [r9, #4]
	mvn	r8, #0
	b	.L2600
.L2607:
	ldr	r1, .L2627+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2605
.L2606:
	ldr	r1, .L2627+64
	mov	r0, #1
	bl	dprint_vfmw
	b	.L2605
.L2602:
	mov	r2, r7
	ldr	r1, .L2627+68
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2612:
	mvn	r8, #0
	b	.L2600
.L2622:
	ldr	r1, .L2627+72
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L2600
.L2624:
	ldr	r1, .L2627+76
	mov	r0, #1
	bl	dprint_vfmw
	add	ip, r9, #12992
	mov	r3, #0
	mvn	r8, #0
	strb	r3, [ip, #12]
	strb	r3, [r6, #957]
	b	.L2600
.L2626:
	mov	r0, r10
	ldr	r1, .L2627+80
	bl	dprint_vfmw
	strb	r8, [r9, #4]
	mvn	r8, #0
	b	.L2600
.L2628:
	.align	2
.L2627:
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
	beq	.L2638
	ldrb	r3, [ip]	@ zero_extendqisi2
	mov	r2, r3, asl #2
	mov	r5, r3, asl #5
	rsb	r4, r2, r5
	add	r4, ip, r4
	add	r6, r4, #8
	ldr	lr, [r4, #8]
	cmp	lr, #0
	beq	.L2638
	ldr	lr, [ip, #68]
	cmp	r3, #1
	cmpls	lr, #2
	bhi	.L2638
	cmp	lr, r3
	bls	.L2638
	ldr	lr, [r4, #12]
	mov	r6, r2
	ldr	r4, [r4, #24]
	mov	lr, lr, asl #3
	add	r1, r4, r1, lsl #3
	cmp	r1, lr
	bhi	.L2633
	b	.L2631
.L2634:
	ldr	r4, [r2, #24]
	ldr	r2, [r2, #12]
	rsb	lr, lr, r4
	add	r1, r1, lr
	mov	lr, r2, asl #3
	cmp	r1, lr
	bls	.L2631
.L2633:
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
	bhi	.L2634
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2631:
	rsb	r2, r6, r5
	mov	r0, #1
	add	ip, ip, r2
	str	r1, [ip, #24]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2638:
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
	beq	.L2654
	ldr	r3, [r0, #232]
	cmp	r3, #0
	cmpne	r2, #0
	beq	.L2654
	ldrb	r7, [r3]	@ zero_extendqisi2
	mov	ip, r7, asl #5
	sub	ip, ip, r7, asl #2
	add	ip, r3, ip
	ldr	ip, [ip, #8]
	cmp	ip, #0
	beq	.L2654
	ldr	ip, [r3, #68]
	cmp	r7, #1
	cmpls	ip, #2
	movhi	lr, #1
	movls	lr, #0
	bhi	.L2654
	cmp	ip, r7
	bls	.L2654
.L2649:
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
	bcs	.L2644
	ldrb	r3, [r8, r4, lsr #3]	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	r2, lr
	add	r4, r1, #1
	add	ip, ip, #1
	strb	r3, [r1]
	b	.L2656
.L2647:
	cmp	ip, r6
	mov	r1, r4
	beq	.L2644
	ldrb	r3, [ip], #1	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	r2, lr
	strb	r3, [r4], #1
.L2656:
	ldr	r3, [r0, #232]
	add	r3, r3, r5
	ldr	r1, [r3, #24]
	add	r1, r1, #8
	str	r1, [r3, #24]
	bhi	.L2647
	mov	r0, r2
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2644:
	ldr	ip, [r0, #232]
	add	r7, r7, #1
	ldrb	r3, [ip]	@ zero_extendqisi2
	add	r3, r3, #1
	strb	r3, [ip]
	ldr	r3, [r0, #232]
	ldr	ip, [r3, #68]
	cmp	ip, r7
	bhi	.L2649
	mov	r0, lr
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2654:
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
	ldr	r1, .L2664
	mov	r4, r0
	bl	mvc_ue_v
	add	r6, r4, #11075584
	add	r5, r6, #45056
	ldr	r2, .L2664+4
	mov	r1, #1
	str	r0, [r5, #2640]
	mov	r0, r4
	bl	mvc_u_v
	uxtb	r0, r0
	strb	r0, [r5, #2617]
	cmp	r0, #0
	beq	.L2663
.L2658:
	mov	r0, r4
	ldr	r2, .L2664+8
	mov	r1, #1
	add	r6, r6, #45056
	bl	mvc_u_v
	mov	r3, #1
	strb	r3, [r6, #2616]
	strb	r0, [r6, #2618]
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2663:
	ldr	r2, .L2664+12
	mov	r1, #7
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+16
	mov	r1, #1
	str	r0, [r5, #2632]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+20
	mov	r1, #6
	strb	r0, [r5, #2619]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+24
	mov	r1, #1
	str	r0, [r5, #2636]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+28
	mov	r1, #1
	strb	r0, [r5, #2620]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+32
	mov	r1, #1
	strb	r0, [r5, #2621]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+36
	mov	r1, #1
	strb	r0, [r5, #2622]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+40
	mov	r1, #1
	strb	r0, [r5, #2623]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+44
	mov	r1, #1
	strb	r0, [r5, #2624]
	mov	r0, r4
	bl	mvc_u_v
	ldrb	r3, [r5, #2619]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r5, #2625]
	bne	.L2659
	ldr	r3, [r5, #2632]
	cmp	r3, #5
	beq	.L2659
	ldr	r2, .L2664+48
	mov	r1, #1
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+52
	mov	r1, #1
	strb	r0, [r5, #2626]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+56
	mov	r1, #1
	strb	r0, [r5, #2627]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2664+60
	mov	r1, #1
	strb	r0, [r5, #2628]
	mov	r0, r4
	bl	mvc_u_v
	strb	r0, [r5, #2629]
.L2659:
	ldr	r2, .L2664+64
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2664+68
	str	r0, [r5, #2644]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #2648]
	b	.L2658
.L2665:
	.align	2
.L2664:
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
	bne	.L2667
	ldr	ip, [r0, #36]
	cmp	ip, #0
	ble	.L2668
	ldrb	r2, [r3, #25]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #3984
	addeq	r3, r3, #8
	beq	.L2671
	b	.L2690
.L2672:
	ldrb	r1, [r3, #-3967]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2669
.L2671:
	add	r2, r2, #1
	mov	r4, r3
	cmp	r2, ip
	add	r3, r3, #3984
	add	r3, r3, #8
	bne	.L2672
.L2668:
	ldr	r1, .L2692
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r3, #0
.L2673:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2690:
	mov	r4, r3
.L2669:
	cmp	r4, #0
	beq	.L2668
.L2667:
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2674
	ldrb	r3, [r4, #35]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2691
	ldr	r1, [r4, #388]
	ldr	r8, [r4, #392]
	add	r1, r1, #1
	add	r8, r8, #1
.L2677:
	ldr	r2, .L2692+4
	mov	r0, r5
	bl	mvc_u_v
	add	r7, r6, #45056
	mov	r1, r8
	ldr	r2, .L2692+8
	str	r0, [r7, #2656]
	mov	r0, r5
	bl	mvc_u_v
	str	r0, [r7, #2660]
.L2674:
	ldrb	r0, [r4, #39]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	r3, r0
	beq	.L2673
	mov	r0, r5
	ldr	r2, .L2692+12
	mov	r1, #4
	add	r6, r6, #45056
	bl	mvc_u_v
	mov	r3, #0
	strb	r0, [r6, #2652]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2691:
	ldrb	r3, [r4, #38]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2674
	ldr	r1, [r4, #700]
	ldr	r8, [r4, #704]
	add	r1, r1, #1
	add	r8, r8, #1
	b	.L2677
.L2693:
	.align	2
.L2692:
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
	b	.L2697
.L2879:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2696
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2878
.L2697:
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
	bls	.L2879
.L2696:
	ldr	r1, .L2910
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2699
.L2878:
	mov	r3, #0
	str	r3, [fp, #-56]
.L2833:
	mov	r5, #0
	b	.L2703
.L2881:
	bl	BsSkip
	cmp	r9, #0
	ble	.L2701
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2880
.L2703:
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
	bls	.L2881
.L2701:
	ldr	r1, .L2910+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2880:
	cmp	r5, #409600
	bgt	.L2701
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
	b	.L2706
.L2708:
	.word	.L2707
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
	.word	.L2722
	.word	.L2723
	.word	.L2724
	.word	.L2725
	.word	.L2726
	.word	.L2727
	.word	.L2728
	.word	.L2729
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2706
	.word	.L2730
.L2730:
	mov	r1, r5
	mov	r0, r4
	ldr	r8, [r4, #572]
	bl	MVC_DecFramePackingSEI
	ldr	r2, [r4, #572]
	mov	r9, r5, asl #3
	rsb	r3, r8, r2
	cmp	r3, r9
	bge	.L2819
	rsb	r9, r3, r9
	ldr	r3, [r4, #564]
	cmp	r9, #0
	add	r8, r9, #7
	movge	r8, r9
	mov	r8, r8, asr #3
	cmp	r8, #0
	movle	r3, r3, asl #3
	ble	.L2821
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bhi	.L2821
	mov	r10, #0
	b	.L2822
.L2823:
	add	r1, r2, #8
	cmp	r1, r3
	bhi	.L2821
.L2822:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #564]
	cmp	r10, r8
	ldr	r2, [r4, #572]
	mov	r3, r3, asl #3
	bne	.L2823
.L2821:
	mov	r1, r9, asr #31
	mov	r0, r1, lsr #29
	add	r1, r9, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r2, r1
	cmp	r3, r2
	bcs	.L2882
.L2819:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2883
.L2732:
	ldr	r3, [r4, #60]
	cmp	r3, #3
	bhi	.L2884
.L2828:
	mov	r8, #0
	b	.L2832
.L2830:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2831
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2885
.L2832:
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
	bls	.L2830
.L2831:
	mov	r0, #0
.L2699:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2729:
	mov	r3, r5
	ldr	r1, .L2910+8
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2886
.L2815:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2816
.L2817:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2816:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2817
	ldr	r3, [r4, #60]
	cmp	r3, #3
	bls	.L2828
.L2884:
	ldr	r1, .L2910+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2728:
	mov	r3, r5
	ldr	r1, .L2910+16
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2887
.L2812:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2813
.L2814:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2813:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2814
	b	.L2732
.L2727:
	mov	r3, r5
	ldr	r1, .L2910+20
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2888
.L2809:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2810
.L2811:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2810:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2811
	b	.L2732
.L2725:
	mov	r3, r5
	ldr	r1, .L2910+24
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2889
.L2803:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2804
.L2805:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2804:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2805
	b	.L2732
.L2726:
	mov	r3, r5
	ldr	r1, .L2910+28
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2890
.L2806:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2807
.L2808:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2807:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2808
	b	.L2732
.L2724:
	mov	r3, r5
	ldr	r1, .L2910+32
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2891
.L2800:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2801
.L2802:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2801:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2802
	b	.L2732
.L2723:
	mov	r3, r5
	ldr	r1, .L2910+36
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2892
.L2797:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2798
.L2799:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2798:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2799
	b	.L2732
.L2715:
	mov	r3, r5
	ldr	r1, .L2910+40
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2893
.L2773:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2774
.L2775:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2774:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2775
	b	.L2732
.L2719:
	mov	r3, r5
	ldr	r1, .L2910+44
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2894
.L2785:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2786
.L2787:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2786:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2787
	b	.L2732
.L2710:
	mov	r3, r5
	ldr	r1, .L2910+48
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2895
.L2742:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2743
.L2744:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2743:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2744
	b	.L2732
.L2721:
	mov	r3, r5
	ldr	r1, .L2910+52
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2896
.L2791:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2792
.L2793:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2792:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2793
	b	.L2732
.L2713:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	bne	.L2712
	mov	r3, r5
	ldr	r1, .L2910+56
	mov	r0, #20
	bl	dprint_vfmw
.L2749:
	ldr	r0, [r4, #120]
	ldr	r8, [r4, #60]
	bl	GetUsd
	add	r8, r8, #132
	str	r0, [r4, r8, asl #2]
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	r1, [r4, r3, asl #2]
	cmp	r1, #0
	beq	.L2836
	ldr	r2, [r4, #232]
	cmp	r5, #1024
	movlt	r10, r5
	movge	r10, #1024
	ldrb	r0, [r2]	@ zero_extendqisi2
	ldr	ip, [r2, #68]
	cmp	r0, ip
	bcs	.L2837
	sxth	r3, r0
	mov	r8, #0
	mov	lr, r3, asl #5
	sub	r3, lr, r3, asl #2
	add	r2, r2, r3
.L2751:
	ldr	r3, [r2, #24]
	add	r0, r0, #1
	ldr	lr, [r2, #12]
	cmp	r0, ip
	add	r3, r3, #7
	add	r2, r2, #28
	sub	r3, lr, r3, lsr #3
	add	r8, r8, r3
	bne	.L2751
	mov	r2, r8
.L2750:
	ldrsb	r3, [fp, #-56]
	cmp	r2, r10
	movge	r8, r10
	cmp	r3, #1
	movne	r3, #0
	movne	r9, r3
	beq	.L2897
.L2753:
	rsb	r2, r3, r8
	mov	r0, r4
	bl	MVC_GetBytes
	subs	r8, r0, #0
	ble	.L2898
	rsb	r3, r9, r10
	cmp	r8, r3
	ldr	r3, [r4, #60]
	blt	.L2899
	add	r3, r3, #132
	cmp	r10, r5
	rsb	r9, r9, r5
	ldr	r3, [r4, r3, asl #2]
	str	r9, [r3, #1052]
	bcc	.L2900
.L2764:
	cmp	r9, #0
	ble	.L2759
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2759
	mov	r5, #0
	b	.L2766
.L2767:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2759
.L2766:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2767
.L2759:
	ldr	r3, [r4, #60]
	mov	r2, #8
	ldr	r1, [r4, #224]
	add	r3, r3, #132
	ldr	ip, .L2910+60
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
	beq	.L2732
	ldr	r2, [r4, #60]
	movw	r3, #1064
	mov	r1, #6
	ldr	r0, [r4, #120]
	add	r2, r2, #132
	ldr	r2, [r4, r2, asl #2]
	blx	ip
	b	.L2732
.L2717:
	mov	r3, r5
	ldr	r1, .L2910+64
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2901
.L2779:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2780
.L2781:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2780:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2781
	b	.L2732
.L2716:
	mov	r3, r5
	ldr	r1, .L2910+68
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2902
.L2776:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2777
.L2778:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2777:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2778
	b	.L2732
.L2718:
	mov	r3, r5
	ldr	r1, .L2910+72
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2903
.L2782:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2783
.L2784:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2783:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2784
	b	.L2732
.L2707:
	mov	r3, r5
	ldr	r1, .L2910+76
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2904
.L2731:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2733
.L2734:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2733:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2734
	b	.L2732
.L2722:
	mov	r3, r5
	ldr	r1, .L2910+80
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2905
.L2794:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2795
.L2796:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2795:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2796
	b	.L2732
.L2714:
	mov	r3, r5
	ldr	r1, .L2910+84
	mov	r0, #20
	bl	dprint_vfmw
	cmp	r5, #0
	ble	.L2770
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2770
	mov	r8, #0
	b	.L2771
.L2772:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2770
.L2771:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2772
.L2770:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2732
	mov	r3, #12416
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2732
.L2720:
	mov	r3, r5
	ldr	r1, .L2910+88
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2906
.L2788:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2789
.L2790:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2789:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2790
	b	.L2732
.L2709:
	mov	r3, r5
	ldr	r1, .L2910+92
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
	bge	.L2736
	rsb	r9, r3, r9
	ldr	r3, [r4, #564]
	cmp	r9, #0
	add	r8, r9, #7
	movge	r8, r9
	mov	r8, r8, asr #3
	cmp	r8, #0
	movle	r3, r3, asl #3
	ble	.L2738
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bhi	.L2738
	mov	r10, #0
	b	.L2739
.L2740:
	add	r1, r2, #8
	cmp	r1, r3
	bhi	.L2738
.L2739:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r10, r10, #1
	ldr	r3, [r4, #564]
	cmp	r10, r8
	ldr	r2, [r4, #572]
	mov	r3, r3, asl #3
	bne	.L2740
.L2738:
	mov	r1, r9, asr #31
	mov	r0, r1, lsr #29
	add	r1, r9, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r2, r1
	cmp	r3, r2
	bcs	.L2907
.L2736:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2732
	movw	r3, #12190
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2732
.L2711:
	mov	r3, r5
	ldr	r1, .L2910+96
	mov	r0, #20
	bl	dprint_vfmw
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2908
.L2745:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2746
.L2747:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2746:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2747
	b	.L2732
.L2706:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2909
.L2825:
	cmp	r5, #0
	ble	.L2732
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
	mov	r8, #0
	b	.L2826
.L2827:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2732
.L2826:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2827
	b	.L2732
.L2885:
	cmp	r3, #128
	bne	.L2833
	b	.L2831
.L2712:
	mov	r3, r5
	ldr	r1, .L2910+100
	mov	r0, #20
	bl	dprint_vfmw
	mov	r3, #1
	str	r3, [fp, #-56]
	b	.L2749
.L2899:
	add	r3, r3, #132
	ldr	r3, [r4, r3, asl #2]
	str	r8, [r3, #1052]
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2759
	mov	r5, #0
	b	.L2760
.L2762:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bhi	.L2759
.L2760:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2762
	b	.L2759
.L2911:
	.align	2
.L2910:
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
.L2897:
	strb	r3, [r1, #1026]
	mov	r0, r4
	ldr	r3, [r4, #60]
	mov	r1, #8
	ldr	r2, .L2910+104
	add	r3, r3, #132
	ldr	r9, [r4, r3, asl #2]
	bl	mvc_u_v
	strb	r0, [r9, #1027]
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	ip, [r4, r3, asl #2]
	ldrb	r3, [ip, #1027]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L2754
	mov	r3, #3
	mov	r2, #0
	mov	r9, r3
	strb	r2, [ip, #1028]
.L2755:
	ldr	ip, [r4, #60]
	mov	r1, #16
	ldr	r2, .L2910+108
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
	beq	.L2759
	ldr	r2, [r4, #60]
	add	r2, r2, #132
	ldr	r1, [r4, r2, asl #2]
	b	.L2753
.L2892:
	movw	r3, #12636
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2797
.L2893:
	movw	r3, #12428
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2773
.L2894:
	movw	r3, #12532
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2785
.L2895:
	movw	r3, #12202
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2742
.L2896:
	movw	r3, #12584
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2791
.L2888:
	movw	r3, #12740
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2809
.L2889:
	movw	r3, #12688
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2803
.L2890:
	movw	r3, #12714
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2806
.L2891:
	movw	r3, #12662
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2800
.L2886:
	movw	r3, #12792
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2815
.L2887:
	movw	r3, #12766
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2812
.L2902:
	movw	r3, #12454
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2776
.L2903:
	movw	r3, #12506
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2782
.L2904:
	movw	r3, #12137
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2731
.L2905:
	movw	r3, #12610
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2794
.L2901:
	mov	r3, #12480
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2779
.L2906:
	movw	r3, #12558
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2788
.L2908:
	movw	r3, #12228
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2745
.L2883:
	movw	r3, #12844
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2732
.L2909:
	movw	r3, #12856
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2825
.L2907:
	mov	r0, r6
	bl	BsSkip
	b	.L2736
.L2882:
	mov	r0, r6
	bl	BsSkip
	b	.L2819
.L2900:
	rsb	r1, r10, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2764
	movw	r3, #12364
	ldr	r2, .L2910+112
	ldr	r1, .L2910+116
	mov	r0, #20
	bl	dprint_vfmw
	b	.L2764
.L2837:
	mov	r2, #0
	mov	r8, r2
	b	.L2750
.L2754:
	ldr	r2, .L2910+120
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
	b	.L2755
.L2836:
	mvn	r0, #0
	b	.L2699
.L2898:
	ldr	r1, .L2910+124
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
	b	.L2699
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
	b	.L2921
.L2916:
	.word	.L2915
	.word	.L2921
	.word	.L2921
	.word	.L2921
	.word	.L2915
	.word	.L2921
	.word	.L2917
	.word	.L2917
.L2917:
	ldr	r3, [r0]
	mov	r2, #1
	str	r2, [r0]
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2915:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L2918
.L2920:
	mvn	r0, #0
.L2919:
	mov	r2, #0
	str	r2, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2921:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2918:
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
	bhi	.L2920
	mov	r3, r0, asl #1
	rsb	r3, r3, #31
	mov	r3, r4, lsr r3
	subs	r0, r3, #1
	mvnne	r0, #0
	b	.L2919
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
	ble	.L2948
	ldr	r3, [fp, #-48]
	subs	r7, r3, #0
	add	r8, r3, #63
	movge	r8, r3
	ands	r3, r3, #63
	movne	r3, #1
	add	r8, r3, r8, asr #6
	cmp	r8, #0
	ble	.L2936
	ldr	r5, .L2949
	ldr	r10, .L2949+4
.L2934:
	cmp	r7, #64
	ldr	r3, [r10, #52]
	ldr	r0, .L2949
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
	bne	.L2926
	ldr	r2, .L2949
	add	r4, r4, #1
	cmp	r3, r2
	bne	.L2929
	b	.L2927
.L2930:
	cmp	r3, r5
	add	r4, r4, #1
	beq	.L2927
.L2929:
	ldrb	r2, [r3, #-1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2930
.L2926:
	mov	r0, r4, asl #3
.L2925:
	ldr	r3, [fp, #-48]
	cmp	r4, r3
	bge	.L2938
	sub	r3, r3, #1
	rsb	r4, r4, r3
	ldrb	r2, [r9, r4]	@ zero_extendqisi2
	tst	r2, #1
	moveq	r2, r2, lsr #1
	moveq	r3, #1
	bne	.L2938
.L2933:
	tst	r2, #1
	mov	r2, r2, lsr #1
	bne	.L2931
	add	r3, r3, #1
	cmp	r3, #8
	bne	.L2933
	b	.L2931
.L2927:
	subs	r8, r8, #1
	sub	r7, r7, #64
	bne	.L2934
	b	.L2926
.L2938:
	mov	r3, #0
.L2931:
	add	r0, r3, r0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2948:
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L2949+8
	bl	dprint_vfmw
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2936:
	mov	r0, r4
	b	.L2925
.L2950:
	.align	2
.L2949:
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
	beq	.L2957
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2956
	ldr	r1, [r3, #40]
	ldr	r0, [r3, #36]
	bl	MVC_FindZeroBitsInSeg
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #40]
	cmp	r0, r2, asl #3
	bcs	.L2960
	cmn	r0, #1
	beq	.L2956
.L2955:
	add	r0, r0, #1
	str	r0, [r3, #72]
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2959
	ldr	r1, [r3, #40]
	ldr	r2, [r3, #72]
	cmp	r2, r1, asl #3
	bcs	.L2961
.L2959:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2961:
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
.L2960:
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
.L2956:
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	bl	MVC_FindZeroBitsInSeg
	ldr	r3, [r4, #232]
	b	.L2955
.L2957:
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
	beq	.L2969
	mov	r2, r0
	mov	r3, #1
	b	.L2967
.L2966:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L2972
.L2967:
	ldrb	r1, [r2, #1025]	@ zero_extendqisi2
	add	r2, r2, #88
	cmp	r1, #0
	bne	.L2966
	mov	r2, r3
.L2965:
	mov	ip, #88
	mov	r1, #1
	mla	r3, ip, r3, r0
	mov	r0, r2
	strb	r1, [r3, #937]
	ldmfd	sp, {fp, sp, pc}
.L2972:
	mvn	r2, #0
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L2969:
	mov	r3, r2
	b	.L2965
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
	ldr	r1, .L3016
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
	beq	.L3009
	cmp	r2, #255
	beq	.L3010
.L2975:
	mov	r0, r4
	bl	MVC_SliceCheck
	subs	r8, r0, #0
	bne	.L2991
	mov	r0, r4
	bl	MVC_ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L3011
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
	beq	.L3012
	cmp	r1, #0
	bne	.L2987
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	ldr	r3, [r3, #28]
	cmp	r2, #1
	beq	.L3013
	ldr	r2, [r4, #28]
	cmp	r2, r3
	movweq	r3, #35364
	movteq	r3, 168
	addeq	r3, r4, r3
	bne	.L3014
.L2986:
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
.L2983:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3009:
	strb	r3, [r5, #1011]
	ldr	r3, [r1, #712]
	str	r3, [r5, #2640]
	ldrb	r3, [r1, #708]	@ zero_extendqisi2
	strb	r3, [r5, #1009]
	ldrb	r3, [r1, #709]	@ zero_extendqisi2
	strb	r3, [r5, #1010]
	b	.L2975
.L3012:
	ldr	r3, [r3, #28]
	movw	r1, #3992
	ldr	r2, [r4, #248]
	mla	r3, r1, r3, r2
	b	.L2986
.L3010:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq	.L2975
	add	r3, r4, #10747904
	add	r3, r3, #20480
	ldr	r2, [r3, #2384]
	cmp	r2, #0
	bne	.L2978
	ldrb	r2, [r3, #2380]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2978
	add	r3, r4, #12992
	add	r3, r3, #16
.L2981:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L2979
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3015
.L2979:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L2981
	mvn	r3, #0
	b	.L3008
.L3013:
	movw	r2, #8500
	movt	r2, 5
	mla	r3, r2, r3, r4
	add	r3, r3, #286720
	add	r3, r3, #932
	b	.L2986
.L2978:
	ldr	r3, [r3, #2388]
.L3008:
	cmn	r3, #1
	str	r3, [r5, #2640]
	beq	.L2975
	sub	r0, r0, #5
	mov	r3, #1
	clz	r0, r0
	strb	r3, [r5, #1011]
	strb	r3, [r5, #1010]
	mov	r0, r0, lsr #5
	strb	r0, [r5, #1009]
	b	.L2975
.L2991:
	mvn	r8, #0
	b	.L2983
.L3014:
	ldr	r1, .L3016+4
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L2983
.L3011:
	ldr	r1, .L3016+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L2983
.L2987:
	ldr	r1, .L3016+12
	mov	r0, #1
	mvn	r8, #0
	bl	dprint_vfmw
	b	.L2983
.L3015:
	movw	r3, #8500
	movt	r3, 5
	mla	r3, r3, r2, r4
	add	r3, r3, #12992
	add	r3, r3, #16
	ldr	r3, [r3, #4]
	b	.L3008
.L3017:
	.align	2
.L3016:
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
	beq	.L3020
	add	r6, r0, #252
	mov	r4, #0
	b	.L3024
.L3021:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L3023
	ldr	r3, [r5, #1048]
	cmp	r4, r3
	bcs	.L3046
.L3024:
	ldr	r3, [r6, #4]!
	add	r4, r4, #1
	ldr	r0, [r7, #120]
	ldr	r3, [r3, #4]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L3021
.L3023:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3046:
	cmp	r3, #0
	beq	.L3020
	ldr	r3, [r5, #1052]
	cmp	r3, #0
	addne	r6, r7, #384
	movne	r4, #0
	bne	.L3027
	b	.L3020
.L3047:
	ldr	r3, [r0, #640]
	cmp	r3, #0
	beq	.L3023
	ldr	r3, [r5, #1052]
	cmp	r4, r3
	bcs	.L3020
.L3027:
	ldr	r3, [r6, #4]!
	add	r4, r4, #1
	ldr	r0, [r7, #120]
	ldr	r3, [r3, #4]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L3047
	b	.L3023
.L3020:
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
	beq	.L3049
	mov	r4, #0
	add	r6, r5, #524
	mov	r7, r4
.L3051:
	ldr	r1, [r6, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3050
	ldr	r0, [r5, #120]
	bl	FreeUsdByDec
	str	r7, [r6]
.L3050:
	ldr	r3, [r5, #60]
	cmp	r3, r4
	bhi	.L3051
.L3049:
	movw	r9, #48936
	movw	r8, #15720
	movt	r9, 169
	movt	r8, 170
	add	r9, r5, r9
	add	r8, r5, r8
	mov	r6, #0
	mov	r7, #0
.L3055:
	sub	r4, r9, #32
.L3053:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3067
.L3052:
	cmp	r4, r9
	bne	.L3053
	add	r9, r4, #808
	cmp	r9, r8
	bne	.L3055
	ldr	r1, .L3068
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L3067:
	mov	r1, r2
	ldr	r0, [r5, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L3052
.L3069:
	.align	2
.L3068:
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
	ldr	r1, .L3081
	bl	dprint_vfmw
	mov	r1, r8
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L3075
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	beq	.L3075
	add	r4, r0, #560
	add	r9, r0, #592
	mov	r6, #0
	mov	r7, #0
.L3073:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3080
.L3072:
	cmp	r4, r9
	bne	.L3073
	ldr	r0, [r5, #120]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3080:
	mov	r1, r2
	ldr	r0, [r5, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L3072
.L3075:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3082:
	.align	2
.L3081:
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
	bne	.L3117
	ldr	r3, [r0, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ble	.L3086
	ldrb	r3, [r0, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3086
	ldr	r3, [r0, #520]
	cmp	r3, #0
	strneb	ip, [r3, #2]
	movne	r0, #1
	strne	ip, [r4, #520]
	beq	.L3088
.L3116:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3086:
	mov	r1, r6
	mov	r0, r4
	bl	MVC_CheckFrameStore
	cmn	r0, #3
	beq	.L3088
	ldr	r1, [r4, #520]
	cmp	r1, #0
	beq	.L3090
	mov	r0, r4
	bl	MVC_GetImagePara
	ldr	r1, [r4, #520]
	mov	r0, r4
	bl	MVC_CheckFrameStore
	subs	r8, r0, #0
	beq	.L3118
	cmn	r8, #3
	beq	.L3098
	mov	r3, r8
	movw	r2, #1910
	ldr	r1, .L3126
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r4, #520]
	mov	r0, r4
	sub	r8, r8, #1
	clz	r8, r8
	ldr	r1, [r3, #228]
	bl	MVC_DEC_RecycleImage
	mov	r8, r8, lsr #5
.L3097:
	ldr	r3, [r4, #520]
	tst	r8, r5
	mov	r0, #0
	strb	r0, [r3, #2]
	str	r0, [r4, #520]
	bne	.L3116
.L3090:
	mov	r1, r6
	mov	r0, r4
	bl	MVC_GetImagePara
	mov	r1, r6
	mov	r0, r4
	bl	MVC_CheckFrameStore
	subs	r3, r0, #0
	beq	.L3119
	movw	r2, #1959
	ldr	r1, .L3126
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r6, #228]
	mov	r0, r4
	bl	MVC_DEC_RecycleImage
	mov	r0, #2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3117:
	mov	r3, r1
	mov	r2, r0
	ldr	r1, .L3126+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3088:
	mov	r0, #1
	b	.L3116
.L3119:
	mov	r2, #1
	ldrsb	r1, [r6, #6]
	ldr	r0, [r4, #120]
	bl	FSP_SetDisplay
	ldrsb	r1, [r6, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L3120
	cmp	r7, #1
	add	r3, r4, #584
	streq	r7, [r5, #188]
	mov	r2, r4
	ldr	r0, [r4, #120]
	mov	r1, #15
	str	r5, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L3121
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r5, #24]
	bcs	.L3122
.L3105:
	ldr	r2, [r4, #144]
	mov	r0, #0
	ldr	r3, [r4, #136]
	add	r2, r2, #1
	str	r2, [r4, #144]
	add	r3, r3, #2
	str	r3, [r4, #136]
	b	.L3116
.L3121:
	mov	r2, r0
	ldr	r1, .L3126+8
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
	b	.L3116
.L3098:
	ldr	r2, [r4, #520]
	mov	r3, #0
	strb	r3, [r2, #2]
	str	r3, [r4, #520]
	b	.L3090
.L3118:
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
	beq	.L3123
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
	bne	.L3124
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r8, #24]
	bcs	.L3125
.L3096:
	ldr	r2, [r4, #144]
	mov	r8, #1
	ldr	r3, [r4, #136]
	add	r2, r2, r8
	str	r2, [r4, #144]
	add	r3, r3, #2
	str	r3, [r4, #136]
	b	.L3097
.L3122:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3105
.L3125:
	mov	r1, r8
	mov	r0, r4
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3096
.L3124:
	mov	r2, r0
	ldr	r1, .L3126+8
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
	b	.L3116
.L3120:
	movw	r2, #1933
	ldr	r1, .L3126+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3116
.L3123:
	movw	r2, #1881
	ldr	r1, .L3126+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3116
.L3127:
	.align	2
.L3126:
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
	beq	.L3129
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
	beq	.L3131
.L3129:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3131:
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
	beq	.L3151
	add	r8, r5, #11075584
	add	r8, r8, #45056
	ldr	r3, [r8, #2856]
	cmp	r3, #0
	movwne	r7, #47716
	movne	r6, #0
	movtne	r7, 169
	addne	r7, r5, r7
	beq	.L3139
.L3138:
	ldr	r1, [r7, #4]!
	add	r6, r6, #1
	cmp	r1, #0
	beq	.L3137
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3137
	ldr	r3, [r1, #56]
	cmn	r4, #1
	cmpne	r3, r4
	bne	.L3137
	mov	r0, r5
	bl	MVC_UnMarkFrameStoreRef
.L3137:
	ldr	r3, [r8, #2856]
	cmp	r3, r6
	bhi	.L3138
.L3139:
	mov	r1, r4
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
	ldr	r0, [r8, #2856]
	cmp	r0, #0
	beq	.L3136
	movw	r1, #47716
	mov	r7, #0
	movt	r1, 169
	add	r1, r5, r1
	mov	r2, r7
	mvn	r6, #0
.L3143:
	ldr	r3, [r1, #4]!
	add	r2, r2, #1
	cmp	r3, #0
	beq	.L3141
	ldr	r3, [r3, #56]
	cmn	r4, #1
	cmpne	r4, r3
	addeq	r7, r7, #1
	cmp	r4, r3
	cmnne	r4, #1
	movne	r6, r3
.L3141:
	cmp	r2, r0
	bne	.L3143
	cmp	r7, #0
	beq	.L3136
	adds	r8, r6, #1
	movne	r8, #1
	cmp	r4, r6
	movle	r9, #0
	andgt	r9, r8, #1
	b	.L3149
.L3150:
	subs	r7, r7, #1
	beq	.L3136
.L3149:
	cmp	r9, #0
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r1, r6
	mov	r0, r5
	beq	.L3146
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3136
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	bne	.L3151
.L3146:
	mov	r1, r4
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3136
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	bne	.L3151
	cmp	r4, r6
	movge	r3, #0
	andlt	r3, r8, #1
	cmp	r3, #0
	beq	.L3150
	mov	r1, r6
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3136
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	beq	.L3150
.L3151:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3136:
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
.L3198:
	ldr	r3, [r10]
	cmp	r3, #6
	ldrls	pc, [pc, r3, asl #2]
	b	.L3189
.L3191:
	.word	.L3190
	.word	.L3192
	.word	.L3193
	.word	.L3194
	.word	.L3195
	.word	.L3196
	.word	.L3197
.L3197:
	ldr	r2, [r10, #12]
	mov	r1, r6
	mov	r0, r4
	bl	MVC_MarkCurrPicLT
.L3189:
	add	r10, r10, #20
	cmp	r10, r5
	bne	.L3198
.L3190:
	ldrb	r3, [r7, #3960]	@ zero_extendqisi2
	cmp	r3, #1
	movne	r0, #0
	beq	.L3206
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3196:
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
	b	.L3189
.L3195:
	mov	r0, r4
	ldr	r1, [r10, #16]
	bl	MVC_UpdateMaxLTFrmIdx
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	b	.L3189
.L3194:
	ldr	r3, [r10, #12]
	mov	r1, r6
	ldr	r2, [r10, #4]
	mov	r0, r4
	bl	MVC_MarkSTToLTRef
	mov	r0, r4
	bl	MVC_UpdateReflist
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	b	.L3189
.L3193:
	mov	r0, r4
	ldr	r2, [r10, #8]
	mov	r1, r6
	bl	MVC_UnMarkLTRef
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	b	.L3189
.L3192:
	mov	r0, r4
	ldr	r2, [r10, #4]
	mov	r1, r6
	bl	MVC_UnMarkSTRef
	mov	r0, r4
	bl	MVC_UpdateReflist
	b	.L3189
.L3206:
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
	beq	.L3201
	bcc	.L3202
	cmp	r3, #2
	streq	r2, [r8, #3448]
	streq	r2, [r8, #3436]
	streq	r2, [r7, #3988]
	streq	r2, [r7, #3996]
.L3200:
	ldr	r1, [r8, #3496]
	mov	r0, r4
	bl	MVC_FlushDPB
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3202:
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
	b	.L3200
.L3201:
	str	r2, [r8, #3444]
	str	r2, [r8, #3436]
	str	r2, [r7, #3984]
	str	r2, [r7, #3996]
	b	.L3200
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
	beq	.L3208
	ldr	lr, [r0, #52]
	cmp	lr, #0
	beq	.L3217
	movw	r1, #47716
	mov	r2, #0
	movt	r1, 169
	add	r0, r0, #148
	add	r1, r7, r1
	mov	r4, r2
.L3216:
	ldr	r3, [r1, #4]!
	add	r2, r2, #1
	cmp	r3, #0
	beq	.L3213
	ldr	ip, [r3, #16]
	cmp	ip, #1
	streqb	ip, [r7, #6]
	ldr	ip, [r3, #56]
	ldr	r3, [r6, #3496]
	cmp	ip, r3
	streq	r4, [r0]
.L3213:
	cmp	r2, lr
	add	r0, r0, #4
	bne	.L3216
.L3217:
	add	r9, r7, #11075584
	add	ip, r9, #45056
	ldr	r10, [ip, #2856]
	cmp	r10, #0
	beq	.L3211
	movw	r4, #47716
	mov	r5, #0
	movt	r4, 169
	mov	r8, r5
	add	r4, r7, r4
	b	.L3222
.L3220:
	cmp	r5, r10
	beq	.L3211
.L3222:
	ldr	r1, [r4, #4]!
	add	r5, r5, #1
	cmp	r1, #0
	beq	.L3220
	ldr	r0, [r1, #56]
	ldr	r2, [r6, #3496]
	cmp	r0, r2
	bne	.L3220
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
	beq	.L3221
	ldr	r1, [r4]
	ldr	r0, [r7, #120]
	ldrsb	r1, [r1, #6]
	bl	FSP_SetDisplay
	ldr	ip, [fp, #-48]
.L3221:
	str	r8, [r4]
	cmp	r5, r10
	str	r8, [r4, #64]
	str	r8, [r4, #128]
	ldr	r2, [ip, #2860]
	sub	r2, r2, #1
	str	r2, [ip, #2860]
	bne	.L3222
.L3211:
	mov	r0, r7
	bl	MVC_UpdateReflist
	mov	r0, r7
	bl	MVC_UpdateLTReflist
	ldr	r3, [r6, #3420]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3235
	add	r9, r9, #45056
	mov	r0, r3
	mov	r2, #1
	str	r3, [r9, #2872]
	strb	r3, [r6, #2780]
	strb	r2, [r6, #2781]
.L3223:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3235:
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
.L3208:
	ldr	r1, [r6, #3496]
	bl	MVC_FlushDPB
	cmp	r0, #0
	addeq	r9, r7, #11075584
	beq	.L3211
.L3224:
	mvn	r0, #0
	b	.L3223
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
	beq	.L3237
	ldrb	r3, [r4, #2788]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3238
	ldr	r3, [r0, #224]
	ldr	r2, [r3, #640]
	cmp	r2, #1
	beq	.L3259
.L3238:
	ldr	r3, [r4, #3420]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r4, #3432]
	beq	.L3260
	cmp	r3, #0
	bne	.L3261
.L3245:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3260:
	cmp	r3, #0
	beq	.L3245
.L3244:
	ldrb	r7, [r4, #2777]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L3262
.L3248:
	add	r5, r5, #12288
	mov	r0, #0
	ldrb	r3, [r5, #2780]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r5, #2781]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3259:
	ldr	r2, [r3, #636]
	cmp	r2, #0
	bne	.L3238
	ldr	r2, [r3, #644]
	cmp	r2, #0
	bne	.L3238
	ldr	r3, [r3, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	bls	.L3238
.L3237:
	mov	r0, r6
	bl	MVC_IDRMemMarking
	cmp	r0, #0
	bne	.L3239
.L3246:
	ldrb	r3, [r4, #2782]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3241
	ldr	r3, [r4, #3432]
	cmp	r3, #0
	beq	.L3245
	ldr	r3, [r4, #3420]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3244
	b	.L3248
.L3262:
	mov	r0, r6
	bl	MVC_SlidingWinMark
	ldr	r3, [r4, #3420]
	strb	r7, [r3]
	strb	r7, [r4, #2780]
.L3241:
	ldr	r3, [r4, #3432]
	cmp	r3, #0
	bne	.L3248
	b	.L3245
.L3261:
	mov	r0, r6
	bl	MVC_AdaptiveMemMark
	ldr	r3, [r4, #3420]
	mov	r2, #0
	strb	r2, [r3]
	cmp	r0, r2
	beq	.L3246
.L3239:
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
	beq	.L3265
	bcc	.L3266
	cmp	r6, #2
	beq	.L3267
	mov	r6, #0
.L3264:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3267:
	ldr	r3, [r5, #3416]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r2, [r5, #2777]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3285
	ldr	r1, [r0, #520]
	cmp	r1, #0
	beq	.L3286
	bl	MVC_OutputFrmToVO
	ldr	r3, [r5, #3416]
	str	r3, [r4, #520]
	str	r3, [r3, #772]
	mov	r6, r0
.L3287:
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
.L3344:
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
	bne	.L3264
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
	beq	.L3297
	ldr	r3, [r5, #3416]
	mov	r2, r8
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	b	.L3297
.L3266:
	ldr	r3, [r5, #3416]
	mov	r2, #0
	strb	r2, [r3, #696]
	ldr	r1, [r0, #520]
	cmp	r1, r2
	beq	.L3268
	bl	MVC_OutputFrmToVO
	cmp	r0, #0
	bne	.L3347
.L3269:
	ldr	r3, [r4, #520]
	cmp	r3, #0
	beq	.L3268
	ldr	r3, [r5, #3416]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
.L3268:
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
	beq	.L3297
.L3339:
	ldr	r3, [r5, #3416]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
.L3297:
	mov	r3, #0
	mov	r0, r6
	str	r3, [r4, #520]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3265:
	ldr	r3, [r5, #3416]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r2, [r5, #2777]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3273
	ldr	r1, [r0, #520]
	cmp	r1, #0
	beq	.L3274
	bl	MVC_OutputFrmToVO
	ldr	r3, [r5, #3416]
	str	r3, [r4, #520]
	str	r3, [r3, #736]
	mov	r6, r0
.L3275:
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
	b	.L3344
.L3285:
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
	beq	.L3345
.L3296:
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
	bne	.L3339
	b	.L3297
.L3273:
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
	bne	.L3296
.L3345:
	orr	r2, r2, ip
	b	.L3296
.L3347:
	str	r0, [sp]
	movw	r3, #2510
	ldr	r2, .L3348
	mov	r0, #22
	ldr	r1, .L3348+4
	bl	dprint_vfmw
	b	.L3269
.L3286:
	ldr	r3, [r5, #3416]
	mov	r6, r1
	str	r3, [r0, #520]
	str	r3, [r3, #772]
	b	.L3287
.L3274:
	ldr	r3, [r5, #3416]
	mov	r6, r1
	str	r3, [r0, #520]
	str	r3, [r3, #736]
	b	.L3275
.L3349:
	.align	2
.L3348:
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
	beq	.L3364
	movw	r8, #47716
	mov	r4, #0
	movt	r8, 169
	add	r8, r0, r8
	mov	r7, r4
	mov	r9, r8
	b	.L3353
.L3352:
	ldr	r3, [r6, #2856]
	cmp	r3, r7
	bls	.L3391
.L3353:
	ldr	r3, [r9, #4]!
	add	r7, r7, #1
	cmp	r3, #0
	beq	.L3352
	ldrb	r2, [r3, #5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3352
	ldrsb	r1, [r3, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L3352
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	sub	r3, r3, #1
	cmp	r3, #1
	ldr	r3, [r6, #2856]
	addls	r4, r4, #1
	cmp	r3, r7
	bhi	.L3353
.L3391:
	cmp	r4, #0
	beq	.L3354
	mvn	r7, #0
	b	.L3357
.L3355:
	ldr	r1, [r5, r3, asl #2]
	bl	MVC_OutputFrmToVO
	subs	r4, r4, #1
	mov	r7, r0
	beq	.L3389
.L3357:
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
	bne	.L3355
.L3389:
	ldr	r3, [r6, #2856]
	cmp	r3, #0
	beq	.L3351
.L3356:
	mov	r4, #0
	b	.L3360
.L3359:
	ldr	r3, [r6, #2856]
	cmp	r3, r4
	bls	.L3351
.L3360:
	ldr	r1, [r8, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3359
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3359
	mov	r0, r5
	bl	MVC_UnMarkFrameStoreRef
	ldr	r3, [r6, #2856]
	cmp	r3, r4
	bhi	.L3360
.L3351:
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
	cmp	r7, #0
	bne	.L3362
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3354:
	cmp	r3, #0
	mvnne	r7, #0
	bne	.L3356
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
.L3362:
	add	r0, r5, #584
	bl	GetVoLastImageID
	mov	r7, r0
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3364:
	mvn	r7, #0
	b	.L3351
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
	beq	.L3409
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
.L3397:
	ldr	r3, [r7]
	mov	r2, #1
	add	r8, r8, r2
	cmp	r3, #0
	beq	.L3394
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3394:
	ldr	r3, [r5]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3395
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3395:
	ldr	r3, [r6]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3396
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3396:
	cmp	r8, #16
	str	r9, [r6], #4
	mov	r2, #0
	str	r9, [r5], #4
	str	r9, [r7], #4
	bne	.L3397
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
	beq	.L3399
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3400
.L3404:
	add	r3, r4, #11141120
	add	r3, r3, #12288
	ldr	r3, [r3, #3416]
	cmp	r3, #0
	beq	.L3402
	ldrsb	r1, [r3, #6]
	mov	r2, #1
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3402:
	mov	r3, #0
	str	r3, [r4, #520]
.L3399:
	ldr	r3, .L3427
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
.L3406:
	str	r2, [r3, #4]!
	cmp	r3, r1
	bne	.L3406
	add	r2, r4, #252
	add	r3, r4, #384
	mov	r0, #0
.L3407:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L3407
	add	r2, r5, #40960
	add	r4, r4, #516
	mov	r0, r1
	str	r1, [r2, #1048]
.L3408:
	str	r0, [r3, #4]!
	cmp	r3, r4
	mov	r2, #0
	bne	.L3408
	add	r5, r5, #40960
	mov	r0, r2
	str	r2, [r5, #1052]
.L3393:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3400:
	mov	r0, r4
	bl	MVC_OutputFrmToVO
	cmp	r0, #0
	bne	.L3426
.L3403:
	ldr	r3, [r4, #520]
	cmp	r3, #0
	bne	.L3404
	b	.L3399
.L3426:
	str	r0, [sp]
	movw	r3, #5922
	ldr	r2, .L3427+4
	mov	r0, #22
	ldr	r1, .L3427+8
	bl	dprint_vfmw
	b	.L3403
.L3409:
	mvn	r0, #0
	b	.L3393
.L3428:
	.align	2
.L3427:
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
	bne	.L3438
.L3430:
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L3431
	movw	r3, #319
	ldr	r2, .L3439
	ldr	r1, .L3439+4
	mov	r0, #22
	bl	dprint_vfmw
.L3431:
	mov	r0, #0
	strb	r0, [r4, #6]
	str	r0, [r4, #220]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3438:
	movw	r3, #314
	ldr	r2, .L3439
	ldr	r1, .L3439+8
	mov	r0, #22
	bl	dprint_vfmw
	b	.L3430
.L3440:
	.align	2
.L3439:
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
	beq	.L3442
	ldrb	r3, [r5, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3442
.L3443:
	ldr	r3, [r4, #108]
	ldrb	r2, [r4, #8]	@ zero_extendqisi2
	ubfx	r7, r3, #16, #2
	cmp	r7, r2
	beq	.L3444
	sub	r2, r2, #2
	cmp	r2, #1
	cmpls	r7, #1
	bls	.L3501
.L3444:
	uxtb	r3, r7
	strb	r3, [r4, #8]
	cmp	r3, #2
	beq	.L3447
	cmp	r3, #3
	beq	.L3448
	cmp	r3, #1
	beq	.L3502
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3503
.L3455:
	ldr	r3, [r4, #64]
.L3451:
	ldrb	r2, [r5, #2789]	@ zero_extendqisi2
	strb	r2, [r4, #11]
.L3456:
	cmp	r3, #0
	beq	.L3499
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3504
	cmp	r3, #1
	bne	.L3459
	ldr	r2, [r5, #3496]
	cmp	r2, #0
	streqb	r3, [r4, #7]
.L3459:
	ldr	r2, [r4, #88]
	mov	r0, #2
	ldrb	r3, [r5, #2779]	@ zero_extendqisi2
	sub	r2, r2, #1
	ldr	r1, .L3507
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
	beq	.L3496
	ldrb	r1, [r5, #2788]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3460
	ldr	r2, .L3507+4
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3461
	ldr	r2, [r3, #1720]
	cmp	r2, #0
	beq	.L3470
.L3463:
	ldr	r3, [r2, #8]
	ldr	r0, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3463
.L3462:
	sub	r2, fp, #28
	mov	r3, #4
	ldr	r0, [r4, #120]
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
	ldr	r2, [r4, #224]
.L3464:
	movw	r3, #17096
	movt	r3, 170
	add	r3, r4, r3
	str	r3, [r4, #228]
	ldr	r1, [r2, #864]
	cmp	r1, #1
	beq	.L3505
	cmp	r1, #2
	beq	.L3506
.L3467:
	ldr	r3, [r2, #592]
	add	r6, r6, #16384
	mov	r0, #0
	str	r3, [r6, #1980]
	ldr	r3, [r2, #632]
	str	r3, [r6, #1984]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3442:
	ldr	r3, [r4, #140]
	ldr	r2, [r4, #144]
	add	r3, r3, #1
	ldr	r1, [r4, #224]
	cmp	r3, r2
	movlt	r3, r2
	str	r3, [r4, #140]
	rsb	r3, r2, r3
	str	r3, [r1, #576]
	b	.L3443
.L3501:
	mov	r2, r7
	ldr	r1, .L3507+8
	mov	r0, #2
	bl	dprint_vfmw
	strb	r7, [r4, #8]
.L3448:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mov	r0, r4
	mvn	r1, #0
	bl	MVC_ClearDPB
	mvn	r0, #0
.L3496:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3502:
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3455
	ldr	r3, [r5, #3432]
	cmp	r3, #0
	beq	.L3452
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3455
.L3468:
	ldrb	r2, [r5, #2789]	@ zero_extendqisi2
	ldr	r3, [r4, #64]
	strb	r2, [r4, #11]
	b	.L3456
.L3447:
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3452
	ldr	r3, [r4, #64]
	cmp	r3, #0
	beq	.L3451
	ldr	r2, [r4, #544]
	ldr	r2, [r2, #48]
	cmp	r2, #0
	beq	.L3451
.L3452:
	mov	r3, #1
	mov	r0, r4
	strb	r3, [r5, #2783]
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3496
.L3460:
	add	r3, r4, #11075584
	add	r3, r3, #45056
	ldr	r3, [r3, #2860]
	cmp	r3, #0
	bne	.L3461
	ldr	r2, [r4, #224]
	ldr	r3, [r2, #12]
	cmp	r3, #2
	beq	.L3464
.L3499:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3496
.L3503:
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3455
	b	.L3468
.L3504:
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3459
	ldr	r3, [r4, #224]
	ldr	r7, [r3, #12]
	cmp	r7, #0
	bne	.L3459
	ldr	r3, [r5, #3432]
	mov	r0, #1
	ldr	r2, [r5, #3436]
	ldr	r1, .L3507+12
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_ClearCurrPic
	ldr	r3, [r5, #3416]
	ldr	r0, [r4, #120]
	mov	r2, r7
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L3496
.L3461:
	ldr	r2, [r4, #224]
	b	.L3464
.L3505:
	cmp	r3, #0
	beq	.L3467
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3467
	mov	r2, #0
	str	r2, [r4, #228]
	ldr	r3, [r5, #3416]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	b	.L3499
.L3506:
	ldr	r1, [r2, #868]
	cmp	r3, #0
	cmpne	r1, #0
	ble	.L3467
	ldrb	r3, [r5, #2788]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3467
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
	b	.L3496
.L3470:
	mov	r1, r2
	b	.L3462
.L3508:
	.align	2
.L3507:
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
	ldr	r2, .L3511
	ldr	r1, .L3511+4
	mov	r0, #22
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L3512:
	.align	2
.L3511:
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
	ldr	r1, .L3576
	str	ip, [sp]
	bl	dprint_vfmw
	add	r3, r6, #16384
	mov	r2, #0
	str	r2, [r3, #2000]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3571
.L3514:
	ldr	r3, [r4, #232]
	cmp	r3, #0
	beq	.L3534
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3572
.L3517:
	ldr	r1, .L3576+4
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L3534
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L3534:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3521
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L3521
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3521
	ldrb	r3, [r4, #937]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3537
	mov	r2, r4
	mov	r3, #1
	b	.L3524
.L3523:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L3535
.L3524:
	ldrb	r1, [r2, #1025]	@ zero_extendqisi2
	add	r2, r2, #88
	cmp	r1, #0
	bne	.L3523
	mov	r1, r3
.L3522:
	mov	r2, #88
	cmn	r1, #1
	mla	r3, r2, r3, r4
	mov	r0, #1
	strb	r0, [r3, #937]
	beq	.L3535
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
	b	.L3527
.L3572:
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bhi	.L3517
	ldrb	r1, [r3, #3]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3518
	b	.L3517
.L3571:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	beq	.L3514
	ldr	r3, [r4, #232]
	cmp	r3, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	beq	.L3534
.L3518:
	strb	r2, [r3, #3]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #832]
	cmp	r2, #1
	beq	.L3573
.L3529:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3530
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq	.L3530
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3530
	ldr	r3, [r4, #232]
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bls	.L3574
.L3531:
	mov	r2, #1
	strb	r2, [r3, #3]
	ldr	r1, [r5, #16]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3576+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3568
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L3568:
	ldr	r3, [r4, #232]
.L3527:
	ldrb	r0, [r3, #3]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3521:
	ldr	r1, .L3576+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r1, [r5, #16]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3576+8
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3570
.L3569:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3570:
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3530:
	ldr	r3, [r4, #232]
	b	.L3531
.L3573:
	ldr	r3, [r3, #872]
	cmp	r3, #0
	bne	.L3568
	b	.L3529
.L3574:
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
	bcc	.L3575
.L3532:
	add	r6, r6, #16384
	mov	r2, #1
	str	r2, [r6, #2000]
	b	.L3527
.L3575:
	mov	r0, r4
	bl	MVC_CombinePacket.part.12
	ldr	r3, [r4, #232]
	b	.L3532
.L3535:
	ldr	r1, .L3576+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_ClearAllNal
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r3, .L3576+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3569
	b	.L3570
.L3537:
	mov	r1, r3
	b	.L3522
.L3577:
	.align	2
.L3576:
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
	ldr	r1, .L3611
	bl	dprint_vfmw
	mov	r0, r8
	bl	MVC_ClearCurrPic
	mov	r0, r8
	bl	MVC_ClearAllNal
	mov	r0, r8
	bl	MVC_InitDPB
	cmp	r0, #0
	bne	.L3608
.L3579:
	cmp	r4, #0
	beq	.L3580
	add	r1, r8, #584
	ldr	r0, [r8, #120]
	bl	FSP_ClearNotInVoQueue
.L3581:
	movw	r4, #48280
	mov	r10, #0
	movt	r4, 169
	add	r4, r8, r4
	mov	r6, #0
	mov	r7, #0
.L3584:
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
.L3583:
	ldrd	r2, [r5, #8]!
	orrs	r1, r2, r3
	bne	.L3609
.L3582:
	cmp	r5, r9
	bne	.L3583
	add	r10, r10, #1
	add	r4, r4, #808
	cmp	r10, #40
	bne	.L3584
	movw	r4, #15632
	movw	r5, #15664
	movt	r4, 170
	movt	r5, 170
	add	r4, r8, r4
	add	r5, r8, r5
	mov	r6, #0
	mov	r7, #0
.L3586:
	ldrd	r2, [r4, #8]!
	orrs	r1, r2, r3
	bne	.L3610
.L3585:
	cmp	r4, r5
	bne	.L3586
	ldr	r3, [r8, #60]
	cmp	r3, #0
	beq	.L3590
	mov	r4, #0
	add	r5, r8, #524
	mov	r6, r4
.L3589:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3588
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	str	r6, [r5]
.L3588:
	ldr	r3, [r8, #60]
	cmp	r3, r4
	bhi	.L3589
.L3590:
	add	r5, r8, #11075584
	ldr	ip, .L3611+4
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
.L3609:
	mov	r1, r2
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	strd	r6, [r5]
	b	.L3582
.L3610:
	mov	r1, r2
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	strd	r6, [r4]
	b	.L3585
.L3580:
	add	r0, r8, #584
	bl	ResetVoQueue
	ldr	r0, [r8, #120]
	bl	FSP_EmptyInstance
	b	.L3581
.L3608:
	mov	r3, #344
	ldr	r2, .L3611+8
	ldr	r1, .L3611+12
	mov	r0, #22
	bl	dprint_vfmw
	b	.L3579
.L3612:
	.align	2
.L3611:
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
	ldreq	r2, .L3714
	streq	r3, [r2, #-1884]
	cmp	r7, #1
	beq	.L3616
	bcc	.L3617
	cmp	r7, #2
	beq	.L3618
	mov	r2, r7
	ldr	r1, .L3714+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	mov	r1, #1
	bl	MVC_ClearAll
	mov	r0, #0
.L3622:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3618:
	ldr	r3, [r4, #640]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [fp, #-48]
	mov	r3, r3, asl #2
	beq	.L3638
	add	r8, r0, r3
	add	r2, r8, #11075584
	str	r2, [fp, #-52]
	add	r5, r2, #45056
	ldr	r2, [r5, #2664]
	cmp	r2, #0
	beq	.L3638
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
	beq	.L3650
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3650
.L3651:
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
	b	.L3637
.L3617:
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
	beq	.L3713
.L3619:
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
	ldr	r10, .L3714
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
	bne	.L3711
.L3637:
	ldr	r2, [fp, #-48]
	movw	r3, #28314
	movt	r3, 42
	add	r3, r2, r3
	ldr	r3, [r6, r3, asl #2]
	ldrb	r0, [r3, #3]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L3622
	ldr	r0, [r6, #120]
	mov	r2, #1
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3616:
	ldr	r3, [r4, #640]
	mov	r2, #3
	strb	r2, [r3, #696]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [fp, #-48]
	mov	r3, r3, asl #2
	beq	.L3623
	add	r8, r0, r3
	add	r2, r8, #11075584
	str	r2, [fp, #-52]
	add	r5, r2, #45056
	ldr	r2, [r5, #2664]
	cmp	r2, #0
	beq	.L3623
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
	beq	.L3634
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3634
.L3635:
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
	b	.L3637
.L3623:
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
	beq	.L3625
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3625
.L3626:
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
	ldr	r3, .L3714
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
	beq	.L3637
	mov	r3, r0
	movw	r2, #2268
	mov	r0, r7
.L3711:
	ldr	r1, .L3714+8
	bl	dprint_vfmw
	mov	r0, r6
	mvn	r1, #0
	bl	MVC_ClearDPB
	mvn	r0, #0
	b	.L3622
.L3638:
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
	beq	.L3640
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3640
.L3641:
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
	ldr	r3, .L3714
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
	beq	.L3637
	b	.L3711
.L3713:
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	b	.L3619
.L3640:
	ldr	r2, [r3, #2664]
	mov	r1, #2
	strb	r1, [r2, #3]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	ldreq	r3, [r3, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3641
.L3625:
	ldr	r2, [r3, #2664]
	mov	r1, #1
	strb	r1, [r2, #3]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, r1
	ldreq	r3, [r3, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3626
.L3634:
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #3]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #3]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r5, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3635
.L3650:
	ldr	r2, [r5, #2664]
	ldrb	r3, [r2, #3]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #3]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r5, #2664]
	ldreq	r2, [r4, #652]
	streq	r2, [r3, #28]
	b	.L3651
.L3715:
	.align	2
.L3714:
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
	beq	.L3717
	ldrb	r3, [r7, #1002]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r5, #2
	moveq	r5, #1
.L3717:
	ldr	r3, [fp, #-52]
	cmp	r3, #1
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	beq	.L3718
	cmp	r3, #2
	beq	.L3800
	add	r3, r6, #11141120
	str	r3, [fp, #-56]
	mov	r2, r3
	sub	r3, r5, #1
	add	r4, r2, #12288
	cmp	r3, #1
	mov	r3, #0
	strb	r3, [r4, #2777]
	bls	.L3801
.L3740:
	mov	r8, #0
	strb	r8, [r4, #2776]
	ldr	r10, [r6, #48]
	cmp	r10, r8
	beq	.L3747
	movw	ip, #48280
	add	r9, r9, #45056
	movt	ip, 169
	add	ip, r6, ip
	movw	r3, #47720
	movt	r3, 169
	add	r3, r6, r3
	str	r3, [fp, #-48]
.L3746:
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3742
	cmp	ip, #0
	beq	.L3759
	ldr	lr, [r9, #2856]
	cmp	lr, #0
	beq	.L3759
	ldr	r2, [r9, #2664]
	rsb	r3, ip, r2
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L3742
	ldr	r1, [fp, #-48]
	b	.L3744
.L3745:
	ldr	r2, [r1, #4]!
	rsb	r0, ip, r2
	cmp	r2, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L3742
.L3744:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L3745
.L3759:
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
	bne	.L3747
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
	beq	.L3748
	ldr	r3, [r6, #224]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L3802
.L3749:
	ldr	r9, .L3809
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
.L3739:
	ldr	r3, [fp, #-52]
	uxtb	r2, r5
	strb	r2, [r4, #2779]
	cmp	r3, #1
	moveq	r0, #0
	beq	.L3793
	ldrb	r3, [r7, #1001]	@ zero_extendqisi2
	ldr	r1, [r4, #3416]
	cmp	r3, #0
	movne	r3, #3
	strb	r3, [r1, #696]
	ldrb	r3, [r4, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [fp, #-56]
	add	r3, r3, #16384
	beq	.L3803
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
.L3793:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3742:
	add	r8, r8, #1
	add	ip, ip, #808
	cmp	r8, r10
	bne	.L3746
.L3747:
	ldr	r1, .L3809+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	mov	r1, #1
	bl	MVC_ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3800:
	add	r4, r6, #11141120
	str	r4, [fp, #-56]
	add	r4, r4, #12288
.L3737:
	mov	r3, #0
	strb	r3, [r4, #2777]
	b	.L3740
.L3718:
	cmp	r3, #2
	beq	.L3804
	add	r3, r6, #11141120
	str	r3, [fp, #-56]
	mov	r2, r3
	add	r4, r2, #12288
	mov	r3, #0
	mov	r5, r3
	strb	r3, [r4, #2777]
	b	.L3740
.L3802:
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
	b	.L3749
.L3801:
	add	r3, r2, #16384
	ldrb	r2, [r3, #136]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3721
	ldr	r0, [r3, #160]
	ldr	r1, [r7, #2640]
	cmp	r0, r1
	beq	.L3722
.L3721:
	add	r3, r9, #45056
	ldr	r1, [r3, #2856]
	subs	r3, r1, #1
	bmi	.L3740
	movw	r2, #28314
	movt	r2, 42
	add	r2, r1, r2
	add	r2, r6, r2, lsl #2
	b	.L3733
.L3724:
	cmp	r5, #2
	beq	.L3805
.L3723:
	subs	r3, r3, #1
	bmi	.L3740
.L3733:
	ldr	r8, [r2, #-4]!
	cmp	r8, #0
	beq	.L3723
	cmp	r5, #1
	bne	.L3724
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L3723
.L3725:
	ldr	r0, [r7, #1028]
	ldr	r1, [r8, #20]
	cmp	r0, r1
	bne	.L3723
	ldr	r0, [r7, #2640]
	ldr	r1, [r8, #56]
	cmp	r0, r1
	bne	.L3723
	ldrb	r1, [r7, #1008]	@ zero_extendqisi2
	cmp	r1, #0
	ldrb	r1, [r8, #3]	@ zero_extendqisi2
	beq	.L3806
	cmp	r1, #0
	beq	.L3723
.L3729:
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
	beq	.L3807
	ldrb	r2, [r4, #2777]	@ zero_extendqisi2
	ldrsb	r3, [r3, #4]
	cmp	r2, #0
	str	r3, [r4, #3492]
	beq	.L3740
	ldr	r3, [r4, #3416]
	cmp	r3, #0
	beq	.L3740
.L3797:
	mov	r8, #0
	b	.L3739
.L3805:
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3723
	b	.L3725
.L3803:
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
.L3804:
	add	r4, r6, #11141120
	mov	r5, #0
	str	r4, [fp, #-56]
	add	r4, r4, #12288
	b	.L3737
.L3806:
	cmp	r1, #0
	bne	.L3723
	b	.L3729
.L3722:
	sub	r8, r5, #2
	cmp	r2, #1
	cmpeq	r5, #2
	clz	r8, r8
	mov	r8, r8, lsr #5
	beq	.L3734
	cmp	r5, #1
	cmpeq	r2, #2
	bne	.L3740
.L3734:
	ldr	r1, [r3, #144]
	ldr	r2, [r7, #1028]
	cmp	r1, r2
	bne	.L3740
	ldrb	r2, [r7, #1008]	@ zero_extendqisi2
	ldr	r3, [r3, #140]
	cmp	r2, #0
	bne	.L3735
	cmp	r3, #0
	bne	.L3740
.L3736:
	ldr	r3, [r6, #128]
	ldr	r2, [r6, #124]
	rsb	r3, r2, r3
	cmp	r3, #2
	bhi	.L3740
	ldr	r3, [r4, #3416]
	mov	r2, #1
	strb	r2, [r4, #2777]
	cmp	r3, #0
	beq	.L3737
	cmp	r8, #0
	bne	.L3808
	cmp	r5, #1
	bne	.L3739
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3737
	b	.L3739
.L3748:
	ldr	ip, [r4, #3416]
	mov	r0, r2
	strb	r2, [r4, #2776]
	ldr	r1, .L3809+8
	strb	r2, [ip, #5]
	str	r3, [fp, #-48]
	bl	dprint_vfmw
	ldr	r3, [fp, #-48]
	mov	r0, r6
	mov	r1, r3
	bl	MVC_ClearAll
	mvn	r0, #0
	b	.L3793
.L3807:
	movw	r2, #6591
	ldr	r1, .L3809+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L3793
.L3808:
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, r2
	bne	.L3737
	b	.L3797
.L3735:
	cmp	r3, #0
	beq	.L3740
	b	.L3736
.L3810:
	.align	2
.L3809:
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
	ldr	r8, .L3823
	movw	r2, #18400
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
	bne	.L3820
	ldr	r5, [r5, #608]
	cmp	r5, #0
	beq	.L3821
.L3816:
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
.L3817:
	strb	r2, [r3]
	add	ip, r3, #274432
	add	r3, r3, #335872
	mov	r4, #0
	add	r3, r3, #308
	strb	r2, [ip, #241]
	cmp	r3, r7
	bne	.L3817
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
	beq	.L3822
	mov	r1, r4
	mov	r0, r6
	bl	MVC_ClearAll
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3821:
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
	b	.L3816
.L3820:
	movw	r3, #13884
	ldr	r2, .L3823+4
	ldr	r1, .L3823+8
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3822:
	mov	r0, r4
	ldr	r1, .L3823+12
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3824:
	.align	2
.L3823:
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
	beq	.L3988
	ldr	r3, [r0, #128]
	str	r3, [r0, #124]
	ldrb	r3, [r6, #2789]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3989
.L3828:
	ldrb	r0, [r5, #8]	@ zero_extendqisi2
	cmp	r0, #2
	beq	.L3990
	ldr	r8, [r5, #224]
	ldr	r3, [r8, #12]
	cmp	r3, #0
	ble	.L3991
.L3833:
	ldrb	r2, [r6, #2778]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3992
.L3834:
	cmp	r3, #2
	ldr	r3, [r6, #3432]
	beq	.L3993
.L3859:
	cmp	r3, #0
	beq	.L3866
	ldr	r3, [r5, #112]
	ldr	r2, [r6, #3468]
	cmp	r3, r2
	bcs	.L3867
	ldrb	r1, [r6, #2778]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L3994
.L3867:
	mov	r0, r5
	bl	MVC_Marking
	cmp	r0, #0
	bne	.L3995
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	ldr	r1, [r6, #3496]
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
.L3860:
	ldrb	r3, [r6, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3870
	add	r4, r4, #45056
	ldr	ip, [r4, #2856]
.L3871:
	ldr	r3, [r6, #3432]
	cmp	r3, #0
	beq	.L3880
	ldr	r2, [r4, #2864]
	ldr	r3, [r4, #2868]
	add	r1, r3, r2
	cmp	r1, ip
	bcs	.L3905
.L3880:
	mvn	r9, #0
	mov	r8, r9
	b	.L3889
.L3884:
	ldr	r3, [r6, #3432]
	mov	r1, r8
	mov	r0, r5
	mov	r9, r2
	cmp	r3, #0
	bne	.L3885
	ldr	r2, [r6, #3436]
	ldr	r3, [fp, #-56]
	cmp	r2, r3
	blt	.L3996
.L3885:
	bl	MVC_OutputFrmFromDPB
	cmn	r0, #1
	mov	r10, r0
	beq	.L3888
	ldr	ip, [r4, #2856]
.L3889:
	ldr	lr, [r4, #2860]
	sub	r3, fp, #52
	sub	r2, fp, #56
	mvn	r1, #0
	cmp	lr, ip
	mov	r0, r5
	bcc	.L3882
	bl	MVC_GetMinPOC
	cmp	r0, #0
	blt	.L3997
	ldr	r2, [r4, #2860]
	cmp	r2, r9
	ldrne	r8, [fp, #-52]
	bne	.L3884
	ldr	r3, [fp, #-52]
	cmp	r3, r8
	beq	.L3998
	mov	r8, r3
	b	.L3884
.L3989:
	ldr	r3, [r6, #3420]
	ldrb	r9, [r3, #1]	@ zero_extendqisi2
	cmp	r9, #0
	beq	.L3829
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L3828
	mov	r0, r8
	ldr	r1, .L4014
	bl	dprint_vfmw
	movw	r3, #3592
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L3992:
	ldrb	r1, [r6, #2779]	@ zero_extendqisi2
	ldr	r10, [r6, #3416]
	sub	r2, r1, #1
	cmp	r2, #1
	movhi	r2, #0
	movls	r2, #1
	cmp	r1, #0
	beq	.L3835
	ldrb	r0, [r10, #2]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	ip, r2
	orrne	ip, r2, #1
	cmp	ip, #0
	beq	.L3834
	cmp	r2, #0
	beq	.L3999
.L3908:
	ldr	r2, [r5, #220]
	cmp	r2, #0
	bne	.L3838
	add	r1, r6, #2768
	mov	r3, #1
	add	r1, r1, #8
	str	r3, [r5, #220]
	mov	r2, #728
	add	r0, r6, #3504
	bl	memcpy
	ldr	r3, [r8, #12]
	b	.L3834
.L3866:
	ldrb	r3, [r6, #2777]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3870
	add	r4, r4, #45056
	ldr	ip, [r4, #2856]
	b	.L3880
.L3835:
	cmp	r2, #0
	bne	.L3908
.L3841:
	cmp	r3, #0
	ble	.L3911
	movw	r3, #15064
	mov	r8, r10
	movt	r3, 170
	add	r3, r5, r3
.L3844:
	cmp	r1, #0
	beq	.L3846
	ldrb	r2, [r3, #1]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L4000
.L3846:
	ldr	r2, [r3, #692]
	str	r2, [r8, #44]
.L3847:
	mov	r1, r8
	mov	r0, r5
	str	r3, [fp, #-64]
	bl	MVC_GetImagePara
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetFsImagePtr
	ldr	r3, [fp, #-64]
	subs	r9, r0, #0
	beq	.L4001
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
	beq	.L3854
	ldr	r3, [r9, #108]
	ands	r3, r3, #3
	beq	.L4002
.L3852:
	ldr	r3, [r5, #224]
	ldr	r3, [r3, #4]
	cmp	r3, r2
	bcc	.L4003
.L3854:
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
	beq	.L3851
	mov	r2, #0
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
.L3851:
	ldr	r3, [r5, #224]
	ldr	r3, [r3, #604]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r9, #24]
	bcs	.L4004
.L3857:
	ldr	r3, [r5, #136]
	add	r3, r3, #2
	str	r3, [r5, #136]
.L3845:
	ldr	r3, [r5, #220]
	cmp	r3, #2
	beq	.L3858
.L3843:
	mov	r3, #1
	str	r3, [r8, #16]
	strb	r3, [r10, #8]
.L3858:
	ldr	r3, [r5, #224]
	mov	r2, #2
	str	r2, [r5, #220]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	ldr	r3, [r6, #3432]
	bne	.L3859
.L3993:
	cmp	r3, #0
	beq	.L3860
	add	r4, r4, #45056
	mov	r0, r5
	bl	MVC_SimpleSlideDPB
	ldr	r1, [r4, #2856]
	cmp	r1, #0
	beq	.L3861
	ldr	r3, [r4, #2664]
	cmp	r3, #0
	beq	.L3913
	movw	r2, #47720
	mov	r3, #0
	movt	r2, 169
	add	r2, r5, r2
	b	.L3862
.L3863:
	ldr	r0, [r2, #4]!
	cmp	r0, #0
	beq	.L3913
.L3862:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3863
.L3861:
	movw	r2, #15064
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	subs	r8, r0, #0
	bne	.L4005
	ldr	r1, [r4, #2860]
	mov	r0, r5
	add	r1, r1, #1
	str	r1, [r4, #2860]
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	mov	r2, r8
	movw	r3, #3773
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3865
.L3870:
	ldr	r3, [r5, #520]
	ldr	lr, [r6, #3416]
	cmp	r3, #0
	rsb	r2, r3, lr
	clz	r2, r2
	mov	r2, r2, lsr #5
	moveq	r2, #0
	cmp	r2, #0
	beq	.L4006
	mov	r0, r5
	bl	MVC_DirectOutput
	cmn	r0, #1
	mov	r4, r0
	beq	.L4007
.L3876:
	movw	r3, #3825
	mov	r2, r4
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3832
.L3997:
	ldr	r1, .L4014+8
	movw	r2, #3875
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
.L3882:
	ldr	r3, [r6, #3432]
	cmp	r3, #0
	beq	.L3894
	ldrb	r3, [r6, #2780]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3894
	ldr	r2, [r4, #2864]
	cmp	r2, #0
	beq	.L3894
	movw	ip, #47780
	mov	lr, r3
	movt	ip, 169
	add	ip, r5, ip
	b	.L3899
.L3896:
	ldr	r2, [r4, #2864]
	cmp	r2, r3
	bls	.L3894
.L3899:
	ldr	r2, [ip, #4]!
	add	r3, r3, #1
	ldr	r1, [r6, #3424]
	ldr	r0, [r2, #20]
	cmp	r0, r1
	bne	.L3896
	ldr	r0, [r2, #56]
	ldr	r1, [r6, #3496]
	cmp	r0, r1
	bne	.L3896
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
	b	.L3896
.L3894:
	ldr	r1, [r4, #2856]
	cmp	r1, #0
	beq	.L3893
	ldr	r3, [r4, #2664]
	cmp	r3, #0
	beq	.L3917
	movw	r2, #47720
	mov	r3, #0
	movt	r2, 169
	add	r2, r5, r2
	b	.L3900
.L3901:
	ldr	r0, [r2, #4]!
	cmp	r0, #0
	beq	.L3917
.L3900:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3901
.L3893:
	movw	r2, #15064
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	cmp	r0, #0
	bne	.L4008
	ldr	r3, [r4, #2860]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #2860]
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
.L3865:
	mov	r4, #0
.L3903:
	add	r7, r7, #12288
	mov	r3, #0
	mov	r0, r4
	strb	r3, [r7, #2776]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3829:
	mvn	r1, #0
	bl	MVC_FlushDPB
	cmp	r0, #0
	beq	.L3828
	mov	r0, r9
	ldr	r1, .L4014+12
	bl	dprint_vfmw
	movw	r3, #3602
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L3991:
	ldr	r2, [r5, #220]
	cmp	r2, #2
	bne	.L3833
	b	.L3834
.L4006:
	add	r4, r4, #45056
	ldr	ip, [r4, #2856]
	cmp	ip, #0
	beq	.L3873
	ldr	r3, [r4, #2664]
	adds	r1, r3, #0
	movne	r1, #1
	cmp	lr, r3
	movne	r1, #0
	cmp	r1, #0
	movweq	r2, #47720
	movteq	r2, 169
	addeq	r2, r5, r2
	bne	.L4009
.L3877:
	add	r1, r1, #1
	cmp	r1, ip
	beq	.L3871
	ldr	r3, [r2, #4]!
	adds	r0, r3, #0
	movne	r0, #1
	cmp	lr, r3
	movne	r0, #0
	cmp	r0, #0
	beq	.L3877
.L3874:
	movw	r2, #15064
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	cmn	r0, #1
	mov	r4, r0
	beq	.L4010
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	movw	r3, #3851
	mov	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3865
.L3999:
	cmp	r0, #0
	beq	.L3834
	b	.L3841
.L3917:
	mov	r1, r3
	b	.L3893
.L3888:
	mov	r3, r0
	movw	r2, #3915
	ldr	r1, .L4014+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, r10
	movw	r3, #3916
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
.L3907:
	ldr	r3, [r6, #3416]
	mov	r2, #1
	ldr	r0, [r5, #120]
	mvn	r4, #0
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	mov	r3, r4
	movw	r2, #3974
	ldr	r1, .L4014+16
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [r6, #3416]
	mov	r3, #0
	strb	r3, [r2, #5]
	ldr	r2, [r6, #3416]
	strb	r3, [r2, #2]
	b	.L3903
.L3990:
	ldr	r1, .L4014+20
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
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
.L3832:
	cmn	r4, #1
	bne	.L3903
	b	.L3907
.L3838:
	cmp	r2, #1
	beq	.L4011
	cmp	r1, #0
	beq	.L3841
	ldrb	r2, [r10, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3841
	b	.L3834
.L3996:
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L4012
.L3886:
	mov	r0, r5
	bl	MVC_DirectOutput
	cmn	r0, #1
	mov	r4, r0
	beq	.L4013
.L3887:
	movw	r3, #3907
	mov	r2, r4
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3832
.L3911:
	mov	r8, r10
	b	.L3845
.L3998:
	mov	r3, r8
	ldr	r2, .L4014+24
	ldr	r1, .L4014+28
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3884
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4004:
	mov	r1, r9
	mov	r0, r5
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3857
.L3913:
	mov	r1, r3
	b	.L3861
.L4012:
	ldrb	r3, [r6, #2779]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r6, #3416]
	strne	r3, [r5, #524]
	b	.L3886
.L4011:
	cmp	r3, #0
	add	r3, r7, #16384
	ldr	r8, [r3, #48]
	ble	.L3843
	movw	r3, #15792
	ldrb	r1, [r6, #3507]	@ zero_extendqisi2
	movt	r3, 170
	add	r3, r5, r3
	b	.L3844
.L4002:
	movw	r2, #3702
	ldr	r1, .L4014+32
	mov	r0, #1
	str	r3, [fp, #-64]
	bl	dprint_vfmw
	ldr	r2, .L4014+36
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3853
	ldr	r3, [fp, #-64]
	mov	r1, #111
	ldr	r0, [r5, #120]
	mov	r2, r3
	blx	ip
.L3853:
	ldr	r2, [r9, #160]
	cmp	r2, #0
	bne	.L3852
	b	.L3854
.L4000:
	mov	r1, #3
	ldr	r2, [r8, #44]
	strb	r1, [r8, #2]
	ldr	r1, [r3, #692]
	add	r2, r2, r1
	str	r2, [r8, #44]
	b	.L3847
.L3988:
	mov	r0, r3
	ldr	r1, .L4014+40
	bl	dprint_vfmw
	movw	r3, #3578
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4013:
	mov	r3, r0
	mov	r2, #3904
	ldr	r1, .L4014+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3887
.L3995:
	mov	r2, r0
	ldr	r1, .L4014+44
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3798
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4008:
	mov	r3, r0
	movw	r2, #3960
	ldr	r1, .L4014+16
	mov	r0, #1
	bl	dprint_vfmw
	movw	r3, #3961
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4009:
	mov	r1, r2
	b	.L3874
.L3873:
	ldr	r3, [r6, #3432]
	cmp	r3, #0
	beq	.L3880
	ldr	r2, [r4, #2864]
	ldr	r3, [r4, #2868]
.L3905:
	str	r3, [sp, #4]
	mov	r0, #1
	mov	r3, ip
	str	r2, [sp]
	ldr	r1, .L4014+48
	movw	r2, #3864
	bl	dprint_vfmw
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
	movw	r3, #3867
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L3994:
	ldr	r1, .L4014+52
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4014+36
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3868
	ldr	r4, [r6, #3468]
	mov	r3, #8
	ldr	lr, [r5, #112]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r5, #120]
	str	r4, [fp, #-52]
	str	lr, [fp, #-48]
	blx	ip
.L3868:
	mov	r0, r5
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
	movw	r3, #3789
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4007:
	mov	r3, r0
	movw	r2, #3821
	ldr	r1, .L4014+16
	mov	r0, #1
	bl	dprint_vfmw
	b	.L3876
.L4005:
	mov	r3, r8
	movw	r2, #3764
	ldr	r1, .L4014+16
	mov	r0, #0
	bl	dprint_vfmw
	movw	r3, #3765
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4010:
	mov	r3, r0
	movw	r2, #3844
	ldr	r1, .L4014+16
	mov	r0, #1
	bl	dprint_vfmw
	mov	r2, r4
	movw	r3, #3845
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4003:
	ldrsb	r1, [r8, #6]
	mov	r2, #0
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
	movw	r3, #3721
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4001:
	movw	r3, #3691
	mvn	r2, #0
	ldr	r1, .L4014+4
	mov	r0, #14
	bl	dprint_vfmw
	b	.L3907
.L4015:
	.align	2
.L4014:
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
	beq	.L4031
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
	ldr	r1, .L4034
	bl	dprint_vfmw
	ldr	r9, [fp, #-52]
	mov	r3, r10
	ldr	r1, .L4034+4
	mov	r0, #2
	mov	r2, r9
	bl	dprint_vfmw
	cmp	r9, r10
	mov	r3, r9
	movgt	r3, r4
	movgt	r4, r10
	movgt	r10, r3
	bgt	.L4024
	b	.L4025
.L4022:
	strb	r9, [r10, #2780]
	mov	r0, r5
	bl	MVC_StorePicInDpb
	movw	lr, #26878
	movt	lr, 42
	ldr	r1, [fp, #-60]
	subs	r3, r0, #0
	add	r0, r4, #1
	bne	.L4032
	ldr	r3, [r7, #2644]
	add	lr, r3, lr
	add	lr, r5, lr, lsl #2
	str	r4, [lr, #4]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-52]
	cmp	r3, r1
	mov	r4, r1
	ble	.L4025
.L4024:
	mov	r1, #1
	mov	r0, r5
	bl	MVC_AllocFrameStore
	mov	r9, #0
	ldr	r1, .L4034+8
	subs	r3, r0, #0
	mov	r0, #2
	bne	.L4033
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
	beq	.L4022
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
	b	.L4022
.L4031:
	ldr	r1, .L4034+12
	mov	r0, r6
	bl	dprint_vfmw
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4025:
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
.L4033:
	ldr	r2, [fp, #-64]
	mov	r0, r9
	ldr	r3, [fp, #-76]
	ldr	r1, .L4034+16
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
.L4032:
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
	ldr	r1, .L4034+20
	strb	lr, [ip, #3]
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4035:
	.align	2
.L4034:
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
	beq	.L4037
	ldr	r3, [r5, #3416]
	cmp	r3, r1
	beq	.L4038
	ldrb	r7, [r3, #2]	@ zero_extendqisi2
	cmp	r7, r1
	beq	.L4196
.L4038:
	mov	r3, #0
	strb	r3, [r5, #2776]
.L4037:
	sub	r1, fp, #48
	mov	r0, r4
	bl	MVC_GetReRangeFlag
	ldrb	r3, [r6, #1012]	@ zero_extendqisi2
	cmp	r3, #255
	mov	r10, r0
	beq	.L4197
.L4039:
	cmp	r3, #0
	bne	.L4042
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L4198
	ldr	r3, [r9, #28]
	ldr	r2, [r4, #28]
	cmp	r2, r3
	movweq	r3, #35364
	movteq	r3, 168
	addeq	r3, r4, r3
	streq	r3, [r4, #236]
	bne	.L4199
.L4042:
	add	r7, r8, #36864
	ldr	r3, [r9, #24]
	ldr	r2, [r7, #1744]
	cmp	r2, r3
	beq	.L4200
.L4046:
	mov	r3, #0
	mov	r1, r9
	strb	r3, [r9, #20]
	ldr	r0, [r4, #236]
	bl	mvc_assign_quant_params
	ldr	r3, .L4214
	movw	r0, #38584
	mov	r2, #2240
	movt	r0, 169
	mov	r1, r9
	add	r0, r4, r0
	ldr	r3, [r3, #56]
	blx	r3
.L4047:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r4, #224]
	bne	.L4201
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L4049
.L4058:
	ldr	r2, [r7, #1744]
	ldr	r3, [r9, #24]
	cmp	r2, r3
	bne	.L4067
	ldrb	r3, [r9, #20]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4067
.L4068:
	ldrb	r3, [r6, #1012]	@ zero_extendqisi2
	cmp	r3, #255
	moveq	r3, #0
	streq	r3, [r6, #2644]
	beq	.L4070
	cmp	r3, #0
	bne	.L4070
	add	r1, r4, #10747904
	movw	r2, #22868
	add	r1, r1, #20480
	movt	r2, 164
	ldr	ip, [r6, #2640]
	add	r2, r4, r2
	ldr	r0, [r1, #2384]
	adds	r0, r0, #1
	beq	.L4071
	ldr	r1, [r1, #2388]
	cmp	ip, r1
	beq	.L4112
	mov	lr, r0
	b	.L4072
.L4073:
	ldr	r1, [r2, #4]!
	cmp	ip, r1
	beq	.L4071
.L4072:
	add	r3, r3, #1
	cmp	r3, lr
	mov	r0, r3
	bne	.L4073
.L4071:
	str	r0, [r6, #2644]
.L4070:
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
	beq	.L4076
	ldr	r2, [r4, #224]
	ldr	r3, [r2, #12]
	str	r2, [fp, #-60]
	cmp	r3, #2
	beq	.L4076
	ldr	r1, [r6, #2644]
	movw	r3, #26878
	movt	r3, 42
	add	r3, r1, r3
	add	r3, r4, r3, lsl #2
	ldr	r0, [r3, #4]
	cmp	r0, r10
	beq	.L4076
	mov	r3, #1
	add	r9, r9, #4
	mov	r9, r3, asl r9
	add	r0, r0, r3
	str	r3, [fp, #-64]
	mov	r1, r9
	bl	__aeabi_uidivmod
	cmp	r1, r10
	beq	.L4076
	ldr	r3, [fp, #-64]
	ldr	r2, [fp, #-60]
	str	r3, [r2, #600]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #600]
	cmp	r2, #0
	beq	.L4202
.L4077:
	add	r3, r8, #32768
	ldrb	r3, [r3, #1843]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4086
	ldr	r1, .L4214+4
	mov	r0, #2
	bl	dprint_vfmw
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #604]
	bics	r3, r3, #1024
	bne	.L4193
	ldr	r2, [r6, #2640]
	cmp	r2, #0
	bne	.L4193
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
	bcs	.L4076
	ldrb	r3, [r6, #1000]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L4076
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
	ldr	r1, .L4214+8
	bl	dprint_vfmw
	mvn	r0, #0
.L4184:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4201:
	ldr	r2, [r3, #708]
	cmp	r2, #0
	bne	.L4049
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L4203
.L4049:
	ldr	r3, [r4, #84]
	cmp	r10, #0
	add	r3, r3, #1
	str	r3, [r4, #84]
	bne	.L4059
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	beq	.L4060
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	cmp	r0, #0
	bne	.L4204
.L4192:
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	bne	.L4110
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	bne	.L4058
.L4064:
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
	beq	.L4205
.L4190:
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #872]
	cmp	r3, #1
	mvneq	r0, #1
	beq	.L4184
	mov	r5, #0
	ldr	r1, .L4214+12
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
.L4067:
	ldr	r3, .L4214
	movw	r0, #38584
	mov	r2, #0
	movt	r0, 169
	strb	r2, [r9, #20]
	add	r0, r4, r0
	mov	r1, r9
	ldr	r3, [r3, #56]
	mov	r2, #2240
	blx	r3
	b	.L4068
.L4086:
	mov	r0, r4
	bl	MVC_DecGap
	cmp	r0, #0
	bne	.L4089
.L4193:
	ldr	r10, [r6, #1028]
.L4076:
	ldrb	r3, [r6, #1008]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4090
	ldr	r2, [r6, #2644]
	movw	r3, #26878
	movt	r3, 42
	add	r3, r2, r3
	add	r3, r4, r3, lsl #2
	str	r10, [r3, #4]
.L4090:
	mov	r0, r4
	str	r10, [r7, #4016]
	bl	MVC_DecPOC
	mov	r1, #0
	mov	r0, r4
	bl	MVC_AllocFrameStore
	cmp	r0, #0
	bne	.L4206
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
	bne	.L4093
	ldrb	r3, [r2, #21]	@ zero_extendqisi2
	adds	r3, r3, #0
	movne	r3, #1
.L4093:
	cmp	r9, #0
	strb	r3, [r5, #2785]
	beq	.L4094
	ldr	r2, [r6, #2644]
	movw	r3, #26878
	movt	r3, 42
	add	r3, r2, r3
	add	r3, r4, r3, lsl #2
	str	r10, [r3, #4]
.L4094:
	ldr	r3, [r5, #3416]
	cmp	r3, #0
	ldrneb	r2, [r6, #1012]	@ zero_extendqisi2
	strneb	r2, [r3, #9]
	mvn	r3, #0
	str	r3, [r5, #3496]
	ldrb	r3, [r6, #1011]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4096
	ldr	r3, [r6, #2640]
	str	r3, [r5, #3496]
	ldrb	r3, [r6, #1009]	@ zero_extendqisi2
	strb	r3, [r5, #2786]
	ldrb	r3, [r6, #1010]	@ zero_extendqisi2
	strb	r3, [r5, #2787]
.L4096:
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
	ldr	r1, .L4214+16
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
	ldr	r1, .L4214+20
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
	beq	.L4207
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
	beq	.L4098
	ldr	r3, [r5, #2888]
	mov	r1, #0
	ldr	r2, [r0, #8]
	subs	r3, r3, r1
	ldr	r0, [r4, #120]
	movne	r3, #1
	bl	FSP_SetStoreType
.L4098:
	movw	ip, #15632
	movw	lr, #15664
	ldr	r3, [r4, #60]
	movt	ip, 170
	movt	lr, 170
	add	ip, r4, ip
	add	lr, r4, lr
	mov	r6, #0
.L4102:
	ldrd	r0, [ip, #8]!
	orrs	r2, r0, r1
	bne	.L4099
	cmp	r3, #0
	beq	.L4099
	ldr	r2, [r4, #528]
	subs	r3, r3, #1
	stmia	ip, {r2, r6}
	beq	.L4100
	add	r1, r4, #528
	mov	r2, #0
.L4101:
	add	r2, r2, #1
	ldr	r0, [r1, #4]!
	cmp	r2, r3
	str	r0, [r1, #-4]
	bne	.L4101
.L4100:
	add	r2, r3, #132
	str	r6, [r4, r2, asl #2]
	str	r3, [r4, #60]
.L4099:
	cmp	ip, lr
	bne	.L4102
	cmp	r3, #0
	beq	.L4104
	mov	r6, r3
	ble	.L4104
	add	r7, r3, #132
	mov	r8, #0
	add	r7, r4, r7, lsl #2
.L4105:
	ldr	r1, [r7, #-4]!
	sub	r6, r6, #1
	ldr	r0, [r4, #120]
	bl	FreeUsdByDec
	cmp	r6, #0
	str	r8, [r7]
	bne	.L4105
.L4104:
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
.L4200:
	ldrb	r3, [r9, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4047
	b	.L4046
.L4197:
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
	beq	.L4208
.L4040:
	ldr	r3, .L4214
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
.L4041:
	str	r7, [r4, #236]
	b	.L4039
.L4060:
	ldr	r1, .L4214+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_GetBackPicFromVOQueue
	b	.L4192
.L4208:
	ldrb	r1, [r2, #26]	@ zero_extendqisi2
	cmp	r1, #0
	movteq	r7, 169
	addeq	r7, r4, r7
	beq	.L4041
	b	.L4040
.L4196:
	ldrsb	r1, [r3, #6]
	mov	r2, #1
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
	ldr	r3, [r5, #3416]
	strb	r7, [r3, #2]
	ldr	r3, [r5, #3416]
	strb	r7, [r3, #5]
	b	.L4038
.L4202:
	ldr	r2, [r3, #640]
	cmp	r2, #1
	beq	.L4209
.L4078:
	ldr	r3, [r6, #2644]
	ldr	r1, [r6, #1028]
	add	r3, r4, r3, lsl #2
	add	r3, r3, #11075584
	add	r2, r3, #40960
	ldr	r2, [r2, #1020]
	cmp	r1, r2
	bls	.L4079
	ldr	ip, [r7, #1700]
	rsb	r0, r2, r1
	cmp	r2, #0
	cmpne	ip, r0
	bcs	.L4077
	ldrb	r2, [r6, #1000]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L4210
	add	r3, r3, #40960
	cmp	r1, #0
	subne	r2, r1, #1
	subeq	r2, r9, #1
	mvn	r1, #0
	str	r2, [r3, #1020]
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r3, r0, #0
	beq	.L4077
	movw	r2, #7363
	ldr	r1, .L4214+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4203:
	ldr	r3, [r4, #84]
	cmp	r10, #0
	add	r3, r3, #1
	str	r3, [r4, #84]
	beq	.L4050
	mvn	r1, #0
	mov	r0, r4
	str	r2, [fp, #-60]
	bl	MVC_FlushDPB
	ldr	r2, [fp, #-60]
	subs	r3, r0, #0
	bne	.L4211
	mov	r0, r4
	bl	MVC_InitDPB
	subs	r3, r0, #0
	bne	.L4107
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
	bne	.L4056
	ldr	r3, [r7, #1700]
	ldr	r0, [r4, #224]
	mov	r3, r3, asl #1
	str	r3, [r4, #52]
	ldr	r1, [r0, #668]
	str	r3, [fp, #-60]
	cmp	r1, r10
	bcc	.L4052
	ldr	r2, [r0, #672]
	cmp	r2, ip
	bcc	.L4052
	add	lr, r8, #45056
	ldr	r3, [r0, #692]
	ldr	lr, [lr, #2856]
	cmp	r3, lr
	bcc	.L4212
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
	bne	.L4190
	b	.L4056
.L4198:
	strb	r3, [r4, #2]
	movw	r1, #8500
	ldr	r3, [r9, #28]
	movt	r1, 5
	mov	r2, r1
	ldr	ip, .L4214
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
	b	.L4042
.L4059:
	ldr	r1, .L4214+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_GetBackPicFromVOQueue
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L4064
.L4110:
	mov	r3, r0
	movw	r2, #7252
	ldr	r1, .L4214+32
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4205:
	mov	ip, ip, lsr #4
	mov	r10, r10, lsr #4
	str	ip, [r4, #12]
	str	r10, [r4, #16]
	b	.L4058
.L4079:
	rsb	r0, r2, r1
	ldr	ip, [r7, #1700]
	add	r0, r0, r9
	cmp	r2, #0
	cmpne	ip, r0
	bcs	.L4077
	ldrb	r2, [r6, #1000]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L4213
	add	r3, r3, #40960
	cmp	r1, #0
	subne	r2, r1, #1
	subeq	r2, r9, #1
	mvn	r1, #0
	str	r2, [r3, #1020]
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r3, r0, #0
	beq	.L4077
	movw	r2, #7390
	ldr	r1, .L4214+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4050:
	mov	r0, r4
	bl	MVC_InitDPB
	subs	r3, r0, #0
	bne	.L4107
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
.L4056:
	mov	r10, r10, lsr #4
	mov	ip, ip, lsr #4
	str	r10, [r4, #12]
	str	ip, [r4, #16]
	b	.L4058
.L4199:
	ldr	r1, .L4214+36
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4089:
	movw	r2, #7444
	ldr	r1, .L4214+40
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4209:
	ldr	r2, [r3, #636]
	cmp	r2, #0
	bne	.L4078
	ldr	r2, [r3, #644]
	cmp	r2, #0
	bne	.L4078
	ldr	r3, [r3, #604]
	add	r3, r3, #1024
	cmp	r3, #2048
	bhi	.L4077
	b	.L4078
.L4112:
	mov	r0, r3
	b	.L4071
.L4206:
	movw	r2, #7464
	ldr	r1, .L4214+44
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4207:
	movw	r2, #7572
	ldr	r1, .L4214+48
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4204:
	mov	r3, r0
	movw	r2, #7243
	mov	r0, r10
	ldr	r1, .L4214+28
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4210:
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
.L4194:
	add	r4, r4, r1, lsl #2
	str	r3, [sp]
	ldr	r1, .L4214+52
	mov	r0, #0
	ldr	r3, [r4, #4]
	rsb	r3, r3, ip
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4107:
	mov	r2, #7168
	ldr	r1, .L4214+32
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4213:
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
	b	.L4194
.L4052:
	ldr	r1, .L4214+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4211:
	mov	r0, r2
	ldr	r1, .L4214+28
	movw	r2, #7159
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4212:
	ldr	r1, .L4214+60
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4184
.L4215:
	.align	2
.L4214:
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
	ldr	r1, .L4268
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
	bne	.L4261
	mov	r0, r5
	bl	MVC_ProcessSliceHeaderSecondPart
	cmp	r0, #0
	bne	.L4261
	ldr	r2, [r5, #64]
	cmp	r2, #0
	moveq	r3, r2
	beq	.L4223
	mov	r1, r0
	movw	r8, #16544
	movt	r8, 170
	mov	r3, r1
	add	r8, r5, r8
	mov	r6, r1
	b	.L4231
.L4262:
	cmp	lr, r1
	add	r3, r7, r3
	rsbhi	r1, r1, lr
	rsbhi	r0, r0, r9
	addhi	r1, r1, r0
	movls	r1, r9
	add	r3, r3, r1
.L4229:
	cmp	ip, #0
	add	r6, r6, #1
	movne	r1, ip
	movne	r0, r7
	moveq	r1, lr
	moveq	r0, r9
	cmp	r6, r2
	beq	.L4223
.L4231:
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
	bne	.L4262
	add	r3, r9, r3
	add	r3, r3, r7
	b	.L4229
.L4223:
	add	r6, r5, #11141120
	str	r3, [r5, #104]
	add	r6, r6, #12288
	movw	r0, #1620
	ldr	ip, [r6, #3476]
	ldr	r1, [r6, #3472]
	mul	r1, r1, ip
	cmp	r1, r0
	movgt	r1, r1, asl #7
	ble	.L4263
.L4233:
	ldr	r0, [r5, #32]
	cmp	r0, r2
	bls	.L4234
	cmp	r1, r3
	bcc	.L4234
	ldrb	r3, [r4, #1005]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4264
.L4237:
	mov	r0, r5
	bl	MVC_PicTypeStatistic
	mov	r0, r5
	bl	MVC_DecList
	cmp	r0, #0
	bne	.L4265
	mov	r0, r5
	bl	MVC_IsRefListWrong
	subs	r4, r0, #0
	bne	.L4266
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
.L4220:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4263:
	mov	r0, r1, asl #9
	sub	r1, r0, r1, asl #7
	cmp	r1, #16384
	movlt	r1, #16384
	b	.L4233
.L4264:
	ldr	r1, .L4268+4
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
	beq	.L4220
	cmp	r0, #0
	bne	.L4267
	mov	r0, r5
	bl	MVC_WritePicMsg
	b	.L4237
.L4261:
	ldr	r1, .L4268+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4268+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4259
	sub	r2, fp, #36
	mvn	r1, #0
	mov	r3, #4
	str	r1, [r2, #-8]!
	mov	r1, #100
	ldr	r0, [r5, #120]
	blx	ip
.L4259:
	mov	r0, r5
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	b	.L4220
.L4234:
	str	r1, [sp]
	mov	r0, #0
	ldr	r1, .L4268+16
	bl	dprint_vfmw
	ldr	r3, [r5, #64]
	ldr	r2, [r5, #32]
	cmp	r3, r2
	bcc	.L4236
	ldr	r1, .L4268+12
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L4236
	str	r3, [fp, #-44]
	mov	r1, #108
	str	r2, [fp, #-40]
	mov	r3, #8
	sub	r2, fp, #44
	ldr	r0, [r5, #120]
	blx	ip
.L4236:
	ldr	r1, .L4268+20
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L4220
.L4265:
	mov	r2, r0
	ldr	r1, .L4268+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4220
.L4266:
	ldr	r1, .L4268+28
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4259
.L4267:
	ldr	r1, .L4268+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4220
.L4269:
	.align	2
.L4268:
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
	beq	.L4275
.L4274:
	ldr	r0, [r2, #12]
	add	r3, r3, #1
	add	r2, r2, #28
	add	ip, ip, r0
	str	ip, [r4, #104]
	ldr	r0, [lr, #68]
	cmp	r0, r3
	bhi	.L4274
.L4275:
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
	beq	.L4456
.L4273:
	ldr	r2, [r4, #68]
	mov	r0, #22
	ldr	r1, .L4467
	bl	dprint_vfmw
	ldr	r3, [r4, #68]
	movw	r2, #371
	cmp	r3, r2
	beq	.L4457
.L4278:
	ldr	r2, [r4, #232]
	add	r3, r3, #1
	str	r3, [r4, #68]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	sub	r3, r2, #1
	cmp	r3, #29
	ldrls	pc, [pc, r3, asl #2]
	b	.L4279
.L4281:
	.word	.L4280
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4280
	.word	.L4282
	.word	.L4283
	.word	.L4284
	.word	.L4285
	.word	.L4286
	.word	.L4287
	.word	.L4288
	.word	.L4289
	.word	.L4290
	.word	.L4291
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4292
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4279
	.word	.L4293
.L4456:
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
	bne	.L4276
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
	beq	.L4458
.L4276:
	ldr	r2, [r3, #64]
	add	r2, r2, #24
	str	r2, [r3, #64]
	ldr	r3, [r4, #232]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	b	.L4273
.L4280:
	ldr	r3, [r4, #80]
	mov	r0, #22
	ldr	r1, .L4467+4
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
	beq	.L4459
	cmp	r3, #255
	beq	.L4460
.L4295:
	ldr	r2, [r4, #88]
	mov	r0, #21
	ldr	r1, .L4467+8
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_SliceCheck
	subs	r5, r0, #0
	mov	r0, r4
	bne	.L4461
	bl	MVC_DecSlice
	cmn	r0, #2
	beq	.L4306
	cmp	r0, #0
	beq	.L4437
	ldr	r3, .L4467+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4436
	ldr	r0, [r4, #120]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L4436:
	mvn	r0, #0
	b	.L4306
.L4282:
	ldr	r1, .L4467+16
	mov	r2, #6
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecSEI
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4313
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4313:
	cmp	r5, #0
	bne	.L4462
.L4437:
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L4283:
	ldr	r1, .L4467+20
	mov	r2, #7
	ldr	r3, [r4, #72]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecSPS
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4311
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4311:
	cmp	r5, #0
	bne	.L4463
	ldr	r3, [r4, #72]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #72]
	b	.L4306
.L4285:
	ldr	r1, .L4467+24
	mov	r2, #9
.L4444:
	mov	r0, #22
	bl	dprint_vfmw
.L4449:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4437
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #232]
.L4306:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L4286:
	mov	r2, #10
	ldr	r1, .L4467+28
	b	.L4444
.L4284:
	ldr	r1, .L4467+32
	mov	r2, #8
	ldr	r3, [r4, #76]
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecPPS
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
	bne	.L4464
	ldr	r3, [r4, #76]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #76]
	b	.L4306
.L4287:
	mov	r2, #11
	ldr	r1, .L4467+36
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
	bne	.L4449
	ldr	r5, .L4467+12
	mov	r3, #1
	strb	r3, [r4, #1]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4316
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L4316:
	mov	r6, #0
	mov	r0, r4
	strb	r6, [r4]
	bl	MVC_DecVDM
	cmp	r0, r6
	beq	.L4449
	b	.L4452
.L4288:
	mov	r2, #12
	ldr	r1, .L4467+40
	b	.L4444
.L4289:
	mov	r2, #13
	ldr	r1, .L4467+44
	b	.L4444
.L4290:
	mov	r2, #14
	ldr	r1, .L4467+48
	b	.L4444
.L4291:
	ldr	r1, .L4467+52
	mov	r2, #15
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	bl	MVC_DecSubSPS
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4317
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4317:
	cmp	r5, #0
	bne	.L4465
	ldr	r3, [r4, #20]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #20]
	b	.L4306
.L4292:
	mov	r2, #19
	ldr	r1, .L4467+56
	b	.L4444
.L4293:
	mov	r2, #30
	ldr	r1, .L4467+60
	mov	r0, #22
	bl	dprint_vfmw
	cmp	r5, #0
	bne	.L4319
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4319
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	str	r5, [r4, #232]
.L4319:
	mov	r1, #32
	mov	r0, r7
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r7
	bl	BsGet
	ldr	r3, .L4467+64
	rev	r5, r5
	ldr	r2, [r3, #440]
	ldr	r3, [r3, #436]
	rev	r0, r0
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L4437
	ldr	r5, .L4467+12
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4320
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L4320:
	mov	r6, #0
	mov	r0, r4
	strb	r6, [r4]
	bl	MVC_DecVDM
	cmp	r0, r6
	beq	.L4437
.L4452:
	ldr	ip, [r5]
	cmp	ip, r6
	beq	.L4436
	ldr	r0, [r4, #120]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4306
.L4279:
	mov	r3, r6
	ldr	r1, .L4467+68
	mov	r0, #22
	bl	dprint_vfmw
	cmp	r6, #256
	beq	.L4321
	ldr	r1, .L4467+72
	mov	r0, #1
	bl	dprint_vfmw
.L4321:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4322
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4322:
	ldr	r3, .L4467+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4437
.L4441:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L4437
.L4457:
	ldr	r1, .L4467+76
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, [r4, #68]
	b	.L4278
.L4458:
	cmp	ip, #0
	movne	r2, #1
	moveq	r2, #5
	strb	r2, [r3, #2]
	ldr	r3, [r4, #232]
	b	.L4276
.L4461:
	bl	MVC_ClearCurrSlice
	ldr	r3, .L4467+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4436
.L4305:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4306
.L4464:
	ldr	r5, .L4467+12
	mov	r0, #1
	ldr	r1, .L4467+80
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4436
.L4454:
	sub	r2, fp, #36
	mvn	r6, #0
	mov	r3, #4
	mov	r1, #100
	str	r6, [r2, #-4]!
	ldr	r0, [r4, #120]
	blx	ip
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4305
	b	.L4436
.L4465:
	ldr	r1, .L4467+84
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4467+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4436
	sub	r2, fp, #36
	ldr	r0, [r4, #120]
	mov	r3, #4
	mvn	r4, #0
	mov	r1, #100
	str	r4, [r2, #-4]!
	blx	ip
	mov	r0, r4
	b	.L4306
.L4463:
	ldr	r5, .L4467+12
	mov	r0, #1
	ldr	r1, .L4467+88
	bl	dprint_vfmw
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4454
	b	.L4436
.L4462:
	ldr	r1, .L4467+92
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, .L4467+12
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L4441
	b	.L4437
.L4460:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq	.L4295
	add	r3, r4, #10747904
	add	r3, r3, #20480
	ldr	r2, [r3, #2384]
	cmp	r2, #0
	beq	.L4298
	ldrb	r2, [r3, #2380]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L4299
.L4298:
	add	r3, r4, #12992
	mov	r2, #0
	add	r3, r3, #16
.L4302:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L4300
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L4466
.L4300:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L4302
	mvn	r3, #0
.L4435:
	cmn	r3, #1
	str	r3, [r0, #2640]
	beq	.L4295
	sub	ip, ip, #5
	mov	r3, #1
	clz	ip, ip
	strb	r3, [r0, #1011]
	strb	r3, [r0, #1010]
	mov	ip, ip, lsr #5
	strb	ip, [r0, #1009]
	b	.L4295
.L4459:
	strb	lr, [r0, #1011]
	ldr	r3, [r2, #712]
	str	r3, [r0, #2640]
	ldrb	r3, [r2, #708]	@ zero_extendqisi2
	strb	r3, [r0, #1009]
	ldrb	r3, [r2, #709]	@ zero_extendqisi2
	strb	r3, [r0, #1010]
	strb	r1, [r2, #704]
	b	.L4295
.L4466:
	movw	r3, #8500
	movt	r3, 5
	mla	r3, r3, r2, r4
	add	r3, r3, #12992
	add	r3, r3, #16
	ldr	r3, [r3, #4]
	b	.L4435
.L4299:
	ldr	r3, [r3, #2388]
	b	.L4435
.L4468:
	.align	2
.L4467:
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
	beq	.L4532
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
	beq	.L4472
	ldr	r3, [r2, #832]
	cmp	r3, #1
	addne	r5, r0, #11141120
	addne	r5, r5, #16384
	beq	.L4533
.L4473:
	mov	r0, r4
	bl	MVC_ReceivePacket
	ldr	r3, [r4, #232]
	str	r3, [r5, #2004]
.L4475:
	cmp	r0, #0
	mov	r3, #0
	strb	r3, [r4, #5]
	beq	.L4476
.L4495:
	mov	r0, #0
.L4527:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L4533:
	ldr	r3, [r2, #872]
	add	r5, r0, #11141120
	add	r5, r5, #16384
	cmp	r3, #1
	bne	.L4473
	ldr	r3, [r5, #2000]
	cmp	r3, #1
	bne	.L4473
	ldr	r3, [r5, #2004]
	str	r3, [r0, #232]
	str	ip, [r5, #2000]
	strb	ip, [r0, #5]
	b	.L4474
.L4476:
	ldr	r3, [r4, #232]
.L4474:
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
	beq	.L4534
.L4530:
	and	r5, r5, #27
	mov	r3, #0
	cmp	r5, #1
	strb	r3, [r4, #10]
	beq	.L4481
.L4482:
	mov	r0, r4
	bl	MVC_FindTrailZeros
	cmp	r0, #0
	bne	.L4535
.L4494:
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
	beq	.L4527
	cmp	r0, r5
	beq	.L4495
	ldr	r3, .L4539
	ldr	ip, [r3]
	cmp	ip, r5
	beq	.L4531
	ldr	r0, [r4, #120]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L4531:
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L4472:
	ldr	r0, [r0, #232]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	b	.L4475
.L4534:
	mov	r1, #24
	mov	r0, r6
	bl	BsGet
	ldr	r3, [r4, #232]
	ubfx	r2, r0, #23, #1
	strb	r2, [r3, #5]
	ldr	r1, [r4, #232]
	ldrsb	r2, [r1, #5]
	cmp	r2, #0
	bne	.L4530
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
.L4481:
	sub	r2, fp, #32
	sub	r1, fp, #36
	mov	r0, r4
	bl	MVC_InquireSliceProperty
	cmp	r0, #0
	bne	.L4483
	ldr	r3, [fp, #-36]
	ldr	r2, [r4, #128]
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #0
	add	r3, r2, r3
	str	r3, [r4, #128]
	bne	.L4536
.L4484:
	ldr	r3, [fp, #-32]
	cmp	r3, #1
	bne	.L4482
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	cmp	r0, #0
	bne	.L4537
.L4491:
	ldr	r0, [r4, #120]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L4482
	sub	r3, fp, #40
	sub	r2, fp, #44
	sub	r1, fp, #48
	ldr	r0, [r4, #120]
	bl	VCTRL_GetChanImgNum
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	bne	.L4482
	ldr	r3, [fp, #-40]
	cmp	r3, #0
	ble	.L4538
.L4488:
	mov	r3, #1
	mvn	r0, #1
	strb	r3, [r4, #5]
	b	.L4527
.L4483:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4493
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4493:
	ldr	r3, .L4539
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4531
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4527
.L4535:
	ldr	r1, .L4539+4
	mov	r0, #22
	bl	dprint_vfmw
	b	.L4494
.L4536:
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
	beq	.L4484
	ldr	r3, [r4, #232]
	mov	ip, #0
	mov	r0, r6
	mov	r2, #12
	ldr	r1, .L4539+8
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
	b	.L4488
.L4537:
	mov	r3, #0
	movw	r2, #14134
	ldr	r1, .L4539+12
	mov	r0, #22
	bl	dprint_vfmw
	b	.L4491
.L4532:
	mov	r0, r1
	movw	r3, #14017
	ldr	r2, .L4539+16
	ldr	r1, .L4539+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L4527
.L4538:
	ldr	r3, [fp, #-44]
	cmp	r3, #32
	bgt	.L4488
	ldr	r3, [r4, #228]
	cmp	r3, #0
	bne	.L4488
	ldr	r3, [r4, #88]
	cmp	r3, #0
	beq	.L4482
	ldr	r1, [r4, #12]
	cmp	r1, #0
	beq	.L4482
	add	r5, r4, #11141120
	ldr	lr, [r4, #16]
	add	r5, r5, #16384
	ldr	r0, [r5, #2008]
	cmp	lr, r0
	beq	.L4482
	ldr	ip, [r5, #2012]
	cmp	r1, ip
	beq	.L4482
	stmib	sp, {r0, r1, lr}
	mov	r0, r2
	str	ip, [sp]
	ldr	r1, .L4539+24
	bl	dprint_vfmw
	ldr	r2, .L4539
	ldr	r3, [r4, #16]
	ldr	ip, [r2]
	str	r3, [r5, #2008]
	ldr	r3, [r4, #12]
	cmp	ip, #0
	str	r3, [r5, #2012]
	beq	.L4482
	mov	r3, #0
	mov	r1, #120
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
	b	.L4482
.L4540:
	.align	2
.L4539:
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
	bne	.L4542
	add	r6, r0, #11141120
	add	r6, r6, #12288
	str	r1, [r6, #3468]
	bl	MVC_ClearAllSlice
	mov	r0, r4
	bl	MVC_StorePicInDpb
	subs	r5, r0, #0
	bne	.L4553
	mov	r3, #1
	mov	r0, r5
	strb	r3, [r4, #4]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4542:
	add	r3, r0, #11141120
	add	r3, r3, #12288
	str	r1, [r3, #3468]
	bl	MVC_ClearAllSlice
	mov	r0, r4
	bl	MVC_StorePicInDpb
	subs	r2, r0, #0
	bne	.L4554
.L4545:
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r2, r0, #0
	bne	.L4555
.L4546:
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
.L4553:
	ldr	r3, [r6, #3416]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	mov	r2, r5
	ldr	r1, .L4556
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
.L4555:
	ldr	r1, .L4556+4
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4546
.L4554:
	ldr	r1, .L4556+8
	mov	r0, #1
	bl	dprint_vfmw
	b	.L4545
.L4557:
	.align	2
.L4556:
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
	beq	.L4574
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
.L4566:
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4561
	cmp	ip, #0
	beq	.L4562
	ldr	lr, [r8, #2856]
	cmp	lr, #0
	beq	.L4562
	ldr	r2, [r8, #2664]
	rsb	r3, ip, r2
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L4561
	mov	r1, r9
	b	.L4563
.L4564:
	ldr	r2, [r1, #4]!
	rsb	r0, ip, r2
	cmp	r2, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L4561
.L4563:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L4564
.L4562:
	ldr	r0, [r5, #120]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L4574
	ldr	r0, [r5, #120]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	bne	.L4594
	add	r1, r5, #584
	ldr	r0, [r5, #120]
	bl	FSP_ClearNotInVoQueue
	b	.L4594
.L4561:
	add	r6, r6, #1
	add	ip, ip, #808
	cmp	r6, r7
	bne	.L4566
	sub	r2, fp, #40
	sub	r1, fp, #44
	add	r0, r5, #584
	bl	GetQueueImgNum
	ldr	r1, .L4598
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r1, .L4598+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	cmp	r3, #0
	beq	.L4596
	movw	r7, #47720
	add	r8, r5, #11075584
	movt	r7, 169
	add	r8, r8, #45056
	add	r7, r5, r7
	mov	r6, #0
.L4567:
	cmp	r4, #0
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r9, [r4, #5]	@ zero_extendqisi2
	beq	.L4578
	ldr	lr, [r8, #2856]
	cmp	lr, #0
	beq	.L4578
	ldr	r2, [r8, #2664]
	rsb	r1, r4, r2
	cmp	r2, #0
	clz	r1, r1
	mov	r1, r1, lsr #5
	moveq	r1, #0
	cmp	r1, #0
	bne	.L4580
	mov	r2, r7
	b	.L4571
.L4572:
	ldr	ip, [r2, #4]!
	rsb	r0, r4, ip
	cmp	ip, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L4580
.L4571:
	add	r1, r1, #1
	cmp	r1, lr
	bne	.L4572
.L4578:
	mov	r2, #1
.L4570:
	str	r2, [sp, #4]
	mov	r0, #0
	mov	r2, r6
	str	r9, [sp]
	ldr	r1, .L4598+8
	add	r6, r6, #1
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	add	r4, r4, #808
	cmp	r3, r6
	bhi	.L4567
.L4573:
	ldr	r3, [fp, #-40]
	mov	r0, #0
	ldr	r2, [fp, #-44]
	ldr	r1, .L4598+12
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	ldr	r0, [r8, #2860]
	ldr	r2, [fp, #-44]
	sub	r3, r3, #2
	ldr	r1, [fp, #-40]
	rsb	r3, r0, r3
	add	r2, r2, r1
	cmp	r2, r3
	blt	.L4597
.L4594:
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4574:
	mov	r0, #1
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4580:
	mov	r2, #0
	b	.L4570
.L4597:
	ldr	r1, .L4598+16
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r5
	mov	r1, #1
	bl	MVC_ClearAll
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L4596:
	add	r8, r5, #11075584
	add	r8, r8, #45056
	b	.L4573
.L4599:
	.align	2
.L4598:
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
	.type	__FUNCTION__.15400, %object
	.size	__FUNCTION__.15400, 18
__FUNCTION__.15400:
	.ascii	"MVC_WriteSliceMsg\000"
	.space	2
	.type	MVC_SarTable.15693, %object
	.size	MVC_SarTable.15693, 136
MVC_SarTable.15693:
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
	.type	__func__.15927, %object
	.size	__func__.15927, 11
__func__.15927:
	.ascii	"MVC_DecSEI\000"
	.space	1
	.type	__func__.14580, %object
	.size	__func__.14580, 17
__func__.14580:
	.ascii	"MVC_DirectOutput\000"
	.space	3
	.type	__func__.15160, %object
	.size	__func__.15160, 12
__func__.15160:
	.ascii	"MVC_InitDPB\000"
	.type	__FUNCTION__.14300, %object
	.size	__FUNCTION__.14300, 13
__FUNCTION__.14300:
	.ascii	"MVC_ClearDPB\000"
	.space	3
	.type	__func__.16038, %object
	.size	__func__.16038, 17
__func__.16038:
	.ascii	"MVC_FlushDecoder\000"
	.space	3
	.type	__FUNCTION__.14308, %object
	.size	__FUNCTION__.14308, 13
__FUNCTION__.14308:
	.ascii	"MVC_ClearAll\000"
	.space	3
	.type	__FUNCTION__.16165, %object
	.size	__FUNCTION__.16165, 13
__FUNCTION__.16165:
	.ascii	"MVC_DEC_Init\000"
	.space	3
	.type	__func__.14730, %object
	.size	__func__.14730, 18
__func__.14730:
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
	.type	pps_tmp.15613, %object
	.size	pps_tmp.15613, 2240
pps_tmp.15613:
	.space	2240
	.type	sps_tmp.15749, %object
	.size	sps_tmp.15749, 3992
sps_tmp.15749:
	.space	3992
	.type	MvcTmpBuf, %object
	.size	MvcTmpBuf, 68
MvcTmpBuf:
	.space	68
	.type	cnt.14566, %object
	.size	cnt.14566, 4
cnt.14566:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
