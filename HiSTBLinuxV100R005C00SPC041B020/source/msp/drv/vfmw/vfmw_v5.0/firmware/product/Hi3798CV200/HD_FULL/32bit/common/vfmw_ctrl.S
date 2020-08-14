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
	.file	"vfmw_ctrl.c"
	.text
	.align	2
	.global	VCTRL_EventReport
	.type	VCTRL_EventReport, %function
VCTRL_EventReport:
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
	UNWIND(.pad #8)
	sub	sp, sp, #8
	cmp	r0, #15
	bhi	.L13
	ldr	lr, .L17
	ldr	ip, [lr, r0, asl #2]
	cmp	ip, #0
	beq	.L14
	ldr	ip, [ip, #40]
	cmp	ip, #0
	beq	.L6
	cmp	ip, #1
	bne	.L15
	ldr	r3, [lr, #308]
	cmp	r3, #0
	beq	.L16
.L9:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L15:
	str	r3, [sp]
	mov	r0, #0
	mov	r3, ip
	ldr	r2, .L17+4
	ldr	r1, .L17+8
	bl	dprint_vfmw
.L3:
	mvn	r0, #0
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L6:
	ldr	r3, [lr, #304]
	cmp	r3, #0
	bne	.L9
	mov	r0, r3
	ldr	r1, .L17+12
	bl	dprint_vfmw
	b	.L3
.L16:
	mov	r0, r3
	ldr	r1, .L17+16
	bl	dprint_vfmw
	b	.L3
.L13:
	ldr	r3, .L17+20
	movw	r2, #978
	ldr	r1, .L17+24
	mov	r0, #0
	bl	dprint_vfmw
	b	.L3
.L14:
	mov	r0, ip
	ldr	r3, .L17+28
	movw	r2, #979
	ldr	r1, .L17+24
	bl	dprint_vfmw
	b	.L3
.L18:
	.align	2
.L17:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LC5
	.word	.LC3
	.word	.LC4
	.word	.LC0
	.word	.LC1
	.word	.LC2
	UNWIND(.fnend)
	.size	VCTRL_EventReport, .-VCTRL_EventReport
	.align	2
	.global	VCTRL_Isr_0
	.type	VCTRL_Isr_0, %function
VCTRL_Isr_0:
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
	bl	VDMDRV_VdmIntServProc
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_Isr_0, .-VCTRL_Isr_0
	.align	2
	.global	VCTRL_Scd_Isr
	.type	VCTRL_Scd_Isr, %function
VCTRL_Scd_Isr:
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
	bl	SM_SCDIntServeProc
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_Scd_Isr, .-VCTRL_Scd_Isr
	.align	2
	.global	VCTRL_MMU_ISR
	.type	VCTRL_MMU_ISR, %function
VCTRL_MMU_ISR:
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
	bl	VDMDRV_MMUIntServeProc
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_MMU_ISR, .-VCTRL_MMU_ISR
	.align	2
	.type	VCTRL_StopTask.part.0, %function
VCTRL_StopTask.part.0:
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
	ldr	r5, .L34
	ldr	r3, [r5, #68]
	cmp	r3, #1
	beq	.L25
	ldr	r6, .L34+4
	mov	r0, #10
	mov	r4, #49
	ldr	r3, [r6, #116]
	blx	r3
	b	.L26
.L33:
	ldr	r3, [r6, #116]
	blx	r3
	subs	r4, r4, #1
	beq	.L32
.L26:
	ldr	r3, [r5, #68]
	mov	r0, #10
	cmp	r3, #1
	bne	.L33
.L25:
	mov	r0, #0
.L24:
	mov	r3, #0
	str	r3, [r5, #72]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L32:
	mvn	r0, #0
	b	.L24
.L35:
	.align	2
.L34:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_StopTask.part.0, .-VCTRL_StopTask.part.0
	.align	2
	.type	VCTRL_StartTask.part.1, %function
VCTRL_StartTask.part.1:
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
	ldr	r5, .L48
	ldr	r3, [r5, #68]
	cmp	r3, #2
	beq	.L39
	ldr	r6, .L48+4
	mov	r0, #10
	mov	r4, #49
	ldr	r3, [r6, #116]
	blx	r3
	b	.L40
.L47:
	ldr	r3, [r6, #116]
	blx	r3
	subs	r4, r4, #1
	beq	.L46
.L40:
	ldr	r3, [r5, #68]
	mov	r0, #10
	cmp	r3, #2
	bne	.L47
.L39:
	mov	r0, #0
.L38:
	mov	r3, #0
	str	r3, [r5, #72]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L46:
	mvn	r0, #0
	b	.L38
.L49:
	.align	2
.L48:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_StartTask.part.1, .-VCTRL_StartTask.part.1
	.align	2
	.type	VCTRL_SetSendStreamAgainFlag.part.2, %function
VCTRL_SetSendStreamAgainFlag.part.2:
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
	ldr	r3, .L51
	movw	r2, #1310
	ldr	r1, .L51+4
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L52:
	.align	2
.L51:
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_SetSendStreamAgainFlag.part.2, .-VCTRL_SetSendStreamAgainFlag.part.2
	.align	2
	.global	VCTRL_Suspend
	.type	VCTRL_Suspend, %function
VCTRL_Suspend:
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
	ldr	r6, .L107
	ldr	r8, .L107+4
	ldr	r3, [r6]
	blx	r3
	ldr	r3, [r8]
	cmp	r3, #0
	mov	r9, r0
	beq	.L54
	mov	r0, #0
	blx	r3
.L55:
	mov	r0, #0
	mov	r4, #32
	mov	r5, r0
	bl	SCDDRV_PrepareSleep
	mov	r0, r5
	mov	r7, r5
	bl	VDMDRV_PrepareSleep
	ldr	r3, [r6]
	blx	r3
	mov	r10, r0
.L64:
	mov	r0, #0
	bl	SCDDRV_GetSleepStage
	cmp	r0, #2
	bne	.L99
	mov	r0, #0
	bl	VDMDRV_GetSleepStage
	cmp	r0, #2
	bne	.L100
.L59:
	ldr	r3, .L107+8
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L101
.L66:
	ldr	r3, .L107+12
	ldr	r2, [r3, #64]
	cmp	r2, #1
	beq	.L102
.L72:
	ldr	r3, [r6]
	blx	r3
	ldr	r1, .L107+16
	cmp	r9, r0
	rsbls	r2, r9, r0
	mvnhi	r9, r9
	addhi	r2, r9, r0
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	b	dprint_vfmw
.L99:
	mov	r0, #0
	str	r7, [fp, #-48]
	bl	VDMDRV_GetSleepStage
	ldr	r3, [fp, #-48]
	cmp	r0, #2
	bne	.L75
	mov	r5, #1
	mov	r2, r5
.L76:
	tst	r2, r3
	bne	.L59
	subs	r4, r4, #1
	beq	.L103
	ldr	r3, [r6, #116]
	mov	r0, #10
	blx	r3
	eor	r3, r5, #1
	eor	r2, r7, #1
	orrs	r3, r3, r2
	bne	.L64
	ldr	r3, .L107+8
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L66
.L101:
	ldr	r3, [r8, #32]
	cmp	r3, #0
	beq	.L67
	mov	r0, #0
	blx	r3
.L67:
	ldr	r3, [r8, #24]
	cmp	r3, #0
	beq	.L104
	mov	r0, #0
	blx	r3
	ldr	r3, [r8, #8]
	cmp	r3, #0
	beq	.L69
.L106:
	mov	r0, #0
	blx	r3
.L70:
	ldr	r3, [r8, #40]
	cmp	r3, #0
	beq	.L66
	mov	r0, #0
	blx	r3
	ldr	r3, .L107+12
	ldr	r2, [r3, #64]
	cmp	r2, #1
	bne	.L72
.L102:
	mov	r2, #2
	str	r2, [r3, #72]
	bl	VCTRL_StopTask.part.0
	b	.L72
.L100:
	mov	r7, #1
	mov	r3, r7
.L75:
	mov	r2, r5
	b	.L76
.L103:
	ldr	r3, [r6]
	blx	r3
	str	r5, [sp]
	mov	r3, r7
	ldr	r1, .L107+20
	cmp	r10, r0
	rsbls	r2, r10, r0
	mvnhi	r10, r10
	addhi	r2, r10, r0
	mov	r0, #0
	bl	dprint_vfmw
	cmp	r7, #1
	beq	.L105
	mov	r2, #0
	ldr	r1, .L107+24
	mov	r0, r2
	bl	dprint_vfmw
	mov	r0, #0
	bl	SCDDRV_ForceSleep
	cmp	r5, #1
	beq	.L59
.L81:
	mov	r2, #0
	ldr	r1, .L107+28
	mov	r0, r2
	bl	dprint_vfmw
	mov	r0, #0
	bl	VDMDRV_ForceSleep
	b	.L59
.L105:
	cmp	r5, #1
	bne	.L81
	b	.L59
.L104:
	mov	r0, r3
	movw	r2, #382
	ldr	r1, .L107+32
	bl	dprint_vfmw
	ldr	r3, [r8, #8]
	cmp	r3, #0
	bne	.L106
.L69:
	mov	r0, r3
	movw	r2, #387
	ldr	r1, .L107+32
	bl	dprint_vfmw
	b	.L70
.L54:
	mov	r0, r3
	mov	r2, #280
	ldr	r1, .L107+32
	bl	dprint_vfmw
	b	.L55
.L108:
	.align	2
.L107:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	g_HalDisable
	.word	.LANCHOR0
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC7
	.word	.LC6
	UNWIND(.fnend)
	.size	VCTRL_Suspend, .-VCTRL_Suspend
	.align	2
	.global	VCTRL_Resume
	.type	VCTRL_Resume, %function
VCTRL_Resume:
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
	ldr	r5, .L133
	ldr	r3, [r5]
	blx	r3
	ldr	r3, .L133+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	mov	r4, r0
	beq	.L129
.L110:
	mov	r0, #0
	bl	SCDDRV_ExitSleep
	mov	r0, #0
	bl	VDMDRV_ExitSleep
	ldr	r3, .L133+8
	ldr	r2, [r3, #64]
	cmp	r2, #1
	beq	.L130
.L120:
	ldr	r3, [r5]
	blx	r3
	ldr	r1, .L133+12
	cmp	r4, r0
	rsbls	r2, r4, r0
	mvnhi	r2, r4
	addhi	r2, r2, r0
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L129:
	ldr	r6, .L133+16
	ldr	r3, [r6, #4]
	cmp	r3, #0
	beq	.L131
	mov	r0, #0
	blx	r3
.L112:
	ldr	r3, [r6, #20]
	cmp	r3, #0
	beq	.L132
	mov	r0, #0
	blx	r3
.L114:
	ldr	r3, [r6, #28]
	cmp	r3, #0
	beq	.L115
	mov	r0, #0
	blx	r3
.L115:
	ldr	r3, [r6, #36]
	cmp	r3, #0
	beq	.L116
	mov	r0, #0
	blx	r3
.L116:
	ldr	r3, [r6, #72]
	cmp	r3, #0
	beq	.L117
	mov	r0, #0
	blx	r3
.L118:
	ldr	r0, [r6, #76]
	cmp	r0, #0
	beq	.L119
	blx	r0
	b	.L110
.L130:
	str	r2, [r3, #72]
	bl	VCTRL_StartTask.part.1
	b	.L120
.L132:
	mov	r0, r3
	mov	r2, #432
	ldr	r1, .L133+20
	bl	dprint_vfmw
	b	.L114
.L117:
	mov	r0, r3
	movw	r2, #439
	ldr	r1, .L133+20
	bl	dprint_vfmw
	b	.L118
.L119:
	mov	r2, #440
	ldr	r1, .L133+20
	bl	dprint_vfmw
	b	.L110
.L131:
	mov	r0, r3
	movw	r2, #427
	ldr	r1, .L133+20
	bl	dprint_vfmw
	b	.L112
.L134:
	.align	2
.L133:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_HalDisable
	.word	.LANCHOR0
	.word	.LC11
	.word	g_vdm_hal_fun_ptr
	.word	.LC6
	UNWIND(.fnend)
	.size	VCTRL_Resume, .-VCTRL_Resume
	.align	2
	.global	ChanCtx_Init
	.type	ChanCtx_Init, %function
ChanCtx_Init:
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
	ldr	r3, .L138
	mov	r2, #192
	mov	r1, #0
	ldr	r0, .L138+4
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, .L138+8
	mvn	r1, #0
	add	r2, r3, #64
.L136:
	str	r1, [r3, #4]!
	cmp	r3, r2
	bne	.L136
	ldmfd	sp, {fp, sp, pc}
.L139:
	.align	2
.L138:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_ChanCtx
	.word	g_ChanCtx+60
	UNWIND(.fnend)
	.size	ChanCtx_Init, .-ChanCtx_Init
	.align	2
	.global	VCTRL_Isr_1
	.type	VCTRL_Isr_1, %function
VCTRL_Isr_1:
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
	mov	r0, #1
	bl	VDMDRV_VdmIntServProc
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_Isr_1, .-VCTRL_Isr_1
	.align	2
	.global	VCTRL_Isr_2
	.type	VCTRL_Isr_2, %function
VCTRL_Isr_2:
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
	mov	r0, #2
	bl	VDMDRV_VdmIntServProc
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_Isr_2, .-VCTRL_Isr_2
	.align	2
	.global	VCTRL_Scd_Isr_1
	.type	VCTRL_Scd_Isr_1, %function
VCTRL_Scd_Isr_1:
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
	mov	r0, #1
	bl	SM_SCDIntServeProc
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_Scd_Isr_1, .-VCTRL_Scd_Isr_1
	.align	2
	.global	VCTRL_Scd_Isr_2
	.type	VCTRL_Scd_Isr_2, %function
VCTRL_Scd_Isr_2:
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
	mov	r0, #2
	bl	SM_SCDIntServeProc
	mov	r0, #1
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_Scd_Isr_2, .-VCTRL_Scd_Isr_2
	.align	2
	.global	VCTRL_InformVdmFree
	.type	VCTRL_InformVdmFree, %function
VCTRL_InformVdmFree:
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
	ldr	r3, .L145
	mov	r0, #0
	ldr	r3, [r3, #124]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L146:
	.align	2
.L145:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_InformVdmFree, .-VCTRL_InformVdmFree
	.align	2
	.global	VCTRL_ExtraWakeUpThread
	.type	VCTRL_ExtraWakeUpThread, %function
VCTRL_ExtraWakeUpThread:
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
	ldr	r3, .L148
	mov	r0, #0
	ldr	r3, [r3, #124]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L149:
	.align	2
.L148:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_ExtraWakeUpThread, .-VCTRL_ExtraWakeUpThread
	.align	2
	.global	VCTRL_SetMoreGapEnable
	.type	VCTRL_SetMoreGapEnable, %function
VCTRL_SetMoreGapEnable:
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
	cmp	r0, #15
	bhi	.L156
	ldr	r3, .L159
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L157
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L158
	cmp	r1, #1
	strls	r1, [r3, #1208]
	movls	r0, #0
	movhi	r2, #0
	strhi	r2, [r3, #1208]
	mvnhi	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L158:
	ldr	r3, .L159+4
	movw	r2, #911
	ldr	r1, .L159+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L156:
	ldr	r3, .L159+12
	movw	r2, #909
	ldr	r1, .L159+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L157:
	mov	r0, r3
	movw	r2, #910
	ldr	r3, .L159+16
	ldr	r1, .L159+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L160:
	.align	2
.L159:
	.word	.LANCHOR0
	.word	.LC13
	.word	.LC1
	.word	.LC12
	.word	.LC2
	UNWIND(.fnend)
	.size	VCTRL_SetMoreGapEnable, .-VCTRL_SetMoreGapEnable
	.align	2
	.global	VCTRL_SetCallBack
	.type	VCTRL_SetCallBack, %function
VCTRL_SetCallBack:
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
	beq	.L169
	cmp	r0, #0
	beq	.L165
	cmp	r0, #1
	bne	.L170
	ldr	r3, .L171
	mov	r0, #0
	str	r1, [r3, #308]
	ldmfd	sp, {fp, sp, pc}
.L170:
	mov	r3, r0
	ldr	r2, .L171+4
	ldr	r1, .L171+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L165:
	ldr	r3, .L171
	str	r1, [r3, #304]
	ldmfd	sp, {fp, sp, pc}
.L169:
	mov	r0, r1
	ldr	r3, .L171+12
	mov	r2, #1020
	ldr	r1, .L171+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L172:
	.align	2
.L171:
	.word	.LANCHOR0
	.word	.LANCHOR1+20
	.word	.LC15
	.word	.LC14
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_SetCallBack, .-VCTRL_SetCallBack
	.align	2
	.global	VCTRL_StopTask
	.type	VCTRL_StopTask, %function
VCTRL_StopTask:
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
	ldr	r3, .L175
	ldr	r2, [r3, #64]
	cmp	r2, #1
	bne	.L174
	mov	r2, #2
	str	r2, [r3, #72]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_StopTask.part.0
.L174:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L176:
	.align	2
.L175:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_StopTask, .-VCTRL_StopTask
	.align	2
	.global	VCTRL_StartTask
	.type	VCTRL_StartTask, %function
VCTRL_StartTask:
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
	ldr	r3, .L179
	ldr	r2, [r3, #64]
	cmp	r2, #1
	bne	.L178
	str	r2, [r3, #72]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_StartTask.part.1
.L178:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L180:
	.align	2
.L179:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_StartTask, .-VCTRL_StartTask
	.align	2
	.global	VCTRL_SetStreamInterface
	.type	VCTRL_SetStreamInterface, %function
VCTRL_SetStreamInterface:
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
	subs	r7, r1, #0
	mov	r5, r0
	beq	.L185
	ldr	r4, .L187
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L186
	ldr	r6, .L187+4
	mov	r2, #12
	add	r0, r0, #172
	add	r4, r4, #336
	ldr	r3, [r6, #52]
	blx	r3
	mov	r0, r5, asl #4
	sub	r0, r0, r5, asl #2
	ldr	r3, [r6, #52]
	mov	r1, r7
	add	r0, r4, r0
	mov	r2, #12
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L186:
	ldr	r3, .L187+8
	movw	r2, #1297
	ldr	r1, .L187+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L185:
	mov	r0, r7
	ldr	r3, .L187+16
	mov	r2, #1296
	ldr	r1, .L187+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L188:
	.align	2
.L187:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	.word	.LC16
	UNWIND(.fnend)
	.size	VCTRL_SetStreamInterface, .-VCTRL_SetStreamInterface
	.align	2
	.global	VCTRL_SetSendStreamAgainFlag
	.type	VCTRL_SetSendStreamAgainFlag, %function
VCTRL_SetSendStreamAgainFlag:
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
	ldr	r3, .L192
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L191
	add	r3, r3, #290816
	mov	r0, #0
	str	r1, [r3, #1868]
	ldmfd	sp, {fp, sp, pc}
.L191:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_SetSendStreamAgainFlag.part.2
.L193:
	.align	2
.L192:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_SetSendStreamAgainFlag, .-VCTRL_SetSendStreamAgainFlag
	.align	2
	.global	VCTRL_GetSendStreamAgainFlag
	.type	VCTRL_GetSendStreamAgainFlag, %function
VCTRL_GetSendStreamAgainFlag:
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
	ldr	r3, .L198
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L197
	add	r0, r0, #290816
	ldr	r0, [r0, #1868]
	ldmfd	sp, {fp, sp, pc}
.L197:
	ldr	r3, .L198+4
	movw	r2, #1317
	ldr	r1, .L198+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L199:
	.align	2
.L198:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_GetSendStreamAgainFlag, .-VCTRL_GetSendStreamAgainFlag
	.align	2
	.global	VCTRL_GetStreamInterface
	.type	VCTRL_GetStreamInterface, %function
VCTRL_GetStreamInterface:
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
	ldr	r5, .L206
	mov	r4, r0
	ldr	r1, [r5, r0, asl #2]
	cmp	r1, #0
	beq	.L204
	ldr	r2, .L206+4
	mov	r3, r0, asl #4
	sub	r3, r3, r0, asl #2
	add	r0, r5, #336
	add	r0, r0, r3
	add	r1, r1, #172
	ldr	r3, [r2, #60]
	mov	r2, #12
	blx	r3
	cmp	r0, #0
	bne	.L205
	ldr	r1, [r5, r4, asl #2]
	add	r0, r1, #172
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L205:
	mov	r3, r4
	movw	r2, #1335
	ldr	r1, .L206+8
	mov	r0, #0
	bl	dprint_vfmw
.L204:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L207:
	.align	2
.L206:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC17
	UNWIND(.fnend)
	.size	VCTRL_GetStreamInterface, .-VCTRL_GetStreamInterface
	.align	2
	.global	VCTRL_SetFrameInterface
	.type	VCTRL_SetFrameInterface, %function
VCTRL_SetFrameInterface:
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
	subs	r3, r1, #0
	beq	.L212
	ldr	r3, .L214
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L213
	ldr	r3, .L214+4
	mov	r2, #12
	add	r0, r0, #196
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L213:
	ldr	r3, .L214+8
	movw	r2, #1349
	ldr	r1, .L214+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L212:
	mov	r0, r3
	movw	r2, #1348
	ldr	r3, .L214+16
	ldr	r1, .L214+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L215:
	.align	2
.L214:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	.word	.LC18
	UNWIND(.fnend)
	.size	VCTRL_SetFrameInterface, .-VCTRL_SetFrameInterface
	.align	2
	.global	VCTRL_GetFrameInterface
	.type	VCTRL_GetFrameInterface, %function
VCTRL_GetFrameInterface:
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
	ldr	r3, .L219
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r0, r0, #196
	ldmfd	sp, {fp, sp, pc}
.L220:
	.align	2
.L219:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetFrameInterface, .-VCTRL_GetFrameInterface
	.align	2
	.global	VCTRL_GetStreamSize
	.type	VCTRL_GetStreamSize, %function
VCTRL_GetStreamSize:
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
	cmp	r0, #15
	bhi	.L226
	ldr	r3, .L229
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L227
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L228
	add	r0, r0, #99328
	add	r0, r0, #592
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	GetSegStreamSize
.L228:
	ldr	r3, .L229+4
	movw	r2, #1390
	ldr	r1, .L229+8
	mov	r0, #0
	bl	dprint_vfmw
.L223:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L226:
	ldr	r3, .L229+12
	movw	r2, #1388
	ldr	r1, .L229+8
	mov	r0, #0
	bl	dprint_vfmw
	b	.L223
.L227:
	ldr	r3, .L229+16
	movw	r2, #1389
	ldr	r1, .L229+8
	bl	dprint_vfmw
	b	.L223
.L230:
	.align	2
.L229:
	.word	.LANCHOR0
	.word	.LC20
	.word	.LC1
	.word	.LC12
	.word	.LC19
	UNWIND(.fnend)
	.size	VCTRL_GetStreamSize, .-VCTRL_GetStreamSize
	.align	2
	.global	VCTRL_CheckFreshRaw
	.type	VCTRL_CheckFreshRaw, %function
VCTRL_CheckFreshRaw:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
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
	cmp	r0, #15
	mov	r3, #0
	str	r3, [fp, #-28]
	str	r3, [fp, #-24]
	bhi	.L238
	ldr	r2, .L241
	ldr	r0, [r2, r0, asl #2]
	cmp	r0, #0
	beq	.L239
	ldr	r2, [r0, #12]
	cmp	r2, #1
	bne	.L240
	add	r4, r0, #1584
	sub	r1, fp, #28
	add	r4, r4, #8
	mov	r0, r4
	bl	GetRawStreamNum
	cmp	r0, #0
	bne	.L237
	mov	r0, r4
	sub	r1, fp, #24
	bl	GetRawStreamSize
	cmp	r0, #0
	bne	.L237
	ldr	r3, [fp, #-24]
	mov	r0, #22
	ldr	r2, [fp, #-28]
	ldr	r1, .L241+4
	bl	dprint_vfmw
	ldr	r3, [fp, #-28]
	cmp	r3, #0
	ble	.L237
	ldr	r0, [fp, #-24]
	cmp	r0, #3
	movgt	r0, #0
	mvnle	r0, #0
.L233:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L237:
	mvn	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L240:
	mov	r0, r3
	movw	r2, #1407
	ldr	r3, .L241+8
	ldr	r1, .L241+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L233
.L238:
	mov	r0, r3
	movw	r2, #1405
	ldr	r3, .L241+16
	ldr	r1, .L241+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L233
.L239:
	ldr	r3, .L241+20
	movw	r2, #1406
	ldr	r1, .L241+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L233
.L242:
	.align	2
.L241:
	.word	.LANCHOR0
	.word	.LC21
	.word	.LC20
	.word	.LC1
	.word	.LC12
	.word	.LC19
	UNWIND(.fnend)
	.size	VCTRL_CheckFreshRaw, .-VCTRL_CheckFreshRaw
	.align	2
	.global	VCTRL_CheckFreshSeg
	.type	VCTRL_CheckFreshSeg, %function
VCTRL_CheckFreshSeg:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	cmp	r0, #15
	mov	r2, #0
	str	r2, [fp, #-20]
	str	r2, [fp, #-16]
	bhi	.L248
	ldr	r3, .L251
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L249
	ldr	r1, [r0, #12]
	cmp	r1, #1
	bne	.L250
	add	r0, r0, #99328
	sub	r2, fp, #16
	add	r0, r0, #592
	sub	r1, fp, #20
	bl	GetFreshSegStream
	ldr	r3, [fp, #-16]
	ldr	r2, [fp, #-20]
	mov	r0, #22
	ldr	r1, .L251+4
	bl	dprint_vfmw
	ldr	r0, [fp, #-20]
	cmp	r0, #0
	movgt	r0, #0
	mvnle	r0, #0
.L245:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L250:
	mov	r0, r2
	ldr	r3, .L251+8
	movw	r2, #1433
	ldr	r1, .L251+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L245
.L248:
	mov	r0, r2
	ldr	r3, .L251+16
	movw	r2, #1431
	ldr	r1, .L251+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L245
.L249:
	ldr	r3, .L251+20
	movw	r2, #1432
	ldr	r1, .L251+12
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L245
.L252:
	.align	2
.L251:
	.word	.LANCHOR0
	.word	.LC22
	.word	.LC20
	.word	.LC1
	.word	.LC12
	.word	.LC19
	UNWIND(.fnend)
	.size	VCTRL_CheckFreshSeg, .-VCTRL_CheckFreshSeg
	.align	2
	.global	VCTRL_SetDiscardPicParam
	.type	VCTRL_SetDiscardPicParam, %function
VCTRL_SetDiscardPicParam:
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
	cmp	r0, #15
	mov	r4, r0
	mov	r0, r1
	bhi	.L259
	cmp	r1, #0
	beq	.L260
	ldr	r5, .L262
	ldr	r0, [r5, r4, asl #2]
	cmp	r0, #0
	beq	.L261
	ldr	r3, .L262+4
	mov	r2, #8
	add	r0, r0, #1472
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r2, [r5, r4, asl #2]
	mov	r0, #0
	ldr	r3, [r2, #1472]
	sub	r3, r3, #1
	cmp	r3, #1
	strhi	r0, [r2, #1472]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L259:
	ldr	r3, .L262+8
	movw	r2, #1454
	ldr	r1, .L262+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L261:
	ldr	r3, .L262+16
	mov	r2, #1456
	ldr	r1, .L262+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L260:
	ldr	r3, .L262+20
	movw	r2, #1455
	ldr	r1, .L262+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L263:
	.align	2
.L262:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
	.word	.LC1
	.word	.LC2
	.word	.LC23
	UNWIND(.fnend)
	.size	VCTRL_SetDiscardPicParam, .-VCTRL_SetDiscardPicParam
	.align	2
	.global	VCTRL_GetSyntaxCtx
	.type	VCTRL_GetSyntaxCtx, %function
VCTRL_GetSyntaxCtx:
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
	ldr	r3, .L267
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	movwne	r0, #34728
	movtne	r0, 4
	addne	r0, r3, r0
	moveq	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L268:
	.align	2
.L267:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetSyntaxCtx, .-VCTRL_GetSyntaxCtx
	.align	2
	.global	VCTRL_GetImageInterface
	.type	VCTRL_GetImageInterface, %function
VCTRL_GetImageInterface:
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
	ldr	r3, .L272
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r0, r0, #184
	ldmfd	sp, {fp, sp, pc}
.L273:
	.align	2
.L272:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetImageInterface, .-VCTRL_GetImageInterface
	.align	2
	.global	VCTRL_PartitionMemForStdUser
	.type	VCTRL_PartitionMemForStdUser, %function
VCTRL_PartitionMemForStdUser:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 48
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
	ldr	r4, .L291
	cmp	r2, #31
	cmpgt	r1, #31
	mov	r7, r2
	mov	r8, r1
	mov	r9, r0
	movle	r2, #1
	movgt	r2, #0
	ldr	r5, [r4, r0, asl #2]
	ble	.L287
	ldr	r1, [fp, #4]
	cmp	r1, #0
	cmpne	r3, #0
	moveq	r6, #1
	movne	r6, #0
	beq	.L288
	ldr	r10, .L291+4
	mov	r2, #32
	mov	r1, r6
	sub	r0, fp, #76
	str	r3, [fp, #-88]
	ldr	ip, [r10, #48]
	blx	ip
	sub	r1, fp, #76
	mov	r0, r9
	mov	r2, #1
	str	r7, [fp, #-64]
	str	r7, [fp, #-48]
	strb	r6, [fp, #-75]
	str	r8, [fp, #-68]
	str	r8, [fp, #-52]
	strb	r6, [fp, #-73]
	strb	r6, [fp, #-71]
	strb	r6, [fp, #-70]
	strb	r2, [fp, #-74]
	bl	FSP_ConfigInstance
	subs	r7, r0, #0
	bne	.L278
	ldr	r3, [fp, #-88]
	mov	r0, r9
	ldr	r2, [fp, #4]
	mov	r1, r3
	sub	r3, fp, #80
	bl	FSP_ConfigFrameBuf
	subs	r6, r0, #0
	bne	.L289
	mov	r0, r9
	add	r1, r4, #528
	bl	FSP_GetInstInfo
	movw	r3, #529
	ldrsb	r3, [r4, r3]
	cmp	r3, #16
	movge	r3, #16
	cmp	r3, #2
	str	r3, [r5, #524]
	ble	.L290
	ldr	ip, [r4, #3736]
	mov	r2, #64
	ldr	r3, [r10, #48]
	mov	r1, r7
	add	r0, r5, #456
	str	ip, [r5, #520]
	blx	r3
	mov	r1, r7
	ldr	r3, [r10, #48]
	mov	r2, #64
	add	r0, r5, #392
	blx	r3
	ldr	r1, [r5, #524]
	cmp	r1, #0
	ble	.L276
	add	r4, r4, #528
	add	r5, r5, #452
	mov	r3, r7
.L283:
	ldr	r2, [r4, #24]
	add	r3, r3, #1
	cmp	r3, r1
	add	r4, r4, #100
	str	r2, [r5, #4]!
	bne	.L283
.L276:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L288:
	mov	r0, r2
	ldr	r1, .L291+8
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L276
.L278:
	mov	r0, r6
	ldr	r1, .L291+12
	bl	dprint_vfmw
.L286:
	mvn	r7, #0
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L289:
	mov	r0, r7
	ldr	r1, .L291+16
	bl	dprint_vfmw
	b	.L286
.L290:
	mov	r0, r6
	movw	r3, #1697
	ldr	r2, .L291+20
	ldr	r1, .L291+24
	bl	dprint_vfmw
	b	.L286
.L287:
	mov	r3, r7
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L291+28
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L276
.L292:
	.align	2
.L291:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC25
	.word	.LC28
	.word	.LC26
	.word	.LANCHOR1+40
	.word	.LC27
	.word	.LC24
	UNWIND(.fnend)
	.size	VCTRL_PartitionMemForStdUser, .-VCTRL_PartitionMemForStdUser
	.align	2
	.global	VCTRL_UnInstallChan
	.type	VCTRL_UnInstallChan, %function
VCTRL_UnInstallChan:
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
	bl	FSP_DisableInstance
	mov	r0, r4
	bl	CloseUsdPool_WithMem
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_UnInstallChan, .-VCTRL_UnInstallChan
	.align	2
	.global	DelAllFrameMemRecord
	.type	DelAllFrameMemRecord, %function
DelAllFrameMemRecord:
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
	cmp	r1, #0
	movne	r4, r1
	addne	r5, r4, #2304
	beq	.L311
.L300:
	ldr	r3, [r4]
	cmp	r3, #255
	beq	.L297
	ldr	r2, [r4, #12]
	ldr	r1, [r4, #24]
	ldr	r0, [r4, #32]
	bl	MEM_DelMemRecord
.L297:
	ldr	r3, [r4, #4]
	cmp	r3, #255
	beq	.L298
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #40]
	ldr	r0, [r4, #48]
	bl	MEM_DelMemRecord
.L298:
	ldr	r3, [r4, #8]
	cmp	r3, #255
	beq	.L299
	ldr	r2, [r4, #20]
	ldr	r1, [r4, #56]
	ldr	r0, [r4, #64]
	bl	MEM_DelMemRecord
.L299:
	add	r4, r4, #72
	cmp	r4, r5
	bne	.L300
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L311:
	mov	r0, r1
	ldr	r3, .L312
	movw	r2, #1754
	ldr	r1, .L312+4
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L313:
	.align	2
.L312:
	.word	.LC29
	.word	.LC1
	UNWIND(.fnend)
	.size	DelAllFrameMemRecord, .-DelAllFrameMemRecord
	.align	2
	.global	VCTRL_FreeChanResource
	.type	VCTRL_FreeChanResource, %function
VCTRL_FreeChanResource:
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
	subs	r5, r1, #0
	beq	.L338
	ldr	r4, [r5]
	cmp	r4, #0
	beq	.L339
	ldr	r3, [r5, #16]
	cmp	r3, #1
	beq	.L319
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_DelMemRecord
.L320:
	ldr	r3, [r5, #20]
	cmp	r3, #1
	beq	.L340
.L318:
	ldr	r4, [r5, #4]
	cmp	r4, #0
	beq	.L341
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_DelMemRecord
	ldr	r3, [r5, #24]
	cmp	r3, #1
	beq	.L342
.L323:
	ldr	r4, [r5, #8]
	cmp	r4, #0
	beq	.L343
.L324:
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_DelMemRecord
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne	.L325
	ldr	r0, [r4, #8]
	cmp	r0, #0
	bne	.L344
.L325:
	ldr	r3, .L346
	ldr	r0, [r4, #4]
	ldr	r3, [r3, #160]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L319:
	ldr	r1, [r5, #12]
	cmp	r1, #0
	beq	.L345
	bl	DelAllFrameMemRecord
	cmp	r0, #0
	beq	.L320
	ldr	r1, .L346+4
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r5, #20]
	cmp	r3, #1
	bne	.L318
.L340:
	ldr	r0, [r4, #8]
	cmp	r0, #0
	beq	.L318
	ldr	r1, [r4, #4]
	bl	MEM_ReleaseMemBlock
	b	.L318
.L342:
	ldr	r0, [r4, #8]
	cmp	r0, #0
	beq	.L323
	ldr	r1, [r4, #4]
	bl	MEM_ReleaseMemBlock
	b	.L323
.L341:
	mov	r0, r4
	ldr	r2, .L346+8
	ldr	r1, .L346+12
	bl	dprint_vfmw
	ldr	r4, [r5, #8]
	cmp	r4, #0
	bne	.L324
.L343:
	ldr	r2, .L346+8
	mov	r0, r4
	ldr	r1, .L346+16
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L344:
	ldr	r1, [r4, #4]
	bl	MEM_ReleaseMemBlock
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L345:
	mov	r0, r1
	ldr	r2, .L346+8
	ldr	r1, .L346+20
	bl	dprint_vfmw
	b	.L320
.L339:
	mov	r0, r4
	ldr	r2, .L346+8
	ldr	r1, .L346+24
	bl	dprint_vfmw
	b	.L318
.L338:
	mov	r0, r5
	ldr	r2, .L346+8
	ldr	r1, .L346+28
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L347:
	.align	2
.L346:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC33
	.word	.LANCHOR1+72
	.word	.LC34
	.word	.LC35
	.word	.LC32
	.word	.LC31
	.word	.LC30
	UNWIND(.fnend)
	.size	VCTRL_FreeChanResource, .-VCTRL_FreeChanResource
	.align	2
	.global	VCTRL_GetChanCtxSize
	.type	VCTRL_GetChanCtxSize, %function
VCTRL_GetChanCtxSize:
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
	cmp	r0, #26
	movweq	r3, #62864
	movwne	r3, #10608
	movteq	r3, 177
	movtne	r3, 12
	str	r3, [r1]
	ldmfd	sp, {fp, sp, pc}
	UNWIND(.fnend)
	.size	VCTRL_GetChanCtxSize, .-VCTRL_GetChanCtxSize
	.align	2
	.global	VCTRL_GetChanCfg
	.type	VCTRL_GetChanCfg, %function
VCTRL_GetChanCfg:
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
	cmp	r0, #15
	mov	r3, r1
	bhi	.L356
	cmp	r1, #0
	beq	.L357
	ldr	r2, .L359
	ldr	r1, [r2, r0, asl #2]
	cmp	r1, #0
	beq	.L358
	ldr	ip, .L359+4
	add	r1, r1, #52
	mov	r0, r3
	mov	r2, #120
	ldr	r3, [ip, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L356:
	ldr	r3, .L359+8
	movw	r2, #3052
	ldr	r1, .L359+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L358:
	mov	r0, r1
	ldr	r3, .L359+16
	movw	r2, #3054
	ldr	r1, .L359+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L357:
	mov	r0, r1
	ldr	r3, .L359+20
	movw	r2, #3053
	ldr	r1, .L359+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L360:
	.align	2
.L359:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
	.word	.LC1
	.word	.LC2
	.word	.LC36
	UNWIND(.fnend)
	.size	VCTRL_GetChanCfg, .-VCTRL_GetChanCfg
	.align	2
	.global	VCTRL_CmpConfigParameter
	.type	VCTRL_CmpConfigParameter, %function
VCTRL_CmpConfigParameter:
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
	ldr	r3, .L380
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #600]
	cmp	r2, #21
	beq	.L377
	ldr	r2, [r3, #52]
	ldr	r0, [r1]
	cmp	r2, r0
	bne	.L377
	ldr	ip, [r3, #72]
	ldr	r0, [r1, #20]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #68]
	ldr	r0, [r1, #16]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #76]
	ldr	r0, [r1, #24]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #80]
	ldr	r0, [r1, #28]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #84]
	ldr	r0, [r1, #32]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #88]
	ldr	r0, [r1, #36]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #112]
	ldr	r0, [r1, #60]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #128]
	ldr	r0, [r1, #76]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #124]
	ldr	r0, [r1, #72]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #116]
	ldr	r0, [r1, #64]
	cmp	ip, r0
	bne	.L377
	ldr	ip, [r3, #132]
	ldr	r0, [r1, #80]
	cmp	ip, r0
	bne	.L377
	cmp	r2, #1
	beq	.L379
	sub	r2, r2, #10
	cmp	r2, #2
	bhi	.L378
	ldr	r3, [r3, #56]
	ldr	r0, [r1, #4]
	subs	r0, r3, r0
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L377:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L378:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L379:
	ldr	r0, [r3, #56]
	ldr	r2, [r1, #4]
	cmp	r0, r2
	bne	.L377
	ldr	r3, [r3, #60]
	ldr	r0, [r1, #8]
	subs	r0, r3, r0
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L381:
	.align	2
.L380:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_CmpConfigParameter, .-VCTRL_CmpConfigParameter
	.align	2
	.global	VCTRL_CheckChanWH
	.type	VCTRL_CheckChanWH, %function
VCTRL_CheckChanWH:
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
	ldr	r3, .L387
	ldr	r2, .L387+4
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r2]
	ldr	r0, [r3, #1276]
	ldr	r3, [r3, #1280]
	cmp	r0, r2
	bgt	.L383
	ldr	r0, .L387+8
	ldr	r0, [r0]
	cmp	r3, r0
	ble	.L385
.L383:
	ldr	r0, [r1]
	cmp	r0, #20
	cmpne	r0, #0
	bic	r0, r0, #2
	movne	r3, #1
	moveq	r3, #0
	cmp	r0, #13
	moveq	r0, #0
	andne	r0, r3, #1
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	r3, .L387+8
	mov	r0, #0
	ldr	r1, .L387+12
	ldr	r3, [r3]
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L385:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L388:
	.align	2
.L387:
	.word	.LANCHOR0
	.word	g_ChipMaxWidth
	.word	g_ChipMaxHeight
	.word	.LC37
	UNWIND(.fnend)
	.size	VCTRL_CheckChanWH, .-VCTRL_CheckChanWH
	.align	2
	.global	VCTRL_SeekPts
	.type	VCTRL_SeekPts, %function
VCTRL_SeekPts:
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
	ldr	r2, .L397
	ldr	r3, [r2, r0, asl #2]
	cmp	r3, #0
	beq	.L395
	ldr	r3, [r3, #600]
	sub	ip, r3, #10
	cmp	ip, #2
	bls	.L396
.L392:
	cmp	r3, #14
	cmpne	r3, #4
	beq	.L394
	bl	SM_SeekPts
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
.L396:
	ldr	r2, [r2, #3740]
	tst	r2, #1
	bne	.L392
.L394:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L395:
	mov	r0, r3
	movw	r2, #3740
	ldr	r3, .L397+4
	ldr	r1, .L397+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L398:
	.align	2
.L397:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_SeekPts, .-VCTRL_SeekPts
	.align	2
	.global	VCTRL_ReleaseStream
	.type	VCTRL_ReleaseStream, %function
VCTRL_ReleaseStream:
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
	ldr	r3, .L403
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L402
	bl	SM_Reset
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
.L402:
	mov	r0, r3
	movw	r2, #4170
	ldr	r3, .L403+4
	ldr	r1, .L403+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L404:
	.align	2
.L403:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ReleaseStream, .-VCTRL_ReleaseStream
	.align	2
	.global	VCTRL_ImageOutputEnable
	.type	VCTRL_ImageOutputEnable, %function
VCTRL_ImageOutputEnable:
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
	cmp	r0, #15
	mov	r4, r0
	mov	r6, r1
	bhi	.L408
	ldr	r5, .L409
	mov	r0, #4
	ldr	r3, [r5, #12]
	blx	r3
	ldr	r2, .L409+4
	ldr	r1, [r6]
	mov	r0, #4
	ldr	r3, [r5, #16]
	ldr	r2, [r2, r4, asl #2]
	str	r1, [r2, #20]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L408:
	ldr	r3, .L409+8
	movw	r2, #4283
	ldr	r1, .L409+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L410:
	.align	2
.L409:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ImageOutputEnable, .-VCTRL_ImageOutputEnable
	.align	2
	.global	VCTRL_TryToRelSpecialFrm
	.type	VCTRL_TryToRelSpecialFrm, %function
VCTRL_TryToRelSpecialFrm:
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
	cmp	r0, #15
	mov	r6, r0
	bhi	.L437
	ldr	r7, .L441
	ldr	r0, [r7, r0, asl #2]
	cmp	r0, #0
	beq	.L438
	add	r3, r0, #294912
	ldr	lr, [r3, #1912]
	cmp	lr, #0
	beq	.L426
	ldr	r3, [r3, #1404]
	cmp	r1, r3
	movne	r3, #0
	bne	.L417
	b	.L439
.L419:
	ldr	r2, [r0, r2]
	cmp	r1, r2
	beq	.L415
.L417:
	add	r3, r3, #1
	movw	r2, #34172
	cmp	r3, lr
	movt	r2, 4
	mov	ip, r3, asl #4
	mov	r5, r3
	add	r2, ip, r2
	bne	.L419
.L426:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L439:
	mov	r5, #0
.L415:
	add	r2, r0, #262144
	movw	r3, #1954
	ldrsb	ip, [r2, r3]
	cmp	ip, #0
	ble	.L426
	add	r3, r0, #282624
	ldr	r3, [r3, #2884]
	cmp	r3, r1
	movwne	r3, #23464
	movne	r4, #0
	movtne	r3, 4
	addne	r3, r0, r3
	bne	.L422
	b	.L440
.L425:
	ldr	r2, [r3], #100
	cmp	r2, r1
	beq	.L420
.L422:
	add	r4, r4, #1
	cmp	r4, ip
	bne	.L425
	b	.L426
.L440:
	mov	r4, #0
.L420:
	ldr	r9, .L441+4
	mov	r0, #3
	sub	r8, lr, #1
	ldr	r3, [r9, #12]
	blx	r3
	ldr	ip, [r7, r6, asl #2]
	mov	r3, #100
	mov	r2, #0
	ldr	r1, [r9, #16]
	mov	r0, #3
	mla	r4, r3, r4, ip
	add	r3, r4, #282624
	add	r4, r4, #286720
	strb	r2, [r3, #2870]
	strb	r2, [r4, #1974]
	blx	r1
	cmp	r8, r5
	bls	.L429
	mov	r4, r5, asl #4
.L424:
	add	r5, r5, #1
	movw	r0, #34168
	ldr	r1, [r7, r6, asl #2]
	movt	r0, 4
	movw	r3, #34168
	add	r0, r4, r0
	mov	r4, r5, asl #4
	movt	r3, 4
	add	r3, r4, r3
	add	r0, r1, r0
	mov	r2, #16
	add	r1, r1, r3
	ldr	r3, [r9, #52]
	blx	r3
	cmp	r8, r5
	bhi	.L424
.L423:
	mov	lr, r8, asl #4
	ldr	r0, [r7, r6, asl #2]
	movw	r3, #34168
	ldr	ip, [r9, #48]
	movt	r3, 4
	add	r3, lr, r3
	add	r0, r0, r3
	mov	r2, #16
	mov	r1, #0
	blx	ip
	ldr	r3, [r7, r6, asl #2]
	mov	r0, #0
	add	r3, r3, #294912
	ldr	r2, [r3, #1912]
	sub	r2, r2, #1
	str	r2, [r3, #1912]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L429:
	mov	r8, r5
	b	.L423
.L437:
	ldr	r3, .L441+8
	movw	r2, #4401
	ldr	r1, .L441+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L438:
	ldr	r3, .L441+8
	movw	r2, #4402
	ldr	r1, .L441+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L442:
	.align	2
.L441:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_TryToRelSpecialFrm, .-VCTRL_TryToRelSpecialFrm
	.align	2
	.global	VCTRL_ReleaseChanImage
	.type	VCTRL_ReleaseChanImage, %function
VCTRL_ReleaseChanImage:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 624
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #628)
	sub	sp, sp, #628
	cmp	r0, #15
	mov	r4, r0
	mov	r7, r1
	bhi	.L464
	ldr	r8, .L472
	ldr	r0, [r8, r0, asl #2]
	cmp	r0, #0
	beq	.L465
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L466
	cmp	r1, #0
	beq	.L467
	ldr	r6, .L472+4
	mov	r2, r4
	ldr	r3, [r1, #156]
	mov	r0, #2
	ldr	r1, .L472+8
	bl	dprint_vfmw
	ldr	r3, [r6, #12]
	mov	r0, #4
	blx	r3
	ldr	r0, [r8, r4, asl #2]
	cmp	r0, #0
	movweq	r2, #4850
	ldreq	r1, .L472+12
	beq	.L463
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L468
	ldr	r1, [r7, #28]
	mov	r0, r4
	bl	VCTRL_TryToRelSpecialFrm
	subs	r5, r0, #0
	beq	.L469
	ldr	r3, [r8, r4, asl #2]
	movw	r0, #34728
	movt	r0, 4
	mov	r1, r7
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	ldr	r3, [r7, #196]
	cmn	r3, #1
	mov	r5, r0
	beq	.L452
	ldr	r2, [r8, r4, asl #2]
	movw	r0, #34728
	movt	r0, 4
	sub	r1, fp, #660
	add	r0, r2, r0
	str	r3, [fp, #-504]
	bl	SYNTAX_ReleaseImage
	mov	r5, r0
.L452:
	mov	r0, #13
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L470
.L454:
	subs	r4, r5, #1
	mvnne	r4, #0
	cmp	r4, #0
	bne	.L471
.L456:
	ldr	r3, [r6, #16]
	mov	r0, #4
	blx	r3
.L445:
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L469:
	ldr	r3, [r6, #16]
	mov	r0, #4
	blx	r3
	movw	r2, #4871
	ldr	r1, .L472+16
	mov	r0, r5
	bl	dprint_vfmw
	mov	r4, r5
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L471:
	ldr	r2, .L472+20
	mov	r0, #0
	ldr	r1, .L472+24
	bl	dprint_vfmw
	b	.L456
.L470:
	ldr	r3, [r6]
	add	r8, r8, r4, lsl #2
	blx	r3
	ldr	r2, .L472+28
	ldr	r3, [r8, #3744]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r2
	bls	.L454
	mov	r2, r4, asl #24
	movw	r1, #4906
	add	r2, r2, #851968
	ldr	r0, .L472+32
	cmp	r5, #1
	addeq	r2, r2, #1
	str	ip, [r8, #3744]
	bl	DBG_AddTrace
	b	.L454
.L468:
	ldr	r1, .L472+36
	movw	r2, #4859
	mov	r0, #0
.L463:
	bl	dprint_vfmw
	ldr	r3, [r6, #16]
	mov	r0, #4
	mvn	r4, #0
	blx	r3
	b	.L445
.L466:
	ldr	r3, .L472+40
	movw	r2, #4842
	ldr	r1, .L472+44
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r4, #0
	b	.L445
.L464:
	ldr	r3, .L472+48
	movw	r2, #4840
	ldr	r1, .L472+44
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r4, #0
	b	.L445
.L467:
	mov	r0, r1
	ldr	r3, .L472+52
	movw	r2, #4843
	ldr	r1, .L472+44
	bl	dprint_vfmw
	mvn	r4, #0
	b	.L445
.L465:
	ldr	r3, .L472+56
	movw	r2, #4841
	ldr	r1, .L472+44
	mvn	r4, #0
	bl	dprint_vfmw
	b	.L445
.L473:
	.align	2
.L472:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC40
	.word	.LC41
	.word	.LC43
	.word	.LANCHOR1+120
	.word	.LC44
	.word	g_TraceImgPeriod
	.word	.LANCHOR1+96
	.word	.LC42
	.word	.LC38
	.word	.LC1
	.word	.LC12
	.word	.LC39
	.word	.LC2
	UNWIND(.fnend)
	.size	VCTRL_ReleaseChanImage, .-VCTRL_ReleaseChanImage
	.align	2
	.global	VCTRL_GetVidStd
	.type	VCTRL_GetVidStd, %function
VCTRL_GetVidStd:
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
	cmp	r0, #15
	bhi	.L475
	ldr	r3, .L479
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L475
	ldr	r2, [r3, #12]
	cmp	r2, #1
	beq	.L478
	mov	r3, r0
	movw	r2, #4939
	ldr	r1, .L479+4
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #21
	ldmfd	sp, {fp, sp, pc}
.L475:
	movw	r2, #4932
	ldr	r1, .L479+8
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #21
	ldmfd	sp, {fp, sp, pc}
.L478:
	ldr	r0, [r3, #600]
	ldmfd	sp, {fp, sp, pc}
.L480:
	.align	2
.L479:
	.word	.LANCHOR0
	.word	.LC46
	.word	.LC45
	UNWIND(.fnend)
	.size	VCTRL_GetVidStd, .-VCTRL_GetVidStd
	.align	2
	.global	VCTRL_GetDecParam
	.type	VCTRL_GetDecParam, %function
VCTRL_GetDecParam:
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
	cmp	r0, #15
	bhi	.L482
	ldr	r2, .L510
	ldr	r4, [r2, r0, asl #2]
	cmp	r4, #0
	beq	.L482
	ldr	r3, [r4, #12]
	cmp	r3, #1
	bne	.L501
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L501
	ldr	r3, [r4, #600]
	cmp	r3, #17
	ldrls	pc, [pc, r3, asl #2]
	b	.L485
.L487:
	.word	.L486
	.word	.L509
	.word	.L489
	.word	.L509
	.word	.L485
	.word	.L509
	.word	.L492
	.word	.L485
	.word	.L509
	.word	.L509
	.word	.L495
	.word	.L495
	.word	.L495
	.word	.L509
	.word	.L485
	.word	.L497
	.word	.L498
	.word	.L509
.L495:
	ldr	r3, [r2, #3740]
	tst	r3, #1
	beq	.L501
.L509:
	add	r3, r4, #294912
	ldr	r5, [r3, #1972]
.L500:
	cmp	r5, #0
	moveq	r3, r5
	beq	.L508
	ldr	r3, .L510+4
	ldr	r3, [r3]
	blx	r3
	mov	r3, r5
	str	r0, [r4, #48]
.L508:
	mov	r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L485:
	ldr	r2, .L510+8
	mov	r0, #0
	ldr	r1, .L510+12
	bl	dprint_vfmw
.L501:
	mov	r3, #0
	mov	r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L482:
	mov	r3, r0
	movw	r2, #4956
	ldr	r1, .L510+16
	mov	r0, #22
	bl	dprint_vfmw
	mov	r3, #0
	b	.L508
.L486:
	add	r3, r4, #294912
	ldr	r5, [r3, #2328]
	b	.L500
.L492:
	add	r3, r4, #294912
	ldr	r5, [r3, #1984]
	b	.L500
.L497:
	add	r3, r4, #294912
	ldr	r5, [r3, #2196]
	b	.L500
.L498:
	add	r3, r4, #299008
	ldr	r5, [r3, #200]
	b	.L500
.L489:
	add	r3, r4, #294912
	ldr	r5, [r3, #3004]
	b	.L500
.L511:
	.align	2
.L510:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+144
	.word	.LC48
	.word	.LC47
	UNWIND(.fnend)
	.size	VCTRL_GetDecParam, .-VCTRL_GetDecParam
	.align	2
	.global	VCTRL_IsSedOnlyMode
	.type	VCTRL_IsSedOnlyMode, %function
VCTRL_IsSedOnlyMode:
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
	subs	r0, r1, #0
	beq	.L514
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L514:
	ldr	r1, .L515
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L516:
	.align	2
.L515:
	.word	.LC49
	UNWIND(.fnend)
	.size	VCTRL_IsSedOnlyMode, .-VCTRL_IsSedOnlyMode
	.align	2
	.global	VCTRL_ChanDecparamInValidFlag
	.type	VCTRL_ChanDecparamInValidFlag, %function
VCTRL_ChanDecparamInValidFlag:
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
	cmp	r0, #15
	mov	r2, r0
	bhi	.L518
	ldr	r5, .L529
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L518
	ldr	r1, [r3, #12]
	cmp	r1, #1
	bne	.L521
	ldr	r6, [r3, #16]
	cmp	r6, #1
	beq	.L522
.L521:
	ldr	r1, .L529+4
	mov	r0, #22
	mov	r4, #0
	bl	dprint_vfmw
.L520:
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L518:
	mov	r3, r2
	ldr	r1, .L529+8
	mov	r2, #5120
	mov	r0, #22
	bl	dprint_vfmw
	mov	r4, #0
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L522:
	str	r0, [fp, #-40]
	bl	VCTRL_GetDecParam
	ldr	r2, [fp, #-40]
	subs	r7, r0, #0
	beq	.L527
	ldr	r2, [r5, r2, asl #2]
	mov	r3, #0
	mov	r4, r3
	str	r3, [r2, #384]
	str	r3, [r2, #388]
	b	.L520
.L527:
	ldr	r8, [r5, r2, asl #2]
	ldr	r4, [r8, #384]
	cmp	r4, #0
	beq	.L528
	ldr	r3, .L529+12
	str	r2, [fp, #-40]
	ldr	r3, [r3]
	blx	r3
	ldr	r2, [fp, #-40]
	ldr	r3, [r5, r2, asl #2]
	ldr	r2, [r3, #388]
	cmp	r0, r2
	bls	.L525
	rsb	r0, r2, r0
	cmp	r0, #40
	movls	r4, r7
	bls	.L520
.L525:
	str	r7, [r3, #384]
	mov	r4, r6
	str	r7, [r3, #388]
	b	.L520
.L528:
	ldr	r3, .L529+12
	str	r6, [r8, #384]
	ldr	r3, [r3]
	blx	r3
	str	r0, [r8, #388]
	b	.L520
.L530:
	.align	2
.L529:
	.word	.LANCHOR0
	.word	.LC50
	.word	.LC47
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_ChanDecparamInValidFlag, .-VCTRL_ChanDecparamInValidFlag
	.align	2
	.global	VCTRL_VdmProcSedOnly
	.type	VCTRL_VdmProcSedOnly, %function
VCTRL_VdmProcSedOnly:
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
	cmp	r0, #15
	mov	r3, r0
	movwhi	r2, #5390
	ldrhi	r3, .L537
	bhi	.L535
	ldr	r2, .L537+4
	ldr	r3, [r2, r3, asl #2]
	cmp	r3, #0
	beq	.L536
	ldr	r3, [r3, #16]
	cmp	r3, #1
	beq	.L534
	ldr	r3, .L537+8
	movw	r2, #5392
.L535:
	ldr	r1, .L537+12
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L534:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_GetDecParam
.L536:
	mov	r0, r3
	movw	r2, #5391
	ldr	r3, .L537+16
	ldr	r1, .L537+12
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L538:
	.align	2
.L537:
	.word	.LC51
	.word	.LANCHOR0
	.word	.LC52
	.word	.LC1
	.word	.LC38
	UNWIND(.fnend)
	.size	VCTRL_VdmProcSedOnly, .-VCTRL_VdmProcSedOnly
	.align	2
	.global	VCTRL_FlushDecoder
	.type	VCTRL_FlushDecoder, %function
VCTRL_FlushDecoder:
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
	ldr	r4, .L569
	mov	r5, r0
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L566
	add	r0, r0, #294912
	ldr	r2, [r0, #1960]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L559
.L543:
	.word	.L542
	.word	.L544
	.word	.L545
	.word	.L546
	.word	.L559
	.word	.L547
	.word	.L548
	.word	.L559
	.word	.L549
	.word	.L550
	.word	.L551
	.word	.L551
	.word	.L551
	.word	.L552
	.word	.L559
	.word	.L553
	.word	.L554
	.word	.L555
.L555:
	add	r0, r0, #1968
	bl	VP9DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L542:
	add	r0, r0, #1968
	bl	H264_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	r0, [r4, r5, asl #2]
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	FlushDPB
	cmp	r0, #0
	bne	.L567
.L559:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L545:
	add	r0, r0, #1968
	bl	MPEG4DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L546:
	add	r0, r0, #1968
	bl	MPEG2DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L544:
	add	r0, r0, #1968
	bl	VC1DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L547:
	add	r0, r0, #1968
	bl	DIVX3DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L548:
	add	r0, r0, #1968
	bl	AVSDEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L549:
	add	r0, r0, #1968
	bl	REAL8DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L550:
	add	r0, r0, #1968
	bl	REAL9DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L551:
	add	r0, r0, #1968
	bl	VP6DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L552:
	add	r0, r0, #1968
	bl	VP8DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L553:
	add	r0, r0, #1968
	bl	MVC_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	r0, [r4, r5, asl #2]
	mvn	r1, #0
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	MVC_FlushDPB
	cmp	r0, #0
	beq	.L559
	movw	r3, #5481
	ldr	r2, .L569+4
	ldr	r1, .L569+8
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L554:
	add	r0, r0, #1968
	bl	HEVC_FlushOutputFrmFromDPB
	cmp	r0, #0
	bne	.L568
.L557:
	ldr	r1, .L569+12
	mov	r0, #32
	bl	dprint_vfmw
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L568:
	movw	r2, #5458
	ldr	r1, .L569+16
	mov	r0, #0
	bl	dprint_vfmw
	b	.L557
.L567:
	movw	r2, #5448
	ldr	r1, .L569+16
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L566:
	ldr	r3, .L569+20
	movw	r2, #5422
	ldr	r1, .L569+24
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L570:
	.align	2
.L569:
	.word	.LANCHOR0
	.word	.LANCHOR1+164
	.word	.LC55
	.word	.LC54
	.word	.LC53
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_FlushDecoder, .-VCTRL_FlushDecoder
	.align	2
	.global	VCTRL_GetImageBuffer
	.type	VCTRL_GetImageBuffer, %function
VCTRL_GetImageBuffer:
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
	cmp	r0, #15
	bhi	.L594
	ldr	r1, .L597
	ldr	r3, [r1, r0, asl #2]
	cmp	r3, #0
	beq	.L595
	ldr	r0, [r3, #16]
	cmp	r0, #1
	bne	.L596
	ldr	r2, [r3, #600]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L590
.L577:
	.word	.L576
	.word	.L578
	.word	.L579
	.word	.L580
	.word	.L590
	.word	.L581
	.word	.L582
	.word	.L590
	.word	.L583
	.word	.L584
	.word	.L585
	.word	.L585
	.word	.L585
	.word	.L586
	.word	.L590
	.word	.L587
	.word	.L588
	.word	.L589
.L589:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP9DEC_GetImageBuffer
.L576:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageBuffer
.L578:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_GetImageBuffer
.L579:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageBuffer
.L580:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_GetImageBuffer
.L581:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_GetImageBuffer
.L582:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_GetImageBuffer
.L583:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_GetImageBuffer
.L584:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_GetImageBuffer
.L585:
	ldr	r2, [r1, #3740]
	tst	r2, #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_GetImageBuffer
.L586:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_GetImageBuffer
.L587:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_GetImageBuffer
.L588:
	add	r0, r3, #294912
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageBuffer
.L590:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L596:
	ldr	r3, .L597+4
	movw	r2, #5546
	ldr	r1, .L597+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L594:
	ldr	r3, .L597+12
	movw	r2, #5544
	ldr	r1, .L597+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L595:
	mov	r0, r3
	movw	r2, #5545
	ldr	r3, .L597+16
	ldr	r1, .L597+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L598:
	.align	2
.L597:
	.word	.LANCHOR0
	.word	.LC52
	.word	.LC1
	.word	.LC51
	.word	.LC38
	UNWIND(.fnend)
	.size	VCTRL_GetImageBuffer, .-VCTRL_GetImageBuffer
	.align	2
	.global	VCTRL_ConfigFFFBSpeed
	.type	VCTRL_ConfigFFFBSpeed, %function
VCTRL_ConfigFFFBSpeed:
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
	cmp	r0, #15
	bhi	.L605
	ldr	r3, .L609
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L606
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L607
	cmp	r1, #0
	beq	.L608
	ldr	r2, [r1]
	mov	r0, #0
	str	r2, [r3, #1212]
	ldmfd	sp, {fp, sp, pc}
.L607:
	ldr	r3, .L609+4
	movw	r2, #5664
	ldr	r1, .L609+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L605:
	ldr	r3, .L609+12
	movw	r2, #5662
	ldr	r1, .L609+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L606:
	mov	r0, r3
	movw	r2, #5663
	ldr	r3, .L609+16
	ldr	r1, .L609+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L608:
	mov	r0, r1
	ldr	r3, .L609+20
	movw	r2, #5665
	ldr	r1, .L609+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L610:
	.align	2
.L609:
	.word	.LANCHOR0
	.word	.LC57
	.word	.LC1
	.word	.LC12
	.word	.LC56
	.word	.LC58
	UNWIND(.fnend)
	.size	VCTRL_ConfigFFFBSpeed, .-VCTRL_ConfigFFFBSpeed
	.align	2
	.global	VCTRL_ConfigPVRInfo
	.type	VCTRL_ConfigPVRInfo, %function
VCTRL_ConfigPVRInfo:
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
	cmp	r0, #15
	bhi	.L617
	ldr	r3, .L621
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L618
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L619
	cmp	r1, #0
	beq	.L620
	ldr	r3, .L621+4
	add	r0, r0, #1232
	mov	r2, #20
	add	r0, r0, #12
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L619:
	ldr	r3, .L621+8
	movw	r2, #5677
	ldr	r1, .L621+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L617:
	ldr	r3, .L621+16
	movw	r2, #5675
	ldr	r1, .L621+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L620:
	mov	r0, r1
	ldr	r3, .L621+20
	movw	r2, #5678
	ldr	r1, .L621+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L618:
	ldr	r3, .L621+24
	movw	r2, #5676
	ldr	r1, .L621+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L622:
	.align	2
.L621:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC57
	.word	.LC1
	.word	.LC12
	.word	.LC58
	.word	.LC56
	UNWIND(.fnend)
	.size	VCTRL_ConfigPVRInfo, .-VCTRL_ConfigPVRInfo
	.align	2
	.global	VCTRL_IsChanDecable
	.type	VCTRL_IsChanDecable, %function
VCTRL_IsChanDecable:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #24)
	sub	sp, sp, #24
	ldr	r5, .L629
	mov	r4, r0
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L626
	sub	r1, fp, #44
	bl	SM_GetInfo
	cmp	r0, #0
	bne	.L626
	ldr	r3, [fp, #-36]
	cmp	r3, #16
	bgt	.L627
	ldr	r3, [fp, #-28]
	cmp	r3, #524288
	ble	.L626
.L627:
	mov	r0, r4
	bl	VCTRL_GetImageBuffer
	subs	r0, r0, #1
	mvnne	r0, #0
	cmp	r0, #0
	bne	.L625
	ldr	r3, .L629+4
	ldr	r3, [r3]
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	ldr	r3, [r3, #48]
	rsb	r0, r3, r0
	cmp	r0, #50
	movls	r0, #0
	mvnhi	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L626:
	mvn	r0, #0
.L625:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L630:
	.align	2
.L629:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_IsChanDecable, .-VCTRL_IsChanDecable
	.align	2
	.global	VCTRL_IsChanSegEnough
	.type	VCTRL_IsChanSegEnough, %function
VCTRL_IsChanSegEnough:
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
	ldr	r3, .L632
	ldr	r3, [r3, r0, asl #2]
	ldr	r0, [r3, #372]
	subs	r0, r0, #1
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L633:
	.align	2
.L632:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_IsChanSegEnough, .-VCTRL_IsChanSegEnough
	.align	2
	.global	VCTRL_IsChanActive
	.type	VCTRL_IsChanActive, %function
VCTRL_IsChanActive:
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
	cmp	r0, #15
	bhi	.L640
	ldr	r3, .L641
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L640
	ldr	r2, [r3, #12]
	cmp	r2, #0
	beq	.L640
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq	.L640
	ldr	r2, [r3, #28]
	cmp	r2, #0
	beq	.L640
	ldr	r0, [r3, #600]
	cmp	r0, #21
	movne	r0, #0
	mvneq	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L640:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L642:
	.align	2
.L641:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_IsChanActive, .-VCTRL_IsChanActive
	.align	2
	.global	USERDEC_Init
	.type	USERDEC_Init, %function
USERDEC_Init:
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
	ldr	r3, .L660
	mov	r4, r0
	ldr	r6, [r0, #352]
	mov	r2, #368
	mov	r5, r1
	mov	r1, #0
	ldr	r3, [r3, #48]
	blx	r3
	str	r6, [r4, #352]
	add	r0, r4, #8
	bl	ResetVoQueue
	ldr	ip, .L660+4
	mov	r2, #0
	str	r5, [r4]
.L647:
	ldr	r1, [ip], #4
	cmp	r1, #0
	add	r3, r1, #294912
	add	r3, r3, #1968
	beq	.L644
	cmp	r4, r3
	beq	.L659
.L644:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L647
	mvn	r3, #0
	str	r3, [r4, #360]
.L645:
	ldr	r1, .L660+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L659:
	cmn	r2, #1
	str	r2, [r4, #360]
	beq	.L645
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L661:
	.align	2
.L660:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC59
	UNWIND(.fnend)
	.size	USERDEC_Init, .-USERDEC_Init
	.align	2
	.global	USERDEC_RecycleImage
	.type	USERDEC_RecycleImage, %function
USERDEC_RecycleImage:
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
	subs	r4, r0, #0
	mov	r5, r1
	beq	.L664
	mov	r2, #0
	ldr	r0, [r4, #360]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #360]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L665
	add	r5, r0, #576
	add	r8, r0, #560
	ldr	r1, [r0, #560]
	mov	r6, #0
	ldr	r0, [r4, #360]
	mov	r7, #0
	bl	FreeUsdByDec
	strd	r6, [r8]
	ldr	r0, [r4, #360]
	ldr	r1, [r5, #-8]
	bl	FreeUsdByDec
	strd	r6, [r5, #-8]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L665:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L664:
	mvn	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	UNWIND(.fnend)
	.size	USERDEC_RecycleImage, .-USERDEC_RecycleImage
	.align	2
	.global	VCTRL_GetUsrdecFrame
	.type	VCTRL_GetUsrdecFrame, %function
VCTRL_GetUsrdecFrame:
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
	ldr	r7, .L688
	mov	r6, r0
	mov	r5, r1
	ldr	r3, [r7, r0, asl #2]
	cmp	r3, #0
	beq	.L683
	ldr	r2, [r3, #600]
	cmp	r2, #20
	bne	.L684
	ldr	r3, [r3, #524]
	cmp	r3, #2
	ble	.L685
	mov	r1, #1
	bl	FSP_NewLogicFs
	subs	r8, r0, #0
	blt	.L686
	mov	r1, r8
	mov	r0, r6
	bl	FSP_GetLogicFs
	mov	r2, r8
	ldr	r1, .L688+4
	mov	r4, r0
	mov	r0, #18
	bl	dprint_vfmw
	cmp	r4, #0
	beq	.L687
	ldr	r2, [r4, #640]
	cmp	r2, #0
	beq	.L673
	ldr	r3, [r4, #644]
	cmp	r3, #0
	beq	.L673
	ldr	r1, [r4, #648]
	cmp	r1, #0
	beq	.L673
	ldr	ip, [r1, #16]
	mov	r0, #18
	ldr	r2, [r2, #16]
	ldr	r3, [r3, #16]
	ldr	r1, .L688+8
	str	ip, [sp]
	bl	dprint_vfmw
	ldr	r3, [r4, #640]
	ldr	r3, [r3, #16]
	str	r3, [r5, #40]
	ldr	r3, [r7, r6, asl #2]
	ldr	r3, [r3, #520]
	str	r3, [r5, #20]
	ldr	r3, [r4, #640]
	ldr	r3, [r3, #28]
	str	r3, [r5, #24]
	ldr	r3, [r4, #640]
	ldr	r0, [r3, #28]
	bl	MEM_Phy2Vir
	mov	r3, #0
	mov	r2, r3
	str	r3, [r5, #36]
	str	r0, [r5, #32]
.L668:
	mov	r0, r2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L684:
	ldr	r1, .L688+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L668
.L685:
	ldr	r1, .L688+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L668
.L673:
	ldr	r1, .L688+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L668
.L686:
	ldr	r1, .L688+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L668
.L683:
	mov	r0, r3
	movw	r2, #6018
	ldr	r3, .L688+28
	ldr	r1, .L688+32
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L668
.L687:
	movw	r2, #6048
	ldr	r1, .L688+36
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L668
.L689:
	.align	2
.L688:
	.word	.LANCHOR0
	.word	.LC63
	.word	.LC65
	.word	.LC60
	.word	.LC61
	.word	.LC66
	.word	.LC62
	.word	.LC2
	.word	.LC1
	.word	.LC64
	UNWIND(.fnend)
	.size	VCTRL_GetUsrdecFrame, .-VCTRL_GetUsrdecFrame
	.align	2
	.global	InitSyntaxCtx
	.type	InitSyntaxCtx, %function
InitSyntaxCtx:
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
	ldr	r3, .L709
	ldr	r1, [r3, r0, asl #2]
	cmp	r1, #0
	beq	.L708
	ldr	r3, [r1, #600]
	cmp	r3, #20
	ldrls	pc, [pc, r3, asl #2]
	b	.L690
.L694:
	.word	.L693
	.word	.L695
	.word	.L696
	.word	.L697
	.word	.L690
	.word	.L698
	.word	.L699
	.word	.L690
	.word	.L700
	.word	.L701
	.word	.L702
	.word	.L702
	.word	.L702
	.word	.L703
	.word	.L690
	.word	.L704
	.word	.L705
	.word	.L706
	.word	.L690
	.word	.L690
	.word	.L707
.L707:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	USERDEC_Init
.L693:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_Init
.L695:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_Init
.L696:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_Init
.L697:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_Init
.L698:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_Init
.L699:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_Init
.L700:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_Init
.L701:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_Init
.L702:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_Init
.L703:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_Init
.L704:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_Init
.L705:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_Init
.L706:
	add	r0, r1, #294912
	add	r1, r1, #608
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP9DEC_Init
.L690:
	ldmfd	sp, {fp, sp, pc}
.L708:
	mov	r0, r1
	ldr	r3, .L709+4
	movw	r2, #6103
	ldr	r1, .L709+8
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L710:
	.align	2
.L709:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	InitSyntaxCtx, .-InitSyntaxCtx
	.align	2
	.global	VCTRL_GetChanRpuSize
	.type	VCTRL_GetChanRpuSize, %function
VCTRL_GetChanRpuSize:
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
	ldr	r3, .L714
	mov	r4, r0
	mov	r5, r1
	ldr	r3, [r3, #160]
	cmp	r3, #0
	mvneq	r0, #0
	beq	.L712
	blx	r3
.L712:
	mul	r4, r4, r0
	str	r4, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L715:
	.align	2
.L714:
	.word	g_vdm_hal_fun_ptr
	UNWIND(.fnend)
	.size	VCTRL_GetChanRpuSize, .-VCTRL_GetChanRpuSize
	.align	2
	.global	VCTRL_GetChanMemSizeWithOption
	.type	VCTRL_GetChanMemSizeWithOption, %function
VCTRL_GetChanMemSizeWithOption:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	movw	ip, #10608
	cmp	r0, #26
	movw	r5, #62864
	movt	ip, 12
	movt	r5, 177
	movne	r5, ip
	cmp	r3, #2
	mov	r7, r0
	mov	r4, r1
	mov	r6, r2
	ldreq	r10, [r1, #36]
	beq	.L719
	ldr	r1, .L749
	ldr	r2, [r4, #36]
	ldr	r0, [r4, #60]
	ldr	ip, [r1, #3812]
	ldr	r3, [r1, #3808]
	add	r8, r0, ip
	add	r1, r2, r3
	orrs	lr, r8, r1
	bmi	.L743
	cmp	r1, #16
	strle	r1, [r4, #36]
	strle	r8, [r4, #60]
	movle	r10, r1
	bgt	.L744
.L719:
	cmp	r10, #0
	blt	.L724
.L723:
	ldr	r2, [r4, #12]
	cmp	r2, #31
	ble	.L724
	ldr	r3, [r4, #16]
	cmp	r3, #31
	ble	.L724
	ldr	r1, [r4, #56]
	cmp	r1, #262144
	blt	.L724
	ldr	r1, [r4, #44]
	cmp	r1, #1
	beq	.L745
	cmp	r1, #2
	beq	.L746
.L726:
	ldr	r1, [r4, #40]
	cmp	r1, #0
	moveq	r0, #1
	bne	.L747
.L728:
	ldr	ip, .L749+4
	sub	r8, r7, #27
	cmp	r8, #6
	ldr	r1, [r4, #60]
	ldr	ip, [ip, #64]
	mvncc	r9, #1
	mvncs	r9, #0
	cmp	ip, #0
	beq	.L730
	ldr	lr, .L749+8
	add	r1, r1, r10
	add	r1, r1, #2
	str	r9, [sp, #12]
	stmia	sp, {r0, r1}
	mov	r0, #0
	str	lr, [sp, #16]
	mvn	r1, #-2147483648
	str	r0, [sp, #8]
	mov	r0, #1
	blx	ip
	ldr	r9, .L749
	cmp	r0, #0
	bne	.L730
	ldr	r3, [r9, #3820]
	add	r2, r3, #16
	str	r2, [r6]
	ldr	r2, [r4, #176]
	cmp	r2, #1
	addne	r3, r3, #307200
	addne	r3, r3, #2576
	ldreq	r3, [r4, #56]
	strne	r3, [r6]
	ldrne	r3, [r4, #56]
	addeq	r3, r3, #339968
	addeq	r3, r3, #2576
	str	r5, [r6, #8]
	addne	r3, r3, #32768
	str	r3, [r6, #4]
	ldr	r3, [r4, #44]
	cmp	r3, #0
	bne	.L748
.L733:
	cmp	r8, #5
	bhi	.L734
	movw	r2, #38076
	movt	r2, 19
	cmp	r3, r2
	movcc	r3, r2
.L734:
	mov	r0, #0
	add	r3, r3, r5
	str	r3, [r6, #8]
.L721:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L747:
	add	r0, r10, #1
	cmp	r0, #17
	movge	r0, #17
	b	.L728
.L748:
	ldr	r2, [r4, #20]
	movw	r0, #4060
	ldr	lr, [r4, #28]
	mov	r3, #4000
	ldr	ip, [r4, #32]
	mov	r1, #2208
	mul	r2, r0, r2
	mla	r3, r3, lr, r2
	mla	r3, r1, ip, r3
	cmp	r3, #0
	movcc	r3, #0
	cmp	r7, #26
	bne	.L733
	movw	r0, #3992
	mov	r1, #2240
	mla	r2, r0, lr, r2
	mla	ip, r1, ip, r2
	cmp	r3, ip
	movcc	r3, ip
	b	.L733
.L746:
	ldr	r1, [r4, #20]
	cmp	r1, #0
	ble	.L724
	ldr	r1, [r4, #24]
	cmp	r1, #0
	ble	.L724
.L742:
	ldr	r1, [r4, #28]
	cmp	r1, #0
	ble	.L724
	ldr	r1, [r4, #32]
	cmp	r1, #0
	bgt	.L726
.L724:
	ldr	r1, .L749+12
	mov	r0, #32
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L730:
	movw	r2, #6390
	ldr	r1, .L749+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L745:
	ldr	r1, [r4, #20]
	cmp	r1, #0
	bgt	.L742
	b	.L724
.L744:
	ldr	r1, .L749+20
	mov	r0, #32
	bl	dprint_vfmw
	mov	r3, #16
	str	r8, [r4, #60]
	mov	r10, r3
	str	r3, [r4, #36]
	b	.L723
.L743:
	stmia	sp, {r0, ip}
	mov	r0, #32
	ldr	r1, .L749+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L721
.L750:
	.align	2
.L749:
	.word	.LANCHOR0
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0+3816
	.word	.LC69
	.word	.LC70
	.word	.LC68
	.word	.LC67
	UNWIND(.fnend)
	.size	VCTRL_GetChanMemSizeWithOption, .-VCTRL_GetChanMemSizeWithOption
	.align	2
	.global	VCTRL_GetChanMemSize
	.type	VCTRL_GetChanMemSize, %function
VCTRL_GetChanMemSize:
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
	UNWIND(.pad #60)
	sub	sp, sp, #60
	ldr	r10, .L776
	mov	r3, r0, asl #2
	mov	r5, r0, asl #5
	str	r3, [fp, #-68]
	rsb	r3, r3, r5
	add	r3, r10, r3
	str	r2, [fp, #-52]
	mov	r4, r0
	sub	r0, r0, #18
	ldr	r2, [r3, #16]
	str	r1, [fp, #-48]
	ldr	r9, [r3, #12]
	str	r2, [fp, #-60]
	ldr	r2, [r3, #20]
	ldr	ip, [r3, #8]
	str	r2, [fp, #-64]
	cmp	r0, #7
	ldrls	pc, [pc, r0, asl #2]
	b	.L752
.L754:
	.word	.L767
	.word	.L767
	.word	.L767
	.word	.L767
	.word	.L767
	.word	.L755
	.word	.L755
	.word	.L755
.L767:
	mov	r3, #3
.L753:
	ldr	r8, .L776+4
	rsb	r3, r3, r9
	sub	r3, r3, #2
	ldr	r2, [r8, #3808]
	cmp	r2, #0
	movlt	r2, #0
	strlt	r2, [r8, #3808]
	cmp	r3, #15
	movgt	r7, #0
	bgt	.L757
	ldr	r7, [r8, #3808]
	add	r3, r3, r7
	cmp	r3, #16
	rsbgt	r3, r3, #16
	addgt	r7, r7, r3
.L757:
	add	r3, ip, r7
	ldr	r6, [r8, #3812]
	cmp	r3, #0
	ble	.L774
	cmp	r3, #17
	movge	r3, #17
	str	r3, [fp, #-56]
.L759:
	cmp	r4, #23
	bhi	.L760
	ldr	r3, [fp, #-60]
	add	r6, r7, r6
	add	r2, r6, r9
	add	r1, r3, r6
	ldr	r3, [fp, #-64]
	cmp	r2, #0
	cmpgt	r1, #0
	add	r6, r3, r6
	movle	r3, #1
	movgt	r3, #0
	cmp	r6, #0
	orrle	r3, r3, #1
	cmp	r3, #0
	streq	r6, [fp, #-64]
	streq	r1, [fp, #-60]
	moveq	r9, r2
	bne	.L775
.L760:
	cmp	r4, #32
	bhi	.L770
	ldr	r6, .L776+8
	ldr	ip, [r6, #64]
	cmp	ip, #0
	beq	.L762
	ldr	r3, [fp, #-68]
	mov	r0, #0
	ldr	r1, [fp, #-56]
	rsb	r5, r3, r5
	ldr	r8, .L776+12
	add	r4, r10, r5
	str	r9, [sp, #4]
	ldr	r2, [r10, r5]
	mvn	r9, #0
	ldr	r3, [r4, #4]
	str	r1, [sp]
	mvn	r1, #-2147483648
	str	r0, [sp, #8]
	mov	r0, #1
	str	r8, [sp, #16]
	str	r9, [sp, #12]
	blx	ip
	ldr	r10, .L776
	ldr	r7, .L776+16
	cmp	r0, #0
	bne	.L762
	ldr	r3, [r7, #-3092]
	ldr	r2, [fp, #-48]
	add	r3, r3, #307200
	add	r3, r3, #2576
	str	r3, [r2]
	ldr	r2, [fp, #-52]
	ldr	r3, [r4, #24]
	add	r3, r3, #32768
	str	r3, [r2]
	ldr	ip, [r6, #64]
	cmp	ip, #0
	beq	.L764
	ldr	r3, [r4, #4]
	mvn	r1, #-2147483648
	ldr	lr, [fp, #-56]
	str	r0, [sp, #8]
	ldr	r0, [fp, #-60]
	str	lr, [sp]
	ldr	r2, [r10, r5]
	str	r0, [sp, #4]
	mov	r0, #1
	str	r8, [sp, #16]
	str	r9, [sp, #12]
	blx	ip
	subs	lr, r0, #0
	bne	.L764
	ldr	r3, [r7, #-3092]
	ldr	r2, [fp, #-48]
	add	r3, r3, #307200
	add	r3, r3, #2576
	str	r3, [r2, #4]
	ldr	r2, [fp, #-52]
	ldr	r3, [r4, #24]
	add	r3, r3, #32768
	str	r3, [r2, #4]
	ldr	ip, [r6, #64]
	cmp	ip, #0
	beq	.L766
	ldr	r3, [r4, #4]
	mvn	r1, #-2147483648
	str	lr, [sp, #8]
	mov	r0, #1
	ldr	lr, [fp, #-64]
	ldr	r2, [r10, r5]
	str	r8, [sp, #16]
	str	lr, [sp, #4]
	ldr	lr, [fp, #-56]
	str	r9, [sp, #12]
	str	lr, [sp]
	blx	ip
	cmp	r0, #0
	bne	.L766
	ldr	r2, [fp, #-48]
	ldr	r3, [r7, #-3092]
	add	r3, r3, #307200
	add	r3, r3, #2576
	str	r3, [r2, #8]
	ldr	r2, [fp, #-52]
	ldr	r3, [r4, #24]
	add	r3, r3, #32768
	str	r3, [r2, #8]
.L761:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L755:
	mov	r3, #1
	b	.L753
.L775:
	ldr	r1, [r8, #3812]
	mov	r3, r7
	ldr	r0, [fp, #-64]
	mov	r2, r9
	str	r7, [sp, #20]
	str	r1, [sp, #24]
	str	r1, [sp, #12]
	str	r1, [sp]
	ldr	r1, [fp, #-60]
	str	r0, [sp, #16]
	mov	r0, #32
	str	r7, [sp, #8]
	str	r1, [sp, #4]
	ldr	r1, .L776+20
	bl	dprint_vfmw
	b	.L760
.L752:
	mov	r3, #6
	b	.L753
.L762:
	movw	r2, #6562
	ldr	r1, .L776+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L764:
	movw	r2, #6586
	ldr	r1, .L776+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L766:
	movw	r2, #6610
	ldr	r1, .L776+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L774:
	mov	r3, r7
	mov	r2, ip
	ldr	r1, .L776+28
	mov	r0, #32
	str	ip, [fp, #-56]
	bl	dprint_vfmw
	ldr	ip, [fp, #-56]
	cmp	ip, #17
	movlt	r3, ip
	movge	r3, #17
	str	r3, [fp, #-56]
	b	.L759
.L770:
	mvn	r0, #0
	b	.L761
.L777:
	.align	2
.L776:
	.word	CapItem
	.word	.LANCHOR0
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR2-3096
	.word	.LANCHOR2
	.word	.LC72
	.word	.LC73
	.word	.LC71
	UNWIND(.fnend)
	.size	VCTRL_GetChanMemSize, .-VCTRL_GetChanMemSize
	.align	2
	.global	CloseDrivers
	.type	CloseDrivers, %function
CloseDrivers:
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
	ldr	r5, [r0, #72]
	mov	r4, r0
	cmp	r5, #0
	beq	.L779
	ldr	r5, [r0, #88]
	adds	r5, r5, #0
	movne	r5, #1
.L779:
	ldr	r6, [r4, #16]
	ldr	r7, [r4, #8]
	cmp	r6, #0
	ldr	r8, [r4, #4]
	beq	.L780
	ldr	r3, .L835
	ldr	r3, [r3, #60]
	cmp	r3, #0
	beq	.L781
	mov	r0, #0
	blx	r3
.L782:
	cmp	r5, #0
	beq	.L824
.L783:
	ldr	r3, .L835+4
	mov	r2, #24
	mov	r1, #0
	mov	r0, r4
	ldr	r3, [r3, #48]
	blx	r3
.L780:
	mov	r2, r6
	mov	r1, r8
	mov	r0, r7
	bl	MEM_DelMemRecord
	ldr	r6, [r4, #64]
	ldr	r7, [r4, #56]
	cmp	r6, #0
	ldr	r8, [r4, #52]
	beq	.L784
	ldr	r3, .L835
	ldr	r3, [r3, #60]
	cmp	r3, #0
	beq	.L785
	mov	r0, #0
	blx	r3
.L786:
	cmp	r5, #0
	beq	.L825
.L787:
	ldr	r3, .L835+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #48
	ldr	r3, [r3, #48]
	blx	r3
.L784:
	mov	r2, r6
	mov	r1, r8
	mov	r0, r7
	bl	MEM_DelMemRecord
	ldr	r6, [r4, #40]
	ldr	r7, [r4, #32]
	cmp	r6, #0
	ldr	r8, [r4, #28]
	bne	.L826
.L788:
	mov	r2, r6
	mov	r1, r8
	mov	r0, r7
	bl	MEM_DelMemRecord
	bl	SM_InstInit
	ldr	r5, [r4, #112]
	ldr	r6, [r4, #104]
	cmp	r5, #0
	ldr	r7, [r4, #100]
	bne	.L827
.L791:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #184]
	ldr	r6, [r4, #176]
	cmp	r5, #0
	ldr	r7, [r4, #172]
	bne	.L828
.L792:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #160]
	ldr	r6, [r4, #152]
	cmp	r5, #0
	ldr	r7, [r4, #148]
	bne	.L829
.L793:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #136]
	ldr	r6, [r4, #128]
	cmp	r5, #0
	ldr	r7, [r4, #124]
	bne	.L830
.L794:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #208]
	ldr	r6, [r4, #200]
	cmp	r5, #0
	ldr	r7, [r4, #196]
	bne	.L831
.L795:
	ldr	r4, .L835+8
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r3, [r4, #312]
	cmp	r3, #1
	beq	.L832
	ldr	r3, [r4, #316]
	ldr	r5, .L835+8
	cmp	r3, #1
	beq	.L833
.L797:
	ldr	r3, [r4, #320]
	ldr	r4, .L835+8
	cmp	r3, #1
	beq	.L834
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L826:
	mov	r0, #0
	bl	SM_CloseSCDDrv
	cmp	r5, #0
	beq	.L789
.L790:
	ldr	r3, .L835+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, r2
	ldr	r3, [r3, #48]
	blx	r3
	b	.L788
.L827:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L835+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #96
	ldr	r3, [r3, #48]
	blx	r3
	b	.L791
.L831:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L835+4
	add	r0, r4, #192
	mov	r2, #24
	mov	r1, #0
	ldr	r3, [r3, #48]
	blx	r3
	b	.L795
.L830:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L835+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #120
	ldr	r3, [r3, #48]
	blx	r3
	b	.L794
.L829:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L835+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #144
	ldr	r3, [r3, #48]
	blx	r3
	b	.L793
.L828:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L835+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #168
	ldr	r3, [r3, #48]
	blx	r3
	b	.L792
.L781:
	mov	r0, r3
	movw	r2, #7098
	ldr	r1, .L835+12
	bl	dprint_vfmw
	b	.L782
.L785:
	mov	r0, r3
	movw	r2, #7119
	ldr	r1, .L835+12
	bl	dprint_vfmw
	b	.L786
.L834:
	ldr	r3, .L835+4
	mov	r1, #0
	mov	r0, #181
	ldr	r3, [r3, #188]
	blx	r3
	mov	r3, #0
	mov	r0, #0
	str	r3, [r4, #320]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L825:
	ldr	r1, [r4, #52]
	ldr	r0, [r4, #56]
	bl	MEM_ReleaseMemBlock
	b	.L787
.L824:
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_ReleaseMemBlock
	b	.L783
.L832:
	ldr	r3, .L835+4
	mov	r1, #0
	mov	r0, #127
	ldr	r5, .L835+8
	ldr	r3, [r3, #188]
	blx	r3
	mov	r3, #0
	str	r3, [r4, #312]
	ldr	r3, [r4, #316]
	cmp	r3, #1
	bne	.L797
.L833:
	ldr	r3, .L835+4
	mov	r1, #0
	mov	r0, #136
	ldr	r3, [r3, #188]
	blx	r3
	mov	r3, #0
	str	r3, [r5, #316]
	b	.L797
.L789:
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #32]
	bl	MEM_ReleaseMemBlock
	b	.L790
.L836:
	.align	2
.L835:
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC6
	UNWIND(.fnend)
	.size	CloseDrivers, .-CloseDrivers
	.align	2
	.global	OpenDrivers
	.type	OpenDrivers, %function
OpenDrivers:
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
	add	r6, r0, #96
	mov	r4, r0
	mov	r0, #0
	mov	r1, #65536
	mov	r2, r6
	movt	r0, 63683
	bl	MEM_MapRegisterAddr
	subs	r5, r0, #0
	bne	.L838
	ldr	r2, [r4, #112]
	add	r6, r4, #168
	ldr	r1, [r4, #100]
	ldr	r0, [r4, #104]
	bl	MEM_AddMemRecord
	mov	r0, #53248
	mov	r2, r6
	movt	r0, 63683
	mov	r1, #4096
	bl	MEM_MapRegisterAddr
	subs	r7, r0, #0
	beq	.L880
	mov	r2, #53248
	ldr	r1, .L885
	movt	r2, 63683
	mov	r0, r5
	bl	dprint_vfmw
	ldr	r3, .L885+4
	mov	r1, r5
.L875:
	mov	r0, r6
.L877:
	mov	r2, #24
.L874:
	ldr	r3, [r3, #48]
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
.L841:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L880:
	ldr	r2, [r4, #184]
	add	r6, r4, #144
	ldr	r1, [r4, #172]
	ldr	r0, [r4, #176]
	bl	MEM_AddMemRecord
	mov	r0, #49152
	mov	r2, r6
	movt	r0, 63683
	mov	r1, #4096
	bl	MEM_MapRegisterAddr
	cmp	r0, #0
	bne	.L842
	ldr	r2, [r4, #160]
	ldr	r1, [r4, #148]
	ldr	r0, [r4, #152]
	bl	MEM_AddMemRecord
	ldr	r3, [r4, #72]
	cmp	r3, #0
	bne	.L843
.L844:
	ldr	r5, .L885+4
	mov	r0, #12
	ldr	r7, .L885+8
	ldr	r8, [r5, #68]
	bl	VFMW_GetString
	mov	r3, #0
	mov	r1, #20
	mov	r2, r0
	sub	r0, fp, #84
	blx	r8
	ldr	r3, [r7, #48]
	cmp	r3, #0
	beq	.L850
	blx	r3
	subs	r2, r0, #0
	ble	.L850
	mov	r6, #0
	mov	r8, #3
	str	r6, [sp]
	mov	r1, r6
	mov	r3, r4
	str	r8, [r4, #20]
	sub	r0, fp, #84
	bl	MEM_AllocMemBlock
	subs	r9, r0, #0
	bne	.L851
	ldr	r2, [r4, #16]
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_AddMemRecord
	ldr	r3, [r5, #48]
	mov	r2, #20
	mov	r1, r9
	sub	r0, fp, #64
	blx	r3
	ldr	r3, [r7, #56]
	ldr	r1, [r4, #8]
	ldr	r2, [r4, #16]
	cmp	r3, r6
	str	r9, [fp, #-56]
	str	r1, [fp, #-64]
	str	r2, [fp, #-60]
	beq	.L853
	sub	r0, fp, #64
	blx	r3
	subs	r7, r0, #0
	bne	.L853
	str	r8, [r4, #68]
	mov	r0, #13
	ldr	r6, [r5, #68]
	bl	VFMW_GetString
	add	r9, r4, #48
	mov	r3, r7
	mov	r1, #20
	mov	r2, r0
	sub	r0, fp, #84
	blx	r6
	mov	r1, #1
	mov	r3, r9
	str	r1, [sp]
	mov	r2, #16384
	mov	r1, r7
	sub	r0, fp, #84
	bl	MEM_AllocMemBlock
	subs	r6, r0, #0
	bne	.L854
	ldr	r1, [r4, #52]
	add	r7, r4, #24
	ldr	r2, [r4, #64]
	ldr	r0, [r4, #56]
	bl	MEM_AddMemRecord
	ldr	r2, .L885+12
	ldr	r3, [r4, #56]
	mov	r0, r6
	str	r3, [r2, #1196]
	ldr	r3, [r4, #56]
	add	r3, r3, #5120
	str	r3, [r2, #1200]
	bl	VDMDRV_Init
	mov	r0, #14
	ldr	r9, [r5, #68]
	bl	VFMW_GetString
	mov	r3, r6
	mov	r1, #20
	mov	r2, r0
	sub	r0, fp, #84
	blx	r9
	str	r8, [r4, #44]
	sub	r0, fp, #84
	str	r6, [sp]
	mov	r3, r7
	mov	r2, #716800
	mov	r1, r6
	bl	MEM_AllocMemBlock
	subs	r8, r0, #0
	bne	.L881
	ldr	r2, [r4, #40]
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #32]
	bl	MEM_AddMemRecord
	mov	r2, r8
	ldr	r1, [r4, #40]
	ldr	r0, [r4, #32]
	bl	SM_OpenSCDDrv
	cmp	r0, #0
	bne	.L882
.L848:
	bl	SM_InstInit
	mov	r0, #0
	ldr	r6, [r5, #184]
	bl	VFMW_GetString
	mov	r5, #0
	mov	r2, #32
	str	r5, [sp]
	ldr	r1, .L885+16
	ldr	r8, .L885+4
	mov	r3, r0
	mov	r0, #127
	blx	r6
	subs	r10, r0, #0
	bne	.L883
	ldr	r5, .L885+20
	mov	r0, #3
	ldr	r9, [r8, #184]
	mov	r6, #1
	str	r6, [r5, #312]
	bl	VFMW_GetString
	str	r10, [sp]
	mov	r2, #32
	ldr	r1, .L885+24
	mov	r3, r0
	mov	r0, #136
	blx	r9
	subs	r9, r0, #0
	bne	.L884
	mov	r0, #11
	ldr	r10, [r8, #184]
	str	r6, [r5, #316]
	bl	VFMW_GetString
	str	r9, [sp]
	mov	r2, #32
	ldr	r1, .L885+28
	mov	r3, r0
	mov	r0, #181
	blx	r10
	cmp	r0, #0
	streq	r6, [r5, #320]
	beq	.L841
	ldr	r1, .L885+32
	mov	r2, #181
	mov	r0, r9
	bl	dprint_vfmw
	ldr	r3, [r8, #48]
	mov	r1, r9
.L872:
	mov	r0, r7
.L873:
	mov	r2, #24
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L841
.L843:
	ldr	r2, [r4, #88]
	cmp	r2, #0
	beq	.L844
	ldr	r6, .L885+8
	ldr	r2, [r6, #48]
	cmp	r2, #0
	mvneq	r5, #0
	beq	.L845
	blx	r2
	ldr	r3, [r4, #72]
	mov	r5, r0
.L845:
	ldr	r1, [r4, #80]
	mov	r0, r3
	str	r3, [r4, #8]
	mov	r2, r5
	str	r5, [r4, #16]
	str	r1, [r4, #4]
	bl	MEM_AddMemRecord
	ldr	r3, [r6, #56]
	ldr	r1, [r4, #8]
	mov	r0, #0
	ldr	r2, [r4, #16]
	cmp	r3, r0
	str	r0, [fp, #-56]
	str	r1, [fp, #-64]
	str	r2, [fp, #-60]
	beq	.L846
	sub	r0, fp, #64
	blx	r3
	subs	r6, r0, #0
	bne	.L846
	bl	VDMDRV_Init
	ldr	r3, [r4, #72]
	ldr	r1, [r4, #80]
	mov	r2, #716800
	add	r0, r5, r3
	str	r2, [r4, #40]
	add	r1, r5, r1
	str	r0, [r4, #32]
	str	r1, [r4, #28]
	add	r7, r4, #24
	bl	MEM_AddMemRecord
	mov	r2, r6
	ldr	r1, [r4, #40]
	ldr	r0, [r4, #32]
	bl	SM_OpenSCDDrv
	cmp	r0, #0
	bne	.L847
	ldr	r5, .L885+4
	b	.L848
.L838:
	mov	r2, #0
	ldr	r1, .L885
	movt	r2, 63683
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, .L885+4
	mov	r0, r6
	mov	r2, #24
	mov	r1, #0
	b	.L874
.L850:
	mov	r2, #0
	ldr	r1, .L885+36
	mov	r0, r2
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r2, #24
	mov	r1, #0
	mov	r0, r6
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L841
.L884:
	ldr	r1, .L885+32
	mov	r2, #136
	mov	r0, r10
	bl	dprint_vfmw
	ldr	r3, [r8, #48]
	mov	r1, r10
	b	.L872
.L883:
	ldr	r1, .L885+32
	mov	r2, #127
	mov	r0, r5
	bl	dprint_vfmw
	ldr	r3, [r8, #48]
	mov	r1, r5
	b	.L872
.L853:
	mov	r2, #0
	ldr	r1, .L885+40
	mov	r0, r2
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r2, #24
	mov	r1, #0
.L876:
	mov	r0, r4
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L841
.L882:
	ldr	r1, .L885+44
	mov	r2, r8
	mov	r0, r8
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r8
	b	.L872
.L881:
	ldr	r1, .L885+48
	mov	r2, r6
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r6
	b	.L872
.L854:
	ldr	r1, .L885+52
	mov	r2, r7
	mov	r0, r7
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r7
	mov	r0, r9
	b	.L873
.L842:
	mov	r2, #49152
	ldr	r1, .L885
	movt	r2, 63683
	mov	r0, r7
	bl	dprint_vfmw
	ldr	r3, .L885+4
	mov	r1, r7
	b	.L875
.L851:
	mov	r2, r6
	ldr	r1, .L885+56
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r6
	mov	r2, #24
	b	.L876
.L846:
	mov	r2, #0
	ldr	r1, .L885+40
	mov	r0, r2
	bl	dprint_vfmw
	ldr	r3, .L885+4
	mov	r2, #24
	mov	r1, #0
	mov	r0, r4
	b	.L874
.L847:
	ldr	r1, .L885+60
	mov	r2, r6
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, .L885+4
	mov	r1, r6
	mov	r0, r7
	b	.L877
.L886:
	.align	2
.L885:
	.word	.LC74
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	g_HwMem
	.word	VCTRL_Isr_0
	.word	.LANCHOR0
	.word	VCTRL_Scd_Isr
	.word	VCTRL_MMU_ISR
	.word	.LC82
	.word	.LC77
	.word	.LC75
	.word	.LC80
	.word	.LC81
	.word	.LC79
	.word	.LC78
	.word	.LC76
	UNWIND(.fnend)
	.size	OpenDrivers, .-OpenDrivers
	.align	2
	.global	VCTRL_OpenVfmw
	.type	VCTRL_OpenVfmw, %function
VCTRL_OpenVfmw:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 16
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
	ldr	r4, .L922
	ldr	r7, .L922+4
	mov	r6, r0
	mov	r2, #8
	sub	r0, fp, #44
	ldr	r3, [r4, #48]
	mov	r1, #0
	blx	r3
	ldr	r5, [r7, #64]
	cmp	r5, #0
	bne	.L917
	mov	r1, #1024
	ldr	r3, [r4, #80]
	str	r5, [fp, #-48]
	ldr	r0, .L922+8
	blx	r3
	ldr	r3, [r4, #96]
	ldr	r1, .L922+12
	ldr	r0, .L922+8
	blx	r3
	cmp	r0, #0
	str	r0, [fp, #-48]
	beq	.L890
	ldr	r3, [r4, #92]
	sub	r0, fp, #48
	ldr	r1, .L922+16
	blx	r3
	cmp	r0, #0
	beq	.L909
	sub	r0, fp, #48
	ldr	r3, [r4, #92]
	ldr	r1, .L922+20
	blx	r3
	cmp	r0, #0
	beq	.L918
	mov	r2, r5
	ldr	r3, [r4, #84]
	mov	r1, r5
	blx	r3
	mov	r2, r0
	str	r0, [r7, #3808]
.L894:
	add	r3, r2, #4992
	movw	r1, #10000
	add	r3, r3, #8
	movw	r8, #5000
	cmp	r3, r1
	bhi	.L895
	cmp	r2, r8
	movw	r3, #60536
.L916:
	movt	r3, 65535
	movge	r2, r8
	cmp	r2, r3
	movlt	r2, r3
.L891:
	str	r2, [r7, #3808]
.L893:
	mov	r9, #0
	ldr	r3, [r4, #80]
	mov	r1, #1024
	str	r9, [fp, #-48]
	ldr	r0, .L922+8
	blx	r3
	ldr	r3, [r4, #96]
	ldr	r1, .L922+24
	ldr	r0, .L922+8
	blx	r3
	ldr	r8, .L922
	cmp	r0, r9
	str	r0, [fp, #-48]
	beq	.L896
	ldr	r3, [r8, #92]
	sub	r0, fp, #48
	ldr	r1, .L922+16
	blx	r3
	cmp	r0, r9
	beq	.L897
	sub	r0, fp, #48
	ldr	r3, [r8, #92]
	ldr	r1, .L922+20
	blx	r3
	cmp	r0, r9
	beq	.L919
	mov	r2, r9
	ldr	r3, [r8, #84]
	mov	r1, r9
	blx	r3
	mov	r2, r0
	str	r0, [r7, #3812]
.L900:
	add	r3, r2, #4992
	movw	r1, #10000
	add	r3, r3, #8
	movw	r8, #5000
	cmp	r3, r1
	bhi	.L901
	cmp	r2, r8
	movw	r3, #60536
	movt	r3, 65535
	movlt	r5, r2
	movge	r5, r8
	cmp	r5, r3
	movlt	r5, r3
.L897:
	str	r5, [r7, #3812]
.L899:
	ldr	r7, .L922+28
	mov	r3, #0
	str	r3, [fp, #-48]
	ldr	r5, [r7, #-800]
	cmp	r5, r3
	beq	.L902
	ldr	r2, .L922+32
	ubfx	r1, r5, #1, #1
	and	r0, r5, #1
	ubfx	r3, r5, #2, #1
	strb	r0, [r7, #-796]
	ubfx	r5, r5, #3, #1
	strb	r1, [r2]
	strb	r3, [r7, #-795]
	strb	r5, [r7, #-794]
.L903:
	ldr	r5, .L922+4
	mov	r2, #272
	ldr	r3, [r4, #48]
	mov	r1, #0
	add	r0, r5, #64
	ldr	r8, .L922
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #64
	mov	r1, #0
	mov	r0, r5
	blx	r3
	ldr	r2, .L922+36
	sub	r0, fp, #44
	mov	r1, #134217728
	mov	r3, #75497472
	str	r1, [fp, #-44]
	str	r2, [r7, #-792]
	str	r3, [fp, #-40]
	bl	MEM_InitMemManager
	bl	FSP_Init
	bl	ChanCtx_Init
	ldr	r3, [r4, #48]
	mov	r2, #64
	mov	r1, #0
	ldr	r0, .L922+40
	blx	r3
	ldr	r3, [r4, #52]
	add	r1, r6, #8
	mov	r2, #24
	add	r0, r5, #160
	blx	r3
	add	r0, r5, #88
	bl	OpenDrivers
	cmp	r0, #0
	bne	.L917
	ldr	r3, [r8, #120]
	mov	r1, #1
	blx	r3
	ldr	r3, [r8, #180]
	ldr	r2, .L922+44
	add	r0, r5, #76
	ldr	r1, .L922+48
	blx	r3
	cmp	r0, #0
	moveq	r3, #1
	streq	r3, [r5, #64]
	bne	.L920
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L902:
	ldr	r3, [r4, #80]
	mov	r1, #1024
	sub	r0, r7, #1824
	ldr	r8, .L922
	blx	r3
	ldr	r3, [r4, #96]
	ldr	r1, .L922+52
	sub	r0, r7, #1824
	blx	r3
	cmp	r0, #0
	str	r0, [fp, #-48]
	beq	.L903
	ldr	r3, [r8, #92]
	sub	r0, fp, #48
	ldr	r1, .L922+16
	blx	r3
	cmp	r0, #0
	ldreq	r0, [r7, #-800]
	beq	.L905
	sub	r0, fp, #48
	ldr	r3, [r8, #92]
	ldr	r1, .L922+20
	blx	r3
	cmp	r0, #0
	beq	.L921
	ldr	r3, [r8, #84]
	mov	r2, r5
	mov	r1, r5
	blx	r3
	str	r0, [r7, #-800]
.L905:
	ldr	r2, .L922+32
	ubfx	r1, r0, #1, #1
	and	ip, r0, #1
	ubfx	r3, r0, #2, #1
	strb	ip, [r7, #-796]
	ubfx	r0, r0, #3, #1
	strb	r1, [r2]
	strb	r3, [r7, #-795]
	strb	r0, [r7, #-794]
	b	.L903
.L896:
	ldr	r2, [r7, #3812]
	b	.L900
.L890:
	ldr	r2, [r7, #3808]
	b	.L894
.L901:
	ldr	r1, .L922+56
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r5, [r7, #3812]
	movw	r3, #60536
	cmp	r5, r8
	movt	r3, 65535
	movge	r5, r8
	cmp	r5, r3
	movlt	r5, r3
	b	.L897
.L895:
	ldr	r1, .L922+60
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r2, [r7, #3808]
	movw	r3, #60536
	cmp	r2, r8
	b	.L916
.L909:
	mov	r2, r5
	b	.L891
.L920:
	add	r0, r5, #88
	bl	CloseDrivers
.L917:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L918:
	movw	r3, #8404
	ldr	r2, .L922+64
	ldr	r1, .L922+68
	mov	r0, #22
	bl	dprint_vfmw
	b	.L893
.L919:
	movw	r3, #8451
	ldr	r2, .L922+72
	ldr	r1, .L922+68
	mov	r0, #22
	bl	dprint_vfmw
	b	.L899
.L921:
	movw	r3, #8508
	ldr	r2, .L922+76
	ldr	r1, .L922+68
	mov	r0, #22
	bl	dprint_vfmw
	b	.L903
.L923:
	.align	2
.L922:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR2-1824
	.word	.LC83
	.word	.LC84
	.word	.LC85
	.word	.LC88
	.word	.LANCHOR2
	.word	.LANCHOR3
	.word	VCTRL_EventReport
	.word	.LANCHOR2-788
	.word	VFMW_CTRL_thread_proc
	.word	.LC91
	.word	.LC90
	.word	.LC89
	.word	.LC87
	.word	.LANCHOR1+184
	.word	.LC86
	.word	.LANCHOR1+204
	.word	.LANCHOR1+224
	UNWIND(.fnend)
	.size	VCTRL_OpenVfmw, .-VCTRL_OpenVfmw
	.align	2
	.global	GetNextDecodeMode
	.type	GetNextDecodeMode, %function
GetNextDecodeMode:
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
	movw	r6, #21846
	mov	r3, r2, asl #1
	cmp	r2, #0
	movt	r6, 21845
	add	r5, r2, #7
	smull	r6, r7, r3, r6
	movge	r5, r2
	mov	r5, r5, asr #3
	str	r2, [sp]
	mov	r6, r0
	str	r1, [sp, #4]
	mov	r4, r2
	mov	r8, r1
	sub	r3, r7, r3, asr #31
	ldr	r1, .L932
	mov	r2, r5
	mov	r0, #30
	bl	dprint_vfmw
	cmp	r6, #3
	beq	.L930
	cmp	r4, r8
	movge	r4, r6
	blt	.L931
.L926:
	mov	r2, r6
	mov	r3, r4
	ldr	r1, .L932+4
	mov	r0, #30
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L931:
	ldr	r1, .L932+8
	mov	r0, #30
	bl	dprint_vfmw
.L929:
	mov	r4, #3
	b	.L926
.L930:
	cmp	r5, r8
	ble	.L929
	ldr	r1, .L932+12
	mov	r0, #30
	bl	dprint_vfmw
	mov	r4, #0
	b	.L926
.L933:
	.align	2
.L932:
	.word	.LC92
	.word	.LC95
	.word	.LC94
	.word	.LC93
	UNWIND(.fnend)
	.size	GetNextDecodeMode, .-GetNextDecodeMode
	.align	2
	.global	VCTRL_GetChanIDByCtx
	.type	VCTRL_GetChanIDByCtx, %function
VCTRL_GetChanIDByCtx:
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
	ldr	ip, .L943
	mov	r2, #0
.L937:
	ldr	r1, [ip], #4
	cmp	r1, #0
	add	r3, r1, #294912
	add	r3, r3, #1968
	beq	.L935
	cmp	r0, r3
	beq	.L938
.L935:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L937
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L938:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L944:
	.align	2
.L943:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetChanIDByCtx, .-VCTRL_GetChanIDByCtx
	.align	2
	.global	VCTRL_GetChanIDByMemAddr
	.type	VCTRL_GetChanIDByMemAddr, %function
VCTRL_GetChanIDByMemAddr:
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
	ldr	ip, .L956
	mov	r2, #0
.L949:
	ldr	r3, [ip], #4
	cmp	r3, #0
	beq	.L946
	ldr	r1, [r3, #228]
	cmp	r1, r0
	bhi	.L947
	ldr	lr, [r3, #236]
	add	r1, r1, lr
	cmp	r0, r1
	bcc	.L951
.L947:
	ldr	r1, [r3, #252]
	cmp	r0, r1
	bcc	.L946
	ldr	r3, [r3, #260]
	add	r1, r1, r3
	cmp	r0, r1
	bcc	.L951
.L946:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L949
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L951:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L957:
	.align	2
.L956:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetChanIDByMemAddr, .-VCTRL_GetChanIDByMemAddr
	.align	2
	.global	DecChanSyntax
	.type	DecChanSyntax, %function
DecChanSyntax:
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
	cmp	r0, #15
	mov	r4, r0
	bhi	.L1027
	ldr	r7, .L1043
	ldr	r0, [r7, r0, asl #2]
	cmp	r0, #0
	beq	.L1028
	ldr	r6, [r0, #16]
	cmp	r6, #1
	bne	.L1029
	ldr	r5, [r0, #32]
	cmp	r5, #0
	bne	.L1030
	ldr	r3, .L1043+4
	mov	r10, r4, asl #2
	str	r5, [r0, #372]
	ldr	r3, [r3]
	blx	r3
	ldr	r3, .L1043+8
	ldr	r1, .L1043+12
	add	r2, r3, r10
	ldr	r3, [r2, #-724]
	ldr	r1, [r1]
	rsb	r3, r3, r0
	cmp	r3, r1
	ldr	r3, [r7, r4, asl #2]
	strhi	r0, [r2, #-724]
	movls	r6, r5
	ldr	r1, .L1043
	ldr	r3, [r3, #600]
	sub	r2, r3, #10
	cmp	r2, #2
	bhi	.L965
	ldr	r2, [r1, #3740]
	tst	r2, #1
	bne	.L965
.L966:
	mov	r0, r4
	bl	VCTRL_GetImageBuffer
	cmp	r0, #1
	beq	.L967
	cmp	r6, #1
	mvnne	r5, #2
	beq	.L1031
.L1005:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L965:
	bic	r2, r3, #16
	cmp	r2, #4
	cmpne	r3, #14
	beq	.L966
	mov	r0, r4
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L966
	cmp	r6, #1
	mvnne	r5, #1
	bne	.L1005
	mov	r2, r4
	ldr	r1, .L1043+16
	mov	r0, #23
	mvn	r5, #1
	bl	dprint_vfmw
	b	.L1005
.L967:
	ldr	ip, [r7, r4, asl #2]
	ldr	r2, [ip, #1440]
	cmp	r2, #0
	bne	.L1032
.L968:
	ldrb	r3, [ip, #745]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r3, ip, #752
	strne	r3, [fp, #-48]
	bne	.L971
	ldr	r3, [ip, #1480]
	cmp	r3, #0
	add	r3, ip, #752
	strne	r3, [fp, #-48]
	beq	.L1033
.L971:
	ldr	r3, [fp, #-48]
	str	ip, [fp, #-68]
	ldrd	r0, [r3, #40]
	ldrd	r8, [r3, #24]
	strd	r0, [fp, #-60]
	mov	r0, #29
	ldrd	r2, [fp, #-60]
	ldr	r1, .L1043+20
	strd	r2, [sp]
	mov	r3, r9
	mov	r2, r8
	bl	dprint_vfmw
	ldr	r3, [r7, r4, asl #2]
	ldr	ip, [fp, #-68]
	ldr	r1, [r3, #52]
	str	r3, [fp, #-60]
	cmp	r1, #1
	beq	.L1034
	sub	r3, r1, #2
	cmp	r1, #6
	cmpne	r3, #1
	bls	.L992
.L974:
	ldr	r3, [fp, #-48]
	mvn	r2, #0
	ldrd	r0, [r3, #24]
	mvn	r3, #0
	cmp	r1, r3
	ldr	r3, [fp, #-60]
	cmpeq	r0, r2
	add	r3, r3, #672
	beq	.L1035
	strd	r0, [r3, #-8]
.L985:
	ldr	r3, [fp, #-48]
	ldrd	r2, [r3, #40]
	mov	r9, r3
	ldr	r3, [fp, #-60]
	mov	r8, r2
	add	r2, r3, #688
	str	r2, [fp, #-68]
	mov	r2, r0
	ldr	r0, [fp, #-68]
	add	r3, r3, #704
	strd	r8, [fp, #-68]
	strd	r8, [r0, #-8]
	mov	lr, r3
	ldr	r0, [fp, #-48]
	mov	r3, r1
	ldr	r1, .L1043+24
	str	ip, [fp, #-48]
	ldrd	r8, [r0, #48]
	mov	r0, #29
	strd	r8, [lr, #-8]
	ldr	r8, [fp, #-60]
	ldr	lr, [ip, #808]
	str	lr, [r8, #712]
	ldr	lr, [ip, #812]
	str	lr, [r8, #716]
	ldr	lr, [ip, #816]
	str	lr, [r8, #720]
	ldr	lr, [ip, #820]
	str	lr, [r8, #736]
	ldrd	r8, [fp, #-68]
	strd	r8, [sp]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
.L983:
	mov	r0, #3
	str	ip, [fp, #-48]
	bl	CHECK_REC_POS_ENABLE
	ldr	ip, [fp, #-48]
	adds	r3, r0, #0
	movne	r3, #1
	cmp	r6, #1
	movne	r3, #0
	cmp	r3, #0
	bne	.L1036
.L986:
	ldr	r3, [r7, r4, asl #2]
	add	r0, ip, #744
	ldr	r6, .L1043
	add	r2, r3, #294912
	ldr	r1, [r3, #600]
	add	r2, r2, #1968
	bl	DecodeStream
	ldr	r0, [r7, r4, asl #2]
	ldr	r3, [r0, #600]
	cmp	r3, #16
	beq	.L1037
.L987:
	mov	r0, #4
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L1005
	ldr	r3, .L1043+4
	ldr	r3, [r3]
	blx	r3
	ldr	r3, .L1043+8
	ldr	r1, .L1043+28
	add	r8, r3, r10
	ldr	r3, [r8, #-660]
	ldr	r1, [r1]
	rsb	r3, r3, r0
	mov	r2, r0
	cmp	r3, r1
	bls	.L1005
	mov	r0, r4
	str	r2, [r8, #-660]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L1005
	mov	r2, r4, asl #24
	movw	r1, #7897
	add	r2, r2, #262144
	ldr	r0, .L1043+32
	bl	DBG_AddTrace
	b	.L1005
.L1032:
	cmp	r2, #1
	addne	ip, ip, #290816
	ldrne	r3, [ip, #1824]
	beq	.L1038
.L991:
	ldr	r1, .L1043+36
	mov	r0, #31
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1005
.L1031:
	mov	r2, r4
	ldr	r1, .L1043+40
	mov	r0, #23
	mvn	r5, #2
	bl	dprint_vfmw
	b	.L1005
.L1034:
	add	r3, r3, #294912
	ldr	r3, [r3, #1968]
	ldr	r1, [r3, #556]
	sxtb	r1, r1
	cmp	r1, #1
	bne	.L974
.L992:
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-48]
	add	r0, r3, #1152
	str	ip, [fp, #-68]
	add	r0, r0, #8
	bl	IsNewpicSeg
	ldr	ip, [fp, #-68]
	cmp	r0, #0
	bne	.L975
	ldr	r1, [r7, r4, asl #2]
	add	r0, r1, #672
	ldr	r3, [r1, #52]
	cmp	r3, #3
	beq	.L1039
	ldrd	r8, [r0]
	mvn	r3, #0
	mvn	r2, #0
	cmp	r9, r3
	cmpeq	r8, r2
	beq	.L1040
	mov	r2, r8
	mov	r3, r9
	strd	r2, [r0, #-8]
.L977:
	add	lr, r1, #688
	mvn	r2, #0
	mvn	r3, #0
	strd	r2, [r0]
	ldrd	r2, [lr]
	add	r0, r1, #704
	strd	r2, [lr, #-8]
	ldrd	r2, [r0]
	orrs	lr, r2, r3
	strd	r2, [r0, #-8]
	ldreq	r3, [fp, #-48]
	ldreqd	r2, [r3, #48]
	streqd	r2, [r0, #-8]
.L979:
	ldr	lr, [r1, #724]
	mov	r3, r9
	str	ip, [fp, #-60]
	mov	r2, r8
	ldr	ip, [r1, #728]
	mov	r0, #29
	str	lr, [r1, #712]
	ldr	lr, [r1, #740]
	str	ip, [r1, #716]
	ldr	ip, [r1, #732]
	str	lr, [r1, #736]
	str	ip, [r1, #720]
	ldr	r1, .L1043+44
	bl	dprint_vfmw
	ldr	lr, [r7, r4, asl #2]
	ldr	ip, [fp, #-60]
	ldr	r3, [lr, #52]
	cmp	r3, #3
	beq	.L1041
.L980:
	ldr	r3, [fp, #-48]
	mvn	r2, #0
	ldrd	r0, [r3, #24]
	mvn	r3, #0
	cmp	r1, r3
	cmpeq	r0, r2
	addne	r3, lr, #672
	strned	r0, [r3]
.L982:
	ldr	r8, [fp, #-48]
	add	r1, lr, #688
	add	r0, lr, #704
	ldrd	r2, [r8, #40]
	strd	r2, [r1]
	ldrd	r2, [r8, #48]
	strd	r2, [r0]
	ldr	r3, [ip, #808]
	str	r3, [lr, #724]
	ldr	r3, [ip, #812]
	str	r3, [lr, #728]
	ldr	r3, [ip, #816]
	str	r3, [lr, #732]
	ldr	r3, [ip, #820]
	str	r3, [lr, #740]
.L981:
	ldrd	r2, [r1, #-8]
	mov	r0, #29
	ldr	r1, .L1043+48
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L983
.L1033:
	mov	r0, r4
	mov	r1, r3
	str	ip, [fp, #-60]
	str	r3, [fp, #-48]
	bl	SM_ReadSegStream
	ldr	ip, [fp, #-60]
	cmp	r0, #0
	beq	.L971
	cmp	r6, #1
	beq	.L1042
.L972:
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #1
	mvn	r5, #3
	str	r2, [r3, #372]
	b	.L1005
.L1029:
	ldr	r3, .L1043+52
	movw	r2, #7649
	ldr	r1, .L1043+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1005
.L1030:
	ldr	r3, .L1043+60
	movw	r2, #7650
	ldr	r1, .L1043+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1005
.L1037:
	ldr	r3, [r0, #1504]
	cmp	r3, #1
	beq	.L987
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	IsMain10Profile
	cmp	r0, #0
	ldreq	r3, [r6, r4, asl #2]
	streq	r0, [r3, #1504]
	beq	.L987
	mov	r3, r4
	ldr	r2, .L1043+32
	ldr	r1, .L1043+64
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r6, r4, asl #2]
	mov	r2, #1
	str	r2, [r3, #1504]
	bl	VDMDRV_AddMatchMode
	b	.L987
.L1038:
	add	r3, ip, #290816
	ldr	r3, [r3, #1824]
	bics	r1, r3, #2
	beq	.L968
	cmp	r3, #1
	bne	.L991
	ldr	r1, .L1043+68
	mov	r0, #31
	bl	dprint_vfmw
	mvn	r5, #2
	b	.L1005
.L1041:
	ldr	r0, [fp, #-48]
	add	r1, lr, #672
	ldrd	r2, [r0, #24]
	mov	r8, r2
	mov	r9, r3
	strd	r8, [r1]
	mov	r8, r0
	ldrd	r0, [r0, #40]
	add	r2, lr, #688
	add	r3, lr, #704
	strd	r0, [r2]
	ldrd	r0, [r8, #48]
	strd	r0, [r3]
	ldr	r3, [ip, #808]
	str	r3, [lr, #724]
	ldr	r3, [ip, #812]
	str	r3, [lr, #728]
	ldr	r3, [ip, #816]
	str	r3, [lr, #732]
	ldr	r3, [ip, #820]
	str	r3, [lr, #740]
.L975:
	ldr	lr, [r7, r4, asl #2]
	ldr	r3, [lr, #52]
	cmp	r3, #3
	addeq	r1, lr, #688
	bne	.L980
	b	.L981
.L1035:
	ldrd	r0, [r3, #-8]
	b	.L985
.L1027:
	ldr	r3, .L1043+72
	movw	r2, #7647
	ldr	r1, .L1043+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1005
.L1036:
	mov	r2, r4, asl #24
	movw	r1, #7859
	add	r2, r2, #196608
	ldr	r0, .L1043+32
	bl	DBG_AddTrace
	ldr	ip, [fp, #-48]
	b	.L986
.L1039:
	ldrd	r2, [r0]
	mov	r8, r2
	mov	r9, r3
	strd	r2, [r0, #-8]
	b	.L977
.L1040:
	ldrd	r2, [r0, #-8]
	mov	r8, r2
	mov	r9, r3
	b	.L977
.L1028:
	ldr	r3, .L1043+76
	movw	r2, #7648
	ldr	r1, .L1043+56
	mvn	r5, #0
	bl	dprint_vfmw
	b	.L1005
.L1042:
	mov	r2, r4
	ldr	r1, .L1043+80
	mov	r0, #23
	bl	dprint_vfmw
	b	.L972
.L1044:
	.align	2
.L1043:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR2
	.word	g_TraceBsPeriod
	.word	.LC97
	.word	.LC100
	.word	.LC103
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+240
	.word	.LC106
	.word	.LC98
	.word	.LC101
	.word	.LC102
	.word	.LC52
	.word	.LC1
	.word	.LC96
	.word	.LC104
	.word	.LC105
	.word	.LC12
	.word	.LC38
	.word	.LC99
	UNWIND(.fnend)
	.size	DecChanSyntax, .-DecChanSyntax
	.align	2
	.global	DecSyntax
	.type	DecSyntax, %function
DecSyntax:
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
	ldr	r0, .L1074
	ldr	r1, .L1074+4
	sub	r3, r0, #64
.L1048:
	ldr	r2, [r1, #4]!
	add	r3, r3, #4
	cmp	r2, #0
	ldrne	r2, [r2, #28]
	str	r2, [r3, #-4]
	cmp	r3, r0
	bne	.L1048
	ldr	r7, .L1074+8
	mvn	ip, #0
	ldr	r6, .L1074+12
	mov	r8, #0
	ldr	r10, .L1074+16
	mov	r9, r7
.L1049:
	mov	r3, #0
	ldr	r0, [r6, #-596]
	mov	r4, r3
	ldr	r1, .L1074+20
	b	.L1050
.L1052:
	ldr	r2, [r1, #4]!
	cmp	r0, r2
	movlt	r0, r2
	movlt	r4, r3
.L1050:
	add	r3, r3, #1
	cmp	r3, #16
	bne	.L1052
	add	r3, r6, r4, lsl #2
	cmp	r0, #0
	add	r2, r4, #1
	mov	r1, #0
	str	r8, [r3, #-596]
	str	r2, [r7, #84]
	ldr	r3, .L1074+8
	ble	.L1053
	ldr	r3, [r9, r4, asl #2]
	cmp	r3, r1
	beq	.L1049
	ldr	r2, [r3, #16]
	cmp	r2, #1
	bne	.L1049
	ldr	r2, [r3, #76]
	cmp	r2, r1
	ble	.L1055
	ldr	r5, [r3, #80]
	cmp	r5, r1
	beq	.L1072
.L1055:
	mov	r5, #200
	b	.L1060
.L1058:
	cmp	r0, #0
	bne	.L1049
	subs	r5, r5, #1
	beq	.L1049
.L1060:
	mov	r0, r4
	bl	DecChanSyntax
	cmn	r0, #1
	mov	ip, r0
	bne	.L1058
	ldr	r5, [r6, #-792]
	cmp	r5, #0
	beq	.L1066
	mov	r3, #0
	str	r0, [fp, #-72]
	mov	r2, r3
	mov	r0, r4
	mov	r1, #102
	blx	r5
	ldr	ip, [fp, #-72]
.L1066:
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1072:
	mov	r0, r4
	sub	r1, fp, #68
	bl	SM_GetInfo
	cmp	r0, #0
	bne	.L1073
.L1056:
	ldr	r5, [r7, r4, asl #2]
	ldr	r1, [fp, #-56]
	ldr	r2, [r5, #76]
	ldr	r0, [r5, #616]
	bl	GetNextDecodeMode
	ldr	r3, [r7, r4, asl #2]
	str	r0, [r5, #616]
	ldr	r2, [r3, #616]
	cmp	r2, #0
	strne	r8, [r3, #612]
	strne	r8, [r3, #608]
	ldreq	r2, [r3, #72]
	streq	r2, [r3, #612]
	streq	r2, [r3, #608]
	b	.L1055
.L1053:
	mov	r0, ip
	str	r1, [r3, #84]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1073:
	mov	r1, r5
	sub	r0, fp, #68
	ldr	r3, [r10, #48]
	mov	r2, #24
	blx	r3
	b	.L1056
.L1075:
	.align	2
.L1074:
	.word	.LANCHOR2-532
	.word	.LANCHOR0-4
	.word	.LANCHOR0
	.word	.LANCHOR2
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR2-596
	UNWIND(.fnend)
	.size	DecSyntax, .-DecSyntax
	.align	2
	.global	VCTRL_RunProcess
	.type	VCTRL_RunProcess, %function
VCTRL_RunProcess:
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
	ldr	r4, .L1085
	mov	r3, #100
	ldr	r5, .L1085+4
	ldr	r6, .L1085+8
	ldr	r0, [r4, #-532]
	str	r3, [r5, #80]
	cmp	r0, #0
	ble	.L1077
	ldr	r3, [r6, #116]
	blx	r3
.L1077:
	ldr	r3, [r6, #12]
	mov	r0, #0
	blx	r3
	bl	SM_Wakeup
	ldr	r3, [r4, #-528]
	mov	r2, #101
	str	r2, [r5, #80]
	cmp	r3, #0
	movne	r7, #0
	beq	.L1084
.L1078:
	mov	r3, #102
	str	r3, [r5, #80]
	bl	VDMDRV_WakeUpVdm
	mov	r4, #0
.L1079:
	mov	r0, r4
	mov	r1, #0
	add	r4, r4, #1
	bl	DeleteRawPacketInBuffer
	cmp	r4, #16
	bne	.L1079
	ldr	r3, [r6, #16]
	mov	r0, #0
	blx	r3
	mov	r1, #0
	mov	ip, #105
	ldr	r3, [r6, #128]
	mov	r0, r1
	mov	r2, #10
	str	ip, [r5, #80]
	blx	r3
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1084:
	bl	DecSyntax
	mov	r7, r0
	b	.L1078
.L1086:
	.align	2
.L1085:
	.word	.LANCHOR2
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_RunProcess, .-VCTRL_RunProcess
	.align	2
	.global	VFMW_CTRL_thread_proc
	.type	VFMW_CTRL_thread_proc, %function
VFMW_CTRL_thread_proc:
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
	ldr	r4, .L1114
	ldr	r6, .L1114+4
.L1088:
.L1095:
	ldr	r1, .L1114+8
	mov	r0, #2
	mov	r3, #0
	str	r3, [r4, #80]
	bl	dprint_vfmw
	ldr	r3, [r4, #72]
	mov	r5, #2
	str	r5, [r4, #68]
	cmp	r3, r5
	bne	.L1110
	b	.L1093
.L1112:
	str	r5, [r4, #80]
	bl	VCTRL_RunProcess
	ldr	r1, .L1114+12
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, [r4, #72]
	cmp	r3, #2
	beq	.L1093
.L1110:
	cmp	r3, #3
	bne	.L1112
.L1091:
	ldr	r1, .L1114+16
	mov	r0, #2
	mov	r3, #6
	str	r3, [r4, #80]
	bl	dprint_vfmw
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #68]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1093:
	ldr	r1, .L1114+20
	mov	r0, #2
	mov	r3, #4
	str	r3, [r4, #80]
	bl	dprint_vfmw
	mov	r3, #1
	str	r3, [r4, #68]
	b	.L1090
.L1113:
	ldr	r3, [r6, #116]
	blx	r3
.L1090:
	ldr	r1, .L1114+24
	mov	r0, #22
	mov	r3, #5
	str	r3, [r4, #80]
	bl	dprint_vfmw
	ldr	r3, [r4, #72]
	mov	r0, #10
	cmp	r3, #1
	beq	.L1095
	cmp	r3, #3
	bne	.L1113
	b	.L1091
.L1115:
	.align	2
.L1114:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC107
	.word	.LC109
	.word	.LC111
	.word	.LC108
	.word	.LC110
	UNWIND(.fnend)
	.size	VFMW_CTRL_thread_proc, .-VFMW_CTRL_thread_proc
	.align	2
	.global	VCTRL_GetChanVoQue
	.type	VCTRL_GetChanVoQue, %function
VCTRL_GetChanVoQue:
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
	ldr	r3, .L1135
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L1133
	ldr	r2, [r3, #600]
	cmp	r2, #20
	ldrls	pc, [pc, r2, asl #2]
	b	.L1134
.L1119:
	.word	.L1132
	.word	.L1120
	.word	.L1121
	.word	.L1122
	.word	.L1134
	.word	.L1123
	.word	.L1124
	.word	.L1134
	.word	.L1125
	.word	.L1126
	.word	.L1127
	.word	.L1127
	.word	.L1127
	.word	.L1128
	.word	.L1134
	.word	.L1129
	.word	.L1130
	.word	.L1131
	.word	.L1134
	.word	.L1134
	.word	.L1132
.L1132:
	movw	r0, #34744
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1120:
	add	r0, r3, #344064
	add	r0, r0, #2032
	ldmfd	sp, {fp, sp, pc}
.L1121:
	add	r0, r3, #319488
	add	r0, r0, #24
	ldmfd	sp, {fp, sp, pc}
.L1122:
	add	r0, r3, #380928
	add	r0, r0, #3664
	ldmfd	sp, {fp, sp, pc}
.L1123:
	add	r0, r3, #335872
	add	r0, r0, #2512
	ldmfd	sp, {fp, sp, pc}
.L1124:
	add	r0, r3, #311296
	add	r0, r0, #1712
	ldmfd	sp, {fp, sp, pc}
.L1125:
	movw	r0, #23032
	movt	r0, 5
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1126:
	movw	r0, #23016
	movt	r0, 5
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1127:
	add	r0, r3, #307200
	add	r0, r0, #2352
	ldmfd	sp, {fp, sp, pc}
.L1128:
	movw	r0, #42920
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1129:
	movw	r0, #35320
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1130:
	add	r0, r3, #356352
	add	r0, r0, #2256
	ldmfd	sp, {fp, sp, pc}
.L1131:
	add	r0, r3, #565248
	add	r0, r0, #2832
	ldmfd	sp, {fp, sp, pc}
.L1134:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1133:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L1136:
	.align	2
.L1135:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetChanVoQue, .-VCTRL_GetChanVoQue
	.align	2
	.global	VCTRL_FindOccupiedFrame
	.type	VCTRL_FindOccupiedFrame, %function
VCTRL_FindOccupiedFrame:
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
	UNWIND(.pad #44)
	sub	sp, sp, #44
	cmp	r0, #15
	mov	r8, r0
	bhi	.L1148
	bl	VCTRL_GetChanVoQue
	subs	r6, r0, #0
	beq	.L1145
	sub	r2, fp, #44
	mov	r3, #0
	sub	r1, fp, #52
	str	r3, [fp, #-52]
	str	r3, [r2, #-4]!
	bl	GetQueueImgNum
	ldr	r2, [fp, #-52]
	cmp	r2, #0
	beq	.L1145
	cmp	r2, #32
	bhi	.L1141
.L1143:
	ldr	r9, .L1149
	mov	r4, #0
	mov	lr, r4
	mov	r7, r4
	str	r8, [fp, #-56]
	str	r9, [fp, #-60]
.L1142:
	ldrh	r1, [r6, #4]
	movw	ip, #34168
	ldr	r3, [fp, #-60]
	movt	ip, 4
	add	r1, r1, lr
	ldr	r2, [fp, #-56]
	and	r1, r1, #63
	mov	r8, #0
	mov	r9, #0
	add	r4, r4, #1
	add	r1, r6, r1, lsl #2
	ldr	r2, [r3, r2, asl #2]
	mov	r0, #1
	movw	r3, #4225
	ldr	r10, [r1, #12]
	add	r2, r2, lr, lsl #4
	add	r5, r2, #294912
	add	ip, r2, ip
	add	r5, r5, #1408
	ldr	r2, .L1149+4
	ldr	r10, [r10, #28]
	uxth	r4, r4
	ldr	r1, .L1149+8
	str	r10, [ip, #4]
	strd	r8, [r5]
	str	r7, [ip]
	str	r7, [sp, #16]
	ldrd	r8, [r5]
	strd	r8, [sp, #8]
	ldr	ip, [ip, #4]
	str	lr, [sp]
	str	ip, [sp, #4]
	bl	dprint_vfmw
	ldr	r3, [fp, #-52]
	sxth	lr, r4
	cmp	lr, r3
	bcc	.L1142
	ldr	r8, [fp, #-56]
	ldr	r9, [fp, #-60]
.L1144:
	ldr	r2, [r9, r8, asl #2]
	mov	r0, #0
	add	r2, r2, #294912
	str	r3, [r2, #1912]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1145:
	mvn	r0, #0
.L1139:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1141:
	mov	r3, #32
	ldr	r1, .L1149+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [fp, #-52]
	cmp	r3, #0
	ldreq	r9, .L1149
	bne	.L1143
	b	.L1144
.L1148:
	ldr	r3, .L1149+16
	movw	r2, #4190
	ldr	r1, .L1149+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1139
.L1150:
	.align	2
.L1149:
	.word	.LANCHOR0
	.word	.LANCHOR1+256
	.word	.LC113
	.word	.LC112
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_FindOccupiedFrame, .-VCTRL_FindOccupiedFrame
	.align	2
	.global	VCTRL_ReportOccupiedFrame
	.type	VCTRL_ReportOccupiedFrame, %function
VCTRL_ReportOccupiedFrame:
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
	cmp	r0, #15
	mov	r7, r0
	mov	r4, r1
	bhi	.L1168
	cmp	r1, #0
	beq	.L1169
	bl	VCTRL_GetChanVoQue
	subs	r5, r0, #0
	mvneq	r0, #0
	beq	.L1153
	ldr	r6, .L1171
	sub	r8, fp, #36
	mov	r3, #0
	mov	r0, #4
	str	r3, [r8, #-4]!
	ldr	r2, [r6, #12]
	str	r3, [fp, #-44]
	blx	r2
	mov	r2, r8
	sub	r1, fp, #44
	mov	r0, r5
	bl	GetQueueImgNum
	ldr	r2, [fp, #-44]
	cmp	r2, #0
	beq	.L1155
	cmp	r2, #32
	bhi	.L1170
.L1156:
	mov	ip, #0
	mov	r0, #0
	mov	r1, #0
	mov	lr, ip
	mov	r8, ip
.L1158:
	ldrh	r3, [r5, #4]
	add	ip, ip, #1
	add	r7, r4, lr, lsl #4
	add	r3, r3, lr
	uxth	ip, ip
	and	r3, r3, #63
	add	r3, r5, r3, lsl #2
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #28]
	strd	r0, [r7, #8]
	str	r3, [r7, #4]
	str	r8, [r4, lr, asl #4]
	sxth	lr, ip
	cmp	lr, r2
	bcc	.L1158
	uxth	r3, r2
.L1157:
	ldrh	r1, [r5, #4]
	add	r3, r3, r1
	and	r3, r3, #63
	strh	r3, [r5, #4]	@ movhi
	str	r2, [r4, #512]
.L1159:
	ldr	r3, [r6, #16]
	mov	r0, #4
	blx	r3
	mov	r0, #0
.L1153:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1155:
	ldr	r8, .L1171+4
	ldr	ip, [r8, r7, asl #2]
	add	r3, ip, #294912
	ldr	r3, [r3, #1912]
	cmp	r3, #0
	beq	.L1159
	mov	r0, #0
	mov	r1, #0
	mov	r5, r2
	mov	r9, r2
.L1160:
	mov	r3, r5, asl #4
	movw	lr, #34168
	add	ip, ip, r3
	movt	lr, 4
	add	lr, ip, lr
	add	r3, r4, r3
	add	r2, r2, #1
	ldr	ip, [lr, #4]
	strd	r0, [r3, #8]
	uxth	r2, r2
	str	ip, [r3, #4]
	str	r9, [r4, r5, asl #4]
	sxth	r5, r2
	ldr	ip, [r8, r7, asl #2]
	add	r3, ip, #294912
	ldr	r3, [r3, #1912]
	cmp	r5, r3
	bcc	.L1160
	str	r3, [r4, #512]
	b	.L1159
.L1170:
	mov	r3, #32
	ldr	r1, .L1171+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [fp, #-44]
	cmp	r2, #0
	moveq	r3, r2
	bne	.L1156
	b	.L1157
.L1168:
	ldr	r3, .L1171+12
	movw	r2, #4305
	ldr	r1, .L1171+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1153
.L1169:
	mov	r0, r1
	ldr	r3, .L1171+20
	movw	r2, #4306
	ldr	r1, .L1171+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1153
.L1172:
	.align	2
.L1171:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC112
	.word	.LC12
	.word	.LC1
	.word	.LC114
	UNWIND(.fnend)
	.size	VCTRL_ReportOccupiedFrame, .-VCTRL_ReportOccupiedFrame
	.align	2
	.global	VCTRL_GetChanImgNum
	.type	VCTRL_GetChanImgNum, %function
VCTRL_GetChanImgNum:
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
	ldr	r4, .L1184
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	mov	r8, r3
	ldr	ip, [r4, r0, asl #2]
	cmp	ip, #0
	beq	.L1181
	bl	VCTRL_GetChanVoQue
	subs	r3, r0, #0
	beq	.L1175
	mov	r2, r8
	mov	r1, r7
	bl	GetQueueImgNum
	ldr	r3, [r4, r5, asl #2]
	ldr	r2, [r3, #600]
	cmp	r2, #0
	beq	.L1182
	cmp	r2, #16
	beq	.L1183
	cmp	r2, #17
	moveq	r3, #9
	streq	r3, [r6]
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r2, #15
	addeq	r3, r3, #11403264
	addeq	r3, r3, #16384
	movne	r3, #2
	ldreq	r3, [r3, #728]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1182:
	add	r3, r3, #323584
	ldr	r3, [r3, #16]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1175:
	str	r3, [r8]
	str	r3, [r7]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1183:
	add	r3, r3, #299008
	ldr	r3, [r3, #376]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1181:
	mov	r0, ip
	ldr	r3, .L1184+4
	movw	r2, #8042
	ldr	r1, .L1184+8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L1185:
	.align	2
.L1184:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_GetChanImgNum, .-VCTRL_GetChanImgNum
	.align	2
	.global	VCTRL_GetChanState
	.type	VCTRL_GetChanState, %function
VCTRL_GetChanState:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #40)
	sub	sp, sp, #40
	cmp	r0, #15
	mov	r6, r0
	mov	r5, r1
	bhi	.L1199
	cmp	r1, #0
	mov	r4, #0
	str	r4, [fp, #-56]
	str	r4, [fp, #-60]
	str	r4, [fp, #-64]
	beq	.L1186
	ldr	r3, .L1200
	mov	r2, #100
	ldr	r7, .L1200+4
	mov	r1, r4
	mov	r0, r5
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, [r7, r6, asl #2]
	cmp	r3, r4
	beq	.L1186
	ldr	r2, [r3, #1176]
	movw	r3, #1954
	str	r2, [r5, #32]
	ldr	r2, [r7, r6, asl #2]
	ldr	r2, [r2, #1180]
	str	r2, [r5, #36]
	ldr	r2, [r7, r6, asl #2]
	ldr	r2, [r2, #1184]
	str	r2, [r5, #44]
	ldr	ip, [r7, r6, asl #2]
	add	r0, ip, #262144
	ldrsb	r1, [r0, r3]
	cmp	r1, r4
	ble	.L1191
	movw	r3, #23350
	mov	r2, r4
	movt	r3, 4
	add	r3, ip, r3
.L1193:
	ldrsb	ip, [r3]
	add	r2, r2, #1
	add	r3, r3, #100
	cmp	ip, #3
	addeq	r4, r4, #1
	cmp	r2, r1
	bne	.L1193
.L1191:
	movw	r3, #1954
	str	r4, [r5, #84]
	ldrsb	ip, [r0, r3]
	sub	r2, fp, #60
	sub	r3, fp, #56
	sub	r1, fp, #64
	mov	r0, r6
	str	ip, [r5, #92]
	bl	VCTRL_GetChanImgNum
	ldr	ip, .L1200+8
	mov	r3, #300
	ldr	r0, [fp, #-56]
	ldr	r2, [fp, #-60]
	sub	r1, fp, #52
	mla	r3, r3, r6, ip
	add	r2, r2, r0
	str	r2, [r5, #48]
	mov	r0, r6
	ldr	r2, [r3, #-504]
	str	r2, [r5, #96]
	ldr	r3, [r3, #-516]
	str	r3, [r5, #20]
	bl	SM_GetInfo
	ldr	r3, [fp, #-44]
	ldr	r1, [fp, #-36]
	ldr	r2, [fp, #-32]
	str	r3, [r5, #56]
	str	r1, [r5, #52]
	cmp	r2, #10
	ldr	r3, [r7, r6, asl #2]
	movle	r2, #0
	movgt	r2, #1
	add	r3, r3, #311296
	str	r2, [r5, #80]
	ldrb	r3, [r3, #1472]	@ zero_extendqisi2
	and	r3, r3, #1
	str	r3, [r5, #88]
.L1186:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1199:
	ldr	r3, .L1200+12
	movw	r2, #1501
	ldr	r1, .L1200+16
	mov	r0, #0
	bl	dprint_vfmw
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1201:
	.align	2
.L1200:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR2
	.word	.LC115
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_GetChanState, .-VCTRL_GetChanState
	.align	2
	.global	VCTRL_GetChanWidth
	.type	VCTRL_GetChanWidth, %function
VCTRL_GetChanWidth:
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
	cmp	r0, #15
	bhi	.L1206
	ldr	r3, .L1208
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1207
	ldr	r0, [r0, #1276]
	ldmfd	sp, {fp, sp, pc}
.L1206:
	ldr	r3, .L1208+4
	movw	r2, #8099
	ldr	r1, .L1208+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1207:
	ldr	r3, .L1208+12
	movw	r2, #8100
	ldr	r1, .L1208+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1209:
	.align	2
.L1208:
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	.word	.LC2
	UNWIND(.fnend)
	.size	VCTRL_GetChanWidth, .-VCTRL_GetChanWidth
	.align	2
	.global	VCTRL_GetChanHeight
	.type	VCTRL_GetChanHeight, %function
VCTRL_GetChanHeight:
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
	cmp	r0, #15
	bhi	.L1214
	ldr	r3, .L1216
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1215
	ldr	r0, [r0, #1280]
	ldmfd	sp, {fp, sp, pc}
.L1214:
	ldr	r3, .L1216+4
	movw	r2, #8107
	ldr	r1, .L1216+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1215:
	ldr	r3, .L1216+12
	movw	r2, #8108
	ldr	r1, .L1216+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1217:
	.align	2
.L1216:
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	.word	.LC2
	UNWIND(.fnend)
	.size	VCTRL_GetChanHeight, .-VCTRL_GetChanHeight
	.align	2
	.global	VCTRL_GetChanCapLevel
	.type	VCTRL_GetChanCapLevel, %function
VCTRL_GetChanCapLevel:
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
	cmp	r0, #15
	bhi	.L1222
	ldr	r3, .L1223
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #44]
	moveq	r0, #33
	ldmfd	sp, {fp, sp, pc}
.L1222:
	ldr	r3, .L1223+4
	movw	r2, #8115
	ldr	r1, .L1223+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1224:
	.align	2
.L1223:
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_GetChanCapLevel, .-VCTRL_GetChanCapLevel
	.align	2
	.global	VCTRL_SetVdecExtra
	.type	VCTRL_SetVdecExtra, %function
VCTRL_SetVdecExtra:
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
	add	r3, r0, #4992
	ldr	r5, .L1232
	add	r3, r3, #8
	movw	r2, #10000
	cmp	r3, r2
	mov	r4, r1
	str	r0, [r5, #3808]
	bhi	.L1230
.L1226:
	movw	r1, #5000
	cmp	r0, r1
	movw	r2, #60536
	add	r3, r4, #4992
	movge	r0, r1
	movt	r2, 65535
	cmp	r0, r2
	add	r3, r3, #8
	movw	r1, #10000
	str	r4, [r5, #3812]
	movlt	r0, r2
	cmp	r3, r1
	str	r0, [r5, #3808]
	ldr	r6, .L1232
	bhi	.L1231
.L1227:
	movw	r2, #5000
	cmp	r4, r2
	movw	r3, #60536
	movge	r4, r2
	movt	r3, 65535
	cmp	r4, r3
	movlt	r4, r3
	str	r4, [r5, #3812]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1230:
	mov	r2, r0
	ldr	r1, .L1232+4
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r0, [r5, #3808]
	b	.L1226
.L1231:
	mov	r2, r4
	ldr	r1, .L1232+8
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r4, [r6, #3812]
	b	.L1227
.L1233:
	.align	2
.L1232:
	.word	.LANCHOR0
	.word	.LC116
	.word	.LC117
	UNWIND(.fnend)
	.size	VCTRL_SetVdecExtra, .-VCTRL_SetVdecExtra
	.align	2
	.global	VCTRL_ExtAccGetDecParam
	.type	VCTRL_ExtAccGetDecParam, %function
VCTRL_ExtAccGetDecParam:
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
	ldr	r6, .L1263
	mov	r7, r0
	mov	r0, #0
	ldr	r3, [r6, #12]
	blx	r3
	ldr	r3, [r6, #12]
	mov	r0, #5
	blx	r3
	ldr	r4, [r7]
	cmn	r4, #1
	beq	.L1255
	mov	r0, r4
	bl	VCTRL_IsChanActive
	subs	r5, r0, #0
	bne	.L1256
	mov	r0, r4
	bl	VDMDRV_IsChanDec
	cmp	r0, #1
	beq	.L1257
	mov	r0, r4
	bl	VCTRL_GetDecParam
	subs	r2, r0, #0
	beq	.L1258
	cmp	r4, #15
	bhi	.L1250
	ldr	r3, .L1263+4
	ldr	r3, [r3, r4, asl #2]
	cmp	r3, #0
	beq	.L1250
	ldr	r1, [r3, #600]
	sub	r0, fp, #56
	ldr	r3, .L1263+8
	str	r4, [fp, #-56]
	str	r1, [fp, #-52]
	str	r2, [fp, #-48]
	ldr	r3, [r3, #-3908]
	blx	r3
	cmp	r0, #1
	bne	.L1252
	ldr	ip, [fp, #-56]
	mov	r0, r4
.L1253:
	ldr	r2, [fp, #-52]
	mov	r1, #4
	ldr	r3, [fp, #-48]
	str	ip, [r7]
	stmib	r7, {r2, r3}
	bl	VDMDRV_SetChanIsDec
	ldr	r3, [r6, #16]
	mov	r0, #5
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, r5
	blx	r3
.L1245:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1255:
	ldr	r8, .L1263+12
	ldr	r9, [r8, #64]
	cmp	r9, #0
	blt	.L1236
	add	r4, r8, #64
	ldr	r10, .L1263+8
	add	r8, r8, #124
	b	.L1237
.L1259:
	mov	r3, r9
	ldr	r2, .L1263+16
	ldr	r1, .L1263+20
	mov	r0, #1
	bl	dprint_vfmw
.L1239:
	cmp	r4, r8
	beq	.L1236
.L1246:
	ldr	r9, [r4, #4]!
	cmp	r9, #0
	blt	.L1236
.L1237:
	mov	r0, r9
	bl	VCTRL_IsChanActive
	subs	r5, r0, #0
	bne	.L1259
	mov	r0, r9
	bl	VDMDRV_IsChanDec
	cmp	r0, #1
	beq	.L1260
	mov	r0, r9
	bl	VCTRL_GetDecParam
	subs	r1, r0, #0
	beq	.L1261
	cmp	r9, #15
	bhi	.L1242
	ldr	r3, .L1263+4
	ldr	r2, [r3, r9, asl #2]
	cmp	r2, #0
	beq	.L1242
	ldr	r2, [r2, #600]
	sub	r0, fp, #56
	str	r9, [fp, #-56]
	str	r1, [fp, #-48]
	str	r2, [fp, #-52]
	ldr	r2, [r10, #-3908]
	blx	r2
	cmp	r0, #1
	beq	.L1262
	cmp	r4, r8
	str	r5, [r7, #8]
	bne	.L1246
.L1236:
	ldr	r3, [r6, #16]
	mov	r0, #5
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, #0
	mvn	r5, #0
	blx	r3
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1242:
	mov	r3, r9
	movw	r2, #8237
	ldr	r1, .L1263+24
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1239
.L1260:
	mov	r2, r9
	ldr	r1, .L1263+28
	mov	r0, #22
	bl	dprint_vfmw
	b	.L1239
.L1257:
	mov	r2, r4
	ldr	r1, .L1263+28
	mov	r0, #22
	bl	dprint_vfmw
.L1252:
	str	r5, [r7, #8]
	mov	r0, #5
	ldr	r3, [r6, #16]
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, r5
	mvn	r5, #0
	blx	r3
	b	.L1245
.L1261:
	mov	r2, r9
	ldr	r1, .L1263+32
	mov	r0, #22
	bl	dprint_vfmw
	b	.L1239
.L1262:
	ldr	ip, [fp, #-56]
	mov	r0, r9
	b	.L1253
.L1256:
	ldr	r2, .L1263+16
	mov	r3, r4
	ldr	r1, .L1263+36
.L1254:
	mov	r0, #1
	mov	r4, #0
	bl	dprint_vfmw
	str	r4, [r7, #8]
	mov	r0, #5
	ldr	r3, [r6, #16]
	blx	r3
	mov	r0, r4
	ldr	r3, [r6, #16]
	mvn	r5, #0
	blx	r3
	b	.L1245
.L1250:
	mov	r3, r4
	movw	r2, #8322
	ldr	r1, .L1263+24
	b	.L1254
.L1258:
	ldr	r3, [r6, #124]
	blx	r3
	mov	r2, r4
	ldr	r1, .L1263+32
	mov	r0, #22
	bl	dprint_vfmw
	b	.L1252
.L1264:
	.align	2
.L1263:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR4
	.word	g_ChanCtx
	.word	.LANCHOR1+280
	.word	.LC118
	.word	.LC47
	.word	.LC119
	.word	.LC120
	.word	.LC121
	UNWIND(.fnend)
	.size	VCTRL_ExtAccGetDecParam, .-VCTRL_ExtAccGetDecParam
	.align	2
	.global	VCTRL_GetLastFrameIdPlus2
	.type	VCTRL_GetLastFrameIdPlus2, %function
VCTRL_GetLastFrameIdPlus2:
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
	cmp	r0, #15
	bhi	.L1269
	ldr	r3, .L1270
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #376]
	mvneq	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1269:
	ldr	r3, .L1270+4
	movw	r2, #9281
	ldr	r1, .L1270+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1271:
	.align	2
.L1270:
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_GetLastFrameIdPlus2, .-VCTRL_GetLastFrameIdPlus2
	.align	2
	.global	VCTRL_SetLastFrameIdPlus2
	.type	VCTRL_SetLastFrameIdPlus2, %function
VCTRL_SetLastFrameIdPlus2:
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
	cmp	r0, #15
	bhi	.L1278
	ldr	r3, .L1279
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	strne	r1, [r3, #376]
	ldmfd	sp, {fp, sp, pc}
.L1278:
	ldr	r3, .L1279+4
	movw	r2, #9293
	ldr	r1, .L1279+8
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L1280:
	.align	2
.L1279:
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_SetLastFrameIdPlus2, .-VCTRL_SetLastFrameIdPlus2
	.align	2
	.global	VCTRL_SetLastFrameState
	.type	VCTRL_SetLastFrameState, %function
VCTRL_SetLastFrameState:
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
	cmp	r0, #15
	bhi	.L1287
	ldr	r3, .L1288
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	strne	r1, [r3, #380]
	ldmfd	sp, {fp, sp, pc}
.L1287:
	ldr	r3, .L1288+4
	movw	r2, #9305
	ldr	r1, .L1288+8
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L1289:
	.align	2
.L1288:
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_SetLastFrameState, .-VCTRL_SetLastFrameState
	.align	2
	.global	VCTRL_ReportLastFrame
	.type	VCTRL_ReportLastFrame, %function
VCTRL_ReportLastFrame:
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
	UNWIND(.pad #20)
	sub	sp, sp, #20
	subs	r5, r1, #0
	mov	r4, r0
	moveq	r2, #2
	beq	.L1292
	cmp	r5, #1
	moveq	r2, #3
	movne	r2, #4
.L1292:
	cmp	r4, #15
	bhi	.L1304
	ldr	r6, .L1305
	ldr	r3, [r6, r4, asl #2]
	cmp	r3, #0
	strne	r2, [r3, #380]
.L1295:
	ldr	r3, .L1305+4
	ldr	ip, [r3, #-792]
	cmp	ip, #0
	beq	.L1296
	sub	r2, fp, #28
	mov	r3, #4
	mov	r1, #115
	mov	r0, r4
	str	r5, [r2, #-4]!
	blx	ip
.L1296:
	ldr	ip, [r6, r4, asl #2]
	mov	r3, r5
	mov	r2, r4
	ldr	r1, .L1305+8
	mov	r0, #1
	ldr	ip, [ip, #380]
	str	ip, [sp]
	bl	dprint_vfmw
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1304:
	ldr	r3, .L1305+12
	movw	r2, #9305
	ldr	r1, .L1305+16
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r6, .L1305
	b	.L1295
.L1306:
	.align	2
.L1305:
	.word	.LANCHOR0
	.word	.LANCHOR2
	.word	.LC122
	.word	.LC12
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ReportLastFrame, .-VCTRL_ReportLastFrame
	.align	2
	.global	VCTRL_OutputLastFrame
	.type	VCTRL_OutputLastFrame, %function
VCTRL_OutputLastFrame:
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
	ldr	r3, .L1333
	mov	r5, r0
	ldr	r3, [r3, r0, asl #2]
	add	r0, r3, #294912
	ldr	r2, [r0, #1960]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L1327
.L1310:
	.word	.L1309
	.word	.L1311
	.word	.L1312
	.word	.L1313
	.word	.L1327
	.word	.L1314
	.word	.L1315
	.word	.L1327
	.word	.L1316
	.word	.L1317
	.word	.L1318
	.word	.L1318
	.word	.L1318
	.word	.L1319
	.word	.L1327
	.word	.L1320
	.word	.L1321
	.word	.L1322
.L1322:
	add	r0, r0, #1968
	bl	VP9DEC_GetRemainImg
	mov	r4, r0
.L1308:
	cmn	r4, #1
	beq	.L1324
	cmp	r4, #0
	bne	.L1332
	mov	r0, r5
	mov	r1, r4
	bl	VCTRL_ReportLastFrame
	ldr	r1, .L1333+4
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1309:
	add	r0, r0, #1968
	bl	H264DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1311:
	add	r0, r0, #1968
	bl	VC1DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1312:
	add	r0, r0, #1968
	bl	MPEG4DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1313:
	add	r0, r0, #1968
	bl	MPEG2DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1314:
	add	r0, r0, #1968
	bl	DIVX3DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1315:
	add	r0, r0, #1968
	bl	AVSDEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1316:
	add	r0, r0, #1968
	bl	REAL8DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1317:
	add	r0, r0, #1968
	bl	REAL9DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1318:
	add	r0, r0, #1968
	bl	VP6DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1319:
	add	r0, r0, #1968
	bl	VP8DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1320:
	add	r0, r0, #1968
	bl	MVC_DEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1321:
	add	r0, r0, #1968
	bl	HEVCDEC_GetRemainImg
	mov	r4, r0
	b	.L1308
.L1332:
	mov	r0, r5
	mov	r1, r4
	bl	VCTRL_ReportLastFrame
	sub	r2, r4, #2
	ldr	r1, .L1333+8
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1327:
	mvn	r4, #0
.L1324:
	mov	r0, r5
	mov	r1, #1
	bl	VCTRL_ReportLastFrame
	ldr	r1, .L1333+12
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1334:
	.align	2
.L1333:
	.word	.LANCHOR0
	.word	.LC124
	.word	.LC125
	.word	.LC123
	UNWIND(.fnend)
	.size	VCTRL_OutputLastFrame, .-VCTRL_OutputLastFrame
	.align	2
	.global	VCTRL_PostProc
	.type	VCTRL_PostProc, %function
VCTRL_PostProc:
	UNWIND(.fnstart)
	@ args = 4, pretend = 0, frame = 0
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
	ldr	r5, .L1375
	mov	r4, r0
	ldr	lr, [fp, #4]
	ldr	r0, [r5, r0, asl #2]
	cmp	r0, #0
	beq	.L1368
	ldr	ip, [r0, #600]
	ldr	r6, [r0, #1224]
	cmp	ip, #17
	ldrls	pc, [pc, ip, asl #2]
	b	.L1335
.L1339:
	.word	.L1338
	.word	.L1340
	.word	.L1341
	.word	.L1342
	.word	.L1335
	.word	.L1343
	.word	.L1344
	.word	.L1335
	.word	.L1345
	.word	.L1346
	.word	.L1347
	.word	.L1347
	.word	.L1347
	.word	.L1348
	.word	.L1335
	.word	.L1349
	.word	.L1350
	.word	.L1351
.L1349:
	add	r0, r0, #294912
	mov	r2, #1
	add	r0, r0, #1968
	bl	MVC_DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	ldr	r5, .L1375
	add	r3, r3, #294912
	ldrb	r2, [r3, #1969]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1369
.L1360:
	mov	r2, #0
	str	r2, [r3, #2196]
.L1335:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1351:
	add	r0, r0, #294912
	mov	r3, lr
	add	r0, r0, #1968
	mov	r2, r6
	bl	VP9DEC_VDMPostProc
	cmp	r6, #1
	bne	.L1367
	b	.L1335
.L1338:
	add	r0, r0, #294912
	str	lr, [sp, #4]
	add	r0, r0, #1968
	str	r6, [sp]
	mov	r2, #1
	ldr	r7, .L1375
	bl	H264DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #307200
	ldrb	r3, [r3, #3240]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1370
.L1352:
	cmp	r6, #1
	ldrne	r3, [r5, r4, asl #2]
	movne	r2, #0
	addne	r3, r3, #294912
	strne	r2, [r3, #2328]
	b	.L1335
.L1341:
	add	r0, r0, #294912
	mov	r2, r3
	add	r0, r0, #1968
	bl	MPEG4DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	ldr	r5, .L1375
	add	r2, r3, #319488
	ldrb	r2, [r2, #412]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1371
.L1357:
	add	r3, r3, #294912
	mov	r2, #0
	str	r2, [r3, #3004]
	b	.L1335
.L1342:
	add	r0, r0, #294912
	ldr	r6, .L1375
	add	r0, r0, #1968
	bl	MPEG2DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r2, r3, #380928
	ldrb	r2, [r2, #4034]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1372
.L1359:
	add	r3, r3, #294912
	mov	r2, #0
	str	r2, [r3, #1972]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1340:
	add	r0, r0, #294912
	ldr	r6, .L1375
	add	r0, r0, #1968
	bl	VC1DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r2, r3, #307200
	ldrb	r2, [r2, #2415]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1359
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #307200
	strb	r1, [r2, #2415]
	b	.L1359
.L1343:
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	DIVX3DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #294912
	str	r2, [r3, #1972]
	b	.L1335
.L1344:
	add	r0, r0, #294912
	ldr	r6, .L1375
	add	r0, r0, #1968
	bl	AVSDEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #294912
	ldrb	r2, [r3, #1968]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1373
.L1358:
	mov	r2, #0
	str	r2, [r3, #1984]
	b	.L1335
.L1345:
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	REAL8DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #294912
	str	r2, [r3, #1972]
	b	.L1335
.L1346:
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	REAL9DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #294912
	str	r2, [r3, #1972]
	b	.L1335
.L1347:
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	VP6DEC_VDMPostProc
.L1367:
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #294912
	str	r2, [r3, #1972]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1350:
	add	r0, r0, #294912
	str	lr, [sp, #4]
	add	r0, r0, #1968
	str	r6, [sp]
	mov	r2, #1
	ldr	r7, .L1375
	bl	HEVCDEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #294912
	ldrb	r3, [r3, #1975]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1374
.L1354:
	cmp	r6, #1
	ldrne	r3, [r5, r4, asl #2]
	movne	r2, #0
	addne	r3, r3, #299008
	strne	r2, [r3, #200]
	b	.L1335
.L1348:
	add	r0, r0, #294912
	add	r0, r0, #1968
	bl	VP8DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #294912
	str	r2, [r3, #1972]
	b	.L1335
.L1368:
	ldr	r3, .L1375+4
	mov	r2, #5184
	ldr	r1, .L1375+8
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1373:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r2, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r2, #294912
	mov	r3, r2
	strb	r1, [r2, #1968]
	b	.L1358
.L1372:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #380928
	strb	r1, [r2, #4034]
	b	.L1359
.L1371:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r5, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #319488
	strb	r1, [r2, #412]
	b	.L1357
.L1374:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #294912
	strb	r2, [r3, #1975]
	b	.L1354
.L1369:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r2, [r5, r4, asl #2]
	mov	r1, #0
	add	r2, r2, #294912
	mov	r3, r2
	strb	r1, [r2, #1969]
	b	.L1360
.L1370:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #307200
	strb	r2, [r3, #3240]
	b	.L1352
.L1376:
	.align	2
.L1375:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_PostProc, .-VCTRL_PostProc
	.align	2
	.global	VCTRL_VdmPostProc
	.type	VCTRL_VdmPostProc, %function
VCTRL_VdmPostProc:
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
	cmp	r0, #15
	mov	r6, r2
	mov	r7, r3
	mov	r4, r0
	mov	r5, r1
	ldr	r8, [fp, #4]
	ldrhi	r3, .L1384
	movwhi	r2, #5374
	bhi	.L1382
	ldr	ip, .L1384+4
	ldr	ip, [ip, r0, asl #2]
	cmp	ip, #0
	beq	.L1383
	ldr	ip, [ip, #16]
	cmp	ip, #1
	beq	.L1380
	ldr	r3, .L1384+8
	mov	r2, #5376
.L1382:
	ldr	r1, .L1384+12
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L1380:
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	str	r8, [fp, #4]
	mov	r3, r7
	mov	r2, r6
	mov	r1, r5
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VCTRL_PostProc
.L1383:
	mov	r0, ip
	ldr	r3, .L1384+16
	movw	r2, #5375
	ldr	r1, .L1384+12
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L1385:
	.align	2
.L1384:
	.word	.LC51
	.word	.LANCHOR0
	.word	.LC52
	.word	.LC1
	.word	.LC38
	UNWIND(.fnend)
	.size	VCTRL_VdmPostProc, .-VCTRL_VdmPostProc
	.align	2
	.global	VCTRL_ExtAccPostProcess
	.type	VCTRL_ExtAccPostProcess, %function
VCTRL_ExtAccPostProcess:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 136
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #144)
	sub	sp, sp, #144
	ldr	r4, .L1387
	mov	r6, r0
	mov	r2, #136
	mov	r7, r1
	sub	r0, fp, #164
	mov	r1, #0
	ldr	r3, [r4, #48]
	blx	r3
	ldr	r3, [r4, #12]
	mov	r0, #0
	mov	r5, r0
	blx	r3
	ldr	r3, [r4, #12]
	mov	r0, #5
	blx	r3
	mov	r1, r7
	mov	r2, r5
	sub	r3, fp, #164
	mov	r0, r6
	str	r5, [sp]
	bl	VCTRL_VdmPostProc
	mov	r0, r6
	bl	VDMDRV_ClearChanIsDec
	ldr	r3, [r4, #16]
	mov	r0, #5
	blx	r3
	ldr	r3, [r4, #16]
	mov	r0, r5
	blx	r3
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1388:
	.align	2
.L1387:
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	VCTRL_ExtAccPostProcess, .-VCTRL_ExtAccPostProcess
	.align	2
	.global	VCTRL_SvdecLastFrame
	.type	VCTRL_SvdecLastFrame, %function
VCTRL_SvdecLastFrame:
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
	add	r1, r1, #2
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_ReportLastFrame
	UNWIND(.fnend)
	.size	VCTRL_SvdecLastFrame, .-VCTRL_SvdecLastFrame
	.align	2
	.global	Get_YuvFormat
	.type	Get_YuvFormat, %function
Get_YuvFormat:
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
	cmp	r1, #6
	ldrls	pc, [pc, r1, asl #2]
	b	.L1391
.L1393:
	.word	.L1392
	.word	.L1394
	.word	.L1395
	.word	.L1396
	.word	.L1397
	.word	.L1398
	.word	.L1399
.L1399:
	cmp	r0, #1
	moveq	r0, #1
	movne	r0, #7
	ldmfd	sp, {fp, sp, pc}
.L1392:
	cmp	r0, #1
	moveq	r0, #0
	movne	r0, #6
	ldmfd	sp, {fp, sp, pc}
.L1394:
	cmp	r0, #1
	moveq	r0, #2
	movne	r0, #8
	ldmfd	sp, {fp, sp, pc}
.L1395:
	cmp	r0, #1
	moveq	r0, #4
	movne	r0, #10
	ldmfd	sp, {fp, sp, pc}
.L1396:
	cmp	r0, #1
	moveq	r0, #3
	movne	r0, #9
	ldmfd	sp, {fp, sp, pc}
.L1397:
	cmp	r0, #1
	moveq	r0, #5
	movne	r0, #11
	ldmfd	sp, {fp, sp, pc}
.L1398:
	cmp	r0, #1
	moveq	r0, #13
	movne	r0, #12
	ldmfd	sp, {fp, sp, pc}
.L1391:
	mov	r3, r1
	movw	r2, #9829
	ldr	r1, .L1408
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #13
	ldmfd	sp, {fp, sp, pc}
.L1409:
	.align	2
.L1408:
	.word	.LC126
	UNWIND(.fnend)
	.size	Get_YuvFormat, .-Get_YuvFormat
	.align	2
	.global	VDH_PutUsrdecFrame
	.type	VDH_PutUsrdecFrame, %function
VDH_PutUsrdecFrame:
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
	ldr	r1, [r1, #40]
	mov	r8, r0
	bl	FSP_GetLogicIdByPhyAddr
	cmn	r0, #1
	mov	r7, r0
	beq	.L1418
	ldr	r3, [r5, #4]
	cmp	r3, #1
	beq	.L1413
	mov	r1, r0
	mov	r2, #0
	mov	r0, r8
	bl	FSP_ClearLogicFs
	mov	r0, #0
.L1412:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1413:
	ldr	r0, [r5, #48]
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L1419
.L1414:
	mov	r1, r7
	mov	r0, r8
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L1420
	str	r7, [r4, #156]
	add	lr, r4, #592
	ldrd	r2, [r5, #64]
	add	r0, r4, #608
	add	ip, r4, #624
	mov	r6, #0
	mov	r10, #1
	add	r1, r4, #528
	strd	r2, [lr]
	ldrd	r2, [r5, #64]
	str	r1, [fp, #-52]
	strd	r2, [r0, #-8]
	ldrd	r2, [r5, #64]
	strd	r2, [r0]
	ldrd	r2, [r5, #64]
	strd	r2, [ip, #-8]
	str	r10, [r4, #8]
	str	r6, [r4, #12]
	str	r6, [r4, #16]
	str	r6, [r4, #20]
	ldr	r3, [r5, #40]
	str	r3, [r4, #28]
	mov	r0, r3
	str	r3, [r4, #60]
	str	r3, [r4, #44]
	ldr	r3, [r5, #48]
	str	r3, [r4, #32]
	str	r3, [r4, #64]
	str	r3, [r4, #48]
	bl	MEM_Phy2Vir
	ldr	r1, [fp, #-52]
	stmdb	r1, {r0, r6}
	str	r0, [r4, #504]
	str	r6, [r4, #508]
	ldr	r0, [r4, #32]
	bl	MEM_Phy2Vir
	ldr	r1, [fp, #-52]
	movw	ip, #11296
	str	r0, [r4, #528]
	str	r6, [r1, #4]
	str	r0, [r4, #512]
	str	r6, [r4, #516]
	ldr	r3, [r4, #28]
	ldr	r2, [r5, #44]
	ldr	r1, [r4, #32]
	add	r2, r2, r3
	str	r2, [r4, #36]
	ldr	r3, [r5, #44]
	str	r6, [r4, #96]
	add	r3, r3, r1
	str	r3, [r4, #40]
	ldr	r3, [r5, #12]
	str	r3, [r4, #112]
	ldr	r3, [r5, #16]
	str	r3, [r4, #116]
	ldr	r3, [r5, #12]
	str	r3, [r4, #120]
	mov	r2, r3, lsr r10
	ldr	r3, [r5, #16]
	str	r2, [r4, #128]
	str	r3, [r4, #124]
	mov	r3, r3, lsr r10
	str	r3, [r4, #132]
	ldr	r3, [r5, #44]
	str	ip, [r4, #108]
	str	r6, [r4, #136]
	str	r3, [r4, #140]
	str	r6, [r4, #160]
	str	r10, [r4, #172]
	ldr	r0, [r5, #24]
	str	r0, [r4, #220]
	bl	MEM_Phy2Vir
	str	r6, [r4, #540]
	str	r0, [r4, #536]
	ldr	r1, [r5]
	ldr	r0, [r5, #8]
	bl	Get_YuvFormat
	cmp	r0, #13
	beq	.L1421
	str	r0, [r4, #468]
	mov	r2, r10
	ldr	r3, [r5, #56]
	mov	r9, #20
	mov	r1, r7
	mov	r0, r8
	str	r3, [r4, #444]
	ldr	r3, [r5, #60]
	str	r3, [r4, #440]
	ldr	r3, [r5, #52]
	str	r9, [r4, #376]
	str	r3, [r4, #448]
	bl	FSP_SetDisplay
	ldr	r2, .L1422
	str	r4, [sp]
	movw	r3, #34744
	mov	r1, r9
	movt	r3, 4
	ldr	ip, [r2, r8, asl #2]
	mov	r0, r8
	add	r2, ip, #294912
	add	r3, ip, r3
	add	r2, r2, #1968
	bl	InsertImgToVoQueue
	cmp	r0, #1
	moveq	r0, r6
	beq	.L1412
	add	r9, r4, #560
	ldr	r1, [r4, #560]
	mov	r0, r8
	add	r5, r4, #576
	bl	FreeUsdByDec
	mov	r2, #0
	mov	r3, #0
	mov	r0, r8
	strd	r2, [r9]
	ldr	r1, [r4, #568]
	strd	r2, [fp, #-52]
	bl	FreeUsdByDec
	ldrd	r2, [fp, #-52]
	mov	r0, r8
	mov	r1, r7
	strd	r2, [r5, #-8]
	mov	r2, r6
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L1412
.L1419:
	ldr	r3, [r5, #4]
	cmp	r3, #1
	bne	.L1414
	ldr	r2, [r5, #48]
	ldr	r1, .L1422+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1412
.L1418:
	ldr	r2, [r5, #40]
	mov	r0, #0
	ldr	r1, .L1422+8
	bl	dprint_vfmw
	mov	r0, r7
	b	.L1412
.L1421:
	mov	r2, r0
	ldr	r1, .L1422+12
	mov	r0, r10
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1412
.L1420:
	movw	r2, #5864
	ldr	r1, .L1422+16
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1412
.L1423:
	.align	2
.L1422:
	.word	.LANCHOR0
	.word	.LC128
	.word	.LC127
	.word	.LC130
	.word	.LC129
	UNWIND(.fnend)
	.size	VDH_PutUsrdecFrame, .-VDH_PutUsrdecFrame
	.align	2
	.global	VCTRL_PutUsrdecFrame
	.type	VCTRL_PutUsrdecFrame, %function
VCTRL_PutUsrdecFrame:
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
	ldr	r3, .L1435
	mov	r4, r0
	mov	r5, r1
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1432
	cmp	r1, #0
	beq	.L1433
	ldr	r2, [r0, #600]
	cmp	r2, #20
	bne	.L1434
	ldr	ip, .L1435+4
	mov	r2, #72
	add	r0, r0, #528
	ldr	r3, [ip, #52]
	blx	r3
	mov	r1, r5
	mov	r0, r4
	bl	VDH_PutUsrdecFrame
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, .L1435+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1434:
	ldr	r1, .L1435+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1433:
	mov	r0, r1
	ldr	r3, .L1435+16
	movw	r2, #6077
	ldr	r1, .L1435+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1432:
	ldr	r3, .L1435+24
	movw	r2, #6076
	ldr	r1, .L1435+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1436:
	.align	2
.L1435:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC133
	.word	.LC132
	.word	.LC131
	.word	.LC1
	.word	.LC2
	UNWIND(.fnend)
	.size	VCTRL_PutUsrdecFrame, .-VCTRL_PutUsrdecFrame
	.align	2
	.global	VCTRL_ResetHardware
	.type	VCTRL_ResetHardware, %function
VCTRL_ResetHardware:
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
	bl	SCDDRV_ResetSCD
	mov	r0, r4
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	b	VDMDRV_ResetVDH
	UNWIND(.fnend)
	.size	VCTRL_ResetHardware, .-VCTRL_ResetHardware
	.align	2
	.global	VCTRL_OpenHardware
	.type	VCTRL_OpenHardware, %function
VCTRL_OpenHardware:
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
	cmp	r0, #15
	mov	r5, r0
	movwhi	r2, #9863
	ldrhi	r1, .L1486
	bhi	.L1478
	ldr	r6, .L1486+4
	ldr	r0, [r6, r0, asl #2]
	cmp	r0, #0
	beq	.L1479
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L1441
	ldr	r3, [r0, #600]
	cmp	r3, #21
	beq	.L1441
	bic	r7, r3, #16
	cmp	r7, #4
	cmpne	r3, #7
	moveq	r7, #1
	movne	r7, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	cmp	r3, #14
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r4, r7
.L1447:
	mov	r0, r4
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L1446
	ldr	r3, [r6, r4, asl #2]
	ldr	r3, [r3, #600]
	cmp	r3, #14
	cmpne	r3, #7
	bic	r3, r3, #16
	movne	r2, #1
	moveq	r2, #0
	cmp	r3, #4
	moveq	r3, #0
	andne	r3, r2, #1
	cmp	r3, #0
	addne	r7, r7, #1
.L1446:
	add	r4, r4, #1
	cmp	r4, #16
	bne	.L1447
	cmp	r7, #1
	beq	.L1480
.L1448:
	ldr	r1, .L1486+8
	mov	ip, #0
	add	lr, r1, #64
.L1456:
	ldr	r2, [r1, #4]!
	mov	r3, #0
	cmp	r2, r3
	beq	.L1454
	ldr	r0, [r2, #600]
	cmp	r0, r3
	ldreq	r2, [r2, #1228]
	subeq	r3, r2, #1
	clzeq	r3, r3
	moveq	r3, r3, lsr #5
	cmp	r0, #6
	orreq	r3, r3, #1
	cmp	r3, #0
	movne	ip, #1
.L1454:
	cmp	r1, lr
	bne	.L1456
	cmp	ip, #1
	beq	.L1481
.L1457:
	ldr	r3, [r6, r5, asl #2]
	ldr	r3, [r3, #600]
	cmp	r3, #1
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, .L1486+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1482
	ldr	r3, .L1486+16
	ldr	r3, [r3, #28]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L1441:
	ldr	r1, .L1486+20
	movw	r2, #9875
.L1478:
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1482:
	mov	r1, #0
	mov	r0, #2
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VDH_Need_Module
.L1481:
	ldr	r3, .L1486+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1458
	ldr	r3, .L1486+16
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L1483
	mov	r0, #0
	blx	r3
	b	.L1457
.L1480:
	ldr	r4, .L1486+12
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1449
	ldr	r3, .L1486+16
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1484
	mov	r0, #0
	blx	r3
.L1451:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1452
	ldr	r3, .L1486+16
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq	.L1485
	mov	r0, #0
	blx	r3
	b	.L1448
.L1479:
	movw	r2, #9869
	ldr	r1, .L1486+24
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1452:
	mov	r1, #0
	mov	r0, r1
	bl	VDH_Need_Module
	b	.L1448
.L1449:
	mov	r0, r7
	mov	r1, #0
	bl	VDH_Need_Module
	b	.L1451
.L1458:
	mov	r1, #0
	mov	r0, #3
	bl	VDH_Need_Module
	b	.L1457
.L1483:
	mov	r0, r3
	movw	r2, #9966
	ldr	r1, .L1486+28
	bl	dprint_vfmw
	b	.L1457
.L1485:
	mov	r0, r3
	movw	r2, #9931
	ldr	r1, .L1486+28
	bl	dprint_vfmw
	b	.L1448
.L1484:
	mov	r0, r3
	movw	r2, #9917
	ldr	r1, .L1486+28
	bl	dprint_vfmw
	b	.L1451
.L1487:
	.align	2
.L1486:
	.word	.LC134
	.word	.LANCHOR0
	.word	.LANCHOR0-4
	.word	g_HalDisable
	.word	g_vdm_hal_fun_ptr
	.word	.LC136
	.word	.LC135
	.word	.LC6
	UNWIND(.fnend)
	.size	VCTRL_OpenHardware, .-VCTRL_OpenHardware
	.align	2
	.global	VCTRL_StartChan
	.type	VCTRL_StartChan, %function
VCTRL_StartChan:
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
	cmp	r0, #15
	mov	r5, r0
	bhi	.L1499
	ldr	r4, .L1503
	ldr	r3, [r4, r0, asl #2]
	cmp	r3, #0
	beq	.L1500
	ldr	r6, [r3, #12]
	cmp	r6, #1
	bne	.L1501
	ldr	r3, [r3, #600]
	cmp	r3, #21
	beq	.L1502
	bl	SM_Start
	ldr	r3, [r4, r5, asl #2]
	mov	r2, #0
	str	r6, [r3, #16]
	ldr	r3, [r4, r5, asl #2]
	str	r6, [r3, #20]
	ldr	r3, [r4, r5, asl #2]
	str	r2, [r3, #32]
	ldr	r3, [r4, r5, asl #2]
	str	r6, [r3, #24]
	ldr	r2, [r4, r5, asl #2]
	ldr	r3, [r2, #28]
	cmp	r3, #0
	bne	.L1494
	ldr	r3, .L1503+4
	sub	ip, r4, #4
	ldr	r1, .L1503+8
	add	r0, r3, #64
	str	r6, [r2, #28]
.L1497:
	ldr	r2, [ip, #4]!
	add	r3, r3, #4
	add	r1, r1, #4
	cmp	r2, #0
	streq	r2, [r3, #-4]
	ldrne	lr, [r2, #28]
	strne	lr, [r3, #-4]
	ldrne	r2, [r2, #28]
	cmp	r3, r0
	str	r2, [r1, #-4]
	bne	.L1497
	ldr	r0, .L1503+4
	bl	VDMDRV_SetPriority
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
.L1494:
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r5
	bl	VCTRL_OpenHardware
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1501:
	ldr	r3, .L1503+12
	movw	r2, #2893
	ldr	r1, .L1503+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1499:
	ldr	r3, .L1503+20
	movw	r2, #2891
	ldr	r1, .L1503+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1502:
	ldr	r3, .L1503+24
	movw	r2, #2894
	ldr	r1, .L1503+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1500:
	mov	r0, r3
	movw	r2, #2892
	ldr	r3, .L1503+28
	ldr	r1, .L1503+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1504:
	.align	2
.L1503:
	.word	.LANCHOR0
	.word	.LANCHOR4-3904
	.word	g_ChanCtx
	.word	.LC57
	.word	.LC1
	.word	.LC12
	.word	.LC137
	.word	.LC19
	UNWIND(.fnend)
	.size	VCTRL_StartChan, .-VCTRL_StartChan
	.align	2
	.global	VCTRL_CloseHardware
	.type	VCTRL_CloseHardware, %function
VCTRL_CloseHardware:
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
	ldr	ip, .L1563
	mov	r5, #0
	mov	r8, r0
	mov	r4, r5
	add	r6, ip, #64
	mov	r7, r5
	b	.L1511
.L1552:
	ldr	r1, [r3, #12]
	cmp	r1, #1
	beq	.L1550
.L1508:
	cmp	r2, #0
	ldreq	lr, [r3, #1228]
	movne	lr, #0
	cmp	r5, #0
	bne	.L1506
	cmp	r2, #6
	cmpne	lr, #1
	bne	.L1506
	ldr	r2, [r3, #12]
	cmp	r2, #1
	ldreq	r5, [r3, #16]
	subeq	r5, r5, #1
	clzeq	r5, r5
	moveq	r5, r5, lsr #5
.L1506:
	cmp	ip, r6
	beq	.L1551
.L1511:
	ldr	r3, [ip, #4]!
	cmp	r3, #0
	beq	.L1506
	cmp	r7, #0
	ldr	r2, [r3, #600]
	beq	.L1552
	cmp	r4, #0
	cmpeq	r2, #1
	bne	.L1508
	ldr	r1, [r3, #12]
	cmp	r1, #1
	bne	.L1508
	ldr	r1, [r3, #16]
.L1529:
	cmp	r1, #1
	moveq	r4, #1
	b	.L1508
.L1550:
	ldr	r1, [r3, #16]
	cmp	r1, #1
	beq	.L1553
.L1509:
	cmp	r4, #0
	cmpeq	r2, #1
	beq	.L1529
	b	.L1508
.L1551:
	cmp	r7, #0
	beq	.L1554
.L1512:
	cmp	r5, #0
	bne	.L1524
	ldr	r3, .L1563+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1555
	mov	r1, r5
	mov	r0, #3
	bl	VDH_UnNeed_Module
.L1524:
	cmp	r4, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1562:
	ldr	r3, .L1563+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1556
	ldr	r3, .L1563+8
	ldr	r3, [r3, #32]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L1554:
	ldr	r6, .L1563+4
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1517
	mov	r0, r8
	bl	SCDDRV_ResetSCD
	mov	r0, r8
	bl	VDMDRV_ResetVDH
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1517
	mov	r1, r7
	mov	r0, #1
	bl	VDH_UnNeed_Module
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1520
.L1560:
	ldr	r3, .L1563+8
	ldr	r3, [r3, #24]
	cmp	r3, #0
	beq	.L1557
	mov	r0, #0
	blx	r3
.L1523:
	mov	r0, #0
	bl	SCDDRV_SetStateIdle
	b	.L1512
.L1553:
	sub	r0, r2, #5
	cmp	r0, #1
	cmphi	r2, #3
	bhi	.L1558
.L1531:
	mov	r7, r1
	b	.L1509
.L1558:
	sub	r0, r2, #8
	cmp	r0, #5
	bls	.L1531
	sub	r7, r2, #15
	cmp	r7, #2
	movhi	r7, #0
	movls	r7, #1
	b	.L1509
.L1556:
	mov	r1, r4
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VDH_UnNeed_Module
.L1517:
	ldr	r3, .L1563+8
	ldr	r3, [r3, #8]
	cmp	r3, #0
	beq	.L1559
	mov	r0, #0
	blx	r3
.L1519:
	mov	r0, #0
	bl	VDMDRV_SetStateIdle
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1560
.L1520:
	mov	r1, #0
	mov	r0, r1
	bl	VDH_UnNeed_Module
	b	.L1512
.L1555:
	ldr	r3, .L1563+8
	mov	r0, r5
	ldr	r3, [r3, #16]
	cmp	r3, #0
	beq	.L1561
	blx	r3
	cmp	r4, #0
	beq	.L1562
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1561:
	movw	r2, #10108
	ldr	r1, .L1563+12
	bl	dprint_vfmw
	b	.L1524
.L1557:
	mov	r0, r3
	movw	r2, #10090
	ldr	r1, .L1563+12
	bl	dprint_vfmw
	b	.L1523
.L1559:
	mov	r0, r3
	movw	r2, #10075
	ldr	r1, .L1563+12
	bl	dprint_vfmw
	b	.L1519
.L1564:
	.align	2
.L1563:
	.word	.LANCHOR0-4
	.word	g_HalDisable
	.word	g_vdm_hal_fun_ptr
	.word	.LC6
	UNWIND(.fnend)
	.size	VCTRL_CloseHardware, .-VCTRL_CloseHardware
	.align	2
	.global	VCTRL_StopChan
	.type	VCTRL_StopChan, %function
VCTRL_StopChan:
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
	cmp	r0, #15
	mov	r4, r0
	bhi	.L1571
	ldr	r6, .L1573
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	beq	.L1572
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L1570
	str	r2, [r3, #32]
	mov	r3, #0
	ldr	r2, [r6, r0, asl #2]
	str	r3, [r2, #20]
	ldr	r2, [r6, r0, asl #2]
	str	r3, [r2, #16]
	bl	SM_Stop
	subs	r5, r0, #0
	bne	.L1570
	mov	r1, r5
	mov	r0, r4
	bl	DeleteRawPacketInBuffer
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	ldr	r3, [r6, r4, asl #2]
	mov	r0, r4
	str	r5, [r3, #32]
	bl	VCTRL_CloseHardware
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1570:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1571:
	ldr	r3, .L1573+4
	movw	r2, #3000
	ldr	r1, .L1573+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1572:
	mov	r0, r3
	movw	r2, #3001
	ldr	r3, .L1573+12
	ldr	r1, .L1573+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1574:
	.align	2
.L1573:
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC1
	.word	.LC138
	UNWIND(.fnend)
	.size	VCTRL_StopChan, .-VCTRL_StopChan
	.align	2
	.global	VCTRL_StopChanWithCheck
	.type	VCTRL_StopChanWithCheck, %function
VCTRL_StopChanWithCheck:
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
	ldr	r5, .L1582
	mov	r7, r0
	mov	r4, #0
	mov	r8, r5
	b	.L1577
.L1581:
	add	r4, r4, #1
	ldr	r3, [r8, #116]
	blx	r3
	cmp	r4, #50
	beq	.L1579
.L1577:
	ldr	r3, [r5, #12]
	mov	r0, #0
	blx	r3
	mov	r0, r7
	bl	VCTRL_StopChan
	ldr	r3, [r5, #16]
	mov	r6, r0
	mov	r0, #0
	blx	r3
	cmp	r6, #0
	mov	r0, #10
	bne	.L1581
	cmp	r4, #49
	bgt	.L1579
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1579:
	ldr	r1, .L1582+4
	mvn	r6, #0
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1583:
	.align	2
.L1582:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC139
	UNWIND(.fnend)
	.size	VCTRL_StopChanWithCheck, .-VCTRL_StopChanWithCheck
	.align	2
	.global	VCTRL_ResetChan
	.type	VCTRL_ResetChan, %function
VCTRL_ResetChan:
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
	ldr	r5, .L1611
	mov	r4, r0
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L1607
	ldr	r8, [r3, #16]
	bl	VCTRL_StopChan
	ldr	r7, .L1611+4
	mov	r6, #0
	ldr	r3, [r5, r4, asl #2]
	mov	r0, #4
	ldr	r2, [r7, #12]
	str	r6, [r3, #384]
	str	r6, [r3, #388]
	blx	r2
	ldr	r3, [r5, r4, asl #2]
	ldr	r2, [r3, #1440]
	cmp	r2, #1
	beq	.L1608
.L1587:
	ldr	r0, .L1611+8
	mov	r2, #300
	ldr	r3, [r7, #48]
	mov	r1, #0
	mla	r0, r2, r4, r0
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	cmp	r3, #0
	addne	r3, r3, #290816
	movne	r2, #0
	strne	r2, [r3, #1868]
	beq	.L1609
.L1589:
	ldr	r3, [r5, r4, asl #2]
	mov	r6, #0
	mov	r0, r4
	str	r6, [r3, #1216]
	bl	InitSyntaxCtx
	ldr	r0, [r5, r4, asl #2]
	mov	r1, r6
	ldr	r3, [r7, #48]
	mov	r2, #416
	add	r0, r0, #744
	blx	r3
	ldr	r1, [r5, r4, asl #2]
	mvn	r2, #0
	mvn	r3, #0
	add	r0, r1, #294912
	add	ip, r1, #672
	strd	r2, [ip, #-8]
	strd	r2, [ip]
	str	r6, [r1, #1160]
	str	r6, [r0, #1944]
	str	r6, [r0, #1936]
	str	r6, [r0, #1940]
	ldr	r3, [r1, #1440]
	cmp	r3, #1
	streq	r6, [r1, #1480]
	streq	r6, [r0, #1916]
	mov	r0, r4
	bl	FSP_SwitchNotify
	ldr	r3, [r5, r4, asl #2]
	mov	r0, r4
	ldr	r3, [r3, #600]
	cmp	r3, #20
	beq	.L1591
	bl	FSP_ResetInstance
.L1592:
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	mov	r0, r4
	bl	InitUsdPool
	mov	r0, r4
	bl	SM_Reset
	ldr	r3, .L1611+12
	ldr	r2, [r3, #-3840]
	cmp	r2, #0
	beq	.L1593
	add	r5, r3, r4, lsl #2
	str	r4, [fp, #-40]
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1595
	mov	r0, #512
	sub	r1, fp, #40
	bl	VCTRL_SetDbgOption
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1595
	sub	r1, fp, #40
	mov	r0, #512
	bl	VCTRL_SetDbgOption
.L1595:
	ldr	r1, .L1611+16
	mov	r0, #32
	bl	dprint_vfmw
.L1593:
	cmp	r8, #0
	bne	.L1610
.L1597:
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	mov	r0, #0
.L1586:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1610:
	mov	r0, r4
	bl	VCTRL_StartChan
	b	.L1597
.L1591:
	bl	FSP_EmptyInstance
	b	.L1592
.L1608:
	add	r2, r3, #294912
	ldr	r1, [r3, #600]
	ldr	r2, [r2, #1960]
	cmp	r2, r1
	bne	.L1587
	str	r6, [r3, #20]
	mov	r0, r4
	bl	VCTRL_FindOccupiedFrame
	b	.L1587
.L1609:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1589
.L1607:
	mov	r0, r3
	movw	r2, #4447
	ldr	r3, .L1611+20
	ldr	r1, .L1611+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1586
.L1612:
	.align	2
.L1611:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR2-524
	.word	.LANCHOR4
	.word	.LC140
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ResetChan, .-VCTRL_ResetChan
	.align	2
	.global	VCTRL_CreateChanWithOption
	.type	VCTRL_CreateChanWithOption, %function
VCTRL_CreateChanWithOption:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 200
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #212)
	sub	sp, sp, #212
	ldr	r7, .L1691
	mov	r8, r1
	mov	r4, r0
	mov	r1, #0
	mov	r5, r2
	ldr	r3, [r7, #48]
	mov	r2, #24
	sub	r0, fp, #204
	ldr	r10, .L1691+4
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #24
	mov	r1, #0
	sub	r0, fp, #180
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #24
	mov	r1, #0
	sub	r0, fp, #156
	blx	r3
	ldr	r3, [r7, #48]
	mov	r2, #24
	mov	r1, #0
	sub	r0, fp, #132
	blx	r3
	mov	r1, #0
	ldr	r3, [r7, #48]
	sub	r0, fp, #80
	mov	r2, #36
	blx	r3
	ldr	r1, [r8, #36]
	str	r8, [r10, #-3708]
	cmp	r1, #0
	blt	.L1614
	ldr	r1, [r8, #12]
	cmp	r1, #31
	ble	.L1614
	ldr	r3, [r8, #16]
	cmp	r3, #31
	ble	.L1614
	ldr	r3, [r8, #56]
	cmp	r3, #262144
	blt	.L1614
	ldr	r1, [r8, #44]
	cmp	r1, #1
	beq	.L1674
	cmp	r1, #2
	beq	.L1675
.L1616:
	ldr	r3, [r8, #176]
	movw	ip, #10608
	movt	ip, 12
	movw	r9, #62864
	cmp	r3, #1
	sub	r2, fp, #220
	mov	r1, r8
	mov	r0, r4
	movhi	r3, #0
	strhi	r3, [r8, #176]
	cmp	r4, #26
	mov	r3, r5
	mov	r5, #1
	str	r5, [r8, #200]
	str	r5, [r8, #204]
	movt	r9, 177
	movne	r9, ip
	bl	VCTRL_GetChanMemSizeWithOption
	subs	r6, r0, #0
	bne	.L1676
	ldrd	r2, [r8, #136]
	orrs	r3, r2, r3
	beq	.L1622
	ldr	r3, [r8, #144]
	cmp	r3, #0
	bne	.L1623
.L1622:
	mov	r0, #5
	mov	r3, #3
	mov	r5, #1
	str	r3, [fp, #-84]
	bl	VFMW_GetString
	ldr	r2, [fp, #-212]
	sub	r3, fp, #104
	str	r5, [sp]
	mov	r1, #0
	bl	MEM_AllocMemBlock
	subs	r6, r0, #0
	bne	.L1677
	ldr	r2, [fp, #-88]
	ldr	r1, [fp, #-100]
	ldr	r0, [fp, #-96]
	str	r5, [fp, #-224]
	bl	MEM_AddMemRecord
	ldr	r1, [fp, #-96]
	ldr	r2, [fp, #-100]
	ldr	r3, [fp, #-88]
	str	r6, [fp, #-144]
	str	r1, [fp, #-156]
	str	r2, [fp, #-148]
	str	r3, [fp, #-140]
.L1625:
	ldrd	r2, [r8, #88]
	orrs	r3, r2, r3
	bne	.L1678
.L1627:
	ldr	r3, [r8, #176]
	cmp	r3, #1
	beq	.L1629
	mov	r0, #6
	mov	r3, #3
	str	r3, [fp, #-84]
	bl	VFMW_GetString
	mov	r3, #0
	ldr	r2, [fp, #-220]
	mov	r1, r3
	str	r3, [sp]
	sub	r3, fp, #104
	bl	MEM_AllocMemBlock
	subs	r5, r0, #0
	bne	.L1679
	ldr	r2, [fp, #-88]
	mov	r3, #1
	ldr	r1, [fp, #-100]
	ldr	r0, [fp, #-96]
	str	r3, [fp, #-228]
	bl	MEM_AddMemRecord
	ldr	r1, [fp, #-96]
	ldr	r2, [fp, #-100]
	ldr	r3, [fp, #-88]
	str	r5, [fp, #-192]
	str	r1, [fp, #-204]
	str	r2, [fp, #-196]
	str	r3, [fp, #-188]
.L1632:
	ldrd	r2, [r8, #112]
	orrs	r3, r2, r3
	beq	.L1636
	ldr	r3, [r8, #120]
	cmp	r3, #0
	bne	.L1637
.L1636:
	mov	r0, #7
	mov	r3, #3
	str	r3, [fp, #-84]
	bl	VFMW_GetString
	mov	r3, #0
	ldr	r2, [fp, #-216]
	mov	r1, r3
	str	r3, [sp]
	sub	r3, fp, #104
	bl	MEM_AllocMemBlock
	subs	r5, r0, #0
	bne	.L1680
	ldr	r2, [fp, #-88]
	mov	r3, #1
	ldr	r1, [fp, #-100]
	ldr	r0, [fp, #-96]
	str	r3, [fp, #-232]
	bl	MEM_AddMemRecord
	ldr	r1, [fp, #-96]
	ldr	r2, [fp, #-100]
	ldr	r3, [fp, #-88]
	str	r5, [fp, #-168]
	str	r1, [fp, #-180]
	str	r2, [fp, #-172]
	str	r3, [fp, #-164]
.L1641:
	ldr	r6, .L1691+8
	mov	r0, #0
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L1655
	mov	r3, r6
	mov	r5, #1
.L1648:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L1681
	add	r5, r5, #1
	cmp	r5, #16
	bne	.L1648
	mov	r2, r5
	ldr	r1, .L1691+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r7, #16]
	mov	r0, #0
	blx	r3
	mvn	r0, #0
.L1670:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1678:
	ldr	r3, [r8, #96]
	cmp	r3, #0
	beq	.L1627
	ldr	r3, [r7, #52]
	mov	r2, #24
	sub	r0, fp, #204
	add	r1, r8, #80
	blx	r3
	ldr	r3, [r8, #176]
	ldr	r2, [fp, #-188]
	cmp	r3, #1
	beq	.L1634
	ldr	r3, [fp, #-220]
	cmp	r2, r3
	blt	.L1682
.L1634:
	ldr	r1, [fp, #-196]
	mov	r3, #0
	ldr	r0, [fp, #-204]
	str	r3, [fp, #-228]
	bl	MEM_AddMemRecord
	b	.L1632
.L1629:
	mov	r3, #0
	mov	r0, #0
	mov	r1, #0
	str	r3, [fp, #-204]
	str	r3, [fp, #-228]
	str	r3, [fp, #-188]
	strd	r0, [fp, #-196]
	b	.L1632
.L1675:
	ldr	r3, [r8, #20]
	cmp	r3, #0
	ble	.L1614
	ldr	r3, [r8, #24]
	cmp	r3, #0
	ble	.L1614
.L1672:
	ldr	r3, [r8, #28]
	cmp	r3, #0
	ble	.L1614
	ldr	r3, [r8, #32]
	cmp	r3, #0
	bgt	.L1616
.L1614:
	ldr	r1, .L1691+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1681:
	uxtb	r3, r5
	str	r3, [fp, #-236]
.L1646:
	ldr	r0, [fp, #-148]
	sub	r2, r9, #205824
	mov	r1, #0
	sub	r2, r2, #520
	ldr	ip, [r7, #48]
	str	r0, [r6, r5, asl #2]
	str	r1, [fp, #-240]
	blx	ip
	ldr	r0, [r6, r5, asl #2]
	mov	r2, #208
	ldr	ip, [r7, #52]
	mov	r1, r8
	add	r0, r0, #1264
	blx	ip
	ldr	r1, [r6, r5, asl #2]
	ldr	r3, [fp, #-240]
	mov	lr, #21
	ldr	r0, .L1691+20
	mov	ip, #30
	str	r4, [r1, #44]
	ldr	r2, [r6, r5, asl #2]
	str	r9, [r1, #1188]
	movw	r1, #1944
	movt	r1, 4
	str	r5, [r2, #8]
	str	r3, [r2, #1208]
	str	r4, [r2, #636]
	ldr	r2, [r6, r5, asl #2]
	ldr	r4, [r8]
	str	r4, [r2, #40]
	ldr	r2, [r6, r5, asl #2]
	add	r4, r2, #294912
	str	lr, [r2, #600]
	str	lr, [r4, #1960]
	mov	r4, r3
	str	r3, [r2, #36]
	ldr	r2, [r6, r5, asl #2]
	ldr	lr, [r8, #200]
	add	r1, r2, r1
	str	r1, [r0, r5, asl #2]
	add	r1, r2, #1520
	mov	r0, r5
	str	lr, [r2, #1584]
	ldr	lr, [r8, #204]
	str	ip, [r2, #1512]
	ldr	ip, .L1691+24
	str	lr, [r2, #1588]
	str	r1, [ip, r5, asl #3]
	bl	VCTRL_ResetChan
	ldr	r1, [r6, r5, asl #2]
	ldr	r3, [fp, #-228]
	add	r2, r1, #290816
	add	r0, r1, #294912
	str	r3, [r1, #292]
	ldr	r3, [fp, #-232]
	str	r3, [r1, #296]
	ldr	r3, [fp, #-224]
	str	r3, [r1, #300]
	ldr	r3, [fp, #-240]
	str	r3, [r2, #1844]
	str	r3, [r2, #1848]
	str	r3, [r2, #1856]
	str	r3, [r2, #1860]
	str	r3, [r2, #1852]
	str	r3, [r2, #1864]
	ldr	r2, [r8, #176]
	str	r3, [r0, #1916]
	str	r2, [r0, #1924]
.L1650:
	mov	r8, r4, asl #6
	ldr	r0, [r6, r5, asl #2]
	add	r8, r8, r4, lsl #3
	ldr	ip, [r7, #48]
	add	r3, r8, #290816
	mov	r2, #72
	add	r3, r3, #1872
	mov	r1, #0
	add	r0, r0, r3
	add	r4, r4, #1
	blx	ip
	ldr	r0, [r6, r5, asl #2]
	movw	r2, #30552
	cmp	r4, #32
	add	r8, r0, r8
	movt	r2, 4
	add	ip, r8, #290816
	add	r2, r8, r2
	mov	r1, #255
	ldr	r3, .L1691
	str	r1, [ip, #1872]
	str	r1, [ip, #1876]
	ldr	r8, .L1691+8
	str	r1, [r2]
	bne	.L1650
	add	r0, r0, #294912
	ldr	ip, [r3, #48]
	movw	r2, #1288
	mov	r1, #0
	add	r0, r0, #80
	str	r3, [fp, #-224]
	blx	ip
	ldr	r3, [fp, #-224]
	ldr	r1, [r8, r5, asl #2]
	movw	r0, #34136
	mov	r2, #24
	movt	r0, 4
	ldr	r3, [r3, #48]
	add	r0, r1, r0
	mov	r1, #0
	add	r10, r10, r5, lsl #2
	blx	r3
	ldr	r3, [r8, r5, asl #2]
	ldr	lr, [fp, #-196]
	mov	r2, #0
	ldr	ip, [fp, #-172]
	add	r1, r3, #290816
	ldr	r0, [fp, #-156]
	ldr	r4, [fp, #-148]
	str	lr, [r3, #224]
	str	ip, [r3, #248]
	ldr	lr, [fp, #-204]
	ldr	ip, [fp, #-188]
	str	r0, [r3, #276]
	str	r4, [r3, #272]
	ldr	r8, [fp, #-140]
	ldr	r0, [fp, #-180]
	ldr	r4, [fp, #-164]
	str	r8, [r3, #284]
	str	lr, [r3, #228]
	str	lr, [r3, #328]
	str	ip, [r3, #236]
	str	ip, [r3, #332]
	str	r0, [r3, #252]
	str	r4, [r3, #260]
	str	r2, [r1, #1824]
	ldr	r1, [r3, #1440]
	str	r2, [r10, #-3704]
	cmp	r1, #1
	beq	.L1651
	ldr	r1, [r3, #320]
	mov	r8, #14848
	ldr	r10, [r3, #4]
	rsb	r2, r1, ip
	str	r0, [r3, #324]
	rsb	r2, r10, r2
	add	r1, r1, r10
	sub	r2, r2, #307200
	add	lr, r1, lr
	sub	r2, r2, #2576
	str	r2, [r3, #312]
	add	r0, r2, lr
	str	r4, [r3, #336]
	add	r1, r0, r8
	add	r2, r0, #47616
	add	r1, r1, #15
	add	r2, r2, #15
	add	r0, r0, #15
	bic	r1, r1, #15
	bic	r2, r2, #15
	str	r1, [r3, #348]
	bic	r0, r0, #15
	str	r2, [r3, #356]
	mov	r1, #32768
	mov	r2, #262144
	str	lr, [r3, #308]
	str	r0, [r3, #340]
	str	r8, [r3, #344]
	str	r1, [r3, #352]
	str	r2, [r3, #360]
.L1652:
	ldr	r3, [r6, r5, asl #2]
	mov	r0, r5
	ldr	r4, .L1691+8
	str	r5, [r3, #8]
	ldr	r3, [r6, r5, asl #2]
	add	r2, r3, #262144
	str	r5, [r3, #208]
	str	r5, [r3, #212]
	ldrb	r3, [fp, #-236]	@ zero_extendqisi2
	str	r5, [r2, #1724]
	strb	r3, [r2, #1944]
	bl	FSP_EnableInstance
	subs	r6, r0, #0
	bne	.L1683
	mov	r0, r5
	sub	r9, r9, #203776
	bl	SM_ClearInst
	ldr	r3, [r4, r5, asl #2]
	sub	r9, r9, #520
	mov	r0, r5
	ldr	r1, [r3, #272]
	add	r1, r1, r9
	bl	OpenUsdPool_WithMem
	ldr	r3, [r4, r5, asl #2]
	mov	r2, #1
	mov	r0, r5
	str	r2, [r3, #12]
	bl	VDMDRV_ClearChanIsDec
	bl	VDMDRV_ParamInit
	ldr	r3, [r7]
	ldr	r4, [r4, r5, asl #2]
	blx	r3
	ldr	r3, [r7, #16]
	add	r4, r4, #294912
	str	r0, [r4, #1932]
	mov	r0, r6
	blx	r3
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1674:
	ldr	r3, [r8, #20]
	cmp	r3, #0
	bgt	.L1672
	b	.L1614
.L1623:
	ldr	r3, [r7, #52]
	mov	r2, #24
	sub	r0, fp, #156
	add	r1, r8, #128
	blx	r3
	ldr	r2, [fp, #-140]
	ldr	r3, [fp, #-212]
	cmp	r2, r3
	blt	.L1684
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	str	r6, [fp, #-224]
	bl	MEM_AddMemRecord
	b	.L1625
.L1651:
	sub	r4, r4, #307200
	str	r2, [r3, #308]
	sub	r4, r4, #2576
	str	r2, [r3, #312]
	add	r2, r4, r0
	str	r0, [r3, #324]
	add	ip, r2, #14848
	add	r1, r2, #47616
	add	ip, ip, #15
	add	r1, r1, #15
	add	r2, r2, #15
	bic	r1, r1, #15
	bic	r2, r2, #15
	bic	ip, ip, #15
	str	r1, [r3, #356]
	mov	r0, #14848
	str	r2, [r3, #340]
	mov	r1, #32768
	mov	r2, #262144
	str	r4, [r3, #336]
	str	ip, [r3, #348]
	str	r0, [r3, #344]
	str	r1, [r3, #352]
	str	r2, [r3, #360]
	b	.L1652
.L1637:
	ldr	r3, [r7, #52]
	add	r1, r8, #104
	sub	r0, fp, #180
	mov	r2, #24
	blx	r3
	ldrd	r0, [fp, #-172]
	orrs	r3, r0, r1
	beq	.L1642
	ldr	r2, [fp, #-164]
	ldr	r3, [fp, #-216]
	cmp	r2, r3
	blt	.L1642
	mov	r1, r0
	ldr	r0, [fp, #-180]
	mov	r3, #0
	str	r3, [fp, #-232]
	bl	MEM_AddMemRecord
	b	.L1641
.L1680:
	ldr	r3, [fp, #-224]
	cmp	r3, #1
	beq	.L1685
.L1639:
	ldr	r3, [fp, #-228]
	cmp	r3, #1
	beq	.L1686
.L1640:
	movw	r2, #2363
	ldr	r1, .L1691+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1670
.L1686:
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-188]
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_DelMemRecord
	b	.L1640
.L1679:
	ldr	r3, [fp, #-224]
	cmp	r3, #1
	beq	.L1687
.L1631:
	movw	r2, #2305
	ldr	r1, .L1691+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1670
.L1655:
	str	r3, [fp, #-236]
	mov	r5, r3
	b	.L1646
.L1677:
	mov	r0, r5
	movw	r2, #2267
	ldr	r1, .L1691+36
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1670
.L1683:
	ldr	r1, .L1691+40
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	bl	FSP_DisableInstance
	mov	r0, r5
	bl	CloseUsdPool_WithMem
	ldr	r3, [r4, r5, asl #2]
	mov	r6, #0
	mov	r0, r5
	add	r1, r3, #294912
	add	r2, r3, #290816
	ldr	lr, [r3, #296]
	add	ip, r3, #220
	ldr	r9, [r3, #300]
	add	r2, r2, #1872
	ldr	r8, [r3, #292]
	ldr	r10, [r1, #1924]
	add	r1, r3, #244
	str	r2, [fp, #-68]
	add	r3, r3, #268
	str	lr, [fp, #-56]
	str	ip, [fp, #-80]
	str	r1, [fp, #-76]
	str	r3, [fp, #-72]
	str	r10, [fp, #-64]
	str	r9, [fp, #-52]
	str	r8, [fp, #-60]
	str	r6, [r4, r5, asl #2]
	bl	VDMDRV_ClearChanIsDec
	ldr	r3, [r7, #16]
	mov	r0, r6
	blx	r3
	sub	r1, fp, #80
	mov	r0, r5
	bl	VCTRL_FreeChanResource
	mvn	r0, #0
	b	.L1670
.L1642:
	ldr	r3, [fp, #-224]
	cmp	r3, #1
	beq	.L1688
.L1644:
	ldr	r3, [fp, #-228]
	cmp	r3, #1
	beq	.L1689
.L1645:
	movw	r2, #2392
	ldr	r1, .L1691+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1670
.L1676:
	movw	r2, #2257
	ldr	r1, .L1691+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1670
.L1685:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	b	.L1639
.L1687:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	b	.L1631
.L1684:
	mov	r0, r5
	movw	r2, #2283
	ldr	r1, .L1691+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1670
.L1682:
	ldr	r1, [fp, #-224]
	cmp	r1, #1
	beq	.L1690
.L1635:
	str	r3, [sp]
	mov	r0, #1
	mov	r3, r2
	ldr	r1, .L1691+56
	movw	r2, #2337
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1670
.L1689:
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-188]
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_DelMemRecord
	b	.L1645
.L1688:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	b	.L1644
.L1690:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	ldr	r2, [fp, #-188]
	ldr	r3, [fp, #-220]
	b	.L1635
.L1692:
	.align	2
.L1691:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR4
	.word	.LANCHOR0
	.word	.LC149
	.word	.LC69
	.word	s_pFspInst
	.word	s_SmIIS
	.word	.LC146
	.word	.LC144
	.word	.LC142
	.word	.LC148
	.word	.LC147
	.word	.LC141
	.word	.LC143
	.word	.LC145
	UNWIND(.fnend)
	.size	VCTRL_CreateChanWithOption, .-VCTRL_CreateChanWithOption
	.align	2
	.global	VCTRL_CreateChan
	.type	VCTRL_CreateChan, %function
VCTRL_CreateChan:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 280
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #296)
	sub	sp, sp, #296
	cmp	r0, #32
	mov	r4, r0
	mov	r6, r1
	bhi	.L1717
	ldr	r5, .L1720
	mov	r2, #208
	mov	r1, #0
	sub	r0, fp, #244
	mov	r7, #136
	ldr	r3, [r5, #48]
	blx	r3
	ldr	r3, [r5, #48]
	mov	r2, #24
	mov	r1, #0
	sub	r0, fp, #292
	blx	r3
	ldr	r3, [r5, #48]
	mov	r2, #24
	mov	r1, #0
	sub	r0, fp, #268
	blx	r3
	ldr	r2, .L1720+4
	mov	r1, r4, asl #2
	mov	r0, r4, asl #5
	rsb	r3, r1, r0
	sub	ip, r4, #18
	add	r8, r2, r3
	mov	lr, #2
	ldr	r3, [r2, r3]
	ldr	r8, [r8, #4]
	str	lr, [fp, #-236]
	mov	lr, #32
	str	r3, [fp, #-232]
	mov	r3, #256
	str	r8, [fp, #-228]
	str	r7, [fp, #-224]
	str	lr, [fp, #-216]
	str	r3, [fp, #-212]
	cmp	ip, #7
	ldrls	pc, [pc, ip, asl #2]
	b	.L1696
.L1698:
	.word	.L1708
	.word	.L1708
	.word	.L1708
	.word	.L1708
	.word	.L1708
	.word	.L1699
	.word	.L1699
	.word	.L1699
.L1708:
	mov	lr, #5
	mov	ip, #3
.L1697:
	rsb	r3, r1, r0
	rsb	r0, r1, r0
	add	r3, r2, r3
	add	r2, r2, r0
	mov	r1, #0
	str	ip, [fp, #-184]
	ldr	r3, [r3, #12]
	ldr	r2, [r2, #24]
	cmp	r3, lr
	str	r1, [fp, #-180]
	rsbgt	r3, ip, r3
	str	r1, [fp, #-176]
	movle	r3, #1
	subgt	r3, r3, #2
	cmp	r4, #24
	str	r3, [fp, #-208]
	str	r2, [fp, #-188]
	mov	r3, #1
	streq	r1, [fp, #-208]
	str	r3, [fp, #-204]
	str	r3, [fp, #-200]
	str	r3, [fp, #-192]
	streq	r3, [fp, #-184]
	beq	.L1703
	cmp	r4, #26
	beq	.L1718
	cmp	r4, #18
	moveq	r2, #4
	moveq	r3, #2
	streq	r2, [fp, #-208]
	streq	r3, [fp, #-184]
.L1703:
	sub	r2, fp, #304
	sub	r1, fp, #316
	mov	r0, r4
	bl	VCTRL_GetChanMemSize
	cmp	r0, #0
	bne	.L1719
	cmp	r6, #0
	beq	.L1709
	ldr	r3, [r6, #16]
	cmp	r3, #0
	beq	.L1706
	ldr	r2, [r6]
	cmp	r2, #0
	beq	.L1707
	ldrd	r8, [r6, #8]
	orrs	r1, r8, r9
	beq	.L1707
	ldr	r1, [fp, #-316]
	ldr	r0, [fp, #-304]
	add	r1, r1, r0
	cmp	r3, r1
	blt	.L1707
	ldr	r0, [fp, #-300]
	sub	lr, fp, #260
	str	r2, [fp, #-268]
	mov	ip, #3
	add	r2, r0, r2
	str	r2, [fp, #-292]
	sub	r2, fp, #284
	strd	r8, [lr]
	adds	r8, r8, r0
	rsb	r3, r0, r3
	adc	r9, r9, r0, asr #31
	sub	r1, fp, #268
	strd	r8, [r2]
	mov	r2, #24
	str	r0, [fp, #-252]
	sub	r0, fp, #140
	str	r3, [fp, #-276]
	str	ip, [fp, #-236]
	ldr	r3, [r5, #52]
	blx	r3
	ldr	r3, [r5, #52]
	sub	r1, fp, #292
	mov	r2, #24
	sub	r0, fp, #164
	blx	r3
	mov	r3, #1
.L1706:
	sub	r1, fp, #244
	mov	r0, r4
	mov	r2, #1
	bl	VCTRL_CreateChanWithOption
	orr	r0, r0, r0, asr #31
.L1695:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1699:
	mov	lr, #3
	mov	ip, #1
	b	.L1697
.L1696:
	mov	lr, #8
	mov	ip, #6
	b	.L1697
.L1718:
	mov	r2, #16
	mov	r3, #6
	str	r2, [fp, #-208]
	str	r3, [fp, #-184]
	b	.L1703
.L1709:
	mov	r3, r6
	b	.L1706
.L1707:
	ldr	r1, .L1720+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #16]
	ldrd	r4, [r6, #8]
	mov	r0, #1
	ldr	r2, [r6]
	ldr	r1, .L1720+12
	str	r3, [sp, #8]
	strd	r4, [sp]
	bl	dprint_vfmw
	ldr	r2, [fp, #-316]
	ldr	r3, [fp, #-304]
	mov	r0, #1
	ldr	r1, .L1720+16
	add	r2, r2, r3
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1695
.L1717:
	mov	r2, r0
	ldr	r1, .L1720+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1695
.L1719:
	ldr	r1, .L1720+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1695
.L1721:
	.align	2
.L1720:
	.word	vfmw_Osal_Func_Ptr_S
	.word	CapItem
	.word	.LC152
	.word	.LC153
	.word	.LC154
	.word	.LC150
	.word	.LC151
	UNWIND(.fnend)
	.size	VCTRL_CreateChan, .-VCTRL_CreateChan
	.align	2
	.global	VCTRL_ConfigChan
	.type	VCTRL_ConfigChan, %function
VCTRL_ConfigChan:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 80
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #92)
	sub	sp, sp, #92
	cmp	r0, #15
	mov	r8, #0
	mov	r5, r0
	mov	r4, r1
	str	r8, [fp, #-116]
	bhi	.L1759
	ldr	r6, .L1766
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	beq	.L1760
	ldr	r3, [r3, #12]
	cmp	r3, #1
	bne	.L1761
	bl	VCTRL_CheckChanWH
	subs	r7, r0, #0
	bne	.L1762
	ldr	r8, .L1766+4
	ldr	r0, .L1766+8
	bl	PDT_GetVdecCapability
	ldr	r2, [r4]
	ldr	r3, [r8, #-3620]
	cmp	r3, #20
	bhi	.L1729
	cmp	r3, r2
	beq	.L1730
	ldr	r3, .L1766+12
	add	r0, r3, #124
.L1732:
	ldr	r1, [r3, #4]!
	cmp	r1, #20
	bhi	.L1729
	cmp	r1, r2
	beq	.L1730
	cmp	r3, r0
	bne	.L1732
.L1730:
	cmp	r2, #8
	beq	.L1763
.L1733:
	cmp	r2, #9
	beq	.L1735
.L1739:
	mov	r1, r4
	mov	r0, r5
	bl	VCTRL_CmpConfigParameter
	subs	r3, r0, #0
	beq	.L1764
	ldr	r10, .L1766+16
	mov	r2, #120
	ldr	r0, [r6, r5, asl #2]
	mov	r1, r4
	add	r9, r4, #4
	mov	r8, #0
	ldr	r3, [r10, #52]
	add	r0, r0, #52
	blx	r3
	ldr	ip, [r4, #16]
	ldr	r3, [r6, r5, asl #2]
	mov	r2, #68
	mov	r1, #0
	sub	r0, fp, #112
	str	ip, [r3, #28]
	ldr	r3, [r6, r5, asl #2]
	ldr	lr, [r4]
	ldr	ip, [r10, #48]
	str	lr, [r3, #600]
	ldr	lr, [r4, #104]
	str	lr, [r3, #1448]
	blx	ip
	ldr	r3, [r6, r5, asl #2]
	ldr	r3, [r3, #324]
	mov	r0, r3
	str	r3, [fp, #-100]
	bl	MEM_Phy2Vir
	ldr	r1, [r4, #92]
	ldr	r3, [r6, r5, asl #2]
	mov	r2, #49152
	cmp	r1, #1
	movt	r2, 63683
	str	r2, [fp, #-92]
	ldreq	r2, [r4, #100]
	ldr	lr, [r3, #336]
	ldr	r3, [r4]
	streq	r2, [fp, #-56]
	streq	r1, [fp, #-64]
	mov	r1, r9
	ldr	ip, [r4, #16]
	str	r3, [fp, #-104]
	str	lr, [fp, #-96]
	strb	ip, [fp, #-108]
	str	r8, [fp, #-72]
	str	r0, [fp, #-112]
	ldreq	r0, [r4, #96]
	streq	r0, [fp, #-60]
	cmp	r3, #0
	ldr	r3, [r4, #88]
	sub	r0, fp, #88
	ldreq	r2, [r4, #52]
	movne	r2, #0
	str	r3, [fp, #-68]
	str	r2, [fp, #-76]
	mov	r2, #12
	ldr	r3, [r10, #52]
	blx	r3
	sub	r1, fp, #112
	mov	r0, r5
	bl	SM_Config
	ldr	r3, [r10, #48]
	mov	r1, r8
	sub	r0, fp, #120
	mov	r2, #1
	blx	r3
	ldr	r3, [r4, #92]
	cmp	r3, #1
	beq	.L1765
.L1742:
	ldr	r3, [r6, r5, asl #2]
	mov	r8, #1
	mov	r1, r9
	mov	r2, #12
	add	r9, r3, #294912
	add	r0, r3, #1152
	ldr	ip, [r3, #284]
	add	r0, r0, #12
	ldr	lr, [r3, #276]
	str	ip, [r3, #1196]
	ldr	ip, [r3, #308]
	str	lr, [r3, #1192]
	ldr	lr, [r3, #312]
	str	ip, [r3, #628]
	ldr	ip, [r3, #340]
	str	lr, [r3, #632]
	ldr	lr, [r3, #348]
	str	ip, [r3, #640]
	ldr	ip, [r3, #344]
	str	lr, [r3, #648]
	ldr	lr, [r3, #360]
	str	ip, [r3, #644]
	ldr	ip, [r3, #352]
	str	lr, [r3, #660]
	str	ip, [r3, #652]
	ldr	ip, [r3, #356]
	str	ip, [r3, #656]
	ldr	lr, [r4, #20]
	str	lr, [r3, #612]
	ldr	lr, [r4, #20]
	str	lr, [r3, #608]
	ldr	lr, [r4, #28]
	str	lr, [r3, #616]
	ldr	lr, [r4, #32]
	str	lr, [r3, #620]
	ldr	lr, [r4, #36]
	str	lr, [r3, #624]
	ldr	lr, [r4, #60]
	str	lr, [r3, #1200]
	ldr	lr, [r4, #68]
	str	r8, [r3, #1236]
	str	r8, [r3, #1240]
	str	lr, [r3, #1204]
	ldr	lr, [r4, #80]
	ldr	ip, [fp, #-72]
	rsb	lr, r8, lr
	ldr	r10, [r10, #52]
	clz	lr, lr
	mov	lr, lr, lsr #5
	str	lr, [r3, #1220]
	ldr	lr, [r4, #84]
	str	lr, [r3, #1224]
	ldr	lr, [r4, #52]
	str	ip, [r3, #1232]
	mov	ip, #0
	str	ip, [r3, #1480]
	str	ip, [r3, #1500]
	str	lr, [r3, #1228]
	ldrb	r3, [r4, #117]	@ zero_extendqisi2
	strb	r3, [r9, #1920]
	blx	r10
	ldr	ip, [r6, r5, asl #2]
	add	r3, ip, #294912
	ldr	r6, [ip, #600]
	ldr	r2, [r3, #1960]
	cmp	r2, r6
	beq	.L1743
	mov	r0, r5
	bl	VCTRL_ResetChan
	ldr	r3, [r4]
	ldr	r2, .L1766
	cmp	r3, #20
	ldr	ip, [r2, r5, asl #2]
	beq	.L1744
	ldr	r6, [ip, #600]
	add	r3, ip, #294912
.L1743:
	ldr	r2, .L1766+20
	ldr	r0, .L1766+24
	ldr	r1, .L1766+28
	add	lr, r2, #64
	str	r6, [r3, #1960]
.L1748:
	ldr	r3, [r2, #4]!
	add	r0, r0, #4
	add	r1, r1, #4
	cmp	r3, #0
	streq	r3, [r0, #-4]
	ldrne	ip, [r3, #28]
	strne	ip, [r0, #-4]
	ldrne	r3, [r3, #28]
	cmp	r2, lr
	str	r3, [r1, #-4]
	bne	.L1748
	ldr	r0, .L1766+24
	bl	VDMDRV_SetPriority
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1764:
	ldr	r1, .L1766+32
	mov	r7, r3
	bl	dprint_vfmw
.L1724:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1729:
	ldr	r1, .L1766+36
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r7, #0
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1763:
	sub	r0, fp, #116
	bl	REAL8DEC_Support
	ldr	r3, [fp, #-116]
	cmp	r3, #0
	ldrne	r2, [r4]
	bne	.L1733
.L1757:
	mvn	r7, #0
	b	.L1724
.L1735:
	sub	r0, fp, #116
	bl	REAL9DEC_Support
	ldr	r3, [fp, #-116]
	cmp	r3, #0
	bne	.L1739
	b	.L1757
.L1761:
	mov	r0, r8
	ldr	r3, .L1766+40
	movw	r2, #3250
	ldr	r1, .L1766+44
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L1724
.L1765:
	sub	r1, fp, #120
	mov	r0, r5
	strb	r3, [fp, #-120]
	bl	FSP_ConfigAttribute
	b	.L1742
.L1762:
	mov	r0, r8
	ldr	r1, .L1766+48
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L1724
.L1744:
	ldr	r3, [ip, #328]
	mov	r0, r5
	ldr	r2, [ip, #1280]
	ldr	r1, [ip, #1276]
	ldr	ip, [ip, #332]
	str	ip, [sp]
	bl	VCTRL_PartitionMemForStdUser
	cmp	r0, #0
	bne	.L1745
	ldr	r3, .L1766
	ldr	ip, [r3, r5, asl #2]
	add	r3, ip, #294912
	ldr	r6, [ip, #600]
	b	.L1743
.L1759:
	mov	r0, r8
	ldr	r3, .L1766+52
	mov	r2, #3248
	ldr	r1, .L1766+44
	bl	dprint_vfmw
	mvn	r7, #0
	b	.L1724
.L1760:
	mov	r0, r3
	movw	r2, #3249
	ldr	r3, .L1766+56
	mvn	r7, #0
	ldr	r1, .L1766+44
	bl	dprint_vfmw
	b	.L1724
.L1745:
	mov	r0, r8
	ldr	r1, .L1766+60
	bl	dprint_vfmw
	b	.L1757
.L1767:
	.align	2
.L1766:
	.word	.LANCHOR0
	.word	.LANCHOR4
	.word	.LANCHOR4-3640
	.word	.LANCHOR4-3620
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0-4
	.word	.LANCHOR4-3492
	.word	g_ChanCtx
	.word	.LC157
	.word	.LC156
	.word	.LC13
	.word	.LC1
	.word	.LC155
	.word	.LC12
	.word	.LC2
	.word	.LC158
	UNWIND(.fnend)
	.size	VCTRL_ConfigChan, .-VCTRL_ConfigChan
	.align	2
	.global	VCTRL_SetDbgOption
	.type	VCTRL_SetDbgOption, %function
VCTRL_SetDbgOption:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 120
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #132)
	sub	sp, sp, #132
	cmp	r1, #0
	beq	.L1861
	cmp	r0, #544
	ldr	r4, [r1]
	beq	.L1770
	bls	.L1884
	cmp	r0, #1280
	beq	.L1793
	bhi	.L1794
	movw	r3, #897
	cmp	r0, r3
	beq	.L1795
	bls	.L1885
	cmp	r0, #1024
	beq	.L1802
	bhi	.L1803
	movw	r3, #898
	cmp	r0, r3
	beq	.L1879
	movw	r3, #899
	cmp	r0, r3
	bne	.L1861
	mov	r0, r4
	bl	VCTRL_StopChan
	mov	r2, r4
	ldr	r1, .L1900
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, r4
	bl	VCTRL_ResetChan
	mov	r2, r4
	ldr	r1, .L1900+4
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
.L1879:
	mov	r0, r4
	bl	VCTRL_StartChan
	mov	r2, r4
	ldr	r1, .L1900+8
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
.L1882:
	mov	r0, #1
.L1769:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1794:
	movw	r3, #1537
	cmp	r0, r3
	beq	.L1808
	bls	.L1886
	movw	r3, #1540
	cmp	r0, r3
	beq	.L1814
	bhi	.L1815
	movw	r3, #1538
	cmp	r0, r3
	beq	.L1816
	movw	r3, #1539
	cmp	r0, r3
	bne	.L1861
	ldr	r2, .L1900+12
	mov	r3, #1
	mov	r0, r3
	ldr	r2, [r2, r4, asl #3]
	str	r3, [r2, #40]
	b	.L1769
.L1884:
	cmp	r0, #11
	beq	.L1772
	bls	.L1887
	cmp	r0, #512
	beq	.L1781
	bls	.L1888
	movw	r3, #515
	cmp	r0, r3
	beq	.L1787
	cmp	r0, #516
	bcs	.L1788
	movw	r3, #513
	cmp	r0, r3
	beq	.L1789
	movw	r3, #514
	cmp	r0, r3
	bne	.L1861
	cmp	r4, #15
	bhi	.L1882
	ldr	r5, .L1900+16
	add	r6, r5, r4, lsl #2
	ldr	r7, [r6, #-3280]
	cmp	r7, #0
	beq	.L1889
	ldr	r7, .L1900+20
	mov	r0, #32
	ldr	r1, .L1900+24
	mov	r4, #0
	bl	dprint_vfmw
	ldr	r0, [r6, #-3280]
	ldr	r3, [r7, #36]
	blx	r3
	ldr	r0, [r5, #-3140]
	ldr	r3, .L1900+28
	mvn	r2, #0
	cmp	r0, r4
	str	r4, [r6, #-3280]
	str	r4, [r5, #-3124]
	str	r2, [r3, #68]
	str	r4, [r5, #-3120]
	str	r4, [r5, #-3116]
	str	r4, [r5, #-3112]
	beq	.L1882
	ldr	r1, [r5, #-3144]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [r5, #-3132]
	ldr	r1, [r5, #-3144]
	ldr	r0, [r5, #-3140]
	bl	MEM_DelMemRecord
	ldr	r3, [r7, #48]
	mov	r1, r4
	mov	r2, #24
	ldr	r0, .L1900+32
	blx	r3
	b	.L1882
.L1888:
	cmp	r0, #13
	beq	.L1783
	bcc	.L1784
	cmp	r0, #256
	beq	.L1785
	movw	r3, #257
	cmp	r0, r3
	bne	.L1861
	ldr	r5, .L1900+36
	mov	r0, r4
	mov	r1, #1
	mov	r3, #67108864
	ldr	r4, [r5]
	str	r3, [r5]
	bl	PrintScdRawState
	mov	r0, #1
	str	r4, [r5]
	b	.L1769
.L1885:
	cmp	r0, #768
	beq	.L1797
	bls	.L1890
	movw	r3, #769
	cmp	r0, r3
	beq	.L1800
	cmp	r0, #896
	bne	.L1861
	mov	r0, r4
	bl	VCTRL_StopChan
	mov	r2, r4
	ldr	r1, .L1900
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1886:
	movw	r3, #1282
	cmp	r0, r3
	beq	.L1810
	bcc	.L1811
	movw	r3, #1283
	cmp	r0, r3
	beq	.L1812
	cmp	r0, #1536
	bne	.L1861
	cmp	r4, #15
	bhi	.L1858
	ldr	r3, .L1900+40
	mov	r5, #1
	mov	r2, r4
	ldr	r1, .L1900+44
	mov	r0, #32
	ldr	r3, [r3, r4, asl #2]
	str	r5, [r3, #1224]
	bl	dprint_vfmw
	mov	r0, r5
	b	.L1769
.L1887:
	cmp	r0, #4
	beq	.L1774
	bhi	.L1775
	cmp	r0, #1
	beq	.L1882
	bcc	.L1777
	cmp	r0, #2
	bne	.L1891
	mov	r5, r4, lsr #24
	bic	r4, r4, #-16777216
	ldr	r1, .L1900+48
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	mov	r2, #120
	mov	r1, #0
	sub	r0, fp, #164
	ldr	r3, [r3, #48]
	blx	r3
	sub	r1, fp, #164
	mov	r0, r5
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1882
	sub	r1, fp, #164
	mov	r0, r5
	str	r4, [fp, #-144]
	bl	VCTRL_ConfigChan
	b	.L1882
.L1775:
	cmp	r0, #6
	beq	.L1882
	bcc	.L1779
	cmp	r0, #7
	bne	.L1892
	mov	r5, r4, lsr #24
	bic	r4, r4, #-16777216
	ldr	r1, .L1900+52
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	mov	r2, #120
	mov	r1, #0
	sub	r0, fp, #164
	ldr	r3, [r3, #48]
	blx	r3
	sub	r1, fp, #164
	mov	r0, r5
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1882
	sub	r1, fp, #164
	mov	r0, r5
	str	r4, [fp, #-140]
	bl	VCTRL_ConfigChan
	b	.L1882
.L1803:
	movw	r3, #1025
	cmp	r0, r3
	beq	.L1806
	movw	r3, #1026
	cmp	r0, r3
	bne	.L1861
	ldr	r3, .L1900+56
	mov	r2, r4
	ldr	r1, .L1900+60
	mov	r0, #32
	str	r4, [r3, #-528]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1788:
	beq	.L1791
	cmp	r0, #528
	bne	.L1861
	cmp	r4, #15
	bhi	.L1882
	ldr	r6, .L1900+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-3108]
	cmp	r7, #0
	beq	.L1893
	ldr	r1, .L1900+64
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	ldr	r0, [r5, #-3108]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-3108]
	b	.L1769
.L1815:
	movw	r3, #1541
	cmp	r0, r3
	beq	.L1818
	cmp	r0, #1792
	bne	.L1861
	cmp	r4, #0
	bne	.L1882
	ldr	r5, .L1900+28
	ldr	r6, [r5, #72]
	cmp	r6, #1
	beq	.L1894
	ldr	r1, .L1900+68
	mov	r0, #32
	bl	dprint_vfmw
	mov	r3, #1
	mov	r0, r3
	str	r3, [r5, #72]
	b	.L1769
.L1816:
	ldr	r3, .L1900+72
	mov	r2, r4
	ldr	r1, .L1900+44
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1789:
	cmp	r4, #15
	bhi	.L1882
	ldr	r6, .L1900+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-3416]
	cmp	r7, #0
	beq	.L1895
	ldr	r1, .L1900+76
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	ldr	r0, [r5, #-3416]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-3416]
	b	.L1769
.L1812:
	ldr	r3, .L1900+80
	mov	r2, r4
	ldr	r1, .L1900+84
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1785:
	ldr	r1, .L1900+88
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, r4
	bl	DBG_PrintTracer
	mov	r0, #1
	b	.L1769
.L1800:
	bl	VCTRL_Resume
	mov	r0, #1
	b	.L1769
.L1806:
	ldr	r3, .L1900+16
	mov	r2, r4
	ldr	r1, .L1900+92
	mov	r0, #32
	str	r4, [r3, #-2700]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1777:
	ldr	r3, .L1900+36
	mov	r0, #1
	str	r4, [r3]
	b	.L1769
.L1818:
	ldr	r3, .L1900+16
	mov	r2, r4
	ldr	r1, .L1900+96
	mov	r0, #32
	str	r4, [r3, #-2696]
	bl	dprint_vfmw
	b	.L1882
.L1791:
	ldr	r5, .L1900+20
	mov	r0, r4
	ldmib	r1, {r8, r9}
	ldr	r3, [r5, #192]
	blx	r3
	subs	r7, r0, #0
	beq	.L1883
	mov	r0, #19
	ldr	r10, [r5, #68]
	bl	VFMW_GetString
	sub	r6, fp, #164
	str	r4, [sp]
	mov	r1, #50
	ldr	r3, .L1900+100
	mov	r2, r0
	mov	r0, r6
	blx	r10
	mov	r0, r6
	ldr	r3, [r5, #32]
	mov	r2, #0
	mov	r1, #66
	blx	r3
	subs	r4, r0, #0
	beq	.L1845
	ldr	r3, .L1900+104
	ldr	ip, [r3, #152]
	cmp	ip, #0
	beq	.L1846
	mov	lr, #1
	mov	r3, r9
	mov	r2, r8
	mov	r1, r7
	str	lr, [sp]
	blx	ip
.L1847:
	mov	r0, r4
	ldr	r3, [r5, #36]
	blx	r3
	mov	r2, r6
	ldr	r1, .L1900+108
	mov	r0, #32
	bl	dprint_vfmw
.L1844:
	mov	r0, r7
	ldr	r3, [r5, #152]
	blx	r3
	mov	r0, #1
	b	.L1769
.L1779:
	mov	r5, r4, lsr #24
	and	r4, r4, #7
	ldr	r1, .L1900+112
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	sub	r6, fp, #164
	mov	r2, #120
	mov	r1, #0
	ldr	r3, [r3, #48]
	mov	r0, r6
	blx	r3
	mov	r1, r6
	mov	r0, r5
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1822
	mov	r1, r6
	mov	r0, r5
	str	r4, [fp, #-136]
	bl	VCTRL_ConfigChan
	b	.L1882
.L1891:
	cmp	r0, #3
	beq	.L1882
.L1861:
	mvn	r0, #1
	b	.L1769
.L1890:
	cmp	r0, #560
	bne	.L1861
	cmp	r4, #15
	bhi	.L1882
	ldr	r6, .L1900+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-2836]
	cmp	r7, #0
	beq	.L1896
	ldr	r1, .L1900+116
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	ldr	r0, [r5, #-2836]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-2836]
	b	.L1769
.L1802:
	ldr	r3, .L1900+56
	mov	r2, r4
	ldr	r1, .L1900+120
	mov	r0, #32
	str	r4, [r3, #-532]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1808:
	cmp	r4, #15
	bhi	.L1858
	ldr	r3, .L1900+40
	mov	ip, #0
	mov	r2, r4
	ldr	r1, .L1900+44
	mov	r0, #32
	ldr	r3, [r3, r4, asl #2]
	str	ip, [r3, #1224]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1797:
	bl	VCTRL_Suspend
	mov	r0, #1
	b	.L1769
.L1787:
	ldr	r5, .L1900+20
	mov	r0, r4
	ldr	r3, [r5, #192]
	blx	r3
	subs	r7, r0, #0
	beq	.L1883
	mov	r0, #18
	ldr	r8, [r5, #68]
	bl	VFMW_GetString
	sub	r6, fp, #164
	str	r4, [sp]
	mov	r1, #50
	ldr	r3, .L1900+100
	mov	r2, r0
	mov	r0, r6
	blx	r8
	mov	r0, r6
	ldr	r3, [r5, #32]
	mov	r2, #0
	mov	r1, #66
	blx	r3
	subs	r4, r0, #0
	beq	.L1845
	mov	r2, r4
	ldr	r3, [r5, #44]
	mov	r1, #3145728
	mov	r0, r7
	blx	r3
	mov	r0, r4
	ldr	r3, [r5, #36]
	blx	r3
	mov	r2, r6
	ldr	r1, .L1900+124
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1844
.L1858:
	ldr	r1, .L1900+128
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1774:
	mov	r5, r4, lsr #24
	and	r4, r4, #3
	ldr	r1, .L1900+132
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	mov	r2, #120
	mov	r1, #0
	sub	r0, fp, #164
	ldr	r3, [r3, #48]
	blx	r3
	sub	r1, fp, #164
	mov	r0, r5
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1882
	sub	r1, fp, #164
	mov	r0, r5
	str	r4, [fp, #-132]
	bl	VCTRL_ConfigChan
	b	.L1882
.L1814:
	ldr	r3, .L1900+12
	mov	r2, #0
	mov	r0, #1
	ldr	r3, [r3, r4, asl #3]
	str	r2, [r3, #40]
	b	.L1769
.L1811:
	ldr	r3, .L1900+136
	mov	r2, r4
	ldr	r1, .L1900+140
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1810:
	ldr	r3, .L1900+144
	mov	r2, r4
	ldr	r1, .L1900+148
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1795:
	mov	r0, r4
	bl	VCTRL_ResetChan
	mov	r2, r4
	ldr	r1, .L1900+4
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1772:
	cmp	r4, #1
	ldrls	r3, .L1900+152
	movls	r2, r4
	ldrls	r1, .L1900+156
	movls	r0, #32
	movhi	r2, r4
	ldrhi	r1, .L1900+160
	strls	r4, [r3]
	movhi	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1781:
	cmp	r4, #15
	bhi	.L1882
	ldr	r6, .L1900+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-3836]
	cmp	r7, #0
	beq	.L1897
	ldr	r1, .L1900+164
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	ldr	r0, [r5, #-3836]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-3836]
	b	.L1769
.L1793:
	ldr	r3, .L1900+168
	mov	r2, r4
	ldr	r1, .L1900+172
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1784:
	cmp	r4, #1
	bls	.L1898
	mov	r2, r4
	ldr	r1, .L1900+176
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1783:
	cmp	r4, #16
	bls	.L1827
	mov	r2, r4
	ldr	r1, .L1900+180
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1770:
	cmp	r4, #15
	bhi	.L1882
	ldr	r6, .L1900+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-2972]
	cmp	r7, #0
	beq	.L1899
	ldr	r1, .L1900+64
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1900+20
	ldr	r0, [r5, #-2972]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-2972]
	b	.L1769
.L1822:
	mov	r2, r5
	ldr	r1, .L1900+184
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1827:
	ldr	r3, .L1900+16
	mov	r2, r4
	ldr	r1, .L1900+188
	mov	r0, #32
	str	r4, [r3, #-3428]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1769
.L1892:
	cmp	r0, #8
	beq	.L1882
	b	.L1861
.L1883:
	mov	r2, r4
	ldr	r1, .L1900+192
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1844
.L1898:
	ldr	r3, .L1900+16
	mov	r2, r4
	ldr	r1, .L1900+196
	mov	r0, #32
	str	r4, [r3, #-3840]
	bl	dprint_vfmw
	b	.L1882
.L1845:
	mov	r2, r6
	ldr	r1, .L1900+200
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1844
.L1894:
	ldr	r1, .L1900+204
	mov	r0, #32
	bl	dprint_vfmw
	str	r4, [r5, #72]
	mov	r0, r6
	b	.L1769
.L1896:
	ldr	r2, [r6, #-2708]
	mov	r1, #50
	ldr	r8, .L1900+20
	sub	r0, fp, #164
	add	ip, r2, #1
	ldr	r3, .L1900+100
	str	r2, [sp]
	ldr	r2, .L1900+208
	str	ip, [r6, #-2708]
	ldr	r9, [r8, #68]
	blx	r9
	ldr	r3, [r8, #32]
	mov	r2, r7
	sub	r0, fp, #164
	mov	r1, #66
	blx	r3
	str	r4, [r6, #-2704]
	cmp	r0, #0
	str	r0, [r5, #-2836]
	beq	.L1855
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1900+212
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1893:
	ldr	r8, .L1900+20
	mov	r0, #20
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-2980]
	mov	r1, #50
	ldr	r3, .L1900+100
	add	ip, r2, #1
	str	ip, [r6, #-2980]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #164
	blx	r9
	ldr	r3, [r8, #32]
	mov	r2, r7
	sub	r0, fp, #164
	mov	r1, #66
	blx	r3
	str	r4, [r6, #-2976]
	cmp	r0, #0
	str	r0, [r5, #-3108]
	beq	.L1849
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1900+216
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1897:
	ldr	r8, .L1900+20
	mov	r0, #15
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-3424]
	mov	r1, #50
	ldr	r3, .L1900+100
	add	ip, r2, #1
	str	ip, [r6, #-3424]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #164
	blx	r9
	ldr	r3, [r8, #32]
	mov	r2, r7
	sub	r0, fp, #164
	mov	r1, #66
	blx	r3
	str	r4, [r6, #-3420]
	cmp	r0, #0
	str	r0, [r5, #-3836]
	beq	.L1830
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1900+220
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1899:
	ldr	r8, .L1900+20
	mov	r0, #21
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-2844]
	mov	r1, #50
	ldr	r3, .L1900+100
	add	ip, r2, #1
	str	ip, [r6, #-2844]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #164
	blx	r9
	ldr	r3, [r8, #32]
	mov	r2, r7
	sub	r0, fp, #164
	mov	r1, #66
	blx	r3
	str	r4, [r6, #-2840]
	cmp	r0, #0
	str	r0, [r5, #-2972]
	beq	.L1852
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1900+224
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1895:
	ldr	r8, .L1900+20
	mov	r0, #16
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-3288]
	mov	r1, #50
	ldr	r3, .L1900+100
	add	ip, r2, #1
	str	ip, [r6, #-3288]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #164
	blx	r9
	ldr	r3, [r8, #32]
	mov	r2, r7
	sub	r0, fp, #164
	mov	r1, #66
	blx	r3
	str	r4, [r6, #-3284]
	cmp	r0, #0
	str	r0, [r5, #-3416]
	beq	.L1834
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1900+228
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1846:
	mov	r0, ip
	movw	r2, #9005
	ldr	r1, .L1900+232
	bl	dprint_vfmw
	b	.L1847
.L1889:
	ldr	r8, .L1900+20
	mov	r0, #17
	ldr	r9, .L1900+28
	ldr	r10, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r5, #-3152]
	add	r3, r9, #4
	mov	r1, #100
	add	ip, r2, #1
	str	ip, [r5, #-3152]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #164
	blx	r10
	ldr	r3, [r8, #32]
	sub	r0, fp, #164
	mov	r2, r7
	movw	r1, #578
	blx	r3
	cmp	r0, #0
	str	r0, [r6, #-3280]
	beq	.L1837
	ldr	r3, [r8, #48]
	mov	r2, #24
	mov	r1, r7
	ldr	r0, .L1900+32
	blx	r3
	mov	r0, #24
	bl	VFMW_GetString
	str	r7, [sp]
	ldr	r3, .L1900+32
	mov	r2, #14155776
	mov	r1, r7
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	beq	.L1838
	ldr	r1, .L1900+236
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r8, #36]
	ldr	r0, [r6, #-3280]
	blx	r3
	mvn	r3, #0
	str	r7, [r6, #-3280]
	str	r7, [r5, #-3124]
	str	r7, [r5, #-3120]
	str	r7, [r5, #-3116]
	str	r7, [r5, #-3112]
	str	r3, [r9, #68]
	b	.L1882
.L1838:
	ldr	r2, [r5, #-3132]
	ldr	r1, [r5, #-3144]
	ldr	r0, [r5, #-3140]
	bl	MEM_AddMemRecord
	ldr	ip, [r5, #-3144]
	mov	lr, r4, lsr #24
	sub	r2, fp, #164
	and	r4, r4, #3
	add	r6, ip, #9437184
	str	r4, [sp]
	mov	r3, lr
	str	ip, [r5, #-3116]
	mov	r0, #32
	add	ip, ip, #11796480
	ldr	r1, .L1900+240
	str	r6, [r5, #-3120]
	str	ip, [r5, #-3124]
	str	lr, [r9, #68]
	str	r4, [r5, #-3112]
	bl	dprint_vfmw
	b	.L1882
.L1855:
	sub	r2, fp, #164
	ldr	r1, .L1900+244
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1849:
	sub	r2, fp, #164
	ldr	r1, .L1900+248
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1852:
	sub	r2, fp, #164
	ldr	r1, .L1900+252
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1830:
	sub	r2, fp, #164
	ldr	r1, .L1900+256
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1834:
	sub	r2, fp, #164
	ldr	r1, .L1900+260
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1837:
	sub	r2, fp, #164
	ldr	r1, .L1900+264
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1882
.L1901:
	.align	2
.L1900:
	.word	.LC194
	.word	.LC195
	.word	.LC196
	.word	s_SmIIS
	.word	.LANCHOR4
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC180
	.word	.LANCHOR3
	.word	.LANCHOR4-3148
	.word	g_PrintEnable
	.word	.LANCHOR0
	.word	.LC205
	.word	.LC159
	.word	.LC163
	.word	.LANCHOR2
	.word	.LC199
	.word	.LC187
	.word	.LC208
	.word	g_TunnelLineNumber
	.word	.LC176
	.word	g_TraceImgPeriod
	.word	.LC203
	.word	.LC170
	.word	.LC198
	.word	.LC206
	.word	.LANCHOR3+4
	.word	g_vdm_hal_fun_ptr
	.word	.LC184
	.word	.LC161
	.word	.LC193
	.word	.LC197
	.word	.LC182
	.word	.LC204
	.word	.LC160
	.word	g_TraceBsPeriod
	.word	.LC201
	.word	g_TraceFramePeriod
	.word	.LC202
	.word	USE_FF_APT_EN
	.word	.LC164
	.word	.LC165
	.word	.LC173
	.word	g_TraceCtrl
	.word	.LC200
	.word	.LC167
	.word	.LC168
	.word	.LC162
	.word	.LC169
	.word	.LC181
	.word	.LC166
	.word	.LC183
	.word	.LC207
	.word	.LC190
	.word	.LC191
	.word	.LC185
	.word	.LC171
	.word	.LC188
	.word	.LC174
	.word	.LC6
	.word	.LC177
	.word	.LC178
	.word	.LC192
	.word	.LC186
	.word	.LC189
	.word	.LC172
	.word	.LC175
	.word	.LC179
	UNWIND(.fnend)
	.size	VCTRL_SetDbgOption, .-VCTRL_SetDbgOption
	.align	2
	.global	VCTRL_ResetChanWithOption
	.type	VCTRL_ResetChanWithOption, %function
VCTRL_ResetChanWithOption:
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
	ldr	r5, .L1935
	mov	r4, r0
	mov	r6, r1
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L1929
	ldr	r2, [r1]
	cmp	r2, #1
	bhi	.L1930
	ldr	r9, [r3, #16]
	bl	VCTRL_StopChan
	ldr	r7, .L1935+4
	mov	r8, #0
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #300
	ldr	r0, .L1935+8
	mov	r1, r8
	ldr	ip, [r7, #48]
	str	r8, [r3, #384]
	str	r8, [r3, #388]
	mla	r0, r2, r4, r0
	blx	ip
	ldr	r3, [r5, r4, asl #2]
	cmp	r3, r8
	addne	r3, r3, #290816
	strne	r8, [r3, #1868]
	beq	.L1931
.L1907:
	ldr	r3, [r7, #12]
	mov	r0, #4
	blx	r3
	mov	r0, r4
	bl	VCTRL_FindOccupiedFrame
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	ldr	r2, [r6, #4]
	ldr	r3, [r5, r4, asl #2]
	mov	r0, r4
	str	r2, [r3, #1216]
	bl	InitSyntaxCtx
	ldr	r3, [r5, r4, asl #2]
	add	r1, r3, #294912
	ldr	r0, [r3, #600]
	add	r1, r1, #1968
	bl	ReleaseCurPic
	ldr	r0, [r5, r4, asl #2]
	ldr	r3, [r7, #48]
	mov	r2, #416
	mov	r1, #0
	add	r0, r0, #744
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	mvn	r0, #0
	mvn	r1, #0
	add	r2, r3, #672
	mov	ip, #0
	strd	r0, [r2, #-8]
	strd	r0, [r2]
	mov	r0, r4
	ldr	r2, [r3, #1440]
	str	ip, [r3, #1160]
	cmp	r2, #1
	streq	ip, [r3, #1480]
	ldr	r3, [r3, #600]
	cmp	r3, #20
	beq	.L1909
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq	.L1932
.L1909:
	bl	FSP_EmptyInstance
.L1910:
	mov	r0, r4
	bl	InitUsdPool
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L1933
.L1911:
	ldr	r3, .L1935+12
	ldr	r2, [r3, #-3840]
	cmp	r2, #0
	beq	.L1912
	add	r5, r3, r4, lsl #2
	str	r4, [fp, #-40]
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1914
	mov	r0, #512
	sub	r1, fp, #40
	bl	VCTRL_SetDbgOption
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1914
	sub	r1, fp, #40
	mov	r0, #512
	bl	VCTRL_SetDbgOption
.L1914:
	ldr	r1, .L1935+16
	mov	r0, #32
	bl	dprint_vfmw
.L1912:
	cmp	r9, #0
	bne	.L1934
.L1916:
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	mov	r0, #0
.L1904:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1933:
	mov	r0, r4
	bl	SM_Reset
	b	.L1911
.L1934:
	mov	r0, r4
	bl	VCTRL_StartChan
	b	.L1916
.L1932:
	bl	FSP_ResetInstance
	b	.L1910
.L1930:
	movw	r2, #4051
	ldr	r1, .L1935+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1931:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1907
.L1929:
	mov	r0, r3
	movw	r2, #4047
	ldr	r3, .L1935+24
	ldr	r1, .L1935+28
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1904
.L1936:
	.align	2
.L1935:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR2-524
	.word	.LANCHOR4
	.word	.LC140
	.word	.LC209
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ResetChanWithOption, .-VCTRL_ResetChanWithOption
	.align	2
	.global	VCTRL_DestroyChanWithOption
	.type	VCTRL_DestroyChanWithOption, %function
VCTRL_DestroyChanWithOption:
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
	ldr	r5, .L1952
	mov	r4, r0
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	cmp	r4, #15
	ldrhi	r3, .L1952+4
	movwhi	r2, #2740
	bhi	.L1947
	ldr	r7, .L1952+8
	ldr	r6, [r7, r4, asl #2]
	cmp	r6, #0
	beq	.L1948
	ldr	r8, [r6, #12]
	cmp	r8, #1
	bne	.L1949
	mov	r0, #0
	str	r0, [r6, #16]
	ldr	r3, [r5, #16]
	blx	r3
	mov	r0, r4
	bl	SCDDRV_IsChanDec
	cmp	r0, #0
	bne	.L1950
.L1942:
	ldr	r3, [r5, #12]
	mov	r0, #0
	blx	r3
	ldr	r2, .L1952+12
	ldr	r3, [r5, #12]
	mov	r8, #0
	mov	r0, #4
	ldr	r6, .L1952
	str	r8, [r2, r4, asl #2]
	blx	r3
	mov	r0, r4
	bl	VCTRL_StopChan
	ldr	r3, [r6, #16]
	subs	r5, r0, #0
	mov	r0, #4
	bne	.L1951
	blx	r3
	mov	r0, r4
	bl	VCTRL_ResetChan
	bl	VDMDRV_ParamInit
	ldr	r3, .L1952+16
	ldr	r2, [r6, #12]
	mov	r0, #4
	add	r3, r3, r4, lsl #2
	str	r5, [r3, #-788]
	blx	r2
	mov	r0, r4
	bl	FSP_DisableInstance
	mov	r0, r4
	bl	CloseUsdPool_WithMem
	ldr	r3, [r7, r4, asl #2]
	ldr	ip, .L1952+20
	mov	r0, r4
	add	lr, r3, #294912
	add	r2, r3, #290816
	ldr	r1, [r3, #296]
	add	r2, r2, #1872
	ldr	r9, [r3, #300]
	ldr	r8, [r3, #292]
	ldr	r10, [lr, #1924]
	add	lr, r3, #220
	str	r5, [r7, r4, asl #2]
	add	r7, r3, #244
	add	r3, r3, #268
	str	r3, [fp, #-72]
	ldr	r3, .L1952+24
	str	r2, [fp, #-68]
	str	lr, [fp, #-80]
	str	r5, [ip, r4, asl #3]
	str	r1, [fp, #-56]
	str	r5, [r3, r4, asl #2]
	str	r10, [fp, #-64]
	str	r9, [fp, #-52]
	str	r8, [fp, #-60]
	str	r7, [fp, #-76]
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r4
	bl	SCDDRV_ResetSCD
	mov	r0, r4
	bl	VDMDRV_ResetVDH
	ldr	r3, [r6, #16]
	mov	r0, #4
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, r5
	blx	r3
	ldr	r3, [r6, #24]
	mov	r1, r4
	mov	r0, #1
	blx	r3
	sub	r1, fp, #80
	mov	r0, r4
	bl	VCTRL_FreeChanResource
	mov	r0, #1
	ldr	r3, [r6, #28]
	mov	r1, r4
	blx	r3
	mov	r0, r5
.L1939:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1950:
	mov	r0, r4
	ldr	r6, [r5, #128]
	bl	SCDDRV_IsChanDec
	mov	r2, #500
	clz	r1, r0
	mov	r0, r8
	mov	r1, r1, lsr #5
	blx	r6
	b	.L1942
.L1949:
	ldr	r3, .L1952+28
	movw	r2, #2764
.L1947:
	ldr	r1, .L1952+32
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r5, #16]
	mov	r0, #0
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1951:
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, r8
	blx	r3
	mvn	r0, #0
	b	.L1939
.L1948:
	ldr	r3, .L1952+36
	movw	r2, #2750
	ldr	r1, .L1952+32
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, [r5, #16]
	mov	r0, r6
	blx	r3
	mvn	r0, #0
	b	.L1939
.L1953:
	.align	2
.L1952:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC12
	.word	.LANCHOR0
	.word	g_LowDelaySeqIndex
	.word	.LANCHOR2
	.word	s_SmIIS
	.word	s_pFspInst
	.word	.LC212
	.word	.LC210
	.word	.LC211
	UNWIND(.fnend)
	.size	VCTRL_DestroyChanWithOption, .-VCTRL_DestroyChanWithOption
	.align	2
	.global	VCTRL_DestroyChan
	.type	VCTRL_DestroyChan, %function
VCTRL_DestroyChan:
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
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_DestroyChanWithOption
	UNWIND(.fnend)
	.size	VCTRL_DestroyChan, .-VCTRL_DestroyChan
	.align	2
	.global	VCTRL_CloseVfmw
	.type	VCTRL_CloseVfmw, %function
VCTRL_CloseVfmw:
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
	ldr	r6, .L1973
	ldr	r3, [r6, #64]
	cmp	r3, #1
	bne	.L1962
	sub	r5, r6, #4
	mov	r4, #0
.L1958:
	ldr	r3, [r5, #4]!
	mov	r0, r4
	add	r4, r4, #1
	cmp	r3, #0
	beq	.L1957
	bl	VCTRL_DestroyChanWithOption
.L1957:
	cmp	r4, #16
	bne	.L1958
	bl	FSP_Exit
	ldr	r3, [r6, #68]
	mov	r2, #3
	ldr	r5, .L1973+4
	cmp	r3, #0
	str	r2, [r6, #72]
	beq	.L1960
	mov	r0, #10
	ldr	r3, [r5, #116]
	mov	r4, #49
	blx	r3
	b	.L1961
.L1972:
	ldr	r3, [r5, #116]
	blx	r3
	subs	r4, r4, #1
	beq	.L1960
.L1961:
	ldr	r3, [r6, #68]
	mov	r0, #10
	cmp	r3, #0
	bne	.L1972
.L1960:
	ldr	r3, [r5, #48]
	mov	r2, #64
	mov	r1, #0
	ldr	r0, .L1973+8
	blx	r3
	ldr	r0, .L1973+12
	bl	CloseDrivers
	mov	r3, #0
	mov	r0, r3
	str	r3, [r6, #64]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1962:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1974:
	.align	2
.L1973:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR2-788
	.word	.LANCHOR0+88
	UNWIND(.fnend)
	.size	VCTRL_CloseVfmw, .-VCTRL_CloseVfmw
	.align	2
	.global	VCTRL_InitDspCode
	.type	VCTRL_InitDspCode, %function
VCTRL_InitDspCode:
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
	cmp	r1, #6
	mov	r5, r0
	moveq	r4, #1
	beq	.L1976
	cmp	r1, #0
	moveq	r4, #2
	movne	r4, #0
.L1976:
	ldr	r3, .L1990
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1986
	mov	r1, r4
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VDH_Require_DspCode
.L1986:
	ldr	r6, .L1990+4
	ldr	r3, [r6, #4]
	cmp	r3, #0
	beq	.L1987
	mov	r0, #0
	blx	r3
.L1979:
	ldr	r3, [r6, #20]
	cmp	r3, #0
	beq	.L1988
	mov	r0, #0
	blx	r3
.L1981:
	ldr	r3, [r6, #12]
	cmp	r3, #0
	beq	.L1989
	mov	r0, #0
	blx	r3
.L1983:
	mov	r1, r4
	mov	r0, #0
	bl	SCDDRV_LoadDspCode
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VCTRL_CloseHardware
.L1987:
	mov	r0, r3
	movw	r2, #3799
	ldr	r1, .L1990+8
	bl	dprint_vfmw
	b	.L1979
.L1988:
	mov	r0, r3
	movw	r2, #3800
	ldr	r1, .L1990+8
	bl	dprint_vfmw
	b	.L1981
.L1989:
	mov	r0, r3
	movw	r2, #3801
	ldr	r1, .L1990+8
	bl	dprint_vfmw
	b	.L1983
.L1991:
	.align	2
.L1990:
	.word	g_HalDisable
	.word	g_vdm_hal_fun_ptr
	.word	.LC6
	UNWIND(.fnend)
	.size	VCTRL_InitDspCode, .-VCTRL_InitDspCode
	.align	2
	.global	VCTRL_LoadDspCode
	.type	VCTRL_LoadDspCode, %function
VCTRL_LoadDspCode:
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
	ldr	r3, .L2016
	mov	r5, r0
	ldr	r3, [r3, r0, asl #2]
	ldr	r1, [r3, #600]
	cmp	r1, #0
	movne	r3, #1
	movne	r6, #0
	ldreq	r6, [r3, #1228]
	clzeq	r3, r6
	moveq	r3, r3, lsr #5
.L1993:
	cmp	r1, #6
	moveq	r3, #0
	andne	r3, r3, #1
	cmp	r3, #0
	beq	.L2014
.L2005:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2014:
	ldr	r7, .L2016+4
	ldr	r4, [r7, #-788]
	cmp	r4, #0
	bne	.L1994
	sub	r3, r7, #788
	sub	r2, r7, #728
.L1995:
	ldr	r4, [r3, #4]!
	cmp	r4, #0
	bne	.L1994
	cmp	r3, r2
	bne	.L1995
	mov	r0, r5
	add	r5, r7, r5, lsl #2
	bl	VCTRL_InitDspCode
	cmp	r6, #1
	mov	r0, r4
	streq	r6, [r5, #-788]
	movne	r3, #2
	strne	r3, [r5, #-788]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1994:
	cmp	r4, #1
	beq	.L2015
	cmp	r1, #6
	beq	.L2005
	ldr	r1, .L2016+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2015:
	cmp	r6, #1
	beq	.L2005
	ldr	r1, .L2016+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2017:
	.align	2
.L2016:
	.word	.LANCHOR0
	.word	.LANCHOR2
	.word	.LC214
	.word	.LC213
	UNWIND(.fnend)
	.size	VCTRL_LoadDspCode, .-VCTRL_LoadDspCode
	.align	2
	.global	VCTRL_SetChanFsPartitionState
	.type	VCTRL_SetChanFsPartitionState, %function
VCTRL_SetChanFsPartitionState:
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
	ldr	r3, .L2019
	ldr	r3, [r3, r0, asl #2]
	add	r3, r3, #290816
	str	r1, [r3, #1824]
	ldmfd	sp, {fp, sp, pc}
.L2020:
	.align	2
.L2019:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_SetChanFsPartitionState, .-VCTRL_SetChanFsPartitionState
	.align	2
	.global	VCTRL_GetCurChanFsSize
	.type	VCTRL_GetCurChanFsSize, %function
VCTRL_GetCurChanFsSize:
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
	ldr	r3, .L2022
	ldr	r3, [r3, r0, asl #2]
	add	r3, r3, #290816
	ldr	r0, [r3, #1844]
	ldmfd	sp, {fp, sp, pc}
.L2023:
	.align	2
.L2022:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetCurChanFsSize, .-VCTRL_GetCurChanFsSize
	.align	2
	.global	VCTRT_SetChanCtrlState
	.type	VCTRT_SetChanCtrlState, %function
VCTRT_SetChanCtrlState:
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
	cmp	r1, #1
	beq	.L2026
	bcc	.L2027
	cmp	r1, #2
	bne	.L2034
	ldr	r3, .L2035
	add	r0, r3, r0, lsl #2
	ldr	r3, [r0, #-3704]
	cmp	r3, #1
	beq	.L2031
	mov	r3, #0
	str	r1, [r0, #-3704]
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2034:
	mov	r3, #0
.L2025:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2027:
	ldr	r2, .L2035
	mov	r1, #0
	mov	r3, r1
	add	r0, r2, r0, lsl #2
	str	r1, [r0, #-3704]
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2026:
	ldr	r3, .L2035
	add	r0, r3, r0, lsl #2
	ldr	r3, [r0, #-3704]
	cmp	r3, #0
	bne	.L2031
	str	r1, [r0, #-3704]
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2031:
	mvn	r3, #0
	b	.L2025
.L2036:
	.align	2
.L2035:
	.word	.LANCHOR4
	UNWIND(.fnend)
	.size	VCTRT_SetChanCtrlState, .-VCTRT_SetChanCtrlState
	.align	2
	.global	VCTRL_SetFsParamToChan
	.type	VCTRL_SetFsParamToChan, %function
VCTRL_SetFsParamToChan:
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
	ldr	r3, .L2038
	ldr	ip, [r1, #4]
	ldr	r3, [r3, r0, asl #2]
	add	r2, r3, #290816
	str	ip, [r3, #312]
	ldr	r0, [r1]
	str	r0, [r3, #308]
	ldr	r0, [r1]
	str	r0, [r3, #228]
	ldr	r0, [r1, #16]
	str	r0, [r3, #224]
	ldr	r0, [r1, #4]
	str	r0, [r3, #236]
	ldr	r3, [r1, #8]
	str	r3, [r2, #1852]
	ldmfd	sp, {fp, sp, pc}
.L2039:
	.align	2
.L2038:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_SetFsParamToChan, .-VCTRL_SetFsParamToChan
	.align	2
	.global	VCTRL_SetSingleFrameBufToChan
	.type	VCTRL_SetSingleFrameBufToChan, %function
VCTRL_SetSingleFrameBufToChan:
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
	ldr	r8, .L2116
	mov	r10, r0
	ldr	r7, .L2116+4
	ldr	lr, .L2116+8
	mov	r5, r1
	ldr	r9, [r8, r10, asl #2]
	mov	r0, #8
	ldr	r2, [r7, #12]
	add	r3, r9, #290816
	add	r4, r9, #294912
	ldr	r6, [lr, r10, asl #2]
	ldr	r3, [r3, #1852]
	ldr	r1, [r4, #1352]
	cmp	r3, #32
	movge	r3, #32
	str	r3, [r4, #1160]
	str	r1, [fp, #-48]
	blx	r2
	ldr	lr, [r8, r10, asl #2]
	add	r3, lr, #290816
	ldr	r3, [r3, #1872]
	cmp	r3, #255
	beq	.L2070
	movw	r3, #30616
	mov	r2, #1
	movt	r3, 4
	add	r3, lr, r3
	b	.L2042
.L2112:
	add	r2, r2, #1
	cmp	r2, #32
	beq	.L2106
.L2042:
	ldr	r1, [r3]
	add	r3, r3, #72
	cmp	r1, #255
	bne	.L2112
.L2041:
	mov	r0, r2, asl #6
	ldr	r3, [r7, #52]
	add	r0, r0, r2, lsl #3
	mov	r1, r5
	add	r0, r0, #290816
	mov	r2, #72
	add	r0, r0, #1872
	add	r0, lr, r0
	blx	r3
	ldr	r3, [r8, r10, asl #2]
	add	r3, r3, #294912
	ldr	r2, [r3, #1392]
	add	r2, r2, #1
	str	r2, [r3, #1392]
	ldr	r3, [r5]
	cmp	r3, #255
	beq	.L2064
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #24]
	ldr	r0, [r5, #32]
	bl	MEM_AddMemRecord
.L2064:
	ldr	r3, [r5, #4]
	cmp	r3, #255
	beq	.L2044
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #40]
	ldr	r0, [r5, #48]
	bl	MEM_AddMemRecord
.L2044:
	ldr	r3, [r5, #8]
	cmp	r3, #255
	beq	.L2045
	ldr	r2, [r5, #20]
	ldr	r1, [r5, #56]
	ldr	r0, [r5, #64]
	bl	MEM_AddMemRecord
.L2045:
	ldr	r3, [r8, r10, asl #2]
	add	r3, r3, #294912
	ldr	r8, [r3, #1396]
	cmp	r8, #0
	beq	.L2062
	ldrsb	r2, [r6, #1]
	cmp	r2, #0
	bne	.L2113
.L2048:
	ldr	r3, [r4, #1160]
	strb	r3, [r6, #10]
.L2049:
	mov	r8, #0
	strb	r8, [r6, #11]
	ldr	r3, [r4, #1328]
	strb	r3, [r6, #13]
	ldr	r3, [r5]
	cmp	r3, #255
	beq	.L2114
	ldrb	r8, [r4, #96]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L2050
	add	r3, r4, #108
	mov	r8, #1
	b	.L2051
.L2115:
	add	r8, r8, #1
	cmp	r8, #32
	beq	.L2106
.L2051:
	ldrb	r2, [r3]	@ zero_extendqisi2
	add	r3, r3, #12
	cmp	r2, #0
	bne	.L2115
.L2050:
	ldr	r3, [r5, #32]
	mov	r0, r8, asl #2
	rsb	r1, r0, r8, lsl #4
	ldr	ip, [fp, #-48]
	add	r2, r3, #1020
	add	r1, r4, r1
	add	r2, r2, #3
	add	r0, r9, r0
	bic	r2, r2, #1020
	rsb	r10, r3, ip
	bic	r2, r2, #3
	str	r2, [r1, #88]
	ldr	lr, [r5, #12]
	mov	ip, #1
	add	r0, r0, #294912
	strb	ip, [r1, #96]
	add	r2, r10, r2
	str	lr, [r1, #92]
	add	r10, r2, r3
	ldr	lr, [r4, #1332]
	add	lr, lr, ip
	str	lr, [r4, #1332]
	str	r10, [r0, #984]
	ldr	lr, [r4, #1116]
	add	r3, r3, lr
	add	r3, r3, r2
	str	r3, [r0, #472]
	ldr	r3, [r5, #48]
	strb	ip, [r1, #608]
	str	r3, [r1, #600]
	ldr	r3, [r4, #1340]
	add	r3, r3, ip
	str	r3, [r4, #1340]
	ldr	r3, [r5, #8]
	cmp	r3, #255
	bne	.L2067
.L2053:
	add	r1, r6, #20480
	ldr	lr, [r1, #1020]
	cmp	lr, #0
	beq	.L2073
	add	r2, r6, #21504
	mov	r3, #1
	add	r2, r2, #96
	b	.L2058
.L2057:
	add	r3, r3, #1
	cmp	r3, #32
	beq	.L2106
.L2058:
	ldr	r0, [r2]
	add	r2, r2, #100
	cmp	r0, #0
	bne	.L2057
	mov	lr, r3
.L2056:
	mov	r0, #100
	add	r2, r6, #24576
	mla	r3, r0, r3, r6
	mov	r0, r8, asl #2
	rsb	r8, r0, r8, lsl #4
	ldr	ip, [r2, #3980]
	add	r8, r4, r8
	add	r0, r9, r0
	add	r10, r3, #21248
	add	ip, ip, #1
	add	r0, r0, #294912
	str	ip, [r2, #3980]
	str	r0, [fp, #-48]
	mov	ip, r10
	str	lr, [r10, #164]
	mov	lr, r10
	ldr	r0, [r8, #88]
	mov	r9, lr
	str	r0, [ip, #172]
	ldr	r0, [r8, #92]
	mov	r8, #0
	str	r0, [ip, #176]
	add	ip, r10, #216
	ldr	r0, [fp, #-48]
	str	ip, [fp, #-52]
	mov	ip, r10
	ldr	r0, [r0, #472]
	str	r0, [r10, #184]
	add	r0, lr, #224
	ldr	r10, [r4, #80]
	str	r0, [fp, #-56]
	str	r10, [lr, #188]
	add	r10, lr, #232
	ldr	r0, [r4, #1132]
	str	r10, [fp, #-60]
	ldr	r10, [fp, #-52]
	str	r0, [lr, #204]
	ldr	r0, [r4, #1140]
	str	r0, [lr, #208]
	ldr	r0, [r4, #1144]
	str	r0, [lr, #212]
	ldr	r0, [r4, #1124]
	str	r0, [lr, #216]
	ldr	r0, [r4, #1128]
	str	r0, [r10, #4]
	ldr	r0, [fp, #-48]
	ldr	r0, [r0, #984]
	str	r0, [lr, #192]
	ldr	lr, [r4, #1112]
	str	lr, [ip, #196]
	ldr	lr, [r4, #1116]
	str	lr, [ip, #200]
	ldr	ip, [r6, #28]
	str	ip, [r10, #8]
	ldr	r10, [fp, #-56]
	ldr	lr, [r6, #32]
	str	lr, [r10, #4]
	ldr	r0, [r4, #1148]
	str	r0, [r10, #8]
	ldr	r10, [fp, #-60]
	ldr	r0, [r4, #1152]
	str	r0, [r10, #4]
	ldr	r0, [r4, #1156]
	str	r0, [r10, #8]
	mov	r0, #1
	strb	r8, [r9, #156]
	strb	r8, [r9, #157]
	strb	r8, [r9, #158]
	str	r0, [r9, #252]
	ldr	r3, [r2, #3324]
	cmp	r3, r8
	bne	.L2069
	mov	r0, #1
	strb	r0, [r6, #12]
	str	r3, [r2, #3236]
	str	r3, [r2, #3244]
	ldr	r8, [r4, #80]
	str	ip, [r2, #3296]
	str	lr, [r2, #3300]
	str	r8, [r2, #3260]
	ldr	r8, [r4, #1148]
	str	r8, [r2, #3304]
	ldr	r8, [r4, #1152]
	str	r8, [r2, #3308]
	ldr	r8, [r4, #1156]
	strb	r3, [r2, #3228]
	strb	r3, [r2, #3229]
	str	r8, [r2, #3312]
	strb	r3, [r2, #3230]
	str	r0, [r2, #3324]
.L2069:
	ldr	r3, [r2, #3984]
	ldrsb	r0, [r6, #13]
	cmp	r3, r0
	bcs	.L2060
	ldr	r0, [r5, #8]
	cmp	r0, #255
	beq	.L2060
	add	r8, r4, r3, lsl #3
	add	r0, r6, r3, lsl #3
	add	r0, r0, #24576
	add	r3, r3, #1
	ldr	r8, [r8, #1164]
	mov	r10, #0
	mov	r9, #1
	strb	r10, [r0, #3821]
	strb	r9, [r0, #3820]
	str	r8, [r0, #3816]
	str	r3, [r2, #3984]
.L2060:
	ldr	r3, [r2, #3988]
	ldr	r0, [r4, #1160]
	cmp	r3, r0
	bcs	.L2061
	ldr	r0, [r5, #4]
	cmp	r0, #255
	beq	.L2061
	ldr	r9, [r4, #1340]
	mov	r0, #100
	mla	r0, r0, r3, r6
	sub	r9, r9, #1
	mov	r10, r3, asl #4
	add	r5, r3, #1
	mov	r8, r9, asl #4
	sub	r3, r10, r3, asl #2
	sub	r8, r8, r9, asl #2
	add	r6, r6, r3
	add	r8, r4, r8
	add	r3, r6, #27904
	add	r0, r0, #21248
	mov	r9, #0
	ldr	r10, [r8, #600]
	mov	r8, #1
	str	r10, [r0, #180]
	str	r9, [r3, #108]
	str	r8, [r3, #104]
	str	r5, [r2, #3988]
.L2061:
	ldr	r3, [r4, #1324]
	str	r3, [r2, #3812]
	ldr	r3, [r1, #920]
	cmp	r3, #0
	bne	.L2062
	ldr	r0, [r1, #940]
	mov	r2, #1
	str	r3, [r1, #832]
	str	r0, [r1, #840]
	ldr	r0, [r4, #80]
	str	ip, [r1, #892]
	str	lr, [r1, #896]
	str	r0, [r1, #856]
	ldr	r0, [r4, #1148]
	str	r0, [r1, #900]
	ldr	r0, [r4, #1152]
	str	r0, [r1, #904]
	ldr	r0, [r4, #1156]
	strb	r3, [r1, #824]
	strb	r3, [r1, #825]
	str	r0, [r1, #908]
	strb	r3, [r1, #826]
	str	r2, [r1, #920]
.L2062:
	ldr	r3, [r7, #16]
	mov	r0, #8
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2106:
	ldr	r3, [r7, #16]
	mov	r0, #8
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2114:
	ldr	r3, [r5, #8]
	cmp	r3, #255
	beq	.L2053
.L2067:
	ldrb	r1, [r4, #1168]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L2054
	mov	r3, #1
.L2055:
	mov	r1, r3, asl #3
	add	r3, r3, #1
	add	r2, r1, #294912
	add	r2, r2, #1168
	ldrb	r2, [r9, r2]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2054
	cmp	r3, #20
	bne	.L2055
	b	.L2053
.L2113:
	ldr	r3, [r3, #1924]
	cmp	r3, #0
	bne	.L2048
	ldrsb	r3, [r6, #23]
	ldr	r2, [r4, #1160]
	add	r3, r3, #4
	cmp	r3, r2
	movge	r3, r2
	strb	r3, [r6, #10]
	b	.L2049
.L2054:
	ldr	r2, [r5, #64]
	add	r1, r9, r1
	add	r1, r1, #294912
	mov	r3, #1
	str	r2, [r1, #1164]
	strb	r3, [r1, #1168]
	ldr	r3, [r4, #1336]
	add	r3, r3, #1
	str	r3, [r4, #1336]
	b	.L2053
.L2070:
	mov	r2, #0
	b	.L2041
.L2073:
	mov	r3, lr
	b	.L2056
.L2117:
	.align	2
.L2116:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	s_pFspInst
	UNWIND(.fnend)
	.size	VCTRL_SetSingleFrameBufToChan, .-VCTRL_SetSingleFrameBufToChan
	.align	2
	.global	VCTRL_AllocMemToDecoder
	.type	VCTRL_AllocMemToDecoder, %function
VCTRL_AllocMemToDecoder:
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
	ldr	r4, .L2127
	mov	r7, r0, asl #2
	mov	r6, r0
	add	r3, r4, r7
	ldr	r8, [r3, #-3704]
	cmp	r8, #0
	bne	.L2119
	mov	r2, #1
	str	r2, [r3, #-3704]
	bl	VCTRL_SetSingleFrameBufToChan
	subs	r5, r0, #0
	bne	.L2120
	ldr	r3, .L2127+4
	ldr	ip, [r3, r6, asl #2]
	add	ip, ip, #290816
.L2121:
	add	r4, r4, r7
	mov	r3, #2
	ldr	r1, .L2127+8
	mov	r0, #31
	str	r3, [ip, #1824]
	bl	dprint_vfmw
	mov	r3, #0
	mov	r0, r5
	str	r3, [r4, #-3704]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2120:
	ldr	r9, .L2127+4
	ldr	ip, [r9, r6, asl #2]
	add	ip, ip, #290816
	ldr	r3, [ip, #1844]
	cmp	r3, #0
	beq	.L2121
	mov	r2, r6
	mov	r0, r8
	ldr	r1, .L2127+12
	bl	dprint_vfmw
	ldr	r3, [r9, r6, asl #2]
	mov	r2, #3
	add	r3, r3, #290816
	mov	ip, r3
	str	r2, [r3, #1824]
	b	.L2121
.L2119:
	mov	r3, r0
	ldr	r2, .L2127+16
	ldr	r1, .L2127+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2128:
	.align	2
.L2127:
	.word	.LANCHOR4
	.word	.LANCHOR0
	.word	.LC216
	.word	.LC215
	.word	.LANCHOR1+304
	.word	.LC217
	UNWIND(.fnend)
	.size	VCTRL_AllocMemToDecoder, .-VCTRL_AllocMemToDecoder
	.align	2
	.global	DBG_WriteBigTileYuv
	.type	DBG_WriteBigTileYuv, %function
DBG_WriteBigTileYuv:
	UNWIND(.fnstart)
	@ args = 32, pretend = 0, frame = 48
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
	mov	r5, r3
	ldr	r3, [fp, #28]
	cmp	r0, #0
	str	r0, [fp, #-64]
	mov	r7, r1
	ldr	r0, [fp, #4]
	mov	r6, r2
	mov	r4, r3
	str	r3, [fp, #-88]
	ldr	r3, [fp, #32]
	mov	lr, r0
	rsb	r1, r4, lr
	rsb	r3, r3, r1
	mov	r1, r5, lsr #1
	str	r1, [fp, #-72]
	mov	r1, lr, lsr #1
	str	r1, [fp, #-56]
	ldr	r1, [fp, #12]
	ldr	ip, [fp, #20]
	str	r0, [fp, #-52]
	str	r1, [fp, #-60]
	ldr	r1, [fp, #16]
	ldr	r0, [fp, #24]
	str	ip, [fp, #-84]
	rsb	ip, ip, r5
	rsb	r2, r0, ip
	str	r3, [fp, #-76]
	str	r2, [fp, #-80]
	ldr	r4, [fp, #8]
	str	r1, [fp, #-68]
	beq	.L2129
	ldr	r1, .L2230
	mov	r0, #32
	bl	dprint_vfmw
	cmp	r4, #0
	cmpne	r4, #3
	bne	.L2132
	ldr	r3, [fp, #-52]
	ldr	r4, .L2230+4
	cmp	r3, #0
	beq	.L2134
	ldr	r10, .L2230+8
	mov	r3, #0
	mov	r8, r3
	str	r3, [fp, #-48]
	str	r6, [fp, #-92]
.L2136:
	cmp	r5, #0
	beq	.L2137
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r6, r3, #15
	mov	r2, r3, lsr #4
	ldr	r3, [fp, #-60]
	mul	r2, r3, r2
	add	r6, r2, r6, lsl #8
.L2135:
	mov	r1, r9, lsr #8
	ldr	r0, [r4, #-3116]
	add	r2, r8, r9
	ldr	r3, [r10, #52]
	add	r1, r6, r1, lsl #12
	add	r0, r0, r2
	add	r1, r7, r1
	add	r9, r9, #256
	mov	r2, #256
	blx	r3
	cmp	r5, r9
	bhi	.L2135
.L2137:
	ldr	r3, [fp, #-48]
	add	r8, r8, r5
	ldr	r2, [fp, #-52]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2136
	ldr	r6, [fp, #-92]
.L2134:
	ldr	r3, [fp, #-80]
	ldr	r2, [fp, #-76]
	ldr	r1, [fp, #-52]
	cmp	r3, r5
	cmpeq	r2, r1
	moveq	r3, #1
	movne	r3, #0
	str	r3, [fp, #-92]
	beq	.L2225
	ldr	r2, [fp, #-84]
	ldr	r3, [fp, #-88]
	ldr	r9, [fp, #-76]
	mla	r7, r3, r5, r2
	cmp	r9, #0
	ldr	r2, [r4, #-3116]
	add	r7, r2, r7
	beq	.L2142
	ldr	r10, .L2230+8
	str	r6, [fp, #-48]
	str	r4, [fp, #-52]
	ldr	r8, [fp, #-92]
	mov	r4, r10
	ldr	r6, [fp, #-80]
	ldr	r10, [fp, #-64]
.L2143:
	mov	r0, r7
	add	r8, r8, #1
	ldr	r3, [r4, #44]
	mov	r2, r10
	mov	r1, r6
	add	r7, r7, r5
	blx	r3
	cmp	r8, r9
	bne	.L2143
	ldr	r6, [fp, #-48]
	ldr	r4, [fp, #-52]
.L2142:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	beq	.L2139
	ldr	r3, [fp, #-60]
	mov	r8, #0
	ldr	r10, .L2230+8
	str	r8, [fp, #-48]
	mov	r3, r3, lsr #1
	str	r3, [fp, #-52]
.L2140:
	cmp	r5, #0
	beq	.L2147
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r7, r3, #7
	mov	r2, r3, lsr #3
	ldr	r3, [fp, #-52]
	mul	r2, r3, r2
	add	r7, r2, r7, lsl #8
.L2145:
	mov	r1, r9, lsr #8
	ldr	r0, [r4, #-3116]
	add	r2, r8, r9
	ldr	r3, [r10, #52]
	add	r1, r7, r1, lsl #11
	add	r0, r0, r2
	add	r1, r6, r1
	add	r9, r9, #256
	mov	r2, #256
	blx	r3
	cmp	r5, r9
	bhi	.L2145
.L2147:
	ldr	r3, [fp, #-48]
	add	r8, r8, r5
	ldr	r2, [fp, #-56]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2140
	ldr	r3, [fp, #-68]
	ldr	lr, [r4, #-3116]
	cmp	r3, #1
	beq	.L2226
.L2149:
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	beq	.L2227
.L2150:
	ldr	r3, [fp, #-92]
	cmp	r3, #0
	bne	.L2228
	ldr	r1, [fp, #-76]
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-84]
	movs	r5, r1, lsr #1
	ldr	r1, [fp, #-72]
	mov	r3, r3, lsr #1
	ldr	r7, [r4, #-3120]
	mov	r2, r2, lsr #1
	ldr	r4, [r4, #-3124]
	mla	r3, r3, r1, r2
	add	r7, r7, r3
	add	r4, r4, r3
	beq	.L2129
	ldr	r3, [fp, #-80]
	mov	r9, r1
	str	r4, [fp, #-48]
	ldr	r10, .L2230+8
	ldr	r8, [fp, #-92]
	mov	r6, r3, lsr #1
	ldr	r4, [fp, #-64]
.L2157:
	mov	r0, r7
	add	r8, r8, #1
	ldr	r3, [r10, #44]
	mov	r2, r4
	mov	r1, r6
	add	r7, r7, r9
	blx	r3
	cmp	r8, r5
	bne	.L2157
	ldr	r4, [fp, #-48]
	mov	r7, #0
	ldr	r8, [fp, #-72]
	ldr	r9, [fp, #-64]
.L2158:
	mov	r0, r4
	add	r7, r7, #1
	ldr	r3, [r10, #44]
	mov	r2, r9
	mov	r1, r6
	add	r4, r4, r8
	blx	r3
	cmp	r7, r5
	bne	.L2158
.L2129:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2132:
	subs	r3, r4, #1
	ldr	r2, [fp, #-52]
	ldr	r10, .L2230+8
	movne	r3, #1
	cmp	r2, r3
	str	r3, [fp, #-76]
	ldrls	r4, .L2230+4
	bls	.L2160
	ldr	r3, [fp, #-76]
	ldr	r4, .L2230+4
	str	r6, [fp, #-80]
	str	r3, [fp, #-48]
.L2162:
	cmp	r5, #0
	beq	.L2163
	ldr	r3, [fp, #-48]
	mov	r6, #0
	and	r8, r3, #15
	mov	r2, r3, lsr #4
	mov	r9, r3, lsr #1
	ldr	r3, [fp, #-60]
	mul	r9, r5, r9
	mul	r2, r3, r2
	add	r8, r2, r8, lsl #8
.L2161:
	mov	r1, r6, lsr #8
	ldr	r0, [r4, #-3116]
	add	r2, r6, r9
	ldr	r3, [r10, #52]
	add	r1, r8, r1, lsl #12
	add	r0, r0, r2
	add	r1, r7, r1
	add	r6, r6, #256
	mov	r2, #256
	blx	r3
	cmp	r5, r6
	bhi	.L2161
.L2163:
	ldr	r3, [fp, #-48]
	ldr	r2, [fp, #-52]
	add	r3, r3, #2
	str	r3, [fp, #-48]
	cmp	r2, r3
	bhi	.L2162
	ldr	r6, [fp, #-80]
.L2160:
	ldr	r3, [fp, #-52]
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-64]
	mul	r1, r3, r5
	ldr	r0, [r4, #-3116]
	mov	r1, r1, lsr #1
	blx	ip
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	beq	.L2164
	ldr	r3, [fp, #-60]
	mov	r8, #0
	str	r8, [fp, #-48]
	mov	r3, r3, lsr #1
	str	r3, [fp, #-52]
.L2165:
	cmp	r5, #0
	beq	.L2168
	ldr	r3, [fp, #-48]
	mov	r7, #0
	and	r9, r3, #7
	mov	r2, r3, lsr #3
	ldr	r3, [fp, #-52]
	mul	r2, r3, r2
	add	r9, r2, r9, lsl #8
.L2166:
	mov	r1, r7, lsr #8
	ldr	r0, [r4, #-3116]
	add	r2, r8, r7
	ldr	r3, [r10, #52]
	add	r1, r9, r1, lsl #11
	add	r0, r0, r2
	add	r1, r6, r1
	add	r7, r7, #256
	mov	r2, #256
	blx	r3
	cmp	r5, r7
	bhi	.L2166
.L2168:
	ldr	r3, [fp, #-48]
	add	r8, r8, r5
	ldr	r2, [fp, #-56]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2165
	ldr	r3, [fp, #-68]
	ldr	r2, [r4, #-3116]
	cmp	r3, #1
	beq	.L2229
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	bne	.L2171
	ldr	r2, [fp, #-56]
	ldr	r3, [fp, #-76]
	cmp	r2, r3
	bls	.L2176
.L2177:
	add	r3, r3, #2
	cmp	r2, r3
	bhi	.L2177
.L2176:
	ldr	ip, [r10, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r4, #-3124]
	blx	ip
	ldr	ip, [r10, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r4, #-3120]
	blx	ip
.L2171:
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-72]
	ldr	r6, [fp, #-64]
	ldr	ip, [r10, #44]
	mul	r5, r3, r2
	ldr	r0, [r4, #-3120]
	mov	r2, r6
	mov	r5, r5, lsr #1
	mov	r1, r5
	blx	ip
	ldr	r3, [r10, #44]
	mov	r1, r5
	mov	r2, r6
.L2223:
	ldr	r0, [r4, #-3124]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L2225:
	ldr	r10, .L2230+8
	ldr	r2, [fp, #-64]
	mul	r1, r1, r5
	ldr	r0, [r4, #-3116]
	ldr	r3, [r10, #44]
	blx	r3
	b	.L2142
.L2229:
	ldr	r3, [fp, #-76]
	mov	r6, #0
	ldr	lr, [fp, #-72]
	ldr	r8, [fp, #-56]
	mla	r2, r3, r5, r2
	mov	r5, r5, asl #1
	add	r7, r2, #1
.L2172:
	cmp	lr, #0
	beq	.L2175
	mov	r3, r6, lsr #1
	mov	r2, r7
	mul	r3, lr, r3
	add	ip, r3, lr
.L2173:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	ldr	r1, [r4, #-3124]
	strb	r0, [r1, r3]
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	ldr	r1, [r4, #-3120]
	strb	r0, [r1, r3]
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L2173
.L2175:
	add	r6, r6, #2
	add	r7, r7, r5
	cmp	r8, r6
	bhi	.L2172
	b	.L2171
.L2226:
	ldr	r7, [fp, #-72]
	mov	r3, #0
	ldr	r8, [fp, #-56]
	add	lr, lr, #1
	mov	r6, r3
.L2151:
	cmp	r7, #0
	moveq	ip, r3
	beq	.L2154
	add	ip, r3, r7
	mov	r2, lr
.L2152:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	ldr	r1, [r4, #-3124]
	strb	r0, [r1, r3]
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	ldr	r1, [r4, #-3120]
	strb	r0, [r1, r3]
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L2152
.L2154:
	add	r6, r6, #1
	mov	r3, ip
	cmp	r6, r8
	add	lr, lr, r5
	bne	.L2151
	b	.L2150
.L2227:
	ldr	r10, .L2230+8
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r4, #-3124]
	ldr	ip, [r10, #48]
	blx	ip
	ldr	r3, [r10, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r4, #-3120]
	blx	r3
	b	.L2150
.L2228:
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-72]
	ldr	r7, [fp, #-64]
	ldr	r6, .L2230+8
	mul	r5, r3, r2
	ldr	r0, [r4, #-3120]
	mov	r2, r7
	ldr	r3, [r6, #44]
	mov	r1, r5
	blx	r3
	ldr	r3, [r6, #44]
	mov	r1, r5
	mov	r2, r7
	b	.L2223
.L2164:
	ldr	r3, [fp, #-68]
	cmp	r3, #1
	beq	.L2171
	cmp	r3, #0
	beq	.L2176
	b	.L2171
.L2139:
	ldr	r3, [fp, #-68]
	cmp	r3, #1
	bne	.L2149
	b	.L2150
.L2231:
	.align	2
.L2230:
	.word	.LC218
	.word	.LANCHOR4
	.word	vfmw_Osal_Func_Ptr_S
	UNWIND(.fnend)
	.size	DBG_WriteBigTileYuv, .-DBG_WriteBigTileYuv
	.align	2
	.global	VCTRL_GetChanImage
	.type	VCTRL_GetChanImage, %function
VCTRL_GetChanImage:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 640
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #676)
	sub	sp, sp, #676
	mov	r5, r0
	mov	r2, r0
	mov	r4, r1
	mov	r0, #2
	ldr	r1, .L2290
	bl	dprint_vfmw
	cmp	r5, #15
	bhi	.L2279
	ldr	r6, .L2290+4
	ldr	r0, [r6, r5, asl #2]
	cmp	r0, #0
	beq	.L2280
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L2281
	ldr	r7, .L2290+8
	mov	r0, #4
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r3, [r6, r5, asl #2]
	ldr	r2, [r3, #20]
	cmp	r2, #1
	bne	.L2282
	movw	r0, #34728
	mov	r1, r4
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReceiveImage
	sub	r9, r0, #1
	mov	r8, r0
	clz	r9, r9
	mov	r9, r9, lsr #5
	cmp	r4, #0
	moveq	r3, #0
	andne	r3, r9, #1
	cmp	r3, #0
	beq	.L2239
	ldr	r10, .L2290+12
	mov	r3, r5, asl #2
	str	r3, [fp, #-672]
	add	r3, r10, r3
	ldr	r0, [r3, #-3280]
	cmp	r0, #0
	beq	.L2240
	ldr	r3, [r4, #108]
	ldr	ip, [r10, #-3112]
	tst	r3, #28
	ldr	r3, [r4, #112]
	moveq	lr, #1
	movne	lr, #0
	cmp	ip, #0
	beq	.L2241
	ldr	ip, [r4, #344]
	ldr	r2, [r4, #512]
	ldr	r1, [r4, #504]
	str	lr, [sp, #12]
	ldr	lr, [r4, #340]
	str	ip, [sp, #28]
	ldr	ip, [r4, #336]
	str	lr, [sp, #24]
	ldr	lr, [r4, #332]
	str	ip, [sp, #20]
	ldr	ip, [r4, #140]
	str	lr, [sp, #16]
	ldr	lr, [r4, #116]
	str	ip, [sp, #8]
	mov	ip, #0
	str	ip, [sp, #4]
	str	lr, [sp]
	bl	DBG_WriteBigTileYuv
.L2240:
	ldr	r3, [fp, #-672]
	add	r10, r10, r3
	ldr	r2, [r10, #-2972]
	cmp	r2, #0
	beq	.L2239
	ldrd	r0, [r4, #232]
	orrs	r3, r0, r1
	beq	.L2239
	ldr	r3, [r4, #224]
	cmp	r3, #0
	bne	.L2283
.L2239:
	cmp	r9, #0
	bne	.L2284
.L2243:
	subs	r9, r8, #1
	mvnne	r9, #0
.L2245:
	mov	r0, #12
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L2256
.L2254:
	cmp	r9, #0
	beq	.L2257
.L2255:
	ldr	r3, [r6, r5, asl #2]
	mov	r0, #4
	ldr	r2, [r7, #16]
	add	r3, r3, #311296
	ldrb	r3, [r3, #1340]	@ zero_extendqisi2
	strb	r3, [r4, #472]
	blx	r2
.L2277:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2284:
	ldr	r3, [r4, #200]
	mvn	r9, #0
	str	r9, [r4, #196]
	cmp	r3, #1
	bne	.L2243
	ldr	r2, [r4, #188]
	cmp	r2, #0
	beq	.L2285
	ldr	r0, [r4, #32]
	mov	r2, #3
	ldr	r1, [r4, #28]
	str	r9, [r4, #196]
	str	r3, [r4, #188]
	str	r0, [r4, #208]
	str	r1, [r4, #204]
	str	r2, [r4, #356]
.L2253:
	mov	r0, #12
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L2286
.L2257:
	ldr	r2, [r4, #108]
	mov	r9, #0
	ldr	r3, [r6, r5, asl #2]
	str	r2, [r3, #364]
	ldr	r2, [r4, #356]
	str	r2, [r3, #368]
	b	.L2255
.L2286:
	mov	r9, #0
	mov	r8, #1
.L2256:
	ldr	r3, [r7]
	blx	r3
	ldr	r3, .L2290+12
	ldr	r1, .L2290+16
	add	r3, r3, r5, lsl #2
	ldr	r2, [r3, #-2692]
	ldr	r1, [r1]
	rsb	r2, r2, r0
	mov	ip, r0
	cmp	r2, r1
	bls	.L2254
	mov	r2, r5, asl #24
	movw	r1, #4803
	add	r2, r2, #786432
	ldr	r0, .L2290+20
	cmp	r8, #1
	addeq	r2, r2, #1
	str	ip, [r3, #-2692]
	bl	DBG_AddTrace
	b	.L2254
.L2241:
	str	r3, [fp, #-676]
	ldr	r3, [r4, #140]
	ldr	r2, [r4, #512]
	ldr	r1, [r4, #504]
	str	r3, [fp, #-680]
	str	ip, [sp, #28]
	str	ip, [sp, #24]
	str	ip, [sp, #20]
	str	ip, [sp, #16]
	str	lr, [sp, #12]
	ldr	lr, [r4, #116]
	str	ip, [sp, #4]
	ldr	ip, [fp, #-680]
	ldr	r3, [fp, #-676]
	str	lr, [sp]
	str	ip, [sp, #8]
	bl	DBG_WriteBigTileYuv
	b	.L2240
.L2281:
	ldr	r3, .L2290+24
	movw	r2, #4573
	ldr	r1, .L2290+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r9, #0
	b	.L2277
.L2282:
	str	r2, [sp]
	movw	r3, #4607
	ldr	r2, .L2290+32
	mov	r0, #0
	ldr	r1, .L2290+36
	mvn	r9, #0
	bl	dprint_vfmw
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	b	.L2277
.L2283:
	mov	r1, r3
	ldr	r3, [r7, #44]
	blx	r3
	b	.L2239
.L2285:
	ldr	r3, [r6, r5, asl #2]
	movw	r0, #34728
	movt	r0, 4
	sub	r1, fp, #668
	add	r0, r3, r0
	str	r2, [fp, #-672]
	bl	SYNTAX_ReceiveImage
	ldr	r10, .L2290+4
	ldr	r2, [fp, #-672]
	cmp	r0, #1
	mov	r8, r0
	bne	.L2287
	ldr	r3, [r4, #192]
	ldr	r1, [fp, #-476]
	cmp	r3, r1
	bge	.L2248
	ldr	r9, [fp, #-480]
	mov	r3, #3
	ldr	r10, [fp, #-636]
	ldr	lr, [fp, #-640]
	cmp	r9, #0
	ldr	ip, [fp, #-632]
	ldr	r0, [fp, #-628]
	movne	r9, r2
	ldr	r1, [fp, #-512]
	str	r10, [r4, #208]
	str	lr, [r4, #204]
	str	ip, [r4, #212]
	str	r0, [r4, #216]
	str	r1, [r4, #196]
	str	r3, [r4, #356]
	strne	r8, [r4, #188]
	b	.L2245
.L2279:
	ldr	r3, .L2290+40
	movw	r2, #4569
	ldr	r1, .L2290+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r9, #0
	b	.L2277
.L2287:
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #34728
	movt	r0, 4
	mov	r9, r8
	add	r0, r3, r0
	bl	SYNTAX_ReturnImage
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	b	.L2277
.L2248:
	bgt	.L2288
	cmp	r3, #0
	movw	r0, #34728
	ldr	r3, [r10, r5, asl #2]
	movt	r0, 4
	bne	.L2251
	add	r0, r3, r0
	bl	SYNTAX_ReturnImage
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #34728
	mov	r1, r4
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	mov	r0, r8
	ldr	r3, [r4, #192]
	mvn	r8, #1
	ldr	r2, .L2290+32
	ldr	r1, .L2290+44
	bl	dprint_vfmw
	b	.L2245
.L2251:
	add	r0, r3, r0
	mov	r1, r4
	bl	SYNTAX_ReleaseImage
	ldr	r3, [fp, #-480]
	cmp	r3, #0
	beq	.L2289
	ldr	r3, [r4, #192]
	mov	r0, r8
	ldr	r2, .L2290+32
	ldr	r1, .L2290+48
	bl	dprint_vfmw
	sub	r1, fp, #668
	ldr	r3, [r7, #52]
	mov	r2, #624
	mov	r0, r4
	blx	r3
	ldr	r1, [fp, #-636]
	ldr	r2, [fp, #-640]
	mov	r3, #3
	str	r9, [r4, #196]
	str	r8, [r4, #188]
	str	r1, [r4, #208]
	str	r2, [r4, #204]
	str	r3, [r4, #356]
	b	.L2253
.L2280:
	ldr	r3, .L2290+52
	movw	r2, #4570
	ldr	r1, .L2290+28
	mvn	r9, #0
	bl	dprint_vfmw
	b	.L2277
.L2288:
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #34728
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReturnImage
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #34728
	mov	r1, r4
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	ldr	ip, [fp, #-476]
	ldr	r3, [r4, #192]
	mov	r0, r8
	ldr	r2, .L2290+32
	mvn	r8, #1
	ldr	r1, .L2290+56
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L2245
.L2289:
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #34728
	sub	r1, fp, #668
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	mov	r0, r8
	ldr	r3, [r4, #192]
	mvn	r8, #1
	ldr	r2, .L2290+32
	ldr	r1, .L2290+48
	bl	dprint_vfmw
	b	.L2245
.L2291:
	.align	2
.L2290:
	.word	.LC219
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR4
	.word	g_TraceImgPeriod
	.word	.LANCHOR1+348
	.word	.LC38
	.word	.LC1
	.word	.LANCHOR1+328
	.word	.LC220
	.word	.LC12
	.word	.LC222
	.word	.LC223
	.word	.LC2
	.word	.LC221
	UNWIND(.fnend)
	.size	VCTRL_GetChanImage, .-VCTRL_GetChanImage
	.align	2
	.global	VCTRL_GetImageWidth
	.type	VCTRL_GetImageWidth, %function
VCTRL_GetImageWidth:
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
	ldr	r3, .L2300
	ldr	r0, [r3, r0, asl #2]
	add	r0, r0, #294912
	ldr	r3, [r0, #1960]
	cmp	r3, #2
	beq	.L2294
	cmp	r3, #16
	beq	.L2295
	cmp	r3, #0
	beq	.L2299
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2299:
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageWidth
.L2295:
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageWidth
.L2294:
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageWidth
.L2301:
	.align	2
.L2300:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetImageWidth, .-VCTRL_GetImageWidth
	.align	2
	.global	VCTRL_GetImageHeight
	.type	VCTRL_GetImageHeight, %function
VCTRL_GetImageHeight:
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
	ldr	r3, .L2310
	ldr	r0, [r3, r0, asl #2]
	add	r0, r0, #294912
	ldr	r3, [r0, #1960]
	cmp	r3, #2
	beq	.L2304
	cmp	r3, #16
	beq	.L2305
	cmp	r3, #0
	beq	.L2309
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2309:
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageHeight
.L2305:
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageHeight
.L2304:
	add	r0, r0, #1968
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageHeight
.L2311:
	.align	2
.L2310:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetImageHeight, .-VCTRL_GetImageHeight
	.align	2
	.global	VCTRL_GetFsSize
	.type	VCTRL_GetFsSize, %function
VCTRL_GetFsSize:
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
	UNWIND(.pad #32)
	sub	sp, sp, #32
	ldr	r3, .L2317
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	sub	r0, fp, #52
	ldr	r3, [r3, #48]
	mov	r2, #24
	mov	r1, #0
	blx	r3
	ldr	r3, .L2317+4
	ldr	r4, [r3, #52]
	cmp	r4, #0
	beq	.L2313
	ldr	ip, .L2317+8
	sub	r3, fp, #52
	str	r3, [sp, #4]
	mov	r2, r7
	mov	r1, r6
	mov	r0, r5
	ldr	ip, [ip, r5, asl #2]
	mov	r3, #0
	add	ip, ip, #294912
	ldr	ip, [ip, #1960]
	str	ip, [sp]
	blx	r4
.L2313:
	ldr	r0, [fp, #-48]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2318:
	.align	2
.L2317:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetFsSize, .-VCTRL_GetFsSize
	.align	2
	.global	VCTRL_EnableEsBuf
	.type	VCTRL_EnableEsBuf, %function
VCTRL_EnableEsBuf:
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
	ldr	r3, .L2320
	mov	r2, #1
	add	r0, r3, r0, lsl #2
	str	r2, [r0, #-2628]
	ldmfd	sp, {fp, sp, pc}
.L2321:
	.align	2
.L2320:
	.word	.LANCHOR4
	UNWIND(.fnend)
	.size	VCTRL_EnableEsBuf, .-VCTRL_EnableEsBuf
	.align	2
	.global	VCTRL_DisableEsBuf
	.type	VCTRL_DisableEsBuf, %function
VCTRL_DisableEsBuf:
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
	ldr	r3, .L2323
	mov	r2, #0
	add	r0, r3, r0, lsl #2
	str	r2, [r0, #-2628]
	ldmfd	sp, {fp, sp, pc}
.L2324:
	.align	2
.L2323:
	.word	.LANCHOR4
	UNWIND(.fnend)
	.size	VCTRL_DisableEsBuf, .-VCTRL_DisableEsBuf
	.align	2
	.global	VCTRL_IfEsBufEnable
	.type	VCTRL_IfEsBufEnable, %function
VCTRL_IfEsBufEnable:
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
	ldr	r3, .L2326
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #-2628]
	ldmfd	sp, {fp, sp, pc}
.L2327:
	.align	2
.L2326:
	.word	.LANCHOR4
	UNWIND(.fnend)
	.size	VCTRL_IfEsBufEnable, .-VCTRL_IfEsBufEnable
	.global	g_EsBufValidFlag
	.global	s_VfmwSavePath
	.global	g_DnrIrqHandle
	.global	g_VdecIrqHandle
	.global	g_StopDec
	.global	g_DnrSleepTime
	.global	g_SleepTime
	.global	MemRecord_YUV
	.global	pVl
	.global	pUl
	.global	pY
	.global	g_u32DFSMaxAllocTime
	.global	pOption
	.global	g_eVdmVersion
	.global	g_VdmCharacter
	.global	g_CurProcChan
	.global	g_ResetCreateNewSave
	.global	save_yuv_with_crop
	.global	save_yuv_chan_num
	.global	save_el_chan_num
	.global	vfmw_save_el_file
	.global	save_metadata_reorder_chan_num
	.global	vfmw_save_metadata_reorder_file
	.global	save_metadata_chan_num
	.global	vfmw_save_metadata_file
	.global	vfmw_save_yuv_file
	.global	save_seg_chan_num
	.global	vfmw_save_seg_file
	.global	save_str_chan_num
	.global	vfmw_save_str_file
	.global	g_apple_airplay_mirrorinig_flag
	.global	g_not_allow_H264FullPictureRepair_flag
	.global	g_not_direct_8x8_inference_flag
	.global	g_allow_B_before_P
	.global	extra_cmd
	.global	extra_disp
	.global	extra_ref
	.global	g_VfmwGlobalStat
	.global	g_VfmwChanStat
	.global	s_pstVfmwChan
	.global	AcceleratorCharacter
	.global	g_event_report
	.global	g_SCDID
	.global	g_DSPState
	.global	mask_mmu_err_int
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__func__.54738, %object
	.size	__func__.54738, 18
__func__.54738:
	.ascii	"VCTRL_EventReport\000"
	.space	2
	.type	__func__.54750, %object
	.size	__func__.54750, 18
__func__.54750:
	.ascii	"VCTRL_SetCallBack\000"
	.space	2
	.type	__FUNCTION__.54856, %object
	.size	__FUNCTION__.54856, 29
__FUNCTION__.54856:
	.ascii	"VCTRL_PartitionMemForStdUser\000"
	.space	3
	.type	__func__.54880, %object
	.size	__func__.54880, 23
__func__.54880:
	.ascii	"VCTRL_FreeChanResource\000"
	.space	1
	.type	__FUNCTION__.55139, %object
	.size	__FUNCTION__.55139, 23
__FUNCTION__.55139:
	.ascii	"VCTRL_ReleaseChanImage\000"
	.space	1
	.type	__func__.55140, %object
	.size	__func__.55140, 23
__func__.55140:
	.ascii	"VCTRL_ReleaseChanImage\000"
	.space	1
	.type	__func__.55166, %object
	.size	__func__.55166, 18
__func__.55166:
	.ascii	"VCTRL_GetDecParam\000"
	.space	2
	.type	__FUNCTION__.55228, %object
	.size	__FUNCTION__.55228, 19
__FUNCTION__.55228:
	.ascii	"VCTRL_FlushDecoder\000"
	.space	1
	.type	__FUNCTION__.55559, %object
	.size	__FUNCTION__.55559, 18
__FUNCTION__.55559:
	.ascii	"GetExtraRefFrmNum\000"
	.space	2
	.type	__FUNCTION__.55565, %object
	.size	__FUNCTION__.55565, 19
__FUNCTION__.55565:
	.ascii	"GetExtraDispFrmNum\000"
	.space	1
	.type	__FUNCTION__.55571, %object
	.size	__FUNCTION__.55571, 16
__FUNCTION__.55571:
	.ascii	"GetExtraCtrlCmd\000"
	.type	__FUNCTION__.55491, %object
	.size	__FUNCTION__.55491, 14
__FUNCTION__.55491:
	.ascii	"DecChanSyntax\000"
	.space	2
	.type	__func__.55057, %object
	.size	__func__.55057, 24
__func__.55057:
	.ascii	"VCTRL_FindOccupiedFrame\000"
	.type	__func__.55549, %object
	.size	__func__.55549, 24
__func__.55549:
	.ascii	"VCTRL_ExtAccGetDecParam\000"
	.type	__func__.55824, %object
	.size	__func__.55824, 24
__func__.55824:
	.ascii	"VCTRL_AllocMemToDecoder\000"
	.type	__func__.55121, %object
	.size	__func__.55121, 19
__func__.55121:
	.ascii	"VCTRL_GetChanImage\000"
	.space	1
	.type	__FUNCTION__.55129, %object
	.size	__FUNCTION__.55129, 19
__FUNCTION__.55129:
	.ascii	"VCTRL_GetChanImage\000"
	.data
	.align	2
.LANCHOR3 = . + 0
	.type	g_not_direct_8x8_inference_flag, %object
	.size	g_not_direct_8x8_inference_flag, 1
g_not_direct_8x8_inference_flag:
	.byte	1
	.space	3
	.type	s_VfmwSavePath, %object
	.size	s_VfmwSavePath, 64
s_VfmwSavePath:
	.byte	47
	.byte	109
	.byte	110
	.byte	116
	.byte	0
	.space	59
	.type	save_yuv_chan_num, %object
	.size	save_yuv_chan_num, 4
save_yuv_chan_num:
	.word	-1
	.type	mask_mmu_err_int, %object
	.size	mask_mmu_err_int, 4
mask_mmu_err_int:
	.word	1
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	ASCII(.ascii	"InstID out of range\000" )
.LC1:
	ASCII(.ascii	"vfmw_ctrl.c,L%d: %s\012\000" )
	.space	3
.LC2:
	ASCII(.ascii	"Chan inactive\000" )
	.space	2
.LC3:
	ASCII(.ascii	"s_VfmwCTrl.event_report_vdec = NULL\012\000" )
	.space	3
.LC4:
	ASCII(.ascii	"s_VfmwCTrl.event_report_omxvdec = NULL\012\000" )
.LC5:
	ASCII(.ascii	"%s Unkown Adapter Type: %d, Len: %d\012\000" )
	.space	3
.LC6:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC7:
	ASCII(.ascii	"Force vdm %d sleep.\012\000" )
	.space	3
.LC8:
	ASCII(.ascii	"Vfmw suspend totally take %d ms\012\000" )
	.space	3
.LC9:
	ASCII(.ascii	"Wait sleep time out %d ms (isScdSleep=%d, isVdmSlee" )
	ASCII(.ascii	"p=%d)!\012\000" )
	.space	1
.LC10:
	ASCII(.ascii	"Force scd %d sleep.\012\000" )
	.space	3
.LC11:
	ASCII(.ascii	"Vfmw resume totally take %d ms\012\000" )
.LC12:
	ASCII(.ascii	"ChanID out of range\000" )
.LC13:
	ASCII(.ascii	"can NOT config a not-opened Chan\000" )
	.space	3
.LC14:
	ASCII(.ascii	"event_report null!\000" )
	.space	1
.LC15:
	ASCII(.ascii	"%s Unkown Adapter Type: %d\012\000" )
.LC16:
	ASCII(.ascii	"pstStreamParam is NULL\000" )
	.space	1
.LC17:
	ASCII(.ascii	"L: %d, Stream Interface Error!!, ChanID: %d\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"pstStreamIntf is NULL\000" )
	.space	2
.LC19:
	ASCII(.ascii	"This Chan has not been created!!!\000" )
	.space	2
.LC20:
	ASCII(.ascii	"can NOT get a not-opened Chan's information of bits" )
	ASCII(.ascii	"tream\000" )
	.space	3
.LC21:
	ASCII(.ascii	"Raw FreshNum=%d, FreshSize=%d\012\000" )
	.space	1
.LC22:
	ASCII(.ascii	"Seg FreshNum=%d, FreshSize=%d\012\000" )
	.space	1
.LC23:
	ASCII(.ascii	"pArgs is null\000" )
	.space	2
.LC24:
	ASCII(.ascii	"image size abnormal(%dx%d)\012\000" )
.LC25:
	ASCII(.ascii	"no mem to arrange for soft dec\012\000" )
.LC26:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC27:
	ASCII(.ascii	"%s: L%d : vdm chan memory is too small to support u" )
	ASCII(.ascii	"ser decoder\012\000" )
.LC28:
	ASCII(.ascii	"FSP_ConfigInstance fail!\012\000" )
	.space	2
.LC29:
	ASCII(.ascii	"parameter err, null pointer!!!\000" )
	.space	1
.LC30:
	ASCII(.ascii	"%s FATAL: pstChan = NULL!\012\000" )
	.space	1
.LC31:
	ASCII(.ascii	"%s FATAL: pChanMem_vdh = NULL!\012\000" )
.LC32:
	ASCII(.ascii	"%s FATAL: pChanMem_frm = NULL!\012\000" )
.LC33:
	ASCII(.ascii	"DelAllFrameMemRecord err in VCTRL_FreeChanResource " )
	ASCII(.ascii	"!\012\000" )
	.space	2
.LC34:
	ASCII(.ascii	"%s FATAL: pChanMem_scd = NULL!\012\000" )
.LC35:
	ASCII(.ascii	"%s FATAL: pChanMem_ctx = NULL!\012\000" )
.LC36:
	ASCII(.ascii	"pstCfg is null\000" )
	.space	1
.LC37:
	ASCII(.ascii	"Chip not support beyond %dx%d when Std != H264/VP8/" )
	ASCII(.ascii	"MVC/USER.\012\000" )
	.space	2
.LC38:
	ASCII(.ascii	"This channel is not opened\000" )
	.space	1
.LC39:
	ASCII(.ascii	"pImage is NULL\000" )
	.space	1
.LC40:
	ASCII(.ascii	"call VCTRL_ReleaseChanImage, ChanID, ImgID = %d,%d\012" )
	ASCII(.ascii	"\000" )
.LC41:
	ASCII(.ascii	"vdm_ctrl.c,L%d: Chan inactive\012\000" )
	.space	1
.LC42:
	ASCII(.ascii	"vdm_ctrl.c,L%d: This channel is not opened\012\000" )
.LC43:
	ASCII(.ascii	"vdm_ctrl.c,L%d: VCTRL_TryToRelSpecialFrm success!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC44:
	ASCII(.ascii	"%s call SYNTAX_ReleaseImage failed\012\000" )
.LC45:
	ASCII(.ascii	"vfmw_ctrl.c, line %d: ChanID out of range\012\000" )
	.space	1
.LC46:
	ASCII(.ascii	"vfmw_ctrl.c, line %d: Chan %d is not open or not ru" )
	ASCII(.ascii	"nning\012\000" )
	.space	2
.LC47:
	ASCII(.ascii	"vfmw_ctrl.c, line %d: ChanID out of range is %d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC48:
	ASCII(.ascii	"%s VidStd=%d Error!\012\000" )
	.space	3
.LC49:
	ASCII(.ascii	"Get DecParam Type Failure!\012\000" )
.LC50:
	ASCII(.ascii	"Chan %d is not open or not running\012\000" )
.LC51:
	ASCII(.ascii	"ChanId out of range\000" )
.LC52:
	ASCII(.ascii	"This channel is not running\000" )
.LC53:
	ASCII(.ascii	"line: %d Flush DPB failed!\012\000" )
.LC54:
	ASCII(.ascii	"VCTRL_FlushDecoder\012\000" )
.LC55:
	ASCII(.ascii	"fuction return value is null,%s %d MVC_OK != MVC_Fl" )
	ASCII(.ascii	"ushDPB!!\012\000" )
	.space	3
.LC56:
	ASCII(.ascii	"chan not create\000" )
.LC57:
	ASCII(.ascii	"can NOT start a not-opened Chan\000" )
.LC58:
	ASCII(.ascii	"NULL == pArgs\000" )
	.space	2
.LC59:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC60:
	ASCII(.ascii	"VCTRL_GetUsrdecFrame but VFMW_USER != s_pstVfmwChan" )
	ASCII(.ascii	"[ChanID]->eVidStd!\012\000" )
	.space	1
.LC61:
	ASCII(.ascii	"VCTRL_GetUsrdecFrame but s_pstVfmwChan[ChanID]->s32" )
	ASCII(.ascii	"UsrdecFrameNum < MIN_USERDEC_FRAME_NUM!\012\000" )
.LC62:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC63:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC64:
	ASCII(.ascii	"line: %d, pLf is NULL!\012\000" )
.LC65:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC66:
	ASCII(.ascii	"VCTRL_GetUsrdecFrame get LogicFs fail!\012\000" )
.LC67:
	ASCII(.ascii	"ERROR: s32MaxRefFrameNum(%d), extra_ref(%d), s32Dis" )
	ASCII(.ascii	"playFrameNum(%d), extra_disp(%d)\012\000" )
	.space	3
.LC68:
	ASCII(.ascii	"ERROR: (MaxRefFrameNum(%d) + extra_ref(%d)) > 16\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC69:
	ASCII(.ascii	"CAP_LEVEL_USER_DEFINE_WITH_OPTION option is illegal" )
	ASCII(.ascii	".\012\000" )
	.space	2
.LC70:
	ASCII(.ascii	"VCTRL_GetChanMemSizeByOption L%d: arrange return fa" )
	ASCII(.ascii	"ilure\012\000" )
	.space	2
.LC71:
	ASCII(.ascii	"ERROR: (nPmv(%d) + extra_ref(%d)) <= 0\012\000" )
.LC72:
	ASCII(.ascii	"ERROR: TempMinFrame(%d + %d + %d), TempMiddleFrame(" )
	ASCII(.ascii	"%d + %d + %d), TempFluentFrame(%d + %d + %d)\012\000" )
	.space	3
.LC73:
	ASCII(.ascii	"VCTRL_GetChanMemSize L%d: arrange return failure\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC74:
	ASCII(.ascii	"MEM_MapRegisterAddr reg 0x%x failed!\012\000" )
	.space	2
.LC75:
	ASCII(.ascii	"VDMHAL_OpenHAL Vdh %d failed!\012\000" )
	.space	1
.LC76:
	ASCII(.ascii	"VDMHAL_OpenHAL Scd %d failed!\012\000" )
	.space	1
.LC77:
	ASCII(.ascii	"VDMHAL_GetHalMemSize Vdh %d failed!\012\000" )
	.space	3
.LC78:
	ASCII(.ascii	"MEM_AllocMemBlock Vdh %d failed!\012\000" )
	.space	2
.LC79:
	ASCII(.ascii	"MEM_AllocMemBlock VFMW_VP9Prob %d failed!\012\000" )
	.space	1
.LC80:
	ASCII(.ascii	"SM_OpenSCDDrv Scd %d failed!\012\000" )
	.space	2
.LC81:
	ASCII(.ascii	"MEM_AllocMemBlock Scd %d failed!\012\000" )
	.space	2
.LC82:
	ASCII(.ascii	"Register irq %d failed!\012\000" )
	.space	3
.LC83:
	ASCII(.ascii	"extra_ref=\000" )
	.space	1
.LC84:
	ASCII(.ascii	"=\000" )
	.space	2
.LC85:
	ASCII(.ascii	" \000" )
	.space	2
.LC86:
	ASCII(.ascii	"fuction return value is null,%s %d unknow error!!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC87:
	ASCII(.ascii	"ERROR: extra_ref = %d is out of [ -5000, 5000 ]!\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC88:
	ASCII(.ascii	"extra_disp=\000" )
.LC89:
	ASCII(.ascii	"ERROR: extra_disp = %d is out of [ -5000, 5000 ]!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC90:
	ASCII(.ascii	"extra_cmd=\000" )
	.space	1
.LC91:
	ASCII(.ascii	"HI_VFMW_VideoDecode\000" )
.LC92:
	ASCII(.ascii	"(safe,warn,danger) :amount = %d,%d,%d,%d\012\000" )
	.space	2
.LC93:
	ASCII(.ascii	"strm_ctrl: I->IPB\012\000" )
	.space	1
.LC94:
	ASCII(.ascii	"strm_ctrl: IPB->I\012\000" )
	.space	1
.LC95:
	ASCII(.ascii	"(CurrMode,NextMode) = %d,%d\012\000" )
	.space	3
.LC96:
	ASCII(.ascii	"Syntax is stopped from This channel\000" )
.LC97:
	ASCII(.ascii	"[%d] vdm busy\012\000" )
	.space	1
.LC98:
	ASCII(.ascii	"[%d] no frame\012\000" )
	.space	1
.LC99:
	ASCII(.ascii	"[%d] no stream\012\000" )
.LC100:
	ASCII(.ascii	"seg_pts: %lld, seg_usertag = %lld\012\000" )
	.space	1
.LC101:
	ASCII(.ascii	"pic_pts: %lld\012\000" )
	.space	1
.LC102:
	ASCII(.ascii	"pic_usertag: %lld\012\000" )
	.space	1
.LC103:
	ASCII(.ascii	"pic_pts: %lld pic_usertag = %lld\012\000" )
	.space	2
.LC104:
	ASCII(.ascii	"%s:ChanId %d this hevc file is 10bit!!!\012\000" )
	.space	3
.LC105:
	ASCII(.ascii	"DFS, Wait Partition\012\000" )
	.space	3
.LC106:
	ASCII(.ascii	"DFS, Partition fail, u32DynamicFrameStoreAllocEn: %" )
	ASCII(.ascii	"d, eFspPartitionState: %d\012\000" )
	.space	2
.LC107:
	ASCII(.ascii	"Thread running...\012\000" )
	.space	1
.LC108:
	ASCII(.ascii	"Thread stopped!\012\000" )
	.space	3
.LC109:
	ASCII(.ascii	"thread running...\012\000" )
	.space	1
.LC110:
	ASCII(.ascii	"thread stopped...\012\000" )
	.space	1
.LC111:
	ASCII(.ascii	"Thread killed!\012\000" )
.LC112:
	ASCII(.ascii	"ERROR:ReadNum = %d larger than VFMW_MAX_RESERVE_NUM" )
	ASCII(.ascii	"(%d)\012\000" )
	.space	3
.LC113:
	ASCII(.ascii	"%s,%d,specialFrmRec[%d]->PhyAddr = 0x%x,VirAddr = 0" )
	ASCII(.ascii	"x%x,Length = %d\012\000" )
.LC114:
	ASCII(.ascii	"pSpecialFrmInfo = null!!!\000" )
	.space	2
.LC115:
	ASCII(.ascii	"ChanID out of range!\000" )
	.space	3
.LC116:
	ASCII(.ascii	"SetVdecExtra ERROR: extra_ref = %d is out of [ -500" )
	ASCII(.ascii	"0, 5000 ]!\012\000" )
	.space	1
.LC117:
	ASCII(.ascii	"SetVdecExtra ERROR: extra_disp = %d is out of [ -50" )
	ASCII(.ascii	"00, 5000 ]!\012\000" )
.LC118:
	ASCII(.ascii	"%s 1 The channel %d is not active\012\000" )
	.space	1
.LC119:
	ASCII(.ascii	"The channel %d is deced\012\000" )
	.space	3
.LC120:
	ASCII(.ascii	"The pDecParam of channel %d is NULL\012\000" )
	.space	3
.LC121:
	ASCII(.ascii	"%s 2 The channel %d is not active\012\000" )
	.space	1
.LC122:
	ASCII(.ascii	"Chan %d Report Last Frame, Value=%d, LastFrameState" )
	ASCII(.ascii	"=%d\012\000" )
.LC123:
	ASCII(.ascii	"Last frame output failed!\012\000" )
	.space	1
.LC124:
	ASCII(.ascii	"Last frame output successed!\012\000" )
	.space	2
.LC125:
	ASCII(.ascii	"Last frame already output, image id: %d!\012\000" )
	.space	2
.LC126:
	ASCII(.ascii	"line %d can not know YuvFormat = %d\012\000" )
	.space	3
.LC127:
	ASCII(.ascii	"userdec frame(phy=%#x) address error!\012\000" )
	.space	1
.LC128:
	ASCII(.ascii	"can not map vir addr for usrdat frame(cbphy=%#x)\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC129:
	ASCII(.ascii	"line: %d, pLogic is Null!\012\000" )
	.space	1
.LC130:
	ASCII(.ascii	"eYuvFormat %d is not support!!\012\000" )
.LC131:
	ASCII(.ascii	"pUsrdecFrame == NULL\000" )
	.space	3
.LC132:
	ASCII(.ascii	"VCTRL_PutUsrdecFrame: VidStd is %d\012\000" )
.LC133:
	ASCII(.ascii	"VCTRL_PutUsrdecFrame return err\012\000" )
	.space	3
.LC134:
	ASCII(.ascii	"L:%d ChanID out of range\012\000" )
	.space	2
.LC135:
	ASCII(.ascii	"L:%d This Chan has not been created!!!\012\000" )
.LC136:
	ASCII(.ascii	"L:%d WARNING: vidio std not setted!\012\000" )
	.space	3
.LC137:
	ASCII(.ascii	"WARNING: vidio std not setted!\000" )
	.space	1
.LC138:
	ASCII(.ascii	"This Chan has not been created or has been destroye" )
	ASCII(.ascii	"d !\000" )
	.space	1
.LC139:
	ASCII(.ascii	"VCTRL_StopChanWithCheck fail!\012\000" )
	.space	1
.LC140:
	ASCII(.ascii	"---------------------------------------------------" )
	ASCII(.ascii	"----------------------\012\000" )
	.space	1
.LC141:
	ASCII(.ascii	"line %d VCTRL_GetChanMemSizeWithOption failed!\012\000" )
.LC142:
	ASCII(.ascii	"L: %d : part we can NOT alloc buffer for channel co" )
	ASCII(.ascii	"ntext\012\000" )
	.space	2
.LC143:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel context\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC144:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel vdh\012\000" )
	.space	2
.LC145:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel vdh, %#x <" )
	ASCII(.ascii	" %#x\012\000" )
	.space	3
.LC146:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel scd\012\000" )
	.space	2
.LC147:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for SCD\012\000" )
	.space	2
.LC148:
	ASCII(.ascii	"can NOT enable FSP instance\012\000" )
	.space	3
.LC149:
	ASCII(.ascii	"vfmw have max(%d) channels opened, can NOT create m" )
	ASCII(.ascii	"ore channels\012\000" )
	.space	3
.LC150:
	ASCII(.ascii	"VCTRL_CreateChan eCapLevel = %d error! Try to use V" )
	ASCII(.ascii	"CTRL_CreateChanWithOption(CAP_LEVEL_USER_DEFINE_WIT" )
	ASCII(.ascii	"H_OPTION, ...)\012\000" )
	.space	2
.LC151:
	ASCII(.ascii	"VCTRL_GetChanMemSize ERROR\012\000" )
.LC152:
	ASCII(.ascii	"channel memory have been allocated outside, but the" )
	ASCII(.ascii	" mem-info is not correct!\012\000" )
	.space	2
.LC153:
	ASCII(.ascii	"mem-info(phy, vir, size) = (%#x, %x, %d)\012\000" )
	.space	2
.LC154:
	ASCII(.ascii	"this channel totally need %d bytes memory\012\000" )
	.space	1
.LC155:
	ASCII(.ascii	"VCTRL_CheckChanWH return err.\012\000" )
	.space	1
.LC156:
	ASCII(.ascii	"VFMW didn't support std %d.\012\000" )
	.space	3
.LC157:
	ASCII(.ascii	"VCTRL_ConfigChan with same config paramater.\012\000" )
	.space	2
.LC158:
	ASCII(.ascii	"can NOT VCTRL_PartitionMemForStdUser\012\000" )
	.space	2
.LC159:
	ASCII(.ascii	"chan %d: new ChanErrThr = %d\012\000" )
	.space	2
.LC160:
	ASCII(.ascii	"chan %d: new DecOrderOutput = %d\012\000" )
	.space	2
.LC161:
	ASCII(.ascii	"chan %d: new DecMode = %d\012\000" )
	.space	1
.LC162:
	ASCII(.ascii	"chan %d: GetChanCfg failed!\012\000" )
	.space	3
.LC163:
	ASCII(.ascii	"chan %d: new StrmOFThr = %d\012\000" )
	.space	3
.LC164:
	ASCII(.ascii	"USE_FF_APT_EN = %d\012\000" )
.LC165:
	ASCII(.ascii	"USE_FF_APT_EN can NOT be set to %d, must be 0 or 1\012" )
	ASCII(.ascii	"\000" )
.LC166:
	ASCII(.ascii	"ResetCreateNewSave = %d\012\000" )
	.space	3
.LC167:
	ASCII(.ascii	"ResetCreateNewSave can NOT be set to %d, must be 0 " )
	ASCII(.ascii	"or 1\012\000" )
	.space	3
.LC168:
	ASCII(.ascii	"NO SUCH CHANNEL : %d\012\000" )
	.space	2
.LC169:
	ASCII(.ascii	"Pay attention to  CHANNEL : %d\012\000" )
.LC170:
	ASCII(.ascii	"print memory tracer...\012\000" )
.LC171:
	ASCII(.ascii	"OK create file '%s' for channel %d raw stream save\012" )
	ASCII(.ascii	"\000" )
.LC172:
	ASCII(.ascii	"failed create file '%s' for raw stream save!\012\000" )
	.space	2
.LC173:
	ASCII(.ascii	">>>> close raw stream saving >>>>\012\000" )
	.space	1
.LC174:
	ASCII(.ascii	"OK create file '%s' for channel %d seg stream save\012" )
	ASCII(.ascii	"\000" )
.LC175:
	ASCII(.ascii	"failed create file '%s' for seg stream save!\012\000" )
	.space	2
.LC176:
	ASCII(.ascii	">>>> close seg stream saving >>>>\012\000" )
	.space	1
.LC177:
	ASCII(.ascii	"Failed Alloc VFMW_BigTile1d_YUV for yuv save!\012\000" )
	.space	1
.LC178:
	ASCII(.ascii	"OK create file '%s' for chan %d yuv save.(crop %d)\012" )
	ASCII(.ascii	"\000" )
.LC179:
	ASCII(.ascii	"failed create file '%s' for yuv save!\012\000" )
	.space	1
.LC180:
	ASCII(.ascii	"OK close yuv save.\012\000" )
.LC181:
	ASCII(.ascii	"address '0x%x' is not valid!\012\000" )
	.space	2
.LC182:
	ASCII(.ascii	"2d image has been saved to '%s'\012\000" )
	.space	3
.LC183:
	ASCII(.ascii	"open file '%s' fail!\012\000" )
	.space	2
.LC184:
	ASCII(.ascii	"1d image has been saved to '%s'\012\000" )
	.space	3
.LC185:
	ASCII(.ascii	"OK create file '%s' for channel %d metadata stream " )
	ASCII(.ascii	"save\012\000" )
	.space	3
.LC186:
	ASCII(.ascii	"failed create file '%s' for metadata stream save!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC187:
	ASCII(.ascii	">>>> close metadata stream saving >>>>\012\000" )
.LC188:
	ASCII(.ascii	"OK create file '%s' for channel %d metadata_reorder" )
	ASCII(.ascii	" stream save\012\000" )
	.space	3
.LC189:
	ASCII(.ascii	"failed create file '%s' for metadata_reorder stream" )
	ASCII(.ascii	" save!\012\000" )
	.space	1
.LC190:
	ASCII(.ascii	"%s/vfmw_el_save_%d.dat\000" )
	.space	1
.LC191:
	ASCII(.ascii	"OK create file '%s' for channel %d el stream save\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC192:
	ASCII(.ascii	"failed create file '%s' for el stream save!\012\000" )
	.space	3
.LC193:
	ASCII(.ascii	">>>> close el stream saving >>>>\012\000" )
	.space	2
.LC194:
	ASCII(.ascii	"stop chan %d: VCTRL_StopChan return %d\012\000" )
.LC195:
	ASCII(.ascii	"reset chan %d: VCTRL_ResetChan return %d\012\000" )
	.space	2
.LC196:
	ASCII(.ascii	"start chan %d: VCTRL_StartChan return %d\012\000" )
	.space	2
.LC197:
	ASCII(.ascii	"------ dec task delay %d ms ------\012\000" )
.LC198:
	ASCII(.ascii	"------ dnr active per %d ms ------\012\000" )
.LC199:
	ASCII(.ascii	"------ dec stop = %d ------\012\000" )
	.space	3
.LC200:
	ASCII(.ascii	"------ g_TraceCtrl = %x ------\012\000" )
.LC201:
	ASCII(.ascii	"------ g_TraceBsPeriod = %x ------\012\000" )
.LC202:
	ASCII(.ascii	"------ g_TraceFramePeriod = %x ------\012\000" )
	.space	1
.LC203:
	ASCII(.ascii	"------ g_TraceImgPeriod = %x ------\012\000" )
	.space	3
.LC204:
	ASCII(.ascii	"The channel number is to Large.\012\000" )
	.space	3
.LC205:
	ASCII(.ascii	"------ s_pstVfmwChan[%d]->stSynExtraData.s32ModuleL" )
	ASCII(.ascii	"owlyEnable = 1 ------\012\000" )
	.space	2
.LC206:
	ASCII(.ascii	"------ Set SCD[%d] to work  ------\012\000" )
.LC207:
	ASCII(.ascii	"unmask vfmw mmu err int!\012\000" )
	.space	2
.LC208:
	ASCII(.ascii	"mask vfmw mmu err int!\012\000" )
.LC209:
	ASCII(.ascii	"vdm_ctrl.c,L%d: pOption->s32KeepBS error\012\000" )
	.space	2
.LC210:
	ASCII(.ascii	"vdm_ctrl.c,L%d: %s\012\000" )
.LC211:
	ASCII(.ascii	"This Chan has not been created or has been destroye" )
	ASCII(.ascii	"d !!!\000" )
	.space	3
.LC212:
	ASCII(.ascii	"can NOT stop a not-opened Chan\000" )
	.space	1
.LC213:
	ASCII(.ascii	"WARNNING: dsp in lowdelay state, we can not support" )
	ASCII(.ascii	"ed  avs in the same time!!!\012\000" )
.LC214:
	ASCII(.ascii	"WARNNING: dsp in avs state, we can not supported  l" )
	ASCII(.ascii	"owdelay in the same time!!!\012\000" )
.LC215:
	ASCII(.ascii	"DFS, request frame store fail!, chanid: %d\012\000" )
.LC216:
	ASCII(.ascii	"DFS, set decoding thread state to success!\012 \000" )
	.space	3
.LC217:
	ASCII(.ascii	"%s set chan %d ctrl state CHAN_FRAME_STORE_ALLOC fa" )
	ASCII(.ascii	"iled!\012\000" )
	.space	2
.LC218:
	ASCII(.ascii	"VFMW: WriteBigTileYuv %dx%d...\012\000" )
.LC219:
	ASCII(.ascii	"call VCTRL_GetChanImage, ChanID = %d,\012\000" )
	.space	1
.LC220:
	ASCII(.ascii	"%s,%d,ImgOutputEn = %d, should not be output!\012\000" )
	.space	1
.LC221:
	ASCII(.ascii	"%s: view_0 id(%d) > view_1 id(%d), error!\012\000" )
	.space	1
.LC222:
	ASCII(.ascii	"%s: continuous view0 id(%d)!\012\000" )
	.space	2
.LC223:
	ASCII(.ascii	"%s: continuous view1 id(%d)!\012\000" )
	.bss
	.align	3
.LANCHOR0 = . + 0
.LANCHOR2 = . + 8184
.LANCHOR4 = . + 16368
	.type	s_pstVfmwChan, %object
	.size	s_pstVfmwChan, 64
s_pstVfmwChan:
	.space	64
	.type	s_VfmwCTrl, %object
	.size	s_VfmwCTrl, 272
s_VfmwCTrl:
	.space	272
	.type	s_StreamIntf, %object
	.size	s_StreamIntf, 192
s_StreamIntf:
	.space	192
	.type	FspInstInfo.54852, %object
	.size	FspInstInfo.54852, 3212
FspInstInfo.54852:
	.space	3212
	.type	g_VdmCharacter, %object
	.size	g_VdmCharacter, 4
g_VdmCharacter:
	.space	4
	.type	last_rec_pos_time.55136, %object
	.size	last_rec_pos_time.55136, 64
last_rec_pos_time.55136:
	.space	64
	.type	extra_ref, %object
	.size	extra_ref, 4
extra_ref:
	.space	4
	.type	extra_disp, %object
	.size	extra_disp, 4
extra_disp:
	.space	4
	.type	MemArrange.55346, %object
	.size	MemArrange.55346, 1272
MemArrange.55346:
	.space	1272
	.type	MemArrange.55380, %object
	.size	MemArrange.55380, 1272
MemArrange.55380:
	.space	1272
	.type	cmdline, %object
	.size	cmdline, 1024
cmdline:
	.space	1024
	.type	extra_cmd, %object
	.size	extra_cmd, 4
extra_cmd:
	.space	4
	.type	g_allow_B_before_P, %object
	.size	g_allow_B_before_P, 1
g_allow_B_before_P:
	.space	1
	.type	g_not_allow_H264FullPictureRepair_flag, %object
	.size	g_not_allow_H264FullPictureRepair_flag, 1
g_not_allow_H264FullPictureRepair_flag:
	.space	1
	.type	g_apple_airplay_mirrorinig_flag, %object
	.size	g_apple_airplay_mirrorinig_flag, 1
g_apple_airplay_mirrorinig_flag:
	.space	1
	.space	1
	.type	g_event_report, %object
	.size	g_event_report, 4
g_event_report:
	.space	4
	.type	g_DSPState, %object
	.size	g_DSPState, 64
g_DSPState:
	.space	64
	.type	last_rec_pos_time.55485, %object
	.size	last_rec_pos_time.55485, 64
last_rec_pos_time.55485:
	.space	64
	.type	last_rec_pos_decparam_time.55486, %object
	.size	last_rec_pos_decparam_time.55486, 64
last_rec_pos_decparam_time.55486:
	.space	64
	.type	ChanPriTab.55446, %object
	.size	ChanPriTab.55446, 64
ChanPriTab.55446:
	.space	64
	.type	g_SleepTime, %object
	.size	g_SleepTime, 4
g_SleepTime:
	.space	4
	.type	g_StopDec, %object
	.size	g_StopDec, 4
g_StopDec:
	.space	4
	.type	g_VfmwChanStat, %object
	.size	g_VfmwChanStat, 4800
g_VfmwChanStat:
	.space	4800
	.type	AcceleratorCharacter, %object
	.size	AcceleratorCharacter, 4
AcceleratorCharacter:
	.space	4
	.type	PrioArray.54952, %object
	.size	PrioArray.54952, 64
PrioArray.54952:
	.space	64
	.type	g_ResetCreateNewSave, %object
	.size	g_ResetCreateNewSave, 4
g_ResetCreateNewSave:
	.space	4
	.type	vfmw_save_str_file, %object
	.size	vfmw_save_str_file, 128
vfmw_save_str_file:
	.space	128
	.type	pOption, %object
	.size	pOption, 4
pOption:
	.space	4
	.type	s_ChanControlState, %object
	.size	s_ChanControlState, 64
s_ChanControlState:
	.space	64
	.type	Capability.54996, %object
	.size	Capability.54996, 148
Capability.54996:
	.space	148
	.type	PrioArray.54995, %object
	.size	PrioArray.54995, 64
PrioArray.54995:
	.space	64
	.type	g_CurProcChan, %object
	.size	g_CurProcChan, 4
g_CurProcChan:
	.space	4
	.type	save_file_cnt.55610, %object
	.size	save_file_cnt.55610, 4
save_file_cnt.55610:
	.space	4
	.type	save_str_chan_num, %object
	.size	save_str_chan_num, 4
save_str_chan_num:
	.space	4
	.type	vfmw_save_seg_file, %object
	.size	vfmw_save_seg_file, 128
vfmw_save_seg_file:
	.space	128
	.type	save_file_cnt.55613, %object
	.size	save_file_cnt.55613, 4
save_file_cnt.55613:
	.space	4
	.type	save_seg_chan_num, %object
	.size	save_seg_chan_num, 4
save_seg_chan_num:
	.space	4
	.type	vfmw_save_yuv_file, %object
	.size	vfmw_save_yuv_file, 128
vfmw_save_yuv_file:
	.space	128
	.type	save_file_cnt.55618, %object
	.size	save_file_cnt.55618, 4
save_file_cnt.55618:
	.space	4
	.type	MemRecord_YUV, %object
	.size	MemRecord_YUV, 24
MemRecord_YUV:
	.space	24
	.type	pVl, %object
	.size	pVl, 4
pVl:
	.space	4
	.type	pUl, %object
	.size	pUl, 4
pUl:
	.space	4
	.type	pY, %object
	.size	pY, 4
pY:
	.space	4
	.type	save_yuv_with_crop, %object
	.size	save_yuv_with_crop, 4
save_yuv_with_crop:
	.space	4
	.type	vfmw_save_metadata_file, %object
	.size	vfmw_save_metadata_file, 128
vfmw_save_metadata_file:
	.space	128
	.type	save_file_cnt.55631, %object
	.size	save_file_cnt.55631, 4
save_file_cnt.55631:
	.space	4
	.type	save_metadata_chan_num, %object
	.size	save_metadata_chan_num, 4
save_metadata_chan_num:
	.space	4
	.type	vfmw_save_metadata_reorder_file, %object
	.size	vfmw_save_metadata_reorder_file, 128
vfmw_save_metadata_reorder_file:
	.space	128
	.type	save_file_cnt.55634, %object
	.size	save_file_cnt.55634, 4
save_file_cnt.55634:
	.space	4
	.type	save_metadata_reorder_chan_num, %object
	.size	save_metadata_reorder_chan_num, 4
save_metadata_reorder_chan_num:
	.space	4
	.type	vfmw_save_el_file, %object
	.size	vfmw_save_el_file, 128
vfmw_save_el_file:
	.space	128
	.type	save_file_cnt.55637, %object
	.size	save_file_cnt.55637, 4
save_file_cnt.55637:
	.space	4
	.type	save_el_chan_num, %object
	.size	save_el_chan_num, 4
save_el_chan_num:
	.space	4
	.type	g_DnrSleepTime, %object
	.size	g_DnrSleepTime, 4
g_DnrSleepTime:
	.space	4
	.type	g_SCDID, %object
	.size	g_SCDID, 4
g_SCDID:
	.space	4
	.type	last_rec_pos_time.55126, %object
	.size	last_rec_pos_time.55126, 64
last_rec_pos_time.55126:
	.space	64
	.type	g_EsBufValidFlag, %object
	.size	g_EsBufValidFlag, 64
g_EsBufValidFlag:
	.space	64
	.type	g_DnrIrqHandle, %object
	.size	g_DnrIrqHandle, 4
g_DnrIrqHandle:
	.space	4
	.type	g_VdecIrqHandle, %object
	.size	g_VdecIrqHandle, 4
g_VdecIrqHandle:
	.space	4
	.type	g_u32DFSMaxAllocTime, %object
	.size	g_u32DFSMaxAllocTime, 4
g_u32DFSMaxAllocTime:
	.space	4
	.type	g_eVdmVersion, %object
	.size	g_eVdmVersion, 4
g_eVdmVersion:
	.space	4
	.type	g_VfmwGlobalStat, %object
	.size	g_VfmwGlobalStat, 96
g_VfmwGlobalStat:
	.space	96
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
