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
	.file	"vdm_drv.c"
	.text
	.align	2
	.global	VDMDRV_Init
	.type	VDMDRV_Init, %function
VDMDRV_Init:
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
	ldr	r3, .L2
	mov	r5, r0
	ldr	r4, .L2+4
	mov	r0, r0, asl #6
	sub	r0, r0, r5, asl #3
	mov	r2, #56
	add	r0, r4, r0
	ldr	r3, [r3, #48]
	mov	r1, #0
	add	r4, r4, r5, lsl #2
	blx	r3
	mov	r2, r5, asl #7
	ldr	r1, .L2+8
	sub	r2, r2, r5, asl #5
	mov	r3, #0
	str	r3, [r4, #56]
	add	r2, r1, r2
	str	r3, [r4, #60]
	str	r3, [r2, #16]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L3:
	.align	2
.L2:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	g_VfmwGlobalStat
	UNWIND(.fnend)
	.size	VDMDRV_Init, .-VDMDRV_Init
	.align	2
	.global	VDMDRV_Reset
	.type	VDMDRV_Reset, %function
VDMDRV_Reset:
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
	ldr	r1, .L5
	mov	r3, r0, asl #6
	ldr	ip, .L5+4
	sub	r0, r3, r0, asl #3
	mov	r2, #56
	ldr	r3, [r1, #48]
	add	r0, ip, r0
	mov	r1, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L6:
	.align	2
.L5:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_Reset, .-VDMDRV_Reset
	.align	2
	.global	VDMDRV_SetPriority
	.type	VDMDRV_SetPriority, %function
VDMDRV_SetPriority:
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
	ldr	r5, .L20
	mov	r4, #0
	sub	r7, r0, #4
	mov	lr, r4
	mov	r6, r4
.L14:
	mov	ip, #0
	mov	r1, r7
	mov	r3, ip
.L9:
	ldr	r2, [r1, #4]!
	cmp	r2, ip
	movgt	lr, r3
	add	r3, r3, #1
	movgt	ip, r2
	cmp	r3, #16
	bne	.L9
	cmp	ip, #0
	ble	.L19
	add	r4, r4, #1
	str	lr, [r5, #4]!
	cmp	r4, #16
	str	r6, [r0, lr, asl #2]
	bne	.L14
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L19:
	cmp	r4, #15
	ldmgtfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r0, .L20+4
	mov	r2, r4, asl #2
	mvn	r1, #0
.L12:
	add	r4, r4, #1
	add	r3, r0, r2
	cmp	r4, #16
	add	r2, r2, #4
	str	r1, [r3, #132]
	bne	.L12
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L21:
	.align	2
.L20:
	.word	.LANCHOR0+128
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_SetPriority, .-VDMDRV_SetPriority
	.align	2
	.global	VDMDRV_AdjustPriorityAfterDec
	.type	VDMDRV_AdjustPriorityAfterDec, %function
VDMDRV_AdjustPriorityAfterDec:
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
	ldr	r5, .L35
	add	ip, r5, #132
	ldr	r3, [r5, #132]
	cmp	r3, r0
	beq	.L31
	mov	r3, #1
	mov	r2, ip
.L27:
	ldr	r1, [r2, #4]!
	cmp	r1, r0
	beq	.L33
	add	r3, r3, #1
	cmp	r3, #16
	bne	.L27
	mov	r3, #0
	mov	r6, #4
	mov	lr, r3
.L28:
	ldr	r0, [ip, r6]
	add	r6, ip, r6
	cmp	r0, #0
	ldmltfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r2, r3, #2
	mov	r2, r2, asl #2
	b	.L26
.L30:
	ldr	r0, [ip, r2]
	add	r2, r2, #4
	cmp	r0, #0
	ldmltfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L26:
	ldr	r4, [ip, lr, asl #2]
	add	r1, r5, r0, lsl #2
	add	r3, r3, #1
	ldr	r7, [r1, #68]
	add	r1, r5, r4, lsl #2
	ldr	r1, [r1, #68]
	cmp	r7, r1
	strge	r0, [ip, lr, asl #2]
	strge	r4, [r6]
	cmp	r3, #14
	add	r6, ip, r2
	mov	lr, r3
	ble	.L30
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L33:
	cmp	r3, #14
	bgt	.L34
.L23:
	add	r6, r3, #1
	mov	lr, r3
	mov	r6, r6, asl #2
	b	.L28
.L31:
	mov	r3, #0
	b	.L23
.L34:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L36:
	.align	2
.L35:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_AdjustPriorityAfterDec, .-VDMDRV_AdjustPriorityAfterDec
	.align	2
	.global	VDMDRV_PrepareSleep
	.type	VDMDRV_PrepareSleep, %function
VDMDRV_PrepareSleep:
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
	ldr	r1, .L41
	add	ip, r1, r0, lsl #2
	ldr	r3, [ip, #56]
	cmp	r3, #0
	bne	.L40
	mov	r2, r0, asl #6
	sub	r0, r2, r0, asl #3
	ldr	r0, [r1, r0]
	cmp	r0, #0
	moveq	r3, #2
	movne	r0, r3
	movne	r3, #1
	str	r3, [ip, #56]
	ldmfd	sp, {fp, sp, pc}
.L40:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L42:
	.align	2
.L41:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_PrepareSleep, .-VDMDRV_PrepareSleep
	.align	2
	.global	VDMDRV_GetSleepStage
	.type	VDMDRV_GetSleepStage, %function
VDMDRV_GetSleepStage:
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
	ldr	r3, .L44
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #56]
	ldmfd	sp, {fp, sp, pc}
.L45:
	.align	2
.L44:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_GetSleepStage, .-VDMDRV_GetSleepStage
	.align	2
	.global	VDMDRV_ForceSleep
	.type	VDMDRV_ForceSleep, %function
VDMDRV_ForceSleep:
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
	ldr	r3, .L51
	add	r4, r3, r0, lsl #2
	ldr	r2, [r4, #56]
	cmp	r2, #2
	beq	.L47
	ldr	ip, .L51+4
	mov	r2, r0, asl #6
	sub	r0, r2, r0, asl #3
	mov	r1, #0
	add	r0, r3, r0
	mov	r2, #56
	ldr	r3, [ip, #48]
	blx	r3
	mov	r3, #2
	str	r3, [r4, #56]
.L47:
	ldr	r1, .L51+8
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	dprint_vfmw
.L52:
	.align	2
.L51:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	UNWIND(.fnend)
	.size	VDMDRV_ForceSleep, .-VDMDRV_ForceSleep
	.align	2
	.global	VDMDRV_ExitSleep
	.type	VDMDRV_ExitSleep, %function
VDMDRV_ExitSleep:
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
	ldr	r3, .L54
	mov	r2, #0
	add	r0, r3, r0, lsl #2
	str	r2, [r0, #56]
	ldmfd	sp, {fp, sp, pc}
.L55:
	.align	2
.L54:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_ExitSleep, .-VDMDRV_ExitSleep
	.align	2
	.global	VDMDRV_StartRepair
	.type	VDMDRV_StartRepair, %function
VDMDRV_StartRepair:
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
	ldr	r3, .L59
	mov	r5, r0
	mov	r4, r1
	ldr	r3, [r3, #120]
	cmp	r3, #0
	beq	.L57
	mov	r1, r2
	mov	r0, r4
	blx	r3
.L58:
	mov	r3, r4, asl #6
	ldr	r2, .L59+4
	sub	r1, r3, r4, asl #3
	str	r5, [r2, r1]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L57:
	mov	r0, r3
	movw	r2, #295
	ldr	r1, .L59+8
	bl	dprint_vfmw
	b	.L58
.L60:
	.align	2
.L59:
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMDRV_StartRepair, .-VDMDRV_StartRepair
	.align	2
	.global	VDMDRV_StartDec
	.type	VDMDRV_StartDec, %function
VDMDRV_StartDec:
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
	ldr	r3, .L64
	mov	r4, r0
	ldr	r3, [r3, #124]
	cmp	r3, #0
	beq	.L62
	blx	r3
.L63:
	mov	r3, r4, asl #6
	ldr	r2, .L64+4
	sub	r0, r3, r4, asl #3
	mov	r3, #1
	str	r3, [r2, r0]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L62:
	mov	r0, r3
	movw	r2, #303
	ldr	r1, .L64+8
	bl	dprint_vfmw
	b	.L63
.L65:
	.align	2
.L64:
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMDRV_StartDec, .-VDMDRV_StartDec
	.align	2
	.global	VDMDRV_IsChanDec
	.type	VDMDRV_IsChanDec, %function
VDMDRV_IsChanDec:
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
	ldr	r3, .L67
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #196]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L68:
	.align	2
.L67:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_IsChanDec, .-VDMDRV_IsChanDec
	.align	2
	.global	VDMDRV_SetChanIsDec
	.type	VDMDRV_SetChanIsDec, %function
VDMDRV_SetChanIsDec:
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
	ldr	r3, .L70
	add	r1, r1, #1
	add	r3, r3, r0, lsl #2
	str	r1, [r3, #196]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDMDRV_AdjustPriorityAfterDec
.L71:
	.align	2
.L70:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_SetChanIsDec, .-VDMDRV_SetChanIsDec
	.align	2
	.global	VDMDRV_ClearChanIsDec
	.type	VDMDRV_ClearChanIsDec, %function
VDMDRV_ClearChanIsDec:
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
	ldr	r3, .L73
	add	r0, r0, #32
	mov	r2, #0
	add	r0, r3, r0, lsl #2
	str	r2, [r0, #68]
	ldmfd	sp, {fp, sp, pc}
.L74:
	.align	2
.L73:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_ClearChanIsDec, .-VDMDRV_ClearChanIsDec
	.align	2
	.global	VDMDRV_ParamInit
	.type	VDMDRV_ParamInit, %function
VDMDRV_ParamInit:
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
	ldr	r3, .L82
	mov	r0, #1
	add	r1, r3, #64
.L77:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	strne	r0, [r2, #24]
	cmp	r3, r1
	bne	.L77
	ldmfd	sp, {fp, sp, pc}
.L83:
	.align	2
.L82:
	.word	s_pstVfmwChan-4
	UNWIND(.fnend)
	.size	VDMDRV_ParamInit, .-VDMDRV_ParamInit
	.align	2
	.global	VDMDRV_AddMatchMode
	.type	VDMDRV_AddMatchMode, %function
VDMDRV_AddMatchMode:
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
	.size	VDMDRV_AddMatchMode, .-VDMDRV_AddMatchMode
	.align	2
	.global	VDMDRV_CheckAttachMode
	.type	VDMDRV_CheckAttachMode, %function
VDMDRV_CheckAttachMode:
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
	cmp	r0, #15
	mov	r4, r0
	bgt	.L85
	ldr	r6, .L111
	ldr	r2, [r6, r0, asl #2]
	cmp	r2, #0
	beq	.L85
	add	r3, r2, #262144
	ldr	r5, .L111+4
	mov	r8, r0, asl #2
	ldr	r1, [r3, #1992]
	ldr	r3, [r3, #1988]
	cmp	r1, #2160
	cmpcs	r3, #3840
	movcs	r7, #1
	movcc	r7, #0
	bcs	.L110
	add	r3, r5, r8
	ldr	r3, [r3, #324]
.L93:
	cmp	r3, #10
	beq	.L95
.L94:
	ldr	r3, [r2, #24]
	cmp	r3, #0
	beq	.L96
.L95:
	ldr	r3, [r2, #1512]
	cmp	r3, #35
	ble	.L97
	cmp	r7, #0
	beq	.L96
	mov	r1, #1
	movw	r3, #558
	str	r1, [r2, #1500]
	mov	r0, #0
	str	r1, [sp, #4]
	ldr	r2, .L111+8
	str	r4, [sp]
	ldr	r1, .L111+12
	bl	dprint_vfmw
	ldr	r2, [r6, r4, asl #2]
.L96:
	mov	r3, #0
	str	r3, [r2, #24]
	ldr	r3, [r6, r4, asl #2]
	add	r5, r5, r8
	ldr	r3, [r3, #1512]
	str	r3, [r5, #260]
.L85:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L110:
	add	r3, r5, r8
	ldr	r2, [r2, #1512]
	mov	r0, #22
	ldr	r1, .L111+16
	ldr	r3, [r3, #260]
	str	r2, [sp]
	cmp	r2, r3
	rsbhi	r9, r3, r2
	rsbls	r9, r2, r3
	str	r3, [sp, #4]
	movw	r3, #530
	ldr	r2, .L111+8
	bl	dprint_vfmw
	cmp	r9, #2
	bgt	.L90
	add	r2, r5, r8
	ldr	r3, [r2, #324]
	add	r3, r3, #1
	str	r3, [r2, #324]
.L91:
	cmn	r3, #1
	beq	.L92
	ldr	r2, [r6, r4, asl #2]
	b	.L93
.L97:
	mov	r3, #0
	str	r3, [r2, #1500]
	ldr	r2, [r6, r4, asl #2]
	b	.L96
.L90:
	add	r9, r5, r8
	movw	r3, #539
	ldr	r2, .L111+8
	mov	r0, #22
	ldr	r1, .L111+20
	mov	ip, #0
	str	ip, [r9, #324]
	bl	dprint_vfmw
	ldr	r3, [r9, #324]
	b	.L91
.L92:
	add	r3, r5, r8
	mov	r1, #11
	ldr	r2, [r6, r4, asl #2]
	str	r1, [r3, #324]
	b	.L94
.L112:
	.align	2
.L111:
	.word	s_pstVfmwChan
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LC4
	.word	.LC2
	.word	.LC3
	UNWIND(.fnend)
	.size	VDMDRV_CheckAttachMode, .-VDMDRV_CheckAttachMode
	.align	2
	.global	VDMDRV_ChnDecReady
	.type	VDMDRV_ChnDecReady, %function
VDMDRV_ChnDecReady:
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
	mov	r6, r1
	mov	r5, r0
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L117
	ldr	ip, .L120
	add	ip, ip, r5, lsl #2
	ldr	r4, [ip, #196]
	cmp	r4, #0
	bne	.L118
	str	r4, [r6]
	mov	r0, r5
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	str	r0, [r6]
	beq	.L119
.L115:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L117:
	mov	r3, r5
	ldr	r2, .L120+4
	ldr	r1, .L120+8
	mov	r4, #1
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L118:
	mov	r2, r5
	ldr	r1, .L120+12
	mov	r4, #1
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L119:
	mov	r2, r5
	ldr	r1, .L120+16
	mov	r0, #22
	mov	r4, #1
	bl	dprint_vfmw
	b	.L115
.L121:
	.align	2
.L120:
	.word	.LANCHOR0
	.word	.LANCHOR1+24
	.word	.LC5
	.word	.LC6
	.word	.LC7
	UNWIND(.fnend)
	.size	VDMDRV_ChnDecReady, .-VDMDRV_ChnDecReady
	.align	2
	.global	VDMDRV_MMUIntServeProc
	.type	VDMDRV_MMUIntServeProc, %function
VDMDRV_MMUIntServeProc:
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
	UNWIND(.pad #4)
	sub	sp, sp, #4
	ldr	r7, .L143
	ldr	r4, .L143+4
	mov	r5, r0
	mov	r0, #0
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r3, [r4, #108]
	cmp	r3, #0
	beq	.L128
	mov	r1, r5
	mov	r0, #5
	blx	r3
	ldr	r3, [r4, #108]
	cmp	r3, #0
	mov	r8, r0
	beq	.L128
	mov	r1, r5
	mov	r0, #6
	blx	r3
	ldr	ip, [r4, #108]
	and	r9, r8, #7
	cmp	ip, #0
	moveq	r6, ip
	mov	r10, r0
	beq	.L123
	mov	r1, r5
	mov	r0, #7
	blx	ip
	mov	r6, r0
.L123:
	cmp	r9, #0
	beq	.L128
	cmp	r10, #0
	bne	.L141
	cmp	r6, #0
	bne	.L142
.L128:
	ldr	r3, [r4, #96]
	cmp	r3, #0
	beq	.L129
	mov	r0, r5
	blx	r3
.L130:
	ldr	r3, [r7, #16]
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L129:
	mov	r0, r3
	movw	r2, #1725
	ldr	r1, .L143+8
	bl	dprint_vfmw
	b	.L130
.L142:
	mov	r3, r6
	mov	r2, r8
	ldr	r1, .L143+12
	mov	r0, #32
	bl	dprint_vfmw
	b	.L128
.L141:
	mov	r3, r10
	mov	r2, r8
	ldr	r1, .L143+16
	mov	r0, #32
	bl	dprint_vfmw
	cmp	r6, #0
	beq	.L128
	b	.L142
.L144:
	.align	2
.L143:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	.LC1
	.word	.LC9
	.word	.LC8
	UNWIND(.fnend)
	.size	VDMDRV_MMUIntServeProc, .-VDMDRV_MMUIntServeProc
	.align	2
	.global	VDMDRV_IsVdmInISR
	.type	VDMDRV_IsVdmInISR, %function
VDMDRV_IsVdmInISR:
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
	ldr	r3, .L146
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #388]
	ldmfd	sp, {fp, sp, pc}
.L147:
	.align	2
.L146:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_IsVdmInISR, .-VDMDRV_IsVdmInISR
	.align	2
	.global	VDMDRV_SetStateIdle
	.type	VDMDRV_SetStateIdle, %function
VDMDRV_SetStateIdle:
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
	ldr	r2, .L149
	mov	r3, r0, asl #6
	sub	r0, r3, r0, asl #3
	mov	r1, #0
	str	r1, [r2, r0]
	ldmfd	sp, {fp, sp, pc}
.L150:
	.align	2
.L149:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_SetStateIdle, .-VDMDRV_SetStateIdle
	.align	2
	.global	VDMDRV_IsVdmIdle
	.type	VDMDRV_IsVdmIdle, %function
VDMDRV_IsVdmIdle:
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
	ldr	r2, .L152
	mov	r3, r0, asl #6
	sub	r0, r3, r0, asl #3
	ldr	r0, [r2, r0]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L153:
	.align	2
.L152:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_IsVdmIdle, .-VDMDRV_IsVdmIdle
	.align	2
	.global	VDMDRV_GetCurrChanID
	.type	VDMDRV_GetCurrChanID, %function
VDMDRV_GetCurrChanID:
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
	ldr	r3, .L155
	mov	r2, r0, asl #6
	sub	r0, r2, r0, asl #3
	add	r0, r3, r0
	ldr	r0, [r0, #8]
	ldmfd	sp, {fp, sp, pc}
.L156:
	.align	2
.L155:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_GetCurrChanID, .-VDMDRV_GetCurrChanID
	.align	2
	.global	VDMDRV_WaitVdmReadyIfNoIsr
	.type	VDMDRV_WaitVdmReadyIfNoIsr, %function
VDMDRV_WaitVdmReadyIfNoIsr:
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
	ldr	r6, .L166
	mov	r5, r0
	mov	r4, #0
	b	.L159
.L161:
	add	r4, r4, #1
	cmp	r4, #400
	beq	.L160
.L159:
	ldr	r3, [r6, #132]
	mov	r0, r5
	cmp	r3, #0
	beq	.L161
	blx	r3
	cmp	r0, #0
	beq	.L161
.L160:
	cmp	r4, #400
	movcc	r0, #0
	movcs	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L167:
	.align	2
.L166:
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	VDMDRV_WaitVdmReadyIfNoIsr, .-VDMDRV_WaitVdmReadyIfNoIsr
	.align	2
	.global	VDMDRV_AvsFirstFldNeedRepair
	.type	VDMDRV_AvsFirstFldNeedRepair, %function
VDMDRV_AvsFirstFldNeedRepair:
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
	cmp	r0, #6
	beq	.L172
.L171:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L172:
	ldrb	r3, [r1, #17]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L171
	ldr	r3, .L173
	movw	r1, #1752
	mla	r2, r1, r2, r3
	ldr	r3, [r2, #1232]
	cmp	r3, #0
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L174:
	.align	2
.L173:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_AvsFirstFldNeedRepair, .-VDMDRV_AvsFirstFldNeedRepair
	.align	2
	.global	VDMDRV_AvsSecondFldNeedRepair
	.type	VDMDRV_AvsSecondFldNeedRepair, %function
VDMDRV_AvsSecondFldNeedRepair:
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
	cmp	r0, #6
	beq	.L179
.L178:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L179:
	ldrb	r3, [r1, #17]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L178
	ldr	r3, .L180
	movw	r1, #1752
	mla	r2, r1, r2, r3
	ldr	r3, [r2, #1716]
	cmp	r3, #0
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L181:
	.align	2
.L180:
	.word	.LANCHOR0+392
	UNWIND(.fnend)
	.size	VDMDRV_AvsSecondFldNeedRepair, .-VDMDRV_AvsSecondFldNeedRepair
	.align	2
	.global	VDMDRV_IsMpeg4NvopCopy
	.type	VDMDRV_IsMpeg4NvopCopy, %function
VDMDRV_IsMpeg4NvopCopy:
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
	subs	r4, r1, #0
	beq	.L186
	cmp	r0, #2
	ldreqb	r0, [r4, #158]	@ zero_extendqisi2
	subeq	r0, r0, #1
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L186:
	ldr	r1, .L187
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L188:
	.align	2
.L187:
	.word	.LC10
	UNWIND(.fnend)
	.size	VDMDRV_IsMpeg4NvopCopy, .-VDMDRV_IsMpeg4NvopCopy
	.align	2
	.global	VDMDRV_IsVc1SkpicCopy
	.type	VDMDRV_IsVc1SkpicCopy, %function
VDMDRV_IsVc1SkpicCopy:
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
	subs	r4, r1, #0
	beq	.L193
	cmp	r0, #1
	ldreqb	r0, [r4, #1]	@ zero_extendqisi2
	subeq	r0, r0, #1
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L193:
	ldr	r1, .L194
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L195:
	.align	2
.L194:
	.word	.LC11
	UNWIND(.fnend)
	.size	VDMDRV_IsVc1SkpicCopy, .-VDMDRV_IsVc1SkpicCopy
	.align	2
	.global	VDMDRV_IsVp6NvopCopy
	.type	VDMDRV_IsVp6NvopCopy, %function
VDMDRV_IsVp6NvopCopy:
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
	subs	r4, r1, #0
	beq	.L200
	sub	r0, r0, #10
	cmp	r0, #2
	ldrlsb	r0, [r4, #1]	@ zero_extendqisi2
	subls	r0, r0, #1
	clzls	r0, r0
	movls	r0, r0, lsr #5
	movhi	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L200:
	ldr	r1, .L201
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L202:
	.align	2
.L201:
	.word	.LC12
	UNWIND(.fnend)
	.size	VDMDRV_IsVp6NvopCopy, .-VDMDRV_IsVp6NvopCopy
	.align	2
	.global	VDMDRV_DoBeforeDec
	.type	VDMDRV_DoBeforeDec, %function
VDMDRV_DoBeforeDec:
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
	ldr	r3, .L252
	mov	r5, r0
	mov	r4, r1
	mov	r10, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r9, #0
	beq	.L204
	ldr	r3, .L252+4
	sub	r0, fp, #232
	mov	r2, #188
	mov	r1, #0
	mov	r9, r0
	ldr	r3, [r3, #48]
	blx	r3
.L204:
	ldr	r3, [r10]
	cmp	r3, #0
	beq	.L245
	mov	r0, r4
	mov	r6, r5, asl #3
	bl	VCTRL_GetVidStd
	ldr	r1, .L252+8
	mov	r8, r5, asl #6
	ldr	r7, .L252+12
	rsb	r2, r6, r8
	ldr	r1, [r1]
	add	r2, r7, r2
	add	r2, r2, #8
	cmp	r1, #0
	str	r0, [r2, #4]
	mov	r3, r0
	str	r2, [fp, #-248]
	beq	.L207
	ldr	ip, [r10]
	sub	r0, fp, #244
	str	r4, [fp, #-244]
	str	r3, [fp, #-240]
	str	ip, [fp, #-236]
	blx	r1
	cmp	r0, #1
	mov	ip, r0
	beq	.L236
	ldr	r2, [fp, #-248]
	ldr	r3, [r2, #4]
.L207:
	ldr	r1, [r10]
	cmp	r1, #0
	beq	.L246
	cmp	r3, #2
	beq	.L247
.L211:
	cmp	r3, #1
	beq	.L248
.L213:
	mov	r0, r3
	bl	VDMDRV_IsVp6NvopCopy
	cmp	r0, #1
	beq	.L249
	ldr	r3, .L252+16
	ldr	ip, [r3, #128]
	cmp	ip, #0
	beq	.L218
	rsb	r3, r6, r8
	ldr	r1, [r10]
	add	r10, r7, r3
	mov	r2, r5
	mov	r3, r9
	ldr	r0, [r10, #12]
	blx	ip
	subs	ip, r0, #0
	beq	.L250
.L218:
	rsb	r6, r6, r8
	ldr	r1, .L252+20
	add	r10, r7, r6
	mov	r0, #0
	bl	dprint_vfmw
	mov	r9, #0
	mov	lr, #100
	str	r5, [sp]
	mov	r1, lr
	mov	r0, r4
	mov	r3, r9
	mov	r2, #15
	str	r9, [r7, r6]
	add	r4, r4, #32
	str	lr, [r10, #4]
	str	r9, [fp, #-248]
	bl	VCTRL_VdmPostProc
	ldr	r3, .L252+4
	mov	r0, r10
	mov	r1, r9
	mov	r2, #56
	add	r7, r7, r4, lsl #2
	ldr	r3, [r3, #48]
	blx	r3
	ldr	ip, [fp, #-248]
	str	r9, [r7, #68]
.L236:
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L247:
	ldrb	r2, [r1, #158]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L213
	mov	r0, r1
	mov	r2, r9
	mov	r1, r5
	str	r3, [fp, #-248]
	bl	PostPro_CopyYuv
	ldr	r3, [fp, #-248]
	subs	ip, r0, #0
	beq	.L251
.L215:
	mov	ip, #0
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L248:
	ldrb	r2, [r1, #1]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L213
	ldr	r1, .L252+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r2, r9
	mov	r1, r5
	ldr	r0, [r10]
	bl	PostPro_CopyYuv_Vc1
	subs	ip, r0, #0
	bne	.L215
	rsb	r6, r6, r8
	ldr	r10, .L252+4
	add	r6, r7, r6
	mov	r2, r9
	mov	r1, r5
	mov	r0, #2
	str	ip, [r6, #4]
	add	r7, r7, r4, lsl #2
	str	ip, [fp, #-248]
	bl	VDMDRV_StartRepair
	ldr	r3, [r10]
	blx	r3
	ldr	r2, .L252+28
	mov	r3, r5, asl #7
	sub	r3, r3, r5, asl #5
	add	r3, r2, r3
	add	r2, r5, #1
	str	r2, [r7, #196]
	mov	r1, r0
	mov	r0, r4
	str	r1, [r6, #20]
	str	r1, [r3, #4]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	ldr	r6, .L252+12
	cmp	r0, #0
	beq	.L215
	ldr	r3, [r10]
	add	r6, r6, r4, lsl #2
	blx	r3
	ldr	r2, .L252+32
	ldr	r3, [r6, #2144]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	addhi	r2, r5, r4, lsl #24
	movhi	r1, #704
	bls	.L215
.L238:
	add	r2, r2, #458752
	ldr	r0, .L252+36
	str	lr, [r6, #2144]
	bl	DBG_AddTrace
	ldr	ip, [fp, #-248]
	b	.L236
.L250:
	ldr	r6, .L252+4
	mov	r1, r9
	mov	r0, r5
	str	ip, [fp, #-248]
	bl	VDMDRV_StartDec
	ldr	r3, [r6]
	blx	r3
	ldr	r2, .L252+28
	mov	r3, r5, asl #7
	sub	r3, r3, r5, asl #5
	add	r3, r2, r3
	add	r2, r5, #1
	mov	r1, r0
	ldr	r0, .L252+12
	str	r1, [r10, #20]
	add	lr, r0, r4, lsl #2
	str	r1, [r3, #4]
	mov	r0, r4
	str	r2, [lr, #196]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	r0, #5
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L215
	ldr	r3, [r6]
	blx	r3
	ldr	r3, .L252+12
	ldr	r1, .L252+32
	add	r3, r3, r4, lsl #2
	ldr	r2, [r3, #2144]
	ldr	r1, [r1]
	rsb	r2, r2, r0
	mov	lr, r0
	cmp	r2, r1
	bls	.L215
	add	r2, r5, r4, lsl #24
	mov	r1, #784
	add	r2, r2, #327680
	ldr	r0, .L252+36
	str	lr, [r3, #2144]
	bl	DBG_AddTrace
	ldr	ip, [fp, #-248]
	b	.L236
.L245:
	mov	r2, r4
	ldr	r1, .L252+40
	mov	r0, #22
	mov	ip, #1
	str	ip, [fp, #-248]
	bl	dprint_vfmw
	ldr	ip, [fp, #-248]
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L249:
	ldr	r1, .L252+24
	mov	r0, #22
	bl	dprint_vfmw
	mov	r2, r9
	mov	r1, r5
	ldr	r0, [r10]
	bl	PostPro_CopyYuv_Vp6
	subs	ip, r0, #0
	bne	.L215
	rsb	r6, r6, r8
	ldr	r10, .L252+4
	add	r6, r7, r6
	mov	r2, r9
	mov	r1, r5
	mov	r0, #2
	str	ip, [r6, #4]
	add	r7, r7, r4, lsl #2
	str	ip, [fp, #-248]
	bl	VDMDRV_StartRepair
	ldr	r3, [r10]
	blx	r3
	ldr	r2, .L252+28
	mov	r3, r5, asl #7
	sub	r3, r3, r5, asl #5
	add	r3, r2, r3
	add	r2, r5, #1
	str	r2, [r7, #196]
	mov	r1, r0
	mov	r0, r4
	str	r1, [r6, #20]
	str	r1, [r3, #4]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	ldr	r6, .L252+12
	cmp	r0, #0
	beq	.L215
	ldr	r3, [r10]
	add	r6, r6, r4, lsl #2
	blx	r3
	ldr	r2, .L252+32
	ldr	r3, [r6, #2144]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	bls	.L215
	add	r2, r5, r4, lsl #24
	movw	r1, #737
	b	.L238
.L246:
	mov	r0, r1
	ldr	r1, .L252+44
	bl	dprint_vfmw
	rsb	r2, r6, r8
	ldr	r1, [r10]
	add	r2, r7, r2
	add	r2, r2, #8
	cmp	r1, #0
	ldr	r3, [r2, #4]
	bne	.L211
	mov	r0, r1
	ldr	r1, .L252+48
	str	r2, [fp, #-248]
	bl	dprint_vfmw
	ldr	r2, [fp, #-248]
	ldr	r1, [r10]
	ldr	r3, [r2, #4]
	b	.L213
.L251:
	rsb	r6, r6, r8
	ldr	r10, .L252+4
	add	r6, r7, r6
	mov	r2, r9
	mov	r0, r3
	mov	r1, r5
	str	ip, [r6, #4]
	add	r7, r7, r4, lsl #2
	str	ip, [fp, #-248]
	bl	VDMDRV_StartRepair
	ldr	r3, [r10]
	blx	r3
	ldr	r2, .L252+28
	mov	r3, r5, asl #7
	sub	r3, r3, r5, asl #5
	add	r3, r2, r3
	add	r2, r5, #1
	str	r2, [r7, #196]
	mov	r1, r0
	mov	r0, r4
	str	r1, [r6, #20]
	str	r1, [r3, #4]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	ldr	r6, .L252+12
	ldr	ip, [fp, #-248]
	cmp	r0, #0
	beq	.L215
	ldr	r3, [r10]
	add	r6, r6, r4, lsl #2
	str	ip, [fp, #-248]
	blx	r3
	ldr	r2, .L252+32
	ldr	r3, [r6, #2144]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	addhi	r2, r5, r4, lsl #24
	movwhi	r1, #671
	bls	.L215
	b	.L238
.L253:
	.align	2
.L252:
	.word	g_HalDisable
	.word	vfmw_Osal_Func_Ptr_S
	.word	AcceleratorCharacter
	.word	.LANCHOR0
	.word	g_vdm_hal_fun_ptr
	.word	.LC15
	.word	.LC14
	.word	g_VfmwGlobalStat
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+44
	.word	.LC13
	.word	.LC10
	.word	.LC11
	UNWIND(.fnend)
	.size	VDMDRV_DoBeforeDec, .-VDMDRV_DoBeforeDec
	.align	2
	.global	VDMDRV_ActivateVDH
	.type	VDMDRV_ActivateVDH, %function
VDMDRV_ActivateVDH:
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
	ldr	r9, .L260
	mov	r5, r0, asl #6
	ldr	r6, .L260+4
	sub	r5, r5, r0, asl #3
	mov	r8, r0
	mov	r0, #5
	ldr	r3, [r9, #12]
	add	r7, r5, #48
	blx	r3
	ldr	r3, [r6, #132]
	add	r7, r6, r7
	cmp	r3, #0
	blt	.L255
	add	r5, r6, r5
	add	r4, r6, #132
	add	r6, r6, #192
	b	.L256
.L259:
	cmp	r4, r6
	beq	.L255
	ldr	r3, [r4, #4]!
	cmp	r3, #0
	blt	.L255
.L256:
	mov	r0, r3
	mov	r1, r7
	str	r3, [r5, #8]
	bl	VDMDRV_ChnDecReady
	cmp	r0, #0
	bne	.L259
	mov	r2, r7
	ldr	r1, [r5, #8]
	mov	r0, r8
	bl	VDMDRV_DoBeforeDec
	cmp	r0, #0
	bne	.L259
.L255:
	ldr	r3, [r9, #16]
	mov	r0, #5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L261:
	.align	2
.L260:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_ActivateVDH, .-VDMDRV_ActivateVDH
	.align	2
	.global	VDMDRV_AfterDec
	.type	VDMDRV_AfterDec, %function
VDMDRV_AfterDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 368
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #380)
	sub	sp, sp, #380
	ldr	r3, .L363
	mov	r5, r0
	str	r1, [fp, #-404]
	ldr	r8, .L363+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [fp, #-408]
	beq	.L263
	sub	r0, fp, #232
	mov	r2, #188
	mov	r1, #0
	ldr	r3, [r8, #48]
	str	r0, [fp, #-408]
	blx	r3
.L263:
	ldr	r4, .L363+8
	mov	r10, r5, asl #3
	mov	r9, r5, asl #6
	ldr	r2, .L363+12
	rsb	r3, r10, r9
	mov	r1, #0
	add	r3, r4, r3
	sub	r0, fp, #368
	rsb	r7, r10, r9
	ldr	r3, [r3, #8]
	add	r6, r4, r7
	ldr	r3, [r2, r3, asl #2]
	mov	r2, #136
	cmp	r3, #0
	ldrne	r3, [r3, #1224]
	str	r3, [fp, #-400]
	ldr	r3, [r8, #48]
	blx	r3
	mov	r3, #180
	mla	r3, r3, r5, r4
	ldr	lr, [r6, #4]
	ldr	r1, .L363+16
	mov	r0, #2
	add	ip, r6, #24
	str	ip, [fp, #-392]
	ldr	r2, [r3, #2208]
	and	r2, r2, #15
	cmp	r2, #5
	ldr	r2, [r4, r7]
	ldreq	r3, [r3, #2220]
	ldrne	r3, [r3, #2216]
	andeq	r3, r3, #1
	andne	r3, r3, #31
	str	r3, [fp, #-396]
	ldr	r3, [r6, #28]
	str	lr, [sp]
	bl	dprint_vfmw
	ldr	r7, [r4, r7]
	ldr	ip, [fp, #-392]
	cmp	r7, #1
	beq	.L347
	cmp	r7, #2
	beq	.L348
	cmp	r7, #3
	beq	.L349
	ldr	r1, [fp, #-404]
	mov	r3, r7
	ldr	r2, [r6, #8]
	mov	r0, #0
	str	r1, [sp]
	ldr	r1, .L363+20
	bl	dprint_vfmw
	ldr	r3, [r6, #8]
	ldr	r1, .L363+8
	mov	r2, #0
	add	r3, r3, #32
	add	r9, r1, r3, lsl #2
	str	r2, [r9, #68]
.L262:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L349:
	ldr	r3, [ip, #4]
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [ip, #4]
.L306:
	rsb	ip, r10, r9
	str	r5, [sp]
	add	r7, r4, ip
	ldr	r2, [fp, #-396]
	sub	r3, fp, #368
	mov	r6, #0
	ldr	r1, [r7, #4]
	ldr	r0, [r7, #8]
	cmp	r1, #100
	str	r6, [r4, ip]
	movge	r1, #100
	bic	r1, r1, r1, asr #31
	str	r1, [r7, #4]
.L342:
	bl	VCTRL_VdmPostProc
	ldr	r3, [r7, #8]
	ldr	ip, [r8, #48]
	mov	r1, r6
	add	r3, r3, #32
	mov	r2, #56
	mov	r0, r7
	add	r4, r4, r3, lsl #2
	str	r6, [r4, #68]
	blx	ip
	mov	r0, r5
	bl	VDMDRV_ActivateVDH
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L347:
	ldr	r3, [ip, #4]
	cmp	r3, #1
	beq	.L350
	mov	r1, #0
	add	r3, r6, #8
	mov	r2, #12
	str	r3, [fp, #-392]
	sub	r0, fp, #380
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r3, [fp, #-392]
	sub	r0, fp, #380
	str	r5, [fp, #-380]
	ldr	r2, [r3, #4]
	ldr	r3, [r6, #48]
	str	r2, [fp, #-376]
	str	r3, [fp, #-372]
	bl	PostPro_GetErrRatio
	ldr	r2, [r6, #8]
	ldr	r1, .L363+24
	mov	ip, r0
	mov	r0, r7
	mov	r3, ip
	str	ip, [r6, #4]
	bl	dprint_vfmw
	ldr	r3, [fp, #-392]
	ldr	r3, [r3, #4]
	sub	r2, r3, #8
	cmp	r3, #5
	cmpne	r2, #1
	movls	r3, #0
	strls	r3, [r6, #4]
	bls	.L269
	ldr	r3, [r6, #4]
	cmp	r3, #0
	beq	.L269
	ldr	r2, .L363+28
	ldr	ip, [r2]
	cmp	ip, #0
	beq	.L269
	sub	r2, fp, #44
	ldr	r0, [r6, #8]
	mov	r1, #114
	str	r3, [r2, #-340]!
	mov	r3, #4
	blx	ip
.L269:
	ldr	r3, [fp, #-400]
	rsb	r6, r10, r9
	add	r6, r4, r6
	ldr	r7, .L363+8
	cmp	r3, #1
	add	ip, r6, #8
	rsbeq	r3, r10, r9
	addeq	r3, r4, r3
	moveq	r2, #0
	streq	r2, [r3, #4]
	ldr	r3, [r6, #8]
	ldr	r2, .L363+12
	ldr	r3, [r2, r3, asl #2]
	add	r3, r3, #294912
	add	r3, r3, #1920
	ldrsb	r3, [r3]
	cmp	r3, #0
	ldreq	r1, [r6, #4]
	bne	.L351
.L310:
	ldr	r3, [fp, #-400]
	rsb	ip, r10, r9
	cmp	r3, #1
	beq	.L352
	cmp	r1, #100
	add	r6, r4, ip
	str	r5, [sp]
	mov	r7, #0
	movge	r1, #100
	str	r7, [r4, ip]
	sub	r3, fp, #368
	ldr	r0, [r6, #8]
	bic	r1, r1, r1, asr #31
	ldr	r2, [fp, #-396]
	str	r1, [r6, #4]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r6, #8]
	ldr	ip, [r8, #48]
	mov	r1, r7
	add	r3, r3, #32
	mov	r2, #56
	mov	r0, r6
	add	r4, r4, r3, lsl #2
	str	r7, [r4, #68]
	blx	ip
	mov	r0, r5
	bl	VDMDRV_ActivateVDH
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L348:
	rsb	r6, r10, r9
	ldr	r3, [ip, #4]
	add	r6, r4, r6
	ldr	r7, .L363+8
	cmp	r3, #1
	ldr	r0, [r6, #12]
	moveq	r3, #0
	streq	r3, [ip, #4]
	cmp	r0, #6
	ldr	ip, [r6, #48]
	beq	.L353
.L295:
	cmp	ip, #0
	beq	.L354
	cmp	r0, #2
	beq	.L355
	rsb	r3, r10, r9
	add	r3, r4, r3
	ldr	r0, [r3, #12]
	ldr	r1, [r3, #48]
.L299:
	cmp	r0, #1
	beq	.L356
.L303:
	bl	VDMDRV_IsVp6NvopCopy
	cmp	r0, #1
	beq	.L301
	rsb	ip, r10, r9
	ldr	r7, .L363+8
	add	r6, r4, ip
	ldr	r0, [r6, #48]
	ldr	r3, [r6, #12]
	cmp	r0, #0
	beq	.L357
	cmp	r3, #17
	bne	.L306
	add	r0, r0, #262144
	ldrb	r3, [r0, #1028]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L306
	mov	r4, #0
	ldr	r0, [r6, #8]
	sub	r3, fp, #368
	str	r5, [sp]
	ldr	r2, [fp, #-396]
	mov	r1, r4
	str	r4, [r7, ip]
	str	r4, [r6, #4]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r6, #8]
	mov	r1, r4
	ldr	ip, [r8, #48]
	add	r3, r3, #32
	mov	r0, r6
	mov	r2, #56
	add	r7, r7, r3, lsl #2
	str	r4, [r7, #68]
	blx	ip
	mov	r0, r5
	bl	VDMDRV_ActivateVDH
	b	.L262
.L351:
	ldr	r1, [r6, #48]
	ldr	r0, [r6, #12]
	str	ip, [fp, #-392]
	bl	PostPro_GetRepairStratage
	ldr	r1, [r6, #4]
	ldr	ip, [fp, #-392]
	cmp	r1, #0
	cmpne	r0, #1
	moveq	r2, #1
	movne	r2, #0
	beq	.L310
	ldr	r0, [ip, #4]
	ldr	r1, [r6, #48]
	cmp	r0, #6
	beq	.L358
.L285:
	ldr	r3, .L363+32
	ldr	ip, [r3, #140]
	cmp	ip, #0
	beq	.L297
	ldr	r3, [fp, #-408]
	mov	r2, #0
	str	r3, [sp]
	mov	r3, r5
	blx	ip
	cmp	r0, #0
	beq	.L359
.L297:
	rsb	r6, r10, r9
	str	r5, [sp]
	add	r5, r4, r6
	sub	r3, fp, #368
	ldr	r2, [fp, #-396]
	mov	r1, #100
	ldr	r0, [r5, #8]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r5, #8]
	mov	r2, #0
	str	r2, [r4, r6]
	add	r3, r3, #32
	add	r4, r4, r3, lsl #2
	str	r2, [r4, #68]
	b	.L262
.L353:
	ldrb	r2, [ip, #17]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L295
	movw	r3, #1752
	mla	r3, r3, r5, r7
	ldr	r3, [r3, #2108]
	cmp	r3, #0
	ble	.L295
	ldr	r3, .L363+32
	ldr	r3, [r3, #140]
	subs	lr, r3, #0
	beq	.L297
	ldr	r3, [fp, #-408]
	mov	r1, ip
	mov	ip, lr
	str	r3, [sp]
	mov	r3, r5
	blx	ip
	cmp	r0, #0
	bne	.L297
	ldr	r2, [fp, #-408]
	mov	r1, r5
	mov	r0, #3
	ldr	r4, .L363+4
	bl	VDMDRV_StartRepair
	ldr	r3, [r8]
	blx	r3
	ldr	r2, .L363+36
	mov	r3, r5, asl #7
	sub	r3, r3, r5, asl #5
	add	r3, r2, r3
	mov	r1, r0
	mov	r0, #7
	str	r1, [r6, #20]
	str	r1, [r3, #4]
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L262
	ldr	r3, [r4]
	blx	r3
	ldr	r2, [r6, #8]
	ldr	r3, .L363+40
	add	r7, r7, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r7, #2388]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L262
	add	r2, r5, r2, lsl #24
	movw	r1, #1192
	add	r2, r2, #458752
	ldr	r0, .L363+44
	str	ip, [r7, #2388]
	bl	DBG_AddTrace
	b	.L262
.L350:
	mov	r2, r5
	ldr	r1, [r6, #48]
	ldr	r0, [r6, #12]
	bl	PostPro_SetFullRepair
	ldr	ip, [fp, #-392]
	mov	r3, #0
	mov	lr, #100
	ldr	r2, [fp, #-404]
	mov	r0, r3
	ldr	r1, .L363+48
	str	r3, [ip, #4]
	str	lr, [r6, #4]
	bl	dprint_vfmw
	b	.L269
.L352:
	add	r6, r4, ip
	cmp	r1, #100
	ldr	r7, .L363+8
	ldr	r3, [r6, #44]
	movge	r1, #100
	bic	r1, r1, r1, asr #31
	str	r1, [r6, #4]
	cmp	r3, #1
	beq	.L360
	cmp	r3, #2
	beq	.L361
	cmp	r3, #3
	bne	.L262
	ldr	r0, [r6, #8]
	sub	r3, fp, #368
	ldr	r2, [fp, #-396]
	str	r5, [sp]
	str	ip, [fp, #-392]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r6, #12]
	mov	r2, #0
	ldr	ip, [fp, #-392]
	cmp	r3, r2
	str	r2, [r7, ip]
	beq	.L343
	cmp	r3, #16
	bne	.L284
	ldr	r3, [r6, #8]
	ldr	r1, .L363+12
	ldr	r3, [r1, r3, asl #2]
	add	r3, r3, #299008
	str	r2, [r3, #200]
.L283:
	rsb	r0, r10, r9
	mov	ip, #0
	add	r0, r4, r0
	ldr	r6, [r8, #48]
	mov	r1, ip
	mov	r2, #56
	ldr	r3, [r0, #8]
	add	r3, r3, #32
	add	r4, r4, r3, lsl #2
	str	ip, [r4, #68]
	blx	r6
	mov	r0, r5
	bl	VDMDRV_ActivateVDH
	b	.L262
.L359:
	ldr	r2, [fp, #-408]
	mov	r1, r5
	mov	r0, #2
	ldr	r6, .L363+4
	bl	VDMDRV_StartRepair
	ldr	r3, [r8]
	blx	r3
	ldr	r2, .L363+36
	mov	r3, r5, asl #7
	ldr	r7, .L363+8
	sub	r3, r3, r5, asl #5
	add	r3, r2, r3
	mov	r1, r0
	rsb	r0, r10, r9
	add	r4, r4, r0
	str	r1, [r3, #4]
	mov	r0, #7
	str	r1, [r4, #20]
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L262
	ldr	r3, [r6]
	blx	r3
	ldr	r2, [r4, #8]
	ldr	r3, .L363+40
	add	r7, r7, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r7, #2388]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L262
	add	r2, r5, r2, lsl #24
	movw	r1, #1151
	add	r2, r2, #458752
	ldr	r0, .L363+44
	str	ip, [r7, #2388]
	bl	DBG_AddTrace
	b	.L262
.L360:
	ldr	r0, [r6, #8]
	sub	r3, fp, #368
	str	r5, [sp]
	ldr	r2, [fp, #-396]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r6, #12]
	cmp	r3, #0
	beq	.L343
	cmp	r3, #16
	bne	.L278
	ldr	r3, [r6, #8]
	mov	r2, #0
	ldr	r1, .L363+12
	ldr	r3, [r1, r3, asl #2]
	add	r3, r3, #299008
	str	r2, [r3, #200]
	b	.L283
.L355:
	ldrb	r3, [ip, #158]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L301
	rsb	r3, r10, r9
	add	r3, r4, r3
	ldr	r0, [r3, #12]
	ldr	r1, [r3, #48]
	b	.L303
.L356:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L303
.L301:
	rsb	ip, r10, r9
	mov	r6, #0
	add	r7, r4, ip
	str	r5, [sp]
	sub	r3, fp, #368
	ldr	r2, [fp, #-396]
	ldr	r0, [r7, #8]
	mov	r1, r6
	str	r6, [r4, ip]
	str	r6, [r7, #4]
	b	.L342
.L358:
	ldrb	r3, [r1, #17]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L362
.L286:
	ldrb	r2, [r1, #17]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L285
	ldr	r6, .L363+8
	movw	r3, #1752
	mla	r3, r3, r5, r6
	ldr	r3, [r3, #2108]
	cmp	r3, #0
	ble	.L285
	ldr	r3, .L363+32
	ldr	ip, [r3, #140]
	cmp	ip, #0
	beq	.L297
	ldr	r3, [fp, #-408]
	mov	r0, #6
	str	r3, [sp]
	mov	r3, r5
	blx	ip
	cmp	r0, #0
	bne	.L297
	ldr	r2, [fp, #-408]
	mov	r1, r5
	mov	r0, #3
	rsb	r7, r10, r9
	bl	VDMDRV_StartRepair
	ldr	r3, [r8]
	blx	r3
	ldr	r2, .L363+36
	mov	r3, r5, asl #7
	add	r7, r6, r7
	sub	r3, r3, r5, asl #5
	ldr	r4, .L363+4
	add	r3, r2, r3
	mov	r1, r0
	mov	r0, #7
	str	r1, [r7, #20]
	str	r1, [r3, #4]
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L262
	ldr	r3, [r4]
	blx	r3
	ldr	r2, [r7, #8]
	ldr	r3, .L363+40
	add	r6, r6, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r6, #2388]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L262
	add	r2, r5, r2, lsl #24
	movw	r1, #1119
	add	r2, r2, #458752
	ldr	r0, .L363+44
	str	ip, [r6, #2388]
	bl	DBG_AddTrace
	b	.L262
.L354:
	rsb	r6, r10, r9
	ldr	r1, .L363+52
	add	r6, r4, r6
	mov	r0, ip
	str	ip, [fp, #-392]
	bl	dprint_vfmw
	ldr	r1, [r6, #48]
	add	r7, r6, #8
	ldr	ip, [fp, #-392]
	cmp	r1, #0
	ldr	r0, [r7, #4]
	bne	.L299
	ldr	r1, .L363+56
	mov	r0, ip
	bl	dprint_vfmw
	ldr	r0, [r7, #4]
	ldr	r1, [r6, #48]
	b	.L303
.L362:
	movw	r3, #1752
	mla	r3, r3, r5, r7
	ldr	r3, [r3, #1232]
	cmp	r3, #0
	ble	.L286
	ldr	r3, .L363+32
	ldr	ip, [r3, #140]
	cmp	ip, #0
	beq	.L297
	ldr	r3, [fp, #-408]
	str	r3, [sp]
	mov	r3, r5
	blx	ip
	cmp	r0, #0
	bne	.L297
	ldr	r2, [fp, #-408]
	mov	r1, r5
	mov	r0, #2
	ldr	r4, .L363+4
	bl	VDMDRV_StartRepair
	ldr	r3, [r8]
	blx	r3
	ldr	r2, .L363+36
	mov	r3, r5, asl #7
	sub	r3, r3, r5, asl #5
	add	r3, r2, r3
	mov	r1, r0
	mov	r0, #7
	str	r1, [r6, #20]
	str	r1, [r3, #4]
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L262
	ldr	r3, [r4]
	blx	r3
	ldr	r2, [r6, #8]
	ldr	r3, .L363+40
	add	r7, r7, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r7, #2388]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L262
	add	r2, r5, r2, lsl #24
	mov	r1, #1088
	add	r2, r2, #458752
	ldr	r0, .L363+44
	str	ip, [r7, #2388]
	bl	DBG_AddTrace
	b	.L262
.L343:
	ldr	r2, [r6, #8]
	ldr	r1, .L363+12
	ldr	r2, [r1, r2, asl #2]
	add	r2, r2, #294912
	str	r3, [r2, #2328]
	b	.L283
.L357:
	ldr	r1, .L363+60
	bl	dprint_vfmw
	b	.L306
.L361:
	ldr	r0, [r6, #8]
	sub	r3, fp, #368
	str	r5, [sp]
	ldr	r2, [fp, #-396]
	bl	VCTRL_VdmPostProc
	b	.L262
.L284:
	cmp	r3, #17
	ldreq	r3, [r6, #8]
	ldreq	r1, .L363+12
	ldreq	r3, [r1, r3, asl #2]
	addeq	r3, r3, #294912
	streq	r2, [r3, #1972]
	b	.L283
.L278:
	cmp	r3, #17
	bne	.L283
	ldr	r3, [r6, #8]
	mov	r2, #0
	ldr	r1, .L363+12
	ldr	r3, [r1, r3, asl #2]
	add	r3, r3, #294912
	str	r2, [r3, #1972]
	b	.L283
.L364:
	.align	2
.L363:
	.word	g_HalDisable
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	.LC16
	.word	.LC20
	.word	.LC18
	.word	g_event_report
	.word	g_vdm_hal_fun_ptr
	.word	g_VfmwGlobalStat
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+64
	.word	.LC17
	.word	.LC10
	.word	.LC11
	.word	.LC19
	UNWIND(.fnend)
	.size	VDMDRV_AfterDec, .-VDMDRV_AfterDec
	.align	2
	.global	VDMDRV_CheckDevice
	.type	VDMDRV_CheckDevice, %function
VDMDRV_CheckDevice:
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
	ldr	r4, .L399
	mov	r7, r0, asl #3
	mov	r5, r0, asl #6
	mov	r6, r0
	rsb	r8, r7, r5
	add	r9, r4, r8
	ldr	r0, [r9, #8]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L395
.L366:
	rsb	r3, r7, r5
	add	r8, r4, r3
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bne	.L396
.L367:
	add	r4, r4, r6, lsl #2
	ldr	r0, [r4, #56]
	cmp	r0, #0
	ldmeqfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
	cmp	r0, #1
	beq	.L397
.L372:
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L396:
	ldr	r3, .L399+4
	ldr	r3, [r3]
	blx	r3
	add	r2, r8, #16
	ldr	r3, [r2, #4]
	str	r0, [r8, #24]
	cmp	r0, r3
	bcc	.L370
	clz	r9, r3
	rsb	r3, r3, r0
	mov	r9, r9, lsr #5
	orrs	r9, r9, r3, lsr #31
	bne	.L370
	cmp	r3, #1000
	bls	.L372
	ldr	r2, [r8, #8]
	mov	r0, #1
	ldr	r1, .L399+8
	bl	dprint_vfmw
	ldr	r3, .L399+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L375
	ldr	r0, [r8, #8]
	mov	r3, r9
	mov	r2, r9
	mov	r1, #101
	blx	ip
.L375:
	ldr	r3, .L399+16
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L376
	mov	r1, r6
	mov	r0, #1
	bl	VDH_Reset_Module
.L377:
	rsb	r3, r7, r5
	mov	r5, #1
	add	r4, r4, r3
	ldr	r2, .L399
	add	lr, r4, #32
	add	ip, r4, #24
	ldr	r1, [r4, #36]
	str	r5, [r4, #28]
	cmp	r1, r5
	bne	.L379
	ldr	r4, [r4, #8]
	mov	r1, #0
	str	r1, [lr, #4]
	mov	r0, r1
	add	lr, r4, #32
	str	r1, [r2, r3]
	str	r1, [ip, #4]
	add	r2, r2, lr, lsl #2
	str	r1, [r2, #68]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L370:
	str	r0, [r2, #4]
	mov	r0, #1
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L397:
	mov	r3, #2
	str	r3, [r4, #56]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L395:
	ldr	r3, [r4, r8]
	cmp	r3, #0
	beq	.L367
	ldr	r2, .L399+20
	ldr	r2, [r2, #136]
	cmp	r2, #0
	beq	.L368
	mov	r0, r6
	blx	r2
	ldr	r3, [r4, r8]
	cmp	r0, #0
	bne	.L398
.L368:
	rsb	r10, r7, r5
	ldr	r1, .L399+24
	add	r9, r4, r10
	mov	r0, #1
	mov	r8, #0
	ldr	r2, [r9, #8]
	bl	dprint_vfmw
	ldr	r2, [r4, r10]
	movw	r1, #1309
	ldr	r0, .L399+28
	str	r8, [r9, #36]
	bl	DBG_AddTrace
	ldr	r3, .L399+4
	mov	r0, r9
	mov	r1, r8
	mov	r2, #56
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, [r9, #8]
	add	r3, r3, #32
	add	r3, r4, r3, lsl #2
	str	r8, [r3, #68]
	b	.L366
.L379:
	mov	r0, r6
	mov	r1, #0
	bl	VDMDRV_AfterDec
	mov	r0, r5
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L376:
	ldr	r3, .L399+20
	ldr	r3, [r3, #80]
	cmp	r3, #0
	beq	.L378
	mov	r0, r6
	blx	r3
	b	.L377
.L398:
	mov	r0, #1
	ldr	r2, [r9, #8]
	ldr	r1, .L399+32
	bl	dprint_vfmw
	mov	r3, #1
	str	r3, [r9, #36]
	b	.L366
.L378:
	mov	r0, r3
	mov	r2, #1360
	ldr	r1, .L399+36
	bl	dprint_vfmw
	b	.L377
.L400:
	.align	2
.L399:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC23
	.word	g_event_report
	.word	g_HalDisable
	.word	g_vdm_hal_fun_ptr
	.word	.LC22
	.word	.LANCHOR1+80
	.word	.LC21
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMDRV_CheckDevice, .-VDMDRV_CheckDevice
	.align	2
	.global	VDMDRV_WakeUpVdm
	.type	VDMDRV_WakeUpVdm, %function
VDMDRV_WakeUpVdm:
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
	bl	VDMDRV_CheckDevice
	cmp	r0, #0
	ldmnefd	sp, {fp, sp, pc}
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDMDRV_ActivateVDH
	UNWIND(.fnend)
	.size	VDMDRV_WakeUpVdm, .-VDMDRV_WakeUpVdm
	.global	__aeabi_uidiv
	.align	2
	.global	VDMDRV_VdmIntServProc
	.type	VDMDRV_VdmIntServProc, %function
VDMDRV_VdmIntServProc:
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
	subs	r4, r0, #0
	mov	r3, #15
	str	r3, [fp, #-56]
	bgt	.L462
	mov	r3, r4, asl #3
	mov	r5, r4, asl #6
	ldr	r6, .L472
	rsb	r7, r3, r5
	str	r3, [fp, #-64]
	add	r7, r6, r7
	ldr	r3, .L472+4
	ldr	r8, .L472+8
	ldr	r2, [r7, #8]
	ldr	r3, [r3, r2, asl #2]
	cmp	r3, #0
	beq	.L406
	ldr	r9, .L472+12
	mov	r0, #0
	ldr	r10, [r3, #1224]
	ldr	r3, [r8, #12]
	blx	r3
	ldr	r3, [r9, #108]
	cmp	r3, #0
	beq	.L411
	mov	r1, r4
	mov	r0, #2
	blx	r3
	cmp	r10, #1
	mov	r3, r0
	beq	.L463
.L408:
	tst	r3, #1
	beq	.L411
.L413:
	mov	r3, #1
	str	r3, [fp, #-72]
.L412:
	mov	r3, r4, asl #2
	mov	r1, #1
	add	r2, r6, r3
	str	r3, [fp, #-68]
	sub	r0, fp, #52
	ldr	r3, [r8, #48]
	str	r1, [r2, #388]
	mov	r2, #8
	mov	r1, #0
	blx	r3
	ldr	r3, [r9, #148]
	sub	r2, fp, #56
	str	r4, [fp, #-48]
	cmp	r3, #0
	str	r2, [fp, #-52]
	beq	.L417
	sub	r0, fp, #52
	blx	r3
.L418:
	ldr	r3, [r9, #92]
	cmp	r3, #0
	beq	.L419
	mov	r0, r4
	blx	r3
.L420:
	mov	r0, #6
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L464
.L422:
	ldr	r3, [fp, #-64]
	rsb	r2, r3, r5
	ldr	r3, .L472
	add	r0, r6, r2
	add	r1, r0, #32
	ldr	ip, [r0, #36]
	cmp	ip, #1
	beq	.L465
	mov	r2, #180
	mov	r1, r4, asl #5
	mov	r0, r4, asl #7
	ldr	r7, .L472+16
	mla	r2, r2, r4, r3
	rsb	r9, r1, r0
	str	r1, [fp, #-76]
	add	r10, r7, r9
	ldr	r1, [fp, #-68]
	str	r0, [fp, #-80]
	add	r3, r3, r1
	ldr	r0, [r10, #4]
	ldr	r2, [r2, #2232]
	ldr	r1, [r3, #2516]
	cmp	r0, #0
	add	r2, r1, r2
	str	r2, [r3, #2516]
	bne	.L466
.L427:
	ldr	r3, [fp, #-64]
	rsb	r3, r3, r5
	add	r7, r6, r3
	ldr	r2, [r6, r3]
	cmp	r2, #0
	beq	.L433
	ldr	r9, [r7, #48]
	cmp	r9, #0
	beq	.L467
.L433:
	ldr	r3, [fp, #-64]
	ldr	r10, .L472
	rsb	r9, r3, r5
	add	r7, r6, r9
	ldr	r0, [r7, #8]
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L468
	ldr	r0, [r7, #8]
	bl	VCTRL_GetDecParam
	subs	ip, r0, #0
	beq	.L469
	mov	r1, #1
	mov	r0, r4
	bl	VDMDRV_AfterDec
.L436:
	ldr	r3, [fp, #-64]
	mov	r2, #0
	rsb	r5, r3, r5
	ldr	r3, [fp, #-68]
	add	r3, r6, r3
	add	r6, r6, r5
	ldr	r0, [r6, #8]
	str	r2, [r3, #388]
	bl	VCTRL_InformVdmFree
.L425:
	ldr	r3, [fp, #-72]
	cmp	r3, #1
	beq	.L410
.L438:
	ldr	r3, [r8, #16]
	mov	r0, #0
	blx	r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L462:
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L472+20
	ldr	r1, .L472+24
	bl	dprint_vfmw
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L411:
	ldr	r3, [fp, #-64]
	rsb	r5, r3, r5
	mov	r3, #0
	str	r3, [r6, r5]
.L410:
	ldr	r3, .L472+28
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L438
	mov	r1, r4
	mov	r0, #1
	bl	VDH_Notify_ModuleIdle
	b	.L438
.L468:
	ldr	r3, [r7, #8]
	movw	r2, #1612
	ldr	r1, .L472+32
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r7, #8]
	mov	r2, #0
	str	r2, [r10, r9]
	add	r3, r3, #32
	add	r10, r10, r3, lsl #2
	str	r2, [r10, #68]
	b	.L436
.L406:
	ldr	r9, .L472+12
	mov	r0, r3
	ldr	r3, [r8, #12]
	blx	r3
	ldr	r3, [r9, #108]
	cmp	r3, #0
	beq	.L411
	mov	r1, r4
	mov	r0, #2
	blx	r3
	mov	r3, r0
	b	.L408
.L466:
	ldr	r3, [r8]
	blx	r3
	ldr	r1, [r7, r9]
	ldr	r2, [r10, #4]
	mov	ip, #180
	add	r3, r1, #49920
	ldr	lr, [fp, #-76]
	add	r3, r3, #80
	cmp	r3, r2
	ldr	r3, [fp, #-80]
	mla	ip, ip, r4, r6
	ldrcs	r10, [r10, #8]
	movcc	r1, r2
	strcc	r2, [r7, r9]
	rsb	r3, lr, r3
	movcc	r10, #0
	add	r3, r7, r3
	rsb	r9, r1, r0
	rsb	r0, r2, r0
	add	r2, r0, r10
	cmp	r9, #1000
	ldr	r0, [r3, #60]
	add	r10, r3, #8
	mov	r1, r2
	str	r1, [r3, #8]
	ldr	r1, [ip, #2232]
	add	ip, r0, #1
	str	r2, [fp, #-84]
	mov	r2, #0
	str	r2, [r3, #4]
	ldr	r2, [r3, #64]
	str	ip, [r3, #60]
	add	r2, r2, r1, lsr #10
	str	r2, [r3, #64]
	ldr	r3, .L472
	bcc	.L427
	ldr	r1, [fp, #-64]
	rsb	r1, r1, r5
	add	r3, r3, r1
	ldr	r1, [r3, #16]
	cmp	r1, #0
	ldreq	r0, [r10, #4]
	beq	.L432
	mov	r0, r2
	str	ip, [fp, #-92]
	str	r2, [fp, #-88]
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-92]
	ldr	r2, [fp, #-88]
	str	r0, [r10, #4]
.L432:
	mov	r3, #1000
	mov	r1, r9
	mul	r0, r3, r0
	str	ip, [fp, #-92]
	str	r2, [fp, #-88]
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-76]
	mov	r1, r9
	ldr	r3, [fp, #-80]
	rsb	r3, r2, r3
	ldr	r2, [fp, #-84]
	add	r10, r7, r3
	str	r3, [fp, #-76]
	mov	r3, #1000
	str	r0, [r10, #20]
	mul	r0, r3, r2
	bl	__aeabi_uidiv
	ldr	ip, [fp, #-92]
	mov	r3, #1000
	mov	r1, r9
	str	r0, [r10, #16]
	mul	r0, r3, ip
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-88]
	ldr	r3, [fp, #-76]
	str	r9, [r10, #72]
	str	r2, [r10, #68]
	mov	r2, #0
	str	r2, [r7, r3]
	str	r2, [r10, #64]
	str	r2, [r10, #60]
	str	r0, [r10, #24]
	b	.L427
.L463:
	tst	r0, #5
	beq	.L411
	ldr	r3, [r9, #108]
	cmp	r3, #0
	beq	.L413
	mov	r0, r10
	mov	r1, r4
	blx	r3
	and	r0, r0, #655360
	cmp	r0, #655360
	beq	.L416
	cmp	r0, #524288
	beq	.L470
	cmp	r0, #131072
	bne	.L413
	ldr	r3, [r7, #44]
	cmp	r3, #2
	beq	.L471
.L416:
	ldr	r3, [fp, #-64]
	mov	r2, #1
	str	r2, [fp, #-72]
	rsb	r3, r3, r5
	add	r3, r6, r3
	str	r2, [r3, #44]
	b	.L412
.L464:
	ldr	r3, [r8]
	blx	r3
	ldr	r3, [fp, #-64]
	ldr	r1, .L472+36
	rsb	r3, r3, r5
	add	r3, r6, r3
	ldr	r1, [r1]
	ldr	r2, [r3, #8]
	add	ip, r6, r2, lsl #2
	ldr	r3, [ip, #2452]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r1
	bls	.L422
	add	r2, r4, r2, lsl #24
	mov	r1, #1520
	add	r2, r2, #393216
	ldr	r0, .L472+40
	str	lr, [ip, #2452]
	bl	DBG_AddTrace
	b	.L422
.L465:
	ldr	r0, [r0, #8]
	mov	ip, #0
	str	ip, [r1, #4]
	add	r1, r0, #32
	str	ip, [r3, r2]
	add	r3, r3, r1, lsl #2
	str	ip, [r3, #68]
	b	.L425
.L470:
	mov	r3, #2
	mov	r2, #0
	str	r3, [r7, #44]
	str	r2, [fp, #-72]
	b	.L412
.L467:
	ldr	r1, .L472+44
	mov	r0, r9
	bl	dprint_vfmw
	ldr	r0, [r7, #8]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L434
	ldr	r0, [r7, #8]
	mov	r3, r9
	ldr	r2, [fp, #-56]
	mov	r1, #100
	str	r4, [sp]
	bl	VCTRL_VdmPostProc
.L434:
	ldr	r3, [fp, #-64]
	mov	r2, #0
	rsb	r5, r3, r5
	add	r3, r6, r5
	str	r2, [r6, r5]
	ldr	r3, [r3, #8]
	add	r3, r3, #32
	add	r6, r6, r3, lsl #2
	str	r2, [r6, #68]
	b	.L425
.L417:
	mov	r0, r3
	movw	r2, #1508
	ldr	r1, .L472+48
	bl	dprint_vfmw
	b	.L418
.L419:
	mov	r0, r3
	movw	r2, #1510
	ldr	r1, .L472+48
	bl	dprint_vfmw
	b	.L420
.L469:
	ldr	r3, [r7, #8]
	movw	r2, #1622
	ldr	r1, .L472+52
	str	ip, [fp, #-76]
	bl	dprint_vfmw
	ldr	ip, [fp, #-76]
	ldr	r3, [r7, #8]
	str	ip, [r10, r9]
	add	r10, r10, r3, lsl #2
	str	ip, [r10, #196]
	b	.L436
.L471:
	ldr	r3, [fp, #-64]
	mov	r2, #1
	str	r2, [fp, #-72]
	mov	r2, #3
	rsb	r3, r3, r5
	add	r3, r6, r3
	str	r2, [r3, #44]
	b	.L412
.L473:
	.align	2
.L472:
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	g_VfmwGlobalStat
	.word	.LANCHOR1+100
	.word	.LC24
	.word	g_HalDisable
	.word	.LC26
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+124
	.word	.LC25
	.word	.LC1
	.word	.LC27
	UNWIND(.fnend)
	.size	VDMDRV_VdmIntServProc, .-VDMDRV_VdmIntServProc
	.align	2
	.global	VDMDRV_IsVp9RptFrmCopy
	.type	VDMDRV_IsVp9RptFrmCopy, %function
VDMDRV_IsVp9RptFrmCopy:
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
	subs	r4, r1, #0
	beq	.L478
	cmp	r0, #17
	addeq	r0, r4, #262144
	ldreqb	r0, [r0, #1028]	@ zero_extendqisi2
	subeq	r0, r0, #1
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L478:
	ldr	r1, .L479
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L480:
	.align	2
.L479:
	.word	.LC19
	UNWIND(.fnend)
	.size	VDMDRV_IsVp9RptFrmCopy, .-VDMDRV_IsVp9RptFrmCopy
	.align	2
	.global	VDMDRV_ResetVDH
	.type	VDMDRV_ResetVDH, %function
VDMDRV_ResetVDH:
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
	ldr	r4, .L486
	ldr	r3, [r4, #8]
	cmp	r3, r0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L486+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L483
	mov	r1, #0
	mov	r0, #1
	bl	VDH_Reset_Module
.L484:
	mov	r3, #0
	str	r3, [r4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L483:
	ldr	r3, .L486+8
	ldr	r3, [r3, #80]
	cmp	r3, #0
	beq	.L485
	mov	r0, #0
	blx	r3
	b	.L484
.L485:
	mov	r0, r3
	movw	r2, #1965
	ldr	r1, .L486+12
	bl	dprint_vfmw
	b	.L484
.L487:
	.align	2
.L486:
	.word	.LANCHOR0
	.word	g_HalDisable
	.word	g_vdm_hal_fun_ptr
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMDRV_ResetVDH, .-VDMDRV_ResetVDH
	.align	2
	.global	VDMDRV_MarkReloadDsp
	.type	VDMDRV_MarkReloadDsp, %function
VDMDRV_MarkReloadDsp:
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
	subs	lr, r0, #0
	mov	ip, r1
	bne	.L488
	add	r3, r1, #1
	cmp	r3, #16
	bhi	.L488
	ldr	r3, .L495
	cmn	r1, #1
	mov	r2, #1
	str	r2, [r3, #40]
	beq	.L488
	ldr	r4, .L495+4
	mvn	r5, #0
	movw	r3, #1995
	ldr	r2, .L495+8
	ldr	r1, .L495+12
	mov	r0, #22
	ldr	ip, [r4, ip, asl #2]
	str	r5, [ip, #36]
	ldr	ip, [r4]
	ldr	ip, [ip, #36]
	str	lr, [sp]
	str	ip, [sp, #4]
	bl	dprint_vfmw
.L488:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L496:
	.align	2
.L495:
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	.LANCHOR1+148
	.word	.LC28
	UNWIND(.fnend)
	.size	VDMDRV_MarkReloadDsp, .-VDMDRV_MarkReloadDsp
	.align	2
	.global	VDMDRV_UpdateMatchMode
	.type	VDMDRV_UpdateMatchMode, %function
VDMDRV_UpdateMatchMode:
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
	cmp	r0, #0
	mov	ip, r1
	bne	.L497
	cmp	r1, #15
	bls	.L499
.L497:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L499:
	ldr	lr, .L500
	mov	r5, #1
	movw	r3, #2008
	ldr	r2, .L500+4
	ldr	r1, .L500+8
	mov	r0, #22
	ldr	r4, [lr, ip, asl #2]
	str	r5, [r4, #36]
	ldr	lr, [lr, ip, asl #2]
	str	ip, [sp, #4]
	ldr	ip, [lr, #36]
	str	ip, [sp]
	bl	dprint_vfmw
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L501:
	.align	2
.L500:
	.word	s_pstVfmwChan
	.word	.LANCHOR1+172
	.word	.LC29
	UNWIND(.fnend)
	.size	VDMDRV_UpdateMatchMode, .-VDMDRV_UpdateMatchMode
	.global	g_VdmExtParam
	.global	g_DspDrvState
	.global	g_VdmDrvParam
	.global	USE_FF_APT_EN
	.global	g_ChanCtx
	.global	g_BackUp
	.global	g_DecReport
	.global	g_RepairParam
	.global	g_HwMem
	.global	g_UpMsg
	.global	CapItem
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__func__.14290, %object
	.size	__func__.14290, 23
__func__.14290:
	.ascii	"VDMDRV_CheckAttachMode\000"
	.space	1
	.type	__func__.14295, %object
	.size	__func__.14295, 19
__func__.14295:
	.ascii	"VDMDRV_ChnDecReady\000"
	.space	1
	.type	__FUNCTION__.14313, %object
	.size	__FUNCTION__.14313, 19
__FUNCTION__.14313:
	.ascii	"VDMDRV_DoBeforeDec\000"
	.space	1
	.type	__FUNCTION__.14348, %object
	.size	__FUNCTION__.14348, 16
__FUNCTION__.14348:
	.ascii	"VDMDRV_AfterDec\000"
	.type	__FUNCTION__.14358, %object
	.size	__FUNCTION__.14358, 19
__FUNCTION__.14358:
	.ascii	"VDMDRV_CheckDevice\000"
	.space	1
	.type	__func__.14374, %object
	.size	__func__.14374, 22
__func__.14374:
	.ascii	"VDMDRV_VdmIntServProc\000"
	.space	2
	.type	__FUNCTION__.14376, %object
	.size	__FUNCTION__.14376, 22
__FUNCTION__.14376:
	.ascii	"VDMDRV_VdmIntServProc\000"
	.space	2
	.type	__func__.14457, %object
	.size	__func__.14457, 21
__func__.14457:
	.ascii	"VDMDRV_MarkReloadDsp\000"
	.space	3
	.type	__func__.14462, %object
	.size	__func__.14462, 23
__func__.14462:
	.ascii	"VDMDRV_UpdateMatchMode\000"
	.data
	.align	2
	.type	USE_FF_APT_EN, %object
	.size	USE_FF_APT_EN, 4
USE_FF_APT_EN:
	.word	1
	.type	CapItem, %object
	.size	CapItem, 924
CapItem:
	.word	176
	.word	144
	.word	4
	.word	10
	.word	11
	.word	12
	.word	524288
	.word	352
	.word	288
	.word	4
	.word	10
	.word	11
	.word	12
	.word	786432
	.word	720
	.word	576
	.word	4
	.word	10
	.word	11
	.word	12
	.word	1048576
	.word	1280
	.word	736
	.word	4
	.word	10
	.word	11
	.word	12
	.word	2097152
	.word	1920
	.word	1088
	.word	4
	.word	10
	.word	11
	.word	12
	.word	3145728
	.word	176
	.word	144
	.word	15
	.word	19
	.word	22
	.word	24
	.word	524288
	.word	352
	.word	288
	.word	15
	.word	19
	.word	22
	.word	24
	.word	786432
	.word	720
	.word	576
	.word	9
	.word	15
	.word	16
	.word	17
	.word	1048576
	.word	1280
	.word	736
	.word	6
	.word	13
	.word	13
	.word	14
	.word	2097152
	.word	1920
	.word	1088
	.word	6
	.word	13
	.word	13
	.word	13
	.word	2097152
	.word	5632
	.word	4224
	.word	1
	.word	3
	.word	3
	.word	3
	.word	16777216
	.word	1280
	.word	800
	.word	6
	.word	13
	.word	13
	.word	14
	.word	2097152
	.word	800
	.word	1280
	.word	6
	.word	13
	.word	13
	.word	14
	.word	2097152
	.word	1488
	.word	1280
	.word	6
	.word	13
	.word	13
	.word	14
	.word	3145728
	.word	1280
	.word	1488
	.word	6
	.word	13
	.word	13
	.word	14
	.word	3145728
	.word	2160
	.word	1280
	.word	6
	.word	13
	.word	13
	.word	13
	.word	3145728
	.word	1280
	.word	2176
	.word	6
	.word	13
	.word	13
	.word	13
	.word	3145728
	.word	2160
	.word	2176
	.word	4
	.word	11
	.word	11
	.word	11
	.word	4194304
	.word	4096
	.word	2304
	.word	2
	.word	9
	.word	9
	.word	9
	.word	5242880
	.word	2304
	.word	4096
	.word	2
	.word	9
	.word	9
	.word	9
	.word	5242880
	.word	4096
	.word	4096
	.word	2
	.word	9
	.word	9
	.word	9
	.word	8388608
	.word	8192
	.word	4096
	.word	2
	.word	9
	.word	9
	.word	9
	.word	10485760
	.word	4096
	.word	8192
	.word	2
	.word	9
	.word	9
	.word	9
	.word	10485760
	.word	8192
	.word	8192
	.word	1
	.word	3
	.word	3
	.word	3
	.word	20971520
	.word	1920
	.word	1088
	.word	1
	.word	3
	.word	3
	.word	3
	.word	2097152
	.word	1920
	.word	1088
	.word	1
	.word	3
	.word	3
	.word	3
	.word	2097152
	.word	1920
	.word	1088
	.word	17
	.word	26
	.word	26
	.word	26
	.word	2097152
	.word	192
	.word	192
	.word	15
	.word	19
	.word	22
	.word	24
	.word	524288
	.word	384
	.word	320
	.word	15
	.word	19
	.word	22
	.word	24
	.word	786432
	.word	768
	.word	576
	.word	9
	.word	15
	.word	16
	.word	17
	.word	1048576
	.word	1280
	.word	768
	.word	6
	.word	13
	.word	13
	.word	14
	.word	2097152
	.word	1920
	.word	1088
	.word	6
	.word	13
	.word	13
	.word	13
	.word	2097152
	.word	4096
	.word	2304
	.word	4
	.word	4
	.word	4
	.word	4
	.word	16777216
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"====== forece VDM sleep! ======\012\000" )
	.space	3
.LC1:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC2:
	ASCII(.ascii	"%s,%d,  frameRate = %d,last = %d\012\000" )
	.space	2
.LC3:
	ASCII(.ascii	"%s,%d, the frameRate change!\012\000" )
	.space	2
.LC4:
	ASCII(.ascii	"%s,%d,chanid = %d,s32Is4k60fpsb = %d\012\000" )
	.space	2
.LC5:
	ASCII(.ascii	"%s The channel %d is not active\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"The channel %d is decoding\012\000" )
.LC7:
	ASCII(.ascii	"The channel %d DEC PARAM is not ready\012\000" )
	.space	1
.LC8:
	ASCII(.ascii	"SMMU_READ_ERR  MODULE : VFMW \011 SMMU INT STATE: 0" )
	ASCII(.ascii	"x%x \011 read_err_addr  = 0x%x\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"SMMU_WRITE_ERR MODULE : VFMW \011 SMMU INT STATE: 0" )
	ASCII(.ascii	"x%x \011 write_err_addr = 0x%x\012\000" )
	.space	3
.LC10:
	ASCII(.ascii	"VDMHAL_V200_IsMpeg4NvopCopy point of picture para n" )
	ASCII(.ascii	"ull, wait forever!!!\012\000" )
	.space	3
.LC11:
	ASCII(.ascii	"VDMDRV_IsVc1SkpicCopy point of picture para null, w" )
	ASCII(.ascii	"ait forever!!!\012\000" )
	.space	1
.LC12:
	ASCII(.ascii	"VDMDRV_IsVp6NvopCopy point of picture para null, wa" )
	ASCII(.ascii	"it forever!!!\012\000" )
	.space	2
.LC13:
	ASCII(.ascii	"The channel %d DEC PARAM is not ready(pDecParam == " )
	ASCII(.ascii	"NULL)\012\000" )
	.space	2
.LC14:
	ASCII(.ascii	"---SKIP REPAIR!---\012\000" )
.LC15:
	ASCII(.ascii	"VDM Start Fail 2 !\012\000" )
.LC16:
	ASCII(.ascii	"isr begin: *pVdmStateMachine = %d, timeout = %d, Er" )
	ASCII(.ascii	"rRatio = %d\012\000" )
.LC17:
	ASCII(.ascii	"ISR process vdm timeout! FromInt = %d\012\000" )
	.space	1
.LC18:
	ASCII(.ascii	"Chan %d: ErrRatio = %d\012\000" )
.LC19:
	ASCII(.ascii	"VDMDRV_IsVp9RptFrmCopy point of picture para null, " )
	ASCII(.ascii	"wait forever!!!\012\000" )
.LC20:
	ASCII(.ascii	"Chan %d VdmStateMachine=%d Error! FromInt = %d.\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC21:
	ASCII(.ascii	"Chan %d Mfde Running VdmStateMachine = %d, but DecP" )
	ASCII(.ascii	"aram = NULL!\012\000" )
	.space	3
.LC22:
	ASCII(.ascii	"Chan %d VdmStateMachine = %d, but DecParam = NULL, " )
	ASCII(.ascii	"Reset VDH MFDE!\012\000" )
.LC23:
	ASCII(.ascii	"Chan %d VDM Time Out(%d ms), Reset VDM!\012\000" )
	.space	3
.LC24:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC25:
	ASCII(.ascii	"vdm state machine = %d, but DecParam is null!\012\000" )
	.space	1
.LC26:
	ASCII(.ascii	"line :%d ISR called but chan %d is not active!\012\000" )
.LC27:
	ASCII(.ascii	"lline: %d ISR called but chan %d param = NULL!\012\000" )
.LC28:
	ASCII(.ascii	"%s,%d, i = %d, MatchVdhIdPlus1 = %d\012\000" )
	.space	3
.LC29:
	ASCII(.ascii	"%s,%d, MatchVdhIdPlus1 = %d,chanId = %d\012\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_VdmDrvParam, %object
	.size	g_VdmDrvParam, 56
g_VdmDrvParam:
	.space	56
	.type	s_eVdmDrvSleepState, %object
	.size	s_eVdmDrvSleepState, 4
s_eVdmDrvSleepState:
	.space	4
	.type	g_DspDrvState, %object
	.size	g_DspDrvState, 8
g_DspDrvState:
	.space	8
	.type	g_ChanCtx, %object
	.size	g_ChanCtx, 192
g_ChanCtx:
	.space	192
	.type	g_LastFPS.14287, %object
	.size	g_LastFPS.14287, 64
g_LastFPS.14287:
	.space	64
	.type	DiffFrameCnt.14286, %object
	.size	DiffFrameCnt.14286, 64
DiffFrameCnt.14286:
	.space	64
	.type	s_VdmIsInISR, %object
	.size	s_VdmIsInISR, 4
s_VdmIsInISR:
	.space	4
	.type	g_RepairParam, %object
	.size	g_RepairParam, 1752
g_RepairParam:
	.space	1752
	.type	last_rec_pos_time.14309, %object
	.size	last_rec_pos_time.14309, 64
last_rec_pos_time.14309:
	.space	64
	.type	g_BackUp, %object
	.size	g_BackUp, 180
g_BackUp:
	.space	180
	.type	last_rec_pos_time.14340, %object
	.size	last_rec_pos_time.14340, 64
last_rec_pos_time.14340:
	.space	64
	.type	last_rec_pos_time.14369, %object
	.size	last_rec_pos_time.14369, 64
last_rec_pos_time.14369:
	.space	64
	.type	cycle.14368, %object
	.size	cycle.14368, 8
cycle.14368:
	.space	8
	.type	g_VdmExtParam, %object
	.size	g_VdmExtParam, 140
g_VdmExtParam:
	.space	140
	.type	g_DecReport, %object
	.size	g_DecReport, 816
g_DecReport:
	.space	816
	.type	g_HwMem, %object
	.size	g_HwMem, 1228
g_HwMem:
	.space	1228
	.type	g_UpMsg, %object
	.size	g_UpMsg, 3200
g_UpMsg:
	.space	3200
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
