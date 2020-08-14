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
	.type	VDMDRV_IsMpeg4NvopCopy.part.3, %function
VDMDRV_IsMpeg4NvopCopy.part.3:
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
	ldr	r3, .L2
	mov	r0, #0
	ldr	r1, .L2+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L3:
	.align	2
.L2:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC0
	UNWIND(.fnend)
	.size	VDMDRV_IsMpeg4NvopCopy.part.3, .-VDMDRV_IsMpeg4NvopCopy.part.3
	.align	2
	.type	VDMDRV_IsVc1SkpicCopy.part.4, %function
VDMDRV_IsVc1SkpicCopy.part.4:
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
	mov	r0, #0
	ldr	r1, .L5+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L6:
	.align	2
.L5:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC1
	UNWIND(.fnend)
	.size	VDMDRV_IsVc1SkpicCopy.part.4, .-VDMDRV_IsVc1SkpicCopy.part.4
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
	ldr	r3, .L8
	mov	r4, r0
	ldr	r5, .L8+4
	mov	r0, r0, asl #6
	sub	r0, r0, r4, asl #3
	mov	r2, #56
	add	r0, r5, r0
	ldr	r3, [r3, #48]
	mov	r1, #0
	add	r5, r5, r4, lsl #2
	blx	r3
	ldr	r2, .L8+8
	mov	r1, #84
	mov	r3, #0
	str	r3, [r5, #56]
	mla	r4, r1, r4, r2
	str	r3, [r5, #60]
	add	r4, r4, #8
	str	r3, [r4, #4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L9:
	.align	2
.L8:
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
	ldr	r1, .L11
	mov	r3, r0, asl #6
	ldr	ip, .L11+4
	sub	r0, r3, r0, asl #3
	mov	r2, #56
	ldr	r3, [r1, #48]
	add	r0, ip, r0
	mov	r1, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L12:
	.align	2
.L11:
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
	ldr	r5, .L26
	mov	r4, #0
	sub	r7, r0, #4
	mov	lr, r4
	mov	r6, r4
.L20:
	mov	ip, #0
	mov	r1, r7
	mov	r3, ip
.L15:
	ldr	r2, [r1, #4]!
	cmp	r2, ip
	movgt	lr, r3
	add	r3, r3, #1
	movgt	ip, r2
	cmp	r3, #16
	bne	.L15
	cmp	ip, #0
	ble	.L25
	add	r4, r4, #1
	str	lr, [r5, #4]!
	cmp	r4, #16
	str	r6, [r0, lr, asl #2]
	bne	.L20
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L25:
	cmp	r4, #15
	ldmgtfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r0, .L26+4
	mov	r2, r4, asl #2
	mvn	r1, #0
.L18:
	add	r4, r4, #1
	add	r3, r0, r2
	cmp	r4, #16
	add	r2, r2, #4
	str	r1, [r3, #132]
	bne	.L18
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L27:
	.align	2
.L26:
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
	ldr	r5, .L41
	add	ip, r5, #132
	ldr	r3, [r5, #132]
	cmp	r3, r0
	beq	.L37
	mov	r3, #1
	mov	r2, ip
.L33:
	ldr	r1, [r2, #4]!
	cmp	r1, r0
	beq	.L39
	add	r3, r3, #1
	cmp	r3, #16
	bne	.L33
	mov	r3, #0
	mov	r6, #4
	mov	lr, r3
.L34:
	ldr	r0, [ip, r6]
	add	r6, ip, r6
	cmp	r0, #0
	ldmltfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r2, r3, #2
	mov	r2, r2, asl #2
	b	.L32
.L36:
	ldr	r0, [ip, r2]
	add	r2, r2, #4
	cmp	r0, #0
	ldmltfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L32:
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
	ble	.L36
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L39:
	cmp	r3, #14
	bgt	.L40
.L29:
	add	r6, r3, #1
	mov	lr, r3
	mov	r6, r6, asl #2
	b	.L34
.L37:
	mov	r3, #0
	b	.L29
.L40:
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L42:
	.align	2
.L41:
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
	ldr	r1, .L47
	add	ip, r1, r0, lsl #2
	ldr	r3, [ip, #56]
	cmp	r3, #0
	bne	.L46
	mov	r2, r0, asl #6
	sub	r0, r2, r0, asl #3
	ldr	r0, [r1, r0]
	cmp	r0, #0
	moveq	r3, #2
	movne	r0, r3
	movne	r3, #1
	str	r3, [ip, #56]
	ldmfd	sp, {fp, sp, pc}
.L46:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L48:
	.align	2
.L47:
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
	ldr	r3, .L50
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #56]
	ldmfd	sp, {fp, sp, pc}
.L51:
	.align	2
.L50:
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
	ldr	r3, .L57
	ldr	r5, .L57+4
	add	r4, r3, r0, lsl #2
	ldr	r2, [r4, #56]
	cmp	r2, #2
	beq	.L53
	mov	r2, r0, asl #6
	mov	r1, #0
	sub	r0, r2, r0, asl #3
	mov	r2, #56
	add	r0, r3, r0
	ldr	r3, [r5, #48]
	blx	r3
	mov	r3, #2
	str	r3, [r4, #56]
.L53:
	ldr	r3, [r5, #68]
	mov	r0, #0
	ldr	r1, .L57+8
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L58:
	.align	2
.L57:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L62
	mov	r4, r0
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L60
	blx	r3
.L61:
	ldr	r3, .L62+4
	mov	r2, #0
	add	r0, r3, r4, lsl #2
	str	r2, [r0, #56]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L60:
	ldr	ip, .L62+8
	mov	r0, r3
	movw	r2, #258
	ldr	r1, .L62+12
	ldr	r3, [ip, #68]
	blx	r3
	b	.L61
.L63:
	.align	2
.L62:
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC3
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
	ldr	r3, .L67
	mov	r5, r0
	mov	r4, r1
	ldr	r3, [r3, #84]
	cmp	r3, #0
	beq	.L65
	mov	r0, r1
	blx	r3
.L66:
	mov	r3, r4, asl #6
	ldr	r2, .L67+4
	sub	r1, r3, r4, asl #3
	str	r5, [r2, r1]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L65:
	ldr	ip, .L67+8
	mov	r0, r3
	movw	r2, #266
	ldr	r1, .L67+12
	ldr	r3, [ip, #68]
	blx	r3
	b	.L66
.L68:
	.align	2
.L67:
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC3
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
	ldr	r3, .L72
	mov	r4, r0
	ldr	r3, [r3, #88]
	cmp	r3, #0
	beq	.L70
	blx	r3
.L71:
	mov	r3, r4, asl #6
	ldr	r2, .L72+4
	sub	r0, r3, r4, asl #3
	mov	r3, #1
	str	r3, [r2, r0]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L70:
	ldr	ip, .L72+8
	mov	r0, r3
	movw	r2, #274
	ldr	r1, .L72+12
	ldr	r3, [ip, #68]
	blx	r3
	b	.L71
.L73:
	.align	2
.L72:
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC3
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
	ldr	r3, .L75
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #196]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L76:
	.align	2
.L75:
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
	ldr	r3, .L78
	add	r1, r1, #1
	add	r3, r3, r0, lsl #2
	str	r1, [r3, #196]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDMDRV_AdjustPriorityAfterDec
.L79:
	.align	2
.L78:
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
	ldr	r3, .L81
	add	r0, r0, #32
	mov	r2, #0
	add	r0, r3, r0, lsl #2
	str	r2, [r0, #68]
	ldmfd	sp, {fp, sp, pc}
.L82:
	.align	2
.L81:
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
	ldr	r3, .L90
	mov	r0, #1
	add	r1, r3, #64
.L85:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	strne	r0, [r2, #20]
	cmp	r3, r1
	bne	.L85
	ldmfd	sp, {fp, sp, pc}
.L91:
	.align	2
.L90:
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r0, #15
	mov	r4, r0
	bgt	.L93
	ldr	r6, .L119
	ldr	r2, [r6, r0, asl #2]
	cmp	r2, #0
	beq	.L93
	add	r3, r2, #204800
	ldr	r5, .L119+4
	mov	r8, r0, asl #2
	ldr	r1, [r3, #1520]
	ldr	r3, [r3, #1516]
	cmp	r1, #2160
	cmpcs	r3, #3840
	movcs	r7, #1
	movcc	r7, #0
	bcs	.L118
	add	r3, r5, r8
	ldr	r3, [r3, #324]
.L101:
	cmp	r3, #10
	beq	.L103
.L102:
	ldr	r3, [r2, #20]
	cmp	r3, #0
	beq	.L104
.L103:
	ldr	r3, [r2, #1448]
	cmp	r3, #35
	ble	.L105
	cmp	r7, #0
	beq	.L104
	ldr	ip, .L119+8
	mov	r1, #1
	movw	r3, #529
	str	r1, [r2, #1440]
	mov	r0, #0
	str	r1, [sp, #4]
	ldr	r2, .L119+12
	str	r4, [sp]
	ldr	r1, .L119+16
	ldr	ip, [ip, #68]
	blx	ip
	ldr	r2, [r6, r4, asl #2]
.L104:
	mov	r3, #0
	str	r3, [r2, #20]
	ldr	r3, [r6, r4, asl #2]
	add	r5, r5, r8
	ldr	r3, [r3, #1448]
	str	r3, [r5, #260]
.L93:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L118:
	add	r3, r5, r8
	ldr	r2, [r2, #1448]
	ldr	r9, .L119+8
	mov	r0, #22
	ldr	r1, [r3, #260]
	movw	r3, #501
	str	r2, [sp]
	cmp	r2, r1
	rsbhi	r10, r1, r2
	rsbls	r10, r2, r1
	str	r1, [sp, #4]
	ldr	r2, .L119+12
	ldr	ip, [r9, #68]
	ldr	r1, .L119+20
	blx	ip
	cmp	r10, #2
	bgt	.L98
	add	r2, r5, r8
	ldr	r3, [r2, #324]
	add	r3, r3, #1
	str	r3, [r2, #324]
.L99:
	cmn	r3, #1
	beq	.L100
	ldr	r2, [r6, r4, asl #2]
	b	.L101
.L105:
	mov	r3, #0
	str	r3, [r2, #1440]
	ldr	r2, [r6, r4, asl #2]
	b	.L104
.L98:
	add	ip, r5, r8
	ldr	r10, [r9, #68]
	movw	r3, #510
	ldr	r2, .L119+12
	mov	r9, ip
	ldr	r1, .L119+24
	mov	r0, #22
	mov	lr, #0
	str	lr, [ip, #324]
	blx	r10
	ldr	r3, [r9, #324]
	b	.L99
.L100:
	add	r3, r5, r8
	mov	r1, #11
	ldr	r2, [r6, r4, asl #2]
	str	r1, [r3, #324]
	b	.L102
.L120:
	.align	2
.L119:
	.word	s_pstVfmwChan
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1
	.word	.LC6
	.word	.LC4
	.word	.LC5
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
	bne	.L125
	ldr	ip, .L128
	add	ip, ip, r5, lsl #2
	ldr	r4, [ip, #196]
	cmp	r4, #0
	bne	.L126
	str	r4, [r6]
	mov	r0, r5
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	str	r0, [r6]
	beq	.L127
.L123:
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L125:
	ldr	ip, .L128+4
	mov	r3, r5
	ldr	r2, .L128+8
	mov	r4, #1
	ldr	r1, .L128+12
	mov	r0, #22
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L126:
	ldr	r3, .L128+4
	mov	r2, r5
	ldr	r1, .L128+16
	mov	r4, #1
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L127:
	ldr	r3, .L128+4
	mov	r2, r5
	ldr	r1, .L128+20
	mov	r0, #22
	mov	r4, #1
	ldr	r3, [r3, #68]
	blx	r3
	b	.L123
.L129:
	.align	2
.L128:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+24
	.word	.LC7
	.word	.LC8
	.word	.LC9
	UNWIND(.fnend)
	.size	VDMDRV_ChnDecReady, .-VDMDRV_ChnDecReady
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
	ldr	r3, .L131
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #388]
	ldmfd	sp, {fp, sp, pc}
.L132:
	.align	2
.L131:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_IsVdmInISR, .-VDMDRV_IsVdmInISR
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
	ldr	r2, .L134
	mov	r3, r0, asl #6
	sub	r0, r3, r0, asl #3
	ldr	r0, [r2, r0]
	adds	r0, r0, #0
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L135:
	.align	2
.L134:
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
	ldr	r3, .L137
	mov	r2, r0, asl #6
	sub	r0, r2, r0, asl #3
	add	r0, r3, r0
	ldr	r0, [r0, #8]
	ldmfd	sp, {fp, sp, pc}
.L138:
	.align	2
.L137:
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
	ldr	r6, .L148
	mov	r5, r0
	mov	r4, #0
	b	.L141
.L143:
	add	r4, r4, #1
	cmp	r4, #400
	beq	.L142
.L141:
	ldr	r3, [r6, #96]
	mov	r0, r5
	cmp	r3, #0
	beq	.L143
	blx	r3
	cmp	r0, #0
	beq	.L143
.L142:
	cmp	r4, #400
	movcc	r0, #0
	movcs	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L149:
	.align	2
.L148:
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	VDMDRV_WaitVdmReadyIfNoIsr, .-VDMDRV_WaitVdmReadyIfNoIsr
	.align	2
	.global	VDMDRV_H264NeedMoreStart
	.type	VDMDRV_H264NeedMoreStart, %function
VDMDRV_H264NeedMoreStart:
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
	bics	r3, r0, #16
	bne	.L152
	ldr	r0, [r1, #20]
	ldr	r3, [r1, #28]
	ldr	r2, [r1, #664]
	mul	r3, r3, r0
	cmp	r3, r2
	movle	r0, #0
	movgt	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L152:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VDMDRV_H264NeedMoreStart, .-VDMDRV_H264NeedMoreStart
	.align	2
	.global	VDMDRV_H264FinishedMultiStart
	.type	VDMDRV_H264FinishedMultiStart, %function
VDMDRV_H264FinishedMultiStart:
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
	bics	r0, r0, #16
	bne	.L155
	ldr	ip, [r1, #20]
	ldr	r3, [r1, #28]
	ldr	r2, [r1, #664]
	mul	r3, r3, ip
	cmp	r3, r2
	ldrlesb	r0, [r1, #2]
	clzle	r0, r0
	movle	r0, r0, lsr #5
	ldmfd	sp, {fp, sp, pc}
.L155:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VDMDRV_H264FinishedMultiStart, .-VDMDRV_H264FinishedMultiStart
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
	beq	.L161
.L160:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L161:
	ldr	r3, [r1, #32]
	cmp	r3, #1
	bne	.L160
	ldr	r3, .L162
	mov	r1, #1744
	mla	r2, r1, r2, r3
	ldr	r3, [r2, #1232]
	cmp	r3, #0
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L163:
	.align	2
.L162:
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
	beq	.L168
.L167:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L168:
	ldr	r3, [r1, #32]
	cmp	r3, #1
	bne	.L167
	ldr	r3, .L169
	mov	r1, #1744
	mla	r2, r1, r2, r3
	ldr	r3, [r2, #1712]
	cmp	r3, #0
	movgt	r0, #1
	movle	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L170:
	.align	2
.L169:
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
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	beq	.L175
	cmp	r0, #2
	ldreqb	r0, [r1, #158]	@ zero_extendqisi2
	subeq	r0, r0, #1
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L175:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDMDRV_IsMpeg4NvopCopy.part.3
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
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r1, #0
	beq	.L180
	cmp	r0, #1
	ldreqb	r0, [r1, #1]	@ zero_extendqisi2
	subeq	r0, r0, #1
	clzeq	r0, r0
	moveq	r0, r0, lsr #5
	movne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L180:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VDMDRV_IsVc1SkpicCopy.part.4
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
	beq	.L185
	sub	r0, r0, #10
	cmp	r0, #2
	ldrlsb	r0, [r4, #1]	@ zero_extendqisi2
	subls	r0, r0, #1
	clzls	r0, r0
	movls	r0, r0, lsr #5
	movhi	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L185:
	ldr	r3, .L186
	mov	r0, r4
	ldr	r1, .L186+4
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L187:
	.align	2
.L186:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC10
	UNWIND(.fnend)
	.size	VDMDRV_IsVp6NvopCopy, .-VDMDRV_IsVp6NvopCopy
	.align	2
	.global	VDMDRV_DoBeforeDec
	.type	VDMDRV_DoBeforeDec, %function
VDMDRV_DoBeforeDec:
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
	ldr	r3, [r2]
	mov	r10, r2
	mov	r5, r0
	cmp	r3, #0
	mov	r4, r1
	beq	.L226
	mov	r0, r1
	mov	r6, r5, asl #3
	bl	VCTRL_GetVidStd
	ldr	r2, .L232
	mov	r8, r5, asl #6
	ldr	r7, .L232+4
	rsb	r9, r6, r8
	ldr	r2, [r2]
	add	r9, r7, r9
	add	r9, r9, #8
	cmp	r2, #0
	str	r0, [r9, #4]
	mov	r3, r0
	beq	.L191
	ldr	r1, [r10]
	sub	r0, fp, #56
	str	r4, [fp, #-56]
	str	r3, [fp, #-52]
	str	r1, [fp, #-48]
	blx	r2
	cmp	r0, #1
	mov	ip, r0
	beq	.L190
	ldr	r3, [r9, #4]
.L191:
	ldr	r1, [r10]
	cmp	r1, #0
	beq	.L227
	cmp	r3, #2
	beq	.L228
.L196:
	cmp	r3, #1
	beq	.L229
.L197:
	mov	r0, r3
	bl	VDMDRV_IsVp6NvopCopy
	cmp	r0, #1
	beq	.L230
	ldr	r3, .L232+8
	ldr	r3, [r3, #92]
	cmp	r3, #0
	beq	.L205
	rsb	r9, r6, r8
	ldr	r1, [r10]
	add	r9, r7, r9
	mov	r2, r5
	ldr	r10, .L232+4
	ldr	r0, [r9, #12]
	blx	r3
	subs	ip, r0, #0
	beq	.L231
.L205:
	ldr	r10, .L232+12
	rsb	r6, r6, r8
	mov	r0, #0
	add	r8, r7, r6
	ldr	r1, .L232+16
	mov	r9, r0
	ldr	r3, [r10, #68]
	str	r0, [fp, #-64]
	blx	r3
	mov	lr, #100
	str	r5, [sp]
	mov	r1, lr
	mov	r0, r4
	mov	r3, r9
	mov	r2, #15
	str	r9, [r7, r6]
	add	r4, r4, #32
	str	lr, [r8, #4]
	bl	VCTRL_VdmPostProc
	mov	r0, r8
	ldr	r3, [r10, #48]
	mov	r1, r9
	mov	r2, #56
	add	r7, r7, r4, lsl #2
	blx	r3
	ldr	ip, [fp, #-64]
	str	r9, [r7, #68]
.L190:
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L228:
	ldrb	r2, [r1, #158]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L197
.L195:
	mov	r0, r1
	mov	r1, r5
	bl	PostPro_CopyYuv
	subs	ip, r0, #0
	beq	.L198
.L199:
	mov	ip, #0
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L229:
	ldrb	r2, [r1, #1]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L197
.L202:
	ldr	r9, .L232+12
	mov	r0, #22
	ldr	r1, .L232+20
	ldr	r3, [r9, #68]
	blx	r3
	mov	r1, r5
	ldr	r0, [r10]
	bl	PostPro_CopyYuv_Vc1
	subs	ip, r0, #0
	bne	.L199
	rsb	r6, r6, r8
	mov	r1, r5
	add	r6, r7, r6
	mov	r0, #2
	add	r7, r7, r4, lsl #2
	str	ip, [fp, #-64]
	str	ip, [r6, #4]
	bl	VDMDRV_StartRepair
	mov	r0, r4
	add	r3, r5, #1
	str	r3, [r7, #196]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	ldr	r8, .L232+4
	cmp	r0, #0
	beq	.L199
	ldr	r3, [r9]
	add	r8, r8, r4, lsl #2
	blx	r3
	ldr	r2, .L232+24
	ldr	r3, [r8, #2136]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	addhi	r2, r5, r4, lsl #24
	movwhi	r1, #655
	bls	.L199
.L224:
	add	r2, r2, #458752
	ldr	r0, .L232+28
	str	lr, [r8, #2136]
	bl	DBG_AddTrace
	ldr	ip, [fp, #-64]
	b	.L190
.L231:
	ldr	r6, .L232+12
	str	ip, [fp, #-64]
	ldr	r3, [r6]
	blx	r3
	ldr	r1, .L232+32
	mov	r3, #84
	mla	r3, r3, r5, r1
	mov	r2, r0
	mov	r0, r5
	str	r2, [r3, #4]
	str	r2, [r9, #20]
	bl	VDMDRV_StartDec
	add	r3, r10, r4, lsl #2
	mov	r0, r4
	add	r2, r5, #1
	str	r2, [r3, #196]
	bl	VDMDRV_AdjustPriorityAfterDec
	ldr	ip, [fp, #-64]
	mov	r1, #5
	mov	r0, r4
	mov	r2, ip
	bl	DBG_CountTimeInfo
	mov	r0, #5
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L199
	ldr	r3, [r6]
	add	r10, r10, r4, lsl #2
	blx	r3
	ldr	r2, .L232+24
	ldr	r3, [r10, #2136]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	bls	.L199
	add	r2, r5, r4, lsl #24
	movw	r1, #722
	add	r2, r2, #327680
	ldr	r0, .L232+28
	str	lr, [r10, #2136]
	bl	DBG_AddTrace
	ldr	ip, [fp, #-64]
	b	.L190
.L226:
	ldr	r3, .L232+12
	mov	r2, r1
	mov	r0, #22
	ldr	r1, .L232+36
	mov	ip, #1
	str	ip, [fp, #-64]
	ldr	r3, [r3, #68]
	blx	r3
	ldr	ip, [fp, #-64]
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L230:
	ldr	r9, .L232+12
	mov	r0, #22
	ldr	r1, .L232+20
	ldr	r3, [r9, #68]
	blx	r3
	mov	r1, r5
	ldr	r0, [r10]
	bl	PostPro_CopyYuv_Vp6
	subs	ip, r0, #0
	bne	.L199
	rsb	r6, r6, r8
	mov	r1, r5
	add	r6, r7, r6
	mov	r0, #2
	add	r7, r7, r4, lsl #2
	str	ip, [fp, #-64]
	str	ip, [r6, #4]
	bl	VDMDRV_StartRepair
	mov	r0, r4
	add	r3, r5, #1
	str	r3, [r7, #196]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	ldr	r8, .L232+4
	cmp	r0, #0
	beq	.L199
	ldr	r3, [r9]
	add	r8, r8, r4, lsl #2
	blx	r3
	ldr	r2, .L232+24
	ldr	r3, [r8, #2136]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	bls	.L199
	add	r2, r5, r4, lsl #24
	movw	r1, #682
	b	.L224
.L227:
	bl	VDMDRV_IsMpeg4NvopCopy.part.3
	cmp	r0, #1
	ldreq	r1, [r10]
	beq	.L195
.L194:
	rsb	r9, r6, r8
	ldr	r1, [r10]
	add	r9, r7, r9
	add	r9, r9, #8
	cmp	r1, #0
	ldr	r3, [r9, #4]
	bne	.L196
	bl	VDMDRV_IsVc1SkpicCopy.part.4
	cmp	r0, #1
	beq	.L202
	ldr	r3, [r9, #4]
	ldr	r1, [r10]
	b	.L197
.L198:
	rsb	r6, r6, r8
	mov	r1, r5
	add	r6, r7, r6
	mov	r0, #2
	add	r7, r7, r4, lsl #2
	str	ip, [fp, #-64]
	str	ip, [r6, #4]
	bl	VDMDRV_StartRepair
	mov	r0, r4
	add	r3, r5, #1
	str	r3, [r7, #196]
	bl	VDMDRV_AdjustPriorityAfterDec
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	ldr	r8, .L232+4
	cmp	r0, #0
	beq	.L199
	ldr	r3, .L232+12
	add	r8, r8, r4, lsl #2
	ldr	r3, [r3]
	blx	r3
	ldr	r2, .L232+24
	ldr	r3, [r8, #2136]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	lr, r0
	cmp	r3, r2
	addhi	r2, r5, r4, lsl #24
	movhi	r1, #628
	bls	.L199
	b	.L224
.L233:
	.align	2
.L232:
	.word	AcceleratorCharacter
	.word	.LANCHOR0
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC13
	.word	.LC12
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+44
	.word	g_VfmwGlobalStat
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
	ldr	r9, .L240
	mov	r5, r0, asl #6
	ldr	r6, .L240+4
	sub	r5, r5, r0, asl #3
	mov	r8, r0
	mov	r0, #5
	ldr	r3, [r9, #12]
	add	r7, r5, #16
	blx	r3
	ldr	r3, [r6, #132]
	add	r7, r6, r7
	cmp	r3, #0
	blt	.L235
	add	r5, r6, r5
	add	r4, r6, #132
	add	r6, r6, #192
	b	.L236
.L239:
	cmp	r4, r6
	beq	.L235
	ldr	r3, [r4, #4]!
	cmp	r3, #0
	blt	.L235
.L236:
	mov	r0, r3
	mov	r1, r7
	str	r3, [r5, #8]
	bl	VDMDRV_ChnDecReady
	cmp	r0, #0
	bne	.L239
	mov	r2, r7
	ldr	r1, [r5, #8]
	mov	r0, r8
	bl	VDMDRV_DoBeforeDec
	cmp	r0, #0
	bne	.L239
.L235:
	ldr	r3, [r9, #16]
	mov	r0, #5
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L241:
	.align	2
.L240:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VDMDRV_ActivateVDH, .-VDMDRV_ActivateVDH
	.align	2
	.global	VDMDRV_AfterDec
	.type	VDMDRV_AfterDec, %function
VDMDRV_AfterDec:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 168
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #180)
	sub	sp, sp, #180
	ldr	r4, .L341
	mov	r10, r0, asl #3
	mov	r9, r0, asl #6
	rsb	r3, r10, r9
	ldr	r2, .L341+4
	add	r3, r4, r3
	ldr	r8, .L341+8
	mov	r5, r0
	mov	r1, #0
	ldr	r3, [r3, #8]
	sub	r0, fp, #180
	rsb	r7, r10, r9
	add	r6, r4, r7
	ldr	r3, [r2, r3, asl #2]
	mov	r2, #136
	cmp	r3, #0
	str	r3, [fp, #-212]
	ldrne	r3, [r3, #1232]
	ldreq	r3, [fp, #-212]
	str	r3, [fp, #-208]
	ldr	r3, [r8, #48]
	blx	r3
	mov	r3, #180
	mla	r3, r3, r5, r4
	ldr	lr, [r6, #4]
	add	ip, r6, #32
	ldr	r1, .L341+12
	str	ip, [fp, #-200]
	mov	r0, #2
	ldr	r2, [r3, #2200]
	and	r2, r2, #15
	cmp	r2, #5
	ldr	r2, [r4, r7]
	ldreq	r3, [r3, #2212]
	ldrne	r3, [r3, #2208]
	andeq	r3, r3, #1
	andne	r3, r3, #31
	str	r3, [fp, #-204]
	ldr	r3, [r6, #36]
	str	lr, [sp]
	ldr	ip, [r8, #68]
	blx	ip
	ldr	r7, [r4, r7]
	ldr	ip, [fp, #-200]
	cmp	r7, #1
	beq	.L323
	cmp	r7, #2
	beq	.L324
	cmp	r7, #3
	beq	.L325
	ldr	r3, .L341+8
	mov	r2, r7
	ldr	r1, .L341+16
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
.L242:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L325:
	ldr	r3, [ip, #4]
	cmp	r3, #1
	moveq	r3, #0
	streq	r3, [ip, #4]
	rsb	ip, r10, r9
.L318:
	add	r7, r4, ip
	str	r5, [sp]
	ldr	r2, [fp, #-204]
	sub	r3, fp, #180
	ldr	r1, [r7, #4]
	mov	r6, #0
	ldr	r0, [r7, #8]
	cmp	r1, #100
	str	r6, [r4, ip]
	movge	r1, #100
	bic	r1, r1, r1, asr #31
	str	r1, [r7, #4]
.L320:
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
.L323:
	ldr	r3, [ip, #4]
	cmp	r3, #1
	beq	.L326
	ldr	r3, .L341+8
	mov	r1, #0
	mov	r2, #12
	sub	r0, fp, #192
	add	ip, r6, #8
	str	ip, [fp, #-200]
	ldr	r3, [r3, #48]
	blx	r3
	ldr	ip, [fp, #-200]
	ldr	r3, [r6, #16]
	sub	r0, fp, #192
	str	r5, [fp, #-184]
	ldr	r2, [ip, #4]
	str	r3, [fp, #-188]
	str	r2, [fp, #-192]
	bl	PostPro_GetErrRatio
	ldr	r3, .L341+8
	ldr	r1, .L341+20
	ldr	r3, [r3, #68]
	mov	r2, r0
	mov	r0, r7
	str	r2, [r6, #4]
	blx	r3
	ldr	ip, [fp, #-200]
	ldr	r3, [ip, #4]
	sub	r2, r3, #8
	cmp	r3, #5
	cmpne	r2, #1
	movls	r3, #0
	strls	r3, [r6, #4]
	bls	.L248
	ldr	r1, [r6, #4]
	cmp	r1, #0
	beq	.L248
	ldr	r2, .L341+24
	ldr	r0, [fp, #-212]
	ldr	ip, [r2]
	ldr	r3, [r0, #1144]
	mov	r2, r0
	cmp	ip, #0
	add	r3, r3, #1
	str	r3, [r0, #1144]
	beq	.L248
	sub	r2, fp, #44
	ldr	r0, [r6, #8]
	mov	r3, #4
	str	r1, [r2, #-152]!
	mov	r1, #114
	blx	ip
.L248:
	ldr	r3, [fp, #-208]
	rsb	r6, r10, r9
	add	r6, r4, r6
	cmp	r3, #1
	rsbeq	r3, r10, r9
	ldr	r1, [r6, #16]
	addeq	r3, r4, r3
	moveq	r2, #0
	ldr	r0, [r6, #12]
	streq	r2, [r3, #4]
	bl	PostPro_GetRepairStratage
	ldr	r3, [r6, #12]
	ldr	r1, [r6, #16]
	bics	r3, r3, #16
	str	r0, [fp, #-200]
	beq	.L327
	mov	r3, #0
	str	r3, [r6, #28]
	mov	r2, r3
	mov	r1, r3
.L254:
	rsb	r7, r10, r9
	mov	r3, r1
	add	r6, r4, r7
	ldr	ip, [r8, #68]
	mov	r0, #2
	str	r1, [r6, #32]
	ldr	r1, .L341+28
	blx	ip
	ldr	r3, [r6, #28]
	ldr	ip, .L341+8
	cmp	r3, #1
	beq	.L328
	ldr	r3, [r6, #32]
	cmp	r3, #1
	beq	.L329
	ldr	r1, [r6, #4]
	ldr	r3, [fp, #-200]
	cmp	r1, #0
	cmpne	r3, #1
	moveq	r2, #1
	movne	r2, #0
	beq	.L330
	add	r3, r6, #8
	ldr	r1, [r3, #4]
	add	r7, r6, #16
	ldr	r0, [r6, #16]
	cmp	r1, #6
	beq	.L331
.L263:
	ldr	r3, [r8]
	rsb	r6, r10, r9
	blx	r3
	ldr	r3, .L341+32
	add	r6, r4, r6
	ldr	r7, .L341+8
	add	r8, r6, #16
	ldr	ip, [r3, #104]
	cmp	ip, #0
	str	r0, [r8, #4]
	beq	.L275
	mov	r3, r5
	mov	r2, #0
	ldr	r1, [r6, #16]
	ldr	r0, [r6, #12]
	blx	ip
	cmp	r0, #0
	beq	.L332
.L275:
	rsb	r6, r10, r9
	str	r5, [sp]
	add	r5, r4, r6
	sub	r3, fp, #180
	ldr	r2, [fp, #-204]
	mov	r1, #100
	ldr	r0, [r5, #8]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r5, #8]
	mov	r2, #0
	str	r2, [r4, r6]
	add	r3, r3, #32
	add	r4, r4, r3, lsl #2
	str	r2, [r4, #68]
	b	.L242
.L324:
	ldr	r3, [ip, #4]
	rsb	r6, r10, r9
	add	r6, r4, r6
	ldr	r7, .L341
	cmp	r3, #1
	ldr	r1, [r6, #16]
	moveq	r3, #0
	streq	r3, [ip, #4]
	add	ip, r6, #8
	add	r3, r6, #16
	str	r3, [fp, #-200]
	ldr	r3, [ip, #4]
	cmp	r3, #6
	beq	.L333
.L273:
	cmp	r1, #0
	beq	.L334
	cmp	r3, #2
	beq	.L335
	rsb	r3, r10, r9
	add	r3, r4, r3
	ldr	r0, [r3, #12]
	ldr	r1, [r3, #16]
.L280:
	cmp	r0, #1
	beq	.L336
.L282:
	bl	VDMDRV_IsVp6NvopCopy
	rsb	ip, r10, r9
	cmp	r0, #1
	bne	.L318
.L321:
	add	r7, r4, ip
	mov	r6, #0
	str	r5, [sp]
	sub	r3, fp, #180
	ldr	r2, [fp, #-204]
	mov	r1, r6
	ldr	r0, [r7, #8]
	str	r6, [r4, ip]
	str	r6, [r7, #4]
	b	.L320
.L327:
	ldr	r0, [r1, #20]
	ldr	r2, [r1, #28]
	ldr	lr, [r1, #664]
	mul	r2, r2, r0
	cmp	r2, lr
	movle	r2, #0
	movgt	r2, #1
	str	r2, [r6, #28]
	ldr	ip, [r1, #20]
	ldr	r0, [r1, #28]
	mul	r0, r0, ip
	cmp	lr, r0
	ldrgesb	r3, [r1, #2]
	movlt	r1, r3
	clzge	r1, r3
	movge	r1, r1, lsr #5
	b	.L254
.L333:
	ldr	r2, [r1, #32]
	cmp	r2, #1
	bne	.L273
	mov	r0, #1744
	mla	r0, r0, r5, r7
	ldr	r0, [r0, #2104]
	cmp	r0, #0
	ble	.L273
	ldr	r3, [r8]
	str	r2, [fp, #-212]
	str	ip, [fp, #-208]
	blx	r3
	ldr	r3, .L341+32
	ldr	ip, [fp, #-208]
	ldr	r2, [fp, #-212]
	ldr	r8, [r3, #104]
	ldr	r3, [fp, #-200]
	cmp	r8, #0
	str	r0, [r3, #4]
	beq	.L275
	ldr	r0, [ip, #4]
	mov	r3, r5
	ldr	r1, [r6, #16]
	blx	r8
	cmp	r0, #0
	bne	.L275
	ldr	ip, .L341+36
	mov	r3, #84
	ldr	r2, [fp, #-200]
	mov	r1, r5
	mov	r0, #3
	mla	r3, r3, r5, ip
	ldr	r2, [r2, #4]
	str	r2, [r3, #4]
	bl	VDMDRV_StartRepair
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L242
	ldr	r3, .L341+8
	ldr	r3, [r3]
	blx	r3
	ldr	r2, [r6, #8]
	ldr	r3, .L341+40
	add	r7, r7, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r7, #2380]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L242
	add	r2, r5, r2, lsl #24
	movw	r1, #1326
	add	r2, r2, #458752
	ldr	r0, .L341+44
	str	ip, [r7, #2380]
	bl	DBG_AddTrace
	b	.L242
.L330:
	ldr	r3, [fp, #-208]
	cmp	r3, #1
	beq	.L337
	cmp	r1, #100
	str	ip, [fp, #-200]
	ldr	ip, .L341
	sub	r3, fp, #180
	movge	r1, #100
	ldr	r0, [r6, #8]
	bic	r1, r1, r1, asr #31
	str	r5, [sp]
	mov	r4, #0
	str	r1, [r6, #4]
	str	r4, [ip, r7]
	ldr	r2, [fp, #-204]
	bl	VCTRL_VdmPostProc
	ldr	ip, [fp, #-200]
	ldr	r3, [r6, #8]
	mov	r1, r4
	mov	r0, r6
	ldr	ip, [ip, #48]
	add	r3, r3, #32
.L319:
	ldr	lr, .L341
	mov	r2, #56
	add	r9, lr, r3, lsl #2
	str	r4, [r9, #68]
	blx	ip
	mov	r0, r5
	bl	VDMDRV_ActivateVDH
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L326:
	mov	r2, r5
	ldr	r1, [r6, #16]
	ldr	r0, [r6, #12]
	bl	PostPro_SetFullRepair
	ldr	ip, [fp, #-200]
	ldr	r2, .L341+8
	mov	r3, #0
	mov	r0, r3
	ldr	r1, .L341+48
	str	r3, [ip, #4]
	mov	r3, #100
	ldr	r2, [r2, #68]
	str	r3, [r6, #4]
	blx	r2
	b	.L248
.L332:
	ldr	ip, .L341+36
	mov	r3, #84
	ldr	r2, [r8, #4]
	mov	r1, r5
	mov	r0, #2
	mla	r3, r3, r5, ip
	str	r2, [r3, #4]
	bl	VDMDRV_StartRepair
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L242
	ldr	r3, [r7]
	blx	r3
	ldr	r2, [r6, #8]
	ldr	r1, .L341
	ldr	r3, .L341+40
	add	r9, r1, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r9, #2380]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L242
	add	r2, r5, r2, lsl #24
	movw	r1, #1285
	add	r2, r2, #458752
	ldr	r0, .L341+44
	str	ip, [r9, #2380]
	bl	DBG_AddTrace
	b	.L242
.L335:
	ldrb	r3, [r1, #158]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L283
	rsb	r3, r10, r9
	add	r3, r4, r3
	ldr	r0, [r3, #12]
	ldr	r1, [r3, #16]
	b	.L282
.L337:
	cmp	r1, #100
	ldr	r3, [r6, #52]
	movge	r1, #100
	cmp	r3, #1
	bic	r1, r1, r1, asr #31
	str	r1, [r6, #4]
	beq	.L338
	cmp	r3, #2
	beq	.L339
	cmp	r3, #3
	bne	.L242
	ldr	r0, [r6, #8]
	sub	r3, fp, #180
	ldr	r2, [fp, #-204]
	str	r5, [sp]
	str	ip, [fp, #-200]
	bl	VCTRL_VdmPostProc
	ldr	r2, [r6, #8]
	mov	r3, #0
	ldr	ip, [fp, #-200]
	mov	r0, r6
	ldr	r1, .L341+4
	add	lr, r2, #32
	ldr	r4, [ip, #48]
	ldr	ip, [r1, r2, asl #2]
	mov	r1, r3
	ldr	r2, .L341
	add	ip, ip, #229376
	add	lr, r2, lr, lsl #2
	str	r3, [r2, r7]
	mov	r2, #56
	str	r3, [lr, #68]
	str	r3, [ip, #1120]
	str	r3, [ip, #3072]
	blx	r4
	mov	r0, r5
	bl	VDMDRV_ActivateVDH
	b	.L242
.L336:
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L282
.L283:
	rsb	ip, r10, r9
	b	.L321
.L328:
	ldr	ip, .L341
	mov	r4, #0
	ldr	r0, [r6, #8]
	sub	r3, fp, #180
	str	r5, [sp]
	mov	r1, r4
	ldr	r2, [fp, #-204]
	str	r4, [ip, r7]
	str	r4, [r6, #4]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r6, #8]
	ldr	r2, .L341
	add	r3, r3, #32
	add	r9, r2, r3, lsl #2
	str	r4, [r9, #68]
	b	.L242
.L329:
	str	ip, [fp, #-200]
	mov	r4, #0
	ldr	ip, .L341
	sub	r3, fp, #180
	ldr	r0, [r6, #8]
	mov	r1, r4
	str	r5, [sp]
	ldr	r2, [fp, #-204]
	str	r4, [ip, r7]
	str	r4, [r6, #4]
	bl	VCTRL_VdmPostProc
	ldr	ip, [fp, #-200]
	ldr	r3, [r6, #8]
	mov	r1, r4
	mov	r0, r6
	ldr	ip, [ip, #48]
	add	r3, r3, #32
	b	.L319
.L331:
	ldr	r1, [r0, #32]
	cmp	r1, #1
	beq	.L340
.L264:
	ldr	r2, [r0, #32]
	cmp	r2, #1
	bne	.L263
	ldr	r6, .L341
	mov	r3, #1744
	mla	r3, r3, r5, r6
	ldr	r3, [r3, #2104]
	cmp	r3, #0
	ble	.L263
	ldr	r3, [r8]
	rsb	r7, r10, r9
	str	r2, [fp, #-200]
	blx	r3
	ldr	r3, .L341+32
	add	r7, r6, r7
	add	r8, r7, #16
	ldr	r2, [fp, #-200]
	ldr	ip, [r3, #104]
	cmp	ip, #0
	str	r0, [r8, #4]
	beq	.L275
	mov	r3, r5
	ldr	r1, [r7, #16]
	ldr	r0, [r7, #12]
	blx	ip
	cmp	r0, #0
	bne	.L275
	ldr	ip, .L341+36
	mov	r3, #84
	ldr	r2, [r8, #4]
	mov	r1, r5
	mov	r0, #3
	mla	r3, r3, r5, ip
	str	r2, [r3, #4]
	bl	VDMDRV_StartRepair
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L242
	ldr	r3, .L341+8
	ldr	r3, [r3]
	blx	r3
	ldr	r2, [r7, #8]
	ldr	r3, .L341+40
	add	r6, r6, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r6, #2380]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L242
	add	r2, r5, r2, lsl #24
	movw	r1, #1252
	add	r2, r2, #458752
	ldr	r0, .L341+44
	str	ip, [r6, #2380]
	bl	DBG_AddTrace
	b	.L242
.L334:
	bl	VDMDRV_IsMpeg4NvopCopy.part.3
	cmp	r0, #1
	beq	.L283
	rsb	r6, r10, r9
	add	r6, r4, r6
	add	r7, r6, #8
	ldr	r1, [r6, #16]
	ldr	r0, [r7, #4]
	cmp	r1, #0
	bne	.L280
	bl	VDMDRV_IsVc1SkpicCopy.part.4
	cmp	r0, #1
	beq	.L283
	ldr	r0, [r7, #4]
	ldr	r1, [r6, #16]
	b	.L282
.L340:
	ldr	lr, .L341
	mov	r1, #1744
	mla	r1, r1, r5, lr
	ldr	r1, [r1, #1232]
	cmp	r1, #0
	ble	.L264
	ldr	r1, [ip]
	str	r3, [fp, #-212]
	str	r2, [fp, #-208]
	str	ip, [fp, #-200]
	blx	r1
	ldr	r1, .L341+32
	ldr	ip, [fp, #-200]
	ldr	r2, [fp, #-208]
	ldr	r8, [r1, #104]
	ldr	r3, [fp, #-212]
	cmp	r8, #0
	str	r0, [r7, #4]
	beq	.L275
	ldr	r0, [r3, #4]
	mov	r3, r5
	ldr	r1, [r6, #16]
	str	ip, [fp, #-200]
	blx	r8
	cmp	r0, #0
	bne	.L275
	ldr	lr, .L341+36
	mov	r3, #84
	ldr	r2, [r7, #4]
	mov	r1, r5
	mov	r0, #2
	mla	r3, r3, r5, lr
	str	r2, [r3, #4]
	bl	VDMDRV_StartRepair
	mov	r0, #7
	bl	CHECK_REC_POS_ENABLE
	ldr	ip, [fp, #-200]
	cmp	r0, #0
	beq	.L242
	ldr	r3, [ip]
	blx	r3
	ldr	r2, [r6, #8]
	ldr	r1, .L341+40
	mov	r3, r2, asl #2
	mov	ip, r0
	ldr	r0, .L341
	add	r9, r0, r3
	ldr	r0, [r1]
	ldr	r1, [r9, #2380]
	rsb	r1, r1, ip
	cmp	r1, r0
	bls	.L242
	add	r3, r4, r3
	add	r2, r5, r2, lsl #24
	add	r2, r2, #458752
	movw	r1, #1218
	ldr	r0, .L341+44
	str	ip, [r3, #2380]
	bl	DBG_AddTrace
	b	.L242
.L338:
	ldr	r0, [r6, #8]
	sub	r3, fp, #180
	ldr	r2, [fp, #-204]
	str	r5, [sp]
	str	ip, [fp, #-200]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r6, #8]
	mov	lr, #0
	ldr	r1, .L341+4
	mov	r0, r6
	add	r2, r3, #32
	ldr	r4, .L341
	ldr	ip, [fp, #-200]
	ldr	r3, [r1, r3, asl #2]
	add	r9, r4, r2, lsl #2
	mov	r1, lr
	mov	r2, #56
	add	r3, r3, #229376
	ldr	ip, [ip, #48]
	str	lr, [r9, #68]
	str	lr, [r3, #1120]
	str	lr, [r3, #3072]
	blx	ip
	mov	r0, r5
	bl	VDMDRV_ActivateVDH
	b	.L242
.L339:
	ldr	r0, [r6, #8]
	sub	r3, fp, #180
	str	r5, [sp]
	ldr	r2, [fp, #-204]
	bl	VCTRL_VdmPostProc
	b	.L242
.L342:
	.align	2
.L341:
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC14
	.word	.LC18
	.word	.LC16
	.word	g_event_report
	.word	.LC17
	.word	g_vdm_hal_fun_ptr
	.word	g_VfmwGlobalStat
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+64
	.word	.LC15
	UNWIND(.fnend)
	.size	VDMDRV_AfterDec, .-VDMDRV_AfterDec
	.align	2
	.global	VDMDRV_CheckDevice
	.type	VDMDRV_CheckDevice, %function
VDMDRV_CheckDevice:
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
	ldr	r4, .L386
	mov	r6, r0, asl #3
	mov	r8, r0, asl #6
	rsb	r5, r6, r8
	mov	r7, r0
	add	r9, r4, r5
	ldr	r0, [r9, #8]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L381
.L344:
	rsb	r3, r6, r8
	add	r5, r4, r3
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bne	.L382
.L345:
	rsb	r10, r6, r8
	ldr	r3, .L386
	add	r10, r4, r10
	ldr	r5, [r10, #28]
	cmp	r5, #1
	beq	.L383
	add	r7, r3, r7, lsl #2
	ldr	r5, [r7, #56]
	cmp	r5, #0
	beq	.L379
	cmp	r5, #1
	moveq	r3, #2
	streq	r3, [r7, #56]
	beq	.L379
.L350:
	mov	r5, #1
.L379:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L382:
	ldr	r9, .L386+4
	ldr	r3, [r9]
	blx	r3
	add	r1, r5, #16
	ldr	r2, [r1, #4]
	str	r0, [r5, #24]
	cmp	r0, r2
	bcc	.L348
	clz	r10, r2
	rsb	r2, r2, r0
	mov	r10, r10, lsr #5
	orrs	r10, r10, r2, lsr #31
	bne	.L348
	cmp	r2, #500
	bls	.L350
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L386+8
	blx	r3
	ldr	r3, .L386+12
	ldr	ip, [r3]
	cmp	ip, #0
	beq	.L353
	ldr	r0, [r5, #8]
	mov	r3, r10
	mov	r2, r10
	mov	r1, #101
	blx	ip
.L353:
	ldr	r3, .L386+16
	ldr	r3, [r3, #56]
	cmp	r3, #0
	beq	.L354
	mov	r0, r7
	blx	r3
.L355:
	rsb	r6, r6, r8
	mov	r5, #1
	add	r4, r4, r6
	ldr	r3, .L386
	ldr	r2, [r4, #44]
	str	r5, [r4, #36]
	cmp	r2, r5
	bne	.L356
	ldr	ip, [r4, #8]
	mov	r2, #0
	str	r2, [r3, r6]
	mov	r5, r2
	add	r0, ip, #32
	str	r2, [r4, #44]
	str	r2, [r4, #36]
	add	r3, r3, r0, lsl #2
	str	r2, [r3, #68]
	b	.L379
.L348:
	str	r0, [r1, #4]
	mov	r5, #1
	b	.L379
.L383:
	ldr	r3, [r10, #40]
	ldr	r9, .L386+4
	cmp	r3, #0
	beq	.L384
.L358:
	rsb	r3, r6, r8
	mov	r1, #0
	add	r10, r4, r3
	str	r1, [fp, #-52]
	add	r3, r10, #16
	str	r3, [fp, #-56]
	ldr	r0, [r10, #8]
	add	r3, r10, #8
	str	r1, [r10, #16]
	str	r3, [fp, #-48]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	str	r0, [r10, #16]
	beq	.L359
	ldr	r1, [fp, #-52]
	ldr	r0, [r10, #8]
	str	r1, [r10, #40]
	bl	VCTRL_GetVidStd
	ldr	r2, .L386+16
	ldr	r3, [fp, #-48]
	ldr	ip, [r2, #92]
	cmp	ip, #0
	str	r0, [r3, #4]
	beq	.L361
	mov	r2, r7
	ldr	r1, [r10, #16]
	blx	ip
	cmp	r0, #0
	beq	.L385
.L361:
	rsb	r6, r6, r8
	mov	r0, #0
	add	r10, r4, r6
	ldr	r3, [r9, #68]
	ldr	r1, .L386+20
	mov	r9, r0
	blx	r3
	str	r7, [sp]
	mov	r3, r9
	ldr	r0, [r10, #8]
	str	r9, [r4, r6]
	mov	r1, r9
	mov	r2, #15
	str	r9, [r10, #4]
	bl	VCTRL_VdmPostProc
	ldr	r2, [r10, #8]
	mov	r0, r5
	add	r2, r2, #32
	add	r4, r4, r2, lsl #2
	str	r9, [r4, #68]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L381:
	ldr	r2, [r4, r5]
	cmp	r2, #0
	beq	.L345
	ldr	r3, .L386+16
	ldr	r3, [r3, #100]
	cmp	r3, #0
	beq	.L346
	mov	r0, r7
	blx	r3
	cmp	r0, #0
	movne	r3, #1
	ldreq	r2, [r4, r5]
	strne	r3, [r9, #44]
	bne	.L344
.L346:
	rsb	r10, r6, r8
	ldr	r9, .L386+4
	add	r10, r4, r10
	mov	r5, #0
	mov	r1, #1440
	ldr	r0, .L386+24
	str	r5, [r10, #44]
	bl	DBG_AddTrace
	ldr	r3, [r9, #48]
	mov	r0, r10
	mov	r1, r5
	mov	r2, #56
	blx	r3
	ldr	r3, [r10, #8]
	add	r3, r3, #32
	add	r3, r4, r3, lsl #2
	str	r5, [r3, #68]
	b	.L344
.L356:
	mov	r0, r7
	bl	VDMDRV_AfterDec
	b	.L379
.L384:
	ldr	r3, [r9]
	blx	r3
	str	r0, [r10, #40]
	b	.L358
.L385:
	ldr	r2, [r9]
	blx	r2
	ldr	ip, .L386+28
	mov	r2, #84
	ldr	r3, [fp, #-56]
	mla	r2, r2, r7, ip
	mov	r1, r0
	mov	r0, r7
	str	r1, [r3, #4]
	add	r7, r7, #1
	str	r1, [r2, #4]
	bl	VDMDRV_StartDec
	ldr	r0, [r10, #8]
	ldr	r3, .L386
	add	r3, r3, r0, lsl #2
	str	r7, [r3, #196]
	bl	VDMDRV_AdjustPriorityAfterDec
	b	.L379
.L359:
	ldr	r2, [r9, #68]
	ldr	r1, .L386+32
	blx	r2
	ldr	r2, [r9]
	blx	r2
	ldr	r3, [r10, #40]
	cmp	r0, r3
	rsbcc	r3, r0, r3
	rsbcs	r3, r3, r0
	mvncc	r3, r3
	cmp	r3, #200
	bls	.L350
	rsb	r6, r6, r8
	ldr	r3, [r9, #68]
	add	r8, r4, r6
	mov	r0, #0
	ldr	r1, .L386+36
	mov	r9, r0
	blx	r3
	mov	ip, #100
	str	r7, [sp]
	mov	r3, r9
	str	r9, [r4, r6]
	mov	r1, ip
	ldr	r0, [r8, #8]
	mov	r2, #15
	str	r9, [r8, #40]
	str	ip, [r8, #4]
	bl	VCTRL_VdmPostProc
	ldr	r3, [r8, #8]
	add	r3, r3, #32
	add	r4, r4, r3, lsl #2
	str	r9, [r4, #68]
	b	.L379
.L354:
	ldr	ip, [r9, #68]
	mov	r0, r3
	movw	r2, #1483
	ldr	r1, .L386+40
	blx	ip
	b	.L355
.L387:
	.align	2
.L386:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC19
	.word	g_event_report
	.word	g_vdm_hal_fun_ptr
	.word	.LC20
	.word	.LANCHOR1+80
	.word	g_VfmwGlobalStat
	.word	.LC21
	.word	.LC22
	.word	.LC3
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
	bgt	.L439
	mov	r3, r4, asl #3
	mov	r6, r4, asl #6
	ldr	r5, .L450
	mov	r0, #0
	str	r3, [fp, #-64]
	rsb	r3, r3, r6
	add	r3, r5, r3
	ldr	r2, .L450+4
	ldr	r9, .L450+8
	ldr	r3, [r3, #8]
	ldr	r8, .L450+12
	ldr	r7, [r2, r3, asl #2]
	ldr	r3, [r9, #12]
	cmp	r7, #0
	ldrne	r7, [r7, #1232]
	blx	r3
	ldr	r3, [r8, #80]
	cmp	r3, #0
	beq	.L424
	mov	r1, r4
	mov	r0, #2
	blx	r3
	ldr	r3, [r8, #80]
	cmp	r3, #0
	mov	r10, r0
	moveq	r0, r3
	beq	.L394
	mov	r1, r4
	mov	r0, #1
	blx	r3
	ldr	r2, [r8, #80]
	cmp	r2, #0
	mov	r3, r0
	moveq	r0, r2
	beq	.L394
	mov	r1, r4
	mov	r0, #3
	str	r3, [fp, #-68]
	blx	r2
	ldr	r3, [fp, #-68]
.L394:
	cmp	r7, #1
	beq	.L440
	tst	r10, #1
	bne	.L398
	tst	r3, #131072
	beq	.L399
	tst	r0, #1
	bne	.L390
.L399:
	ldr	r3, [r9, #16]
	mov	r0, #0
	blx	r3
	ldr	r3, [fp, #-64]
	rsb	r6, r3, r6
	mov	r3, #0
	str	r3, [r5, r6]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L439:
	ldr	r1, .L450+8
	mov	r0, #0
	mov	r3, r4
	str	r0, [sp]
	ldr	r2, .L450+16
	ldr	ip, [r1, #68]
	ldr	r1, .L450+20
	blx	ip
.L390:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L440:
	ands	r10, r10, #5
	beq	.L441
	ldr	r3, [r8, #80]
	cmp	r3, #0
	beq	.L398
	mov	r0, r7
	mov	r1, r4
	blx	r3
	and	r0, r0, #655360
	cmp	r0, #655360
	beq	.L403
	cmp	r0, #524288
	beq	.L442
	cmp	r0, #131072
	bne	.L398
	ldr	r3, [fp, #-64]
	rsb	r3, r3, r6
	add	r3, r5, r3
	add	r3, r3, #48
	ldr	r2, [r3, #4]
	cmp	r2, #2
	beq	.L443
.L403:
	ldr	r3, [fp, #-64]
	mov	r2, #1
	rsb	r3, r3, r6
	add	r3, r5, r3
	str	r2, [r3, #52]
.L398:
	mov	r3, r4, asl #2
	mov	r1, #1
	add	r2, r5, r3
	str	r3, [fp, #-68]
	sub	r0, fp, #52
	ldr	r3, [r9, #48]
	str	r1, [r2, #388]
	mov	r2, #8
	mov	r1, #0
	ldr	r7, .L450+8
	blx	r3
	ldr	r3, [r8, #112]
	sub	r2, fp, #56
	str	r4, [fp, #-48]
	cmp	r3, #0
	str	r2, [fp, #-52]
	beq	.L404
	sub	r0, fp, #52
	blx	r3
.L405:
	ldr	r3, [r8, #68]
	cmp	r3, #0
	beq	.L406
	mov	r0, r4
	blx	r3
.L407:
	ldr	r3, [fp, #-64]
	mov	r2, #0
	mov	r1, #6
	ldr	r8, .L450
	rsb	r7, r3, r6
	add	r7, r5, r7
	ldr	r0, [r7, #8]
	bl	DBG_CountTimeInfo
	mov	r0, #6
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L444
.L409:
	ldr	r3, [fp, #-64]
	rsb	r2, r3, r6
	ldr	r3, .L450
	add	r0, r5, r2
	add	r1, r0, #40
	ldr	ip, [r0, #44]
	cmp	ip, #1
	beq	.L445
	mov	r8, #84
	mov	r1, #180
	mul	r8, r8, r4
	ldr	r7, .L450+24
	mla	r1, r1, r4, r3
	ldr	r2, [fp, #-68]
	add	r10, r7, r8
	add	r3, r3, r2
	ldr	r0, [r10, #4]
	ldr	r2, [r3, #2508]
	ldr	r1, [r1, #2224]
	cmp	r0, #0
	add	r2, r2, r1
	str	r2, [r3, #2508]
	bne	.L446
.L414:
	ldr	r3, [fp, #-64]
	rsb	r3, r3, r6
	add	r7, r5, r3
	ldr	r2, [r5, r3]
	cmp	r2, #0
	beq	.L418
	ldr	r8, [r7, #16]
	cmp	r8, #0
	beq	.L447
.L418:
	ldr	r3, [fp, #-64]
	ldr	r10, .L450
	rsb	r8, r3, r6
	add	r7, r5, r8
	ldr	r0, [r7, #8]
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L448
	ldr	r0, [r7, #8]
	bl	VCTRL_GetDecParam
	subs	ip, r0, #0
	beq	.L449
	mov	r0, r4
	bl	VDMDRV_AfterDec
.L421:
	ldr	r3, [fp, #-64]
	mov	r2, #0
	rsb	r6, r3, r6
	ldr	r3, [fp, #-68]
	add	r3, r5, r3
	add	r5, r5, r6
	ldr	r0, [r5, #8]
	str	r2, [r3, #388]
	bl	VCTRL_InformVdmFree
.L412:
	ldr	r3, [r9, #16]
	mov	r0, #0
	blx	r3
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L424:
	mov	r10, r3
	mov	r0, r3
	b	.L394
.L448:
	movw	r2, #1806
	ldr	r3, [r7, #8]
	ldr	ip, [r9, #68]
	mov	r0, #0
	ldr	r1, .L450+28
	blx	ip
	ldr	r3, [r7, #8]
	mov	r2, #0
	str	r2, [r10, r8]
	add	r3, r3, #32
	add	r10, r10, r3, lsl #2
	str	r2, [r10, #68]
	b	.L421
.L446:
	ldr	r3, [r9]
	blx	r3
	ldr	ip, [r7, r8]
	ldr	r1, [r10, #4]
	mov	lr, #84
	add	r3, ip, #49920
	add	r3, r3, #80
	cmp	r3, r1
	mov	r3, #180
	mla	r3, r3, r4, r5
	strcc	r1, [r7, r8]
	mul	lr, lr, r4
	movcc	ip, r1
	ldrcs	r2, [r10, #8]
	movcc	r2, #0
	add	r7, r7, lr
	ldr	r8, [r3, #2224]
	ldr	r3, .L450+24
	str	lr, [fp, #-72]
	ldr	lr, [r7, #52]
	add	r8, lr, r8, lsr #10
	str	r8, [r7, #52]
	rsb	ip, ip, r0
	rsb	r0, r1, r0
	cmp	ip, #1000
	ldr	r1, [r7, #48]
	add	r0, r0, r2
	add	r2, r7, #8
	add	r1, r1, #1
	str	r2, [fp, #-76]
	str	r0, [r7, #8]
	mov	r2, #0
	str	r1, [r7, #48]
	str	r2, [r7, #4]
	bcc	.L414
	mov	lr, #1000
	mov	r1, ip
	mul	r0, lr, r0
	str	r2, [fp, #-88]
	str	r3, [fp, #-84]
	str	ip, [fp, #-80]
	bl	__aeabi_uidiv
	ldr	r2, [fp, #-88]
	ldr	r3, [fp, #-84]
	ldr	r1, [fp, #-72]
	ldr	ip, [fp, #-80]
	str	r8, [r7, #56]
	str	r2, [r3, r1]
	ldr	r3, [fp, #-76]
	str	ip, [r7, #60]
	str	r2, [r7, #52]
	str	r2, [r7, #48]
	str	r0, [r3, #4]
	b	.L414
.L444:
	ldr	r3, [r9]
	blx	r3
	ldr	r2, [r7, #8]
	ldr	r3, .L450+32
	add	r8, r8, r2, lsl #2
	ldr	r1, [r3]
	ldr	r3, [r8, #2444]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r1
	bls	.L409
	add	r2, r4, r2, lsl #24
	movw	r1, #1733
	add	r2, r2, #393216
	ldr	r0, .L450+36
	str	ip, [r8, #2444]
	bl	DBG_AddTrace
	b	.L409
.L445:
	ldr	r0, [r0, #8]
	mov	ip, #0
	str	ip, [r1, #4]
	add	r1, r0, #32
	str	ip, [r3, r2]
	add	r3, r3, r1, lsl #2
	str	ip, [r3, #68]
	b	.L412
.L406:
	mov	r0, r3
	movw	r2, #1719
	ldr	r3, [r9, #68]
	ldr	r1, .L450+40
	blx	r3
	b	.L407
.L404:
	ldr	ip, [r7, #68]
	mov	r0, r3
	movw	r2, #1717
	ldr	r1, .L450+40
	blx	ip
	b	.L405
.L442:
	ldr	r3, [fp, #-64]
	mov	r2, #2
	rsb	r3, r3, r6
	add	r3, r5, r3
	str	r2, [r3, #52]
	b	.L398
.L447:
	ldr	r3, [r9, #68]
	mov	r0, r8
	ldr	r1, .L450+44
	blx	r3
	ldr	r0, [r7, #8]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L419
	ldr	r0, [r7, #8]
	mov	r3, r8
	ldr	r2, [fp, #-56]
	mov	r1, #100
	str	r4, [sp]
	bl	VCTRL_VdmPostProc
.L419:
	ldr	r3, [fp, #-64]
	mov	r2, #0
	rsb	r6, r3, r6
	add	r3, r5, r6
	str	r2, [r5, r6]
	ldr	r3, [r3, #8]
	add	r3, r3, #32
	add	r5, r5, r3, lsl #2
	str	r2, [r5, #68]
	b	.L412
.L441:
	ldr	r3, [r9, #16]
	mov	r0, r10
	blx	r3
	ldr	r3, [fp, #-64]
	rsb	r6, r3, r6
	str	r10, [r5, r6]
	b	.L390
.L449:
	ldr	r3, [r7, #8]
	movw	r2, #1816
	ldr	r4, [r9, #68]
	ldr	r1, .L450+48
	str	ip, [fp, #-72]
	blx	r4
	ldr	ip, [fp, #-72]
	ldr	r3, [r7, #8]
	str	ip, [r10, r8]
	add	r10, r10, r3, lsl #2
	str	ip, [r10, #196]
	b	.L421
.L443:
	mov	r2, #3
	str	r2, [r3, #4]
	b	.L398
.L451:
	.align	2
.L450:
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR1+100
	.word	.LC23
	.word	g_VfmwGlobalStat
	.word	.LC25
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+124
	.word	.LC3
	.word	.LC24
	.word	.LC26
	UNWIND(.fnend)
	.size	VDMDRV_VdmIntServProc, .-VDMDRV_VdmIntServProc
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
	ldr	r4, .L456
	ldr	r3, [r4, #8]
	cmp	r3, r0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L456+4
	ldr	r3, [r3, #56]
	cmp	r3, #0
	beq	.L454
	mov	r0, #0
	blx	r3
.L455:
	mov	r3, #0
	str	r3, [r4]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L454:
	ldr	ip, .L456+8
	mov	r0, r3
	movw	r2, #2094
	ldr	r1, .L456+12
	ldr	r3, [ip, #68]
	blx	r3
	b	.L455
.L457:
	.align	2
.L456:
	.word	.LANCHOR0
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC3
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
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	ip, r0, #0
	bne	.L458
	add	r3, r1, #1
	cmp	r3, #16
	bhi	.L458
	ldr	r3, .L465
	cmn	r1, #1
	mov	r2, #1
	str	r2, [r3, #48]
	beq	.L458
	ldr	lr, .L465+4
	mvn	r6, #0
	ldr	r4, .L465+8
	movw	r3, #2123
	ldr	r2, .L465+12
	mov	r0, #22
	ldr	r5, [lr, r1, asl #2]
	ldr	r1, .L465+16
	str	r6, [r5, #32]
	ldr	lr, [lr]
	ldr	r4, [r4, #68]
	ldr	lr, [lr, #32]
	stmia	sp, {ip, lr}
	blx	r4
.L458:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L466:
	.align	2
.L465:
	.word	.LANCHOR0
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+148
	.word	.LC27
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
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r0, #0
	mov	ip, r1
	bne	.L467
	cmp	r1, #15
	bls	.L469
.L467:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L469:
	ldr	lr, .L470
	mov	r6, #1
	ldr	r4, .L470+4
	movw	r3, #2136
	ldr	r2, .L470+8
	mov	r0, #22
	ldr	r5, [lr, r1, asl #2]
	ldr	r1, .L470+12
	str	r6, [r5, #32]
	ldr	lr, [lr, ip, asl #2]
	str	ip, [sp, #4]
	ldr	ip, [r4, #68]
	ldr	lr, [lr, #32]
	str	lr, [sp]
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L471:
	.align	2
.L470:
	.word	s_pstVfmwChan
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+172
	.word	.LC28
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
	.type	__func__.13647, %object
	.size	__func__.13647, 23
__func__.13647:
	.ascii	"VDMDRV_CheckAttachMode\000"
	.space	1
	.type	__func__.13652, %object
	.size	__func__.13652, 19
__func__.13652:
	.ascii	"VDMDRV_ChnDecReady\000"
	.space	1
	.type	__FUNCTION__.13668, %object
	.size	__FUNCTION__.13668, 19
__FUNCTION__.13668:
	.ascii	"VDMDRV_DoBeforeDec\000"
	.space	1
	.type	__FUNCTION__.13703, %object
	.size	__FUNCTION__.13703, 16
__FUNCTION__.13703:
	.ascii	"VDMDRV_AfterDec\000"
	.type	__FUNCTION__.13722, %object
	.size	__FUNCTION__.13722, 19
__FUNCTION__.13722:
	.ascii	"VDMDRV_CheckDevice\000"
	.space	1
	.type	__func__.13740, %object
	.size	__func__.13740, 22
__func__.13740:
	.ascii	"VDMDRV_VdmIntServProc\000"
	.space	2
	.type	__FUNCTION__.13741, %object
	.size	__FUNCTION__.13741, 22
__FUNCTION__.13741:
	.ascii	"VDMDRV_VdmIntServProc\000"
	.space	2
	.type	__func__.13825, %object
	.size	__func__.13825, 21
__func__.13825:
	.ascii	"VDMDRV_MarkReloadDsp\000"
	.space	3
	.type	__func__.13830, %object
	.size	__func__.13830, 23
__func__.13830:
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
	ASCII(.ascii	"VDMHAL_V200_IsMpeg4NvopCopy point of picture para n" )
	ASCII(.ascii	"ull, wait forever!!!\012\000" )
	.space	3
.LC1:
	ASCII(.ascii	"VDMDRV_IsVc1SkpicCopy point of picture para null, w" )
	ASCII(.ascii	"ait forever!!!\012\000" )
	.space	1
.LC2:
	ASCII(.ascii	"====== forece VDM sleep! ======\012\000" )
	.space	3
.LC3:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC4:
	ASCII(.ascii	"%s,%d,  frameRate = %d,last = %d\012\000" )
	.space	2
.LC5:
	ASCII(.ascii	"%s,%d, the frameRate change!\012\000" )
	.space	2
.LC6:
	ASCII(.ascii	"%s,%d,chanid = %d,s32Is4k60fpsb = %d\012\000" )
	.space	2
.LC7:
	ASCII(.ascii	"%s The channel %d is not active\012\000" )
	.space	3
.LC8:
	ASCII(.ascii	"The channel %d is decoding\012\000" )
.LC9:
	ASCII(.ascii	"The channel %d DEC PARAM is not ready\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"VDMDRV_IsVp6NvopCopy point of picture para null, wa" )
	ASCII(.ascii	"it forever!!!\012\000" )
	.space	2
.LC11:
	ASCII(.ascii	"The channel %d DEC PARAM is not ready(pDecParam == " )
	ASCII(.ascii	"NULL)\012\000" )
	.space	2
.LC12:
	ASCII(.ascii	"---SKIP REPAIR!---\012\000" )
.LC13:
	ASCII(.ascii	"VDM Start Fail 2 !\012\000" )
.LC14:
	ASCII(.ascii	"isr begin: *pVdmStateMachine = %d, timeout = %d, Er" )
	ASCII(.ascii	"rRatio = %d\012\000" )
.LC15:
	ASCII(.ascii	"ISR process vdm timeout!\012\000" )
	.space	2
.LC16:
	ASCII(.ascii	"*pErrRatio = %d\012\000" )
	.space	3
.LC17:
	ASCII(.ascii	"*pH264NeedMoreStart, *pH264FinishedMultiStart = %d," )
	ASCII(.ascii	"%d\012\000" )
	.space	1
.LC18:
	ASCII(.ascii	"VdmStateMachine=%d Error!\012\000" )
	.space	1
.LC19:
	ASCII(.ascii	"VDM Time Out(%d ms), Reset VDM!\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"VDM Start Fail 1 !\012\000" )
.LC21:
	ASCII(.ascii	"h264 more start need parameter\012\000" )
.LC22:
	ASCII(.ascii	"wait more start time out!\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"%s: VdhId(%d) > %d\012\000" )
.LC24:
	ASCII(.ascii	"vdm state machine = %d, but DecParam is null!\012\000" )
	.space	1
.LC25:
	ASCII(.ascii	"line :%d ISR called but chan %d is not active!\012\000" )
.LC26:
	ASCII(.ascii	"lline: %d ISR called but chan %d is not active!\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC27:
	ASCII(.ascii	"%s,%d, i = %d, MatchVdhIdPlus1 = %d\012\000" )
	.space	3
.LC28:
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
	.type	g_LastFPS.13644, %object
	.size	g_LastFPS.13644, 64
g_LastFPS.13644:
	.space	64
	.type	DiffFrameCnt.13643, %object
	.size	DiffFrameCnt.13643, 64
DiffFrameCnt.13643:
	.space	64
	.type	s_VdmIsInISR, %object
	.size	s_VdmIsInISR, 4
s_VdmIsInISR:
	.space	4
	.type	g_RepairParam, %object
	.size	g_RepairParam, 1744
g_RepairParam:
	.space	1744
	.type	last_rec_pos_time.13666, %object
	.size	last_rec_pos_time.13666, 64
last_rec_pos_time.13666:
	.space	64
	.type	g_BackUp, %object
	.size	g_BackUp, 180
g_BackUp:
	.space	180
	.type	last_rec_pos_time.13697, %object
	.size	last_rec_pos_time.13697, 64
last_rec_pos_time.13697:
	.space	64
	.type	last_rec_pos_time.13736, %object
	.size	last_rec_pos_time.13736, 64
last_rec_pos_time.13736:
	.space	64
	.type	cycle.13735, %object
	.size	cycle.13735, 8
cycle.13735:
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
	.size	g_HwMem, 1208
g_HwMem:
	.space	1208
	.type	g_UpMsg, %object
	.size	g_UpMsg, 3200
g_UpMsg:
	.space	3200
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
