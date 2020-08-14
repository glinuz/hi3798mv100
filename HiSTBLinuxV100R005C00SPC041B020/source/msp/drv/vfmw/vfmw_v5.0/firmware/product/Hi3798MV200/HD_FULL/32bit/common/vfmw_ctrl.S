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
	add	r3, r3, #307200
	mov	r0, #0
	str	r1, [r3, #1996]
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
	add	r0, r0, #307200
	ldr	r0, [r0, #1996]
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
	movwne	r0, #51240
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
	movweq	r3, #13848
	movwne	r3, #27120
	movteq	r3, 178
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
	add	r3, r0, #311296
	ldr	lr, [r3, #2040]
	cmp	lr, #0
	beq	.L426
	ldr	r3, [r3, #1532]
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
	movw	r2, #50684
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
	add	r2, r0, #278528
	movw	r3, #2082
	ldrsb	ip, [r2, r3]
	cmp	ip, #0
	ble	.L426
	add	r3, r0, #299008
	ldr	r3, [r3, #3012]
	cmp	r3, r1
	movwne	r3, #39976
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
	add	r3, r4, #299008
	add	r4, r4, #303104
	strb	r2, [r3, #2998]
	strb	r2, [r4, #2102]
	blx	r1
	cmp	r8, r5
	bls	.L429
	mov	r4, r5, asl #4
.L424:
	add	r5, r5, #1
	movw	r0, #50680
	ldr	r1, [r7, r6, asl #2]
	movt	r0, 4
	movw	r3, #50680
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
	movw	r3, #50680
	ldr	ip, [r9, #48]
	movt	r3, 4
	add	r3, lr, r3
	add	r0, r0, r3
	mov	r2, #16
	mov	r1, #0
	blx	ip
	ldr	r3, [r7, r6, asl #2]
	mov	r0, #0
	add	r3, r3, #311296
	ldr	r2, [r3, #2040]
	sub	r2, r2, #1
	str	r2, [r3, #2040]
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
	movw	r0, #51240
	movt	r0, 4
	mov	r1, r7
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	ldr	r3, [r7, #196]
	cmn	r3, #1
	mov	r5, r0
	beq	.L452
	ldr	r2, [r8, r4, asl #2]
	movw	r0, #51240
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
	add	r3, r4, #311296
	ldr	r5, [r3, #2100]
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
	add	r3, r4, #311296
	ldr	r5, [r3, #2456]
	b	.L500
.L492:
	add	r3, r4, #311296
	ldr	r5, [r3, #2112]
	b	.L500
.L497:
	add	r3, r4, #311296
	ldr	r5, [r3, #2324]
	b	.L500
.L498:
	add	r3, r4, #315392
	ldr	r5, [r3, #360]
	b	.L500
.L489:
	add	r3, r4, #311296
	ldr	r5, [r3, #3132]
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
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	subs	r4, r1, #0
	beq	.L520
	cmp	r0, #0
	beq	.L515
	cmp	r0, #16
	bne	.L521
	add	r0, r4, #4096
	ldrb	r0, [r0, #1184]	@ zero_extendqisi2
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L521:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L515:
	ldrb	r0, [r4, #1280]	@ zero_extendqisi2
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L520:
	ldr	r1, .L522
	mov	r0, r4
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L523:
	.align	2
.L522:
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
	bhi	.L525
	ldr	r5, .L536
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L525
	ldr	r1, [r3, #12]
	cmp	r1, #1
	bne	.L528
	ldr	r6, [r3, #16]
	cmp	r6, #1
	beq	.L529
.L528:
	ldr	r1, .L536+4
	mov	r0, #22
	mov	r4, #0
	bl	dprint_vfmw
.L527:
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L525:
	mov	r3, r2
	ldr	r1, .L536+8
	mov	r2, #5120
	mov	r0, #22
	bl	dprint_vfmw
	mov	r4, #0
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L529:
	str	r0, [fp, #-40]
	bl	VCTRL_GetDecParam
	ldr	r2, [fp, #-40]
	subs	r7, r0, #0
	beq	.L534
	ldr	r2, [r5, r2, asl #2]
	mov	r3, #0
	mov	r4, r3
	str	r3, [r2, #384]
	str	r3, [r2, #388]
	b	.L527
.L534:
	ldr	r8, [r5, r2, asl #2]
	ldr	r4, [r8, #384]
	cmp	r4, #0
	beq	.L535
	ldr	r3, .L536+12
	str	r2, [fp, #-40]
	ldr	r3, [r3]
	blx	r3
	ldr	r2, [fp, #-40]
	ldr	r3, [r5, r2, asl #2]
	ldr	r2, [r3, #388]
	cmp	r0, r2
	bls	.L532
	rsb	r0, r2, r0
	cmp	r0, #40
	movls	r4, r7
	bls	.L527
.L532:
	str	r7, [r3, #384]
	mov	r4, r6
	str	r7, [r3, #388]
	b	.L527
.L535:
	ldr	r3, .L536+12
	str	r6, [r8, #384]
	ldr	r3, [r3]
	blx	r3
	str	r0, [r8, #388]
	b	.L527
.L537:
	.align	2
.L536:
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
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	UNWIND(.pad #4)
	UNWIND(.save {r4, r5, r6, r7, fp, ip, lr})
	UNWIND(.setfp fp, ip, #-4)
	sub	fp, ip, #4
	cmp	r0, #15
	mov	r4, r0
	mov	r6, r1
	ldrhi	r3, .L551
	movwhi	r2, #5390
	bhi	.L548
	ldr	r5, .L551+4
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L550
	ldr	r3, [r3, #16]
	cmp	r3, #1
	beq	.L541
	ldr	r3, .L551+8
	movw	r2, #5392
.L548:
	ldr	r1, .L551+12
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L541:
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r0, [r5, r4, asl #2]
	ldr	r3, [r0, #600]
	cmp	r3, #0
	beq	.L543
	cmp	r3, #16
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	add	r0, r0, #311296
	mov	r1, r6
	add	r0, r0, #2096
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	HEVCDEC_VDMProcSedOnly
.L543:
	add	r0, r0, #311296
	mov	r1, r6
	add	r0, r0, #2096
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	H264DEC_VDMProcSedOnly
.L550:
	mov	r0, r3
	movw	r2, #5391
	ldr	r3, .L551+16
	ldr	r1, .L551+12
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L552:
	.align	2
.L551:
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
	ldr	r4, .L583
	mov	r5, r0
	ldr	r0, [r4, r0, asl #2]
	cmp	r0, #0
	beq	.L580
	add	r0, r0, #311296
	ldr	r2, [r0, #2088]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L573
.L557:
	.word	.L556
	.word	.L558
	.word	.L559
	.word	.L560
	.word	.L573
	.word	.L561
	.word	.L562
	.word	.L573
	.word	.L563
	.word	.L564
	.word	.L565
	.word	.L565
	.word	.L565
	.word	.L566
	.word	.L573
	.word	.L567
	.word	.L568
	.word	.L569
.L569:
	add	r0, r0, #2096
	bl	VP9DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L556:
	add	r0, r0, #2096
	bl	H264_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	r0, [r4, r5, asl #2]
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	FlushDPB
	cmp	r0, #0
	bne	.L581
.L573:
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L559:
	add	r0, r0, #2096
	bl	MPEG4DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L560:
	add	r0, r0, #2096
	bl	MPEG2DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L558:
	add	r0, r0, #2096
	bl	VC1DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L561:
	add	r0, r0, #2096
	bl	DIVX3DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L562:
	add	r0, r0, #2096
	bl	AVSDEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L563:
	add	r0, r0, #2096
	bl	REAL8DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L564:
	add	r0, r0, #2096
	bl	REAL9DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L565:
	add	r0, r0, #2096
	bl	VP6DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L566:
	add	r0, r0, #2096
	bl	VP8DEC_GetRemainImg
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L567:
	add	r0, r0, #2096
	bl	MVC_FlushDecoder
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	ldr	r0, [r4, r5, asl #2]
	mvn	r1, #0
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	MVC_FlushDPB
	cmp	r0, #0
	beq	.L573
	movw	r3, #5481
	ldr	r2, .L583+4
	ldr	r1, .L583+8
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L568:
	add	r0, r0, #2096
	bl	HEVC_FlushOutputFrmFromDPB
	cmp	r0, #0
	bne	.L582
.L571:
	ldr	r1, .L583+12
	mov	r0, #32
	bl	dprint_vfmw
	bl	VDMDRV_WakeUpVdm
	bl	VDMDRV_WakeUpVdm
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L582:
	movw	r2, #5458
	ldr	r1, .L583+16
	mov	r0, #0
	bl	dprint_vfmw
	b	.L571
.L581:
	movw	r2, #5448
	ldr	r1, .L583+16
	mov	r0, #22
	bl	dprint_vfmw
	mov	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L580:
	ldr	r3, .L583+20
	movw	r2, #5422
	ldr	r1, .L583+24
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L584:
	.align	2
.L583:
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
	bhi	.L608
	ldr	r1, .L611
	ldr	r3, [r1, r0, asl #2]
	cmp	r3, #0
	beq	.L609
	ldr	r0, [r3, #16]
	cmp	r0, #1
	bne	.L610
	ldr	r2, [r3, #600]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L604
.L591:
	.word	.L590
	.word	.L592
	.word	.L593
	.word	.L594
	.word	.L604
	.word	.L595
	.word	.L596
	.word	.L604
	.word	.L597
	.word	.L598
	.word	.L599
	.word	.L599
	.word	.L599
	.word	.L600
	.word	.L604
	.word	.L601
	.word	.L602
	.word	.L603
.L603:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP9DEC_GetImageBuffer
.L590:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageBuffer
.L592:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_GetImageBuffer
.L593:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageBuffer
.L594:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_GetImageBuffer
.L595:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_GetImageBuffer
.L596:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_GetImageBuffer
.L597:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_GetImageBuffer
.L598:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_GetImageBuffer
.L599:
	ldr	r2, [r1, #3740]
	tst	r2, #1
	ldmeqfd	sp, {fp, sp, pc}
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_GetImageBuffer
.L600:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_GetImageBuffer
.L601:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_GetImageBuffer
.L602:
	add	r0, r3, #311296
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageBuffer
.L604:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L610:
	ldr	r3, .L611+4
	movw	r2, #5546
	ldr	r1, .L611+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L608:
	ldr	r3, .L611+12
	movw	r2, #5544
	ldr	r1, .L611+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L609:
	mov	r0, r3
	movw	r2, #5545
	ldr	r3, .L611+16
	ldr	r1, .L611+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L612:
	.align	2
.L611:
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
	bhi	.L619
	ldr	r3, .L623
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L620
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L621
	cmp	r1, #0
	beq	.L622
	ldr	r2, [r1]
	mov	r0, #0
	str	r2, [r3, #1212]
	ldmfd	sp, {fp, sp, pc}
.L621:
	ldr	r3, .L623+4
	movw	r2, #5664
	ldr	r1, .L623+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L619:
	ldr	r3, .L623+12
	movw	r2, #5662
	ldr	r1, .L623+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L620:
	mov	r0, r3
	movw	r2, #5663
	ldr	r3, .L623+16
	ldr	r1, .L623+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L622:
	mov	r0, r1
	ldr	r3, .L623+20
	movw	r2, #5665
	ldr	r1, .L623+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L624:
	.align	2
.L623:
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
	bhi	.L631
	ldr	r3, .L635
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L632
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L633
	cmp	r1, #0
	beq	.L634
	ldr	r3, .L635+4
	add	r0, r0, #1232
	mov	r2, #20
	add	r0, r0, #12
	ldr	r3, [r3, #52]
	blx	r3
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L633:
	ldr	r3, .L635+8
	movw	r2, #5677
	ldr	r1, .L635+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L631:
	ldr	r3, .L635+16
	movw	r2, #5675
	ldr	r1, .L635+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L634:
	mov	r0, r1
	ldr	r3, .L635+20
	movw	r2, #5678
	ldr	r1, .L635+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L632:
	ldr	r3, .L635+24
	movw	r2, #5676
	ldr	r1, .L635+12
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L636:
	.align	2
.L635:
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
	ldr	r5, .L643
	mov	r4, r0
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L640
	sub	r1, fp, #44
	bl	SM_GetInfo
	cmp	r0, #0
	bne	.L640
	ldr	r3, [fp, #-36]
	cmp	r3, #16
	bgt	.L641
	ldr	r3, [fp, #-28]
	cmp	r3, #524288
	ble	.L640
.L641:
	mov	r0, r4
	bl	VCTRL_GetImageBuffer
	subs	r0, r0, #1
	mvnne	r0, #0
	cmp	r0, #0
	bne	.L639
	ldr	r3, .L643+4
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
.L640:
	mvn	r0, #0
.L639:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L644:
	.align	2
.L643:
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
	ldr	r3, .L646
	ldr	r3, [r3, r0, asl #2]
	ldr	r0, [r3, #372]
	subs	r0, r0, #1
	movne	r0, #1
	ldmfd	sp, {fp, sp, pc}
.L647:
	.align	2
.L646:
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
	bhi	.L654
	ldr	r3, .L655
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L654
	ldr	r2, [r3, #12]
	cmp	r2, #0
	beq	.L654
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq	.L654
	ldr	r2, [r3, #28]
	cmp	r2, #0
	beq	.L654
	ldr	r0, [r3, #600]
	cmp	r0, #21
	movne	r0, #0
	mvneq	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L654:
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L656:
	.align	2
.L655:
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
	ldr	r3, .L674
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
	ldr	ip, .L674+4
	mov	r2, #0
	str	r5, [r4]
.L661:
	ldr	r1, [ip], #4
	cmp	r1, #0
	add	r3, r1, #311296
	add	r3, r3, #2096
	beq	.L658
	cmp	r4, r3
	beq	.L673
.L658:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L661
	mvn	r3, #0
	str	r3, [r4, #360]
.L659:
	ldr	r1, .L674+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #19
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L673:
	cmn	r2, #1
	str	r2, [r4, #360]
	beq	.L659
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L675:
	.align	2
.L674:
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
	beq	.L678
	mov	r2, #0
	ldr	r0, [r4, #360]
	bl	FSP_SetDisplay
	mov	r1, r5
	ldr	r0, [r4, #360]
	bl	FSP_GetFsImagePtr
	cmp	r0, #0
	beq	.L679
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
.L679:
	mov	r0, #1
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L678:
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
	ldr	r7, .L702
	mov	r6, r0
	mov	r5, r1
	ldr	r3, [r7, r0, asl #2]
	cmp	r3, #0
	beq	.L697
	ldr	r2, [r3, #600]
	cmp	r2, #20
	bne	.L698
	ldr	r3, [r3, #524]
	cmp	r3, #2
	ble	.L699
	mov	r1, #1
	bl	FSP_NewLogicFs
	subs	r8, r0, #0
	blt	.L700
	mov	r1, r8
	mov	r0, r6
	bl	FSP_GetLogicFs
	mov	r2, r8
	ldr	r1, .L702+4
	mov	r4, r0
	mov	r0, #18
	bl	dprint_vfmw
	cmp	r4, #0
	beq	.L701
	ldr	r2, [r4, #640]
	cmp	r2, #0
	beq	.L687
	ldr	r3, [r4, #644]
	cmp	r3, #0
	beq	.L687
	ldr	r1, [r4, #648]
	cmp	r1, #0
	beq	.L687
	ldr	ip, [r1, #16]
	mov	r0, #18
	ldr	r2, [r2, #16]
	ldr	r3, [r3, #16]
	ldr	r1, .L702+8
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
.L682:
	mov	r0, r2
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L698:
	ldr	r1, .L702+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L682
.L699:
	ldr	r1, .L702+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L682
.L687:
	ldr	r1, .L702+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L682
.L700:
	ldr	r1, .L702+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L682
.L697:
	mov	r0, r3
	movw	r2, #6018
	ldr	r3, .L702+28
	ldr	r1, .L702+32
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L682
.L701:
	movw	r2, #6048
	ldr	r1, .L702+36
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r2, #0
	b	.L682
.L703:
	.align	2
.L702:
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
	ldr	r3, .L723
	ldr	r1, [r3, r0, asl #2]
	cmp	r1, #0
	beq	.L722
	ldr	r3, [r1, #600]
	cmp	r3, #20
	ldrls	pc, [pc, r3, asl #2]
	b	.L704
.L708:
	.word	.L707
	.word	.L709
	.word	.L710
	.word	.L711
	.word	.L704
	.word	.L712
	.word	.L713
	.word	.L704
	.word	.L714
	.word	.L715
	.word	.L716
	.word	.L716
	.word	.L716
	.word	.L717
	.word	.L704
	.word	.L718
	.word	.L719
	.word	.L720
	.word	.L704
	.word	.L704
	.word	.L721
.L721:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	USERDEC_Init
.L707:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_Init
.L709:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VC1DEC_Init
.L710:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_Init
.L711:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG2DEC_Init
.L712:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	DIVX3DEC_Init
.L713:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	AVSDEC_Init
.L714:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL8DEC_Init
.L715:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	REAL9DEC_Init
.L716:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP6DEC_Init
.L717:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP8DEC_Init
.L718:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MVC_DEC_Init
.L719:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_Init
.L720:
	add	r0, r1, #311296
	add	r1, r1, #608
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	VP9DEC_Init
.L704:
	ldmfd	sp, {fp, sp, pc}
.L722:
	mov	r0, r1
	ldr	r3, .L723+4
	movw	r2, #6103
	ldr	r1, .L723+8
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L724:
	.align	2
.L723:
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
	ldr	r3, .L728
	mov	r4, r0
	mov	r5, r1
	ldr	r3, [r3, #160]
	cmp	r3, #0
	mvneq	r0, #0
	beq	.L726
	blx	r3
.L726:
	mul	r4, r4, r0
	str	r4, [r5]
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L729:
	.align	2
.L728:
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
	movw	ip, #27120
	cmp	r0, #26
	movw	r5, #13848
	movt	ip, 12
	movt	r5, 178
	movne	r5, ip
	cmp	r3, #2
	mov	r7, r0
	mov	r4, r1
	mov	r6, r2
	ldreq	r10, [r1, #36]
	beq	.L733
	ldr	r1, .L763
	ldr	r2, [r4, #36]
	ldr	r0, [r4, #60]
	ldr	ip, [r1, #3812]
	ldr	r3, [r1, #3808]
	add	r8, r0, ip
	add	r1, r2, r3
	orrs	lr, r8, r1
	bmi	.L757
	cmp	r1, #16
	strle	r1, [r4, #36]
	strle	r8, [r4, #60]
	movle	r10, r1
	bgt	.L758
.L733:
	cmp	r10, #0
	blt	.L738
.L737:
	ldr	r2, [r4, #12]
	cmp	r2, #31
	ble	.L738
	ldr	r3, [r4, #16]
	cmp	r3, #31
	ble	.L738
	ldr	r1, [r4, #56]
	cmp	r1, #262144
	blt	.L738
	ldr	r1, [r4, #44]
	cmp	r1, #1
	beq	.L759
	cmp	r1, #2
	beq	.L760
.L740:
	ldr	r1, [r4, #40]
	cmp	r1, #0
	moveq	r0, #1
	bne	.L761
.L742:
	ldr	ip, .L763+4
	sub	r8, r7, #27
	cmp	r8, #6
	ldr	r1, [r4, #60]
	ldr	ip, [ip, #64]
	mvncc	r9, #1
	mvncs	r9, #0
	cmp	ip, #0
	beq	.L744
	ldr	lr, .L763+8
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
	ldr	r9, .L763
	cmp	r0, #0
	bne	.L744
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
	bne	.L762
.L747:
	cmp	r8, #5
	bhi	.L748
	movw	r2, #38076
	movt	r2, 19
	cmp	r3, r2
	movcc	r3, r2
.L748:
	mov	r0, #0
	add	r3, r3, r5
	str	r3, [r6, #8]
.L735:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L761:
	add	r0, r10, #1
	cmp	r0, #17
	movge	r0, #17
	b	.L742
.L762:
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
	bne	.L747
	movw	r0, #3992
	mov	r1, #2240
	mla	r2, r0, lr, r2
	mla	ip, r1, ip, r2
	cmp	r3, ip
	movcc	r3, ip
	b	.L747
.L760:
	ldr	r1, [r4, #20]
	cmp	r1, #0
	ble	.L738
	ldr	r1, [r4, #24]
	cmp	r1, #0
	ble	.L738
.L756:
	ldr	r1, [r4, #28]
	cmp	r1, #0
	ble	.L738
	ldr	r1, [r4, #32]
	cmp	r1, #0
	bgt	.L740
.L738:
	ldr	r1, .L763+12
	mov	r0, #32
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L744:
	movw	r2, #6390
	ldr	r1, .L763+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L759:
	ldr	r1, [r4, #20]
	cmp	r1, #0
	bgt	.L756
	b	.L738
.L758:
	ldr	r1, .L763+20
	mov	r0, #32
	bl	dprint_vfmw
	mov	r3, #16
	str	r8, [r4, #60]
	mov	r10, r3
	str	r3, [r4, #36]
	b	.L737
.L757:
	stmia	sp, {r0, ip}
	mov	r0, #32
	ldr	r1, .L763+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L735
.L764:
	.align	2
.L763:
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
	ldr	r10, .L790
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
	b	.L766
.L768:
	.word	.L781
	.word	.L781
	.word	.L781
	.word	.L781
	.word	.L781
	.word	.L769
	.word	.L769
	.word	.L769
.L781:
	mov	r3, #3
.L767:
	ldr	r8, .L790+4
	rsb	r3, r3, r9
	sub	r3, r3, #2
	ldr	r2, [r8, #3808]
	cmp	r2, #0
	movlt	r2, #0
	strlt	r2, [r8, #3808]
	cmp	r3, #15
	movgt	r7, #0
	bgt	.L771
	ldr	r7, [r8, #3808]
	add	r3, r3, r7
	cmp	r3, #16
	rsbgt	r3, r3, #16
	addgt	r7, r7, r3
.L771:
	add	r3, ip, r7
	ldr	r6, [r8, #3812]
	cmp	r3, #0
	ble	.L788
	cmp	r3, #17
	movge	r3, #17
	str	r3, [fp, #-56]
.L773:
	cmp	r4, #23
	bhi	.L774
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
	bne	.L789
.L774:
	cmp	r4, #32
	bhi	.L784
	ldr	r6, .L790+8
	ldr	ip, [r6, #64]
	cmp	ip, #0
	beq	.L776
	ldr	r3, [fp, #-68]
	mov	r0, #0
	ldr	r1, [fp, #-56]
	rsb	r5, r3, r5
	ldr	r8, .L790+12
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
	ldr	r10, .L790
	ldr	r7, .L790+16
	cmp	r0, #0
	bne	.L776
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
	beq	.L778
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
	bne	.L778
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
	beq	.L780
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
	bne	.L780
	ldr	r2, [fp, #-48]
	ldr	r3, [r7, #-3092]
	add	r3, r3, #307200
	add	r3, r3, #2576
	str	r3, [r2, #8]
	ldr	r2, [fp, #-52]
	ldr	r3, [r4, #24]
	add	r3, r3, #32768
	str	r3, [r2, #8]
.L775:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L769:
	mov	r3, #1
	b	.L767
.L789:
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
	ldr	r1, .L790+20
	bl	dprint_vfmw
	b	.L774
.L766:
	mov	r3, #6
	b	.L767
.L776:
	movw	r2, #6562
	ldr	r1, .L790+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L778:
	movw	r2, #6586
	ldr	r1, .L790+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L780:
	movw	r2, #6610
	ldr	r1, .L790+24
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L788:
	mov	r3, r7
	mov	r2, ip
	ldr	r1, .L790+28
	mov	r0, #32
	str	ip, [fp, #-56]
	bl	dprint_vfmw
	ldr	ip, [fp, #-56]
	cmp	ip, #17
	movlt	r3, ip
	movge	r3, #17
	str	r3, [fp, #-56]
	b	.L773
.L784:
	mvn	r0, #0
	b	.L775
.L791:
	.align	2
.L790:
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
	beq	.L793
	ldr	r5, [r0, #88]
	adds	r5, r5, #0
	movne	r5, #1
.L793:
	ldr	r6, [r4, #16]
	ldr	r7, [r4, #8]
	cmp	r6, #0
	ldr	r8, [r4, #4]
	beq	.L794
	ldr	r3, .L849
	ldr	r3, [r3, #60]
	cmp	r3, #0
	beq	.L795
	mov	r0, #0
	blx	r3
.L796:
	cmp	r5, #0
	beq	.L838
.L797:
	ldr	r3, .L849+4
	mov	r2, #24
	mov	r1, #0
	mov	r0, r4
	ldr	r3, [r3, #48]
	blx	r3
.L794:
	mov	r2, r6
	mov	r1, r8
	mov	r0, r7
	bl	MEM_DelMemRecord
	ldr	r6, [r4, #64]
	ldr	r7, [r4, #56]
	cmp	r6, #0
	ldr	r8, [r4, #52]
	beq	.L798
	ldr	r3, .L849
	ldr	r3, [r3, #60]
	cmp	r3, #0
	beq	.L799
	mov	r0, #0
	blx	r3
.L800:
	cmp	r5, #0
	beq	.L839
.L801:
	ldr	r3, .L849+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #48
	ldr	r3, [r3, #48]
	blx	r3
.L798:
	mov	r2, r6
	mov	r1, r8
	mov	r0, r7
	bl	MEM_DelMemRecord
	ldr	r6, [r4, #40]
	ldr	r7, [r4, #32]
	cmp	r6, #0
	ldr	r8, [r4, #28]
	bne	.L840
.L802:
	mov	r2, r6
	mov	r1, r8
	mov	r0, r7
	bl	MEM_DelMemRecord
	bl	SM_InstInit
	ldr	r5, [r4, #112]
	ldr	r6, [r4, #104]
	cmp	r5, #0
	ldr	r7, [r4, #100]
	bne	.L841
.L805:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #184]
	ldr	r6, [r4, #176]
	cmp	r5, #0
	ldr	r7, [r4, #172]
	bne	.L842
.L806:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #160]
	ldr	r6, [r4, #152]
	cmp	r5, #0
	ldr	r7, [r4, #148]
	bne	.L843
.L807:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #136]
	ldr	r6, [r4, #128]
	cmp	r5, #0
	ldr	r7, [r4, #124]
	bne	.L844
.L808:
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r5, [r4, #208]
	ldr	r6, [r4, #200]
	cmp	r5, #0
	ldr	r7, [r4, #196]
	bne	.L845
.L809:
	ldr	r4, .L849+8
	mov	r2, r5
	mov	r1, r7
	mov	r0, r6
	bl	MEM_DelMemRecord
	ldr	r3, [r4, #312]
	cmp	r3, #1
	beq	.L846
	ldr	r3, [r4, #316]
	ldr	r5, .L849+8
	cmp	r3, #1
	beq	.L847
.L811:
	ldr	r3, [r4, #320]
	ldr	r4, .L849+8
	cmp	r3, #1
	beq	.L848
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L840:
	mov	r0, #0
	bl	SM_CloseSCDDrv
	cmp	r5, #0
	beq	.L803
.L804:
	ldr	r3, .L849+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, r2
	ldr	r3, [r3, #48]
	blx	r3
	b	.L802
.L841:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L849+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #96
	ldr	r3, [r3, #48]
	blx	r3
	b	.L805
.L845:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L849+4
	add	r0, r4, #192
	mov	r2, #24
	mov	r1, #0
	ldr	r3, [r3, #48]
	blx	r3
	b	.L809
.L844:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L849+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #120
	ldr	r3, [r3, #48]
	blx	r3
	b	.L808
.L843:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L849+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #144
	ldr	r3, [r3, #48]
	blx	r3
	b	.L807
.L842:
	mov	r1, r7
	mov	r0, r6
	bl	MEM_UnmapRegisterAddr
	ldr	r3, .L849+4
	mov	r2, #24
	mov	r1, #0
	add	r0, r4, #168
	ldr	r3, [r3, #48]
	blx	r3
	b	.L806
.L795:
	mov	r0, r3
	movw	r2, #7098
	ldr	r1, .L849+12
	bl	dprint_vfmw
	b	.L796
.L799:
	mov	r0, r3
	movw	r2, #7119
	ldr	r1, .L849+12
	bl	dprint_vfmw
	b	.L800
.L848:
	ldr	r3, .L849+4
	mov	r1, #0
	mov	r0, #181
	ldr	r3, [r3, #188]
	blx	r3
	mov	r3, #0
	mov	r0, #0
	str	r3, [r4, #320]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L839:
	ldr	r1, [r4, #52]
	ldr	r0, [r4, #56]
	bl	MEM_ReleaseMemBlock
	b	.L801
.L838:
	ldr	r1, [r4, #4]
	ldr	r0, [r4, #8]
	bl	MEM_ReleaseMemBlock
	b	.L797
.L846:
	ldr	r3, .L849+4
	mov	r1, #0
	mov	r0, #127
	ldr	r5, .L849+8
	ldr	r3, [r3, #188]
	blx	r3
	mov	r3, #0
	str	r3, [r4, #312]
	ldr	r3, [r4, #316]
	cmp	r3, #1
	bne	.L811
.L847:
	ldr	r3, .L849+4
	mov	r1, #0
	mov	r0, #136
	ldr	r3, [r3, #188]
	blx	r3
	mov	r3, #0
	str	r3, [r5, #316]
	b	.L811
.L803:
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #32]
	bl	MEM_ReleaseMemBlock
	b	.L804
.L850:
	.align	2
.L849:
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
	bne	.L852
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
	beq	.L894
	mov	r2, #53248
	ldr	r1, .L899
	movt	r2, 63683
	mov	r0, r5
	bl	dprint_vfmw
	ldr	r3, .L899+4
	mov	r1, r5
.L889:
	mov	r0, r6
.L891:
	mov	r2, #24
.L888:
	ldr	r3, [r3, #48]
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
.L855:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L894:
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
	bne	.L856
	ldr	r2, [r4, #160]
	ldr	r1, [r4, #148]
	ldr	r0, [r4, #152]
	bl	MEM_AddMemRecord
	ldr	r3, [r4, #72]
	cmp	r3, #0
	bne	.L857
.L858:
	ldr	r5, .L899+4
	mov	r0, #12
	ldr	r7, .L899+8
	ldr	r8, [r5, #68]
	bl	VFMW_GetString
	mov	r3, #0
	mov	r1, #20
	mov	r2, r0
	sub	r0, fp, #84
	blx	r8
	ldr	r3, [r7, #48]
	cmp	r3, #0
	beq	.L864
	blx	r3
	subs	r2, r0, #0
	ble	.L864
	mov	r6, #0
	mov	r8, #3
	str	r6, [sp]
	mov	r1, r6
	mov	r3, r4
	str	r8, [r4, #20]
	sub	r0, fp, #84
	bl	MEM_AllocMemBlock
	subs	r9, r0, #0
	bne	.L865
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
	beq	.L867
	sub	r0, fp, #64
	blx	r3
	subs	r7, r0, #0
	bne	.L867
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
	bne	.L868
	ldr	r1, [r4, #52]
	add	r7, r4, #24
	ldr	r2, [r4, #64]
	ldr	r0, [r4, #56]
	bl	MEM_AddMemRecord
	ldr	r2, .L899+12
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
	bne	.L895
	ldr	r2, [r4, #40]
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #32]
	bl	MEM_AddMemRecord
	mov	r2, r8
	ldr	r1, [r4, #40]
	ldr	r0, [r4, #32]
	bl	SM_OpenSCDDrv
	cmp	r0, #0
	bne	.L896
.L862:
	bl	SM_InstInit
	mov	r0, #0
	ldr	r6, [r5, #184]
	bl	VFMW_GetString
	mov	r5, #0
	mov	r2, #32
	str	r5, [sp]
	ldr	r1, .L899+16
	ldr	r8, .L899+4
	mov	r3, r0
	mov	r0, #127
	blx	r6
	subs	r10, r0, #0
	bne	.L897
	ldr	r5, .L899+20
	mov	r0, #3
	ldr	r9, [r8, #184]
	mov	r6, #1
	str	r6, [r5, #312]
	bl	VFMW_GetString
	str	r10, [sp]
	mov	r2, #32
	ldr	r1, .L899+24
	mov	r3, r0
	mov	r0, #136
	blx	r9
	subs	r9, r0, #0
	bne	.L898
	mov	r0, #11
	ldr	r10, [r8, #184]
	str	r6, [r5, #316]
	bl	VFMW_GetString
	str	r9, [sp]
	mov	r2, #32
	ldr	r1, .L899+28
	mov	r3, r0
	mov	r0, #181
	blx	r10
	cmp	r0, #0
	streq	r6, [r5, #320]
	beq	.L855
	ldr	r1, .L899+32
	mov	r2, #181
	mov	r0, r9
	bl	dprint_vfmw
	ldr	r3, [r8, #48]
	mov	r1, r9
.L886:
	mov	r0, r7
.L887:
	mov	r2, #24
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L855
.L857:
	ldr	r2, [r4, #88]
	cmp	r2, #0
	beq	.L858
	ldr	r6, .L899+8
	ldr	r2, [r6, #48]
	cmp	r2, #0
	mvneq	r5, #0
	beq	.L859
	blx	r2
	ldr	r3, [r4, #72]
	mov	r5, r0
.L859:
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
	beq	.L860
	sub	r0, fp, #64
	blx	r3
	subs	r6, r0, #0
	bne	.L860
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
	bne	.L861
	ldr	r5, .L899+4
	b	.L862
.L852:
	mov	r2, #0
	ldr	r1, .L899
	movt	r2, 63683
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, .L899+4
	mov	r0, r6
	mov	r2, #24
	mov	r1, #0
	b	.L888
.L864:
	mov	r2, #0
	ldr	r1, .L899+36
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
	b	.L855
.L898:
	ldr	r1, .L899+32
	mov	r2, #136
	mov	r0, r10
	bl	dprint_vfmw
	ldr	r3, [r8, #48]
	mov	r1, r10
	b	.L886
.L897:
	ldr	r1, .L899+32
	mov	r2, #127
	mov	r0, r5
	bl	dprint_vfmw
	ldr	r3, [r8, #48]
	mov	r1, r5
	b	.L886
.L867:
	mov	r2, #0
	ldr	r1, .L899+40
	mov	r0, r2
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r2, #24
	mov	r1, #0
.L890:
	mov	r0, r4
	blx	r3
	mov	r0, r4
	bl	CloseDrivers
	mvn	r0, #0
	b	.L855
.L896:
	ldr	r1, .L899+44
	mov	r2, r8
	mov	r0, r8
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r8
	b	.L886
.L895:
	ldr	r1, .L899+48
	mov	r2, r6
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r6
	b	.L886
.L868:
	ldr	r1, .L899+52
	mov	r2, r7
	mov	r0, r7
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r7
	mov	r0, r9
	b	.L887
.L856:
	mov	r2, #49152
	ldr	r1, .L899
	movt	r2, 63683
	mov	r0, r7
	bl	dprint_vfmw
	ldr	r3, .L899+4
	mov	r1, r7
	b	.L889
.L865:
	mov	r2, r6
	ldr	r1, .L899+56
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, [r5, #48]
	mov	r1, r6
	mov	r2, #24
	b	.L890
.L860:
	mov	r2, #0
	ldr	r1, .L899+40
	mov	r0, r2
	bl	dprint_vfmw
	ldr	r3, .L899+4
	mov	r2, #24
	mov	r1, #0
	mov	r0, r4
	b	.L888
.L861:
	ldr	r1, .L899+60
	mov	r2, r6
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, .L899+4
	mov	r1, r6
	mov	r0, r7
	b	.L891
.L900:
	.align	2
.L899:
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
	ldr	r4, .L936
	ldr	r7, .L936+4
	mov	r6, r0
	mov	r2, #8
	sub	r0, fp, #44
	ldr	r3, [r4, #48]
	mov	r1, #0
	blx	r3
	ldr	r5, [r7, #64]
	cmp	r5, #0
	bne	.L931
	mov	r1, #1024
	ldr	r3, [r4, #80]
	str	r5, [fp, #-48]
	ldr	r0, .L936+8
	blx	r3
	ldr	r3, [r4, #96]
	ldr	r1, .L936+12
	ldr	r0, .L936+8
	blx	r3
	cmp	r0, #0
	str	r0, [fp, #-48]
	beq	.L904
	ldr	r3, [r4, #92]
	sub	r0, fp, #48
	ldr	r1, .L936+16
	blx	r3
	cmp	r0, #0
	beq	.L923
	sub	r0, fp, #48
	ldr	r3, [r4, #92]
	ldr	r1, .L936+20
	blx	r3
	cmp	r0, #0
	beq	.L932
	mov	r2, r5
	ldr	r3, [r4, #84]
	mov	r1, r5
	blx	r3
	mov	r2, r0
	str	r0, [r7, #3808]
.L908:
	add	r3, r2, #4992
	movw	r1, #10000
	add	r3, r3, #8
	movw	r8, #5000
	cmp	r3, r1
	bhi	.L909
	cmp	r2, r8
	movw	r3, #60536
.L930:
	movt	r3, 65535
	movge	r2, r8
	cmp	r2, r3
	movlt	r2, r3
.L905:
	str	r2, [r7, #3808]
.L907:
	mov	r9, #0
	ldr	r3, [r4, #80]
	mov	r1, #1024
	str	r9, [fp, #-48]
	ldr	r0, .L936+8
	blx	r3
	ldr	r3, [r4, #96]
	ldr	r1, .L936+24
	ldr	r0, .L936+8
	blx	r3
	ldr	r8, .L936
	cmp	r0, r9
	str	r0, [fp, #-48]
	beq	.L910
	ldr	r3, [r8, #92]
	sub	r0, fp, #48
	ldr	r1, .L936+16
	blx	r3
	cmp	r0, r9
	beq	.L911
	sub	r0, fp, #48
	ldr	r3, [r8, #92]
	ldr	r1, .L936+20
	blx	r3
	cmp	r0, r9
	beq	.L933
	mov	r2, r9
	ldr	r3, [r8, #84]
	mov	r1, r9
	blx	r3
	mov	r2, r0
	str	r0, [r7, #3812]
.L914:
	add	r3, r2, #4992
	movw	r1, #10000
	add	r3, r3, #8
	movw	r8, #5000
	cmp	r3, r1
	bhi	.L915
	cmp	r2, r8
	movw	r3, #60536
	movt	r3, 65535
	movlt	r5, r2
	movge	r5, r8
	cmp	r5, r3
	movlt	r5, r3
.L911:
	str	r5, [r7, #3812]
.L913:
	ldr	r7, .L936+28
	mov	r3, #0
	str	r3, [fp, #-48]
	ldr	r5, [r7, #-800]
	cmp	r5, r3
	beq	.L916
	ldr	r2, .L936+32
	ubfx	r1, r5, #1, #1
	and	r0, r5, #1
	ubfx	r3, r5, #2, #1
	strb	r0, [r7, #-796]
	ubfx	r5, r5, #3, #1
	strb	r1, [r2]
	strb	r3, [r7, #-795]
	strb	r5, [r7, #-794]
.L917:
	ldr	r5, .L936+4
	mov	r2, #272
	ldr	r3, [r4, #48]
	mov	r1, #0
	add	r0, r5, #64
	ldr	r8, .L936
	blx	r3
	ldr	r3, [r4, #48]
	mov	r2, #64
	mov	r1, #0
	mov	r0, r5
	blx	r3
	ldr	r2, .L936+36
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
	ldr	r0, .L936+40
	blx	r3
	ldr	r3, [r4, #52]
	add	r1, r6, #8
	mov	r2, #24
	add	r0, r5, #160
	blx	r3
	add	r0, r5, #88
	bl	OpenDrivers
	cmp	r0, #0
	bne	.L931
	ldr	r3, [r8, #120]
	mov	r1, #1
	blx	r3
	ldr	r3, [r8, #180]
	ldr	r2, .L936+44
	add	r0, r5, #76
	ldr	r1, .L936+48
	blx	r3
	cmp	r0, #0
	moveq	r3, #1
	streq	r3, [r5, #64]
	bne	.L934
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L916:
	ldr	r3, [r4, #80]
	mov	r1, #1024
	sub	r0, r7, #1824
	ldr	r8, .L936
	blx	r3
	ldr	r3, [r4, #96]
	ldr	r1, .L936+52
	sub	r0, r7, #1824
	blx	r3
	cmp	r0, #0
	str	r0, [fp, #-48]
	beq	.L917
	ldr	r3, [r8, #92]
	sub	r0, fp, #48
	ldr	r1, .L936+16
	blx	r3
	cmp	r0, #0
	ldreq	r0, [r7, #-800]
	beq	.L919
	sub	r0, fp, #48
	ldr	r3, [r8, #92]
	ldr	r1, .L936+20
	blx	r3
	cmp	r0, #0
	beq	.L935
	ldr	r3, [r8, #84]
	mov	r2, r5
	mov	r1, r5
	blx	r3
	str	r0, [r7, #-800]
.L919:
	ldr	r2, .L936+32
	ubfx	r1, r0, #1, #1
	and	ip, r0, #1
	ubfx	r3, r0, #2, #1
	strb	ip, [r7, #-796]
	ubfx	r0, r0, #3, #1
	strb	r1, [r2]
	strb	r3, [r7, #-795]
	strb	r0, [r7, #-794]
	b	.L917
.L910:
	ldr	r2, [r7, #3812]
	b	.L914
.L904:
	ldr	r2, [r7, #3808]
	b	.L908
.L915:
	ldr	r1, .L936+56
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r5, [r7, #3812]
	movw	r3, #60536
	cmp	r5, r8
	movt	r3, 65535
	movge	r5, r8
	cmp	r5, r3
	movlt	r5, r3
	b	.L911
.L909:
	ldr	r1, .L936+60
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r2, [r7, #3808]
	movw	r3, #60536
	cmp	r2, r8
	b	.L930
.L923:
	mov	r2, r5
	b	.L905
.L934:
	add	r0, r5, #88
	bl	CloseDrivers
.L931:
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L932:
	movw	r3, #8404
	ldr	r2, .L936+64
	ldr	r1, .L936+68
	mov	r0, #22
	bl	dprint_vfmw
	b	.L907
.L933:
	movw	r3, #8451
	ldr	r2, .L936+72
	ldr	r1, .L936+68
	mov	r0, #22
	bl	dprint_vfmw
	b	.L913
.L935:
	movw	r3, #8508
	ldr	r2, .L936+76
	ldr	r1, .L936+68
	mov	r0, #22
	bl	dprint_vfmw
	b	.L917
.L937:
	.align	2
.L936:
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
	ldr	r1, .L946
	mov	r2, r5
	mov	r0, #30
	bl	dprint_vfmw
	cmp	r6, #3
	beq	.L944
	cmp	r4, r8
	movge	r4, r6
	blt	.L945
.L940:
	mov	r2, r6
	mov	r3, r4
	ldr	r1, .L946+4
	mov	r0, #30
	bl	dprint_vfmw
	mov	r0, r4
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L945:
	ldr	r1, .L946+8
	mov	r0, #30
	bl	dprint_vfmw
.L943:
	mov	r4, #3
	b	.L940
.L944:
	cmp	r5, r8
	ble	.L943
	ldr	r1, .L946+12
	mov	r0, #30
	bl	dprint_vfmw
	mov	r4, #0
	b	.L940
.L947:
	.align	2
.L946:
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
	ldr	ip, .L957
	mov	r2, #0
.L951:
	ldr	r1, [ip], #4
	cmp	r1, #0
	add	r3, r1, #311296
	add	r3, r3, #2096
	beq	.L949
	cmp	r0, r3
	beq	.L952
.L949:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L951
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L952:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L958:
	.align	2
.L957:
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
	ldr	ip, .L970
	mov	r2, #0
.L963:
	ldr	r3, [ip], #4
	cmp	r3, #0
	beq	.L960
	ldr	r1, [r3, #228]
	cmp	r1, r0
	bhi	.L961
	ldr	lr, [r3, #236]
	add	r1, r1, lr
	cmp	r0, r1
	bcc	.L965
.L961:
	ldr	r1, [r3, #252]
	cmp	r0, r1
	bcc	.L960
	ldr	r3, [r3, #260]
	add	r1, r1, r3
	cmp	r0, r1
	bcc	.L965
.L960:
	add	r2, r2, #1
	cmp	r2, #16
	bne	.L963
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L965:
	mov	r0, r2
	ldmfd	sp, {fp, sp, pc}
.L971:
	.align	2
.L970:
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
	bhi	.L1041
	ldr	r7, .L1057
	ldr	r0, [r7, r0, asl #2]
	cmp	r0, #0
	beq	.L1042
	ldr	r6, [r0, #16]
	cmp	r6, #1
	bne	.L1043
	ldr	r5, [r0, #32]
	cmp	r5, #0
	bne	.L1044
	ldr	r3, .L1057+4
	mov	r10, r4, asl #2
	str	r5, [r0, #372]
	ldr	r3, [r3]
	blx	r3
	ldr	r3, .L1057+8
	ldr	r1, .L1057+12
	add	r2, r3, r10
	ldr	r3, [r2, #-724]
	ldr	r1, [r1]
	rsb	r3, r3, r0
	cmp	r3, r1
	ldr	r3, [r7, r4, asl #2]
	strhi	r0, [r2, #-724]
	movls	r6, r5
	ldr	r1, .L1057
	ldr	r3, [r3, #600]
	sub	r2, r3, #10
	cmp	r2, #2
	bhi	.L979
	ldr	r2, [r1, #3740]
	tst	r2, #1
	bne	.L979
.L980:
	mov	r0, r4
	bl	VCTRL_GetImageBuffer
	cmp	r0, #1
	beq	.L981
	cmp	r6, #1
	mvnne	r5, #2
	beq	.L1045
.L1019:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L979:
	bic	r2, r3, #16
	cmp	r2, #4
	cmpne	r3, #14
	beq	.L980
	mov	r0, r4
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L980
	cmp	r6, #1
	mvnne	r5, #1
	bne	.L1019
	mov	r2, r4
	ldr	r1, .L1057+16
	mov	r0, #23
	mvn	r5, #1
	bl	dprint_vfmw
	b	.L1019
.L981:
	ldr	ip, [r7, r4, asl #2]
	ldr	r2, [ip, #1440]
	cmp	r2, #0
	bne	.L1046
.L982:
	ldrb	r3, [ip, #745]	@ zero_extendqisi2
	cmp	r3, #0
	addne	r3, ip, #752
	strne	r3, [fp, #-48]
	bne	.L985
	ldr	r3, [ip, #1480]
	cmp	r3, #0
	add	r3, ip, #752
	strne	r3, [fp, #-48]
	beq	.L1047
.L985:
	ldr	r3, [fp, #-48]
	str	ip, [fp, #-68]
	ldrd	r0, [r3, #40]
	ldrd	r8, [r3, #24]
	strd	r0, [fp, #-60]
	mov	r0, #29
	ldrd	r2, [fp, #-60]
	ldr	r1, .L1057+20
	strd	r2, [sp]
	mov	r3, r9
	mov	r2, r8
	bl	dprint_vfmw
	ldr	r3, [r7, r4, asl #2]
	ldr	ip, [fp, #-68]
	ldr	r1, [r3, #52]
	str	r3, [fp, #-60]
	cmp	r1, #1
	beq	.L1048
	sub	r3, r1, #2
	cmp	r1, #6
	cmpne	r3, #1
	bls	.L1006
.L988:
	ldr	r3, [fp, #-48]
	mvn	r2, #0
	ldrd	r0, [r3, #24]
	mvn	r3, #0
	cmp	r1, r3
	ldr	r3, [fp, #-60]
	cmpeq	r0, r2
	add	r3, r3, #672
	beq	.L1049
	strd	r0, [r3, #-8]
.L999:
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
	ldr	r1, .L1057+24
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
.L997:
	mov	r0, #3
	str	ip, [fp, #-48]
	bl	CHECK_REC_POS_ENABLE
	ldr	ip, [fp, #-48]
	adds	r3, r0, #0
	movne	r3, #1
	cmp	r6, #1
	movne	r3, #0
	cmp	r3, #0
	bne	.L1050
.L1000:
	ldr	r3, [r7, r4, asl #2]
	add	r0, ip, #744
	ldr	r6, .L1057
	add	r2, r3, #311296
	ldr	r1, [r3, #600]
	add	r2, r2, #2096
	bl	DecodeStream
	ldr	r0, [r7, r4, asl #2]
	ldr	r3, [r0, #600]
	cmp	r3, #16
	beq	.L1051
.L1001:
	mov	r0, #4
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	beq	.L1019
	ldr	r3, .L1057+4
	ldr	r3, [r3]
	blx	r3
	ldr	r3, .L1057+8
	ldr	r1, .L1057+28
	add	r8, r3, r10
	ldr	r3, [r8, #-660]
	ldr	r1, [r1]
	rsb	r3, r3, r0
	mov	r2, r0
	cmp	r3, r1
	bls	.L1019
	mov	r0, r4
	str	r2, [r8, #-660]
	bl	VCTRL_GetDecParam
	cmp	r0, #0
	beq	.L1019
	mov	r2, r4, asl #24
	movw	r1, #7897
	add	r2, r2, #262144
	ldr	r0, .L1057+32
	bl	DBG_AddTrace
	b	.L1019
.L1046:
	cmp	r2, #1
	addne	ip, ip, #307200
	ldrne	r3, [ip, #1952]
	beq	.L1052
.L1005:
	ldr	r1, .L1057+36
	mov	r0, #31
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1019
.L1045:
	mov	r2, r4
	ldr	r1, .L1057+40
	mov	r0, #23
	mvn	r5, #2
	bl	dprint_vfmw
	b	.L1019
.L1048:
	add	r3, r3, #311296
	ldr	r3, [r3, #2096]
	ldr	r1, [r3, #556]
	sxtb	r1, r1
	cmp	r1, #1
	bne	.L988
.L1006:
	ldr	r3, [fp, #-60]
	ldr	r2, [fp, #-48]
	add	r0, r3, #1152
	str	ip, [fp, #-68]
	add	r0, r0, #8
	bl	IsNewpicSeg
	ldr	ip, [fp, #-68]
	cmp	r0, #0
	bne	.L989
	ldr	r1, [r7, r4, asl #2]
	add	r0, r1, #672
	ldr	r3, [r1, #52]
	cmp	r3, #3
	beq	.L1053
	ldrd	r8, [r0]
	mvn	r3, #0
	mvn	r2, #0
	cmp	r9, r3
	cmpeq	r8, r2
	beq	.L1054
	mov	r2, r8
	mov	r3, r9
	strd	r2, [r0, #-8]
.L991:
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
.L993:
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
	ldr	r1, .L1057+44
	bl	dprint_vfmw
	ldr	lr, [r7, r4, asl #2]
	ldr	ip, [fp, #-60]
	ldr	r3, [lr, #52]
	cmp	r3, #3
	beq	.L1055
.L994:
	ldr	r3, [fp, #-48]
	mvn	r2, #0
	ldrd	r0, [r3, #24]
	mvn	r3, #0
	cmp	r1, r3
	cmpeq	r0, r2
	addne	r3, lr, #672
	strned	r0, [r3]
.L996:
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
.L995:
	ldrd	r2, [r1, #-8]
	mov	r0, #29
	ldr	r1, .L1057+48
	str	ip, [fp, #-48]
	bl	dprint_vfmw
	ldr	ip, [fp, #-48]
	b	.L997
.L1047:
	mov	r0, r4
	mov	r1, r3
	str	ip, [fp, #-60]
	str	r3, [fp, #-48]
	bl	SM_ReadSegStream
	ldr	ip, [fp, #-60]
	cmp	r0, #0
	beq	.L985
	cmp	r6, #1
	beq	.L1056
.L986:
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #1
	mvn	r5, #3
	str	r2, [r3, #372]
	b	.L1019
.L1043:
	ldr	r3, .L1057+52
	movw	r2, #7649
	ldr	r1, .L1057+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1019
.L1044:
	ldr	r3, .L1057+60
	movw	r2, #7650
	ldr	r1, .L1057+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1019
.L1051:
	ldr	r3, [r0, #1504]
	cmp	r3, #1
	beq	.L1001
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	IsMain10Profile
	cmp	r0, #0
	ldreq	r3, [r6, r4, asl #2]
	streq	r0, [r3, #1504]
	beq	.L1001
	mov	r3, r4
	ldr	r2, .L1057+32
	ldr	r1, .L1057+64
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r6, r4, asl #2]
	mov	r2, #1
	str	r2, [r3, #1504]
	bl	VDMDRV_AddMatchMode
	b	.L1001
.L1052:
	add	r3, ip, #307200
	ldr	r3, [r3, #1952]
	bics	r1, r3, #2
	beq	.L982
	cmp	r3, #1
	bne	.L1005
	ldr	r1, .L1057+68
	mov	r0, #31
	bl	dprint_vfmw
	mvn	r5, #2
	b	.L1019
.L1055:
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
.L989:
	ldr	lr, [r7, r4, asl #2]
	ldr	r3, [lr, #52]
	cmp	r3, #3
	addeq	r1, lr, #688
	bne	.L994
	b	.L995
.L1049:
	ldrd	r0, [r3, #-8]
	b	.L999
.L1041:
	ldr	r3, .L1057+72
	movw	r2, #7647
	ldr	r1, .L1057+56
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r5, #0
	b	.L1019
.L1050:
	mov	r2, r4, asl #24
	movw	r1, #7859
	add	r2, r2, #196608
	ldr	r0, .L1057+32
	bl	DBG_AddTrace
	ldr	ip, [fp, #-48]
	b	.L1000
.L1053:
	ldrd	r2, [r0]
	mov	r8, r2
	mov	r9, r3
	strd	r2, [r0, #-8]
	b	.L991
.L1054:
	ldrd	r2, [r0, #-8]
	mov	r8, r2
	mov	r9, r3
	b	.L991
.L1042:
	ldr	r3, .L1057+76
	movw	r2, #7648
	ldr	r1, .L1057+56
	mvn	r5, #0
	bl	dprint_vfmw
	b	.L1019
.L1056:
	mov	r2, r4
	ldr	r1, .L1057+80
	mov	r0, #23
	bl	dprint_vfmw
	b	.L986
.L1058:
	.align	2
.L1057:
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
	ldr	r0, .L1088
	ldr	r1, .L1088+4
	sub	r3, r0, #64
.L1062:
	ldr	r2, [r1, #4]!
	add	r3, r3, #4
	cmp	r2, #0
	ldrne	r2, [r2, #28]
	str	r2, [r3, #-4]
	cmp	r3, r0
	bne	.L1062
	ldr	r7, .L1088+8
	mvn	ip, #0
	ldr	r6, .L1088+12
	mov	r8, #0
	ldr	r10, .L1088+16
	mov	r9, r7
.L1063:
	mov	r3, #0
	ldr	r0, [r6, #-596]
	mov	r4, r3
	ldr	r1, .L1088+20
	b	.L1064
.L1066:
	ldr	r2, [r1, #4]!
	cmp	r0, r2
	movlt	r0, r2
	movlt	r4, r3
.L1064:
	add	r3, r3, #1
	cmp	r3, #16
	bne	.L1066
	add	r3, r6, r4, lsl #2
	cmp	r0, #0
	add	r2, r4, #1
	mov	r1, #0
	str	r8, [r3, #-596]
	str	r2, [r7, #84]
	ldr	r3, .L1088+8
	ble	.L1067
	ldr	r3, [r9, r4, asl #2]
	cmp	r3, r1
	beq	.L1063
	ldr	r2, [r3, #16]
	cmp	r2, #1
	bne	.L1063
	ldr	r2, [r3, #76]
	cmp	r2, r1
	ble	.L1069
	ldr	r5, [r3, #80]
	cmp	r5, r1
	beq	.L1086
.L1069:
	mov	r5, #200
	b	.L1074
.L1072:
	cmp	r0, #0
	bne	.L1063
	subs	r5, r5, #1
	beq	.L1063
.L1074:
	mov	r0, r4
	bl	DecChanSyntax
	cmn	r0, #1
	mov	ip, r0
	bne	.L1072
	ldr	r5, [r6, #-792]
	cmp	r5, #0
	beq	.L1080
	mov	r3, #0
	str	r0, [fp, #-72]
	mov	r2, r3
	mov	r0, r4
	mov	r1, #102
	blx	r5
	ldr	ip, [fp, #-72]
.L1080:
	mov	r0, ip
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1086:
	mov	r0, r4
	sub	r1, fp, #68
	bl	SM_GetInfo
	cmp	r0, #0
	bne	.L1087
.L1070:
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
	b	.L1069
.L1067:
	mov	r0, ip
	str	r1, [r3, #84]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1087:
	mov	r1, r5
	sub	r0, fp, #68
	ldr	r3, [r10, #48]
	mov	r2, #24
	blx	r3
	b	.L1070
.L1089:
	.align	2
.L1088:
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
	ldr	r4, .L1099
	mov	r3, #100
	ldr	r5, .L1099+4
	ldr	r6, .L1099+8
	ldr	r0, [r4, #-532]
	str	r3, [r5, #80]
	cmp	r0, #0
	ble	.L1091
	ldr	r3, [r6, #116]
	blx	r3
.L1091:
	ldr	r3, [r6, #12]
	mov	r0, #0
	blx	r3
	bl	SM_Wakeup
	ldr	r3, [r4, #-528]
	mov	r2, #101
	str	r2, [r5, #80]
	cmp	r3, #0
	movne	r7, #0
	beq	.L1098
.L1092:
	mov	r3, #102
	str	r3, [r5, #80]
	bl	VDMDRV_WakeUpVdm
	mov	r4, #0
.L1093:
	mov	r0, r4
	mov	r1, #0
	add	r4, r4, #1
	bl	DeleteRawPacketInBuffer
	cmp	r4, #16
	bne	.L1093
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
.L1098:
	bl	DecSyntax
	mov	r7, r0
	b	.L1092
.L1100:
	.align	2
.L1099:
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
	ldr	r4, .L1128
	ldr	r6, .L1128+4
.L1102:
.L1109:
	ldr	r1, .L1128+8
	mov	r0, #2
	mov	r3, #0
	str	r3, [r4, #80]
	bl	dprint_vfmw
	ldr	r3, [r4, #72]
	mov	r5, #2
	str	r5, [r4, #68]
	cmp	r3, r5
	bne	.L1124
	b	.L1107
.L1126:
	str	r5, [r4, #80]
	bl	VCTRL_RunProcess
	ldr	r1, .L1128+12
	mov	r0, #22
	bl	dprint_vfmw
	ldr	r3, [r4, #72]
	cmp	r3, #2
	beq	.L1107
.L1124:
	cmp	r3, #3
	bne	.L1126
.L1105:
	ldr	r1, .L1128+16
	mov	r0, #2
	mov	r3, #6
	str	r3, [r4, #80]
	bl	dprint_vfmw
	mov	r3, #0
	mov	r0, r3
	str	r3, [r4, #68]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1107:
	ldr	r1, .L1128+20
	mov	r0, #2
	mov	r3, #4
	str	r3, [r4, #80]
	bl	dprint_vfmw
	mov	r3, #1
	str	r3, [r4, #68]
	b	.L1104
.L1127:
	ldr	r3, [r6, #116]
	blx	r3
.L1104:
	ldr	r1, .L1128+24
	mov	r0, #22
	mov	r3, #5
	str	r3, [r4, #80]
	bl	dprint_vfmw
	ldr	r3, [r4, #72]
	mov	r0, #10
	cmp	r3, #1
	beq	.L1109
	cmp	r3, #3
	bne	.L1127
	b	.L1105
.L1129:
	.align	2
.L1128:
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
	ldr	r3, .L1149
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	beq	.L1147
	ldr	r2, [r3, #600]
	cmp	r2, #20
	ldrls	pc, [pc, r2, asl #2]
	b	.L1148
.L1133:
	.word	.L1146
	.word	.L1134
	.word	.L1135
	.word	.L1136
	.word	.L1148
	.word	.L1137
	.word	.L1138
	.word	.L1148
	.word	.L1139
	.word	.L1140
	.word	.L1141
	.word	.L1141
	.word	.L1141
	.word	.L1142
	.word	.L1148
	.word	.L1143
	.word	.L1144
	.word	.L1145
	.word	.L1148
	.word	.L1148
	.word	.L1146
.L1146:
	movw	r0, #51256
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1134:
	add	r0, r3, #360448
	add	r0, r0, #2160
	ldmfd	sp, {fp, sp, pc}
.L1135:
	add	r0, r3, #335872
	add	r0, r0, #152
	ldmfd	sp, {fp, sp, pc}
.L1136:
	add	r0, r3, #397312
	add	r0, r0, #3792
	ldmfd	sp, {fp, sp, pc}
.L1137:
	add	r0, r3, #352256
	add	r0, r0, #2640
	ldmfd	sp, {fp, sp, pc}
.L1138:
	add	r0, r3, #327680
	add	r0, r0, #1840
	ldmfd	sp, {fp, sp, pc}
.L1139:
	movw	r0, #39544
	movt	r0, 5
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1140:
	movw	r0, #39528
	movt	r0, 5
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1141:
	add	r0, r3, #323584
	add	r0, r0, #2480
	ldmfd	sp, {fp, sp, pc}
.L1142:
	movw	r0, #59432
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1143:
	movw	r0, #51832
	movt	r0, 4
	add	r0, r3, r0
	ldmfd	sp, {fp, sp, pc}
.L1144:
	add	r0, r3, #372736
	add	r0, r0, #2416
	ldmfd	sp, {fp, sp, pc}
.L1145:
	add	r0, r3, #581632
	add	r0, r0, #2960
	ldmfd	sp, {fp, sp, pc}
.L1148:
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1147:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L1150:
	.align	2
.L1149:
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
	bhi	.L1162
	bl	VCTRL_GetChanVoQue
	subs	r6, r0, #0
	beq	.L1159
	sub	r2, fp, #44
	mov	r3, #0
	sub	r1, fp, #52
	str	r3, [fp, #-52]
	str	r3, [r2, #-4]!
	bl	GetQueueImgNum
	ldr	r2, [fp, #-52]
	cmp	r2, #0
	beq	.L1159
	cmp	r2, #32
	bhi	.L1155
.L1157:
	ldr	r9, .L1163
	mov	r4, #0
	mov	lr, r4
	mov	r7, r4
	str	r8, [fp, #-56]
	str	r9, [fp, #-60]
.L1156:
	ldrh	r1, [r6, #4]
	movw	ip, #50680
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
	add	r5, r2, #311296
	add	ip, r2, ip
	add	r5, r5, #1536
	ldr	r2, .L1163+4
	ldr	r10, [r10, #28]
	uxth	r4, r4
	ldr	r1, .L1163+8
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
	bcc	.L1156
	ldr	r8, [fp, #-56]
	ldr	r9, [fp, #-60]
.L1158:
	ldr	r2, [r9, r8, asl #2]
	mov	r0, #0
	add	r2, r2, #311296
	str	r3, [r2, #2040]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1159:
	mvn	r0, #0
.L1153:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1155:
	mov	r3, #32
	ldr	r1, .L1163+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [fp, #-52]
	cmp	r3, #0
	ldreq	r9, .L1163
	bne	.L1157
	b	.L1158
.L1162:
	ldr	r3, .L1163+16
	movw	r2, #4190
	ldr	r1, .L1163+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1153
.L1164:
	.align	2
.L1163:
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
	bhi	.L1182
	cmp	r1, #0
	beq	.L1183
	bl	VCTRL_GetChanVoQue
	subs	r5, r0, #0
	mvneq	r0, #0
	beq	.L1167
	ldr	r6, .L1185
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
	beq	.L1169
	cmp	r2, #32
	bhi	.L1184
.L1170:
	mov	ip, #0
	mov	r0, #0
	mov	r1, #0
	mov	lr, ip
	mov	r8, ip
.L1172:
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
	bcc	.L1172
	uxth	r3, r2
.L1171:
	ldrh	r1, [r5, #4]
	add	r3, r3, r1
	and	r3, r3, #63
	strh	r3, [r5, #4]	@ movhi
	str	r2, [r4, #512]
.L1173:
	ldr	r3, [r6, #16]
	mov	r0, #4
	blx	r3
	mov	r0, #0
.L1167:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1169:
	ldr	r8, .L1185+4
	ldr	ip, [r8, r7, asl #2]
	add	r3, ip, #311296
	ldr	r3, [r3, #2040]
	cmp	r3, #0
	beq	.L1173
	mov	r0, #0
	mov	r1, #0
	mov	r5, r2
	mov	r9, r2
.L1174:
	mov	r3, r5, asl #4
	movw	lr, #50680
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
	add	r3, ip, #311296
	ldr	r3, [r3, #2040]
	cmp	r5, r3
	bcc	.L1174
	str	r3, [r4, #512]
	b	.L1173
.L1184:
	mov	r3, #32
	ldr	r1, .L1185+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r2, [fp, #-44]
	cmp	r2, #0
	moveq	r3, r2
	bne	.L1170
	b	.L1171
.L1182:
	ldr	r3, .L1185+12
	movw	r2, #4305
	ldr	r1, .L1185+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1167
.L1183:
	mov	r0, r1
	ldr	r3, .L1185+20
	movw	r2, #4306
	ldr	r1, .L1185+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1167
.L1186:
	.align	2
.L1185:
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
	ldr	r4, .L1198
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	mov	r8, r3
	ldr	ip, [r4, r0, asl #2]
	cmp	ip, #0
	beq	.L1195
	bl	VCTRL_GetChanVoQue
	subs	r3, r0, #0
	beq	.L1189
	mov	r2, r8
	mov	r1, r7
	bl	GetQueueImgNum
	ldr	r3, [r4, r5, asl #2]
	ldr	r2, [r3, #600]
	cmp	r2, #0
	beq	.L1196
	cmp	r2, #16
	beq	.L1197
	cmp	r2, #17
	moveq	r3, #9
	streq	r3, [r6]
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	cmp	r2, #15
	addeq	r3, r3, #11403264
	addeq	r3, r3, #32768
	movne	r3, #2
	ldreq	r3, [r3, #856]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1196:
	add	r3, r3, #339968
	ldr	r3, [r3, #144]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1189:
	str	r3, [r8]
	str	r3, [r7]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1197:
	add	r3, r3, #315392
	ldr	r3, [r3, #536]
	str	r3, [r6]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1195:
	mov	r0, ip
	ldr	r3, .L1198+4
	movw	r2, #8042
	ldr	r1, .L1198+8
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L1199:
	.align	2
.L1198:
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
	bhi	.L1213
	cmp	r1, #0
	mov	r4, #0
	str	r4, [fp, #-56]
	str	r4, [fp, #-60]
	str	r4, [fp, #-64]
	beq	.L1200
	ldr	r3, .L1214
	mov	r2, #100
	ldr	r7, .L1214+4
	mov	r1, r4
	mov	r0, r5
	ldr	r3, [r3, #48]
	blx	r3
	ldr	r3, [r7, r6, asl #2]
	cmp	r3, r4
	beq	.L1200
	ldr	r2, [r3, #1176]
	movw	r3, #2082
	str	r2, [r5, #32]
	ldr	r2, [r7, r6, asl #2]
	ldr	r2, [r2, #1180]
	str	r2, [r5, #36]
	ldr	r2, [r7, r6, asl #2]
	ldr	r2, [r2, #1184]
	str	r2, [r5, #44]
	ldr	ip, [r7, r6, asl #2]
	add	r0, ip, #278528
	ldrsb	r1, [r0, r3]
	cmp	r1, r4
	ble	.L1205
	movw	r3, #39862
	mov	r2, r4
	movt	r3, 4
	add	r3, ip, r3
.L1207:
	ldrsb	ip, [r3]
	add	r2, r2, #1
	add	r3, r3, #100
	cmp	ip, #3
	addeq	r4, r4, #1
	cmp	r2, r1
	bne	.L1207
.L1205:
	movw	r3, #2082
	str	r4, [r5, #84]
	ldrsb	ip, [r0, r3]
	sub	r2, fp, #60
	sub	r3, fp, #56
	sub	r1, fp, #64
	mov	r0, r6
	str	ip, [r5, #92]
	bl	VCTRL_GetChanImgNum
	ldr	ip, .L1214+8
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
	add	r3, r3, #327680
	str	r2, [r5, #80]
	ldrb	r3, [r3, #1600]	@ zero_extendqisi2
	and	r3, r3, #1
	str	r3, [r5, #88]
.L1200:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1213:
	ldr	r3, .L1214+12
	movw	r2, #1501
	ldr	r1, .L1214+16
	mov	r0, #0
	bl	dprint_vfmw
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1215:
	.align	2
.L1214:
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
	bhi	.L1220
	ldr	r3, .L1222
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1221
	ldr	r0, [r0, #1276]
	ldmfd	sp, {fp, sp, pc}
.L1220:
	ldr	r3, .L1222+4
	movw	r2, #8099
	ldr	r1, .L1222+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1221:
	ldr	r3, .L1222+12
	movw	r2, #8100
	ldr	r1, .L1222+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1223:
	.align	2
.L1222:
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
	bhi	.L1228
	ldr	r3, .L1230
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1229
	ldr	r0, [r0, #1280]
	ldmfd	sp, {fp, sp, pc}
.L1228:
	ldr	r3, .L1230+4
	movw	r2, #8107
	ldr	r1, .L1230+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1229:
	ldr	r3, .L1230+12
	movw	r2, #8108
	ldr	r1, .L1230+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1231:
	.align	2
.L1230:
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
	bhi	.L1236
	ldr	r3, .L1237
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #44]
	moveq	r0, #33
	ldmfd	sp, {fp, sp, pc}
.L1236:
	ldr	r3, .L1237+4
	movw	r2, #8115
	ldr	r1, .L1237+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1238:
	.align	2
.L1237:
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
	ldr	r5, .L1246
	add	r3, r3, #8
	movw	r2, #10000
	cmp	r3, r2
	mov	r4, r1
	str	r0, [r5, #3808]
	bhi	.L1244
.L1240:
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
	ldr	r6, .L1246
	bhi	.L1245
.L1241:
	movw	r2, #5000
	cmp	r4, r2
	movw	r3, #60536
	movge	r4, r2
	movt	r3, 65535
	cmp	r4, r3
	movlt	r4, r3
	str	r4, [r5, #3812]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1244:
	mov	r2, r0
	ldr	r1, .L1246+4
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r0, [r5, #3808]
	b	.L1240
.L1245:
	mov	r2, r4
	ldr	r1, .L1246+8
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r4, [r6, #3812]
	b	.L1241
.L1247:
	.align	2
.L1246:
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
	ldr	r6, .L1277
	mov	r7, r0
	mov	r0, #0
	ldr	r3, [r6, #12]
	blx	r3
	ldr	r3, [r6, #12]
	mov	r0, #5
	blx	r3
	ldr	r4, [r7]
	cmn	r4, #1
	beq	.L1269
	mov	r0, r4
	bl	VCTRL_IsChanActive
	subs	r5, r0, #0
	bne	.L1270
	mov	r0, r4
	bl	VDMDRV_IsChanDec
	cmp	r0, #1
	beq	.L1271
	mov	r0, r4
	bl	VCTRL_GetDecParam
	subs	r2, r0, #0
	beq	.L1272
	cmp	r4, #15
	bhi	.L1264
	ldr	r3, .L1277+4
	ldr	r3, [r3, r4, asl #2]
	cmp	r3, #0
	beq	.L1264
	ldr	r1, [r3, #600]
	sub	r0, fp, #56
	ldr	r3, .L1277+8
	str	r4, [fp, #-56]
	str	r1, [fp, #-52]
	str	r2, [fp, #-48]
	ldr	r3, [r3, #-3908]
	blx	r3
	cmp	r0, #1
	bne	.L1266
	ldr	ip, [fp, #-56]
	mov	r0, r4
.L1267:
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
.L1259:
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1269:
	ldr	r8, .L1277+12
	ldr	r9, [r8, #64]
	cmp	r9, #0
	blt	.L1250
	add	r4, r8, #64
	ldr	r10, .L1277+8
	add	r8, r8, #124
	b	.L1251
.L1273:
	mov	r3, r9
	ldr	r2, .L1277+16
	ldr	r1, .L1277+20
	mov	r0, #1
	bl	dprint_vfmw
.L1253:
	cmp	r4, r8
	beq	.L1250
.L1260:
	ldr	r9, [r4, #4]!
	cmp	r9, #0
	blt	.L1250
.L1251:
	mov	r0, r9
	bl	VCTRL_IsChanActive
	subs	r5, r0, #0
	bne	.L1273
	mov	r0, r9
	bl	VDMDRV_IsChanDec
	cmp	r0, #1
	beq	.L1274
	mov	r0, r9
	bl	VCTRL_GetDecParam
	subs	r1, r0, #0
	beq	.L1275
	cmp	r9, #15
	bhi	.L1256
	ldr	r3, .L1277+4
	ldr	r2, [r3, r9, asl #2]
	cmp	r2, #0
	beq	.L1256
	ldr	r2, [r2, #600]
	sub	r0, fp, #56
	str	r9, [fp, #-56]
	str	r1, [fp, #-48]
	str	r2, [fp, #-52]
	ldr	r2, [r10, #-3908]
	blx	r2
	cmp	r0, #1
	beq	.L1276
	cmp	r4, r8
	str	r5, [r7, #8]
	bne	.L1260
.L1250:
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
.L1256:
	mov	r3, r9
	movw	r2, #8237
	ldr	r1, .L1277+24
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1253
.L1274:
	mov	r2, r9
	ldr	r1, .L1277+28
	mov	r0, #22
	bl	dprint_vfmw
	b	.L1253
.L1271:
	mov	r2, r4
	ldr	r1, .L1277+28
	mov	r0, #22
	bl	dprint_vfmw
.L1266:
	str	r5, [r7, #8]
	mov	r0, #5
	ldr	r3, [r6, #16]
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, r5
	mvn	r5, #0
	blx	r3
	b	.L1259
.L1275:
	mov	r2, r9
	ldr	r1, .L1277+32
	mov	r0, #22
	bl	dprint_vfmw
	b	.L1253
.L1276:
	ldr	ip, [fp, #-56]
	mov	r0, r9
	b	.L1267
.L1270:
	ldr	r2, .L1277+16
	mov	r3, r4
	ldr	r1, .L1277+36
.L1268:
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
	b	.L1259
.L1264:
	mov	r3, r4
	movw	r2, #8322
	ldr	r1, .L1277+24
	b	.L1268
.L1272:
	ldr	r3, [r6, #124]
	blx	r3
	mov	r2, r4
	ldr	r1, .L1277+32
	mov	r0, #22
	bl	dprint_vfmw
	b	.L1266
.L1278:
	.align	2
.L1277:
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
	bhi	.L1283
	ldr	r3, .L1284
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	ldrne	r0, [r3, #376]
	mvneq	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1283:
	ldr	r3, .L1284+4
	movw	r2, #9281
	ldr	r1, .L1284+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L1285:
	.align	2
.L1284:
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
	bhi	.L1292
	ldr	r3, .L1293
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	strne	r1, [r3, #376]
	ldmfd	sp, {fp, sp, pc}
.L1292:
	ldr	r3, .L1293+4
	movw	r2, #9293
	ldr	r1, .L1293+8
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L1294:
	.align	2
.L1293:
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
	bhi	.L1301
	ldr	r3, .L1302
	ldr	r3, [r3, r0, asl #2]
	cmp	r3, #0
	strne	r1, [r3, #380]
	ldmfd	sp, {fp, sp, pc}
.L1301:
	ldr	r3, .L1302+4
	movw	r2, #9305
	ldr	r1, .L1302+8
	mov	r0, #0
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	dprint_vfmw
.L1303:
	.align	2
.L1302:
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
	beq	.L1306
	cmp	r5, #1
	moveq	r2, #3
	movne	r2, #4
.L1306:
	cmp	r4, #15
	bhi	.L1318
	ldr	r6, .L1319
	ldr	r3, [r6, r4, asl #2]
	cmp	r3, #0
	strne	r2, [r3, #380]
.L1309:
	ldr	r3, .L1319+4
	ldr	ip, [r3, #-792]
	cmp	ip, #0
	beq	.L1310
	sub	r2, fp, #28
	mov	r3, #4
	mov	r1, #115
	mov	r0, r4
	str	r5, [r2, #-4]!
	blx	ip
.L1310:
	ldr	ip, [r6, r4, asl #2]
	mov	r3, r5
	mov	r2, r4
	ldr	r1, .L1319+8
	mov	r0, #1
	ldr	ip, [ip, #380]
	str	ip, [sp]
	bl	dprint_vfmw
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L1318:
	ldr	r3, .L1319+12
	movw	r2, #9305
	ldr	r1, .L1319+16
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r6, .L1319
	b	.L1309
.L1320:
	.align	2
.L1319:
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
	ldr	r3, .L1347
	mov	r5, r0
	ldr	r3, [r3, r0, asl #2]
	add	r0, r3, #311296
	ldr	r2, [r0, #2088]
	cmp	r2, #17
	ldrls	pc, [pc, r2, asl #2]
	b	.L1341
.L1324:
	.word	.L1323
	.word	.L1325
	.word	.L1326
	.word	.L1327
	.word	.L1341
	.word	.L1328
	.word	.L1329
	.word	.L1341
	.word	.L1330
	.word	.L1331
	.word	.L1332
	.word	.L1332
	.word	.L1332
	.word	.L1333
	.word	.L1341
	.word	.L1334
	.word	.L1335
	.word	.L1336
.L1336:
	add	r0, r0, #2096
	bl	VP9DEC_GetRemainImg
	mov	r4, r0
.L1322:
	cmn	r4, #1
	beq	.L1338
	cmp	r4, #0
	bne	.L1346
	mov	r0, r5
	mov	r1, r4
	bl	VCTRL_ReportLastFrame
	ldr	r1, .L1347+4
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1323:
	add	r0, r0, #2096
	bl	H264DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1325:
	add	r0, r0, #2096
	bl	VC1DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1326:
	add	r0, r0, #2096
	bl	MPEG4DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1327:
	add	r0, r0, #2096
	bl	MPEG2DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1328:
	add	r0, r0, #2096
	bl	DIVX3DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1329:
	add	r0, r0, #2096
	bl	AVSDEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1330:
	add	r0, r0, #2096
	bl	REAL8DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1331:
	add	r0, r0, #2096
	bl	REAL9DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1332:
	add	r0, r0, #2096
	bl	VP6DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1333:
	add	r0, r0, #2096
	bl	VP8DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1334:
	add	r0, r0, #2096
	bl	MVC_DEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1335:
	add	r0, r0, #2096
	bl	HEVCDEC_GetRemainImg
	mov	r4, r0
	b	.L1322
.L1346:
	mov	r0, r5
	mov	r1, r4
	bl	VCTRL_ReportLastFrame
	sub	r2, r4, #2
	ldr	r1, .L1347+8
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1341:
	mvn	r4, #0
.L1338:
	mov	r0, r5
	mov	r1, #1
	bl	VCTRL_ReportLastFrame
	ldr	r1, .L1347+12
	mov	r0, #2
	bl	dprint_vfmw
	mov	r0, r4
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1348:
	.align	2
.L1347:
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
	ldr	r5, .L1389
	mov	r4, r0
	ldr	lr, [fp, #4]
	ldr	r0, [r5, r0, asl #2]
	cmp	r0, #0
	beq	.L1382
	ldr	ip, [r0, #600]
	ldr	r6, [r0, #1224]
	cmp	ip, #17
	ldrls	pc, [pc, ip, asl #2]
	b	.L1349
.L1353:
	.word	.L1352
	.word	.L1354
	.word	.L1355
	.word	.L1356
	.word	.L1349
	.word	.L1357
	.word	.L1358
	.word	.L1349
	.word	.L1359
	.word	.L1360
	.word	.L1361
	.word	.L1361
	.word	.L1361
	.word	.L1362
	.word	.L1349
	.word	.L1363
	.word	.L1364
	.word	.L1365
.L1363:
	add	r0, r0, #311296
	mov	r2, #1
	add	r0, r0, #2096
	bl	MVC_DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	ldr	r5, .L1389
	add	r3, r3, #311296
	ldrb	r2, [r3, #2097]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1383
.L1374:
	mov	r2, #0
	str	r2, [r3, #2324]
.L1349:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1365:
	add	r0, r0, #311296
	mov	r3, lr
	add	r0, r0, #2096
	mov	r2, r6
	bl	VP9DEC_VDMPostProc
	cmp	r6, #1
	bne	.L1381
	b	.L1349
.L1352:
	add	r0, r0, #311296
	str	lr, [sp, #4]
	add	r0, r0, #2096
	str	r6, [sp]
	mov	r2, #1
	ldr	r7, .L1389
	bl	H264DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #323584
	ldrb	r3, [r3, #3368]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1384
.L1366:
	cmp	r6, #1
	ldrne	r3, [r5, r4, asl #2]
	movne	r2, #0
	addne	r3, r3, #311296
	strne	r2, [r3, #2456]
	b	.L1349
.L1355:
	add	r0, r0, #311296
	mov	r2, r3
	add	r0, r0, #2096
	bl	MPEG4DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	ldr	r5, .L1389
	add	r2, r3, #335872
	ldrb	r2, [r2, #540]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1385
.L1371:
	add	r3, r3, #311296
	mov	r2, #0
	str	r2, [r3, #3132]
	b	.L1349
.L1356:
	add	r0, r0, #311296
	ldr	r6, .L1389
	add	r0, r0, #2096
	bl	MPEG2DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r2, r3, #401408
	ldrb	r2, [r2, #66]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1386
.L1373:
	add	r3, r3, #311296
	mov	r2, #0
	str	r2, [r3, #2100]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1354:
	add	r0, r0, #311296
	ldr	r6, .L1389
	add	r0, r0, #2096
	bl	VC1DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r2, r3, #323584
	ldrb	r2, [r2, #2543]	@ zero_extendqisi2
	cmp	r2, #1
	bne	.L1373
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #323584
	strb	r1, [r2, #2543]
	b	.L1373
.L1357:
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	DIVX3DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #311296
	str	r2, [r3, #2100]
	b	.L1349
.L1358:
	add	r0, r0, #311296
	ldr	r6, .L1389
	add	r0, r0, #2096
	bl	AVSDEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #311296
	ldrb	r2, [r3, #2096]	@ zero_extendqisi2
	cmp	r2, #1
	beq	.L1387
.L1372:
	mov	r2, #0
	str	r2, [r3, #2112]
	b	.L1349
.L1359:
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	REAL8DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #311296
	str	r2, [r3, #2100]
	b	.L1349
.L1360:
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	REAL9DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #311296
	str	r2, [r3, #2100]
	b	.L1349
.L1361:
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	VP6DEC_VDMPostProc
.L1381:
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #311296
	str	r2, [r3, #2100]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1364:
	add	r0, r0, #311296
	str	lr, [sp, #4]
	add	r0, r0, #2096
	str	r6, [sp]
	mov	r2, #1
	ldr	r7, .L1389
	bl	HEVCDEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	add	r3, r3, #311296
	ldrb	r3, [r3, #2103]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1388
.L1368:
	cmp	r6, #1
	ldrne	r3, [r5, r4, asl #2]
	movne	r2, #0
	addne	r3, r3, #315392
	strne	r2, [r3, #360]
	b	.L1349
.L1362:
	add	r0, r0, #311296
	add	r0, r0, #2096
	bl	VP8DEC_VDMPostProc
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #311296
	str	r2, [r3, #2100]
	b	.L1349
.L1382:
	ldr	r3, .L1389+4
	mov	r2, #5184
	ldr	r1, .L1389+8
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1387:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r2, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r2, #311296
	mov	r3, r2
	strb	r1, [r2, #2096]
	b	.L1372
.L1386:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r6, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #401408
	strb	r1, [r2, #66]
	b	.L1373
.L1385:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r5, r4, asl #2]
	mov	r1, #0
	add	r2, r3, #335872
	strb	r1, [r2, #540]
	b	.L1371
.L1388:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #311296
	strb	r2, [r3, #2103]
	b	.L1368
.L1383:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r2, [r5, r4, asl #2]
	mov	r1, #0
	add	r2, r2, #311296
	mov	r3, r2
	strb	r1, [r2, #2097]
	b	.L1374
.L1384:
	mov	r0, r4
	bl	VCTRL_OutputLastFrame
	ldr	r3, [r7, r4, asl #2]
	mov	r2, #0
	add	r3, r3, #323584
	strb	r2, [r3, #3368]
	b	.L1366
.L1390:
	.align	2
.L1389:
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
	ldrhi	r3, .L1398
	movwhi	r2, #5374
	bhi	.L1396
	ldr	ip, .L1398+4
	ldr	ip, [ip, r0, asl #2]
	cmp	ip, #0
	beq	.L1397
	ldr	ip, [ip, #16]
	cmp	ip, #1
	beq	.L1394
	ldr	r3, .L1398+8
	mov	r2, #5376
.L1396:
	ldr	r1, .L1398+12
	mov	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L1394:
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
.L1397:
	mov	r0, ip
	ldr	r3, .L1398+16
	movw	r2, #5375
	ldr	r1, .L1398+12
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	dprint_vfmw
.L1399:
	.align	2
.L1398:
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
	ldr	r4, .L1401
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
.L1402:
	.align	2
.L1401:
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
	b	.L1405
.L1407:
	.word	.L1406
	.word	.L1408
	.word	.L1409
	.word	.L1410
	.word	.L1411
	.word	.L1412
	.word	.L1413
.L1413:
	cmp	r0, #1
	moveq	r0, #1
	movne	r0, #7
	ldmfd	sp, {fp, sp, pc}
.L1406:
	cmp	r0, #1
	moveq	r0, #0
	movne	r0, #6
	ldmfd	sp, {fp, sp, pc}
.L1408:
	cmp	r0, #1
	moveq	r0, #2
	movne	r0, #8
	ldmfd	sp, {fp, sp, pc}
.L1409:
	cmp	r0, #1
	moveq	r0, #4
	movne	r0, #10
	ldmfd	sp, {fp, sp, pc}
.L1410:
	cmp	r0, #1
	moveq	r0, #3
	movne	r0, #9
	ldmfd	sp, {fp, sp, pc}
.L1411:
	cmp	r0, #1
	moveq	r0, #5
	movne	r0, #11
	ldmfd	sp, {fp, sp, pc}
.L1412:
	cmp	r0, #1
	moveq	r0, #13
	movne	r0, #12
	ldmfd	sp, {fp, sp, pc}
.L1405:
	mov	r3, r1
	movw	r2, #9829
	ldr	r1, .L1422
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, #13
	ldmfd	sp, {fp, sp, pc}
.L1423:
	.align	2
.L1422:
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
	beq	.L1432
	ldr	r3, [r5, #4]
	cmp	r3, #1
	beq	.L1427
	mov	r1, r0
	mov	r2, #0
	mov	r0, r8
	bl	FSP_ClearLogicFs
	mov	r0, #0
.L1426:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1427:
	ldr	r0, [r5, #48]
	bl	MEM_Phy2Vir
	cmp	r0, #0
	beq	.L1433
.L1428:
	mov	r1, r7
	mov	r0, r8
	bl	FSP_GetFsImagePtr
	subs	r4, r0, #0
	beq	.L1434
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
	beq	.L1435
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
	ldr	r2, .L1436
	str	r4, [sp]
	movw	r3, #51256
	mov	r1, r9
	movt	r3, 4
	ldr	ip, [r2, r8, asl #2]
	mov	r0, r8
	add	r2, ip, #311296
	add	r3, ip, r3
	add	r2, r2, #2096
	bl	InsertImgToVoQueue
	cmp	r0, #1
	moveq	r0, r6
	beq	.L1426
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
	b	.L1426
.L1433:
	ldr	r3, [r5, #4]
	cmp	r3, #1
	bne	.L1428
	ldr	r2, [r5, #48]
	ldr	r1, .L1436+4
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1426
.L1432:
	ldr	r2, [r5, #40]
	mov	r0, #0
	ldr	r1, .L1436+8
	bl	dprint_vfmw
	mov	r0, r7
	b	.L1426
.L1435:
	mov	r2, r0
	ldr	r1, .L1436+12
	mov	r0, r10
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1426
.L1434:
	movw	r2, #5864
	ldr	r1, .L1436+16
	mov	r0, #22
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1426
.L1437:
	.align	2
.L1436:
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
	ldr	r3, .L1449
	mov	r4, r0
	mov	r5, r1
	ldr	r0, [r3, r0, asl #2]
	cmp	r0, #0
	beq	.L1446
	cmp	r1, #0
	beq	.L1447
	ldr	r2, [r0, #600]
	cmp	r2, #20
	bne	.L1448
	ldr	ip, .L1449+4
	mov	r2, #72
	add	r0, r0, #528
	ldr	r3, [ip, #52]
	blx	r3
	mov	r1, r5
	mov	r0, r4
	bl	VDH_PutUsrdecFrame
	cmp	r0, #0
	ldmeqfd	sp, {r4, r5, fp, sp, pc}
	ldr	r1, .L1449+8
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1448:
	ldr	r1, .L1449+12
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1447:
	mov	r0, r1
	ldr	r3, .L1449+16
	movw	r2, #6077
	ldr	r1, .L1449+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1446:
	ldr	r3, .L1449+24
	movw	r2, #6076
	ldr	r1, .L1449+20
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L1450:
	.align	2
.L1449:
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
	ldrhi	r1, .L1500
	bhi	.L1492
	ldr	r6, .L1500+4
	ldr	r0, [r6, r0, asl #2]
	cmp	r0, #0
	beq	.L1493
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L1455
	ldr	r3, [r0, #600]
	cmp	r3, #21
	beq	.L1455
	bic	r7, r3, #16
	cmp	r7, #4
	cmpne	r3, #7
	moveq	r7, #1
	movne	r7, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	cmp	r3, #14
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r4, r7
.L1461:
	mov	r0, r4
	bl	VCTRL_IsChanActive
	cmp	r0, #0
	bne	.L1460
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
.L1460:
	add	r4, r4, #1
	cmp	r4, #16
	bne	.L1461
	cmp	r7, #1
	beq	.L1494
.L1462:
	ldr	r1, .L1500+8
	mov	ip, #0
	add	lr, r1, #64
.L1470:
	ldr	r2, [r1, #4]!
	mov	r3, #0
	cmp	r2, r3
	beq	.L1468
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
.L1468:
	cmp	r1, lr
	bne	.L1470
	cmp	ip, #1
	beq	.L1495
.L1471:
	ldr	r3, [r6, r5, asl #2]
	ldr	r3, [r3, #600]
	cmp	r3, #1
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}
	ldr	r3, .L1500+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1496
	ldr	r3, .L1500+16
	ldr	r3, [r3, #28]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L1455:
	ldr	r1, .L1500+20
	movw	r2, #9875
.L1492:
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1496:
	mov	r1, #0
	mov	r0, #2
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VDH_Need_Module
.L1495:
	ldr	r3, .L1500+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1472
	ldr	r3, .L1500+16
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L1497
	mov	r0, #0
	blx	r3
	b	.L1471
.L1494:
	ldr	r4, .L1500+12
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1463
	ldr	r3, .L1500+16
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1498
	mov	r0, #0
	blx	r3
.L1465:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1466
	ldr	r3, .L1500+16
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq	.L1499
	mov	r0, #0
	blx	r3
	b	.L1462
.L1493:
	movw	r2, #9869
	ldr	r1, .L1500+24
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	dprint_vfmw
.L1466:
	mov	r1, #0
	mov	r0, r1
	bl	VDH_Need_Module
	b	.L1462
.L1463:
	mov	r0, r7
	mov	r1, #0
	bl	VDH_Need_Module
	b	.L1465
.L1472:
	mov	r1, #0
	mov	r0, #3
	bl	VDH_Need_Module
	b	.L1471
.L1497:
	mov	r0, r3
	movw	r2, #9966
	ldr	r1, .L1500+28
	bl	dprint_vfmw
	b	.L1471
.L1499:
	mov	r0, r3
	movw	r2, #9931
	ldr	r1, .L1500+28
	bl	dprint_vfmw
	b	.L1462
.L1498:
	mov	r0, r3
	movw	r2, #9917
	ldr	r1, .L1500+28
	bl	dprint_vfmw
	b	.L1465
.L1501:
	.align	2
.L1500:
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
	bhi	.L1513
	ldr	r4, .L1517
	ldr	r3, [r4, r0, asl #2]
	cmp	r3, #0
	beq	.L1514
	ldr	r6, [r3, #12]
	cmp	r6, #1
	bne	.L1515
	ldr	r3, [r3, #600]
	cmp	r3, #21
	beq	.L1516
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
	bne	.L1508
	ldr	r3, .L1517+4
	sub	ip, r4, #4
	ldr	r1, .L1517+8
	add	r0, r3, #64
	str	r6, [r2, #28]
.L1511:
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
	bne	.L1511
	ldr	r0, .L1517+4
	bl	VDMDRV_SetPriority
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
.L1508:
	mov	r0, r5
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r5
	bl	VCTRL_OpenHardware
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1515:
	ldr	r3, .L1517+12
	movw	r2, #2893
	ldr	r1, .L1517+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1513:
	ldr	r3, .L1517+20
	movw	r2, #2891
	ldr	r1, .L1517+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1516:
	ldr	r3, .L1517+24
	movw	r2, #2894
	ldr	r1, .L1517+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1514:
	mov	r0, r3
	movw	r2, #2892
	ldr	r3, .L1517+28
	ldr	r1, .L1517+16
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1518:
	.align	2
.L1517:
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
	ldr	ip, .L1577
	mov	r5, #0
	mov	r8, r0
	mov	r4, r5
	add	r6, ip, #64
	mov	r7, r5
	b	.L1525
.L1566:
	ldr	r1, [r3, #12]
	cmp	r1, #1
	beq	.L1564
.L1522:
	cmp	r2, #0
	ldreq	lr, [r3, #1228]
	movne	lr, #0
	cmp	r5, #0
	bne	.L1520
	cmp	r2, #6
	cmpne	lr, #1
	bne	.L1520
	ldr	r2, [r3, #12]
	cmp	r2, #1
	ldreq	r5, [r3, #16]
	subeq	r5, r5, #1
	clzeq	r5, r5
	moveq	r5, r5, lsr #5
.L1520:
	cmp	ip, r6
	beq	.L1565
.L1525:
	ldr	r3, [ip, #4]!
	cmp	r3, #0
	beq	.L1520
	cmp	r7, #0
	ldr	r2, [r3, #600]
	beq	.L1566
	cmp	r4, #0
	cmpeq	r2, #1
	bne	.L1522
	ldr	r1, [r3, #12]
	cmp	r1, #1
	bne	.L1522
	ldr	r1, [r3, #16]
.L1543:
	cmp	r1, #1
	moveq	r4, #1
	b	.L1522
.L1564:
	ldr	r1, [r3, #16]
	cmp	r1, #1
	beq	.L1567
.L1523:
	cmp	r4, #0
	cmpeq	r2, #1
	beq	.L1543
	b	.L1522
.L1565:
	cmp	r7, #0
	beq	.L1568
.L1526:
	cmp	r5, #0
	bne	.L1538
	ldr	r3, .L1577+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1569
	mov	r1, r5
	mov	r0, #3
	bl	VDH_UnNeed_Module
.L1538:
	cmp	r4, #0
	ldmnefd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1576:
	ldr	r3, .L1577+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1570
	ldr	r3, .L1577+8
	ldr	r3, [r3, #32]
	cmp	r3, #0
	ldmeqfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
	mov	r0, r4
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	bx	r3	@ indirect register sibling call
.L1568:
	ldr	r6, .L1577+4
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1531
	mov	r0, r8
	bl	SCDDRV_ResetSCD
	mov	r0, r8
	bl	VDMDRV_ResetVDH
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1531
	mov	r1, r7
	mov	r0, #1
	bl	VDH_UnNeed_Module
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L1534
.L1574:
	ldr	r3, .L1577+8
	ldr	r3, [r3, #24]
	cmp	r3, #0
	beq	.L1571
	mov	r0, #0
	blx	r3
.L1537:
	mov	r0, #0
	bl	SCDDRV_SetStateIdle
	b	.L1526
.L1567:
	sub	r0, r2, #5
	cmp	r0, #1
	cmphi	r2, #3
	bhi	.L1572
.L1545:
	mov	r7, r1
	b	.L1523
.L1572:
	sub	r0, r2, #8
	cmp	r0, #5
	bls	.L1545
	sub	r7, r2, #15
	cmp	r7, #2
	movhi	r7, #0
	movls	r7, #1
	b	.L1523
.L1570:
	mov	r1, r4
	mov	r0, #2
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, lr}
	b	VDH_UnNeed_Module
.L1531:
	ldr	r3, .L1577+8
	ldr	r3, [r3, #8]
	cmp	r3, #0
	beq	.L1573
	mov	r0, #0
	blx	r3
.L1533:
	mov	r0, #0
	bl	VDMDRV_SetStateIdle
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L1574
.L1534:
	mov	r1, #0
	mov	r0, r1
	bl	VDH_UnNeed_Module
	b	.L1526
.L1569:
	ldr	r3, .L1577+8
	mov	r0, r5
	ldr	r3, [r3, #16]
	cmp	r3, #0
	beq	.L1575
	blx	r3
	cmp	r4, #0
	beq	.L1576
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1575:
	movw	r2, #10108
	ldr	r1, .L1577+12
	bl	dprint_vfmw
	b	.L1538
.L1571:
	mov	r0, r3
	movw	r2, #10090
	ldr	r1, .L1577+12
	bl	dprint_vfmw
	b	.L1537
.L1573:
	mov	r0, r3
	movw	r2, #10075
	ldr	r1, .L1577+12
	bl	dprint_vfmw
	b	.L1533
.L1578:
	.align	2
.L1577:
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
	bhi	.L1585
	ldr	r6, .L1587
	ldr	r3, [r6, r0, asl #2]
	cmp	r3, #0
	beq	.L1586
	ldr	r2, [r3, #12]
	cmp	r2, #1
	bne	.L1584
	str	r2, [r3, #32]
	mov	r3, #0
	ldr	r2, [r6, r0, asl #2]
	str	r3, [r2, #20]
	ldr	r2, [r6, r0, asl #2]
	str	r3, [r2, #16]
	bl	SM_Stop
	subs	r5, r0, #0
	bne	.L1584
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
.L1584:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1585:
	ldr	r3, .L1587+4
	movw	r2, #3000
	ldr	r1, .L1587+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1586:
	mov	r0, r3
	movw	r2, #3001
	ldr	r3, .L1587+12
	ldr	r1, .L1587+8
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1588:
	.align	2
.L1587:
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
	ldr	r5, .L1596
	mov	r7, r0
	mov	r4, #0
	mov	r8, r5
	b	.L1591
.L1595:
	add	r4, r4, #1
	ldr	r3, [r8, #116]
	blx	r3
	cmp	r4, #50
	beq	.L1593
.L1591:
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
	bne	.L1595
	cmp	r4, #49
	bgt	.L1593
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1593:
	ldr	r1, .L1596+4
	mvn	r6, #0
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, r6
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1597:
	.align	2
.L1596:
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
	ldr	r5, .L1625
	mov	r4, r0
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L1621
	ldr	r8, [r3, #16]
	bl	VCTRL_StopChan
	ldr	r7, .L1625+4
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
	beq	.L1622
.L1601:
	ldr	r0, .L1625+8
	mov	r2, #300
	ldr	r3, [r7, #48]
	mov	r1, #0
	mla	r0, r2, r4, r0
	blx	r3
	ldr	r3, [r5, r4, asl #2]
	cmp	r3, #0
	addne	r3, r3, #307200
	movne	r2, #0
	strne	r2, [r3, #1996]
	beq	.L1623
.L1603:
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
	add	r0, r1, #311296
	add	ip, r1, #672
	strd	r2, [ip, #-8]
	strd	r2, [ip]
	str	r6, [r1, #1160]
	str	r6, [r0, #2072]
	str	r6, [r0, #2064]
	str	r6, [r0, #2068]
	ldr	r3, [r1, #1440]
	cmp	r3, #1
	streq	r6, [r1, #1480]
	streq	r6, [r0, #2044]
	mov	r0, r4
	bl	FSP_SwitchNotify
	ldr	r3, [r5, r4, asl #2]
	mov	r0, r4
	ldr	r3, [r3, #600]
	cmp	r3, #20
	beq	.L1605
	bl	FSP_ResetInstance
.L1606:
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	mov	r0, r4
	bl	InitUsdPool
	mov	r0, r4
	bl	SM_Reset
	ldr	r3, .L1625+12
	ldr	r2, [r3, #-3840]
	cmp	r2, #0
	beq	.L1607
	add	r5, r3, r4, lsl #2
	str	r4, [fp, #-40]
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1609
	mov	r0, #512
	sub	r1, fp, #40
	bl	VCTRL_SetDbgOption
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1609
	sub	r1, fp, #40
	mov	r0, #512
	bl	VCTRL_SetDbgOption
.L1609:
	ldr	r1, .L1625+16
	mov	r0, #32
	bl	dprint_vfmw
.L1607:
	cmp	r8, #0
	bne	.L1624
.L1611:
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	mov	r0, #0
.L1600:
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L1624:
	mov	r0, r4
	bl	VCTRL_StartChan
	b	.L1611
.L1605:
	bl	FSP_EmptyInstance
	b	.L1606
.L1622:
	add	r2, r3, #311296
	ldr	r1, [r3, #600]
	ldr	r2, [r2, #2088]
	cmp	r2, r1
	bne	.L1601
	str	r6, [r3, #20]
	mov	r0, r4
	bl	VCTRL_FindOccupiedFrame
	b	.L1601
.L1623:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1603
.L1621:
	mov	r0, r3
	movw	r2, #4447
	ldr	r3, .L1625+20
	ldr	r1, .L1625+24
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1600
.L1626:
	.align	2
.L1625:
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
	ldr	r7, .L1705
	mov	r8, r1
	mov	r4, r0
	mov	r1, #0
	mov	r5, r2
	ldr	r3, [r7, #48]
	mov	r2, #24
	sub	r0, fp, #204
	ldr	r10, .L1705+4
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
	blt	.L1628
	ldr	r1, [r8, #12]
	cmp	r1, #31
	ble	.L1628
	ldr	r3, [r8, #16]
	cmp	r3, #31
	ble	.L1628
	ldr	r3, [r8, #56]
	cmp	r3, #262144
	blt	.L1628
	ldr	r1, [r8, #44]
	cmp	r1, #1
	beq	.L1688
	cmp	r1, #2
	beq	.L1689
.L1630:
	ldr	r3, [r8, #176]
	movw	ip, #27120
	movt	ip, 12
	movw	r9, #13848
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
	movt	r9, 178
	movne	r9, ip
	bl	VCTRL_GetChanMemSizeWithOption
	subs	r6, r0, #0
	bne	.L1690
	ldrd	r2, [r8, #136]
	orrs	r3, r2, r3
	beq	.L1636
	ldr	r3, [r8, #144]
	cmp	r3, #0
	bne	.L1637
.L1636:
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
	bne	.L1691
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
.L1639:
	ldrd	r2, [r8, #88]
	orrs	r3, r2, r3
	bne	.L1692
.L1641:
	ldr	r3, [r8, #176]
	cmp	r3, #1
	beq	.L1643
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
	bne	.L1693
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
.L1646:
	ldrd	r2, [r8, #112]
	orrs	r3, r2, r3
	beq	.L1650
	ldr	r3, [r8, #120]
	cmp	r3, #0
	bne	.L1651
.L1650:
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
	bne	.L1694
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
.L1655:
	ldr	r6, .L1705+8
	mov	r0, #0
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L1669
	mov	r3, r6
	mov	r5, #1
.L1662:
	ldr	r2, [r3, #4]!
	cmp	r2, #0
	beq	.L1695
	add	r5, r5, #1
	cmp	r5, #16
	bne	.L1662
	mov	r2, r5
	ldr	r1, .L1705+12
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r7, #16]
	mov	r0, #0
	blx	r3
	mvn	r0, #0
.L1684:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1692:
	ldr	r3, [r8, #96]
	cmp	r3, #0
	beq	.L1641
	ldr	r3, [r7, #52]
	mov	r2, #24
	sub	r0, fp, #204
	add	r1, r8, #80
	blx	r3
	ldr	r3, [r8, #176]
	ldr	r2, [fp, #-188]
	cmp	r3, #1
	beq	.L1648
	ldr	r3, [fp, #-220]
	cmp	r2, r3
	blt	.L1696
.L1648:
	ldr	r1, [fp, #-196]
	mov	r3, #0
	ldr	r0, [fp, #-204]
	str	r3, [fp, #-228]
	bl	MEM_AddMemRecord
	b	.L1646
.L1643:
	mov	r3, #0
	mov	r0, #0
	mov	r1, #0
	str	r3, [fp, #-204]
	str	r3, [fp, #-228]
	str	r3, [fp, #-188]
	strd	r0, [fp, #-196]
	b	.L1646
.L1689:
	ldr	r3, [r8, #20]
	cmp	r3, #0
	ble	.L1628
	ldr	r3, [r8, #24]
	cmp	r3, #0
	ble	.L1628
.L1686:
	ldr	r3, [r8, #28]
	cmp	r3, #0
	ble	.L1628
	ldr	r3, [r8, #32]
	cmp	r3, #0
	bgt	.L1630
.L1628:
	ldr	r1, .L1705+16
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1695:
	uxtb	r3, r5
	str	r3, [fp, #-236]
.L1660:
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
	ldr	r0, .L1705+20
	mov	ip, #30
	str	r4, [r1, #44]
	ldr	r2, [r6, r5, asl #2]
	str	r9, [r1, #1188]
	movw	r1, #18456
	movt	r1, 4
	str	r5, [r2, #8]
	str	r3, [r2, #1208]
	str	r4, [r2, #636]
	ldr	r2, [r6, r5, asl #2]
	ldr	r4, [r8]
	str	r4, [r2, #40]
	ldr	r2, [r6, r5, asl #2]
	add	r4, r2, #311296
	str	lr, [r2, #600]
	str	lr, [r4, #2088]
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
	ldr	ip, .L1705+24
	str	lr, [r2, #1588]
	str	r1, [ip, r5, asl #3]
	bl	VCTRL_ResetChan
	ldr	r1, [r6, r5, asl #2]
	ldr	r3, [fp, #-228]
	add	r2, r1, #307200
	add	r0, r1, #311296
	str	r3, [r1, #292]
	ldr	r3, [fp, #-232]
	str	r3, [r1, #296]
	ldr	r3, [fp, #-224]
	str	r3, [r1, #300]
	ldr	r3, [fp, #-240]
	str	r3, [r2, #1972]
	str	r3, [r2, #1976]
	str	r3, [r2, #1984]
	str	r3, [r2, #1988]
	str	r3, [r2, #1980]
	str	r3, [r2, #1992]
	ldr	r2, [r8, #176]
	str	r3, [r0, #2044]
	str	r2, [r0, #2052]
.L1664:
	mov	r8, r4, asl #6
	ldr	r0, [r6, r5, asl #2]
	add	r8, r8, r4, lsl #3
	ldr	ip, [r7, #48]
	add	r3, r8, #307200
	mov	r2, #72
	add	r3, r3, #2000
	mov	r1, #0
	add	r0, r0, r3
	add	r4, r4, #1
	blx	ip
	ldr	r0, [r6, r5, asl #2]
	movw	r2, #47064
	cmp	r4, #32
	add	r8, r0, r8
	movt	r2, 4
	add	ip, r8, #307200
	add	r2, r8, r2
	mov	r1, #255
	ldr	r3, .L1705
	str	r1, [ip, #2000]
	str	r1, [ip, #2004]
	ldr	r8, .L1705+8
	str	r1, [r2]
	bne	.L1664
	add	r0, r0, #311296
	ldr	ip, [r3, #48]
	movw	r2, #1288
	mov	r1, #0
	add	r0, r0, #208
	str	r3, [fp, #-224]
	blx	ip
	ldr	r3, [fp, #-224]
	ldr	r1, [r8, r5, asl #2]
	movw	r0, #50648
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
	add	r1, r3, #307200
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
	str	r2, [r1, #1952]
	ldr	r1, [r3, #1440]
	str	r2, [r10, #-3704]
	cmp	r1, #1
	beq	.L1665
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
.L1666:
	ldr	r3, [r6, r5, asl #2]
	mov	r0, r5
	ldr	r4, .L1705+8
	str	r5, [r3, #8]
	ldr	r3, [r6, r5, asl #2]
	add	r1, r3, #278528
	add	r2, r3, #262144
	str	r5, [r3, #208]
	str	r5, [r3, #212]
	ldrb	r3, [fp, #-236]	@ zero_extendqisi2
	strb	r3, [r1, #2072]
	str	r5, [r2, #1724]
	bl	FSP_EnableInstance
	subs	r6, r0, #0
	bne	.L1697
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
	add	r4, r4, #311296
	str	r0, [r4, #2060]
	mov	r0, r6
	blx	r3
	mov	r0, r5
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1688:
	ldr	r3, [r8, #20]
	cmp	r3, #0
	bgt	.L1686
	b	.L1628
.L1637:
	ldr	r3, [r7, #52]
	mov	r2, #24
	sub	r0, fp, #156
	add	r1, r8, #128
	blx	r3
	ldr	r2, [fp, #-140]
	ldr	r3, [fp, #-212]
	cmp	r2, r3
	blt	.L1698
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	str	r6, [fp, #-224]
	bl	MEM_AddMemRecord
	b	.L1639
.L1665:
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
	b	.L1666
.L1651:
	ldr	r3, [r7, #52]
	add	r1, r8, #104
	sub	r0, fp, #180
	mov	r2, #24
	blx	r3
	ldrd	r0, [fp, #-172]
	orrs	r3, r0, r1
	beq	.L1656
	ldr	r2, [fp, #-164]
	ldr	r3, [fp, #-216]
	cmp	r2, r3
	blt	.L1656
	mov	r1, r0
	ldr	r0, [fp, #-180]
	mov	r3, #0
	str	r3, [fp, #-232]
	bl	MEM_AddMemRecord
	b	.L1655
.L1694:
	ldr	r3, [fp, #-224]
	cmp	r3, #1
	beq	.L1699
.L1653:
	ldr	r3, [fp, #-228]
	cmp	r3, #1
	beq	.L1700
.L1654:
	movw	r2, #2363
	ldr	r1, .L1705+28
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1684
.L1700:
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-188]
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_DelMemRecord
	b	.L1654
.L1693:
	ldr	r3, [fp, #-224]
	cmp	r3, #1
	beq	.L1701
.L1645:
	movw	r2, #2305
	ldr	r1, .L1705+32
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1684
.L1669:
	str	r3, [fp, #-236]
	mov	r5, r3
	b	.L1660
.L1691:
	mov	r0, r5
	movw	r2, #2267
	ldr	r1, .L1705+36
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1684
.L1697:
	ldr	r1, .L1705+40
	mov	r0, #1
	bl	dprint_vfmw
	mov	r0, r5
	bl	FSP_DisableInstance
	mov	r0, r5
	bl	CloseUsdPool_WithMem
	ldr	r3, [r4, r5, asl #2]
	mov	r6, #0
	mov	r0, r5
	add	r1, r3, #311296
	add	r2, r3, #307200
	ldr	lr, [r3, #296]
	add	ip, r3, #220
	ldr	r9, [r3, #300]
	add	r2, r2, #2000
	ldr	r8, [r3, #292]
	ldr	r10, [r1, #2052]
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
	b	.L1684
.L1656:
	ldr	r3, [fp, #-224]
	cmp	r3, #1
	beq	.L1702
.L1658:
	ldr	r3, [fp, #-228]
	cmp	r3, #1
	beq	.L1703
.L1659:
	movw	r2, #2392
	ldr	r1, .L1705+44
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1684
.L1690:
	movw	r2, #2257
	ldr	r1, .L1705+48
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1684
.L1699:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	b	.L1653
.L1701:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	b	.L1645
.L1698:
	mov	r0, r5
	movw	r2, #2283
	ldr	r1, .L1705+52
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1684
.L1696:
	ldr	r1, [fp, #-224]
	cmp	r1, #1
	beq	.L1704
.L1649:
	str	r3, [sp]
	mov	r0, #1
	mov	r3, r2
	ldr	r1, .L1705+56
	movw	r2, #2337
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1684
.L1703:
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-188]
	ldr	r1, [fp, #-196]
	ldr	r0, [fp, #-204]
	bl	MEM_DelMemRecord
	b	.L1659
.L1702:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	b	.L1658
.L1704:
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [fp, #-140]
	ldr	r1, [fp, #-148]
	ldr	r0, [fp, #-156]
	bl	MEM_DelMemRecord
	ldr	r2, [fp, #-188]
	ldr	r3, [fp, #-220]
	b	.L1649
.L1706:
	.align	2
.L1705:
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
	bhi	.L1731
	ldr	r5, .L1734
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
	ldr	r2, .L1734+4
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
	b	.L1710
.L1712:
	.word	.L1722
	.word	.L1722
	.word	.L1722
	.word	.L1722
	.word	.L1722
	.word	.L1713
	.word	.L1713
	.word	.L1713
.L1722:
	mov	lr, #5
	mov	ip, #3
.L1711:
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
	beq	.L1717
	cmp	r4, #26
	beq	.L1732
	cmp	r4, #18
	moveq	r2, #4
	moveq	r3, #2
	streq	r2, [fp, #-208]
	streq	r3, [fp, #-184]
.L1717:
	sub	r2, fp, #304
	sub	r1, fp, #316
	mov	r0, r4
	bl	VCTRL_GetChanMemSize
	cmp	r0, #0
	bne	.L1733
	cmp	r6, #0
	beq	.L1723
	ldr	r3, [r6, #16]
	cmp	r3, #0
	beq	.L1720
	ldr	r2, [r6]
	cmp	r2, #0
	beq	.L1721
	ldrd	r8, [r6, #8]
	orrs	r1, r8, r9
	beq	.L1721
	ldr	r1, [fp, #-316]
	ldr	r0, [fp, #-304]
	add	r1, r1, r0
	cmp	r3, r1
	blt	.L1721
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
.L1720:
	sub	r1, fp, #244
	mov	r0, r4
	mov	r2, #1
	bl	VCTRL_CreateChanWithOption
	orr	r0, r0, r0, asr #31
.L1709:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1713:
	mov	lr, #3
	mov	ip, #1
	b	.L1711
.L1710:
	mov	lr, #8
	mov	ip, #6
	b	.L1711
.L1732:
	mov	r2, #16
	mov	r3, #6
	str	r2, [fp, #-208]
	str	r3, [fp, #-184]
	b	.L1717
.L1723:
	mov	r3, r6
	b	.L1720
.L1721:
	ldr	r1, .L1734+8
	mov	r0, #1
	bl	dprint_vfmw
	ldr	r3, [r6, #16]
	ldrd	r4, [r6, #8]
	mov	r0, #1
	ldr	r2, [r6]
	ldr	r1, .L1734+12
	str	r3, [sp, #8]
	strd	r4, [sp]
	bl	dprint_vfmw
	ldr	r2, [fp, #-316]
	ldr	r3, [fp, #-304]
	mov	r0, #1
	ldr	r1, .L1734+16
	add	r2, r2, r3
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1709
.L1731:
	mov	r2, r0
	ldr	r1, .L1734+20
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1709
.L1733:
	ldr	r1, .L1734+24
	mov	r0, #1
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1709
.L1735:
	.align	2
.L1734:
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
	@ args = 0, pretend = 0, frame = 88
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
	cmp	r0, #15
	mov	r4, #0
	mov	r6, r0
	mov	r9, r1
	str	r4, [fp, #-116]
	bhi	.L1782
	ldr	r7, .L1790
	ldr	r2, [r7, r0, asl #2]
	cmp	r2, #0
	beq	.L1783
	ldr	r2, [r2, #12]
	cmp	r2, #1
	bne	.L1784
	bl	VCTRL_CheckChanWH
	subs	r8, r0, #0
	bne	.L1785
	ldr	r4, .L1790+4
	ldr	r0, .L1790+8
	bl	PDT_GetVdecCapability
	ldr	r2, [r9]
	ldr	r1, [r4, #-3620]
	cmp	r1, #20
	bhi	.L1743
	cmp	r1, r2
	beq	.L1744
	ldr	r1, .L1790+12
	add	ip, r1, #124
.L1746:
	ldr	r3, [r1, #4]!
	cmp	r3, #20
	bhi	.L1743
	cmp	r3, r2
	beq	.L1744
	cmp	r1, ip
	bne	.L1746
.L1744:
	cmp	r2, #8
	beq	.L1786
.L1747:
	cmp	r2, #9
	beq	.L1749
.L1753:
	mov	r1, r9
	mov	r0, r6
	bl	VCTRL_CmpConfigParameter
	subs	r2, r0, #0
	beq	.L1787
	ldr	r10, .L1790+16
	mov	r2, #120
	ldr	r0, [r7, r6, asl #2]
	mov	r1, r9
	ldr	ip, [r10, #52]
	add	r0, r0, #52
	blx	ip
	ldr	lr, [r9, #16]
	ldr	ip, [r7, r6, asl #2]
	mov	r1, #0
	mov	r2, #68
	sub	r0, fp, #112
	str	lr, [ip, #28]
	ldr	ip, [r7, r6, asl #2]
	ldr	lr, [r9]
	ldr	r4, [r10, #48]
	str	lr, [ip, #600]
	ldr	lr, [r9, #104]
	str	lr, [ip, #1448]
	blx	r4
	ldr	r2, [r7, r6, asl #2]
	ldr	r2, [r2, #324]
	mov	r0, r2
	str	r2, [fp, #-100]
	bl	MEM_Phy2Vir
	ldr	ip, [r9, #92]
	ldr	r2, [r7, r6, asl #2]
	mov	r1, #49152
	cmp	ip, #1
	movt	r1, 63683
	str	r1, [fp, #-92]
	ldreq	r1, [r9, #100]
	ldr	r4, [r2, #336]
	ldr	r2, [r9]
	ldr	lr, [r9, #16]
	streq	ip, [fp, #-64]
	streq	r1, [fp, #-56]
	str	r4, [fp, #-96]
	strb	lr, [fp, #-108]
	str	r2, [fp, #-104]
	str	r0, [fp, #-112]
	ldreq	r0, [r9, #96]
	streq	r0, [fp, #-60]
	cmp	r2, #0
	beq	.L1788
	cmp	r2, #16
	ldr	r1, [r9, #88]
	mov	r2, #0
	str	r2, [fp, #-76]
	ldreq	r2, [r9, #56]
	str	r1, [fp, #-68]
.L1756:
	ldr	r4, .L1790+20
	mov	r0, #0
	str	r2, [fp, #-72]
	add	r5, r4, #64
	mov	r2, r4
.L1758:
	ldr	r3, [r2, #4]!
	cmp	r3, #0
	addne	r0, r0, #1
	cmp	r2, r5
	bne	.L1758
	cmp	r0, #1
	add	ip, r9, #4
	ldr	r3, [r10, #52]
	sub	r0, fp, #88
	movgt	r2, #0
	mov	r1, ip
	strgt	r2, [fp, #-72]
	mov	r2, #12
	str	ip, [fp, #-128]
	blx	r3
	sub	r1, fp, #112
	mov	r0, r6
	bl	SM_Config
	mov	r2, #1
	sub	r0, fp, #120
	ldr	r3, [r10, #48]
	mov	r1, #0
	blx	r3
	ldr	r2, [r9, #92]
	ldr	ip, [fp, #-128]
	cmp	r2, #1
	beq	.L1789
.L1760:
	ldr	lr, [r7, r6, asl #2]
	mov	r1, ip
	mov	r3, #0
	add	r2, lr, #311296
	str	r2, [fp, #-132]
	ldr	ip, [lr, #284]
	add	r0, lr, #1152
	ldr	r2, [lr, #276]
	add	r0, r0, #12
	str	ip, [lr, #1196]
	ldr	ip, [lr, #308]
	str	r2, [lr, #1192]
	ldr	r2, [lr, #312]
	str	ip, [lr, #628]
	ldr	ip, [lr, #340]
	str	r2, [lr, #632]
	ldr	r2, [lr, #344]
	str	ip, [lr, #640]
	ldr	ip, [lr, #348]
	str	r2, [lr, #644]
	ldr	r2, [lr, #352]
	str	ip, [lr, #648]
	ldr	ip, [lr, #356]
	str	r2, [lr, #652]
	mov	r2, #12
	str	ip, [lr, #656]
	ldr	ip, [lr, #360]
	str	ip, [lr, #660]
	ldr	ip, [r9, #20]
	str	ip, [lr, #612]
	ldr	ip, [r9, #20]
	str	ip, [lr, #608]
	ldr	ip, [r9, #28]
	str	ip, [lr, #616]
	ldr	ip, [r9, #32]
	str	ip, [lr, #620]
	ldr	ip, [r9, #36]
	str	ip, [lr, #624]
	ldr	ip, [r9, #60]
	str	ip, [lr, #1200]
	ldr	ip, [r9, #68]
	str	ip, [fp, #-128]
	mov	ip, #1
	str	ip, [lr, #1236]
	str	ip, [lr, #1240]
	ldr	ip, [fp, #-128]
	str	ip, [lr, #1204]
	ldr	ip, [r10, #52]
	ldr	r10, [r9, #80]
	sub	r10, r10, #1
	clz	r10, r10
	mov	r10, r10, lsr #5
	str	r10, [lr, #1220]
	ldr	r10, [r9, #84]
	str	r10, [lr, #1224]
	ldr	r10, [r9, #52]
	str	r3, [lr, #1480]
	str	r3, [lr, #1500]
	str	r10, [fp, #-128]
	ldr	r10, [fp, #-72]
	ldr	r3, [fp, #-132]
	str	r10, [lr, #1232]
	ldr	r10, [fp, #-128]
	str	r10, [lr, #1228]
	ldrb	lr, [r9, #117]	@ zero_extendqisi2
	ldr	r10, .L1790
	strb	lr, [r3, #2048]
	blx	ip
	ldr	ip, [r7, r6, asl #2]
	add	r0, ip, #311296
	ldr	lr, [ip, #600]
	ldr	r2, [r0, #2088]
	cmp	r2, lr
	beq	.L1761
	mov	r0, r6
	bl	VCTRL_ResetChan
	ldr	r3, [r9]
	ldr	ip, [r10, r6, asl #2]
	cmp	r3, #20
	beq	.L1762
	ldr	lr, [ip, #600]
	add	r0, ip, #311296
.L1761:
	ldr	r1, .L1790+24
	ldr	r2, .L1790+28
	str	lr, [r0, #2088]
.L1766:
	ldr	r3, [r4, #4]!
	add	r1, r1, #4
	add	r2, r2, #4
	cmp	r3, #0
	streq	r3, [r1, #-4]
	ldrne	r0, [r3, #28]
	strne	r0, [r1, #-4]
	ldrne	r3, [r3, #28]
	cmp	r4, r5
	str	r3, [r2, #-4]
	bne	.L1766
	ldr	r0, .L1790+24
	bl	VDMDRV_SetPriority
	mov	r0, r6
	bl	VDMDRV_ClearChanIsDec
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1787:
	ldr	r1, .L1790+32
	mov	r8, r2
	bl	dprint_vfmw
.L1777:
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1743:
	ldr	r1, .L1790+36
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r8, #0
	mov	r0, r8
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1788:
	ldr	r0, [r9, #52]
	ldr	r1, [r9, #88]
	ldr	r2, [r9, #56]
	str	r0, [fp, #-76]
	str	r1, [fp, #-68]
	b	.L1756
.L1786:
	sub	r0, fp, #116
	bl	REAL8DEC_Support
	ldr	r2, [fp, #-116]
	cmp	r2, #0
	ldrne	r2, [r9]
	bne	.L1747
.L1780:
	mvn	r8, #0
	b	.L1777
.L1749:
	sub	r0, fp, #116
	bl	REAL9DEC_Support
	ldr	r2, [fp, #-116]
	cmp	r2, #0
	bne	.L1753
	b	.L1780
.L1784:
	mov	r0, r4
	ldr	r3, .L1790+40
	movw	r2, #3250
	ldr	r1, .L1790+44
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1777
.L1789:
	sub	r1, fp, #120
	mov	r0, r6
	strb	r2, [fp, #-120]
	bl	FSP_ConfigAttribute
	ldr	ip, [fp, #-128]
	b	.L1760
.L1785:
	mov	r0, r4
	ldr	r1, .L1790+48
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1777
.L1762:
	ldr	r3, [ip, #328]
	mov	r0, r6
	ldr	r2, [ip, #1280]
	ldr	r1, [ip, #1276]
	ldr	ip, [ip, #332]
	str	ip, [sp]
	bl	VCTRL_PartitionMemForStdUser
	cmp	r0, #0
	bne	.L1763
	ldr	ip, [r10, r6, asl #2]
	add	r0, ip, #311296
	ldr	lr, [ip, #600]
	b	.L1761
.L1782:
	mov	r0, r4
	ldr	r3, .L1790+52
	mov	r2, #3248
	ldr	r1, .L1790+44
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1777
.L1783:
	mov	r0, r2
	ldr	r3, .L1790+56
	movw	r2, #3249
	ldr	r1, .L1790+44
	bl	dprint_vfmw
	mvn	r8, #0
	b	.L1777
.L1763:
	ldr	r1, .L1790+60
	mov	r0, #1
	bl	dprint_vfmw
	b	.L1780
.L1791:
	.align	2
.L1790:
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
	beq	.L1885
	cmp	r0, #544
	ldr	r4, [r1]
	beq	.L1794
	bls	.L1908
	cmp	r0, #1280
	beq	.L1817
	bhi	.L1818
	movw	r3, #897
	cmp	r0, r3
	beq	.L1819
	bls	.L1909
	cmp	r0, #1024
	beq	.L1826
	bhi	.L1827
	movw	r3, #898
	cmp	r0, r3
	beq	.L1903
	movw	r3, #899
	cmp	r0, r3
	bne	.L1885
	mov	r0, r4
	bl	VCTRL_StopChan
	mov	r2, r4
	ldr	r1, .L1924
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, r4
	bl	VCTRL_ResetChan
	mov	r2, r4
	ldr	r1, .L1924+4
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
.L1903:
	mov	r0, r4
	bl	VCTRL_StartChan
	mov	r2, r4
	ldr	r1, .L1924+8
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
.L1906:
	mov	r0, #1
.L1793:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1818:
	movw	r3, #1537
	cmp	r0, r3
	beq	.L1832
	bls	.L1910
	movw	r3, #1540
	cmp	r0, r3
	beq	.L1838
	bhi	.L1839
	movw	r3, #1538
	cmp	r0, r3
	beq	.L1840
	movw	r3, #1539
	cmp	r0, r3
	bne	.L1885
	ldr	r2, .L1924+12
	mov	r3, #1
	mov	r0, r3
	ldr	r2, [r2, r4, asl #3]
	str	r3, [r2, #40]
	b	.L1793
.L1908:
	cmp	r0, #11
	beq	.L1796
	bls	.L1911
	cmp	r0, #512
	beq	.L1805
	bls	.L1912
	movw	r3, #515
	cmp	r0, r3
	beq	.L1811
	cmp	r0, #516
	bcs	.L1812
	movw	r3, #513
	cmp	r0, r3
	beq	.L1813
	movw	r3, #514
	cmp	r0, r3
	bne	.L1885
	cmp	r4, #15
	bhi	.L1906
	ldr	r5, .L1924+16
	add	r6, r5, r4, lsl #2
	ldr	r7, [r6, #-3280]
	cmp	r7, #0
	beq	.L1913
	ldr	r7, .L1924+20
	mov	r0, #32
	ldr	r1, .L1924+24
	mov	r4, #0
	bl	dprint_vfmw
	ldr	r0, [r6, #-3280]
	ldr	r3, [r7, #36]
	blx	r3
	ldr	r0, [r5, #-3140]
	ldr	r3, .L1924+28
	mvn	r2, #0
	cmp	r0, r4
	str	r4, [r6, #-3280]
	str	r4, [r5, #-3124]
	str	r2, [r3, #68]
	str	r4, [r5, #-3120]
	str	r4, [r5, #-3116]
	str	r4, [r5, #-3112]
	beq	.L1906
	ldr	r1, [r5, #-3144]
	bl	MEM_ReleaseMemBlock
	ldr	r2, [r5, #-3132]
	ldr	r1, [r5, #-3144]
	ldr	r0, [r5, #-3140]
	bl	MEM_DelMemRecord
	ldr	r3, [r7, #48]
	mov	r1, r4
	mov	r2, #24
	ldr	r0, .L1924+32
	blx	r3
	b	.L1906
.L1912:
	cmp	r0, #13
	beq	.L1807
	bcc	.L1808
	cmp	r0, #256
	beq	.L1809
	movw	r3, #257
	cmp	r0, r3
	bne	.L1885
	ldr	r5, .L1924+36
	mov	r0, r4
	mov	r1, #1
	mov	r3, #67108864
	ldr	r4, [r5]
	str	r3, [r5]
	bl	PrintScdRawState
	mov	r0, #1
	str	r4, [r5]
	b	.L1793
.L1909:
	cmp	r0, #768
	beq	.L1821
	bls	.L1914
	movw	r3, #769
	cmp	r0, r3
	beq	.L1824
	cmp	r0, #896
	bne	.L1885
	mov	r0, r4
	bl	VCTRL_StopChan
	mov	r2, r4
	ldr	r1, .L1924
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1910:
	movw	r3, #1282
	cmp	r0, r3
	beq	.L1834
	bcc	.L1835
	movw	r3, #1283
	cmp	r0, r3
	beq	.L1836
	cmp	r0, #1536
	bne	.L1885
	cmp	r4, #15
	bhi	.L1882
	ldr	r3, .L1924+40
	mov	r5, #1
	mov	r2, r4
	ldr	r1, .L1924+44
	mov	r0, #32
	ldr	r3, [r3, r4, asl #2]
	str	r5, [r3, #1224]
	bl	dprint_vfmw
	mov	r0, r5
	b	.L1793
.L1911:
	cmp	r0, #4
	beq	.L1798
	bhi	.L1799
	cmp	r0, #1
	beq	.L1906
	bcc	.L1801
	cmp	r0, #2
	bne	.L1915
	mov	r5, r4, lsr #24
	bic	r4, r4, #-16777216
	ldr	r1, .L1924+48
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	mov	r2, #120
	mov	r1, #0
	sub	r0, fp, #164
	ldr	r3, [r3, #48]
	blx	r3
	sub	r1, fp, #164
	mov	r0, r5
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1906
	sub	r1, fp, #164
	mov	r0, r5
	str	r4, [fp, #-144]
	bl	VCTRL_ConfigChan
	b	.L1906
.L1799:
	cmp	r0, #6
	beq	.L1906
	bcc	.L1803
	cmp	r0, #7
	bne	.L1916
	mov	r5, r4, lsr #24
	bic	r4, r4, #-16777216
	ldr	r1, .L1924+52
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	mov	r2, #120
	mov	r1, #0
	sub	r0, fp, #164
	ldr	r3, [r3, #48]
	blx	r3
	sub	r1, fp, #164
	mov	r0, r5
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1906
	sub	r1, fp, #164
	mov	r0, r5
	str	r4, [fp, #-140]
	bl	VCTRL_ConfigChan
	b	.L1906
.L1827:
	movw	r3, #1025
	cmp	r0, r3
	beq	.L1830
	movw	r3, #1026
	cmp	r0, r3
	bne	.L1885
	ldr	r3, .L1924+56
	mov	r2, r4
	ldr	r1, .L1924+60
	mov	r0, #32
	str	r4, [r3, #-528]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1812:
	beq	.L1815
	cmp	r0, #528
	bne	.L1885
	cmp	r4, #15
	bhi	.L1906
	ldr	r6, .L1924+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-3108]
	cmp	r7, #0
	beq	.L1917
	ldr	r1, .L1924+64
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	ldr	r0, [r5, #-3108]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-3108]
	b	.L1793
.L1839:
	movw	r3, #1541
	cmp	r0, r3
	beq	.L1842
	cmp	r0, #1792
	bne	.L1885
	cmp	r4, #0
	bne	.L1906
	ldr	r5, .L1924+28
	ldr	r6, [r5, #72]
	cmp	r6, #1
	beq	.L1918
	ldr	r1, .L1924+68
	mov	r0, #32
	bl	dprint_vfmw
	mov	r3, #1
	mov	r0, r3
	str	r3, [r5, #72]
	b	.L1793
.L1840:
	ldr	r3, .L1924+72
	mov	r2, r4
	ldr	r1, .L1924+44
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1813:
	cmp	r4, #15
	bhi	.L1906
	ldr	r6, .L1924+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-3416]
	cmp	r7, #0
	beq	.L1919
	ldr	r1, .L1924+76
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	ldr	r0, [r5, #-3416]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-3416]
	b	.L1793
.L1836:
	ldr	r3, .L1924+80
	mov	r2, r4
	ldr	r1, .L1924+84
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1809:
	ldr	r1, .L1924+88
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, r4
	bl	DBG_PrintTracer
	mov	r0, #1
	b	.L1793
.L1824:
	bl	VCTRL_Resume
	mov	r0, #1
	b	.L1793
.L1830:
	ldr	r3, .L1924+16
	mov	r2, r4
	ldr	r1, .L1924+92
	mov	r0, #32
	str	r4, [r3, #-2700]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1801:
	ldr	r3, .L1924+36
	mov	r0, #1
	str	r4, [r3]
	b	.L1793
.L1842:
	ldr	r3, .L1924+16
	mov	r2, r4
	ldr	r1, .L1924+96
	mov	r0, #32
	str	r4, [r3, #-2696]
	bl	dprint_vfmw
	b	.L1906
.L1815:
	ldr	r5, .L1924+20
	mov	r0, r4
	ldmib	r1, {r8, r9}
	ldr	r3, [r5, #192]
	blx	r3
	subs	r7, r0, #0
	beq	.L1907
	mov	r0, #19
	ldr	r10, [r5, #68]
	bl	VFMW_GetString
	sub	r6, fp, #164
	str	r4, [sp]
	mov	r1, #50
	ldr	r3, .L1924+100
	mov	r2, r0
	mov	r0, r6
	blx	r10
	mov	r0, r6
	ldr	r3, [r5, #32]
	mov	r2, #0
	mov	r1, #66
	blx	r3
	subs	r4, r0, #0
	beq	.L1869
	ldr	r3, .L1924+104
	ldr	ip, [r3, #152]
	cmp	ip, #0
	beq	.L1870
	mov	lr, #1
	mov	r3, r9
	mov	r2, r8
	mov	r1, r7
	str	lr, [sp]
	blx	ip
.L1871:
	mov	r0, r4
	ldr	r3, [r5, #36]
	blx	r3
	mov	r2, r6
	ldr	r1, .L1924+108
	mov	r0, #32
	bl	dprint_vfmw
.L1868:
	mov	r0, r7
	ldr	r3, [r5, #152]
	blx	r3
	mov	r0, #1
	b	.L1793
.L1803:
	mov	r5, r4, lsr #24
	and	r4, r4, #7
	ldr	r1, .L1924+112
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1924+20
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
	bne	.L1846
	mov	r1, r6
	mov	r0, r5
	str	r4, [fp, #-136]
	bl	VCTRL_ConfigChan
	b	.L1906
.L1915:
	cmp	r0, #3
	beq	.L1906
.L1885:
	mvn	r0, #1
	b	.L1793
.L1914:
	cmp	r0, #560
	bne	.L1885
	cmp	r4, #15
	bhi	.L1906
	ldr	r6, .L1924+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-2836]
	cmp	r7, #0
	beq	.L1920
	ldr	r1, .L1924+116
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	ldr	r0, [r5, #-2836]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-2836]
	b	.L1793
.L1826:
	ldr	r3, .L1924+56
	mov	r2, r4
	ldr	r1, .L1924+120
	mov	r0, #32
	str	r4, [r3, #-532]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1832:
	cmp	r4, #15
	bhi	.L1882
	ldr	r3, .L1924+40
	mov	ip, #0
	mov	r2, r4
	ldr	r1, .L1924+44
	mov	r0, #32
	ldr	r3, [r3, r4, asl #2]
	str	ip, [r3, #1224]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1821:
	bl	VCTRL_Suspend
	mov	r0, #1
	b	.L1793
.L1811:
	ldr	r5, .L1924+20
	mov	r0, r4
	ldr	r3, [r5, #192]
	blx	r3
	subs	r7, r0, #0
	beq	.L1907
	mov	r0, #18
	ldr	r8, [r5, #68]
	bl	VFMW_GetString
	sub	r6, fp, #164
	str	r4, [sp]
	mov	r1, #50
	ldr	r3, .L1924+100
	mov	r2, r0
	mov	r0, r6
	blx	r8
	mov	r0, r6
	ldr	r3, [r5, #32]
	mov	r2, #0
	mov	r1, #66
	blx	r3
	subs	r4, r0, #0
	beq	.L1869
	mov	r2, r4
	ldr	r3, [r5, #44]
	mov	r1, #3145728
	mov	r0, r7
	blx	r3
	mov	r0, r4
	ldr	r3, [r5, #36]
	blx	r3
	mov	r2, r6
	ldr	r1, .L1924+124
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1868
.L1882:
	ldr	r1, .L1924+128
	mov	r0, #0
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1798:
	mov	r5, r4, lsr #24
	and	r4, r4, #3
	ldr	r1, .L1924+132
	mov	r0, #32
	mov	r3, r4
	mov	r2, r5
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	mov	r2, #120
	mov	r1, #0
	sub	r0, fp, #164
	ldr	r3, [r3, #48]
	blx	r3
	sub	r1, fp, #164
	mov	r0, r5
	bl	VCTRL_GetChanCfg
	cmp	r0, #0
	bne	.L1906
	sub	r1, fp, #164
	mov	r0, r5
	str	r4, [fp, #-132]
	bl	VCTRL_ConfigChan
	b	.L1906
.L1838:
	ldr	r3, .L1924+12
	mov	r2, #0
	mov	r0, #1
	ldr	r3, [r3, r4, asl #3]
	str	r2, [r3, #40]
	b	.L1793
.L1835:
	ldr	r3, .L1924+136
	mov	r2, r4
	ldr	r1, .L1924+140
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1834:
	ldr	r3, .L1924+144
	mov	r2, r4
	ldr	r1, .L1924+148
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1819:
	mov	r0, r4
	bl	VCTRL_ResetChan
	mov	r2, r4
	ldr	r1, .L1924+4
	mov	r3, r0
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1796:
	cmp	r4, #1
	ldrls	r3, .L1924+152
	movls	r2, r4
	ldrls	r1, .L1924+156
	movls	r0, #32
	movhi	r2, r4
	ldrhi	r1, .L1924+160
	strls	r4, [r3]
	movhi	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1805:
	cmp	r4, #15
	bhi	.L1906
	ldr	r6, .L1924+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-3836]
	cmp	r7, #0
	beq	.L1921
	ldr	r1, .L1924+164
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	ldr	r0, [r5, #-3836]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-3836]
	b	.L1793
.L1817:
	ldr	r3, .L1924+168
	mov	r2, r4
	ldr	r1, .L1924+172
	mov	r0, #32
	str	r4, [r3]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1808:
	cmp	r4, #1
	bls	.L1922
	mov	r2, r4
	ldr	r1, .L1924+176
	mov	r0, #32
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1807:
	cmp	r4, #16
	bls	.L1851
	mov	r2, r4
	ldr	r1, .L1924+180
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1794:
	cmp	r4, #15
	bhi	.L1906
	ldr	r6, .L1924+16
	add	r5, r6, r4, lsl #2
	ldr	r7, [r5, #-2972]
	cmp	r7, #0
	beq	.L1923
	ldr	r1, .L1924+64
	mov	r0, #32
	bl	dprint_vfmw
	ldr	r3, .L1924+20
	ldr	r0, [r5, #-2972]
	ldr	r3, [r3, #36]
	blx	r3
	mov	r3, #0
	mov	r0, #1
	str	r3, [r5, #-2972]
	b	.L1793
.L1846:
	mov	r2, r5
	ldr	r1, .L1924+184
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1851:
	ldr	r3, .L1924+16
	mov	r2, r4
	ldr	r1, .L1924+188
	mov	r0, #32
	str	r4, [r3, #-3428]
	bl	dprint_vfmw
	mov	r0, #1
	b	.L1793
.L1916:
	cmp	r0, #8
	beq	.L1906
	b	.L1885
.L1907:
	mov	r2, r4
	ldr	r1, .L1924+192
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1868
.L1922:
	ldr	r3, .L1924+16
	mov	r2, r4
	ldr	r1, .L1924+196
	mov	r0, #32
	str	r4, [r3, #-3840]
	bl	dprint_vfmw
	b	.L1906
.L1869:
	mov	r2, r6
	ldr	r1, .L1924+200
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1868
.L1918:
	ldr	r1, .L1924+204
	mov	r0, #32
	bl	dprint_vfmw
	str	r4, [r5, #72]
	mov	r0, r6
	b	.L1793
.L1920:
	ldr	r2, [r6, #-2708]
	mov	r1, #50
	ldr	r8, .L1924+20
	sub	r0, fp, #164
	add	ip, r2, #1
	ldr	r3, .L1924+100
	str	r2, [sp]
	ldr	r2, .L1924+208
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
	beq	.L1879
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1924+212
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1917:
	ldr	r8, .L1924+20
	mov	r0, #20
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-2980]
	mov	r1, #50
	ldr	r3, .L1924+100
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
	beq	.L1873
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1924+216
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1921:
	ldr	r8, .L1924+20
	mov	r0, #15
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-3424]
	mov	r1, #50
	ldr	r3, .L1924+100
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
	beq	.L1854
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1924+220
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1923:
	ldr	r8, .L1924+20
	mov	r0, #21
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-2844]
	mov	r1, #50
	ldr	r3, .L1924+100
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
	beq	.L1876
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1924+224
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1919:
	ldr	r8, .L1924+20
	mov	r0, #16
	ldr	r9, [r8, #68]
	bl	VFMW_GetString
	ldr	r2, [r6, #-3288]
	mov	r1, #50
	ldr	r3, .L1924+100
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
	beq	.L1858
	mov	r3, r4
	sub	r2, fp, #164
	ldr	r1, .L1924+228
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1870:
	mov	r0, ip
	movw	r2, #9005
	ldr	r1, .L1924+232
	bl	dprint_vfmw
	b	.L1871
.L1913:
	ldr	r8, .L1924+20
	mov	r0, #17
	ldr	r9, .L1924+28
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
	beq	.L1861
	ldr	r3, [r8, #48]
	mov	r2, #24
	mov	r1, r7
	ldr	r0, .L1924+32
	blx	r3
	mov	r0, #24
	bl	VFMW_GetString
	str	r7, [sp]
	ldr	r3, .L1924+32
	mov	r2, #14155776
	mov	r1, r7
	bl	MEM_AllocMemBlock
	cmp	r0, #0
	beq	.L1862
	ldr	r1, .L1924+236
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
	b	.L1906
.L1862:
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
	ldr	r1, .L1924+240
	str	r6, [r5, #-3120]
	str	ip, [r5, #-3124]
	str	lr, [r9, #68]
	str	r4, [r5, #-3112]
	bl	dprint_vfmw
	b	.L1906
.L1879:
	sub	r2, fp, #164
	ldr	r1, .L1924+244
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1873:
	sub	r2, fp, #164
	ldr	r1, .L1924+248
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1876:
	sub	r2, fp, #164
	ldr	r1, .L1924+252
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1854:
	sub	r2, fp, #164
	ldr	r1, .L1924+256
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1858:
	sub	r2, fp, #164
	ldr	r1, .L1924+260
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1861:
	sub	r2, fp, #164
	ldr	r1, .L1924+264
	mov	r0, #32
	bl	dprint_vfmw
	b	.L1906
.L1925:
	.align	2
.L1924:
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
	ldr	r5, .L1959
	mov	r4, r0
	mov	r6, r1
	ldr	r3, [r5, r0, asl #2]
	cmp	r3, #0
	beq	.L1953
	ldr	r2, [r1]
	cmp	r2, #1
	bhi	.L1954
	ldr	r9, [r3, #16]
	bl	VCTRL_StopChan
	ldr	r7, .L1959+4
	mov	r8, #0
	ldr	r3, [r5, r4, asl #2]
	mov	r2, #300
	ldr	r0, .L1959+8
	mov	r1, r8
	ldr	ip, [r7, #48]
	str	r8, [r3, #384]
	str	r8, [r3, #388]
	mla	r0, r2, r4, r0
	blx	ip
	ldr	r3, [r5, r4, asl #2]
	cmp	r3, r8
	addne	r3, r3, #307200
	strne	r8, [r3, #1996]
	beq	.L1955
.L1931:
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
	add	r1, r3, #311296
	ldr	r0, [r3, #600]
	add	r1, r1, #2096
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
	beq	.L1933
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq	.L1956
.L1933:
	bl	FSP_EmptyInstance
.L1934:
	mov	r0, r4
	bl	InitUsdPool
	ldr	r3, [r6]
	cmp	r3, #0
	beq	.L1957
.L1935:
	ldr	r3, .L1959+12
	ldr	r2, [r3, #-3840]
	cmp	r2, #0
	beq	.L1936
	add	r5, r3, r4, lsl #2
	str	r4, [fp, #-40]
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1938
	mov	r0, #512
	sub	r1, fp, #40
	bl	VCTRL_SetDbgOption
	ldr	r3, [r5, #-3836]
	cmp	r3, #0
	beq	.L1938
	sub	r1, fp, #40
	mov	r0, #512
	bl	VCTRL_SetDbgOption
.L1938:
	ldr	r1, .L1959+16
	mov	r0, #32
	bl	dprint_vfmw
.L1936:
	cmp	r9, #0
	bne	.L1958
.L1940:
	mov	r0, r4
	bl	VDMDRV_ClearChanIsDec
	mov	r0, #0
.L1928:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1957:
	mov	r0, r4
	bl	SM_Reset
	b	.L1935
.L1958:
	mov	r0, r4
	bl	VCTRL_StartChan
	b	.L1940
.L1956:
	bl	FSP_ResetInstance
	b	.L1934
.L1954:
	movw	r2, #4051
	ldr	r1, .L1959+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L1955:
	bl	VCTRL_SetSendStreamAgainFlag.part.2
	b	.L1931
.L1953:
	mov	r0, r3
	movw	r2, #4047
	ldr	r3, .L1959+24
	ldr	r1, .L1959+28
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L1928
.L1960:
	.align	2
.L1959:
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
	ldr	r5, .L1976
	mov	r4, r0
	mov	r0, #0
	ldr	r3, [r5, #12]
	blx	r3
	cmp	r4, #15
	ldrhi	r3, .L1976+4
	movwhi	r2, #2740
	bhi	.L1971
	ldr	r7, .L1976+8
	ldr	r6, [r7, r4, asl #2]
	cmp	r6, #0
	beq	.L1972
	ldr	r8, [r6, #12]
	cmp	r8, #1
	bne	.L1973
	mov	r0, #0
	str	r0, [r6, #16]
	ldr	r3, [r5, #16]
	blx	r3
	mov	r0, r4
	bl	SCDDRV_IsChanDec
	cmp	r0, #0
	bne	.L1974
.L1966:
	ldr	r3, [r5, #12]
	mov	r0, #0
	blx	r3
	ldr	r2, .L1976+12
	ldr	r3, [r5, #12]
	mov	r8, #0
	mov	r0, #4
	ldr	r6, .L1976
	str	r8, [r2, r4, asl #2]
	blx	r3
	mov	r0, r4
	bl	VCTRL_StopChan
	ldr	r3, [r6, #16]
	subs	r5, r0, #0
	mov	r0, #4
	bne	.L1975
	blx	r3
	mov	r0, r4
	bl	VCTRL_ResetChan
	bl	VDMDRV_ParamInit
	ldr	r3, .L1976+16
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
	ldr	ip, .L1976+20
	mov	r0, r4
	add	lr, r3, #311296
	add	r2, r3, #307200
	ldr	r1, [r3, #296]
	add	r2, r2, #2000
	ldr	r9, [r3, #300]
	ldr	r8, [r3, #292]
	ldr	r10, [lr, #2052]
	add	lr, r3, #220
	str	r5, [r7, r4, asl #2]
	add	r7, r3, #244
	add	r3, r3, #268
	str	r3, [fp, #-72]
	ldr	r3, .L1976+24
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
.L1963:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1974:
	mov	r0, r4
	ldr	r6, [r5, #128]
	bl	SCDDRV_IsChanDec
	mov	r2, #500
	clz	r1, r0
	mov	r0, r8
	mov	r1, r1, lsr #5
	blx	r6
	b	.L1966
.L1973:
	ldr	r3, .L1976+28
	movw	r2, #2764
.L1971:
	ldr	r1, .L1976+32
	mov	r0, #0
	bl	dprint_vfmw
	ldr	r3, [r5, #16]
	mov	r0, #0
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L1975:
	blx	r3
	ldr	r3, [r6, #16]
	mov	r0, r8
	blx	r3
	mvn	r0, #0
	b	.L1963
.L1972:
	ldr	r3, .L1976+36
	movw	r2, #2750
	ldr	r1, .L1976+32
	mov	r0, r6
	bl	dprint_vfmw
	ldr	r3, [r5, #16]
	mov	r0, r6
	blx	r3
	mvn	r0, #0
	b	.L1963
.L1977:
	.align	2
.L1976:
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
	ldr	r6, .L1997
	ldr	r3, [r6, #64]
	cmp	r3, #1
	bne	.L1986
	sub	r5, r6, #4
	mov	r4, #0
.L1982:
	ldr	r3, [r5, #4]!
	mov	r0, r4
	add	r4, r4, #1
	cmp	r3, #0
	beq	.L1981
	bl	VCTRL_DestroyChanWithOption
.L1981:
	cmp	r4, #16
	bne	.L1982
	bl	FSP_Exit
	ldr	r3, [r6, #68]
	mov	r2, #3
	ldr	r5, .L1997+4
	cmp	r3, #0
	str	r2, [r6, #72]
	beq	.L1984
	mov	r0, #10
	ldr	r3, [r5, #116]
	mov	r4, #49
	blx	r3
	b	.L1985
.L1996:
	ldr	r3, [r5, #116]
	blx	r3
	subs	r4, r4, #1
	beq	.L1984
.L1985:
	ldr	r3, [r6, #68]
	mov	r0, #10
	cmp	r3, #0
	bne	.L1996
.L1984:
	ldr	r3, [r5, #48]
	mov	r2, #64
	mov	r1, #0
	ldr	r0, .L1997+8
	blx	r3
	ldr	r0, .L1997+12
	bl	CloseDrivers
	mov	r3, #0
	mov	r0, r3
	str	r3, [r6, #64]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1986:
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L1998:
	.align	2
.L1997:
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
	beq	.L2000
	cmp	r1, #0
	moveq	r4, #2
	movne	r4, #0
.L2000:
	ldr	r3, .L2014
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L2010
	mov	r1, r4
	mov	r0, #0
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VDH_Require_DspCode
.L2010:
	ldr	r6, .L2014+4
	ldr	r3, [r6, #4]
	cmp	r3, #0
	beq	.L2011
	mov	r0, #0
	blx	r3
.L2003:
	ldr	r3, [r6, #20]
	cmp	r3, #0
	beq	.L2012
	mov	r0, #0
	blx	r3
.L2005:
	ldr	r3, [r6, #12]
	cmp	r3, #0
	beq	.L2013
	mov	r0, #0
	blx	r3
.L2007:
	mov	r1, r4
	mov	r0, #0
	bl	SCDDRV_LoadDspCode
	mov	r0, r5
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, lr}
	b	VCTRL_CloseHardware
.L2011:
	mov	r0, r3
	movw	r2, #3799
	ldr	r1, .L2014+8
	bl	dprint_vfmw
	b	.L2003
.L2012:
	mov	r0, r3
	movw	r2, #3800
	ldr	r1, .L2014+8
	bl	dprint_vfmw
	b	.L2005
.L2013:
	mov	r0, r3
	movw	r2, #3801
	ldr	r1, .L2014+8
	bl	dprint_vfmw
	b	.L2007
.L2015:
	.align	2
.L2014:
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
	ldr	r3, .L2040
	mov	r5, r0
	ldr	r3, [r3, r0, asl #2]
	ldr	r1, [r3, #600]
	cmp	r1, #0
	movne	r3, #1
	movne	r6, #0
	ldreq	r6, [r3, #1228]
	clzeq	r3, r6
	moveq	r3, r3, lsr #5
.L2017:
	cmp	r1, #6
	moveq	r3, #0
	andne	r3, r3, #1
	cmp	r3, #0
	beq	.L2038
.L2029:
	mov	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2038:
	ldr	r7, .L2040+4
	ldr	r4, [r7, #-788]
	cmp	r4, #0
	bne	.L2018
	sub	r3, r7, #788
	sub	r2, r7, #728
.L2019:
	ldr	r4, [r3, #4]!
	cmp	r4, #0
	bne	.L2018
	cmp	r3, r2
	bne	.L2019
	mov	r0, r5
	add	r5, r7, r5, lsl #2
	bl	VCTRL_InitDspCode
	cmp	r6, #1
	mov	r0, r4
	streq	r6, [r5, #-788]
	movne	r3, #2
	strne	r3, [r5, #-788]
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2018:
	cmp	r4, #1
	beq	.L2039
	cmp	r1, #6
	beq	.L2029
	ldr	r1, .L2040+8
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2039:
	cmp	r6, #1
	beq	.L2029
	ldr	r1, .L2040+12
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2041:
	.align	2
.L2040:
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
	ldr	r3, .L2043
	ldr	r3, [r3, r0, asl #2]
	add	r3, r3, #307200
	str	r1, [r3, #1952]
	ldmfd	sp, {fp, sp, pc}
.L2044:
	.align	2
.L2043:
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
	ldr	r3, .L2046
	ldr	r3, [r3, r0, asl #2]
	add	r3, r3, #307200
	ldr	r0, [r3, #1972]
	ldmfd	sp, {fp, sp, pc}
.L2047:
	.align	2
.L2046:
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
	beq	.L2050
	bcc	.L2051
	cmp	r1, #2
	bne	.L2058
	ldr	r3, .L2059
	add	r0, r3, r0, lsl #2
	ldr	r3, [r0, #-3704]
	cmp	r3, #1
	beq	.L2055
	mov	r3, #0
	str	r1, [r0, #-3704]
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2058:
	mov	r3, #0
.L2049:
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2051:
	ldr	r2, .L2059
	mov	r1, #0
	mov	r3, r1
	add	r0, r2, r0, lsl #2
	str	r1, [r0, #-3704]
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2050:
	ldr	r3, .L2059
	add	r0, r3, r0, lsl #2
	ldr	r3, [r0, #-3704]
	cmp	r3, #0
	bne	.L2055
	str	r1, [r0, #-3704]
	mov	r0, r3
	ldmfd	sp, {fp, sp, pc}
.L2055:
	mvn	r3, #0
	b	.L2049
.L2060:
	.align	2
.L2059:
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
	ldr	r3, .L2062
	ldr	ip, [r1, #4]
	ldr	r3, [r3, r0, asl #2]
	add	r2, r3, #307200
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
	str	r3, [r2, #1980]
	ldmfd	sp, {fp, sp, pc}
.L2063:
	.align	2
.L2062:
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
	UNWIND(.pad #28)
	sub	sp, sp, #28
	ldr	r8, .L2147
	ldr	r3, .L2147+4
	mov	r5, r1
	mov	r10, r0
	ldr	r9, [r8, r0, asl #2]
	ldr	r6, [r3, r0, asl #2]
	add	r2, r9, #307200
	add	r4, r9, #311296
	ldr	r3, [r2, #1980]
	ldr	r1, [r4, #1480]
	cmp	r3, #32
	ldr	r0, [r4, #1488]
	movge	r3, #32
	str	r1, [fp, #-48]
	str	r3, [r4, #1288]
	ldr	r1, [r5, #12]
	cmp	r1, r0
	beq	.L2065
	ldr	r3, [r2, #1968]
	cmp	r3, #0
	beq	.L2141
.L2065:
	ldr	r3, [r4, #1456]
	cmp	r3, #0
	beq	.L2067
	ldr	r2, [r4, #1520]
	cmp	r2, #0
	beq	.L2142
.L2067:
	ldr	r7, .L2147+8
	mov	r0, #8
	ldr	r3, [r7, #12]
	blx	r3
	ldr	lr, [r8, r10, asl #2]
	add	r3, lr, #307200
	ldr	r3, [r3, #2000]
	cmp	r3, #255
	beq	.L2096
	movw	r3, #47128
	mov	r2, #1
	movt	r3, 4
	add	r3, lr, r3
	b	.L2069
.L2143:
	add	r2, r2, #1
	cmp	r2, #32
	beq	.L2135
.L2069:
	ldr	r1, [r3]
	add	r3, r3, #72
	cmp	r1, #255
	bne	.L2143
.L2068:
	mov	r0, r2, asl #6
	ldr	r3, [r7, #52]
	add	r0, r0, r2, lsl #3
	mov	r1, r5
	add	r0, r0, #307200
	mov	r2, #72
	add	r0, r0, #2000
	add	r0, lr, r0
	blx	r3
	ldr	r3, [r8, r10, asl #2]
	add	r3, r3, #311296
	ldr	r2, [r3, #1520]
	add	r2, r2, #1
	str	r2, [r3, #1520]
	ldr	r3, [r5]
	cmp	r3, #255
	beq	.L2090
	ldr	r2, [r5, #12]
	ldr	r1, [r5, #24]
	ldr	r0, [r5, #32]
	bl	MEM_AddMemRecord
.L2090:
	ldr	r3, [r5, #4]
	cmp	r3, #255
	beq	.L2071
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #40]
	ldr	r0, [r5, #48]
	bl	MEM_AddMemRecord
.L2071:
	ldr	r3, [r5, #8]
	cmp	r3, #255
	beq	.L2072
	ldr	r2, [r5, #20]
	ldr	r1, [r5, #56]
	ldr	r0, [r5, #64]
	bl	MEM_AddMemRecord
.L2072:
	ldr	r3, [r8, r10, asl #2]
	add	r3, r3, #311296
	ldr	r8, [r3, #1524]
	cmp	r8, #0
	beq	.L2088
	ldrsb	r2, [r6, #1]
	cmp	r2, #0
	bne	.L2144
.L2074:
	ldr	r3, [r4, #1288]
	strb	r3, [r6, #10]
.L2075:
	mov	r8, #0
	strb	r8, [r6, #11]
	ldr	r3, [r4, #1456]
	strb	r3, [r6, #13]
	ldr	r3, [r5]
	cmp	r3, #255
	beq	.L2145
	ldrb	r8, [r4, #224]	@ zero_extendqisi2
	cmp	r8, #0
	beq	.L2076
	add	r3, r4, #236
	mov	r8, #1
	b	.L2077
.L2146:
	add	r8, r8, #1
	cmp	r8, #32
	beq	.L2135
.L2077:
	ldrb	r2, [r3]	@ zero_extendqisi2
	add	r3, r3, #12
	cmp	r2, #0
	bne	.L2146
.L2076:
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
	str	r2, [r1, #216]
	ldr	lr, [r5, #12]
	mov	ip, #1
	add	r0, r0, #311296
	strb	ip, [r1, #224]
	add	r2, r10, r2
	str	lr, [r1, #220]
	add	r10, r2, r3
	ldr	lr, [r4, #1460]
	add	lr, lr, ip
	str	lr, [r4, #1460]
	str	r10, [r0, #1112]
	ldr	lr, [r4, #1244]
	add	r3, r3, lr
	add	r3, r3, r2
	str	r3, [r0, #600]
	ldr	r3, [r5, #48]
	strb	ip, [r1, #736]
	str	r3, [r1, #728]
	ldr	r3, [r4, #1468]
	add	r3, r3, ip
	str	r3, [r4, #1468]
	ldr	r3, [r5, #8]
	cmp	r3, #255
	bne	.L2093
.L2079:
	add	r1, r6, #20480
	ldr	lr, [r1, #1020]
	cmp	lr, #0
	beq	.L2099
	add	r2, r6, #21504
	mov	r3, #1
	add	r2, r2, #96
	b	.L2084
.L2083:
	add	r3, r3, #1
	cmp	r3, #32
	beq	.L2135
.L2084:
	ldr	r0, [r2]
	add	r2, r2, #100
	cmp	r0, #0
	bne	.L2083
	mov	lr, r3
.L2082:
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
	add	r0, r0, #311296
	str	ip, [r2, #3980]
	str	r0, [fp, #-48]
	mov	ip, r10
	str	lr, [r10, #164]
	mov	lr, r10
	ldr	r0, [r8, #216]
	mov	r9, lr
	str	r0, [ip, #172]
	ldr	r0, [r8, #220]
	mov	r8, #0
	str	r0, [ip, #176]
	add	ip, r10, #216
	ldr	r0, [fp, #-48]
	str	ip, [fp, #-52]
	mov	ip, r10
	ldr	r0, [r0, #600]
	str	r0, [r10, #184]
	add	r0, lr, #224
	ldr	r10, [r4, #208]
	str	r0, [fp, #-56]
	str	r10, [lr, #188]
	add	r10, lr, #232
	ldr	r0, [r4, #1260]
	str	r10, [fp, #-60]
	ldr	r10, [fp, #-52]
	str	r0, [lr, #204]
	ldr	r0, [r4, #1268]
	str	r0, [lr, #208]
	ldr	r0, [r4, #1272]
	str	r0, [lr, #212]
	ldr	r0, [r4, #1252]
	str	r0, [lr, #216]
	ldr	r0, [r4, #1256]
	str	r0, [r10, #4]
	ldr	r0, [fp, #-48]
	ldr	r0, [r0, #1112]
	str	r0, [lr, #192]
	ldr	lr, [r4, #1240]
	str	lr, [ip, #196]
	ldr	lr, [r4, #1244]
	str	lr, [ip, #200]
	ldr	ip, [r6, #28]
	str	ip, [r10, #8]
	ldr	r10, [fp, #-56]
	ldr	lr, [r6, #32]
	str	lr, [r10, #4]
	ldr	r0, [r4, #1276]
	str	r0, [r10, #8]
	ldr	r10, [fp, #-60]
	ldr	r0, [r4, #1280]
	str	r0, [r10, #4]
	ldr	r0, [r4, #1284]
	str	r0, [r10, #8]
	mov	r0, #1
	strb	r8, [r9, #156]
	strb	r8, [r9, #157]
	strb	r8, [r9, #158]
	str	r0, [r9, #252]
	ldr	r3, [r2, #3324]
	cmp	r3, r8
	bne	.L2095
	mov	r0, #1
	strb	r0, [r6, #12]
	str	r3, [r2, #3236]
	str	r3, [r2, #3244]
	ldr	r8, [r4, #208]
	str	ip, [r2, #3296]
	str	lr, [r2, #3300]
	str	r8, [r2, #3260]
	ldr	r8, [r4, #1276]
	str	r8, [r2, #3304]
	ldr	r8, [r4, #1280]
	str	r8, [r2, #3308]
	ldr	r8, [r4, #1284]
	strb	r3, [r2, #3228]
	strb	r3, [r2, #3229]
	str	r8, [r2, #3312]
	strb	r3, [r2, #3230]
	str	r0, [r2, #3324]
.L2095:
	ldr	r3, [r2, #3984]
	ldrsb	r0, [r6, #13]
	cmp	r3, r0
	bcs	.L2086
	ldr	r0, [r5, #8]
	cmp	r0, #255
	beq	.L2086
	add	r8, r4, r3, lsl #3
	add	r0, r6, r3, lsl #3
	add	r0, r0, #24576
	add	r3, r3, #1
	ldr	r8, [r8, #1292]
	mov	r10, #0
	mov	r9, #1
	strb	r10, [r0, #3821]
	strb	r9, [r0, #3820]
	str	r8, [r0, #3816]
	str	r3, [r2, #3984]
.L2086:
	ldr	r3, [r2, #3988]
	ldr	r0, [r4, #1288]
	cmp	r3, r0
	bcs	.L2087
	ldr	r0, [r5, #4]
	cmp	r0, #255
	beq	.L2087
	ldr	r9, [r4, #1468]
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
	ldr	r10, [r8, #728]
	mov	r8, #1
	str	r10, [r0, #180]
	str	r9, [r3, #108]
	str	r8, [r3, #104]
	str	r5, [r2, #3988]
.L2087:
	ldr	r3, [r4, #1452]
	str	r3, [r2, #3812]
	ldr	r3, [r1, #920]
	cmp	r3, #0
	bne	.L2088
	ldr	r0, [r1, #940]
	mov	r2, #1
	str	r3, [r1, #832]
	str	r0, [r1, #840]
	ldr	r0, [r4, #208]
	str	ip, [r1, #892]
	str	lr, [r1, #896]
	str	r0, [r1, #856]
	ldr	r0, [r4, #1276]
	str	r0, [r1, #900]
	ldr	r0, [r4, #1280]
	str	r0, [r1, #904]
	ldr	r0, [r4, #1284]
	strb	r3, [r1, #824]
	strb	r3, [r1, #825]
	str	r0, [r1, #908]
	strb	r3, [r1, #826]
	str	r2, [r1, #920]
.L2088:
	ldr	r3, [r7, #16]
	mov	r0, #8
	blx	r3
	mov	r0, #0
.L2132:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2135:
	ldr	r3, [r7, #16]
	mov	r0, #8
	blx	r3
	mvn	r0, #0
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2145:
	ldr	r3, [r5, #8]
	cmp	r3, #255
	beq	.L2079
.L2093:
	ldrb	r1, [r4, #1296]	@ zero_extendqisi2
	cmp	r1, #0
	beq	.L2080
	mov	r3, #1
.L2081:
	mov	r1, r3, asl #3
	add	r3, r3, #1
	add	r2, r1, #311296
	add	r2, r2, #1296
	ldrb	r2, [r9, r2]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L2080
	cmp	r3, #20
	bne	.L2081
	b	.L2079
.L2144:
	ldr	r3, [r3, #2052]
	cmp	r3, #0
	bne	.L2074
	ldrsb	r3, [r6, #23]
	ldr	r2, [r4, #1288]
	add	r3, r3, #4
	cmp	r3, r2
	movge	r3, r2
	strb	r3, [r6, #10]
	b	.L2075
.L2142:
	ldr	r3, [r5, #8]
	cmp	r3, #255
	bne	.L2067
	str	r3, [sp, #4]
	mov	r0, #1
	str	r2, [sp]
	movw	r3, #10235
	ldr	r2, .L2147+12
	ldr	r1, .L2147+16
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2132
.L2080:
	ldr	r2, [r5, #64]
	add	r1, r9, r1
	add	r1, r1, #311296
	mov	r3, #1
	str	r2, [r1, #1292]
	strb	r3, [r1, #1296]
	ldr	r3, [r4, #1464]
	add	r3, r3, #1
	str	r3, [r4, #1464]
	b	.L2079
.L2096:
	mov	r2, #0
	b	.L2068
.L2099:
	mov	r3, lr
	b	.L2082
.L2141:
	str	r0, [sp, #4]
	movw	r3, #10225
	str	r1, [sp]
	mov	r0, #1
	ldr	r2, .L2147+12
	ldr	r1, .L2147+20
	bl	dprint_vfmw
	mvn	r0, #0
	b	.L2132
.L2148:
	.align	2
.L2147:
	.word	.LANCHOR0
	.word	s_pFspInst
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR1+304
	.word	.LC216
	.word	.LC215
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
	ldr	r4, .L2158
	mov	r7, r0, asl #2
	mov	r6, r0
	add	r3, r4, r7
	ldr	r8, [r3, #-3704]
	cmp	r8, #0
	bne	.L2150
	mov	r2, #1
	str	r2, [r3, #-3704]
	bl	VCTRL_SetSingleFrameBufToChan
	subs	r5, r0, #0
	bne	.L2151
	ldr	r3, .L2158+4
	ldr	ip, [r3, r6, asl #2]
	add	ip, ip, #307200
.L2152:
	add	r4, r4, r7
	mov	r3, #2
	ldr	r1, .L2158+8
	mov	r0, #31
	str	r3, [ip, #1952]
	bl	dprint_vfmw
	mov	r3, #0
	mov	r0, r5
	str	r3, [r4, #-3704]
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2151:
	ldr	r9, .L2158+4
	ldr	ip, [r9, r6, asl #2]
	add	ip, ip, #307200
	ldr	r3, [ip, #1972]
	cmp	r3, #0
	beq	.L2152
	mov	r2, r6
	mov	r0, r8
	ldr	r1, .L2158+12
	bl	dprint_vfmw
	ldr	r3, [r9, r6, asl #2]
	mov	r2, #3
	add	r3, r3, #307200
	mov	ip, r3
	str	r2, [r3, #1952]
	b	.L2152
.L2150:
	mov	r3, r0
	ldr	r2, .L2158+16
	ldr	r1, .L2158+20
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r0, #0
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, fp, sp, pc}
.L2159:
	.align	2
.L2158:
	.word	.LANCHOR4
	.word	.LANCHOR0
	.word	.LC218
	.word	.LC217
	.word	.LANCHOR1+336
	.word	.LC219
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
	beq	.L2160
	ldr	r1, .L2261
	mov	r0, #32
	bl	dprint_vfmw
	cmp	r4, #0
	cmpne	r4, #3
	bne	.L2163
	ldr	r3, [fp, #-52]
	ldr	r4, .L2261+4
	cmp	r3, #0
	beq	.L2165
	ldr	r10, .L2261+8
	mov	r3, #0
	mov	r8, r3
	str	r3, [fp, #-48]
	str	r6, [fp, #-92]
.L2167:
	cmp	r5, #0
	beq	.L2168
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r6, r3, #15
	mov	r2, r3, lsr #4
	ldr	r3, [fp, #-60]
	mul	r2, r3, r2
	add	r6, r2, r6, lsl #8
.L2166:
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
	bhi	.L2166
.L2168:
	ldr	r3, [fp, #-48]
	add	r8, r8, r5
	ldr	r2, [fp, #-52]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2167
	ldr	r6, [fp, #-92]
.L2165:
	ldr	r3, [fp, #-80]
	ldr	r2, [fp, #-76]
	ldr	r1, [fp, #-52]
	cmp	r3, r5
	cmpeq	r2, r1
	moveq	r3, #1
	movne	r3, #0
	str	r3, [fp, #-92]
	beq	.L2256
	ldr	r2, [fp, #-84]
	ldr	r3, [fp, #-88]
	ldr	r9, [fp, #-76]
	mla	r7, r3, r5, r2
	cmp	r9, #0
	ldr	r2, [r4, #-3116]
	add	r7, r2, r7
	beq	.L2173
	ldr	r10, .L2261+8
	str	r6, [fp, #-48]
	str	r4, [fp, #-52]
	ldr	r8, [fp, #-92]
	mov	r4, r10
	ldr	r6, [fp, #-80]
	ldr	r10, [fp, #-64]
.L2174:
	mov	r0, r7
	add	r8, r8, #1
	ldr	r3, [r4, #44]
	mov	r2, r10
	mov	r1, r6
	add	r7, r7, r5
	blx	r3
	cmp	r8, r9
	bne	.L2174
	ldr	r6, [fp, #-48]
	ldr	r4, [fp, #-52]
.L2173:
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	beq	.L2170
	ldr	r3, [fp, #-60]
	mov	r8, #0
	ldr	r10, .L2261+8
	str	r8, [fp, #-48]
	mov	r3, r3, lsr #1
	str	r3, [fp, #-52]
.L2171:
	cmp	r5, #0
	beq	.L2178
	ldr	r3, [fp, #-48]
	mov	r9, #0
	and	r7, r3, #7
	mov	r2, r3, lsr #3
	ldr	r3, [fp, #-52]
	mul	r2, r3, r2
	add	r7, r2, r7, lsl #8
.L2176:
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
	bhi	.L2176
.L2178:
	ldr	r3, [fp, #-48]
	add	r8, r8, r5
	ldr	r2, [fp, #-56]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2171
	ldr	r3, [fp, #-68]
	ldr	lr, [r4, #-3116]
	cmp	r3, #1
	beq	.L2257
.L2180:
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	beq	.L2258
.L2181:
	ldr	r3, [fp, #-92]
	cmp	r3, #0
	bne	.L2259
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
	beq	.L2160
	ldr	r3, [fp, #-80]
	mov	r9, r1
	str	r4, [fp, #-48]
	ldr	r10, .L2261+8
	ldr	r8, [fp, #-92]
	mov	r6, r3, lsr #1
	ldr	r4, [fp, #-64]
.L2188:
	mov	r0, r7
	add	r8, r8, #1
	ldr	r3, [r10, #44]
	mov	r2, r4
	mov	r1, r6
	add	r7, r7, r9
	blx	r3
	cmp	r8, r5
	bne	.L2188
	ldr	r4, [fp, #-48]
	mov	r7, #0
	ldr	r8, [fp, #-72]
	ldr	r9, [fp, #-64]
.L2189:
	mov	r0, r4
	add	r7, r7, #1
	ldr	r3, [r10, #44]
	mov	r2, r9
	mov	r1, r6
	add	r4, r4, r8
	blx	r3
	cmp	r7, r5
	bne	.L2189
.L2160:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2163:
	subs	r3, r4, #1
	ldr	r2, [fp, #-52]
	ldr	r10, .L2261+8
	movne	r3, #1
	cmp	r2, r3
	str	r3, [fp, #-76]
	ldrls	r4, .L2261+4
	bls	.L2191
	ldr	r3, [fp, #-76]
	ldr	r4, .L2261+4
	str	r6, [fp, #-80]
	str	r3, [fp, #-48]
.L2193:
	cmp	r5, #0
	beq	.L2194
	ldr	r3, [fp, #-48]
	mov	r6, #0
	and	r8, r3, #15
	mov	r2, r3, lsr #4
	mov	r9, r3, lsr #1
	ldr	r3, [fp, #-60]
	mul	r9, r5, r9
	mul	r2, r3, r2
	add	r8, r2, r8, lsl #8
.L2192:
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
	bhi	.L2192
.L2194:
	ldr	r3, [fp, #-48]
	ldr	r2, [fp, #-52]
	add	r3, r3, #2
	str	r3, [fp, #-48]
	cmp	r2, r3
	bhi	.L2193
	ldr	r6, [fp, #-80]
.L2191:
	ldr	r3, [fp, #-52]
	ldr	ip, [r10, #44]
	ldr	r2, [fp, #-64]
	mul	r1, r3, r5
	ldr	r0, [r4, #-3116]
	mov	r1, r1, lsr #1
	blx	ip
	ldr	r3, [fp, #-56]
	cmp	r3, #0
	beq	.L2195
	ldr	r3, [fp, #-60]
	mov	r8, #0
	str	r8, [fp, #-48]
	mov	r3, r3, lsr #1
	str	r3, [fp, #-52]
.L2196:
	cmp	r5, #0
	beq	.L2199
	ldr	r3, [fp, #-48]
	mov	r7, #0
	and	r9, r3, #7
	mov	r2, r3, lsr #3
	ldr	r3, [fp, #-52]
	mul	r2, r3, r2
	add	r9, r2, r9, lsl #8
.L2197:
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
	bhi	.L2197
.L2199:
	ldr	r3, [fp, #-48]
	add	r8, r8, r5
	ldr	r2, [fp, #-56]
	add	r3, r3, #1
	str	r3, [fp, #-48]
	cmp	r3, r2
	bne	.L2196
	ldr	r3, [fp, #-68]
	ldr	r2, [r4, #-3116]
	cmp	r3, #1
	beq	.L2260
	ldr	r3, [fp, #-68]
	cmp	r3, #0
	bne	.L2202
	ldr	r2, [fp, #-56]
	ldr	r3, [fp, #-76]
	cmp	r2, r3
	bls	.L2207
.L2208:
	add	r3, r3, #2
	cmp	r2, r3
	bhi	.L2208
.L2207:
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
.L2202:
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
.L2254:
	ldr	r0, [r4, #-3124]
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
	bx	r3
.L2256:
	ldr	r10, .L2261+8
	ldr	r2, [fp, #-64]
	mul	r1, r1, r5
	ldr	r0, [r4, #-3116]
	ldr	r3, [r10, #44]
	blx	r3
	b	.L2173
.L2260:
	ldr	r3, [fp, #-76]
	mov	r6, #0
	ldr	lr, [fp, #-72]
	ldr	r8, [fp, #-56]
	mla	r2, r3, r5, r2
	mov	r5, r5, asl #1
	add	r7, r2, #1
.L2203:
	cmp	lr, #0
	beq	.L2206
	mov	r3, r6, lsr #1
	mov	r2, r7
	mul	r3, lr, r3
	add	ip, r3, lr
.L2204:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	ldr	r1, [r4, #-3124]
	strb	r0, [r1, r3]
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	ldr	r1, [r4, #-3120]
	strb	r0, [r1, r3]
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L2204
.L2206:
	add	r6, r6, #2
	add	r7, r7, r5
	cmp	r8, r6
	bhi	.L2203
	b	.L2202
.L2257:
	ldr	r7, [fp, #-72]
	mov	r3, #0
	ldr	r8, [fp, #-56]
	add	lr, lr, #1
	mov	r6, r3
.L2182:
	cmp	r7, #0
	moveq	ip, r3
	beq	.L2185
	add	ip, r3, r7
	mov	r2, lr
.L2183:
	ldrb	r0, [r2, #-1]	@ zero_extendqisi2
	ldr	r1, [r4, #-3124]
	strb	r0, [r1, r3]
	ldrb	r0, [r2], #2	@ zero_extendqisi2
	ldr	r1, [r4, #-3120]
	strb	r0, [r1, r3]
	add	r3, r3, #1
	cmp	r3, ip
	bne	.L2183
.L2185:
	add	r6, r6, #1
	mov	r3, ip
	cmp	r6, r8
	add	lr, lr, r5
	bne	.L2182
	b	.L2181
.L2258:
	ldr	r10, .L2261+8
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
	b	.L2181
.L2259:
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-72]
	ldr	r7, [fp, #-64]
	ldr	r6, .L2261+8
	mul	r5, r3, r2
	ldr	r0, [r4, #-3120]
	mov	r2, r7
	ldr	r3, [r6, #44]
	mov	r1, r5
	blx	r3
	ldr	r3, [r6, #44]
	mov	r1, r5
	mov	r2, r7
	b	.L2254
.L2195:
	ldr	r3, [fp, #-68]
	cmp	r3, #1
	beq	.L2202
	cmp	r3, #0
	beq	.L2207
	b	.L2202
.L2170:
	ldr	r3, [fp, #-68]
	cmp	r3, #1
	bne	.L2180
	b	.L2181
.L2262:
	.align	2
.L2261:
	.word	.LC220
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
	ldr	r1, .L2321
	bl	dprint_vfmw
	cmp	r5, #15
	bhi	.L2310
	ldr	r6, .L2321+4
	ldr	r0, [r6, r5, asl #2]
	cmp	r0, #0
	beq	.L2311
	ldr	r3, [r0, #12]
	cmp	r3, #1
	bne	.L2312
	ldr	r7, .L2321+8
	mov	r0, #4
	ldr	r3, [r7, #12]
	blx	r3
	ldr	r3, [r6, r5, asl #2]
	ldr	r2, [r3, #20]
	cmp	r2, #1
	bne	.L2313
	movw	r0, #51240
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
	beq	.L2270
	ldr	r10, .L2321+12
	mov	r3, r5, asl #2
	str	r3, [fp, #-672]
	add	r3, r10, r3
	ldr	r0, [r3, #-3280]
	cmp	r0, #0
	beq	.L2271
	ldr	r3, [r4, #108]
	ldr	ip, [r10, #-3112]
	tst	r3, #28
	ldr	r3, [r4, #112]
	moveq	lr, #1
	movne	lr, #0
	cmp	ip, #0
	beq	.L2272
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
.L2271:
	ldr	r3, [fp, #-672]
	add	r10, r10, r3
	ldr	r2, [r10, #-2972]
	cmp	r2, #0
	beq	.L2270
	ldrd	r0, [r4, #232]
	orrs	r3, r0, r1
	beq	.L2270
	ldr	r3, [r4, #224]
	cmp	r3, #0
	bne	.L2314
.L2270:
	cmp	r9, #0
	bne	.L2315
.L2274:
	subs	r9, r8, #1
	mvnne	r9, #0
.L2276:
	mov	r0, #12
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L2287
.L2285:
	cmp	r9, #0
	beq	.L2288
.L2286:
	ldr	r3, [r6, r5, asl #2]
	mov	r0, #4
	ldr	r2, [r7, #16]
	add	r3, r3, #327680
	ldrb	r3, [r3, #1468]	@ zero_extendqisi2
	strb	r3, [r4, #472]
	blx	r2
.L2308:
	mov	r0, r9
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, r10, fp, sp, pc}
.L2315:
	ldr	r3, [r4, #200]
	mvn	r9, #0
	str	r9, [r4, #196]
	cmp	r3, #1
	bne	.L2274
	ldr	r2, [r4, #188]
	cmp	r2, #0
	beq	.L2316
	ldr	r0, [r4, #32]
	mov	r2, #3
	ldr	r1, [r4, #28]
	str	r9, [r4, #196]
	str	r3, [r4, #188]
	str	r0, [r4, #208]
	str	r1, [r4, #204]
	str	r2, [r4, #356]
.L2284:
	mov	r0, #12
	bl	CHECK_REC_POS_ENABLE
	cmp	r0, #0
	bne	.L2317
.L2288:
	ldr	r2, [r4, #108]
	mov	r9, #0
	ldr	r3, [r6, r5, asl #2]
	str	r2, [r3, #364]
	ldr	r2, [r4, #356]
	str	r2, [r3, #368]
	b	.L2286
.L2317:
	mov	r9, #0
	mov	r8, #1
.L2287:
	ldr	r3, [r7]
	blx	r3
	ldr	r3, .L2321+12
	ldr	r1, .L2321+16
	add	r3, r3, r5, lsl #2
	ldr	r2, [r3, #-2692]
	ldr	r1, [r1]
	rsb	r2, r2, r0
	mov	ip, r0
	cmp	r2, r1
	bls	.L2285
	mov	r2, r5, asl #24
	movw	r1, #4803
	add	r2, r2, #786432
	ldr	r0, .L2321+20
	cmp	r8, #1
	addeq	r2, r2, #1
	str	ip, [r3, #-2692]
	bl	DBG_AddTrace
	b	.L2285
.L2272:
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
	b	.L2271
.L2312:
	ldr	r3, .L2321+24
	movw	r2, #4573
	ldr	r1, .L2321+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r9, #0
	b	.L2308
.L2313:
	str	r2, [sp]
	movw	r3, #4607
	ldr	r2, .L2321+32
	mov	r0, #0
	ldr	r1, .L2321+36
	mvn	r9, #0
	bl	dprint_vfmw
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	b	.L2308
.L2314:
	mov	r1, r3
	ldr	r3, [r7, #44]
	blx	r3
	b	.L2270
.L2316:
	ldr	r3, [r6, r5, asl #2]
	movw	r0, #51240
	movt	r0, 4
	sub	r1, fp, #668
	add	r0, r3, r0
	str	r2, [fp, #-672]
	bl	SYNTAX_ReceiveImage
	ldr	r10, .L2321+4
	ldr	r2, [fp, #-672]
	cmp	r0, #1
	mov	r8, r0
	bne	.L2318
	ldr	r3, [r4, #192]
	ldr	r1, [fp, #-476]
	cmp	r3, r1
	bge	.L2279
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
	b	.L2276
.L2310:
	ldr	r3, .L2321+40
	movw	r2, #4569
	ldr	r1, .L2321+28
	mov	r0, #0
	bl	dprint_vfmw
	mvn	r9, #0
	b	.L2308
.L2318:
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #51240
	movt	r0, 4
	mov	r9, r8
	add	r0, r3, r0
	bl	SYNTAX_ReturnImage
	ldr	r3, [r7, #16]
	mov	r0, #4
	blx	r3
	b	.L2308
.L2279:
	bgt	.L2319
	cmp	r3, #0
	movw	r0, #51240
	ldr	r3, [r10, r5, asl #2]
	movt	r0, 4
	bne	.L2282
	add	r0, r3, r0
	bl	SYNTAX_ReturnImage
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #51240
	mov	r1, r4
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	mov	r0, r8
	ldr	r3, [r4, #192]
	mvn	r8, #1
	ldr	r2, .L2321+32
	ldr	r1, .L2321+44
	bl	dprint_vfmw
	b	.L2276
.L2282:
	add	r0, r3, r0
	mov	r1, r4
	bl	SYNTAX_ReleaseImage
	ldr	r3, [fp, #-480]
	cmp	r3, #0
	beq	.L2320
	ldr	r3, [r4, #192]
	mov	r0, r8
	ldr	r2, .L2321+32
	ldr	r1, .L2321+48
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
	b	.L2284
.L2311:
	ldr	r3, .L2321+52
	movw	r2, #4570
	ldr	r1, .L2321+28
	mvn	r9, #0
	bl	dprint_vfmw
	b	.L2308
.L2319:
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #51240
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReturnImage
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #51240
	mov	r1, r4
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	ldr	ip, [fp, #-476]
	ldr	r3, [r4, #192]
	mov	r0, r8
	ldr	r2, .L2321+32
	mvn	r8, #1
	ldr	r1, .L2321+56
	str	ip, [sp]
	bl	dprint_vfmw
	b	.L2276
.L2320:
	ldr	r3, [r10, r5, asl #2]
	movw	r0, #51240
	sub	r1, fp, #668
	movt	r0, 4
	add	r0, r3, r0
	bl	SYNTAX_ReleaseImage
	mov	r0, r8
	ldr	r3, [r4, #192]
	mvn	r8, #1
	ldr	r2, .L2321+32
	ldr	r1, .L2321+48
	bl	dprint_vfmw
	b	.L2276
.L2322:
	.align	2
.L2321:
	.word	.LC221
	.word	.LANCHOR0
	.word	vfmw_Osal_Func_Ptr_S
	.word	.LANCHOR4
	.word	g_TraceImgPeriod
	.word	.LANCHOR1+380
	.word	.LC38
	.word	.LC1
	.word	.LANCHOR1+360
	.word	.LC222
	.word	.LC12
	.word	.LC224
	.word	.LC225
	.word	.LC2
	.word	.LC223
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
	ldr	r3, .L2331
	ldr	r0, [r3, r0, asl #2]
	add	r0, r0, #311296
	ldr	r3, [r0, #2088]
	cmp	r3, #2
	beq	.L2325
	cmp	r3, #16
	beq	.L2326
	cmp	r3, #0
	beq	.L2330
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2330:
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageWidth
.L2326:
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageWidth
.L2325:
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageWidth
.L2332:
	.align	2
.L2331:
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
	ldr	r3, .L2341
	ldr	r0, [r3, r0, asl #2]
	add	r0, r0, #311296
	ldr	r3, [r0, #2088]
	cmp	r3, #2
	beq	.L2335
	cmp	r3, #16
	beq	.L2336
	cmp	r3, #0
	beq	.L2340
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.L2340:
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	H264DEC_GetImageHeight
.L2336:
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	HEVCDEC_GetImageHeight
.L2335:
	add	r0, r0, #2096
	sub	sp, fp, #12
	ldmia	sp, {fp, sp, lr}
	b	MPEG4DEC_GetImageHeight
.L2342:
	.align	2
.L2341:
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
	ldr	r3, .L2348
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	sub	r0, fp, #52
	ldr	r3, [r3, #48]
	mov	r2, #24
	mov	r1, #0
	blx	r3
	ldr	r3, .L2348+4
	ldr	r4, [r3, #52]
	cmp	r4, #0
	beq	.L2344
	ldr	ip, .L2348+8
	sub	r3, fp, #52
	str	r3, [sp, #4]
	mov	r2, r7
	mov	r1, r6
	mov	r0, r5
	ldr	ip, [ip, r5, asl #2]
	mov	r3, #0
	add	ip, ip, #311296
	ldr	ip, [ip, #2088]
	str	ip, [sp]
	blx	r4
.L2344:
	ldr	r0, [fp, #-48]
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L2349:
	.align	2
.L2348:
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
	ldr	r3, .L2351
	mov	r2, #1
	add	r0, r3, r0, lsl #2
	str	r2, [r0, #-2628]
	ldmfd	sp, {fp, sp, pc}
.L2352:
	.align	2
.L2351:
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
	ldr	r3, .L2354
	mov	r2, #0
	add	r0, r3, r0, lsl #2
	str	r2, [r0, #-2628]
	ldmfd	sp, {fp, sp, pc}
.L2355:
	.align	2
.L2354:
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
	ldr	r3, .L2357
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #-2628]
	ldmfd	sp, {fp, sp, pc}
.L2358:
	.align	2
.L2357:
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
	.type	__func__.52662, %object
	.size	__func__.52662, 18
__func__.52662:
	.ascii	"VCTRL_EventReport\000"
	.space	2
	.type	__func__.52674, %object
	.size	__func__.52674, 18
__func__.52674:
	.ascii	"VCTRL_SetCallBack\000"
	.space	2
	.type	__FUNCTION__.52780, %object
	.size	__FUNCTION__.52780, 29
__FUNCTION__.52780:
	.ascii	"VCTRL_PartitionMemForStdUser\000"
	.space	3
	.type	__func__.52804, %object
	.size	__func__.52804, 23
__func__.52804:
	.ascii	"VCTRL_FreeChanResource\000"
	.space	1
	.type	__FUNCTION__.53063, %object
	.size	__FUNCTION__.53063, 23
__FUNCTION__.53063:
	.ascii	"VCTRL_ReleaseChanImage\000"
	.space	1
	.type	__func__.53064, %object
	.size	__func__.53064, 23
__func__.53064:
	.ascii	"VCTRL_ReleaseChanImage\000"
	.space	1
	.type	__func__.53090, %object
	.size	__func__.53090, 18
__func__.53090:
	.ascii	"VCTRL_GetDecParam\000"
	.space	2
	.type	__FUNCTION__.53156, %object
	.size	__FUNCTION__.53156, 19
__FUNCTION__.53156:
	.ascii	"VCTRL_FlushDecoder\000"
	.space	1
	.type	__FUNCTION__.53487, %object
	.size	__FUNCTION__.53487, 18
__FUNCTION__.53487:
	.ascii	"GetExtraRefFrmNum\000"
	.space	2
	.type	__FUNCTION__.53493, %object
	.size	__FUNCTION__.53493, 19
__FUNCTION__.53493:
	.ascii	"GetExtraDispFrmNum\000"
	.space	1
	.type	__FUNCTION__.53499, %object
	.size	__FUNCTION__.53499, 16
__FUNCTION__.53499:
	.ascii	"GetExtraCtrlCmd\000"
	.type	__FUNCTION__.53419, %object
	.size	__FUNCTION__.53419, 14
__FUNCTION__.53419:
	.ascii	"DecChanSyntax\000"
	.space	2
	.type	__func__.52981, %object
	.size	__func__.52981, 24
__func__.52981:
	.ascii	"VCTRL_FindOccupiedFrame\000"
	.type	__func__.53477, %object
	.size	__func__.53477, 24
__func__.53477:
	.ascii	"VCTRL_ExtAccGetDecParam\000"
	.type	__func__.53734, %object
	.size	__func__.53734, 30
__func__.53734:
	.ascii	"VCTRL_SetSingleFrameBufToChan\000"
	.space	2
	.type	__func__.53753, %object
	.size	__func__.53753, 24
__func__.53753:
	.ascii	"VCTRL_AllocMemToDecoder\000"
	.type	__func__.53045, %object
	.size	__func__.53045, 19
__func__.53045:
	.ascii	"VCTRL_GetChanImage\000"
	.space	1
	.type	__FUNCTION__.53053, %object
	.size	__FUNCTION__.53053, 19
__FUNCTION__.53053:
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
	ASCII(.ascii	"%s,%d,actul size = 0x%x,except size= 0x%x\012\000" )
	.space	1
.LC216:
	ASCII(.ascii	"%s,%d,configuredFrameNum = %x,PmvNodeStatus= 0x%x\012" )
	ASCII(.ascii	"\000" )
	.space	1
.LC217:
	ASCII(.ascii	"DFS, request frame store fail!, chanid: %d\012\000" )
.LC218:
	ASCII(.ascii	"DFS, set decoding thread state to success!\012 \000" )
	.space	3
.LC219:
	ASCII(.ascii	"%s set chan %d ctrl state CHAN_FRAME_STORE_ALLOC fa" )
	ASCII(.ascii	"iled!\012\000" )
	.space	2
.LC220:
	ASCII(.ascii	"VFMW: WriteBigTileYuv %dx%d...\012\000" )
.LC221:
	ASCII(.ascii	"call VCTRL_GetChanImage, ChanID = %d,\012\000" )
	.space	1
.LC222:
	ASCII(.ascii	"%s,%d,ImgOutputEn = %d, should not be output!\012\000" )
	.space	1
.LC223:
	ASCII(.ascii	"%s: view_0 id(%d) > view_1 id(%d), error!\012\000" )
	.space	1
.LC224:
	ASCII(.ascii	"%s: continuous view0 id(%d)!\012\000" )
	.space	2
.LC225:
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
	.type	FspInstInfo.52776, %object
	.size	FspInstInfo.52776, 3212
FspInstInfo.52776:
	.space	3212
	.type	g_VdmCharacter, %object
	.size	g_VdmCharacter, 4
g_VdmCharacter:
	.space	4
	.type	last_rec_pos_time.53060, %object
	.size	last_rec_pos_time.53060, 64
last_rec_pos_time.53060:
	.space	64
	.type	extra_ref, %object
	.size	extra_ref, 4
extra_ref:
	.space	4
	.type	extra_disp, %object
	.size	extra_disp, 4
extra_disp:
	.space	4
	.type	MemArrange.53274, %object
	.size	MemArrange.53274, 1272
MemArrange.53274:
	.space	1272
	.type	MemArrange.53308, %object
	.size	MemArrange.53308, 1272
MemArrange.53308:
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
	.type	last_rec_pos_time.53413, %object
	.size	last_rec_pos_time.53413, 64
last_rec_pos_time.53413:
	.space	64
	.type	last_rec_pos_decparam_time.53414, %object
	.size	last_rec_pos_decparam_time.53414, 64
last_rec_pos_decparam_time.53414:
	.space	64
	.type	ChanPriTab.53374, %object
	.size	ChanPriTab.53374, 64
ChanPriTab.53374:
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
	.type	PrioArray.52876, %object
	.size	PrioArray.52876, 64
PrioArray.52876:
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
	.type	Capability.52920, %object
	.size	Capability.52920, 148
Capability.52920:
	.space	148
	.type	PrioArray.52919, %object
	.size	PrioArray.52919, 64
PrioArray.52919:
	.space	64
	.type	g_CurProcChan, %object
	.size	g_CurProcChan, 4
g_CurProcChan:
	.space	4
	.type	save_file_cnt.53538, %object
	.size	save_file_cnt.53538, 4
save_file_cnt.53538:
	.space	4
	.type	save_str_chan_num, %object
	.size	save_str_chan_num, 4
save_str_chan_num:
	.space	4
	.type	vfmw_save_seg_file, %object
	.size	vfmw_save_seg_file, 128
vfmw_save_seg_file:
	.space	128
	.type	save_file_cnt.53541, %object
	.size	save_file_cnt.53541, 4
save_file_cnt.53541:
	.space	4
	.type	save_seg_chan_num, %object
	.size	save_seg_chan_num, 4
save_seg_chan_num:
	.space	4
	.type	vfmw_save_yuv_file, %object
	.size	vfmw_save_yuv_file, 128
vfmw_save_yuv_file:
	.space	128
	.type	save_file_cnt.53546, %object
	.size	save_file_cnt.53546, 4
save_file_cnt.53546:
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
	.type	save_file_cnt.53559, %object
	.size	save_file_cnt.53559, 4
save_file_cnt.53559:
	.space	4
	.type	save_metadata_chan_num, %object
	.size	save_metadata_chan_num, 4
save_metadata_chan_num:
	.space	4
	.type	vfmw_save_metadata_reorder_file, %object
	.size	vfmw_save_metadata_reorder_file, 128
vfmw_save_metadata_reorder_file:
	.space	128
	.type	save_file_cnt.53562, %object
	.size	save_file_cnt.53562, 4
save_file_cnt.53562:
	.space	4
	.type	save_metadata_reorder_chan_num, %object
	.size	save_metadata_reorder_chan_num, 4
save_metadata_reorder_chan_num:
	.space	4
	.type	vfmw_save_el_file, %object
	.size	vfmw_save_el_file, 128
vfmw_save_el_file:
	.space	128
	.type	save_file_cnt.53565, %object
	.size	save_file_cnt.53565, 4
save_file_cnt.53565:
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
	.type	last_rec_pos_time.53050, %object
	.size	last_rec_pos_time.53050, 64
last_rec_pos_time.53050:
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
