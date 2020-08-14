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
	mov	r2, #8192
	mov	r1, #0
	ldr	r0, .L2+4
	blx	r3
	ldr	r3, .L2+8
	ldr	lr, [r5]
	mov	r1, #0
	ldr	ip, [r5, #4]
	mov	r0, #1
	ldr	r2, [r4, #16]
	str	lr, [r3, #8]
	str	ip, [r3, #12]
	str	r1, [r3, #16]
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
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	str	r1, [r3, #20]
	str	r2, [r3, #24]
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
	str	r2, [r3, #20]
	str	r2, [r3, #24]
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
	ldr	r8, .L24
	mov	r6, r0
	ldr	r4, .L24+4
	mov	r0, #1
	mov	r7, r1
	mov	r5, r2
	ldr	r3, [r8, #12]
	blx	r3
	ldr	r3, [r4, #12]
	cmp	r3, #0
	beq	.L11
	mov	ip, r4
	mov	r3, #1
	b	.L12
.L23:
	add	r3, r3, #1
	cmp	r3, #512
	beq	.L22
.L12:
	ldr	lr, [ip, #28]
	add	ip, ip, #16
	cmp	lr, #0
	bne	.L23
.L11:
	add	r3, r4, r3, lsl #4
	ldr	r2, [r8, #16]
	mov	r0, #1
	str	r6, [r3, #8]
	str	r7, [r3, #4]
	str	r5, [r3, #12]
	blx	r2
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L22:
	ldr	r3, [r8, #16]
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L25:
	.align	2
.L24:
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
	ldr	r8, .L38
	mov	r5, r0
	mov	r0, #1
	mov	r6, r2
	mov	r7, r1
	ldr	r3, [r8, #12]
	blx	r3
	ldr	r2, .L38+4
	mov	ip, #0
	mov	r3, r2
	b	.L29
.L27:
	add	ip, ip, #1
	add	r3, r3, #16
	cmp	ip, #512
	beq	.L37
.L29:
	ldr	lr, [r3, #12]
	cmp	lr, #0
	beq	.L27
	ldr	r4, [r3, #8]
	cmp	r4, r5
	bne	.L27
	ldr	r0, [r3, #4]
	cmp	r0, r7
	cmpeq	lr, r6
	bne	.L27
	add	r2, r2, ip, lsl #4
	mov	r4, #0
	ldr	r3, [r8, #16]
	mov	r0, #1
	str	r4, [r2, #12]
	str	r4, [r2, #8]
	str	r4, [r2, #4]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L37:
	ldr	r3, [r8, #16]
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L39:
	.align	2
.L38:
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
	ldr	lr, .L49
	mov	r1, #0
	mov	r3, lr
	b	.L43
.L41:
	add	r1, r1, #1
	add	r3, r3, #16
	cmp	r1, #512
	beq	.L48
.L43:
	ldr	r2, [r3, #12]
	cmp	r2, #0
	beq	.L41
	ldr	ip, [r3, #8]
	cmp	ip, r0
	add	r2, ip, r2
	bhi	.L41
	cmp	r0, r2
	bcs	.L41
	add	r1, lr, r1, lsl #4
	rsb	r0, ip, r0
	ldr	r3, [r1, #4]
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L48:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L50:
	.align	2
.L49:
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
	ldr	lr, .L60
	mov	r1, #0
	mov	r3, lr
	b	.L54
.L52:
	add	r1, r1, #1
	add	r3, r3, #16
	cmp	r1, #512
	beq	.L59
.L54:
	ldr	r2, [r3, #12]
	cmp	r2, #0
	beq	.L52
	ldr	ip, [r3, #4]
	cmp	ip, r0
	add	r2, ip, r2
	bhi	.L52
	cmp	r0, r2
	bcs	.L52
	add	r1, lr, r1, lsl #4
	rsb	r0, ip, r0
	ldr	r3, [r1, #8]
	add	r0, r0, r3
	ldmfd	sp, {fp, sp, pc}
.L59:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L61:
	.align	2
.L60:
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
	ldr	r3, .L72
	mov	r1, r0
	mov	r2, r6
	mov	r0, r4
	ldr	r3, [r3, #52]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3
.L73:
	.align	2
.L72:
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
	ldr	r3, .L76
	mov	r1, r0
	mov	r2, r5
	mov	r0, r4
	ldr	r3, [r3, #52]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L77:
	.align	2
.L76:
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
	ldr	r3, .L80
	mov	r2, r5
	mov	r1, r4
	ldr	r3, [r3, #52]
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L81:
	.align	2
.L80:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MEM_CopyVir2Phy, .-MEM_CopyVir2Phy
	.align	2
	.global	MEM_AllocMemBlock
	.type	MEM_AllocMemBlock, %function
MEM_AllocMemBlock:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	cmp	r2, #0
	cmpne	r3, #0
	mov	r8, r0
	mov	r5, r3
	moveq	r4, #1
	movne	r4, #0
	mov	r9, r2
	beq	.L85
	ldr	r6, .L92
	sub	r7, fp, #52
	mov	r2, #16
	mov	r1, r4
	mov	r0, r3
	ldr	r3, [r6, #48]
	blx	r3
	ldr	r3, [r6, #48]
	mov	r2, #16
	mov	r1, r4
	mov	r0, r7
	blx	r3
	mov	r2, #4
	str	r7, [sp]
	mov	r1, r9
	ldr	ip, [r6, #144]
	mov	r0, r8
	ldr	r3, [fp, #4]
	blx	ip
	ldr	r2, [fp, #-48]
	cmp	r2, #0
	beq	.L85
	ldr	r1, [fp, #-52]
	ldr	ip, [fp, #-44]
	cmp	r1, #0
	beq	.L85
	ldr	lr, .L92+4
	mov	r0, r4
	str	r2, [r5, #4]
	str	r1, [r5, #8]
	ldr	r2, [lr, #16]
	str	ip, [r5, #12]
	add	r3, r2, #3
	add	r3, r3, ip
	bic	r3, r3, #3
	str	r3, [lr, #16]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L85:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L93:
	.align	2
.L92:
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
	beq	.L97
	ldr	r7, .L101
	mov	r2, #16
	mov	r1, r4
	mov	r0, r5
	ldr	r3, [r7, #48]
	blx	r3
	ldr	r3, [r7, #152]
	mov	r0, r6
	blx	r3
	cmp	r0, #0
	beq	.L97
	stmib	r5, {r0, r6, r8}
.L96:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L97:
	mvn	r4, #0
	b	.L96
.L102:
	.align	2
.L101:
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
	ldr	r3, .L105
	mov	r0, r1
	ldr	r3, [r3, #156]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L106:
	.align	2
.L105:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	MEM_UnmapRegisterAddr, .-MEM_UnmapRegisterAddr
	.align	2
	.global	MEM_ReleaseMemBlock
	.type	MEM_ReleaseMemBlock, %function
MEM_ReleaseMemBlock:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #20)
	sub	sp, sp, #20
	cmp	r1, #0
	cmpne	r0, #0
	mov	r4, r0
	mov	r5, r1
	moveq	r3, #1
	movne	r3, #0
	bne	.L116
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L116:
	ldr	r6, .L117
	mov	r1, r3
	mov	r2, #16
	sub	r0, fp, #44
	ldr	r3, [r6, #48]
	blx	r3
	ldr	r3, .L117+4
	str	r4, [fp, #-44]
	add	ip, r3, #8192
	str	r5, [fp, #-40]
	b	.L111
.L109:
	add	r3, r3, #16
	cmp	r3, ip
	beq	.L110
.L111:
	ldr	r2, [r3, #12]
	cmp	r2, #0
	beq	.L109
	ldr	r1, [r3, #8]
	cmp	r4, r1
	add	r0, r1, r2
	bcc	.L109
	cmp	r4, r0
	bcs	.L109
	str	r2, [fp, #-36]
.L110:
	ldr	r3, [r6, #148]
	sub	r0, fp, #44
	blx	r3
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L118:
	.align	2
.L117:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	MEM_ReleaseMemBlock, .-MEM_ReleaseMemBlock
	.global	mem_free
	.global	mem_malloc
	.global	s_MemRecord
	.bss
	.align	2
.LANCHOR0 = . + 0
.LANCHOR1 = . + 8184
	.type	s_MemRecord, %object
	.size	s_MemRecord, 8192
s_MemRecord:
	.space	8192
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
