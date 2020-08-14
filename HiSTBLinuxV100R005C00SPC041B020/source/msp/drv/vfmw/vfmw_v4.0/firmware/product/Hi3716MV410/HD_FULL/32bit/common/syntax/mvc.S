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
	ldr	r2, [ip, #2376]
	mov	r3, r3, lsr #1
	add	r2, r2, lr, lsr #1
	add	ip, r2, #1
	cmp	r3, ip
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
	ldrhi	r3, [r0, #136]
	ldrls	r3, [r0, #136]
	addhi	r3, r3, #4
	addls	r3, r3, #2
	str	r3, [r0, #136]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MVC_SetFrmRepeatCount.part.1, .-MVC_SetFrmRepeatCount.part.1
	.align	2
	.type	MVC_CombinePacket.part.10, %function
MVC_CombinePacket.part.10:
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
	ldr	r6, .L33
	mov	r4, r0
	ldr	r1, .L33+4
	mov	r0, #7
	mov	r5, #0
	ldr	r3, [r6, #68]
	blx	r3
	ldr	ip, [r4, #232]
	ldr	r7, [r6, #68]
	mov	r0, #7
	ldr	r1, .L33+8
	ldr	lr, [ip, #40]
	ldr	r3, [ip, #44]
	ldr	r2, [ip, #16]
	str	lr, [sp, #4]
	ldr	ip, [ip, #12]
	str	ip, [sp]
	blx	r7
	ldr	r3, [r4, #232]
	ldr	ip, [r6, #52]
	ldr	r2, [r3, #12]
	ldr	r0, [r3, #36]
	ldr	r1, [r3, #8]
	rsb	r0, r2, r0
	blx	ip
	ldr	r3, [r4, #232]
	mov	ip, #1
	ldr	r1, .L33+12
	mov	r0, #7
	ldr	r2, [r3, #60]
	ldr	r7, [r3, #32]
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
	ldr	ip, [r6, #68]
	ldr	r3, [r2, #12]
	ldr	r2, [r2, #16]
	blx	ip
	mov	r1, r7
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #232]
	str	r5, [r3, #36]
	ldr	r3, [r4, #232]
	str	r5, [r3, #52]
	ldr	r3, [r4, #232]
	str	r5, [r3, #40]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L34:
	.align	2
.L33:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	.word	.LC1
	.word	.LC2
	UNWIND(.fnend)
	.size	MVC_CombinePacket.part.10, .-MVC_CombinePacket.part.10
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
	ldr	ip, .L41
	mov	r3, r5
	ldr	r1, .L41+4
	mov	r0, #21
	ldr	lr, [r6, #64]
	add	r4, lr, r8
	str	r4, [r6, #64]
	ldr	ip, [ip, #68]
	blx	ip
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
	.word	vfmw_Osal_Func_Ptr_S
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
	bhi	.L44
	mov	r4, r0, asl #1
	mov	r0, r8
	rsb	r3, r4, #31
	add	r4, r4, #1
	mov	r3, r5, lsr r3
	and	r2, r3, #1
	mov	r1, r4
	rsb	r5, r2, #0
	eor	r5, r5, r3, lsr #1
	add	r5, r5, r2
	bl	BsSkip
.L45:
	ldr	lr, [r6, #232]
	mov	r2, r7
	ldr	r6, .L48
	mov	r3, r5
	ldr	r1, .L48+4
	mov	r0, #21
	ldr	ip, [lr, #64]
	add	r4, ip, r4
	str	r4, [lr, #64]
	ldr	ip, [r6, #68]
	blx	ip
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L44:
	cmp	r0, #31
	bls	.L47
	mov	r3, #1
	mvn	r5, #-2147483648
	mov	r4, #32
	strb	r3, [r6, #10]
	b	.L45
.L47:
	mov	r1, r0
	mov	r0, r8
	bl	BsSkip
	mov	r1, r4
	mov	r0, r8
	mov	r4, r4, asl #1
	bl	BsGet
	mov	r1, #1
	add	r4, r4, #1
	mov	r9, r0
	mov	r0, r8
	bl	BsGet
	and	r0, r0, #1
	rsb	r5, r0, #0
	eor	r9, r9, r5
	add	r5, r9, r0
	b	.L45
.L49:
	.align	2
.L48:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r5, .L51
	mov	r2, r7
	ldr	r1, .L51+4
	ldr	ip, [lr, #64]
	add	ip, ip, r6
	str	ip, [lr, #64]
	ldr	ip, [r5, #68]
	mov	r4, r0
	mov	r3, r0
	mov	r0, #21
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L52:
	.align	2
.L51:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r5, .L54
	mov	r2, r6
	ldr	r1, .L54+4
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
.L55:
	.align	2
.L54:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r3, [r10, #2376]
	mov	r7, r1
	ldr	r2, [r10, #2380]
	cmp	r2, r3
	strhi	r3, [r10, #2380]
	cmp	r3, #0
	beq	.L63
	movw	r5, #47236
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
	ldr	r3, [r10, #2376]
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
	ldr	r3, [r10, #2376]
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
	ldrb	r0, [r0, #649]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L88:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L96:
	ldrb	r2, [r0, #613]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L84
.L89:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L95:
	ldrb	r3, [r0, #577]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L89
	ldrb	r3, [r0, #613]	@ zero_extendqisi2
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
	ldr	r0, [r0, #648]
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
	ldr	r2, [r0, #612]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #65536
	bne	.L101
.L106:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L112:
	ldr	r3, [r0, #576]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #65536
	beq	.L106
	ldr	r3, [r0, #612]
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
	ldr	r0, [r0, #648]
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
	ldr	r2, [r0, #612]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #256
	bne	.L118
.L123:
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L129:
	ldr	r3, [r0, #576]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cmp	r3, #256
	beq	.L123
	ldr	r3, [r0, #612]
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
	ldr	r10, [r3, #2376]
	cmp	r10, #0
	beq	.L132
	movw	r8, #47236
	add	r9, r0, #11141120
	movt	r8, 169
	mov	r4, #0
	add	r9, r9, #8192
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
	ldr	r2, [r9, #2192]
	ldr	r1, [r5, #56]
	cmp	r1, r2
	movweq	r2, #28226
	movteq	r2, 42
	addeq	r2, r4, r2
	addeq	r4, r4, #1
	streq	r5, [r7, r2, asl #2]
.L133:
	cmp	r6, r10
	bne	.L134
	ldr	r3, [fp, #-48]
	cmp	r4, r10
	str	r4, [r3, #2388]
	bcs	.L131
	movw	r0, #28225
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
	str	r10, [r3, #2388]
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
	ldr	r10, [r3, #2376]
	cmp	r10, #0
	beq	.L148
	movw	r8, #47236
	add	r9, r0, #11141120
	movt	r8, 169
	mov	r4, #0
	add	r9, r9, #8192
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
	ldr	r2, [r9, #2192]
	ldr	r1, [r5, #56]
	cmp	r1, r2
	movweq	r2, #28210
	movteq	r2, 42
	addeq	r2, r4, r2
	addeq	r4, r4, #1
	streq	r5, [r7, r2, asl #2]
.L149:
	cmp	r6, r10
	bne	.L150
	ldr	r3, [fp, #-48]
	cmp	r4, r10
	str	r4, [r3, #2384]
	bcs	.L147
	movw	r0, #28209
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
	str	r10, [r3, #2384]
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
	ldr	r3, [r0, #528]
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
	strb	ip, [r3, #613]
	and	r1, r1, #2
	strb	ip, [r3, #614]
	strb	r1, [r3, #3]
.L167:
	tst	r2, #2
	beq	.L168
	ldrb	r1, [r3, #3]	@ zero_extendqisi2
	mov	ip, #0
	strb	ip, [r3, #649]
	and	r1, r1, #1
	strb	ip, [r3, #650]
	strb	r1, [r3, #3]
.L168:
	cmp	r2, #3
	ldrsb	r1, [r3, #6]
	moveq	r2, #0
	streqb	r2, [r3, #578]
	streqb	r2, [r3, #577]
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
	ldr	r4, [r3, #2388]
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, fp, sp, pc}
	movw	ip, #47364
	add	r6, r0, #11141120
	movt	ip, 169
	add	r6, r6, #8192
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
	ldr	lr, [r6, #2192]
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
	ldr	r6, [ip, #2388]
	cmp	r6, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	movw	r4, #47364
	add	r8, r0, #11141120
	movt	r4, 169
	add	r8, r8, #8192
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
	ldr	r5, [r8, #2192]
	cmp	r7, r5
	bne	.L187
	cmp	r2, #1
	ldreqb	r5, [lr, #649]	@ zero_extendqisi2
	beq	.L202
	cmp	r2, #2
	bne	.L187
	ldrb	r5, [lr, #613]	@ zero_extendqisi2
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
	ldr	r1, [r1, #528]
	add	ip, ip, #45056
	sub	r1, r1, #1
	ldr	r4, [ip, #2384]
	rsb	r2, r2, r1
	cmp	r4, #0
	beq	.L234
	movw	ip, #47300
	add	r5, r0, #11141120
	movt	ip, 169
	add	r5, r5, #8192
	add	ip, r0, ip
	b	.L211
.L210:
	cmp	r3, r4
	beq	.L235
.L211:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #588]
	cmp	lr, r2
	bne	.L210
	ldrb	lr, [r1, #3]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L210
	ldr	lr, [r1, #576]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L210
	ldr	r6, [r1, #56]
	ldr	lr, [r5, #2192]
	cmp	r6, lr
	bne	.L210
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	MVC_UnMarkFrameStoreRef
.L205:
	add	r3, r0, #11075584
	ldr	r1, [r1, #528]
	add	r3, r3, #45056
	ldr	r5, [r3, #2384]
	rsb	r2, r2, r1, lsl #1
	cmp	r5, #0
	beq	.L236
	movw	r4, #47300
	add	r7, r0, #11141120
	movt	r4, 169
	add	r7, r7, #8192
	add	r4, r0, r4
	mov	ip, #0
	b	.L216
.L212:
	tst	r3, #2
	beq	.L214
	ldr	r3, [r1, #648]
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
	ldr	lr, [r1, #612]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L212
	ldr	r8, [r1, #56]
	ldr	lr, [r7, #2192]
	cmp	r8, lr
	bne	.L212
	ldr	lr, [r1, #624]
	cmp	lr, r2
	bne	.L212
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #11075584
	strb	r3, [r1, #3]
	add	r2, r2, #45056
	mov	r1, #0
	ldr	r3, [r2, #2248]
	strb	r1, [r3, #614]
	ldr	r3, [r2, #2248]
	ldrb	ip, [r3, #576]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #578]
	ldreq	r3, [r2, #2248]
	streqb	r1, [r3, #577]
	ldreq	r3, [r2, #2248]
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
	ldr	r3, [r7, #2192]
	cmp	lr, r3
	bne	.L214
	ldr	r3, [r1, #660]
	cmp	r3, r2
	bne	.L214
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #3]
	add	r3, r3, #11075584
	mov	r2, #0
	add	r3, r3, #45056
	ldr	r1, [r3, #2248]
	strb	r2, [r1, #650]
	ldr	r1, [r3, #2248]
	ldrb	ip, [r1, #576]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r2
	streqb	r2, [r1, #578]
	ldreq	r1, [r3, #2248]
	streqb	r2, [r1, #577]
	ldreq	r1, [r3, #2248]
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
	ldr	r5, [r3, #2388]
	cmp	r5, #0
	beq	.L269
	movw	r4, #47364
	add	r7, r0, #11141120
	movt	r4, 169
	add	r7, r7, #8192
	add	r4, r0, r4
	mov	ip, #0
	b	.L249
.L245:
	tst	r3, #2
	beq	.L247
	ldr	r3, [r1, #648]
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
	ldr	lr, [r1, #612]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L245
	ldr	r8, [r1, #56]
	ldr	lr, [r7, #2192]
	cmp	r8, lr
	bne	.L245
	ldr	lr, [r1, #620]
	cmp	r2, lr
	bne	.L245
	add	r2, r0, ip, lsl #2
	and	r3, r3, #2
	add	r2, r2, #11075584
	strb	r3, [r1, #3]
	add	r2, r2, #45056
	mov	r1, #0
	ldr	r3, [r2, #2312]
	strb	r1, [r3, #613]
	ldr	r3, [r2, #2312]
	ldrb	ip, [r3, #576]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r1
	streqb	r1, [r3, #578]
	ldreq	r3, [r2, #2312]
	streqb	r1, [r3, #577]
	ldreq	r3, [r2, #2312]
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
	ldr	r4, [r1, #2388]
	cmp	r4, #0
	beq	.L272
	movw	ip, #47364
	add	r5, r0, #11141120
	movt	ip, 169
	add	r5, r5, #8192
	add	ip, r0, ip
	b	.L251
.L250:
	cmp	r3, r4
	beq	.L273
.L251:
	ldr	r1, [ip, #4]!
	add	r3, r3, #1
	ldr	lr, [r1, #584]
	cmp	lr, r2
	bne	.L250
	ldrb	lr, [r1, #3]	@ zero_extendqisi2
	cmp	lr, #3
	bne	.L250
	ldr	lr, [r1, #576]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #256
	bne	.L250
	ldr	r6, [r1, #56]
	ldr	lr, [r5, #2192]
	cmp	r6, lr
	bne	.L250
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	b	MVC_UnMarkFrameStoreRef
.L270:
	ldr	lr, [r1, #56]
	ldr	r3, [r7, #2192]
	cmp	lr, r3
	bne	.L247
	ldr	r3, [r1, #656]
	cmp	r2, r3
	bne	.L247
	add	r3, r0, ip, lsl #2
	strb	r6, [r1, #3]
	add	r3, r3, #11075584
	mov	r2, #0
	add	r3, r3, #45056
	ldr	r1, [r3, #2312]
	strb	r2, [r1, #649]
	ldr	r1, [r3, #2312]
	ldrb	ip, [r1, #576]	@ zero_extendqisi2
	cmp	ip, #3
	cmpne	ip, r2
	streqb	r2, [r1, #578]
	ldreq	r1, [r3, #2312]
	streqb	r2, [r1, #577]
	ldreq	r1, [r3, #2312]
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
	ldr	r7, [r3, #2384]
	cmp	r7, #0
	beq	.L304
	movw	r5, #47300
	add	r8, r0, #11141120
	movt	r5, 169
	add	r8, r8, #8192
	add	r5, r0, r5
	mov	ip, #0
	b	.L289
.L281:
	tst	r4, #2
	beq	.L284
	ldr	lr, [r3, #648]
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
	ldr	lr, [r3, #612]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L281
	ldr	lr, [r3, #624]
	cmp	lr, r2
	bne	.L281
	ldr	r9, [r3, #56]
	ldr	lr, [r8, #2192]
	cmp	r9, lr
	bne	.L281
	ldrb	lr, [r3, #649]	@ zero_extendqisi2
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
	ldr	r2, [r3, #2248]
	mov	r6, r6, lsr #5
	add	r1, r6, r1, lsl #1
	str	r1, [r2, #620]
	ldr	r2, [r3, #2248]
	strb	ip, [r2, #613]
	ldr	r2, [r3, #2248]
	strb	r0, [r2, #614]
	ldr	r2, [r3, #2248]
	ldrb	r1, [r2, #576]	@ zero_extendqisi2
	cmp	r1, #3
	cmpne	r1, r0
	bne	.L307
	ldrb	r1, [r2, #649]	@ zero_extendqisi2
	ldrb	r0, [r2, #613]	@ zero_extendqisi2
	cmp	r1, r0
	streqb	r1, [r2, #577]
	ldreq	r3, [r3, #2248]
	ldreqb	r2, [r3, #614]	@ zero_extendqisi2
	streqb	r2, [r3, #578]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L275:
	ldr	r4, [r3, #2384]
	cmp	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	movw	ip, #47300
	add	r5, r0, #11141120
	movt	ip, 169
	add	r5, r5, #8192
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
	ldr	lr, [r3, #576]
	bic	lr, lr, #-16777216
	bic	lr, lr, #255
	cmp	lr, #65536
	bne	.L279
	ldr	lr, [r3, #588]
	cmp	lr, r2
	bne	.L279
	ldr	r7, [r3, #56]
	ldr	lr, [r5, #2192]
	cmp	r7, lr
	bne	.L279
	add	r6, r0, r6, lsl #2
	str	r1, [r3, #28]
	add	r3, r6, #11075584
	mov	r0, #1
	add	r3, r3, #45056
	mov	r2, #0
	ldr	ip, [r3, #2248]
	str	r1, [ip, #584]
	ldr	r1, [r3, #2248]
	strb	r0, [r1, #577]
	ldr	r1, [r3, #2248]
	strb	r0, [r1, #613]
	ldr	r1, [r3, #2248]
	strb	r0, [r1, #649]
	ldr	r1, [r3, #2248]
	strb	r2, [r1, #578]
	ldr	r1, [r3, #2248]
	strb	r2, [r1, #614]
	ldr	r3, [r3, #2248]
	strb	r2, [r3, #650]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L305:
	ldr	lr, [r3, #660]
	cmp	lr, r2
	bne	.L284
	ldr	r4, [r3, #56]
	ldr	lr, [r8, #2192]
	cmp	r4, lr
	bne	.L284
	ldrb	lr, [r3, #613]	@ zero_extendqisi2
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
	ldr	r2, [r3, #2248]
	mov	r6, r6, lsr #5
	add	r1, r6, r1, lsl #1
	str	r1, [r2, #656]
	ldr	r2, [r3, #2248]
	strb	ip, [r2, #649]
	ldr	r2, [r3, #2248]
	strb	r0, [r2, #650]
	ldr	r2, [r3, #2248]
	ldrb	r1, [r2, #576]	@ zero_extendqisi2
	cmp	r1, #3
	cmpne	r1, r0
	bne	.L309
	ldrb	r1, [r2, #649]	@ zero_extendqisi2
	ldrb	r0, [r2, #613]	@ zero_extendqisi2
	cmp	r1, r0
	streqb	r1, [r2, #577]
	ldreq	r3, [r3, #2248]
	ldreqb	r2, [r3, #650]	@ zero_extendqisi2
	streqb	r2, [r3, #578]
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
	ldr	r5, [r6, #528]
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
	ldr	r5, [r6, #528]
	add	r3, r3, #45056
	ldr	r1, [r3, #2384]
	rsb	r5, r2, r5, lsl #1
	cmp	r1, #0
	beq	.L312
	movw	r2, #47300
	add	r0, r0, #11141120
	movt	r2, 169
	add	r0, r0, #8192
	add	r2, r7, r2
	mov	lr, #0
	b	.L315
.L313:
	tst	r3, #2
	beq	.L314
	ldr	r3, [ip, #648]
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
	ldr	r4, [ip, #612]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	bne	.L313
	ldr	r4, [ip, #624]
	cmp	r4, r5
	bne	.L313
	ldr	r9, [ip, #56]
	ldr	r4, [r0, #2192]
	cmp	r9, r4
	bne	.L313
	mov	r1, ip
	mov	r3, r8
	mov	r2, #1
	mov	r0, r7
	bl	MVC_UnMarkLTFldByFrmIdx
	b	.L312
.L326:
	ldr	r3, [ip, #660]
	cmp	r3, r5
	bne	.L314
	ldr	r4, [ip, #56]
	ldr	r3, [r0, #2192]
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
	ldr	r3, [r5, #2388]
	str	r1, [r5, #2392]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	movw	r6, #47364
	add	r7, r0, #11141120
	movt	r6, 169
	add	r7, r7, #8192
	add	r6, r0, r6
	mov	r3, r1
	mov	r4, #0
	b	.L331
.L329:
	ldr	r3, [r5, #2388]
	cmp	r3, r4
	ldmlsfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, [r5, #2392]
.L331:
	ldr	r1, [r6, #4]!
	add	r4, r4, #1
	ldr	r2, [r1, #28]
	cmp	r2, r3
	bcc	.L329
	ldr	r2, [r1, #56]
	ldr	r3, [r7, #2192]
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
	ldr	r3, [r6, #2384]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	movw	r5, #47300
	add	r7, r0, #11141120
	movt	r5, 169
	add	r7, r7, #8192
	add	r5, r0, r5
	mov	r4, #0
	b	.L335
.L334:
	ldr	r3, [r6, #2384]
	cmp	r3, r4
	bls	.L338
.L335:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	ldr	r3, [r7, #2192]
	ldr	r2, [r1, #56]
	cmp	r2, r3
	bne	.L334
	mov	r0, r8
	bl	MVC_UnMarkFrameStoreRef
	ldr	r3, [r6, #2384]
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
	ldr	r1, [r1, #520]
	bl	MVC_UnMarkLTFldByFrmIdx
.L341:
	mov	r2, #1
	mov	r3, #0
	str	r5, [r4, #532]
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
	ldr	r2, [r7, #2184]
	cmp	r2, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r1, [r2, #52]
	movw	r3, #47448
	add	r6, r0, #11075584
	movt	r3, 169
	add	r1, r0, r1
	add	r0, r6, #45056
	add	r3, r1, r3
	mov	r1, #0
	strb	r1, [r3, #4]
	ldr	r3, [r0, #2608]
	cmp	r3, r1
	subne	r3, r3, #1
	str	r3, [r0, #2608]
	ldrsb	r1, [r2, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L346
	ldrsb	r3, [r0, #1]
	cmp	r3, #3
	beq	.L347
	ldr	r3, [r7, #2184]
	mov	r2, #0
	ldr	r0, [r5, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
.L347:
	ldr	r3, [r7, #2184]
	mov	r2, #0
	ldr	r0, [r5, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
.L346:
	add	r1, r4, #45056
	mov	r3, #0
	add	r6, r6, #45056
	ldr	r2, [r1, #2184]
	strb	r3, [r2, #2]
	ldr	r2, [r1, #2184]
	strb	r3, [r2, #5]
	ldr	r2, [r1, #2184]
	strb	r3, [r2, #3]
	str	r3, [r1, #2184]
	ldr	r3, [r6, #2380]
	sub	r3, r3, #1
	str	r3, [r6, #2380]
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
	ldr	r6, [r3, #2376]
	cmp	r6, #0
	beq	.L375
	movw	r5, #47236
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
	ldr	r2, [r0, #208]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	movls	r0, #0
	bhi	.L404
.L398:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L400:
	ldr	ip, .L405
	mov	r0, #1
	ldrsb	r2, [r4, #6]
	ldr	r1, .L405+4
	ldr	ip, [ip, #68]
	blx	ip
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
	ldr	ip, .L405
	mov	r2, r3
	ldr	r1, .L405+8
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L398
.L402:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	tst	r3, #1
	beq	.L385
	ldr	ip, .L405
	mov	r0, r2
	ldr	r1, .L405+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L398
.L403:
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	tst	r3, #2
	beq	.L385
	ldr	ip, .L405
	mov	r0, #1
	ldr	r1, .L405+16
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L398
.L390:
	mvn	r0, #0
	b	.L398
.L380:
	ldr	r1, .L405
	mov	r2, r4
	ldrsb	lr, [r4, #6]
	mov	r0, #1
	ldr	r3, [r4, #16]
	ldr	ip, [r1, #68]
	ldr	r1, .L405+20
	str	lr, [sp]
	blx	ip
	b	.L382
.L389:
	ldr	ip, .L405
	mov	r0, #1
	ldr	r1, .L405+24
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L398
.L404:
	ldr	ip, .L405
	mov	r0, #1
	ldr	r1, .L405+28
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L398
.L406:
	.align	2
.L405:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldrd	r2, [r1, #80]
	mvn	r7, #0
	mvn	r6, #0
	cmp	r3, r7
	mov	r8, r1
	cmpeq	r2, r6
	mov	r5, r0
	beq	.L407
	add	r9, r0, #11075584
	add	r9, r9, #45056
	ldr	r3, [r9, #2376]
	cmp	r3, #0
	beq	.L407
	movw	r10, #47236
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
	ldrd	r0, [r3, #80]
	cmp	r1, r7
	cmpeq	r0, r6
	strcc	r4, [fp, #-48]
	movcc	r6, r0
	movcc	r7, r1
.L411:
	ldr	r3, [r9, #2376]
	add	r4, r4, #1
	cmp	r4, r3
	bcc	.L412
	ldr	r3, [fp, #-48]
	cmn	r3, #1
	beq	.L407
	ldrd	r2, [r8, #80]
	cmp	r3, r7
	cmpeq	r2, r6
	bhi	.L426
.L407:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L426:
	ldr	ip, .L427
	mov	r0, #29
	strd	r6, [sp]
	ldr	r1, .L427+4
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r3, [fp, #-48]
	movw	r1, #28194
	movt	r1, 42
	add	r1, r3, r1
	ldrd	r2, [r8, #80]
	ldr	r1, [r5, r1, asl #2]
	strd	r2, [r1, #80]
	strd	r6, [r8, #80]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L428:
	.align	2
.L427:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r3, r0, #11075584
	ldr	r2, [r0, #252]
	add	ip, r3, #40960
	mov	r4, r1
	mov	r5, r0
	mov	r0, #2240
	ldr	r1, [ip, #536]
	movw	r8, #3992
	ldr	lr, [r4, #232]
	ldr	r7, [r5, #248]
	mla	r0, r0, r1, r2
	bic	r1, lr, #768
	bic	r1, r1, #3
	ldrb	r6, [r4, #2]	@ zero_extendqisi2
	ldr	r2, [r0, #28]
	str	r1, [r4, #232]
	ldr	r0, [r5, #56]
	mla	r2, r8, r2, r7
	mov	r0, r0, asl #10
	str	r0, [r4, #260]
	cmp	r6, #3
	ldrls	pc, [pc, r6, asl #2]
	b	.L467
.L432:
	.word	.L431
	.word	.L433
	.word	.L434
	.word	.L431
.L434:
	ldrb	r0, [r4]	@ zero_extendqisi2
	add	r3, r5, #11075584
	ldr	lr, [r4, #44]
	add	r3, r3, #45056
	orr	r1, r1, #2816
	and	r0, r0, #3
	orr	r1, r1, r0
	str	r1, [r4, #232]
	str	lr, [r4, #272]
.L430:
	ldrb	r1, [r2, #20]	@ zero_extendqisi2
	str	r1, [r4, #556]
	str	r1, [r4, #560]
	ldrb	r1, [r2, #24]	@ zero_extendqisi2
	cmp	r1, #0
	ldrne	r1, [r2, #68]
	mov	r2, #1
	str	r2, [r4, #564]
	str	r1, [r4, #568]
	ldrb	r2, [ip, #531]	@ zero_extendqisi2
	cmp	r2, #1
	movne	r2, #0
	str	r2, [r4, #328]
	ldr	r2, [r5, #224]
	ldr	r2, [r2, #12]
	cmp	r2, #0
	ble	.L452
	ldrb	r2, [r4, #576]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L452
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	cmp	r2, #3
	ldreq	r2, [r4, #232]
	biceq	r2, r2, #768
	streq	r2, [r4, #232]
.L452:
	ldrb	r2, [r3, #2136]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L453
	ldr	r2, [r3, #2152]
	cmp	r2, #3
	moveq	r3, #1
	streq	r3, [r4, #436]
	beq	.L453
	cmp	r2, #4
	moveq	r3, #2
	streq	r3, [r4, #436]
	beq	.L453
	cmp	r2, #5
	moveq	r3, #3
	streq	r3, [r4, #436]
	beq	.L453
	ldr	ip, .L479
	mov	r2, #0
	mov	r0, r2
	str	r2, [r4, #436]
	ldr	r1, .L479+4
	ldr	r2, [r3, #2152]
	ldr	r3, [ip, #68]
	blx	r3
.L453:
	ldr	r0, [r5, #120]
	ldrsb	r1, [r4, #6]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	ldr	r3, .L479
	mov	r2, #504
	add	r1, r4, #72
	ldr	r3, [r3, #52]
	blx	r3
	mov	r3, #16
	str	r3, [r5, #384]
	mov	r2, #1
	ldrb	r3, [r4, #576]	@ zero_extendqisi2
	strb	r2, [r5, #379]
	cmp	r3, #0
	movne	r3, #4
	strb	r3, [r5, #372]
	ldr	r3, [r4, #604]
	strb	r2, [r5, #377]
	str	r3, [r5, #388]
	str	r3, [r5, #396]
	str	r3, [r5, #392]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L433:
	ldrb	r0, [r4]	@ zero_extendqisi2
	add	r3, r5, #11075584
	ldr	lr, [r4, #44]
	orr	r1, r1, #1792
	and	r0, r0, #3
	add	r3, r3, #45056
	orr	r1, r1, r0
	str	r1, [r4, #232]
	str	lr, [r4, #272]
	b	.L430
.L431:
	ldrb	r1, [r4, #576]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L435
	ldr	r1, [r4, #40]
	mov	r7, #3072
	ldr	r0, [r4, #44]
	cmp	r1, #0
	ldrb	r1, [r4]	@ zero_extendqisi2
	moveq	r6, #512
	movne	r6, #768
	str	r0, [r4, #272]
.L436:
	ldr	r8, [r4, #628]
	ldr	r0, [r4, #664]
	cmp	r8, r0
	beq	.L476
	movle	r0, #1
	movgt	r0, #0
.L447:
	add	r3, r3, #45056
	ldrb	r8, [r3, #2172]	@ zero_extendqisi2
	cmp	r8, #3
	moveq	r0, #4096
	beq	.L448
	cmp	r8, #4
	movne	r0, r0, asl #12
	moveq	r0, #0
.L448:
	bic	lr, lr, #13056
	orr	r6, r7, r6
	bic	lr, lr, #3
	and	r1, r1, #3
	orr	lr, r6, lr
	orr	lr, lr, r1
	orr	lr, lr, r0
	str	lr, [r4, #232]
	b	.L430
.L435:
	ldr	r8, [r4, #636]
	ldr	r1, [r5, #116]
	ldr	r0, [r4, #672]
	cmp	r8, r1
	bhi	.L477
	cmp	r1, r0
	bcc	.L440
.L438:
	ldr	r0, [r4, #44]
	mov	r6, #3
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldrb	r10, [r4, #615]	@ zero_extendqisi2
	ldrb	r9, [r4, #651]	@ zero_extendqisi2
	str	r0, [r4, #272]
.L441:
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	cmp	r0, #1
	beq	.L478
	cmp	r0, #2
	streq	r8, [r4, #272]
	movne	r7, r6, asl #10
	moveq	r1, r10
	moveq	r7, #1024
.L443:
	mov	r6, #768
	str	r10, [r4, #224]
	str	r9, [r4, #228]
	b	.L436
.L440:
	ldrb	r1, [r4, #615]	@ zero_extendqisi2
	mov	r6, #1
	ldrb	r9, [r4, #651]	@ zero_extendqisi2
	str	r8, [r4, #272]
	mov	r10, r1
	b	.L441
.L476:
	ldr	r0, [r4, #236]
	cmp	r0, #720
	movne	r0, #1
	bne	.L447
	ldr	r0, [r4, #240]
	subs	r0, r0, #480
	movne	r0, #1
	b	.L447
.L477:
	cmp	r1, r0
	bcc	.L438
	ldrb	r1, [r4, #651]	@ zero_extendqisi2
	mov	r6, #2
	str	r0, [r4, #272]
	ldrb	r10, [r4, #615]	@ zero_extendqisi2
	mov	r9, r1
	b	.L441
.L478:
	ldr	r0, [r4, #672]
	mov	r1, r9
	mov	r7, #2048
	str	r0, [r4, #272]
	b	.L443
.L467:
	add	r3, r5, #11075584
	add	r3, r3, #45056
	b	.L430
.L480:
	.align	2
.L479:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
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
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r1, #84]
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
	ldrb	lr, [r0, #577]	@ zero_extendqisi2
	mov	r5, #1
	ldrb	ip, [r0, #578]	@ zero_extendqisi2
	mov	r4, #2
	ldr	r1, [r0, #600]
	ldrb	r2, [r0, #579]	@ zero_extendqisi2
	ldr	r3, [r0, #580]
	strb	r5, [r0, #612]
	strb	r4, [r0, #648]
	strb	lr, [r0, #613]
	strb	lr, [r0, #649]
	strb	ip, [r0, #614]
	strb	ip, [r0, #650]
	str	r1, [r0, #636]
	str	r1, [r0, #672]
	strb	r2, [r0, #615]
	strb	r2, [r0, #651]
	str	r3, [r0, #616]
	str	r3, [r0, #652]
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
	ldr	r2, [r0, #664]
	mov	lr, #3
	ldr	r1, [r0, #628]
	ldr	r3, [r0, #672]
	cmp	r2, r1
	ldr	r5, [r0, #636]
	ldrb	r4, [r0, #649]	@ zero_extendqisi2
	ldr	ip, [r0, #616]
	movge	r2, r1
	add	r3, r3, r5
	cmp	r4, #0
	strb	lr, [r0, #576]
	mov	r3, r3, lsr #1
	str	ip, [r0, #580]
	str	r2, [r0, #592]
	str	r2, [r0, #32]
	str	r3, [r0, #600]
	beq	.L485
	ldrb	r2, [r0, #613]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L486
.L485:
	ldr	r2, [r0, #648]
	mov	r1, #0
	strb	r1, [r0, #577]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, r1
	beq	.L490
	ldr	r2, [r0, #612]
	bic	r2, r2, #-16777216
	bic	r2, r2, #255
	cmp	r2, #0
	movne	ip, #1
	bne	.L488
.L490:
	mov	ip, #0
.L488:
	ldrb	r2, [r0, #651]	@ zero_extendqisi2
	ldrb	r1, [r0, #615]	@ zero_extendqisi2
	str	r3, [r0, #44]
	cmp	r2, r1
	strb	ip, [r0, #578]
	movcs	r3, r2
	movcc	r3, r1
	strb	r3, [r0, #579]
	strb	r3, [r0]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L486:
	mov	r1, #1
	strb	r1, [r0, #577]
	b	.L490
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
	beq	.L521
	cmp	r2, #1
	beq	.L502
	cmp	r2, #2
	beq	.L503
	ldr	r9, [r6, #628]
	ldr	r10, [r6, #664]
.L504:
	add	r8, r5, #11075584
	add	r7, r8, #45056
	ldr	ip, [r7, #2604]
	cmp	ip, #0
	beq	.L513
	ldrb	r4, [r7, #2396]	@ zero_extendqisi2
	cmp	r4, #0
	movwne	r3, #47452
	movne	r4, #0
	movtne	r3, 169
	addne	r3, r5, r3
	bne	.L507
	b	.L505
.L511:
	ldrb	r2, [r3, #1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L505
.L507:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L511
.L513:
	mvn	r0, #0
.L517:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L503:
	ldr	r9, [r6, #664]
	mov	r10, r9
	b	.L504
.L505:
	ldrsb	r1, [r6, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L508
	ldr	r3, [r0, #520]
	cmp	r3, #0
	beq	.L508
	add	r2, r5, r4
	movw	r3, #47448
	movt	r3, 169
	add	r3, r2, r3
	mov	r2, #1
	add	r5, r5, r4, lsl #2
	strb	r2, [r3, #4]
	add	r5, r5, #11075584
	ldr	r3, [r0, #520]
	add	r5, r5, #45056
	ldr	ip, .L522
	add	r8, r8, #45056
	ldr	r1, .L522+4
	mov	r0, #13
	ldr	r3, [r3, #4]
	str	r9, [r5, #2476]
	str	r10, [r5, #2540]
	str	r3, [r5, #2412]
	ldr	r3, [r7, #2608]
	ldr	ip, [ip, #68]
	cmp	r3, #15
	addls	r2, r3, r2
	mov	r3, r4
	movhi	r2, #16
	str	r2, [r8, #2608]
	ldr	r2, [r6, #268]
	blx	ip
	mov	r0, #0
	str	r4, [r6, #52]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L502:
	ldr	r9, [r6, #628]
	mov	r10, r9
	b	.L504
.L508:
	ldr	r1, .L522
	ldrsb	r2, [r6, #6]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	ldr	ip, [r1, #68]
	mov	r1, #0
	stmia	sp, {r0, r1}
	mov	r0, r1
	ldr	r1, .L522+8
	blx	ip
	mvn	r0, #0
	b	.L517
.L521:
	ldr	ip, .L522
	mov	r0, r6
	movw	r3, #2094
	ldr	r2, .L522+12
	ldr	r1, .L522+16
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L517
.L523:
	.align	2
.L522:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r0, [r3, #2388]
	ldr	r2, [r2, #3944]
	ldr	r1, [r3, #2384]
	rsb	r2, r0, r2
	cmp	r1, r2
	beq	.L549
.L524:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L549:
	ldr	r8, [r3, #2376]
	cmp	r8, #0
	beq	.L529
	movw	r6, #47236
	add	r1, r10, #11141120
	movt	r6, 169
	mov	r2, #0
	add	r1, r1, #8192
	add	r6, r10, r6
	mov	r5, r2
	mvn	r9, #-2147483648
.L528:
	ldr	r4, [r6, #4]!
	cmp	r4, #0
	beq	.L527
	ldr	r7, [r4, #64]
	cmp	r9, r7
	bls	.L527
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	mov	r0, r4
	cmp	r3, #0
	beq	.L527
	str	r1, [fp, #-52]
	str	r2, [fp, #-48]
	bl	MVC_NonLongTermRefFlg
	ldr	r2, [fp, #-48]
	ldr	r1, [fp, #-52]
	cmp	r0, #0
	beq	.L527
	ldr	r0, [r4, #56]
	ldr	r3, [r1, #2192]
	cmp	r0, r3
	moveq	r9, r7
	moveq	r2, r5
.L527:
	add	r5, r5, #1
	cmp	r5, r8
	bne	.L528
.L526:
	movw	r3, #28194
	movt	r3, 42
	add	r3, r2, r3
	ldr	r4, [r10, r3, asl #2]
	cmp	r4, #0
	beq	.L524
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L524
	mov	r0, r4
	bl	MVC_NonLongTermRefFlg
	cmp	r0, #0
	beq	.L524
	add	r3, r10, #11141120
	ldr	r2, [r4, #56]
	add	r3, r3, #8192
	ldr	r3, [r3, #2192]
	cmp	r2, r3
	bne	.L524
	mov	r0, r10
	mov	r1, r4
	bl	MVC_UnMarkFrameStoreRef
	mov	r0, r10
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	MVC_UpdateReflist
.L529:
	mov	r2, r8
	b	.L526
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r3, .L561
	mov	r8, r0
	ldr	r3, [r3]
	tst	r3, #16384
	beq	.L550
	add	r7, r0, #11075584
	add	r7, r7, #45056
	ldr	r3, [r7, #2380]
	cmp	r3, #0
	beq	.L560
	movw	r5, #47236
	ldr	r6, .L561+4
	movt	r5, 169
	add	r5, r0, r5
	mov	r4, #0
.L555:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r9, [r6, #68]
	mov	r0, #14
	ldr	r1, .L561+8
	add	r4, r4, #1
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	ldr	r3, [ip, #20]
	str	lr, [sp, #4]
	ldr	ip, [ip, #32]
	str	ip, [sp]
	blx	r9
	ldr	r3, [r7, #2380]
	cmp	r3, r4
	bhi	.L555
.L556:
	ldr	r3, [r6, #68]
	mov	r0, #14
	ldr	r1, .L561+12
	blx	r3
	ldr	r3, [r7, #2384]
	cmp	r3, #0
	movwne	r5, #47300
	movne	r4, #0
	movtne	r5, 169
	addne	r5, r8, r5
	beq	.L554
.L557:
	ldr	ip, [r5, #4]!
	mov	r2, r4
	ldr	r1, .L561+16
	mov	r0, #14
	ldr	r8, [r6, #68]
	ldrb	lr, [ip, #3]	@ zero_extendqisi2
	ldr	r3, [ip, #20]
	str	lr, [sp, #4]
	ldr	ip, [ip, #32]
	str	ip, [sp]
	blx	r8
	ldr	r3, [r5]
	mov	r2, r4
	ldr	ip, [r6, #68]
	ldr	r1, .L561+20
	mov	r0, #14
	ldr	r3, [r3, #592]
	add	r4, r4, #1
	blx	ip
	ldr	r3, [r7, #2384]
	cmp	r3, r4
	bhi	.L557
.L554:
	ldr	r3, [r6, #68]
	mov	r0, #14
	ldr	r1, .L561+12
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L550:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L560:
	ldr	r6, .L561+4
	b	.L556
.L562:
	.align	2
.L561:
	.word	g_PrintEnable
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r3, r0, #8192
	ldrb	r2, [r3, #1594]	@ zero_extendqisi2
	ldr	r4, [r3, #2112]
	strb	r2, [r4, #1]
	ldrb	r2, [r3, #1596]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
	bne	.L564
	ldrb	r2, [r3, #1597]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r2, #3
.L564:
	strb	r2, [r4, #3]
	ldr	r2, [r3, #2120]
	str	r2, [r4, #20]
	ldr	r2, [r3, #2164]
	str	r2, [r4, #44]
	ldrb	r2, [r3, #1604]	@ zero_extendqisi2
	strb	r2, [r4]
	ldr	r2, [r3, #2132]
	str	r2, [r4, #32]
	ldr	r2, [r3, #2188]
	str	r2, [r4, #48]
	ldr	r2, [r3, #2124]
	str	r2, [r4, #28]
	ldrb	r2, [r3, #1601]	@ zero_extendqisi2
	str	r2, [r4, #40]
	ldrb	r2, [r3, #1599]	@ zero_extendqisi2
	cmp	r2, #1
	moveq	r1, #3
	movne	r1, #0
	strb	r1, [r4, #4]
	ldrb	r2, [r3, #1595]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L567
	bcc	.L568
	cmp	r2, #2
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	str	r4, [r4, #652]
	mov	ip, #1
	strb	ip, [r4, #648]
	mov	r6, #0
	ldrb	ip, [r3, #1596]	@ zero_extendqisi2
	strb	ip, [r4, #649]
	ldrb	ip, [r3, #1597]	@ zero_extendqisi2
	strb	ip, [r4, #650]
	ldr	ip, [r3, #2140]
	str	ip, [r4, #664]
	ldrb	ip, [r3, #1604]	@ zero_extendqisi2
	strb	ip, [r4, #651]
	ldr	lr, [r3, #2164]
	str	r6, [r4, #40]
	str	lr, [r4, #672]
	ldrb	r5, [r3, #1593]	@ zero_extendqisi2
	cmp	r5, r6
	beq	.L585
	ldr	r2, [r4, #636]
	mov	r5, #3
	strb	r5, [r4, #2]
	mvn	r7, #0
	add	lr, lr, r2
	str	lr, [r4, #44]
	str	lr, [r4, #600]
	ldrb	r3, [r3, #1599]	@ zero_extendqisi2
	cmp	r3, #1
	add	r3, r0, #9792
	orreq	r6, r1, #2
	strb	r6, [r4, #4]
	ldrd	r8, [r3, #16]
	mvn	r6, #0
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L576
	ldrd	r2, [r4, #80]
	cmp	r3, r7
	cmpeq	r2, r6
	beq	.L586
.L576:
	ldrb	r3, [r4, #615]	@ zero_extendqisi2
.L583:
	add	r1, r0, #8192
	cmp	r3, ip
	mov	r0, r4
	movcc	r3, ip
	strb	r3, [r4]
	ldr	r3, [r1, #2132]
	str	r3, [r4, #32]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MVC_CombineFldsToFrm
.L568:
	str	r4, [r4, #580]
	mov	r1, #3
	mov	r2, #0
	strb	r1, [r4, #2]
	strb	r2, [r4, #576]
	add	r1, r3, #1600
	ldrb	r5, [r3, #1596]	@ zero_extendqisi2
	mov	r2, #504
	add	r1, r1, #8
	add	r0, r4, #72
	strb	r5, [r4, #577]
	ldrb	r6, [r3, #1597]	@ zero_extendqisi2
	strb	r6, [r4, #578]
	ldr	ip, [r3, #2136]
	str	ip, [r4, #592]
	ldrb	r7, [r3, #1604]	@ zero_extendqisi2
	strb	r7, [r4, #579]
	ldr	r8, [r3, #2164]
	str	r8, [r4, #600]
	ldr	ip, [r3, #2140]
	str	ip, [r4, #628]
	ldr	r3, [r3, #2144]
	str	r3, [r4, #664]
	bl	memcpy
	mov	r2, #1
	mov	r3, #2
	str	r8, [r4, #636]
	str	r8, [r4, #672]
	strb	r7, [r4, #615]
	strb	r7, [r4, #651]
	strb	r6, [r4, #614]
	strb	r6, [r4, #650]
	strb	r5, [r4, #613]
	strb	r5, [r4, #649]
	str	r4, [r4, #616]
	str	r4, [r4, #652]
	strb	r2, [r4, #612]
	strb	r3, [r4, #648]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L567:
	strb	r2, [r4, #612]
	mov	r5, #0
	str	r4, [r4, #616]
	ldrb	ip, [r3, #1596]	@ zero_extendqisi2
	strb	ip, [r4, #613]
	ldrb	ip, [r3, #1597]	@ zero_extendqisi2
	strb	ip, [r4, #614]
	ldr	ip, [r3, #2140]
	str	ip, [r4, #628]
	ldrb	ip, [r3, #1604]	@ zero_extendqisi2
	strb	ip, [r4, #615]
	ldr	lr, [r3, #2164]
	str	r5, [r4, #40]
	str	lr, [r4, #636]
	ldrb	r6, [r3, #1593]	@ zero_extendqisi2
	cmp	r6, r5
	beq	.L587
	ldr	r2, [r4, #672]
	mov	r6, #3
	strb	r6, [r4, #2]
	mvn	r7, #0
	add	lr, lr, r2
	str	lr, [r4, #44]
	str	lr, [r4, #600]
	mvn	r6, #0
	ldrb	r3, [r3, #1599]	@ zero_extendqisi2
	cmp	r3, #1
	add	r3, r0, #9792
	orreq	r5, r1, #1
	strb	r5, [r4, #4]
	ldrd	r8, [r3, #16]
	cmp	r9, r7
	cmpeq	r8, r6
	beq	.L572
	ldrd	r2, [r4, #80]
	cmp	r3, r7
	cmpeq	r2, r6
	streqd	r8, [r4, #80]
.L572:
	ldrb	r3, [r4, #651]	@ zero_extendqisi2
	b	.L583
.L585:
	strb	r2, [r4, #2]
	add	r1, r0, #9792
	str	lr, [r4, #44]
	add	r1, r1, #8
	str	lr, [r4, #600]
	add	r0, r4, #72
	ldrb	r3, [r3, #1599]	@ zero_extendqisi2
	mov	r2, #504
	cmp	r3, #1
	moveq	r5, #2
	strb	r5, [r4, #4]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L587:
	strb	r2, [r4, #2]
	add	r1, r3, #1600
	str	lr, [r4, #44]
	add	r1, r1, #8
	str	lr, [r4, #600]
	add	r0, r4, #72
	ldrb	r3, [r3, #1599]	@ zero_extendqisi2
	mov	r2, #504
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r4, #4]
	bl	memcpy
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L586:
	strd	r8, [r4, #80]
	b	.L576
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
	ldr	r5, [r3, #2376]
	ldr	r3, [r3, #2380]
	cmp	r3, r5
	addcc	r6, r0, #11141120
	bcc	.L589
	cmp	r5, #0
	beq	.L594
	movw	ip, #47236
	add	r6, r0, #11141120
	movt	ip, 169
	mov	r1, #0
	add	r8, r6, #8192
	add	ip, r0, ip
	mov	r3, r1
	mvn	r4, #-2147483648
.L592:
	ldr	r2, [ip, #4]!
	cmp	r2, #0
	beq	.L591
	ldr	lr, [r2, #64]
	cmp	r4, lr
	bls	.L591
	ldr	r0, [r2, #56]
	ldr	r2, [r8, #2192]
	cmp	r0, r2
	moveq	r4, lr
	moveq	r1, r3
.L591:
	add	r3, r3, #1
	cmp	r3, r5
	bne	.L592
	mov	r5, r1
.L590:
	movw	r3, #28194
	mov	r0, r7
	movt	r3, 42
	add	r3, r1, r3
	ldr	r1, [r7, r3, asl #2]
	bl	MVC_UnMarkFrameStoreRef
	mov	r1, r5
	mov	r0, r7
	bl	MVC_RemoveFrameStoreOutDPB
.L589:
	add	r3, r6, #8192
	mov	r1, #0
	ldr	r2, [r3, #2128]
	strb	r1, [r3, #1596]
	cmp	r2, r1
	movne	r2, #1
	strneb	r2, [r3, #1597]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L594:
	add	r6, r0, #11141120
	mov	r1, r5
	b	.L590
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r5, r1, #0
	mov	r6, r0
	beq	.L612
	ldr	r7, .L613
	add	r8, r5, #56
	mov	r4, r5
	mov	r3, #0
	strb	r3, [r5, #1]
	strb	r3, [r5, #3]
.L607:
	ldr	r3, [r4, #8]
	mov	r0, r6
	cmp	r3, #0
	beq	.L606
	ldr	r1, [r4, #32]
	bl	SM_ReleaseStreamSeg
	ldr	r3, [r4, #12]
	ldr	ip, [r7, #68]
	mov	r0, #7
	ldr	r2, [r4, #8]
	ldr	r1, .L613+4
	blx	ip
	mov	r3, #0
	str	r3, [r4, #8]
	str	r3, [r4, #24]
	str	r3, [r4, #12]
.L606:
	add	r4, r4, #28
	cmp	r4, r8
	bne	.L607
	mov	r3, #0
	strb	r3, [r5]
	str	r3, [r5, #68]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L612:
	ldr	ip, .L613
	mov	r0, r5
	movw	r3, #4007
	ldr	r2, .L613+8
	ldr	r1, .L613+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L614:
	.align	2
.L613:
	.word	vfmw_Osal_Func_Ptr_S
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
	beq	.L616
	ldr	r0, [r0, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L616:
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
	ldr	r3, .L625
	mov	r4, r0
	ldr	r1, .L625+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L621
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L621:
	ldr	r3, [r4, #80]
	mov	r0, #0
	add	r3, r3, #1
	str	r3, [r4, #80]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L626:
	.align	2
.L625:
	.word	vfmw_Osal_Func_Ptr_S
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
	b	.L629
.L628:
	add	r4, r4, #88
	cmp	r4, r5
	beq	.L631
.L629:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L628
	mov	r1, r4
	ldr	r0, [r6, #120]
	add	r4, r4, #88
	bl	MVC_ReleaseNAL
	cmp	r4, r5
	bne	.L629
.L631:
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
	movw	r4, #11024
	movw	r5, #11568
	movt	r4, 170
	movt	r5, 170
	add	r4, r0, r4
	add	r5, r0, r5
	mov	r6, r0
	mov	r7, #0
.L634:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L633
	ldr	r0, [r6, #120]
	bl	MVC_ReleaseNAL
	str	r7, [r4]
.L633:
	cmp	r4, r5
	bne	.L634
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
	str	r0, [r3, #2188]
	str	r1, [r3, #2192]
	mov	r0, #2
	mov	r1, #7
	strb	r0, [r3, #2177]
	strb	r1, [r3, #2179]
	mov	r0, #32
	mov	r1, #3
	strb	r0, [r3, #2178]
	str	r1, [r3, #2200]
	mvn	r0, #0
	mov	r1, #262144
	str	r0, [r3, #2204]
	str	r1, [r3, #2216]
	mov	r0, r2
	strb	r2, [r3, #2176]
	str	r2, [r3, #2208]
	str	r2, [r3, #2212]
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	ldr	r7, .L648
	mov	r5, r1
	mov	r6, r2
	mov	r4, r0
	mov	r2, #32
	ldr	r3, [r7, #48]
	sub	r0, fp, #68
	mov	r1, #0
	blx	r3
	ldr	r2, [r4, #224]
	str	r6, [fp, #-40]
	mov	r3, #0
	str	r5, [fp, #-60]
	ldr	r2, [r2, #28]
	str	r6, [fp, #-56]
	cmp	r2, #24
	str	r5, [fp, #-52]
	str	r6, [fp, #-48]
	str	r5, [fp, #-44]
	strb	r3, [fp, #-68]
	strb	r3, [fp, #-67]
	beq	.L641
	add	r1, r4, #11075584
	ldrb	r2, [fp, #4]	@ zero_extendqisi2
	add	r1, r1, #45056
	strb	r3, [fp, #-63]
	mov	r3, #5
	strb	r3, [fp, #-62]
	ldr	r3, [r1, #2376]
	add	r3, r3, #3
	strb	r3, [fp, #-64]
.L645:
	sub	r1, fp, #68
	ldr	r0, [r4, #120]
	strb	r2, [fp, #-61]
	mov	r6, #0
	mov	r5, #1
	strb	r6, [fp, #-65]
	strb	r5, [fp, #-66]
	bl	FSP_ConfigInstance
	subs	r8, r0, #0
	bne	.L642
	ldr	r1, [r4, #224]
	sub	r3, fp, #72
	ldr	r0, [r4, #120]
	ldr	r2, [r1, #24]
	ldr	r1, [r1, #20]
	bl	FSP_ConfigFrameBuf
	cmp	r0, r6
	moveq	r0, r5
	bne	.L647
.L646:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L641:
	mov	r1, #5
	strb	r3, [fp, #-64]
	strb	r3, [fp, #-63]
	mov	r2, #1
	strb	r1, [fp, #-62]
	b	.L645
.L642:
	ldr	r3, [r7, #68]
	mov	r0, r6
	ldr	r1, .L648+4
	blx	r3
	mvn	r0, #19
	b	.L646
.L647:
	ldr	r3, [r7, #68]
	mov	r0, r8
	ldr	r1, .L648+8
	blx	r3
	mvn	r0, #19
	b	.L646
.L649:
	.align	2
.L648:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC24
	.word	.LC23
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
	ldrb	r3, [r3, #520]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L652
	bcc	.L667
	cmp	r3, #2
	bne	.L673
.L668:
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L664:
	mov	r0, r7
.L673:
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L652:
	mov	r0, #2
.L653:
	movw	r6, #41524
	add	r8, r7, #11141120
	movt	r6, 169
	add	r6, r7, r6
	add	r8, r8, #8192
	add	r5, r7, #256
	ldr	r2, [r6, #4]!
	mov	lr, r7
	mov	r4, #0
	mov	r9, #1
	cmp	r2, #0
	beq	.L655
.L675:
	ldr	r10, [lr, #256]
	cmp	r10, #0
	bne	.L669
	mov	ip, r5
	mov	r3, r10
	b	.L657
.L658:
	ldr	r1, [ip, #4]!
	cmp	r1, #0
	bne	.L660
.L657:
	add	r3, r3, #1
	cmp	r3, r2
	bne	.L658
	ldr	r3, [r7, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L664
	ldr	r1, [r8, #2112]
	add	r1, r1, #576
.L660:
	mov	ip, r5
	mov	r3, #0
	b	.L666
.L662:
	ldr	r10, [ip, #4]!
.L666:
	add	r3, r3, #1
	cmp	r10, #0
	streq	r1, [ip]
	cmp	r3, r2
	bne	.L662
.L663:
	add	r4, r4, #1
	add	r5, r5, #132
	cmp	r0, r4
	add	lr, lr, #132
	bls	.L668
	ldr	r2, [r6, #4]!
	cmp	r2, #0
	bne	.L675
.L655:
	ldr	r3, [r7, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L664
	str	r9, [r6]
	ldr	r3, [r8, #2112]
	add	r3, r3, #576
	str	r3, [lr, #256]
	b	.L663
.L669:
	mov	r1, r10
	b	.L660
.L667:
	mov	r0, #1
	b	.L653
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
	ldrb	r4, [r2, #521]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L677
	ldrb	r2, [r2, #522]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r4, #2
	moveq	r4, #1
.L677:
	add	r3, r3, #45056
	ldr	r5, [r3, #2384]
	cmp	r5, #0
	beq	.L685
	movw	r3, #47304
	mov	r2, #0
	movt	r3, 169
	add	r3, r0, r3
	b	.L682
.L695:
	ldrb	ip, [r0, #3]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L693
.L680:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L694
.L682:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L695
	ldrb	lr, [r0, #3]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L681
	ldr	ip, [r0, #612]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	beq	.L696
.L681:
	tst	lr, #2
	beq	.L680
	ldr	ip, [r0, #648]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L680
	ldr	ip, [r0, #660]
	cmp	ip, r1
	bne	.L680
	add	r0, r0, #648
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L693:
	ldr	ip, [r0, #576]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L680
	ldr	ip, [r0, #588]
	cmp	ip, r1
	bne	.L680
	add	r0, r0, #576
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L696:
	ldr	ip, [r0, #624]
	cmp	ip, r1
	bne	.L681
	add	r0, r0, #612
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L694:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L685:
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
	ldrb	r4, [r2, #521]	@ zero_extendqisi2
	cmp	r4, #0
	beq	.L698
	ldrb	r2, [r2, #522]	@ zero_extendqisi2
	cmp	r2, #0
	movne	r4, #2
	moveq	r4, #1
.L698:
	add	r3, r3, #45056
	ldr	r5, [r3, #2388]
	cmp	r5, #0
	beq	.L706
	movw	r3, #47368
	mov	r2, #0
	movt	r3, 169
	add	r3, r0, r3
	b	.L703
.L716:
	ldrb	ip, [r0, #3]	@ zero_extendqisi2
	cmp	ip, #3
	beq	.L714
.L701:
	add	r2, r2, #1
	add	r3, r3, #4
	cmp	r2, r5
	beq	.L715
.L703:
	cmp	r4, #0
	ldr	r0, [r3]
	beq	.L716
	ldrb	lr, [r0, #3]	@ zero_extendqisi2
	tst	lr, #1
	beq	.L702
	ldr	ip, [r0, #612]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	beq	.L717
.L702:
	tst	lr, #2
	beq	.L701
	ldr	ip, [r0, #648]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L701
	ldr	ip, [r0, #656]
	cmp	ip, r1
	bne	.L701
	add	r0, r0, #648
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L714:
	ldr	ip, [r0, #576]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	bne	.L701
	ldr	ip, [r0, #584]
	cmp	ip, r1
	bne	.L701
	add	r0, r0, #576
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L717:
	ldr	ip, [r0, #620]
	cmp	ip, r1
	bne	.L702
	add	r0, r0, #612
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L715:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L706:
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
	ble	.L719
	add	lr, r6, r3, lsl #2
	mov	ip, r3
.L720:
	ldr	r4, [lr, #-4]!
	sub	ip, ip, #1
	str	r4, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L720
.L719:
	add	r2, r4, #1
	str	r2, [r7]
	str	r0, [r6, r4, asl #2]
	ldr	ip, [r7]
	cmp	r3, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r6, ip, lsl #2
	mov	lr, ip
	b	.L724
.L723:
	str	r0, [r6, lr, asl #2]
	add	lr, lr, #1
.L722:
	cmp	ip, r2
	beq	.L732
.L724:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L722
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L723
	ldr	r3, [r0, #12]
	cmp	r3, r5
	bne	.L723
	ldr	r3, [r0, #4]
	ldr	r3, [r3, #56]
	cmp	r3, r1
	bne	.L723
	cmp	ip, r2
	bne	.L724
.L732:
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
	ble	.L734
	add	lr, r6, r3, lsl #2
	mov	ip, r3
.L735:
	ldr	r4, [lr, #-4]!
	sub	ip, ip, #1
	str	r4, [lr, #4]
	ldr	r4, [r7]
	cmp	r4, ip
	blt	.L735
.L734:
	add	r2, r4, #1
	str	r2, [r7]
	str	r0, [r6, r4, asl #2]
	ldr	ip, [r7]
	cmp	r3, ip
	ldmltfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r2, r8, #2
	add	r4, r6, ip, lsl #2
	mov	lr, ip
	b	.L739
.L738:
	str	r0, [r6, lr, asl #2]
	add	lr, lr, #1
.L737:
	cmp	ip, r2
	beq	.L750
.L739:
	ldr	r0, [r4], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L737
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L738
	ldr	r3, [r0, #8]
	cmp	r3, r5
	bne	.L738
	ldr	r3, [r0, #4]
	ldr	r3, [r3, #56]
	cmp	r3, r1
	bne	.L738
	cmp	ip, r2
	bne	.L739
.L750:
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
	beq	.L758
	ldr	ip, [ip, #2388]
	cmp	r1, ip
	beq	.L759
	mov	r6, r5
	mov	ip, #0
	b	.L753
.L755:
	ldr	r4, [lr, #4]!
	cmp	r1, r4
	beq	.L761
.L753:
	add	ip, ip, #1
	cmp	ip, r5
	bne	.L755
	cmp	r6, #0
	blt	.L762
.L752:
	cmp	r2, #0
	add	r6, r6, r3, lsl #1
	movwne	r3, #5718
	movweq	r3, #5726
	movt	r3, 41
	add	r3, r6, r3
	add	r0, r0, r3, lsl #2
	ldr	r0, [r0, #4]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L761:
	mov	r6, ip
	cmp	r6, #0
	bge	.L752
.L762:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L758:
	mov	r6, r5
	b	.L752
.L759:
	mov	r6, #0
	b	.L752
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
	movw	ip, #27166
	mov	r4, #1376
	movt	ip, 42
	add	ip, r3, ip
	mla	r3, r4, r3, r0
	ldr	lr, [r0, ip, asl #2]
	movw	ip, #31360
	cmp	lr, #0
	movt	ip, 169
	add	ip, r3, ip
	beq	.L769
	add	r4, r0, #11141120
	mov	r3, #0
	mov	r0, ip
	add	r4, r4, #8192
	b	.L768
.L765:
	cmp	r3, lr
	add	r0, r0, #688
	beq	.L771
.L768:
	ldr	ip, [r0, #56]
	add	r3, r3, #1
	cmp	ip, r1
	bne	.L765
	ldrb	ip, [r4, #1595]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L766
	ldr	ip, [r0, #596]
	cmp	ip, r2
	bne	.L765
	add	r0, r0, #576
	ldmfd	sp, {r4, fp, sp, pc}
.L766:
	cmp	ip, #1
	beq	.L772
	cmp	ip, #2
	bne	.L765
	ldr	ip, [r0, #668]
	cmp	ip, r2
	bne	.L765
	add	r0, r0, #648
	ldmfd	sp, {r4, fp, sp, pc}
.L772:
	ldr	ip, [r0, #632]
	cmp	ip, r2
	bne	.L765
	add	r0, r0, #612
	ldmfd	sp, {r4, fp, sp, pc}
.L771:
	mov	r0, #0
	ldmfd	sp, {r4, fp, sp, pc}
.L769:
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
	bls	.L775
	add	lr, r4, r1, lsl #2
	mov	ip, r1
.L776:
	ldr	r3, [lr, #-4]!
	sub	ip, ip, #1
	str	r3, [lr, #4]
	ldr	r3, [r7]
	cmp	r3, ip
	bcc	.L776
.L775:
	add	r2, r3, #1
	str	r2, [r7]
	str	r0, [r4, r3, asl #2]
	ldr	ip, [r7]
	cmp	r1, ip
	ldmccfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	add	r3, r4, ip, lsl #2
	mov	lr, ip
	b	.L780
.L778:
	cmp	r1, ip
	str	r0, [r4, lr, asl #2]
	add	lr, lr, #1
	bcc	.L787
.L780:
	ldr	r0, [r3], #4
	add	ip, ip, #1
	cmp	r0, #0
	beq	.L778
	ldr	r2, [r0, #4]
	ldr	r2, [r2, #56]
	cmp	r2, r5
	bne	.L778
	ldr	r2, [r0, #20]
	cmp	r2, r6
	bne	.L778
	cmp	r1, ip
	bcs	.L780
.L787:
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
	add	r6, r0, #11075584
	add	r9, r6, #40960
	str	r2, [fp, #-64]
	mov	r2, #0
	mov	r5, r0
	ldrb	r3, [r9, #521]	@ zero_extendqisi2
	mov	r10, r1
	str	r2, [fp, #-48]
	cmp	r3, r2
	beq	.L789
	ldrb	r3, [r9, #522]	@ zero_extendqisi2
	cmp	r3, r2
	movne	r3, #2
	moveq	r3, #1
.L789:
	cmp	r10, #1
	bls	.L837
.L788:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L837:
	cmp	r10, #0
	beq	.L838
	movw	r1, #40564
	movw	r8, #40828
	movw	r6, #41348
	movt	r1, 169
	movt	r8, 169
	movt	r6, 169
	add	r1, r5, r1
	add	r8, r5, r8
	add	r6, r5, r6
	movw	r2, #41088
	movt	r2, 169
	add	r2, r5, r2
	str	r2, [fp, #-100]
.L792:
	ldr	r0, [r5, #236]
	cmp	r3, #0
	mov	r2, #1
	ldr	r3, [r0, #2896]
	add	r3, r3, #4
	mov	r3, r2, asl r3
	str	r3, [fp, #-88]
	ldr	r3, [r9, #548]
	ldrne	r2, [fp, #-88]
	movne	r3, r3, asl #1
	addne	r3, r3, #1
	str	r3, [fp, #-92]
	ldr	r3, [fp, #-64]
	movne	r2, r2, asl #1
	strne	r2, [fp, #-88]
	adds	r3, r3, #2
	str	r3, [fp, #-72]
	beq	.L788
	ldr	r2, [r1]
	cmp	r2, #3
	beq	.L788
	movw	r3, #22868
	str	r9, [fp, #-68]
	mov	r0, r3
	mov	r3, #0
	mov	r4, r3
	str	r3, [fp, #-76]
	str	r3, [fp, #-80]
	mov	r7, r3
	ldr	r3, [fp, #-92]
	movt	r0, 164
	mov	r9, r1
	add	r0, r5, r0
	str	r0, [fp, #-96]
	str	r3, [fp, #-60]
	mov	r3, #1
	str	r3, [fp, #-84]
	b	.L796
.L797:
	cmp	r2, #2
	beq	.L839
	ldr	r3, [fp, #-84]
	cmp	r3, #1
	beq	.L803
	ldr	r3, [fp, #-68]
	ldrb	r3, [r3, #529]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	mov	r3, r10, asl #1
.L804:
	cmp	r2, #4
	ldr	r2, [r6]
	beq	.L840
	add	r2, r2, #1
	add	r4, r2, r4
	ldr	r2, [fp, #-76]
	cmp	r2, r4
	rsble	r4, r2, r4
.L814:
	ldr	r2, [fp, #-56]
	add	r0, r5, #11141120
	add	r0, r0, #8192
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
	ldr	ip, [r0, #2132]
	mov	r0, r5
	str	ip, [sp]
	bl	mvc_reorder_interview
.L801:
	ldr	r3, [fp, #-72]
	add	r7, r7, #1
	cmp	r7, r3
	beq	.L788
	ldr	r2, [r9, #4]!
	add	r6, r6, #4
	add	r8, r8, #4
	cmp	r2, #3
	beq	.L788
.L796:
	cmp	r2, #1
	bhi	.L797
	cmp	r2, #0
	ldr	r3, [r8]
	bne	.L798
	ldr	r2, [fp, #-60]
	mvn	r3, r3
	adds	r3, r2, r3
	str	r3, [fp, #-60]
	bmi	.L841
.L799:
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
	ldr	ip, [r2, #2160]
	sub	r2, fp, #48
	str	r2, [sp]
	ldr	r2, [fp, #-64]
	str	ip, [sp, #4]
	bl	MVC_ReorderSTList
	b	.L801
.L838:
	movw	r1, #40432
	movw	r8, #40696
	movt	r1, 169
	movt	r8, 169
	add	r6, r6, #41216
	add	r1, r5, r1
	add	r8, r5, r8
	str	r9, [fp, #-100]
	b	.L792
.L840:
	mvn	r2, r2
	adds	r4, r2, r4
	ldrmi	r2, [fp, #-76]
	addmi	r4, r4, r2
	b	.L814
.L803:
	add	r3, r5, #10747904
	ldr	r1, [fp, #-68]
	add	r3, r3, #20480
	ldr	ip, [r3, #2384]
	ldr	r1, [r1, #2160]
	adds	ip, ip, #1
	beq	.L805
	ldr	r3, [r3, #2388]
	cmp	r1, r3
	beq	.L806
	ldr	r0, [fp, #-96]
	mov	r3, #0
	str	ip, [fp, #-80]
	b	.L807
.L809:
	ldr	lr, [r0, #4]!
	cmp	r1, lr
	beq	.L823
.L807:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L809
.L808:
	ldr	r3, [fp, #-68]
	mov	r4, ip
	ldr	r0, [fp, #-96]
	ldrb	r3, [r3, #529]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	mov	r3, #0
	b	.L819
.L811:
	ldr	lr, [r0, #4]!
	cmp	r1, lr
	beq	.L824
.L819:
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L811
	cmp	r4, #0
	blt	.L825
.L818:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	mov	r3, r10, asl #1
	beq	.L812
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
	b	.L804
.L798:
	ldr	r2, [fp, #-60]
	add	r3, r3, #1
	add	r3, r2, r3
	ldr	r2, [fp, #-88]
	str	r3, [fp, #-60]
	cmp	r2, r3
	rsble	r3, r2, r3
	strle	r3, [fp, #-60]
	b	.L799
.L839:
	ldr	r1, [fp, #-68]
	sub	r0, fp, #48
	ldr	r3, [fp, #-100]
	ldr	r2, [fp, #-64]
	ldr	ip, [r1, #2160]
	mov	r1, r10
	ldr	r3, [r3, r7, asl #2]
	str	r0, [sp]
	mov	r0, r5
	str	ip, [sp, #4]
	bl	MVC_ReorderLTList
	b	.L801
.L823:
	str	r3, [fp, #-80]
	b	.L808
.L824:
	mov	r4, r3
	cmp	r4, #0
	bge	.L818
.L825:
	mov	r3, #0
	mvn	r4, #0
	mov	r1, r3
	str	r3, [fp, #-84]
	str	r1, [fp, #-76]
	mov	r3, r10, asl #1
	b	.L804
.L812:
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
	b	.L804
.L841:
	ldr	r2, [fp, #-88]
	add	r3, r3, r2
	str	r3, [fp, #-60]
	b	.L799
.L805:
	ldr	r3, [fp, #-68]
	mov	r4, ip
	str	ip, [fp, #-80]
	ldrb	r3, [r3, #529]	@ zero_extendqisi2
	str	r3, [fp, #-56]
	b	.L818
.L806:
	ldr	r1, [fp, #-68]
	mov	r3, #0
	mov	r4, r3
	str	r3, [fp, #-80]
	ldrb	r1, [r1, #529]	@ zero_extendqisi2
	str	r1, [fp, #-56]
	b	.L818
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
	ldrb	r1, [r5, #520]	@ zero_extendqisi2
	cmp	r1, #2
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r7, r4, #36864
	ldrb	r3, [r7, #3564]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L854
.L845:
	ldr	r3, [r5, #580]
	cmp	r1, #1
	add	r3, r3, #1
	str	r3, [r5, #568]
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldrb	r3, [r7, #3565]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L855
.L847:
	add	r4, r4, #40960
	ldr	r3, [r4, #584]
	add	r3, r3, #1
	str	r3, [r4, #572]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L854:
	mov	r1, #0
	ldr	r2, [r5, #580]
	bl	MVC_ReorderRefPiclist
	ldrb	r1, [r5, #520]	@ zero_extendqisi2
	b	.L845
.L855:
	ldr	r2, [r5, #584]
	mov	r0, r6
	bl	MVC_ReorderRefPiclist
	b	.L847
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
	ldr	lr, .L919
	movcc	r4, r2
	ldr	ip, .L919+4
	movcs	r4, #31
	cmp	r6, #0
	str	r3, [fp, #-52]
	movne	r6, lr
	moveq	r6, ip
	cmp	r0, #1
	moveq	r5, #0
	moveq	r10, r5
	beq	.L859
	cmp	r0, #2
	moveq	r5, #0
	moveq	r10, r5
	bne	.L867
.L868:
	cmp	r10, r4
	movcs	r3, #0
	movcc	r3, #1
	cmp	r5, r4
	str	r3, [fp, #-48]
	movcs	r2, #0
	movcc	r2, #1
	orrs	r3, r2, r3
	beq	.L867
	cmp	r2, #0
	beq	.L873
	add	r8, r7, r5, lsl #2
	b	.L872
.L869:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L873
.L872:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	tst	ip, #2
	beq	.L869
	add	r0, r0, #648
	blx	r6
	cmp	r0, #0
	beq	.L869
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #648
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L873:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r10, lsl #2
	bne	.L875
	b	.L868
.L874:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L868
.L875:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	tst	r2, #1
	beq	.L874
	add	r0, r0, #612
	blx	r6
	cmp	r0, #0
	beq	.L874
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #612
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
	b	.L868
.L918:
	add	r0, r0, #612
	blx	r6
	cmp	r0, #0
	beq	.L860
	ldr	r3, [fp, #4]
	add	r10, r10, #1
	ldr	r2, [r8]
	ldr	r0, [r3]
	add	r2, r2, #612
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L864:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	addne	r8, r7, r5, lsl #2
	bne	.L866
	b	.L859
.L865:
	add	r5, r5, #1
	cmp	r4, r5
	bls	.L859
.L866:
	mov	r9, r8
	ldr	r0, [r8], #4
	ldrb	r2, [r0, #2]	@ zero_extendqisi2
	tst	r2, #2
	beq	.L865
	add	r0, r0, #648
	blx	r6
	cmp	r0, #0
	beq	.L865
	ldr	r3, [fp, #4]
	add	r5, r5, #1
	ldr	r2, [r9]
	ldr	r0, [r3]
	add	r2, r2, #648
	ldr	r3, [fp, #-52]
	str	r2, [r3, r0, asl #2]
	ldr	r3, [fp, #4]
	ldr	r2, [r3]
	add	r2, r2, #1
	str	r2, [r3]
.L859:
	cmp	r10, r4
	movcs	r2, #0
	movcc	r2, #1
	cmp	r5, r4
	movcs	r3, #0
	movcc	r3, #1
	str	r3, [fp, #-48]
	orrs	r3, r3, r2
	beq	.L867
	cmp	r2, #0
	beq	.L864
	add	r9, r7, r10, lsl #2
	b	.L863
.L860:
	add	r10, r10, #1
	cmp	r4, r10
	bls	.L864
.L863:
	mov	r8, r9
	ldr	r0, [r9], #4
	ldrb	ip, [r0, #2]	@ zero_extendqisi2
	tst	ip, #1
	beq	.L860
	b	.L918
.L867:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L920:
	.align	2
.L919:
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
.L924:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L922
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L932
.L922:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L924
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L932:
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
	beq	.L937
	ldr	r3, [r0]
	cmp	r3, r2
	beq	.L938
	mov	ip, r0
	mov	r3, #0
	mov	r0, r1
	b	.L935
.L936:
	ldr	lr, [ip, #4]!
	cmp	lr, r2
	beq	.L939
.L935:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L936
	ldmfd	sp, {fp, sp, pc}
.L939:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L937:
	mov	r0, r1
	ldmfd	sp, {fp, sp, pc}
.L938:
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
	beq	.L944
	ldr	r3, [r1]
	cmp	r3, r0
	beq	.L945
	mov	r3, #0
	b	.L942
.L943:
	ldr	ip, [r1, #4]!
	cmp	ip, r0
	beq	.L941
.L942:
	add	r3, r3, #1
	cmp	r3, r2
	mov	lr, r3
	bne	.L943
.L941:
	cmp	r2, #0
	cmpne	r2, lr
	movhi	r0, #1
	movls	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L944:
	mov	lr, r2
	b	.L941
.L945:
	mov	lr, #0
	b	.L941
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
	beq	.L962
	cmp	r0, #2
	ldmnefd	sp, {fp, sp, pc}
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r0, [ip]
	add	r1, r1, #648
	mov	lr, #0
.L953:
	add	lr, lr, #1
	str	r1, [r3, r0, asl #2]
	cmp	lr, r2
	ldr	r0, [ip]
	add	r1, r1, #688
	add	r0, r0, #1
	str	r0, [ip]
	bne	.L953
	ldmfd	sp, {fp, sp, pc}
.L962:
	cmp	r2, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r0, [ip]
	add	r1, r1, #612
	mov	lr, #0
.L949:
	add	lr, lr, #1
	str	r1, [r3, r0, asl #2]
	cmp	lr, r2
	ldr	r0, [ip]
	add	r1, r1, #688
	add	r0, r0, #1
	str	r0, [ip]
	bne	.L949
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
	ldr	r7, [r5, #2160]
	add	r0, r9, r0
	beq	.L982
	ldr	r2, [lr, #2388]
	cmp	r7, r2
	beq	.L983
	mov	r2, #0
	b	.L965
.L966:
	ldr	lr, [r0, #4]!
	cmp	r7, lr
	beq	.L964
.L965:
	add	r2, r2, #1
	cmp	r2, r6
	mov	r4, r2
	bne	.L966
.L964:
	ldrb	r2, [r5, #529]	@ zero_extendqisi2
	add	r4, r4, r1, lsl #1
	add	r6, r9, #11141120
	add	ip, ip, #45056
	cmp	r2, #0
	add	r6, r6, #8192
	movwne	r2, #5718
	movweq	r2, #5726
	movtne	r2, 41
	movteq	r2, 41
	addne	r2, r4, r2
	addeq	r2, r4, r2
	ldrb	r8, [r6, #1595]	@ zero_extendqisi2
	addne	r4, r9, r4, lsl #2
	add	r2, r9, r2, lsl #2
	movwne	r10, #22892
	addeq	r4, r9, r4, lsl #2
	movweq	r10, #22924
	ldr	r7, [r2, #4]
	sub	r8, r8, #2
	ldr	r2, [ip, #2376]
	movt	r10, 164
	clz	r8, r8
	add	r10, r4, r10
	subs	r4, r2, #1
	mov	r8, r8, lsr #5
	bmi	.L963
	movw	r5, #28194
	movt	r5, 42
	add	r5, r2, r5
	mov	r2, r3
	mov	r3, r9
	add	r5, r9, r5, lsl #2
	mov	r9, r2
	b	.L980
.L1006:
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r2, #3
	ldreq	r0, [r1, #596]
	bne	.L970
.L972:
	add	r2, r1, r8
	ldrb	r2, [r2, #12]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L970
	ldr	r2, [r6, #2132]
	cmp	r2, r0
	beq	.L1005
.L970:
	subs	r4, r4, #1
	bmi	.L963
.L980:
	ldr	r1, [r5, #-4]!
	cmp	r1, #0
	beq	.L970
	ldrb	r2, [r6, #1595]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1006
	cmp	r2, #1
	beq	.L1007
	cmp	r2, #2
	bne	.L970
	ldrb	r0, [r6, #1593]	@ zero_extendqisi2
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	ubfx	r2, r2, #1, #1
	beq	.L975
	ldr	r0, [r6, #2112]
	ldr	ip, [r1, #632]
	ldr	r0, [r0, #632]
	cmp	ip, r0
	movne	r2, #0
	andeq	r2, r2, #1
.L975:
	cmp	r2, #0
	ldrne	r0, [r1, #668]
	bne	.L972
	subs	r4, r4, #1
	bpl	.L980
.L963:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1007:
	ldrb	r0, [r6, #1593]	@ zero_extendqisi2
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	and	r2, r2, #1
	beq	.L974
	ldr	r0, [r6, #2112]
	ldr	ip, [r1, #668]
	ldr	r0, [r0, #668]
	cmp	ip, r0
	movne	r2, #0
	andeq	r2, r2, #1
.L974:
	cmp	r2, #0
	ldrne	r0, [r1, #632]
	bne	.L972
	b	.L970
.L1005:
	cmp	r7, #0
	ldr	lr, [r1, #56]
	beq	.L970
	ldr	r2, [r10]
	cmp	lr, r2
	beq	.L976
	mov	r0, r10
	mov	r2, #0
.L977:
	add	r2, r2, #1
	cmp	r2, r7
	beq	.L970
	ldr	ip, [r0, #4]!
	cmp	lr, ip
	bne	.L977
.L976:
	str	r3, [fp, #-52]
	mov	r2, #688
	ldr	r3, .L1008
	ldr	r0, [r9]
	ldr	ip, [r3, #52]
	ldr	r3, [fp, #-48]
	mla	r0, r2, r0, r3
	blx	ip
	ldr	r2, [r9]
	ldr	r0, [fp, #-48]
	mov	r1, #688
	mov	ip, #688
	ldr	r3, [fp, #-52]
	mla	r2, r1, r2, r0
	str	r2, [r2, #652]
	str	r2, [r2, #616]
	str	r2, [r2, #580]
	ldr	r2, [r9]
	ldr	r1, [r3, #52]
	mla	r2, ip, r2, r0
	sub	r1, r1, #1
	str	r1, [r2, #48]
	ldr	r2, [r9]
	cmp	r2, r7
	beq	.L963
	add	r2, r2, #1
	str	r2, [r9]
	b	.L970
.L982:
	mov	r4, r6
	b	.L964
.L983:
	mov	r4, #0
	b	.L964
.L1009:
	.align	2
.L1008:
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
	add	r7, r7, #8192
	ldrb	r9, [r5, #520]	@ zero_extendqisi2
	mov	r6, r0
	str	r3, [r5, #2172]
	cmp	r9, r3
	str	r3, [r5, #2168]
	ldrb	r4, [r7, #1595]	@ zero_extendqisi2
	bne	.L1011
	cmp	r4, r3
	add	r3, r2, #45056
	str	r3, [fp, #-240]
	bne	.L1169
	ldr	ip, [r3, #2384]
	cmp	ip, #0
	beq	.L1095
	movw	r1, #47300
	mov	r3, r4
	movt	r1, 169
	add	r1, r0, r1
	b	.L1017
.L1016:
	cmp	r3, ip
	beq	.L1015
.L1017:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1016
	ldr	r0, [r2, #576]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #65536
	addeq	r0, r4, #64
	addeq	r2, r2, #576
	addeq	r4, r4, #1
	streq	r2, [r6, r0, asl #2]
	cmp	r3, ip
	bne	.L1017
.L1015:
	add	r3, r6, #256
	mov	r1, r4
	str	r3, [fp, #-244]
	mov	r2, #4
	mov	r0, r3
	ldr	r3, .L1180
	bl	qsort
	ldr	r3, [fp, #-240]
	str	r4, [r5, #568]
	ldr	r1, [r3, #2388]
	cmp	r1, #0
	beq	.L1096
	movw	r0, #47364
	mov	r8, r4
	movt	r0, 169
	mov	r3, #0
	add	r0, r6, r0
	b	.L1020
.L1019:
	cmp	r3, r1
	beq	.L1170
.L1020:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #3]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1019
	ldr	ip, [r2, #576]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #256
	addeq	ip, r8, #64
	addeq	r2, r2, #576
	addeq	r8, r8, #1
	streq	r2, [r6, ip, asl #2]
	cmp	r3, r1
	bne	.L1020
.L1170:
	rsb	r1, r4, r8
.L1018:
	add	r0, r4, #64
	ldr	r3, .L1180+4
	mov	r2, #4
	add	r0, r6, r0, lsl #2
	bl	qsort
	str	r8, [r5, #568]
	b	.L1021
.L1011:
	cmp	r4, #0
	beq	.L1038
	add	r2, r2, #45056
	str	r2, [fp, #-240]
	ldr	ip, [r2, #2384]
	cmp	ip, #0
	beq	.L1171
	movw	r9, #47300
	mov	r4, r3
	movt	r9, 169
	add	r9, r0, r9
	mov	r1, r9
.L1057:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1056
	sub	r0, fp, #44
	ldr	lr, [r7, #2132]
	add	r8, r0, r4, lsl #2
	ldr	r0, [r2, #32]
	cmp	lr, r0
	strge	r2, [r8, #-192]
	addge	r4, r4, #1
.L1056:
	cmp	r3, ip
	bne	.L1057
	sub	r10, fp, #236
	ldr	r3, .L1180+8
	mov	r2, #4
	mov	r1, r4
	mov	r0, r10
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2384]
	cmp	r8, #0
	beq	.L1166
.L1093:
	mov	r3, r9
	mov	r2, #0
	mov	r9, r4
.L1060:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #2]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L1059
	sub	r0, fp, #44
	ldr	ip, [r7, #2132]
	add	lr, r0, r9, lsl #2
	ldr	r0, [r1, #32]
	cmp	ip, r0
	strlt	r1, [lr, #-192]
	addlt	r9, r9, #1
.L1059:
	cmp	r2, r8
	bne	.L1060
	rsb	r8, r4, r9
	mov	r3, r4, asl #2
	add	r0, r10, r3
	str	r3, [fp, #-244]
	mov	r1, r8
	ldr	r3, .L1180+12
	mov	r2, #4
	bl	qsort
	cmp	r4, #0
	subeq	ip, fp, #172
	beq	.L1061
.L1088:
	sub	ip, fp, #172
	mov	r2, r10
	add	r8, ip, r8, lsl #2
	mov	r3, #0
.L1062:
	add	r3, r3, #1
	ldr	r1, [r2], #4
	cmp	r3, r4
	str	r1, [r8], #4
	bcc	.L1062
.L1061:
	cmp	r9, r4
	bls	.L1063
	ldr	r3, [fp, #-244]
	add	r2, r10, r3
	mov	r3, ip
.L1064:
	add	r4, r4, #1
	ldr	r1, [r2], #4
	cmp	r4, r9
	str	r1, [r3], #4
	bne	.L1064
.L1063:
	movw	r8, #41528
	mov	r4, #0
	add	r3, r6, #256
	str	r4, [r5, #568]
	str	r4, [r5, #572]
	mov	r1, r10
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	mov	r2, r9
	str	r4, [sp, #4]
	movt	r8, 169
	add	r8, r6, r8
	str	r8, [sp]
	str	ip, [fp, #-252]
	movw	r10, #41532
	str	r3, [fp, #-244]
	bl	MVC_GenPiclistfromFrmlist
	ldr	ip, [fp, #-252]
	add	r3, r6, #388
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	mov	r2, r9
	str	r4, [sp, #4]
	movt	r10, 169
	mov	r1, ip
	add	r10, r6, r10
	str	r10, [sp]
	str	r3, [fp, #-248]
	bl	MVC_GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	r9, [r3, #2388]
	cmp	r9, r4
	subeq	ip, fp, #108
	beq	.L1065
	movw	r3, #47364
	sub	ip, fp, #108
	movt	r3, 169
	add	r3, r6, r3
	mov	r2, ip
.L1066:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r9
	str	r1, [r2], #4
	bne	.L1066
.L1065:
	mov	r0, ip
	ldr	r3, .L1180+16
	mov	r2, #4
	mov	r1, r9
	str	ip, [fp, #-240]
	bl	qsort
	ldr	ip, [fp, #-240]
	mov	r4, #1
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	mov	r2, r9
	ldr	r3, [fp, #-244]
	mov	r1, ip
	str	r4, [sp, #4]
	str	r8, [sp]
	bl	MVC_GenPiclistfromFrmlist
	ldr	ip, [fp, #-240]
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	mov	r2, r9
	str	r4, [sp, #4]
	ldr	r3, [fp, #-248]
	mov	r1, ip
	str	r10, [sp]
	bl	MVC_GenPiclistfromFrmlist
	ldr	r4, [r5, #568]
	ldr	r3, [r5, #572]
	rsb	r3, r3, r4
	clz	r3, r3
	mov	r3, r3, lsr #5
.L1055:
	cmp	r4, #1
	movls	r3, #0
	andhi	r3, r3, #1
	cmp	r3, #0
	beq	.L1067
	cmp	r4, #0
	beq	.L1068
	ldr	lr, [r6, #256]
	ldr	r3, [r6, #388]
	cmp	lr, r3
	bne	.L1067
	ldr	r1, [fp, #-244]
	add	r2, r6, #388
	mov	r3, #0
	b	.L1069
.L1070:
	ldr	ip, [r1, #4]!
	ldr	r0, [r2, #4]!
	cmp	ip, r0
	bne	.L1067
.L1069:
	add	r3, r3, #1
	cmp	r3, r4
	bne	.L1070
.L1071:
	ldr	r3, [r6, #392]
	str	lr, [r6, #392]
	str	r3, [r6, #388]
	ldrb	r3, [r5, #532]	@ zero_extendqisi2
.L1089:
	cmp	r3, #0
	bne	.L1158
.L1072:
	movw	r4, #31360
	movw	r3, #43128
	movt	r4, 169
	add	r4, r6, r4
	mov	r0, r6
	movt	r3, 169
	mov	r1, #0
	mov	r2, r4
	add	r3, r6, r3
	bl	mvc_append_interview_list
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1172
	ldr	r1, [r5, #2168]
	ldr	ip, [r5, #568]
	cmp	r1, #0
	beq	.L1074
	add	r2, ip, #63
	movw	r3, #31936
	movt	r3, 169
	add	r3, r6, r3
	add	r2, r6, r2, lsl #2
.L1075:
	add	r0, r0, #1
	str	r3, [r2, #4]!
	cmp	r0, r1
	add	r3, r3, #688
	bne	.L1075
	add	ip, ip, r0
.L1074:
	str	ip, [r5, #568]
.L1076:
	ldrb	r1, [r5, #520]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1173
.L1159:
	ldr	r0, [r5, #568]
.L1073:
	cmp	r1, #0
	bne	.L1031
	cmp	r0, #0
	beq	.L1174
.L1030:
	ldr	r2, [r5, #580]
	ldr	r3, [r5, #584]
	add	r2, r2, #1
	ldr	r1, [r5, #572]
	cmp	r0, r2
	add	r3, r3, #1
	movcc	r2, r0
	cmp	r3, r1
	str	r2, [r5, #568]
	movcs	r3, r1
	cmp	r2, #32
	str	r3, [r5, #572]
	bhi	.L1082
.L1090:
	add	r1, r2, #63
	mov	r0, #0
	add	r1, r6, r1, lsl #2
.L1083:
	add	r2, r2, #1
	str	r0, [r1, #4]!
	cmp	r2, #32
	bls	.L1083
.L1082:
	cmp	r3, #32
	bhi	.L1086
	add	r3, r3, #96
	add	r1, r6, #516
	mov	r2, #0
	add	r6, r6, r3, lsl #2
.L1085:
	str	r2, [r6, #4]!
	cmp	r6, r1
	bne	.L1085
.L1086:
	mov	r0, #0
.L1152:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1038:
	add	r3, r2, #45056
	str	r3, [fp, #-240]
	ldr	r10, [r3, #2384]
	cmp	r10, #0
	beq	.L1041
	movw	r9, #47300
	mov	r3, r4
	movt	r9, 169
	add	r9, r0, r9
	mov	r1, r9
	b	.L1043
.L1042:
	cmp	r3, r10
	beq	.L1175
.L1043:
	ldr	r2, [r1, #4]!
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1042
	ldr	r0, [r2, #592]
	add	lr, r4, #64
	ldr	ip, [r7, #2136]
	add	r2, r2, #576
	cmp	ip, r0
	strge	r2, [r6, lr, asl #2]
	addge	r4, r4, #1
	cmp	r3, r10
	bne	.L1043
.L1175:
	add	r3, r6, #256
	mov	r2, #4
	str	r3, [fp, #-244]
	mov	r1, r4
	mov	r0, r3
	ldr	r3, .L1180+20
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2384]
	cmp	r8, #0
	beq	.L1165
.L1091:
	mov	r3, r9
	mov	r10, r4
	mov	r2, #0
	b	.L1046
.L1045:
	cmp	r2, r8
	beq	.L1176
.L1046:
	ldr	r1, [r3, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1045
	ldr	r0, [r1, #592]
	add	lr, r10, #64
	ldr	ip, [r7, #2136]
	add	r1, r1, #576
	cmp	ip, r0
	strlt	r1, [r6, lr, asl #2]
	addlt	r10, r10, #1
	cmp	r2, r8
	bne	.L1046
.L1176:
	rsb	r8, r4, r10
	add	r0, r4, #64
	ldr	r3, .L1180+24
	mov	r2, #4
	mov	r1, r8
	add	r0, r6, r0, lsl #2
	bl	qsort
	add	r9, r10, #64
	cmp	r4, #0
	add	r9, r6, r9, lsl #2
	beq	.L1048
.L1087:
	add	r8, r8, #96
	add	r2, r6, #252
	mov	r3, #0
	add	r8, r6, r8, lsl #2
.L1049:
	add	r3, r3, #1
	ldr	r1, [r2, #4]!
	cmp	r3, r4
	str	r1, [r8, #4]!
	bcc	.L1049
.L1048:
	cmp	r10, r4
	bls	.L1050
	add	r3, r4, #63
	add	r2, r6, #384
	add	r3, r6, r3, lsl #2
.L1051:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r10
	str	r1, [r2, #4]!
	bne	.L1051
.L1050:
	ldr	r3, [fp, #-240]
	str	r10, [r5, #572]
	str	r10, [r5, #568]
	ldr	r1, [r3, #2388]
	cmp	r1, #0
	beq	.L1097
	movw	r0, #47364
	mov	r4, r10
	movt	r0, 169
	mov	r3, #0
	add	r0, r6, r0
	b	.L1054
.L1053:
	cmp	r3, r1
	beq	.L1177
.L1054:
	ldr	r2, [r0, #4]!
	add	r3, r3, #1
	ldrb	ip, [r2, #2]	@ zero_extendqisi2
	cmp	ip, #3
	bne	.L1053
	ldrb	ip, [r2, #577]	@ zero_extendqisi2
	cmp	ip, #1
	addeq	ip, r6, r4, lsl #2
	addeq	r2, r2, #576
	addeq	r4, r4, #1
	streq	r2, [ip, #256]
	streq	r2, [ip, #388]
	cmp	r3, r1
	bne	.L1054
.L1177:
	rsb	r1, r10, r4
.L1052:
	mov	r0, r9
	ldr	r3, .L1180+4
	mov	r2, #4
	bl	qsort
	ldr	r1, [r5, #568]
	ldr	r3, .L1180+4
	mov	r2, #4
	add	r0, r1, #97
	rsb	r1, r1, r4
	add	r0, r6, r0, lsl #2
	bl	qsort
	str	r4, [r5, #572]
	str	r4, [r5, #568]
	mov	r3, #1
	b	.L1055
.L1169:
	ldr	lr, [r3, #2384]
	cmp	lr, #0
	moveq	r9, lr
	beq	.L1014
	movw	r1, #47300
	mov	r3, r9
	movt	r1, 169
	add	r1, r0, r1
.L1023:
	ldr	r2, [r1, #4]!
	sub	r0, fp, #44
	add	ip, r0, r9, lsl #2
	add	r3, r3, #1
	ldrb	r0, [r2, #3]	@ zero_extendqisi2
	cmp	r0, #0
	strne	r2, [ip, #-192]
	addne	r9, r9, #1
	cmp	r3, lr
	bne	.L1023
.L1014:
	mov	r1, r9
	ldr	r3, .L1180+28
	mov	r2, #4
	sub	r0, fp, #236
	movw	r8, #41528
	bl	qsort
	mov	r4, #0
	add	r3, r6, #256
	str	r4, [r5, #568]
	mov	r2, r9
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	sub	r1, fp, #236
	str	r4, [sp, #4]
	movt	r8, 169
	add	r8, r6, r8
	str	r8, [sp]
	str	r3, [fp, #-244]
	bl	MVC_GenPiclistfromFrmlist
	ldr	r3, [fp, #-240]
	ldr	r9, [r3, #2388]
	cmp	r9, r4
	subeq	ip, fp, #108
	beq	.L1024
	movw	r3, #47364
	sub	ip, fp, #108
	movt	r3, 169
	add	r3, r6, r3
	mov	r2, ip
.L1025:
	add	r4, r4, #1
	ldr	r1, [r3, #4]!
	cmp	r4, r9
	str	r1, [r2], #4
	bne	.L1025
.L1024:
	mov	r0, ip
	ldr	r3, .L1180+16
	mov	r2, #4
	mov	r1, r9
	str	ip, [fp, #-240]
	bl	qsort
	ldr	ip, [fp, #-240]
	mov	r3, #1
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	mov	r2, r9
	str	r3, [sp, #4]
	str	r8, [sp]
	mov	r1, ip
	ldr	r3, [fp, #-244]
	bl	MVC_GenPiclistfromFrmlist
.L1021:
	ldrb	r3, [r5, #532]	@ zero_extendqisi2
	mov	r2, #0
	str	r2, [r5, #572]
	cmp	r3, r2
	beq	.L1072
	ldr	r0, [r5, #568]
	cmp	r0, #0
	bne	.L1178
	ldr	r3, [r6, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L1032
	ldrb	r3, [r7, #1595]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1033
	ldr	r3, [r7, #2112]
	add	r3, r3, #576
	str	r3, [r6, #256]
.L1034:
	mov	r3, #1
	str	r3, [r5, #568]
.L1158:
	ldrb	r1, [r5, #520]	@ zero_extendqisi2
	b	.L1159
.L1178:
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1030
.L1031:
	cmp	r0, #0
	bne	.L1030
	ldr	r1, [r5, #572]
	cmp	r1, #0
	beq	.L1179
	ldr	r3, [r5, #584]
	mov	r2, r0
	str	r0, [r5, #568]
	add	r3, r3, #1
	cmp	r3, r1
	movcs	r3, r1
	str	r3, [r5, #572]
	b	.L1090
.L1067:
	ldrb	r3, [r5, #532]	@ zero_extendqisi2
	b	.L1089
.L1172:
	movw	ip, #41528
	ldr	r2, [r5, #2168]
	ldr	r3, [fp, #-244]
	mov	r1, r4
	movt	ip, 169
	add	ip, r6, ip
	str	ip, [sp]
	bl	MVC_GenPiclistfromFrmlist_Interview
	b	.L1076
.L1033:
	cmp	r3, #1
	ldr	r3, [r7, #2112]
	addeq	r3, r3, #612
	addne	r3, r3, #648
	str	r3, [r6, #256]
	b	.L1034
.L1068:
	ldr	lr, [r6, #388]
	b	.L1071
.L1173:
	movw	r4, #32736
	movw	r3, #43132
	movt	r4, 169
	add	r4, r6, r4
	mov	r0, r6
	movt	r3, 169
	mov	r2, r4
	add	r3, r6, r3
	bl	mvc_append_interview_list
	ldrb	r0, [r7, #1595]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1078
	ldr	r1, [r5, #2172]
	ldr	ip, [r5, #572]
	cmp	r1, #0
	beq	.L1079
	add	r2, ip, #96
	movw	r3, #33312
	movt	r3, 169
	add	r3, r6, r3
	add	r2, r6, r2, lsl #2
.L1080:
	add	r0, r0, #1
	str	r3, [r2, #4]!
	cmp	r0, r1
	add	r3, r3, #688
	bne	.L1080
	add	ip, ip, r0
.L1079:
	str	ip, [r5, #572]
	ldrb	r1, [r5, #520]	@ zero_extendqisi2
	ldr	r0, [r5, #568]
	b	.L1073
.L1166:
	mov	r3, r4, asl #2
	mov	r1, r8
	str	r3, [fp, #-244]
	mov	r2, r3
	ldr	r3, .L1180+12
	add	r0, r10, r2
	mov	r2, #4
	bl	qsort
	subs	r9, r4, #0
	bne	.L1088
.L1161:
	sub	ip, fp, #172
	b	.L1063
.L1165:
	add	r9, r4, #64
	ldr	r3, .L1180+24
	mov	r2, #4
	mov	r1, r8
	add	r9, r6, r9, lsl #2
	mov	r0, r9
	bl	qsort
	subs	r10, r4, #0
	bne	.L1087
	b	.L1050
.L1095:
	mov	r4, ip
	b	.L1015
.L1096:
	mov	r8, r4
	b	.L1018
.L1097:
	mov	r4, r10
	b	.L1052
.L1171:
	sub	r10, fp, #236
	ldr	r3, .L1180+8
	mov	r1, ip
	mov	r2, #4
	mov	r0, r10
	str	ip, [fp, #-244]
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	ip, [fp, #-244]
	ldr	r8, [r3, #2384]
	cmp	r8, #0
	movwne	r9, #47300
	movne	r4, ip
	movtne	r9, 169
	addne	r9, r6, r9
	bne	.L1093
	ldr	r3, .L1180+12
	mov	r2, #4
	mov	r1, r8
	mov	r0, r10
	mov	r9, r8
	bl	qsort
	b	.L1161
.L1041:
	add	r3, r0, #256
	mov	r2, #4
	str	r3, [fp, #-244]
	mov	r1, r10
	mov	r0, r3
	ldr	r3, .L1180+20
	bl	qsort
	ldr	r3, [fp, #-240]
	ldr	r8, [r3, #2384]
	cmp	r8, #0
	movwne	r9, #47300
	movne	r4, r10
	movtne	r9, 169
	addne	r9, r6, r9
	bne	.L1091
	ldr	r9, [fp, #-244]
	mov	r2, #4
	ldr	r3, .L1180+24
	mov	r1, r8
	mov	r10, r8
	mov	r0, r9
	bl	qsort
	b	.L1050
.L1078:
	movw	ip, #41532
	ldr	r2, [r5, #2172]
	mov	r1, r4
	movt	ip, 169
	add	r3, r6, #388
	add	ip, r6, ip
	str	ip, [sp]
	bl	MVC_GenPiclistfromFrmlist_Interview
	b	.L1158
.L1174:
	ldr	r3, .L1180+32
	mov	r0, #1
	ldr	r1, .L1180+36
.L1160:
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r6
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	b	.L1152
.L1032:
	ldr	r3, .L1180+32
	mov	r0, #1
	ldr	r1, .L1180+40
	b	.L1160
.L1179:
	ldr	r3, .L1180+32
	mov	r0, #1
	ldr	r1, .L1180+44
	b	.L1160
.L1181:
	.align	2
.L1180:
	.word	MVC_compare_pic_by_pic_num_desc
	.word	MVC_compare_pic_by_lt_pic_num_asc
	.word	MVC_compare_fs_by_poc_desc
	.word	MVC_compare_fs_by_poc_asc
	.word	MVC_compare_fs_by_lt_pic_idx_asc
	.word	MVC_compare_pic_by_poc_desc
	.word	MVC_compare_pic_by_poc_asc
	.word	MVC_compare_fs_by_frame_num_desc
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC26
	.word	.LC25
	.word	.LC27
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	ldr	r3, .L1195
	ldr	r3, [r3]
	tst	r3, #8192
	beq	.L1182
	movw	r6, #41524
	ldr	r9, .L1195+4
	movt	r6, 169
	add	r8, r0, #252
	add	r6, r0, r6
	mov	r7, #0
.L1184:
	ldr	r3, [r6, #4]!
	cmp	r3, #0
	movne	r5, r8
	movne	r4, #0
	beq	.L1186
.L1185:
	ldr	ip, [r5, #4]!
	mov	r3, r4
	ldr	r10, [r9, #68]
	mov	r2, r7
	ldr	r1, .L1195+8
	mov	r0, #13
	ldr	ip, [ip, #4]
	add	r4, r4, #1
	ldr	lr, [ip, #32]
	str	lr, [sp, #4]
	ldr	ip, [ip, #20]
	str	ip, [sp]
	blx	r10
	ldr	r3, [r6]
	cmp	r3, r4
	bhi	.L1185
.L1186:
	add	r7, r7, #1
	add	r8, r8, #132
	cmp	r7, #2
	bne	.L1184
.L1182:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1196:
	.align	2
.L1195:
	.word	g_PrintEnable
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r2, [r3, #2380]
	cmp	r2, #0
	beq	.L1202
	ldr	r4, [r3, #2376]
	add	r3, r0, #11141120
	add	r3, r3, #8192
	cmp	r4, #0
	ldr	r5, [r3, #2132]
	beq	.L1203
	movw	ip, #47236
	mov	r6, #0
	movt	ip, 169
	add	ip, r0, ip
	mov	r1, r6
	mvn	lr, #-2147483648
.L1201:
	ldr	r2, [ip, #4]!
	add	r1, r1, #1
	cmp	r2, #0
	beq	.L1200
	ldr	r3, [r2, #32]
	rsb	r3, r3, r5
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, lr
	ldrlt	r6, [r2, #268]
	movlt	lr, r3
.L1200:
	cmp	r1, r4
	bne	.L1201
.L1199:
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1203:
	mov	r6, r4
	b	.L1199
.L1202:
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
	ldrb	r2, [r3, #520]	@ zero_extendqisi2
	cmp	r2, #2
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	add	r2, r0, #11141120
	movw	r6, #47668
	add	r2, r2, #8192
	ldrb	lr, [r2, #1595]	@ zero_extendqisi2
	cmp	lr, #0
	bne	.L1238
	ldr	r4, [r3, #568]
	movw	r5, #47672
	movt	r6, 169
	movt	r5, 169
	cmp	r4, #0
	add	r6, r0, r6
	add	r5, r0, r5
	mov	r7, lr
	beq	.L1216
.L1240:
	add	r2, r0, #252
	mov	ip, #32
	mov	r3, #0
	b	.L1214
.L1213:
	add	r3, r3, #1
	cmp	r3, r4
	beq	.L1239
.L1214:
	ldr	r1, [r2, #4]!
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #52]
	cmp	r1, lr
	bne	.L1213
	cmp	ip, r3
	movcs	ip, r3
	add	r3, r3, #1
	cmp	r3, r4
	str	ip, [r5, #-4]
	str	ip, [r6, #4]
	bne	.L1214
.L1239:
	cmp	ip, #31
	bhi	.L1216
.L1215:
	add	lr, lr, #1
	add	r6, r6, #8
	cmp	lr, #16
	add	r5, r5, #8
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r4, #0
	bne	.L1240
.L1216:
	str	r7, [r5, #-4]
	str	r7, [r6, #4]
	b	.L1215
.L1238:
	ldr	r5, [r3, #568]
	mov	lr, #0
	movt	r6, 169
	mov	r7, lr
	cmp	r5, #0
	add	r6, r0, r6
	beq	.L1220
.L1242:
	add	r1, r0, #252
	mov	r4, #32
	mov	r2, #0
	b	.L1218
.L1217:
	add	r2, r2, #1
	cmp	r2, r5
	beq	.L1241
.L1218:
	ldr	r3, [r1, #4]!
	ldr	r8, [r3, #4]
	ldrb	ip, [r3]	@ zero_extendqisi2
	ldr	r3, [r8, #52]
	mov	r3, r3, asl #1
	cmp	ip, #2
	orreq	r3, r3, #1
	cmp	r3, lr
	bne	.L1217
	cmp	r4, r2
	movcs	r4, r2
	add	r2, r2, #1
	cmp	r2, r5
	str	r4, [r6]
	bne	.L1218
.L1241:
	cmp	r4, #31
	bhi	.L1220
.L1219:
	add	lr, lr, #1
	add	r6, r6, #4
	cmp	lr, #32
	ldmeqfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	cmp	r5, #0
	bne	.L1242
.L1220:
	str	r7, [r6]
	b	.L1219
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
	mov	r5, r0
	bl	MVC_FindNearestPOCPicId
	add	r6, r5, #11075584
	mvn	r4, #0
	add	r3, r6, #40960
	ldrb	r2, [r3, #520]	@ zero_extendqisi2
	str	r4, [r3, #2152]
	cmp	r2, #2
	str	r0, [r3, #2156]
	beq	.L1250
	mov	r0, r5
	bl	MVC_InitListX
	cmp	r0, #0
	bne	.L1251
	mov	r0, r5
	bl	MVC_ReorderListX
	mov	r0, r5
	bl	MVC_RepairList
	cmp	r0, #0
	bne	.L1248
	mov	r0, r5
	add	r6, r6, #40960
	bl	MVC_FindMinRefIdx
	ldr	r3, [r5, #256]
	mov	r0, #0
	cmp	r3, #0
	ldrne	r3, [r3, #4]
	ldrne	r4, [r3, #268]
	str	r4, [r6, #2152]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1250:
	mov	r4, #0
	str	r4, [r3, #568]
	str	r4, [r3, #572]
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1248:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1251:
	ldr	r3, .L1252
	mov	r0, #13
	ldr	r1, .L1252+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1253:
	.align	2
.L1252:
	.word	vfmw_Osal_Func_Ptr_S
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
	beq	.L1261
	movw	r3, #47807
	mov	r2, #0
	movt	r3, 169
	mov	lr, r2
	add	r3, r0, r3
	b	.L1260
.L1269:
	ldrb	r1, [r3, #-2]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L1258
	cmp	r2, ip
	add	r3, r3, #688
	beq	.L1261
.L1260:
	ldrb	r1, [r3]	@ zero_extendqisi2
	add	r2, r2, #1
	cmp	r1, #1
	bne	.L1269
.L1258:
	cmp	r2, ip
	strb	lr, [r3, #-5]
	strb	lr, [r3, #-4]
	add	r3, r3, #688
	bne	.L1260
.L1261:
	ldr	r2, [r0, #52]
	cmp	r2, #0
	beq	.L1270
	mov	r3, #0
	add	r0, r0, #144
	mov	r1, r3
.L1262:
	add	r3, r3, #1
	str	r1, [r0, #4]!
	cmp	r3, r2
	bne	.L1262
	ldmfd	sp, {fp, sp, pc}
.L1270:
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
	mov	r6, r0
	ldr	r0, [r0, #120]
	bl	FSP_GetFspType
	cmp	r0, #0
	beq	.L1288
.L1272:
	ldr	r3, [r6, #48]
	cmp	r3, #0
	beq	.L1279
	movw	r5, #48096
	mov	r8, #0
	movt	r5, 169
	add	r5, r6, r5
	mov	r7, r8
.L1278:
	sub	r4, r5, #16
	strb	r7, [r5, #-294]
	strb	r7, [r5, #-293]
.L1277:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L1276
	ldr	r0, [r6, #120]
	bl	FreeUsdByDec
	str	r7, [r4]
.L1276:
	cmp	r4, r5
	bne	.L1277
	ldr	r3, [r6, #48]
	add	r8, r8, #1
	add	r5, r5, #688
	cmp	r3, r8
	bhi	.L1278
.L1279:
	ldr	r2, [r6, #52]
	cmp	r2, #0
	beq	.L1289
	mov	r3, #0
	add	r6, r6, #144
	mov	r1, r3
.L1280:
	add	r3, r3, #1
	str	r1, [r6, #4]!
	cmp	r3, r2
	bne	.L1280
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1288:
	add	r0, r6, #584
	bl	ResetVoQueue
	b	.L1272
.L1289:
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
.L1292:
	add	r3, r3, #1
	cmp	r0, r2, asl r3
	bge	.L1292
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
	add	r7, ip, #40960
	mov	r5, r1
	mov	r4, r0
	ldr	r0, [r0, #252]
	ldrb	r1, [r7, #532]	@ zero_extendqisi2
	mov	lr, #2240
	ldr	r3, [r7, #536]
	sxtb	r2, r1
	cmn	r2, #1
	mla	r3, lr, r3, r0
	beq	.L1312
	cmp	r1, #0
	bne	.L1297
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	ldr	r3, [r3, #28]
	cmp	r2, #1
	beq	.L1313
	ldr	r2, [r4, #28]
	cmp	r2, r3
	movweq	r6, #35364
	movteq	r6, 168
	addeq	r6, r4, r6
	bne	.L1314
.L1296:
	ldrb	r3, [r6, #20]	@ zero_extendqisi2
	ldr	r1, [r6, #3952]
	rsb	r3, r3, #2
	ldr	r2, [r6, #3948]
	ldr	r0, [r4, #12]
	mla	r3, r1, r3, r3
	ldr	r1, [r4, #16]
	add	r2, r2, #1
	cmp	r2, r0
	cmpeq	r3, r1
	movne	lr, #1
	moveq	lr, #0
	beq	.L1315
.L1301:
	ldr	ip, .L1317
	ldr	ip, [ip]
	cmp	ip, #0
	beq	.L1308
	mov	r0, r0, asl #4
	mov	r1, r1, asl #4
	strh	r0, [fp, #-44]	@ movhi
	mov	r2, r2, asl #4
	strh	r1, [fp, #-42]	@ movhi
	mov	r1, r3, asl #4
	strh	r2, [fp, #-40]	@ movhi
	mov	r3, #8
	strh	r1, [fp, #-38]	@ movhi
	sub	r2, fp, #44
	mov	r1, #2
	ldr	r0, [r4, #120]
	blx	ip
	mov	lr, #1
	mov	r0, lr
.L1302:
	ldrb	r3, [r7, #531]	@ zero_extendqisi2
	cmp	r3, #1
	ldrne	ip, [r6, #3972]
	subne	ip, ip, #1
	beq	.L1316
.L1305:
	ldr	r3, [r4, #44]
	cmp	r3, ip
	orrne	lr, lr, #1
	str	lr, [r5]
	str	ip, [r4, #44]
.L1310:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1315:
	add	ip, ip, #45056
	ldr	r8, [r6, #3972]
	ldr	ip, [ip, #2376]
	add	ip, ip, #1
	cmp	r8, ip
	bhi	.L1301
.L1311:
	mov	r0, lr
	b	.L1302
.L1312:
	ldr	r2, [r3, #28]
	movw	r6, #3992
	ldr	r3, [r4, #248]
	mla	r6, r6, r2, r3
	b	.L1296
.L1316:
	movw	r1, #39336
	add	r3, r4, #290816
	movt	r1, 168
	add	r3, r3, #808
	add	r1, r4, r1
	mov	ip, #0
.L1304:
	ldr	r2, [r3]
	add	r3, r3, #335872
	add	r3, r3, #308
	cmp	r2, ip
	subhi	ip, r2, #1
	cmp	r3, r1
	bne	.L1304
	add	ip, ip, #1
	mov	ip, ip, asl #1
	cmp	ip, #16
	movcs	ip, #16
	b	.L1305
.L1308:
	mov	lr, #1
	b	.L1311
.L1313:
	movw	r6, #8500
	movt	r6, 5
	mla	r6, r6, r3, r4
	add	r6, r6, #286720
	add	r6, r6, #932
	b	.L1296
.L1314:
	ldr	r3, .L1317+4
	mov	r0, #1
	ldr	r1, .L1317+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1310
.L1297:
	ldr	r3, .L1317+4
	mov	r0, #1
	ldr	r1, .L1317+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1310
.L1318:
	.align	2
.L1317:
	.word	g_event_report
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC30
	.word	.LC31
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
	ldrb	r1, [r7, #523]	@ zero_extendqisi2
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
	ldrb	r2, [r4, #3480]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1355
	ldr	r1, [r4, #3536]
	ldr	r0, [r4, #3532]
	ldr	r2, [r4, #3544]
	cmp	r1, r0
	addcc	r2, r2, r3
	strcc	r2, [r4, #3540]
	bcs	.L1357
.L1358:
	add	r3, r2, r1
	str	r3, [r4, #3528]
	ldrb	r0, [r7, #528]	@ zero_extendqisi2
	mov	r3, r3, asl #1
	cmp	r0, #0
	subeq	r3, r3, #1
	str	r3, [r4, #3516]
	ldrb	r0, [r7, #521]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1361
	str	r3, [r4, #3512]
	str	r3, [r4, #3508]
	str	r3, [r4, #3504]
.L1354:
	add	r6, r6, #36864
	str	r1, [r6, #3532]
	str	r2, [r6, #3544]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1322:
	cmp	r1, #0
	add	lr, lr, #4
	mov	r2, r2, asl lr
	bne	.L1378
	add	r4, r6, #36864
	ldrb	r3, [r4, #3480]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1326
	ldr	lr, [r4, #3492]
	ldr	ip, [r4, #3496]
	mov	r1, lr
.L1327:
	ldr	r0, [r4, #3484]
	cmp	r0, ip
	bcs	.L1325
	rsb	r3, r0, ip
	cmp	r3, r2, lsr #1
	addcs	r1, r1, r2
	strcs	r1, [r4, #3500]
	bcs	.L1329
.L1325:
	cmp	r0, ip
	bls	.L1330
	rsb	r3, ip, r0
	cmp	r3, r2, lsr #1
	rsbhi	r1, r2, r1
	strhi	r1, [r4, #3500]
	bls	.L1330
.L1329:
	ldrb	r3, [r7, #521]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1331
.L1380:
	ldr	r3, [r4, #3488]
	add	r1, r1, r0
	str	r1, [r4, #3504]
	add	r3, r1, r3
	str	r3, [r4, #3508]
	cmp	r3, r1
	movge	r3, r1
	str	r3, [r4, #3516]
.L1332:
	ldr	r2, [r4, #3536]
	str	r3, [r4, #3512]
	add	r3, r6, #40960
	ldr	r1, [r4, #3532]
	cmp	r2, r1
	strne	r2, [r4, #3532]
	ldrb	r3, [r3, #528]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r6, r6, #36864
	ldrne	r2, [r6, #3484]
	ldrne	r3, [r6, #3500]
	strne	r2, [r6, #3496]
	strne	r3, [r6, #3492]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1321:
	cmp	r1, #0
	add	r4, r6, #36864
	movne	r3, #0
	strne	r3, [r4, #3540]
	bne	.L1336
	ldrb	r2, [r4, #3480]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1337
	mov	r3, r1
	str	r1, [r4, #3544]
	str	r1, [r4, #3532]
.L1338:
	str	r3, [r4, #3540]
.L1336:
	ldr	r3, [r5, #2916]
	cmp	r3, #0
	beq	.L1340
	ldr	r3, [r4, #3536]
	ldr	r0, [r4, #3540]
	add	r0, r3, r0
	str	r0, [r4, #3528]
	ldrb	r8, [r7, #528]	@ zero_extendqisi2
	cmp	r8, #0
	bne	.L1342
	cmp	r0, #0
	beq	.L1343
	sub	r0, r0, #1
	str	r0, [r4, #3528]
.L1342:
	mov	r3, #0
	str	r3, [r4, #3560]
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
	str	r10, [r4, #3552]
	ldr	r1, [r5, #2916]
	bl	__aeabi_uidivmod
	ldr	r0, [r4, #3560]
	mul	r0, r0, r10
	str	r0, [r4, #3556]
	cmp	r1, #0
	str	r1, [r4, #3548]
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
	str	r0, [r4, #3556]
	bne	.L1348
.L1347:
	cmp	r8, #0
	ldreq	r3, [r5, #2908]
	addeq	r0, r0, r3
	streq	r0, [r4, #3556]
	ldrb	r3, [r7, #521]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1350
	ldr	r2, [r4, #3520]
	ldr	r1, [r4, #3524]
	add	r2, r0, r2
	str	r2, [r4, #3504]
	ldr	r3, [r5, #2912]
	add	r3, r2, r3
	add	r0, r3, r1
	str	r0, [r4, #3508]
	cmp	r0, r2
	movge	r0, r2
	str	r0, [r4, #3516]
.L1351:
	add	r6, r6, #36864
	ldr	r2, [r6, #3536]
	ldr	r3, [r6, #3540]
	str	r0, [r6, #3512]
	str	r2, [r6, #3532]
	str	r3, [r6, #3544]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1340:
	str	r3, [r4, #3528]
	ldrb	r8, [r7, #528]	@ zero_extendqisi2
.L1343:
	mov	r0, #0
	str	r0, [r4, #3560]
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
	str	r3, [r4, #3560]
	ldr	r1, [r5, #2916]
	cmp	r1, r2
	bgt	.L1345
	cmp	r0, #0
	bne	.L1379
.L1346:
	mov	r0, #0
	str	r0, [r4, #3556]
	b	.L1347
.L1378:
	add	r4, r0, #11075584
	add	r4, r4, #36864
.L1328:
	mov	r3, #0
	ldr	r0, [r4, #3484]
	str	r3, [r4, #3492]
	mov	lr, r3
	str	r3, [r4, #3496]
	mov	r1, ip
	b	.L1325
.L1377:
	add	r3, r6, #36864
	mov	r1, #0
	mov	r2, r1
	str	r1, [r3, #3540]
	str	r1, [r3, #3508]
	str	r1, [r3, #3504]
	str	r1, [r3, #3512]
	str	r1, [r3, #3516]
	ldr	r1, [r3, #3536]
	b	.L1354
.L1330:
	str	lr, [r4, #3500]
	ldrb	r3, [r7, #521]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1380
.L1331:
	ldrb	r3, [r7, #522]	@ zero_extendqisi2
	cmp	r3, #0
	add	r3, r1, r0
	streq	r3, [r4, #3504]
	strne	r3, [r4, #3508]
	str	r3, [r4, #3516]
	b	.L1332
.L1350:
	ldrb	r3, [r7, #522]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r2, [r5, #2912]
	ldreq	r3, [r4, #3520]
	ldrne	r3, [r4, #3520]
	addne	r0, r0, r2
	addeq	r0, r0, r3
	streq	r0, [r4, #3504]
	addne	r0, r0, r3
	strne	r0, [r4, #3508]
	str	r0, [r4, #3516]
	b	.L1351
.L1326:
	movw	r3, #3481
	ldrsb	r3, [r4, r3]
	cmp	r3, #0
	bne	.L1328
	ldr	r0, [r4, #3504]
	mov	r1, ip
	mov	lr, r3
	str	r3, [r4, #3492]
	mov	ip, r0
	str	r0, [r4, #3496]
	b	.L1327
.L1337:
	ldr	r1, [r4, #3536]
	ldr	r2, [r4, #3532]
	cmp	r1, r2
	bcs	.L1339
	ldr	r2, [r4, #3544]
	add	r3, r3, r2
	str	r3, [r4, #3540]
	b	.L1336
.L1361:
	ldrb	r0, [r7, #522]	@ zero_extendqisi2
	str	r3, [r4, #3512]
	cmp	r0, #0
	streq	r3, [r4, #3504]
	strne	r3, [r4, #3508]
	b	.L1354
.L1355:
	str	r1, [r4, #3532]
	mov	r2, r1
	str	r1, [r4, #3544]
	ldr	r1, [r4, #3536]
.L1357:
	str	r2, [r4, #3540]
	b	.L1358
.L1339:
	ldr	r3, [r4, #3544]
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
	add	r5, r5, #8192
	mov	r6, #1
	ldrb	r2, [r5, #1595]	@ zero_extendqisi2
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
	ldr	r1, [r3, #2384]
	clz	r2, r2
	mov	r7, r7, lsr #5
	cmp	r1, #0
	mov	r2, r2, lsr #5
	movwne	lr, #47300
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
	ldr	r8, [r5, #2120]
	cmp	r4, r8
	rsbhi	r4, r6, r4
	str	r4, [ip, #24]
	ldr	ip, [lr]
	ldrb	r4, [ip, #3]	@ zero_extendqisi2
	tst	r4, #1
	beq	.L1399
	ldr	r8, [ip, #612]
	bic	r8, r8, #-16777216
	bic	r8, r8, #255
	cmp	r8, #65536
	ldreq	r4, [ip, #24]
	addeq	r4, r7, r4, lsl #1
	streq	r4, [ip, #624]
	ldreq	ip, [lr]
	ldreqb	r4, [ip, #3]	@ zero_extendqisi2
.L1399:
	tst	r4, #2
	beq	.L1396
	ldr	r4, [ip, #648]
	bic	r4, r4, #-16777216
	bic	r4, r4, #255
	cmp	r4, #65536
	ldreq	r4, [ip, #24]
	addeq	r4, r2, r4, lsl #1
	streq	r4, [ip, #660]
.L1396:
	ldr	ip, [r3, #2384]
	add	r1, r1, #1
	cmp	ip, r1
	bhi	.L1401
.L1402:
	ldr	r1, [r3, #2388]
	cmp	r1, #0
	beq	.L1426
	movw	r4, #47364
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
	ldr	r0, [lr, #612]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #256
	ldreq	r1, [lr, #28]
	addeq	r1, r7, r1, lsl #1
	streq	r1, [lr, #620]
	ldreq	lr, [r4]
	ldreqb	r1, [lr, #3]	@ zero_extendqisi2
.L1405:
	tst	r1, #2
	beq	.L1404
	ldr	r1, [lr, #648]
	bic	r1, r1, #-16777216
	bic	r1, r1, #255
	cmp	r1, #256
	ldreq	r1, [lr, #28]
	addeq	r1, r2, r1, lsl #1
	streq	r1, [lr, #656]
.L1404:
	ldr	r1, [r3, #2388]
	cmp	r1, ip
	bhi	.L1407
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1382:
	add	r3, r3, #45056
	ldr	r1, [r3, #2384]
	cmp	r1, #0
	movwne	lr, #47300
	movtne	lr, 169
	addne	lr, r0, lr
	bne	.L1391
.L1392:
	ldr	r2, [r3, #2388]
	cmp	r2, #0
	beq	.L1427
	movw	ip, #47364
	mov	r2, #0
	movt	ip, 169
	add	ip, r0, ip
	b	.L1394
.L1393:
	ldr	r1, [r3, #2388]
	cmp	r1, r2
	bls	.L1428
.L1394:
	ldr	r1, [ip, #4]!
	add	r2, r2, #1
	ldrb	r0, [r1, #3]	@ zero_extendqisi2
	cmp	r0, #3
	bne	.L1393
	ldr	r0, [r1, #576]
	bic	r0, r0, #-16777216
	bic	r0, r0, #255
	cmp	r0, #256
	ldreq	r0, [r1, #28]
	streq	r0, [r1, #584]
	ldr	r1, [r3, #2388]
	cmp	r1, r2
	bhi	.L1394
.L1428:
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1429:
	ldr	ip, [r1, #576]
	bic	ip, ip, #-16777216
	bic	ip, ip, #255
	cmp	ip, #65536
	bne	.L1388
	ldr	ip, [r1, #20]
	ldr	r4, [r5, #2120]
	rsb	r7, r6, ip
	cmp	ip, r4
	strhi	r7, [r1, #24]
	strls	ip, [r1, #24]
	ldr	r1, [lr]
	ldr	ip, [r1, #24]
	str	ip, [r1, #588]
.L1388:
	ldr	r1, [r3, #2384]
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
	ldr	lr, [r3, #2376]
	cmp	lr, #0
	beq	.L1435
	ldr	r2, [r3, #2184]
	rsb	r3, r2, r1
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L1437
	movw	ip, #47240
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
	movweq	r3, #6960
	ldreq	ip, .L1484
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
.L1479:
	ldr	ip, .L1484
	mov	r0, r3
	movw	r3, #6961
.L1478:
	ldr	r2, .L1484+12
	ldr	r1, .L1484+16
	ldr	ip, [ip, #68]
	blx	ip
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
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	add	r5, r0, #11141120
	add	r3, r5, #8192
	ldr	r6, .L1489
	add	r5, r5, #8192
	ldr	r1, .L1489+4
	ldrb	r4, [r3, #1595]	@ zero_extendqisi2
	mov	r0, #2
	ldr	r3, [r3, #2112]
	ldr	ip, [r6, #68]
	mov	r2, r4
	ldr	r3, [r3, #268]
	blx	ip
	ldr	lr, [r5, #2172]
	cmp	r4, #3
	cmpne	r4, #0
	ldr	r3, [r5, #2168]
	addne	r2, r4, #1
	ldr	ip, [r6, #68]
	moveq	r2, #1
	ldr	r1, .L1489+8
	str	lr, [sp]
	mov	r0, #22
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1490:
	.align	2
.L1489:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r10, r0, #11141120
	add	r4, r10, #8192
	ldr	ip, .L1527
	add	lr, r10, #12288
	str	lr, [fp, #-48]
	ldr	r5, [r4, #2188]
	mov	r6, r0
	ldr	lr, [r4, #1804]
	mov	r0, #2
	ldr	r8, [ip, #68]
	add	r7, r6, #11075584
	ldrb	r3, [r4, #1595]	@ zero_extendqisi2
	add	r9, r7, #32768
	ldr	r2, [r4, #2128]
	ldr	r1, .L1527+4
	str	r5, [sp, #4]
	add	r5, r7, #36864
	str	lr, [sp]
	str	ip, [fp, #-52]
	blx	r8
	movw	r3, #9808
	mov	r2, #0
	movt	r3, 170
	str	r2, [r6, #64]
	ldrd	r0, [r6, r3]
	add	r3, r10, #11584
	ldr	lr, [fp, #-48]
	strd	r0, [r3]
	ldr	r3, [r6, #544]
	str	r3, [lr, #156]
	ldr	r1, [r4, #1800]
	ldr	r2, [r4, #2128]
	ldrb	r3, [r4, #1595]	@ zero_extendqisi2
	str	r1, [r4, #4040]
	str	r2, [r4, #3400]
	strb	r3, [r4, #3384]
	ldr	r3, [r6, #236]
	ldr	r0, [r4, #2172]
	ldr	r1, [r4, #2176]
	ldrb	r2, [r3, #21]	@ zero_extendqisi2
	str	r0, [r4, #3404]
	str	r1, [r4, #3408]
	str	r2, [r4, #3416]
	ldr	r2, [r3, #3948]
	add	r2, r2, #1
	str	r2, [r4, #3412]
	ldrb	r2, [r9, #1364]	@ zero_extendqisi2
	strb	r2, [r4, #3387]
	ldr	r2, [r5, #1300]
	str	r2, [r4, #3420]
	ldrb	r2, [r5, #1240]	@ zero_extendqisi2
	str	r2, [r4, #3424]
	ldr	r3, [r3, #748]
	strb	r3, [r4, #3385]
	ldrb	r3, [r5, #1244]	@ zero_extendqisi2
	str	r3, [r4, #3428]
	ldr	r3, [r5, #3516]
	str	r3, [r4, #3432]
	ldr	r3, [r5, #3504]
	str	r3, [r4, #3436]
	ldr	r3, [r4, #2112]
	ldr	r2, [r5, #3508]
	str	r2, [r4, #3440]
	ldrsb	r1, [r3, #6]
	ldr	r0, [r6, #120]
	bl	FSP_GetLogicFs
	ldr	ip, [fp, #-52]
	subs	r8, r0, #0
	beq	.L1525
	ldr	r2, [r8, #520]
	ldr	r3, [r8, #524]
	cmp	r2, #0
	beq	.L1495
	cmp	r3, #0
	beq	.L1495
	movw	r3, #12012
	movw	r2, #11884
	movw	r1, #12140
	ldr	r0, [r6, #120]
	movt	r3, 170
	movt	r2, 170
	add	r3, r6, r3
	add	r2, r6, r2
	movt	r1, 170
	add	r1, r6, r1
	bl	FSP_GetDecFsAddrTab
	ldr	r3, [r8, #520]
	movw	r2, #12144
	add	r1, r10, #12224
	movt	r2, 170
	add	r2, r6, r2
	ldr	r3, [r3, #4]
	add	r7, r7, #45056
	str	r3, [r4, #3676]
	ldr	r0, [r6, #120]
	bl	FSP_GetPmvAddrTab
	ldr	r3, [r4, #2188]
	ldr	r2, [fp, #-48]
	str	r3, [r4, #3684]
	ldr	r3, [r8, #536]
	str	r3, [r4, #4036]
	ldr	r3, [r8, #524]
	ldr	r3, [r3, #12]
	str	r3, [r4, #3680]
	ldr	r3, [r7, #2604]
	cmp	r3, #0
	str	r3, [fp, #-52]
	str	r3, [r2, #148]
	beq	.L1501
	movw	r4, #47464
	movw	lr, #12240
	movw	ip, #47528
	movw	r0, #12304
	movw	r1, #47592
	movw	r2, #12368
	ldr	r10, [fp, #-52]
	movt	r4, 169
	movt	lr, 170
	movt	ip, 169
	movt	r0, 170
	movt	r1, 169
	movt	r2, 170
	add	r4, r6, r4
	add	lr, r6, lr
	add	ip, r6, ip
	add	r0, r6, r0
	add	r1, r6, r1
	add	r2, r6, r2
	mov	r3, #0
.L1500:
	ldr	r8, [r4, #4]!
	add	r3, r3, #1
	cmp	r3, r10
	str	r8, [lr, #4]!
	ldr	r8, [ip, #4]!
	str	r8, [r0, #4]!
	ldr	r8, [r1, #4]!
	str	r8, [r2, #4]!
	bne	.L1500
.L1501:
	ldrb	r3, [r9, #1371]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1526
	ldrb	ip, [r5, #1258]	@ zero_extendqisi2
	cmp	ip, #0
	bne	.L1502
	movw	lr, #11632
	ldr	r0, .L1527+8
	movt	lr, 170
	add	lr, r6, lr
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
	str	r3, [lr, #4]!
	bne	.L1503
	movw	r8, #11732
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
	ldr	r3, [fp, #-52]
	cmp	r3, #0
	beq	.L1506
	movw	r4, #47468
	movw	r9, #12572
	movw	r8, #12636
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
	ldr	r3, [r7, #2604]
	cmp	r3, r5
	bhi	.L1512
.L1506:
	mov	r0, #0
.L1522:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1526:
	ldrb	ip, [r5, #1258]	@ zero_extendqisi2
.L1502:
	cmp	ip, #1
	movw	ip, #11632
	movt	ip, 170
	mov	r0, #0
	add	ip, r6, ip
	beq	.L1507
.L1510:
	movw	r3, #25580
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
	str	r3, [ip, #4]!
	bne	.L1510
	movw	r9, #11732
	mov	r10, #1
	movt	r9, 170
	add	r9, r6, r9
	mov	r8, #0
.L1511:
	mov	r3, r8, lsr #3
	and	r2, r10, #15
	mov	r0, r8, asl #1
	movw	r1, #25604
	mov	r3, r3, asl #4
	and	r0, r0, #14
	add	r2, r3, r2
	movt	r1, 42
	add	r3, r3, r0
	add	r1, r2, r1
	movw	r2, #25604
	add	r8, r8, #1
	movt	r2, 42
	add	r2, r3, r2
	ldr	r1, [r6, r1, asl #2]
	cmp	r8, #16
	ldr	r3, [r6, r2, asl #2]
	add	r10, r10, #2
	and	ip, r1, #-16777216
	ubfx	r0, r1, #8, #8
	uxtb	r5, r1
	mov	r1, r1, lsr #16
	mov	r4, r3, lsr #24
	ubfx	r2, r3, #8, #8
	mov	r5, r5, asl #16
	uxtb	lr, r3
	orr	r1, r5, r1, asl #24
	mov	r3, r3, lsr #8
	orr	r2, r2, r4, asl #8
	orr	r1, r1, lr
	and	r3, r3, #65280
	orr	r2, r2, ip
	orr	r3, r1, r3
	orr	r2, r2, r0, asl #16
	str	r3, [r9]
	add	r9, r9, #8
	str	r2, [r9, #-4]
	bne	.L1511
	b	.L1509
.L1507:
	movw	r3, #26414
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
	str	r3, [ip, #4]!
	bne	.L1507
	movw	r9, #11732
	mov	r10, #1
	movt	r9, 170
	add	r9, r6, r9
	mov	r8, #0
.L1508:
	mov	r3, r8, lsr #3
	and	r2, r10, #15
	mov	r0, r8, asl #1
	movw	r1, #26438
	mov	r3, r3, asl #4
	and	r0, r0, #14
	add	r2, r3, r2
	movt	r1, 42
	add	r3, r3, r0
	add	r1, r2, r1
	movw	r2, #26438
	add	r8, r8, #1
	movt	r2, 42
	add	r2, r3, r2
	ldr	r1, [r6, r1, asl #2]
	cmp	r8, #16
	ldr	r3, [r6, r2, asl #2]
	add	r10, r10, #2
	and	ip, r1, #-16777216
	ubfx	r0, r1, #8, #8
	uxtb	r5, r1
	mov	r1, r1, lsr #16
	mov	r4, r3, lsr #24
	ubfx	r2, r3, #8, #8
	mov	r5, r5, asl #16
	uxtb	lr, r3
	orr	r1, r5, r1, asl #24
	mov	r3, r3, lsr #8
	orr	r2, r2, r4, asl #8
	orr	r1, r1, lr
	and	r3, r3, #65280
	orr	r2, r2, ip
	orr	r3, r1, r3
	orr	r2, r2, r0, asl #16
	str	r3, [r9]
	add	r9, r9, #8
	str	r2, [r9, #-4]
	bne	.L1508
	b	.L1509
.L1495:
	ldr	ip, [ip, #68]
	mov	r0, #0
	ldr	r1, .L1527+12
	blx	ip
	mvn	r0, #0
	b	.L1522
.L1525:
	ldr	r2, [r4, #2112]
	ldr	r3, [ip, #68]
	ldr	r1, .L1527+16
	ldrsb	r2, [r2, #6]
	blx	r3
	mvn	r0, #0
	b	.L1522
.L1528:
	.align	2
.L1527:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r8, r4, #11141120
	ldr	r3, [fp, #-48]
	add	r8, r8, #8192
	add	r5, r4, #11075584
	cmp	r3, #0
	add	r9, r5, #36864
	ldrb	r3, [r8, #1601]	@ zero_extendqisi2
	add	r5, r5, #40960
	strb	r3, [r6, #1]
	ldr	r3, [r9, #1288]
	ldr	r2, [r5, #2136]
	add	r3, r3, #26
	add	r3, r3, r2
	str	r3, [r6, #32]
	bne	.L1635
	ldr	r2, [r8, #2112]
	str	r3, [r2, #604]
	ldr	r3, [r8, #2112]
	ldr	r2, [r6, #32]
	str	r2, [r3, #608]
.L1540:
	ldr	r3, [r5, #2132]
	str	r3, [r6, #36]
	ldr	r3, [r5, #584]
	str	r3, [r6, #40]
	ldr	r3, [r5, #580]
	str	r3, [r6, #44]
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	strb	r3, [r0, r1]
	ldr	r3, [r5, #576]
	str	r3, [r6, #48]
	ldrb	r3, [r5, #526]	@ zero_extendqisi2
	strb	r3, [r6, #2]
	ldr	r3, [r4, #236]
	ldrb	r3, [r3, #22]	@ zero_extendqisi2
	strb	r3, [r6, #3]
	ldr	r3, [r5, #568]
	str	r3, [r6, #52]
	ldr	r3, [r5, #572]
	str	r3, [r6, #56]
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r9, #1284]
	streqb	r3, [r6, #4]
	beq	.L1542
	cmp	r3, #0
	ldreqb	r3, [r9, #1242]	@ zero_extendqisi2
	movne	r3, #0
	strb	r3, [r6, #4]
.L1542:
	ldr	r3, [r9, #1292]
	str	r3, [r6, #60]
	ldr	r3, [r9, #1296]
	str	r3, [r6, #64]
	ldr	r3, [r5, #2144]
	str	r3, [r6, #68]
	ldr	r3, [r5, #2148]
	str	r3, [r6, #72]
	ldr	r3, [r5, #2140]
	str	r3, [r6, #76]
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1581
	ldr	r7, [r5, #568]
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
	str	r3, [fp, #-52]
.L1546:
	ldr	ip, [r5, #572]
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
	str	r3, [fp, #-56]
.L1552:
	ldrb	ip, [r8, #1595]	@ zero_extendqisi2
	ldr	r3, [r5, #568]
	cmp	ip, #0
	beq	.L1558
	cmp	r3, #0
	beq	.L1573
	mov	r3, r6
	mov	r7, #0
	str	r6, [fp, #-60]
	add	r10, r4, #256
	mov	r6, r7
	str	r8, [fp, #-64]
	mov	r7, r3
	mov	r8, r5
	ldr	r5, [fp, #-52]
	b	.L1572
.L1570:
	strb	r1, [r7, #1630]
	ldr	r1, [r10]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r7, #1631]
	ldr	r1, [r10]
	ldr	r1, [r1, #16]
	str	r1, [r7, #1656]
.L1571:
	ldr	r1, [r8, #568]
	add	r6, r6, #1
	add	r10, r10, #4
	add	r7, r7, #36
	cmp	r1, r6
	bls	.L1636
.L1572:
	ldr	r1, [r10]
	ldr	r0, [r4, #120]
	ldr	r1, [r1, #4]
	ldrsb	r1, [r1, #6]
	bl	FSP_GetLogicFs
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #48]
	str	r1, [r7, #1636]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #2]	@ zero_extendqisi2
	strb	r1, [r7, #1625]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r5
	beq	.L1569
	cmp	r0, #0
	ldrne	r1, [r0, #520]
	ldrne	r0, [r1, #4]
.L1569:
	str	r0, [r7, #1640]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #52]
	str	r1, [r7, #1644]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #32]
	str	r1, [r7, #1632]
	ldr	r1, [r10]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r7, #1624]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #576]	@ zero_extendqisi2
	strb	r1, [r7, #1626]
	ldr	r1, [r10]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L1570
	strb	r1, [r7, #1628]
	ldr	r1, [r10]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r7, #1629]
	ldr	r1, [r10]
	ldr	r1, [r1, #16]
	str	r1, [r7, #1652]
	b	.L1571
.L1636:
	mov	r5, r8
	ldr	r6, [fp, #-60]
	ldr	r8, [fp, #-64]
.L1573:
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1637
.L1561:
	movw	r2, #47664
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
	ldrb	r3, [r9, #1242]	@ zero_extendqisi2
	cmp	r3, #0
	str	r3, [r8, #3668]
	ldr	r3, [r9, #1284]
	str	r3, [r8, #3672]
	beq	.L1579
	ldrb	r2, [r5, #520]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1580
.L1579:
	cmp	r3, #1
	beq	.L1638
.L1581:
	ldr	r2, [fp, #-48]
	movw	r3, #35524
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
	ldr	r2, [r8, #2112]
	ldr	lr, [r2, #604]
	ldr	ip, [r2, #608]
	cmp	lr, r3
	movlt	lr, r3
	cmp	ip, r3
	str	lr, [r2, #604]
	movge	ip, r3
	str	ip, [r2, #608]
	b	.L1540
.L1547:
	ldrsb	r1, [r2, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	ldreq	ip, .L1640
	movweq	r3, #8019
	beq	.L1631
	ldr	r3, [r3, #520]
	ldr	r3, [r3, #4]
	str	r3, [fp, #-52]
	b	.L1546
.L1638:
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1581
.L1580:
	ldr	r3, [r5, #588]
	movw	r8, #41552
	movw	r7, #41680
	movw	lr, #41808
	movw	ip, #42320
	movw	r0, #42448
	str	r3, [r6, #80]
	movw	r1, #42576
	ldr	r10, [r5, #592]
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
	ldr	r10, [r5, #580]
	cmp	r10, r2
	bcs	.L1582
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1581
	movw	r7, #41936
	movw	lr, #42064
	movw	ip, #42192
	movw	r0, #42704
	movw	r1, #42832
	movw	r2, #42960
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
	ldr	r9, [r5, #584]
	cmp	r9, r3
	bcs	.L1583
	b	.L1581
.L1558:
	cmp	r3, #0
	beq	.L1566
	add	r2, r4, #256
	str	r8, [fp, #-64]
	mov	r8, r5
	ldr	r5, [fp, #-52]
	mov	r7, r6
	str	r6, [fp, #-60]
	mov	r10, r2
	mov	r6, ip
.L1565:
	ldr	r1, [r10]
	ldr	r0, [r4, #120]
	ldr	r1, [r1, #4]
	ldrsb	r1, [r1, #6]
	bl	FSP_GetLogicFs
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #48]
	str	r1, [r7, #1636]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #2]	@ zero_extendqisi2
	strb	r1, [r7, #1625]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r5
	beq	.L1564
	cmp	r0, #0
	ldrne	r1, [r0, #520]
	ldrne	r0, [r1, #4]
.L1564:
	str	r0, [r7, #1640]
	mov	r3, #0
	ldr	r1, [r10]
	add	r6, r6, #1
	add	r7, r7, #36
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #52]
	str	r1, [r7, #1608]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #32]
	str	r1, [r7, #1596]
	ldr	r1, [r10]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r7, #1591]
	ldr	r1, [r10]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r7, #1590]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #592]
	str	r1, [r7, #1612]
	ldr	r1, [r10]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #628]
	str	r1, [r7, #1616]
	ldr	r1, [r10], #4
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #664]
	strb	r3, [r7, #1588]
	str	r1, [r7, #1620]
	ldr	r1, [r8, #568]
	cmp	r1, r6
	bhi	.L1565
	mov	r5, r8
	ldr	r6, [fp, #-60]
	ldr	r8, [fp, #-64]
.L1566:
	ldrb	r3, [r5, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1561
	ldr	r3, [r5, #572]
	cmp	r3, #0
	beq	.L1561
	add	r3, r4, #388
	str	r8, [fp, #-60]
	mov	r8, r5
	ldr	r5, [fp, #-56]
	mov	r7, #0
	mov	r10, r6
	str	r6, [fp, #-52]
	mov	r6, r7
	mov	r7, r3
.L1568:
	ldr	r1, [r7]
	ldr	r0, [r4, #120]
	ldr	r1, [r1, #4]
	ldrsb	r1, [r1, #6]
	bl	FSP_GetLogicFs
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #48]
	str	r1, [r10, #2788]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #2]	@ zero_extendqisi2
	strb	r1, [r10, #2777]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r5
	beq	.L1567
	cmp	r0, #0
	ldrne	r1, [r0, #520]
	ldrne	r0, [r1, #4]
.L1567:
	str	r0, [r10, #1640]
	mov	r3, #0
	ldr	r1, [r7]
	add	r6, r6, #1
	add	r10, r10, #36
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #52]
	str	r1, [r10, #2760]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #32]
	str	r1, [r10, #2748]
	ldr	r1, [r7]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2743]
	ldr	r1, [r7]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r10, #2742]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #592]
	str	r1, [r10, #2764]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #628]
	str	r1, [r10, #2768]
	ldr	r1, [r7], #4
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #664]
	strb	r3, [r10, #2740]
	str	r1, [r10, #2772]
	ldr	r1, [r8, #572]
	cmp	r1, r6
	bhi	.L1568
.L1630:
	mov	r5, r8
	ldr	r6, [fp, #-52]
	ldr	r8, [fp, #-60]
	b	.L1561
.L1553:
	ldrsb	r1, [r2, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L1639
	ldr	r3, [r3, #520]
	ldr	r3, [r3, #4]
	str	r3, [fp, #-56]
	b	.L1552
.L1637:
	ldr	r3, [r5, #572]
	cmp	r3, #0
	beq	.L1561
	mov	r3, #0
	mov	r10, r6
	str	r6, [fp, #-52]
	add	r7, r4, #388
	str	r8, [fp, #-60]
	mov	r6, r3
	mov	r8, r5
	ldr	r5, [fp, #-56]
	b	.L1577
.L1575:
	strb	r1, [r10, #2782]
	ldr	r1, [r7]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2783]
	ldr	r1, [r7]
	ldr	r1, [r1, #16]
	str	r1, [r10, #2808]
.L1576:
	ldr	r1, [r8, #572]
	add	r6, r6, #1
	add	r7, r7, #4
	add	r10, r10, #36
	cmp	r1, r6
	bls	.L1630
.L1577:
	ldr	r1, [r7]
	ldr	r0, [r4, #120]
	ldr	r1, [r1, #4]
	ldrsb	r1, [r1, #6]
	bl	FSP_GetLogicFs
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #48]
	str	r1, [r10, #2788]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #2]	@ zero_extendqisi2
	strb	r1, [r10, #2777]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	cmp	r1, #1
	moveq	r0, r5
	beq	.L1574
	cmp	r0, #0
	ldrne	r1, [r0, #520]
	ldrne	r0, [r1, #4]
.L1574:
	str	r0, [r10, #2792]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #52]
	str	r1, [r10, #2796]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #32]
	str	r1, [r10, #2784]
	ldr	r1, [r7]
	ldrb	r1, [r1]	@ zero_extendqisi2
	strb	r1, [r10, #2776]
	ldr	r1, [r7]
	ldr	r1, [r1, #4]
	ldrb	r1, [r1, #576]	@ zero_extendqisi2
	strb	r1, [r10, #2778]
	ldr	r1, [r7]
	ldrb	r1, [r1]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L1575
	strb	r1, [r10, #2780]
	ldr	r1, [r7]
	ldrb	r1, [r1, #1]	@ zero_extendqisi2
	strb	r1, [r10, #2781]
	ldr	r1, [r7]
	ldr	r1, [r1, #16]
	str	r1, [r10, #2804]
	b	.L1576
.L1639:
	ldr	ip, .L1640
	movw	r3, #8040
.L1631:
	ldr	r2, .L1640+4
	ldr	r1, .L1640+8
	ldr	ip, [ip, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L1584:
	str	r7, [fp, #-52]
	b	.L1546
.L1585:
	str	ip, [fp, #-56]
	b	.L1552
.L1641:
	.align	2
.L1640:
	.word	vfmw_Osal_Func_Ptr_S
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
	add	r0, r0, #12288
	ldr	r2, [r0, #156]
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
	str	r0, [r5, #576]
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
	ldr	r7, .L1659+8
	ldr	r1, .L1659+12
	ldr	r3, [r7, #68]
	mov	r2, r0
	mov	r6, r0
	mov	r0, #19
	blx	r3
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
	strb	r1, [r5, #520]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1654:
	strb	r3, [r5, #520]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1653:
	ldr	r3, [r7, #68]
	mov	r2, r6
	ldr	r1, .L1659+16
	mov	r0, #1
	blx	r3
.L1657:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1655:
	mov	r3, #2
	strb	r3, [r5, #520]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1658:
	ldr	r3, .L1659+8
	mov	r0, #1
	ldr	r1, .L1659+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1660:
	.align	2
.L1659:
	.word	.LC38
	.word	.LC40
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L1673
	add	r4, r0, #11075584
	add	r4, r4, #40960
	mov	r5, r0
	ldr	r1, .L1673+4
	mov	r0, #19
	ldr	r3, [r6, #68]
	ldr	r2, [r4, #536]
	blx	r3
	ldr	r3, [r5, #252]
	ldr	r2, [r4, #536]
	mov	r1, #2240
	mla	r1, r1, r2, r3
	ldrb	r3, [r1, #19]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1671
	ldrb	r3, [r4, #532]	@ zero_extendqisi2
	sxtb	r4, r3
	cmn	r4, #1
	beq	.L1672
	cmp	r3, #0
	bne	.L1666
	ldr	r2, [r1, #28]
	movw	r3, #8500
	movt	r3, 5
	mul	r3, r3, r2
	add	r0, r5, r3
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
	str	r1, [r5, #244]
	str	r3, [r5, #240]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1672:
	ldr	r2, [r1, #28]
	movw	r0, #3992
	ldr	r3, [r5, #248]
	mla	r3, r0, r2, r3
	ldrb	r0, [r3, #25]	@ zero_extendqisi2
	cmp	r0, #0
	bne	.L1665
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1673+8
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1667:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1673+12
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1671:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1673+16
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1666:
	ldr	r3, [r6, #68]
	mov	r2, r4
	ldr	r1, .L1673+20
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1674:
	.align	2
.L1673:
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r6, .L1684
	mov	r2, r1
	mov	r5, r1
	mov	r4, r0
	ldr	r1, .L1684+4
	mov	r0, #19
	ldr	r3, [r6, #68]
	blx	r3
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
	movw	r3, #41492
	movt	r3, 169
	add	r3, r4, r3
	ldrsb	r5, [r3]
	cmn	r5, #1
	beq	.L1683
.L1680:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1683:
	ldr	r1, [r4, #248]
	movw	r3, #3992
	mla	r3, r3, r2, r1
	ldrb	r3, [r3, #25]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1680
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1684+8
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1681:
	ldr	r3, [r6, #68]
	mov	r2, r5
	ldr	r1, .L1684+12
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1682:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1684+16
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1685:
	.align	2
.L1684:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r0, [r3, #2184]
	ldr	r2, [r3, #2188]
	ldr	r4, [r3, #536]
	ldr	r6, [r3, #2192]
	cmp	r2, r4
	cmpeq	r0, r1
	ldr	r5, [r3, #548]
	ldrb	r4, [r3, #2176]	@ zero_extendqisi2
	ldrb	r0, [r3, #521]	@ zero_extendqisi2
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
	ldrb	r0, [r3, #2177]	@ zero_extendqisi2
	ldrb	r1, [r3, #522]	@ zero_extendqisi2
	cmp	r0, r1
	orrne	r2, r2, #1
.L1688:
	ldrb	r1, [r3, #2179]	@ zero_extendqisi2
	ldrb	r0, [r3, #528]	@ zero_extendqisi2
	cmp	r1, r0
	moveq	r1, #0
	beq	.L1689
	cmp	r0, #0
	cmpne	r1, #0
	moveq	r1, #1
	movne	r1, #0
.L1689:
	ldrb	r0, [r3, #2178]	@ zero_extendqisi2
	ldrb	r4, [r3, #523]	@ zero_extendqisi2
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
	ldr	r0, [r3, #2204]
	ldr	r4, [r3, #2200]
	ldr	r5, [r1, #3488]
	ldr	lr, [r1, #3484]
	cmp	r4, lr
	cmpeq	r0, r5
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
.L1692:
	ldrb	r1, [r3, #531]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L1693
	ldr	r1, [r3, #2220]
	ldrb	r0, [r3, #2180]	@ zero_extendqisi2
	ldrb	r6, [r3, #529]	@ zero_extendqisi2
	ldr	r5, [r3, #2160]
	ldrb	r4, [r3, #2181]	@ zero_extendqisi2
	cmp	r1, r5
	cmpeq	r0, r6
	ldrb	lr, [r3, #530]	@ zero_extendqisi2
	movne	r1, #1
	moveq	r1, #0
	cmp	r4, lr
	moveq	r3, r1
	orrne	r3, r1, #1
	orr	r2, r2, r3
.L1693:
	add	r3, ip, #40960
	ldr	ip, .L1702
	ldr	r1, .L1702+4
	mov	r0, #19
	ldr	r4, [r3, #576]
	ldr	r3, [ip, #68]
	cmp	r4, #0
	movne	r4, r2
	orreq	r4, r2, #1
	mov	r2, r4
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1691:
	cmp	r1, #1
	bne	.L1692
	add	r1, ip, #36864
	ldr	r0, [r3, #2212]
	ldr	r4, [r3, #2208]
	ldr	r5, [r1, #3524]
	ldr	lr, [r1, #3520]
	cmp	r4, lr
	cmpeq	r0, r5
	movne	r1, #1
	moveq	r1, #0
	orr	r2, r2, r1
	b	.L1692
.L1701:
	ldr	r0, [r3, #2196]
	ldr	r1, [r3, #564]
	cmp	r0, r1
	orrne	r2, r2, #1
	b	.L1690
.L1703:
	.align	2
.L1702:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldrb	r1, [r8, #531]	@ zero_extendqisi2
	mov	r4, r0
	strb	r3, [r5, #3564]
	cmp	r1, r3
	strb	r3, [r5, #3565]
	ldrb	r3, [r8, #521]	@ zero_extendqisi2
	movne	r7, #5
	moveq	r7, #3
	cmp	r3, #0
	ldr	r3, [r2, #2896]
	addne	r3, r3, #5
	addeq	r3, r3, #4
	mov	r6, r6, asl r3
	ldrb	r3, [r8, #520]	@ zero_extendqisi2
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
	strb	r0, [r5, #3564]
	cmp	r0, #0
	beq	.L1715
	ldr	r3, [r8, #580]
	cmn	r3, #2
	beq	.L1741
	movw	r10, #40428
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
	ldr	r3, [r8, #580]
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
	ldr	ip, .L1766+12
	mov	r2, r0
	ldr	r1, .L1766+16
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1715:
	ldrb	r3, [r8, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1758
	ldr	r1, .L1766+20
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #3565]
	cmp	r0, #0
	beq	.L1758
	ldr	r3, [r8, #584]
	cmn	r3, #2
	beq	.L1758
	movw	r9, #40560
	mov	r5, #0
	movt	r9, 169
	add	r9, r4, r9
	b	.L1738
.L1737:
	add	r5, r5, #1
	cmp	r5, r3
	bcs	.L1758
.L1738:
	ldr	r1, .L1766+24
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
	ldr	r3, [r8, #584]
	add	r3, r3, #2
	cmp	r3, r5
	bne	.L1737
.L1739:
	ldr	r3, .L1766+12
	mov	r0, #1
	ldr	r1, .L1766+28
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1763:
	ldr	r1, .L1766+32
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, r0
	str	r0, [r9, #264]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	bls	.L1733
	cmp	r3, #0
	bne	.L1733
.L1734:
	ldr	r2, [r8, #584]
	add	r3, r2, #2
	cmp	r3, r5
	bne	.L1737
	movw	r1, #26526
	movt	r1, 42
	add	r1, r2, r1
	add	r1, r4, r1, lsl #2
	ldr	r2, [r1, #4]
	cmp	r2, #3
	beq	.L1737
	b	.L1739
.L1760:
	ldr	r1, .L1766+36
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r10, #528]
	b	.L1719
.L1764:
	ldr	r1, .L1766+40
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #524]
	b	.L1734
.L1761:
	ldr	r1, .L1766+44
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r10, #784]
	b	.L1719
.L1765:
	ldr	r1, .L1766+48
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #784]
	b	.L1734
.L1712:
	ldr	ip, .L1766+12
	mov	r2, r0
	ldr	r1, .L1766+52
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1729:
	ldr	ip, .L1766+12
	mov	r2, r0
	ldr	r1, .L1766+56
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1762:
	cmp	r9, r3
	bne	.L1715
.L1710:
	movw	r2, #26490
	movt	r2, 42
	add	r2, r3, r2
	add	r2, r4, r2, lsl #2
	ldr	r3, [r2, #8]
	cmp	r3, #3
	beq	.L1715
	ldr	r3, .L1766+12
	mov	r0, #1
	ldr	r1, .L1766+60
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1733:
	ldr	ip, .L1766+12
	mov	r2, r0
	ldr	r1, .L1766+64
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
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
	.word	vfmw_Osal_Func_Ptr_S
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
	str	r0, [r6, #588]
	mov	r8, r4, asl r0
	ldr	r9, [r7, #748]
	cmp	r9, #0
	bne	.L1792
.L1769:
	cmp	r0, #7
	bhi	.L1772
	ldr	r3, [r6, #592]
	cmp	r3, #7
	bhi	.L1772
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
	cmp	r2, #0
	movweq	r10, #41684
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
	ldr	r2, [r6, #580]
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
	ldr	r0, [r6, #588]
	mov	r9, r4, asl r3
	str	r3, [r6, #592]
	b	.L1769
.L1781:
	ldrb	r3, [r6, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1782
	movw	r4, #42068
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
	ldr	r3, [r6, #584]
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
	ldr	r3, .L1798+48
	mov	r0, #1
	ldr	r1, .L1798+52
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1774
.L1797:
	ldr	r1, .L1798+56
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+60
	str	r0, [r4]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+56
	str	r0, [r4, #768]
	mov	r0, r5
	bl	mvc_se_v
	ldr	r1, .L1798+60
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
	.word	vfmw_Osal_Func_Ptr_S
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
.L1835:
	ldr	r1, .L1841
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r5, #2
	beq	.L1833
.L1814:
	cmp	r5, #6
	orreq	r6, r6, #1
	cmp	r6, #0
	bne	.L1817
	cmp	r5, #4
	beq	.L1834
.L1818:
	cmp	r5, #6
	bhi	.L1819
.L1815:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1819
	cmp	r5, #0
	beq	.L1821
.L1820:
	subs	r7, r7, #1
	beq	.L1822
.L1812:
	ldr	r1, .L1841+4
	mov	r0, r4
	bl	mvc_ue_v
	bic	r3, r0, #2
	sub	r6, r0, #3
	clz	r6, r6
	cmp	r3, #1
	mov	r5, r0
	mov	r6, r6, lsr #5
	beq	.L1835
	cmp	r5, #2
	bne	.L1814
.L1833:
	ldr	r1, .L1841+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r6, #0
	beq	.L1815
.L1817:
	ldr	r1, .L1841+12
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r5, #4
	bne	.L1818
.L1834:
	ldr	r1, .L1841+16
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1820
.L1819:
	ldr	r3, .L1841+20
	mov	r0, #1
	ldr	r1, .L1841+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1840:
	ldr	r1, .L1841
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r3, [r8, #4]
	cmp	r3, #2
	str	r0, [r8, #8]
	beq	.L1836
.L1806:
	cmp	r3, #3
	cmpne	r3, #6
	beq	.L1837
.L1807:
	cmp	r3, #4
	beq	.L1838
.L1808:
	cmp	r3, #6
	bhi	.L1819
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1819
	cmp	r3, #0
	add	r9, r9, #1
	beq	.L1821
	cmp	r9, #100
	beq	.L1839
.L1803:
	ldr	r1, .L1841+4
	mov	r0, r4
	ldrb	r5, [r4, #3]	@ zero_extendqisi2
	bl	mvc_ue_v
	mov	r6, r9, asl #2
	mov	r7, r9, asl #4
	add	r1, r6, r7
	movw	r2, #2004
	movw	r8, #43184
	mla	r2, r2, r5, r1
	movt	r8, 169
	add	r2, r4, r2
	add	r8, r2, r8
	mov	r3, r0
	bic	r0, r0, #2
	cmp	r0, #1
	str	r3, [r8, #4]
	beq	.L1840
	cmp	r3, #2
	bne	.L1806
.L1836:
	ldr	r1, .L1841+8
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r6, r7
	movw	r2, #2004
	movw	r1, #43192
	mla	r3, r2, r5, r3
	movw	r2, #43184
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
.L1837:
	ldr	r1, .L1841+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r6, r7
	movw	r2, #2004
	movw	r1, #43192
	mla	r3, r2, r5, r3
	movw	r2, #43184
	movt	r1, 169
	movt	r2, 169
	add	r3, r4, r3
	add	r1, r3, r1
	add	r2, r3, r2
	str	r0, [r1, #8]
	ldr	r3, [r2, #4]
	cmp	r3, #4
	bne	.L1808
.L1838:
	ldr	r1, .L1841+16
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, r7
	movw	r7, #2004
	movw	r2, #43200
	mla	r5, r7, r5, r6
	movw	r3, #43184
	movt	r2, 169
	movt	r3, 169
	add	r5, r4, r5
	add	r2, r5, r2
	add	r3, r5, r3
	str	r0, [r2, #4]
	ldr	r3, [r3, #4]
	b	.L1808
.L1821:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1839:
	ldr	r3, .L1841+20
	mov	r0, #1
	ldr	r1, .L1841+28
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1822:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1842:
	.align	2
.L1841:
	.word	.LC85
	.word	.LC84
	.word	.LC86
	.word	.LC87
	.word	.LC88
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	add	r3, r0, #11075584
	add	r3, r3, #40960
	mov	r4, r0
	ldrb	r7, [r0, #3]	@ zero_extendqisi2
	ldrb	r8, [r3, #525]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L1844
	clz	r2, r7
	mov	r2, r2, lsr #5
	strb	r2, [r0, #3]
	mov	r6, r2
.L1853:
	ldrb	r3, [r3, #523]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1864
	ldr	r1, .L1868
	mov	r0, r4
	bl	mvc_u_1
	movw	r2, #2004
	mla	r6, r2, r6, r4
	movw	r5, #43184
	ldr	r9, .L1868+4
	movt	r5, 169
	ldr	r1, .L1868+8
	add	r5, r6, r5
	ldr	ip, [r9, #68]
	mov	r3, r0
	uxtb	r2, r0
	strb	r3, [r5, #3]
	mov	r0, #16
	blx	ip
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1847
.L1851:
	mov	r8, #0
.L1861:
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1844:
	movw	r2, #2004
	movw	r1, #43184
	mla	r2, r2, r7, r0
	movt	r1, 169
	movw	r5, #43184
	mov	r6, r7
	movt	r5, 169
	add	r5, r2, r5
	ldrb	r2, [r2, r1]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L1853
	ldrb	r3, [r3, #523]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1865
	ldr	r1, .L1868
	ldrb	r6, [r5, #3]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r6, r0
	bne	.L1866
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1851
	mov	r1, r8
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	MVC_DecMMCO
.L1847:
	mov	r1, #1
	mov	r0, r4
	bl	MVC_DecMMCO
	cmn	r0, #1
	mov	r8, r0
	movne	r3, #1
	movne	r8, #0
	strneb	r3, [r5]
	bne	.L1861
	mov	r3, #0
	ldr	r1, .L1868+12
	strb	r3, [r5]
	mov	r0, #1
	strb	r7, [r4, #3]
	ldr	r3, [r9, #68]
	blx	r3
	b	.L1861
.L1865:
	ldr	r1, .L1868+16
	ldrb	r6, [r5, #1]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r6, r0
	bne	.L1867
	mov	r0, r4
	ldr	r1, .L1868+20
	ldrb	r4, [r5, #2]	@ zero_extendqisi2
	bl	mvc_u_1
	cmp	r4, r0
	beq	.L1851
	ldr	r3, .L1868+4
	mov	r0, #1
	ldr	r1, .L1868+24
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1861
.L1864:
	ldr	r1, .L1868+16
	mov	r0, r4
	bl	mvc_u_1
	movw	r2, #2004
	mla	r2, r2, r6, r4
	movw	r5, #43184
	movt	r5, 169
	ldr	r1, .L1868+20
	mov	r8, #0
	add	r5, r2, r5
	strb	r0, [r5, #1]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, .L1868+4
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	ldr	r1, .L1868+28
	ldr	r3, [r3, #68]
	strb	r0, [r5, #2]
	mov	r0, #16
	blx	r3
	mov	r0, r8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1866:
	ldr	r5, .L1868+4
	mov	r0, #1
	ldr	r1, .L1868+32
	mvn	r8, #0
	ldr	r3, [r5, #68]
	blx	r3
	ldr	lr, [r4, #68]
	ldr	r3, [r4, #100]
	mov	r0, #1
	ldr	r2, [r4, #88]
	ldr	ip, [r5, #68]
	ldr	r1, .L1868+36
	str	lr, [sp]
	blx	ip
	b	.L1861
.L1867:
	ldr	r3, .L1868+4
	mov	r0, #1
	ldr	r1, .L1868+40
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1861
.L1869:
	.align	2
.L1868:
	.word	.LC93
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, .L1935
	mov	r4, r0
	bl	mvc_ue_v
	ldr	r3, [r4, #40]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r6, r0
	bhi	.L1871
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1871
	mov	r1, r0
	mov	r0, r4
	bl	MVC_PPSSPSCheckTmpId
	cmp	r0, #0
	bne	.L1926
	add	r7, r4, #11075584
	mov	r0, r4
	add	r5, r7, #40960
	str	r6, [r5, #536]
	bl	MVC_PPSSPSCheck
	subs	r8, r0, #0
	bne	.L1927
	ldr	r6, [r4, #240]
	mov	r0, r4
	ldr	r2, .L1935+4
	ldr	r9, [r4, #244]
	ldr	r1, [r6, #2896]
	add	r1, r1, #4
	bl	mvc_u_v
	str	r0, [r5, #548]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1924
	adds	r0, r0, #0
	ldrb	r3, [r5, #523]	@ zero_extendqisi2
	movne	r0, #1
	cmp	r3, #5
	movne	r0, #0
	cmp	r0, #0
	bne	.L1928
.L1877:
	mov	r3, #0
	strb	r3, [r5, #521]
	strb	r3, [r5, #522]
	ldrb	r3, [r6, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1929
	ldr	r2, [r6, #3948]
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	mla	r3, r1, r3, r3
.L1900:
	ldrb	r2, [r6, #21]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1883
	strb	r2, [r5, #524]
.L1885:
	ldr	r2, [r5, #576]
	cmp	r2, r3
	bcs	.L1930
.L1886:
	ldrb	r3, [r5, #523]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L1931
.L1888:
	add	r10, r7, #36864
	mov	r3, #0
	str	r3, [r10, #3484]
	str	r3, [r10, #3488]
	ldr	r3, [r6, #2900]
	cmp	r3, #0
	beq	.L1932
.L1892:
	mov	r3, #0
	str	r3, [r10, #3520]
	str	r3, [r10, #3524]
	ldr	r3, [r6, #2900]
	cmp	r3, #1
	beq	.L1933
.L1895:
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1924
	ldrb	r3, [r9, #17]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1898
	ldr	r1, .L1935+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L1899
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1898
.L1899:
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+16
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, .L1935+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L1924
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L1924:
	mvn	r8, #0
.L1910:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1929:
	ldr	r1, .L1935+24
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #521]
	cmp	r0, #0
	bne	.L1934
	ldr	r2, [r6, #3948]
	ldrb	ip, [r6, #20]	@ zero_extendqisi2
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	cmp	ip, #0
	mla	r3, r1, r3, r3
	bne	.L1902
	mov	r3, r3, asl #1
	b	.L1900
.L1883:
	mov	r2, #1
	strb	r2, [r5, #524]
.L1903:
	ldr	r2, [r5, #576]
	cmp	r2, r3, lsr #1
	bcc	.L1886
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+28
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1910
.L1934:
	ldr	r1, .L1935+32
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #522]
	ldr	r2, [r6, #3948]
	ldrb	r0, [r6, #20]	@ zero_extendqisi2
	ldr	r1, [r6, #3952]
	add	r3, r2, #1
	cmp	r0, #0
	ldrb	r0, [r5, #521]	@ zero_extendqisi2
	mla	r3, r1, r3, r3
	moveq	r2, #2
	beq	.L1881
.L1902:
	mov	r2, #1
.L1881:
	cmp	r0, #0
	mul	r3, r2, r3
	beq	.L1900
	cmp	r0, #1
	mov	r2, #0
	strb	r2, [r5, #524]
	bne	.L1885
	b	.L1903
.L1898:
	mov	r0, r4
	add	r7, r7, #40960
	bl	MVC_IsNewPic
	uxtb	r0, r0
	strb	r0, [r7, #525]
	cmp	r0, #0
	bne	.L1910
	ldr	r2, [r7, #576]
	ldr	r3, [r7, #2216]
	cmp	r2, r3
	bhi	.L1910
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+36
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1910
.L1932:
	ldr	r1, [r6, #2904]
	mov	r0, r4
	ldr	r2, .L1935+40
	add	r1, r1, #4
	bl	mvc_u_v
	str	r0, [r10, #3484]
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1892
	ldrb	r3, [r5, #521]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1892
	ldr	r1, .L1935+44
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #3488]
	b	.L1892
.L1933:
	ldrb	r3, [r6, #18]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1895
	ldr	r1, .L1935+48
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #3520]
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1895
	ldrb	r3, [r5, #521]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1895
	ldr	r1, .L1935+52
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r10, #3524]
	b	.L1895
.L1871:
	ldr	r3, .L1935+12
	mov	r2, r6
	ldr	r1, .L1935+56
	mvn	r8, #0
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1931:
	ldr	r1, .L1935+60
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #65536
	str	r0, [r5, #564]
	bcs	.L1889
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1888
.L1889:
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+64
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1888
.L1926:
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+68
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1910
.L1927:
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+72
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1910
.L1928:
	ldr	r3, [r5, #2160]
	cmp	r3, #0
	bgt	.L1877
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+76
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1877
.L1930:
	ldr	r3, .L1935+12
	mov	r0, #1
	ldr	r1, .L1935+80
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1910
.L1936:
	.align	2
.L1935:
	.word	.LC100
	.word	.LC104
	.word	.LC116
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r2, [r4, #536]
	cmp	r3, r2
	bcc	.L1938
	ldrb	r7, [r0, #10]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L1938
	bl	MVC_PPSSPSCheck
	cmp	r0, #0
	bne	.L2020
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L2021
.L1942:
	ldr	r2, [r6, #36]
	cmp	r3, #1
	str	r2, [r4, #580]
	ldr	r2, [r6, #40]
	str	r2, [r4, #584]
	bls	.L2022
	cmp	r3, #2
	beq	.L1969
.L1953:
	mov	r3, #32
	str	r3, [r4, #584]
.L1954:
	mov	r0, r5
	bl	mvc_ref_pic_list_reordering
	subs	r8, r0, #0
	bne	.L2023
	ldrb	r3, [r6, #2]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1956
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	cmp	r3, #0
	moveq	r7, #1
	moveq	r2, r7
	beq	.L1957
.L1956:
	ldr	r3, [r6, #44]
	cmp	r3, #1
	beq	.L2024
.L1973:
	mov	r2, #0
.L1957:
	ldr	r9, .L2030
	mov	r0, #19
	strb	r7, [r4, #527]
	ldr	r1, .L2030+4
	ldr	r3, [r9, #68]
	blx	r3
	ldrb	r2, [r4, #527]	@ zero_extendqisi2
	mov	r3, #0
	str	r3, [r4, #588]
	cmp	r2, r3
	str	r3, [r4, #592]
	bne	.L2025
.L1958:
	ldrb	r3, [r4, #528]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2026
.L1959:
	mov	r3, #0
	str	r3, [r4, #2132]
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1960
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L2027
.L1960:
	ldr	r1, .L2030+8
	mov	r0, r5
	bl	mvc_se_v
	mov	r3, #0
	str	r3, [r4, #2140]
	str	r3, [r4, #2144]
	str	r3, [r4, #2148]
	str	r0, [r4, #2136]
	ldrb	r3, [r6, #3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2028
.L1989:
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1969:
	mov	r3, #32
	str	r3, [r4, #580]
	b	.L1953
.L2028:
	ldr	r1, .L2030+12
	mov	r0, r5
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r4, #2140]
	bhi	.L1963
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1963
	cmp	r0, #1
	beq	.L1989
	ldr	r1, .L2030+16
	mov	r0, r5
	bl	mvc_se_v
	add	r3, r0, #6
	str	r0, [r4, #2144]
	cmp	r3, #12
	bhi	.L1966
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1966
	ldr	r1, .L2030+20
	mov	r0, r5
	bl	mvc_se_v
	add	r3, r0, #6
	str	r0, [r4, #2148]
	cmp	r3, #12
	bhi	.L1968
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1989
.L1968:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L2030+24
	mvn	r8, #0
	blx	r3
	b	.L1989
.L2025:
	mov	r0, r5
	bl	mvc_pred_weight_table
	cmp	r0, #0
	beq	.L1958
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L2030+28
	mvn	r8, #0
	blx	r3
	b	.L1989
.L2026:
	mov	r0, r5
	bl	mvc_dec_ref_pic_marking
	cmp	r0, #0
	beq	.L1959
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L2030+32
	mvn	r8, #0
	blx	r3
	b	.L1989
.L2024:
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r7, r3
	moveq	r2, r7
	bne	.L1973
	b	.L1957
.L2021:
	ldr	r1, .L2030+36
	mov	r0, r5
	bl	mvc_u_1
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	strb	r0, [r4, #526]
	b	.L1942
.L2022:
	ldr	r1, .L2030+40
	mov	r0, r5
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L1944
	ldr	r3, [r4, #580]
.L1945:
	ldrb	r2, [r4, #521]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L1946
	cmp	r3, #15
	bhi	.L1947
	ldr	r3, [r4, #584]
	cmp	r3, #15
	bhi	.L2029
.L1948:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L1947
.L1949:
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L1969
	cmp	r3, #1
	bne	.L1953
	b	.L1954
.L1938:
	ldr	r3, .L2030
	mvn	r8, #0
	ldr	r1, .L2030+44
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r8
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2027:
	ldr	r1, .L2030+48
	mov	r0, r5
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r4, #2132]
	bhi	.L1961
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1960
.L1961:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L2030+52
	mvn	r8, #0
	blx	r3
	b	.L1989
.L1946:
	cmp	r3, #31
	bhi	.L1950
	ldr	r3, [r4, #584]
	cmp	r3, #31
	bls	.L1951
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1951
.L1950:
	ldr	r3, .L2030
	mov	r0, #1
	ldr	r1, .L2030+56
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1989
.L1951:
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1949
	b	.L1950
.L2029:
	ldrb	r3, [r4, #520]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1948
.L1947:
	ldr	r3, .L2030
	mov	r0, #1
	ldr	r1, .L2030+60
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1989
.L1944:
	ldr	r1, .L2030+64
	mov	r0, r5
	bl	mvc_ue_v
	ldrb	r2, [r4, #520]	@ zero_extendqisi2
	cmp	r2, #1
	mov	r3, r0
	str	r0, [r4, #580]
	bne	.L1945
	ldr	r1, .L2030+68
	mov	r0, r5
	bl	mvc_ue_v
	ldr	r3, [r4, #580]
	str	r0, [r4, #584]
	b	.L1945
.L1966:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L2030+72
	mvn	r8, #0
	blx	r3
	b	.L1989
.L1963:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L2030+76
	mvn	r8, #0
	blx	r3
	b	.L1989
.L2020:
	ldr	r3, .L2030
	mov	r0, #1
	ldr	r1, .L2030+80
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1989
.L2023:
	ldr	r3, .L2030
	mov	r0, #1
	ldr	r1, .L2030+84
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1989
.L2031:
	.align	2
.L2030:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldrb	r1, [r3, #521]	@ zero_extendqisi2
	ldr	lr, [r3, #536]
	cmp	r1, #0
	ldr	r5, [r3, #576]
	str	lr, [r3, #2188]
	ldr	r4, [ip, #744]
	strb	r1, [r3, #2176]
	ldrneb	r1, [r3, #522]	@ zero_extendqisi2
	ldr	lr, [r3, #548]
	str	r5, [r3, #2216]
	strneb	r1, [r3, #2177]
	ldrb	r1, [r3, #523]	@ zero_extendqisi2
	str	lr, [r3, #2192]
	cmp	r1, #5
	ldrb	lr, [r3, #528]	@ zero_extendqisi2
	strb	r1, [r3, #2178]
	ldreq	r1, [r3, #564]
	str	r4, [r3, #2184]
	strb	lr, [r3, #2179]
	streq	r1, [r3, #2196]
	ldr	r1, [ip, #2900]
	cmp	r1, #0
	bne	.L2035
	add	r1, r2, #36864
	ldr	lr, [r1, #3484]
	str	lr, [r3, #2200]
	ldr	r1, [r1, #3488]
	str	r1, [r3, #2204]
	ldr	r1, [ip, #2900]
.L2035:
	cmp	r1, #1
	addeq	r2, r2, #36864
	ldreq	r1, [r2, #3520]
	streq	r1, [r3, #2208]
	ldreq	r2, [r2, #3524]
	streq	r2, [r3, #2212]
	ldrb	r2, [r3, #531]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L2037
	ldr	ip, [r3, #2160]
	ldrb	r1, [r3, #529]	@ zero_extendqisi2
	ldrb	r2, [r3, #530]	@ zero_extendqisi2
	str	ip, [r3, #2220]
	strb	r1, [r3, #2180]
	strb	r2, [r3, #2181]
.L2037:
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
	ldrb	r2, [r3, #520]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L2043
	bcc	.L2044
	cmp	r2, #2
	ldmnefd	sp, {fp, sp, pc}
	ldrb	r3, [r3, #523]	@ zero_extendqisi2
	add	r0, r0, #11141120
	cmp	r3, #5
	beq	.L2053
.L2046:
	add	r0, r0, #8192
	mov	r3, #0
	strb	r3, [r0, #1605]
	ldmfd	sp, {fp, sp, pc}
.L2044:
	add	r0, r0, #11141120
	add	r3, r0, #8192
	ldrb	r2, [r3, #1604]	@ zero_extendqisi2
	cmp	r2, #2
	movne	r2, #1
	strneb	r2, [r3, #1604]
	b	.L2046
.L2043:
	add	r0, r0, #11141120
	mov	r2, #2
	add	r0, r0, #8192
	mov	r3, #0
	strb	r2, [r0, #1604]
	strb	r3, [r0, #1605]
	ldmfd	sp, {fp, sp, pc}
.L2053:
	add	r0, r0, #8192
	mov	r3, #1
	strb	r3, [r0, #1605]
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
	ldrb	r3, [r3, #1240]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2055
	ldr	r2, [r0, #232]
	ldr	r3, [r2, #64]
	add	r3, r3, #7
	bic	r3, r3, #7
	str	r3, [r2, #64]
	ldr	r2, [r0, #232]
	ldr	r3, [r2, #72]
	sub	r3, r3, #1
	str	r3, [r2, #72]
.L2055:
	ldr	ip, [r4, #232]
	ldrb	r6, [ip]	@ zero_extendqisi2
	cmp	r6, #0
	beq	.L2066
	mov	r1, #0
	mov	r0, ip
	mov	r5, r1
.L2057:
	add	r1, r1, #1
	ldr	r3, [r0, #12]
	cmp	r1, r6
	add	r0, r0, #28
	add	r5, r5, r3
	bne	.L2057
.L2056:
	mov	r8, r6, asl #5
	ldr	r3, [ip, #72]
	sub	r8, r8, r6, asl #2
	ldr	r2, [ip, #64]
	add	ip, ip, r8
	ldr	r7, .L2077
	str	r3, [sp, #4]
	mov	r0, #7
	ldr	lr, [ip, #12]
	mov	r3, r5
	ldr	ip, [r7, #68]
	ldr	r1, .L2077+4
	str	lr, [sp]
	blx	ip
	ldr	r3, [r4, #232]
	mov	r2, r6
	mov	r0, #7
	ldr	r1, .L2077+8
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	ip, [r3, #64]
	mov	lr, r6, asl #5
	and	ip, ip, r0
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	str	ip, [r3, #24]
	ldr	r3, [r4, #232]
	ldrb	ip, [r3]	@ zero_extendqisi2
	ldr	r6, [r3, #64]
	mov	lr, ip, asl #5
	sub	lr, lr, ip, asl #2
	add	r3, r3, lr
	ldr	ip, [r3, #16]
	add	ip, ip, r6, lsr #3
	rsb	ip, r5, ip
	str	ip, [r3, #28]
	ldr	r3, [r4, #232]
	ldrb	r6, [r3]	@ zero_extendqisi2
	ldr	ip, [r3, #64]
	mov	lr, r6, asl #5
	sub	lr, lr, r6, asl #2
	add	r3, r3, lr
	ldr	lr, [r3, #12]
	add	r5, r5, lr
	rsb	r5, ip, r5, lsl #3
	str	r5, [r3, #20]
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
	ldr	ip, [r4, #232]
	ldr	r2, [ip, #68]
	sub	r3, r2, #1
	cmp	r3, #1
	bhi	.L2076
	ldrb	r5, [ip]	@ zero_extendqisi2
	add	r1, r5, #1
	cmp	r2, r1
	movhi	r2, #1
	movls	r2, #0
	cmp	r1, #1
	movhi	r2, #0
	cmp	r2, #0
	beq	.L2059
	add	r5, r5, #2
	mov	r6, #0
.L2060:
	sub	r3, r5, #1
	ldr	r1, .L2077+12
	mov	r0, #7
	mov	lr, r3, asl #5
	mov	r2, r3
	sub	r3, lr, r3, asl #2
	add	ip, ip, r3
	str	r6, [ip, #24]
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
	ldr	r8, [ip, #24]
	and	lr, lr, #3
	add	lr, r8, lr, lsl #3
	str	lr, [ip, #24]
	ldr	ip, [r4, #232]
	add	ip, ip, r3
	ldr	lr, [ip, #28]
	bic	lr, lr, #3
	str	lr, [ip, #28]
	ldr	ip, [r4, #232]
	ldr	r8, [r7, #68]
	add	ip, ip, r3
	ldr	r3, [ip, #24]
	ldr	ip, [ip, #20]
	str	ip, [sp]
	blx	r8
	ldr	ip, [r4, #232]
	ldr	r3, [ip, #68]
	cmp	r5, #1
	cmpls	r5, r3
	add	r5, r5, #1
	movcc	r2, #1
	movcs	r2, #0
	cmp	r2, #0
	bne	.L2060
	sub	r3, r3, #1
.L2059:
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
	bcs	.L2054
	sxth	r3, r1
	add	r4, r4, #11141120
	add	r4, r4, #8192
	mov	r2, r3, asl #5
	sub	r3, r2, r3, asl #2
	add	r3, ip, r3
.L2064:
	ldr	r2, [r3, #28]
	add	r1, r1, #1
	ldr	r0, [r4, #2184]
	add	r3, r3, #28
	cmp	r2, r0
	strcc	r2, [r4, #2184]
	ldr	r2, [ip, #68]
	cmp	r2, r1
	bhi	.L2064
.L2054:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2076:
	ldr	r3, [r7, #68]
	mov	r0, #0
	ldr	r1, .L2077+16
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, lr}
	bx	r3
.L2066:
	mov	r5, r6
	b	.L2056
.L2078:
	.align	2
.L2077:
	.word	vfmw_Osal_Func_Ptr_S
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
	mov	r9, r1
	ble	.L2079
	mov	r3, r7
	mov	r1, #8
	mov	r6, r9
	ldr	r8, .L2090
	mov	r5, r1
	mov	r4, #0
	mov	r10, #255
	mov	r7, r2
	mov	r9, r3
	b	.L2084
.L2083:
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
	beq	.L2079
.L2084:
	cmp	r9, #16
	ldreq	r3, .L2090+4
	ldrneb	ip, [r4, r8]	@ zero_extendqisi2
	ldreqb	ip, [r4, r3]	@ zero_extendqisi2
	cmp	r1, #0
	add	r4, r4, #1
	beq	.L2083
	ldr	r1, .L2090+8
	ldr	r0, [fp, #-52]
	str	ip, [fp, #-48]
	bl	mvc_se_v
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
	b	.L2083
.L2079:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2091:
	.align	2
.L2090:
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
	ldr	r1, .L2150
	mov	r4, r0
	bl	mvc_u_1
	strb	r0, [r5]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2141
	ldr	r1, .L2150+4
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #1]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2141
	ldr	r1, .L2150+8
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	str	r0, [r5, #32]
	bne	.L2096
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2097
.L2096:
	ldr	r3, .L2150+12
	mov	r0, #1
	ldr	r1, .L2150+16
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, .L2150+20
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2141
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #102
	blx	ip
	mvn	r0, #0
.L2094:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2104:
	ldr	r1, .L2150+24
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2150+28
	str	r0, [r5, #60]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #18]
	cmp	r0, #1
	beq	.L2105
.L2123:
	ldr	r1, .L2150+32
	mov	r0, r4
	bl	mvc_se_v
	add	r3, r0, #12
	str	r0, [r5, #56]
	cmp	r3, #24
	bhi	.L2106
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2124
.L2106:
	ldr	r3, .L2150+12
	mov	r0, #1
	ldr	r1, .L2150+36
	ldr	r3, [r3, #68]
	blx	r3
.L2141:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2097:
	ldr	r1, .L2150+40
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #31
	str	r0, [r5, #36]
	bhi	.L2098
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2098
	ldr	r1, .L2150+44
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #31
	str	r0, [r5, #40]
	bhi	.L2100
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2100
	ldr	r1, .L2150+48
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #2]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2141
	ldr	r2, .L2150+52
	mov	r1, #2
	mov	r0, r4
	bl	mvc_u_v
	cmp	r0, #2
	str	r0, [r5, #44]
	bhi	.L2142
	ldr	r1, .L2150+56
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2150+60
	str	r0, [r5, #48]
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2150+64
	mov	r0, r4
	bl	mvc_se_v
	ldr	r1, .L2150+68
	str	r0, [r5, #52]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2150+72
	strb	r0, [r5, #3]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2150+76
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #17]
	cmp	r0, #0
	bne	.L2143
.L2103:
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
	bcs	.L2104
.L2124:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2143:
	ldr	r3, .L2150+12
	mov	r0, #1
	ldr	r1, .L2150+80
	ldr	r3, [r3, #68]
	blx	r3
	b	.L2103
.L2098:
	ldr	r3, .L2150+12
	mov	r0, #1
	ldr	r1, .L2150+84
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2094
.L2100:
	ldr	r3, .L2150+12
	mov	r0, #1
	ldr	r1, .L2150+88
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2094
.L2105:
	sub	r8, fp, #72
	add	r10, r5, #4
	add	r7, r5, #2016
	add	r9, r5, #64
	b	.L2114
.L2147:
	ldr	lr, [r5, #28]
	ldr	r3, [r4, #248]
	mla	r3, ip, lr, r3
	ldrb	ip, [r3, #27]	@ zero_extendqisi2
	cmp	ip, #1
	beq	.L2144
	ldr	r3, .L2150+92
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2150+12
	ldr	r3, [r3, #52]
	blx	r3
.L2109:
	add	r6, r6, #1
	add	r7, r7, #16
	cmp	r6, #6
	add	r9, r9, #64
	beq	.L2145
.L2114:
	ldr	r1, .L2150+96
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
	beq	.L2146
	cmp	r6, #0
	cmpne	r6, #3
	mov	r1, r1, lsr #5
	beq	.L2147
	ldr	r3, .L2150+12
	mov	r2, #16
	sub	r1, r7, #16
	mov	r0, r7
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2109
.L2145:
	add	r8, r5, #2112
	add	r10, r5, #448
	sub	r7, fp, #76
	add	r9, r5, #11
	mov	r6, #0
.L2122:
	ldr	r2, [r5, #60]
	mov	r3, #0
	cmp	r2, #1
	strne	r3, [r7]
	strneb	r3, [r9]
	beq	.L2148
.L2117:
	ldr	r1, [r4, #248]
	movw	ip, #3992
	ldr	r3, [r5, #28]
	mov	r0, r8
	mov	r2, #64
	mla	r3, ip, r3, r1
	ldrb	r1, [r3, #27]	@ zero_extendqisi2
	cmp	r1, #1
	beq	.L2149
	ldr	r1, .L2150+100
	cmp	r6, #0
	ldr	r3, .L2150+12
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2118:
	add	r6, r6, #1
	add	r8, r8, #64
	cmp	r6, #2
	add	r10, r10, #256
	add	r7, r7, #4
	add	r9, r9, #1
	bne	.L2122
	b	.L2123
.L2146:
	mov	r3, r2
	str	r8, [sp]
	mov	r2, r7
	mov	r1, r9
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r8]
	cmp	r3, #1
	bne	.L2109
	ldr	r1, .L2150+92
	mov	r2, #16
	ldr	r3, .L2150+12
	cmp	r6, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r7
	blx	r3
	b	.L2109
.L2144:
	ldr	r2, .L2150+12
	add	r1, r6, #167
	mov	r0, r7
	add	r1, r3, r1, lsl #4
	ldr	ip, [r2, #52]
	mov	r2, #16
	blx	ip
	b	.L2109
.L2142:
	ldr	r3, .L2150+12
	mov	r0, #1
	ldr	r1, .L2150+104
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2094
.L2148:
	ldr	r1, .L2150+108
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
	bne	.L2117
	str	r7, [sp]
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2118
	ldr	r1, .L2150+100
	mov	r2, #64
	ldr	r3, .L2150+12
	cmp	r6, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r8
	blx	r3
	b	.L2118
.L2149:
	add	r1, r3, r6, lsl #6
	ldr	r3, .L2150+12
	add	r1, r1, #2768
	ldr	ip, [r3, #52]
	blx	ip
	b	.L2118
.L2151:
	.align	2
.L2150:
	.word	.LC143
	.word	.LC144
	.word	.LC145
	.word	vfmw_Osal_Func_Ptr_S
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
	ldreq	ip, .L2181
	movweq	r3, #10246
	beq	.L2179
	cmp	r1, #0
	beq	.L2180
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
	beq	.L2156
	mov	ip, ip, asl #1
	adds	r2, ip, #6
	str	r2, [fp, #-56]
	beq	.L2156
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
.L2161:
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
	beq	.L2178
	ldr	r2, [r8]
	ldr	r1, [r7]
	cmp	r2, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r0, #5
	bhi	.L2158
	cmp	r2, #0
	add	r4, r10, #64
	bne	.L2157
	add	r10, r10, #128
	mov	r1, r6
	mov	r2, r4
.L2159:
	ldr	lr, [r2], #4
	ldr	ip, [r1], #4
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2159
.L2157:
	ldr	r2, [fp, #-56]
	add	r0, r0, #1
	add	r8, r8, #4
	add	r7, r7, #4
	cmp	r0, r2
	add	r5, r5, #256
	add	r9, r9, #256
	mov	r10, r4
	add	r6, r6, #64
	bne	.L2161
	ldr	r6, [fp, #-60]
	ldr	r1, [fp, #-64]
.L2156:
	ldr	r0, [r6, #56]
	ldr	r2, [r1, #56]
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L2154:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2158:
	cmp	r2, #0
	bne	.L2178
	sub	r2, r5, #1088
	sub	r4, r5, #836
	sub	r2, r2, #4
	mov	r1, r9
.L2160:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r4
	bne	.L2160
.L2178:
	add	r4, r10, #64
	b	.L2157
.L2180:
	ldr	ip, .L2181
	mov	r0, r1
	movw	r3, #10247
.L2179:
	ldr	r2, .L2181+4
	ldr	r1, .L2181+8
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L2154
.L2182:
	.align	2
.L2181:
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r1, .L2198
	mov	r4, r0
	bl	mvc_ue_v
	ldr	r3, [r4, #40]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	r5, r0
	bls	.L2184
	ldr	r3, .L2198+4
	mov	r2, r0
	ldr	r1, .L2198+8
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r2, [r4, #40]
	sub	r3, r2, #1
	cmp	r5, r3
	bls	.L2191
	ldr	r3, .L2198+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2191
	str	r2, [fp, #-40]
	mov	r3, #8
	str	r5, [fp, #-44]
	sub	r2, fp, #44
	ldr	r0, [r4, #120]
	mov	r1, #110
	blx	ip
	mvn	r0, #0
.L2185:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2184:
	ldr	r1, .L2198+16
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r3, [r4, #36]
	sub	r3, r3, #1
	cmp	r0, r3
	mov	ip, r0
	bhi	.L2195
	mov	r8, #2240
	ldr	r3, [r4, #252]
	mul	r6, r8, r5
	add	r3, r3, r6
	ldrb	r7, [r3, #19]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L2187
	ldr	r7, .L2198+20
	mov	r0, r4
	mov	r1, r7
	str	r5, [r7, #24]
	str	ip, [r7, #28]
	bl	MVC_ProcessPPS
	subs	r5, r0, #0
	bne	.L2196
	ldr	r1, [r4, #252]
	mov	r0, r7
	add	r1, r1, r6
	bl	MVC_PPSEqual
	cmp	r0, #0
	beq	.L2185
	ldr	ip, .L2198+4
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
	b	.L2185
.L2187:
	str	r5, [r3, #24]
	mov	r0, r4
	ldr	r3, [r4, #252]
	add	r3, r3, r6
	str	ip, [r3, #28]
	ldr	r1, [r4, #252]
	add	r1, r1, r6
	bl	MVC_ProcessPPS
	cmp	r0, #0
	bne	.L2197
	ldr	r3, [r4, #252]
	mov	r2, #1
	add	r3, r3, r6
	strb	r2, [r3, #20]
	ldr	r3, [r4, #252]
	add	r6, r3, r6
	strb	r2, [r6, #19]
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2191:
	mvn	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2195:
	ldr	r3, .L2198+4
	mov	r0, #1
	ldr	r1, .L2198+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2185
.L2196:
	ldr	r3, .L2198+4
	movw	r2, #10010
	ldr	r1, .L2198+28
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2185
.L2197:
	ldr	r3, .L2198+4
	movw	r2, #10030
	ldr	r1, .L2198+28
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r4, #252]
	mov	r2, #1
	mvn	r0, #0
	add	r3, r3, r6
	strb	r2, [r3, #20]
	ldr	r3, [r4, #252]
	add	r6, r3, r6
	strb	r7, [r6, #19]
	b	.L2185
.L2199:
	.align	2
.L2198:
	.word	.LC167
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC168
	.word	g_event_report
	.word	.LC169
	.word	.LANCHOR2
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
	ldr	r1, .L2250
	mov	r4, r0
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5]
	cmp	r0, #0
	streqb	r0, [r5, #15]
	bne	.L2244
.L2202:
	ldr	r1, .L2250+4
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #1]
	cmp	r0, #0
	bne	.L2245
.L2203:
	ldr	r1, .L2250+8
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #3]
	cmp	r0, #0
	moveq	r3, #5
	streq	r3, [r5, #28]
	bne	.L2246
.L2205:
	ldr	r1, .L2250+12
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #6]
	cmp	r0, #0
	bne	.L2247
.L2206:
	ldr	r1, .L2250+16
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #8]
	cmp	r0, #0
	bne	.L2248
.L2208:
	ldr	r1, .L2250+20
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #7]
	cmp	r0, #0
	beq	.L2210
	ldr	r1, .L2250+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r2, .L2250+28
	mov	r1, #4
	str	r0, [r5, #96]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+32
	mov	r1, #4
	strb	r0, [r5, #92]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [r5, #96]
	cmp	r3, #31
	strb	r0, [r5, #93]
	bhi	.L2216
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	addeq	r7, r5, #96
	addeq	r8, r5, #59
	bne	.L2216
.L2214:
	ldr	r1, .L2250+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #96]
	cmp	r3, r6
	bcs	.L2214
	ldr	r2, .L2250+48
	mov	r1, #5
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+52
	mov	r1, #5
	str	r0, [r5, #356]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+56
	mov	r1, #5
	str	r0, [r5, #360]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+60
	mov	r1, #5
	str	r0, [r5, #364]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #368]
.L2210:
	ldr	r1, .L2250+64
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #10]
	cmp	r0, #0
	bne	.L2249
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2221
.L2220:
	ldr	r1, .L2250+68
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2250+72
	strb	r0, [r5, #11]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #12]
	cmp	r0, #0
	bne	.L2222
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
.L2223:
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2249:
	ldr	r1, .L2250+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r2, .L2250+28
	mov	r1, #4
	str	r0, [r5, #408]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+32
	mov	r1, #4
	strb	r0, [r5, #404]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [r5, #408]
	cmp	r3, #31
	strb	r0, [r5, #405]
	bhi	.L2216
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	addeq	r8, r5, #368
	addeq	r7, r5, #408
	addeq	r8, r8, #3
	bne	.L2216
.L2218:
	ldr	r1, .L2250+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+40
	add	r6, r6, #1
	str	r0, [r7, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+44
	str	r0, [r7, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r8, #1]!
	ldr	r3, [r5, #408]
	cmp	r3, r6
	bcs	.L2218
	ldr	r2, .L2250+48
	mov	r1, #5
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+52
	mov	r1, #5
	str	r0, [r5, #668]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+56
	mov	r1, #5
	str	r0, [r5, #672]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+60
	mov	r1, #5
	str	r0, [r5, #676]
	mov	r0, r4
	bl	mvc_u_v
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	cmp	r3, #0
	str	r0, [r5, #680]
	bne	.L2221
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2220
.L2221:
	ldr	r1, .L2250+76
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #14]
	b	.L2220
.L2248:
	ldr	r2, .L2250+80
	mov	r1, #32
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+84
	mov	r1, #32
	str	r0, [r5, #52]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2250+88
	str	r0, [r5, #56]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #9]
	cmp	r0, #1
	bne	.L2208
	ldr	r1, [r5, #52]
	cmp	r1, #0
	beq	.L2208
	ldr	r0, [r5, #56]
	bl	__aeabi_uidiv
	mov	r0, r0, lsr #1
	str	r0, [r4, #56]
	b	.L2208
.L2247:
	ldr	r1, .L2250+92
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+96
	str	r0, [r5, #44]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #48]
	b	.L2206
.L2246:
	ldr	r2, .L2250+100
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2250+104
	str	r0, [r5, #28]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2250+108
	strb	r0, [r5, #4]
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #5]
	cmp	r0, #0
	beq	.L2205
	ldr	r2, .L2250+112
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+116
	mov	r1, #8
	str	r0, [r5, #32]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+120
	mov	r1, #8
	str	r0, [r5, #36]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #40]
	b	.L2205
.L2245:
	ldr	r1, .L2250+124
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #2]
	b	.L2203
.L2244:
	ldr	r2, .L2250+128
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	uxtb	r0, r0
	strb	r0, [r5, #15]
	cmp	r0, #255
	bne	.L2202
	ldr	r2, .L2250+132
	mov	r1, #16
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2250+136
	mov	r1, #16
	str	r0, [r5, #16]
	mov	r0, r4
	bl	mvc_u_v
	str	r0, [r5, #20]
	b	.L2202
.L2222:
	ldr	r1, .L2250+140
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2250+144
	strb	r0, [r5, #13]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+148
	str	r0, [r5, #684]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+152
	str	r0, [r5, #688]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+156
	str	r0, [r5, #696]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+160
	str	r0, [r5, #692]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2250+164
	str	r0, [r5, #700]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #704]
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L2223
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2216:
	ldr	r3, .L2250+168
	mov	r0, #1
	ldr	r1, .L2250+172
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2251:
	.align	2
.L2250:
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
	.word	vfmw_Osal_Func_Ptr_S
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
	ldreq	ip, .L2287
	movweq	r3, #10490
	beq	.L2284
	cmp	r1, #0
	beq	.L2285
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
	beq	.L2256
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
.L2261:
	ldrb	r2, [r4, #1]!	@ zero_extendqisi2
	ldrsb	ip, [r5, #1]!
	sxtb	r1, r2
	cmp	ip, r1
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, #0
	beq	.L2257
	cmp	r0, #5
	bhi	.L2258
	add	r2, r6, #748
	add	r10, r6, #812
	mov	r1, r7
.L2259:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2259
.L2257:
	add	r0, r0, #1
	add	r8, r8, #256
	cmp	r0, #8
	add	r9, r9, #256
	add	r6, r6, #64
	add	r7, r7, #64
	bne	.L2261
	ldr	r5, [fp, #-48]
	ldr	r1, [fp, #-52]
.L2256:
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
	beq	.L2263
	cmp	ip, #1
	bne	.L2262
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
	beq	.L2262
	add	ip, r5, #2912
	add	r0, r1, #2912
	add	ip, ip, #4
	add	r0, r0, #4
	mov	r2, #0
.L2265:
	ldr	r4, [ip, #4]!
	add	r2, r2, #1
	ldr	lr, [r0, #4]!
	cmp	r4, lr
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r6
	bne	.L2265
.L2262:
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
	bne	.L2266
	ldrb	r0, [r5, #21]	@ zero_extendqisi2
	ldrb	r2, [r1, #21]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
.L2266:
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
	bne	.L2286
.L2267:
	ldrb	r0, [r5, #24]	@ zero_extendqisi2
	ldrb	r2, [r1, #24]	@ zero_extendqisi2
	cmp	r0, r2
	movne	r0, #0
	andeq	r0, r3, #1
	eor	r0, r0, #1
	rsb	r0, r0, #0
.L2254:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2286:
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
	b	.L2267
.L2258:
	sub	r2, r8, #404
	sub	r10, r8, #148
	mov	r1, r9
.L2260:
	ldr	lr, [r2, #4]!
	ldr	ip, [r1, #4]!
	cmp	lr, ip
	movne	r3, #0
	andeq	r3, r3, #1
	cmp	r2, r10
	bne	.L2260
	b	.L2257
.L2263:
	ldr	r0, [r5, #2904]
	ldr	r2, [r1, #2904]
	cmp	r0, r2
	movne	r3, #0
	andeq	r3, r3, #1
	b	.L2262
.L2285:
	ldr	ip, .L2287
	mov	r0, r1
	movw	r3, #10491
.L2284:
	ldr	r2, .L2287+4
	ldr	r1, .L2287+8
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L2254
.L2288:
	.align	2
.L2287:
	.word	vfmw_Osal_Func_Ptr_S
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
	cmp	r0, #0
	moveq	r4, r0
	beq	.L2290
	subs	lr, r0, #255
	movne	lr, #1
	cmp	r0, #16
	movle	r4, #0
	andgt	r4, lr, #1
	cmp	r4, #0
	movne	r4, #0
	bne	.L2290
	cmp	r2, #0
	cmpne	r1, #0
	moveq	ip, #1
	movne	ip, #0
	cmp	r0, #255
	movne	ip, #0
	andeq	ip, ip, #1
	cmp	ip, #0
	bne	.L2290
	cmp	r0, #1
	moveq	r4, #5
	beq	.L2290
	cmp	lr, #0
	ldrne	r2, .L2312
	addne	r0, r2, r0, lsl #3
	ldrne	r1, [r0, #100]
	ldrne	r2, [r0, #104]
	mul	r0, r3, r1
	ldr	r3, [fp, #4]
	mul	r1, r3, r2
	mov	r0, r0, asl #10
	bl	__aeabi_idiv
	movw	r3, #2405
	movw	r2, #2262
	cmp	r0, r3
	rsble	r3, r0, #2400
	subgt	r3, r0, #2400
	addle	r3, r3, #6
	subgt	r3, r3, #6
	cmp	r0, r2
	bgt	.L2294
	rsb	r2, r0, #2256
	add	r2, r2, #7
	cmp	r3, r2
	bge	.L2295
.L2305:
	mov	r4, #4
.L2290:
	ldr	r3, .L2312+4
	mov	r2, r4
	ldr	r1, .L2312+8
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2294:
	sub	r2, r0, #2256
	sub	r2, r2, #7
	cmp	r3, r2
	blt	.L2305
.L2295:
	movw	r3, #1819
	cmp	r0, r3
	bgt	.L2296
	rsb	r3, r0, #1808
	add	r3, r3, #12
	cmp	r2, r3
	blt	.L2307
.L2297:
	movw	r2, #1364
	cmp	r0, r2
	rsble	r0, r0, #1360
	subgt	r0, r0, #1360
	addle	r0, r0, #5
	subgt	r0, r0, #5
	cmp	r3, r0
	movlt	r4, #2
	movge	r4, #1
	b	.L2290
.L2296:
	sub	r3, r0, #1808
	sub	r3, r3, #12
	cmp	r2, r3
	bge	.L2297
.L2307:
	mov	r4, #3
	b	.L2290
.L2313:
	.align	2
.L2312:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	beq	.L2428
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
	bne	.L2429
.L2327:
	ldr	r1, .L2443
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #12
	str	r0, [r5, #2896]
	bhi	.L2340
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2340
	ldr	r1, .L2443+4
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #2
	str	r0, [r5, #2900]
	bhi	.L2342
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2342
	cmp	r0, #0
	beq	.L2430
	cmp	r0, #1
	beq	.L2431
.L2348:
	ldr	r1, .L2443+8
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #3944]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	ldr	r1, .L2443+12
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #19]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	ldr	r1, .L2443+16
	mov	r0, r4
	bl	mvc_ue_v
	movw	r7, #509
	sub	r3, r0, #1
	str	r0, [r5, #3948]
	cmp	r3, r7
	bhi	.L2353
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2353
	ldr	r1, .L2443+20
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #3952]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	ldr	r1, .L2443+24
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #20]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	cmp	r0, #0
	bne	.L2432
	ldr	r1, .L2443+28
	mov	r0, r4
	bl	mvc_u_1
	ldr	r6, [r5, #3952]
	add	r6, r6, #1
	cmp	r6, #255
	strb	r0, [r5, #21]
	bhi	.L2386
	ldr	r3, [r5, #3948]
	add	r3, r3, #1
	cmp	r6, #1
	cmphi	r3, #3
	movhi	r8, #2
	bhi	.L2357
.L2386:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+36
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2429:
	ldr	r1, .L2443+40
	bl	mvc_ue_v
	cmp	r0, #1
	str	r0, [r5, #748]
	bhi	.L2318
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2318
	ldr	r1, .L2443+44
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L2320
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2321
.L2320:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+48
	ldr	r3, [r3, #68]
	blx	r3
.L2321:
	ldr	r1, .L2443+52
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #0
	bne	.L2322
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2323
.L2322:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+56
	ldr	r3, [r3, #68]
	blx	r3
.L2323:
	ldr	r1, .L2443+60
	mov	r0, r4
	bl	mvc_u_1
	cmp	r0, #0
	bne	.L2324
	ldrb	r10, [r4, #10]	@ zero_extendqisi2
	cmp	r10, #0
	bne	.L2324
	ldr	r1, .L2443+64
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #27]
	cmp	r0, #1
	bne	.L2327
	sub	r7, fp, #72
	add	r9, r5, #5
	add	r6, r5, #2672
	add	r8, r5, #752
	b	.L2333
.L2435:
	ldr	r3, .L2443+68
	cmp	r1, #0
	add	ip, r3, #16
	moveq	r3, ip
	mov	r1, r3
	ldr	r3, .L2443+32
	ldr	r3, [r3, #52]
	blx	r3
.L2329:
	add	r10, r10, #1
	add	r6, r6, #16
	cmp	r10, #6
	add	r8, r8, #64
	beq	.L2433
.L2333:
	ldr	r1, .L2443+72
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
	beq	.L2434
	cmp	r10, #0
	cmpne	r10, #3
	beq	.L2435
	ldr	r3, .L2443+32
	mov	r2, #16
	sub	r1, r6, #16
	mov	r0, r6
	ldr	r3, [r3, #52]
	blx	r3
	b	.L2329
.L2384:
	cmp	r1, r9
	cmpcs	r0, r6
	bcs	.L2383
	ldr	r3, .L2443+76
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2426
	strh	r1, [fp, #-72]	@ movhi
	mov	r3, #8
	strh	r0, [fp, #-70]	@ movhi
	sub	r2, fp, #76
	strh	r9, [fp, #-76]	@ movhi
	mov	r1, #107
	strh	r6, [fp, #-74]	@ movhi
	ldr	r0, [r4, #120]
	blx	ip
.L2426:
	mvn	r0, #0
.L2419:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2432:
	ldr	r6, [r5, #3952]
	strb	r3, [r5, #21]
	sub	r3, r6, #1
	cmp	r3, r7
	bhi	.L2386
	ldr	r3, [r5, #3948]
	mov	r8, #1
	add	r6, r6, #1
	add	r3, r3, r8
.L2357:
	mul	r6, r6, r8
	mov	r9, r3, asl #4
	mov	r6, r6, asl #4
	mul	r1, r9, r6
	add	r1, r1, r1, lsr #1
	cmp	r1, #100663296
	bhi	.L2436
	ldr	r2, [r5, #740]
	sub	r3, r2, #10
	cmp	r3, #31
	ldrls	pc, [pc, r3, asl #2]
	b	.L2359
.L2361:
	.word	.L2360
	.word	.L2362
	.word	.L2394
	.word	.L2394
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2394
	.word	.L2364
	.word	.L2366
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2366
	.word	.L2367
	.word	.L2368
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2359
	.word	.L2370
	.word	.L2370
.L2430:
	ldr	r1, .L2443+80
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #12
	str	r0, [r5, #2904]
	bhi	.L2345
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2348
.L2345:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+84
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2394:
	mov	r0, #60416
	movt	r0, 13
.L2363:
	bl	__aeabi_uidiv
	ldr	r10, .L2443+32
	ldr	r1, .L2443+88
	ldr	r3, [r10, #68]
	cmp	r0, #16
	movcc	r7, r0
	movcs	r7, #16
	mov	r2, r7
	mov	r0, #21
	blx	r3
	ldr	r2, [r5, #3944]
	cmp	r2, r7
	bhi	.L2371
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2372
.L2371:
	mov	r3, r7
	ldr	ip, [r10, #68]
	ldr	r1, .L2443+92
	mov	r0, #0
	blx	ip
	ldr	r3, [r5, #3944]
	cmp	r3, #16
	bhi	.L2423
	cmp	r7, r3
	movcc	r7, r3
.L2423:
	str	r7, [r5, #3944]
.L2372:
	ldr	r1, .L2443+96
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #22]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	ldr	r1, .L2443+100
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r0, r0
	strb	r0, [r5, #23]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	cmp	r0, #0
	bne	.L2437
.L2374:
	mov	r2, r9, lsr #1
	mov	r3, r6, lsr #1
	str	r9, [r5, #3976]
	str	r6, [r5, #3980]
	str	r2, [r5, #3984]
	str	r3, [r5, #3988]
.L2376:
	ldr	r1, .L2443+104
	mov	r0, r4
	bl	mvc_u_1
	uxtb	r3, r0
	strb	r3, [r5, #24]
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2426
	cmp	r3, #0
	mov	r2, #2
	str	r2, [r5, #68]
	bne	.L2438
	mov	r0, r3
	strb	r3, [r5, #43]
	mov	r3, #5
	str	r3, [r5, #56]
.L2379:
	ldr	r2, [r5, #48]
	mov	r3, r9
	ldr	r1, [r5, #44]
	str	r6, [sp]
	bl	MVC_GetDar
	ldr	r3, [r5, #3944]
	cmp	r3, #0
	str	r0, [r5, #52]
	beq	.L2382
	cmp	r7, r3
	movcs	r7, r3
.L2382:
	ldr	r3, [r4, #224]
	cmp	r7, #16
	movcs	r7, #16
	ldr	r2, [r3, #28]
	cmp	r2, #25
	beq	.L2439
.L2383:
	cmp	r6, #1920
	bcc	.L2385
	cmp	r7, #4
	movcs	r7, #4
.L2385:
	add	r7, r7, #1
	mov	r0, #0
	str	r7, [r5, #3972]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2370:
	mov	r0, #12582912
	b	.L2363
.L2366:
	mov	r0, #30208
	movt	r0, 47
	b	.L2363
.L2433:
	sub	r7, fp, #88
	add	r9, r5, #11
	add	r6, r5, #2768
	add	r8, r5, #1136
	mov	r10, #0
.L2338:
	ldr	r1, .L2443+72
	mov	r0, r4
	bl	mvc_u_1
	mov	r2, #64
	mov	r3, r0
	cmp	r3, #1
	mov	r0, r6
	str	r3, [r7, #4]!
	strb	r3, [r9, #1]!
	beq	.L2440
	ldr	r1, .L2443+108
	cmp	r10, #0
	ldr	r3, .L2443+32
	add	ip, r1, #64
	ldr	r3, [r3, #52]
	movne	r1, ip
	blx	r3
.L2335:
	add	r10, r10, #1
	add	r6, r6, #64
	cmp	r10, #2
	add	r8, r8, #256
	bne	.L2338
	b	.L2327
.L2434:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2329
	ldr	r1, .L2443+68
	mov	r2, #16
	ldr	r3, .L2443+32
	cmp	r10, #2
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movhi	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2329
.L2318:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+112
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2340:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+116
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2437:
	ldr	r1, .L2443+120
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2443+124
	str	r0, [r5, #3956]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2443+128
	str	r0, [r5, #3960]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2443+132
	str	r0, [r5, #3964]
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r2, [r5, #3964]
	ldr	r1, [r5, #3956]
	mov	ip, r8, asl #1
	ldr	r3, [r5, #3960]
	mov	r2, r2, asl #1
	mov	r1, r1, asl #1
	mul	r8, r8, r2
	rsb	r2, r1, r9
	sub	r3, r2, r3, asl #1
	rsb	r2, r8, r6
	str	r0, [r5, #3968]
	mls	r0, r0, ip, r2
	cmp	r3, #0
	cmpgt	r0, #0
	ble	.L2374
	add	r1, r1, r3, lsr #1
	add	r8, r8, r0, lsr #1
	str	r1, [r5, #3984]
	str	r8, [r5, #3988]
	str	r3, [r5, #3976]
	str	r0, [r5, #3980]
	b	.L2376
.L2438:
	add	r1, r5, #28
	mov	r0, r4
	bl	mvc_vui_parameters
	cmp	r0, #0
	bne	.L2425
	ldrb	r3, [r5, #40]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2425
	ldr	r2, [r5, #732]
	clz	r3, r2
	mov	r3, r3, lsr #5
	cmp	r7, r2
	orrcc	r3, r3, #1
	cmp	r3, #0
	bne	.L2441
	ldr	r3, [r5, #3944]
	ldrb	r0, [r5, #43]	@ zero_extendqisi2
	cmp	r2, r3
	movcs	r7, r2
	movcc	r7, r3
	b	.L2379
.L2441:
	ldr	ip, [r10, #68]
	mov	r3, r7
	ldr	r1, .L2443+136
	mov	r0, #1
	blx	ip
.L2425:
	ldrb	r0, [r5, #43]	@ zero_extendqisi2
	b	.L2379
.L2439:
	ldr	r2, [r3, #776]
	cmp	r2, #0
	bne	.L2383
	ldr	r2, [r3, #760]
	ldr	r1, [r3, #736]
	cmp	r7, r2
	ldr	r0, [r3, #740]
	bls	.L2384
	ldr	r3, .L2443+76
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2426
	str	r2, [fp, #-72]
	mov	r3, #8
	str	r7, [fp, #-76]
	sub	r2, fp, #76
	ldr	r0, [r4, #120]
	mov	r1, #106
	blx	ip
	mvn	r0, #0
	b	.L2419
.L2324:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+140
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2431:
	ldr	r1, .L2443+144
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r5, #18]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	ldr	r1, .L2443+148
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r5, #2908]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	ldr	r1, .L2443+152
	mov	r0, r4
	bl	mvc_se_v
	str	r0, [r5, #2912]
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2426
	ldr	r1, .L2443+156
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #255
	str	r0, [r5, #2916]
	bhi	.L2350
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2350
	cmp	r0, #0
	addne	r7, r5, #2912
	addne	r7, r7, #4
	bne	.L2352
	b	.L2348
.L2442:
	ldr	r3, [r5, #2916]
	cmp	r3, r6
	bls	.L2348
.L2352:
	ldr	r1, .L2443+160
	mov	r0, r4
	bl	mvc_se_v
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2442
	b	.L2426
.L2353:
	ldr	r3, .L2443+32
	mov	r2, r0
	ldr	r1, .L2443+164
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2367:
	mov	r0, #30720
	movt	r0, 105
	b	.L2363
.L2364:
	mov	r0, #55296
	movt	r0, 27
	b	.L2363
.L2368:
	mov	r0, #7864320
	b	.L2363
.L2362:
	mov	r0, #17920
	movt	r0, 5
	b	.L2363
.L2360:
	mov	r0, #20992
	movt	r0, 2
	b	.L2363
.L2359:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+168
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2350:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+172
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2440:
	mov	r3, r2
	str	r7, [sp]
	mov	r2, r6
	mov	r1, r8
	mov	r0, r4
	bl	MVC_Scaling_List
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L2335
	ldr	r1, .L2443+108
	mov	r2, #64
	ldr	r3, .L2443+32
	cmp	r10, #0
	add	r0, r1, r2
	ldr	r3, [r3, #52]
	movne	r1, r0
	mov	r0, r6
	blx	r3
	b	.L2335
.L2342:
	ldr	r3, .L2443+32
	mov	r0, #1
	ldr	r1, .L2443+176
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2436:
	ldr	r3, .L2443+32
	mov	r0, #0
	ldr	r1, .L2443+180
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2419
.L2428:
	ldr	ip, .L2443+32
	mov	r0, r5
	movw	r3, #10680
	ldr	r2, .L2443+184
	ldr	r1, .L2443+188
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L2419
.L2444:
	.align	2
.L2443:
	.word	.LC225
	.word	.LC227
	.word	.LC230
	.word	.LC238
	.word	.LC239
	.word	.LC241
	.word	.LC242
	.word	.LC256
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC243
	.word	.LC216
	.word	.LC218
	.word	.LC219
	.word	.LC220
	.word	.LC221
	.word	.LC222
	.word	.LC224
	.word	.LANCHOR1
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
	ldr	r2, .L2490
	mov	r1, #8
	mov	r4, r0
	bl	mvc_u_v
	ldr	r1, .L2490+4
	mov	r7, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2490+8
	mov	r9, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2490+12
	mov	r8, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2490+16
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2490+20
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2490+24
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r2, .L2490+28
	mov	r1, #2
	str	r0, [fp, #-68]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2490+32
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2490+36
	mov	r5, r0
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	cmp	r2, #0
	mov	r6, r0
	bne	.L2483
	ldr	r2, [r4, #36]
	sub	r2, r2, #1
	cmp	r0, r2
	bhi	.L2485
	cmp	r7, #100
	mov	r10, r7
	beq	.L2451
	bhi	.L2452
	cmp	r7, #77
	beq	.L2451
	cmp	r7, #88
	beq	.L2453
	cmp	r7, #66
	bne	.L2450
	ldr	r3, .L2490+40
	mov	r0, #1
	ldr	r1, .L2490+44
	ldr	r3, [r3, #68]
	blx	r3
.L2451:
	cmp	r5, #9
	mov	r2, r5
	bls	.L2482
.L2488:
	cmp	r5, #41
	bhi	.L2482
.L2457:
	movw	r1, #3992
	ldr	r3, [r4, #248]
	mul	r5, r1, r6
	add	r3, r3, r5
	ldrb	ip, [r3, #25]	@ zero_extendqisi2
	cmp	ip, #0
	beq	.L2458
	ldr	r3, .L2490+48
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
	bne	.L2486
	ldr	r1, [r4, #248]
	add	r0, r3, #2240
	str	r3, [fp, #-56]
	add	r1, r1, r5
	bl	MVC_SPSEqual
	ldr	r3, [fp, #-56]
	cmp	r0, #0
	bne	.L2487
.L2447:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2452:
	cmp	r7, #122
	beq	.L2455
	cmp	r7, #144
	beq	.L2455
	cmp	r7, #110
	beq	.L2455
.L2450:
	ldr	ip, .L2490+40
	mov	r2, r7
	ldr	r1, .L2490+52
	mov	r0, #1
	mov	r10, #100
	ldr	r3, [ip, #68]
	blx	r3
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2488
.L2482:
	ldr	ip, .L2490+40
	mov	r3, #41
	ldr	r1, .L2490+56
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mov	r2, #41
	b	.L2457
.L2458:
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
	bne	.L2489
	ldr	r3, [r4, #248]
	mov	r2, #1
	add	r3, r3, r5
	strb	r2, [r3, #26]
	ldr	r3, [r4, #248]
	add	r5, r3, r5
	strb	r2, [r5, #25]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2485:
	ldr	r3, .L2490+40
	mov	r0, #1
	ldr	r1, .L2490+60
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r2, [r4, #36]
	sub	r3, r2, #1
	cmp	r6, r3
	bls	.L2483
	ldr	r3, .L2490+64
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L2483
	str	r2, [fp, #-48]
	mov	r3, #8
	str	r6, [fp, #-52]
	sub	r2, fp, #52
	ldr	r0, [r4, #120]
	mov	r1, #109
	blx	ip
.L2483:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2455:
	ldr	r3, .L2490+40
	mov	r0, #1
	ldr	r1, .L2490+68
	ldr	r3, [r3, #68]
	blx	r3
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2488
	b	.L2482
.L2453:
	ldr	r3, .L2490+40
	mov	r0, #1
	ldr	r1, .L2490+72
	ldr	r3, [r3, #68]
	blx	r3
	cmp	r5, #9
	mov	r2, r5
	bhi	.L2488
	b	.L2482
.L2487:
	ldr	lr, .L2490+40
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
	b	.L2447
.L2486:
	ldr	ip, .L2490+40
	mov	r3, r6
	movw	r2, #11224
	ldr	r1, .L2490+76
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L2447
.L2489:
	ldr	lr, .L2490+40
	mov	r3, r6
	movw	r2, #11251
	ldr	r1, .L2490+76
	mov	r0, #1
	str	ip, [fp, #-56]
	ldr	r6, [lr, #68]
	blx	r6
	ldr	r3, [r4, #248]
	mov	r2, #1
	ldr	ip, [fp, #-56]
	add	r3, r3, r5
	mvn	r0, #0
	strb	r2, [r3, #26]
	ldr	r3, [r4, #248]
	add	r5, r3, r5
	strb	ip, [r5, #25]
	b	.L2447
.L2491:
	.align	2
.L2490:
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
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC267
	.word	.LANCHOR2
	.word	.LC270
	.word	.LC271
	.word	.LC170
	.word	g_event_report
	.word	.LC269
	.word	.LC268
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
	beq	.L2557
	ldr	r1, .L2558
	bl	mvc_ue_v
	cmp	r0, #1
	str	r0, [r5, #4]
	bhi	.L2497
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	addeq	r7, r5, #4
	bne	.L2497
.L2499:
	ldr	r1, .L2558+4
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r7, #4]!
	ldr	r3, [r5, #4]
	cmp	r3, r6
	bcs	.L2499
	cmp	r3, #0
	mov	r2, #0
	str	r2, [r5, #16]
	str	r2, [r5, #24]
	beq	.L2500
	add	r8, r5, #16
	mov	r7, #1
.L2510:
	ldr	r1, .L2558+8
	mov	r0, r4
	bl	mvc_ue_v
	add	r9, r8, #4
	str	r0, [r8, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcs	r3, #15
	cmp	r0, r3
	bhi	.L2501
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2501
	cmp	r0, #0
	addne	r10, r8, #16
	beq	.L2506
.L2505:
	ldr	r1, .L2558+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r10, #4]!
	ldr	r3, [r9]
	cmp	r3, r6
	bhi	.L2505
.L2506:
	ldr	r1, .L2558+16
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #8]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcc	r2, r3
	movcs	r2, #15
	cmp	r0, r2
	bhi	.L2503
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2503
	cmp	r0, #0
	addne	r8, r8, #24
	beq	.L2508
.L2509:
	ldr	r1, .L2558+20
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r8, #4]!
	ldr	r3, [r9, #8]
	cmp	r3, r6
	bhi	.L2509
	ldr	r3, [r5, #4]
.L2508:
	add	r7, r7, #1
	mov	r8, r9
	cmp	r7, r3
	bls	.L2510
	cmp	r3, #0
	mov	r3, #0
	addne	r8, r5, #48
	str	r3, [r5, #48]
	str	r3, [r5, #56]
	movne	r7, #1
	beq	.L2524
.L2523:
	ldr	r1, .L2558+24
	mov	r0, r4
	bl	mvc_ue_v
	add	r9, r8, #4
	str	r0, [r8, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcs	r3, #15
	cmp	r0, r3
	bhi	.L2514
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2514
	cmp	r0, #0
	addne	r10, r8, #16
	beq	.L2519
.L2518:
	ldr	r1, .L2558+28
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r10, #4]!
	ldr	r3, [r9]
	cmp	r3, r6
	bhi	.L2518
.L2519:
	ldr	r1, .L2558+32
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r9, #8]
	ldr	r3, [r5, #4]
	cmp	r3, #15
	movcc	r2, r3
	movcs	r2, #15
	cmp	r0, r2
	bhi	.L2516
	ldrb	r6, [r4, #10]	@ zero_extendqisi2
	cmp	r6, #0
	bne	.L2516
	cmp	r0, #0
	addne	r8, r8, #24
	beq	.L2521
.L2522:
	ldr	r1, .L2558+36
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	str	r0, [r8, #4]!
	ldr	r3, [r9, #8]
	cmp	r3, r6
	bhi	.L2522
	ldr	r3, [r5, #4]
.L2521:
	add	r7, r7, #1
	mov	r8, r9
	cmp	r7, r3
	bls	.L2523
.L2524:
	ldr	r1, .L2558+40
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r0, #1
	str	r0, [r5, #80]
	cmp	r3, #16
	str	r3, [fp, #-60]
	bhi	.L2512
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2512
	ldr	r2, [fp, #-60]
	cmp	r2, #0
	beq	.L2529
	add	r2, r5, #8384
	add	r1, r5, #80
	add	r2, r2, #16
	str	r1, [fp, #-72]
	str	r2, [fp, #-68]
	add	r2, r5, #212
	str	r3, [fp, #-56]
	str	r2, [fp, #-64]
	str	r5, [fp, #-76]
.L2528:
	ldr	r2, .L2558+44
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-72]
	ldr	r1, .L2558+48
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
	bhi	.L2526
	ldrb	r7, [r4, #10]	@ zero_extendqisi2
	cmp	r7, #0
	bne	.L2526
	cmp	r2, #0
	beq	.L2536
	ldr	r3, [fp, #-64]
	movw	r10, #8188
	ldr	r2, [fp, #-68]
	movt	r10, 4
	add	r10, r3, r10
	mov	r8, r3
	str	r2, [fp, #-48]
.L2535:
	ldr	r2, .L2558+52
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2558+56
	str	r0, [r8], #4
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r0, #1
	str	r0, [r8, #4092]
	cmp	r6, #64
	bhi	.L2530
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2530
	cmp	r6, #0
	movne	r5, r3
	ldrne	r9, [fp, #-48]
	beq	.L2534
.L2531:
	ldr	r1, .L2558+60
	mov	r0, r4
	bl	mvc_ue_v
	add	r5, r5, #1
	cmp	r6, r5
	str	r0, [r9, #4]!
	bne	.L2531
.L2534:
	ldr	r1, .L2558+64
	mov	r0, r4
	bl	mvc_ue_v
	cmp	r0, #1024
	str	r0, [r10, #4]!
	bcs	.L2532
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2532
	ldr	r3, [fp, #-48]
	add	r7, r7, #1
	add	r3, r3, #256
	str	r3, [fp, #-48]
	ldr	r3, [fp, #-52]
	cmp	r3, r7
	bne	.L2535
.L2536:
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
	bne	.L2528
.L2529:
	mov	r0, #0
	b	.L2548
.L2501:
	ldr	r3, .L2558+68
	mov	r2, r0
	ldr	r1, .L2558+72
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
.L2548:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2503:
	ldr	r3, .L2558+68
	mov	r2, r0
	ldr	r1, .L2558+76
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2516:
	ldr	r3, .L2558+68
	mov	r2, r0
	ldr	r1, .L2558+80
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2548
.L2514:
	ldr	r3, .L2558+68
	mov	r2, r0
	ldr	r1, .L2558+84
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2548
.L2497:
	ldr	r3, .L2558+68
	mov	r2, r0
	ldr	r1, .L2558+88
	movw	r0, #65534
.L2556:
	str	r0, [r5, #80]
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2548
.L2500:
	str	r3, [r5, #48]
	str	r3, [r5, #56]
	b	.L2524
.L2532:
	ldr	r3, .L2558+68
	mov	r2, r0
	ldr	r1, .L2558+92
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2548
.L2530:
	ldr	r3, [fp, #-56]
	movw	lr, #65534
	ldr	r5, [fp, #-76]
	mov	r2, r6
	ldr	r1, .L2558+68
	mov	r0, #1
	add	r3, r7, r3, lsl #6
	add	r3, r3, #1072
	add	r3, r3, #4
	ldr	ip, [r1, #68]
	ldr	r1, .L2558+96
	add	r3, r5, r3, lsl #2
	str	lr, [r3, #4]
	blx	ip
	mvn	r0, #0
	b	.L2548
.L2557:
	ldr	ip, .L2558+68
	mov	r0, r5
	movw	r3, #11371
	ldr	r2, .L2558+100
	ldr	r1, .L2558+104
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L2548
.L2512:
	ldr	r3, .L2558+68
	movw	r0, #65534
	ldr	r2, [fp, #-60]
	ldr	r1, .L2558+108
	b	.L2556
.L2526:
	ldr	r2, [fp, #-56]
	movw	ip, #65534
	ldr	r5, [fp, #-76]
	mov	r0, #1
	ldr	r3, .L2558+68
	add	r5, r5, r2, lsl #2
	ldr	r1, .L2558+112
	ldr	r2, [fp, #-52]
	str	ip, [r5, #148]
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2548
.L2559:
	.align	2
.L2558:
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
	.word	vfmw_Osal_Func_Ptr_S
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
	@ args = 0, pretend = 0, frame = 64
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
	mov	r5, r1
	ldr	r1, .L2593
	mov	r4, r0
	bl	mvc_ue_v
	mov	r3, #0
	adds	r6, r0, #1
	adc	r7, r3, #0
	add	r3, r5, #278528
	orrs	r2, r6, r7
	strd	r6, [fp, #-108]
	str	r0, [r3, #560]
	beq	.L2579
	add	r1, r5, #294912
	add	r9, r5, #315392
	add	r9, r9, #856
	movw	r2, #16687
	mov	ip, r1
	movw	r7, #34136
	movw	r1, #16751
	movw	r0, #33844
	mov	r8, r5
	movt	r2, 4
	add	ip, ip, #820
	add	r2, r5, r2
	movt	r7, 4
	str	r2, [fp, #-96]
	movt	r1, 4
	add	r7, r5, r7
	add	r2, r5, r1
	movt	r0, 4
	str	r2, [fp, #-88]
	add	r2, r3, #111
	str	ip, [fp, #-68]
	str	r2, [fp, #-80]
	add	r2, r5, r0
	str	r7, [fp, #-48]
	str	r2, [fp, #-72]
	add	r2, r3, #560
	str	r2, [fp, #-84]
	add	r2, r3, #176
	str	r2, [fp, #-64]
	add	r2, r3, #432
	str	r2, [fp, #-52]
	add	r2, r3, #496
	str	r2, [fp, #-56]
	add	r2, r3, #240
	add	r3, r3, #816
	str	r2, [fp, #-76]
	str	r3, [fp, #-60]
	mov	r3, #0
	str	r3, [fp, #-92]
.L2578:
	ldr	r2, .L2593+4
	mov	r1, #3
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-80]
	ldr	r1, .L2593+8
	strb	r0, [r3, #1]!
	mov	r0, r4
	str	r3, [fp, #-80]
	bl	mvc_ue_v
	ldr	r3, [fp, #-84]
	adds	r5, r0, #1
	str	r0, [r3, #4]!
	str	r3, [fp, #-84]
	beq	.L2566
	ldr	r10, [fp, #-60]
	mov	r6, #0
	ldr	r7, [fp, #-48]
.L2563:
	ldr	r1, .L2593+12
	mov	r0, r4
	bl	mvc_ue_v
	add	r6, r6, #1
	cmp	r5, r6
	str	r0, [r10, #4]!
	bne	.L2563
	str	r7, [fp, #-48]
.L2566:
	ldr	r1, .L2593+16
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-88]
	uxtb	r0, r0
	strb	r0, [r3, #1]!
	cmp	r0, #0
	str	r3, [fp, #-88]
	bne	.L2591
.L2565:
	ldr	r1, .L2593+20
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-52]
	uxtb	r0, r0
	strb	r0, [r3]
	cmp	r0, #0
	beq	.L2567
	ldr	r1, .L2593+24
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r5, [fp, #-48]
	ldr	r2, .L2593+28
	mov	r1, #4
	str	r0, [r5]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34132
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2593+32
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
	bhi	.L2572
	ldrb	r5, [r4, #10]	@ zero_extendqisi2
	cmp	r5, #0
	bne	.L2572
	ldr	r7, [fp, #-48]
	movw	r10, #34099
	movt	r10, 4
	add	r10, r8, r10
	mov	r6, r7
.L2570:
	ldr	r1, .L2593+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2593+40
	add	r5, r5, #1
	str	r0, [r6, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2593+44
	str	r0, [r6, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r10, #1]!
	ldr	r3, [r7]
	cmp	r3, r5
	bcs	.L2570
	ldr	r2, .L2593+48
	mov	r1, #5
	mov	r0, r4
	str	r7, [fp, #-48]
	bl	mvc_u_v
	movw	r3, #34396
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2593+52
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	add	r3, r8, #294912
	ldr	r2, .L2593+56
	mov	r1, #5
	str	r0, [r3, #1632]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34404
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2593+60
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #34408
	movt	r3, 4
	add	r3, r8, r3
	str	r0, [r3]
.L2567:
	ldr	r1, .L2593+64
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-56]
	uxtb	r0, r0
	strb	r0, [r3]
	cmp	r0, #0
	bne	.L2592
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L2577
.L2576:
	ldr	r1, .L2593+68
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-92]
	ldrd	r6, [fp, #-108]
	add	r8, r8, #312
	add	r1, r3, #1
	mov	r3, #0
	cmp	r7, r3
	ldr	r3, [fp, #-52]
	mov	r2, r1
	str	r1, [fp, #-92]
	add	r3, r3, #1
	str	r3, [fp, #-52]
	ldr	r3, [fp, #-56]
	cmpeq	r6, r2
	ldr	r1, [fp, #-64]
	add	r9, r9, #312
	add	r3, r3, #1
	str	r3, [fp, #-56]
	ldr	r3, [fp, #-48]
	add	r1, r1, #1
	str	r1, [fp, #-64]
	add	r3, r3, #312
	str	r3, [fp, #-48]
	ldr	r3, [fp, #-68]
	add	r3, r3, #4
	str	r3, [fp, #-68]
	ldr	r3, [fp, #-72]
	add	r3, r3, #4
	str	r3, [fp, #-72]
	ldr	r3, [fp, #-76]
	add	r3, r3, #1
	str	r3, [fp, #-76]
	ldr	r3, [fp, #-60]
	add	r3, r3, #256
	str	r3, [fp, #-60]
	ldr	r3, [fp, #-96]
	strb	r0, [r3, #1]!
	str	r3, [fp, #-96]
	bhi	.L2578
.L2579:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2592:
	ldr	r1, .L2593+24
	mov	r0, r4
	bl	mvc_ue_v
	add	r3, r8, #315392
	ldr	r2, .L2593+28
	mov	r1, #4
	mov	r5, r3
	str	r3, [fp, #-100]
	str	r0, [r9]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2593+32
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
	bhi	.L2572
	ldrb	r5, [r4, #10]	@ zero_extendqisi2
	cmp	r5, #0
	bne	.L2572
	movw	r10, #54067
	ldr	r7, [fp, #-48]
	movt	r10, 4
	add	r10, r8, r10
	mov	r6, r9
.L2574:
	ldr	r1, .L2593+36
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2593+40
	add	r5, r5, #1
	str	r0, [r6, #4]!
	mov	r0, r4
	bl	mvc_ue_v
	ldr	r1, .L2593+44
	str	r0, [r6, #128]
	mov	r0, r4
	bl	mvc_u_1
	strb	r0, [r10, #1]!
	ldr	r3, [r9]
	cmp	r3, r5
	bcs	.L2574
	ldr	r2, .L2593+48
	mov	r1, #5
	mov	r0, r4
	str	r7, [fp, #-48]
	bl	mvc_u_v
	movw	r3, #54364
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2593+52
	mov	r1, #5
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-100]
	ldr	r2, .L2593+56
	mov	r1, #5
	str	r0, [r3, #1120]
	mov	r0, r4
	bl	mvc_u_v
	movw	r3, #54372
	movt	r3, 4
	add	r3, r8, r3
	ldr	r2, .L2593+60
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
	bne	.L2577
	ldr	r3, [fp, #-56]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2576
.L2577:
	ldr	r1, .L2593+72
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-64]
	strb	r0, [r3]
	b	.L2576
.L2591:
	ldr	r2, .L2593+76
	mov	r1, #32
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-68]
	ldr	r2, .L2593+80
	mov	r1, #32
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r3, [fp, #-72]
	ldr	r1, .L2593+84
	str	r0, [r3]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r3, [fp, #-76]
	strb	r0, [r3]
	b	.L2565
.L2572:
	ldr	r3, .L2593+88
	mov	r0, #1
	ldr	r1, .L2593+92
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2594:
	.align	2
.L2593:
	.word	.LC299
	.word	.LC300
	.word	.LC301
	.word	.LC303
	.word	.LC302
	.word	.LC307
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
	.word	.LC320
	.word	.LC319
	.word	.LC304
	.word	.LC305
	.word	.LC306
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC196
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
	ldr	r2, .L2623
	mov	r1, #8
	mov	r4, r0
	bl	mvc_u_v
	ldr	r1, .L2623+4
	mov	r7, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2623+8
	mov	r10, r0
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2623+12
	str	r0, [fp, #-48]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2623+16
	str	r0, [fp, #-52]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2623+20
	str	r0, [fp, #-56]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r1, .L2623+24
	str	r0, [fp, #-60]
	mov	r0, r4
	bl	mvc_u_1
	ldr	r2, .L2623+28
	mov	r1, #2
	str	r0, [fp, #-64]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2623+32
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2623+36
	mov	r8, r0
	mov	r0, r4
	bl	mvc_ue_v
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r5, r0
	bne	.L2608
	cmp	r0, #31
	bhi	.L2618
	sub	r3, r7, #66
	cmp	r3, #78
	ldrls	pc, [pc, r3, asl #2]
	b	.L2598
.L2600:
	.word	.L2599
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2601
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2602
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2601
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2603
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2601
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2603
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2601
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2598
	.word	.L2603
.L2599:
	ldr	r3, .L2623+40
	mov	r0, #1
	ldr	r1, .L2623+44
	ldr	r3, [r3, #68]
	blx	r3
.L2601:
	cmp	r8, #41
	bhi	.L2619
.L2604:
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
	bne	.L2620
	cmp	r7, #118
	cmpne	r7, #128
	mov	r10, #1
	strb	r10, [r6, #957]
	beq	.L2621
.L2606:
	movw	r3, #8500
	mov	r2, #1
	movt	r3, 5
	mla	r4, r3, r5, r4
	add	r4, r4, #12992
	strb	r2, [r4, #12]
.L2596:
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2619:
	ldr	r3, .L2623+40
	mov	r2, r8
	ldr	r1, .L2623+48
	mov	r0, #1
	mov	r8, #41
	ldr	r3, [r3, #68]
	blx	r3
	b	.L2604
.L2621:
	ldr	r1, .L2623+52
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
	bne	.L2622
	ldr	r1, .L2623+56
	mov	r0, r4
	bl	mvc_u_1
	cmp	r0, #0
	str	r0, [r6, #928]
	beq	.L2606
	mov	r1, r7
	mov	r0, r4
	bl	MVC_ProcessSUBSPSMvcVuiExt
	cmp	r0, #0
	beq	.L2606
	ldr	r3, .L2623+40
	mov	r0, r10
	ldr	r1, .L2623+60
	ldr	r3, [r3, #68]
	blx	r3
	strb	r8, [r9, #4]
	mvn	r8, #0
	b	.L2596
.L2603:
	ldr	r3, .L2623+40
	mov	r0, #1
	ldr	r1, .L2623+64
	ldr	r3, [r3, #68]
	blx	r3
	b	.L2601
.L2602:
	ldr	r3, .L2623+40
	mov	r0, #1
	ldr	r1, .L2623+68
	ldr	r3, [r3, #68]
	blx	r3
	b	.L2601
.L2598:
	ldr	r3, .L2623+40
	mov	r2, r7
	ldr	r1, .L2623+72
	mov	r0, #1
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2608:
	mvn	r8, #0
	b	.L2596
.L2618:
	ldr	r3, .L2623+40
	mov	r0, #1
	ldr	r1, .L2623+76
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L2596
.L2620:
	ldr	r3, .L2623+40
	mov	r0, #1
	ldr	r1, .L2623+80
	mvn	r8, #0
	ldr	r3, [r3, #68]
	blx	r3
	add	ip, r9, #12992
	mov	r3, #0
	strb	r3, [ip, #12]
	strb	r3, [r6, #957]
	b	.L2596
.L2622:
	ldr	r3, .L2623+40
	mov	r0, r10
	ldr	r1, .L2623+84
	ldr	r3, [r3, #68]
	blx	r3
	strb	r8, [r9, #4]
	mvn	r8, #0
	b	.L2596
.L2624:
	.align	2
.L2623:
	.word	.LC321
	.word	.LC322
	.word	.LC323
	.word	.LC324
	.word	.LC325
	.word	.LC326
	.word	.LC327
	.word	.LC328
	.word	.LC329
	.word	.LC330
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC267
	.word	.LC332
	.word	.LC334
	.word	.LC336
	.word	.LC337
	.word	.LC269
	.word	.LC268
	.word	.LC331
	.word	.LC170
	.word	.LC333
	.word	.LC335
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
	beq	.L2634
	ldrb	r3, [ip]	@ zero_extendqisi2
	mov	r2, r3, asl #2
	mov	r5, r3, asl #5
	rsb	r4, r2, r5
	add	r4, ip, r4
	add	r6, r4, #8
	ldr	lr, [r4, #8]
	cmp	lr, #0
	beq	.L2634
	ldr	lr, [ip, #68]
	cmp	r3, #1
	cmpls	lr, #2
	bhi	.L2634
	cmp	lr, r3
	bls	.L2634
	ldr	lr, [r4, #12]
	mov	r6, r2
	ldr	r4, [r4, #24]
	mov	lr, lr, asl #3
	add	r1, r4, r1, lsl #3
	cmp	r1, lr
	bhi	.L2629
	b	.L2627
.L2630:
	ldr	r4, [r2, #24]
	ldr	r2, [r2, #12]
	rsb	lr, lr, r4
	add	r1, r1, lr
	mov	lr, r2, asl #3
	cmp	r1, lr
	bls	.L2627
.L2629:
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
	bhi	.L2630
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2627:
	rsb	r2, r6, r5
	mov	r0, #1
	add	ip, ip, r2
	str	r1, [ip, #24]
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L2634:
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
	beq	.L2650
	ldr	r3, [r0, #232]
	cmp	r3, #0
	cmpne	r2, #0
	beq	.L2650
	ldrb	r7, [r3]	@ zero_extendqisi2
	mov	ip, r7, asl #5
	sub	ip, ip, r7, asl #2
	add	ip, r3, ip
	ldr	ip, [ip, #8]
	cmp	ip, #0
	beq	.L2650
	ldr	ip, [r3, #68]
	cmp	r7, #1
	cmpls	ip, #2
	movhi	lr, #1
	movls	lr, #0
	bhi	.L2650
	cmp	ip, r7
	bls	.L2650
.L2645:
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
	bcs	.L2640
	ldrb	r3, [r8, r4, lsr #3]	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	r2, lr
	add	r4, r1, #1
	add	ip, ip, #1
	strb	r3, [r1]
	b	.L2652
.L2643:
	cmp	ip, r6
	mov	r1, r4
	beq	.L2640
	ldrb	r3, [ip], #1	@ zero_extendqisi2
	add	lr, lr, #1
	cmp	r2, lr
	strb	r3, [r4], #1
.L2652:
	ldr	r3, [r0, #232]
	add	r3, r3, r5
	ldr	r1, [r3, #24]
	add	r1, r1, #8
	str	r1, [r3, #24]
	bhi	.L2643
	mov	r0, r2
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2640:
	ldr	ip, [r0, #232]
	add	r7, r7, #1
	ldrb	r3, [ip]	@ zero_extendqisi2
	add	r3, r3, #1
	strb	r3, [ip]
	ldr	r3, [r0, #232]
	ldr	ip, [r3, #68]
	cmp	ip, r7
	bhi	.L2645
	mov	r0, lr
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L2650:
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
	ldr	r1, .L2660
	mov	r4, r0
	bl	mvc_ue_v
	add	r6, r4, #11075584
	add	r5, r6, #45056
	ldr	r2, .L2660+4
	mov	r1, #1
	str	r0, [r5, #2160]
	mov	r0, r4
	bl	mvc_u_v
	uxtb	r0, r0
	strb	r0, [r5, #2137]
	cmp	r0, #0
	beq	.L2659
.L2654:
	mov	r0, r4
	ldr	r2, .L2660+8
	mov	r1, #1
	add	r6, r6, #45056
	bl	mvc_u_v
	mov	r3, #1
	strb	r3, [r6, #2136]
	strb	r0, [r6, #2138]
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2659:
	ldr	r2, .L2660+12
	mov	r1, #7
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+16
	mov	r1, #1
	str	r0, [r5, #2152]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+20
	mov	r1, #6
	strb	r0, [r5, #2139]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+24
	mov	r1, #1
	str	r0, [r5, #2156]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+28
	mov	r1, #1
	strb	r0, [r5, #2140]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+32
	mov	r1, #1
	strb	r0, [r5, #2141]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+36
	mov	r1, #1
	strb	r0, [r5, #2142]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+40
	mov	r1, #1
	strb	r0, [r5, #2143]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+44
	mov	r1, #1
	strb	r0, [r5, #2144]
	mov	r0, r4
	bl	mvc_u_v
	ldrb	r3, [r5, #2139]	@ zero_extendqisi2
	cmp	r3, #0
	strb	r0, [r5, #2145]
	bne	.L2655
	ldr	r3, [r5, #2152]
	cmp	r3, #5
	beq	.L2655
	ldr	r2, .L2660+48
	mov	r1, #1
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+52
	mov	r1, #1
	strb	r0, [r5, #2146]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+56
	mov	r1, #1
	strb	r0, [r5, #2147]
	mov	r0, r4
	bl	mvc_u_v
	ldr	r2, .L2660+60
	mov	r1, #1
	strb	r0, [r5, #2148]
	mov	r0, r4
	bl	mvc_u_v
	strb	r0, [r5, #2149]
.L2655:
	ldr	r2, .L2660+64
	mov	r1, #8
	mov	r0, r4
	bl	mvc_u_v
	ldr	r1, .L2660+68
	str	r0, [r5, #2164]
	mov	r0, r4
	bl	mvc_ue_v
	str	r0, [r5, #2168]
	b	.L2654
.L2661:
	.align	2
.L2660:
	.word	.LC338
	.word	.LC339
	.word	.LC355
	.word	.LC340
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
	ldr	r2, [r2, #2088]
	mla	r4, r4, r2, r3
	ldrb	r2, [r4, #25]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2663
	ldr	ip, [r0, #36]
	cmp	ip, #0
	ble	.L2664
	ldrb	r2, [r3, #25]	@ zero_extendqisi2
	cmp	r2, #0
	addeq	r3, r3, #3984
	addeq	r3, r3, #8
	beq	.L2667
	b	.L2686
.L2668:
	ldrb	r1, [r3, #-3967]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L2665
.L2667:
	add	r2, r2, #1
	mov	r4, r3
	cmp	r2, ip
	add	r3, r3, #3984
	add	r3, r3, #8
	bne	.L2668
.L2664:
	ldr	r3, .L2688
	mov	r0, #1
	ldr	r1, .L2688+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r3, #0
.L2669:
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2686:
	mov	r4, r3
.L2665:
	cmp	r4, #0
	beq	.L2664
.L2663:
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2670
	ldrb	r3, [r4, #35]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2687
	ldr	r1, [r4, #388]
	ldr	r8, [r4, #392]
	add	r1, r1, #1
	add	r8, r8, #1
.L2673:
	ldr	r2, .L2688+8
	mov	r0, r5
	bl	mvc_u_v
	add	r7, r6, #45056
	mov	r1, r8
	ldr	r2, .L2688+12
	str	r0, [r7, #2176]
	mov	r0, r5
	bl	mvc_u_v
	str	r0, [r7, #2180]
.L2670:
	ldrb	r0, [r4, #39]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	r3, r0
	beq	.L2669
	mov	r0, r5
	ldr	r2, .L2688+16
	mov	r1, #4
	add	r6, r6, #45056
	bl	mvc_u_v
	mov	r3, #0
	strb	r0, [r6, #2172]
	mov	r0, r3
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2687:
	ldrb	r3, [r4, #38]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2670
	ldr	r1, [r4, #700]
	ldr	r8, [r4, #704]
	add	r1, r1, #1
	add	r8, r8, #1
	b	.L2673
.L2689:
	.align	2
.L2688:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC356
	.word	.LC357
	.word	.LC358
	.word	.LC359
	UNWIND(.fnend)
	.size	MVC_DecPicTimingSEI, .-MVC_DecPicTimingSEI
	.align	2
	.global	MVC_DecSEI
	.type	MVC_DecSEI, %function
MVC_DecSEI:
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
	ldr	r2, [r0, #232]
	sub	r7, fp, #44
	mov	r3, #0
	mov	r1, #32
	mov	r4, r0
	add	r6, r0, #548
	mov	r8, r3
	strb	r3, [r7, #-1]!
	str	r1, [r2, #24]
	b	.L2693
.L2875:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2692
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2874
.L2693:
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
	ble	.L2875
.L2692:
	ldr	r3, .L2906
	mov	r0, #1
	ldr	r1, .L2906+4
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L2695
.L2874:
	ldr	r10, .L2906
	mov	r3, #0
	str	r3, [fp, #-60]
.L2829:
	mov	r5, #0
	b	.L2699
.L2877:
	bl	BsSkip
	cmp	r9, #0
	ble	.L2697
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2876
.L2699:
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
	ble	.L2877
.L2697:
	ldr	r3, .L2906
	mov	r0, #1
	ldr	r1, .L2906+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2876:
	cmp	r5, #409600
	bgt	.L2697
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
	b	.L2702
.L2704:
	.word	.L2703
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
	.word	.L2722
	.word	.L2723
	.word	.L2724
	.word	.L2725
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2702
	.word	.L2726
.L2726:
	mov	r1, r5
	mov	r0, r4
	ldr	r8, [r4, #572]
	bl	MVC_DecFramePackingSEI
	ldr	r0, [r4, #572]
	mov	r9, r5, asl #3
	rsb	r3, r8, r0
	cmp	r3, r9
	bge	.L2815
	rsb	r3, r3, r9
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r8, r3, #7
	movge	r8, r3
	ldr	r3, [r4, #564]
	mov	r8, r8, asr #3
	cmp	r8, #0
	movle	r3, r3, asl #3
	ble	.L2817
	add	r2, r0, #8
	mov	r3, r3, asl #3
	cmp	r2, r3
	bgt	.L2817
	mov	r9, #0
	b	.L2818
.L2819:
	add	r2, r0, #8
	cmp	r2, r3
	bgt	.L2817
.L2818:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r9, r9, #1
	ldr	r3, [r4, #564]
	cmp	r9, r8
	ldr	r0, [r4, #572]
	mov	r3, r3, asl #3
	bne	.L2819
.L2817:
	ldr	r2, [fp, #-56]
	mov	r1, r2, asr #31
	mov	r1, r1, lsr #29
	add	r2, r2, r1
	and	r2, r2, #7
	rsb	r1, r1, r2
	add	r2, r1, r0
	cmp	r2, r3
	ble	.L2878
.L2815:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2879
.L2728:
	ldr	r3, [r4, #60]
	cmp	r3, #3
	bhi	.L2880
.L2824:
	mov	r8, #0
	b	.L2828
.L2826:
	bl	BsSkip
	cmp	r5, #0
	ble	.L2827
	ldrb	r3, [fp, #-45]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2881
.L2828:
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
	ble	.L2826
.L2827:
	mov	r0, #0
.L2695:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2725:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+12
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2882
.L2811:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2812
.L2813:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2812:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2813
	ldr	r3, [r4, #60]
	cmp	r3, #3
	bls	.L2824
.L2880:
	ldr	r3, .L2906
	mov	r0, #1
	ldr	r1, .L2906+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2724:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+20
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2883
.L2808:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2809
.L2810:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2809:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2810
	b	.L2728
.L2723:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+24
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2884
.L2805:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2806
.L2807:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2806:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2807
	b	.L2728
.L2721:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+28
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2885
.L2799:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2800
.L2801:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2800:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2801
	b	.L2728
.L2722:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+32
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2886
.L2802:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2803
.L2804:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2803:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2804
	b	.L2728
.L2720:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+36
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2887
.L2796:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2797
.L2798:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2797:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2798
	b	.L2728
.L2719:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+40
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2888
.L2793:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2794
.L2795:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2794:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2795
	b	.L2728
.L2711:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+44
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2889
.L2769:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2770
.L2771:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2770:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2771
	b	.L2728
.L2715:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+48
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2890
.L2781:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2782
.L2783:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2782:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2783
	b	.L2728
.L2706:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+52
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2891
.L2738:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2739
.L2740:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2739:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2740
	b	.L2728
.L2717:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+56
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2892
.L2787:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2788
.L2789:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2788:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2789
	b	.L2728
.L2709:
	ldr	r3, [fp, #-60]
	cmp	r3, #0
	bne	.L2708
	ldr	lr, .L2906
	mov	r3, r5
	ldr	ip, [r10, #68]
	mov	r0, #20
	ldr	r1, .L2906+60
	str	lr, [fp, #-64]
	blx	ip
.L2745:
	ldr	r0, [r4, #120]
	ldr	r8, [r4, #60]
	bl	GetUsd
	add	r8, r8, #132
	str	r0, [r4, r8, asl #2]
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	r1, [r4, r3, asl #2]
	cmp	r1, #0
	beq	.L2832
	ldr	r2, [r4, #232]
	cmp	r5, #1024
	movlt	r3, r5
	movge	r3, #1024
	ldrb	ip, [r2]	@ zero_extendqisi2
	ldr	lr, [r2, #68]
	str	r3, [fp, #-56]
	cmp	ip, lr
	bcs	.L2833
	sxth	r3, ip
	mov	r8, #0
	mov	r0, r3, asl #5
	sub	r0, r0, r3, asl #2
	add	r0, r2, r0
.L2747:
	ldr	r3, [r0, #24]
	add	ip, ip, #1
	ldr	r2, [r0, #12]
	cmp	ip, lr
	add	r3, r3, #7
	add	r0, r0, #28
	sub	r3, r2, r3, lsr #3
	add	r8, r8, r3
	bne	.L2747
	mov	r2, r8
.L2746:
	ldr	r0, [fp, #-56]
	ldrsb	r3, [fp, #-60]
	cmp	r0, r2
	movle	r8, r0
	cmp	r3, #1
	movne	r3, #0
	movne	r9, r3
	beq	.L2893
.L2749:
	rsb	r2, r3, r8
	mov	r0, r4
	bl	MVC_GetBytes
	subs	r8, r0, #0
	ble	.L2894
	ldr	r3, [fp, #-56]
	rsb	r3, r9, r3
	cmp	r8, r3
	ldr	r3, [r4, #60]
	blt	.L2895
	add	r3, r3, #132
	ldr	r2, [fp, #-56]
	rsb	r9, r9, r5
	ldr	r3, [r4, r3, asl #2]
	cmp	r2, r5
	str	r9, [r3, #1048]
	bcc	.L2896
.L2760:
	cmp	r9, #0
	ble	.L2755
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2755
	mov	r5, #0
	b	.L2762
.L2763:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2755
.L2762:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r9
	bne	.L2763
.L2755:
	ldr	r3, [r4, #60]
	mov	r2, #8
	ldr	r1, [r4, #224]
	add	r3, r3, #132
	ldr	ip, .L2906+64
	ldr	r3, [r4, r3, asl #2]
	ldrd	r0, [r1, #48]
	add	r3, r3, #1056
	ldr	ip, [ip]
	strd	r0, [r3]
	cmp	ip, #0
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	r3, [r4, r3, asl #2]
	str	r2, [r3, #1036]
	ldr	r3, [r4, #60]
	ldr	r2, [r4, #84]
	add	r3, r3, #132
	ldr	r3, [r4, r3, asl #2]
	str	r2, [r3, #1040]
	beq	.L2728
	ldr	r2, [r4, #60]
	mov	r3, #1072
	mov	r1, #6
	ldr	r0, [r4, #120]
	add	r2, r2, #132
	ldr	r2, [r4, r2, asl #2]
	blx	ip
	b	.L2728
.L2713:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+68
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2897
.L2775:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2776
.L2777:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2776:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2777
	b	.L2728
.L2712:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+72
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2898
.L2772:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2773
.L2774:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2773:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2774
	b	.L2728
.L2714:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+76
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2899
.L2778:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2779
.L2780:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2779:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2780
	b	.L2728
.L2703:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+80
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2900
.L2727:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2729
.L2730:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2729:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2730
	b	.L2728
.L2718:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+84
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2901
.L2790:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2791
.L2792:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2791:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2792
	b	.L2728
.L2710:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+88
	mov	r0, #20
	blx	ip
	cmp	r5, #0
	ble	.L2766
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2766
	mov	r8, #0
	b	.L2767
.L2768:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2766
.L2767:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2768
.L2766:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2728
	ldr	ip, [r10, #68]
	mov	r3, #12416
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2728
.L2716:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+92
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2902
.L2784:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2785
.L2786:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2785:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2786
	b	.L2728
.L2705:
	mov	r3, r5
	ldr	ip, [r10, #68]
	ldr	r1, .L2906+96
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	ldr	r8, [r4, #572]
	bl	MVC_DecPicTimingSEI
	ldr	r2, [r4, #572]
	mov	r9, r5, asl #3
	rsb	r3, r8, r2
	cmp	r3, r9
	bge	.L2732
	rsb	r3, r3, r9
	str	r3, [fp, #-56]
	cmp	r3, #0
	add	r8, r3, #7
	movge	r8, r3
	ldr	r3, [r4, #564]
	mov	r8, r8, asr #3
	cmp	r8, #0
	movle	r3, r3, asl #3
	ble	.L2734
	add	r1, r2, #8
	mov	r3, r3, asl #3
	cmp	r1, r3
	bgt	.L2734
	mov	r9, #0
	b	.L2735
.L2736:
	add	r1, r2, #8
	cmp	r1, r3
	bgt	.L2734
.L2735:
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	add	r9, r9, #1
	ldr	r3, [r4, #564]
	cmp	r9, r8
	ldr	r2, [r4, #572]
	mov	r3, r3, asl #3
	bne	.L2736
.L2734:
	ldr	ip, [fp, #-56]
	mov	r1, ip, asr #31
	mov	r0, r1, lsr #29
	add	r1, ip, r0
	and	r1, r1, #7
	rsb	r1, r0, r1
	add	r2, r2, r1
	cmp	r3, r2
	bge	.L2903
.L2732:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2728
	ldr	ip, [r10, #68]
	movw	r3, #12190
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2728
.L2707:
	ldr	ip, [r10, #68]
	mov	r3, r5
	ldr	r1, .L2906+100
	mov	r0, #20
	blx	ip
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2904
.L2741:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2742
.L2743:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2742:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2743
	b	.L2728
.L2702:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	blt	.L2905
.L2821:
	cmp	r5, #0
	ble	.L2728
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
	mov	r8, #0
	b	.L2822
.L2823:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2728
.L2822:
	add	r8, r8, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2823
	b	.L2728
.L2881:
	cmp	r3, #128
	bne	.L2829
	b	.L2827
.L2708:
	ldr	lr, .L2906
	mov	r3, r5
	ldr	ip, [r10, #68]
	mov	r0, #20
	ldr	r1, .L2906+104
	str	lr, [fp, #-64]
	blx	ip
	mov	r3, #1
	str	r3, [fp, #-60]
	b	.L2745
.L2895:
	add	r3, r3, #132
	ldr	r3, [r4, r3, asl #2]
	str	r8, [r3, #1048]
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2755
	mov	r5, #0
	b	.L2756
.L2907:
	.align	2
.L2906:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC360
	.word	.LC361
	.word	.LC388
	.word	.LC389
	.word	.LC387
	.word	.LC386
	.word	.LC384
	.word	.LC385
	.word	.LC383
	.word	.LC382
	.word	.LC374
	.word	.LC378
	.word	.LC365
	.word	.LC380
	.word	.LC368
	.word	g_event_report
	.word	.LC376
	.word	.LC375
	.word	.LC377
	.word	.LC362
	.word	.LC381
	.word	.LC373
	.word	.LC379
	.word	.LC364
	.word	.LC366
	.word	.LC367
	.word	.LC369
	.word	.LC371
	.word	.LANCHOR0+236
	.word	.LC363
	.word	.LC370
	.word	.LC372
.L2758:
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #564]
	add	r3, r3, #8
	cmp	r3, r2, asl #3
	bgt	.L2755
.L2756:
	add	r5, r5, #1
	mov	r1, #8
	mov	r0, r6
	bl	BsSkip
	cmp	r5, r8
	bne	.L2758
	b	.L2755
.L2893:
	strb	r3, [r1, #1064]
	mov	r0, r4
	ldr	r3, [r4, #60]
	mov	r1, #8
	ldr	r2, .L2906+108
	add	r3, r3, #132
	ldr	r9, [r4, r3, asl #2]
	bl	mvc_u_v
	strb	r0, [r9, #1065]
	ldr	r3, [r4, #60]
	add	r3, r3, #132
	ldr	ip, [r4, r3, asl #2]
	ldrb	r3, [ip, #1065]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L2750
	mov	r3, #3
	mov	r2, #0
	mov	r9, r3
	strb	r2, [ip, #1066]
.L2751:
	ldr	ip, [r4, #60]
	mov	r1, #16
	ldr	r2, .L2906+112
	mov	r0, r4
	add	ip, ip, #132
	str	r3, [fp, #-68]
	ldr	ip, [r4, ip, asl #2]
	str	ip, [fp, #-72]
	bl	mvc_u_v
	ldr	ip, [fp, #-72]
	ldr	r3, [fp, #-68]
	add	ip, ip, #1056
	mov	r1, r3
	strh	r0, [ip, #12]	@ movhi
	mov	r0, r4
	bl	MVC_PassBytes
	ldr	r3, [fp, #-68]
	cmp	r8, r3
	beq	.L2755
	ldr	r2, [r4, #60]
	add	r2, r2, #132
	ldr	r1, [r4, r2, asl #2]
	b	.L2749
.L2888:
	ldr	ip, [r10, #68]
	movw	r3, #12636
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2793
.L2889:
	ldr	ip, [r10, #68]
	movw	r3, #12428
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2769
.L2890:
	ldr	ip, [r10, #68]
	movw	r3, #12532
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2781
.L2891:
	ldr	ip, [r10, #68]
	movw	r3, #12202
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2738
.L2892:
	ldr	ip, [r10, #68]
	movw	r3, #12584
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2787
.L2884:
	ldr	ip, [r10, #68]
	movw	r3, #12740
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2805
.L2885:
	ldr	ip, [r10, #68]
	movw	r3, #12688
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2799
.L2886:
	ldr	ip, [r10, #68]
	movw	r3, #12714
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2802
.L2887:
	ldr	ip, [r10, #68]
	movw	r3, #12662
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2796
.L2882:
	ldr	ip, [r10, #68]
	movw	r3, #12792
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2811
.L2883:
	ldr	ip, [r10, #68]
	movw	r3, #12766
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2808
.L2898:
	ldr	ip, [r10, #68]
	movw	r3, #12454
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2772
.L2899:
	ldr	ip, [r10, #68]
	movw	r3, #12506
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2778
.L2900:
	ldr	ip, [r10, #68]
	movw	r3, #12137
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2727
.L2901:
	ldr	ip, [r10, #68]
	movw	r3, #12610
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2790
.L2897:
	ldr	ip, [r10, #68]
	mov	r3, #12480
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2775
.L2902:
	ldr	ip, [r10, #68]
	movw	r3, #12558
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2784
.L2904:
	ldr	ip, [r10, #68]
	movw	r3, #12228
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2741
.L2879:
	ldr	ip, [r10, #68]
	movw	r3, #12844
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2728
.L2905:
	ldr	ip, [r10, #68]
	movw	r3, #12856
	ldr	r2, .L2906+116
	mov	r0, #20
	ldr	r1, .L2906+120
	blx	ip
	b	.L2821
.L2903:
	mov	r0, r6
	bl	BsSkip
	b	.L2732
.L2878:
	mov	r0, r6
	bl	BsSkip
	b	.L2815
.L2896:
	rsb	r1, r2, r5
	mov	r0, r4
	bl	MVC_PassBytes
	cmp	r0, #0
	bge	.L2760
	ldr	r3, [fp, #-64]
	mov	r0, #20
	ldr	r2, .L2906+116
	ldr	r1, .L2906+120
	ldr	ip, [r3, #68]
	movw	r3, #12364
	blx	ip
	b	.L2760
.L2833:
	mov	r2, #0
	mov	r8, r2
	b	.L2746
.L2750:
	ldr	r2, .L2906+124
	mov	r1, #8
	mov	r0, r4
	mov	r3, #4
	str	ip, [fp, #-72]
	str	r3, [fp, #-68]
	bl	mvc_u_v
	ldr	r3, [fp, #-68]
	ldr	ip, [fp, #-72]
	mov	r9, r3
	strb	r0, [ip, #1066]
	b	.L2751
.L2832:
	mvn	r0, #0
	b	.L2695
.L2894:
	ldr	r3, [fp, #-64]
	mov	r0, #1
	ldr	r1, .L2906+128
	ldr	r3, [r3, #68]
	blx	r3
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
	b	.L2695
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
	str	r1, [r3, #2188]
	mov	r1, #7
	str	r2, [r3, #2192]
	mov	r2, #32
	strb	r1, [r3, #2179]
	mov	r1, #3
	strb	r2, [r3, #2178]
	mvn	r2, #0
	str	r1, [r3, #2200]
	mov	r1, #262144
	str	r2, [r3, #2204]
	mov	r2, #0
	strb	r0, [r3, #2177]
	str	r1, [r3, #2216]
	strb	r2, [r3, #2176]
	str	r2, [r3, #2208]
	str	r2, [r3, #2212]
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
	b	.L2917
.L2912:
	.word	.L2911
	.word	.L2917
	.word	.L2917
	.word	.L2917
	.word	.L2911
	.word	.L2917
	.word	.L2913
	.word	.L2913
.L2913:
	ldr	r3, [r0]
	mov	r2, #1
	str	r2, [r0]
	adds	r3, r3, #0
	movne	r3, #1
	rsb	r0, r3, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2911:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L2914
.L2916:
	mvn	r0, #0
.L2915:
	mov	r2, #0
	str	r2, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2917:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2914:
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
	bhi	.L2916
	mov	r3, r0, asl #1
	rsb	r3, r3, #31
	mov	r3, r4, lsr r3
	subs	r0, r3, #1
	mvnne	r0, #0
	b	.L2915
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
	ble	.L2944
	ldr	r3, [fp, #-48]
	subs	r7, r3, #0
	add	r8, r3, #63
	movge	r8, r3
	ands	r3, r3, #63
	movne	r3, #1
	add	r8, r3, r8, asr #6
	cmp	r8, #0
	ble	.L2932
	ldr	r5, .L2945
	ldr	r10, .L2945+4
.L2930:
	cmp	r7, #64
	ldr	r3, [r10, #52]
	ldr	r0, .L2945
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
	bne	.L2922
	ldr	r2, .L2945
	add	r4, r4, #1
	cmp	r3, r2
	bne	.L2925
	b	.L2923
.L2926:
	cmp	r3, r5
	add	r4, r4, #1
	beq	.L2923
.L2925:
	ldrb	r2, [r3, #-1]!	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2926
.L2922:
	mov	r0, r4, asl #3
.L2921:
	ldr	r3, [fp, #-48]
	cmp	r4, r3
	bge	.L2934
	sub	r3, r3, #1
	rsb	r4, r4, r3
	ldrb	r2, [r9, r4]	@ zero_extendqisi2
	tst	r2, #1
	moveq	r2, r2, lsr #1
	moveq	r3, #1
	bne	.L2934
.L2929:
	tst	r2, #1
	mov	r2, r2, lsr #1
	bne	.L2927
	add	r3, r3, #1
	cmp	r3, #8
	bne	.L2929
	b	.L2927
.L2923:
	subs	r8, r8, #1
	sub	r7, r7, #64
	bne	.L2930
	b	.L2922
.L2934:
	mov	r3, #0
.L2927:
	add	r0, r3, r0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2944:
	ldr	r3, .L2945+4
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L2945+8
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2932:
	mov	r0, r4
	b	.L2921
.L2946:
	.align	2
.L2945:
	.word	.LANCHOR3-1952
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC390
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
	beq	.L2953
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2952
	ldr	r1, [r3, #40]
	ldr	r0, [r3, #36]
	bl	MVC_FindZeroBitsInSeg
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #40]
	cmp	r0, r2, asl #3
	bcs	.L2956
	cmn	r0, #1
	beq	.L2952
.L2951:
	add	r0, r0, #1
	str	r0, [r3, #72]
	ldr	r3, [r4, #232]
	ldr	r2, [r3, #68]
	cmp	r2, #1
	bls	.L2955
	ldr	r1, [r3, #40]
	ldr	r2, [r3, #72]
	cmp	r2, r1, asl #3
	bcs	.L2957
.L2955:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L2957:
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
.L2956:
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
.L2952:
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	bl	MVC_FindZeroBitsInSeg
	ldr	r3, [r4, #232]
	b	.L2951
.L2953:
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
	b	MVC_CombinePacket.part.10
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
	beq	.L2965
	mov	r2, r0
	mov	r3, #1
	b	.L2963
.L2962:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L2968
.L2963:
	ldrb	r1, [r2, #1025]	@ zero_extendqisi2
	add	r2, r2, #88
	cmp	r1, #0
	bne	.L2962
	mov	r2, r3
.L2961:
	mov	ip, #88
	mov	r1, #1
	mla	r3, ip, r3, r0
	mov	r0, r2
	strb	r1, [r3, #937]
	ldmfd	sp, {fp, sp, pc}
.L2968:
	mvn	r2, #0
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L2965:
	mov	r3, r2
	b	.L2961
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
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r10, .L3012
	mov	r4, r0
	ldr	r3, [r0, #68]
	mov	r9, r1
	mov	r7, r2
	ldr	r1, .L3012+4
	sub	r2, r3, #1
	mov	r0, #22
	ldr	r3, [r10, #68]
	add	r6, r4, #11075584
	blx	r3
	mov	r3, #0
	str	r3, [r7]
	add	r5, r6, #40960
	str	r3, [r9]
	add	r1, r4, #12288
	ldr	r2, [r4, #232]
	mvn	ip, #0
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
	strb	r0, [r5, #523]
	ldrb	lr, [r2, #4]	@ zero_extendqisi2
	strb	lr, [r5, #528]
	ldrb	r2, [r2, #5]	@ zero_extendqisi2
	strb	r3, [r5, #531]
	str	ip, [r5, #2160]
	strb	r2, [r5, #532]
	ldrb	r3, [r1, #704]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3005
	cmp	r2, #255
	beq	.L3006
.L2971:
	mov	r0, r4
	bl	MVC_SliceCheck
	subs	r8, r0, #0
	bne	.L2987
	mov	r0, r4
	bl	MVC_ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L3007
	ldrb	r3, [r5, #525]	@ zero_extendqisi2
	mov	r0, #2240
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	str	r3, [r9]
	ldrb	r1, [r5, #532]	@ zero_extendqisi2
	ldr	r3, [r5, #536]
	ldr	ip, [r4, #252]
	sxtb	r2, r1
	cmn	r2, #1
	mla	r3, r0, r3, ip
	beq	.L3008
	cmp	r1, #0
	bne	.L2983
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	ldr	r3, [r3, #28]
	cmp	r2, #1
	beq	.L3009
	ldr	r2, [r4, #28]
	cmp	r2, r3
	movweq	r3, #35364
	movteq	r3, 168
	addeq	r3, r4, r3
	bne	.L3010
.L2982:
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
	ldr	r3, [r6, #2376]
	add	r3, r3, #1
	cmp	r2, r3
	movhi	r3, #1
	strhi	r3, [r7]
.L2979:
	mov	r0, r8
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3005:
	strb	r3, [r5, #531]
	ldr	r3, [r1, #712]
	str	r3, [r5, #2160]
	ldrb	r3, [r1, #708]	@ zero_extendqisi2
	strb	r3, [r5, #529]
	ldrb	r3, [r1, #709]	@ zero_extendqisi2
	strb	r3, [r5, #530]
	b	.L2971
.L3008:
	ldr	r3, [r3, #28]
	movw	r1, #3992
	ldr	r2, [r4, #248]
	mla	r3, r1, r3, r2
	b	.L2982
.L3006:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq	.L2971
	add	r3, r4, #10747904
	add	r3, r3, #20480
	ldr	r2, [r3, #2384]
	cmp	r2, #0
	bne	.L2974
	ldrb	r2, [r3, #2380]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2974
	add	r3, r4, #12992
	add	r3, r3, #16
.L2977:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L2975
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3011
.L2975:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L2977
	mvn	r3, #0
	b	.L3004
.L3009:
	movw	r2, #8500
	movt	r2, 5
	mla	r3, r2, r3, r4
	add	r3, r3, #286720
	add	r3, r3, #932
	b	.L2982
.L2974:
	ldr	r3, [r3, #2388]
.L3004:
	cmn	r3, #1
	str	r3, [r5, #2160]
	beq	.L2971
	sub	r0, r0, #5
	mov	r3, #1
	clz	r0, r0
	strb	r3, [r5, #531]
	strb	r3, [r5, #530]
	mov	r0, r0, lsr #5
	strb	r0, [r5, #529]
	b	.L2971
.L2987:
	mvn	r8, #0
	b	.L2979
.L3010:
	ldr	r3, [r10, #68]
	mov	r0, #1
	ldr	r1, .L3012+8
	mvn	r8, #0
	blx	r3
	b	.L2979
.L3007:
	ldr	r3, [r10, #68]
	mov	r0, #1
	ldr	r1, .L3012+12
	mvn	r8, #0
	blx	r3
	b	.L2979
.L2983:
	ldr	r3, [r10, #68]
	mov	r0, #1
	ldr	r1, .L3012+16
	mvn	r8, #0
	blx	r3
	b	.L2979
.L3011:
	movw	r3, #8500
	movt	r3, 5
	mla	r3, r3, r2, r4
	add	r3, r3, #12992
	add	r3, r3, #16
	ldr	r3, [r3, #4]
	b	.L3004
.L3013:
	.align	2
.L3012:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC391
	.word	.LC30
	.word	.LC392
	.word	.LC393
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
	ldr	r3, [r5, #568]
	cmp	r3, #0
	beq	.L3016
	add	r6, r0, #252
	mov	r4, #0
	b	.L3020
.L3017:
	ldr	r3, [r0, #520]
	cmp	r3, #0
	beq	.L3019
	ldr	r3, [r5, #568]
	cmp	r4, r3
	bcs	.L3042
.L3020:
	ldr	r3, [r6, #4]!
	add	r4, r4, #1
	ldr	r0, [r7, #120]
	ldr	r3, [r3, #4]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L3017
.L3019:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3042:
	cmp	r3, #0
	beq	.L3016
	ldr	r3, [r5, #572]
	cmp	r3, #0
	addne	r6, r7, #384
	movne	r4, #0
	bne	.L3023
	b	.L3016
.L3043:
	ldr	r3, [r0, #520]
	cmp	r3, #0
	beq	.L3019
	ldr	r3, [r5, #572]
	cmp	r4, r3
	bcs	.L3016
.L3023:
	ldr	r3, [r6, #4]!
	add	r4, r4, #1
	ldr	r0, [r7, #120]
	ldr	r3, [r3, #4]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	bne	.L3043
	b	.L3019
.L3016:
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
	mov	r6, r0
	bl	MVC_ClearCurrPic
	mov	r0, r6
	bl	MVC_ClearAllNal
	ldr	r3, [r6, #60]
	cmp	r3, #0
	beq	.L3045
	mov	r4, #0
	add	r5, r6, #524
	mov	r7, r4
.L3047:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3046
	ldr	r0, [r6, #120]
	bl	FreeUsdByDec
	str	r7, [r5]
.L3046:
	ldr	r3, [r6, #60]
	cmp	r3, r4
	bhi	.L3047
.L3045:
	movw	r5, #48096
	movw	r8, #10080
	movt	r5, 169
	movt	r8, 170
	add	r5, r6, r5
	add	r8, r6, r8
	mov	r7, #0
.L3051:
	sub	r4, r5, #16
.L3049:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L3048
	ldr	r0, [r6, #120]
	bl	FreeUsdByDec
	str	r7, [r4]
.L3048:
	cmp	r4, r5
	bne	.L3049
	add	r5, r4, #688
	cmp	r5, r8
	bne	.L3051
	ldr	r3, .L3063
	mov	r0, #2
	ldr	r1, .L3063+4
	ldr	r3, [r3, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L3064:
	.align	2
.L3063:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC394
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
	ldr	r3, .L3075
	mov	r6, r0
	mov	r8, r1
	mov	r0, #2
	ldr	r1, .L3075+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r1, r8
	ldr	r0, [r6, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L3070
	ldrsb	r3, [r0, #1]
	cmp	r3, #0
	beq	.L3070
	add	r4, r0, #216
	add	r5, r0, #232
	mov	r7, #0
.L3068:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L3067
	ldr	r0, [r6, #120]
	bl	FreeUsdByDec
	str	r7, [r4]
.L3067:
	cmp	r4, r5
	bne	.L3068
	ldr	r0, [r6, #120]
	mov	r1, r8
	mov	r2, #0
	bl	FSP_SetDisplay
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3070:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3076:
	.align	2
.L3075:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC395
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
	bne	.L3111
	ldr	r3, [r0, #224]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	ble	.L3080
	ldrb	r3, [r0, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3080
	ldr	r3, [r0, #520]
	cmp	r3, #0
	strneb	ip, [r3, #2]
	movne	r0, #1
	strne	ip, [r4, #520]
	beq	.L3082
.L3110:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3080:
	mov	r1, r6
	mov	r0, r4
	bl	MVC_CheckFrameStore
	cmn	r0, #3
	beq	.L3082
	ldr	r1, [r4, #520]
	cmp	r1, #0
	beq	.L3084
	mov	r0, r4
	bl	MVC_GetImagePara
	ldr	r1, [r4, #520]
	mov	r0, r4
	bl	MVC_CheckFrameStore
	subs	r8, r0, #0
	beq	.L3112
	cmn	r8, #3
	beq	.L3092
	ldr	ip, .L3120
	mov	r3, r8
	movw	r2, #1919
	ldr	r1, .L3120+4
	mov	r0, #1
	sub	r8, r8, #1
	ldr	ip, [ip, #68]
	clz	r8, r8
	blx	ip
	ldr	r3, [r4, #520]
	mov	r0, r4
	mov	r8, r8, lsr #5
	ldr	r1, [r3, #268]
	bl	MVC_DEC_RecycleImage
.L3091:
	ldr	r3, [r4, #520]
	tst	r8, r5
	mov	r0, #0
	strb	r0, [r3, #2]
	str	r0, [r4, #520]
	bne	.L3110
.L3084:
	mov	r1, r6
	mov	r0, r4
	bl	MVC_GetImagePara
	mov	r1, r6
	mov	r0, r4
	bl	MVC_CheckFrameStore
	subs	r3, r0, #0
	beq	.L3113
	ldr	ip, .L3120
	mov	r2, #1968
	ldr	r1, .L3120+4
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r1, [r6, #268]
	mov	r0, r4
	bl	MVC_DEC_RecycleImage
	mov	r0, #2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3111:
	ldr	ip, .L3120
	mov	r3, r1
	mov	r2, r0
	ldr	r1, .L3120+8
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L3082:
	mov	r0, #1
	b	.L3110
.L3113:
	mov	r2, #1
	ldrsb	r1, [r6, #6]
	ldr	r0, [r4, #120]
	bl	FSP_SetDisplay
	ldrsb	r1, [r6, #6]
	ldr	r0, [r4, #120]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L3114
	cmp	r7, #1
	add	r3, r4, #584
	streq	r7, [r5, #244]
	mov	r2, r4
	ldr	r0, [r4, #120]
	mov	r1, #16
	str	r5, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L3115
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r5, #84]
	bcs	.L3116
.L3099:
	ldr	r2, [r4, #144]
	mov	r0, #0
	ldr	r3, [r4, #136]
	add	r2, r2, #1
	str	r2, [r4, #144]
	add	r3, r3, #2
	str	r3, [r4, #136]
	b	.L3110
.L3115:
	ldr	r3, .L3120
	mov	r2, r0
	ldr	r1, .L3120+12
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	ldrsb	r1, [r6, #6]
	mov	r2, #0
	ldr	r0, [r4, #120]
	bl	FSP_SetDisplay
	mov	r0, r4
	mov	r1, #1
	bl	MVC_ClearAll
	mvn	r0, #0
	b	.L3110
.L3092:
	ldr	r2, [r4, #520]
	mov	r3, #0
	strb	r3, [r2, #2]
	str	r3, [r4, #520]
	b	.L3084
.L3112:
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
	beq	.L3117
	ldr	r3, [r4, #520]
	mov	r2, r4
	mov	r1, #16
	cmp	r6, r3
	cmpeq	r7, #1
	moveq	r3, #1
	streq	r3, [r8, #244]
	add	r3, r4, #584
	ldr	r0, [r4, #120]
	str	r8, [sp]
	bl	InsertImgToVoQueue
	cmp	r0, #1
	bne	.L3118
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r8, #84]
	bcs	.L3119
.L3090:
	ldr	r2, [r4, #144]
	mov	r8, #1
	ldr	r3, [r4, #136]
	add	r2, r2, r8
	str	r2, [r4, #144]
	add	r3, r3, #2
	str	r3, [r4, #136]
	b	.L3091
.L3116:
	mov	r1, r5
	mov	r0, r4
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3099
.L3119:
	mov	r1, r8
	mov	r0, r4
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3090
.L3118:
	ldr	r3, .L3120
	mov	r2, r0
	ldr	r1, .L3120+12
	mov	r0, #0
	mov	r5, r0
	ldr	r3, [r3, #68]
	blx	r3
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
	b	.L3110
.L3114:
	ldr	r3, .L3120
	movw	r2, #1942
	ldr	r1, .L3120+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3110
.L3117:
	ldr	r3, .L3120
	movw	r2, #1890
	ldr	r1, .L3120+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3110
.L3121:
	.align	2
.L3120:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC399
	.word	.LC396
	.word	.LC398
	.word	.LC397
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
	ldr	r3, [r4, #2184]
	cmp	r3, #0
	moveq	r4, r3
	beq	.L3123
	mov	r2, #0
	strb	r2, [r3, #5]
	ldr	r1, [r4, #2184]
	bl	MVC_OutputFrmToVO
	ldr	r3, [r4, #2184]
	ldrb	r2, [r3, #3]	@ zero_extendqisi2
	cmn	r0, #1
	movne	r0, #0
	moveq	r0, #1
	cmp	r2, #0
	rsb	r4, r0, #0
	beq	.L3125
.L3123:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3125:
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
	beq	.L3145
	add	r8, r5, #11075584
	add	r8, r8, #45056
	ldr	r3, [r8, #2376]
	cmp	r3, #0
	movwne	r7, #47236
	movne	r6, #0
	movtne	r7, 169
	addne	r7, r5, r7
	beq	.L3133
.L3132:
	ldr	r1, [r7, #4]!
	add	r6, r6, #1
	cmp	r1, #0
	beq	.L3131
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3131
	ldr	r3, [r1, #56]
	cmn	r4, #1
	cmpne	r3, r4
	bne	.L3131
	mov	r0, r5
	bl	MVC_UnMarkFrameStoreRef
.L3131:
	ldr	r3, [r8, #2376]
	cmp	r3, r6
	bhi	.L3132
.L3133:
	mov	r1, r4
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
	ldr	r0, [r8, #2376]
	cmp	r0, #0
	beq	.L3130
	movw	r1, #47236
	mov	r7, #0
	movt	r1, 169
	add	r1, r5, r1
	mov	r2, r7
	mvn	r6, #0
.L3137:
	ldr	r3, [r1, #4]!
	add	r2, r2, #1
	cmp	r3, #0
	beq	.L3135
	ldr	r3, [r3, #56]
	cmn	r4, #1
	cmpne	r4, r3
	addeq	r7, r7, #1
	cmp	r4, r3
	cmnne	r4, #1
	movne	r6, r3
.L3135:
	cmp	r2, r0
	bne	.L3137
	cmp	r7, #0
	beq	.L3130
	adds	r8, r6, #1
	movne	r8, #1
	cmp	r4, r6
	movle	r9, #0
	andgt	r9, r8, #1
	b	.L3143
.L3144:
	subs	r7, r7, #1
	beq	.L3130
.L3143:
	cmp	r9, #0
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r1, r6
	mov	r0, r5
	beq	.L3140
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3130
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	bne	.L3145
.L3140:
	mov	r1, r4
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3130
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	bne	.L3145
	cmp	r4, r6
	movge	r3, #0
	andlt	r3, r8, #1
	cmp	r3, #0
	beq	.L3144
	mov	r1, r6
	sub	r3, fp, #44
	sub	r2, fp, #40
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	mov	r0, r5
	cmn	r1, #1
	beq	.L3130
	bl	MVC_OutputFrmFromDPB
	cmp	r0, #0
	beq	.L3144
.L3145:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3130:
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	add	r9, r0, #11141120
	add	r8, r0, #11075584
	add	r9, r9, #8192
	movw	r7, #9784
	movt	r7, 170
	add	r8, r8, #36864
	ldr	r4, [r9, #2116]
	add	r10, r0, #11075584
	add	r7, r0, r7
	mov	r5, r0
	add	r6, r4, #2000
	add	r4, r4, #4
	add	r6, r6, #4
.L3192:
	ldr	r3, [r4]
	cmp	r3, #6
	ldrls	pc, [pc, r3, asl #2]
	b	.L3183
.L3185:
	.word	.L3184
	.word	.L3186
	.word	.L3187
	.word	.L3188
	.word	.L3189
	.word	.L3190
	.word	.L3191
.L3191:
	ldr	r2, [r4, #12]
	mov	r1, r7
	mov	r0, r5
	bl	MVC_MarkCurrPicLT
.L3183:
	add	r4, r4, #20
	cmp	r4, r6
	bne	.L3192
.L3184:
	ldrb	r3, [r8, #3480]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3200
	mov	r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3190:
	mov	r0, r5
	bl	MVC_UnMarkAllSTRef
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r1, #0
	mov	r0, r5
	bl	MVC_UpdateMaxLTFrmIdx
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	mov	r3, #1
	strb	r3, [r8, #3480]
	b	.L3183
.L3189:
	mov	r0, r5
	ldr	r1, [r4, #16]
	bl	MVC_UpdateMaxLTFrmIdx
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	b	.L3183
.L3188:
	ldr	r3, [r4, #12]
	mov	r1, r7
	ldr	r2, [r4, #4]
	mov	r0, r5
	bl	MVC_MarkSTToLTRef
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	b	.L3183
.L3187:
	mov	r0, r5
	ldr	r2, [r4, #8]
	mov	r1, r7
	bl	MVC_UnMarkLTRef
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	b	.L3183
.L3186:
	mov	r0, r5
	ldr	r2, [r4, #4]
	mov	r1, r7
	bl	MVC_UnMarkSTRef
	mov	r0, r5
	bl	MVC_UpdateReflist
	b	.L3183
.L3200:
	add	r10, r10, #40960
	mov	r2, #0
	str	r2, [r9, #2120]
	movw	r3, #26758
	ldr	r1, [r10, #2164]
	movt	r3, 42
	add	r3, r1, r3
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	ldrb	r3, [r9, #1595]	@ zero_extendqisi2
	str	r2, [r9, #2816]
	cmp	r3, #1
	beq	.L3195
	bcc	.L3196
	cmp	r3, #2
	streq	r2, [r9, #2144]
	streq	r2, [r9, #2132]
	streq	r2, [r8, #3508]
	streq	r2, [r8, #3516]
.L3194:
	ldr	r1, [r9, #2192]
	mov	r0, r5
	bl	MVC_FlushDPB
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3196:
	ldr	r1, [r9, #2132]
	ldr	r2, [r9, #2140]
	ldr	r3, [r9, #2144]
	rsb	r2, r1, r2
	str	r2, [r9, #2140]
	rsb	r3, r1, r3
	str	r3, [r9, #2144]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r9, #2136]
	str	r3, [r9, #2132]
	ldr	r1, [r8, #3516]
	ldr	r2, [r8, #3504]
	ldr	r3, [r8, #3508]
	rsb	r2, r1, r2
	str	r2, [r8, #3504]
	rsb	r3, r1, r3
	str	r3, [r8, #3508]
	cmp	r3, r2
	movge	r3, r2
	str	r3, [r8, #3512]
	str	r3, [r8, #3516]
	b	.L3194
.L3195:
	str	r2, [r9, #2140]
	str	r2, [r9, #2132]
	str	r2, [r8, #3504]
	str	r2, [r8, #3516]
	b	.L3194
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
	add	r6, r6, #8192
	mov	r7, r0
	ldr	r3, [r6, #2116]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3202
	ldr	lr, [r0, #52]
	cmp	lr, #0
	beq	.L3211
	movw	r1, #47236
	mov	r2, #0
	movt	r1, 169
	add	r0, r0, #148
	add	r1, r7, r1
	mov	r4, r2
.L3210:
	ldr	r3, [r1, #4]!
	add	r2, r2, #1
	cmp	r3, #0
	beq	.L3207
	ldr	ip, [r3, #16]
	cmp	ip, #1
	streqb	ip, [r7, #6]
	ldr	ip, [r3, #56]
	ldr	r3, [r6, #2192]
	cmp	ip, r3
	streq	r4, [r0]
.L3207:
	cmp	r2, lr
	add	r0, r0, #4
	bne	.L3210
.L3211:
	add	r9, r7, #11075584
	add	ip, r9, #45056
	ldr	r10, [ip, #2376]
	cmp	r10, #0
	beq	.L3205
	movw	r4, #47236
	mov	r5, #0
	movt	r4, 169
	mov	r8, r5
	add	r4, r7, r4
	b	.L3216
.L3214:
	cmp	r5, r10
	beq	.L3205
.L3216:
	ldr	r1, [r4, #4]!
	add	r5, r5, #1
	cmp	r1, #0
	beq	.L3214
	ldr	r0, [r1, #56]
	ldr	r2, [r6, #2192]
	cmp	r0, r2
	bne	.L3214
	strb	r8, [r1, #578]
	mov	r2, #0
	strb	r8, [r1, #577]
	ldr	r1, [r4]
	str	ip, [fp, #-48]
	strb	r8, [r1, #614]
	strb	r8, [r1, #613]
	ldr	r1, [r4]
	strb	r8, [r1, #650]
	strb	r8, [r1, #649]
	ldr	r1, [r4]
	strb	r8, [r1, #3]
	ldr	r1, [r4]
	ldr	r0, [r7, #120]
	ldrsb	r1, [r1, #6]
	bl	FSP_SetRef
	ldr	r1, [r4]
	mov	r3, #1
	movw	r2, #47448
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
	beq	.L3215
	ldr	r1, [r4]
	ldr	r0, [r7, #120]
	ldrsb	r1, [r1, #6]
	bl	FSP_SetDisplay
	ldr	ip, [fp, #-48]
.L3215:
	str	r8, [r4]
	cmp	r5, r10
	str	r8, [r4, #64]
	str	r8, [r4, #128]
	ldr	r2, [ip, #2380]
	sub	r2, r2, #1
	str	r2, [ip, #2380]
	bne	.L3216
.L3205:
	mov	r0, r7
	bl	MVC_UpdateReflist
	mov	r0, r7
	bl	MVC_UpdateLTReflist
	ldr	r3, [r6, #2116]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3229
	add	r9, r9, #45056
	mov	r0, r3
	mov	r2, #1
	str	r3, [r9, #2392]
	strb	r3, [r6, #1596]
	strb	r2, [r6, #1597]
.L3217:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3229:
	add	r9, r9, #45056
	mov	r3, #0
	mov	r2, #1
	mov	r0, r3
	str	r2, [r9, #2392]
	strb	r2, [r6, #1596]
	str	r3, [r6, #2124]
	strb	r3, [r6, #1597]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3202:
	ldr	r1, [r6, #2192]
	bl	MVC_FlushDPB
	cmp	r0, #0
	addeq	r9, r7, #11075584
	beq	.L3205
.L3218:
	mvn	r0, #0
	b	.L3217
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
	add	r4, r5, #8192
	mov	r2, #0
	mov	r6, r0
	strb	r2, [r3, #3480]
	ldrb	r2, [r4, #1595]	@ zero_extendqisi2
	sub	r2, r2, #2
	clz	r2, r2
	mov	r2, r2, lsr #5
	strb	r2, [r3, #3481]
	ldrb	r3, [r4, #1598]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3231
	ldrb	r3, [r4, #1604]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3232
	ldr	r3, [r0, #224]
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L3253
.L3232:
	ldr	r3, [r4, #2116]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r4, #2128]
	beq	.L3254
	cmp	r3, #0
	bne	.L3255
.L3239:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3254:
	cmp	r3, #0
	beq	.L3239
.L3238:
	ldrb	r7, [r4, #1593]	@ zero_extendqisi2
	cmp	r7, #0
	beq	.L3256
.L3242:
	add	r5, r5, #8192
	mov	r0, #0
	ldrb	r3, [r5, #1596]	@ zero_extendqisi2
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r5, #1597]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3253:
	ldr	r2, [r3, #704]
	cmp	r2, #0
	bne	.L3232
	ldr	r2, [r3, #712]
	cmp	r2, #0
	bne	.L3232
	ldr	r3, [r3, #684]
	add	r3, r3, #1024
	cmp	r3, #2048
	bls	.L3232
.L3231:
	mov	r0, r6
	bl	MVC_IDRMemMarking
	cmp	r0, #0
	bne	.L3233
.L3240:
	ldrb	r3, [r4, #1598]	@ zero_extendqisi2
	cmp	r3, #5
	beq	.L3235
	ldr	r3, [r4, #2128]
	cmp	r3, #0
	beq	.L3239
	ldr	r3, [r4, #2116]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3238
	b	.L3242
.L3256:
	mov	r0, r6
	bl	MVC_SlidingWinMark
	ldr	r3, [r4, #2116]
	strb	r7, [r3]
	strb	r7, [r4, #1596]
.L3235:
	ldr	r3, [r4, #2128]
	cmp	r3, #0
	bne	.L3242
	b	.L3239
.L3255:
	mov	r0, r6
	bl	MVC_AdaptiveMemMark
	ldr	r3, [r4, #2116]
	mov	r2, #0
	strb	r2, [r3]
	cmp	r0, r2
	beq	.L3240
.L3233:
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
	add	r5, r0, #11141120
	add	r5, r5, #8192
	mov	r4, r0
	ldrb	r6, [r5, #1595]	@ zero_extendqisi2
	cmp	r6, #1
	beq	.L3259
	bcc	.L3260
	cmp	r6, #2
	beq	.L3261
	mov	r6, #0
.L3258:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3261:
	ldr	r3, [r5, #2112]
	mov	r2, #3
	strb	r2, [r3, #576]
	ldrb	r2, [r5, #1593]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3279
	ldr	r1, [r0, #520]
	cmp	r1, #0
	beq	.L3280
	bl	MVC_OutputFrmToVO
	ldr	r3, [r5, #2112]
	str	r3, [r4, #520]
	str	r3, [r3, #652]
	mov	r6, r0
.L3281:
	ldr	r3, [r4, #520]
	mov	r8, #2
	ldrb	r0, [r5, #1594]	@ zero_extendqisi2
	mov	r7, #0
	add	r1, r5, #1600
	mov	r2, #504
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
	ldrb	r0, [r5, #1604]	@ zero_extendqisi2
	strb	r0, [r3]
	ldr	r3, [r4, #520]
	str	r7, [r3, #40]
	ldr	r0, [r4, #520]
	add	r0, r0, #72
	bl	memcpy
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #2164]
	str	r2, [r3, #672]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #672]
	str	r2, [r3, #600]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	strb	r8, [r3, #648]
	ldrb	r2, [r5, #1599]	@ zero_extendqisi2
	ldr	r3, [r4, #520]
	cmp	r2, #1
	movne	r2, r7
	moveq	r2, r8
	strb	r2, [r3, #4]
.L3338:
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #2132]
	str	r2, [r3, #32]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #2192]
	str	r2, [r3, #56]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #2196]
	str	r2, [r3, #60]
	ldrb	r8, [r4, #9]	@ zero_extendqisi2
	cmp	r8, #1
	bne	.L3258
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
	beq	.L3291
	ldr	r3, [r5, #2112]
	mov	r2, r8
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	b	.L3291
.L3260:
	ldr	r3, [r5, #2112]
	mov	r2, #0
	strb	r2, [r3, #576]
	ldr	r1, [r0, #520]
	cmp	r1, r2
	beq	.L3262
	bl	MVC_OutputFrmToVO
	cmp	r0, #0
	bne	.L3341
.L3263:
	ldr	r3, [r4, #520]
	cmp	r3, #0
	beq	.L3262
	ldr	r3, [r5, #2112]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
.L3262:
	ldr	r3, [r5, #2112]
	mov	r0, #3
	mov	r7, #0
	add	r1, r5, #1600
	add	r1, r1, #8
	mov	r2, #504
	str	r3, [r4, #520]
	str	r3, [r3, #580]
	ldr	r3, [r4, #520]
	ldr	ip, [r5, #2112]
	str	ip, [r3, #616]
	ldr	r3, [r4, #520]
	ldr	ip, [r5, #2112]
	str	ip, [r3, #652]
	ldrb	ip, [r5, #1594]	@ zero_extendqisi2
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
	ldr	r1, [r5, #2164]
	mov	r2, r7
	str	r1, [r3, #600]
	str	r1, [r3, #44]
	ldr	r3, [r4, #520]
	strb	r7, [r3, #576]
	ldr	r3, [r4, #520]
	ldrb	r1, [r5, #1604]	@ zero_extendqisi2
	strb	r1, [r3]
	ldr	r3, [r4, #520]
	ldrb	r1, [r5, #1601]	@ zero_extendqisi2
	str	r1, [r3, #40]
	ldrb	r1, [r5, #1599]	@ zero_extendqisi2
	ldr	r3, [r4, #520]
	cmp	r1, #1
	moveq	r6, #3
	strb	r6, [r3, #4]
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #2132]
	str	r1, [r3, #32]
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #2192]
	str	r1, [r3, #56]
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #2196]
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
	beq	.L3291
.L3333:
	ldr	r3, [r5, #2112]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
.L3291:
	mov	r3, #0
	mov	r0, r6
	str	r3, [r4, #520]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3259:
	ldr	r3, [r5, #2112]
	mov	r2, #3
	strb	r2, [r3, #576]
	ldrb	r2, [r5, #1593]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3267
	ldr	r1, [r0, #520]
	cmp	r1, #0
	beq	.L3268
	bl	MVC_OutputFrmToVO
	ldr	r3, [r5, #2112]
	str	r3, [r4, #520]
	str	r3, [r3, #616]
	mov	r6, r0
.L3269:
	ldr	r3, [r4, #520]
	mov	r8, #1
	ldrb	r0, [r5, #1594]	@ zero_extendqisi2
	mov	r7, #0
	add	r1, r5, #1600
	mov	r2, #504
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
	ldrb	r0, [r5, #1604]	@ zero_extendqisi2
	strb	r0, [r3]
	ldr	r3, [r4, #520]
	str	r7, [r3, #40]
	ldr	r0, [r4, #520]
	add	r0, r0, #72
	bl	memcpy
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #2164]
	str	r2, [r3, #636]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #636]
	str	r2, [r3, #600]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	strb	r8, [r3, #612]
	ldrb	r3, [r5, #1599]	@ zero_extendqisi2
	ldr	r2, [r4, #520]
	rsb	r3, r8, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r2, #4]
	b	.L3338
.L3279:
	ldr	r3, [r0, #520]
	add	r1, r5, #1600
	mov	r2, #504
	add	r1, r1, #8
	cmp	r3, #0
	ldreq	r3, [r5, #2112]
	streq	r3, [r0, #520]
	streq	r3, [r3, #652]
	ldreq	r3, [r0, #520]
	add	r0, r3, #72
	ldr	ip, [r3, #80]
	ldrd	r6, [r3, #88]
	ldrd	r8, [r3, #96]
	ldr	r10, [r3, #84]
	str	ip, [fp, #-48]
	bl	memcpy
	movw	r3, #9808
	ldr	ip, [fp, #-48]
	mov	r1, r3
	movt	r3, 170
	add	r3, r4, r3
	movt	r1, 170
	ldr	r1, [r4, r1]
	and	r2, ip, r10
	ldr	r3, [r3, #4]
	and	r1, r1, r3
	adds	r3, r2, #1
	movne	r3, #1
	cmn	r1, #1
	movw	r1, #9824
	movne	r3, #0
	cmp	r3, #0
	movt	r1, 170
	ldrne	r3, [r4, #520]
	strne	ip, [r3, #80]
	mov	ip, #2
	strne	r10, [r3, #84]
	movw	r3, #9824
	movt	r3, 170
	add	r3, r4, r3
	ldr	r0, [r4, #520]
	ldrd	r2, [r3, #-8]
	cmp	r3, r7
	cmpeq	r2, r6
	movhi	r2, r6
	movhi	r3, r7
	strd	r2, [r0, #88]
	ldrd	r2, [r4, r1]
	ldr	r0, [r4, #520]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r2, r8
	movhi	r3, r9
	strd	r2, [r0, #96]
	mov	r0, #3
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r5, #1594]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #2]
	ldr	r2, [r4, #520]
	ldrb	r1, [r5, #1604]	@ zero_extendqisi2
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r4, #520]
	strb	ip, [r3, #648]
	ldr	r3, [r4, #520]
	strb	r0, [r3, #576]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #2164]
	str	r2, [r3, #672]
	ldr	r2, [r4, #520]
	ldr	r3, [r2, #672]
	ldr	r1, [r2, #636]
	add	r3, r3, r1
	mov	r3, r3, lsr #1
	str	r3, [r2, #600]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #600]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #652]
	str	r2, [r3, #580]
	ldrb	r3, [r5, #1599]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #520]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	beq	.L3339
.L3290:
	strb	r2, [r3, #4]
	mov	r2, #0
	ldr	r3, [r4, #520]
	ldr	r1, [r5, #2132]
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
	bne	.L3333
	b	.L3291
.L3267:
	ldr	r3, [r0, #520]
	add	r1, r5, #1600
	mov	r2, #504
	add	r1, r1, #8
	cmp	r3, #0
	ldreq	r3, [r5, #2112]
	streq	r3, [r0, #520]
	streq	r3, [r3, #616]
	ldreq	r3, [r0, #520]
	add	r0, r3, #72
	ldr	ip, [r3, #80]
	ldrd	r6, [r3, #88]
	ldrd	r8, [r3, #96]
	ldr	r10, [r3, #84]
	str	ip, [fp, #-48]
	bl	memcpy
	movw	r3, #9808
	ldr	ip, [fp, #-48]
	mov	r1, r3
	movt	r3, 170
	add	r3, r4, r3
	movt	r1, 170
	ldr	r1, [r4, r1]
	and	r2, ip, r10
	ldr	r3, [r3, #4]
	and	r1, r1, r3
	adds	r3, r2, #1
	movne	r3, #1
	cmn	r1, #1
	movw	r1, #9824
	movne	r3, #0
	cmp	r3, #0
	movt	r1, 170
	ldrne	r3, [r4, #520]
	strne	ip, [r3, #80]
	mov	ip, #1
	strne	r10, [r3, #84]
	movw	r3, #9824
	movt	r3, 170
	add	r3, r4, r3
	ldr	r0, [r4, #520]
	ldrd	r2, [r3, #-8]
	cmp	r3, r7
	cmpeq	r2, r6
	movhi	r2, r6
	movhi	r3, r7
	strd	r2, [r0, #88]
	ldrd	r2, [r4, r1]
	ldr	r0, [r4, #520]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r2, r8
	movhi	r3, r9
	strd	r2, [r0, #96]
	mov	r0, #3
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r5, #1594]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r4, #520]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, ip
	strb	r3, [r2, #2]
	ldr	r2, [r4, #520]
	ldrb	r1, [r5, #1604]	@ zero_extendqisi2
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r4, #520]
	strb	ip, [r3, #612]
	ldr	r3, [r4, #520]
	strb	r0, [r3, #576]
	ldr	r3, [r4, #520]
	ldr	r2, [r5, #2164]
	str	r2, [r3, #636]
	ldr	r2, [r4, #520]
	ldr	r3, [r2, #672]
	ldr	r1, [r2, #636]
	add	r3, r3, r1
	mov	r3, r3, lsr ip
	str	r3, [r2, #600]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #600]
	str	r2, [r3, #44]
	ldr	r3, [r4, #520]
	ldr	r2, [r3, #616]
	str	r2, [r3, #580]
	ldrb	r3, [r5, #1599]	@ zero_extendqisi2
	cmp	r3, ip
	ldr	r3, [r4, #520]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	bne	.L3290
.L3339:
	orr	r2, r2, ip
	b	.L3290
.L3341:
	ldr	r1, .L3342
	movw	r3, #2519
	str	r0, [sp]
	mov	r0, #22
	ldr	r2, .L3342+4
	ldr	ip, [r1, #68]
	ldr	r1, .L3342+8
	blx	ip
	b	.L3263
.L3280:
	ldr	r3, [r5, #2112]
	mov	r6, r1
	str	r3, [r0, #520]
	str	r3, [r3, #652]
	b	.L3281
.L3268:
	ldr	r3, [r5, #2112]
	mov	r6, r1
	str	r3, [r0, #520]
	str	r3, [r3, #616]
	b	.L3269
.L3343:
	.align	2
.L3342:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+248
	.word	.LC400
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
	ldr	r3, [r6, #2376]
	cmp	r3, #0
	beq	.L3358
	movw	r8, #47236
	mov	r4, #0
	movt	r8, 169
	add	r8, r0, r8
	mov	r7, r4
	mov	r9, r8
	b	.L3347
.L3346:
	ldr	r3, [r6, #2376]
	cmp	r3, r7
	bls	.L3385
.L3347:
	ldr	r3, [r9, #4]!
	add	r7, r7, #1
	cmp	r3, #0
	beq	.L3346
	ldrb	r2, [r3, #5]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3346
	ldrsb	r1, [r3, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetLogicFs
	cmp	r0, #0
	beq	.L3346
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	sub	r3, r3, #1
	cmp	r3, #1
	ldr	r3, [r6, #2376]
	addls	r4, r4, #1
	cmp	r3, r7
	bhi	.L3347
.L3385:
	cmp	r4, #0
	beq	.L3348
	mvn	r7, #0
	b	.L3351
.L3349:
	ldr	r1, [r5, r3, asl #2]
	bl	MVC_OutputFrmToVO
	subs	r4, r4, #1
	mov	r7, r0
	beq	.L3383
.L3351:
	sub	r3, fp, #44
	sub	r2, fp, #40
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_GetMinPOC
	ldr	r1, [fp, #-44]
	movw	r3, #28194
	mov	r2, r4
	cmn	r1, #1
	movt	r3, 42
	mov	r0, r5
	add	r3, r1, r3
	bne	.L3349
.L3383:
	ldr	r3, [r6, #2376]
	cmp	r3, #0
	beq	.L3345
.L3350:
	mov	r4, #0
	b	.L3354
.L3353:
	ldr	r3, [r6, #2376]
	cmp	r3, r4
	bls	.L3345
.L3354:
	ldr	r1, [r8, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3353
	ldrb	r3, [r1, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3353
	mov	r0, r5
	bl	MVC_UnMarkFrameStoreRef
	ldr	r3, [r6, #2376]
	cmp	r3, r4
	bhi	.L3354
.L3345:
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
	cmp	r7, #0
	bne	.L3356
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3348:
	cmp	r3, #0
	mvnne	r7, #0
	bne	.L3350
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
.L3356:
	add	r0, r5, #584
	bl	GetVoLastImageID
	mov	r7, r0
	mov	r0, r7
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3358:
	mvn	r7, #0
	b	.L3345
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
	beq	.L3403
	bl	MVC_ClearAllSlice
	movw	r7, #47240
	movw	r6, #47368
	movw	r5, #47304
	movt	r7, 169
	movt	r6, 169
	movt	r5, 169
	mov	r8, #0
	add	r7, r4, r7
	add	r6, r4, r6
	add	r5, r4, r5
	mov	r9, r8
.L3391:
	ldr	r3, [r7]
	mov	r2, #1
	add	r8, r8, r2
	cmp	r3, #0
	beq	.L3388
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3388:
	ldr	r3, [r5]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3389
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3389:
	ldr	r3, [r6]
	mov	r2, #1
	cmp	r3, #0
	beq	.L3390
	ldrsb	r1, [r3, #6]
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3390:
	cmp	r8, #16
	str	r9, [r6], #4
	mov	r2, #0
	str	r9, [r5], #4
	str	r9, [r7], #4
	bne	.L3391
	ldr	r3, [r4, #44]
	add	r5, r4, #11075584
	add	r6, r5, #45056
	str	r3, [r6, #2376]
	str	r2, [r6, #2380]
	str	r2, [r6, #2384]
	str	r2, [r6, #2388]
	str	r2, [r6, #2392]
	ldr	r1, [r4, #520]
	cmp	r1, r2
	ldreq	r7, .L3421
	beq	.L3393
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3394
	ldr	r7, .L3421
.L3398:
	add	r3, r4, #11141120
	add	r3, r3, #8192
	ldr	r3, [r3, #2112]
	cmp	r3, #0
	beq	.L3396
	ldrsb	r1, [r3, #6]
	mov	r2, #1
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
.L3396:
	mov	r3, #0
	str	r3, [r4, #520]
.L3393:
	movw	r0, #47452
	ldr	r3, [r7, #48]
	mov	r2, #344
	mov	r1, #0
	movt	r0, 169
	add	r0, r4, r0
	blx	r3
	ldr	ip, [r6, #2376]
	mov	r0, #0
	add	r3, r4, #144
	add	r1, r4, #216
	mov	r2, r0
	str	ip, [r6, #2604]
	str	r0, [r6, #2608]
.L3400:
	str	r2, [r3, #4]!
	cmp	r3, r1
	bne	.L3400
	add	r2, r4, #252
	add	r3, r4, #384
	mov	r0, #0
.L3401:
	str	r0, [r2, #4]!
	cmp	r2, r3
	mov	r1, #0
	bne	.L3401
	add	r2, r5, #40960
	add	r4, r4, #516
	mov	r0, r1
	str	r1, [r2, #568]
.L3402:
	str	r0, [r3, #4]!
	cmp	r3, r4
	mov	r2, #0
	bne	.L3402
	add	r5, r5, #40960
	mov	r0, r2
	str	r2, [r5, #572]
.L3387:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3394:
	mov	r0, r4
	ldr	r7, .L3421
	bl	MVC_OutputFrmToVO
	cmp	r0, #0
	bne	.L3420
.L3397:
	ldr	r3, [r4, #520]
	cmp	r3, #0
	bne	.L3398
	b	.L3393
.L3420:
	str	r0, [sp]
	movw	r3, #5930
	ldr	r2, .L3421+4
	mov	r0, #22
	ldr	ip, [r7, #68]
	ldr	r1, .L3421+8
	blx	ip
	b	.L3397
.L3403:
	mvn	r0, #0
	b	.L3387
.L3422:
	.align	2
.L3421:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+268
	.word	.LC401
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
	bne	.L3432
.L3424:
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L3425
	ldr	ip, .L3433
	mov	r3, #340
	ldr	r2, .L3433+4
	mov	r0, #22
	ldr	r1, .L3433+8
	ldr	ip, [ip, #68]
	blx	ip
.L3425:
	mov	r0, #0
	strb	r0, [r4, #6]
	str	r0, [r4, #220]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3432:
	ldr	ip, .L3433
	movw	r3, #335
	ldr	r2, .L3433+4
	mov	r0, #22
	ldr	r1, .L3433+12
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3424
.L3434:
	.align	2
.L3433:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+280
	.word	.LC403
	.word	.LC402
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
	add	r5, r6, #8192
	mov	r4, r0
	ldrb	r3, [r5, #1595]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3436
	ldrb	r3, [r5, #1593]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3436
.L3437:
	ldr	r3, [r4, #108]
	ldrb	r2, [r4, #8]	@ zero_extendqisi2
	ubfx	r7, r3, #16, #2
	cmp	r7, r2
	beq	.L3438
	sub	r2, r2, #2
	cmp	r2, #1
	cmpls	r7, #1
	bls	.L3495
.L3438:
	uxtb	r3, r7
	strb	r3, [r4, #8]
	cmp	r3, #2
	beq	.L3441
	cmp	r3, #3
	beq	.L3442
	cmp	r3, #1
	beq	.L3496
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3497
.L3449:
	ldr	r3, [r4, #64]
.L3445:
	ldrb	r2, [r5, #1605]	@ zero_extendqisi2
	strb	r2, [r4, #11]
.L3450:
	cmp	r3, #0
	beq	.L3493
	ldrb	r3, [r5, #1604]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L3498
	cmp	r3, #1
	bne	.L3453
	ldr	r2, [r5, #2192]
	cmp	r2, #0
	streqb	r3, [r4, #7]
.L3453:
	ldr	r1, .L3501
	mov	r0, #2
	ldr	r2, [r4, #88]
	ldrb	r3, [r5, #1595]	@ zero_extendqisi2
	ldr	ip, [r1, #68]
	sub	r2, r2, #1
	ldr	r1, .L3501+4
	blx	ip
	ldr	r2, [r5, #2184]
	ldr	r1, [r5, #2180]
	mov	r3, #1
	strb	r3, [r5, #3386]
	add	r3, r6, #12288
	str	r2, [r5, #4044]
	str	r1, [r5, #4048]
	ldr	r2, [r4, #64]
	str	r2, [r3, #152]
	ldr	r0, [r4, #88]
	cmp	r0, #0
	beq	.L3490
	ldrb	r1, [r5, #1604]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L3454
	ldr	r2, .L3501+8
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3455
	ldr	r2, [r3, #156]
	cmp	r2, #0
	beq	.L3464
.L3457:
	ldr	r3, [r2, #8]
	ldr	r0, [r2, #12]
	ldr	r2, [r2, #4056]
	add	r3, r3, r0
	add	r3, r3, #7
	cmp	r2, #0
	add	r1, r1, r3, lsr #3
	bne	.L3457
.L3456:
	sub	r2, fp, #28
	mov	r3, #4
	ldr	r0, [r4, #120]
	str	r1, [r2, #-4]!
	mov	r1, #20
	blx	ip
	ldr	r2, [r4, #224]
.L3458:
	movw	r3, #11576
	movt	r3, 170
	add	r3, r4, r3
	str	r3, [r4, #228]
	ldr	r1, [r2, #884]
	cmp	r1, #1
	beq	.L3499
	cmp	r1, #2
	beq	.L3500
.L3461:
	ldr	r3, [r2, #676]
	add	r6, r6, #12288
	mov	r0, #0
	str	r3, [r6, #416]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3436:
	ldr	r3, [r4, #140]
	ldr	r1, [r4, #144]
	add	r3, r3, #1
	ldr	r2, [r4, #224]
	cmp	r3, r1
	movlt	r3, r1
	str	r3, [r4, #140]
	str	r3, [r2, #600]
	ldr	r3, [r4, #140]
	ldr	r1, [r4, #144]
	rsb	r3, r1, r3
	str	r3, [r2, #604]
	b	.L3437
.L3495:
	ldr	ip, .L3501
	mov	r2, r7
	ldr	r1, .L3501+12
	mov	r0, #2
	ldr	ip, [ip, #68]
	blx	ip
	strb	r7, [r4, #8]
.L3442:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mov	r0, r4
	mvn	r1, #0
	bl	MVC_ClearDPB
	mvn	r0, #0
.L3490:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3496:
	ldrb	r3, [r5, #1604]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3449
	ldr	r3, [r5, #2128]
	cmp	r3, #0
	beq	.L3446
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L3449
.L3462:
	ldrb	r2, [r5, #1605]	@ zero_extendqisi2
	ldr	r3, [r4, #64]
	strb	r2, [r4, #11]
	b	.L3450
.L3441:
	ldrb	r3, [r5, #1604]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3446
	ldr	r3, [r4, #64]
	cmp	r3, #0
	beq	.L3445
	ldr	r2, [r4, #544]
	ldr	r2, [r2, #48]
	cmp	r2, #0
	beq	.L3445
.L3446:
	mov	r3, #1
	mov	r0, r4
	strb	r3, [r5, #1599]
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3490
.L3454:
	add	r3, r4, #11075584
	add	r3, r3, #45056
	ldr	r3, [r3, #2380]
	cmp	r3, #0
	bne	.L3455
	ldr	r2, [r4, #224]
	ldr	r3, [r2, #12]
	cmp	r3, #2
	beq	.L3458
.L3493:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3490
.L3497:
	ldrb	r3, [r5, #1604]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3449
	b	.L3462
.L3498:
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3453
	ldr	r3, [r4, #224]
	ldr	r7, [r3, #12]
	cmp	r7, #0
	bne	.L3453
	ldr	r1, .L3501
	mov	r0, #1
	ldr	r3, [r5, #2128]
	ldr	r2, [r5, #2132]
	ldr	ip, [r1, #68]
	ldr	r1, .L3501+16
	blx	ip
	mov	r0, r4
	bl	MVC_ClearCurrPic
	ldr	r3, [r5, #2112]
	ldr	r0, [r4, #120]
	mov	r2, r7
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L3490
.L3455:
	ldr	r2, [r4, #224]
	b	.L3458
.L3499:
	cmp	r3, #0
	beq	.L3461
	ldrb	r3, [r5, #1604]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3461
	mov	r2, #0
	str	r2, [r4, #228]
	ldr	r3, [r5, #2112]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	b	.L3493
.L3500:
	ldr	r1, [r2, #888]
	cmp	r3, #0
	cmpne	r1, #0
	ble	.L3461
	ldrb	r3, [r5, #1604]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3461
	mov	r2, #0
	str	r2, [r4, #228]
	ldr	r3, [r5, #2112]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_SetDisplay
	ldr	r2, [r4, #224]
	mov	r0, r4
	ldr	r3, [r2, #888]
	sub	r3, r3, #1
	str	r3, [r2, #888]
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L3490
.L3464:
	mov	r1, r2
	b	.L3456
.L3502:
	.align	2
.L3501:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC406
	.word	g_event_report
	.word	.LC404
	.word	.LC405
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
	strb	r1, [r3, #525]
	str	ip, [r3, #576]
	strb	r1, [r0]
	bl	MVC_DecVDM
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	ip, .L3505
	movw	r3, #12955
	ldr	r2, .L3505+4
	mov	r0, #22
	ldr	r1, .L3505+8
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L3506:
	.align	2
.L3505:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+296
	.word	.LC407
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r6, .L3570
	ldr	lr, [r1, #12]
	mov	r4, r0
	ldr	r3, [r1, #8]
	mov	r5, r1
	ldr	r2, [r1, #4]
	mov	r0, #7
	add	r7, r4, #11141120
	ldr	ip, [r6, #68]
	ldr	r1, .L3570+4
	str	lr, [sp]
	blx	ip
	add	r3, r7, #12288
	mov	r2, #0
	str	r2, [r3, #424]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L3565
.L3508:
	ldr	r3, [r4, #232]
	cmp	r3, #0
	beq	.L3528
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3566
.L3511:
	ldr	r1, .L3570+8
	mov	r0, #1
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L3528
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L3528:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3515
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq	.L3515
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3515
	ldrb	r3, [r4, #937]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3531
	mov	r2, r4
	mov	r3, #1
	b	.L3518
.L3517:
	add	r3, r3, #1
	cmp	r3, #137
	beq	.L3529
.L3518:
	ldrb	r1, [r2, #1025]	@ zero_extendqisi2
	add	r2, r2, #88
	cmp	r1, #0
	bne	.L3517
	mov	r1, r3
.L3516:
	mov	r2, #88
	cmn	r1, #1
	mla	r3, r2, r3, r4
	mov	r0, #1
	strb	r0, [r3, #937]
	beq	.L3529
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
	b	.L3521
.L3566:
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bhi	.L3511
	ldrb	r1, [r3, #3]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3512
	b	.L3511
.L3565:
	ldr	r3, [r3, #892]
	cmp	r3, #0
	beq	.L3508
	ldr	r3, [r4, #232]
	cmp	r3, #0
	ldrneb	r2, [r5]	@ zero_extendqisi2
	beq	.L3528
.L3512:
	strb	r2, [r3, #3]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #852]
	cmp	r2, #1
	beq	.L3567
.L3523:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	ble	.L3524
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq	.L3524
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L3524
	ldr	r3, [r4, #232]
	ldr	r1, [r3, #68]
	cmp	r1, #1
	bls	.L3568
.L3525:
	mov	r2, #1
	strb	r2, [r3, #3]
	ldr	r1, [r5, #16]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3570+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3562
	mov	r3, #0
	mov	r1, #113
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L3562:
	ldr	r3, [r4, #232]
.L3521:
	ldrb	r0, [r3, #3]	@ zero_extendqisi2
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3515:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L3570+16
	blx	r3
	ldr	r1, [r5, #16]
	ldr	r0, [r4, #120]
	bl	SM_ReleaseStreamSeg
	ldr	r3, .L3570+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L3564
.L3563:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
.L3564:
	mvn	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L3524:
	ldr	r3, [r4, #232]
	b	.L3525
.L3567:
	ldr	r3, [r3, #892]
	cmp	r3, #0
	bne	.L3562
	b	.L3523
.L3568:
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
	bcc	.L3569
.L3526:
	add	r7, r7, #12288
	mov	r2, #1
	str	r2, [r7, #424]
	b	.L3521
.L3569:
	mov	r0, r4
	bl	MVC_CombinePacket.part.10
	ldr	r3, [r4, #232]
	b	.L3526
.L3529:
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r1, .L3570+20
	blx	r3
	mov	r0, r4
	bl	MVC_ClearAllNal
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r3, .L3570+12
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L3563
	b	.L3564
.L3531:
	mov	r1, r3
	b	.L3516
.L3571:
	.align	2
.L3570:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC408
	.word	.LC409
	.word	g_event_report
	.word	.LC411
	.word	.LC410
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
	ldr	r9, .L3603
	mov	r8, r0
	mov	r4, r1
	mov	r0, #22
	ldr	r1, .L3603+4
	ldr	r3, [r9, #68]
	blx	r3
	mov	r0, r8
	bl	MVC_ClearCurrPic
	mov	r0, r8
	bl	MVC_ClearAllNal
	mov	r0, r8
	bl	MVC_InitDPB
	cmp	r0, #0
	bne	.L3602
.L3573:
	cmp	r4, #0
	beq	.L3574
	add	r1, r8, #584
	ldr	r0, [r8, #120]
	bl	FSP_ClearNotInVoQueue
.L3575:
	movw	r4, #47800
	mov	r7, #0
	movt	r4, 169
	add	r4, r8, r4
	mov	r10, r7
.L3578:
	add	r5, r4, #280
	add	r6, r4, #296
	mov	r3, #1
	mvn	ip, #0
	mov	r0, #18
	mov	r1, #16
	mov	r2, #2
	strb	r10, [r4, #1]
	strb	r10, [r4, #2]
	strb	r10, [r4, #3]
	str	r10, [r4, #28]
	str	r10, [r4, #20]
	str	r10, [r4, #32]
	strb	r10, [r4, #5]
	str	r7, [r4, #268]
	str	r4, [r4, #652]
	str	r4, [r4, #616]
	str	r4, [r4, #580]
	strb	r10, [r4, #576]
	str	ip, [r4, #24]
	strb	r3, [r4, #7]
	strb	r3, [r4, #612]
	str	r0, [r4, #48]
	str	r1, [r4, #52]
	strb	r2, [r4, #648]
.L3577:
	ldr	r1, [r5, #4]!
	cmp	r1, #0
	beq	.L3576
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	str	r10, [r5]
.L3576:
	cmp	r5, r6
	bne	.L3577
	add	r7, r7, #1
	add	r4, r4, #688
	cmp	r7, #40
	bne	.L3578
	movw	r4, #10008
	movw	r5, #10024
	movt	r4, 170
	movt	r5, 170
	add	r4, r8, r4
	add	r5, r8, r5
	mov	r6, #0
.L3580:
	ldr	r1, [r4, #4]!
	cmp	r1, #0
	beq	.L3579
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	str	r6, [r4]
.L3579:
	cmp	r4, r5
	bne	.L3580
	ldr	r3, [r8, #60]
	cmp	r3, #0
	beq	.L3584
	mov	r4, #0
	add	r5, r8, #524
	mov	r6, r4
.L3583:
	ldr	r1, [r5, #4]!
	add	r4, r4, #1
	cmp	r1, #0
	beq	.L3582
	ldr	r0, [r8, #120]
	bl	FreeUsdByDec
	str	r6, [r5]
.L3582:
	ldr	r3, [r8, #60]
	cmp	r3, r4
	bhi	.L3583
.L3584:
	add	r5, r8, #11075584
	movw	r0, #41480
	mov	r4, #0
	add	r3, r5, #45056
	strb	r4, [r8, #4]
	add	r5, r5, #40960
	strb	r4, [r8, #9]
	movt	r0, 169
	strb	r4, [r8, #6]
	add	r0, r8, r0
	strb	r4, [r8, #7]
	mov	r1, r4
	strb	r4, [r8, #3]
	movw	r2, #1656
	str	r4, [r8, #60]
	str	r4, [r8, #64]
	str	r4, [r8, #104]
	str	r4, [r8, #220]
	str	r4, [r8, #232]
	add	r8, r8, #11141120
	add	r8, r8, #8192
	strb	r4, [r3, #132]
	ldr	ip, [r9, #48]
	strb	r4, [r5, #2224]
	strb	r4, [r3, #133]
	strb	r4, [r5, #2225]
	blx	ip
	mov	r2, #2
	mov	r3, #1
	strb	r2, [r5, #520]
	strb	r3, [r5, #525]
	mov	r0, r4
	mov	r3, #256
	strb	r4, [r8, #1592]
	strb	r4, [r8, #2808]
	str	r4, [r8, #2812]
	str	r3, [r8, #2816]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3574:
	add	r0, r8, #584
	bl	ResetVoQueue
	ldr	r0, [r8, #120]
	bl	FSP_EmptyInstance
	b	.L3575
.L3602:
	ldr	ip, [r9, #68]
	movw	r3, #365
	ldr	r2, .L3603+8
	mov	r0, #22
	ldr	r1, .L3603+12
	blx	ip
	b	.L3573
.L3604:
	.align	2
.L3603:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC412
	.word	.LANCHOR0+316
	.word	.LC403
	UNWIND(.fnend)
	.size	MVC_ClearAll, .-MVC_ClearAll
	.align	2
	.global	MVC_InsertFrmInDPB
	.type	MVC_InsertFrmInDPB, %function
MVC_InsertFrmInDPB:
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
	add	r3, r0, #11075584
	add	r3, r3, #45056
	mov	r4, r2
	ldrb	r8, [r4, #3]	@ zero_extendqisi2
	mov	r5, r0
	ldr	r3, [r3, #2380]
	mov	r6, r1
	cmp	r3, #0
	ldreq	r2, .L3707
	streq	r3, [r2, #-1884]
	cmp	r8, #1
	beq	.L3608
	bcc	.L3609
	cmp	r8, #2
	beq	.L3610
	ldr	r3, .L3707+4
	mov	r2, r8
	ldr	r1, .L3707+8
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r5
	mov	r1, #1
	bl	MVC_ClearAll
	mov	r0, #0
.L3614:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3610:
	ldr	r3, [r4, #520]
	mov	r2, #3
	strb	r2, [r3, #576]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r3, r1, asl #2
	beq	.L3630
	add	r10, r0, r3
	add	r10, r10, #11075584
	add	r7, r10, #45056
	ldr	r2, [r7, #2184]
	cmp	r2, #0
	beq	.L3630
	strb	r8, [r2, #648]
	ldr	r3, [r7, #2184]
	ldr	r2, [r4, #520]
	str	r2, [r3, #652]
	ldr	r3, [r7, #2184]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #649]
	ldr	r3, [r7, #2184]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #650]
	ldr	r3, [r7, #2184]
	ldr	r2, [r4, #552]
	str	r2, [r3, #664]
	ldr	r3, [r7, #2184]
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strb	r2, [r3, #651]
	ldr	r3, [r7, #2184]
	ldr	r2, [r4, #572]
	str	r2, [r3, #672]
	ldr	r3, [r7, #2184]
	ldr	r1, [r3, #636]
	ldr	r2, [r3, #672]
	add	r2, r2, r1
	str	r2, [r3, #600]
	str	r2, [r3, #44]
	add	r1, r4, #16
	ldr	r2, [r7, #2184]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r7, #2184]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r7, #2184]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	orreq	r2, r2, #2
	strb	r2, [r3, #4]
	mov	r2, #504
	ldr	lr, [r7, #2184]
	add	r0, lr, #72
	ldr	ip, [lr, #80]
	ldr	r3, [lr, #84]
	ldrd	r8, [lr, #88]
	str	ip, [fp, #-60]
	str	r3, [fp, #-56]
	strd	r8, [fp, #-52]
	ldrd	r8, [lr, #96]
	bl	memcpy
	ldr	r3, [fp, #-56]
	ldr	ip, [fp, #-60]
	ldr	r0, [r4, #24]
	ldr	r1, [r4, #28]
	and	r2, ip, r3
	adds	r2, r2, #1
	and	r1, r1, r0
	movne	r2, #1
	cmn	r1, #1
	ldrd	r0, [fp, #-52]
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r7, #2184]
	strne	ip, [r2, #80]
	strne	r3, [r2, #84]
	ldrd	r2, [r4, #32]
	ldr	ip, [r7, #2184]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r3, r1
	movhi	r2, r0
	strd	r2, [ip, #88]
	ldrd	r2, [r4, #40]
	ldr	r1, [r7, #2184]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r3, r9
	movhi	r2, r8
	strd	r2, [r1, #96]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3642
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3642
.L3643:
	add	r10, r10, #45056
	ldr	r2, [r10, #2184]
	ldrb	r3, [r2, #651]	@ zero_extendqisi2
	ldrb	r1, [r2, #615]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r10, #2184]
	ldr	r2, [r4, #568]
	str	r2, [r3, #668]
	ldr	r2, [r10, #2184]
	ldr	r3, [r2, #668]
	ldr	r1, [r2, #632]
	cmp	r3, r1
	movge	r3, r1
	str	r3, [r2, #36]
	ldr	r0, [r10, #2184]
	bl	MVC_CombineFldsToFrm
	ldr	r2, [r10, #2184]
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	movw	r3, #28282
	movt	r3, 42
	strb	r1, [r2, #13]
	ldrb	r1, [r4, #10]	@ zero_extendqisi2
	ldr	r2, [r10, #2184]
	strb	r1, [r2, #11]
	ldr	r2, [r10, #2184]
	ldr	r1, [r2, #52]
	ldr	r2, [r2, #664]
	add	r3, r1, r3
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3629
.L3609:
	add	r3, r0, r1, lsl #2
	ldr	r2, [r4, #520]
	add	r10, r3, #11075584
	mov	r3, #3
	add	r7, r10, #45056
	str	r2, [r7, #2184]
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	strb	r1, [r2, #1]
	ldr	r2, [r7, #2184]
	strb	r3, [r2, #2]
	ldr	r1, [r4, #520]
	ldr	r2, [r7, #2184]
	ldrb	r1, [r1, #5]	@ zero_extendqisi2
	strb	r1, [r2, #5]
	ldr	r1, [r4, #520]
	ldr	r2, [r7, #2184]
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	strb	r1, [r2, #7]
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	ldr	r2, [r7, #2184]
	cmp	r1, #0
	beq	.L3705
.L3611:
	strb	r3, [r2, #3]
	add	r1, r4, #16
	ldr	r3, [r7, #2184]
	mov	r2, #504
	ldr	r0, [r4, #596]
	mov	r9, #0
	str	r0, [r3, #48]
	ldr	r0, [r7, #2184]
	add	r0, r0, #72
	bl	memcpy
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #572]
	add	r3, r10, #45056
	mov	lr, #1
	ldr	r10, .L3707
	mov	r2, r9
	mov	r0, r5
	str	ip, [r1, #600]
	str	ip, [r1, #44]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #520]
	str	ip, [r1, #652]
	str	ip, [r1, #616]
	str	ip, [r1, #580]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #532]
	str	ip, [r1, #28]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #528]
	str	ip, [r1, #20]
	ldr	r1, [r7, #2184]
	strb	r9, [r1, #576]
	ldr	r1, [r7, #2184]
	ldrb	ip, [r4, #4]	@ zero_extendqisi2
	strb	ip, [r1, #577]
	ldr	r1, [r7, #2184]
	ldrb	ip, [r4, #5]	@ zero_extendqisi2
	strb	ip, [r1, #578]
	ldr	r1, [r7, #2184]
	ldrb	ip, [r4, #12]	@ zero_extendqisi2
	strb	ip, [r1, #579]
	strb	ip, [r1]
	ldr	r1, [r7, #2184]
	ldrb	ip, [r4, #9]	@ zero_extendqisi2
	str	ip, [r1, #40]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #540]
	str	ip, [r1, #32]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #544]
	str	ip, [r1, #592]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #548]
	str	ip, [r1, #628]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #552]
	str	ip, [r1, #664]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #556]
	str	ip, [r1, #36]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #560]
	str	ip, [r1, #596]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #564]
	str	ip, [r1, #632]
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #568]
	str	ip, [r1, #668]
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	ldr	ip, [r7, #2184]
	cmp	r1, lr
	moveq	r8, #3
	strb	r8, [ip, #4]
	ldr	r1, [r3, #2184]
	ldr	ip, [r4, #600]
	str	ip, [r1, #56]
	ldr	r1, [r3, #2184]
	ldr	ip, [r4, #604]
	str	ip, [r1, #60]
	ldr	r1, [r3, #2184]
	ldrb	ip, [r4, #11]	@ zero_extendqisi2
	strb	ip, [r1, #13]
	strb	ip, [r1, #12]
	ldrb	r7, [r4, #10]	@ zero_extendqisi2
	ldr	ip, [r3, #2184]
	ldr	r1, [r10, #-1884]
	strb	r7, [ip, #11]
	add	r8, r1, lr
	strb	r7, [ip, #10]
	ldr	ip, [r3, #2184]
	str	r8, [r10, #-1884]
	str	r1, [ip, #64]
	ldr	r3, [r3, #2184]
	ldr	r1, [r3, #580]
	ldrb	r10, [r3, #577]	@ zero_extendqisi2
	ldrb	r8, [r3, #578]	@ zero_extendqisi2
	ldr	r7, [r3, #600]
	ldrb	ip, [r3, #579]	@ zero_extendqisi2
	strb	lr, [r3, #612]
	mov	lr, #2
	strb	r10, [r3, #613]
	strb	lr, [r3, #648]
	strb	r10, [r3, #649]
	strb	r8, [r3, #614]
	strb	r8, [r3, #650]
	str	r7, [r3, #636]
	str	r7, [r3, #672]
	strb	ip, [r3, #615]
	strb	ip, [r3, #651]
	str	r1, [r3, #616]
	str	r1, [r3, #652]
	ldr	r1, [r4, #520]
	bl	MVC_GetAPC
	cmp	r0, #0
	bne	.L3706
.L3629:
	movw	r3, #28194
	movt	r3, 42
	add	r3, r6, r3
	ldr	r3, [r5, r3, asl #2]
	ldrb	r0, [r3, #3]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L3614
	ldr	r0, [r5, #120]
	mov	r2, #1
	ldrsb	r1, [r3, #6]
	bl	FSP_SetRef
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3608:
	ldr	r3, [r4, #520]
	mov	r2, #3
	strb	r2, [r3, #576]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #0
	mov	r3, r1, asl #2
	beq	.L3615
	add	r10, r0, r3
	add	r10, r10, #11075584
	add	r7, r10, #45056
	ldr	r2, [r7, #2184]
	cmp	r2, #0
	beq	.L3615
	strb	r8, [r2, #612]
	ldr	r3, [r7, #2184]
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strb	r2, [r3, #615]
	ldr	r3, [r7, #2184]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	strb	r2, [r3, #613]
	ldr	r3, [r7, #2184]
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	strb	r2, [r3, #614]
	ldr	r3, [r7, #2184]
	ldr	r2, [r4, #548]
	str	r2, [r3, #628]
	ldr	r3, [r7, #2184]
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strb	r2, [r3, #615]
	ldr	r3, [r7, #2184]
	ldr	r2, [r4, #572]
	str	r2, [r3, #636]
	ldr	r3, [r7, #2184]
	ldr	r1, [r3, #636]
	ldr	r2, [r3, #672]
	add	r2, r2, r1
	str	r2, [r3, #600]
	str	r2, [r3, #44]
	add	r1, r4, #16
	ldr	r2, [r7, #2184]
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cmp	r3, #0
	ldrneb	r3, [r4, #2]	@ zero_extendqisi2
	strb	r3, [r2, #1]
	ldr	r2, [r7, #2184]
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #2]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r7, #2184]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	orreq	r2, r2, #1
	strb	r2, [r3, #4]
	mov	r2, #504
	ldr	lr, [r7, #2184]
	add	r0, lr, #72
	ldr	ip, [lr, #80]
	ldr	r3, [lr, #84]
	ldrd	r8, [lr, #88]
	str	ip, [fp, #-60]
	str	r3, [fp, #-56]
	strd	r8, [fp, #-52]
	ldrd	r8, [lr, #96]
	bl	memcpy
	ldr	r3, [fp, #-56]
	ldr	ip, [fp, #-60]
	ldr	r0, [r4, #24]
	ldr	r1, [r4, #28]
	and	r2, ip, r3
	adds	r2, r2, #1
	and	r1, r1, r0
	movne	r2, #1
	cmn	r1, #1
	ldrd	r0, [fp, #-52]
	movne	r2, #0
	cmp	r2, #0
	ldrne	r2, [r7, #2184]
	strne	ip, [r2, #80]
	strne	r3, [r2, #84]
	ldrd	r2, [r4, #32]
	ldr	ip, [r7, #2184]
	cmp	r3, r1
	cmpeq	r2, r0
	movhi	r3, r1
	movhi	r2, r0
	strd	r2, [ip, #88]
	ldrd	r2, [r4, #40]
	ldr	r1, [r7, #2184]
	cmp	r3, r9
	cmpeq	r2, r8
	movhi	r3, r9
	movhi	r2, r8
	strd	r2, [r1, #96]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3626
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3626
.L3627:
	add	r10, r10, #45056
	ldr	r2, [r10, #2184]
	ldrb	r3, [r2, #651]	@ zero_extendqisi2
	ldrb	r1, [r2, #615]	@ zero_extendqisi2
	cmp	r3, r1
	movcc	r3, r1
	strb	r3, [r2]
	ldr	r3, [r10, #2184]
	ldr	r2, [r4, #564]
	str	r2, [r3, #632]
	ldr	r2, [r10, #2184]
	ldr	r3, [r2, #668]
	ldr	r1, [r2, #632]
	cmp	r3, r1
	movge	r3, r1
	str	r3, [r2, #36]
	ldr	r3, [r10, #2184]
	ldrb	r2, [r4, #11]	@ zero_extendqisi2
	strb	r2, [r3, #12]
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	ldr	r3, [r10, #2184]
	strb	r2, [r3, #10]
	ldr	r0, [r10, #2184]
	bl	MVC_CombineFldsToFrm
	ldr	r2, [r10, #2184]
	movw	r3, #28266
	movt	r3, 42
	ldr	r1, [r2, #52]
	ldr	r2, [r2, #628]
	add	r3, r1, r3
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #4]
	b	.L3629
.L3615:
	add	r7, r5, r3
	ldr	r2, [r4, #520]
	add	r7, r7, #11075584
	mov	r1, #1
	add	r3, r7, #45056
	str	r2, [r3, #2184]
	strb	r1, [r2, #612]
	ldr	r2, [r3, #2184]
	ldr	r0, [r4, #520]
	str	r0, [r2, #616]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	strb	r0, [r2, #613]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #5]	@ zero_extendqisi2
	strb	r0, [r2, #614]
	ldr	r2, [r3, #2184]
	ldr	r0, [r4, #548]
	str	r0, [r2, #628]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #12]	@ zero_extendqisi2
	strb	r0, [r2, #615]
	ldr	r2, [r3, #2184]
	ldr	r0, [r4, #572]
	str	r0, [r2, #636]
	ldr	r2, [r3, #2184]
	ldr	r0, [r2, #636]
	str	r0, [r2, #600]
	str	r0, [r2, #44]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #2]	@ zero_extendqisi2
	strb	r0, [r2, #1]
	ldr	r2, [r3, #2184]
	strb	r1, [r2, #2]
	ldr	r1, [r4, #520]
	ldr	r2, [r3, #2184]
	ldrb	r1, [r1, #5]	@ zero_extendqisi2
	strb	r1, [r2, #5]
	ldr	r1, [r4, #520]
	ldr	r2, [r3, #2184]
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	strb	r1, [r2, #7]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3617
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3617
.L3618:
	add	r7, r7, #45056
	mov	r8, #0
	mov	r2, #504
	add	r1, r4, #16
	ldr	r3, [r7, #2184]
	str	r8, [r3, #40]
	ldr	r0, [r7, #2184]
	add	r0, r0, #72
	bl	memcpy
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #596]
	mov	r2, #1
	ldr	r3, .L3707
	mov	r0, r5
	str	ip, [r1, #48]
	ldr	ip, [r7, #2184]
	ldr	lr, [r4, #528]
	ldr	r1, [r3, #-1884]
	str	lr, [ip, #20]
	add	ip, r1, r2
	ldr	lr, [r7, #2184]
	ldr	r9, [r4, #540]
	str	r9, [lr, #32]
	ldr	lr, [r7, #2184]
	ldr	r9, [r4, #556]
	str	r9, [lr, #36]
	ldr	lr, [r7, #2184]
	ldrb	r9, [r4, #12]	@ zero_extendqisi2
	strb	r9, [lr]
	ldr	lr, [r7, #2184]
	ldr	r9, [r4, #564]
	str	r9, [lr, #632]
	ldr	lr, [r7, #2184]
	ldr	r9, [r4, #600]
	str	r9, [lr, #56]
	ldr	lr, [r7, #2184]
	ldr	r9, [r4, #604]
	str	r9, [lr, #60]
	ldr	lr, [r7, #2184]
	ldrb	r9, [r4, #11]	@ zero_extendqisi2
	strb	r9, [lr, #12]
	ldr	lr, [r7, #2184]
	ldrb	r9, [r4, #10]	@ zero_extendqisi2
	strb	r9, [lr, #10]
	ldr	lr, [r7, #2184]
	str	r1, [lr, #64]
	str	ip, [r3, #-1884]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [r7, #2184]
	rsb	r3, r2, r3
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r1, #4]
	ldr	r1, [r4, #520]
	bl	MVC_GetAPC
	cmp	r0, #0
	beq	.L3629
	mov	r3, r0
	ldr	ip, .L3707+4
	mov	r0, r8
	movw	r2, #2277
	b	.L3703
.L3630:
	add	r7, r5, r3
	ldr	r2, [r4, #520]
	add	r7, r7, #11075584
	mov	r1, #2
	add	r3, r7, #45056
	str	r2, [r3, #2184]
	strb	r1, [r2, #648]
	ldr	r2, [r3, #2184]
	ldr	r0, [r4, #520]
	str	r0, [r2, #652]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	strb	r0, [r2, #649]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #5]	@ zero_extendqisi2
	strb	r0, [r2, #650]
	ldr	r2, [r3, #2184]
	ldr	r0, [r4, #552]
	str	r0, [r2, #664]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #12]	@ zero_extendqisi2
	strb	r0, [r2, #651]
	ldr	r2, [r3, #2184]
	ldr	r0, [r4, #572]
	str	r0, [r2, #672]
	ldr	r2, [r3, #2184]
	ldr	r0, [r2, #636]
	str	r0, [r2, #600]
	str	r0, [r2, #44]
	ldr	r2, [r3, #2184]
	ldrb	r0, [r4, #2]	@ zero_extendqisi2
	strb	r0, [r2, #1]
	ldr	r2, [r3, #2184]
	strb	r1, [r2, #2]
	ldr	r1, [r4, #520]
	ldr	r2, [r3, #2184]
	ldrb	r1, [r1, #5]	@ zero_extendqisi2
	strb	r1, [r2, #5]
	ldr	r1, [r4, #520]
	ldr	r2, [r3, #2184]
	ldrb	r1, [r1, #7]	@ zero_extendqisi2
	strb	r1, [r2, #7]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3632
	ldrb	r2, [r4, #5]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L3632
.L3633:
	add	r7, r7, #45056
	mov	r9, #0
	mov	r2, #504
	add	r1, r4, #16
	ldr	r3, [r7, #2184]
	str	r9, [r3, #40]
	ldr	r0, [r7, #2184]
	add	r0, r0, #72
	bl	memcpy
	ldr	r1, [r7, #2184]
	ldr	ip, [r4, #596]
	mov	r2, #2
	ldr	r3, .L3707
	mov	r0, r5
	str	ip, [r1, #48]
	ldr	ip, [r7, #2184]
	ldr	lr, [r4, #528]
	ldr	r1, [r3, #-1884]
	str	lr, [ip, #20]
	add	r8, r1, #1
	ldr	ip, [r7, #2184]
	ldr	lr, [r4, #540]
	str	lr, [ip, #32]
	ldr	ip, [r7, #2184]
	ldr	lr, [r4, #556]
	str	lr, [ip, #36]
	ldr	ip, [r7, #2184]
	ldrb	lr, [r4, #12]	@ zero_extendqisi2
	strb	lr, [ip, #651]
	strb	lr, [ip]
	ldr	ip, [r7, #2184]
	ldr	lr, [r4, #568]
	str	lr, [ip, #668]
	ldr	ip, [r7, #2184]
	ldr	lr, [r4, #600]
	str	lr, [ip, #56]
	ldr	ip, [r7, #2184]
	ldr	lr, [r4, #604]
	str	lr, [ip, #60]
	ldr	ip, [r7, #2184]
	ldrb	lr, [r4, #11]	@ zero_extendqisi2
	strb	lr, [ip, #13]
	ldr	ip, [r7, #2184]
	ldrb	lr, [r4, #10]	@ zero_extendqisi2
	strb	lr, [ip, #11]
	ldr	ip, [r7, #2184]
	str	r1, [ip, #64]
	ldrb	ip, [r4, #7]	@ zero_extendqisi2
	ldr	r1, [r7, #2184]
	cmp	ip, #1
	str	r8, [r3, #-1884]
	moveq	r8, r2
	movne	r8, r9
	strb	r8, [r1, #4]
	ldr	r1, [r4, #520]
	bl	MVC_GetAPC
	cmp	r0, #0
	beq	.L3629
	ldr	ip, .L3707+4
	mov	r3, r0
	movw	r2, #2402
	mov	r0, r9
.L3703:
	ldr	r1, .L3707+12
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r5
	mvn	r1, #0
	bl	MVC_ClearDPB
	mvn	r0, #0
	b	.L3614
.L3705:
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r3, #3
	b	.L3611
.L3617:
	ldr	r2, [r3, #2184]
	mov	r1, #1
	strb	r1, [r2, #3]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, r1
	ldreq	r3, [r3, #2184]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #28]
	b	.L3618
.L3632:
	ldr	r2, [r3, #2184]
	mov	r1, #2
	strb	r1, [r2, #3]
	ldrb	r2, [r4, #4]	@ zero_extendqisi2
	cmp	r2, #1
	ldreq	r3, [r3, #2184]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #28]
	b	.L3633
.L3626:
	ldr	r2, [r7, #2184]
	ldrb	r3, [r2, #3]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r2, #3]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r7, #2184]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #28]
	b	.L3627
.L3642:
	ldr	r2, [r7, #2184]
	ldrb	r3, [r2, #3]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r2, #3]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #1
	ldreq	r3, [r7, #2184]
	ldreq	r2, [r4, #532]
	streq	r2, [r3, #28]
	b	.L3643
.L3706:
	mov	r3, r0
	ldr	ip, .L3707+4
	mov	r0, r9
	movw	r2, #2214
	b	.L3703
.L3708:
	.align	2
.L3707:
	.word	.LANCHOR3
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC414
	.word	.LC413
	UNWIND(.fnend)
	.size	MVC_InsertFrmInDPB, .-MVC_InsertFrmInDPB
	.align	2
	.global	MVC_AllocFrameStore
	.type	MVC_AllocFrameStore, %function
MVC_AllocFrameStore:
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
	add	r9, r0, #11075584
	add	r7, r9, #40960
	mov	r6, r0
	str	r1, [fp, #-52]
	ldrb	r5, [r7, #521]	@ zero_extendqisi2
	cmp	r5, #0
	beq	.L3710
	ldrb	r3, [r7, #522]	@ zero_extendqisi2
	cmp	r3, #0
	movne	r5, #2
	moveq	r5, #1
.L3710:
	ldr	r3, [fp, #-52]
	cmp	r3, #1
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	beq	.L3711
	cmp	r3, #2
	add	r4, r6, #11141120
	beq	.L3790
	sub	r3, r5, #1
	add	r4, r4, #8192
	cmp	r3, #1
	mov	r3, #0
	strb	r3, [r4, #1593]
	bls	.L3794
.L3733:
	mov	r8, #0
	strb	r8, [r4, #1592]
	ldr	r10, [r6, #48]
	cmp	r10, r8
	beq	.L3740
	movw	ip, #47800
	add	r9, r9, #45056
	movt	ip, 169
	add	ip, r6, ip
	movw	r3, #47240
	movt	r3, 169
	add	r3, r6, r3
	str	r3, [fp, #-48]
.L3739:
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3735
	cmp	ip, #0
	beq	.L3752
	ldr	lr, [r9, #2376]
	cmp	lr, #0
	beq	.L3752
	ldr	r2, [r9, #2184]
	rsb	r3, ip, r2
	cmp	r2, #0
	clz	r3, r3
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L3735
	ldr	r1, [fp, #-48]
	b	.L3737
.L3738:
	ldr	r2, [r1, #4]!
	rsb	r0, ip, r2
	cmp	r2, #0
	clz	r0, r0
	mov	r0, r0, lsr #5
	moveq	r0, #0
	cmp	r0, #0
	bne	.L3735
.L3737:
	add	r3, r3, #1
	cmp	r3, lr
	bne	.L3738
.L3752:
	mov	r3, #688
	movw	r2, #47840
	mla	r8, r3, r8, r6
	movt	r2, 169
	add	r0, r4, #1600
	mov	r9, #0
	mov	r3, #1
	str	ip, [r4, #2112]
	add	r2, r8, r2
	add	r1, r8, #11075584
	strb	r3, [r4, #1592]
	mov	r10, ip
	str	r9, [r2, #4]
	mov	ip, #101
	mov	r2, #504
	add	r1, r1, #47872
	add	r0, r0, #8
	str	ip, [r4, #2164]
	str	r3, [fp, #-48]
	bl	memcpy
	movw	r2, #47800
	str	r9, [r4, #1808]
	movt	r2, 169
	add	r2, r8, r2
	ldr	r3, [fp, #-48]
	strb	r9, [r2, #4]
	strb	r9, [r10, #7]
	ldr	r2, [r4, #2112]
	strb	r9, [r2, #3]
	ldr	r2, [r4, #2112]
	strb	r3, [r2, #5]
	ldr	r3, [r4, #2112]
	str	r9, [r3, #16]
	ldrb	r8, [r4, #1592]	@ zero_extendqisi2
	cmp	r8, #1
	bne	.L3740
	ldr	r3, [fp, #-52]
	ldr	r0, [r6, #120]
	clz	r1, r3
	mov	r1, r1, lsr #5
	bl	FSP_NewLogicFs
	mov	r9, r0
	mov	r1, r0
	ldr	r0, [r6, #120]
	bl	FSP_GetLogicFs
	mvn	r3, r9
	mov	r3, r3, lsr #31
	cmp	r0, #0
	mov	r10, r0
	moveq	r3, #0
	cmp	r3, #0
	beq	.L3741
	ldr	r3, [r6, #224]
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L3795
.L3742:
	ldr	r8, .L3803
	mov	r2, #504
	ldr	r0, [r4, #2112]
	add	r1, r10, #8
	ldr	r3, [r8, #52]
	add	r0, r0, #72
	blx	r3
	ldr	r1, [r4, #2112]
	movw	r0, #9800
	ldr	r3, [r8, #52]
	movt	r0, 170
	add	r1, r1, #72
	add	r0, r6, r0
	mov	r2, #504
	blx	r3
	ldr	r3, [r4, #2112]
	strb	r9, [r3, #6]
	ldrsb	r3, [r10, #4]
	str	r3, [r4, #2188]
.L3732:
	ldr	r3, [fp, #-52]
	uxtb	r2, r5
	strb	r2, [r4, #1595]
	cmp	r3, #1
	moveq	r0, #0
	beq	.L3786
	ldrb	r3, [r7, #521]	@ zero_extendqisi2
	ldr	r1, [r4, #2112]
	cmp	r3, #0
	movne	r3, #3
	strb	r3, [r1, #576]
	ldrb	r3, [r4, #1593]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3796
	strb	r2, [r4, #2808]
	cmp	r5, #0
	ldr	r3, [r7, #548]
	movne	r0, #0
	moveq	r0, r5
	str	r3, [r4, #2816]
	ldrb	r3, [r7, #528]	@ zero_extendqisi2
	str	r3, [r4, #2812]
	ldr	r3, [r6, #128]
	str	r3, [r4, #2820]
	ldr	r3, [r7, #2160]
	str	r9, [r4, #2824]
	streqb	r5, [r4, #2809]
	str	r3, [r4, #2832]
	movne	r3, #1
	strneb	r3, [r4, #2809]
.L3786:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3735:
	add	r8, r8, #1
	add	ip, ip, #688
	cmp	r8, r10
	bne	.L3739
.L3740:
	ldr	r3, .L3803
	mov	r0, #0
	ldr	r1, .L3803+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r6
	mov	r1, #1
	bl	MVC_ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3797:
	add	r4, r6, #11141120
	mov	r5, #0
.L3790:
	add	r4, r4, #8192
.L3730:
	mov	r3, #0
	strb	r3, [r4, #1593]
	b	.L3733
.L3711:
	cmp	r3, #2
	beq	.L3797
	add	r4, r6, #11141120
	mov	r3, #0
	add	r4, r4, #8192
	mov	r5, r3
	strb	r3, [r4, #1593]
	b	.L3733
.L3795:
	ldr	r3, [r6, #16]
	ldr	r8, [r0, #200]
	ldr	r2, [r6, #12]
	mov	r3, r3, asl #4
	ldr	r0, [r0, #112]
	str	r3, [r10, #176]
	mul	r8, r8, r3
	mov	r3, r2, asl #4
	str	r3, [r10, #172]
	add	r0, r8, r0
	str	r0, [r10, #116]
	bl	MEM_Phy2Vir
	ldr	r3, [r10, #128]
	ldr	r1, [r10, #96]
	add	r3, r8, r3
	ldr	r2, [r10, #200]
	add	r8, r8, r1
	str	r3, [r10, #132]
	str	r8, [r10, #100]
	add	r8, r8, r2
	str	r8, [r10, #108]
	str	r0, [r10, #44]
	mov	r0, r3
	bl	MEM_Phy2Vir
	str	r0, [r10, #60]
	b	.L3742
.L3794:
	ldrb	r3, [r4, #2808]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L3714
	ldr	r1, [r4, #2832]
	ldr	r2, [r7, #2160]
	cmp	r1, r2
	beq	.L3715
.L3714:
	add	r3, r9, #45056
	ldr	r1, [r3, #2376]
	subs	r3, r1, #1
	bmi	.L3733
	movw	r2, #28194
	movt	r2, 42
	add	r2, r1, r2
	add	r2, r6, r2, lsl #2
	b	.L3726
.L3717:
	cmp	r5, #2
	beq	.L3798
.L3716:
	subs	r3, r3, #1
	bmi	.L3733
.L3726:
	ldr	r8, [r2, #-4]!
	cmp	r8, #0
	beq	.L3716
	cmp	r5, #1
	bne	.L3717
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #2
	bne	.L3716
.L3718:
	ldr	r0, [r7, #548]
	ldr	r1, [r8, #20]
	cmp	r0, r1
	bne	.L3716
	ldr	r0, [r7, #2160]
	ldr	r1, [r8, #56]
	cmp	r0, r1
	bne	.L3716
	ldrb	r1, [r7, #528]	@ zero_extendqisi2
	cmp	r1, #0
	ldrb	r1, [r8, #3]	@ zero_extendqisi2
	beq	.L3799
	cmp	r1, #0
	beq	.L3716
.L3722:
	add	r0, r4, #1600
	mov	r3, #1
	mov	r2, #504
	strb	r3, [r4, #1593]
	add	r1, r8, #72
	str	r8, [r4, #2112]
	add	r0, r0, #8
	bl	memcpy
	ldrsb	r1, [r8, #6]
	ldr	r0, [r6, #120]
	bl	FSP_GetLogicFs
	subs	r3, r0, #0
	beq	.L3800
	ldrb	r2, [r4, #1593]	@ zero_extendqisi2
	ldrsb	r3, [r3, #4]
	cmp	r2, #0
	str	r3, [r4, #2188]
	beq	.L3733
	ldr	r3, [r4, #2112]
	cmp	r3, #0
	beq	.L3733
.L3791:
	mov	r9, #0
	b	.L3732
.L3798:
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	cmp	r1, #1
	bne	.L3716
	b	.L3718
.L3796:
	mov	r3, #0
	strb	r3, [r4, #2808]
	ldr	r2, [r7, #548]
	mov	r0, r3
	str	r2, [r4, #2816]
	ldrb	r2, [r7, #528]	@ zero_extendqisi2
	str	r2, [r4, #2812]
	ldr	r2, [r6, #128]
	strb	r3, [r4, #2809]
	str	r9, [r4, #2824]
	str	r2, [r4, #2820]
	ldr	r3, [r7, #2160]
	str	r3, [r4, #2832]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3799:
	cmp	r1, #0
	bne	.L3716
	b	.L3722
.L3715:
	sub	r2, r5, #2
	cmp	r3, #1
	cmpeq	r5, #2
	clz	r2, r2
	mov	r2, r2, lsr #5
	beq	.L3727
	cmp	r5, #1
	cmpeq	r3, #2
	bne	.L3733
.L3727:
	ldr	r1, [r4, #2816]
	ldr	r3, [r7, #548]
	cmp	r1, r3
	bne	.L3733
	ldrb	r3, [r7, #528]	@ zero_extendqisi2
	cmp	r3, #0
	ldr	r3, [r4, #2812]
	bne	.L3728
	cmp	r3, #0
	bne	.L3733
.L3729:
	ldr	r3, [r6, #128]
	ldr	r1, [r6, #124]
	rsb	r3, r1, r3
	cmp	r3, #2
	bhi	.L3733
	ldr	r3, [r4, #2112]
	mov	r1, #1
	strb	r1, [r4, #1593]
	cmp	r3, #0
	beq	.L3730
	cmp	r2, #0
	bne	.L3801
	cmp	r5, #1
	beq	.L3802
.L3750:
	mov	r9, r2
	b	.L3732
.L3741:
	ldr	ip, [r4, #2112]
	mov	r0, r3
	ldr	r2, .L3803
	strb	r3, [r4, #1592]
	ldr	r1, .L3803+8
	strb	r3, [ip, #5]
	ldr	r3, [r2, #68]
	blx	r3
	mov	r1, r8
	mov	r0, r6
	bl	MVC_ClearAll
	mvn	r0, #0
	b	.L3786
.L3800:
	ldr	r3, .L3803
	movw	r2, #6601
	ldr	r1, .L3803+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L3786
.L3802:
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L3730
	b	.L3750
.L3801:
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cmp	r3, r1
	bne	.L3730
	b	.L3791
.L3728:
	cmp	r3, #0
	beq	.L3733
	b	.L3729
.L3804:
	.align	2
.L3803:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC416
	.word	.LC417
	.word	.LC415
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
	ldr	r7, .L3817
	movw	r2, #12720
	ldr	r4, [r0, #928]
	mov	r6, r0
	mov	r5, r1
	movt	r2, 170
	mov	r1, #0
	ldr	r3, [r7, #48]
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
	ldreq	r3, [r5, #752]
	movne	r3, #32
	movne	r2, #256
	strne	r3, [r6, #36]
	movne	r3, #136
	streq	r3, [r6, #36]
	ldreq	r3, [r5, #756]
	streq	r3, [r6, #40]
	ldreq	r3, [r5, #744]
	str	r3, [r6, #32]
	strne	r2, [r6, #40]
	ldr	r0, [r5, #668]
	bl	MEM_Phy2Vir
	ldr	ip, [r6, #36]
	ldr	r3, [r6, #224]
	movw	r2, #3992
	ldr	lr, [r6, #40]
	mul	r2, r2, ip
	ldr	r3, [r3, #664]
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
	bne	.L3814
	ldr	r5, [r5, #688]
	cmp	r5, #0
	beq	.L3815
.L3810:
	add	r3, r6, #11075584
	movw	r2, #4060
	add	r1, r3, #36864
	add	r3, r3, #32768
	ldr	r9, [r7, #48]
	add	r4, r6, #12992
	str	lr, [r1, #1264]
	mov	r1, #0
	str	ip, [r3, #2088]
	mov	r5, r1
	ldr	r3, [r6, #32]
	add	r4, r4, #12
	movw	r8, #22860
	movt	r8, 164
	add	r8, r6, r8
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
	ldr	r3, [r7, #48]
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
	movt	r2, 5
	ldr	r3, [r7, #48]
	mov	r0, r8
	mov	r1, r5
	blx	r3
	mov	r3, r4
	mov	r2, r5
.L3811:
	strb	r2, [r3]
	add	ip, r3, #274432
	add	r3, r3, #335872
	mov	r4, #0
	add	r3, r3, #308
	strb	r2, [ip, #241]
	cmp	r3, r8
	bne	.L3811
	add	r2, r6, #12288
	movw	r3, #34112
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
	beq	.L3816
	mov	r1, r4
	mov	r0, r6
	bl	MVC_ClearAll
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3815:
	mov	r0, r3
	mov	r1, r5
	ldr	r3, [r7, #48]
	blx	r3
	ldr	r2, [r6, #40]
	mov	r1, r5
	ldr	r3, [r7, #48]
	ldr	r0, [r6, #252]
	mul	r2, r4, r2
	blx	r3
	ldr	lr, [r6, #40]
	ldr	ip, [r6, #36]
	ldr	r0, [r6, #544]
	b	.L3810
.L3814:
	ldr	ip, [r7, #68]
	movw	r3, #13886
	ldr	r2, .L3817+4
	mov	r0, #22
	ldr	r1, .L3817+8
	blx	ip
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3816:
	ldr	r3, [r7, #68]
	mov	r0, r4
	ldr	r1, .L3817+12
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L3818:
	.align	2
.L3817:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+332
	.word	.LC418
	.word	.LC419
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
	add	r6, r7, #8192
	mov	r8, #0
	strb	r8, [r2, #3480]
	mov	r5, r0
	ldrb	r3, [r6, #1595]	@ zero_extendqisi2
	sub	r3, r3, #2
	clz	r3, r3
	mov	r3, r3, lsr #5
	strb	r3, [r2, #3481]
	ldrb	r3, [r6, #1592]	@ zero_extendqisi2
	cmp	r3, r8
	beq	.L3986
	ldr	r3, [r0, #128]
	str	r3, [r0, #124]
	ldrb	r3, [r6, #1605]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3987
.L3822:
	ldrb	r0, [r5, #8]	@ zero_extendqisi2
	cmp	r0, #2
	beq	.L3988
	ldr	r8, [r5, #224]
	ldr	r3, [r8, #12]
	cmp	r3, #0
	ble	.L3989
.L3827:
	ldrb	r2, [r6, #1594]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L3990
.L3828:
	cmp	r3, #2
	ldr	r3, [r6, #2128]
	beq	.L3991
.L3853:
	cmp	r3, #0
	beq	.L3860
	ldr	r3, [r5, #112]
	ldr	r2, [r6, #2164]
	cmp	r3, r2
	bcs	.L3861
	ldrb	r1, [r6, #1594]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L3992
.L3861:
	mov	r0, r5
	bl	MVC_Marking
	cmp	r0, #0
	bne	.L3993
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	ldr	r1, [r6, #2192]
	mov	r0, r5
	bl	MVC_RemoveUnUsedFrameStore
.L3854:
	ldrb	r3, [r6, #1593]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3864
	add	r4, r4, #45056
	ldr	lr, [r4, #2376]
.L3865:
	ldr	r3, [r6, #2128]
	cmp	r3, #0
	beq	.L3874
	ldr	r1, [r4, #2384]
	ldr	r2, [r4, #2388]
	add	r3, r2, r1
	cmp	r3, lr
	bcs	.L3899
.L3874:
	mvn	r9, #0
	mov	r8, r9
	b	.L3883
.L3878:
	ldr	r3, [r6, #2128]
	mov	r1, r8
	mov	r0, r5
	mov	r9, r2
	cmp	r3, #0
	bne	.L3879
	ldr	r2, [r6, #2132]
	ldr	r3, [fp, #-56]
	cmp	r2, r3
	blt	.L3994
.L3879:
	bl	MVC_OutputFrmFromDPB
	cmn	r0, #1
	mov	r10, r0
	beq	.L3882
	ldr	lr, [r4, #2376]
.L3883:
	ldr	ip, [r4, #2380]
	sub	r3, fp, #52
	sub	r2, fp, #56
	mvn	r1, #0
	cmp	ip, lr
	mov	r0, r5
	bcc	.L3876
	bl	MVC_GetMinPOC
	cmp	r0, #0
	blt	.L3995
	ldr	r2, [r4, #2380]
	cmp	r2, r9
	ldrne	r8, [fp, #-52]
	bne	.L3878
	ldr	r3, [fp, #-52]
	cmp	r3, r8
	beq	.L3996
	mov	r8, r3
	b	.L3878
.L3987:
	ldr	r3, [r6, #2116]
	ldrb	r10, [r3, #1]	@ zero_extendqisi2
	cmp	r10, #0
	beq	.L3823
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L3822
	ldr	r9, .L4012
	mov	r0, r8
	ldr	r1, .L4012+4
	ldr	r3, [r9, #68]
	blx	r3
	ldr	r4, [r9, #68]
	movw	r3, #3601
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L3990:
	ldr	r2, [r6, #2112]
	ldrb	r1, [r6, #1595]	@ zero_extendqisi2
	str	r2, [fp, #-64]
	sub	r2, r1, #1
	cmp	r2, #1
	movhi	r2, #0
	movls	r2, #1
	cmp	r1, #0
	beq	.L3829
	ldr	r0, [fp, #-64]
	ldrb	r0, [r0, #2]	@ zero_extendqisi2
	cmp	r0, #0
	moveq	ip, r2
	orrne	ip, r2, #1
	cmp	ip, #0
	beq	.L3828
	cmp	r2, #0
	beq	.L3997
.L3902:
	ldr	r2, [r5, #220]
	cmp	r2, #0
	bne	.L3832
	add	r1, r6, #1584
	add	r0, r6, #2192
	mov	r3, #1
	add	r1, r1, #8
	str	r3, [r5, #220]
	add	r0, r0, #8
	mov	r2, #608
	bl	memcpy
	ldr	r3, [r8, #12]
	b	.L3828
.L3860:
	ldrb	r3, [r6, #1593]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3864
	add	r4, r4, #45056
	ldr	lr, [r4, #2376]
	b	.L3874
.L3829:
	cmp	r2, #0
	bne	.L3902
.L3835:
	cmp	r3, #0
	ble	.L3905
	movw	r9, #9784
	ldr	r8, [fp, #-64]
	movt	r9, 170
	add	r9, r5, r9
.L3838:
	cmp	r1, #0
	beq	.L3840
	ldrb	r3, [r9, #1]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L3998
.L3840:
	ldr	r3, [r9, #572]
	str	r3, [r8, #44]
.L3841:
	mov	r1, r8
	mov	r0, r5
	bl	MVC_GetImagePara
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_GetFsImagePtr
	subs	r10, r0, #0
	beq	.L3999
	ldrd	r2, [r9, #24]
	strd	r2, [r10, #8]
	ldrd	r2, [r9, #32]
	strd	r2, [r10, #16]
	ldrd	r2, [r9, #40]
	strd	r2, [r10, #24]
	ldr	r2, [r8, #44]
	cmp	r2, #0
	str	r2, [r10, #200]
	beq	.L3848
	ldr	r3, [r10, #160]
	ands	r3, r3, #3
	beq	.L4000
.L3846:
	ldr	r3, [r5, #224]
	ldr	r3, [r3, #4]
	cmp	r3, r2
	bcc	.L4001
.L3848:
	mov	r2, #1
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
	ldr	r0, [r5, #120]
	add	r3, r5, #584
	str	r10, [sp]
	mov	r2, r5
	mov	r1, #16
	bl	InsertImgToVoQueue
	cmp	r0, #1
	beq	.L3845
	mov	r2, #0
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
.L3845:
	ldr	r3, [r5, #224]
	ldr	r3, [r3, #684]
	add	r3, r3, #2032
	add	r3, r3, #15
	cmp	r3, #4096
	movcc	r3, #0
	strcc	r3, [r10, #84]
	bcs	.L4002
.L3851:
	ldr	r3, [r5, #136]
	add	r3, r3, #2
	str	r3, [r5, #136]
.L3839:
	ldr	r3, [r5, #220]
	cmp	r3, #2
	beq	.L3852
.L3837:
	ldr	r2, [fp, #-64]
	mov	r3, #1
	str	r3, [r8, #16]
	strb	r3, [r2, #8]
.L3852:
	ldr	r3, [r5, #224]
	mov	r2, #2
	str	r2, [r5, #220]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	ldr	r3, [r6, #2128]
	bne	.L3853
.L3991:
	cmp	r3, #0
	beq	.L3854
	add	r4, r4, #45056
	mov	r0, r5
	bl	MVC_SimpleSlideDPB
	ldr	r1, [r4, #2376]
	cmp	r1, #0
	beq	.L3855
	ldr	r3, [r4, #2184]
	cmp	r3, #0
	beq	.L3907
	movw	r2, #47240
	mov	r3, #0
	movt	r2, 169
	add	r2, r5, r2
	b	.L3856
.L3857:
	ldr	r0, [r2, #4]!
	cmp	r0, #0
	beq	.L3907
.L3856:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3857
.L3855:
	movw	r2, #9784
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	subs	r8, r0, #0
	bne	.L4003
	ldr	r1, [r4, #2380]
	mov	r0, r5
	add	r1, r1, #1
	str	r1, [r4, #2380]
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	ldr	ip, .L4012
	mov	r2, r8
	movw	r3, #3782
	ldr	r1, .L4012+8
	mov	r0, #14
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3859
.L3864:
	ldr	r3, [r5, #520]
	ldr	ip, [r6, #2112]
	cmp	r3, #0
	rsb	r2, r3, ip
	clz	r2, r2
	mov	r2, r2, lsr #5
	moveq	r2, #0
	cmp	r2, #0
	beq	.L4004
	mov	r0, r5
	ldr	r9, .L4012
	bl	MVC_DirectOutput
	cmn	r0, #1
	mov	r4, r0
	beq	.L4005
.L3870:
	ldr	r8, [r9, #68]
	movw	r3, #3834
	mov	r2, r4
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r8
	b	.L3826
.L3995:
	ldr	r3, .L4012
	movw	r2, #3884
	ldr	r1, .L4012+12
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
.L3876:
	ldr	r3, [r6, #2128]
	cmp	r3, #0
	beq	.L3888
	ldrb	r3, [r6, #1596]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3888
	ldr	r2, [r4, #2384]
	cmp	r2, #0
	beq	.L3888
	movw	ip, #47300
	mov	lr, r3
	movt	ip, 169
	add	ip, r5, ip
	b	.L3893
.L3890:
	ldr	r2, [r4, #2384]
	cmp	r2, r3
	bls	.L3888
.L3893:
	ldr	r2, [ip, #4]!
	add	r3, r3, #1
	ldr	r1, [r6, #2120]
	ldr	r0, [r2, #20]
	cmp	r0, r1
	bne	.L3890
	ldr	r0, [r2, #56]
	ldr	r1, [r6, #2192]
	cmp	r0, r1
	bne	.L3890
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
	b	.L3890
.L3888:
	ldr	r1, [r4, #2376]
	cmp	r1, #0
	beq	.L3887
	ldr	r3, [r4, #2184]
	cmp	r3, #0
	beq	.L3911
	movw	r2, #47240
	mov	r3, #0
	movt	r2, 169
	add	r2, r5, r2
	b	.L3894
.L3895:
	ldr	r0, [r2, #4]!
	cmp	r0, #0
	beq	.L3911
.L3894:
	add	r3, r3, #1
	cmp	r3, r1
	bne	.L3895
.L3887:
	movw	r2, #9784
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	cmp	r0, #0
	bne	.L4006
	ldr	r3, [r4, #2380]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #2380]
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
.L3859:
	mov	r4, #0
.L3897:
	add	r7, r7, #8192
	mov	r3, #0
	mov	r0, r4
	strb	r3, [r7, #1592]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L3823:
	mvn	r1, #0
	bl	MVC_FlushDPB
	cmp	r0, #0
	beq	.L3822
	ldr	r9, .L4012
	mov	r0, r10
	ldr	r1, .L4012+16
	ldr	r3, [r9, #68]
	blx	r3
	ldr	r4, [r9, #68]
	movw	r3, #3611
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L3989:
	ldr	r2, [r5, #220]
	cmp	r2, #2
	bne	.L3827
	b	.L3828
.L4004:
	add	r4, r4, #45056
	ldr	lr, [r4, #2376]
	cmp	lr, #0
	beq	.L3867
	ldr	r3, [r4, #2184]
	adds	r1, r3, #0
	movne	r1, #1
	cmp	ip, r3
	movne	r1, #0
	cmp	r1, #0
	movweq	r2, #47240
	movteq	r2, 169
	addeq	r2, r5, r2
	bne	.L4007
.L3871:
	add	r1, r1, #1
	cmp	r1, lr
	beq	.L3865
	ldr	r3, [r2, #4]!
	adds	r0, r3, #0
	movne	r0, #1
	cmp	ip, r3
	movne	r0, #0
	cmp	r0, #0
	beq	.L3871
.L3868:
	movw	r2, #9784
	mov	r0, r5
	movt	r2, 170
	add	r2, r5, r2
	bl	MVC_InsertFrmInDPB
	cmn	r0, #1
	mov	r4, r0
	beq	.L4008
	mov	r0, r5
	bl	MVC_UpdateReflist
	mov	r0, r5
	bl	MVC_UpdateLTReflist
	ldr	ip, .L4012
	movw	r3, #3860
	mov	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3859
.L3997:
	cmp	r0, #0
	beq	.L3828
	b	.L3835
.L3911:
	mov	r1, r3
	b	.L3887
.L3882:
	ldr	r9, .L4012
	mov	r3, r0
	movw	r2, #3924
	ldr	r1, .L4012+20
	mov	r0, #1
	ldr	r4, [r9, #68]
	blx	r4
	mov	r2, r10
	ldr	r4, [r9, #68]
	movw	r3, #3925
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
.L3901:
	ldr	r3, [r6, #2112]
	mov	r2, #1
	ldr	r0, [r5, #120]
	mvn	r4, #0
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	mov	r3, r4
	movw	r2, #3983
	ldr	ip, [r9, #68]
	ldr	r1, .L4012+20
	mov	r0, #1
	blx	ip
	ldr	r2, [r6, #2112]
	mov	r3, #0
	strb	r3, [r2, #5]
	ldr	r2, [r6, #2112]
	strb	r3, [r2, #2]
	b	.L3897
.L3988:
	ldr	r9, .L4012
	ldr	r1, .L4012+24
	ldr	r3, [r9, #68]
	blx	r3
	mov	r0, r5
	bl	MVC_DirectOutput
	ldr	r3, [r6, #2188]
	ldr	r2, [r5, #52]
	ldr	r8, [r9, #68]
	cmp	r3, r2
	addne	r3, r3, #36
	movne	r1, #0
	addne	r3, r5, r3, lsl #2
	mov	r4, r0
	strne	r1, [r3, #4]
	strne	r2, [r6, #2188]
	movw	r3, #3630
	mov	r2, r0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r8
.L3826:
	cmn	r4, #1
	bne	.L3897
	b	.L3901
.L3832:
	cmp	r2, #1
	beq	.L4009
	cmp	r1, #0
	beq	.L3835
	ldr	r2, [fp, #-64]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L3835
	b	.L3828
.L3994:
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cmp	r3, #2
	beq	.L4010
.L3880:
	mov	r0, r5
	ldr	r9, .L4012
	bl	MVC_DirectOutput
	cmn	r0, #1
	mov	r4, r0
	beq	.L4011
.L3881:
	ldr	r8, [r9, #68]
	movw	r3, #3916
	mov	r2, r4
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r8
	b	.L3826
.L3905:
	ldr	r8, [fp, #-64]
	b	.L3839
.L3996:
	ldr	r9, .L4012
	mov	r3, r8
	ldr	r2, .L4012+28
	mov	r0, #1
	ldr	r1, .L4012+32
	ldr	r4, [r9, #68]
	blx	r4
	ldr	r4, [r9, #68]
	movw	r3, #3893
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L4002:
	mov	r1, r10
	mov	r0, r5
	bl	MVC_SetFrmRepeatCount.part.1
	b	.L3851
.L3907:
	mov	r1, r3
	b	.L3855
.L4010:
	ldrb	r3, [r6, #1595]	@ zero_extendqisi2
	cmp	r3, #0
	ldrne	r3, [r6, #2112]
	strne	r3, [r5, #524]
	b	.L3880
.L4009:
	cmp	r3, #0
	ldr	r8, [r6, #2720]
	ble	.L3837
	movw	r9, #10392
	ldrb	r1, [r6, #2203]	@ zero_extendqisi2
	movt	r9, 170
	add	r9, r5, r9
	b	.L3838
.L4000:
	ldr	r9, .L4012
	movw	r2, #3711
	ldr	r1, .L4012+36
	mov	r0, #1
	str	r3, [fp, #-68]
	ldr	ip, [r9, #68]
	blx	ip
	ldr	r2, .L4012+40
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L3847
	ldr	r3, [fp, #-68]
	mov	r1, #111
	ldr	r0, [r5, #120]
	mov	r2, r3
	blx	ip
.L3847:
	ldr	r2, [r10, #200]
	cmp	r2, #0
	bne	.L3846
	b	.L3848
.L3998:
	mov	r2, #3
	ldr	r3, [r8, #44]
	strb	r2, [r8, #2]
	ldr	r2, [r9, #572]
	add	r3, r3, r2
	str	r3, [r8, #44]
	b	.L3841
.L3986:
	ldr	r9, .L4012
	mov	r0, r3
	ldr	r1, .L4012+44
	ldr	r3, [r9, #68]
	blx	r3
	ldr	r4, [r9, #68]
	movw	r3, #3587
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L4011:
	mov	r3, r0
	movw	r2, #3913
	ldr	r1, .L4012+20
	mov	r0, #1
	ldr	r8, [r9, #68]
	blx	r8
	b	.L3881
.L3993:
	ldr	r9, .L4012
	mov	r2, r0
	ldr	r1, .L4012+48
	mov	r0, #1
	ldr	r3, [r9, #68]
	blx	r3
	ldr	r4, [r9, #68]
	movw	r3, #3807
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L4006:
	ldr	r9, .L4012
	mov	r3, r0
	movw	r2, #3969
	ldr	r1, .L4012+20
	mov	r0, #1
	ldr	r4, [r9, #68]
	blx	r4
	ldr	r4, [r9, #68]
	movw	r3, #3970
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L4007:
	mov	r1, r2
	b	.L3868
.L3867:
	ldr	r3, [r6, #2128]
	cmp	r3, #0
	beq	.L3874
	ldr	r1, [r4, #2384]
	ldr	r2, [r4, #2388]
.L3899:
	ldr	r9, .L4012
	mov	r3, lr
	str	r2, [sp, #4]
	mov	r0, #1
	str	r1, [sp]
	movw	r2, #3873
	ldr	r4, [r9, #68]
	ldr	r1, .L4012+52
	blx	r4
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
	ldr	r4, [r9, #68]
	movw	r3, #3876
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L3992:
	ldr	r9, .L4012
	mov	r0, #1
	ldr	r1, .L4012+56
	ldr	r4, [r9, #68]
	blx	r4
	ldr	r3, .L4012+40
	ldr	r4, [r3]
	cmp	r4, #0
	beq	.L3862
	ldr	r8, [r6, #2164]
	mov	r3, #8
	ldr	lr, [r5, #112]
	sub	r2, fp, #52
	mov	r1, #104
	ldr	r0, [r5, #120]
	str	r8, [fp, #-52]
	str	lr, [fp, #-48]
	blx	r4
.L3862:
	mov	r0, r5
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r5
	bl	MVC_ClearDPB
	ldr	r4, [r9, #68]
	movw	r3, #3798
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L4005:
	mov	r3, r0
	movw	r2, #3830
	ldr	r1, .L4012+20
	mov	r0, #1
	ldr	r8, [r9, #68]
	blx	r8
	b	.L3870
.L4003:
	ldr	r9, .L4012
	mov	r3, r8
	movw	r2, #3773
	ldr	r1, .L4012+20
	mov	r0, #0
	ldr	r4, [r9, #68]
	blx	r4
	ldr	r4, [r9, #68]
	movw	r3, #3774
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L4008:
	ldr	r9, .L4012
	mov	r3, r0
	movw	r2, #3853
	ldr	r1, .L4012+20
	mov	r0, #1
	ldr	r8, [r9, #68]
	blx	r8
	mov	r2, r4
	movw	r3, #3854
	ldr	r4, [r9, #68]
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L4001:
	ldr	r9, .L4012
	mov	r2, #0
	ldrsb	r1, [r8, #6]
	ldr	r0, [r5, #120]
	bl	FSP_SetDisplay
	ldr	r4, [r9, #68]
	movw	r3, #3730
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	blx	r4
	b	.L3901
.L3999:
	ldr	r9, .L4012
	movw	r3, #3700
	mvn	r2, #0
	ldr	r1, .L4012+8
	mov	r0, #14
	ldr	r4, [r9, #68]
	blx	r4
	b	.L3901
.L4013:
	.align	2
.L4012:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC422
	.word	.LC421
	.word	.LC430
	.word	.LC423
	.word	.LC426
	.word	.LC424
	.word	.LANCHOR0+348
	.word	.LC431
	.word	.LC425
	.word	g_event_report
	.word	.LC420
	.word	.LC428
	.word	.LC429
	.word	.LC427
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
	ldr	r2, [r7, #2164]
	ldr	r1, [r0, #236]
	add	r0, r0, #11141120
	mov	ip, r3
	str	r3, [fp, #-80]
	movw	r3, #26758
	mov	r4, r0
	movt	r3, 42
	str	r0, [fp, #-76]
	add	r3, r2, r3
	add	r0, r5, lr
	movw	r2, #43184
	add	r8, ip, #36864
	movt	r2, 169
	add	r2, r0, r2
	ldrb	r0, [r1, #19]	@ zero_extendqisi2
	add	r6, r5, r3, lsl #2
	ldrb	r2, [r2, #3]	@ zero_extendqisi2
	movw	r3, #43184
	cmp	r0, #1
	add	r4, r4, #8192
	add	ip, ip, #45056
	movt	r3, 169
	str	r2, [fp, #-72]
	add	r3, lr, r3
	ldr	r2, [r7, #548]
	add	r3, r5, r3
	ldr	lr, [r6, #4]
	str	r2, [fp, #-52]
	ldr	r2, [r8, #3520]
	str	r2, [fp, #-64]
	moveq	r2, #2
	movne	r2, #0
	str	r2, [fp, #-56]
	ldr	r2, [r8, #3524]
	str	r2, [fp, #-68]
	ldr	r2, [r1, #2896]
	str	r3, [r4, #2116]
	ldr	r6, [ip, #2380]
	cmp	r6, #0
	beq	.L4029
	mov	ip, #1
	add	r2, r2, #4
	mov	r3, ip, asl r2
	add	r0, lr, ip
	str	ip, [fp, #-48]
	mov	r1, r3
	str	r3, [fp, #-60]
	bl	__aeabi_uidivmod
	ldr	r10, .L4032
	mov	r3, #0
	mov	r0, #2
	str	r3, [r8, #3524]
	ldr	r2, [r10, #68]
	str	r3, [r8, #3520]
	mov	r9, r1
	ldr	r1, .L4032+4
	blx	r2
	ldr	r6, [r10, #68]
	mov	r3, r9
	ldr	r2, [fp, #-52]
	ldr	r1, .L4032+8
	mov	r0, #2
	blx	r6
	ldr	r3, [fp, #-52]
	ldr	ip, [fp, #-48]
	cmp	r3, r9
	movgt	r6, ip
	bgt	.L4022
	b	.L4023
.L4020:
	mov	r3, #0
	mov	r0, r5
	strb	r3, [r4, #1596]
	bl	MVC_StorePicInDpb
	movw	ip, #26758
	movt	ip, 42
	ldr	r1, [fp, #-60]
	subs	r3, r0, #0
	add	r0, r9, #1
	bne	.L4030
	ldr	r3, [r7, #2164]
	add	ip, r3, ip
	add	ip, r5, ip, lsl #2
	str	r9, [ip, #4]
	bl	__aeabi_idivmod
	ldr	r3, [fp, #-52]
	cmp	r3, r1
	mov	r9, r1
	ble	.L4023
.L4022:
	mov	r1, #1
	mov	r0, r5
	bl	MVC_AllocFrameStore
	ldr	r1, .L4032+12
	subs	r3, r0, #0
	mov	r0, #2
	bne	.L4031
	ldr	ip, [r10, #68]
	str	r3, [fp, #-48]
	blx	ip
	ldr	r1, [r4, #2112]
	strb	r6, [r4, #1598]
	ldr	r3, [fp, #-56]
	str	r9, [r1, #20]
	ldr	r1, [r4, #2112]
	str	r9, [r4, #2120]
	str	r9, [r1, #588]
	str	r3, [r4, #2128]
	ldr	r1, [r4, #2112]
	ldr	r3, [fp, #-48]
	str	r3, [r1, #584]
	ldr	r1, [r4, #2112]
	strb	r6, [r1, #7]
	ldr	r1, [r4, #2112]
	strb	r3, [r1, #5]
	ldr	r1, [r4, #2112]
	strb	r6, [r1, #1]
	strb	r6, [r4, #1594]
	ldr	r1, [r4, #2116]
	ldr	r0, [r7, #2160]
	str	r0, [r4, #2192]
	strb	r3, [r1, #3]
	ldr	r3, [r5, #236]
	ldr	r3, [r3, #2900]
	cmp	r3, #0
	beq	.L4020
	str	r9, [r8, #3536]
	mov	r0, r5
	bl	MVC_DecPOC
	ldr	r3, [r8, #3516]
	str	r3, [r4, #2132]
	ldr	r3, [r8, #3512]
	str	r3, [r4, #2136]
	ldr	r3, [r8, #3504]
	str	r3, [r4, #2140]
	ldr	r3, [r8, #3508]
	str	r3, [r4, #2144]
	b	.L4020
.L4029:
	ldr	r3, .L4032
	mov	r0, r6
	ldr	r1, .L4032+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4023:
	ldr	r2, [fp, #-64]
	mov	r0, #0
	ldr	r3, [fp, #-76]
	str	r2, [r8, #3520]
	add	r3, r3, #8192
	ldr	r2, [fp, #-68]
	str	r2, [r8, #3524]
	ldrb	r2, [fp, #-72]	@ zero_extendqisi2
	ldr	r3, [r3, #2116]
	strb	r2, [r3, #3]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4031:
	ldr	r2, [fp, #-64]
	mov	r0, #0
	ldr	r3, [fp, #-76]
	ldr	r1, .L4032+20
	str	r2, [r8, #3520]
	add	r3, r3, #8192
	ldr	r2, [fp, #-68]
	str	r2, [r8, #3524]
	ldrb	r2, [fp, #-72]	@ zero_extendqisi2
	ldr	r3, [r3, #2116]
	strb	r2, [r3, #3]
	ldr	r3, [r10, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4030:
	ldr	r2, [fp, #-80]
	mov	r0, #0
	ldr	ip, [fp, #-64]
	add	r1, r2, #36864
	ldrb	lr, [fp, #-72]	@ zero_extendqisi2
	movw	r2, #6901
	str	ip, [r1, #3520]
	ldr	ip, [fp, #-68]
	str	ip, [r1, #3524]
	ldr	ip, [r4, #2116]
	ldr	r1, .L4032+24
	strb	lr, [ip, #3]
	ldr	ip, [r10, #68]
	blx	ip
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4033:
	.align	2
.L4032:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC433
	.word	.LC434
	.word	.LC436
	.word	.LC432
	.word	.LC435
	.word	.LC437
	UNWIND(.fnend)
	.size	MVC_DecGap, .-MVC_DecGap
	.align	2
	.global	MVC_InitPic
	.type	MVC_InitPic, %function
MVC_InitPic:
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
	add	r5, r0, #11141120
	add	r3, r0, #11075584
	add	r5, r5, #8192
	add	r6, r3, #40960
	mov	r4, r0
	str	r3, [fp, #-56]
	mov	r1, #0
	ldr	r3, [r0, #252]
	mov	ip, #2240
	ldrb	r0, [r5, #1592]	@ zero_extendqisi2
	ldr	r2, [r6, #536]
	cmp	r0, r1
	str	r1, [fp, #-48]
	mla	r10, ip, r2, r3
	beq	.L4035
	ldr	r3, [r5, #2112]
	cmp	r3, r1
	beq	.L4036
	ldrb	r7, [r3, #2]	@ zero_extendqisi2
	cmp	r7, r1
	beq	.L4198
.L4036:
	mov	r3, #0
	strb	r3, [r5, #1592]
.L4035:
	sub	r1, fp, #48
	mov	r0, r4
	bl	MVC_GetReRangeFlag
	ldrb	r3, [r6, #532]	@ zero_extendqisi2
	cmp	r3, #255
	mov	r8, r0
	beq	.L4199
.L4037:
	cmp	r3, #0
	bne	.L4040
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L4200
	ldr	r3, [r10, #28]
	ldr	r2, [r4, #28]
	cmp	r2, r3
	movweq	r3, #35364
	movteq	r3, 168
	addeq	r3, r4, r3
	streq	r3, [r4, #236]
	bne	.L4201
.L4040:
	ldr	r3, [fp, #-56]
	add	r7, r3, #36864
	ldr	r3, [r10, #24]
	ldr	r2, [r7, #1264]
	cmp	r2, r3
	beq	.L4202
.L4044:
	ldr	r3, .L4217
	mov	r1, r10
	mov	r9, r3
	mov	r3, #0
	strb	r3, [r10, #20]
	ldr	r0, [r4, #236]
	bl	mvc_assign_quant_params
	movw	r0, #38104
	movt	r0, 169
	ldr	r3, [r9, #56]
	add	r0, r4, r0
	mov	r2, #2240
	mov	r1, r10
	blx	r3
.L4045:
	ldr	r3, [fp, #-48]
	cmp	r3, #0
	ldr	r3, [r4, #224]
	bne	.L4203
	ldr	r3, [r3, #892]
	cmp	r3, #1
	beq	.L4047
.L4056:
	ldr	r2, [r7, #1264]
	ldr	r3, [r10, #24]
	cmp	r2, r3
	bne	.L4065
	ldrb	r3, [r10, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4204
.L4065:
	ldr	r3, .L4217
	movw	r0, #38104
	movt	r0, 169
	mov	r1, r10
	add	r0, r4, r0
	mov	r2, r3
	str	r3, [fp, #-60]
	mov	r3, #0
	strb	r3, [r10, #20]
	ldr	r3, [r2, #56]
	mov	r2, #2240
	blx	r3
.L4066:
	ldrb	r3, [r6, #532]	@ zero_extendqisi2
	cmp	r3, #255
	moveq	r3, #0
	streq	r3, [r6, #2164]
	beq	.L4068
	cmp	r3, #0
	bne	.L4068
	add	r1, r4, #10747904
	movw	r2, #22868
	add	r1, r1, #20480
	movt	r2, 164
	ldr	ip, [r6, #2160]
	add	r2, r4, r2
	ldr	r0, [r1, #2384]
	adds	r0, r0, #1
	beq	.L4069
	ldr	r1, [r1, #2388]
	cmp	ip, r1
	beq	.L4106
	mov	lr, r0
	ldr	r9, [fp, #-60]
	ldr	r8, [fp, #-56]
	b	.L4070
.L4071:
	ldr	r1, [r2, #4]!
	cmp	ip, r1
	beq	.L4192
.L4070:
	add	r3, r3, #1
	cmp	r3, lr
	mov	r0, r3
	bne	.L4071
.L4192:
	str	r9, [fp, #-60]
	str	r8, [fp, #-56]
.L4069:
	str	r0, [r6, #2164]
.L4068:
	ldrb	r2, [r6, #523]	@ zero_extendqisi2
	ldr	r3, [r4, #236]
	cmp	r2, #5
	ldreq	r1, [r6, #2164]
	movweq	r2, #26758
	movteq	r2, 42
	ldr	r10, [r3, #2896]
	addeq	r2, r1, r2
	ldreq	r3, [r6, #548]
	ldrne	r3, [r6, #548]
	addeq	r2, r4, r2, lsl #2
	streq	r3, [r2, #4]
	ldrb	r2, [r4, #8]	@ zero_extendqisi2
	cmp	r2, #2
	beq	.L4074
	ldr	ip, [r4, #224]
	ldr	r2, [ip, #12]
	str	ip, [fp, #-64]
	cmp	r2, #2
	beq	.L4074
	ldr	r1, [r6, #2164]
	movw	r2, #26758
	movt	r2, 42
	add	r2, r1, r2
	add	r2, r4, r2, lsl #2
	ldr	r0, [r2, #4]
	cmp	r0, r3
	beq	.L4074
	mov	r2, #1
	add	r10, r10, #4
	mov	r10, r2, asl r10
	add	r0, r0, r2
	str	r3, [fp, #-68]
	mov	r1, r10
	str	r2, [fp, #-72]
	bl	__aeabi_uidivmod
	ldr	r3, [fp, #-68]
	cmp	r1, r3
	beq	.L4074
	ldr	r2, [fp, #-72]
	ldr	ip, [fp, #-64]
	str	r2, [ip, #680]
	ldr	r3, [r4, #224]
	ldr	r2, [r3, #680]
	cmp	r2, #0
	beq	.L4205
.L4075:
	ldr	r3, [fp, #-56]
	add	r3, r3, #32768
	ldrb	r3, [r3, #1363]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4084
	ldr	r3, [fp, #-60]
	mov	r0, #2
	ldr	r1, .L4217+4
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r3, [r4, #224]
	ldr	ip, .L4217
	ldr	r3, [r3, #684]
	bics	r3, r3, #1024
	bne	.L4193
	ldr	r2, [r6, #2160]
	cmp	r2, #0
	bne	.L4193
	ldr	r3, [r6, #2164]
	movw	r0, #26758
	movt	r0, 42
	mov	r1, r10
	add	r0, r3, r0
	ldr	r3, [r6, #548]
	str	r2, [fp, #-72]
	add	r0, r4, r0, lsl #2
	str	ip, [fp, #-68]
	str	r3, [fp, #-64]
	ldr	r0, [r0, #4]
	add	r0, r0, #1
	bl	__aeabi_uidivmod
	ldr	r3, [fp, #-64]
	ldr	ip, [fp, #-68]
	ldr	r2, [fp, #-72]
	cmp	r3, r1
	bcs	.L4074
	ldrb	r1, [r6, #520]	@ zero_extendqisi2
	cmp	r1, #2
	beq	.L4074
	mov	r0, r4
	str	r2, [fp, #-60]
	str	ip, [fp, #-56]
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r0, [r6, #2164]
	movw	r3, #26758
	mov	r1, r10
	movt	r3, 42
	add	r3, r0, r3
	add	r3, r4, r3, lsl #2
	ldr	r0, [r3, #4]
	add	r0, r0, #1
	bl	__aeabi_uidivmod
	ldr	r2, [fp, #-60]
	ldr	ip, [fp, #-56]
	ldr	r3, [r6, #548]
	mov	r0, r2
	movw	r2, #7427
	ldr	ip, [ip, #68]
	str	r1, [sp]
	ldr	r1, .L4217+8
	blx	ip
	mvn	r0, #0
.L4190:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4203:
	ldr	r9, [r3, #776]
	cmp	r9, #0
	bne	.L4047
	ldr	r3, [r3, #28]
	cmp	r3, #25
	beq	.L4206
.L4047:
	ldr	r3, [r4, #84]
	cmp	r8, #0
	add	r3, r3, #1
	str	r3, [r4, #84]
	bne	.L4057
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #892]
	cmp	r3, #1
	beq	.L4058
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	cmp	r0, #0
	bne	.L4207
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	ldrne	r3, .L4217
	strne	r3, [fp, #-60]
	bne	.L4104
.L4105:
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #892]
	cmp	r3, #1
	bne	.L4056
.L4062:
	ldr	r1, [r7, #1220]
	mov	r0, r4
	ldr	r3, [fp, #-56]
	mov	r1, r1, asl #1
	add	lr, r3, #32768
	str	r1, [r4, #52]
	add	r3, r4, #48
	ldr	r2, [r7, #1200]
	ldrb	r9, [lr, #1364]	@ zero_extendqisi2
	add	r2, r2, #1
	ldr	ip, [r7, #1196]
	rsb	r9, r9, #2
	str	r1, [sp]
	mov	r2, r2, asl #4
	add	r1, ip, #1
	mul	r9, r9, r2
	mov	ip, r1, asl #4
	str	ip, [fp, #-60]
	mov	r1, ip
	mov	r2, r9
	bl	MVC_ArrangeVahbMem
	ldr	ip, [fp, #-60]
	cmp	r0, #1
	beq	.L4191
.L4196:
	ldr	r3, [r4, #224]
	ldr	r3, [r3, #892]
	cmp	r3, #1
	mvneq	r0, #1
	beq	.L4190
	ldr	r3, .L4217
	mov	r5, #0
	mov	r0, r5
	str	r5, [r4, #12]
	str	r5, [r4, #16]
	ldr	r1, .L4217+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r1, r5
	mov	r0, r4
	bl	MVC_ClearAll
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4084:
	mov	r0, r4
	bl	MVC_DecGap
	cmp	r0, #0
	bne	.L4087
.L4193:
	ldr	r3, [r6, #548]
.L4074:
	ldrb	r2, [r6, #528]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L4088
	ldr	r1, [r6, #2164]
	movw	r2, #26758
	movt	r2, 42
	add	r2, r1, r2
	add	r2, r4, r2, lsl #2
	str	r3, [r2, #4]
.L4088:
	str	r3, [r7, #3536]
	mov	r0, r4
	bl	MVC_DecPOC
	mov	r1, #0
	mov	r0, r4
	bl	MVC_AllocFrameStore
	cmp	r0, #0
	bne	.L4208
	strb	r0, [r5, #1599]
	mov	r1, #1
	strb	r1, [r5, #1592]
	ldrb	r3, [r6, #523]	@ zero_extendqisi2
	strb	r3, [r5, #1598]
	ldr	r3, [r6, #548]
	str	r3, [r5, #2120]
	mov	r8, r3
	ldrb	r10, [r6, #528]	@ zero_extendqisi2
	str	r10, [r5, #2128]
	ldr	lr, [r7, #3516]
	str	lr, [r5, #2132]
	ldr	r2, [r7, #3512]
	str	r2, [r5, #2136]
	ldr	ip, [r7, #3504]
	str	ip, [r5, #2140]
	ldr	r3, [r7, #3508]
	str	lr, [r5, #2148]
	str	ip, [r5, #2156]
	str	r3, [r5, #2144]
	str	r3, [r5, #2160]
	mvn	r3, #0
	str	r2, [r5, #2152]
	strb	r0, [r5, #1594]
	strb	r0, [r5, #1597]
	strb	r0, [r5, #1596]
	str	r3, [r5, #2184]
	ldrb	r2, [r6, #521]	@ zero_extendqisi2
	cmp	r2, #0
	strneb	r1, [r6, #521]
	ldr	ip, [r4, #236]
	movne	r2, r1
	movne	r1, #2
	str	r2, [fp, #-68]
	ldrb	r0, [ip, #20]	@ zero_extendqisi2
	ldr	lr, [ip, #3952]
	rsb	r0, r0, #2
	str	ip, [fp, #-64]
	mla	r0, lr, r0, r0
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-64]
	ldr	r2, [fp, #-68]
	cmp	r2, #0
	str	r0, [r5, #2172]
	ldrb	r1, [ip, #20]	@ zero_extendqisi2
	ldr	lr, [ip, #3952]
	rsb	r2, r1, #2
	mla	r2, lr, r2, r2
	str	r2, [r5, #2176]
	ldr	r1, [ip, #3948]
	add	r2, r1, #1
	str	r2, [r5, #2168]
	mul	r2, r2, r0
	str	r2, [r5, #2180]
	ldrb	r2, [r7, #1240]	@ zero_extendqisi2
	strb	r2, [r5, #1600]
	movne	r2, #0
	bne	.L4091
	ldrb	r2, [ip, #21]	@ zero_extendqisi2
	adds	r2, r2, #0
	movne	r2, #1
.L4091:
	cmp	r10, #0
	strb	r2, [r5, #1601]
	beq	.L4092
	ldr	r1, [r6, #2164]
	movw	r2, #26758
	movt	r2, 42
	add	r2, r1, r2
	add	r2, r4, r2, lsl #2
	str	r8, [r2, #4]
.L4092:
	ldr	r3, [r5, #2112]
	cmp	r3, #0
	ldrneb	r2, [r6, #532]	@ zero_extendqisi2
	strneb	r2, [r3, #9]
	mvn	r3, #0
	str	r3, [r5, #2192]
	ldrb	r3, [r6, #531]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4094
	ldr	r3, [r6, #2160]
	str	r3, [r5, #2192]
	ldrb	r3, [r6, #529]	@ zero_extendqisi2
	strb	r3, [r5, #1602]
	ldrb	r3, [r6, #530]	@ zero_extendqisi2
	strb	r3, [r5, #1603]
.L4094:
	ldr	r1, [r6, #2164]
	movw	ip, #2004
	movw	r3, #43184
	movw	r2, #9808
	movt	r3, 169
	movt	r2, 170
	str	r1, [r5, #2196]
	movw	r7, #9824
	ldrb	r1, [r4, #3]	@ zero_extendqisi2
	movt	r7, 170
	add	r7, r4, r7
	movw	lr, #9824
	movt	lr, 170
	mov	r0, #29
	mla	ip, ip, r1, r4
	ldr	r1, .L4217+16
	add	r3, ip, r3
	str	r3, [r5, #2116]
	ldr	r3, [r4, #224]
	ldrd	r8, [r3, #48]
	strd	r8, [r4, r2]
	ldrd	r8, [r3, #64]
	strd	r8, [r7, #-8]
	ldrd	r8, [r3, #80]
	strd	r8, [r4, lr]
	mvn	r8, #0
	mvn	r9, #0
	strd	r8, [r3, #48]
	ldr	r8, [fp, #-60]
	ldrd	r2, [r2, r4]
	ldr	ip, [r8, #68]
	blx	ip
	ldrd	r2, [r7, #-8]
	ldr	ip, [r8, #68]
	mov	r0, #29
	ldr	r1, .L4217+20
	blx	ip
	ldr	ip, [r4, #236]
	ldr	r3, [fp, #-56]
	movw	lr, #8864
	movw	r0, #9800
	ldr	r7, .L4217
	add	r8, r3, #32768
	ldr	r3, [ip, #3948]
	movt	r0, 170
	add	r0, r4, r0
	add	r3, r3, #1
	mov	r3, r3, asl #4
	str	r3, [r5, #1772]
	ldrb	r2, [ip, #20]	@ zero_extendqisi2
	ldr	r1, [ip, #3952]
	rsb	r3, r2, #2
	mla	r3, r1, r3, r3
	mov	r3, r3, asl #4
	str	r3, [r5, #1776]
	ldr	r1, [r8, #1396]
	str	lr, [r5, #1768]
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
	str	r3, [r5, #1768]
	bl	SetAspectRatio
	ldr	r0, [r5, #2112]
	ldr	r3, [r4, #84]
	cmp	r0, #0
	str	r3, [r5, #1812]
	beq	.L4209
	ldr	r3, [r4, #236]
	ldr	r3, [r3, #3984]
	str	r3, [r0, #252]
	str	r3, [r5, #1788]
	ldr	r3, [r4, #236]
	ldr	r2, [r5, #2112]
	ldr	r3, [r3, #3988]
	str	r3, [r2, #256]
	str	r3, [r5, #1792]
	ldr	r3, [r4, #236]
	ldr	r2, [r5, #2112]
	ldr	r3, [r3, #3976]
	str	r3, [r2, #244]
	str	r3, [r5, #1780]
	ldr	r2, [r4, #236]
	ldr	r3, [r5, #2112]
	ldr	r2, [r2, #3980]
	str	r2, [r3, #248]
	ldrb	r3, [r5, #1595]	@ zero_extendqisi2
	ldr	r1, [r5, #2112]
	ldr	r0, [r5, #1768]
	adds	r3, r3, #0
	str	r2, [r5, #1784]
	movne	r3, #1
	str	r3, [r5, #1756]
	str	r0, [r1, #232]
	ldr	r2, [r5, #1756]
	ldr	r3, [r5, #2112]
	str	r2, [r3, #220]
	ldr	r3, [r6, #548]
	ldr	r2, [r5, #2112]
	str	r3, [r5, #1848]
	ldr	r1, [r6, #2160]
	str	r1, [r5, #1856]
	str	r3, [r2, #312]
	ldr	r3, [r5, #2112]
	ldr	r2, [r6, #2160]
	str	r2, [r3, #320]
	ldr	r3, [r5, #2112]
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_GetDispPhyFs
	cmp	r0, #0
	beq	.L4096
	ldr	r3, [r5, #1756]
	mov	r1, #0
	ldr	r2, [r0, #4]
	subs	r3, r3, r1
	ldr	r0, [r4, #120]
	movne	r3, #1
	bl	FSP_SetStoreType
.L4096:
	movw	r2, #10008
	movw	r0, #10024
	ldr	r6, [r4, #60]
	movt	r2, 170
	movt	r0, 170
	add	r2, r4, r2
	add	r0, r4, r0
	mov	lr, #0
.L4100:
	ldr	r3, [r2, #4]!
	cmp	r3, #0
	beq	.L4210
.L4097:
	cmp	r2, r0
	bne	.L4100
	cmp	r6, #0
	beq	.L4102
	ble	.L4102
	add	r7, r6, #132
	mov	r8, #0
	add	r7, r4, r7, lsl #2
.L4103:
	ldr	r1, [r7, #-4]!
	sub	r6, r6, #1
	ldr	r0, [r4, #120]
	bl	FreeUsdByDec
	cmp	r6, #0
	str	r8, [r7]
	bne	.L4103
.L4102:
	mov	r6, #0
	mov	r0, r4
	str	r6, [r4, #60]
	strb	r6, [r5, #1604]
	bl	MVC_UpdateReflist
	mov	r0, r4
	bl	MVC_UpdateLTReflist
	mov	r0, r4
	bl	MVC_CalcPicNum
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4202:
	ldrb	r3, [r10, #20]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L4045
	b	.L4044
.L4210:
	cmp	r6, #0
	beq	.L4097
	ldr	r1, [r4, #528]
	subs	r6, r6, #1
	str	r1, [r2]
	beq	.L4098
	add	r1, r4, #528
.L4099:
	add	r3, r3, #1
	ldr	ip, [r1, #4]!
	cmp	r3, r6
	str	ip, [r1, #-4]
	bne	.L4099
.L4098:
	add	r3, r6, #132
	str	lr, [r4, r3, asl #2]
	str	r6, [r4, #60]
	b	.L4097
.L4199:
	mov	r2, #1
	strb	r2, [r4, #2]
	ldr	r2, [r10, #28]
	movw	r7, #3992
	ldr	lr, [r4, #248]
	ldr	r1, [fp, #-56]
	str	r2, [r4, #24]
	ldr	r2, [r10, #28]
	add	r1, r1, #32768
	ldr	r0, [r1, #2088]
	mla	r2, r7, r2, lr
	movw	r7, #34112
	ldr	r1, [r2, #744]
	cmp	r0, r1
	beq	.L4211
.L4038:
	ldr	r3, .L4217
	movt	r7, 169
	add	r7, r4, r7
	mov	lr, #0
	mov	r1, r2
	mov	ip, r3
	ldr	r3, [r4, #84]
	mov	r0, r7
	add	r3, r3, #1
	str	r3, [r4, #84]
	strb	lr, [r2, #26]
	movw	r2, #3992
	ldr	r3, [ip, #56]
	blx	r3
	ldrb	r3, [r6, #532]	@ zero_extendqisi2
.L4039:
	str	r7, [r4, #236]
	b	.L4037
.L4058:
	ldr	r3, .L4217
	mov	r0, #22
	ldr	r1, .L4217+24
	str	r3, [fp, #-60]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	bl	MVC_GetBackPicFromVOQueue
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L4105
.L4104:
	ldr	r3, [fp, #-60]
	movw	r2, #7262
	ldr	r1, .L4217+28
	ldr	ip, [r3, #68]
	mov	r3, r0
	mov	r0, #0
	blx	ip
	mvn	r0, #0
	b	.L4190
.L4204:
	ldr	r3, .L4217
	str	r3, [fp, #-60]
	b	.L4066
.L4211:
	ldrb	r1, [r2, #26]	@ zero_extendqisi2
	cmp	r1, #0
	movteq	r7, 169
	addeq	r7, r4, r7
	beq	.L4039
	b	.L4038
.L4198:
	ldrsb	r1, [r3, #6]
	mov	r2, #1
	ldr	r0, [r4, #120]
	bl	FSP_ClearLogicFs
	ldr	r3, [r5, #2112]
	strb	r7, [r3, #2]
	ldr	r3, [r5, #2112]
	strb	r7, [r3, #5]
	b	.L4036
.L4205:
	ldr	r2, [r3, #708]
	cmp	r2, #1
	beq	.L4212
.L4076:
	ldr	r3, [r6, #2164]
	ldr	r1, [r6, #548]
	add	r3, r4, r3, lsl #2
	add	r3, r3, #11075584
	add	r2, r3, #40960
	ldr	r2, [r2, #540]
	cmp	r1, r2
	bls	.L4077
	ldr	ip, [r7, #1220]
	rsb	r0, r2, r1
	cmp	r2, #0
	cmpne	ip, r0
	bcs	.L4075
	ldrb	r2, [r6, #520]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L4213
	add	r3, r3, #40960
	cmp	r1, #0
	subne	r2, r1, #1
	subeq	r2, r10, #1
	mvn	r1, #0
	str	r2, [r3, #540]
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r3, r0, #0
	beq	.L4075
	ldr	r2, [fp, #-60]
	mov	r0, #0
	ldr	r1, .L4217+32
	ldr	ip, [r2, #68]
	movw	r2, #7373
	blx	ip
	mvn	r0, #0
	b	.L4190
.L4200:
	strb	r3, [r4, #2]
	movw	r1, #8500
	ldr	r3, [r10, #28]
	movt	r1, 5
	ldr	r2, .L4217
	movw	r0, #22860
	movt	r0, 164
	add	r0, r4, r0
	mov	ip, r2
	mov	r2, r1
	mla	r1, r1, r3, r4
	ldr	r3, [ip, #56]
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
	b	.L4040
.L4206:
	ldr	r3, [r4, #84]
	cmp	r8, #0
	add	r3, r3, #1
	str	r3, [r4, #84]
	beq	.L4048
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r3, r0, #0
	beq	.L4048
	ldr	ip, .L4217
	mov	r0, r9
	movw	r2, #7169
	ldr	r1, .L4217+32
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L4190
.L4048:
	mov	r0, r4
	bl	MVC_InitDPB
	subs	r3, r0, #0
	bne	.L4214
	ldr	r2, [fp, #-56]
	cmp	r8, #0
	ldr	r3, [r7, #1200]
	add	r1, r2, #32768
	ldr	r2, [r7, #1196]
	add	r3, r3, #1
	ldrb	r9, [r1, #1364]	@ zero_extendqisi2
	add	r2, r2, #1
	mov	r3, r3, asl #4
	rsb	r9, r9, #2
	mov	ip, r2, asl #4
	mul	r9, r9, r3
	beq	.L4191
	ldr	r3, [r4, #12]
	cmp	r3, #0
	bne	.L4191
	ldr	r2, [r7, #1220]
	ldr	r3, [r4, #224]
	mov	r2, r2, asl #1
	str	r2, [r4, #52]
	ldr	r1, [r3, #736]
	str	r2, [fp, #-60]
	cmp	ip, r1
	bhi	.L4051
	ldr	r2, [r3, #740]
	cmp	r9, r2
	bhi	.L4051
	ldr	lr, [fp, #-56]
	ldr	r8, [r3, #760]
	add	lr, lr, #45056
	ldr	lr, [lr, #2376]
	cmp	r8, lr
	bcc	.L4215
	ldr	r0, [r3, #764]
	str	ip, [fp, #-64]
	cmp	r0, #0
	mov	r0, r4
	moveq	r2, #1
	streq	r2, [r4, #52]
	streq	r2, [fp, #-60]
	ldreq	r1, [r3, #736]
	ldreq	r2, [r3, #740]
	ldr	r3, [fp, #-60]
	str	r3, [sp]
	add	r3, r4, #48
	bl	MVC_ArrangeVahbMem
	ldr	ip, [fp, #-64]
	cmp	r0, #1
	bne	.L4196
.L4191:
	mov	r3, ip, lsr #4
	mov	r9, r9, lsr #4
	str	r3, [r4, #12]
	str	r9, [r4, #16]
	b	.L4056
.L4057:
	ldr	r3, .L4217
	mov	r0, #22
	ldr	r1, .L4217+24
	str	r3, [fp, #-60]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	bl	MVC_GetBackPicFromVOQueue
	mov	r0, r4
	bl	MVC_InitDPB
	cmp	r0, #0
	beq	.L4062
	b	.L4104
.L4077:
	rsb	r0, r2, r1
	ldr	ip, [r7, #1220]
	add	r0, r0, r10
	cmp	r2, #0
	cmpne	ip, r0
	bcs	.L4075
	ldrb	r2, [r6, #520]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L4216
	add	r3, r3, #40960
	cmp	r1, #0
	subne	r2, r1, #1
	subeq	r2, r10, #1
	mvn	r1, #0
	str	r2, [r3, #540]
	mov	r0, r4
	bl	MVC_FlushDPB
	subs	r3, r0, #0
	beq	.L4075
	ldr	r2, [fp, #-60]
	mov	r0, #0
	ldr	r1, .L4217+32
	ldr	ip, [r2, #68]
	movw	r2, #7400
	blx	ip
	mvn	r0, #0
	b	.L4190
.L4201:
	ldr	r3, .L4217
	mov	r0, #1
	ldr	r1, .L4217+36
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4190
.L4087:
	ldr	r3, [fp, #-60]
	movw	r2, #7454
	ldr	r1, .L4217+40
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4190
.L4212:
	ldr	r2, [r3, #704]
	cmp	r2, #0
	bne	.L4076
	ldr	r2, [r3, #712]
	cmp	r2, #0
	bne	.L4076
	ldr	r3, [r3, #684]
	add	r3, r3, #1024
	cmp	r3, #2048
	bhi	.L4075
	b	.L4076
.L4106:
	mov	r0, r3
	b	.L4069
.L4208:
	ldr	r3, [fp, #-60]
	movw	r2, #7474
	ldr	r1, .L4217+44
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4190
.L4209:
	ldr	r3, [r7, #68]
	movw	r2, #7582
	ldr	r1, .L4217+48
	blx	r3
	mvn	r0, #0
	b	.L4190
.L4207:
	ldr	ip, .L4217
	mov	r3, r0
	movw	r2, #7253
	mov	r0, r8
	ldr	r1, .L4217+32
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L4190
.L4214:
	ldr	ip, .L4217
	movw	r2, #7178
	ldr	r1, .L4217+28
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L4190
.L4051:
	ldr	r3, .L4217
	mov	r0, #0
	ldr	r1, .L4217+52
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4190
.L4215:
	ldr	r3, .L4217
	ldr	r1, .L4217+56
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L4190
.L4216:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r2, [r6, #2164]
	ldr	r3, [r7, #1220]
	movw	r1, #26758
	ldr	lr, [r6, #548]
	movt	r1, 42
	add	r1, r2, r1
	movw	r2, #7389
.L4195:
	str	r3, [sp]
	add	r1, r4, r1, lsl #2
	ldr	r3, [fp, #-60]
	mov	r0, #0
	ldr	ip, [r3, #68]
	ldr	r3, [r1, #4]
	ldr	r1, .L4217+60
	rsb	r3, r3, lr
	blx	ip
	mvn	r0, #0
	b	.L4190
.L4213:
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_ClearDPB
	ldr	r2, [r6, #2164]
	movw	r1, #26758
	ldr	r3, [r7, #1220]
	movt	r1, 42
	ldr	lr, [r6, #548]
	add	r1, r2, r1
	movw	r2, #7362
	b	.L4195
.L4218:
	.align	2
.L4217:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC444
	.word	.LC445
	.word	.LC442
	.word	.LC448
	.word	.LC449
	.word	.LC451
	.word	.LC439
	.word	.LC438
	.word	.LC30
	.word	.LC446
	.word	.LC447
	.word	.LC450
	.word	.LC440
	.word	.LC441
	.word	.LC443
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	ldr	r6, .L4270
	ldr	r2, [r0, #68]
	mov	r7, r0
	ldr	r1, .L4270+4
	mov	r0, #22
	sub	r2, r2, #1
	ldr	r3, [r6, #68]
	blx	r3
	ldr	r3, [r7, #232]
	add	r5, r7, #11075584
	mov	r0, r7
	add	r5, r5, #40960
	ldrb	r2, [r3, #2]	@ zero_extendqisi2
	strb	r2, [r5, #523]
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	strb	r3, [r5, #528]
	bl	MVC_ProcessSliceHeaderFirstPart
	cmp	r0, #0
	bne	.L4264
	mov	r0, r7
	bl	MVC_ProcessSliceHeaderSecondPart
	cmp	r0, #0
	bne	.L4264
	ldr	r2, [r7, #64]
	cmp	r2, #0
	moveq	r3, r2
	beq	.L4226
	movw	r10, #11024
	mov	lr, r0
	movt	r10, 170
	add	r10, r7, r10
	mov	r3, r0
	mov	r8, lr
.L4234:
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
	beq	.L4232
	cmp	r4, r0
	add	r3, ip, r3
	addhi	r0, r0, lr
	rsbhi	r0, r0, r4
	movls	r0, r9
	addhi	r0, r0, r9
	add	r3, r3, r0
.L4232:
	cmp	r1, #0
	add	r8, r8, #1
	movne	r0, r1
	movne	lr, ip
	moveq	r0, r4
	moveq	lr, r9
	cmp	r8, r2
	bne	.L4234
.L4226:
	add	r4, r7, #11141120
	str	r3, [r7, #104]
	add	r4, r4, #8192
	movw	r0, #1620
	ldr	ip, [r4, #2172]
	ldr	r1, [r4, #2168]
	mul	r1, r1, ip
	cmp	r1, r0
	movgt	r1, r1, asl #7
	ble	.L4265
.L4236:
	ldr	r0, [r7, #32]
	cmp	r0, r2
	bls	.L4237
	cmp	r1, r3
	bcc	.L4237
	ldrb	r3, [r5, #525]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4266
.L4240:
	mov	r0, r7
	bl	MVC_PicTypeStatistic
	mov	r0, r7
	bl	MVC_DecList
	cmp	r0, #0
	bne	.L4267
	mov	r0, r7
	bl	MVC_IsRefListWrong
	subs	r4, r0, #0
	bne	.L4268
	mov	r0, r7
	bl	MVC_CalcStreamBits
	mov	r0, r7
	bl	MVC_WriteSliceMsg
	mov	r0, r7
	bl	MVC_ExitSlice
	ldr	r3, [r7, #100]
	mov	r0, r4
	mov	r2, #1
	add	r3, r3, r2
	strb	r2, [r7, #4]
	str	r3, [r7, #100]
.L4223:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4265:
	mov	r0, r1, asl #9
	sub	r1, r0, r1, asl #7
	cmp	r1, #16384
	movlt	r1, #16384
	b	.L4236
.L4266:
	ldr	r3, [r6, #68]
	mov	r0, #2
	ldr	r1, .L4270+8
	blx	r3
	ldr	r3, [r7, #88]
	mov	r2, #0
	str	r2, [r7, #100]
	add	r3, r3, #1
	str	r3, [r7, #88]
	ldrb	r3, [r4, #1595]	@ zero_extendqisi2
	mov	r0, r7
	cmp	r3, r2
	ldreq	r3, [r7, #92]
	ldrne	r3, [r7, #96]
	addeq	r3, r3, #1
	addne	r3, r3, #1
	streq	r3, [r7, #92]
	strne	r3, [r7, #96]
	bl	MVC_InitPic
	cmn	r0, #2
	beq	.L4223
	cmp	r0, #0
	bne	.L4269
	mov	r0, r7
	bl	MVC_WritePicMsg
	b	.L4240
.L4264:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L4270+12
	blx	r3
	ldr	r3, .L4270+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4262
	sub	r2, fp, #44
	mvn	r1, #0
	mov	r3, #4
	str	r1, [r2, #-8]!
	mov	r1, #100
	ldr	r0, [r7, #120]
	blx	ip
.L4262:
	mov	r0, r7
	bl	MVC_ClearCurrSlice
	mvn	r0, #0
	b	.L4223
.L4237:
	str	r1, [sp]
	mov	r0, #0
	ldr	ip, [r6, #68]
	ldr	r1, .L4270+20
	blx	ip
	ldr	r3, [r7, #64]
	ldr	r2, [r7, #32]
	cmp	r3, r2
	bcc	.L4239
	ldr	r1, .L4270+16
	ldr	ip, [r1]
	cmp	ip, #0
	beq	.L4239
	str	r3, [fp, #-52]
	mov	r1, #108
	str	r2, [fp, #-48]
	mov	r3, #8
	sub	r2, fp, #52
	ldr	r0, [r7, #120]
	blx	ip
.L4239:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L4270+24
	blx	r3
	mov	r0, r7
	bl	MVC_ClearCurrPic
	mvn	r0, #0
	b	.L4223
.L4267:
	mov	r2, r0
	ldr	r3, [r6, #68]
	ldr	r1, .L4270+28
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	b	.L4223
.L4268:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L4270+32
	blx	r3
	b	.L4262
.L4269:
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L4270+36
	blx	r3
	mvn	r0, #0
	b	.L4223
.L4271:
	.align	2
.L4270:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC391
	.word	.LC454
	.word	.LC392
	.word	g_event_report
	.word	.LC452
	.word	.LC453
	.word	.LC456
	.word	.LC457
	.word	.LC455
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
	beq	.L4277
.L4276:
	ldr	r0, [r2, #12]
	add	r3, r3, #1
	add	r2, r2, #28
	add	ip, ip, r0
	str	ip, [r4, #104]
	ldr	r0, [lr, #68]
	cmp	r0, r3
	bhi	.L4276
.L4277:
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
	beq	.L4458
.L4275:
	ldr	r8, .L4469
	mov	r0, #22
	ldr	r2, [r4, #68]
	ldr	r1, .L4469+4
	ldr	ip, [r8, #68]
	blx	ip
	ldr	r3, [r4, #68]
	movw	r2, #371
	cmp	r3, r2
	beq	.L4459
.L4280:
	ldr	r2, [r4, #232]
	add	r3, r3, #1
	str	r3, [r4, #68]
	ldrb	r2, [r2, #2]	@ zero_extendqisi2
	sub	r3, r2, #1
	cmp	r3, #29
	ldrls	pc, [pc, r3, asl #2]
	b	.L4281
.L4283:
	.word	.L4282
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4282
	.word	.L4284
	.word	.L4285
	.word	.L4286
	.word	.L4287
	.word	.L4288
	.word	.L4289
	.word	.L4290
	.word	.L4291
	.word	.L4292
	.word	.L4293
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4294
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4281
	.word	.L4295
.L4458:
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
	bne	.L4278
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
	beq	.L4460
.L4278:
	ldr	r2, [r3, #64]
	add	r2, r2, #24
	str	r2, [r3, #64]
	ldr	r3, [r4, #232]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	b	.L4275
.L4282:
	ldr	ip, [r8, #68]
	mov	r0, #22
	ldr	r3, [r4, #80]
	ldr	r1, .L4469+8
	blx	ip
	ldr	r3, [r4, #232]
	add	r0, r4, #11075584
	add	r2, r4, #12288
	add	r0, r0, #40960
	mvn	lr, #0
	ldrb	ip, [r3, #2]	@ zero_extendqisi2
	mov	r1, #0
	strb	ip, [r0, #523]
	ldrb	r5, [r3, #4]	@ zero_extendqisi2
	strb	r5, [r0, #528]
	ldrb	r3, [r3, #5]	@ zero_extendqisi2
	str	lr, [r0, #2160]
	strb	r1, [r0, #531]
	strb	r3, [r0, #532]
	ldrb	lr, [r2, #704]	@ zero_extendqisi2
	cmp	lr, #1
	beq	.L4461
	cmp	r3, #255
	beq	.L4462
.L4297:
	ldr	r3, [r8, #68]
	mov	r0, #21
	ldr	r2, [r4, #88]
	ldr	r1, .L4469+12
	blx	r3
	mov	r0, r4
	bl	MVC_SliceCheck
	subs	r5, r0, #0
	mov	r0, r4
	bne	.L4463
	bl	MVC_DecSlice
	cmn	r0, #2
	beq	.L4308
	cmp	r0, #0
	beq	.L4439
	ldr	r3, .L4469+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4438
	ldr	r0, [r4, #120]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L4438:
	mvn	r0, #0
	b	.L4308
.L4284:
	ldr	r1, .L4469+20
	mov	r2, #6
	ldr	r3, [r8, #68]
	mov	r0, #22
	blx	r3
	mov	r0, r4
	bl	MVC_DecSEI
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4315
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4315:
	cmp	r5, #0
	bne	.L4464
.L4439:
	mov	r0, #0
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L4285:
	ldr	r1, .L4469+24
	mov	r2, #7
	ldr	ip, [r8, #68]
	mov	r0, #22
	ldr	r3, [r4, #72]
	blx	ip
	mov	r0, r4
	bl	MVC_DecSPS
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
	bne	.L4465
	ldr	r3, [r4, #72]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #72]
	b	.L4308
.L4287:
	ldr	r3, [r8, #68]
	mov	r2, #9
	ldr	r1, .L4469+28
.L4446:
	mov	r0, #22
	blx	r3
.L4451:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4439
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #232]
.L4308:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L4288:
	ldr	r3, [r8, #68]
	mov	r2, #10
	ldr	r1, .L4469+32
	b	.L4446
.L4286:
	ldr	r1, .L4469+36
	mov	r2, #8
	ldr	ip, [r8, #68]
	mov	r0, #22
	ldr	r3, [r4, #76]
	blx	ip
	mov	r0, r4
	bl	MVC_DecPPS
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
	bne	.L4466
	ldr	r3, [r4, #76]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #76]
	b	.L4308
.L4289:
	ldr	r3, [r8, #68]
	mov	r2, #11
	ldr	r1, .L4469+40
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
	bne	.L4451
	ldr	r5, .L4469+16
	mov	r3, #1
	strb	r3, [r4, #1]
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4318
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L4318:
	mov	r6, #0
	mov	r0, r4
	strb	r6, [r4]
	bl	MVC_DecVDM
	cmp	r0, r6
	beq	.L4451
	b	.L4454
.L4290:
	ldr	r3, [r8, #68]
	mov	r2, #12
	ldr	r1, .L4469+44
	b	.L4446
.L4291:
	ldr	r3, [r8, #68]
	mov	r2, #13
	ldr	r1, .L4469+48
	b	.L4446
.L4292:
	ldr	r3, [r8, #68]
	mov	r2, #14
	ldr	r1, .L4469+52
	b	.L4446
.L4293:
	ldr	r1, .L4469+56
	mov	r2, #15
	ldr	r3, [r8, #68]
	mov	r0, #22
	blx	r3
	mov	r0, r4
	bl	MVC_DecSubSPS
	ldr	r1, [r4, #232]
	cmp	r1, #0
	mov	r5, r0
	beq	.L4319
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4319:
	cmp	r5, #0
	bne	.L4467
	ldr	r3, [r4, #20]
	mov	r0, r5
	add	r3, r3, #1
	str	r3, [r4, #20]
	b	.L4308
.L4294:
	ldr	r3, [r8, #68]
	mov	r2, #19
	ldr	r1, .L4469+60
	b	.L4446
.L4295:
	ldr	r3, [r8, #68]
	mov	r2, #30
	ldr	r1, .L4469+64
	mov	r0, #22
	blx	r3
	cmp	r5, #0
	bne	.L4321
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4321
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	str	r5, [r4, #232]
.L4321:
	mov	r1, #32
	mov	r0, r7
	bl	BsGet
	mov	r1, #32
	mov	r5, r0
	mov	r0, r7
	bl	BsGet
	ldr	r3, .L4469+68
	rev	r5, r5
	ldr	r2, [r3, #440]
	ldr	r3, [r3, #436]
	rev	r0, r0
	cmp	r0, r2
	cmpeq	r5, r3
	bne	.L4439
	ldr	r5, .L4469+16
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4322
	mov	r3, #0
	mov	r1, #112
	mov	r2, r3
	ldr	r0, [r4, #120]
	blx	ip
.L4322:
	mov	r6, #0
	mov	r0, r4
	strb	r6, [r4]
	bl	MVC_DecVDM
	cmp	r0, r6
	beq	.L4439
.L4454:
	ldr	ip, [r5]
	cmp	ip, r6
	beq	.L4438
	ldr	r0, [r4, #120]
	mov	r3, r6
	mov	r2, r6
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4308
.L4281:
	ldr	ip, [r8, #68]
	mov	r3, r6
	ldr	r1, .L4469+72
	mov	r0, #22
	blx	ip
	cmp	r6, #256
	ldr	r5, .L4469
	beq	.L4323
	ldr	r3, [r5, #68]
	mov	r0, #1
	ldr	r1, .L4469+76
	blx	r3
.L4323:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4324
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4324:
	ldr	r3, .L4469+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4439
.L4443:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	b	.L4439
.L4459:
	ldr	r3, [r8, #68]
	mov	r0, #22
	ldr	r1, .L4469+80
	blx	r3
	ldr	r3, [r4, #68]
	b	.L4280
.L4460:
	cmp	ip, #0
	movne	r2, #1
	moveq	r2, #5
	strb	r2, [r3, #2]
	ldr	r3, [r4, #232]
	b	.L4278
.L4463:
	bl	MVC_ClearCurrSlice
	ldr	r3, .L4469+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4438
.L4307:
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4308
.L4466:
	ldr	r5, .L4469+16
	mov	r0, #1
	ldr	r3, [r8, #68]
	ldr	r1, .L4469+84
	blx	r3
	ldr	ip, [r5]
	cmp	ip, #0
	beq	.L4438
.L4456:
	sub	r2, fp, #36
	mvn	r6, #0
	mov	r3, #4
	mov	r1, #100
	str	r6, [r2, #-4]!
	ldr	r0, [r4, #120]
	blx	ip
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4307
	b	.L4438
.L4467:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L4469+88
	blx	r3
	ldr	r3, .L4469+16
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4438
	sub	r2, fp, #36
	ldr	r0, [r4, #120]
	mov	r3, #4
	mvn	r4, #0
	mov	r1, #100
	str	r4, [r2, #-4]!
	blx	ip
	mov	r0, r4
	b	.L4308
.L4465:
	ldr	r5, .L4469+16
	mov	r0, #1
	ldr	r3, [r8, #68]
	ldr	r1, .L4469+92
	blx	r3
	ldr	ip, [r5]
	cmp	ip, #0
	bne	.L4456
	b	.L4438
.L4464:
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L4469+96
	blx	r3
	ldr	r3, .L4469+16
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L4443
	b	.L4439
.L4462:
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq	.L4297
	add	r3, r4, #10747904
	add	r3, r3, #20480
	ldr	r2, [r3, #2384]
	cmp	r2, #0
	beq	.L4300
	ldrb	r2, [r3, #2380]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L4301
.L4300:
	add	r3, r4, #12992
	mov	r2, #0
	add	r3, r3, #16
.L4304:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L4302
	ldrb	r1, [r3, #-4]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L4468
.L4302:
	add	r2, r2, #1
	add	r3, r3, #335872
	cmp	r2, #32
	add	r3, r3, #308
	bne	.L4304
	mvn	r3, #0
.L4437:
	cmn	r3, #1
	str	r3, [r0, #2160]
	beq	.L4297
	sub	ip, ip, #5
	mov	r3, #1
	clz	ip, ip
	strb	r3, [r0, #531]
	strb	r3, [r0, #530]
	mov	ip, ip, lsr #5
	strb	ip, [r0, #529]
	b	.L4297
.L4461:
	strb	lr, [r0, #531]
	ldr	r3, [r2, #712]
	str	r3, [r0, #2160]
	ldrb	r3, [r2, #708]	@ zero_extendqisi2
	strb	r3, [r0, #529]
	ldrb	r3, [r2, #709]	@ zero_extendqisi2
	strb	r3, [r0, #530]
	strb	r1, [r2, #704]
	b	.L4297
.L4468:
	movw	r3, #8500
	movt	r3, 5
	mla	r3, r3, r2, r4
	add	r3, r3, #12992
	add	r3, r3, #16
	ldr	r3, [r3, #4]
	b	.L4437
.L4301:
	ldr	r3, [r3, #2388]
	b	.L4437
.L4470:
	.align	2
.L4469:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC458
	.word	.LC460
	.word	.LC461
	.word	g_event_report
	.word	.LC466
	.word	.LC464
	.word	.LC468
	.word	.LC469
	.word	.LC462
	.word	.LC471
	.word	.LC470
	.word	.LC472
	.word	.LC473
	.word	.LC474
	.word	.LC475
	.word	.LC476
	.word	.LANCHOR1
	.word	.LC477
	.word	.LC478
	.word	.LC459
	.word	.LC463
	.word	.LC333
	.word	.LC465
	.word	.LC467
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	cmp	r1, #0
	mov	ip, #0
	mov	r4, r0
	str	ip, [fp, #-40]
	str	ip, [fp, #-44]
	str	ip, [fp, #-48]
	beq	.L4525
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
	beq	.L4474
	ldr	r3, [r2, #852]
	cmp	r3, #1
	addne	r5, r0, #11141120
	addne	r5, r5, #12288
	beq	.L4526
.L4475:
	mov	r0, r4
	bl	MVC_ReceivePacket
	ldr	r3, [r4, #232]
	str	r3, [r5, #428]
.L4477:
	cmp	r0, #0
	mov	r3, #0
	strb	r3, [r4, #5]
	beq	.L4478
.L4497:
	mov	r0, #0
.L4520:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L4526:
	ldr	r3, [r2, #892]
	add	r5, r0, #11141120
	add	r5, r5, #12288
	cmp	r3, #1
	bne	.L4475
	ldr	r3, [r5, #424]
	cmp	r3, #1
	bne	.L4475
	ldr	r3, [r5, #428]
	str	r3, [r0, #232]
	str	ip, [r5, #424]
	strb	ip, [r0, #5]
	b	.L4476
.L4478:
	ldr	r3, [r4, #232]
.L4476:
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
	beq	.L4527
.L4523:
	and	r5, r5, #27
	mov	r3, #0
	cmp	r5, #1
	strb	r3, [r4, #10]
	beq	.L4483
.L4484:
	mov	r0, r4
	bl	MVC_FindTrailZeros
	cmp	r0, #0
	bne	.L4528
.L4496:
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
	beq	.L4520
	cmp	r0, r5
	beq	.L4497
	ldr	r3, .L4531
	ldr	ip, [r3]
	cmp	ip, r5
	beq	.L4524
	ldr	r0, [r4, #120]
	mov	r3, r5
	mov	r2, r5
	mov	r1, #113
	blx	ip
.L4524:
	mvn	r0, #0
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L4474:
	ldr	r0, [r0, #232]
	clz	r0, r0
	mov	r0, r0, lsr #5
	rsb	r0, r0, #0
	b	.L4477
.L4527:
	mov	r1, #24
	mov	r0, r6
	bl	BsGet
	ldr	r3, [r4, #232]
	ubfx	r2, r0, #23, #1
	strb	r2, [r3, #5]
	ldr	r1, [r4, #232]
	ldrsb	r2, [r1, #5]
	cmp	r2, #0
	bne	.L4523
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
.L4483:
	sub	r2, fp, #32
	sub	r1, fp, #36
	mov	r0, r4
	bl	MVC_InquireSliceProperty
	cmp	r0, #0
	bne	.L4485
	ldr	r3, [fp, #-36]
	ldr	r2, [r4, #128]
	sub	r3, r3, #1
	clz	r3, r3
	mov	r3, r3, lsr #5
	cmp	r3, #0
	add	r3, r2, r3
	str	r3, [r4, #128]
	bne	.L4529
.L4486:
	ldr	r3, [fp, #-32]
	cmp	r3, #1
	bne	.L4484
	mvn	r1, #0
	mov	r0, r4
	bl	MVC_FlushDPB
	cmp	r0, #0
	bne	.L4530
.L4493:
	ldr	r0, [r4, #120]
	bl	FSP_GetFspType
	cmp	r0, #0
	bne	.L4484
	sub	r3, fp, #40
	sub	r2, fp, #44
	sub	r1, fp, #48
	ldr	r0, [r4, #120]
	bl	VCTRL_GetChanImgNum
	b	.L4484
.L4485:
	ldr	r1, [r4, #232]
	cmp	r1, #0
	beq	.L4495
	ldr	r0, [r4, #120]
	bl	MVC_ReleaseNAL
	mov	r3, #0
	str	r3, [r4, #232]
.L4495:
	ldr	r3, .L4531
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L4524
	mov	r3, #0
	ldr	r0, [r4, #120]
	mov	r2, r3
	mov	r1, #113
	blx	ip
	mvn	r0, #0
	b	.L4520
.L4528:
	ldr	r3, .L4531+4
	mov	r0, #22
	ldr	r1, .L4531+8
	ldr	r3, [r3, #68]
	blx	r3
	b	.L4496
.L4529:
	add	r5, r4, #11075584
	add	r5, r5, #40960
	ldrb	r3, [r5, #521]	@ zero_extendqisi2
	cmp	r3, #1
	ldr	r3, [r4, #132]
	addeq	r3, r3, #1
	addne	r3, r3, #2
	str	r3, [r4, #132]
	ldr	r3, [r4, #64]
	cmp	r3, #0
	beq	.L4486
	ldr	r3, [r4, #232]
	mov	ip, #0
	mov	r0, r6
	mov	r2, #12
	ldr	r1, .L4531+12
	str	ip, [r3, #64]
	bl	BsInit
	mov	r1, #1
	mov	r0, r4
	bl	MVC_DecOneNal
	ldrb	r3, [r5, #521]	@ zero_extendqisi2
	mvn	r0, #1
	cmp	r3, #1
	ldr	r3, [r4, #132]
	subeq	r3, r3, #1
	subne	r3, r3, #2
	str	r3, [r4, #132]
	mov	r3, #1
	strb	r3, [r4, #5]
	b	.L4520
.L4530:
	ldr	ip, .L4531+4
	mov	r3, #0
	movw	r2, #14143
	ldr	r1, .L4531+16
	mov	r0, #22
	ldr	ip, [ip, #68]
	blx	ip
	b	.L4493
.L4525:
	ldr	ip, .L4531+4
	mov	r0, r1
	movw	r3, #14026
	ldr	r2, .L4531+20
	ldr	r1, .L4531+24
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L4520
.L4532:
	.align	2
.L4531:
	.word	g_event_report
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC479
	.word	.LANCHOR1+432
	.word	.LC438
	.word	.LC13
	.word	.LC14
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
	add	r6, r6, #8192
	str	r1, [r6, #2164]
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
	add	r3, r3, #8192
	str	r1, [r3, #2164]
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
	str	ip, [r3, #2188]
	mov	r5, #2
	strb	r1, [r3, #2176]
	mov	r4, #7
	str	r1, [r3, #2208]
	mov	lr, #32
	str	r1, [r3, #2212]
	mov	ip, #3
	str	r2, [r3, #2192]
	mvn	r1, #0
	mov	r2, #262144
	strb	r5, [r3, #2177]
	strb	r4, [r3, #2179]
	strb	lr, [r3, #2178]
	str	ip, [r3, #2200]
	str	r1, [r3, #2204]
	str	r2, [r3, #2216]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4545:
	ldr	r3, [r6, #2112]
	mov	r2, #1
	ldr	r0, [r4, #120]
	ldrsb	r1, [r3, #6]
	bl	FSP_ClearLogicFs
	ldr	r3, .L4548
	mov	r2, r5
	ldr	r1, .L4548+4
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	bl	MVC_ClearCurrPic
	mov	r0, r4
	mvn	r1, #0
	bl	MVC_ClearDPB
	str	r7, [r6, #2112]
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L4547:
	ldr	r3, .L4548
	mov	r0, #1
	ldr	r1, .L4548+8
	ldr	r3, [r3, #68]
	blx	r3
	b	.L4538
.L4546:
	ldr	r3, .L4548
	mov	r0, #1
	ldr	r1, .L4548+12
	ldr	r3, [r3, #68]
	blx	r3
	b	.L4537
.L4549:
	.align	2
.L4548:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC480
	.word	.LC482
	.word	.LC481
	UNWIND(.fnend)
	.size	MVC_DEC_VDMPostProc, .-MVC_DEC_VDMPostProc
	.align	2
	.global	MVC_DEC_GetImageBuffer
	.type	MVC_DEC_GetImageBuffer, %function
MVC_DEC_GetImageBuffer:
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
	ldr	r7, [r0, #48]
	mov	r6, r0
	cmp	r7, #0
	beq	.L4566
	movw	r4, #47800
	movw	r9, #47240
	movt	r4, 169
	add	r8, r0, #11075584
	add	r4, r0, r4
	movt	r9, 169
	add	r8, r8, #45056
	add	r9, r0, r9
	mov	ip, r4
	mov	r5, #0
.L4558:
	ldrb	r3, [ip, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L4553
	cmp	ip, #0
	beq	.L4554
	ldr	lr, [r8, #2376]
	cmp	lr, #0
	beq	.L4554
	ldr	r2, [r8, #2184]
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
	ldr	r0, [r6, #120]
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	beq	.L4566
	ldr	r0, [r6, #120]
	bl	FSP_IsNewFsAvalible
	cmn	r0, #1
	bne	.L4588
	add	r1, r6, #584
	ldr	r0, [r6, #120]
	bl	FSP_ClearNotInVoQueue
	b	.L4588
.L4553:
	add	r5, r5, #1
	add	ip, ip, #688
	cmp	r5, r7
	bne	.L4558
	ldr	r8, .L4592
	sub	r2, fp, #48
	sub	r1, fp, #52
	add	r0, r6, #584
	bl	GetQueueImgNum
	ldr	r1, .L4592+4
	ldr	r3, [r8, #68]
	mov	r0, #0
	blx	r3
	ldr	r3, [r8, #68]
	ldr	r1, .L4592+8
	mov	r0, #0
	blx	r3
	ldr	r3, [r6, #48]
	cmp	r3, #0
	beq	.L4590
	add	r7, r6, #11075584
	movw	r3, #47240
	add	r7, r7, #45056
	mov	r5, #0
	movt	r3, 169
	add	r3, r6, r3
	str	r3, [fp, #-56]
.L4559:
	cmp	r4, #0
	ldr	ip, [r8, #68]
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r10, [r4, #5]	@ zero_extendqisi2
	beq	.L4570
	ldr	r9, [r7, #2376]
	cmp	r9, #0
	beq	.L4570
	ldr	r2, [r7, #2184]
	rsb	r1, r4, r2
	cmp	r2, #0
	clz	r1, r1
	mov	r1, r1, lsr #5
	moveq	r1, #0
	cmp	r1, #0
	bne	.L4572
	ldr	r2, [fp, #-56]
	b	.L4563
.L4564:
	ldr	r0, [r2, #4]!
	rsb	lr, r4, r0
	cmp	r0, #0
	clz	lr, lr
	mov	lr, lr, lsr #5
	moveq	lr, #0
	cmp	lr, #0
	bne	.L4572
.L4563:
	add	r1, r1, #1
	cmp	r1, r9
	bne	.L4564
.L4570:
	mov	r2, #1
.L4562:
	str	r2, [sp, #4]
	mov	r0, #0
	mov	r2, r5
	str	r10, [sp]
	ldr	r1, .L4592+12
	add	r5, r5, #1
	blx	ip
	ldr	r3, [r6, #48]
	add	r4, r4, #688
	cmp	r3, r5
	bhi	.L4559
.L4565:
	ldr	r3, [fp, #-48]
	mov	r0, #0
	ldr	r2, [fp, #-52]
	ldr	r1, .L4592+16
	ldr	ip, [r8, #68]
	blx	ip
	ldr	r3, [r6, #48]
	ldr	r0, [r7, #2380]
	ldr	r2, [fp, #-52]
	sub	r3, r3, #2
	ldr	r1, [fp, #-48]
	rsb	r3, r0, r3
	ldr	r0, .L4592
	add	r2, r2, r1
	cmp	r2, r3
	blt	.L4591
.L4588:
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4566:
	mov	r0, #1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4572:
	mov	r2, #0
	b	.L4562
.L4591:
	ldr	r3, [r0, #68]
	mov	r0, #0
	ldr	r1, .L4592+20
	blx	r3
	mov	r0, r6
	mov	r1, #1
	bl	MVC_ClearAll
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L4590:
	add	r7, r6, #11075584
	add	r7, r7, #45056
	b	.L4565
.L4593:
	.align	2
.L4592:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC483
	.word	.LC484
	.word	.LC486
	.word	.LC485
	.word	.LC487
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
	.type	__FUNCTION__.14712, %object
	.size	__FUNCTION__.14712, 18
__FUNCTION__.14712:
	.ascii	"MVC_WriteSliceMsg\000"
	.space	2
	.type	MVC_SarTable.15006, %object
	.size	MVC_SarTable.15006, 136
MVC_SarTable.15006:
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
	.type	__func__.15240, %object
	.size	__func__.15240, 11
__func__.15240:
	.ascii	"MVC_DecSEI\000"
	.space	1
	.type	__func__.13892, %object
	.size	__func__.13892, 17
__func__.13892:
	.ascii	"MVC_DirectOutput\000"
	.space	3
	.type	__func__.14472, %object
	.size	__func__.14472, 12
__func__.14472:
	.ascii	"MVC_InitDPB\000"
	.type	__FUNCTION__.13612, %object
	.size	__FUNCTION__.13612, 13
__FUNCTION__.13612:
	.ascii	"MVC_ClearDPB\000"
	.space	3
	.type	__func__.15351, %object
	.size	__func__.15351, 17
__func__.15351:
	.ascii	"MVC_FlushDecoder\000"
	.space	3
	.type	__FUNCTION__.13620, %object
	.size	__FUNCTION__.13620, 13
__FUNCTION__.13620:
	.ascii	"MVC_ClearAll\000"
	.space	3
	.type	__FUNCTION__.15478, %object
	.size	__FUNCTION__.15478, 13
__FUNCTION__.15478:
	.ascii	"MVC_DEC_Init\000"
	.space	3
	.type	__func__.14042, %object
	.size	__func__.14042, 18
__func__.14042:
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
	ASCII(.ascii	"\012 VFMW ** release streambuff=%08x   bitstream_le" )
	ASCII(.ascii	"ngth=%d\012\000" )
	.space	3
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
	ASCII(.ascii	"dar=%d\012\000" )
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
	ASCII(.ascii	"MVC VUI: vui_mvc_temporal_id[]\000" )
	.space	1
.LC301:
	ASCII(.ascii	"SUBSPS VUI: vui_mvc_num_target_output_views_minus1[" )
	ASCII(.ascii	"]\000" )
	.space	3
.LC302:
	ASCII(.ascii	"MVC VUI: vui_mvc_timing_info_present_flag\000" )
	.space	2
.LC303:
	ASCII(.ascii	"MVC VUI: vui_mvc_view_id[][]\000" )
	.space	3
.LC304:
	ASCII(.ascii	"MVC VUI: vui_mvc_num_units_in_tick\000" )
	.space	1
.LC305:
	ASCII(.ascii	"MVC VUI: vui_mvc_time_scale[]\000" )
	.space	2
.LC306:
	ASCII(.ascii	"MVC VUI: vui_mvc_fixed_frame_rate_flag\000" )
	.space	1
.LC307:
	ASCII(.ascii	"MVC VUI: vui_mvc_nal_hrd_parameters_present_flag[]\000" )
	.space	1
.LC308:
	ASCII(.ascii	"MVC VUI: cpb_cnt_minus1\000" )
.LC309:
	ASCII(.ascii	"MVC VUI: bit_rate_scale\000" )
.LC310:
	ASCII(.ascii	"MVC VUI: cpb_size_scale\000" )
.LC311:
	ASCII(.ascii	"MVC VUI: bit_rate_value_minus1\000" )
	.space	1
.LC312:
	ASCII(.ascii	"MVC VUI: cpb_size_value_minus1\000" )
	.space	1
.LC313:
	ASCII(.ascii	"MVC VUI: cbr_flag\000" )
	.space	2
.LC314:
	ASCII(.ascii	"MVC VUI: initial_cpb_removal_delay_length_minus1\000" )
	.space	3
.LC315:
	ASCII(.ascii	"MVC VUI: cpb_removal_delay_length_minus1\000" )
	.space	3
.LC316:
	ASCII(.ascii	"MVC VUI: dpb_output_delay_length_minus1\000" )
.LC317:
	ASCII(.ascii	"MVC VUI: time_offset_length\000" )
.LC318:
	ASCII(.ascii	"MVC VUI: vui_mvc_vcl_hrd_parameters_present_flag[]\000" )
	.space	1
.LC319:
	ASCII(.ascii	"MVC VUI: vui_mvc_low_delay_hrd_flag[]\000" )
	.space	2
.LC320:
	ASCII(.ascii	"MVC VUI: vui_mvc_pic_struct_present_flag[]\000" )
	.space	1
.LC321:
	ASCII(.ascii	"SUBSPS: profile_idc\000" )
.LC322:
	ASCII(.ascii	"SUBSPS: constrained_set0_flag\000" )
	.space	2
.LC323:
	ASCII(.ascii	"SUBSPS: constrained_set1_flag\000" )
	.space	2
.LC324:
	ASCII(.ascii	"SUBSPS: constrained_set2_flag\000" )
	.space	2
.LC325:
	ASCII(.ascii	"SUBSPS: constrained_set3_flag\000" )
	.space	2
.LC326:
	ASCII(.ascii	"SUBSPS: constrained_set4_flag\000" )
	.space	2
.LC327:
	ASCII(.ascii	"SUBSPS: constrained_set5_flag\000" )
	.space	2
.LC328:
	ASCII(.ascii	"SUBSPS: reserved_zero_2bits\000" )
.LC329:
	ASCII(.ascii	"SUBSPS: level_idc\000" )
	.space	2
.LC330:
	ASCII(.ascii	"SUBSPS: seq_parameter_set_id\000" )
	.space	3
.LC331:
	ASCII(.ascii	"profile_idc = %5d error\012\000" )
	.space	3
.LC332:
	ASCII(.ascii	"level_idc = %5d error, try to decode as level_idc 4" )
	ASCII(.ascii	"1.\012\000" )
	.space	1
.LC333:
	ASCII(.ascii	"SUBSPS decode error.\012\000" )
	.space	2
.LC334:
	ASCII(.ascii	"SUBSPS: bit_equal_to_one\000" )
	.space	3
.LC335:
	ASCII(.ascii	"SUBSPS Mvc Ext decode error.\012\000" )
	.space	2
.LC336:
	ASCII(.ascii	"SUBSPS: mvc_vui_parameters_present_flag\000" )
.LC337:
	ASCII(.ascii	"SUBSPS Mvc Vui Ext decode error.\012\000" )
	.space	2
.LC338:
	ASCII(.ascii	"SEI: frame_packing_arrangement_id\000" )
	.space	2
.LC339:
	ASCII(.ascii	"SEI: frame_packing_arrangement_cancel_flag\000" )
	.space	1
.LC340:
	ASCII(.ascii	"SEI: frame_packing_arrangement_type\000" )
.LC341:
	ASCII(.ascii	"SEI: quincunx_sampling_flag\000" )
.LC342:
	ASCII(.ascii	"SEI: content_interpretation_type\000" )
	.space	3
.LC343:
	ASCII(.ascii	"SEI: spatial_flipping_flag\000" )
	.space	1
.LC344:
	ASCII(.ascii	"SEI: frame0_flipped_flag\000" )
	.space	3
.LC345:
	ASCII(.ascii	"SEI: field_views_flag\000" )
	.space	2
.LC346:
	ASCII(.ascii	"SEI: current_frame_is_frame0_flag\000" )
	.space	2
.LC347:
	ASCII(.ascii	"SEI: frame0_self_contained_flag\000" )
.LC348:
	ASCII(.ascii	"SEI: frame1_self_contained_flag\000" )
.LC349:
	ASCII(.ascii	"SEI: frame0_grid_position_x\000" )
.LC350:
	ASCII(.ascii	"SEI: frame0_grid_position_y\000" )
.LC351:
	ASCII(.ascii	"SEI: frame1_grid_position_x\000" )
.LC352:
	ASCII(.ascii	"SEI: frame1_grid_position_y\000" )
.LC353:
	ASCII(.ascii	"SEI: frame_packing_arrangement_reserved_byte\000" )
	.space	3
.LC354:
	ASCII(.ascii	"SEI: frame_packing_arrangement_repetition_period\000" )
	.space	3
.LC355:
	ASCII(.ascii	"SEI: frame_packing_arrangement_extension_flag\000" )
	.space	2
.LC356:
	ASCII(.ascii	"DecPicTimingSEI but SPS is invalid.\012\000" )
	.space	3
.LC357:
	ASCII(.ascii	"SEI: cpb_removal_delay\000" )
	.space	1
.LC358:
	ASCII(.ascii	"SEI: dpb_output_delay\000" )
	.space	2
.LC359:
	ASCII(.ascii	"pic_struct\000" )
	.space	1
.LC360:
	ASCII(.ascii	"SEI nal dec payload type err\012\000" )
	.space	2
.LC361:
	ASCII(.ascii	"SEI nal dec payloadSize err\012\000" )
	.space	3
.LC362:
	ASCII(.ascii	"MVC_SEI_BUFFERING_PERIOD,offset = %d,payload_size =" )
	ASCII(.ascii	" %d \012\000" )
	.space	3
.LC363:
	ASCII(.ascii	"MVC_PassBytes err [%s][%d]\012\000" )
.LC364:
	ASCII(.ascii	"MVC_SEI_PIC_TIMING,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC365:
	ASCII(.ascii	"MVC_SEI_PAN_SCAN_RECT,offset = %d,payload_size = %d" )
	ASCII(.ascii	" \012\000" )
	.space	2
.LC366:
	ASCII(.ascii	"MVC_SEI_FILLER_PAYLOAD,offset = %d,payload_size = %" )
	ASCII(.ascii	"d \012\000" )
	.space	1
.LC367:
	ASCII(.ascii	"MVC_SEI_USER_DATA_REGISTERED_ITU_T_T35, offset = %d" )
	ASCII(.ascii	", payload_size = %d \012\000" )
	.space	3
.LC368:
	ASCII(.ascii	"MVC_SEI_USER_DATA_UNREGISTERED, offset = %d, payloa" )
	ASCII(.ascii	"d_size = %d \012\000" )
	.space	3
.LC369:
	ASCII(.ascii	"SEI: itu_t_t35_country_code\000" )
.LC370:
	ASCII(.ascii	"SEI: itu_t_t35_country_code_extension_byte\000" )
	.space	1
.LC371:
	ASCII(.ascii	"SEI: itu_t_t35_provider_code\000" )
	.space	3
.LC372:
	ASCII(.ascii	"cann't dec usrdata\012\000" )
.LC373:
	ASCII(.ascii	"MVC_SEI_RECOVERY_POINT,offset = %d,payload_size = %" )
	ASCII(.ascii	"d \012\000" )
	.space	1
.LC374:
	ASCII(.ascii	"MVC_SEI_DEC_REF_PIC_MARKING_REPETITION,offset = %d," )
	ASCII(.ascii	"payload_size = %d \012\000" )
	.space	1
.LC375:
	ASCII(.ascii	"MVC_SEI_SPARE_PIC,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC376:
	ASCII(.ascii	"MVC_SEI_SCENE_INFO,offset = %d,payload_size = %d \012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC377:
	ASCII(.ascii	"MVC_SEI_SUB_SEQ_INFO,offset = %d,payload_size = %d " )
	ASCII(.ascii	"\012\000" )
	.space	3
.LC378:
	ASCII(.ascii	"MVC_SEI_SUB_SEQ_LAYER_CHARACTERISTICS,offset = %d,p" )
	ASCII(.ascii	"ayload_size = %d \012\000" )
	.space	2
.LC379:
	ASCII(.ascii	"MVC_SEI_SUB_SEQ_CHARACTERISTICS,offset = %d,payload" )
	ASCII(.ascii	"_size = %d \012\000" )
.LC380:
	ASCII(.ascii	"MVC_SEI_FULL_FRAME_FREEZE,offset = %d,payload_size " )
	ASCII(.ascii	"= %d \012\000" )
	.space	2
.LC381:
	ASCII(.ascii	"MVC_SEI_FULL_FRAME_FREEZE_RELEASE,offset = %d,paylo" )
	ASCII(.ascii	"ad_size = %d \012\000" )
	.space	2
.LC382:
	ASCII(.ascii	"MVC_SEI_FULL_FRAME_SNAPSHOT,offset = %d,payload_siz" )
	ASCII(.ascii	"e = %d \012\000" )
.LC383:
	ASCII(.ascii	"MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START,offset" )
	ASCII(.ascii	" = %d,payload_size = %d \012\000" )
	.space	3
.LC384:
	ASCII(.ascii	"MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END,offset =" )
	ASCII(.ascii	" %d,payload_size = %d \012\000" )
	.space	1
.LC385:
	ASCII(.ascii	"MVC_SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,offset =" )
	ASCII(.ascii	" %d,payload_size = %d \012\000" )
	.space	1
.LC386:
	ASCII(.ascii	"MVC_SEI_FILM_GRAIN_CHARACTERISTICS,offset = %d,payl" )
	ASCII(.ascii	"oad_size = %d \012\000" )
	.space	1
.LC387:
	ASCII(.ascii	"MVC_SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,offset" )
	ASCII(.ascii	" = %d,payload_size = %d \012\000" )
	.space	3
.LC388:
	ASCII(.ascii	"MVC_SEI_STEREO_VIDEO_INFO,offset = %d,payload_size " )
	ASCII(.ascii	"= %d \012\000" )
	.space	2
.LC389:
	ASCII(.ascii	"too much usrdat, cann't dec SEI\012\000" )
	.space	3
.LC390:
	ASCII(.ascii	"\012 mvc the stream is error,len:%d\012\000" )
	.space	2
.LC391:
	ASCII(.ascii	"\012Slice nal or IDR nal = %d\012\000" )
.LC392:
	ASCII(.ascii	"sliceheader dec err\012\000" )
	.space	3
.LC393:
	ASCII(.ascii	"MVC_InquireSliceProperty mvc flag %d is wrong\012\000" )
	.space	1
.LC394:
	ASCII(.ascii	"MvcDec destroy\012\000" )
.LC395:
	ASCII(.ascii	"MVC recycle image\012\000" )
	.space	1
.LC396:
	ASCII(.ascii	"pH264Ctx is %#x, pFrameStore is %#x\012\000" )
	.space	3
.LC397:
	ASCII(.ascii	"line: %d, pstLogicFsImage is NULL\012\000" )
	.space	1
.LC398:
	ASCII(.ascii	"InsertVO err:%d, MVC_ClearAll\012\000" )
	.space	1
.LC399:
	ASCII(.ascii	"mvc.c line %d: MVC_FRAME para err(ret=%d), recycle " )
	ASCII(.ascii	"image self\012\000" )
	.space	1
.LC400:
	ASCII(.ascii	"[%s][%d] MVC_OutputFrmToVO return %d !\012\000" )
.LC401:
	ASCII(.ascii	"[%s][%d] MVC_OutputFrmToVO return %d\012\000" )
	.space	2
.LC402:
	ASCII(.ascii	"%s %d MVC_OK != MVC_FlushDPB!!\012\000" )
.LC403:
	ASCII(.ascii	"%s %d MVC_OK != MVC_InitDPB!!\012\000" )
	.space	1
.LC404:
	ASCII(.ascii	"----------------- MVC_IMODE -> %d -----------------" )
	ASCII(.ascii	"-\012\000" )
	.space	2
.LC405:
	ASCII(.ascii	"Discard this B(poc=%d) befor P, is_ref_idc=%d.\012\000" )
.LC406:
	ASCII(.ascii	"***** VDM start, TotalPicNum=%d, pMvcCtx->CurrPic.s" )
	ASCII(.ascii	"tructure=%d.\012\000" )
	.space	3
.LC407:
	ASCII(.ascii	"MVC_DecVDM fail [%s][%d]\012\000" )
	.space	2
.LC408:
	ASCII(.ascii	"\012 receive streambuff=0x%08x; phy_addr=0x%x; bits" )
	ASCII(.ascii	"tream_length=%d\012\000" )
	.space	3
.LC409:
	ASCII(.ascii	"nal_releaMVC_SE_ERR\012\000" )
	.space	3
.LC410:
	ASCII(.ascii	"cann't find slot for current nal\012\000" )
	.space	2
.LC411:
	ASCII(.ascii	"receive a zero packet\012\000" )
	.space	1
.LC412:
	ASCII(.ascii	"clear all dec para\012\000" )
.LC413:
	ASCII(.ascii	"line %d, get APC error, ret %d\012\000" )
.LC414:
	ASCII(.ascii	"InserFrmInDPB: cur pic struct = %d!\012\000" )
	.space	3
.LC415:
	ASCII(.ascii	"line: %d, pImage is NULL!\012\000" )
	.space	1
.LC416:
	ASCII(.ascii	"FS_ALLOC_ERR, MVC_ClearAll\012\000" )
.LC417:
	ASCII(.ascii	"Can not new logic fs! MVC_ClearAll\012\000" )
.LC418:
	ASCII(.ascii	"%s %d NULL== pMvcCtx->SPS || NULL == pMvcCtx->PPS |" )
	ASCII(.ascii	"| NULL == pMvcCtx->DecSlicePara!!\012\000" )
	.space	2
.LC419:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC420:
	ASCII(.ascii	"CurrPic.state is 'MVC_EMPTY'\012\000" )
	.space	2
.LC421:
	ASCII(.ascii	"MVC_StorePicInDpb return(%d) from L%d\012\000" )
	.space	1
.LC422:
	ASCII(.ascii	"Is IDR, but init DPB failed!\012\000" )
	.space	2
.LC423:
	ASCII(.ascii	"Is IDR, but FlushDPB failed!\012\000" )
	.space	2
.LC424:
	ASCII(.ascii	"MVC_DirectOutput mode\012\000" )
	.space	1
.LC425:
	ASCII(.ascii	"line %d, REPORT_IFRAME_ERR\012\000" )
.LC426:
	ASCII(.ascii	"line %d, return %d\012\000" )
.LC427:
	ASCII(.ascii	"err(%d) > ref_thr(%d)\012\000" )
	.space	1
.LC428:
	ASCII(.ascii	"MVC_Marking return %d\012\000" )
	.space	1
.LC429:
	ASCII(.ascii	"line %d: pMvcCtx->DPB.size = %d, ref %d, ltref %d\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC430:
	ASCII(.ascii	"line %d, MVC_GetMinPOC failed\012\000" )
	.space	1
.LC431:
	ASCII(.ascii	"%s: pos(%d) = pre_pos, force return.\012\000" )
	.space	2
.LC432:
	ASCII(.ascii	"GAP found while DPB is MVC_EMPTY!\012\000" )
	.space	1
.LC433:
	ASCII(.ascii	"MVC_FRAME num gap try to fill it \012\000" )
	.space	1
.LC434:
	ASCII(.ascii	"CurrFrameNum = %d  UnusedShortTermFrameNum = %d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC435:
	ASCII(.ascii	"cann't allocate MVC_FRAME store when gap find\012\000" )
	.space	1
.LC436:
	ASCII(.ascii	"allocate MVC_FRAME store when gap find\012\000" )
.LC437:
	ASCII(.ascii	"line %d: store gap pic err, ret=%d\012\000" )
.LC438:
	ASCII(.ascii	"line %d: flush dpb return %d\012\000" )
	.space	2
.LC439:
	ASCII(.ascii	"line %d: init dpb return %d\012\000" )
	.space	3
.LC440:
	ASCII(.ascii	"resolution error, the CAP_LEVEL_USER_DEFINE_WITH_OP" )
	ASCII(.ascii	"TION channel with s32ReRangeEn == 0 can't support s" )
	ASCII(.ascii	"uch bitstream.\012\000" )
	.space	2
.LC441:
	ASCII(.ascii	"s32MaxRefFrameNum < pMvcCtx->DPB.size, the CAP_LEVE" )
	ASCII(.ascii	"L_USER_DEFINE_WITH_OPTION channel can't support suc" )
	ASCII(.ascii	"h bitstream.\012\000" )
.LC442:
	ASCII(.ascii	"mem arrange err, MVC_ClearAll\012\000" )
	.space	1
.LC443:
	ASCII(.ascii	"line %d: MVC_FRAME gap(=%d) > dpb size(=%d)\012\000" )
	.space	3
.LC444:
	ASCII(.ascii	"MVC_FRAME num gap don't allowed but gap find\012\000" )
	.space	2
.LC445:
	ASCII(.ascii	"line %d: frame num(%d/%d) find gap in NON-I slice b" )
	ASCII(.ascii	"ut here gap is not allowed\012\000" )
	.space	1
.LC446:
	ASCII(.ascii	"line %d: dec gap failed\012\000" )
	.space	3
.LC447:
	ASCII(.ascii	"line %d: alloc framestore failed\012\000" )
	.space	2
.LC448:
	ASCII(.ascii	"dec_pts: %lld\012\000" )
	.space	1
.LC449:
	ASCII(.ascii	"dec_usertag: %lld\012\000" )
	.space	1
.LC450:
	ASCII(.ascii	"line %d: CurrPic.frame_store is NULL\012\000" )
	.space	2
.LC451:
	ASCII(.ascii	"get back frm\012\000" )
	.space	2
.LC452:
	ASCII(.ascii	"Start Reason: SliceParaNum, MaxBytesReceived = %d, " )
	ASCII(.ascii	"%d(thr=%d)\012\000" )
	.space	1
.LC453:
	ASCII(.ascii	"Too many slice or bitstream, err!\012\000" )
	.space	1
.LC454:
	ASCII(.ascii	"Start Reason: new_pic_flag\012\000" )
.LC455:
	ASCII(.ascii	"init pic err, find next recover point or next valid" )
	ASCII(.ascii	" sps, pps, or exit\012\000" )
	.space	1
.LC456:
	ASCII(.ascii	"MVC_DecList error, ret=%d\012\000" )
	.space	1
.LC457:
	ASCII(.ascii	"dec list err.\012\000" )
	.space	1
.LC458:
	ASCII(.ascii	"pMvcCtx->TotalNal = %d, type:%d\012\000" )
	.space	3
.LC459:
	ASCII(.ascii	"stop i want\012\000" )
	.space	3
.LC460:
	ASCII(.ascii	"***** NAL: IDR/Slice, nal_unit_type=%d, TotalSlice=" )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC461:
	ASCII(.ascii	"*******TotalPicNum=%d********\012\000" )
	.space	1
.LC462:
	ASCII(.ascii	"***** NAL: PPS, nal_unit_type=%d, TotalPPS=%d\012\000" )
	.space	1
.LC463:
	ASCII(.ascii	"PPS decode error.\012\000" )
	.space	1
.LC464:
	ASCII(.ascii	"***** NAL: SPS, nal_unit_type=%d, TotalSPS=%d\012\000" )
	.space	1
.LC465:
	ASCII(.ascii	"SPS decode error.\012\000" )
	.space	1
.LC466:
	ASCII(.ascii	"***** NAL: SEI, nal_unit_type=%d\012\000" )
	.space	2
.LC467:
	ASCII(.ascii	"SEI decode error.\012\000" )
	.space	1
.LC468:
	ASCII(.ascii	"***** NAL: AUD, nal_unit_type=%d\012\000" )
	.space	2
.LC469:
	ASCII(.ascii	"***** NAL: EOSEQ, nal_unit_type=%d\012\000" )
.LC470:
	ASCII(.ascii	"***** NAL: FILL, nal_unit_type=%d\012\000" )
	.space	1
.LC471:
	ASCII(.ascii	"***** NAL: EOSTREAM, nal_unit_type=%d\012\000" )
	.space	1
.LC472:
	ASCII(.ascii	"***** NAL: SPSEXT, nal_unit_type=%d\012\000" )
	.space	3
.LC473:
	ASCII(.ascii	"***** NAL: PREFIX, nal_unit_type=%d\012\000" )
	.space	3
.LC474:
	ASCII(.ascii	"***** NAL: SUBSPS, nal_unit_type=%d\012\000" )
	.space	3
.LC475:
	ASCII(.ascii	"***** NAL: AUX, nal_unit_type=%d\012\000" )
	.space	2
.LC476:
	ASCII(.ascii	"***** NAL: EOPIC, nal_unit_type=%d\012\000" )
.LC477:
	ASCII(.ascii	"***** NAL: UNSUPPORT, nal_unit_type=%d,nalu header:" )
	ASCII(.ascii	"%x\012\000" )
	.space	1
.LC478:
	ASCII(.ascii	"nal_header != 0x00000100 not support.\012\000" )
	.space	1
.LC479:
	ASCII(.ascii	"MVC_FindTrailZeros ERR\012\000" )
.LC480:
	ASCII(.ascii	"store pic err, ret = %d\012\000" )
	.space	3
.LC481:
	ASCII(.ascii	"MVC_IMODE nal store pic err, ret = %d\012\000" )
	.space	1
.LC482:
	ASCII(.ascii	"MVC_IMODE nal flush dpb err, ret = %d\012\000" )
	.space	1
.LC483:
	ASCII(.ascii	"cann't find FrameStore\012\000" )
.LC484:
	ASCII(.ascii	"========== MVC FrameStore state(is_used, is_in_dpb," )
	ASCII(.ascii	" MVC_IsOutDPB) ========\012\000" )
.LC485:
	ASCII(.ascii	"ReadImgNum = %d, NewImgNum = %d\012\000" )
	.space	3
.LC486:
	ASCII(.ascii	"%02d: %d %d %d\012\000" )
.LC487:
	ASCII(.ascii	"FrameStore leak, MVC_ClearAll\012\000" )
	.bss
	.align	2
.LANCHOR2 = . + 0
.LANCHOR3 = . + 8184
	.type	pps_tmp.14926, %object
	.size	pps_tmp.14926, 2240
pps_tmp.14926:
	.space	2240
	.type	sps_tmp.15062, %object
	.size	sps_tmp.15062, 3992
sps_tmp.15062:
	.space	3992
	.type	MvcTmpBuf, %object
	.size	MvcTmpBuf, 68
MvcTmpBuf:
	.space	68
	.type	cnt.13878, %object
	.size	cnt.13878, 4
cnt.13878:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
