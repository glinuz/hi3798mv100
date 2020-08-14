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
	stmfd	sp!, {r4, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #12)
	sub	sp, sp, #12
	cmp	r0, #15
	mov	r4, r3
	bhi	.L13
	ldr	lr, .L17
	ldr	ip, [lr, r0, asl #2]
	cmp	ip, #0
	beq	.L14
	ldr	r3, [ip, #36]
	cmp	r3, #0
	beq	.L6
	cmp	r3, #1
	bne	.L15
	ldr	r3, [lr, #220]
	cmp	r3, #0
	beq	.L16
.L9:
	sub	sp, fp, #16
	ldmia	sp, {r4, fp, sp, lr}
	bx	r3
.L15:
	ldr	r1, .L17+4
	mov	r0, #0
	str	r4, [sp]
	ldr	r2, .L17+8
	ldr	ip, [r1, #68]
	ldr	r1, .L17+12
	blx	ip
.L3:
	mvn	r0, #0
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L6:
	ldr	r3, [lr, #216]
	cmp	r3, #0
	bne	.L9
	ldr	r2, .L17+4
	mov	r0, r3
	ldr	r1, .L17+16
	ldr	r3, [r2, #68]
	blx	r3
	b	.L3
.L16:
	ldr	r2, .L17+4
	mov	r0, r3
	ldr	r1, .L17+20
	ldr	r3, [r2, #68]
	blx	r3
	b	.L3
.L13:
	ldr	ip, .L17+4
	movw	r2, #805
	ldr	r3, .L17+24
	mov	r0, #0
	ldr	r1, .L17+28
	ldr	ip, [ip, #68]
	blx	ip
	b	.L3
.L14:
	ldr	lr, .L17+4
	mov	r0, ip
	ldr	r3, .L17+32
	movw	r2, #806
	ldr	r1, .L17+28
	ldr	ip, [lr, #68]
	blx	ip
	b	.L3
.L18:
	.align	2
.L17:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r5, .L33
	ldr	r3, [r5, #68]
	cmp	r3, #1
	beq	.L24
	ldr	r6, .L33+4
	mov	r0, #10
	mov	r4, #49
	ldr	r3, [r6, #120]
	blx	r3
	b	.L25
.L32:
	ldr	r3, [r6, #120]
	blx	r3
	subs	r4, r4, #1
	beq	.L31
.L25:
	ldr	r3, [r5, #68]
	mov	r0, #10
	cmp	r3, #1
	bne	.L32
.L24:
	mov	r0, #0
.L23:
	mov	r3, #0
	str	r3, [r5, #72]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L31:
	mvn	r0, #0
	b	.L23
.L34:
	.align	2
.L33:
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
	ldr	r5, .L47
	ldr	r3, [r5, #68]
	cmp	r3, #2
	beq	.L38
	ldr	r6, .L47+4
	mov	r0, #10
	mov	r4, #49
	ldr	r3, [r6, #120]
	blx	r3
	b	.L39
.L46:
	ldr	r3, [r6, #120]
	blx	r3
	subs	r4, r4, #1
	beq	.L45
.L39:
	ldr	r3, [r5, #68]
	mov	r0, #10
	cmp	r3, #2
	bne	.L46
.L38:
	mov	r0, #0
.L37:
	mov	r3, #0
	str	r3, [r5, #72]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L45:
	mvn	r0, #0
	b	.L37
.L48:
	.align	2
.L47:
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
	ldr	ip, .L50
	movw	r2, #1149
	ldr	r3, .L50+4
	mov	r0, #0
	ldr	r1, .L50+8
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L51:
	.align	2
.L50:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_SetSendStreamAgainFlag.part.2, .-VCTRL_SetSendStreamAgainFlag.part.2
	.align	2
	.type	VCTRL_SetLastFrameState.part.6, %function
VCTRL_SetLastFrameState.part.6:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	ip, .L53
	movw	r2, #8675
	ldr	r3, .L53+4
	mov	r0, #0
	ldr	r1, .L53+8
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L54:
	.align	2
.L53:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_SetLastFrameState.part.6, .-VCTRL_SetLastFrameState.part.6
	.align	2
	.global	VCTRL_RegisterSoftDecoder
	.type	VCTRL_RegisterSoftDecoder, %function
VCTRL_RegisterSoftDecoder:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	mov	r1, r0
	ldr	r0, [r0]
	cmp	r0, #0
	beq	.L64
	ldr	r0, [r1, #4]
	cmp	r0, #0
	beq	.L65
	ldr	r0, [r1, #8]
	cmp	r0, #0
	beq	.L66
	ldr	r0, [r1, #12]
	cmp	r0, #0
	beq	.L67
	ldr	r0, [r1, #16]
	cmp	r0, #0
	beq	.L68
	ldr	r0, [r1, #20]
	cmp	r0, #0
	beq	.L69
	ldr	r0, [r1, #24]
	ldr	r3, .L71
	cmp	r0, #0
	beq	.L70
	mov	r2, #28
	ldr	r3, [r3, #52]
	ldr	r0, .L71+4
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L70:
	ldr	r2, .L71+8
	ldr	r1, .L71+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L69:
	ldr	r3, .L71
	ldr	r2, .L71+16
	ldr	r1, .L71+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L68:
	ldr	r3, .L71
	ldr	r2, .L71+20
	ldr	r1, .L71+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L67:
	ldr	r3, .L71
	ldr	r2, .L71+24
	ldr	r1, .L71+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L66:
	ldr	r3, .L71
	ldr	r2, .L71+28
	ldr	r1, .L71+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L65:
	ldr	r3, .L71
	ldr	r2, .L71+32
	ldr	r1, .L71+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L64:
	ldr	r3, .L71
	ldr	r2, .L71+36
	ldr	r1, .L71+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L72:
	.align	2
.L71:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+240
	.word	.LC14
	.word	.LC8
	.word	.LC13
	.word	.LC12
	.word	.LC11
	.word	.LC10
	.word	.LC9
	.word	.LC7
	UNWIND(.fnend)
	.size	VCTRL_RegisterSoftDecoder, .-VCTRL_RegisterSoftDecoder
	.align	2
	.global	VCTRL_UnRegisterSoftDecoder
	.type	VCTRL_UnRegisterSoftDecoder, %function
VCTRL_UnRegisterSoftDecoder:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L74
	mov	r2, #28
	mov	r1, #0
	ldr	r0, .L74+4
	ldr	r3, [r3, #48]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L75:
	.align	2
.L74:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+240
	UNWIND(.fnend)
	.size	VCTRL_UnRegisterSoftDecoder, .-VCTRL_UnRegisterSoftDecoder
	.align	2
	.global	VCTRL_Suspend
	.type	VCTRL_Suspend, %function
VCTRL_Suspend:
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
	ldr	r5, .L120
	mov	r6, #0
	mov	r4, #32
	mov	r7, r6
	ldr	r3, [r5]
	blx	r3
	mov	r8, r0
	bl	VDM_ResetClock
	mov	r0, r6
	bl	SCDDRV_PrepareSleep
	mov	r0, r6
	bl	VDMDRV_PrepareSleep
	ldr	r3, [r5]
	blx	r3
	mov	r9, r0
.L84:
	mov	r0, #0
	bl	SCDDRV_GetSleepStage
	cmp	r0, #2
	bne	.L114
	mov	r0, #0
	bl	VDMDRV_GetSleepStage
	cmp	r0, #2
	bne	.L115
.L96:
	ldr	r4, .L120+4
	ldr	r3, [r4, #20]
	cmp	r3, #0
	beq	.L85
	mov	r0, #0
	blx	r3
.L86:
	ldr	r3, [r4, #28]
	cmp	r3, #0
	beq	.L87
	blx	r3
.L87:
	ldr	r3, [r4, #4]
	cmp	r3, #0
	beq	.L116
	mov	r0, #0
	blx	r3
	ldr	r3, .L120+8
	ldr	r2, [r3, #64]
	cmp	r2, #1
	beq	.L117
.L89:
	ldr	r3, [r5]
	blx	r3
	ldr	r3, [r5, #68]
	ldr	r1, .L120+12
	cmp	r8, r0
	rsbls	r2, r8, r0
	mvnhi	r8, r8
	addhi	r2, r8, r0
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L85:
	mov	r0, r3
	mov	r2, #396
	ldr	r3, [r5, #68]
	ldr	r1, .L120+16
	blx	r3
	b	.L86
.L116:
	mov	r0, r3
	movw	r2, #403
	ldr	r3, [r5, #68]
	ldr	r1, .L120+16
	blx	r3
	ldr	r3, .L120+8
	ldr	r2, [r3, #64]
	cmp	r2, #1
	bne	.L89
.L117:
	mov	r2, #2
	str	r2, [r3, #72]
	bl	VCTRL_StopTask.part.0
	b	.L89
.L114:
	mov	r0, #0
	mov	r10, r7
	bl	VDMDRV_GetSleepStage
	cmp	r0, #2
	bne	.L92
	mov	r6, #1
	mov	r3, r6
.L93:
	tst	r3, r10
	bne	.L96
	subs	r4, r4, #1
	beq	.L118
	ldr	r3, [r5, #120]
	mov	r0, #10
	blx	r3
	eor	r2, r6, #1
	eor	r3, r7, #1
	orrs	r3, r2, r3
	bne	.L84
	b	.L96
.L115:
	mov	r7, #1
	mov	r10, r7
.L92:
	mov	r3, r6
	b	.L93
.L118:
	ldr	r3, [r5]
	blx	r3
	ldr	ip, [r5, #68]
	str	r6, [sp]
	mov	r3, r7
	ldr	r1, .L120+20
	cmp	r9, r0
	rsbls	r2, r9, r0
	mvnhi	r9, r9
	addhi	r2, r9, r0
	mov	r0, #0
	blx	ip
	cmp	r7, #1
	beq	.L119
	mov	r2, #0
	ldr	r3, [r5, #68]
	mov	r0, r2
	ldr	r1, .L120+24
	blx	r3
	mov	r0, #0
	bl	SCDDRV_ForceSleep
	cmp	r6, #1
	beq	.L96
.L99:
	mov	r2, #0
	ldr	r3, [r5, #68]
	mov	r0, r2
	ldr	r1, .L120+28
	blx	r3
	mov	r0, #0
	bl	VDMDRV_ForceSleep
	b	.L96
.L119:
	cmp	r6, #1
	bne	.L99
	b	.L96
.L121:
	.align	2
.L120:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	.word	.LC17
	.word	.LC16
	.word	.LC18
	.word	.LC19
	.word	.LC15
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L131
	ldr	r3, [r5]
	blx	r3
	mov	r4, r0
	mov	r0, #0
	bl	SCDDRV_ExitSleep
	mov	r0, #0
	bl	VDMDRV_ExitSleep
	ldr	r3, .L131+4
	ldr	r3, [r3, #24]
	cmp	r3, #0
	beq	.L123
	blx	r3
.L123:
	ldr	r3, .L131+8
	ldr	r2, [r3, #64]
	cmp	r2, #1
	beq	.L130
.L124:
	ldr	r3, [r5]
	blx	r3
	ldr	ip, [r5, #68]
	ldr	r1, .L131+12
	cmp	r4, r0
	rsbls	r2, r4, r0
	mvnhi	r2, r4
	addhi	r2, r2, r0
	mov	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	ip
.L130:
	str	r2, [r3, #72]
	bl	VCTRL_StartTask.part.1
	b	.L124
.L132:
	.align	2
.L131:
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0
	.word	.LC20
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
	ldr	r3, .L136
	mov	r2, #192
	mov	r1, #0
	ldr	r0, .L136+4
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, .L136+8
	mvn	r1, #0
	add	r2, r3, #64
.L134:
	str	r1, [r3, #4]!
	cmp	r3, r2
	bne	.L134
	ldmfd	sp, {fp, sp, pc}
.L137:
	.align	2
.L136:
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
	ldr	r3, .L143
	mov	r0, #0
	ldr	r3, [r3, #128]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L144:
	.align	2
.L143:
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
	ldr	r3, .L146
	mov	r0, #0
	ldr	r3, [r3, #128]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L147:
	.align	2
.L146:
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
	bhi	.L155
	ldr	r3, .L158
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L156
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L157
	cmp	r1, #1
	strls	r1, [r3, #1216]
	movls	r0, #0
	movhi	r2, #0
	strhi	r2, [r3, #1216]
	mvnhi	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L157:
	ldr	ip, .L158+4
	movw	r2, #785
	ldr	r3, .L158+8
.L154:
	ldr	r1, .L158+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L155:
	ldr	ip, .L158+4
	movw	r2, #783
	ldr	r3, .L158+16
	b	.L154
.L156:
	ldr	ip, .L158+4
	mov	r0, r3
	mov	r2, #784
	ldr	r3, .L158+20
	ldr	r1, .L158+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L159:
	.align	2
.L158:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC21
	.word	.LC1
	.word	.LC6
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
	beq	.L168
	cmp	r0, #0
	beq	.L164
	cmp	r0, #1
	bne	.L169
	ldr	r3, .L170
	mov	r0, #0
	str	r1, [r3, #220]
	ldmfd	sp, {fp, sp, pc}
.L169:
	ldr	ip, .L170+4
	mov	r3, r0
	ldr	r2, .L170+8
	mov	r0, #0
	ldr	r1, .L170+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L164:
	ldr	r3, .L170
	str	r1, [r3, #216]
	ldmfd	sp, {fp, sp, pc}
.L168:
	ldr	ip, .L170+4
	mov	r0, r1
	ldr	r3, .L170+16
	movw	r2, #846
	ldr	r1, .L170+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L171:
	.align	2
.L170:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+20
	.word	.LC23
	.word	.LC22
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
	ldr	r3, .L174
	ldr	r2, [r3, #64]
	cmp	r2, #1
	bne	.L173
	mov	r2, #2
	str	r2, [r3, #72]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_StopTask.part.0
.L173:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L175:
	.align	2
.L174:
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
	ldr	r3, .L178
	ldr	r2, [r3, #64]
	cmp	r2, #1
	bne	.L177
	str	r2, [r3, #72]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_StartTask.part.1
.L177:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L179:
	.align	2
.L178:
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
	beq	.L184
	ldr	r4, .L186
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L185
	ldr	r6, .L186+4
	mov	r2, #12
	add	r0, r0, #152
	add	r4, r4, #268
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
.L185:
	ldr	ip, .L186+4
	mov	r2, #1136
	ldr	r3, .L186+8
	ldr	r1, .L186+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L184:
	ldr	ip, .L186+4
	mov	r0, r7
	ldr	r3, .L186+16
	movw	r2, #1135
	ldr	r1, .L186+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L187:
	.align	2
.L186:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	.word	.LC24
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
	ldr	r3, .L191
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L190
	add	r3, r3, #225280
	mov	r0, #0
	str	r1, [r3, #3300]
	ldmfd	sp, {fp, sp, pc}
.L190:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_SetSendStreamAgainFlag.part.2
.L192:
	.align	2
.L191:
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
	ldr	r3, .L197
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L196
	add	r0, r0, #225280
	ldr	r0, [r0, #3300]
	ldmfd	sp, {fp, sp, pc}
.L196:
	ldr	ip, .L197+4
	movw	r2, #1156
	ldr	r3, .L197+8
	ldr	r1, .L197+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L198:
	.align	2
.L197:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r5, .L205
	mov	r4, r0
	ldr	r1, [r5, r0, asl #2]
	cmp	r1, #0
	beq	.L203
	ldr	r6, .L205+4
	mov	r3, r0, asl #4
	sub	r3, r3, r0, asl #2
	add	r0, r5, #268
	add	r0, r0, r3
	add	r1, r1, #152
	ldr	r3, [r6, #60]
	mov	r2, #12
	blx	r3
	cmp	r0, #0
	bne	.L204
	ldr	r1, [r5, r4, asl #2]
	add	r0, r1, #152
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L204:
	ldr	ip, [r6, #68]
	mov	r3, r4
	movw	r2, #1172
	ldr	r1, .L205+8
	mov	r0, #0
	blx	ip
.L203:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L206:
	.align	2
.L205:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC25
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
	beq	.L211
	ldr	r3, .L213
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L212
	ldr	r3, .L213+4
	mov	r2, #12
	add	r0, r0, #176
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L212:
	ldr	ip, .L213+4
	movw	r2, #1185
	ldr	r3, .L213+8
	ldr	r1, .L213+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L211:
	ldr	ip, .L213+4
	mov	r0, r3
	mov	r2, #1184
	ldr	r3, .L213+16
	ldr	r1, .L213+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L214:
	.align	2
.L213:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	.word	.LC26
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
	ldr	r3, .L218
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r0, r0, #176
	ldmfd	sp, {fp, sp, pc}
.L219:
	.align	2
.L218:
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
	bhi	.L225
	ldr	r3, .L228
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L226
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L227
	add	r0, r0, #99328
	add	r0, r0, #560
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	GetSegStreamSize
.L227:
	ldr	ip, .L228+4
	movw	r2, #1226
	ldr	r3, .L228+8
	mov	r0, #0
	ldr	r1, .L228+12
	ldr	ip, [ip, #68]
	blx	ip
.L222:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L225:
	ldr	ip, .L228+4
	movw	r2, #1224
	ldr	r3, .L228+16
	mov	r0, #0
	ldr	r1, .L228+12
	ldr	ip, [ip, #68]
	blx	ip
	b	.L222
.L226:
	ldr	ip, .L228+4
	movw	r2, #1225
	ldr	r3, .L228+20
	ldr	r1, .L228+12
	ldr	ip, [ip, #68]
	blx	ip
	b	.L222
.L229:
	.align	2
.L228:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC28
	.word	.LC1
	.word	.LC6
	.word	.LC27
	UNWIND(.fnend)
	.size	VCTRL_GetStreamSize, .-VCTRL_GetStreamSize
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
	bhi	.L236
	cmp	r1, #0
	beq	.L237
	ldr	r5, .L239
	ldr	r0, [r5, r4, asl #2]
	cmp	r0, #0
	beq	.L238
	ldr	r3, .L239+4
	add	r0, r0, #1408
	mov	r2, #8
	add	r0, r0, #12
	ldr	r3, [r3, #52]
	blx	r3
	ldr	r2, [r5, r4, asl #2]
	mov	r0, #0
	ldr	r3, [r2, #1420]
	sub	r3, r3, #1
	cmp	r3, #1
	strhi	r0, [r2, #1420]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L236:
	ldr	ip, .L239+4
	movw	r2, #1238
	ldr	r3, .L239+8
	mov	r0, #0
	ldr	r1, .L239+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L238:
	ldr	ip, .L239+4
	movw	r2, #1240
	ldr	r3, .L239+16
	ldr	r1, .L239+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L237:
	ldr	ip, .L239+4
	movw	r2, #1239
	ldr	r3, .L239+20
	ldr	r1, .L239+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L240:
	.align	2
.L239:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC1
	.word	.LC2
	.word	.LC29
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
	ldr	r3, .L244
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r0, r0, #229376
	addne	r0, r0, #752
	ldmfd	sp, {fp, sp, pc}
.L245:
	.align	2
.L244:
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
	ldr	r3, .L249
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	addne	r0, r0, #164
	ldmfd	sp, {fp, sp, pc}
.L250:
	.align	2
.L249:
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
	ldr	r4, .L267
	cmp	r2, #31
	cmpgt	r1, #31
	mov	r7, r2
	mov	r8, r1
	mov	r9, r0
	movle	r2, #1
	movgt	r2, #0
	ldr	r5, [r4, r0, asl #2]
	ble	.L263
	ldr	r1, [fp, #4]
	cmp	r1, #0
	cmpne	r3, #0
	moveq	r6, #1
	movne	r6, #0
	beq	.L264
	ldr	r10, .L267+4
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
	strb	r6, [fp, #-72]
	strb	r6, [fp, #-70]
	strb	r6, [fp, #-69]
	strb	r2, [fp, #-73]
	bl	FSP_ConfigInstance
	subs	r7, r0, #0
	bne	.L255
	ldr	r3, [fp, #-88]
	mov	r0, r9
	ldr	r2, [fp, #4]
	mov	r1, r3
	sub	r3, fp, #80
	bl	FSP_ConfigFrameBuf
	subs	r6, r0, #0
	bne	.L265
	mov	r0, r9
	add	r1, r4, #460
	bl	FSP_GetInstInfo
	movw	r3, #461
	ldrsb	r3, [r4, r3]
	cmp	r3, #16
	movge	r3, #16
	cmp	r3, #2
	str	r3, [r5, #468]
	ble	.L266
	ldr	ip, [r4, #2644]
	mov	r2, #64
	ldr	r3, [r10, #48]
	mov	r1, r7
	add	r0, r5, #400
	str	ip, [r5, #464]
	blx	r3
	mov	r1, r7
	ldr	r3, [r10, #48]
	mov	r2, #64
	add	r0, r5, #336
	blx	r3
	ldr	r1, [r5, #468]
	cmp	r1, #0
	ble	.L253
	add	r4, r4, #460
	add	r5, r5, #396
	mov	r3, r7
.L260:
	ldr	r2, [r4, #20]
	add	r3, r3, #1
	cmp	r3, r1
	add	r4, r4, #68
	str	r2, [r5, #4]!
	bne	.L260
.L253:
	mov	r0, r7
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L264:
	ldr	r3, .L267+4
	mov	r0, r2
	ldr	r1, .L267+8
	mvn	r7, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L253
.L255:
	ldr	r3, [r10, #68]
	mov	r0, r6
	ldr	r1, .L267+12
	mvn	r7, #0
	blx	r3
	b	.L253
.L265:
	ldr	r3, [r10, #68]
	mov	r0, r7
	ldr	r1, .L267+16
	blx	r3
.L257:
	mvn	r7, #0
	b	.L253
.L266:
	ldr	ip, [r10, #68]
	mov	r0, r6
	movw	r3, #1491
	ldr	r2, .L267+20
	ldr	r1, .L267+24
	blx	ip
	b	.L257
.L263:
	ldr	ip, .L267+4
	mov	r3, r7
	mov	r2, r1
	mov	r0, #0
	ldr	r1, .L267+28
	mvn	r7, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L253
.L268:
	.align	2
.L267:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC31
	.word	.LC34
	.word	.LC32
	.word	.LANCHOR1+40
	.word	.LC33
	.word	.LC30
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
	.global	VCTRL_RlsAllFrameNode
	.type	VCTRL_RlsAllFrameNode, %function
VCTRL_RlsAllFrameNode:
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
	mov	r4, #0
	mov	r7, r0
	ldr	r8, .L279
	mov	r0, r4
	mvn	r6, #0
	b	.L273
.L272:
	add	r3, r9, r5
	add	r3, r3, #228352
	ldr	r2, [r3, #244]
	ldr	r1, [r3, #236]
	ldr	r0, [r3, #240]
	bl	MEM_DelMemRecord
	ldr	r3, [r8, r7, asl #2]
	add	r5, r3, r5
	add	r5, r5, #228352
	strb	r6, [r5, #248]
.L271:
	cmp	r4, #32
	beq	.L278
.L273:
	mov	r3, r4, asl #2
	mov	r2, r4, asl #4
	ldr	r9, [r8, r7, asl #2]
	add	r5, r3, r2
	add	r4, r4, #1
	add	r3, r9, r5
	add	r3, r3, #228352
	ldrb	r2, [r3, #248]	@ zero_extendqisi2
	cmp	r2, #255
	beq	.L271
	cmp	r2, #1
	bne	.L272
	ldr	r1, [r3, #236]
	ldr	r0, [r3, #240]
	bl	MEM_ReleaseMemBlock
	b	.L272
.L278:
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L280:
	.align	2
.L279:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_RlsAllFrameNode, .-VCTRL_RlsAllFrameNode
	.align	2
	.type	VCTRL_AllocMemPre, %function
VCTRL_AllocMemPre:
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
	ldr	r10, .L295
	mov	r7, r1
	mov	r9, r0
	ldr	lr, [r10, r0, asl #2]
	add	r1, lr, #225280
	ldr	r2, [lr, #208]
	ldr	r6, [lr, #204]
	ldr	r8, [r1, #3272]
	cmp	r6, #0
	cmpne	r2, #0
	movne	r1, #1
	moveq	r1, #0
	cmp	r7, #31
	movgt	r1, #0
	andle	r1, r1, #1
	cmp	r1, #0
	beq	.L282
	mov	r5, #0
	rsb	r3, r6, r2
	mov	r4, r5
	str	r3, [fp, #-48]
	mov	r0, r5
	b	.L286
.L283:
	cmp	r7, r5
	ble	.L285
	add	r4, r4, #1
	add	r6, r6, r8
	cmp	r4, #32
	beq	.L285
.L293:
	ldr	lr, [r10, r9, asl #2]
.L286:
	mov	r2, r4, asl #4
	cmp	r5, r7
	add	r2, r2, r4, lsl #2
	add	r2, lr, r2
	movge	r1, #0
	add	r2, r2, #228352
	movlt	r1, #1
	ldrb	lr, [r2, #248]	@ zero_extendqisi2
	cmp	lr, #255
	movne	lr, #0
	andeq	lr, r1, #1
	cmp	lr, #0
	beq	.L283
	ldr	r3, [fp, #-48]
	add	r1, r2, #232
	str	r6, [r2, #236]
	add	r5, r5, #1
	add	r0, r3, r6
	str	r8, [r2, #244]
	str	r0, [r2, #240]
	mov	r3, #0
	strb	r3, [r2, #248]
	mov	r2, r8
	ldr	r1, [r1, #4]
	add	r4, r4, #1
	bl	MEM_AddMemRecord
	cmp	r7, r5
	add	r6, r6, r8
	movle	r1, #0
	movgt	r1, #1
	cmp	r4, #32
	bne	.L293
.L285:
	cmp	r4, #32
	movne	r4, #0
	andeq	r4, r1, #1
	cmp	r4, #0
	bne	.L294
.L287:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L294:
	ldr	r4, .L295+4
	mov	r3, r5
	mov	r2, r7
	ldr	r1, .L295+8
	mov	r0, #31
	ldr	r5, [r4, #68]
	blx	r5
	mov	r0, r9
	bl	VCTRL_RlsAllFrameNode
	cmp	r0, #0
	mvneq	r0, #0
	beq	.L287
	ldr	r3, [r4, #68]
	mov	r2, r9
	ldr	r1, .L295+12
	mov	r0, #31
	blx	r3
	mvn	r0, #0
	b	.L287
.L282:
	ldr	ip, .L295+4
	mov	r3, r6
	str	r7, [sp]
	mov	r0, #31
	ldr	r1, .L295+16
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L287
.L296:
	.align	2
.L295:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC36
	.word	.LC37
	.word	.LC35
	UNWIND(.fnend)
	.size	VCTRL_AllocMemPre, .-VCTRL_AllocMemPre
	.align	2
	.type	VCTRL_AllocMemMMZ, %function
VCTRL_AllocMemMMZ:
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
	ldr	r10, .L316
	mov	r6, #0
	mov	r9, r0
	str	r1, [fp, #-48]
	mov	r8, r6
	ldr	r4, [r10, r0, asl #2]
	mov	r5, r6
	mov	r0, r6
	mov	ip, r6
	add	r2, r4, #225280
	ldr	r3, [r4, #1392]
	ldr	r2, [r2, #3272]
	add	r3, r3, r3, lsr #31
	str	r2, [fp, #-56]
	mov	r3, r3, asr #1
	str	r3, [fp, #-72]
	b	.L304
.L298:
	ldr	r3, [fp, #-48]
	cmp	r3, ip
	ble	.L303
	add	r5, r5, #1
	cmp	r5, #32
	beq	.L301
.L314:
	ldr	r4, [r10, r9, asl #2]
.L304:
	mov	r3, r5, asl #4
	mov	r7, r5, asl #2
	add	r6, r7, r3
	str	r3, [fp, #-52]
	add	r3, r4, r6
	add	r3, r3, #228352
	ldrb	r2, [r3, #248]	@ zero_extendqisi2
	ldr	r3, [fp, #-48]
	cmp	ip, r3
	movge	r3, #0
	movlt	r3, #1
	cmp	r2, #255
	movne	r3, #0
	cmp	r3, #0
	beq	.L298
	add	r6, r6, #228352
	mov	r1, #0
	add	r6, r6, #232
	str	r1, [sp]
	add	r4, r4, r6
	ldr	r2, [fp, #-56]
	ldr	r0, .L316+4
	mov	r3, r4
	str	ip, [fp, #-60]
	bl	MEM_AllocMemBlock
	ldr	r3, [fp, #-72]
	ldr	ip, [fp, #-60]
	cmp	r0, #0
	cmpne	r3, r8
	ble	.L299
	str	r5, [fp, #-64]
	mov	r4, r6
	ldr	r5, [fp, #-56]
	mov	r6, r3
	str	r7, [fp, #-68]
.L300:
	ldr	r3, .L316+8
	mov	r0, #2
	add	r8, r8, #1
	ldr	r3, [r3, #120]
	blx	r3
	ldr	ip, [r10, r9, asl #2]
	mov	r1, #0
	mov	r2, r5
	add	r7, ip, r4
	str	r1, [sp]
	ldr	r0, .L316+4
	mov	r3, r7
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	cmpne	r6, r8
	bgt	.L300
	mov	r4, r7
	ldr	ip, [fp, #-60]
	ldr	r5, [fp, #-64]
	ldr	r7, [fp, #-68]
.L299:
	cmp	r0, #0
	bne	.L301
	ldr	r2, [r4, #12]
	add	ip, ip, #1
	ldr	r1, [r4, #4]
	add	r5, r5, #1
	ldr	r0, [r4, #8]
	str	ip, [fp, #-60]
	str	ip, [fp, #-64]
	bl	MEM_AddMemRecord
	ldr	r3, [fp, #-52]
	cmp	r5, #32
	ldr	r2, [r10, r9, asl #2]
	add	r7, r7, r3
	ldr	ip, [fp, #-60]
	add	r7, r2, r7
	mov	r3, #1
	add	r7, r7, #228352
	strb	r3, [r7, #248]
	bne	.L314
.L301:
	ldr	r1, [fp, #-48]
	cmp	r1, ip
	ble	.L303
	ldr	r4, .L316+8
	mov	r3, ip
	str	r1, [sp]
	mov	r2, r9
	str	r5, [sp, #4]
	mov	r0, #31
	ldr	ip, [r4, #68]
	ldr	r1, .L316+12
	blx	ip
	mov	r0, r9
	bl	VCTRL_RlsAllFrameNode
	cmp	r0, #0
	mvneq	r0, #0
	bne	.L315
.L303:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L315:
	ldr	r3, [r4, #68]
	mov	r0, #31
	ldr	r1, .L316+16
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L317:
	.align	2
.L316:
	.word	.LANCHOR0
	.word	.LC38
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC39
	.word	.LC40
	UNWIND(.fnend)
	.size	VCTRL_AllocMemMMZ, .-VCTRL_AllocMemMMZ
	.align	2
	.global	VCTRL_FreeChanResource
	.type	VCTRL_FreeChanResource, %function
VCTRL_FreeChanResource:
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
	ldr	r3, [r1, #88]
	mov	r6, r1
	cmp	r3, #1
	beq	.L319
	ldr	r2, [r1, #12]
	ldr	r0, [r6, #8]
	ldr	r1, [r1, #4]
	bl	MEM_DelMemRecord
.L320:
	ldr	r2, [r6, #28]
	cmp	r2, #0
	bne	.L348
	ldr	r3, [r6, #64]
	cmp	r3, #1
	beq	.L349
.L325:
	ldr	r3, [r6, #68]
	cmp	r3, #1
	beq	.L350
.L326:
	ldr	r3, [r6, #72]
	cmp	r3, #1
	beq	.L351
.L327:
	ldr	r3, .L352
	ldr	r0, [r6, #36]
	ldr	r3, [r3, #164]
	blx	r3
.L328:
	ldr	r2, [r6, #44]
	ldr	r1, [r6, #36]
	ldr	r0, [r6, #40]
	bl	MEM_DelMemRecord
	add	r0, r6, #48
	bl	SM_DeletDSPCtxMem
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L351:
	ldr	r0, [r6, #40]
	cmp	r0, #0
	beq	.L327
	ldr	r1, [r6, #36]
	bl	MEM_ReleaseMemBlock
	b	.L328
.L319:
	ldr	r7, .L352
	mov	r5, #0
	b	.L323
.L322:
	ldr	r3, [r6, #92]
	mvn	r2, #0
	add	r3, r3, r5
	strb	r2, [r3, #16]
.L321:
	add	r5, r5, #20
	cmp	r5, #640
	beq	.L320
.L323:
	ldr	r4, [r6, #92]
	add	r4, r4, r5
	ldrb	r3, [r4, #16]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L321
	ldr	r2, [r4, #12]
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_DelMemRecord
	cmp	r0, #0
	beq	.L322
	ldr	r2, [r4, #8]
	mov	r0, #1
	ldr	r3, [r7, #68]
	ldr	r1, .L352+4
	blx	r3
	b	.L322
.L348:
	ldr	r1, [r6, #20]
	ldr	r0, [r6, #24]
	bl	MEM_DelMemRecord
	ldr	r3, [r6, #64]
	cmp	r3, #1
	bne	.L325
.L349:
	ldr	r0, [r6, #8]
	cmp	r0, #0
	beq	.L325
	ldr	r1, [r6, #4]
	bl	MEM_ReleaseMemBlock
	b	.L325
.L350:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	beq	.L326
	ldr	r1, [r6, #20]
	bl	MEM_ReleaseMemBlock
	b	.L326
.L353:
	.align	2
.L352:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC41
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
	movweq	r3, #57520
	movwne	r3, #28424
	movteq	r3, 176
	movtne	r3, 8
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
	bhi	.L362
	cmp	r1, #0
	beq	.L363
	ldr	r2, .L365
	ldr	ip, .L365+4
	ldr	r1, [r2, r0, asl #2]
	cmp	r1, #0
	beq	.L364
	add	r1, r1, #48
	mov	r0, r3
	mov	r2, #104
	ldr	r3, [ip, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L362:
	ldr	ip, .L365+4
	movw	r2, #2776
	ldr	r3, .L365+8
	mov	r0, #0
	ldr	r1, .L365+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L364:
	mov	r0, r1
	ldr	r3, .L365+16
	movw	r2, #2778
	ldr	r1, .L365+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L363:
	ldr	ip, .L365+4
	mov	r0, r1
	ldr	r3, .L365+20
	movw	r2, #2777
	ldr	r1, .L365+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L366:
	.align	2
.L365:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC1
	.word	.LC2
	.word	.LC42
	UNWIND(.fnend)
	.size	VCTRL_GetChanCfg, .-VCTRL_GetChanCfg
	.align	2
	.global	RefreshDecCore
	.type	RefreshDecCore, %function
RefreshDecCore:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 8
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
	ldr	r4, .L384
	mov	r5, r0
	ldr	r0, [r4, r0, asl #2]
	ldr	r3, [r0, #528]
	ldr	r2, [r0, #1452]
	cmp	r3, #4
	beq	.L369
	cmp	r3, #15
	moveq	r3, #2
	streq	r3, [r0, #1452]
	bne	.L383
.L371:
	cmp	r2, r3
	beq	.L375
	sub	r2, r2, #1
	cmp	r2, #4
	bhi	.L374
	ldr	r3, [r4, #244]
	cmp	r3, #0
	beq	.L374
	add	r0, r0, #229376
	str	r1, [fp, #-24]
	add	r0, r0, #760
	blx	r3
	ldr	r1, [fp, #-24]
.L374:
	ldr	r2, [r4, r5, asl #2]
	ldr	r0, .L384
	ldr	r3, [r2, #1452]
	sub	r3, r3, #1
	cmp	r3, #4
	bhi	.L375
	ldr	r3, [r0, #240]
	add	r0, r2, #229376
	ldr	ip, [r2, #40]
	cmp	r3, #0
	str	ip, [r0, #772]
	beq	.L377
	ldr	r2, [r2, #528]
	add	r0, r0, #760
	blx	r3
	cmp	r0, #0
	bne	.L377
.L375:
	mov	r0, #0
.L373:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L383:
	mov	r3, #0
	str	r3, [r0, #1452]
	b	.L371
.L369:
	mov	r3, #1
	str	r3, [r0, #1452]
	b	.L371
.L377:
	ldr	r3, .L384+4
	mov	r0, #0
	ldr	r1, .L384+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L373
.L385:
	.align	2
.L384:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC43
	UNWIND(.fnend)
	.size	RefreshDecCore, .-RefreshDecCore
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
	ldr	r3, .L405
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r3, #528]
	cmp	r2, #20
	beq	.L402
	ldr	r2, [r3, #48]
	ldr	r0, [r1]
	cmp	r2, r0
	bne	.L402
	ldr	ip, [r3, #64]
	ldr	r0, [r1, #16]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #60]
	ldr	r0, [r1, #12]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #68]
	ldr	r0, [r1, #20]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #72]
	ldr	r0, [r1, #24]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #76]
	ldr	r0, [r1, #28]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #80]
	ldr	r0, [r1, #32]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #100]
	ldr	r0, [r1, #52]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #112]
	ldr	r0, [r1, #64]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #108]
	ldr	r0, [r1, #60]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #104]
	ldr	r0, [r1, #56]
	cmp	ip, r0
	bne	.L402
	ldr	ip, [r3, #116]
	ldr	r0, [r1, #68]
	cmp	ip, r0
	bne	.L402
	cmp	r2, #1
	beq	.L404
	sub	r2, r2, #10
	cmp	r2, #2
	bhi	.L403
	ldr	r3, [r3, #52]
	ldr	r0, [r1, #4]
	subs	r0, r3, r0
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L402:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L403:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L404:
	ldr	r0, [r3, #52]
	ldr	r2, [r1, #4]
	cmp	r0, r2
	bne	.L402
	ldr	r3, [r3, #56]
	ldr	r0, [r1, #8]
	subs	r0, r3, r0
	mvnne	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L406:
	.align	2
.L405:
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
	ldr	r3, .L412
	ldr	r2, .L412+4
	ldr	r3, [r3, r0, asl #2]
	ldr	r2, [r2]
	ldr	r0, [r3, #1272]
	ldr	r3, [r3, #1276]
	cmp	r0, r2
	bgt	.L408
	ldr	r0, .L412+8
	ldr	r0, [r0]
	cmp	r3, r0
	ble	.L410
.L408:
	ldr	r3, [r1]
	cmp	r3, #19
	cmpne	r3, #13
	movne	r0, #1
	moveq	r0, #0
	bics	r3, r3, #16
	andne	r0, r0, #1
	moveq	r0, #0
	cmp	r0, #0
	ldmeqfd	sp, {fp, sp, pc}
	ldr	ip, .L412+12
	mov	r0, #0
	ldr	r3, .L412+8
	ldr	r1, .L412+16
	ldr	ip, [ip, #68]
	ldr	r3, [r3]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L410:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L413:
	.align	2
.L412:
	.word	.LANCHOR0
	.word	g_ChipMaxWidth
	.word	g_ChipMaxHeight
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC44
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
	ldr	r2, .L422
	ldr	r3, [r2, r0, asl #2]
	cmp	r3, #0
	beq	.L420
	ldr	r3, [r3, #528]
	sub	ip, r3, #10
	cmp	ip, #2
	bls	.L421
.L417:
	cmp	r3, #15
	cmpne	r3, #4
	beq	.L419
	bl	SM_SeekPts
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
.L421:
	ldr	r2, [r2, #2648]
	tst	r2, #1
	bne	.L417
.L419:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L420:
	ldr	ip, .L422+4
	mov	r0, r3
	movw	r2, #3410
	ldr	r3, .L422+8
	ldr	r1, .L422+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L423:
	.align	2
.L422:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	ldr	r3, .L428
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L427
	bl	SM_Reset
	adds	r0, r0, #0
	movne	r0, #1
	rsb	r0, r0, #0
	ldmfd	sp, {fp, sp, pc}
.L427:
	ldr	ip, .L428+4
	mov	r0, r3
	movw	r2, #3859
	ldr	r3, .L428+8
	ldr	r1, .L428+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L429:
	.align	2
.L428:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ReleaseStream, .-VCTRL_ReleaseStream
	.align	2
	.global	VCTRL_ReleaseChanImage
	.type	VCTRL_ReleaseChanImage, %function
VCTRL_ReleaseChanImage:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 504
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #508)
	sub	sp, sp, #508
	cmp	r0, #15
	mov	r4, r0
	mov	r5, r1
	bhi	.L458
	ldr	r8, .L467
	ldr	r0, [r8, r0, asl #2]
	cmp	r0, #0
	beq	.L459
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L460
	cmp	r1, #0
	beq	.L461
	ldr	r6, .L467+4
	mov	r2, r4
	ldr	r3, [r1, #196]
	mov	r0, #2
	ldr	r1, .L467+8
	ldr	ip, [r6, #68]
	blx	ip
	ldr	r3, [r6, #12]
	mov	r0, #4
	blx	r3
	ldr	r3, [r8, r4, asl #2]
	cmp	r3, #0
	beq	.L462
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L463
	ldr	r2, [r3, #1388]
	add	r0, r3, #229376
	cmp	r2, #1
	beq	.L464
.L438:
	add	r0, r0, #752
	mov	r1, r5
	bl	SYNTAX_ReleaseImage
	ldr	r3, [r5, #252]
	cmn	r3, #1
	mov	r7, r0
	beq	.L440
	ldr	r0, [r8, r4, asl #2]
	sub	r1, fp, #540
	str	r3, [fp, #-344]
	add	r0, r0, #229376
	add	r0, r0, #752
	bl	SYNTAX_ReleaseImage
	mov	r7, r0
.L440:
	mov	r0, #13
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L465
.L442:
	subs	r4, r7, #1
	mvnne	r4, #0
	cmp	r4, #0
	bne	.L466
.L444:
	ldr	r3, [r6, #16]
	mov	r0, #4
	blx	r3
.L432:
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L466:
	ldr	r3, [r6, #68]
	mov	r0, #0
	ldr	r2, .L467+12
	ldr	r1, .L467+16
	blx	r3
	b	.L444
.L465:
	ldr	r3, [r6]
	add	r8, r8, r4, lsl #2
	blx	r3
	ldr	r2, .L467+20
	ldr	r3, [r8, #2656]
	ldr	r2, [r2]
	rsb	r3, r3, r0
	mov	ip, r0
	cmp	r3, r2
	bls	.L442
	mov	r2, r4, asl #24
	movw	r1, #4455
	add	r2, r2, #851968
	ldr	r0, .L467+24
	cmp	r7, #1
	addeq	r2, r2, #1
	str	ip, [r8, #2656]
	bl	DBG_AddTrace
	b	.L442
.L464:
	ldr	r3, [r0, #712]
	cmp	r3, #1
	bne	.L438
	ldr	r1, [r5, #104]
	ldr	r3, [r0, #700]
	cmp	r1, r3
	bcc	.L438
	ldr	r2, [r0, #708]
	add	r3, r3, r2
	cmp	r1, r3
	bcs	.L438
	ldr	ip, [r8, #2652]
	ldr	r7, .L467
	cmp	ip, #0
	beq	.L439
	sub	r2, fp, #36
	mov	r3, #4
	mov	r0, r4
	str	r1, [r2, #-504]!
	mov	r1, #123
	blx	ip
	ldr	r3, [r7, r4, asl #2]
	add	r0, r3, #229376
.L439:
	ldr	r3, [r8, r4, asl #2]
	mov	r2, #0
	str	r2, [r0, #712]
	add	r0, r3, #229376
	b	.L438
.L460:
	ldr	ip, .L467+4
	movw	r2, #4338
	ldr	r3, .L467+28
.L456:
	ldr	r1, .L467+32
	mov	r0, #0
	ldr	ip, [ip, #68]
	mvn	r4, #0
	blx	ip
	b	.L432
.L463:
	ldr	r3, [r6, #68]
	movw	r2, #4357
	ldr	r1, .L467+36
	mov	r0, #0
.L457:
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, #4
	mvn	r4, #0
	blx	r3
	b	.L432
.L458:
	ldr	ip, .L467+4
	movw	r2, #4336
	ldr	r3, .L467+40
	b	.L456
.L461:
	ldr	ip, .L467+4
	mov	r0, r1
	ldr	r3, .L467+44
	movw	r2, #4339
	ldr	r1, .L467+32
	mvn	r4, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L432
.L459:
	ldr	ip, .L467+4
	movw	r2, #4337
	ldr	r3, .L467+48
	mvn	r4, #0
	ldr	r1, .L467+32
	ldr	ip, [ip, #68]
	blx	ip
	b	.L432
.L462:
	mov	r0, r3
	movw	r2, #4347
	ldr	r3, [r6, #68]
	ldr	r1, .L467+52
	b	.L457
.L468:
	.align	2
.L467:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC47
	.word	.LANCHOR1+96
	.word	.LC50
	.word	g_TraceImgPeriod
	.word	.LANCHOR1+72
	.word	.LC45
	.word	.LC1
	.word	.LC49
	.word	.LC6
	.word	.LC46
	.word	.LC2
	.word	.LC48
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
	bhi	.L470
	ldr	r3, .L474
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L470
	ldr	r2, [r3, #12]
	cmp	r2, #1
	beq	.L473
	ldr	ip, .L474+4
	mov	r3, r0
	movw	r2, #4507
	ldr	r1, .L474+8
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #20
	ldmfd	sp, {fp, sp, pc}
.L470:
	ldr	r3, .L474+4
	movw	r2, #4500
	ldr	r1, .L474+12
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #20
	ldmfd	sp, {fp, sp, pc}
.L473:
	ldr	r0, [r3, #528]
	ldmfd	sp, {fp, sp, pc}
.L475:
	.align	2
.L474:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC52
	.word	.LC51
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
	bhi	.L477
	ldr	r3, .L502
	ldr	r4, [r3, r0, asl #2]
	cmp	r4, #0
	beq	.L477
	ldr	r2, [r4, #12]
	cmp	r2, #1
	bne	.L498
	ldr	r2, [r4, #16]
	cmp	r2, #1
	bne	.L498
	ldr	r2, [r4, #528]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L498
.L482:
	.word	.L481
	.word	.L501
	.word	.L484
	.word	.L501
	.word	.L498
	.word	.L501
	.word	.L501
	.word	.L498
	.word	.L501
	.word	.L501
	.word	.L490
	.word	.L490
	.word	.L490
	.word	.L501
	.word	.L498
	.word	.L498
	.word	.L492
	.word	.L493
.L498:
	mov	r3, #0
.L479:
	mov	r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L490:
	ldr	r3, [r3, #2648]
	ands	r3, r3, #1
	beq	.L479
.L501:
	add	r3, r4, #229376
	ldr	r5, [r3, #764]
.L480:
	cmp	r5, #0
	beq	.L498
	ldr	r3, .L502+4
	ldr	r3, [r3]
	blx	r3
	mov	r3, r5
	str	r0, [r4, #44]
	mov	r0, r3
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L477:
	ldr	ip, .L502+4
	mov	r3, r0
	movw	r2, #4524
	ldr	r1, .L502+8
	mov	r0, #22
	ldr	ip, [ip, #68]
	blx	ip
	mov	r3, #0
	b	.L479
.L481:
	add	r3, r4, #229376
	ldr	r5, [r3, #1120]
	b	.L480
.L492:
	add	r3, r4, #229376
	ldr	r5, [r3, #988]
	b	.L480
.L484:
	add	r3, r4, #229376
	ldr	r5, [r3, #1796]
	b	.L480
.L493:
	add	r3, r4, #229376
	ldr	r5, [r3, #3072]
	b	.L480
.L503:
	.align	2
.L502:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC53
	UNWIND(.fnend)
	.size	VCTRL_GetDecParam, .-VCTRL_GetDecParam
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
	bhi	.L505
	ldr	r5, .L516
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L505
	ldr	r1, [r3, #12]
	cmp	r1, #1
	bne	.L508
	ldr	r6, [r3, #16]
	cmp	r6, #1
	beq	.L509
.L508:
	ldr	r3, .L516+4
	mov	r0, #22
	ldr	r1, .L516+8
	mov	r4, #0
	ldr	r3, [r3, #68]
	blx	r3
.L507:
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L505:
	ldr	ip, .L516+4
	mov	r3, r2
	ldr	r1, .L516+12
	movw	r2, #4650
	mov	r0, #22
	mov	r4, #0
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L509:
	str	r0, [fp, #-40]
	bl	VCTRL_GetDecParam
	ldr	r2, [fp, #-40]
	subs	r7, r0, #0
	beq	.L514
	ldr	r2, [r5, r2, asl #2]
	mov	r3, #0
	mov	r4, r3
	str	r3, [r2, #328]
	str	r3, [r2, #332]
	b	.L507
.L514:
	ldr	r8, [r5, r2, asl #2]
	ldr	r4, [r8, #328]
	cmp	r4, #0
	beq	.L515
	ldr	r3, .L516+4
	str	r2, [fp, #-40]
	ldr	r3, [r3]
	blx	r3
	ldr	r2, [fp, #-40]
	ldr	r3, [r5, r2, asl #2]
	ldr	r2, [r3, #332]
	cmp	r0, r2
	bls	.L512
	rsb	r0, r2, r0
	cmp	r0, #40
	movls	r4, r7
	bls	.L507
.L512:
	str	r7, [r3, #328]
	mov	r4, r6
	str	r7, [r3, #332]
	b	.L507
.L515:
	ldr	r3, .L516+4
	str	r6, [r8, #328]
	ldr	r3, [r3]
	blx	r3
	str	r0, [r8, #332]
	b	.L507
.L517:
	.align	2
.L516:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC54
	.word	.LC53
	UNWIND(.fnend)
	.size	VCTRL_ChanDecparamInValidFlag, .-VCTRL_ChanDecparamInValidFlag
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
	ldr	r4, .L547
	mov	r5, r0
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L544
	add	r0, r0, #229376
	ldr	r2, [r0, #752]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L537
.L522:
	.word	.L521
	.word	.L523
	.word	.L524
	.word	.L525
	.word	.L537
	.word	.L526
	.word	.L527
	.word	.L537
	.word	.L528
	.word	.L529
	.word	.L530
	.word	.L530
	.word	.L530
	.word	.L531
	.word	.L537
	.word	.L537
	.word	.L532
	.word	.L533
.L533:
	add	r0, r0, #760
	ldr	r4, .L547+4
	bl	HEVC_FlushOutputFrmFromDPB
	cmp	r0, #0
	bne	.L545
.L535:
	ldr	r3, [r4, #68]
	mov	r0, #32
	ldr	r1, .L547+8
	blx	r3
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L521:
	add	r0, r0, #760
	bl	H264_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	r0, [r4, r5, asl #2]
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	FlushDPB
	cmp	r0, #0
	bne	.L546
.L537:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L523:
	add	r0, r0, #760
	bl	VC1DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L524:
	add	r0, r0, #760
	bl	MPEG4DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L525:
	add	r0, r0, #760
	bl	MPEG2DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L526:
	add	r0, r0, #760
	bl	DIVX3DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L527:
	add	r0, r0, #760
	bl	AVSDEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L528:
	add	r0, r0, #760
	bl	REAL8DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L529:
	add	r0, r0, #760
	bl	REAL9DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L530:
	add	r0, r0, #760
	bl	VP6DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L531:
	add	r0, r0, #760
	bl	VP8DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L532:
	add	r0, r0, #760
	bl	MVC_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	r0, [r4, r5, asl #2]
	mvn	r1, #0
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	MVC_FlushDPB
	cmp	r0, #0
	beq	.L537
	ldr	ip, .L547+4
	movw	r3, #4961
	ldr	r2, .L547+12
	mov	r0, #22
	ldr	r1, .L547+16
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L545:
	movw	r2, #4944
	ldr	r1, .L547+20
	ldr	r3, [r4, #68]
	mov	r0, #0
	blx	r3
	b	.L535
.L546:
	ldr	r3, .L547+4
	movw	r2, #4934
	ldr	r1, .L547+20
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L544:
	ldr	ip, .L547+4
	movw	r2, #4908
	ldr	r3, .L547+24
	ldr	r1, .L547+28
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L548:
	.align	2
.L547:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC56
	.word	.LANCHOR1+120
	.word	.LC57
	.word	.LC55
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
	bhi	.L573
	ldr	r1, .L576
	ldr	r3, [r1, r0, asl #2]
	cmp	r3, #0
	beq	.L574
	ldr	r0, [r3, #16]
	cmp	r0, #1
	bne	.L575
	ldr	r2, [r3, #528]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L568
.L555:
	.word	.L554
	.word	.L556
	.word	.L557
	.word	.L558
	.word	.L551
	.word	.L560
	.word	.L561
	.word	.L568
	.word	.L562
	.word	.L563
	.word	.L564
	.word	.L564
	.word	.L564
	.word	.L565
	.word	.L568
	.word	.L551
	.word	.L566
	.word	.L567
.L568:
	mov	r0, #0
.L551:
	ldmfd	sp, {fp, sp, pc}
.L558:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_GetImageBuffer
.L560:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_GetImageBuffer
.L561:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_GetImageBuffer
.L562:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_GetImageBuffer
.L563:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_GetImageBuffer
.L564:
	ldr	r2, [r1, #2648]
	tst	r2, #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_GetImageBuffer
.L565:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_GetImageBuffer
.L566:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_GetImageBuffer
.L567:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageBuffer
.L554:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageBuffer
.L556:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_GetImageBuffer
.L557:
	add	r0, r3, #229376
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageBuffer
.L575:
	ldr	ip, .L576+4
	movw	r2, #5026
	ldr	r3, .L576+8
.L572:
	ldr	r1, .L576+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L573:
	ldr	ip, .L576+4
	movw	r2, #5024
	ldr	r3, .L576+16
	b	.L572
.L574:
	ldr	ip, .L576+4
	mov	r0, r3
	movw	r2, #5025
	ldr	r3, .L576+20
	ldr	r1, .L576+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L577:
	.align	2
.L576:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC59
	.word	.LC1
	.word	.LC58
	.word	.LC45
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
	bhi	.L585
	ldr	r3, .L589
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L586
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L587
	cmp	r1, #0
	beq	.L588
	ldr	r2, [r1]
	mov	r0, #0
	str	r2, [r3, #1220]
	ldmfd	sp, {fp, sp, pc}
.L587:
	ldr	ip, .L589+4
	movw	r2, #5138
	ldr	r3, .L589+8
.L584:
	ldr	r1, .L589+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L585:
	ldr	ip, .L589+4
	movw	r2, #5136
	ldr	r3, .L589+16
	b	.L584
.L586:
	ldr	ip, .L589+4
	mov	r0, r3
	movw	r2, #5137
	ldr	r3, .L589+20
	ldr	r1, .L589+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L588:
	ldr	ip, .L589+4
	mov	r0, r1
	ldr	r3, .L589+24
	movw	r2, #5139
	ldr	r1, .L589+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L590:
	.align	2
.L589:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC61
	.word	.LC1
	.word	.LC6
	.word	.LC60
	.word	.LC62
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
	bhi	.L598
	ldr	r3, .L602
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L599
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L600
	cmp	r1, #0
	beq	.L601
	ldr	r3, .L602+4
	add	r0, r0, #1232
	mov	r2, #20
	add	r0, r0, #8
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L600:
	ldr	ip, .L602+4
	movw	r2, #5151
	ldr	r3, .L602+8
.L597:
	ldr	r1, .L602+12
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L598:
	ldr	ip, .L602+4
	movw	r2, #5149
	ldr	r3, .L602+16
	b	.L597
.L601:
	ldr	ip, .L602+4
	mov	r0, r1
	ldr	r3, .L602+20
	movw	r2, #5152
	ldr	r1, .L602+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L599:
	ldr	ip, .L602+4
	movw	r2, #5150
	ldr	r3, .L602+24
	ldr	r1, .L602+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L603:
	.align	2
.L602:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC61
	.word	.LC1
	.word	.LC6
	.word	.LC62
	.word	.LC60
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
	ldr	r5, .L610
	mov	r4, r0
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L607
	sub	r1, fp, #44
	bl	SM_GetInfo
	cmp	r0, #0
	bne	.L607
	ldr	r3, [fp, #-36]
	cmp	r3, #16
	bgt	.L608
	ldr	r3, [fp, #-28]
	cmp	r3, #524288
	ble	.L607
.L608:
	mov	r0, r4
	bl	VCTRL_GetImageBuffer
	subs	r0, r0, #1
	mvnne	r0, #0
	cmp	r0, #0
	bne	.L606
	ldr	r3, .L610+4
	ldr	r3, [r3]
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	ldr	r3, [r3, #44]
	rsb	r0, r3, r0
	cmp	r0, #50
	movls	r0, #0
	mvnhi	r0, #0
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L607:
	mvn	r0, #0
.L606:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L611:
	.align	2
.L610:
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
	ldr	r3, .L613
	ldr	r3, [r3, r0, asl #2]
	ldr	r0, [r3, #316]
	subs	r0, r0, #1
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L614:
	.align	2
.L613:
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
	bhi	.L621
	ldr	r3, .L622
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L621
	ldr	r2, [r3, #12]
	cmp	r2, #0
	beq	.L621
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq	.L621
	ldr	r2, [r3, #24]
	cmp	r2, #0
	beq	.L621
	ldr	r0, [r3, #528]
	cmp	r0, #20
	movne	r0, #0
	mvneq	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L621:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L623:
	.align	2
.L622:
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
	ldr	r5, .L641
	mov	r4, r0
	ldr	r7, [r0, #352]
	mov	r2, #368
	mov	r6, r1
	mov	r1, #0
	ldr	r3, [r5, #48]
	blx	r3
	str	r7, [r4, #352]
	add	r0, r4, #8
	bl	ResetVoQueue
	ldr	ip, .L641+4
	mov	r2, #0
	str	r6, [r4]
.L628:
	ldr	r1, [ip], #4
	cmp	r1, #0
	add	r3, r1, #229376
	add	r3, r3, #760
	beq	.L625
	cmp	r4, r3
	beq	.L640
.L625:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L628
	mvn	r3, #0
	str	r3, [r4, #360]
.L626:
	ldr	r3, [r5, #68]
	mov	r0, #0
	ldr	r1, .L641+8
	blx	r3
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L640:
	cmn	r2, #1
	str	r2, [r4, #360]
	beq	.L626
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L642:
	.align	2
.L641:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC63
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r0, #0
	mov	r5, r1
	beq	.L645
	mov	r2, #0
	ldr	r0, [r4, #360]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #360]
	bl	FSP_GetFsImagePtr
	subs	r5, r0, #0
	beq	.L646
	ldr	r1, [r5, #212]
	mov	r6, #0
	ldr	r0, [r4, #360]
	bl	FreeUsdByDec
	str	r6, [r5, #212]
	ldr	r1, [r5, #216]
	ldr	r0, [r4, #360]
	bl	FreeUsdByDec
	str	r6, [r5, #216]
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L646:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L645:
	mvn	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	ldr	r6, .L671
	mov	r5, r0
	mov	r7, r1
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	beq	.L665
	ldr	r2, [r3, #528]
	cmp	r2, #19
	bne	.L666
	ldr	r3, [r3, #468]
	cmp	r3, #2
	ble	.L667
	bl	FSP_IsNewFsAvalible
	cmp	r0, #1
	bne	.L668
	mov	r1, r0
	mov	r0, r5
	bl	FSP_NewLogicFs
	subs	r8, r0, #0
	blt	.L669
	mov	r1, r8
	mov	r0, r5
	bl	FSP_GetLogicFs
	ldr	r9, .L671+4
	mov	r2, r8
	ldr	r1, .L671+8
	ldr	r3, [r9, #68]
	mov	r4, r0
	mov	r0, #18
	blx	r3
	cmp	r4, #0
	beq	.L670
	ldr	r2, [r4, #520]
	cmp	r2, #0
	beq	.L655
	ldr	r3, [r4, #524]
	cmp	r3, #0
	beq	.L655
	ldr	r1, [r4, #528]
	cmp	r1, #0
	beq	.L655
	ldr	lr, [r1, #12]
	mov	r0, #18
	ldr	r3, [r3, #12]
	ldr	r2, [r2, #12]
	ldr	ip, [r9, #68]
	ldr	r1, .L671+12
	str	lr, [sp]
	blx	ip
	ldr	r3, [r4, #520]
	mov	r0, #0
	ldr	r3, [r3, #12]
	str	r3, [r7]
	ldr	r3, [r6, r5, asl #2]
	ldr	r3, [r3, #464]
	str	r3, [r7, #8]
.L649:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L668:
	ldr	r3, .L671+4
	mov	r0, #0
	ldr	r1, .L671+16
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L649
.L666:
	ldr	r3, .L671+4
	mov	r0, #0
	ldr	r1, .L671+20
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L649
.L667:
	ldr	r3, .L671+4
	mov	r0, #0
	ldr	r1, .L671+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L649
.L655:
	ldr	r3, [r9, #68]
	mov	r0, #0
	ldr	r1, .L671+28
	blx	r3
	mvn	r0, #0
	b	.L649
.L669:
	ldr	r3, .L671+4
	mov	r0, #0
	ldr	r1, .L671+32
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L649
.L665:
	ldr	ip, .L671+4
	mov	r0, r3
	movw	r2, #5517
	ldr	r3, .L671+36
	ldr	r1, .L671+40
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L649
.L670:
	ldr	r3, [r9, #68]
	movw	r2, #5553
	ldr	r1, .L671+44
	mov	r0, #22
	blx	r3
	mvn	r0, #0
	b	.L649
.L672:
	.align	2
.L671:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC68
	.word	.LC70
	.word	.LC66
	.word	.LC64
	.word	.LC65
	.word	.LC71
	.word	.LC67
	.word	.LC2
	.word	.LC1
	.word	.LC69
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
	ldr	r2, .L695
	ldr	r1, [r2, r0, asl #2]
	cmp	r1, #0
	beq	.L694
	ldr	r3, [r1, #528]
	cmp	r3, #19
	ldrls	pc, [pc, r3, asl #2]
	b	.L673
.L677:
	.word	.L676
	.word	.L678
	.word	.L679
	.word	.L680
	.word	.L681
	.word	.L682
	.word	.L683
	.word	.L673
	.word	.L684
	.word	.L685
	.word	.L686
	.word	.L686
	.word	.L686
	.word	.L687
	.word	.L673
	.word	.L681
	.word	.L688
	.word	.L689
	.word	.L673
	.word	.L690
.L690:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	USERDEC_Init
.L676:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_Init
.L678:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_Init
.L679:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_Init
.L680:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_Init
.L681:
	ldr	r3, [r2, #248]
	cmp	r3, #0
	ldmeqfd	sp, {fp, sp, pc}
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	r3
.L682:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_Init
.L683:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_Init
.L684:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_Init
.L685:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_Init
.L686:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_Init
.L687:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_Init
.L688:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_Init
.L689:
	add	r0, r1, #229376
	add	r1, r1, #536
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_Init
.L673:
	ldmfd	sp, {fp, sp, pc}
.L694:
	ldr	ip, .L695+4
	mov	r0, r1
	ldr	r3, .L695+8
	movw	r2, #5635
	ldr	r1, .L695+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L696:
	.align	2
.L695:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	InitSyntaxCtx, .-InitSyntaxCtx
	.align	2
	.global	VCTRL_GetChanMemSizeWithOption
	.type	VCTRL_GetChanMemSizeWithOption, %function
VCTRL_GetChanMemSizeWithOption:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 816
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #840)
	sub	sp, sp, #840
	movw	ip, #28424
	cmp	r0, #26
	movw	r5, #57520
	movt	ip, 8
	movt	r5, 176
	movne	r5, ip
	cmp	r3, #2
	mov	r7, r0
	mov	r8, r3
	mov	r4, r1
	mov	r6, r2
	beq	.L699
	ldr	r1, .L737
	ldr	r2, [r4, #36]
	ldr	r0, [r4, #60]
	ldr	ip, [r1, #2724]
	ldr	r3, [r1, #2720]
	add	r9, r0, ip
	add	r1, r2, r3
	orrs	lr, r9, r1
	bmi	.L729
	cmp	r1, #16
	strle	r1, [r4, #36]
	strle	r9, [r4, #60]
	bgt	.L730
.L703:
	subs	r3, r7, #25
	movne	r3, #1
	cmp	r8, #0
	movne	r3, #0
	cmp	r3, #0
	bne	.L731
.L699:
	ldr	r1, [r4, #36]
	cmp	r1, #0
	blt	.L704
	ldr	r2, [r4, #12]
	cmp	r2, #31
	ble	.L704
	ldr	r3, [r4, #16]
	cmp	r3, #31
	ble	.L704
	ldr	r0, [r4, #56]
	cmp	r0, #262144
	blt	.L704
	ldr	r0, [r4, #44]
	cmp	r0, #1
	beq	.L732
	cmp	r0, #2
	beq	.L733
.L706:
	ldr	r0, [r4, #40]
	cmp	r0, #0
	moveq	r0, #1
	bne	.L734
.L708:
	ldr	ip, .L737+4
	sub	r8, r7, #27
	cmp	r8, #6
	ldr	r9, [r4, #60]
	ldr	ip, [ip, #48]
	mvncc	lr, #1
	mvncs	lr, #0
	cmp	ip, #0
	beq	.L710
	add	r1, r1, r9
	str	lr, [sp, #12]
	add	r1, r1, #2
	stmia	sp, {r0, r1}
	sub	r0, fp, #848
	mov	r1, #0
	str	r0, [sp, #16]
	mov	r0, #1
	str	r1, [sp, #8]
	mvn	r1, #-2147483648
	blx	ip
	cmp	r0, #0
	bne	.L710
	ldr	r3, [fp, #-844]
	add	r2, r3, #16
	str	r2, [r6]
	ldr	r2, [r4, #128]
	cmp	r2, #1
	addne	r3, r3, #47616
	addne	r3, r3, #16
	ldreq	r3, [r4, #56]
	strne	r3, [r6]
	ldrne	r3, [r4, #56]
	addeq	r3, r3, #79872
	addeq	r3, r3, #528
	str	r5, [r6, #8]
	addne	r3, r3, #32768
	str	r3, [r6, #4]
	ldr	r3, [r4, #44]
	cmp	r3, #0
	beq	.L713
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
	beq	.L735
.L713:
	cmp	r8, #5
	bhi	.L714
.L736:
	movw	r2, #51528
	movt	r2, 13
	cmp	r3, r2
	movcc	r3, r2
.L714:
	mov	r0, #0
	add	r3, r3, r5
	str	r3, [r6, #8]
.L701:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L734:
	add	r0, r1, #1
	cmp	r0, #17
	movge	r0, #17
	b	.L708
.L733:
	ldr	r0, [r4, #20]
	cmp	r0, #0
	ble	.L704
	ldr	r0, [r4, #24]
	cmp	r0, #0
	ble	.L704
.L728:
	ldr	r0, [r4, #28]
	cmp	r0, #0
	ble	.L704
	ldr	r0, [r4, #32]
	cmp	r0, #0
	bgt	.L706
.L704:
	ldr	r3, .L737+8
	mov	r0, #32
	ldr	r1, .L737+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L710:
	ldr	r3, .L737+8
	movw	r2, #5909
	ldr	r1, .L737+16
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L732:
	ldr	r0, [r4, #20]
	cmp	r0, #0
	bgt	.L728
	b	.L704
.L730:
	ldr	ip, .L737+8
	mov	r0, #32
	ldr	r1, .L737+20
	ldr	ip, [ip, #68]
	blx	ip
	mov	r3, #16
	str	r9, [r4, #60]
	str	r3, [r4, #36]
	b	.L703
.L735:
	movw	r0, #3992
	mov	r1, #2240
	mla	r2, r0, lr, r2
	mla	ip, r1, ip, r2
	cmp	r3, ip
	movcc	r3, ip
	cmp	r8, #5
	bls	.L736
	b	.L714
.L731:
	ldr	r3, .L737+8
	mov	r0, #32
	ldr	r1, .L737+24
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L701
.L729:
	ldr	r1, .L737+8
	stmia	sp, {r0, ip}
	mov	r0, #32
	ldr	ip, [r1, #68]
	ldr	r1, .L737+28
	blx	ip
	mvn	r0, #0
	b	.L701
.L738:
	.align	2
.L737:
	.word	.LANCHOR0
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC75
	.word	.LC76
	.word	.LC73
	.word	.LC74
	.word	.LC72
	UNWIND(.fnend)
	.size	VCTRL_GetChanMemSizeWithOption, .-VCTRL_GetChanMemSizeWithOption
	.align	2
	.global	VCTRL_GetChanMemSize
	.type	VCTRL_GetChanMemSize, %function
VCTRL_GetChanMemSize:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 840
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #876)
	sub	sp, sp, #876
	ldr	r9, .L766
	mov	r3, r0, asl #2
	mov	r4, r0
	mov	r0, r0, asl #5
	str	r3, [fp, #-880]
	rsb	r3, r3, r0
	str	r2, [fp, #-872]
	add	r3, r9, r3
	str	r0, [fp, #-884]
	sub	r0, r4, #18
	str	r1, [fp, #-868]
	ldr	r2, [r3, #16]
	ldr	r8, [r3, #12]
	ldr	r10, [r3, #8]
	str	r2, [fp, #-864]
	ldr	r2, [r3, #20]
	str	r2, [fp, #-876]
	cmp	r0, #7
	ldrls	pc, [pc, r0, asl #2]
	b	.L740
.L742:
	.word	.L756
	.word	.L756
	.word	.L756
	.word	.L756
	.word	.L756
	.word	.L743
	.word	.L743
	.word	.L743
.L756:
	mov	r3, #3
.L741:
	ldr	r7, .L766+4
	rsb	r3, r3, r8
	sub	r3, r3, #2
	ldr	r2, [r7, #2720]
	cmp	r2, #0
	movlt	r2, #0
	strlt	r2, [r7, #2720]
	cmp	r3, #15
	movgt	r6, #0
	bgt	.L745
	ldr	r6, [r7, #2720]
	add	r3, r3, r6
	cmp	r3, #16
	rsbgt	r3, r3, #16
	addgt	r6, r6, r3
.L745:
	add	r3, r10, r6
	ldr	r5, [r7, #2724]
	cmp	r3, #0
	ble	.L764
	cmp	r3, #17
	movlt	r10, r3
	movge	r10, #17
.L747:
	cmp	r4, #23
	bhi	.L748
	ldr	r3, [fp, #-864]
	add	r5, r6, r5
	add	r2, r5, r8
	add	r1, r3, r5
	ldr	r3, [fp, #-876]
	cmp	r2, #0
	cmpgt	r1, #0
	add	r5, r3, r5
	movle	r3, #1
	movgt	r3, #0
	cmp	r5, #0
	orrle	r3, r3, #1
	cmp	r3, #0
	streq	r5, [fp, #-876]
	streq	r1, [fp, #-864]
	moveq	r8, r2
	bne	.L765
.L748:
	cmp	r4, #32
	bhi	.L759
	ldr	r5, .L766+8
	ldr	ip, [r5, #48]
	cmp	ip, #0
	beq	.L750
	ldr	r3, [fp, #-884]
	mov	r1, #0
	ldr	r2, [fp, #-880]
	mvn	r7, #0
	str	r8, [sp, #4]
	mov	r0, #1
	rsb	r6, r2, r3
	str	r10, [sp]
	add	r4, r9, r6
	sub	r8, fp, #856
	ldr	r2, [r9, r6]
	ldr	r3, [r4, #4]
	str	r1, [sp, #8]
	mvn	r1, #-2147483648
	str	r8, [sp, #16]
	str	r7, [sp, #12]
	blx	ip
	ldr	r9, .L766
	cmp	r0, #0
	bne	.L750
	ldr	r2, [fp, #-868]
	ldr	r3, [fp, #-852]
	add	r3, r3, #47616
	add	r3, r3, #16
	str	r3, [r2]
	ldr	r2, [fp, #-872]
	ldr	r3, [r4, #24]
	add	r3, r3, #32768
	str	r3, [r2]
	ldr	ip, [r5, #48]
	cmp	ip, #0
	beq	.L752
	ldr	r3, [r4, #4]
	mvn	r1, #-2147483648
	str	r0, [sp, #8]
	ldr	r0, [fp, #-864]
	ldr	r2, [r9, r6]
	str	r8, [sp, #16]
	str	r0, [sp, #4]
	mov	r0, #1
	str	r7, [sp, #12]
	str	r10, [sp]
	blx	ip
	subs	lr, r0, #0
	bne	.L752
	ldr	r2, [fp, #-868]
	ldr	r3, [fp, #-852]
	add	r3, r3, #47616
	add	r3, r3, #16
	str	r3, [r2, #4]
	ldr	r2, [fp, #-872]
	ldr	r3, [r4, #24]
	add	r3, r3, #32768
	str	r3, [r2, #4]
	ldr	ip, [r5, #48]
	cmp	ip, #0
	beq	.L755
	ldr	r3, [r4, #4]
	mvn	r1, #-2147483648
	str	lr, [sp, #8]
	mov	r0, #1
	ldr	lr, [fp, #-876]
	ldr	r2, [r9, r6]
	str	r8, [sp, #16]
	str	r7, [sp, #12]
	stmia	sp, {r10, lr}
	blx	ip
	cmp	r0, #0
	bne	.L755
	ldr	r2, [fp, #-868]
	ldr	r3, [fp, #-852]
	add	r3, r3, #47616
	add	r3, r3, #16
	str	r3, [r2, #8]
	ldr	r2, [fp, #-872]
	ldr	r3, [r4, #24]
	add	r3, r3, #32768
	str	r3, [r2, #8]
.L749:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L743:
	mov	r3, #1
	b	.L741
.L752:
	ldr	r3, .L766+12
	movw	r2, #6173
.L763:
	ldr	r1, .L766+16
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L765:
	ldr	r1, [r7, #2724]
	mov	r3, r6
	ldr	ip, .L766+12
	mov	r2, r8
	str	r6, [sp, #20]
	mov	r0, #32
	str	r1, [sp, #24]
	str	r1, [sp, #12]
	str	r1, [sp]
	ldr	r1, [fp, #-876]
	str	r6, [sp, #8]
	str	r1, [sp, #16]
	ldr	r1, [fp, #-864]
	str	r1, [sp, #4]
	ldr	r1, .L766+20
	ldr	ip, [ip, #68]
	blx	ip
	b	.L748
.L740:
	mov	r3, #6
	b	.L741
.L750:
	ldr	r3, .L766+12
	movw	r2, #6152
	b	.L763
.L755:
	ldr	r3, .L766+12
	movw	r2, #6194
	b	.L763
.L764:
	ldr	ip, .L766+12
	mov	r2, r10
	mov	r3, r6
	ldr	r1, .L766+24
	mov	r0, #32
	ldr	ip, [ip, #68]
	blx	ip
	cmp	r10, #17
	movge	r10, #17
	b	.L747
.L759:
	mvn	r0, #0
	b	.L749
.L767:
	.align	2
.L766:
	.word	CapItem
	.word	.LANCHOR0
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC79
	.word	.LC78
	.word	.LC77
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
	ldr	r5, [r0, #12]
	mov	r4, r0
	ldr	r6, [r0, #8]
	cmp	r5, #0
	ldr	r7, [r0, #4]
	beq	.L769
	ldr	r3, .L813
	ldr	r3, [r3, #44]
	cmp	r3, #0
	beq	.L804
	mov	r0, #0
	ldr	r8, .L813+4
	blx	r3
.L772:
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_ReleaseMemBlock
	ldr	r3, [r8, #48]
	mov	r2, #16
	mov	r1, #0
	mov	r0, r4
	blx	r3
.L769:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #28]
	ldr	r6, [r4, #24]
	cmp	r5, #0
	ldr	r7, [r4, #20]
	bne	.L805
.L773:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	bl	SM_InstInit
	ldr	r5, [r4, #60]
	ldr	r6, [r4, #56]
	cmp	r5, #0
	ldr	r7, [r4, #52]
	bne	.L806
.L774:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #108]
	ldr	r6, [r4, #104]
	cmp	r5, #0
	ldr	r7, [r4, #100]
	bne	.L807
.L775:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #92]
	ldr	r6, [r4, #88]
	cmp	r5, #0
	ldr	r7, [r4, #84]
	bne	.L808
.L776:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #76]
	ldr	r6, [r4, #72]
	cmp	r5, #0
	ldr	r7, [r4, #68]
	bne	.L809
.L777:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #124]
	ldr	r6, [r4, #120]
	cmp	r5, #0
	ldr	r7, [r4, #116]
	bne	.L810
.L778:
	ldr	r4, .L813+8
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r3, [r4, #224]
	cmp	r3, #1
	beq	.L811
.L779:
	ldr	r3, [r4, #228]
	ldr	r4, .L813+8
	cmp	r3, #1
	beq	.L812
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L805:
	mov	r0, #0
	bl	SM_CloseSCDDrv
	ldr	r1, [r4, #20]
	ldr	r0, [r4, #24]
	bl	MEM_ReleaseMemBlock
	ldr	r3, .L813+4
	mov	r2, #16
	mov	r1, #0
	add	r0, r4, r2
	ldr	r3, [r3, #48]
	blx	r3
	b	.L773
.L806:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L813+4
	mov	r2, #16
	mov	r1, #0
	add	r0, r4, #48
	ldr	r3, [r3, #48]
	blx	r3
	b	.L774
.L810:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L813+4
	add	r0, r4, #112
	mov	r2, #16
	mov	r1, #0
	ldr	r3, [r3, #48]
	blx	r3
	b	.L778
.L809:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L813+4
	mov	r2, #16
	mov	r1, #0
	add	r0, r4, #64
	ldr	r3, [r3, #48]
	blx	r3
	b	.L777
.L808:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L813+4
	mov	r2, #16
	mov	r1, #0
	add	r0, r4, #80
	ldr	r3, [r3, #48]
	blx	r3
	b	.L776
.L807:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L813+4
	mov	r2, #16
	mov	r1, #0
	add	r0, r4, #96
	ldr	r3, [r3, #48]
	blx	r3
	b	.L775
.L804:
	ldr	r8, .L813+4
	mov	r0, r3
	movw	r2, #6574
	ldr	r1, .L813+12
	ldr	r3, [r8, #68]
	blx	r3
	b	.L772
.L812:
	ldr	r3, .L813+4
	mov	r1, #0
	mov	r0, #136
	ldr	r3, [r3, #180]
	blx	r3
	mov	r3, #0
	mov	r0, #0
	str	r3, [r4, #228]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L811:
	ldr	r3, .L813+4
	mov	r1, #0
	mov	r0, #127
	ldr	r3, [r3, #180]
	blx	r3
	mov	r3, #0
	str	r3, [r4, #224]
	b	.L779
.L814:
	.align	2
.L813:
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC16
	UNWIND(.fnend)
	.size	CloseDrivers, .-CloseDrivers
	.align	2
	.global	OpenDrivers
	.type	OpenDrivers, %function
OpenDrivers:
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
	add	r5, r0, #48
	mov	r4, r0
	mov	r0, #0
	mov	r1, #4096
	mov	r2, r5
	movt	r0, 63683
	bl	MEM_MapRegisterAddr
	subs	r6, r0, #0
	bne	.L816
	ldr	r2, [r4, #60]
	add	r7, r4, #96
	ldr	r1, [r4, #52]
	ldr	r0, [r4, #56]
	bl	MEM_AddMemRecord
	mov	r0, #0
	mov	r2, r7
	movt	r0, 63686
	mov	r1, #4096
	bl	MEM_MapRegisterAddr
	cmp	r0, #0
	bne	.L822
	ldr	r5, .L845
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq	.L821
	blx	r3
	subs	r6, r0, #0
	beq	.L844
.L821:
	ldr	r3, .L845+4
	mov	r0, r7
.L840:
	mov	r2, #16
	mov	r1, #0
.L843:
	ldr	r3, [r3, #48]
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
.L819:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L844:
	ldr	r2, [r4, #108]
	add	r7, r4, #80
	ldr	r1, [r4, #100]
	ldr	r0, [r4, #104]
	bl	MEM_AddMemRecord
	mov	r0, #49152
	mov	r2, r7
	movt	r0, 63683
	mov	r1, #4096
	bl	MEM_MapRegisterAddr
	subs	r8, r0, #0
	bne	.L822
	ldr	r9, .L845+4
	ldr	r2, [r4, #92]
	ldr	r1, [r4, #84]
	ldr	r0, [r4, #88]
	bl	MEM_AddMemRecord
	mov	r0, #11
	ldr	r6, [r9, #72]
	bl	VFMW_GetString
	mov	r3, r8
	mov	r1, #20
	mov	r2, r0
	sub	r0, fp, #64
	blx	r6
	ldr	r3, [r5, #36]
	cmp	r3, #0
	beq	.L824
	blx	r3
	subs	r2, r0, #0
	ble	.L824
	str	r8, [sp]
	mov	r3, r4
	mov	r1, r8
	sub	r0, fp, #64
	bl	MEM_AllocMemBlock
	subs	r6, r0, #0
	bne	.L825
	ldr	r2, [r4, #12]
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_AddMemRecord
	ldr	r3, [r9, #48]
	mov	r2, #12
	mov	r1, r6
	sub	r0, fp, #76
	blx	r3
	ldr	r3, [r5, #40]
	ldr	r1, [r4, #8]
	ldr	r2, [r4, #12]
	cmp	r3, #0
	str	r6, [fp, #-68]
	str	r1, [fp, #-76]
	str	r2, [fp, #-72]
	beq	.L826
	sub	r0, fp, #76
	blx	r3
	cmp	r0, #0
	bne	.L826
	bl	VDMDRV_Init
	mov	r0, #12
	ldr	r8, [r9, #72]
	bl	VFMW_GetString
	mov	r6, #0
	add	r7, r4, #16
	mov	r3, r6
	mov	r1, #20
	ldr	r5, .L845+4
	mov	r2, r0
	sub	r0, fp, #64
	blx	r8
	sub	r0, fp, #64
	mov	r3, r7
	str	r6, [sp]
	mov	r1, r6
	mov	r2, #716800
	bl	MEM_AllocMemBlock
	subs	r8, r0, #0
	bne	.L830
	ldr	r2, [r4, #28]
	ldr	r1, [r4, #20]
	ldr	r0, [r4, #24]
	bl	MEM_AddMemRecord
	mov	r2, r8
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #24]
	bl	SM_OpenSCDDrv
	subs	r6, r0, #0
	bne	.L839
	bl	SM_InstInit
	mov	r0, r6
	ldr	r8, [r5, #176]
	bl	VFMW_GetString
	str	r6, [sp]
	mov	r2, #32
	ldr	r1, .L845+8
	mov	r3, r0
	mov	r0, #127
	blx	r8
	subs	r8, r0, #0
	bne	.L830
	ldr	r6, .L845+12
	mov	r0, #3
	mov	r9, #1
	ldr	r10, [r5, #176]
	str	r9, [r6, #224]
	bl	VFMW_GetString
	str	r8, [sp]
	mov	r2, #32
	ldr	r1, .L845+16
	mov	r3, r0
	mov	r0, #136
	blx	r10
	cmp	r0, #0
	streq	r9, [r6, #228]
	beq	.L819
.L839:
	ldr	r3, [r5, #48]
	mov	r1, r8
.L842:
	mov	r2, #16
	mov	r0, r7
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L819
.L822:
	ldr	r3, .L845+4
	mov	r1, r6
	mov	r0, r7
	mov	r2, #16
	b	.L843
.L830:
	ldr	r3, [r5, #48]
	mov	r1, r6
	b	.L842
.L826:
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
.L841:
	mov	r0, r4
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L819
.L825:
	ldr	r3, [r9, #48]
	mov	r1, r8
	mov	r2, #16
	b	.L841
.L824:
	ldr	r3, [r9, #48]
	mov	r2, #16
	mov	r1, #0
	mov	r0, r7
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L819
.L816:
	ldr	r3, .L845+4
	mov	r0, r5
	b	.L840
.L846:
	.align	2
.L845:
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	VCTRL_Isr_0
	.word	.LANCHOR0
	.word	VCTRL_Scd_Isr
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
	ldr	r4, .L882
	ldr	r6, .L882+4
	mov	r2, #8
	sub	r0, fp, #44
	mov	r1, #0
	ldr	r3, [r4, #48]
	blx	r3
	ldr	r5, [r6, #64]
	cmp	r5, #0
	bne	.L877
	mov	r1, #1024
	ldr	r3, [r4, #84]
	str	r5, [fp, #-48]
	ldr	r0, .L882+8
	blx	r3
	ldr	r3, [r4, #100]
	ldr	r1, .L882+12
	ldr	r0, .L882+8
	blx	r3
	cmp	r0, #0
	str	r0, [fp, #-48]
	beq	.L850
	ldr	r3, [r4, #96]
	sub	r0, fp, #48
	ldr	r1, .L882+16
	blx	r3
	cmp	r0, #0
	beq	.L869
	sub	r0, fp, #48
	ldr	r3, [r4, #96]
	ldr	r1, .L882+20
	blx	r3
	cmp	r0, #0
	beq	.L878
	mov	r2, r5
	ldr	r3, [r4, #88]
	mov	r1, r5
	blx	r3
	mov	r2, r0
	str	r0, [r6, #2720]
.L854:
	add	r3, r2, #4992
	movw	r1, #10000
	add	r3, r3, #8
	movw	r7, #5000
	cmp	r3, r1
	bhi	.L855
	cmp	r2, r7
	movw	r3, #60536
.L876:
	movt	r3, 65535
	movge	r2, r7
	cmp	r2, r3
	movlt	r2, r3
.L851:
	str	r2, [r6, #2720]
.L853:
	mov	r8, #0
	ldr	r3, [r4, #84]
	mov	r1, #1024
	str	r8, [fp, #-48]
	ldr	r0, .L882+8
	blx	r3
	ldr	r3, [r4, #100]
	ldr	r1, .L882+24
	ldr	r0, .L882+8
	blx	r3
	ldr	r7, .L882
	ldr	r9, .L882+4
	cmp	r0, r8
	str	r0, [fp, #-48]
	beq	.L856
	ldr	r3, [r7, #96]
	sub	r0, fp, #48
	ldr	r1, .L882+16
	blx	r3
	cmp	r0, r8
	beq	.L857
	sub	r0, fp, #48
	ldr	r3, [r7, #96]
	ldr	r1, .L882+20
	blx	r3
	cmp	r0, r8
	beq	.L879
	mov	r2, r8
	ldr	r3, [r7, #88]
	mov	r1, r8
	blx	r3
	mov	r2, r0
	str	r0, [r9, #2724]
.L860:
	add	r3, r2, #4992
	movw	r1, #10000
	add	r3, r3, #8
	movw	r7, #5000
	cmp	r3, r1
	bhi	.L861
	cmp	r2, r7
	movw	r3, #60536
	movt	r3, 65535
	movlt	r5, r2
	movge	r5, r7
	cmp	r5, r3
	movlt	r5, r3
.L857:
	str	r5, [r6, #2724]
.L859:
	ldr	r5, [r6, #3752]
	mov	r3, #0
	ldr	r7, .L882+4
	cmp	r5, r3
	str	r3, [fp, #-48]
	beq	.L862
	ldr	r2, .L882+28
	ubfx	r1, r5, #1, #1
	and	r0, r5, #1
	ubfx	r3, r5, #2, #1
	strb	r0, [r7, #3756]
	ubfx	r5, r5, #3, #1
	strb	r1, [r2]
	strb	r3, [r7, #3757]
	strb	r5, [r7, #3758]
.L863:
	ldr	r5, .L882+4
	mov	r2, #176
	ldr	r3, [r4, #48]
	mov	r1, #0
	add	r0, r5, #64
	ldr	r7, .L882
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #64
	mov	r1, #0
	mov	r0, r5
	blx	r3
	ldr	r2, .L882+32
	sub	r0, fp, #44
	mov	r1, #134217728
	mov	r3, #75497472
	str	r1, [fp, #-44]
	str	r2, [r6, #2652]
	str	r3, [fp, #-40]
	bl	MEM_InitMemManager
	bl	FSP_Init
	bl	ChanCtx_Init
	ldr	r3, [r4, #48]
	mov	r2, #64
	mov	r1, #0
	add	r0, r5, #3760
	blx	r3
	add	r0, r5, #88
	bl	OpenDrivers
	subs	r4, r0, #0
	bne	.L877
	bl	DBG_CreateTracer
	ldr	r3, [r7, #124]
	mov	r1, #1
	mov	r0, r4
	blx	r3
	ldr	r3, [r7, #172]
	ldr	r2, .L882+36
	add	r0, r5, #76
	ldr	r1, .L882+40
	blx	r3
	cmp	r0, #0
	moveq	r2, #3
	moveq	r3, #1
	streq	r2, [r5, #68]
	streq	r3, [r5, #64]
	bne	.L880
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L862:
	ldr	r3, [r4, #84]
	mov	r1, #1024
	ldr	r0, .L882+8
	blx	r3
	ldr	r3, [r4, #100]
	ldr	r1, .L882+44
	ldr	r0, .L882+8
	blx	r3
	ldr	r8, .L882
	cmp	r0, #0
	str	r0, [fp, #-48]
	beq	.L863
	ldr	r3, [r8, #96]
	sub	r0, fp, #48
	ldr	r1, .L882+16
	blx	r3
	cmp	r0, #0
	ldreq	r0, [r7, #3752]
	beq	.L865
	sub	r0, fp, #48
	ldr	r3, [r8, #96]
	ldr	r1, .L882+20
	blx	r3
	cmp	r0, #0
	beq	.L881
	ldr	r3, [r8, #88]
	mov	r2, r5
	mov	r1, r5
	blx	r3
	str	r0, [r7, #3752]
.L865:
	ldr	r2, .L882+28
	ubfx	r1, r0, #1, #1
	and	ip, r0, #1
	ubfx	r3, r0, #2, #1
	strb	ip, [r6, #3756]
	ubfx	r0, r0, #3, #1
	strb	r1, [r2]
	strb	r3, [r6, #3757]
	strb	r0, [r6, #3758]
	b	.L863
.L856:
	ldr	r2, [r9, #2724]
	b	.L860
.L850:
	ldr	r2, [r6, #2720]
	b	.L854
.L861:
	ldr	r3, [r4, #68]
	mov	r0, #32
	ldr	r1, .L882+48
	blx	r3
	ldr	r5, [r6, #2724]
	movw	r3, #60536
	cmp	r5, r7
	movt	r3, 65535
	movge	r5, r7
	cmp	r5, r3
	movlt	r5, r3
	b	.L857
.L855:
	ldr	r3, [r4, #68]
	mov	r0, #32
	ldr	r1, .L882+52
	blx	r3
	ldr	r2, [r6, #2720]
	movw	r3, #60536
	cmp	r2, r7
	b	.L876
.L869:
	mov	r2, r5
	b	.L851
.L880:
	add	r0, r5, #88
	bl	CloseDrivers
.L877:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L878:
	ldr	ip, [r4, #68]
	movw	r3, #7908
	ldr	r2, .L882+56
	mov	r0, #22
	ldr	r1, .L882+60
	blx	ip
	b	.L853
.L879:
	ldr	ip, [r7, #68]
	movw	r3, #7955
	ldr	r2, .L882+64
	mov	r0, #22
	ldr	r1, .L882+60
	blx	ip
	b	.L859
.L881:
	ldr	ip, [r8, #68]
	movw	r3, #8012
	ldr	r2, .L882+68
	mov	r0, #22
	ldr	r1, .L882+60
	blx	ip
	b	.L863
.L883:
	.align	2
.L882:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR0+2728
	.word	.LC80
	.word	.LC81
	.word	.LC82
	.word	.LC85
	.word	.LANCHOR2
	.word	VCTRL_EventReport
	.word	VFMW_CTRL_thread_proc
	.word	.LC88
	.word	.LC87
	.word	.LC86
	.word	.LC84
	.word	.LANCHOR1+140
	.word	.LC83
	.word	.LANCHOR1+160
	.word	.LANCHOR1+180
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #8)
	sub	sp, sp, #8
	movw	r6, #21846
	mov	r3, r2, asl #1
	cmp	r2, #0
	movt	r6, 21845
	add	r5, r2, #7
	smull	r6, r7, r3, r6
	movge	r5, r2
	ldr	r8, .L891
	mov	r5, r5, asr #3
	str	r2, [sp]
	mov	r6, r0
	str	r1, [sp, #4]
	mov	r4, r2
	mov	r9, r1
	sub	r3, r7, r3, asr #31
	mov	r2, r5
	ldr	ip, [r8, #68]
	ldr	r1, .L891+4
	mov	r0, #30
	blx	ip
	cmp	r6, #3
	beq	.L889
	cmp	r4, r9
	blt	.L890
.L888:
	mov	r4, r6
.L886:
	ldr	ip, [r8, #68]
	mov	r2, r6
	mov	r3, r4
	ldr	r1, .L891+8
	mov	r0, #30
	blx	ip
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L890:
	ldr	r3, [r8, #68]
	mov	r0, #30
	ldr	r1, .L891+12
	mov	r4, #3
	blx	r3
	b	.L886
.L889:
	cmp	r5, r9
	ble	.L888
	ldr	r3, [r8, #68]
	mov	r0, #30
	ldr	r1, .L891+16
	mov	r4, #0
	blx	r3
	b	.L886
.L892:
	.align	2
.L891:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC89
	.word	.LC92
	.word	.LC91
	.word	.LC90
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
	ldr	ip, .L902
	mov	r2, #0
.L896:
	ldr	r1, [ip], #4
	cmp	r1, #0
	add	r3, r1, #229376
	add	r3, r3, #760
	beq	.L894
	cmp	r0, r3
	beq	.L897
.L894:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L896
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L897:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L903:
	.align	2
.L902:
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
	ldr	ip, .L915
	mov	r2, #0
.L908:
	ldr	r3, [ip], #4
	cmp	r3, #0
	beq	.L905
	ldr	r1, [r3, #208]
	cmp	r1, r0
	bhi	.L906
	ldr	lr, [r3, #212]
	add	r1, r1, lr
	cmp	r0, r1
	bcc	.L910
.L906:
	ldr	r1, [r3, #224]
	cmp	r0, r1
	bcc	.L905
	ldr	r3, [r3, #228]
	add	r1, r1, r3
	cmp	r0, r1
	bcc	.L910
.L905:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L908
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L910:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L916:
	.align	2
.L915:
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
	bhi	.L987
	ldr	r7, .L1002
	ldr	r0, [r7, r0, asl #2]
	cmp	r0, #0
	beq	.L988
	ldr	r10, [r0, #16]
	cmp	r10, #1
	bne	.L989
	ldr	r5, [r0, #28]
	cmp	r5, #0
	bne	.L990
	ldr	r6, .L1002+4
	mov	r3, r4, asl #2
	str	r5, [r0, #316]
	mov	r8, r3
	str	r3, [fp, #-48]
	ldr	r3, [r6]
	blx	r3
	add	r2, r7, r8
	ldr	r1, .L1002+8
	ldr	r3, [r2, #3824]
	ldr	r1, [r1]
	rsb	r3, r3, r0
	cmp	r3, r1
	ldr	r3, [r7, r4, asl #2]
	strhi	r0, [r2, #3824]
	movls	r10, r5
	ldr	r1, .L1002
	ldr	r3, [r3, #528]
	sub	r2, r3, #10
	cmp	r2, #2
	bhi	.L924
	ldr	r2, [r1, #2648]
	tst	r2, #1
	bne	.L924
.L925:
	mov	r0, r4
	bl	VCTRL_GetImageBuffer
	cmp	r0, #1
	beq	.L926
	cmp	r10, #1
	mvnne	r5, #2
	beq	.L991
.L964:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L924:
	sub	r2, r3, #15
	bics	r2, r2, #4
	movne	r2, #1
	moveq	r2, #0
	cmp	r3, #4
	moveq	r3, #0
	andne	r3, r2, #1
	cmp	r3, #0
	beq	.L925
	mov	r0, r4
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L925
	cmp	r10, #1
	mvnne	r5, #1
	bne	.L964
	ldr	r3, [r6, #68]
	mov	r2, r4
	ldr	r1, .L1002+12
	mov	r0, #23
	blx	r3
	mvn	r5, #1
	b	.L964
.L926:
	ldr	r3, [r7, r4, asl #2]
	ldr	r2, [r3, #1388]
	str	r3, [fp, #-52]
	cmp	r2, #0
	bne	.L992
.L927:
	ldr	r3, [fp, #-52]
	ldrb	r3, [r3, #665]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L984
	ldr	r3, [fp, #-52]
	add	r3, r3, #672
	str	r3, [fp, #-56]
.L930:
	ldr	r3, [fp, #-56]
	ldr	ip, [r6, #68]
	ldrd	r0, [r3, #40]
	ldrd	r8, [r3, #24]
	strd	r0, [fp, #-68]
	mov	r0, #29
	ldrd	r2, [fp, #-68]
	ldr	r1, .L1002+16
	strd	r2, [sp]
	mov	r2, r8
	mov	r3, r9
	blx	ip
	ldr	ip, [r7, r4, asl #2]
	ldr	r1, [ip, #48]
	cmp	r1, #1
	beq	.L993
	sub	r3, r1, #2
	cmp	r1, #6
	cmpne	r3, #1
	bls	.L951
.L933:
	ldr	r3, [fp, #-56]
	mvn	r2, #0
	ldrd	r0, [r3, #24]
	mvn	r3, #0
	cmp	r1, r3
	add	r3, ip, #592
	cmpeq	r0, r2
	beq	.L994
	strd	r0, [r3, #-8]
.L944:
	ldr	r3, [fp, #-56]
	add	lr, ip, #608
	ldrd	r2, [r3, #40]
	mov	r8, r2
	mov	r9, r3
	add	r2, ip, #624
	mov	r3, r1
	str	r2, [fp, #-60]
	mov	r1, r9
	mov	r2, r0
	mov	r0, r8
	strd	r0, [lr, #-8]
	ldr	r1, [fp, #-56]
	strd	r8, [fp, #-68]
	ldrd	r0, [r1, #48]
	mov	r9, r1
	ldr	r1, [fp, #-60]
	mov	r8, r0
	mov	r0, #29
	strd	r8, [r1, #-8]
	ldr	r8, [fp, #-52]
	ldr	r1, .L1002+20
	ldr	lr, [r8, #728]
	str	lr, [ip, #632]
	ldr	lr, [r8, #732]
	str	lr, [ip, #636]
	ldr	lr, [r8, #736]
	str	lr, [ip, #640]
	ldr	lr, [r8, #740]
	ldrd	r8, [fp, #-68]
	str	lr, [ip, #656]
	strd	r8, [sp]
	ldr	ip, [r6, #68]
	blx	ip
.L942:
	mov	r0, #3
	bl	CHECK_REC_POS_ENABLE
	adds	r3, r0, #0
	movne	r3, #1
	cmp	r10, #1
	movne	r3, #0
	cmp	r3, #0
	bne	.L995
.L945:
	ldr	r3, [r7, r4, asl #2]
	ldr	r2, [fp, #-52]
	ldr	r9, .L1002
	add	r0, r2, #664
	add	r2, r3, #229376
	ldr	r1, [r3, #528]
	add	r2, r2, #760
	bl	DecodeStream
	ldr	r0, [r7, r4, asl #2]
	ldr	r3, [r0, #528]
	cmp	r3, #17
	beq	.L996
.L946:
	mov	r0, #4
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L964
	ldr	r3, [r6]
	blx	r3
	ldr	r3, [fp, #-48]
	ldr	r1, .L1002+24
	add	r7, r7, r3
	ldr	r3, [r7, #3888]
	ldr	r1, [r1]
	rsb	r3, r3, r0
	mov	r2, r0
	cmp	r3, r1
	bls	.L964
	mov	r0, r4
	str	r2, [r7, #3888]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L964
	mov	r2, r4, asl #24
	movw	r1, #7338
	add	r2, r2, #262144
	ldr	r0, .L1002+28
	bl	DBG_AddTrace
	b	.L964
.L992:
	cmp	r2, #1
	beq	.L997
	ldr	r3, [fp, #-52]
	add	r9, r3, #225280
	ldr	r3, [r9, #3256]
.L950:
	ldr	ip, [r6, #68]
	mov	r0, #31
	ldr	r1, .L1002+32
	mvn	r5, #0
	blx	ip
	b	.L964
.L991:
	ldr	r3, [r6, #68]
	mov	r2, r4
	ldr	r1, .L1002+36
	mov	r0, #23
	blx	r3
	mvn	r5, #2
	b	.L964
.L984:
	ldr	r2, [fp, #-52]
	ldr	r3, [r2, #1428]
	cmp	r3, #0
	beq	.L985
	add	r3, r2, #672
	str	r3, [fp, #-56]
	b	.L930
.L993:
	add	r3, ip, #229376
	ldr	r3, [r3, #760]
	ldr	r1, [r3, #548]
	sxtb	r1, r1
	cmp	r1, #1
	bne	.L933
.L951:
	add	r0, ip, #1072
	ldr	r2, [fp, #-56]
	add	r0, r0, #8
	bl	IsNewpicSeg
	cmp	r0, #0
	bne	.L934
	ldr	r1, [r7, r4, asl #2]
	add	r0, r1, #592
	ldr	r3, [r1, #48]
	cmp	r3, #3
	beq	.L998
	ldrd	r8, [r0]
	mvn	r3, #0
	mvn	r2, #0
	cmp	r9, r3
	cmpeq	r8, r2
	beq	.L999
	mov	r2, r8
	mov	r3, r9
	strd	r2, [r0, #-8]
.L936:
	add	ip, r1, #608
	mvn	r2, #0
	mvn	r3, #0
	strd	r2, [r0]
	ldrd	r2, [ip]
	add	r0, r1, #624
	strd	r2, [ip, #-8]
	ldrd	r2, [r0]
	orrs	ip, r2, r3
	strd	r2, [r0, #-8]
	ldreq	r3, [fp, #-56]
	ldreqd	r2, [r3, #48]
	streqd	r2, [r0, #-8]
.L938:
	ldr	ip, [r1, #644]
	mov	r3, r9
	ldr	lr, [r1, #648]
	mov	r2, r8
	mov	r0, #29
	str	ip, [r1, #632]
	str	lr, [r1, #636]
	ldr	ip, [r1, #660]
	ldr	lr, [r1, #652]
	str	ip, [r1, #656]
	str	lr, [r1, #640]
	ldr	ip, [r6, #68]
	ldr	r1, .L1002+40
	blx	ip
	ldr	ip, [r7, r4, asl #2]
	ldr	r3, [ip, #48]
	cmp	r3, #3
	beq	.L1000
.L939:
	ldr	r3, [fp, #-56]
	mvn	r1, #0
	mvn	r0, #0
	ldrd	r2, [r3, #24]
	cmp	r3, r1
	cmpeq	r2, r0
	addne	r1, ip, #592
	strned	r2, [r1]
.L941:
	ldr	lr, [fp, #-56]
	add	r1, ip, #608
	add	r0, ip, #624
	ldrd	r2, [lr, #40]
	strd	r2, [r1]
	ldrd	r2, [lr, #48]
	strd	r2, [r0]
	ldr	r2, [fp, #-52]
	ldr	r3, [r2, #728]
	str	r3, [ip, #644]
	ldr	r3, [r2, #732]
	str	r3, [ip, #648]
	ldr	r3, [r2, #736]
	str	r3, [ip, #652]
	ldr	r3, [r2, #740]
	str	r3, [ip, #660]
.L940:
	ldrd	r2, [r1, #-8]
	mov	r0, #29
	ldr	ip, [r6, #68]
	ldr	r1, .L1002+44
	blx	ip
	b	.L942
.L985:
	ldr	r3, [fp, #-52]
	mov	r0, r4
	add	r3, r3, #672
	str	r3, [fp, #-56]
	mov	r1, r3
	bl	SM_ReadStreamSeg
	cmp	r0, #0
	beq	.L930
	cmp	r10, #1
	beq	.L1001
.L931:
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #1
	mvn	r5, #3
	str	r2, [r3, #316]
	b	.L964
.L989:
	ldr	ip, .L1002+4
	movw	r2, #7090
	ldr	r3, .L1002+48
.L986:
	ldr	r1, .L1002+52
	mov	r0, #0
	ldr	ip, [ip, #68]
	mvn	r5, #0
	blx	ip
	b	.L964
.L990:
	ldr	ip, .L1002+4
	movw	r2, #7091
	ldr	r3, .L1002+56
	b	.L986
.L996:
	ldr	r3, [r0, #1444]
	cmp	r3, #1
	beq	.L946
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	IsMain10Profile
	cmp	r0, #0
	ldreq	r3, [r9, r4, asl #2]
	streq	r0, [r3, #1444]
	beq	.L946
	mov	r3, r4
	ldr	ip, [r6, #68]
	ldr	r2, .L1002+28
	mov	r0, #0
	ldr	r1, .L1002+60
	blx	ip
	ldr	r3, [r9, r4, asl #2]
	mov	r2, #1
	str	r2, [r3, #1444]
	bl	VDMDRV_AddMatchMode
	b	.L946
.L997:
	add	r3, r3, #225280
	ldr	r3, [r3, #3256]
	bics	r1, r3, #2
	beq	.L927
	cmp	r3, #1
	bne	.L950
	ldr	r3, [r6, #68]
	mov	r0, #31
	ldr	r1, .L1002+64
	mvn	r5, #2
	blx	r3
	b	.L964
.L1000:
	ldr	lr, [fp, #-56]
	add	r1, ip, #592
	ldrd	r2, [lr, #24]
	mov	r8, r2
	mov	r9, r3
	strd	r8, [r1]
	add	r2, ip, #608
	ldrd	r0, [lr, #40]
	add	r3, ip, #624
	strd	r0, [r2]
	ldr	r2, [fp, #-52]
	ldrd	r0, [lr, #48]
	strd	r0, [r3]
	ldr	r3, [r2, #728]
	str	r3, [ip, #644]
	ldr	r3, [r2, #732]
	str	r3, [ip, #648]
	ldr	r3, [r2, #736]
	str	r3, [ip, #652]
	ldr	r3, [r2, #740]
	str	r3, [ip, #660]
.L934:
	ldr	ip, [r7, r4, asl #2]
	ldr	r3, [ip, #48]
	cmp	r3, #3
	addeq	r1, ip, #608
	bne	.L939
	b	.L940
.L994:
	ldrd	r0, [r3, #-8]
	b	.L944
.L987:
	ldr	ip, .L1002+4
	movw	r2, #7088
	ldr	r3, .L1002+68
	b	.L986
.L995:
	mov	r2, r4, asl #24
	movw	r1, #7300
	add	r2, r2, #196608
	ldr	r0, .L1002+28
	bl	DBG_AddTrace
	b	.L945
.L998:
	ldrd	r2, [r0]
	mov	r8, r2
	mov	r9, r3
	strd	r2, [r0, #-8]
	b	.L936
.L999:
	ldrd	r2, [r0, #-8]
	mov	r8, r2
	mov	r9, r3
	b	.L936
.L988:
	ldr	ip, .L1002+4
	movw	r2, #7089
	ldr	r3, .L1002+72
	mvn	r5, #0
	ldr	r1, .L1002+52
	ldr	ip, [ip, #68]
	blx	ip
	b	.L964
.L1001:
	ldr	r3, [r6, #68]
	mov	r2, r4
	ldr	r1, .L1002+76
	mov	r0, #23
	blx	r3
	b	.L931
.L1003:
	.align	2
.L1002:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_TraceBsPeriod
	.word	.LC94
	.word	.LC97
	.word	.LC100
	.word	g_TraceFramePeriod
	.word	.LANCHOR1+196
	.word	.LC103
	.word	.LC95
	.word	.LC98
	.word	.LC99
	.word	.LC59
	.word	.LC1
	.word	.LC93
	.word	.LC101
	.word	.LC102
	.word	.LC6
	.word	.LC45
	.word	.LC96
	UNWIND(.fnend)
	.size	DecChanSyntax, .-DecChanSyntax
	.align	2
	.global	DecSyntax
	.type	DecSyntax, %function
DecSyntax:
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
	ldr	r0, .L1033
	ldr	r1, .L1033+4
	sub	r3, r0, #64
.L1007:
	ldr	r2, [r1, #4]!
	add	r3, r3, #4
	cmp	r2, #0
	ldrne	r2, [r2, #24]
	str	r2, [r3, #-4]
	cmp	r3, r0
	bne	.L1007
	ldr	r5, .L1033+8
	mvn	r9, #0
	ldr	r8, .L1033+12
	mov	r6, #0
	mov	r7, r5
.L1008:
	mov	r3, #0
	ldr	r0, [r5, #3952]
	mov	r4, r3
	ldr	r1, .L1033+16
	b	.L1009
.L1011:
	ldr	r2, [r1, #4]!
	cmp	r0, r2
	movlt	r0, r2
	movlt	r4, r3
.L1009:
	add	r3, r3, #1
	cmp	r3, #16
	bne	.L1011
	add	r3, r5, r4, lsl #2
	cmp	r0, #0
	add	r2, r4, #1
	ldr	r1, .L1033+8
	str	r6, [r3, #3952]
	mov	r3, #0
	str	r2, [r5, #84]
	ble	.L1012
	ldr	r3, [r7, r4, asl #2]
	cmp	r3, #0
	beq	.L1008
	ldr	r2, [r3, #16]
	cmp	r2, #1
	bne	.L1008
	ldr	r2, [r3, #68]
	cmp	r2, #0
	ble	.L1014
	ldr	r9, [r3, #72]
	cmp	r9, #0
	beq	.L1031
.L1014:
	mov	r10, #200
	b	.L1019
.L1017:
	cmp	r0, #0
	bne	.L1008
	subs	r10, r10, #1
	beq	.L1008
.L1019:
	mov	r0, r4
	bl	DecChanSyntax
	cmn	r0, #1
	mov	r9, r0
	bne	.L1017
	ldr	ip, [r5, #2652]
	cmp	ip, #0
	beq	.L1025
	mov	r3, #0
	mov	r0, r4
	mov	r2, r3
	mov	r1, #102
	blx	ip
.L1025:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1031:
	mov	r0, r4
	sub	r1, fp, #68
	bl	SM_GetInfo
	cmp	r0, #0
	bne	.L1032
.L1015:
	ldr	r9, [r5, r4, asl #2]
	ldr	r1, [fp, #-56]
	ldr	r2, [r9, #68]
	ldr	r0, [r9, #544]
	bl	GetNextDecodeMode
	ldr	r3, [r5, r4, asl #2]
	str	r0, [r9, #544]
	ldr	r2, [r3, #544]
	cmp	r2, #0
	strne	r6, [r3, #540]
	strne	r6, [r3, #536]
	ldreq	r2, [r3, #64]
	streq	r2, [r3, #540]
	streq	r2, [r3, #536]
	b	.L1014
.L1012:
	mov	r0, r9
	str	r3, [r1, #84]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1032:
	mov	r1, r9
	sub	r0, fp, #68
	ldr	r3, [r8, #48]
	mov	r2, #24
	blx	r3
	b	.L1015
.L1034:
	.align	2
.L1033:
	.word	.LANCHOR0+4016
	.word	.LANCHOR0-4
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+3952
	UNWIND(.fnend)
	.size	DecSyntax, .-DecSyntax
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
	ldr	r3, .L1054
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L1052
	ldr	r2, [r3, #528]
	cmp	r2, #19
	ldrls	pc, [pc, r2, asl #2]
	b	.L1053
.L1038:
	.word	.L1051
	.word	.L1039
	.word	.L1040
	.word	.L1041
	.word	.L1042
	.word	.L1043
	.word	.L1044
	.word	.L1053
	.word	.L1045
	.word	.L1046
	.word	.L1047
	.word	.L1047
	.word	.L1047
	.word	.L1048
	.word	.L1053
	.word	.L1042
	.word	.L1049
	.word	.L1050
	.word	.L1053
	.word	.L1051
.L1051:
	add	r0, r3, #229376
	add	r0, r0, #768
	ldmfd	sp, {fp, sp, pc}
.L1039:
	add	r0, r3, #278528
	add	r0, r0, #88
	ldmfd	sp, {fp, sp, pc}
.L1040:
	add	r0, r3, #251904
	add	r0, r0, #840
	ldmfd	sp, {fp, sp, pc}
.L1041:
	movw	r0, #48504
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1042:
	add	r0, r3, #256000
	ldmfd	sp, {fp, sp, pc}
.L1043:
	movw	r0, #1800
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1044:
	add	r0, r3, #245760
	add	r0, r0, #584
	ldmfd	sp, {fp, sp, pc}
.L1045:
	add	r0, r3, #274432
	add	r0, r0, #1840
	ldmfd	sp, {fp, sp, pc}
.L1046:
	add	r0, r3, #274432
	add	r0, r0, #1824
	ldmfd	sp, {fp, sp, pc}
.L1047:
	add	r0, r3, #242688
	add	r0, r0, #96
	ldmfd	sp, {fp, sp, pc}
.L1048:
	add	r0, r3, #237568
	add	r0, r0, #736
	ldmfd	sp, {fp, sp, pc}
.L1049:
	add	r0, r3, #230400
	add	r0, r0, #320
	ldmfd	sp, {fp, sp, pc}
.L1050:
	add	r0, r3, #278528
	add	r0, r0, #2608
	ldmfd	sp, {fp, sp, pc}
.L1053:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1052:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L1055:
	.align	2
.L1054:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetChanVoQue, .-VCTRL_GetChanVoQue
	.align	2
	.global	VCTRL_FindOccupiedFrame
	.type	VCTRL_FindOccupiedFrame, %function
VCTRL_FindOccupiedFrame:
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
	bhi	.L1074
	bl	VCTRL_GetChanVoQue
	ldr	r3, .L1076
	ldr	r4, [r3, r4, asl #2]
	cmp	r0, #0
	beq	.L1067
	ldrh	r3, [r0, #4]
	ldrh	r1, [r0, #6]
	and	r3, r3, #63
	cmp	r1, r3
	beq	.L1067
	add	r2, r0, r3, lsl #2
	ldr	ip, [r2, #12]
	cmp	ip, #0
	beq	.L1061
	b	.L1059
.L1073:
	ldr	ip, [r2, #12]
	cmp	ip, #0
	bne	.L1059
.L1061:
	add	r3, r3, #1
	and	r3, r3, #63
	cmp	r3, r1
	add	r2, r0, r3, lsl #2
	bne	.L1073
.L1067:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1059:
	add	lr, r4, #228352
	add	r3, lr, #232
	add	lr, lr, #872
.L1064:
	ldrb	r2, [r3, #16]	@ zero_extendqisi2
	cmp	r2, #255
	beq	.L1063
	ldr	r1, [ip, #104]
	ldr	r2, [r3, #8]
	cmp	r1, r2
	bcc	.L1063
	ldr	r0, [r3, #12]
	add	r0, r2, r0
	cmp	r1, r0
	bcc	.L1075
.L1063:
	add	r3, r3, #20
	cmp	r3, lr
	bne	.L1064
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1075:
	add	r4, r4, #229376
	mov	r1, #1
	mov	r0, #0
	str	r2, [r4, #700]
	ldr	r2, [r3, #4]
	str	r2, [r4, #704]
	ldr	r3, [r3, #12]
	str	r1, [r4, #712]
	str	r3, [r4, #708]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1074:
	ldr	ip, .L1076+4
	movw	r2, #3880
	ldr	r3, .L1076+8
	mov	r0, #0
	ldr	r1, .L1076+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1077:
	.align	2
.L1076:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_FindOccupiedFrame, .-VCTRL_FindOccupiedFrame
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
	ldr	r4, .L1088
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	mov	r8, r3
	ldr	ip, [r4, r0, asl #2]
	cmp	ip, #0
	beq	.L1085
	bl	VCTRL_GetChanVoQue
	subs	r3, r0, #0
	beq	.L1080
	mov	r2, r8
	mov	r1, r7
	bl	GetQueueImgNum
	ldr	r3, [r4, r5, asl #2]
	ldr	r2, [r3, #528]
	cmp	r2, #0
	beq	.L1086
	cmp	r2, #17
	beq	.L1087
	cmp	r2, #16
	addeq	r3, r3, #11337728
	addeq	r3, r3, #12288
	movne	r3, #2
	ldreq	r3, [r3, #3136]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1086:
	add	r3, r3, #253952
	ldr	r3, [r3, #2816]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1080:
	str	r3, [r8]
	str	r3, [r7]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1087:
	add	r3, r3, #229376
	ldr	r3, [r3, #3248]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1085:
	ldr	lr, .L1088+4
	mov	r0, ip
	ldr	r3, .L1088+8
	movw	r2, #7477
	ldr	r1, .L1088+12
	ldr	ip, [lr, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L1089:
	.align	2
.L1088:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #44)
	sub	sp, sp, #44
	cmp	r0, #15
	mov	r7, r0
	mov	r5, r1
	bhi	.L1103
	cmp	r1, #0
	mov	r4, #0
	str	r4, [fp, #-64]
	str	r4, [fp, #-68]
	str	r4, [fp, #-72]
	beq	.L1090
	ldr	r8, .L1104
	mov	r1, r4
	ldr	r6, .L1104+4
	mov	r2, #108
	mov	r0, r5
	ldr	r3, [r8, #48]
	blx	r3
	ldr	r1, [r6, r7, asl #2]
	cmp	r1, r4
	beq	.L1090
	add	r1, r1, #1088
	ldr	r3, [r8, #52]
	add	r1, r1, #4
	mov	r2, #108
	mov	r0, r5
	blx	r3
	ldr	ip, [r6, r7, asl #2]
	movw	r3, #1466
	add	r0, ip, #204800
	ldrsb	r1, [r0, r3]
	cmp	r1, r4
	ble	.L1095
	movw	r3, #27198
	mov	r2, r4
	movt	r3, 3
	add	r3, ip, r3
.L1097:
	ldrsb	ip, [r3]
	add	r2, r2, #1
	add	r3, r3, #68
	cmp	ip, #3
	addeq	r4, r4, #1
	cmp	r2, r1
	bne	.L1097
.L1095:
	movw	r3, #1466
	str	r4, [r5, #92]
	ldrsb	ip, [r0, r3]
	sub	r2, fp, #68
	sub	r3, fp, #64
	sub	r1, fp, #72
	mov	r0, r7
	str	ip, [r5, #100]
	bl	VCTRL_GetChanImgNum
	mov	r3, #300
	mla	r3, r3, r7, r6
	ldr	ip, [fp, #-64]
	ldr	r2, [fp, #-68]
	sub	r1, fp, #60
	mov	r0, r7
	add	r2, r2, ip
	str	r2, [r5, #56]
	ldr	r2, [r3, #4036]
	str	r2, [r5, #104]
	ldr	r3, [r3, #4024]
	str	r3, [r5, #24]
	bl	SM_GetInfo
	ldr	r2, [fp, #-44]
	ldr	r3, [fp, #-52]
	sub	r1, fp, #60
	mov	r0, r7
	str	r2, [r5, #60]
	str	r3, [r5, #64]
	bl	SM_GetInfo
	ldr	r2, [fp, #-40]
	ldr	r3, [r6, r7, asl #2]
	cmp	r2, #10
	add	r3, r3, #245760
	movle	r2, #0
	movgt	r2, #1
	str	r2, [r5, #88]
	ldrb	r3, [r3, #240]	@ zero_extendqisi2
	and	r3, r3, #1
	str	r3, [r5, #96]
.L1090:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1103:
	ldr	ip, .L1104
	movw	r2, #1285
	ldr	r3, .L1104+8
	mov	r0, #0
	ldr	r1, .L1104+12
	ldr	ip, [ip, #68]
	blx	ip
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1105:
	.align	2
.L1104:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LC104
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
	bhi	.L1110
	ldr	r3, .L1112
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1111
	ldr	r0, [r0, #1272]
	ldmfd	sp, {fp, sp, pc}
.L1110:
	ldr	ip, .L1112+4
	movw	r2, #7525
	ldr	r3, .L1112+8
	mov	r0, #0
	ldr	r1, .L1112+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1111:
	ldr	ip, .L1112+4
	movw	r2, #7526
	ldr	r3, .L1112+16
	ldr	r1, .L1112+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1113:
	.align	2
.L1112:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
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
	bhi	.L1118
	ldr	r3, .L1120
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1119
	ldr	r0, [r0, #1276]
	ldmfd	sp, {fp, sp, pc}
.L1118:
	ldr	ip, .L1120+4
	movw	r2, #7533
	ldr	r3, .L1120+8
	mov	r0, #0
	ldr	r1, .L1120+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1119:
	ldr	ip, .L1120+4
	movw	r2, #7534
	ldr	r3, .L1120+16
	ldr	r1, .L1120+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1121:
	.align	2
.L1120:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
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
	bhi	.L1126
	ldr	r3, .L1127
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #40]
	moveq	r0, #33
	ldmfd	sp, {fp, sp, pc}
.L1126:
	ldr	ip, .L1127+4
	movw	r2, #7541
	ldr	r3, .L1127+8
	mov	r0, #0
	ldr	r1, .L1127+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1128:
	.align	2
.L1127:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
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
	ldr	r5, .L1136
	add	r3, r3, #8
	movw	r2, #10000
	cmp	r3, r2
	mov	r4, r1
	str	r0, [r5, #2720]
	bhi	.L1134
.L1130:
	movw	r1, #5000
	cmp	r0, r1
	movw	r2, #60536
	add	r3, r4, #4992
	movge	r0, r1
	movt	r2, 65535
	cmp	r0, r2
	add	r3, r3, #8
	movw	r1, #10000
	str	r4, [r5, #2724]
	movlt	r0, r2
	cmp	r3, r1
	str	r0, [r5, #2720]
	ldr	r6, .L1136
	bhi	.L1135
.L1131:
	movw	r2, #5000
	cmp	r4, r2
	movw	r3, #60536
	movge	r4, r2
	movt	r3, 65535
	cmp	r4, r3
	movlt	r4, r3
	str	r4, [r5, #2724]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1134:
	ldr	r3, .L1136+4
	mov	r2, r0
	ldr	r1, .L1136+8
	mov	r0, #32
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r0, [r5, #2720]
	b	.L1130
.L1135:
	ldr	r3, .L1136+4
	mov	r2, r4
	ldr	r1, .L1136+12
	mov	r0, #32
	ldr	r3, [r3, #68]
	blx	r3
	ldr	r4, [r6, #2724]
	b	.L1131
.L1137:
	.align	2
.L1136:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC105
	.word	.LC106
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
	ldr	r5, .L1167
	mov	r7, r0
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	ldr	r3, [r5, #12]
	mov	r0, #5
	blx	r3
	ldr	r4, [r7]
	cmn	r4, #1
	beq	.L1159
	mov	r0, r4
	bl	VCTRL_IsChanActive
	subs	r6, r0, #0
	bne	.L1160
	mov	r0, r4
	bl	VDMDRV_IsChanDec
	cmp	r0, #1
	beq	.L1161
	mov	r0, r4
	bl	VCTRL_GetDecParam
	subs	r2, r0, #0
	beq	.L1162
	cmp	r4, #15
	bhi	.L1154
	ldr	r3, .L1167+4
	ldr	r3, [r3, r4, asl #2]
	cmp	r3, #0
	beq	.L1154
	ldr	r1, [r3, #528]
	sub	r0, fp, #56
	ldr	r3, .L1167+8
	str	r4, [fp, #-56]
	str	r1, [fp, #-52]
	str	r2, [fp, #-48]
	ldr	r3, [r3, #632]
	blx	r3
	cmp	r0, #1
	bne	.L1156
	ldr	ip, [fp, #-56]
	mov	r0, r4
.L1157:
	ldr	r2, [fp, #-52]
	mov	r1, #4
	ldr	r3, [fp, #-48]
	str	ip, [r7]
	stmib	r7, {r2, r3}
	bl	VDMDRV_SetChanIsDec
	ldr	r3, [r5, #16]
	mov	r0, #5
	blx	r3
	ldr	r3, [r5, #16]
	mov	r0, r6
	blx	r3
.L1149:
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1159:
	ldr	r8, .L1167+12
	ldr	r9, [r8, #64]
	cmp	r9, #0
	blt	.L1140
	add	r4, r8, #64
	ldr	r10, .L1167+8
	add	r8, r8, #124
	b	.L1141
.L1163:
	mov	r3, r9
	ldr	ip, [r5, #68]
	ldr	r2, .L1167+16
	mov	r0, #1
	ldr	r1, .L1167+20
	blx	ip
.L1143:
	cmp	r4, r8
	beq	.L1140
.L1150:
	ldr	r9, [r4, #4]!
	cmp	r9, #0
	blt	.L1140
.L1141:
	mov	r0, r9
	bl	VCTRL_IsChanActive
	subs	r6, r0, #0
	bne	.L1163
	mov	r0, r9
	bl	VDMDRV_IsChanDec
	cmp	r0, #1
	beq	.L1164
	mov	r0, r9
	bl	VCTRL_GetDecParam
	subs	r1, r0, #0
	beq	.L1165
	cmp	r9, #15
	bhi	.L1146
	ldr	r3, .L1167+4
	ldr	r2, [r3, r9, asl #2]
	cmp	r2, #0
	beq	.L1146
	ldr	r2, [r2, #528]
	sub	r0, fp, #56
	str	r9, [fp, #-56]
	str	r1, [fp, #-48]
	str	r2, [fp, #-52]
	ldr	r2, [r10, #632]
	blx	r2
	cmp	r0, #1
	beq	.L1166
	cmp	r4, r8
	str	r6, [r7, #8]
	bne	.L1150
.L1140:
	ldr	r3, [r5, #16]
	mov	r0, #5
	blx	r3
	ldr	r3, [r5, #16]
	mov	r0, #0
	mvn	r6, #0
	blx	r3
	mov	r0, r6
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1146:
	mov	r3, r9
	ldr	ip, [r5, #68]
	movw	r2, #7663
	ldr	r1, .L1167+24
	mov	r0, #1
	blx	ip
	b	.L1143
.L1164:
	mov	r2, r9
	ldr	r3, [r5, #68]
	ldr	r1, .L1167+28
	mov	r0, #22
	blx	r3
	b	.L1143
.L1161:
	mov	r2, r4
	ldr	r3, [r5, #68]
	ldr	r1, .L1167+28
	mov	r0, #22
	blx	r3
.L1156:
	str	r6, [r7, #8]
	mov	r0, #5
	ldr	r3, [r5, #16]
	blx	r3
	ldr	r3, [r5, #16]
	mov	r0, r6
	mvn	r6, #0
	blx	r3
	b	.L1149
.L1165:
	mov	r2, r9
	ldr	r3, [r5, #68]
	ldr	r1, .L1167+32
	mov	r0, #22
	blx	r3
	b	.L1143
.L1166:
	ldr	ip, [fp, #-56]
	mov	r0, r9
	b	.L1157
.L1160:
	ldr	ip, [r5, #68]
	mov	r3, r4
	ldr	r2, .L1167+16
	mov	r0, #1
	ldr	r1, .L1167+36
.L1158:
	blx	ip
	mov	r4, #0
	mov	r0, #5
	str	r4, [r7, #8]
	ldr	r3, [r5, #16]
	mvn	r6, #0
	blx	r3
	mov	r0, r4
	ldr	r3, [r5, #16]
	blx	r3
	b	.L1149
.L1154:
	mov	r3, r4
	ldr	ip, [r5, #68]
	movw	r2, #7748
	ldr	r1, .L1167+24
	mov	r0, #1
	b	.L1158
.L1162:
	ldr	r3, [r5, #128]
	blx	r3
	mov	r2, r4
	ldr	r3, [r5, #68]
	mov	r0, #22
	ldr	r1, .L1167+32
	blx	r3
	b	.L1156
.L1168:
	.align	2
.L1167:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	.LANCHOR3
	.word	g_ChanCtx
	.word	.LANCHOR1+212
	.word	.LC107
	.word	.LC53
	.word	.LC108
	.word	.LC109
	.word	.LC110
	UNWIND(.fnend)
	.size	VCTRL_ExtAccGetDecParam, .-VCTRL_ExtAccGetDecParam
	.align	2
	.global	VCTRL_MaskAllInt
	.type	VCTRL_MaskAllInt, %function
VCTRL_MaskAllInt:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L1173
	ldr	r3, [r3, #72]
	cmp	r3, #0
	beq	.L1172
	mov	r0, #0
	blx	r3
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	SCDDRV_MaskInt
.L1172:
	ldr	ip, .L1173+4
	mov	r0, r3
	movw	r2, #7818
	ldr	r1, .L1173+8
	ldr	r3, [ip, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	SCDDRV_MaskInt
.L1174:
	.align	2
.L1173:
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC16
	UNWIND(.fnend)
	.size	VCTRL_MaskAllInt, .-VCTRL_MaskAllInt
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
	ldr	r4, .L1190
	mov	r3, #100
	ldr	r5, .L1190+4
	ldr	r6, .L1190+8
	ldr	r0, [r4, #636]
	str	r3, [r5, #80]
	cmp	r0, #0
	ble	.L1176
	ldr	r3, [r6, #120]
	blx	r3
.L1176:
	ldr	r3, [r6, #12]
	mov	r0, #0
	blx	r3
	bl	VCTRL_MaskAllInt
	bl	SM_Wakeup
	ldr	r3, [r4, #640]
	mov	r2, #101
	str	r2, [r5, #80]
	cmp	r3, #0
	movne	r7, #0
	beq	.L1188
.L1177:
	mov	r3, #102
	str	r3, [r5, #80]
	bl	VDMDRV_WakeUpVdm
	mov	r0, #0
	bl	SCDDRV_EnableInt
	ldr	r3, .L1190+12
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq	.L1189
	mov	r0, #0
	blx	r3
.L1179:
	mov	r4, #0
.L1180:
	mov	r0, r4
	mov	r1, #0
	add	r4, r4, #1
	bl	DeleteRawPacketInBuffer
	cmp	r4, #16
	bne	.L1180
	ldr	r3, [r6, #16]
	mov	r0, #0
	blx	r3
	mov	r2, #105
	ldr	r3, [r6, #132]
	mov	r1, #10
	str	r2, [r5, #80]
	mov	r0, #0
	blx	r3
	mov	r0, r7
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1188:
	bl	DecSyntax
	mov	r7, r0
	b	.L1177
.L1189:
	mov	r0, r3
	movw	r2, #623
	ldr	r3, [r6, #68]
	ldr	r1, .L1190+16
	blx	r3
	b	.L1179
.L1191:
	.align	2
.L1190:
	.word	.LANCHOR3
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	g_vdm_hal_fun_ptr
	.word	.LC16
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
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L1222
	ldr	r5, .L1222+4
	mov	r6, r4
	mov	r7, r5
.L1193:
.L1200:
	mov	r2, #0
	ldr	r3, [r5, #68]
	str	r2, [r4, #80]
	mov	r0, #2
	ldr	r1, .L1222+8
	blx	r3
	ldr	r3, [r4, #72]
	mov	r2, #2
	str	r2, [r4, #68]
	cmp	r3, r2
	beq	.L1198
	cmp	r3, #3
	bne	.L1218
	b	.L1197
.L1220:
	cmp	r3, #3
	beq	.L1197
	ldr	r3, [r6, #68]
	cmp	r3, #0
	beq	.L1197
.L1218:
	mov	r3, #2
	str	r3, [r4, #80]
	bl	VCTRL_RunProcess
	ldr	r3, [r5, #68]
	ldr	r1, .L1222+12
	mov	r0, #22
	blx	r3
	ldr	r3, [r4, #72]
	cmp	r3, #2
	bne	.L1220
.L1198:
	mov	r2, #4
	ldr	r3, [r5, #68]
	str	r2, [r4, #80]
	mov	r0, #2
	ldr	r1, .L1222+16
	mov	r8, #5
	blx	r3
	mov	r3, #1
	str	r3, [r4, #68]
	b	.L1195
.L1221:
	ldr	r3, [r7, #120]
	blx	r3
	ldr	r3, [r6, #68]
	cmp	r3, #0
	beq	.L1197
.L1195:
	ldr	r3, [r5, #68]
	mov	r0, #22
	str	r8, [r4, #80]
	ldr	r1, .L1222+20
	blx	r3
	ldr	r3, [r4, #72]
	mov	r0, #10
	cmp	r3, #1
	beq	.L1200
	cmp	r3, #3
	bne	.L1221
.L1197:
	mov	r2, #6
	ldr	r3, [r5, #68]
	ldr	r1, .L1222+24
	mov	r0, #2
	str	r2, [r4, #80]
	blx	r3
	mov	r0, #0
	str	r0, [r4, #68]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1223:
	.align	2
.L1222:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC111
	.word	.LC114
	.word	.LC112
	.word	.LC115
	.word	.LC113
	UNWIND(.fnend)
	.size	VFMW_CTRL_thread_proc, .-VFMW_CTRL_thread_proc
	.align	2
	.global	VCTRL_EnableAllInt
	.type	VCTRL_EnableAllInt, %function
VCTRL_EnableAllInt:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L1228
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq	.L1227
	mov	r0, #0
	blx	r3
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	SCDDRV_EnableInt
.L1227:
	ldr	ip, .L1228+4
	mov	r0, r3
	movw	r2, #7857
	ldr	r1, .L1228+8
	ldr	r3, [ip, #68]
	blx	r3
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	SCDDRV_EnableInt
.L1229:
	.align	2
.L1228:
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC16
	UNWIND(.fnend)
	.size	VCTRL_EnableAllInt, .-VCTRL_EnableAllInt
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
	bhi	.L1234
	ldr	r3, .L1235
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #320]
	mvneq	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1234:
	ldr	ip, .L1235+4
	movw	r2, #8651
	ldr	r3, .L1235+8
	mov	r0, #0
	ldr	r1, .L1235+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1236:
	.align	2
.L1235:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
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
	bhi	.L1243
	ldr	r3, .L1244
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	strne	r1, [r3, #320]
	ldmfd	sp, {fp, sp, pc}
.L1243:
	ldr	ip, .L1244+4
	movw	r2, #8663
	ldr	r3, .L1244+8
	mov	r0, #0
	ldr	r1, .L1244+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	bx	ip
.L1245:
	.align	2
.L1244:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
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
	bhi	.L1252
	ldr	r3, .L1253
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	strne	r1, [r3, #324]
	ldmfd	sp, {fp, sp, pc}
.L1252:
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VCTRL_SetLastFrameState.part.6
.L1254:
	.align	2
.L1253:
	.word	.LANCHOR0
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
	UNWIND(.pad #12)
	sub	sp, sp, #12
	subs	r6, r1, #0
	mov	r4, r0
	moveq	r2, #2
	beq	.L1257
	cmp	r6, #1
	moveq	r2, #3
	movne	r2, #4
.L1257:
	cmp	r4, #15
	bhi	.L1269
	ldr	r5, .L1270
	ldr	r3, [r5, r4, asl #2]
	cmp	r3, #0
	strne	r2, [r3, #324]
.L1260:
	ldr	ip, [r5, #2652]
	cmp	ip, #0
	beq	.L1261
	sub	r2, fp, #28
	mov	r3, #4
	mov	r1, #115
	mov	r0, r4
	str	r6, [r2, #-4]!
	blx	ip
.L1261:
	ldr	r3, [r5, r4, asl #2]
	mov	r2, r6
	ldr	ip, .L1270+4
	mov	r0, #1
	ldr	r1, .L1270+8
	ldr	r3, [r3, #324]
	ldr	ip, [ip, #68]
	blx	ip
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1269:
	bl	VCTRL_SetLastFrameState.part.6
	ldr	r5, .L1270
	b	.L1260
.L1271:
	.align	2
.L1270:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC116
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
	ldr	r3, .L1297
	mov	r5, r0
	ldr	r3, [r3, r0, asl #2]
	add	r0, r3, #229376
	ldr	r2, [r0, #752]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L1291
.L1275:
	.word	.L1274
	.word	.L1276
	.word	.L1277
	.word	.L1278
	.word	.L1291
	.word	.L1279
	.word	.L1280
	.word	.L1291
	.word	.L1281
	.word	.L1282
	.word	.L1283
	.word	.L1283
	.word	.L1283
	.word	.L1284
	.word	.L1291
	.word	.L1291
	.word	.L1285
	.word	.L1286
.L1286:
	add	r0, r0, #760
	bl	HEVCDEC_GetRemainImg
	mov	r4, r0
.L1273:
	cmn	r4, #1
	beq	.L1288
	cmp	r4, #0
	bne	.L1296
	mov	r0, r5
	mov	r1, r4
	bl	VCTRL_ReportLastFrame
	ldr	r3, .L1297+4
	ldr	r1, .L1297+8
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1274:
	add	r0, r0, #760
	bl	H264DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1276:
	add	r0, r0, #760
	bl	VC1DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1277:
	add	r0, r0, #760
	bl	MPEG4DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1278:
	add	r0, r0, #760
	bl	MPEG2DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1279:
	add	r0, r0, #760
	bl	DIVX3DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1280:
	add	r0, r0, #760
	bl	AVSDEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1281:
	add	r0, r0, #760
	bl	REAL8DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1282:
	add	r0, r0, #760
	bl	REAL9DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1283:
	add	r0, r0, #760
	bl	VP6DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1284:
	add	r0, r0, #760
	bl	VP8DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1285:
	add	r0, r0, #760
	bl	MVC_DEC_GetRemainImg
	mov	r4, r0
	b	.L1273
.L1296:
	mov	r0, r5
	mov	r1, r4
	bl	VCTRL_ReportLastFrame
	ldr	r3, .L1297+4
	sub	r2, r4, #2
	ldr	r1, .L1297+12
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1291:
	mvn	r4, #0
.L1288:
	mov	r0, r5
	mov	r1, #1
	bl	VCTRL_ReportLastFrame
	ldr	r3, .L1297+4
	ldr	r1, .L1297+16
	mov	r0, #2
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1298:
	.align	2
.L1297:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC118
	.word	.LC119
	.word	.LC117
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
	ldr	r5, .L1333
	mov	r4, r0
	ldr	lr, [fp, #4]
	ldr	r0, [r5, r0, asl #2]
	cmp	r0, #0
	beq	.L1327
	ldr	ip, [r0, #528]
	ldr	r6, [r0, #1232]
	cmp	ip, #17
	ldrls	pc, [pc, ip, asl #2]
	b	.L1299
.L1303:
	.word	.L1302
	.word	.L1304
	.word	.L1305
	.word	.L1306
	.word	.L1299
	.word	.L1307
	.word	.L1308
	.word	.L1299
	.word	.L1309
	.word	.L1310
	.word	.L1311
	.word	.L1311
	.word	.L1311
	.word	.L1312
	.word	.L1299
	.word	.L1299
	.word	.L1313
	.word	.L1314
.L1302:
	add	r0, r0, #229376
	str	lr, [sp, #4]
	add	r0, r0, #760
	str	r6, [sp]
	mov	r2, #1
	ldr	r7, .L1333
	bl	H264DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #241664
	ldrb	r3, [r3, #2032]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1328
.L1315:
	cmp	r6, #1
	ldrne	r3, [r5, r4, asl #2]
	movne	r2, #0
	addne	r3, r3, #229376
	strne	r2, [r3, #1120]
.L1299:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1314:
	add	r0, r0, #229376
	str	lr, [sp, #4]
	add	r0, r0, #760
	str	r6, [sp]
	mov	r2, #1
	ldr	r7, .L1333
	bl	HEVCDEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #229376
	ldrb	r3, [r3, #766]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1329
.L1317:
	cmp	r6, #1
	ldrne	r3, [r5, r4, asl #2]
	movne	r2, #0
	addne	r3, r3, #229376
	strne	r2, [r3, #3072]
	b	.L1299
.L1304:
	add	r0, r0, #229376
	ldr	r6, .L1333
	add	r0, r0, #760
	bl	VC1DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r2, r3, #241664
	ldrb	r2, [r2, #1191]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1330
.L1322:
	add	r3, r3, #229376
	mov	r2, #0
	str	r2, [r3, #764]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1305:
	add	r0, r0, #229376
	mov	r2, r3
	add	r0, r0, #760
	bl	MPEG4DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	ldr	r5, .L1333
	add	r2, r3, #286720
	ldrb	r2, [r2, #224]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1331
.L1320:
	add	r3, r3, #229376
	mov	r2, #0
	str	r2, [r3, #1796]
	b	.L1299
.L1306:
	add	r0, r0, #229376
	ldr	r6, .L1333
	add	r0, r0, #760
	bl	MPEG2DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r2, r3, #344064
	ldrb	r2, [r2, #762]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1322
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #344064
	strb	r1, [r2, #762]
	b	.L1322
.L1307:
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	DIVX3DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #229376
	str	r2, [r3, #764]
	b	.L1299
.L1308:
	add	r0, r0, #229376
	ldr	r6, .L1333
	add	r0, r0, #760
	bl	AVSDEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r2, r3, #245760
	ldrb	r2, [r2, #1620]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1322
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #245760
	strb	r1, [r2, #1620]
	b	.L1322
.L1309:
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	REAL8DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #229376
	str	r2, [r3, #764]
	b	.L1299
.L1310:
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	REAL9DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #229376
	str	r2, [r3, #764]
	b	.L1299
.L1311:
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	VP6DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #229376
	str	r2, [r3, #764]
	b	.L1299
.L1312:
	add	r0, r0, #229376
	add	r0, r0, #760
	bl	VP8DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #229376
	str	r2, [r3, #764]
	b	.L1299
.L1313:
	add	r0, r0, #229376
	mov	r2, #1
	add	r0, r0, #760
	bl	MVC_DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	ldr	r5, .L1333
	add	r3, r3, #229376
	ldrb	r2, [r3, #761]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1332
.L1323:
	mov	r2, #0
	str	r2, [r3, #988]
	b	.L1299
.L1327:
	ldr	lr, .L1333+4
	movw	r2, #4714
	ldr	r3, .L1333+8
	ldr	r1, .L1333+12
	ldr	ip, [lr, #68]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	ip
.L1331:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r5, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #286720
	strb	r1, [r2, #224]
	b	.L1320
.L1330:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #241664
	strb	r1, [r2, #1191]
	b	.L1322
.L1332:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r2, [r5, r4, asl #2]
	mov	r1, #0
	add	r2, r2, #229376
	mov	r3, r2
	strb	r1, [r2, #761]
	b	.L1323
.L1328:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #241664
	strb	r2, [r3, #2032]
	b	.L1315
.L1329:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #229376
	strb	r2, [r3, #766]
	b	.L1317
.L1334:
	.align	2
.L1333:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
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
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
	mov	r7, r3
	ldr	r8, [fp, #4]
	bhi	.L1341
	ldr	ip, .L1343
	ldr	ip, [ip, r0, asl #2]
	cmp	ip, #0
	beq	.L1342
	ldr	ip, [ip, #16]
	cmp	ip, #1
	beq	.L1338
	ldr	ip, .L1343+4
	movw	r2, #4894
	ldr	r3, .L1343+8
.L1340:
	mov	r0, #0
	ldr	r1, .L1343+12
	ldr	ip, [ip, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L1338:
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
.L1341:
	ldr	ip, .L1343+4
	movw	r2, #4892
	ldr	r3, .L1343+16
	b	.L1340
.L1342:
	ldr	lr, .L1343+4
	mov	r0, ip
	ldr	r3, .L1343+20
	movw	r2, #4893
	ldr	r1, .L1343+12
	ldr	ip, [lr, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	ip
.L1344:
	.align	2
.L1343:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC59
	.word	.LC1
	.word	.LC58
	.word	.LC45
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
	ldr	r4, .L1346
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
.L1347:
	.align	2
.L1346:
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
	b	.L1350
.L1352:
	.word	.L1351
	.word	.L1353
	.word	.L1354
	.word	.L1355
	.word	.L1356
	.word	.L1357
	.word	.L1358
.L1358:
	cmp	r0, #1
	moveq	r0, #1
	movne	r0, #7
	ldmfd	sp, {fp, sp, pc}
.L1351:
	cmp	r0, #1
	moveq	r0, #0
	movne	r0, #6
	ldmfd	sp, {fp, sp, pc}
.L1353:
	cmp	r0, #1
	moveq	r0, #2
	movne	r0, #8
	ldmfd	sp, {fp, sp, pc}
.L1354:
	cmp	r0, #1
	moveq	r0, #4
	movne	r0, #10
	ldmfd	sp, {fp, sp, pc}
.L1355:
	cmp	r0, #1
	moveq	r0, #3
	movne	r0, #9
	ldmfd	sp, {fp, sp, pc}
.L1356:
	cmp	r0, #1
	moveq	r0, #5
	movne	r0, #11
	ldmfd	sp, {fp, sp, pc}
.L1357:
	cmp	r0, #1
	moveq	r0, #13
	movne	r0, #12
	ldmfd	sp, {fp, sp, pc}
.L1350:
	ldr	ip, .L1367
	mov	r3, r1
	movw	r2, #8881
	ldr	r1, .L1367+4
	mov	r0, #1
	ldr	ip, [ip, #68]
	blx	ip
	mov	r0, #13
	ldmfd	sp, {fp, sp, pc}
.L1368:
	.align	2
.L1367:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC120
	UNWIND(.fnend)
	.size	Get_YuvFormat, .-Get_YuvFormat
	.align	2
	.global	VDH_PutUsrdecFrame
	.type	VDH_PutUsrdecFrame, %function
VDH_PutUsrdecFrame:
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
	mov	r5, r1
	ldr	r1, [r1, #28]
	mov	r8, r0
	bl	FSP_GetLogicIdByPhyAddr
	cmn	r0, #1
	mov	r7, r0
	beq	.L1377
	ldr	r3, [r5, #4]
	cmp	r3, #1
	beq	.L1372
	mov	r1, r0
	mov	r2, #0
	mov	r0, r8
	bl	FSP_ClearLogicFs
	mov	r0, #0
.L1371:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1372:
	ldr	r0, [r5, #36]
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L1378
.L1373:
	mov	r1, r7
	mov	r0, r8
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L1379
	str	r7, [r4, #196]
	mov	r6, #0
	ldrd	r2, [r5, #8]
	mov	r9, #1
	strd	r2, [r4]
	ldrd	r2, [r5, #8]
	strd	r2, [r4, #8]
	ldrd	r2, [r5, #8]
	strd	r2, [r4, #16]
	ldrd	r2, [r5, #8]
	str	r9, [r4, #68]
	str	r6, [r4, #72]
	strd	r2, [r4, #24]
	str	r6, [r4, #76]
	str	r6, [r4, #80]
	ldr	r3, [r5, #28]
	str	r3, [r4, #88]
	mov	r0, r3
	str	r3, [r4, #120]
	str	r3, [r4, #104]
	ldr	r3, [r5, #36]
	str	r3, [r4, #92]
	str	r3, [r4, #124]
	str	r3, [r4, #108]
	bl	MEM_Phy2Vir
	mov	r3, r0
	ldr	r0, [r4, #92]
	str	r3, [r4, #48]
	str	r3, [r4, #32]
	bl	MEM_Phy2Vir
	ldr	r3, [r4, #88]
	movw	r1, #11296
	str	r0, [r4, #52]
	str	r0, [r4, #36]
	ldr	r2, [r5, #32]
	ldr	r0, [r4, #92]
	add	r2, r2, r3
	str	r2, [r4, #96]
	ldr	r3, [r5, #32]
	str	r6, [r4, #148]
	add	r3, r3, r0
	str	r3, [r4, #100]
	ldr	r3, [r5, #20]
	str	r3, [r4, #164]
	ldr	r3, [r5, #24]
	str	r3, [r4, #168]
	ldr	r3, [r5, #20]
	str	r3, [r4, #172]
	mov	r2, r3, lsr r9
	ldr	r3, [r5, #24]
	str	r2, [r4, #180]
	str	r3, [r4, #176]
	mov	r3, r3, lsr r9
	str	r3, [r4, #184]
	ldr	r3, [r5, #32]
	str	r6, [r4, #188]
	str	r6, [r4, #200]
	str	r3, [r4, #192]
	str	r9, [r4, #228]
	str	r6, [r4, #276]
	str	r6, [r4, #56]
	str	r1, [r4, #160]
	ldr	r1, [r5]
	ldr	r0, [r5, #16]
	bl	Get_YuvFormat
	cmp	r0, #13
	beq	.L1380
	str	r0, [r4, #476]
	mov	r2, r9
	ldr	r3, [r5, #44]
	mov	r9, #19
	mov	r1, r7
	mov	r0, r8
	str	r3, [r4, #452]
	ldr	r3, [r5, #48]
	str	r3, [r4, #448]
	ldr	r3, [r5, #40]
	str	r9, [r4, #384]
	str	r3, [r4, #456]
	bl	FSP_SetDisplay
	ldr	r3, .L1381
	str	r4, [sp]
	mov	r1, r9
	mov	r0, r8
	ldr	r2, [r3, r8, asl #2]
	add	r2, r2, #229376
	add	r3, r2, #768
	add	r2, r2, #760
	bl	InsertImgToVoQueue
	cmp	r0, #1
	moveq	r0, r6
	beq	.L1371
	ldr	r1, [r4, #212]
	mov	r0, r8
	bl	FreeUsdByDec
	ldr	r1, [r4, #216]
	mov	r0, r8
	str	r6, [r4, #212]
	bl	FreeUsdByDec
	mov	r0, r8
	str	r6, [r4, #216]
	mov	r2, r6
	mov	r1, r7
	bl	FSP_SetDisplay
	mvn	r0, #0
	b	.L1371
.L1378:
	ldr	r3, [r5, #4]
	cmp	r3, #1
	bne	.L1373
	ldr	r3, .L1381+4
	ldr	r2, [r5, #36]
	ldr	r1, .L1381+8
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1371
.L1377:
	ldr	r3, .L1381+4
	mov	r0, #0
	ldr	r2, [r5, #28]
	ldr	r1, .L1381+12
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r7
	b	.L1371
.L1380:
	ldr	r3, .L1381+4
	mov	r2, r0
	ldr	r1, .L1381+16
	mov	r0, r9
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1371
.L1379:
	ldr	r3, .L1381+4
	movw	r2, #5338
	ldr	r1, .L1381+20
	mov	r0, #22
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1371
.L1382:
	.align	2
.L1381:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC122
	.word	.LC121
	.word	.LC124
	.word	.LC123
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r3, .L1394
	mov	r4, r0
	mov	r5, r1
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1391
	cmp	r1, #0
	beq	.L1392
	ldr	r2, [r0, #528]
	cmp	r2, #19
	bne	.L1393
	ldr	r6, .L1394+4
	mov	r2, #56
	add	r0, r0, #472
	ldr	r3, [r6, #52]
	blx	r3
	mov	r1, r5
	mov	r0, r4
	bl	VDH_PutUsrdecFrame
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, [r6, #68]
	mov	r0, #1
	ldr	r1, .L1394+8
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1393:
	ldr	r3, .L1394+4
	mov	r0, #1
	ldr	r1, .L1394+12
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1392:
	ldr	ip, .L1394+4
	mov	r0, r1
	ldr	r3, .L1394+16
	movw	r2, #5607
	ldr	r1, .L1394+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1391:
	ldr	ip, .L1394+4
	movw	r2, #5606
	ldr	r3, .L1394+24
	ldr	r1, .L1394+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1395:
	.align	2
.L1394:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC127
	.word	.LC126
	.word	.LC125
	.word	.LC1
	.word	.LC2
	UNWIND(.fnend)
	.size	VCTRL_PutUsrdecFrame, .-VCTRL_PutUsrdecFrame
	.align	2
	.global	VCTRL_OpenHardware
	.type	VCTRL_OpenHardware, %function
VCTRL_OpenHardware:
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
	bhi	.L1433
	ldr	r7, .L1436
	ldr	r5, [r7, r0, asl #2]
	cmp	r5, #0
	beq	.L1434
	ldr	r3, [r5, #12]
	cmp	r3, #1
	bne	.L1399
	ldr	r3, [r5, #528]
	cmp	r3, #20
	beq	.L1399
	bic	r4, r3, #8
	cmp	r3, #4
	cmpne	r4, #7
	moveq	r4, #1
	movne	r4, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r3, #19
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r8, r4
.L1405:
	mov	r0, r4
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L1404
	ldr	r3, [r7, r4, asl #2]
	ldr	r3, [r3, #528]
	cmp	r3, #19
	cmpne	r3, #4
	bic	r3, r3, #8
	movne	r2, #1
	moveq	r2, #0
	cmp	r3, #7
	moveq	r3, #0
	andne	r3, r2, #1
	cmp	r3, #0
	addne	r8, r8, #1
.L1404:
	add	r4, r4, #1
	cmp	r4, #16
	bne	.L1405
	cmp	r8, #1
	beq	.L1406
	ldr	r3, .L1436+4
	ldr	r4, [r3]
.L1407:
	ldr	r1, .L1436+8
	mov	ip, #0
	add	lr, r1, #64
.L1412:
	ldr	r2, [r1, #4]!
	mov	r3, #0
	cmp	r2, r3
	beq	.L1410
	ldr	r0, [r2, #528]
	cmp	r0, r3
	ldreq	r2, [r2, #1236]
	subeq	r3, r2, #1
	clzeq	r3, r3
	moveq	r3, r3, lsr #5
	cmp	r0, #6
	orreq	r3, r3, #1
	cmp	r3, #0
	movne	ip, #1
.L1410:
	cmp	r1, lr
	bne	.L1412
	ldr	r3, [r4, #120]
	cmp	ip, #1
	orreq	r3, r3, #2
	bfcne	r3, #1, #1
	str	r3, [r4, #120]
	ldr	r3, [r5, #528]
	cmp	r3, #1
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	ldr	r3, .L1436+12
	ldr	r3, [r3, #24]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L1399:
	ldr	r3, .L1436+16
	movw	r2, #8915
	ldr	r1, .L1436+20
	mov	r0, #0
.L1432:
	ldr	r3, [r3, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L1433:
	ldr	r3, .L1436+16
	movw	r2, #8903
	ldr	r1, .L1436+24
	mov	r0, #0
	ldr	r3, [r3, #68]
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3
.L1406:
	ldr	r3, .L1436+12
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L1435
	mov	r0, #0
	blx	r3
.L1409:
	ldr	r3, .L1436+4
	mov	r2, #25
	ldr	r5, [r7, r6, asl #2]
	ldr	r4, [r3]
	ldr	r3, [r4, #120]
	bfi	r3, r2, #12, #5
	bfc	r3, #17, #1
	str	r3, [r4, #120]
	ldr	r3, [r4, #120]
	orr	r3, r3, #131072
	str	r3, [r4, #120]
	b	.L1407
.L1434:
	ldr	r3, .L1436+16
	mov	r0, r5
	movw	r2, #8909
	ldr	r1, .L1436+28
	b	.L1432
.L1435:
	ldr	ip, .L1436+16
	mov	r0, r3
	movw	r2, #8951
	ldr	r1, .L1436+32
	ldr	r3, [ip, #68]
	blx	r3
	b	.L1409
.L1437:
	.align	2
.L1436:
	.word	.LANCHOR0
	.word	g_pstRegCrg
	.word	.LANCHOR0-4
	.word	g_vdm_hal_fun_ptr
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC130
	.word	.LC128
	.word	.LC129
	.word	.LC16
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
	bhi	.L1450
	ldr	r4, .L1454
	ldr	r3, [r4, r0, asl #2]
	cmp	r3, #0
	beq	.L1451
	ldr	r6, [r3, #12]
	cmp	r6, #1
	bne	.L1452
	ldr	r3, [r3, #528]
	cmp	r3, #20
	beq	.L1453
	bl	SM_Start
	ldr	r3, [r4, r5, asl #2]
	mov	r2, #0
	str	r6, [r3, #16]
	ldr	r3, [r4, r5, asl #2]
	str	r2, [r3, #28]
	ldr	r3, [r4, r5, asl #2]
	str	r6, [r3, #20]
	ldr	r2, [r4, r5, asl #2]
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bne	.L1444
	ldr	r3, .L1454+4
	sub	ip, r4, #4
	ldr	r1, .L1454+8
	add	r0, r3, #64
	str	r6, [r2, #24]
.L1447:
	ldr	r2, [ip, #4]!
	add	r3, r3, #4
	add	r1, r1, #4
	cmp	r2, #0
	streq	r2, [r3, #-4]
	ldrne	lr, [r2, #24]
	strne	lr, [r3, #-4]
	ldrne	r2, [r2, #24]
	cmp	r3, r0
	str	r2, [r1, #-4]
	bne	.L1447
	ldr	r0, .L1454+4
	bl	VDMDRV_SetPriority
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
.L1444:
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r5
	bl	VCTRL_OpenHardware
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1452:
	ldr	ip, .L1454+12
	movw	r2, #2621
	ldr	r3, .L1454+16
.L1449:
	ldr	r1, .L1454+20
	mov	r0, #0
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1450:
	ldr	ip, .L1454+12
	movw	r2, #2619
	ldr	r3, .L1454+24
	b	.L1449
.L1453:
	ldr	ip, .L1454+12
	movw	r2, #2622
	ldr	r3, .L1454+28
	b	.L1449
.L1451:
	ldr	ip, .L1454+12
	mov	r0, r3
	movw	r2, #2620
	ldr	r3, .L1454+32
	ldr	r1, .L1454+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1455:
	.align	2
.L1454:
	.word	.LANCHOR0
	.word	.LANCHOR3+644
	.word	g_ChanCtx
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC61
	.word	.LC1
	.word	.LC6
	.word	.LC131
	.word	.LC27
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r1, .L1494
	mov	r5, #0
	mov	r4, r5
	mov	r0, r5
	add	lr, r1, #64
	b	.L1462
.L1488:
	ldr	ip, [r3, #12]
	cmp	ip, #1
	beq	.L1486
.L1459:
	cmp	r2, #0
	ldreq	ip, [r3, #1236]
	movne	ip, #0
	cmp	r5, #0
	bne	.L1457
	cmp	r2, #6
	cmpne	ip, #1
	bne	.L1457
	ldr	r2, [r3, #12]
	cmp	r2, #1
	ldreq	r5, [r3, #16]
	subeq	r5, r5, #1
	clzeq	r5, r5
	moveq	r5, r5, lsr #5
.L1457:
	cmp	r1, lr
	beq	.L1487
.L1462:
	ldr	r3, [r1, #4]!
	cmp	r3, #0
	beq	.L1457
	cmp	r0, #0
	ldr	r2, [r3, #528]
	beq	.L1488
	cmp	r4, #0
	cmpeq	r2, #1
	bne	.L1459
	ldr	ip, [r3, #12]
	cmp	ip, #1
	bne	.L1459
	ldr	ip, [r3, #16]
.L1469:
	cmp	ip, #1
	moveq	r4, #1
	b	.L1459
.L1486:
	ldr	ip, [r3, #16]
	cmp	ip, #1
	beq	.L1489
.L1460:
	cmp	r4, #0
	cmpeq	r2, #1
	beq	.L1469
	b	.L1459
.L1487:
	cmp	r0, #0
	beq	.L1490
.L1463:
	cmp	r5, #0
	bne	.L1466
	ldr	r3, .L1494+4
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L1491
	mov	r0, r5
	blx	r3
.L1466:
	cmp	r4, #0
	ldmnefd	sp, {r4, r5, fp, sp, pc}
	ldr	r3, .L1494+4
	ldr	r3, [r3, #28]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, lr}
	bx	r3
.L1490:
	ldr	r3, .L1494+4
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1492
	blx	r3
.L1465:
	ldr	r1, .L1494+8
	mov	r3, #0
	ldr	r2, .L1494+12
	str	r3, [r1]
	str	r3, [r2, #84]
	b	.L1463
.L1489:
	sub	r0, r2, #5
	cmp	r0, #1
	cmphi	r2, #3
	bhi	.L1493
.L1471:
	mov	r0, ip
	b	.L1460
.L1493:
	sub	r0, r2, #8
	cmp	r0, #5
	bls	.L1471
	sub	r0, r2, #16
	cmp	r0, #1
	movhi	r0, #0
	movls	r0, #1
	b	.L1460
.L1492:
	ldr	r3, .L1494+16
	movw	r2, #9076
	ldr	r1, .L1494+20
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1465
.L1491:
	ldr	r3, .L1494+16
	mov	r0, r5
	movw	r2, #9092
	ldr	r1, .L1494+20
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1466
.L1495:
	.align	2
.L1494:
	.word	.LANCHOR0-4
	.word	g_vdm_hal_fun_ptr
	.word	g_VdmDrvParam
	.word	g_ScdDrvParam
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC16
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
	bhi	.L1502
	ldr	r6, .L1504
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	beq	.L1503
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L1501
	str	r2, [r3, #28]
	mov	r2, #0
	ldr	r3, [r6, r0, asl #2]
	str	r2, [r3, #16]
	bl	SM_Stop
	subs	r5, r0, #0
	bne	.L1501
	mov	r1, r5
	mov	r0, r4
	bl	DeleteRawPacketInBuffer
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	ldr	r3, [r6, r4, asl #2]
	str	r5, [r3, #28]
	bl	VCTRL_CloseHardware
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1501:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1502:
	ldr	ip, .L1504+4
	movw	r2, #2726
	ldr	r3, .L1504+8
	mov	r0, #0
	ldr	r1, .L1504+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1503:
	ldr	ip, .L1504+4
	mov	r0, r3
	movw	r2, #2727
	ldr	r3, .L1504+16
	ldr	r1, .L1504+12
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1505:
	.align	2
.L1504:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC6
	.word	.LC1
	.word	.LC132
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
	ldr	r5, .L1513
	mov	r7, r0
	mov	r4, #0
	mov	r8, r5
	b	.L1508
.L1512:
	add	r4, r4, #1
	ldr	r3, [r8, #120]
	blx	r3
	cmp	r4, #50
	beq	.L1510
.L1508:
	ldr	r3, [r5, #12]
	mov	r0, #0
	blx	r3
	mov	r0, r7
	bl	VCTRL_StopChan
	mov	r6, r0
	bl	VCTRL_EnableAllInt
	ldr	r3, [r5, #16]
	mov	r0, #0
	blx	r3
	cmp	r6, #0
	mov	r0, #10
	bne	.L1512
	cmp	r4, #49
	bgt	.L1510
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1510:
	ldr	r3, [r5, #68]
	mvn	r6, #0
	ldr	r1, .L1513+4
	mov	r0, #0
	blx	r3
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1514:
	.align	2
.L1513:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC133
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
	ldr	r5, .L1542
	mov	r4, r0
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L1538
	ldr	r8, [r3, #16]
	bl	VCTRL_StopChan
	ldr	r7, .L1542+4
	mov	r2, #0
	ldr	r3, [r5, r4, asl #2]
	mov	r0, #4
	ldr	r1, [r7, #12]
	str	r2, [r3, #328]
	str	r2, [r3, #332]
	blx	r1
	ldr	r3, [r5, r4, asl #2]
	ldr	r2, [r3, #1388]
	cmp	r2, #1
	beq	.L1539
.L1518:
	ldr	r0, .L1542+8
	mov	r2, #300
	ldr	r3, [r7, #48]
	mov	r1, #0
	mla	r0, r2, r4, r0
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	cmp	r3, #0
	addne	r3, r3, #225280
	movne	r2, #0
	strne	r2, [r3, #3300]
	beq	.L1540
.L1520:
	mov	r2, #896
	mov	r1, #0
	ldr	r3, [r7, #48]
	mov	r6, #0
	ldr	r0, .L1542+12
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	mov	r0, r4
	str	r6, [r3, #1224]
	bl	InitSyntaxCtx
	ldr	r0, [r5, r4, asl #2]
	mov	r1, r6
	ldr	r3, [r7, #48]
	mov	r2, #416
	add	r0, r0, #664
	blx	r3
	ldr	r1, [r5, r4, asl #2]
	mvn	r2, #0
	mvn	r3, #0
	add	r0, r1, #229376
	add	ip, r1, #592
	strd	r2, [ip, #-8]
	strd	r2, [ip]
	str	r6, [r1, #1080]
	str	r6, [r0, #736]
	str	r6, [r0, #728]
	str	r6, [r0, #732]
	ldr	r3, [r1, #1388]
	cmp	r3, #1
	streq	r6, [r1, #1428]
	streq	r6, [r0, #716]
	mov	r0, r4
	bl	FSP_SwitchNotify
	ldr	r3, [r5, r4, asl #2]
	mov	r0, r4
	ldr	r3, [r3, #528]
	cmp	r3, #19
	beq	.L1522
	bl	FSP_ResetInstance
.L1523:
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	mov	r0, r4
	bl	InitUsdPool
	mov	r0, r4
	bl	SM_Reset
	ldr	r3, .L1542+16
	ldr	r2, [r3, #708]
	cmp	r2, #0
	beq	.L1524
	add	r5, r3, r4, lsl #2
	str	r4, [fp, #-40]
	ldr	r3, [r5, #712]
	cmp	r3, #0
	beq	.L1526
	mov	r0, #512
	sub	r1, fp, #40
	bl	VCTRL_SetDbgOption
	ldr	r3, [r5, #712]
	cmp	r3, #0
	beq	.L1526
	sub	r1, fp, #40
	mov	r0, #512
	bl	VCTRL_SetDbgOption
.L1526:
	ldr	r3, [r7, #68]
	mov	r0, #32
	ldr	r1, .L1542+20
	blx	r3
.L1524:
	cmp	r8, #0
	bne	.L1541
.L1528:
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	mov	r0, #0
.L1517:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1541:
	mov	r0, r4
	bl	VCTRL_StartChan
	b	.L1528
.L1522:
	bl	FSP_EmptyInstance
	b	.L1523
.L1539:
	add	r2, r3, #229376
	ldr	r3, [r3, #528]
	ldr	r2, [r2, #752]
	cmp	r2, r3
	bne	.L1518
	mov	r0, r4
	bl	VCTRL_FindOccupiedFrame
	b	.L1518
.L1540:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1520
.L1538:
	ldr	ip, .L1542+4
	mov	r0, r3
	movw	r2, #3937
	ldr	r3, .L1542+24
	ldr	r1, .L1542+28
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L1517
.L1543:
	.align	2
.L1542:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+4016
	.word	g_stLowdlyInfo
	.word	.LANCHOR3
	.word	.LC134
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ResetChan, .-VCTRL_ResetChan
	.align	2
	.global	VCTRL_CreateChanWithOption
	.type	VCTRL_CreateChanWithOption, %function
VCTRL_CreateChanWithOption:
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
	ldr	r5, .L1622
	mov	r8, r0
	mov	r7, r1
	mov	r4, r2
	mov	r1, #0
	ldr	r3, [r5, #48]
	mov	r2, #16
	sub	r0, fp, #204
	ldr	r9, .L1622+4
	blx	r3
	ldr	r3, [r5, #48]
	mov	r2, #16
	mov	r1, #0
	sub	r0, fp, #188
	blx	r3
	ldr	r3, [r5, #48]
	mov	r2, #16
	mov	r1, #0
	sub	r0, fp, #172
	blx	r3
	ldr	r3, [r5, #48]
	sub	r0, fp, #140
	mov	r2, #96
	mov	r1, #0
	blx	r3
	subs	r3, r8, #25
	str	r7, [r9, #840]
	movne	r3, #1
	cmp	r4, #0
	movne	r3, #0
	cmp	r3, #0
	bne	.L1606
	ldr	r3, [r7, #36]
	cmp	r3, #0
	blt	.L1547
	ldr	r3, [r7, #12]
	cmp	r3, #31
	ble	.L1547
	ldr	r3, [r7, #16]
	cmp	r3, #31
	ble	.L1547
	ldr	r3, [r7, #56]
	cmp	r3, #262144
	blt	.L1547
	ldr	r3, [r7, #44]
	cmp	r3, #1
	beq	.L1607
	cmp	r3, #2
	beq	.L1608
.L1549:
	ldr	r3, [r7, #128]
	movw	ip, #28424
	movt	ip, 8
	movw	lr, #57520
	cmp	r3, #1
	sub	r2, fp, #216
	mov	r1, r7
	mov	r0, r8
	movhi	r3, #0
	strhi	r3, [r7, #128]
	cmp	r8, #26
	mov	r3, r4
	movt	lr, 176
	movne	lr, ip
	str	lr, [fp, #-224]
	bl	VCTRL_GetChanMemSizeWithOption
	subs	r10, r0, #0
	bne	.L1609
	ldr	r3, [r7, #116]
	cmp	r3, #0
	beq	.L1554
	ldr	r3, [r7, #120]
	cmp	r3, #0
	bne	.L1555
.L1554:
	mov	r0, #5
	mov	r10, #1
	bl	VFMW_GetString
	ldr	r2, [fp, #-208]
	str	r10, [sp]
	sub	r3, fp, #156
	mov	r1, #0
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	bne	.L1610
	ldr	r2, [fp, #-144]
	ldr	r1, [fp, #-152]
	ldr	r0, [fp, #-148]
	bl	MEM_AddMemRecord
	ldr	r1, [fp, #-148]
	ldr	r2, [fp, #-152]
	ldr	r3, [fp, #-144]
	str	r1, [fp, #-172]
	str	r2, [fp, #-168]
	str	r3, [fp, #-164]
.L1557:
	ldr	r3, [r7, #84]
	cmp	r3, #0
	beq	.L1559
	ldr	r3, [r7, #88]
	cmp	r3, #0
	bne	.L1560
.L1559:
	ldr	r3, [r7, #128]
	cmp	r3, #1
	beq	.L1561
	mov	r0, #6
	bl	VFMW_GetString
	mov	r1, #0
	ldr	r2, [fp, #-216]
	sub	r3, fp, #156
	str	r1, [sp]
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	bne	.L1611
	ldr	r2, [fp, #-144]
	mov	r3, #1
	ldr	r1, [fp, #-152]
	ldr	r0, [fp, #-148]
	str	r3, [fp, #-228]
	bl	MEM_AddMemRecord
	ldr	r1, [fp, #-148]
	ldr	r2, [fp, #-152]
	ldr	r3, [fp, #-144]
	str	r1, [fp, #-204]
	str	r2, [fp, #-200]
	str	r3, [fp, #-196]
.L1564:
	ldr	r3, [r7, #100]
	cmp	r3, #0
	beq	.L1568
	ldr	r3, [r7, #104]
	cmp	r3, #0
	bne	.L1569
.L1568:
	mov	r0, #7
	bl	VFMW_GetString
	mov	r1, #0
	ldr	r2, [fp, #-212]
	sub	r3, fp, #156
	str	r1, [sp]
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	bne	.L1612
	ldr	r2, [fp, #-144]
	mov	r3, #1
	ldr	r1, [fp, #-152]
	ldr	r0, [fp, #-148]
	str	r3, [fp, #-232]
	bl	MEM_AddMemRecord
	ldr	r1, [fp, #-148]
	ldr	r2, [fp, #-152]
	ldr	r3, [fp, #-144]
	str	r1, [fp, #-188]
	str	r2, [fp, #-184]
	str	r3, [fp, #-180]
.L1573:
	ldr	r6, .L1622+8
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	bl	VCTRL_MaskAllInt
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L1587
	mov	r3, r6
	mov	r4, #1
.L1580:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L1613
	add	r4, r4, #1
	cmp	r4, #16
	bne	.L1580
	mov	r2, r4
	ldr	r3, [r5, #68]
	ldr	r1, .L1622+12
	mov	r0, #1
	blx	r3
	bl	VCTRL_EnableAllInt
	ldr	r3, [r5, #16]
	mov	r0, #0
	blx	r3
	mvn	r0, #0
.L1602:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1561:
	mov	r3, #0
	str	r3, [fp, #-204]
	str	r3, [fp, #-200]
	str	r3, [fp, #-228]
	str	r3, [fp, #-196]
	b	.L1564
.L1608:
	ldr	r3, [r7, #20]
	cmp	r3, #0
	ble	.L1547
	ldr	r3, [r7, #24]
	cmp	r3, #0
	ble	.L1547
.L1604:
	ldr	r3, [r7, #28]
	cmp	r3, #0
	ble	.L1547
	ldr	r3, [r7, #32]
	cmp	r3, #0
	bgt	.L1549
.L1547:
	ldr	r3, [r5, #68]
	mov	r0, #0
	ldr	r1, .L1622+16
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1613:
	uxtb	r3, r4
	str	r3, [fp, #-236]
.L1578:
	ldr	r3, [fp, #-224]
	mov	r1, #0
	ldr	r0, [fp, #-168]
	sub	r2, r3, #207872
	ldr	ip, [r5, #48]
	sub	r2, r2, #8
	str	r1, [fp, #-244]
	str	r0, [r6, r4, asl #2]
	blx	ip
	ldr	r0, [r6, r4, asl #2]
	ldr	ip, [r5, #52]
	mov	r2, #160
	add	r0, r0, #1248
	mov	r1, r7
	add	r0, r0, #12
	mov	lr, r4, asl #4
	str	lr, [fp, #-240]
	blx	ip
	ldr	r1, [r6, r4, asl #2]
	mov	r2, r4, asl #6
	ldr	r3, [fp, #-224]
	mov	r0, #20
	ldr	lr, [fp, #-240]
	mov	ip, r2
	str	r8, [r1, #40]
	str	r3, [r1, #1200]
	rsb	r1, lr, ip
	ldr	r3, .L1622+20
	mov	ip, #30
	str	r2, [fp, #-248]
	add	r1, r3, r1
	ldr	r2, [r6, r4, asl #2]
	ldr	r3, [fp, #-244]
	str	r4, [r2, #8]
	str	r8, [r2, #564]
	str	r3, [r2, #1216]
	ldr	r2, [r6, r4, asl #2]
	ldr	lr, [r7]
	str	lr, [r2, #36]
	ldr	r2, [r6, r4, asl #2]
	add	lr, r2, #229376
	str	r0, [r2, #528]
	str	r0, [lr, #752]
	mov	r0, r4
	str	r3, [r2, #32]
	ldr	r2, [r6, r4, asl #2]
	ldr	lr, .L1622+24
	str	ip, [r2, #1448]
	add	ip, r2, #205824
	add	r2, r2, #1456
	str	r2, [r1, #40]
	add	ip, ip, #432
	str	ip, [lr, r4, asl #2]
	bl	VCTRL_ResetChan
	ldr	r1, [r6, r4, asl #2]
	ldr	r3, [fp, #-244]
	ldr	r0, [fp, #-228]
	add	r2, r1, #225280
	ldr	ip, [fp, #-232]
	str	r10, [r1, #256]
	mov	r8, r3
	str	r0, [r1, #248]
	add	r0, r1, #229376
	str	ip, [r1, #252]
	str	r3, [r2, #3272]
	str	r3, [r2, #3276]
	str	r3, [r2, #3284]
	str	r3, [r2, #3288]
	str	r3, [r2, #3280]
	str	r3, [r2, #3296]
	ldr	r2, [r7, #128]
	str	r3, [r0, #716]
	str	r2, [r0, #720]
.L1582:
	ldr	r0, [r6, r4, asl #2]
	mov	r3, r8, asl #4
	add	r3, r3, r8, lsl #2
	mvn	r1, #0
	add	r2, r0, r3
	add	r3, r3, #228352
	add	r2, r2, #228352
	add	r3, r3, #232
	add	r0, r0, r3
	add	r8, r8, #1
	strb	r1, [r2, #248]
	mov	r2, #16
	ldr	r3, [r5, #48]
	mov	r1, #0
	blx	r3
	cmp	r8, #32
	ldr	r10, .L1622+8
	ldr	r3, .L1622
	bne	.L1582
	ldr	r0, [r10, r4, asl #2]
	mov	r2, #820
	ldr	ip, [r3, #48]
	mov	r1, #0
	add	r0, r0, #228352
	str	r3, [fp, #-228]
	add	r0, r0, #872
	add	r9, r9, r4, lsl #2
	blx	ip
	ldr	r0, [r10, r4, asl #2]
	ldr	r3, [fp, #-228]
	mov	r2, #24
	add	r0, r0, #229376
	mov	r1, #0
	add	r0, r0, #668
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, [r10, r4, asl #2]
	ldr	r2, [fp, #-200]
	mov	ip, #0
	ldr	r1, [fp, #-184]
	add	r8, r3, #225280
	ldr	lr, [fp, #-172]
	ldr	r10, [fp, #-168]
	str	r2, [r3, #204]
	ldr	r0, [fp, #-204]
	str	r1, [r3, #220]
	ldr	r2, [fp, #-196]
	str	lr, [r3, #240]
	str	r10, [r3, #236]
	ldr	lr, [fp, #-188]
	ldr	r1, [fp, #-180]
	ldr	r10, [fp, #-164]
	str	r0, [r3, #208]
	str	r0, [r3, #280]
	str	r10, [r3, #244]
	str	r2, [r3, #212]
	str	r2, [r3, #284]
	str	lr, [r3, #224]
	str	r1, [r3, #228]
	str	ip, [r8, #3256]
	ldr	r8, [r3, #1388]
	str	ip, [r9, #844]
	cmp	r8, #1
	beq	.L1583
	ldr	r8, [r3, #272]
	ldr	ip, [r3, #4]
	rsb	r2, r8, r2
	str	r1, [r3, #288]
	rsb	r2, ip, r2
	add	ip, r8, ip
	sub	r2, r2, #47616
	add	ip, ip, r0
	sub	r2, r2, #16
	str	r2, [r3, #264]
	add	r1, r2, ip
	mov	r0, #14848
	add	r2, r1, #14848
	add	r1, r1, #15
	add	r2, r2, #15
	bic	r1, r1, #15
	bic	r2, r2, #15
	str	lr, [r3, #276]
	str	r2, [r3, #300]
	mov	r2, #32768
	str	ip, [r3, #260]
	str	r1, [r3, #292]
	str	r0, [r3, #296]
	str	r2, [r3, #304]
.L1584:
	ldr	r3, [r6, r4, asl #2]
	mov	r0, r4
	ldr	r8, .L1622+8
	str	r4, [r3, #8]
	ldr	r3, [r6, r4, asl #2]
	add	r1, r3, #204800
	add	r2, r3, #188416
	str	r4, [r3, #188]
	str	r4, [r3, #192]
	ldrb	r3, [fp, #-236]	@ zero_extendqisi2
	strb	r3, [r1, #1456]
	str	r4, [r2, #3800]
	bl	FSP_EnableInstance
	subs	r9, r0, #0
	bne	.L1614
	mov	r0, r4
	mov	r6, #1
	bl	SM_ClearInst
	ldr	r3, [r8, r4, asl #2]
	ldr	r2, [fp, #-224]
	mov	r0, r4
	sub	r1, r2, #205824
	ldr	r3, [r3, #236]
	sub	r1, r1, #8
	add	r1, r3, r1
	bl	OpenUsdPool_WithMem
	ldr	r3, [r8, r4, asl #2]
	mov	r0, r4
	str	r6, [r3, #12]
	bl	VDMDRV_ClearChanIsDec
	bl	VCTRL_EnableAllInt
	bl	VDMDRV_ParamInit
	ldr	r3, [r5, #16]
	mov	r0, r9
	blx	r3
	str	r6, [r7, #48]
	mov	r0, r4
	bl	SM_AllocDSPCtxMem
	mov	r0, r4
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1607:
	ldr	r3, [r7, #20]
	cmp	r3, #0
	bgt	.L1604
	b	.L1547
.L1555:
	ldr	r3, [r5, #52]
	mov	r2, #16
	add	r1, r7, #112
	sub	r0, fp, #172
	blx	r3
	ldr	r2, [fp, #-164]
	ldr	r3, [fp, #-208]
	ldr	r1, .L1622
	cmp	r2, r3
	blt	.L1615
	ldr	r1, [fp, #-168]
	ldr	r0, [fp, #-172]
	bl	MEM_AddMemRecord
	b	.L1557
.L1569:
	add	r1, r7, #96
	sub	r0, fp, #188
	ldr	r3, [r5, #52]
	mov	r2, #16
	blx	r3
	ldr	r1, [fp, #-184]
	cmp	r1, #0
	beq	.L1574
	ldr	r2, [fp, #-180]
	ldr	r3, [fp, #-212]
	cmp	r2, r3
	blt	.L1574
	ldr	r0, [fp, #-188]
	mov	r3, #0
	str	r3, [fp, #-232]
	bl	MEM_AddMemRecord
	b	.L1573
.L1560:
	ldr	r3, [r5, #52]
	mov	r2, #16
	sub	r0, fp, #204
	add	r1, r7, #80
	blx	r3
	ldr	r3, [r7, #128]
	ldr	r2, [fp, #-196]
	cmp	r3, #1
	beq	.L1566
	ldr	r3, [fp, #-216]
	cmp	r2, r3
	blt	.L1616
.L1566:
	ldr	r1, [fp, #-200]
	mov	r3, #0
	ldr	r0, [fp, #-204]
	str	r3, [fp, #-228]
	bl	MEM_AddMemRecord
	b	.L1564
.L1583:
	sub	r1, r1, #47616
	str	ip, [r3, #260]
	sub	r1, r1, #16
	str	r1, [r3, #288]
	add	r2, r1, lr
	str	ip, [r3, #264]
	add	r1, r2, #14848
	add	r2, r2, #15
	add	r1, r1, #15
	bic	r2, r2, #15
	bic	r1, r1, #15
	str	r2, [r3, #292]
	str	r1, [r3, #300]
	mov	r2, #32768
	mov	r1, #14848
	str	lr, [r3, #276]
	str	r1, [r3, #296]
	str	r2, [r3, #304]
	b	.L1584
.L1612:
	cmp	r10, #1
	beq	.L1617
.L1571:
	ldr	r3, [fp, #-228]
	cmp	r3, #1
	beq	.L1618
.L1572:
	ldr	r3, [r5, #68]
	movw	r2, #2115
	ldr	r1, .L1622+28
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	b	.L1602
.L1574:
	cmp	r10, #1
	beq	.L1619
.L1576:
	ldr	r3, [fp, #-228]
	cmp	r3, #1
	beq	.L1620
.L1577:
	ldr	r3, [r5, #68]
	movw	r2, #2143
	ldr	r1, .L1622+32
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	b	.L1602
.L1618:
	ldr	r1, [fp, #-200]
	ldr	r0, [fp, #-204]
	bl	MEM_ReleaseMemBlock
	sub	r0, fp, #204
	ldmia	r0, {r0, r1, r2}
	bl	MEM_DelMemRecord
	b	.L1572
.L1617:
	ldr	r1, [fp, #-168]
	ldr	r0, [fp, #-172]
	bl	MEM_ReleaseMemBlock
	sub	r0, fp, #172
	ldmia	r0, {r0, r1, r2}
	bl	MEM_DelMemRecord
	b	.L1571
.L1587:
	str	r3, [fp, #-236]
	mov	r4, r3
	b	.L1578
.L1611:
	cmp	r10, #1
	beq	.L1621
.L1563:
	ldr	r3, [r5, #68]
	movw	r2, #2058
	ldr	r1, .L1622+36
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	b	.L1602
.L1614:
	ldr	r3, [r5, #68]
	mov	r0, #1
	ldr	r1, .L1622+40
	mov	r6, #0
	blx	r3
	mov	r0, r4
	bl	FSP_DisableInstance
	mov	r0, r4
	bl	CloseUsdPool_WithMem
	ldr	ip, [r8, r4, asl #2]
	mov	r2, #16
	ldr	r7, [r5, #52]
	add	r1, ip, #232
	add	r3, ip, #228352
	ldr	lr, [ip, #248]
	add	r3, r3, #232
	ldr	r0, [ip, #256]
	ldr	ip, [ip, #252]
	str	lr, [fp, #-76]
	str	r3, [fp, #-48]
	str	ip, [fp, #-72]
	str	r0, [fp, #-68]
	sub	r0, fp, #108
	blx	r7
	ldr	r1, [r8, r4, asl #2]
	ldr	r3, [r5, #52]
	mov	r2, #16
	add	r1, r1, #200
	sub	r0, fp, #140
	blx	r3
	ldr	r1, [r8, r4, asl #2]
	ldr	r3, [r5, #52]
	mov	r2, #16
	add	r1, r1, #216
	sub	r0, fp, #124
	blx	r3
	ldr	r3, [fp, #-248]
	ldr	r2, [fp, #-240]
	sub	r0, fp, #92
	rsb	r1, r2, r3
	ldr	r3, .L1622+20
	mov	r2, #16
	add	r1, r3, r1
	ldr	r3, [r5, #52]
	blx	r3
	ldr	r1, [r8, r4, asl #2]
	mov	r2, #12
	ldr	r3, [r5, #52]
	add	r1, r1, #152
	sub	r0, fp, #64
	blx	r3
	mov	r0, r4
	str	r6, [r8, r4, asl #2]
	bl	VDMDRV_ClearChanIsDec
	bl	VCTRL_EnableAllInt
	ldr	r3, [r5, #16]
	mov	r0, r6
	blx	r3
	sub	r1, fp, #140
	mov	r0, r4
	bl	VCTRL_FreeChanResource
	mvn	r0, #0
	b	.L1602
.L1610:
	ldr	r3, [r5, #68]
	mov	r0, r10
	movw	r2, #2023
	ldr	r1, .L1622+44
	blx	r3
	mvn	r0, #0
	b	.L1602
.L1606:
	ldr	r3, [r5, #68]
	mov	r0, #0
	ldr	r1, .L1622+48
	blx	r3
	mvn	r0, #0
	b	.L1602
.L1609:
	ldr	r3, [r5, #68]
	movw	r2, #2015
	ldr	r1, .L1622+52
	mov	r0, #0
	blx	r3
	mvn	r0, #0
	b	.L1602
.L1615:
	ldr	r3, [r1, #68]
	movw	r2, #2039
	ldr	r1, .L1622+56
	mov	r0, #1
	blx	r3
	mvn	r0, #0
	b	.L1602
.L1621:
	ldr	r1, [fp, #-168]
	ldr	r0, [fp, #-172]
	bl	MEM_ReleaseMemBlock
	sub	r0, fp, #172
	ldmia	r0, {r0, r1, r2}
	bl	MEM_DelMemRecord
	b	.L1563
.L1616:
	cmp	r10, #1
	bne	.L1567
	ldr	r1, [fp, #-168]
	ldr	r0, [fp, #-172]
	bl	MEM_ReleaseMemBlock
	sub	r0, fp, #172
	ldmia	r0, {r0, r1, r2}
	bl	MEM_DelMemRecord
	ldr	r2, [fp, #-196]
	ldr	r3, [fp, #-216]
.L1567:
	str	r3, [sp]
	mov	r0, #1
	mov	r3, r2
	ldr	ip, [r5, #68]
	movw	r2, #2090
	ldr	r1, .L1622+60
	blx	ip
	mvn	r0, #0
	b	.L1602
.L1620:
	ldr	r1, [fp, #-200]
	ldr	r0, [fp, #-204]
	bl	MEM_ReleaseMemBlock
	sub	r0, fp, #204
	ldmia	r0, {r0, r1, r2}
	bl	MEM_DelMemRecord
	b	.L1577
.L1619:
	ldr	r1, [fp, #-168]
	ldr	r0, [fp, #-172]
	bl	MEM_ReleaseMemBlock
	sub	r0, fp, #172
	ldmia	r0, {r0, r1, r2}
	bl	MEM_DelMemRecord
	b	.L1576
.L1623:
	.align	2
.L1622:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR3
	.word	.LANCHOR0
	.word	.LC144
	.word	.LC75
	.word	s_SmIIS
	.word	s_pFspInst
	.word	.LC141
	.word	.LC142
	.word	.LC139
	.word	.LC143
	.word	.LC137
	.word	.LC135
	.word	.LC136
	.word	.LC138
	.word	.LC140
	UNWIND(.fnend)
	.size	VCTRL_CreateChanWithOption, .-VCTRL_CreateChanWithOption
	.align	2
	.global	VCTRL_CreateChan
	.type	VCTRL_CreateChan, %function
VCTRL_CreateChan:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 216
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #228)
	sub	sp, sp, #228
	cmp	r0, #32
	mov	r4, r0
	mov	r6, r1
	bhi	.L1648
	ldr	r5, .L1651
	mov	r2, #160
	mov	r1, #0
	sub	r0, fp, #196
	mov	r7, #136
	ldr	r3, [r5, #48]
	blx	r3
	ldr	r3, [r5, #48]
	mov	r2, #16
	mov	r1, #0
	sub	r0, fp, #228
	blx	r3
	ldr	r3, [r5, #48]
	mov	r2, #16
	mov	r1, #0
	sub	r0, fp, #212
	blx	r3
	ldr	r2, .L1651+4
	mov	r1, r4, asl #2
	mov	r0, r4, asl #5
	rsb	r3, r1, r0
	sub	ip, r4, #18
	add	r8, r2, r3
	mov	lr, #2
	ldr	r3, [r2, r3]
	ldr	r8, [r8, #4]
	str	lr, [fp, #-188]
	mov	lr, #32
	str	r3, [fp, #-184]
	mov	r3, #256
	str	r8, [fp, #-180]
	str	r7, [fp, #-176]
	str	lr, [fp, #-168]
	str	r3, [fp, #-164]
	cmp	ip, #7
	ldrls	pc, [pc, ip, asl #2]
	b	.L1627
.L1629:
	.word	.L1639
	.word	.L1639
	.word	.L1639
	.word	.L1639
	.word	.L1639
	.word	.L1630
	.word	.L1630
	.word	.L1630
.L1639:
	mov	lr, #5
	mov	ip, #3
.L1628:
	rsb	r3, r1, r0
	rsb	r0, r1, r0
	add	r3, r2, r3
	add	r2, r2, r0
	mov	r1, #0
	str	ip, [fp, #-136]
	ldr	r3, [r3, #12]
	ldr	r2, [r2, #24]
	cmp	r3, lr
	str	r1, [fp, #-132]
	rsbgt	r3, ip, r3
	str	r1, [fp, #-128]
	movle	r3, #1
	subgt	r3, r3, #2
	cmp	r4, #24
	str	r3, [fp, #-160]
	str	r2, [fp, #-140]
	mov	r3, #1
	streq	r1, [fp, #-160]
	str	r3, [fp, #-156]
	str	r3, [fp, #-152]
	str	r3, [fp, #-144]
	streq	r3, [fp, #-136]
	beq	.L1634
	cmp	r4, #26
	beq	.L1649
	cmp	r4, #18
	moveq	r2, #4
	moveq	r3, #2
	streq	r2, [fp, #-160]
	streq	r3, [fp, #-136]
.L1634:
	sub	r2, fp, #240
	sub	r1, fp, #252
	mov	r0, r4
	bl	VCTRL_GetChanMemSize
	cmp	r0, #0
	bne	.L1650
	cmp	r6, #0
	beq	.L1640
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq	.L1637
	ldr	r2, [r6]
	cmp	r2, #0
	beq	.L1638
	ldr	r0, [r6, #4]
	cmp	r0, #0
	beq	.L1638
	ldr	r1, [fp, #-252]
	ldr	ip, [fp, #-240]
	add	r1, r1, ip
	cmp	r3, r1
	blt	.L1638
	ldr	ip, [fp, #-236]
	mov	lr, #3
	sub	r1, fp, #212
	str	r2, [fp, #-212]
	rsb	r3, ip, r3
	add	r2, ip, r2
	str	r0, [fp, #-208]
	add	r0, r0, ip
	str	r2, [fp, #-228]
	mov	r2, #16
	str	r0, [fp, #-224]
	sub	r0, fp, #100
	str	ip, [fp, #-204]
	str	r3, [fp, #-220]
	str	lr, [fp, #-188]
	ldr	r3, [r5, #52]
	blx	r3
	ldr	r3, [r5, #52]
	sub	r1, fp, #228
	mov	r2, #16
	sub	r0, fp, #116
	blx	r3
	mov	r3, #1
.L1637:
	sub	r1, fp, #196
	mov	r0, r4
	mov	r2, #1
	bl	VCTRL_CreateChanWithOption
	orr	r0, r0, r0, asr #31
.L1626:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1630:
	mov	lr, #3
	mov	ip, #1
	b	.L1628
.L1627:
	mov	lr, #8
	mov	ip, #6
	b	.L1628
.L1649:
	mov	r2, #16
	mov	r3, #6
	str	r2, [fp, #-160]
	str	r3, [fp, #-136]
	b	.L1634
.L1640:
	mov	r3, r6
	b	.L1637
.L1638:
	ldr	r3, [r5, #68]
	mov	r0, #1
	ldr	r1, .L1651+8
	blx	r3
	ldr	lr, [r6, #8]
	ldmia	r6, {r2, r3}
	mov	r0, #1
	ldr	ip, [r5, #68]
	ldr	r1, .L1651+12
	str	lr, [sp]
	blx	ip
	ldr	r1, [fp, #-252]
	ldr	r2, [fp, #-240]
	mov	r0, #1
	ldr	r3, [r5, #68]
	add	r2, r1, r2
	ldr	r1, .L1651+16
	blx	r3
	mvn	r0, #0
	b	.L1626
.L1648:
	ldr	r3, .L1651
	mov	r2, r0
	ldr	r1, .L1651+20
	mov	r0, #1
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	b	.L1626
.L1650:
	ldr	r3, [r5, #68]
	mov	r0, #1
	ldr	r1, .L1651+24
	blx	r3
	mvn	r0, #0
	b	.L1626
.L1652:
	.align	2
.L1651:
	.word	vfmw_Osal_Func_Ptr_S
	.word	CapItem
	.word	.LC147
	.word	.LC148
	.word	.LC149
	.word	.LC145
	.word	.LC146
	UNWIND(.fnend)
	.size	VCTRL_CreateChan, .-VCTRL_CreateChan
	.align	2
	.global	VCTRL_ConfigChan
	.type	VCTRL_ConfigChan, %function
VCTRL_ConfigChan:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 96
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #108)
	sub	sp, sp, #108
	cmp	r0, #15
	mov	r9, #0
	mov	r5, r0
	mov	r4, r1
	str	r9, [fp, #-120]
	bhi	.L1738
	ldr	r6, .L1755
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	beq	.L1739
	ldr	r7, [r3, #12]
	cmp	r7, #1
	bne	.L1740
	bl	VCTRL_CheckChanWH
	subs	r8, r0, #0
	bne	.L1741
	ldr	r9, .L1755+4
	add	r0, r9, #908
	bl	GetVdecCapability
	ldr	r3, [r9, #928]
	ldr	r2, [r4]
	cmp	r3, #19
	bhi	.L1660
	cmp	r3, r2
	beq	.L1661
	ldr	r0, .L1755+8
	add	r3, r9, #928
.L1663:
	ldr	r1, [r3, #4]!
	cmp	r1, #19
	bhi	.L1660
	cmp	r1, r2
	beq	.L1661
	cmp	r3, r0
	bne	.L1663
.L1661:
	cmp	r2, #8
	beq	.L1742
.L1664:
	cmp	r2, #9
	beq	.L1666
.L1670:
	mov	r1, r4
	mov	r0, r5
	bl	VCTRL_CmpConfigParameter
	subs	r9, r0, #0
	beq	.L1743
	ldr	r3, [r4, #80]
	cmp	r3, #0
	bne	.L1671
	ldr	r3, [r4]
	sub	r2, r3, #10
	cmp	r2, #2
	bls	.L1744
	ldr	r10, [r6, r5, asl #2]
	cmp	r3, #15
	cmpne	r3, #4
	ldr	r1, .L1755
	moveq	r7, #1
	movne	r7, #0
	ldr	r3, [r10, #48]
	sub	r2, r3, #10
	cmp	r2, #2
	bhi	.L1675
	ldr	r2, [r1, #2648]
	tst	r2, #1
	eoreq	r7, r7, #1
	beq	.L1676
.L1675:
	cmp	r3, #15
	cmpne	r3, #4
	moveq	r3, #1
	movne	r3, #0
	eor	r7, r7, r3
	and	r7, r7, #1
.L1676:
	cmp	r7, #0
	ldr	r7, .L1755+12
	bne	.L1745
.L1677:
	ldr	r3, [r7, #52]
	mov	r2, #104
	mov	r1, r4
	add	r0, r10, #48
	blx	r3
	ldr	ip, [r4, #12]
	ldr	r3, [r6, r5, asl #2]
	mov	r2, #52
	mov	r1, #0
	sub	r0, fp, #100
	add	r9, r4, #4
	str	ip, [r3, #24]
	ldr	r3, [r6, r5, asl #2]
	ldr	lr, [r4]
	ldr	ip, [r7, #48]
	str	lr, [r3, #528]
	ldr	lr, [r4, #92]
	str	lr, [r3, #1396]
	blx	ip
	ldr	r3, [r6, r5, asl #2]
	ldr	r3, [r3, #276]
	mov	r0, r3
	str	r3, [fp, #-88]
	bl	MEM_Phy2Vir
	ldr	lr, [r6, r5, asl #2]
	ldr	r1, [r4, #80]
	mov	r2, #49152
	movt	r2, 63683
	ldr	r3, [r4]
	cmp	r1, #1
	ldr	ip, [r4, #12]
	str	r0, [fp, #-100]
	ldr	r0, [lr, #288]
	str	r2, [fp, #-80]
	ldreq	r2, [r4, #88]
	str	r0, [fp, #-84]
	ldreq	r0, [r4, #84]
	streq	r1, [fp, #-60]
	mov	r1, r9
	streq	r2, [fp, #-52]
	mov	r2, #8
	streq	r0, [fp, #-56]
	cmp	r3, #0
	ldr	r0, [r4, #76]
	ldreq	r8, [r4, #48]
	strb	ip, [fp, #-96]
	str	r3, [fp, #-92]
	str	r0, [fp, #-64]
	sub	r0, fp, #76
	ldr	r3, [r7, #52]
	str	r8, [fp, #-68]
	blx	r3
	sub	r1, fp, #100
	mov	r0, r5
	bl	SM_Config
	ldr	r3, [r7, #48]
	sub	r0, fp, #124
	mov	r2, #1
	mov	r1, #0
	blx	r3
	ldr	r3, [r4, #80]
	cmp	r3, #1
	beq	.L1746
.L1695:
	ldr	r3, [r6, r5, asl #2]
	mov	r1, r9
	mov	ip, #0
	mov	r2, #8
	add	r0, r3, #1072
	ldr	r10, [r3, #240]
	add	r0, r0, #12
	ldr	r9, [r3, #244]
	ldr	lr, [r3, #260]
	ldr	r8, [r3, #264]
	str	r10, [r3, #1204]
	str	r9, [r3, #1208]
	str	lr, [r3, #556]
	str	r8, [r3, #560]
	ldr	r9, [r3, #300]
	ldr	lr, [r3, #304]
	ldr	r10, [r3, #296]
	ldr	r8, [r3, #292]
	str	r9, [r3, #576]
	str	r10, [r3, #572]
	str	r8, [r3, #568]
	str	lr, [r3, #580]
	ldr	lr, [r4, #16]
	ldr	r9, .L1755+12
	str	lr, [r3, #540]
	ldr	lr, [r4, #16]
	str	lr, [r3, #536]
	ldr	lr, [r4, #24]
	str	lr, [r3, #544]
	ldr	lr, [r4, #28]
	str	lr, [r3, #548]
	ldr	lr, [r4, #32]
	str	lr, [r3, #552]
	ldr	lr, [r4, #52]
	str	lr, [r3, #1212]
	ldr	lr, [r4, #68]
	sub	lr, lr, #1
	clz	lr, lr
	mov	lr, lr, lsr #5
	str	lr, [r3, #1228]
	ldr	lr, [r4, #72]
	str	lr, [r3, #1232]
	ldr	lr, [r4, #48]
	str	ip, [r3, #1428]
	str	ip, [r3, #1440]
	str	lr, [r3, #1236]
	ldr	r7, [r7, #52]
	blx	r7
	ldr	ip, [r6, r5, asl #2]
	add	r3, ip, #229376
	ldr	r2, [ip, #528]
	ldr	r1, [r3, #752]
	cmp	r1, r2
	beq	.L1696
	mov	r0, r5
	bl	VCTRL_ResetChan
	ldr	r3, [r4]
	ldr	r2, .L1755
	cmp	r3, #19
	ldr	ip, [r2, r5, asl #2]
	beq	.L1697
	ldr	r2, [ip, #528]
	add	r3, ip, #229376
.L1696:
	str	r2, [r3, #752]
	ldr	r3, [ip, #280]
	mov	r0, r3
	str	r3, [fp, #-116]
	bl	MEM_Phy2Vir
	ldr	r3, [r6, r5, asl #2]
	sub	r1, fp, #116
	ldr	r3, [r3, #284]
	str	r3, [fp, #-108]
	str	r0, [fp, #-112]
	mov	r0, r5
	bl	RefreshDecCore
	subs	r9, r0, #0
	bne	.L1734
	ldr	r3, .L1755+16
	ldr	r0, .L1755+20
	ldr	r1, .L1755+24
	add	lr, r3, #64
.L1701:
	ldr	r2, [r0, #4]!
	add	r3, r3, #4
	add	r1, r1, #4
	cmp	r2, #0
	streq	r2, [r3, #-4]
	ldrne	ip, [r2, #24]
	strne	ip, [r3, #-4]
	ldrne	r2, [r2, #24]
	cmp	r3, lr
	str	r2, [r1, #-4]
	bne	.L1701
	ldr	r0, .L1755+16
	bl	VDMDRV_SetPriority
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1660:
	ldr	r3, .L1755+12
	mov	r0, #0
	ldr	r1, .L1755+28
	mvn	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
.L1727:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1744:
	ldr	r1, [r6, #2648]
	ldr	r2, .L1755
	tst	r1, #1
	bne	.L1747
	ldr	r10, [r2, r5, asl #2]
	ldr	r3, [r10, #48]
	sub	r2, r3, #10
	cmp	r2, #2
	bhi	.L1675
	ldr	r7, .L1755+12
	b	.L1677
.L1743:
	ldr	r3, .L1755+12
	ldr	r1, .L1755+32
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1666:
	sub	r0, fp, #120
	bl	REAL9DEC_Support
	ldr	r3, [fp, #-120]
	cmp	r3, #0
	bne	.L1670
.L1734:
	mvn	r9, #0
	b	.L1727
.L1742:
	sub	r0, fp, #120
	bl	REAL8DEC_Support
	ldr	r3, [fp, #-120]
	cmp	r3, #0
	ldrne	r2, [r4]
	bne	.L1664
	b	.L1734
.L1671:
	ldr	r10, [r6, r5, asl #2]
	ldr	r7, .L1755+12
	b	.L1677
.L1740:
	ldr	ip, .L1755+12
	mov	r0, r9
	ldr	r3, .L1755+36
	movw	r2, #2971
	ldr	r1, .L1755+40
	mvn	r9, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L1727
.L1745:
	ldr	r3, [r7, #16]
	mov	r0, #0
	blx	r3
	ldr	r3, [r10, #228]
	cmp	r3, #0
	bne	.L1748
.L1678:
	ldr	r3, [r7, #164]
	ldr	r0, [r10, #204]
	blx	r3
	ldr	r3, [r4]
	sub	r2, r3, #10
	cmp	r2, #2
	bhi	.L1679
	ldr	r2, [r6, #2648]
	tst	r2, #1
	bne	.L1679
.L1680:
	ldr	r3, [r7, #168]
	ldr	r1, [r10, #212]
	ldr	r0, [r10, #208]
	blx	r3
	ldr	ip, [r6, #2652]
	cmp	ip, #0
	mov	r9, r0
	beq	.L1682
	mov	r2, #0
	str	r0, [fp, #-100]
	str	r2, [fp, #-96]
	mov	r3, #8
	sub	r2, fp, #100
	mov	r1, #126
	mov	r0, r5
	blx	ip
.L1682:
	ldr	r1, [r10, #228]
	mov	r3, #1
	strb	r3, [r10, #200]
	cmp	r1, #0
	bne	.L1749
.L1703:
	str	r1, [fp, #-128]
.L1683:
	ldr	r3, [r7, #12]
	mov	r0, #0
	blx	r3
	ldr	r0, [r6, r5, asl #2]
	cmp	r0, #0
	beq	.L1750
	cmp	r9, #0
	beq	.L1686
	ldr	r3, [fp, #-128]
	ldr	r2, [r10, #228]
	clz	r3, r3
	cmp	r2, #0
	mov	r3, r3, lsr #5
	moveq	r3, #0
	cmp	r3, #0
	bne	.L1686
	ldr	r2, [r10, #212]
	ldr	r1, [r10, #204]
	ldr	r0, [r10, #208]
	str	r3, [fp, #-132]
	bl	MEM_DelMemRecord
	subs	r3, r0, #0
	str	r3, [fp, #-136]
	ldr	r3, [fp, #-132]
	bne	.L1751
	str	r9, [r10, #204]
	mov	r1, r9
	ldr	r2, [r10, #212]
	ldr	r0, [r10, #208]
	bl	MEM_AddMemRecord
	subs	r9, r0, #0
	bne	.L1752
	ldr	r2, [r10, #228]
	cmp	r2, #0
	bne	.L1753
.L1732:
	ldr	r3, .L1755
	ldr	r10, [r3, r5, asl #2]
	b	.L1677
.L1679:
	cmp	r3, #15
	cmpne	r3, #4
	moveq	r2, #1
	movne	r2, #0
	beq	.L1680
	ldr	r3, [r7, #160]
	ldr	r1, [r10, #212]
	ldr	r0, [r10, #208]
	str	r2, [fp, #-128]
	blx	r3
	ldr	r1, [r10, #228]
	ldr	r2, [fp, #-128]
	cmp	r1, #0
	ldr	r3, .L1755+12
	strb	r2, [r10, #200]
	mov	r9, r0
	beq	.L1703
	ldr	r3, [r3, #160]
	ldr	r0, [r10, #224]
	str	r2, [fp, #-132]
	blx	r3
	ldr	r2, [fp, #-132]
	strb	r2, [r10, #216]
	str	r0, [fp, #-128]
	b	.L1683
.L1747:
	ldr	r10, [r2, r5, asl #2]
	cmp	r3, #15
	cmpne	r3, #4
	moveq	r7, #1
	movne	r7, #0
	ldr	r3, [r10, #48]
	b	.L1675
.L1746:
	sub	r1, fp, #124
	mov	r0, r5
	strb	r3, [fp, #-124]
	bl	FSP_ConfigAttribute
	b	.L1695
.L1749:
	ldr	r3, [r7, #168]
	ldr	r0, [r10, #224]
	blx	r3
	ldr	ip, [r6, #2652]
	cmp	ip, #0
	str	r0, [fp, #-128]
	beq	.L1684
	mov	r2, #0
	str	r0, [fp, #-100]
	str	r2, [fp, #-96]
	mov	r3, #8
	sub	r2, fp, #100
	mov	r1, #125
	mov	r0, r5
	blx	ip
.L1684:
	mov	r3, #1
	strb	r3, [r10, #216]
	b	.L1683
.L1697:
	ldr	r3, [ip, #280]
	mov	r0, r5
	ldr	r2, [ip, #1276]
	ldr	r1, [ip, #1272]
	ldr	ip, [ip, #284]
	str	ip, [sp]
	bl	VCTRL_PartitionMemForStdUser
	cmp	r0, #0
	bne	.L1698
	ldr	r3, .L1755
	ldr	ip, [r3, r5, asl #2]
	add	r3, ip, #229376
	ldr	r2, [ip, #528]
	b	.L1696
.L1741:
	ldr	r3, .L1755+12
	mov	r0, r9
	ldr	r1, .L1755+44
	mvn	r9, #0
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1727
.L1748:
	ldr	r3, [r7, #164]
	ldr	r0, [r10, #220]
	blx	r3
	b	.L1678
.L1753:
	ldr	r1, [r10, #220]
	ldr	r0, [r10, #224]
	bl	MEM_DelMemRecord
	subs	r3, r0, #0
	str	r3, [fp, #-132]
	bne	.L1754
	ldr	r3, [fp, #-128]
	ldr	r2, [r10, #228]
	ldr	r0, [r10, #224]
	str	r3, [r10, #220]
	mov	r1, r3
	bl	MEM_AddMemRecord
	cmp	r0, #0
	beq	.L1732
	ldr	r3, [r10, #228]
	mvn	r9, #0
	ldr	r2, .L1755+12
	ldr	r0, [fp, #-132]
	ldr	r1, .L1755+48
	ldr	r4, [r2, #68]
	str	r3, [sp]
	ldr	r3, [r10, #220]
	ldr	r2, [r10, #224]
	blx	r4
	b	.L1727
.L1738:
	ldr	ip, .L1755+12
	mov	r0, r9
	ldr	r3, .L1755+52
	movw	r2, #2969
	ldr	r1, .L1755+40
	mvn	r9, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L1727
.L1751:
	ldr	r1, [r10, #204]
	mov	r0, r3
	ldr	r2, [r10, #208]
	mvn	r9, #0
	ldr	lr, [r10, #212]
.L1735:
	ldr	r3, .L1755+12
	ldr	ip, [r3, #68]
	mov	r3, r1
	str	lr, [sp]
	ldr	r1, .L1755+48
	blx	ip
	b	.L1727
.L1739:
	ldr	ip, .L1755+12
	mov	r0, r3
	movw	r2, #2970
	ldr	r3, .L1755+56
	ldr	r1, .L1755+40
	mvn	r9, #0
	ldr	ip, [ip, #68]
	blx	ip
	b	.L1727
.L1698:
	ldr	r3, [r9, #68]
	mov	r0, #1
	ldr	r1, .L1755+60
	mvn	r9, #0
	blx	r3
	b	.L1727
.L1754:
	mov	r0, r9
	ldr	r1, [r10, #220]
	ldr	r2, [r10, #224]
	mvn	r9, #0
	ldr	lr, [r10, #228]
	b	.L1735
.L1752:
	ldr	lr, [r10, #212]
	mvn	r9, #0
	ldr	r1, .L1755+12
	ldr	r3, [r10, #204]
	ldr	r2, [r10, #208]
	ldr	ip, [r1, #68]
	ldr	r0, [fp, #-136]
	ldr	r1, .L1755+48
	str	lr, [sp]
	blx	ip
	b	.L1727
.L1686:
	ldr	r3, [r7, #68]
	mov	r2, r5
	ldr	r1, .L1755+64
	mov	r0, #0
	blx	r3
	b	.L1734
.L1750:
	ldr	r3, .L1755+12
	mvn	r9, #0
	ldr	r2, .L1755+68
	ldr	r1, .L1755+72
	ldr	ip, [r3, #68]
	movw	r3, #3086
	blx	ip
	b	.L1727
.L1756:
	.align	2
.L1755:
	.word	.LANCHOR0
	.word	.LANCHOR3
	.word	.LANCHOR3+1052
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR3+1056
	.word	.LANCHOR0-4
	.word	g_ChanCtx
	.word	.LC151
	.word	.LC152
	.word	.LC21
	.word	.LC1
	.word	.LC150
	.word	.LC155
	.word	.LC6
	.word	.LC2
	.word	.LC156
	.word	.LC154
	.word	.LANCHOR1+236
	.word	.LC153
	UNWIND(.fnend)
	.size	VCTRL_ConfigChan, .-VCTRL_ConfigChan
	.align	2
	.global	VCTRL_SetDbgOption
	.type	VCTRL_SetDbgOption, %function
VCTRL_SetDbgOption:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 104
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #116)
	sub	sp, sp, #116
	cmp	r1, #0
	beq	.L1836
	cmp	r0, #516
	ldr	r4, [r1]
	beq	.L1759
	bls	.L1861
	cmp	r0, #1280
	beq	.L1781
	bhi	.L1782
	movw	r3, #898
	cmp	r0, r3
	beq	.L1783
	bhi	.L1784
	movw	r3, #769
	cmp	r0, r3
	beq	.L1785
	bls	.L1862
	cmp	r0, #896
	beq	.L1788
	movw	r3, #897
	cmp	r0, r3
	bne	.L1836
	mov	r0, r4
	bl	VCTRL_ResetChan
	ldr	r3, .L1872
	ldr	r1, .L1872+4
	mov	r2, r4
.L1855:
	ldr	ip, [r3, #68]
	mov	r3, r0
	mov	r0, #32
	blx	ip
.L1858:
	mov	r0, #1
.L1758:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1782:
	movw	r3, #1537
	cmp	r0, r3
	beq	.L1795
	bhi	.L1796
	movw	r3, #1282
	cmp	r0, r3
	beq	.L1797
	bcc	.L1798
	movw	r3, #1283
	cmp	r0, r3
	beq	.L1799
	cmp	r0, #1536
	bne	.L1836
	cmp	r4, #15
	bhi	.L1854
	ldr	ip, .L1872+8
	mov	r5, #1
	ldr	r3, .L1872
	mov	r2, r4
	ldr	r1, .L1872+12
	mov	r0, #32
	ldr	ip, [ip, r4, asl #2]
	ldr	r3, [r3, #68]
	str	r5, [ip, #1232]
	blx	r3
	mov	r0, r5
	b	.L1758
.L1861:
	cmp	r0, #8
	beq	.L1858
	bls	.L1863
	movw	r3, #257
	cmp	r0, r3
	beq	.L1769
	bhi	.L1770
	cmp	r0, #12
	beq	.L1771
	bls	.L1864
	cmp	r0, #13
	beq	.L1774
	cmp	r0, #256
	bne	.L1836
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+16
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, r4
	bl	DBG_PrintTracer
	b	.L1858
.L1784:
	cmp	r0, #1024
	beq	.L1790
	bls	.L1865
	movw	r3, #1025
	cmp	r0, r3
	beq	.L1793
	movw	r3, #1026
	cmp	r0, r3
	bne	.L1836
	ldr	ip, .L1872+20
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+24
	str	r4, [ip, #640]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1770:
	movw	r3, #513
	cmp	r0, r3
	beq	.L1776
	bls	.L1866
	movw	r3, #514
	cmp	r0, r3
	beq	.L1779
	movw	r3, #515
	cmp	r0, r3
	bne	.L1836
	ldr	r5, .L1872
	mov	r0, r4
	ldr	r3, [r5, #184]
	blx	r3
	subs	r7, r0, #0
	beq	.L1859
	mov	r0, #16
	ldr	r6, [r5, #72]
	bl	VFMW_GetString
	str	r4, [sp]
	mov	r1, #50
	ldr	r3, .L1872+28
	mov	r2, r0
	sub	r0, fp, #148
	blx	r6
	sub	r0, fp, #148
	ldr	r3, [r5, #32]
	mov	r2, #0
	mov	r1, #66
	blx	r3
	subs	r4, r0, #0
	beq	.L1827
	mov	r2, r4
	mov	r1, #3145728
	ldr	r3, [r5, #44]
	mov	r0, r7
	blx	r3
	mov	r0, r4
	ldr	r3, [r5, #36]
	blx	r3
	sub	r2, fp, #148
	ldr	r3, [r5, #68]
	mov	r0, #32
	ldr	r1, .L1872+32
	blx	r3
.L1829:
	mov	r0, r7
	ldr	r3, [r5, #156]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1796:
	movw	r3, #1539
	cmp	r0, r3
	beq	.L1801
	bcc	.L1802
	movw	r3, #1540
	cmp	r0, r3
	beq	.L1803
	movw	r3, #1541
	cmp	r0, r3
	bne	.L1836
	ldr	r3, .L1872
	mov	r2, r4
	ldr	r1, .L1872+36
	mov	r0, #32
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1858
.L1863:
	cmp	r0, #3
	beq	.L1858
	bls	.L1867
	cmp	r0, #5
	beq	.L1766
	bcc	.L1767
	cmp	r0, #6
	beq	.L1858
	cmp	r0, #7
	bne	.L1836
	ldr	r3, .L1872
	mov	r5, r4, lsr #24
	bic	r4, r4, #-16777216
	ldr	r1, .L1872+40
	mov	r2, r5
	mov	r0, #32
	ldr	ip, [r3, #68]
	mov	r3, r4
	blx	ip
	mov	r0, r5
	sub	r1, fp, #148
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1858
	sub	r1, fp, #148
	mov	r0, r5
	str	r4, [fp, #-128]
	bl	VCTRL_ConfigChan
	b	.L1858
.L1867:
	cmp	r0, #1
	beq	.L1858
	ldrls	r3, .L1872+44
	movls	r0, #1
	strls	r4, [r3]
	bls	.L1758
.L1868:
	ldr	r3, .L1872
	mov	r5, r4, lsr #24
	bic	r4, r4, #-16777216
	sub	r6, fp, #148
	mov	r2, r5
	ldr	r1, .L1872+48
	ldr	ip, [r3, #68]
	mov	r0, #32
	mov	r3, r4
	blx	ip
	mov	r0, r5
	mov	r1, r6
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1858
	mov	r1, r6
	mov	r0, r5
	str	r4, [fp, #-132]
	bl	VCTRL_ConfigChan
	b	.L1858
.L1799:
	ldr	ip, .L1872+52
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+56
	str	r4, [ip]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1767:
	ldr	r3, .L1872
	mov	r5, r4, lsr #24
	and	r4, r4, #3
	sub	r6, fp, #148
	mov	r2, r5
	ldr	r1, .L1872+60
	ldr	ip, [r3, #68]
	mov	r0, #32
	mov	r3, r4
	blx	ip
	mov	r0, r5
	mov	r1, r6
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1858
	mov	r1, r6
	mov	r0, r5
	str	r4, [fp, #-120]
	bl	VCTRL_ConfigChan
	b	.L1858
.L1788:
	mov	r0, r4
	bl	VCTRL_StopChan
	ldr	r3, .L1872
	mov	r2, r4
	ldr	r1, .L1872+64
	b	.L1855
.L1779:
	cmp	r4, #15
	bhi	.L1858
	ldr	r6, .L1872+20
	add	r7, r6, r4, lsl #2
	ldr	r5, [r7, #1268]
	cmp	r5, #0
	beq	.L1869
	ldr	r5, .L1872
	mov	r0, #32
	ldr	r1, .L1872+68
	mov	r4, #0
	ldr	r3, [r5, #68]
	blx	r3
	ldr	r3, [r5, #36]
	ldr	r0, [r7, #1268]
	blx	r3
	ldr	r0, [r6, #1408]
	ldr	r3, .L1872+72
	mvn	r2, #0
	cmp	r0, r4
	str	r4, [r7, #1268]
	str	r4, [r6, #1416]
	str	r2, [r3, #68]
	str	r4, [r6, #1420]
	str	r4, [r6, #1424]
	str	r4, [r6, #1428]
	beq	.L1858
	ldr	r1, [r6, #1404]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [r6, #1412]
	ldr	r1, [r6, #1404]
	ldr	r0, [r6, #1408]
	bl	MEM_DelMemRecord
	ldr	r3, [r5, #48]
	mov	r1, r4
	mov	r2, #16
	ldr	r0, .L1872+76
	blx	r3
	mov	r0, #1
	b	.L1758
.L1803:
	mov	r2, r4, asl #6
	ldr	r3, .L1872+80
	sub	r4, r2, r4, asl #4
	mov	r0, #1
	add	r4, r3, r4
	mov	r2, #0
	ldr	r3, [r4, #40]
	str	r2, [r3, #80]
	b	.L1758
.L1774:
	cmp	r4, #16
	ldrhi	r3, .L1872
	movhi	r2, r4
	ldrls	ip, .L1872+20
	movhi	r0, #32
	ldrls	r3, .L1872
	movls	r2, r4
	ldrhi	r1, .L1872+84
	movls	r0, #32
	ldrls	r1, .L1872+88
	strls	r4, [ip, #1120]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1793:
	ldr	ip, .L1872+20
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+92
	str	r4, [ip, #1432]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1836:
	mvn	r0, #1
	b	.L1758
.L1865:
	movw	r3, #899
	cmp	r0, r3
	bne	.L1836
	mov	r0, r4
	ldr	r5, .L1872
	bl	VCTRL_StopChan
	mov	r2, r4
	ldr	r1, .L1872+64
	ldr	ip, [r5, #68]
	mov	r3, r0
	mov	r0, #32
	blx	ip
	mov	r0, r4
	bl	VCTRL_ResetChan
	ldr	ip, [r5, #68]
	mov	r2, r4
	ldr	r1, .L1872+4
	mov	r3, r0
	mov	r0, #32
	blx	ip
	mov	r0, r4
	bl	VCTRL_StartChan
	ldr	ip, [r5, #68]
	mov	r2, r4
	ldr	r1, .L1872+96
	mov	r3, r0
	mov	r0, #32
	blx	ip
	mov	r0, #1
	b	.L1758
.L1864:
	cmp	r0, #11
	bne	.L1836
	cmp	r4, #1
	ldrls	ip, .L1872+100
	movls	r2, r4
	ldrls	r3, .L1872
	movls	r0, #32
	ldrhi	r3, .L1872
	movhi	r2, r4
	strls	r4, [ip]
	movhi	r0, #32
	ldrls	r1, .L1872+104
	ldrhi	r1, .L1872+108
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1862:
	cmp	r0, #768
	bne	.L1836
	bl	VCTRL_Suspend
	mov	r0, #1
	b	.L1758
.L1866:
	cmp	r0, #512
	bne	.L1836
	cmp	r4, #15
	bhi	.L1858
	ldr	r6, .L1872+20
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #712]
	cmp	r7, #0
	beq	.L1870
	ldr	r4, .L1872
	mov	r0, #32
	ldr	r1, .L1872+112
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r3, [r4, #36]
	ldr	r0, [r5, #712]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #712]
	b	.L1758
.L1790:
	ldr	ip, .L1872+20
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+116
	str	r4, [ip, #636]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1771:
	cmp	r4, #1
	ldrls	ip, .L1872+20
	movls	r2, r4
	ldrls	r3, .L1872
	movls	r0, #32
	ldrhi	r3, .L1872
	movhi	r2, r4
	ldrls	r1, .L1872+120
	movhi	r0, #32
	strls	r4, [ip, #708]
	ldrhi	r1, .L1872+124
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1785:
	bl	VCTRL_Resume
	mov	r0, #1
	b	.L1758
.L1776:
	cmp	r4, #15
	bhi	.L1858
	ldr	r5, .L1872+20
	mov	r6, r4, asl #2
	add	r3, r5, r6
	ldr	r3, [r3, #1132]
	cmp	r3, #0
	beq	.L1871
	ldr	r4, .L1872
	add	r5, r5, r6
	ldr	r1, .L1872+128
	mov	r0, #32
	ldr	r3, [r4, #68]
	blx	r3
	ldr	r3, [r4, #36]
	ldr	r0, [r5, #1132]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #1132]
	b	.L1758
.L1766:
	ldr	r6, .L1872
	mov	r5, r4, lsr #24
	and	r4, r4, #7
	ldr	r1, .L1872+132
	mov	r2, r5
	mov	r0, #32
	ldr	ip, [r6, #68]
	mov	r3, r4
	blx	ip
	mov	r0, r5
	sub	r1, fp, #148
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1807
	sub	r1, fp, #148
	mov	r0, r5
	str	r4, [fp, #-124]
	bl	VCTRL_ConfigChan
	b	.L1858
.L1781:
	ldr	ip, .L1872+136
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+140
	str	r4, [ip]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1802:
	ldr	ip, .L1872+144
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+12
	str	r4, [ip]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1801:
	mov	r2, r4, asl #6
	ldr	r3, .L1872+80
	sub	r4, r2, r4, asl #4
	mov	r2, #1
	add	r4, r3, r4
	mov	r0, r2
	ldr	r3, [r4, #40]
	str	r2, [r3, #80]
	b	.L1758
.L1854:
	ldr	r3, .L1872
	mov	r0, #0
	ldr	r1, .L1872+148
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1858
.L1798:
	ldr	ip, .L1872+152
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+156
	str	r4, [ip]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1797:
	ldr	ip, .L1872+160
	mov	r2, r4
	ldr	r3, .L1872
	mov	r0, #32
	ldr	r1, .L1872+164
	str	r4, [ip]
	ldr	r3, [r3, #68]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1759:
	ldr	r5, .L1872
	mov	r0, r4
	ldmib	r1, {r8, r9}
	ldr	r3, [r5, #184]
	blx	r3
	subs	r7, r0, #0
	beq	.L1859
	mov	r0, #17
	ldr	r10, [r5, #72]
	bl	VFMW_GetString
	sub	r6, fp, #148
	str	r4, [sp]
	mov	r1, #50
	ldr	r3, .L1872+28
	mov	r2, r0
	mov	r0, r6
	blx	r10
	mov	r0, r6
	ldr	r3, [r5, #32]
	mov	r2, #0
	mov	r1, #66
	blx	r3
	subs	r4, r0, #0
	beq	.L1830
	ldr	r3, .L1872+168
	ldr	ip, [r3, #116]
	cmp	ip, #0
	beq	.L1831
	mov	lr, #1
	mov	r3, r9
	mov	r2, r8
	mov	r1, r7
	str	lr, [sp]
	blx	ip
.L1832:
	mov	r0, r4
	ldr	r3, [r5, #36]
	blx	r3
	mov	r2, r6
	ldr	r3, [r5, #68]
	mov	r0, #32
	ldr	r1, .L1872+172
	blx	r3
	b	.L1829
.L1783:
	mov	r0, r4
	bl	VCTRL_StartChan
	ldr	r3, .L1872
	mov	r2, r4
	ldr	r1, .L1872+96
	b	.L1855
.L1769:
	ldr	r5, .L1872+44
	mov	r0, r4
	mov	r1, #1
	mov	r3, #67108864
	ldr	r4, [r5]
	str	r3, [r5]
	bl	PrintScdRawState
	mov	r0, #1
	str	r4, [r5]
	b	.L1758
.L1795:
	cmp	r4, #15
	bhi	.L1854
	ldr	ip, .L1872+8
	mov	lr, #0
	ldr	r3, .L1872
	mov	r2, r4
	ldr	r1, .L1872+12
	mov	r0, #32
	ldr	ip, [ip, r4, asl #2]
	ldr	r3, [r3, #68]
	str	lr, [ip, #1232]
	blx	r3
	mov	r0, #1
	b	.L1758
.L1807:
	ldr	r3, [r6, #68]
	mov	r2, r5
	ldr	r1, .L1872+176
	mov	r0, #32
	blx	r3
	b	.L1858
.L1859:
	mov	r2, r4
	ldr	r3, [r5, #68]
	ldr	r1, .L1872+180
	mov	r0, #32
	blx	r3
	b	.L1829
.L1870:
	ldr	r8, .L1872
	mov	r0, #13
	ldr	r9, [r8, #72]
	bl	VFMW_GetString
	ldr	r2, [r6, #1124]
	mov	r1, #50
	ldr	r3, .L1872+28
	add	ip, r2, #1
	str	ip, [r6, #1124]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #148
	blx	r9
	mov	r2, r7
	ldr	r3, [r8, #32]
	sub	r0, fp, #148
	mov	r1, #66
	blx	r3
	str	r4, [r6, #1128]
	cmp	r0, #0
	str	r0, [r5, #712]
	beq	.L1815
	ldr	ip, [r8, #68]
	mov	r3, r4
	sub	r2, fp, #148
	ldr	r1, .L1872+184
	mov	r0, #32
	blx	ip
	b	.L1858
.L1871:
	ldr	r7, .L1872
	mov	r0, #14
	add	r6, r5, r6
	ldr	r8, [r7, #72]
	bl	VFMW_GetString
	ldr	r2, [r5, #1260]
	mov	r1, #50
	ldr	r3, .L1872+28
	add	ip, r2, #1
	str	ip, [r5, #1260]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #148
	blx	r8
	ldr	r3, [r7, #32]
	sub	r0, fp, #148
	mov	r2, #0
	mov	r1, #66
	blx	r3
	str	r4, [r5, #1264]
	cmp	r0, #0
	str	r0, [r6, #1132]
	beq	.L1819
	ldr	ip, [r7, #68]
	mov	r3, r4
	sub	r2, fp, #148
	ldr	r1, .L1872+188
	mov	r0, #32
	blx	ip
	b	.L1858
.L1827:
	sub	r2, fp, #148
	ldr	r3, [r5, #68]
	ldr	r1, .L1872+192
	mov	r0, #32
	blx	r3
	b	.L1829
.L1831:
	mov	r0, ip
	ldr	r3, [r5, #68]
	movw	r2, #8504
	ldr	r1, .L1872+196
	blx	r3
	b	.L1832
.L1869:
	ldr	r8, .L1872
	mov	r0, #15
	ldr	r9, .L1872+72
	ldr	r10, [r8, #72]
	bl	VFMW_GetString
	ldr	r2, [r6, #1396]
	add	r3, r9, #4
	mov	r1, #100
	add	ip, r2, #1
	str	ip, [r6, #1396]
	str	r2, [sp]
	mov	r2, r0
	sub	r0, fp, #148
	blx	r10
	ldr	r3, [r8, #32]
	sub	r0, fp, #148
	mov	r2, r5
	movw	r1, #578
	blx	r3
	cmp	r0, #0
	str	r0, [r7, #1268]
	beq	.L1822
	ldr	r3, [r8, #48]
	mov	r2, #16
	mov	r1, r5
	ldr	r0, .L1872+76
	blx	r3
	mov	r0, #19
	bl	VFMW_GetString
	str	r5, [sp]
	ldr	r3, .L1872+76
	mov	r2, #14155776
	mov	r1, r5
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	beq	.L1823
	ldr	r3, [r8, #68]
	mov	r0, #1
	ldr	r1, .L1872+200
	blx	r3
	ldr	r3, [r8, #36]
	ldr	r0, [r7, #1268]
	blx	r3
	mvn	r3, #0
	str	r5, [r7, #1268]
	str	r5, [r6, #1416]
	str	r5, [r6, #1420]
	str	r5, [r6, #1424]
	str	r5, [r6, #1428]
	str	r3, [r9, #68]
	b	.L1858
.L1830:
	mov	r2, r6
	ldr	r3, [r5, #68]
	ldr	r1, .L1872+192
	mov	r0, #32
	blx	r3
	b	.L1829
.L1823:
	ldr	r2, [r6, #1412]
	ldr	r1, [r6, #1404]
	ldr	r0, [r6, #1408]
	bl	MEM_AddMemRecord
	ldr	r1, [r6, #1404]
	mov	r0, r4, lsr #24
	ldr	ip, [r8, #68]
	add	r3, r1, #9437184
	and	r4, r4, #3
	str	r1, [r6, #1424]
	sub	r2, fp, #148
	str	r3, [r6, #1420]
	add	r1, r1, #11796480
	str	r0, [r9, #68]
	mov	r3, r0
	str	r1, [r6, #1416]
	mov	r0, #32
	str	r4, [r6, #1428]
	ldr	r1, .L1872+204
	str	r4, [sp]
	blx	ip
	b	.L1858
.L1815:
	ldr	r3, [r8, #68]
	sub	r2, fp, #148
	ldr	r1, .L1872+208
	mov	r0, #32
	blx	r3
	b	.L1858
.L1819:
	ldr	r3, [r7, #68]
	sub	r2, fp, #148
	ldr	r1, .L1872+212
	mov	r0, #32
	blx	r3
	b	.L1858
.L1822:
	ldr	r3, [r8, #68]
	sub	r2, fp, #148
	ldr	r1, .L1872+216
	mov	r0, #32
	blx	r3
	b	.L1858
.L1873:
	.align	2
.L1872:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC184
	.word	.LANCHOR0
	.word	.LC194
	.word	.LC168
	.word	.LANCHOR3
	.word	.LC188
	.word	.LANCHOR2+4
	.word	.LC180
	.word	.LC195
	.word	.LC161
	.word	g_PrintEnable
	.word	.LC157
	.word	g_TraceImgPeriod
	.word	.LC192
	.word	.LC158
	.word	.LC183
	.word	.LC178
	.word	.LANCHOR2
	.word	.LANCHOR3+1400
	.word	s_SmIIS
	.word	.LC166
	.word	.LC167
	.word	.LC187
	.word	.LC185
	.word	USE_FF_APT_EN
	.word	.LC162
	.word	.LC163
	.word	.LC171
	.word	.LC186
	.word	.LC164
	.word	.LC165
	.word	.LC174
	.word	.LC159
	.word	g_TraceCtrl
	.word	.LC189
	.word	g_TunnelLineNumber
	.word	.LC193
	.word	g_TraceBsPeriod
	.word	.LC190
	.word	g_TraceFramePeriod
	.word	.LC191
	.word	g_vdm_hal_fun_ptr
	.word	.LC182
	.word	.LC160
	.word	.LC179
	.word	.LC169
	.word	.LC172
	.word	.LC181
	.word	.LC16
	.word	.LC175
	.word	.LC176
	.word	.LC170
	.word	.LC173
	.word	.LC177
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
	ldr	r5, .L1907
	mov	r4, r0
	mov	r6, r1
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L1901
	ldr	r2, [r1]
	cmp	r2, #1
	bhi	.L1902
	ldr	r9, [r3, #16]
	bl	VCTRL_StopChan
	ldr	r8, .L1907+4
	add	r0, r5, #4016
	ldr	r3, [r5, r4, asl #2]
	mov	r7, #0
	mov	r2, #300
	mov	r1, r7
	ldr	ip, [r8, #48]
	str	r7, [r3, #328]
	str	r7, [r3, #332]
	mla	r0, r2, r4, r0
	blx	ip
	ldr	r3, [r5, r4, asl #2]
	cmp	r3, r7
	addne	r3, r3, #225280
	strne	r7, [r3, #3300]
	beq	.L1903
.L1879:
	ldr	r2, [r6, #4]
	mov	r0, r4
	ldr	r3, [r5, r4, asl #2]
	str	r2, [r3, #1224]
	bl	InitSyntaxCtx
	ldr	r3, [r5, r4, asl #2]
	add	r1, r3, #229376
	ldr	r0, [r3, #528]
	add	r1, r1, #760
	bl	ReleaseCurPic
	ldr	r0, [r5, r4, asl #2]
	ldr	r3, [r8, #48]
	mov	r2, #416
	mov	r1, #0
	add	r0, r0, #664
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	mvn	r0, #0
	mvn	r1, #0
	add	r2, r3, #592
	mov	ip, #0
	strd	r0, [r2, #-8]
	strd	r0, [r2]
	mov	r0, r4
	ldr	r2, [r3, #1388]
	str	ip, [r3, #1080]
	cmp	r2, #1
	streq	ip, [r3, #1428]
	ldr	r3, [r3, #528]
	cmp	r3, #19
	beq	.L1881
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq	.L1904
.L1881:
	bl	FSP_EmptyInstance
.L1882:
	mov	r0, r4
	bl	InitUsdPool
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L1905
.L1883:
	ldr	r3, .L1907+8
	ldr	r2, [r3, #708]
	cmp	r2, #0
	beq	.L1884
	add	r5, r3, r4, lsl #2
	str	r4, [fp, #-40]
	ldr	r3, [r5, #712]
	cmp	r3, #0
	beq	.L1886
	mov	r0, #512
	sub	r1, fp, #40
	bl	VCTRL_SetDbgOption
	ldr	r3, [r5, #712]
	cmp	r3, #0
	beq	.L1886
	sub	r1, fp, #40
	mov	r0, #512
	bl	VCTRL_SetDbgOption
.L1886:
	ldr	r3, [r8, #68]
	mov	r0, #32
	ldr	r1, .L1907+12
	blx	r3
.L1884:
	cmp	r9, #0
	bne	.L1906
.L1888:
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	mov	r0, #0
.L1876:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1905:
	mov	r0, r4
	bl	SM_Reset
	b	.L1883
.L1906:
	mov	r0, r4
	bl	VCTRL_StartChan
	b	.L1888
.L1904:
	bl	FSP_ResetInstance
	b	.L1882
.L1902:
	ldr	r3, .L1907+4
	movw	r2, #3751
	ldr	r1, .L1907+16
	mov	r0, #0
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1903:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1879
.L1901:
	ldr	ip, .L1907+4
	mov	r0, r3
	movw	r2, #3747
	ldr	r3, .L1907+20
	ldr	r1, .L1907+24
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L1876
.L1908:
	.align	2
.L1907:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR3
	.word	.LC134
	.word	.LC196
	.word	.LC2
	.word	.LC1
	UNWIND(.fnend)
	.size	VCTRL_ResetChanWithOption, .-VCTRL_ResetChanWithOption
	.align	2
	.global	VCTRL_DestroyChanWithOption
	.type	VCTRL_DestroyChanWithOption, %function
VCTRL_DestroyChanWithOption:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 96
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #100)
	sub	sp, sp, #100
	ldr	r5, .L1925
	mov	r4, r0
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	cmp	r4, #15
	bhi	.L1920
	ldr	r6, .L1925+4
	ldr	r7, [r6, r4, asl #2]
	cmp	r7, #0
	beq	.L1921
	ldr	r3, [r7, #12]
	cmp	r3, #1
	bne	.L1922
	ldr	r2, .L1925+8
	mov	r8, #0
	ldr	r3, [r5, #12]
	mov	r0, #4
	str	r8, [r2, r4, asl #2]
	blx	r3
	mov	r0, r4
	bl	VCTRL_StopChan
	subs	r9, r0, #0
	bne	.L1923
	mov	r0, r4
	bl	SCDDRV_ResetSCD
	ldr	r3, [r5, #16]
	mov	r0, #4
	blx	r3
	mov	r0, r4
	bl	VCTRL_ResetChan
	bl	VDMDRV_ParamInit
	add	r3, r6, r4, lsl #2
	ldr	r2, [r5, #12]
	mov	r0, #4
	str	r9, [r3, #3760]
	blx	r2
	ldr	r3, [r7, #528]
	cmp	r3, #4
	beq	.L1924
.L1915:
	mov	r0, r4
	mov	r7, #0
	bl	FSP_DisableInstance
	mov	r0, r4
	bl	CloseUsdPool_WithMem
	ldr	r3, [r6, r4, asl #2]
	ldr	r8, [r5, #52]
	mov	r2, #16
	add	r1, r3, #232
	add	ip, r3, #228352
	ldr	lr, [r3, #252]
	add	ip, ip, #232
	ldr	r10, [r3, #256]
	sub	r0, fp, #108
	ldr	r9, [r3, #248]
	ldr	r3, [r3, #1388]
	str	lr, [fp, #-72]
	str	ip, [fp, #-48]
	str	r3, [fp, #-52]
	str	r10, [fp, #-68]
	str	r9, [fp, #-76]
	blx	r8
	ldr	r1, [r6, r4, asl #2]
	ldr	r3, [r5, #52]
	mov	r2, #16
	add	r1, r1, #200
	sub	r0, fp, #140
	blx	r3
	ldr	r1, [r6, r4, asl #2]
	ldr	r3, [r5, #52]
	mov	r2, #16
	add	r1, r1, #216
	sub	r0, fp, #124
	blx	r3
	ldr	r8, .L1925+12
	mov	r3, r4, asl #6
	ldr	ip, [r5, #52]
	sub	r3, r3, r4, asl #4
	mov	r2, #16
	add	r8, r3, r8
	sub	r0, fp, #92
	mov	r1, r8
	blx	ip
	ldr	r1, [r6, r4, asl #2]
	mov	r2, #12
	ldr	r3, [r5, #52]
	add	r1, r1, #152
	sub	r0, fp, #64
	blx	r3
	ldr	r3, .L1925+16
	mov	r0, r4
	str	r7, [r8, #40]
	str	r7, [r6, r4, asl #2]
	str	r7, [r3, r4, asl #2]
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r4
	bl	VDMDRV_ResetVDH
	ldr	r3, [r5, #16]
	mov	r0, #4
	blx	r3
	bl	VCTRL_EnableAllInt
	ldr	r3, [r5, #16]
	mov	r0, r7
	blx	r3
	sub	r1, fp, #140
	mov	r0, r4
	bl	VCTRL_FreeChanResource
	mov	r0, r7
.L1911:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1924:
	ldr	r3, [r6, #244]
	cmp	r3, #0
	beq	.L1915
	add	r0, r7, #229376
	add	r0, r0, #760
	blx	r3
	b	.L1915
.L1922:
	ldr	ip, [r5, #68]
	movw	r2, #2522
	ldr	r3, .L1925+20
.L1919:
	ldr	r1, .L1925+24
	mov	r0, #0
	blx	ip
	bl	VCTRL_EnableAllInt
	ldr	r3, [r5, #16]
	mov	r0, #0
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1923:
	ldr	r3, [r5, #16]
	mov	r0, #4
	blx	r3
	ldr	r3, [r5, #16]
	mov	r0, r8
	blx	r3
	mvn	r0, #0
	b	.L1911
.L1920:
	ldr	ip, [r5, #68]
	mov	r2, #2496
	ldr	r3, .L1925+28
	b	.L1919
.L1921:
	ldr	ip, [r5, #68]
	movw	r2, #2507
	ldr	r3, .L1925+32
	mov	r0, r7
	ldr	r1, .L1925+24
	blx	ip
	bl	VCTRL_EnableAllInt
	ldr	r3, [r5, #16]
	mov	r0, r7
	blx	r3
	mvn	r0, #0
	b	.L1911
.L1926:
	.align	2
.L1925:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0
	.word	g_LowDelaySeqIndex
	.word	s_SmIIS
	.word	s_pFspInst
	.word	.LC199
	.word	.LC197
	.word	.LC6
	.word	.LC198
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
	ldr	r6, .L1946
	ldr	r3, [r6, #64]
	cmp	r3, #1
	bne	.L1935
	sub	r5, r6, #4
	mov	r4, #0
.L1931:
	ldr	r3, [r5, #4]!
	mov	r0, r4
	add	r4, r4, #1
	cmp	r3, #0
	beq	.L1930
	bl	VCTRL_DestroyChanWithOption
.L1930:
	cmp	r4, #16
	bne	.L1931
	bl	FSP_Exit
	ldr	r3, [r6, #68]
	mov	r2, #3
	ldr	r5, .L1946+4
	cmp	r3, #0
	str	r2, [r6, #72]
	beq	.L1933
	mov	r0, #10
	ldr	r3, [r5, #120]
	mov	r4, #49
	blx	r3
	b	.L1934
.L1945:
	ldr	r3, [r5, #120]
	blx	r3
	subs	r4, r4, #1
	beq	.L1933
.L1934:
	ldr	r3, [r6, #68]
	mov	r0, #10
	cmp	r3, #0
	bne	.L1945
.L1933:
	ldr	r3, [r5, #48]
	mov	r2, #64
	mov	r1, #0
	ldr	r0, .L1946+8
	blx	r3
	ldr	r0, .L1946+12
	bl	CloseDrivers
	bl	DBG_DestroyTracer
	mov	r3, #0
	mov	r0, r3
	str	r3, [r6, #64]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1935:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1947:
	.align	2
.L1946:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR0+3760
	.word	.LANCHOR0+88
	UNWIND(.fnend)
	.size	VCTRL_CloseVfmw, .-VCTRL_CloseVfmw
	.align	2
	.global	VCTRL_LoadDspCode
	.type	VCTRL_LoadDspCode, %function
VCTRL_LoadDspCode:
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
	ldr	r8, .L1992
	mov	r6, r0
	ldr	r2, [r8, r0, asl #2]
	ldr	r3, [r2, #528]
	cmp	r3, #0
	bne	.L1949
	ldr	r2, [r2, #1236]
	ldr	r5, [r8, #3760]
	sub	r7, r2, #1
	clz	r7, r7
	cmp	r5, #0
	mov	r7, r7, lsr #5
	bne	.L1950
.L1969:
	ldr	ip, .L1992+4
	add	lr, ip, #60
.L1954:
	ldr	r5, [ip, #4]!
	cmp	r5, #0
	bne	.L1950
	cmp	ip, lr
	bne	.L1954
.L1961:
	ldr	r4, .L1992+8
	ldr	r3, [r4]
	cmp	r3, #0
	beq	.L1988
	mov	r0, #0
	blx	r3
.L1956:
	ldr	r3, [r4, #8]
	cmp	r3, #0
	beq	.L1989
	mov	r0, #0
	blx	r3
.L1958:
	clz	r5, r5
	mov	r5, r5, lsr #5
	ands	r1, r5, r7
	beq	.L1990
	ldr	r3, [r8, r6, asl #2]
	add	r6, r8, r6, lsl #2
	mov	r1, #0
	ldr	r0, [r3, #528]
	bl	WriteHexFile
	mov	r3, #1
	str	r3, [r6, #3760]
.L1963:
	ldr	r1, .L1992+12
	mov	r0, #0
	add	lr, r1, #64
.L1966:
	ldr	r2, [r1, #4]!
	mov	r3, #0
	cmp	r2, r3
	beq	.L1964
	ldr	ip, [r2, #528]
	cmp	ip, r3
	ldreq	r2, [r2, #1236]
	subeq	r3, r2, #1
	clzeq	r3, r3
	moveq	r3, r3, lsr #5
	cmp	ip, #6
	orreq	r3, r3, #1
	cmp	r3, #0
	movne	r0, #1
.L1964:
	cmp	r1, lr
	bne	.L1966
	cmp	r0, #0
	bne	.L1967
	ldr	r3, [r4, #12]
	cmp	r3, #0
	beq	.L1991
	blx	r3
.L1967:
	bl	VCTRL_CloseHardware
	mov	r0, #0
.L1986:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1949:
	ldr	r5, [r8, #3760]
	cmp	r5, #0
	bne	.L1973
	mov	r2, r5
	mov	r7, r5
	b	.L1969
.L1990:
	ldr	r3, [r8, r6, asl #2]
	ldr	r7, .L1992
	ldr	r0, [r3, #528]
	cmp	r0, #6
	movne	r5, #0
	andeq	r5, r5, #1
	cmp	r5, #0
	beq	.L1963
	add	r6, r7, r6, lsl #2
	bl	WriteHexFile
	mov	r3, #2
	str	r3, [r6, #3760]
	b	.L1963
.L1950:
	cmp	r5, #2
	movne	r1, #0
	andeq	r1, r7, #1
	cmp	r1, #0
	bne	.L1952
.L1953:
	cmp	r5, #1
	cmpeq	r3, #6
	bne	.L1961
.L1952:
	ldr	ip, .L1992+16
	mov	r0, #0
	str	r5, [sp]
	ldr	r1, .L1992+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r0, #0
	b	.L1986
.L1989:
	ldr	ip, .L1992+16
	mov	r0, r3
	movw	r2, #3617
	ldr	r1, .L1992+24
	ldr	r3, [ip, #68]
	blx	r3
	b	.L1958
.L1988:
	ldr	ip, .L1992+16
	mov	r0, r3
	mov	r2, #3616
	ldr	r1, .L1992+24
	ldr	r3, [ip, #68]
	blx	r3
	b	.L1956
.L1991:
	ldr	r3, .L1992+16
	movw	r2, #3670
	ldr	r1, .L1992+24
	ldr	r3, [r3, #68]
	blx	r3
	b	.L1967
.L1973:
	mov	r2, #0
	mov	r7, r2
	b	.L1953
.L1993:
	.align	2
.L1992:
	.word	.LANCHOR0
	.word	.LANCHOR0+3760
	.word	g_vdm_hal_fun_ptr
	.word	.LANCHOR0-4
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC200
	.word	.LC16
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
	ldr	r3, .L1995
	ldr	r3, [r3, r0, asl #2]
	add	r3, r3, #225280
	str	r1, [r3, #3256]
	ldmfd	sp, {fp, sp, pc}
.L1996:
	.align	2
.L1995:
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
	ldr	r3, .L1998
	ldr	r3, [r3, r0, asl #2]
	add	r3, r3, #225280
	ldr	r0, [r3, #3272]
	ldmfd	sp, {fp, sp, pc}
.L1999:
	.align	2
.L1998:
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	ldr	r4, .L2012
	mov	r6, r0
	mov	r0, #0
	mov	r5, r1
	ldr	r3, [r4, #12]
	blx	r3
	cmp	r5, #1
	beq	.L2002
	bcc	.L2003
	cmp	r5, #2
	bne	.L2011
	ldr	r3, .L2012+4
	add	r6, r3, r6, lsl #2
	ldr	r3, [r6, #844]
	cmp	r3, #1
	beq	.L2007
.L2009:
	str	r5, [r6, #844]
	mov	r5, #0
	b	.L2001
.L2011:
	mov	r5, #0
.L2001:
	ldr	r3, [r4, #16]
	mov	r0, #0
	blx	r3
	mov	r0, r5
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2003:
	ldr	r3, .L2012+4
	mov	r2, #0
	mov	r5, r2
	add	r6, r3, r6, lsl #2
	str	r2, [r6, #844]
	b	.L2001
.L2002:
	ldr	r3, .L2012+4
	add	r6, r3, r6, lsl #2
	ldr	r3, [r6, #844]
	cmp	r3, #0
	beq	.L2009
.L2007:
	mvn	r5, #0
	b	.L2001
.L2013:
	.align	2
.L2012:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR3
	UNWIND(.fnend)
	.size	VCTRT_SetChanCtrlState, .-VCTRT_SetChanCtrlState
	.global	__aeabi_idiv
	.align	2
	.global	VCTRL_RequestChanMem
	.type	VCTRL_RequestChanMem, %function
VCTRL_RequestChanMem:
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
	ldr	r6, .L2029
	mov	r5, r0
	ldr	r3, [r6, r0, asl #2]
	add	ip, r3, #225280
	ldr	r2, [r3, #264]
	ldr	r1, [ip, #3272]
	ldr	r7, [ip, #3280]
	cmp	r1, #0
	ble	.L2026
	ldr	r3, [r3, #260]
	cmp	r3, #0
	cmpne	r2, #0
	ble	.L2017
	mov	r0, r2
	bl	__aeabi_idiv
	cmp	r7, r0
	mov	r9, r0
	ble	.L2018
	rsb	r8, r0, r7
	cmp	r8, #0
	ble	.L2018
	mov	r1, r0
	mov	r0, r5
	bl	VCTRL_AllocMemPre
	ldr	r7, .L2029+4
	ldr	r10, [r6, r5, asl #2]
	ldr	r3, [r7]
	add	r10, r10, #229376
	mov	r4, r0
	blx	r3
	ldr	r3, [r6, r5, asl #2]
	cmp	r4, #0
	add	r3, r3, #225280
	str	r0, [r10, #672]
	str	r9, [r3, #3284]
	bne	.L2019
	ldr	r3, [r3, #3292]
	cmp	r3, #1
	bne	.L2020
	mov	r1, r8
	mov	r0, r5
	bl	VCTRL_AllocMemMMZ
	subs	r4, r0, #0
	bne	.L2027
	ldr	r3, [r7]
	ldr	r7, [r6, r5, asl #2]
	blx	r3
	ldr	r2, [r6, r5, asl #2]
	mov	r3, r4
	add	r7, r7, #229376
	add	r2, r2, #225280
	str	r0, [r7, #676]
	mov	r0, r3
	str	r8, [r2, #3288]
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2017:
	ldr	r3, [ip, #3292]
	cmp	r3, #1
	bne	.L2022
	mov	r1, r7
	ldr	r8, .L2029+4
	bl	VCTRL_AllocMemMMZ
	ldr	r9, [r6, r5, asl #2]
	ldr	r3, [r8]
	add	r9, r9, #229376
	mov	r4, r0
	blx	r3
	ldr	r3, [r6, r5, asl #2]
	cmp	r4, #0
	add	r3, r3, #225280
	str	r0, [r9, #676]
	str	r7, [r3, #3288]
	bne	.L2028
.L2024:
	mov	r3, r4
.L2016:
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2018:
	mov	r1, r7
	mov	r0, r5
	bl	VCTRL_AllocMemPre
	ldr	r8, .L2029+4
	ldr	r9, [r6, r5, asl #2]
	ldr	r3, [r8]
	add	r9, r9, #229376
	mov	r4, r0
	blx	r3
	ldr	r3, [r6, r5, asl #2]
	cmp	r4, #0
	add	r3, r3, #225280
	str	r0, [r9, #672]
	str	r7, [r3, #3284]
	beq	.L2024
	ldr	ip, [r8, #68]
	movw	r3, #9358
	b	.L2025
.L2028:
	ldr	ip, [r8, #68]
	movw	r3, #9375
	ldr	r2, .L2029+8
	mov	r0, #31
	ldr	r1, .L2029+12
	blx	ip
	mov	r3, r4
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2019:
	ldr	ip, [r7, #68]
	mov	r3, #9344
.L2025:
	ldr	r2, .L2029+8
	mov	r0, #31
	ldr	r1, .L2029+16
	blx	ip
	mov	r3, r4
	mov	r0, r3
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2022:
	ldr	ip, .L2029+4
	mov	r0, #31
	ldr	r2, .L2029+8
	ldr	r1, .L2029+20
	ldr	ip, [ip, #68]
	blx	ip
	mvn	r3, #0
	b	.L2016
.L2020:
	ldr	ip, [r7, #68]
	mov	r0, #31
	ldr	r2, .L2029+8
	ldr	r1, .L2029+24
	blx	ip
	mvn	r3, #0
	b	.L2016
.L2027:
	movw	r3, #9327
	ldr	ip, [r7, #68]
	ldr	r2, .L2029+8
	mov	r0, #31
	ldr	r1, .L2029+12
	blx	ip
	mvn	r3, #0
	b	.L2016
.L2026:
	ldr	r3, .L2029+4
	mov	r2, r1
	mov	r0, #31
	ldr	r1, .L2029+28
	ldr	r3, [r3, #68]
	blx	r3
	mvn	r3, #0
	b	.L2016
.L2030:
	.align	2
.L2029:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+256
	.word	.LC202
	.word	.LC204
	.word	.LC205
	.word	.LC203
	.word	.LC201
	UNWIND(.fnend)
	.size	VCTRL_RequestChanMem, .-VCTRL_RequestChanMem
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
	ldr	r3, .L2032
	ldr	ip, [r1, #8]
	ldr	r3, [r3, r0, asl #2]
	add	r2, r3, #225280
	str	ip, [r3, #264]
	ldr	r0, [r1]
	str	r0, [r3, #260]
	ldr	r0, [r1]
	str	r0, [r3, #208]
	ldr	r0, [r1, #4]
	str	r0, [r3, #204]
	ldr	r0, [r1, #8]
	str	r0, [r3, #212]
	ldr	r3, [r1, #12]
	str	r3, [r2, #3280]
	ldr	r3, [r1, #16]
	str	r3, [r2, #3292]
	ldmfd	sp, {fp, sp, pc}
.L2033:
	.align	2
.L2032:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_SetFsParamToChan, .-VCTRL_SetFsParamToChan
	.align	2
	.global	VCTRL_SetSingleFrameBufToChan
	.type	VCTRL_SetSingleFrameBufToChan, %function
VCTRL_SetSingleFrameBufToChan:
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
	ldr	r9, .L2051
	mov	r2, r0
	ldr	r3, [r1, #16]
	ldr	r7, .L2051+4
	mov	r10, r1
	ldr	r8, [r9, r2, asl #2]
	cmp	r3, #40
	ldr	lr, .L2051+8
	mov	r0, #8
	add	r4, r8, #229376
	add	r6, r8, #225280
	movlt	r5, r3
	movge	r5, #40
	str	r3, [r6, #3280]
	ldr	r3, [r4, #660]
	ldr	r1, [r7, #12]
	str	r5, [r4, #268]
	str	r3, [fp, #-52]
	ldr	r5, [lr, r2, asl #2]
	str	r2, [fp, #-48]
	blx	r1
	ldr	r2, [fp, #-48]
	ldr	r3, [r9, r2, asl #2]
	add	lr, r3, #229376
	ldr	r1, [lr, #696]
	cmp	r1, #0
	bne	.L2035
	add	r0, r3, #225280
	ldr	r2, [lr, #692]
	str	r1, [fp, #-48]
	ldr	r0, [r0, #3280]
	cmp	r2, r0
	bcs	.L2044
	mov	r0, r2, asl #4
	add	r0, r0, r2, lsl #2
	add	r3, r3, r0
	add	r3, r3, #228352
	ldrb	r0, [r3, #248]	@ zero_extendqisi2
	cmp	r0, #255
	bne	.L2044
	ldr	r4, [r10, #4]
	add	r2, r2, #1
	ldr	ip, [r7, #16]
	mov	r0, #8
	str	r4, [r3, #240]
	ldr	r4, [r10]
	str	r4, [r3, #236]
	ldr	r4, [r10, #8]
	str	r4, [r3, #244]
	ldr	r4, [r10, #12]
	strb	r4, [r3, #248]
	str	r2, [lr, #692]
	blx	ip
	ldr	r1, [fp, #-48]
	mov	r0, r1
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2035:
	ldrsb	r3, [r5, #1]
	cmp	r3, #0
	bne	.L2049
	ldr	r3, [r4, #268]
	strb	r3, [r5, #10]
.L2039:
	mov	r3, #0
	strb	r3, [r5, #11]
	ldr	r3, [r4, #648]
	strb	r3, [r5, #13]
	ldr	r2, [r9, r2, asl #2]
	add	r0, r2, #229376
	add	r3, r2, #225280
	ldr	r1, [r0, #692]
	ldr	r3, [r3, #3280]
	cmp	r1, r3
	bcs	.L2044
	mov	r3, r1, asl #4
	add	r3, r3, r1, lsl #2
	add	r3, r2, r3
	add	r3, r3, #228352
	ldrb	r2, [r3, #248]	@ zero_extendqisi2
	cmp	r2, #255
	bne	.L2044
	ldr	r2, [r10, #4]
	add	r1, r1, #1
	str	r2, [r3, #240]
	ldr	r2, [r10]
	str	r2, [r3, #236]
	ldr	r2, [r10, #8]
	str	r2, [r3, #244]
	ldr	r2, [r10, #12]
	strb	r2, [r3, #248]
	str	r1, [r0, #692]
	ldr	r2, [r4, #652]
	ldr	r3, [r4, #268]
	cmp	r2, r3
	bcs	.L2044
	add	r8, r8, r2, lsl #3
	add	r8, r8, #225280
	ldr	r3, [r8, #3956]
	cmp	r3, #0
	bne	.L2044
	ldr	r1, [r10, #4]
	add	ip, r2, #66
	ldr	lr, [fp, #-52]
	mov	r0, #1
	add	r3, r1, #1020
	add	ip, r6, ip, lsl #2
	add	r3, r3, #3
	rsb	r9, r1, lr
	bic	r3, r3, #1020
	str	r0, [r8, #3956]
	bic	r3, r3, #3
	str	r3, [r8, #3952]
	add	r3, r9, r3
	add	r2, r2, r0
	add	lr, r3, r1
	str	lr, [ip, #3944]
	ldr	ip, [r4, #656]
	ldr	lr, [r4, #648]
	str	r2, [r4, #652]
	cmp	ip, lr
	bcs	.L2043
	ldr	r2, [r4, #408]
	cmp	r2, #0
	beq	.L2050
.L2043:
	add	r3, r5, #20480
	ldrsb	r2, [r5, #10]
	ldr	r9, [r3, #1804]
	cmp	r9, r2
	bcs	.L2044
	mov	r2, r9, asl #6
	add	r2, r2, r9, lsl #2
	add	r2, r5, r2
	add	lr, r2, #17408
	add	lr, lr, #200
	ldr	r8, [lr, #4]
	cmp	r8, #0
	bne	.L2044
	add	r1, r2, #17408
	add	r10, r1, #168
	str	r10, [fp, #-48]
	str	r9, [r1, #144]
	add	r9, r9, #1
	str	r9, [fp, #-52]
	ldr	r9, [r4, #652]
	add	r9, r6, r9, lsl #3
	ldr	r9, [r9, #3944]
	str	r9, [r1, #152]
	mov	r9, #1
	ldr	r0, [r4, #652]
	add	r0, r0, #65
	add	r0, r6, r0, lsl #2
	ldr	r0, [r0, #3944]
	str	r0, [r1, #156]
	ldr	r0, [r6, #3944]
	str	r0, [r1, #160]
	ldr	r0, [fp, #-48]
	ldr	ip, [r5, #44]
	str	ip, [r0, #8]
	ldr	r0, [r5, #48]
	str	r0, [r1, #180]
	str	r0, [fp, #-48]
	ldr	r0, [r4, #256]
	str	r0, [r1, #184]
	ldr	r10, [r4, #260]
	str	r10, [r1, #188]
	ldr	r10, [r4, #264]
	strb	r8, [r1, #140]
	strb	r8, [r1, #141]
	str	r10, [r1, #192]
	strb	r8, [r1, #142]
	str	r9, [lr, #4]
	ldr	r2, [r3, #1484]
	ldr	r1, [fp, #-52]
	cmp	r2, #0
	str	r1, [r3, #1804]
	bne	.L2046
	strb	r9, [r5, #12]
	ldr	r0, [fp, #-48]
	str	r2, [r3, #1424]
	str	r2, [r3, #1432]
	ldr	r1, [r6, #3944]
	str	ip, [r3, #1456]
	str	r0, [r3, #1460]
	str	r1, [r3, #1440]
	ldr	r1, [r4, #256]
	str	r1, [r3, #1464]
	ldr	r1, [r4, #260]
	str	r1, [r3, #1468]
	ldr	r1, [r4, #264]
	strb	r2, [r3, #1420]
	strb	r2, [r3, #1421]
	str	r1, [r3, #1472]
	strb	r2, [r3, #1422]
	str	r9, [r3, #1484]
.L2046:
	ldr	r8, [r3, #1808]
	ldrsb	r2, [r5, #13]
	cmp	r8, r2
	bcs	.L2047
	ldr	r2, [r4, #656]
	mov	r1, r8, asl #4
	sub	r1, r1, r8, asl #2
	add	r0, r8, #1
	sub	r2, r2, #1
	add	r1, r5, r1
	movw	r9, #22048
	add	lr, r1, #22016
	mov	r8, r2, asl #4
	mov	r10, #0
	sub	r2, r8, r2, asl #2
	mov	r8, #1
	add	r2, r6, r2
	add	r2, r2, #4480
	add	r2, r2, #16
	ldr	r2, [r2, #4]
	str	r10, [r1, r9]
	movw	r9, #22040
	str	r2, [r1, r9]
	str	r8, [lr, #28]
	str	r0, [r3, #1808]
.L2047:
	ldr	r2, [r4, #644]
	add	r5, r5, #16384
	str	r2, [r3, #1556]
	ldr	r3, [r5, #1160]
	cmp	r3, #0
	bne	.L2048
	ldr	r1, [r5, #1176]
	mov	r2, #1
	str	r3, [r5, #1100]
	ldr	r0, [fp, #-48]
	str	r1, [r5, #1108]
	ldr	r1, [r6, #3944]
	str	ip, [r5, #1132]
	str	r0, [r5, #1136]
	str	r1, [r5, #1116]
	ldr	r1, [r4, #256]
	str	r1, [r5, #1140]
	ldr	r1, [r4, #260]
	str	r1, [r5, #1144]
	ldr	r1, [r4, #264]
	strb	r3, [r5, #1096]
	strb	r3, [r5, #1097]
	str	r1, [r5, #1148]
	strb	r3, [r5, #1098]
	str	r2, [r5, #1160]
.L2048:
	ldr	r3, [r7, #16]
	mov	r0, #8
	blx	r3
	mov	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2049:
	ldrsb	r3, [r5, #40]
	ldr	r1, [r4, #268]
	add	r3, r3, #4
	cmp	r3, r1
	movge	r3, r1
	strb	r3, [r5, #10]
	b	.L2039
.L2044:
	ldr	r3, [r7, #16]
	mov	r0, #8
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2050:
	mov	r2, ip, asl #4
	add	r1, r1, #32
	sub	r2, r2, ip, asl #2
	add	r3, r1, r3
	add	r2, r6, r2
	add	ip, ip, r0
	add	r2, r2, #4480
	str	r3, [r2, #20]
	str	r0, [r2, #24]
	str	ip, [r4, #656]
	b	.L2043
.L2052:
	.align	2
.L2051:
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	s_pFspInst
	UNWIND(.fnend)
	.size	VCTRL_SetSingleFrameBufToChan, .-VCTRL_SetSingleFrameBufToChan
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
	mov	r4, r3
	ldr	r3, [fp, #28]
	cmp	r0, #0
	str	r0, [fp, #-64]
	mov	r7, r1
	ldr	r0, [fp, #4]
	mov	r6, r2
	mov	r5, r3
	str	r3, [fp, #-88]
	ldr	r3, [fp, #32]
	mov	lr, r0
	rsb	r1, r5, lr
	rsb	r3, r3, r1
	mov	r1, r4, lsr #1
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
	rsb	ip, ip, r4
	rsb	r2, r0, ip
	str	r3, [fp, #-76]
	str	r2, [fp, #-80]
	ldr	r8, [fp, #8]
	str	r1, [fp, #-68]
	beq	.L2053
	ldr	r5, .L2153
	mov	r0, #32
	ldr	r1, .L2153+4
	ldr	ip, [r5, #68]
	blx	ip
	cmp	r8, #0
	cmpne	r8, #3
	bne	.L2056
	ldr	r3, [fp, #-52]
	ldr	r10, .L2153+8
	cmp	r3, #0
	strne	r6, [fp, #-92]
	movne	r3, #0
	strne	r3, [fp, #-48]
	movne	r8, r3
	beq	.L2058
.L2060:
	cmp	r4, #0
	beq	.L2061
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r6, r3, #15
	mov	r2, r3, lsr #4
	ldr	r3, [fp, #-60]
	mul	r2, r3, r2
	add	r6, r2, r6, lsl #8
.L2059:
	mov	r1, r9, lsr #8
	ldr	r0, [r10, #1424]
	add	r2, r8, r9
	ldr	r3, [r5, #52]
	add	r1, r6, r1, lsl #12
	add	r0, r0, r2
	add	r1, r7, r1
	add	r9, r9, #256
	mov	r2, #256
	blx	r3
	cmp	r4, r9
	bhi	.L2059
.L2061:
	ldr	r3, [fp, #-48]
	add	r8, r8, r4
	ldr	r2, [fp, #-52]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2060
	ldr	r6, [fp, #-92]
.L2058:
	ldr	r3, [fp, #-80]
	ldr	r2, [fp, #-76]
	ldr	r1, [fp, #-52]
	cmp	r3, r4
	cmpeq	r2, r1
	moveq	r3, #1
	movne	r3, #0
	str	r3, [fp, #-92]
	beq	.L2148
	ldr	r3, [fp, #-88]
	ldr	r2, [fp, #-84]
	ldr	r1, [r10, #1424]
	mla	r7, r3, r4, r2
	ldr	r3, [fp, #-76]
	cmp	r3, #0
	add	r7, r1, r7
	beq	.L2066
	ldr	r8, [fp, #-92]
	mov	r9, r3
	str	r6, [fp, #-48]
	str	r10, [fp, #-52]
	ldr	r6, [fp, #-80]
	mov	r10, r8
	ldr	r8, [fp, #-64]
.L2067:
	mov	r0, r7
	add	r10, r10, #1
	ldr	ip, [r5, #44]
	mov	r2, r8
	mov	r1, r6
	add	r7, r7, r4
	blx	ip
	cmp	r10, r9
	bne	.L2067
	ldr	r6, [fp, #-48]
	ldr	r10, [fp, #-52]
.L2066:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	beq	.L2063
	ldr	r3, [fp, #-60]
	mov	r8, #0
	str	r8, [fp, #-48]
	mov	r3, r3, lsr #1
	str	r3, [fp, #-52]
.L2064:
	cmp	r4, #0
	beq	.L2071
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r7, r3, #7
	mov	r2, r3, lsr #3
	ldr	r3, [fp, #-52]
	mul	r2, r3, r2
	add	r7, r2, r7, lsl #8
.L2069:
	mov	r1, r9, lsr #8
	ldr	r0, [r10, #1424]
	add	r2, r8, r9
	ldr	r3, [r5, #52]
	add	r1, r7, r1, lsl #11
	add	r0, r0, r2
	add	r1, r6, r1
	add	r9, r9, #256
	mov	r2, #256
	blx	r3
	cmp	r4, r9
	bhi	.L2069
.L2071:
	ldr	r3, [fp, #-48]
	add	r8, r8, r4
	ldr	r2, [fp, #-56]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2064
	ldr	r3, [fp, #-68]
	ldr	r6, [r10, #1424]
	cmp	r3, #1
	beq	.L2149
.L2073:
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	beq	.L2150
.L2074:
	ldr	r3, [fp, #-92]
	cmp	r3, #0
	bne	.L2151
	ldr	r3, [fp, #-88]
	ldr	r8, [r10, #1420]
	mov	r2, r3, lsr #1
	ldr	r3, [fp, #-84]
	mov	r6, r3, lsr #1
	ldr	r3, [fp, #-76]
	movs	r4, r3, lsr #1
	ldr	r3, [fp, #-72]
	mla	r2, r2, r3, r6
	ldr	r6, [r10, #1416]
	add	r8, r8, r2
	add	r6, r6, r2
	beq	.L2053
	ldr	r9, [fp, #-92]
	mov	r10, r3
	ldr	r2, [fp, #-80]
	str	r6, [fp, #-48]
	mov	r6, r9
	ldr	r9, [fp, #-64]
	mov	r7, r2, lsr #1
.L2081:
	mov	r0, r8
	add	r6, r6, #1
	ldr	ip, [r5, #44]
	mov	r2, r9
	mov	r1, r7
	add	r8, r8, r10
	blx	ip
	cmp	r6, r4
	bne	.L2081
	ldr	r6, [fp, #-48]
	mov	r8, #0
	ldr	r9, [fp, #-72]
	ldr	r10, [fp, #-64]
.L2082:
	mov	r0, r6
	add	r8, r8, #1
	ldr	r3, [r5, #44]
	mov	r2, r10
	mov	r1, r7
	add	r6, r6, r9
	blx	r3
	cmp	r8, r4
	bne	.L2082
.L2053:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2056:
	subs	r3, r8, #1
	ldr	r2, [fp, #-52]
	ldr	r10, .L2153+8
	movne	r3, #1
	cmp	r2, r3
	str	r3, [fp, #-76]
	ldrhi	r3, [fp, #-76]
	strhi	r6, [fp, #-80]
	strhi	r3, [fp, #-48]
	bls	.L2084
.L2086:
	cmp	r4, #0
	beq	.L2087
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r6, r3, #15
	mov	r2, r3, lsr #4
	mov	r8, r3, lsr #1
	ldr	r3, [fp, #-60]
	mul	r8, r4, r8
	mul	r2, r3, r2
	add	r6, r2, r6, lsl #8
.L2085:
	mov	r1, r9, lsr #8
	ldr	r0, [r10, #1424]
	add	r2, r9, r8
	ldr	r3, [r5, #52]
	add	r1, r6, r1, lsl #12
	add	r0, r0, r2
	add	r1, r7, r1
	add	r9, r9, #256
	mov	r2, #256
	blx	r3
	cmp	r4, r9
	bhi	.L2085
.L2087:
	ldr	r3, [fp, #-48]
	ldr	r2, [fp, #-52]
	add	r3, r3, #2
	str	r3, [fp, #-48]
	cmp	r2, r3
	bhi	.L2086
	ldr	r6, [fp, #-80]
.L2084:
	ldr	r3, [fp, #-52]
	ldr	ip, [r5, #44]
	ldr	r2, [fp, #-64]
	mul	r1, r3, r4
	ldr	r0, [r10, #1424]
	mov	r1, r1, lsr #1
	blx	ip
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	beq	.L2088
	ldr	r3, [fp, #-60]
	mov	r8, #0
	str	r8, [fp, #-48]
	mov	r3, r3, lsr #1
	str	r3, [fp, #-52]
.L2089:
	cmp	r4, #0
	beq	.L2092
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r7, r3, #7
	mov	r2, r3, lsr #3
	ldr	r3, [fp, #-52]
	mul	r2, r3, r2
	add	r7, r2, r7, lsl #8
.L2090:
	mov	r1, r9, lsr #8
	ldr	r0, [r10, #1424]
	add	r2, r8, r9
	ldr	r3, [r5, #52]
	add	r1, r7, r1, lsl #11
	add	r0, r0, r2
	add	r1, r6, r1
	add	r9, r9, #256
	mov	r2, #256
	blx	r3
	cmp	r4, r9
	bhi	.L2090
.L2092:
	ldr	r3, [fp, #-48]
	add	r8, r8, r4
	ldr	r2, [fp, #-56]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2089
	ldr	r3, [fp, #-68]
	ldr	r2, [r10, #1424]
	cmp	r3, #1
	beq	.L2152
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	bne	.L2095
	ldr	r2, [fp, #-56]
	ldr	r3, [fp, #-76]
	cmp	r2, r3
	bls	.L2100
.L2101:
	add	r3, r3, #2
	cmp	r2, r3
	bhi	.L2101
.L2100:
	ldr	ip, [r5, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r10, #1416]
	blx	ip
	ldr	ip, [r5, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r10, #1420]
	blx	ip
.L2095:
	ldr	r2, [fp, #-72]
	ldr	r3, [fp, #-56]
	ldr	r6, [fp, #-64]
	ldr	ip, [r5, #44]
	mul	r4, r3, r2
	ldr	r0, [r10, #1420]
	mov	r2, r6
	mov	r4, r4, lsr #1
.L2147:
	mov	r1, r4
	blx	ip
	mov	r1, r4
	ldr	ip, [r5, #44]
	mov	r2, r6
	ldr	r0, [r10, #1416]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	ip
.L2148:
	mul	r1, r1, r4
	ldr	ip, [r5, #44]
	ldr	r2, [fp, #-64]
	ldr	r0, [r10, #1424]
	blx	ip
	b	.L2066
.L2152:
	ldr	r3, [fp, #-76]
	mov	r6, #0
	ldr	lr, [fp, #-72]
	ldr	r8, [fp, #-56]
	mla	r2, r3, r4, r2
	mov	r4, r4, asl #1
	add	r7, r2, #1
.L2096:
	cmp	lr, #0
	beq	.L2099
	mov	r3, r6, lsr #1
	mov	r2, r7
	mul	r3, lr, r3
	add	ip, r3, lr
.L2097:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	ldr	r1, [r10, #1416]
	strb	r0, [r1, r3]
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	ldr	r1, [r10, #1420]
	strb	r0, [r1, r3]
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L2097
.L2099:
	add	r6, r6, #2
	add	r7, r7, r4
	cmp	r8, r6
	bhi	.L2096
	b	.L2095
.L2149:
	ldr	lr, [fp, #-72]
	mov	r2, #0
	ldr	r8, [fp, #-56]
	add	r6, r6, #1
	mov	r7, r2
	mov	r3, r2
.L2075:
	cmp	lr, #0
	moveq	ip, r3
	beq	.L2078
	add	ip, r3, lr
	mov	r2, r6
.L2076:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	ldr	r1, [r10, #1416]
	strb	r0, [r1, r3]
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	ldr	r1, [r10, #1420]
	strb	r0, [r1, r3]
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L2076
.L2078:
	add	r7, r7, #1
	mov	r3, ip
	cmp	r7, r8
	add	r6, r6, r4
	bne	.L2075
	b	.L2074
.L2150:
	ldr	ip, [r5, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r10, #1416]
	blx	ip
	ldr	ip, [r5, #48]
	mov	r2, #1048576
	mov	r1, #128
	ldr	r0, [r10, #1420]
	blx	ip
	b	.L2074
.L2151:
	ldr	r2, [fp, #-72]
	ldr	r3, [fp, #-56]
	ldr	r6, [fp, #-64]
	ldr	ip, [r5, #44]
	mul	r4, r3, r2
	ldr	r0, [r10, #1420]
	mov	r2, r6
	b	.L2147
.L2088:
	ldr	r3, [fp, #-68]
	cmp	r3, #1
	beq	.L2095
	cmp	r3, #0
	beq	.L2100
	b	.L2095
.L2063:
	ldr	r3, [fp, #-68]
	cmp	r3, #1
	bne	.L2073
	b	.L2074
.L2154:
	.align	2
.L2153:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC206
	.word	.LANCHOR3
	UNWIND(.fnend)
	.size	DBG_WriteBigTileYuv, .-DBG_WriteBigTileYuv
	.align	2
	.global	VCTRL_GetChanImage
	.type	VCTRL_GetChanImage, %function
VCTRL_GetChanImage:
	UNWIND(.fnstart)
	@ args = 0, pretend = 0, frame = 512
	@ frame_needed = 1, uses_anonymous_args = 0
	UNWIND(.movsp ip)
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, r8, r9, r10, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	UNWIND(.pad #548)
	sub	sp, sp, #548
	ldr	r6, .L2207
	mov	r5, r0
	mov	r2, r0
	mov	r4, r1
	mov	r0, #2
	ldr	r1, .L2207+4
	ldr	r3, [r6, #68]
	blx	r3
	cmp	r5, #15
	bhi	.L2195
	ldr	r7, .L2207+8
	ldr	r0, [r7, r5, asl #2]
	cmp	r0, #0
	beq	.L2196
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L2197
	ldr	r3, [r6, #12]
	mov	r0, #4
	blx	r3
	ldr	r0, [r7, r5, asl #2]
	cmp	r0, #0
	beq	.L2198
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L2199
	add	r0, r0, #229376
	mov	r1, r4
	add	r0, r0, #752
	bl	SYNTAX_ReceiveImage
	sub	r9, r0, #1
	mov	r8, r0
	clz	r9, r9
	mov	r9, r9, lsr #5
	cmp	r4, #0
	moveq	r3, #0
	andne	r3, r9, #1
	cmp	r3, #0
	beq	.L2162
	ldr	r3, .L2207+12
	add	r2, r3, r5, lsl #2
	ldr	r0, [r2, #1268]
	cmp	r0, #0
	beq	.L2162
	ldr	r2, [r4, #160]
	ldr	ip, [r3, #1428]
	tst	r2, #28
	ldr	r1, [r4, #32]
	ldr	r2, [r4, #36]
	moveq	r3, #1
	movne	r3, #0
	cmp	ip, #0
	str	r3, [fp, #-552]
	ldr	r3, [r4, #164]
	beq	.L2163
	ldr	ip, [r4, #348]
	ldr	r10, [r4, #352]
	ldr	lr, [r4, #344]
	str	ip, [fp, #-556]
	ldr	ip, [fp, #-552]
	str	ip, [sp, #12]
	ldr	ip, [r4, #340]
	str	r10, [sp, #28]
	ldr	r10, [r4, #192]
	str	ip, [fp, #-552]
	ldr	ip, [fp, #-556]
	str	ip, [sp, #24]
	ldr	ip, [r4, #168]
	str	lr, [sp, #20]
	ldr	lr, [fp, #-552]
	str	ip, [sp]
	mov	ip, #0
	str	r10, [sp, #8]
	str	lr, [sp, #16]
	str	ip, [sp, #4]
	bl	DBG_WriteBigTileYuv
.L2162:
	cmp	r9, #0
	bne	.L2200
.L2164:
	subs	r9, r8, #1
	mvnne	r9, #0
.L2166:
	mov	r0, #12
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L2177
.L2175:
	cmp	r9, #0
	beq	.L2178
.L2176:
	ldr	r3, [r7, r5, asl #2]
	mov	r0, #4
	ldr	r2, [r6, #16]
	add	r3, r3, #245760
	ldrb	r3, [r3, #580]	@ zero_extendqisi2
	strb	r3, [r4, #480]
	blx	r2
.L2192:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2200:
	ldr	r10, [r4, #256]
	mvn	r9, #0
	str	r9, [r4, #252]
	cmp	r10, #1
	bne	.L2164
	ldr	r3, [r4, #244]
	cmp	r3, #0
	beq	.L2201
	ldr	lr, [r4, #92]
	mov	r2, #60
	ldr	ip, [r4, #88]
	add	r1, r4, #420
	ldr	r3, [r6, #52]
	add	r0, r4, #280
	str	lr, [r4, #264]
	str	ip, [r4, #260]
	blx	r3
	mov	r3, #3
	str	r9, [r4, #252]
	str	r10, [r4, #244]
	str	r3, [r4, #364]
.L2174:
	mov	r0, #12
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L2202
.L2178:
	mov	r2, #0
	mov	r1, #12
	mov	r0, r5
	mov	r9, r2
	bl	DBG_CountTimeInfo
	ldr	r2, [r4, #160]
	ldr	r3, [r7, r5, asl #2]
	str	r2, [r3, #308]
	ldr	r2, [r4, #364]
	str	r2, [r3, #312]
	b	.L2176
.L2202:
	mov	r9, #0
	mov	r8, #1
.L2177:
	ldr	r3, [r6]
	blx	r3
	ldr	r3, .L2207+12
	ldr	r1, .L2207+16
	add	r3, r3, r5, lsl #2
	ldr	r2, [r3, #1436]
	ldr	r1, [r1]
	rsb	r2, r2, r0
	mov	ip, r0
	cmp	r2, r1
	bls	.L2175
	mov	r2, r5, asl #24
	movw	r1, #4291
	add	r2, r2, #786432
	ldr	r0, .L2207+20
	cmp	r8, #1
	addeq	r2, r2, #1
	str	ip, [r3, #1436]
	bl	DBG_AddTrace
	b	.L2175
.L2163:
	ldr	lr, [r4, #168]
	ldr	r10, [r4, #192]
	str	ip, [sp, #28]
	str	lr, [fp, #-556]
	ldr	lr, [fp, #-552]
	str	ip, [sp, #24]
	str	ip, [sp, #20]
	str	lr, [sp, #12]
	ldr	lr, [fp, #-556]
	str	ip, [sp, #16]
	str	ip, [sp, #4]
	str	r10, [sp, #8]
	str	lr, [sp]
	bl	DBG_WriteBigTileYuv
	cmp	r9, #0
	beq	.L2164
	b	.L2200
.L2197:
	ldr	ip, [r6, #68]
	movw	r2, #4083
	ldr	r3, .L2207+24
	mov	r0, #0
	ldr	r1, .L2207+28
	mvn	r9, #0
	blx	ip
	b	.L2192
.L2199:
	ldr	r3, [r6, #68]
	movw	r2, #4101
	ldr	r1, .L2207+32
	mov	r0, #0
.L2193:
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, #4
	mvn	r9, #0
	blx	r3
	b	.L2192
.L2201:
	ldr	r0, [r7, r5, asl #2]
	sub	r1, fp, #548
	str	r3, [fp, #-552]
	add	r0, r0, #229376
	ldr	r10, .L2207+8
	add	r0, r0, #752
	bl	SYNTAX_ReceiveImage
	ldr	r3, [fp, #-552]
	cmp	r0, #1
	mov	r8, r0
	bne	.L2203
	ldr	r2, [r4, #248]
	ldr	r1, [fp, #-300]
	cmp	r2, r1
	blt	.L2204
	ldr	r0, [r10, r5, asl #2]
	bgt	.L2205
	cmp	r2, #0
	bne	.L2172
	add	r0, r0, #229376
	add	r0, r0, #752
	bl	SYNTAX_ReturnImage
	ldr	r0, [r10, r5, asl #2]
	mov	r1, r4
	add	r0, r0, #229376
	add	r0, r0, #752
	bl	SYNTAX_ReleaseImage
	mov	r0, r8
	ldr	ip, [r6, #68]
	mvn	r8, #1
	ldr	r3, [r4, #248]
	ldr	r2, .L2207+36
	ldr	r1, .L2207+40
	blx	ip
	b	.L2166
.L2195:
	ldr	ip, [r6, #68]
	movw	r2, #4079
	ldr	r3, .L2207+44
	mov	r0, #0
	ldr	r1, .L2207+28
	mvn	r9, #0
	blx	ip
	b	.L2192
.L2203:
	ldr	r0, [r10, r5, asl #2]
	mov	r9, r8
	add	r0, r0, #229376
	add	r0, r0, #752
	bl	SYNTAX_ReturnImage
	ldr	r3, [r6, #16]
	mov	r0, #4
	blx	r3
	b	.L2192
.L2204:
	ldr	r9, [fp, #-460]
	mov	r2, #60
	ldr	ip, [fp, #-448]
	sub	r1, fp, #128
	ldr	r10, [fp, #-456]
	add	r0, r4, #280
	ldr	lr, [fp, #-452]
	str	r9, [r4, #260]
	str	ip, [r4, #272]
	str	r10, [r4, #264]
	str	lr, [r4, #268]
	ldr	ip, [r6, #52]
	str	r3, [fp, #-552]
	blx	ip
	ldr	r9, [fp, #-304]
	mov	r1, #3
	ldr	r2, [fp, #-352]
	ldr	r3, [fp, #-552]
	cmp	r9, #0
	str	r1, [r4, #364]
	str	r2, [r4, #252]
	strne	r8, [r4, #244]
	movne	r9, r3
	b	.L2166
.L2172:
	add	r0, r0, #229376
	mov	r1, r4
	add	r0, r0, #752
	bl	SYNTAX_ReleaseImage
	ldr	r3, [fp, #-304]
	cmp	r3, #0
	beq	.L2206
	ldr	ip, [r6, #68]
	mov	r0, r8
	ldr	r3, [r4, #248]
	ldr	r2, .L2207+36
	ldr	r1, .L2207+48
	blx	ip
	sub	r1, fp, #548
	ldr	r3, [r6, #52]
	mov	r2, #504
	mov	r0, r4
	blx	r3
	ldr	r0, [fp, #-456]
	ldr	ip, [fp, #-460]
	mov	r2, #60
	ldr	r3, [r6, #52]
	sub	r1, fp, #128
	str	r0, [r4, #264]
	add	r0, r4, #280
	str	ip, [r4, #260]
	blx	r3
	mov	r3, #3
	str	r9, [r4, #252]
	str	r8, [r4, #244]
	str	r3, [r4, #364]
	b	.L2174
.L2205:
	add	r0, r0, #229376
	add	r0, r0, #752
	bl	SYNTAX_ReturnImage
	ldr	r0, [r10, r5, asl #2]
	mov	r1, r4
	add	r0, r0, #229376
	add	r0, r0, #752
	bl	SYNTAX_ReleaseImage
	ldr	r1, [fp, #-300]
	ldr	r3, [r4, #248]
	mov	r0, r8
	ldr	ip, [r6, #68]
	mvn	r8, #1
	str	r1, [sp]
	ldr	r2, .L2207+36
	ldr	r1, .L2207+52
	blx	ip
	b	.L2166
.L2196:
	ldr	ip, [r6, #68]
	mov	r2, #4080
	ldr	r3, .L2207+56
	mvn	r9, #0
	ldr	r1, .L2207+28
	blx	ip
	b	.L2192
.L2198:
	ldr	r3, [r6, #68]
	movw	r2, #4091
	ldr	r1, .L2207+60
	b	.L2193
.L2206:
	ldr	r0, [r10, r5, asl #2]
	sub	r1, fp, #548
	add	r0, r0, #229376
	add	r0, r0, #752
	bl	SYNTAX_ReleaseImage
	mov	r0, r8
	ldr	ip, [r6, #68]
	mvn	r8, #1
	ldr	r3, [r4, #248]
	ldr	r2, .L2207+36
	ldr	r1, .L2207+48
	blx	ip
	b	.L2166
.L2208:
	.align	2
.L2207:
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LC207
	.word	.LANCHOR0
	.word	.LANCHOR3
	.word	g_TraceImgPeriod
	.word	.LANCHOR1+300
	.word	.LC45
	.word	.LC1
	.word	.LC49
	.word	.LANCHOR1+280
	.word	.LC209
	.word	.LC6
	.word	.LC210
	.word	.LC208
	.word	.LC2
	.word	.LC48
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
	ldr	r3, .L2217
	ldr	r0, [r3, r0, asl #2]
	add	r0, r0, #229376
	ldr	r3, [r0, #752]
	cmp	r3, #2
	beq	.L2211
	cmp	r3, #17
	beq	.L2212
	cmp	r3, #0
	beq	.L2216
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2216:
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageWidth
.L2212:
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageWidth
.L2211:
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageWidth
.L2218:
	.align	2
.L2217:
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
	ldr	r3, .L2227
	ldr	r0, [r3, r0, asl #2]
	add	r0, r0, #229376
	ldr	r3, [r0, #752]
	cmp	r3, #2
	beq	.L2221
	cmp	r3, #17
	beq	.L2222
	cmp	r3, #0
	beq	.L2226
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2226:
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageHeight
.L2222:
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageHeight
.L2221:
	add	r0, r0, #760
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageHeight
.L2228:
	.align	2
.L2227:
	.word	.LANCHOR0
	UNWIND(.fnend)
	.size	VCTRL_GetImageHeight, .-VCTRL_GetImageHeight
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
	.global	g_SdecFunc
	.global	g_DSPState
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__func__.31777, %object
	.size	__func__.31777, 18
__func__.31777:
	.ascii	"VCTRL_EventReport\000"
	.space	2
	.type	__func__.31789, %object
	.size	__func__.31789, 18
__func__.31789:
	.ascii	"VCTRL_SetCallBack\000"
	.space	2
	.type	__FUNCTION__.31884, %object
	.size	__FUNCTION__.31884, 29
__FUNCTION__.31884:
	.ascii	"VCTRL_PartitionMemForStdUser\000"
	.space	3
	.type	__FUNCTION__.32141, %object
	.size	__FUNCTION__.32141, 23
__FUNCTION__.32141:
	.ascii	"VCTRL_ReleaseChanImage\000"
	.space	1
	.type	__func__.32142, %object
	.size	__func__.32142, 23
__func__.32142:
	.ascii	"VCTRL_ReleaseChanImage\000"
	.space	1
	.type	__FUNCTION__.32215, %object
	.size	__FUNCTION__.32215, 19
__FUNCTION__.32215:
	.ascii	"VCTRL_FlushDecoder\000"
	.space	1
	.type	__FUNCTION__.32558, %object
	.size	__FUNCTION__.32558, 18
__FUNCTION__.32558:
	.ascii	"GetExtraRefFrmNum\000"
	.space	2
	.type	__FUNCTION__.32564, %object
	.size	__FUNCTION__.32564, 19
__FUNCTION__.32564:
	.ascii	"GetExtraDispFrmNum\000"
	.space	1
	.type	__FUNCTION__.32570, %object
	.size	__FUNCTION__.32570, 16
__FUNCTION__.32570:
	.ascii	"GetExtraCtrlCmd\000"
	.type	__FUNCTION__.32469, %object
	.size	__FUNCTION__.32469, 14
__FUNCTION__.32469:
	.ascii	"DecChanSyntax\000"
	.space	2
	.type	__func__.32528, %object
	.size	__func__.32528, 24
__func__.32528:
	.ascii	"VCTRL_ExtAccGetDecParam\000"
	.type	__func__.32047, %object
	.size	__func__.32047, 17
__func__.32047:
	.ascii	"VCTRL_ConfigChan\000"
	.space	3
	.type	__FUNCTION__.32809, %object
	.size	__FUNCTION__.32809, 21
__FUNCTION__.32809:
	.ascii	"VCTRL_RequestChanMem\000"
	.space	3
	.type	__func__.32122, %object
	.size	__func__.32122, 19
__func__.32122:
	.ascii	"VCTRL_GetChanImage\000"
	.space	1
	.type	__FUNCTION__.32130, %object
	.size	__FUNCTION__.32130, 19
__FUNCTION__.32130:
	.ascii	"VCTRL_GetChanImage\000"
	.data
	.align	2
.LANCHOR2 = . + 0
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
	ASCII(.ascii	"ChanID out of range\000" )
.LC7:
	ASCII(.ascii	"pfun_iMediaSoftDEC_Create\000" )
	.space	2
.LC8:
	ASCII(.ascii	"register soft decoder fail: '%s' can not be NULL\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC9:
	ASCII(.ascii	"pfun_iMediaSoftDEC_Destroy\000" )
	.space	1
.LC10:
	ASCII(.ascii	"pfun_iMediaSoftDEC_Init\000" )
.LC11:
	ASCII(.ascii	"pfun_iMediaSoftDEC_Decode\000" )
	.space	2
.LC12:
	ASCII(.ascii	"pfun_iMediaSoftDEC_RecycleImage\000" )
.LC13:
	ASCII(.ascii	"pfun_iMediaSoftDEC_GetRemainImg\000" )
.LC14:
	ASCII(.ascii	"pfun_iMediaSoftDEC_GetImageBuffer\000" )
	.space	2
.LC15:
	ASCII(.ascii	"Force vdm %d sleep.\012\000" )
	.space	3
.LC16:
	ASCII(.ascii	"VDMHAL_NULL_FUN_PRINT,L%d\012\000" )
	.space	1
.LC17:
	ASCII(.ascii	"Vfmw suspend totally take %d ms\012\000" )
	.space	3
.LC18:
	ASCII(.ascii	"Wait sleep time out %d ms (isScdSleep=%d, isVdmSlee" )
	ASCII(.ascii	"p=%d)!\012\000" )
	.space	1
.LC19:
	ASCII(.ascii	"Force scd %d sleep.\012\000" )
	.space	3
.LC20:
	ASCII(.ascii	"Vfmw resume totally take %d ms\012\000" )
.LC21:
	ASCII(.ascii	"can NOT config a not-opened Chan\000" )
	.space	3
.LC22:
	ASCII(.ascii	"event_report null!\000" )
	.space	1
.LC23:
	ASCII(.ascii	"%s Unkown Adapter Type: %d\012\000" )
.LC24:
	ASCII(.ascii	"pstStreamParam is NULL\000" )
	.space	1
.LC25:
	ASCII(.ascii	"L: %d, Stream Interface Error!!, ChanID: %d\012\000" )
	.space	3
.LC26:
	ASCII(.ascii	"pstStreamIntf is NULL\000" )
	.space	2
.LC27:
	ASCII(.ascii	"This Chan has not been created!!!\000" )
	.space	2
.LC28:
	ASCII(.ascii	"can NOT get a not-opened Chan's information of bits" )
	ASCII(.ascii	"tream\000" )
	.space	3
.LC29:
	ASCII(.ascii	"pArgs is null\000" )
	.space	2
.LC30:
	ASCII(.ascii	"image size abnormal(%dx%d)\012\000" )
.LC31:
	ASCII(.ascii	"no mem to arrange for soft dec\012\000" )
.LC32:
	ASCII(.ascii	"partition fs memory fail!\012\000" )
	.space	1
.LC33:
	ASCII(.ascii	"%s: L%d : vdm chan memory is too small to support u" )
	ASCII(.ascii	"ser decoder\012\000" )
.LC34:
	ASCII(.ascii	"FSP_ConfigInstance fail!\012\000" )
	.space	2
.LC35:
	ASCII(.ascii	"DFS, Wrong! Phyaddr:0x%x, Viraddr:0x%x, frm_num: 0x" )
	ASCII(.ascii	"%x\012\000" )
	.space	1
.LC36:
	ASCII(.ascii	"DFS, Not enough Frame node,rls all node, FrmNum:%d," )
	ASCII(.ascii	" AllocedFrameNum: %d\012\000" )
	.space	3
.LC37:
	ASCII(.ascii	"DFS, Rls Fail, chanid: %d\012\000" )
	.space	1
.LC38:
	ASCII(.ascii	"VFMW_FRM_BUF\000" )
	.space	3
.LC39:
	ASCII(.ascii	"DFS, Not enough resource, ChanID: %d, AllocedFrmNum" )
	ASCII(.ascii	": %d, FrmNum: %d, i: %d\012\000" )
.LC40:
	ASCII(.ascii	"DFS, Rls Frame Node Fail!\012\000" )
	.space	1
.LC41:
	ASCII(.ascii	"DelMemRecord 0x%x failed!\012\000" )
	.space	1
.LC42:
	ASCII(.ascii	"pstCfg is null\000" )
	.space	1
.LC43:
	ASCII(.ascii	"create soft-decoder fail!\012\000" )
	.space	1
.LC44:
	ASCII(.ascii	"Chip not support beyond %dx%d when Std != H264/VP8/" )
	ASCII(.ascii	"MVC/USER.\012\000" )
	.space	2
.LC45:
	ASCII(.ascii	"This channel is not opened\000" )
	.space	1
.LC46:
	ASCII(.ascii	"pImage is NULL\000" )
	.space	1
.LC47:
	ASCII(.ascii	"call VCTRL_ReleaseChanImage, ChanID, ImgID = %d,%d\012" )
	ASCII(.ascii	"\000" )
.LC48:
	ASCII(.ascii	"vdm_ctrl.c,L%d: Chan inactive\012\000" )
	.space	1
.LC49:
	ASCII(.ascii	"vdm_ctrl.c,L%d: This channel is not opened\012\000" )
.LC50:
	ASCII(.ascii	"%s call SYNTAX_ReleaseImage failed\012\000" )
.LC51:
	ASCII(.ascii	"vfmw_ctrl.c, line %d: ChanID out of range\012\000" )
	.space	1
.LC52:
	ASCII(.ascii	"vfmw_ctrl.c, line %d: Chan %d is not open or not ru" )
	ASCII(.ascii	"nning\012\000" )
	.space	2
.LC53:
	ASCII(.ascii	"vfmw_ctrl.c, line %d: ChanID out of range is %d\012" )
	ASCII(.ascii	"\000" )
	.space	3
.LC54:
	ASCII(.ascii	"Chan %d is not open or not running\012\000" )
.LC55:
	ASCII(.ascii	"line: %d Flush DPB failed!\012\000" )
.LC56:
	ASCII(.ascii	"VCTRL_FlushDecoder\012\000" )
.LC57:
	ASCII(.ascii	"fuction return value is null,%s %d MVC_OK != MVC_Fl" )
	ASCII(.ascii	"ushDPB!!\012\000" )
	.space	3
.LC58:
	ASCII(.ascii	"ChanId out of range\000" )
.LC59:
	ASCII(.ascii	"This channel is not running\000" )
.LC60:
	ASCII(.ascii	"chan not create\000" )
.LC61:
	ASCII(.ascii	"can NOT start a not-opened Chan\000" )
.LC62:
	ASCII(.ascii	"NULL == pArgs\000" )
	.space	2
.LC63:
	ASCII(.ascii	"-1 == VCTRL_GetChanIDByCtx() Err! \012\000" )
.LC64:
	ASCII(.ascii	"VCTRL_GetUsrdecFrame but VFMW_USER != s_pstVfmwChan" )
	ASCII(.ascii	"[ChanID]->eVidStd!\012\000" )
	.space	1
.LC65:
	ASCII(.ascii	"VCTRL_GetUsrdecFrame but s_pstVfmwChan[ChanID]->s32" )
	ASCII(.ascii	"UsrdecFrameNum < MIN_USERDEC_FRAME_NUM!\012\000" )
.LC66:
	ASCII(.ascii	"VCTRL_GetUsrdecFrame but FSP_IsNewFsAvalible(ChanID" )
	ASCII(.ascii	") != 1!\012\000" )
.LC67:
	ASCII(.ascii	"get frame store fail!\012\000" )
	.space	1
.LC68:
	ASCII(.ascii	"get image buffer ok: LogicFsID = %d\012\000" )
	.space	3
.LC69:
	ASCII(.ascii	"line: %d, pLf is NULL!\012\000" )
.LC70:
	ASCII(.ascii	"decode %p, disp %p, tf %p\012\000" )
	.space	1
.LC71:
	ASCII(.ascii	"VCTRL_GetUsrdecFrame get LogicFs fail!\012\000" )
.LC72:
	ASCII(.ascii	"ERROR: s32MaxRefFrameNum(%d), extra_ref(%d), s32Dis" )
	ASCII(.ascii	"playFrameNum(%d), extra_disp(%d)\012\000" )
	.space	3
.LC73:
	ASCII(.ascii	"ERROR: (MaxRefFrameNum(%d) + extra_ref(%d)) > 16\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC74:
	ASCII(.ascii	"VDEC_CID_GET_CHAN_DETAIL_MEMSIZE_BY_OPTION is desig" )
	ASCII(.ascii	"ned for CAP_LEVEL_USER_DEFINE_WITH_OPTION, please c" )
	ASCII(.ascii	"hange channel's capability or use VDEC_CID_GET_CHAN" )
	ASCII(.ascii	"_MEMSIZE.\012\000" )
.LC75:
	ASCII(.ascii	"CAP_LEVEL_USER_DEFINE_WITH_OPTION option is illegal" )
	ASCII(.ascii	".\012\000" )
	.space	2
.LC76:
	ASCII(.ascii	"VCTRL_GetChanMemSizeByOption L%d: arrange return fa" )
	ASCII(.ascii	"ilure\012\000" )
	.space	2
.LC77:
	ASCII(.ascii	"ERROR: (nPmv(%d) + extra_ref(%d)) <= 0\012\000" )
.LC78:
	ASCII(.ascii	"ERROR: TempMinFrame(%d + %d + %d), TempMiddleFrame(" )
	ASCII(.ascii	"%d + %d + %d), TempFluentFrame(%d + %d + %d)\012\000" )
	.space	3
.LC79:
	ASCII(.ascii	"VCTRL_GetChanMemSize L%d: arrange return failure\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC80:
	ASCII(.ascii	"extra_ref=\000" )
	.space	1
.LC81:
	ASCII(.ascii	"=\000" )
	.space	2
.LC82:
	ASCII(.ascii	" \000" )
	.space	2
.LC83:
	ASCII(.ascii	"fuction return value is null,%s %d unknow error!!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC84:
	ASCII(.ascii	"ERROR: extra_ref = %d is out of [ -5000, 5000 ]!\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC85:
	ASCII(.ascii	"extra_disp=\000" )
.LC86:
	ASCII(.ascii	"ERROR: extra_disp = %d is out of [ -5000, 5000 ]!\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC87:
	ASCII(.ascii	"extra_cmd=\000" )
	.space	1
.LC88:
	ASCII(.ascii	"HI_VFMW_VideoDecode\000" )
.LC89:
	ASCII(.ascii	"(safe,warn,danger) :amount = %d,%d,%d,%d\012\000" )
	.space	2
.LC90:
	ASCII(.ascii	"strm_ctrl: I->IPB\012\000" )
	.space	1
.LC91:
	ASCII(.ascii	"strm_ctrl: IPB->I\012\000" )
	.space	1
.LC92:
	ASCII(.ascii	"(CurrMode,NextMode) = %d,%d\012\000" )
	.space	3
.LC93:
	ASCII(.ascii	"Syntax is stopped from This channel\000" )
.LC94:
	ASCII(.ascii	"[%d] vdm busy\012\000" )
	.space	1
.LC95:
	ASCII(.ascii	"[%d] no frame\012\000" )
	.space	1
.LC96:
	ASCII(.ascii	"[%d] no stream\012\000" )
.LC97:
	ASCII(.ascii	"seg_pts: %lld, seg_usertag = %lld\012\000" )
	.space	1
.LC98:
	ASCII(.ascii	"pic_pts: %lld\012\000" )
	.space	1
.LC99:
	ASCII(.ascii	"pic_usertag: %lld\012\000" )
	.space	1
.LC100:
	ASCII(.ascii	"pic_pts: %lld pic_usertag = %lld\012\000" )
	.space	2
.LC101:
	ASCII(.ascii	"%s:ChanId %d this hevc file is 10bit!!!\012\000" )
	.space	3
.LC102:
	ASCII(.ascii	"DFS, Wait Partition\012\000" )
	.space	3
.LC103:
	ASCII(.ascii	"DFS, Partition fail, u32DynamicFrameStoreAllocEn: %" )
	ASCII(.ascii	"d, eFspPartitionState: %d\012\000" )
	.space	2
.LC104:
	ASCII(.ascii	"ChanID out of range!\000" )
	.space	3
.LC105:
	ASCII(.ascii	"SetVdecExtra ERROR: extra_ref = %d is out of [ -500" )
	ASCII(.ascii	"0, 5000 ]!\012\000" )
	.space	1
.LC106:
	ASCII(.ascii	"SetVdecExtra ERROR: extra_disp = %d is out of [ -50" )
	ASCII(.ascii	"00, 5000 ]!\012\000" )
.LC107:
	ASCII(.ascii	"%s 1 The channel %d is not active\012\000" )
	.space	1
.LC108:
	ASCII(.ascii	"The channel %d is deced\012\000" )
	.space	3
.LC109:
	ASCII(.ascii	"The pDecParam of channel %d is NULL\012\000" )
	.space	3
.LC110:
	ASCII(.ascii	"%s 2 The channel %d is not active\012\000" )
	.space	1
.LC111:
	ASCII(.ascii	"Thread running...\012\000" )
	.space	1
.LC112:
	ASCII(.ascii	"Thread stopped!\012\000" )
	.space	3
.LC113:
	ASCII(.ascii	"Thread killed!\012\000" )
.LC114:
	ASCII(.ascii	"thread running...\012\000" )
	.space	1
.LC115:
	ASCII(.ascii	"thread stopped...\012\000" )
	.space	1
.LC116:
	ASCII(.ascii	"Report Last Frame, Value=%d, LastFrameState=%d\012\000" )
.LC117:
	ASCII(.ascii	"Last frame output failed!\012\000" )
	.space	1
.LC118:
	ASCII(.ascii	"Last frame output successed!\012\000" )
	.space	2
.LC119:
	ASCII(.ascii	"Last frame already output, image id: %d!\012\000" )
	.space	2
.LC120:
	ASCII(.ascii	"line %d can not know YuvFormat = %d\012\000" )
	.space	3
.LC121:
	ASCII(.ascii	"userdec frame(phy=%#x) address error!\012\000" )
	.space	1
.LC122:
	ASCII(.ascii	"can not map vir addr for usrdat frame(cbphy=%#x)\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC123:
	ASCII(.ascii	"line: %d, pLogic is Null!\012\000" )
	.space	1
.LC124:
	ASCII(.ascii	"eYuvFormat %d is not support!!\012\000" )
.LC125:
	ASCII(.ascii	"pUsrdecFrame == NULL\000" )
	.space	3
.LC126:
	ASCII(.ascii	"VCTRL_PutUsrdecFrame: VidStd is %d\012\000" )
.LC127:
	ASCII(.ascii	"VCTRL_PutUsrdecFrame return err\012\000" )
	.space	3
.LC128:
	ASCII(.ascii	"L:%d ChanID out of range\012\000" )
	.space	2
.LC129:
	ASCII(.ascii	"L:%d This Chan has not been created!!!\012\000" )
.LC130:
	ASCII(.ascii	"L:%d WARNING: vidio std not setted!\012\000" )
	.space	3
.LC131:
	ASCII(.ascii	"WARNING: vidio std not setted!\000" )
	.space	1
.LC132:
	ASCII(.ascii	"This Chan has not been created or has been destroye" )
	ASCII(.ascii	"d !\000" )
	.space	1
.LC133:
	ASCII(.ascii	"VCTRL_StopChanWithCheck fail!\012\000" )
	.space	1
.LC134:
	ASCII(.ascii	"---------------------------------------------------" )
	ASCII(.ascii	"----------------------\012\000" )
	.space	1
.LC135:
	ASCII(.ascii	"VDEC_CID_CREATE_CHAN_WITH_OPTION is designed for CA" )
	ASCII(.ascii	"P_LEVEL_USER_DEFINE_WITH_OPTION, please change chan" )
	ASCII(.ascii	"nel's capability.\012\000" )
	.space	3
.LC136:
	ASCII(.ascii	"line %d VCTRL_GetChanMemSizeWithOption failed!\012\000" )
.LC137:
	ASCII(.ascii	"L: %d : part we can NOT alloc buffer for channel co" )
	ASCII(.ascii	"ntext\012\000" )
	.space	2
.LC138:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel context\012" )
	ASCII(.ascii	"\000" )
	.space	2
.LC139:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel vdh\012\000" )
	.space	2
.LC140:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel vdh, %#x <" )
	ASCII(.ascii	" %#x\012\000" )
	.space	3
.LC141:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for channel scd\012\000" )
	.space	2
.LC142:
	ASCII(.ascii	"L: %d : can NOT alloc buffer for SCD\012\000" )
	.space	2
.LC143:
	ASCII(.ascii	"can NOT enable FSP instance\012\000" )
	.space	3
.LC144:
	ASCII(.ascii	"vfmw have max(%d) channels opened, can NOT crerate " )
	ASCII(.ascii	"more channels\012\000" )
	.space	2
.LC145:
	ASCII(.ascii	"VCTRL_CreateChan eCapLevel = %d error! Try to use V" )
	ASCII(.ascii	"CTRL_CreateChanWithOption(CAP_LEVEL_USER_DEFINE_WIT" )
	ASCII(.ascii	"H_OPTION, ...)\012\000" )
	.space	2
.LC146:
	ASCII(.ascii	"VCTRL_GetChanMemSize ERROR\012\000" )
.LC147:
	ASCII(.ascii	"channel memory have been allocated outside, but the" )
	ASCII(.ascii	" mem-info is not correct!\012\000" )
	.space	2
.LC148:
	ASCII(.ascii	"mem-info(phy, vir, size) = (%#x, %p, %d)\012\000" )
	.space	2
.LC149:
	ASCII(.ascii	"this channel totally need %d bytes memory\012\000" )
	.space	1
.LC150:
	ASCII(.ascii	"VCTRL_CheckChanWH return err.\012\000" )
	.space	1
.LC151:
	ASCII(.ascii	"VFMW didn't support std %d.\012\000" )
	.space	3
.LC152:
	ASCII(.ascii	"VCTRL_ConfigChan with same config paramater.\012\000" )
	.space	2
.LC153:
	ASCII(.ascii	"%s %d: s_pstVfmwChan has been destroyed!!\012\000" )
	.space	1
.LC154:
	ASCII(.ascii	"====== Fatal Error0: can NOT re-map vir address for" )
	ASCII(.ascii	" channel %d ======\012\000" )
	.space	1
.LC155:
	ASCII(.ascii	"fatal error: can not del mem record(phy=0x%x,vir=%p" )
	ASCII(.ascii	",len=%d)\012\000" )
	.space	3
.LC156:
	ASCII(.ascii	"can NOT VCTRL_PartitionMemForStdUser\012\000" )
	.space	2
.LC157:
	ASCII(.ascii	"chan %d: new ChanErrThr = %d\012\000" )
	.space	2
.LC158:
	ASCII(.ascii	"chan %d: new DecOrderOutput = %d\012\000" )
	.space	2
.LC159:
	ASCII(.ascii	"chan %d: new DecMode = %d\012\000" )
	.space	1
.LC160:
	ASCII(.ascii	"chan %d: GetChanCfg failed!\012\000" )
	.space	3
.LC161:
	ASCII(.ascii	"chan %d: new StrmOFThr = %d\012\000" )
	.space	3
.LC162:
	ASCII(.ascii	"USE_FF_APT_EN = %d\012\000" )
.LC163:
	ASCII(.ascii	"USE_FF_APT_EN can NOT be set to %d, must be 0 or 1\012" )
	ASCII(.ascii	"\000" )
.LC164:
	ASCII(.ascii	"ResetCreateNewSave = %d\012\000" )
	.space	3
.LC165:
	ASCII(.ascii	"ResetCreateNewSave can NOT be set to %d, must be 0 " )
	ASCII(.ascii	"or 1\012\000" )
	.space	3
.LC166:
	ASCII(.ascii	"NO SUCH CHANNEL : %d\012\000" )
	.space	2
.LC167:
	ASCII(.ascii	"Pay attention to  CHANNEL : %d\012\000" )
.LC168:
	ASCII(.ascii	"print memory tracer...\012\000" )
.LC169:
	ASCII(.ascii	"OK create file '%s' for channel %d raw stream save\012" )
	ASCII(.ascii	"\000" )
.LC170:
	ASCII(.ascii	"failed create file '%s' for raw stream save!\012\000" )
	.space	2
.LC171:
	ASCII(.ascii	">>>> close raw stream saving >>>>\012\000" )
	.space	1
.LC172:
	ASCII(.ascii	"OK create file '%s' for channel %d seg stream save\012" )
	ASCII(.ascii	"\000" )
.LC173:
	ASCII(.ascii	"failed create file '%s' for seg stream save!\012\000" )
	.space	2
.LC174:
	ASCII(.ascii	">>>> close seg stream saving >>>>\012\000" )
	.space	1
.LC175:
	ASCII(.ascii	"Failed Alloc VFMW_BigTile1d_YUV for yuv save!\012\000" )
	.space	1
.LC176:
	ASCII(.ascii	"OK create file '%s' for chan %d yuv save.(crop %d)\012" )
	ASCII(.ascii	"\000" )
.LC177:
	ASCII(.ascii	"failed create file '%s' for yuv save!\012\000" )
	.space	1
.LC178:
	ASCII(.ascii	"OK close yuv save.\012\000" )
.LC179:
	ASCII(.ascii	"address '0x%x' is not valid!\012\000" )
	.space	2
.LC180:
	ASCII(.ascii	"2d image has been saved to '%s'\012\000" )
	.space	3
.LC181:
	ASCII(.ascii	"open file '%s' fail!\012\000" )
	.space	2
.LC182:
	ASCII(.ascii	"1d image has been saved to '%s'\012\000" )
	.space	3
.LC183:
	ASCII(.ascii	"stop chan %d: VCTRL_StopChan return %d\012\000" )
.LC184:
	ASCII(.ascii	"reset chan %d: VCTRL_ResetChan return %d\012\000" )
	.space	2
.LC185:
	ASCII(.ascii	"start chan %d: VCTRL_StartChan return %d\012\000" )
	.space	2
.LC186:
	ASCII(.ascii	"------ dec task delay %d ms ------\012\000" )
.LC187:
	ASCII(.ascii	"------ dnr active per %d ms ------\012\000" )
.LC188:
	ASCII(.ascii	"------ dec stop = %d ------\012\000" )
	.space	3
.LC189:
	ASCII(.ascii	"------ g_TraceCtrl = %x ------\012\000" )
.LC190:
	ASCII(.ascii	"------ g_TraceBsPeriod = %x ------\012\000" )
.LC191:
	ASCII(.ascii	"------ g_TraceFramePeriod = %x ------\012\000" )
	.space	1
.LC192:
	ASCII(.ascii	"------ g_TraceImgPeriod = %x ------\012\000" )
	.space	3
.LC193:
	ASCII(.ascii	"The channel number is to Large.\012\000" )
	.space	3
.LC194:
	ASCII(.ascii	"------ s_pstVfmwChan[%d]->stSynExtraData.s32ModuleL" )
	ASCII(.ascii	"owlyEnable = 1 ------\012\000" )
	.space	2
.LC195:
	ASCII(.ascii	"------ Set SCD[%d] to work  ------\012\000" )
.LC196:
	ASCII(.ascii	"vdm_ctrl.c,L%d: pOption->s32KeepBS error\012\000" )
	.space	2
.LC197:
	ASCII(.ascii	"vdm_ctrl.c,L%d: %s\012\000" )
.LC198:
	ASCII(.ascii	"This Chan has not been created or has been destroye" )
	ASCII(.ascii	"d !!!\000" )
	.space	3
.LC199:
	ASCII(.ascii	"can NOT stop a not-opened Chan\000" )
	.space	1
.LC200:
	ASCII(.ascii	"WARNNING: ScdLowdlyEnable = %d, VidStd = %d, eDSPSt" )
	ASCII(.ascii	"ate = %d AVS(+) and LOWDLY  cannot supported in the" )
	ASCII(.ascii	" same time!!!\012\000" )
	.space	3
.LC201:
	ASCII(.ascii	"DFS, framsize wrong! framesize: 0x%x\012\000" )
	.space	2
.LC202:
	ASCII(.ascii	"%s, %d, AllocMemMMZ fail!\012\000" )
	.space	1
.LC203:
	ASCII(.ascii	"%s,can not use mmz. NeedMMZ: %d\012\000" )
	.space	3
.LC204:
	ASCII(.ascii	"%s, %d, AllocMemPre fail!\012\000" )
	.space	1
.LC205:
	ASCII(.ascii	"%s,can't use mmz. NeedMMZ: %d\012\000" )
	.space	1
.LC206:
	ASCII(.ascii	"VFMW: WriteBigTileYuv %dx%d...\012\000" )
.LC207:
	ASCII(.ascii	"call VCTRL_GetChanImage, ChanID = %d,\012\000" )
	.space	1
.LC208:
	ASCII(.ascii	"%s: view_0 id(%d) > view_1 id(%d), error!\012\000" )
	.space	1
.LC209:
	ASCII(.ascii	"%s: continuous view0 id(%d)!\012\000" )
	.space	2
.LC210:
	ASCII(.ascii	"%s: continuous view1 id(%d)!\012\000" )
	.bss
	.align	2
.LANCHOR0 = . + 0
.LANCHOR3 = . + 8184
	.type	s_pstVfmwChan, %object
	.size	s_pstVfmwChan, 64
s_pstVfmwChan:
	.space	64
	.type	s_VfmwCTrl, %object
	.size	s_VfmwCTrl, 176
s_VfmwCTrl:
	.space	176
	.type	g_SdecFunc, %object
	.size	g_SdecFunc, 28
g_SdecFunc:
	.space	28
	.type	s_StreamIntf, %object
	.size	s_StreamIntf, 192
s_StreamIntf:
	.space	192
	.type	FspInstInfo.31880, %object
	.size	FspInstInfo.31880, 2188
FspInstInfo.31880:
	.space	2188
	.type	g_VdmCharacter, %object
	.size	g_VdmCharacter, 4
g_VdmCharacter:
	.space	4
	.type	g_event_report, %object
	.size	g_event_report, 4
g_event_report:
	.space	4
	.type	last_rec_pos_time.32138, %object
	.size	last_rec_pos_time.32138, 64
last_rec_pos_time.32138:
	.space	64
	.type	extra_ref, %object
	.size	extra_ref, 4
extra_ref:
	.space	4
	.type	extra_disp, %object
	.size	extra_disp, 4
extra_disp:
	.space	4
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
	.type	g_DSPState, %object
	.size	g_DSPState, 64
g_DSPState:
	.space	64
	.type	last_rec_pos_time.32463, %object
	.size	last_rec_pos_time.32463, 64
last_rec_pos_time.32463:
	.space	64
	.type	last_rec_pos_decparam_time.32464, %object
	.size	last_rec_pos_decparam_time.32464, 64
last_rec_pos_decparam_time.32464:
	.space	64
	.type	ChanPriTab.32424, %object
	.size	ChanPriTab.32424, 64
ChanPriTab.32424:
	.space	64
	.type	g_VfmwChanStat, %object
	.size	g_VfmwChanStat, 4800
g_VfmwChanStat:
	.space	4800
	.type	AcceleratorCharacter, %object
	.size	AcceleratorCharacter, 4
AcceleratorCharacter:
	.space	4
	.type	g_SleepTime, %object
	.size	g_SleepTime, 4
g_SleepTime:
	.space	4
	.type	g_StopDec, %object
	.size	g_StopDec, 4
g_StopDec:
	.space	4
	.type	PrioArray.31979, %object
	.size	PrioArray.31979, 64
PrioArray.31979:
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
	.type	Capability.32031, %object
	.size	Capability.32031, 148
Capability.32031:
	.space	148
	.type	PrioArray.32030, %object
	.size	PrioArray.32030, 64
PrioArray.32030:
	.space	64
	.type	g_CurProcChan, %object
	.size	g_CurProcChan, 4
g_CurProcChan:
	.space	4
	.type	save_file_cnt.32609, %object
	.size	save_file_cnt.32609, 4
save_file_cnt.32609:
	.space	4
	.type	save_str_chan_num, %object
	.size	save_str_chan_num, 4
save_str_chan_num:
	.space	4
	.type	vfmw_save_seg_file, %object
	.size	vfmw_save_seg_file, 128
vfmw_save_seg_file:
	.space	128
	.type	save_file_cnt.32612, %object
	.size	save_file_cnt.32612, 4
save_file_cnt.32612:
	.space	4
	.type	save_seg_chan_num, %object
	.size	save_seg_chan_num, 4
save_seg_chan_num:
	.space	4
	.type	vfmw_save_yuv_file, %object
	.size	vfmw_save_yuv_file, 128
vfmw_save_yuv_file:
	.space	128
	.type	save_file_cnt.32617, %object
	.size	save_file_cnt.32617, 4
save_file_cnt.32617:
	.space	4
	.type	MemRecord_YUV, %object
	.size	MemRecord_YUV, 16
MemRecord_YUV:
	.space	16
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
	.type	g_DnrSleepTime, %object
	.size	g_DnrSleepTime, 4
g_DnrSleepTime:
	.space	4
	.type	last_rec_pos_time.32127, %object
	.size	last_rec_pos_time.32127, 64
last_rec_pos_time.32127:
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
	.size	g_VfmwGlobalStat, 84
g_VfmwGlobalStat:
	.space	84
	.ident	"GCC: (gcc-4.9.2 + glibc-2.22 (Build by czyong) Wed Mar  9 16:30:48 CST 2016) 4.9.2 20140904 (prerelease)"
	.section	.note.GNU-stack,"",%progbits
