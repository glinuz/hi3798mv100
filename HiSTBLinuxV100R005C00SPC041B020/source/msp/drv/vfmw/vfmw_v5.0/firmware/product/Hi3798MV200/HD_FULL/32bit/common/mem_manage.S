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
	.file	"mem_manage.c"
	.text
	.align	2
	.global	MEM_InitMemManager
	.type	MEM_InitMemManager, %function
MEM_InitMemManager:
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
	ldr	r4, .L2
	mov	r5, r0
	mov	r0, #1
	ldr	r3, [r4, #12]
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #12288
	mov	r1, #0
	ldr	r0, .L2+4
	blx	r3
	ldr	r3, .L2+8
	ldr	lr, [r5]
	mov	r1, #0
	ldr	ip, [r5, #4]
	mov	r0, #1
	ldr	r2, [r4, #16]
	str	lr, [r3, #-4080]
	str	ip, [r3, #-4076]
	str	r1, [r3, #-4072]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r2
.L3:
	.align	2
.L2:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	MEM_InitMemManager, .-MEM_InitMemManager
	.align	2
	.global	MEM_ManagerWithOperation
	.type	MEM_ManagerWithOperation, %function
MEM_ManagerWithOperation:
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
	ldr	r3, .L5
	ldr	r1, [r0, #52]
	ldr	r2, [r0, #56]
	str	r1, [r3, #-4068]
	str	r2, [r3, #-4064]
	ldmfd	sp, {fp, sp, pc}
.L6:
	.align	2
.L5:
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	MEM_ManagerWithOperation, .-MEM_ManagerWithOperation
	.align	2
	.global	MEM_ManagerClearOperation
	.type	MEM_ManagerClearOperation, %function
MEM_ManagerClearOperation:
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
	ldr	r3, .L8
	mov	r2, #0
	str	r2, [r3, #-4068]
	str	r2, [r3, #-4064]
	ldmfd	sp, {fp, sp, pc}
.L9:
	.align	2
.L8:
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	MEM_ManagerClearOperation, .-MEM_ManagerClearOperation
	.align	2
	.global	MEM_AddMemRecord
	.type	MEM_AddMemRecord, %function
MEM_AddMemRecord:
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
	cmp	r2, #0
	cmpne	r1, #0
	mov	r6, r1
	mov	r5, r2
	mov	r7, r0
	moveq	r3, #1
	movne	r3, #0
	cmp	r0, #0
	orreq	r3, r3, #1
	cmp	r3, #0
	beq	.L26
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L26:
	ldr	r8, .L29
	mov	r0, #1
	ldr	r4, .L29+4
	ldr	r3, [r8, #12]
	blx	r3
	ldr	r3, [r4, #16]
	cmp	r3, #0
	beq	.L12
	mov	ip, r4
	mov	r3, #1
	b	.L13
.L28:
	add	r3, r3, #1
	cmp	r3, #512
	beq	.L27
.L13:
	ldr	lr, [ip, #40]
	add	ip, ip, #24
	cmp	lr, #0
	bne	.L28
.L12:
	mov	r2, r3, asl #5
	ldr	r1, [r8, #16]
	sub	r3, r2, r3, asl #3
	mov	r0, #1
	add	r4, r4, r3
	stmib	r4, {r6, r7}
	str	r5, [r4, #16]
	blx	r1
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L27:
	ldr	r3, [r8, #16]
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L30:
	.align	2
.L29:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MEM_AddMemRecord, .-MEM_AddMemRecord
	.align	2
	.global	MEM_DelMemRecord
	.type	MEM_DelMemRecord, %function
MEM_DelMemRecord:
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
	cmp	r2, #0
	cmpne	r1, #0
	mov	r6, r1
	mov	r7, r2
	mov	r5, r0
	moveq	ip, #1
	movne	ip, #0
	cmp	r0, #0
	movne	r4, ip
	orreq	r4, ip, #1
	cmp	r4, #0
	beq	.L43
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L43:
	ldr	r8, .L45
	mov	r0, #1
	ldr	r3, [r8, #12]
	blx	r3
	ldr	r2, .L45+4
	mov	ip, r4
	mov	r3, r2
	b	.L34
.L33:
	add	ip, ip, #1
	add	r3, r3, #24
	cmp	ip, #512
	beq	.L44
.L34:
	ldr	lr, [r3, #16]
	cmp	lr, #0
	beq	.L33
	ldr	r4, [r3, #8]
	cmp	r4, r5
	bne	.L33
	ldr	r0, [r3, #4]
	cmp	r6, r0
	cmpeq	r7, lr
	bne	.L33
	mov	r3, ip, asl #5
	mov	r4, #0
	sub	ip, r3, ip, asl #3
	ldr	r1, [r8, #16]
	add	r2, r2, ip
	mov	r0, #1
	str	r4, [r2, #16]
	str	r4, [r2, #8]
	str	r4, [r2, #4]
	blx	r1
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L44:
	ldr	r3, [r8, #16]
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L46:
	.align	2
.L45:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MEM_DelMemRecord, .-MEM_DelMemRecord
	.align	2
	.global	MEM_Phy2Vir
	.type	MEM_Phy2Vir, %function
MEM_Phy2Vir:
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
	ldr	lr, .L56
	mov	r1, #0
	mov	r3, lr
	b	.L50
.L48:
	add	r1, r1, #1
	add	r3, r3, #24
	cmp	r1, #512
	beq	.L55
.L50:
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq	.L48
	ldr	ip, [r3, #8]
	cmp	ip, r0
	add	r2, ip, r2
	bhi	.L48
	cmp	r0, r2
	bcs	.L48
	mov	r3, r1, asl #5
	rsb	r0, ip, r0
	sub	r1, r3, r1, asl #3
	add	lr, lr, r1
	ldr	r3, [lr, #4]
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L55:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L57:
	.align	2
.L56:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MEM_Phy2Vir, .-MEM_Phy2Vir
	.align	2
	.global	MEM_Vir2Phy
	.type	MEM_Vir2Phy, %function
MEM_Vir2Phy:
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
	ldr	lr, .L67
	mov	r1, #0
	mov	r3, lr
	b	.L61
.L59:
	add	r1, r1, #1
	add	r3, r3, #24
	cmp	r1, #512
	beq	.L66
.L61:
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq	.L59
	ldr	ip, [r3, #4]
	cmp	ip, r0
	add	r2, ip, r2
	bhi	.L59
	cmp	r0, r2
	bcs	.L59
	mov	r3, r1, asl #5
	rsb	r0, ip, r0
	sub	r1, r3, r1, asl #3
	add	lr, lr, r1
	ldr	r3, [lr, #8]
	add	r0, r0, r3
	ldmfd	sp, {fp, sp, pc}
.L66:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L68:
	.align	2
.L67:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MEM_Vir2Phy, .-MEM_Vir2Phy
	.align	2
	.global	MEM_WritePhyWord
	.type	MEM_WritePhyWord, %function
MEM_WritePhyWord:
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
	mov	r4, r1
	bl	MEM_Phy2Vir
	cmp	r0, #0
	strne	r4, [r0]
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	MEM_WritePhyWord, .-MEM_WritePhyWord
	.align	2
	.global	MEM_ReadPhyWord
	.type	MEM_ReadPhyWord, %function
MEM_ReadPhyWord:
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
	bl	MEM_Phy2Vir
	cmp	r0, #0
	ldrne	r0, [r0]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	MEM_ReadPhyWord, .-MEM_ReadPhyWord
	.align	2
	.global	MEM_CopyPhy2Phy
	.type	MEM_CopyPhy2Phy, %function
MEM_CopyPhy2Phy:
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
	mov	r5, r1
	mov	r6, r2
	bl	MEM_Phy2Vir
	mov	r4, r0
	mov	r0, r5
	bl	MEM_Phy2Vir
	cmp	r0, #0
	cmpne	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, .L79
	mov	r1, r0
	mov	r2, r6
	mov	r0, r4
	ldr	r3, [r3, #52]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L80:
	.align	2
.L79:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MEM_CopyPhy2Phy, .-MEM_CopyPhy2Phy
	.align	2
	.global	MEM_CopyPhy2Vir
	.type	MEM_CopyPhy2Vir, %function
MEM_CopyPhy2Vir:
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
	mov	r0, r1
	mov	r5, r2
	bl	MEM_Phy2Vir
	cmp	r0, #0
	cmpne	r4, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L83
	mov	r1, r0
	mov	r2, r5
	mov	r0, r4
	ldr	r3, [r3, #52]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L84:
	.align	2
.L83:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MEM_CopyPhy2Vir, .-MEM_CopyPhy2Vir
	.align	2
	.global	MEM_CopyVir2Phy
	.type	MEM_CopyVir2Phy, %function
MEM_CopyVir2Phy:
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
	mov	r4, r1
	mov	r5, r2
	bl	MEM_Phy2Vir
	cmp	r0, #0
	cmpne	r4, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L87
	mov	r2, r5
	mov	r1, r4
	ldr	r3, [r3, #52]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L88:
	.align	2
.L87:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MEM_CopyVir2Phy, .-MEM_CopyVir2Phy
	.align	2
	.global	MEM_AllocMemBlock
	.type	MEM_AllocMemBlock, %function
MEM_AllocMemBlock:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 24
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
	cmp	r2, #0
	cmpne	r3, #0
	mov	r8, r0
	mov	r4, r3
	moveq	r6, #1
	movne	r6, #0
	mov	r9, r2
	beq	.L98
	ldr	r5, .L102
	sub	r7, fp, #68
	ldr	r10, [r3, #20]
	mov	r2, #24
	mov	r1, r6
	mov	r0, r3
	ldr	r3, [r5, #48]
	blx	r3
	mov	r1, r6
	ldr	r3, [r5, #48]
	mov	r2, #24
	mov	r0, r7
	blx	r3
	mov	r1, r9
	ldr	r3, [fp, #4]
	mov	r2, #4
	str	r10, [fp, #-48]
	mov	r0, r8
	str	r7, [sp]
	ldr	ip, [r5, #140]
	blx	ip
	ldrd	r2, [fp, #-60]
	orrs	r1, r2, r3
	beq	.L98
	ldr	r1, [fp, #-48]
	ldr	r0, [fp, #-68]
	ldr	ip, [fp, #-64]
	ldr	lr, [fp, #-52]
	cmp	r1, #3
	ldrls	pc, [pc, r1, asl #2]
	b	.L98
.L94:
	.word	.L93
	.word	.L95
	.word	.L95
	.word	.L96
.L96:
	cmp	ip, #0
	cmpne	r2, #0
	beq	.L98
.L97:
	ldr	r1, .L102+4
	mov	r0, #0
	stmib	r4, {r2, ip}
	str	lr, [r4, #16]
	ldr	r2, [r1, #-4072]
	add	r3, r2, #3
	add	r3, r3, lr
	bic	r3, r3, #3
	str	r3, [r1, #-4072]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L95:
	cmp	r0, #0
	cmpne	ip, #0
	moveq	r0, #1
	movne	r0, #0
	cmp	r2, #0
	orreq	r0, r0, #1
	cmp	r0, #0
	beq	.L97
.L98:
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L93:
	cmp	r0, #0
	cmpne	r2, #0
	beq	.L98
	mov	ip, r0
	b	.L97
.L103:
	.align	2
.L102:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1
	UNWIND(.fnend)
	.size	MEM_AllocMemBlock, .-MEM_AllocMemBlock
	.align	2
	.global	MEM_MapRegisterAddr
	.type	MEM_MapRegisterAddr, %function
MEM_MapRegisterAddr:
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
	cmp	r0, #0
	cmpne	r2, #0
	mov	r8, r1
	mov	r5, r2
	mov	r6, r0
	moveq	r4, #1
	movne	r4, #0
	beq	.L107
	ldr	r7, .L111
	mov	r2, #24
	mov	r1, r4
	mov	r0, r5
	ldr	r3, [r7, #48]
	blx	r3
	ldr	r3, [r7, #148]
	mov	r0, r6
	blx	r3
	cmp	r0, #0
	beq	.L107
	stmib	r5, {r0, r6}
	str	r8, [r5, #16]
.L106:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L107:
	mvn	r4, #0
	b	.L106
.L112:
	.align	2
.L111:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MEM_MapRegisterAddr, .-MEM_MapRegisterAddr
	.align	2
	.global	MEM_UnmapRegisterAddr
	.type	MEM_UnmapRegisterAddr, %function
MEM_UnmapRegisterAddr:
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
	cmpne	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L115
	mov	r0, r1
	ldr	r3, [r3, #152]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L116:
	.align	2
.L115:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MEM_UnmapRegisterAddr, .-MEM_UnmapRegisterAddr
	.align	2
	.global	MEM_ReleaseMemBlock
	.type	MEM_ReleaseMemBlock, %function
MEM_ReleaseMemBlock:
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
	cmpne	r0, #0
	mov	r4, r0
	mov	r6, r1
	moveq	r5, #1
	movne	r5, #0
	bne	.L126
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L126:
	ldr	r7, .L127
	mov	r2, #24
	mov	r1, r5
	sub	r0, fp, #52
	ldr	r3, [r7, #48]
	blx	r3
	ldr	r3, .L127+4
	str	r4, [fp, #-52]
	add	ip, r3, #12288
	str	r6, [fp, #-44]
	str	r5, [fp, #-40]
	b	.L121
.L119:
	add	r3, r3, #24
	cmp	r3, ip
	beq	.L120
.L121:
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq	.L119
	ldr	r1, [r3, #8]
	cmp	r4, r1
	add	r0, r1, r2
	bcc	.L119
	cmp	r4, r0
	bcs	.L119
	str	r2, [fp, #-36]
.L120:
	ldr	r3, [r7, #144]
	sub	r0, fp, #52
	blx	r3
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L128:
	.align	2
.L127:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MEM_ReleaseMemBlock, .-MEM_ReleaseMemBlock
	.align	2
	.global	MEM_NsMapToSec
	.type	MEM_NsMapToSec, %function
MEM_NsMapToSec:
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
	.size	MEM_NsMapToSec, .-MEM_NsMapToSec
	.align	2
	.global	MEM_NsInSecUnMap
	.type	MEM_NsInSecUnMap, %function
MEM_NsInSecUnMap:
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
	.size	MEM_NsInSecUnMap, .-MEM_NsInSecUnMap
	.global	mem_free
	.global	mem_malloc
	.global	s_MemRecord
	.bss
	.align	2
.LANCHOR0 = . + 0
.LANCHOR1 = . + 16368
	.type	s_MemRecord, %object
	.size	s_MemRecord, 12288
s_MemRecord:
	.space	12288
	.type	s_MemBaseAddr, %object
	.size	s_MemBaseAddr, 4
s_MemBaseAddr:
	.space	4
	.type	s_MemSize, %object
	.size	s_MemSize, 4
s_MemSize:
	.space	4
	.type	s_MemOffset, %object
	.size	s_MemOffset, 4
s_MemOffset:
	.space	4
	.type	mem_malloc, %object
	.size	mem_malloc, 4
mem_malloc:
	.space	4
	.type	mem_free, %object
	.size	mem_free, 4
mem_free:
	.space	4
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
